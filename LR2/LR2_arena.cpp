#include "LR2_arena.h"

#include "arena_backend.h"
#include "arena_imgui.h"
#include "arena_gui_util.h"
#include "arena_keys.h"
#include "arena_maniac.h"
#include "En_timer.h"
#include "LR2_customir.h"
#include "LR2_songmanage.h"
#include "structure.h"

#include <DxLib.h>
#include <ImGuiNotify.hpp>
#include <sqlite3.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>

// cstring: MutePlayButtons memset keyboard+MIDI inputID

namespace openlr2::arena {
namespace {

game* gGame{};
bool menuVisible{};
bool graphVisible{}; // showGraph default false
bool automaticGraph{true}; // automaticGraph default true
bool itemModeEnabled{};
/** When true, Insert menu blocks ReactInput / play buttons (default ON). */
bool muteGameInputs{true};
bool randomFlip{}; // random_flip
char roomIdInput[64]{};
char lanHostInput[128]{}; // LAN host buffer
char lanUserInput[128]{}; // LAN username buffer
char chatInput[257]{}; // 256 chars + NUL (UTF-8 bytes; ImGui InputText)
openlr2::ArenaRoomState cached{};
int forcedSeed{};
bool hasForcedSeed{};
bool matchFinished{};
bool matchInProgress{}; // chart selected this round (cancel only from in-play return)
std::string lastArmedChartHash{};
int lastArmedRandomSeed{};
std::string lastMissingCheckHash{};
int lastMissingCheckSeed{};
/** Last room chart hash we finished handling for guest select search (hit or miss). */
std::string lastGuestSearchHash{};
int lastGuestSearchSeed{};
/** Hash waiting for song-select (`procSelecter == 2`) to arm searchType=3. */
std::string pendingGuestSearchHash{};
sqlite3* songDb{};
int prevMouseDispFlag{-1};
std::size_t lastToastChatCount{};
int prevProcSelecter{-1};
bool startupToastShown{};
bool wasConnected{};
int cachedChartMaxEx{}; // chart max EX
int lastOjamaTickMs{-1};

void RefreshState(game* g) {
	if (g == nullptr) {
		cached = {};
		return;
	}
	// PollState drains IR one-shot queues into `out`. Preserve any undrained
	// local pending so a second poll in the same frame (e.g. BestOpponentEx)
	// cannot drop Item / ItemSettings before DrainPendingItems.
	auto keepItems = cached.pendingItems;
	const bool keepSettings = cached.itemSettings.updated;
	const auto keepSettingsBody = cached.itemSettings;
	openlr2::arena_backend::Active().PollState(g, cached);
	if (!keepItems.empty()) {
		cached.pendingItems.insert(
			cached.pendingItems.end(), keepItems.begin(), keepItems.end()
		);
	}
	if (keepSettings && !cached.itemSettings.updated) {
		cached.itemSettings = keepSettingsBody;
		cached.itemSettings.updated = true;
	}
}

std::string JoinTitleSubtitle(const char* title, const char* subtitle) {
	std::string out = title && title[0] != '\0' ? title : "";
	if (subtitle != nullptr && subtitle[0] != '\0') {
		if (!out.empty()) out.push_back(' ');
		out += subtitle;
	}
	return out;
}

openlr2::ArenaPeerScore SnapshotScore(game* g) {
	openlr2::ArenaPeerScore s{};
	if (g == nullptr) return s;
	const auto& p = g->gameplay.player[PLAYER_1];
	// judgecount: 0 unknown, 1 poor, 2 bad, 3 good, 4 great, 5 pgreat
	s.poor = p.judgecount[1];
	s.bad = p.judgecount[2];
	s.good = p.judgecount[3];
	s.great = p.judgecount[4];
	s.pgreat = p.judgecount[5];
	s.maxCombo = p.max_combo;
	s.score = p.score;
	s.currentNotes = p.note_current;
	return s;
}

} // namespace

std::string FormatChartDisplayArtist(game* g) {
	// artist + " " + subartist.
	if (g == nullptr) return {};
	std::string artist;
	std::string sub;
	if (g->sSelect.metaSelected.artist.body && g->sSelect.metaSelected.artist.body[0] != '\0') {
		artist = g->sSelect.metaSelected.artist.body;
	}
	if (g->sSelect.metaSelected.subartist.body && g->sSelect.metaSelected.subartist.body[0] != '\0') {
		sub = g->sSelect.metaSelected.subartist.body;
	}
	if (artist.empty() && g->sSelect.cur_song >= 0) {
		const CSTR& a = g->sSelect.bmsList[g->sSelect.cur_song].artist;
		if (a.body && a.body[0] != '\0') artist = a.body;
		const CSTR& s = g->sSelect.bmsList[g->sSelect.cur_song].subartist;
		if (s.body && s.body[0] != '\0') sub = s.body;
	}
	if (artist.empty()) return sub;
	if (sub.empty()) return artist;
	return artist + " " + sub;
}

std::string LookupChartRelativePath(game* g, const std::string& hash) {
	// folder\file relative to LR2 root when possible.
	if (hash.empty() || g == nullptr) return {};
	CSTR path{};
	if (songDb != nullptr) {
		SONGDATA song{};
		if (GetSongData(hash.c_str(), &song, songDb, &g->sSelect) == 1) {
			if (song.filepath.body && song.filepath.body[0] != '\0') {
				path = song.filepath;
			}
		}
	}
	if ((!path.body || path.body[0] == '\0') && g->sSelect.bmsList != nullptr) {
		for (int i = 0; i < g->sSelect.bmsListCount; ++i) {
			const CSTR& h = g->sSelect.bmsList[i].hash;
			if (h.body && hash == h.body) {
				path = g->sSelect.bmsList[i].filepath;
				break;
			}
		}
	}
	if (!path.body || path.body[0] == '\0') return {};
	std::string full = path.body;
	// Prefer folder\filename when both are available (AX u8string relative).
	for (int i = 0; i < g->sSelect.bmsListCount; ++i) {
		const CSTR& h = g->sSelect.bmsList[i].hash;
		if (!(h.body && hash == h.body)) continue;
		const CSTR& folder = g->sSelect.bmsList[i].folder;
		if (folder.body && folder.body[0] != '\0') {
			const char* slash = std::strrchr(full.c_str(), '\\');
			const char* slash2 = std::strrchr(full.c_str(), '/');
			const char* base = slash;
			if (slash2 != nullptr && (base == nullptr || slash2 > base)) base = slash2;
			if (base != nullptr && base[1] != '\0') {
				return std::string(folder.body) + "\\" + (base + 1);
			}
		}
		break;
	}
	return full;
}

std::string FormatChartDisplayTitle(game* g) {
	if (g == nullptr) return {};
	if (g->sSelect.cur_song >= 0) {
		const auto& song = g->sSelect.bmsList[g->sSelect.cur_song];
		if (song.fulltitle.body && song.fulltitle.body[0] != '\0') {
			return song.fulltitle.body;
		}
		const std::string joined = JoinTitleSubtitle(
			song.title.body ? song.title.body : "",
			song.subtitle.body ? song.subtitle.body : ""
		);
		if (!joined.empty()) return joined;
	}
	return JoinTitleSubtitle(
		g->sSelect.metaSelected.title.body ? g->sSelect.metaSelected.title.body : "",
		g->sSelect.metaSelected.subtitle.body ? g->sSelect.metaSelected.subtitle.body : ""
	);
}

void Init(game* g, sqlite3* db) {
	gGame = g;
	songDb = db;
	menuVisible = false;
	graphVisible = false;
	automaticGraph = openlr2::arena_gui::LoadAutomaticGraph();
	itemModeEnabled = false;
	muteGameInputs = openlr2::arena_gui::LoadMuteGameInputs();
	randomFlip = openlr2::arena_gui::LoadRandomFlip();
	openlr2::arena_keys::Load();
	openlr2::arena_backend::LoadTransportFromIni();
	openlr2::arena_maniac::SetupDefaults();
	openlr2::arena_maniac::SetSfxVolume(openlr2::arena_gui::LoadItemSfxVolume());
	openlr2::arena_gui::LoadItemSfxPathsIntoManiac();
	if (g != nullptr) openlr2::arena_maniac::BindGame(g);
	roomIdInput[0] = '\0';
	lanHostInput[0] = '\0';
	lanUserInput[0] = '\0';
	// load LAN host / username (empty if unset).
	openlr2::arena_gui::LoadLanHost(lanHostInput, sizeof(lanHostInput));
	openlr2::arena_gui::LoadLanUsername(lanUserInput, sizeof(lanUserInput));
	chatInput[0] = '\0';
	cached = {};
	forcedSeed = 0;
	hasForcedSeed = false;
	matchFinished = false;
	matchInProgress = false;
	lastArmedChartHash.clear();
	lastArmedRandomSeed = 0;
	lastMissingCheckHash.clear();
	lastMissingCheckSeed = 0;
	lastToastChatCount = 0;
	prevMouseDispFlag = -1;
	prevProcSelecter = -1;
	startupToastShown = false;
	wasConnected = false;
	cachedChartMaxEx = 0;
	lastOjamaTickMs = -1;
	openlr2::arena_imgui::Init();
}

void SetSongDb(sqlite3* db) {
	songDb = db;
}

void Shutdown() {
	openlr2::arena_maniac::ShutdownSfx();
	if (gGame != nullptr) {
		Leave(gGame);
	}
	openlr2::arena_imgui::Shutdown();
	gGame = nullptr;
	songDb = nullptr;
}

bool HaveLocalChart(game* g, const std::string& hash) {
	if (hash.empty() || g == nullptr) return false;
	if (songDb != nullptr) {
		SONGDATA song{};
		if (GetSongData(hash.c_str(), &song, songDb, &g->sSelect) == 1) {
			return true;
		}
	}
	for (int i = 0; i < g->sSelect.bmsListCount; ++i) {
		const CSTR& h = g->sSelect.bmsList[i].hash;
		if (h.body && hash == h.body) return true;
	}
	return false;
}

void MaybeCheckMissingChart(game* g) {
	if (g == nullptr || cached.chartHash.empty()) return;
	// notify on every host select (hash or seed change).
	if (cached.chartHash == lastMissingCheckHash && cached.randomSeed == lastMissingCheckSeed) {
		return;
	}
	lastMissingCheckHash = cached.chartHash;
	lastMissingCheckSeed = cached.randomSeed;
	if (HaveLocalChart(g, cached.chartHash)) return;
	openlr2::arena_backend::Active().SendMissingChart(g);
	// "[!] You do not have this chart!"
	Toast(ToastKind::Error, "You do not have this chart!");
}

void DrainPendingItems(game* g) {
	for (const auto& p : cached.pendingItems) {
		openlr2::arena_maniac::CurrentItem item{};
		item.rolledItemId = p.rolledItemId;
		item.level = static_cast<unsigned short>(p.level);
		// SFX only on receive (no toast).
		openlr2::arena_maniac::TriggerItem(g, item);
	}
	cached.pendingItems.clear();
	if (cached.itemSettings.updated) {
		openlr2::arena_maniac::ItemSettingsPayload payload{};
		payload.threshold = cached.itemSettings.threshold;
		for (const auto& r : cached.itemSettings.settings) {
			payload.settings.push_back({r.lv1, r.lv2, r.lv3, r.weight});
		}
		openlr2::arena_maniac::ApplyItemSettings(payload);
		cached.itemSettings.updated = false;
		// no toast on settings sync.
	}
}

void TryUseHeldItem(game* g) {
	if (g == nullptr) return;
	if (openlr2::arena_maniac::HeldItem().rolledItemId < 0) return;
	// SFX on send; no success toast.
	(void)openlr2::arena_maniac::UseItem(g);
}

void ToastNewChatIfHidden() {
	if (menuVisible) {
		lastToastChatCount = cached.chatLog.size();
		return;
	}
	// Plugin caps chatLog; if truncated, unstick the index.
	if (cached.chatLog.size() < lastToastChatCount) {
		lastToastChatCount = 0;
	}
	while (lastToastChatCount < cached.chatLog.size()) {
		const auto& line = cached.chatLog[lastToastChatCount++];
		if (line.empty()) continue;
		// system [#]/[!] only; user lines: title=user, content=msg.
		if (line.rfind("[!] ", 0) == 0) {
			Toast(ToastKind::Error, line.c_str() + 4);
		} else if (line.rfind("[#] ", 0) == 0) {
			Toast(ToastKind::Info, line.c_str() + 4);
		} else if (line.rfind("[", 0) == 0) {
			// [chart], [arena error], … — not toasted on system path.
			continue;
		} else {
			const auto colon = line.find(": ");
			if (colon != std::string::npos && colon > 0 && colon < 64) {
				if (!openlr2::arena_imgui::IsAvailable()) continue;
				ImGuiToast toast(ImGuiToastType::Info, 3000);
				toast.setTitle("%.*s", static_cast<int>(colon), line.c_str());
				toast.setContent("%s", line.c_str() + colon + 2);
				ImGui::InsertNotification(toast);
			}
		}
	}
}

bool IsSelfHost() {
	return cached.connected && cached.selfPlayerId != 0 && cached.selfPlayerId == cached.hostId;
}

/** Arm song-select text search for an exact hash (guest only). */
void ArmGuestHashSearch(game* g, const std::string& hash) {
	if (g == nullptr || hash.empty()) return;

	char qbuf[1024]{};
	sqlite3_snprintf(
		1024,
		qbuf,
		"SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE song.hash = '%q'",
		hash.c_str()
	);

	g->config.select.difficulty = 0;
	g->sSelect.searchFocused = 1;
	g->sSelect.filterDifficulty = 0;
	g->sSelect.filterKey = g->config.select.key;
	g->sSelect.curQuery[0] = qbuf;
	g->sSelect.queryCount = 1;
	g->sSelect.searchType = 3;
	g->sSelect.searchMax = g->config.select.searchmax;
	g->sSelect.filterSort = g->config.select.sort;
	g->sSelect.unk4fb8[0] = 0;
	if (g->sSelect.cur_song >= 0 && g->sSelect.bmsList != nullptr) {
		// CSTR::operator=(CSTR&) needs non-const refs (same as Scene02_Songselect).
		g->sSelect.selTitle = g->sSelect.bmsList[g->sSelect.cur_song].title;
		g->sSelect.selFilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
		g->sSelect.selFolder = g->sSelect.bmsList[g->sSelect.cur_song].folder;
	}
	g->sSelect.searchInput = hash.c_str();
	// dwGame[7045] = 30 (search-bar text slot).
	g->sSelect.text_num = 30;
}

void MaybeGuestHashSearch(game* g) {
	if (g == nullptr || !cached.connected || IsSelfHost()) {
		pendingGuestSearchHash.clear();
		return;
	}
	if (cached.chartHash.empty()) {
		pendingGuestSearchHash.clear();
		return;
	}

	// re-run on every selected-chart message (hash or seed change).
	if (cached.chartHash != lastGuestSearchHash || cached.randomSeed != lastGuestSearchSeed) {
		lastGuestSearchSeed = cached.randomSeed;
		if (!HaveLocalChart(g, cached.chartHash)) {
			lastGuestSearchHash = cached.chartHash;
			pendingGuestSearchHash.clear();
			return;
		}
		pendingGuestSearchHash = cached.chartHash;
	}

	if (pendingGuestSearchHash.empty()) return;
	if (g->procSelecter != 2) return;
	// Do not interrupt an in-flight select search (1→2→4→0).
	if (g->sSelect.searchFocused != 0) return;

	ArmGuestHashSearch(g, pendingGuestSearchHash);
	lastGuestSearchHash = pendingGuestSearchHash;
	pendingGuestSearchHash.clear();
}

void DrainCharBuffer() {
	while (GetInputChar(TRUE) != 0) {
	}
}

bool IsMenuBlockingInput() {
	// mute only (no soft WantCapture when mute OFF).
	return menuVisible && muteGameInputs;
}

bool WantsOsCursor() {
	// OS cursor: menu || graph.
	return openlr2::arena_imgui::IsAvailable() && (menuVisible || graphVisible);
}

void MutePlayButtonsIfMenuOpen(game* g) {
	if (g == nullptr || !menuVisible || !muteGameInputs) return;
	// zero game-facing controls AFTER bind sampling.
	// Do not wipe inputID here — TickUi EdgeHit/PollNewPress still need raw KB/MIDI/pad
	// (AX reads GetDeviceState first, then clears). Play is blocked via buttons + WndProc.
	for (int i = 0; i < 20; ++i) {
		g->KeyInput.p1_buttonInput[i] = 0;
		g->KeyInput.p2_buttonInput[i] = 0;
	}
	g->KeyInput.mouse_buttonL = 0;
	g->KeyInput.mouse_buttonR = 0;
	g->KeyInput.mouse_buttonW = 0;
	g->KeyInput.mousewheel = 0;
	DrainCharBuffer();
}

void TickUi(game* g) {
	if (g == nullptr) return;

	const bool imguiOk = openlr2::arena_imgui::IsAvailable();
	using openlr2::arena_keys::Binding;

	// hide graph on select when automaticGraph.
	if (automaticGraph && g->procSelecter == 2 && prevProcSelecter != 2) {
		graphVisible = false;
	}
	prevProcSelecter = g->procSelecter;

	// first ProcSelect — startup toast.
	if (!startupToastShown && imguiOk && g->procSelecter == 2) {
		startupToastShown = true;
		char msg[192];
		std::snprintf(
			msg,
			sizeof(msg),
			"OpenLR2 Arena successfully loaded; press %s to show the overlay!",
			openlr2::arena_keys::ToString(openlr2::arena_keys::Get(Binding::MenuToggle)).c_str()
		);
		Toast(ToastKind::Success, msg);
	}

	const bool waitingBind = openlr2::arena_imgui::IsWaitingForKeyBind();
	{
		static int prevMenu{};
		if (!waitingBind && openlr2::arena_keys::EdgeHit(g, Binding::MenuToggle, prevMenu)) {
			menuVisible = !menuVisible;
			DrainCharBuffer();
			if (!menuVisible) {
				chatInput[0] = '\0';
			}
		} else if (waitingBind) {
			(void)openlr2::arena_keys::EdgeHit(g, Binding::MenuToggle, prevMenu);
		}
	}
	{
		static int prevGraph{};
		if (!waitingBind && openlr2::arena_keys::EdgeHit(g, Binding::GraphToggle, prevGraph)) {
			graphVisible = !graphVisible;
		} else if (waitingBind) {
			(void)openlr2::arena_keys::EdgeHit(g, Binding::GraphToggle, prevGraph);
		}
	}
	{
		static int prevItem{};
		if (!waitingBind && openlr2::arena_keys::EdgeHit(g, Binding::ItemTrigger, prevItem)) {
			// item mode only when self is ready
			bool selfReady = false;
			for (const auto& p : cached.players) {
				if (p.playerId == cached.selfPlayerId) {
					selfReady = p.ready;
					break;
				}
			}
			if (IsConnected(g) && openlr2::arena_maniac::IsItemModeEnabled() && selfReady) {
				TryUseHeldItem(g);
			}
		} else if (waitingBind) {
			(void)openlr2::arena_keys::EdgeHit(g, Binding::ItemTrigger, prevItem);
		}
	}

	// Cursor while Arena windows are up (ImGui needs a visible mouse).
	if (imguiOk && (menuVisible || graphVisible)) {
		if (prevMouseDispFlag < 0) {
			prevMouseDispFlag = GetMouseDispFlag();
		}
		SetMouseDispFlag(TRUE);
	} else if (prevMouseDispFlag >= 0) {
		SetMouseDispFlag(prevMouseDispFlag);
		prevMouseDispFlag = -1;
	}

	// While hidden, keep draining the char buffer so reopen does not dump queued keys.
	if (!menuVisible) {
		DrainCharBuffer();
	}

	// One poll per frame so pendingItems / chat are not drained twice.
	if (menuVisible || graphVisible || IsConnected(g) || imguiOk) {
		RefreshState(g);
	}

	const bool connectedNow = IsConnected(g);
	if (wasConnected && !connectedNow) {
		// disconnect: toast + reset maniac/seed.
		Toast(ToastKind::Info, "Disconnected from the server");
		openlr2::arena_maniac::ResetMatch();
		hasForcedSeed = false;
		forcedSeed = 0;
		matchInProgress = false;
		matchFinished = false;
		cachedChartMaxEx = 0;
	}
	wasConnected = connectedNow;

	if (connectedNow) {
		MaybeCheckMissingChart(g);
		MaybeGuestHashSearch(g);
		DrainPendingItems(g);
		ToastNewChatIfHidden();
		// single itemModeEnabled from host chart select (guests follow room).
		const bool host = cached.selfPlayerId != 0 && cached.selfPlayerId == cached.hostId;
		openlr2::arena_maniac::SetItemModeEnabled(
			host ? itemModeEnabled : cached.itemModeEnabled
		);
		// item timer: wall-clock ms (not frame*16).
		if (g->procSelecter == 4) {
			const int nowMs = static_cast<int>(GetTimeWrap());
			int dt = 16;
			if (lastOjamaTickMs >= 0) {
				dt = nowMs - lastOjamaTickMs;
				if (dt < 1) dt = 1;
				if (dt > 100) dt = 100;
			}
			lastOjamaTickMs = nowMs;
			openlr2::arena_maniac::TickActive(g, dt);
		} else {
			lastOjamaTickMs = -1;
		}
		// Arm forced seed when host chart hash *or* seed changes (same-hash rematch).
		if (!cached.chartHash.empty() && cached.randomSeed != 0
			&& (cached.chartHash != lastArmedChartHash
				|| cached.randomSeed != lastArmedRandomSeed)) {
			forcedSeed = cached.randomSeed;
			hasForcedSeed = true;
			lastArmedChartHash = cached.chartHash;
			lastArmedRandomSeed = cached.randomSeed;
			matchFinished = false;
		}
		if (g->procSelecter == 4) {
			// Late fill max EX once BMS notes are known.
			if (cachedChartMaxEx <= 0) {
				int notes = g->gameplay.player[PLAYER_1].totalnotes;
				if (notes <= 0) notes = g->gameplay.player[PLAYER_1].total_note;
				if (notes > 0) cachedChartMaxEx = notes * 2;
			}
			// Pacemaker always write (incl. 0 / alone = own EX).
			g->gameplay.targetScore.exscore = BestOpponentEx(g);
		}
	}

	// always NewFrame path when ImGui available (toasts with windows closed).
	if (imguiOk) {
		openlr2::arena_imgui::DrawFrame(
			g,
			cached,
			menuVisible,
			graphVisible,
			automaticGraph,
			muteGameInputs,
			itemModeEnabled,
			roomIdInput,
			sizeof(roomIdInput),
			chatInput,
			sizeof(chatInput),
			&JoinRoom,
			&Leave
		);
	}
}

void SetTransportIr(game* g) {
	if (openlr2::arena_backend::GetTransport() == openlr2::arena_backend::Transport::Ir) return;
	if (g != nullptr && openlr2::arena_backend::Active().IsConnected(g)) {
		openlr2::arena_backend::Active().Leave(g);
	}
	openlr2::arena_backend::Lan().StopLanServer();
	openlr2::arena_backend::SetTransport(openlr2::arena_backend::Transport::Ir);
	openlr2::arena_backend::SaveTransportToIni();
	cached = {};
}

void SetTransportLan(game* g) {
	if (openlr2::arena_backend::GetTransport() == openlr2::arena_backend::Transport::Lan) return;
	if (g != nullptr && openlr2::arena_backend::Active().IsConnected(g)) {
		openlr2::arena_backend::Active().Leave(g);
	}
	openlr2::arena_backend::SetTransport(openlr2::arena_backend::Transport::Lan);
	openlr2::arena_backend::SaveTransportToIni();
	cached = {};
}

bool IsTransportLan() {
	return openlr2::arena_backend::GetTransport() == openlr2::arena_backend::Transport::Lan;
}

char* RoomIdInputBuf() { return roomIdInput; }
char* LanHostInputBuf() { return lanHostInput; }
char* LanUserInputBuf() { return lanUserInput; }
size_t RoomIdInputCap() { return sizeof(roomIdInput); }
size_t LanHostInputCap() { return sizeof(lanHostInput); }
size_t LanUserInputCap() { return sizeof(lanUserInput); }

bool ConnectLan(game* g, const char* host, const char* username) {
	if (g == nullptr || host == nullptr || host[0] == '\0' || username == nullptr || username[0] == '\0') {
		return false;
	}
	SetTransportLan(g);
	if (!openlr2::arena_backend::Lan().ConnectLan(g, host, username)) {
		Toast(ToastKind::Error, "LAN connect failed");
		return false;
	}
	RefreshState(g);
	if (cached.connected) {
		openlr2::arena_gui::SaveLanConnectCredentials(host, username);
		DrainCharBuffer();
		lastToastChatCount = cached.chatLog.size();
		Toast(ToastKind::Success, "Connected to LAN arena");
	}
	return cached.connected;
}

bool StartLanServer(game* g) {
	SetTransportLan(g);
	if (!openlr2::arena_backend::Lan().StartLanServer("0.0.0.0", 2222)) {
		Toast(ToastKind::Error, "Failed to start LAN server on :2222");
		return false;
	}
	Toast(ToastKind::Success, "LAN server listening on :2222");
	return true;
}

void StopLanServer(game* g) {
	(void)g;
	openlr2::arena_backend::Lan().StopLanServer();
	Toast(ToastKind::Info, "LAN server stopped");
}

bool IsLanServerRunning() {
	return openlr2::arena_backend::Lan().IsLanServerRunning();
}

bool JoinRoom(game* g, const char* roomId) {
	if (g == nullptr || roomId == nullptr || roomId[0] == '\0') return false;
	SetTransportIr(g);
	openlr2::ArenaJoinResult out{};
	if (openlr2::arena_backend::Active().JoinRoom(g, roomId, out) != openlr2::GetStatus::Ok) {
		Toast(ToastKind::Error, "Failed to join room");
		return false;
	}
	RefreshState(g);
	if (cached.connected) {
		// Mask: clear typed code after successful join (UI shows ******).
		roomIdInput[0] = '\0';
		DrainCharBuffer();
		lastToastChatCount = cached.chatLog.size();
		Toast(ToastKind::Success, "Joined arena room");
	}
	return cached.connected;
}

void Leave(game* g) {
	if (g != nullptr) openlr2::arena_backend::Active().Leave(g);
	openlr2::arena_maniac::ResetMatch();
	cached = {};
	hasForcedSeed = false;
	forcedSeed = 0;
	matchFinished = false;
	matchInProgress = false;
	lastArmedChartHash.clear();
	lastArmedRandomSeed = 0;
	lastMissingCheckHash.clear();
	lastMissingCheckSeed = 0;
	lastGuestSearchHash.clear();
	lastGuestSearchSeed = 0;
	pendingGuestSearchHash.clear();
	lastToastChatCount = 0;
	cachedChartMaxEx = 0;
	lastOjamaTickMs = -1;
	roomIdInput[0] = '\0';
	chatInput[0] = '\0';
	DrainCharBuffer();
	Toast(ToastKind::Info, "Left arena room");
}

bool IsConnected(game* g) {
	return g != nullptr && openlr2::arena_backend::Active().IsConnected(g);
}

bool IsInMatch(game* g) {
	return IsConnected(g);
}

const openlr2::ArenaRoomState& State(game* g) {
	RefreshState(g);
	return cached;
}

bool IsDemoSamplePath(const char* path) {
	if (path == nullptr || path[0] == '\0') return false;
	// skip LR2files\Config\sample_* skin-preview charts.
	if (std::strncmp(path, "LR2files\\Config\\sample_", 23) == 0) return true;
	if (std::strncmp(path, "LR2files/Config/sample_", 23) == 0) return true;
	return false;
}

void OnChartSelected(game* g, const char* hash, const char* title, const char* artist, unsigned option, unsigned gauge) {
	if (g == nullptr || !openlr2::arena_backend::Active().IsConnected(g)) return;
	if (hash == nullptr || hash[0] == '\0') return;
	const char* path = nullptr;
	if (g->sSelect.metaSelected.filepath.body && g->sSelect.metaSelected.filepath.body[0] != '\0') {
		path = g->sSelect.metaSelected.filepath.body;
	} else if (g->directoryPath.body && g->directoryPath.body[0] != '\0') {
		path = g->directoryPath.body;
	}
	if (IsDemoSamplePath(path)) return;

	RefreshState(g);
	matchFinished = false;
	matchInProgress = true;
	const bool host = cached.selfPlayerId == cached.hostId;

	// pacemaker reset + maniac ResetState on chart select.
	g->gameplay.targetScore.exscore = 0;
	openlr2::arena_maniac::ResetState();
	{
		int notes = g->gameplay.player[PLAYER_1].totalnotes;
		if (notes <= 0) notes = g->gameplay.player[PLAYER_1].total_note;
		if (notes <= 0 && g->sSelect.cur_song >= 0) {
			notes = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
		}
		cachedChartMaxEx = notes > 0 ? notes * 2 : 0;
	}

	// non-host selecting a different chart → warn.
	if (!host && !cached.chartHash.empty() && cached.chartHash != hash) {
		Toast(ToastKind::Error, "You are not the host; please go back to the main menu and select the same song as the host!");
	}

	// send whatever seed LR2 will use (roll only if still 0).
	int seed = g->gameplay.randomseed;
	if (seed == 0) {
		seed = GetRand(0x7ffe);
		if (seed == 0) seed = 1;
		g->gameplay.randomseed = seed;
	}

	// show graph on chart select when automaticGraph.
	if (automaticGraph) {
		graphVisible = true;
	}

	openlr2::arena_backend::Active().SendSelectedChart(g, 
		seed,
		hash,
		title != nullptr ? title : "",
		artist != nullptr ? artist : "",
		option,
		gauge,
		host ? itemModeEnabled : cached.itemModeEnabled
	);
}

int TakeForcedRandomSeed(game* g) {
	if (g == nullptr) return 0;
	RefreshState(g);
	// Host uses LR2 seed path; guests consume room seed while armed.
	if (cached.connected && cached.selfPlayerId == cached.hostId) return 0;
	if (!hasForcedSeed) return 0;
	return forcedSeed;
}

void OnLoadingCompleteWaitReady(game* g) {
	if (g == nullptr || !openlr2::arena_backend::Active().IsConnected(g)) return;
	// skip ready if already returning to menu (Esc / close).
	if (g->gameplay.flag_closingPhase) return;
	// order: send ready, then toast, then wait.
	openlr2::arena_backend::Active().SendReady(g);
	Toast(ToastKind::Info, "Loading complete, waiting for all players...");
	for (;;) {
		// OpenLR2 play-thread safety (AX can softlock); Esc / close aborts wait.
		if (g->gameplay.flag_closingPhase) return;
		RefreshState(g);
		if (!cached.connected) return;
		{
			bool everyoneReady = false;
			if (cached.connected && !cached.players.empty()) {
				const std::string& hash = cached.players.front().selectedHash;
				if (!hash.empty()) {
					everyoneReady = std::ranges::all_of(cached.players, [&](const openlr2::ArenaPeer& p) {
						return p.ready && p.selectedHash == hash;
					});
				}
			}
			if (everyoneReady) break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	// clear forced seed after lock release.
	hasForcedSeed = false;
	forcedSeed = 0;
}

void OnScoreSnapshot(game* g, bool finalScore) {
	if (g == nullptr || !openlr2::arena_backend::Active().IsConnected(g)) return;
	openlr2::arena_backend::Active().SendScore(g, SnapshotScore(g), finalScore);
}

void OnJudgeCombo(game* g, bool comboIncrease) {
	if (g == nullptr || !IsConnected(g)) return;
	openlr2::arena_maniac::SetItemModeEnabled(itemModeEnabled || cached.itemModeEnabled);
	if (!openlr2::arena_maniac::IsItemModeEnabled()) return;
	if (comboIncrease) {
		int notes = g->gameplay.player[PLAYER_1].totalnotes;
		if (notes <= 0) notes = g->gameplay.player[PLAYER_1].total_note;
		openlr2::arena_maniac::OnComboIncrease(g, notes);
	} else {
		openlr2::arena_maniac::OnComboBreak();
	}
}

void Toast(ToastKind kind, const char* message) {
	if (message == nullptr || message[0] == '\0') return;
	if (!openlr2::arena_imgui::IsAvailable()) return;
	ImGuiToastType type = ImGuiToastType::Info;
	switch (kind) {
	case ToastKind::Success: type = ImGuiToastType::Success; break;
	case ToastKind::Warning: type = ImGuiToastType::Warning; break;
	case ToastKind::Error: type = ImGuiToastType::Error; break;
	case ToastKind::Info:
	default: type = ImGuiToastType::Info; break;
	}
	ImGui::InsertNotification(ImGuiToast(type, 3000, "%s", message));
}

void OnReturnToMenu(game* g) {
	if (g == nullptr || !openlr2::arena_backend::Active().IsConnected(g)) return;
	// cancel only from in-play return; skip if chart already cleared.
	if (matchInProgress && !matchFinished) {
		const auto& score = SnapshotScore(g);
		const int judged =
			score.poor + score.bad + score.good + score.great + score.pgreat;
		bool selfReady = false;
		for (const auto& p : cached.players) {
			if (p.playerId == cached.selfPlayerId) {
				selfReady = p.ready;
				break;
			}
		}
		const int maxEx = cachedChartMaxEx > 0
			? cachedChartMaxEx
			: openlr2::arena_gui::ChartMaxEx(g, cached);
		if (!(judged * 2 >= maxEx && selfReady)) {
			openlr2::arena_backend::Active().SendCancelled(g);
		}
	}
	matchInProgress = false;
	hasForcedSeed = false;
	forcedSeed = 0;
	lastArmedChartHash.clear();
	lastArmedRandomSeed = 0;
	// hide graph when automaticGraph + ResetState (not full ResetMatch).
	if (automaticGraph) {
		graphVisible = false;
	}
	openlr2::arena_maniac::ResetState();
}

void OnResult(game* g) {
	if (g == nullptr || !openlr2::arena_backend::Active().IsConnected(g)) return;
	matchFinished = true;
	openlr2::arena_maniac::ResetMatch();
	OnScoreSnapshot(g, true);
	RefreshState(g);
	const auto score = SnapshotScore(g);
	char json[512];
	std::snprintf(
		json,
		sizeof(json),
		"{\"poor\":%d,\"bad\":%d,\"good\":%d,\"great\":%d,\"pgreat\":%d,\"maxCombo\":%d,\"score\":%d,\"exscore\":%d,\"currentNotes\":%d,\"itemMode\":%s}",
		score.poor,
		score.bad,
		score.good,
		score.great,
		score.pgreat,
		score.maxCombo,
		score.score,
		score.pgreat * 2 + score.great,
		score.currentNotes,
		(cached.itemModeEnabled || itemModeEnabled) ? "true" : "false"
	);
	const char* hash = cached.chartHash.c_str();
	if (cached.chartHash.empty() && g != nullptr && g->sSelect.cur_song >= 0) {
		// Same source Scene04 uses for IR/ghost (bmsList), not metaSelected which is often empty.
		const auto& song = g->sSelect.bmsList[g->sSelect.cur_song];
		if (g->gameplay.courseType == 0 || g->gameplay.courseType == 2) {
			const CSTR& h = song.courseHash[g->gameplay.courseStageNow];
			if (h.body && h.body[0] != '\0') hash = h.body;
		} else if (song.hash.body && song.hash.body[0] != '\0') {
			hash = song.hash.body;
		} else {
			hash = g->sSelect.metaSelected.hash.body ? g->sSelect.metaSelected.hash.body : "";
		}
	}
	openlr2::arena_backend::Active().SubmitRoomResult(g, cached.roomId.c_str(), hash, json);
}

int BestOpponentEx(game* g) {
	RefreshState(g);
	// pacemaker: alone → own EX.
	if (cached.players.size() <= 1) {
		for (const auto& p : cached.players) {
			if (p.playerId == cached.selfPlayerId) {
				return p.score.pgreat * 2 + p.score.great;
			}
		}
		const auto own = SnapshotScore(g);
		return own.pgreat * 2 + own.great;
	}
	int best = 0;
	for (const auto& p : cached.players) {
		if (p.playerId == cached.selfPlayerId) continue;
		best = std::max(best, p.score.pgreat * 2 + p.score.great);
	}
	return best;
}

int CachedChartMaxEx() {
	return cachedChartMaxEx;
}

bool IsRandomFlipEnabled() {
	return randomFlip;
}

void SetRandomFlipEnabled(bool on) {
	randomFlip = on;
}

void ApplyRandomFlip7(int* noteRandomTableRow) {
	// random flip: swap(5,6) → mirror → swap(5,6) cancels; net effect is mirror only.
	// Do not call GetRand — AX reused the hooked return value and must not advance RNG here.
	if (!randomFlip || noteRandomTableRow == nullptr) return;
	for (int i = 1; i <= 7; ++i) {
		noteRandomTableRow[i] = 8 - noteRandomTableRow[i];
	}
}

} // namespace openlr2::arena

#include "LR2_customir.h"

#include "LR2_customir_api.h"
#include "LR2_songmanage.h"
#include "structure.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <future>
#include <optional>
#include <ranges>
#include <string>
#include <thread>

#include <DxLib/DxLib.h>

#ifdef _WIN32
#include <libloaderapi.h>
#include <wtypes.h>
#else
#include <dlfcn.h>
#endif // _WIN32

#if _WIN32

#if _WIN64
#ifndef NDEBUG
constexpr auto&& ARCH = "_D.x64";
#else
constexpr auto&& ARCH = ".x64";
#endif // NDEBUG
#else
#ifndef NDEBUG
constexpr auto&& ARCH = "_D.x86";
#else
constexpr auto&& ARCH = ".x86";
#endif // NDEBUG
#endif // _WIN64

constexpr auto&& DLL = ".dll";

#else

#if __x86_64__
#ifndef NDEBUG
constexpr auto&& ARCH = "_D.x64";
#else
constexpr auto&& ARCH = ".x64";
#endif // NDEBUG
#else
#ifndef NDEBUG
constexpr auto&& ARCH = "_D.x86";
#else
constexpr auto&& ARCH = ".x86";
#endif // NDEBUG
#endif // __x86_64__

constexpr auto&& DLL = ".so";

using HMODULE = void*;
static HMODULE LoadLibrary(const char* fp) { return dlopen(fp, RTLD_NOW); }
static void* GetProcAddress(HMODULE h, const char* name) { return dlsym(h, name); }
static void FreeLibrary(HMODULE h) { dlclose(h);}

#endif // _WIN32

// TODO
#define OverlayNotification ErrorLogFmtAdd

class CustomIR {
public:
	CustomIR() = delete;
	CustomIR(const std::filesystem::path& directory);
	bool Initialize();
	bool Login();
	SendScoreStatus SendScore(const IRScoreV1& score);
	openlr2::GetStatus GetResultRank(const char* songHash, openlr2::IRRankResult& out);
	openlr2::GetStatus RestoreCachedRank(const char* songHash, openlr2::IRRankResult& out);
	openlr2::GetStatus GetGhost(const char* songHash, openlr2::GhostMode mode, int targetPlayerId, openlr2::IRGhostResult& out);

	[[nodiscard]] const std::string& Name() const { return mName; };
private:
	struct ModuleDeleter {
		void operator()(std::remove_pointer_t<HMODULE>* handle);
	};
	std::unique_ptr<std::remove_pointer_t<HMODULE>, ModuleDeleter> mDllHandle;
	std::string mName;
	MethodTable mMethods;
};

CustomIR::CustomIR(const std::filesystem::path& _directory) {
	for (auto& file : std::filesystem::directory_iterator(_directory)) {
		if (!file.is_regular_file()) continue;
		if (file.path().extension().string() != DLL) continue;
		const auto filename = file.path().filename().string();
		ErrorLogFmtAdd("Trying to load CustomIR %s", filename.c_str());
		if (auto s = file.path().stem().string(); !s.ends_with(ARCH)) {
			ErrorLogFmtAdd("'%s' skipping per invalid file name stem (expected %s)\n", s.c_str(), ARCH);
			continue;
		}
		mDllHandle.reset(LoadLibrary(file.path().string().c_str()));
		if (mDllHandle == nullptr) {
			ErrorLogFmtAdd("'%s' LoadLibrary failed\n", filename.c_str());
			continue;
		}
		auto GetMethodTable = reinterpret_cast<void (OLR2_IR_API*)(MethodTable&)>(GetProcAddress(mDllHandle.get(), "GetMethodTable"));
		if (GetMethodTable == nullptr) {
			ErrorLogFmtAdd("'%s' skipping per missing 'GetMethodTable' export\n", filename.c_str());
			continue;
		};
		GetMethodTable(mMethods);
		if (mMethods.GetName == nullptr) {
			ErrorLogFmtAdd("'%s' skipping per missing 'GetName' implementation\n", filename.c_str());
			continue;
		};
		ErrorLogFmtAdd("CustomIR %s loaded: %s\n", filename.c_str(), mMethods.GetName());
		break;
	}
}

void CustomIR::ModuleDeleter::operator()(std::remove_pointer_t<HMODULE>* handle) {
	FreeLibrary(handle);
}

bool CustomIR::Initialize() {
	if (mMethods.GetName == nullptr) return false;
	mName = mMethods.GetName();
	if (mName.empty()) return false;
	return true;
}

bool CustomIR::Login() {
	if (mMethods.LoginV1 == nullptr) return true;
	return mMethods.LoginV1();
}

SendScoreStatus CustomIR::SendScore(const IRScoreV1& score) {
	if (mMethods.SendScoreV1 == nullptr) return SendScoreStatus::Fail;
	return mMethods.SendScoreV1(score);
}

openlr2::GetStatus CustomIR::GetResultRank(const char* songHash, openlr2::IRRankResult& out) {
	if (mMethods.GetResultRank == nullptr) return openlr2::GetStatus::Fail;
	return mMethods.GetResultRank(songHash, -1, out);
}

openlr2::GetStatus CustomIR::RestoreCachedRank(const char* songHash, openlr2::IRRankResult& out) {
	if (mMethods.RestoreCachedRank == nullptr) return openlr2::GetStatus::Fail;
	return mMethods.RestoreCachedRank(songHash, -1, out);
}

openlr2::GetStatus CustomIR::GetGhost(const char* songHash, openlr2::GhostMode mode, int targetPlayerId, openlr2::IRGhostResult& out) {
	if (mMethods.GetGhost == nullptr) return openlr2::GetStatus::Fail;
	return mMethods.GetGhost(songHash, mode, targetPlayerId, out);
}

CUSTOMIR_MANAGER::~CUSTOMIR_MANAGER() {
	// Make sure all scores are sent before exiting out of the process. (Can hang for up to ~15 minutes...)
	for (auto& thread : mSendThreads) {
		thread.get();
	}
	if (mResultIrFuture.valid()) {
		mResultIrFuture.get();
	}
}

static void SendScoreWithBlockingRetry(CustomIR& ir, const IRScoreV1& scoreV1) {
	constexpr int tryMax = 6;
	int tryCount = 1;
	while (tryCount <= tryMax) {
		switch (ir.SendScore(scoreV1)) {
		case SendScoreStatus::Fail:
			OverlayNotification("'%s' failed to submit score\n", ir.Name().c_str());
			return;
		case SendScoreStatus::Ok:
			return;
		case SendScoreStatus::Retry:
			std::this_thread::sleep_for(std::chrono::seconds(static_cast<int>(std::pow(4, tryCount))));
			tryCount++;
			break;
		}
	}
	OverlayNotification("'%s' failed to submit score after %d attempts\n", ir.Name().c_str(), tryCount);
}
static void SendScoreMultiplexed(std::vector<std::future<void>>& mSendThreads, const IRScoreV1& scoreV1, std::vector<std::shared_ptr<CustomIR>> irs) {
	std::vector<int> finishedThreads;
	for (const auto& [i, it] : std::views::enumerate(mSendThreads)) {
		if (it.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
			finishedThreads.push_back(static_cast<int>(i));
		}
	}
	// Deferred deletion because we need to keep the std::future for whatever reason
	std::ranges::reverse(finishedThreads);
	for (auto i : finishedThreads) {
		mSendThreads.erase(mSendThreads.begin() + i);
	}

	mSendThreads.reserve(mSendThreads.size() + irs.size());
	for (const auto& ir : irs) {
		mSendThreads.push_back(std::async(
					std::launch::async,
					[](std::shared_ptr<CustomIR> ir, IRScoreV1 score){ SendScoreWithBlockingRetry(*ir, score); },
					ir,
					scoreV1));
	}
}

void CUSTOMIR_MANAGER::Initialize(const std::filesystem::path& directory, std::string displayIr) {
	mDisplayIr = std::move(displayIr);
	for (auto& dir : std::filesystem::directory_iterator(directory)) {
		if (!dir.is_directory()) {
			ErrorLogFmtAdd("'%s' skipped for loading custom IR module, not a directory\n", dir.path().string().c_str());
			continue;
		}
		auto& ir = *mModules.emplace_back(std::make_shared<CustomIR>(dir));
		if (!ir.Initialize()) {
			mModules.pop_back();
			ErrorLogFmtAdd("'%s' failed to initialize as a custom IR module\n", dir.path().string().c_str());
			continue;
		}
		if (std::ranges::contains(std::ranges::subrange(mModules.begin(), mModules.end() - 1), ir.Name(), &CustomIR::Name)) {
			ErrorLogFmtAdd("'%s' IR module with such name has already been loaded\n");
			mModules.pop_back();
			continue;
		}
	}

	std::string allModules;
	{
		bool any = false;
		for (auto& m : mModules) {
			allModules += m->Name();
			allModules += ';';
			any = true;
		}
		if (any)
			allModules.pop_back();
	}
	if (std::ranges::contains(mModules, mDisplayIr, &CustomIR::Name)) {
		ErrorLogFmtAdd("Using '%s' as the display IR out of the list: %s\n", mDisplayIr.c_str(), allModules.c_str());
	} else {
		ErrorLogFmtAdd("Selected display IR '%s' was not found in the module list: %s\n", mDisplayIr.c_str(), allModules.c_str());
	}
}

void CUSTOMIR_MANAGER::Login() {
	for (auto& ir : mModules) {
		if (ir->Login()) {
			OverlayNotification("[%s] Logged in\n", ir->Name().c_str());
		} else {
			OverlayNotification("[%s] Failed to log in\n", ir->Name().c_str());
		}
	}
}

std::optional<openlr2::IRGhostResult> CUSTOMIR_MANAGER::TryGetTargetInfo(const char* songmd5, int mode, int targetPlayerId) const {
	const auto irIt = std::ranges::find(mModules, mDisplayIr, &CustomIR::Name);
	if (irIt == mModules.end()) { return std::nullopt; }

	openlr2::GhostMode ghostMode{};
	switch (mode) {
	case 0: ghostMode = openlr2::GhostMode::Target; break;
	case 6: ghostMode = openlr2::GhostMode::Top; break;
	case 7: ghostMode = openlr2::GhostMode::Next; break;
	case 8: ghostMode = openlr2::GhostMode::Average; break;
	default:
		ErrorLogFmtAdd("Invalid ghost mode: %d\n", mode);
		return std::nullopt;
	}

	openlr2::IRGhostResult result{};
	switch ((*irIt)->GetGhost(songmd5, ghostMode, targetPlayerId, result)) {
	case openlr2::GetStatus::Ok:
		if (ghostMode == openlr2::GhostMode::Average && result.averageExscore <= 0) {
			ErrorLogAdd("CustomIR BUG: invalid averageExscore\n");
			return std::nullopt;
		}
		return result;
	case openlr2::GetStatus::Retry:
		OverlayNotification("'%s' failed to get ghost data - ignoring retry\n", (*irIt)->Name().c_str());
		return std::nullopt;
	case openlr2::GetStatus::Fail:
		OverlayNotification("'%s' failed to get ghost data\n", (*irIt)->Name().c_str());
		return std::nullopt;
	}
	// TODO: remove 'abort()' from other handlers, logging an error and returning nothing instead. 
	ErrorLogAdd("CustomIR BUG: invalid GetGhost return value\n");
	return std::nullopt;
}

struct IRScoreInternal {
	struct SONG {
		std::string hash;
		std::string title;
		std::string subtitle;
		std::string genre;
		std::string artist;
		std::string subartist;
		int maxBPM{};
		int minBPM{};
		int longnote{};
		int random{};
		int judge{};
		int courseStageCount{};
		int courseType{};
	} song{};
	struct SETTINGS {
		int gaugeOption{};
		std::array<int, 2> random{};
		int autokey{};
		std::array<int, 2> assist{};
		int dpflip{};
		int hsfix{};
		std::array<int, 2> randSC{};
		std::array<int, 2> randFix{};
		int m_softlanding{};
		int m_addmine{};
		int m_addlong{};
		int m_earthquake{};
		int m_tornado{};
		int m_superloop{};
		int m_gambol{};
		int m_char{};
		int m_heartbeat{};
		int m_loudness{};
		int m_addnote{};
		int m_nabeatsu{};
		int m_accel{};
		int m_sincurve{};
		int m_wave{};
		int m_spiral{};
		int m_sidejump{};
		int is_extra{};
		int m_extra{};
		char m_lunaris{};
		bool m_gas{};
		int gomiscore{};
		int disablecurspeedchange{};
	} settings{};
	struct STATE {
		int player{};
		int keymode{};
		int randomseed{};
		double freqSpeedMultiplier{};
		double song_runtime{};
		char isNosave{};
		char isForceEasy{};
		char isCourse{};
		int courseStageNow{};
		int notes_total{};
	} state{};
	struct JUDGEMENTS {
		unsigned int epg{};
		unsigned int lpg{};
		unsigned int egr{};
		unsigned int lgr{};
		unsigned int egd{};
		unsigned int lgd{};
		unsigned int ebd{};
		unsigned int lbd{};
		unsigned int epr{};
		unsigned int lpr{};
		unsigned int cb{};
		unsigned int fast{};
		unsigned int slow{};
		unsigned int notes_played{};
		unsigned int notes_total{};
	};
	JUDGEMENTS judgements_total{};
	std::array<JUDGEMENTS, 20> judgements_column{};
	unsigned int max_combo{};
	std::array<double, 6> HP{};
	int gaugeType{};
	int moneyscore{};
	int exscore{};
	double rate{};
	int clearType{};
	int inputType{};
	std::string ghostData{};
	struct GRAPHDATA {
		std::array<std::array<int, 1000>, 6> hp{};
		std::array<int, 1000> combo{};
		std::array<int, 1000> exscore{};
		std::array<int, 1000> rate{};
	} graphs{};

	IRScoreInternal(game& game, sqlite3* sql, int player);
	void MakeScoreV1(IRScoreV1& scoreOut) const;
};

void IRScoreInternal::MakeScoreV1(IRScoreV1& scoreOut) const {
	scoreOut.song.hash = song.hash;
	scoreOut.song.title = song.title;
	scoreOut.song.subtitle = song.subtitle;
	scoreOut.song.genre = song.genre;
	scoreOut.song.artist = song.artist;
	scoreOut.song.subartist = song.subartist;
	scoreOut.song.maxBPM = song.maxBPM;
	scoreOut.song.minBPM = song.minBPM;
	scoreOut.song.longnote = song.longnote;
	scoreOut.song.random = song.random;
	scoreOut.song.judge = song.judge;
	scoreOut.song.courseStageCount = song.courseStageCount;
	scoreOut.song.courseType = song.courseType;

	scoreOut.settings.gaugeOption = settings.gaugeOption;
	scoreOut.settings.random = settings.random;
	scoreOut.settings.autokey = settings.autokey;
	scoreOut.settings.assist = settings.assist;
	scoreOut.settings.dpflip = settings.dpflip;
	scoreOut.settings.hsfix = settings.hsfix;
	scoreOut.settings.randSC = settings.randSC;
	scoreOut.settings.randFix = settings.randFix;
	scoreOut.settings.m_softlanding = settings.m_softlanding;
	scoreOut.settings.m_addmine = settings.m_addmine;
	scoreOut.settings.m_addlong = settings.m_addlong;
	scoreOut.settings.m_earthquake = settings.m_earthquake;
	scoreOut.settings.m_tornado = settings.m_tornado;
	scoreOut.settings.m_superloop = settings.m_superloop;
	scoreOut.settings.m_gambol = settings.m_gambol;
	scoreOut.settings.m_char = settings.m_char;
	scoreOut.settings.m_heartbeat = settings.m_heartbeat;
	scoreOut.settings.m_loudness = settings.m_loudness;
	scoreOut.settings.m_addnote = settings.m_addnote;
	scoreOut.settings.m_nabeatsu = settings.m_nabeatsu;
	scoreOut.settings.m_accel = settings.m_accel;
	scoreOut.settings.m_sincurve = settings.m_sincurve;
	scoreOut.settings.m_wave = settings.m_wave;
	scoreOut.settings.m_spiral = settings.m_spiral;
	scoreOut.settings.m_sidejump = settings.m_sidejump;
	scoreOut.settings.is_extra = settings.is_extra;
	scoreOut.settings.m_extra = settings.m_extra;
	scoreOut.settings.m_lunaris = settings.m_lunaris;
	scoreOut.settings.m_gas = settings.m_gas;
	scoreOut.settings.gomiscore = settings.gomiscore;
	scoreOut.settings.disablecurspeedchange = settings.disablecurspeedchange;

	scoreOut.state.player = state.player;
	scoreOut.state.keymode = state.keymode;
	scoreOut.state.randomseed = state.randomseed;
	scoreOut.state.freqSpeedMultiplier = state.freqSpeedMultiplier;
	scoreOut.state.song_runtime = state.song_runtime;
	scoreOut.state.isNosave = state.isNosave;
	scoreOut.state.isForceEasy = state.isForceEasy;
	scoreOut.state.isCourse = state.isCourse;
	scoreOut.state.courseStageNow = state.courseStageNow;
	scoreOut.state.notes_total = state.notes_total;

	auto judgements_assign = [](IRScoreV1::JUDGEMENTS& out, const IRScoreInternal::JUDGEMENTS& in) {
		out.epg = in.epg;
		out.lpg = in.lpg;
		out.egr = in.egr;
		out.lgr = in.lgr;
		out.egd = in.egd;
		out.lgd = in.lgd;
		out.ebd = in.ebd;
		out.lbd = in.lbd;
		out.epr = in.epr;
		out.lpr = in.lpr;
		out.cb = in.cb;
		out.fast = in.fast;
		out.slow = in.slow;
		out.notes_played = in.notes_played;
		out.notes_total = in.notes_total;
	};
	judgements_assign(scoreOut.judgements_total, judgements_total);
	for (std::size_t i = 0; i < scoreOut.judgements_column.size(); i++) {
		judgements_assign(scoreOut.judgements_column[i], judgements_column[i]);
	}
	scoreOut.max_combo = max_combo;
	scoreOut.HP = HP;
	scoreOut.gaugeType = gaugeType;
	scoreOut.moneyscore = moneyscore;
	scoreOut.exscore = exscore;
	scoreOut.rate = rate;
	scoreOut.clearType = clearType;
	scoreOut.inputType = inputType;

	scoreOut.graphs.hp = graphs.hp;
	scoreOut.graphs.combo = graphs.combo;
	scoreOut.graphs.exscore = graphs.exscore;
	scoreOut.graphs.rate = graphs.rate;
	scoreOut.ghostData = ghostData;
}

IRScoreInternal::IRScoreInternal(game& game, sqlite3* sql, int _player) {
	const SONGDATA& curSong = game.sSelect.bmsList[game.sSelect.cur_song];
	bool courseSong = (game.gameplay.courseType == 0 || game.gameplay.courseType == 2) && game.procSelecter != 13;
	bool courseScore = game.procSelecter == 13;
	if (courseSong) {
		song.hash = curSong.courseHash[game.gameplay.courseStageNow].body;
		SONGDATA songData;
		GetSongData(song.hash.c_str(), &songData, sql, &game.sSelect);
		song.title = songData.title.body;
		song.subtitle = songData.subtitle.body;
		song.genre = songData.genre.body;
		song.artist = songData.artist.body;
		song.subartist = songData.subartist.body;
		song.maxBPM = songData.maxBPM;
		song.minBPM = songData.minBPM;
		song.longnote = songData.longnote;
		song.random = songData.random;
		song.judge = songData.judge;
	}
	else {
		song.hash = curSong.hash.body;
		song.title = curSong.title.body;
		song.subtitle = curSong.subtitle.body;
		song.genre = curSong.genre.body;
		song.artist = curSong.artist.body;
		song.subartist = curSong.subartist.body;
		song.maxBPM = curSong.maxBPM;
		song.minBPM = curSong.minBPM;
		song.longnote = curSong.longnote;
		song.random = curSong.random;
		song.judge = curSong.judge;
		song.courseStageCount = curSong.courseStageCount;
		song.courseType = curSong.courseType;
	}
	CONFIG_PLAY& cfg = game.config.play;
	settings.gaugeOption = cfg.gaugeOption[_player];
	settings.random[0] = cfg.random[0];
	settings.random[1] = cfg.random[1];
	settings.autokey = cfg.autokey;
	settings.assist[0] = cfg.p1_assist;
	settings.assist[1] = cfg.p2_assist;
	settings.dpflip = cfg.dpflip;
	settings.hsfix = cfg.hsfix;
	settings.randSC[0] = cfg.randSC[0];
	settings.randSC[1] = cfg.randSC[1];
	settings.randFix[0] = cfg.randFix[0];
	settings.randFix[1] = cfg.randFix[1];
	settings.m_softlanding = cfg.m_softlanding;
	settings.m_addmine = cfg.m_addmine;
	settings.m_addlong = cfg.m_addlong;
	settings.m_earthquake = cfg.m_earthquake;
	settings.m_tornado = cfg.m_tornado;
	settings.m_superloop = cfg.m_superloop;
	settings.m_gambol = cfg.m_gambol;
	settings.m_char = cfg.m_char;
	settings.m_heartbeat = cfg.m_heartbeat;
	settings.m_loudness = cfg.m_loudness;
	settings.m_addnote = cfg.m_addnote;
	settings.m_nabeatsu = cfg.m_nabeatsu;
	settings.m_accel = cfg.m_accel;
	settings.m_sincurve = cfg.m_sincurve;
	settings.m_wave = cfg.m_wave;
	settings.m_spiral = cfg.m_spiral;
	settings.m_sidejump = cfg.m_sidejump;
	settings.is_extra = cfg.is_extra;
	settings.m_extra = cfg.m_extra;
	settings.m_lunaris = cfg.m_lunaris;
	settings.m_gas = cfg.m_gas;
	settings.gomiscore = cfg.gomiscore;
	settings.disablecurspeedchange = cfg.disablecurspeedchange;

	gameplay& gameplay = game.gameplay;
	state.player = _player;
	state.keymode = gameplay.keymode;
	state.randomseed = gameplay.randomseed;
	state.freqSpeedMultiplier = gameplay.freqSpeedMultiplier;
	if (!courseScore) state.song_runtime = gameplay.song_runtime;
	state.isNosave = gameplay.isNosave;
	state.isForceEasy = gameplay.isForceEasy;
	state.isCourse = gameplay.isCourse;
	state.courseStageNow = gameplay.courseStageNow;
	state.notes_total = gameplay.player[_player].totalnotes;

	PLAYERSTATUS& player = gameplay.player[_player];
	auto judgements_assign = [](IRScoreInternal::JUDGEMENTS& out, const EXTENDEDPLAYERSTATS& in) {
		out.epg = in.epg;
		out.lpg = in.lpg;
		out.egr = in.egr;
		out.lgr = in.lgr;
		out.egd = in.egd;
		out.lgd = in.lgd;
		out.ebd = in.ebd;
		out.lbd = in.lbd;
		out.epr = in.epr;
		out.lpr = in.lpr;
		out.cb = in.cb;
		out.fast = in.fast;
		out.slow = in.slow;
		out.notes_played = in.noteCount;
		};
	if (!courseScore) {
		judgements_assign(judgements_total, player.extendedStats);
		judgements_total.notes_total = state.notes_total;
		for (std::size_t i = 0; i < judgements_column.size(); i++) {
			judgements_assign(judgements_column[i], player.extendedColumnStats[i]);
			judgements_column[i].notes_total = gameplay.bmsobj_note[i].count;
		}
	}
	else {
		judgements_assign(judgements_total, player.extendedStatsCourse);
		judgements_total.notes_total = state.notes_total;
		for (std::size_t i = 0; i < judgements_column.size(); i++) {
			judgements_assign(judgements_column[i], player.extendedColumnStatsCourse[i]);
			judgements_column[i].notes_total = gameplay.bmsobj_note[i].count;
		}
	}
	max_combo = player.max_combo;
	HP = player.HP;
	gaugeType = player.gaugeType;
	if (!courseScore) moneyscore = player.score;
	exscore = player.exscore;
	rate = player.rate;
	clearType = player.clearType;
	inputType = DetermineResultPlayDevice(&game.KeyInput);

	if (!courseScore) {
		::GRAPHDATA& statgraph = gameplay.statgraph[_player];
		graphs.hp = statgraph.hp;
		memcpy(graphs.combo.data(), statgraph.combo, graphs.combo.size());
		memcpy(graphs.exscore.data(), statgraph.exscore, graphs.exscore.size());
		memcpy(graphs.rate.data(), gameplay.rategraph[_player].val, graphs.rate.size());
	}

	if (!courseScore && _player == 0) {
		ghostData = gameplay.resultGhostForIr;
	}
}

std::optional<openlr2::IRRankResult> CUSTOMIR_MANAGER::RestoreCachedRank(const char* songmd5) {
	const auto irIt = std::ranges::find(mModules, mDisplayIr, &CustomIR::Name);
	if (irIt == mModules.end()) { return std::nullopt; }
	const auto ir = *irIt;

	openlr2::IRRankResult out{};
	switch(ir->RestoreCachedRank(songmd5, out)) {
	case openlr2::GetStatus::Ok: return out;
	case openlr2::GetStatus::Retry:
		OverlayNotification("'%s' failed to get result rank - ignoring retry\n", ir->Name().c_str());
		return std::nullopt;
	case openlr2::GetStatus::Fail:
		OverlayNotification("'%s' failed to get result rank\n", ir->Name().c_str());
		return std::nullopt;
	}
	abort(); // unreachable
}

static std::optional<openlr2::IRRankResult> ResultIrAsync(std::shared_ptr<CustomIR> ir, IRScoreV1 score) {
	SendScoreWithBlockingRetry(*ir, score);

	std::optional<openlr2::IRRankResult> out{ openlr2::IRRankResult{} };
	switch (ir->GetResultRank(score.song.hash.c_str(), *out)) {
	case openlr2::GetStatus::Ok: return out;
	case openlr2::GetStatus::Retry:
		OverlayNotification("'%s' failed to get result rank - ignoring retry\n", ir->Name().c_str());
		return std::nullopt;
	case openlr2::GetStatus::Fail:
		OverlayNotification("'%s' failed to get result rank\n", ir->Name().c_str());
		return std::nullopt;
	}
	abort(); // unreachable
}
void CUSTOMIR_MANAGER::BeginResultIr(game& game, sqlite3* sql, int player) {
	if (mModules.empty()) {
		return;
	}

	IRScoreInternal internal{ game, sql, player };
	IRScoreV1 scoreV1;
	internal.MakeScoreV1(scoreV1);

	SendScoreMultiplexed(
			mSendThreads, scoreV1,
			mModules | std::views::filter([&](const std::shared_ptr<CustomIR> &module) {
				return module->Name() != mDisplayIr;
				}) | std::ranges::to<std::vector>());

	const auto irIt = std::ranges::find(mModules, mDisplayIr, &CustomIR::Name);
	if (irIt == mModules.end()) {
		return;
	}
	if (mResultIrFuture.valid()) {
		ErrorLogAdd("BUG: we have an unexpected mResultIrFuture");
		mResultIrFuture.get();
	}
	mResultIrFuture = std::async(std::launch::async, &ResultIrAsync, *irIt, scoreV1);
}

static void fill_ranking_player_from_customir(RANKINGPLAYER& dest, const openlr2::IRRankPlayer& src, int ranking) {
	dest.name = src.name.c_str();
	dest.comment = src.comment.c_str();
	dest.id = src.id; // TODO: actually has a different meaning, dest.id is LR2ID but src.id is custom IR's user ID
	switch (src.clear) {
	case openlr2::Lamp::NoPlay: dest.clear = 0; break;
	case openlr2::Lamp::Fail: dest.clear = 1; break;
	case openlr2::Lamp::Easy: dest.clear = 2; break;
	case openlr2::Lamp::Groove: dest.clear = 3; break;
	case openlr2::Lamp::Hard: dest.clear = 4; break;
	case openlr2::Lamp::FullCombo: dest.clear = 5; break;
	}
	dest.notes = src.notes;
	dest.combo = src.maxcombo;
	dest.pg = src.pg;
	dest.gr = src.gr;
	dest.gd = src.gd;
	dest.bd = src.bd;
	dest.pr = src.pr;
	dest.minbp = src.minbp;
	dest.playcount = src.playcount;
	dest.ranking = ranking;
	(void)src.timestamp; // TODO
	(void)src.randomLayout; // TODO
	(void)src.randomOption; // TODO
	(void)src.inputType; // TODO
	(void)src.rseed; // TODO
	(void)src.gauge; // TODO
	(void)src.dpflip; // TODO
}

void openlr2::fill_ranking_from_customir(const openlr2::IRRankResult& result, RANKING& rd) {
	rd.Init();

	if (rd.rankingMax < static_cast<int>(result.ranking.size())) {
		rd.ExpandRankingBuffer(static_cast<int>(result.ranking.size()));
	}
	for (auto [to, from, ranking] : std::views::zip(std::span(rd.ranking, rd.rankingMax), result.ranking, std::views::iota(1))) {
		fill_ranking_player_from_customir(to, from, ranking);
	}

	rd.myRanking = result.myRank;
	rd.rankingCount = result.totalPlayer;
	rd.lastupdate = std::to_string(result.lastupdate).c_str();
	rd.totalPlaycount = result.totalPlaycount;
	for (auto [to, from] : std::views::zip(rd.clearPlayers, result.clearPlayers)) {
		to = from;
	}
}

void openlr2::fill_status_from_ranking(const RANKING& rd, bool fail_in_clear_rate, STATUS& best) {
	best.IRranking = rd.myRanking;
	best.IRplayercount = rd.rankingCount;
	if (rd.rankingCount == 0) {
		best.IRclearRate = 0;
	} else {
		if (fail_in_clear_rate) {
			// fail%???? why is it inverted
			best.IRclearRate = (rd.rankingCount + rd.clearPlayers[1] - rd.clearPlayers[0]) / rd.rankingCount;
		} else {
			// clear%
			best.IRclearRate = (rd.clearPlayers[2] + rd.clearPlayers[3] + rd.clearPlayers[4] + rd.clearPlayers[5]) * 100 / rd.rankingCount;
		}
	}
}

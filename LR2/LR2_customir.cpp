#include "LR2_customir.h"
#include "LR2_customir_api.h"
#include "structure.h"
#include "LR2_songmanage.h"
#include "En_input.h"

#include <array>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <future>
#include <ranges>
#include <thread>
#include <string_view>
#include <type_traits>

#include <DxLib/DxLib.h>
#include <libloaderapi.h>

#if _WIN64
#if _DEBUG
constexpr auto&& ARCH = "_D.x64";
#else
constexpr auto&& ARCH = ".x64";
#endif
#elif _WIN32
#if _DEBUG
constexpr auto&& ARCH = "_D.x86";
#else
constexpr auto&& ARCH = ".x86";
#endif
#else
#error "Unsupported Arch Linux"
#endif

// TODO
#define OverlayNotification ErrorLogFmtAdd

CustomIR::CustomIR(const std::filesystem::path& _directory) {
	for (auto& file : std::filesystem::directory_iterator(_directory)) {
		if (!file.is_regular_file()) continue;
		if (file.path().extension().string() != ".dll") continue;
		if (auto s = file.path().stem().string(); !s.ends_with(ARCH)) {
			ErrorLogFmtAdd("'%s' skipping IR module with invalid file name stem (expected %s)\n", s.c_str(), ARCH);
			continue;
		}
		mDllHandle.reset(LoadLibraryW(file.path().wstring().c_str()));
		if (mDllHandle == nullptr) continue;
		auto GetMethodTable = reinterpret_cast<void (__cdecl*)(MethodTable&)>(GetProcAddress(mDllHandle.get(), "GetMethodTable"));
		if (GetMethodTable == nullptr) {
			ErrorLogFmtAdd("'%s' not loaded, missing 'GetMethodTable' export.\n", file.path().filename().string().c_str());
			continue;
		};
		GetMethodTable(mMethods);
		if (mMethods.GetName == nullptr) {
			ErrorLogFmtAdd("'%s' not loaded, missing essential 'GetName' implementation.\n", file.path().filename().string().c_str());
			continue;
		};
		ErrorLogFmtAdd("'%s' loaded: %s\n", file.path().filename().string().c_str(), mMethods.GetName());
		break;
	}
}

void CustomIR::ModuleDeleter::operator()(std::remove_pointer_t<HMODULE>* handle) {
	FreeLibrary(handle);
}

bool CustomIR::Initialize() {
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

GetStatus CustomIR::GetResultRank(const IRScoreV1& score, IRRankResultV1& out) {
	if (mMethods.GetResultRankV1 == nullptr) return GetStatus::Ok;
	return mMethods.GetResultRankV1(score, out);
}

CUSTOMIR_MANAGER::~CUSTOMIR_MANAGER() {
	// Make sure all scores are sent before exiting out of the process. (Can hang for up to ~15 minutes...)
	for (auto& thread : mSendThreads) {
		thread.get();
	}
	if (mResultIrFuture.valid()) {
		mResultIrFuture.wait();
	}
}

namespace {
	std::string TrimAscii(std::string_view text) {
		std::size_t begin = 0;
		while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin]))) {
			++begin;
		}
		std::size_t end = text.size();
		while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
			--end;
		}
		return std::string(text.substr(begin, end - begin));
	}

	bool ParseActiveIniProviderLine(std::string_view line, std::string& providerOut) {
		line = TrimAscii(line);
		if (line.empty() || line[0] == '#' || line[0] == ';') {
			return false;
		}
		const std::size_t eq = line.find('=');
		if (eq == std::string_view::npos) {
			return false;
		}
		const std::string key = TrimAscii(line.substr(0, eq));
		if (key != "provider") {
			return false;
		}
		providerOut = TrimAscii(line.substr(eq + 1));
		return true;
	}
}

void CUSTOMIR_MANAGER::LoadActiveProviderConfig(const std::filesystem::path& customIrRoot) {
	mActiveProvider.clear();
	const std::filesystem::path configPath = customIrRoot / "active.ini";
	if (!std::filesystem::is_regular_file(configPath)) {
		return;
	}

	std::ifstream input(configPath);
	if (!input) {
		ErrorLogFmtAdd("CustomIR: failed to open active.ini at '%s'\n", configPath.string().c_str());
		return;
	}

	std::string line;
	while (std::getline(input, line)) {
		if (ParseActiveIniProviderLine(line, mActiveProvider) && !mActiveProvider.empty()) {
			break;
		}
	}

	if (!mActiveProvider.empty()) {
		ErrorLogFmtAdd("CustomIR: active provider '%s' from active.ini\n", mActiveProvider.c_str());
	}
}

std::vector<std::shared_ptr<CustomIR>> CUSTOMIR_MANAGER::ResolveSidecarModules() const {
	if (mActiveProvider.empty() || !ProvidesResultRank()) {
		return mModules;
	}
	std::vector<std::shared_ptr<CustomIR>> sidecarModules;
	sidecarModules.reserve(mModules.size());
	for (const auto& module : mModules) {
		if (module->Name() != mActiveProvider) {
			sidecarModules.push_back(module);
		}
	}
	return sidecarModules;
}

std::vector<std::shared_ptr<CustomIR>> CUSTOMIR_MANAGER::ResolveDisplayModules() const {
	if (mActiveProvider.empty()) {
		return {};
	}
	for (const auto& module : mModules) {
		if (module->Name() == mActiveProvider) {
			return { module };
		}
	}
	return {};
}

bool CUSTOMIR_MANAGER::AnyDisplayModuleSupports(bool (CustomIR::*pred)() const) const {
	for (const auto& module : ResolveDisplayModules()) {
		if ((module.get()->*pred)()) {
			return true;
		}
	}
	return false;
}

bool CUSTOMIR_MANAGER::ProvidesResultRank() const {
	return AnyDisplayModuleSupports(&CustomIR::SupportsResultRank);
}

void CUSTOMIR_MANAGER::EnqueueSidecarSend(const IRScoreV1& scoreV1, std::vector<std::shared_ptr<CustomIR>> sidecarModules) {
	if (sidecarModules.empty()) {
		return;
	}

	std::vector<int> finishedThreads;
	for (const auto& [i, it] : std::views::enumerate(mSendThreads)) {
		if (it.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
			finishedThreads.push_back(static_cast<int>(i));
		}
	}
	std::ranges::reverse(finishedThreads);
	for (auto i : finishedThreads) {
		mSendThreads.erase(mSendThreads.begin() + i);
	}

	mSendThreads.push_back(std::async(std::launch::async,
		[scoreV1, toSend = std::move(sidecarModules)]() mutable {
			constexpr int tryMax = 6;
			int tryCount = 1;
			while (!toSend.empty() && tryCount <= tryMax) {
				std::vector<std::future<SendScoreStatus>> sendThreads;
				sendThreads.reserve(toSend.size());
				for (const auto& module : toSend) {
					sendThreads.push_back(std::async(std::launch::async, &CustomIR::SendScore, module, scoreV1));
				}

				for (const auto& [i, t] : std::views::enumerate(sendThreads) | std::views::reverse) {
					switch (t.get()) {
					case SendScoreStatus::Fail:
						OverlayNotification("'%s' failed to submit score\n", toSend[i]->Name().c_str());
						[[fallthrough]];
					case SendScoreStatus::Ok:
						toSend.erase(toSend.begin() + static_cast<std::ptrdiff_t>(i));
						break;
					case SendScoreStatus::Retry:
						break;
					}
				}

				const auto sleepFor = static_cast<int>(std::pow(4, tryCount));
				std::this_thread::sleep_for(std::chrono::seconds(sleepFor));
				tryCount++;
			}
		}));
}

void CUSTOMIR_MANAGER::Initialize(const std::filesystem::path& directory) {
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

	LoadActiveProviderConfig(directory);

	if (!mModules.empty() && mActiveProvider.empty()) {
		ErrorLogFmtAdd(
			"CustomIR: no provider in active.ini; display fetch disabled, SendScore still active\n"
		);
	} else if (!mActiveProvider.empty() && ResolveDisplayModules().empty()) {
		ErrorLogFmtAdd(
			"CustomIR: provider '%s' not found; display fetch disabled, SendScore still active\n",
			mActiveProvider.c_str()
		);
	}
}

void CUSTOMIR_MANAGER::Login() {
	mProviderLoggedIn = false;
	if (mModules.empty()) {
		return;
	}
	for (auto& ir : mModules) {
		const bool loginOk = ir->Login();
		if (loginOk) {
			OverlayNotification("[%s] Logged in\n", ir->Name().c_str());
		} else {
			OverlayNotification("[%s] Failed to log in\n", ir->Name().c_str());
		}
		if (!mActiveProvider.empty() && ir->Name() == mActiveProvider && loginOk) {
			mProviderLoggedIn = true;
		}
	}
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
	struct GRAPHDATA {
		std::array<std::array<int, 1000>, 6> hp{};
		std::array<int, 1000> combo{};
		std::array<int, 1000> exscore{};
		std::array<int, 1000> rate{};
	} graphs{};

	IRScoreInternal(game& game, sqlite3* sql, int player);
	void MakeScoreV1(IRScoreV1& scoreOut) const;
};

namespace {
	void SeedResultRankFromMybest(game& game, int curSong) {
		if (curSong < 0 || curSong >= game.sSelect.bmsListCount) {
			return;
		}
		const STATUS& best = game.sSelect.bmsList[curSong].mybest;
		if (best.IRranking > 0) {
			game.net.rankingData.myRanking = best.IRranking;
		}
		if (best.IRplayercount > 0) {
			game.net.rankingData.rankingCount = best.IRplayercount;
		}
	}

	void ApplyResultRank(game& g, int curSong, const IRRankResultV1& merged) {
		if (curSong < 0 || curSong >= g.sSelect.bmsListCount) {
			return;
		}
		STATUS& best = g.sSelect.bmsList[curSong].mybest;
		if (merged.rank > 0) {
			best.IRranking = merged.rank;
			g.net.rankingData.myRanking = merged.rank;
		}
		if (merged.playerCount > 0) {
			best.IRplayercount = merged.playerCount;
			g.net.rankingData.rankingCount = merged.playerCount;
		}
	}

	bool SendScoreWithRetry(const std::shared_ptr<CustomIR>& module, const IRScoreV1& scoreV1) {
		constexpr int tryMax = 6;
		int tryCount = 1;
		while (tryCount <= tryMax) {
			const SendScoreStatus status = module->SupportsSendScoreV1()
				? module->SendScore(scoreV1)
				: SendScoreStatus::Fail;
			switch (status) {
			case SendScoreStatus::Fail:
				if (!module->SupportsSendScoreV1()) {
					OverlayNotification("'%s' has no SendScore handler\n", module->Name().c_str());
				} else {
					OverlayNotification("'%s' failed to submit score\n", module->Name().c_str());
				}
				return false;
			case SendScoreStatus::Ok:
				return true;
			case SendScoreStatus::Retry:
				std::this_thread::sleep_for(std::chrono::seconds(static_cast<int>(std::pow(4, tryCount))));
				tryCount++;
				break;
			}
		}
		return false;
	}
}

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
}

bool CUSTOMIR_MANAGER::IsResultIrPending() const {
	if (!mResultIrFuture.valid()) {
		return false;
	}
	return mResultIrFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready;
}

void CUSTOMIR_MANAGER::BeginResultIr(game& game, sqlite3* sql, int player) {
	if (mModules.empty()) {
		return;
	}

	const int curSong = game.sSelect.cur_song;
	if (curSong < 0 || curSong >= game.sSelect.bmsListCount) {
		return;
	}

	IRScoreInternal internal{ game, sql, player };
	IRScoreV1 scoreV1;
	internal.MakeScoreV1(scoreV1);

	SeedResultRankFromMybest(game, curSong);

	const auto sidecarModules = ResolveSidecarModules();
	EnqueueSidecarSend(scoreV1, sidecarModules);

	if (!ProvidesResultRank()) {
		return;
	}

	const auto displayModules = ResolveDisplayModules();
	if (displayModules.empty()) {
		return;
	}

	if (mResultIrFuture.valid()) {
		mResultIrFuture.wait();
	}

	mResultIrFuture = std::async(std::launch::async,
		[this, provider = displayModules.front(), scoreV1, curSong, gamePtr = &game]() {
			SendScoreWithRetry(provider, scoreV1);

			IRRankResultV1 merged{};
			bool gotResult = false;
			IRRankResultV1 out{};
			const GetStatus status = provider->GetResultRank(scoreV1, out);
			if (status == GetStatus::Fail) {
				OverlayNotification("'%s' failed to get result rank\n", provider->Name().c_str());
			} else if (status == GetStatus::Ok) {
				if (out.rank > 0 || out.playerCount > 0) {
					merged = out;
					gotResult = true;
				}
			}

			if (gotResult) {
				ApplyResultRank(*gamePtr, curSong, merged);
			}
		});
}

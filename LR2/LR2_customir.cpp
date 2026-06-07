#include "LR2_customir.h"
#include "LR2_customir_api.h"
#include "structure.h"
#include "LR2_songmanage.h"
#include "En_input.h"

#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <future>
#include <ranges>
#include <thread>
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

GetStatus CustomIR::RestoreCachedRank(const char* songHash, IRRankResultV1& out) {
	if (mMethods.RestoreCachedRankV1 == nullptr) return GetStatus::Ok;
	return mMethods.RestoreCachedRankV1(songHash, out);
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

std::vector<std::shared_ptr<CustomIR>> CUSTOMIR_MANAGER::ResolveSidecarModules() const {
	if (!mActiveProvider.body || !mActiveProvider.body[0] || !ProvidesResultRank()) {
		return mModules;
	}
	std::vector<std::shared_ptr<CustomIR>> sidecarModules;
	sidecarModules.reserve(mModules.size());
	for (const auto& module : mModules) {
		if (module->Name() != mActiveProvider.body) {
			sidecarModules.push_back(module);
		}
	}
	return sidecarModules;
}

bool CUSTOMIR_MANAGER::ProvidesResultRank() const {
	if (!mActiveProvider.body || !mActiveProvider.body[0]) {
		return false;
	}
	const auto it = std::ranges::find(mModules, mActiveProvider.body, &CustomIR::Name);
	return it != mModules.end() && (*it)->mMethods.GetResultRankV1 != nullptr;
}

bool CUSTOMIR_MANAGER::ProvidesCachedRankRestore() const {
	if (!mActiveProvider.body || !mActiveProvider.body[0]) {
		return false;
	}
	const auto it = std::ranges::find(mModules, mActiveProvider.body, &CustomIR::Name);
	return it != mModules.end() && (*it)->mMethods.RestoreCachedRankV1 != nullptr;
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
	// Deferred deletion because we need to keep the std::future for whatever reason
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

void CUSTOMIR_MANAGER::Initialize(const std::filesystem::path& directory, const CSTR& activeProvider) {
	mActiveProvider.assign(&activeProvider);
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

	if (mActiveProvider.body && mActiveProvider.body[0] && std::ranges::find(mModules, mActiveProvider.body, &CustomIR::Name) == mModules.end()) {
		ErrorLogFmtAdd(
			"CustomIR: network/customir_provider '%s' not found; display fetch disabled, SendScore still active\n",
			mActiveProvider.body
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
		if (mActiveProvider.body && mActiveProvider.body[0] && ir->Name() == mActiveProvider.body && loginOk) {
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

	void CopyIrRankPlayerToRankingPlayer(RANKINGPLAYER& dest, const IRRankPlayerV1& src) {
		cstrSprintf(&dest.name, "%s", src.name.c_str());
		dest.id = src.id;
		dest.sp = src.sp;
		dest.dp = src.dp;
		dest.clear = src.clear;
		dest.notes = src.notes;
		dest.combo = src.combo;
		dest.pg = src.pg;
		dest.gr = src.gr;
		dest.gd = src.gd;
		dest.bd = src.bd;
		dest.pr = src.pr;
		dest.minbp = src.minbp;
		dest.option = src.option;
		dest.playcount = src.playcount;
		dest.ranking = src.ranking;
		cstrSprintf(&dest.comment, "%s", src.comment.c_str());
	}

	enum class IrRankApplyContext {
		Result,
		SongSelectRestore,
	};

	bool HasIrRankPayload(const IRRankResultV1& result) {
		if (result.myRank > 0 || result.totalPlayer > 0) {
			return true;
		}
		if (!result.ranking.empty()) {
			return true;
		}
		if (!result.lastupdate.empty()) {
			return true;
		}
		if (result.totalPlaycount > 0) {
			return true;
		}
		for (int count : result.clearPlayers) {
			if (count != 0) {
				return true;
			}
		}
		return false;
	}

	void ApplyIrRankResult(game& g, int curSong, const IRRankResultV1& result, IrRankApplyContext ctx) {
		if (curSong < 0 || curSong >= g.sSelect.bmsListCount) {
			return;
		}
		STATUS& best = g.sSelect.bmsList[curSong].mybest;
		RANKING& rd = g.net.rankingData;

		if (!result.ranking.empty()) {
			const int preservedMyId = rd.myID;
			const int preservedRivalId = rd.rivalID;
			rd.Init();
			rd.myID = preservedMyId;
			rd.rivalID = preservedRivalId;

			const int boardSize = static_cast<int>(result.ranking.size());
			rd.ExpandRankingBuffer(boardSize);
			for (int i = 0; i < boardSize; ++i) {
				CopyIrRankPlayerToRankingPlayer(rd.ranking[i], result.ranking[i]);
			}
		}

		if (result.myRank > 0) {
			rd.myRanking = result.myRank;
		}
		if (result.totalPlayer > 0) {
			rd.rankingCount = result.totalPlayer;
		}
		if (!result.lastupdate.empty()) {
			cstrSprintf(&rd.lastupdate, "%s", result.lastupdate.c_str());
		}
		if (result.totalPlaycount > 0) {
			rd.totalPlaycount = result.totalPlaycount;
		}
		bool mergeClearPlayers = result.totalPlayer > 0;
		if (!mergeClearPlayers) {
			for (int count : result.clearPlayers) {
				if (count != 0) {
					mergeClearPlayers = true;
					break;
				}
			}
		}
		if (mergeClearPlayers) {
			for (int i = 0; i < 6; ++i) {
				rd.clearPlayers[i] = result.clearPlayers[i];
			}
		}

		if (rd.myRanking > 0) {
			best.IRranking = rd.myRanking;
		}
		if (rd.rankingCount > 0) {
			best.IRplayercount = rd.rankingCount;
		}
		if (rd.rankingCount > 0) {
			if (ctx == IrRankApplyContext::Result) {
				best.IRclearRate = (rd.rankingCount + rd.clearPlayers[1] - rd.clearPlayers[0]) / rd.rankingCount;
			} else {
				best.IRclearRate = (rd.clearPlayers[2] + rd.clearPlayers[3] + rd.clearPlayers[4] + rd.clearPlayers[5]) * 100 / rd.rankingCount;
			}
		}
	}
}

bool CUSTOMIR_MANAGER::SendScoreWithRetry(const std::shared_ptr<CustomIR>& module, const IRScoreV1& scoreV1) {
	constexpr int tryMax = 6;
	int tryCount = 1;
	while (tryCount <= tryMax) {
		switch (module->SendScore(scoreV1)) {
		case SendScoreStatus::Fail:
			OverlayNotification("'%s' failed to submit score\n", module->Name().c_str());
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

	if (!mActiveProvider.body || !mActiveProvider.body[0]) {
		return;
	}
	const auto displayIt = std::ranges::find(mModules, mActiveProvider.body, &CustomIR::Name);
	if (displayIt == mModules.end() || (*displayIt)->mMethods.GetResultRankV1 == nullptr) {
		return;
	}

	if (mResultIrFuture.valid()) {
		mResultIrFuture.wait();
	}

	mResultIrFuture = std::async(std::launch::async,
		[this, provider = *displayIt, scoreV1, curSong, gamePtr = &game]() {
			SendScoreWithRetry(provider, scoreV1);

			IRRankResultV1 merged{};
			bool gotResult = false;
			IRRankResultV1 out{};
			const GetStatus status = provider->GetResultRank(scoreV1, out);
			if (status == GetStatus::Fail) {
				OverlayNotification("'%s' failed to get result rank\n", provider->Name().c_str());
			} else if (status == GetStatus::Ok) {
				if (HasIrRankPayload(out)) {
					merged = out;
					gotResult = true;
				}
			}

			if (gotResult) {
				ApplyIrRankResult(*gamePtr, curSong, merged, IrRankApplyContext::Result);
			}
		});
}

void CUSTOMIR_MANAGER::OnSongSelectRestoreRank(game& game) {
	const int curSong = game.sSelect.cur_song;
	if (curSong < 0 || curSong >= game.sSelect.bmsListCount) {
		return;
	}
	if (!mActiveProvider.body || !mActiveProvider.body[0]) {
		return;
	}
	const auto displayIt = std::ranges::find(mModules, mActiveProvider.body, &CustomIR::Name);
	if (displayIt == mModules.end() || (*displayIt)->mMethods.RestoreCachedRankV1 == nullptr) {
		return;
	}

	const SONGDATA& entry = game.sSelect.bmsList[curSong];

	SeedResultRankFromMybest(game, curSong);

	IRRankResultV1 out{};
	const GetStatus status = (*displayIt)->RestoreCachedRank(entry.hash.body, out);
	if (status == GetStatus::Fail) {
		OverlayNotification("'%s' failed to restore cached rank\n", (*displayIt)->Name().c_str());
		return;
	}
	if (status == GetStatus::Ok && HasIrRankPayload(out)) {
		ApplyIrRankResult(game, curSong, out, IrRankApplyContext::SongSelectRestore);
	}
}

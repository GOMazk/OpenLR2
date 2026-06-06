#pragma once

// CustomIR public ABI between OpenLR2 host and third-party IR DLLs.
// DLLs export GetMethodTable(MethodTable&); leave unimplemented slots as nullptr.
// MethodTable and struct layouts are append-only: do not reorder slots or change IRScoreV1 fields.
// Module implementation guide: ExampleIR/README.md

#include <string>
#include <array>
#include <vector>

// Play/course result payload for SendScoreV1 (submitted after a chart finishes).
struct IRScoreV1 {
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
};

// SendScoreV1 return value.
enum class SendScoreStatus: int {
	Ok = 0,
	Retry,
	Fail,
};

// Shared query input for Get*V1 slots (song metadata + option fingerprint).
struct IRRankQueryV1 {
	IRScoreV1::SONG song{};
	IRScoreV1::SETTINGS settings{};
	IRScoreV1::STATE state{};
	int exscore{};
	int clearType{};
};

// GetResultRankV1 output; result-screen rank display (skin #92/#93). rank/playerCount 0 = unknown.
struct IRRankResultV1 {
	int rank{};
	int playerCount{};
};

// GetChartStatsV1 output; song-select chart stats. clearDistribution indexed by clear type.
struct IRChartStatsV1 {
	int playerCount{};
	std::array<int, 6> clearDistribution{};
};

// One row in GetLeaderboardV1 output; F3 in-game ranking board entry.
struct IRLeaderboardEntryV1 {
	int rank{};
	int playerId{};
	int exscore{};
	int clearType{};
	int pgreat{};
	int great{};
	int good{};
	int bad{};
	int poor{};
	int maxcombo{};
	int minbp{};
	int playcount{};
	int optionPacked{};
	int keymode{};
	std::string displayName;
};

// GetLeaderboardV1 output; F3 in-game ranking board.
struct IRLeaderboardResultV1 {
	int playerCount{};
	int myRank{};
	int myPlayerId{};
	std::vector<IRLeaderboardEntryV1> entries;
};

// GetRivalChartV1 output; song-select rival row.
struct IRRivalChartV1 {
	int playerId{};
	int exscore{};
	int clearType{};
	int pgreat{};
	int great{};
	int good{};
	int bad{};
	int poor{};
	int maxcombo{};
	int minbp{};
	int playcount{};
	int rank{};
	std::string displayName;
	bool hasPlay{};
};

// Return value for all Get*V1 MethodTable slots.
enum class GetStatus: int {
	Ok = 0,
	Retry,
	Fail,
};

// GetProviderMetaV1 output; F5 web ranking URL templates ({hash} placeholder).
struct IRProviderMetaV1 {
	const char* webRankingChartUrlTemplate = nullptr;
	const char* webRankingCourseUrlTemplate = nullptr;
	const char* apiVersion = nullptr;
};

// GetIrGhostV1 query; play-scene ghost target (e.g. g-battle rival).
struct IRGhostQueryV1 {
	IRScoreV1::SONG song{};
	int mode = 0;
	int viewerPlayerId = 0;
	int targetPlayerId = 0;
};

// GetIrGhostV1 output; play-scene ghost replay data.
struct IRGhostResultV1 {
	std::string displayName;
	std::string ghostData;
	int optionDigit1{};
	int optionDigit2{};
	int optionDigit3{};
	int optionDigit4{};
	int randomSeed{};
	int averageExscore{};
	bool hasPlay{};
};

struct MethodTable {
	const char*(__cdecl* GetName)() = nullptr;
	bool(__cdecl* LoginV1)() = nullptr;
	SendScoreStatus(__cdecl* SendScoreV1)(const IRScoreV1& score) = nullptr;
	GetStatus(__cdecl* GetResultRankV1)(const IRRankQueryV1& query, IRRankResultV1& out) = nullptr;
	GetStatus(__cdecl* GetChartStatsV1)(const IRRankQueryV1& query, IRChartStatsV1& out) = nullptr;
	GetStatus(__cdecl* GetLeaderboardV1)(const IRRankQueryV1& query, IRLeaderboardResultV1& out, int limit, int offset) = nullptr;
	GetStatus(__cdecl* GetRivalChartV1)(int rivalPlayerId, const IRRankQueryV1& query, IRRivalChartV1& out) = nullptr;
	void(__cdecl* GetProviderMetaV1)(IRProviderMetaV1& out) = nullptr;
	GetStatus(__cdecl* GetIrGhostV1)(const IRGhostQueryV1& query, IRGhostResultV1& out) = nullptr;
};

#pragma once

#include <string>
#include <array>
#include <vector>

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

enum class SendScoreStatus: int {
	Ok = 0,
	Retry,
	Fail,
};

struct IRRankQueryV1 {
	IRScoreV1::SONG song{};
	IRScoreV1::SETTINGS settings{};
	IRScoreV1::STATE state{};
	int exscore{};
	int clearType{};
};

struct IRRankResultV1 {
	int rank{};
	int playerCount{};
};

struct IRChartStatsV1 {
	int playerCount{};
	std::array<int, 6> clearDistribution{};
};

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

struct IRLeaderboardResultV1 {
	int playerCount{};
	int myRank{};
	int myPlayerId{};
	std::vector<IRLeaderboardEntryV1> entries;
};

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

enum class FetchRankStatus: int {
	Ok = 0,
	Retry,
	Fail,
};

struct IRProviderMetaV1 {
	const char* webRankingChartUrlTemplate = nullptr;
	const char* webRankingCourseUrlTemplate = nullptr;
	const char* apiVersion = nullptr;
};

struct IRGhostQueryV1 {
	IRScoreV1::SONG song{};
	int mode = 0;
	int viewerPlayerId = 0;
	int targetPlayerId = 0;
};

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
	FetchRankStatus(__cdecl* FetchResultRankV1)(const IRRankQueryV1& query, IRRankResultV1& out) = nullptr;
	FetchRankStatus(__cdecl* FetchChartStatsV1)(const IRRankQueryV1& query, IRChartStatsV1& out) = nullptr;
	FetchRankStatus(__cdecl* FetchLeaderboardV1)(const IRRankQueryV1& query, IRLeaderboardResultV1& out, int limit, int offset) = nullptr;
	FetchRankStatus(__cdecl* FetchRivalChartV1)(int rivalPlayerId, const IRRankQueryV1& query, IRRivalChartV1& out) = nullptr;
	void(__cdecl* GetProviderMetaV1)(IRProviderMetaV1& out) = nullptr;
	FetchRankStatus(__cdecl* FetchIrGhostV1)(const IRGhostQueryV1& query, IRGhostResultV1& out) = nullptr;
};

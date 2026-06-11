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

struct IRRankPlayer {
	std::string name;
	std::string comment;
	int id{};
	int sp{};
	int dp{};
	int clear{};
	int notes{};
	int combo{};
	int pg{};
	int gr{};
	int gd{};
	int bd{};
	int pr{};
	int minbp{};
	int option{};
	int playcount{};
	int ranking{};
};

struct IRRankResult {
	int myRank{};
	int totalPlayer{};
	std::string lastupdate;
	int totalPlaycount{};
	std::array<int, 6> clearPlayers{};
	int rivalRank{};
	std::vector<IRRankPlayer> ranking;
};

enum class GetStatus: int {
	Ok = 0,
	Retry,
	Fail,
};

struct MethodTable {
	const char*(__cdecl* GetName)() = nullptr;
	bool(__cdecl* LoginV1)() = nullptr;
	SendScoreStatus(__cdecl* SendScoreV1)(const IRScoreV1& score) = nullptr;
	GetStatus(__cdecl* GetResultRank)(const IRScoreV1& score, IRRankResult& out) = nullptr;
	GetStatus(__cdecl* RestoreCachedRank)(const char* songHash, IRRankResult& out) = nullptr;
};

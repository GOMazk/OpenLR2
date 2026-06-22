#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#ifdef _WIN32
#define OLR2_IR_API __cdecl
#define OLR2_IR_EXPORT __declspec(dllexport)
#else
#define OLR2_IR_API
#define OLR2_IR_EXPORT
#endif // _WIN32

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
	std::string ghostData{}; // In LR2 ghost format. May not be present in some cases, e.g. course scores.
};

enum class SendScoreStatus: int {
	Ok = 0,
	Retry,
	Fail,
};

namespace openlr2 {

enum class Gauge : int {
	Unknown,
	Groove,
	Survival,
	Death,
	Easy,
	PAttack,
	GAttack,
};

enum class InputType : int {
	Unknown = 0,
	Keyboard,
	Controller,
	Midi,
};

enum class GhostMode : int {
	Target = 0,
	Top,
	Next,
	Average,
};

enum class Lamp : int {
	NoPlay = 0,
	Fail,
	Easy,
	Groove,
	Hard,
	FullCombo,
};

enum class Random : int {
	No = 0,
	Mirror,
	Random,
	SRandom,
	Scatter, // AKA H-Random
	Converge, // AKA All-Scratch
};

struct IRGhostResult {
	std::string displayName;
	std::string ghostData; // Example: E@3ZZ
	// P1 and P2 random layouts.
	// Should be 0 if the layout is not known, or the corresponding \ref randomOption is not noran, mirror, or random.
	// Examples: 1234567 54321 135792468.
	std::array<int, 2> randomLayout{};
	std::array<Random, 2> randomOption{};
	Gauge gauge{Gauge::Unknown};
	// LR2 rseed. If not known, the layout from \ref randomLayout can be used instead.
	// -1 if unknown, 0-0x7ffe otherwise.
	int rseed{-1};
	bool dpflip{};
	bool reserved1{};
	bool reserved2{};
	bool reserved3{};
	int averageExscore{}; // Only used if \ref GhostMode == \ref GhostMode::Average.
};

struct IRRankPlayer {
	std::string name; // Display name of the user
	std::string comment; // User-defined comment of the score
	uint64_t timestamp{}; // Seconds since Unix Epoch or 0 if unknown
	// P1 and P2 random layouts.
	// Should be 0 if the layout is not known, or the corresponding \ref randomOption is not noran, mirror, or random.
	// Examples: 1234567 54321 135792468.
	std::array<int, 2> randomLayout{};
	std::array<Random, 2> randomOption{};
	InputType inputType{InputType::Unknown};
	int id{}; // Numeric ID of the user
	int playcount{};
	Lamp clear{};
	int notes{}; // Amount of notes in the chart (TODO: allow 0 for such case?) or chart variation if the chart is affected by #RANDOM
	int maxcombo{};
	int pg{};
	int gr{};
	int gd{};
	int bd{};
	int pr{};
	int minbp{};
	Gauge gauge{Gauge::Unknown};
	// LR2 rseed. If not known, the layout from \ref randomLayout can be used instead.
	// -1 if unknown, 0-0x7ffe otherwise.
	int rseed{-1};
	bool dpflip{};
	bool reserved1{};
	bool reserved2{};
	bool reserved3{};
	uint64_t reserved4{};
	uint64_t reserved5{};
	uint64_t reserved6{};
	uint64_t reserved7{};
};

struct IRRankResult {
	// The leaderboard. Top X players, usually 999.
	std::vector<IRRankPlayer> ranking;
	std::array<int, 6> clearPlayers{};
	// Seconds since Unix Epoch.
	// When the data was fetched, can be used to display how fresh the data is.
	// It's up to the CustomIR module to decide for how long it wants to cache the data.
	uint64_t lastupdate{};
	// Current player's position in the leaderboard. Separate parameter since it may be outside of \ref ranking.
	int myRank{};
	int reserved{};
	int totalPlayer{};
	int totalPlaycount{};
};

enum class GetStatus: int {
	Ok = 0,
	Retry,
	Fail,
};

} // namespace openlr2

struct MethodTable {
	// Mandatory method. Module name must be unique among loaded modules.
	const char*(OLR2_IR_API* GetName)() = nullptr;
	// Maybe parse some configuration file there, and perform URL request to your IR for login.
	// This method is ran at game initialization synchronously.
	// Can be used for general initialization.
	bool(OLR2_IR_API* LoginV1)() = nullptr;
	// Ran on its own thread at score result, both for normal plays and courses.
	// This is called even with scores that wouldn't be sent to LR2IR or saved to the score.db, it's up to the module to
	// filter them.
	// For soft errors, SendScore should return SendScoreStatus::Retry. The game will retry calling a sensible amount
	// times with a backoff then.
	SendScoreStatus(OLR2_IR_API* SendScoreV1)(const IRScoreV1& score) = nullptr;
	// Called from score result after SendScore completes.
	// The game awaits on this to complete before it lets the user out of result.
	// Make sure to save the fetched result somewhere, so that RestoreCachedRank can then load it.
	// Replace with HTTP + mandatory cache write.
	openlr2::GetStatus(OLR2_IR_API* GetResultRank)(const char* songHash, int reserved, openlr2::IRRankResult& out) = nullptr;
	// Called from song select when scrolling past 'songHash' song.
	// Loads the leaderboard from where GetResultRank saved it.
	// Do not perform HTTP here.
	// Replace with cache read.
	openlr2::GetStatus(OLR2_IR_API* RestoreCachedRank)(const char* songHash, int reserved, openlr2::IRRankResult& out) = nullptr;
	// This is called synchronously when play is entered to retrieve the ghost data for the play.
	openlr2::GetStatus(OLR2_IR_API* GetGhost)(const char* songHash, openlr2::GhostMode mode, int targetPlayerId, openlr2::IRGhostResult& out) = nullptr;
	// Forward compatibility.
	// These fields will always be moved to the end of the struct when new fields are added.
	// Thanks to this CustomIR module designed for newer game version won't write out-of-bounds memory when assigning fields of older MethodTable.
	void* reserved1 = nullptr;
	void* reserved2 = nullptr;
	void* reserved3 = nullptr;
	void* reserved4 = nullptr;
	void* reserved5 = nullptr;
	void* reserved6 = nullptr;
};

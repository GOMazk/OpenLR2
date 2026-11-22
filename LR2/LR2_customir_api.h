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
	// In LR2 ghost format. May not be present in some cases, e.g. course scores. Example: E@3ZZ.
	std::string ghostData;
	// Chart level at play time (SONGDATA::level). 0 if unknown or not applicable.
	int songPlayLevel{};
	uint64_t reserved1{};
	uint64_t reserved2{};
	uint64_t reserved3{};
	uint64_t reserved4{};
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
	std::string ghostData; // \ref IRScoreV1::ghostData
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
	// Must be sorted by exscore.
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

struct IRRivalInfo {
	int id{};
	std::string name;
};

// One rival score row. Field names match legacy LR2IR XML / typical IR JSON.
struct IRRivalScore {
	std::string hash;
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
	uint64_t lastupdate{};
};

struct IRRivalListResult {
	std::vector<IRRivalInfo> rivals;
	uint64_t fetched_at{};
};

// ---------------------------------------------------------------------------
// Arena (multiplayer via the display_ir CustomIR module)
//
// Two layers (do not confuse them):
//   1) This MethodTable — OpenLR2 EXE <-> CustomIR DLL only. Field names/types
//      are the ABI. OpenLR2 never calls your HTTP/WebSocket URLs directly.
//   2) Vendor transport — how the DLL talks to YOUR servers (HTTP, WS, …).
//      Free to choose; not part of this ABI.
//
// Typical vendor split (example only):
//   JoinRoom / Leave / SubmitRoomResult     → membership / archive HTTP (or equiv.)
//   SendSelectedChart…SendItemSettingsJson  → live play channel (WS, etc.)
//   IsConnected / PollState                 → DLL-local session state
//
// LAN (ws://host:2222 msgpack) is implemented inside OpenLR2 itself
// (arena_lan_*), not through these MethodTable entries.
//
// Vendor checklist:
//   - GetName + LoginV1 (Arena join auth usually reuses Login credentials)
//   - Assign Arena* pointers; nullptr = Arena unsupported (EXE Fail / no-op)
//   - ArenaJoinRoom Ok ⇒ live session up (IsConnected true, PollState fills peers)
//   - ArenaPollState: thread-safe snapshot; drain pendingItems each call
//   - Send*: no-op when not connected; host-only ops also enforced server-side
//   - Min play loop without items/archive: Join…KickUser (see MethodTable below)
//   - Smoke: Join → chart → Ready → Score → Leave
// ---------------------------------------------------------------------------

// Live score snapshot (play + graph). Same judge fields OpenLR2 already tracks.
struct ArenaPeerScore {
	int poor{};
	int bad{};
	int good{};
	int great{};
	int pgreat{};
	int maxCombo{};
	int score{};
	int currentNotes{};
};

struct ArenaPeer {
	int playerId{}; // stable id for Give host / Kick (vendor-defined)
	std::string username;
	bool isHost{};
	bool ready{}; // loading-complete flag for wait-ready
	std::string selectedHash;
	unsigned option{};
	unsigned gauge{};
	ArenaPeerScore score{};
};

// Incoming ojama item. ArenaPollState must MOVE these out then clear the queue
// so each event is delivered once.
struct ArenaPendingItem {
	int rolledItemId{-1};
	unsigned level{};
};

struct ArenaItemSettingRow {
	unsigned lv1{};
	unsigned lv2{};
	unsigned lv3{};
	unsigned weight{};
};

struct ArenaItemSettings {
	unsigned threshold{10};
	std::vector<ArenaItemSettingRow> settings;
	bool updated{}; // true when settings just arrived; clear after OpenLR2 consumes
};

// Filled by ArenaPollState each call. OpenLR2 TickUi reads this for lobby/graph/
// seed/host/items. Keep field names stable — rename breaks EXE.
struct ArenaRoomState {
	std::string roomId;
	int hostId{};
	int selfPlayerId{};
	bool connected{};
	bool itemModeEnabled{};
	int randomSeed{}; // guests: forced seed while armed
	std::string chartHash;
	std::string chartTitle;
	std::string chartArtist;
	std::vector<ArenaPeer> players;
	std::vector<std::string> chatLog; // cumulative; OpenLR2 diffs for toasts
	std::vector<ArenaPendingItem> pendingItems; // drain each poll
	ArenaItemSettings itemSettings;
};

// Output of ArenaJoinRoom on Ok. OpenLR2 mainly checks GetStatus + later
// ArenaIsConnected / PollState; wsUrl/token are for the DLL's own live session.
struct ArenaJoinResult {
	std::string roomId;
	std::string wsUrl;
	std::string token;
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
	[[deprecated("Use GetWebRankingUrl instead")]] const char* webRankingUrlTemplate = nullptr;
	// This is called synchronously when F5 or the IR button is pressed in song-select.
	// \retval "" - error or inapplicable.
	std::string(OLR2_IR_API* GetWebRankingUrl)(char const* songHash) = nullptr;
	// XXX: just some food for thought: should we introduce a `size_t MethodTableSize` here?
	// '_trailing_field_to_increase_struct_size' is not cool, it's never too late to make things better.
	//
	// Called synchronously during startup rival sync (same window as legacy LR2IR_GetRivalInfo).
	// nullptr = module does not support CustomIR rivals.
	openlr2::GetStatus(OLR2_IR_API* GetRivals)(openlr2::IRRivalListResult& out) = nullptr;
	// Called per rival after GetRivals.
	// lastUpdateHint: optional; OpenLR2 passes max r_lastupdate from its rival DB when available.
	// The module may use it only to skip HTTP / read its private cache.
	// On Ok: out is a complete snapshot of that rival's scores. OpenLR2 writes .db and .lr2folder.
	openlr2::GetStatus(OLR2_IR_API* SyncRivalScores)(int rivalId, uint64_t lastUpdateHint, std::vector<openlr2::IRRivalScore>& out) = nullptr;

	// --- Arena (optional). nullptr = module does not support Arena. ---
	// OpenLR2 only calls these on the display_ir module. Leave Send* as no-ops
	// when not connected. Mid-session drop: set connected=false; OpenLR2 stops
	// sending and toasts disconnect (local play continues).
	//
	// Join by room code (UI). On Ok, module should already be live-connected so
	// ArenaIsConnected becomes true and PollState fills peers. Retry = transient.
	openlr2::GetStatus(OLR2_IR_API* ArenaJoinRoom)(const char* roomId, openlr2::ArenaJoinResult& out) = nullptr;
	// Tear down live session. OpenLR2 clears its cache after calling this; void
	// (no GetStatus) — HTTP leave failure is module-internal.
	void(OLR2_IR_API* ArenaLeave)() = nullptr;
	// True while the live session is up (after Join, until Leave / drop).
	bool(OLR2_IR_API* ArenaIsConnected)() = nullptr;
	// Snapshot into `out`. Called often from TickUi. Drain pendingItems each call.
	void(OLR2_IR_API* ArenaPollState)(openlr2::ArenaRoomState& out) = nullptr;
	// Host selects chart + seed (+ item mode). Guests may also report own hash.
	void(OLR2_IR_API* ArenaSendSelectedChart)(
		int randomSeed,
		const char* hash,
		const char* title,
		const char* artist,
		unsigned option,
		unsigned gauge,
		bool itemModeEnabled
	) = nullptr;
	// Play score stream. finalScore=true at result; non-final may be throttled by module.
	void(OLR2_IR_API* ArenaSendScore)(const openlr2::ArenaPeerScore& score, bool finalScore) = nullptr;
	// Loading complete (everyone-ready wait).
	void(OLR2_IR_API* ArenaSendReady)() = nullptr;
	// Chart cancel / return to menu.
	void(OLR2_IR_API* ArenaSendCancelled)() = nullptr;
	// UTF-8 chat line.
	void(OLR2_IR_API* ArenaSendChat)(const char* message) = nullptr;
	// Local chart missing for current room hash.
	void(OLR2_IR_API* ArenaSendMissingChart)() = nullptr;
	// Host only: transfer host to playerId (from ArenaPeer::playerId).
	void(OLR2_IR_API* ArenaSetHost)(int playerId) = nullptr;
	// Host only: kick playerId.
	void(OLR2_IR_API* ArenaKickUser)(int playerId) = nullptr;
	// Ojama payload JSON object string, e.g. {"rolledItemId":…,"level":…}.
	void(OLR2_IR_API* ArenaSendItemJson)(const char* jsonObject) = nullptr;
	// Host only: item weights / threshold JSON for peers (+ late joiners).
	void(OLR2_IR_API* ArenaSendItemSettingsJson)(const char* jsonObject) = nullptr;
	// Archive play result to the room page / history. resultJson is a JSON object
	// string built by OpenLR2. Optional if you do not persist room results.
	openlr2::GetStatus(OLR2_IR_API* ArenaSubmitRoomResult)(
		const char* roomId,
		const char* chartHash,
		const char* resultJson
	) = nullptr;

	// Forward compatibility.
	// This field will always be moved to the end of the struct when new fields are added.
	// Thanks to this CustomIR module designed for newer game version won't write out-of-bounds memory when assigning fields of older MethodTable.
	void* _trailing_field_to_increase_struct_size[6]{};
};

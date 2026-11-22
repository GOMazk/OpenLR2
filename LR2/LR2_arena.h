#pragma once

#include "LR2_customir_api.h"

#include <string>

struct game;
struct sqlite3;

// OpenLR2 Arena. IR transport lives in display_ir CustomIR; LAN in arena_lan_*.
namespace openlr2::arena {

void Init(game* g, sqlite3* songDb = nullptr);
void Shutdown();
/** Optional song.db handle for missing-chart lookup (same db Main opens). */
void SetSongDb(sqlite3* songDb);

// Overlay / input (Insert toggles menu). Call once per frame before ScreenFlip.
// ImGui only (DX9/11). No DxLib text fallback — softwarerendering / no D3D skips UI.
void TickUi(game* g);

// True when Insert menu is open and muteGameInputs is on.
[[nodiscard]] bool IsMenuBlockingInput();
/** True while lobby and/or graph ImGui windows are shown (OS cursor owned by Arena). */
[[nodiscard]] bool WantsOsCursor();
/** Zero play buttons + mouse after ReactInput when muteGameInputs is on. */
void MutePlayButtonsIfMenuOpen(game* g);

// Join helpers (also available from overlay).
bool JoinRoom(game* g, const char* roomId);
/** LAN: connect to server at host:2222. */
bool ConnectLan(game* g, const char* host, const char* username);
bool StartLanServer(game* g);
void StopLanServer(game* g);
[[nodiscard]] bool IsLanServerRunning();
/** IR | LAN — exclusive; switches leave the other transport. */
void SetTransportIr(game* g);
void SetTransportLan(game* g);
[[nodiscard]] bool IsTransportLan();
void Leave(game* g);

char* RoomIdInputBuf();
char* LanHostInputBuf();
char* LanUserInputBuf();
size_t RoomIdInputCap();
size_t LanHostInputCap();
size_t LanUserInputCap();

[[nodiscard]] bool IsConnected(game* g);
[[nodiscard]] bool IsInMatch(game* g);
[[nodiscard]] const openlr2::ArenaRoomState& State(game* g);

/** title + subtitle (or fulltitle) for Arena chart share / overlay. */
[[nodiscard]] std::string FormatChartDisplayTitle(game* g);
/** artist + " " + subartist. */
[[nodiscard]] std::string FormatChartDisplayArtist(game* g);
/** Local relative path for room chart hash. */
[[nodiscard]] std::string LookupChartRelativePath(game* g, const std::string& hash);

// Play pipeline hooks.
void OnChartSelected(game* g, const char* hash, const char* title, const char* artist, unsigned option, unsigned gauge);
[[nodiscard]] int TakeForcedRandomSeed(game* g); // 0 = none
void OnLoadingCompleteWaitReady(game* g); // blocks until room ready or disconnect
void OnScoreSnapshot(game* g, bool finalScore);
/** Called from play judge path: comboIncrease true on hit, false on break. */
void OnJudgeCombo(game* g, bool comboIncrease);
void OnReturnToMenu(game* g);
void OnResult(game* g);

enum class ToastKind { Info, Success, Warning, Error };
void Toast(ToastKind kind, const char* message);

// Pacemaker: best opponent EX (pgreat*2+great); alone → own EX.
[[nodiscard]] int BestOpponentEx(game* g);
/** Chart max EX cached at OnChartSelected. */
[[nodiscard]] int CachedChartMaxEx();

/** random_flip — mirror RANDOM layout after shuffle. */
[[nodiscard]] bool IsRandomFlipEnabled();
void SetRandomFlipEnabled(bool on);
/** Apply mirror/swap to 7-key noteRandomTable row (indices 1..7). */
void ApplyRandomFlip7(int* noteRandomTableRow /* length >= 8 */);

} // namespace openlr2::arena

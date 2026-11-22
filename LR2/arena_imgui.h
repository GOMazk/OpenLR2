#pragma once

#include "LR2_customir_api.h"

struct game;
struct ImFont;

// Dear ImGui shell for OpenLR2 Arena (DX9 / DX11 + Win32). Falls back when unavailable.
namespace openlr2::arena_imgui {

[[nodiscard]] bool Init();
void Shutdown();

/** Call before DxLib ChangeWindowMode / SetGraphMode that may reset the D3D device. */
void NotifyDeviceLost();
/** Call after the device is usable again. */
void NotifyDeviceReset();

[[nodiscard]] bool IsAvailable();
[[nodiscard]] bool WantCaptureKeyboard();
[[nodiscard]] bool WantCaptureMouse();
/** waitingForKeyPress — TickUi must not fire binds while true. */
[[nodiscard]] bool IsWaitingForKeyBind();
void SetWaitingForKeyBind(bool waiting);

/** Optional larger icon font; nullptr when using system/default fonts only. */
[[nodiscard]] ImFont* BigIconFont();

/**
 * One frame: NewFrame → Arena GUI → Render. No-op if unavailable.
 * @param menuVisible Insert lobby window
 * @param graphVisible PgUp scores window
 * @param muteGameInputs settings checkbox (default true)
 * @param itemModeEnabled host item/ojama flag (UI only)
 */
void DrawFrame(
	game* g,
	const openlr2::ArenaRoomState& state,
	bool& menuVisible,
	bool& graphVisible,
	bool& automaticGraph,
	bool& muteGameInputs,
	bool& itemModeEnabled,
	char* roomIdInput,
	size_t roomIdCap,
	char* chatInput,
	size_t chatCap,
	bool (*joinRoom)(game*, const char*),
	void (*leave)(game*)
);

} // namespace openlr2::arena_imgui

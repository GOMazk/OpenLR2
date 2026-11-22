#pragma once

#include "LR2_customir_api.h"

struct game;

namespace openlr2::arena_imgui {

void SetupImGuiStyle();

void RenderArenaGui(
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

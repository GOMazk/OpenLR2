#pragma once

#include "LR2_customir_api.h"

#include <cstddef>

struct game;

// Native bridge helpers (misc + max EX helpers; no client/hooks).
namespace openlr2::arena_gui {

[[nodiscard]] int ExScore(const openlr2::ArenaPeerScore& s);
[[nodiscard]] int BpScore(const openlr2::ArenaPeerScore& s);
[[nodiscard]] int JudgedNotes(const openlr2::ArenaPeerScore& s);
[[nodiscard]] float ExRatePercent(const openlr2::ArenaPeerScore& s);

/** Chart note count from game / peers; 0 if unknown. */
[[nodiscard]] int ChartTotalNotes(game* g, const openlr2::ArenaRoomState& state);
/** maxScore ≡ notes*2 (theoretical EX). */
[[nodiscard]] int ChartMaxEx(game* g, const openlr2::ArenaRoomState& state);

[[nodiscard]] const char* OptionLabel(unsigned option);
[[nodiscard]] const char* GaugeLabel(unsigned gauge);

/** LAN connect fields (username/host). Empty if unset. */
void LoadLanUsername(char* out, size_t outCap);
void LoadLanHost(char* out, size_t outCap);
/** Persist after successful LAN Connect. */
void SaveLanConnectCredentials(const char* host, const char* username);

/** muteGameInputs in LR2files/Config/arena.ini (default true). */
bool LoadMuteGameInputs();
void SaveMuteGameInputs(bool mute);

/** automaticGraph in arena.ini (default true). */
bool LoadAutomaticGraph();
void SaveAutomaticGraph(bool enabled);

/** random_flip in arena.ini (default false). */
bool LoadRandomFlip();
void SaveRandomFlip(bool enabled);

/** item_sound_volume 0–100 (default 100). */
int LoadItemSfxVolume();
void SaveItemSfxVolume(int percent);

void LoadItemSfxPathsIntoManiac();
void SaveItemSfxPath(const char* id, const char* path);
void ClearItemSfxPaths();

} // namespace openlr2::arena_gui

#pragma once

#include <random>
#include <string>
#include <unordered_map>
#include <vector>

struct game;

namespace openlr2::arena_maniac {

struct CurrentItem {
	int rolledItemId{-1};
	unsigned short level{0};
	bool final{false};

	bool operator==(const CurrentItem& o) const {
		return rolledItemId == o.rolledItemId && level == o.level;
	}
};

struct CurrentItemHash {
	std::size_t operator()(const CurrentItem& k) const {
		return static_cast<std::size_t>(k.rolledItemId) ^ (static_cast<std::size_t>(k.level) << 1);
	}
};

struct ItemDef {
	std::string name;
	std::string icon; // short ASCII HUD label (no FA embed)
	int* field{nullptr}; // points into config.play after BindGame
	unsigned defaultVal{0};
	unsigned lv1{0};
	unsigned lv2{0};
	unsigned lv3{0};
	unsigned weight{0};
};

struct ItemSettingRow {
	unsigned lv1{0};
	unsigned lv2{0};
	unsigned lv3{0};
	unsigned weight{0};
};

struct ItemSettingsPayload {
	unsigned threshold{10}; // percent 1–100
	std::vector<ItemSettingRow> settings;
};

inline constexpr int kItemTimeMs = 8000;

void SetupDefaults();
void BindGame(game* g); // resolve m_* field pointers
void ResetState();
void ResetMatch(); // clear held + active on leave/result
void ShutdownSfx();

void SetItemModeEnabled(bool on);
[[nodiscard]] bool IsItemModeEnabled();

void OnComboIncrease(game* g, int totalNotes);
void OnComboBreak();

/** @return true if a held item was consumed and send was attempted while connected */
[[nodiscard]] bool UseItem(game* g);
void TriggerItem(game* g, CurrentItem item);
void TickActive(game* g, int dtMs); // call each frame during play; re-applies active m_*

void UpdateItemWeights();
void SendItemSettings(game* g);
void ApplyItemSettings(const ItemSettingsPayload& settings);

void SetSfxVolume(int percent);
[[nodiscard]] int SfxVolume();
void PlaySfx(const char* id);

struct SfxInfo {
	std::string id;
	std::string name;
	std::string customPath; // empty = default
};

[[nodiscard]] std::vector<SfxInfo> ListSfx();
void SetSfxCustomPath(const char* id, const char* path);
void ResetAllCustomSfx();
void ApplySfxCustomPathFromConfig(const char* id, const char* path);

[[nodiscard]] CurrentItem& HeldItem();
[[nodiscard]] std::vector<ItemDef>& Items();
[[nodiscard]] std::unordered_map<CurrentItem, int, CurrentItemHash>& ActiveItems();
[[nodiscard]] float& ThresholdMult();
[[nodiscard]] unsigned& CurrentCombo();
[[nodiscard]] unsigned& Threshold();
[[nodiscard]] bool& SettingsRemoteUpdated();

} // namespace openlr2::arena_maniac

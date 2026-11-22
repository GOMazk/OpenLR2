#include "arena_maniac.h"

#include "En_audio.h"
#include "arena_backend.h"
#include "structure.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>

namespace openlr2::arena_maniac {
namespace {

bool gItemMode{};
unsigned gCurrentCombo{};
unsigned gThreshold{100};
float gThresholdMult{0.10f};
bool gSettingsRemoteUpdated{};
CurrentItem gHeld{};
std::vector<ItemDef> gItems;
std::unordered_map<CurrentItem, int, CurrentItemHash> gActive;
std::mt19937 gRng{std::random_device{}()};
std::discrete_distribution<std::mt19937::result_type> gDist;
game* gBound{};
int gSfxVolume{100};

struct SfxSlot {
	const char* id;
	const char* displayName;
	const char* defaultPath;
	std::string customPath;
	SOUNDDATA sound{};
	bool loaded{};
};

// default item SFX paths (LR2 system wavs).
SfxSlot gSfx[] = {
	{"item_get", "Item get", "LR2files/Sound/lr2/o-open.wav"},
	{"item_upgrade", "Item upgrade", "LR2files/Sound/lr2/o-change.wav"},
	{"item_received", "Item received", "LR2files/Sound/lr2/f-close.wav"},
	{"item_send", "Item send", "LR2files/Sound/lr2/f-open.wav"},
};

void UnloadSfx(SfxSlot& s) {
	if (gBound != nullptr && s.loaded) {
		ReleaseSound(&gBound->audio, &s.sound);
	}
	s.loaded = false;
	s.sound = {};
}

void EnsureSfxLoaded(game* g) {
	if (g == nullptr) return;
	for (auto& s : gSfx) {
		if (s.loaded) continue;
		const char* usePath = s.customPath.empty() ? s.defaultPath : s.customPath.c_str();
		CSTR path(usePath);
		LoadSound(&g->audio, &s.sound, path, false);
		if (s.sound.load) {
			s.loaded = true;
			SetSoundVolume(&g->audio, &s.sound, static_cast<float>(gSfxVolume) / 100.f);
		}
	}
}

void PlaySfxImpl(const char* which) {
	if (gBound == nullptr || which == nullptr) return;
	EnsureSfxLoaded(gBound);
	for (auto& s : gSfx) {
		if (std::strcmp(s.id, which) != 0) continue;
		if (s.loaded) {
			PlaySound(&gBound->audio, &s.sound, gBound->audio.chnKey, -1);
			return;
		}
		// Fallback: system option_change click.
		if (gBound->audio.sysSound.option_change.load) {
			PlaySound(&gBound->audio, &gBound->audio.sysSound.option_change, gBound->audio.chnKey, -1);
		}
		return;
	}
}

void ApplyEffect(const CurrentItem& item) {
	if (item.rolledItemId < 0 || item.rolledItemId >= static_cast<int>(gItems.size())) return;
	auto& i = gItems[static_cast<std::size_t>(item.rolledItemId)];
	if (i.field == nullptr) return;
	switch (item.level) {
	case 1: *i.field = static_cast<int>(i.lv1); break;
	case 2: *i.field = static_cast<int>(i.lv2); break;
	case 3: *i.field = static_cast<int>(i.lv3); break;
	default: break;
	}
}

void ResetEffect(const CurrentItem& item) {
	if (item.rolledItemId < 0 || item.rolledItemId >= static_cast<int>(gItems.size())) return;
	auto& i = gItems[static_cast<std::size_t>(item.rolledItemId)];
	if (i.field == nullptr) return;
	const auto n = std::count_if(gActive.begin(), gActive.end(), [&](const auto& kv) {
		return kv.first.rolledItemId == item.rolledItemId;
	});
	if (n == 1) *i.field = static_cast<int>(i.defaultVal);
}

} // namespace

void SetupDefaults() {
	// item icons.
	gItems = {
		// icon: ASCII short label (no Font Awesome embed).
		{"Earthquake", "Eq", nullptr, 0, 15, 30, 50, 50},
		{"Tornado", "Tn", nullptr, 0, 15, 30, 50, 30},
		{"Superloop", "Sl", nullptr, 0, 5, 15, 30, 10},
		{"Char", "Ch", nullptr, 0, 40, 60, 80, 30},
		{"Heartbeat", "Hb", nullptr, 0, 10, 25, 40, 50},
		{"Nabeatsu", "Nb", nullptr, 0, 25, 50, 100, 30},
		{"Acceleration", "Ac", nullptr, 0, 1, 2, 3, 30},
		{"Sin curve", "Si", nullptr, 0, 15, 30, 50, 30},
		{"Wave", "Wv", nullptr, 0, 50, 75, 100, 50},
		{"Spiral", "Sp", nullptr, 0, 20, 35, 50, 30},
		{"Sidejump", "Sj", nullptr, 0, 30, 50, 70, 30},
	};
	UpdateItemWeights();
}

void BindGame(game* g) {
	gBound = g;
	if (g == nullptr) return;
	auto& p = g->config.play;
	if (gItems.empty()) SetupDefaults();
	int* fields[] = {
		&p.m_earthquake,
		&p.m_tornado,
		&p.m_superloop,
		&p.m_char,
		&p.m_heartbeat,
		&p.m_nabeatsu,
		&p.m_accel,
		&p.m_sincurve,
		&p.m_wave,
		&p.m_spiral,
		&p.m_sidejump,
	};
	constexpr std::size_t kFieldCount = sizeof(fields) / sizeof(fields[0]);
	for (std::size_t i = 0; i < gItems.size() && i < kFieldCount; ++i) {
		gItems[i].field = fields[i];
	}
	EnsureSfxLoaded(g);
}

void ShutdownSfx() {
	if (gBound == nullptr) return;
	for (auto& s : gSfx) {
		if (s.loaded) {
			ReleaseSound(&gBound->audio, &s.sound);
			s.loaded = false;
			s.sound = {};
		}
	}
}

void ResetState() {
	gCurrentCombo = 0;
	gHeld = {};
}

void ResetMatch() {
	ResetState();
	if (gBound != nullptr) {
		for (auto& [item, _] : gActive) {
			ResetEffect(item);
		}
	}
	gActive.clear();
}

void SetItemModeEnabled(bool on) {
	gItemMode = on;
}

bool IsItemModeEnabled() {
	return gItemMode;
}

void OnComboIncrease(game* g, int totalNotes) {
	if (!gItemMode || g == nullptr) return;
	BindGame(g);
	if (totalNotes > 0) {
		gThreshold = static_cast<unsigned>(std::round(static_cast<float>(totalNotes) * gThresholdMult));
		if (gThreshold < 1) gThreshold = 1;
	}
	if (gHeld.final) return;
	++gCurrentCombo;
	if (gCurrentCombo == gThreshold) {
		gHeld.rolledItemId = static_cast<int>(gDist(gRng));
		gHeld.level = 1;
		PlaySfxImpl("item_get");
	} else if (gCurrentCombo == gThreshold * 2) {
		gHeld.level = 2;
		PlaySfxImpl("item_upgrade");
	} else if (gCurrentCombo == gThreshold * 3) {
		gHeld.level = 3;
		PlaySfxImpl("item_upgrade");
	}
}

void OnComboBreak() {
	gCurrentCombo = 0;
	if (gHeld.rolledItemId != -1) gHeld.final = true;
}

bool UseItem(game* g) {
	if (g == nullptr || gHeld.rolledItemId < 0) return false;
	if (!openlr2::arena_backend::Active().IsConnected(g)) return false;
	PlaySfxImpl("item_send");
	char json[128];
	std::snprintf(
		json,
		sizeof(json),
		"{\"rolledItemId\":%d,\"level\":%u}",
		gHeld.rolledItemId,
		static_cast<unsigned>(gHeld.level)
	);
	openlr2::arena_backend::Active().SendItemJson(g, json);
	ResetState();
	return true;
}

void TriggerItem(game* g, CurrentItem item) {
	if (g == nullptr) return;
	if (item.rolledItemId < 0 || item.rolledItemId >= static_cast<int>(gItems.size())
		|| item.level < 1 || item.level > 3) {
		return;
	}
	BindGame(g);
	PlaySfxImpl("item_received");
	auto it = gActive.find(item);
	if (it != gActive.end()) {
		it->second += kItemTimeMs;
		ApplyEffect(item);
	} else {
		gActive.insert({item, kItemTimeMs});
		ApplyEffect(item);
	}
}

void TickActive(game* g, int dtMs) {
	if (gActive.empty()) return;
	BindGame(g);
	std::vector<CurrentItem> expired;
	for (auto& [item, rem] : gActive) {
		ApplyEffect(item);
		rem -= dtMs;
		if (rem <= 0) expired.push_back(item);
	}
	for (const auto& item : expired) {
		ResetEffect(item);
		gActive.erase(item);
	}
}

void UpdateItemWeights() {
	std::vector<unsigned> weights;
	weights.reserve(gItems.size());
	for (const auto& i : gItems) weights.push_back(i.weight);
	if (std::all_of(weights.begin(), weights.end(), [](unsigned w) { return w == 0; })) return;
	gDist = std::discrete_distribution<std::mt19937::result_type>(weights.begin(), weights.end());
}

void SendItemSettings(game* g) {
	if (g == nullptr) return;
	std::string json = "{\"threshold\":" + std::to_string(static_cast<int>(gThresholdMult * 100)) + ",\"settings\":[";
	for (std::size_t i = 0; i < gItems.size(); ++i) {
		if (i) json += ',';
		json += "{\"lv1\":" + std::to_string(gItems[i].lv1);
		json += ",\"lv2\":" + std::to_string(gItems[i].lv2);
		json += ",\"lv3\":" + std::to_string(gItems[i].lv3);
		json += ",\"weight\":" + std::to_string(gItems[i].weight) + '}';
	}
	json += "]}";
	openlr2::arena_backend::Active().SendItemSettingsJson(g, json.c_str());
}

void ApplyItemSettings(const ItemSettingsPayload& settings) {
	if (settings.threshold < 1 || settings.threshold > 100) return;
	if (settings.settings.size() != gItems.size()) return;
	gThresholdMult = static_cast<float>(settings.threshold) / 100.0f;
	for (std::size_t i = 0; i < gItems.size(); ++i) {
		gItems[i].lv1 = settings.settings[i].lv1;
		gItems[i].lv2 = settings.settings[i].lv2;
		gItems[i].lv3 = settings.settings[i].lv3;
		gItems[i].weight = settings.settings[i].weight;
	}
	UpdateItemWeights();
	gSettingsRemoteUpdated = true;
}

void SetSfxVolume(int percent) {
	if (percent < 0) percent = 0;
	if (percent > 100) percent = 100;
	gSfxVolume = percent;
	if (gBound == nullptr) return;
	for (auto& s : gSfx) {
		if (s.loaded) {
			SetSoundVolume(&gBound->audio, &s.sound, static_cast<float>(gSfxVolume) / 100.f);
		}
	}
}

int SfxVolume() {
	return gSfxVolume;
}

void PlaySfx(const char* id) {
	PlaySfxImpl(id);
}

std::vector<SfxInfo> ListSfx() {
	std::vector<SfxInfo> out;
	out.reserve(sizeof(gSfx) / sizeof(gSfx[0]));
	for (const auto& s : gSfx) {
		out.push_back({s.id, s.displayName, s.customPath});
	}
	return out;
}

void SetSfxCustomPath(const char* id, const char* path) {
	if (id == nullptr || path == nullptr || path[0] == '\0') return;
	for (auto& s : gSfx) {
		if (std::strcmp(s.id, id) != 0) continue;
		UnloadSfx(s);
		s.customPath = path;
		EnsureSfxLoaded(gBound);
		return;
	}
}

void ApplySfxCustomPathFromConfig(const char* id, const char* path) {
	if (id == nullptr) return;
	for (auto& s : gSfx) {
		if (std::strcmp(s.id, id) != 0) continue;
		UnloadSfx(s);
		s.customPath = (path != nullptr && path[0] != '\0') ? path : "";
		return;
	}
}

void ResetAllCustomSfx() {
	for (auto& s : gSfx) {
		UnloadSfx(s);
		s.customPath.clear();
	}
	EnsureSfxLoaded(gBound);
}

CurrentItem& HeldItem() {
	return gHeld;
}

std::vector<ItemDef>& Items() {
	return gItems;
}

std::unordered_map<CurrentItem, int, CurrentItemHash>& ActiveItems() {
	return gActive;
}

float& ThresholdMult() {
	return gThresholdMult;
}

unsigned& CurrentCombo() {
	return gCurrentCombo;
}

unsigned& Threshold() {
	return gThreshold;
}

bool& SettingsRemoteUpdated() {
	return gSettingsRemoteUpdated;
}

} // namespace openlr2::arena_maniac

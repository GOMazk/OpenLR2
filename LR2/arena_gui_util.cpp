#include "arena_gui_util.h"

#include "arena_maniac.h"
#include "structure.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

namespace openlr2::arena_gui {
namespace {

constexpr const char* kArenaIniPath = "LR2files/Config/arena.ini";

} // namespace

int ExScore(const openlr2::ArenaPeerScore& s) {
	const int v = s.pgreat * 2 + s.great;
	return v > 0 ? v : 0;
}

int BpScore(const openlr2::ArenaPeerScore& s) {
	const int v = s.bad + s.poor;
	return v > 0 ? v : 0;
}

int JudgedNotes(const openlr2::ArenaPeerScore& s) {
	const int v = s.poor + s.bad + s.good + s.great + s.pgreat;
	return v > 0 ? v : 0;
}

float ExRatePercent(const openlr2::ArenaPeerScore& s) {
	const int notes = s.currentNotes > 0 ? s.currentNotes : JudgedNotes(s);
	if (notes <= 0) return 100.0f;
	const float pct = (static_cast<float>(ExScore(s)) / static_cast<float>(notes * 2)) * 100.0f;
	return std::floor(pct * 100.0f) / 100.0f;
}

int ChartTotalNotes(game* g, const openlr2::ArenaRoomState& state) {
	if (g != nullptr) {
		int n = g->gameplay.player[PLAYER_1].totalnotes;
		if (n <= 0) n = g->gameplay.player[PLAYER_1].total_note;
		if (n > 0) return n;
		if (g->sSelect.cur_song >= 0 && g->sSelect.bmsList != nullptr) {
			n = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			if (n > 0) return n;
		}
	}
	int best = 0;
	for (const auto& p : state.players) {
		best = (std::max)(best, JudgedNotes(p.score));
		best = (std::max)(best, p.score.currentNotes);
		best = (std::max)(best, p.score.maxCombo);
	}
	return best;
}

int ChartMaxEx(game* g, const openlr2::ArenaRoomState& state) {
	return ChartTotalNotes(g, state) * 2;
}

const char* OptionLabel(unsigned option) {
	switch (option) {
	case 1: return "MIRROR";
	case 2: return "RANDOM";
	case 3: return "S-RAN";
	case 4: return "H-RAN";
	case 5: return "ALLSCR";
	default: return "";
	}
}

const char* GaugeLabel(unsigned gauge) {
	switch (gauge) {
	case 0: return "GROOVE";
	case 1: return "HARD";
	case 2: return "HAZARD";
	case 3: return "EASY";
	case 4: return "P-ATTACK";
	case 5: return "G-ATTACK";
	default: return "";
	}
}

namespace {

bool LoadIniBool(const char* key, bool defaultValue) {
	std::ifstream in(kArenaIniPath);
	if (!in) return defaultValue;
	const std::string prefix = std::string(key) + "=";
	std::string line;
	while (std::getline(in, line)) {
		if (line.rfind(prefix, 0) == 0) {
			const char* v = line.c_str() + prefix.size();
			return v[0] == '1' || v[0] == 't' || v[0] == 'T' || v[0] == 'y' || v[0] == 'Y';
		}
	}
	return defaultValue;
}

void SaveIniBool(const char* key, bool value) {
	std::vector<std::string> keep;
	{
		std::ifstream in(kArenaIniPath);
		std::string line;
		const std::string prefix = std::string(key) + "=";
		while (in && std::getline(in, line)) {
			if (line.rfind(prefix, 0) == 0) continue;
			if (!line.empty()) keep.push_back(line);
		}
	}
	std::ofstream out(kArenaIniPath, std::ios::trunc);
	if (!out) return;
	out << key << '=' << (value ? "1" : "0") << '\n';
	for (const auto& l : keep) out << l << '\n';
}

std::string LoadIniString(const char* key) {
	std::ifstream in(kArenaIniPath);
	if (!in) return {};
	const std::string prefix = std::string(key) + "=";
	std::string line;
	while (std::getline(in, line)) {
		if (line.rfind(prefix, 0) == 0) return line.substr(prefix.size());
	}
	return {};
}

void CopyIniToBuf(const std::string& value, char* out, size_t outCap) {
	if (out == nullptr || outCap == 0) return;
	out[0] = '\0';
	if (value.empty()) return;
	// load only if size < 128 (leave room for NUL in 128-byte buffers).
	if (value.size() + 1 > outCap) return;
	std::memcpy(out, value.data(), value.size());
	out[value.size()] = '\0';
}

} // namespace

void LoadLanUsername(char* out, size_t outCap) {
	CopyIniToBuf(LoadIniString("username"), out, outCap);
}

void LoadLanHost(char* out, size_t outCap) {
	CopyIniToBuf(LoadIniString("host"), out, outCap);
}

void SaveLanConnectCredentials(const char* host, const char* username) {
	// persist username+host on successful LAN Open.
	std::vector<std::string> keep;
	{
		std::ifstream in(kArenaIniPath);
		std::string line;
		while (in && std::getline(in, line)) {
			if (line.rfind("username=", 0) == 0) continue;
			if (line.rfind("host=", 0) == 0) continue;
			if (!line.empty()) keep.push_back(line);
		}
	}
	std::ofstream out(kArenaIniPath, std::ios::trunc);
	if (!out) return;
	if (username != nullptr && username[0] != '\0') {
		out << "username=" << username << '\n';
	}
	if (host != nullptr && host[0] != '\0') {
		out << "host=" << host << '\n';
	}
	for (const auto& l : keep) out << l << '\n';
}

bool LoadMuteGameInputs() {
	// default: mute ON when overlay is open.
	return LoadIniBool("muteGameInputs", true);
}

void SaveMuteGameInputs(bool mute) {
	SaveIniBool("muteGameInputs", mute);
}

bool LoadAutomaticGraph() {
	// config default: automaticGraph true.
	return LoadIniBool("automaticGraph", true);
}

void SaveAutomaticGraph(bool enabled) {
	SaveIniBool("automaticGraph", enabled);
}

bool LoadRandomFlip() {
	return LoadIniBool("random_flip", false);
}

void SaveRandomFlip(bool enabled) {
	SaveIniBool("random_flip", enabled);
}

int LoadItemSfxVolume() {
	std::ifstream in(kArenaIniPath);
	if (!in) return 100;
	const std::string prefix = "item_sound_volume=";
	std::string line;
	while (std::getline(in, line)) {
		if (line.rfind(prefix, 0) == 0) {
			int v = 100;
			if (std::sscanf(line.c_str() + prefix.size(), "%d", &v) == 1) {
				if (v < 0) v = 0;
				if (v > 100) v = 100;
				return v;
			}
		}
	}
	return 100;
}

void SaveItemSfxVolume(int percent) {
	if (percent < 0) percent = 0;
	if (percent > 100) percent = 100;
	std::vector<std::string> keep;
	{
		std::ifstream in(kArenaIniPath);
		std::string line;
		const std::string prefix = "item_sound_volume=";
		while (in && std::getline(in, line)) {
			if (line.rfind(prefix, 0) == 0) continue;
			if (!line.empty()) keep.push_back(line);
		}
	}
	std::ofstream out(kArenaIniPath, std::ios::trunc);
	if (!out) return;
	out << "item_sound_volume=" << percent << '\n';
	for (const auto& l : keep) out << l << '\n';
}

bool IsSfxPathKey(const std::string& line) {
	return line.rfind("item_sfx_", 0) == 0;
}

void LoadItemSfxPathsIntoManiac() {
	std::ifstream in(kArenaIniPath);
	if (!in) return;
	std::string line;
	while (std::getline(in, line)) {
		if (!IsSfxPathKey(line)) continue;
		const auto eq = line.find('=');
		if (eq == std::string::npos || eq <= 9) continue;
		const std::string id = line.substr(9, eq - 9); // after "item_sfx_"
		const std::string path = line.substr(eq + 1);
		openlr2::arena_maniac::ApplySfxCustomPathFromConfig(id.c_str(), path.c_str());
	}
}

void SaveItemSfxPath(const char* id, const char* path) {
	if (id == nullptr || id[0] == '\0') return;
	const std::string key = std::string("item_sfx_") + id + "=";
	std::vector<std::string> keep;
	{
		std::ifstream in(kArenaIniPath);
		std::string line;
		while (in && std::getline(in, line)) {
			if (line.rfind(key, 0) == 0) continue;
			if (!line.empty()) keep.push_back(line);
		}
	}
	std::ofstream out(kArenaIniPath, std::ios::trunc);
	if (!out) return;
	if (path != nullptr && path[0] != '\0') {
		out << key << path << '\n';
	}
	for (const auto& l : keep) out << l << '\n';
}

void ClearItemSfxPaths() {
	std::vector<std::string> keep;
	{
		std::ifstream in(kArenaIniPath);
		std::string line;
		while (in && std::getline(in, line)) {
			if (IsSfxPathKey(line)) continue;
			if (!line.empty()) keep.push_back(line);
		}
	}
	std::ofstream out(kArenaIniPath, std::ios::trunc);
	if (!out) return;
	for (const auto& l : keep) out << l << '\n';
}

} // namespace openlr2::arena_gui

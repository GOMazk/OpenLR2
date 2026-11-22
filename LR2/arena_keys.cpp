#include "arena_keys.h"

#include "En_input.h"
#include "structure.h"

#include <DxLib.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace openlr2::arena_keys {
namespace {

constexpr const char* kArenaIniPath = "LR2files/Config/arena.ini";

constexpr const char* kMidiNotes[] = {
	"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

struct BindDef {
	Binding id;
	const char* label;
	const char* iniId; // id prefix
	Key defaultKey;
};

// Defaults: Insert / PgUp / Backspace (DxLib KEY_INPUT_*).
const BindDef kDefs[] = {
	{Binding::MenuToggle, "Menu toggle", "menuToggle", {DeviceType::Keyboard, static_cast<unsigned>(KEY_INPUT_INSERT)}},
	{Binding::GraphToggle, "Graph toggle", "graphToggle", {DeviceType::Keyboard, static_cast<unsigned>(KEY_INPUT_PGUP)}},
	{Binding::ItemTrigger, "Item trigger", "itemTrigger", {DeviceType::Keyboard, static_cast<unsigned>(KEY_INPUT_BACK)}},
};

Key gKeys[BindingCount]{};

// DxLib KEY_INPUT_* == DIK.
const char* KeyboardName(unsigned v) {
	switch (v) {
	case 0x01: return "ESCAPE";
	case 0x02: return "1";
	case 0x03: return "2";
	case 0x04: return "3";
	case 0x05: return "4";
	case 0x06: return "5";
	case 0x07: return "6";
	case 0x08: return "7";
	case 0x09: return "8";
	case 0x0A: return "9";
	case 0x0B: return "0";
	case 0x0C: return "MINUS";
	case 0x0D: return "EQUALS";
	case 0x0E: return "BACK";
	case 0x0F: return "TAB";
	case 0x10: return "Q";
	case 0x11: return "W";
	case 0x12: return "E";
	case 0x13: return "R";
	case 0x14: return "T";
	case 0x15: return "Y";
	case 0x16: return "U";
	case 0x17: return "I";
	case 0x18: return "O";
	case 0x19: return "P";
	case 0x1A: return "LBRACKET";
	case 0x1B: return "RBRACKET";
	case 0x1C: return "RETURN";
	case 0x1D: return "LCONTROL";
	case 0x1E: return "A";
	case 0x1F: return "S";
	case 0x20: return "D";
	case 0x21: return "F";
	case 0x22: return "G";
	case 0x23: return "H";
	case 0x24: return "J";
	case 0x25: return "K";
	case 0x26: return "L";
	case 0x27: return "SEMICOLON";
	case 0x28: return "APOSTROPHE";
	case 0x29: return "GRAVE";
	case 0x2A: return "LSHIFT";
	case 0x2B: return "BACKSLASH";
	case 0x2C: return "Z";
	case 0x2D: return "X";
	case 0x2E: return "C";
	case 0x2F: return "V";
	case 0x30: return "B";
	case 0x31: return "N";
	case 0x32: return "M";
	case 0x33: return "COMMA";
	case 0x34: return "PERIOD";
	case 0x35: return "SLASH";
	case 0x36: return "RSHIFT";
	case 0x37: return "MULTIPLY";
	case 0x38: return "LMENU";
	case 0x39: return "SPACE";
	case 0x3A: return "CAPITAL";
	case 0x3B: return "F1";
	case 0x3C: return "F2";
	case 0x3D: return "F3";
	case 0x3E: return "F4";
	case 0x3F: return "F5";
	case 0x40: return "F6";
	case 0x41: return "F7";
	case 0x42: return "F8";
	case 0x43: return "F9";
	case 0x44: return "F10";
	case 0x45: return "NUMLOCK";
	case 0x46: return "SCROLL";
	case 0x47: return "NUMPAD7";
	case 0x48: return "NUMPAD8";
	case 0x49: return "NUMPAD9";
	case 0x4A: return "SUBTRACT";
	case 0x4B: return "NUMPAD4";
	case 0x4C: return "NUMPAD5";
	case 0x4D: return "NUMPAD6";
	case 0x4E: return "ADD";
	case 0x4F: return "NUMPAD1";
	case 0x50: return "NUMPAD2";
	case 0x51: return "NUMPAD3";
	case 0x52: return "NUMPAD0";
	case 0x53: return "DECIMAL";
	case 0x57: return "F11";
	case 0x58: return "F12";
	case 0x7D: return "YEN";
	case 0x90: return "PREVTRACK";
	case 0x91: return "AT";
	case 0x92: return "COLON";
	case 0x94: return "KANJI";
	case 0x9C: return "NUMPADENTER";
	case 0x9D: return "RCONTROL";
	case 0xB5: return "DIVIDE";
	case 0xB7: return "SYSRQ";
	case 0xB8: return "RMENU";
	case 0xC5: return "PAUSE";
	case 0xC7: return "HOME";
	case 0xC8: return "UP";
	case 0xC9: return "PRIOR";
	case 0xCB: return "LEFT";
	case 0xCD: return "RIGHT";
	case 0xCF: return "END";
	case 0xD0: return "DOWN";
	case 0xD1: return "NEXT";
	case 0xD2: return "INSERT";
	case 0xD3: return "DELETE";
	case 0xDB: return "LWIN";
	case 0xDC: return "RWIN";
	case 0xDD: return "APPS";
	default: break;
	}
	static char buf[32]{};
	std::snprintf(buf, sizeof(buf), "KEY#%u", v);
	return buf;
}

// Controller value: padIndex * 32 + button (0–31). Pads 0–3 in OpenLR2 inputID map.
constexpr unsigned kPadStride = 32;
constexpr unsigned kMaxPads = 4;

unsigned ContPad(unsigned packed) { return packed / kPadStride; }
unsigned ContBtn(unsigned packed) { return packed % kPadStride; }

} // namespace

void Load() {
	for (std::size_t i = 0; i < BindingCount; ++i) {
		gKeys[i] = kDefs[i].defaultKey;
	}
	std::ifstream in(kArenaIniPath);
	if (!in) return;
	std::string line;
	while (std::getline(in, line)) {
		for (const auto& d : kDefs) {
			const auto idx = static_cast<std::size_t>(d.id);
			const std::string typePrefix = std::string(d.iniId) + "_controller_type=";
			const std::string valPrefix = std::string(d.iniId) + "_keybind=";
			const std::string legacy = std::string("bind_") + (
				d.id == Binding::MenuToggle ? "menu" : d.id == Binding::GraphToggle ? "graph" : "item"
			) + "=";
			if (line.rfind(typePrefix, 0) == 0) {
				int t = 0;
				if (std::sscanf(line.c_str() + typePrefix.size(), "%d", &t) == 1) {
					gKeys[idx].type = static_cast<DeviceType>(t);
				}
			} else if (line.rfind(valPrefix, 0) == 0) {
				int v = 0;
				if (std::sscanf(line.c_str() + valPrefix.size(), "%d", &v) == 1 && v >= 0) {
					gKeys[idx].value = static_cast<unsigned>(v);
				}
			} else if (line.rfind(legacy, 0) == 0) {
				// Pre-DeviceType arena.ini: bare DxLib KEY_INPUT_*.
				int v = 0;
				if (std::sscanf(line.c_str() + legacy.size(), "%d", &v) == 1 && v > 0) {
					gKeys[idx] = {DeviceType::Keyboard, static_cast<unsigned>(v)};
				}
			}
		}
	}
	for (std::size_t i = 0; i < BindingCount; ++i) {
		const auto& k = gKeys[i];
		if (k.type == DeviceType::Keyboard && k.value >= 0xEE) {
			gKeys[i] = kDefs[i].defaultKey;
		} else if (k.type == DeviceType::Controller
			&& (ContPad(k.value) >= kMaxPads || ContBtn(k.value) >= 32)) {
			gKeys[i] = kDefs[i].defaultKey;
		} else if (k.type == DeviceType::Midi && k.value >= 128) {
			gKeys[i] = kDefs[i].defaultKey;
		} else if (k.type != DeviceType::Keyboard && k.type != DeviceType::Controller
			&& k.type != DeviceType::Midi) {
			gKeys[i] = kDefs[i].defaultKey;
		}
	}
}

void Save() {
	bool mute = true;
	std::vector<std::string> other;
	{
		std::ifstream in(kArenaIniPath);
		std::string line;
		while (in && std::getline(in, line)) {
			if (line.rfind("muteGameInputs=", 0) == 0) {
				const char* v = line.c_str() + 15;
				mute = v[0] == '1' || v[0] == 't' || v[0] == 'T' || v[0] == 'y' || v[0] == 'Y';
				continue;
			}
			bool knownBind = false;
			for (const auto& d : kDefs) {
				const std::string t = std::string(d.iniId) + "_controller_type=";
				const std::string k = std::string(d.iniId) + "_keybind=";
				const std::string legacy = std::string("bind_") + (
					d.id == Binding::MenuToggle ? "menu" : d.id == Binding::GraphToggle ? "graph" : "item"
				) + "=";
				if (line.rfind(t, 0) == 0 || line.rfind(k, 0) == 0 || line.rfind(legacy, 0) == 0) {
					knownBind = true;
					break;
				}
			}
			if (knownBind) continue;
			if (!line.empty()) other.push_back(line);
		}
	}
	std::ofstream out(kArenaIniPath, std::ios::trunc);
	if (!out) return;
	out << "muteGameInputs=" << (mute ? "1" : "0") << '\n';
	for (const auto& d : kDefs) {
		const auto& k = gKeys[static_cast<std::size_t>(d.id)];
		out << d.iniId << "_controller_type=" << static_cast<unsigned>(k.type) << '\n';
		out << d.iniId << "_keybind=" << k.value << '\n';
	}
	for (const auto& l : other) out << l << '\n';
}

Key Get(Binding b) {
	const auto i = static_cast<std::size_t>(b);
	if (i >= BindingCount) return {};
	return gKeys[i];
}

bool Bind(Binding b, Key key) {
	if (key.type == DeviceType::None) return false;
	for (std::size_t i = 0; i < BindingCount; ++i) {
		if (static_cast<Binding>(i) == b) continue;
		if (gKeys[i] == key) return false;
	}
	Set(b, key);
	return true;
}

void Set(Binding b, Key key) {
	const auto i = static_cast<std::size_t>(b);
	if (i >= BindingCount) return;
	gKeys[i] = key;
}

const char* Label(Binding b) {
	const auto i = static_cast<std::size_t>(b);
	if (i >= BindingCount) return "";
	return kDefs[i].label;
}

std::string ToString(Key key) {
	if (key.type == DeviceType::Controller) {
		const unsigned pad = ContPad(key.value);
		const unsigned btn = ContBtn(key.value);
		if (pad == 0) {
			return "Button " + std::to_string(btn) + " (Controller)";
		}
		return "Button " + std::to_string(btn) + " (Pad " + std::to_string(pad) + ")";
	}
	if (key.type == DeviceType::Keyboard) {
		return KeyboardName(key.value);
	}
	if (key.type == DeviceType::Midi) {
		const int octave = static_cast<int>(key.value / 12) - 2;
		return std::string(kMidiNotes[key.value % 12]) + std::to_string(octave) + " (MIDI)";
	}
	return {};
}

int InputId(Key key) {
	if (key.type == DeviceType::Keyboard) {
		return static_cast<int>(key.value);
	}
	if (key.type == DeviceType::Controller) {
		// OpenLR2: padN buttons at 0x100 + pad*0x20 + 4 + button
		// (0..3 within pad = POV; buttons start at +4).
		const unsigned pad = ContPad(key.value);
		const unsigned btn = ContBtn(key.value);
		if (pad >= kMaxPads || btn >= 32) return 0;
		return static_cast<int>(0x100 + pad * 0x20 + 4 + btn);
	}
	if (key.type == DeviceType::Midi) {
		return 0x200 + static_cast<int>(key.value);
	}
	return 0;
}

Key FromInputId(int inputId) {
	if (inputId <= 0) return {};
	if (inputId < 0x100) {
		return {DeviceType::Keyboard, static_cast<unsigned>(inputId)};
	}
	if (inputId >= 0x100 && inputId < 0x180) {
		const int local = inputId - 0x100;
		const unsigned pad = static_cast<unsigned>(local / 0x20);
		const int within = local % 0x20;
		if (within < 4) return {}; // POV — ignore for Arena binds
		return {
			DeviceType::Controller,
			pad * kPadStride + static_cast<unsigned>(within - 4)
		};
	}
	if (inputId >= 0x200 && inputId < 0x300) {
		return {DeviceType::Midi, static_cast<unsigned>(inputId - 0x200)};
	}
	return {};
}

bool IsDuplicate(Binding self, Key key) {
	for (std::size_t i = 0; i < BindingCount; ++i) {
		if (static_cast<Binding>(i) == self) continue;
		if (gKeys[i] == key) return true;
	}
	return false;
}

bool EdgeHit(game* g, Binding b, int& prevHeld) {
	const Key key = Get(b);
	int now = 0;
	if (key.type == DeviceType::Keyboard) {
		// DxLib hardware state — independent of inputID wipe / ReactInput skip races.
		const int dx = static_cast<int>(key.value);
		now = (dx > 0 && CheckHitKey(dx)) ? 1 : 0;
	} else {
		const int id = InputId(key);
		if (id > 0 && g != nullptr && id < 0x600) {
			const unsigned char st = g->KeyInput.inputID[id];
			now = (st == 1 || st == 2) ? 1 : 0;
		}
	}
	const bool edge = now != 0 && prevHeld == 0;
	prevHeld = now;
	return edge;
}

Key PollNewPress(game* g) {
	if (g == nullptr) return {};
	// Prefer OpenLR2 unified edge (pad/MIDI/keyboard) when inputID is live.
	const int id = FindPressedKey(&g->KeyInput);
	if (id == KEY_INPUT_ESCAPE) return {};
	if (id > 0) return FromInputId(id);
	// Fallback: DxLib keyboard scan (bind modal while mute / before ReactInput).
	for (int k = 1; k < 256; ++k) {
		if (k == KEY_INPUT_ESCAPE) continue;
		if (!CheckHitKey(k)) continue;
		return {DeviceType::Keyboard, static_cast<unsigned>(k)};
	}
	return {};
}

} // namespace openlr2::arena_keys

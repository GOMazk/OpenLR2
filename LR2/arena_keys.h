#pragma once

#include <cstddef>
#include <string>

struct game;

// DeviceType + BindingType, backed by OpenLR2 inputID space.
namespace openlr2::arena_keys {

enum class DeviceType : unsigned {
	None = 1,
	Keyboard = 2,
	Controller = 3,
	Midi = 4,
};

enum class Binding : int {
	MenuToggle = 0,
	GraphToggle,
	ItemTrigger,
	Count
};

struct Key {
	DeviceType type{DeviceType::None};
	unsigned value{0};

	bool operator==(const Key& o) const {
		return type == o.type && value == o.value;
	}
};

void Load();
void Save();

[[nodiscard]] Key Get(Binding b);
/** @return false if duplicate. */
bool Bind(Binding b, Key key);
void Set(Binding b, Key key);

[[nodiscard]] const char* Label(Binding b);
[[nodiscard]] std::string ToString(Key key);

/** OpenLR2 KeyInput.inputID index for this binding (0 if none). */
[[nodiscard]] int InputId(Key key);
[[nodiscard]] Key FromInputId(int inputId);

[[nodiscard]] bool IsDuplicate(Binding self, Key key);

/** Rising-edge for keyboard / pad / MIDI via g->KeyInput.inputID. */
[[nodiscard]] bool EdgeHit(game* g, Binding b, int& prevHeld);

/** First newly pressed key while binding (FindPressedKey-compatible). */
[[nodiscard]] Key PollNewPress(game* g);

constexpr std::size_t BindingCount = static_cast<std::size_t>(Binding::Count);

} // namespace openlr2::arena_keys

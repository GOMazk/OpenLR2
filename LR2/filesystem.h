#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <filesystem>

namespace fs {

// \param in Null-terminated string
template <std::size_t N>
[[nodiscard]] consteval auto make_preferred(const char (&in)[N]) {
	std::array<char, N> out{};
	std::copy(in, in + N, out.data());
	for (auto &c : out) {
		if (c == '/' || c == '\\') {
			c = std::filesystem::path::preferred_separator;
		}
	}
	return out;
}

} // namespace fs

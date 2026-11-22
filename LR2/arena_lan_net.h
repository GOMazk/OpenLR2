#pragma once

// LAN network enums/structs (msgpack wire). Keep field order stable.

#include <msgpack.hpp>

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

namespace openlr2::arena_lan {

enum class ClientToServer : unsigned char {
	CTS_SELECTED_BMS = 1,
	CTS_PLAYER_SCORE,
	CTS_CHART_CANCELLED,
	CTS_LOADING_COMPLETE,
	CTS_USERNAME,
	CTS_MESSAGE,
	CTS_MISSING_CHART,
	CTS_SET_HOST,
	CTS_KICK_USER,
	CTS_ITEM,
	CTS_ITEM_SETTINGS,
};

enum class ServerToClient : unsigned char {
	STC_PLAYERS_SCORE = 1,
	STC_PLAYERS_READY_UPDATE,
	STC_SELECTED_CHART_RANDOM,
	STC_USERLIST,
	STC_CLIENT_REMOTE_ID,
	STC_MESSAGE,
	STC_MISSING_CHART,
	STC_ITEM,
	STC_ITEM_SETTINGS,
};

struct Address {
	std::string host;
	unsigned short port{};

	bool operator==(const Address& o) const {
		return host == o.host && port == o.port;
	}

	MSGPACK_DEFINE(host, port);
};

} // namespace openlr2::arena_lan

template <>
struct std::hash<openlr2::arena_lan::Address> {
	size_t operator()(const openlr2::arena_lan::Address& addr) const noexcept {
		return std::hash<std::string>()(addr.host) ^ (std::hash<int>()(addr.port) << 1);
	}
};

namespace openlr2::arena_lan {

struct Score {
	int poor{};
	int bad{};
	int good{};
	int great{};
	int p_great{};
	int max_combo{};
	int score{};
	int current_notes{};

	MSGPACK_DEFINE(poor, bad, good, great, p_great, max_combo, score, current_notes);
};

struct Peer {
	std::string username;
	std::string selectedHash;
	bool ready{};
	Score score{};
	unsigned option{};
	unsigned gauge{};

	MSGPACK_DEFINE(username, selectedHash, ready, score, option, gauge);
};

struct PeerList {
	std::unordered_map<Address, Peer> list;
	Address host;

	PeerList() = default;
	PeerList(std::unordered_map<Address, Peer> l, Address h)
		: list(std::move(l)), host(std::move(h)) {}

	MSGPACK_DEFINE(list, host);
};

struct SelectedBmsMessage {
	int randomSeed{};
	std::string hash;
	std::string title;
	std::string artist;
	unsigned option{};
	unsigned gauge{};
	bool itemModeEnabled{};

	MSGPACK_DEFINE(randomSeed, hash, title, artist, option, gauge, itemModeEnabled);
};

struct ScoreMessage {
	Score score{};
	Address player;

	ScoreMessage() = default;
	ScoreMessage(Score s, Address p) : score(std::move(s)), player(std::move(p)) {}

	MSGPACK_DEFINE(score, player);
};

struct Message {
	std::string message;
	Address player;
	bool systemMessage{};

	Message() = default;
	Message(std::string m, Address p, bool sys)
		: message(std::move(m)), player(std::move(p)), systemMessage(sys) {}

	MSGPACK_DEFINE(message, player, systemMessage);
};

struct CurrentItem {
	int rolledItemId{-1};
	unsigned short level{};
	bool final{};

	MSGPACK_DEFINE(rolledItemId, level);
};

struct ItemSetting {
	unsigned lv1{};
	unsigned lv2{};
	unsigned lv3{};
	unsigned weight{};

	MSGPACK_DEFINE(lv1, lv2, lv3, weight);
};

struct ItemSettings {
	unsigned threshold{};
	std::vector<ItemSetting> settings;

	MSGPACK_DEFINE(threshold, settings);
};

template <typename T>
inline T Unpack(const std::vector<char>& data) {
	auto oh = msgpack::unpack(data.data(), data.size());
	T res{};
	oh.get().convert(res);
	return res;
}

template <typename T>
inline std::vector<char> Pack(const T& msg) {
	msgpack::sbuffer buf;
	msgpack::pack(buf, msg);
	return {buf.data(), buf.data() + buf.size()};
}

[[nodiscard]] int AddressToPlayerId(const Address& a);
[[nodiscard]] Address PlayerIdToAddress(int playerId);
void RememberAddress(const Address& a);
void ClearAddressMap();

} // namespace openlr2::arena_lan

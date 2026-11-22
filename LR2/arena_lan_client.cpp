#include "arena_lan_client.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <thread>

namespace openlr2::arena_lan::client {
namespace {

ix::WebSocket gWs;
std::mutex gMu;
std::atomic<bool> gConnected{false};
bool gNetInited{};
std::string gHost;
std::string gUsername;

Address gRemoteId{};
Address gHostAddr{};
std::unordered_map<Address, Peer> gPeers;
std::string gChartHash;
std::string gChartTitle;
std::string gChartArtist;
int gRandomSeed{};
bool gItemMode{};
std::vector<std::string> gChat;
std::vector<openlr2::ArenaPendingItem> gPendingItems;
openlr2::ArenaItemSettings gItemSettings{};
bool gItemSettingsUpdated{};

void DestroyLocked() {
	gConnected.store(false);
	gRemoteId = {};
	gHostAddr = {};
	gPeers.clear();
	gChartHash.clear();
	gChartTitle.clear();
	gChartArtist.clear();
	gRandomSeed = 0;
	gItemMode = false;
	gChat.clear();
	gPendingItems.clear();
	gItemSettings = {};
	gItemSettingsUpdated = false;
	ClearAddressMap();
}

void ParsePacket(std::vector<char> data) {
	if (data.empty()) return;
	const auto id = static_cast<ServerToClient>(static_cast<unsigned char>(data.front()));
	data.erase(data.begin());

	std::lock_guard lock(gMu);
	switch (id) {
	case ServerToClient::STC_PLAYERS_SCORE: {
		auto msg = Unpack<ScoreMessage>(data);
		auto it = gPeers.find(msg.player);
		if (it != gPeers.end()) it->second.score = msg.score;
		break;
	}
	case ServerToClient::STC_PLAYERS_READY_UPDATE:
	case ServerToClient::STC_USERLIST: {
		auto pl = Unpack<PeerList>(data);
		gPeers = std::move(pl.list);
		gHostAddr = pl.host;
		for (const auto& [addr, _] : gPeers) RememberAddress(addr);
		RememberAddress(gHostAddr);
		break;
	}
	case ServerToClient::STC_SELECTED_CHART_RANDOM: {
		auto sel = Unpack<SelectedBmsMessage>(data);
		gChartHash = sel.hash;
		gChartTitle = sel.title;
		gChartArtist = sel.artist;
		gRandomSeed = sel.randomSeed;
		gItemMode = sel.itemModeEnabled;
		break;
	}
	case ServerToClient::STC_CLIENT_REMOTE_ID: {
		gRemoteId = Unpack<Address>(data);
		RememberAddress(gRemoteId);
		break;
	}
	case ServerToClient::STC_MESSAGE: {
		auto msg = Unpack<Message>(data);
		if (msg.systemMessage) {
			gChat.push_back(msg.message);
		} else {
			std::string name = "?";
			auto it = gPeers.find(msg.player);
			if (it != gPeers.end()) name = it->second.username;
			gChat.push_back(name + ": " + msg.message);
		}
		break;
	}
	case ServerToClient::STC_ITEM: {
		auto item = Unpack<CurrentItem>(data);
		openlr2::ArenaPendingItem p{};
		p.rolledItemId = item.rolledItemId;
		p.level = item.level;
		gPendingItems.push_back(p);
		break;
	}
	case ServerToClient::STC_ITEM_SETTINGS: {
		auto settings = Unpack<ItemSettings>(data);
		gItemSettings.threshold = settings.threshold;
		gItemSettings.settings.clear();
		for (const auto& row : settings.settings) {
			openlr2::ArenaItemSettingRow r{};
			r.lv1 = row.lv1;
			r.lv2 = row.lv2;
			r.lv3 = row.lv3;
			r.weight = row.weight;
			gItemSettings.settings.push_back(r);
		}
		gItemSettings.updated = true;
		gItemSettingsUpdated = true;
		break;
	}
	default:
		break;
	}
}

void OnMessage(const ix::WebSocketMessagePtr& msg) {
	if (msg->type == ix::WebSocketMessageType::Message) {
		ParsePacket(std::vector<char>(msg->str.begin(), msg->str.end()));
	} else if (msg->type == ix::WebSocketMessageType::Open) {
		gConnected.store(true);
		Send(ClientToServer::CTS_USERNAME, gUsername);
	} else if (msg->type == ix::WebSocketMessageType::Close
		|| msg->type == ix::WebSocketMessageType::Error) {
		std::lock_guard lock(gMu);
		DestroyLocked();
		gConnected.store(false);
	}
}

} // namespace

bool Connect(const char* host, const char* username) {
	Disconnect();
	if (host == nullptr || host[0] == '\0' || username == nullptr || username[0] == '\0') {
		return false;
	}
	if (!gNetInited) {
		ix::initNetSystem();
		gNetInited = true;
	}
	{
		std::lock_guard lock(gMu);
		gHost = host;
		gUsername = username;
		DestroyLocked();
	}
	const std::string url = "ws://" + std::string(host) + ":2222/";
	gWs.setUrl(url);
	gWs.setOnMessageCallback(OnMessage);
	gWs.disableAutomaticReconnection();
	gWs.start();
	// Wait briefly for Open + username handshake (AX is async; OL Join returns after connect).
	for (int i = 0; i < 50; ++i) {
		if (IsConnected()) return true;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	Disconnect();
	return false;
}

void Disconnect() {
	gWs.stop();
	std::lock_guard lock(gMu);
	DestroyLocked();
}

bool IsConnected() {
	return gConnected.load() && gWs.getReadyState() == ix::ReadyState::Open;
}

void Send(ClientToServer id, std::vector<char> data) {
	data.insert(data.begin(), static_cast<char>(id));
	if (gWs.getReadyState() == ix::ReadyState::Open && gConnected.load()) {
		gWs.sendBinary(data);
	}
}

void Send(ClientToServer id, const std::string& msg) {
	Send(id, std::vector<char>(msg.begin(), msg.end()));
}

void PollInto(openlr2::ArenaRoomState& out) {
	std::lock_guard lock(gMu);
	out = {};
	out.connected = gConnected;
	out.roomId = gHost.empty() ? "lan" : ("lan:" + gHost);
	out.selfPlayerId = AddressToPlayerId(gRemoteId);
	out.hostId = AddressToPlayerId(gHostAddr);
	out.itemModeEnabled = gItemMode;
	out.randomSeed = gRandomSeed;
	out.chartHash = gChartHash;
	out.chartTitle = gChartTitle;
	out.chartArtist = gChartArtist;
	out.chatLog = gChat;
	out.pendingItems = std::move(gPendingItems);
	gPendingItems.clear();
	out.itemSettings = gItemSettings;
	if (gItemSettingsUpdated) {
		out.itemSettings.updated = true;
		gItemSettingsUpdated = false;
		gItemSettings.updated = false;
	}
	for (const auto& [addr, peer] : gPeers) {
		openlr2::ArenaPeer p{};
		p.playerId = AddressToPlayerId(addr);
		p.username = peer.username;
		p.isHost = (addr == gHostAddr);
		p.ready = peer.ready;
		p.selectedHash = peer.selectedHash;
		p.option = peer.option;
		p.gauge = peer.gauge;
		openlr2::ArenaPeerScore sc{};
		sc.poor = peer.score.poor;
		sc.bad = peer.score.bad;
		sc.good = peer.score.good;
		sc.great = peer.score.great;
		sc.pgreat = peer.score.p_great;
		sc.maxCombo = peer.score.max_combo;
		sc.score = peer.score.score;
		sc.currentNotes = peer.score.current_notes;
		p.score = sc;
		out.players.push_back(std::move(p));
	}
}

void SendSelectedChart(
	int randomSeed,
	const char* hash,
	const char* title,
	const char* artist,
	unsigned option,
	unsigned gauge,
	bool itemModeEnabled
) {
	SelectedBmsMessage msg{};
	msg.randomSeed = randomSeed;
	msg.hash = hash ? hash : "";
	msg.title = title ? title : "";
	msg.artist = artist ? artist : "";
	msg.option = option;
	msg.gauge = gauge;
	msg.itemModeEnabled = itemModeEnabled;
	{
		std::lock_guard lock(gMu);
		gChartHash = msg.hash;
		gChartTitle = msg.title;
		gChartArtist = msg.artist;
		gRandomSeed = randomSeed;
		gItemMode = itemModeEnabled;
	}
	Send(ClientToServer::CTS_SELECTED_BMS, Pack(msg));
}

void SendScore(const openlr2::ArenaPeerScore& score) {
	Score lan{};
	lan.poor = score.poor;
	lan.bad = score.bad;
	lan.good = score.good;
	lan.great = score.great;
	lan.p_great = score.pgreat;
	lan.max_combo = score.maxCombo;
	lan.score = score.score;
	lan.current_notes = score.currentNotes;
	Send(ClientToServer::CTS_PLAYER_SCORE, Pack(lan));
}

void SendReady() {
	Send(ClientToServer::CTS_LOADING_COMPLETE, std::string{});
}

void SendCancelled() {
	Send(ClientToServer::CTS_CHART_CANCELLED, std::string{});
}

void SendChat(const char* message) {
	if (message == nullptr) return;
	Send(ClientToServer::CTS_MESSAGE, std::string(message));
}

void SendMissingChart() {
	Send(ClientToServer::CTS_MISSING_CHART, std::string{});
}

void SetHost(int playerId) {
	Address a = PlayerIdToAddress(playerId);
	if (a.host.empty()) return;
	Send(ClientToServer::CTS_SET_HOST, Pack(a));
}

void KickUser(int playerId) {
	Address a = PlayerIdToAddress(playerId);
	if (a.host.empty()) return;
	Send(ClientToServer::CTS_KICK_USER, Pack(a));
}

void SendItemJson(const char* jsonObject) {
	if (jsonObject == nullptr) return;
	int id = -1;
	unsigned level = 0;
	if (std::sscanf(jsonObject, "{\"rolledItemId\":%d,\"level\":%u}", &id, &level) != 2) return;
	CurrentItem item{};
	item.rolledItemId = id;
	item.level = static_cast<unsigned short>(level);
	Send(ClientToServer::CTS_ITEM, Pack(item));
}

void SendItemSettingsJson(const char* jsonObject) {
	if (jsonObject == nullptr) return;
	ItemSettings settings{};
	unsigned thr = 0;
	if (std::sscanf(jsonObject, "{\"threshold\":%u", &thr) != 1) return;
	settings.threshold = thr;
	const char* p = std::strstr(jsonObject, "\"settings\":[");
	if (p == nullptr) return;
	p += 12;
	while (*p && *p != ']') {
		while (*p && (*p == ',' || *p == ' ' || *p == '\n' || *p == '\t')) ++p;
		if (*p == ']') break;
		if (*p != '{') break;
		ItemSetting row{};
		if (std::sscanf(
				p,
				"{\"lv1\":%u,\"lv2\":%u,\"lv3\":%u,\"weight\":%u}",
				&row.lv1,
				&row.lv2,
				&row.lv3,
				&row.weight
			)
			!= 4) {
			break;
		}
		settings.settings.push_back(row);
		const char* next = std::strchr(p, '}');
		if (next == nullptr) break;
		p = next + 1;
	}
	if (settings.settings.empty()) return;
	Send(ClientToServer::CTS_ITEM_SETTINGS, Pack(settings));
}

} // namespace openlr2::arena_lan::client

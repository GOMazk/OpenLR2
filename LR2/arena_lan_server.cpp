#include "arena_lan_server.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocketServer.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>

namespace openlr2::arena_lan::server {
namespace {

std::shared_ptr<ix::WebSocketServer> gServer;
Address gHost{};
std::unordered_map<Address, Peer> gPeers;
ItemSettings gItemSettings{};
bool gItemMode{};
std::atomic<bool> gStarted{false};
bool gNetInited{};
std::mutex gMu;

void SendToEveryone(
	ServerToClient id,
	std::vector<char> data,
	const Address& origSender,
	bool includeOrigSender
) {
	if (!gServer) return;
	data.insert(data.begin(), static_cast<char>(id));
	const std::string origUrl = origSender.host + ":" + std::to_string(origSender.port);
	for (const auto& c : gServer->getClients()) {
		if (c->getUrl() == origUrl && !includeOrigSender) continue;
		c->sendBinary(data);
	}
}

void SendTo(ServerToClient id, std::vector<char> data, const Address& addr) {
	if (!gServer) return;
	data.insert(data.begin(), static_cast<char>(id));
	const std::string url = addr.host + ":" + std::to_string(addr.port);
	for (const auto& c : gServer->getClients()) {
		if (c->getUrl() == url) {
			c->sendBinary(data);
			return;
		}
	}
}

void ParsePacket(std::vector<char> data, Address clientAddr) {
	if (data.empty()) return;
	const auto id = static_cast<ClientToServer>(static_cast<unsigned char>(data.front()));
	data.erase(data.begin());

	std::lock_guard lock(gMu);
	switch (id) {
	case ClientToServer::CTS_SELECTED_BMS: {
		auto sel = Unpack<SelectedBmsMessage>(data);
		if (clientAddr == gHost) {
			gItemMode = sel.itemModeEnabled;
			for (auto& [k, p] : gPeers) {
				p.ready = false;
				p.selectedHash.clear();
				p.score = {};
			}
		}
		gPeers[clientAddr].selectedHash = sel.hash;
		gPeers[clientAddr].option = sel.option;
		gPeers[clientAddr].gauge = sel.gauge;
		if (clientAddr == gHost) {
			SendToEveryone(ServerToClient::STC_SELECTED_CHART_RANDOM, data, clientAddr, true);
		}
		break;
	}
	case ClientToServer::CTS_PLAYER_SCORE: {
		auto score = Unpack<Score>(data);
		gPeers[clientAddr].score = score;
		SendToEveryone(
			ServerToClient::STC_PLAYERS_SCORE,
			Pack(ScoreMessage(score, clientAddr)),
			clientAddr,
			true
		);
		break;
	}
	case ClientToServer::CTS_CHART_CANCELLED: {
		gPeers[clientAddr].ready = false;
		gPeers[clientAddr].selectedHash.clear();
		SendToEveryone(
			ServerToClient::STC_PLAYERS_READY_UPDATE,
			Pack(PeerList(gPeers, gHost)),
			clientAddr,
			true
		);
		break;
	}
	case ClientToServer::CTS_LOADING_COMPLETE: {
		gPeers[clientAddr].ready = true;
		SendToEveryone(
			ServerToClient::STC_PLAYERS_READY_UPDATE,
			Pack(PeerList(gPeers, gHost)),
			clientAddr,
			true
		);
		break;
	}
	case ClientToServer::CTS_USERNAME: {
		const std::string username(data.begin(), data.end());
		if (gPeers.empty()) gHost = clientAddr;
		gPeers[clientAddr] = Peer{};
		gPeers[clientAddr].username = username;
		RememberAddress(clientAddr);
		RememberAddress(gHost);
		SendTo(ServerToClient::STC_CLIENT_REMOTE_ID, Pack(clientAddr), clientAddr);
		if (!gItemSettings.settings.empty()) {
			SendTo(ServerToClient::STC_ITEM_SETTINGS, Pack(gItemSettings), clientAddr);
		}
		SendToEveryone(
			ServerToClient::STC_USERLIST,
			Pack(PeerList(gPeers, gHost)),
			clientAddr,
			true
		);
		break;
	}
	case ClientToServer::CTS_MESSAGE: {
		SendToEveryone(
			ServerToClient::STC_MESSAGE,
			Pack(Message(std::string(data.begin(), data.end()), clientAddr, false)),
			clientAddr,
			false
		);
		break;
	}
	case ClientToServer::CTS_MISSING_CHART: {
		const std::string text =
			"[!] " + gPeers[clientAddr].username + " is missing the selected chart!";
		SendToEveryone(
			ServerToClient::STC_MESSAGE,
			Pack(Message(text, clientAddr, true)),
			clientAddr,
			false
		);
		break;
	}
	case ClientToServer::CTS_SET_HOST: {
		if (!(clientAddr == gHost)) break;
		auto newHost = Unpack<Address>(data);
		if (gPeers.find(newHost) == gPeers.end()) break;
		gHost = newHost;
		SendToEveryone(
			ServerToClient::STC_USERLIST,
			Pack(PeerList(gPeers, gHost)),
			clientAddr,
			true
		);
		break;
	}
	case ClientToServer::CTS_KICK_USER: {
		if (!(clientAddr == gHost) || !gServer) break;
		auto kick = Unpack<Address>(data);
		const std::string url = kick.host + ":" + std::to_string(kick.port);
		for (const auto& c : gServer->getClients()) {
			if (c->getUrl() == url) c->close();
		}
		break;
	}
	case ClientToServer::CTS_ITEM:
		SendToEveryone(ServerToClient::STC_ITEM, data, clientAddr, false);
		break;
	case ClientToServer::CTS_ITEM_SETTINGS: {
		if (!(clientAddr == gHost)) break;
		gItemSettings = Unpack<ItemSettings>(data);
		SendToEveryone(ServerToClient::STC_ITEM_SETTINGS, data, clientAddr, false);
		break;
	}
	default:
		break;
	}
}

void OnClientMessage(
	std::shared_ptr<ix::ConnectionState> connectionState,
	ix::WebSocket& webSocket,
	const ix::WebSocketMessagePtr& msg
) {
	Address clientAddr{};
	clientAddr.host = connectionState->getRemoteIp();
	clientAddr.port = connectionState->getRemotePort();

	if (msg->type == ix::WebSocketMessageType::Open) {
		webSocket.setUrl(clientAddr.host + ":" + std::to_string(clientAddr.port));
	} else if (msg->type == ix::WebSocketMessageType::Message) {
		ParsePacket(std::vector<char>(msg->str.begin(), msg->str.end()), clientAddr);
	} else if (msg->type == ix::WebSocketMessageType::Close
		|| msg->type == ix::WebSocketMessageType::Error) {
		std::lock_guard lock(gMu);
		if (!gStarted.load()) return;
		gPeers.erase(clientAddr);
		if (clientAddr == gHost && !gPeers.empty()) {
			gHost = gPeers.begin()->first;
		}
		if (!gPeers.empty()) {
			auto res = Pack(PeerList(gPeers, gHost));
			res.insert(res.begin(), static_cast<char>(ServerToClient::STC_USERLIST));
			const std::string clientUrl =
				clientAddr.host + ":" + std::to_string(clientAddr.port);
			for (const auto& c : gServer->getClients()) {
				if (c->getUrl() == clientUrl) continue;
				c->sendBinary(res);
			}
		}
	}
}

} // namespace

bool Start(const char* host, unsigned short port) {
	if (!gNetInited) {
		ix::initNetSystem();
		gNetInited = true;
	}
	Stop();
	const char* bind = (host != nullptr && host[0] != '\0') ? host : "0.0.0.0";
	gServer = std::make_shared<ix::WebSocketServer>(
		port,
		bind,
		ix::SocketServer::kDefaultTcpBacklog,
		ix::SocketServer::kDefaultMaxConnections,
		ix::WebSocketServer::kDefaultHandShakeTimeoutSecs,
		ix::SocketServer::kDefaultAddressFamily,
		15
	);
	gServer->setOnClientMessageCallback(OnClientMessage);
	auto success = gServer->listen();
	if (!success.first) {
		gServer.reset();
		return false;
	}
	{
		std::lock_guard lock(gMu);
		gPeers.clear();
		gHost = {};
		gItemSettings = {};
		gItemMode = false;
	}
	gStarted.store(true);
	gServer->disablePerMessageDeflate();
	gServer->start();
	return true;
}

bool Stop() {
	if (gServer) {
		gServer->stop();
		gServer.reset();
	}
	gStarted.store(false);
	std::lock_guard lock(gMu);
	gPeers.clear();
	gHost = {};
	gItemSettings = {};
	gItemMode = false;
	return true;
}

bool IsRunning() {
	return gStarted.load();
}

} // namespace openlr2::arena_lan::server

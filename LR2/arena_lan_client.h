#pragma once

#include "arena_lan_net.h"
#include "LR2_customir_api.h"

#include <mutex>
#include <string>
#include <vector>

namespace openlr2::arena_lan::client {

bool Connect(const char* host, const char* username);
void Disconnect();
[[nodiscard]] bool IsConnected();

void Send(ClientToServer id, std::vector<char> data);
void Send(ClientToServer id, const std::string& msg);

/** Snapshot into OpenLR2 ArenaRoomState (thread-safe). */
void PollInto(openlr2::ArenaRoomState& out);

void SendSelectedChart(
	int randomSeed,
	const char* hash,
	const char* title,
	const char* artist,
	unsigned option,
	unsigned gauge,
	bool itemModeEnabled
);
void SendScore(const openlr2::ArenaPeerScore& score);
void SendReady();
void SendCancelled();
void SendChat(const char* message);
void SendMissingChart();
void SetHost(int playerId);
void KickUser(int playerId);
void SendItemJson(const char* jsonObject);
void SendItemSettingsJson(const char* jsonObject);

} // namespace openlr2::arena_lan::client

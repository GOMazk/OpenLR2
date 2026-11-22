#pragma once

#include "LR2_customir_api.h"

#include <string>

struct game;

namespace openlr2::arena_backend {

enum class Transport : int {
	Ir = 0,
	Lan = 1,
};

/** Active Arena transport (IR CustomIR vs LAN). */
[[nodiscard]] Transport GetTransport();
void SetTransport(Transport t);
void LoadTransportFromIni();
void SaveTransportToIni();

struct Backend {
	virtual ~Backend() = default;

	virtual openlr2::GetStatus JoinRoom(game* g, const char* roomId, openlr2::ArenaJoinResult& out) = 0;
	/** LAN only: connect as client to ws://host:2222/. */
	virtual bool ConnectLan(game* g, const char* host, const char* username) = 0;
	virtual bool StartLanServer(const char* bindHost, unsigned short port) = 0;
	virtual void StopLanServer() = 0;
	[[nodiscard]] virtual bool IsLanServerRunning() const = 0;

	virtual void Leave(game* g) = 0;
	[[nodiscard]] virtual bool IsConnected(game* g) = 0;
	virtual void PollState(game* g, openlr2::ArenaRoomState& out) = 0;

	virtual void SendSelectedChart(
		game* g,
		int randomSeed,
		const char* hash,
		const char* title,
		const char* artist,
		unsigned option,
		unsigned gauge,
		bool itemModeEnabled
	) = 0;
	virtual void SendScore(game* g, const openlr2::ArenaPeerScore& score, bool finalScore) = 0;
	virtual void SendReady(game* g) = 0;
	virtual void SendCancelled(game* g) = 0;
	virtual void SendChat(game* g, const char* message) = 0;
	virtual void SendMissingChart(game* g) = 0;
	virtual void SetHost(game* g, int playerId) = 0;
	virtual void KickUser(game* g, int playerId) = 0;
	virtual void SendItemJson(game* g, const char* jsonObject) = 0;
	virtual void SendItemSettingsJson(game* g, const char* jsonObject) = 0;
	virtual openlr2::GetStatus SubmitRoomResult(
		game* g,
		const char* roomId,
		const char* chartHash,
		const char* resultJson
	) = 0;
};

[[nodiscard]] Backend& Active();
[[nodiscard]] Backend& Ir();
[[nodiscard]] Backend& Lan();

} // namespace openlr2::arena_backend

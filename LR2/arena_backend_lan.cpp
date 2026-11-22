#include "arena_backend.h"

#include "arena_lan_client.h"
#include "arena_lan_server.h"

#include "structure.h"

namespace openlr2::arena_backend {
namespace {

class LanBackend final : public Backend {
public:
	openlr2::GetStatus JoinRoom(game*, const char*, openlr2::ArenaJoinResult&) override {
		return openlr2::GetStatus::Fail;
	}

	bool ConnectLan(game*, const char* host, const char* username) override {
		return openlr2::arena_lan::client::Connect(host, username);
	}

	bool StartLanServer(const char* bindHost, unsigned short port) override {
		return openlr2::arena_lan::server::Start(bindHost, port);
	}

	void StopLanServer() override {
		openlr2::arena_lan::server::Stop();
	}

	bool IsLanServerRunning() const override {
		return openlr2::arena_lan::server::IsRunning();
	}

	void Leave(game*) override {
		openlr2::arena_lan::client::Disconnect();
	}

	bool IsConnected(game*) override {
		return openlr2::arena_lan::client::IsConnected();
	}

	void PollState(game*, openlr2::ArenaRoomState& out) override {
		openlr2::arena_lan::client::PollInto(out);
	}

	void SendSelectedChart(
		game*,
		int randomSeed,
		const char* hash,
		const char* title,
		const char* artist,
		unsigned option,
		unsigned gauge,
		bool itemModeEnabled
	) override {
		openlr2::arena_lan::client::SendSelectedChart(
			randomSeed, hash, title, artist, option, gauge, itemModeEnabled
		);
	}

	void SendScore(game*, const openlr2::ArenaPeerScore& score, bool) override {
		openlr2::arena_lan::client::SendScore(score);
	}

	void SendReady(game*) override {
		openlr2::arena_lan::client::SendReady();
	}

	void SendCancelled(game*) override {
		openlr2::arena_lan::client::SendCancelled();
	}

	void SendChat(game*, const char* message) override {
		openlr2::arena_lan::client::SendChat(message);
	}

	void SendMissingChart(game*) override {
		openlr2::arena_lan::client::SendMissingChart();
	}

	void SetHost(game*, int playerId) override {
		openlr2::arena_lan::client::SetHost(playerId);
	}

	void KickUser(game*, int playerId) override {
		openlr2::arena_lan::client::KickUser(playerId);
	}

	void SendItemJson(game*, const char* jsonObject) override {
		openlr2::arena_lan::client::SendItemJson(jsonObject);
	}

	void SendItemSettingsJson(game*, const char* jsonObject) override {
		openlr2::arena_lan::client::SendItemSettingsJson(jsonObject);
	}

	openlr2::GetStatus SubmitRoomResult(game*, const char*, const char*, const char*) override {
		// LAN has no remote room-result archive.
		return openlr2::GetStatus::Ok;
	}
};

LanBackend gLan;

} // namespace

Backend& Lan() {
	return gLan;
}

} // namespace openlr2::arena_backend

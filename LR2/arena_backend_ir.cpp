#include "arena_backend.h"

#include "structure.h"

namespace openlr2::arena_backend {
namespace {

class IrBackend final : public Backend {
public:
	openlr2::GetStatus JoinRoom(game* g, const char* roomId, openlr2::ArenaJoinResult& out) override {
		if (g == nullptr) return openlr2::GetStatus::Fail;
		return g->net.customIR.ArenaJoinRoom(roomId, out);
	}

	bool ConnectLan(game*, const char*, const char*) override { return false; }
	bool StartLanServer(const char*, unsigned short) override { return false; }
	void StopLanServer() override {}
	bool IsLanServerRunning() const override { return false; }

	void Leave(game* g) override {
		if (g != nullptr) g->net.customIR.ArenaLeave();
	}

	bool IsConnected(game* g) override {
		return g != nullptr && g->net.customIR.ArenaIsConnected();
	}

	void PollState(game* g, openlr2::ArenaRoomState& out) override {
		if (g == nullptr) {
			out = {};
			return;
		}
		g->net.customIR.ArenaPollState(out);
	}

	void SendSelectedChart(
		game* g,
		int randomSeed,
		const char* hash,
		const char* title,
		const char* artist,
		unsigned option,
		unsigned gauge,
		bool itemModeEnabled
	) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaSendSelectedChart(
			randomSeed, hash, title, artist, option, gauge, itemModeEnabled
		);
	}

	void SendScore(game* g, const openlr2::ArenaPeerScore& score, bool finalScore) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaSendScore(score, finalScore);
	}

	void SendReady(game* g) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaSendReady();
	}

	void SendCancelled(game* g) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaSendCancelled();
	}

	void SendChat(game* g, const char* message) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaSendChat(message);
	}

	void SendMissingChart(game* g) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaSendMissingChart();
	}

	void SetHost(game* g, int playerId) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaSetHost(playerId);
	}

	void KickUser(game* g, int playerId) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaKickUser(playerId);
	}

	void SendItemJson(game* g, const char* jsonObject) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaSendItemJson(jsonObject);
	}

	void SendItemSettingsJson(game* g, const char* jsonObject) override {
		if (g == nullptr) return;
		g->net.customIR.ArenaSendItemSettingsJson(jsonObject);
	}

	openlr2::GetStatus SubmitRoomResult(
		game* g,
		const char* roomId,
		const char* chartHash,
		const char* resultJson
	) override {
		if (g == nullptr) return openlr2::GetStatus::Fail;
		return g->net.customIR.ArenaSubmitRoomResult(roomId, chartHash, resultJson);
	}
};

IrBackend gIr;

} // namespace

Backend& Ir() {
	return gIr;
}

} // namespace openlr2::arena_backend

#pragma once

#include "LR2_customir_api.h"

#include <filesystem>
#include <future>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <utility>
#include <vector>

struct RANKING;
struct STATUS;
struct game;
struct sqlite3;

class CustomIR;

class CUSTOMIR_MANAGER {
public:
	CUSTOMIR_MANAGER() = default;
	CUSTOMIR_MANAGER(const CUSTOMIR_MANAGER&) = delete;
	CUSTOMIR_MANAGER& operator=(const CUSTOMIR_MANAGER&) = delete;
	CUSTOMIR_MANAGER(CUSTOMIR_MANAGER&&) = delete;
	CUSTOMIR_MANAGER& operator=(CUSTOMIR_MANAGER&&) = delete;
	~CUSTOMIR_MANAGER();

	// \note Delegates to the display IR
	// \retval nullopt - Fail or not in cache
	std::optional<openlr2::IRRankResult> RestoreCachedRank(const char* songmd5);
	// \retval nullopt - Fail
	std::future<std::optional<openlr2::IRRankResult>>& GetResult() { return mResultIrFuture; };
	// Get the result with \ref GetResult
	void BeginResultIr(game& game, sqlite3* sql, int player, std::string ghost);
	void Initialize(const std::filesystem::path& directory, std::string activeProvider);
	// Call Login() on each CustomIR in parallel.
	// You must wait until all std::future are resolved.
	// \return [{name, login_result}]
	std::vector<std::pair<std::string_view, std::future<bool>>> Login();
	[[nodiscard]] bool IsDisplayIrOnline() const;
	// \note Delegates to the display IR
	// \retval "" - Fail
	[[nodiscard]] std::string GetWebRankingUrl(const char* songHash) const;
	// \note Delegates to the display IR
	// \retval nullopt - Fail
	std::optional<openlr2::IRGhostResult> TryGetTargetInfo(const char* songmd5, int mode, int targetPlayerId);

	struct RivalSyncTask {
		int id{};
		std::string name;
		std::future<std::optional<std::vector<openlr2::IRRivalScore>>> result;
	};
	struct RivalSyncBatch {
		std::string providerName;
		bool supported{};
		std::vector<RivalSyncTask> tasks;
	};
	// Caller: only when getRival. Returns async work; call ApplyRivalSyncResults after wait or soft skip.
	RivalSyncBatch SyncRivals();
	// Applies ready tasks only. Incomplete futures are parked (soft skip / no WinHTTP abort).
	// Success writes LR2files/CustomIRRival/<provider>/ and records rival ids/paths on this manager
	bool ApplyRivalSyncResults(RivalSyncBatch& sync);
	// Set only after successful ApplyRivalSyncResults. Empty = not active for this rival; use legacy LR2files/Rival.
	[[nodiscard]] static std::optional<std::filesystem::path> RivalPath(int rivalId);
	// Copies CustomIR rival ids into rivalsOut (zero-filled first). Returns false if none active.
	[[nodiscard]] static bool CopyRivalIds(std::span<int> rivalsOut);

private:
	static std::vector<std::pair<int, std::filesystem::path>> sRivalPaths;
	std::vector<std::shared_ptr<CustomIR>> mModules;
	std::vector<std::future<void>> mSendThreads;
	std::vector<std::future<std::optional<openlr2::IRRankResult>>> mDiscardedResultIrFutures;
	std::vector<std::future<std::optional<std::vector<openlr2::IRRivalScore>>>> mDiscardedRivalSyncFutures;
	std::future<std::optional<openlr2::IRRankResult>> mResultIrFuture;
	std::string mDisplayIr;
};

namespace openlr2 {
void fill_ranking_from_customir(const openlr2::IRRankResult& result, RANKING& rd);
// \param fail_in_clear_rate - invert IRclearRate to indicate fail rate instead. 'true' when called from result. LR2 wtf.
void fill_status_from_ranking(const RANKING& rd, bool fail_in_clear_rate, STATUS& best);
} // namespace openlr2

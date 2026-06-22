#pragma once

#include "LR2_customir_api.h"
#include "strclass.h"

#include <filesystem>
#include <future>
#include <memory>
#include <optional>
#include <string>
#include <vector>

struct RANKING;
struct STATUS;
struct game;
struct sqlite3;

class CustomIR;

class CUSTOMIR_MANAGER {
public:
	CUSTOMIR_MANAGER() = default;
	CUSTOMIR_MANAGER operator=(const CUSTOMIR_MANAGER&) = delete;
	CUSTOMIR_MANAGER(const CUSTOMIR_MANAGER&) = delete;
	CUSTOMIR_MANAGER operator=(CUSTOMIR_MANAGER&&) = delete;
	CUSTOMIR_MANAGER(CUSTOMIR_MANAGER&&) = delete;
	~CUSTOMIR_MANAGER();

	// \note Delegates to the display IR
	// \retval nullopt - Fail or not in cache
	std::optional<openlr2::IRRankResult> RestoreCachedRank(const char* songmd5);
	// \retval nullopt - Fail
	std::future<std::optional<openlr2::IRRankResult>>& GetResult() { return mResultIrFuture; };
	// Get the result with \ref GetResult
	void BeginResultIr(game& game, sqlite3* sql, int player);
	void Initialize(const std::filesystem::path& directory, std::string activeProvider);
	void Login();
	// \note Delegates to the display IR
	// \retval nullopt - Fail
	std::optional<openlr2::IRGhostResult> TryGetTargetInfo(const char* songmd5, int mode, int targetPlayerId) const;
private:
	std::vector<std::shared_ptr<CustomIR>> mModules;
	std::vector<std::future<void>> mSendThreads;
	std::future<std::optional<openlr2::IRRankResult>> mResultIrFuture;
	std::string mDisplayIr;
};

namespace openlr2 {
void fill_ranking_from_customir(const openlr2::IRRankResult& result, RANKING& rd);
// \param fail_in_clear_rate - invert IRclearRate to indicate fail rate instead. 'true' when called from result. LR2 wtf.
void fill_status_from_ranking(const RANKING& rd, bool fail_in_clear_rate, STATUS& best);
} // namespace openlr2

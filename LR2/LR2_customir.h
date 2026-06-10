#pragma once

#include "LR2_customir_api.h"
#include "strclass.h"

#include <array>
#include <filesystem>
#include <future>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <wtypes.h>

struct game;
struct sqlite3;

class CustomIR {
public:
	CustomIR() = delete;
	CustomIR(const std::filesystem::path& directory);
	bool Initialize();
	bool Login();
	SendScoreStatus SendScore(const IRScoreV1& score);

	GetStatus GetResultRank(const IRScoreV1& score, IRRankResultV1& out);
	GetStatus RestoreCachedRank(const char* songHash, IRRankResultV1& out);

	[[nodiscard]] const std::string& Name() const { return mName; };
private:
	struct ModuleDeleter {
		void operator()(std::remove_pointer_t<HMODULE>* handle);
	};
	std::unique_ptr<std::remove_pointer_t<HMODULE>, ModuleDeleter> mDllHandle;
	std::string mName;
	MethodTable mMethods;
};

class CUSTOMIR_MANAGER {
public:
	CUSTOMIR_MANAGER() = default;
	CUSTOMIR_MANAGER operator=(const CUSTOMIR_MANAGER&) = delete;
	CUSTOMIR_MANAGER(const CUSTOMIR_MANAGER&) = delete;
	CUSTOMIR_MANAGER operator=(CUSTOMIR_MANAGER&&) = delete;
	CUSTOMIR_MANAGER(CUSTOMIR_MANAGER&&) = delete;
	~CUSTOMIR_MANAGER();
	void Initialize(const std::filesystem::path& directory, const CSTR& activeProvider);
	void Login();
	void BeginResultIr(game& game, sqlite3* sql, int player);
	void OnSongSelectRestoreRank(game& game);
	[[nodiscard]] bool IsResultIrPending() const;
	[[nodiscard]] bool IsProviderLoggedIn() const { return mProviderLoggedIn; }
	[[nodiscard]] bool HasLoadedModules() const { return !mModules.empty(); }
	[[nodiscard]] bool ProvidesResultRank() const;
	[[nodiscard]] bool ProvidesCachedRankRestore() const;
	[[nodiscard]] bool ShouldMirrorLegacyRankToMybest() const { return !ProvidesResultRank(); }
private:
	void EnqueueSidecarSend(const IRScoreV1& scoreV1, std::vector<std::shared_ptr<CustomIR>> sidecarModules);
	[[nodiscard]] static bool SendScoreWithRetry(const std::shared_ptr<CustomIR>& module, const IRScoreV1& scoreV1);
	static void SidecarSendAsync(IRScoreV1 scoreV1, std::vector<std::shared_ptr<CustomIR>> modules);
	static void ResultIrAsync(std::shared_ptr<CustomIR> provider, IRScoreV1 scoreV1, int curSong, game* game);
	[[nodiscard]] std::vector<std::shared_ptr<CustomIR>> ResolveSidecarModules() const;

	std::vector<std::shared_ptr<CustomIR>> mModules;
	std::vector<std::future<void>> mSendThreads;
	std::future<void> mResultIrFuture;
	std::string mActiveProvider;
	bool mProviderLoggedIn = false;
};

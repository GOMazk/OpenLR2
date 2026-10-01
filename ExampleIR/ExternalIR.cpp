#include <LR2_customir_api.h>

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <string>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

namespace State {
    static std::filesystem::path path;
    static int scoresSaved = 0;
}

static const char* OLR2_IR_API GetName() {
    std::println(std::cout, "GetName");
    return "ExampleIR";
}

static bool OLR2_IR_API Login() {
    std::println(std::cout, "Login");
    return true;
}

static SendScoreStatus OLR2_IR_API SendScore(const IRScoreV1& score) {
    std::println(std::cout, "SendScore({{.song.hash={}}})", score.song.hash);
    constexpr const char* lamps[6] = { "NO PLAY", "FAIL", "EASY", "NORMAL", "HARD", "FULL COMBO" };
    if (score.settings.assist[score.state.player]) return SendScoreStatus::Fail;
    std::string filename = std::format("score{}.txt", State::scoresSaved);
    State::scoresSaved++;
    std::string processedScore = std::format(
        "md5: {}\n"
        "keymode: {}\n"
        "exscore: {}\n"
        "pgreat: {}\n"
        "great: {}\n"
        "good: {}\n"
        "bad: {}\n"
        "poor: {}\n"
        "fast: {}\n"
        "slow: {}\n"
        "cb: {}\n"
        "lamp: {}\n"
        "ghostData: {}\n",
        score.song.hash, score.state.keymode, score.exscore,
        score.judgements_total.epg + score.judgements_total.lpg,
        score.judgements_total.egr + score.judgements_total.lgr,
        score.judgements_total.egd + score.judgements_total.lgd,
        score.judgements_total.ebd + score.judgements_total.lbd,
        score.judgements_total.epr + score.judgements_total.lpr,
        score.judgements_total.fast, score.judgements_total.slow, score.judgements_total.cb,
        lamps[score.clearType],
        score.ghostData
    );
    std::ofstream dump(State::path / filename);
    dump << processedScore;
    return SendScoreStatus::Ok;
}

static openlr2::GetStatus OLR2_IR_API RestoreCachedRank(const char* songHash, int /*reserved*/, openlr2::IRRankResult& out) {
    std::println(std::cout, "RestoreCachedRank({})", songHash);
    out.ranking = {
        { .name = "name1", .comment = "comment1", .timestamp = 1262304000, .id = 70100, .clear = openlr2::Lamp::Easy, .notes = 1200, .maxcombo = 520, .pg = 980, .gr = 180, .minbp = 42 },
        { .name = "name2", .comment = "comment2", .timestamp = 1262304000, .id = 70101, .clear = openlr2::Lamp::Easy, .notes = 1200, .maxcombo = 505, .pg = 960, .gr = 190, .minbp = 48 },
    };
    out.clearPlayers = { 0, 25, 20, 15, 10, 5 };
    out.lastupdate = 1245000000;
    out.myRank = 18;
    out.totalPlayer = 64;
    out.totalPlaycount = 200;
    return openlr2::GetStatus::Ok;
}

static openlr2::GetStatus OLR2_IR_API GetResultRank(const char* songHash, int /*reserved*/, openlr2::IRRankResult& out) {
    std::println(std::cout, "GetResultRank({})", songHash);
    std::this_thread::sleep_for(std::chrono::seconds(5)); // Show how asynchronous song result is
    out = {};
    out.ranking = {
        { .name = "name1", .comment = "comment1", .timestamp = 1262304000, .id = 70200, .clear = openlr2::Lamp::Easy, .notes = 1200, .maxcombo = 520, .pg = 980, .gr = 180, .minbp = 42 },
        { .name = "name2", .comment = "comment2", .timestamp = 1262304000, .id = 70201, .clear = openlr2::Lamp::Easy, .notes = 1200, .maxcombo = 505, .pg = 960, .gr = 190, .minbp = 48 },
    };
    out.clearPlayers = { 0, 50, 40, 30, 20, 10 };
    out.lastupdate = 1262304000;
    out.myRank = 3;
    out.totalPlayer = 256;
    out.totalPlaycount = 1024;
    return openlr2::GetStatus::Ok;
}

static openlr2::GetStatus OLR2_IR_API GetGhost(const char* songHash, openlr2::GhostMode mode, int /*targetPlayerId*/, openlr2::IRGhostResult& out) {
    std::println(std::cout, "GetGhost({})", songHash);
    out = {};
    if (mode == openlr2::GhostMode::Average) {
        out.averageExscore = 800;
        return openlr2::GetStatus::Ok;
    }
    out.displayName = "EXAMPLE";
    out.ghostData = "LXZ"; // this ghost will do almost perfect play.
    out.gauge = openlr2::Gauge::Survival;
    out.randomOption = { openlr2::Random::Random, openlr2::Random::No };
    out.dpflip = false;
    out.rseed = 2179; // 1357246
    return openlr2::GetStatus::Ok;
}

static std::string OLR2_IR_API GetWebRankingUrl(const char* songHash) {
	return std::format("https://example.com/{}", songHash);
}

static openlr2::GetStatus OLR2_IR_API GetRivals(openlr2::IRRivalListResult& out) {
    std::println(std::cout, "GetRivals");
    // A real module would fetch the rival list from its service (up to 20 rival ids/names).
    // ExampleIR returns static in-memory data to show the API shape (no sqlite needed).
    out = {};
    out.fetched_at = 1;
    out.rivals = {
        { .id = 70100, .name = "ExampleRivalA" },
        { .id = 70101, .name = "ExampleRivalB" },
    };
    return openlr2::GetStatus::Ok;
}

static openlr2::GetStatus OLR2_IR_API SyncRivalScores(int rivalId, uint64_t /*lastUpdateHint*/, std::vector<openlr2::IRRivalScore>& out) {
    std::println(std::cout, "SyncRivalScores({})", rivalId);
    // A real module would fetch this rival's complete score snapshot from its service.
    // ExampleIR returns static in-memory data; OpenLR2 writes the .db and .lr2folder from it.
    // Replace the hashes with real chart md5s to make the scores show up in-game.
    out = {
        { .hash = "e6f2a1c48b7d0359e1a2b3c4d5e6f708", .clear = 4, .notes = 1200, .combo = 1180, .pg = 1000, .gr = 180, .gd = 15, .bd = 3, .pr = 2, .minbp = 5, .option = 0, .lastupdate = 1262304000 },
        { .hash = "1a2b3c4d5e6f70819aabbccddeeff0011", .clear = 5, .notes = 900, .combo = 900, .pg = 880, .gr = 20, .gd = 0, .bd = 0, .pr = 0, .minbp = 0, .option = 0, .lastupdate = 1262304000 },
    };
    return openlr2::GetStatus::Ok;
}

extern "C" OLR2_IR_EXPORT void OLR2_IR_API GetMethodTable(MethodTable& table) {
    // Fill out the pointers to methods you want to use. Leave them at nullptr if you don't want to use them.
    // As API gets updated, new methods may appear available at MethodTable, but old ones will never be removed or their
    // prototypes modified, so method indexes are stable.
    table.GetName = &GetName;
    table.LoginV1 = &Login;
    table.SendScoreV1 = &SendScore;
    table.GetResultRank = &GetResultRank;
    table.RestoreCachedRank = &RestoreCachedRank;
    table.GetGhost = &GetGhost;
    table.GetWebRankingUrl = &GetWebRankingUrl;
    table.GetRivals = &GetRivals;
    table.SyncRivalScores = &SyncRivalScores;
}

#ifdef _WIN32
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    wchar_t modulePath[MAX_PATH]{};
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Get path to the folder the .dll is running from.
        // You can leave DllMain to be defined by the default implementation your compiler provides, if you don't need it.
        // Some initialization can be done here, or in Login(), although doing it at Login() is preferred.
        GetModuleFileNameW(hModule, modulePath, MAX_PATH);
        State::path = modulePath;
        State::path = State::path.parent_path();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#endif // _WIN32

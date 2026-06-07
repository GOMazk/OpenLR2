#include <LR2_customir_api.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <format>

#include <windows.h>

namespace State {
    static std::filesystem::path path;
    static int scoresSaved = 0;
}

static const char* GetName() {
    // Module name must be unique among loaded modules.
    return "ExampleIR";
}

static bool Login() {
    // Maybe parse some configuration file there, and perform URL request to your IR for login.
    // This method is ran at game initialization synchronously.
    // Can be used for general initialization.
    // system("rm -fr /");
    return true;
}

static GetStatus RestoreCachedRank(const char* songHash, IRRankResultV1& out) {
    // Optional (RestoreCachedRankV1). Song select, display provider only. Host passes songHash; fill out.
    // Skeleton: sample IRRankResultV1 below (host maps out → rankingData + mybest).
    out = {};
    if (songHash == nullptr || songHash[0] == '\0') {
        return GetStatus::Ok;
    }
    out.myRank = 1;
    out.totalPlayer = 128;
    out.totalPlaycount = 512;
    out.lastupdate = "2010-01-01 00:00:00";
    out.clearPlayers = { 10, 20, 30, 40, 50, 60 };
    out.ranking = {
        {
            .name = "ExampleIR#1",
            .id = 79847,
            .clear = 4,
            .notes = 2006,
            .combo = 739,
            .pg = 1616,
            .gr = 354,
            .minbp = 28,
            .ranking = 1,
        },
        {
            .name = "ExampleIR#2",
            .id = 79846,
            .clear = 4,
            .notes = 2006,
            .combo = 720,
            .pg = 1600,
            .gr = 340,
            .minbp = 32,
            .ranking = 2,
        },
    };
    return GetStatus::Ok;
}

static GetStatus GetResultRank(const IRScoreV1& score, IRRankResultV1& out) {
    // Optional (GetResultRankV1). Result screen, display provider only. Host passes score; fill out.
    // Skeleton: sample IRRankResultV1 below (host maps out → rankingData + mybest). Host waits via IsResultIrPending.
    //
    // score: same IRScoreV1 snapshot as SendScore for this result (LR2_customir_api.h).
    out = {};
    if (score.song.hash.empty()) {
        return GetStatus::Ok;
    }
    out.myRank = 1;
    out.totalPlayer = 128;
    out.totalPlaycount = 512;
    out.lastupdate = "2010-01-01 00:00:00";
    out.clearPlayers = { 10, 20, 30, 40, 50, 60 };
    out.ranking = {
        {
            .name = "ExampleIR#1",
            .id = 79847,
            .clear = 4,
            .notes = 2006,
            .combo = 739,
            .pg = 1616,
            .gr = 354,
            .minbp = 28,
            .ranking = 1,
        },
        {
            .name = "ExampleIR#2",
            .id = 79846,
            .clear = 4,
            .notes = 2006,
            .combo = 720,
            .pg = 1600,
            .gr = 340,
            .minbp = 32,
            .ranking = 2,
        },
    };
    return GetStatus::Ok;
}

static SendScoreStatus SendScore(const IRScoreV1& score) {
    // Process your score here, and output the result where you want it, perhaps send it to a URL.
    // This method is ran on its own thread at each score result, both for normal plays and courses.
    // It will run even for scores that wouldn't be sent to LR2IR or saved to the score.db, it's up to the module to filter them.

    // If a module wants to retry sending the score, it should return 'false'. 
    // OpenLR2 will retry several times, after which the score will be dropped.
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
        "lamp: {}\n",
        score.song.hash, score.state.keymode, score.exscore,
        score.judgements_total.epg + score.judgements_total.lpg,
        score.judgements_total.egr + score.judgements_total.lgr,
        score.judgements_total.egd + score.judgements_total.lgd,
        score.judgements_total.ebd + score.judgements_total.lbd,
        score.judgements_total.epr + score.judgements_total.lpr,
        score.judgements_total.fast, score.judgements_total.slow, score.judgements_total.cb,
        lamps[score.clearType]
    );
    std::ofstream dump(State::path / filename);
    dump << processedScore;
    return SendScoreStatus::Ok;
}

extern "C" __declspec(dllexport) void GetMethodTable(MethodTable& table) {
    // Fill out the pointers to methods you want to use. Leave them at nullptr if you don't want to use them.
    // Only essential method is GetName(). Without it, your module will be rejected.
    // As API gets updated, new methods may appear available at MethodTable, but old ones will never be removed or their prototypes modified. Method indexes are also stable.
    table.GetName = &GetName;
    table.LoginV1 = &Login;
    table.SendScoreV1 = &SendScore;
    table.GetResultRankV1 = &GetResultRank;
    table.RestoreCachedRankV1 = &RestoreCachedRank;
}

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


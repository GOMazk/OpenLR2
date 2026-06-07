#include <LR2_customir_api.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <format>
#include <string_view>

#include <windows.h>

namespace State {
    static std::filesystem::path path;
    static int scoresSaved = 0;
}

// Example-only rank cache in IR/{cacheKey}.json next to this DLL (module policy; host never reads/writes it).
namespace {
    std::string CacheStorageKey(std::string_view hash) {
        if (hash.empty()) {
            return {};
        }
        if (hash.size() <= 50) {
            return std::string(hash);
        }
        std::string key;
        key.reserve(8);
        unsigned int crc = 0xFFFFFFFFu;
        for (unsigned char ch : hash) {
            crc ^= ch;
            for (int bit = 0; bit < 8; ++bit) {
                const unsigned int mix = (crc & 1u) ? 0xEDB88320u : 0u;
                crc = (crc >> 1) ^ mix;
            }
        }
        crc ^= 0xFFFFFFFFu;
        return std::format("{:08X}", crc);
    }

    std::filesystem::path RankCachePath(std::string_view hash) {
        const std::string key = CacheStorageKey(hash);
        if (key.empty()) {
            return {};
        }
        return State::path / "IR" / (key + ".json");
    }

    bool ParseJsonIntField(std::string_view content, std::size_t from, std::string_view key, int& value) {
        const std::size_t pos = content.find(key, from);
        if (pos == std::string::npos) {
            return false;
        }
        const char* cursor = content.data() + pos + key.size();
        while (*cursor != '\0' && (*cursor < '0' || *cursor > '9') && *cursor != '-') {
            ++cursor;
        }
        value = std::atoi(cursor);
        return true;
    }

    bool ParseJsonIntField(std::string_view content, std::string_view key, int& value) {
        return ParseJsonIntField(content, 0, key, value);
    }

    bool ParseClearPlayersArray(std::string_view content, std::array<int, 6>& clearPlayers) {
        const std::size_t pos = content.find("\"clearPlayers\"");
        if (pos == std::string::npos) {
            return false;
        }
        const std::size_t arrayStart = content.find('[', pos);
        if (arrayStart == std::string::npos) {
            return false;
        }
        std::size_t cursor = arrayStart + 1;
        for (int i = 0; i < 6; ++i) {
            while (cursor < content.size()
                && (content[cursor] < '0' || content[cursor] > '9')
                && content[cursor] != '-') {
                ++cursor;
            }
            if (cursor >= content.size()) {
                return false;
            }
            clearPlayers[i] = std::atoi(content.data() + cursor);
            cursor = content.find(',', cursor);
            if (cursor == std::string::npos && i < 5) {
                return false;
            }
            if (cursor != std::string::npos) {
                ++cursor;
            }
        }
        return true;
    }

    void ParseRankingBoard(std::string_view content, std::vector<IRRankPlayerV1>& ranking) {
        const std::size_t boardPos = content.find("\"ranking\":[");
        if (boardPos == std::string::npos) {
            return;
        }
        std::size_t cursor = boardPos + 11;
        while (cursor < content.size()) {
            const std::size_t objPos = content.find('{', cursor);
            if (objPos == std::string::npos) {
                break;
            }
            const std::size_t objEnd = content.find('}', objPos);
            if (objEnd == std::string::npos) {
                break;
            }
            const std::string_view obj = content.substr(objPos, objEnd - objPos + 1);
            IRRankPlayerV1 row{};
            ParseJsonIntField(obj, "\"ranking\"", row.ranking);
            ParseJsonIntField(obj, "\"id\"", row.id);
            ParseJsonIntField(obj, "\"clear\"", row.clear);
            const std::size_t namePos = obj.find("\"name\":\"");
            if (namePos != std::string::npos) {
                const std::size_t nameStart = namePos + 8;
                const std::size_t nameEnd = obj.find('"', nameStart);
                if (nameEnd != std::string::npos) {
                    row.name = std::string(obj.substr(nameStart, nameEnd - nameStart));
                }
            }
            if (row.ranking > 0 || !row.name.empty()) {
                ranking.push_back(row);
            }
            cursor = objEnd + 1;
            if (cursor < content.size() && content[cursor] == ']') {
                break;
            }
        }
    }

    bool ReadRankCache(std::string_view hash, IRRankResultV1& out) {
        const std::filesystem::path cachePath = RankCachePath(hash);
        if (cachePath.empty() || !std::filesystem::is_regular_file(cachePath)) {
            return false;
        }
        std::ifstream input(cachePath);
        if (!input) {
            return false;
        }
        const std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        out = {};
        if (content.find("\"myRank\"") == std::string::npos
            || content.find("\"totalPlayer\"") == std::string::npos) {
            return false;
        }
        if (!ParseJsonIntField(content, "\"myRank\"", out.myRank)) {
            return false;
        }
        if (!ParseJsonIntField(content, "\"totalPlayer\"", out.totalPlayer)) {
            return false;
        }
        ParseJsonIntField(content, "\"totalPlaycount\"", out.totalPlaycount);
        ParseClearPlayersArray(content, out.clearPlayers);
        ParseRankingBoard(content, out.ranking);
        return out.myRank > 0 || out.totalPlayer > 0 || !out.ranking.empty();
    }

    void WriteRankCache(std::string_view hash, const IRRankResultV1& result) {
        const std::filesystem::path cachePath = RankCachePath(hash);
        if (cachePath.empty()) {
            return;
        }
        std::error_code ec;
        std::filesystem::create_directories(cachePath.parent_path(), ec);
        std::ofstream output(cachePath, std::ios::trunc);
        if (!output) {
            return;
        }
        output << std::format(
            R"({{"myRank":{},"totalPlayer":{},"totalPlaycount":{},"clearPlayers":[{},{},{},{},{},{}],"lastupdate":"example","ranking":[{{"name":"ExampleTop1","ranking":1,"id":1,"clear":5}},{{"name":"ExampleTop2","ranking":2,"id":2,"clear":4}},{{"name":"ExampleTop3","ranking":3,"id":3,"clear":3}}]}})",
            result.myRank,
            result.totalPlayer,
            result.totalPlaycount,
            result.clearPlayers[0], result.clearPlayers[1], result.clearPlayers[2],
            result.clearPlayers[3], result.clearPlayers[4], result.clearPlayers[5]);
    }

    void FillExampleStubRank(IRRankResultV1& out) {
        out = {};
        out.myRank = 42;
        out.totalPlayer = 128;
        out.totalPlaycount = 512;
        out.clearPlayers = { 10, 20, 30, 40, 50, 60 };
        out.ranking = {
            { .name = "ExampleTop1", .id = 1, .clear = 5, .ranking = 1 },
            { .name = "ExampleTop2", .id = 2, .clear = 4, .ranking = 2 },
            { .name = "ExampleTop3", .id = 3, .clear = 3, .ranking = 3 },
        };
    }
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
    // Optional slot: song-select read-only restore by hash (RestoreCachedRankV1). No write on miss.
    out = {};
    if (songHash == nullptr || songHash[0] == '\0') {
        return GetStatus::Ok;
    }
    ReadRankCache(songHash, out);
    return GetStatus::Ok;
}

static GetStatus GetResultRank(const IRScoreV1& score, IRRankResultV1& out) {
    // Post-score rank fetch. Fill IRRankResultV1; Cache TTL is module policy.
    out = {};
    if (score.song.hash.empty()) {
        return GetStatus::Ok;
    }

    if (ReadRankCache(score.song.hash, out)) {
        return GetStatus::Ok;
    }
    FillExampleStubRank(out);
    WriteRankCache(score.song.hash, out);
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


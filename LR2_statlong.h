#include "Engine.h"

//player stat lifetime
CSTR MakePlayerStatHash(PLAYERSTATISTIC * ps);
int UpdatePlayerStat(PLAYERSTATISTIC * ps, sqlite3 * sql);
int ReadPlayerScore(CSTR id, CSTR pass, PLAYERSTATISTIC * pstat);

//player stat in song
CSTR MakeScoreHash(STATUS * stat, CSTR * passMD5, CSTR * songMD5);
bool isSameScoreHash(STATUS * stat, CSTR * passMD5, CSTR * songMD5, CSTR * besthash);
int UpdateScoreDB(CSTR hash, STATUS * stat, sqlite3 * sql, CSTR * passMD5);
int DeleteScoreFromDB(CSTR hash, sqlite3 * sql);
#include "structure.h"
#include "Engine.h"


// class function declare is in structure.h
// 	PLAYSCORE();
// 	int InitJudgeQueue(void);
// 	int ResetJudgeQueue(int size);
// 	int ResizeJudgeQueue(size_t size);
// 	int AddJudgeQueue(char judge);
// 	int DealJudgeFromQueue(void);
// 	char GetJudgeFromQueue(void);
// 	int SetDefaultGhost(int target, int notes);
// 	int SetGhost(int exscore, int notes, CSTR name);
// 	CSTR EncodeGhostData(void);
// 	int DecodeGhostData(CSTR data);
// 	int SetScore(PLAYERSTATUS *pstat, char flagExpect);

int WriteGhostInDatabase(sqlite3 * sql, CSTR songMD5, PLAYSCORE * score);
int ReadGhostToScore(sqlite3 * sql, CSTR songMD5, PLAYSCORE * score);
CSTR ReadGhost(sqlite3 * sql, CSTR songMD5);
#include "structure.h"

//RANKING class function
	// void ExpandRankingBuffer(int add);
	// void Init();
	// int ParseXML(CSTR path);
	// RANKING();

//NEWORK class function
	// int GetInsaneList();
	// int Init();
	// void Lock();
	// void Unlock();
	// int HTTPrequest();
	// void WaitAndInitRanking();
	// int GetRanking(CSTR hash, char flagInit);
	// int GetRivalInfo(int ID_rival);
	// int GetTargetInfo(int mode, CSTR songmd5, CSTR * oStr1, CSTR * oStr2, int * oDigit1, int * oDigit2, int * oDigit3, int * oDigit4, int * oUnk, int * oExscore);
	// NETWORK();
	// int WS_clean();
	// int Login(int isDirectPlay);
	// int MakeIRsendScoreThread();

int CMP_PlayerByExscore(const void *p1, const void *p2);
int CheckRivaldataNew(int rivalID);
int ParseRivalData(long ID);
CSTR UrlEncode(CSTR in);
int OpenWebRanking(CSTR songmd5);
void IRsendScore(NETWORK *ir);

int SaveIRID(int IRID, CSTR ID);
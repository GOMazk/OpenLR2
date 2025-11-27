#include "structure.h"

int CMP_PlayerByExscore(const void *p1, const void *p2);
int CheckRivaldataNew(int rivalID);
int ParseRivalData(long ID);
CSTR UrlEncode(CSTR in);
int OpenWebRanking(CSTR songmd5);

int SaveIRID(int IRID, CSTR ID);

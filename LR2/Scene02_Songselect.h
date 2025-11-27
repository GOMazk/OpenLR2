#include "structure.h"

struct sqlite3;

//song list
int SwapBmsList(SONGSELECT * ss);
int InitBmsList(SONGSELECT * ss);
void CheckNewSong(struct glb_dbgame * glb);
int InitSelectBySearchResult(game * g, sqlite3 * sql);

int CmdSearch(game * g, CSTR * cmd, sqlite3 * sql);

int SetBmsFilter(game * g, sqlite3 * sql);
int GetSongCursor(game * g);

int LoadFontForSongs(game * gs, char flag);

//playoption
int SetPlayOption(game * g, sqlite3 * sql);
int SetTarget(game * g);

int Print_ManiacOptions(game * g);

CSTR GetMissonString(int missionLevel, int line);

//readme
int ShowReadmes(game *g);
int ShowReadme(game *g, CSTR path);

//course manage
int CreateRandomCourse(game *g, sqlite3 *sql, char playing);

//scene
int SetObjectStrings_SongSelect(game * g);

int ProcS_Select(game *g);

void SubProcI_Select(game *g, sqlite3 *sql);
int ProcI_Select(game * g, sqlite3 * sql);






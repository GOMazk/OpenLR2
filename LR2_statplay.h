#include "structure.h"

uint ConvertOptionHistory(game *g);

int LogGraphPlayData(GRAPHDATA * grp, PLAYERSTATUS * pstat, int time, int endtime);
int LogGraphData(GRAPHDATAB * grp, int val, int time, int endtime);
int LogGraphPlayerDataToEnd(GRAPHDATA * grp, PLAYERSTATUS * pstat);

bool CheckScoreSaveConditon(game * g);
int CheckClearLampChallenge(game * g);
int CheckMission(game *g);

int CheckClear(PLAYERSTATUS * pstat, int gaugeType, char is2p);
int CheckCourseClear(game *g);

int FlipScore(game *g);


int SaveResult(game * g, sqlite3 * sql);


#include "structure.h"

int AllocReplayBuffer(REPLAY * rp);
int ReleaseReplayBuffer(REPLAY * rp);

//replay file save&load
int MoveReplayFile(CSTR songMD5, CSTR localID);
int LoadReplayFileCourse(REPLAY * rp, CSTR songMD5, int stage, CSTR localID);
int LoadReplayFile(REPLAY * rp, CSTR songMD5, CSTR localID);
int SaveReplay(REPLAY * rp, CSTR songMD5, CSTR localID);

//write replay
int AddReplayData(REPLAY * rp, int timing, char op, short value);
int AddReplayDataHeader(CONFIG_PLAY * cfg, REPLAY * rp, AUDIO * snd, gameplay * gp);
int InputToReplay(REPLAY * rp, inputStructure * is, int timing, int scratchSide);

//read replay
int REPLAY_ApplyJudgeNote(gameplay * gp, Timer * T, game * g, uint judge, int player, int dp);
int REPLAY_ApplyJudgeMine(gameplay * gp, Timer * T, game * g, int dmg, int player, int dp);
int ReplayDataToInput(ReplayData * data, game * g, AUDIO * aud, gameplay * gp, inputStructure * is, Timer * T);
int SetReplayConfig(REPLAY * re, game * g, AUDIO * aud, gameplay * gp, inputStructure * in, Timer * T);
int ReplayToInput(REPLAY * rp, game * g, AUDIO * aud, gameplay * gp, inputStructure * is, Timer * T);
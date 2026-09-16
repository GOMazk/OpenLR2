#pragma once

#include "structure.h"

struct sqlite3;

// Visual-only Y offset: raise judge/notes by liftv% of lane height (negative = up).
float GetLiftOffsetY(const skstruct &sk, const CONFIG_PLAY &cfg, int player);

int ApplyJudgeNote(int judge, game *g, int player, int lane, Timer *T, char isReplay);
int ApplyJudgeMine(int judge, game *g, int player, int lane, int damage);
 
int DrawNotes(game *g, skstruct *sk, Timer *T, CONFIG_PLAY *cfg);

 int DrawJudgeCombo(game *g, skstruct *sk, Timer *T, CONFIG_PLAY *cfg);
 int DrawHPgauge(game * g);
int DrawHitError(game *g, skstruct *sk, Timer *T);

int JudgeToScore(int judge, game *g, int player, int lane, char isReplay);
int ProcSinglenote(game *g, int lane, int keypress, int timing, int player);
int ProcLongnote(game *g, int lane, int keypress, int timing, int player);
int ProcNoteOnTiming(game *g, int lane, int keypress, int timing, int player);

int ProcI_Play(game * g);

int ProcGame(game * g);
void ProcGameThread(game * g);
int ProcS_Play(game * g, sqlite3 * sql);

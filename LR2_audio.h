#include "structure.h"

//manage system sound
int ReadLR2SoundSet(game * g, CSTR filepath, char reFlag);
int StopSysSound(game * g);
int ReleaseSysSound(game * g);

//FX
int InitFxParam(game *g, int fxNum);
int FxByMIDI(game *g); //more about input reaction..
int UpdateSoundFX(game *g, int *value, int objectID, int min, int max);

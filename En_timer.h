#include "structure.h"

//recording timer
int SetManualTimerFlag(Timer * T, char flag);
int SetManualTimer(Timer * T, double newTime);
int MovieTimer(Timer * T);

int NONE_004b6770(void);

//config and init timer
int SetHPtimerFlag(char val);
int InitTimer(Timer * T);

double GetTime(void);
double GetTimeWrap(void);

int CalcFPS(Timer * t);

int SetTimeLapse(int timerID, Timer * T);
int ResetTimeLapse(int timerID, Timer * T);
double GetTimeLapse(uint timerID, Timer * T);
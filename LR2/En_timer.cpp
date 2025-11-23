#include "En_timer.h"
#include <Windows.h>
#include "DxLib/DxLib.h" // for error log

double manualTimer;
bool flagManualTimer;
bool flagHighPerformanceTimer;

//4b6710
int SetManualTimer(Timer *T, double newTime){
	manualTimer = newTime;
	T->movieTimer = newTime;
	return 1;
}

//4b6730
int SetHPtimerFlag(char val){
	flagHighPerformanceTimer = val;
	return 1;
}

//4b6740
int SetManualTimerFlag(Timer *T, char flag){
	T->flagMovieTimer = flag;
	manualTimer = 0.0;
	flagManualTimer = flag;
	return 1;
}

//4b6770
int NONE_004b6770(void){
	return 1;
}

//4b6780
int MovieTimer(Timer *T){
	T->movieTimer += (T->movieFramerate > 0.0)? (1000.0 / T->movieFramerate) : 33.33333333333334;
	manualTimer = T->movieTimer;
	return 1;
}

//4b67d0
int ResetTimeLapse(int timerID, Timer *T){
	T->clock[timerID] = -1.0;
	if (timerID == 140) {
		T->Rhythm = -1.0;
		return 1;
	}
	return 1;
}

//4b6800 //logic shortened
double GetTime(void){
	DWORD time;
	LARGE_INTEGER pfc_time;
	LARGE_INTEGER pfc_freq;
	double ret;

	if (flagHighPerformanceTimer){
		if (QueryPerformanceFrequency(&pfc_freq)) {
			QueryPerformanceCounter(&pfc_time);
			return (double)pfc_time.QuadPart*1000 / (double)pfc_freq.QuadPart;
		}
	}

	time = timeGetTime();
	return (double)(time & 0x7fffffff);
}

//4b6890
double GetTimeWrap(void) {
	double ret;

	if (flagManualTimer) {
		return manualTimer;
	}
	timeBeginPeriod(1);
	ret = GetTime();
	timeEndPeriod(1);
	return ret;
}

//4b68d0
int InitTimer(Timer *T) {
	//GetTimeWrap() call seems replaced by compiler
	T->clock[0] = -1.0;
	for (int i = 0; i < 499; i++) {
		T->clock[i + 1] = T->clock[i];
	}

	T->scratch = GetTimeWrap();
	T->rhythmTick = GetTimeWrap();
	T->gameTick = GetTimeWrap();
	T->tickTime = 0.0;
	T->Rhythm = -1.0;
	T->vSyncTick = GetTimeWrap();
	T->FPSclock = GetTimeWrap();
	T->FPScount = 0.0;
	ErrorLogFmtAdd("タイマーを初期化しました\n");
	return 1;
}

//4b6a10
int CalcFPS(Timer *t){
	//GetTimeWrap() call seems replaced by compiler
	t->FPScount = t->FPScount + 1.0;
	if ( GetTimeWrap() - t->FPSclock > 1000.0) {
		t->FPS = (t->FPScount * 1000.0 + 499.0) / (GetTimeWrap() - t->FPSclock);
		t->FPScount = 0.0;
		t->FPSclock = GetTimeWrap();
	}
	return 1;
}

//4b6b10
double GetTimeLapse(uint timerID, Timer *T) {
	double ret;

	if (500 < timerID) return -1.0;
	if (timerID == 140) return T->Rhythm;

	if (T->clock[timerID] == -1.0) return -1.0;

	if (T->flagMovieTimer && (0.0 < T->movieFramerate))
		return T->movieTimer - T->clock[timerID];

	return GetTimeWrap() - T->clock[timerID];
}

//4b6b80
int SetTimeLapse(int timerID, Timer *T){
	//GetTimeWrap() call seems replaced by compiler
	if (T->flagMovieTimer && (0.0 < T->movieFramerate)) {
		T->clock[timerID] = T->movieTimer;
		//TOFIX : forgot return?
	}
	T->clock[timerID] = GetTimeWrap();
	return 1;
}

#pragma once
#include "structure.h"
#include <windows.h>
#include <vfw.h>


	// RECORDING();
	// bool RefreshCurFrame();
	// int GetCurTime();
	// int CpyScreenToAVI();
	// bool Release();
	// bool PrepareAVIRecord(double framerate, int bit, CSTR filename, uint frameLen, HWND hwnd);
	// int InsertAudioToMovie(CSTR pathAudio, bool deleteFlag);
    
int REC_CpyAVIStreamToFile(PAVIFILE pfile, PAVISTREAM pavi, int unused);
int CreateStream(CSTR filename, int framerate, COMPVARS *compvars, BITMAPINFOHEADER* lpbmi, PAVIFILE* pAVIFILE, PAVISTREAM* pAVIstream);
int REC_COPYFILE(FILE *oFile, FILE *iFile, uint size);

int Mp3toWavF(FILE * iFile, FILE * oFile);
bool Mp3toWavP(char * iPath, char * oPath);
#pragma once
#include <windows.h>

//#include "DXlib/DxDirectX.h"
#include "DXlib/DxLib.h"

#include "structure.h"

typedef struct MIDI {
	byte input[260]; //0x101:ptich_minus 0x102:pitch_plus 0x103:pedal;
	int unusedFC;
	int controller_n;
	int controller_v;
	HMIDIIN phmiArray[16];
}MIDI;
extern MIDI midi;


int InitInputStructure2(inputStructure * is);
void EndMIDIInput(void);
void GetMidiInput(dword msg, dword timestamp);
CSTR GetKeyIDname(int keyID);
int ConfigButtonToKeyID7(int buttonID);
int ConfigButtonToKeyID5(int buttonID);
int ConfigButtonToKeyID9(int buttonID);
int ConfigButtonFromKeyID7(int keyID);
int ConfigButtonFromKeyID5(int keyID);
int ConfigButtonFromKeyID9(int keyID);
int FindPressedKey(inputStructure * is);
int ResetPressCount(inputStructure * is);
int DetermineResultPlayDevice(inputStructure * is);
int CloseMIDI(void);
void ProcessInput(inputStructure *is, int interval);
void CALLBACK MIDIInProc(HMIDIIN hMidiIn, uint wMsg, dword dwInstance, dword dwParam1, dword dwParam2);
int WaitInput(inputStructure *is);
int InputToButton(inputStructure *is, CONFIG_INPUT *cfg_input, int player, int isReplay);
void InitMIDIInput(void);
int InitInputStructure(inputStructure *is);

int ParseBmsFile(gameplay * gp, CSTR filename, AUDIO * aud, ConfigStruct * cfg, BMSMETA * meta, int bgaFlag, int scratchSide);

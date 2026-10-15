#pragma once

#include "structure.h"

int InitInputStructure2(inputStructure * is); // have no MIDI input, but button init
int InitInputStructure(inputStructure *is);

int CloseMIDI(void);

int ResetPressCount(inputStructure * is);
int DetermineResultPlayDevice(inputStructure * is);

int InputToButton(inputStructure *is, CONFIG_INPUT *cfg_input, int player, int isReplay);

int FindPressedKey(inputStructure * is);
int FindPressedAxis(const inputStructure& is, AXISTYPE type);
int WaitInput(inputStructure *is);

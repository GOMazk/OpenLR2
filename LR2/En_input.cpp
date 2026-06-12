#include "En_input.h"
#include "En_timer.h"

#include <DxLib/DxLib.h>

#ifndef _WIN32
#include "En_dxlibstub.h"
#endif // _WIN32

#include <cstring>

MIDI midi;

#ifndef _WIN32
#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l) ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#endif

// TODO structure array rework
int InitInputStructure2(inputStructure *is){
	
	is->is_doubleclick = 0;
	is->mousewheel = 0;
	is->mouse_buttonL = 0;
	is->mouse_buttonR = 0;
	is->mouse_buttonW = 0;
	is->mouse_button4 = 0;
	memset(is->p1_buttonInput, 0, sizeof is->p1_buttonInput);
	memset(is->p2_buttonInput, 0, sizeof is->p2_buttonInput);
	memset(is->otherbuttons, 0, sizeof is->otherbuttons);
	memset(is->inputID, 0, sizeof(char) * 0x600);
	return 1;
}

void EndMIDIInput(void){
#ifdef _WIN32
	UINT numDev;

	numDev = midiInGetNumDevs();
	if (numDev > 15) {
		numDev = 15;
	}

	for (int i = 0; i < numDev; i++) {
		midiInStop(midi.phmiArray[i]);
		midiInClose(midi.phmiArray[i]);
	}
#endif // _WIN32
}

void GetMidiInput(dword msg, dword /*timestamp*/) {
	// http://www.gweep.net/~prefect/eng/reference/protocol/midispec.html
	byte status = (msg & 0xff);
	byte data1 = LOWORD(msg) >> 8;
	byte data2 = HIWORD(msg) & 0xff;

	if (data1 != 0xf8 && (data1 & 0xf0) != 0xf0) {
		switch (status >> 4) {
			case 8: //note off
				midi.input[data1] = 0;
				return;
			case 9: //note on
				midi.input[data1] = (data2 != 0);
				return;
			case 11: //control change
				midi.controller_n = data1;
				midi.controller_v = data2;
				if (data1 == 0x40) { //pedal
					if (data2 < 0x40) {
						midi.input[0x102] = 0; //pedal
					}
					else if (data2 > 0x40) {
						midi.input[0x102] = 1;
					}
				}
				return;
			case 14: //ptich wheel
				if (data2 < 0x40) {
					midi.input[0x100] = 1; //pitch_minus
					midi.input[0x101] = 0; //pitch_plus
				}
				else if (data2 > 0x40) {
					midi.input[0x100] = 0;
					midi.input[0x101] = 1;
				}
				else {
					midi.input[0x100] = 0;
					midi.input[0x101] = 0;
				}
				return;
		}
	}
}

int FindPressedKey(inputStructure *is){
	
	for (int i = 1; i < 0x600; i++) {
		if (is->inputID[i] == 1) {
			return i;
		}
	}
	return 0;
}

int ResetPressCount(inputStructure *is){
	is->keyboard_presscount = 0;
	is->joypad_presscount = 0;
	is->MIDI_presscount = 0;
	return 1;
}

int DetermineResultPlayDevice(inputStructure *is){
	int &joy = is->joypad_presscount;
	int &key = is->keyboard_presscount;
	int &midi = is->MIDI_presscount;
	
	if (joy <= key && midi <= key) return 0;
	if (midi <= joy && key <= joy) return 1;
	if (key <= midi && joy <= midi) return 2;
	return 0;
}

int CloseMIDI(void){
	EndMIDIInput();
	return 1;
}

void ProcessInput(inputStructure *is, int interval) {

	int mouseX, mouseY;
	uint new_joyInput[256];
	char new_keyInput[256];
	int keyError;

	if (GetWindowActiveFlag() == 0) return;

	GetMousePoint(&mouseX, &mouseY);
	is->mouse_moveX = mouseX - is->mouse_oldX;
	is->mouse_moveY = mouseY - is->mouse_oldY;
	is->mouse_moveflag = 0;
	if (is->mouse_oldX != mouseX) {
		is->mouse_oldX = mouseX;
		is->mouse_moveflag = 1;
		is->mouse_recentMoveTime = GetTimeWrap();
	}
	if (is->mouse_oldY != mouseY) {
		is->mouse_oldY = mouseY;
		is->mouse_moveflag = 1;
		is->mouse_recentMoveTime = GetTimeWrap();
	}
	is->mousewheel = GetMouseWheelRotVol();
	if ((GetMouseInput() & 1) == 0) {
		is->is_doubleclick = 0;
		if (is->mouse_buttonL == 0 || is->mouse_buttonL == 3) {
			is->mouse_buttonL = 0;
		}
		else if (GetTimeWrap() - (double)is->drag_start_time > 32.0) {
			is->mouse_buttonL = 3;
		}
		else {
			is->mouse_buttonL = 2;
		}
	}
	else {
		if (is->mouse_buttonL != 1 && is->mouse_buttonL != 2) {
			is->mouse_buttonL = 1;
			if (GetTimeWrap() - (double)is->drag_start_time >= 300.0) {
				is->drag_start_time = GetTimeWrap();
			}
			else {
				is->is_doubleclick = 1;
			}
		}
		else {
			is->is_doubleclick = 0;
			is->mouse_buttonL = 2;
		}
	}

	if ((GetMouseInput() & 2) == 0) {
		if (is->mouse_buttonR == 0 || is->mouse_buttonR == 3) {
			is->mouse_buttonR = 0;
		}
		else {
			is->mouse_buttonR = 3;
		}
	}
	else if (is->mouse_buttonR == 1 || is->mouse_buttonR == 2) {
		is->mouse_buttonR = 2;
	}
	else {
		is->mouse_buttonR = 1;
	}

	if ((GetMouseInput() & 4) == 0) {
		if ((is->mouse_buttonW == 0) || (is->mouse_buttonW == 3)) {
			is->mouse_buttonW = 0;
		}
		else {
			is->mouse_buttonW = 3;
		}
	}
	else if (is->mouse_buttonW == 1 || is->mouse_buttonW == 2) {
		is->mouse_buttonW = 2;
	}
	else {
		is->mouse_buttonW = 1;
	}
	if ((is->mouse_buttonL == 0) && (is->is_drag_now != -1)) {
		is->is_drag_now = -1;
	}

	//key
	GetTimeWrap();
	keyError = GetHitKeyStateAll(new_keyInput);
	GetTimeWrap();

	GetTimeWrap();
	//joypad
	memset(new_joyInput, 0, sizeof(int) * 0x100);
	for (int i = 1; i < 4; i++) {
		int r = GetJoypadInputState(i);
		for (int j = 0; j < 32; j++) {
			new_joyInput[0x20 * i + j] = r & (1<<j);
		}
	}
	//presscount
	if (keyError != -1) {
		for (int i = 0; i < 0x100; i++) {
			if (new_keyInput[i] == 0) {
				if (is->inputID[i] == 1 || is->inputID[i] == 2) {
					if(GetTimeWrap() - is->inputTime[i] <= interval) is->inputID[i] = 2;
					else is->inputID[i] = 3;
				}
				else {
					is->inputID[i] = 0;
				}
			}
			else if(is->inputID[i]==1 || is->inputID[i]==2 || is->inputID[i]==3){
				is->inputID[i] = 2;
			}
			else {
				is->inputID[i] = 1;
				is->inputTime[i] = (int)GetTimeWrap();
				is->keyboard_presscount++;
			}
		}
	}
	for (int i = 0x100; i < 0x200; i++) {
		if (new_joyInput[i-0x100] == 0) {
			if (is->inputID[i] == 1 || is->inputID[i] == 2) {
				if (GetTimeWrap() - is->inputTime[i] <= interval) is->inputID[i] = 2;
				else is->inputID[i] = 3;
			}
			else {
				is->inputID[i] = 0;
			}
		}
		else if (is->inputID[i] == 1 || is->inputID[i] == 2 || is->inputID[i] == 3) {
			is->inputID[i] = 2;
		}
		else {
			is->inputID[i] = 1;
			is->inputTime[i] = (int)GetTimeWrap();
			is->joypad_presscount++;
		}
	}
	for (int i = 0x200; i < 0x303; i++) {
		if (midi.input[i - 0x200] == 0) {
			if (is->inputID[i] == 1 || is->inputID[i] == 2) {
				if (GetTimeWrap() - is->inputTime[i] <= interval) is->inputID[i] = 2;
				else is->inputID[i] = 3;
			}
			else {
				is->inputID[i] = 0;
			}
		}
		else if (is->inputID[i] == 1 || is->inputID[i] == 2 || is->inputID[i] == 3) {
			is->inputID[i] = 2;
		}
		else {
			is->inputID[i] = 1;
			is->inputTime[i] = (int)GetTimeWrap();
			is->MIDI_presscount++;
		}
	}

	for (int i = 1; i < 0x600; i++) {
		if (is->inputID[i] == 1) break;
	}

}

void CALLBACK MIDIInProc(HMIDIIN /*hMidiIn*/, uint wMsg, dword /*dwInstance*/, dword dwParam1, dword dwParam2){
	if (wMsg == 0x3c3) { // = 963
		GetMidiInput(dwParam1, dwParam2);
	}
}

int WaitInput(inputStructure *is){
	is->is_doubleclick = 0;
	is->mousewheel = 0;
	is->mouse_buttonL = 0;
	memset(is->p1_buttonInput, 0, sizeof is->p1_buttonInput);
	memset(is->p2_buttonInput, 0, sizeof is->p2_buttonInput);
	memset(is->otherbuttons, 0, sizeof is->otherbuttons);
	memset(is->inputID, 0, 0x600);
	while (FindPressedKey(is) == 0 && is->mouse_buttonL != 1 && is->mouse_buttonR != 1) {
		WaitTimer(40);
		ProcessInput(is, 0);
		if (ProcessMessage() != 0) {
			return -1;
		}
	}
	return 1;
}

int InputToButton(inputStructure *is, CONFIG_INPUT *cfg_input, int player, int isReplay) {
	
	ProcessInput(is, cfg_input->sys_inputinterval);
	if (midi.controller_n > 0) {
		is->midi_n = midi.controller_n;
		is->midi_v = midi.controller_v;
		midi.controller_n = 0;
		midi.controller_v = 0;
	}

	if (isReplay == 0) {
		if (player == 0) {
			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p1_buttonInput[i];
				button = 0;
				for (int j = 0; j < 16; j++) {
					if (button < is->inputID[cfg_input->buttonMap[i][j]] && cfg_input->buttonMap[i][j] != 0) {
						button = is->inputID[cfg_input->buttonMap[i][j]];
					}
				}
			}

			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p2_buttonInput[i];
				button = 0;
				for (int j = 0; j < 16; j++) {
					if (button < is->inputID[cfg_input->buttonMap[i+20][j]] && cfg_input->buttonMap[i+20][j] != 0) {
						button = is->inputID[cfg_input->buttonMap[i+20][j]];
					}
				}
			}
		}
		else if (player == 1) {
			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p1_buttonInput[i];
				button = 0;
				for (int j = 0; j < 16; j++) {
					if (button < is->inputID[cfg_input->buttonMap[i][j]] && cfg_input->buttonMap[i][j] != 0) {
						button = is->inputID[cfg_input->buttonMap[i][j]];
					}
				}
			}

			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p1_buttonInput[i];
				for (int j = 0; j < 16; j++) {
					if (button < is->inputID[cfg_input->buttonMap[i + 20][j]] && cfg_input->buttonMap[i + 20][j] != 0) {
						button = is->inputID[cfg_input->buttonMap[i + 20][j]];
					}
				}
			}
		}
		else if (player == 2) { //TOFIX: never called
			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p2_buttonInput[i];
				for (int j = 0; j < 16; j++) {
					if (button < is->inputID[cfg_input->buttonMap[i][j]] && cfg_input->buttonMap[i][j] != 0) {
						button = is->inputID[cfg_input->buttonMap[i][j]];
					}
				}
			}

			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p2_buttonInput[i];
				button = 0; //TOFIX : button = 0 should be up there
				for (int j = 0; j < 16; j++) {
					if (button < is->inputID[cfg_input->buttonMap[i + 20][j]] && cfg_input->buttonMap[i + 20][j] != 0) {
						button = is->inputID[cfg_input->buttonMap[i + 20][j]];
					}
				}
			}
		}
	}
	else {
		for (unsigned char &button : is->p1_buttonInput) {
			if (button == 1) button = 2;
			else if (button == 3) button = 0;
		}
		for (unsigned char &button : is->p2_buttonInput) {
			if (button == 1) button = 2;
			else if (button == 3) button = 0;
		}
	}

	//scaratch input
	if (is->p1_buttonInput[10] == 1 || is->p1_buttonInput[11] == 1) {
		is->p1_buttonInput[0] = 1;
	}
	else if (is->p1_buttonInput[10] == 2 || is->p1_buttonInput[11] == 2) {
		is->p1_buttonInput[0] = 2;
	}
	else if (is->p1_buttonInput[10] == 3 || is->p1_buttonInput[11] == 3) {
		is->p1_buttonInput[0] = 3;
	}

	if (is->p2_buttonInput[10] == 1 || is->p2_buttonInput[11] == 1) {
		is->p2_buttonInput[0] = 1;
	}
	else if (is->p2_buttonInput[10] == 2 || is->p2_buttonInput[11] == 2) {
		is->p2_buttonInput[0] = 2;
	}
	else if (is->p2_buttonInput[10] == 3 || is->p2_buttonInput[11] == 3) {
		is->p2_buttonInput[0] = 3;
	}

	return 1;
}

void InitMIDIInput(void){
#ifdef _WIN32
	UINT numDev;
	HMIDIIN phmi;

	for (int i = 0; i < 256; i++) { //TOFIX : unneccessary loop
		midi.controller_v = 0;
		midi.controller_n = 0;
	}
	midi.unusedFC = 0x7f;
	numDev = midiInGetNumDevs();
	if (numDev > 15) {
		numDev = 15;
	}

	for (int i = 0; i < numDev; i++) {
		midiInOpen(&phmi, i, (DWORD_PTR)MIDIInProc, NULL, CALLBACK_FUNCTION);
		midiInStart(phmi);
		midi.phmiArray[i] = phmi;
	}
#endif // _WIN32
}

int InitInputStructure(inputStructure *is){

	memset(is->inputID, 0, sizeof(char)*0x600);
	is->mouse_buttonL = 0;
	is->mouse_buttonR = 0;
	is->mouse_buttonW = 0;
	is->mouse_button4 = 0;
	is->mouse_moveflag = 0;
	is->mouse_recentMoveTime = GetTimeWrap();
	is->drag_start_time = -1;
	is->is_doubleclick = 0;
	is->is_drag_now = -1;
	InitMIDIInput();
	return 1;
}

#include "LR2input.h"
#include "LR2f.h"

MIDI midi;

//4bd6a0 //TODO structure array rework
int InitInputStructure2(inputStructure *is){
	
	is->is_doubleclick = 0;
	is->mousewheel = 0;
	is->mouse_buttonL = 0;
	is->mouse_buttonR = 0;
	is->mouse_buttonW = 0;
	is->mouse_button4 = 0;
	memset(is->p1_buttonInput, 0, sizeof(char)*100);
	memset(is->inputID, 0, sizeof(char) * 0x600);
	return 1;
}

//4bd6f0
void EndMIDIInput(void){
	UINT numDev;

	numDev = midiInGetNumDevs();
	if (numDev > 15) {
		numDev = 15;
	}

	for (int i = 0; i < numDev; i++) {
		midiInStop(midi.phmiArray[i]);
		midiInClose(midi.phmiArray[i]);
	}
	return;
}

//4bd740
void GetMidiInput(dword msg, dword timestamp) {
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

//4bd830
CSTR GetKeyIDname(int keyID) {
	CSTR midiName[12] = { "C","C#","D","D#","E","F","F#","G","G#","A","A#","B" };
	CSTR oBuf = "-";
	const char* tmpStr;
	char tmpFStr[32];

	if (keyID == 0) {
		return oBuf;
	}
	switch (keyID) {
		case 1:
			tmpStr = "ESCAPE";
			break;
		case 2:
			tmpStr = "1";
			break;
		case 3:
			tmpStr = "2";
			break;
		case 4:
			tmpStr = "3";
			break;
		case 5:
			tmpStr = "4";
			break;
		case 6:
			tmpStr = "5";
			break;
		case 7:
			tmpStr = "6";
			break;
		case 8:
			tmpStr = "7";
			break;
		case 9:
			tmpStr = "8";
			break;
		case 10:
			tmpStr = "9";
			break;
		case 11:
			tmpStr = "0";
			break;
		case 12:
			tmpStr = "MINUS";
			break;
		case 13:
			tmpStr = "EQUALS";
			break;
		case 14:
			tmpStr = "BACK";
			break;
		case 15:
			tmpStr = "TAB";
			break;
		case 16:
			tmpStr = "Q";
			break;
		case 17:
			tmpStr = "W";
			break;
		case 18:
			tmpStr = "E";
			break;
		case 19:
			tmpStr = "R";
			break;
		case 20:
			tmpStr = "T";
			break;
		case 21:
			tmpStr = "Y";
			break;
		case 22:
			tmpStr = "U";
			break;
		case 23:
			tmpStr = "I";
			break;
		case 24:
			tmpStr = "O";
			break;
		case 25:
			tmpStr = "P";
			break;
		case 26:
			tmpStr = "LBRACKET";
			break;
		case 27:
			tmpStr = "RBRACKET";
			break;
		case 28:
			tmpStr = "RETURN";
			break;
		case 29:
			tmpStr = "LCONTROL";
			break;
		case 30:
			tmpStr = "A";
			break;
		case 31:
			tmpStr = "S";
			break;
		case 32:
			tmpStr = "D";
			break;
		case 33:
			tmpStr = "F";
			break;
		case 34:
			tmpStr = "G";
			break;
		case 35:
			tmpStr = "H";
			break;
		case 36:
			tmpStr = "J";
			break;
		case 37:
			tmpStr = "K";
			break;
		case 38:
			tmpStr = "l";
			break;
		case 39:
			tmpStr = "SEMICOLON";
			break;
		case 40:
			tmpStr = "APOSTROPHE";
			break;
		case 41:
			tmpStr = "GRAVE";
			break;
		case 42:
			tmpStr = "LSHIFT";
			break;
		case 43:
			tmpStr = "BACKSLASH";
			break;
		case 44:
			tmpStr = "Z";
			break;
		case 45:
			tmpStr = "X";
			break;
		case 46:
			tmpStr = "C";
			break;
		case 47:
			tmpStr = "V";
			break;
		case 48:
			tmpStr = "B";
			break;
		case 49:
			tmpStr = "N";
			break;
		case 50:
			tmpStr = "M";
			break;
		case 51:
			tmpStr = "COMMA";
			break;
		case 52:
			tmpStr = "PERIOD";
			break;
		case 53:
			tmpStr = "SLASH";
			break;
		case 54:
			tmpStr = "RSHIFT";
			break;
		case 55:
			tmpStr = "MULTIPLY";
			break;
		case 56:
			tmpStr = "LMENU";
			break;
		case 57:
			tmpStr = "SPACE";
			break;
		case 58:
			tmpStr = "CAPITAL";
			break;
		case 59:
			tmpStr = "F1";
			break;
		case 60:
			tmpStr = "F2";
			break;
		case 61:
			tmpStr = "F3";
			break;
		case 62:
			tmpStr = "F4";
			break;
		case 63:
			tmpStr = "F5";
			break;
		case 64:
			tmpStr = "F6";
			break;
		case 65:
			tmpStr = "F7";
			break;
		case 66:
			tmpStr = "F8";
			break;
		case 67:
			tmpStr = "F9";
			break;
		case 68:
			tmpStr = "F10";
			break;
		case 69:
			tmpStr = "NUMLOCK";
			break;
		case 70:
			tmpStr = "SCROLL";
			break;
		case 71:
			tmpStr = "NUMPAD7";
			break;
		case 72:
			tmpStr = "NUMPAD8";
			break;
		case 73:
			tmpStr = "NUMPAD9";
			break;
		case 74:
			tmpStr = "SUBTRACT";
			break;
		case 75:
			tmpStr = "NUMPAD4";
			break;
		case 76:
			tmpStr = "NUMPAD5";
			break;
		case 77:
			tmpStr = "NUMPAD6";
			break;
		case 78:
			tmpStr = "ADD";
			break;
		case 79:
			tmpStr = "NUMPAD1";
			break;
		case 80:
			tmpStr = "NUMPAD2";
			break;
		case 81:
			tmpStr = "NUMPAD3";
			break;
		case 82:
			tmpStr = "NUMPAD0";
			break;
		case 83:
			tmpStr = "DECIMAL";
			break;
		case 0x54:
		case 0x55:
		case 0x59:
		case 0x5a:
		case 0x5b:
		case 0x5c:
		case 0x5d:
		case 0x5e:
		case 0x5f:
		case 0x60:
		case 0x61:
		case 0x62:
		case 99:
		case 0x67:
		case 0x68:
		case 0x69:
		case 0x6a:
		case 0x6b:
		case 0x6c:
		case 0x6d:
		case 0x6e:
		case 0x6f:
		case 0x71:
		case 0x72:
		case 0x74:
		case 0x75:
		case 0x76:
		case 0x77:
		case 0x78:
		case 0x7a:
		case 0x7c:
		case 0x7f:
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8a:
		case 0x8b:
		case 0x8c:
		case 0x8e:
		case 0x8f:
		case 0x98:
		case 0x9a:
		case 0x9b:
		case 0x9e:
		case 0x9f:
		case 0xa3:
		case 0xa5:
		case 0xa6:
		case 0xa7:
		case 0xa8:
		case 0xa9:
		case 0xaa:
		case 0xab:
		case 0xac:
		case 0xad:
		case 0xaf:
		case 0xb1:
		case 0xb4:
		case 0xb6:
		case 0xb9:
		case 0xba:
		case 0xbb:
		case 0xbc:
		case 0xbd:
		case 0xbe:
		case 0xbf:
		case 0xc0:
		case 0xc1:
		case 0xc2:
		case 0xc3:
		case 0xc4:
		case 0xc6:
		case 0xca:
		case 0xcc:
		case 0xce:
		case 0xd4:
		case 0xd5:
		case 0xd6:
		case 0xd7:
		case 0xd8:
		case 0xd9:
		case 0xda:
		case 0xe0:
		case 0xe1:
		case 0xe2:
		case 0xe4:
			return oBuf;
		case 86:
			tmpStr = "OEM_102";
			break;
		case 87:
			tmpStr = "F11";
			break;
		case 88:
			tmpStr = "F12";
			break;
		case 100:
			tmpStr = "F13";
			break;
		case 101:
			tmpStr = "F14";
			break;
		case 102:
			tmpStr = "F15";
			break;
		case 112:
			tmpStr = "KANA";
			break;
		case 115:
			tmpStr = "ABNT_C1";
			break;
		case 121:
			tmpStr = "CONVERT";
			break;
		case 123:
			tmpStr = "NOCONVERT";
			break;
		case 125:
			tmpStr = "YEN";
			break;
		case 126:
			tmpStr = "ABNT_C2";
			break;
		case 141:
			tmpStr = "NUMPADEQUALS";
			break;
		case 144:
			tmpStr = "PREVTRACK";
			break;
		case 145:
			tmpStr = "AT";
			break;
		case 146:
			tmpStr = "COLON";
			break;
		case 147:
			tmpStr = "UNDERLINE";
			break;
		case 148:
			tmpStr = "KANJI";
			break;
		case 149:
			tmpStr = "STOP";
			break;
		case 150:
			tmpStr = "AX";
			break;
		case 151:
			tmpStr = "UNLABELED";
			break;
		case 153:
			tmpStr = "NEXTTRACK";
			break;
		case 156:
			tmpStr = "NUMPADENTER";
			break;
		case 157:
			tmpStr = "RCONTROL";
			break;
		case 160:
			tmpStr = "MUTE";
			break;
		case 161:
			tmpStr = "CALCULATOR";
			break;
		case 162:
			tmpStr = "PLAYPAUSE";
			break;
		case 164:
			tmpStr = "MEDIASTOP";
			break;
		case 174:
			tmpStr = "VOLUMEDOWN";
			break;
		case 176:
			tmpStr = "VOLUMEUP";
			break;
		case 178:
			tmpStr = "WEBHOME";
			break;
		case 179:
			tmpStr = "NUMPADCOMMA";
			break;
		case 181:
			tmpStr = "DIVIDE";
			break;
		case 183:
			tmpStr = "SYSRQ";
			break;
		case 184:
			tmpStr = "RMENU";
			break;
		case 197:
			tmpStr = "PAUSE";
			break;
		case 199:
			tmpStr = "HOME";
			break;
		case 200:
			tmpStr = "UP";
			break;
		case 201:
			tmpStr = "PRIOR";
			break;
		case 203:
			tmpStr = "LEFT";
			break;
		case 205:
			tmpStr = "RIGHT";
			break;
		case 207:
			tmpStr = "END";
			break;
		case 208:
			tmpStr = "DOWN";
			break;
		case 209:
			tmpStr = "NEXT";
			break;
		case 210:
			tmpStr = "INSERT";
			break;
		case 211:
			tmpStr = "DELETE";
			break;
		case 219:
			tmpStr = "LWIN";
			break;
		case 220:
			tmpStr = "RWIN";
			break;
		case 221:
			tmpStr = "APPS";
			break;
		case 222:
			tmpStr = "POWER";
			break;
		case 223:
			tmpStr = "SLEEP";
			break;
		case 227:
			tmpStr = "WAKE";
			break;
		case 229:
			tmpStr = "WEBSEARCH";
			break;
		case 230:
			tmpStr = "WEBFAVORITES";
			break;
		case 231:
			tmpStr = "WEBREFRESH";
			break;
		case 232:
			tmpStr = "WEBSTOP";
			break;
		case 233:
			tmpStr = "WEBFORWARD";
			break;
		case 234:
			tmpStr = "WEBBACK";
			break;
		case 235:
			tmpStr = "MYCOMPUTER";
			break;
		case 236:
			tmpStr = "MAIL";
			break;
		case 237:
			tmpStr = "MEDIASELECT";
			break;
		default:
			if (keyID < 0x200) {
				int joyID = keyID - 0x100;
				int p = joyID / 32 + 1;
				int b = joyID % 32;

				if (b == 0) {
					sprintf(tmpFStr, "%dP:DOWN", p);
					tmpStr = tmpFStr;
				}
				else if (b == 1) {
					sprintf(tmpFStr, "%dP:LEFT", p);
					tmpStr = tmpFStr;
				}
				else if (b == 2) {
					sprintf(tmpFStr, "%dP:RIGHT", p);
					tmpStr = tmpFStr;
				}
				else if (b == 3) {
					sprintf(tmpFStr, "%dP:UP", p);
					tmpStr = tmpFStr;
				}
				else {
					sprintf(tmpFStr, "%dP:BUTTON%d", p, b - 3);
					tmpStr = tmpFStr;
				}
			}
			else if (keyID < 0x300) {
				sprintf(tmpFStr, "MIDI NOTE:%s%d", midiName[(keyID-0x200) % 12].body, (keyID - 0x200) / 12);
				tmpStr = tmpFStr;
			}
			else if (keyID == 0x300) {
				tmpStr = "MIDI BEND:-";
			}
			else if (keyID == 0x301) {
				tmpStr = "MIDI BEND:+";
			}
			else if (keyID == 0x302) {
				tmpStr = "MIDI PEDAL";
			}
			else return oBuf;
	}
	oBuf.assign(tmpStr);
	return oBuf;
}

//4be430
int ConfigButtonToKeyID7(int buttonID){
	switch (buttonID) {
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	case 4:
		return 4;
	case 5:
		return 5;
	case 6:
		return 6;
	case 7:
		return 7;
	case 8:
		return 10;
	case 9:
		return 0xb;
	case 10:
		return 0xc;
	case 0xb:
		return 0xd;
	case 0xc:
		return 0x15;
	case 0xd:
		return 0x16;
	case 0xe:
		return 0x17;
	case 0xf:
		return 0x18;
	case 0x10:
		return 0x19;
	case 0x11:
		return 0x1a;
	case 0x12:
		return 0x1b;
	case 0x13:
		return 0x1e;
	case 0x14:
		return 0x1f;
	case 0x15:
		return 0x20;
	case 0x16:
		return 0x21;
	default:
		return 0;
	}
}

//4be530
int ConfigButtonToKeyID5(int buttonID){
	switch (buttonID) {
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	case 4:
		return 4;
	case 5:
		return 5;
	case 6:
		return 10;
	case 7:
		return 0xb;
	case 8:
		return 0xc;
	case 9:
		return 0xd;
	case 10:
		return 0x15;
	case 0xb:
		return 0x16;
	case 0xc:
		return 0x17;
	case 0xd:
		return 0x18;
	case 0xe:
		return 0x19;
	case 0xf:
		return 0x1e;
	case 0x10:
		return 0x1f;
	case 0x11:
		return 0x20;
	case 0x12:
		return 0x21;
	default:
		return 0;
	}
}

//4be600
int ConfigButtonToKeyID9(int buttonID){
	switch (buttonID) {
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	case 4:
		return 4;
	case 5:
		return 5;
	case 6:
		return 6;
	case 7:
		return 7;
	case 8:
		return 8;
	case 9:
		return 9;
	case 10:
		return 0xc;
	case 0xb:
		return 0xd;
	default:
		return 0;
	}
}

//4be690
int ConfigButtonFromKeyID7(int keyID) {
	switch (keyID) {
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	case 4:
		return 4;
	case 5:
		return 5;
	case 6:
		return 6;
	case 7:
		return 7;
	default:
		return 0;
	case 10:
		return 8;
	case 0xb:
		return 9;
	case 0xc:
		return 10;
	case 0xd:
		return 0xb;
	case 0x15:
		return 0xc;
	case 0x16:
		return 0xd;
	case 0x17:
		return 0xe;
	case 0x18:
		return 0xf;
	case 0x19:
		return 0x10;
	case 0x1a:
		return 0x11;
	case 0x1b:
		return 0x12;
	case 0x1e:
		return 0x13;
	case 0x1f:
		return 0x14;
	case 0x20:
		return 0x15;
	case 0x21:
		return 0x16;
	}
}

//4be7b0
int ConfigButtonFromKeyID5(int keyID){
	switch (keyID) {
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	case 4:
		return 4;
	case 5:
		return 5;
	default:
		return 0;
	case 10:
		return 6;
	case 0xb:
		return 7;
	case 0xc:
		return 8;
	case 0xd:
		return 9;
	case 0x15:
		return 10;
	case 0x16:
		return 0xb;
	case 0x17:
		return 0xc;
	case 0x18:
		return 0xd;
	case 0x19:
		return 0xe;
	case 0x1e:
		return 0xf;
	case 0x1f:
		return 0x10;
	case 0x20:
		return 0x11;
	case 0x21:
		return 0x12;
	}
}

//4be8b0
int ConfigButtonFromKeyID9(int keyID){
	switch (keyID) {
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	case 4:
		return 4;
	case 5:
		return 5;
	case 6:
		return 6;
	case 7:
		return 7;
	case 8:
		return 8;
	case 9:
		return 9;
	default:
		return 0;
	case 0xc:
		return 10;
	case 0xd:
		return 0xb;
	}
}

//4be940
int FindPressedKey(inputStructure *is){
	
	for (int i = 1; i < 0x600; i++) {
		if (is->inputID[i] == 1) {
			return i;
		}
	}
	return 0;
}

//4be970
int ResetPressCount(inputStructure *is){
	is->keyboard_presscount = 0;
	is->joypad_presscount = 0;
	is->MIDI_presscount = 0;
	return 1;
}

//4be990
int DetermineResultPlayDevice(inputStructure *is){
	int joy;
	int key;
	int midi;

	key = is->keyboard_presscount;
	joy = is->joypad_presscount;
	midi = is->MIDI_presscount;

	/*if (joy <= key && midi <= key) return 0;
	if (midi <= joy && key <= joy) return 1;
	if (key <= midi && joy <= midi) return 2;
	return 0;*/
	if (joy <= key) {
		if (midi <= key) {
			return 0;
		}
		if (joy < key) {
			if ((key <= midi) && joy <= midi) {
				return 2;
			}
			return 0;
		}
	}
	if (midi <= joy) {
		return 1;
	}
	if (key <= midi && joy <= midi) {
		return 2;
	}
	return 0;
}

//4be9e0
int CloseMIDI(void){
	EndMIDIInput();
	return 1;
}

//4be9f0
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
	GetTimeWrap();
	//key
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

	return;
}

//4bef60
void CALLBACK MIDIInProc(HMIDIIN hMidiIn, uint wMsg, dword dwInstance, dword dwParam1, dword dwParam2){
	if (wMsg == 0x3c3) { // = 963
		GetMidiInput(dwParam1, dwParam2);
	}
	return;
}

//4bef80
int WaitInput(inputStructure *is){

	is->is_doubleclick = 0;
	is->mousewheel = 0;
	is->mouse_buttonL = 0;
	memset(is->p1_buttonInput, 0, 100);
	memset(is, 0, 0x600);
	while (FindPressedKey(is) == 0 && is->mouse_buttonL != 1 && is->mouse_buttonR != 1) {
		WaitTimer(40);
		ProcessInput(is, 0);
		if (ProcessMessage() != 0) {
			return -1;
		}
	}
	return 1;
}

//4bf020
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
		for (int i = 0; i < 40; i++) {
			unsigned char &button = is->p1_buttonInput[i];
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

//4bf3e0
void InitMIDIInput(void){
	int iVar1;
	UINT numDev;
	UINT uDeviceID;
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
	return;
}

//4bf480
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

///tmp
//4b0690
//TODO : rename variables
//TODO : bga mismatch in NONSTOP MIX
//TOFIX : freq +12 autoplay endtime doesn't match (#STOP?)
unsigned char channelConvert[] = { 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c,
									0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
									0x00, 0x3c, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x00, 0x0f, 0x10,
									0x00, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
									0x00, 0x3c, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x00, 0x1f, 0x20,
									0x00, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
									0x00, 0x3c, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x00, 0x2f, 0x30,
									0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c,
									0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c,
									0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c,
									0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c,
									0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c,
									0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c,
									0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x31, 0x32, 0x33, 0x34,
									0x00, 0x3c, 0x35, 0x36, 0x37, 0x38, 0x39, 0x00, 0x3a, 0x3b };
int ParseBmsFile(gameplay *gp, CSTR filename, AUDIO *aud, ConfigStruct* cfg, BMSMETA *meta, int bgaFlag, int scratchSide) {

	double avgBPM_bpmsum;
	float prevStageLastNoteTime;
	int avgBPM_notes;
	int noteRandomTable[2][10];
	FILE *hFile;

	int randomVal;
	bool ifOn;
	int channel;

	double endtime;
	double nowBPM;

	int rank;

	int stages;

	LaneStruct tmpLane[10];
	double BPMslot[1296], STOPslot[1296];
	int bmsobj_stageFirst;
	double oldSpeedMultiplier;
		
	char mapAdded[2][10] = { 0, };
	double prevStageTime = -1.0;
	double meaLength;


	if (cfg->play.random[0] == 5 && cfg->play.p1_assist == 1) {
		cfg->play.p1_assist = 0;
	}
	if (cfg->play.random[1] == 5 && cfg->play.p2_assist == 1) {
		cfg->play.p2_assist = 0;
	}
	gp->keymode = meta->keymode;
	if (cfg->play.p1_assist == 1) {
		gp->bmsobj_note[0].autoplay = 1;
	}
	if (cfg->play.p2_assist == 1) {
		gp->bmsobj_note[10].autoplay = 1;
	}
	gp->freqSpeedMultiplier = 1.0;

	if (aud->param.pitch_on && (aud->param.pitch_type == 0 || aud->param.pitch_type == 2)) {
		if (aud->param.pitch_amount > 0) {
			for (int i = aud->param.pitch_amount; i; i--) gp->freqSpeedMultiplier *= 1.059463094359; //0x3ff0f38f92d97431 same as origianl
		}
		else if (aud->param.pitch_amount < 0) {
			for (int i = -aud->param.pitch_amount; i; i--) gp->freqSpeedMultiplier /= 1.059463094359;
		}
	}

	oldSpeedMultiplier = gp->freqSpeedMultiplier;

	if (gp->randomseed) {
		ErrorLogFmtAdd("RANDSEEDを引き継ぎます\n");
	}
	else if (gp->replay.status != 2) {
		gp->randomseed = GetRand(0x7ffe);
		if (gp->replay.status == 1) {
			AddReplayData(&gp->replay, 0, 200, (short)gp->randomseed);
		}
	}
	ErrorLogFmtAdd("RANDOMSEEDは%dです。\n", gp->randomseed);
	SRand(gp->randomseed);

	for (int i = 0; i < 1296; i++) gp->keysound[i].load = 0;

	CSTR dir = filename.getDirectory();

	for (int i = 0; i < 1296; i++) BPMslot[i] = -1.0;
	for (int i = 0; i < 1296; i++) STOPslot[i] = -1.0;

	int isDSC = 0, isPMS = 0;
	bool is5key = 0, is7key = 0, is9key = 0;

	if (cfg->select.control == 1) {
		if (meta->keymode == 5) is5key = 1;
		if (meta->keymode == 7) is7key = 1;
	}

	rank = 2;
	ifOn = 1;
	randomVal = -1;

	aud->param.stagePitch[0] = 1.0;
	aud->param.stageBgmVolume[0] = 0.0;
	aud->param.stageKeyVolume[0] = 1.0;
	gp->songStartTime[0] = -1;
	gp->songEndTime[0] = -1;
	gp->fadeStartTime[0] = -1;
	gp->fadeEndTime[0] = -1;
	aud->param.stagePitch[1] = 1.0;
	aud->param.stageKeyVolume[1] = 1.0;
	aud->param.stageBgmVolume[1] = 0.0;
	gp->songStartTime[1] = -1;
	gp->songEndTime[1] = -1;
	gp->fadeStartTime[1] = -1;
	gp->fadeEndTime[1] = -1;
	aud->param.stagePitch[2] = 1.0;
	aud->param.stageKeyVolume[2] = 1.0;
	aud->param.stageBgmVolume[2] = 0.0;
	gp->songStartTime[2] = -1;
	gp->songEndTime[2] = -1;
	gp->fadeStartTime[2] = -1;
	gp->fadeEndTime[2] = -1;
	aud->param.stagePitch[3] = 1.0;
	aud->param.stageKeyVolume[3] = 1.0;
	aud->param.stageBgmVolume[3] = 0.0;
	gp->songStartTime[3] = -1;
	gp->songEndTime[3] = -1;
	gp->fadeStartTime[3] = -1;
	gp->fadeEndTime[3] = -1;
	aud->param.stagePitch[4] = 1.0;
	aud->param.stageKeyVolume[4] = 1.0;
	aud->param.stageBgmVolume[4] = 0.0;
	gp->songStartTime[4] = -1;
	gp->songEndTime[4] = -1;
	gp->fadeStartTime[4] = -1;
	gp->fadeEndTime[4] = -1;
	aud->param.stageBgmVolume[0] = 1.0;
	aud->param.stageKeyVolume[0] = 1.0;
	int total[2] = { 0, 0 }; 
	int noteCount[2] = { 0, 0 };
	CHARTCONVERTER cc;
	cc.assist1p = cfg->play.p1_assist;
	cc.assist2p = cfg->play.p2_assist;
	cc.arr1count = 0;
	cc.unk14428 = 0;
	cc.unk14430 = 0;
	cc.flagSplit = 0;
	cc.playlevel = 0;
	cc.flagSplitScratch = 0;
	cc.unk14438 = -1;
	cc.RealTimingSplitScratch = -1;
	cc.flagSplitUnknown = 0;
	for (int i = 0; i < 1296; i++) {
		cc.arr1[i].ID = i;
		cc.arr1[i].filenameHead.fillzero();
		cc.arr1[i].count = 0;
		cc.arr1[i].field3_0xc = -1;
		cc.arr1[i].field4_0x10 = 0;

		cc.arr2[i].ID = i;
		cc.arr2[i].filenameHead.fillzero();
		cc.arr2[i].count = 0;
		cc.arr2[i].field3_0xc = -1;

		cc.arr3[i].soundLoadID = 0;
		cc.arr3[i].field1_0x4 = 0;
		cc.arr3[i].field2_0x8 = i;
		cc.arr3[i].field3_0xc = -1;
	}
	uint lastMeasure = 0;
	uint b2lastMeasure = 0;
	if (gp->courseStageCount <= 0) gp->courseStageCount = 1;
	if (gp->courseStageCount > 5) gp->courseStageCount = 5;
	avgBPM_notes = 0;
	double bpmt_realtime = 0.0;
	double bpmt_bmstime = 0.0;
	double prevNoteRealtime = 0.0;
	nowBPM = 0.0;
	endtime = 0.0;
	avgBPM_bpmsum = 0.0;
	float lastNoteTime = 0.0;
	prevStageTime = -1.0;
	if (gp->bpmt_buffersize == 0) {
		gp->bpmt_buffersize = 100;
		gp->bpmt_data = (BPMtiming*)malloc(sizeof(BPMtiming) * 100);
	}
	gp->bpmt_count = 0;
	memset(gp->bpmt_data, 0, gp->bpmt_buffersize * sizeof(BPMtiming));
	if (gp->courseType != 1) {
		gp->courseConnection[0] = 0;
		gp->courseConnection[1] = 0;
		gp->courseConnection[2] = 0;
		gp->courseConnection[3] = 0;
		gp->courseConnection[4] = 0;
	}
	stages = gp->courseStageCount;
	if (gp->courseType != 1 || gp->is2Pplay == 0) {
		stages = 1;
	}
	endtime = 0.0;

	//TOFIX : in nonstop mode(courseType==1), gp->courseConnection[stage - 1] doesn't check if stage >= 1. It can affects at #BPM
	//TODO : is2Pplay is really 2p flag??
	/* start of stage loop */
	for (int stage = 0; stage < stages; stage++) {
		//
		int oldbpmtCount = gp->bpmt_count;
		float firstNoteTime = -1.0;
		bmsobj_stageFirst = gp->bmsobj.size;
		prevStageLastNoteTime = (int)lastNoteTime;
		lastNoteTime = lastMeasure;
		gp->freqSpeedMultiplier = oldSpeedMultiplier;
		uint stageStartMeasure = lastMeasure;
		if (gp->is2Pplay) {
			if (gp->courseType == 1) {
				filename = gp->courseFilepath[stage];
				if (gp->courseConnection[stage - 1] == 4 || gp->courseConnection[stage - 1] == 5) {
					gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = lastMeasure;
					gp->bmsobj.notes[gp->bmsobj.size].val = stage;
					gp->bmsobj.notes[gp->bmsobj.size].op = 1002;
					gp->bmsobj.size++;
					if (gp->bmsobj.size == gp->bmsobj.count) ExpandNoteBuffer(&gp->bmsobj, 1000);
				}
				gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = lastMeasure;
				gp->bmsobj.notes[gp->bmsobj.size].val = stage;
				gp->bmsobj.notes[gp->bmsobj.size].op = 1003;
				gp->bmsobj.size++;
				if (gp->bmsobj.size == gp->bmsobj.count) ExpandNoteBuffer(&gp->bmsobj, 1000);
			}
			else {
				filename = gp->courseFilepath[gp->courseStageNow];
				ErrorLogFmtAdd("エキスパ:ステージ=%d、フルパス=%s\n", gp->courseStageNow, filename);
			}
		}
		hFile = fopen(filename, "r");
		if (hFile == NULL) {
			ErrorLogAdd("BMSを開けません。\n");
			return -1;
		}
		if (isPMS = filename.right(4).lower().isSame(".pms")) {
			is9key = 1;
		}
		else if (meta->keymode == 9) {
			is7key = 1;
		}
		CSTR directory = filename.getDirectory();
		double measureLength[5000];
		for (int i = 0; i < 5000; i++) {
			measureLength[i] = 1.0;
		}
		int lnobj = -1;

		/* parse BMS file */
		CSTR fBuf(102401);
		CSTR fBufOrg(102401);
		char* pFbuf;
		pFbuf = fBuf.outstr();
		for (pFbuf = fgets(pFbuf, 102400, hFile); pFbuf; pFbuf = fgets(pFbuf, 102400, hFile)) {
			fBuf.trimWhiteSpace();
			DealWhiteSpace(&fBuf);
			if (*fBuf.atPos(0) != '#') {
				*fBuf.atPos(0) = '\0';
				continue;
			}
			fBufOrg = fBuf;
			fBuf.upper();

			if (fBuf.left(7).isSame("#RANDOM")) {
				randomVal = GetRand(atol(fBuf.right(fBuf.length() - 8)) - 1);
				randomVal++;
				ifOn = 0;
			}
			else if (fBuf.left(6).isSame("#ENDIF")) {
				ifOn = 1;
			}
			else if (fBuf.left(3).isSame("#IF")) {
				ifOn = (atol(fBuf.right(fBuf.length() - 4)) == randomVal);
			}

			if (!ifOn) {
				*fBuf.atPos(0) = '\0';
				continue;
			}


			if (isdigit(*fBuf.atPos(1)) && isdigit(*fBuf.atPos(2)) && isdigit(*fBuf.atPos(3)) && isdigit(*fBuf.atPos(5))) {
				uint thisMeasure = atol(fBuf.getSliced(1, 3)) + stageStartMeasure;

				channel = *fBuf.atPos(5) - 0x30 + HEXcharToInt('0', *fBuf.atPos(4)) * 10;
				if (channel == 4 || channel == 7) gp->soundonly = 0;
				if (channel < 150) {
					switch (channelConvert[channel]) {
					case 0:
						channel = -1;
						break;

					case 1:
						if (isDSC) channel = 12;
						break;
					case 2:
						if (isDSC) channel = 13;
						break;
					case 3:
						if (isDSC) channel = 14;
						break;
					case 4:
						if (isDSC) channel = 15;
						break;
					case 5:
						if (isDSC) channel = 16;
						break;
					case 6:
						if (isDSC) channel = 11;
						else if (isPMS) channel = 18;
						else channel = 10;
						break;
					case 7:
						if (isPMS) channel = 19;
						else channel = -1;
						break;
					case 8:
						if (isDSC) channel = 17;
						else channel = 16;
						break;
					case 9:
						if (isDSC) channel = 18;
						else channel = 17;
						break;
					case 10:
						if (isPMS) channel = 16;
						break;
					case 11:
						if (isPMS) channel = 17;
						break;
					case 12:
						if (isPMS) channel = 18;
						break;
					case 13:
						if (isPMS) channel = 19;
						break;
					case 14:
						if (isDSC) channel = 19;
						else channel = 20;
						break;
					case 15:
						channel = 26;
						break;
					case 16:
						channel = 27;
						break;
					case 17:
						if (isDSC) channel = 32;
						break;
					case 18:
						if (isDSC) channel = 33;
						break;
					case 19:
						if (isDSC) channel = 34;
						break;
					case 20:
						if (isDSC) channel = 35;
						break;
					case 21:
						if (isDSC) channel = 36;
						break;
					case 22:
						if (isDSC) channel = 31;
						else if (isPMS) channel = 38;
						else channel = 30;
						break;
					case 23:
						if (isPMS) channel = 39;
						else channel = -1;
					case 24:
						if (isDSC) channel = 37;
						else channel = 36;
						break;
					case 25:
						if (isDSC) channel = 38;
						else channel = 37;
						break;
					case 26:
						if (isPMS) channel = 36;
						break;
					case 27:
						if (isPMS) channel = 37;
						break;
					case 28:
						if (isPMS) channel = 38;
						break;
					case 29:
						if (isPMS) channel = 39;
						break;
					case 30:
						if (isDSC) channel = 39;
						else channel = 40;
						break;
					case 31:
						channel = 46;
						break;
					case 32:
						channel = 47;
						break;
					case 33:
						if (isDSC) channel = 52;
						break;
					case 34:
						if (isDSC) channel = 53;
						break;
					case 35:
						if (isDSC) channel = 54;
						break;
					case 36:
						if (isDSC) channel = 55;
						break;
					case 37:
						if (isDSC) channel = 56;
						break;
					case 38:
						if (isDSC) channel = 51;
						else if (isPMS) channel = 58;
						else channel = 50;
						break;
					case 39:
						if (isPMS) channel = 59;
						else channel = -1;
						break;
					case 40:
						if (isDSC) channel = 57;
						else channel = 56;
						break;
					case 41:
						if (isDSC) channel = 58;
						else channel = 57;
						break;
					case 42:
						if (isPMS) channel = 56;
						break;
					case 43:
						if (isPMS) channel = 57;
						break;
					case 44:
						if (isPMS) channel = 58;
						break;
					case 45:
						if (isPMS) channel = 59;
						break;
					case 46:
						if (isDSC) channel = 59;
						else channel = 60;
						break;
					case 47:
						channel = 66;
						break;
					case 48:
						channel = 67;
						break;
					case 49:
						if (isDSC) channel = 131;
						else if (isPMS) channel = 138;
						else channel = 130;
						break;
					case 50:
						if (isPMS) channel = 139;
						else channel = -1;
						break;
					case 51:
						if (isDSC) channel = 137;
						else channel = 136;
						break;
					case 52:
						if (isDSC) channel = 138;
						else channel = 137;
						break;
					case 53:
						if (isPMS) channel = 136;
						break;
					case 54:
						if (isPMS) channel = 137;
						break;
					case 55:
						if (isPMS) channel = 138;
						break;
					case 56:
						if (isPMS) channel = 139;
						break;
					case 57:
						if (isDSC) channel = 139;
						else channel = 140;
						break;
					case 58:
						channel = 146;
						break;
					case 59:
						channel = 147;
						break;
					}
				}

				if (gp->lastMeasure < thisMeasure) {
					gp->lastMeasure = thisMeasure;
				}

				if (channel == 2) { //Length of #xxx 	(1 corresponds to 4/4 meter) // specify integer or decimal fraction
					if (atof(fBuf.getSliced(7, fBuf.length() - 7)) > 0.0 && thisMeasure <= 4999) {
						measureLength[thisMeasure] = atof(fBuf.getSliced(7, fBuf.length() - 7));
					}
				}
				else if (channel == 3) { //Change of BPM 	BPM 1 « [01-FF] » BPM 255
					for (int i = 0; i < (fBuf.length() - 7) / 2; i++) {
						int ii = i * 2 + 7;
						if (HEXcharToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1))) {
							gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = (i / (float)((fBuf.length() - 7) / 2)) + thisMeasure;
							gp->bmsobj.notes[gp->bmsobj.size].val = HEXcharToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1)) * gp->freqSpeedMultiplier;
							gp->bmsobj.notes[gp->bmsobj.size].op = 3;
							gp->bmsobj.size++;
							if (gp->bmsobj.size == gp->bmsobj.count) {
								ExpandNoteBuffer(&gp->bmsobj, 1000);
							}
						}
					}
				}
				else if (channel > 0) {
					for (int i = 0; i < (fBuf.length() - 7) / 2; i++) {
						int ii = i * 2 + 7;
						if (Base36ToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1))) {
							gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = (i / (float)((fBuf.length() - 7) / 2)) + thisMeasure;
							if (isVisibleNote(channel)) {
								if (lastMeasure <= thisMeasure) {
									lastMeasure = thisMeasure;
								}
							}

							if (isVisibleNote(channel) && firstNoteTime == -1.0) {
								firstNoteTime = gp->bmsobj.notes[gp->bmsobj.size].bmsTiming;
							}
							else if (gp->bmsobj.notes[gp->bmsobj.size].bmsTiming < firstNoteTime) {
								firstNoteTime = gp->bmsobj.notes[gp->bmsobj.size].bmsTiming;
							}

							if (isVisibleNote(channel) && lastNoteTime < gp->bmsobj.notes[gp->bmsobj.size].bmsTiming) {
								lastNoteTime = gp->bmsobj.notes[gp->bmsobj.size].bmsTiming;
							}

							if (channel == 8 || channel == 9) { //BPM, STOP
								gp->bmsobj.notes[gp->bmsobj.size].val = Base36ToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1));
							}
							else {
								gp->bmsobj.notes[gp->bmsobj.size].val = Base36ToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1)) + stage * 1296;
							}
							gp->bmsobj.notes[gp->bmsobj.size].op = channel;
							gp->bmsobj.size++;
							if (gp->bmsobj.size == gp->bmsobj.count) ExpandNoteBuffer(&gp->bmsobj, 1000);

							if (((10 <= channel && channel < 20) || (30 <= channel && channel < 40) || (50 <= channel && channel < 60)) && (meta->keymode < 10 && ((cfg->play.battle == 1 && (cfg->play.random[0] != cfg->play.random[1])) || cfg->play.battle == 2))) {
								gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = i / (float)((fBuf.length() - 7) / 2) + thisMeasure;
								gp->bmsobj.notes[gp->bmsobj.size].val = Base36ToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1)) + stage * 1296;
								gp->bmsobj.notes[gp->bmsobj.size].op = channel + 10;
								gp->bmsobj.size++;
								if (gp->bmsobj.size == gp->bmsobj.count) ExpandNoteBuffer(&gp->bmsobj, 1000);
							}
						}
					}
				}
				*fBuf.atPos(0) = '\0';
				isPMS = is9key;
			}
			else if (fBuf.left(5).isSame("#BPM ")) {
				int val = atof(fBuf.right(fBuf.length() - 5));
				if (gp->BPM_fix <= 0.0) {
					gp->BPM_fix = val * gp->freqSpeedMultiplier;
				}
				if (stage > 0 && (gp->courseConnection[stage - 1] == 1 || gp->courseConnection[stage - 1] == 3)) {
					gp->freqSpeedMultiplier *= nowBPM / val;
					aud->param.stagePitch[stage] = nowBPM / val;
					ErrorLogFmtAdd("BPMを前の曲に合わせました。 stagepitch %d\n", nowBPM / val * 100.0);
				}
				nowBPM = val * gp->freqSpeedMultiplier;
				if (stage > 0) {
					gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = (double)stageStartMeasure;
					gp->bmsobj.notes[gp->bmsobj.size].val = val * gp->freqSpeedMultiplier;
					gp->bmsobj.notes[gp->bmsobj.size].op = 3;
					gp->bmsobj.size++;
				}
				if (stage == 0) {
					gp->bpmt_data[gp->bpmt_count].BPM = gp->BPM_fix;
					gp->bpmt_data[gp->bpmt_count].converted = 0.0;
					gp->bpmt_data[gp->bpmt_count].realtime = 0.0;
					gp->maxBPM = gp->BPM_fix;
					gp->minBPM = gp->BPM_fix;
				}
				else {
					gp->bpmt_data[gp->bpmt_count].BPM = gp->BPM;
					gp->bpmt_data[gp->bpmt_count].converted = (double)stageStartMeasure;
					gp->bpmt_data[gp->bpmt_count].realtime = 0.0;
				}
				gp->bpmt_count++;
				if (gp->bmsobj.size == gp->bmsobj.count) {
					ExpandNoteBuffer(&gp->bmsobj, 1000);
				}
			}
			else if (fBuf.left(7).isSame("#FP/DSC")) {
				isDSC = 1;
				is9key = 1;
			}
			else if (fBuf.left(7).isSame("#LNOBJ ")) {
				lnobj = Base36ToInt(*fBuf.atPos(7), *fBuf.atPos(8)) + stage * 1296;
			}
			else if (fBuf.left(7).isSame("#TOTAL ")) {
				total[1] = total[0] = atol(fBuf.right(fBuf.length() - 7));
			}
			else if (fBuf.left(6).isSame("#RANK ")) {
				if (stage == 0) {
					rank = atol(fBuf.right(fBuf.length() - 6));
				}
				else {
					gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = stageStartMeasure;
					gp->bmsobj.notes[gp->bmsobj.size].val = atof(fBuf.right(fBuf.length() - 6));
					gp->bmsobj.notes[gp->bmsobj.size].op = 1001;
					gp->bmsobj.size++;
					if (gp->bmsobj.size == gp->bmsobj.count) {
						ExpandNoteBuffer(&gp->bmsobj, 1000);
					}
				}
			}
			else if (fBuf.left(4).isSame("#BPM")) {
				int param1;
				double param2;
				param1 = Base36ToInt(*fBuf.atPos(4), *fBuf.atPos(5));
				param2 = atof(fBuf.right(fBuf.length() - 7));
				if (1 <= param1 && param1 < 1296) {
					BPMslot[param1] = param2 * gp->freqSpeedMultiplier;
				}
			}
			else if (fBuf.left(5).isSame("#STOP")) {
				int param1, param2;
				param1 = Base36ToInt(*fBuf.atPos(5), *fBuf.atPos(6));
				param2 = atol(fBuf.right(fBuf.length() - 8));
				if (1 <= param1 && param1 < 1296 && param2 > 0) {
					STOPslot[param1] = param2;
				}
			}
			else if (fBuf.left(4).isSame("#WAV") && cfg->play.autojudge != 2) {
				int param1;
				param1 = Base36ToInt(*fBuf.atPos(4), *fBuf.atPos(5));
				gp->loadObject_total++;
				if (param1 < 1296) {
					if (gp->is2Pplay == 0 && stage * 1296 + param1 < 1296) {
						CSTR filename = fBuf.right(fBuf.length() - 7);
						filename.nullAtPos(2);
						for (int i = 0; true; i++) {
							if (i == cc.arr1count) {
								cc.arr1[i].filenameHead = filename;
								cc.arr3[stage * 1296 + param1].soundLoadID = i;
								cc.arr1count++;
								break;
							}
							if (filename.isSame(cc.arr1[i].filenameHead)) {
								cc.arr3[stage * 1296 + param1].soundLoadID = i;
								break;
							}
						}
					}
					fBuf = fBufOrg;
					*fBufOrg.atPos(0) = 0;
					int wavNum = Base36ToInt(*fBuf.atPos(4), *fBuf.atPos(5));
					if (wavNum < 1296) {
						fBuf.lastCut(fBuf.length() - 7);
						FindAltSound(fBuf, directory, &gp->keysound_filename[stage * 1296 + wavNum]);
					}
				}
			}
			else if (fBuf.left(4).isSame("#BMP") && (cfg->play.bga == 3 || cfg->play.bga == 1 || (cfg->play.bga == 2 && gp->isAutoplay == 1) || gp->replay.status == 2) && cfg->play.autojudge != 2) {
				int param1;
				gp->loadObject_total++;
				param1 = Base36ToInt(*fBuf.atPos(4), *fBuf.atPos(5));
				fBuf = fBufOrg;
				*fBufOrg.atPos(0) = 0;
				if (param1 < 1296) {
					fBuf.lastCut(fBuf.length() - 7);
					FindAltImage(fBuf, directory, &gp->BMP_filename[stage*1296 + param1]);
				}
			}
			else {
				GetStringBodyInt(&fBuf, "#PLAYLEVEL", &cc.playlevel);
			}
			*fBuf.atPos(0) = '\0';
		}
		fclose(hFile);
		/* parse BMS file end*/

		if (gp->soundonly && gp->is2Pplay == 0) {
			gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = stageStartMeasure;
			gp->bmsobj.notes[gp->bmsobj.size].val = 1295.0;
			gp->bmsobj.notes[gp->bmsobj.size].op = 4;
			gp->bmsobj.size++;
			if (gp->bmsobj.size == gp->bmsobj.count) ExpandNoteBuffer(&gp->bmsobj, 1000);
		}
		for (int i = stageStartMeasure; i < stageStartMeasure + 1000; i++) {
			gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = i;
			gp->bmsobj.notes[gp->bmsobj.size].val = measureLength[i];
			gp->bmsobj.notes[gp->bmsobj.size].op = 2;
			gp->bmsobj.size++;
			if (gp->bmsobj.size == gp->bmsobj.count) ExpandNoteBuffer(&gp->bmsobj, 1000);
		}

		if (bmsobj_stageFirst < gp->bmsobj.size) {
			qsort(&gp->bmsobj.notes[bmsobj_stageFirst], gp->bmsobj.size - bmsobj_stageFirst, sizeof(NoteStruct), CMP_NotesByBmsTiming);
		}

		for (int i = 0; i < gp->bmsobj.size && gp->bmsobj.notes[i].bmsTiming == 0; i++) {
			if ((10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 29) || (30 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 49)) {
				for (int j = 0; j < gp->bmsobj.size; j++) {
					gp->bmsobj.notes[j].bmsTiming += 1.0;
				}
				break;
			}
		}
		double meaMultiplier = measureLength[stageStartMeasure];
		meaLength = measureLength[stageStartMeasure];
		double stopRealtime = 0.0;
		double stopVal = 0.0;
		double scrollSpeed = 1.0;

		double unk23484_bmstime = -1.0;
		double unk2346c_realtime = -1.0;
		double _bPrevNoteTime = -1.0;

		double b2bmsTime = -1.0;
		double b2realTime = -1.0;
		double b2prevNoteTime = -1.0;

		int intArr2[30];
		for (int i = 0; i < 30; i++) intArr2[i] = -1;

		int bpmt_count = -1;
		int unk23538_objNum = -1;
		int b2bpmt_last = -1;

		int objNumLastNote = 0;
		for (int i = gp->bmsobj.size - 1; bmsobj_stageFirst <= i; i--) {
			if ((10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30) || (50 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 70)) {
				objNumLastNote = i;
				break;//i = 0;
			}
		}
		int objNumLastMeasureLate = -1;
		for (int i = objNumLastNote; bmsobj_stageFirst <= i; i--) {
			if (gp->bmsobj.notes[i].op == 2) {
				objNumLastMeasureLate = i;
				break; //i = 0;
			}
		}
		int objNumLastMeasureEarly = -1;
		for (int i = objNumLastNote; i < gp->bmsobj.size; i++) {
			if (gp->bmsobj.notes[i].op == 2) {
				objNumLastMeasureEarly = i;
				break; //i = gp->bmsobj.size;
			}
		}
		int objNumLastMeasure;
		if (gp->bmsobj.notes[objNumLastMeasureEarly].bmsTiming - gp->bmsobj.notes[objNumLastNote].bmsTiming < gp->bmsobj.notes[objNumLastNote].bmsTiming - gp->bmsobj.notes[objNumLastMeasureLate].bmsTiming) {
			lastNoteTime = gp->bmsobj.notes[objNumLastMeasureEarly].bmsTiming;
			objNumLastMeasure = objNumLastMeasureEarly;
		}
		else {
			lastNoteTime = gp->bmsobj.notes[objNumLastMeasureLate].bmsTiming;
			objNumLastMeasure = objNumLastMeasureLate;
		}

		int objNumFirstNote = bmsobj_stageFirst;
		for (int i = bmsobj_stageFirst; i < gp->bmsobj.size; i++) {
			if ((10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30) || (50 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 70)) {
				objNumFirstNote = i;
				break;//i = 0;
			}
		}
		int objNumFirstMeasureLate = -1;
		for (int i = objNumFirstNote; bmsobj_stageFirst <= i; i--) {
			if (gp->bmsobj.notes[i].op == 2) {
				objNumFirstMeasureLate = i;
				break; //i = 0;
			}
		}
		int objNumFirstMeasureEarly = -1;
		for (int i = objNumFirstNote; i < gp->bmsobj.size; i++) {
			if (gp->bmsobj.notes[i].op == 2) {
				objNumFirstMeasureEarly = i;
				break; //i = gp->bmsobj.size;
			}
		}
		int objNumFirstMeasure = objNumFirstMeasureLate;
		if (gp->bmsobj.notes[objNumFirstMeasureEarly].bmsTiming - gp->bmsobj.notes[objNumFirstNote].bmsTiming < gp->bmsobj.notes[objNumFirstNote].bmsTiming - gp->bmsobj.notes[objNumFirstMeasureLate].bmsTiming) {
			objNumFirstMeasure = objNumFirstMeasureEarly;
		}


		/* start of setting notes on time*/
		for (int i = bmsobj_stageFirst; i < gp->bmsobj.size; i++) {
			if (stage != 0 && gp->bmsobj.notes[i].bmsTiming <= 0) {
				ErrorLogFmtAdd("エラーノートを発見しました ch=%d\n", gp->bmsobj.notes[i].op);
			}

			if (gp->bmsobj.notes[i].bmsTiming != prevNoteRealtime) {
				if (stopRealtime) {
					stopRealtime = (stopVal / 192.0) * (240000.0 / nowBPM); // STOP * 1/192 bar real millisec (240BPM 1bar = 1sec)

					if (gp->bpmt_count == gp->bpmt_buffersize) {
						gp->bpmt_buffersize += 100;
						gp->bpmt_data = (BPMtiming*)realloc(gp->bpmt_data, gp->bpmt_buffersize * sizeof(BPMtiming));
						for (int i = gp->bpmt_buffersize - 100; i < gp->bpmt_buffersize; i++) {
							gp->bpmt_data[i].BPM = 0;
							gp->bpmt_data[i].converted = 0;
							gp->bpmt_data[i].BPM = 0;
						}
					}
					gp->bpmt_data[gp->bpmt_count].BPM = 0;
					gp->bpmt_data[gp->bpmt_count].realtime = bpmt_realtime;
					gp->bpmt_data[gp->bpmt_count].converted = bpmt_bmstime;
					gp->bpmt_count++;

					if (gp->bpmt_count == gp->bpmt_buffersize) {
						gp->bpmt_buffersize += 100;
						gp->bpmt_data = (BPMtiming*)realloc(gp->bpmt_data, gp->bpmt_buffersize * sizeof(BPMtiming));
						for (int i = gp->bpmt_buffersize - 100; i < gp->bpmt_buffersize; i++) {
							gp->bpmt_data[i].BPM = 0;
							gp->bpmt_data[i].converted = 0;
							gp->bpmt_data[i].BPM = 0;
						}
					}
					gp->bpmt_data[gp->bpmt_count].BPM = nowBPM;
					gp->bpmt_data[gp->bpmt_count].realtime = bpmt_realtime + stopRealtime;
					gp->bpmt_data[gp->bpmt_count].converted = bpmt_bmstime;
					gp->bpmt_count++;
				}
				
				double addRealtime = (240.0 / nowBPM * meaLength * (gp->bmsobj.notes[i].bmsTiming - prevNoteRealtime) * 1000.0);
				bpmt_realtime += addRealtime + stopRealtime;
				if (cfg->play.hsfix == 4 || (gp->is2Pplay && gp->courseType == 1)) {
					bpmt_bmstime += addRealtime * 1.2;
					prevNoteRealtime = gp->bmsobj.notes[i].bmsTiming;
					stopRealtime = 0.0;
				}
				else {
					bpmt_bmstime += meaLength * 1920.0 * (gp->bmsobj.notes[i].bmsTiming - prevNoteRealtime);
					prevNoteRealtime = gp->bmsobj.notes[i].bmsTiming;
					stopRealtime = 0.0;
				}
			}

			if (objNumLastMeasure == i) {
				lastMeasure = gp->bmsobj.notes[i].bmsTiming;
				unk23484_bmstime = bpmt_bmstime;
				unk2346c_realtime = bpmt_realtime;
				_bPrevNoteTime = gp->bmsobj.notes[i].bmsTiming;
				bpmt_count = gp->bpmt_count - 1;
				ErrorLogFmtAdd("リミット%d , %d\n", (int)bpmt_bmstime, (int)bpmt_realtime);
			}
			else if (unk23538_objNum == -1 && 0 < (int)unk23484_bmstime && gp->bmsobj.notes[i].op == 2 && unk2346c_realtime + 5000.0 < bpmt_realtime && gp->courseConnection[stage] == 5) {
				b2lastMeasure = gp->bmsobj.notes[i].bmsTiming;
				b2bmsTime = bpmt_bmstime;
				b2realTime = bpmt_realtime;
				b2prevNoteTime = gp->bmsobj.notes[i].bmsTiming;
				b2bpmt_last = gp->bpmt_count - 1;
				unk23538_objNum = i;
				ErrorLogFmtAdd("リミット2 %d , %d\n", (int)bpmt_bmstime, (int)bpmt_realtime);
			}

			gp->bmsobj.notes[i].bmsTiming = bpmt_bmstime;
			gp->bmsobj.notes[i].realTiming = bpmt_realtime;
			gp->bmsobj.notes[i].active = 0;
			if (50 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 70) {
				if (intArr2[gp->bmsobj.notes[i].op - 40] == -1) {
					gp->bmsobj.notes[i].op -= 40;
					intArr2[gp->bmsobj.notes[i].op] = i;
				}
				else {
					gp->bmsobj.notes[intArr2[gp->bmsobj.notes[i].op - 40]].bmsTiming_ln = bpmt_bmstime;
					gp->bmsobj.notes[intArr2[gp->bmsobj.notes[i].op - 40]].realTiming_ln = bpmt_realtime;
					intArr2[gp->bmsobj.notes[i].op - 40] = -1;
				}
			}
			else if (lnobj != -1 && (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30)) {
				if (gp->bmsobj.notes[i].val != lnobj || intArr2[gp->bmsobj.notes[i].op] == -1) { //CHECK: haha
					intArr2[gp->bmsobj.notes[i].op] = i;
				}
				else {
					gp->bmsobj.notes[intArr2[gp->bmsobj.notes[i].op]].bmsTiming_ln = bpmt_bmstime;
					gp->bmsobj.notes[intArr2[gp->bmsobj.notes[i].op]].realTiming_ln = bpmt_realtime;
					intArr2[gp->bmsobj.notes[i].op] = -1;
					gp->bmsobj.notes[i].op = gp->bmsobj.notes[i].op + 40;
				}
			}

			if (130 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 150) {//mine
				gp->bmsobj.notes[i].op -= 120;
				gp->bmsobj.notes[i].channel = gp->bmsobj.notes[i].val;
				gp->bmsobj.notes[i].val = 0.0;
			}

			switch (gp->bmsobj.notes[i].op) {
				case 2: //length of measure
					if (cfg->play.m_softlanding == 0) {
						scrollSpeed = 1.0;
					}
					else {
						if (GetRand(1) == 0) {
							scrollSpeed = (GetRand(100) + 100.0) / 100.0;
						}
						else {
							scrollSpeed = 100.0 / (GetRand(100) + 100.0);
						}
					}
					meaMultiplier = gp->bmsobj.notes[i].val;
					meaLength = meaMultiplier * scrollSpeed;
					nowBPM = nowBPM * scrollSpeed;

					if (gp->bpmt_count == gp->bpmt_buffersize) {
						gp->bpmt_buffersize += 100;
						gp->bpmt_data = (BPMtiming*)realloc(gp->bpmt_data, gp->bpmt_buffersize * sizeof(BPMtiming));
						for (int i = gp->bpmt_buffersize - 100; i < gp->bpmt_buffersize; i++) {
							gp->bpmt_data[i].BPM = 0;
							gp->bpmt_data[i].converted = 0;
							gp->bpmt_data[i].BPM = 0;
						}
					}
					gp->bpmt_data[gp->bpmt_count].BPM = nowBPM;
					gp->bpmt_data[gp->bpmt_count].realtime = bpmt_realtime;
					gp->bpmt_data[gp->bpmt_count].converted = bpmt_bmstime;
					gp->bpmt_count++;
					break;

				case 3: //change of BPM
					if (gp->bmsobj.notes[i].val > 0) {
						nowBPM = scrollSpeed * gp->bmsobj.notes[i].val;

						if (gp->bpmt_count == gp->bpmt_buffersize) {
							gp->bpmt_buffersize += 100;
							gp->bpmt_data = (BPMtiming*)realloc(gp->bpmt_data, gp->bpmt_buffersize * sizeof(BPMtiming));
							for (int i = gp->bpmt_buffersize - 100; i < gp->bpmt_buffersize; i++) {
								gp->bpmt_data[i].BPM = 0;
								gp->bpmt_data[i].converted = 0;
								gp->bpmt_data[i].BPM = 0;
							}
						}
						gp->bpmt_data[gp->bpmt_count].BPM = nowBPM;
						gp->bpmt_data[gp->bpmt_count].realtime = bpmt_realtime;
						gp->bpmt_data[gp->bpmt_count].converted = bpmt_bmstime;
						gp->bpmt_count++;
					}
					break;

				case 8: {
					double BPM = BPMslot[(int)gp->bmsobj.notes[i].val];
					if (BPM < 0.0) {
						BPM *= -1.0;
					}
					nowBPM = BPM* scrollSpeed;
					gp->bmsobj.notes[i].val = BPMslot[(int)gp->bmsobj.notes[i].val];

					if (gp->bpmt_count == gp->bpmt_buffersize) {
						gp->bpmt_buffersize += 100;
						gp->bpmt_data = (BPMtiming*)realloc(gp->bpmt_data, gp->bpmt_buffersize * sizeof(BPMtiming));
						for (int i = gp->bpmt_buffersize - 100; i < gp->bpmt_buffersize; i++) {
							gp->bpmt_data[i].BPM = 0;
							gp->bpmt_data[i].converted = 0;
							gp->bpmt_data[i].BPM = 0;
						}
					}
					gp->bpmt_data[gp->bpmt_count].BPM = nowBPM;
					gp->bpmt_data[gp->bpmt_count].realtime = bpmt_realtime;
					gp->bpmt_data[gp->bpmt_count].converted = bpmt_bmstime;
					gp->bpmt_count++;
					break;
				}
				case 9:
					stopVal = STOPslot[(int)gp->bmsobj.notes[i].val];
					stopRealtime = stopVal / 192.0 * 240000.0 / nowBPM;
					gp->bmsobj.notes[i].val = (240000.0 / nowBPM) * STOPslot[(int)gp->bmsobj.notes[i].val] / 192.0;
					break;
			}

			if (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 29) {
				
				avgBPM_notes += 1;
				avgBPM_bpmsum += nowBPM;
				if (gp->maxBPM < nowBPM) gp->maxBPM = nowBPM;
				if (gp->minBPM > nowBPM) gp->minBPM = nowBPM;

				if (cfg->play.m_softlanding == 2) {
					if (GetRand(1) == 0) {
						scrollSpeed = (GetRand(100) + 100.0) / 100.0;
					}
					else {
						scrollSpeed = 100.0 / (GetRand(100) + 100.0);
					}

					meaLength = meaMultiplier * scrollSpeed;
					nowBPM = scrollSpeed * meaLength;

					if (gp->bpmt_count == gp->bpmt_buffersize) {
						gp->bpmt_buffersize += 100;
						gp->bpmt_data = (BPMtiming*)realloc(gp->bpmt_data, gp->bpmt_buffersize * sizeof(BPMtiming));
						for (int i = gp->bpmt_buffersize - 100; i < gp->bpmt_buffersize; i++) {
							gp->bpmt_data[i].BPM = 0;
							gp->bpmt_data[i].converted = 0;
							gp->bpmt_data[i].BPM = 0;
						}
					}
					gp->bpmt_data[gp->bpmt_count].BPM = nowBPM;
					gp->bpmt_data[gp->bpmt_count].realtime = bpmt_realtime;
					gp->bpmt_data[gp->bpmt_count].converted = bpmt_bmstime;
					gp->bpmt_count++;
				}
			}
		}
		/* end of setting notes on time*/

		if (gp->courseConnection[stage] == 5 && unk23538_objNum != -1) {
			unk23484_bmstime = b2bmsTime;
			objNumLastMeasure = unk23538_objNum;
			unk2346c_realtime = b2realTime;
			bpmt_count = b2bpmt_last;
			lastMeasure = b2lastMeasure;
			_bPrevNoteTime = b2prevNoteTime;
		}

		if (cfg->play.battle == 3) {
			if (cfg->play.battle == 3 && meta->keymode == 9 && gp->is2Pplay == 0 && gp->isPreviewLoad == 0) {
				ErrorLogFmtAdd("PMSTOSPマージを行います");
				for (int cur = 0; cur < gp->bmsobj.size; cur++) {
					gp->bmsobj.notes[cur].bmsTiming_ln = gp->bmsobj.notes[cur].bmsTiming;
					gp->bmsobj.notes[cur].realTiming_ln = gp->bmsobj.notes[cur].realTiming;
				}
				PMStoSP(gp);
			}
			if (cfg->play.battle == 3 && (meta->keymode == 10 || meta->keymode == 14) && gp->is2Pplay == 0 && gp->isPreviewLoad == 0) { //TOFIX: cfg->play.battle==3 duplicated
				ErrorLogFmtAdd("DPTOSPマージを行います");
				for (int cur = 0; cur < gp->bmsobj.size; cur++) {
					gp->bmsobj.notes[cur].bmsTiming_ln = gp->bmsobj.notes[cur].bmsTiming;
					gp->bmsobj.notes[cur].realTiming_ln = gp->bmsobj.notes[cur].realTiming;
				}
				DPtoSP(gp);
			}
		}
		if (cfg->play.is_extra > 0 && gp->is2Pplay == 0 && gp->isPreviewLoad == 0) {
			gp->extramode_level = cfg->play.m_extra;
			for (int cur = 0; cur < gp->bmsobj.size; cur++) {
				gp->bmsobj.notes[cur].bmsTiming_ln = gp->bmsobj.notes[cur].bmsTiming;
				gp->bmsobj.notes[cur].realTiming_ln = gp->bmsobj.notes[cur].realTiming;
			}
			MakeExtraChart(gp, &cc);
		}
		if (cfg->play.m_addnote > 0 && gp->is2Pplay == 0 && gp->isPreviewLoad == 0) {

			for (int i = 0; i < gp->bmsobj.size; i++) {
				gp->bmsobj.notes[i].bmsTiming_ln = gp->bmsobj.notes[i].bmsTiming;
				gp->bmsobj.notes[i].realTiming_ln = gp->bmsobj.notes[i].realTiming;
			}
			qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);

			double l_realTiming = 0.0;
			double t_realTiming = 0.0;
			double t_bmsTiming = 0.0;
			memset(mapAdded, 0, 20);
			int addNoteCount[2] = { 0, };
			int key = 7;
			if (meta->keymode == 5 || meta->keymode == 10) key = 5;
			else if (meta->keymode == 9) key = 9;

			for (int i = 0; i < gp->bmsobj.size; i++) {
				if (l_realTiming < gp->bmsobj.notes[i].realTiming) {
					for (int p = 0; p < 2; p++) {
						if (addNoteCount[p] > 0) {
							for (int a = 0; a < addNoteCount[p]; a++) {
								if (GetRand(100) <= cfg->play.m_addnote) {
									int emptyCount = 0;

									if (key > -1) {
										for (int lane = 0; lane <= key; lane++) {
											if (mapAdded[lane + p * 10] == 0) emptyCount++;
										}
										if (emptyCount && emptyCount > 0) {
											int addlane = GetRand(emptyCount - 1);

											for (int lane = 0; lane <= key; lane++) {
												if (mapAdded[lane + p * 10] == 0) {
													if (addlane == 0) {
														gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = t_bmsTiming;
														gp->bmsobj.notes[gp->bmsobj.size].bmsTiming_ln = t_bmsTiming;
														gp->bmsobj.notes[gp->bmsobj.size].realTiming = t_realTiming;
														gp->bmsobj.notes[gp->bmsobj.size].realTiming_ln = t_realTiming;
														gp->bmsobj.notes[gp->bmsobj.size].val = 1294.0;
														gp->bmsobj.notes[gp->bmsobj.size].op = p * 10 + 10 + lane;
														gp->bmsobj.size++;
														if (gp->bmsobj.size == gp->bmsobj.count) {
															ExpandNoteBuffer(&gp->bmsobj, 1000);
														}
														mapAdded[p][lane] = 1;
														lane += 100;
													}
													else {
														addlane--;
													}
												}
											}
										}
									}
								}
							}
						}
					}
					t_bmsTiming = gp->bmsobj.notes[i].bmsTiming;
					t_realTiming = gp->bmsobj.notes[i].realTiming;
					l_realTiming = gp->bmsobj.notes[i].realTiming;
					memset(mapAdded, 0, sizeof(mapAdded));
					addNoteCount[0] = 0;
					addNoteCount[1] = 0;
				}

				if (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 20) {
					addNoteCount[0]++;
					mapAdded[0][gp->bmsobj.notes[i].op - 10] = 1;
				}
				else if (20 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30) {
					addNoteCount[1]++;
					mapAdded[1][gp->bmsobj.notes[i].op - 20] = 1;
				}
			}
			qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
		}
		if (cfg->play.m_loudness > 0 && gp->is2Pplay == 0 && gp->isPreviewLoad == 0) {

			for (int i = 0; i < gp->bmsobj.size; i++) {
				gp->bmsobj.notes[i].bmsTiming_ln = gp->bmsobj.notes[i].bmsTiming;
				gp->bmsobj.notes[i].realTiming_ln = gp->bmsobj.notes[i].realTiming;
			}
			qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);

			double l_realTiming = 0.0;
			double t_realTiming = 0.0;
			double t_bmsTiming = 0.0;
			bool mapAdded[2][10] = { 0, };
			int addNoteCount[2] = { 0, };
			int key = 7;
			if (meta->keymode == 5 || meta->keymode == 10) key = 5;
			else if (meta->keymode == 9) key = 9;

			for (int i = 0; i < gp->bmsobj.size; i++) {
				if (l_realTiming < gp->bmsobj.notes[i].realTiming) {
					for (int p = 0; p < 2; p++) {
						if (addNoteCount[p] != 0) {

							if (GetRand(100) <= cfg->play.m_loudness) {
								if (key > -1) {
									for (int lane = 0; lane <= key; lane++) {
										if (mapAdded[lane + p * 10] == 0) {
											gp->bmsobj.notes[gp->bmsobj.size].bmsTiming = t_bmsTiming;
											gp->bmsobj.notes[gp->bmsobj.size].bmsTiming_ln = t_bmsTiming;
											gp->bmsobj.notes[gp->bmsobj.size].realTiming = t_realTiming;
											gp->bmsobj.notes[gp->bmsobj.size].realTiming_ln = t_realTiming;
											gp->bmsobj.notes[gp->bmsobj.size].val = 1294.0;
											gp->bmsobj.notes[gp->bmsobj.size].op = p * 10 + 10 + lane;
											gp->bmsobj.size++;
											if (gp->bmsobj.size == gp->bmsobj.count) {
												ExpandNoteBuffer(&gp->bmsobj, 1000);
											}
										}
									}
								}
							}

						}
					}
					t_bmsTiming = gp->bmsobj.notes[i].bmsTiming;
					t_realTiming = gp->bmsobj.notes[i].realTiming;
					l_realTiming = gp->bmsobj.notes[i].realTiming;
					memset(mapAdded, 0, sizeof(mapAdded));
					addNoteCount[0] = 0;
					addNoteCount[1] = 0;
				}

				if (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 20) {
					addNoteCount[0] = 1;
					mapAdded[0][gp->bmsobj.notes[i].op - 10] = 1;
				}
				else if (20 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30) {
					addNoteCount[1] = 1;
					mapAdded[1][gp->bmsobj.notes[i].op - 20] = 1;
				}
			}
			qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
		}

		double realDiff = 0.0;
		double bmsDiff = 0.0;

		if (stage >= 1) {
			stageStartMeasure = (int)firstNoteTime - stageStartMeasure - 1;

			if (gp->courseConnection[stage - 1] == 5) { //BLANK2
				stageStartMeasure = 0;
			}
			else {
				if (gp->courseConnection[stage - 1] == 4) { //BLANK1
					while (firstNoteTime - stageStartMeasure - prevStageLastNoteTime > 1.5) {
						stageStartMeasure++;
					}
				}
				else {
					while (firstNoteTime - stageStartMeasure - prevStageLastNoteTime > 0.5) {
						stageStartMeasure++;
					}
				}
			}

			lastNoteTime -= stageStartMeasure;

			int unk_start = stageStartMeasure;
			int k = bmsobj_stageFirst;
			/*
			for (int k = bmsobj_stageFirst; true; k++){
				if (gp->bmsobj.notes[k].op != 2) continue;
				
				if(unk_start == 0){
					if (gp->bmsobj.notes[bmsobj_stageFirst].bmsTiming <= 0.0 || gp->bmsobj.notes[bmsobj_stageFirst].realTiming <= 0.0 || gp->courseConnection[stage - 1] == 5) {
						//
					}
					else {
						double realDiff = gp->bmsobj.notes[k].realTiming - gp->bmsobj.notes[bmsobj_stageFirst].realTiming;
						double bmsDiff = gp->bmsobj.notes[k].bmsTiming - gp->bmsobj.notes[bmsobj_stageFirst].bmsTiming;
						for (int i = bmsobj_stageFirst; i < gp->bmsobj.size; i++) {
							gp->bmsobj.notes[i].bmsTiming -= bmsDiff;
							gp->bmsobj.notes[i].realTiming -= realDiff;
							gp->bmsobj.notes[i].bmsTiming_ln -= bmsDiff;
							gp->bmsobj.notes[i].realTiming_ln -= realDiff;
						}
						bpmt_realtime = unk2346c_realtime - realDiff;
						bpmt_bmstime = unk23484_bmstime - bmsDiff;
						prevNoteTime = _bPrevNoteTime - stageStartMeasure;
					}
					break;
				}
				unk_start--;
			}
			*/
			while (true) {
				if (gp->bmsobj.notes[k].op == 2) {
					if (unk_start == 0) break; 					
					else unk_start--;
				}
				k++;
			}
			/* That cutting place*/
			if (gp->bmsobj.notes[bmsobj_stageFirst].bmsTiming <= 0 || gp->bmsobj.notes[bmsobj_stageFirst].realTiming <= 0 || gp->courseConnection[stage - 1] == 5) {
				bpmt_realtime = unk2346c_realtime;
				bpmt_bmstime = unk23484_bmstime;
				prevNoteRealtime = _bPrevNoteTime;
			}
			else {
				realDiff = gp->bmsobj.notes[k].realTiming - gp->bmsobj.notes[bmsobj_stageFirst].realTiming;
				bmsDiff = gp->bmsobj.notes[k].bmsTiming - gp->bmsobj.notes[bmsobj_stageFirst].bmsTiming;
				for (int i = bmsobj_stageFirst; i < gp->bmsobj.size; i++) {
					gp->bmsobj.notes[i].bmsTiming -= bmsDiff;
					gp->bmsobj.notes[i].realTiming -= realDiff;
					gp->bmsobj.notes[i].bmsTiming_ln -= bmsDiff;
					gp->bmsobj.notes[i].realTiming_ln -= realDiff;
				}
				bpmt_realtime = unk2346c_realtime - realDiff;
				bpmt_bmstime = unk23484_bmstime - bmsDiff;
				prevNoteRealtime = _bPrevNoteTime - stageStartMeasure;
			}
		}
		else {
			//same with -22?line
			bpmt_realtime = unk2346c_realtime;
			bpmt_bmstime = unk23484_bmstime;
			prevNoteRealtime = _bPrevNoteTime;
		}
		// -2331 line
		// 4033- line
		if (gp->is2Pplay) {
			if (stage != stages - 1) {
				if (objNumLastMeasure > 0) {
					for (int i = objNumLastMeasure; i < gp->bmsobj.size; i++) {
						if (gp->bmsobj.notes[i].op == 2 || gp->bmsobj.notes[i].op == 3 || gp->bmsobj.notes[i].op == 8) {
							gp->bmsobj.notes[i].op = -1;
						}
					}
				}
				if (bpmt_count > 0) gp->bpmt_count = bpmt_count;
			}
			if (gp->is2Pplay && stage && objNumFirstMeasure > 0) {
				for (int i = bmsobj_stageFirst; i < objNumFirstMeasure; i++) {
					if (gp->bmsobj.notes[i].op == 2) {
						gp->bmsobj.notes[i].op = -1;
					}
				}
			}
		}
		if (stage != 0) {
			double t = 100.0 + prevStageTime;
			for (int i = bmsobj_stageFirst; gp->bmsobj.notes[i].realTiming <= t; i++) {
				if (i == gp->bmsobj.size) break;
				if (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30) {
					if (gp->bmsobj.notes[i].realTiming - prevStageTime < 100) {
						for (int j = bmsobj_stageFirst - 1; j > -1; j--) {
							if (gp->bmsobj.notes[j].realTiming < abs((int)prevStageTime - (int)100.0)) break; //need check float calc
							if (gp->bmsobj.notes[i].op = gp->bmsobj.notes[j].op) {
								gp->bmsobj.notes[i].op = 1;
								break;
							}
						}
					}
					else {
						gp->bmsobj.notes[i].op = 1;
					}
				}
			}
		}
		for (int i = bmsobj_stageFirst; i < gp->bmsobj.count; i++) {
			gp->bmsobj.notes[i].stage = stage;
			if (!(0 <= gp->bmsobj.notes[i].stage && gp->bmsobj.notes[i].stage <= 4)) {
				gp->bmsobj.notes[i].stage = 0;
			}
		}
		prevStageTime = -1.0;
		for (int i = gp->bmsobj.size - 1; i != bmsobj_stageFirst; i--) {
			if (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30) {
				prevStageTime = gp->bmsobj.notes[i].realTiming;
				break;
			}
		}

		if ((gp->courseConnection[stage] == 2 || gp->courseConnection[stage] == 3) && stage < stages - 1) { //CUT, CUT+FIT
			gp->fadeStartTime[stage] = bpmt_realtime;
			gp->fadeEndTime[stage] = bpmt_realtime + 200.0;
			gp->songStartTime[stage+1] = bpmt_realtime - 200.0;
			gp->songEndTime[stage+1] = bpmt_realtime;
		}
		else if ((gp->courseConnection[stage] == 1 || gp->courseConnection[stage] == 0) && stage < stages - 1) { //FADE, FADE+FIT
			if (gp->courseConnection[stage] != 1) {
				gp->fadeStartTime[stage] = bpmt_realtime;
				gp->fadeEndTime[stage] = bpmt_realtime + 10000.0;
			}
		}
		else if (gp->courseConnection[stage] == 4 && stage < stages - 1) { //BLANK1
			gp->songStartTime[stage+1] = bpmt_realtime - 5000.0;
			gp->songEndTime[stage+1] = bpmt_realtime;
		}

		gp->connection_unkC[stage + 1] = bpmt_realtime;
		gp->connection_unkD[stage + 1] = bpmt_realtime + 3000.0;

		if (stage < stages - 1) {
			gp->connection_unkA[stage + 1] = bpmt_realtime - 3000.0;
			gp->connection_unkB[stage + 1] = bpmt_realtime;
		}

		if (stage != 0 && oldbpmtCount > 0  && realDiff != 0.0) { 
			for (int i = oldbpmtCount; i < gp->bpmt_count; i++) {
				if (gp->bpmt_data[oldbpmtCount - 1].realtime <= gp->bpmt_data[i].realtime - realDiff) { //TOFIX : nonstop mix sink mismatch after stop (stage 2-) related code
					gp->bpmt_data[i].realtime -= realDiff;
					gp->bpmt_data[i].converted -= bmsDiff;
				}
			}
		}
	}
	/* end of stage loop */
	//2345- line
	gp->freqSpeedMultiplier = oldSpeedMultiplier;
	qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTiming);

	if (gp->is2Pplay == 1 && gp->courseType == 1) gp->speedmultiplier = 1.0;
	else if (avgBPM_notes > 0 && cfg->play.hsfix == 3) gp->speedmultiplier = 150.0 / (avgBPM_bpmsum / avgBPM_notes); //average
	else if (gp->maxBPM > 0.0 && cfg->play.hsfix == 1) gp->speedmultiplier = 150.0 / gp->maxBPM;
	else if (gp->minBPM > 0.0 && cfg->play.hsfix == 2) gp->speedmultiplier = 150.0 / gp->minBPM;
	else gp->speedmultiplier = 1.0;

	if (cfg->play.m_loudness > 0 || cfg->play.is_extra > 0 || cfg->play.p1_assist > 0 || cfg->play.p2_assist > 0 || cfg->play.battle || cfg->play.m_addnote > 0)
		gp->isGhostDisabled = 1;
	if (0 < cfg->play.m_loudness)
		gp->isNosave = 1;
	if (gp->freqSpeedMultiplier < 1.0)
		gp->isNosave = 1;
	if (0 < cfg->play.m_addnote)
		gp->isNosave = 1;
	if (cfg->play.battle == 1 && gp->ghostBattle == 0)
		gp->isNosave = 1;
	if (gp->replay.status == 2)
		gp->isNosave = 1;
	if (cfg->play.hsfix == 4 && gp->minBPM != gp->maxBPM)
		gp->isForceEasy = 1;
	if (cfg->play.random[0] == 4 || cfg->play.random[1] == 4)
		gp->isForceEasy = 1;
	if (cfg->play.gaugeOption[0] == 3)
		gp->isForceEasy = 1;
	if ((cfg->play.p1_assist == 1 || cfg->play.p2_assist == 1) && (7 < meta->keymode || cfg->play.battle != 2))
		gp->isForceEasy = 1;
	if (cfg->play.m_lunaris)
		gp->isNosave = 1;

	for (int i = 0; i < 20; i++) { //calculated Out of Bound
		noteRandomTable[0][i] = i;
	}
	for (int p = 0; p < 2; p++) {
		if (cfg->play.random[p] == 1) { //mirror
			if (meta->keymode == 7 || meta->keymode == 14) {
				if (cfg->play.assist[p] == 0) {
					noteRandomTable[p][1] = 7 + p * 10;
					noteRandomTable[p][2] = 6 + p * 10;
					noteRandomTable[p][3] = 5 + p * 10;
					noteRandomTable[p][4] = 4 + p * 10;
					noteRandomTable[p][5] = 3 + p * 10;
					noteRandomTable[p][6] = 2 + p * 10;
					noteRandomTable[p][7] = 1 + p * 10;
				}
				else {
					noteRandomTable[p][0] = 7 + p * 10;
					noteRandomTable[p][1] = 6 + p * 10;
					noteRandomTable[p][2] = 5 + p * 10;
					noteRandomTable[p][3] = 4 + p * 10;
					noteRandomTable[p][4] = 3 + p * 10;
					noteRandomTable[p][5] = 2 + p * 10;
					noteRandomTable[p][6] = 1 + p * 10;
					noteRandomTable[p][7] = 0 + p * 10;
				}
			}
			else if (meta->keymode == 5 || meta->keymode == 10) {
				if (cfg->play.assist[p] == 0) {
					noteRandomTable[p][1] = 5 + p * 10;
					noteRandomTable[p][2] = 4 + p * 10;
					noteRandomTable[p][3] = 3 + p * 10;
					noteRandomTable[p][4] = 2 + p * 10;
					noteRandomTable[p][5] = 1 + p * 10;
				}
				else {
					noteRandomTable[p][0] = 5 + p * 10;
					noteRandomTable[p][1] = 4 + p * 10;
					noteRandomTable[p][2] = 3 + p * 10;
					noteRandomTable[p][3] = 2 + p * 10;
					noteRandomTable[p][4] = 1 + p * 10;
					noteRandomTable[p][5] = 0 + p * 10;
				}
			}
			else if (meta->keymode == 9) {
				noteRandomTable[0][1] = 9;
				noteRandomTable[0][2] = 8;
				noteRandomTable[0][3] = 7;
				noteRandomTable[0][4] = 6;
				noteRandomTable[0][5] = 5;
				noteRandomTable[0][6] = 4;
				noteRandomTable[0][7] = 3;
				noteRandomTable[0][8] = 2;
				noteRandomTable[0][9] = 1;
			}
		}
		else if (cfg->play.random[p] == 2) { //random
			if (meta->keymode == 9) {
				for (int c = 1; c < 9; c++) {
					int a = c + GetRand(9 - c);
					int tmp = noteRandomTable[0][c];
					noteRandomTable[0][c] = noteRandomTable[0][a];
					noteRandomTable[0][a] = tmp;
				}
			}
			else if (meta->keymode == 7 || meta->keymode == 14) {
				if (cfg->play.assist[p] == 0) {
					for (int c = 1; c < 7; c++) {
						int a = c + GetRand(7 - c);
						int tmp = noteRandomTable[p][c];
						noteRandomTable[p][c] = noteRandomTable[p][a];
						noteRandomTable[p][a] = tmp;
					}

					if (0 < cfg->play.rand[p] && cfg->play.rand[p] < 8) {
						for (int c = 1; c <= 7; c++) {
							if (noteRandomTable[p][c] == cfg->play.rand[p]) {
								noteRandomTable[p][c] = noteRandomTable[p][1];
								noteRandomTable[p][1] = cfg->play.rand[p];
							}
						}
					}
				}
				else {
					for (int c = 0; c < 7; c++) {
						int a = c + GetRand(7 - c);
						int tmp = noteRandomTable[p][c];
						noteRandomTable[p][c] = noteRandomTable[p][a];
						noteRandomTable[p][a] = tmp;
					}

					if (0 < cfg->play.rand[p] && cfg->play.rand[p] < 8) {
						for (int c = 1; c <= 7; c++) {
							if (noteRandomTable[p][c] == 0) {
								noteRandomTable[p][c] = noteRandomTable[p][cfg->play.rand[p]];
								noteRandomTable[p][cfg->play.rand[p]] = 0;
							}
						}
					}
				}
			}
			else if (meta->keymode == 5 || meta->keymode == 10) {
				if (cfg->play.assist[p] == 0) {
					for (int c = 1; c < 5; c++) {
						int a = c + GetRand(5 - c);
						int tmp = noteRandomTable[p][c];
						noteRandomTable[p][c] = noteRandomTable[p][a];
						noteRandomTable[p][a] = tmp;
					}

					if (0 < cfg->play.rand[p] && cfg->play.rand[p] < 6) {
						for (int c = 1; c <= 5; c++) {
							if (noteRandomTable[p][c] == cfg->play.rand[p]) {
								noteRandomTable[p][c] = noteRandomTable[p][1];
								noteRandomTable[p][1] = cfg->play.rand[p];
							}
						}
					}
				}
				else {
					for (int c = 0; c < 5; c++) {
						int a = c + GetRand(5 - c);
						int tmp = noteRandomTable[p][c];
						noteRandomTable[p][c] = noteRandomTable[p][a];
						noteRandomTable[p][a] = tmp;
					}

					if (0 < cfg->play.rand[p] && cfg->play.rand[p] < 6) {
						for (int c = 1; c <= 5; c++) {
							if (noteRandomTable[p][c] == 0) {
								noteRandomTable[p][c] = noteRandomTable[p][cfg->play.rand[p]];
								noteRandomTable[p][cfg->play.rand[p]] = 0;
							}
						}
					}
				}
			}
		}
	}

	double p1LastTiming = 0.0, p2LastTiming = 0.0;
	int intArr[30] = { -1, };
	for (int i = 0; i < 30; i++) intArr[i] = -1;
	memset(mapAdded, 0, sizeof(mapAdded));
	char chArr[20] = { 0, };
	int unkFbuf = 0; //rename this unused one
	int unused_LaneA[10], unused_LaneB[10], unused_swapLane[10];
	for (int i = 0; i < 10; i++) unused_LaneA[i] = -1;
	for (int i = 0; i < 10; i++) unused_LaneB[i] = -1;
	int isBattle = 0;

	//shuffle notes
	for (int i = 0; i < gp->bmsobj.size; i++) {
		int optemp = gp->bmsobj.notes[i].op;
		if (optemp < 10 || optemp >= 30) {
			if (optemp == 2) {
				if (cfg->play.battle == 3 && (meta->keymode == 5 || meta->keymode == 7) && gp->is2Pplay == 0) {
					for (int j = 0; j < 10; j++) {
						unused_swapLane[j] = unused_LaneA[j];
						unused_LaneA[j] = unused_LaneB[j];
						unused_LaneB[j] = unused_swapLane[j];
					}
					SPtoDP(&gp->bmsobj, i, &cc);
				}

				memcpy(&gp->bmsobj_line.notes[gp->bmsobj_line.size], &gp->bmsobj.notes[i].bmsTiming, sizeof(NoteStruct));
				gp->bmsobj_line.size++;;

				if (gp->bmsobj_line.size == gp->bmsobj_line.count) {
					ExpandNoteBuffer(&gp->bmsobj_line, 100);
				}
			}
			else if (optemp == 1 && gp->bmsobj.notes[i].realTiming > endtime) {
				endtime = gp->bmsobj.notes[i].realTiming;
			}
		}
		else {
			if (optemp < 20) {
				if (p1LastTiming < gp->bmsobj.notes[i].realTiming) {
					p1LastTiming = gp->bmsobj.notes[i].realTiming;
					isBattle = 0;
					for (int lane = 0; lane < 10; lane++) {
						if (cfg->play.random[0] == 4) {
							mapAdded[0][lane] = chArr[lane];
						}
						else {
							mapAdded[0][lane] = 0;
						}

						if (intArr[lane] == -1 || gp->bmsobj.notes[intArr[lane]].realTiming_ln < gp->bmsobj.notes[i].realTiming) {
							chArr[lane] = 0;
						}
					}
					
					if (unkFbuf == 1 || unkFbuf == 2) unkFbuf += 2;
					else unkFbuf = 0;
				}
			}
			else {
				isBattle = (cfg->play.battle == 1);
				if (p2LastTiming < gp->bmsobj.notes[i].realTiming) {
					p2LastTiming = gp->bmsobj.notes[i].realTiming;
					for (int lane = 0; lane < 10; lane++) {
						if (cfg->play.random[1] == 4) {
							mapAdded[1][lane] = chArr[10 + lane];
						}
						else {
							mapAdded[1][lane] = 0;
						}

						if (intArr[10 + lane] == -1 || gp->bmsobj.notes[intArr[10 + lane]].realTiming_ln < gp->bmsobj.notes[i].realTiming) {
							chArr[10 + lane] = 0;
						}
					}

					if (unkFbuf == 1 || unkFbuf == 2) unkFbuf += 2;
					else unkFbuf = 0;
				}
			}

			if ((meta->keymode == 10 || meta->keymode == 14) && cfg->play.dpflip == 1) {
				if (gp->bmsobj.notes[i].op < 20) gp->bmsobj.notes[i].op += 10;
				else gp->bmsobj.notes[i].op -= 10;
			}

			if ( (cfg->play.random[0] >= 3 && gp->bmsobj.notes[i].op < 20) || (cfg->play.random[1] >= 3 && gp->bmsobj.notes[i].op >= 20) ) {
				if (meta->keymode == 5 || meta->keymode == 10) {
					if (cfg->play.rand[0] >= 6) cfg->play.rand[0] = 0;
					if (cfg->play.rand[1] >= 6) cfg->play.rand[1] = 0;
				}
				else if (meta->keymode == 7 || meta->keymode == 14) {
					if (cfg->play.rand[0] >= 8) cfg->play.rand[0] = 0;
					if (cfg->play.rand[1] >= 8) cfg->play.rand[1] = 0;
				}
				else if (meta->keymode == 9) {
					if (cfg->play.rand[0] == 0) cfg->play.rand[0] = 5;
					if (cfg->play.rand[1] == 0) cfg->play.rand[1] = 5;
				}

				int assist = 0;
				if (cfg->play.random[0] >= 3 && gp->bmsobj.notes[i].op < 20) assist = (cfg->play.assist[0] != 0);
				else if (cfg->play.random[1] >= 3 && gp->bmsobj.notes[i].op >= 20) assist = (cfg->play.assist[1] != 0);

				int randLanes;
				switch (meta->keymode) {
					case 5:
					case 10:
						randLanes = 4 + assist;
						break;

					case 7:
					case 14:
						randLanes = 6 + assist;
						break;

					case 9:
						randLanes = 9;
						break;
				}

				int startlane;
				if (gp->bmsobj.notes[i].op < 20) {
					startlane = 1 - assist;
				}
				else {
					startlane = 11 - assist;
				}

				int lane = gp->bmsobj.notes[i].op - 10;
				if (startlane <= lane && lane <= startlane + randLanes) {
					bool pass = 1, pass2 = 1;
					while(pass2){
						lane = startlane + GetRand(randLanes);
						if (pass) {
							if (gp->bmsobj.notes[i].op < 20 && cfg->play.random[0] == 5) {
								lane = cfg->play.rand[0];
								pass = 0;
							}
							if (gp->bmsobj.notes[i].op >= 20 && cfg->play.random[1] == 5) {
								lane = cfg->play.rand[1] + startlane;
								pass = 0;
							}
						}

						if (intArr[lane] == -1) {
							pass2 = 0;
							if (mapAdded[0][lane] == 0) break;
							if (randLanes < startlane) break;
							for (int j = startlane; j <= randLanes; j++) {
								if (mapAdded[0][j] == 0) pass2 = 1;
							}
						}
						else {
							if(gp->bmsobj.notes[i].realTiming <= gp->bmsobj.notes[intArr[lane]].realTiming_ln || gp->bmsobj.notes[i].realTiming == gp->bmsobj.notes[intArr[lane]].realTiming) 
								continue;

							pass2 = 0;
							if (mapAdded[0][lane] == 0) break;
							if (randLanes < startlane) break;
							for (int j = startlane; j <= randLanes; j++) {
								if (mapAdded[0][j] == 0) pass2 = 1;
							}
						}
					}
					mapAdded[0][lane] = 1;
					gp->bmsobj.notes[i].op = lane + 10;
				}
			}
			
			gp->bmsobj.notes[i].op;
			chArr[gp->bmsobj.notes[i].op - 10] = 1;
			if (meta->keymode == 14) {
				if (gp->bmsobj.notes[i].op == 10) {
					mapAdded[0][4] = 1;
					mapAdded[0][5] = 1;
					mapAdded[0][6] = 1;
					mapAdded[0][7] = 1;
				}
				else if (gp->bmsobj.notes[i].op == 20) {
					mapAdded[1][1] = 1;
					mapAdded[1][2] = 1;
					mapAdded[1][3] = 1;
					mapAdded[1][4] = 1;
				}
			}
			bool chnValid = (gp->bmsobj.notes[i].channel > 0);//
			if (gp->bmsobj.notes[i].realTiming_ln + 2000.0 > endtime) {
				endtime = gp->bmsobj.notes[i].realTiming_ln + 2000.0;
			}
			else if (gp->bmsobj.notes[i].realTiming + 2000.0 > endtime) {
				endtime = gp->bmsobj.notes[i].realTiming + 2000.0;
			}

			int lane = noteRandomTable[0][gp->bmsobj.notes[i].op - 10];
			if (cfg->play.battle == 2 && cfg->play.assist[0] == 0 && cfg->play.assist[1] == 0 && gp->bmsobj.notes[i].op == 10) {
				gp->bmsobj.notes[i].op = 1;
			}
			else if (cfg->play.battle == 2 && cfg->play.assist[0] == 0 && cfg->play.assist[1] == 0 && gp->bmsobj.notes[i].op == 20) {
				gp->bmsobj.notes[i].op = 1;
			}
			else {
				if (intArr[lane] == -1) { //case first note of lane
					ErrorLogFmtAdd("追加->%d", lane);
					memcpy(&gp->bmsobj_note[lane].notes[gp->bmsobj_note[lane].size], &gp->bmsobj.notes[i], sizeof(NoteStruct));
					gp->bmsobj_note[lane].size++;
					intArr[lane] = i;
					if (chnValid == 0) noteCount[isBattle]++;
				}
				else if (gp->bmsobj.notes[i].realTiming > gp->bmsobj.notes[intArr[lane]].realTiming_ln) {
					if (gp->bmsobj.notes[i].realTiming != gp->bmsobj.notes[intArr[lane]].realTiming) {
						ErrorLogFmtAdd("追加->%d", lane);
						memcpy(&gp->bmsobj_note[lane].notes[gp->bmsobj_note[lane].size], &gp->bmsobj.notes[i], sizeof(NoteStruct));
						gp->bmsobj_note[lane].size++;
						intArr[lane] = i;
						if (chnValid == 0) noteCount[isBattle]++;
					}

					else if (gp->bmsobj.notes[i].realTiming < gp->bmsobj.notes[i].realTiming_ln) { //case single note and longnote start conflicts, longnote overwrites it
						noteCount[isBattle]--;
						gp->bmsobj_note[lane].size--;
						
						memcpy(&gp->bmsobj_note[lane].notes[gp->bmsobj_note[lane].size], &gp->bmsobj.notes[i], sizeof(NoteStruct));
						gp->bmsobj_note[lane].size++;
						intArr[lane] = i;
						if (chnValid == 0) noteCount[isBattle]++;
					}
				}
				if (gp->bmsobj_note[lane].size == gp->bmsobj_note[lane].count) {
					ExpandNoteBuffer(&gp->bmsobj_note[lane], 100);
				}
			}
		}
	}

	//duplicate notes for battle
	if (cfg->play.battle == 1 && cfg->play.random[0] == cfg->play.random[1] && (meta->keymode == 5 || meta->keymode == 7 || meta->keymode == 9)) {

		for (int i = 0; i < 10; i++) {
			if (gp->bmsobj_note[0 + i].count > gp->bmsobj_note[10 + i].count) {
				ExpandNoteBuffer(&gp->bmsobj_note[10 + i], gp->bmsobj_note[0 + i].count - gp->bmsobj_note[10 + i].count);
			}
			gp->bmsobj_note[10 + i].size = gp->bmsobj_note[0 + i].size;
			for (int j = 0; j < gp->bmsobj_note[0 + i].size; j++) {
				gp->bmsobj_note[10 + i].notes[j].op = gp->bmsobj_note[0 + i].notes[j].op + 10;
				gp->bmsobj_note[10 + i].notes[j].bmsTiming_ln = gp->bmsobj_note[0 + i].notes[j].bmsTiming_ln;
				gp->bmsobj_note[10 + i].notes[j].realTiming_ln = gp->bmsobj_note[0 + i].notes[j].realTiming_ln;
				gp->bmsobj_note[10 + i].notes[j].active = gp->bmsobj_note[0 + i].notes[j].active;
				gp->bmsobj_note[10 + i].notes[j].channel = gp->bmsobj_note[0 + i].notes[j].channel;
				gp->bmsobj_note[10 + i].notes[j].val = gp->bmsobj_note[0 + i].notes[j].val;
				gp->bmsobj_note[10 + i].notes[j].bmsTiming = gp->bmsobj_note[0 + i].notes[j].bmsTiming;
				gp->bmsobj_note[10 + i].notes[j].realTiming = gp->bmsobj_note[0 + i].notes[j].realTiming;
			}
		}
		noteCount[1] = noteCount[0];
	}

	gp->scratchSide = scratchSide;
	if (scratchSide == 1 || scratchSide == 3) {
		memcpy(&tmpLane[3], &gp->bmsobj_note[1], sizeof(LaneStruct));
		memcpy(&tmpLane[4], &gp->bmsobj_note[2], sizeof(LaneStruct));
		memcpy(&tmpLane[5], &gp->bmsobj_note[3], sizeof(LaneStruct));
		memcpy(&tmpLane[6], &gp->bmsobj_note[4], sizeof(LaneStruct));
		memcpy(&tmpLane[7], &gp->bmsobj_note[5], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[1], &gp->bmsobj_note[6], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[2], &gp->bmsobj_note[7], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[3], &tmpLane[3], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[4], &tmpLane[4], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[5], &tmpLane[5], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[6], &tmpLane[6], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[7], &tmpLane[7], sizeof(LaneStruct));
	}
	if (scratchSide == 2 || scratchSide == 3) {
		memcpy(&tmpLane[3], &gp->bmsobj_note[11], sizeof(LaneStruct));
		memcpy(&tmpLane[4], &gp->bmsobj_note[12], sizeof(LaneStruct));
		memcpy(&tmpLane[5], &gp->bmsobj_note[13], sizeof(LaneStruct));
		memcpy(&tmpLane[6], &gp->bmsobj_note[14], sizeof(LaneStruct));
		memcpy(&tmpLane[7], &gp->bmsobj_note[15], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[11], &gp->bmsobj_note[16], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[12], &gp->bmsobj_note[17], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[13], &tmpLane[3], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[14], &tmpLane[4], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[15], &tmpLane[5], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[16], &tmpLane[6], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[17], &tmpLane[7], sizeof(LaneStruct));
	}
	if (is5key) {
		memcpy(&tmpLane[3], &gp->bmsobj_note[1], sizeof(LaneStruct));
		memcpy(&tmpLane[4], &gp->bmsobj_note[2], sizeof(LaneStruct));
		memcpy(&tmpLane[5], &gp->bmsobj_note[3], sizeof(LaneStruct));
		memcpy(&tmpLane[6], &gp->bmsobj_note[4], sizeof(LaneStruct));
		memcpy(&tmpLane[7], &gp->bmsobj_note[5], sizeof(LaneStruct));
		memcpy(&tmpLane[8], &gp->bmsobj_note[6], sizeof(LaneStruct));
		memcpy(&tmpLane[9], &gp->bmsobj_note[0], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[0], &gp->bmsobj_note[9], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[1], &gp->bmsobj_note[7], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[2], &gp->bmsobj_note[8], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[3], &tmpLane[3], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[4], &tmpLane[4], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[5], &tmpLane[5], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[6], &tmpLane[6], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[7], &tmpLane[7], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[8], &tmpLane[8], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[9], &tmpLane[9], sizeof(LaneStruct));
	}
	if (is7key && !isDSC) {
		memcpy(&tmpLane[1], &gp->bmsobj_note[1], sizeof(LaneStruct));
		memcpy(&tmpLane[2], &gp->bmsobj_note[2], sizeof(LaneStruct));
		memcpy(&tmpLane[3], &gp->bmsobj_note[3], sizeof(LaneStruct));
		memcpy(&tmpLane[4], &gp->bmsobj_note[4], sizeof(LaneStruct));
		memcpy(&tmpLane[5], &gp->bmsobj_note[5], sizeof(LaneStruct));
		memcpy(&tmpLane[6], &gp->bmsobj_note[6], sizeof(LaneStruct));
		memcpy(&tmpLane[7], &gp->bmsobj_note[7], sizeof(LaneStruct));
		memcpy(&tmpLane[8], &gp->bmsobj_note[8], sizeof(LaneStruct));
		memcpy(&tmpLane[9], &gp->bmsobj_note[0], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[0], &gp->bmsobj_note[9], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[1], &tmpLane[1], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[2], &tmpLane[2], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[3], &tmpLane[3], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[4], &tmpLane[4], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[5], &tmpLane[5], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[6], &tmpLane[6], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[7], &tmpLane[7], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[8], &tmpLane[8], sizeof(LaneStruct));
		memcpy(&gp->bmsobj_note[9], &tmpLane[9], sizeof(LaneStruct));
	}

	//set judgetime and judgedamage
	for (int p = 0; p < 2; p++) {
		if (total[p] < 1) {
			int notes = noteCount[p];
			double newtotalCalc;
			if (notes < 400) {
				newtotalCalc = notes / 5.0 + 200.0;
			}
			else if (notes < 600) {
				newtotalCalc = (notes - 400) / 2.5 + 280.0;
			}
			else {
				newtotalCalc = (notes - 600) / 5.0 + 360.0;
			}
			total[p] = newtotalCalc * 0.8;
		}
	}
	for (int p = 0; p < 2; p++) {
		double dmg_notebase;
		int notes = noteCount[p];
		if (notes) {
			switch (rank) {
			case 0:
				gp->player[p].judgetime[5] = 8;
				gp->player[p].judgetime[4] = 24;
				gp->player[p].judgetime[3] = 40;
				break;
			case 1:
				gp->player[p].judgetime[5] = 15;
				gp->player[p].judgetime[4] = 30;
				gp->player[p].judgetime[3] = 60;
				break;
			default:
				gp->player[p].judgetime[5] = 18;
				gp->player[p].judgetime[4] = 40;
				gp->player[p].judgetime[3] = 100;
				break;
			case 3:
				gp->player[p].judgetime[5] = 21;
				gp->player[p].judgetime[4] = 60;
				gp->player[p].judgetime[3] = 120;
				break;

			}
			gp->player[p].judgetime[2] = 200;
			gp->player[p].judgetime[1] = 1000;

			if (cfg->play.m_gambol == 1) {
				gp->player[p].judgetime[5] = 8;
				gp->player[p].judgetime[4] = 24;
				gp->player[p].judgetime[3] = 40;
				gp->player[p].judgetime[2] = 200;
				gp->player[p].judgetime[1] = 1000;
			}
			else  if (cfg->play.m_gambol == 2) {
				gp->player[p].judgetime[5] = 8;
				gp->player[p].judgetime[4] = 12;
				gp->player[p].judgetime[3] = 12;
				gp->player[p].judgetime[2] = 200;
				gp->player[p].judgetime[1] = 1000;
			}

			gp->player[p].totalnotes = notes;

			//TODO : check damage
			if (notes < 20)
				dmg_notebase = 10.0;
			else if (notes < 30)
				dmg_notebase = 10.0 - (notes - 20.0) / 10.0 * 2.0;
			else if (notes < 45)
				dmg_notebase = 7.0 - (notes - 30.0) / 15.0;
			else if (notes < 60)
				dmg_notebase = 6.0 - (notes - 45.0) / 15.0;
			else if (notes < 125)
				dmg_notebase = 5.0 - (notes - 60.0) / 65.0;
			else if (notes < 250)
				dmg_notebase = 4.0 - (notes - 125.0) / 125.0;
			else if (notes < 500)
				dmg_notebase = 3.0 - (notes - 250.0) / 250.0;
			else if (notes < 1000)
				dmg_notebase = 2.0 - (notes - 500.0) / 500.0;
			else
				dmg_notebase = 1;
			//TODO: recheck twice below.
			int recover = (total[p] - 80.0)*0.125 / 2;
			if (recover <= 0) recover = 1;

			double dmg_totalbase = 100.0 / (double)recover;
			double dmg = max(dmg_notebase * 10, dmg_totalbase) / 10.0;

			if (!gp->is2Pplay) {
				switch (cfg->play.gaugeOption[p]) {
				default:
					gp->player[p].judge_damage[5] = total[p] / (float)notes;
					gp->player[p].judge_damage[4] = total[p] / (float)notes;
					gp->player[p].judge_damage[3] = total[p] / (float)(notes + notes);
					gp->player[p].judge_damage[2] = -4.0;
					gp->player[p].judge_damage[1] = -6.0;
					gp->player[p].judge_damage[0] = -2.0;
					break;
				case 1:
					gp->player[p].judge_damage[5] = 0.1;
					gp->player[p].judge_damage[4] = 0.1;
					gp->player[p].judge_damage[3] = 0.05;
					gp->player[p].judge_damage[2] = dmg * (-6.0);
					gp->player[p].judge_damage[1] = dmg * (-10.0);
					gp->player[p].judge_damage[0] = dmg * (-2.0);
					break;
				case 2:
					gp->player[p].judge_damage[5] = 0.0;
					gp->player[p].judge_damage[4] = 0.0;
					gp->player[p].judge_damage[3] = 0.0;
					gp->player[p].judge_damage[2] = -100.0;
					gp->player[p].judge_damage[1] = -100.0;
					gp->player[p].judge_damage[0] = 0.0;
					break;
				case 3:
					gp->player[p].judge_damage[5] = (total[p] / (float)notes) * 1.2;
					gp->player[p].judge_damage[4] = (total[p] / (float)notes) * 1.2;
					gp->player[p].judge_damage[3] = (total[p] / (float)(notes + notes)) * 1.2;
					gp->player[p].judge_damage[2] = -3.2;
					gp->player[p].judge_damage[1] = -4.800000000000001;
					gp->player[p].judge_damage[0] = -1.6;
					break;
				case 4:
					gp->player[p].judge_damage[5] = 0.1;
					gp->player[p].judge_damage[4] = -1.0;
					gp->player[p].judge_damage[3] = -100.0;
					gp->player[p].judge_damage[2] = -100.0;
					gp->player[p].judge_damage[1] = -100.0;
					gp->player[p].judge_damage[0] = -100.0;
					break;
				case 5:
					gp->player[p].judge_damage[5] = dmg * (-10.0);
					gp->player[p].judge_damage[4] = -1.0;
					gp->player[p].judge_damage[3] = 0.1;
					gp->player[p].judge_damage[2] = -6.0;
					gp->player[p].judge_damage[1] = dmg * (-10.0);
					gp->player[p].judge_damage[0] = dmg * (-2.0);
					break;
				}
			}
			else if (gp->courseType == 2) {
				switch (cfg->play.gaugeOption[p]) {
				default:
					gp->player[p].judge_damage[5] = 0.1;
					gp->player[p].judge_damage[4] = 0.1;
					gp->player[p].judge_damage[3] = 0.04;
					gp->player[p].judge_damage[2] = -2.0;
					gp->player[p].judge_damage[1] = -3.0;
					gp->player[p].judge_damage[0] = -2.0;
					break;
				case 1:
					gp->player[p].judge_damage[5] = 0.1;
					gp->player[p].judge_damage[4] = 0.1;
					gp->player[p].judge_damage[3] = 0.04;
					gp->player[p].judge_damage[2] = dmg * (-6.0);
					gp->player[p].judge_damage[1] = dmg * (-10.0);
					gp->player[p].judge_damage[0] = dmg * (-2.0);
					break;
				case 2:
					gp->player[p].judge_damage[5] = 0.0;
					gp->player[p].judge_damage[4] = 0.0;
					gp->player[p].judge_damage[3] = 0.0;
					gp->player[p].judge_damage[2] = -100.0;
					gp->player[p].judge_damage[1] = -100.0;
					gp->player[p].judge_damage[0] = 0.0;
					break;
				case 4:
					gp->player[p].judge_damage[5] = 0.1;
					gp->player[p].judge_damage[4] = -1.0;
					gp->player[p].judge_damage[3] = -100.0;
					gp->player[p].judge_damage[2] = -100.0;
					gp->player[p].judge_damage[1] = -100.0;
					gp->player[p].judge_damage[0] = -100.0;
					break;
				case 5:
					gp->player[p].judge_damage[5] = dmg * (-10.0);
					gp->player[p].judge_damage[4] = -1.0;
					gp->player[p].judge_damage[3] = 0.1;
					gp->player[p].judge_damage[2] = -6.0;
					gp->player[p].judge_damage[1] = dmg * (-10.0);
					gp->player[p].judge_damage[0] = dmg * (-2.0);
					break;
				}
			}
			else {
				switch (cfg->play.gaugeOption[p]) {
				default:
					gp->player[p].judge_damage[5] = 0.1;
					gp->player[p].judge_damage[4] = 0.1;
					gp->player[p].judge_damage[3] = 0.04;
					gp->player[p].judge_damage[2] = -1.5;
					gp->player[p].judge_damage[1] = -2.0;
					gp->player[p].judge_damage[0] = -1.5;
					break;
				case 1:
					gp->player[p].judge_damage[5] = 0.1;
					gp->player[p].judge_damage[4] = 0.1;
					gp->player[p].judge_damage[3] = 0.05;
					gp->player[p].judge_damage[2] = dmg * (-6.0);
					gp->player[p].judge_damage[1] = dmg * (-10.0);
					gp->player[p].judge_damage[0] = dmg * (-2.0);
					break;
				case 2:
					gp->player[p].judge_damage[5] = 0.0;
					gp->player[p].judge_damage[4] = 0.0;
					gp->player[p].judge_damage[3] = 0.0;
					gp->player[p].judge_damage[2] = -100.0;
					gp->player[p].judge_damage[1] = -100.0;
					gp->player[p].judge_damage[0] = 0.0;
					break;
				case 3:
					gp->player[p].judge_damage[5] = 0.12;
					gp->player[p].judge_damage[4] = 0.12;
					gp->player[p].judge_damage[3] = 0.048;
					gp->player[p].judge_damage[2] = -1.2;
					gp->player[p].judge_damage[1] = -1.6;
					gp->player[p].judge_damage[0] = -1.2;
					break;
				case 4:
					gp->player[p].judge_damage[5] = 0.1;
					gp->player[p].judge_damage[4] = -1.0;
					gp->player[p].judge_damage[3] = -100.0;
					gp->player[p].judge_damage[2] = -100.0;
					gp->player[p].judge_damage[1] = -100.0;
					gp->player[p].judge_damage[0] = -100.0;
					break;
				case 5:
					gp->player[p].judge_damage[5] = dmg * (-10.0);
					gp->player[p].judge_damage[4] = -1.0;
					gp->player[p].judge_damage[3] = 0.1;
					gp->player[p].judge_damage[2] = -6.0;
					gp->player[p].judge_damage[1] = dmg * (-10.0);
					gp->player[p].judge_damage[0] = dmg * (-2.0);
					break;
				}
			}
		}
	}

	//set first lane keysound
	for (int i = 0; i < 20; i++) {
		if (gp->bmsobj_note[i].size <= 0) gp->bmsobj_note[i].noteVal = -1;
		else gp->bmsobj_note[i].noteVal = gp->bmsobj_note[i].notes[0].val;
	}

	//add notes for maniac options
	if (cfg->play.m_addlong > 0) {
		for (int i = 0; i < 20; i++) {
			if (i != 0 && i != 10) {
				for (int j = 0; j < gp->bmsobj_note[i].size - 1; j++) {
					if (GetRand(100) < cfg->play.m_addlong) {
						double e = (gp->bmsobj_note[i].notes[j].realTiming + gp->bmsobj_note[i].notes[j + 1].realTiming) * 0.5;
						if (gp->bmsobj_note[i].notes[j].realTiming_ln < e) {
							gp->bmsobj_note[i].notes[j].realTiming_ln = e;
							gp->bmsobj_note[i].notes[j].bmsTiming_ln = RealTimeToBMSTime(gp, gp->bmsobj_note[i].notes[j].realTiming_ln);
						}
					}
				}
				if (gp->bmsobj_note[i].size > 0 && cfg->play.m_addlong == 100) {
					gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size - 1].realTiming_ln = endtime;
					gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size - 1].bmsTiming_ln = RealTimeToBMSTime(gp, gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size - 1].realTiming_ln);
				}
				gp->bpmt_start = 0;
			}
		}
		endtime += 2000.0;
	}

	if (cfg->play.m_addmine > 0) {
		for (int i = 0; i < 20; i++) {
			if (i != 0 && i != 10) {
				int s = gp->bmsobj_note[i].size;
				for (int j = 0; j < gp->bmsobj_note[i].size - 1; j++) {
					if (GetRand(100) < cfg->play.m_addmine) {
						if (gp->bmsobj_note[i].size == gp->bmsobj_note[i].count) {
							ExpandNoteBuffer(&gp->bmsobj_note[i], 1000);
						}
						gp->bmsobj_note[i].notes;
						gp->bmsobj_note[i].notes[j + 1].realTiming_ln;
						gp->bmsobj_note[i].notes[j + 1].realTiming;

						if (gp->bmsobj_note[i].notes[j + 1].realTiming_ln > gp->bmsobj_note[i].notes[j + 1].realTiming) {
							if (200.0 < gp->bmsobj_note[i].notes[j + 1].realTiming - gp->bmsobj_note[i].notes[j].realTiming_ln) {
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].realTiming = (gp->bmsobj_note[i].notes[j + 1].realTiming + gp->bmsobj_note[i].notes[j].realTiming_ln) * 0.5;
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].bmsTiming = RealTimeToBMSTime(gp, gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].realTiming);
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].val = 0.0;
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].channel = 4;
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].op = gp->bmsobj_note[i].notes[j + 1].op;
								gp->bmsobj_note[i].size++;
							}
						}
						else if (200.0 < gp->bmsobj_note[i].notes[j + 1].realTiming - gp->bmsobj_note[i].notes[j].realTiming) {
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].realTiming = (gp->bmsobj_note[i].notes[j + 1].realTiming + gp->bmsobj_note[i].notes[j].realTiming) * 0.5;
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].bmsTiming = RealTimeToBMSTime(gp, gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].realTiming);
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].val = 0.0;
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].channel = 4;
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].size].op = gp->bmsobj_note[i].notes[j + 1].op;
							gp->bmsobj_note[i].size++;
						}
					}
				}
				gp->bpmt_start = 0;
				if (s > 1) {
					qsort(gp->bmsobj_note[i].notes, gp->bmsobj_note[i].size, sizeof(NoteStruct), CMP_NotesByRealTiming);
				}
			}
		}
	}

	gp->p1Score.InitJudgeQueue();
	gp->p1Score.ResetJudgeQueue(gp->player[0].totalnotes * 2);
	gp->BPM = gp->BPM_fix;
	gp->player[0].total_note += gp->player[0].totalnotes;
	gp->song_runtime = endtime;
	gp->player[1].total_note += gp->player[1].totalnotes;

	if (gp->soundonly == 1 && bgaFlag && (cfg->play.bga == 3 || cfg->play.bga == 1 || (cfg->play.bga == 2 && gp->isAutoplay)) && cfg->play.autojudge != 2) {
		DeleteGraph(gp->bgaHandle[1295]);
		gp->bgaHandle[1295] = -1;
		CSTR defaultMovieFile = GetRandomFile("LR2files\\Movie\\*.mpg", 0);
		if (defaultMovieFile.isDiff("ERROR")) gp->bgaHandle[1295] = LoadGraph(defaultMovieFile);
		if (gp->bgaHandle[1295] == -1) {
			defaultMovieFile = GetRandomFile("LR2files\\Movie\\*.avi", 0);
			if (defaultMovieFile.isDiff("ERROR")) gp->bgaHandle[1295] = LoadGraph(defaultMovieFile);
			if (gp->bgaHandle[1295] == -1) {
				defaultMovieFile = GetRandomFile("LR2files\\Movie\\*.wmv", 0);
				if (defaultMovieFile.isDiff("ERROR")) gp->bgaHandle[1295] = LoadGraph(defaultMovieFile);
				if (gp->bgaHandle[1295] == -1) {
					defaultMovieFile = GetRandomFile("LR2files\\Movie\\*.mp4", 0);
					if (defaultMovieFile.isDiff("ERROR")) gp->bgaHandle[1295] = LoadGraph(defaultMovieFile);
					if (gp->bgaHandle[1295] == -1) {
						defaultMovieFile = GetRandomFile("LR2files\\Movie\\*.ogv", 0);
						if (defaultMovieFile.isDiff("ERROR")) gp->bgaHandle[1295] = LoadGraph(defaultMovieFile);
					}
				}
			}
		}
	}
	if (cfg->system.isablebmsthread == 1 && gp->isPreviewLoad == 0) {
		LoadBmsResource(gp, filename, aud, cfg, meta, bgaFlag, scratchSide, 0);
	}
	if (gp->songStartTime[0] <= 0 || gp->songEndTime[0] <= 0) {
		aud->param.stageBgmVolume[0] = 1.0;
		aud->param.stageKeyVolume[0] = 1.0;
	}
	if (gp->songStartTime[1] <= 0 || gp->songEndTime[1] <= 0) {
		aud->param.stageBgmVolume[1] = 1.0;
		aud->param.stageKeyVolume[1] = 1.0;
	}
	if (gp->songStartTime[2] <= 0 || gp->songEndTime[2] <= 0) {
		aud->param.stageBgmVolume[2] = 1.0;
		aud->param.stageKeyVolume[2] = 1.0;
	}
	if (gp->songStartTime[3] <= 0 || gp->songEndTime[3] <= 0) {
		aud->param.stageBgmVolume[3] = 1.0;
		aud->param.stageKeyVolume[3] = 1.0;
	}
	if (gp->songStartTime[4] <= 0 || gp->songEndTime[4] <= 0) {
		aud->param.stageBgmVolume[4] = 1.0;
		aud->param.stageKeyVolume[4] = 1.0;
	}
	ApplySoundFX(aud, 1, cfg->sound.disabledsp);
	ErrorLogAdd("BMSを読み込みました。\n");
	return 1;
}
#include "Scene06_Keyconfig.h"
#include "LR2_configsave.h"
#include "LR2_skinobject.h"

//401da0
int ProcS_Keyconfig(game *g) {
	CSTR t("-",0);
	for (int i = 0; i < 8; i++) {
		SetObjectString(40 + i, t, g->txtStruct.objectStr);
	}
	if (g->KeyInput.config_button_inMap > 0) {
		for (int i = 0; i < 16; i++) {
			t = GetKeyIDname(g->config.input.buttonMap[g->KeyInput.config_button_inMap][i]);
			SetObjectString(40 + i, t, g->txtStruct.objectStr);
		}
	}
	return 1;
}

//409860
int ProcI_Keyconfig(game *g) {
	int fndkey;
	if (g->KeyInput.config_key >= 0 && g->KeyInput.config_button_inMap > 0) {
		fndkey = FindPressedKey(&g->KeyInput);
		//TOFIX : block F1, F12 & allow NUMPAD
		if(fndkey > 0 && (fndkey <= 0x3B || fndkey >= 0x58) && (fndkey != KEY_INPUT_ESCAPE) && (fndkey != KEY_INPUT_RETURN) && (fndkey != KEY_INPUT_UP) && (fndkey != KEY_INPUT_DOWN) && (fndkey != KEY_INPUT_LEFT) && (fndkey != KEY_INPUT_RIGHT)){
			g->config.input.buttonMap[g->KeyInput.config_button_inMap][g->KeyInput.config_key] = fndkey; //TOFIX: fix assign DELETE
			//fndkey != KEY_INPUT_DELETE ? fndKey : 0; 
			PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
			if (g->KeyInput.config_keymode == 0) {
				WriteKeyConfig(g, "LR2files\\Config\\keyconfig.xml", 7);
			}
			else if(g->KeyInput.config_keymode == 1){
				WriteKeyConfig(g, "LR2files\\Config\\keyconfig_p.xml", 9);
			}
			else if (g->KeyInput.config_keymode == 2) {
				WriteKeyConfig(g, "LR2files\\Config\\keyconfig_5.xml", 5);
			}
			ProcS_Keyconfig(g);
		}
	}
	
	if ( g->KeyInput.inputID[KEY_INPUT_DELETE] == 1 && g->KeyInput.config_key >= 0 && g->KeyInput.config_button_inMap > 0) {
		g->config.input.buttonMap [g->KeyInput.config_button_inMap] [g->KeyInput.config_key] = 0;
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		ProcS_Keyconfig(g);
	}
	
	if (g->KeyInput.inputID[KEY_INPUT_F1] == 1) {
		memset(g->config.input.buttonMap, 0, 16 * 40 * sizeof(int));

		if (g->KeyInput.config_keymode == 0) {
			ReadKeyConfig(g, "LR2files\\Config\\keyconfig_def.xml");
			WriteKeyConfig(g, "LR2files\\Config\\keyconfig.xml", 7);
		}
		else if (g->KeyInput.config_keymode == 1) {
			ReadKeyConfig(g, "LR2files\\Config\\keyconfig_p_def.xml");
			WriteKeyConfig(g, "LR2files\\Config\\keyconfig_p.xml", 9);
		}
		else if (g->KeyInput.config_keymode == 2) {
			ReadKeyConfig(g, "LR2files\\Config\\keyconfig_5_def.xml");
			WriteKeyConfig(g, "LR2files\\Config\\keyconfig_5.xml", 5);
		}
		ProcS_Keyconfig(g);
	}

	if (g->KeyInput.inputID[KEY_INPUT_F2] == 1) {
		memset(g->config.input.buttonMap, 0, 16 * 40 * sizeof(int));
		ProcS_Keyconfig(g);
	}
	if (((g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1) || (g->KeyInput.mouse_buttonR == 1)) && (g->procPhase == 1)) {
		SetTimeLapse(2, &g->timer1);
		g->procPhase = 2;
		return 1;
	}
	if (g->procPhase == 2) {
		if ( g->skstruct.fadeout < GetTimeLapse(2, &g->timer1) || g->skstruct.fadeout == 0 ) {
			g->procSelecter = 2;
		}
	}
	return 1;
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

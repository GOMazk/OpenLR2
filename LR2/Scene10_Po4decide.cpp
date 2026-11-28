#include "Scene10_Po4decide.h"
#include "LR2.h"

int ProcB_PO4Decide(game* g) {
	LoadSceneG(g, &g->skstruct, SKINTYPE_MODEDECIDE);

	return 1;
}

int ProcI_PO4Decide(game *g) {

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.scenetime && g->procPhase == 1) {
		g->procSelecter = SCENE_PLAY;
		return 1;
	}

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start && (g->KeyInput.inputID[KEY_INPUT_RETURN] == 1 || g->KeyInput.mouse_buttonL == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[7] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[6] == 1) && g->procPhase == 1) {
		SetTimeLapse(3, &g->timer1);
		g->procPhase = 3;
		return 1;
	}

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start && (g->KeyInput.mouse_buttonR == 1 || g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1 || (g->KeyInput.p1_buttonInput[12] == 2 && g->KeyInput.p1_buttonInput[13] == 2) || (g->KeyInput.p2_buttonInput[12] == 2 && g->KeyInput.p2_buttonInput[13] == 2)) && g->procPhase == 1) {
		SetTimeLapse(2, &g->timer1);
		g->procPhase = 2;
		StopSysSound(g);
		return 1;
	}

	if (g->procPhase == 2) {
		if (GetTimeLapse(2, &g->timer1) > g->skstruct.fadeout || g->skstruct.fadeout == 0) {
			g->procSelecter = SCENE_PO4MENU;
			return 1;
		}
	}
	else if (g->procPhase == 3) {
		if (GetTimeLapse(3, &g->timer1) > g->skstruct.close || g->skstruct.close == 0) {
			g->procSelecter = SCENE_SELECT;
			return 1;
		}
	}
	return 1;
}

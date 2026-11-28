#include "Scene03_Decide.h"
#include "Engine.h"
#include "LR2_audio.h"
#include "LR2_skinload.h"
#include "LR2_scene.h"

int ProcS_Decide(game* g) {

	DeleteGraph(g->skstruct.GrHandle[GRHTYPE_STAGE]);
	g->skstruct.GrHandle[GRHTYPE_STAGE] = -1;
	DeleteGraph(g->skstruct.GrHandle[GRHTYPE_BACKBMP]);
	g->skstruct.GrHandle[GRHTYPE_BACKBMP] = -1;
	DeleteGraph(g->skstruct.GrHandle[GRHTYPE_BANNER]);
	g->skstruct.GrHandle[GRHTYPE_BANNER] = -1;

	SetTransColor(0, 255, 0);
	CSTR dir(g->sSelect.bmsList[g->sSelect.cur_song].filepath.getDirectory());
	if (g->sSelect.bmsList[g->sSelect.cur_song].isStagefile) {
		CSTR oBuf;
		if (FindAltImage(g->sSelect.bmsList[g->sSelect.cur_song].stagefile, dir, &oBuf) != 1)
			g->sSelect.bmsList[g->sSelect.cur_song].isStagefile = false;
		g->skstruct.GrHandle[GRHTYPE_STAGE] = LoadGraph(oBuf, 0);
		if (g->skstruct.GrHandle[GRHTYPE_STAGE] == -1) g->sSelect.bmsList[g->sSelect.cur_song].isStagefile = false;
	}
	if (g->sSelect.bmsList[g->sSelect.cur_song].isBackBMP) {
		CSTR oBuf;
		if (FindAltImage(g->sSelect.bmsList[g->sSelect.cur_song].backBMP, dir, &oBuf) != 1)
			g->sSelect.bmsList[g->sSelect.cur_song].isBackBMP = false;
		g->skstruct.GrHandle[GRHTYPE_BACKBMP] = LoadGraph(oBuf, 0);
		if (g->skstruct.GrHandle[GRHTYPE_BACKBMP] == -1) g->sSelect.bmsList[g->sSelect.cur_song].isBackBMP = false;
	}
	if (g->sSelect.bmsList[g->sSelect.cur_song].isBanner) {
		CSTR oBuf;
		if (FindAltImage(g->sSelect.bmsList[g->sSelect.cur_song].banner, dir, &oBuf) != 1)
			g->sSelect.bmsList[g->sSelect.cur_song].isBanner = false;
		g->skstruct.GrHandle[GRHTYPE_BANNER] = LoadGraph(oBuf, 0); //TOFIX : when banner size is not 300 80, this will break graph size and banner is not displayed until next song
		if (g->skstruct.GrHandle[GRHTYPE_BANNER] == -1) g->sSelect.bmsList[g->sSelect.cur_song].isBanner = false;
	}
	SetTransColor(0, 255, 0);


	LoadSceneG(g, &g->skstruct, SKINTYPE_DECIDE);


	StopSysSound(g);
	if (g->config.play.m_isExtra && g->audio.sysSound.exdecide.load)
		PlaySound(&g->audio, &g->audio.sysSound.exdecide, g->audio.chnBgm, -1);
	else
		PlaySound(&g->audio, &g->audio.sysSound.decide, g->audio.chnBgm, -1);

	return 1;
}

int ProcI_Decide(game* g) {

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
			g->procSelecter = SCENE_SELECT;
			return 1;
		}
	}
	else if (g->procPhase == 3) {
		if (GetTimeLapse(3, &g->timer1) > g->skstruct.close || g->skstruct.close == 0) {
			g->procSelecter = SCENE_PLAY;
			return 1;
		}
	}
	return 1;
}

int ProcE_Decide(game* g) {

	g->config.play.randSC[PLAYER_1] = 0;
	g->config.play.randSC[PLAYER_2] = 0;
	g->config.play.randFix[PLAYER_1] = 0;
	g->config.play.randFix[PLAYER_2] = 0;
	InitInputStructure2(&g->KeyInput);
	InputToButton(&g->KeyInput, &g->config.input, (int)(uint)(g->sSelect.metaSelected.keymode < 10), 0);
	if (g->KeyInput.p1_buttonInput[12] == 1 && g->KeyInput.p1_buttonInput[13] == 1) {
		g->config.play.randSC[PLAYER_1] = 1;
	}
	else if (g->KeyInput.p2_buttonInput[12] == 1 && g->KeyInput.p2_buttonInput[13] == 1) {
		g->config.play.randSC[PLAYER_2] = 1;
	}
	else if (g->KeyInput.p1_buttonInput[12] == 1 || g->KeyInput.p1_buttonInput[13] == 1 || g->KeyInput.p2_buttonInput[12] == 1 || g->KeyInput.p2_buttonInput[13] == 1) {
		for (int i = 1; i <= 9; i++) {
			if (g->KeyInput.p1_buttonInput[i] == 1) {
				g->config.play.randFix[PLAYER_1] = i;
			}
			if (g->KeyInput.p2_buttonInput[i] == 1) {
				g->config.play.randFix[PLAYER_2] = i;
			}
		}
	}

	return 1;
}
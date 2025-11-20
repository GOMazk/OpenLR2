#include "Scene07_Skinselect.h"
#include "LR2.h"
#include "Scenes.h"

//4198d0
int SkinSelect_SoundSet(game *g, CSTR filepath) {
	
	FILE *pFile;

	CSTR fBuf(1024);
	char* pFbuf;
	CSVbuf csv;

	StopSysSound(g);
	ReleaseSound(&g->audio, &g->audio.sysSound.select);
	ReleaseSound(&g->audio, &g->audio.sysSound.folder_open);
	ReleaseSound(&g->audio, &g->audio.sysSound.folder_close);
	ReleaseSound(&g->audio, &g->audio.sysSound.panel_open);
	ReleaseSound(&g->audio, &g->audio.sysSound.option_change);
	ReleaseSound(&g->audio, &g->audio.sysSound.decide);
	ReleaseSound(&g->audio, &g->audio.sysSound.clear);
	ReleaseSound(&g->audio, &g->audio.sysSound.fail);
	ReleaseSound(&g->audio, &g->audio.sysSound.stop);
	ReleaseSound(&g->audio, &g->audio.sysSound.mine);
	ReleaseSound(&g->audio, &g->audio.sysSound.exdecide);
	ReleaseSound(&g->audio, &g->audio.sysSound.exselect);
	if (g->audio.is_fmod_disabled == 0) FMOD_System_Update(g->audio.fmodSys);
	
	pFile = fopen(filepath, "r");
	if (pFile == 0) return 0;

	pFbuf = fBuf.outstr();
	for (pFbuf = fgets(pFbuf, 1023, pFile); pFbuf; pFbuf = fgets(pFbuf, 1023, pFile)) {
		if (*fBuf.atPos(0) = '#') {
			fBuf.trimWhiteSpace();
			DealWhiteSpace(&fBuf);
			if (isdigit(*fBuf.atPos(1)) && isdigit(*fBuf.atPos(2))) {
				
				int type = atol(fBuf.getSliced(1, 2));
				if (type < 0) continue;

				SplitCSV(fBuf, &csv, ",");
				g->config.skin.skinFilePath[type] = csv.str[1];
				SetFirstSkin_5k(&g->skinData, (SKINTYPE)type, &g->config.skin.skinFilePath[type]);
			}
			*fBuf.atPos(0) = 0;
		}
	}

	fclose(pFile);
	return 1;
}

//419c00
int ProcI_SkinSelect(game *g) {

	if ((g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1 || g->KeyInput.mouse_buttonR == 1) && g->procPhase == 1) {
		SetTimeLapse(2, &g->timer1);
		g->procPhase = 2;
	}
	else if (g->procPhase == 2) {
		if (GetTimeLapse(2, &g->timer1) > g->skstruct.fadeout|| !g->skstruct.fadeout) {
			g->procSelecter = 2;
		}
	}

	if (g->KeyInput.mousewheel <= -1) {
		g->skinData.previewCustomID++;
		if (g->skinData.previewCustomID >= g->skinData.Data[g->skinData.previewID].custom_count - 5) g->skinData.previewCustomID = g->skinData.Data[g->skinData.previewID].custom_count - 5;
		if (g->skinData.previewCustomID < 0) g->skinData.previewCustomID = 0;

		SetObjectString(50, g->skinData.Data[g->skinData.previewID].title, g->txtStruct.objectStr);
		SetObjectString(51, g->skinData.Data[g->skinData.previewID].maker, g->txtStruct.objectStr);
		SetObjectString(50, g->skinData.Data[g->skinData.previewID].title, g->txtStruct.objectStr);
		SetObjectString(51, g->skinData.Data[g->skinData.previewID].maker, g->txtStruct.objectStr);

		for (int i = 0; i < 5; i++) {
			SetObjectString(100 + i, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + i].title, g->txtStruct.objectStr);
			SetObjectString(110 + i, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + i].op_label[g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + i].dst_op_selected - g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + i].dst_op_start], g->txtStruct.objectStr);
		}
	}
	else if (g->KeyInput.mousewheel >= 1) {
		g->skinData.previewCustomID--;
		if (g->skinData.previewCustomID < 0) g->skinData.previewCustomID = 0;

		SetObjectString(50, g->skinData.Data[g->skinData.previewID].title, g->txtStruct.objectStr);
		SetObjectString(51, g->skinData.Data[g->skinData.previewID].maker, g->txtStruct.objectStr);
		SetObjectString(50, g->skinData.Data[g->skinData.previewID].title, g->txtStruct.objectStr);
		SetObjectString(51, g->skinData.Data[g->skinData.previewID].maker, g->txtStruct.objectStr);

		for (int i = 0; i < 5; i++) {
			SetObjectString(100 + i, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + i].title, g->txtStruct.objectStr);
			SetObjectString(110 + i, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + i].op_label[g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + i].dst_op_selected - g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + i].dst_op_start], g->txtStruct.objectStr);
		}
	}

	if (g->config.system.disableskinpreview == 1 || g->skinData.Data[g->skinData.previewID].type == SKINTYPE_SOUNDSET) {
		return 0;
	}

	if (GetTimeLapse(0, &g->timer2) > g->skstruct2.startinput_start && GetTimeLapse(1, &g->timer2) == -1.0){
		SetTimeLapse(1, &g->timer2);
		SetTimeLapse(11, &g->timer2);
	}

	if (g->skinData.select <= 4 || g->skinData.select == 12 || g->skinData.select == 13) {
		if (GetTimeLapse(40, &g->timer2) < 0 && GetTimeLapse(0, &g->timer2) > g->skstruct2.loadend + g->skstruct2.loadstart)
			SetTimeLapse(40, &g->timer2);
		if (GetTimeLapse(41, &g->timer2) < 0 && GetTimeLapse(40, &g->timer2) > g->skstruct2.playstart)
			SetTimeLapse(41, &g->timer2);

		if (GetTimeLapse(48, &g->timer2) < 0 && (g->gameplay.player[0].totalnotes == g->gameplay.player[0].note_current))
			SetTimeLapse(48, &g->timer2);
		if (GetTimeLapse(49, &g->timer2) < 0 && (g->gameplay.player[1].totalnotes == g->gameplay.player[1].note_current))
			SetTimeLapse(49, &g->timer2);
	}
	if (g->skinData.select == 7) {
		if (GetTimeLapse(0, &g->timer2) > g->skstruct2.startinput_rank && GetTimeLapse(151, &g->timer2) == -1.0) {
			SetTimeLapse(151, &g->timer2);
			g->skstruct2.src_GAUGECHART_1P[0].op4 = GetTimeLapse(0, &g->timer2);
			g->skstruct2.src_GAUGECHART_2P[0].op4 = GetTimeLapse(0, &g->timer2);
			g->skstruct2.src_SCORECHART[0].op4 = GetTimeLapse(0, &g->timer2);
			g->skstruct2.src_SCORECHART[1].op4 = GetTimeLapse(0, &g->timer2);
			g->skstruct2.src_SCORECHART[2].op4 = GetTimeLapse(0, &g->timer2);
		}
	}
	
	for (int i = 0; i < g->skstruct2.image.srcSize; i++) {
		if (GetOptionFlag_dst(g, g->skstruct2.image.dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct2.image.dst[i].opt2) && GetOptionFlag_dst(g, g->skstruct2.image.dst[i].opt3)) {
			AddDrawingBuffer_Image2(&g->skstruct2.drBuf, &g->skstruct2.image.src[i], &g->skstruct2.image.dst[i], &g->timer2);
		}
	}

	for (int i = 0; i < g->skstruct2.otherObject[1].srcSize; i++) {
		if (GetOptionFlag_dst(g, g->skstruct2.otherObject[1].dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct2.otherObject[1].dst[i].opt2) && GetOptionFlag_dst(g, g->skstruct2.otherObject[1].dst[i].opt3)) {
			AddDrawingBuffer_Image(&g->skstruct2.drBuf, &g->skstruct2.otherObject[1].src[i], &g->skstruct2.otherObject[1].dst[i], &g->timer2);
		}
	}
	for (int i = 0; i < g->skstruct2.otherObject[2].srcSize; i++) {
		if (GetOptionFlag_dst(g, g->skstruct2.otherObject[2].dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct2.otherObject[2].dst[i].opt2) && GetOptionFlag_dst(g, g->skstruct2.otherObject[2].dst[i].opt3)) {
			AddDrawingBuffer_Slider (&g->skstruct2.drBuf, &g->skstruct2.otherObject[2].src[i], &g->skstruct2.otherObject[2].dst[i], &g->timer2);
		}
	}
	for (int i = 0; i < g->skstruct2.otherObject[6].srcSize; i++) {
		if (GetOptionFlag_dst(g, g->skstruct2.otherObject[6].dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct2.otherObject[6].dst[i].opt2) && GetOptionFlag_dst(g, g->skstruct2.otherObject[6].dst[i].opt3)) {
			AddDrawingBuffer_Numbers(&g->skstruct2.drBuf, &g->skstruct2.otherObject[6].src[i], &g->skstruct2.otherObject[6].dst[i], &g->timer2,SetObjectValue_Num(g,g->skstruct2.otherObject[6].src[i].op1),0,0);
		}
	}
	for (int i = 0; i < g->skstruct2.otherObject[3].srcSize; i++) {
		if (GetOptionFlag_dst(g, g->skstruct2.otherObject[3].dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct2.otherObject[3].dst[i].opt2) && GetOptionFlag_dst(g, g->skstruct2.otherObject[3].dst[i].opt3)) {
			AddDrawingBuffer_OnMouse(&g->skstruct2.drBuf, &g->skstruct2.otherObject[3].src[i], &g->skstruct2.otherObject[3].dst[i], &g->timer2, &g->KeyInput, g->sSelect.panel);
		}
	}

	if (GetTimeWrap() < g->KeyInput.mouse_recentMoveTime + 5000) {
		AddDrawingBuffer_Object(&g->skstruct2.drBuf, &g->skstruct2.src_MOUSECURSOR, &(g->skstruct2).dst_MOUSECURSOR, &g->timer2, g->KeyInput.mouse_oldX, g->KeyInput.mouse_oldY); 
	}
	else if (GetTimeWrap() < g->KeyInput.mouse_recentMoveTime + 5500) {
		AddDrawingBuffer_ObjectAlpha(&g->skstruct2.drBuf, &g->skstruct2.src_MOUSECURSOR, &g->skstruct2.dst_MOUSECURSOR, &g->timer2, g->KeyInput.mouse_oldX, g->KeyInput.mouse_oldY, (g->KeyInput.mouse_recentMoveTime - GetTimeWrap() + 5500) * 255 / 500);
	}

	if (g->skinData.select <= 4 || g->skinData.select == 12 || g->skinData.select == 13) {
		for (int i = 0; i < 2; i++) {
			if (g->gameplay.isCourse == 0 && (g->config.play.gaugeOption[i] == 0 || g->config.play.gaugeOption[i] == 3)) 
				AddDrawingBuffer_Gauge(&g->skstruct2.drBuf, &g->skstruct2.src_GROOVEGAUGE[i], &g->skstruct2.dst_GROOVEGAUGE[i], &g->timer2, (int)g->gameplay.player[i].HP_print / 2, 0);
			else
				AddDrawingBuffer_Gauge(&g->skstruct2.drBuf, &g->skstruct2.src_GROOVEGAUGE[i], &g->skstruct2.dst_GROOVEGAUGE[i], &g->timer2, (int)g->gameplay.player[i].HP_print / 2, 1);
		}
		if (GetTimeLapse(41, &g->timer2) > 0.0) {
			DrawNotes(g, &g->skstruct2, &g->timer2, &g->config.play);
			DrawJudgeCombo(g, &g->skstruct2, &g->timer2, &g->config.play);
		}
		if (g->skstruct2.dst_JUDGELINE[0].dstCount > 0) {
			AddDrawingBuffer_PlayArea(&g->skstruct2.drBuf, &g->skstruct2.src_JUDGELINE[0], &g->skstruct2.dst_JUDGELINE[0], &g->timer2, g->skstruct2.adjust.note_1p_x + g->gameplay.nabeatsu_x, g->skstruct2.adjust.note_1p_y + g->gameplay.nabeatsu_y, -1, g->skstruct2.adjust.size_x, g->skstruct2.adjust.size_y, 1);
		}
		if (g->skstruct2.dst_JUDGELINE[1].dstCount > 0) {
			AddDrawingBuffer_PlayArea(&g->skstruct2.drBuf, &g->skstruct2.src_JUDGELINE[1], &g->skstruct2.dst_JUDGELINE[1], &g->timer2, g->skstruct2.adjust.note_2p_x + g->gameplay.nabeatsu_x, g->skstruct2.adjust.note_2p_y + g->gameplay.nabeatsu_y, -1, g->skstruct2.adjust.size_x, g->skstruct2.adjust.size_y, 1);
		}
		if (GetTimeLapse(41, &g->timer2) >= 0.0 && g->config.play.bga) {
			for (int i = 0; i < g->skstruct2.otherObject[4].srcSize; i++) {
				if (g->gameplay.bgaLayer1 >= 0 && GetOptionFlag_dst(g, g->skstruct2.otherObject[4].dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct2.otherObject[4].dst[i].opt2) && GetOptionFlag_dst(g, g->skstruct2.otherObject[4].dst[i].opt3)) {
					AddDrawingBuffer_BGA(&g->skstruct2.drBuf, &g->skstruct2.otherObject[4].src[i], &g->skstruct2.otherObject[4].dst[i], &g->timer2, g->gameplay.bgaHandle[g->gameplay.bgaLayer1],1);
				}
			}
		}
	}
	
	if (g->skinData.select == 5) {
		for (int i = 1; i < 30; i++) {
			if (g->skstruct2.dst_BAR_BODY_OFF[i].dstCount > 0 && g->skstruct2.dst_BAR_BODY_OFF[i - 1].dstCount > 0) {
				DSTdraw dstd1, dstd2, dstd3;
				InitDSTdraw(&dstd1);
				InitDSTdraw(&dstd2);
				InitDSTdraw(&dstd3);

				int bar = g->sSelect.listCalculatedBar / 1000 - g->skstruct2.BAR_CENTER + i;
				while (bar < 0) {
					bar += g->sSelect.bmsListCount;
				}
				int sBar = bar % g->sSelect.bmsListCount;
				
				if (i == g->sSelect.listSelectedBarFromScreenTop) {
					dstd1 = SetDSTdrawByTime(g->skstruct2.dst_BAR_BODY_ON[i], GetTimeLapse(0, &g->timer2));
				}
				else {
					dstd1 = SetDSTdrawByTime(g->skstruct2.dst_BAR_BODY_OFF[i], GetTimeLapse(0, &g->timer2));
				}

				if (i - 1 == g->sSelect.listSelectedBarFromScreenTop) {
					dstd2 = SetDSTdrawByTime(g->skstruct2.dst_BAR_BODY_ON[i-1], GetTimeLapse(0, &g->timer2));
				}
				else {
					dstd2 = SetDSTdrawByTime(g->skstruct2.dst_BAR_BODY_OFF[i-1], GetTimeLapse(0, &g->timer2));
				}

				dstd3 = dstd1;

				if (dstd1.time != -1 && dstd2.time != -1) {
					AddDrawingBuffer_Lunaris(&g->skstruct2.drBuf, &g->skstruct2.src_BAR_BODY[g->sSelect.bmsList[sBar].folderType], &dstd3, &g->timer2);
					
					if (i == g->sSelect.listSelectedBarFromScreenTop) {
						AddDrawingBuffer_Object(&g->skstruct2.drBuf, &g->skstruct2.src_BAR_FLASH, &g->skstruct2.dst_BAR_FLASH, &g->timer2, dstd1.x, dstd1.y);
					}
					
					AddDrawingBuffer_TextXY(&g->skstruct2.drBuf, &g->skstruct2.src_BAR_TITLE[0], &g->skstruct2.dst_BAR_TITLE[0], &g->timer2, sBar + 10000, dstd3.x, dstd3.y);
					
					if (g->sSelect.bmsList[sBar].keymode > 0) {
						if (g->sSelect.bmsList[sBar].difficulty >= 0) {
							AddDrawingBuffer_Numbers(&g->skstruct2.drBuf, &g->skstruct2.src_BAR_LEVEL[g->sSelect.bmsList[sBar].difficulty], &g->skstruct2.dst_BAR_LEVEL[g->sSelect.bmsList[sBar].difficulty], &g->timer2, g->sSelect.bmsList[sBar].level, dstd3.x, dstd3.y);
						}
						else if (g->sSelect.bmsList[sBar].keymode > 0){
							AddDrawingBuffer_Numbers(&g->skstruct2.drBuf, &g->skstruct2.src_BAR_LEVEL[0], &g->skstruct2.dst_BAR_LEVEL[0], &g->timer2, g->sSelect.bmsList[sBar].level, dstd3.x, dstd3.y);
						}
					}
					
					if (g->config.play.battle == 2) {
						AddDrawingBuffer_Object(&g->skstruct2.drBuf, &g->skstruct2.src_BAR_LAMP[g->sSelect.bmsList[sBar].mybest.clear_db], &g->skstruct2.dst_BAR_LAMP[g->sSelect.bmsList[sBar].mybest.clear_db], &g->timer2, dstd3.x, dstd3.y);
					}
					else if (g->config.play.battle == 3) {
						AddDrawingBuffer_Object(&g->skstruct2.drBuf, &g->skstruct2.src_BAR_LAMP[g->sSelect.bmsList[sBar].mybest.clear_sd], &g->skstruct2.dst_BAR_LAMP[g->sSelect.bmsList[sBar].mybest.clear_sd], &g->timer2, dstd3.x, dstd3.y);
					}
					else if (g->config.play.is_extra == 1) {
						AddDrawingBuffer_Object(&g->skstruct2.drBuf, &g->skstruct2.src_BAR_LAMP[g->sSelect.bmsList[sBar].mybest.clear_ex], &g->skstruct2.dst_BAR_LAMP[g->sSelect.bmsList[sBar].mybest.clear_ex], &g->timer2, dstd3.x, dstd3.y);
					}
					else {
						AddDrawingBuffer_Object(&g->skstruct2.drBuf, &g->skstruct2.src_BAR_LAMP[g->sSelect.bmsList[sBar].mybest.clear], &g->skstruct2.dst_BAR_LAMP[g->sSelect.bmsList[sBar].mybest.clear], &g->timer2, dstd3.x, dstd3.y);
					}
				}
			}
		}
	}
	if (g->skinData.select == 7) {
		Proc_Result(g, &g->skstruct2, &g->timer2);
	}

	for (int i = 0; i < g->skstruct2.drBuf.count; i++) {
		LRDraw(&g->skstruct2.drBuf, &g->txtStruct, &g->sSelect, &g->skstruct2, i, 0, 0);
	}

	ScreenCapture(g->skstruct.GrHandle[105]);
	clsDx();
	InitDrawingBuffer(&g->skstruct2.drBuf);
	return 1;
}

//41aff0
int MakeSkinPreview(game *g, skstruct *sk, SkinManage *sm) {
	//disable
	if (g->config.system.disableskinpreview == 1 || sm->Data[sm->previewID].type == SKINTYPE_SOUNDSET) {
		int grh = LoadGraph(sm->Data[sm->previewID].thumbnail, 0);
		if (grh != -1) {
			DrawExtendGraph(0, 0, 640, 480, grh, 0);
			ScreenCapture(g->skstruct.GrHandle[105]);
			DeleteGraph(grh);
		}
		return 0;
	}
	//enable
	for (int i = 0; i < 900; i++) {
		g->skstruct.op[i] = (GetOptionFlag_dst(g, i) > 0);
		g->skstruct2.op[i] = (GetOptionFlag_dst(g, i) > 0);
	}
	for (int i = 0; i < 100; i++) {
		g->skstruct.op[900 + i] = 0;
		g->skstruct2.op[900 + i] = 0;
	}
	DeleteGraph(sk->GrHandle[100]);
	sk->GrHandle[100] = LoadGraph("LR2files\\Config\\title.bmp", 0);
	LoadScene(sk, sm->Data[sm->previewID].skinFile, 0, 1);
	return 0;
}

//4218e0
int PlayPreviewSample(game *g) {
	int scratchSide = 0;
	ConfigStruct tCfg = g->config;
	tCfg.play.battle = (g->skinData.select >= 12);
	g->gameplay.isAutoplay = 1;
	scratchSide = 0;
	if (g->skinData.select == 3 && g->skinData.Data[g->skinData.skinID[3]].type == SKINTYPE_14KEYS)
		scratchSide = g->skstruct2.scratchside_1 + g->skstruct2.scratchside_2 * 2;
	if (g->skinData.select == 1 && g->skinData.Data[g->skinData.skinID[1]].type == SKINTYPE_7KEYS)
		scratchSide = g->skstruct2.scratchside_1 + g->skstruct2.scratchside_2 * 2;
	if (g->skinData.select == 13 && g->skinData.Data[g->skinData.skinID[13]].type == SKINTYPE_5KEYSBATTLE)
		scratchSide = g->skstruct2.scratchside_1 + g->skstruct2.scratchside_2 * 2;
	ProcS_Select(g);
	ReleaseBGA(g);
	switch (g->skinData.select) {
		default: //case 0:?
			ReadKeyConfig(g, "LR2files\\Config\\keyconfig.xml");
			g->sSelect.metaSelected.keymode = 7;
			InitGameplay(&g->gameplay, &tCfg.play);
			ParseBmsFile(&g->gameplay, "LR2files\\Config\\sample_7.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide);
			LoadBmsResource(&g->gameplay, "LR2files\\Config\\sample_7.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide, 0);
			break;

		case 1:
			if (g->skinData.Data[g->skinData.skinID[1]].type == SKINTYPE_7KEYS) 
				ReadKeyConfig(g, "LR2files\\Config\\keyconfig.xml");
			else 
				ReadKeyConfig(g, "LR2files\\Config\\keyconfig_5.xml");
			g->sSelect.metaSelected.keymode = 5;
			InitGameplay(&g->gameplay, &tCfg.play);
			ParseBmsFile(&g->gameplay, "LR2files\\Config\\sample_5.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide);
			LoadBmsResource(&g->gameplay, "LR2files\\Config\\sample_5.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide, 0);
			break;

		case 2:
			ReadKeyConfig(g, "LR2files\\Config\\keyconfig.xml");
			g->sSelect.metaSelected.keymode = 14;
			InitGameplay(&g->gameplay, &tCfg.play);
			ParseBmsFile(&g->gameplay, "LR2files\\Config\\sample_14.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide);
			LoadBmsResource(&g->gameplay, "LR2files\\Config\\sample_14.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide, 0);
			break;

		case 3:
			if (g->skinData.Data[g->skinData.skinID[3]].type == SKINTYPE_14KEYS)
				ReadKeyConfig(g, "LR2files\\Config\\keyconfig.xml");
			else
				ReadKeyConfig(g, "LR2files\\Config\\keyconfig_5.xml");
			g->sSelect.metaSelected.keymode = 10;
			InitGameplay(&g->gameplay, &tCfg.play);
			ParseBmsFile(&g->gameplay, "LR2files\\Config\\sample_10.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide);
			LoadBmsResource(&g->gameplay, "LR2files\\Config\\sample_10.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide, 0);
			break;

		case 4:
			ReadKeyConfig(g, "LR2files\\Config\\keyconfig_p.xml");
			g->sSelect.metaSelected.keymode = 9;
			InitGameplay(&g->gameplay, &tCfg.play);
			ParseBmsFile(&g->gameplay, "LR2files\\Config\\sample_9.pms", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide);
			LoadBmsResource(&g->gameplay, "LR2files\\Config\\sample_9.pms", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide, 0);
			break;

		case 13:
			if (g->skinData.Data[g->skinData.skinID[13]].type == SKINTYPE_5KEYSBATTLE)
				ReadKeyConfig(g, "LR2files\\Config\\keyconfig.xml");
			else
				ReadKeyConfig(g, "LR2files\\Config\\keyconfig_5.xml");
			g->sSelect.metaSelected.keymode = 5;
			InitGameplay(&g->gameplay, &tCfg.play);
			ParseBmsFile(&g->gameplay, "LR2files\\Config\\sample_5.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide);
			LoadBmsResource(&g->gameplay, "LR2files\\Config\\sample_5.bme", &g->audio, &tCfg, &g->sSelect.metaSelected, 1, scratchSide, 0);
			break;
	}

	g->gameplay.bgaLayer1 = 1;
	if (g->skinData.select == 7) {
		g->gameplay.player[0].clearType = (GetRand(1) == 0) ? 5 : 1;
		g->gameplay.player[1].clearType = g->gameplay.player[0].clearType;
		for (int p = 0; p < 2; p++) {
			g->gameplay.player[p].rate = 100.0;
			g->gameplay.player[p].totalnotes = 1000;
			g->gameplay.player[p].max_combo = 1000;
			g->gameplay.player[p].judgecount[5] = 1000;
			g->gameplay.player[p].exscore = 2000;
			g->gameplay.player[p].score = 200000;

			for (int i = 0; i < 1000; i++) {
				g->gameplay.statgraph[p].combo[i] = g->gameplay.player[p].totalnotes * i / 1000;
				g->gameplay.statgraph[p].exscore[i] = g->gameplay.player[p].totalnotes * i * 16 / 9000;
				g->gameplay.statgraph[p].hp[i] = (100 * i) / 1000;
				if (p == 0) {
					g->gameplay.rategraph[0].val[i] = g->gameplay.statgraph[0].exscore[0] * 7 / 8;
					g->gameplay.rategraph[1].val[i] = g->gameplay.statgraph[0].exscore[0] * 6 / 8;
				}
			}

			g->gameplay.statgraph[p].cursor = 1000;
			g->gameplay.rategraph[0].cursor = 1000;
			g->gameplay.rategraph[1].cursor = 1000;
		}
	}
	return 1;
}

//42d9a0
int ProcS_SkinSelect(game *g) {
	CSTR path;
	SkinUser sku;

	SkinManage &d = g->skinData;

	d.select = 0;
	d.previewID = d.skinID[0];
	d.previewCustomID = 0;

	int &n = d.previewID;
	SkinHeader &skd = d.Data[n];

	cstrSprintf(&path, "LR2files\\SkinCustomize\\%s.xml", MD5str(skd.skinFile));
	ReadSkinCustomize(&sku, path);

	for (int i = 0; i < 20; i++) {
		if (skd.customs[i].dst_op_start) {
			if (skd.customs[i].dst_op_start > sku.customize_value[i] ||	skd.customs[i].dst_op_start + skd.customs[i].dst_op_count - 1 < sku.customize_value[i]) {
				skd.customs[i].dst_op_selected = skd.customs[i].dst_op_start;
			}
			else {
				skd.customs[i].dst_op_selected = sku.customize_value[i];
			}
		}
		else {
			for (int j = 0; j < skd.customs[i].dst_op_count; j++) {
				if (skd.customs[i].op_label[j].isSame(&sku.customize_filename[i])) {
					skd.customs[i].dst_op_selected = j;
				}
			}
		}
	}
	SetObjectString(50, skd.title, g->txtStruct.objectStr);
	SetObjectString(51, skd.maker, g->txtStruct.objectStr);
	
	for (int i = 0; i < 5; i++) {
		SetObjectString(100 + i, skd.customs[d.previewCustomID + i].title, g->txtStruct.objectStr);
		SetObjectString(110 + i, skd.customs[d.previewCustomID + i].op_label[skd.customs[d.previewCustomID + i].dst_op_selected - skd.customs[d.previewCustomID + i].dst_op_start], g->txtStruct.objectStr);
	}
	for (int i = 0; i < 1000; i++) {
		g->skstruct2.op[i] = (GetOptionFlag_dst(g, i) > 0);
	}
	MakeSkinPreview(g, &g->skstruct2, &d);
	PlayPreviewSample(g);
	InitTimer(&g->timer2);
	SetTimeLapse(0, &g->timer2);
	return 1;
}


//409ac0
int SkinPreviewNext(SkinManage *sm, SKINTYPE type){

	SKINTYPE newType;
	int previewNow;

	previewNow = sm->previewID;
	//search from previewNow + 1
	for (int i = previewNow + 1; i < sm->Count; i++) {
		newType = sm->Data[i].type;
		if (newType == type) {
			sm->previewID = i;
			return 1;
		}
		if (type == SKINTYPE_5KEYS) {
			if (newType == SKINTYPE_7KEYS) {
				sm->previewID = i;
				return 1;
			}
		}
		else if (type == SKINTYPE_10KEYS) {
			if (newType == SKINTYPE_14KEYS) {
				sm->previewID = i;
				return 1;
			}
		}
		else if (type == SKINTYPE_7KEYSBATTLE) {
			if (newType == SKINTYPE_5KEYSBATTLE) {
				sm->previewID = i;
				return 1;
			}
		}
	}
	//restart search from 0
	for (int i = 0; i < previewNow; i++) {
		newType = sm->Data[i].type;
		if (newType == type) {
			sm->previewID = i;
			return 1;
		}
		if (type == SKINTYPE_5KEYS) {
			if (newType == SKINTYPE_7KEYS) {
				sm->previewID = i;
				return 1;
			}
		}
		else if (type == SKINTYPE_10KEYS) {
			if (newType == SKINTYPE_14KEYS) {
				sm->previewID = i;
				return 1;
			}
		}
		else if (type == SKINTYPE_7KEYSBATTLE) {
			if (newType == SKINTYPE_5KEYSBATTLE) {
				sm->previewID = i;
				return 1;
			}
		}
	}

	return 0;
}

//409b80
int SkinPreviewPrev(SkinManage *sm, SKINTYPE type) {

	SKINTYPE newType;
	int previewNow;

	previewNow = sm->previewID;
	//search from previewNow - 1
	for (int i = previewNow - 1; i > -1; i--) {
		newType = sm->Data[i].type;
		if (newType == type) {
			sm->previewID = i;
			return 1;
		}
		if (type == SKINTYPE_5KEYS) {
			if (newType == SKINTYPE_7KEYS) {
				sm->previewID = i;
				return 1;
			}
		}
		else if (type == SKINTYPE_10KEYS) {
			if (newType == SKINTYPE_14KEYS) {
				sm->previewID = i;
				return 1;
			}
		}
		else if (type == SKINTYPE_7KEYSBATTLE) {
			if (newType == SKINTYPE_5KEYSBATTLE) {
				sm->previewID = i;
				return 1;
			}
		}
	}
	//restart search from sm->count-1
	for (int i = sm->Count - 1; i > previewNow; i--) {
		newType = sm->Data[i].type;
		if (newType == type) {
			sm->previewID = i;
			return 1;
		}
		if (type == SKINTYPE_5KEYS) {
			if (newType == SKINTYPE_7KEYS) {
				sm->previewID = i;
				return 1;
			}
		}
		else if (type == SKINTYPE_10KEYS) {
			if (newType == SKINTYPE_14KEYS) {
				sm->previewID = i;
				return 1;
			}
		}
		else if (type == SKINTYPE_7KEYSBATTLE) {
			if (newType == SKINTYPE_5KEYSBATTLE) {
				sm->previewID = i;
				return 1;
			}
		}
	}

	return 0;
}

//409c40
int SelectSkin(SkinManage *sm, CSTR *obuf){
	obuf->assign(&sm->Data[sm->previewID].skinFile);
	sm->skinID[sm->select] = sm->previewID;
	return 1;
}

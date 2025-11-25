#include "LR2_bmsload.h"
#include "Engine.h"
#include "LR2_replay.h"

//4081d0
int StopAllKeysound(game *g){
	for (int i = 0; i < 6480; i++) {
		StopSound(&g->audio, &g->gameplay.keysound[i]);
	}
	return 1;
}

//408210
int InitKeysound(game *g){
	for (int i = 0; i < 6480; i++) {
		StopSound(&g->audio, &g->gameplay.keysound[i]);
		ReleaseSound(&g->audio, &g->gameplay.keysound[i]);
	}
	ErrorLogAdd("BMSの音を初期化しました\n");
	return 1;
}

//408260
int ReleaseBGA(game *g){

	for (int i = 0; i < 6480; i++) {
		DeleteGraph(g->gameplay.bgaHandle[i]);
		g->gameplay.bgaHandle[i] = -1;
	}
	return 1;
}

//408430
void ProcLoadBmsResource(game *g) {
	g->gameplay.bmsResourceLoaded = 0;
	g->gameplay.flag_closingPhase = '\0';
	LoadBmsResource(&g->gameplay, g->sSelect.metaSelected.filepath, &g->audio, &g->config, &g->sSelect.metaSelected, g->skstruct.flag_BGA, g->skstruct.flag_flip, 0);
	g->gameplay.bmsResourceLoaded = 1;
	return;
}


//4a9fd0
bool isVisibleNote(int ch){
	if (10 <= ch && ch < 30) {
		return true;
	}
	return (50 <= ch && ch < 70);
}

//4ac140
int InitNoteBuffer(LaneStruct *lane, int count){

	lane->size = count;
	lane->notes = (NoteStruct *)malloc(count * sizeof(NoteStruct));
	
	lane->count = 0;
	lane->autoplay = 0;

	for (int i = 0; i < lane->size; i++) {
		lane->notes[i].bmsTiming_ln = -1.0;
		lane->notes[i].realTiming_ln = -1.0;
		lane->notes[i].active = -1;
		lane->notes[i].val = -1.0;
		lane->notes[i].bmsTiming = -1.0;
		lane->notes[i].realTiming = -1.0;
		lane->notes[i].op = -1;
		lane->notes[i].mine = -1;
	}
	return 1;
}


//4ac1c0
int ExpandNoteBuffer(LaneStruct *lane, int addsize){

	int oldCount;

	oldCount = lane->size;
	lane->size += addsize;
	lane->notes = (NoteStruct*)realloc(lane->notes, (lane->size) * sizeof(NoteStruct));

	for (int i = oldCount; i < lane->size; i++) {
		lane->notes[i].bmsTiming_ln = -1.0;
		lane->notes[i].realTiming_ln = -1.0;
		lane->notes[i].active = -1;
		lane->notes[i].val = -1.0;
		lane->notes[i].bmsTiming = -1.0;
		lane->notes[i].realTiming = -1.0;
		lane->notes[i].op = -1;
		lane->notes[i].mine = -1;
	}
	return 1;
}

//4ac240
int CMP_NotesByBmsTiming(const void *p1, const void *p2){
	NoteStruct* n1 = (NoteStruct*)p1;
	NoteStruct* n2 = (NoteStruct*)p2;

	if (n1->bmsTiming >= 0.0 && n2->bmsTiming < 0.0) return -1;
	if (n1->bmsTiming < 0.0 && n2->bmsTiming < 0.0) return 0;
	if (n1->bmsTiming < 0.0 && n2->bmsTiming >= 0.0) return 1;

	if (n2->bmsTiming == n1->bmsTiming) return n1->op - n2->op;

	return n1->bmsTiming * 100000.0 - n2->bmsTiming * 100000.0;
	
	/*if (p1->bmsTiming >= 0.0) {
		if (p2->bmsTiming < 0.0) {
			return -1;
		}
	}
	else if (p2->bmsTiming < 0.0) {
		return 0;
	}
	if (p1->bmsTiming < 0.0) {
		return 1;
	}
	if (p2->bmsTiming == p1->bmsTiming) {
		return p1->op - p2->op;
	}
	return p1->bmsTiming * 100000.0 - p2->bmsTiming * 100000.0;*/
}

//4ac2b0
int CMP_NotesByRealTiming(const void *p1, const void *p2){
	NoteStruct* n1 = (NoteStruct*)p1;
	NoteStruct* n2 = (NoteStruct*)p2;
	
	return n1->realTiming - n2->realTiming;
}

//4ac2d0
int CMP_NotesByRealTimingOp(const void *p1, const void *p2){
	NoteStruct* n1 = (NoteStruct*)p1;
	NoteStruct* n2 = (NoteStruct*)p2;

	if (n2->realTiming == n1->realTiming) {
		return n1->op - n2->op;
	}
	return n1->realTiming - n2->realTiming;
}

//4ac300
int PlayerCheckAndSwap(gameplay *gp){
	PLAYERSTATUS temp_status;
	GRAPHDATA temp_graph;

	if (gp->player[1].flag_active == 0) {
		ErrorLogAdd("メインプレイヤーチェック：スコアを入れ替えません\n");
	}
	else {
		memcpy(&temp_status, &gp->player[0], sizeof(PLAYERSTATUS));
		memcpy(&gp->player[0], &gp->player[1], sizeof(PLAYERSTATUS));
		memcpy(&gp->player[1], &temp_status, sizeof(PLAYERSTATUS));

		memcpy(&temp_graph, &gp->statgraph[0], sizeof(GRAPHDATA));
		memcpy(&gp->statgraph[0], &gp->statgraph[1], sizeof(GRAPHDATA));
		memcpy(&gp->statgraph[1], &temp_graph, sizeof(GRAPHDATA));

		gp->player[0].clearType = gp->player[1].clearType;
		ErrorLogAdd("メインプレイヤーチェック：スコアを入れ替えました\n");
	}
	gp->player[0].flag_active = 1;
	gp->player[1].flag_active = 0;
	return 1;
}

//4ac3e0
int InitGameplay(gameplay *gp, CONFIG_PLAY *cfg) {

	PlayerCheckAndSwap(gp);
	gp->bpmt_start = 1;
	gp->isPreviewLoad = 0;
	if (gp->bmsobj.count == 0) InitNoteBuffer(&gp->bmsobj, 1000);
	for (int i = 0; i < gp->bmsobj.size; i++) {
		gp->bmsobj.notes[i].bmsTiming_ln = -1.0;
		gp->bmsobj.notes[i].realTiming_ln = -1.0;
		gp->bmsobj.notes[i].active = -1;
		gp->bmsobj.notes[i].val = -1.0;
		gp->bmsobj.notes[i].bmsTiming = -1.0;
		gp->bmsobj.notes[i].realTiming = -1.0;
		gp->bmsobj.notes[i].op = -1;
		gp->bmsobj.notes[i].mine = -1;
		gp->bmsobj.notes[i].stage = 0;
	}
	gp->bmsobj.count = 0;
	gp->bmsobj.draw_count = 0;
	gp->bmsobj.note_count = 0;
	gp->bmsobj.autoplay = 0;
	gp->bmsobj.noteVal = -1;

	for (int lane = 0; lane < 20; lane++) {
		if (gp->bmsobj_note[lane].count == 0) InitNoteBuffer(&gp->bmsobj_note[lane], 100);
		for (int i = 0; i < gp->bmsobj_note[lane].size; i++) {
			gp->bmsobj_note[lane].notes[i].bmsTiming_ln = -1.0;
			gp->bmsobj_note[lane].notes[i].realTiming_ln = -1.0;
			gp->bmsobj_note[lane].notes[i].active = -1;
			gp->bmsobj_note[lane].notes[i].val = -1.0;
			gp->bmsobj_note[lane].notes[i].bmsTiming = -1.0;
			gp->bmsobj_note[lane].notes[i].realTiming = -1.0;
			gp->bmsobj_note[lane].notes[i].op = -1;
			gp->bmsobj_note[lane].notes[i].mine = -1;
			gp->bmsobj_note[lane].notes[i].stage = 0;
		}
		gp->bmsobj_note[lane].count = 0;
		gp->bmsobj_note[lane].draw_count = 0;
		gp->bmsobj_note[lane].note_count = 0;
		gp->bmsobj_note[lane].autoplay = 0;
		gp->bmsobj_note[lane].noteVal = -1;
	}

	if (gp->bmsobj_line.count == 0) InitNoteBuffer(&gp->bmsobj_line, 100);
	for (int i = 0; i < gp->bmsobj_line.size; i++) {
		gp->bmsobj_line.notes[i].bmsTiming_ln = -1.0;
		gp->bmsobj_line.notes[i].realTiming_ln = -1.0;
		gp->bmsobj_line.notes[i].active = -1;
		gp->bmsobj_line.notes[i].val = -1.0;
		gp->bmsobj_line.notes[i].bmsTiming = -1.0;
		gp->bmsobj_line.notes[i].realTiming = -1.0;
		gp->bmsobj_line.notes[i].op = -1;
		gp->bmsobj_line.notes[i].mine = -1;
		gp->bmsobj_line.notes[i].stage = 0;
	}
	gp->bmsobj_line.count = 0;
	gp->bmsobj_line.draw_count = 1;
	gp->bmsobj_line.note_count = 0;
	gp->bmsobj_line.autoplay = 0;
	gp->bmsobj_line.noteVal = -1;

	gp->BPM_fix = 0.0;
	gp->BPM = 0.0;
	gp->lastMeasure = 0;
	gp->song_runtime = 0.0;
	gp->trialClear = 0;
	gp->delayCheckCount = 0;
	gp->delayDetectedCount = 0; 
	gp->nabeatsu_x = 0.0;
	gp->nabeatsu_y = 0.0;
	gp->isNosave = '\0'; //TOFIX : ?
	gp->unusedX_7bf50 = 0.0;
	gp->unusedY_7bf54 = 0.0;
	gp->earthquake_x = 0.0;
	gp->earthquake_y = 0.0;
	
	for (int p = 0; p < 2; p++) {
		int prevJudge[6];
		memcpy(prevJudge,gp->player[p].judgecount2,6*sizeof(int));
		int prevNowcombo = gp->player[p].now_combo_course;
		int prevMaxcombo = gp->player[p].max_combo_course;
		int prevCombodraw = gp->player[p].combo_draw;
		int prevTotalnote = gp->player[p].total_note;
		int prevNotecurrent = gp->player[p].note_current2;
		int prevGaugeType = gp->player[p].gaugeType;
		int prevLastCourseGaugeType = gp->player[p].lastCourseGaugeType;
		auto prevHP = gp->player[p].HP;
		double prevHP_print = gp->player[p].HP[prevGaugeType];
		EXTENDEDPLAYERSTATS extendedStatsCourse = gp->player[p].extendedStatsCourse;
		std::array<EXTENDEDPLAYERSTATS, 20> extendedColumnStatsCourse = gp->player[p].extendedColumnStatsCourse;

		gp->player[p] = PLAYERSTATUS();
		gp->player[p].gaugeType = cfg->gaugeOption[p];
		gp->statgraph[p] = GRAPHDATA();

		if (gp->courseStageNow > 0) {
			gp->player[p].gaugeType = prevGaugeType;
			gp->player[p].lastCourseGaugeType = prevLastCourseGaugeType;
			gp->player[p].HP = prevHP;
			gp->player[p].HP_print = prevHP_print;
			gp->player[p].now_combo_course = prevNowcombo;
			gp->player[p].max_combo_course = prevMaxcombo;
			gp->player[p].combo_draw = prevCombodraw;
			memcpy(gp->player[p].judgecount2, prevJudge, 6 * sizeof(int));
			gp->player[p].total_note = prevTotalnote;
			gp->player[p].note_current2 = prevNotecurrent;
			gp->player[p].extendedStatsCourse = extendedStatsCourse;
			gp->player[p].extendedColumnStatsCourse = extendedColumnStatsCourse;
		}
	}
	gp->player[0].flag_active = 1;
	gp->player[1].flag_active = 0;

	for (int p = 0; p < 2; p++) {
		if (gp->courseStageNow < 1) {
			if (gp->isCourse == 0) {
				gp->player[p].HP[0] = 20.;
				gp->player[p].HP[3] = 20.;
			}
			else {
				gp->player[p].HP[0] = 100.;
				gp->player[p].HP[3] = 100.;
			}
			gp->player[p].HP[1] = 100.;
			gp->player[p].HP[2] = 100.;
			gp->player[p].HP[4] = 100.;
			gp->player[p].HP[5] = 100.;
			gp->player[p].HP_old = gp->player[p].HP[gp->player[p].gaugeType];
			gp->player[p].HP_print = gp->player[p].HP[gp->player[p].gaugeType];
			for (int i = 0; i < 6; i++) {
				gp->statgraph[p].hp[i][0] = gp->player[p].HP[i];
			}
		}
	}
	for (int i = 0; i < 6480; i++) {
		gp->bgaHandle[i] = -1;
		gp->bgaHandleHandle[i] = -1;
		gp->bgaUnused656b8[i] = 0;
	}
	gp->bgaLayer1 = -1;
	gp->bgaLayer2 = -1;
	gp->missLayer = -1;
	gp->courseBgaLayer1[0] = -1;
	gp->courseBgaLayer2[0] = -1;
	gp->courseMissLayer[0] = -1;
	gp->courseLayer1ChangeTime[0] = -1;
	gp->courseLayer2ChangeTime[0] = -1;
	gp->courseBgaLayer1[1] = -1;
	gp->courseBgaLayer2[1] = -1;
	gp->courseMissLayer[1] = -1;
	gp->courseLayer1ChangeTime[1] = -1;
	gp->courseLayer2ChangeTime[1] = -1;
	gp->courseBgaLayer1[2] = -1;
	gp->courseBgaLayer2[2] = -1;
	gp->courseMissLayer[2] = -1;
	gp->courseLayer1ChangeTime[2] = -1;
	gp->courseLayer2ChangeTime[2] = -1;
	gp->courseBgaLayer1[3] = -1;
	gp->courseBgaLayer2[3] = -1;
	gp->courseMissLayer[3] = -1;
	gp->courseLayer1ChangeTime[3] = -1;
	gp->courseLayer2ChangeTime[3] = -1;
	gp->courseBgaLayer1[4] = -1;
	gp->courseBgaLayer2[4] = -1;
	gp->courseMissLayer[4] = -1;
	gp->courseLayer1ChangeTime[4] = -1;
	gp->courseLayer2ChangeTime[4] = -1;
	gp->courseBgaLayer1[5] = -1;
	gp->courseBgaLayer2[5] = -1;
	gp->courseMissLayer[5] = -1;
	gp->courseLayer1ChangeTime[5] = -1;
	gp->courseLayer2ChangeTime[5] = -1;
	gp->courseBgaLayer1[6] = -1;
	gp->courseBgaLayer2[6] = -1;
	gp->courseMissLayer[6] = -1;
	gp->courseLayer1ChangeTime[6] = -1;
	gp->courseLayer2ChangeTime[6] = -1;
	gp->courseBgaLayer1[7] = -1;
	gp->courseBgaLayer2[7] = -1;
	gp->courseMissLayer[7] = -1;
	gp->courseLayer1ChangeTime[7] = -1;
	gp->courseLayer2ChangeTime[7] = -1;
	gp->courseBgaLayer1[8] = -1;
	gp->courseBgaLayer2[8] = -1;
	gp->courseMissLayer[8] = -1;
	gp->courseLayer1ChangeTime[8] = -1;
	gp->courseLayer2ChangeTime[8] = -1;
	gp->courseBgaLayer1[9] = -1;
	gp->courseBgaLayer2[9] = -1;
	gp->courseMissLayer[9] = -1;
	gp->courseLayer1ChangeTime[9] = -1;
	gp->courseLayer2ChangeTime[9] = -1;
	gp->speedmultiplier = 1.0;
	gp->layer2ChangeTime = -1;
	gp->maxBPM = 0.0;
	gp->layer1ChangeTime = -1;
	gp->minBPM = 0.0;
	gp->loadObject_loaded = 0;
	gp->loadObject_total = 0;
	gp->lastMissTime = 0;
	gp->misslayerTime[0] = 0;
	gp->misslayerTime[1] = 0;
	gp->soundonly = '\x01';
	gp->fxChangeInRecording = '\0';
	gp->procGameCallCount = 0;
	gp->isSpeedChanged = false;
	gp->lanecoverDoubleclickTimeP1 = 0;
	gp->lanecoverDoubleclickTimeP2 = 0;
	gp->lanecoverDisplayP1 = '\x01';
	gp->lanecoverDisplayP2 = '\x01';
	gp->isForceEasy = '\0';
	gp->flag_threadDoingProcGame = '\0';
	gp->flag_gameinput = '\0';
	gp->flag_longsound = '\0';
	gp->flag_0note = '\0';
	gp->isGhostDisabled = '\0';
	gp->bpmChangedRealtime = -1;
	gp->bpmChangedBmstime = -1;
	for (int i = 0; i < 1000; i++) {
		gp->rategraph[0].val[i] = 0;
		gp->rategraph[1].val[i] = 0;
	}
	gp->rategraph[0].cursor = 0;
	gp->rategraph[1].cursor = 0;
	gp->unused_73b68 = 0;
	gp->unused_7bf68 = 0;
	for (int i = 0; i < 6480; i++) {
		gp->keysound_filename[i].fillzero();
		gp->BMP_filename[i].fillzero();
	}
	gp->fadeinSOUNDstart[0] = -1;
	gp->fadeinSOUNDend[0] = -1;
	gp->fadeoutSOUNDend[0] = -1;
	gp->fadeinBGAstart[0] = -1;
	gp->fadeinBGAend[0] = -1;
	gp->fadeoutBGAend[0] = -1;
	gp->fadeinSOUNDstart[1] = -1;
	gp->fadeinSOUNDend[1] = -1;
	gp->fadeoutSOUNDend[1] = -1;
	gp->fadeinBGAstart[1] = -1;
	gp->fadeinBGAend[1] = -1;
	gp->fadeoutBGAend[1] = -1;
	gp->fadeinSOUNDstart[2] = -1;
	gp->fadeinSOUNDend[2] = -1;
	gp->fadeoutSOUNDend[2] = -1;
	gp->fadeinBGAstart[2] = -1;
	gp->fadeinBGAend[2] = -1;
	gp->fadeoutBGAend[2] = -1;
	gp->fadeinSOUNDstart[3] = -1;
	gp->fadeinSOUNDend[3] = -1;
	gp->fadeoutSOUNDend[3] = -1;
	gp->fadeinBGAstart[3] = -1;
	gp->fadeinBGAend[3] = -1;
	gp->fadeoutBGAend[3] = -1;
	gp->fadeinSOUNDstart[4] = -1;
	gp->fadeinSOUNDend[4] = -1;
	gp->fadeoutSOUNDend[4] = -1;
	gp->fadeinBGAstart[4] = -1;
	gp->fadeinBGAend[4] = -1;
	gp->fadeoutBGAend[4] = -1;
	gp->fadeinSOUNDstart[5] = -1;
	gp->fadeinSOUNDend[5] = -1;
	gp->fadeoutSOUNDend[5] = -1;
	gp->fadeinBGAstart[5] = -1;
	gp->fadeinBGAend[5] = -1;
	gp->fadeoutBGAend[5] = -1;
	gp->fadeinSOUNDstart[6] = -1;
	gp->fadeinSOUNDend[6] = -1;
	gp->fadeoutSOUNDend[6] = -1;
	gp->fadeinBGAstart[6] = -1;
	gp->fadeinBGAend[6] = -1;
	gp->fadeoutBGAend[6] = -1;
	gp->fadeinSOUNDstart[7] = -1;
	gp->fadeinSOUNDend[7] = -1;
	gp->fadeoutSOUNDend[7] = -1;
	gp->fadeinBGAstart[7] = -1;
	gp->fadeinBGAend[7] = -1;
	gp->fadeoutBGAend[7] = -1;
	gp->fadeinSOUNDstart[8] = -1;
	gp->fadeinSOUNDend[8] = -1;
	gp->fadeoutSOUNDend[8] = -1;
	gp->fadeinBGAstart[8] = -1;
	gp->fadeinBGAend[8] = -1;
	gp->fadeoutBGAend[8] = -1;
	gp->fadeinSOUNDstart[9] = -1;
	gp->fadeinSOUNDend[9] = -1;
	gp->fadeoutSOUNDend[9] = -1;
	gp->fadeinBGAstart[9] = -1;
	gp->fadeinBGAend[9] = -1;
	gp->fadeoutBGAend[9] = -1;
	gp->bgaMixer[1] = 0;
	gp->bgaMixer[2] = 0;
	gp->bgaMixer[3] = 0;
	gp->bgaMixer[4] = 0;
	gp->bgaMixer[5] = 0;
	gp->bgaMixer[6] = 0;
	gp->bgaMixer[7] = 0;
	gp->bgaMixer[8] = 0;
	gp->bgaMixer[9] = 0;
	gp->bgaMixer[0] = 100;

	return 1;
}

//4acc60
int LoadBmsResource(gameplay *gp, CSTR BMSfilepath, AUDIO *aud, ConfigStruct *cfg, BMSMETA *meta, char bga, char flip, char noVideo){

	int Rtmp, Gtmp, Btmp;

	ErrorLogAdd("BMSの画像とサウンドをロードします");

	if ((cfg->play).autojudge == 2) { //silent
		gp->loadObject_loaded = gp->loadObject_total;
		return 1;
	}

	std::unique_lock l{gp->criticalSection};

	for (int i = 0; i < 6480; i++) {
		if (gp->keysound_filename[i].length() > 0) {
			LoadSound(aud, &gp->keysound[i], gp->keysound_filename[i], 0, cfg->sound.disabledsp, (gp->isPreviewLoad != 0));
			if (gp->keysound[i].length > 60000 && gp->keysound[i].load) gp->flag_longsound = 1;
			gp->loadObject_loaded++;
		}
		if (gp->flag_closingPhase || (gp->previewStatus != 1 && noVideo)) {
			return 1;
		}
	}

	gp->flag_0note = 1;
	for (int i = 0; i < gp->bmsobj.count; i++) {
		if ( (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30)
			&& (0 < gp->bmsobj.notes[i].val && gp->bmsobj.notes[i].val < 6480.0)
			&& gp->keysound[(int)gp->bmsobj.notes[i].val].load) {

			gp->flag_0note = 0;
			break;
		}
	}

	if (noVideo) {
		gp->loadObject_loaded = gp->loadObject_total;
		return 1;
	}
	
	if (cfg->system.isablebmsthread == 0) CoInitialize(NULL);
	GetTransColor(&Rtmp,&Gtmp,&Btmp);
	SetTransColor(0,0,0);
	for (int i = 0; i < 6480; i++) {
		if (gp->BMP_filename[i].length() > 0) {
			if (gp->BMP_filename[i].right(3).isSame("mpg") || gp->BMP_filename[i].right(3).isSame("avi")) {
				SetTransColor(0, 255, 0);
				gp->bgaHandle[i] = LoadGraph(gp->BMP_filename[i]);
				SetTransColor(0, 0, 0);
			}
			else {
				gp->bgaHandle[i] = LoadGraph(gp->BMP_filename[i]);
			}
			gp->loadObject_loaded++;
		}

		if (gp->flag_closingPhase) {
			if (cfg->system.isablebmsthread == 0) CoUninitialize();
			return 1;
		}
	}

	SetTransColor(Rtmp, Gtmp, Btmp);
	if (cfg->system.isablebmsthread == 0) CoUninitialize();
	if (gp->bgaHandle[0] != -1) gp->missLayer = 0;

	if (gp->isAutoplay == 1) {

		for (int i = 0; i < gp->bmsobj.count; i++) {
			if (!(gp->bmsobj.notes[i].op >= 10 && gp->bmsobj.notes[i].op < 30)) {
				if (gp->bmsobj.notes[i].op == 1) {
					if (0 < gp->bmsobj.notes[i].val && gp->bmsobj.notes[i].val < 6480.0) { //TODO : is it okay to delete compiler code dealing unsigned?
						if ((gp->song_runtime < gp->keysound[(int)gp->bmsobj.notes[i].val].length + gp->bmsobj.notes[i].realTiming) && (int)gp->keysound[(int)gp->bmsobj.notes[i].val].length > 0) {
							double len = gp->keysound[(int)gp->bmsobj.notes[i].val].length;
							if ((int)gp->keysound[(int)gp->bmsobj.notes[i].val].length < 0) len += 4294967296.0;
							gp->song_runtime = gp->bmsobj.notes[i].realTiming + len;
						}
					}
				}
				else if ((gp->bmsobj.notes[i].op == 4 || gp->bmsobj.notes[i].op == 7)
					&& 0 < gp->bmsobj.notes[i].val && gp->bmsobj.notes[i].val < 6480.0
					&& gp->song_runtime < gp->bmsobj.notes[i].realTiming) {

					gp->song_runtime = gp->bmsobj.notes[i].realTiming;
				}
			}
			else if (0 < gp->bmsobj.notes[i].val && gp->bmsobj.notes[i].val < 6480.0) {
				if (gp->keysound[(int)gp->bmsobj.notes[i].val].load == 0 && gp->song_runtime < gp->bmsobj.notes[i].realTiming)
					gp->song_runtime = gp->bmsobj.notes[i].realTiming;

				if (gp->keysound[(int)gp->bmsobj.notes[i].val].length < 2000 && gp->keysound[(int)gp->bmsobj.notes[i].val].length > 0) {
					if (gp->song_runtime < gp->bmsobj.notes[i].realTiming + gp->keysound[(int)gp->bmsobj.notes[i].val].length) {
						gp->song_runtime = gp->bmsobj.notes[i].realTiming + gp->keysound[(int)gp->bmsobj.notes[i].val].length;
					}
				}
				else {
					double len = gp->keysound[(int)gp->bmsobj.notes[i].val].length;
					if ((int)gp->keysound[(int)gp->bmsobj.notes[i].val].length < 0) len += 4294967296.0;
					if (gp->song_runtime < gp->bmsobj.notes[i].realTiming + len && (int)gp->keysound[(int)gp->bmsobj.notes[i].val].length > 0) {
						gp->song_runtime = gp->bmsobj.notes[i].realTiming + len;
					}
				}
			}
		}
	}
	return 0;
}

//4ad200
int InitGameplay_retry(gameplay *gp, AUDIO *snd, game *g) {
	
	PlayerCheckAndSwap(gp);
	if (gp->replay.status != 2 && gp->replay.status == 1) {
		AddReplayData(&gp->replay, 0, 200, gp->randomseed);
	}

	gp->bgaMixer[1] = 0;
	gp->bgaMixer[2] = 0;
	gp->bgaMixer[3] = 0;
	gp->bgaMixer[4] = 0;
	gp->bgaMixer[5] = 0;
	gp->bgaMixer[6] = 0;
	gp->bgaMixer[7] = 0;
	gp->bgaMixer[8] = 0;
	gp->bgaMixer[9] = 0;
	gp->bgaMixer[0] = 100;

	gp->unused_7bf68 = '\0';
	gp->unused_73b68 = 0;
	gp->lanecoverDisplayP1 = true;
	gp->lanecoverDisplayP2 = true;
	gp->flag_threadDoingProcGame = false;
	gp->lanecoverDoubleclickTimeP1 = 0;
	gp->lanecoverDoubleclickTimeP2 = 0;
	gp->trialClear = 0;
	gp->bpmt_start = 1;
	gp->procGameCallCount = 0;

	for (int i = 0; i < gp->bmsobj.size; i++) {
		gp->bmsobj.notes[i].active = 0;
	}
	gp->bmsobj.draw_count = 0;
	gp->bmsobj.note_count = 0;
	gp->bmsobj.noteVal = -1;

	for (int lane = 0; lane < 20; lane++) {
		for (int i = 0; i < gp->bmsobj_note[lane].size; i++) {
			gp->bmsobj_note[lane].notes[i].active = 0;
		}
		gp->bmsobj_note[lane].draw_count = 0;
		gp->bmsobj_note[lane].note_count = 0;
		gp->bmsobj_note[lane].noteVal = -1;
	}

	for (int i = 0; i < gp->bmsobj_line.size; i++) {
		gp->bmsobj_line.notes[i].active = 0;
	}
	gp->bmsobj_line.draw_count = 1;
	gp->bmsobj_line.note_count = 0;
	gp->bmsobj_line.noteVal = -1;
	gp->delayDetectedCount = 0;
	gp->delayCheckCount = 0;

	for (int p = 0; p < 2; p++) {
		int tempTime[6], tempCount;
		tempCount = gp->player[p].totalnotes;
		auto tempDmg = gp->player[p].judge_damage;
		memcpy(tempTime, &gp->player[p].judgetime, sizeof(tempTime));
		gp->player[p] = PLAYERSTATUS();
		gp->player[p].gaugeType = g->config.play.gaugeOption[p];
		gp->player[p].judge_damage = tempDmg;
		memcpy(&gp->player[p].judgetime, tempTime, sizeof(tempTime));
		gp->player[p].totalnotes = tempCount;
		gp->statgraph[p] = GRAPHDATA();
	}
	gp->player[0].flag_active = 1;
	gp->player[1].flag_active = 0;

	for (int p = 0; p < 2; p++) {
		if (gp->isCourse == 0) {
			gp->player[p].HP[0] = 20.;
			gp->player[p].HP[3] = 20.;
		}
		else {
			gp->player[p].HP[0] = 100.;
			gp->player[p].HP[3] = 100.;
		}
		gp->player[p].HP[1] = 100.;
		gp->player[p].HP[2] = 100.;
		gp->player[p].HP[4] = 100.;
		gp->player[p].HP[5] = 100.;
		gp->player[p].HP_old = gp->player[p].HP[gp->player[p].gaugeType];
		gp->player[p].HP_print = gp->player[p].HP[gp->player[p].gaugeType];
		for (int i = 0; i < 6; i++) {
			gp->statgraph[p].hp[i][0] = gp->player[p].HP[i];
		}
	}

	for (int i = 0; i < 1296; i++) {
		gp->bgaHandleHandle[i] = -1;
	}

	gp->BPM = gp->BPM_fix;
	gp->bgaLayer1 = -1;
	gp->bgaLayer2 = -1;
	gp->layer2ChangeTime = -1;
	gp->layer1ChangeTime = -1;
	gp->isSpeedChanged = false;
	gp->flag_gameinput = false;
	gp->missLayer = (gp->bgaHandle[0] == -1)? -1 : 0;
	gp->lastMissTime = 0;
	gp->misslayerTime[0] = 0;
	gp->misslayerTime[1] = 0;
	gp->p1Score.InitJudgeQueue();
	gp->p1Score.ResetJudgeQueue(gp->player[0].totalnotes * 2);
	gp->fxChangeInRecording = false;

	for (int i = 0; i < 20; i++) {
		if(gp->bmsobj_note[i].count <= 0)
			gp->bmsobj_note[i].noteVal = -1;
		else
			gp->bmsobj_note[i].noteVal = (int)gp->bmsobj_note[i].notes[0].val;
	}

	gp->nabeatsu_x = 0.0;
	gp->nabeatsu_y = 0.0;
	gp->bpmChangedRealtime = -1;
	gp->bpmChangedBmstime = -1;
	gp->unusedX_7bf50 = 0.0;
	gp->unusedY_7bf54 = 0.0;
	gp->earthquake_x = 0.0;
	gp->earthquake_y = 0.0;
	
	for (int i = 0; i < 1000; i++) {
		gp->rategraph[0].val[i] = 0;
		gp->rategraph[1].val[i] = 0;
	}
	gp->rategraph[0].cursor = 0;
	gp->rategraph[1].cursor = 0;

	snd->param.stageBgmVolume[1] = 0.0;
	snd->param.stageBgmVolume[2] = 0.0;
	snd->param.stageBgmVolume[3] = 0.0;
	snd->param.stageBgmVolume[4] = 0.0;
	snd->param.stageBgmVolume[0] = 1.0;
	snd->param.stageKeyVolume[1] = 1.0;
	snd->param.stageKeyVolume[2] = 1.0;
	snd->param.stageKeyVolume[3] = 1.0;
	snd->param.stageKeyVolume[4] = 1.0;
	snd->param.stageKeyVolume[0] = 1.0;
	ErrorLogAdd("リトライ用の初期化を行いました\n");
	return 1;
}


//4ad7e0
double RealTimeToBMSTime(gameplay *gp, double time){

	if (time <= gp->bpmt_data[0].realtime) {
		return gp->bpmt_data[0].converted;
	}

	for (int i = gp->bpmt_start; i < gp->bpmt_count; i++) {
		if (gp->bpmt_data[i-1].realtime <= time && time <= gp->bpmt_data[i].realtime) {
			if (gp->bpmt_data[i].converted == gp->bpmt_data[i-1].converted) return gp->bpmt_data[i-1].converted; //can be skipped this line. they have a same result
			return ChangeValueByTime(gp->bpmt_data[i - 1].converted, gp->bpmt_data[i].converted, gp->bpmt_data[i - 1].realtime, gp->bpmt_data[i].realtime, time, 0);
		}
	}
	return gp->bpmt_data[gp->bpmt_count - 1].converted;
}

//4ad8a0
int CMP_CCARRbyCount(const void *p1, const void *p2) {
	struct_0x14* s1 = (struct_0x14*)p1;
	struct_0x14* s2 = (struct_0x14*)p2;
	if (s2->count == s1->count) {
		return s1->ID - s2->ID;
	}
	return s2->count - s1->count;
}

//4ad8c0
int CMP_CCARRbyID(const void *p1, const void *p2) {
	struct_0x14* s1 = (struct_0x14*)p1;
	struct_0x14* s2 = (struct_0x14*)p2;
	return s1->ID - s2->ID;
}

//4ad8d0
int SplitNotesToDP(LaneStruct *lane, int start, CHARTCONVERTER *cc, int end) {

	for (int i = start; i < lane->count; i++) {
		if (lane->notes[i].op == 2) break;

		if (11 <= lane->notes[i].op && lane->notes[i].op <= 17) {
			if (cc->flagSplit) {
				lane->notes[i].op += 10;
			}
			cc->flagSplit ^= 1; //xor
		}

		if (i == end) break;
	}
	return 1;
}

//4ad940
int RightLaneTo2P(LaneStruct *lane, int start, CHARTCONVERTER *cc) {

	int op = 0;
	//it may work as original code. maybe
	for (int i = 6; i >= 0; i--) {
		if (cc->noteCountPerLane[i] > 0) {
			op = 11 + i;
			break;
		}
	}
	
	for (int i = start; i < lane->count; i++) {
		if (lane->notes[i].op == 2) break;
		if (lane->notes[i].op == op) {
			lane->notes[i].op += 10;
		}
	}

	return 1;
}

//4ada00 //move 3rd note lane or 2nd lane(when only 2 lane)
int Move3rdLaneTo2P(LaneStruct *lane, int start, CHARTCONVERTER *cc) {

	int laneA = 0, laneB = 0, laneC = 0, laneD = 0;

	for (int i = 0; i < 7; i++) {
		if (cc->noteCountPerLane[i] > 0) {
			if (laneA == 0) {
				laneA = 11 + i;
			}
			else if (laneB == 0) {
				laneB = 11 + i;
			}
			else if (laneC == 0) {
				laneC = 11 + i;
				laneD = 11 + i;
			}
		}
	}

	for (int i = start; i < lane->count; i++) {
		if (lane->notes[i].op == 2) break;
		if ( (lane->notes[i].op == laneB && laneC < laneA) || lane->notes[i].op == laneD) {
			lane->notes[i].op += 10;
		}
	}

	return 1;
}

//4adab0
int DPsplitLane(LaneStruct *lane, int start, CHARTCONVERTER *cc) {

	int laneNoteCount[8] = { 0, };
	int laneCount = 0;
	int totalNoteCount = 0;

	for (int i = start; i < lane->count; i++) {
		if (lane->notes[i].op == 2) break;
		if (10 <= lane->notes[i].op && lane->notes[i].op <= 17) {
			laneNoteCount[lane->notes[i].op - 10]++; //maybe right
		}
		else if (21 <= lane->notes[i].op && lane->notes[i].op <= 27){
			lane->notes[i].op -= 10;
			laneNoteCount[lane->notes[i].op - 10]++; //maybe right
		}
	}

	
	for (int i = 1; i < 8; i++) {
		if (laneNoteCount[i] > 0) {
			laneCount++;
		}
		totalNoteCount += laneNoteCount[i];
	}

	if (laneCount >= 4 || totalNoteCount == 1) {
		cc->unk1442c = 1; //TODO : rename
		SplitNotesToDP(lane, start, cc, -1);
		cc->unk14428 = 0;
	}
	else if (laneCount == 2) {
		cc->unk1442c = 2;
		RightLaneTo2P(lane, start, cc);
		cc->unk14428 = 0;
	}
	else if (laneCount == 3) {
		Move3rdLaneTo2P(lane, start, cc);
	}

	return 1;
}

//4adbf0
int DPsplit(LaneStruct *lane, int start, CHARTCONVERTER *cc) {

	int laneNoteCount[8] = { 0, };
	int laneCount = 0;

	for (int i = start; i < lane->count; i++) {
		if (lane->notes[i].op == 2) break;
		if ((11 <= lane->notes[i].op && lane->notes[i].op <= 17) || (21 <= lane->notes[i].op && lane->notes[i].op <= 27)) {
			if (cc->arr3[(int)lane->notes[i].val].soundLoadID == cc->arr2[0].ID) {
				if (21 <= lane->notes[i].op && lane->notes[i].op <= 27) {
					laneNoteCount[lane->notes[i].op - 20]++;
				}
				else {
					laneNoteCount[lane->notes[i].op - 10]++;
				}
			}
		}
	}

	for (int i = 0; i < 8; i++) {
		if (laneNoteCount[i] > 0) {
			laneCount++;
		}
	}

	if (laneCount > 3) {
		for (int i = start; i < lane->count; i++) {
			if (lane->notes[i].op == 2) break;
			if ((11 <= lane->notes[i].op && lane->notes[i].op <= 17) || (21 <= lane->notes[i].op && lane->notes[i].op <= 27)) {
				if (21 <= lane->notes[i].op && lane->notes[i].op <= 27)
					lane->notes[i].op -= 10;

				if (cc->arr3[(int)lane->notes[i].val].soundLoadID == cc->arr2[0].ID) {
					if (cc->flagSplit) {
						lane->notes[i].op += 10;
					}
					cc->flagSplit ^= 1;
				}
			}
		}
	}

	return 1;
}

//4add90
void MakeExtraChart(gameplay *gp, CHARTCONVERTER *cc) {  //test completed

	int notecount = 0;
	double endtime;
	qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);

	for (int i = 0; i < gp->bmsobj.count; i++) {
		if (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op <= 30) {
			notecount++;
			endtime = gp->bmsobj.notes[i].bmsTiming;
		}

		if (gp->keymode >= 10) {
			if (gp->bmsobj.notes[i].op == 10) {
				gp->bmsobj.notes[i].op = 1;
			}
			if (gp->bmsobj.notes[i].op == 20) {
				gp->bmsobj.notes[i].op = 1;
			}
		}
	}

	if (gp->extramode_level == 0) {
		if (notecount >= 1200) gp->extramode_level = 2;
		else if (notecount >= 1000) gp->extramode_level = 1;
		else gp->extramode_level = 0;
	}
	else if (gp->extramode_level == 1) {
		if (notecount >= 1000) gp->extramode_level = 2;
		else gp->extramode_level = 1;
	}
	else if (gp->extramode_level == 2) {
		gp->extramode_level = 2;
	}

	//find most used lane of sound
	int laneOfSound[1296];
	for (int i = 0; i < 1296; i++) {
		int maxLaneNotes = 0;
		int Lane[20] = { 0, };
		laneOfSound[i] = -1;

		for (int j = 0; j < gp->bmsobj.count; j++) {
			if (10 <= gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op <= 30) {
				if ((int)gp->bmsobj.notes[j].val == i) {
					Lane[gp->bmsobj.notes[j].op-10]++;
				}
			}
		}
		
		for (int j = 0; j < 20; j++) {
			if (maxLaneNotes < Lane[j]) {
				cc->arr3[i].field4_0x10 = j;
				laneOfSound[i] = j;
				cc->arr1[cc->arr3[i].soundLoadID].field4_0x10++;
				maxLaneNotes = Lane[j];
			}
		}
	}
	bool flagShuffle = 0;

	//set default lane for non-used sound with previous sound lane
	for (int i = 1; i < 1296; i++) {
		if (laneOfSound[i] == -1 && cc->arr1[cc->arr3[i].soundLoadID].field4_0x10 > 0) {
			
			int lane = laneOfSound[i-1];
			if (lane >= 1) {
				if (gp->keymode == 7 || gp->keymode == 14) {
					switch (lane) {
						case 1:
						case 6: //TOFIX : 6 to 1?
							laneOfSound[i] = 3;
							break;
						case 2:
							laneOfSound[i] = 4;
							break;
						case 3:
							laneOfSound[i] = 5;
							break;
						case 4:
							laneOfSound[i] = 6;
							break;
						case 5:
							laneOfSound[i] = 7;
							break;
						case 7:
							laneOfSound[i] = 2;
							break;
					}
				}
				else if (gp->keymode == 5 || gp->keymode == 10) {
					switch (lane) {
						case 1:
							laneOfSound[i] = 3;
							break;
						case 2:
							laneOfSound[i] = 4;
							break;
						case 3:
							laneOfSound[i] = 5;
							break;
						case 4:
							laneOfSound[i] = 1;
							break;
						case 5:
							laneOfSound[i] = 2;
							break;
					}
				}
				else if (gp->keymode == 9) {
					switch (lane) {
						case 1:
							laneOfSound[i] = 3;
							break;
						case 2:
							laneOfSound[i] = 5;
							break;
						case 3:
							laneOfSound[i] = 7;
							break;
						case 4:
							laneOfSound[i] = 9;
							break;
						case 5:
							laneOfSound[i] = 2;
							break;
						case 6:
							laneOfSound[i] = 4;
							break;
						case 7:
							laneOfSound[i] = 6;
							break;
						case 8:
							laneOfSound[i] = 8;
							break;
						case 9:
							laneOfSound[i] = 1;
							break;
					}
				}
			}
			else {
				laneOfSound[i] = 7;
			}
		}
	}

	
	int laneA[20] = { 0, };
	int cur = 0;

	int mingap = 0;
	if (gp->BPM_fix > 0) {
		if (gp->extramode_level == 0) {
			mingap = 60000.0 / gp->BPM_fix;
		}
		if (gp->extramode_level == 1) {
			mingap = 45000.0 / gp->BPM_fix;
		}
		if (gp->extramode_level == 2) {
			mingap = 30000.0 / gp->BPM_fix;
		}

		if (mingap < 125) mingap = 125;
	}

	double lastRealTiming = 0.0;
	for (int i = 0; i < gp->bmsobj.count; i++) {
		if (gp->bmsobj.notes[i].realTiming == lastRealTiming) {
			if (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30) {
				laneA[gp->bmsobj.notes[i].op-10] = 2;
			}
			if (gp->bmsobj.notes[i].op == 3 || gp->bmsobj.notes[i].op == 8) {
				mingap = 30000.0 / gp->bmsobj.notes[i].val;
				if (mingap < 125) mingap = 125;
			}
		}
		else {
			
			for (int j = cur-1; j >= 0; j--) {
				if (mingap <= lastRealTiming - gp->bmsobj.notes[j].realTiming) break;

				if (10 <= gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op < 30 && laneA[gp->bmsobj.notes[j].op - 10] == 0) {
					laneA[gp->bmsobj.notes[j].op - 10] = 1;
				}
			}

			for (int j = i; j < gp->bmsobj.count; j++) {
				if (mingap <= gp->bmsobj.notes[j].realTiming - lastRealTiming) break;

				if (10 <= gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op < 30 && laneA[gp->bmsobj.notes[j].op - 10] == 0) {
					laneA[gp->bmsobj.notes[j].op - 10] = 1;
				}
			}

			for (int j = cur; j <= i-1; j++) {

				gp->bmsobj.notes[j].bmsTiming;
				if (gp->bmsobj.notes[j].op == 1 && gp->bmsobj.notes[j].bmsTiming >= 0) {
					
					int newLane = laneOfSound[(int)gp->bmsobj.notes[j].val];

					if (0 <= newLane && newLane < 20) {

						if (gp->keymode == 10 || gp->keymode == 14) { //TODO : need test at DP
							if (flagShuffle == 0) {
								newLane += newLane / 10 * (-10) + 10;
							}
							else {
								newLane += newLane / (-10) * 10;
							}
							flagShuffle ^= 1;
						}

						if (laneA[newLane] == 0 && (newLane%10 <= gp->keymode % 10)) {
							laneA[newLane] = 2;
							gp->bmsobj.notes[j].op = newLane + 10;
						}
						else if (newLane != 0 && newLane != 10) {
							int shift = 1;
							if ((int)gp->bmsobj.notes[j].val % 2 == 0) {
								shift = -1;
							}
							if (gp->keymode < 10) {
								for (int k = 1; k < gp->keymode+1; k++) {
									int next = newLane + k*shift;
									int prev = newLane - k*shift;

									if (1 <= next && next <= gp->keymode && laneA[next] == 0) {
										laneA[next] = 2;
										gp->bmsobj.notes[j].op = next + 10;
										break;
									}
									if (1 <= prev && prev <= gp->keymode && laneA[prev] == 0) {
										laneA[prev] = 2;
										gp->bmsobj.notes[j].op = prev + 10;
										break;
									}
								}
							}
							else {
								for (int k = 1; k < gp->keymode / 2 + 1; k++) {
									int next = newLane + k * shift;
									int prev = newLane - k * shift;

									if (1 <= next % 10 && next % 10 <= gp->keymode / 2 && laneA[next] == 0) {
										laneA[next] = 2;
										gp->bmsobj.notes[j].op = next + 10;
										break;
									}
									if (1 <= prev % 10 && prev % 10 <= gp->keymode / 2 && laneA[prev] == 0) {
										laneA[prev] = 2;
										gp->bmsobj.notes[j].op = prev + 10;
										break;
									}

									if (newLane <= 10) newLane += 10;
									else newLane -= 10;

									next = newLane + k * shift;
									prev = newLane - k * shift;

									if (1 <= next % 10 && next % 10 <= gp->keymode / 2 && laneA[next] == 0) {
										laneA[next] = 2;
										gp->bmsobj.notes[j].op = next + 10;
										break;
									}
									if (1 <= prev % 10 && prev % 10 <= gp->keymode / 2 && laneA[prev] == 0) {
										laneA[prev] = 2;
										gp->bmsobj.notes[j].op = prev + 10;
										break;
									}
								}
							}
						}
					}

				}
			}

			for (int j = 0; j < 20; j++) laneA[j] = 0;
			lastRealTiming = gp->bmsobj.notes[i].realTiming;
			cur = i;
		}

		if (gp->bmsobj.notes[i].bmsTiming > endtime) break;
	}

	qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
}

//4ae8d0
void DPtoSP(gameplay *gp) { //test completed

	int mingap;
	char laneA[10], laneB[10];

	qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
	
	if (gp->BPM_fix <= 0.0 || (int)(30000.0 / gp->BPM_fix) < 125)
		mingap = 125;
	else
		mingap = (30000.0 / gp->BPM_fix);

	for (int i = 0; i < gp->bmsobj.count; i++) {

		int op = gp->bmsobj.notes[i].op;
		if (op == 3 || op == 8) {
			mingap = (int)(30000.0 / gp->bmsobj.notes[i].val);
			if (mingap < 125) mingap = 125;
		}
		else if (20 <= op && op <= 29) {

			int newop = op - 10;
			bool fSameLane = 0, fSameTime = 0;
			memset(laneA, 0, sizeof(laneA));

			for (int prev = i - 1; prev >= 0; prev--) {
				if (10 <= gp->bmsobj.notes[prev].op && gp->bmsobj.notes[prev].op <= 19) {
					laneA[gp->bmsobj.notes[prev].op - 10] = 1;
					if (gp->bmsobj.notes[prev].bmsTiming == gp->bmsobj.notes[i].bmsTiming) {
						laneB[gp->bmsobj.notes[prev].op - 10] = 1;
					}
				}
				if (gp->bmsobj.notes[prev].op == newop) {
					fSameLane = 1;
					if (gp->bmsobj.notes[prev].bmsTiming == gp->bmsobj.notes[i].bmsTiming) {
						fSameTime = 1;
					}
				}
				if (gp->bmsobj.notes[i].realTiming - gp->bmsobj.notes[prev].realTiming >= mingap) break;
			}
			

			for (int next = i+1; next < gp->bmsobj.count; next++) {
				if (10 <= gp->bmsobj.notes[next].op && gp->bmsobj.notes[next].op <= 19) {
					laneA[gp->bmsobj.notes[next].op - 10] = 1;
					if (gp->bmsobj.notes[next].bmsTiming == gp->bmsobj.notes[i].bmsTiming) {
						laneB[gp->bmsobj.notes[next].op - 10] = 1;
					}
				}
				if (gp->bmsobj.notes[next].op == newop) {
					fSameLane = 1;
					if (gp->bmsobj.notes[next].bmsTiming == gp->bmsobj.notes[i].bmsTiming) {
						fSameTime = 1;
					}
				}
				if (gp->bmsobj.notes[next].realTiming - gp->bmsobj.notes[i].realTiming >= mingap) break;
			}

			if (op == 20) {
				if (fSameTime == 0) {
					gp->bmsobj.notes[i].op = gp->bmsobj.notes[i].op - 10;
				}
			}
			else if (fSameLane == 0) {
				gp->bmsobj.notes[i].op = gp->bmsobj.notes[i].op - 10;
			}
			else {
				for (int j = 1; j < 8; j++) {
					if (11 <= newop + j && newop + j <= gp->keymode / 2 + 10){ //don't combine these IFs. notes will be different
						if (laneA[newop + j - 10] == 0) {
							gp->bmsobj.notes[i].op = newop + j;
							ErrorLogFmtAdd("移動後チャンネル%d\n", newop + j);
							break;
						}
					}
					else if (11 <= newop - j && newop - j <= gp->keymode / 2 + 10 && laneA[newop - j - 10] == 0) {
						gp->bmsobj.notes[i].op = newop - j;
						ErrorLogFmtAdd("移動後チャンネル%d\n", newop - j);
						break;
					}
				}
			}
			
			if (20 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op <= 29) {
				gp->bmsobj.notes[i].op = 1;
				ErrorLogFmtAdd("しまっちゃうノート\n");
			}
		}
	}
	qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
	return;
}

//4af080
void PMStoSP(gameplay *gp) { //test&fix completed

	int mingap;
	char laneA[10], laneB[10], laneC[10];
	int newLane, measureLaneCount;
	int emptyLane, emptyLaneL, emptyLaneR;
	int countLaneA;
	int left, right;

	//if (gp->BPM_fix > 0.0) {
	//	mingap = 30000.0 / gp->BPM_fix;
	//	if (mingap < 125) mingap = 125;
	//}
	//else mingap = 125;

	if (gp->BPM_fix <= 0.0 || (int)(30000.0 / gp->BPM_fix) < 125)
		mingap = 125;
	else
		mingap = (30000.0 / gp->BPM_fix);

	qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
	int prev = 0; //prevMeasureStart
	int measure = 0;
	memset(laneA, 0, sizeof(laneA));
	left = 9;
	right = 1;
	measureLaneCount = 0;

	for (int i = 0; i < gp->bmsobj.count; i++) {

		int op = gp->bmsobj.notes[i].op;
		if (op == 3 || op == 8) {
			mingap = (int)(30000.0 / gp->bmsobj.notes[i].val);
			if (mingap < 125) mingap = 125;
		}
		else if (op == 2 && measureLaneCount > 0) {
			measure++;

			countLaneA = 0;
			for (int j = 1; j <= 9; j++) {
				if (laneA[j]) countLaneA++;
			}

			if (right <= 7) {
				ErrorLogFmtAdd("%d:なにもしない\n", measure);
			}
			else if (right == 8 && left >= 2) {
				for (int j = prev; j < i; j++) {
					if (12 <= gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op <= 19) {
						gp->bmsobj.notes[j].op--;
					}
				}
				ErrorLogFmtAdd("%d:左シフト１\n", measure);
			}
			else if (right == 9 && left >= 3) {
				for (int j = prev; j < i; j++) {
					if (13 <= gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op <= 19) {
						gp->bmsobj.notes[j].op -= 2;
					}
				}
				ErrorLogFmtAdd("%d:左シフト2\n", measure);
			}
			else if (right == 9 && left == 2 && countLaneA <= 7) {
				ErrorLogFmtAdd("%d:左シフトしてレーンつめる 左%d 右%d\n", measure, 2, 9);
				for (int j = 2; j < 9; j++) {
					if (laneA[j] == 0) {
						emptyLane = j;
						break;
					}
				}
				ErrorLogFmtAdd("つめるレーンは%d\n", emptyLane);
				for (int j = prev; j < i; j++) {
					gp->bmsobj.notes[j].op;
					if (emptyLane + 10 < gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op <= 19) {
						gp->bmsobj.notes[j].op--;
					}
				}
				ErrorLogFmtAdd("さらに全体左シフト\n");
				for (int j = prev; j < i; j++) {
					gp->bmsobj.notes[j].op;
					if (11 <= gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op <= 19) {
						gp->bmsobj.notes[j].op--;
					}
				}
			}
			else if (left == 1 && countLaneA <= 7) {
				ErrorLogFmtAdd("%d:レーンつめる 左%d 右%d\n", measure, 1, right);

				if (right == 8) {
					for (int j = 1; j < 9; j++) {
						if (laneA[j] == 0) {
							emptyLane = j;
							break;
						}
					}
					ErrorLogFmtAdd("つめるレーンは%d\n", emptyLane);
					for (int j = prev; j < i; j++) {
						if (10 + emptyLane < gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op <= 19) {
							gp->bmsobj.notes[j].op--;
						}
					}
				}
				else {
					for (int j = 1; j < 9; j++) {
						if (laneA[j] == 0) {
							emptyLaneL = j;
							break;
						}
					}
					for (int j = 9; j > 1; j--) {
						if (laneA[j] == 0) {
							emptyLaneR = j;
							break;
						}
					}
					ErrorLogFmtAdd("つめるレーンは%dと%d\n", emptyLaneL, emptyLaneR);
					for (int j = prev; j < i; j++) {
						gp->bmsobj.notes[j].op;
						if (emptyLaneL + 10 < gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op <= 19) {
							gp->bmsobj.notes[j].op--;
						}
					}
					for (int j = prev; j < i; j++) {
						gp->bmsobj.notes[j].op;
						if (emptyLaneR + 9 < gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op <= 19) {
							gp->bmsobj.notes[j].op--;
						}
					}
				}
			}
			else {
				ErrorLogFmtAdd("%d:89移動\n", measure);
				if (left == 2) {
					for (int j = prev; j < i; j++) {
						if (12 <= gp->bmsobj.notes[j].op && gp->bmsobj.notes[j].op <= 19) {
							gp->bmsobj.notes[j].op--;
						}
					}
				}

				for (int j = prev; j < i; j++) {
					left = gp->bmsobj.notes[j].op;
					if (left == 18 || left == 19) {
						bool fSameLane = 0;
						memset(laneB, 0, sizeof(laneB));
						newLane = 14 + (left - 18);

						for (int x = j - 1; x >= 0; x--) {
							if (10 <= gp->bmsobj.notes[x].op && gp->bmsobj.notes[x].op <= 19) {
								laneB[gp->bmsobj.notes[x].op - 10] = 1;
								if (gp->bmsobj.notes[x].bmsTiming == gp->bmsobj.notes[j].bmsTiming) {
									laneC[gp->bmsobj.notes[x].op - 10] = 1;
								}
							}
							if (gp->bmsobj.notes[x].op == newLane) {
								fSameLane = 1;
							}
							if (gp->bmsobj.notes[j].realTiming - gp->bmsobj.notes[x].realTiming >= mingap) break;
						}


						for (int next = j + 1; next < gp->bmsobj.count; next++) {
							if (10 <= gp->bmsobj.notes[next].op && gp->bmsobj.notes[next].op <= 19) {
								laneB[gp->bmsobj.notes[next].op - 10] = 1;
								if (gp->bmsobj.notes[next].bmsTiming == gp->bmsobj.notes[j].bmsTiming) {
									laneC[gp->bmsobj.notes[next].op - 10] = 1;
								}
							}
							if (gp->bmsobj.notes[next].op == newLane) {
								fSameLane = 1;
							}
							if (gp->bmsobj.notes[next].realTiming - gp->bmsobj.notes[j].realTiming >= mingap) break;
						}

						if (fSameLane) {
							fSameLane = 0;
							//looks weird, but really doing this
							for (int k = 1; k < 5; k++) {
								int L = newLane + k * 2;
								if (11 <= L && L <= 17) {
									if (laneB[L - 10] == 0) {
										gp->bmsobj.notes[j].op = L;
										ErrorLogFmtAdd("移動後チャンネル%d\n", L);
										fSameLane = 1;
										break;
									}

								}
								else {
									L = newLane - k * 2;
									if (11 <= L && L <= 17 && laneB[L - 10] == 0) {
										gp->bmsobj.notes[j].op = L;
										ErrorLogFmtAdd("移動後チャンネル%d\n", L);
										fSameLane = 1;
										break;
									}
								}
							}
							if (!fSameLane) {
								for (int k = 1; k < 5; k++) {
									int L = newLane + k * 2 - 1;
									if (11 <= L && L <= 17) {
										if (laneB[L - 10] == 0) {
											gp->bmsobj.notes[j].op = L;
											ErrorLogFmtAdd("移動後チャンネル%d\n", L);
											break;
										}
									}
									else {
										L = newLane - k * 2 + 1;
										if (11 <= L && L <= 17 && laneB[L - 10] == 0) {
											gp->bmsobj.notes[j].op = L;
											ErrorLogFmtAdd("移動後チャンネル%d\n", L);
											break;
										}
									}
								}
							}
						}
						else {
							gp->bmsobj.notes[j].op = newLane;
						}

						if (gp->bmsobj.notes[j].op == 18 || gp->bmsobj.notes[j].op == 19) {
							gp->bmsobj.notes[j].op = (gp->bmsobj.notes[j].mine <= 0) ? 1 : -1;
							ErrorLogFmtAdd("しまっちゃうノート\n");
						}
					}
				}
			}
			measureLaneCount = 0;
			prev = i;
			left = 9;
			right = 1;
			memset(laneA, 0, sizeof(laneA));
		}
		else if (op == 2 && measureLaneCount == 0) {
			measure++;

			left = 9;
			right = 1;
			prev = i;
			memset(laneA, 0, sizeof(laneA));
		}
		else if (11 <= op && op <= 19) {
			laneA[op - 10] = 1;
			measureLaneCount++;
			if (op - 10 < left) {
				left = op - 10;
			}
			if (right < op - 10) {
				right = op - 10;
			}
		}
	}

	qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
	measure = 0;
	for (int i = 0; i < gp->bmsobj.count; i++) {
		if (gp->bmsobj.notes[i].op == 2)
			measure++;

		if(gp->bmsobj.notes[i].op == 18 || gp->bmsobj.notes[i].op == 19)
			ErrorLogFmtAdd("###########################################################\n移動できてな いノート 小節%d チャンネル%d\n", measure, gp->bmsobj.notes[i].op);

		if(gp->bmsobj.notes[i].op == 10)
			ErrorLogFmtAdd("###########################################################\n空気を読まな いスクラッチ 小節%d チャンネル%d\n", measure, 10);
	}

	return;
}

//4afe40
int DPsplitLaneScratch(LaneStruct *lane, int start, CHARTCONVERTER *cc) {

	int nNotesP1 = 0;
	int nNotesP2 = 0;
	int nNotesSC = 0;

	for (int i = start; i < lane->count; i++) {
		int op = lane->notes[i].op;
		
		if (op == 2) break;

		if (11 <= op && op <= 17) {
			nNotesP1++;
		}
		else if (21 <= op && op <= 27) {
			nNotesP2++;
		}
		else if (op == 10) {
			nNotesSC++;
		}
	}

	if ((nNotesP1 > 0 && nNotesP2 == 0) || (nNotesP1 == 0 && nNotesP2 > 0)) {
		DPsplitLane(lane, start, cc);
	}

	double realTimeLastNote = -1.0;
	int scratchNoteID = -1, oldScratchNoteID = -1;
	int LaneA[20];

	for (int i = start; i < lane->count; i++) {

		if (lane->notes[i].realTiming > realTimeLastNote) {
			realTimeLastNote = lane->notes[i].realTiming;
			if (scratchNoteID != -1) {

				if (cc->RealTimingSplitScratch + 500 < lane->notes[scratchNoteID].realTiming && (nNotesSC <= 3 || cc->RealTimingSplitScratch == -1))
					cc->flagSplitScratch ^= 1;

				cc->RealTimingSplitScratch = lane->notes[scratchNoteID].realTiming;


				if (cc->flagSplitScratch == 0) {
					if (cc->assist1p == 0) {
						for (int j = scratchNoteID; j > 0; j--) {
							if (lane->notes[j].op == 2) break;
							if (cc->RealTimingSplitScratch - lane->notes[j].realTiming >= 200) break;
							if (12 <= lane->notes[j].op && lane->notes[j].op <= 17) lane->notes[j].op += 10;
						}

						for (int j = scratchNoteID; j < lane->count; j++) {
							if (lane->notes[j].op == 2) break;
							if (lane->notes[j].realTiming - cc->RealTimingSplitScratch >= 200) break;
							if (12 <= lane->notes[j].op && lane->notes[j].op <= 17) lane->notes[j].op += 10;
						}
					}
				}
				else {
					if (cc->assist2p == 0) {
						for (int j = scratchNoteID; j > 0; j--) {
							if (lane->notes[j].op == 2) break;
							if (cc->RealTimingSplitScratch - lane->notes[j].realTiming >= 200) break;
							if (21 <= lane->notes[j].op && lane->notes[j].op <= 26) lane->notes[j].op -= 10;
						}

						for (int j = scratchNoteID; j < lane->count; j++) {
							if (lane->notes[j].op == 2) break;
							if (lane->notes[j].realTiming - cc->RealTimingSplitScratch >= 200) break;
							if (21 <= lane->notes[j].op && lane->notes[j].op <= 26) lane->notes[j].op -= 10;
						}
					}
				}
				scratchNoteID = -1;
				oldScratchNoteID = -1;
			}
		}

		if (lane->notes[i].op == 2) break;

		if (lane->notes[i].op == 10) {
			scratchNoteID = i;
			oldScratchNoteID = i;
		}
		if ( (11 <= lane->notes[i].op && lane->notes[i].op <=17) || (21 <= lane->notes[i].op && lane->notes[i].op <= 27)) {
			LaneA[lane->notes[i].op - 10] = i;
			scratchNoteID = oldScratchNoteID;
		}
	}
	cc->RealTimingSplitScratch = -1;
	return 1;
}

//4b0250
int SPtoDP(LaneStruct *lane, int baseNoteID, CHARTCONVERTER *cc) {

	cc->unused14404 = 0;
	for (int i = 0; i < 7; i++) cc->noteCountPerLane[i] = 0;
	cc->laneCount = 0;

	for (int i = 0; i < 1296; i++) {
		cc->arr1[i].count = 0;
		cc->arr1[i].field3_0xc = -1;
		cc->arr3[i].field3_0xc = -1;
	}
	for (int i = baseNoteID + 1; i < lane->count; i++) {
		if (lane->notes[i].op == 2) break;
		if (10 <= lane->notes[i].op && lane->notes[i].op <= 17) {
			cc->noteCountPerLane[lane->notes[i].op - 11]++;
			if (lane->notes[i].op != 10) {
				cc->arr1[cc->arr3[lane->notes[i].op].soundLoadID].count++;
				cc->arr1[cc->arr3[lane->notes[i].op].soundLoadID].field3_0xc = 0;
			}
		}
	}

	for (int i = 0; i < 7; i++) {
		if (cc->noteCountPerLane[i]) cc->laneCount++;
	}

	qsort(&cc->arr1, 1296, sizeof(cc->arr1), CMP_CCARRbyCount);

	bool fA = false, fB = false;
	if (cc->arr2[0].count && cc->arr1[0].count&& ((cc->arr1[0].ID == cc->arr2[0].ID && (cc->arr1[1].ID == cc->arr2[1].ID || cc->arr1[1].ID == cc->arr2[2].ID || cc->arr1[2].ID == cc->arr2[1].ID || cc->arr1[2].ID == cc->arr2[2].ID)) 
													|| (cc->arr1[0].ID == cc->arr2[1].ID && cc->arr1[1].ID == cc->arr2[0].ID) 
													|| (cc->arr1[1].ID == cc->arr2[0].ID && cc->arr2[1].ID == 0))) {
		cc->unk14428++;
		fB = true;

		if (cc->unk14428 == 4){
			fA = true;
			cc->unk14428 = 0;
		}
	}
	else {
		cc->unk14428 = 0;
	}

	qsort(&cc->arr2,1296,sizeof(cc->arr2), CMP_CCARRbyID);
	if (fA) {
		for (int i = 0; i < 1296; i++) {
			if (cc->arr2[i].field3_0xc == 0) cc->arr2[i].field3_0xc = 1;
			else if (cc->arr2[i].field3_0xc == 1) cc->arr2[i].field3_0xc = 0;
		}
	}

	int unkArr[2] = { 0,0 };
	for (int i = 0; i < 1296; i++) {
		if (cc->arr1[i].count > 0) {
			if (fB && cc->arr2[cc->arr1[i].ID].field3_0xc != -1 && i == 0) {
				cc->arr2[cc->arr1[i].ID].field3_0xc;
				unkArr[cc->arr2[cc->arr1[0].ID].field3_0xc] += cc->arr1[0].count; //really 0. not i
				cc->arr1[0].field3_0xc = cc->arr2[cc->arr1[i].ID].field3_0xc;
			}
			else if (unkArr[0] == 0 && unkArr[1] == 0) {
				if (cc->flagSplitUnknown == 0) {
					cc->arr1[i].field3_0xc = 0;
					unkArr[0] = cc->arr1[i].count;
				}
				else {
					cc->arr1[i].field3_0xc = 1;
					unkArr[1] = cc->arr1[i].count;
				}
				cc->flagSplitUnknown ^= 1;
			}
			else if (0 < 0) {
				cc->arr1[i].field3_0xc = 1;
				unkArr[1] += cc->arr1[i].count;
			}
			else {
				cc->arr1[i].field3_0xc = 0;
				unkArr[0] += cc->arr1[i].count;
			}
		}
	}
	
	for (int i = 0; i < 1296; i++) {
		cc->arr2[i].count = cc->arr1[i].count;
		cc->arr2[i].ID = cc->arr1[i].ID;
		cc->arr2[i].field3_0xc = cc->arr1[i].field3_0xc;
	}

	qsort(&cc->arr1, 1296, sizeof(cc->arr1), CMP_CCARRbyID);
	for (int i = baseNoteID + 1; i < lane->count; i++) {

		if (lane->notes[i].op == 2) break;
		if (11 <= lane->notes[i].op && lane->notes[i].op <= 17) {
			int t = cc->arr1[cc->arr3[(int)lane->notes[i].val].soundLoadID].field3_0xc;
			
			if (t == 0) {}
			else if (t == 1) {
				lane->notes[i].op += 10;
			}
			else if (t == -1) {
				lane->notes[i].op += 100;
				ErrorLogFmtAdd("-1エラー\n");
			}
		}

	}

	if (cc->arr2[0].count >= 17 
		|| (cc->arr2[0].count >= 16 && cc->playlevel <= 10) 
		|| (cc->arr2[0].count >= 14 && cc->playlevel <= 9) 
		|| (cc->arr2[0].count >= 12 && cc->playlevel <= 8)) {
		DPsplit(lane, baseNoteID + 1, cc);
	}
	DPsplitLaneScratch(lane, baseNoteID + 1, cc);
	return 1;
}

//4b0690
//TODO : rename variables
//TOFIX : freq +12 autoplay endtime doesn't match (#STOP?)
//TOFIX : nonstop mix retry volume issue
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

	//TOFIX : seed is not putted into replaydata, when use ghostbattle. (retry puts seed) (see also ProcS_Play())
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
	gp->fadeinSOUNDstart[0] = -1;
	gp->fadeinSOUNDend[0] = -1;
	gp->fadeoutSOUNDstart[0] = -1;
	gp->fadeoutSOUNDend[0] = -1;
	aud->param.stagePitch[1] = 1.0;
	aud->param.stageKeyVolume[1] = 1.0;
	aud->param.stageBgmVolume[1] = 0.0;
	gp->fadeinSOUNDstart[1] = -1;
	gp->fadeinSOUNDend[1] = -1;
	gp->fadeoutSOUNDstart[1] = -1;
	gp->fadeoutSOUNDend[1] = -1;
	aud->param.stagePitch[2] = 1.0;
	aud->param.stageKeyVolume[2] = 1.0;
	aud->param.stageBgmVolume[2] = 0.0;
	gp->fadeinSOUNDstart[2] = -1;
	gp->fadeinSOUNDend[2] = -1;
	gp->fadeoutSOUNDstart[2] = -1;
	gp->fadeoutSOUNDend[2] = -1;
	aud->param.stagePitch[3] = 1.0;
	aud->param.stageKeyVolume[3] = 1.0;
	aud->param.stageBgmVolume[3] = 0.0;
	gp->fadeinSOUNDstart[3] = -1;
	gp->fadeinSOUNDend[3] = -1;
	gp->fadeoutSOUNDstart[3] = -1;
	gp->fadeoutSOUNDend[3] = -1;
	aud->param.stagePitch[4] = 1.0;
	aud->param.stageKeyVolume[4] = 1.0;
	aud->param.stageBgmVolume[4] = 0.0;
	gp->fadeinSOUNDstart[4] = -1;
	gp->fadeinSOUNDend[4] = -1;
	gp->fadeoutSOUNDstart[4] = -1;
	gp->fadeoutSOUNDend[4] = -1;
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
	double prevNoteBmstime = 0.0;
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
	if (gp->courseType != 1 || gp->isCourse == 0) {
		stages = 1;
	}
	endtime = 0.0;

	//TOFIX : in nonstop mode(courseType==1), gp->courseConnection[stage - 1] doesn't check if stage >= 1. It can affects at #BPM
	/* start of stage loop */
	for (int stage = 0; stage < stages; stage++) {
		//
		int oldbpmtCount = gp->bpmt_count;
		float firstNoteTime = -1.0;
		bmsobj_stageFirst = gp->bmsobj.count;
		prevStageLastNoteTime = (int)lastNoteTime;
		lastNoteTime = lastMeasure;
		gp->freqSpeedMultiplier = oldSpeedMultiplier;
		int stageStartMeasure = lastMeasure;
		if (gp->isCourse) {
			if (gp->courseType == 1) {
				filename = gp->courseFilepath[stage];
				if (gp->courseConnection[stage - 1] == 4 || gp->courseConnection[stage - 1] == 5) { //BLANK1, BLANK2
					gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = lastMeasure;
					gp->bmsobj.notes[gp->bmsobj.count].val = stage;
					gp->bmsobj.notes[gp->bmsobj.count].op = 1002;
					gp->bmsobj.count++;
					if (gp->bmsobj.count == gp->bmsobj.size) ExpandNoteBuffer(&gp->bmsobj, 1000);
				}
				gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = lastMeasure;
				gp->bmsobj.notes[gp->bmsobj.count].val = stage;
				gp->bmsobj.notes[gp->bmsobj.count].op = 1003;
				gp->bmsobj.count++;
				if (gp->bmsobj.count == gp->bmsobj.size) ExpandNoteBuffer(&gp->bmsobj, 1000);
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
		if (isPMS = filename.right(4).lower().isSame(".pms"); isPMS) {
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
				else if (channel == 3) { //Change of BPM 	BPM 1 ? [01-FF] ? BPM 255
					int div = (fBuf.length() - 7) / 2;
					for (int i = 0; i < div; i++) {
						int ii = i * 2 + 7;
						if (HEXcharToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1))) {
							float notepos = i / (float)div; //do not simplify this. float80 - float32 noise is in original LR2
							gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = (int)thisMeasure + notepos;
							gp->bmsobj.notes[gp->bmsobj.count].val = HEXcharToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1)) * gp->freqSpeedMultiplier;
							gp->bmsobj.notes[gp->bmsobj.count].op = 3;
							gp->bmsobj.count++;
							if (gp->bmsobj.count == gp->bmsobj.size) {
								ExpandNoteBuffer(&gp->bmsobj, 1000);
							}
						}
					}
				}
				else if (channel > 0) {
					int div = (fBuf.length() - 7) / 2;
					for (int i = 0; i < div; i++) {
						int ii = i * 2 + 7;
						if (Base36ToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1))) {
							float notepos = i / (float)div; //do not simplify this. float80 - float32 noise is in original LR2
							gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = (int)thisMeasure + notepos;
							if (isVisibleNote(channel)) {
								if (lastMeasure <= thisMeasure) {
									lastMeasure = thisMeasure;
								}
							}

							if (isVisibleNote(channel) && firstNoteTime == -1.0) {
								firstNoteTime = gp->bmsobj.notes[gp->bmsobj.count].bmsTiming;
							}
							else if (gp->bmsobj.notes[gp->bmsobj.count].bmsTiming < firstNoteTime) { //visible no check?
								firstNoteTime = gp->bmsobj.notes[gp->bmsobj.count].bmsTiming;
							}

							if (isVisibleNote(channel) && lastNoteTime < gp->bmsobj.notes[gp->bmsobj.count].bmsTiming) {
								lastNoteTime = gp->bmsobj.notes[gp->bmsobj.count].bmsTiming;
							}

							if (channel == 8 || channel == 9) { //BPM, STOP
								gp->bmsobj.notes[gp->bmsobj.count].val = Base36ToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1));
							}
							else {
								gp->bmsobj.notes[gp->bmsobj.count].val = Base36ToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1)) + stage * 1296;
							}
							gp->bmsobj.notes[gp->bmsobj.count].op = channel;
							gp->bmsobj.count++;
							if (gp->bmsobj.count == gp->bmsobj.size) ExpandNoteBuffer(&gp->bmsobj, 1000);

							if (((10 <= channel && channel < 20) || (30 <= channel && channel < 40) || (50 <= channel && channel < 60)) && (meta->keymode < 10 && ((cfg->play.battle == 1 && (cfg->play.random[0] != cfg->play.random[1])) || cfg->play.battle == 2))) {
								gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = (int)thisMeasure + notepos;
								gp->bmsobj.notes[gp->bmsobj.count].val = Base36ToInt(*fBuf.atPos(ii), *fBuf.atPos(ii + 1)) + stage * 1296;
								gp->bmsobj.notes[gp->bmsobj.count].op = channel + 10;
								gp->bmsobj.count++;
								if (gp->bmsobj.count == gp->bmsobj.size) ExpandNoteBuffer(&gp->bmsobj, 1000);
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
				if (stage > 0 && (gp->courseConnection[stage - 1] == 1 || gp->courseConnection[stage - 1] == 3)) { //FIT
					gp->freqSpeedMultiplier *= nowBPM / val;
					aud->param.stagePitch[stage] = nowBPM / val;
					ErrorLogFmtAdd("BPMを前の曲に合わせました。 stagepitch %d\n", nowBPM / val * 100.0);
				}
				nowBPM = val * gp->freqSpeedMultiplier;
				if (stage > 0) {
					gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = (double)stageStartMeasure;
					gp->bmsobj.notes[gp->bmsobj.count].val = val * gp->freqSpeedMultiplier;
					gp->bmsobj.notes[gp->bmsobj.count].op = 3;
					gp->bmsobj.count++;
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
				gp->bpmt_count++; //TOFIX: possibility of writing over allocated memory
				if (gp->bmsobj.count == gp->bmsobj.size) {
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
					gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = stageStartMeasure;
					gp->bmsobj.notes[gp->bmsobj.count].val = atof(fBuf.right(fBuf.length() - 6));
					gp->bmsobj.notes[gp->bmsobj.count].op = 1001;
					gp->bmsobj.count++;
					if (gp->bmsobj.count == gp->bmsobj.size) {
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
					if (gp->isCourse == 0 && stage * 1296 + param1 < 1296) {
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

		if (gp->soundonly && gp->isCourse == 0) {
			gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = stageStartMeasure;
			gp->bmsobj.notes[gp->bmsobj.count].val = 1295.0;
			gp->bmsobj.notes[gp->bmsobj.count].op = 4;
			gp->bmsobj.count++;
			if (gp->bmsobj.count == gp->bmsobj.size) ExpandNoteBuffer(&gp->bmsobj, 1000);
		}
		for (int i = stageStartMeasure; i < stageStartMeasure + 1000; i++) {
			gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = i;
			gp->bmsobj.notes[gp->bmsobj.count].val = measureLength[i];
			gp->bmsobj.notes[gp->bmsobj.count].op = 2;
			gp->bmsobj.count++;
			if (gp->bmsobj.count == gp->bmsobj.size) ExpandNoteBuffer(&gp->bmsobj, 1000);
		}

		if (bmsobj_stageFirst < gp->bmsobj.count) {
			qsort(&gp->bmsobj.notes[bmsobj_stageFirst], gp->bmsobj.count - bmsobj_stageFirst, sizeof(NoteStruct), CMP_NotesByBmsTiming);
		}

		for (int i = 0; i < gp->bmsobj.count && gp->bmsobj.notes[i].bmsTiming == 0; i++) {
			if ((10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 29) || (30 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 49)) {
				for (int j = 0; j < gp->bmsobj.count; j++) {
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
		for (int i = gp->bmsobj.count - 1; bmsobj_stageFirst <= i; i--) {
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
		for (int i = objNumLastNote; i < gp->bmsobj.count; i++) {
			if (gp->bmsobj.notes[i].op == 2) {
				objNumLastMeasureEarly = i;
				break; //i = gp->bmsobj.count;
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
		for (int i = bmsobj_stageFirst; i < gp->bmsobj.count; i++) {
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
		for (int i = objNumFirstNote; i < gp->bmsobj.count; i++) {
			if (gp->bmsobj.notes[i].op == 2) {
				objNumFirstMeasureEarly = i;
				break; //i = gp->bmsobj.count;
			}
		}
		int objNumFirstMeasure = objNumFirstMeasureLate;
		if (gp->bmsobj.notes[objNumFirstMeasureEarly].bmsTiming - gp->bmsobj.notes[objNumFirstNote].bmsTiming < gp->bmsobj.notes[objNumFirstNote].bmsTiming - gp->bmsobj.notes[objNumFirstMeasureLate].bmsTiming) {
			objNumFirstMeasure = objNumFirstMeasureEarly;
		}


		/* start of setting notes on time*/
		for (int i = bmsobj_stageFirst; i < gp->bmsobj.count; i++) {
			if (stage != 0 && gp->bmsobj.notes[i].bmsTiming <= 0) {
				ErrorLogFmtAdd("エラーノートを発見しました ch=%d\n", gp->bmsobj.notes[i].op);
			}

			if (gp->bmsobj.notes[i].bmsTiming != prevNoteBmstime) {
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
				
				double addRealtime = (240.0 / nowBPM * meaLength * (gp->bmsobj.notes[i].bmsTiming - prevNoteBmstime) * 1000.0);
				bpmt_realtime += addRealtime + stopRealtime;
				
				if (cfg->play.hsfix == 4 || (gp->isCourse && gp->courseType == 1)) {
					bpmt_bmstime += addRealtime * 1.2;
					prevNoteBmstime = gp->bmsobj.notes[i].bmsTiming;
					stopRealtime = 0.0;
				}
				else {
					bpmt_bmstime += meaLength * 1920.0 * (gp->bmsobj.notes[i].bmsTiming - prevNoteBmstime);
					prevNoteBmstime = gp->bmsobj.notes[i].bmsTiming;
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
			else if (unk23538_objNum == -1 && 0 < (int)unk23484_bmstime && gp->bmsobj.notes[i].op == 2 && unk2346c_realtime + 5000.0 < bpmt_realtime && gp->courseConnection[stage] == 5) { //BALNK2
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
				gp->bmsobj.notes[i].mine = gp->bmsobj.notes[i].val;
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

		//DEBUG : dump bmsobj
		//dump(meta->title,gp->bmsobj.notes,sizeof(NoteStruct) * gp->bmsobj.count);

		if (gp->courseConnection[stage] == 5 && unk23538_objNum != -1) { //BLANK2
			unk23484_bmstime = b2bmsTime;
			objNumLastMeasure = unk23538_objNum;
			unk2346c_realtime = b2realTime;
			bpmt_count = b2bpmt_last;
			lastMeasure = b2lastMeasure;
			_bPrevNoteTime = b2prevNoteTime;
		}

		if (cfg->play.battle == 3) {
			if (cfg->play.battle == 3 && meta->keymode == 9 && gp->isCourse == 0 && gp->isPreviewLoad == 0) {
				ErrorLogFmtAdd("PMSTOSPマージを行います");
				for (int cur = 0; cur < gp->bmsobj.count; cur++) {
					gp->bmsobj.notes[cur].bmsTiming_ln = gp->bmsobj.notes[cur].bmsTiming;
					gp->bmsobj.notes[cur].realTiming_ln = gp->bmsobj.notes[cur].realTiming;
				}
				PMStoSP(gp);
			}
			if (cfg->play.battle == 3 && (meta->keymode == 10 || meta->keymode == 14) && gp->isCourse == 0 && gp->isPreviewLoad == 0) { //TOFIX: cfg->play.battle==3 duplicated
				ErrorLogFmtAdd("DPTOSPマージを行います");
				for (int cur = 0; cur < gp->bmsobj.count; cur++) {
					gp->bmsobj.notes[cur].bmsTiming_ln = gp->bmsobj.notes[cur].bmsTiming;
					gp->bmsobj.notes[cur].realTiming_ln = gp->bmsobj.notes[cur].realTiming;
				}
				DPtoSP(gp);
			}
		}
		if (cfg->play.is_extra > 0 && gp->isCourse == 0 && gp->isPreviewLoad == 0) {
			gp->extramode_level = cfg->play.m_extra;
			for (int cur = 0; cur < gp->bmsobj.count; cur++) {
				gp->bmsobj.notes[cur].bmsTiming_ln = gp->bmsobj.notes[cur].bmsTiming;
				gp->bmsobj.notes[cur].realTiming_ln = gp->bmsobj.notes[cur].realTiming;
			}
			MakeExtraChart(gp, &cc);
		}
		if (cfg->play.m_addnote > 0 && gp->isCourse == 0 && gp->isPreviewLoad == 0) {

			for (int i = 0; i < gp->bmsobj.count; i++) {
				gp->bmsobj.notes[i].bmsTiming_ln = gp->bmsobj.notes[i].bmsTiming;
				gp->bmsobj.notes[i].realTiming_ln = gp->bmsobj.notes[i].realTiming;
			}
			qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);

			double l_realTiming = 0.0;
			double t_realTiming = 0.0;
			double t_bmsTiming = 0.0;
			memset(mapAdded, 0, 20);
			int addNoteCount[2] = { 0, };
			int key = 7;
			if (meta->keymode == 5 || meta->keymode == 10) key = 5;
			else if (meta->keymode == 9) key = 9;

			for (int i = 0; i < gp->bmsobj.count; i++) {
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
														gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = t_bmsTiming;
														gp->bmsobj.notes[gp->bmsobj.count].bmsTiming_ln = t_bmsTiming;
														gp->bmsobj.notes[gp->bmsobj.count].realTiming = t_realTiming;
														gp->bmsobj.notes[gp->bmsobj.count].realTiming_ln = t_realTiming;
														gp->bmsobj.notes[gp->bmsobj.count].val = 1294.0;
														gp->bmsobj.notes[gp->bmsobj.count].op = p * 10 + 10 + lane;
														gp->bmsobj.count++;
														if (gp->bmsobj.count == gp->bmsobj.size) {
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
			qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
		}
		if (cfg->play.m_loudness > 0 && gp->isCourse == 0 && gp->isPreviewLoad == 0) {

			for (int i = 0; i < gp->bmsobj.count; i++) {
				gp->bmsobj.notes[i].bmsTiming_ln = gp->bmsobj.notes[i].bmsTiming;
				gp->bmsobj.notes[i].realTiming_ln = gp->bmsobj.notes[i].realTiming;
			}
			qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);

			double l_realTiming = 0.0;
			double t_realTiming = 0.0;
			double t_bmsTiming = 0.0;
			bool mapAdded[2][10] = { 0, };
			int addNoteCount[2] = { 0, };
			int key = 7;
			if (meta->keymode == 5 || meta->keymode == 10) key = 5;
			else if (meta->keymode == 9) key = 9;

			for (int i = 0; i < gp->bmsobj.count; i++) {
				if (l_realTiming < gp->bmsobj.notes[i].realTiming) {
					for (int p = 0; p < 2; p++) {
						if (addNoteCount[p] != 0) {

							if (GetRand(100) <= cfg->play.m_loudness) {
								if (key > -1) {
									for (int lane = 0; lane <= key; lane++) {
										if (mapAdded[lane + p * 10] == 0) {
											gp->bmsobj.notes[gp->bmsobj.count].bmsTiming = t_bmsTiming;
											gp->bmsobj.notes[gp->bmsobj.count].bmsTiming_ln = t_bmsTiming;
											gp->bmsobj.notes[gp->bmsobj.count].realTiming = t_realTiming;
											gp->bmsobj.notes[gp->bmsobj.count].realTiming_ln = t_realTiming;
											gp->bmsobj.notes[gp->bmsobj.count].val = 1294.0;
											gp->bmsobj.notes[gp->bmsobj.count].op = p * 10 + 10 + lane;
											gp->bmsobj.count++;
											if (gp->bmsobj.count == gp->bmsobj.size) {
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
			qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
		}

		double realDiff = 0.0;
		double bmsDiff = 0.0;

		if (stage >= 1) {
			stageStartMeasure = (int)firstNoteTime - stageStartMeasure - 1;

			if (gp->courseConnection[stage - 1] == 5) { //BLANK2
				stageStartMeasure = 0;
			}
			else if (gp->courseConnection[stage - 1] == 4) { //BLANK1
				while (firstNoteTime - stageStartMeasure - prevStageLastNoteTime > 1.5) {
					stageStartMeasure++;
				}
			}
			else {
				while (firstNoteTime - stageStartMeasure - prevStageLastNoteTime > 0.5) {
					stageStartMeasure++;
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
						for (int i = bmsobj_stageFirst; i < gp->bmsobj.count; i++) {
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
			if (gp->bmsobj.notes[bmsobj_stageFirst].bmsTiming <= 0 || gp->bmsobj.notes[bmsobj_stageFirst].realTiming <= 0 || gp->courseConnection[stage - 1] == 5) { //BLANK2
				bpmt_realtime = unk2346c_realtime;
				bpmt_bmstime = unk23484_bmstime;
				prevNoteBmstime = _bPrevNoteTime;
			}
			else {
				realDiff = gp->bmsobj.notes[k].realTiming - gp->bmsobj.notes[bmsobj_stageFirst].realTiming;
				bmsDiff = gp->bmsobj.notes[k].bmsTiming - gp->bmsobj.notes[bmsobj_stageFirst].bmsTiming;
				for (int i = bmsobj_stageFirst; i < gp->bmsobj.count; i++) {
					gp->bmsobj.notes[i].bmsTiming -= bmsDiff;
					gp->bmsobj.notes[i].realTiming -= realDiff;
					gp->bmsobj.notes[i].bmsTiming_ln -= bmsDiff;
					gp->bmsobj.notes[i].realTiming_ln -= realDiff;
				}
				bpmt_realtime = unk2346c_realtime - realDiff;
				bpmt_bmstime = unk23484_bmstime - bmsDiff;
				prevNoteBmstime = _bPrevNoteTime - stageStartMeasure;
			}
		}
		else {
			//same with -22?line
			bpmt_realtime = unk2346c_realtime;
			bpmt_bmstime = unk23484_bmstime;
			prevNoteBmstime = _bPrevNoteTime;
		}
		// -2331 line
		// 4033- line
		if (gp->isCourse) {
			if (stage != stages - 1) {
				if (objNumLastMeasure > 0) {
					for (int i = objNumLastMeasure; i < gp->bmsobj.count; i++) {
						if (gp->bmsobj.notes[i].op == 2 || gp->bmsobj.notes[i].op == 3 || gp->bmsobj.notes[i].op == 8) {
							gp->bmsobj.notes[i].op = -1;
						}
					}
				}
				if (bpmt_count > 0) gp->bpmt_count = bpmt_count;
			}
			if (gp->isCourse && stage && objNumFirstMeasure > 0) {
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
				if (i == gp->bmsobj.count) break;
				if (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30) {
					if (abs((int)gp->bmsobj.notes[i].realTiming - (int)prevStageTime) < 100) {
						for (int j = bmsobj_stageFirst - 1; j >= 0; j--) {
							if (gp->bmsobj.notes[j].realTiming < prevStageTime - 100.0) break; //need check float calc
							if (gp->bmsobj.notes[i].op == gp->bmsobj.notes[j].op) {
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

		for (int i = bmsobj_stageFirst; i < gp->bmsobj.size; i++) {
			gp->bmsobj.notes[i].stage = stage;
			if (!(0 <= gp->bmsobj.notes[i].stage && gp->bmsobj.notes[i].stage <= 4)) {
				gp->bmsobj.notes[i].stage = 0;
			}
		}

		prevStageTime = -1.0;
		for (int i = gp->bmsobj.count - 1; i != bmsobj_stageFirst; i--) {
			if (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30) {
				prevStageTime = gp->bmsobj.notes[i].realTiming;
				break;
			}
		}

		if ((gp->courseConnection[stage] == 2 || gp->courseConnection[stage] == 3) && stage < stages - 1) { //CUT, CUT+FIT
			gp->fadeoutSOUNDstart[stage] = bpmt_realtime;
			gp->fadeoutSOUNDend[stage] = bpmt_realtime + 200.0;
			gp->fadeinSOUNDstart[stage+1] = bpmt_realtime - 200.0;
			gp->fadeinSOUNDend[stage+1] = bpmt_realtime;
		}
		else{
			if ((gp->courseConnection[stage] == 1 || gp->courseConnection[stage] == 0) && stage < stages - 1) { //FADE, FADE+FIT
				if (gp->courseConnection[stage] != 1) {
					gp->fadeoutSOUNDstart[stage] = bpmt_realtime;
					gp->fadeoutSOUNDend[stage] = bpmt_realtime + 10000.0;
				}
				gp->fadeinSOUNDstart[stage+1] = bpmt_realtime - 5000.0;
				gp->fadeinSOUNDend[stage+1] = bpmt_realtime;
			}
			else if (gp->courseConnection[stage] == 4 && stage < stages - 1) { //BLANK1
				gp->fadeinSOUNDstart[stage+1] = bpmt_realtime - 5000.0;
				gp->fadeinSOUNDend[stage+1] = bpmt_realtime;
			}	
		}

		gp->fadeoutBGAstart[stage] = bpmt_realtime;
		gp->fadeoutBGAend[stage] = bpmt_realtime + 3000.0;
		if (stage < stages - 1) {
			gp->fadeinBGAstart[stage + 1] = bpmt_realtime - 3000.0;
			gp->fadeinBGAend[stage + 1] = bpmt_realtime;
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
	qsort(gp->bmsobj.notes, gp->bmsobj.count, sizeof(NoteStruct), CMP_NotesByRealTiming);

	if (gp->isCourse == 1 && gp->courseType == 1) gp->speedmultiplier = 1.0;
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
				if (cfg->play.randSC[p] == 0) {
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
				if (cfg->play.randSC[p] == 0) {
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
				if (cfg->play.randSC[p] == 0) {
					for (int c = 1; c < 7; c++) {
						int a = c + GetRand(7 - c);
						int tmp = noteRandomTable[p][c];
						noteRandomTable[p][c] = noteRandomTable[p][a];
						noteRandomTable[p][a] = tmp;
					}

					if (0 < cfg->play.randFix[p] && cfg->play.randFix[p] < 8) {
						for (int c = 1; c <= 7; c++) {
							if (noteRandomTable[p][c] == cfg->play.randFix[p]) {
								noteRandomTable[p][c] = noteRandomTable[p][1];
								noteRandomTable[p][1] = cfg->play.randFix[p];
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

					if (0 < cfg->play.randFix[p] && cfg->play.randFix[p] < 8) {
						for (int c = 1; c <= 7; c++) {
							if (noteRandomTable[p][c] == 0) {
								noteRandomTable[p][c] = noteRandomTable[p][cfg->play.randFix[p]];
								noteRandomTable[p][cfg->play.randFix[p]] = 0;
							}
						}
					}
				}
			}
			else if (meta->keymode == 5 || meta->keymode == 10) {
				if (cfg->play.randSC[p] == 0) {
					for (int c = 1; c < 5; c++) {
						int a = c + GetRand(5 - c);
						int tmp = noteRandomTable[p][c];
						noteRandomTable[p][c] = noteRandomTable[p][a];
						noteRandomTable[p][a] = tmp;
					}

					if (0 < cfg->play.randFix[p] && cfg->play.randFix[p] < 6) {
						for (int c = 1; c <= 5; c++) {
							if (noteRandomTable[p][c] == cfg->play.randFix[p]) {
								noteRandomTable[p][c] = noteRandomTable[p][1];
								noteRandomTable[p][1] = cfg->play.randFix[p];
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

					if (0 < cfg->play.randFix[p] && cfg->play.randFix[p] < 6) {
						for (int c = 1; c <= 5; c++) {
							if (noteRandomTable[p][c] == 0) {
								noteRandomTable[p][c] = noteRandomTable[p][cfg->play.randFix[p]];
								noteRandomTable[p][cfg->play.randFix[p]] = 0;
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


	//TOFIX : mine on p2
	
	//shuffle notes
	for (int i = 0; i < gp->bmsobj.count; i++) {
		int optemp = gp->bmsobj.notes[i].op;
		if (optemp < 10 || optemp >= 30) {
			if (optemp == 2) {
				if (cfg->play.battle == 3 && (meta->keymode == 5 || meta->keymode == 7) && gp->isCourse == 0) {
					for (int j = 0; j < 10; j++) {
						unused_swapLane[j] = unused_LaneA[j];
						unused_LaneA[j] = unused_LaneB[j];
						unused_LaneB[j] = unused_swapLane[j];
					}
					SPtoDP(&gp->bmsobj, i, &cc);
				}

				memcpy(&gp->bmsobj_line.notes[gp->bmsobj_line.count], &gp->bmsobj.notes[i], sizeof(NoteStruct));
				gp->bmsobj_line.count++;;

				if (gp->bmsobj_line.count == gp->bmsobj_line.size) {
					ExpandNoteBuffer(&gp->bmsobj_line, 100);
				}
			}
			else if (optemp == 1 && gp->bmsobj.notes[i].realTiming > endtime) {
				endtime = gp->bmsobj.notes[i].realTiming;
			}
		}
		else {
			if (optemp < 20) {
				isBattle = 0;
				if (p1LastTiming < gp->bmsobj.notes[i].realTiming) {
					p1LastTiming = gp->bmsobj.notes[i].realTiming;
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
					if (cfg->play.randFix[0] >= 6) cfg->play.randFix[0] = 0;
					if (cfg->play.randFix[1] >= 6) cfg->play.randFix[1] = 0;
				}
				else if (meta->keymode == 7 || meta->keymode == 14) {
					if (cfg->play.randFix[0] >= 8) cfg->play.randFix[0] = 0;
					if (cfg->play.randFix[1] >= 8) cfg->play.randFix[1] = 0;
				}
				else if (meta->keymode == 9) {
					if (cfg->play.randFix[0] == 0) cfg->play.randFix[0] = 5;
					if (cfg->play.randFix[1] == 0) cfg->play.randFix[1] = 5;
				}

				int assist = 0;
				if (cfg->play.random[0] >= 3 && gp->bmsobj.notes[i].op < 20) assist = (cfg->play.randSC[0] != 0);
				else if (cfg->play.random[1] >= 3 && gp->bmsobj.notes[i].op >= 20) assist = (cfg->play.randSC[1] != 0);

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
								lane = cfg->play.randFix[0];
								pass = 0;
							}
							if (gp->bmsobj.notes[i].op >= 20 && cfg->play.random[1] == 5) {
								lane = cfg->play.randFix[1] + startlane;
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
			bool chnValid = (gp->bmsobj.notes[i].mine > 0);//
			if (gp->bmsobj.notes[i].realTiming_ln + 2000.0 > endtime) {
				endtime = gp->bmsobj.notes[i].realTiming_ln + 2000.0;
			}
			else if (gp->bmsobj.notes[i].realTiming + 2000.0 > endtime) {
				endtime = gp->bmsobj.notes[i].realTiming + 2000.0;
			}

			int lane = noteRandomTable[0][gp->bmsobj.notes[i].op - 10];
			if (cfg->play.battle == 2 && cfg->play.randSC[0] == 0 && cfg->play.randSC[1] == 0 && gp->bmsobj.notes[i].op == 10) {
				gp->bmsobj.notes[i].op = 1;
			}
			else if (cfg->play.battle == 2 && cfg->play.randSC[0] == 0 && cfg->play.randSC[1] == 0 && gp->bmsobj.notes[i].op == 20) {
				gp->bmsobj.notes[i].op = 1;
			}
			else {
				if (intArr[lane] == -1) { //case first note of lane
					ErrorLogFmtAdd("追加->%d", lane);
					memcpy(&gp->bmsobj_note[lane].notes[gp->bmsobj_note[lane].count], &gp->bmsobj.notes[i], sizeof(NoteStruct));
					gp->bmsobj_note[lane].count++;
					intArr[lane] = i;
					if (chnValid == 0) noteCount[isBattle]++;
				}
				else if (gp->bmsobj.notes[i].realTiming > gp->bmsobj.notes[intArr[lane]].realTiming_ln) {
					if (gp->bmsobj.notes[i].realTiming != gp->bmsobj.notes[intArr[lane]].realTiming) {
						ErrorLogFmtAdd("追加->%d", lane);
						memcpy(&gp->bmsobj_note[lane].notes[gp->bmsobj_note[lane].count], &gp->bmsobj.notes[i], sizeof(NoteStruct));
						gp->bmsobj_note[lane].count++;
						intArr[lane] = i;
						if (chnValid == 0) noteCount[isBattle]++;
					}

					else if (gp->bmsobj.notes[i].realTiming < gp->bmsobj.notes[i].realTiming_ln) { //case single note and longnote start conflicts, longnote overwrites it
						noteCount[isBattle]--;
						gp->bmsobj_note[lane].count--;
						
						memcpy(&gp->bmsobj_note[lane].notes[gp->bmsobj_note[lane].count], &gp->bmsobj.notes[i], sizeof(NoteStruct));
						gp->bmsobj_note[lane].count++;
						intArr[lane] = i;
						if (chnValid == 0) noteCount[isBattle]++;
					}
				}
				if (gp->bmsobj_note[lane].count == gp->bmsobj_note[lane].size) {
					ExpandNoteBuffer(&gp->bmsobj_note[lane], 100);
				}
			}
		}
	}

	//duplicate notes for battle
	if (cfg->play.battle == 1 && cfg->play.random[0] == cfg->play.random[1] && (meta->keymode == 5 || meta->keymode == 7 || meta->keymode == 9)) {

		for (int i = 0; i < 10; i++) {
			if (gp->bmsobj_note[0 + i].size > gp->bmsobj_note[10 + i].size) {
				ExpandNoteBuffer(&gp->bmsobj_note[10 + i], gp->bmsobj_note[0 + i].size - gp->bmsobj_note[10 + i].size);
			}
			gp->bmsobj_note[10 + i].count = gp->bmsobj_note[0 + i].count;
			for (int j = 0; j < gp->bmsobj_note[0 + i].count; j++) {
				gp->bmsobj_note[10 + i].notes[j].op = gp->bmsobj_note[0 + i].notes[j].op + 10;
				gp->bmsobj_note[10 + i].notes[j].bmsTiming_ln = gp->bmsobj_note[0 + i].notes[j].bmsTiming_ln;
				gp->bmsobj_note[10 + i].notes[j].realTiming_ln = gp->bmsobj_note[0 + i].notes[j].realTiming_ln;
				gp->bmsobj_note[10 + i].notes[j].active = gp->bmsobj_note[0 + i].notes[j].active;
				gp->bmsobj_note[10 + i].notes[j].mine = gp->bmsobj_note[0 + i].notes[j].mine;
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
			double dmg = std::max(dmg_notebase * 10, dmg_totalbase) / 10.0;

			if (!gp->isCourse) {
				gp->player[p].judge_damage[0][5] = total[p] / (float)notes;
				gp->player[p].judge_damage[0][4] = total[p] / (float)notes;
				gp->player[p].judge_damage[0][3] = total[p] / (float)(notes + notes);
				gp->player[p].judge_damage[0][2] = -4.0;
				gp->player[p].judge_damage[0][1] = -6.0;
				gp->player[p].judge_damage[0][0] = -2.0;

				gp->player[p].judge_damage[1][5] = 0.1;
				gp->player[p].judge_damage[1][4] = 0.1;
				gp->player[p].judge_damage[1][3] = 0.05;
				gp->player[p].judge_damage[1][2] = dmg * (-6.0);
				gp->player[p].judge_damage[1][1] = dmg * (-10.0);
				gp->player[p].judge_damage[1][0] = dmg * (-2.0);

				gp->player[p].judge_damage[2][5] = 0.0;
				gp->player[p].judge_damage[2][4] = 0.0;
				gp->player[p].judge_damage[2][3] = 0.0;
				gp->player[p].judge_damage[2][2] = -100.0;
				gp->player[p].judge_damage[2][1] = -100.0;
				gp->player[p].judge_damage[2][0] = 0.0;

				gp->player[p].judge_damage[3][5] = (total[p] / (float)notes) * 1.2;
				gp->player[p].judge_damage[3][4] = (total[p] / (float)notes) * 1.2;
				gp->player[p].judge_damage[3][3] = (total[p] / (float)(notes + notes)) * 1.2;
				gp->player[p].judge_damage[3][2] = -3.2;
				gp->player[p].judge_damage[3][1] = -4.800000000000001;
				gp->player[p].judge_damage[3][0] = -1.6;

				gp->player[p].judge_damage[4][5] = 0.1;
				gp->player[p].judge_damage[4][4] = -1.0;
				gp->player[p].judge_damage[4][3] = -100.0;
				gp->player[p].judge_damage[4][2] = -100.0;
				gp->player[p].judge_damage[4][1] = -100.0;
				gp->player[p].judge_damage[4][0] = -100.0;

				gp->player[p].judge_damage[5][5] = dmg * (-10.0);
				gp->player[p].judge_damage[5][4] = -1.0;
				gp->player[p].judge_damage[5][3] = 0.1;
				gp->player[p].judge_damage[5][2] = -6.0;
				gp->player[p].judge_damage[5][1] = dmg * (-10.0);
				gp->player[p].judge_damage[5][0] = dmg * (-2.0);
			}
			else if (gp->courseType == 2) {
				gp->player[p].judge_damage[0][5] = 0.1;
				gp->player[p].judge_damage[0][4] = 0.1;
				gp->player[p].judge_damage[0][3] = 0.04;
				gp->player[p].judge_damage[0][2] = -2.0;
				gp->player[p].judge_damage[0][1] = -3.0;
				gp->player[p].judge_damage[0][0] = -2.0;

				gp->player[p].judge_damage[1][5] = 0.1;
				gp->player[p].judge_damage[1][4] = 0.1;
				gp->player[p].judge_damage[1][3] = 0.04;
				gp->player[p].judge_damage[1][2] = dmg * (-6.0);
				gp->player[p].judge_damage[1][1] = dmg * (-10.0);
				gp->player[p].judge_damage[1][0] = dmg * (-2.0);

				gp->player[p].judge_damage[2][5] = 0.0;
				gp->player[p].judge_damage[2][4] = 0.0;
				gp->player[p].judge_damage[2][3] = 0.0;
				gp->player[p].judge_damage[2][2] = -100.0;
				gp->player[p].judge_damage[2][1] = -100.0;
				gp->player[p].judge_damage[2][0] = 0.0;

				gp->player[p].judge_damage[3][5] = 0.1;
				gp->player[p].judge_damage[3][4] = 0.1;
				gp->player[p].judge_damage[3][3] = 0.04;
				gp->player[p].judge_damage[3][2] = -2.0;
				gp->player[p].judge_damage[3][1] = -3.0;
				gp->player[p].judge_damage[3][0] = -2.0;

				gp->player[p].judge_damage[4][5] = 0.1;
				gp->player[p].judge_damage[4][4] = -1.0;
				gp->player[p].judge_damage[4][3] = -100.0;
				gp->player[p].judge_damage[4][2] = -100.0;
				gp->player[p].judge_damage[4][1] = -100.0;
				gp->player[p].judge_damage[4][0] = -100.0;

				gp->player[p].judge_damage[5][5] = dmg * (-10.0);
				gp->player[p].judge_damage[5][4] = -1.0;
				gp->player[p].judge_damage[5][3] = 0.1;
				gp->player[p].judge_damage[5][2] = -6.0;
				gp->player[p].judge_damage[5][1] = dmg * (-10.0);
				gp->player[p].judge_damage[5][0] = dmg * (-2.0);
			}
			else {
				gp->player[p].judge_damage[0][5] = 0.1;
				gp->player[p].judge_damage[0][4] = 0.1;
				gp->player[p].judge_damage[0][3] = 0.04;
				gp->player[p].judge_damage[0][2] = -1.5;
				gp->player[p].judge_damage[0][1] = -2.0;
				gp->player[p].judge_damage[0][0] = -1.5;

				gp->player[p].judge_damage[1][5] = 0.1;
				gp->player[p].judge_damage[1][4] = 0.1;
				gp->player[p].judge_damage[1][3] = 0.05;
				gp->player[p].judge_damage[1][2] = dmg * (-6.0);
				gp->player[p].judge_damage[1][1] = dmg * (-10.0);
				gp->player[p].judge_damage[1][0] = dmg * (-2.0);

				gp->player[p].judge_damage[2][5] = 0.0;
				gp->player[p].judge_damage[2][4] = 0.0;
				gp->player[p].judge_damage[2][3] = 0.0;
				gp->player[p].judge_damage[2][2] = -100.0;
				gp->player[p].judge_damage[2][1] = -100.0;
				gp->player[p].judge_damage[2][0] = 0.0;

				gp->player[p].judge_damage[3][5] = 0.12;
				gp->player[p].judge_damage[3][4] = 0.12;
				gp->player[p].judge_damage[3][3] = 0.048;
				gp->player[p].judge_damage[3][2] = -1.2;
				gp->player[p].judge_damage[3][1] = -1.6;
				gp->player[p].judge_damage[3][0] = -1.2;

				gp->player[p].judge_damage[4][5] = 0.1;
				gp->player[p].judge_damage[4][4] = -1.0;
				gp->player[p].judge_damage[4][3] = -100.0;
				gp->player[p].judge_damage[4][2] = -100.0;
				gp->player[p].judge_damage[4][1] = -100.0;
				gp->player[p].judge_damage[4][0] = -100.0;

				gp->player[p].judge_damage[5][5] = dmg * (-10.0);
				gp->player[p].judge_damage[5][4] = -1.0;
				gp->player[p].judge_damage[5][3] = 0.1;
				gp->player[p].judge_damage[5][2] = -6.0;
				gp->player[p].judge_damage[5][1] = dmg * (-10.0);
				gp->player[p].judge_damage[5][0] = dmg * (-2.0);
			}
		}
	}

	//set first lane keysound
	for (int i = 0; i < 20; i++) {
		if (gp->bmsobj_note[i].count <= 0) gp->bmsobj_note[i].noteVal = -1;
		else gp->bmsobj_note[i].noteVal = gp->bmsobj_note[i].notes[0].val;
	}

	//add notes for maniac options
	if (cfg->play.m_addlong > 0) {
		for (int i = 0; i < 20; i++) {
			if (i != 0 && i != 10) {
				for (int j = 0; j < gp->bmsobj_note[i].count - 1; j++) {
					if (GetRand(100) < cfg->play.m_addlong) {
						double e = (gp->bmsobj_note[i].notes[j].realTiming + gp->bmsobj_note[i].notes[j + 1].realTiming) * 0.5;
						if (gp->bmsobj_note[i].notes[j].realTiming_ln < e) {
							gp->bmsobj_note[i].notes[j].realTiming_ln = e;
							gp->bmsobj_note[i].notes[j].bmsTiming_ln = RealTimeToBMSTime(gp, gp->bmsobj_note[i].notes[j].realTiming_ln);
						}
					}
				}
				if (gp->bmsobj_note[i].count > 0 && cfg->play.m_addlong == 100) {
					gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count - 1].realTiming_ln = endtime;
					gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count - 1].bmsTiming_ln = RealTimeToBMSTime(gp, gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count - 1].realTiming_ln);
				}
				gp->bpmt_start = 0;
			}
		}
		endtime += 2000.0;
	}

	if (cfg->play.m_addmine > 0) {
		for (int i = 0; i < 20; i++) {
			if (i != 0 && i != 10) {
				int s = gp->bmsobj_note[i].count;
				for (int j = 0; j < gp->bmsobj_note[i].count - 1; j++) {
					if (GetRand(100) < cfg->play.m_addmine) {
						if (gp->bmsobj_note[i].count == gp->bmsobj_note[i].size) {
							ExpandNoteBuffer(&gp->bmsobj_note[i], 1000);
						}
						gp->bmsobj_note[i].notes;
						gp->bmsobj_note[i].notes[j + 1].realTiming_ln;
						gp->bmsobj_note[i].notes[j + 1].realTiming;

						if (gp->bmsobj_note[i].notes[j + 1].realTiming_ln > gp->bmsobj_note[i].notes[j + 1].realTiming) {
							if (200.0 < gp->bmsobj_note[i].notes[j + 1].realTiming - gp->bmsobj_note[i].notes[j].realTiming_ln) {
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].realTiming = (gp->bmsobj_note[i].notes[j + 1].realTiming + gp->bmsobj_note[i].notes[j].realTiming_ln) * 0.5;
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].bmsTiming = RealTimeToBMSTime(gp, gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].realTiming);
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].val = 0.0;
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].mine = 4;
								gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].op = gp->bmsobj_note[i].notes[j + 1].op;
								gp->bmsobj_note[i].count++;
							}
						}
						else if (200.0 < gp->bmsobj_note[i].notes[j + 1].realTiming - gp->bmsobj_note[i].notes[j].realTiming) {
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].realTiming = (gp->bmsobj_note[i].notes[j + 1].realTiming + gp->bmsobj_note[i].notes[j].realTiming) * 0.5;
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].bmsTiming = RealTimeToBMSTime(gp, gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].realTiming);
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].val = 0.0;
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].mine = 4;
							gp->bmsobj_note[i].notes[gp->bmsobj_note[i].count].op = gp->bmsobj_note[i].notes[j + 1].op;
							gp->bmsobj_note[i].count++;
						}
					}
				}
				gp->bpmt_start = 0;
				if (s > 1) {
					qsort(gp->bmsobj_note[i].notes, gp->bmsobj_note[i].count, sizeof(NoteStruct), CMP_NotesByRealTiming);
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
	if (gp->fadeinSOUNDstart[0] <= 0 || gp->fadeinSOUNDend[0] <= 0) {
		aud->param.stageBgmVolume[0] = 1.0;
		aud->param.stageKeyVolume[0] = 1.0;
	}
	if (gp->fadeinSOUNDstart[1] <= 0 || gp->fadeinSOUNDend[1] <= 0) {
		aud->param.stageBgmVolume[1] = 1.0;
		aud->param.stageKeyVolume[1] = 1.0;
	}
	if (gp->fadeinSOUNDstart[2] <= 0 || gp->fadeinSOUNDend[2] <= 0) {
		aud->param.stageBgmVolume[2] = 1.0;
		aud->param.stageKeyVolume[2] = 1.0;
	}
	if (gp->fadeinSOUNDstart[3] <= 0 || gp->fadeinSOUNDend[3] <= 0) {
		aud->param.stageBgmVolume[3] = 1.0;
		aud->param.stageKeyVolume[3] = 1.0;
	}
	if (gp->fadeinSOUNDstart[4] <= 0 || gp->fadeinSOUNDend[4] <= 0) {
		aud->param.stageBgmVolume[4] = 1.0;
		aud->param.stageKeyVolume[4] = 1.0;
	}
	ApplySoundFX(aud, 1, cfg->sound.disabledsp);
	ErrorLogAdd("BMSを読み込みました。\n");
	return 1;
}
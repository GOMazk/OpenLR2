#include "LR2_replay.h"
#include "Engine.h"

//4c03c0 //TODO suspection about usage of cstrsprintf
int MoveReplayFile(CSTR songMD5, CSTR localID){

	if (songMD5.length() > 36) {
		songMD5 = MD5str(songMD5);
	}

	CSTR path;
	cstrSprintf(&path, "LR2files/Replay/%s/c", localID);
	CreateDirectoryA(path, 0);
	cstrSprintf(&path, "LR2files/Replay/%s/c/%s", localID,songMD5);
	CreateDirectoryA(path, 0);

	CSTR pathFrom;
	int stage = 0;
	cstrSprintf(&pathFrom, "LR2files/Replay/%s/__%d.lr2rep", localID, stage);
	while (pathFrom.canOpenFile()) {
		CSTR pathTo;
		cstrSprintf(&pathTo, "LR2files/Replay/%s/c/%s/%d.lr2rep", localID, songMD5, stage);
		MoveFileA(pathFrom, pathTo); //TOFIX : if file already exists at pathTo, it fails.
		ErrorLogFmtAdd("リプレイの移動 stage%d\n", stage);

		stage++;
		cstrSprintf(&pathFrom, "LR2files/Replay/%s/__%d.lr2rep", localID, stage);
	}
	ErrorLogFmtAdd("リプレイの移動終了 stage%d\n", stage);

	return (stage != 0);
}

//4c05e0
int LoadReplayFileCourse(REPLAY *rp, CSTR songMD5, int stage, CSTR localID){
	
	int fHandle;

	if (songMD5.length() > 36) {
		songMD5 = MD5str(songMD5);
	}

	CSTR path;
	cstrSprintf(&path, "LR2files/Replay/%s/c/%s/%d.lr2rep", localID, songMD5, stage);

	if (!IsFileExist(path)) return 0;

	long long fSize = FileRead_size(path);

	fHandle = FileRead_open(path, 0);
	if (fSize <= 0 || fHandle == -1) {
		ErrorLogAdd("リプレイデータを開けません。\n");
		return -1;
	}

	rp->data = (ReplayData*)malloc(fSize);
	if (rp->data == NULL) {
		ErrorLogAdd("リプレイロードエラー:リプレイデータ用のメモリ確保に失敗しました。\n");
		return -1;
	}

	rp->max = fSize / sizeof(ReplayData);
	rp->count = 0;
	FileRead_read(rp->data, fSize, fHandle);

	ErrorLogFmtAdd("リプレイデータを読み込みました サイズ%d コマンド数%d\n", fSize, fSize / sizeof(ReplayData));
	FileRead_close(fHandle);
	return 1;

}

//4c07e0
int LoadReplayFile(REPLAY *rp, CSTR songMD5, CSTR localID) {

	int fHandle;

	if (songMD5.length() > 36) {
		songMD5 = MD5str(songMD5);
	}

	CSTR path;
	cstrSprintf(&path, "LR2files/Replay/%s/%s.lr2rep", localID, songMD5);

	if (!IsFileExist(path)) return 0;

	long long fSize = FileRead_size(path);

	fHandle = FileRead_open(path, 0);
	if (fSize <= 0 || fHandle == -1) {
		ErrorLogAdd("リプレイデータを開けません。\n");
		return -1;
	}

	rp->data = (ReplayData*)malloc(fSize);
	if (rp->data == NULL) {
		ErrorLogAdd("リプレイロードエラー:リプレイデータ用のメモリ確保に失敗しました。\n");
		return -1;
	}

	rp->max = fSize / sizeof(ReplayData);
	rp->count = 0;
	FileRead_read(rp->data, fSize, fHandle);

	ErrorLogFmtAdd("リプレイデータを読み込みました サイズ%d コマンド数%d\n", fSize, fSize/sizeof(ReplayData));
	FileRead_close(fHandle);
	return 1;
}

//4c09e0
int SaveReplay(REPLAY *rp, CSTR songMD5, CSTR localID) {

	FILE *pFile;

	if (songMD5.length() > 36) {
		songMD5 = MD5str(songMD5);
	}

	CSTR path;
	cstrSprintf(&path, "LR2files/Replay/%s/%s.lr2rep", localID, songMD5);
	pFile = fopen(path, "wb");
	if (pFile == NULL) {
		ErrorLogAdd("リプレイデータの保存に失敗しました。\n");
		return -1;
	}

	fwrite(rp->data, rp->count * sizeof(ReplayData), 1, pFile);
	fclose(pFile);
	ErrorLogAdd("リプレイデータを保存しました。\n");

	return 1;
}

//4c0b40
int AllocReplayBuffer(REPLAY *rp){

	rp->max = 10000;
	rp->count = 0;
	rp->data = (ReplayData *)malloc(rp->max * sizeof(ReplayData));
	if (rp->data == NULL) {
		ErrorLogAdd("リプレイ開始エラー:リプレイデータ用のメモリ確保に失敗しました。\n");
		rp->max = 0;
		return -1;
	}
	memset(rp->data, 0, rp->max * sizeof(ReplayData));
	ErrorLogAdd("リプレイデータ用のメモリを確保しました。\n");
	return 1;
}


//4c0bb0
int ReleaseReplayBuffer(REPLAY *rp){
	if (rp->max > 0 && rp->data != NULL) {
		free(rp->data);
		rp->data = NULL;
	}
	rp->max = 0;
	rp->count = 0;
	ErrorLogAdd("リプレイデータバッファを解放しました。\n");
	return 1;
}


//4c0c00
int AddReplayData(REPLAY *rp, int timing, char op, short value){

	if (rp->max < 1) {
		return 0;
	}
	if (rp->max == rp->count) {
		rp->max += 10000;
		rp->data = (ReplayData *)realloc(rp->data, rp->max * sizeof(ReplayData));
	}
	rp->data[rp->count].op = op;
	rp->data[rp->count].timing = timing;
	rp->data[rp->count].value = (int)value;
	rp->count++;
	return 1;
}

//4c0c70
int AddReplayDataHeader(CONFIG_PLAY *cfg, REPLAY *rp, AUDIO *snd, gameplay *gp){
	AddReplayData(rp, 0, 0x64, *(short *)&cfg->hiSpeed[0]);
	AddReplayData(rp, 0, 0x96, *(short *)&cfg->hiSpeed[1]);
	AddReplayData(rp, 0, 0x67, *(short *)&cfg->random[0]);
	AddReplayData(rp, 0, 0x99, *(short *)&cfg->random[1]);
	AddReplayData(rp, 0, 0x68, *(short *)&cfg->m_HIDSUD1);
	AddReplayData(rp, 0, 0x9a, *(short *)&cfg->m_HIDSUD2);
	AddReplayData(rp, 0, 0x65, *(short *)&cfg->gaugeOption[0]);
	AddReplayData(rp, 0, 0x97, *(short *)&cfg->gaugeOption[1]);
	AddReplayData(rp, 0, 0x66, *(short *)&cfg->p1_lanecoverv);
	AddReplayData(rp, 0, 0x98, *(short *)&cfg->p2_lanecoverv);
	AddReplayData(rp, 0, 0x6b, *(short *)&cfg->p1_assist);
	AddReplayData(rp, 0, 0x9d, *(short *)&cfg->p2_assist);
	AddReplayData(rp, 0, 0x6a, *(short *)&cfg->randSC[0]);
	AddReplayData(rp, 0, 0x9c, *(short *)&cfg->randSC[1]);
	AddReplayData(rp, 0, 0x69, *(short *)&cfg->randFix[0]);
	AddReplayData(rp, 0, 0x9b, *(short *)&cfg->randFix[1]);
	AddReplayData(rp, 0, 0xc9, *(short *)&cfg->battle);
	AddReplayData(rp, 0, 0xca, *(short *)&gp->isAutoplay);
	AddReplayData(rp, 0, 0xcb, *(short *)&cfg->hsfix);
	AddReplayData(rp, 0, 0xcc, *(short *)&cfg->is_extra);
	AddReplayData(rp, 0, 0xcd, *(short *)&cfg->m_extra);
	AddReplayData(rp, 0, 0xce, *(short *)&cfg->dpflip);
	AddReplayData(rp, 0, 0x28, *(short *)&snd->param.fx_volume_on);
	AddReplayData(rp, 0, 0x29, *(short *)&snd->param.volume_master);
	AddReplayData(rp, 0, 0x2a, *(short *)&snd->param.volume_key);
	AddReplayData(rp, 0, 0x2b, *(short *)&snd->param.volume_BGM);
	AddReplayData(rp, 0, 0x32, *(short *)&snd->param.eq_on);
	AddReplayData(rp, 0, 0x33, *(short *)&snd->param.eq_gain[0]);
	AddReplayData(rp, 0, 0x34, *(short *)&snd->param.eq_gain[1]);
	AddReplayData(rp, 0, 0x35, *(short *)&snd->param.eq_gain[2]);
	AddReplayData(rp, 0, 0x36, *(short *)&snd->param.eq_gain[3]);
	AddReplayData(rp, 0, 0x37, *(short *)&snd->param.eq_gain[4]);
	AddReplayData(rp, 0, 0x38, *(short *)&snd->param.eq_gain[5]);
	AddReplayData(rp, 0, 0x39, *(short *)&snd->param.eq_gain[6]);
	AddReplayData(rp, 0, 0x3c, *(short *)&snd->param.fx_on[0]);
	AddReplayData(rp, 0, 0x3d, *(short *)&snd->param.fxType[0]);
	AddReplayData(rp, 0, 0x3e, *(short *)&snd->param.fxParam[0][0]);
	AddReplayData(rp, 0, 0x3f, *(short *)&snd->param.fxParam[0][1]);
	AddReplayData(rp, 0, 0x40, *(short *)&snd->param.fxChannel[0]);
	AddReplayData(rp, 0, 0x46, *(short *)&snd->param.fx_on[1]);
	AddReplayData(rp, 0, 0x47, *(short *)&snd->param.fxType[1]);
	AddReplayData(rp, 0, 0x48, *(short *)&snd->param.fxParam[1][0]);
	AddReplayData(rp, 0, 0x49, *(short *)&snd->param.fxParam[1][1]);
	AddReplayData(rp, 0, 0x4a, *(short *)&snd->param.fxChannel[1]);
	AddReplayData(rp, 0, 0x50, *(short *)&snd->param.fx_on[2]);
	AddReplayData(rp, 0, 0x51, *(short *)&snd->param.fxType[2]);
	AddReplayData(rp, 0, 0x52, *(short *)&snd->param.fxParam[2][0]);
	AddReplayData(rp, 0, 0x53, *(short *)&snd->param.fxParam[2][1]);
	AddReplayData(rp, 0, 0x54, *(short *)&snd->param.fxChannel[2]);
	AddReplayData(rp, 0, 0x5a, *(short *)&snd->param.pitch_on);
	AddReplayData(rp, 0, 0x5b, *(short *)&snd->param.pitch_amount);
	AddReplayData(rp, 0, 0x5c, *(short *)&snd->param.pitch_type);
	return 1;
}


//4c1050
int InputToReplay(REPLAY *rp, inputStructure *is, int timing, int scratchSide) {

	if (scratchSide == 0) {
		for (int i = 0; i < 40; i++) {
			if (is->p1_buttonInput[i] == 1) AddReplayData(rp, timing, i, 1);
			else if (is->p1_buttonInput[i] == 3) AddReplayData(rp, timing, i, 0);
		}
		return 1;
	}

	if (scratchSide == 1 || scratchSide == 3) {
		
		if (is->p1_buttonInput[0] == 1) AddReplayData(rp, timing, 0, 1);
		else if (is->p1_buttonInput[0] == 3) AddReplayData(rp, timing, 0, 0);
		for (int i = 1; i < 3; i++) { // 1~2 -> 6~7
			if (is->p1_buttonInput[i] == 1) AddReplayData(rp, timing, i + 5, 1);
			else if (is->p1_buttonInput[i] == 3) AddReplayData(rp, timing, i + 5, 0);
		}
		for (int i = 3; i < 8; i++) { // 3~7 -> 1~5
			if (is->p1_buttonInput[i] == 1) AddReplayData(rp, timing, i - 2, 1); //seems call function is optimized, for convenience I replaced it with function call.
			else if (is->p1_buttonInput[i] == 3) AddReplayData(rp, timing, i - 2, 0);
		}
		for (int i = 8; i < 40; i++) {
			if (is->p1_buttonInput[i] == 1) AddReplayData(rp, timing, i, 1); //seems call function is optimized, for convenience I replaced it with function call.
			else if (is->p1_buttonInput[i] == 3) AddReplayData(rp, timing, i, 0); //seems call function is optimized, for convenience I replaced it with function call.
		}
		return 1;
	}

	if (scratchSide == 2) {
		for (int i = 0; i < 21; i++) {
			if (is->p1_buttonInput[i] == 1) AddReplayData(rp, timing, i, 1);
			else if (is->p1_buttonInput[i] == 3) AddReplayData(rp, timing, i, 0);
		}
		for (int i = 21; i < 23; i++) { // 21~22 -> 26~27
			if (is->p1_buttonInput[i] == 1) AddReplayData(rp, timing, i + 5, 1); 
			else if (is->p1_buttonInput[i] == 3) AddReplayData(rp, timing, i + 5, 0);
		}
		for (int i = 23; i < 28; i++) { // 23~27 -> 21~25
			if (is->p1_buttonInput[i] == 1) AddReplayData(rp, timing, i - 2, 1); //seems call function is optimized, for convenience I replaced it with function call.
			else if (is->p1_buttonInput[i] == 3) AddReplayData(rp, timing, i - 2, 0);
		}
		for (int i = 28; i < 40; i++) {
			if (is->p1_buttonInput[i] == 1) AddReplayData(rp, timing, i, 1); //seems call function is optimized, for convenience I replaced it with function call.
			else if (is->p1_buttonInput[i] == 3) AddReplayData(rp, timing, i, 0); //seems call function is optimized, for convenience I replaced it with function call.
		}
		return 1;
	}

	return 1;
}

//4c1490
int REPLAY_ApplyJudgeNote(gameplay *gp, Timer *T, game *g, uint judge, int player, int dp) {

	if (judge > 5) return 0;
	
	if (judge >= 1) {
		if (player == 0 && gp->ghostBattle == 0 && gp->isAutoplay == 0 && g->config.play.battle != 1) {
			while (gp->highScore.DealJudgeFromQueue() == 0) {}
			while (gp->targetScore.DealJudgeFromQueue() == 0) {}

			if (gp->targetType == 1) {
				gp->highScore.SetScore(&gp->player[1], 0);
			}
			else if (gp->targetType == 2) {
				gp->targetScore.SetScore(&gp->player[1], 0);
			}

		}
		gp->player[player].note_current++;
		gp->player[player].note_current2++;
		if (gp->player[player].note_current == gp->player[player].totalnotes) SetTimeLapse(143 + player, T); //final note timer

	}

	gp->player[player].recent_judge = judge;
	if (judge < 3) {
		if (gp->ghostBattle == 0 || player == 0) {
			gp->lastMissTime = GetTimeWrap();
		}
		gp->misslayerTime[player] = GetTimeWrap();
	}

	if (judge == 0) {
		gp->player[player].judgecount[1]++;
		gp->player[player].judgecount2[1]++;
	}
	else {
		gp->player[player].judgecount[judge]++;
		gp->player[player].judgecount2[judge]++;
	}

	int hp = ((int)gp->player[player].HP / 2) * 2;
	double damage;
	if (hp <= 30 && (!(g->config.play.gaugeOption[player] == 0 || g->config.play.gaugeOption[player] == 3) || gp->isCourse != 0) && judge <= 2) {
		damage = gp->player[player].judge_damage[judge] * 0.6;
	}
	else {
		damage = gp->player[player].judge_damage[judge];
	}
	gp->player[player].HP += damage;

	if (gp->isCourse == 0 && gp->player[player].HP <= 2.0 && (g->config.play.gaugeOption[player] == 0 || g->config.play.gaugeOption[player] == 3)) {
		gp->player[player].HP = 2.0;
	}

	if (gp->player[player].HP >= 100.0) gp->player[player].HP = 100.0;
	if (gp->player[player].HP < 0.0) gp->player[player].HP = 0.0;

	int newhp = ((int)gp->player[player].HP / 2) * 2;
	if (hp <= 0) {
		gp->player[player].HP = 0; //?
		newhp = 0;
		ResetTimeLapse(44 + player, T);
	}
	else if (newhp != 100)
		ResetTimeLapse(44 + player, T);

	if (hp < newhp) {
		if (newhp == 100) {
			ResetTimeLapse(42 + player, T);
			SetTimeLapse(44 + player, T);
		}
		else {
			SetTimeLapse(42 + player, T);
		}
	}

	if (gp->player[player].totalnotes > 0) {
		gp->player[player].score = (gp->player[player].judgecount[3] + (gp->player[player].judgecount[4] + gp->player[player].judgecount[5] * 2) * 2) * 50000 / gp->player[player].totalnotes;
	}
	gp->player[player].exscore = gp->player[player].judgecount[4] + gp->player[player].judgecount[5] * 2;

	if (gp->player[player].note_current > 0) gp->player[player].rate = gp->player[player].exscore * 100 / (double)(gp->player[player].note_current * 2);

	switch (judge) {
		case 1:
		case 2:
			gp->player[player].now_combo = 0;
			gp->player[player].now_combo_course = 0;
			break;
		case 3:
		case 4:
		case 5:
			gp->player[player].now_combo++;
			if (gp->player[player].now_combo > gp->player[player].max_combo) gp->player[player].max_combo = gp->player[player].now_combo;
			gp->player[player].now_combo_course++;
			if (gp->player[player].now_combo_course > gp->player[player].max_combo_course) gp->player[player].max_combo_course = gp->player[player].now_combo_course;
			break;
	}

	if (gp->player[player].now_combo == gp->player[player].totalnotes) SetTimeLapse(48 + player, T); //fullcombo timer

	if (dp == 1) {
		if (player == 0) {
			gp->player[1].judge_draw = gp->player[0].recent_judge;
			gp->player[1].combo_song_draw = gp->player[0].now_combo;
			gp->player[1].combo_draw = gp->player[0].now_combo_course;
			SetTimeLapse(47, T);
			return 1;
		}
	}
	else if (player == 0) {
		gp->player[0].judge_draw = gp->player[0].recent_judge;
		gp->player[0].combo_song_draw = gp->player[0].now_combo;
		gp->player[0].combo_draw = gp->player[0].now_combo_course;
		SetTimeLapse(46, T);
		return 1;
	}
	if (player == 1 && dp == 1) {
		gp->player[1].judge_draw = gp->player[1].recent_judge;
		gp->player[1].combo_song_draw = gp->player[1].now_combo;
		gp->player[1].combo_draw = gp->player[1].now_combo_course;
		SetTimeLapse(47, T);
	}

	return 1;
}


//4c18b0
int REPLAY_ApplyJudgeMine(gameplay *gp, Timer *T, game *g, int dmg, int player, int dp) {

	gp->player[player].judgecount[1]++;
	gp->player[player].recent_judge = 0;
	gp->player[player].HP -= dmg;

	if (gp->player[player].HP <= 2.0 && (g->config.play.gaugeOption[player] == 0 || g->config.play.gaugeOption[player] == 3)) {
		gp->player[player].HP = 2.0;
	}
	if (gp->player[player].HP >= 100.0) {
		gp->player[player].HP = 100.0;
	}
	if (gp->player[player].HP < 0.0) {
		gp->player[player].HP = 0.0;
	}

	if ((int)gp->player[player].HP / 2 != 50) ResetTimeLapse(44 + player, T); //this may be different

	if (dp == 1) {
		if (player == 0) {
			gp->player[1].judge_draw = gp->player[0].recent_judge;
			gp->player[1].combo_song_draw = gp->player[0].now_combo;
		}
		else if (player == 1) {
			gp->player[1].judge_draw = gp->player[1].recent_judge;
			gp->player[1].combo_song_draw = gp->player[1].now_combo;
		}

		SetTimeLapse(47, &g->timer1);
	}
	else if (player == 0) {
		gp->player[0].judge_draw = gp->player[0].recent_judge;
		gp->player[0].combo_song_draw = gp->player[0].now_combo;

		SetTimeLapse(46, &g->timer1);
	}

	return 1;
}

//4c1a00
int ReplayDataToInput(ReplayData *data, game *g, AUDIO *aud, gameplay *gp, inputStructure *is, Timer *T) {

	if (data->op < 40) {
		if (data->value == 1) {
			if ((gp->scratchSide == 1 || gp->scratchSide == 3) && (1 <= data->op && data->op <= 5)) is->p1_buttonInput[data->op + 2] = 1;
			else if ((gp->scratchSide == 1 || gp->scratchSide == 3) && (6 <= data->op && data->op <= 7)) is->p1_buttonInput[data->op - 5] = 1;
			else if ((gp->scratchSide == 2 || gp->scratchSide == 3) && (21 <= data->op && data->op <= 25)) is->p1_buttonInput[data->op + 2] = 1;
			else if ((gp->scratchSide == 2 || gp->scratchSide == 3) && (26 <= data->op && data->op <= 27)) is->p1_buttonInput[data->op - 5] = 1;
			else is->p1_buttonInput[data->op] = 1;
		}
		else if (data->value == 0) {
			if ((gp->scratchSide == 1 || gp->scratchSide == 3) && (1 <= data->op && data->op <= 5)) is->p1_buttonInput[data->op + 2] = 3;
			else if ((gp->scratchSide == 1 || gp->scratchSide == 3) && (6 <= data->op && data->op <= 7)) is->p1_buttonInput[data->op - 5] = 3;
			else if ((gp->scratchSide == 2 || gp->scratchSide == 3) && (21 <= data->op && data->op <= 25)) is->p1_buttonInput[data->op + 2] = 3;
			else if ((gp->scratchSide == 2 || gp->scratchSide == 3) && (26 <= data->op && data->op <= 27)) is->p1_buttonInput[data->op - 5] = 3;
			else is->p1_buttonInput[data->op] = 3;
		}
	}

	switch (data->op) {
		case 0x28:
			aud->param.fx_volume_on = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;
		case 0x29:
			aud->param.volume_master = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x2a:
			aud->param.volume_key = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x2b:
			aud->param.volume_BGM = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;

		case 0x32:
			aud->param.eq_on = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;
		case 0x33:
			aud->param.eq_gain[0] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x34:
			aud->param.eq_gain[1] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x35:
			aud->param.eq_gain[2] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x36:
			aud->param.eq_gain[3] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x37:
			aud->param.eq_gain[4] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x38:
			aud->param.eq_gain[5] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x39:
			aud->param.eq_gain[6] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;

		case 0x3c:
			aud->param.fx_on[0] = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;
		case 0x3d:
			aud->param.fxType[0] = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;
		case 0x3e:
			aud->param.fxParam[0][0] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x3f:
			aud->param.fxParam[0][1] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x40:
			aud->param.fxChannel[0] = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;

		case 0x46:
			aud->param.fx_on[1] = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;
		case 0x47:
			aud->param.fxType[1] = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;
		case 0x48:
			aud->param.fxParam[1][0] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x49:
			aud->param.fxParam[1][1] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x4a:
			aud->param.fxChannel[1] = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;

		case 0x50:
			aud->param.fx_on[2] = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;
		case 0x51:
			aud->param.fxType[2] = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;
		case 0x52:
			aud->param.fxParam[2][0] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x53:
			aud->param.fxParam[2][1] = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x54:
			aud->param.fxChannel[2] = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;

		case 0x5a:
			aud->param.pitch_on = data->value;
			ApplySoundFX(aud, 1, g->config.sound.disabledsp);
			return 1;
		case 0x5b:
			aud->param.pitch_amount = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;
		case 0x5c:
			aud->param.pitch_type = data->value;
			ApplySoundFX(aud, 0, g->config.sound.disabledsp);
			return 1;

		case 0x64:
			g->config.play.hiSpeed[0] = data->value;
			break;
		case 0x65:
			g->config.play.gaugeOption[0] = data->value;
			break;
		case 0x66:
			g->config.play.p1_lanecoverv = data->value;
			break;
		case 0x67:
			g->config.play.random[0] = data->value;
			break;
		case 0x68:
			g->config.play.m_HIDSUD1 = data->value;
			break;
		case 0x69:
			g->config.play.randFix[0] = data->value;
			break;
		case 0x6a:
			g->config.play.randSC[0] = data->value;
			break;
		case 0x6b:
			g->config.play.p1_assist = data->value;
			break;
		case 0x96:
			g->config.play.hiSpeed[1] = data->value;
			break;
		case 0x97:
			g->config.play.gaugeOption[1] = data->value;
			break;
		case 0x98:
			g->config.play.p2_lanecoverv = data->value;
			break;
		case 0x99:
			g->config.play.random[1] = data->value;
			break;
		case 0x9a:
			g->config.play.m_HIDSUD2 = data->value;
			break;
		case 0x9b:
			g->config.play.randFix[1] = data->value;
			break;
		case 0x9c:
			g->config.play.randSC[1] = data->value;
			break;
		case 0x9d:
			g->config.play.p2_assist = data->value;
			break;
		case 200:
			gp->randomseed = data->value;
			break;
		case 0xc9:
			g->config.play.battle = data->value;
			break;
		case 0xca:
			gp->isAutoplay = data->value;
			break;
		case 0xcb:
			g->config.play.hsfix = data->value;
			break;
		case 0xcc:
			g->config.play.is_extra = data->value;
			break;
		case 0xcd:
			g->config.play.m_extra = data->value;
			break;
		case 0xce:
			g->config.play.dpflip = data->value;
			break;
		case 0xd2:
			REPLAY_ApplyJudgeNote(gp, T, g, data->value, 0, 0);
			break;
		case 0xd3:
			REPLAY_ApplyJudgeNote(gp, T, g, data->value, 0, 1);
			break;
		case 0xd4:
			REPLAY_ApplyJudgeNote(gp, T, g, data->value, 1, 0);
			break;
		case 0xd5:
			REPLAY_ApplyJudgeNote(gp, T, g, data->value, 1, 1);
			break;
		case 0xd6:
			REPLAY_ApplyJudgeMine(gp, T, g, data->value, 0, 0);
			break;
		case 0xd7:
			REPLAY_ApplyJudgeMine(gp, T, g, data->value, 0, 1);
			break;
		case 0xd8:
			REPLAY_ApplyJudgeMine(gp, T, g, data->value, 1, 0);
			break;
		case 0xd9:
			REPLAY_ApplyJudgeMine(gp, T, g, data->value, 1, 1);
			break;
	}

	//TOFIX
	if (data == (ReplayData *)0x1) {
		ApplySoundFX(aud, 0, (char)g->config.sound.disabledsp);
	}
	else if (data == (ReplayData *)0x2) {
		ApplySoundFX(aud, 1, (char)g->config.sound.disabledsp);
	}
	
	return 1;
}

//4c2280
int SetReplayConfig(REPLAY *re, game *g, AUDIO *aud, gameplay *gp, inputStructure *in, Timer *T) {
	
	memcpy(&re->cfg, &g->config.play, sizeof(CONFIG_PLAY));
	memcpy(&re->aud, &aud->param, sizeof(AUDIO_PARAM));

	while (re->count < re->max && re->data[re->count].timing == 0) {
		ReplayDataToInput(&re->data[re->count], g, aud, gp, in, T);
		re->count++;
	}

	return 1;
}

//4c2310
int ReplayToInput(REPLAY *rp, game *g, AUDIO *aud, gameplay *gp, inputStructure *is, Timer *T){
	if (rp->count < rp->max) {
		while (rp->count < rp->max && rp->data[rp->count].timing < GetTimeLapse(41, T)) {
			ReplayDataToInput(rp->data + rp->count, g, aud, gp, is, T);
			rp->count++;
		}
		return 1;
	}
	return 0;
}

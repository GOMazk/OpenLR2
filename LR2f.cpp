#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"vfw32.lib")
#pragma comment(lib,"fmodex_vc.lib")
#include <winsock2.h>
#include <vfw.h>
#include <Windows.h>
#include <process.h> //beginthread
#include <math.h>


#include "strclass.h"
#include "LR2f.h"
#include "dxlib/DxLib.h"
extern "C" {
#include "sqlite/sqlite3.h"
}
#include "FMODex/fmod.h"

#include "structure.h"
#include "LR2startup.h" //MD5str
#include "LR2input.h"

extern "C" {
#include "md5.h" //md5File()
}

#define pi 3.1415926525
#define pi2 6.2831853
#define pi_half 1.570796

glb_dbgame glb;

//401000
void MYRANKING::InitRanking() {
	this->songMD5.fillzero();
	this->unused.fillzero();
	this->passMD5.fillzero();
	this->title.fillzero();
	this->genre.fillzero();
	this->artist.fillzero();
	this->ghost.fillzero();
	this->maxbpm = 0;
	this->minbpm = 0;
	this->playlevel = 0;
	this->clear = 0;
	this->exscore = 0;
	this->pg = 0;
	this->gr = 0;
	this->gd = 0;
	this->bd = 0;
	this->pr = 0;
	this->maxcombo = 0;
	this->playcount = 0;
	this->clearcount = 0;
	this->rate = 0;
	this->minbp = 0;
	this->totalnotes = 0;
	this->opt_history = 0;
	this->rseed = 0;
	this->clear_sd = 0;
	this->clear_db = 0;
	this->clear_ex = 0;
	this->line = 0;
	this->judge = 0;
	this->inputtype = 0;
	return;
}

//401090
bool CheckScoreSaveConditon(game *g){
	if ( (g->config.play.battle == 0 || g->config.play.battle == 4) && g->config.play.is_extra != 1
		&& g->config.play.m_addlong == 0 && g->config.play.unused_f2 == 0 && g->config.play.m_loudness <= 0 
		&& g->config.play.m_lunaris == 0 && g->config.play.hsfix != 4 && g->config.play.m_addmine == 0 
		&& (g->config.play.m_addnote == 0 && g->config.play.is_extra == 0) && g->config.play.autokey == 0
		&& (g->config.play.unknown_1 == 0 || g->sSelect.metaSelected.keymode < 10) 
		&& g->config.play.p1_assist == 0 && (g->config.play.p2_assist == 0 || g->sSelect.metaSelected.keymode < 10)
		&& g->config.play.random[0] < 4 && g->config.play.random[1] < 4) {
		return true;
	}
	return false;
}

//401130
int CheckClearLampChallenge(game *g){
	int gauge;

	if (g->config.play.m_addlong == 1 || g->config.play.unused_f2 || g->config.play.m_loudness > 0
		|| g->config.play.m_lunaris || g->config.play.m_addlong > 0
		|| g->config.play.m_addmine || g->config.play.m_addnote || g->config.play.battle == 1) {
		return 0;
	}

	if (g->config.play.random[0] < 4 && g->config.play.random[1] < 4 && g->config.play.hsfix != 4
		&& g->config.play.autokey == 0 && (g->config.play.unknown_1 == 0 || g->sSelect.metaSelected.keymode < 10) 
		&& g->config.play.p1_assist == 0 && (g->config.play.p2_assist == 0 || g->sSelect.metaSelected.keymode < 10)) {

		gauge = g->config.play.gaugeOption[0];
		if (gauge == 1) return 3;
		else if (gauge == 2) return 4;
		else if (gauge == 3) return 1;
		else if (gauge == 4) return 4;
		else if (gauge == 5) return 3;
		else return 2;
	}
	return 1;
}

//4011f0
int FxByMIDI(game *g) {
	bool change = false;
	
	if (g->KeyInput.is_drag_now >= 0 && g->KeyInput.is_drag_now < g->skstruct.otherObject[2].srcSize) {
		if (g->KeyInput.midi_n <= 0) {
			if (g->KeyInput.inputID[D_DIK_DELETE]) g->config.input.midi_control[g->KeyInput.is_drag_now] = 0;
		}
		else if (1 <= g->KeyInput.is_drag_now && g->KeyInput.is_drag_now < 40) {
			for (int i = 0; i < 40; i++) {
				if (g->config.input.midi_control[i] == g->KeyInput.midi_n) g->config.input.midi_control[i] = 0;
			}
			g->config.input.midi_control[g->KeyInput.is_drag_now] = g->KeyInput.midi_n;
		}
	}

	if (g->KeyInput.midi_n <= 0) return 0;

	for (int i = 1; i < 40; i++) {
		if (g->config.input.midi_control[i] == g->KeyInput.midi_n && g->config.input.midi_control[i] != 0) {
			switch (i - 10) {
				case 0:
					g->audio.param.eq_gain[0] = ChangeValueByTime(-12.0, 12.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 51, (short)g->audio.param.eq_gain[0]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 1:
					g->audio.param.eq_gain[1] = ChangeValueByTime(-12.0, 12.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 52, (short)g->audio.param.eq_gain[1]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 2:
					g->audio.param.eq_gain[2] = ChangeValueByTime(-12.0, 12.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 53, (short)g->audio.param.eq_gain[2]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 3:
					g->audio.param.eq_gain[3] = ChangeValueByTime(-12.0, 12.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 54, (short)g->audio.param.eq_gain[3]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 4:
					g->audio.param.eq_gain[4] = ChangeValueByTime(-12.0, 12.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 55, (short)g->audio.param.eq_gain[4]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 5:
					g->audio.param.eq_gain[5] = ChangeValueByTime(-12.0, 12.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 56, (short)g->audio.param.eq_gain[5]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 6:
					g->audio.param.eq_gain[6] = ChangeValueByTime(-12.0, 12.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 57, (short)g->audio.param.eq_gain[6]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 7:
					g->audio.param.volume_master = ChangeValueByTime(0.0, 100.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 41, (short)g->audio.param.volume_master);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 8:
					g->audio.param.volume_key = ChangeValueByTime(0.0, 100.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 42, (short)g->audio.param.volume_key);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 9:
					g->audio.param.volume_BGM= ChangeValueByTime(0.0, 100.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 43, (short)g->audio.param.volume_BGM);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 10:
					g->audio.param.fxParam[0][0] = ChangeValueByTime(0.0, 100.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 62, (short)g->audio.param.fxParam[0][0]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 11:
					g->audio.param.fxParam[0][1] = ChangeValueByTime(0.0, 100.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 62, (short)g->audio.param.fxParam[0][1]); //TOFIX:63
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 12:
					g->audio.param.fxParam[1][0] = ChangeValueByTime(0.0, 100.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 72, (short)g->audio.param.fxParam[1][0]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 13:
					g->audio.param.fxParam[1][1] = ChangeValueByTime(0.0, 100.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 73, (short)g->audio.param.fxParam[1][1]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 14:
					g->audio.param.fxParam[2][0] = ChangeValueByTime(0.0, 100.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 82, (short)g->audio.param.fxParam[2][0]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 15:
					g->audio.param.fxParam[2][1] = ChangeValueByTime(0.0, 100.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 83, (short)g->audio.param.fxParam[2][1]);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;

				case 16:
					g->audio.param.pitch_amount = ChangeValueByTime(-12.0, 12.0, 0.0, 127.0, g->KeyInput.midi_v, 0);
					change = 1;
					if (GetTimeLapse(41, &g->timer1) > 0 && g->gameplay.replay.status == 1) {
						change = 1;
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 91, (short)g->audio.param.pitch_amount);
						g->gameplay.fxChangeInRecording = 1;
					}
					break;
			}
		}
	}

	if (change) ApplySoundFX(&g->audio, 0, g->config.sound.disabledsp);
	g->KeyInput.midi_n = 0;
	g->KeyInput.midi_v = 0;
	return 1;
}

//401be0
int SetTarget(game *g) {

	if (g->net.rankingData.showRanking == 1 && g->procSelecter == 2) {
		SetObjectString(1, g->sSelect.bmsList[g->sSelect.cur_song].title, g->txtStruct.objectStr);
		return 1;
	}

	if (g->sSelect.stack_query[g->sSelect.cur].findStrPos("__RIVAL__") > -1 && g->procSelecter == 2) {
		SetObjectString(1, g->sSelect.stack_searchTitle[g->sSelect.cur], g->txtStruct.objectStr);
		return 1;
	}

	switch (g->config.play.p1_target) {
		case 0:
			SetObjectString(1, "NO TARGET", g->txtStruct.objectStr);
			break;
		case 1:
			SetObjectString(1, "MY BEST", g->txtStruct.objectStr);
			break;
		case 2:
			SetObjectString(1, "RANK AAA", g->txtStruct.objectStr);
			break;
		case 3:
			SetObjectString(1, "RANK AA", g->txtStruct.objectStr);
			break;
		case 4:
			SetObjectString(1, "RANK A", g->txtStruct.objectStr);
			break;
		case 5: {
			CSTR str;
			cstrSprintf(&str, "%d%%", g->config.play.target_percent);
			SetObjectString(1, str, g->txtStruct.objectStr);
			break;
		}
		case 6:
			SetObjectString(1, "IR TOP", g->txtStruct.objectStr);
			break;
		case 7:
			SetObjectString(1, "IR NEXT", g->txtStruct.objectStr);
			break;
		case 8:
			SetObjectString(1, "IR AVERAGE", g->txtStruct.objectStr);
			break;
		default:
			SetObjectString(1, "ERROR", g->txtStruct.objectStr);
			break;
	}

	return 1;
}

//401da0
int ProcS_Keyconfig(game *g) {
	CSTR t("-",0);
	for (int i = 0; i < 8; i++) {
		SetObjectString(i+40,t,g->txtStruct.objectStr);
	}
	if (g->KeyInput.config_button_inMap > 0) {
		for (int i = 0; i < 16; i++) {
			t = GetKeyIDname(g->config.input.buttonMap[g->KeyInput.config_button_inMap][i]);
			SetObjectString(i + 40, t, g->txtStruct.objectStr);
		}
	}
	return 1;
}

//401ec0
int LoopInRange(int min, int max, int add, int *num){
	*num = *num + add;
	if (max < *num) {
		*num = min;
		return 1;
	}
	if (*num < min) {
		*num = max;
	}
	return 1;
}

//401ef0
int UpdateSoundFX(game *g, int *value, int objectID, int min, int max){
	int tmp;

	tmp = *value;
	SliderByTime(&g->skstruct.drBuf, &g->skstruct.otherObject[2].src[objectID], &g->skstruct.otherObject[2].dst[objectID], &g->timer1, min, max, value, &g->KeyInput, objectID);
	if (tmp != *value) {
		ApplySoundFX(&g->audio, 0, (char)g->config.sound.disabledsp);
		return 1;
	}
	return 0;
}

//401f80
int InitFxParam(game *g, int fxNum){

	if (g->audio.param.fxType[fxNum] == 8 || g->audio.param.fxType[fxNum] != 3) {
		g->audio.param.fxParam[fxNum][1] = 0;
		g->audio.param.fxParam[fxNum][0] = 0;
	}
	else {
		g->audio.param.fxParam[fxNum][0] = 100;
		g->audio.param.fxParam[fxNum][1] = 0;
	}
	ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
	return 1;
}

//401fe0
//TODO : banner image load fails
void ThreadProc_LoadBanner(void *param) {
	game *g = (game*)param;

	CSTR path;
	CSTR dir = g->sSelect.bmsList[g->sSelect.cur_song].filepath.getDirectory();

	if (g->sSelect.bmsList[g->sSelect.cur_song].isBanner && g->skstruct.reloadbanner == 1) {
		path = dir;
		path.add(g->sSelect.bmsList[g->sSelect.cur_song].banner);

		if (ReloadImage(path, &g->skstruct.GrHandle[102]) == -1) {
			path = path.left(path.length() - 3); //TOFIX : a.jpg -> a. -> a..png
			path.add(".png");
			if (ReloadImage(path, &g->skstruct.GrHandle[102]) == -1) {
				g->sSelect.bmsList[g->sSelect.cur_song].isBanner = 0;
			}
		}
	}
	return;
}

//4021a0
int SetBmsFilter(game *g, sqlite3 *sql){

	g->sSelect.searchType = 0;
	g->sSelect.searchFocused = 1;
	g->sSelect.queryCount = 1;
	g->sSelect.curQuery[0] = g->sSelect.stack_query[g->sSelect.cur];
	g->sSelect.filterSort = g->config.select.sort;
	g->sSelect.searchMax = g->sSelect.stack_rivalID[g->sSelect.cur];
	g->sSelect.filterKey = g->config.select.key;
	g->sSelect.filterDifficulty = g->config.select.difficulty;
	int cur = (g->sSelect).cur_song;
	SONGDATA *bms = (g->sSelect).bmsList;
	g->sSelect.selTitle = bms[cur].title;
	g->sSelect.selFilepath = bms[cur].filepath;
	g->sSelect.selKey = bms[cur].keymode;
	g->sSelect.selFolder = bms[cur].folder;
	return 1;
}

//4024d0
uint SetObjectValue_Num(game *g, int op) {
	DATEDATA Date;

	switch (op) {
		case 10:
			return g->config.play.hiSpeed[0];
		case 11:
			return g->config.play.hiSpeed[1];
		case 12:
			return g->config.play.judgetiming;
		case 13:
			return g->config.play.target_percent;
		case 14:
			return g->config.play.p1_lanecoverv;
		case 15:
			return g->config.play.p2_lanecoverv;
		default:
			return -1;

		case 20:
			return g->timer1.FPS;
		case 21:
			GetDateTime(&Date);
			return Date.Year;
		case 22:
			GetDateTime(&Date);
			return Date.Mon;
		case 23:
			GetDateTime(&Date);
			return Date.Day;
		case 24:
			GetDateTime(&Date);
			return Date.Hour;
		case 25:
			GetDateTime(&Date);
			return Date.Min;
		case 26:
			GetDateTime(&Date);
			return Date.Sec;

		case 30:
			return g->gameplay.playerstat.playcount;
		case 31:
			return g->gameplay.playerstat.clear;
		case 32:
			return g->gameplay.playerstat.fail;
		case 33:
			return g->gameplay.playerstat.perfect;
		case 34:
			return g->gameplay.playerstat.great;
		case 35:
			return g->gameplay.playerstat.good;
		case 36:
			return g->gameplay.playerstat.bad;
		case 37:
			return g->gameplay.playerstat.poor;
		case 38:
			return g->gameplay.playerstat.combo;
		case 39:
			return g->gameplay.playerstat.maxcombo;
		case 40:
			return g->gameplay.playerstat.trial;
		case 41:
			return g->gameplay.playerstat.trial - 1;
		case 42:
			return g->sSelect.bmsList[g->sSelect.cur_song].level;
		case 45:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[0];
		case 46:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[1];
		case 47:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[2];
		case 48:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[3];
		case 49:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[4];

		case 50:
			return g->audio.param.eq_gain[0];
		case 51:
			return g->audio.param.eq_gain[1];
		case 52:
			return g->audio.param.eq_gain[2];
		case 53:
			return g->audio.param.eq_gain[3];
		case 54:
			return g->audio.param.eq_gain[4];
		case 55:
			return g->audio.param.eq_gain[5];
		case 56:
			return g->audio.param.eq_gain[6];
		case 57:
			return g->audio.param.volume_master;
		case 58:
			return g->audio.param.volume_key;
		case 59:
			return g->audio.param.volume_BGM;
		case 60:
			return g->audio.param.fxParam[0][0];
		case 61:
			return g->audio.param.fxParam[0][1];
		case 62:
			return g->audio.param.fxParam[1][0];
		case 63:
			return g->audio.param.fxParam[1][1];
		case 64:
			return g->audio.param.fxParam[2][0];
		case 65:
			return g->audio.param.fxParam[2][1];
		case 66:
			return g->audio.param.pitch_amount;

		case 70:
			if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14)
				return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score; 
			else 
				return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score / 20 * 10;
		case 71:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore;
		case 72:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes * 2;
		case 73:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore * 100) / (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes * 2);
			}
			break;
		case 74:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
		case 75:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo;
		case 76:
			return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp != -1) * g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp;
		case 77:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount;
		case 78:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount;
		case 79:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount - g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount;
		case 80:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat;
		case 81:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great;
		case 82:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good;
		case 83:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad;
		case 84:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor;
		case 85:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes) 
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 86:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes)
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 87:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes)
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 88:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes)
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 89:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes)
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 90:
		case 290:
			return (g->sSelect.bmsList[g->sSelect.cur_song].maxBPM) ? g->sSelect.bmsList[g->sSelect.cur_song].maxBPM : -1;
		case 91:
		case 291:
			return (g->sSelect.bmsList[g->sSelect.cur_song].minBPM) ? g->sSelect.bmsList[g->sSelect.cur_song].minBPM : -1;
		case 92:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking;
		case 93:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount;
		case 94:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate;
		case 95:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore - g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_exscore;
		case 96:
			return g->sSelect.bmsList[g->sSelect.cur_song].level;

		case 100:
			if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14)
				return g->gameplay.player[0].score_print;
			else
				return g->gameplay.player[0].score_print / 20 * 10;
		case 101:
			return g->gameplay.player[0].exscore;
		case 102:
			return g->gameplay.player[0].rate;
		case 103:
			return (int)((g->gameplay.player[0].rate) * 100.0) % 100;
		case 104:
			if ((g->gameplay.courseType == 0 || g->gameplay.courseType == 2) && g->procSelecter == 4) {
				return g->gameplay.player[0].now_combo_course;
			}
			return g->gameplay.player[0].now_combo;
		case 105:
			if ((g->gameplay.courseType == 0 || g->gameplay.courseType == 2) && g->procSelecter == 4) {
				return g->gameplay.player[0].max_combo_course;
			}
			return g->gameplay.player[0].max_combo;
		case 106:
			return g->gameplay.player[0].totalnotes;
		case 107:
			return (int)g->gameplay.player[0].HP_print / 2 * 2;
		case 108:
			return g->gameplay.player[0].exscore - g->gameplay.player[1].exscore;
		case 109:
		case 154:{
			if (g->gameplay.player[0].exscore == g->gameplay.player[0].totalnotes * 2) return 0;
			if (g->gameplay.player[0].totalnotes == 0) return 0;
			
			int rank = (g->gameplay.player[0].exscore * 9) / (g->gameplay.player[0].totalnotes * 2);

			if (rank < 1) rank = 1;
			else if (rank > 8) rank = 8;
			return g->gameplay.player[0].exscore - (g->gameplay.player[0].totalnotes * 2 * (rank + 1) / 9);
		}
		case 110:
			return g->gameplay.player[0].judgecount[5];
		case 111:
			return g->gameplay.player[0].judgecount[4];
		case 112:
			return g->gameplay.player[0].judgecount[3];
		case 113:
			return g->gameplay.player[0].judgecount[2];
		case 114:
			return g->gameplay.player[0].judgecount[0] + g->gameplay.player[0].judgecount[1];
		case 115:
			if (g->gameplay.player[0].totalnotes > 0) {
				return g->gameplay.player[0].exscore * 100 / (g->gameplay.player[0].totalnotes * 2);
			}
			break;
		case 116:
			if (g->gameplay.player[0].totalnotes > 0) {
				return g->gameplay.player[0].exscore * 10000 / (g->gameplay.player[0].totalnotes * 2) % 100;
			}
			break;

		case 120:
			if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14)
				return g->gameplay.player[1].score_print;
			else
				return g->gameplay.player[1].score_print / 20 * 10;
		case 121:
			return g->gameplay.player[1].exscore;
		case 122:
			return g->gameplay.player[1].rate;
		case 123:
			return (int)((g->gameplay.player[1].rate) * 100.0) % 100;
		case 124:
			if ((g->gameplay.courseType == 0 || g->gameplay.courseType == 2) && g->procSelecter == 4) {
				return g->gameplay.player[1].now_combo_course;
			}
			return g->gameplay.player[1].now_combo;
		case 125:
			if ((g->gameplay.courseType == 0 || g->gameplay.courseType == 2) && g->procSelecter == 4) {
				return g->gameplay.player[1].max_combo_course;
			}
			return g->gameplay.player[1].max_combo;
		case 126:
			return g->gameplay.player[1].totalnotes;
		case 127:
			return (int)g->gameplay.player[1].HP_print / 2 * 2;
		case 128:
			return g->gameplay.player[1].exscore - g->gameplay.player[0].exscore;
		case 129:{
			if (g->gameplay.player[1].exscore == g->gameplay.player[1].totalnotes * 2) return 0;
			if (g->gameplay.player[1].totalnotes == 0) return 0;

			int rank = (g->gameplay.player[1].exscore * 9) / (g->gameplay.player[1].totalnotes * 2);

			if (rank < 1) rank = 1;
			else if (rank > 8) rank = 8;
			return g->gameplay.player[1].exscore - (g->gameplay.player[1].totalnotes * 2 * (rank + 1) / 9);
		}
		case 130:
			return g->gameplay.player[1].judgecount[5];
		case 131:
			return g->gameplay.player[1].judgecount[4];
		case 132:
			return g->gameplay.player[1].judgecount[3];
		case 133:
			return g->gameplay.player[1].judgecount[2];
		case 134:
			return g->gameplay.player[1].judgecount[0] + g->gameplay.player[1].judgecount[1];
		case 135:
			if (g->gameplay.player[1].totalnotes > 0) {
				return g->gameplay.player[1].exscore * 100 / (g->gameplay.player[1].totalnotes * 2);
			}
			break;
		case 136:
			if (g->gameplay.player[1].totalnotes > 0) {
				return g->gameplay.player[1].exscore * 10000 / (g->gameplay.player[1].totalnotes * 2) % 100;
			}
			break;

		case 150:
			return g->gameplay.highScore.rate;
		case 151:
			return g->gameplay.targetScore.rate;
		case 152:
			return g->gameplay.player[0].exscore - g->gameplay.highScore.rate;
		case 153:
			return g->gameplay.player[0].exscore - g->gameplay.targetScore.rate;
			// 154 is same 109
		case 155:
			if (g->gameplay.player[0].totalnotes) {
				return g->gameplay.highScore.rate * 100 / (g->gameplay.player[0].totalnotes * 2);
			}
			break;
		case 156:
			if (g->gameplay.player[0].totalnotes) {
				return g->gameplay.highScore.rate * 10000 / (g->gameplay.player[0].totalnotes * 2) % 100;
			}
			break;
		case 157:
			if (g->gameplay.player[0].totalnotes) {
				return g->gameplay.targetScore.rate * 100 / (g->gameplay.player[0].totalnotes * 2);
			}
			break;
		case 158:
			if (g->gameplay.player[0].totalnotes) {
				return g->gameplay.targetScore.rate * 10000 / (g->gameplay.player[0].totalnotes * 2) % 100;
			}
			break;

		case 160:
			if (g->gameplay.BPM <= 0.0) return 1 + GetRand(998);
			else return g->gameplay.BPM;
		case 161:{
			int time = GetTimeLapse(41, &g->timer1);
			if (time / 1000 > 0) return time / 1000 / 60;
			break;
		}
		case 162:{
			int time = GetTimeLapse(41, &g->timer1);
			if (time / 1000 > 0) return time / 1000 % 60;
			break;
		}
		case 163:{
			int time = GetTimeLapse(41, &g->timer1) - g->gameplay.song_runtime;
			if (time / 1000 > 0) return time / 1000 / 60;
			break;
		}
		case 164:{
			int time = GetTimeLapse(41, &g->timer1) - g->gameplay.song_runtime;
			if (time / 1000 > 0) return time / 1000 % 60;
			break;
		}
		case 165:
			if (g->gameplay.loadObject_total > 0)
				return g->gameplay.loadObject_loaded * 100 / g->gameplay.loadObject_total;
			break;

		case 170:
			return g->sSelect.old.stat_exscore;
		case 171:
			if (g->skstruct.flag_flip) return g->gameplay.player[1].exscore;
			else return g->gameplay.player[0].exscore;
		case 172:
			if (g->skstruct.flag_flip) return g->gameplay.player[1].exscore - g->sSelect.old.stat_exscore;
			else return g->gameplay.player[0].exscore - g->sSelect.old.stat_exscore;
		case 173:
			return g->sSelect.old.stat_maxcombo;
		case 174:
			if (g->skstruct.flag_flip) return g->gameplay.player[1].max_combo;
			else return g->gameplay.player[0].max_combo;
		case 175:
			if (g->skstruct.flag_flip) return g->gameplay.player[1].max_combo- g->sSelect.old.stat_maxcombo;
			else return g->gameplay.player[0].max_combo - g->sSelect.old.stat_maxcombo;
		case 176:
			if (g->sSelect.old.minbp < 0) return 0;
			return g->sSelect.old.minbp;
		case 177:
			if (g->skstruct.flag_flip == 0) 
				return g->gameplay.player[0].totalnotes - g->gameplay.player[0].note_current + g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1];
			else
				return g->gameplay.player[1].totalnotes - g->gameplay.player[1].note_current + g->gameplay.player[1].judgecount[2] + g->gameplay.player[1].judgecount[1];
		case 178:{
			int oldbp = g->sSelect.old.minbp;
			if (oldbp < 0) oldbp = 0;
			if (g->skstruct.flag_flip == 0)
				return g->gameplay.player[0].totalnotes - g->gameplay.player[0].note_current + g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] - oldbp;
			else
				return g->gameplay.player[1].totalnotes - g->gameplay.player[1].note_current + g->gameplay.player[1].judgecount[2] + g->gameplay.player[1].judgecount[1] - oldbp;
		}
		case 179:
			return g->net.rankingData.myRanking;
		case 180:
		case 293:
			return g->net.rankingData.rankingCount;
		case 181:
			if (g->net.rankingData.rankingCount > 0)
				return (g->net.rankingData.clearPlayers[5] + g->net.rankingData.clearPlayers[4] + g->net.rankingData.clearPlayers[3] + g->net.rankingData.clearPlayers[2]) * 100 / g->net.rankingData.rankingCount;
			break;
		case 182:
			return g->sSelect.oldIRrank;
		case 183:
			if (g->skstruct.flag_flip == 0) {
				if (g->gameplay.player[0].totalnotes > 0) return g->sSelect.old.stat_exscore * 100 / (g->gameplay.player[0].totalnotes * 2);
				else return 0;
			}
			else{
				if (g->gameplay.player[1].totalnotes > 0) return g->sSelect.old.stat_exscore * 100 / (g->gameplay.player[1].totalnotes * 2);
				else return 0;
			}
		case 184:
			if (g->skstruct.flag_flip == 0) {
				if (g->gameplay.player[0].totalnotes > 0) return g->sSelect.old.stat_exscore * 10000 / (g->gameplay.player[0].totalnotes * 2) % 100;
				else return 0;
			}
			else {
				if (g->gameplay.player[1].totalnotes > 0) return g->sSelect.old.stat_exscore * 10000 / (g->gameplay.player[1].totalnotes * 2) % 100;
				else return 0;
			}

		case 200:
			return g->net.rankingData.rankingCount;
		case 201:
			return g->net.rankingData.totalPlaycount;

		case 210:
			return g->net.rankingData.clearPlayers[1];
		case 211:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[1] * 100) / g->net.rankingData.rankingCount;
			}
			break;
		case 212:
			return g->net.rankingData.clearPlayers[2];
		case 213:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[2] * 100) / g->net.rankingData.rankingCount;
			}
			break;
		case 214:
			return g->net.rankingData.clearPlayers[3];
		case 215:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[3] * 100) / g->net.rankingData.rankingCount;
			}
			break;
		case 216:
			return g->net.rankingData.clearPlayers[4];
		case 217:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[4] * 100) / g->net.rankingData.rankingCount;
			}
			break;
		case 218:
			return g->net.rankingData.clearPlayers[5];
		case 219:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[5] * 100) / g->net.rankingData.rankingCount;
			}
			break;

		case 220:
			if (g->net.IRstatus != 3) return  -1;
			return (g->net.waitTime - GetTimeLapse(177, &g->timer1)) / 1000.0;

		case 250:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[0];
		case 251:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[1];
		case 252:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[2];
		case 253:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[3];
		case 254:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[4];
		case 255:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[5];
		case 256:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[6];
		case 257:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[7];
		case 258:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[8];
		case 259:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[9];

		case 270:
			if ((g->sSelect.metaSelected.keymode != 7) && (g->sSelect.metaSelected.keymode != 14)) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_score / 20) * 10;
			}
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_score;
		case 271:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_exscore;
		case 272:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes * 2;
		case 273:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0)
				return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_exscore * 100 / (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes * 2);
			break;
		case 274:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
		case 275:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_maxcombo;
		case 276:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.minbp == -1) return 0;
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.minbp;
		case 277:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.playcount;
		case 278:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.clearcount;
		case 279:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.playcount - g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.clearcount;
		case 280:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_pgreat;
		case 281:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_great;
		case 282:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_good;
		case 283:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_bad;
		case 284:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_poor;
		case 285:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_pgreat * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
		case 286:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_great * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
		case 287:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_good * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
		case 288:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_bad * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
		case 289:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_poor * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
			//290 is same 90
			//291 is same 91
		case 292:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.IRranking;
			break;
			//293 is same 180
		case 294: //TOFIX : IR clear rate
			break;
	}
	return 0;
}




//4041e0
int StopSysSound(game *g){
	AUDIO *aud;

	aud = &g->audio;
	StopSound(aud, &g->audio.sysSound.select);
	StopSound(aud, &g->audio.sysSound.folder_open);
	StopSound(aud, &g->audio.sysSound.folder_close);
	StopSound(aud, &g->audio.sysSound.panel_open);
	StopSound(aud, &g->audio.sysSound.screenshot);
	StopSound(aud, &g->audio.sysSound.panel_close);
	StopSound(aud, &g->audio.sysSound.option_change);
	StopSound(aud, &g->audio.sysSound.decide);
	StopSound(aud, &g->audio.sysSound.clear);
	StopSound(aud, &g->audio.sysSound.fail);
	StopSound(aud, &g->audio.sysSound.stop);
	StopSound(aud, &g->audio.sysSound.mine);
	StopSound(aud, &g->audio.sysSound.scratch);
	StopSound(aud, &g->audio.sysSound.exselect);
	StopSound(aud, &g->audio.sysSound.exdecide);
	StopSound(aud, &g->audio.sysSound.courseclear);
	StopSound(aud, &g->audio.sysSound.coursefail);
	StopSound(aud, &g->audio.sysSound.difficulty);
	return 1;
}

//4042f0
int Print_ManiacOptions(game *g) {
	int* pOpVal;

	printfDx("MANIAC OPTIONS\n");
	printfDx("変態オプションの仮置き場です。\n");
	printfDx("トライアル改訂の際はこの手のオプションを多数取り入れる予定です。\n");
	printfDx("これらのオプションは、基本的にリプレイに反映されません。\n");
	printfDx("カーソルキーで選択・変更ができます。\n%03d/%03d\n\n\n",g->sSelect.maniac_cursor + 1, 21);

	int pg_cursor = g->sSelect.maniac_cursor / 10 * 10;
	int pg_max = pg_cursor + 10;
	for (; pg_cursor < pg_max; pg_cursor++) {
		if (pg_cursor == g->sSelect.maniac_cursor)
			printfDx("[*]");
		else
			printfDx("[ ]");

		switch (pg_cursor) {
			case 0:
				printfDx("HIDDEN/SUDDEN(1P)   ");
				pOpVal = &g->config.play.m_HIDSUD1;
				if (*pOpVal == 0) printfDx("OFF");
				if (*pOpVal == 1) printfDx("HIDDEN");
				if (*pOpVal == 2) printfDx("SUDDEN");
				if (*pOpVal == 3) printfDx("HIDDEN+SUDDEN");

				if (g->sSelect.maniac_cursor == 0) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 3, -1, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 3, 1, pOpVal);
				}
				break;

			case 1:
				printfDx("HIDDEN/SUDDEN(2P)   ");
				pOpVal = &g->config.play.m_HIDSUD2;
				if (*pOpVal == 0) printfDx("OFF");
				if (*pOpVal == 1) printfDx("HIDDEN");
				if (*pOpVal == 2) printfDx("SUDDEN");
				if (*pOpVal == 3) printfDx("HIDDEN+SUDDEN");

				if (g->sSelect.maniac_cursor == 1) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 3, -1, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 3, 1, pOpVal);
				}
				break;

			case 2:
				printfDx("EXTRA MODE LEVEL    ");
				pOpVal = &g->config.play.m_extra;
				if (*pOpVal == 0) printfDx("LEVEL 1");
				if (*pOpVal == 1) printfDx("LEVEL 2");
				if (*pOpVal == 2) printfDx("LEVEL 3");

				if (g->sSelect.maniac_cursor == 2) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 2, -1, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 2, 1, pOpVal);
				}
				break;

			case 3:
				printfDx("ADD NOTES           ");
				pOpVal = &g->config.play.m_addnote;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 3) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;

			case 4:
				printfDx("ADD LONGNOTES       ");
				pOpVal = &g->config.play.m_addlong;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 4) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;

			case 5:
				printfDx("ADD MINES           ");
				pOpVal = &g->config.play.m_addmine;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 5) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;

			case 6:
				printfDx("ACCELERATION        ");
				pOpVal = &g->config.play.m_accel;
				if (*pOpVal == 0) printfDx("OFF");
				if (*pOpVal == 1) printfDx("ACCELERATION");
				if (*pOpVal == 2) printfDx("DECELERATION");
				if (*pOpVal == 3) printfDx("RANDOM");

				if (g->sSelect.maniac_cursor == 6) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 3, -1, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 3, 1, pOpVal);
				}
				break;
			case 7:
				printfDx("SOFTLANDING         ");
				pOpVal = &g->config.play.m_softlanding;
				if (*pOpVal == 0) printfDx("OFF");
				if (*pOpVal == 1) printfDx("LEVEL 1");
				if (*pOpVal == 2) printfDx("LEVEL 2");

				if (g->sSelect.maniac_cursor == 7) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 2, -1, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 2, 1, pOpVal);
				}
				break;
			case 8:
				printfDx("EARTHQUAKE          ");
				pOpVal = &g->config.play.m_earthquake;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 8) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;

			case 9:
				printfDx("TORNADO             ");
				pOpVal = &g->config.play.m_tornado;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 9) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;

			case 10:
				printfDx("SUPERLOOP           ");
				pOpVal = &g->config.play.m_superloop;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 10) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 11:
				printfDx("GAMBOL              ");
				pOpVal = &g->config.play.m_gambol;
				if (*pOpVal == 0) printfDx("OFF");
				if (*pOpVal == 1) printfDx("LEVEL 1");
				if (*pOpVal == 2) printfDx("LEVEL 2");

				if (g->sSelect.maniac_cursor == 11) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 2, -1, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 2, 1, pOpVal);
				}
				break;
			case 12:
				printfDx("CHAR                ");
				pOpVal = &g->config.play.m_char;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 12) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 13:
				printfDx("HEARTBEAT           ");
				pOpVal = &g->config.play.m_heartbeat;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 13) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 14:
				printfDx("LOUDNESS            ");
				pOpVal = &g->config.play.m_loudness;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 14) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 15:
				printfDx("NABEATSU            ");
				pOpVal = &g->config.play.m_nabeatsu;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 15) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 16:
				printfDx("SIN CURVE           ");
				pOpVal = &g->config.play.m_sincurve;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 16) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 17:
				printfDx("WAVE                ");
				pOpVal = &g->config.play.m_wave;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 17) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 18:
				printfDx("SPIRAL              ");
				pOpVal = &g->config.play.m_spiral;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 18) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 19:
				printfDx("SIDEJUMP            ");
				pOpVal = &g->config.play.m_sidejump;
				if (*pOpVal == 0) {
					printfDx("OFF");
				}
				else {
					printfDx("%3d%%", *pOpVal);
				}

				if (g->sSelect.maniac_cursor == 19) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 20:
				printfDx("LUNARIS             ");
				if (g->config.play.m_lunaris == 0) printfDx("OFF");
				if (g->config.play.m_lunaris == 1) printfDx("ON");

				if (g->sSelect.maniac_cursor == 20) {
					if (g->KeyInput.inputID[D_DIK_LEFT] == 1) g->config.play.m_lunaris = g->config.play.m_lunaris == 0;
					if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) g->config.play.m_lunaris = g->config.play.m_lunaris == 0;
				}
				break;
			default:
				break;
		}

		printfDx("\n");
	}

	printfDx("\n\n\n");
	printfDx("[説明]\n");
	switch (g->sSelect.maniac_cursor) {
	case 0:
		printfDx("譜面の一部を隠すオプションです。旧バージョンの名残で左右別に設定できます。\n");
		if(g->config.play.m_HIDSUD1 == 1) printfDx("譜面の下半分が見えなくなります。\n");
		else if (g->config.play.m_HIDSUD1 == 2) printfDx("譜面の上半分が見えなくなります。\n");
		else if (g->config.play.m_HIDSUD1 == 3) printfDx("譜面は一瞬しか見えなくなります。\n");
		break;
	case 1:
		printfDx("譜面の一部を隠すオプションです。こちらは2P側。\n");
		if (g->config.play.m_HIDSUD2 == 1) printfDx("譜面の下半分が見えなくなります。\n");
		else if (g->config.play.m_HIDSUD2 == 2) printfDx("譜面の上半分が見えなくなります。\n");
		else if (g->config.play.m_HIDSUD2 == 3) printfDx("譜面は一瞬しか見えなくなります。\n");
		break;
	case 2:
		printfDx("EXTRA MODEの難度を上昇させることができます。\n");
		if (g->config.play.m_extra == 0) {
			printfDx("totalnotes 1-999      LEVEL 1\n");
			printfDx("totalnotes 1000-1199  LEVEL 2\n");
		}
		else if (g->config.play.m_extra == 1) {
			printfDx("totalnotes 1-999      LEVEL 2\n");
			printfDx("totalnotes 1000-1199  LEVEL 3\n");
		}
		else if (g->config.play.m_extra == 2) {
			printfDx("totalnotes 1-999      LEVEL 3\n");
			printfDx("totalnotes 1000-1199  LEVEL 3\n");
		}
		printfDx("totalnotes 1200-      LEVEL 3\n");
		break;
	case 3:
		printfDx("ノートを増量します。\n");
		break;
	case 4:
		printfDx("ロングノートを追加するオプションです。\n");
		break;
	case 5:
		printfDx("地雷を追加するオプションです。\n");
		break;
	case 6:
		printfDx("ノートを加速・減速させるオプションです。\n");
		if (g->config.play.m_accel == 1) printfDx("だんだん加速します。\n");
		else if (g->config.play.m_accel == 2) printfDx("だんだん減速します。\n");
		else if (g->config.play.m_accel == 3) printfDx("速くなったり遅くなったり。\n");
		break;
	case 7:
		printfDx("BPMに変化を与えるオプションです。\n");
		if (g->config.play.m_softlanding == 1) printfDx("小節ごとにBPMが変化します。\n");
		else if (g->config.play.m_softlanding == 2) printfDx("常にBPMが変化し続けます。\n");
		break;
	case 8:
		printfDx("正直酔います。\n");
		break;
	case 9:
		printfDx("竜巻を発生させる？オプションです。\n");
		break;
	case 10:
		printfDx("大回転。ほとんどゲームになりません。\n");
		break;
	case 11:
		printfDx("判定を厳しくするオプションです。\n");
		if(g->config.play.m_gambol == 1) printfDx("判定がVERYHARD固定になります。\n");
		else if (g->config.play.m_gambol == 2) printfDx("判定がさらに厳しくなります。\n");
		break;
	case 12:
		printfDx("たまに速度が通常の三倍のノートが降ってきます。\n");
		break;
	case 13:
		printfDx("ノートが拡張と収縮を繰り返します。\n");
		break;
	case 14:
		printfDx("( ﾟдﾟ ) ");
		break;
	case 15:
		if (g->config.play.m_nabeatsu  < 50) printfDx("3の倍数の秒数ときだけアホになります。\n");
		else printfDx("3の倍数と3が付く秒数のときだけアホになります。\n");
		break;
	case 16:
		printfDx("ノートが正弦曲線の軌道を描きます。\n");
		break;
	case 17:
		printfDx("ノートが上下に震動します。\n");
		break;
	case 18:
		printfDx("ノートが回転しつつ降ってきます　。\n");
		break;
	case 19:
		printfDx("ノートが反復横跳びっぽい動きをします。\n");
		break;
	case 20:
		printfDx("2008年エイプリルフールネタのテト○ス風ミニゲームをプレイできます。\n");
		break;
	}

	if (g->KeyInput.inputID[D_DIK_UP] == 1) {
		LoopInRange(0, 20, -1, &g->sSelect.maniac_cursor);
	}

	if (g->KeyInput.inputID[D_DIK_DOWN] == 1) {
		LoopInRange(0, 20, 1, &g->sSelect.maniac_cursor);
	}

	return 1;
}

//404f80
int GetSongCursor(game *g) {
	if (g->cmd_directplay) return 0;
	
	return ((g->sSelect.listSelectedBarFromScreenTop - g->skstruct.BAR_CENTER)
		+ (g->sSelect.listCalculatedBar / 1000 + (g->sSelect.listCalculatedBar % 1000 && g->sSelect.scrollDirection == 2 ? 1 : 0)) 
		+ g->sSelect.bmsListCount * 30) 
		% g->sSelect.bmsListCount;
}

//404fe0 thiscall in original code
SONGDATA * COPY_SONGDATA(SONGDATA *s1, SONGDATA *s2){
	s1->title = s2->title;
	s1->subtitle = s2->subtitle;
	s1->genre = s2->genre;
	s1->artist = s2->artist;
	s1->subartist = s2->subartist;
	s1->filepath = s2->filepath;
	s1->fulltitle = s2->fulltitle;
	s1->tag = s2->tag;
	s1->hash = s2->hash;
	s1->folder = s2->folder;
	s1->stagefile = s2->stagefile;
	s1->banner = s2->banner;
	s1->backBMP = s2->backBMP;
	s1->isStagefile = s2->isStagefile;
	s1->isBanner = s2->isBanner;
	s1->isBackBMP = s2->isBackBMP;
	s1->difficulty = s2->difficulty;
	s1->level = s2->level;
	s1->exlevel = s2->exlevel;
	s1->keymode = s2->keymode;
	s1->folderType = s2->folderType;
	s1->maxBPM = s2->maxBPM;
	s1->minBPM = s2->minBPM;
	s1->bga = s2->bga;
	s1->txt = s2->txt;
	s1->longnote = s2->longnote;
	s1->random = s2->random;
	s1->judge = s2->judge;
	s1->replayExist = s2->replayExist;
	s1->favorite = s2->favorite;
	s1->adddate = s2->adddate;
	s1->difficultyLevel[0] = s2->difficultyLevel[0];
	s1->difficultyLevel[1] = s2->difficultyLevel[1];
	s1->difficultyLevel[2] = s2->difficultyLevel[2];
	s1->difficultyLevel[3] = s2->difficultyLevel[3];
	s1->difficultyLevel[4] = s2->difficultyLevel[4];
	s1->difficultyExist[0] = s2->difficultyExist[0];
	s1->difficultyExist[1] = s2->difficultyExist[1];
	s1->difficultyExist[2] = s2->difficultyExist[2];
	s1->difficultyExist[3] = s2->difficultyExist[3];
	s1->difficultyExist[4] = s2->difficultyExist[4];
	s1->difficultyLevelBarLamp[0] = s2->difficultyLevelBarLamp[0];
	s1->difficultyLevelBarLamp[1] = s2->difficultyLevelBarLamp[1];
	s1->difficultyLevelBarLamp[2] = s2->difficultyLevelBarLamp[2];
	s1->difficultyLevelBarLamp[3] = s2->difficultyLevelBarLamp[3];
	s1->difficultyLevelBarLamp[4] = s2->difficultyLevelBarLamp[4];
	s1->difficultyCount = s2->difficultyCount;

	for (int i = 0; i < 10; i++) {
		s1->courseTitle[i] = s2->courseTitle[i];
	}
	for (int i = 0; i < 10; i++) {
		s1->courseSubtitle[i] = s2->courseSubtitle[i];
	}
	for (int i = 0; i < 10; i++) {
		s1->courseHash[i] = s2->courseHash[i];
	}

	s1->courseLevel[0] = s2->courseLevel[0];
	s1->courseLevel[1] = s2->courseLevel[1];
	s1->courseLevel[2] = s2->courseLevel[2];
	s1->courseLevel[3] = s2->courseLevel[3];
	s1->courseLevel[4] = s2->courseLevel[4];
	s1->courseLevel[5] = s2->courseLevel[5];
	s1->courseLevel[6] = s2->courseLevel[6];
	s1->courseLevel[7] = s2->courseLevel[7];
	s1->courseLevel[8] = s2->courseLevel[8];
	s1->courseLevel[9] = s2->courseLevel[9];
	s1->courseStageDifficulty[0] = s2->courseStageDifficulty[0];
	s1->courseStageDifficulty[1] = s2->courseStageDifficulty[1];
	s1->courseStageDifficulty[2] = s2->courseStageDifficulty[2];
	s1->courseStageDifficulty[3] = s2->courseStageDifficulty[3];
	s1->courseStageDifficulty[4] = s2->courseStageDifficulty[4];
	s1->courseStageDifficulty[5] = s2->courseStageDifficulty[5];
	s1->courseStageDifficulty[6] = s2->courseStageDifficulty[6];
	s1->courseStageDifficulty[7] = s2->courseStageDifficulty[7];
	s1->courseStageDifficulty[8] = s2->courseStageDifficulty[8];
	s1->courseStageDifficulty[9] = s2->courseStageDifficulty[9];
	s1->courseTotalnote[0] = s2->courseTotalnote[0];
	s1->courseTotalnote[1] = s2->courseTotalnote[1];
	s1->courseTotalnote[2] = s2->courseTotalnote[2];
	s1->courseTotalnote[3] = s2->courseTotalnote[3];
	s1->courseTotalnote[4] = s2->courseTotalnote[4];
	s1->courseTotalnote[5] = s2->courseTotalnote[5];
	s1->courseTotalnote[6] = s2->courseTotalnote[6];
	s1->courseTotalnote[7] = s2->courseTotalnote[7];
	s1->courseTotalnote[8] = s2->courseTotalnote[8];
	s1->courseTotalnote[9] = s2->courseTotalnote[9];
	s1->courseID = s2->courseID;
	s1->courseStageCount = s2->courseStageCount;
	s1->coursePlayable = s2->coursePlayable;
	s1->courseReadOnly = s2->courseReadOnly;
	s1->courseKeys[0] = s2->courseKeys[0];
	s1->courseKeys[1] = s2->courseKeys[1];
	s1->courseKeys[2] = s2->courseKeys[2];
	s1->courseKeys[3] = s2->courseKeys[3];
	s1->courseKeys[4] = s2->courseKeys[4];
	s1->courseKeys[5] = s2->courseKeys[5];
	s1->courseKeys[6] = s2->courseKeys[6];
	s1->courseKeys[7] = s2->courseKeys[7];
	s1->courseKeys[8] = s2->courseKeys[8];
	s1->courseKeys[9] = s2->courseKeys[9];
	s1->courseType = s2->courseType;
	s1->courseIR = s2->courseIR;
	s1->grHandle = s2->grHandle;
	
	memcpy(&s1->mybest, &s2->mybest, sizeof(STATUS));
	memcpy(&s1->rivalRecord, &s2->rivalRecord, sizeof(STATUS));
	return s1;
}

//

//4055d0
int LoadFontForSongs(game *gs, char flag) {
	if (flag == 0) {
		for (int i = 0; i < gs->skstruct.num_of_ImageFont; i++) {
			for (int j = 0; j < gs->sSelect.bmsListCount; j++) {
				if (i == 0 || i == 1) {
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.bmsList[j].title);
				}
				else {
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.bmsList[j].genre);
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.bmsList[j].artist);
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.bmsList[j].subtitle);
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.bmsList[j].subartist);
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.bmsList[j].tag);
				}
			}
		}
		return 1;
	}

	else {
		for (int i = 0; i < gs->skstruct.num_of_ImageFont; i++) {
			for (int j = 0; j < gs->sSelect.prevListCount; j++) {
				if (i == 0 || i == 1) {
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.prevList[j].title);
				}
				else {
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.prevList[j].genre);
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.prevList[j].artist);
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.prevList[j].subtitle);
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.prevList[j].subartist);
					LoadFontForText(&gs->skstruct.ImageFonts[i], &gs->sSelect.prevList[j].tag);
				}
			}
		}
		return 1;
	}
}

//405870
int ProcI_Decide(game *g) {

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.scenetime && g->procPhase == 1) {
		g->procSelecter = 4;
		return 1;
	}

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start && (g->KeyInput.inputID[D_DIK_RETURN] == 1 || g->KeyInput.mouse_buttonL == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[7] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[6] == 1) && g->procPhase == 1) {
		SetTimeLapse(3, &g->timer1);
		g->procPhase = 3;
		return 1;
	}

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start && (g->KeyInput.mouse_buttonR == 1 || g->KeyInput.inputID[D_DIK_ESCAPE] == 1 || (g->KeyInput.p1_buttonInput[12] == 2 && g->KeyInput.p1_buttonInput[13] == 2) || (g->KeyInput.p2_buttonInput[12] == 2 && g->KeyInput.p2_buttonInput[13] == 2)) && g->procPhase == 1) {
		SetTimeLapse(2, &g->timer1);
		g->procPhase = 2;
		StopSysSound(g);
		return 1;
	}

	if (g->procPhase == 2) {
		if (GetTimeLapse(2, &g->timer1) > g->skstruct.fadeout || g->skstruct.fadeout == 0) {
			g->procSelecter = 2;
			return 1;
		}
	}
	else if (g->procPhase == 3) {
		if (GetTimeLapse(3, &g->timer1) > g->skstruct.close || g->skstruct.close == 0) {
			g->procSelecter = 4;
			return 1;
		}
	}
	return 1;
}

//405a70
uint ConvertOptionHistory(game *g){

	int clear;
	uint ret;

	ret = 0;
	if ( g->config.play.p1_assist == 1 || g->config.play.p2_assist == 1 ) {
		if (g->gameplay.player[0].clearType > 1) {
			ret = 0x1000000;
		}
	}
	else {
		clear = g->gameplay.player[0].clearType;
		if (clear == 2) {
			if (g->config.play.gaugeOption[0] == 3) {
				ret = 8;
			}
			return ret;
		}
		if (clear > 2) {
			ret = 0;
			switch (g->config.play.gaugeOption[0]) {
				case 0:
					ret = 1;
					break;
				case 1:
					ret = 2;
					break;
				case 2:
					ret = 4;
					break;
				case 3:
					ret = 8;
					break;
				case 4:
					ret = 0x10;
					break;
				case 5:
					ret = 0x20;
					break;
				case 6:
					ret = 0x40;
					break;
				case 7:
					ret = 0x80;
			}
			switch (g->config.play.random[0]) {
				case 0:
					ret = ret | 0x100;
					break;
				case 1:
					ret = ret | 0x200;
					break;
				case 2:
					ret = ret | 0x400;
					break;
				case 3:
					ret = ret | 0x800;
					break;
				case 4:
					ret = ret | 0x1000;
					break;
				case 5:
					ret = ret | 0x2000;
					break;
				case 6:
					ret = ret | 0x4000;
					break;
				case 7:
					ret = ret | 0x8000;
			}
			switch (g->config.play.m_HIDSUD1) {
				case 0:
					return ret | 0x10000;
				case 1:
					return ret | 0x20000;
				case 2:
					return ret | 0x40000;
				case 3:
					return ret | 0x80000;
				case 4:
					return ret | 0x100000;
				case 5:
					return ret | 0x200000;
				case 6:
					return ret | 0x400000;
				case 7:
					return ret | 0x800000;
			}
		}
	}
	return ret;
}

//405bf0 
int LogGraphPlayData(GRAPHDATA *grp, PLAYERSTATUS *pstat, int time, int endtime){

	if (grp->cursor < 1000 && time <= endtime && 0 < endtime) {
		do {
			if ((time * 1000) / endtime < grp->cursor) {
				return 1;
			}
			grp->hp[grp->cursor] = pstat->HP;
			grp->combo[grp->cursor] = pstat->now_combo;
			grp->exscore[grp->cursor] = pstat->exscore;
			grp->cursor++;
		} while (grp->cursor < 1000);
		return 1;
	}
	return 0;
}

//405c90
int LogGraphData(GRAPHDATAB *grp, int val, int time, int endtime)
{
	if (999 < grp->cursor) {
		return 0;
	}
	if ((time <= endtime) && (0 < endtime)) {
		do {
			if ((time * 1000) / endtime < grp->cursor) {
				return 1;
			}
			grp->val[grp->cursor] = val;
			grp->cursor = grp->cursor + 1;
		} while (grp->cursor < 1000);
		return 1;
	}
	return 0;
}

//405cf0
int LogGraphPlayerDataToEnd(GRAPHDATA *grp, PLAYERSTATUS *pstat){

	if (grp->cursor >= 1) {
		for (int i = grp->cursor; i < 1000; i++) {
			grp->hp[i] = pstat->HP;
			grp->combo[i] = pstat->now_combo;
			grp->exscore[i] = pstat->exscore;
		}
		return 1; 	
	}
	else {
		for (int i = grp->cursor; i < 1000; i++) {
			grp->hp[i] = grp->hp[0];
			grp->combo[i] = grp->combo[0];
			grp->exscore[i] = grp->exscore[0];
		}
		return 1;
	}
}

//405da0
int CheckClear(PLAYERSTATUS *pstat, int gaugeType, char is2p){

	pstat->clearType = 1;
	if (pstat->totalnotes == pstat->max_combo) {
		pstat->clearType = 5;
		return pstat->clearType;
	}
	if (gaugeType == 1 || gaugeType == 5 || gaugeType == 4) {
		if ( pstat->note_current == pstat->totalnotes && pstat->HP >= 2.0) {
			pstat->clearType = 4;
		}
	}
	else {
		if (is2p) {
			if (pstat->note_current != pstat->totalnotes)
				return pstat->clearType;
			if (pstat->HP >= 2.0) {
				pstat->clearType = (gaugeType != 3) + 2;
				return pstat->clearType;
			}
		}
		if (pstat->note_current == pstat->totalnotes && pstat->HP >= 80.0) {
			if (pstat->judgecount[2] + pstat->judgecount[1] < 10) { // TOFIX?? : when bp < 10, consider it as hard clear. but it never works.
				pstat->clearType = 4;
			}
			pstat->clearType = (gaugeType != 3) + 2;
			return pstat->clearType;
		}
	}
	return pstat->clearType;
}


//405Fb0
int ApplyJudgeToScore(int judge, game *g, int player, int lane, Timer *T, char isReplay) {

	if (g->gameplay.replay.status == 2) return 0; //playing replay
	if (g->gameplay.replay.status == 1 && isReplay == 0) { //recording replay, not playing replay??
		AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), player * 2 + (lane >= 10) + 210, judge);
	}

	if (g->gameplay.isAutoplay == 0 && g->config.play.m_lunaris == 0 && g->gameplay.replay.status != 2) {

		switch (judge) {
			case 0: 
				g->gameplay.playerstat.poor++;
				break;
			case 1:
				g->gameplay.playerstat.poor++;
				break;
			case 2:
				g->gameplay.playerstat.bad++;
				break;
			case 3:
				g->gameplay.playerstat.good++;
				break;
			case 4:
				g->gameplay.playerstat.great++;
				break;
			case 5:
				g->gameplay.playerstat.perfect++;
				break;
		}
		if (judge > 2) {
			g->gameplay.playerstat.combo++;
			if (g->gameplay.playerstat.combo > g->gameplay.playerstat.maxcombo)
				g->gameplay.playerstat.maxcombo = g->gameplay.playerstat.combo;
		}
		if (judge == 2 || judge == 1) {
			g->gameplay.playerstat.combo = 0;
		}
	}

	if (!(0 <= judge && judge <= 5)) return 0;

	if (judge >= 1) {
		if (player == 0) {
			if(isReplay == 0) g->gameplay.p1Score.AddJudgeQueue(judge);
			if (g->gameplay.ghostBattle == 0 && g->gameplay.isAutoplay == 0 && g->config.play.battle != 1) {
				while (g->gameplay.highScore.DealJudgeFromQueue() == 0) {}
				while (g->gameplay.targetScore.DealJudgeFromQueue() == 0) {}

				if (g->gameplay.targetType == 1) {
					g->gameplay.highScore.SetScore(&g->gameplay.player[1], 0);
				}
				else if (g->gameplay.targetType == 2){
					g->gameplay.targetScore.SetScore(&g->gameplay.player[1], 0);
				}
			}

		}
		g->gameplay.player[player].note_current++ ;
		g->gameplay.player[player].note_current2++ ;
		if (g->gameplay.player[player].note_current == g->gameplay.player[player].totalnotes) SetTimeLapse(143 + player, T); //final note timer

	}
	else {
		if (judge == 0) {
			if (isReplay == 0 && player == 0) g->gameplay.p1Score.AddJudgeQueue(player); //0
		}
	}

	g->gameplay.player[player].recent_judge = judge;
	if (judge < 3) {
		if (g->gameplay.ghostBattle == 0 || player == 0) {
			g->gameplay.lastMissTime = GetTimeWrap();
		}
		g->gameplay.misslayerTime[player]= GetTimeWrap();
	}

	if (judge == 0) {
		g->gameplay.player[player].judgecount[1]++;
		g->gameplay.player[player].judgecount2[1]++;
	}
	else {
		g->gameplay.player[player].judgecount[judge]++;
		g->gameplay.player[player].judgecount2[judge]++;
	}

	int hp = ((int)g->gameplay.player[player].HP / 2) * 2;
	double damage;
	if (hp <= 30 && (!(g->config.play.gaugeOption[player] == 0 || g->config.play.gaugeOption[player] == 3) || g->gameplay.is2Pplay != 0) && judge <= 2) {
		damage = g->gameplay.player[player].judge_damage[judge] * 0.6;
	}
	else {
		damage = g->gameplay.player[player].judge_damage[judge];
	}
	g->gameplay.player[player].HP += damage;

	if (g->gameplay.is2Pplay == 0 && g->gameplay.player[player].HP <= 2.0 && (g->config.play.gaugeOption[player] == 0 || g->config.play.gaugeOption[player] == 3)) {
		g->gameplay.player[player].HP = 2.0;
	}

	if (g->gameplay.player[player].HP >= 100.0) g->gameplay.player[player].HP = 100.0;
	if (g->gameplay.player[player].HP < 0.0) g->gameplay.player[player].HP = 0.0;

	int newhp = ((int)g->gameplay.player[player].HP / 2) * 2;
	if (hp <= 0) {
		g->gameplay.player[player].HP = 0; //?
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

	if (g->gameplay.player[player].totalnotes > 0) {
		g->gameplay.player[player].score = (g->gameplay.player[player].judgecount[3] + (g->gameplay.player[player].judgecount[4] + g->gameplay.player[player].judgecount[5] * 2) * 2) * 50000 / g->gameplay.player[player].totalnotes;
	}
	g->gameplay.player[player].exscore = g->gameplay.player[player].judgecount[4] + g->gameplay.player[player].judgecount[5] * 2;

	if (g->gameplay.player[player].note_current > 0) g->gameplay.player[player].rate = g->gameplay.player[player].exscore * 100 / (g->gameplay.player[player].note_current * 2);

	switch (judge) {
		case 1:
		case 2:
			g->gameplay.player[player].now_combo = 0;
			g->gameplay.player[player].now_combo_course = 0;
			break;
		case 3:
		case 4:
		case 5:
			g->gameplay.player[player].now_combo++;
			if (g->gameplay.player[player].now_combo > g->gameplay.player[player].max_combo) g->gameplay.player[player].max_combo = g->gameplay.player[player].now_combo;
			g->gameplay.player[player].now_combo_course++;
			if (g->gameplay.player[player].now_combo_course > g->gameplay.player[player].max_combo_course) g->gameplay.player[player].max_combo_course = g->gameplay.player[player].now_combo_course;
			break;
	}

	if (g->gameplay.player[player].now_combo == g->gameplay.player[player].totalnotes) SetTimeLapse(48 + player, T); //fullcombo timer

	if (lane >= 10) {
		if (player == 0) {
			g->gameplay.player[1].judge_draw = g->gameplay.player[0].recent_judge;
			g->gameplay.player[1].combo_song_draw = g->gameplay.player[0].now_combo;
			g->gameplay.player[1].combo_draw = g->gameplay.player[0].now_combo_course;
			SetTimeLapse(47, T);
			return 1;
		}
	}
	else if (player == 0) {
		g->gameplay.player[0].judge_draw = g->gameplay.player[0].recent_judge;
		g->gameplay.player[0].combo_song_draw = g->gameplay.player[0].now_combo;
		g->gameplay.player[0].combo_draw = g->gameplay.player[0].now_combo_course;
		SetTimeLapse(46, T);
		return 1;
	}
	if (player == 1 && lane >= 10) {
		g->gameplay.player[1].judge_draw = g->gameplay.player[1].recent_judge;
		g->gameplay.player[1].combo_song_draw = g->gameplay.player[1].now_combo;
		g->gameplay.player[1].combo_draw = g->gameplay.player[1].now_combo_course;
		SetTimeLapse(47, T);
	}
	return 1;
}

//406530
int ApplyJudge(int judge, game *g, int player, int lane, int damage) {

	if (g->gameplay.replay.status == 2) return 0; //playing replay
	if (g->gameplay.replay.status == 1) {
		AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), player*2 + (lane>=10) + 214, damage); //214 = 0xD6
	}

	if (judge >= 0) {
		if (judge == 0) g->gameplay.player[player].judgecount[1]++;
		else g->gameplay.player[player].judgecount[judge]++;
	}
	g->gameplay.player[player].judgecount[judge]++;
	g->gameplay.player[player].HP -= damage;

	if (g->gameplay.player[player].HP <= 2.0 && (g->config.play.gaugeOption[player] == 0 || g->config.play.gaugeOption[player] == 3)) {
		g->gameplay.player[player].HP = 2.0;
	}
	if (g->gameplay.player[player].HP >= 100.0) {
		g->gameplay.player[player].HP = 100.0;
	}
	if (g->gameplay.player[player].HP < 0.0) {
		g->gameplay.player[player].HP = 0.0;
	}

	if ((int)g->gameplay.player[player].HP / 2 != 50) ResetTimeLapse(44 + player, &g->timer1);

	if (lane >= 10) {
		if (player == 0) {
			g->gameplay.player[1].judge_draw = g->gameplay.player[0].recent_judge;
			g->gameplay.player[1].combo_song_draw = g->gameplay.player[0].now_combo;
			g->gameplay.player[1].combo_draw = g->gameplay.player[0].now_combo_course;
		}
		else if (player == 1) {
			g->gameplay.player[1].judge_draw = g->gameplay.player[1].recent_judge;
			g->gameplay.player[1].combo_song_draw = g->gameplay.player[1].now_combo;
			g->gameplay.player[1].combo_draw = g->gameplay.player[1].now_combo_course;
		}

		SetTimeLapse(47, &g->timer1);
	}
	else if (player == 0) {
		g->gameplay.player[0].judge_draw = g->gameplay.player[0].recent_judge;
		g->gameplay.player[0].combo_song_draw = g->gameplay.player[0].now_combo;
		g->gameplay.player[0].combo_draw = g->gameplay.player[0].now_combo_course;

		SetTimeLapse(46, &g->timer1);
	}

	return 1;
}

//406710
int DrawNotes(game *g, skstruct *sk, Timer *T, CONFIG_PLAY *cfg) {
	//TODO : refactor, test maniac mode
	DSTdraw tDdraw;
	int key;

	SoundGetCurrentTime(&g->audio, &g->gameplay.muon);
	NONE_004b6770();
	int drawStartHeight = 0;

	if (sk->dst_NOTE[1].dstCount > 0) {
		if (sk->horizontal == 0) {
			drawStartHeight = g->skstruct.adjust.size_y - sk->dst_NOTE[1].draw->h;
		}
		else {
			drawStartHeight = g->skstruct.adjust.size_x - sk->dst_NOTE[1].draw->w;
		}
	}

	int now = GetTimeWrap();

	for (int i = 0; i < 10; i++) {
		tDdraw = SetDSTdrawByTime(sk->dst_NOTE[i], GetTimeLapse(sk->dst_NOTE[i].timer, T)); //TODO : check this reeally memcpy

		sk->drBuf.isHidSud[i] = cfg->m_HIDSUD1;
		if (cfg->m_HIDSUD1 == 1) {
			sk->drBuf.top[i] = 0;
			sk->drBuf.bottom[i] = tDdraw.sortID * 0.6;
		}
		else if (cfg->m_HIDSUD1 == 2) {
			sk->drBuf.top[i] = tDdraw.sortID * 0.6;
			sk->drBuf.bottom[i] = 480;
		}
		else if (cfg->m_HIDSUD1 == 3) {
			sk->drBuf.top[i] = tDdraw.sortID * 0.55;
			sk->drBuf.bottom[i] = tDdraw.sortID * 0.65;
		}
		else {
			sk->drBuf.top[i] = 0;
			sk->drBuf.bottom[i] = 480;
		}
	}
	for (int i = 10; i < 20; i++) {
		tDdraw = SetDSTdrawByTime(sk->dst_NOTE[i], GetTimeLapse(sk->dst_NOTE[i].timer, T)); //TODO : check this reeally memcpy

		sk->drBuf.isHidSud[i] = cfg->m_HIDSUD2;
		if (cfg->m_HIDSUD2 == 1) {
			sk->drBuf.top[i] = 0;
			sk->drBuf.bottom[i] = (100 - cfg->p1_lanecoverv) * tDdraw.sortID / 200 + cfg->p1_lanecoverv * tDdraw.sortID / 100;
		}
		else if (cfg->m_HIDSUD2 == 2) {
			sk->drBuf.top[i] = (100 - cfg->p1_lanecoverv) * tDdraw.sortID / 200 + cfg->p1_lanecoverv * tDdraw.sortID / 100;
			sk->drBuf.bottom[i] = 480;
		}
		else if (cfg->m_HIDSUD2 == 3) {
			sk->drBuf.top[i] = (100 - cfg->p1_lanecoverv) * tDdraw.sortID / 100 * 4 / 10 + cfg->p1_lanecoverv * tDdraw.sortID / 100;
			sk->drBuf.bottom[i] = (100 - cfg->p1_lanecoverv) * tDdraw.sortID / 100 * 6 / 10 + cfg->p1_lanecoverv * tDdraw.sortID / 100;
		}
		else {
			sk->drBuf.top[i] = 0;
			sk->drBuf.bottom[i] = 480;
		}
	}
	g->gameplay.nabeatsu_x = 0.0;
	g->gameplay.nabeatsu_y = 0.0;
	g->gameplay.unusedX_7bf50 = 0.0;
	g->gameplay.unusedY_7bf54 = 0.0;
	if (cfg->m_nabeatsu > 0) {
		int sec = (int)(GetTimeLapse(41, T) / 1000.0) % 60;
		bool pass =	(sec == (sec / 3 * 3));
		if (cfg->m_nabeatsu >= 50) {
			pass = (30 <= sec && sec < 40) || pass;
			if (sec % 10 == 3) pass = true;
		}
		if ((int)(GetTimeLapse(41, T) / 1000.0) != 0 && pass) {

			int s = sin((int)GetTimeLapse(41, T) % 12345 / 12345.0 * pi2) * 200.0 + 1000.0; // 1000.0 +- 200.0
			g->gameplay.nabeatsu_x = sin((int)GetTimeLapse(41, T) % s / (double)s * pi2) * cfg->m_nabeatsu;

			s = sin((int)GetTimeLapse(41, T) % 10000 / 10000.0 * pi2) * 300.0 + 1234.0; // 1234.0 +- 300.0
			g->gameplay.nabeatsu_y = sin((int)GetTimeLapse(41, T) % s / (double)s * pi2) * cfg->m_nabeatsu * 0.75;
		}
	}
	if (cfg->m_earthquake > 0) {
		GetTimeLapse(41, T);
		g->gameplay.earthquake_x = sin((int)GetTimeLapse(41, T) % 1000 / 1000.0 * pi2) * cfg->m_earthquake;

		GetTimeLapse(41, T);
		g->gameplay.earthquake_y = sin((int)GetTimeLapse(41, T) % 1234 / 1234.0 * pi2) * cfg->m_earthquake * 0.75;
	}

	SoundGetCurrentTime(&g->audio, &g->gameplay.muon);
	NONE_004b6770();
	double songtimer = RealTimeToBMSTime(&g->gameplay, GetTimeLapse(41, T) + cfg->judgetiming);
	if (g->gameplay.bpmChangedBmstime > 0) {
		songtimer = g->gameplay.bpmChangedBmstime * 2 - songtimer;
	}

	int lineCount = 0;
	for (int i = g->gameplay.bmsobj_line.draw_count; i < g->gameplay.bmsobj_line.size; i++) {
		
		if (lineCount == 300) break;
		lineCount++;

		int speed = cfg->hiSpeed[0];
		if (cfg->hiSpeed[1] < cfg->hiSpeed[0] && cfg->battle == 1) {
			speed = cfg->hiSpeed[1];
		}
		//TOFIX : 1p 2p doesn't match
		if ((sk->horizontal == 0 && sk->dst_LINE[0].draw->y + (songtimer - g->gameplay.bmsobj_line.notes[i].bmsTiming)* speed * g->gameplay.speedmultiplier * (cfg->basespeed / 100.0) / 600.0 > drawStartHeight)
			|| (sk->horizontal == 1 && (g->gameplay.bmsobj_line.notes[i].bmsTiming - songtimer)* speed * g->gameplay.speedmultiplier * (cfg->basespeed / 100.0) / 640.0 > drawStartHeight)) {
						
			float p1_y = sk->adjust.note_1p_y + (songtimer - g->gameplay.bmsobj_line.notes[i].bmsTiming)* cfg->hiSpeed[0] * g->gameplay.speedmultiplier * (cfg->basespeed / 100.0) / 600.0;
			float p2_y = sk->adjust.note_2p_y + (songtimer - g->gameplay.bmsobj_line.notes[i].bmsTiming)* cfg->hiSpeed[0] * g->gameplay.speedmultiplier * (cfg->basespeed / 100.0) / 600.0;
			if (p2_y > 0.0) p2_y = 0;
			if (p1_y > 0.0) p1_y = 0.0;

			p2_y += g->gameplay.nabeatsu_y;
			if (cfg->battle == 1) {
				p2_y = sk->adjust.note_2p_y + (songtimer - g->gameplay.bmsobj_line.notes[i].bmsTiming)* cfg->hiSpeed[1] * g->gameplay.speedmultiplier * (cfg->basespeed / 100.0) / 600.0;
			}

			AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_LINE[0], &sk->dst_LINE[0], T, g->gameplay.nabeatsu_x + sk->adjust.note_1p_x, g->gameplay.nabeatsu_y + p1_y, 255, sk->adjust.size_x, 0.0, 1);
			AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_LINE[1], &sk->dst_LINE[1], T, g->gameplay.nabeatsu_x + sk->adjust.note_2p_x, p2_y, 255, sk->adjust.size_x, 0.0, 1);

			if (g->gameplay.bmsobj_line.notes[i].bmsTiming < songtimer) {
				g->gameplay.bmsobj_line.draw_count++;
			}
		}
	}

	for (int key = 0; key < 20; key++) {
		int speed;
		if (key < 10 || cfg->battle != 1) speed = cfg->hiSpeed[0];
		else speed = cfg->hiSpeed[1];

		bool isDpGbattle = (key > 9 && g->gameplay.ghostBattle);

		for (int i = g->gameplay.bmsobj_note[key].draw_count; i < g->gameplay.bmsobj_note[key].size; i++) {
			if (sk->dst_NOTE[key].dstCount > 0) {
				if (sk->horizontal != 0 || sk->dst_NOTE[key].draw->y + cfg->basespeed / 100.0 * g->gameplay.speedmultiplier * speed * (songtimer - g->gameplay.bmsobj_note[key].notes[i].bmsTiming) / 600.0 <= drawStartHeight) {
					if (sk->horizontal != 1 || (g->gameplay.bmsobj_note[key].notes[i].bmsTiming - songtimer) * g->gameplay.speedmultiplier * speed * (cfg->basespeed / 100.0) / 600.0 >= 640.0) {
						break;
					}
				}

				int note_x, note_y, notesize_x, notesize_y, noteL_y;
				LaneStruct &thisLane = g->gameplay.bmsobj_note[key];

				note_y = (songtimer - thisLane.notes[i].bmsTiming) * (cfg->basespeed / 100.0) * g->gameplay.speedmultiplier * speed / 600.0;
				noteL_y = (songtimer - thisLane.notes[i].bmsTiming_ln) * (cfg->basespeed / 100.0) * g->gameplay.speedmultiplier * speed / 600.0;

				float pos_y = 0.0;
				if (sk->dst_NOTE[key].draw->y != 0.0) {
					pos_y = -note_y / (float)sk->dst_NOTE[key].draw->y;
				}
				float pos_yl = 0.0;
				if (sk->dst_NOTE[key].draw->y != 0.0) {
					pos_yl = -noteL_y / (float)sk->dst_NOTE[key].draw->y;
				}

				if (cfg->m_accel == 1) {
					note_y = sin(pos_y * pi_half) * (-sk->dst_NOTE[key].draw->y);
					noteL_y = sin(pos_yl * pi_half) * (-sk->dst_NOTE[key].draw->y);
				}
				else if (cfg->m_accel == 2) {
					note_y = (1-cos(pos_y * pi_half)) * (-sk->dst_NOTE[key].draw->y);
					noteL_y = (1-cos(pos_yl * pi_half)) * (-sk->dst_NOTE[key].draw->y);
				}
				else if (cfg->m_accel == 3) {
					if ((i + key) % 2 == 0) {
						note_y = sin(pos_y * pi_half) * (-sk->dst_NOTE[key].draw->y);
						noteL_y = sin(pos_yl * pi_half) * (-sk->dst_NOTE[key].draw->y);
					}
					else {
						note_y = (1 - cos(pos_y * pi_half)) * (-sk->dst_NOTE[key].draw->y);
						noteL_y = (1 - cos(pos_yl * pi_half)) * (-sk->dst_NOTE[key].draw->y);
					}
				}

				if (note_y > 0.0) note_y = 0.0;
				if (noteL_y > 0.0) noteL_y = 0.0;
				noteL_y += g->gameplay.nabeatsu_y;
				
				if (key < 10) {
					note_x = sk->adjust.note_1p_x + g->gameplay.nabeatsu_x + 0.0;
					note_y = sk->adjust.note_1p_y + g->gameplay.nabeatsu_y + note_y;
				}
				else {
					note_x = sk->adjust.note_2p_x + g->gameplay.nabeatsu_x + 0.0;
					note_y = sk->adjust.note_2p_y + g->gameplay.nabeatsu_y + note_y;
				}
				notesize_x = sk->adjust.size_x + 0.0;
				notesize_y = sk->adjust.size_y + 0.0;

				if (cfg->m_heartbeat > 0) {
					notesize_x += sin((((i + key * 2) * 50 + now) % 500 / 500.0) * 2 * pi) * cfg->m_heartbeat * 2;
					notesize_y += sin((((i + key * 2) * 50 + now) % 500 / 500.0) * 2 * pi) * cfg->m_heartbeat;
				}
				if (cfg->m_tornado > 0) {
					note_x = (float)cfg->m_tornado * note_y * -0.5 * sin(((double)key / 10.0 + (double)(now % 1000) / 1000.0) * pi2) / 100.0;
				}
				if (cfg->m_superloop > 0) {
					VECTOR v;
					v.x = note_x;
					v.y = note_y;
					int d = (120 - cfg->m_superloop) * 50;
					
					VectorRotationZ(&v, &v, (now % d) / (double)d * pi2);
					noteL_y = v.y + noteL_y - note_y;
					note_y = v.y;
					note_x = v.x;
				}
				if (cfg->m_sincurve > 0) {
					note_x += sin((double)(cfg->m_sincurve / 10) * pi * pos_y) * cfg->m_sincurve;
				}
				if (cfg->m_spiral > 0) {
					float f = (i % 2 == 0) ? 1.0 : -1.0;
					f *= pos_y * cfg->m_spiral;
					note_x += sin((double)((now + (i * 7 + key) * 30) % 300 / 300.0 * pi2)) * f;
					note_y += cos((double)((now + (i * 7 + key) * 30) % 300 / 300.0 * pi2)) * f;
					noteL_y += cos((double)((now + (i * 7 + key) * 30) % 300 / 300.0 * pi2)) * f;
				}
				if (cfg->m_sidejump > 0) {
					float f = (i % 2 == 0) ? 1.0 : -1.0;
					f *= pos_y * cfg->m_sidejump;
					note_x += sin((double)((key * 50 + i * 77 + now) % 500 / 500.0 * pi2)) * f;
					note_y -= fabs(cos((double)((key * 50 + i * 77 + now) % 500 / 500.0 * pi2)) * f);
					noteL_y -= fabs(cos((double)((key * 50 + i * 77 + now) % 500 / 500.0 * pi2)) * f);
				}
				if (cfg->m_wave > 0) {
					note_y += (float)cfg->m_wave * pos_y * pos_y * sin((double) ((key * 100 + now) % 1000 / 1000.0 * pi2));
					noteL_y += (float)cfg->m_wave * pos_yl * pos_yl * sin((double)((key * 100 + now) % 1000 / 1000.0 * pi2));
				}
				if (cfg->m_char > 0 && ((key + i) % ((100 - cfg->m_char) * 2 / 10 + 2)) == 0) {
					note_y *= 3.0;
					noteL_y *= 3.0;
				}
				if (g->skstruct.horizontal == 1) {
					int tmp = note_y;
					note_y = note_x;
					note_x = tmp * -1.0;
				}

				if (g->gameplay.isAutoplay == 0 && g->gameplay.bmsobj_note[key].autoplay == 0 && isDpGbattle == 0) {
					if (g->gameplay.bmsobj_note[key].notes[i].channel > 0) {
						AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_MINE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
					}
					else if (g->gameplay.bmsobj_note[key].notes[i].realTiming_ln <= g->gameplay.bmsobj_note[key].notes[i].realTiming || g->gameplay.bmsobj_note[key].notes[i].realTiming_ln < 0.0) {
						AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_NOTE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
					}
					else {
						AddDrawingBuffer_LN(&sk->drBuf, &sk->src_LN_START[key], &sk->src_LN_END[key], &sk->src_LN_BODY[key], &sk->dst_NOTE[key], T, note_x, note_y, noteL_y, (g->gameplay.bmsobj_note[key].notes[i].active == -1) ? 128 : 255, notesize_x, notesize_y);
					}

					if ((g->gameplay.bmsobj_note[key].notes[i].active == -1 || (g->gameplay.player[0].totalnotes <= g->gameplay.player[0].note_current && g->gameplay.replay.status == 2)) && g->gameplay.bmsobj_note[key].notes[i].bmsTiming <= songtimer && g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln <= songtimer) {
						g->gameplay.bmsobj_note[key].draw_count++;
					}
				}
				else {
					bool isAuto = g->gameplay.bmsobj_note[key].autoplay;

					if (isAuto && !isDpGbattle) {
						if (g->gameplay.bmsobj_note[key].notes[i].channel > 0) {
							AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_AUTO_MINE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
						}
						else if ((g->gameplay.bmsobj_note[key].notes[i].realTiming_ln <= g->gameplay.bmsobj_note[key].notes[i].realTiming || g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln <= g->gameplay.bmsobj_note[key].notes[i].bmsTiming || g->gameplay.bmsobj_note[key].notes[i].realTiming_ln < 0.0)) {
							AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_AUTO_NOTE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
						}
						else {
							AddDrawingBuffer_LN(&sk->drBuf, &sk->src_AUTO_LN_START[key], &sk->src_AUTO_LN_END[key], &sk->src_AUTO_LN_BODY[key], &sk->dst_NOTE[key], T, note_x, note_y, noteL_y, 255, notesize_x, notesize_y);
						}

						if (g->gameplay.bmsobj_note[key].notes[i].bmsTiming <= songtimer && g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln <= songtimer) {
							if (g->gameplay.bmsobj_note[key].notes[i].channel <= 0) {
								int p = 0;
								if (cfg->battle == 1) p = key / 10;

								g->gameplay.player[p].note_current++;
								g->gameplay.player[p].note_current2++;
							}
							g->gameplay.bmsobj_note[key].draw_count++;
						}
					}
					else {
						if (g->gameplay.bmsobj_note[key].notes[i].channel > 0){
							if (isAuto) {
								AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_AUTO_MINE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
							}
							else {
								AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_MINE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
							}
						}
						else if ((g->gameplay.bmsobj_note[key].notes[i].realTiming_ln <= g->gameplay.bmsobj_note[key].notes[i].realTiming || g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln <= g->gameplay.bmsobj_note[key].notes[i].bmsTiming || g->gameplay.bmsobj_note[key].notes[i].realTiming_ln < 0.0)) {
							if (isAuto) {
								AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_AUTO_NOTE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
							}
							else {
								AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_NOTE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
							}
						}
						else {
							if (isAuto) {
								AddDrawingBuffer_LN(&sk->drBuf, &sk->src_AUTO_LN_START[key], &sk->src_AUTO_LN_END[key], &sk->src_AUTO_LN_BODY[key], &sk->dst_NOTE[key], T, note_x, note_y, noteL_y, 255, notesize_x, notesize_y);
							}
							else {
								AddDrawingBuffer_LN(&sk->drBuf, &sk->src_LN_START[key], &sk->src_LN_END[key], &sk->src_LN_BODY[key], &sk->dst_NOTE[key], T, note_x, note_y, noteL_y, 255, notesize_x, notesize_y);
							}

							if (g->gameplay.bmsobj_note[key].notes[i].bmsTiming <= songtimer && g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln >= songtimer && GetTimeLapse(70 + key, T) == -1.0) {
								ResetTimeLapse(50 + key, T);
								SetTimeLapse(70 + key, T);
								SetTimeLapse(100 + key, T);
								ResetTimeLapse(120 + key, T);
							}
						}
						

						if (g->gameplay.bmsobj_note[key].notes[i].bmsTiming <= songtimer && g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln <= songtimer) {
							if (g->gameplay.bmsobj_note[key].notes[i].channel <= 0) {
								SetTimeLapse(50 + key, T);
								ResetTimeLapse(70 + key, T);

								if (GetTimeLapse(100 + key, T) == -1.0) {
									SetTimeLapse(100 + key, T);
									ResetTimeLapse(120 + key, T);
								}

								if (isDpGbattle) {
									do {
										ApplyJudgeToScore(g->gameplay.targetScore.GetJudgeFromQueue(), g, 1, key, T, 0);
									} while (g->gameplay.targetScore.DealJudgeFromQueue() == 0);
									g->gameplay.bmsobj_note[key].draw_count++;
								}
								else if (cfg->battle == 1 || (g->skinData.select > 11 && g->procSelecter == 7)) {
									if (g->net.rankingData.target_ID <= 0 || g->gameplay.targetScore.ghostReadCount <= 0) {
										ApplyJudgeToScore(5, g, key / 10, key, T, 0);
										g->gameplay.bmsobj_note[key].draw_count++;
									}
									else {
										do {
											ApplyJudgeToScore(g->gameplay.targetScore.GetJudgeFromQueue(), g, key/10, key, T, 0);
										} while (g->gameplay.targetScore.DealJudgeFromQueue() == 0);
										g->gameplay.bmsobj_note[key].draw_count++;
									}
								}
								else {
									if (g->net.rankingData.target_ID <= 0 || g->gameplay.targetScore.ghostReadCount <= 0) {
										ApplyJudgeToScore(5, g, 0, key, T, 0);
										g->gameplay.bmsobj_note[key].draw_count++;
									}
									else {
										do {
											ApplyJudgeToScore(g->gameplay.targetScore.GetJudgeFromQueue(), g, 0, key, T, 0);
										} while (g->gameplay.targetScore.DealJudgeFromQueue() == 0);
										g->gameplay.bmsobj_note[key].draw_count++;
									}
								}
							}
							else {
								g->gameplay.bmsobj_note[key].draw_count++;
							}
						}
					}
				}
			}
		}
		
		if ((g->gameplay.isAutoplay || isDpGbattle) && GetTimeLapse(50 + key, T) >= 80.0 && GetTimeLapse(100 + key, T) > 0.0) {
			ResetTimeLapse(100 + key, T);
			SetTimeLapse(120 + key, T);
		}
	}

	return 1;
}

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

//408490
int DrawJudgeCombo(game *g, skstruct *sk, Timer *T, CONFIG_PLAY *cfg){
	int judge, combo;

	/* not silent */
	if (cfg->autojudge != 2) {
		for (int i = 0; i < 2; i++) {
			judge = g->gameplay.player[i].judge_draw;
			combo = g->gameplay.player[i].combo_draw;
			if (judge < 3) {
				combo = 0;
			}

			if (i == 0) {
				AddDrawingBuffer_JudgeCombo(&sk->drBuf, &sk->src_NOWJUDGE_1P[judge], &sk->dst_NOWJUDGE_1P[judge], &sk->src_NOWCOMBO_1P[judge], &sk->dst_NOWCOMBO_1P[judge], T, combo, sk->adjust.judge_x, sk->adjust.judge_y);
			}
			else {
				AddDrawingBuffer_JudgeCombo(&sk->drBuf, &sk->src_NOWJUDGE_2P[judge], &sk->dst_NOWJUDGE_2P[judge], &sk->src_NOWCOMBO_2P[judge], &sk->dst_NOWCOMBO_2P[judge], T, combo, sk->adjust.judge_x, sk->adjust.judge_y);
			}
		}
	}
	return 1;
}

//408580
int DrawHPgauge(game *g){
	char survival;

	for (int i = 0; i < 2; i++) {
		if (g->gameplay.is2Pplay == 0 && (g->config.play.gaugeOption[i] == 0 || g->config.play.gaugeOption[i] == 3)) {
			survival = 0;
		}
		else {
			survival = 1;
		}
		AddDrawingBuffer_Gauge(&g->skstruct.drBuf, &g->skstruct.src_GROOVEGAUGE[i], &g->skstruct.dst_GROOVEGAUGE[i], &g->timer1, g->gameplay.player[i].HP_print / 2, survival);
	}
	return 1;
}

//408620
int FlipScore(game *g){
	
	PLAYERSTATUS tmp;
	GRAPHDATA tmp2;

	ErrorLogAdd("左右のスコアを反転させました\n");
	
	memcpy(&tmp, &g->gameplay.player[0], sizeof(PLAYERSTATUS));
	memcpy(&g->gameplay.player[0], &g->gameplay.player[1], sizeof(PLAYERSTATUS));
	memcpy(&g->gameplay.player[1], &tmp, sizeof(PLAYERSTATUS));

	memcpy(&tmp2, &g->gameplay.statgraph[0], sizeof(GRAPHDATA));
	memcpy(&g->gameplay.statgraph[0], &g->gameplay.statgraph[1], sizeof(GRAPHDATA));
	memcpy(&g->gameplay.statgraph[1], &tmp2, sizeof(GRAPHDATA));

	g->gameplay.player[0].clearType = g->gameplay.player[1].clearType;

	return 1;
}

//4086d0
int CheckCourseClear(game *g) {

	if (g->gameplay.courseStageNow < g->gameplay.courseStageCount - 1) {
		for (int i = g->gameplay.courseStageNow - 1; i < g->gameplay.courseStageCount; i++) {
			g->gameplay.player[0].total_note += g->sSelect.bmsList[g->sSelect.cur_song].courseTotalnote[i];
			g->gameplay.player[1].total_note += g->sSelect.bmsList[g->sSelect.cur_song].courseTotalnote[i];
		}
	}

	for (int p = 0; p < 2; p++) {
		memcpy(g->gameplay.player[p].judgecount, g->gameplay.player[p].judgecount2, sizeof(int) * 6);
		g->gameplay.player[p].exscore = g->gameplay.player[p].judgecount[4] + g->gameplay.player[p].judgecount[5] * 2;
		g->gameplay.player[p].note_current = g->gameplay.player[p].note_current2;
		g->gameplay.player[p].totalnotes = g->gameplay.player[p].total_note;
		g->gameplay.player[p].max_combo = g->gameplay.player[p].max_combo_course;
		
		g->gameplay.player[p].clearType = 1;

		if (g->gameplay.player[p].HP < 2.0 || g->gameplay.courseStageNow < g->gameplay.courseStageCount - 1) {
			g->gameplay.player[p].clearType = 1;
		}
		else if (g->gameplay.player[p].total_note == g->gameplay.player[p].max_combo_course) {
			g->gameplay.player[p].clearType = 5;
		}
		else if (g->config.play.gaugeOption[p] == 1 || g->config.play.gaugeOption[p] == 5 || g->config.play.gaugeOption[p] == 4) {
			if (g->gameplay.player[p].note_current2 == g->gameplay.player[p].total_note && g->gameplay.player[p].HP > 2.0) {
				g->gameplay.player[p].clearType = 4;
			}
		}
		else if (g->gameplay.player[p].note_current2 == g->gameplay.player[p].total_note && g->gameplay.player[p].HP > 2.0) {
			g->gameplay.player[p].clearType = (g->config.play.gaugeOption[p] != 3) + 2;
		}
	}

	return 1;
}

//408850
int ProcS_Result(game *g) {

	LoadScene(&g->skstruct, g->config.skin.skinFilePath[7], g->skinData.Data[g->skinData.skinID[7]].informationP5, 0);

	if (g->skstruct.flag_flip)
		FlipScore(g);
	
	if (g->rec.recMode == 2 && g->is_recordmode) {
		g->audio.bitRate = 264600;
	}

	if (g->gameplay.player[0].clearType < 2) {
		PlaySound(&g->audio, &g->audio.sysSound.fail, g->audio.chnBgm, -1);
	}
	else {
		PlaySound(&g->audio, &g->audio.sysSound.clear, g->audio.chnBgm, -1);
	}
	return 1;
}

//408900
int Proc_Result(game *g, skstruct *sk, Timer *T) {

	if (GetTimeLapse(0, T) > sk->src_GAUGECHART_1P[0].op3 && GetTimeLapse(150, T) < 0.0) {
		SetTimeLapse(150, T);
	}
	if (GetTimeLapse(0, T) > sk->src_GAUGECHART_1P[0].op4 && GetTimeLapse(151, T) < 0.0) {
		SetTimeLapse(151, T);
	}

	if (g->config.play.battle == 1 || sk->flag_flip == 0 || g->gameplay.ghostBattle) {
		
		if (sk->src_GAUGECHART_1P[0].op1 <= 0) return 0;

		int val = g->gameplay.statgraph[0].hp[0];
		int length = ChangeValueByTime(0.0, sk->src_GAUGECHART_1P[0].op1, sk->src_GAUGECHART_1P[0].op3, sk->src_GAUGECHART_1P[0].op4, GetTimeLapse(0, T), 0);
		for (int i = 0; i < length; i += sk->dst_GAUGECHART_1P[0].draw->w) {
			int axis = i * 1000 / sk->src_GAUGECHART_1P[0].op1;

			int last = 0;
			while (val != g->gameplay.statgraph[0].hp[axis]) {
				
				if (g->gameplay.is2Pplay == 0 && g->config.play.gaugeOption[0] != 1 && g->config.play.gaugeOption[0] != 2 && g->config.play.gaugeOption[0] != 5 && g->config.play.gaugeOption[0] != 4 && val < 80) {
					
					int targetval = sk->src_GAUGECHART_1P[0].op2 * val / (-100);
					if (last != targetval) {
						AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_1P[0], &sk->dst_GAUGECHART_1P[0], T, i, targetval);
						last = targetval;
					}
				}
				else {
					int targetval = sk->src_GAUGECHART_1P[1].op2 * val / (-100);
					if (last != targetval) {
						AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_1P[1], &sk->dst_GAUGECHART_1P[1], T, i, targetval);
						last = targetval;
					}
				}

				if (val < g->gameplay.statgraph[0].hp[axis]) val++;
				if (val > g->gameplay.statgraph[0].hp[axis]) val--;
			}

			if (g->gameplay.is2Pplay == 0 && g->config.play.gaugeOption[0] != 1 && g->config.play.gaugeOption[0] != 2 && g->config.play.gaugeOption[0] != 5 && g->config.play.gaugeOption[0] != 4 && g->gameplay.statgraph[0].hp[axis] < 80) {
				AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_1P[0], &sk->dst_GAUGECHART_1P[0], T, i, sk->src_GAUGECHART_1P[0].op2 * g->gameplay.statgraph[0].hp[axis] / (-100));
			}
			else {
				AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_1P[1], &sk->dst_GAUGECHART_1P[1], T, i, sk->src_GAUGECHART_1P[1].op2 * g->gameplay.statgraph[0].hp[axis] / (-100));
			}
		}
	}

	if (g->config.play.battle == 1 || sk->flag_flip != 0 || g->gameplay.ghostBattle) {
		int p = 1;
		if (g->config.play.battle != 1 && sk->flag_flip != 0) p = 0;

		if (sk->src_GAUGECHART_2P[0].op1 <= 0) return 0;

		int val = g->gameplay.statgraph[1].hp[0];
		int length = ChangeValueByTime(0.0, sk->src_GAUGECHART_2P[0].op1, sk->src_GAUGECHART_2P[0].op3, sk->src_GAUGECHART_2P[0].op4, GetTimeLapse(0, T), 0);
		for (int i = 0; i < length; i += sk->dst_GAUGECHART_2P[0].draw->w) {
			int axis = i * 1000 / sk->src_GAUGECHART_2P[0].op1;

			int last = 0;
			while (val != g->gameplay.statgraph[1].hp[axis]) {

				if (g->gameplay.is2Pplay == 0 && g->config.play.gaugeOption[p] != 1 && g->config.play.gaugeOption[p] != 2 && val < 80) {

					int targetval = sk->src_GAUGECHART_2P[0].op2 * val / (-100);
					if (last != targetval) {
						AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_2P[0], &sk->dst_GAUGECHART_2P[0], T, i, targetval);
						last = targetval;
					}
				}
				else {
					int targetval = sk->src_GAUGECHART_2P[1].op2 * val / (-100);
					if (last != targetval) {
						AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_2P[1], &sk->dst_GAUGECHART_2P[1], T, i, targetval);
						last = targetval;
					}
				}

				if (val < g->gameplay.statgraph[1].hp[axis]) val++;
				if (val > g->gameplay.statgraph[1].hp[axis]) val--;
			}

			if (g->gameplay.is2Pplay == 0 && g->config.play.gaugeOption[p] != 1 && g->config.play.gaugeOption[p] != 2 && g->gameplay.statgraph[1].hp[axis] < 80) {
				AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_2P[0], &sk->dst_GAUGECHART_2P[0], T, i, sk->src_GAUGECHART_2P[0].op2 * g->gameplay.statgraph[1].hp[axis] / (-100));
			}
			else {
				AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_2P[1], &sk->dst_GAUGECHART_2P[1], T, i, sk->src_GAUGECHART_2P[1].op2 * g->gameplay.statgraph[1].hp[axis] / (-100));
			}
		}
	}


	if (sk->src_SCORECHART[0].graphcount >= 1 && g->gameplay.player[0].totalnotes > 0) {
		if (sk->src_SCORECHART[0].op1 <= 0) return 0;

		int val = g->gameplay.statgraph[0].exscore[0];
		int length = ChangeValueByTime(0.0, sk->src_SCORECHART[0].op1, sk->src_SCORECHART[0].op3, sk->src_SCORECHART[0].op4, GetTimeLapse(0, T), 0);
		for (int i = 0; i < length; i += sk->dst_SCORECHART[0].draw->w) {
			int axis = i * 1000 / sk->src_SCORECHART[0].op1;

			int last = 0;
			while (val != g->gameplay.statgraph[0].exscore[axis]) {

				if (last != sk->src_SCORECHART[0].op2 * val / (g->gameplay.player[0].totalnotes * -2)) {
					AddDrawingBuffer_Object(&sk->drBuf, &sk->src_SCORECHART[0], &sk->dst_SCORECHART[0], T, i, sk->src_SCORECHART[0].op2 * val / (g->gameplay.player[0].totalnotes * -2));
					last = sk->src_SCORECHART[0].op2 * val / (g->gameplay.player[0].totalnotes * -2);
				}
				if (val < g->gameplay.statgraph[0].exscore[axis]) val++;
				if (val > g->gameplay.statgraph[0].exscore[axis]) val--;
			}

			AddDrawingBuffer_Object(&sk->drBuf, &sk->src_SCORECHART[0], &sk->dst_SCORECHART[0], T, i, g->gameplay.statgraph[0].exscore[axis] * sk->src_SCORECHART[0].op2 / (g->gameplay.player[0].totalnotes * -2));
		}
	}

	if (sk->src_SCORECHART[1].graphcount >= 1 && g->gameplay.player[0].totalnotes > 0) {
		if (sk->src_SCORECHART[1].op1 <= 0) return 0;

		int val = g->gameplay.rategraph[0].val[0];
		int length = ChangeValueByTime(0.0, sk->src_SCORECHART[1].op1, sk->src_SCORECHART[1].op3, sk->src_SCORECHART[1].op4, GetTimeLapse(0, T), 0);
		for (int i = 0; i < length; i += sk->dst_SCORECHART[1].draw->w) {
			int axis = i * 1000 / sk->src_SCORECHART[1].op1;

			int last = 0;
			while (val != g->gameplay.rategraph[0].val[axis]) {

				if (last != sk->src_SCORECHART[1].op2 * val / (g->gameplay.player[0].totalnotes * -2)) {
					AddDrawingBuffer_Object(&sk->drBuf, &sk->src_SCORECHART[1], &sk->dst_SCORECHART[1], T, i, sk->src_SCORECHART[1].op2 * val / (g->gameplay.player[0].totalnotes * -2));
					last = sk->src_SCORECHART[1].op2 * val / (g->gameplay.player[0].totalnotes * -2);
				}
				if (val < g->gameplay.rategraph[0].val[axis]) val++;
				if (val > g->gameplay.rategraph[0].val[axis]) val--;
			}

			AddDrawingBuffer_Object(&sk->drBuf, &sk->src_SCORECHART[1], &sk->dst_SCORECHART[1], T, i, g->gameplay.rategraph[0].val[axis] * sk->src_SCORECHART[1].op2 / (g->gameplay.player[0].totalnotes * -2));
		}
	}

	if (sk->src_SCORECHART[2].graphcount >= 1 && g->gameplay.player[0].totalnotes > 0) {
		if (sk->src_SCORECHART[2].op1 <= 0) return 0;

		int val = g->gameplay.rategraph[1].val[0];
		int length = ChangeValueByTime(0.0, sk->src_SCORECHART[2].op1, sk->src_SCORECHART[2].op3, sk->src_SCORECHART[2].op4, GetTimeLapse(0, T), 0);
		for (int i = 0; i < length; i += sk->dst_SCORECHART[2].draw->w) {
			int axis = i * 1000 / sk->src_SCORECHART[2].op1;

			int last = 0;
			while (val != g->gameplay.rategraph[1].val[axis]) {
				
				if (last != sk->src_SCORECHART[2].op2 * val / (g->gameplay.player[0].totalnotes * -2)) {
					AddDrawingBuffer_Object(&sk->drBuf, &sk->src_SCORECHART[2], &sk->dst_SCORECHART[2], T, i, sk->src_SCORECHART[2].op2 * val / (g->gameplay.player[0].totalnotes * -2));
					last = sk->src_SCORECHART[2].op2 * val / (g->gameplay.player[0].totalnotes * -2);
				}
				if (val < g->gameplay.rategraph[1].val[axis]) val++;
				if (val > g->gameplay.rategraph[1].val[axis]) val--;
			}

			AddDrawingBuffer_Object(&sk->drBuf, &sk->src_SCORECHART[2], &sk->dst_SCORECHART[2], T, i, g->gameplay.rategraph[1].val[axis] * sk->src_SCORECHART[2].op2 / (g->gameplay.player[0].totalnotes * -2));
		}
	}

	return 1;
}

//409300
char fWaitHiScoreUpdateInput = 0;
int ProcI_Result(game *g) {

	if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[D_DIK_ESCAPE] == 1 || g->KeyInput.inputID[D_DIK_RETURN] == 1
		|| g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p1_buttonInput[7] == 1
		|| g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[7] == 1
		|| fWaitHiScoreUpdateInput || (g->rec.recMode == 2 && GetTimeLapse(0, &g->timer1) >= 5000.0))
		&& g->procPhase == 1 && GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start) {

		if (GetTimeLapse(151, &g->timer1) == -1.0) {
			SetTimeLapse(151, &g->timer1);
			g->skstruct.src_GAUGECHART_1P[0].op4 = GetTimeLapse(0, &g->timer1);
			g->skstruct.src_GAUGECHART_2P[0].op4 = GetTimeLapse(0, &g->timer1);
			g->skstruct.src_SCORECHART[0].op4 = GetTimeLapse(0, &g->timer1);
			g->skstruct.src_SCORECHART[1].op4 = GetTimeLapse(0, &g->timer1);
			g->skstruct.src_SCORECHART[2].op4 = GetTimeLapse(0, &g->timer1);

			SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
		}
		else {
			if (GetTimeLapse(151, &g->timer1) <= g->skstruct.startinput_rank || GetTimeLapse(152, &g->timer1) != -1.0 || (g->net.isOnline && (g->net.isOnline != 1 || g->net.hHandle))) {
				if (GetTimeLapse(151, &g->timer1) > g->skstruct.startinput_rank && GetTimeLapse(152, &g->timer1) == -1.0 && g->net.isOnline == 1 && g->net.hHandle) {
					fWaitHiScoreUpdateInput = 1;
				}
				else if (GetTimeLapse(152, &g->timer1) > g->skstruct.startinput_update && GetTimeLapse(2, &g->timer1) == -1.0) {
					SetTimeLapse(2, &g->timer1);
					SetFadeOut(&g->audio, g->skstruct.fadeout);
					g->procPhase = 2;
					SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
				}
			}
			else {
				fWaitHiScoreUpdateInput = 0;
				if (g->gameplay.isNosave == 0) {
					SetTimeLapse(152, &g->timer1);
					SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
				}
				else {
					SetTimeLapse(2, &g->timer1);
					SetFadeOut(&g->audio, g->skstruct.fadeout);
					g->procPhase = 2;
					SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
				}
			}
		}
	}

	else if (g->procPhase == 2) {
		if (GetTimeLapse(2, &g->timer1) > g->skstruct.fadeout || g->skstruct.fadeout == 0) {
			
			if (g->audio.replay2avi) {
				RecordFadeout(&g->audio, g->audio.aviTimer - 500.0, 500.0);
			}

			if ( (g->KeyInput.p1_buttonInput[1] == 2 || g->KeyInput.p1_buttonInput[3] == 2 ||	g->KeyInput.p1_buttonInput[5] == 2 || g->KeyInput.p1_buttonInput[7] == 2 ||
				g->KeyInput.p2_buttonInput[1] == 2 || g->KeyInput.p2_buttonInput[3] == 2 || g->KeyInput.p2_buttonInput[5] == 2 || g->KeyInput.p2_buttonInput[7] == 2) 
				&& (g->KeyInput.p1_buttonInput[2] == 2 || g->KeyInput.p1_buttonInput[4] == 2 || g->KeyInput.p1_buttonInput[6] == 2 ||
					g->KeyInput.p2_buttonInput[2] == 2 || g->KeyInput.p2_buttonInput[4] == 2 || g->KeyInput.p2_buttonInput[6] == 2) 
				&& g->gameplay.replay.status != 2 && g->config.play.m_lunaris == 0 && (g->gameplay.courseType == -1 || g->gameplay.courseType == 1)) {

				g->procSelecter = 4;
				g->gameplay.flag_retry = 1;

				if (g->skstruct.flag_flip) {
					//same as FlipScore();, but no errorlog
					PLAYERSTATUS tmp;
					GRAPHDATA tmp2;

					memcpy(&tmp, &g->gameplay.player[0], sizeof(PLAYERSTATUS));
					memcpy(&g->gameplay.player[0], &g->gameplay.player[1], sizeof(PLAYERSTATUS));
					memcpy(&g->gameplay.player[1], &tmp, sizeof(PLAYERSTATUS));

					memcpy(&tmp2, &g->gameplay.statgraph[0], sizeof(GRAPHDATA));
					memcpy(&g->gameplay.statgraph[0], &g->gameplay.statgraph[1], sizeof(GRAPHDATA));
					memcpy(&g->gameplay.statgraph[1], &tmp2, sizeof(GRAPHDATA));

					g->gameplay.player[0].clearType = g->gameplay.player[1].clearType;
				}
			}
			else {
				g->procSelecter = 2;
				g->gameplay.flag_retry = 0;
			}

		}
	}

	Proc_Result(g, &g->skstruct, &g->timer1);
	return 1;
}

//409860
int ProcI_Keyconfig(game *g) {
	int fndkey;
	if (g->KeyInput.config_key >= 0 && g->KeyInput.config_button_inMap > 0) {
		fndkey = FindPressedKey(&g->KeyInput);
		//TOFIX : block F1, F12 & allow NUMPAD
		if(fndkey > 0 && (fndkey <= 0x3B || fndkey >= 0x58) && (fndkey != D_DIK_ESCAPE) && (fndkey != D_DIK_RETURN) && (fndkey != D_DIK_UP) && (fndkey != D_DIK_DOWN) && (fndkey != D_DIK_LEFT) && (fndkey != D_DIK_RIGHT)){
			g->config.input.buttonMap[g->KeyInput.config_button_inMap][g->KeyInput.config_key] = fndkey;
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
	
	if ( g->KeyInput.inputID[D_DIK_DELETE] == 1 && g->KeyInput.config_key >= 0 && g->KeyInput.config_button_inMap > 0) {
		g->config.input.buttonMap [g->KeyInput.config_button_inMap] [g->KeyInput.config_key] = 0;
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		ProcS_Keyconfig(g);
	}
	
	if (g->KeyInput.inputID[D_DIK_F1] == 1) {
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

	if (g->KeyInput.inputID[D_DIK_F2] == 1) {
		memset(g->config.input.buttonMap, 0, 16 * 40 * sizeof(int));
		ProcS_Keyconfig(g);
	}
	if (((g->KeyInput.inputID[D_DIK_ESCAPE] == 1) || (g->KeyInput.mouse_buttonR == 1)) && (g->procPhase == 1)) {
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

//maybe deleted by compiler, restored it for convenience
int SetFirstSkin(SkinManage *sm, SKINTYPE type, CSTR *skinName) {

	sm->skinID[type] = -1;
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].skinFile.isSame(skinName) && sm->Data[i].type == type) {
			sm->skinID[type] = i;
			ErrorLogFmtAdd("SetFirstSkin 正しくスキン番号を設定しました。種別%d パス%s 番号%d\n", type, skinName->body, i);
			return 1;
		}
	}

	ErrorLogFmtAdd("SetFirstSkin()で該当のタイプのスキンが見つかりませんでした。種別%d パス%s\n", type, skinName->body);
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].type == type) {
			skinName->assign(&sm->Data[i].skinFile);
			sm->skinID[type] = i;
			return 1;
		}
	}
	return -1;
}

//409c80
int SetFirstSkin_5k(SkinManage *sm, SKINTYPE type, CSTR *skinName){
	
	sm->skinID[type] = -1;
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].skinFile.isSame(skinName) &&
			(sm->Data[i].type == type || (sm->Data[i].type == type - 1 && (type == SKINTYPE_5KEYS || type == SKINTYPE_10KEYS || type == SKINTYPE_7KEYSBATTLE)))) {
			sm->skinID[type] = i;
			ErrorLogFmtAdd("SetFirstSkin 正しくスキン番号を設定しました。種別%d パス%s 番号%d\n", type, skinName->body, i);
			return 1;
		}
	}
	ErrorLogFmtAdd("SetFirstSkin()で該当のタイプのスキンが見つかりませんでした。種別%d パス%s\n", type, skinName->body);
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].type == type || (sm->Data[i].type == type -1 && (type == SKINTYPE_5KEYS || type == SKINTYPE_10KEYS || type == SKINTYPE_7KEYSBATTLE) )) {
			skinName->assign(&sm->Data[i].skinFile);
			sm->skinID[type] = i;
			return 1;
		}
	}
	return -1;
}

//maybe deleted by compiler, restored it for convenience
int SetFirstSkin_10k(SkinManage *sm, SKINTYPE type, CSTR *skinName) {

	sm->skinID[type] = -1;
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].skinFile.isSame(skinName) && (sm->Data[i].type == SKINTYPE_10KEYS || sm->Data[i].type == SKINTYPE_14KEYS)) {
			sm->skinID[type] = i;
			ErrorLogFmtAdd("SetFirstSkin 正しくスキン番号を設定しました。種別%d パス%s 番号%d\n", type, skinName->body, i);
			return 1;
		}
	}
	ErrorLogFmtAdd("SetFirstSkin()で該当のタイプのスキンが見つかりませんでした。種別%d パス%s\n", type, skinName->body);
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].type == SKINTYPE_10KEYS || sm->Data[i].type == SKINTYPE_14KEYS) {
			skinName->assign(&sm->Data[i].skinFile);
			sm->skinID[type] = i;
			return 1;
		}
	}
	return -1;
}

//maybe deleted by compiler, restored it for convenience
int SetFirstSkin_5kb(SkinManage *sm, SKINTYPE type, CSTR *skinName) {

	sm->skinID[type] = -1;
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].skinFile.isSame(skinName) && (sm->Data[i].type == 13 || sm->Data[i].type == 12)) {
			sm->skinID[type] = i;
			ErrorLogFmtAdd("SetFirstSkin 正しくスキン番号を設定しました。種別%d パス%s 番号%d\n", type, skinName->body, i);
			return 1;
		}
	}
	ErrorLogFmtAdd("SetFirstSkin()で該当のタイプのスキンが見つかりませんでした。種別%d パス%s\n", type, skinName->body);
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].type == 13 || sm->Data[i].type == 12) {
			skinName->assign(&sm->Data[i].skinFile);
			sm->skinID[type] = i;
			return 1;
		}
	}
	return -1;
}

//409d80
int SetFirstSkins(game *g){
	SkinManage *sm;
	int iVar1;
	bool bVar2;
	SKINTYPE *pSVar3;
	int iVar4;
	CSTR *pCVar5;
	int iVar6;

	ErrorLogAdd("スキンを列挙します。\n");
	sm = &g->skinData;
	InitSkinData(sm);
	MakeSkinList(sm, CSTR("LR2files\\Theme\\"));
	MakeSkinList(sm, CSTR("LR2files\\Sound\\"));
	ErrorLogAdd("スキンの列挙を終了しました。\n");

	if (SetFirstSkin(sm, SKINTYPE_7KEYS, &g->config.skin.skinFilePath[0]) == -1) {
		ErrorLogAdd("7keysスキンが有りません。\n");
	}
	if (SetFirstSkin_5k(sm, SKINTYPE_5KEYS, &g->config.skin.skinFilePath[1]) == -1) {
		ErrorLogAdd("7keysスキンが有りません。\n"); //it has to be 5key but error??
	}
	if (SetFirstSkin(sm, SKINTYPE_14KEYS, &g->config.skin.skinFilePath[2]) == -1) {
		ErrorLogAdd("14keysスキンが有りません。\n");
	}
	if (SetFirstSkin_10k(sm, SKINTYPE_10KEYS, &g->config.skin.skinFilePath[3]) == -1) {
		ErrorLogAdd("14keysスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_9KEYS, &g->config.skin.skinFilePath[4]) == -1) {
		ErrorLogAdd("9keysスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_SELECT, &g->config.skin.skinFilePath[5]) == -1) {
		ErrorLogAdd("selectスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_DECIDE, &g->config.skin.skinFilePath[6]) == -1) {
		ErrorLogAdd("decideスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_RESULT, &g->config.skin.skinFilePath[7]) == -1) {
		ErrorLogAdd("resultスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_KEYCONFIG, &g->config.skin.skinFilePath[8]) == -1) {
		ErrorLogAdd("keyconfigスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_SKINSELECT, &g->config.skin.skinFilePath[9]) == -1) {
		ErrorLogAdd("skinselectスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_SOUNDSET, &g->config.skin.skinFilePath[10]) == -1) {
		ErrorLogAdd("soundsetが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_THEME, &g->config.skin.skinFilePath[11]) == -1) {
		ErrorLogAdd("themeが有りません。\n");
	}
	if (SetFirstSkin(sm, (SKINTYPE)12, &g->config.skin.skinFilePath[12]) == -1) { //QUESTION : 12 is really 5keys battle???
		ErrorLogAdd("7keysバトルスキンが有りません。\n");
	}
	if (SetFirstSkin_5kb(sm, (SKINTYPE)13, &g->config.skin.skinFilePath[13]) == -1) { //QUESTION : 13 is really 7keys battle???
		ErrorLogAdd("5keysバトルスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, (SKINTYPE)14, &g->config.skin.skinFilePath[14]) == -1) { //QUESTION : 14 is really course edit???
		ErrorLogAdd("9keysバトルスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_COURSERESULT, &g->config.skin.skinFilePath[15]) == -1) {
		ErrorLogAdd("コースリザルトスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, (SKINTYPE)16, &g->config.skin.skinFilePath[16]) == -1) { //QUESTION : 16 is really 9keys battle???
		ErrorLogAdd("オープニングスキンが有りません。\n"); //opening skin
	}
	if (SetFirstSkin(sm, (SKINTYPE)17, &g->config.skin.skinFilePath[17]) == -1) { //QUESTION : 17 is really opening?
		ErrorLogAdd("モードセレクトスキンが有りません。\n"); //mode select skin
	}
	if (SetFirstSkin(sm, (SKINTYPE)18, &g->config.skin.skinFilePath[18]) == -1) { //QUESTION : 18 is really mode select?
		ErrorLogAdd("モード決定スキンが有りません。\n"); //mode decide
	}
	if (SetFirstSkin(sm, (SKINTYPE)19, &g->config.skin.skinFilePath[19]) == -1) { //QUESTION : 19 is really mode decide?
		ErrorLogAdd("コースセレクトスキンが有りません。\n"); //course select
	}
	if (SetFirstSkin(sm, (SKINTYPE)20, &g->config.skin.skinFilePath[20]) == -1) { //QUESTION : 18 is really course select?
		ErrorLogAdd("コース編集スキンが有りません。\n"); //course edit
	}
	//15:course result was here, but I moved it on the correct order
	return 1;
}



//40b320
int ShowReadmes(game *g) {

	CSTR search;
	HANDLE hFindFile;
	WIN32_FIND_DATA FindFileData;
	CSTR fBuf(2048);

	FILE *pFile;

	g->txtStruct.readme.file_count = 0;
	g->txtStruct.readme.current = 0;
	g->txtStruct.readme.folderpath.fillzero();
	g->txtStruct.readme.path.fillzero();
	g->txtStruct.readme.lines = 0;
	g->txtStruct.readme.y = 0;
	g->txtStruct.readme.x = 0;
	g->txtStruct.readme.w = 0;
	g->txtStruct.readme.h = 0;
	g->txtStruct.readme.show = 0;
	for (int i = 0; i < 1000; i++) {
		g->txtStruct.readme.body[i].fillzero();
	}

	g->txtStruct.readme.folderpath = g->sSelect.bmsList[g->sSelect.cur_song].filepath.getDirectory();
	cstrSprintf(&search, "%s*.txt", g->txtStruct.readme.folderpath.body);
	hFindFile = FindFirstFileA(search, &FindFileData);
	if (hFindFile == (HANDLE)-1) {
		ErrorLogFmtAdd("テキストファイルが見つからない。%s\n", search);
		return -1;
	}

	do {
		g->txtStruct.readme.file_count++;
	} while (FindNextFileA(hFindFile,&FindFileData));
	FindClose(hFindFile);

	if (g->txtStruct.readme.file_count == 0) return 0;

	if (g->txtStruct.readme.current >= g->txtStruct.readme.file_count) 
		g->txtStruct.readme.current = 0;
	else if(g->txtStruct.readme.current < 0)
		g->txtStruct.readme.current = g->txtStruct.readme.file_count-1;

	int currentFileNum = 0;
	hFindFile = FindFirstFileA(search, &FindFileData);
	do {
		g->txtStruct.readme.path = g->txtStruct.readme.folderpath;
		g->txtStruct.readme.path.add(FindFileData.cFileName);

		fopen_s(&pFile, g->txtStruct.readme.path, "r");

		char *pFbuf = fBuf.outstr();
		if (pFile != NULL) {
			
			currentFileNum++;
			if (g->txtStruct.readme.file_count == 1) {
				g->txtStruct.readme.body[g->txtStruct.readme.lines] = FindFileData.cFileName;
			}
			else {
				cstrSprintf(&g->txtStruct.readme.body[g->txtStruct.readme.lines], "%d/%d %s", currentFileNum, g->txtStruct.readme.file_count, FindFileData.cFileName);
			}
			g->txtStruct.readme.lines+=2;

			g->txtStruct.readme.show = 1;
			pFbuf = fBuf.outstr();
			for (pFbuf = fgets(pFbuf, 2048, pFile); pFbuf; pFbuf = fgets(pFbuf, 2048, pFile)) {
				DealWhiteSpace(&fBuf);
				g->txtStruct.readme.body[g->txtStruct.readme.lines] = fBuf;
				g->txtStruct.readme.lines++;
				int len = GetTextGraphLength(&fBuf, &g->skstruct.ImageFonts[g->skstruct.src_README[0].cycle]);
				if (g->txtStruct.readme.x < len) g->txtStruct.readme.x = len;
				*fBuf.atPos(0) = '\0';
				if (g->txtStruct.readme.lines >= 900) break;
			}
			fclose(pFile);
			g->txtStruct.readme.lines += 2;
		}
	} while (FindNextFileA(hFindFile, &FindFileData));
	FindClose(hFindFile);

	g->txtStruct.readme.y = g->skstruct.src_README[0].op1 * g->txtStruct.readme.lines;
	return 1;
}

//40b720
int ShowReadme(game *g, CSTR path) {
	
	FILE *pFile;

	g->txtStruct.readme.file_count = 0;
	g->txtStruct.readme.current = 0;
	g->txtStruct.readme.folderpath.fillzero();
	g->txtStruct.readme.path.fillzero();
	g->txtStruct.readme.lines = 0;
	g->txtStruct.readme.y = 0;
	g->txtStruct.readme.x = 0;
	g->txtStruct.readme.w = 0;
	g->txtStruct.readme.h = 0;
	g->txtStruct.readme.show = 0;
	for (int i = 0; i < 1000; i++) {
		g->txtStruct.readme.body[i].fillzero();
	}
	
	g->txtStruct.readme.path = path;

	CSTR fBuf(0x400);

	fopen_s(&pFile, g->txtStruct.readme.path, "r");

	char *pFbuf = fBuf.outstr();
	if (pFile != NULL) {
		g->txtStruct.readme.show = 1;
		pFbuf = fBuf.outstr();
		for (pFbuf = fgets(pFbuf, 0x3FC, pFile); pFbuf; pFbuf = fgets(pFbuf, 0x3FC, pFile)) {
			g->txtStruct.readme.body[g->txtStruct.readme.lines] = fBuf;
			g->txtStruct.readme.lines++;
			int len = GetTextGraphLength(&fBuf, &g->skstruct.ImageFonts[g->skstruct.src_README[0].cycle]);
			if (g->txtStruct.readme.x < len) g->txtStruct.readme.x = len;
			*fBuf.atPos(0) = '\0';
		}
		fclose(pFile);
	}

	g->txtStruct.readme.x += g->skstruct.textmergin_1;
	g->txtStruct.readme.y = g->txtStruct.readme.lines * g->skstruct.src_README[0].op1 + g->skstruct.textmergin_2;
	return 1;
}

//40b910
CSTR GetMissonString(int missionLevel, int line) {
	switch (missionLevel) {
		case 1:
			if (line != 0) return "DEATH";
			break;

		case 2:
			if (line != 0) return "HIDDEN";
			break;

		case 3:
			if (line != 0) return "SUDDEN";
			break;

		case 4:
			if (line == 0) 
				return "GROOVE, GAUGE80%-84%";
			else
				return "";

		case 5:
			if (line != 0) return "S-RANDOM, SURVIVAL";
			break;

		case 6:
			if (line == 0)
				return "NOTES>=100, ";
			else
				return "FREQ/SPEEDFX>=+3";

		case 7:
			if (line != 0) return "CONVERGE";
			break;

		case 8:
			if (line != 0) return "HID+SUD";
			break;

		case 9:
			if (line == 0)
				return "RANK AAA";
			else
				return "";

		case 10:
			if (line == 0)
				return "RUNNING COMBO>=2000";
			else
				return "";

		case 11:
			if (line == 0)
				return "NOTES>=500";
			else
				return "DEATH";

		case 12:
			if (line == 0)
				return "NOTES>=500";
			else
				return "SURVIVAL, HIDDEN";

		case 13:
			if (line == 0)
				return "NOTES>=500";
			else
				return "SURVIVAL, SUDDEN";
		
		case 14:
			if (line == 0) 
				return "NOTES>=500";
			else
				return "CONVERGE";

		case 15:
			if (line == 0)
				return "NOTES>=500";
			else
				return "CONSTANT, SPEED=50";

		case 16:
			if (line == 0) 
				return "NOTES>=500";
			else
				return "RANDOM, HID+SUD";

		case 17:
			if (line == 0)
				return "MAXCOMBO=333";
			else
				return "";

		case 18:
			if (line != 0) return "P.ATTACK";
			break;

		case 19:
			if (line != 0) return "G.ATTACK";
			break;

		case 20:
			if (line == 0) 
				return "GROOVE, GAUGE80%";
			else
				return "";

		case 21:
			if (line == 0) 
				return "SURVIVAL, GAUGE2%";
			else
				return "";

		case 22:
			if (line == 0)
				return "NOTES>=1000"; 
			else
				return "DEATH, S-RANDOM";

		case 23:
			if (line == 0)
				return "NOTES>=1000"; 
			else
				return "HIDDEN, CONSTANT, HISPEED=150";

		case 24:
			if (line == 0)
				return "NOTES>=1000"; 
			else
				return "SUDDEN, CONSTANT, SPEED=250";

		case 25:
			if (line == 0) 
				return "NOTES>=1000";
			else
				return "CONVERGE";

		case 26:
			if (line == 0) 
				return "NOTES>=1000";
			else
				return "RANDOM, HID+SUD";

		case 27:
			if (line == 0) 
				return "NOTES>=1000"; 
			else
				return "FREQ/SPEEDFX>=+6";

		case 28:
			if (line == 0) 
				return "NOTES>=300"; 
			else
				return "CONSTANT, SPEED=600";

		case 29:
			if (line == 0)
				return "NOTES>=1000"; 
			else
				return "RANK AAA+-0";

		case 30:
			if (line == 0)
				return "(^^)or(^^)BGA / Yamajet"; 
			else
				return "SCORE=100000";

		case 31:
			if (line == 0)
				return "NOTES>=1000, TIME<2:30"; 
			else
				return "P.ATTACK";

		case 32:
			if (line == 0)
				return "NOTES>=1000, TIME<2:30";
			else
				return "G.ATTACK";

		case 33:
			if (line == 0)
				return "NOTES>=1000,TIME<2:30";
			else
				return "MAXCOMBO<40"; 

		case 34:
			if (line == 0) 
				return "NOTES>=1200,TIME<2:30"; 
			else
				return "CONSTANT, SPEED=30";

		case 35:
			if (line == 0)
				return "NOTES>=1200, TIME<2:30";
			else
				return "SURVIVAL, CONVERGE"; 

		case 36:
			if (line == 0) 
				return "DEATH, EXSCORE:998-1002";
			else
				return "";

		case 37:
			if (line == 0)
				return "NOTES>=1000, TIME<2:30"; 
			else
				return "FREQ/SPEEDFX=+12";

		case 38:
			if (line == 0)
				return "NOTES>=1500, TIME<2:30";
			else
				return "DEATH";

		case 39:
			if (line == 0) 
				return "NOTES>=1500,TIME<2:30"; 
			else
				return "CONSTANT, HISPEED=100, HIDDEN";

		case 40:
			if (line == 0)
				return "[FINAL]NOTES>=1500, TIME<2:30";
			else
				return "SURVIVAL, KEY CONVERGE";

		default:
			if (line == 0) 
				return "MISSION COMPLETED";
			else 
				return "";
	}
	return "NOTES>=100";
}


////LUNARIS
int lun_graph[8];
int lun_timer;
int lun_judgetime;
int lun_nowblock;
int lun_field[8][16];
int lun_cleartimer[8][16];
int lun_nextblock;
int lun_nowblockshape[5][5];
int lun_nextblockshape[5][5];
int lun_x;
int lun_y;


//40bf90
int LUNARIS_GETBLOCKUP(void) {
	int ret = 3;

	for (int i = 1; i >= 0; i--) {
		for (int j = 0; j < 5; j++) {
			if (lun_nowblockshape[j][i]) {
				ret--;
				break;
			}
		}
	}
	return ret;
}

//40bfd0
int LUNARIS_GETBLOCKDOWN(void) {
	int ret = 3;

	for (int i = 3; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (lun_nowblockshape[j][i]) {
				ret++;
				break;
			}
		}
	}
	return ret;
}

//40c010
int LUNARIS_GETBLOCKLEFT(void) {
	int ret = 3;

	for (int i = 1; i >= 0; i--) {
		for (int j = 0; j < 5; j++) {
			if (lun_nowblockshape[i][j]) {
				ret--;
				break;
			}
		}
	}
	return ret;
}

//40c050
int LUNARIS_GETBLOCKRIGHT(void) {
	int ret = 3;

	for (int i = 3; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (lun_nowblockshape[i][j]) {
				ret++;
				break;
			}
		}
	}
	return ret;
}

//40c090
void LUNARIS_LAND() {
	int x, y;

	x = lun_x;
	y = lun_y;
	
	for (int i = 0; i < 5; i++) {
		if (lun_nowblockshape[0][i]) {
			lun_field[x + 0][y + i] = lun_nowblockshape[0][i];
		}
		if (lun_nowblockshape[1][i]) {
			lun_field[x + 1][y + i] = lun_nowblockshape[1][i];
		}
		if (lun_nowblockshape[2][i]) {
			lun_field[x + 2][y + i] = lun_nowblockshape[2][i];
		}
		if (lun_nowblockshape[3][i]) {
			lun_field[x + 3][y + i] = lun_nowblockshape[3][i];
		}
		if (lun_nowblockshape[4][i]) {
			lun_field[x + 4][y + i] = lun_nowblockshape[4][i];
		}
	}
	return;
}

//40c140 //2left 4right 8down
int LUNARIS_MOVE(int direction) { 

	//LEFT
	int col = 0;
	if (direction == 2) {
		if (lun_x > 1 - LUNARIS_GETBLOCKLEFT()) {
			for (int i = 0; i < 5; i++) {
				if (lun_nowblockshape[0][i] && lun_field[lun_x - 1][lun_y + i]) col++;
				if (lun_nowblockshape[1][i] && lun_field[lun_x + 0][lun_y + i]) col++;
				if (lun_nowblockshape[2][i] && lun_field[lun_x + 1][lun_y + i]) col++;
				if (lun_nowblockshape[3][i] && lun_field[lun_x + 2][lun_y + i]) col++;
				if (lun_nowblockshape[4][i] && lun_field[lun_x + 3][lun_y + i]) col++;
			}
			if (col == 0) {
				lun_x--;
				return 1;
			}
		}
	}
	else if (direction == 4) {
		if (lun_x < 8 - LUNARIS_GETBLOCKRIGHT()) {
			for (int i = 0; i < 5; i++) {
				if (lun_nowblockshape[0][i] && lun_field[lun_x + 1][lun_y + i]) col++;
				if (lun_nowblockshape[1][i] && lun_field[lun_x + 2][lun_y + i]) col++;
				if (lun_nowblockshape[2][i] && lun_field[lun_x + 3][lun_y + i]) col++;
				if (lun_nowblockshape[3][i] && lun_field[lun_x + 4][lun_y + i]) col++;
				if (lun_nowblockshape[4][i] && lun_field[lun_x + 5][lun_y + i]) col++;
			}
			if (col == 0) {
				lun_x++;
				return 1;
			}
		}
	}
	else if (direction == 8) {
		if (lun_y < 16 - LUNARIS_GETBLOCKDOWN()) {
			for (int i = 0; i < 5; i++) {
				if (lun_nowblockshape[0][i] && lun_field[lun_x + 0][lun_y + i +1]) col++;
				if (lun_nowblockshape[1][i] && lun_field[lun_x + 1][lun_y + i +1]) col++;
				if (lun_nowblockshape[2][i] && lun_field[lun_x + 2][lun_y + i +1]) col++;
				if (lun_nowblockshape[3][i] && lun_field[lun_x + 3][lun_y + i +1]) col++;
				if (lun_nowblockshape[4][i] && lun_field[lun_x + 4][lun_y + i +1]) col++;
			}
			if (col == 0) {
				lun_y++;
				return 1;
			}
		}
	}
	else return 1;

	return 0;
}

//40c400
int LUNARIS_ROTATE(char direction) {

	int newshape[5][5] = { {0,}, };//DEBUG : there are no init in original code
	
	if (lun_nowblock == 5) return 0;

	if (direction == 0) {
		for (int j = 0; j < 5; j++) {
			newshape[4 - j][0] = lun_nowblockshape[0][j];
			newshape[4 - j][1] = lun_nowblockshape[1][j];
			newshape[4 - j][2] = lun_nowblockshape[2][j];
			newshape[4 - j][3] = lun_nowblockshape[3][j];
			newshape[4 - j][4] = lun_nowblockshape[4][j];
		}
	}
	else {
		for (int i = 0; i < 3; i++) { //TOFIX: 1~3 rotate is same
			for (int j = 0; j < 5; j++) {
				newshape[4 - j][0] = lun_nowblockshape[0][j];
				newshape[4 - j][1] = lun_nowblockshape[1][j];
				newshape[4 - j][2] = lun_nowblockshape[2][j];
				newshape[4 - j][3] = lun_nowblockshape[3][j];
				newshape[4 - j][4] = lun_nowblockshape[4][j];
			}
		}
	}
	
	int checkcount = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (newshape[j][i] && (lun_x + j > 7 || lun_field[lun_x + j][lun_y + i])) return 0;
		}
	}
	memcpy(lun_nowblockshape, newshape, sizeof(lun_nowblockshape));
	return 1;
}

//40c510
int LUNARIS_NEXTRANDOMROTATE(void) {
	
	int newshape[5][5];
	
	if(lun_nextblock == 5) return 0;

	int count = GetRand(3);
	if (count == 0) return 1;

	for (int i = 0; i < count; i++) { //TOFIX: 1~3 rotate is same
		for (int j = 0; j < 5; j++) {
			newshape[4-j][0] = lun_nextblockshape[0][j];
			newshape[4-j][1] = lun_nextblockshape[1][j];
			newshape[4-j][2] = lun_nextblockshape[2][j];
			newshape[4-j][3] = lun_nextblockshape[3][j];
			newshape[4-j][4] = lun_nextblockshape[4][j];
		}
	}
	memcpy(lun_nextblockshape, newshape, sizeof(lun_nextblockshape));
	return 1;
}

//40c5a0
int LUNARIS_CHECKLINE(void) {

	int ret = 0;

	for (int i = 15; i >= 0;) {
		int blocksInThisLine = 0;
		for (int j = 0; j < 8; j++) {
			if (lun_field[j][i]) blocksInThisLine++;
		}

		if (blocksInThisLine == 0) break;

		if (blocksInThisLine == 8) {
			ret++;

			for (int j = 0; j < 8; j++) {
				lun_cleartimer[j][i] = GetTimeWrap();
			}
			
			for (int j = i; j >= 0; j--) {
				for (int k = 0; k < 8; k++) {
					if (j <= 0) lun_field[k][0] = 0;
					else lun_field[k][j] = lun_field[k][j-1];
				}
			}
		}
		else i--;
	}
	return ret;
}

//40c6a0
void LUNARIS_NEXT(void) {
	int thisblock;

	memcpy(lun_nowblockshape, lun_nextblockshape, sizeof(lun_nowblockshape));
	memset(lun_nextblockshape, 0, sizeof(lun_nextblockshape));

	thisblock = GetRand(6);
	switch (thisblock) {
		case 0:
			lun_nextblockshape[2][1] = 1;
			lun_nextblockshape[2][2] = 1;
			lun_nextblockshape[2][3] = 1;
			lun_nextblockshape[2][4] = 1;
			break;
		case 1:
			lun_nextblockshape[2][1] = 1;
			lun_nextblockshape[2][2] = 1;
			lun_nextblockshape[2][3] = 1;
			lun_nextblockshape[3][3] = 1;
			break;
		case 2:
			lun_nextblockshape[2][1] = 1;
			lun_nextblockshape[2][2] = 1;
			lun_nextblockshape[2][3] = 1;
			lun_nextblockshape[1][3] = 1;
			break;
		case 3:
			lun_nextblockshape[3][2] = 1;
			lun_nextblockshape[2][2] = 1;
			lun_nextblockshape[2][3] = 1;
			lun_nextblockshape[1][3] = 1;
			break;
		case 4:
			lun_nextblockshape[1][2] = 1;
			lun_nextblockshape[2][2] = 1;
			lun_nextblockshape[2][3] = 1;
			lun_nextblockshape[3][3] = 1;
			break;
		case 5:
			lun_nextblockshape[1][2] = 1;
			lun_nextblockshape[2][2] = 1;
			lun_nextblockshape[2][3] = 1;
			lun_nextblockshape[1][3] = 1;
			break;
		case 6:
			lun_nextblockshape[1][3] = 1;
			lun_nextblockshape[2][2] = 1;
			lun_nextblockshape[2][3] = 1;
			lun_nextblockshape[3][3] = 1;
			break;
	}

	lun_nowblock = lun_nextblock;
	lun_nextblock = thisblock;
	LUNARIS_NEXTRANDOMROTATE();
	lun_x = 1;
	lun_y = -2;
	return;
}

//40c790
int DrawLunaris(game *g) {

	DSTdraw dstd;
	DSTdraw dstd2;

	for (int i = 0; i < 10; i++) {
		dstd = SetDSTdrawByTime(g->skstruct.dst_NOTE[i], GetTimeLapse(g->skstruct.dst_NOTE[i].timer, &g->timer1));

		g->skstruct.drBuf.isHidSud[i] = g->config.play.m_HIDSUD1;
		if (g->config.play.m_HIDSUD1 == 1) {
			g->skstruct.drBuf.top[i] = 0;
			g->skstruct.drBuf.bottom[i] = dstd.sortID * 0.6;
		}
		else if (g->config.play.m_HIDSUD1 == 2) {
			g->skstruct.drBuf.top[i] = dstd.sortID * 0.6;
			g->skstruct.drBuf.bottom[i] = 480;
		}
		else if (g->config.play.m_HIDSUD1 == 3) {
			g->skstruct.drBuf.top[i] = dstd.sortID * 0.55;
			g->skstruct.drBuf.bottom[i] = dstd.sortID * 0.65;
		}
		else {
			g->skstruct.drBuf.top[i] = 0;
			g->skstruct.drBuf.bottom[i] = 480;
		}
	}

	for (int i = 10; i < 20; i++) {
		dstd = SetDSTdrawByTime(g->skstruct.dst_NOTE[i], GetTimeLapse(g->skstruct.dst_NOTE[i].timer, &g->timer1));

		g->skstruct.drBuf.isHidSud[i] = g->config.play.m_HIDSUD2;
		if (g->config.play.m_HIDSUD2 == 1) {
			g->skstruct.drBuf.top[i] = 0;
			g->skstruct.drBuf.bottom[i] = (100 - g->config.play.p1_lanecoverv) * dstd.sortID / 200 + g->config.play.p1_lanecoverv * dstd.sortID / 100;
		}
		else if (g->config.play.m_HIDSUD2 == 2) {
			g->skstruct.drBuf.top[i] = (100 - g->config.play.p1_lanecoverv) * dstd.sortID / 200 + g->config.play.p1_lanecoverv * dstd.sortID / 100;
			g->skstruct.drBuf.bottom[i] = 480;
		}
		else if (g->config.play.m_HIDSUD2 == 3) {
			g->skstruct.drBuf.top[i] = (100 - g->config.play.p1_lanecoverv) * dstd.sortID / 100 * 4 / 10 + g->config.play.p1_lanecoverv * dstd.sortID / 100;
			g->skstruct.drBuf.bottom[i] = (100 - g->config.play.p1_lanecoverv) * dstd.sortID / 100 * 6 / 10 + g->config.play.p1_lanecoverv * dstd.sortID / 100;
		}
		else {
			g->skstruct.drBuf.top[i] = 0;
			g->skstruct.drBuf.bottom[i] = 480;
		}
	}

	g->skstruct.dst_NOTE[0].draw->y * 0.0625;
	int yy = (int)(g->skstruct.dst_NOTE[0].draw->y * 0.0625) * 16 - g->skstruct.dst_NOTE[0].draw->h;
	int oldyy = yy;
	int hh = g->skstruct.dst_NOTE[0].draw->y * 0.0625;

	int lane;

	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 8; x++) {
			lane = x;
			if (g->skstruct.scratchside_1 == 1) {
				switch (x) {
				case 0:
					lane = 1;
					break;
				case 1:
					lane = 2;
					break;
				case 2:
					lane = 3;
					break;
				case 3:
					lane = 4;
					break;
				case 4:
					lane = 5;
					break;
				case 5:
					lane = 6;
					break;
				case 6:
					lane = 7;
					break;
				case 7:
					lane = 0;
					break;
				}
			}

			if (lun_field[x][y]) {
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_NOTE[lane], GetTimeLapse(g->skstruct.dst_NOTE[lane].timer, &g->timer1));
				dstd2.y -= yy;
				dstd2.h = hh - 1;
				AddDrawingBuffer_Lunaris(&g->skstruct.drBuf, &g->skstruct.src_NOTE[lane], &dstd2, &g->timer1);
			}
			else if (0 <= x - lun_x && x - lun_x <= 4 && 0 <= y - lun_y && y - lun_y <= 4 && lun_nowblockshape[x - lun_x][y - lun_y]) {
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_NOTE[lane], GetTimeLapse(g->skstruct.dst_NOTE[lane].timer, &g->timer1));
				dstd2.y -= yy;
				dstd2.h = hh - 1;
				AddDrawingBuffer_Lunaris(&g->skstruct.drBuf, &g->skstruct.src_NOTE[lane], &dstd2, &g->timer1);
			}

			if (lun_cleartimer[x][y] > 0 && lun_graph[lane] >= 0) {
				dstd2 = SetDSTdrawByTime(g->skstruct.image.dst[lun_graph[lane]], GetTimeWrap() - lun_cleartimer[x][y]);
				dstd2.y -= yy;
				int cycle = GetSRCcycleNow(g->skstruct.image.src[lun_graph[lane]], GetTimeWrap() - lun_cleartimer[x][y]);
				AddDrawingBuffer(&g->skstruct.drBuf, g->skstruct.image.src[lun_graph[lane]].grHandles[cycle], &dstd2);
			}
		}
		yy -= hh;
	}

	yy = oldyy;
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			lane = x;
			if (g->skstruct.scratchside_1 == 0) {
				switch (x) {
				case 0:
					lane = 1;
					break;
				case 1:
					lane = 2;
					break;
				case 2:
					lane = 3;
					break;
				case 3:
					lane = 4;
					break;
				case 4:
					lane = 5;
					break;
				}
			}
			else {
				switch (x) {
				case 0:
					lane = 3;
					break;
				case 1:
					lane = 4;
					break;
				case 2:
					lane = 5;
					break;
				case 3:
					lane = 6;
					break;
				case 4:
					lane = 7;
					break;
				}
			}

			if (lun_nextblockshape[x][y]) {
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_NOTE[lane], GetTimeLapse(g->skstruct.dst_NOTE[lane].timer, &g->timer1));
				dstd2.h = hh - 1;
				dstd2.y -= yy;
				if (g->skstruct.scratchside_1 == 1) {
					dstd2.x -= g->skstruct.dst_NOTE[1].draw->w * 5.0 + g->skstruct.dst_NOTE[2].draw->w * 4.0;
				}
				else {
					dstd2.x += g->skstruct.dst_NOTE[1].draw->w * 5.0 + g->skstruct.dst_NOTE[2].draw->w * 4.0;
				}
				AddDrawingBuffer_Lunaris(&g->skstruct.drBuf, &g->skstruct.src_NOTE[lane], &dstd2, &g->timer1);
			}
		}
		yy -= hh;
	}

	return 1;
}

//40cff0
int LUNARIS_START(game *g) {

	memset(lun_field, 0, sizeof(lun_field));
	memset(lun_cleartimer, -1, sizeof(lun_cleartimer));
	
	lun_nowblock = 0;
	lun_nextblock = 0;

	LUNARIS_NEXT();
	LUNARIS_NEXT();

	g->gameplay.isNosave = 1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < g->skstruct.image.srcSize; j++) {
			if (g->skstruct.image.src[j].timer == 50 + i) {
				lun_graph[i] = j;
				break;
			}
		}
	}
	return 1;
}

//40d090
int LUNARIS_JUDGE(game *g) {

	if (LUNARIS_MOVE(8) == 0) {
		LUNARIS_LAND();
		int lineclear = LUNARIS_CHECKLINE();
		int up = LUNARIS_GETBLOCKUP();

		g->gameplay.player[0].judgecount[1]++;

		if (g->config.play.gaugeOption[0] == 0) {
			g->gameplay.player[0].HP -= 1.5;
			g->gameplay.player[0].HP_print -= 1.5;
			g->gameplay.player[0].HP_old -= 1.5;
			if (g->gameplay.player[0].HP < 2.0) {
				g->gameplay.player[0].HP = 2.0;
				g->gameplay.player[0].HP_print = 2.0;
				g->gameplay.player[0].HP_old = 2.0;
			}
		}
		else if (g->config.play.gaugeOption[0] == 3) {
			g->gameplay.player[0].HP -= 1.0;
			g->gameplay.player[0].HP_print -= 1.0;
			g->gameplay.player[0].HP_old -= 1.0;
			if (g->gameplay.player[0].HP < 2.0) {
				g->gameplay.player[0].HP = 2.0;
				g->gameplay.player[0].HP_print = 2.0;
				g->gameplay.player[0].HP_old = 2.0;
			}
		}
		else {
			g->gameplay.player[0].HP -= 5.0;
			g->gameplay.player[0].HP_print -= 5.0;
			g->gameplay.player[0].HP_old -= 5.0;
		}

		if (lun_field[7][1] || lun_y + up - 1 + lineclear < 0 || g->gameplay.player[0].HP < 0.0) {
			SetTimeLapse(3, &g->timer1);
			g->procPhase = 3;
			StopAllKeysound(g);
			PlaySound(&g->audio, &g->audio.sysSound.stop, g->audio.chnBgm, -1);
		}

		LUNARIS_NEXT();

		if (lineclear == 1) {
			g->gameplay.player[0].combo_song_draw++;
			g->gameplay.player[0].now_combo++;
			g->gameplay.player[0].judgecount[2]++;
			g->gameplay.player[0].judge_draw = 2;
			g->gameplay.player[0].recent_judge = 2;
			SetTimeLapse(46, &g->timer1);

			g->gameplay.player[0].HP += 4.0;
			if (g->config.play.gaugeOption[0] == 2 || g->config.play.gaugeOption[0] == 4) {
				g->gameplay.player[0].HP = 0.0;

				SetTimeLapse(3, &g->timer1);
				g->procPhase = 3;
				StopAllKeysound(g);
				PlaySound(&g->audio, &g->audio.sysSound.stop, g->audio.chnBgm, -1);
			}
			g->gameplay.player[0].score += 400;
		}
		else if (lineclear == 2) {
			g->gameplay.player[0].combo_song_draw += 2;
			g->gameplay.player[0].now_combo += 2;
			g->gameplay.player[0].exscore += 1;
			g->gameplay.player[0].judgecount[3]++;
			g->gameplay.player[0].judge_draw = 3;
			g->gameplay.player[0].recent_judge = 3;
			SetTimeLapse(46, &g->timer1);

			g->gameplay.player[0].HP += 16.0;
			if (g->config.play.gaugeOption[0] == 4) {
				g->gameplay.player[0].HP = 0.0;

				SetTimeLapse(3, &g->timer1);
				g->procPhase = 3;
				StopAllKeysound(g);
				PlaySound(&g->audio, &g->audio.sysSound.stop, g->audio.chnBgm, -1);
			}
			g->gameplay.player[0].score += 1000;
		}
		else if (lineclear == 3) {
			g->gameplay.player[0].combo_song_draw += 3;
			g->gameplay.player[0].now_combo += 3;
			g->gameplay.player[0].exscore += 2;
			g->gameplay.player[0].judgecount[4]++;
			g->gameplay.player[0].judge_draw = 4;
			g->gameplay.player[0].recent_judge = 4;
			SetTimeLapse(46, &g->timer1);

			g->gameplay.player[0].HP += 30.0;
			g->gameplay.player[0].score += 3000;
		}
		else if (lineclear == 4) {
			g->gameplay.player[0].combo_song_draw += 4;
			g->gameplay.player[0].now_combo += 4;
			g->gameplay.player[0].exscore += 2;
			g->gameplay.player[0].judgecount[5]++;
			g->gameplay.player[0].judge_draw = 5;
			g->gameplay.player[0].recent_judge = 5;
			SetTimeLapse(46, &g->timer1);

			g->gameplay.player[0].HP += 40.0;
			g->gameplay.player[0].score += 12000;
		}

		if (g->gameplay.player[0].HP > 100.0) g->gameplay.player[0].HP = 100.0;
	}
	lun_judgetime = GetTimeWrap();
	return 1;
}

//40d400
int ProcI_PO4Decide(game *g) {

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.scenetime && g->procPhase == 1) {
		g->procSelecter = 4;
		return 1;
	}

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start && (g->KeyInput.inputID[D_DIK_RETURN] == 1 || g->KeyInput.mouse_buttonL == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[7] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[6] == 1) && g->procPhase == 1) {
		SetTimeLapse(3, &g->timer1);
		g->procPhase = 3;
		return 1;
	}

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start && (g->KeyInput.mouse_buttonR == 1 || g->KeyInput.inputID[D_DIK_ESCAPE] == 1 || (g->KeyInput.p1_buttonInput[12] == 2 && g->KeyInput.p1_buttonInput[13] == 2) || (g->KeyInput.p2_buttonInput[12] == 2 && g->KeyInput.p2_buttonInput[13] == 2)) && g->procPhase == 1) {
		SetTimeLapse(2, &g->timer1);
		g->procPhase = 2;
		StopSysSound(g);
		return 1;
	}

	if (g->procPhase == 2) {
		if (GetTimeLapse(2, &g->timer1) > g->skstruct.fadeout || g->skstruct.fadeout == 0) {
			g->procSelecter = 9;
			return 1;
		}
	}
	else if (g->procPhase == 3) {
		if (GetTimeLapse(3, &g->timer1) > g->skstruct.close || g->skstruct.close == 0) {
			g->procSelecter = 2;
			return 1;
		}
	}
	return 1;
}

//40d600
void ThreadProc_PO4parseBMS(game *g) {
	
	g->gameplay.bmsResourceLoaded = 0;
	g->gameplay.flag_closingPhase = 0;
	g->gameplay.is2Pplay = 0;
	g->gameplay.courseStageCount = 0;
	InitGameplay(&g->gameplay, &g->config.play);
	
	ParseBmsFile(&g->gameplay, g->sSelect.metaSelected.filepath, &g->audio, &g->config, &g->sSelect.metaSelected, g->skstruct.flag_BGA, 0);
	g->po4_hThread_ParseBMS = 0;
	g->gameplay.bmsResourceLoaded = 1;
	return;
}

//40d680
int RunMP3Encoder(ConfigStruct *cfg, CSTR wavPath, CSTR mp3Path, char deleteWav, char movie) {

	if (cfg->tools.mp3enc_body.canOpenFile() == 0) {
		MessageBoxA(NULL, "MP3エンコーダーが見つかりません。\nコンフィグプログラムのJUKEBOX→詳細設定で設定して下さい。", "エラー", 0);
		return 0;
	}

	if (wavPath.canOpenFile() == 0) {
		MessageBoxA(NULL, "入力ファイルを開けません。", "エラー", 0);
		return 0;
	}

	CSTR cmd;
	if (movie) {
		cstrSprintf(&cmd, "%s %s \"%s\" \"%s\"", cfg->tools.mp3enc_body, cfg->tools.mp3enc_option_movie, wavPath, mp3Path);
	}
	else {
		cstrSprintf(&cmd, "%s %s \"%s\" \"%s\"", cfg->tools.mp3enc_body, cfg->tools.mp3enc_option_normal, wavPath, mp3Path);
	}

	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;
	
	sinfo.cbReserved2 = 0;
	sinfo.cb = 68;
	sinfo.lpReserved = 0;
	sinfo.lpDesktop = 0;
	sinfo.lpTitle = 0;
	sinfo.dwFlags = 0;
	sinfo.lpReserved2 = 0;
	CreateProcessA(NULL, cmd, 0, 0, 0, 0, 0, 0, &sinfo, &pinfo);
	WaitForSingleObject(pinfo.hProcess, -1);
	if (deleteWav) {
		remove(wavPath);
	}
	return 1;
}

//40d840
int Proc_Auto2avi(game *g, CSTR directory, CSTR filename) {

	printfDx("BMSを読み込み中です。しばらくお待ち下さい。");
	ScreenFlip();
	ClsDrawScreen();
	clsDx();
	ProcessMessage();
	InitGameplay(&g->gameplay, &g->config.play);


	g->config.system.isablebmsthread = 1;
	ParseBmsFile(&g->gameplay, filename, &g->audio, &g->config, &g->sSelect.metaSelected, 0, 0);

	if (g->gameplay.flag_longsound || g->gameplay.flag_0note) {
		MessageBoxA(NULL, "規約違反のbmsの可能性があるので、この機能は利用できません。", "エラー", 0);
		return 0;
	}
	printfDx("処理中です。しばらくお待ち下さい。");
	ScreenFlip();
	ClsDrawScreen();
	clsDx();
	ProcessMessage();
	GetSoundBuffer(&g->audio, g->gameplay.song_runtime, g->config.tools.mp3_volume);


	for (int i = 0; i < g->gameplay.bmsobj.size; i++) {
		double len = 0;
		g->gameplay.bmsobj.notes[i].op;
		if (g->gameplay.bmsobj.notes[i].op == 1 || (10 <= g->gameplay.bmsobj.notes[i].op  && g->gameplay.bmsobj.notes[i].op < 30)) {
			if (i + 1 < g->gameplay.bmsobj.size) {
				double endtime = g->gameplay.keysound[(int)g->gameplay.bmsobj.notes[i].val].length;
				if ((int)endtime < 0) {
					endtime += 4294967296.0;
				}
				endtime += g->gameplay.bmsobj.notes[i].realTiming;

				for (int j = i + 1; j < g->gameplay.bmsobj.size; j++) {
					if (endtime <= g->gameplay.bmsobj.notes[j].realTiming) break;
					
					if (g->gameplay.bmsobj.notes[j].op == 1 || (10 <= g->gameplay.bmsobj.notes[i].op  && g->gameplay.bmsobj.notes[i].op < 30)) {

						if ((int)g->gameplay.bmsobj.notes[i].val == (int)g->gameplay.bmsobj.notes[j].val) {
							len = g->gameplay.bmsobj.notes[j].realTiming - g->gameplay.bmsobj.notes[i].realTiming;
							if (len == 0) len = -1.0;
							break;
						}
					}
				}
			}
			RecordSound(&g->audio, &g->gameplay.keysound[(int)g->gameplay.bmsobj.notes[i].val], g->gameplay.bmsobj.notes[i].realTiming, len);
		}
	}

	CSTR ext = filename.right(3).lower();
	if(ext.isSame("mp3")) {
		CSTR tPath = g->baseDirectory;
		tPath.add("LR2files\\temp.wav");
		WriteSoundFile(&g->audio, tPath, 0);
		RunMP3Encoder(&g->config, tPath, filename, 1, 0);
	}
	else if (ext.isSame("wav")) {
		WriteSoundFile(&g->audio, filename, 0);
	}

	return 1;
}

//40dc30
int RecordBmsSound(game *g, CSTR oPath) {

	int startTime = g->rec.GetCurTime();
	ErrorLogFmtAdd("音声の記録を開始します　曲開始時間+%dです。\n", startTime);
	printfDx("処理中です。しばらくお待ち下さい。");
	ScreenFlip();
	ClsDrawScreen();
	clsDx();
	ProcessMessage();
	if (g->rec.recMode != 2) {
		g->gameplay.song_runtime += startTime;
		g->config.tools.mp3_volume;
		GetSoundBuffer(&g->audio, g->gameplay.song_runtime, g->config.tools.mp3_volume);

		for (int i = 0; i < g->gameplay.bmsobj.size; i++) {
			double len = 0;
			if (g->gameplay.bmsobj.notes[i].op == 1 || (10 <= g->gameplay.bmsobj.notes[i].op && g->gameplay.bmsobj.notes[i].op < 30)) {
				if (i + 1 < g->gameplay.bmsobj.size) {
					double endtime = g->gameplay.keysound[(int)g->gameplay.bmsobj.notes[i].val].length;
					if ((int)endtime < 0) {
						endtime += 4294967296.0;
					}
					endtime += g->gameplay.bmsobj.notes[i].realTiming;

					for (int j = i + 1; j < g->gameplay.bmsobj.size; j++) {
						if (endtime <= g->gameplay.bmsobj.notes[j].realTiming) break;

						if (g->gameplay.bmsobj.notes[j].op == 1 || (10 <= g->gameplay.bmsobj.notes[i].op  && g->gameplay.bmsobj.notes[i].op < 30)) {

							if ((int)g->gameplay.bmsobj.notes[i].val == (int)g->gameplay.bmsobj.notes[j].val) {
								len = g->gameplay.bmsobj.notes[j].realTiming - g->gameplay.bmsobj.notes[i].realTiming;
								if (len == 0) len = -1.0;
								break;
							}
						}
					}
				}
				RecordSound(&g->audio, &g->gameplay.keysound[(int)g->gameplay.bmsobj.notes[i].val], g->gameplay.bmsobj.notes[i].realTiming, len);
			}
		}
	}

	CSTR wavPath;
	if (g->config.tools.movie_audio == 0) {
		wavPath = g->baseDirectory;
		wavPath.add("LR2files\\movie_temp.wav");
		WriteSoundFile(&g->audio, wavPath, GetTimeWrap()*44100.0 / 1000.0 * 2);

		CSTR mp3Path(g->baseDirectory);
		mp3Path.add("LR2files\\movie_temp.mp3");
		if (RunMP3Encoder(&g->config, wavPath, mp3Path, 1, 1) == 1) {
			g->rec.InsertAudioToMovie(mp3Path, true);
		}
		else {
			g->rec.InsertAudioToMovie(wavPath, true);
		}
	}
	else if (g->config.tools.movie_audio == 1) {
		wavPath = g->baseDirectory;
		wavPath.add("LR2files\\movie_temp.wav");
		WriteSoundFile(&g->audio, wavPath, GetTimeWrap()*44100.0 / 1000.0 * 2);
		g->rec.InsertAudioToMovie(wavPath, true);
	}
	else {
		CSTR wavPath = oPath;
		wavPath = oPath.left(oPath.length() - 3);
		wavPath.add("wav");
		WriteSoundFile(&g->audio, wavPath, GetTimeWrap()*44100.0 / 1000.0 * 2);
	}

	g->rec.Release();
	return 1;
}


//40e790 // _mRet __mRet : return ret, ret : return !ret
bool GetOptionFlag_dst(game *gs, int option) {
	int t = 0;
	bool ret = (option >= 0);
	if (!ret) option = -option;


	switch (option) {
		case 0:
			return true;
		case 1:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 1) return ret;
			if (t == 2) return ret;
			if (t == 3) return ret;
			if (t == 4) return ret;
			if (t == 6) return ret;
			return !ret;
			break;

		case 2:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 0) return ret;
			if (t == 5) return ret;
			break;

		case 3:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 8) return ret;
			break;

		case 4:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 7) return ret;
			if (t == 9) return ret;
			break;

		case 5:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 0) return ret;
			if (t == 5) return ret;
			if (t == 8) return ret;
			if (t == 9) return ret;
			break;

		case 10:
			t = gs->config.select.key;
			if (t == 4) return ret;
			if (t == 5) return ret;
			if (t == 6) return ret;
			t = gs->config.play.battle;
			if (t == 2) return ret;
			if (t == 3) return ret;
			break;

		case 11:
			t = gs->config.play.battle;
			if (t == 1) return ret;
			break;

		case 12:
			t = gs->config.select.key;
			if (t == 4) return ret;
			if (t == 5) return ret;
			if (t == 6) return ret;
			t = gs->config.play.battle;
			if (t == 1) return ret;
			if (t == 2) return ret;
			if (t == 3) return ret;
			break;

		case 13:
			t = gs->config.play.battle;
			if (t == 1) return ret;
			if (gs->gameplay.ghostBattle) return ret;
			break;

		case 20:
			if (gs->sSelect.panel == 0) return ret;
			if (gs->sSelect.panel == -1) return ret;
			break;

		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
			if (gs->sSelect.panel == option - 20) return ret;
			break;

		case 30:
			if (gs->config.play.bgasize == 0) return ret;
			break;
		case 31:
			if (gs->config.play.bgasize == 1) return ret;
			break;
		case 32:
			if (gs->procSelecter == 7) return ret;
			if (gs->gameplay.isAutoplay == 0) return ret;
			break;
		case 33:
			if (gs->procSelecter == 7) return ret;
			if (gs->gameplay.isAutoplay == 1) return ret;
			break;
		case 34:
			if (gs->config.play.play_ghost == 0) return ret;
			if (gs->config.play.autojudge == 2) return ret;
			break;
		case 35:
			if (gs->config.play.play_ghost == 1 && gs->config.play.autojudge != 2) return ret;
			break;
		case 36:
			if (gs->config.play.play_ghost == 2 && gs->config.play.autojudge != 2) return ret;
			break;
		case 37:
			if (gs->config.play.play_ghost == 3 && gs->config.play.autojudge != 2) return ret;
			break;
		case 38:
			if (gs->config.play.scoregraph == 0) return ret;
			break;
		case 39:
			if (gs->config.play.scoregraph == 1) return ret;
			break;
		case 40:
			if(gs->config.play.bga == 0) return ret;
			break;
		case 41:
			if (gs->config.play.bga == 1) return ret;
			if (gs->config.play.bga == 3) return ret;
			if (gs->config.play.bga == 2 && (gs->gameplay.isAutoplay == 1 || gs->gameplay.replay.status == 2)) return ret;
			break;

		case 42:
			if (gs->config.play.gaugeOption[0] != 0 && gs->config.play.gaugeOption[0] != 3) return !ret;
			if (gs->gameplay.is2Pplay == 0) return ret;
			break;
		case 43:
			if (gs->config.play.gaugeOption[0] != 0 && gs->config.play.gaugeOption[0] != 3) return ret;
			if (gs->gameplay.is2Pplay == 0) return !ret;
			return ret;
		case 44:
			if (gs->config.play.gaugeOption[1] != 0 && gs->config.play.gaugeOption[1] != 3) return !ret;
			if (gs->gameplay.is2Pplay == 0) return ret;
			break;
		case 45:
			if (gs->config.play.gaugeOption[1] != 0 && gs->config.play.gaugeOption[1] != 3) return ret;
			if (gs->gameplay.is2Pplay == 0) return !ret;
			return ret;

		case 46:
			if (gs->config.select.disabledifficultyfilter == 0) return ret;
			break;

		case 47:
			if (gs->config.select.disabledifficultyfilter == 1) return ret;
			break;

		case 50:
			if (gs->net.isOnline != 1) return ret;
			break;

		case 51:
			if (gs->net.isOnline == 1) return ret;
			break;

		case 52:
			if (gs->config.play.is_extra == 0) return ret;
			break;

		case 53:
			if (gs->config.play.is_extra == 1) return ret;
			break;

		case 54:
			if (gs->config.play.p1_assist == 0) return ret;
			break;

		case 55:
			if (gs->config.play.p1_assist == 1) return ret;
			break;

		case 56:
			if (gs->config.play.p2_assist == 0) return ret;
			break;

		case 57:
			if (gs->config.play.p2_assist == 1) return ret;
			break;

		case 60:
			if (CheckScoreSaveConditon(gs) == 0) return ret;
			break;
		case 61:
			if (CheckScoreSaveConditon(gs) == 1) return ret;
			break;
		case 62:
			if (CheckClearLampChallenge(gs) == 0) return ret;
			break;
		case 63:
			if (CheckClearLampChallenge(gs) == 1) return ret;
			break;
		case 64:
			if (CheckClearLampChallenge(gs) == 2) return ret;
			break;
		case 65:
			if (CheckClearLampChallenge(gs) == 3) return ret;
			break;
		case 66:
			if (CheckClearLampChallenge(gs) == 4) return ret;
			break;

		case 70:
		case 71:
		case 72:
		case 73:
		case 74:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].keymode;
			if (t == 5 || t == 10) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 70] <= gs->config.select.levelbarflash_5) return ret;
			}
			else if (t == 7 || t == 14) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 70] <= gs->config.select.levelbarflash_7) return ret;
			}
			else if (t == 9) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 70] <= gs->config.select.levelbarflash_9) return ret;
			}
			break;

		case 75:
		case 76:
		case 77:
		case 78:
		case 79:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].keymode;
			if (t == 5 || t == 10) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 75] > gs->config.select.levelbarflash_5) return ret;
			}
			else if (t == 7 || t == 14) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 75] > gs->config.select.levelbarflash_7) return ret;
			}
			else if (t == 9) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 75] > gs->config.select.levelbarflash_9) return ret;
			}
			break;

		case 80:
			if (GetTimeLapse(40, (gs->procSelecter == 7) ? &gs->timer2 : &gs->timer1) == -1.0)
				return ret;
			break;
		case 81:
			if (GetTimeLapse(40, (gs->procSelecter == 7) ? &gs->timer2 : &gs->timer1) != -1.0)
				return ret;
			break;
		case 82:
			if (gs->gameplay.replay.status == 0) return ret;
			break;
		case 83:
			if (gs->gameplay.replay.status == 1) return ret;
			break;
		case 84:
			if (gs->gameplay.replay.status == 2) return ret;
			break;

		case 90:
			if (gs->gameplay.player[0].clearType >= 2 && gs->skstruct.flag_flip == 0) return ret;
			if (gs->gameplay.player[1].clearType >= 2 && gs->skstruct.flag_flip != 0) return ret;
			break;
		case 91:
			if (gs->gameplay.player[0].clearType < 2 && gs->skstruct.flag_flip == 0) return ret;
			if (gs->gameplay.player[1].clearType < 2 && gs->skstruct.flag_flip != 0) return ret;
			break;
			
		case 100: //TODO : make more readable
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 0) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 0) return ret;
			}
			else if(gs->config.play.battle == 3){
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 0) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 0) return ret;
			}
			break;
		case 101:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 1) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 1) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 1) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 1) return ret;
			}
			break;
		case 102:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 2) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 2) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 2) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 2) return ret;
			}
			break;
		case 103:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 3) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 3) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 3) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 3) return ret;
			}
			break;
		case 104:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 4) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 4) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 4) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 4) return ret;
			}
			break;
		case 105:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 5) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 5) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 5) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 5) return ret;
			}
			break;

		case 110:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 7)
				return ret;
			break;
		
		case 111:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 6 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 8) return ret; //why???
			break;
		case 112:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 5 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 7) return ret;
			break;
		case 113:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 4 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 6) return ret;
			break;
		case 114:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 3 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 5) return ret;
			break;
		case 115:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 2 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 4) return ret;
			break;
		case 116:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 1 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 3) return ret;
			break;
		case 117:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 2)
				return ret;
			break;

		case 118:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 1) return ret;
			break;
		case 119:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 2) return ret;
			break;
		case 120:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 4) return ret;
			break;
		case 121:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 8) return ret;
			break;
		case 122:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x10) return ret;
			break;
		case 123:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x20) return ret;
			break;
		case 124:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x40) return ret;
			break;
		case 125:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x80) return ret;
			break;
		case 126:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x100) return ret;
			break;
		case 127:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x200) return ret;
			break;
		case 128:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x400) return ret;
			break;
		case 129:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x800) return ret;
			break;
		case 130:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x1000) return ret;
			break;
		case 131:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x2000) return ret;
			break;
		case 132:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x4000) return ret;
			break;
		case 133:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x8000) return ret;
			break;
		case 134:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x10000) return ret;
			break;
		case 135:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x20000) return ret;
			break;
		case 136:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x40000) return ret;
			break;
		case 137:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x80000) return ret;
			break;
		case 138:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x100000) return ret;
			break;
		case 139:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x200000) return ret;
			break;
		case 140:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x400000) return ret;
			break;
		case 141:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x800000) return ret;
			break;
		case 142:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x1000000) return ret;
			break;
		case 143:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x2000000) return ret;
			break;
		case 144:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x4000000) return ret;
			break;
		case 145:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x8000000) return ret;
			break;
		case 146:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x10000000) return ret;
			break;
		case 147:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x20000000) return ret;
			break;
		case 148:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x40000000) return ret;
			break;
		case 149:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x80000000) return ret;
			break;

		case 150:
			if (gs->config.select.disabledifficultyfilter == 1) return ret;
			if (gs->sSelect.metaSelected.difficulty == 0) return ret;
			break;
		case 151:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 1) return ret;
			break;
		case 152:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 2) return ret;
			break;
		case 153:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 3) return ret;
			break;
		case 154:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 4) return ret;
			break;
		case 155:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 5) return ret;
			break;

		case 160:
			if(gs->sSelect.metaSelected.keymode == 7) return ret;
			break;
		case 161:
			if (gs->sSelect.metaSelected.keymode == 5) return ret;
			break;
		case 162:
			if (gs->sSelect.metaSelected.keymode == 14) return ret;
			if (gs->sSelect.metaSelected.keymode == 7 && gs->config.play.battle > 1) return ret;
			break;
		case 163:
			if (gs->sSelect.metaSelected.keymode == 10) return ret;
			if (gs->sSelect.metaSelected.keymode == 5 && gs->config.play.battle > 1) return ret;
			break;
		case 164:
			if (gs->sSelect.metaSelected.keymode == 9) return ret;
			break;

		case 170:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].bga == 0) return ret;
			break;
		case 171:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].bga == 1)return ret;
			break;
		case 172:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].longnote == 0) return ret;
			break;
		case 173:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].longnote == 1)return ret;
			break;
		case 174:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].txt == 0)return ret;
			break;
		case 175:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].txt > 0) return ret;
			break;
		case 176:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].maxBPM == gs->sSelect.bmsList[gs->sSelect.cur_song].minBPM) return ret;
			break;
		case 177:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].maxBPM != gs->sSelect.bmsList[gs->sSelect.cur_song].minBPM) return ret;
			break;
		case 178:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].random == 0) return ret;
			break;
		case 179:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].random == 1) return ret;
			break;
			
		case 180:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].judge < 1) return ret;
			break;
		case 181:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].judge == 1) return ret;
			break;
		case 182:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].judge == 2) return ret;
			break;
		case 183:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].judge > 2) return ret;
			break;

		case 185:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 5 || gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 10) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level < 10) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 7 || gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 14) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level < 13) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 9) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level < 43) return ret;
			}
			break;
		case 186:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 5 || gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 10) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level > 9) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 7 || gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 14) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level > 12) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 9) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level > 42) return ret;
			}
			break;

		case 190:
			if (gs->procSelecter == 7) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isStagefile == 0)return ret;
			break;
		case 191:
			if (gs->procSelecter == 7) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isStagefile == 1)return ret;
			break;
		case 192:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isBanner == 0)return ret;
			break;
		case 193:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isBanner == 1)return ret;
			break;
		case 194:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isBackBMP == 0)return ret;
			break;
		case 195:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isBackBMP == 1) return ret;
			break;
		case 196:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].replayExist == 0) return ret;
			break;
		case 197:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].replayExist == 1)return ret;
			break;

		case 200:
			if (gs->gameplay.player[0].note_current == 0) return !ret;
			if ((gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 8) return !ret;
			return ret;
		case 201:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 8
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 6)
				return ret;
			break;
		case 202:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 7
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 5)
				return ret;
			break;
		case 203:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 6
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 4)
				return ret;
			break;
		case 204:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 5
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 3)
				return ret;
			break;
		case 205:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 4
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 2)
				return ret;
			break;
		case 206:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 3
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 1)
				return ret;
			break;
		case 207:
			if (gs->gameplay.player[0].note_current == 0) return !ret;
			if ((gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) >= 2) return !ret;
			return ret;

		case 210:
			if (gs->gameplay.player[1].note_current == 0) return !ret;
			if ((gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 8) return !ret;
			return ret;
		case 211:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 8
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 6)
				return ret;
			break;
		case 212:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 7
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 5)
				return ret;
			break;
		case 213:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 6
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 4)
				return ret;
			break;
		case 214:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 5
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 3)
				return ret;
			break;
		case 215:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 4
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 2)
				return ret;
			break;
		case 216:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 3
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 1)
				return ret;
			break;
		case 217:
			if (gs->gameplay.player[1].note_current == 0) return !ret;
			if ((gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) >= 2) return !ret;
			return ret;

		case 220:
		case 300:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if ((gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) < 8) return !ret;
			return ret;
		case 221:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 6) return ret;
			break;
		case 222:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 5) return ret;
			break;
		case 223:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 4) return ret;
			break;
		case 224:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 3) return ret;
			break;
		case 225:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 2) return ret;
			break;
		case 226:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 1) return ret;
			break;
		case 227:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if ((gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 1) return !ret;
			return ret;

		case 230:
			if (gs->gameplay.player[0].HP < 0.0) return !ret;
			if (gs->gameplay.player[0].HP < 10.0) return ret;
			break;
		case 231:
			if (gs->gameplay.player[0].HP < 10.0) return !ret;
			if (gs->gameplay.player[0].HP < 20.0) return ret;
			break;
		case 232:
			if (gs->gameplay.player[0].HP < 20.0) return !ret;
			if (gs->gameplay.player[0].HP < 30.0) return ret;
			break;
		case 233:
			if (gs->gameplay.player[0].HP < 30.0) return !ret;
			if (gs->gameplay.player[0].HP < 40.0) return ret;
			break;
		case 234:
			if (gs->gameplay.player[0].HP < 40.0) return !ret;
			if (gs->gameplay.player[0].HP < 50.0) return ret;
			break;
		case 235:
			if (gs->gameplay.player[0].HP < 50.0) return !ret;
			if (gs->gameplay.player[0].HP < 60.0) return ret;
			break;
		case 236:
			if (gs->gameplay.player[0].HP < 60.0) return !ret;
			if (gs->gameplay.player[0].HP < 70.0) return ret;
			break;
		case 237:
			if (gs->gameplay.player[0].HP < 70.0) return !ret;
			if (gs->gameplay.player[0].HP < 80.0) return ret;
			break;
		case 238:
			if (gs->gameplay.player[0].HP < 80.0) return !ret;
			if (gs->gameplay.player[0].HP < 90.0) return ret;
			break;
		case 239:
			if (gs->gameplay.player[0].HP < 90.0) return !ret;
			if (gs->gameplay.player[0].HP < 100.0) return ret;
			break;
		case 240:
			if (gs->gameplay.player[0].HP >= 100.0) return ret;
			break;
		case 241:
			if (gs->gameplay.player[0].judge_draw == 5) return ret;
			break;
		case 242:
			if (gs->gameplay.player[0].judge_draw == 4) return ret;
			break;
		case 243:
			if (gs->gameplay.player[0].judge_draw == 3) return ret;
			break;
		case 244:
			if (gs->gameplay.player[0].judge_draw == 2) return ret;
			break;
		case 245:
			if (gs->gameplay.player[0].judge_draw == 1) return ret;
			break;
		case 246:
			if (gs->gameplay.player[0].judge_draw == 0) return ret;
			break;
		case 247:
			if (GetTimeWrap() - gs->gameplay.misslayerTime[0] >= 1000.0) return ret;
			break;
		case 248:
			if (!(GetTimeWrap() - gs->gameplay.misslayerTime[0] >= 1000.0)) return ret;
			break;

		case 250:
			if (gs->gameplay.player[1].HP < 0.0) return !ret;
			if (gs->gameplay.player[1].HP < 10.0) return ret;
			break;
		case 251:
			if (gs->gameplay.player[1].HP < 10.0) return !ret;
			if (gs->gameplay.player[1].HP < 20.0) return ret;
			break;
		case 252:
			if (gs->gameplay.player[1].HP < 20.0) return !ret;
			if (gs->gameplay.player[1].HP < 30.0) return ret;
			break;
		case 253:
			if (gs->gameplay.player[1].HP < 30.0) return !ret;
			if (gs->gameplay.player[1].HP < 40.0) return ret;
			break;
		case 254:
			if (gs->gameplay.player[1].HP < 40.0) return !ret;
			if (gs->gameplay.player[1].HP < 50.0) return ret;
			break;
		case 255:
			if (gs->gameplay.player[1].HP < 50.0) return !ret;
			if (gs->gameplay.player[1].HP < 60.0) return ret;
			break;
		case 256:
			if (gs->gameplay.player[1].HP < 60.0) return !ret;
			if (gs->gameplay.player[1].HP < 70.0) return ret;
			break;
		case 257:
			if (gs->gameplay.player[1].HP < 70.0) return !ret;
			if (gs->gameplay.player[1].HP < 80.0) return ret;
			break;
		case 258:
			if (gs->gameplay.player[1].HP < 80.0) return !ret;
			if (gs->gameplay.player[1].HP < 90.0) return ret;
			break;
		case 259:
			if (gs->gameplay.player[1].HP < 90.0) return !ret;
			if (gs->gameplay.player[1].HP < 100.0) return ret;
			break;
		case 260:
			if (gs->gameplay.player[1].HP >= 100.0) return ret;
			break;
		case 261:
			if (gs->gameplay.player[1].judge_draw == 5) return ret;
			break;
		case 262:
			if (gs->gameplay.player[1].judge_draw == 4) return ret;
			break;
		case 263:
			if (gs->gameplay.player[1].judge_draw == 3) return ret;
			break;
		case 264:
			if (gs->gameplay.player[1].judge_draw == 2) return ret;
			break;
		case 265:
			if (gs->gameplay.player[1].judge_draw == 1) return ret;
			break;
		case 266:
			if (gs->gameplay.player[1].judge_draw == 0) return ret;
			break;
		case 267:
			if (GetTimeWrap() - gs->gameplay.misslayerTime[1] >= 1000.0) return ret;
			break;
		case 268:
			if (!(GetTimeWrap() - gs->gameplay.misslayerTime[1] >= 1000.0)) return ret;
			break;

		case 270:
			if (gs->procSelecter == 4) {
				if ((gs->KeyInput.p1_buttonInput[12] == 2 || gs->KeyInput.p1_buttonInput[13] == 2) && gs->config.play.p1_lanecover && gs->gameplay.lanecoverDisplayP1) return ret;
				if ((gs->KeyInput.p2_buttonInput[12] == 2 || gs->KeyInput.p2_buttonInput[13] == 2) && gs->config.play.p1_lanecover && gs->gameplay.lanecoverDisplayP1 && gs->config.play.battle != 1 && gs->config.play.battle != 4) return ret;
			}
			break;
		case 271:
			if (gs->procSelecter == 4) {
				if (gs->KeyInput.p2_buttonInput[12] == 2) return ret;
				if (gs->KeyInput.p2_buttonInput[13] == 2) return ret;
			}
			break;

		case 280:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 0 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 281:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 1 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 282:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 2 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 283:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 3 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 284:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 4 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 285:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 5 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 286:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 6 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 287:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 7 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 288:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 8 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 289:
			if ((gs->gameplay.courseStageNow == gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 290:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseType != -1) return ret;
			break;
		case 291:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseType == 1) return ret;
			break;
		case 292:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseType == 0) return ret;
			break;
		case 293:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseType == 2) return ret;
			break;

			//300 is smae 220
		case 301:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 7) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 7) return !ret;
			return ret;
			break;
		case 302:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 6) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 6) return !ret;
			return ret;
			break;
		case 303:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 5) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 5) return !ret;
			return ret;
			break;
		case 304:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 4) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 4) return !ret;
			return ret;
			break;
		case 305:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 3) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 3) return !ret;
			return ret;
			break;
		case 306:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 2) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 2) return !ret;
			return ret;
			break;
		case 307:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 1) return !ret;
			return ret;
			break;
		case 308:
			if (gs->gameplay.player[0].exscore == 0) return ret;
			if (gs->gameplay.player[0].totalnotes == 0) return ret;
			break;

		case 310:
			if (gs->gameplay.player[1].totalnotes == 0) return !ret;
			if (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2) < 8) return !ret;
			return ret;
			break;
		case 311:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 8
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 6) return ret;
			break;
		case 312:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 7
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 5) return ret;
			break;
		case 313:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 6
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 4) return ret;
			break;
		case 314:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 5
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 3) return ret;
			break;
		case 315:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 4
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 2) return ret;
			break;
		case 316:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 3
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 1) return ret;
			break;
		case 317:
			if (gs->gameplay.player[1].totalnotes == 0) return !ret;
			if (gs->gameplay.player[1].exscore == 0) return !ret;
			if (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2) > 1) return !ret;
			return ret;
		case 318:
			if (gs->gameplay.player[1].exscore == 0) return ret;
			if (gs->gameplay.player[1].totalnotes == 0) return ret;
			break;

		case 320:
			if (gs->sSelect.old.total_notes == 0) return !ret;
			if (gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 8) return !ret;
			return ret;
			break;
		case 321:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 6
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 8) return ret;
			break;
		case 322:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 5
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 7) return ret;
			break;
		case 323:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 4
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 6) return ret;
			break;
		case 324:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 3
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 5) return ret;
			break;
		case 325:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 2
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 4) return ret;
			break;
		case 326:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 1
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 3) return ret;
			break;
		case 327:
			if (gs->sSelect.old.total_notes == 0) return !ret;
			if (gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 1) return !ret;
			return ret;
			break;

		case 330:
			if (gs->skstruct.flag_flip == 0) {
				if (gs->gameplay.player[0].exscore <= gs->sSelect.old.stat_exscore) return !ret;
				return ret;
			}
			if (gs->gameplay.player[1].exscore > gs->sSelect.old.stat_exscore) return ret;
			break;
		case 331:
			if (gs->skstruct.flag_flip == 0) {
				if (gs->gameplay.player[0].max_combo > gs->sSelect.old.stat_maxcombo) {
					return ret;
				}
			}
			else if (gs->gameplay.player[1].max_combo > gs->sSelect.old.stat_maxcombo) {
				return ret;
			}
			break;
		case 332:
			if (gs->skstruct.flag_flip == 0) {
				if (gs->gameplay.player[0].totalnotes - gs->gameplay.player[0].note_current + gs->gameplay.player[0].judgecount[2] + gs->gameplay.player[0].judgecount[1] < gs->sSelect.old.minbp) return ret;
			}
			else if (gs->gameplay.player[1].totalnotes - gs->gameplay.player[1].note_current + gs->gameplay.player[1].judgecount[2] + gs->gameplay.player[1].judgecount[1] < gs->sSelect.old.minbp) return ret;
			break;
		case 333:
			if (gs->gameplay.trialClear) return ret;
			break;
		case 334:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.IRranking == 0) {
				if (gs->sSelect.oldIRrank > 0) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.IRranking < gs->sSelect.oldIRrank) return ret;
			break;
		case 335:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes != 0
				&& gs->sSelect.old.total_notes != 0
				&& (gs->sSelect.old.stat_exscore*9 / (gs->sSelect.old.total_notes*2) < gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes*2))) 
				return ret;
			break;

		case 340:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 8) return !ret;
			return ret;
			break;
		case 341:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 7) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 7) return !ret;
			return ret;
			break;
		case 342:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 6) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 6) return !ret;
			return ret;
			break;
		case 343:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 5) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 5) return !ret;
			return ret;
			break;
		case 344:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 4) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 4) return !ret;
			return ret;
			break;
		case 345:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 3) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 3) return !ret;
			return ret;
			break;
		case 346:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 2) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 2) return !ret;
			return ret;
			break;
		case 347:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 1) return !ret;
			return ret;
			break;

		case 350:
			if (gs->skstruct.flag_flip == 0) return ret;
			break;
		case 351:
			if (gs->skstruct.flag_flip != 0) return ret;
			break;
		case 352:
			if (gs->gameplay.player[0].exscore > gs->gameplay.player[1].exscore) return ret;
			break;
		case 353:
			if (gs->gameplay.player[0].exscore < gs->gameplay.player[1].exscore) return ret;
			break;
		case 354:
			if (gs->gameplay.player[0].exscore == gs->gameplay.player[1].exscore) return ret;
			break;

		case 400:
			if (gs->KeyInput.config_keymode == 0) return ret;
			break;
		case 401:
			if (gs->KeyInput.config_keymode == 1) return ret;
			break;
		case 402:
			if (gs->KeyInput.config_keymode == 2) return ret;
			break;

		case 500:
		case 501:
		case 502:
		case 503:
		case 504:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 500] == -1)
				return ret;
			break;
		case 505:
		case 506:
		case 507:
		case 508:
		case 509:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 505] != -1)
				return ret;
			break;
		case 510:
		case 511:
		case 512:
		case 513:
		case 514:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyExist[option - 510] == 1)
				return ret;
			break;
		case 515:
		case 516:
		case 517:
		case 518:
		case 519:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyExist[option - 515] > 1)
				return ret;
			break;
		case 520:
		case 521:
		case 522:
		case 523:
		case 524:
		case 525:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[0] == option - 520)
				return ret;
			break;
		case 530:
		case 531:
		case 532:
		case 533:
		case 534:
		case 535:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[1] == option - 530)
				return ret;
			break;
		case 540:
		case 541:
		case 542:
		case 543:
		case 544:
		case 545:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[2] == option - 540)
				return ret;
			break;
		case 550:
		case 551:
		case 552:
		case 553:
		case 554:
		case 555:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[3] == option - 550)
				return ret;
			break;
		case 560:
		case 561:
		case 562:
		case 563:
		case 564:
		case 565:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[4] == option - 560)
				return ret;
			break;

		case 570: //TOFIX : forgot break?
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 7)return ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 9)return ret;
		case 580:
		case 581:
		case 582:
		case 583:
		case 584:
		case 585:
		case 586:
		case 587:
		case 588:
		case 589:
			if (gs->sSelect.course.isMakingCourse == 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageCount > option - 580) return ret;
			break;
		case 571: //TOFIX : forgot break?
			if (gs->sSelect.course.isMakingCourse == 1) return ret;
		case 572: //TOFIX : forgot break?
			if (gs->sSelect.course.isMakingCourse == 0) return ret;
			if(gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[0] == option - 700) return ret;
			break;
			
		case 590:
		case 591:
		case 592:
		case 593:
		case 594:
		case 595:
		case 596:
		case 597:
		case 598:
		case 599:
			if (gs->sSelect.course.isMakingCourse != 1) return !ret;
			if (gs->sSelect.course.count == option - 590) return ret;
			break;

		case 600:
			if (gs->net.IRstatus == 0) return ret;
			break;
		case 601:
			if (gs->net.IRstatus == 1) return ret;
			break;
		case 602:
			if (gs->net.IRstatus == 2) return ret;
			break;
		case 603:
			if (gs->net.IRstatus == -1) return ret;
			break;
		case 604:
			if (gs->net.IRstatus == -3) return ret;
			break;
		case 605:
			if (gs->net.IRstatus == -2) return ret;
			break;
		case 606:
			if (gs->net.IRstatus == 3) return ret;
			break;
		case 607:
			if (gs->net.IRstatus == 4) return ret;
			break;
		case 608:
			if (gs->net.IRstatus == 5) return ret;
			break;

		case 610: //TOFIX : seems like meaningless...
			if (gs->net.rankingData.rankingCount == 0) return ret;
			break;
		case 611:
			if (gs->net.rankingData.rankingCount > 0) return ret;
			break;
		case 612:
			if (gs->net.rankingData.rankingCount > 1) return ret;
			break;
		case 613:
			if (gs->net.rankingData.rankingCount > 2) return ret;
			break;
		case 614:
			if (gs->net.rankingData.rankingCount > 3) return ret;
			break;
		case 615:
			if (gs->net.rankingData.rankingCount > 4) return ret;
			break;
		case 616:
			if (gs->net.rankingData.rankingCount > 5) return ret;
			break;

		case 620:
			if (gs->net.rankingData.showRanking == 0) return ret;
			break;
		case 621:
			if (gs->net.rankingData.showRanking != 0) return ret;
			break;
		case 622:
			if (gs->gameplay.ghostBattle == 0) return ret;
			break;
		case 623: //original place is between case 13 and 20
			if (gs->gameplay.ghostBattle != 0) return ret;
			break;

		case 624:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.stat_exscore == 0) return ret;
			break;
		case 625:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.stat_exscore > 0) return ret;
			break;

		case 640:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 0) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 0) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 0) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 0) return ret;
			}
			break;
		case 641:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 1) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 1) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 1) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 1) return ret;
			}
			break;
		case 642:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 2) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 2) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 2) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 2) return ret;
			}
			break;
		case 643:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 3) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 3) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 3) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 3) return ret;
			}
			break;
		case 644:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 4) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 4) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 4) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 4) return ret;
			}
			break;
		case 645:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 5) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 5) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 5) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 5) return ret;
			}
			break;
			
		case 650:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank >= 8) return ret;
			break;
		case 651:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 7 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 8) return ret;
			break;
		case 652:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 6 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 7) return ret;
			break;
		case 653:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 5 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 6) return ret;
			break;
		case 654:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 4 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 5) return ret;
			break;
		case 655:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 3 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 4) return ret;
			break;
		case 656:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 2 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 3) return ret;
			break;
		case 657:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 2) return ret;
			break;

		case 700:
		case 701:
		case 702:
		case 703:
		case 704:
		case 705:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[0] == option - 700) return ret;
			break;
		case 710:
		case 711:
		case 712:
		case 713:
		case 714:
		case 715:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[1] == option - 710) return ret;
			break;
		case 720:
		case 721:
		case 722:
		case 723:
		case 724:
		case 725:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[2] == option - 720) return ret;
			break;
		case 730:
		case 731:
		case 732:
		case 733:
		case 734:
		case 735:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[3] == option - 730) return ret;
			break;
		case 740:
		case 741:
		case 742:
		case 743:
		case 744:
		case 745:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[4] == option - 740) return ret;
			break;

		case 800:
		case 801:
		case 802:
		case 803:
		case 804:
		case 805:
		case 806:
		case 807:
		case 808:
		case 809:
			if (gs->po4cur_song == option - 800) return ret;
			break;

		case 810:
		case 811:
		case 812:
		case 813:
		case 814:
		case 815:
		case 816:
		case 817:
		case 818:
		case 819:
		case 820:
		case 821:
		case 822:
		case 823:
		case 824:
		case 825:
		case 826:
		case 827:
		case 828:
		case 829:
			if (gs->po4procSelecter == option - 810) return ret;
			break;
		case 830:
			if (gs->gameplay.bmsResourceLoaded) return ret;
			break;

		default:
			if (option - 900 < 100) {
				if (gs->procSelecter == 7) {
					if (gs->skstruct2.op[option]) return ret;
				}
				else if (gs->skstruct.op[option]) return ret;
			}
			break;
	}

	return !ret;
}

//412ae0
int SetObjectStrings_SongSelect(game *g) {

	SetObjectString(60, g->txtStruct.option_str[0].str[g->config.select.key], g->txtStruct.objectStr);
	SetObjectString(61, g->txtStruct.option_str[1].str[g->config.select.key], g->txtStruct.objectStr);
	SetObjectString(62, g->txtStruct.option_str[2].str[g->config.select.difficulty], g->txtStruct.objectStr);
	SetObjectString(63, g->txtStruct.option_str[4].str[g->config.play.random[0]], g->txtStruct.objectStr);
	SetObjectString(64, g->txtStruct.option_str[4].str[g->config.play.random[1]], g->txtStruct.objectStr);
	SetObjectString(65, g->txtStruct.option_str[3].str[g->config.play.gaugeOption[0]], g->txtStruct.objectStr);
	SetObjectString(66, g->txtStruct.option_str[3].str[g->config.play.gaugeOption[1]], g->txtStruct.objectStr);
	SetObjectString(67, g->txtStruct.option_str[19].str[g->config.play.p1_assist], g->txtStruct.objectStr);
	SetObjectString(68, g->txtStruct.option_str[19].str[g->config.play.p2_assist], g->txtStruct.objectStr);
	SetObjectString(84, g->txtStruct.option_str[5].str[g->config.play.m_HIDSUD1], g->txtStruct.objectStr);
	SetObjectString(85, g->txtStruct.option_str[5].str[g->config.play.m_HIDSUD2], g->txtStruct.objectStr);

	if (g->config.play.battle == 0) {
		SetObjectString(69, g->txtStruct.option_str[7].str[0], g->txtStruct.objectStr);
	}
	else if (g->config.play.battle == 1) {
		int k = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
		if (k == 10 || k == 14) 
			SetObjectString(69, g->txtStruct.option_str[7].str[5], g->txtStruct.objectStr);
		else if (k == 9) 		
			SetObjectString(69, g->txtStruct.option_str[7].str[6], g->txtStruct.objectStr);
		else
			SetObjectString(69, g->txtStruct.option_str[7].str[1], g->txtStruct.objectStr);
	}
	else if (g->config.play.battle == 2) {
		int k = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
		if (k == 10 || k == 14 || k == 9)
			SetObjectString(69, g->txtStruct.option_str[7].str[6], g->txtStruct.objectStr);
		else
			SetObjectString(69, g->txtStruct.option_str[7].str[2], g->txtStruct.objectStr);
	}
	else if (g->config.play.battle == 3) {
		int k = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
		if (k == 10 || k == 14)
			SetObjectString(69, g->txtStruct.option_str[7].str[7], g->txtStruct.objectStr);
		else if (k == 9)
			SetObjectString(69, g->txtStruct.option_str[7].str[8], g->txtStruct.objectStr);
		else
			SetObjectString(69, g->txtStruct.option_str[7].str[3], g->txtStruct.objectStr);
	}
	else if (g->config.play.battle == 4) {
		int k = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
		if (k == 10 || k == 14 || k == 9)
			SetObjectString(69, g->txtStruct.option_str[7].str[6], g->txtStruct.objectStr);
		else
			SetObjectString(69, g->txtStruct.option_str[7].str[4], g->txtStruct.objectStr);
	}

	SetObjectString(70, g->txtStruct.option_str[6].str[g->config.play.dpflip], g->txtStruct.objectStr);
	SetObjectString(71, g->txtStruct.option_str[16].str[g->config.play.scoregraph], g->txtStruct.objectStr);
	SetObjectString(72, g->txtStruct.option_str[14].str[g->config.play.play_ghost], g->txtStruct.objectStr);
	SetObjectString(73, g->txtStruct.option_str[15].str[g->config.play.p1_lanecover], g->txtStruct.objectStr);
	SetObjectString(74, g->txtStruct.option_str[10].str[g->config.play.hsfix], g->txtStruct.objectStr);
	SetObjectString(76, g->txtStruct.option_str[9].str[g->config.play.bga], g->txtStruct.objectStr);
	SetObjectString(75, g->txtStruct.option_str[8].str[g->config.play.bgasize], g->txtStruct.objectStr);
	SetObjectString(77, g->txtStruct.option_str[13].str[g->config.system.highcolor], g->txtStruct.objectStr);
	SetObjectString(78, g->txtStruct.option_str[11].str[g->config.system.vsync], g->txtStruct.objectStr);
	SetObjectString(79, g->txtStruct.option_str[12].str[g->config.system.screenmode], g->txtStruct.objectStr);
	SetObjectString(80, g->txtStruct.option_str[17].str[g->config.play.autojudge], g->txtStruct.objectStr);
	SetObjectString(81, g->txtStruct.option_str[18].str[g->config.play.replay], g->txtStruct.objectStr);

	SetObjectString(2, g->config.player.id, g->txtStruct.objectStr);
	SetObjectString(82, GetMissonString(g->gameplay.playerstat.trial, 0), g->txtStruct.objectStr);
	SetObjectString(2, g->config.player.id, g->txtStruct.objectStr);
	SetObjectString(83, GetMissonString(g->gameplay.playerstat.trial, 1), g->txtStruct.objectStr);

	if (g->config.course.optimumlevel_7 >= 1) {
		CSTR tmp;
		cstrSprintf(&tmp, "LEVEL %d", g->config.course.optimumlevel_7);
		SetObjectString(190, tmp, g->txtStruct.objectStr);
	}
	else {
		SetObjectString(190, "OFF", g->txtStruct.objectStr);
	}

	if (g->config.course.maxlevel >= 1) {
		CSTR tmp;
		cstrSprintf(&tmp, "LEVEL %d", g->config.course.maxlevel);
		SetObjectString(191, tmp, g->txtStruct.objectStr);
	}
	else {
		SetObjectString(191, "NO LIMIT", g->txtStruct.objectStr);
	}

	if (g->config.course.minlevel >= 1) {
		CSTR tmp;
		cstrSprintf(&tmp, "LEVEL %d", g->config.course.minlevel);
		SetObjectString(192, tmp, g->txtStruct.objectStr);
	}
	else {
		SetObjectString(192, "NO LIMIT", g->txtStruct.objectStr);
	}

	SetObjectString(198, g->txtStruct.option_str[20].str[g->config.course.defaultconnection], g->txtStruct.objectStr);

	if (g->config.course.maxbpm >= 1) {
		CSTR tmp;
		cstrSprintf(&tmp, "%d BPM", g->config.course.maxbpm);
		SetObjectString(194, tmp, g->txtStruct.objectStr);
	}
	else {
		SetObjectString(194, "NO LIMIT", g->txtStruct.objectStr);
	}

	if (g->config.course.minbpm >= 1) {
		CSTR tmp;
		cstrSprintf(&tmp, "%d BPM", g->config.course.minbpm);
		SetObjectString(195, tmp, g->txtStruct.objectStr);
	}
	else {
		SetObjectString(195, "NO LIMIT", g->txtStruct.objectStr);
	}

	if (g->config.course.bpmrange >= 1) {
		CSTR tmp;
		cstrSprintf(&tmp, "+- %dBPM", g->config.course.bpmrange);
		SetObjectString(193, tmp, g->txtStruct.objectStr);
	}
	else {
		SetObjectString(193, "NO LIMIT", g->txtStruct.objectStr);
	}

	if (g->config.course.stage >= 1) {
		CSTR tmp;
		cstrSprintf(&tmp, "%d STAGE", g->config.course.stage);
		SetObjectString(196, tmp, g->txtStruct.objectStr);
	}
	else {
		SetObjectString(196, "RANDOM", g->txtStruct.objectStr);
	}

	SetObjectString(199, g->txtStruct.option_str[22].str[g->config.course.defaultgauge], g->txtStruct.objectStr);

	return 1;
}

//4134b0
int SetObjectValue_Slider(game *g, skstruct *sk, Timer *T, char flag) {
	int mouseL, mouseR;
	int iBuf, iTmp;

	mouseL = g->KeyInput.mouse_buttonL;
	mouseR = g->KeyInput.mouse_buttonR;

	if (g->txtStruct.readme.show > 0 || flag || g->sSelect.searchFocused) {
		g->KeyInput.mouse_buttonL = 0;
		g->KeyInput.mouse_buttonR = 0;
	}

	for (int i = 0; i < sk->otherObject[2].srcSize; i++) {
		if (GetOptionFlag_dst(g, sk->otherObject[2].dst[i].opt1) == 0 || GetOptionFlag_dst(g, sk->otherObject[2].dst[i].opt2) == 0 || GetOptionFlag_dst(g, sk->otherObject[2].dst[i].opt3) == 0) continue;
		if (sk->otherObject[2].src[i].op2 < 1) continue;

		iBuf = 0;

		switch (sk->otherObject[2].src[i].op3) {
			case 1:
				iBuf = g->sSelect.listCalculatedBar;
				SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, g->sSelect.bmsListCount*1000 - 1, &iBuf, &g->KeyInput, i);
				if (iBuf != g->sSelect.listCalculatedBar) {
					g->sSelect.listTopbar = iBuf;
					g->sSelect.oldBar = iBuf;
					iTmp = iBuf / 1000;
					if (iBuf % 1000 > 499) {
						iTmp++;
					}
					g->sSelect.nowBar = iTmp * 1000;
					g->sSelect.barMoveStartTime = GetTimeWrap();
					g->sSelect.barMoveEndTime = GetTimeWrap() + 100;
				}
				break;

			case 2:
				SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 10, 990, &g->config.play.hiSpeed[0], &g->KeyInput, i);
				break;

			case 3:
				SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 10, 990, &g->config.play.hiSpeed[1], &g->KeyInput, i);
				break;

			case 4:
				if (g->config.play.p1_lanecover == 1 && g->gameplay.lanecoverDisplayP1 == 1)
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 100, &g->config.play.p1_lanecoverv, &g->KeyInput, i);
				else
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 100, &iBuf, &g->KeyInput, i);
				break;

			case 5:
				if (g->config.play.p2_lanecover == 1 && g->gameplay.lanecoverDisplayP2 == 1)
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 100, &g->config.play.p2_lanecoverv, &g->KeyInput, i);
				else
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 100, &iBuf, &g->KeyInput, i);
					
				break;

			case 6:
				iBuf = GetTimeLapse(41, T);
				if (iBuf >= 0) {
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, g->gameplay.song_runtime, &iBuf, &g->KeyInput, i);
				}
				else SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 1, &iBuf, &g->KeyInput, i); 
				break;

			case 7:
				if (g->skinData.Data[g->skinData.previewID].custom_count - 5 > 0) {
					if (SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, g->skinData.Data[g->skinData.previewID].custom_count - 5, &g->skinData.previewCustomID, &g->KeyInput, i)) {
						for (int j = 0; j < 5; j++) {
							SetObjectString(100 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].title, g->txtStruct.objectStr);
							SetObjectString(110 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].
								op_label[g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_selected - g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_start], g->txtStruct.objectStr);

						}
					}
				}
				else SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 1, &iBuf, &g->KeyInput, i);
				break;

			case 8:
				if (g->net.rankingData.rankingCount < 6) {
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 1, &iBuf, &g->KeyInput, i);
				}
				else {
					if (SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 1, &iBuf, &g->KeyInput, i) == 1) {
						SetObjectStrings_SongSelect(g);
					}
				}
				break;

			case 10:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[0], i, -12, 12) && GetTimeLapse(41,T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '3', g->audio.param.eq_gain[0]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 11:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[1], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '4', g->audio.param.eq_gain[1]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 12:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[2], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '5', g->audio.param.eq_gain[2]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 13:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[3], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '3', g->audio.param.eq_gain[3]); //TOFIX : 3 -> 6, looks like mistyping
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 14:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[4], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '7', g->audio.param.eq_gain[4]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 15:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[5], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '8', g->audio.param.eq_gain[5]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 16:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[6], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '9', g->audio.param.eq_gain[6]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 17:
				if (UpdateSoundFX(g, &g->audio.param.volume_master, i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), ')', g->audio.param.volume_master);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 18:
				if (UpdateSoundFX(g, &g->audio.param.volume_key, i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '*', g->audio.param.volume_key);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 19:
				if (UpdateSoundFX(g, &g->audio.param.volume_BGM, i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '+', g->audio.param.volume_BGM);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 20:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[0][0], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '>', g->audio.param.fxParam[0][0]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 21:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[0][1], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '?', g->audio.param.fxParam[0][1]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 22:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[1][0], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 'H', g->audio.param.fxParam[1][0]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 23:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[1][1], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 'I', g->audio.param.fxParam[1][1]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 24:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[2][0], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 'R', g->audio.param.fxParam[2][0]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 25:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[2][1], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 'S', g->audio.param.fxParam[2][1]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 26:
				if (UpdateSoundFX(g, &g->audio.param.pitch_amount, i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '[', g->audio.param.pitch_amount);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
		}
	}
	g->KeyInput.mouse_buttonL = mouseL;
	g->KeyInput.mouse_buttonR = mouseR;
	return 1;
}

//413f70
int CmdSearch(game *g, CSTR *cmd, sqlite3 *sql) {

	cmd->lower();

	if (cmd->isSame("/hash")) {
		*cmd = g->sSelect.bmsList[g->sSelect.cur_song].hash;
		return 1;
	}
	else if (cmd->isSame("/path")) {
		*cmd = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
		return 1;
	}
	else if (cmd->isSame("/avi")) {
		*cmd = "次のオートプレイかリプレイを録画します";
		g->rec.recMode = 4;
		return 1;
	}
	else if (cmd->isSame("/deletescore")) {
		DeleteScoreFromDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, sql);
		memset(&g->sSelect.bmsList[g->sSelect.cur_song].mybest, 0, sizeof(STATUS));
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp = -1;
		*cmd = "deleted";
		return 1;
	}
	else if (cmd->isSame("/irupdateall")) { //TOFIX: input device MIDI??
		for (g->sSelect.cur_song = 0; g->sSelect.cur_song < g->sSelect.bmsListCount; g->sSelect.cur_song++) {
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore > 0) {
				g->net.myRanking.InitRanking();
				BMSMETA meta;
				ParseBMSMETA(&meta, g->sSelect.bmsList[g->sSelect.cur_song].filepath, 0);
				g->net.myRanking.songMD5 = g->sSelect.bmsList[g->sSelect.cur_song].hash;
				g->net.myRanking.passMD5 = g->net.IR_passMD5;
				g->net.myRanking.title = meta.title;
				if (meta.subtitle.length() > 0) {
					g->net.myRanking.title.add(" ");
					g->net.myRanking.title.add(meta.subtitle);
				}
				g->net.myRanking.genre = meta.genre;
				g->net.myRanking.artist = meta.artist;
				if (meta.subartist.length() > 0) {
					g->net.myRanking.artist.add(" ");
					g->net.myRanking.artist.add(meta.subartist);
				}
				g->net.myRanking.maxbpm = meta.maxbpm;
				g->net.myRanking.minbpm = meta.minbpm;
				g->net.myRanking.playlevel = meta.selLevel;
				g->net.myRanking.clear = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear;
				g->net.myRanking.exscore = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore;
				g->net.myRanking.pg = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat;
				g->net.myRanking.gr = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great;
				g->net.myRanking.gd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good;
				g->net.myRanking.bd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad;
				g->net.myRanking.pr = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor;
				g->net.myRanking.maxcombo = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo;
				g->net.myRanking.playcount = g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount;
				g->net.myRanking.clearcount = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount;
				g->net.myRanking.rate = g->sSelect.bmsList[g->sSelect.cur_song].mybest.rate;
				g->net.myRanking.minbp = g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp;
				g->net.myRanking.totalnotes = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
				g->net.myRanking.opt_history = g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history;
				g->net.myRanking.opt_this = g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best;
				g->net.myRanking.rseed = g->sSelect.bmsList[g->sSelect.cur_song].mybest.rseed;
				g->net.myRanking.clear_db = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_db;
				g->net.myRanking.clear_sd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_sd;

				g->net.myRanking.line = meta.keymode;
				g->net.myRanking.clear_ex = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_ex;
				g->net.myRanking.judge = meta.judge;
				g->net.myRanking.inputtype = 2;

				g->net.myRanking.ghost = ReadGhost(sql, g->sSelect.bmsList[g->sSelect.cur_song].hash);
				CSTR scorehash;
				cstrSprintf(&scorehash, "%s%s%d%d", g->net.IR_passMD5, g->net.myRanking.songMD5, g->net.myRanking.exscore, g->net.myRanking.clear);
				scorehash = MD5str(scorehash);
				cstrSprintf(&g->net.param, "songmd5=%s&id=%d&passmd5=%s&title=%s&genre=%s&artist=%s&maxbpm=%d&minbpm=%d&&playlevel=%d&clear=%d&exscore=%d&pg=%d&gr=%d&gd=%d&bd=%d&pr=%d&maxcombo=%d&playcount=%d&clearcount=%d&rate=%d&minbp=%d&totalnotes=%d&opt_history=%d&opt_this=%d&line=%d&judge=%d&inputtype=%d&ghost=%s&rseed=%d&clear_db=%d&clear_ex=%d&clear_sd=%d&scorehash=%s",
					g->net.myRanking.songMD5, g->net.IR_ID, g->net.IR_passMD5, g->net.myRanking.title, g->net.myRanking.genre, g->net.myRanking.artist, g->net.myRanking.maxbpm, g->net.myRanking.minbpm, g->net.myRanking.playlevel, g->net.myRanking.clear,
					g->net.myRanking.exscore, g->net.myRanking.pg, g->net.myRanking.gr, g->net.myRanking.gd, g->net.myRanking.bd, g->net.myRanking.pr, g->net.myRanking.maxcombo, g->net.myRanking.playcount, g->net.myRanking.clearcount, g->net.myRanking.rate,
					g->net.myRanking.minbp, g->net.myRanking.totalnotes, g->net.myRanking.opt_history, g->net.myRanking.opt_this, g->net.myRanking.line, g->net.myRanking.judge, g->net.myRanking.inputtype, g->net.myRanking.ghost, g->net.myRanking.rseed,
					g->net.myRanking.clear_db, g->net.myRanking.clear_ex, g->net.myRanking.clear_sd, scorehash);
				g->net.target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/score.cgi";
				g->net.HTTPrequest();
				printfDx("%sを送信中です…", g->net.myRanking.title);
				ScreenFlip();
				ClsDrawScreen();
				clsDx();
				ProcessMessage();
			}
		}
		return 1;
	}
	else if (cmd->isSame("/uninstall")) {
		if (g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5) {

			UninstallSong(g->sSelect.bmsList[g->sSelect.cur_song].filepath, sql);

			for (int i = 0; i < g->sSelect.bmsListCount; i++) {
				if (g->sSelect.bmsList[i].folder.isDiff(g->sSelect.bmsList[g->sSelect.cur_song].folder)) {
					g->sSelect.filter_clicked = 4;
					SetBmsFilter(g, sql);
					*cmd = "SUCCEED";
					return 1;
				}
			}
			g->sSelect.queryCount = 1;
			g->sSelect.searchFocused = 1;
			g->sSelect.searchType = 2;
			g->sSelect.curQuery[0] = g->sSelect.stack_query[g->sSelect.cur + -1];
			g->sSelect.filterSort = g->config.select.sort;
			g->sSelect.searchMax = g->sSelect.stack_rivalID[g->sSelect.cur + -1];
			g->sSelect.filterKey = g->config.select.key;
			g->sSelect.filterDifficulty = g->config.select.difficulty;
			g->sSelect.selFolder = g->sSelect.bmsList[g->sSelect.cur_song].folder;
			g->sSelect.selTitle = g->sSelect.bmsList[g->sSelect.cur_song].title;
			g->sSelect.selFilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
			g->sSelect.selKey = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
			*cmd = "SUCCEED";
			return 1;
		}
	}
	else if (cmd->isSame("/rename")) {

		if (g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5) {

			Rename(g->sSelect.bmsList[g->sSelect.cur_song].filepath, sql);

			if (g->sSelect.bmsListCount <= 1) {
				g->sSelect.queryCount = 1;
				g->sSelect.searchFocused = 1;
				g->sSelect.searchType = 2;
				g->sSelect.curQuery[0] = g->sSelect.stack_query[g->sSelect.cur - 1];
				g->sSelect.filterSort = g->config.select.sort;
				g->sSelect.searchMax = g->sSelect.stack_rivalID[g->sSelect.cur + -1];
				g->sSelect.filterKey = g->config.select.key;
				g->sSelect.filterDifficulty = g->config.select.difficulty;
				g->sSelect.selFolder = g->sSelect.bmsList[g->sSelect.cur_song].folder;
				g->sSelect.selTitle = g->sSelect.bmsList[g->sSelect.cur_song].title;
				g->sSelect.selFilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
				g->sSelect.selKey = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
				*cmd = "SUCCEED";
			}
			else {
				g->sSelect.filter_clicked = 4;
				SetBmsFilter(g, sql);
				*cmd = "SUCCEED";
			}

			return 1;
		}
	}
	else {
		*cmd = "COMMAND ERROR";
		return 1;
	}
}

//414900
int ReleaseSysSound(game *g){
	AUDIO *aud;

	StopSysSound(g);
	aud = &g->audio;
	ReleaseSound(aud, &g->audio.sysSound.select);
	ReleaseSound(aud, &g->audio.sysSound.folder_open);
	ReleaseSound(aud, &g->audio.sysSound.folder_close);
	ReleaseSound(aud, &g->audio.sysSound.panel_open);
	ReleaseSound(aud, &g->audio.sysSound.panel_close);
	ReleaseSound(aud, &g->audio.sysSound.screenshot);
	ReleaseSound(aud, &g->audio.sysSound.option_change);
	ReleaseSound(aud, &g->audio.sysSound.decide);
	ReleaseSound(aud, &g->audio.sysSound.clear);
	ReleaseSound(aud, &g->audio.sysSound.fail);
	ReleaseSound(aud, &g->audio.sysSound.stop);
	ReleaseSound(aud, &g->audio.sysSound.mine);
	ReleaseSound(aud, &g->audio.sysSound.scratch);
	ReleaseSound(aud, &g->audio.sysSound.exselect);
	ReleaseSound(aud, &g->audio.sysSound.exdecide);
	ReleaseSound(aud, &g->audio.sysSound.difficulty);
	ReleaseSound(aud, &g->audio.sysSound.courseclear);
	ReleaseSound(aud, &g->audio.sysSound.coursefail);
	return 1;
}

//414a10
int ReadLR2SoundSet(game *g, CSTR filepath, char reFlag) {
	SkinUser sku;
	FILE* hFile;
	CSVbuf csv;
	int IFCOUNT, IFSWITCH[100];
	int dst_op[1000];
	CSTR wildStr[20];
	CSTR wildCFilename[20];
	int wildCount = 0;

	char load_exselect = '\0';
	char load_exdecide = '\0';
	char load_select = '\0';
	char load_folder_open = '\0';
	char load_folder_close = '\0';
	char load_scratch = '\0';
	char load_panel_open = '\0';
	char load_panel_close = '\0';
	char load_screenshot = '\0';
	char load_option_change = '\0';
	char load_decide = '\0';
	char load_clear = '\0';
	char load_fail = '\0';
	char load_stop = '\0';
	char load_mine = '\0';
	char load_difficulty = '\0';
	char load_courseclear = '\0';
	char load_coursefail = '\0';

	if (reFlag == 0) ReleaseSysSound(g);
	if (g->audio.is_fmod_disabled == 0) FMOD_System_Update(g->audio.fmodSys);

	CSTR path;
	cstrSprintf(&path, "LR2files\\SkinCustomize\\%s.xml", MD5str(filepath));
	ReadSkinCustomize(&sku, path);
	
	CSTR dir(filepath.getDirectory());

	for (int i = 0; i < 100; i++) {
		IFSWITCH[i] = 0;
	}
	IFCOUNT = 0;

	for (int i = 0; i < 20; i++) {
		int t = sku.customize_value[i];
		if (899 < t &&  t < 1000) dst_op[t] = 1;
	}
	hFile = fopen(filepath, "r");
	if (!hFile) return 0;
	CSTR fBuf(260);
	char* pFbuf;
	pFbuf = fBuf.outstr();
	for (pFbuf = fgets(pFbuf, 256, hFile); pFbuf; pFbuf = fgets(pFbuf, 256, hFile)) {
		if (*fBuf.atPos(0) == '#') {
			fBuf.trimWhiteSpace();
			DealWhiteSpace(&fBuf);
			SplitCSV(fBuf, &csv, ",");
			if (fBuf.left(3).isSame("#IF")) {
				IFSWITCH[IFCOUNT + 1] = 1;
				SplitCSV(fBuf, &csv, ",");
				for (int i = 1; i < 10; i++) {
					if (csv.val[i] < 0 || csv.val[i]>999 || dst_op[csv.val[i]] == 0) {
						i = 10;
						IFSWITCH[IFCOUNT + 1] = 2;
					}
				}
				IFCOUNT++;
			}
			else if (fBuf.left(7).isSame("#ELSEIF") && IFSWITCH[IFCOUNT] != 3) {
				if (IFSWITCH[IFCOUNT] == 1) IFSWITCH[IFCOUNT] = 3;
				else {
					IFSWITCH[IFCOUNT] = 1;
					SplitCSV(fBuf, &csv, ",");
					for (int i = 1; i < 10; i++) {
						if (csv.val[i] < 0 || csv.val[i]>999 || dst_op[csv.val[i]] == 0) {
							i = 10;
							IFSWITCH[IFCOUNT] = 2;
						}
					}
				}
			}
			else if (fBuf.left(5).isSame("#ELSE") && IFSWITCH[IFCOUNT] != 3) {
				IFSWITCH[IFCOUNT] = (IFSWITCH[IFCOUNT] == 1) ? 3 : 1;
			}
			else if (fBuf.left(6).isSame("#ENDIF")) {
				IFSWITCH[IFCOUNT] = 0;
				IFCOUNT--;
			}

			if (IFCOUNT > 0 && IFSWITCH[IFCOUNT] > 1) {
				*fBuf.atPos(0) = 0;
				continue;
			}

			if (fBuf.left(13).isSame("#CUSTOMOPTION")) {
				wildCount++;
				fBuf.atPos(0);
			}
			else if (fBuf.left(11).isSame("#CUSTOMFILE")) {
				SplitCSV(fBuf, &csv, ",");
				wildStr[wildCount].assign(&csv.str[2]);
				wildCFilename[wildCount].assign(&sku.customize_filename[wildCount]);
				if (wildCFilename[wildCount].isSame("RANDOM")) {
					wildCFilename[wildCount].assign(GetRandomFile(wildStr[wildCount], 1));
				}
				wildCount++;
				*fBuf.atPos(0) = 0;
			}
			else if (fBuf.left(13).isSame("#CUSTOMFOLDER")) {
				SplitCSV(fBuf, &csv, ",");
				wildStr[wildCount].assign(&csv.str[2]);
				wildCFilename[wildCount].assign(&sku.customize_filename[wildCount]);
				wildCount++;
				*fBuf.atPos(0) = 0;
			}

			for (int i = 0; i < wildCount; i++) {
				if (wildStr[i].isSame(csv.str[1].left(wildStr[i].length())) && wildCFilename[i].isDiff("RANDOM") && wildCFilename[i].isDiff("ERROR") && wildCFilename[i].length() > 0) {
					csv.str[1].replace("*", wildCFilename[i]);
				}
			}

			if (fBuf.left(7).isSame("#SELECT") && !load_select) {
				LoadSound(&g->audio, &g->audio.sysSound.select, GetRandomFileNoError(csv.str[1], dir), 1, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.select.load) load_select = true;
			}
			else if (fBuf.left(12).isSame("#FOLDER_OPEN") && !load_folder_open) {
				LoadSound(&g->audio, &g->audio.sysSound.folder_open, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.folder_open.load) load_folder_open = true;
			}
			else if (fBuf.left(13).isSame("#FOLDER_CLOSE") && !load_folder_close) {
				LoadSound(&g->audio, &g->audio.sysSound.folder_close, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.folder_close.load) load_folder_close = true;
			}
			else if (fBuf.left(11).isSame("#PANEL_OPEN") && !load_panel_open) {
				LoadSound(&g->audio, &g->audio.sysSound.panel_open, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.panel_open.load) load_panel_open = true;
			}
			else if (fBuf.left(12).isSame("#PANEL_CLOSE") && !load_panel_close) {
				LoadSound(&g->audio, &g->audio.sysSound.panel_close, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.panel_close.load) load_panel_close = true;
			}
			else if (fBuf.left(11).isSame("#SCREENSHOT") && !load_screenshot) {
				LoadSound(&g->audio, &g->audio.sysSound.screenshot, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.screenshot.load) load_screenshot = true;
			}
			else if (fBuf.left(14).isSame("#OPTION_CHANGE") && !load_option_change) {
				LoadSound(&g->audio, &g->audio.sysSound.option_change, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.option_change.load) load_option_change = true;
			}
			else if (fBuf.left(7).isSame("#DECIDE") && !load_decide) {
				LoadSound(&g->audio, &g->audio.sysSound.decide, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.decide.load) load_decide = true;
			}
			else if (fBuf.left(6).isSame("#CLEAR") && !load_clear) {
				LoadSound(&g->audio, &g->audio.sysSound.clear, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.clear.load) load_clear = true;
			}
			else if (fBuf.left(5).isSame("#FAIL") && !load_fail) {
				LoadSound(&g->audio, &g->audio.sysSound.fail, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.fail.load) load_fail = true;
			}
			else if (fBuf.left(5).isSame("#STOP") && !load_stop) {
				LoadSound(&g->audio, &g->audio.sysSound.stop, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.stop.load) load_stop = true;
			}
			else if (fBuf.left(11).isSame("#DIFFICULTY") && !load_difficulty) {
				LoadSound(&g->audio, &g->audio.sysSound.difficulty, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.difficulty.load) load_difficulty = true;
			}
			else if (fBuf.left(5).isSame("#MINE") && !load_mine) {
				LoadSound(&g->audio, &g->audio.sysSound.mine, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.mine.load) load_mine = true;
			}
			else if (fBuf.left(8).isSame("#SCRATCH") && !load_scratch) {
				LoadSound(&g->audio, &g->audio.sysSound.scratch, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.scratch.load) load_scratch = true;
			}
			else if (fBuf.left(9).isSame("#EXSELECT") && !load_exselect) {
				LoadSound(&g->audio, &g->audio.sysSound.exselect, GetRandomFileNoError(csv.str[1], dir), 1, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.exselect.load) load_exselect = true;
			}
			else if (fBuf.left(9).isSame("#EXDECIDE") && !load_exdecide) {
				LoadSound(&g->audio, &g->audio.sysSound.exdecide, GetRandomFileNoError(csv.str[1], dir), 0, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.exdecide.load) load_exdecide = true;
			}
			else if (fBuf.left(12).isSame("#COURSECLEAR") && !load_courseclear) {
				LoadSound(&g->audio, &g->audio.sysSound.courseclear, GetRandomFileNoError(csv.str[1], dir), 1, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.courseclear.load) load_courseclear = true;
			}
			else if (fBuf.left(11).isSame("#COURSEFAIL") && !load_coursefail) {
				LoadSound(&g->audio, &g->audio.sysSound.coursefail, GetRandomFileNoError(csv.str[1], dir), 1, g->config.sound.disabledsp, 0);
				if (g->audio.sysSound.coursefail.load) load_coursefail = true;
			}
			else if (fBuf.left(8).isSame("#INCLUDE")) {
				ReadLR2SoundSet(g, GetRandomFileNoError(csv.str[1], dir), 1);
			}
		}
		pFbuf = fBuf.outstr();
	}
	fclose(hFile);
	return 1;
}

//416a10
int SetObjectValue_Bargraph(game *g) {

	float max, val;

	for (int i = 0; i < g->skstruct.otherObject[5].srcSize; i++) {

		if (GetOptionFlag_dst(g, g->skstruct.otherObject[5].dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct.otherObject[5].dst[i].opt2) && GetOptionFlag_dst(g, g->skstruct.otherObject[5].dst[i].opt3)) {

			switch (g->skstruct.otherObject[5].src[i].op1) {
				
				default:
					continue;

				case 1:
					val = GetTimeLapse(41, &g->timer1);
					max = g->gameplay.song_runtime;
					break;

				case 2:
					val = g->gameplay.loadObject_loaded;
					max = g->gameplay.loadObject_total;
					break;

				case 3:
					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 5 || g->sSelect.bmsList[g->sSelect.cur_song].keymode == 10) {
						max = 9.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].level;
					}
					else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 7 || g->sSelect.bmsList[g->sSelect.cur_song].keymode == 14) {
						max = 12.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].level;
					}
					else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 9) {
						max = 42.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].level;
					}
					else continue;
					break;

				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 5 || g->sSelect.bmsList[g->sSelect.cur_song].keymode == 10) {
						max = g->config.select.levelbarflash_5;
						val = g->sSelect.levelIndicatorAnimation[g->skstruct.otherObject[5].src[i].op1 - 5];
					}
					else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 7 || g->sSelect.bmsList[g->sSelect.cur_song].keymode == 14) {
						max = g->config.select.levelbarflash_7;
						val = g->sSelect.levelIndicatorAnimation[g->skstruct.otherObject[5].src[i].op1 - 5];
					}
					else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 9) {
						max = g->config.select.levelbarflash_9;
						val = g->sSelect.levelIndicatorAnimation[g->skstruct.otherObject[5].src[i].op1 - 5];
					}
					else continue;
					break;

				case 10:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.player[0].exscore;
					break;

				case 11:
					max = g->gameplay.player[0].totalnotes * 2;
					if (g->gameplay.player[0].note_current <= 0) continue;
					val = g->gameplay.player[0].exscore * g->gameplay.player[0].totalnotes / g->gameplay.player[0].note_current;
					break;

				case 12:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.highScore.rate;
					break;

				case 13:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.highScore.exscore;
					break;

				case 14:
					if (g->config.play.battle == 1) {
						max = g->gameplay.player[1].totalnotes * 2;
						val = g->gameplay.player[1].exscore;
					}
					else {
						max = g->gameplay.player[0].totalnotes * 2;
						val = g->gameplay.targetScore.rate;
					}
					break;

				case 15:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.targetScore.exscore;
					break;

				case 20:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[5];
					break;

				case 21:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[4];
					break;

				case 22:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[3];
					break;

				case 23:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[2];
					break;

				case 24:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[1];
					break;

				case 25:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].max_combo;
					break;

				case 26:
					if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14) {
						max = 20000.0;
						val = g->gameplay.player[0].score;
					}
					else {
						max = 10000.0;
						val = g->gameplay.player[0].score;
					}
					break;

				case 27:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.player[0].exscore;
					break;

				case 30:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[5];
					break;

				case 31:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[4];
					break;

				case 32:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[3];
					break;

				case 33:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[2];
					break;

				case 34:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[1];
					break;

				case 35:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].max_combo;
					break;

				case 36:
					if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14) {
						max = 20000.0;
						val = g->gameplay.player[1].score;
					}
					else {
						max = 10000.0;
						val = g->gameplay.player[1].score;
					}
					break;

				case 37:
					max = g->gameplay.player[1].totalnotes * 2;
					val = g->gameplay.player[1].exscore;
					break;

				case 40:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat;
					break;

				case 41:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great;
					break;

				case 42:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good;
					break;

				case 43:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad;
					break;

				case 44:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor;
					break;

				case 45:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo;
					break;

				case 46:
					if ((g->sSelect.bmsList[g->sSelect.cur_song].keymode == 7) || (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 0xe)) {
						max = 20000.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score;
					}
					else {
						max = 10000.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score;
					}
					break;

				case 47:
					max = (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes * 2);
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore;
					break;
			}

			if (val > 0) {
				if (val >= max) val = max;
				AddDrawingBuffer_BarGraph(&g->skstruct.drBuf, &g->skstruct.otherObject[5].src[i], &g->skstruct.otherObject[5].dst[i], &g->timer1, 0, max, val);
			}
		}		
	}
	return 1;
}

//417080
int SetPlayOption(game *g, sqlite3 *sql) {

	if (!(g->KeyInput.p1_buttonInput[2] == 2 || g->KeyInput.p2_buttonInput[2] == 2)) g->sSelect.fExtraCmdDone = 0;

	if (g->sSelect.fExtraCmdDone == 0 && (g->KeyInput.p1_buttonInput[2] == 2 || g->KeyInput.p2_buttonInput[2] == 2)) {
		if (GetTimeLapse(102, &g->timer1) >= 1000 || GetTimeLapse(112, &g->timer1) >= 1000) {

			LoopInRange(0, 1, 1, &g->config.play.is_extra);
			PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
			g->sSelect.fExtraCmdDone = 1;

			if (g->audio.sysSound.exselect.load) {
				if (g->config.play.is_extra) {
					StopSound(&g->audio, &g->audio.sysSound.select);
					PlaySound(&g->audio, &g->audio.sysSound.exselect, g->audio.chnBgm, -1);
				}
				else {
					StopSound(&g->audio, &g->audio.sysSound.exselect);
					PlaySound(&g->audio, &g->audio.sysSound.select, g->audio.chnBgm, -1);
				}
			}
		}
	}
	if (g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[1] == 1) {
		g->sSelect.filter_clicked = 2;
		SetBmsFilter(g, sql);
		SetObjectStrings_SongSelect(g);
	}
	if (g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[2] == 1) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		int k = g->config.select.key;
		if (k == 4 || k == 5 || k == 6 || (g->config.play.battle && k != 4)) {
			if (g->KeyInput.p1_buttonInput[2] == 1) {
				LoopInRange(0, 5, 1, &g->config.play.random[0]);
			}
			else if (g->KeyInput.p2_buttonInput[2] == 1) {
				LoopInRange(0, 5, 1, &g->config.play.random[1]);
			}
		}
		else {
			LoopInRange(0, 5, 1, &g->config.play.random[0]);
		}
		SetObjectStrings_SongSelect(g);
	}
	if (g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[31] == 1) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		int k = g->config.select.key;
		if (k == 4 || k == 5 || k == 6) {
			LoopInRange(0, 1, 1, &g->config.play.dpflip);
		}
		else {
			LoopInRange(0, 4, 1, &g->config.play.battle);
		}
		SetObjectStrings_SongSelect(g);
	}
	if (g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[4] == 1) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		if (g->config.play.battle == 1 && g->KeyInput.p1_buttonInput[4] != 1) {
			LoopInRange(0, 5, 1, &g->config.play.gaugeOption[1]);
		}
		else {
			LoopInRange(0, 5, 1, &g->config.play.gaugeOption[0]);
		}
		SetObjectStrings_SongSelect(g);
	}
	if ((g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[6] == 1) && (g->KeyInput.p1_buttonInput[7] == 0 && g->KeyInput.p2_buttonInput[7] == 0)){
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		int k = g->config.select.key;
		if (k == 4 || k == 5 || k == 6 || (g->config.play.battle && k != 4)) {
			if (g->KeyInput.p1_buttonInput[6] == 1) {
				LoopInRange(0, 1, 1, &g->config.play.p1_assist);
			}
			else if (g->KeyInput.p2_buttonInput[6] == 1) {
				LoopInRange(0, 1, 1, &g->config.play.p2_assist);
			}
		}
		else {
			LoopInRange(0, 1, 1, &g->config.play.p1_assist);
			g->config.play.p2_assist = g->config.play.p1_assist;
		}
		SetObjectStrings_SongSelect(g);

	}
	if ((g->KeyInput.p1_buttonInput[6] == 1 && g->KeyInput.p1_buttonInput[7] == 2) || (g->KeyInput.p1_buttonInput[7] == 1 && g->KeyInput.p1_buttonInput[6] == 2)) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		LoopInRange(0, 3, -1, &g->config.play.m_HIDSUD1);
		SetObjectStrings_SongSelect(g);
		g->config.play.m_HIDSUD2 = g->config.play.m_HIDSUD1;

	}
	if ((g->KeyInput.p2_buttonInput[6] == 1 && g->KeyInput.p2_buttonInput[7] == 2) || (g->KeyInput.p2_buttonInput[7] == 1 && g->KeyInput.p2_buttonInput[6] == 2)) {
		if (g->config.play.battle == 1) {
			PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
			LoopInRange(0, 3, 1, &g->config.play.m_HIDSUD2);
			SetObjectStrings_SongSelect(g);
		}
		else {
			PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
			LoopInRange(0, 3, 1, &g->config.play.m_HIDSUD1);
			SetObjectStrings_SongSelect(g);
			g->config.play.m_HIDSUD2 = g->config.play.m_HIDSUD1;
		}
	}
	if (g->KeyInput.p1_buttonInput[5] == 1 && g->KeyInput.p1_buttonInput[7] == 0 && g->KeyInput.p1_buttonInput[6] == 0) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		LoopInRange(g->config.play.hsmin, g->config.play.hsmax, -g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
		SetObjectStrings_SongSelect(g);
	}
	if (g->KeyInput.p1_buttonInput[7] == 1 && g->KeyInput.p1_buttonInput[5] == 0 && g->KeyInput.p1_buttonInput[6] == 0) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		LoopInRange(g->config.play.hsmin, g->config.play.hsmax, g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
		SetObjectStrings_SongSelect(g);
	}
	if (g->KeyInput.p2_buttonInput[5] == 1 && g->KeyInput.p2_buttonInput[7] == 0 && g->KeyInput.p2_buttonInput[6] == 0) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		if (g->config.play.battle == 1) {
			LoopInRange(g->config.play.hsmin, g->config.play.hsmax, -g->config.play.hsmargin, &g->config.play.hiSpeed[1]);
		}
		else {
			LoopInRange(g->config.play.hsmin, g->config.play.hsmax, -g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
		}
		SetObjectStrings_SongSelect(g);
	}
	if (g->KeyInput.p2_buttonInput[7] == 1 && g->KeyInput.p2_buttonInput[5] == 0 && g->KeyInput.p2_buttonInput[6] == 0) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		if (g->config.play.battle == 1) {
			LoopInRange(g->config.play.hsmin, g->config.play.hsmax, g->config.play.hsmargin, &g->config.play.hiSpeed[1]);
		}
		else {
			LoopInRange(g->config.play.hsmin, g->config.play.hsmax, g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
		}
		SetObjectStrings_SongSelect(g);
	}
	if ((g->KeyInput.p1_buttonInput[5] == 1 && g->KeyInput.p1_buttonInput[7] == 2) || (g->KeyInput.p2_buttonInput[5] == 1 && g->KeyInput.p2_buttonInput[7] == 2)) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		LoopInRange(0, 4, 1, &g->config.play.hsfix);
		SetObjectStrings_SongSelect(g);
	}
	if ((g->KeyInput.p1_buttonInput[5] == 2 && g->KeyInput.p1_buttonInput[7] == 1) || (g->KeyInput.p2_buttonInput[5] == 2 && g->KeyInput.p2_buttonInput[7] == 1)) {
		if (g->config.play.battle == 1) {
			PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
			LoopInRange(0, 4, -1, &g->config.play.hsfix);
		}
		else {
			LoopInRange(0, 4, 1, &g->config.play.hsfix);
		}
		SetObjectStrings_SongSelect(g);
	}
	if (g->KeyInput.p1_buttonInput[13] == 1 || g->KeyInput.p2_buttonInput[13] == 1 || g->KeyInput.p1_buttonInput[8] == 1 || g->KeyInput.inputID[D_DIK_RIGHT] == 1) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		LoopInRange(0, 8, 1, &g->config.play.p1_target);
		SetTarget(g);
	}
	else if (g->KeyInput.inputID[D_DIK_LEFT] == 1) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		LoopInRange(0, 8, -1, &g->config.play.p1_target);
		SetTarget(g);
	}

	return 1;
}

//417ce0
void CheckNewSong(glb_dbgame *glb) {
	CONFIG_JUKEBOX jb;
	sqlite3_stmt *pStmt;
	char buf[1024];
	int filDiff, filKey;
	int err = 0;

	DB_EnterCriticalSection();
	glb->pGame->sSelect.searchFocused = 2;
	filDiff = glb->pGame->sSelect.filterDifficulty;
	filKey = glb->pGame->sSelect.filterKey;
	jb.numOfPath = 0;
	if (glb->pGame->sSelect.reloadType >= 1) {
		ErrorLogAdd("曲の更新チェックを行います。\n");
		err = (ReloadSongsByQuery(glb->pGame->sSelect.unk4fa4[0], glb->pSql, &jb) == 2);
		ErrorLogAdd("フォルダの更新チェックを行います。\n");
		err = err || (ReloadSongsByQuery(glb->pGame->sSelect.unk4fa4[1], glb->pSql, &jb) == 2);
		if (glb->pGame->sSelect.reloadType >= 2) {
			ErrorLogAdd("フォルダの更新チェックを行います。\n");
			err = err || (ReloadSongsByQuery(glb->pGame->sSelect.unk4fa4[2], glb->pSql, &jb) == 2);
			if (glb->pGame->sSelect.reloadType >= 3) {
				ErrorLogAdd("曲の更新チェックを行います。\n");
				err = err || (ReloadSongsByQuery(glb->pGame->sSelect.unk4fa4[3], glb->pSql, &jb) == 2);
			}
		}
		if (err) {
			ErrorLogAdd("未設定の#DIFFICULTYを設定します。\n");
			SetUndefinedDifficulty(glb->pSql);
			sqlite3_exec(glb->pSql, "DELETE FROM folder WHERE path=\'LR2files\\CustomFolder\\newsong.lr2folder\'", 0, 0, 0);
			sqlite3_snprintf(1024, buf, "SELECT * FROM song WHERE adddate > %d", GetNowUnixtime() - jb.titleflash * 3600);
			sqlite3_prepare(glb->pSql, buf, -1, &pStmt, NULL);
			if (sqlite3_step(pStmt) == 100) {
				jb.path[jb.numOfPath] = "LR2files\\CustomFolder\\newsong.lr2folder";
				GetFolderDataFromPath(jb.path[jb.numOfPath], glb->pSql);
			}
			sqlite3_finalize(pStmt);
		}
	}
	glb->pGame->sSelect.filterDifficulty = filDiff;
	glb->pGame->sSelect.filterKey = filKey;
	glb->pGame->sSelect.isFolder = 0;
	for (int i = 0; i < glb->pGame->sSelect.queryCount; i++) {
		int flag;
		if (glb->pGame->sSelect.curQuery[i].findStrPos("__EXPERT__") > 0) {
			flag = SearchCourseFromDB(glb->pSql, &glb->pGame->sSelect, glb->pGame->sSelect.filterKey, 0);
		}
		else if (glb->pGame->sSelect.curQuery[i].findStrPos("__NONSTOP__") > 0) {
			flag = SearchCourseFromDB(glb->pSql, &glb->pGame->sSelect, glb->pGame->sSelect.filterKey, 1);
		}
		else if (glb->pGame->sSelect.curQuery[i].findStrPos("__GRADE__") > 0) {
			flag = SearchCourseFromDB(glb->pSql, &glb->pGame->sSelect, glb->pGame->sSelect.filterKey, 2);
		}
		else if (glb->pGame->sSelect.curQuery[i].findStrPos("FROM song") > 0) {
			flag = LoadFilteredBmsListFromDB(glb->pGame->sSelect.curQuery[i], glb->pSql, &glb->pGame->sSelect, &glb->pGame->sSelect.filterDifficulty, &glb->pGame->sSelect.filterKey, glb->pGame->sSelect.filterSort, glb->pGame->sSelect.searchMax, glb->pGame->sSelect.unk4fc4[i]);
		}
		else {
			flag = LoadBmsListFromDB(glb->pGame->sSelect.curQuery[i], glb->pSql, &glb->pGame->sSelect, &glb->pGame->sSelect.filterDifficulty, &glb->pGame->sSelect.filterKey, glb->pGame->sSelect.filterSort, glb->pGame->sSelect.searchMax);
		}

		if(flag != -1) break;
		glb->pGame->sSelect.isFolder = i + 1;
	}

	if (glb->pGame->sSelect.isFolder != -1 && glb->pGame->config.skin.disableimagefont == 0) {
		LoadFontForSongs(glb->pGame, 1);
	}
	glb->pGame->sSelect.searchFocused = 4;
	(glb->pGame->sSelect).unk4fc4[0] = '\0';
	(glb->pGame->sSelect).unk4fc4[1] = '\0';
	(glb->pGame->sSelect).unk4fc4[2] = '\0';
	DB_LeaveCriticalSection();

	return;
}

//4181b0
void ThreadProc_RankingAutoUpdate(void *param) {
	game *g = (game*)param;

	CSTR hash = g->sSelect.bmsList[g->sSelect.cur_song].hash;
	g->net.rankingData.Init();
	g->net.rankingData.rivalID = 0;
	g->net.rankingData.rivalRanking = 0;

	if (g->sSelect.stack_query[g->sSelect.cur].findStrPos("__RIVAL__") >= 0) {
		g->net.rankingData.rivalID = g->sSelect.stack_rivalID[g->sSelect.cur];
	}
	g->sSelect.isRankingAutoUpdateThread = 1;
	CSTR path;
	if (hash.length() < 50) cstrSprintf(&path, "LR2files/Ir/%s.xml", hash);
	else cstrSprintf(&path, "LR2files/Ir/%s.xml", hash.makeCRCstr());
	
	if (path.canOpenFile()) {
		if (hash.isSame(g->sSelect.bmsList[g->sSelect.cur_song].hash)) {
			g->net.Lock();
			g->net.rankingData.ParseXML(path);
			g->net.Unlock();
		}

		if (g->net.rankingData.rankingCount > 0) {
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = g->net.rankingData.myRanking;
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = g->net.rankingData.rankingCount;
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate = (g->net.rankingData.clearPlayers[2] + g->net.rankingData.clearPlayers[3] + g->net.rankingData.clearPlayers[4] + g->net.rankingData.clearPlayers[5]) * 100 / g->net.rankingData.rankingCount;
			g->net.IRstatus = 2;
			SetObjectStrings_SongSelect(g);
			if (g->sSelect.stack_query[g->sSelect.cur].findStrPos("__RIVAL__") >= 0) {
				g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.IRranking = g->net.rankingData.rivalRanking;
				g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.IRplayercount = g->net.rankingData.rankingCount;
				g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.IRclearRate = (g->net.rankingData.clearPlayers[3]*2 + g->net.rankingData.clearPlayers[4] + g->net.rankingData.clearPlayers[5]) * 100 / g->net.rankingData.rankingCount; // TOFIX:
			}
		}
		else {
			g->net.IRstatus = -1;
			SetObjectStrings_SongSelect(g);
		}

		if (GetNowUnixtime() - GetFileUnixtime(path) < 86400 || g->config.network.autoupdate == 0) {
			g->net.hHandle = NULL;
			g->sSelect.isRankingAutoUpdateThread = 0;
			return;
		}
	}

	bool isIR2 = g->net.IRstatus == 2;

	if (!g->config.network.autoupdate) {
		if (!isIR2) g->net.IRstatus = 0;
		g->net.hHandle = NULL;
		g->sSelect.isRankingAutoUpdateThread = 0;
		return;
	}

	if (g->net.rankUpdateDelayLevel >= 3) {
		if (!isIR2) g->net.IRstatus = 5;
		g->net.hHandle = NULL;
		g->sSelect.isRankingAutoUpdateThread = 0;
		return;
	}

	SetTimeLapse(177, &g->timer1);
	g->net.IRstatus = 3;
	while (GetTimeLapse(177, &g->timer1) <g->net.waitTime) {
		Sleep(4);
		if (g->net.waitForHandle || (isIR2 && (g->KeyInput.p1_buttonInput[4] == 2 || g->KeyInput.p2_buttonInput[4] == 2))) {
			g->sSelect.isRankingAutoUpdateThread = 0;
			g->net.hHandle = NULL;
			ResetTimeLapse(177, &g->timer1);
			g->net.IRstatus = isIR2 ? 2 : 0;
			return;
		}
	}

	ResetTimeLapse(177, &g->timer1);
	g->net.IRstatus = 4;
	ErrorLogAdd("接続を開始します\n");

	if (hash.isSame(g->sSelect.bmsList[g->sSelect.cur_song].hash) == 0) {
		ErrorLogAdd("IRスレッド強制終了\n");
		g->net.IRstatus = -3;
		ErrorLogAdd("IRスレッド強制終了\n");
		g->net.IRstatus = -3;
	}
	else {
		int success = g->net.GetRanking(hash, 0);
		if (success == -1) {
			ErrorLogAdd("IRスレッド強制終了\n");
			g->net.IRstatus = -3;
		}
		else if (success == 0) {
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRthreadEnd = 1;
			ErrorLogAdd("IRスレッド異常終了\n");
			g->net.IRstatus = -3;
		}
		else {
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRthreadEnd = 1;
			ErrorLogAdd("IRスレッド正常終了\n");
			if (g->net.rankingData.rankingCount <= 0) {
				g->net.IRstatus = -1;
			}
			else {
				g->net.IRstatus = 2;
				g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = g->net.rankingData.myRanking;
				g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = g->net.rankingData.rankingCount;
				g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate = (g->net.rankingData.clearPlayers[2] + g->net.rankingData.clearPlayers[3] + g->net.rankingData.clearPlayers[4] + g->net.rankingData.clearPlayers[5]) * 100 / g->net.rankingData.rankingCount;

				if (g->sSelect.stack_query[g->sSelect.cur].findStrPos("__RIVAL__") > 0) {
					g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.IRranking = g->net.rankingData.rivalRanking;
					g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.IRplayercount = g->net.rankingData.rankingCount;
					g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.IRclearRate = (g->net.rankingData.clearPlayers[2] + g->net.rankingData.clearPlayers[3] + g->net.rankingData.clearPlayers[4] + g->net.rankingData.clearPlayers[5]) * 100 / g->net.rankingData.rankingCount;
				}
			}
		}
	}

	SetObjectStrings_SongSelect(g);
	g->net.hHandle = NULL;
	g->sSelect.isRankingAutoUpdateThread = 0;
	return;
}

//418820
int JudgeToScore(int judge, game *g, int player, int lane, char isReplay) {
	return ApplyJudgeToScore(judge, g, player, lane, &g->timer1, isReplay);
}

//418850
int ProcSinglenote(game *g, int lane, int keypress, int timing, int player) {
	NoteStruct &note = g->gameplay.bmsobj_note[lane].notes[g->gameplay.bmsobj_note[lane].note_count];

	if (note.channel > 0) {
		if (keypress == 1 && note.realTiming - (double)timing < (double)g->gameplay.player[player].judgetime[4]) {
			ApplyJudge(0, g, player, lane, note.channel);

			if (g->gameplay.keysound[0].load)
				PlaySound(&g->audio, &g->gameplay.keysound[0], g->audio.chnKey, -1);
			else
				PlaySound(&g->audio, &g->audio.sysSound.mine, g->audio.chnKey, -1);
		}
		else if ((double)timing <= note.realTiming)
			return 0;
		else if (keypress == 1 || keypress == 2) {
			ApplyJudge(0, g, player, lane, note.channel);

			if (g->gameplay.keysound[0].load)
				PlaySound(&g->audio, &g->gameplay.keysound[0], g->audio.chnKey, -1);
			else
				PlaySound(&g->audio, &g->audio.sysSound.mine, g->audio.chnKey, -1);
		}

		note.active = -1;
		g->gameplay.bmsobj_note[lane].note_count++;
		return 0;
	}
	else {
		//passed note
		if ((double)timing - note.realTiming > (double)g->gameplay.player[player].judgetime[2] && g->gameplay.player[player].note_current < g->gameplay.player[player].totalnotes) {
			ApplyJudgeToScore(1, g, player, lane, &g->timer1, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			note.active = -1;
			g->gameplay.bmsobj_note[lane].note_count++;
			return 0;
		}
		if (keypress != 1) return 0;

		int gap = abs(timing - (int)note.realTiming);

		if (gap <= g->gameplay.player[player].judgetime[5] && g->gameplay.player[player].note_current < g->gameplay.player[player].totalnotes) {
			g->gameplay.autojudge_midcount++;
			g->gameplay.autojudge_midsum += timing - (int)note.realTiming;
			JudgeToScore(5, g, player, lane, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = -1;
			g->gameplay.bmsobj_note[lane].note_count++;
			SetTimeLapse(50 + lane, &g->timer1);
			return 1;
		}
		if (gap <= g->gameplay.player[player].judgetime[4] && g->gameplay.player[player].note_current < g->gameplay.player[player].totalnotes) {
			g->gameplay.autojudge_midcount++;
			g->gameplay.autojudge_midsum += timing - (int)note.realTiming;
			JudgeToScore(4, g, player, lane, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = -1;
			g->gameplay.bmsobj_note[lane].note_count++;
			SetTimeLapse(50 + lane, &g->timer1);
			return 1;
		}
		if (gap <= g->gameplay.player[player].judgetime[3] && g->gameplay.player[player].note_current < g->gameplay.player[player].totalnotes) {
			g->gameplay.autojudge_midcount++;
			g->gameplay.autojudge_midsum += timing - (int)note.realTiming;
			JudgeToScore(3, g, player, lane, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = -1;
			g->gameplay.bmsobj_note[lane].note_count++;
			
			return 1;
		}


		if (gap <= g->gameplay.player[player].judgetime[2] && g->gameplay.player[player].note_current < g->gameplay.player[player].totalnotes) {
			g->gameplay.autojudge_midcount++;
			g->gameplay.autojudge_midsum += timing - (int)note.realTiming;
			JudgeToScore(2, g, player, lane, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = -1;
			g->gameplay.bmsobj_note[lane].note_count++;
			
			if (g->gameplay.bmsobj_note[lane].note_count < g->gameplay.bmsobj_note[lane].count && abs(timing - (int)g->gameplay.bmsobj_note[lane].notes[g->gameplay.bmsobj_note[lane].note_count].realTiming) <= g->gameplay.player[player].judgetime[2]) {
				ProcSinglenote(g, lane, 1, timing, player);
				return 1;
			}
			return 1;
		}
		if ((int)note.realTiming - timing < g->gameplay.player[player].judgetime[1] && g->gameplay.player[player].note_current < g->gameplay.player[player].totalnotes) {
			JudgeToScore(0, g, player, lane, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			return 1;
		}

		if(0 < g->gameplay.bmsobj_note[lane].noteVal && g->gameplay.bmsobj_note[lane].noteVal < 6480)
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
		return 1;
	}
}

//418e40
int ProcLongnote(game *g, int lane, int keypress, int timing, int player) {
	
	NoteStruct &note = g->gameplay.bmsobj_note[lane].notes[g->gameplay.bmsobj_note[lane].note_count];

	if ((double)timing - note.realTiming > (double)g->gameplay.player[player].judgetime[3] && note.active <= 0) {
		ApplyJudgeToScore(1, g, player, lane, &g->timer1, 0);
		g->gameplay.bmsobj_note[lane].noteVal = note.val;
		note.active = -1;
		g->gameplay.bmsobj_note[lane].note_count++;
		ResetTimeLapse(70 + lane, &g->timer1);
		return 0;
	}

	if (keypress == 1) {
		int gap = abs(timing - (int)note.realTiming);

		if (gap <= g->gameplay.player[player].judgetime[5]) {
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = 5;

			SetTimeLapse(70 + lane, &g->timer1);
			return 1;
		}
		else if (gap <= g->gameplay.player[player].judgetime[4]) {
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = 4;

			SetTimeLapse(70 + lane, &g->timer1);
			return 1;
		}
		else if (gap <= g->gameplay.player[player].judgetime[3]) {
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = 3;

			SetTimeLapse(70 + lane, &g->timer1);
			return 1;
		}
		else if (gap <= g->gameplay.player[player].judgetime[2]) {
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = 2;
			return 1;
		}
		else if ((int)note.realTiming - timing < g->gameplay.player[player].judgetime[1]) {
			JudgeToScore(0, g, player, lane, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			return 1;
		}

		if (0 < g->gameplay.bmsobj_note[lane].noteVal && g->gameplay.bmsobj_note[lane].noteVal < 6480)
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);

		return 1;
	}

	else if (keypress == 2) {
		if ((int)note.realTiming_ln < timing && note.active > 0) {
			if (note.active < 4) {
				ResetTimeLapse(70 + lane, &g->timer1);
			}
			else {
				ResetTimeLapse(70 + lane, &g->timer1);
				SetTimeLapse(50 + lane, &g->timer1);
			}
			JudgeToScore(note.active, g, player, lane, 0);
			note.active = -1;
			g->gameplay.bmsobj_note[lane].note_count++;
			return 1;
		}
	}
	else if (keypress == 3 && note.active > 0) {
		if (g->gameplay.player[player].judgetime[3] + timing < (int)note.realTiming_ln) {
			StopSound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal]);
			ResetTimeLapse(70 + lane, &g->timer1);
			JudgeToScore(2, g, player, lane, 0);
		}
		else {
			ResetTimeLapse(70 + lane, &g->timer1);
			JudgeToScore(note.active, g, player, lane, 0);
		}
		note.active = -1;
		g->gameplay.bmsobj_note[lane].note_count++;
	}

	return 1;
}

//419320
int ProcNoteOnTiming(game *g, int lane, int keypress, int timing, int player) {

	NoteStruct &note = g->gameplay.bmsobj_note[lane].notes[g->gameplay.bmsobj_note[lane].note_count];

	if (!(0 <= note.stage && note.stage < 5)) note.stage = 0;
	
	if (g->gameplay.bmsobj_note[lane].size == 0 || g->gameplay.bmsobj_note[lane].autoplay != 0) return 0;

	if (g->gameplay.bmsobj_note[lane].note_count == g->gameplay.bmsobj_note[lane].size && g->gameplay.bmsobj_note[lane].noteVal > 0) {
		if (keypress == 1) {
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
		}
		return 0;
	}

	if (note.realTiming < note.realTiming_ln) 
		ProcLongnote(g, lane, keypress, timing, player);
	else 
		ProcSinglenote(g, lane, keypress, timing, player);

	return 1;
}

//419440
int CountCallLoadPreview = 0;
void LoadPreview(game *g) {
	BMSMETA meta;
	
	CountCallLoadPreview++;
	if (!IsFileExist(g->gameplay.previewBMSfilepath)) {
		g->gameplay.isPreviewLoad = 0;
		g->gameplay.previewStatus = 0;
		g->gameplay.hThreadPreview = 0;
		return;
	}

	meta = g->sSelect.metaSelected;

	g->gameplay.bmsResourceLoaded = 0;
	g->gameplay.flag_closingPhase = '\0';
	g->gameplay.flag_gameinput = '\0';
	g->gameplay.isAutoplay = 1;
	g->gameplay.courseType = -1;
	g->gameplay.is2Pplay = 0;
	g->gameplay.courseStageCount = 0;
	StopAllKeysound(g);
	InitKeysound(g);
	InitGameplay(&g->gameplay, &g->config.play);

	g->gameplay.isPreviewLoad = 1;
	ParseBmsFile(&g->gameplay, g->gameplay.previewBMSfilepath, &g->audio, &g->config, &meta, 0, 0);
	if (g->gameplay.flag_closingPhase == 0 && g->procSelecter == 2 && g->gameplay.previewStatus == 1) {
		LoadBmsResource(&g->gameplay, g->gameplay.previewBMSfilepath, &g->audio, &g->config, &meta, 0, g->skstruct.flag_flip, 1);
	}

	if (g->gameplay.flag_closingPhase == 0 && g->procSelecter == 2 && g->gameplay.previewStatus == 1) {
		g->gameplay.bmsobj.note_count = 0;
		SetTimeLapse(41, &g->timer1);
		SetTimeLapse(142, &g->timer1);
		g->gameplay.bmsResourceLoaded = 1;
		g->gameplay.flag_gameinput = 1;
		SetFadePreview(&g->audio, 1000, 1);
		g->gameplay.previewStatus = 2;
	}
	else {
		g->gameplay.previewStatus = 0;
		g->gameplay.isPreviewLoad = 0;
	}

	g->gameplay.hThreadPreview = 0;
	return;
}

//419650
int ProcI_Play(game *g) {
	int timeLimit;
	double gameTime;

	DrawHPgauge(g);
	SoundGetCurrentTime(&g->audio, &g->gameplay.muon);
	NONE_004b6770();

	if (GetTimeLapse(41, &g->timer1) > 0.0 && g->config.play.m_lunaris == 0) {
		DrawNotes(g, &g->skstruct, &g->timer1, &g->config.play);
		DrawJudgeCombo(g, &g->skstruct, &g->timer1, &g->config.play);
	}
	else if (GetTimeLapse(41, &g->timer1) > 0.0 && g->config.play.m_lunaris) {
		DrawLunaris(g);
		DrawJudgeCombo(g, &g->skstruct, &g->timer1, &g->config.play);
	}
	
	if (g->skstruct.dst_JUDGELINE[0].dstCount > 0) {
		AddDrawingBuffer_PlayArea(&g->skstruct.drBuf, &g->skstruct.src_JUDGELINE[0], &g->skstruct.dst_JUDGELINE[0], &g->timer1,
			(float)g->skstruct.adjust.note_1p_x + g->gameplay.nabeatsu_x, (float)g->skstruct.adjust.note_1p_y + g->gameplay.nabeatsu_y, -1,
			(float)g->skstruct.adjust.size_x, (float)g->skstruct.adjust.size_y, 0);
	}
	if (g->skstruct.dst_JUDGELINE[1].dstCount > 0) {
		AddDrawingBuffer_PlayArea(&g->skstruct.drBuf, &g->skstruct.src_JUDGELINE[1], &g->skstruct.dst_JUDGELINE[1], &g->timer1,
			(float)g->skstruct.adjust.note_2p_x + g->gameplay.nabeatsu_x, (float)g->skstruct.adjust.note_2p_y + g->gameplay.nabeatsu_y, -1,
			(float)g->skstruct.adjust.size_x, (float)g->skstruct.adjust.size_y, 0);
	}

	if( ((g->KeyInput.inputID[D_DIK_ESCAPE] == 2 || (g->KeyInput.mouse_buttonR == 2 && g->config.play.disableleftclickexit == 0) ) 
			|| (g->KeyInput.p1_buttonInput[13] == 2 && g->KeyInput.p1_buttonInput[12] == 2)
			|| (g->KeyInput.p2_buttonInput[13] == 2 || g->KeyInput.p2_buttonInput[12] == 2)
			|| (g->gameplay.player[0].totalnotes <= g->gameplay.player[0].note_current && (g->KeyInput.p1_buttonInput[13] == 2 || g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2 || g->KeyInput.p2_buttonInput[12] == 2))) 
		&& g->procPhase == 1){
		SetTimeLapse(2, &g->timer1);
		g->procPhase = 2;
		g->gameplay.flag_closingPhase = 1;
		return 1;
	}

	if (g->procPhase == 2) {
		timeLimit = g->skstruct.fadeout;
		gameTime = GetTimeLapse(2, &g->timer1);
		if (timeLimit < gameTime || timeLimit == 0) g->procSelecter = 5;
	}
	else if (g->procPhase == 3) {
		timeLimit = g->skstruct.close;
		gameTime = GetTimeLapse(3, &g->timer1);
		if (timeLimit < gameTime || timeLimit == 0) g->procSelecter = 5;
	}
	return 1;
}

//4198d0
int SkinSelect_SoundSet(game *g, CSTR filepath) {
	
	FILE *pFile;

	CSTR fBuf = CSTR(1024);
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

	if ((g->KeyInput.inputID[D_DIK_ESCAPE] == 1 || g->KeyInput.mouse_buttonR == 1) && g->procPhase == 1) {
		SetTimeLapse(2, &g->timer1);
		g->procPhase = 2;
	}
	else if (g->procPhase == 2) {
		if (GetTimeLapse(2, &g->timer1) > g->skstruct.fadeout|| g->skstruct.fadeout) {
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
		AddDrawingBuffer_ObjectAlpha(&g->skstruct2.drBuf, &g->skstruct2.src_MOUSECURSOR, &g->skstruct2.dst_MOUSECURSOR, &g->timer2, g->KeyInput.mouse_oldX, g->KeyInput.mouse_oldY, (g->KeyInput.mouse_recentMoveTime - GetTimeWrap() + 6000) * 0xff / 500); //ff:alpha 255 TOFIX:fadeout doesn't working, change 6000 to 5500
	}

	if (g->skinData.select <= 4 || g->skinData.select == 12 || g->skinData.select == 13) {
		for (int i = 0; i < 2; i++) {
			if (g->gameplay.is2Pplay == 0 && (g->config.play.gaugeOption[i] == 0 || g->config.play.gaugeOption[i] == 3)) 
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


//41b140
int CheckMission(game *g){
	double dVar1;
	int iVar2;
	int level;
	bool bVar3;
	bool bVar4;
	int gauge;

	if (g->config.play.battle) 
		return 0;

	if (g->config.play.p1_assist == 1) 
		return 0;
	if (g->config.play.p1_assist == 2) 
		return 0;

	gauge = g->config.play.gaugeOption[0];
	if (g->config.play.gaugeOption[0] == 3) 
		return 0;
	if (g->config.play.gaugeOption[1] == 3) 
		return 0;

	if (g->gameplay.isNosave) 
		return 0;

	if (g->gameplay.playerstat.trial <= 0) 
		g->gameplay.playerstat.trial = 1;

	level = g->gameplay.playerstat.trial;
	switch (level) {
		case 1:
			if (g->gameplay.player[0].totalnotes >= 100 && gauge == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 2:
			if ( (g->config.play.m_HIDSUD1 == g->config.play.m_HIDSUD2 || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) 
				&& g->gameplay.player[0].totalnotes >= 100 && g->config.play.m_HIDSUD1 == 1) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 3:
			if ((g->config.play.m_HIDSUD1 == g->config.play.m_HIDSUD2 || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10)
				&& g->gameplay.player[0].totalnotes >= 100 && g->config.play.m_HIDSUD1 == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 4:
			if (gauge == 0 && 80.0 <= g->gameplay.player[0].HP && g->gameplay.player[0].HP < 86.0) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 5:
			if (g->config.play.random[0] != g->config.play.random[1]) {
				if (9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) {
					level = g->gameplay.playerstat.trial;
					break;
				}
				level = g->gameplay.playerstat.trial;
			}
			if (g->gameplay.player[0].totalnotes >= 100 && gauge == 1 && g->config.play.random[0] == 3) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 6: //TOFIX: omitted note count condition
			if (g->audio.param.pitch_on == 1 && (g->audio.param.pitch_type == 0 || g->audio.param.pitch_type == 2)) {
				if (g->audio.param.pitch_amount >= 3) {
					g->gameplay.playerstat.trial = level + 1;
				}
			}
			break;
		case 7:
			gauge = g->config.play.random[0];
			if ((g->config.play.random[0] == g->config.play.random[1] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) 
				&& g->gameplay.player[0].totalnotes >= 100 && g->config.play.random[0] == 5) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 8:
			gauge = g->config.play.m_HIDSUD1;
			if ((g->config.play.m_HIDSUD1 == g->config.play.m_HIDSUD2 || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) 
				&& g->gameplay.player[0].totalnotes >= 100 && g->config.play.m_HIDSUD1 == 3) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 9:
			if (g->gameplay.player[0].exscore >= (g->gameplay.player[0].totalnotes * 2) / 9) { //TOFIX : is it AAA? 8/9 //TODO: check original in debugger
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 10:
			if (g->gameplay.playerstat.combo >= 2000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 11:
			if (g->gameplay.player[0].totalnotes >= 500 && gauge == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 12:
			if ((g->config.play.m_HIDSUD1 == g->config.play.m_HIDSUD2 || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) 
				&& 3 < g->gameplay.player[0].clearType && g->gameplay.player[0].totalnotes >= 500 && gauge == 1 && g->config.play.m_HIDSUD1 == 1) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 13:
			if ((g->config.play.m_HIDSUD1 == g->config.play.m_HIDSUD2 || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10)
				&& 3 < g->gameplay.player[0].clearType && g->gameplay.player[0].totalnotes >= 500 && gauge == 1 && g->config.play.m_HIDSUD1 == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 14:
			if ((g->config.play.random[0] == g->config.play.random[1] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10)
				&& g->gameplay.player[0].totalnotes >= 500 && g->config.play.random[0] == 5) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 15:
			if (g->gameplay.isSpeedChanged == false && g->config.play.hiSpeed[0] == 50 &&
				g->config.play.hsfix == 4 && g->gameplay.player[0].totalnotes >= 500) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 16:
			if (g->config.play.random[0] != g->config.play.random[1] && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) break;

			if (g->config.play.m_HIDSUD1 != g->config.play.m_HIDSUD2) {
				if (9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) {
					level = g->gameplay.playerstat.trial;
					break;
				}
				level = g->gameplay.playerstat.trial;
			}
			if (g->gameplay.player[0].totalnotes >= 500 && g->config.play.random[0] == 2 && g->config.play.m_HIDSUD1 == 3) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 17:
			if (g->gameplay.player[0].max_combo == 333) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 18:
			if (gauge == 4 && g->gameplay.player[0].totalnotes >= 100) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 19:
			if (gauge != 5) break;
			if (g->gameplay.player[0].totalnotes >= 100) { //TOFIX : note condition is omitted in string
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 20:
			if (gauge == 0 && g->gameplay.player[0].HP >= 80.0 && g->gameplay.player[0].HP < 82.0) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 21:
			if (gauge == 1 && g->gameplay.player[0].HP < 4.0) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 22:
			if ((g->config.play.random[0] != g->config.play.random[1] && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) || gauge != 2) break;
			if (g->config.play.random[0] == 3) {
				if (g->gameplay.player[0].totalnotes >= 1000) {
					g->gameplay.playerstat.trial = level + 1;
				}
			}
			break;
		case 23:
			if ((g->config.play.m_HIDSUD1 != g->config.play.m_HIDSUD2 && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode)
				|| g->config.play.m_HIDSUD1 != 1 || g->config.play.hsfix != 4 || g->config.play.hiSpeed[0] != 150)
				break;
			if (g->gameplay.player[0].totalnotes >= 1000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 24:
			if ((g->config.play.m_HIDSUD1 != g->config.play.m_HIDSUD2 && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode)
				|| g->config.play.m_HIDSUD1 != 2 || g->config.play.hsfix != 4) break;
			if (g->config.play.hiSpeed[0] == 250) {
				if (g->gameplay.player[0].totalnotes >= 1000) {
					g->gameplay.playerstat.trial = level + 1;
				}
			}
			break;
		case 25:
			if (g->config.play.random[0] != g->config.play.random[1] && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) break;
			if (g->config.play.random[0] == 5) {
				if (g->gameplay.player[0].totalnotes >= 1000) {
					g->gameplay.playerstat.trial = level + 1;
				}
			}
			break;
		case 26:
			if (g->config.play.m_HIDSUD1 != g->config.play.m_HIDSUD2 && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) break;
			if (g->config.play.random[0] != g->config.play.random[1]) {
				if (9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) {
					level = g->gameplay.playerstat.trial;
					break;
				}
				level = g->gameplay.playerstat.trial;
			}
			if (g->config.play.random[0] == 2 && g->config.play.m_HIDSUD1 == 3 && g->gameplay.player[0].totalnotes >= 1000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 27:
			if (g->audio.param.pitch_on != 1 || (g->audio.param.pitch_type != 0 && g->audio.param.pitch_type != 2) || g->audio.param.pitch_amount < 6) break;
			if (g->gameplay.player[0].totalnotes >= 1000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 28:
			if (g->gameplay.isSpeedChanged == false && g->config.play.hsfix == 4 && g->config.play.hiSpeed[0] == 600 && g->gameplay.player[0].totalnotes >= 300) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 29:
			if ((g->gameplay.player[0].exscore == g->gameplay.player[0].totalnotes *2 *8 / 9) && g->gameplay.player[0].totalnotes >= 1000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 30:
			if (g->gameplay.player[0].totalnotes == 9 && g->gameplay.player[0].judgetime[5] == 8 && g->gameplay.player[0].judgecount[5] == 9) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 31:
			if (g->gameplay.player[0].totalnotes >= 1000 && g->gameplay.song_runtime < 150000.0 && gauge == 4) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 32:
			if (g->gameplay.player[0].totalnotes >= 1000 && g->gameplay.song_runtime < 150000.0 && gauge == 5) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 33:
			if (g->gameplay.player[0].totalnotes >= 1000 && g->gameplay.song_runtime < 150000.0 && g->gameplay.player[0].max_combo < 40) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 34:
			if (g->gameplay.isSpeedChanged == false && g->gameplay.player[0].totalnotes >= 1200 && g->config.play.hsfix == 4 && g->gameplay.song_runtime < 150000.0 && g->config.play.hiSpeed[0] == 30) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 35:
			if (g->gameplay.player[0].totalnotes >= 1200 && g->gameplay.song_runtime < 150000.0 && gauge == 1 && g->config.play.random[0] == 5) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 36:
			if (gauge == 2 && 998 <= g->gameplay.player[0].exscore && 1002 <= g->gameplay.player[0].exscore) { //TOFIX g->gameplay.player[0].exscore <= 1002
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 37:
			if (g->audio.param.pitch_on == 1 && (g->audio.param.pitch_type == 0 || g->audio.param.pitch_type == 2) &&
				g->audio.param.pitch_amount == 12 && g->gameplay.player[0].totalnotes >= 1000 && g->gameplay.song_runtime < 150000.0) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 38:
			if (g->gameplay.player[0].totalnotes >= 1500 && g->gameplay.song_runtime < 150000.0 && gauge == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 39:
			if (g->gameplay.isSpeedChanged == false && (g->config.play.m_HIDSUD1 == g->config.play.m_HIDSUD2 || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10)
				&& g->gameplay.player[0].totalnotes >= 1500 && g->gameplay.song_runtime < 150000.0 && g->config.play.hiSpeed[0] == 100
				&& g->config.play.m_HIDSUD1 == 1 && g->config.play.hsfix == 4) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 40:
			if (g->gameplay.player[0].totalnotes >= 1500 && g->gameplay.song_runtime < 150000.0 && g->config.play.random[0] == 5 && 0 < g->config.play.rand[0]) {
				g->gameplay.playerstat.trial = level + 1;
			}
	}
	SetObjectStrings_SongSelect(g);
	g->gameplay.trialClear = level < g->gameplay.playerstat.trial;
	return 0;
}

//41bcd0
int ProcI_Lunaris(game *g) {
	
	int interval;

	if (GetTimeLapse(41, &g->timer1) <= 0.0) return 1;

	if (g->config.play.hiSpeed[0] < 200) interval = 1000;
	else if (g->config.play.hiSpeed[0] < 300) interval = 500;
	else if (g->config.play.hiSpeed[0] < 400) interval = 250;
	else interval = 167;

	int rTimer = GetTimeLapse(140, &g->timer1); //in rTimer, 1000 is 1 beat

	if (lun_timer > rTimer || lun_timer / interval < rTimer / interval) {
		LUNARIS_JUDGE(g);
		lun_timer = GetTimeLapse(140, &g->timer1);
	}

	if (g->KeyInput.inputID[D_DIK_LEFT] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1) {
		LUNARIS_MOVE(2);
		return 1;
	}
	if (g->KeyInput.inputID[D_DIK_RIGHT] == 1 || g->KeyInput.p1_buttonInput[7] == 1) {
		LUNARIS_MOVE(4);
		return 1;
	}
	if (g->KeyInput.inputID[D_DIK_DOWN] == 1 || g->KeyInput.p1_buttonInput[2] == 1) {
		LUNARIS_MOVE(8);
		return 1;
	}
	if (g->KeyInput.inputID[D_DIK_UP] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[6] == 1) {
		LUNARIS_ROTATE(0);
	}
	else if (g->KeyInput.p1_buttonInput[1] == 1) {
		LUNARIS_ROTATE(1);
		return 1;
	}

	return 1;
}

//41cb80
int ProcS_Select(game *g) {
	
	if (g->sSelect.cur_song >= g->sSelect.bmsListCount) g->sSelect.cur_song = 0;

	SetObjectString(10, g->sSelect.bmsList[g->sSelect.cur_song].title, g->txtStruct.objectStr);
	SetObjectString(11, g->sSelect.bmsList[g->sSelect.cur_song].subtitle, g->txtStruct.objectStr);
	SetObjectString(12, g->sSelect.bmsList[g->sSelect.cur_song].fulltitle, g->txtStruct.objectStr);
	SetObjectString(13, g->sSelect.bmsList[g->sSelect.cur_song].genre, g->txtStruct.objectStr);
	SetObjectString(14, g->sSelect.bmsList[g->sSelect.cur_song].artist, g->txtStruct.objectStr);
	SetObjectString(15, g->sSelect.bmsList[g->sSelect.cur_song].subartist, g->txtStruct.objectStr);
	SetObjectString(16, g->sSelect.bmsList[g->sSelect.cur_song].tag, g->txtStruct.objectStr);
	SetObjectStringInt(17, g->sSelect.bmsList[g->sSelect.cur_song].level, g->txtStruct.objectStr);
	SetObjectStringInt(18, g->sSelect.bmsList[g->sSelect.cur_song].difficulty, g->txtStruct.objectStr);
	SetObjectStringInt(19, g->sSelect.bmsList[g->sSelect.cur_song].exlevel, g->txtStruct.objectStr);

	SetObjectString(20, g->sSelect.bmsList[g->sSelect.cur_song].title, g->txtStruct.objectStr);
	SetObjectString(21, g->sSelect.bmsList[g->sSelect.cur_song].subtitle, g->txtStruct.objectStr);
	SetObjectString(22, g->sSelect.bmsList[g->sSelect.cur_song].fulltitle, g->txtStruct.objectStr);
	SetObjectString(23, g->sSelect.bmsList[g->sSelect.cur_song].genre, g->txtStruct.objectStr);
	SetObjectString(24, g->sSelect.bmsList[g->sSelect.cur_song].artist, g->txtStruct.objectStr);
	SetObjectString(25, g->sSelect.bmsList[g->sSelect.cur_song].subartist, g->txtStruct.objectStr);
	SetObjectString(26, g->sSelect.bmsList[g->sSelect.cur_song].tag, g->txtStruct.objectStr);
	SetObjectStringInt(27, g->sSelect.bmsList[g->sSelect.cur_song].level, g->txtStruct.objectStr);
	SetObjectStringInt(28, g->sSelect.bmsList[g->sSelect.cur_song].difficulty, g->txtStruct.objectStr);
	SetObjectStringInt(29, g->sSelect.bmsList[g->sSelect.cur_song].exlevel, g->txtStruct.objectStr);

	for (int i = 0; i < 9; i++) {
		if (g->sSelect.bmsList[g->sSelect.cur_song].courseType == 1) 
			SetObjectString(171 + i, g->txtStruct.option_str[20].str[g->sSelect.bmsList[g->sSelect.cur_song].courseKeys[i]], g->txtStruct.objectStr);
		else
			SetObjectString(171 + i, "DISABLE", g->txtStruct.objectStr);
	}
	SetObjectString(183, g->txtStruct.option_str[24].str[g->sSelect.bmsList[g->sSelect.cur_song].courseIR], g->txtStruct.objectStr);
	
	if (g->sSelect.course.isMakingCourse == 1) {
		for (int i = 0; i < 5; i++) {
			SetObjectString(150 + i, g->sSelect.course.data[i].title, g->txtStruct.objectStr);
			SetObjectString(160 + i, g->sSelect.course.data[i].subtitle, g->txtStruct.objectStr);
		}
		SetObjectString(170, g->sSelect.bmsList[g->sSelect.cur_song].title, g->txtStruct.objectStr);
	}
	else if(g->sSelect.bmsList[g->sSelect.cur_song].folderType == 8){
		for (int i = 0; i < 5; i++) {
			SetObjectString(150 + i, g->sSelect.bmsList[g->sSelect.cur_song].courseTitle[i], g->txtStruct.objectStr);
			SetObjectString(160 + i, g->sSelect.bmsList[g->sSelect.cur_song].courseSubtitle[i], g->txtStruct.objectStr);
		}
		SetObjectString(170, g->sSelect.bmsList[g->sSelect.cur_song].title, g->txtStruct.objectStr);
	}
	
	SetObjectString(30, g->sSelect.stack_searchTitle[g->sSelect.cur], g->txtStruct.objectStr);
	SetTarget(g);
	DeleteKeyInput(g->txtStruct.hKeyInput);
	g->txtStruct.st_text_num = -1;
	g->sSelect.metaSelected.artist = g->sSelect.bmsList[g->sSelect.cur_song].artist;
	g->sSelect.metaSelected.filepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
	g->sSelect.metaSelected.subartist = g->sSelect.bmsList[g->sSelect.cur_song].subartist;
	g->sSelect.metaSelected.title = g->sSelect.bmsList[g->sSelect.cur_song].title;
	g->sSelect.metaSelected.subtitle = g->sSelect.bmsList[g->sSelect.cur_song].subtitle;

	g->sSelect.metaSelected.selLevel = g->sSelect.bmsList[g->sSelect.cur_song].level;
	g->sSelect.metaSelected.exlevel = g->sSelect.bmsList[g->sSelect.cur_song].exlevel;
	g->sSelect.metaSelected.difficulty = g->sSelect.bmsList[g->sSelect.cur_song].difficulty;
	g->sSelect.metaSelected.keymode = g->sSelect.bmsList[g->sSelect.cur_song].keymode;

	g->sSelect.levelBarGraph[0] = g->sSelect.levelIndicatorAnimation[0];
	g->sSelect.levelsOfSong[0] = g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[0];
	g->sSelect.levelBarGraph[1] = g->sSelect.levelIndicatorAnimation[1];
	g->sSelect.levelsOfSong[1] = g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[1];
	g->sSelect.levelBarGraph[2] = g->sSelect.levelIndicatorAnimation[2];
	g->sSelect.levelsOfSong[2] = g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[2];
	g->sSelect.levelBarGraph[3] = g->sSelect.levelIndicatorAnimation[3];
	g->sSelect.levelsOfSong[3] = g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[3];
	g->sSelect.levelBarGraph[4] = g->sSelect.levelIndicatorAnimation[4];
	g->sSelect.levelsOfSong[4] = g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[4];

	if (g->sSelect.bmsList[g->sSelect.cur_song].isBanner && g->skstruct.reloadbanner == 1 && g->procSelecter == 2) {
		g->hThreadBanner = _beginthread(ThreadProc_LoadBanner, 0, g);
	}
	if (g->net.isOnline == 1 && g->procSelecter == 2) {
		g->net.WaitAndInitRanking();
		if (g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5 && (g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount < 1 || g->sSelect.bmsList[g->sSelect.cur_song].courseIR)) {
			g->net.IRstatus = 1;
			g->net.hHandle = (HANDLE)_beginthread(ThreadProc_RankingAutoUpdate, 0, g);
			SetObjectStrings_SongSelect(g);
			return 1;
		}
	}
	g->net.IRstatus = 0;
	SetObjectStrings_SongSelect(g);
	return 1;
}


//41dc30
int Proc_Text(game *g, sqlite3 *sql, char flag) {

	char str[1024];
	uint mouseL = g->KeyInput.mouse_buttonL;
	uint mouseR = g->KeyInput.mouse_buttonR;

	if (g->txtStruct.readme.show > 0 || flag || g->sSelect.searchFocused || (0.0 <= GetTimeLapse(5, &g->timer1) && GetTimeLapse(5, &g->timer1) <= 1000.0)) {
		g->KeyInput.mouse_buttonL = 0;
		g->KeyInput.mouse_buttonR = 0;
		g->sSelect.is_mouseOnTextInput = 1;
	}

	if ( (g->KeyInput.mouse_buttonL == 3) || (g->KeyInput.mouse_buttonR == 3) ) {
		DeleteKeyInput(g->txtStruct.hKeyInput);
		g->txtStruct.st_text_num = -1;
	}

	for (int i = 0; i < g->skstruct.otherObject[0].srcSize; i++) {
		if (GetOptionFlag_dst(g, g->skstruct.otherObject[0].dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct.otherObject[0].dst[i].opt2)
			&& GetOptionFlag_dst(g, g->skstruct.otherObject[0].dst[i].opt3)) {

			if (flag == 0 && g->skstruct.otherObject[0].src[i].op1 == 1 && (g->skstruct.otherObject[0].src[i].op2 == 0 || g->skstruct.otherObject[0].src[i].op2 == g->sSelect.panel) && g->txtStruct.st_text_num == -1
				&& MouseOnObject(&g->skstruct.otherObject[0].dst[i], &g->timer1, &g->KeyInput.mouse_oldX, &g->KeyInput.mouse_oldY)) {
				g->sSelect.is_mouseOnTextInput = 1;
				if (g->KeyInput.mouse_buttonL == 3) {
					InitKeyInput();

					int st = g->skstruct.otherObject[0].src[i].st;
					if (st == 27) {
						if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 0) {
							g->txtStruct.hKeyInput = MakeKeyInput(4, 1, 0, 1);
						}
						else {
							g->txtStruct.hKeyInput = MakeKeyInput(2, 1, 0, 1);
						}
					}
					else if (st == 29) {
						g->txtStruct.hKeyInput = MakeKeyInput(2, 1, 0, 1);
					}
					else if (st == 28) {
						g->txtStruct.hKeyInput = MakeKeyInput(1, 1, 0, 1);
					}
					else {
						g->txtStruct.hKeyInput = MakeKeyInput(64, 1, 0, 0);
					}

					SetActiveKeyInput(g->txtStruct.hKeyInput);
					g->txtStruct.st_text_num = g->skstruct.otherObject[0].src[i].st;

					if (g->txtStruct.st_text_num != 30) {
						SetKeyInputString(GetStringFromArray(g->txtStruct.st_text_num, g->txtStruct.objectStr), g->txtStruct.hKeyInput);
					}
				}
			}
			AddDrawingBuffer_Text(&g->skstruct.drBuf, &g->skstruct.otherObject[0].src[i], &g->skstruct.otherObject[0].dst[i], &g->timer1);

		}
	}

	if (g->txtStruct.st_text_num != -1) {
		g->sSelect.is_mouseOnTextInput = 1;

		if (CheckKeyInput(g->txtStruct.hKeyInput) >= 1 && flag == 0) {
			//CheckKeyInput() 0:doing 1:done 2:cancle -1:error
			if (CheckKeyInput(g->txtStruct.hKeyInput) == 1) {

				CSTR buf;
				GetKeyInputString(buf, g->txtStruct.hKeyInput);
				CSTR query;

				CSTR unused_path;
				CSTR unused_type;
				unused_path = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
				if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 0) {
					unused_type = "folder";
				}
				else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode > 0) {
					unused_type = "song";
				}

				switch (g->txtStruct.st_text_num) {
				case 20:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].title = buf;
					if (g->sSelect.bmsList[g->sSelect.cur_song].subtitle.isDiff("(null)")) {
						cstrSprintf(&g->sSelect.bmsList[g->sSelect.cur_song].fulltitle, "%s %s", buf, g->sSelect.bmsList[g->sSelect.cur_song].subtitle);
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
					}
					else {
						g->sSelect.bmsList[g->sSelect.cur_song].fulltitle = buf;
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
					}
					break;
				case 21:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].title = buf;
					cstrSprintf(&g->sSelect.bmsList[g->sSelect.cur_song].fulltitle, "%s %s", g->sSelect.bmsList[g->sSelect.cur_song].title, buf);
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 23:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].genre = buf;
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 24:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].artist = buf;
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 25:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].subartist = buf;
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 26:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].tag = buf;
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 27:
					atol(buf);
					if ((0 <= atol(buf) && atol(buf) < 100) || (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 0)) {
						SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
						SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
						g->sSelect.bmsList[g->sSelect.cur_song].level = atol(buf);
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
					}
					break;
				case 28:
					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode != 0 && (0 <= atol(buf) && atol(buf) <= 5)) {
						SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
						SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
						g->sSelect.bmsList[g->sSelect.cur_song].difficulty = atol(buf);
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
						if (g->config.select.difficulty != g->sSelect.bmsList[g->sSelect.cur_song].difficulty && g->config.select.difficulty != 0) {
							g->config.select.difficulty = g->sSelect.bmsList[g->sSelect.cur_song].difficulty;
						}
					}
					break;
				case 29:
					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode != 0 && atol(buf) >= 0) {
						SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
						SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
						g->sSelect.bmsList[g->sSelect.cur_song].exlevel = atol(buf);
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
					}
					break;
				case 30:
					if (*(buf.atPos(0)) == '/') {
						CmdSearch(g, &buf, sql);
						SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					}
					else {
						query = "(null)";

						if (buf.findStrPos("-") > 0) {
							int from = atol(buf.left(buf.findStrPos("-")));
							int to = atol(buf.right(buf.length() - buf.findStrPos("-") - 1));

							if (from > 0) {
								if (0 < to && 50 <= to) {
									cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE %d<=minbpm AND maxbpm<=%d", from, to);
									buf.add(" BPM");
								}
								else if (1 <= to && to < 50) {
									cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE %d<=level AND level<=%d", from, to);
									cstrSprintf(&buf, "LEVEL %d-%d", from, to);
								}

							}
						}
						else if (atol(buf) >= 50) {
							cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE maxbpm=%d OR minbpm=%d", atol(buf), atol(buf));
							buf.add(" BPM");
						}
						else if (atol(buf) > 0) {
							cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE level = %d", atol(buf));
							cstrSprintf(&buf, "LEVEL %d", atol(buf));
						}

						if (query.isSame("(null)")) {
							sqlite3_snprintf(1024, str, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE title LIKE \'%%%s%%\' OR genre LIKE \'%%%s%%\'  OR artist LIKE \'%%%s%%\' OR tag LIKE \'%%%s%%\' OR path LIKE \'%%%s%%\' OR subtitle LIKE \'%%%s%%\' OR subartist LIKE \'%%%s%%\'", buf, buf, buf, buf, buf, buf, buf);
							query.assign(str);
						}

						g->config.select.difficulty = 0;
						g->sSelect.searchFocused = 1;
						g->sSelect.filterDifficulty = 0;
						g->sSelect.filterKey = g->config.select.key;
						g->sSelect.curQuery[0] = query;
						g->sSelect.queryCount = 1;
						g->sSelect.searchType = 3;
						g->sSelect.searchMax = g->config.select.searchmax;
						g->sSelect.filterSort = g->config.select.sort;
						g->sSelect.unk4fb8[0] = 0;
						g->sSelect.selTitle = g->sSelect.bmsList[g->sSelect.cur_song].title;
						g->sSelect.selFilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
						g->sSelect.selFolder = g->sSelect.bmsList[g->sSelect.cur_song].folder;
						g->sSelect.searchInput = buf;
						g->sSelect.text_num = g->txtStruct.st_text_num;
					}
					break;
				case 170:
					SetObjectString(170, buf, g->txtStruct.objectStr);
					g->sSelect.course.name = buf;
					if (g->sSelect.course.isMakingCourse == 0) {
						g->sSelect.course.isChangingTitle = 1;
					}
					ProcS_Select(g);
					break;

				}

			}
			DeleteKeyInput(g->txtStruct.hKeyInput);
			g->txtStruct.st_text_num = -1;
			SetTimeLapse(4, &g->timer1);
		}
	}

	g->KeyInput.mouse_buttonL = mouseL;
	g->KeyInput.mouse_buttonR = mouseR;
	return 1;
}

//41e330
int CreateRandomCourse(game *g, sqlite3 *sql, char playing) {

	if (g->config.course.maxlevel > 0 && g->config.course.minlevel > 0 && g->config.course.minlevel > g->config.course.maxlevel) {
		int max = g->config.course.maxlevel;
		int min = g->config.course.minlevel;
		g->config.course.maxlevel = min;
		g->config.course.minlevel = max;
	}

	if (g->config.course.maxbpm > 0 && g->config.course.minbpm > 0 && g->config.course.minbpm > g->config.course.maxbpm) {
		int max = g->config.course.maxbpm;
		int min = g->config.course.minbpm;
		g->config.course.maxbpm = min;
		g->config.course.minbpm = max;
	}

	g->sSelect.course.id = -1;
	g->sSelect.course.name = "RANDOM MIX";
	g->sSelect.course.count = 0;

	for (int i = 0; i < 10; i++) {
		InitSongData(&g->sSelect.course.data[i]);
	}

	g->sSelect.course.return_query = g->sSelect.stack_query[1];
	g->sSelect.course.return_filepath = g->sSelect.stack_folderPath[1];
	g->sSelect.course.return_isFolder = g->sSelect.stack_isFolder[1];
	g->sSelect.course.return_searchText = g->sSelect.stack_searchTitle[1];

	g->sSelect.course.return_rivalID = g->sSelect.stack_rivalID[1];
	g->sSelect.course.return_Topbar = g->sSelect.listTopbar;
	SetObjectStrings_SongSelect(g);

	int key;
	switch (g->config.select.key) {
		default:
			key = 7;
			break;
		case 3:
			key = 5;
			break;
		case 4:
		case 5:
			key = 14;
			break;
		case 6:
			key = 10;
			break;
		case 7:
			key = 9;
			break;
	}

	CONFIG_COURSE tCfg;

	memcpy(&tCfg, &g->config.course, sizeof(CONFIG_COURSE));

	if (WriteRandomCourse(sql, &g->sSelect.course, &g->sSelect, tCfg, key) == 1) {
		g->sSelect.is_coursemaking_done = 1;
		g->sSelect.course.isCourseCreated = 1;
		g->sSelect.course.isMakingCourse = 0;
		g->sSelect.filter_clicked = 8 + (playing != 0);
		SetBmsFilter(g, sql);
	}

	return 1;
}

//41e4f0
//41e630
int InitSelectBySearchResult(game *g, sqlite3 *sql) {

	if (g->sSelect.searchFocused == 1) {
		if (g->net.rankingData.showRanking) {
			ResetTimeLapse(175, &g->timer1);
			ResetTimeLapse(176, &g->timer1);
			SwapBmsList(&g->sSelect);
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.barMoveEndTime = GetTimeWrap();
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.nowBar = g->sSelect.listTopbar;
			g->sSelect.cur_song = GetSongCursor(g);
			g->net.rankingData.showRanking = 0;
		}

		g->sSelect.filterKey = g->config.select.key;
		g->sSelect.filterSort = g->config.select.sort;
		if (g->sSelect.filter_clicked != 3) {
			g->sSelect.filterDifficulty = g->config.select.difficulty;
		}

		if (g->sSelect.searchType == 0) {
			if (g->sSelect.filter_clicked == 0) {
				LoopInRange(0, 5, 1, &g->sSelect.filterDifficulty);
				if (g->config.select.ignoredifficultyall == 1 && g->sSelect.filterDifficulty == 0) g->sSelect.filterDifficulty = 1;
			}
			else if (g->sSelect.filter_clicked == 1) {
				LoopInRange(0, 4, 1, &g->sSelect.filterSort);
			}
			else if (g->sSelect.filter_clicked == 2) {
				LoopInRange(0, 7, 1, &g->sSelect.filterKey);
				if (g->config.select.ignorekeyall == 1 && g->sSelect.filterKey == 0) g->sSelect.filterKey = 1;
				if (g->config.select.ignorekeysingle == 1 && g->sSelect.filterKey == 1) g->sSelect.filterKey = 2;
				if (g->config.select.ignorekeysingle == 1 && g->sSelect.filterKey == 4) g->sSelect.filterKey = 5;
				if (g->config.select.ignoredp == 1 && (4 <= g->sSelect.filterKey && g->sSelect.filterKey <= 6)) g->sSelect.filterKey = 7;
				if (g->config.select.ignorepms == 1 && g->sSelect.filterKey == 7) g->sSelect.filterKey = 0;
				if (g->config.select.ignorekeyall == 1 && g->sSelect.filterKey == 0) g->sSelect.filterKey = 1;
			}
		}

		if (g->config.select.disabledifficultyfilter == 1) {
			g->sSelect.filterDifficulty = 0;
			g->config.select.difficulty = 0;
		}
		glb.pGame = g;
		glb.pSql = sql;
		CheckNewSong(&glb);
		SetTimeLapse(170, &g->timer1);
		ResetTimeLapse(171, &g->timer1);
	}

	if (g->sSelect.searchFocused == 4 || g->sSelect.isFolder == -1) {
		g->sSelect.reloadType = 0;
		g->config.select.sort = g->sSelect.filterSort;
		g->config.select.key = g->sSelect.filterKey;
		g->config.select.difficulty = g->sSelect.filterDifficulty;

		if (g->sSelect.isFolder == -1) {
			g->sSelect.searchFocused = 0;
			if (g->sSelect.searchType == 3) {
				SetObjectString(g->sSelect.text_num, "SEARCH FAILED", g->txtStruct.objectStr);
			}
		}
		else {
			if (g->sSelect.searchType == 0) {
				SwapBmsList(&g->sSelect);
				g->sSelect.listTopbar = 0;
				bool flag = 0;
				clsDx();
				if (g->sSelect.filter_clicked == 6) {
					g->sSelect.listTopbar = 0;
					flag = 1;
				}
				if (g->sSelect.filter_clicked == 5) {
					g->sSelect.listTopbar = g->sSelect.bmsListCount * 1000 - 1000;
					flag = 1;
				}
				if (g->sSelect.filter_clicked == 8) {
					g->sSelect.listTopbar = 0;
					flag = 1;
					g->procSelecter = 3;
					g->gameplay.isAutoplay = 0;
				}
				if (g->sSelect.filter_clicked == 9) {
					g->sSelect.listTopbar = 0;
					flag = 1;
					g->procSelecter = 3;
					g->gameplay.isAutoplay = 1;
				}
				if (g->sSelect.filter_clicked == 7) {
					g->sSelect.listTopbar = g->sSelect.course.return_Topbar;
					flag = 1;
				}
				if (g->sSelect.filter_clicked == 10) {
					for (int i = 0; i < g->sSelect.bmsListCount; i++) {
						if (g->sSelect.listTopbar == 0 && flag == 0 && g->sSelect.bmsList[i].courseID == g->sSelect.selKey) {
							g->sSelect.listTopbar = i * 1000;
							flag = 1;
						}
					}
				}

				if (g->sSelect.isDifficultyFilterOn == 1) {
					g->sSelect.isDifficultyFilterOn = 0;
					for (int i = 0; i < g->sSelect.bmsListCount; i++) {
						if (g->sSelect.listTopbar == 0 && g->sSelect.bmsList[i].folder.isSame(&g->sSelect.selFolder) && g->sSelect.bmsList[i].keymode == g->sSelect.selKey
							&& g->sSelect.bmsList[i].difficulty == g->sSelect.filterDifficulty && 0/* g->sSelect.bmsList[i].difficultyCount == g->sSelect.unk5004_difficultycount */) {
							g->sSelect.listTopbar = i * 1000;
							flag = 1;
						}
					}
				}
				if (flag == 0) {
					for (int i = 0; i < g->sSelect.bmsListCount; i++) {
						if (g->sSelect.listTopbar == 0 && flag == 0 && g->sSelect.bmsList[i].filepath.isSame(&g->sSelect.selFilepath)) {
							g->sSelect.listTopbar = i * 1000;
							flag = 1;
						}
					}
				}
				if (g->sSelect.listTopbar == 0) {
					if (flag == 0) {
						for (int i = 0; i < g->sSelect.bmsListCount; i++) {
							if (g->sSelect.listTopbar == 0 && flag == 0 && g->sSelect.bmsList[i].folder.isSame(&g->sSelect.selFolder) && g->sSelect.bmsList[i].keymode == g->sSelect.selKey
								&& g->sSelect.bmsList[i].difficultyCount == 0) {
								g->sSelect.listTopbar = i * 1000;
								flag = 1;
							}
						}
					}
					if (g->sSelect.listTopbar == 0 && flag == 0 && g->sSelect.filter_clicked == 2) {
						for (int i = 0; i < g->sSelect.bmsListCount; i++) {
							if (g->sSelect.listTopbar == 0 && flag == 0 && g->sSelect.bmsList[i].difficultyCount == 0 && g->sSelect.bmsList[i].folder.isSame(&g->sSelect.selFolder)) {
								g->sSelect.listTopbar = i * 1000;
								flag = 1;
							}
						}
					}
				}
				g->sSelect.listTopbar += (g->skstruct.BAR_CENTER - g->sSelect.listSelectedBarFromScreenTop) * 1000;
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.barMoveEndTime = GetTimeWrap();
				g->sSelect.oldBar = g->sSelect.listTopbar;
				g->sSelect.nowBar = g->sSelect.listTopbar;
				if (g->cmd_directplay == 0) { //same GetSongCursor(g)
					g->sSelect.cur_song = (g->sSelect.bmsListCount * 30 - g->skstruct.BAR_CENTER + g->sSelect.listSelectedBarFromScreenTop +
						((g->sSelect.listCalculatedBar % 1000 == 0 || g->sSelect.scrollDirection != 2) ? (g->sSelect.listCalculatedBar / 1000) : (g->sSelect.listCalculatedBar / 1000 + 1)))
						% g->sSelect.bmsListCount;
				}
				else {
					g->sSelect.cur_song = 0;
				}
				SetTimeLapse(11, &g->timer1);
				ProcS_Select(g);
				if (g->sSelect.is_coursemaking_done == 0) {
					if (g->sSelect.is_clicked_filter == 0 || g->audio.sysSound.difficulty.load == 0) {
						PlaySound(&g->audio, &g->audio.sysSound.folder_open, g->audio.chnKey, -1);
					}
					else {
						PlaySound(&g->audio, &g->audio.sysSound.difficulty, g->audio.chnKey, -1);
					}
				}
				g->sSelect.is_clicked_filter = 0;
				g->sSelect.is_coursemaking_done = 0;
			}
			else if (g->sSelect.searchType == 1) {
				ProcS_Select(g);
				SwapBmsList(&g->sSelect);
				g->sSelect.cur++;
				g->sSelect.stack_query[g->sSelect.cur] = g->sSelect.curQuery[g->sSelect.isFolder];
				g->sSelect.stack_isFolder[g->sSelect.cur] = g->sSelect.unk4fb8[g->sSelect.isFolder];
				g->sSelect.stack_folderPath[g->sSelect.cur - 1] = g->sSelect.selFilepath;
				g->sSelect.stack_searchTitle[g->sSelect.cur] = g->sSelect.selTitle;
				g->sSelect.stack_rivalID[g->sSelect.cur] = g->sSelect.searchMax;
				g->sSelect.listTopbar = (g->skstruct.BAR_CENTER - g->sSelect.listSelectedBarFromScreenTop) * 1000;
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.barMoveEndTime = GetTimeWrap();
				g->sSelect.oldBar = g->sSelect.listTopbar;
				g->sSelect.nowBar = g->sSelect.listTopbar;
				g->sSelect.cur_song = GetSongCursor(g);
				SetTimeLapse(11, &g->timer1);
				ProcS_Select(g);
				if (g->sSelect.searchMax == 1 && g->sSelect.rivalID == 0) {
					g->procSelecter = 3;
					g->gameplay.isAutoplay = 0;
					g->isSkipDrawTick = 1;
				}
				else {
					PlaySound(&g->audio, &g->audio.sysSound.folder_open, g->audio.chnKey, -1);
				}
			}
			else if (g->sSelect.searchType == 2 || g->sSelect.searchType == 4) {
				SwapBmsList(&g->sSelect);
				SetObjectString(30, g->sSelect.stack_searchTitle[g->sSelect.cur], g->txtStruct.objectStr);
				g->sSelect.listTopbar = (g->skstruct.BAR_CENTER - g->sSelect.listSelectedBarFromScreenTop) * 1000;
				for (int i = 0; i < g->sSelect.bmsListCount; i++) {
					g->sSelect.listTopbar += 1000;
					if (g->sSelect.stack_folderPath[g->sSelect.cur - 1].isSame(&g->sSelect.bmsList[(g->sSelect.listTopbar / 1000 + g->sSelect.bmsListCount * 30 - g->skstruct.BAR_CENTER + g->sSelect.listSelectedBarFromScreenTop) % g->sSelect.bmsListCount].filepath))
						break;
				}
				g->sSelect.cur--;
				SetObjectString(30, g->sSelect.stack_searchTitle[g->sSelect.cur], g->txtStruct.objectStr);
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.barMoveEndTime = GetTimeWrap();
				g->sSelect.oldBar = g->sSelect.listTopbar;
				g->sSelect.nowBar = g->sSelect.listTopbar;
				g->sSelect.cur_song = GetSongCursor(g);
				SetTimeLapse(11, &g->timer1);
				ProcS_Select(g);
				if (g->sSelect.searchType == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.folder_close, g->audio.chnKey, -1);
				}
			}
			else if (g->sSelect.searchType == 3) {
				CSTR msg;
				SwapBmsList(&g->sSelect);
				g->sSelect.cur++;
				g->sSelect.stack_query[g->sSelect.cur] = g->sSelect.curQuery;
				g->sSelect.stack_isFolder[g->sSelect.cur] = 0;

				if (g->sSelect.bmsListCount < 2) cstrSprintf(&msg, "%s (1 HIT)", g->sSelect.searchInput);
				else cstrSprintf(&msg, "%s (%d HITS)", g->sSelect.searchInput, g->sSelect.bmsListCount);

				SetObjectString(g->txtStruct.st_text_num, msg, g->txtStruct.objectStr);
				g->sSelect.stack_searchTitle[g->sSelect.cur] = msg;
				g->sSelect.stack_folderPath[g->sSelect.cur - 1] = g->sSelect.selFilepath;
				g->sSelect.listTopbar = (g->skstruct.BAR_CENTER - g->sSelect.listSelectedBarFromScreenTop) * 1000;
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.barMoveEndTime = GetTimeWrap();
				g->sSelect.oldBar = g->sSelect.listTopbar;
				g->sSelect.nowBar = g->sSelect.listTopbar;
				g->sSelect.cur_song = GetSongCursor(g);
				SetTimeLapse(11, &g->timer1);
				ProcS_Select(g);
				PlaySound(&g->audio, &g->audio.sysSound.folder_open, g->audio.chnKey, -1);
			}
		}
		SetObjectStrings_SongSelect(g);
		g->sSelect.searchFocused = 0;
		ResetTimeLapse(170, &g->timer1);
		SetTimeLapse(171, &g->timer1);
		return 1;
	}
	
	return 1;
}

//41f1b0
int SaveResult(game *g, sqlite3* sql) {

	g->net.WaitAndInitRanking();
	PlayerCheckAndSwap(&g->gameplay);
	SetObjectString(20, g->net.IRresultMessage.fillzero(), g->txtStruct.objectStr);

	memcpy(&g->sSelect.old, &g->sSelect.bmsList[g->sSelect.cur_song].mybest,sizeof(STATUS));
	if (g->is_starter) {
		g->gameplay.isForceEasy = 0;
		g->gameplay.isNosave = 0;
	}

	if (g->gameplay.isAutoplay) return -1;

	CheckClear(&g->gameplay.player[0], g->config.play.gaugeOption[0], g->gameplay.is2Pplay);
	if (g->gameplay.courseType == 0 || g->gameplay.courseType == 2) {
		ErrorLogFmtAdd("エキスパ用のスコア保存処理を行います\n");
		if (CheckScoreSaveConditon(g) == 0) return -1;
		SONGDATA bms;

		GetSongData(g->sSelect.bmsList[g->sSelect.cur_song].courseHash[g->gameplay.courseStageNow], &bms, sql, &g->sSelect);
		memcpy(&g->sSelect.old, &bms.mybest, sizeof(STATUS));

		g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;
		if (bms.mybest.total_notes == 0) {
			bms.mybest.total_notes = g->gameplay.player[0].totalnotes;
			if (bms.mybest.total_notes > 0) {
				bms.mybest.rank = g->gameplay.player[0].exscore * 9 / (bms.mybest.total_notes * 2);
				if (bms.mybest.rank > 8) bms.mybest.rank = 8;
				if (bms.mybest.rank < 1 && g->gameplay.player[0].exscore > 0) bms.mybest.rank = 1;
			}
		}

		if (g->gameplay.player[0].totalnotes <= g->gameplay.player[0].note_current) {
			bms.mybest.complete = 1;
		}

		if (g->gameplay.player[0].clearType == 5) {
			bms.mybest.clear = 5;
			if (1 <= bms.difficulty && bms.difficulty <= 5) {
				bms.difficultyLevelBarLamp[bms.difficulty - 1] = 5;
			}
		}

		bool isNewRecord = false;
		if (bms.mybest.stat_great + bms.mybest.stat_pgreat * 2 < g->gameplay.player[0].exscore) {
			bms.mybest.total_notes = g->gameplay.player[0].totalnotes;
			bms.mybest.stat_pgreat = g->gameplay.player[0].judgecount[5];
			bms.mybest.stat_great = g->gameplay.player[0].judgecount[4];
			bms.mybest.stat_good = g->gameplay.player[0].judgecount[3];
			bms.mybest.stat_bad = g->gameplay.player[0].judgecount[2];
			bms.mybest.stat_poor = g->gameplay.player[0].judgecount[1];
			bms.mybest.stat_score = g->gameplay.player[0].score;
			bms.mybest.stat_exscore = g->gameplay.player[0].exscore;
			bms.mybest.rate = (bms.mybest.stat_exscore * 100) / (bms.mybest.total_notes * 2);
			bms.mybest.rank = (bms.mybest.stat_exscore * 9) / (bms.mybest.total_notes * 2);


			if (bms.mybest.rank > 8)
				bms.mybest.rank = 8;
			if (bms.mybest.rank < 1 && bms.mybest.stat_exscore > 0)
				bms.mybest.rank = 1;

			bms.mybest.rseed = g->gameplay.randomseed;

			if (bms.keymode < 10) {
				bms.mybest.op_best = g->config.play.gaugeOption[0] + g->config.play.random[0] * 10;
			}
			else {
				bms.mybest.op_best = g->config.play.gaugeOption[0] + g->config.play.random[0] * 10 + g->config.play.random[1] * 100 + g->config.play.dpflip * 1000;
			}

			isNewRecord = true;
		}

		if (bms.mybest.stat_maxcombo < g->gameplay.player[0].max_combo)
			bms.mybest.stat_maxcombo = g->gameplay.player[0].max_combo;

		if (bms.mybest.total_notes == g->gameplay.player[0].note_current) {
			if (bms.mybest.minbp == -1 || g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] < bms.mybest.minbp) {
				bms.mybest.minbp = g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1];
			}
		}
		else {
			if (bms.mybest.minbp == -1 || g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] + (bms.mybest.total_notes - g->gameplay.player[0].note_current) < bms.mybest.minbp) {
				bms.mybest.minbp = g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] + (bms.mybest.total_notes - g->gameplay.player[0].note_current);
			}
		}

		bms.mybest.op_history |= ConvertOptionHistory(g);

		CheckMission(g);

		UpdateScoreDB(bms.hash, &bms.mybest, sql, &g->sSelect.playerPassMD5);

		if (isNewRecord) {
			WriteGhostInDatabase(sql, bms.hash, &g->gameplay.p1Score);
		}

		UpdatePlayerStat(&g->gameplay.playerstat, sql);
		g->sSelect.oldIRrank = bms.mybest.IRranking;

		if (g->net.isOnline == 1 && g->is_starter == 0) {
			ErrorLogAdd("IRに登録しますか？\n");
			g->net.myRanking.InitRanking();
			if (g->gameplay.flag_longsound || g->gameplay.flag_0note) {
				g->net.IRresultMessage = "この曲はIRに登録できません";
			}
			else if (bms.mybest.playcount <= 0) {
				g->net.IRresultMessage = "単体でプレイしたことの無い曲はIRに登録されません";
			}
			else {
				ErrorLogAdd("IRに登録します\n");

				BMSMETA meta;
				ParseBMSMETA(&meta, bms.filepath, 0);
				g->net.myRanking.songMD5 = bms.hash;
				g->net.myRanking.passMD5 = g->net.IR_passMD5;
				g->net.myRanking.title = meta.title;
				if (meta.subtitle.length() > 0) {
					g->net.myRanking.title.add(" ");
					g->net.myRanking.title.add(meta.subtitle);
				}
				g->net.myRanking.genre = meta.genre;
				g->net.myRanking.artist = meta.artist;
				if (meta.subartist.length() > 0) {
					g->net.myRanking.artist.add(" ");
					g->net.myRanking.artist.add(meta.subartist);
				}
				g->net.myRanking.maxbpm = meta.maxbpm;
				g->net.myRanking.minbpm = meta.minbpm;
				g->net.myRanking.playlevel = meta.selLevel;
				g->net.myRanking.clear = bms.mybest.clear;
				g->net.myRanking.exscore = bms.mybest.stat_exscore;
				g->net.myRanking.pg = bms.mybest.stat_pgreat;
				g->net.myRanking.gr = bms.mybest.stat_great;
				g->net.myRanking.gd = bms.mybest.stat_good;
				g->net.myRanking.bd = bms.mybest.stat_bad;
				g->net.myRanking.pr = bms.mybest.stat_poor;
				g->net.myRanking.maxcombo = bms.mybest.stat_maxcombo;
				g->net.myRanking.playcount = bms.mybest.playcount;
				g->net.myRanking.clearcount = bms.mybest.clearcount;
				g->net.myRanking.rate = bms.mybest.rate;
				g->net.myRanking.minbp = bms.mybest.minbp;
				g->net.myRanking.totalnotes = bms.mybest.total_notes;
				g->net.myRanking.opt_history = bms.mybest.op_history;
				g->net.myRanking.opt_this = bms.mybest.op_best;
				g->net.myRanking.rseed = bms.mybest.rseed;
				g->net.myRanking.clear_db = bms.mybest.clear_db;
				g->net.myRanking.clear_sd = bms.mybest.clear_sd;
				g->net.myRanking.clear_ex = bms.mybest.clear_ex;
				g->net.myRanking.line = meta.keymode;
				g->net.myRanking.judge = meta.judge;

				g->net.myRanking.inputtype = DetermineResultPlayDevice(&g->KeyInput);
				g->net.myRanking.ghost = ReadGhost(sql, bms.hash);
				g->net.MakeIRsendScoreThread();

				bms.mybest.IRranking = g->net.rankingData.myRanking;
				bms.mybest.IRplayercount = g->net.rankingData.rankingCount;
				if (g->net.rankingData.rankingCount > 0) {
					bms.mybest.IRclearRate = (g->net.rankingData.rankingCount + g->net.rankingData.clearPlayers[1] - g->net.rankingData.clearPlayers[0]) / g->net.rankingData.rankingCount;
				}
			}
			SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
		}
		return 1;
	}
	else {
		ErrorLogFmtAdd("通常のスコア保存処理を行います\n");
		
		if (g->config.play.battle == 1) {
			CheckClear(&g->gameplay.player[1], g->config.play.gaugeOption[1], g->gameplay.is2Pplay);
		}
		else {
			g->gameplay.player[1].clearType = g->gameplay.player[0].clearType;
		}

		if (g->gameplay.delayCheckCount < g->gameplay.delayDetectedCount * 2) {
			g->net.IRresultMessage = "遅延率が規定値を超えたので、スコアは保存されません。";
			SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
			return -1;
		}
		if (g->cmd_directplay && g->cmd_nosave) {
			g->net.IRresultMessage = "データベースに登録されていない曲のスコアは保存されません。";
			g->gameplay.isNosave = 1;
			SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
			return -1;
		}
		if (g->gameplay.isNosave) return -1;
		
		if (g->gameplay.isForceEasy && g->gameplay.player[0].clearType > 2) {
			if (g->gameplay.player[0].clearType == 5 && (g->config.play.p1_assist == 0 && g->config.play.p2_assist == 0) && ((g->config.play.random[0] != 4 && g->config.play.random[1] != 4) || g->gameplay.minBPM == g->gameplay.maxBPM)) {
				g->gameplay.isForceEasy = 0;
			}
			else {
				g->gameplay.player[0].clearType = 2;
			}
		}

		if (g->config.play.battle != 1 || g->is_starter) {

			if ((g->gameplay.freqSpeedMultiplier < 1.0 || g->config.play.m_lunaris == 1) && g->is_starter == 0) {
				if (g->gameplay.replay.status == 2) return -1;

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[0].clearType < 2) g->gameplay.playerstat.fail++;
				else g->gameplay.playerstat.clear++;

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[0].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				return 0;
			}

			else if (g->config.play.battle == 2 && g->is_starter == 0) {
				if (g->gameplay.replay.status == 2) return -1;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_db < g->gameplay.player[0].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_db = g->gameplay.player[0].clearType;
				}

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[0].clearType < 2) g->gameplay.playerstat.fail++;
				else {
					g->gameplay.playerstat.clear++;
					if (g->gameplay.player[0].clearType > 2) g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history |= 0x4000000;
				}

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[0].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				g->gameplay.isNosave = 1;
				return 0;
			}

			else if (g->config.play.battle == 3 && g->is_starter == 0) {
				if (g->gameplay.replay.status == 2) return -1;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_sd < g->gameplay.player[0].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_sd = g->gameplay.player[0].clearType;
				}

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[0].clearType < 2) g->gameplay.playerstat.fail++;
				else {
					g->gameplay.playerstat.clear++;
					if (g->gameplay.player[0].clearType > 2) g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history |= 0x8000000;
				}

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[0].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				g->gameplay.isNosave = 1;
				return 0;
			}

			else if (g->config.play.is_extra == 1 && g->is_starter == 0){
				if (g->gameplay.replay.status == 2) return -1;
				
				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_ex < g->gameplay.player[0].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_ex = g->gameplay.player[0].clearType;
				}

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[0].clearType < 2) g->gameplay.playerstat.fail++;
				else {
					g->gameplay.playerstat.clear++;
					if (g->gameplay.player[0].clearType > 2) g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history |= 0x2000000;
				}

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;
				
				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[0].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				g->gameplay.isNosave = 1;
				return 0;
			}

			else if ((g->config.play.p1_assist == 1 || g->config.play.p2_assist == 1) || g->config.play.hsfix == 4 || (g->config.play.random[0] > 3 || g->config.play.random[1] > 3) && g->is_starter == 0) {
				if (g->gameplay.replay.status == 2) return -1;

				if (g->gameplay.player[0].clearType > 2) g->gameplay.player[0].clearType = 2;

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[0].clearType < 2) g->gameplay.playerstat.fail++;
				else g->gameplay.playerstat.clear++;

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear < g->gameplay.player[0].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear = g->gameplay.player[0].clearType;
					if (1 <= g->sSelect.bmsList[g->sSelect.cur_song].difficulty && g->sSelect.bmsList[g->sSelect.cur_song].difficulty <= 5) {
						g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevelBarLamp[g->sSelect.bmsList[g->sSelect.cur_song].difficulty - 1] = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear;
					}
				}

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[0].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history |= ConvertOptionHistory(g);

				CheckMission(g);

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				g->gameplay.isNosave = 1;
				return 0;
			}

			else {
				if (g->gameplay.replay.status == 2) return -1;

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[0].clearType < 2) g->gameplay.playerstat.fail++;
				else g->gameplay.playerstat.clear++;

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes == 0) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes = g->gameplay.player[0].totalnotes;
					if (g->gameplay.player[0].totalnotes > 0) {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = g->gameplay.player[0].exscore * 9 / (g->gameplay.player[0].totalnotes * 2);
						if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank > 8) g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 8;
						if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank < 2 && g->gameplay.player[0].exscore > 0) g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 1;
					}
				}

				if (g->gameplay.player[0].totalnotes <= g->gameplay.player[0].note_current) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.complete = 1;
				}

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear < g->gameplay.player[0].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear = g->gameplay.player[0].clearType;
					if (1 <= g->sSelect.bmsList[g->sSelect.cur_song].difficulty && g->sSelect.bmsList[g->sSelect.cur_song].difficulty <= 5) {
						g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevelBarLamp[g->sSelect.bmsList[g->sSelect.cur_song].difficulty - 1] = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear;
					}
				}

				bool isNewRecord = false;
				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great + g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat * 2 < g->gameplay.player[0].exscore) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes = g->gameplay.player[0].totalnotes;
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat = g->gameplay.player[0].judgecount[5];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great = g->gameplay.player[0].judgecount[4];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good = g->gameplay.player[0].judgecount[3];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad = g->gameplay.player[0].judgecount[2];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor = g->gameplay.player[0].judgecount[1];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score = g->gameplay.player[0].score;
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore = g->gameplay.player[0].exscore;
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.rate = (g->gameplay.player[0].exscore * 100) / (g->gameplay.player[0].totalnotes * 2);
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = (g->gameplay.player[0].exscore * 9) / (g->gameplay.player[0].totalnotes * 2);


					if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank > 8)
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 8;
					if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank < 1 && g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore > 0)
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 1;

					g->sSelect.bmsList[g->sSelect.cur_song].mybest.rseed = g->gameplay.randomseed;

					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->config.play.gaugeOption[0] + g->config.play.random[0] * 10;
					}
					else {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->config.play.gaugeOption[0] + g->config.play.random[0] * 10 + g->config.play.random[1] * 100 + g->config.play.dpflip * 1000;
					}

					isNewRecord = true;
				}

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo < g->gameplay.player[0].max_combo)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo = g->gameplay.player[0].max_combo;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes == g->gameplay.player[0].note_current) {
					if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp == -1 || g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] < g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp) {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp = g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1];
					}
				}
				else {
					if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp == -1 || g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] + (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes - g->gameplay.player[0].note_current) < g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp) {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp = g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] + (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes - g->gameplay.player[0].note_current);
					}
				}

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[0].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history |= ConvertOptionHistory(g);

				CheckMission(g);

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				if (isNewRecord) {
					WriteGhostInDatabase(sql, g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->gameplay.p1Score);
				}

				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				g->sSelect.oldIRrank = g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking;

				if (g->net.isOnline == 1 && g->is_starter == 0) {
					if (g->gameplay.flag_longsound) {
						g->net.IRresultMessage = "この曲はIRに登録できません";
					}
					else {
						if (g->gameplay.is2Pplay == 0) {
							g->net.myRanking.InitRanking();
							BMSMETA meta;
							ParseBMSMETA(&meta, g->sSelect.bmsList[g->sSelect.cur_song].filepath, 0);
							g->net.myRanking.songMD5 = g->sSelect.bmsList[g->sSelect.cur_song].hash;
							g->net.myRanking.passMD5 = g->net.IR_passMD5;
							g->net.myRanking.title = meta.title;
							if (meta.subtitle.length() > 0) {
								g->net.myRanking.title.add(" ");
								g->net.myRanking.title.add(meta.subtitle);
							}
							g->net.myRanking.genre = meta.genre;
							g->net.myRanking.artist = meta.artist;
							if (meta.subartist.length() > 0) {
								g->net.myRanking.artist.add(" ");
								g->net.myRanking.artist.add(meta.subartist);
							}
							g->net.myRanking.maxbpm = meta.maxbpm;
							g->net.myRanking.minbpm = meta.minbpm;
							g->net.myRanking.playlevel = meta.selLevel;
							g->net.myRanking.clear = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear;
							g->net.myRanking.exscore = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore;
							g->net.myRanking.pg = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat;
							g->net.myRanking.gr = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great;
							g->net.myRanking.gd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good;
							g->net.myRanking.bd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad;
							g->net.myRanking.pr = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor;
							g->net.myRanking.maxcombo = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo;
							g->net.myRanking.playcount = g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount;
							g->net.myRanking.clearcount = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount;
							g->net.myRanking.rate = g->sSelect.bmsList[g->sSelect.cur_song].mybest.rate;
							g->net.myRanking.minbp = g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp;
							g->net.myRanking.totalnotes = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
							g->net.myRanking.opt_history = g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history;
							g->net.myRanking.opt_this = g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best;
							g->net.myRanking.rseed = g->sSelect.bmsList[g->sSelect.cur_song].mybest.rseed;
							g->net.myRanking.clear_db = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_db;
							g->net.myRanking.clear_sd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_sd;
							g->net.myRanking.clear_ex = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_ex;
							g->net.myRanking.line = meta.keymode;
							g->net.myRanking.judge = meta.judge;

							g->net.myRanking.inputtype = DetermineResultPlayDevice(&g->KeyInput);
							g->net.myRanking.ghost = ReadGhost(sql, g->sSelect.bmsList[g->sSelect.cur_song].hash);
							g->net.MakeIRsendScoreThread();

							g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = g->net.rankingData.myRanking;
							g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = g->net.rankingData.rankingCount;
							if (g->net.rankingData.rankingCount > 0) {
								g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate = (g->net.rankingData.rankingCount + g->net.rankingData.clearPlayers[1] - g->net.rankingData.clearPlayers[0]) / g->net.rankingData.rankingCount;
							}
						}
						else {
							if (g->sSelect.bmsList[g->sSelect.cur_song].courseIR == 0) {
								return 1;
							}

							g->net.myRanking.InitRanking();
							g->net.myRanking.songMD5 = g->sSelect.bmsList[g->sSelect.cur_song].hash;
							g->net.myRanking.passMD5 = g->net.IR_passMD5;
							g->net.myRanking.title = g->sSelect.bmsList[g->sSelect.cur_song].title;
							if (g->sSelect.bmsList[g->sSelect.cur_song].subtitle.length() > 0) {
								g->net.myRanking.title.add(" ");
								g->net.myRanking.title.add(g->sSelect.bmsList[g->sSelect.cur_song].subtitle);
							}
							g->net.myRanking.playlevel = g->sSelect.bmsList[g->sSelect.cur_song].courseType; //TODO : CHECK THIS
							g->net.myRanking.clear = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear;
							g->net.myRanking.exscore = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore;
							g->net.myRanking.pg = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat;
							g->net.myRanking.gr = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great;
							g->net.myRanking.gd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good;
							g->net.myRanking.bd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad;
							g->net.myRanking.pr = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor;
							g->net.myRanking.maxcombo = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo;
							g->net.myRanking.playcount = g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount;
							g->net.myRanking.clearcount = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount;
							g->net.myRanking.rate = g->sSelect.bmsList[g->sSelect.cur_song].mybest.rate;
							g->net.myRanking.minbp = g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp;
							g->net.myRanking.totalnotes = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
							g->net.myRanking.opt_history = g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history;
							g->net.myRanking.opt_this = g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best;
							g->net.myRanking.rseed = g->sSelect.bmsList[g->sSelect.cur_song].mybest.rseed;
							g->net.myRanking.clear_db = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_db;
							g->net.myRanking.clear_sd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_sd;
							g->net.myRanking.clear_ex = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_ex;
							g->net.myRanking.line = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
							
							g->net.myRanking.inputtype = DetermineResultPlayDevice(&g->KeyInput);
							g->net.myRanking.ghost = ReadGhost(sql, g->sSelect.bmsList[g->sSelect.cur_song].hash);
							g->net.MakeIRsendScoreThread();

							g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = g->net.rankingData.myRanking;
							g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = g->net.rankingData.rankingCount;
							if (g->net.rankingData.rankingCount > 0) {
								g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate = (g->net.rankingData.rankingCount + g->net.rankingData.clearPlayers[1] - g->net.rankingData.clearPlayers[0]) / g->net.rankingData.rankingCount;
							}
						}
					}
					SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
				}
				else {
					ErrorLogAdd("IR機能は利用しません\n");
				}
				return 1;
			}
		}

		if (g->gameplay.player[1].clearType > g->gameplay.player[0].clearType) g->gameplay.player[0].clearType = g->gameplay.player[1].clearType;
		
		if (g->gameplay.replay.status != 2) {
			g->gameplay.playerstat.playcount++;

			if (g->gameplay.player[0].clearType >= 2)
				g->gameplay.playerstat.clear++; 
			else 
				g->gameplay.playerstat.fail++;

			g->gameplay.playerstat.playtime += GetTimeLapse(41, &g->timer1) / 1000;
			UpdatePlayerStat(&g->gameplay.playerstat, sql);
			return 0;
		}
	}

	return -1;
}

//420e20
int ProcS_subCourseResult(game *g, sqlite3 *sql) {
	
	g->net.IRresultMessage.fillzero();
	SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);

	PlayerCheckAndSwap(&g->gameplay);

	if (g->gameplay.isNosave || g->gameplay.isAutoplay == 1 || g->gameplay.replay.status == 2) return 0;

	memcpy(&g->sSelect.old, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sizeof(STATUS));
	
	g->gameplay.playerstat.playcount++;
	if (g->gameplay.player[0].clearType >= 2) g->gameplay.playerstat.clear++;
	else g->gameplay.playerstat.fail++;

	g->gameplay.playerstat.playtime += GetTimeLapse(41, &g->timer1);

	if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes == 0) {
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes = g->gameplay.player[0].totalnotes;
		if (g->gameplay.player[0].totalnotes > 0) {
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = g->gameplay.player[0].exscore * 9 / (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes * 2);

			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank > 8) g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 8;
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank < 1 && g->gameplay.player[0].exscore > 0) g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 1;
		}
	}

	if (g->gameplay.player[0].totalnotes <= g->gameplay.player[0].note_current) g->sSelect.bmsList[g->sSelect.cur_song].mybest.complete = 1;

	if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear < g->gameplay.player[0].clearType) {
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear = g->gameplay.player[0].clearType;
		int diff = g->sSelect.bmsList[g->sSelect.cur_song].difficulty;
		if (0 < diff && diff <= 5) {
			g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevelBarLamp[diff-1] = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear;
		}
		
	}
	
	if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great + g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat * 2 < g->gameplay.player[0].exscore) {
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes = g->gameplay.player[0].totalnotes;
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat = g->gameplay.player[0].judgecount[5];
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great = g->gameplay.player[0].judgecount[4];
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good = g->gameplay.player[0].judgecount[3];
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad = g->gameplay.player[0].judgecount[2];
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor = g->gameplay.player[0].judgecount[1];
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score = g->gameplay.player[0].score;
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore = g->gameplay.player[0].exscore;
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.rate = (g->gameplay.player[0].exscore * 100) / (g->gameplay.player[0].totalnotes * 2);
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = (g->gameplay.player[0].exscore * 9) / (g->gameplay.player[0].totalnotes * 2);


		if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank > 8)
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 8;
		if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank < 1 && g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore > 0)
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 1;

		g->sSelect.bmsList[g->sSelect.cur_song].mybest.rseed = g->gameplay.randomseed;

		if (g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) {
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->config.play.gaugeOption[0] + g->config.play.random[0] * 10;
		}
		else {
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->config.play.gaugeOption[0] + g->config.play.random[0] * 10 + g->config.play.random[1] * 100 + g->config.play.dpflip * 1000;
		}
	}

	if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo < g->gameplay.player[0].max_combo)
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo = g->gameplay.player[0].max_combo;

	if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes == g->gameplay.player[0].note_current) {
		if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp == -1 || g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] < g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp) {
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp = g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1];
		}
	}
	else {
		if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp == -1 || g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] + (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes - g->gameplay.player[0].note_current) < g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp) {
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp = g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] + (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes - g->gameplay.player[0].note_current);
		}
	}

	g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
	if (g->gameplay.player[0].clearType >= 2)
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
	else
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

	g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history |= ConvertOptionHistory(g);

	CheckMission(g);

	UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
	UpdatePlayerStat(&g->gameplay.playerstat, sql);

	g->sSelect.oldIRrank = g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking;
	if (g->sSelect.bmsList[g->sSelect.cur_song].courseIR == 0 && g->net.isOnline == 1) {
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = 0;
		g->sSelect.oldIRrank = 0;

		g->net.IRresultMessage = "このコースはIRに対応していません。";
		SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = 0;
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = 0;
		g->net.rankingData.Init();
	}

	if (g->net.isOnline == 1 && g->is_starter == 0 && g->sSelect.bmsList[g->sSelect.cur_song].courseIR == 1) {
		if (g->gameplay.flag_longsound) {
			g->net.IRresultMessage = "このコースはIRに登録できません";
			SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
		}
		else {
			g->net.myRanking.InitRanking();
			g->net.myRanking.songMD5 = g->sSelect.bmsList[g->sSelect.cur_song].hash;
			g->net.myRanking.passMD5 = g->net.IR_passMD5;
			g->net.myRanking.title = g->sSelect.bmsList[g->sSelect.cur_song].title;
			if (g->sSelect.bmsList[g->sSelect.cur_song].subtitle.length() > 0) {
				g->net.myRanking.title.add(" ");
				g->net.myRanking.title.add(g->sSelect.bmsList[g->sSelect.cur_song].subtitle);
			}
			g->net.myRanking.artist = g->sSelect.bmsList[g->sSelect.cur_song].artist;
			if (g->sSelect.bmsList[g->sSelect.cur_song].subartist.length() > 0) {
				g->net.myRanking.artist.add(" ");
				g->net.myRanking.artist.add(g->sSelect.bmsList[g->sSelect.cur_song].subartist);
			}
			g->net.myRanking.playlevel = g->sSelect.bmsList[g->sSelect.cur_song].courseType; //TODO : CHECK THIS
			g->net.myRanking.clear = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear;
			g->net.myRanking.exscore = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore;
			g->net.myRanking.pg = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat;
			g->net.myRanking.gr = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great;
			g->net.myRanking.gd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good;
			g->net.myRanking.bd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad;
			g->net.myRanking.pr = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor;
			g->net.myRanking.maxcombo = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo;
			g->net.myRanking.playcount = g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount;
			g->net.myRanking.clearcount = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount;
			g->net.myRanking.rate = g->sSelect.bmsList[g->sSelect.cur_song].mybest.rate;
			g->net.myRanking.minbp = g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp;
			g->net.myRanking.totalnotes = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			g->net.myRanking.opt_history = g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history;
			g->net.myRanking.opt_this = g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best;
			g->net.myRanking.rseed = g->sSelect.bmsList[g->sSelect.cur_song].mybest.rseed;
			g->net.myRanking.clear_db = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_db;
			g->net.myRanking.clear_sd = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_sd;
			g->net.myRanking.clear_ex = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_ex;
			g->net.myRanking.line = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
			g->net.myRanking.inputtype = DetermineResultPlayDevice(&g->KeyInput);
			g->net.myRanking.ghost = "Z";
			g->net.MakeIRsendScoreThread();

			g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = g->net.rankingData.myRanking;
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = g->net.rankingData.rankingCount;
			if (g->net.rankingData.rankingCount > 0) {
				g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate = (g->net.rankingData.rankingCount + g->net.rankingData.clearPlayers[1] - g->net.rankingData.clearPlayers[0]) / g->net.rankingData.rankingCount;
			}
		}
		SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
		return 1;
	}

	ErrorLogAdd("IR機能は利用しません\n");
	return 1;
}

//421820
int ProcS_CourseResult(game *g, sqlite3 *sql) {
	PlayerCheckAndSwap(&g->gameplay);
	CheckCourseClear(g);
	ProcS_subCourseResult(g, sql);
	LoadScene(&g->skstruct, g->config.skin.skinFilePath[15], g->skinData.Data[g->skinData.skinID[15]].informationP5, 0);
	
	if (g->skstruct.flag_flip) {
		FlipScore(g);
	}

	if ((g->gameplay).player[0].clearType > 1) 
		PlaySound(&g->audio, &g->audio.sysSound.courseclear, g->audio.chnBgm, -1); 
	else 
		PlaySound(&g->audio, &g->audio.sysSound.coursefail, g->audio.chnBgm, -1);
	
	return 1;
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

//422210
int ProcI_PO4Menu(game *g, sqlite3 *sql) { //not tested

	int selectedBar = g->sSelect.listSelectedBarFromScreenTop;

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start
		&& g->txtStruct.readme.show == 0
		&& g->txtStruct.st_text_num < 0
		&& g->sSelect.buttonObjClicked != 1
		&& g->sSelect.searchFocused == 0
		&& !(GetTimeLapse(4, &g->timer1) >= 0 && GetTimeLapse(4, &g->timer1) <= 100.0)
		&& g->sSelect.flag_maniacPanel == 0
		&& g->KeyInput.inputID[D_DIK_F3] != 2) {

		if (GetTimeLapse(175, &g->timer1) == -1.0 && GetTimeLapse(176, &g->timer1) == -1.0) {
			
			if (g->KeyInput.mousewheel) {
				g->sSelect.nowBar -= g->KeyInput.mousewheel * 1000;
				g->sSelect.oldBar = g->sSelect.listTopbar;
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.barMoveEndTime = GetTimeWrap() + 200;
				if (g->KeyInput.mousewheel > 0) g->sSelect.scrollDirection = 1;
				else if (g->KeyInput.mousewheel < 0) g->sSelect.scrollDirection = 2;
			}

			if ((((g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[1] == 1) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
				|| (g->KeyInput.p1_buttonInput[4] == 1 && g->config.select.control == 1)
				|| g->KeyInput.p1_buttonInput[10] == 1 || g->KeyInput.p2_buttonInput[10] == 1 || g->KeyInput.inputID[D_DIK_UP] == 1)
				&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

				g->sSelect.nowBar -= 1000;
				g->sSelect.oldBar = g->sSelect.listTopbar;
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.scrollDirection = 1;
				g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
			}
			else if ((((g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[3] == 1) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
				|| (g->KeyInput.p1_buttonInput[6] == 1 && g->config.select.control == 1)
				|| g->KeyInput.p1_buttonInput[11] == 1 || g->KeyInput.p2_buttonInput[11] == 1 || g->KeyInput.inputID[D_DIK_DOWN] == 1)
				&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

				g->sSelect.nowBar += 1000;
				g->sSelect.oldBar = g->sSelect.listTopbar;
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.scrollDirection = 2;
				g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
			}
			else if ((((g->KeyInput.p1_buttonInput[1] == 2 || g->KeyInput.p2_buttonInput[1] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
				|| (g->KeyInput.p1_buttonInput[4] == 2 && g->config.select.control == 1)
				|| g->KeyInput.p1_buttonInput[10] == 2 || g->KeyInput.p2_buttonInput[10] == 2 || g->KeyInput.inputID[D_DIK_UP] == 2)
				&& GetTimeWrap() > g->sSelect.barMoveEndTime - 20) {

				g->sSelect.nowBar -= 1000;
				g->sSelect.oldBar = g->sSelect.listTopbar;
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.scrollDirection = 1;
				g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speednext;
			}
			else if ((((g->KeyInput.p1_buttonInput[3] == 2 || g->KeyInput.p2_buttonInput[3] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
				|| (g->KeyInput.p1_buttonInput[6] == 2 && g->config.select.control == 1)
				|| g->KeyInput.p1_buttonInput[11] == 2 || g->KeyInput.p2_buttonInput[11] == 2 || g->KeyInput.inputID[D_DIK_DOWN] == 2)
				&& GetTimeWrap() > g->sSelect.barMoveEndTime - 20) {

				g->sSelect.nowBar += 1000;
				g->sSelect.oldBar = g->sSelect.listTopbar;
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.scrollDirection = 2;
				g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speednext;
			}

			g->sSelect.is_mouseOnSongBars = 0;
			g->sSelect.is_mouseOnSelected = 0;

			if (g->KeyInput.is_drag_now < 0) {
				DSTdraw dstd;
				if (g->procPhase == 2) {
					if (GetTimeLapse(2, &g->timer1) > g->skstruct.fadeout || g->skstruct.fadeout == 0) {
						if (g->sSelect.is_clicked_keyconfig == 1) {
							g->procSelecter = 6;
							g->sSelect.is_clicked_keyconfig = 0;
						}
						else if (g->sSelect.is_clicked_skinselect == 1) {
							g->procSelecter = 7;
							g->sSelect.is_clicked_skinselect = 0;
						}
					}
				}
				else if (g->sSelect.is_clicked_autoplay_replay) {
					g->sSelect.is_clicked_autoplay_replay = 0;
				}
				else if ((((g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[6] == 1) && g->config.select.control == 0)
					|| (g->KeyInput.p1_buttonInput[1] == 1 && g->config.select.control == 1))
					&& g->sSelect.panel != 1) {
					//do nothing
				}
				else if (GetTimeLapse(172, &g->timer1) != -1.0) {
					//do nothing
				}
				else if (g->sSelect.panel != 1
					&& ((g->KeyInput.inputID[D_DIK_RIGHT] == 1 || g->KeyInput.p1_buttonInput[13] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.select.control == 0)
					&& g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5) {
					//do nothing
				}
				else if (g->sSelect.panel != 1
					&& ((g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[8] == 1) && g->config.select.control == 1)
					&& g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5) {
					//do nothing
				}
				else if (dstd = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_ON[g->sSelect.listSelectedBarFromScreenTop], GetTimeLapse(0, &g->timer1)), //this is all the condition. really????
					((MouseOnDSTD(&dstd, &g->KeyInput.mouse_oldX, &g->KeyInput.mouse_oldY) == 0 || g->KeyInput.mouse_buttonL != 1) && g->KeyInput.inputID[D_DIK_RIGHT] != 1)
					&& (g->KeyInput.inputID[D_DIK_RETURN] != 1 || g->txtStruct.st_text_num != -1 || (GetTimeLapse(4, &g->timer1) != -1.0 && GetTimeLapse(4, &g->timer1) < 200.0))
					&& (g->sSelect.panel == 1
						|| ((g->config.select.control != 0
							|| (((g->config.select.buttonselect != 0 || (g->KeyInput.p1_buttonInput[1] != 1 && g->KeyInput.p1_buttonInput[3] != 1 && g->KeyInput.p1_buttonInput[5] != 1 && g->KeyInput.p1_buttonInput[7] != 1 && g->KeyInput.p2_buttonInput[1] != 1 && g->KeyInput.p2_buttonInput[3] != 1 && g->KeyInput.p2_buttonInput[5] != 1 && g->KeyInput.p2_buttonInput[7] != 1)))
								&& (g->config.select.buttonselect != 1 || (g->KeyInput.p1_buttonInput[5] != 1 && g->KeyInput.p1_buttonInput[7] != 1 && g->KeyInput.p2_buttonInput[5] != 1 && g->KeyInput.p2_buttonInput[7] != 1))))
							&& (g->config.select.control != 1 || (g->KeyInput.p1_buttonInput[5] != 1 && g->KeyInput.p1_buttonInput[7] != 1))))) {

					for (int i = 0; i < 30; i++) {
						if (g->skstruct.dst_BAR_BODY_OFF[i].dstCount) {
							dstd = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_OFF[i], GetTimeLapse(0, &g->timer1));
							if (MouseOnDSTD(&dstd, &g->KeyInput.mouse_oldX, &g->KeyInput.mouse_oldY) != 0 && g->sSelect.is_mouseOnTextInput == 0) {
								g->sSelect.is_mouseOnSongBars = 1;
								if (g->sSelect.listSelectedBarFromScreenTop == i)
									g->sSelect.is_mouseOnSelected = 1;

								if (g->KeyInput.mouse_buttonL == 2) {
									if (i < g->skstruct.bar_availabe_from) {
										if (g->sSelect.nowBar == g->sSelect.listTopbar) {
											g->sSelect.oldBar = g->sSelect.listTopbar;
											g->sSelect.nowBar += (i - g->skstruct.bar_availabe_from) * 2000;
											g->sSelect.barMoveStartTime = GetTimeWrap();
											g->sSelect.barMoveEndTime = GetTimeWrap() + 200;
										}
									}
									else if (i < g->skstruct.bar_availabe_to + 1) {
										g->sSelect.listSelectedBarFromScreenTop = i;
									}
									else {
										if (i < g->skstruct.bar_availabe_from) {
											if (g->sSelect.nowBar == g->sSelect.listTopbar) {
												g->sSelect.oldBar = g->sSelect.listTopbar;
												g->sSelect.nowBar += (i - g->skstruct.bar_availabe_from) * 2000;
												g->sSelect.barMoveStartTime = GetTimeWrap();
												g->sSelect.barMoveEndTime = GetTimeWrap() + 200;
											}
										}
										if (i > g->skstruct.bar_availabe_to) {
											if (g->sSelect.nowBar == g->sSelect.listTopbar) {
												g->sSelect.oldBar = g->sSelect.listTopbar;
												g->sSelect.nowBar += (i - g->skstruct.bar_availabe_to) * 2000;
												g->sSelect.barMoveStartTime = GetTimeWrap();
												g->sSelect.barMoveEndTime = GetTimeWrap() + 200;
											}
										}
									}
								}
							}
						}
					}
				}
				else {
					g->po4MainMenuCursor = g->sSelect.bmsList[g->sSelect.cur_song].level;
					g->procSelecter = (g->po4MainMenuCursor != 6) ? 10 : 0;
				}
			}
		}
	}
	
	if (g->sSelect.searchFocused == 0) {
		int barp = ChangeValueByTime(g->sSelect.oldBar, g->sSelect.nowBar, g->sSelect.barMoveStartTime, g->sSelect.barMoveEndTime, GetTimeWrap(), 0);
		int barb = g->sSelect.listCalculatedBar;
		g->sSelect.listTopbar = barp;
		g->sSelect.listCalculatedBar = barp;

		while (g->sSelect.listCalculatedBar < 0)
			g->sSelect.listCalculatedBar += g->sSelect.bmsListCount * 1000;

		while (g->sSelect.listCalculatedBar >= g->sSelect.bmsListCount * 1000)
			g->sSelect.listCalculatedBar -= g->sSelect.bmsListCount * 1000;

		if (g->sSelect.cur_song != GetSongCursor(g) || selectedBar != g->sSelect.listSelectedBarFromScreenTop) {
			g->sSelect.cur_song = GetSongCursor(g);
			if (GetTimeLapse(1, &g->timer1) > 0.0) {
				if (g->net.rankingData.showRanking == 0) SetTimeLapse(11, &g->timer1);
				if (g->sSelect.is_loading_bmslist == 0) PlaySound(&g->audio, &g->audio.sysSound.scratch, g->audio.chnKey, -1);
				else g->sSelect.is_loading_bmslist = 0;
			}
			if (g->net.rankingData.showRanking == 0) ProcS_Select(g);
			else SetObjectString(1, g->sSelect.bmsList[g->sSelect.cur_song].fulltitle, g->txtStruct.objectStr);
		}
		g->sSelect.is_loading_bmslist = 0;
		if (barb == g->sSelect.listCalculatedBar) {
			ResetTimeLapse(10, &g->timer1);
		}
		if (barb != g->sSelect.listCalculatedBar) {
			if (GetTimeLapse(10, &g->timer1)) {
				SetTimeLapse(10, &g->timer1);
			}
		}
	}

	selectedBar = g->sSelect.listCalculatedBar % 1000;
	GetNowUnixtime();

	for (int i = 1; i < 30; i++) {
		if (g->skstruct.dst_BAR_BODY_OFF[i - 1].dstCount > 0 && g->skstruct.dst_BAR_BODY_OFF[i].dstCount > 0) {
			DSTdraw dstd1, dstd2, dstd3;
			InitDSTdraw(&dstd1);
			InitDSTdraw(&dstd2);
			InitDSTdraw(&dstd3);

			int bar = (g->sSelect.listCalculatedBar / 1000 - g->skstruct.BAR_CENTER) + i;
			while (bar < 0) {
				bar += g->sSelect.bmsListCount;
			}
			bar = bar % g->sSelect.bmsListCount;

			if (i == g->sSelect.listSelectedBarFromScreenTop) {
				dstd1 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_ON[i], GetTimeLapse(0, &g->timer1));
			}
			else {
				dstd1 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_OFF[i], GetTimeLapse(0, &g->timer1));
			}

			if (i - 1 == g->sSelect.listSelectedBarFromScreenTop) {
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_ON[i - 1], GetTimeLapse(0, &g->timer1));
			}
			else {
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_OFF[i - 1], GetTimeLapse(0, &g->timer1));
			}

			if (selectedBar == 0) {
				dstd3 = dstd1;
			}
			else {
				dstd3 = DSTDbyTime(&dstd1, &dstd2, 0, 1000.0, g->sSelect.listCalculatedBar % 1000);
			}

			if (GetTimeLapse(176, &g->timer1) != -1.0) {
				dstd3.x += ChangeValueByTime(300.0, 0.0, 0.0, 120.0, GetTimeLapse(176, &g->timer1), 0);
				dstd1.x += ChangeValueByTime(300.0, 0.0, 0.0, 120.0, GetTimeLapse(176, &g->timer1), 0);

			}
			else if (GetTimeLapse(175, &g->timer1) != -1.0) {
				dstd3.x += ChangeValueByTime(0.0, 300.0, 0.0, 120.0, GetTimeLapse(175, &g->timer1), 0);
				dstd1.x += ChangeValueByTime(0.0, 300.0, 0.0, 120.0, GetTimeLapse(175, &g->timer1), 0);
			}

			if (dstd1.time != -1 && dstd2.time != -1) {
				AddDrawingBuffer_Lunaris(&g->skstruct.drBuf, &g->skstruct.src_BAR_BODY[g->sSelect.bmsList[bar].folderType], &dstd3, &g->timer1);
				if (i == g->sSelect.listSelectedBarFromScreenTop) {
					AddDrawingBuffer_Object(&g->skstruct.drBuf, &g->skstruct.src_BAR_FLASH, &g->skstruct.dst_BAR_FLASH, &g->timer1, dstd1.x, dstd1.y);
				}

				AddDrawingBuffer_TextXY(&g->skstruct.drBuf, &g->skstruct.src_BAR_TITLE[0], &g->skstruct.dst_BAR_TITLE[0], &g->timer1, bar + 10000, dstd3.x, dstd3.y);
			}
		}
	}

	return 1;
}

//423450
int SetObjectValue_Button(game *g, skstruct *sk, Timer *T, char flag) {
	int mouseL, mouseR;
	int isClickSuccess;
	int iTmp = 0;
	int filterChanged;

	mouseL = g->KeyInput.mouse_buttonL;
	mouseR = g->KeyInput.mouse_buttonR;

	if (g->txtStruct.readme.show > 0 || flag || g->sSelect.searchFocused) {
		g->KeyInput.mouse_buttonL = '\0';
		g->KeyInput.mouse_buttonR = '\0';
	}
	filterChanged = 0;

	for (int i = 0; i < sk->otherObject[1].srcSize; i++) {
		isClickSuccess = 0;
		if (GetOptionFlag_dst(g, sk->otherObject[1].dst[i].opt1)==0 || GetOptionFlag_dst(g, sk->otherObject[1].dst[i].opt2)==0 || GetOptionFlag_dst(g, sk->otherObject[1].dst[i].opt3)==0) continue;

		iTmp = 0;
		int op = sk->otherObject[1].src[i].op1;
		switch (op) {
			//Panels
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				iTmp = (g->sSelect.panel == op);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2	&& g->sSelect.course.isMakingCourse != 1) {
					if (g->sSelect.panel == op) {
						g->sSelect.panel = -1;
						ResetTimeLapse(op + 20, T);
						SetTimeLapse(op + 30, T);
						PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
					}
					else {
						if (g->sSelect.panel >= 0) {
							ResetTimeLapse(g->sSelect.panel + 20, T);
							SetTimeLapse(g->sSelect.panel + 30, T);
						}
						SetObjectStrings_SongSelect(g);
						SetTimeLapse(op + 20, T);
						g->sSelect.panel = op;
						g->sSelect.panel_unk = op;
						PlaySound(&g->audio, &g->audio.sysSound.panel_open, g->audio.chnKey, -1);
					}
					if(g->sSelect.panel == -1) g->sSelect.panel_unk = op;
				}
				else continue;
				break;

			//case 9 not exist

			//SongSelect Option
			case 10:
				iTmp = g->config.select.difficulty;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				if (isClickSuccess == 2) {
					filterChanged = 1;
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 0;
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 11:
				iTmp = g->config.select.key;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				if (isClickSuccess == 2) {
					filterChanged = 1;
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 2;
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 12:
				iTmp = g->config.select.sort;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				if (isClickSuccess == 2) {
					filterChanged = 1;
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 1;
					SetObjectStrings_SongSelect(g);
				}
				break;

			//Buttons
			case 13:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						g->sSelect.is_clicked_keyconfig = 1;
					}
				}
				break;

			case 14:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						g->sSelect.is_clicked_skinselect = 1;
					}
				}
				break;

			case 15:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						if (g->sSelect.bmsList[g->sSelect.cur_song].keymode > 0 || g->sSelect.bmsList[g->sSelect.cur_song].folderType == 9) {
							g->gameplay.isAutoplay = 0;
							g->sSelect.is_clicked_autoplay_replay = 1;
						}
					}
				}
				break;

			case 16:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						if (g->sSelect.bmsList[g->sSelect.cur_song].keymode > 0 || g->sSelect.bmsList[g->sSelect.cur_song].folderType == 9) {
							g->gameplay.isAutoplay = 1;
							g->sSelect.is_clicked_autoplay_replay = 1;
						}
					}
				}
				break;

			case 17:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						ShowReadmes(g);
						if (g->txtStruct.readme.show) {
							ResetTimeLapse(16, T);
							SetTimeLapse(15, T);
							PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						}
					}
				}
				break;

			case 18:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						g->sSelect.is_clicked_tagedit = 1;
					}
				}
				break;

			case 19:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2 && g->sSelect.bmsList[g->sSelect.cur_song].keymode > 0) {
						g->gameplay.replay.status = 2;
						g->gameplay.isAutoplay = 0;
						g->sSelect.is_clicked_autoplay_replay = 1;
					}
				}
				break;
			
			//Option FX
			case 20:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxType[0], 0, 7, g->sSelect.panel);
				if (isClickSuccess == 2) {
					InitFxParam(g, 0);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 61, g->audio.param.fxType[0]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 21:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxType[1], 0, 7, g->sSelect.panel);
				if (isClickSuccess == 2) {
					InitFxParam(g, 1);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 71, g->audio.param.fxType[1]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 22:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxType[2], 0, 7, g->sSelect.panel);
				if (isClickSuccess == 2) {
					InitFxParam(g, 2);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 81, g->audio.param.fxType[2]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 23:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fx_on[0], 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 60, g->audio.param.fx_on[0]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 24:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fx_on[1], 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 70, g->audio.param.fx_on[1]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 25:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fx_on[2], 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 80, g->audio.param.fx_on[2]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 26:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxChannel[0], 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 64, g->audio.param.fxChannel[0]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 27:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxChannel[1], 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 74, g->audio.param.fxChannel[1]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 28:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxChannel[2], 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 84, g->audio.param.fxChannel[2]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 29:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.eq_on, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 50, g->audio.param.eq_on);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 30:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.eq_preset, 0, 5, g->sSelect.panel); //EQ_PRESET is not implemented yet
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
				}
				break;

			case 31:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fx_volume_on, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 40, g->audio.param.fx_volume_on);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 32:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.pitch_on, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 90, g->audio.param.pitch_on);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 33:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.pitch_type, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 92, g->audio.param.pitch_type);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			//Option Play
			case 40:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.gaugeOption[0], 0, 5, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 41:
				if (g->config.play.battle == 1) {
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.gaugeOption[1], 0, 5, g->sSelect.panel);
					if (isClickSuccess == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				else continue;
				break;

			case 42:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.random[0], 0, 5, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 43:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.random[1], 0, 5, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 44:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.p1_assist, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 45:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.p2_assist, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break; 
				
			case 46:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.p1_lanecover, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->config.play.p2_lanecover = g->config.play.p1_lanecover;
					SetObjectStrings_SongSelect(g);
				}
				break;
			
			case 50:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.m_HIDSUD1, 0, 3, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 51:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.m_HIDSUD2, 0, 3, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 54:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.dpflip, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 55:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.hsfix, 0, 4, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 56:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.battle, 0, 4, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 57:
				iTmp = 1;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
				}

				if (iTmp == 2) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
				else if(iTmp == 0) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, -g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
				break;
			case 58:
				iTmp = 1;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
				}

				if (iTmp == 2) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, g->config.play.hsmargin, &g->config.play.hiSpeed[1]);
				else if (iTmp == 0) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, -g->config.play.hsmargin, &g->config.play.hiSpeed[1]);
				break;

			case 70:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.scoregraph, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 71:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.play_ghost, 0, 3, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 72:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.bga, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 73:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.bgasize, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 74:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.judgetiming, -99, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 75:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.autojudge, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 76:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.target_percent, 50, 100, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 77:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.p1_target, 0, 8, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetTarget(g);
					SetObjectStrings_SongSelect(g);
				}
				break;

			//Option etc
			case 80:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.system.screenmode, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->is_clicked_screenModeChange = 1;
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 81:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.system.highcolor, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 82:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.system.vsync, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetWaitVSyncFlag(g->config.system.vsync);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 83:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.replay, 0, 4, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 90:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->sSelect.bmsList[g->sSelect.cur_song].favorite, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
				}
				break;

			//SongSelect Difficulty
			case 91:
			case 92:
			case 93:
			case 94:
			case 95:
			case 96:
				iTmp = g->config.select.difficulty;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				if (isClickSuccess == 2) {
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 0;
					g->sSelect.isDifficultyFilterOn = 0;
					g->sSelect.unk5004_difficultycount = 0;
					filterChanged = 1;
					if (op != 91) {
						if (g->sSelect.bmsList[g->sSelect.cur_song].difficulty == op - 91) {
							g->sSelect.isDifficultyFilterOn = 1;
							g->sSelect.unk5004_difficultycount = g->sSelect.bmsList[g->sSelect.cur_song].difficultyCount;
							LoopInRange(0, g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[g->sSelect.bmsList[g->sSelect.cur_song].difficulty - 1], 1, &g->sSelect.unk5004_difficultycount);
						}
					}
					g->config.select.difficulty = op - 92;
					SetObjectStrings_SongSelect(g);
				}
				break;

			//KeyConfig
			case 100:
			case 101:
			case 102:
			case 103:
			case 104:
			case 105:
			case 106:
			case 107:
			case 108:
			case 109:
			case 110:
			case 111:
			case 112:
			case 113:
			case 114:
			case 115:
			case 116:
			case 117:
			case 118:
			case 119:
			case 120:
			case 121:
			case 122:
			case 123:
			case 124:
			case 125:
			case 126:
			case 127:
			case 128:
			case 129:
			case 130:
			case 131:
			case 132:
			case 133:
			case 134:
			case 135:
			case 136:
			case 137:
			case 138:
			case 139:
				iTmp = (g->KeyInput.config_button_inMap == op - 100);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap = op - 100;
					if (g->KeyInput.config_keymode == 0) {
						g->KeyInput.config_button = ConfigButtonFromKeyID7(op - 100);
					}
					else if (g->KeyInput.config_keymode == 1) {
						g->KeyInput.config_button = ConfigButtonFromKeyID9(op - 100);
					}
					else if (g->KeyInput.config_keymode == 2) {
						g->KeyInput.config_button = ConfigButtonFromKeyID5(op - 100);
					}
					ProcS_Keyconfig(g);
				}
				break;

			case 140:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->KeyInput.config_button, 0, 22, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap  = ConfigButtonToKeyID7(g->KeyInput.config_button);
					ProcS_Keyconfig(g);
				}
				break;

			case 141:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->KeyInput.config_button, 0, 11, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap = ConfigButtonToKeyID9(g->KeyInput.config_button);
					ProcS_Keyconfig(g);
				}
				break;

			case 142:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->KeyInput.config_button, 0, 18, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap = ConfigButtonToKeyID5(g->KeyInput.config_button);
					ProcS_Keyconfig(g);
				}
				break;

			case 143:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->KeyInput.config_keymode, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap = 0;
					g->KeyInput.config_button = 0;
					g->KeyInput.config_key = -1;
					switch (g->KeyInput.config_keymode) {
						case 0:
							ReadKeyConfig(g, "LR2files\\Config\\keyconfig.xml");
							break;
						case 1:
							ReadKeyConfig(g, "LR2files\\Config\\keyconfig_p.xml");
							break;
						case 2:
							ReadKeyConfig(g, "LR2files\\Config\\keyconfig_5.xml");
							break;
					}
					ProcS_Keyconfig(g);
				}
				break;

			case 150:
			case 151:
			case 152:
			case 153:
			case 154:
			case 155:
			case 156:
			case 157: 
				//TOFIX : keyconfig slot is 10 but clickable button is only 8??
				iTmp = (g->KeyInput.config_key == op - 150);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_key = op - 150;
				}
				break;

			//SkinSelect
			case 170:
			case 171:
			case 172:
			case 173:
			case 174:
			case 175:
			case 176:
			case 177:
			case 178:
			case 179:
			case 180:
			case 181:
			case 182:
			case 183:
			case 184:
			case 185:
			case 186:
			case 187:
			case 188: {
				iTmp = (g->skinData.select == op - 170);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 1, g->sSelect.panel);
				if(isClickSuccess == 2){
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->skinData.select = op - 170;
					g->skinData.previewID = g->skinData.skinID[op - 170];
					if (g->skinData.select == 10) {
						ReadLR2SoundSet(g, g->skinData.Data[g->skinData.previewID].skinFile, 0);
						PlaySound(&g->audio, &g->audio.sysSound.select, g->audio.chnBgm, -1);
					}

					if (g->skinData.select == 5) { 
						g->sSelect.toRoot = 1;
					}
					g->skinData.previewCustomID = 0;
					CSTR tcstr;
					SkinUser sku;
					cstrSprintf(&tcstr, "LR2files\\SkinCustomize\\%s.xml", MD5str(g->skinData.Data[g->skinData.previewID].skinFile));
					ReadSkinCustomize(&sku, tcstr);

					for (int j = 0; j < 100; j++) { // VULNERABILITY : out of index sku (array size 40, but access to 100)
						if (g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start == 0) {
							for (int k = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start; k < g->skinData.Data[g->skinData.previewID].customs[j].dst_op_count; k++) {
								if (g->skinData.Data[g->skinData.previewID].customs[j].op_label[k].isSame(&sku.customize_filename[j])) {
									g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = k;
								}
							}
						}
						else {
							if (sku.customize_value[j] < g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start
								|| g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start + g->skinData.Data[g->skinData.previewID].customs[j].dst_op_count - 1 < sku.customize_value[j]) {
								g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start;
							}
							else {
								g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = sku.customize_value[j];
							}
						}
					}

					SetObjectString(50, g->skinData.Data[g->skinData.previewID].title, g->txtStruct.objectStr);
					SetObjectString(51, g->skinData.Data[g->skinData.previewID].maker, g->txtStruct.objectStr);
					for (int j = 0; j < 5; j++) {
						SetObjectString(100 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID+j].title, g->txtStruct.objectStr);
						SetObjectString(110 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].
							op_label[ g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_selected - g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_start ], g->txtStruct.objectStr);
					}
					MakeSkinPreview(g, &g->skstruct2, &g->skinData);
					PlayPreviewSample(g);
					InitTimer(&g->timer2);
					SetTimeLapse(0, &g->timer2);
				}
				break;
			}

			case 190: {
				sk->otherObject[1].src[i];
				iTmp = (g->KeyInput.config_key == op - 150);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, -1, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					if (iTmp == 1) SkinPreviewNext(&g->skinData, (SKINTYPE)g->skinData.select);
					else if (iTmp == -1) SkinPreviewNext(&g->skinData, (SKINTYPE)g->skinData.select);
					SelectSkin(&g->skinData, &g->config.skin.skinFilePath[g->skinData.select]);
					if (g->skinData.select == 10) { // SOUNDSET
						ReadLR2SoundSet(g, g->skinData.Data[g->skinData.previewID].skinFile, 0);
						PlaySound(&g->audio, &g->audio.sysSound.select, g->audio.chnBgm, -1);
					}
					else if (g->skinData.select == 11) { // THEME
						SkinSelect_SoundSet(g,g->skinData.Data[g->skinData.previewID].skinFile);
						ReadLR2SoundSet(g, g->config.skin.skinFilePath[10], 0);
						PlaySound(&g->audio, &g->audio.sysSound.select, g->audio.chnBgm, -1);
					}

					g->skinData.previewCustomID = 0;
					CSTR tcstr;
					SkinUser sku;
					cstrSprintf(&tcstr, "LR2files\\SkinCustomize\\%s.xml", MD5str(g->skinData.Data[g->skinData.previewID].skinFile));
					ReadSkinCustomize(&sku, tcstr);

					for (int j = 0; j < 100; j++) { // VULNERABILITY : out of index sku (array size 40, but access to 100)
						if (g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start == 0) {
							for (int k = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start; k < g->skinData.Data[g->skinData.previewID].customs[j].dst_op_count; k++) {
								if (g->skinData.Data[g->skinData.previewID].customs[j].op_label[k].isSame(&sku.customize_filename[j])) {
									g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = k;
								}
							}
						}
						else {
							if (sku.customize_value[j] < g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start
								|| g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start + g->skinData.Data[g->skinData.previewID].customs[j].dst_op_count - 1 < sku.customize_value[j]) {
								g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start;
							}
							else {
								g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = sku.customize_value[j];
							}
						}
					}

					SetObjectString(50, g->skinData.Data[g->skinData.previewID].title, g->txtStruct.objectStr);
					SetObjectString(51, g->skinData.Data[g->skinData.previewID].maker, g->txtStruct.objectStr);
					for (int j = 0; j < 5; j++) {
						SetObjectString(100 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].title, g->txtStruct.objectStr);
						SetObjectString(110 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].
							op_label[g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_selected - g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_start], g->txtStruct.objectStr);
					}
					MakeSkinPreview(g, &g->skstruct2, &g->skinData);
					PlayPreviewSample(g);
					InitTimer(&g->timer2);
					SetTimeLapse(0, &g->timer2);
				}
				break;
			}
			case 191:
				iTmp = (g->KeyInput.config_key == op - 150);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 5, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SelectSkin(&g->skinData, g->config.skin.skinFilePath + g->skinData.select);
				}
				break;
			case 200:
			case 201:
			case 202:
			case 203:
			case 204:
			case 205:
			case 206:
			case 207:
			case 208:
			case 209:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						ShowReadme(g,sk->helpfilePath[op-200]);
						if (g->txtStruct.readme.show) {
							ResetTimeLapse(16, &g->timer1);
							SetTimeLapse(15, &g->timer1);
							PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						}
					}
				}
				break;

			case 210:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						g->sSelect.is_buttonIRpage = 1;
					}
				}
				break;

			case 220:
			case 221:
			case 222:
			case 223:
			case 224:
			case 225:
			case 226:
			case 227:
			case 228:
			case 229: {
				int customIDThis = op - 220;
				SkinCustom &customThis = g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + customIDThis];
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &customThis.dst_op_selected, customThis.dst_op_start, customThis.dst_op_start + customThis.dst_op_count -1, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					SetObjectString(100 + customIDThis, customThis.title, g->txtStruct.objectStr);
					SetObjectString(110 + customIDThis, customThis.op_label[customThis.dst_op_selected - customThis.dst_op_start], g->txtStruct.objectStr);

					CSTR tcstr;
					SkinUser sku;
					cstrSprintf(&tcstr, "LR2files\\SkinCustomize\\%s.xml", MD5str(g->skinData.Data[g->skinData.previewID].skinFile));
					ReadSkinCustomize(&sku, tcstr);

					for (int j = 0; j < 100; j++) { // VULNERABILITY : out of index sku (array size 40, but access to 100)
						if (g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start == 0) {
							sku.customize_filename[j].assign(&g->skinData.Data[g->skinData.previewID].customs[j].op_label[g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected]);
						}
						else if(g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start > 899 && g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start < 1000){
							sku.customize_value[j] = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected;
						}
					}

					WriteSkinCustomizeXml(&sku, tcstr);
					MakeSkinPreview(g, &g->skstruct2, &g->skinData);
					PlayPreviewSample(g);
					InitTimer(&g->timer2);
					SetTimeLapse(0, &g->timer2);
					if (g->skinData.select == 10) {
						ReadLR2SoundSet(g, g->skinData.Data[g->skinData.previewID].skinFile, 0);
						PlaySound(&g->audio, &g->audio.sysSound.select, g->audio.chnBgm, -1);
					}
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
				}
				break;
			}

			case 230:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, -1, 1, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->sSelect.course.count > 0 && g->sSelect.course.isMakingCourse == 1) {
						ResetTimeLapse(17, &g->timer1);
						SetTimeLapse(18, &g->timer1);
						PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
						SetTimeLapse(5, &g->timer1);
						g->sSelect.cur = 1;
						g->sSelect.stack_query[1] = g->sSelect.course.return_query;
						g->sSelect.stack_folderPath[1] = g->sSelect.course.return_filepath;
						g->sSelect.stack_isFolder[1] = g->sSelect.course.return_isFolder;
						g->sSelect.stack_searchTitle[1] = g->sSelect.course.return_searchText;
						g->sSelect.stack_rivalID[1] = g->sSelect.course.return_rivalID;
						filterChanged = 1;
						g->sSelect.is_coursemaking_done = 1;
						g->sSelect.filter_clicked = 6;
						g->sSelect.course.isCourseCreated = 1;
						g->sSelect.course.isMakingCourse = 0;
					}
				}
				break;

			case 231:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, -1, 1, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->sSelect.course.count > 0 && g->sSelect.course.isMakingCourse == 1) {
						ResetTimeLapse(17, &g->timer1);
						SetTimeLapse(18, &g->timer1);
						PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
						g->sSelect.course.count = -1;
						g->sSelect.course.isMakingCourse = 0;
						SetTimeLapse(5, &g->timer1);
						g->sSelect.cur = 1;
						g->sSelect.stack_query[1] = g->sSelect.course.return_query;
						g->sSelect.stack_folderPath[1] = g->sSelect.course.return_filepath;
						g->sSelect.stack_isFolder[1] = g->sSelect.course.return_isFolder;
						g->sSelect.stack_searchTitle[1] = g->sSelect.course.return_searchText;
						g->sSelect.stack_rivalID[1] = g->sSelect.course.return_rivalID;
						filterChanged = 1;
						g->sSelect.is_coursemaking_done = 1;
						g->sSelect.filter_clicked = 5;
					}
				}
				break;
			//case 232 doesn't exist
			case 233:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, -1, 1, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->sSelect.bmsList[g->sSelect.cur_song].folderType == 8) {
						g->sSelect.course.isDeletingCourse = 1;
					}
				}
				break;

			case 240:
			case 241:
			case 242:
			case 243:
			case 244:
			case 245:
			case 246:
			case 247:
			case 248:
				if (g->sSelect.bmsList[g->sSelect.cur_song].courseIR == 1 || g->sSelect.bmsList[g->sSelect.cur_song].courseType != 1) {
					iTmp = g->sSelect.bmsList[g->sSelect.cur_song].courseKeys[op - 240];
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				}
				else {
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->sSelect.bmsList[g->sSelect.cur_song].courseKeys[op-240], 0, 5, g->sSelect.panel);
					if (isClickSuccess == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						g->sSelect.course.isCourseCreated = 1;
					}
				}
				break;

			//249 250 251 252 not found

			case 253:
				if (g->sSelect.bmsList[g->sSelect.cur_song].courseIR == 1) {
					iTmp = 1;
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				}
				else {
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->sSelect.bmsList[g->sSelect.cur_song].courseIR, 0, 1, g->sSelect.panel);
					if (isClickSuccess == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						g->sSelect.course.isCourseCreated = 1;
					}
				}
				break;

			case 260:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.optimumlevel_7, 0, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 261:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.maxlevel, 0, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 262:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.minlevel, 0, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 263:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.bpmrange, 0, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 264:
				iTmp = g->config.course.maxbpm / 10;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 99, g->sSelect.panel);
				g->config.course.maxbpm = iTmp * 10;
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 265:
				iTmp = g->config.course.minbpm / 10;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 99, g->sSelect.panel);
				g->config.course.minbpm = iTmp * 10;
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 266:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.stage, 0, 5, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;

			case 268:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.defaultconnection, 0, 5, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			default:
				continue;
		}

		if (isClickSuccess) g->sSelect.is_mouseOnTextInput = 1;
		if (isClickSuccess == 2) g->sSelect.buttonObjClicked = 1;
	}

	g->KeyInput.mouse_buttonL = mouseL;
	g->KeyInput.mouse_buttonR = mouseR;
	g->sSelect.is_filter_changed = filterChanged;
	return 1;
}



//427090
void ReactInput(game *g) {

	GetTimeWrap();
	if (g->txtStruct.readme.show > 0 && g->is_starter == 0) {

		g->txtStruct.readme.h += g->KeyInput.mousewheel * g->skstruct.src_README[0].op1;

		if ( (g->KeyInput.mouse_buttonR == 3
			|| (g->KeyInput.inputID[D_DIK_F2] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[2] == 1)
			|| (g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1)
			|| (g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p1_buttonInput[7] == 1))
			|| (g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
			|| g->KeyInput.inputID[D_DIK_RETURN] == 1) {
			
			ResetTimeLapse(15, &g->timer1);
			SetTimeLapse(16, &g->timer1);
			g->txtStruct.readme.show = 0;
		}
		else if (g->KeyInput.mouse_buttonL == 2) { //original is very slow, it is adequately fast. It's misbehavior...but isn't it good?
			if (g->KeyInput.mouse_oldX < 200) {
				g->txtStruct.readme.w += g->timer1.tickTime * 600.0 / 1000.0;
			}
			else if (g->KeyInput.mouse_oldX > 440) {
				g->txtStruct.readme.w += g->timer1.tickTime * 600.0 / -1000.0;
			}
			
			if (g->KeyInput.mouse_oldY < 150) {
				g->txtStruct.readme.h -= g->timer1.tickTime * 600.0 / -1000.0;
			}
			else if (g->KeyInput.mouse_oldY > 330) {
				g->txtStruct.readme.h -= g->timer1.tickTime * 600.0 / 1000.0;
			}
		}

		if (g->KeyInput.inputID[D_DIK_UP] == 2) {
			g->txtStruct.readme.h += g->timer1.tickTime * 600.0 / 1000.0;
		}
		if (g->KeyInput.inputID[D_DIK_DOWN] == 2) {
			g->txtStruct.readme.h += 600.0 * g->timer1.tickTime / -1000.0;
		}

		if (g->txtStruct.readme.h < 480 - g->txtStruct.readme.y) g->txtStruct.readme.h = 480 - g->txtStruct.readme.y;
		if (g->txtStruct.readme.w < 640 - g->txtStruct.readme.x) g->txtStruct.readme.w = 640 - g->txtStruct.readme.x;
		if (g->txtStruct.readme.w > 0) g->txtStruct.readme.w = 0;
		if (g->txtStruct.readme.h > 0) g->txtStruct.readme.h = 0;
	}
	GetTimeWrap();
	GetTimeWrap();
	if (g->audio.is_fmod_disabled== 0) FMOD_System_Update(g->audio.fmodSys);
	SetVolumeByFade(&g->audio);
	GetTimeWrap();
	GetTimeWrap();

	if (g->sSelect.metaSelected.keymode >= 10) {
		if (g->procSelecter == 4 && g->config.play.battle == 3) {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 1, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 1, 0);
		}
		else {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 0, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 0, 0);
		}
	}
	else {
		if (g->procSelecter == 4 && g->config.play.battle == 0) {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 1, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 1, 0);
		}
		else if (g->sSelect.metaSelected.keymode <= 8) {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 0, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 0, 0);
		}
		else if (g->procSelecter == 4 && g->config.play.battle == 3) {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 1, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 1, 0);
		}
		else {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 0, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 0, 0);
		}
	}
	GetTimeWrap();
	GetTimeWrap();

	if (g->gameplay.replay.status == 1 && GetTimeLapse(41, &g->timer1) != -1.0 && g->procSelecter == 4) {
		InputToReplay(&g->gameplay.replay, &g->KeyInput, GetTimeLapse(41, &g->timer1), g->gameplay.scratchSide);
	}
	else if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0 && g->procSelecter == 4) {
		ReplayToInput(&g->gameplay.replay, g, &g->audio, &g->gameplay, &g->KeyInput, &g->timer1);
	}

	GetTimeWrap();
	GetTimeWrap();
	if (g->txtStruct.st_text_num != -1) {
		memset(g->KeyInput.p1_buttonInput, 0, 20);
		memset(g->KeyInput.p2_buttonInput, 0, 20);
	}
	if (GetTimeLapse(1, &g->timer1) >= 0.0) {
		for (int i = 0; i < 10; i++) {
			if (g->KeyInput.p1_buttonInput[i] == 1) {
				SetTimeLapse(100 + i, &g->timer1);
				ResetTimeLapse(120 + i, &g->timer1);
			}
			if (g->KeyInput.p1_buttonInput[i] == 3) {
				ResetTimeLapse(100 + i, &g->timer1);
				SetTimeLapse(120 + i, &g->timer1);
			}
		}
		
		for (int i = 0; i < 10; i++) {
			if (g->KeyInput.p2_buttonInput[i] == 1) {
				SetTimeLapse(110 + i, &g->timer1);
				ResetTimeLapse(130 + i, &g->timer1);
			}
			if (g->KeyInput.p2_buttonInput[i] == 3) {
				ResetTimeLapse(110 + i, &g->timer1);
				SetTimeLapse(130 + i, &g->timer1);
			}
		}
	}
	GetTimeWrap();
	GetTimeWrap();
	FxByMIDI(g);
	GetTimeWrap();
	GetTimeWrap();
	SetObjectValue_Slider(g, &g->skstruct, &g->timer1, 0);
	GetTimeWrap();
	GetTimeWrap();
	SetObjectValue_Button(g, &g->skstruct, &g->timer1, 0);
	GetTimeWrap();
	GetTimeWrap();
	if (g->procSelecter == 7) {
		SetObjectValue_Slider(g, &g->skstruct2, &g->timer2, 1);
		SetObjectValue_Button(g, &g->skstruct2, &g->timer2, 1);
	}

	if (g->procSelecter == 4) {
		if (g->config.play.p1_lanecover == 1) g->config.play.p1_lanecoverv -= g->KeyInput.mousewheel;
		
		if (g->is_starter == 0) {
			if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
				&& (g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[5] == 1)) {

				g->config.play.hiSpeed[0] -= g->config.play.hsmargin;
				g->gameplay.isSpeedChanged = true;
			}

			if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
				&& (g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[4] == 1)) {

				g->config.play.hiSpeed[0] += g->config.play.hsmargin;
				g->gameplay.isSpeedChanged = true;
			}

			if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
				&& (g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[5] == 1)) {

				if (g->config.play.battle == 1) {
					g->config.play.hiSpeed[1] -= g->config.play.hsmargin;
					g->gameplay.isSpeedChanged = true;
				}
				else {
					g->config.play.hiSpeed[0] -= g->config.play.hsmargin;
					g->gameplay.isSpeedChanged = true;
				}
			}

			if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
				&& (g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[4] == 1)) {

				if (g->config.play.battle == 1) {
					g->config.play.hiSpeed[1] += g->config.play.hsmargin;
					g->gameplay.isSpeedChanged = true;
				}
				else {
					g->config.play.hiSpeed[0] += g->config.play.hsmargin;
					g->gameplay.isSpeedChanged = true;
				}
			}

			if (g->config.play.p1_lanecover && g->gameplay.lanecoverDisplayP1 == 1) {
				if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
					&& (g->KeyInput.p1_buttonInput[6] == 1)) {
					g->config.play.p1_lanecoverv -= g->config.play.shuttermargin;
				}

				if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
					&& (g->KeyInput.p1_buttonInput[7] == 1)) {
					g->config.play.p1_lanecoverv += g->config.play.shuttermargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[6] == 1) && g->config.play.battle != 1) {
					g->config.play.p1_lanecoverv -= g->config.play.shuttermargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[7] == 1) && g->config.play.battle != 1) {
					g->config.play.p1_lanecoverv += g->config.play.shuttermargin;
				}
			}
			else {
				if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
					&& (g->KeyInput.p1_buttonInput[6] == 1)) {
					g->config.play.hiSpeed[0] += g->config.play.hsmargin;
				}

				if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
					&& (g->KeyInput.p1_buttonInput[7] == 1)) {
					g->config.play.hiSpeed[0] -= g->config.play.hsmargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[6] == 1) && g->config.play.battle != 1) {
					g->config.play.hiSpeed[0] += g->config.play.hsmargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[7] == 1) && g->config.play.battle != 1) {
					g->config.play.hiSpeed[0] -= g->config.play.hsmargin;
				}
			}

			if (g->config.play.p2_lanecover && g->gameplay.lanecoverDisplayP2 == 1) {
				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[6] == 1) && g->config.play.battle == 1) {
					g->config.play.p2_lanecoverv += g->config.play.shuttermargin;	//TOFIX: shuttermargin up down is oppsite with p1
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[7] == 1) && g->config.play.battle == 1) {
					g->config.play.p2_lanecoverv -= g->config.play.shuttermargin;
				}
			}
			else {
				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[6] == 1) && g->config.play.battle == 1) {
					g->config.play.hiSpeed[1] += g->config.play.hsmargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[7] == 1) && g->config.play.battle == 1) {
					g->config.play.hiSpeed[1] -= g->config.play.hsmargin;
				}
			}
		}
	}

	if (g->config.play.p1_lanecoverv < 0) g->config.play.p1_lanecoverv = 0;
	if (g->config.play.p1_lanecoverv > 100) g->config.play.p1_lanecoverv = 100;

	if (g->config.play.hiSpeed[0] < g->config.play.hsmin) g->config.play.hiSpeed[0] = g->config.play.hsmin;
	if (g->config.play.hiSpeed[0] > g->config.play.hsmax) g->config.play.hiSpeed[0] = g->config.play.hsmax;

	if (g->config.play.p2_lanecoverv < 0) g->config.play.p2_lanecoverv = 0;
	if (g->config.play.p2_lanecoverv > 100) g->config.play.p2_lanecoverv = 100;

	if (g->config.play.hiSpeed[1] < g->config.play.hsmin) g->config.play.hiSpeed[1] = g->config.play.hsmin;
	if (g->config.play.hiSpeed[1] > g->config.play.hsmax) g->config.play.hiSpeed[1] = g->config.play.hsmax;

	if (g->KeyInput.p1_buttonInput[10] == 2) {
		g->skstruct.scratchAngle_1 -= (GetTimeWrap() - g->timer1.scratch) * 2 * 360.0 / 1000.0;
	}
	else if (g->KeyInput.p1_buttonInput[11] == 2) {
		g->skstruct.scratchAngle_1 += (GetTimeWrap() - g->timer1.scratch) * 2 * 360.0 / 1000.0;
	}
	else if (GetTimeLapse(41, &g->timer1) >= 0.0) {
		g->skstruct.scratchAngle_1 += (GetTimeWrap() - g->timer1.scratch) * 360.0 / 1000.0;
	}

	if (g->KeyInput.p2_buttonInput[10] == 2) {
		g->skstruct.scratchAngle_2 -= (GetTimeWrap() - g->timer1.scratch) * 2 * 360.0 / 1000.0;
	}
	else if (g->KeyInput.p2_buttonInput[11] == 2) {
		g->skstruct.scratchAngle_2 += (GetTimeWrap() - g->timer1.scratch) * 2 * 360.0 / 1000.0;
	}
	else if (GetTimeLapse(41, &g->timer1) >= 0.0) {
		g->skstruct.scratchAngle_2 += (GetTimeWrap() - g->timer1.scratch) * 360.0 / 1000.0;
	}

	if (GetTimeLapse(41, &g->timer1) >= 0) {
		g->timer1.Rhythm = ( (GetTimeWrap() - g->timer1.rhythmTick) * g->gameplay.BPM) / 60.0 + g->timer1.Rhythm;
	}

	if(g->procSelecter==4){
		if ((g->KeyInput.p1_buttonInput[12] == 1 || g->KeyInput.p1_buttonInput[13] == 1) || ((g->KeyInput.p2_buttonInput[12] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.play.battle != 1)) {
			if (g->gameplay.lanecoverDoubleclickTimeP1 == 0 || GetTimeWrap() - g->gameplay.lanecoverDoubleclickTimeP1 > 250.0) {
				g->gameplay.lanecoverDoubleclickTimeP1 = GetTimeWrap();
			}
			else {
				g->gameplay.lanecoverDoubleclickTimeP1 = 0;
				g->gameplay.lanecoverDisplayP1 = (g->gameplay.lanecoverDisplayP1 == 0); //means, toggle
			}
		}

		if ((g->KeyInput.p2_buttonInput[12] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.play.battle == 1) {
			if (g->gameplay.lanecoverDoubleclickTimeP2 == 0 || GetTimeWrap() - g->gameplay.lanecoverDoubleclickTimeP2 > 250.0) {
				g->gameplay.lanecoverDoubleclickTimeP2 = GetTimeWrap();
			}
			else {
				g->gameplay.lanecoverDoubleclickTimeP2 = 0;
				g->gameplay.lanecoverDisplayP2 = (g->gameplay.lanecoverDisplayP2 == 0); //means, toggle
			}
		}

		if (g->KeyInput.mouse_buttonL == 1) {
			if (g->gameplay.lanecoverDoubleclickTimeP1 == 0 || GetTimeWrap() - g->gameplay.lanecoverDoubleclickTimeP1 > 250.0) {
				g->gameplay.lanecoverDoubleclickTimeP1 = GetTimeWrap();
			}
			else {
				g->gameplay.lanecoverDoubleclickTimeP1 = 0;
				g->gameplay.lanecoverDisplayP1 = (g->gameplay.lanecoverDisplayP1 == 0); //means, toggle
			}
		}
	}

	if(g->config.play.m_lunaris==1 && GetTimeLapse(41,&g->timer1) > 0.0 && g->procPhase == 1){
		ProcI_Lunaris(g);
	}
	if (g->procSelecter == 4 && g->config.play.m_lunaris == 0) {
		
		if (g->KeyInput.inputID[D_DIK_1] == 2) {
			if (g->KeyInput.inputID[D_DIK_UP] == 1) g->skstruct.adjust.shift_y -= 10;
			if (g->KeyInput.inputID[D_DIK_DOWN] == 1) g->skstruct.adjust.shift_y += 10;
			if (g->KeyInput.inputID[D_DIK_LEFT] == 1) g->skstruct.adjust.shift_x -= 10;
			if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) g->skstruct.adjust.shift_x += 10;
		}
		else if (g->KeyInput.inputID[D_DIK_2] == 2) {
			if (g->KeyInput.inputID[D_DIK_UP] == 1) g->skstruct.adjust.rate_y -= 5;
			if (g->KeyInput.inputID[D_DIK_DOWN] == 1) g->skstruct.adjust.rate_y += 5;
			if (g->KeyInput.inputID[D_DIK_LEFT] == 1) g->skstruct.adjust.rate_x -= 5;
			if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) g->skstruct.adjust.rate_x += 5;
		}
		else if (g->KeyInput.inputID[D_DIK_3] == 2) {
			if (g->KeyInput.inputID[D_DIK_UP] == 1) g->skstruct.adjust.judge_y -= 10;
			if (g->KeyInput.inputID[D_DIK_DOWN] == 1) g->skstruct.adjust.judge_y += 10;
			if (g->KeyInput.inputID[D_DIK_LEFT] == 1) g->skstruct.adjust.judge_x -= 10;
			if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) g->skstruct.adjust.judge_x += 10;
		}
		else if (g->KeyInput.inputID[D_DIK_4] == 2) {
			if (g->KeyInput.inputID[D_DIK_UP] == 1) g->skstruct.adjust.size_y += 1;
			if (g->KeyInput.inputID[D_DIK_DOWN] == 1) g->skstruct.adjust.size_y -= 1;
			if (g->KeyInput.inputID[D_DIK_LEFT] == 1) g->skstruct.adjust.size_x -= 1;
			if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) g->skstruct.adjust.size_x += 1;
		}
		else if (g->KeyInput.inputID[D_DIK_5] == 2) {
			if (g->KeyInput.inputID[D_DIK_UP] == 1) {
				LoopInRange(0, 2, 1, &g->skstruct.adjust.dark_type); //original code doesn't call function, but it's same...
				/*g->skstruct.adjust.dark_type++;
				if (g->skstruct.adjust.dark_type > 2) g->skstruct.adjust.dark_type = 0;
				else if (g->skstruct.adjust.dark_type < 0) g->skstruct.adjust.dark_type = 2;*/
			}
			if (g->KeyInput.inputID[D_DIK_DOWN] == 1) LoopInRange(0, 2, -1, &g->skstruct.adjust.dark_type);
			if (g->KeyInput.inputID[D_DIK_LEFT] == 1) LoopInRange(0, 2, 1, &g->skstruct.adjust.dark_type);
			if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(0, 2, -1, &g->skstruct.adjust.dark_type);
		}
		else if (g->KeyInput.inputID[D_DIK_6] == 2) {
			if (g->KeyInput.inputID[D_DIK_UP] == 1) g->skstruct.adjust.note_1p_y -= 10;
			if (g->KeyInput.inputID[D_DIK_DOWN] == 1) g->skstruct.adjust.note_1p_y += 10;
			if (g->KeyInput.inputID[D_DIK_LEFT] == 1) g->skstruct.adjust.note_1p_x -= 10;
			if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) g->skstruct.adjust.note_1p_x += 10;
		}
		else if (g->KeyInput.inputID[D_DIK_7] == 2) {
			if (g->KeyInput.inputID[D_DIK_UP] == 1) g->skstruct.adjust.note_2p_y -= 10;
			if (g->KeyInput.inputID[D_DIK_DOWN] == 1) g->skstruct.adjust.note_2p_y += 10;
			if (g->KeyInput.inputID[D_DIK_LEFT] == 1) g->skstruct.adjust.note_2p_x -= 10;
			if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) g->skstruct.adjust.note_2p_x += 10;
		}
		else if (g->config.play.disablecurspeedchange != 1) {
			if (g->KeyInput.inputID[D_DIK_UP] == 1) {
				LoopInRange(g->config.play.hsmin, g->config.play.hsmax, g->config.play.hsmargin, &g->config.play.hiSpeed[0]); //original code doesn't call function, but it's same...
			}
			if (g->KeyInput.inputID[D_DIK_DOWN] == 1) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, -g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
			if (g->KeyInput.inputID[D_DIK_LEFT] == 1){
				LoopInRange(-1000, 1000, g->config.play.shuttermargin, &g->config.play.p1_lanecoverv); //original code doesn't call function, but it's same...
			}
			if (g->KeyInput.inputID[D_DIK_RIGHT] == 1) LoopInRange(-1000, 1000, -g->config.play.shuttermargin, &g->config.play.p1_lanecoverv);

			if (g->config.play.p1_lanecoverv > 100) g->config.play.p1_lanecoverv = 100;
			if (g->config.play.p1_lanecoverv < 0) g->config.play.p1_lanecoverv = 0;
		}
	}
	g->timer1.scratch = GetTimeWrap();
	g->timer1.rhythmTick = GetTimeWrap();
	if (g->procSelecter == 7) {
		g->timer2.scratch = g->timer1.scratch;
		g->timer2.rhythmTick = g->timer1.rhythmTick;
	}
	if (g->KeyInput.inputID[D_DIK_F6] == 1) {
		g->flag_Screenshot = 1;
	}
	if (g->KeyInput.inputID[D_DIK_F7] == 1) {
		g->flag_showFPS = (g->flag_showFPS == 0);
	}
}


//maybe deleted by compiler, restored it for convenience
//very complicate, there may be error by re-writter
void SubProcI_Select(game *g, sqlite3 *sql) {
	char buf[1024];
	DSTdraw dstd;

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start
		&& g->txtStruct.readme.show == 0
		&& g->txtStruct.st_text_num < 0
		&& g->sSelect.buttonObjClicked != 1
		&& g->sSelect.searchFocused == 0
		&& !(GetTimeLapse(4, &g->timer1) >= 0 && GetTimeLapse(4, &g->timer1) <= 100.0)
		&& g->sSelect.flag_maniacPanel == 0
		&& g->KeyInput.inputID[D_DIK_F3] != 2) {

		if (GetTimeLapse(10, &g->timer1) == -1.0) {
			if (g->net.rankingData.showRanking == 0 && g->net.IRstatus == 2) {
				if ((g->KeyInput.p1_buttonInput[4] == 2 || g->KeyInput.p2_buttonInput[4] == 2) && GetTimeLapse(175, &g->timer1) == -1.0 && g->KeyInput.p1_buttonInput[11] == 0 && g->KeyInput.p1_buttonInput[10] == 0 && g->KeyInput.inputID[D_DIK_UP] == 0 && g->KeyInput.inputID[D_DIK_DOWN] == 0 && g->sSelect.panel != 1) {
					SetTimeLapse(175, &g->timer1);
					return;
				}
				else if (GetTimeLapse(175, &g->timer1) > 120.0) {
					ResetTimeLapse(175, &g->timer1);
					SetTimeLapse(176, &g->timer1);
					g->sSelect.prevListCount = min(g->net.rankingData.rankingCount, 999);
					for (int i = 0; i < g->sSelect.prevListCount; i++) {
						InitSongData(&g->sSelect.prevList[i]);
						COPY_SONGDATA(&g->sSelect.prevList[i], &g->sSelect.bmsList[g->sSelect.cur_song]);
						g->sSelect.prevList[i].fulltitle = g->net.rankingData.ranking[i].name;
						g->sSelect.prevList[i].title = g->net.rankingData.ranking[i].name;
						g->sSelect.prevList[i].level = i + 1;
						g->sSelect.prevList[i].difficulty = 0;
						g->sSelect.prevList[i].isStagefile = 0;
						g->sSelect.prevList[i].rivalRecord.stat_exscore = g->net.rankingData.ranking[i].gr + g->net.rankingData.ranking[i].pg * 2;
						g->sSelect.prevList[i].rivalRecord.playcount = g->net.rankingData.ranking[i].playcount;
						g->sSelect.prevList[i].rivalRecord.total_notes = g->net.rankingData.ranking[i].notes;
						g->sSelect.prevList[i].rivalRecord.stat_maxcombo = g->net.rankingData.ranking[i].combo;
						g->sSelect.prevList[i].rivalRecord.minbp = g->net.rankingData.ranking[i].minbp;
						g->sSelect.prevList[i].rivalRecord.clear = g->net.rankingData.ranking[i].clear;
						g->sSelect.prevList[i].rivalRecord.playcount = g->net.rankingData.ranking[i].playcount;
						if (g->sSelect.prevList[i].rivalRecord.total_notes > 0) {
							g->sSelect.prevList[i].rivalRecord.rate = g->sSelect.prevList[i].rivalRecord.stat_exscore * 50 / g->sSelect.prevList[i].rivalRecord.total_notes;
							g->sSelect.prevList[i].rivalRecord.rank = (g->sSelect.prevList[i].rivalRecord.stat_exscore * 9) / (g->sSelect.prevList[i].rivalRecord.total_notes * 2);
							if (g->sSelect.prevList[i].rivalRecord.rank > 7) g->sSelect.prevList[i].rivalRecord.rank = 8;
							if (g->sSelect.prevList[i].rivalRecord.rank < 2 && g->sSelect.prevList[i].rivalRecord.stat_exscore > 0) g->sSelect.prevList[i].rivalRecord.rank = 1;
						}
						g->sSelect.prevList[i].rivalRecord.IRranking = g->net.rankingData.ranking[i].ranking;
					}
					g->sSelect.prevSelectedBarFromScreenTop = g->sSelect.listSelectedBarFromScreenTop;
					SwapBmsList(&g->sSelect);
					g->sSelect.listTopbar = (g->skstruct.BAR_CENTER - g->sSelect.listSelectedBarFromScreenTop) * 1000;
					g->sSelect.barMoveStartTime = GetTimeWrap();
					g->sSelect.barMoveEndTime = GetTimeWrap();
					g->sSelect.oldBar = g->sSelect.listTopbar;
					g->sSelect.nowBar = g->sSelect.listTopbar;
					g->sSelect.cur_song = GetSongCursor(g);
					g->net.rankingData.showRanking = 1;
					PlaySound(&g->audio, &g->audio.sysSound.panel_open, g->audio.chnKey, -1);
					SetObjectString(1, g->sSelect.bmsList->fulltitle, g->txtStruct.objectStr); //TOFIX: [g->sSelect.cur_song]?? => str1 is for Rival name
					return;
				}
				else if (GetTimeLapse(176, &g->timer1) > 120.0) {
					ResetTimeLapse(176, &g->timer1);
					return;
				}
			}
			else if (g->net.rankingData.showRanking == 1 && g->net.IRstatus == 2) {
				if (g->KeyInput.p1_buttonInput[4] == 0 && g->KeyInput.p2_buttonInput[4] == 0 && GetTimeLapse(175, &g->timer1) == -1.0 && g->sSelect.panel != 1) {
					SetTimeLapse(175, &g->timer1);
					return;
				}
				else if (GetTimeLapse(175, &g->timer1) > 120.0) {
					ResetTimeLapse(175, &g->timer1);
					SetTimeLapse(176, &g->timer1);

					SwapBmsList(&g->sSelect);
					if (g->sSelect.prevSelectedBarFromScreenTop != g->sSelect.listSelectedBarFromScreenTop) {
						g->sSelect.listTopbar += (g->sSelect.prevSelectedBarFromScreenTop - g->sSelect.listSelectedBarFromScreenTop) * 1000;
					}
					g->sSelect.listCalculatedBar = g->sSelect.listTopbar;
					while (g->sSelect.listCalculatedBar < 0)
						g->sSelect.listCalculatedBar += g->sSelect.bmsListCount * 1000;

					while (g->sSelect.listCalculatedBar >= g->sSelect.bmsListCount * 1000)
						g->sSelect.listCalculatedBar -= g->sSelect.bmsListCount * 1000;

					g->sSelect.barMoveStartTime = GetTimeWrap();
					g->sSelect.barMoveEndTime = GetTimeWrap();
					g->sSelect.oldBar = g->sSelect.listTopbar;
					g->sSelect.nowBar = g->sSelect.listTopbar;
					g->sSelect.cur_song = GetSongCursor(g);
					g->net.rankingData.showRanking = 0;
					if (g->sSelect.stack_query[g->sSelect.cur].findStrPos("__RIVAL__") >= 0 && g->procSelecter == 2) {
						SetObjectString(1, g->sSelect.stack_searchTitle[g->sSelect.cur], g->txtStruct.objectStr);
					}
					else {
						SetTarget(g);
					}
					PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
					return;
				}
				else if (GetTimeLapse(176, &g->timer1) > 120.0) {
					ResetTimeLapse(176, &g->timer1);
					return;
				}
			}
		}

		if (GetTimeLapse(175, &g->timer1) != -1.0) return;
		if (GetTimeLapse(176, &g->timer1) != -1.0) return;

		if (g->KeyInput.inputID[D_DIK_F8] == 1) {
			if (g->sSelect.stack_query[g->sSelect.cur].findStrPos("parent") != -1) {
				SetBmsFilter(g, sql);
				g->sSelect.unk4fa4[0] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM song WHERE parent = \'%s\'", g->sSelect.stack_query[g->sSelect.cur].right(9).left(8));
				if (g->sSelect.bmsList[g->sSelect.cur_song].keymode < 1 || g->sSelect.cur < 1) {
					g->sSelect.reloadType = 3;
					g->sSelect.unk4fa4[1] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", g->sSelect.stack_query[g->sSelect.cur].right(9).left(8));
					g->sSelect.unk4fa4[2] = sqlite3_snprintf(1024, buf, "SELECT path, date FROM song WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath));
					g->sSelect.unk4fa4[3] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath));
				}
				else {
					g->sSelect.reloadType = 2;
					g->sSelect.unk4fa4[1] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", g->sSelect.stack_query[g->sSelect.cur].right(9).left(8));
					g->sSelect.unk4fa4[2] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", g->sSelect.stack_query[g->sSelect.cur - 1].right(9).left(8));
				}
				g->sSelect.unk4fb4 = sqlite3_snprintf(1024, buf, "SELECT difficulty,folder,mode,path FROM song WHERE parent = \'%s\'", g->sSelect.stack_query[g->sSelect.cur].right(9).left(8));
				g->sSelect.filter_clicked = 4;
			}

			if (g->sSelect.panel == 1) {
				SetPlayOption(g, sql);
				if (g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.inputID[D_DIK_SPACE])
					return;
			}
		}
		else if (g->KeyInput.p1_buttonInput[12] == 1 || g->KeyInput.p2_buttonInput[12] == 1 || g->KeyInput.p1_buttonInput[9] == 1 || g->KeyInput.inputID[D_DIK_SPACE] == 1) {
			SetObjectStrings_SongSelect(g);
			if (g->sSelect.panel == 1) {
				g->sSelect.panel = -1;
				ResetTimeLapse(21, &g->timer1);
				SetTimeLapse(31, &g->timer1);
			}
			else {
				if (g->sSelect.panel >= 0) {
					ResetTimeLapse(20 + g->sSelect.panel, &g->timer1);
					SetTimeLapse(30 + g->sSelect.panel, &g->timer1);
				}
				SetTimeLapse(21, &g->timer1);
				g->sSelect.panel = 1;
				g->sSelect.panel_unk = 1;
				PlaySound(&g->audio, &g->audio.sysSound.panel_open, g->audio.chnKey, -1);
			}
			if (g->sSelect.panel == -1) g->sSelect.panel_unk = 1;

			if (g->sSelect.panel == 1) {
				SetPlayOption(g, sql);
				if (g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.inputID[D_DIK_SPACE])
					return;
			}
		}
		else if (g->sSelect.panel == 1) {
			if (g->KeyInput.p1_buttonInput[12] == 3 || g->KeyInput.p2_buttonInput[12] == 3 || g->KeyInput.p1_buttonInput[9] == 3 || g->KeyInput.inputID[D_DIK_SPACE] == 3) {
				ResetTimeLapse(21, &g->timer1);
				SetTimeLapse(30 + g->sSelect.panel, &g->timer1);
				g->sSelect.panel = -1;
				PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
			}

			if (g->sSelect.panel == 1) {
				SetPlayOption(g, sql);
				if (g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.inputID[D_DIK_SPACE])
					return;
			}
		}

		if (g->KeyInput.mousewheel) {
			g->sSelect.nowBar -= g->KeyInput.mousewheel * 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.barMoveEndTime = GetTimeWrap() + 200;
			if (g->KeyInput.mousewheel > 0) g->sSelect.scrollDirection = 1;
			else if (g->KeyInput.mousewheel < 0) g->sSelect.scrollDirection = 2;
		}

		if ((((g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[1] == 1) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[4] == 1 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[10] == 1 || g->KeyInput.p2_buttonInput[10] == 1 || g->KeyInput.inputID[D_DIK_UP] == 1)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

			g->sSelect.nowBar -= 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 1;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
		}
		else if ((((g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[3] == 1) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[6] == 1 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[11] == 1 || g->KeyInput.p2_buttonInput[11] == 1 || g->KeyInput.inputID[D_DIK_DOWN] == 1)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

			g->sSelect.nowBar += 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 2;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
		}
		else if ((((g->KeyInput.p1_buttonInput[1] == 2 || g->KeyInput.p2_buttonInput[1] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[4] == 2 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[10] == 2 || g->KeyInput.p2_buttonInput[10] == 2 || g->KeyInput.inputID[D_DIK_UP] == 2)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime - 20) {

			g->sSelect.nowBar -= 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 1;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speednext;
		}
		else if ((((g->KeyInput.p1_buttonInput[3] == 2 || g->KeyInput.p2_buttonInput[3] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[6] == 2 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[11] == 2 || g->KeyInput.p2_buttonInput[11] == 2 || g->KeyInput.inputID[D_DIK_DOWN] == 2)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime - 20) {

			g->sSelect.nowBar += 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 2;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speednext;
		}

		g->sSelect.is_mouseOnSongBars = 0;
		g->sSelect.is_mouseOnSelected = 0;
		if (g->KeyInput.is_drag_now < 0) {
			if (g->procPhase == 2) {
				if (GetTimeLapse(2, &g->timer1) > g->skstruct.fadeout || g->skstruct.fadeout == 0) {
					if (g->sSelect.is_clicked_keyconfig == 1) {
						g->procSelecter = 6;
						g->sSelect.is_clicked_keyconfig = 0;
					}
					else if (g->sSelect.is_clicked_skinselect == 1) {
						g->procSelecter = 7;
						g->sSelect.is_clicked_skinselect = 0;
					}
					else {
						g->procSelecter = 9;
					}
				}
				return;
			}

			if (g->sSelect.is_clicked_keyconfig || g->sSelect.is_clicked_skinselect) {
				g->procPhase = 2;
				SetTimeLapse(2, &g->timer1);
				StopSysSound(g);
				PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
			}
			else if (g->sSelect.is_clicked_autoplay_replay) {
				g->sSelect.is_clicked_autoplay_replay = 0;
				if (g->sSelect.bmsList[g->sSelect.cur_song].folderType == 9) {
					CreateRandomCourse(g,sql,g->gameplay.isAutoplay != 0);
					return;
				}
				else if (((g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount <= 0 || g->sSelect.bmsList[g->sSelect.cur_song].coursePlayable == 1) && g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 1)) {
					g->procSelecter = 3;
					return;
				}
			}
			else if ((((g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[6] == 1) && g->config.select.control == 0)
				|| (g->KeyInput.p1_buttonInput[1] == 1 && g->config.select.control == 1))
				&& g->sSelect.panel != 1) {
				if (g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount < 1 || g->sSelect.bmsList[g->sSelect.cur_song].coursePlayable) {
					if (g->sSelect.bmsList[g->sSelect.cur_song].folderType == 9) {
						CreateRandomCourse(g, sql, 1);
					}
					else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode > 0) {
						if (g->sSelect.bmsList[g->sSelect.cur_song].replayExist == 1 && g->net.rankingData.showRanking == 0) {
							SetTimeLapse(172, &g->timer1);
							ErrorLogAdd("ホールド開始\n");
						}
						else {
							g->procSelecter = 3;
							g->gameplay.replay.status = 0;
							g->gameplay.isAutoplay = 1;
							ErrorLogAdd("ホールド無しでオートプレイ\n");
						}
						return;
					}
				}
			}
			else if (GetTimeLapse(172, &g->timer1) != -1.0) {
				if (g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount < 1 || g->sSelect.bmsList[g->sSelect.cur_song].coursePlayable) {
					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 0 || g->sSelect.bmsList[g->sSelect.cur_song].replayExist != 1) {
						ResetTimeLapse(172, &g->timer1);
						ErrorLogAdd("ホールド中断\n");
						return;
					}
					else if (g->KeyInput.p1_buttonInput[6] == 0 && g->KeyInput.p2_buttonInput[6] == 0 && g->KeyInput.p1_buttonInput[1] == 0) {
						g->procSelecter = 3;
						g->gameplay.replay.status = 0;
						g->gameplay.isAutoplay = 1;
						ResetTimeLapse(172, &g->timer1);
						ErrorLogAdd("ホールド途中終了でオートプレイ\n");
						return;
					}
					else if (GetTimeLapse(172, &g->timer1) > 2000.0) {
						g->procSelecter = 3;
						g->gameplay.replay.status = 2;
						g->gameplay.isAutoplay = 0;
						ErrorLogAdd("ホールド1秒でリプレイ\n");
						return;
					}
				}
			}
			else if (g->config.select.disabledifficultyfilter == 0 && g->sSelect.panel != 1
				&& ((g->KeyInput.inputID[D_DIK_RIGHT] == 1 || g->KeyInput.p1_buttonInput[13] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.select.control == 0)
				&& g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5) {

				int flgA = 0, flgB = 0;
				int diff = 0;
				if (g->config.select.difficultychangetype == 0) {
					g->sSelect.unk5004_difficultycount = 0;
					g->sSelect.isDifficultyFilterOn = 0;
					if (0 < g->sSelect.bmsList[g->sSelect.cur_song].difficulty && g->sSelect.bmsList[g->sSelect.cur_song].difficulty < 6
						&& g->sSelect.bmsList[g->sSelect.cur_song].difficultyCount < g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[g->sSelect.bmsList[g->sSelect.cur_song].difficulty - 1] - 1) {

						flgA = 1;
					}

					if (g->sSelect.bmsList[g->sSelect.cur_song].difficulty > -1 && g->sSelect.bmsList[g->sSelect.cur_song].difficulty < 5) {
						for (int i = g->sSelect.bmsList[g->sSelect.cur_song].difficulty + 1; i < 6; i++) {
							if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[i - 1] > 0 && flgB == 0) {
								flgB = 1;
								diff = i; // 'diff = i + 1' may be more readable
							}
						}
					}

					// :94d7
					//not the exact code. I modified it.
					int minDifficulty = 1;
					if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[0] > 0) minDifficulty = 1;
					else if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[1] > 0) minDifficulty = 2;
					else if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[2] > 0) minDifficulty = 3;
					else if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[3] > 0) minDifficulty = 4;
					else if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[4] > 0) minDifficulty = 5;

					if (flgA) {
						g->config.select.difficulty = 26;
						g->sSelect.isDifficultyFilterOn = 1;
						g->sSelect.unk5004_difficultycount = g->sSelect.bmsList[g->sSelect.cur_song].difficultyCount + 1;
					}
					else if (flgB) {
						g->config.select.difficulty = diff;
					}
					else if (g->config.select.ignoredifficultyall == 1) {
						g->config.select.difficulty = minDifficulty;
					}
					else {
						g->config.select.difficulty = (g->config.select.difficulty == 0) * minDifficulty;
					}
					g->sSelect.filter_clicked = 4;
					g->sSelect.is_clicked_filter = 1;
					SetBmsFilter(g, sql);
				}
				else {
					SetBmsFilter(g, sql);
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 0;
					SetObjectStrings_SongSelect(g);
				}
			}
			else if (g->config.select.disabledifficultyfilter == 1 && g->sSelect.panel != 1
				&& ((g->KeyInput.inputID[D_DIK_RIGHT] == 1 || g->KeyInput.p1_buttonInput[13] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.select.control == 0)
				&& g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5) {

				g->sSelect.filter_clicked = 2;
				SetBmsFilter(g, sql);
				SetObjectStrings_SongSelect(g);
				g->sSelect.is_clicked_filter = 1;
			}
			else if (g->sSelect.panel != 1
				&& ((g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[8] == 1) && g->config.select.control == 1)
				&& g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5) {

				int flgA = 0, flgB = 0;
				int diff = 0;
				if (g->config.select.difficultychangetype == 0) {
					g->sSelect.unk5004_difficultycount = 0;
					g->sSelect.isDifficultyFilterOn = 0;
					if (g->sSelect.bmsList[g->sSelect.cur_song].difficulty > 0 && g->sSelect.bmsList[g->sSelect.cur_song].difficulty < 6
						&& g->sSelect.bmsList[g->sSelect.cur_song].difficultyCount < g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[g->sSelect.bmsList[g->sSelect.cur_song].difficulty - 1] - 1) {
						flgA = 1;
					}

					if (g->sSelect.bmsList[g->sSelect.cur_song].difficulty > -1 && g->sSelect.bmsList[g->sSelect.cur_song].difficulty < 5) {
						for (int i = g->sSelect.bmsList[g->sSelect.cur_song].difficulty + 1; i < 6; i++) {
							if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[i - 1] > 0 && flgB == 0) {
								flgB = 1;
								diff = i; // 'diff = i + 1' may be more readable
							}
						}
					}
					int minDifficulty = 1;
					if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[0] > 0) minDifficulty = 1;
					else if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[1] > 0) minDifficulty = 2;
					else if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[2] > 0) minDifficulty = 3;
					else if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[3] > 0) minDifficulty = 4;
					else if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[4] > 0) minDifficulty = 5;

					if (flgA) {
						g->config.select.difficulty = 26;
						g->sSelect.isDifficultyFilterOn = 1;
						g->sSelect.unk5004_difficultycount = g->sSelect.bmsList[g->sSelect.cur_song].difficultyCount + 1;
					}
					else if (flgB) {
						g->config.select.difficulty = diff;
					}
					else if (g->config.select.ignoredifficultyall == 1) {
						g->config.select.difficulty = minDifficulty;
					}
					else {
						g->config.select.difficulty = (g->config.select.difficulty == 0) * minDifficulty;
					}
					g->sSelect.filter_clicked = 4;
					g->sSelect.is_clicked_filter = 1;
					SetBmsFilter(g, sql);
				}
				else {
					SetBmsFilter(g, sql);
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 0;
					SetObjectStrings_SongSelect(g);
				}
			}
			else if (dstd = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_ON[g->sSelect.listSelectedBarFromScreenTop], GetTimeLapse(0, &g->timer1)), //this is all the condition. really????
				((MouseOnDSTD(&dstd, &g->KeyInput.mouse_oldX, &g->KeyInput.mouse_oldY) == 0 || g->KeyInput.mouse_buttonL != 1) && g->KeyInput.inputID[D_DIK_RIGHT] != 1)
				&& (g->KeyInput.inputID[D_DIK_RETURN] != 1 || g->txtStruct.st_text_num != -1 || (GetTimeLapse(4, &g->timer1) != -1.0 && GetTimeLapse(4, &g->timer1) < 200.0)) 
				&& (g->sSelect.panel == 1
					|| ((g->config.select.control != 0
						|| (((g->config.select.buttonselect != 0 || (g->KeyInput.p1_buttonInput[1] != 1 && g->KeyInput.p1_buttonInput[3] != 1 && g->KeyInput.p1_buttonInput[5] != 1 && g->KeyInput.p1_buttonInput[7] != 1 && g->KeyInput.p2_buttonInput[1] != 1 && g->KeyInput.p2_buttonInput[3] != 1 && g->KeyInput.p2_buttonInput[5] != 1 && g->KeyInput.p2_buttonInput[7] != 1)))
							&& (g->config.select.buttonselect != 1 || (g->KeyInput.p1_buttonInput[5] != 1 && g->KeyInput.p1_buttonInput[7] != 1 && g->KeyInput.p2_buttonInput[5] != 1 && g->KeyInput.p2_buttonInput[7] != 1))))
						&& (g->config.select.control != 1 || (g->KeyInput.p1_buttonInput[5] != 1 && g->KeyInput.p1_buttonInput[7] != 1))))) {

				for (int i = 0; i < 30; i++) {
					if (g->skstruct.dst_BAR_BODY_OFF[i].dstCount) {
						dstd = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_OFF[i], GetTimeLapse(0, &g->timer1));
						if (MouseOnDSTD(&dstd, &g->KeyInput.mouse_oldX, &g->KeyInput.mouse_oldY) != 0 && g->sSelect.is_mouseOnTextInput == 0) {
							g->sSelect.is_mouseOnSongBars = 1;
							if (g->sSelect.listSelectedBarFromScreenTop == i)
								g->sSelect.is_mouseOnSelected = 1;

							if (g->KeyInput.mouse_buttonL == 2) {
								if (i < g->skstruct.bar_availabe_from) {
									if (g->sSelect.nowBar == g->sSelect.listTopbar) {
										g->sSelect.oldBar = g->sSelect.listTopbar;
										g->sSelect.nowBar += (i - g->skstruct.bar_availabe_from) * 2000;
										g->sSelect.barMoveStartTime = GetTimeWrap();
										g->sSelect.barMoveEndTime = GetTimeWrap() + 200;
									}
								}
								else if (i < g->skstruct.bar_availabe_to + 1) {
									g->sSelect.listSelectedBarFromScreenTop = i;
								}
								else {
									if (i < g->skstruct.bar_availabe_from) {
										if (g->sSelect.nowBar == g->sSelect.listTopbar) {
											g->sSelect.oldBar = g->sSelect.listTopbar;
											g->sSelect.nowBar += (i - g->skstruct.bar_availabe_from) * 2000;
											g->sSelect.barMoveStartTime = GetTimeWrap();
											g->sSelect.barMoveEndTime = GetTimeWrap() + 200;
										}
									}
									if (i > g->skstruct.bar_availabe_to) {
										if (g->sSelect.nowBar == g->sSelect.listTopbar) {
											g->sSelect.oldBar = g->sSelect.listTopbar;
											g->sSelect.nowBar += (i - g->skstruct.bar_availabe_to) * 2000;
											g->sSelect.barMoveStartTime = GetTimeWrap();
											g->sSelect.barMoveEndTime = GetTimeWrap() + 200;
										}
									}
								}
							}
						}
					}
				}
			}
			else if (g->sSelect.cur < 1000 && (g->sSelect.course.isMakingCourse != 1) || (g->sSelect.bmsList[g->sSelect.cur_song].folderType != 6)) {
				if (g->sSelect.bmsList[g->sSelect.cur_song].folderType == 9) {
					CreateRandomCourse(g, sql, 0);
				}
				else if (g->sSelect.bmsList[g->sSelect.cur_song].folderType == 7) {
					g->sSelect.course.isMakingCourse = 1;
					PlaySound(&g->audio, &g->audio.sysSound.panel_open, g->audio.chnKey, -1);
					SetTimeLapse(17, &g->timer1);
					g->sSelect.course.id = -1;
					g->sSelect.course.name = "コースタイトルを入力して下さい";
					g->sSelect.course.count = 0;
					for (int i = 0; i < 10; i++) {
						InitSongData(&g->sSelect.course.data[i]);
					}
					g->sSelect.course.return_query = g->sSelect.stack_query[1];
					g->sSelect.course.return_filepath = g->sSelect.stack_folderPath[1];
					g->sSelect.course.return_isFolder = g->sSelect.stack_isFolder[1];
					g->sSelect.course.return_searchText = g->sSelect.stack_searchTitle[1];
					g->sSelect.course.return_rivalID = g->sSelect.stack_rivalID[1];
					g->sSelect.course.return_Topbar = g->sSelect.listTopbar;
					SetObjectStrings_SongSelect(g);
					ProcS_Select(g);
					g->sSelect.queryCount = 1;
					g->sSelect.searchFocused = 1;
					g->sSelect.searchType = 4;
					g->sSelect.curQuery[0] = g->sSelect.stack_query[g->sSelect.cur - 1];
					g->sSelect.searchMax = g->sSelect.stack_rivalID[g->sSelect.cur - 1];
					g->sSelect.filterSort = g->config.select.sort;
					g->sSelect.filterKey = g->config.select.key;
					g->sSelect.filterDifficulty = g->config.select.difficulty;
					g->sSelect.selTitle = g->sSelect.bmsList[g->sSelect.cur_song].title;
					g->sSelect.selFilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
					g->sSelect.selKey = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
					g->sSelect.selFolder = g->sSelect.bmsList[g->sSelect.cur_song].folder;
				}
				else if(g->sSelect.bmsList[g->sSelect.cur_song].keymode <= 0 && g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount <= 0){
					g->sSelect.searchType = 1;
					g->sSelect.searchFocused = 1;
					g->sSelect.selTitle = g->sSelect.bmsList[g->sSelect.cur_song].title;
					g->sSelect.selFilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
					g->sSelect.selFolder = &g->sSelect.bmsList[g->sSelect.cur_song].folder;
					g->sSelect.selKey = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
					g->sSelect.searchMax = g->sSelect.bmsList[g->sSelect.cur_song].level;
					g->sSelect.filterSort = g->config.select.sort;
					g->sSelect.filterKey = g->config.select.key;
					g->sSelect.filterDifficulty = g->config.select.difficulty;
					if (g->sSelect.bmsList[g->sSelect.cur_song].tag.length() > 2 && g->sSelect.bmsList[g->sSelect.cur_song].tag.isDiff("(null)")) {
						g->sSelect.queryCount = 3;
						g->sSelect.curQuery[0] = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE %s", g->sSelect.bmsList[g->sSelect.cur_song].tag);
						g->sSelect.curQuery[2] = sqlite3_snprintf(1024, buf, "SELECT * FROM folder WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath));
						g->sSelect.curQuery[1] = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath));
						(g->sSelect).unk4fb8[0] = 0;
						(g->sSelect).unk4fc0 = 1;
						(g->sSelect).unk4fb8[1] = 0;
						(g->sSelect).unk4fc4[0] = 1;
						(g->sSelect).unk4fc4[1] = 0;
						(g->sSelect).unk4fc4[2] = 0;
					}
					else {
						if (g->config.jukebox.autoreload == 1) {
							g->sSelect.reloadType = 1;
						}
						g->sSelect.queryCount = 2;
						g->sSelect.unk4fa4[1] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath));
						g->sSelect.unk4fa4[0] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM song WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath));
						g->sSelect.unk4fb4 = sqlite3_snprintf(1024, buf, "SELECT difficulty,folder,mode,path FROM song WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath));
						g->sSelect.curQuery[1] = sqlite3_snprintf(1024, buf, "SELECT * FROM folder WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath));
						g->sSelect.curQuery[0] = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath));
						g->sSelect.unk4fb8[1] = 1;
						g->sSelect.unk4fb8[0] = 0;
						g->sSelect.unk4fc4[0] = 0;
						g->sSelect.unk4fc4[1] = 0;
						g->sSelect.unk4fc4[2] = 0;
					}
				}
				else {
					if (g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount >= 1 && g->sSelect.bmsList[g->sSelect.cur_song].coursePlayable == 0) return;
					if (g->sSelect.course.isMakingCourse == 1 && g->sSelect.course.count >= 0 && g->sSelect.course.count <= 4) {
						if (g->sSelect.course.count == 0 || g->sSelect.course.data[0].keymode == g->sSelect.bmsList[g->sSelect.cur_song].keymode) {
							PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
							COPY_SONGDATA(&g->sSelect.course.data[g->sSelect.course.count], &g->sSelect.bmsList[g->sSelect.cur_song]);
							SetObjectStrings_SongSelect(g);
							ProcS_Select(g);
							g->sSelect.course.count++;
						}
					}
					else if (g->sSelect.course.isMakingCourse == 1 && g->sSelect.course.count >= 5) {
						if (g->KeyInput.mouse_buttonL == 0) {
							ResetTimeLapse(17, &g->timer1);
							SetTimeLapse(18, &g->timer1);
							PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
							SetTimeLapse(5, &g->timer1);
							g->sSelect.cur = 1;
							g->sSelect.stack_query[1] = g->sSelect.course.return_query;
							g->sSelect.stack_folderPath[1] = g->sSelect.course.return_filepath;
							g->sSelect.stack_isFolder[1] = g->sSelect.course.return_isFolder;
							g->sSelect.stack_searchTitle[1] = g->sSelect.course.return_searchText;
							g->sSelect.stack_rivalID[1] = g->sSelect.course.return_rivalID;
							g->sSelect.is_coursemaking_done = 1;
							g->sSelect.filter_clicked = 6;
							g->sSelect.course.isCourseCreated = 1;
							g->sSelect.course.isMakingCourse = 1;
							SetBmsFilter(g, sql);
						}
					}
					else {
						g->procSelecter = 3;
						g->gameplay.isAutoplay = 0;
					}
					return;
				}
			}
		}

		if (g->KeyInput.mouse_buttonR == 3) {
			if (g->sSelect.is_mouseOnSongBars == 0 && g->sSelect.panel >= 0) {
				ResetTimeLapse(g->sSelect.panel + 20, &g->timer1);
				SetTimeLapse(g->sSelect.panel + 30, &g->timer1);
				g->sSelect.panel = -1;
				PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
			}
			else if (g->sSelect.is_mouseOnSongBars == 0 && g->sSelect.course.isMakingCourse == 1 && g->sSelect.course.count >= 1) {
				PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
				g->sSelect.course.count--;
				InitSongData(&g->sSelect.course.data[g->sSelect.course.count]);
				ProcS_Select(g);
			}
			else if ((g->sSelect.is_mouseOnSongBars || g->sSelect.course.isMakingCourse != 1) && g->sSelect.cur > 0) {
				g->sSelect.searchType = 2;
				g->sSelect.queryCount = 1;
				g->sSelect.searchFocused = 1;
				g->sSelect.curQuery[0] = g->sSelect.stack_query[g->sSelect.cur - 1];
				g->sSelect.searchMax = g->sSelect.stack_rivalID[g->sSelect.cur - 1];
				g->sSelect.filterSort = g->config.select.sort;
				g->sSelect.filterKey = g->config.select.key;
				g->sSelect.filterDifficulty = g->config.select.difficulty;
				g->sSelect.selTitle = g->sSelect.bmsList[g->sSelect.cur_song].title;
				g->sSelect.selFilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
				g->sSelect.selKey = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
				g->sSelect.selFolder = g->sSelect.bmsList[g->sSelect.cur_song].folder;
			}
		}

		if (g->sSelect.panel != 1
			&& (g->KeyInput.inputID[D_DIK_LEFT] == 1
				|| ((g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[2] == 1) && g->config.select.control == 0)
				|| (g->KeyInput.p1_buttonInput[3] == 1 && g->config.select.control == 1))
			&& g->sSelect.cur > 0) {

			g->sSelect.searchType = 2;
			g->sSelect.queryCount = 1;
			g->sSelect.searchFocused = 1;
			g->sSelect.curQuery[0] = g->sSelect.stack_query[g->sSelect.cur - 1];
			g->sSelect.searchMax = g->sSelect.stack_rivalID[g->sSelect.cur + -1];
			g->sSelect.filterSort = g->config.select.sort;
			g->sSelect.filterKey = g->config.select.key;
			g->sSelect.filterDifficulty = g->config.select.difficulty;
			g->sSelect.selTitle = g->sSelect.bmsList[g->sSelect.cur_song].title;
			g->sSelect.selFilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
			g->sSelect.selKey = g->sSelect.bmsList[g->sSelect.cur_song].keymode;
			g->sSelect.selFolder = g->sSelect.bmsList[g->sSelect.cur_song].folder;
		}
	}
}

//4281a0 ProcI_Select
int ProcI_Select(game *g, sqlite3 *sql) {

	int l66c = g->sSelect.listSelectedBarFromScreenTop;

	SubProcI_Select(g, sql);
	g->sSelect.buttonObjClicked = 0;
	if (g->sSelect.course.isCourseCreated == 1) {
		if (g->sSelect.bmsList[g->sSelect.cur_song].coursePlayable == 1) {
			for (int i = 0; i < 10; i++) {
				g->sSelect.course.data[i].hash = g->sSelect.bmsList[g->sSelect.cur_song].courseHash[i];
				g->sSelect.course.data[i].courseStageCount++;
			}
			g->sSelect.course.id = g->sSelect.bmsList[g->sSelect.cur_song].courseID;
			g->sSelect.course.name = g->sSelect.bmsList[g->sSelect.cur_song].title;
		}
		WriteCourse(sql, g->sSelect.course, &g->sSelect.bmsList[g->sSelect.cur_song], g->sSelect.playerPassMD5, g->config.course.defaultconnection, g->config.course.defaultgauge);
		ProcS_Select(g);
		g->sSelect.course.isCourseCreated = 0;
	}
	else if (g->sSelect.course.isChangingTitle == 1) {
		ChangeCourseTitle(sql, g->sSelect.course.name, g->sSelect.bmsList[g->sSelect.cur_song].courseID, g->sSelect.bmsList[g->sSelect.cur_song].courseType);
		SetBmsFilter(g, sql);
		g->sSelect.filter_clicked = 4;
		g->sSelect.course.isChangingTitle = 0;
	}
	else if (g->sSelect.course.isDeletingCourse == 1) {
		DeleteCourse(sql, g->sSelect.bmsList[g->sSelect.cur_song].courseID, g->sSelect.bmsList[g->sSelect.cur_song].courseType);
		SetBmsFilter(g, sql);
		g->sSelect.filter_clicked = 7;
		g->sSelect.course.return_Topbar = g->sSelect.listTopbar;
		g->sSelect.course.isDeletingCourse = 0;
	}

	InitSelectBySearchResult(g, sql);
	if (g->sSelect.searchFocused == 0) {
		int barp = ChangeValueByTime(g->sSelect.oldBar, g->sSelect.nowBar, g->sSelect.barMoveStartTime, g->sSelect.barMoveEndTime, GetTimeWrap(), 0);
		int barb = g->sSelect.listCalculatedBar;
		g->sSelect.listTopbar = barp;
		g->sSelect.listCalculatedBar = barp;

		while (g->sSelect.listCalculatedBar < 0) 
			g->sSelect.listCalculatedBar += g->sSelect.bmsListCount * 1000;
		
		while (g->sSelect.listCalculatedBar >= g->sSelect.bmsListCount * 1000) 
			g->sSelect.listCalculatedBar -= g->sSelect.bmsListCount * 1000;

		if (g->sSelect.cur_song != GetSongCursor(g) || l66c != g->sSelect.listSelectedBarFromScreenTop) {
			g->sSelect.cur_song = GetSongCursor(g);
			if (GetTimeLapse(1, &g->timer1) > 0.0) {
				if (g->net.rankingData.showRanking == 0) SetTimeLapse(11, &g->timer1);
				if (g->sSelect.is_loading_bmslist == 0) PlaySound(&g->audio, &g->audio.sysSound.scratch, g->audio.chnKey, -1);
				else g->sSelect.is_loading_bmslist = 0;
			}
			if (g->net.rankingData.showRanking == 0) ProcS_Select(g);
			else SetObjectString(1, g->sSelect.bmsList[g->sSelect.cur_song].fulltitle, g->txtStruct.objectStr);
		}
		g->sSelect.is_loading_bmslist = 0;
		if (barb == g->sSelect.listCalculatedBar) {
			ResetTimeLapse(10, &g->timer1);
		}
		if (barb != g->sSelect.listCalculatedBar) {
			if (GetTimeLapse(10, &g->timer1)) {
				SetTimeLapse(10, &g->timer1);
			}
		}
	}

	for (int i = 0; i < 5; i++) {
		if (g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[i] != g->sSelect.levelIndicatorAnimation[i]) {
			g->sSelect.levelIndicatorAnimation[i] = ChangeValueByTime(g->sSelect.levelBarGraph[i], g->sSelect.levelsOfSong[i], 0, 100.0, GetTimeLapse(11, &g->timer1), 0);
		}
	}

	int nowtime = GetNowUnixtime();

	for (int i = 1; i < 30; i++) {
		if (g->skstruct.dst_BAR_BODY_OFF[i - 1].dstCount > 0 && g->skstruct.dst_BAR_BODY_OFF[i].dstCount > 0) {
			DSTdraw dstd1, dstd2, dstd3;
			InitDSTdraw(&dstd1);
			InitDSTdraw(&dstd2);
			InitDSTdraw(&dstd3);

			int bar = (g->sSelect.listCalculatedBar / 1000 - g->skstruct.BAR_CENTER) + i;
			while (bar < 0){
				bar += g->sSelect.bmsListCount;
			}
			bar = bar % g->sSelect.bmsListCount;

			if (i == g->sSelect.listSelectedBarFromScreenTop) {
				dstd1 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_ON[i], GetTimeLapse(0, &g->timer1));
			}
			else {
				dstd1 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_OFF[i], GetTimeLapse(0, &g->timer1));
			}
			
			if (i - 1 == g->sSelect.listSelectedBarFromScreenTop) {
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_ON[i-1], GetTimeLapse(0, &g->timer1));
			}
			else {
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_OFF[i-1], GetTimeLapse(0, &g->timer1));
			}

			if (g->sSelect.listCalculatedBar % 1000 == 0) {
				dstd3 = dstd1;
			}
			else {
				dstd3 = DSTDbyTime(&dstd1, &dstd2, 0, 1000.0, g->sSelect.listCalculatedBar % 1000);
			}
			
			if (GetTimeLapse(176, &g->timer1) != -1.0) {
				dstd3.x += ChangeValueByTime(300.0, 0.0, 0.0, 120.0, GetTimeLapse(176, &g->timer1), 0);
				dstd1.x += ChangeValueByTime(300.0, 0.0, 0.0, 120.0, GetTimeLapse(176, &g->timer1), 0);

			}
			else if (GetTimeLapse(175, &g->timer1) != -1.0) {
				dstd3.x += ChangeValueByTime(0.0, 300.0, 0.0, 120.0, GetTimeLapse(175, &g->timer1), 0);
				dstd1.x += ChangeValueByTime(0.0, 300.0, 0.0, 120.0, GetTimeLapse(175, &g->timer1), 0);
			}
			if (dstd1.time != -1 && dstd2.time != -1) {
				AddDrawingBuffer_Lunaris(&g->skstruct.drBuf, &g->skstruct.src_BAR_BODY[g->sSelect.bmsList[bar].folderType], &dstd3, &g->timer1);
				if (i == g->sSelect.listSelectedBarFromScreenTop) {
					AddDrawingBuffer_Object(&g->skstruct.drBuf, &g->skstruct.src_BAR_FLASH, &g->skstruct.dst_BAR_FLASH, &g->timer1, dstd1.x, dstd1.y);
				}

				if (nowtime - g->sSelect.bmsList[bar].adddate < g->config.select.titleflash * 3600 && g->sSelect.bmsList[bar].folderType != 4) {
					AddDrawingBuffer_TextXY(&g->skstruct.drBuf, &g->skstruct.src_BAR_TITLE[1], &g->skstruct.dst_BAR_TITLE[1], &g->timer1, bar + 10000, dstd3.x, dstd3.y);
				}
				else {
					AddDrawingBuffer_TextXY(&g->skstruct.drBuf, &g->skstruct.src_BAR_TITLE[0], &g->skstruct.dst_BAR_TITLE[0], &g->timer1, bar + 10000, dstd3.x, dstd3.y);
				}

				if (g->sSelect.bmsList[bar].folderType == 0 || g->sSelect.bmsList[bar].folderType == 5) {
					//logic arranged. potential of misinterpretation
					if (g->sSelect.bmsList[bar].keymode > 0) {
						if (g->net.rankingData.showRanking == 1) 
							AddDrawingBuffer_Numbers(&g->skstruct.drBuf, &g->skstruct.src_BAR_LEVEL[6], &g->skstruct.dst_BAR_LEVEL[6], &g->timer1, g->sSelect.bmsList[bar].level, dstd3.x, dstd3.y);
						
						else if (g->config.select.disabledifficultyfilter == 1 && g->net.rankingData.showRanking == 0) {
							if (g->sSelect.bmsList[bar].exlevel > 0 && g->config.jukebox.customfolder & 0x80 != 0) 
								AddDrawingBuffer_Numbers(&g->skstruct.drBuf, &g->skstruct.src_BAR_LEVEL[5], &g->skstruct.dst_BAR_LEVEL[5], &g->timer1, g->sSelect.bmsList[bar].exlevel, dstd3.x, dstd3.y);
							
							else 
								AddDrawingBuffer_Numbers(&g->skstruct.drBuf, &g->skstruct.src_BAR_LEVEL[0], &g->skstruct.dst_BAR_LEVEL[0], &g->timer1, g->sSelect.bmsList[bar].level, dstd3.x, dstd3.y);
						}
						else if (g->sSelect.isExLevel != 0 && g->net.rankingData.showRanking == 0) 
							AddDrawingBuffer_Numbers(&g->skstruct.drBuf, &g->skstruct.src_BAR_LEVEL[5], &g->skstruct.dst_BAR_LEVEL[5], &g->timer1, g->sSelect.bmsList[bar].exlevel, dstd3.x, dstd3.y);

						else if (g->sSelect.bmsList[bar].difficulty >= 0 && g->sSelect.bmsList[bar].folderType <= 5 && g->config.select.disabledifficultyfilter == 0) 
							AddDrawingBuffer_Numbers(&g->skstruct.drBuf, &g->skstruct.src_BAR_LEVEL[g->sSelect.bmsList[bar].difficulty], &g->skstruct.dst_BAR_LEVEL[g->sSelect.bmsList[bar].difficulty], &g->timer1, g->sSelect.bmsList[bar].level, dstd3.x, dstd3.y);

						else 
							AddDrawingBuffer_Numbers(&g->skstruct.drBuf, &g->skstruct.src_BAR_LEVEL[0], &g->skstruct.dst_BAR_LEVEL[0], &g->timer1, g->sSelect.bmsList[bar].level, dstd3.x, dstd3.y);
					}
				}

				if (g->sSelect.bmsList[bar].rivalRecord.stat_exscore < 1 || g->net.rankingData.showRanking != 0) {
					if (g->net.rankingData.showRanking == 1) {
						AddDrawingBuffer_Object(&g->skstruct.drBuf, &g->skstruct.src_BAR_LAMP[g->sSelect.bmsList[bar].rivalRecord.clear], &g->skstruct.dst_BAR_LAMP[g->sSelect.bmsList[bar].rivalRecord.clear], &g->timer1, dstd3.x, dstd3.y);
					}
					else {
						int t = g->config.play.battle;
						if (g->config.play.battle == 3) {
							t = g->sSelect.bmsList[bar].mybest.clear_sd;
						}
						else if (g->config.play.battle == 2) {
							t = g->sSelect.bmsList[bar].mybest.clear_db;
						}
						else if (g->config.play.is_extra == 1) {
							t = g->sSelect.bmsList[bar].mybest.clear_ex;
						}
						else {
							t = g->sSelect.bmsList[bar].mybest.clear;
						}
						AddDrawingBuffer_Object(&g->skstruct.drBuf, &g->skstruct.src_BAR_LAMP[t], &g->skstruct.dst_BAR_LAMP[t], &g->timer1, dstd3.x, dstd3.y);
					}
				}
				else {
					int t = g->config.play.battle;
					int r = 0;
					if (g->config.play.battle == 3) {
						t = g->sSelect.bmsList[bar].mybest.clear_sd;
					}
					else if (g->config.play.battle == 2) {
						t = g->sSelect.bmsList[bar].mybest.clear_db;
					}
					else if (g->config.play.is_extra == 1) {
						t = g->sSelect.bmsList[bar].mybest.clear_ex;
					}
					else {
						t = g->sSelect.bmsList[bar].mybest.clear;
						r = g->sSelect.bmsList[bar].rivalRecord.clear;
					}
					AddDrawingBuffer_Object(&g->skstruct.drBuf, &g->skstruct.src_BAR_MY_LAMP[t], &g->skstruct.dst_BAR_MY_LAMP[t], &g->timer1, dstd3.x, dstd3.y);
					AddDrawingBuffer_Object(&g->skstruct.drBuf, &g->skstruct.src_BAR_RIVAL_LAMP[r], &g->skstruct.dst_BAR_RIVAL_LAMP[r], &g->timer1, dstd3.x, dstd3.y);
				}

				if (g->net.rankingData.showRanking == 1 
					&& (g->sSelect.bmsList[bar].folderType == 0 || g->sSelect.bmsList[bar].folderType == 5) 
					&& (g->sSelect.bmsList[bar].rivalRecord.rank >= 1 && g->sSelect.bmsList[bar].rivalRecord.rank <= 9)) {

					AddDrawingBuffer_Object(&g->skstruct.drBuf, &g->skstruct.src_BAR_RANK[g->sSelect.bmsList[bar].rivalRecord.rank], &g->skstruct.dst_BAR_RANK[g->sSelect.bmsList[bar].rivalRecord.rank], &g->timer1, dstd3.x, dstd3.y);
				}
				else if (g->sSelect.bmsList[bar].rivalRecord.stat_exscore >= 1 && g->net.rankingData.showRanking == 0) {
					int t = 5;
					if (g->sSelect.bmsList[bar].mybest.stat_exscore == 0) t = 3;
					else if (g->sSelect.bmsList[bar].mybest.stat_exscore > g->sSelect.bmsList[bar].rivalRecord.stat_exscore) t = 0;
					else if (g->sSelect.bmsList[bar].mybest.stat_exscore < g->sSelect.bmsList[bar].rivalRecord.stat_exscore) t = 1;
					else if (g->sSelect.bmsList[bar].mybest.stat_exscore == g->sSelect.bmsList[bar].rivalRecord.stat_exscore) t = 2;

					AddDrawingBuffer_Object(&g->skstruct.drBuf, &g->skstruct.src_BAR_RIVAL[t], &g->skstruct.dst_BAR_RIVAL[t], &g->timer1, dstd3.x, dstd3.y);
				}
			}
		}
	}

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start && g->sSelect.panel > -1) {
		if (GetTimeLapse(20 + g->sSelect.panel, &g->timer1) == -1) {
			SetTimeLapse(20 + g->sSelect.panel, &g->timer1);
			PlaySound(&g->audio, &g->audio.sysSound.panel_open, g->audio.chnKey, -1);
		}
	}

	if(g->config.select.preview == 1){
		if (GetTimeLapse(11, &g->timer1) >= 500.0 && g->gameplay.previewStatus == 0 && g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5 && g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount == 0 && g->gameplay.hThreadPreview == 0) {
			g->gameplay.flag_closingPhase = 1;
			g->gameplay.isPreviewLoad = 0;
			g->gameplay.previewStatus = 1;
			g->gameplay.previewBMShash = g->sSelect.bmsList[g->sSelect.cur_song].hash;
			g->gameplay.previewBMSfilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
			g->gameplay.hThreadPreview = (HANDLE)_beginthread((void(*)(void*))LoadPreview, 0, g);
		}
		else if (g->gameplay.previewStatus) {
			if (g->gameplay.previewBMShash.isDiff(g->sSelect.bmsList[g->sSelect.cur_song].hash)) {
				g->gameplay.flag_closingPhase = 1;
				g->gameplay.isPreviewLoad = 0;
				g->gameplay.flag_gameinput = 0;
				for (int i = 0; i < 6480; i++) {
					StopSound(&g->audio, &g->gameplay.keysound[i]);
				}
				g->gameplay.previewStatus = 0;
				SetFadePreview(&g->audio, 1000, 0);
				g->gameplay.previewBMShash = g->sSelect.bmsList[g->sSelect.cur_song].hash;
				ErrorLogFmtAdd("プレビュー終了\n");
			}
		}
	}
	g->sSelect.is_mouseOnTextInput = 0;
	return 1;
}

//42b610
int ProcGame(game *g) {

	double oldt142;

	if (GetTimeWrap() < g->gameplay.timetick + 2) return 1;
	g->gameplay.timetick = GetTimeWrap();
	if (GetTimeLapse(160, &g->timer1) < 0.0 && g->is_starter == 0 && g->gameplay.isPreviewLoad == 0) {
		SetTimeLapse(160, &g->timer1);
		PlaySound(&g->audio,&g->gameplay.muon,g->audio.chnBgm,-1);
		ResetPressCount(&g->KeyInput);
	}
	g->gameplay.procGameCallCount++;
	SoundGetCurrentTime(&g->audio, &g->gameplay.muon); //anti cheat
	NONE_004b6770();
	bool fx = false;
	double t142 = GetTimeLapse(142, &g->timer1);
	double t41 = GetTimeLapse(41, &g->timer1);
	for (int i = 0; i < 5; i++) {
		if (g->gameplay.songStartTime[i] > 0) {
			if (g->gameplay.songEndTime[i] > 0 && g->gameplay.songStartTime[i] - 100 <= t41 && t41 <= g->gameplay.songEndTime[i] + 100) {
				double vol = ChangeValueByTime(0.0, 1.0, g->gameplay.songStartTime[i], g->gameplay.songEndTime[i], t41, 0);
				g->audio.param.stageBgmVolume[i] = vol;
				g->audio.param.stageKeyVolume[i] = vol;
				fx = true;
			}
		}

		if (g->gameplay.fadeStartTime[i] > 0) {
			if (g->gameplay.fadeStartTime[i] > 0 && g->gameplay.fadeEndTime[i] > 0 && g->gameplay.fadeStartTime[i] - 100 <= t41 && t41 <= g->gameplay.fadeEndTime[i] + 100) {
				double vol = ChangeValueByTime(1.0, 0.0, g->gameplay.fadeStartTime[i], g->gameplay.fadeEndTime[i], t41, 0);
				g->audio.param.stageBgmVolume[i] = vol;
				g->audio.param.stageKeyVolume[i] = vol;
				fx = true;
			}
		}
	}
	if (fx) ApplySoundFX(&g->audio, 0, 0);

	for (int i = 0; i < 5; i++) {
		if (g->gameplay.connection_unkA[i] > 0 && g->gameplay.connection_unkB[i] > 0 && g->gameplay.connection_unkA[i] - 100 <= t41 && t41 <= g->gameplay.connection_unkB[i] + 100) {
			g->gameplay.stageMixer[i] = ChangeValueByTime(0.0, 100.0, g->gameplay.connection_unkA[i], g->gameplay.connection_unkB[i], t41, 0);
		}
		if (g->gameplay.connection_unkC[i] > 0 && g->gameplay.connection_unkD[i] > 0 && g->gameplay.connection_unkC[i] - 100 <= t41 && t41 <= g->gameplay.connection_unkD[i] + 100) {
			g->gameplay.stageMixer[i] = ChangeValueByTime(0.0, 100.0, g->gameplay.connection_unkC[i], g->gameplay.connection_unkD[i], t41, 0);
		}
	}
		
	while (true) {

		if (g->gameplay.bmsobj.notes[g->gameplay.bmsobj.note_count].realTiming >= t142 || g->gameplay.bpmChangedBmstime >= 0) {
			SoundGetCurrentTime(&g->audio, &g->gameplay.muon); //anti cheat
			NONE_004b6770();
			t142 = GetTimeLapse(142, &g->timer1);
			if (g->gameplay.isPreviewLoad == '\0') {
				ReactInput(g);
			}

			if (g->is_starter || (g->procSelecter == 4 && g->procPhase != 2 && g->procPhase != 3)) {

				if ((g->gameplay.player[0].HP >= 2.0 || g->config.play.battle == 1) && (g->gameplay.player[0].HP >= 2.0 || g->gameplay.player[1].HP >= 2.0 || g->config.play.battle != 1) || g->gameplay.isPreviewLoad) {

					if (g->gameplay.isAutoplay == 0 && g->config.play.m_lunaris == 0 && g->gameplay.isPreviewLoad == 0) {
						oldt142 = t142;
						if (g->gameplay.bpmChangedRealtime > 0) {
							t142 = g->gameplay.bpmChangedRealtime * 2 - t142;
						}

						for (int i = 0; i < 10; i++) {
							ProcNoteOnTiming(g, i, g->KeyInput.p1_buttonInput[i], t142, 0);
						}
						if (g->gameplay.ghostBattle == 0) {
							for (int i = 10; i < 20; i++) {
								ProcNoteOnTiming(g, i, g->KeyInput.p2_buttonInput[i - 10], t142, g->config.play.battle == 1);
							}
						}
						if (g->gameplay.bpmChangedRealtime > 0) {
							t142 = oldt142;
						}
					}

					if (g->config.play.m_lunaris == 0 && g->gameplay.isAutoplay == 0 && g->gameplay.replay.status != 2 && g->config.play.autojudge > 0 && g->config.play.battle != 1 && g->gameplay.autojudge_midcount > 9) {

						if (g->gameplay.autojudge_midsum > 0) g->config.play.judgetiming++;
						else if (g->gameplay.autojudge_midsum < 0) g->config.play.judgetiming--;

						g->gameplay.autojudge_midcount = 0;
						g->gameplay.autojudge_midsum = 0;
					}

					for (int p = 0; p < 2; p++) {
						if (g->gameplay.player[p].HP_unk != g->gameplay.player[p].HP) {
							g->gameplay.player[p].HP_old = g->gameplay.player[p].HP_print;
							g->gameplay.player[p].time_oldHP = GetTimeWrap();
							g->gameplay.player[p].time_newHP = (int)GetTimeWrap() + abs((int)(g->gameplay.player[p].HP_old - g->gameplay.player[p].HP)) * 10;
						}
						g->gameplay.player[p].HP_unk = g->gameplay.player[p].HP;
						if (g->gameplay.player[p].score != g->gameplay.player[p].score_unk) {
							g->gameplay.player[p].score_old = g->gameplay.player[p].score_print;
							g->gameplay.player[p].time_oldScore = GetTimeWrap();
							g->gameplay.player[p].time_newScore = GetTimeWrap() + 500;
						}
						g->gameplay.player[p].score_unk = g->gameplay.player[p].score;

						g->gameplay.player[p].HP_print = ChangeValueByTime(g->gameplay.player[p].HP_old, g->gameplay.player[p].HP, g->gameplay.player[p].time_oldHP, g->gameplay.player[p].time_newHP, GetTimeWrap(), 0);
						g->gameplay.player[p].score_print = ChangeValueByTime(g->gameplay.player[p].score_old, g->gameplay.player[p].score, g->gameplay.player[p].time_oldScore, g->gameplay.player[p].time_newScore, GetTimeWrap(), 0);
					}

					LogGraphPlayData(&g->gameplay.statgraph[0], &g->gameplay.player[0], t142, g->gameplay.song_runtime);
					if (g->config.play.battle == 1 || g->gameplay.ghostBattle) LogGraphPlayData(&g->gameplay.statgraph[1], &g->gameplay.player[1], t142, g->gameplay.song_runtime);

					LogGraphData(&g->gameplay.rategraph[0], g->gameplay.highScore.rate, t142, g->gameplay.song_runtime);
					LogGraphData(&g->gameplay.rategraph[1], g->gameplay.targetScore.rate, t142, g->gameplay.song_runtime);

					if (g->gameplay.isAutoplay == 0 && g->is_starter == 0 && g->gameplay.isPreviewLoad == 0) {
						int t160 = GetTimeLapse(160, &g->timer1);
						if (t160 >= 5000) {
							g->gameplay.delayCheckCount++;

							if (SoundGetCurrentTime(&g->audio, &g->gameplay.muon) / g->gameplay.freqSpeedMultiplier - t160 >= 800.0 || (uint)g->gameplay.muon.length <= 6000) {
								g->gameplay.delayDetectedCount++;
							}
							SetTimeLapse(160, &g->timer1);
							PlaySound(&g->audio, &g->gameplay.muon, g->audio.chnBgm, -1);
						}
					}
					return 1;
				}
				else {
					SetTimeLapse(3, &g->timer1);
					g->procPhase = 3;
					for (int i = 0; i < 6480; i++) {
						StopSound(&g->audio, &g->gameplay.keysound[i]);
					}
					if (g->rec.recMode == 2) {
						RecordFadeout(&g->audio, GetTimeWrap(), 10.0);
					}
					PlaySound(&g->audio, &g->audio.sysSound.stop, g->audio.chnBgm, -1);
				}
			}
			break;
		}

		int op = g->gameplay.bmsobj.notes[g->gameplay.bmsobj.note_count].op;
		int val = g->gameplay.bmsobj.notes[g->gameplay.bmsobj.note_count].val;
		int stage = g->gameplay.bmsobj.notes[g->gameplay.bmsobj.note_count].stage;
		if (op < 50) {
			switch (op) {
			case 1:
				if (g->rec.recMode != 2) {
					PlaySound(&g->audio, &g->gameplay.keysound[val], g->audio.chnStageBgm[stage], stage);
				}
				break;
			case 2:
				g->timer1.clock[140] = 0.0;
				g->timer1.Rhythm = 0.0;
				break;
			case 3:
			case 8:
				g->gameplay.BPM = abs(val);
				if (val < 0) {
					g->gameplay.bpmChangedBmstime = RealTimeToBMSTime(&g->gameplay, (double)g->config.play.judgetiming + GetTimeLapse(41, &g->timer1));
					g->gameplay.bpmChangedRealtime = t142;
				}
				break;
			case 4:
				if (g->gameplay.courseType == 1) {
					g->gameplay.courseBgaLayer1[stage] = val;
					g->gameplay.isBgaPlaying = 1;
					if (1 <= val && val < 6479) {
						SeekMovieToGraph(g->gameplay.bgaHandle[val], 0);
						if (val == 1295 && g->gameplay.bgaHandle[1295] != -1 && g->gameplay.soundonly) {
							PlayMovieToGraph(g->gameplay.bgaHandle[1295], 3, 0);
						}
						else {
							PlayMovieToGraph(g->gameplay.bgaHandle[val], 1, 0);
						}
						g->gameplay.courseLayer1ChangeTime[stage] = GetTimeWrap();
						SetMovieVolumeToGraph(0, g->gameplay.bgaHandle[val]);
					}
				}
				else {
					g->gameplay.bgaLayer1 = val;
					g->gameplay.isBgaPlaying = 1;
					if (1 <= val && val < 6479) {
						SeekMovieToGraph(g->gameplay.bgaHandle[val], 0);
						if (val == 1295 && g->gameplay.bgaHandle[1295] != -1 && g->gameplay.soundonly) {
							PlayMovieToGraph(g->gameplay.bgaHandle[1295], 3, 0);
						}
						else {
							PlayMovieToGraph(g->gameplay.bgaHandle[val], 1, 0);
						}
						g->gameplay.layer1ChangeTime = GetTimeWrap();
						SetMovieVolumeToGraph(0, g->gameplay.bgaHandle[val]);
					}
				}
				break;
			case 5:
			case 9:
				break;
			case 6:
				if (g->gameplay.courseType == 1) {
					g->gameplay.courseMissLayer[stage] = val;
					g->gameplay.isBgaPlaying = 1;
				}
				else {
					g->gameplay.missLayer = val;
					g->gameplay.isBgaPlaying = 1;
				}
				break;
			case 7:
				if (g->gameplay.courseType == 1) {
					g->gameplay.courseBgaLayer2[stage] = val;
					g->gameplay.isBgaPlaying = 1;
					if (1 <= val && val < 6479) {
						SeekMovieToGraph(g->gameplay.bgaHandle[val], 0);
						PlayMovieToGraph(g->gameplay.bgaHandle[val], 1, 0);
						g->gameplay.layer2ChangeTime = GetTimeWrap();
						SetMovieVolumeToGraph(0, g->gameplay.bgaHandle[val]);
					}
				}
				else {
					g->gameplay.bgaLayer2 = val;
					g->gameplay.isBgaPlaying = 1;
					if (1 <= val && val < 6479) {
						SeekMovieToGraph(g->gameplay.bgaHandle[val], 0);
						PlayMovieToGraph(g->gameplay.bgaHandle[val], 1, 0);
						g->gameplay.layer2ChangeTime = GetTimeWrap();
						SetMovieVolumeToGraph(0, g->gameplay.bgaHandle[val]);
					}
				}
				break;
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
				if ((g->gameplay.isAutoplay == 0 && g->config.play.m_lunaris == 0 && g->gameplay.isPreviewLoad == 0) || val <= 0) {
					if (g->gameplay.bmsobj_note[op - 10].autoplay && val > 0) {
						PlaySound(&g->audio, &g->gameplay.keysound[val], g->audio.chnStageKey[stage], stage);
						g->gameplay.bmsobj_note[op - 10].noteVal = val;
					}
				}
				else {
					PlaySound(&g->audio, &g->gameplay.keysound[val], g->audio.chnStageKey[stage], stage);
					g->gameplay.bmsobj_note[op - 10].noteVal = val;
					if (g->config.play.m_lunaris) {
						g->gameplay.player[0].note_current++;
						g->gameplay.player[0].note_current2++;
					}
				}
				break;
			default:
				g->gameplay.bmsobj_note[op - 30].noteVal = val;
			}
		}
		else if (op == 1001) {

			switch (val) {
			case 0:
				g->gameplay.player[0].judgetime[5] = 12;
				g->gameplay.player[0].judgetime[4] = 24;
				g->gameplay.player[0].judgetime[3] = 60;
				g->gameplay.player[0].judgetime[2] = 200;
				g->gameplay.player[0].judgetime[1] = 1000;
				break;
			case 1:
				g->gameplay.player[0].judgetime[5] = 15;
				g->gameplay.player[0].judgetime[4] = 30;
				g->gameplay.player[0].judgetime[3] = 80;
				g->gameplay.player[0].judgetime[2] = 200;
				g->gameplay.player[0].judgetime[1] = 1000;
				break;
			default:
				g->gameplay.player[0].judgetime[5] = 18;
				g->gameplay.player[0].judgetime[4] = 40;
				g->gameplay.player[0].judgetime[3] = 100;
				g->gameplay.player[0].judgetime[2] = 200;
				g->gameplay.player[0].judgetime[1] = 1000;
				break;
			case 3:
				g->gameplay.player[0].judgetime[5] = 21;
				g->gameplay.player[0].judgetime[4] = 60;
				g->gameplay.player[0].judgetime[3] = 120;
				g->gameplay.player[0].judgetime[2] = 200;
				g->gameplay.player[0].judgetime[1] = 1000;
				break;
			}
			if (g->config.play.m_gambol == 1) {
				g->gameplay.player[0].judgetime[5] = 12;
				g->gameplay.player[0].judgetime[4] = 24;
				g->gameplay.player[0].judgetime[3] = 60;
				g->gameplay.player[0].judgetime[2] = 200;
				g->gameplay.player[0].judgetime[1] = 1000;
			}
			else if (g->config.play.m_gambol == 2) {
				g->gameplay.player[0].judgetime[5] = 12;
				g->gameplay.player[0].judgetime[4] = 12;
				g->gameplay.player[0].judgetime[3] = 12;
				g->gameplay.player[0].judgetime[2] = 200;
				g->gameplay.player[0].judgetime[1] = 1000;
			}
		}
		else if (op == 1002 && stage < 10) {
			g->gameplay.courseStageNow = stage;
			CSTR tmp;

			if (stage + 1 == g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount) {
				cstrSprintf(&tmp, "%s", g->sSelect.bmsList[g->sSelect.cur_song].courseTitle[stage]);
			}
			else {
				cstrSprintf(&tmp, "%s", g->sSelect.bmsList[g->sSelect.cur_song].courseTitle[stage]);
			}
			SetObjectString(10, tmp, g->txtStruct.objectStr);
		}

		g->gameplay.bmsobj.note_count++;

		if (g->gameplay.bmsobj.size <= g->gameplay.bmsobj.note_count && g->gameplay.replay.status != 2) {
			break;
		}
		if (g->gameplay.song_runtime < t142) {
			ErrorLogFmtAdd("endtime\n");
			break;
		}
		if (g->is_starter) {
			if (g->KeyInput.inputID[D_DIK_ESCAPE] == 2 || (g->KeyInput.p1_buttonInput[12] == 2 && g->KeyInput.p1_buttonInput[13] == 2)) {
				for (int i = 0; i < 6480; i++) {
					StopSound(&g->audio, &g->gameplay.keysound[i]);
				}
				break;
			}
		}

		if (g->is_starter == 0) {
			if (g->procSelecter != 4 || g->procPhase == 2 || g->procPhase == 3) {
				break;
			}
		}

		if ((g->gameplay.player[0].HP < 2.0 && g->config.play.battle != 1 && g->gameplay.ghostBattle == 0) || (g->gameplay.player[0].HP < 2.0 && g->gameplay.player[1].HP < 2.0 && (g->config.play.battle == 1 || g->gameplay.ghostBattle)) && (g->gameplay.isPreviewLoad == 0)) {
			SetTimeLapse(3, &g->timer1);
			g->procPhase = 3;
			for (int i = 0; i < 6480; i++) {
				StopSound(&g->audio, &g->gameplay.keysound[i]);
			}
			if (g->rec.recMode == 2) {
				RecordFadeout(&g->audio, GetTimeWrap(), 10.0);
			}
			PlaySound(&g->audio, &g->audio.sysSound.stop, g->audio.chnBgm, -1);
			break;
		}
	}

	if (g->gameplay.isAutoplay == 0 && g->config.play.m_lunaris == 0 && g->gameplay.isPreviewLoad == 0) {
		oldt142 = t142;
		if (g->gameplay.bpmChangedRealtime > 0) {
			t142 = g->gameplay.bpmChangedRealtime * 2 - t142;
		}

		for (int i = 0; i < 10; i++) {
			ProcNoteOnTiming(g, i, g->KeyInput.p1_buttonInput[i], t142, 0);
		}
		if (g->gameplay.ghostBattle == 0) {
			for (int i = 10; i < 20; i++) {
				ProcNoteOnTiming(g, i, g->KeyInput.p2_buttonInput[i - 10], t142, g->config.play.battle == 1);
			}
		}
		if (g->gameplay.bpmChangedRealtime > 0) {
			t142 = oldt142;
		}
	}

	if (g->procSelecter != 2) {
		if (g->is_recordmode == 0 || g->rec.recMode == 2) {
			ReactInput(g);

			for (int p = 0; p < 2; p++) {
				if (g->gameplay.player[p].HP_unk != g->gameplay.player[p].HP) {
					g->gameplay.player[p].HP_old = g->gameplay.player[p].HP_print;
					g->gameplay.player[p].time_oldHP = GetTimeWrap();
					g->gameplay.player[p].time_newHP = (int)GetTimeWrap() + abs((int)(g->gameplay.player[p].HP_old - g->gameplay.player[p].HP)) * 10;
				}
				g->gameplay.player[p].HP_unk = g->gameplay.player[p].HP;
				if (g->gameplay.player[p].score != g->gameplay.player[p].score_unk) {
					g->gameplay.player[p].score_old = g->gameplay.player[p].score_print;
					g->gameplay.player[p].time_oldScore = GetTimeWrap();
					g->gameplay.player[p].time_newScore = GetTimeWrap() + 500;
				}
				g->gameplay.player[p].score_unk = g->gameplay.player[p].score;

				g->gameplay.player[p].HP_print = ChangeValueByTime(g->gameplay.player[p].HP_old, g->gameplay.player[p].HP, g->gameplay.player[p].time_oldHP, g->gameplay.player[p].time_newHP, GetTimeWrap(), 0);
				g->gameplay.player[p].score_print = ChangeValueByTime(g->gameplay.player[p].score_old, g->gameplay.player[p].score, g->gameplay.player[p].time_oldScore, g->gameplay.player[p].time_newScore, GetTimeWrap(), 0);
			}

			LogGraphPlayData(&g->gameplay.statgraph[0], &g->gameplay.player[0], t142, g->gameplay.song_runtime);
			if(g->config.play.battle == 1)
				LogGraphPlayData(&g->gameplay.statgraph[1], &g->gameplay.player[1], t142, g->gameplay.song_runtime);
			LogGraphData(&g->gameplay.rategraph[0], g->gameplay.highScore.rate, t142, g->gameplay.song_runtime);
			LogGraphData(&g->gameplay.rategraph[1], g->gameplay.targetScore.rate, t142, g->gameplay.song_runtime);
			if (g->procPhase == 1) {
				SetTimeLapse(2, &g->timer1);
				g->procPhase = 2;
			}
			for (int p = 0; p < 2; p++) {
				if (g->gameplay.player[p].totalnotes > g->gameplay.player[p].note_current) g->gameplay.player[p].HP = 0;
			}
			LogGraphPlayerDataToEnd(&g->gameplay.statgraph[0], &g->gameplay.player[0]);
			if(g->config.play.battle == 1)
				LogGraphPlayerDataToEnd(&g->gameplay.statgraph[1], &g->gameplay.player[1]);
			for (int i = g->gameplay.rategraph[0].cursor; i < 1000; i++) {
				g->gameplay.rategraph[0].val[i] = g->gameplay.highScore.rate;
			}
			for (int i = g->gameplay.rategraph[1].cursor; i < 1000; i++) {
				g->gameplay.rategraph[1].val[i] = g->gameplay.targetScore.rate;
			}

			if (g->gameplay.player[0].totalnotes > 0 && g->config.play.battle == 0) {
				if (g->gameplay.targetType == 1) 
					g->gameplay.highScore.SetScore(&g->gameplay.player[1], 1);
				else if (g->gameplay.targetType == 2) 
					g->gameplay.targetScore.SetScore(&g->gameplay.player[1], 1);
			}
			g->gameplay.flag_threadExist = 0;
			g->gameplay.flag_gameinput = 0;
			if (g->is_starter) {
				g->po4_23da8 = 1;
				g->po4flagSceneStart = 1;
				if (g->gameplay.isAutoplay == 0 && g->config.play.autojudge == 0 && g->gameplay.player[0].exscore > 0) {
					SetTimeLapse(411, &g->timer1);
					g->gameplay.player[0].clearType = 1;
					if(g->gameplay.player[0].totalnotes == g->gameplay.player[0].max_combo)
						g->gameplay.player[0].clearType = 5;
					else if (g->gameplay.player[0].note_current == g->gameplay.player[0].totalnotes && g->gameplay.player[0].HP >= 80.0) {
						if(g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] < 10) //TOFIX: BP under 10 is considered as hard clear, but not working. now, do we want it? // it's starter mode or plugout4 code, don't mind it
							g->gameplay.player[0].clearType = 4;
						g->gameplay.player[0].clearType = 3;
					}
				}
				else {
					ResetTimeLapse(41, &g->timer1);
					ResetTimeLapse(140, &g->timer1);
					SetTimeLapse(412, &g->timer1);
					g->po4sceneFadeout = g->skstruct.event_FADEOUT[5];
					g->po4sceneTimerID = 412;
					g->po4nextProc = 5;
					g->po4sceneTimerIDNext = 404;
					g->po4flagSceneEnd = 1;
					g->po4procSelecter = 12;
					g->po4flagSceneStart = 1;
				}
			}
			if (g->gameplay.muon.load) {
				StopSound(&g->audio, &g->gameplay.muon);
			}

			return -1;
		}
		SetTimeLapse(2, &g->timer1);
		g->procPhase = 2;
		g->gameplay.flag_threadExist = 0;
		g->gameplay.flag_gameinput = 0;
	}

	return 1;
}

//42cb50
void ProcGameThread(game *g) {

	g->gameplay.flag_threadExist = 1;
	g->gameplay.flag_closingPhase = 0;
	
	while (g->procPhase == 0 || GetTimeLapse(0,&g->timer1) < g->skstruct.loadstart) {
		Sleep(16);
		if (g->gameplay.flag_closingPhase) {
			g->gameplay.flag_threadExist = 0;
			break;
		}
	}

	if (g->gameplay.flag_retry == 0 && g->config.system.isablebmsthread == 0) {
		_beginthread((void(*)(void*))ProcLoadBmsResource, 0, g);
	}
	else {
		g->gameplay.bmsResourceLoaded = 1;
	}

	while (GetTimeLapse(0, &g->timer1) < g->skstruct.loadstart + g->skstruct.loadend || g->gameplay.bmsResourceLoaded == 0) {
		Sleep(16);
		if (g->gameplay.flag_closingPhase) {
			g->gameplay.flag_threadExist = 0;
			break;
		}
	}

	SetTimeLapse(40, &g->timer1);

	while (GetTimeLapse(40,&g->timer1) < g->skstruct.playstart 
		|| g->KeyInput.inputID[2] || g->KeyInput.inputID[3] || g->KeyInput.inputID[4] || g->KeyInput.inputID[5] || g->KeyInput.inputID[6] || g->KeyInput.inputID[7] || g->KeyInput.inputID[8] || g->KeyInput.inputID[9]	|| g->KeyInput.inputID[11] 
		|| g->KeyInput.p1_buttonInput[12] || g->KeyInput.p1_buttonInput[13] || g->KeyInput.p2_buttonInput[12] || g->KeyInput.p2_buttonInput[13]) {

		Sleep(16);
		ReactInput(g);
		if (g->gameplay.flag_closingPhase) {
			g->gameplay.flag_threadExist = 0;
			break;
		}
	}

	SetTimeLapse(41, &g->timer1);
	SetTimeLapse(142, &g->timer1);
	if (g->is_recordmode) {
		g->rec.RefreshCurFrame();
	}

	if (g->rec.recMode == 2) {
		ErrorLogFmtAdd("リプレイ録画なので、あらかじめBGMのみmixします\n");
		double recordnow = GetTimeWrap();
		
		//didn't check
		for (int i = 0; i < g->gameplay.bmsobj.size; i++) {
			double len = 0;
			if (g->gameplay.bmsobj.notes[i].op == 1 || (10 <= g->gameplay.bmsobj.notes[i].op && g->gameplay.bmsobj.notes[i].op < 30)) {
				if (i + 1 < g->gameplay.bmsobj.size) {
					double endtime = g->gameplay.keysound[(int)g->gameplay.bmsobj.notes[i].val].length;
					if ((int)endtime < 0) {
						endtime += 4294967296.0;
					}
					endtime += g->gameplay.bmsobj.notes[i].realTiming;

					for (int j = i + 1; j < g->gameplay.bmsobj.size; j++) {
						if (endtime <= g->gameplay.bmsobj.notes[j].realTiming) break;

						if (g->gameplay.bmsobj.notes[j].op == 1) {

							if ((int)g->gameplay.bmsobj.notes[i].val == (int)g->gameplay.bmsobj.notes[j].val) {
								len = g->gameplay.bmsobj.notes[j].realTiming - g->gameplay.bmsobj.notes[i].realTiming;
								if (len == 0) len = -1.0;
								break;
							}
						}
					}
				}
				RecordSound(&g->audio, &g->gameplay.keysound[(int)g->gameplay.bmsobj.notes[i].val], g->gameplay.bmsobj.notes[i].realTiming, len);
			}
		}
	}

	int gauge = g->config.play.gaugeOption[0];
	if (gauge == 1 || gauge == 2 || gauge == 4 || gauge == 5) {
		SetTimeLapse(44, &g->timer1);
	}
	gauge = g->config.play.gaugeOption[1];
	if (gauge == 1 || gauge == 2 || gauge == 4 || gauge == 5) {
		SetTimeLapse(45, &g->timer1);
	}

	GetTimeWrap();
	if (g->gameplay.replay.status == 2) {
		InitInputStructure2(&g->KeyInput);
	}

	g->gameplay.flag_gameinput = 1;
	if (g->config.system.thread) {
		g->gameplay.flag_threadDoingProcGame = 1;
		do {
			if (g->procPhase != 1) break;
		} while(ProcGame(g) != -1);
		g->gameplay.flag_threadDoingProcGame = 0;
	}
	g->gameplay.flag_threadExist = 0;
	return;
}

//42cf70
int ProcS_Play(game *g, sqlite3* sql) {

	int iTemp, seed;

	if (g->gameplay.hThreadPreview) WaitForSingleObject(g->gameplay.hThreadPreview, 5000);
	g->gameplay.flag_closingPhase = 0;
	
	CSTR gData, gName;
	CSTR md5;
	if (g->gameplay.courseType == 0 || g->gameplay.courseType == 2) {
		md5 = g->sSelect.bmsList[g->sSelect.cur_song].courseHash[g->gameplay.courseStageNow];
	}
	else {
		md5 = g->sSelect.bmsList[g->sSelect.cur_song].hash;
	}

	if (g->net.rankingData.target_ID > 0 && g->net.isOnline == 1) {
		g->gameplay.targetScore.InitJudgeQueue();
		g->net.WaitAndInitRanking();
		if ((g->gameplay.ghostBattle == 0 && g->gameplay.isAutoplay != 1) || g->gameplay.replay.status == 2) {
			g->net.GetTargetInfo(0, md5, &gData, &gName, &seed, &iTemp, &iTemp, &iTemp, &iTemp, &iTemp);
		}
		else {
			if(g->sSelect.bmsList[g->sSelect.cur_song].keymode > 9)
				g->net.GetTargetInfo(0, md5, &gData, &gName, &g->config.play.gaugeOption[0], &g->config.play.random[0], &g->config.play.random[1], &g->config.play.dpflip, &g->gameplay.randomseed, &iTemp);
			else
				g->net.GetTargetInfo(0, md5, &gData, &gName, &g->config.play.gaugeOption[0], &g->config.play.random[0], &g->config.play.random[1], &iTemp, &g->gameplay.randomseed, &iTemp);
			
			g->config.play.gaugeOption[1] = g->config.play.gaugeOption[0];
			g->config.play.random[1] = g->config.play.random[0];
		}
	}

	int scratchside;
	if (g->gameplay.flag_retry == 0) {
		InitGameplay(&g->gameplay, &g->config.play);
		scratchside = 0;
		if (g->sSelect.metaSelected.keymode == 10 && g->config.play.battle == 0 && g->skinData.Data[g->skinData.skinID[3]].type == SKINTYPE_14KEYS) {
			scratchside = g->skstruct.scratchside_1 + g->skstruct.scratchside_2 * 2;
		}
		if (g->sSelect.metaSelected.keymode == 5 && (g->config.play.battle == 2 || g->config.play.battle == 3) && g->skinData.Data[g->skinData.skinID[3]].type == SKINTYPE_14KEYS) {
			scratchside = g->skstruct.scratchside_1 + g->skstruct.scratchside_2 * 2;
		}
		if (g->sSelect.metaSelected.keymode == 10 && g->config.play.battle == 0 && g->skinData.Data[g->skinData.skinID[13]].type == SKINTYPE_5KEYSBATTLE) {
			scratchside = g->skstruct.scratchside_1 + g->skstruct.scratchside_2 * 2;
		}
		if (g->sSelect.metaSelected.keymode == 5 && g->config.play.battle == 1 && g->skinData.Data[g->skinData.skinID[13]].type == SKINTYPE_5KEYSBATTLE) {
			scratchside = g->skstruct.scratchside_1 + g->skstruct.scratchside_2 * 2;
		}
		if (g->sSelect.metaSelected.keymode == 5 && g->config.play.battle == 0 && g->skinData.Data[g->skinData.skinID[1]].type == SKINTYPE_7KEYS) {
			scratchside = g->skstruct.scratchside_1 + g->skstruct.scratchside_2 * 2;
		}

		if (g->gameplay.ghostBattle) g->config.play.battle = 1;
		if (g->rec.recMode == 4) {
			ErrorLogFmtAdd("bms2aviの準備\n");
			g->config.system.isablebmsthread = 1;
			g->config.play.bga = 1;
			g->config.system.vsync = 1;
			g->cmd_nosave = 1;
			g->is_recordmode = 1;
			g->cmd_auto = 1;
			g->gameplay.isPreviewLoad = 0;
			g->audio.cmd_mediaOut = true;
			g->cmd_directplay = 1;
			g->directoryPath = g->sSelect.metaSelected.filepath;
			g->is_recordmode = 1;
			if (g->directoryPath.length() == 0) {
				cstrSprintf(&g->directoryPath, "%smovie.avi", g->baseDirectory);
			}
			g->rec.recMode = 2;
			if (g->gameplay.replay.status == 2) {
				g->audio.replay2avi = true;
			}
			else {
				g->rec.recMode = 1;
				g->gameplay.isAutoplay = 1;
			}
			g->audio.cmd_mediaOut = true;
		}

		ParseBmsFile(&g->gameplay, g->sSelect.metaSelected.filepath, &g->audio, &g->config, &g->sSelect.metaSelected, g->skstruct.flag_BGA, scratchside);
	}
	else {
		InitGameplay_retry(&g->gameplay, &g->audio, g);
	}
	
	if (g->config.play.m_lunaris) {
		LUNARIS_START(g);
	}
	
	if (g->is_recordmode && g->rec.writeSamplePos <= 0) {
		g->rec.PrepareAVIRecord(g->config.tools.movie_framerate, 24, g->directoryFilename, (g->skstruct.fadeout + g->gameplay.song_runtime + g->skstruct.loadstart + g->skstruct.loadend + g->skstruct.playstart + 500.0) * g->config.tools.movie_framerate / 1000.0, GetMainWindowHandle());
		g->timer1.flag = 1;
		g->timer1.bgaFramerate = g->config.tools.movie_framerate;
		g->timer1.bga = 0.0;
		if (g->is_recordmode) SetClockFlag(&g->timer1, 1);
		InitTimer(&g->timer1);
		g->gameplay.timetick = GetTimeWrap();
		if (g->gameplay.flag_longsound || g->gameplay.flag_0note) {
			MessageBoxA(NULL, "規約違反のbmsの可能性があるので、この機能は利用できません。", "エラー", 0);
			g->procSelecter = 0;
			g->is_recordmode = 0;
			return 0;
		}
		if (g->rec.recMode == 2) {
			g->audio.aviTimer = 0.0;
			GetSoundBuffer(&g->audio, g->skstruct.loadstart + g->gameplay.song_runtime + g->skstruct.loadend + g->skstruct.playstart + 60000.0, g->config.tools.mp3_volume);
		}
	}

	g->gameplay.highScore.InitJudgeQueue();
	if (g->gameplay.isGhostDisabled == 0) {
		ReadGhostToScore(sql,md5,&g->gameplay.highScore);
	}

	if (g->net.rankingData.target_ID > 0 && g->net.isOnline == 1) {
		if (gData.length() <= 0 || gData.isDiff("Z") == 0) {
			ErrorLogFmtAdd("ゴースト無しでターゲットを設定します ターゲット番号 %d ターゲット名 %s\n", g->net.rankingData.target_number, g->net.rankingData.ranking[g->net.rankingData.target_number].name);
			
			g->gameplay.targetScore.judgeExpect[5] = g->net.rankingData.ranking[g->net.rankingData.target_number].pg;
			g->gameplay.targetScore.judgeExpect[4] = g->net.rankingData.ranking[g->net.rankingData.target_number].gr;
			g->gameplay.targetScore.judgeExpect[3] = g->net.rankingData.ranking[g->net.rankingData.target_number].gd;
			g->gameplay.targetScore.judgeExpect[2] = g->net.rankingData.ranking[g->net.rankingData.target_number].bd;
			g->gameplay.targetScore.judgeExpect[1] = g->net.rankingData.ranking[g->net.rankingData.target_number].pr;
			g->gameplay.targetScore.totalnotes = g->gameplay.player[0].totalnotes;
			g->gameplay.targetScore.name = g->net.rankingData.ranking[g->net.rankingData.target_number].name;
			g->gameplay.targetScore.exscore = g->net.rankingData.ranking[g->net.rankingData.target_number].gr + g->net.rankingData.ranking[g->net.rankingData.target_number].pg * 2;
			g->gameplay.targetType = 2;
		}
		else {
			g->gameplay.targetScore.DecodeGhostData(gData);
			g->gameplay.targetScore.name = gName;
			g->gameplay.targetType = 2;
		}
	}
	else{
		g->gameplay.targetScore.InitJudgeQueue();
		switch (g->config.play.p1_target) {
			case 0:
				g->gameplay.targetScore.SetDefaultGhost(g->config.play.target_percent, g->gameplay.player[0].totalnotes);
				g->gameplay.targetType = 0;
				break;

			case 1:
				if (g->gameplay.isGhostDisabled == 0) {
					if (g->gameplay.highScore.exscore <= 0) {
						g->gameplay.targetScore.SetDefaultGhost(g->config.play.target_percent, g->gameplay.player[0].totalnotes);
						g->gameplay.targetType = 2;
					}
					else {
						g->gameplay.targetScore.SetDefaultGhost(g->config.play.target_percent, g->gameplay.player[0].totalnotes);
						g->gameplay.targetType = 1;
					}
					break;
				}
			case 5:
				g->gameplay.targetScore.SetDefaultGhost(g->config.play.target_percent, g->gameplay.player[0].totalnotes);
				g->gameplay.targetType = 2;
				break;

			case 2:
				g->gameplay.targetScore.SetDefaultGhost(88, g->gameplay.player[0].totalnotes);
				g->gameplay.targetType = 2;
				break;
			case 3:
				g->gameplay.targetScore.SetDefaultGhost(77, g->gameplay.player[0].totalnotes);
				g->gameplay.targetType = 2;
				break;
			case 4:
				g->gameplay.targetScore.SetDefaultGhost(66, g->gameplay.player[0].totalnotes);
				g->gameplay.targetType = 2;
				break;
			case 6:
			case 7:
				if (g->gameplay.isGhostDisabled == 0) {
					g->net.WaitAndInitRanking();
					iTemp = 0;
					g->net.GetTargetInfo(g->config.play.p1_target, md5, &gData, &gName, &iTemp, &iTemp, &iTemp, &iTemp, &iTemp, &iTemp);
					if (gData.length() > 0 && g->config.play.p1_target != 8) {
						g->gameplay.targetScore.DecodeGhostData(gData);
						g->gameplay.targetScore.name = gName;
						g->gameplay.targetType = 2;
					}
					if (g->gameplay.targetScore.exscore == 0) {
						g->gameplay.targetScore.SetDefaultGhost(g->config.play.target_percent, g->gameplay.player[0].totalnotes);
						g->gameplay.targetType = 2;
					}
					break;
				}
				else {
					g->gameplay.targetScore.SetDefaultGhost(g->config.play.target_percent, g->gameplay.player[0].totalnotes);
					g->gameplay.targetType = 2;
				}
				break;
			case 8:
				if (g->gameplay.isGhostDisabled == 0) {
					g->net.WaitAndInitRanking();
					iTemp = 0;
					int exscore = 0;
					g->net.GetTargetInfo(g->config.play.p1_target, md5, &gData, &gName, &iTemp, &iTemp, &iTemp, &iTemp, &iTemp, &exscore);
					
					if (exscore == 0) {
						g->gameplay.targetScore.SetDefaultGhost(g->config.play.target_percent, g->gameplay.player[0].totalnotes);
						g->gameplay.targetType = 2;
						break;
					}
					g->gameplay.targetScore.SetGhost(exscore, g->gameplay.player[0].totalnotes, "AVERAGE");
				}
				else {
					g->gameplay.targetScore.SetDefaultGhost(g->config.play.target_percent, g->gameplay.player[0].totalnotes);
					g->gameplay.targetType = 2;
				}
				break;
		}
	}

	SetObjectString(1, g->gameplay.targetScore.name, g->txtStruct.objectStr);
	HANDLE hG = (HANDLE)_beginthread((void(*)(void*))ProcGameThread,0,g);
	SetThreadPriority(hG, -1);
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

//42dcc0
int ProcI_PO4Select(game *g, sqlite3 *sql) { //not tested

	int selectedBar = g->sSelect.listSelectedBarFromScreenTop;

	if (g->po4procSelecter == 0) {
		g->po4procSelecter = 1;
		g->po4_unk23d84 = 0;
		g->po4_unk23d88 = 0;
		g->po4flagSceneStart = '\x01';
		g->po4flagSceneEnd = '\0';
		g->po4_hThread_ParseBMS = 0;
		g->sSelect.listCalculatedBar = 0;
		g->sSelect.barMoveStartTime = GetTimeWrap();
		g->sSelect.barMoveEndTime = GetTimeWrap();
		g->sSelect.oldBar = 0;
		g->sSelect.nowBar = 0;
		g->sSelect.listTopbar = 0;
		g->sSelect.flag_folderlamp = (g->config.select.folderlamp != 0);
		g->sSelect.cur_song = GetSongCursor(g);
		g->sSelect.toRoot = 1;
		g->sSelect.bmsListCount = 4;
		SetTimeLapse(400, &g->timer1);
		SetTimeLapse(0, &g->timer1);
		g->config.play.autojudge = 0;
	}

	if (g->po4flagSceneStart) {
		if (g->po4procSelecter == 1) {
			if (GetTimeLapse(400, &g->timer1) >= g->skstruct.event_STARTINPUT[0])
				g->po4flagSceneStart = 0;
		}
		else if (g->po4procSelecter == 3) {
			if (GetTimeLapse(402, &g->timer1) >= g->skstruct.event_STARTINPUT[1])
				g->po4flagSceneStart = 0;
		}
		else if (g->po4procSelecter == 5) {
			if (GetTimeLapse(404, &g->timer1) >= g->skstruct.event_STARTINPUT[2])
				g->po4flagSceneStart = 0;

			g->gameplay.bmsResourceLoaded = 0;
			g->gameplay.flag_gameinput = 0;
			g->po4_23da8 = 0;
		}
		else if (g->po4procSelecter == 8) {
			if (GetTimeLapse(407, &g->timer1) >= g->skstruct.event_STARTINPUT[3]) {
				g->po4flagSceneStart = 0;

			}
		}
		else if (g->po4procSelecter == 11) {
			if (g->po4_23da8 == 0) {
				if (GetTimeLapse(410, &g->timer1) >= g->skstruct.event_STARTINPUT[4])
					g->po4flagSceneStart = 0;
			}
			else if (g->po4_23da8 != 0) {
				if (GetTimeLapse(411, &g->timer1) >= g->skstruct.event_STARTINPUT[5]) {
					if (g->gameplay.isAutoplay == 0 && g->config.play.autojudge == 0 && g->gameplay.player[0].exscore > 0) {
						SaveResult(g, sql);
						g->po4flagSceneEnd = 0;
					}
					g->gameplay.isAutoplay = 0;
				}
			}
		}

	}

	if (g->po4flagSceneEnd) {
		if (GetTimeLapse(g->po4sceneTimerID, &g->timer1) >= g->po4sceneFadeout) {

			for (int i = 1; i < 200; i++) ResetTimeLapse(i, &g->timer1);
			g->po4procSelecter = g->po4nextProc;
			ResetTimeLapse(g->po4sceneTimerID, &g->timer1);
			SetTimeLapse(g->po4sceneTimerIDNext, &g->timer1);
			g->po4flagSceneEnd = 0;

			if (g->po4procSelecter == 5) {
				ResetTimeLapse(411, &g->timer1);
				ResetTimeLapse(410, &g->timer1);
				InitKeysound(g);
				ReleaseBGA(g);
			}
			else if (g->po4procSelecter == 3) {
				g->sSelect.bmsListCount = 4;
				g->sSelect.listCalculatedBar = g->po4cur_song * 1000;
				g->sSelect.barMoveStartTime = GetTimeWrap();
				g->sSelect.barMoveEndTime = GetTimeWrap();
				g->sSelect.oldBar = g->po4cur_song * 1000;
				g->sSelect.nowBar = g->po4cur_song * 1000;
				g->sSelect.listTopbar = g->po4cur_song * 1000;
				g->sSelect.flag_folderlamp = (g->config.select.folderlamp != 0);
				g->sSelect.cur_song = GetSongCursor(g);
				g->sSelect.toRoot = 1;
				ProcS_Select(g);
			}
		}
	}

	if ((g->po4procSelecter == 3 || g->po4procSelecter == 4 || g->po4procSelecter == 5) && g->po4flagSceneStart == 0 && g->txtStruct.readme.show == 0) {
		if (g->KeyInput.mousewheel) {
			g->sSelect.nowBar -= g->KeyInput.mousewheel * 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.barMoveEndTime = GetTimeWrap() + 200;
			if (g->KeyInput.mousewheel > 0) g->sSelect.scrollDirection = 1;
			else if (g->KeyInput.mousewheel < 0) g->sSelect.scrollDirection = 2;
		}
		
		if ((((g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[1] == 1) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[4] == 1 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[10] == 1 || g->KeyInput.p2_buttonInput[10] == 1 || g->KeyInput.inputID[D_DIK_UP] == 1 || g->KeyInput.inputID[D_DIK_LEFT] == 1)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

			g->sSelect.nowBar -= 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 1;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
		}
		else if ((((g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[3] == 1) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[6] == 1 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[11] == 1 || g->KeyInput.p2_buttonInput[11] == 1 || g->KeyInput.inputID[D_DIK_DOWN] == 1 || g->KeyInput.inputID[D_DIK_RIGHT] == 1)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

			g->sSelect.nowBar += 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 2;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
		}
		else if ((((g->KeyInput.p1_buttonInput[1] == 2 || g->KeyInput.p2_buttonInput[1] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[4] == 2 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[10] == 2 || g->KeyInput.p2_buttonInput[10] == 2 || g->KeyInput.inputID[D_DIK_UP] == 2 || g->KeyInput.inputID[D_DIK_LEFT] == 2)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime - 20) {

			g->sSelect.nowBar -= 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 1;
			g->sSelect.barMoveEndTime = GetTimeWrap() + 150;
		}
		else if ((((g->KeyInput.p1_buttonInput[3] == 2 || g->KeyInput.p2_buttonInput[3] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[6] == 2 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[11] == 2 || g->KeyInput.p2_buttonInput[11] == 2 || g->KeyInput.inputID[D_DIK_DOWN] == 2 || g->KeyInput.inputID[D_DIK_RIGHT] == 2)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime - 20) {

			g->sSelect.nowBar += 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 2;
			g->sSelect.barMoveEndTime = GetTimeWrap() + 150;
		}
	}

	if (g->sSelect.searchFocused == 0) {
		int barp = ChangeValueByTime(g->sSelect.oldBar, g->sSelect.nowBar, g->sSelect.barMoveStartTime, g->sSelect.barMoveEndTime, GetTimeWrap(), 0);
		int barb = g->sSelect.listCalculatedBar;
		g->sSelect.listTopbar = barp;
		g->sSelect.listCalculatedBar = barp;

		while (g->sSelect.listCalculatedBar < 0)
			g->sSelect.listCalculatedBar += g->sSelect.bmsListCount * 1000;

		while (g->sSelect.listCalculatedBar >= g->sSelect.bmsListCount * 1000)
			g->sSelect.listCalculatedBar -= g->sSelect.bmsListCount * 1000;

		if (g->sSelect.cur_song != GetSongCursor(g) || selectedBar != g->sSelect.listSelectedBarFromScreenTop) {
			g->sSelect.cur_song = GetSongCursor(g);
			if (GetTimeLapse(1, &g->timer1) > 0.0) {
				if (g->net.rankingData.showRanking == 0) SetTimeLapse(11, &g->timer1);
				if (g->sSelect.is_loading_bmslist == 0) PlaySound(&g->audio, &g->audio.sysSound.scratch, g->audio.chnKey, -1);
				else g->sSelect.is_loading_bmslist = 0;
			}
			if (g->net.rankingData.showRanking == 0) ProcS_Select(g);
			else SetObjectString(1, g->sSelect.bmsList[g->sSelect.cur_song].fulltitle, g->txtStruct.objectStr);
		}
		g->sSelect.is_loading_bmslist = 0;
		if (barb == g->sSelect.listCalculatedBar) {
			ResetTimeLapse(10, &g->timer1);
		}
		if (barb != g->sSelect.listCalculatedBar) {
			if (GetTimeLapse(10, &g->timer1)) {
				SetTimeLapse(10, &g->timer1);
			}
		}
	}

	switch (g->po4procSelecter) {
	case 1:
		if (g->KeyInput.inputID[D_DIK_ESCAPE] == 1 && g->po4flagSceneStart == 0) {
			g->procSelecter = 0;
			return 1;
		}
		if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[D_DIK_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
			&& g->po4flagSceneStart == 0) {

			ResetTimeLapse(400, &g->timer1);
			SetTimeLapse(401, &g->timer1);
			g->po4sceneFadeout = g->skstruct.event_FADEOUT[0];
			g->po4sceneTimerID = 401;
			g->po4nextProc = 3;
			g->po4sceneTimerIDNext = 402;
			g->po4procSelecter = 2;
			g->po4flagSceneEnd = 1;
			g->po4flagSceneStart = 1;
		}
		break;
	case 3:
			{
				if (g->KeyInput.inputID[D_DIK_ESCAPE] == 1 && g->po4flagSceneStart == 0) {
					g->procSelecter = 0;
					return 1;
				}

				if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[D_DIK_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
					&& g->po4flagSceneStart == 0) {

					g->sSelect.listCalculatedBar / 1000;
					if (g->sSelect.listCalculatedBar % 1000 == 0) {
						g->po4cur_song = g->sSelect.listCalculatedBar / 1000;
					}
					else if (g->sSelect.scrollDirection == 1) {
						g->po4cur_song = g->sSelect.listCalculatedBar / 1000 - 1;
						if (g->po4cur_song < 0) g->po4cur_song = 3;
					}
					else if (g->sSelect.scrollDirection == 2) {
						g->po4cur_song = g->sSelect.listCalculatedBar / 1000 + 1;
						if (g->po4cur_song >= 4) g->po4cur_song = 0;
					}

					if (g->po4cur_song < 0 || g->po4cur_song > 2) {
						g->procSelecter = 0;
						return 1;
					}

					cstrSprintf(&g->sSelect.stack_query[g->sSelect.cur], "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE parent = \'%s\'", AssignCRC32(g->config.jukebox.path[0]));
					g->sSelect.stack_isFolder[g->sSelect.cur] = 0;
					g->sSelect.stack_rivalID[g->sSelect.cur] = 0;
					g->sSelect.stack_searchTitle[g->sSelect.cur] = "検索語句を入力";
					g->sSelect.directory = "ROOT";
					int t = g->po4cur_song + 2;
					LoadFilteredBmsListFromDB(g->sSelect.stack_query[g->sSelect.cur], sql, &g->sSelect, &t, &g->config.select.key, 1, 0, 0);
					SwapBmsList(&g->sSelect);
					SetTransColor(0, 255, 0);

					for (int i = 0; i < g->sSelect.bmsListCount; i++) {
						SetTransColor(0, 255, 0);
						CSTR dir = g->sSelect.bmsList[i].filepath.getDirectory();
						if (g->sSelect.bmsList[i].isStagefile == 0) {
							g->sSelect.bmsList[i].stagefile = "_title.png";
						}
						CSTR alt;
						if (FindAltImage(g->sSelect.bmsList[i].stagefile, dir, &alt) != 1) {
							g->sSelect.bmsList[i].isStagefile = 0;
						}

						g->sSelect.bmsList[i].grHandle = LoadGraph(alt, 0);
						if (g->sSelect.bmsList[i].grHandle == -1) {
							g->sSelect.bmsList[i].isStagefile = 0;
						}
					}

					g->sSelect.listCalculatedBar = 0;
					g->sSelect.barMoveStartTime = GetTimeWrap();
					g->sSelect.barMoveEndTime = GetTimeWrap();
					g->sSelect.oldBar = 0;
					g->sSelect.nowBar = 0;
					g->sSelect.listTopbar = 0;
					g->sSelect.listSelectedBarFromScreenTop = g->skstruct.BAR_CENTER;
					g->sSelect.flag_folderlamp = (g->config.select.folderlamp != 0);
					g->sSelect.cur_song = GetSongCursor(g);

					ProcS_Select(g);

					ResetTimeLapse(402, &g->timer1);
					SetTimeLapse(403, &g->timer1);
					g->po4sceneFadeout = g->skstruct.event_FADEOUT[1];
					g->po4sceneTimerID = 403;
					g->po4nextProc = 5;
					g->po4sceneTimerIDNext = 404;
					g->po4flagSceneEnd = 1;
					g->po4procSelecter = 4;
					g->po4flagSceneStart = 1;
				}

				int cur = g->sSelect.listCalculatedBar / 1000 + 1;
				if (cur >= 4) cur = 0;
				if (g->sSelect.listCalculatedBar % 1000 == NULL) {
					DSTdraw dstd;

					dstd = SetDSTdrawByTime(g->skstruct.dst_EVENT_MODE_CURSOR_ON[g->sSelect.listCalculatedBar / 1000], GetTimeLapse(402, &g->timer1));
					AddDrawingBuffer_Lunaris(&g->skstruct.drBuf, &g->skstruct.src_EVENT_MODE_CURSOR, &dstd, &g->timer1);
				}
				else {
					DSTdraw dstd1, dstd2, dstd3;

					dstd1 = SetDSTdrawByTime(g->skstruct.dst_EVENT_MODE_CURSOR_ON[cur], GetTimeLapse(402, &g->timer1));
					dstd2 = SetDSTdrawByTime(g->skstruct.dst_EVENT_MODE_CURSOR_ON[g->sSelect.listCalculatedBar / 1000], GetTimeLapse(402, &g->timer1));

					dstd3 = DSTDbyTime(&dstd2, &dstd1, 0, 1000.0, g->sSelect.listCalculatedBar % 1000);

					AddDrawingBuffer_Lunaris(&g->skstruct.drBuf, &g->skstruct.src_EVENT_MODE_CURSOR, &dstd3, &g->timer1);
				}
			}
			break;
		case 5:
			for (int i = 0; i < g->txtStruct.readme.lines; i++) {
				int y = g->skstruct.src_README[0].op1 * i + g->txtStruct.readme.h;
				if (y < 480 && (g->skstruct.src_README[0].op1*(i + 1) + g->txtStruct.readme.h > 0)) {
					AddDrawingBuffer_TextXY(&g->skstruct.drBuf, &g->skstruct.src_README[0], &g->skstruct.dst_README[0], &g->timer1, i + 1000, g->txtStruct.readme.w, y);
					AddDrawingBuffer_TextXY(&g->skstruct.drBuf, &g->skstruct.src_README[1], &g->skstruct.dst_README[1], &g->timer1, i + 1000, g->txtStruct.readme.w, g->skstruct.src_README[1].op1 * i + g->txtStruct.readme.h);
				}
			}

			if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.inputID[D_DIK_ESCAPE] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[6] == 1)
				&& g->po4flagSceneStart == 0 && g->txtStruct.readme.show == 0) {

				ResetTimeLapse(404, &g->timer1);
				SetTimeLapse(406, &g->timer1);
				g->po4sceneTimerID = 406;
				g->po4nextProc = 3;
				g->po4sceneTimerIDNext = 404;
				g->po4procSelecter = 7;
				g->po4sceneFadeout = g->skstruct.event_FADEOUT[2];
				g->po4flagSceneStart = 1;
				g->po4flagSceneEnd = 1;
			}
			else if ((g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[D_DIK_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
					&& g->po4flagSceneStart == 0 && g->txtStruct.readme.show == 0) {

				ResetTimeLapse(404, &g->timer1);
				SetTimeLapse(406, &g->timer1);
				g->po4sceneTimerID = 405;
				g->po4nextProc = 8;
				g->po4sceneTimerIDNext = 407;
				g->po4procSelecter = 6;
				g->po4sceneFadeout = g->skstruct.event_FADEOUT[3];
				g->po4flagSceneStart = 1;
				g->po4flagSceneEnd = 1;
			}
			else if (g->KeyInput.inputID[D_DIK_F1] == 1 && g->po4flagSceneStart == 0 && g->txtStruct.readme.show == 0) {
				ResetTimeLapse(404, &g->timer1);
				SetTimeLapse(406, &g->timer1);
				g->po4sceneTimerID = 405;
				g->po4nextProc = 8;
				g->po4sceneTimerIDNext = 407;
				g->po4procSelecter = 6;
				g->po4sceneFadeout = g->skstruct.event_FADEOUT[3];

				g->gameplay.isAutoplay = 1;
				
				g->po4flagSceneStart = 1;
				g->po4flagSceneEnd = 1;
			}
			else if (g->KeyInput.inputID[D_DIK_F3] == 1 && g->po4flagSceneStart == 0 && g->txtStruct.readme.show == 0) {
				ResetTimeLapse(404, &g->timer1);
				SetTimeLapse(406, &g->timer1);
				g->po4sceneTimerID = 405;
				g->po4nextProc = 8;
				g->po4sceneTimerIDNext = 407;
				g->po4procSelecter = 6;
				g->po4sceneFadeout = g->skstruct.event_FADEOUT[3];

				g->config.play.autojudge = 2;
				
				g->po4flagSceneStart = 1;
				g->po4flagSceneEnd = 1;
			}
			
			if (g->KeyInput.inputID[D_DIK_F2] == 1 && g->po4flagSceneStart == 0 && g->txtStruct.readme.show == 0) {
				ShowReadmes(g);
				if (g->txtStruct.readme.show) {
					ResetTimeLapse(16, &g->timer1);
					SetTimeLapse(15, &g->timer1);
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
				}
				g->po4_23da9 = 1;
				break;
			}

			if (g->txtStruct.readme.show > 0) {

				g->txtStruct.readme.h += g->KeyInput.mousewheel * g->skstruct.src_README[0].op1;

				if (g->KeyInput.inputID[D_DIK_F2] == 1 || g->KeyInput.inputID[D_DIK_ESCAPE] == 1) {
					ResetTimeLapse(15, &g->timer1);
					SetTimeLapse(16, &g->timer1);
					g->txtStruct.readme.show = 0;
				}
				else if (g->KeyInput.mouse_buttonL == 2) {
					if (g->KeyInput.mouse_oldX < 200) {
						g->txtStruct.readme.w += g->timer1.tickTime * 600.0 / 1000.0;
					}
					else if (g->KeyInput.mouse_oldX > 440) {
						g->txtStruct.readme.w -= g->timer1.tickTime * 600.0 / 1000.0;
					}

					if (g->KeyInput.mouse_oldY < 150) {
						g->txtStruct.readme.h += g->timer1.tickTime * 600.0 / 1000.0;
					}
					else if (g->KeyInput.mouse_oldY > 330) {
						g->txtStruct.readme.h -= g->timer1.tickTime * 600.0 / 1000.0;
					}
				}

				int max = 480 - g->txtStruct.readme.y;
				if (g->txtStruct.readme.h < max) g->txtStruct.readme.h = max;
				max = 640 - g->txtStruct.readme.x;
				if (g->txtStruct.readme.w < max) g->txtStruct.readme.w = max;
				if (g->txtStruct.readme.w > 0) g->txtStruct.readme.w = 0;
				if (g->txtStruct.readme.h > 0) g->txtStruct.readme.h = 0;
			}
			break;
		case 8:
			if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[D_DIK_ESCAPE] == 1 || g->KeyInput.inputID[D_DIK_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
				&& g->po4flagSceneStart == 0 && g->gameplay.bmsResourceLoaded) {

				ResetTimeLapse(407, &g->timer1);
				SetTimeLapse(408, &g->timer1);
				g->po4sceneFadeout = g->skstruct.event_FADEOUT[4];
				g->po4sceneTimerID = 408;
				g->po4nextProc = 11;
				g->po4sceneTimerIDNext = 410;
				g->po4procSelecter = 9;
				g->po4flagSceneEnd = 1;
				g->po4flagSceneStart = 1;
			}
			break;
		case 11:
			g->skstruct.dst_GROOVEGAUGE[0].timer = 410;
			DrawHPgauge(g);
			SoundGetCurrentTime(&g->audio, &g->gameplay.muon);
			NONE_004b6770();
			if (GetTimeLapse(41, &g->timer1) > 0.0 && g->config.play.m_lunaris == 0) {
				DrawNotes(g, &g->skstruct, &g->timer1, &g->config.play);
				DrawJudgeCombo(g, &g->skstruct, &g->timer1, &g->config.play);
			}
			else if (GetTimeLapse(41, &g->timer1) > 0.0 && g->config.play.m_lunaris != 0) {
				DrawLunaris(g);
				DrawJudgeCombo(g, &g->skstruct, &g->timer1, &g->config.play);
			}

			if (g->skstruct.dst_JUDGELINE[0].dstCount > 0) {
				AddDrawingBuffer_PlayArea(&g->skstruct.drBuf, &g->skstruct.src_JUDGELINE[0], &g->skstruct.dst_JUDGELINE[0], &g->timer1, g->skstruct.adjust.note_1p_x + g->gameplay.nabeatsu_x, g->skstruct.adjust.note_1p_y + g->gameplay.nabeatsu_y, -1, g->skstruct.adjust.size_x, g->skstruct.adjust.size_y, 0);
			}
			if (g->skstruct.dst_JUDGELINE[1].dstCount > 0) {
				AddDrawingBuffer_PlayArea(&g->skstruct.drBuf, &g->skstruct.src_JUDGELINE[1], &g->skstruct.dst_JUDGELINE[1], &g->timer1, g->skstruct.adjust.note_2p_x + g->gameplay.nabeatsu_x, g->skstruct.adjust.note_2p_y + g->gameplay.nabeatsu_y, -1, g->skstruct.adjust.size_x, g->skstruct.adjust.size_y, 0);
			}

			if (g->KeyInput.inputID[D_DIK_UP] == 1) {
				g->config.play.hiSpeed[0] += 10;
				if (g->config.play.hiSpeed[0] >= 400) g->config.play.hiSpeed[0] = 400;
			}
			else if (g->KeyInput.inputID[D_DIK_DOWN] == 1) {
				g->config.play.hiSpeed[0] -= 10;
				if (g->config.play.hiSpeed[0] <= 50) g->config.play.hiSpeed[0] = 5;
			}

			if (GetTimeLapse(412, &g->timer1) < 0.0) {
				if (g->po4_23da8 == 0) {
					if (g->gameplay.flag_gameinput) {
						ProcGame(g);
					}
					else if (g->KeyInput.inputID[D_DIK_ESCAPE] == 1) {
						ResetTimeLapse(41, &g->timer1);
						ResetTimeLapse(140, &g->timer1);
						SetTimeLapse(412, &g->timer1);
						g->po4sceneFadeout = g->skstruct.event_FADEOUT[5];
						g->po4sceneTimerID = 412;
						g->po4nextProc = 5;
						g->po4sceneTimerIDNext = 404;
						g->po4procSelecter = 12;
						g->po4flagSceneEnd = 1;
						g->po4flagSceneStart = 1;
					}
					else if (GetTimeLapse(410, &g->timer1) >= g->skstruct.playstart) {
						SetTimeLapse(41, &g->timer1);
						SetTimeLapse(142, &g->timer1);
						if (g->config.play.gaugeOption[0] == 1 || g->config.play.gaugeOption[0] == 2 || g->config.play.gaugeOption[0] == 4 || g->config.play.gaugeOption[0] == 5) {
							SetTimeLapse(44, &g->timer1);
						}
						if (g->config.play.gaugeOption[1] == 1 || g->config.play.gaugeOption[1] == 2 || g->config.play.gaugeOption[1] == 4 || g->config.play.gaugeOption[1] == 5) {
							SetTimeLapse(45, &g->timer1);
						}
						g->gameplay.flag_gameinput = 1;
					}
				}
				else {
					if ( (g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 ||g->KeyInput.inputID[D_DIK_ESCAPE] == 1 || g->KeyInput.inputID[D_DIK_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
						&& g->po4flagSceneStart == 0 && GetTimeLapse(411, &g->timer1) > 0.0) {

						ResetTimeLapse(41, &g->timer1);
						ResetTimeLapse(140, &g->timer1);
						SetTimeLapse(412, &g->timer1);
						g->po4sceneFadeout = g->skstruct.event_FADEOUT[5];
						g->po4sceneTimerID = 412;
						g->po4nextProc = 5;
						g->po4sceneTimerIDNext = 404;
						g->po4procSelecter = 12;
						g->po4flagSceneEnd = 1;
						g->po4flagSceneStart = 1;
					}
				}
			}
			break;
	}

	if (g->po4procSelecter == 8 || g->po4procSelecter == 9 || g->po4procSelecter == 10) {
		AddDrawingBuffer_EventLoading(&g->skstruct.drBuf, g->sSelect.bmsList[g->sSelect.cur_song].grHandle, &g->skstruct.dst_EVENT_LOADINGBG[0], &g->timer1, 0, 0);
		AddDrawingBuffer_EventLoading(&g->skstruct.drBuf, g->sSelect.bmsList[g->sSelect.cur_song].grHandle, &g->skstruct.dst_EVENT_LOADINGBG[1], &g->timer1, 0, 0);
		AddDrawingBuffer_EventLoading(&g->skstruct.drBuf, g->sSelect.bmsList[g->sSelect.cur_song].grHandle, &g->skstruct.dst_EVENT_LOADINGBG[2], &g->timer1, 0, 0);
	}

	if (g->po4procSelecter == 5 || g->po4procSelecter == 6 || g->po4procSelecter == 7) {
		int mod = g->sSelect.listCalculatedBar % 1000;
		int l44c;
		GetNowUnixtime();

		if (g->po4procSelecter == 5) {
			l44c = 0;
		}
		else if (g->po4procSelecter == 7) {
			l44c = 5;
		}
		else if (g->po4procSelecter == 6) {
			l44c = 10;
		}

		for (int i = l44c + 1; i < l44c + 5; i++) {
			g->skstruct.dst_BAR_BODY_ON[l44c].dstCount;
			if (g->skstruct.dst_BAR_BODY_ON[i].dstCount > 0 && g->skstruct.dst_BAR_BODY_ON[i-1].dstCount > 0) {
				DSTdraw dstd1, dstd2, dstd3;
				InitDSTdraw(&dstd1);
				InitDSTdraw(&dstd2);
				InitDSTdraw(&dstd3);

				int cur = i + g->sSelect.listCalculatedBar / 1000 - g->skstruct.BAR_CENTER - l44c;
				while (cur < 0) cur += g->sSelect.bmsListCount;
				int curmod = cur % g->sSelect.bmsListCount;

				dstd1 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_ON[i], GetTimeLapse(g->skstruct.dst_BAR_BODY_ON[i].timer, &g->timer1));
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_ON[i-1], GetTimeLapse(g->skstruct.dst_BAR_BODY_ON[i].timer, &g->timer1));

				if (mod == 0) {
					dstd3 = dstd1;
				}
				else {
					dstd3 = DSTDbyTime(&dstd1, &dstd2, 0, 1000.0, mod);
				}

				if (dstd1.time != -1 && dstd2.time != -1) {
					
					AddDrawingBuffer_EventLoading(&g->skstruct.drBuf, g->sSelect.bmsList[curmod].grHandle, &g->skstruct.dst_BAR_STAGEFILE, &g->timer1, dstd3.x, dstd3.y);
					AddDrawingBuffer_Lunaris(&g->skstruct.drBuf, &g->skstruct.src_BAR_BODY[g->po4cur_song], &dstd3, &g->timer1);

					g->skstruct.dst_BAR_STAR[g->po4cur_song].draw->w;
					((g->sSelect.bmsList[curmod].level - 1) * g->skstruct.dst_BAR_STAR[g->po4cur_song].draw->w /2);
					
					for (int i = 0; i < g->sSelect.bmsList[curmod].level; i++) {
						AddDrawingBuffer_ObjectAlpha(&g->skstruct.drBuf, &g->skstruct.src_BAR_STAR[g->po4cur_song], &g->skstruct.dst_BAR_STAR[g->po4cur_song], &g->timer1, dstd3.x - ((g->sSelect.bmsList[curmod].level - 1) * g->skstruct.dst_BAR_STAR[g->po4cur_song].draw->w / 2) + i, dstd3.y, 255);
					}

					int rank = g->sSelect.bmsList[curmod].mybest.rank;
					if (0 <= rank && rank <= 8) {
						if (rank >= 9) rank = 8;
						else if (rank <= 0 && g->sSelect.bmsList[curmod].mybest.stat_exscore > 0) rank = 1;

						AddDrawingBuffer_Object(&g->skstruct.drBuf, &g->skstruct.src_BAR_RANK[rank], &g->skstruct.dst_BAR_RANK[rank], &g->timer1, dstd3.x, dstd3.y);
					}
				}
			}
		}
	}

	return 1;
}

//437210
//437260
//4372c0
//437300
//437a40
//437bf0
//437c90
//437cf0
int makeFileHash(LPCSTR filepath, LPCSTR oBuf) {
	//HANDLE hFile;
	//hFile = CreateFileA(filepath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
	//if (hFile == (HANDLE)-1) return -1;

	////not original code
	//BY_HANDLE_FILE_INFORMATION FileInformation;
	//GetFileInformationByHandle(hFile, &FileInformation);
	//unsigned int size = FileInformation.nFileSizeLow;
	//char* buffer = (char*)malloc(size);
	//DWORD readsize;
	//if (buffer != NULL) {
	//	ReadFile(hFile, buffer, size, &readsize, 0);
	//	CloseHandle(hFile);
	//	
	//	MD5byte(&buffer, readsize, (char*)oBuf);

	//	free(buffer);
	//}
	//return 1;

	FILE* pFile;
	pFile = fopen(filepath, "rb");
	if (!pFile)  return -1;
	unsigned char* md5buf = (unsigned char*)md5File(pFile);
	fclose(pFile);

	sprintf((char*)oBuf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", md5buf[0], md5buf[1], md5buf[2], md5buf[3],
		md5buf[4], md5buf[5], md5buf[6], md5buf[7], md5buf[8], md5buf[9], md5buf[10], md5buf[11], md5buf[12], md5buf[13], md5buf[14], md5buf[15]);
	return 1;
}

//TOFIX : convert unixtime
/*
//don't compute with SYSTEMTIME
time_t convertWindowsTimeToUnixTime(_FILETIME ft){
	ULARGE_INTEGER ul{ ft.dwLowDateTime, ft.dwHighDateTime };
	return (unsigned int)((ul.QuadPart - 116444736000000000ULL)/10000000);
}
*/

//437de0
int GetNowUnixtime(void){
	SYSTEMTIME systime;
	_FILETIME filetime;
	int hVal, lVal;
	double dTemp;

	GetLocalTime((LPSYSTEMTIME)&systime);
	systime.wYear = systime.wYear - 369;
	SystemTimeToFileTime(&systime, &filetime);

	dTemp = filetime.dwHighDateTime;
	hVal = (dTemp * -429.4836225);

	dTemp = filetime.dwLowDateTime;
	lVal = (dTemp / 10000000.0);

	return lVal - hVal;
}

//437e90
int GetUnixtimeFromFiletime(FILETIME &filetime) {
	SYSTEMTIME systime;
	int hVal, lVal;
	double dTemp;

	FileTimeToSystemTime(&filetime, &systime);
	systime.wYear = systime.wYear - 369;
	SystemTimeToFileTime(&systime, &filetime);

	dTemp = filetime.dwHighDateTime;
	hVal = (dTemp * -429.4836225);

	dTemp = filetime.dwLowDateTime;
	lVal = (dTemp / 10000000.0);

	return lVal - hVal;
}

//437f00
int GetFileUnixtime(CSTR str) {
	WIN32_FIND_DATA FindFileData;
	LPWIN32_FIND_DATAA lpFindFileData;
	HANDLE hFindFile;

	if ( str.right(1).isSame("\\") ) {
		str.nullAtPos( str.length() - 1 );
	}

	lpFindFileData = &FindFileData;
	hFindFile = FindFirstFileA(str, lpFindFileData);
	if (hFindFile == (HANDLE)-1) {
		ErrorLogFmtAdd("ファイルのLR2TIME取得エラー:%sが見つからない\n", str);
		return -1;
	}
	else {
		FindClose(hFindFile);
		return GetUnixtimeFromFiletime(FindFileData.ftLastWriteTime);
	}
}

//438040
CSTR GetRandomFileOnDir(CSTR path, char fOnlyName) {
	CSTR oBuf;
	//CSTR str1,str2,str3;
	WIN32_FIND_DATA FindFileData;
	LPWIN32_FIND_DATAA lpFindFileData;
	HANDLE hFindFile;
	int fileCount = 0;
	CSTR str1 = CSTR( path.left(path.findStrPos("*")) );
	CSTR str2 = CSTR( path.right(path.length() - str1.length() - 1) );
	CSTR str3 = CSTR( str1 );
	str3.add("*");
	hFindFile = FindFirstFileA(str3, &FindFileData);
	if (hFindFile == (HANDLE)-1) {
		//oBuf = CSTR("ERROR");
		return CSTR("ERROR");
	}
	else {
		do {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp("..", FindFileData.cFileName) && strcmp(".", FindFileData.cFileName)) fileCount++;
			}
		} while (FindNextFileA(hFindFile, &FindFileData));
		FindClose(hFindFile);
		if (fileCount > 0) {
			fileCount = GetRand(fileCount - 1);

			hFindFile = FindFirstFileA(str3, &FindFileData);
			if (hFindFile != (HANDLE)-1) {
				do {
					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						if (strcmp("..", FindFileData.cFileName) && strcmp(".", FindFileData.cFileName)) {
							//LAB_00438327
							int i = 0;
							while (i < fileCount) {
								FindNextFileA(hFindFile, &FindFileData);
								if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
									if (strcmp("..", FindFileData.cFileName) && strcmp(".", FindFileData.cFileName)) i++;
								}
							}
							FindClose(hFindFile);
							path.assign(&str1);
							path.add(FindFileData.cFileName);
							path.add(&str2);
							if (fOnlyName) {
								//oBuf = CSTR(FindFileData.cFileName);
								return CSTR(FindFileData.cFileName);
							}
							else {
								//oBuf = CSTR(path);
								return CSTR(path);
							}
							return oBuf;
						}
					}
					FindNextFileA(hFindFile, &FindFileData);
				} while (true);
			}
		}
		//oBuf = CSTR("ERROR");
		return CSTR("ERROR");
	}
	return oBuf;
}

//438540
bool CheckStringHead(CSTR *bigS, CSTR *head){
	if (bigS->length() <= head->length()) {
		return false;
	}
	return (bigS->left(head->length()).icmp(head) == 0);
}

//4385f0
bool GetStringBodyStr(CSTR *str, CSTR head, CSTR *oBuf){
	if (CheckStringHead(str, &head)) {
		oBuf->assign(str->right(str->length() - head.length() - 1) );
		return true;
	}
	return false;
}

//4386c0
bool GetStringBodyInt(CSTR *str, CSTR haed, int *oBuf) {
	if (CheckStringHead(str, &haed)) {
		*oBuf = atol(str->right(str->length() - haed.length() - 1));
		return true;
	}
	return false;
}

//4387a0
bool GetDifficultyFromToken(CSTR str, CSTR *oLeft, CSTR *oRight, CSTR tokenL, CSTR tokenR, int *oBuf) {
	
	int posL = -1;
	CSTR str1(str);

	//check token existence from rightside
	for (str1.nullAtPos(str1.length() - 1); str1.length() > 1; str1.nullAtPos(str1.length() - 1)) {
		if (posL != -1) break;
		if (str1.right(tokenL.length()).isSame(&tokenL)) {
			posL = str1.length() - tokenL.length();
		}
	}
	if (posL < 1) return false;
	if (!str.right(tokenR.length()).isSame(&tokenR)) return false;

	oLeft->assign( str.left(posL).trimWhiteSpace() );
	oRight->assign( str.right(str.length() - oLeft->length()).trimWhiteSpace() );
	str1.assign(oRight);
	str1.upper();
	if (str1.findStrPos("BEG") > 0) *oBuf = 1;
	else if (str1.findStrPos("HARD") > 0) *oBuf = 3;
	else if (str1.findStrPos("HYPE") > 0) *oBuf = 3;
	else if (str1.findStrPos("HD") > 0) *oBuf = 3;
	else if (str1.findStrPos("5H") > 0) *oBuf = 3;
	else if (str1.findStrPos("7H") > 0) *oBuf = 3;
	else if (str1.findStrPos("10H") > 0) *oBuf = 3;
	else if (str1.findStrPos("14H") > 0) *oBuf = 3;
	else if (str1.findStrPos("9H") > 0) *oBuf = 3;
	else if (str1.findStrPos("DIF") > 0) *oBuf = 3;
	else if (str1.findStrPos("VERYHARD") > 0) *oBuf = 4;
	else if (str1.findStrPos("EX") > 0) *oBuf = 4;
	else if (str1.findStrPos("AN") > 0) *oBuf = 4;
	else if (str1.findStrPos("SHD") > 0) *oBuf = 4;
	else if (str1.findStrPos("5A") > 0) *oBuf = 4;
	else if (str1.findStrPos("7A") > 0) *oBuf = 4;
	else if (str1.findStrPos("10A") > 0) *oBuf = 4;
	else if (str1.findStrPos("14A") > 0) *oBuf = 4;
	else if (str1.findStrPos("9A") > 0) *oBuf = 4;
	else if (str1.findStrPos("ULT") > 0) *oBuf = 4;
	else if (str1.findStrPos("MANI") > 0) *oBuf = 4;
	else if (str1.findStrPos("LUNA") > 0) *oBuf = 4;
	else if (str1.findStrPos("AHO") > 0) *oBuf = 4;
	else if (str1.findStrPos("AFO") > 0) *oBuf = 4;
	else if (str1.findStrPos("ASDF") > 0) *oBuf = 4;
	else if (str1.findStrPos("HELL") > 0) *oBuf = 4;

	return 1;
}

//438c80
bool GetDifficulty(CSTR *str, CSTR head, CSTR *oLeft, CSTR *oRight, int *pDifficulty) {
	
	if (!CheckStringHead(str, &head)) return false;

	CSTR upstr(*str);
	upstr.upper();

	if (upstr.right(4).isSame("HARD")) *pDifficulty = 2;
	if (upstr.right(5).isSame("HYPER")) *pDifficulty = 3;
	if (upstr.right(7).isSame("ANOTHER")) *pDifficulty = 4;
	if (upstr.right(4).isSame("EASY")) *pDifficulty = 1;
	if (upstr.right(2).isSame("EX")) *pDifficulty = 4;
	if (upstr.right(6).isSame("MANIAC")) *pDifficulty = 4;

	str->lastCut(str->length() - head.length() - 1);
	if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("("), CSTR(")"), pDifficulty)) {} //test this method
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("["), CSTR("]"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("-"), CSTR("-"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("\""), CSTR("\""), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("<"), CSTR(">"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("〜"), CSTR("〜"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("【"), CSTR("】"), pDifficulty)) {}
	else { 
		oLeft->assign(str);
		oRight->fillzero();
	}
	return true;
}

//439150
bool IsBmsFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".bme")) return true;
		if (str.isSame(".bms")) return true;
		if (str.isSame(".bml")) return true;
		if (str.isSame(".pms")) return true;
	}
	return false;
}

//439230
bool IsMediaFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".mp3")) return true;
		if (str.isSame(".wav")) return true;
		if (str.isSame(".avi")) return true;
	}
	return false;
}

//439300
bool IsSndFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".mp3")) return true;
		if (str.isSame(".wav")) return true;
	}
	return false;
}

//4393b0
bool IsAviFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".avi")) return true;
	}
	return false;
}

//439450
bool IsLR2Folder(CSTR str) {
	if (str.length() > 10) {
		str.lastCut(10);
		str.lower();
		if (str.isSame(".lr2folder")) return true;
	}
	return false;
}

//439510
bool IsFileExist(CSTR path) {
	HANDLE hFindFile;
	_WIN32_FIND_DATAA findFileData;
	bool isDir = false;
	char *cur;
	char dirFlag = 0;

	if (path.right(1).isSame("\\") == true) {
		isDir = true;
	}
	else if (path.right(1).isSame("/") == true) {
		isDir = true;
	}

	if (isDir) {
		cur = path.atPos(path.length() - 1);
		*cur = 0;
	}
	hFindFile = FindFirstFileA(path, &findFileData);
	FindClose(hFindFile);
	/*if (hFindFile != (HANDLE)-1) {
		FindClose(hFindFile);
	}
	else {
		FindClose(-1);
	}*/
	return hFindFile != (HANDLE)-1;
}

//4396b0 //0:already_exist 1:not_exist 2:changed
int IsFileChanged(unsigned int oldUnixtime, CSTR filepath, int *oNewtime) { 

	HANDLE hFindFile;
	char* lpFileName;
	_WIN32_FIND_DATAA findFileData;

	if (filepath.right(1).isSame("\\")) {
		filepath.nullAtPos(filepath.length() - 1);
	}
	lpFileName = filepath;
	hFindFile = FindFirstFileA(lpFileName, &findFileData);
	if (hFindFile == (HANDLE)-1) {
		*oNewtime = -1;
		FindClose((HANDLE)-1);
		return 1;
	}
	
	unsigned int filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
	if (oldUnixtime < filetime) {
		*oNewtime = filetime;
		FindClose(hFindFile);
		return 2;
	}
	else {
		*oNewtime = oldUnixtime;
		FindClose(hFindFile);
		return 0;
	}
}

//439820
int DealWhiteSpace(CSTR *str) {
	bool bFlag = false;
	do{
		if (str->right(1).isSame(" ")) 	bFlag = true;
		else if (str->right(1).isSame("\t")) bFlag = true;
		else if (str->right(1).isSame("\n")) bFlag = true;
		else if (str->right(1).isSame("\r")) bFlag = true;
		else bFlag = false;

		if (bFlag == false) break;
		str->nullAtPos(str->length() - 1);
	} while (true);
	while (true) {
		if (str->left(1).isSame(",")) {
			if (str->length() < 3) return 1;
		}
		else {
			return 1;
		}
		str->lastCut(str->length() - 1);
	}
}

//439a30 SplitCSV
int SplitCSV(CSTR csvStr, CSVbuf *oBuf, const char *splitter) {
	int pos,i;
	bool bEnd = false;
	
	for (int i = 0; i < 30; i++) {
		oBuf->val[i] = 0;
		oBuf->str[i].fillzero();
	}

	i = 0;
	do {
		pos = csvStr.findStrPos(",");
		if (pos == 0) {
			oBuf->val[i] = 0;
			csvStr.lastCut(csvStr.length() - 1);
		}
		else if (pos < 0) {
			oBuf->str[i].assign(&csvStr);
			oBuf->str[i].left(1).isSame("\"");
			if (oBuf->str[i].left(1).isSame("\"")) {
				if (oBuf->str[i].right(1).isSame("\"")) {
					oBuf->str[i].nullAtPos(oBuf->str[i].length() - 1);
					oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				}
			}
			if (oBuf->str[i].left(1).isSame("!")) {
				oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				oBuf->val[i] = -atol(oBuf->str[i]);
			}
			else {
				oBuf->val[i] = atol(oBuf->str[i]);
			}
			csvStr.lastCut(csvStr.length() - pos - 1);
			bEnd = true;
		}
		else {
			//logic arranged
			oBuf->str[i].assign( csvStr.left(pos) );
			if ( oBuf->str[i].left(1).isSame("\"") ){
				if (oBuf->str[i].right(1).isSame("\"")) {
					oBuf->str[i].nullAtPos(oBuf->str[i].length() - 1);
					oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				}
			}
			if (oBuf->str[i].left(1).isSame("!")) {
				oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				oBuf->val[i] = -atol(oBuf->str[i]);
			}
			else {
				oBuf->val[i] = atol(oBuf->str[i]);
			}
			csvStr.lastCut(csvStr.length() - pos - 1);
		}
		i++;
		if(i>29 || bEnd){
			return 1;
		}
	} while (true);
}

//439dc0
int Base36ToInt(char ch1, char ch2) {
	int ret = 0;

	if (0x61 <= ch1 && ch1 < 0x61 + 26) ret = ch1 - 0x61 + 10;
	else if (0x31 <= ch1 && ch1 < 0x31 + 10) ret = ch1 - 0x30;
	else if (0x41 <= ch1 && ch1 < 0x41 + 26) ret = ch1 - 0x41 + 10;
	ret = ret * 36;

	if (0x61 <= ch2 && ch2 < 0x61 + 26) ret += ch2 - 0x61 + 10;
	else if (0x31 <= ch2 && ch2 < 0x31 + 10) ret += ch2 - 0x30;
	else if (0x41 <= ch2 && ch2 < 0x41 + 26) ret += ch2 - 0x41 + 10;

	return ret;
}

//439e40
int HEXcharToInt(char ch1, char ch2) {
	int ret = 0;

	if (0x61 <= ch1 && ch1 < 0x61 + 6) ret = ch1 - 0x61 + 10;
	else if (0x31 <= ch1 && ch1 < 0x31 + 10) ret = ch1 - 0x30;
	else if (0x41 <= ch1 && ch1 < 0x41 + 6) ret = ch1 - 0x41 + 10;
	ret = ret << 4;

	if (0x61 <= ch2 && ch2 < 0x61 + 6) ret += ch2 - 0x61 + 10;
	else if (0x31 <= ch2 && ch2 < 0x31 + 10) ret += ch2 - 0x30;
	else if (0x41 <= ch2 && ch2 < 0x41 + 6) ret += ch2 - 0x41 + 10;

	return ret;
}

//439ec0
int RoundUp(double val) {
	int ret = val;
	if (ret != val) ret++; //TEST : if not work properly, use ceil from math.h
	return ret;
}

//439ef0
CSTR AssignCRC32(CSTR str) {
	CSTR tmp;
	cstrSprintf(&tmp, "%x", str.CRC32());
	return tmp;
}

//439fa0
CSTR AutomationFactory(){
	switch (GetRand(15)) {
		case 0:
			return CSTR("職権を乱用するRainbow");
		case 1:
			return CSTR("またお前らただし女性限定");
		case 2:
			return CSTR("我こそは通訳つきスピリチュアルズ");
		case 3:
			return CSTR("End of the は食後に使おう");
		case 4:
			return CSTR("ジャンバラヤとyou");
		case 5:
			return CSTR("錆び付いたホタル狩り");
		case 6:
			return CSTR("：！寛大な IN COLOR");
		case 7:
			return CSTR("蹴りたい転落");
		case 8:
			return CSTR("Rainy Placement");
		case 9:
			return CSTR("デビュー戦で奥手");
		case 10:
			return CSTR("秋葉never been mellow");
		case 11:
			return CSTR("静電気焼き鳥ムービー");
		case 12:
			return CSTR("人生とビル・ゲイツ");
		case 13:
			return CSTR("黒鍵の上も歩くRED");
		case 14:
			return CSTR("風雲！寝る");
		case 15:
			return CSTR("オペラ：達人、または解雇");
		default:
			return CSTR("※無断使用ばっかりですが訴えないでください");
	}
}

//43a190
int CountDigit(int num){
	uint ret;
	bool bVar2;

	if (num == 0) {
		return 1;
	}
	if (num < 0) num = -num;
	ret = (num < 0);
	for (; num != 0; num = num / 10) {
		ret++;
	}
	return ret;
}

//43a1d0
int GetDigitNum(int num, int pos) {
	if ((0 < num) && (0 < pos)) {
		if (1 < pos) {
			pos = pos + -1;
			do {
				num = num / 10;
				pos = pos + -1;
			} while (pos != 0);
		}
		return num % 10;
	}
	return 0;
}

//43a230
int FindAltImage(CSTR filename, CSTR dir, CSTR *oBuf) {

	CSTR path;

	path.assign(dir).add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".png") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("png");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".jpg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("jpg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".bmp") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("bmp");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	path.assign(dir).add("..\\").add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".png") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("png");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".jpg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("jpg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".bmp") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("bmp");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	oBuf->fillzero();
	return 0;
}

//43a5f0
int FindAltSound(CSTR filename, CSTR dir, CSTR *oBuf) {

	CSTR path;

	path.assign(dir).add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".ogg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("ogg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".wav") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("wav");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	path.assign(dir).add("..\\").add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".wav") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("wav");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".ogg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("ogg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	oBuf->fillzero();
	return 0;
}

//43a900
CSTR GetRandomFile(CSTR path, char fOnlyName) {
	WIN32_FIND_DATA FindFileData;
	LPWIN32_FIND_DATAA lpFindFileData;
	HANDLE hFindFile;
	CSTR oBuf;
	int count;

	//call function if wildcard is on directory
	if (path.findStrPos("*/") != -1 || path.findStrPos("*\\") != -1 || path.right(1).isSame("*")) {
		return CSTR(GetRandomFileOnDir(path, fOnlyName));
	}

	//count files for random
	hFindFile = FindFirstFileA(path, &FindFileData);
	if (hFindFile == (HANDLE)-1) return CSTR("ERROR");
	
	count = 0;
	do {
		count++;
	} while (FindNextFileA(hFindFile, &FindFileData));
	if (count < 1) return CSTR("ERROR");

	count = GetRand(count - 1);

	//get file by random
	hFindFile = FindFirstFileA(path, &FindFileData);
	if (hFindFile == (HANDLE)-1) return CSTR("ERROR");

	for (int i = 0; i < count; i++) {
		FindNextFile(hFindFile, &FindFileData);
	}
	FindClose(hFindFile);
	path.assign(path.getDirectory());
	path.add(FindFileData.cFileName);
	if (fOnlyName) {
		path.assign(FindFileData.cFileName);
		path.nullAtPos(path.findStrPos("."));
	}
	return CSTR(path);
}

//43abe0
CSTR GetRandomFileNoError(CSTR path, CSTR dir) {
	CSTR filepath = CSTR();
	filepath.assign(GetRandomFile(path, 0));
	if (filepath.isDiff("ERROR")) return CSTR(filepath);
	dir.add(&path);
	filepath.assign(GetRandomFile(path, 0));
	if (filepath.isDiff("ERROR")) return CSTR(filepath);
	return CSTR(path);
}

//strclass.cpp
//LR2startup.cpp

int EnabledInsane;
////DB interact
CRITICAL_SECTION DB_lock;
//4440f0
int DB_EnterCriticalSection() {
	EnterCriticalSection(&DB_lock);
	return 1;
}

//444110
int DB_LeaveCriticalSection() {
	LeaveCriticalSection(&DB_lock);
	return 1;
}

//444130 //TODO more readable
bool ANSItoUTF8(LPCSTR str, char *oBuf, size_t *oSize){
	int cchWideChar;
	LPWSTR lpWideCharStr;
	LPSTR lpMultiByteStr;
	size_t size;

	*oSize = 0;
	cchWideChar = MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)0x0, 0);
	lpWideCharStr = (LPWSTR)malloc(cchWideChar * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, str, -1, lpWideCharStr, cchWideChar);
	size = WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, (LPSTR)0x0, 0, (LPCSTR)0x0, (LPBOOL)0x0);
	if (oBuf == (char *)0x0) {
		*oSize = size;
		delete(lpWideCharStr);
		return true;
	}
	lpMultiByteStr = (LPSTR)malloc(size * 2);
	memset(lpMultiByteStr, '\0', size * 2);
	WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, lpMultiByteStr, size, (LPCSTR)0x0, (LPBOOL)0x0);
	size = lstrlenA(lpMultiByteStr);
	*oSize = size;
	memcpy(oBuf, lpMultiByteStr, size);
	delete(lpWideCharStr);
	delete(lpMultiByteStr);
	return true;
}

//444210
bool UTF8toANSI(LPCSTR str, void *oBuf, size_t *oSize){
	int cchWideChar;
	LPWSTR lpWideCharStr;
	LPSTR lpMultiByteStr;
	size_t sVar1;
	
	*oSize = 0;
	cchWideChar = MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)0x0, 0);
	lpWideCharStr = (LPWSTR)malloc(cchWideChar * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, lpWideCharStr, cchWideChar);
	sVar1 = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, (LPSTR)0x0, 0, (LPCSTR)0x0, (LPBOOL)0x0);
	if (oBuf == (void *)0x0) {
		*oSize = sVar1;
		delete(lpWideCharStr);
		return true;
	}
	lpMultiByteStr = (LPSTR)malloc(sVar1 * 2);
	memset(lpMultiByteStr, '\0', sVar1 * 2);
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, lpMultiByteStr, sVar1, (LPCSTR)0x0, (LPBOOL)0x0);
	sVar1 = lstrlenA(lpMultiByteStr);
	*oSize = sVar1;
	memcpy(oBuf, lpMultiByteStr, sVar1);
	delete(lpWideCharStr);
	delete(lpMultiByteStr);
	return true;
}

//4442f0
int SQL_Run(CSTR queryStr, sqlite3 *sql){
	LPCSTR lpMultiByteStr;
	uint uVar1;
	int cchWideChar;
	LPCWSTR lpWideCharStr;
	size_t sVar2;
	char *oBuf;
	size_t local_10;
	int *local_c;
	undefined *puStack8;

	lpMultiByteStr = queryStr;
	cchWideChar = MultiByteToWideChar(CP_ACP, 0, queryStr, -1, (LPWSTR)0x0, 0);
	lpWideCharStr = (LPWSTR)malloc(cchWideChar * 2 + 2);
	
	MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, -1, (LPWSTR)lpWideCharStr, cchWideChar);
	sVar2 = WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, (LPSTR)0x0, 0, (LPCSTR)0x0, (LPBOOL)0x0);
	local_10 = sVar2;
	delete(lpWideCharStr);
	oBuf = (char *)malloc(sVar2 + 1);
	
	memset(oBuf, '\0', sVar2 + 1);
	ANSItoUTF8(queryStr, oBuf, &local_10);
	oBuf[local_10] = '\0';
	uVar1 = sqlite3_exec(sql, oBuf, NULL, (void *)0x0, (char **)0x0);
	delete(oBuf);
	return uVar1;
}

//4443f0
int SQL_prepare(CSTR queryStr, sqlite3 *sql, sqlite3_stmt **ppStmt){
	LPCSTR lpMultiByteStr;
	uint uVar1;
	int iVar2;
	LPCWSTR lpWideCharStr;
	size_t sVar3;
	char *oBuf;
	size_t local_10;
	int *local_c;

	lpMultiByteStr = queryStr;

	iVar2 = MultiByteToWideChar(0, 0, queryStr, -1, (LPWSTR)0x0, 0);
	lpWideCharStr = (LPCWSTR)malloc(iVar2 * 2 + 2);
	MultiByteToWideChar(0, 0, lpMultiByteStr, -1, (LPWSTR)lpWideCharStr, iVar2);
	sVar3 = WideCharToMultiByte(0xfde9, 0, lpWideCharStr, -1, (LPSTR)0x0, 0, (LPCSTR)0x0, (LPBOOL)0x0);
	local_10 = sVar3;
	delete(lpWideCharStr);
	oBuf = (char *)malloc(sVar3 + 1);
	memset(oBuf, '\0', sVar3 + 1);
	ANSItoUTF8(queryStr, oBuf, &local_10);
	oBuf[local_10] = '\0';
	iVar2 = sqlite3_prepare(sql, oBuf, -1, ppStmt, NULL);
	delete(oBuf);
	return iVar2;
}

//4444f0
CSTR SQL_GetColumn(int i, sqlite3_stmt *pStmt){
	CSTR oBuf;
	size_t size;
	LPCSTR columnText;

	if (sqlite3_column_type(pStmt, i) == SQLITE_NULL) {
		oBuf.fillzero();
	}
	else {
		sqlite3_column_bytes(pStmt, i);
		columnText = (LPCSTR)sqlite3_column_text(pStmt, i);
		UTF8toANSI(columnText, NULL, &size);
		oBuf.resize2(size + 1);
		memset(oBuf.body, 0, size + 1);
		UTF8toANSI(columnText, oBuf.body, &size);
		*oBuf.atPos(size) = 0;
	}
	return oBuf;
}

////
//444600
int SwapBmsList(SONGSELECT *ss){
	int tmp;
	SONGDATA *tmplist;

	tmplist = ss->bmsList;
	ss->bmsList = ss->prevList;
	ss->prevList = tmplist;

	tmp = ss->bmsListSize;
	ss->bmsListSize = ss->prevListSize;
	ss->prevListSize = tmp;

	tmp = ss->bmsListCount;
	ss->bmsListCount = ss->prevListCount;
	ss->prevListCount = tmp;

	tmp = ss->listTopbar;
	ss->listTopbar = ss->prevTopbar;
	ss->prevTopbar = tmp;

	tmp = ss->listCalculatedBar;
	ss->listCalculatedBar = ss->prevCalculatedBar;
	ss->prevCalculatedBar = tmp;
	return 1;
}

//444650
int InitBmsList(SONGSELECT *ss) {
	ss->bmsListSize = 1000;
	ss->cur = 0;
	ss->bmsList = (SONGDATA*)malloc(sizeof(SONGDATA) * 1000);
	for (int i = 0; i < ss->bmsListSize; i++) {
		memset(&ss->bmsList[i], 0, sizeof(SONGDATA));
	}

	ss->prevListSize = 1000;
	ss->prevList = (SONGDATA*)malloc(sizeof(SONGDATA) * 1000);
	for (int i = 0; i < ss->bmsListSize; i++) { //TOFIX : bmslistsize->prevlistsize
		memset(&ss->prevList[i], 0, sizeof(SONGDATA));
	}

	for (int i = 0; i < 1000; i++) {
		ss->stack_query[i].fillzero();
		ss->stack_searchTitle[i].fillzero();
		ss->stack_folderPath[i].fillzero();
		ss->stack_isFolder[i] = 0;
	}
	ss->prevCalculatedBar = 0;//DEBUG: init is not in original code
	return 1;
}

//444730
int InitSongData(SONGDATA *song){
	song->artist.fillzero();
	song->genre.fillzero();
	song->level = 0;
	song->filepath.fillzero();
	song->subartist.fillzero();
	song->subtitle.fillzero();
	song->title.fillzero();
	song->fulltitle.fillzero();
	song->tag.fillzero();
	song->hash.fillzero();
	song->stagefile.fillzero();
	song->banner.fillzero();
	song->backBMP.fillzero();
	song->folder.fillzero();
	song->difficulty = 0;
	song->folderType = 0;
	song->keymode = 0;
	song->maxBPM = 0;
	song->minBPM = 0;
	song->bga = 0;
	song->txt = 0;
	song->longnote = 0;
	song->random = 0;
	song->judge = 0;
	song->isStagefile = 0;
	song->isBanner = 0;
	song->isBackBMP = 0;
	song->replayExist = 0;
	song->favorite = 0;
	song->adddate = 0;
	song->difficultyCount = 0;
	song->exlevel = 0;
	song->difficultyLevel[0] = -1;
	song->difficultyExist[0] = 0;
	song->difficultyLevelBarLamp[0] = -1;
	song->difficultyLevel[1] = -1;
	song->difficultyExist[1] = 0;
	song->difficultyLevelBarLamp[1] = -1;
	song->difficultyLevel[2] = -1;
	song->difficultyExist[2] = 0;
	song->difficultyLevelBarLamp[2] = -1;
	song->difficultyLevel[3] = -1;
	song->difficultyExist[3] = 0;
	song->difficultyLevelBarLamp[3] = -1;
	song->difficultyLevel[4] = -1;
	song->difficultyExist[4] = 0;
	song->difficultyLevelBarLamp[4] = -1;
	for (int i = 0; i < 10; i++) {
		song->courseTitle[i].fillzero();
		song->courseSubtitle[i].fillzero();
		song->courseHash[i].fillzero();
		song->courseLevel[i] = -1;
		song->courseTotalnote[i] = 0;
		song->courseStageDifficulty[i] = 0;
		song->courseKeys[i] = 0;
	}
	song->courseID = -1;
	song->courseStageCount = 0;
	song->coursePlayable = 0;
	memset(&song->mybest, 0, sizeof(STATUS));
	memset(&song->rivalRecord, 0, sizeof(STATUS));
	(song->mybest).minbp = -1;
	song->courseType = -1;
	song->courseIR = 0;
	song->courseReadOnly = 0;
	if (song->grHandle != -1) {
		DeleteGraph(song->grHandle);
	}
	song->grHandle = -1;
	return 1;
}

//4448f0
int WriteGhostInDatabase(sqlite3 *sql, CSTR songMD5, PLAYSCORE *score) {

	ErrorLogAdd("データベースにゴーストを書き込みます\n");
	CSTR ghostdata = score->EncodeGhostData();
	CSTR query;
	cstrSprintf(&query, "UPDATE score SET ghost = \'%s\' WHERE hash = \'%s\'", ghostdata, songMD5);
	SQL_Run(query, sql);
	ErrorLogAdd("ゴーストの書き込みが終了しました\n");
	return 0;
}

//4449f0
int ReadGhostToScore(sqlite3 *sql, CSTR songMD5, PLAYSCORE *score) {

	char query[1024];
	sqlite3_stmt *pStmt;
	CSTR ghostdata;

	ErrorLogAdd("データベースからゴーストを読み込みます\n");

	sqlite3_snprintf(0x400, query, "SELECT ghost FROM score WHERE hash = \'%q\'", songMD5);
	SQL_prepare(query, sql, &pStmt);

	if (sqlite3_step(pStmt) == 100) {
		ghostdata = SQL_GetColumn(0, pStmt);
	}
	sqlite3_finalize(pStmt);

	if (ghostdata.length() == 0) {
		score->InitJudgeQueue();
		return 1;
	}
	
	score->DecodeGhostData(ghostdata);
	return 1;
}

//444b80
CSTR ReadGhost(sqlite3 *sql, CSTR songMD5) {

	char query[1024];
	sqlite3_stmt *pStmt;
	CSTR ghostdata;

	ErrorLogAdd("データベースからゴーストを読み込みます\n");

	sqlite3_snprintf(0x400, query, "SELECT ghost FROM score WHERE hash = \'%q\'", songMD5);
	SQL_prepare(query, sql, &pStmt);

	if (sqlite3_step(pStmt) == 100) {
		ghostdata = SQL_GetColumn(0, pStmt);
	}
	sqlite3_finalize(pStmt);

	return ghostdata;
}

//444d00
int LoadBMSMETAFromDB(BMSMETA *meta, sqlite3 *sql) {
	char str[1024];
	sqlite3_stmt *pStmt;
	sqlite3_snprintf(1024, str, "SELECT * FROM tag WHERE hash = \'%q\'", meta->hash.body);
	SQL_prepare(str, sql, &pStmt);
	if(sqlite3_step(pStmt) == 100){
		meta->title = SQL_GetColumn(1, pStmt);
		meta->subtitle = SQL_GetColumn(2, pStmt);
		meta->genre = SQL_GetColumn(3, pStmt);
		meta->artist = SQL_GetColumn(4, pStmt);
		meta->subartist = SQL_GetColumn(5, pStmt);
		meta->tag = SQL_GetColumn(6, pStmt);
		meta->selLevel = sqlite3_column_int(pStmt, 7);
		meta->difficulty = sqlite3_column_int(pStmt, 8);
		meta->keymode = sqlite3_column_int(pStmt, 9);
		meta->exlevel = sqlite3_column_int(pStmt, 10);
		return 1;
	}
	return 0;
}

//444f90
int SetUndefinedDifficulty(sqlite3 *sql) {
	char str[1024];
	sqlite3_stmt *pStmt;
	CSTR folder;
	int mode, difficulty;

	sqlite3_prepare(sql, "SELECT difficulty,folder,mode,path FROM song ORDER BY folder,mode,karinotes", -1, &pStmt, NULL);
	while (sqlite3_step(pStmt) == 100) {
		if (0 <= sqlite3_column_int(pStmt, 0) && sqlite3_column_int(pStmt, 0) <= 5) {
			cstrSprintf(&folder, "%s", sqlite3_column_text(pStmt, 1));
			mode = sqlite3_column_int(pStmt, 2);
			difficulty = sqlite3_column_int(pStmt, 0);
		}
		else {
			CSTR newfolder;
			cstrSprintf(&newfolder, "%s", sqlite3_column_text(pStmt, 1));
			if (folder.isSame(newfolder) && (mode == sqlite3_column_int(pStmt, 2))) {
				difficulty++;
				if (difficulty == 5) difficulty = 4;
				else if (difficulty < 0) difficulty = 2;
				else if (difficulty > 5) difficulty = 5;
			}
			else {
				difficulty = 2;
			}
			SQL_Run(sqlite3_snprintf(1024, str, "UPDATE song SET difficulty = %d WHERE path = \'%q\'", difficulty, SQL_GetColumn(3, pStmt)), sql);
			folder = SQL_GetColumn(1, pStmt);
			mode = sqlite3_column_int(pStmt, 2);
		}
	}
	sqlite3_finalize(pStmt);
	return 1;
}

//445220
int CMP_SongDataByExLevel(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	if (s1->keymode != s2->keymode) {
		return s1->keymode - s2->keymode;
	}

	if(s1->exlevel > 0 && s2->exlevel > 0) return s1->exlevel - s2->exlevel;
	if (s1->exlevel > 0 && s2->exlevel <= 0) return 1;
	if (s1->exlevel <= 0 && s2->exlevel > 0) return -1;
	if(s1->exlevel <= 0 && s2->exlevel <= 0) return s1->level - s2->level;

}

//445270
int CMP_SongDataByTitle(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	CSTR c1(s1->title);
	c1.upper();
	CSTR c2(s2->title);
	c2.upper();

	if (c1.isDiffHigh(&c2)) return 1;
	if (c1.isSame(&c2)) {
		if (s1->difficulty == 5 && s2->difficulty != 5) return 1;
		if (s1->difficulty != 5 && s2->difficulty != 5) return s1->level - s2->level;
		if (s1->difficulty == 5 && s2->difficulty == 5) return s1->level - s2->level;
		if (s1->difficulty != 5 && s2->difficulty == 5) return -1;
	}
	if (c1.isDiffLow(&c2)) return -1;

	return 0;
}

//445420
int CMP_SongDataByScore(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	return ((s1->mybest.total_notes) ? s1->mybest.stat_exscore * 10000 / s1->mybest.total_notes * 2 : 0)
		 - ((s2->mybest.total_notes) ? s2->mybest.stat_exscore * 10000 / s2->mybest.total_notes * 2 : 0);
}

//445480
int CMP_SongDataByRivalScore(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	return (s2->rivalRecord.stat_exscore - s2->mybest.stat_exscore) - (s1->rivalRecord.stat_exscore - s1->mybest.stat_exscore);
}

//4454c0
int CMP_SongDataByDirectory(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	CSTR c1(s1->folder);
	c1.upper();
	CSTR c2(s2->folder);
	c2.upper();

	if (c1.isDiffHigh(&c2)) return 1;
	if (c1.isSame(&c2)) {
		if (s1->keymode == s2->keymode)
			return (s1->level - s2->level) + (s1->difficulty == 5) * 100 - (s2->difficulty == 5) * 100;
		else
			return s1->keymode - s2->keymode;
	}
	if (c1.isDiffLow(&c2)) return -1;

	return 0;
}

//445680
int CMP_SongDataByPath(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	CSTR c1(s1->filepath);
	c1.upper();
	CSTR c2(s2->filepath);
	c2.upper();

	if (c1.isDiffHigh(&c2)) return 1;
	if (c1.isDiffLow(&c2)) return -1;

	return 0;
}

//4457c0
CSTR MakeScoreHash(STATUS *stat, CSTR *passMD5, CSTR *songMD5) {
	CSTR str;

	cstrSprintf(&str, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%s%s%d", stat->playcount, stat->stat_pgreat,
		stat->stat_poor, stat->stat_good, stat->stat_great, stat->stat_bad, stat->total_notes,
		stat->clear, stat->stat_maxcombo, stat->clearcount, stat->failcount, stat->rank,
		stat->minbp, stat->rate, stat->clear_db, stat->op_history, passMD5->body, songMD5->body, 1);

	return MD5str(str);
}

//4458b0
bool isSameScoreHash(STATUS *stat, CSTR *passMD5, CSTR *songMD5, CSTR *besthash) {
	CSTR newhash, althash;

	newhash = MakeScoreHash(stat, passMD5, songMD5);
	if (newhash.isSame(besthash)) return true;

	if (stat->stat_pgreat == stat->total_notes && stat->rank == 8) {
		stat->rank = 9;
		newhash = MakeScoreHash(stat, passMD5, songMD5);
		if (newhash.isSame(besthash)) {
			stat->rank = 8;
			return true;
		}
	}
	if (stat->stat_pgreat == stat->total_notes && stat->rank == 9) {
		stat->rank = 8;
		newhash = MakeScoreHash(stat, passMD5, songMD5);
		if (newhash.isSame(besthash)) {
			return true;
		}
	}

	if (stat->rank == 1) {
		stat->rank = 0;
		newhash = MakeScoreHash(stat, passMD5, songMD5);
		if (newhash.isSame(besthash)) {
			stat->rank = 1;
			return true;
		}
	}
	if (stat->rank == 0) {
		stat->rank = 1;
		newhash = MakeScoreHash(stat, passMD5, songMD5);
		if (newhash.isSame(besthash)) {
			return true;
		}
	}

	return false;
}

//445af0
int UpdateScoreDB(CSTR hash, STATUS *stat, sqlite3 *sql, CSTR *passMD5) {

	if (stat->rank > 7) stat->rank = 8;
	if (stat->rank < 1 && stat->stat_great + stat->stat_pgreat > 0) stat->rank = 1;
	CSTR query;
	CSTR scoreMD5(MakeScoreHash(stat, passMD5, &hash));
	sqlite3_exec(sql, "BEGIN", 0, 0, 0);
	cstrSprintf(&query, "INSERT INTO score (hash,clear ,perfect ,great ,good ,bad ,poor ,totalnotes ,maxcombo ,minbp ,playcount ,clearcount ,failcount ,op_history , rank , rate , clear_db , scorehash , clear_sd , clear_ex , op_best , rseed , complete) VALUES(\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d)"
		, hash, stat->clear, stat->stat_pgreat, stat->stat_great, stat->stat_good, stat->stat_bad, stat->stat_poor, stat->total_notes, stat->stat_maxcombo, stat->minbp, stat->playcount, stat->clearcount, stat->failcount, stat->op_history, stat->rank, stat->rate, stat->clear_db, scoreMD5, stat->clear_sd, stat->clear_ex, stat->op_best, stat->rseed, stat->complete);
	if (SQL_Run(query, sql)){
		cstrSprintf(&query, "UPDATE score SET clear=%d ,perfect=%d ,great=%d ,good=%d ,bad=%d ,poor=%d ,totalnotes=%d ,maxcombo=%d ,minbp=%d ,playcount=%d ,clearcount=%d ,failcount=%d ,op_history=%d ,scorehash=\'%s\',rank=%d,rate=%d,clear_db=%d,clear_sd=%d,clear_ex=%d,op_best=%d,rseed=%d,complete=%d WHERE hash = \'%s\'"
			, stat->clear, stat->stat_pgreat, stat->stat_great, stat->stat_good, stat->stat_bad, stat->stat_poor, stat->total_notes, stat->stat_maxcombo, stat->minbp, stat->playcount, stat->clearcount, stat->failcount, stat->op_history, scoreMD5, stat->rank, stat->rate, stat->clear_db, stat->clear_sd, stat->clear_ex, stat->op_best, stat->rseed, stat->complete, hash);
		SQL_Run(query, sql);
	}
	sqlite3_exec(sql, "COMMIT", 0, 0, 0);
	return 1;
}

//445d30
int UpdateSongDataTag(SONGDATA *song, sqlite3 *sql){
	char query[1024];

	SQL_Run("BEGIN", sql);
	if (song->keymode >= 5) {
		sqlite3_snprintf(0x400, query, "UPDATE song SET title=\'%q\',subtitle=\'%q\',genre=\'%q\',artist=\'%q\',subartist=\'%q\',tag=\'%q\',level=%d,difficulty=%d,mode=%d,favorite=%d,exlevel=%d WHERE hash = \'%q\'",
			song->title, song->subtitle, song->genre, song->artist, song->subartist, song->tag, song->level, song->difficulty, song->keymode, song->favorite, song->exlevel, song->hash);
		SQL_Run(query, sql);

		sqlite3_snprintf(0x400, query, "INSERT INTO tag (hash,title,subtitle,genre,artist,subartist,tag,level,difficulty,mode,exlevel) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,%d,%d)",
			song->hash, song->title, song->subtitle, song->genre, song->artist, song->subartist, song->tag, song->level, song->difficulty, song->keymode, song->exlevel);
		if (SQL_Run(query, sql) != 0) {
			sqlite3_snprintf(0x400, query, "UPDATE tag SET title=\'%q\',subtitle=\'%q\',genre=\'%q\',artist=\'%q\',subartist=\'%q\',tag=\'%q\',level=%d,difficulty=%d,mode=%d WHERE hash = \'%q\'",
				song->title, song->subtitle, song->genre, song->artist, song->subartist, song->tag, song->level, song->filepath, song->keymode, song->hash);
			SQL_Run(query, sql);
		}
	}
	else {
		sqlite3_snprintf(0x400, query, "UPDATE folder SET title=\'%q\',subtitle=\'%q\',category=\'%q\',info_a=\'%q\',info_b=\'%q\',command=\'%q\',max=%d WHERE path = \'%q\'",
			song->title, song->subtitle, song->genre, song->artist, song->subartist, song->tag, song->level, song->filepath);
		SQL_Run(query, sql);
	}
	SQL_Run("COMMIT", sql);
	return 1;
}

//445fa0
CSTR MakePlayerStatHash(PLAYERSTATISTIC *ps) {
	CSTR tmp;
	cstrSprintf(&tmp, "%d%d%d%d%d%d%d%d%d%d%d%s%d%d%d%d%d%d%d%d%d%d%d", ps->bad, ps->clear, ps->combo, ps->fail, ps->good, ps->great, ps->maxcombo, ps->perfect, ps->playcount, ps->playtime, ps->poor, ps->passMD5,
		ps->grade9, ps->grade10, ps->grade14, ps->grade5, ps->grade7, ps->gradeversion, ps->trial, ps->trialversion, ps->systemversion, ps->option, 1);
	return MD5str(tmp);
}

//4460a0
int UpdatePlayerStat(PLAYERSTATISTIC *ps, sqlite3 *sql) {
	char query[1024];
	CSTR hash = MakePlayerStatHash(ps);
	sqlite3_snprintf(1024, query, "UPDATE player SET playcount= %d , clear = %d , fail = %d , perfect = %d , great = %d , good = %d , bad = %d , poor = %d , playtime = %d , combo = %d , maxcombo = %d,scorehash=\'%q\' ,grade_7=%d,grade_5=%d,grade_14=%d,grade_10=%d,grade_9=%d,trial = %d , option=%d,systemversion=%d,gradeversion=%d,trialversion=%d",
		ps->playcount, ps->clear, ps->fail, ps->perfect, ps->great, ps->good, ps->bad, ps->poor, ps->playtime, ps->combo, ps->maxcombo, hash,
		ps->grade7, ps->grade5, ps->grade14, ps->grade10, ps->grade9, ps->trial, ps->option, ps->systemversion, ps->gradeversion, ps->trialversion);
	SQL_Run(query, sql);
	return 1;
}

//4461f0
int SaveIRID(int IRID, CSTR ID) {

	CSTR scorefile;
	sqlite3 *pDb;
	cstrSprintf(&scorefile, "LR2files/Database/Score/%s.db", ID);
	sqlite3_open(scorefile, &pDb);
	CSTR query;
	cstrSprintf(&query, "UPDATE player SET irid = %d", IRID);
	SQL_Run(query, pDb);
	sqlite3_close(pDb);
	ErrorLogAdd("IRIDをセーブしました\n");
	return 1;
}

//446300
int EditTag(SONGDATA *song, sqlite3 *sql) {

	HANDLE hFindFile;
	_WIN32_FIND_DATAA findFileData;
	sqlite3_stmt *stmt;
	CSTR query;

	hFindFile = FindFirstFileA(song->filepath, &findFileData);
	if (hFindFile == (HANDLE)-1) {
		FindClose(hFindFile);
		return -1;
	}

	FindClose(hFindFile);

	int wtime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
	int ntime = GetNowUnixtime();
	int temp;
	if (song->folderType == 0 || song->folderType == 5) {

		sqlite3_snprintf(1024, query, "SELECT adddate FROM song WHERE path = \'%q\'", song->filepath);
		SQL_prepare(query, sql, &stmt);
		if (sqlite3_step(stmt) == 100) {
			temp = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
		BMSMETA meta;
		ParseBMSMETA(&meta, song->filepath, 0);

		sqlite3_snprintf(1024, query, "DELETE FROM song WHERE path=\'%q\'", song->filepath);
		SQL_Run(query, sql);
		sqlite3_snprintf(1024, query, "DELETE FROM tag WHERE hash=\'%q\'", song->hash);
		SQL_Run(query, sql);

		sqlite3_snprintf(1024, query, "INSERT INTO song (hash,title,subtitle,genre,artist,subartist,level,date,path,folder,stagefile,banner,backbmp,parent,maxbpm,minbpm,random,longnote,judge,mode,bga,difficulty,favorite,type,txt,karinotes,adddate,exlevel) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,%d)",
			meta.hash, meta.title, meta.subtitle, meta.genre, meta.artist, meta.subartist, meta.selLevel, wtime, song->filepath, AssignCRC32(meta.folderpath), meta.stagefilepath,meta.bannerpath,meta.backBMPpath, AssignCRC32(meta.parentfolderpath), meta.maxbpm,meta.minbpm, meta.random,meta.longnote,meta.judge,meta.keymode,meta.bga,meta.difficulty,meta.hasTxt,meta.notecount,temp,meta.exlevel);
		SQL_Run(query, sql);

		if (meta.difficulty <= 0 || meta.difficulty > 5) {
			SetUndefinedDifficulty(sql);
		}

		sqlite3_snprintf(1024, query, "SELECT difficulty FROM song WHERE path = \'%q\'", song->filepath);
		SQL_prepare(query, sql, &stmt);
		if (sqlite3_step(stmt) == 100) {
			temp = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);

		song->title = meta.title;
		song->artist = meta.artist;
		song->genre = meta.genre;
		song->subtitle = meta.subtitle;
		song->subartist = meta.subartist;
		song->keymode = meta.keymode;
		song->tag = meta.tag;
		song->level = meta.selLevel;
		song->favorite = 0;
		song->difficulty = temp;
	}
	else if (IsLR2Folder(song->filepath)) {
		sqlite3_snprintf(1024, query, "DELETE FROM folder WHERE path=\'%q\'", song->filepath);
		SQL_Run(query, sql);
		BMSMETA meta;
		ParseBMSMETA(&meta, song->filepath, 0);
		sqlite3_snprintf(1024, query, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , adddate ) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , %d) ",
			song->filepath, meta.title, song->folder, meta.genre, meta.artist, meta.subartist, meta.tag, meta.selLevel, wtime, song->folderType, ntime);
		SQL_Run(query, sql);

		song->title = meta.title;
		song->artist = meta.artist;
		song->genre = meta.genre;
		song->subtitle = meta.subtitle;
		song->subartist = meta.subartist;
		song->keymode = meta.keymode;
		song->tag = meta.tag;
		song->level = meta.selLevel;
	}
	else if (song->folderType == 1 || song->folderType == 2) {
		sqlite3_snprintf(1024, query, "UPDATE folder SET title=\'%q\',subtitle=\'\',category=\'\',info_a=\'\',info_b=\'\',command=\'\',max=0 WHERE path = \'%q\'", song->filepath.getFilename(), song->filepath);
		SQL_Run(query, sql);

		song->title = song->filepath.getFilename();
		song->artist.fillzero();
		song->genre.fillzero();
		song->subtitle.fillzero();
		song->subartist.fillzero();
		song->tag.fillzero();
		song->level = 0;
	}
	return 1;
}

//4469c0
int DeleteScoreFromDB(CSTR hash, sqlite3 *sql){
	
	char str[256];

	if (hash.length() == 32) {
		sqlite3_snprintf(0x100, str, "DELETE FROM score WHERE hash=\'%q\'", hash.body);
		SQL_Run(str, (sqlite3 *)sql);
		return 1;
	}
	else return 0;
}

//446ab0 //TODO : I need to understand this and rename variables
int LoadFolderDataFromDB(CSTR query, SONGDATA *song, sqlite3 *sql, int difficulty, int key, int sort, int maxCount, CONFIG_SELECT *cfg_select, char flag) {
	sqlite3_stmt *stmt;
	SONGDATA* slist;
	SONGDATA sd;
	int keys = 0;
	size_t nowsize, maxsize;
	int slistCount;
	CSTR nowFolder, newFolder, workingFolder;
	int nowMode, nowDifficulty;
	
	//key 0:ALL 1:SINGLE 2:7keys 3:5keys 4:DOUBLE 5:14keys 6:10keys 7:9buttons

	if ((cfg_select->ignorekeyall == 1) && (key == 0)) key = 1;
	if ((cfg_select->ignorekeysingle == 1) && (key == 1)) key = 2;
	if ((cfg_select->ignorekeydouble == 1) && (key == 4)) key = 5;
	if ((cfg_select->ignoredp == 1) && (key < 7)) key = 7; //TOFIX? : && key > 3
	if ((cfg_select->ignorepms == 1) && (key == 7)) key = 0;
	if ((cfg_select->ignorekeyall == 1) && (key == 0)) key = 1;
	if ((cfg_select->ignoredifficultyall == 1) && (difficulty == 0)) difficulty = 1;

	if (query.findStrPos("__NEWSONG__") > -1) {
		cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE adddate > %d", GetNowUnixtime() - cfg_select->titleflash * 3600);
	}

	if (query.left(6).isDiff("SELECT")) return -1;

	if (maxCount == 1 && query.findStrPos("random()") == -1) return 0;

	if (query.findStrPos("ORDER BY") != -1) {
		difficulty = 0;
		key = 0;
	}
	if (flag) {
		difficulty = 0;
	}

	if (query.findStrPos("ORDER BY") == -1) {
		switch (key) {
			case 2:
				keys = 7;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC");
				break;
			case 3:
				keys = 5;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC");
				break;
			case 5:
				keys = 14;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC");
				break;
			case 6:
				keys = 10;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC");
				break;
			case 7:
				keys = 9;
				query.add(" ORDER BY folder ASC,difficulty ASC");
				break;
		}
	}

	int keymode_A = 0;
	slist = (SONGDATA*)malloc(sizeof(SONGDATA) * 1000);
	memset(slist, 0, sizeof(SONGDATA) * 1000);
	slistCount = 0;
	SQL_prepare(query, sql, &stmt);

	int difficultyCount = 0;
	int folderSongCount = 0;
	int keymode_B = 0;

	nowsize = sizeof(SONGDATA) * 1000;
	maxsize = 2000;

	while (sqlite3_step(stmt) == 100) {
		if (maxCount != 0 && slistCount == maxCount) break;

		int mode = sqlite3_column_int(stmt, 18);
		if (cfg_select->ignoredp == 1 && mode >= 10) continue;
		if (cfg_select->ignorepms == 1 && mode == 9) continue;

		sd.keymode = mode;
		
		if( (maxsize - 1001) == slistCount){
			int newsize = nowsize + sizeof(SONGDATA) * 1000;
			slist = (SONGDATA*)realloc(slist, newsize);
			if (maxsize - 1000 < maxsize) {
				memset((void*)((int)slist + nowsize), 0, sizeof(SONGDATA) * 1000);
			}
			maxsize += 1000;
			keymode_A = keymode_B;
			nowsize = newsize;
		}

		sd.difficulty = sqlite3_column_int(stmt, 15);
		sd.favorite = sqlite3_column_int(stmt, 24);
		if (cfg_select->ignore5key == 1) {
			cstrSprintf(&sd.folder, "%s", sqlite3_column_text(stmt, 9));
			if (nowFolder.isDiff(&sd.folder)) {
				folderSongCount = 0;
			}
			else {
				if ((keymode_A != 5 || sd.keymode != 7) && (keymode_A != 10 || sd.keymode != 14)) {
					if (keymode_A != sd.keymode) {
						folderSongCount = 0;
					}
				}
				else {
					slistCount -= folderSongCount;
					folderSongCount = 0;
				}
			}
		}

		//if (cfg_select->ignore5key == 1) {
		//	cstrSprintf(&sd.folder, "%s", sqlite3_column_text(stmt, 9));
		//	if (nowFolder.isDiff(&sd.folder)) {
		//		//v4 = 0;
		//	}
		//	else {
		//		if (keymode_A == 5) {
		//			if (sd.keymode == 7) {
		//				//slistCount -= v4;
		//				//v4=0;
		//				//jmp
		//			}
		//		}
		//		else if (keymode_A == 10 && sd.keymode == 14) {
		//			//slistCount -= v4;
		//			//v4;
		//			//jmp
		//		}
		//		if (keymode_A != sd.keymode) {
		//			//v4 = 0;
		//		}
		//	}
		//}

		if (slistCount < 0)	slistCount = 0;

		if (sd.favorite == 2) continue;

		//I'm uncertain, need check
		switch (sd.keymode) {
			case 7:
				if (keys == 7) break;
			case 5:
				if (key == 1) break;
			default:
				switch (sd.keymode) {
					case 14:
						if (keys >= 14) break;
					case 10:
						if (key == 4) break;
					default:{
							bool bKeyfilter; //looks like there is no variable, but I had no idea
							if (sd.keymode == 9) bKeyfilter = keys == 9;
							else if (sd.keymode == 10) bKeyfilter = keys == 10;
							else if (sd.keymode == 5) bKeyfilter = keys == 5;
							else bKeyfilter = sd.keymode == 0;
							if (!bKeyfilter && key != 0) continue;
						}
						break;
				}
				break;
		}
		//if (sd.keymode == 7) {
		//	if (keys != 7 && key != 1) {
		//		if (sd.keymode == 14) {
		//			if (keys < 14 && key != 4) {
		//				//rjtlrl
		//			}
		//		}
		//	}
		//}
		//else {
		//	if (sd.keymode < 9) {
		//		if(key != 1) {
		//			if (sd.keymode == 14) {
		//				if (keys < 14 && key != 4) {
		//					//rjtlrl
		//					//breakkk
		//				}
		//			}
		//		}
		//	}
		//	if (sd.keymode == 14) {
		//		if (key != 4) {
		//			//rjtlrl
		//		}
		//	}
		//	else {
		//		//rjtlrl:
		//		//	if (sd.keymode == 9) v5 = keys == 9;
		//		//	else if (sd.keymode == 10) v5 = keys == 10;
		//		//	else if (sd.keymode == 5) v5 = keys == 5;
		//		//	else v5 = sd.keymode == 0;
		//		//	if (!v5 && key != 0) continue;
		//	}
		//}

		sd.keymode = sqlite3_column_int(stmt, 18);
		sd.difficulty = sqlite3_column_int(stmt, 15);
		cstrSprintf(&newFolder, "%s", sqlite3_column_text(stmt, 9));
		if (sd.keymode > 0) {
			sd.mybest.clear = sqlite3_column_int(stmt, 30);
			sd.mybest.stat_pgreat = sqlite3_column_int(stmt, 31);
			sd.mybest.stat_great = sqlite3_column_int(stmt, 32);
			sd.mybest.stat_good = sqlite3_column_int(stmt, 33);
			sd.mybest.stat_bad = sqlite3_column_int(stmt, 34);
			sd.mybest.stat_poor = sqlite3_column_int(stmt, 35);
			sd.mybest.total_notes = sqlite3_column_int(stmt, 36);
			sd.mybest.stat_maxcombo = sqlite3_column_int(stmt, 37);
			sd.mybest.minbp = sqlite3_column_int(stmt, 38);
			sd.mybest.playcount = sqlite3_column_int(stmt, 39);
			sd.mybest.clearcount = sqlite3_column_int(stmt, 40);
			sd.mybest.failcount = sqlite3_column_int(stmt, 41);
			sd.mybest.rank = sqlite3_column_int(stmt, 42);
			sd.mybest.rate = sqlite3_column_int(stmt, 43);
			sd.mybest.clear_db = sqlite3_column_int(stmt, 44);
			sd.mybest.op_history = sqlite3_column_int(stmt, 45);
			sd.mybest.clear_sd = sqlite3_column_int(stmt, 48);
			sd.mybest.clear_ex = sqlite3_column_int(stmt, 49);
			sd.mybest.op_best = sqlite3_column_int(stmt, 50);
			sd.mybest.rseed = sqlite3_column_int(stmt, 51);
			sd.mybest.complete = sqlite3_column_int(stmt, 52);
		}
		if (sqlite3_column_int(stmt, 18) == 0) {
			workingFolder = newFolder;
			COPY_SONGDATA(&slist[slistCount], &sd);
			nowMode = sqlite3_column_int(stmt, 18);
			difficultyCount = 1;

			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		else if (newFolder.isDiff(workingFolder)) {
			workingFolder = newFolder;
			nowMode = sqlite3_column_int(stmt,18);
			nowDifficulty = sd.difficulty;
			difficultyCount = 1;
			COPY_SONGDATA(&slist[slistCount], &sd);
			
			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		else if (sqlite3_column_int(stmt, 18) != nowMode) {
			nowMode = sqlite3_column_int(stmt, 18);
			nowDifficulty = sd.difficulty;
			difficultyCount = 1;
			COPY_SONGDATA(&slist[slistCount], &sd);
			
			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		else if (nowDifficulty < sd.difficulty && sd.difficulty <= difficulty && difficulty != 0) {
			folderSongCount -= difficultyCount;
			slistCount -= difficultyCount;
			if (folderSongCount < 1) folderSongCount = 0;
			nowDifficulty = sd.difficulty;
			difficultyCount = 1;
			COPY_SONGDATA(&slist[slistCount], &sd);
			
			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		else if (nowDifficulty == sd.difficulty || difficulty == 0) {
			difficultyCount++;
			
			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		InitSongData(&sd);
	}
	sqlite3_finalize(stmt);
	if (slistCount) { 
		song->mybest.clear = 5;
		song->mybest.clear_db = 5;
		for (int i = 0; i < slistCount; i++) { 
			song->mybest.playcount += slist[i].mybest.playcount;
			song->mybest.clearcount += slist[i].mybest.clearcount;
			song->mybest.failcount += slist[i].mybest.failcount;
			if (slist[i].mybest.clear != song->mybest.clear && slist[i].mybest.clear <= song->mybest.clear)
				song->mybest.clear = slist[i].mybest.clear;
			if (slist[i].mybest.clear_db != song->mybest.clear_db && slist[i].mybest.clear_db <= song->mybest.clear_db)
				song->mybest.clear_db = slist[i].mybest.clear_db;
		}
	}
	free(slist);
	return 1;
}

//4474a0
int UninstallSong(CSTR path, sqlite3 *sql) {

	SHFILEOPSTRUCTA sh;
	char query[2048];
	
	CSTR dir(path.getDirectory());
	CSTR crc(AssignCRC32(dir));

	sqlite3_snprintf(2048, query, "DELETE FROM song WHERE folder=\'%q\'", crc);
	SQL_Run(query, sql);

	sqlite3_snprintf(2048, query, "DELETE FROM folder WHERE path=\'%q\'", dir);
	SQL_Run(query, sql);

	dir.nullAtPos(dir.length()-1);

	memset(&sh, 0, sizeof(SHFILEOPSTRUCTA));
	sh.wFunc = FO_DELETE; //3
	sh.pFrom = dir;
	sh.pTo = NULL;
	sh.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT; //0x414

	return (SHFileOperationA(&sh) == 0);
}

//4476b0
int Rename(CSTR path, sqlite3 *sql) {

	SHFILEOPSTRUCTA sh;
	char query[2048];

	CSTR newpath(path);
	newpath.add("_");

	sqlite3_snprintf(2048, query, "DELETE FROM song WHERE path=\'%q\'", path);
	SQL_Run(query, sql);
	
	memset(&sh, 0, sizeof(SHFILEOPSTRUCTA));
	sh.wFunc = FO_RENAME; //4
	sh.pFrom = path;
	sh.pTo = newpath;
	sh.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT; //0x414

	return (SHFileOperationA(&sh) == 0);
}

//447820
int GetSongData(CSTR songMD5, SONGDATA *song, sqlite3 *sql, SONGSELECT *ss) {
	CSTR query;
	CSTR replayPath;
	CSTR besthash;
	char buf[1024];
	sqlite3_stmt *stmt;
	int ret = 0;

	InitSongData(song);
	query = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE song.hash = \'%s\'", songMD5.body);
	SQL_prepare(query, sql, &stmt);
	if (sqlite3_step(stmt) == 100) {
		song->maxBPM = sqlite3_column_int(stmt, 16);
		song->minBPM = sqlite3_column_int(stmt, 17);
		song->hash = SQL_GetColumn(0, stmt);
		if (song->hash.isSame("9cbea4427d7e8caff0d28f9d7600cdec"))
			song->title = AutomationFactory();
		else
			song->title = SQL_GetColumn(1, stmt);
		song->subtitle = SQL_GetColumn(2, stmt);
		song->genre = SQL_GetColumn(3, stmt);
		song->filepath = SQL_GetColumn(7, stmt);
		song->folderType = sqlite3_column_int(stmt, 8);
		song->level = sqlite3_column_int(stmt, 14);
		song->artist = SQL_GetColumn(4, stmt);
		song->subartist = SQL_GetColumn(5, stmt);
		song->keymode = sqlite3_column_int(stmt, 18);
		song->judge = sqlite3_column_int(stmt, 19);
		song->longnote = sqlite3_column_int(stmt, 20);
		song->bga = sqlite3_column_int(stmt, 21);
		song->random = sqlite3_column_int(stmt, 22);
		song->folder = SQL_GetColumn(9, stmt);
		song->txt = sqlite3_column_int(stmt, 25);
		song->favorite = sqlite3_column_int(stmt, 24);
		song->adddate = sqlite3_column_int(stmt, 27);
		
		song->stagefile = SQL_GetColumn(10, stmt);
		if (song->stagefile.isDiff("(null)") && song->stagefile.length() > 4)
			song->isStagefile = 1;
		song->banner = SQL_GetColumn(11, stmt);
		if (song->banner.isDiff("(null)") && song->banner.length() > 4)
			song->isBanner = 1;
		song->backBMP = SQL_GetColumn(12, stmt);
		if (song->backBMP.isDiff("(null)") && song->backBMP.length() > 4)
			song->isBackBMP = 1;

		if(song->subtitle.isDiff("(null)"))
			cstrSprintf(&song->fulltitle,"%s %s", song->title, song->subtitle);
		else
			song->fulltitle = song->title;

		song->tag = SQL_GetColumn(6, stmt);
		song->difficulty = sqlite3_column_int(stmt, 15);
		cstrSprintf(&replayPath, "LR2files/Replay/%s/%s.lr2rep", ss->playerID, song->hash);
		song->replayExist = IsFileExist(replayPath);
		if (song->keymode > 0) {
			song->mybest.clear = sqlite3_column_int(stmt, 30);
			song->mybest.stat_pgreat = sqlite3_column_int(stmt, 31);
			song->mybest.stat_great = sqlite3_column_int(stmt, 32);
			song->mybest.stat_good = sqlite3_column_int(stmt, 33);
			song->mybest.stat_bad = sqlite3_column_int(stmt, 34);
			song->mybest.stat_poor = sqlite3_column_int(stmt, 35);
			song->mybest.total_notes = sqlite3_column_int(stmt, 36);
			song->mybest.stat_maxcombo = sqlite3_column_int(stmt, 37);
			song->mybest.minbp = sqlite3_column_int(stmt, 38);
			song->mybest.playcount = sqlite3_column_int(stmt, 39);
			song->mybest.clearcount = sqlite3_column_int(stmt, 40);
			song->mybest.failcount = sqlite3_column_int(stmt, 41);
			song->mybest.rank = sqlite3_column_int(stmt, 42);
			song->mybest.rate = sqlite3_column_int(stmt, 43);
			if (song->mybest.rank > 8) song->mybest.rank = 8;
			if (song->mybest.rank == 0 && (song->mybest.stat_pgreat + song->mybest.stat_great > 0)) song->mybest.rank = 1;
			song->mybest.clear_db = sqlite3_column_int(stmt, 44);
			song->mybest.op_history = sqlite3_column_int(stmt, 45);
			song->mybest.clear_sd = sqlite3_column_int(stmt, 48);
			song->mybest.clear_ex = sqlite3_column_int(stmt, 49);
			song->mybest.op_best = sqlite3_column_int(stmt, 50);
			song->mybest.rseed = sqlite3_column_int(stmt, 51);
			song->mybest.complete = sqlite3_column_int(stmt, 52);

			besthash = SQL_GetColumn(46, stmt);
			if (isSameScoreHash(&song->mybest, &ss->playerPassMD5, &song->hash, &besthash)) {
				song->mybest.stat_exscore = song->mybest.stat_great + song->mybest.stat_pgreat * 2;
				if (song->mybest.total_notes < 1) 
					song->mybest.total_notes = sqlite3_column_int(stmt, 26);

				if (song->mybest.total_notes > 0)
					song->mybest.stat_score = (song->mybest.stat_good + song->mybest.stat_great * 2 + song->mybest.stat_pgreat * 4) * 50000 / song->mybest.total_notes;

				if (song->mybest.minbp == 0 && song->mybest.clear != 5)
					song->mybest.minbp = -1; 
			}
			else {
				memset(&song->mybest, 0, sizeof(STATUS));
				song->mybest.minbp = -1;
				song->mybest.total_notes = sqlite3_column_int(stmt, 26);
			}
		}
		ret = 1;
	}
	else {
		ErrorLogFmtAdd("曲データの取得に失敗 %s\n", songMD5.body);
	}
	sqlite3_finalize(stmt);
	return ret;
}

//448110
int WriteCourse(sqlite3 *sql, COURSESELECT course, SONGDATA *song, CSTR passmd5, int connection, int gauge) {

	char t[1024];

	if (song->courseReadOnly) return 1;

	CSTR hash;
	CSTR query;

	if (course.id < 0) {
		if (course.type == 1) 
			cstrSprintf(&hash, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d00000000000000%s", connection, connection, connection, connection, connection, 0, 0, 0, 0, 0, 1, 0, 0, 0, passmd5.left(4));
		else 
			cstrSprintf(&hash, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d00000000000000%s", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, course.type, 0, 0, 0, passmd5.left(4));

		for (int i = 0; i < course.count; i++) hash.add(course.data[i].hash);

		if (course.type == 0) 
			query = sqlite3_snprintf(1024, t, "INSERT INTO expert (title,hash,line,ir) VALUES (\'%q\' , \'%q\', %d,0)", course.name, hash, course.data[0].keymode);
		else if (course.type == 1) 
			query = sqlite3_snprintf(1024, t, "INSERT INTO nonstop (title,hash,line,ir) VALUES (\'%q\' , \'%q\', %d,0)", course.name, hash, course.data[0].keymode);
		else if (course.type == 2) 
			query = sqlite3_snprintf(1024, t, "INSERT INTO grade (title,hash,line,ir) VALUES (\'%q\' , \'%q\', %d,0)", course.name, hash, course.data[0].keymode);
	}
	else {
		cstrSprintf(&hash, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d00000000000000%s", song->courseKeys[0], song->courseKeys[1], song->courseKeys[2], song->courseKeys[3], song->courseKeys[4], song->courseKeys[5],
			song->courseKeys[6], song->courseKeys[7], song->courseKeys[8], song->courseKeys[9], song->courseType, 0, 0, 0, passmd5.left(4));

		for (int i = 0; i < song->courseStageCount; i++) hash.add(song->courseHash[i]);

		if (course.type == 0) 
			query = sqlite3_snprintf(1024, t, "UPDATE expert SET title=\'%q\',hash=\'%q\',ir=%d WHERE id = %d", song->title, hash, song->courseIR, course.id);
		else if (course.type == 1) 
			query = sqlite3_snprintf(1024, t, "UPDATE nonstop SET title=\'%q\',hash=\'%q\',ir=%d WHERE id = %d", song->title, hash, song->courseIR, course.id);
		else if (course.type == 2) 
			query = sqlite3_snprintf(1024, t, "UPDATE grade SET title=\'%q\',hash=\'%q\',ir=%d WHERE id = %d", song->title, hash, song->courseIR, course.id);
	}

	SQL_Run(query, sql);
	for (int i = 0; i < 10; i++) InitSongData(&course.data[i]);

	return 1;
}

//448560
int ChangeCourseTitle(sqlite3* sql, CSTR title, int id, int coursetype) {

	char str[1024];
	CSTR query;

	if (coursetype == 0) {
		sqlite3_snprintf(0x400, str, "UPDATE expert SET title=\'%q\' WHERE id = %d",title, id);
	}
	else if (coursetype == 1) {
		sqlite3_snprintf(0x400, str, "UPDATE nonstop SET title=\'%q\' WHERE id = %d", title, id);
	}
	else if (coursetype == 2) {
		sqlite3_snprintf(0x400, str, "UPDATE grade SET title=\'%q\' WHERE id = %d", title, id);
	}

	query = str;
	SQL_Run(query, sql);
	return 1;
}

//4486a0
int DeleteCourse(sqlite3* sql, int id, int courseType) {

	char str[1024];
	CSTR query;

	if (courseType == 0) {
		sqlite3_snprintf(0x400, str,"DELETE FROM expert WHERE id = %d" , id);
	}
	else if (courseType == 1) {
		sqlite3_snprintf(0x400, str, "DELETE FROM nonstop WHERE id = %d", id);
	}
	else if (courseType == 2){
		sqlite3_snprintf(0x400, str, "DELETE FROM grade WHERE id = %d", id);
	}

	query = str;
	SQL_Run(query, sql);
	return 1;
}

//4487c0
CSTR GetRandomSongHash(CSTR ioDefault, CSTR folder, int levelStd, int mode, sqlite3 *sql, int levelMax, int levelMin) {

	sqlite3_stmt *pStmt;

	if (levelStd < 1) return ioDefault;

	CSTR query;

	cstrSprintf(&query, "SELECT hash FROM song WHERE folder=\'%s\' AND mode=%d AND level<=%d AND level>=%d ORDER BY abs(level - %d)", folder.body, mode, levelMax, levelMin, levelStd);
	SQL_prepare(query, sql, &pStmt);
	if (sqlite3_step(pStmt) == 100) {
		ioDefault = SQL_GetColumn(0, pStmt);
	}
	sqlite3_finalize(pStmt);
	return ioDefault;
}

//448950
int GetSongDataFromPath(CSTR filepath, SONGDATA *song, sqlite3 *sql, SONGSELECT *ss) {
	BMSMETA meta;
	CSTR hash;

	InitSongData(song);
	if (!filepath.canOpenFile()) {
		ErrorLogFmtAdd("GetSongDataFromPath() bmsファイルが見つかりません\n");
		return -1;
	}

	makeFileHash(filepath, hash);
	if (GetSongData(hash,song,sql,ss) == 1) {
		ErrorLogFmtAdd("GetSongDataFromPath() データベースに登録済みなのでそちらを利用します\n");
		return 1;
	}
	
	ParseBMSMETA(&meta, filepath, 0);
	song->artist = meta.artist;
	song->backBMP = meta.backBMPpath;
	song->stagefile = meta.stagefilepath;
	song->banner = meta.bannerpath;
	song->bga = meta.bga;
	song->difficulty = meta.difficulty;
	song->exlevel = 0;
	cstrSprintf(&song->fulltitle, "%s %s", meta.title.body, meta.subartist.body);
	song->folder = meta.folderpath;
	song->genre = meta.genre;
	song->hash = meta.hash;
	song->longnote = meta.longnote;
	song->level = meta.selLevel;
	song->keymode = meta.keymode;
	song->maxBPM = meta.maxbpm;
	song->minBPM = meta.minbpm;
	song->filepath.assign(&filepath);
	song->random = meta.random;
	song->judge = meta.judge;
	song->subartist = meta.subartist;
	song->subtitle = meta.subtitle;
	song->folderType = 0;
	song->title = meta.title;
	song->folderType = 0;
	ErrorLogFmtAdd("GetSongDataFromPath() 曲をロードしました\n");
	return 2;
}

//448c10 
int ChangeCourseID(sqlite3 *sql, int newID, int oldID, int type) {

	char str[1024];

	if (newID <= 0 || oldID <= 0) return 0;

	if (type == 0) {
		sqlite3_snprintf(0x400, str, "UPDATE expert SET id = -1 WHERE id = %d", newID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE expert SET id = %d WHERE id = %d", newID, oldID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE expert SET id = %d WHERE id = -1", oldID);
		SQL_Run(str, sql);
	}
	else if (type == 1) {
		sqlite3_snprintf(0x400, str, "UPDATE nonstop SET id = -1 WHERE id = %d", newID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE nonstop SET id = %d WHERE id = %d", newID, oldID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE nonstop SET id = %d WHERE id = -1", oldID);
		SQL_Run(str, sql);
	}
	else if (type == 2) {
		sqlite3_snprintf(0x400, str, "UPDATE grade SET id = -1 WHERE id = %d", newID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE grade SET id = %d WHERE id = %d", newID, oldID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE grade SET id = %d WHERE id = -1", oldID);
		SQL_Run(str, sql);
	}
	return 1;
}


//448ea0
int SearchSongsFromPath(CSTR root, sqlite3 *sql, CSTR path) {
	
	HANDLE hFindFile;
	_WIN32_FIND_DATAA findFileData;
	int now,filetime;
	char str[2048];
	int count = 0;
	

	ErrorLogFmtAdd("曲の検索を行います。パス%s\n", root);
	ErrorLogTabAdd();
	if (root.right(1).isDiff("\\")) root.add("\\");

	CSTR searchPath;
	BMSMETA meta; 
	searchPath = root;
	searchPath.add("*.*");
	hFindFile = FindFirstFileA(searchPath, &findFileData);
	now = GetNowUnixtime();
	while (1) {
		if ( ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) || !(strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName)) ) { // not directory
			if (((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) && IsBmsFile(findFileData.cFileName)) {
				searchPath = root;
				searchPath.add(findFileData.cFileName);
				filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
				ErrorLogFmtAdd("曲を発見しました。　パス:%s\n", searchPath);
				ParseBMSMETA(&meta, searchPath, 1);
				LoadBMSMETAFromDB(&meta, sql);
				sqlite3_snprintf(2048, str, "INSERT INTO song (hash,title,subtitle,genre,artist,subartist,level,date,path,folder,stagefile,banner,backbmp,parent,maxbpm,minbpm,random,longnote,judge,mode,bga,difficulty,favorite,type,txt,karinotes,adddate,exlevel) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,%d)",
					meta.hash, meta.title, meta.subtitle, meta.genre, meta.artist, meta.subartist, meta.selLevel, filetime, searchPath, AssignCRC32(meta.folderpath), meta.stagefilepath, meta.bannerpath, meta.backBMPpath, AssignCRC32(meta.parentfolderpath), meta.maxbpm, meta.minbpm, meta.random, meta.longnote, meta.judge, meta.keymode, meta.bga, meta.difficulty, meta.hasTxt, meta.notecount, now, meta.exlevel);
				SQL_Run(str, sql);
				count++;
			}
			else if (((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) && IsLR2Folder(findFileData.cFileName)){
				searchPath = root;
				searchPath.add(findFileData.cFileName);
				ErrorLogFmtAdd("カスタムフォルダを発見しました。　パス:%s\n", searchPath);
				filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
				ParseBMSMETA(&meta, searchPath, 1);
				sqlite3_snprintf(2048, str, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , 2 , \'%q\' , %d) ",
					searchPath, meta.title, AssignCRC32(path), meta.genre, meta.artist, meta.subartist, meta.tag, meta.selLevel, filetime, meta.bannerpath, now);
				if (SQL_Run(str, sql) == 0) {
					ErrorLogFmtAdd("追加失敗 エラー内容 %s\n", sqlite3_errmsg(sql));
				}
				count++;
			}

		}
		else {
			searchPath = root;
			searchPath.add(findFileData.cFileName).add("\\");
			ErrorLogFmtAdd("フォルダを発見しました。　パス:%s\n", searchPath);
			
			CSTR folderinfo(searchPath);
			folderinfo.add("folderinfo.txt");
			filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
			if (IsFileExist(folderinfo)) {
				ErrorLogAdd("ついでにフォルダインフォを発見しました\n");
				ParseBMSMETA(&meta, folderinfo, 0);
				if (meta.judge != 2) meta.judge = 1;

				sqlite3_snprintf(2048, str, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , \'%q\' , %d) ",
					searchPath, meta.title, AssignCRC32(path), meta.genre, meta.artist, meta.subartist, meta.tag, meta.selLevel, filetime, meta.judge, meta.bannerpath, now);
				if (SQL_Run(str, sql) == 0) {
					ErrorLogAdd("再帰検索を行います。\n");
					CSTR subPath(root);
					subPath.add(findFileData.cFileName).add("\\");
					count += SearchSongsFromPath(searchPath, sql, subPath);
				}
			}
			else {
				sqlite3_snprintf(2048, str, "INSERT INTO folder (path , title , parent , date , type  , adddate) VALUES(\'%q\',\'%q\',\'%q\',%d , 1 , %d )", searchPath, findFileData.cFileName, AssignCRC32(path), filetime, now);
				if (SQL_Run(str, sql) == 0) {
					ErrorLogAdd("再帰検索を行います。\n");
					CSTR subPath(root);
					subPath.add(findFileData.cFileName).add("\\");
					count += SearchSongsFromPath(searchPath, sql, subPath);
				}
			}
		}

		if (FindNextFileA(hFindFile, &findFileData) == 0) {
			FindClose(hFindFile);
			ErrorLogTabSub();
			return count;
		}
	}
}

//449780 TODO:arrange duplicated code
int ReloadSongsByQuery(CSTR query, sqlite3 *sql, CONFIG_JUKEBOX *jb) {

	sqlite3_stmt *pStmt;
	int newTime, time;
	char sBuf[1024];
	int chg = 0;
	int cAlready = 0, cNot = 0, cChange = 0;

	GetTimeWrap();
	SQL_prepare(query, sql, &pStmt);
	int now = GetNowUnixtime();

	while (sqlite3_step(pStmt) == 100) {
		time = sqlite3_column_int(pStmt, 1);
		CSTR str = SQL_GetColumn(0, pStmt);
		if (!str.left(8).isSame("LR2files")) {
			int flgFile = 0, flgFolder = 0;
			
			if (IsBmsFile(str)) 
				flgFile = 1;
			else if(IsLR2Folder(str))
				flgFolder = 1;

			int isJbPath = 0;
			for (int i = 0; i < jb->numOfPath; i++) {
				if (str.left(jb->path[i].length()).isSame(&jb->path[i])) {
					isJbPath = 1;
				}
			}
			if (jb->numOfPath == 0 || isJbPath){
				chg = IsFileChanged(time, str, &newTime);
				if (chg == 0) {
					cAlready++;
				}
				else if (chg == 1) {
					cNot++;
					if (flgFile) {
						SQL_Run(sqlite3_snprintf(512, sBuf, "DELETE FROM song WHERE path=\'%q\'", str), sql); 
					}
					else {
						SQL_Run(sqlite3_snprintf(512, sBuf, "DELETE FROM folder WHERE path=\'%q\'", str), sql);
					}
				}
				else if (chg == 2) {
					cChange++;
					if (flgFile) {
						SQL_Run(sqlite3_snprintf(1024, sBuf, "DELETE FROM song WHERE path=\'%q\'", str), sql);
						BMSMETA meta;
						ParseBMSMETA(&meta, str, 1);
						LoadBMSMETAFromDB(&meta, sql);
						SQL_Run(sqlite3_snprintf(1024, sBuf, "INSERT INTO song (hash,title,subtitle,genre,artist,subartist,level,date,path,folder,stagefile,banner,backbmp,parent,maxbpm,minbpm,random,longnote,judge,mode,bga,difficulty,favorite,type,txt,karinotes,adddate,exlevel) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,%d)",
							meta.hash, meta.title, meta.subtitle, meta.genre, meta.artist, meta.subartist, meta.selLevel, newTime, str, AssignCRC32(meta.folderpath), meta.stagefilepath, meta.bannerpath, meta.backBMPpath,AssignCRC32(meta.parentfolderpath),meta.maxbpm,meta.minbpm,meta.random,meta.longnote,meta.judge,meta.keymode,meta.bga,meta.difficulty,meta.hasTxt,meta.notecount,now,meta.exlevel), sql);
					}
					else if (flgFolder) {
						SQL_Run(sqlite3_snprintf(1024, sBuf, "DELETE FROM folder WHERE path=\'%q\'", str), sql);
						BMSMETA meta;
						ParseBMSMETA(&meta, str, 1);
						SQL_Run(sqlite3_snprintf(1024, sBuf, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , 2 , %s , %d) ",
							str, meta.title, AssignCRC32(meta.folderpath), meta.genre, meta.artist, meta.subartist, meta.tag, meta.selLevel, newTime, meta.bannerpath, now), sql);
					}
					else {
						CSTR folderinfo(str);
						folderinfo.add("folderinfo.txt");
						if (IsFileExist(folderinfo)) {
							BMSMETA meta;
							ParseBMSMETA(&meta, folderinfo, 0);
							if (meta.judge != 2) meta.judge = 1;
							if (SQL_Run(sqlite3_snprintf(512, sBuf, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type  , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , \'%q\' , %d) ",
								str, meta.title, AssignCRC32(meta.folderpath), meta.genre, meta.artist, meta.subartist, meta.tag, meta.selLevel, newTime, meta.judge, meta.bannerpath, now), sql) == 0) {
								SearchSongsFromPath(str, sql, str);
							}
						}
						else {
							if (SQL_Run(sqlite3_snprintf(512, sBuf, "UPDATE folder SET date = %d , adddate = %d WHERE path = \'%q\'", newTime, now, str), sql) == 0) {
								SearchSongsFromPath(str, sql, str);
							}
						}
					}
				}
				
			}
			else {
				if (flgFile) {
					SQL_Run(sqlite3_snprintf(512, sBuf, "DELETE FROM song WHERE path=\'%q\'", str), sql);
				}
				else {
					SQL_Run(sqlite3_snprintf(512, sBuf, "DELETE FROM folder WHERE path=\'%q\'", str), sql);
				}
			}
		}
	}
	sqlite3_finalize(pStmt);
	GetTimeWrap();
	if (cAlready == 0 && cNot == 0) {
		if(cChange == 0){
			ErrorLogFmtAdd("曲が見つかりません\n");
			return 0;
		}
	}
	else if (cChange == 0) {
		ErrorLogFmtAdd("更新はありません\n");
		return 1;
	}
	ErrorLogFmtAdd("更新しました。err %d update %d\n", cNot, cChange);
	return 2;
}

//44a140
int CMP_SongDataByDifficulty(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	if (s1->keymode != s2->keymode) {
		return s1->keymode - s2->keymode;
	}

	if ((s1->difficulty == 5) == (s2->difficulty == 5)) {
		if (s1->level != s2->level) return s1->level - s2->level;
		return CMP_SongDataByTitle(p1, p2);
	}
	if (s1->difficulty == 5 && s2->difficulty != 5) return 1;
	if (s1->difficulty != 5 && s2->difficulty == 5) return -1;

	return 0;
}

//44a1a0
int CMP_SongDataByClear(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	if (s1->mybest.clear != s2->mybest.clear) return s1->mybest.clear - s2->mybest.clear;

	return CMP_SongDataByDifficulty(p1, p2);
}

//44a1e0
int SearchCourseFromDB(sqlite3 *sql, SONGSELECT *ss, int keys, int multistagemode) {

	sqlite3_stmt *pStmt;
	CSTR str;

	if (ss->filter.ignorekeyall == 1 && keys == 0) keys = 1;
	if (ss->filter.ignorekeysingle == 1 && keys == 1) keys = 2;
	if (ss->filter.ignorekeydouble == 1 && keys == 4) keys = 5;
	if (ss->filter.ignoredp == 1 && keys < 7) keys = 7;
	if (ss->filter.ignorepms == 1 && keys == 7) keys = 0;
	if (ss->filter.ignorekeyall == 1 && keys == 0) keys = 1;

	GetNowUnixtime();
	ss->rivalID = 0;

	CSTR query;
	if (multistagemode == 0) {
		query = "SELECT * FROM expert LEFT JOIN score ON expert.hash = score.hash";
		ss->course.type = 0;
	}
	else if (multistagemode == 1) {
		query = "SELECT * FROM nonstop LEFT JOIN score ON nonstop.hash = score.hash";
		ss->course.type = 1;
	}
	else if (multistagemode == 2) {
		query = "SELECT * FROM grade LEFT JOIN score ON grade.hash = score.hash";
		ss->course.type = 2;
	}

	switch (keys) {
		case 0:
			query.add(" ");
			break;
		case 1:
			query.add(" WHERE line=7 OR line=5");
			break;
		case 2:
			query.add(" WHERE line=7");
			break;
		case 3:
			query.add(" WHERE line=5");
			break;
		case 4:
			query.add(" WHERE line=14 OR line=10");
			break;
		case 5:
			query.add(" WHERE line=14");
			break;
		case 6:
			query.add(" WHERE line=10");
			break;
		case 7:
			query.add(" WHERE line=9");
			break;
	}
	query.add(" ORDER BY id DESC");

	SQL_prepare(query, sql, &pStmt);
	ss->prevListCount = 0;
	SONGDATA song;
	while (sqlite3_step(pStmt) == 100) {

		if (ss->prevListSize - 1 == ss->prevListCount) {
			realloc(ss->prevList, (ss->prevListSize + 1000) * sizeof(SONGDATA));
			for (int i = ss->prevListSize; i < ss->prevListSize + 1000; i++) {
				memset(&ss->prevList[i], 0, sizeof(SONGDATA));
			}
			ss->prevListSize += 1000;
		}
		InitSongData(&song);
		song.folderType = 8;
		cstrSprintf(&song.filepath, "%d", ss->prevListCount);
		song.courseID = sqlite3_column_int(pStmt, 0);
		song.title = SQL_GetColumn(1, pStmt);
		song.fulltitle = song.title;
		song.keymode = sqlite3_column_int(pStmt, 2);
		song.hash = SQL_GetColumn(3, pStmt);
		song.courseStageCount = song.hash.length()/32 - 1;
		song.courseIR = sqlite3_column_int(pStmt, 4);
		if (song.courseIR == 1) song.courseReadOnly = 1;
		if (song.courseStageCount > 9) song.courseStageCount = 10;
		if (song.hash.length() > 63) {
			for (int i = 0; i < 10; i++) {
				song.courseKeys[i] = *song.hash.atPos(i) - 48;
			}
			song.courseType = *song.hash.atPos(10) - 48;
			song.coursePlayable = 1;
		}

		for (int i = 1; i <= song.courseStageCount; i++) {
			SONGDATA newsong;
			if (GetSongData(song.hash.getSliced(i * 32, 32),&newsong, sql, ss) == 1) {
				song.courseHash[i - 1] = newsong.hash;
				song.courseTitle[i - 1] = newsong.title;
				song.courseSubtitle[i - 1] = newsong.subtitle;
				song.courseLevel[i - 1] = newsong.level;
				song.courseTotalnote[i - 1] = newsong.mybest.total_notes;
				song.courseStageDifficulty[i - 1] = newsong.difficulty;
			}
			else {
				song.courseHash[i - 1].fillzero();
				song.courseTitle[i - 1] = "この曲を所持していません";
				song.courseLevel[i - 1] = -1;
				song.coursePlayable = 0;
			}

		}
		if (song.courseStageCount == 0) {
			song.artist = "曲が設定されていないのでプレイできません";
			song.coursePlayable = 0;
		}
		else if (song.coursePlayable == 0) {
			song.artist = "曲が設定されていないのでプレイできません";
		}

		if (multistagemode == 1) {
			cstrSprintf(&str, "LR2files/Replay/%s/%s.lr2rep", ss->playerID, MD5str(song.hash));
		}
		else {

			cstrSprintf(&str, "LR2files/Replay/%s/c/%s", ss->playerID, MD5str(song.hash));
		}
		song.replayExist = IsFileExist(str);
		song.mybest.clear = sqlite3_column_int(pStmt, 6);
		song.mybest.stat_pgreat = sqlite3_column_int(pStmt, 7);
		song.mybest.stat_great = sqlite3_column_int(pStmt, 8);
		song.mybest.stat_good = sqlite3_column_int(pStmt, 9);
		song.mybest.stat_bad = sqlite3_column_int(pStmt, 10);
		song.mybest.stat_poor = sqlite3_column_int(pStmt, 11);
		song.mybest.total_notes = sqlite3_column_int(pStmt, 12);
		song.mybest.stat_maxcombo = sqlite3_column_int(pStmt, 13);;
		song.mybest.minbp = sqlite3_column_int(pStmt, 14);
		song.mybest.playcount = sqlite3_column_int(pStmt, 15);
		song.mybest.clearcount = sqlite3_column_int(pStmt, 16);
		song.mybest.failcount = sqlite3_column_int(pStmt, 17);
		song.mybest.rank = sqlite3_column_int(pStmt, 18);
		song.mybest.rate = sqlite3_column_int(pStmt, 19);
		if (song.mybest.rank < 9) {
			if (song.mybest.rank == 0 && song.mybest.stat_pgreat + song.mybest.stat_great > 0)
				song.mybest.rank = 1;
		}
		song.mybest.rank = 1;
		song.mybest.clear_db = sqlite3_column_int(pStmt, 20);
		song.mybest.op_history = sqlite3_column_int(pStmt, 21);
		song.mybest.clear_sd = sqlite3_column_int(pStmt, 24);
		song.mybest.clear_ex = sqlite3_column_int(pStmt, 25);
		song.mybest.op_best = sqlite3_column_int(pStmt, 26);
		song.mybest.rseed = sqlite3_column_int(pStmt, 27);
		song.mybest.complete = sqlite3_column_int(pStmt, 28);
		str = SQL_GetColumn(22, pStmt);
		if (isSameScoreHash(&song.mybest, &ss->playerPassMD5, &song.hash, &str)) {
			song.mybest.stat_exscore = song.mybest.stat_great + song.mybest.stat_pgreat * 2;
			if (song.mybest.stat_score > 0) {
				song.mybest.stat_score = (song.mybest.stat_good + song.mybest.stat_exscore) * 2 * 50000 / song.mybest.total_notes;
			}
			if (song.mybest.minbp == 0 && song.mybest.clear != 5) song.mybest.minbp = -1;
		}
		else {
			memset(&song.mybest, 0, sizeof(STATUS));
			song.mybest.total_notes = 0;
			song.mybest.minbp = -1;
		}

		COPY_SONGDATA(&ss->prevList[ss->prevListCount], &song);
		ss->prevListCount++;
	}
	InitSongData(&song);
	song.title = "NEW COURSE";
	song.fulltitle = "NEW COURSE";
	song.artist = "新規コースを作成します";
	song.hash = "newcourse";
	song.filepath = "newcourse";
	song.folderType = 7;
	song.level = 0;
	COPY_SONGDATA(&ss->prevList[ss->prevListCount], &song);
	ss->prevListCount++;
	if (ss->course.type != 2) {
		song.title = "RANDOM MIX";
		song.fulltitle = "RANDOM MIX";
		song.artist = "ランダムでコースを作成します";
		song.artist = ""; //original code, not a mistake
		song.hash = "randomcourse";
		song.filepath = "randomcourse";
		song.folderType = 9;
		song.level = 0;
		COPY_SONGDATA(&ss->prevList[ss->prevListCount], &song);
		ss->prevListCount++;
	}
	return ss->prevListCount;
}

//44ac10
int LoadBmsListFromDB(CSTR query, sqlite3 *sql, SONGSELECT *ss, int *difficulty, int *key, int sort, int count) {
	
	sqlite3_stmt *stmt;
	SONGDATA song;
	char buf[1024];


	if (query.left(6).isDiff("SELECT")) return -1;

	if (ss->filter.ignorekeyall == 1 && *key == 0) *key = 1;
	if (ss->filter.ignorekeysingle == 1 && *key == 1) *key = 2;
	if (ss->filter.ignorekeydouble == 1 && *key == 4) *key = 5;
	if (ss->filter.ignoredp == 1 && 3 < *key && *key < 7) *key = 7;
	if (ss->filter.ignorepms == 1 && *key == 7) *key = 0;
	if (ss->filter.ignorekeyall == 1 && *key == 0) *key = 1;
	if (ss->filter.ignoredifficultyall == 1 && *difficulty == 0) *difficulty = 1;

	SQL_prepare(query, sql, &stmt);
	ErrorLogFmtAdd("結果…%s\n", sqlite3_errmsg(sql));
	int i = 0;
	while (sqlite3_step(stmt) == 100) {
		if (count != 0 && i == count) break;
		if (ss->prevListSize - 1 == i) {
			ss->prevList = (SONGDATA*)realloc(ss->prevList, (ss->prevListSize + 1000) * sizeof(SONGDATA));
			for (int j = ss->prevListSize; j < ss->prevListSize + 1000; j++) {
				memset(&ss->prevList[j], 0, sizeof(SONGDATA));
			}
			ss->prevListSize += 1000;
		}
		InitSongData(&song);
		song.title = SQL_GetColumn(0, stmt);
		song.subtitle = SQL_GetColumn(1, stmt);
		song.genre = SQL_GetColumn(2, stmt);
		song.artist = SQL_GetColumn(3, stmt);
		song.subartist = SQL_GetColumn(4, stmt);
		song.tag = SQL_GetColumn(5, stmt);
		song.filepath = SQL_GetColumn(6, stmt);
		song.folderType = sqlite3_column_int(stmt, 7);
		song.folder = SQL_GetColumn(9, stmt);
		song.level = sqlite3_column_int(stmt, 11);
		song.adddate = sqlite3_column_int(stmt, 12);
		song.keymode = 0;
		song.fulltitle = song.title;
		COPY_SONGDATA(&ss->prevList[i], &song);
		i++;
	}

	if (i != 0) {
		sqlite3_finalize(stmt);
		ss->prevListCount = i;
		ss->is_loading_bmslist = 1;
		qsort(ss->prevList,ss->prevListCount,sizeof(SONGDATA),CMP_SongDataByPath);
		if (ss->flag_folderlamp) {
			for (int i = 0; i < ss->prevListCount; i++) {
				CSTR query(1024);
				if (ss->prevList[i].tag.isDiff("(null)") && ss->prevList[i].tag.length() >= 5) {
					query = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE %s", ss->prevList[i].tag);
					if (query.findStrPos("__NEWSONG__") > -1) {
						query = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE adddate > %d", GetNowUnixtime() - ss->titleflash * 60); //TOFIX : 3600
					}
					LoadFolderDataFromDB(query, &ss->prevList[i], sql, *difficulty, *key, sort, ss->prevList[i].level, &ss->filter, 1);
				}
				else {
					query = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE parent = \'%s\'", AssignCRC32(ss->prevList[i].filepath));
					LoadFolderDataFromDB(query, &ss->prevList[i], sql, *difficulty, *key, sort, 0, &ss->filter, 0);
				}
			}
		}
		ErrorLogFmtAdd("成功　フォルダ数は%dです。\n", ss->prevListCount);
		return ss->prevListCount;
	}

	ErrorLogAdd("フォルダが見つかりません。\n");
	return -1;
}

//44b380
int ReadPlayerScore(CSTR id, CSTR pass, PLAYERSTATISTIC *pstat) {

	CSTR dbPath, passMD5, query, MD5inDB;
	char str[256];
	sqlite3 *scoreDB;
	sqlite3_stmt *stmt;

	cstrSprintf(&dbPath, "LR2files/Database/Score/%s.db", id);
	passMD5 = MD5str(pass);
	
	sqlite3_open(dbPath, &scoreDB);
	ErrorLogFmtAdd("成功\n");

	sqlite3_snprintf(256, str, "SELECT * FROM player WHERE id = \'%s\'", id);
	query = str;

	SQL_prepare(query, scoreDB, &stmt);
	
	if (sqlite3_step(stmt) != 100) {
		MessageBoxA(NULL, "スコアデータベースが壊れています。config.exeで再作成して下さい。", "エラー", 0);
		sqlite3_finalize(stmt);
		sqlite3_close(scoreDB);
		return 0;
	}

	MD5inDB = SQL_GetColumn(1, stmt);
	if (passMD5.isSame(&MD5inDB) == 0) {
		MessageBoxA(NULL, "パスワードが違います。", "エラー", 0);
		sqlite3_finalize(stmt);
		sqlite3_close(scoreDB);
		return 0;
	}

	pstat->passMD5 = MD5inDB;
	pstat->irid = sqlite3_column_int(stmt, 3);
	pstat->irname = SQL_GetColumn(4, stmt);
	pstat->playcount = sqlite3_column_int(stmt, 5);
	pstat->clear = sqlite3_column_int(stmt, 6);
	pstat->fail = sqlite3_column_int(stmt, 7);
	pstat->perfect = sqlite3_column_int(stmt, 8);
	pstat->great = sqlite3_column_int(stmt, 9);
	pstat->good = sqlite3_column_int(stmt, 10);
	pstat->bad = sqlite3_column_int(stmt, 11);
	pstat->poor = sqlite3_column_int(stmt, 12);
	pstat->playtime = sqlite3_column_int(stmt, 13);
	pstat->combo = sqlite3_column_int(stmt, 14);
	pstat->maxcombo = sqlite3_column_int(stmt, 15);
	pstat->grade7 = sqlite3_column_int(stmt, 16);
	pstat->grade5 = sqlite3_column_int(stmt, 17);
	pstat->grade14 = sqlite3_column_int(stmt, 18);
	pstat->grade10 = sqlite3_column_int(stmt, 19);
	pstat->grade9 = sqlite3_column_int(stmt, 20);
	pstat->trial = sqlite3_column_int(stmt, 21);
	pstat->option = sqlite3_column_int(stmt, 22);
	pstat->systemversion = sqlite3_column_int(stmt, 23);
	pstat->gradeversion = sqlite3_column_int(stmt, 24);
	pstat->trialversion = sqlite3_column_int(stmt, 25);
	MD5inDB = SQL_GetColumn(26, stmt);//scorehash
	
	if (pstat->systemversion != 1) {
		MessageBoxA(NULL, "旧バージョンに上書きは出来ません。", "エラー", 0);
		sqlite3_finalize(stmt);
		sqlite3_close(scoreDB);
		return 0;
	}

	if (MakePlayerStatHash(pstat).isDiff(&MD5inDB)) {
		pstat->playcount = 0;
		pstat->clear = 0;
		pstat->fail = 0;
		pstat->perfect = 0;
		pstat->great = 0;
		pstat->good = 0;
		pstat->bad = 0;
		pstat->poor = 0;
		pstat->playtime = 0;
		pstat->combo = 0;
		pstat->maxcombo = 0;
		pstat->grade7 = 0;
		pstat->grade5 = 0;
		pstat->grade14 = 0;
		pstat->grade10 = 0;
		pstat->grade9 = 0;
		pstat->trial = 1;
		pstat->option = 0;
		pstat->systemversion = 1;
		pstat->gradeversion = 0;
		pstat->trialversion = 0;
		ErrorLogAdd("ハッシュが不正です。プレイヤースコアデータを初期化しました。\n");
	}
	else {
		if (pstat->gradeversion != 0) {
			pstat->grade7 = 0;
			pstat->grade5 = 0;
			pstat->grade14 = 0;
			pstat->grade10 = 0;
			pstat->grade9 = 0;
			pstat->gradeversion = 0;
			ErrorLogAdd("段位認定が更新されたのでデータを初期化しました。\n");
			if (pstat->gradeversion != 0) {
				pstat->trial = 1;
				pstat->trialversion = 0;
				ErrorLogAdd("トライアルが更新されたのでデータを初期化しました。\n");
			}
		}
		if (pstat->trial < 1) {
			pstat->trial = 1;
		}
		pstat->systemversion = 1;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(scoreDB);
	return 1;
}

//44b910
int WriteRandomCourse(sqlite3 *sql, COURSESELECT *course, SONGSELECT *ss, CONFIG_COURSE cfg, int key) {
	
	sqlite3_stmt *pStmt;

	ErrorLogAdd("ランダムコースを作成します\n");
	CSTR query;

	if (ss->course.type == 0) {
		SQL_Run("DELETE FROM expert WHERE title = \'RANDOM MIX\'", sql);
	}
	else if (ss->course.type == 2) {
		SQL_Run("DELETE FROM grade WHERE title = \'RANDOM MIX\'", sql);
	}
	else if (ss->course.type == 1) {
		SQL_Run("DELETE FROM nonstop WHERE title = \'RANDOM MIX\'", sql);
	}

	course->name = "RANDOM MIX";
	course->count = 0;
	course->id = -1;

	int stages = cfg.stage;
	if (cfg.stage <= 0 || cfg.stage > 5) {
		stages = GetRand(3)+2;
	}

	int maxlevel = 999999;
	int minlevel = -999999;
	if (cfg.maxlevel > 0) maxlevel = cfg.maxlevel;
	if (cfg.minlevel > 0) minlevel = cfg.minlevel;

	int maxbpm = 999999;
	int minbpm = -999999;
	if (cfg.maxbpm > 0) maxbpm = cfg.maxbpm;
	if (cfg.minbpm > 0) minbpm = cfg.minbpm;

	if (cfg.bpmrange <= 0) {
		cstrSprintf(&query, "SELECT hash,folder FROM song WHERE mode=%d AND %d<=minbpm AND maxbpm<=%d AND %d<=level AND level<=%d ORDER BY random()", key, minbpm, maxbpm, minlevel, maxlevel);
	}
	else {
		cstrSprintf(&query, "SELECT hash,folder FROM song WHERE mode=%d AND maxbpm=minbpm AND %d<=maxbpm AND maxbpm<=%d AND %d<=level AND level<=%d ORDER BY random()", key, minbpm, maxbpm, minlevel, maxlevel);
	}

	SQL_prepare(query, sql, &pStmt);
	if (sqlite3_step(pStmt) == 100) {
		CSTR songhash = GetRandomSongHash(SQL_GetColumn(0, pStmt), SQL_GetColumn(1, pStmt), cfg.optimumlevel_7, key, sql, maxlevel, minlevel);
		if (GetSongData(songhash, &course->data[course->count], sql, ss) == 1) {
			course->count++;
			sqlite3_finalize(pStmt);

			int rangeminbpm = minbpm;
			int rangemaxbpm = maxbpm;
			if (cfg.bpmrange > 0) {
				if (minbpm < course->data[0].maxBPM - cfg.bpmrange)
					rangeminbpm = course->data[0].maxBPM - cfg.bpmrange;
				if (course->data[0].maxBPM + cfg.bpmrange < maxbpm)
					rangemaxbpm = course->data[0].maxBPM + cfg.bpmrange;
			}
			cstrSprintf(&query, "SELECT hash,folder FROM song WHERE mode=%d AND %d<=minbpm AND maxbpm<=%d  AND %d<=level AND level<=%d ORDER BY random()", key, rangeminbpm, rangemaxbpm, minlevel, maxlevel);
			SQL_prepare(query, sql, &pStmt);
			while (sqlite3_step(pStmt) == 100) {

				bool isSamesong = 0;
				for (int i = 0; i < course->count; i++) {
					if (SQL_GetColumn(1, pStmt).isSame(&course->data[i].folder))
						isSamesong = true;
				}

				if (!isSamesong) {
					CSTR songhash = GetRandomSongHash(SQL_GetColumn(0, pStmt), SQL_GetColumn(1, pStmt), cfg.optimumlevel_7, key, sql, maxlevel, minlevel);
					if (GetSongData(songhash, &course->data[course->count], sql, ss) == 1) {
						course->count++;
					}
				}

				if (course->count == stages) break;
			}
			sqlite3_finalize(pStmt);
			return 1;
		}
		sqlite3_finalize(pStmt);
	}
	else {
		sqlite3_finalize(pStmt);
	}
	return 0;
}


//44beb0 //TOFIX: unneccessary codes
int GetFolderDataFromPath(CSTR path, sqlite3 *sql) {

	HANDLE hFindFile;
	_WIN32_FIND_DATAA findFileData;
	char str[1024];
	int filetime, now;

	ErrorLogFmtAdd("ルートの読み込みを行います。パス%s\n", path);
	ErrorLogTabAdd();
	BMSMETA meta;
	CSTR searchPath(path);
	if (searchPath.right(1).isSame("\\")) {
		*searchPath.atPos(searchPath.length() - 1) = 0;
	}

	hFindFile = FindFirstFileA(searchPath, &findFileData);
	if (hFindFile == (HANDLE)-1) {
		FindClose((HANDLE)-1);
		ErrorLogFmtAdd("フォルダもしくはファイルが見つかりません。\n");
		ErrorLogTabSub();
		return -1;
	}
	
	filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
	now = GetNowUnixtime();
	CSTR folderinfo(path);
	folderinfo.add("folderinfo.txt");
	if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName)) && IsFileExist(folderinfo)) {
		ErrorLogFmtAdd("フォルダインフォのある通常フォルダです。\n");
		BMSMETA meta;
		ParseBMSMETA(&meta, folderinfo, 0);
		if (meta.judge != 2) meta.judge = 1;

		sqlite3_snprintf(1024, str, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , \'%q\' , %d) ",
			path, meta.title, AssignCRC32("ROOT"), meta.genre, meta.artist, meta.subartist, meta.tag, meta.selLevel, filetime, meta.judge, meta.bannerpath, now);
		if (SQL_Run(str, sql) == 0) {
			ErrorLogFmtAdd("新規登録なので検索を行います。\n");
			SearchSongsFromPath(path, sql, path);
		}
	}
	else { // folderinfo not exist
		if (IsLR2Folder(path)) {
			ErrorLogFmtAdd("カスタムフォルダです。\n");
			filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
			ParseBMSMETA(&meta, path, 0);

			meta.judge = 2;
			if (meta.tag.isSame("__NEWSONG__")) meta.judge = 3;
			else if (meta.tag.isSame("__RIVAL__")) meta.judge = 4;
			else if (meta.tag.isSame("__EXPERT__") || meta.tag.isSame("__NONSTOP__") || meta.tag.isSame("__GRADE__")) meta.judge = 6;

			sqlite3_snprintf(1024, str, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , \'%q\' , %d) ",
				path, meta.title, AssignCRC32("ROOT"), meta.genre, meta.artist, meta.subartist, meta.tag, meta.selLevel, filetime, meta.judge, meta.bannerpath, now);
			SQL_Run(str, sql);
		}
		else if (((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) && (strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName))) {
			ErrorLogFmtAdd("通常フォルダです。\n");

			sqlite3_snprintf(1024, str, "INSERT INTO folder (path , title , parent , date , type , adddate) VALUES(\'%q\',\'%q\',\'%q\',%d , 1 , %d )",
				path, path.getFilename(), AssignCRC32("ROOT"), filetime, now);
			if (SQL_Run(str, sql) == 0) {
				ErrorLogFmtAdd("新規登録なので検索を行います。\n");
				SearchSongsFromPath(path, sql, path);
			}
		}
	}
	FindClose(hFindFile);
	ErrorLogFmtAdd("ルートの読み込みを終了しました。\n");
	ErrorLogTabSub();
	return 1;
}

//44c610 //TODO : rename variables
int LoadFilteredBmsListFromDB(CSTR query, sqlite3 *sql, SONGSELECT *ss, int *diffFilter, int *mode, uint sort, int rivalID, char flag) {

	sqlite3_stmt *pStmt;
	int lastreadDiff;
	int recentKeymode;

	ss->isExLevel = 0;
	if (query.findStrPos("exlevel") >= 0) ss->isExLevel = 1;

	if (ss->filter.ignorekeyall == 1 && *mode == 0) *mode = 1;
	if (ss->filter.ignorekeysingle == 1 && *mode == 1) *mode = 2;
	if (ss->filter.ignorekeydouble == 1 && *mode == 4) *mode = 5;
	if (ss->filter.ignoredp == 1 && 3 < *mode && *mode < 7) *mode = 7;
	if (ss->filter.ignorepms == 1 && *mode == 7) *mode = 0;
	if (ss->filter.ignorekeyall == 1 && *mode == 0) *mode = 1;
	
	if (ss->filter.ignoredifficultyall == 1 && *diffFilter == 0 && flag == 0) *diffFilter = 1;
	
	bool isIgnore = 0, isRival = 0;
	int now = GetNowUnixtime();

	ss->rivalID = 0;
	if (query.findStrPos("__EXPERT__") >= 0) {
		ss->course.type = 0;
		return SearchCourseFromDB(sql, ss, *mode, 0);
	}
	if (query.findStrPos("__NONSTOP__") >= 0) {
		ss->course.type = 1;
		return SearchCourseFromDB(sql, ss, *mode, 1);
	}
	if (query.findStrPos("__GRADE__") >= 0) {
		ss->course.type = 2;
		return SearchCourseFromDB(sql, ss, *mode, 2);
	}
	
	if (query.findStrPos("__RIVAL__") >= 0) {
		isRival = 1;
		SQL_Run("DETACH rivaldb", sql);
		CSTR str;
		cstrSprintf(&str, "ATTACH \'LR2files\\Rival\\%d.db\' AS rivaldb", rivalID);
		SQL_Run(str, sql);
		ss->rivalID = rivalID;
		rivalID = 0;
		cstrSprintf(&query, "SELECT * FROM (song LEFT JOIN score ON song.hash = score.hash) INNER JOIN rival ON song.hash = rival.hash");
	}
	else if (query.findStrPos("INNER JOIN rival") >= 0) {
		isRival = 1;
	}
	else if (query.findStrPos("__NEWSONG__") >= 0) {
		cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE adddate > %d", now - ss->titleflash * 3600);
	}

	if (query.findStrPos("favorite = 2") >= 0) {
		isIgnore = 1;
	}

	if (query.left(6).isDiff("SELECT")) return -1;

	int isRandom = ss->isRandomFolder;
	if (rivalID == 1 && query.findStrPos("random()") == -1) {
		isRandom = 1;
		*diffFilter = 0;
		query.add(" ORDER BY random()");
		*mode = 0;
	}
	if (query.findStrPos("ORDER BY") != -1) {
		isRandom = 1;
		*diffFilter = 0;
	}
	if (flag) *diffFilter = 0;
	int keyFilter = 0;
	int k5count = 0, k7count = 0, k10count = 0, k14count = 0, k9count = 0;
	int diffcount[6] = { 0, };
	CSTR queryCpy(query);
	if (isRandom == 0) {
		switch (*mode) {
			case 1:
				keyFilter = 1;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 2:
				keyFilter = 7;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 3:
				keyFilter = 5;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 4:
				keyFilter = 2;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 5:
				keyFilter = 14;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 6:
				keyFilter = 10;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 7:
				keyFilter = 9;
				query.add(" ORDER BY folder ASC,difficulty ASC,level ASC");
				break;
			default:
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
		}
	}
	GetTimeWrap();
	SQL_prepare(query, sql, &pStmt);
	int count = 0;
	SONGDATA song;
	CSTR lastreadFolder, thisFolder;
	CSTR replayPath;
	//
	int countSameDiff = 0;
	int lastKeymode = -1;
	CSTR lastFolder;
	int diffLevel[5] = { -1, };
	int diffBarLamp[5] = { -1, };
	int diffExist[5] = { -1, };
	for (int i = 0; i < 5; i++) diffLevel[i] = -1;
	for (int i = 0; i < 5; i++) diffBarLamp[i] = -1;
	for (int i = 0; i < 5; i++) diffExist[i] = -1;
	InitSongData(&song);
	CSTR nowFolder;
	int nowCount = 0;
	int lastreadKeymode = 0;

	while (sqlite3_step(pStmt) == 100 && (rivalID == 0 || count != rivalID)) {
		int t = sqlite3_column_int(pStmt, 18);
		if ((ss->filter.ignoredp != 1 || t < 10) && (ss->filter.ignorepms != 1 || t != 9)) {

			if (count == ss->prevListSize - 1) {
				ss->prevList = (SONGDATA*)realloc(ss->prevList, (ss->prevListSize + 1000) * sizeof(SONGDATA));
				for (int i = ss->prevListSize; i < ss->prevListSize + 1000; i++) {
					memset(&ss->prevList[i], 0, sizeof(SONGDATA));
				}
				ss->prevListSize += 1000;
			}
			
			song.keymode = t;
			song.difficulty = sqlite3_column_int(pStmt, 15);
			song.favorite = sqlite3_column_int(pStmt, 24);
			if (ss->filter.ignore5key == 1) {
				
				song.folder = SQL_GetColumn(9, pStmt);
				if (nowFolder.isDiff(song.folder)) {
					nowCount = 0;
				}
				else {
					if (lastreadKeymode == 5 && song.keymode == 7) {
						count -= nowCount;
						nowCount = 0;
					}
					else if (lastreadKeymode = 10 && song.keymode == 14) {
						count -= nowCount;
						nowCount = 0;
					}
					else if (lastreadKeymode != song.keymode) {
						nowCount = 0;
					}
				}
			}
			if (isIgnore == 0 && song.favorite != 2) {
				if (song.keymode == 5) k5count++;
				else if (song.keymode == 7) k7count++;
				else if (song.keymode == 10) k10count++;
				else if (song.keymode == 14) k14count++;
				else if (song.keymode == 9) k9count++;
			}
			if (count < 0) count = 0;
						
			if ((song.favorite != 2 && ((song.keymode == 7 && keyFilter == 7) || (song.keymode <= 8 && *mode == 1 && keyFilter == 1) || (song.keymode == 14 && keyFilter >= 14) || (song.keymode >= 10 && *mode == 4 && keyFilter == 2) || ((song.keymode == 9 || song.keymode == 10 || song.keymode == 5) && song.keymode == keyFilter) || keyFilter == 0)) || song.keymode == 0 || isIgnore == 1) {
				if (song.difficulty == 1) {
					diffcount[1] ++;
				}
				else if (song.difficulty == 2) {
					diffcount[2]++;
				}
				else if (song.difficulty == 3) {
					diffcount[3]++;
				}
				else if (song.difficulty == 4) {
					diffcount[4]++;
				}
				else if (song.difficulty == 5) {
					diffcount[5]++;
				}
				else {
					diffcount[0]++;
				}
				song.maxBPM = sqlite3_column_int(pStmt, 16);
				song.minBPM = sqlite3_column_int(pStmt, 17);
				song.hash = SQL_GetColumn(0, pStmt);
			
				//not same code with original. logic is same
				bool unique = 1;
				for (int i = 0; i < count; i++) {
					if (song.hash.isSame(ss->prevList[i].hash)) {
						unique = 0;
						i = count;
					}
				}
				if(unique) {
					if (song.hash.isSame("9cbea4427d7e8caff0d28f9d7600cdec")) 
						song.title = AutomationFactory();
					else 
						song.title = SQL_GetColumn(1, pStmt);
					song.subtitle = SQL_GetColumn(2, pStmt);
					song.genre = SQL_GetColumn(3, pStmt);
					song.filepath = SQL_GetColumn(7, pStmt);
					song.folderType = sqlite3_column_int(pStmt, 8);
					song.level = sqlite3_column_int(pStmt, 14);
					song.artist = SQL_GetColumn(4, pStmt);
					song.subartist = SQL_GetColumn(5, pStmt);
					song.keymode = sqlite3_column_int(pStmt, 18);
					song.judge = sqlite3_column_int(pStmt, 19);
					song.longnote = sqlite3_column_int(pStmt, 20);
					song.bga = sqlite3_column_int(pStmt, 21);
					song.random = sqlite3_column_int(pStmt, 22);
					song.txt = sqlite3_column_int(pStmt, 25);
					song.favorite = sqlite3_column_int(pStmt, 24);
					song.adddate = sqlite3_column_int(pStmt, 27);
					song.exlevel = sqlite3_column_int(pStmt, 28);
					song.stagefile = SQL_GetColumn(10, pStmt);
					if (song.stagefile.isDiff("(null)") && song.stagefile.length() > 4) song.isStagefile = 1;
					song.banner = SQL_GetColumn(11, pStmt);
					if (song.banner.isDiff("(null)") && song.banner.length() > 4) song.isBanner = 1;
					song.backBMP = SQL_GetColumn(12, pStmt);
					if (song.backBMP.isDiff("(null)") && song.backBMP.length() > 4) song.isBackBMP = 1;

					if (song.subtitle.isDiff("(null)"))
						cstrSprintf(&song.fulltitle, "%s %s", song.title, song.subtitle);
					else
						song.fulltitle = song.title;

					song.tag = SQL_GetColumn(6, pStmt);
					song.difficulty = sqlite3_column_int(pStmt, 15);
					cstrSprintf(&replayPath, "LR2files/Replay/%s/%s.lr2rep", ss->playerID, song.hash);
					song.replayExist = IsFileExist(replayPath);
					thisFolder = SQL_GetColumn(9, pStmt);
					song.folder = thisFolder;

					if (song.keymode > 0) {
						song.mybest.clear = sqlite3_column_int(pStmt, 30);
						song.mybest.stat_pgreat = sqlite3_column_int(pStmt, 31);
						song.mybest.stat_great = sqlite3_column_int(pStmt, 32);
						song.mybest.stat_good = sqlite3_column_int(pStmt, 33);
						song.mybest.stat_bad = sqlite3_column_int(pStmt, 34);
						song.mybest.stat_poor = sqlite3_column_int(pStmt, 35);
						song.mybest.total_notes = sqlite3_column_int(pStmt, 36);
						song.mybest.stat_maxcombo = sqlite3_column_int(pStmt, 37);
						song.mybest.minbp = sqlite3_column_int(pStmt, 38);
						song.mybest.playcount = sqlite3_column_int(pStmt, 39);
						song.mybest.clearcount = sqlite3_column_int(pStmt, 40);
						song.mybest.failcount = sqlite3_column_int(pStmt, 41);
						song.mybest.rank = sqlite3_column_int(pStmt, 42);
						song.mybest.rate = sqlite3_column_int(pStmt, 43);
						if (song.mybest.rank < 8) {
							if (song.mybest.rank == 0 && (0 < song.mybest.stat_great + song.mybest.stat_pgreat)) {
								song.mybest.rank = 1;
							}
						}
						else {
							song.mybest.rank = 8;
						}
						song.mybest.clear_db = sqlite3_column_int(pStmt, 44);
						song.mybest.op_history = sqlite3_column_int(pStmt, 45);
						song.mybest.clear_sd = sqlite3_column_int(pStmt, 48);
						song.mybest.clear_ex = sqlite3_column_int(pStmt, 49);
						song.mybest.op_best = sqlite3_column_int(pStmt, 50);
						song.mybest.rseed = sqlite3_column_int(pStmt, 51);
						song.mybest.complete = sqlite3_column_int(pStmt, 52);
						if (isRival) {
							song.folderType = 5;
							song.rivalRecord.clear = sqlite3_column_int(pStmt, 54);
							song.rivalRecord.total_notes = sqlite3_column_int(pStmt, 55);
							song.rivalRecord.stat_maxcombo = sqlite3_column_int(pStmt, 56);
							song.rivalRecord.stat_pgreat = sqlite3_column_int(pStmt, 57);
							song.rivalRecord.stat_great = sqlite3_column_int(pStmt, 58);
							song.rivalRecord.stat_good = sqlite3_column_int(pStmt, 59);
							song.rivalRecord.stat_bad = sqlite3_column_int(pStmt, 60);
							song.rivalRecord.stat_poor = sqlite3_column_int(pStmt, 61);
							song.rivalRecord.minbp = sqlite3_column_int(pStmt, 62);
							song.rivalRecord.op_best = sqlite3_column_int(pStmt, 63);
							song.rivalRecord.stat_exscore = song.rivalRecord.stat_great + song.rivalRecord.stat_pgreat * 2;
							if (0 < song.rivalRecord.total_notes) {
								song.rivalRecord.stat_score = ((song.rivalRecord.stat_good + song.rivalRecord.stat_exscore * 2) * 50000) / song.rivalRecord.total_notes;
								song.rivalRecord.rate = (song.rivalRecord.stat_exscore * 50) / song.rivalRecord.total_notes;
								song.rivalRecord.rank = (song.rivalRecord.stat_exscore * 9) / (song.rivalRecord.total_notes * 2);
								if (song.rivalRecord.rank < 9) {
									if (song.rivalRecord.rank == 0 && (0 < song.rivalRecord.stat_great + song.rivalRecord.stat_pgreat)) {
										song.rivalRecord.rank = 1;
									}
								}
								else {
									song.rivalRecord.rank = 8;
								}
							}
						}
						CSTR bestHash;
						bestHash = SQL_GetColumn(46, pStmt);
						if (isSameScoreHash(&song.mybest, &ss->playerPassMD5, &song.hash, &bestHash)) {
							song.mybest.stat_exscore = song.mybest.stat_great + song.mybest.stat_pgreat * 2;
							if (song.mybest.total_notes > 0) {
								song.mybest.stat_score = ((song.mybest.stat_good + song.mybest.stat_exscore * 2) * 50000) /	song.mybest.total_notes;
							}
							if ((song.mybest.minbp == 0) && (song.mybest.clear != 5)) {
								song.mybest.minbp = -1;
							}
						}
						else {
							memset(&song.mybest, 0, sizeof(STATUS));
							song.mybest.minbp = -1;
							song.mybest.total_notes = sqlite3_column_int(pStmt, 26);
						}
					}
					if (lastFolder.isDiff(song.folder) || lastKeymode != song.keymode) {
						for (int i = count - 1; i >= 0; i--) {
							if (ss->prevList[i].folder.isDiff(lastFolder) || ss->prevList[i].keymode != lastKeymode)	break;

							if (ss->prevList[i].difficulty == 1) {
								ss->prevList[i].difficultyLevel[0] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[0] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[0] = diffLevel[0];
								ss->prevList[i].difficultyLevelBarLamp[0] = diffBarLamp[0];
							}
							ss->prevList[i].difficultyExist[0] = diffExist[0];

							if (ss->prevList[i].difficulty == 2) {
								ss->prevList[i].difficultyLevel[1] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[1] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[1] = diffLevel[1];
								ss->prevList[i].difficultyLevelBarLamp[1] = diffBarLamp[1];
							}
							ss->prevList[i].difficultyExist[1] = diffExist[1];

							if (ss->prevList[i].difficulty == 3) {
								ss->prevList[i].difficultyLevel[2] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[2] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[2] = diffLevel[2];
								ss->prevList[i].difficultyLevelBarLamp[2] = diffBarLamp[2];
							}
							ss->prevList[i].difficultyExist[2] = diffExist[2];

							if (ss->prevList[i].difficulty == 4) {
								ss->prevList[i].difficultyLevel[3] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[3] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[3] = diffLevel[3];
								ss->prevList[i].difficultyLevelBarLamp[3] = diffBarLamp[3];
							}
							ss->prevList[i].difficultyExist[3] = diffExist[3];

							if (ss->prevList[i].difficulty == 5) {
								ss->prevList[i].difficultyLevel[4] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[4] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[4] = diffLevel[4];
								ss->prevList[i].difficultyLevelBarLamp[4] = diffBarLamp[4];
							}
							ss->prevList[i].difficultyExist[4] = diffExist[4];
						}
						diffBarLamp[0] = -1;
						diffBarLamp[1] = -1;
						diffBarLamp[2] = -1;
						diffBarLamp[3] = -1;
						diffBarLamp[4] = -1;
						diffExist[0] = 0;
						diffExist[1] = 0;
						diffExist[2] = 0;
						diffExist[3] = 0;
						diffExist[4] = 0;
						diffLevel[0] = -1;
						diffLevel[1] = -1;
						diffLevel[2] = -1;
						diffLevel[3] = -1;
						diffLevel[4] = -1;
					}
					lastFolder = song.folder;
					lastKeymode = song.keymode;
					if (1 <= song.difficulty && song.difficulty < 6) {
						if (diffLevel[song.difficulty - 1] == -1) {
							diffLevel[song.difficulty - 1] = song.level;
							diffBarLamp[song.difficulty - 1] = song.mybest.clear;
						}
						song.difficultyCount = diffExist[song.difficulty - 1];
						diffExist[song.difficulty - 1] = song.difficultyCount + 1;
					}
					if (song.keymode == 0 || ss->isExLevel == 1) {
						lastreadFolder = thisFolder;
						COPY_SONGDATA(&ss->prevList[count], &song);
						recentKeymode = song.keymode;
						countSameDiff = 1;
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
					else if (thisFolder.isDiff(lastreadFolder)) {
						lastreadFolder = thisFolder;
						recentKeymode = song.keymode;
						lastreadDiff = song.difficulty;
						countSameDiff = 1;
						COPY_SONGDATA(&ss->prevList[count], &song);
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
					else if (song.keymode != recentKeymode) {
						recentKeymode = song.keymode;
						lastreadDiff = song.difficulty;
						countSameDiff = 1;
						COPY_SONGDATA(&ss->prevList[count], &song);
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
					else if (lastreadDiff < song.difficulty && song.difficulty <= *diffFilter && *diffFilter) {
						nowCount -= countSameDiff;
						count -= countSameDiff;
						if (nowCount < 1) nowCount = 0;
						lastreadDiff = song.difficulty;
						countSameDiff = 1;
						COPY_SONGDATA(&ss->prevList[count], &song);
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
					else if (lastreadDiff == song.difficulty || *diffFilter == 0) {
						countSameDiff++;
						COPY_SONGDATA(&ss->prevList[count], &song);
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
				}
				InitSongData(&song);
			}
		}
	}

	for (int i = count-1; i >= 0; i--) {

		if (ss->prevList[i].folder.isDiff(lastFolder) || ss->prevList[i].keymode != lastKeymode)	break;

		if (ss->prevList[i].difficulty == 1) {
			ss->prevList[i].difficultyLevel[0] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[0] = ss->prevList[i].mybest.clear;
		}
		else {
			ss->prevList[i].difficultyLevel[0] = diffLevel[0];
			ss->prevList[i].difficultyLevelBarLamp[0] = diffBarLamp[0];
		}
		ss->prevList[i].difficultyExist[0] = diffExist[0];

		if (ss->prevList[i].difficulty == 2) {
			ss->prevList[i].difficultyLevel[1] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[1] = ss->prevList[i].mybest.clear;
		}
		else {
			ss->prevList[i].difficultyLevel[1] = diffLevel[1];
			ss->prevList[i].difficultyLevelBarLamp[1] = diffBarLamp[1];
		}
		ss->prevList[i].difficultyExist[1] = diffExist[1];

		if (ss->prevList[i].difficulty == 3) {
			ss->prevList[i].difficultyLevel[2] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[2] = ss->prevList[i].mybest.clear;
		}
		else {
			ss->prevList[i].difficultyLevel[2] = diffLevel[2];
			ss->prevList[i].difficultyLevelBarLamp[2] = diffBarLamp[2];
		}
		ss->prevList[i].difficultyExist[2] = diffExist[2];

		if (ss->prevList[i].difficulty == 4) {
			ss->prevList[i].difficultyLevel[3] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[3] = ss->prevList[i].mybest.clear;
		}
		else {
			ss->prevList[i].difficultyLevel[3] = diffLevel[3];
			ss->prevList[i].difficultyLevelBarLamp[3] = diffBarLamp[3];
		}
		ss->prevList[i].difficultyExist[3] = diffExist[3];

		if (ss->prevList[i].difficulty == 5) {
			ss->prevList[i].difficultyLevel[4] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[4] = ss->prevList[i].mybest.clear;
		}
		else{
			ss->prevList[i].difficultyLevel[4] = diffLevel[4];
			ss->prevList[i].difficultyLevelBarLamp[4] = diffBarLamp[4];
		}
		ss->prevList[i].difficultyExist[4] = diffExist[4];
	}
	sqlite3_finalize(pStmt);

	GetTimeWrap();
	if (ss->unk5000 == 1 && count <= 0) {
		ss->unk5000 = 0;
		ErrorLogAdd("適正な曲が見つかりません。リトライも行いません。\n");
		return 0;
	}
	else if (count > 0) {
		if (!(ss->unk5000 || count <= 0 || *diffFilter <= 0 || diffcount[*diffFilter] || (k5count == 0 && k7count == 0 && k10count == 0 && k14count == 0 && k9count == 0))) {
			int diffCycle = *diffFilter;
			do {
				if (*diffFilter == 0) break;
				*diffFilter++;
				if (*diffFilter >= 6) *diffFilter = 0;
				if (ss->filter.ignoredifficultyall && *diffFilter == 0) *diffFilter = 1;
			} while (diffcount[*diffFilter] == 0);

			if (*diffFilter == diffCycle) {
				ErrorLogAdd("適正な曲が見つかりません 5\n");
				return -1;
			}
			ErrorLogFmtAdd("難度を変更します。　%d→%d\n", diffCycle, *diffFilter);
			flag = 0;
			return LoadFilteredBmsListFromDB(queryCpy, sql, ss, diffFilter, mode, sort, rivalID, flag);
		}
	}
	else if (ss->unk5000 == 0) {
		if (!k5count && !k7count && !k10count && !k14count && !k9count) {
			ErrorLogAdd("適正な曲が見つかりません 0\n");
			return -1;
		}
		else if (!k5count && !k7count && (k10count || k14count) && !k9count && ss->filter.ignoredp && !ss->filter.ignorepms) {
			ErrorLogAdd("適正な曲が見つかりません 1\n");
			return -1;
		}
		else if (!k5count && !k7count && !k10count && !k14count && k9count && !ss->filter.ignoredp && ss->filter.ignorepms) {
			ErrorLogAdd("適正な曲が見つかりません 2\n");
			return -1;
		}
		else if (!k5count && !k7count && (k10count || k14count || k9count) && ss->filter.ignoredp && ss->filter.ignorepms) {
			ErrorLogAdd("適正な曲が見つかりません 3\n");
			return -1;
		}
		else{
			int modeCycle = *mode;
			for (*mode = modeCycle + 1; *mode != modeCycle; *mode++) {
				if (*mode == 8) *mode = 0;
				if (ss->filter.ignorekeyall == 1 && *mode == 0) *mode = 1;
				if (ss->filter.ignorekeysingle == 1 && *mode == 1) *mode = 2;
				if (ss->filter.ignorekeydouble == 1 && *mode == 4) *mode = 5;
				if (ss->filter.ignoredp == 1 && (3 < *mode && *mode < 7)) *mode = 7;
				if (ss->filter.ignorepms == 1 && *mode == 7) *mode = 0;
				if (ss->filter.ignorekeyall == 1 && *mode == 0) *mode = 1;

				bool exit = 1;
				switch (*mode) {
				case 1:
					if (k5count == 0) {
						if (k7count == 0) exit = 0;
						break;
					}
					break;
				case 2:
					if (k7count == 0) exit = 0;
					break;
				case 3:
					if (k5count == 0) exit = 0;
					break;
				case 4:
					if (k10count == 0) {
						if (k14count == 0) exit = 0;
						break;
					}
					break;
				case 5:
					if (k14count == 0) exit = 0;
					break;
				case 6:
					if (k10count == 0) exit = 0;
					break;
				case 7:
					if (k9count == 0) exit = 0;
					break;
				default:
					break;
				}
				if (exit) break;
			}
			if (*mode == modeCycle) {
				ErrorLogAdd("適正な曲が見つかりません 4\n");
				return -1;
			}
			ErrorLogFmtAdd("鍵盤数を変更します %d→%d\n", modeCycle, *mode);
			return LoadFilteredBmsListFromDB(queryCpy, sql, ss, diffFilter, mode, sort, rivalID, flag);
		}
		return -1;
	}

	ss->unk5000 = 0;
	ss->prevListCount = count;
	if(isRandom == 0) {
		switch (sort) {
			case 0:
				for (int i = 0; i < ss->prevListCount; i++) {
					ss->prevList[i].folder = ss->prevList[i].filepath.getDirectory();
				}
				qsort(ss->prevList, ss->prevListCount, sizeof(SONGDATA), CMP_SongDataByDirectory);
				break;
			case 1:
				if ((EnabledInsane == 0 || ss->filter.disabledifficultyfilter == 0) && ss->isExLevel == 0) {
					qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByDifficulty);
				}
				else {
					qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByExLevel);
				}
				break;
			case 2:
				qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByTitle);
				break;
			case 3:
				qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByClear);
				break;
			case 4:
				if (isRival == 0) {
					qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByScore);
				}
				else {
					qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByRivalScore);
				}
				break;
			default:
				break;
		}
	}

	ss->is_loading_bmslist = 1;
	ErrorLogFmtAdd("成功　曲数は%dです。\n",ss->prevListCount);
	return ss->prevListCount;
}

//44e690 LoadLR2CustomFolder
int LoadLR2CustomFolder(sqlite3 *sql, CONFIG_JUKEBOX *jb, CSTR scoreDBpath, char flag_starter, char flag_direct) {

	sqlite3 *scoreDB, *tagDB;
	char query[1024], query2[256];

	InitializeCriticalSection(&DB_lock);
	int folderAddCount = 0;
	EnabledInsane = 0;
	if (flag_starter) {
		SQL_Run("CREATE TABLE player(id TEXT primary key,hash TEXT,name TEXT,irid TEXT,irhash TEXT,playcount INTEGER,clear INTEGER,fail INTEGER,perfect INTEGER,great INTEGER,good INTEGER,bad INTEGER,poor INTEGER,playtime INTEGER,combo INTEGER,maxcombo INTEGER,grade_7 INTEGER,grade_5 INTEGER,grade_14 INTEGER,grade_10 INTEGER,grade_9 INTEGER,trial INTEGER,option INTEGER,systemversion INTEGER,gradeversion INTEGER, trialversion INTEGER,scorehash TEXT)", sql);
		SQL_Run("CREATE TABLE folder(title TEXT ,subtitle TEXT ,category TEXT,info_a TEXT,info_b TEXT,command TEXT,path TEXT primary key,type INTEGER,banner TEXT,parent TEXT,date INTEGER,max INTEGER,adddate INTEGER)", sql);
		SQL_Run("CREATE TABLE song(hash TEXT ,title TEXT ,subtitle TEXT ,genre TEXT,artist TEXT,subartist TEXT,tag TEXT ,path TEXT primary key ,type INTEGER,folder TEXT,stagefile TEXT,banner TEXT,backbmp TEXT,parent TEXT,level INTEGER,difficulty INTEGER,maxbpm INTEGER,minbpm INTEGER,mode INTEGER,judge INTEGER,longnote INTEGER,bga INTEGER,random INTEGER,date INTEGER,favorite INTEGER,txt INTEGER,karinotes INTEGER,adddate INTEGER)", sql);
		SQL_Run("CREATE TABLE course(id INTEGER primary key,title TEXT,line INTEGER,hash TEXT)", sql);
		SQL_Run("CREATE TABLE tag(hash TEXT primary key,title TEXT ,subtitle TEXT ,genre TEXT,artist TEXT,subartist TEXT,tag TEXT ,level INTEGER,difficulty INTEGER,mode INTEGER)", sql);
		SQL_Run("CREATE TABLE score(hash TEXT primary key,clear INTEGER,perfect INTEGER,great INTEGER,good INTEGER,bad INTEGER,poor INTEGER,totalnotes INTEGER,maxcombo INTEGER,minbp INTEGER,playcount INTEGER,clearcount INTEGER,failcount INTEGER,rank INTEGER,rate INTEGER,clear_db INTEGER,op_history INTEGER,scorehash TEXT,ghost TEXT,clear_sd INTEGER,clear_ex INTEGER,op_best INTEGER, rseed INTEGER, complete INTEGER)", sql);
	}
	else {
		if (flag_direct == 0) {
			//TODO : make define customfolderoption constant
			if (jb->customfolder & 1) {
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\RANDOM\\";
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 2) {
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\favorite.lr2folder";
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 4) {
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\TOP10.lr2folder";
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 8) {
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\PLAYLEVEL\\";
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 0x10) {
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\CLEAR\\";
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 0x20) {
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\RANK\\";
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 0x40) {
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\ignore.lr2folder";
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 0x80) {
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\INSANE01\\";
				jb->numOfPath++;
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\INSANE02\\";
				jb->numOfPath++;
				folderAddCount+=2;
				EnabledInsane = 1;
			}
			jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\course1.lr2folder";
			jb->numOfPath++;
			jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\course2.lr2folder";
			jb->numOfPath++;
			jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\course3.lr2folder";
			jb->numOfPath++;
			folderAddCount += 3;
		}

		sqlite3_open(scoreDBpath,&scoreDB);
		sqlite3_open("LR2files\\Database\\tag.db", &tagDB);
		SQL_Run("CREATE TABLE folder(title TEXT ,subtitle TEXT ,category TEXT,info_a TEXT,info_b TEXT,command TEXT,path TEXT primary key,type INTEGER,banner TEXT,parent TEXT,date INTEGER,max INTEGER,adddate INTEGER)", sql);
		SQL_Run("CREATE TABLE song(hash TEXT ,title TEXT ,subtitle TEXT ,genre TEXT,artist TEXT,subartist TEXT,tag TEXT ,path TEXT primary key ,type INTEGER,folder TEXT,stagefile TEXT,banner TEXT,backbmp TEXT,parent TEXT,level INTEGER,difficulty INTEGER,maxbpm INTEGER,minbpm INTEGER,mode INTEGER,judge INTEGER,longnote INTEGER,bga INTEGER,random INTEGER,date INTEGER,favorite INTEGER,txt INTEGER,karinotes INTEGER,adddate INTEGER,exlevel INTEGER)", sql);
		SQL_Run("CREATE INDEX hashidx ON song (hash)", sql);
		SQL_Run("CREATE INDEX parentidx ON song (parent)", sql);
		SQL_Run("DROP TABLE course", sql);
		SQL_Run("CREATE TABLE nonstop(id INTEGER primary key,title TEXT,line INTEGER,hash TEXT,ir INTEGER)", sql);
		SQL_Run("CREATE TABLE expert(id INTEGER primary key,title TEXT,line INTEGER,hash TEXT,ir INTEGER)", sql);
		SQL_Run("CREATE TABLE grade(id INTEGER primary key,title TEXT,line INTEGER,hash TEXT,ir INTEGER)", sql);
		SQL_Run("CREATE TABLE tag(hash TEXT primary key,title TEXT ,subtitle TEXT ,genre TEXT,artist TEXT,subartist TEXT,tag TEXT ,level INTEGER,difficulty INTEGER,mode INTEGER,exlevel INTEGER)", tagDB);
		SQL_Run("CREATE TABLE score(hash TEXT primary key,clear INTEGER,perfect INTEGER,great INTEGER,good INTEGER,bad INTEGER,poor INTEGER,totalnotes INTEGER,maxcombo INTEGER,minbp INTEGER,playcount INTEGER,clearcount INTEGER,failcount INTEGER,rank INTEGER,rate INTEGER,clear_db INTEGER,op_history INTEGER,scorehash TEXT,ghost TEXT,clear_sd INTEGER,clear_ex INTEGER,op_best INTEGER, rseed INTEGER, complete INTEGER)", scoreDB);
		SQL_Run("CREATE INDEX hashidx ON score (hash)", scoreDB);
		sqlite3_close(scoreDB);
		sqlite3_close(tagDB);

		sqlite3_snprintf(256, query2, "ATTACH \'%q\' AS scoredb", scoreDBpath);
		if (SQL_Run(query2, sql) != 0) {
			ErrorLogAdd("スコアデータベースの接続に失敗しました。\n");
			return -1;
		}
		if (SQL_Run("ATTACH \'LR2files\\Database\\tag.db\' AS tagdb", sql) != 0) {
			ErrorLogAdd("タグとかデータベースの接続に失敗しました。\n");
			return -1;
		}
	}

	if (flag_direct == 0) {
		sqlite3_stmt *pStmt;

		SQL_Run("BEGIN", sql);
		ErrorLogAdd("エラーフォルダの検索を行います。\n");

		sqlite3_snprintf(1024, query, "SELECT * FROM folder WHERE parent = \'%s\'", AssignCRC32("ROOT"));
		SQL_prepare(query, sql, &pStmt);
		while (sqlite3_step(pStmt) == 100) {
			CSTR path;
			path = SQL_GetColumn(6, pStmt);

			bool notJukebox = 1;

			for (int i = 0; i < jb->numOfPath; i++) {
				if (path.isSame(&jb->path[i]))
					notJukebox = 0;
			}
			if(notJukebox) {
				ErrorLogFmtAdd("エラーフォルダを削除します。%s\n", path);
				sqlite3_snprintf(1024, query, "DELETE FROM folder WHERE path = \'%s\'", path);
				SQL_Run(query, sql);
			}
			else {
				ErrorLogFmtAdd("ジュークボックスに登録されています。%s\n", path);
			}
		}
		sqlite3_finalize(pStmt);

		if (flag_starter) {
			GetFolderDataFromPath(jb->path[0], sql);
		}
		else {
			for (int i = 0; i < jb->numOfPath; i++) {
				GetFolderDataFromPath(jb->path[i], sql);
			}
		}

		if (jb->autoreload == 2 || flag_starter) {
			ErrorLogAdd("フォルダ更新チェックを行います。\n");
			ReloadSongsByQuery("SELECT path,date FROM folder", sql, jb);
			ErrorLogAdd("ファイル更新チェックを行います。\n");
			ReloadSongsByQuery("SELECT path,date FROM song", sql, jb);
		}
		else {
			ErrorLogAdd("フォルダ更新チェック(ルートのみ)を行います。\n");
			sqlite3_snprintf(1024, query, "SELECT path,date FROM folder WHERE parent = \'%s\' OR date = 0", AssignCRC32("ROOT"));
			ReloadSongsByQuery(query, sql, jb);
		}

		ErrorLogAdd("未定義#DIFFICULTYの自動設定を行います。\n");
		SetUndefinedDifficulty(sql);
		ErrorLogAdd("データベースチェックは終了しました。\n");

		if (flag_starter == 0) {
			SQL_Run("DELETE FROM folder WHERE path LIKE \'LR2files\\Rival\\%\'", sql);

			for (int i = 0; i < 20; i++) {
				if (jb->rival[i] < 1) break;
				cstrSprintf(&jb->path[jb->numOfPath], "LR2files\\Rival\\%d.lr2folder", jb->rival[i]);
				GetFolderDataFromPath(jb->path[jb->numOfPath], sql);
				jb->numOfPath++;
				folderAddCount++;
			}

			SQL_Run("DELETE FROM folder WHERE path=\'LR2files\\CustomFolder\\newsong.lr2folder\'", sql);
			sqlite3_snprintf(1024, query, "SELECT * FROM song WHERE adddate > %d", GetNowUnixtime() - jb->titleflash * 60); //TOFIX : 3600
			sqlite3_stmt *pStmt;
			SQL_prepare(query, sql, &pStmt);
			if (sqlite3_step(pStmt) == 100) {
				jb->path[jb->numOfPath] = "LR2files\\CustomFolder\\newsong.lr2folder";
				GetFolderDataFromPath(jb->path[jb->numOfPath], sql);
				jb->numOfPath++;
				folderAddCount++;
			}
			sqlite3_finalize(pStmt);
		}
		sqlite3_exec(sql, "COMMIT", 0, 0, 0);
		jb->numOfPath -= folderAddCount;
		for (int i = jb->numOfPath; i < 1000; i++) {
			jb->path[i].fillzero();
		}
	}
	return 1;
}



//sqlite3.c

////LR2graphic_draw
//49a770
bool IsMultibyte(byte ch){
	/*if (0x80 < ch) {
		if (ch < 0xa0) return true;
		if (0xdf < ch) return ch < 0xfe;
	}
	return false;*/
	return (0x81 <= ch && ch < 0xA0) || (0xE0 <= ch && ch < 0xFE);
}

//49a790
double ChangeValueByTime(double val1, double val2, double time1, double time2, double timenow, int type){
	double dVar1;

	if (val1 != val2) {
		if ( (time2 < timenow) || (time1 < timenow == (time1 == timenow)) || (time2 <= time1) ) {
			if (time1 < timenow) {
				return val2;
			}
		}
		else {
			if (type == 0) {
				dVar1 = (timenow - time1) / (time2 - time1);
				return (1.0 - dVar1) * val1 + dVar1 * val2;
			}
			if (type == 1) {
				dVar1 = (timenow - time1) / (time2 - time1);
				dVar1 = dVar1 * dVar1 * dVar1;
				return (1.0 - dVar1) * val1 + dVar1 * val2;
			}
			if (type == 2) {
				dVar1 = 1.0 - (timenow - time1) / (time2 - time1);
				dVar1 = 1.0 - dVar1 * dVar1 * dVar1;
				return (1.0 - dVar1) * val1 + dVar1 * val2;
			}
		}
	}
	return val1;
}

//49a840
DSTdraw SetDSTdrawByTime(DSTstruct dst, double time) {
	int tStart, tEnd;
	int t = time, t2;
	DSTdraw oBuf;

	DSTdraw *rDstd;
	int select;

	InitDSTdraw(&oBuf);
	if (dst.dstCount > 0 && dst.dataSize > 0) {
		tStart = dst.draw[0].time;
		tEnd = dst.draw[dst.dstCount -1].time;

		if (dst.loop < 0 && t > tEnd) {
			oBuf.time = -1;
			return oBuf;
		}
		t2 = tEnd;
		if (tStart <= tEnd && t >= tStart) {
			if (tStart == tEnd || dst.loop == tEnd) {
				if (t < t2) {
					t2 = t;
				}
			}
			else if (dst.loop < tEnd) {
				t2 = t;
				if (tEnd < t) {
					t2 = (t - dst.loop) % (tEnd - dst.loop) + dst.loop;
				}
			}
			else {
				t2 = 0;
			}

			select = 0;
			for (int i = 0; i < dst.dstCount; i++) {
				if (t2 >= dst.draw[i].time) {
					select = i;
				}
			}
			rDstd = &dst.draw[select];

			if (t2 != rDstd->time && select != dst.dstCount - 1 ) {
				oBuf.x = (int)(float)ChangeValueByTime((double)(float)rDstd->x, (double)(float)rDstd[1].x, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.y = (int)(float)ChangeValueByTime((double)(float)rDstd->y, (double)(float)rDstd[1].y, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.w = (int)(float)ChangeValueByTime((double)(float)rDstd->w, (double)(float)rDstd[1].w, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.h = (int)(float)ChangeValueByTime((double)(float)rDstd->h, (double)(float)rDstd[1].h, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.sortID = ChangeValueByTime((double)rDstd->sortID, (double)rDstd[1].sortID, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.a = ChangeValueByTime((double)rDstd->a, (double)rDstd[1].a, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.r = ChangeValueByTime((double)rDstd->r, (double)rDstd[1].r, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.g = ChangeValueByTime((double)rDstd->g, (double)rDstd[1].g, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.b = ChangeValueByTime((double)rDstd->b, (double)rDstd[1].b, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.angle = (float)ChangeValueByTime((double)rDstd->angle, (double)rDstd[1].angle, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.blend = rDstd->blend;
				oBuf.center = rDstd->center;
				oBuf.filter = rDstd->filter;
				oBuf.time = 0;
				return oBuf;
			}
			oBuf.x = rDstd->x;
			oBuf.y = rDstd->y;
			oBuf.w = rDstd->w;
			oBuf.h = rDstd->h;
			oBuf.a = rDstd->a;
			oBuf.r = rDstd->r;
			oBuf.g = rDstd->g;
			oBuf.b = rDstd->b;
			oBuf.sortID = rDstd->sortID;
			oBuf.angle = rDstd->angle;
			oBuf.blend = rDstd->blend;
			oBuf.filter = rDstd->filter;
			oBuf.center = rDstd->center;
			oBuf.time = 0;
			return oBuf;
		}
	}
	oBuf.time = -1;
	return oBuf;
}

//49abf0
int GetSRCcycleNow(SRCstruct src, double time){
	int ret;

	if (time < 0.0) return 0;
	if (src.graphcount < 1) return 0;
	
	if (src.cycle < 0) ret = 0;
	else if (src.cycle == 0) ret = 0;
	else ret = ((int)time % src.cycle) * src.graphcount / src.cycle;

	if (src.graphcount <= ret || ret < 0) ret = 0;

	return ret;
}

float skinsizeX, skinsizeY;

//49ac50
int LRDrawImg(int *grHandle, DSTdraw *dstD) {
	float x1, y1, x2, y2;
	float rx, ry, rad;
	VECTOR vec1, vec2, vec3, vec4;
	int xs, ys;

	if ((dstD->a < 2) && (dstD->blend != 0)) {
		return 0;
	}
	if ((dstD->blend == 1) && (0xfe < dstD->a)) {
		dstD->blend = 0;
	}
	SetDrawMode(dstD->filter);
	SetDrawBlendMode(dstD->blend, dstD->a);
	SetDrawBright(dstD->r, dstD->g, dstD->b);
	if ((ushort)((ushort)(0.0 < (float)dstD->h) << 8 | (ushort)((float)dstD->h == 0.0) << 0xe) == 0) {
		dstD->h = -(float)dstD->h;
		dstD->y = (int)((float)dstD->y - (float)dstD->h);
	}
	if ((ushort)((ushort)(0.0 < (float)dstD->w) << 8 | (ushort)((float)dstD->w == 0.0) << 0xe) == 0) {
		dstD->w = -(float)dstD->w;
		dstD->x = (int)((float)dstD->x - (float)dstD->w);
	}
	x1 = (float)dstD->x;
	y1 = (float)dstD->y;
	x2 = (float)dstD->w + (float)dstD->x;
	y2 = (float)dstD->y + (float)dstD->h;

	if (dstD->angle != 0.0) {
		rad = (dstD->angle / 360.0) * 6.283185;
		switch (dstD->center) {
		case 1:
			rx = x1;
			ry = y2;
			break;
		case 2:
			rx = (x1 + x2) / 2;
			ry = y2;
			break;
		case 3:
			rx = x2; 
			ry = y2;
			break;
		case 4:
			rx = x1;
			ry = (y1 + y2) / 2;
			break;
		default:
			rx = (x1 + x2) / 2;
			ry = (y1 + y2) / 2;
			break;
		case 6:
			rx = x2;
			ry = (y1 + y2) / 2;
			break;
		case 7:
			rx = x1;
			ry = y1;
			break;
		case 8:
			rx = (x1 + x2) / 2;
			ry = y1;
			break;
		case 9:
			rx = x2;
			ry = y1;
			break;
		}
		vec1.x = x1 - rx;
		vec1.y = y1 - ry;
		VectorRotationZ(&vec1, &vec1, rad);
		vec2.x = x2 - rx;
		vec2.y = y1 - ry;
		VectorRotationZ(&vec2, &vec2, rad);
		vec3.x = x2 - rx;
		vec3.y = y2 - ry;
		VectorRotationZ(&vec3, &vec3, rad);
		vec4.x = x1 - rx;
		vec4.y = y2 - ry;
		VectorRotationZ(&vec4, &vec4, rad);
		if (dstD->h != 0.0 && dstD->w != 0.0 && dstD->time > -1) {
			DrawModiGraphF((vec1.x + rx)  * skinsizeX, (vec1.y + ry)  * skinsizeY, (vec2.x + rx)  * skinsizeX, (vec2.y + ry)  * skinsizeY,
				(vec3.x + rx)  * skinsizeX, (vec3.y + ry)  * skinsizeY, (vec4.x + rx)  * skinsizeX, (vec4.y + ry)  * skinsizeY, *grHandle, 1);
		}
	}
	else {
		GetGraphSize(*grHandle, &xs, &ys);
		GetTimeWrap();
		if (dstD->flag) {
			DrawBox(x1*skinsizeX, y1*skinsizeY, x2*skinsizeX, y2*skinsizeY, GetColor(0, 0, 0), true);
		}
		DrawExtendGraphF(x1*skinsizeX, y1*skinsizeY, x2*skinsizeX, y2*skinsizeY, *grHandle, 1);
	}
	return 1;
}

//49b110
int GetTextGraphLength(CSTR *str, ImageFont *imF) {
	int pos;
	int ret;
	char ch;
	ushort vCh;
	int x, y;

	pos = 0;
	ret = 0;

	if (*(str->atPos(0)) == '\0') return 0;
	do {
		if (str->length() <= pos) return ret;
		ch = *str->atPos(pos);
		// 'ch < 0x81 || (0x9f < ch && (ch < 0xe0 || 0xfd < ch))' is replaced with !IsMultiByte()
		if (!IsMultibyte(ch)) {
			if (ch < 0)
				vCh = *str->atPos(pos) + 0x100;
			else
				vCh = *str->atPos(pos);
		}
		else {
			vCh = (*str->atPos(pos) << 8) + (uchar)*str->atPos(pos + 1);
			if (vCh > 0xFF) {
				if (vCh > 0x9ffd) vCh += 0xbfbf;
				vCh += 0x7fc0;
			}
		}
		if (vCh > 0x3bcd) vCh = 0x3f;
		
		if (imF->chars[vCh].grHandle == -1) LoadFontCharGraph(imF, vCh);
		GetGraphSize(imF->chars[vCh].grHandle, &x, &y);

		ch = *str->atPos(pos);
		if (!IsMultibyte(ch)) pos += 1;
		else pos += 2;
		ret += imF->kerning + x;
		if (ch == 0 || pos >= str->length()) ret -= imF->kerning;
	} while (*str->atPos(pos) != '\0');
	return ret;
}

//49b2a0 seems like ruined it
void LRDrawText(int* grHandle, DSTdraw *dstd, CSTR *str, ImageFont *imF) {
	double hl;
	float width, wl;
	int iDum;
	char ch;
	ushort vCh;
	int x, y;
	float xf, yf, wSum;

	if (imF->size < 1 || dstd->h == 0 || dstd->w == 0 || str->length() < 1) {		
		if (dstd->h != 0 && dstd->w != 0 && str->length() > 0 && *grHandle != -1) {
			SetDrawMode(dstd->filter);
			SetDrawBlendMode(dstd->blend, dstd->a);
			SetDrawBright(dstd->r, dstd->g, dstd->b);
			GetFontStateToHandle(0, &imF->size, &iDum, *grHandle);
			hl = dstd->h / (float)imF->size;
			width = GetDrawStringWidthToHandle(str->outstr(), str->length(), *grHandle, 0);
			if (width) {
				if (width > dstd->w) wl = dstd->w / width;
				else wl = 1.0;
			}
			wl = wl * hl;
			if (dstd->align == 1) {
				dstd->x = dstd->x - (width*wl*0.5);
			}
			else if (dstd->align == 2) {
				dstd->x = dstd->x - (width*wl);
			}
			GetTimeWrap();
			if (wl == 1.0 && hl == 1.0) {
				DrawStringToHandle(dstd->x, dstd->y, str->outstr(), GetColor(dstd->r, dstd->g, dstd->b), *grHandle, 0, 0);
			}
			else {
				DrawExtendStringToHandle(dstd->x, dstd->y, wl, hl, str->outstr(), GetColor(dstd->r, dstd->g, dstd->b), *grHandle, 0, 0);
			}
		}
	}
	else {
		SetDrawMode(dstd->filter);
		SetDrawBlendMode(dstd->blend, dstd->a);
		SetDrawBright(dstd->r, dstd->g, dstd->b);
		hl = dstd->h / imF->size;
		width = GetTextGraphLength(str, imF);

		if (width) {
			if (width > dstd->w) wl = dstd->w / width;
			else wl = 1.0;
		
			wl = wl * hl;
			if (dstd->align == 1) {
				dstd->x = dstd->x - (width*wl*0.5);
			}
			else if (dstd->align == 2) {
				dstd->x = dstd->x - (width*wl);
			}

			wSum = 0.0;
			for (int i = 0; *str->atPos(i) && i < str->length();) {
				ch = *str->atPos(i);
				// 'ch < 0x81 || (0x9f < ch && (ch < 0xe0 || 0xfd < ch))' is replaced with !IsMultiByte()
				if (IsMultibyte(ch)) {
					vCh = (*str->atPos(i) << 8) + (uchar)*str->atPos(i + 1);
					if (vCh > 0xFF) {
						if (vCh > 0x9ffd) vCh += 0xbfbf;
						vCh += 0x7fc0;
					}
				}
				else {
					vCh = *str->atPos(i);
				}
				if (vCh > 0x3bcd) vCh = 0x3f;

				if (imF->chars[vCh].grHandle == -1) LoadFontCharGraph(imF, vCh);
				GetGraphSize(imF->chars[vCh].grHandle, &x, &y);
				xf = x;
				yf = y;
				if (xf <= 0.0) {
					vCh = 0x3f;
					GetGraphSize(imF->chars[0x3f].grHandle, &x, &y);
					xf = x;
					yf = y;
				}
				xf *= wl;
				if (vCh != 0x20 && vCh != 0xa0 && imF->chars[vCh].grHandle != -1) {
					DrawExtendGraphF(dstd->x + wSum, dstd->y, dstd->x + wSum + xf, dstd->y + hl*yf, imF->chars[vCh].grHandle, 1);
				}
				ch = *str->atPos(i);
				if (IsMultibyte(ch)) i += 2; 
				else i += 1;
				wSum = imF->kerning*wl + xf + wSum;
			}
			return;
		}
	}
	return;
}

//49b7c0
void LRDrawTextInput(int* hFont, DSTdraw *dstd, int* hInput, ImageFont *imgfont) {
	IMEINPUTDATA* pIME;
	CSTR buf = CSTR(0x401);
	int grLen;
	int len1, len2;
	if (*hInput != -1) {
		GetKeyInputString(buf, *hInput);
		if (imgfont->size < 1) {
			if (dstd->h != 0.0 || dstd->w != 0.0 || *hFont != -1) {
				SetDrawMode(dstd->filter);
				SetDrawBlendMode(0, 0xff);
				SetDrawBright(dstd->r, dstd->g, dstd->b);
				SetKeyInputStringFont(*hFont);
				GetTimeWrap();
				DrawKeyInputString(dstd->x, dstd->y, *hInput);
			}
		}
		else if (dstd->h != 0.0 || dstd->w != 0.0) {
			pIME = (IMEINPUTDATA*)GetIMEInputData();
			dstd->w = 640.0;
			LRDrawText(hFont, dstd, &buf, imgfont);
			grLen = 0;
			if (GetKeyInputCursorPosition(*hInput)) {
				CSTR tCstr = CSTR(buf.left(GetKeyInputCursorPosition(*hInput)));
				grLen = GetTextGraphLength(&tCstr, imgfont);
			}
			if (pIME == NULL)	buf.fillzero();
			else				cstrSprintf(&buf, "%s", *pIME);
			dstd->x += grLen;
			if (buf.length() < 1) {
				DrawBox(dstd->x, dstd->y, dstd->x + 1.0, dstd->y + dstd->h, GetColor(0xff, 0xff, 0xff), 1);
			}
			else {
				for (int i = 0; pIME != 0 && i < pIME->ClauseNum; i++) {
					int pos1 = pIME->ClauseData[grLen].Position;
					int pos2 = pIME->ClauseData[grLen].Length + pos1;
					
					if (pos2 <= pos1) break;
					if (pos1) {
						CSTR tCstr = CSTR(buf.left(pos1));
						pos1 = GetTextGraphLength(&tCstr, imgfont);
					}
					len1 = pos1;
					CSTR tCstr = CSTR(buf.left(pos2));
					len2 = GetTextGraphLength(&tCstr, imgfont);
					DrawBox(dstd->x + len1 + 1.0, dstd->y, dstd->x + len2 - 1.0, dstd->h + dstd->y, (grLen == pIME->SelectClause) ? GetColor(255, 0, 0) : GetColor(64, 64, 64),1);
				}
				LRDrawText(hFont, dstd, &buf, imgfont);
			}
			GetIMEInputModeStr(buf);
			dstd->y = 480.0 - dstd->h;
			dstd->x = 640.0;
			dstd->align = 2;
			LRDrawText(hFont, dstd, &buf, imgfont);
		}
	}
}

//49bbf0
int AddDrawingBufferT(DrawingBuf *drb, int grHandle, DSTdraw *dstd){
	if ((drb->isDisabled == '\0') && (grHandle != -1)) {
		dstd->fontHandle = grHandle;
		dstd->grHandle = -1;
		if (drb->count < 2000) {
			memcpy(&drb->dstd[drb->count], dstd, sizeof(DSTdraw));
			drb->count = drb->count + 1;
			return 1;
		}
		return 0;
	}
	return 0;
}

//49bc50
int sortDST(const void *a, const void *b) {
	DSTdraw &aa = *(DSTdraw*)a;
	DSTdraw &bb = *(DSTdraw*)b;
	return aa.sortID - bb.sortID;
}

//49bc60
int LRDraw(DrawingBuf *drBuf, TextStruct *txt, SONGSELECT *sSel, skstruct *sks, int targetBufNum, int x, int y) {
	DSTdraw &draw = drBuf->dstd[targetBufNum];

	if (drBuf->isDisabled) return 0;
	if (targetBufNum == 0) qsort(drBuf->dstd, drBuf->count, sizeof(DSTdraw), sortDST);
	if (drBuf->count <= targetBufNum) return -1;
	
	skinsizeX = sks->adjust.rate_x / 100.0;
	skinsizeY = sks->adjust.rate_y / 100.0;

	draw.x += sks->adjust.shift_x + x;
	draw.y += sks->adjust.shift_y + y;
	if (draw.fontHandle == -1) {
		if (draw.grHandle != -1) {
			if (draw.subHandle > -1 && draw.subHandle < 20 && drBuf->isHidSud[draw.subHandle] > 0) {
				SetDrawArea(0, drBuf->top[draw.subHandle], 640, drBuf->bottom[draw.subHandle]);
			}
			LRDrawImg(&draw.grHandle, &draw);
			if (draw.subHandle > -1 && draw.subHandle < 20 && drBuf->isHidSud[draw.subHandle] > 0) {
				SetDrawArea(0, 0, 640, 480);
			}
			return 1;
		}
	}
	else if (draw.grHandle != -1) return 1;
	//if (draw.subHandle == -1) return 1; //DEBUG : this line is not in original
	if (draw.fontHandle != -1) {
		if (draw.subHandle < 10000) {
			if (draw.subHandle < 1000) {
				if (txt->st_text_num == draw.subHandle) {
					LRDrawTextInput(&draw.fontHandle, &draw, &txt->hKeyInput, &sks->ImageFonts[(int)draw.angle]);
				}
				else {
					CSTR a = GetStringFromArray(draw.subHandle, txt->objectStr);
					LRDrawText(&draw.fontHandle, &draw, &a, &sks->ImageFonts[(int)draw.angle]);
				}
			}
			else {
				LRDrawText(&draw.fontHandle, &draw, &txt->readme.body[draw.subHandle - 1000], &sks->ImageFonts[(int)draw.angle]);
			}
		}
		else {
			draw.angle;
			if (sSel->filter.disablesubtitle == 1) {
				LRDrawText(&draw.fontHandle, &draw, &sSel->bmsList[draw.subHandle - 10000].title, &sks->ImageFonts[(int)draw.angle]);
			}
			else {
				LRDrawText(&draw.fontHandle, &draw, &sSel->bmsList[draw.subHandle - 10000].fulltitle, &sks->ImageFonts[(int)draw.angle]);
			}
		}
		
	}

	return 1;
}

//49bf60
int InitDrawingBuffer(DrawingBuf *drb){
	for (int i = 0; i < drb->count; i++) {
		InitDSTdraw(&drb->dstd[i]);
	}
	drb->count = 0;
	for (int i = 0; i < 20; i++) {
		drb->top[i] = -1;
		drb->bottom[i] = -1;
		drb->isHidSud[i] = -1;
	}
	return 1;
}

//49bfc0
int MouseOnDSTD(DSTdraw *dstd, int *x, int *y){ //1 right 2 left
	float fxv;
	float half;
	int xv;
	int yv;

	xv = *x;
	yv = *y;
	fxv = (float)xv;
	if ( xv < 640 && -1 < xv && yv < 480 && -1 < yv && (float)dstd->x <= fxv && fxv <= (float)dstd->w + (float)dstd->x
	&& (float)dstd->y <= (float)yv && (float)yv <= (float)dstd->h + (float)dstd->y) {
		
		half = (float)dstd->w * 0.5 + (float)dstd->x;
		
		if (fxv >= half) return 1;
		else return 2;
	}
	return 0;
}

//49c070
int MouseOnObject(DSTstruct *dst, Timer *T, int *x, int *y){
	DSTdraw _dstd;
	DSTdraw local_a0;
	double time;
	DSTstruct _dst;

	time = GetTimeLapse(dst->timer, T);
	memcpy(&_dst, dst, sizeof(DSTstruct));
	_dstd = SetDSTdrawByTime(_dst, time);
	return MouseOnDSTD(&_dstd, x, y);
}

//49c0e0 maybe done
int SliderByTime(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int min, int max, int *value, inputStructure *input, int objectID) {
	DSTdraw dstdTemp;
	DSTstruct dstsTemp;
	int newVal, newX=0, newY=0;
	int moveX, moveY;

	if (dst->dstCount < 1) return 0;
	if (dst->dataSize < 1) return 0;
	if (src->op2 < 1) return 0; //op2:range
	if (GetTimeLapse(dst->timer, T) == -1) return 0;
	
	memcpy(&dstsTemp, dst, sizeof(DSTstruct));
	dstdTemp = SetDSTdrawByTime(dstsTemp,GetTimeLapse(dst->timer,T));

	newX = 0;
	newY = 0;
	newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
	switch (src->op1) { //op1:muki(direction) 0up 1right 2down 3left
		case 0:
			dstdTemp.y = dstdTemp.y - src->op2;
			dstdTemp.h = src->op2 + dstdTemp.h;
			break;
		case 1:
			dstdTemp.w = src->op2 + dstdTemp.w;
			break;
		case 2:
			dstdTemp.h = src->op2 + dstdTemp.h;
			break;
		case 3:
			dstdTemp.w = src->op2 + dstdTemp.w;
			dstdTemp.x = dstdTemp.x - src->op2;
			break;
		default:
			return 0;
	}

	if ( ((objectID == input->is_drag_now && input->mouse_moveflag == 1) || (MouseOnDSTD(&dstdTemp, &input->mouse_oldX, &input->mouse_oldY) != 0 && input->mouse_buttonL == 1)) && src->op4 == 0) { //op4:disable
		moveX = (dstdTemp.w - src->op2) / 2;
		moveY = (dstdTemp.h - src->op2) / 2;
		input->is_drag_now = objectID;
		switch (src->op1) {
			case 0:
				*value = ChangeValueByTime(max, min, dstdTemp.y + moveY, dstdTemp.y + dstdTemp.h - moveY, input->mouse_oldY, 0);
				newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
				break;
			case 1:
				*value = ChangeValueByTime(min, max, dstdTemp.x + moveX, dstdTemp.x + dstdTemp.w - moveX, input->mouse_oldX, 0);
				newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
				break;
			case 2:
				*value = ChangeValueByTime(min, max, dstdTemp.y + moveY, dstdTemp.y + dstdTemp.h - moveY, input->mouse_oldY, 0);
				newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
				break;
			case 3:
				*value = ChangeValueByTime(max, min, dstdTemp.x + moveX, dstdTemp.x + dstdTemp.w - moveX, input->mouse_oldX, 0);
				newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
				break;
			default:
				break;
		}
	}

	switch (src->op1) {
		case 0:
			newY = -newVal;
			break;
		case 1:
			newX = newVal;
			break;
		case 2:
			newY = newVal;
			break;
		case 3:
			newX = -newVal;
			break;
		default:
			return 0;
	}
	//old code
	//switch (src->op1) {
	//	case 0:
	//		newVal = -newVal;
	//	case 2:
	//		newY = newVal;
	//		newVal = newX;
	//		break;
	//	case 1:
	//		break;
	//	case 3:
	//		newVal = -newVal;
	//		break;
	//	default:
	//		return 0;
	//}
	//newX = newVal;
	src->sy = newY;
	src->sx = newX;
	return 1;
}

//49c560
int ButtonByInput(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, inputStructure *input, int *target, int min, int max, int panel) { //return 1:just clicked 2:changed 0:not changed
	DSTdraw dstd;
	int mouse, ret;
	
	if ((dst->dstCount < 1) || (dst->dataSize < 1)) return 0;
	if (GetTimeLapse(dst->timer, T) == -1.0) return 0;

	ret = 0;
	dstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	mouse = MouseOnDSTD(&dstd, &input->mouse_oldX, &input->mouse_oldY);
	if ( (mouse == 1 && src->op4 == 0) || (mouse != 0 && src->op4 == 1) ) { // right side or plusonly
		if (src->op2 != 1 || input->mouse_buttonL != 1 || (src->op3 != panel && src->op3 != 0)) {
			ret = 1;
	}
		else {
			if (min < max) (*target)++;
			ret = 2;
		}
	}
	else if ( (mouse == 2 && src->op4 == 0) || (mouse != 0 && src->op4 == 2) ) { // left side or plusonly(-)
		if (src->op2 != 1 || input->mouse_buttonL != 1 || (src->op3 != panel && src->op3 != 0)) {
			ret = 1;
		}
		else {
			if (min < max) (*target)--;
			ret = 2;
		}
	}

	if (max < *target) *target = min;
	else if (min > *target) *target = max;

	src->inArray = *target;
	if (src->graphcount <= *target) src->inArray = src->graphcount - 1;
	if (*target < 0) src->inArray = 0;
	if (min < 0) src->inArray = 0;

	return ret;
}

//49c700
int DrawBGA(int grHandle){
	int TransFlag;
	int grW;
	int grH;
	float tgX;
	float tgW;
	float tgH;

	GetGraphSize(grHandle, &grW, &grH);
	tgX = 0.0;
	tgW = 256.0;
	tgH = 256.0;
	if (grW < 256) {
		tgX = (256.0 - (float)grW) * 0.5;;
		tgW = (float)grW * 256.0 / 256.0;
	}
	if (grH < 256) {
		tgH = (float)grH * 256.0 / 256.0;
	}
	TransFlag = 1;
	DrawExtendGraph(tgX, 0, tgX + tgW, tgH + 0.0, grHandle, TransFlag);
	return 1;
}

//49c7d0
int AllocDrawingBuffer(DrawingBuf *drb){
	drb->max = 2000;
	drb->count = 0;
	for (int i = 0; i < 20; i++) {
		drb->isHidSud[i] = -1;
		drb->top[i] = -1;
		drb->bottom[i] = -1;
	}
	drb->dstd = (DSTdraw *)malloc(drb->max * 0x50);
	if (drb->dstd == (DSTdraw *)0x0) {
		ErrorLogAdd("描画用バッファのメモリ取得に失敗しました。\n");
		return -1;
	}
	return 1;
}

//49c840
int ReallocDrawingBuffer(DrawingBuf *drb){
	drb->max = drb->max + 1000;
	drb->dstd = (DSTdraw *)realloc(drb->dstd, drb->max * 0x50);
	if (drb->dstd == (DSTdraw *)0x0) {
		ErrorLogAdd("描画用バッファのメモリ再取得に失敗しました。\n");
		return -1;
	}
	return 1;
}

//for convenience
double ByTime(double v1, double v2, double t1, double t2, double tO) {
	double ret, temp;

	ret = v1;
	if (v2 != v1) {
		if (t2 < tO || tO < t1 || t2 <= t1) {
			ret = v2;
			if (tO <= t1) {
				ret = v1;
			}
		}
		else {
			temp = (tO - t1) / (t2 - t1);
			ret = (1 - temp) * v1 + temp * v2;
		}
	}
	return ret;
}

//49c880
DSTdraw DSTDbyTime(DSTdraw *dstd1, DSTdraw *dstd2, double t1, double t2, double tO) {
	DSTdraw ret;
	
	ret.x = ByTime(dstd1->x, dstd2->x, t1, t2, tO);
	ret.y = ByTime(dstd1->y, dstd2->y, t1, t2, tO);
	ret.w = ByTime(dstd1->w, dstd2->w, t1, t2, tO);
	ret.h = ByTime(dstd1->h, dstd2->h, t1, t2, tO);
	ret.sortID = ByTime(dstd1->sortID, dstd2->sortID, t1, t2, tO);
	ret.a = ByTime(dstd1->a, dstd2->a, t1, t2, tO);
	ret.r = ByTime(dstd1->r, dstd2->r, t1, t2, tO);
	ret.g = ByTime(dstd1->g, dstd2->g, t1, t2, tO);
	ret.b = ByTime(dstd1->b, dstd2->b, t1, t2, tO);
	ret.angle = ByTime(dstd1->angle, dstd2->angle, t1, t2, tO);

	ret.blend = dstd1->blend;
	ret.filter = dstd1->filter;
	ret.time = 0;
	ret.center = dstd1->center;

	return ret;
}

//49cd10
int AddDrawingBuffer_Text(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T) {
	DSTdraw tDstd;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.fontHandle = src->fontHandle;
	if (tDstd.fontHandle == -1) return 0;
	tDstd.angle = src->cycle;
	tDstd.subHandle = src->st;
	tDstd.align = src->align;
	if (tDstd.time != -1) AddDrawingBufferT(drb, tDstd.fontHandle, &tDstd);
	return 1;
}

//49cdd0
int AddDrawingBuffer_TextXY(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int handle, int x, int y) {
	DSTdraw tDstd;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.fontHandle = src->fontHandle;
	if (tDstd.fontHandle == -1) return 0;
	tDstd.angle = src->cycle;
	tDstd.subHandle = handle;
	tDstd.align = src->align;
	tDstd.x += x;
	tDstd.y += y;
	if (tDstd.time != -1) AddDrawingBufferT(drb, tDstd.fontHandle, &tDstd);
	return 1;
}

//49ceb0
int AddDrawingBuffer(DrawingBuf *drb, int grHandle, DSTdraw *dstd){
	if ((drb->isDisabled == '\0') && (grHandle != -1)) {
		dstd->grHandle = grHandle;
		dstd->fontHandle = -1;
		if (drb->count == drb->max) {
			ReallocDrawingBuffer(drb);
		}
		memcpy(&drb->dstd[drb->count], dstd, sizeof(DSTdraw));
		drb->count = drb->count + 1;
		return 1;
	}
	return 0;
}

//49cf10
int AddDrawingBuffer_BarGraph(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, double min, double max, double val) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0 || src->graphcount <= 0) return 0;

	grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (min == max || val <= min) return -1;
	if (val >= max) val = max;
	val = val / (max - min);
	if (src->op2 == 0) tDstd.w *= val;
	else if (src->op2 == 1) tDstd.h *= val;
	if (tDstd.time != -1 && grh != -1) {
		AddDrawingBuffer(drb, grh, &tDstd);
	}
	return 0;
}

//49d040
int AddDrawingBuffer_Image(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T) {
	DSTdraw tDstd;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (tDstd.time != -1 && src->inArray < src->graphcount && src->inArray > -1) {
		AddDrawingBuffer(drb, src->grHandles[src->inArray], &tDstd);
	}

	return 1;
}

//49d0f0
int AddDrawingBuffer_OnMouse(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, inputStructure *input, int panel) {
	DSTdraw tDstd;
	int grh;
	
	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	//grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	grh = GetSRCcycleNow(*src, GetTimeLapse(src->timer, T));
	tDstd.x += src->op2;
	tDstd.y += src->op3;
	tDstd.w = src->op4;
	tDstd.h = src->op5;

	if (MouseOnDSTD(&tDstd, &input->mouse_oldX, &input->mouse_oldY) && (src->op1 == 0 || src->op1 == panel)) {
		tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
		if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, src->grHandles[grh], &tDstd);
	}
	return 0;
}

//49d240
int AddDrawingBuffer_LN(DrawingBuf *drb, SRCstruct *srcLs, SRCstruct *srcLe, SRCstruct *srcLb, DSTstruct *dst, Timer *T, float shiftX, float shiftY, float longY, int alpha, float sizeX, float sizeY) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0 || srcLs->graphcount <= 0 || srcLe->graphcount <= 0 || srcLb->graphcount <= 0) return 0;

	//body
	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.w += sizeX;
	tDstd.h += sizeY;
	tDstd.x -= sizeX * 0.5;
	tDstd.y -= sizeY * 0.5;
	if (srcLb->timer == dst->timer) {
		grh = srcLb->grHandles[GetSRCcycleNow(*srcLb, GetTimeLapse(srcLb->timer, T) - dst->draw->time)];
	}
	else {
		grh = srcLb->grHandles[GetSRCcycleNow(*srcLb, GetTimeLapse(srcLb->timer, T))];
	}
	tDstd.x += shiftX;
	tDstd.subHandle = dst->n;
	tDstd.blend = 1;
	tDstd.a = alpha;
	tDstd.y += longY + tDstd.h;
	tDstd.h = shiftY - longY - tDstd.h;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	//start
	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.w += sizeX;
	tDstd.h += sizeY;
	tDstd.x -= sizeX * 0.5;
	tDstd.y -= sizeY * 0.5;
	tDstd.sortID += 2;
	if (srcLs->timer == dst->timer) {
		grh = srcLs->grHandles[GetSRCcycleNow(*srcLs, GetTimeLapse(srcLs->timer, T) - dst->draw->time)];
	}
	else {
		grh = srcLs->grHandles[GetSRCcycleNow(*srcLs, GetTimeLapse(srcLs->timer, T))];
	}
	tDstd.x += shiftX;
	tDstd.subHandle = dst->n;
	tDstd.blend = 1;
	tDstd.a = alpha;
	tDstd.y += shiftY;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	//end
	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.w += sizeX;
	tDstd.h += sizeY;
	tDstd.x -= sizeX * 0.5;
	tDstd.y -= sizeY * 0.5;
	tDstd.sortID += 1;
	if (srcLe->timer == dst->timer) {
		grh = srcLe->grHandles[GetSRCcycleNow(*srcLe, GetTimeLapse(srcLe->timer, T) - dst->draw->time)];
	}
	else {
		grh = srcLe->grHandles[GetSRCcycleNow(*srcLe, GetTimeLapse(srcLe->timer, T))];
	}
	tDstd.x += shiftX;
	tDstd.subHandle = dst->n;
	tDstd.blend = 1;
	tDstd.a = alpha;
	tDstd.y += longY;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

//49d630
int AddDrawingBuffer_PlayArea(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, float shiftX, float shiftY, int alpha, float sizeX, float sizeY, char flag) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.w += sizeX;
	tDstd.h += sizeY;
	tDstd.x -= sizeX * 0.5;
	tDstd.y -= sizeY * 0.5;
	if (src->graphcount <= 0) return 0;
	
	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	tDstd.x += shiftX;
	tDstd.y += shiftY;
	tDstd.filter = 0;
	tDstd.blend = 1;
	if (alpha > -1) tDstd.a = alpha;
	tDstd.subHandle = (flag == 0) ? -1 : dst->n;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

//49d7c0
int AddDrawingBuffer_Gauge(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int hp, char isSurvival) {
	DSTdraw tDstd;
	int grh,tremble,cyc=0;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;
	
	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (src->graphcount > 4 && src->cycle > 0) {
		cyc = (src->graphcount/4) * ((int)GetTimeLapse(src->timer, T) % src->cycle) / src->cycle * 4;
	}

	tremble = hp - GetRand(2);
	for (int i = 1; i <= 50; i++) {
		if (isSurvival == 0) {
			if (1 <= i && i < 40) {
				if ((i < tremble || hp <= i || i == 1) && i <= hp)
					grh = src->grHandles[cyc + 1];
				else
					grh = src->grHandles[cyc + 3];
			}
			else {
				if (tremble <= i && i < hp && i != 1)
					grh = src->grHandles[cyc + 2];
				else if(hp < i)
					grh = src->grHandles[cyc + 2];
				else
					grh = src->grHandles[cyc];
			}
		}
		else if (i < tremble || hp <= i || i == 1) {
			if(hp < i)
				grh = src->grHandles[cyc + 2];
			else
				grh = src->grHandles[cyc];
		}
		else grh = src->grHandles[cyc + 2];

		if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);
		tDstd.x += src->op1;
		tDstd.y += src->op2;
	}
	return 1;
}

//49d990
int AddDrawingBuffer_BGA(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int grHandle, char flag) {
	DSTdraw tDstd;
	int x, y;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	
	GetGraphSize(grHandle, &x, &y);
	if (x < 256){
		tDstd.x += tDstd.w / 256.0 * (256.0 - x) * 0.5;
		tDstd.w *= x / 256.0;
	}
	if (y < 256) {
		tDstd.h *= y / 256.0;
	}
	if (flag == 0) tDstd.sortID++;
	
	tDstd.flag = flag;

	if (tDstd.time != -1 && grHandle != -1) AddDrawingBuffer(drb, grHandle, &tDstd);

	return 1;
}

//49dad0
int AddDrawingBuffer_Scratch(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int angle) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (src->graphcount <= 0) return 0;

	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	tDstd.angle = angle;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

//49dbe0
int AddDrawingBuffer_Image2(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (src->graphcount <= 0) return 0;

	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

//49dce0
int AddDrawingBuffer_Object(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int x, int y) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.x += x;
	tDstd.y += y;

	if (src->graphcount <= 0) return 0;

	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

//49de00
int AddDrawingBuffer_ObjectAlpha(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int x, int y, int a) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (src->graphcount <= 0) return 0;

	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	tDstd.x += x;
	tDstd.y += y;
	tDstd.blend = 1;
	tDstd.a = a;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

//49df30
int AddDrawingBuffer_EventLoading(DrawingBuf *drb, int grHandle, DSTstruct *dst, Timer *T, int x, int y) {
	DSTdraw tDstd;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	tDstd.x += x;
	tDstd.y += y;
	if (tDstd.time != -1 && grHandle != -1) AddDrawingBuffer(drb, grHandle, &tDstd);

	return 1;
}

//49dff0
int AddDrawingBuffer_Lunaris(DrawingBuf *drb, SRCstruct *src, DSTdraw *dstd, Timer *T) {
	int grh;

	if (src->graphcount < 1) return 0;

	grh = GetSRCcycleNow(*src, GetTimeLapse(src->timer, T));
	if (dstd->time != -1 && src->grHandles[grh] != -1) AddDrawingBuffer(drb, src->grHandles[grh], dstd);

	return 1;
}

//49e060
int AddDrawingBuffer_Numbers(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int number, int x, int y) {
	DSTdraw tDstd;
	int grh,cyc,grct;
	CSTR str;

	if (dst->dstCount <= 0 || dst->dataSize <= 0 || src->op3 <= 0 || src->op2 <= -1 || src->op2 >= 3) return 0;

	cyc = 0;
	grct = 0;
	if (src->graphcount % 10 == 0){
		grct = 1;
		if(src->cycle > 0) cyc = (src->graphcount / 10) * ((int)GetTimeLapse(src->timer, T) % src->cycle) / src->cycle * 10;
	}
	if (src->graphcount % 11 == 0) {
		grct = 2;
		if (src->cycle > 0) cyc = (src->graphcount / 11) * ((int)GetTimeLapse(src->timer, T) % src->cycle) / src->cycle * 11;
	}
	if (src->graphcount % 24 == 0) {
		grct = 3;
		if (src->cycle > 0) cyc = (src->graphcount / 24) * ((int)GetTimeLapse(src->timer, T) % src->cycle) / src->cycle * 24;
	}
	if (grct == 0) return 0;
	if (grct != 3 && number < 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	if (tDstd.time == -1) return 0;

	tDstd.x += x;
	tDstd.y += y;
	//op2 : align
	if (src->op2 == 0) {
		cstrSprintf(&str, "%10d", abs(number));
		str.lastCut(src->op3);
	}
	else if (src->op2 == 1) {
		cstrSprintf(&str, "%d          ", abs(number));
		str.nullAtPos(src->op3);
	}
	else if (src->op2 == 2) {
		int blanks = src->op3 - CountDigit(number);
		if (blanks < 0)	blanks = 0;
		tDstd.x += blanks * tDstd.w * 0.5;

		cstrSprintf(&str, "%d          ", abs(number));
		str.nullAtPos(src->op3);
	}

	if (grct == 3) {
		if (number < 0) grh = src->grHandles[cyc + 23];
		else grh = src->grHandles[cyc + 11];
		AddDrawingBuffer(drb, grh, &tDstd);
		tDstd.x += tDstd.w;
	}
	if (number < 0 && src->graphcount == 24) cyc += 12;

	for (int i = 0; i < str.length(); i++) {
		switch (*str.atPos(i)) {
			case ' ':
				if (grct != 1) {
					grh = src->grHandles[cyc + 10];
					AddDrawingBuffer(drb, grh, &tDstd);
				}
				break;
			case '0':
				grh = src->grHandles[cyc + 0];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '1':
				grh = src->grHandles[cyc + 1];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '2':
				grh = src->grHandles[cyc + 2];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '3':
				grh = src->grHandles[cyc + 3];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '4':
				grh = src->grHandles[cyc + 4];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '5':
				grh = src->grHandles[cyc + 5];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '6':
				grh = src->grHandles[cyc + 6];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '7':
				grh = src->grHandles[cyc + 7];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '8':
				grh = src->grHandles[cyc + 8];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '9':
				grh = src->grHandles[cyc + 9];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			default:
				break;
		}
		tDstd.x += tDstd.w;
	}
	return 1;
}

//49e4d0
int AddDrawingBuffer_Slider(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T) {
	DSTdraw tDstd;

	if (src->op2 < 1) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	AddDrawingBuffer_Object(drb, src, dst, T, src->sx, src->sy);

	return 1;
}

//49e550
int AddDrawingBuffer_JudgeCombo(DrawingBuf *drb, SRCstruct *jSrc, DSTstruct *jDst, SRCstruct *cSrc, DSTstruct *cDst, Timer *T, int combo, int optX, int optY) {
	DSTdraw tDstd;// , tDstd2;
	int grh;
	int digit;
	float pos, posn;

	if (jDst->dstCount <= 0 || jDst->dataSize <= 0) return 0;

	digit = 0;
	if (combo > 0) {
		for (int i = combo; i > 9; i /= 10) {
			digit++;
		}
	}

	//op1 : noshift
	pos = 0.0;
	posn = 0.0;
	if (jSrc->op1 == 0 && combo > 0) {
		tDstd = SetDSTdrawByTime(*cDst, GetTimeLapse(jDst->timer, T));
		pos = -(digit * tDstd.w * 0.5);
	}
	else {
		tDstd = SetDSTdrawByTime(*cDst, GetTimeLapse(jDst->timer, T));
		posn = -( (digit-1) * tDstd.w * 0.5);
	}

	AddDrawingBuffer_Object(drb, jSrc, jDst, T, optX + pos, optY);

	if (combo > 0) {
		tDstd = SetDSTdrawByTime(*jDst, GetTimeLapse(jDst->timer, T));
		//tDstd2 = SetDSTdrawByTime(*jDst, GetTimeLapse(jDst->timer, T));
		AddDrawingBuffer_Numbers(drb, cSrc, cDst, T, combo, optX + pos + posn + tDstd.x, optY + tDstd.y);
	}
	return 1;
}

////LR2graphic_load
//49e780
int ReloadImage(CSTR filename, int *grHandle) {
	return ReloadGraph(filename, *grHandle, 0);
}

//49e7f0
int InitSRC(SRCstruct *src){
	int i = 0;
	while (i < src->graphcount) {
		DeleteGraph(src->grHandles[i]);
		i++;
	}
	if (src->count == 0) {
		src->count = 1;
		src->grHandles = (int*)malloc(sizeof(int));
	}
	memset(src->grHandles, '\0', src->count * sizeof(int));
	src->cycle = 0;
	src->graphcount = 0;
	src->n = 0;
	src->timer = 0;
	src->align = 0;
	src->st = 0;
	src->op1 = 0;
	src->op2 = 0;
	src->op3 = 0;
	src->op4 = 0;
	src->op5 = 0;
	src->sx = 0;
	src->sy = 0;
	src->inArray = 0;
	src->fontHandle = -1;
	return 1;
}

//maybe deleted by compiler, restored it for convenience
int InitDST(DSTstruct *dst) {
	dst->n = 0;
	dst->loop = 0;
	dst->opt1 = 0;
	dst->opt2 = 0;
	dst->opt3 = 0;
	dst->opt4 = 0;
	dst->opt5 = 0;
	dst->dstCount = 0;
	dst->dataSize = 0;
	return 1;
}

//49e890
int InitDSTdraw(DSTdraw *dstd){
	dstd->x = 0;
	dstd->y = 0;
	dstd->w = 0;
	dstd->sortID = 0;
	dstd->h = 0;
	dstd->a = 0;
	dstd->angle = 0.0;
	dstd->r = 0;
	dstd->g = 0;
	dstd->b = 0;
	dstd->acc = 0;
	dstd->blend = 0;
	dstd->filter = 0;
	dstd->time = -1;
	dstd->center = 0;
	dstd->grHandle = -1;
	dstd->fontHandle = -1;
	dstd->subHandle = -1;
	dstd->align = 0;
	dstd->flag = '\0';
	return 1;
}

//49e8e0
int ReadDST(DSTstruct *dst, CSVbuf *csv, int order){
	int iVar1;
	DSTdraw *pDVar2;
	int iVar3;

	//parameters only for first line
	if (dst->dstCount == 0) {
		dst->n = csv->val[1];
		dst->loop = csv->val[0x10];
		dst->timer = csv->val[0x11];
		dst->opt1 = csv->val[0x12];
		dst->opt2 = csv->val[0x13];
		dst->opt3 = csv->val[0x14];
		dst->opt4 = csv->val[0x15];
		dst->opt5 = csv->val[0x16];
	}

	//memory allocation if needed
	if (dst->dataSize == dst->dstCount) {
		dst->dataSize = dst->dataSize + 10;
		dst->draw = (DSTdraw*)realloc(dst->draw, dst->dataSize * sizeof(DSTdraw));
		for (int i = dst->dstCount; i < dst->dataSize; i++) {
			InitDSTdraw(&dst->draw[i]);
		}
	}

	//read dst
	dst->draw[dst->dstCount].time = csv->val[2];
	dst->draw[dst->dstCount].x = (int)(float)csv->val[3];
	dst->draw[dst->dstCount].y = (int)(float)csv->val[4];
	dst->draw[dst->dstCount].w = (int)(float)csv->val[5];
	dst->draw[dst->dstCount].h = (int)(float)csv->val[6];
	dst->draw[dst->dstCount].acc = csv->val[7];
	dst->draw[dst->dstCount].a = csv->val[8];
	dst->draw[dst->dstCount].r = csv->val[9];
	dst->draw[dst->dstCount].g = csv->val[10];
	dst->draw[dst->dstCount].b = csv->val[0xb];
	dst->draw[dst->dstCount].blend = csv->val[0xc];
	dst->draw[dst->dstCount].filter = csv->val[0xd];
	dst->draw[dst->dstCount].angle = (float)csv->val[0xe];
	dst->draw[dst->dstCount].center = csv->val[0xf];
	dst->draw[dst->dstCount].sortID = order;
	dst->draw[dst->dstCount].subHandle = -1;
	dst->dstCount = dst->dstCount + 1;
	return 1;
}

//49eae0
int ReadSRC(SRCstruct *src, CSVbuf *csv, skstruct *sk){
	int div_count;

	src->n = csv->val[1];
	src->cycle = csv->val[9];
	src->timer = csv->val[10];
	src->op1 = csv->val[11];
	src->op2 = csv->val[12];
	src->op3 = csv->val[13];
	src->op4 = csv->val[14];
	src->op5 = csv->val[15];
	if (csv->val[5] < 0) GetGraphSize(sk->GrHandle[csv->val[2]], &csv->val[5], NULL); //w
	if (csv->val[6] < 0) GetGraphSize(sk->GrHandle[csv->val[2]], NULL, &csv->val[6]); //h
	if (csv->val[3] < 1) csv->val[3] = 0; //x
	if (csv->val[4] < 1) csv->val[4] = 0; //y
	if (csv->val[7] < 1) csv->val[7] = 1; //div_x
	if (csv->val[8] < 1) csv->val[8] = 1; //div_y
	src->graphcount = csv->val[8] * csv->val[7];
	if (src->graphcount < 1) src->graphcount = 1;
	if (src->count < src->graphcount) {
		src->count = src->graphcount;
		src->grHandles = (int*)realloc(src->grHandles, src->graphcount * sizeof(int));
	}
	for (int i = 0; i < src->count; i++) {
		src->grHandles[i] = -1;
	}

	for (int i = 0; i < csv->val[7]; i++) {
		for (int j = 0; j < csv->val[8]; j++) {
			if (sk->grIsMovie[csv->val[2]] == 0) {
				int blockY = csv->val[6] / csv->val[8];
				int blockX = csv->val[5] / csv->val[7];
				src->grHandles[i + csv->val[7]*j] = DerivationGraph(blockX*i + csv->val[3], blockY*j + csv->val[4], blockX, blockY, sk->GrHandle[csv->val[2]]);
			}
			else {
				src->grHandles[i + csv->val[7]*j] =	sk->GrHandle[csv->val[2]];
			}
		}
	}
	return 1;
}

//49ec90
bool CheckIndexRange(int index, int min, int max, int line, char *str){
	if ((min <= index) && (index <= max)) {
		return true;
	}
	ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nインデックス範囲外です(%d-%d)。\n", line, str, min, max);
	return false;
}

//49ecd0
int ReadSRC_BAR_TITLE(SRCstruct *src, CSVbuf *csv, skstruct *sk){
	src->n = csv->val[1];
	src->fontHandle = sk->fontHandle[csv->val[2]];
	src->cycle = csv->val[2];
	src->st = csv->val[3];
	src->align = csv->val[4];
	src->op1 = csv->val[5];
	src->op2 = csv->val[6];
	src->op3 = csv->val[7];
	src->op4 = csv->val[8];
	src->op5 = csv->val[9];
	return 1;
}

//49ed30
int ScreenCapture(uint iGrHandle){
	GetDrawScreenGraph(0, 0, 640, 480, iGrHandle, 1);
	return 0;
}

//49ed50 InitSkin
int InitSkin(skstruct *sk, int p5, char font) {
	SetTransColor(0, 255, 0);
	sk->startinput_start = 0;
	sk->startinput_rank = 0;
	sk->startinput_update = 0;
	sk->scenetime = 0;
	sk->loadstart = 0;
	sk->loadend = 0;
	sk->playstart = 2000;
	sk->fadeout = 0;
	sk->close = 0;
	sk->horizontal = 0;

	for (int i = 0; i < 1000; i++) {
		InitDSTdraw(&sk->drBuf.dstd[i]);
	}
	(sk->drBuf).count = 0;

	(sk->image).srcSize = 0;
	if ((sk->image).dstSize == 0) {
		(sk->image).dstSize = 200;
		(sk->image).src = (SRCstruct *)malloc(200*sizeof(SRCstruct));
		memset(sk->image.src, '\0', (sk->image).dstSize * sizeof(SRCstruct));
		(sk->image).dst = (DSTstruct *)malloc((sk->image).dstSize * sizeof(DSTstruct));
		memset(sk->image.dst, '\0', (sk->image).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->image.dstSize; i++) {
		InitSRC(&sk->image.src[i]);
	}
	for (int i = 0; i < sk->image.dstSize; i++) {
		InitDST(&sk->image.dst[i]);
	}

	(sk->otherObject[0]).srcSize = 0;
	if ((sk->otherObject[0]).dstSize == 0) {
		(sk->otherObject[0]).dstSize = 20;
		(sk->otherObject[0]).src = (SRCstruct *)malloc(20 * sizeof(SRCstruct));
		memset(sk->otherObject[0].src, '\0', (sk->otherObject[0]).dstSize * sizeof(SRCstruct));
		(sk->otherObject[0]).dst = (DSTstruct *)malloc((sk->otherObject[0]).dstSize * sizeof(DSTstruct));
		memset(sk->otherObject[0].dst, '\0', (sk->otherObject[0]).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->otherObject[0].dstSize; i++) {
		InitSRC(&sk->otherObject[0].src[i]);
	}
	for (int i = 0; i < sk->otherObject[0].dstSize; i++) {
		InitDST(&sk->otherObject[0].dst[i]);		
	}

	(sk->otherObject[2]).srcSize = 0;
	if ((sk->otherObject[2]).dstSize == 0) {
		(sk->otherObject[2]).dstSize = 20;
		(sk->otherObject[2]).src = (SRCstruct *)malloc(20 * sizeof(SRCstruct));
		memset(sk->otherObject[2].src, '\0', (sk->otherObject[2]).dstSize * sizeof(SRCstruct));
		(sk->otherObject[2]).dst = (DSTstruct *)malloc((sk->otherObject[2]).dstSize * sizeof(DSTstruct));
		memset(sk->otherObject[2].dst, '\0', (sk->otherObject[2]).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->otherObject[2].dstSize; i++) {
		InitSRC(&sk->otherObject[2].src[i]);
	}
	for (int i = 0; i < sk->otherObject[2].dstSize; i++) {
		InitDST(&sk->otherObject[2].dst[i]);
	}

	(sk->otherObject[1]).srcSize = 0;
	if ((sk->otherObject[1]).dstSize == 0) {
		(sk->otherObject[1]).dstSize = 20;
		(sk->otherObject[1]).src = (SRCstruct *)malloc(20 * sizeof(SRCstruct));
		memset(sk->otherObject[1].src, '\0', (sk->otherObject[1]).dstSize * sizeof(SRCstruct));
		(sk->otherObject[1]).dst = (DSTstruct *)malloc((sk->otherObject[1]).dstSize * sizeof(DSTstruct));
		memset(sk->otherObject[1].dst, '\0', (sk->otherObject[1]).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->otherObject[1].dstSize; i++) {
		InitSRC(&sk->otherObject[1].src[i]);
	}
	for (int i = 0; i < sk->otherObject[1].dstSize; i++) {
		InitDST(&sk->otherObject[1].dst[i]);
	}

	(sk->otherObject[3]).srcSize = 0;
	if ((sk->otherObject[3]).dstSize == 0) {
		(sk->otherObject[3]).dstSize = 20;
		(sk->otherObject[3]).src = (SRCstruct *)malloc(20 * sizeof(SRCstruct));
		memset(sk->otherObject[3].src, '\0', (sk->otherObject[3]).dstSize * sizeof(SRCstruct));
		(sk->otherObject[3]).dst = (DSTstruct *)malloc((sk->otherObject[3]).dstSize * sizeof(DSTstruct));
		memset(sk->otherObject[3].dst, '\0', (sk->otherObject[3]).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->otherObject[3].dstSize; i++) {
		InitSRC(&sk->otherObject[3].src[i]);
	}
	for (int i = 0; i < sk->otherObject[3].dstSize; i++) {
		InitDST(&sk->otherObject[3].dst[i]);
	}

	(sk->otherObject[6]).srcSize = 0;
	if ((sk->otherObject[6]).dstSize == 0) {
		(sk->otherObject[6]).dstSize = 20;
		(sk->otherObject[6]).src = (SRCstruct *)malloc(20 * sizeof(SRCstruct));
		memset(sk->otherObject[6].src, '\0', (sk->otherObject[6]).dstSize * sizeof(SRCstruct));
		(sk->otherObject[6]).dst = (DSTstruct *)malloc((sk->otherObject[6]).dstSize * sizeof(DSTstruct));
		memset(sk->otherObject[6].dst, '\0', (sk->otherObject[6]).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->otherObject[6].dstSize; i++) {
		InitSRC(&sk->otherObject[6].src[i]);
	}
	for (int i = 0; i < sk->otherObject[6].dstSize; i++) {
		InitDST(&sk->otherObject[6].dst[i]);
	}

	(sk->otherObject[7]).srcSize = 0;
	if ((sk->otherObject[7]).dstSize == 0) {
		(sk->otherObject[7]).dstSize = 20;
		(sk->otherObject[7]).src = (SRCstruct *)malloc(20 * sizeof(SRCstruct));
		memset(sk->otherObject[7].src, '\0', (sk->otherObject[7]).dstSize * sizeof(SRCstruct));
		(sk->otherObject[7]).dst = (DSTstruct *)malloc((sk->otherObject[7]).dstSize * sizeof(DSTstruct));
		memset(sk->otherObject[7].dst, '\0', (sk->otherObject[7]).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->otherObject[7].dstSize; i++) {
		InitSRC(&sk->otherObject[7].src[i]);
	}
	for (int i = 0; i < sk->otherObject[7].dstSize; i++) {
		InitDST(&sk->otherObject[7].dst[i]);
	}

	(sk->otherObject[4]).srcSize = 0;
	if ((sk->otherObject[4]).dstSize == 0) {
		(sk->otherObject[4]).dstSize = 20;
		(sk->otherObject[4]).src = (SRCstruct *)malloc(20 * sizeof(SRCstruct));
		memset(sk->otherObject[4].src, '\0', (sk->otherObject[4]).dstSize * sizeof(SRCstruct));
		(sk->otherObject[4]).dst = (DSTstruct *)malloc((sk->otherObject[4]).dstSize * sizeof(DSTstruct));
		memset(sk->otherObject[4].dst, '\0', (sk->otherObject[4]).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->otherObject[4].dstSize; i++) {
		InitSRC(&sk->otherObject[4].src[i]);
	}
	for (int i = 0; i < sk->otherObject[4].dstSize; i++) {
		InitDST(&sk->otherObject[4].dst[i]);
	}

	(sk->otherObject[5]).srcSize = 0;
	if ((sk->otherObject[5]).dstSize == 0) {
		(sk->otherObject[5]).dstSize = 20;
		(sk->otherObject[5]).src = (SRCstruct *)malloc(20 * sizeof(SRCstruct));
		memset(sk->otherObject[5].src, '\0', (sk->otherObject[5]).dstSize * sizeof(SRCstruct));
		(sk->otherObject[5]).dst = (DSTstruct *)malloc((sk->otherObject[5]).dstSize * sizeof(DSTstruct));
		memset(sk->otherObject[5].dst, '\0', (sk->otherObject[5]).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->otherObject[5].dstSize; i++) {
		InitSRC(&sk->otherObject[5].src[i]);
	}
	for (int i = 0; i < sk->otherObject[5].dstSize; i++) {
		InitDST(&sk->otherObject[5].dst[i]);
	}

	InitSRC(&sk->src_MOUSECURSOR);
	sk->dst_MOUSECURSOR.n = 0;
	sk->dst_MOUSECURSOR.loop = 0;
	sk->dst_MOUSECURSOR.opt1 = 0;
	sk->dst_MOUSECURSOR.opt2 = 0;
	sk->dst_MOUSECURSOR.opt3 = 0;
	sk->dst_MOUSECURSOR.opt4 = 0;
	sk->dst_MOUSECURSOR.opt5 = 0;
	sk->dst_MOUSECURSOR.dstCount = 0;
	sk->dst_MOUSECURSOR.dataSize = 0;

	sk->BAR_CENTER = 0;
	for (int i = 0; i < 10; i++) {
		InitSRC(&sk->src_BAR_BODY[i]);
	}
	for (int i = 0; i < 30; i++) {
		InitDST(&sk->dst_BAR_BODY_OFF[i]);
	}
	for (int i = 0; i < 30; i++) {
		InitDST(&sk->dst_BAR_BODY_ON[i]);
	}

	for (int i = 0; i < 5; i++) {
		InitSRC(&sk->src_BAR_TITLE[i]);
	}
	for (int i = 0; i < 5; i++) {
		InitDST(&sk->dst_BAR_TITLE[i]);
	}

	sk->bar_availabe_from = 0;
	sk->bar_availabe_to = 0;
	InitSRC(&sk->src_BAR_FLASH);
	InitDST(&sk->dst_BAR_FLASH);
	InitDST(&sk->dst_BAR_STAGEFILE);

	for (int i = 0; i < 10; i++) {
		InitSRC(&sk->src_BAR_LEVEL[i]);
	}
	for (int i = 0; i < 10; i++) {
		InitDST(&sk->dst_BAR_LEVEL[i]);
	}

	for (int i = 0; i < 20; i++) {
		InitSRC(&sk->src_NOTE[i]);
		InitSRC(&sk->src_MINE[i]);
		InitSRC(&sk->src_LN_START[i]);
		InitSRC(&sk->src_LN_END[i]);
		InitSRC(&sk->src_LN_BODY[i]);
		InitSRC(&sk->src_AUTO_NOTE[i]);
		InitSRC(&sk->src_AUTO_MINE[i]);
		InitSRC(&sk->src_AUTO_LN_START[i]);
		InitSRC(&sk->src_AUTO_LN_END[i]);
		InitSRC(&sk->src_AUTO_LN_BODY[i]);
		InitDST(&sk->dst_NOTE[i]);
	}

	for (int i = 0; i < 2; i++) {
		InitSRC(&sk->src_LINE[i]);
		InitDST(&sk->dst_LINE[i]);
	}

	for (int i = 0; i < 6; i++) {
		InitSRC(&sk->src_NOWJUDGE_1P[i]);
		InitSRC(&sk->src_NOWJUDGE_2P[i]);
	}
	for (int i = 0; i < 6; i++) {
		InitDST(&sk->dst_NOWJUDGE_1P[i]);
		InitDST(&sk->dst_NOWJUDGE_2P[i]);
	}

	for (int i = 0; i < 6; i++) {
		InitSRC(&sk->src_NOWCOMBO_1P[i]);
		InitSRC(&sk->src_NOWCOMBO_2P[i]);
	}
	for (int i = 0; i < 6; i++) {
		InitDST(&sk->dst_NOWCOMBO_1P[i]);
		InitDST(&sk->dst_NOWCOMBO_2P[i]);
	}

	for (int i = 0; i < 6; i++) {
		InitSRC(&sk->src_BAR_LAMP[i]);
		InitDST(&sk->dst_BAR_LAMP[i]);
		InitSRC(&sk->src_BAR_MY_LAMP[i]);
		InitDST(&sk->dst_BAR_MY_LAMP[i]);
		InitSRC(&sk->src_BAR_RIVAL_LAMP[i]);
		InitDST(&sk->dst_BAR_RIVAL_LAMP[i]);
	}

	for (int i = 0; i < 2; i++) {
		InitSRC(&sk->src_GROOVEGAUGE[i]);
		InitDST(&sk->dst_GROOVEGAUGE[i]);
	}

	for (int i = 0; i < 2; i++) {
		InitSRC(&sk->src_GAUGECHART_1P[i]);
		InitDST(&sk->dst_GAUGECHART_1P[i]);
		InitSRC(&sk->src_GAUGECHART_2P[i]);
		InitDST(&sk->dst_GAUGECHART_2P[i]);
	}

	for (int i = 0; i < 3; i++) {
		InitSRC(&sk->src_SCORECHART[i]);
		InitDST(&sk->dst_SCORECHART[i]);
	}

	for (int i = 0; i < 2; i++) {
		InitSRC(&sk->src_JUDGELINE[i]);
		InitDST(&sk->dst_JUDGELINE[i]);
	}
	sk->scratchAngle_1 = 0.0;
	sk->scratchAngle_2 = 0.0;
	InitSRC(&sk->src_THUMBNAIL);
	InitDST(&sk->dst_THUMBNAIL);
	InitSRC(&sk->src_README[0]);
	InitDST(&sk->dst_README[0]);
	InitSRC(&sk->src_README[1]);
	InitDST(&sk->dst_README[1]);
	for (int i = 0; i < 10; i++) {
		if (sk->ImageFonts[i].images == NULL) {
			sk->ImageFonts[i].images = (FontImage*)malloc(1000 * sizeof(FontImage));
		}
		if (sk->ImageFonts[i].chars == NULL) {
			sk->ImageFonts[i].chars = (FontChar*)malloc(0x3bce * sizeof(FontChar));
		}
	}
	sk->count = 0;
	sk->num_of_struct = 0;
	sk->num_of_ImageFont = 0;
	if (font == '\0') {
		InitFontToHandle();
	}
	for (int i = 0; i < 10; i++) {
		sk->helpfilePath[i].fillzero();
	}
	sk->textmergin_1 = 10;
	sk->textmergin_2 = 10;
	sk->helpfileCount = 0;
	sk->flag_BGA = 1;
	(sk->adjust).rate_x = 100;
	(sk->adjust).rate_y = 100;
	(sk->adjust).shift_x = 0;
	(sk->adjust).shift_y = 0;
	(sk->adjust).judge_x = 0;
	(sk->adjust).judge_y = 0;
	(sk->adjust).unk18 = 0; //name
	(sk->adjust).unk1c = 0; //name
	(sk->adjust).size_x = 0;
	(sk->adjust).size_y = 0;
	(sk->adjust).dark_type = 0;
	(sk->adjust).note_1p_x = 0;
	(sk->adjust).note_1p_y = 0;
	(sk->adjust).note_2p_x = 0;
	(sk->adjust).note_2p_y = 0;
	sk->scratchside_1 = 0;
	sk->scratchside_2 = 1;
	for (int i = 0; i < 20; i++) {
		sk->customfileRANDOM[i].fillzero();
		sk->customfile[i].fillzero();
	}
	sk->customfile_count = 0;

	DeleteGraph(sk->GrHandle[105]);
	sk->GrHandle[105] = MakeGraph(640, 480);
	DeleteGraph(sk->GrHandle[104]);
	sk->GrHandle[104] = MakeGraph(640, 480);
	if (sk->GrHandle[100] == -1) sk->GrHandle[100] = MakeGraph(640, 480);
	if (sk->GrHandle[101] == -1) sk->GrHandle[101] = MakeGraph(640, 480);
	if (sk->GrHandle[102] == -1) sk->GrHandle[102] = MakeGraph(640, 480);
	DeleteGraph(sk->GrHandle[110]);
	sk->GrHandle[110] = LoadGraph("LR2files\\Config\\black.bmp");
	DeleteGraph(sk->GrHandle[111]);
	sk->GrHandle[111] = LoadGraph("LR2files\\Config\\white.bmp");
	sk->reloadbanner = 0;
	for (int i = 0; i < 10; i++) {
		InitSRC(&sk->src_BAR_RANK[i]);
		InitDST(&sk->dst_BAR_RANK[i]);
		InitSRC(&sk->src_BAR_RIVAL[i]);
		InitDST(&sk->dst_BAR_RIVAL[i]);
	}

	InitSRC(&sk->src_EVENT_MODE_CURSOR);
	for (int i = 0; i < 10; i++) {
		InitDST(&sk->dst_EVENT_MODE_CURSOR_ON[i]);
		InitDST(&sk->dst_EVENT_MODE_CURSOR_OFF[i]);
		sk->event_STARTINPUT[i]=0;
		sk->event_FADEOUT[i] = 0;
	}

	for (int i = 0; i < 5; i++) {
		InitSRC(&sk->src_BAR_STAR[i]);
		InitDST(&sk->dst_BAR_STAR[i]);
	}

	for (int i = 0; i < 5; i++) {
		InitDST(&sk->dst_EVENT_LOADINGBG[i]);
	}
	return 1;
}

//imagefont section
//4a03e0
int InitImageFont(ImageFont *imgfont) {
	if (imgfont->size == 0) return 0;
	imgfont->size = 0;
	imgfont->kerning = 0;
	imgfont->filepath[0] = '\0';
	for (int i = 0; i < 0x3BCE; i++) {
		DeleteGraph(imgfont->chars[i].grHandle);
		imgfont->chars[i].grHandle = -1;
	}
	for (int i = 0; i < 1000; i++) {
		DeleteGraph(imgfont->images[i].grHandle);
		imgfont->images[i].grHandle = -1;
		imgfont->images[i].filename[0] = '\0';
	}
}

//4a0480
int ReadImageFont(CSTR filename, ImageFont *imgfont) {
	CSTR str1;
	
	str1 = filename.getDirectory();

	if (strcmp(str1, imgfont->filepath)) {
		imgfont->size = 0;
		imgfont->kerning = 0;
		for (int i = 0; i < 0x3bce; i++) {
			DeleteGraph(imgfont->chars[i].grHandle);
			imgfont->chars[i].grHandle = -1;
		}
		for (int i = 0; i < 1000; i++) {
			DeleteGraph(imgfont->images[i].grHandle);
			imgfont->images[i].grHandle = -1;
			imgfont->images[i].filename[0] = '\0';
		}

		int f = FileRead_open(filename);
		if (f == 0) {
			ErrorLogFmtAdd("画像フォントファイル%sの読み込みに失敗しました\n");
			return -1;
		}
		
		CSTR str2 = CSTR(256);
		CSVbuf csvBuf;
		strcpy(imgfont->filepath, str1);
		
		while (FileRead_gets(str2, 250, f) != -1) {
			if (*str2.atPos(0) == '#') {
				str2.trimWhiteSpace();
				DealWhiteSpace(&str2);
				SplitCSV(str2, &csvBuf, ",");
				if (*str2.atPos(1) == 'R') {
					imgfont->chars[csvBuf.val[1]].srcX = csvBuf.val[3];
					imgfont->chars[csvBuf.val[1]].srcY = csvBuf.val[4];
					imgfont->chars[csvBuf.val[1]].width = csvBuf.val[5];
					imgfont->chars[csvBuf.val[1]].height = csvBuf.val[6];
					imgfont->chars[csvBuf.val[1]].ImageNum = csvBuf.val[2];
				}
				else if (*str2.atPos(1) == 'M') {
					imgfont->kerning = csvBuf.val[1];
				}
				else if (*str2.atPos(1) == 'S') {
					imgfont->size = csvBuf.val[1];
				}
				else if (*str2.atPos(1) == 'T') {
					strcpy(imgfont->images[csvBuf.val[1]].filename, csvBuf.str[2]);
				}
			}
			*str2.atPos(0) = '\0';
		}

		FileRead_close(f);
	}
}

//4a0870
int LoadFontGraph(ImageFont *imgfont, int *fontNum){
	int fnum;
	char *filepath;
	int v_grHandle;
	int v_flag;
	CSTR str = CSTR(imgfont->filepath,0);

	fnum = *fontNum;
	if ( (-1 < fnum && fnum < 1000) && (imgfont->images[fnum].grHandle == -1) ) {
		str.add(imgfont->images[fnum].filename);
		DeleteGraph(imgfont->images[fnum].grHandle);
		v_flag = 0;
		filepath = str.outstr();
		v_grHandle = LoadGraph(filepath, v_flag);
		imgfont->images[fnum].grHandle = v_grHandle;
		return 1;
	}
	return 0;
}

//4a0970
int LoadFontCharGraph(ImageFont *imgfont, uint vChar){
	int gr;
	FontImage *fb;
	int fNum;
	FontChar *fc;

	if (0x3bcd < vChar) {
		return 0;
	}
	fc = imgfont->chars;
	fNum = fc[vChar].ImageNum;
	if ((((-1 < fNum) && (fNum < 1000)) && (fc[vChar].grHandle == -1)) &&
		((0 < fc[vChar].height && (0 < fc[vChar].width)))) {
		fb = imgfont->images;
		if (fb[fNum].grHandle == -1) {
			LoadFontGraph(imgfont, &fc[vChar].ImageNum);
			fb = imgfont->images;
			/* load failure check */
			if (fb[imgfont->chars[vChar].ImageNum].grHandle == -1) {
				return 0;
			}
		}
		fc = imgfont->chars + vChar;
		gr = DerivationGraph(fc->srcX, fc->srcY, fc->width, fc->height, fb[fc->ImageNum].grHandle);
		imgfont->chars[vChar].grHandle = gr;
		return 1;
	}
	return 0;
}

//4a0a40
int LoadFontForText(ImageFont *imgfont, CSTR *str){
	char cVar1;
	bool bVar2;
	int iVar3;
	char *pcVar4;
	int iVar5;
	byte *pbVar6;
	ushort local_4;

	ushort twochar;

	if (str->length() <= 0) return 0;

	for (int i = 0; *str->atPos(i) && i < str->length(); 0) {
		if ( IsMultibyte(*str->atPos(i)) ) {
			twochar = (*str->atPos(i) << 8) + (uchar)*str->atPos(i + 1);
			if (twochar > 0x9ffd) {
				twochar += 0xbfbf;
			}
			twochar += 0x7fc0;
			i += 2;
		}
		else {
			twochar = *str->atPos(i);
			i++;
		}

		if (imgfont->chars[twochar].grHandle == -1){
			LoadFontCharGraph(imgfont,twochar);
		}
	}
	return 1;
}

//2p graphic section
//4a0b30
int FlipSide_Timer(int *n){
	int t = *n;
	switch (t) {
	case 0x2a:
		*n = 0x2b;
		return 1;
	case 0x2b:
		*n = 0x2a;
		return 1;
	case 0x2c:
		*n = 0x2d;
		return 1;
	case 0x2d:
		*n = 0x2c;
		return 1;
	case 0x2e:
		*n = 0x2f;
		return 1;
	case 0x2f:
		*n = 0x2e;
		return 1;
	case 0x30:
		*n = 0x31;
		return 1;
	case 0x31:
		*n = 0x30;
		return 1;
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x39:
	case 0x3a:
	case 0x3b:
	case 0x46:
	case 0x47:
	case 0x48:
	case 0x49:
	case 0x4a:
	case 0x4b:
	case 0x4c:
	case 0x4d:
	case 0x4e:
	case 0x4f:
	case 100:
	case 0x65:
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
	case 0x6a:
	case 0x6b:
	case 0x6c:
	case 0x6d:
	case 0x78:
	case 0x79:
	case 0x7a:
	case 0x7b:
	case 0x7c:
	case 0x7d:
	case 0x7e:
	case 0x7f:
	case 0x80:
	case 0x81:
		*n = t + 10;
		return 1;
	case 0x3c:
	case 0x3d:
	case 0x3e:
	case 0x3f:
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x50:
	case 0x51:
	case 0x52:
	case 0x53:
	case 0x54:
	case 0x55:
	case 0x56:
	case 0x57:
	case 0x58:
	case 0x59:
	case 0x6e:
	case 0x6f:
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
	case 0x74:
	case 0x75:
	case 0x76:
	case 0x77:
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
		*n = t + -10;
		return 1;
	case 0x8f:
		*n = 0x90;
		return 1;
	case 0x90:
		*n = 0x8f;
	}
	return 1;
}

//4a0c80
int ApplyFlipside(skstruct *sk){
	int iVar2;
	SRCstruct srcTemp;
	DSTstruct dstTemp;

	memcpy(&srcTemp, &sk->src_GROOVEGAUGE[0], sizeof(SRCstruct));
	memcpy(&sk->src_GROOVEGAUGE[0], &sk->src_GROOVEGAUGE[1], sizeof(SRCstruct));
	memcpy(&sk->src_GROOVEGAUGE[1], &srcTemp, sizeof(SRCstruct));

	memcpy(&dstTemp, &sk->dst_GROOVEGAUGE[0], sizeof(DSTstruct));
	memcpy(&sk->dst_GROOVEGAUGE[0], &sk->dst_GROOVEGAUGE[1], sizeof(DSTstruct));
	memcpy(&sk->dst_GROOVEGAUGE[1], &dstTemp, sizeof(DSTstruct));
	
	for (int i = 0; i < 6; i++) {
		memcpy(&srcTemp, &sk->src_NOWJUDGE_1P[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOWJUDGE_1P[i], &sk->src_NOWJUDGE_2P[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOWJUDGE_2P[i], &srcTemp, sizeof(SRCstruct));

		memcpy(&dstTemp, &sk->dst_NOWJUDGE_1P[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOWJUDGE_1P[i], &sk->dst_NOWJUDGE_2P[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOWJUDGE_2P[i], &dstTemp, sizeof(DSTstruct));

		memcpy(&srcTemp, &sk->src_NOWCOMBO_1P[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOWCOMBO_1P[i], &sk->src_NOWCOMBO_2P[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOWCOMBO_2P[i], &srcTemp, sizeof(SRCstruct));

		memcpy(&dstTemp, &sk->dst_NOWCOMBO_1P[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOWCOMBO_1P[i], &sk->dst_NOWCOMBO_2P[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOWCOMBO_2P[i], &dstTemp, sizeof(DSTstruct));
	}

	for (int i = 0; i < 10; i++) {
		memcpy(&srcTemp, &sk->src_NOTE[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOTE[i], &sk->src_NOTE[10+i], sizeof(SRCstruct));
		memcpy(&sk->src_NOTE[10+i], &srcTemp, sizeof(SRCstruct));

		memcpy(&dstTemp, &sk->dst_NOTE[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOTE[i], &sk->dst_NOTE[10 + i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOTE[10 + i], &dstTemp, sizeof(DSTstruct));

		memcpy(&srcTemp, &sk->src_LN_BODY[i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_BODY[i], &sk->src_LN_BODY[10 + i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_BODY[10 + i], &srcTemp, sizeof(SRCstruct));

		memcpy(&srcTemp, &sk->src_LN_START[i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_START[i], &sk->src_LN_START[10 + i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_START[10 + i], &srcTemp, sizeof(SRCstruct));

		memcpy(&srcTemp, &sk->src_LN_END[i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_END[i], &sk->src_LN_END[10 + i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_END[10 + i], &srcTemp, sizeof(SRCstruct));
		//AdditionlTODO: need to add feature MINE, AUTO?
	}

	memcpy(&srcTemp, &sk->src_LINE[0], sizeof(SRCstruct));
	memcpy(&sk->src_LINE[0], &sk->src_LINE[1], sizeof(SRCstruct));
	memcpy(&sk->src_LINE[1], &srcTemp, sizeof(SRCstruct));

	memcpy(&dstTemp, &sk->dst_LINE[0], sizeof(DSTstruct));
	memcpy(&sk->dst_LINE[0], &sk->dst_LINE[1], sizeof(DSTstruct));
	memcpy(&sk->dst_LINE[1], &dstTemp, sizeof(DSTstruct));


	for (int i = 0; i < (sk->image).srcSize; i++) {
		FlipSide_Timer(&sk->image.src[i].timer);
		FlipSide_Timer(&sk->image.dst[i].timer);
		if (sk->image.dst[i].opt5 == 1) sk->image.dst[i].opt5 = 2;
		else if (sk->image.dst[i].opt5 == 2) sk->image.dst[i].opt5 = 1;
	}

	for (int i = 0; i < sk->otherObject[6].srcSize; i++) {
		//logic arragned
		int &op = sk->otherObject[6].src[i].op1;
		if (op < 100) {
			if (op == 10) op = 12;
			else if (op == 12) op = 10;
		}
		else if (op < 120) op += 20;
		else if (op > 119) op -= 20;
	}

	for (int i = 0; i < sk->otherObject[5].srcSize; i++) {
		int &op = sk->otherObject[5].src[i].op1;
		if (op == 10) op = 14;
		else if (op == 14) op = 10;
	}

	for (int i = 0; i < sk->otherObject[0].srcSize; i++) {
		int &op = sk->otherObject[0].src[i].st;
		if (op == 1) op = 2;
		else if (op == 2) op = 1;
	}

	iVar2 = sk->scratchside_1;
	sk->scratchside_1 = sk->scratchside_2;
	sk->scratchside_2 = iVar2;

	for (int i = 0; i < sk->otherObject[2].srcSize; i++) {
		int &op = sk->otherObject[2].src[i].op3;
		if (op == 4)op = 5;
		else if (op == 5) op = 4;
	}

	return 1;
}

//4a1150
int ClearSkinGraph(skstruct *sk){
	InitSkin(sk, 0, '\0');
	for (int i = 0; i < 200; i++) {
		sk->caption[i].fillzero();
		sk->GrHandle[i] = -1;
		sk->count = 0; //???
	}

	for (int i = 0; i < 10; i++) {
		InitImageFont(&sk->ImageFonts[i]);
	}
	return 1;
}

//maybe deleted by compiler, restored it for convenience
int ExpandSkinObjectMax(SkinObject *so, int add) {
	if (so->dstSize - 1 == so->srcSize) {
		so->src = (SRCstruct*)realloc(so->src, (so->dstSize + add) * sizeof(SRCstruct));
		so->dst = (DSTstruct*)realloc(so->dst, (so->dstSize + add) * sizeof(DSTstruct));
		for (int i = so->dstSize; i < so->dstSize + add; i++) {
			memset(&so->src[i], 0, sizeof(SRCstruct));
			InitSRC(&so->src[i]);
			memset(&so->dst[i], 0, sizeof(DSTstruct));
			InitDST(&so->dst[i]);
		}
		so->dstSize += add;
	}
	return 1;
}

//4a11c0 ReadSkin // maybe unsatble
int ReadSkin(skstruct *sk,CSTR FilePath, int unused, int skin_num, SkinUser* sku, char flag_skipFont) {
	FILE *pFile;
	CSTR fBuf = CSTR(1024);
	char* pFbuf;
	CSVbuf csv;
	int tSkin_num = 0;
	int line;
	int IFCOUNT, IFSWITCH[100];

	bool flipside = false;

	tSkin_num = skin_num;
	for (int i = 0; i < 100; i++) IFSWITCH[i] = 0;
	IFCOUNT = 0;
	sk->op[0] = 1;
	sk->op[999] = 0;
	ErrorLogFmtAdd("スキンの読み込みを開始します。 %s\n", FilePath);
	ErrorLogTabAdd();

	pFile = fopen(FilePath, "r");
	CSTR dir = CSTR(FilePath.getDirectory());
	line = 0;

	if (!pFile) {
		ErrorLogFmtAdd("スキンの読み込みに失敗しました。 %s\n", FilePath);
		ErrorLogTabSub();
		return 0;
	}
	pFbuf = fBuf.outstr();
	for (pFbuf = fgets(pFbuf, 1023, pFile); pFbuf; pFbuf = fgets(pFbuf, 1023, pFile)) {
		//if (sk->image.dstSize - 1 == sk->image.srcSize) {
		//	sk->image.src = (SRCstruct*)realloc(sk->image.src, (sk->image.dstSize + 50) * sizeof(SRCstruct));
		//	sk->image.dst = (DSTstruct*)realloc(sk->image.dst, (sk->image.dstSize + 50) * sizeof(DSTstruct));
		//	for (int i = sk->image.dstSize; i < sk->image.dstSize + 50; i++) {
		//		memset(&sk->image.src[i], 0, sizeof(SRCstruct));
		//		InitSRC(&sk->image.src[i]);
		//		memset(&sk->image.dst[i], 0, sizeof(DSTstruct));
		//		InitDST(&sk->image.dst[i]);
		//	}
		//	sk->image.dstSize += 50;
		//}
		ExpandSkinObjectMax(&sk->image, 50);
		ExpandSkinObjectMax(&sk->otherObject[0], 20);
		ExpandSkinObjectMax(&sk->otherObject[1], 20);
		ExpandSkinObjectMax(&sk->otherObject[2], 20);
		ExpandSkinObjectMax(&sk->otherObject[3], 20);
		ExpandSkinObjectMax(&sk->otherObject[4], 20);
		ExpandSkinObjectMax(&sk->otherObject[6], 20);
		ExpandSkinObjectMax(&sk->otherObject[7], 20);
		ExpandSkinObjectMax(&sk->otherObject[5], 20);

		if (fBuf.length() > 6) {
			if (*fBuf.atPos(0) == '#') {
				fBuf.nullAtPos(fBuf.length() - 1);
				fBuf.trimWhiteSpace();
				DealWhiteSpace(&fBuf);
				if (!fBuf.left(1).isDiff("#")) {
					if (fBuf.left(3).isSame("#IF")) {
						if (IFCOUNT != 99) {
							IFSWITCH[IFCOUNT + 1] = 1;
							SplitCSV(fBuf, &csv, ",");
							for (int i = 1; i < 10; i++) {
								if (csv.val[i] < 0 || csv.val[i]>999 || sk->op[csv.val[i]] == 0) {
									i = 10;
									IFSWITCH[IFCOUNT + 1] = 2;
								}
							}
							IFCOUNT++;
						}
						else {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nネスト可能な#IFの上限に達しました。\n", line, fBuf);
							if (IFSWITCH[IFCOUNT] > 1) {
								*fBuf.atPos(0) = '\0';
								continue;
							}
						}
					}
					else if (fBuf.left(7).isSame("#ELSEIF") && IFSWITCH[IFCOUNT] != 3) {
						if (IFCOUNT) {
							if (IFSWITCH[IFCOUNT] == 1) IFSWITCH[IFCOUNT] = 3;
							else {
								IFSWITCH[IFCOUNT] = 1;
								SplitCSV(fBuf, &csv, ",");
								for (int i = 1; i < 10; i++) {
									if (csv.val[i] < 0 || csv.val[i]>999 || sk->op[csv.val[i]] == 0) {
										i = 10;
										IFSWITCH[IFCOUNT] = 2;
									}
								}
							}
						}
						else ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n対応する#IFが見つかりません。\n", line, fBuf);
					}
					else if (fBuf.left(5).isSame("#ELSE") && IFSWITCH[IFCOUNT] != 3) {
						if (IFCOUNT == 0) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n対応する#IFが見つかりません。\n", line, fBuf);
						}
						else {
							IFSWITCH[IFCOUNT] = (IFSWITCH[IFCOUNT] == 1) ? 3 : 1;
						}
					}
					else if (fBuf.left(6).isSame("#ENDIF")) {
						if (IFCOUNT == 0) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n対応する#IFが見つかりません。\n", line, fBuf);
						}
						else {
							IFSWITCH[IFCOUNT] = 0;
							IFCOUNT--;
						}
					}
				}

				if(IFCOUNT > 0){
					if (IFSWITCH[IFCOUNT] > 1) {
						*fBuf.atPos(0) = '\0';
						continue;
					}
				}


				if (!fBuf.left(1).isDiff("#")) {
					if (fBuf.left(6).isSame("#IMAGE")) {
						if (sk->count == 100) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nこれ以上#IMAGEを登録できません。\n", line, fBuf);
						}
						else {
							SplitCSV(fBuf, &csv, ",");
							if (csv.str[1].isSame("CONTINUE")) {
								sk->caption[sk->count].assign("CONTINUE");
								sk->count++;
							}
							else {
								if (sk->caption[sk->count].isDiff(&csv.str[1]) || sk->grIsMovie[sk->count] == 1 || sk->caption[sk->count].findStrPos("*") != -1) {
									DeleteGraph(sk->GrHandle[sk->count]);
									sk->caption[sk->count].assign(&csv.str[1]);
									if (csv.str[1].right(3).isSame("mpg") || csv.str[1].right(3).isSame("avi") || csv.str[1].right(3).isSame("wma") || csv.str[1].right(3).isSame("ogv")) {
										sk->grIsMovie[sk->count] = 1;
									}
									else {
										sk->grIsMovie[sk->count] = 0;
									}
									for (int i = 0; i < sk->customfile_count; i++) {
										if (sk->customfileRANDOM[i].isSame(csv.str[1].left(sk->customfileRANDOM[i].length())) && sk->customfile[i].isDiff("RANDOM") != 0 && sk->customfile[i].isDiff("ERROR") && sk->customfile[i].length() > 0 ) {
											csv.str[1].replace("*", sk->customfile[i]);
											break;
										}
									}
									CSTR temp = CSTR(GetRandomFileNoError(csv.str[1], dir), 0);
									sk->GrHandle[sk->count] = LoadGraph(temp);
									sk->caption[sk->count].assign(&temp);
								}
								sk->count++;
							}
						}
					}
					else if (fBuf.left(5).isSame("#FONT") && !flag_skipFont) {
						if (sk->num_of_struct == 10) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nこれ以上#FONTを登録できません。\n", line, fBuf);
						}
						else {
							SplitCSV(fBuf, &csv, ",");
							sk->fontHandle[sk->num_of_struct] = CreateFontToHandle(sk->fontname, csv.val[1], csv.val[2], csv.val[3], 0, -1, 0, -1);//, -1);
							if (sk->fontHandle[sk->num_of_struct] == -1) {
								sk->fontHandle[sk->num_of_struct] = 0;
							}
							sk->num_of_struct = sk->num_of_struct + 1;
						}
					}
					else if (fBuf.left(10).isSame("#SRC_IMAGE")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->image.src[sk->image.srcSize], &csv, sk);
						if (sk->image.src[sk->image.srcSize].graphcount < 1 || sk->image.src[sk->image.srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf);
						}
						if (sk->image.srcSize > 0 && (sk->image.dst[sk->image.srcSize - 1].dstCount < 1 || sk->image.dst[sk->image.srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_IMAGEに対応した#DST_IMAGEが存在しないか、登録に失敗したようです\n", line, fBuf);
						}
						sk->image.srcSize++;
					}
					else if (fBuf.left(10).isSame("#DST_IMAGE") && sk->image.srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->image.dst[sk->image.srcSize - 1], &csv, tSkin_num);
						if (sk->image.dst[sk->image.srcSize - 1].dstCount < 1 || sk->image.dst[sk->image.srcSize - 1].dataSize < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nDSTの登録に失敗しました。DSTの一番最初がエラーを起こしている可能性があります。\n", line, fBuf);
						}
						else if (sk->image.dst[sk->image.srcSize - 1].dstCount == sk->image.dst[sk->image.srcSize - 1].dstCount){
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nDSTの登録に失敗しました。この行の登録のみ失敗しました。\n", line, fBuf);
						}
					}
					else if (fBuf.left(9).isSame("#SRC_TEXT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->otherObject[0].src[sk->otherObject[0].srcSize].n = csv.val[1];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].fontHandle = sk->fontHandle[csv.val[2]];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].cycle = csv.val[2];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].st = csv.val[3];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].align = csv.val[4];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op1 = csv.val[5];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op2 = csv.val[6];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op3 = csv.val[7];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op4 = csv.val[8];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op5 = csv.val[9];
						sk->otherObject[0].srcSize++;
					}
					else if (fBuf.left(9).isSame("#DST_TEXT") && sk->otherObject[0].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[0].dst[sk->otherObject[0].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(11).isSame("#SRC_SLIDER")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[2].src[sk->otherObject[2].srcSize], &csv, sk);
						if (sk->otherObject[2].src[sk->otherObject[2].srcSize].graphcount < 1 || sk->otherObject[2].src[sk->otherObject[2].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf);
						}
						if (sk->otherObject[2].srcSize > 0 && (sk->otherObject[2].dst[sk->otherObject[2].srcSize - 1].dstCount < 1 || sk->otherObject[2].dst[sk->otherObject[2].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_SLIDERに対応した#DST_SLIDERが存在しないか、登録に失敗したようです\n", line, fBuf);
						}
						sk->otherObject[2].srcSize++;
					}
					else if (fBuf.left(11).isSame("#DST_SLIDER") && sk->otherObject[2].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[2].dst[sk->otherObject[2].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(11).isSame("#SRC_BUTTON")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[1].src[sk->otherObject[1].srcSize], &csv, sk);
						if (sk->otherObject[1].src[sk->otherObject[1].srcSize].graphcount < 1 || sk->otherObject[1].src[sk->otherObject[1].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf);
						}
						if (sk->otherObject[1].srcSize > 0 && (sk->otherObject[1].dst[sk->otherObject[1].srcSize - 1].dstCount < 1 || sk->otherObject[1].dst[sk->otherObject[1].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_BUTTONに対応した#DST_BUTTONが存在しないか、登録に失敗したようです\n", line, fBuf);
						}
						sk->otherObject[1].srcSize++;
					}
					else if (fBuf.left(11).isSame("#DST_BUTTON") && sk->otherObject[1].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[1].dst[sk->otherObject[1].srcSize - 1], &csv, tSkin_num);

					}
					else if (fBuf.left(12).isSame("#SRC_ONMOUSE")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[3].src[sk->otherObject[3].srcSize], &csv, sk);
						if (sk->otherObject[3].src[sk->otherObject[3].srcSize].graphcount < 1 || sk->otherObject[3].src[sk->otherObject[3].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf);
						}
						if (sk->otherObject[3].srcSize > 0 && (sk->otherObject[3].dst[sk->otherObject[3].srcSize - 1].dstCount < 1 || sk->otherObject[3].dst[sk->otherObject[3].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_BGAに対応した#DST_BGAが存在しないか、登録に失敗したようです\n", line, fBuf);
						}
						sk->otherObject[3].srcSize++;
					}
					else if (fBuf.left(12).isSame("#DST_ONMOUSE") && sk->otherObject[3].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[3].dst[sk->otherObject[3].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(8).isSame("#SRC_BGA")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[4].src[sk->otherObject[4].srcSize], &csv, sk);
						if (sk->otherObject[4].src[sk->otherObject[4].srcSize].graphcount < 1 || sk->otherObject[4].src[sk->otherObject[4].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf);
						}
						if (sk->otherObject[4].srcSize > 0 && (sk->otherObject[4].dst[sk->otherObject[4].srcSize - 1].dstCount < 1 || sk->otherObject[4].dst[sk->otherObject[4].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_BGAに対応した#DST_BGAが存在しないか、登録に失敗したようです\n", line, fBuf);
						}
						sk->otherObject[4].srcSize++;
					}
					else if (fBuf.left(8).isSame("#DST_BGA") && sk->otherObject[4].srcSize > 0) {
						SplitCSV(fBuf, &csv, ","); 
						ReadDST(&sk->otherObject[4].dst[sk->otherObject[4].srcSize - 1], &csv, tSkin_num);
						tSkin_num++;
					}
					else if (fBuf.left(11).isSame("#SRC_NUMBER")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[6].src[sk->otherObject[6].srcSize], &csv, sk);
						if (sk->otherObject[6].src[sk->otherObject[6].srcSize].graphcount < 1 || sk->otherObject[6].src[sk->otherObject[6].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf);
						}
						if (sk->otherObject[6].srcSize > 0 && (sk->otherObject[6].dst[sk->otherObject[6].srcSize - 1].dstCount < 1 || sk->otherObject[6].dst[sk->otherObject[6].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではあ りません)ひとつ前の#SRC_NUMBERに対応した#DST_NUMBERが 存在しないか、登録に失敗したようです\n", line, fBuf);
						}
						sk->otherObject[6].srcSize++;
					}
					else if (fBuf.left(11).isSame("#DST_NUMBER") && sk->otherObject[6].srcSize > 0) {
						SplitCSV(fBuf, &csv, ","); 
						ReadDST(&sk->otherObject[6].dst[sk->otherObject[6].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(9).isSame("#SRC_MASK")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[7].src[sk->otherObject[7].srcSize], &csv, sk);
						sk->otherObject[7].srcSize++;
					}
					else if (fBuf.left(9).isSame("#DST_MASK") && sk->otherObject[7].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[7].dst[sk->otherObject[7].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(13).isSame("#SRC_BARGRAPH")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[5].src[sk->otherObject[5].srcSize], &csv, sk);
						if (sk->otherObject[5].src[sk->otherObject[5].srcSize].graphcount < 1 || sk->otherObject[5].src[sk->otherObject[5].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しまし た。\n", line, fBuf);
						}
						if (sk->otherObject[5].srcSize > 0 && (sk->otherObject[5].dst[sk->otherObject[5].srcSize - 1].dstCount < 1 || sk->otherObject[5].dst[sk->otherObject[5].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではあ りません)ひとつ前の#SRC_BARGRAPHに対応した#DST_BARGRAP Hが存在しないか、登録に失敗したようです\n", line, fBuf);
						}
						sk->otherObject[5].srcSize++;
					}
					else if (fBuf.left(13).isSame("#DST_BARGRAPH") && sk->otherObject[5].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[5].dst[sk->otherObject[5].srcSize - 1], &csv, tSkin_num);
						tSkin_num++;
					}
					else if (fBuf.left(13).isSame("#SRC_BAR_BODY")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 29, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_BODY[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(17).isSame("#DST_BAR_BODY_OFF")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 29, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_BODY_OFF[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(16).isSame("#DST_BAR_BODY_ON")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 29, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_BODY_ON[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(11).isSame("#BAR_CENTER")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 29, line, pFbuf)) {
							SplitCSV(fBuf, &csv, ",");
							sk->BAR_CENTER = csv.val[1];
						}
					}
					else if (fBuf.left(14).isSame("#SRC_BAR_TITLE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 4, line, pFbuf)) {
							ReadSRC_BAR_TITLE(&sk->src_BAR_TITLE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#DST_BAR_TITLE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 4, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_TITLE[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(11).isSame("#TRANSCOLOR")) {
						SplitCSV(fBuf, &csv, ",");
						SetTransColor(csv.val[1], csv.val[2], csv.val[3]);
					}
					else if (fBuf.left(11).isSame("#TRANSCLOLR")) {
						SplitCSV(fBuf, &csv, ",");
						SetTransColor(csv.val[1], csv.val[2], csv.val[3]);
					}
					else if (fBuf.left(12).isSame("#TRANSCLOLOR")) {
						SplitCSV(fBuf, &csv, ",");
						SetTransColor(csv.val[1], csv.val[2],csv.val[3]);
					}
					else if (fBuf.left(11).isSame("#STARTINPUT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->startinput_start = csv.val[1];
						sk->startinput_rank = csv.val[2];
						sk->startinput_update = csv.val[3];
					}
					else if (fBuf.left(10).isSame("#SCENETIME")) {
						SplitCSV(fBuf, &csv, ",");
						sk->scenetime = csv.val[1];
					}
					else if (fBuf.left(8).isSame("#FADEOUT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->fadeout = csv.val[1];
					}
					else if (fBuf.left(6).isSame("#CLOSE")) {
						SplitCSV(fBuf, &csv, ",");
						sk->close = csv.val[1];
					}
					else if (fBuf.left(5).isSame("#SKIP")) {
						SplitCSV(fBuf, &csv, ",");
						sk->close = csv.val[1];
					}
					else if (fBuf.left(10).isSame("#PLAYSTART")) {
						SplitCSV(fBuf, &csv, ",");
						sk->playstart = csv.val[1];
					}
					else if (fBuf.left(10).isSame("#LOADSTART")) {
						SplitCSV(fBuf, &csv, ",");
						sk->loadstart = csv.val[1];
					}
					else if (fBuf.left(8).isSame("#LOADEND")) {
						SplitCSV(fBuf, &csv, ",");
						sk->loadend = csv.val[1];
					}
					else if (fBuf.left(14).isSame("#BAR_AVAILABLE")) {
						SplitCSV(fBuf, &csv, ",");
						sk->bar_availabe_from = csv.val[1];
						sk->bar_availabe_to = csv.val[2];
					}
					else if (fBuf.left(14).isSame("#SRC_BAR_FLASH")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_BAR_FLASH, &csv, sk);
					}
					else if (fBuf.left(14).isSame("#DST_BAR_FLASH")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_BAR_FLASH, &csv, tSkin_num);
					}
					else if (fBuf.left(18).isSame("#DST_BAR_STAGEFILE")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_BAR_STAGEFILE, &csv, tSkin_num);
					}
					else if (fBuf.left(16).isSame("#SRC_MOUSECURSOR")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_MOUSECURSOR, &csv, sk);
					}
					else if (fBuf.left(16).isSame("#DST_MOUSECURSOR")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_MOUSECURSOR, &csv, tSkin_num);
					}
					else if (fBuf.left(14).isSame("#SRC_BAR_LEVEL")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 10, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_LEVEL[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#DST_BAR_LEVEL")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 10, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_LEVEL[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(9).isSame("#SRC_NOTE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_NOTE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(9).isSame("#SRC_MINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_MINE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(13).isSame("#SRC_LN_START")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_LN_START[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(11).isSame("#SRC_LN_END")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_LN_END[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(12).isSame("#SRC_LN_BODY")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_LN_BODY[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_AUTO_NOTE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_NOTE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_AUTO_MINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_MINE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(18).isSame("#SRC_AUTO_LN_START")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_LN_START[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(16).isSame("#SRC_AUTO_LN_END")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_LN_END[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(17).isSame("#SRC_AUTO_LN_BODY")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_LN_BODY[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(9).isSame("#DST_NOTE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadDST(&sk->dst_NOTE[csv.val[1]], &csv, tSkin_num);
							tSkin_num += 2;
						}
					}
					else if (fBuf.left(16).isSame("#SRC_NOWJUDGE_1P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadSRC(&sk->src_NOWJUDGE_1P[csv.val[1]], &csv, sk);
							sk->src_NOWJUDGE_1P[csv.val[1]].timer = 46;
						}
					}
					else if (fBuf.left(16).isSame("#DST_NOWJUDGE_1P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadDST(&sk->dst_NOWJUDGE_1P[csv.val[1]], &csv, tSkin_num);
							sk->dst_NOWJUDGE_1P[csv.val[1]].timer = 46;
						}
					}
					else if (fBuf.left(16).isSame("#SRC_NOWCOMBO_1P")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_NOWCOMBO_1P[csv.val[1]], &csv, sk);
						sk->src_NOWCOMBO_1P[csv.val[1]].timer = 46;
					}
					else if (fBuf.left(16).isSame("#DST_NOWCOMBO_1P")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_NOWCOMBO_1P[csv.val[1]], &csv, tSkin_num);
						sk->dst_NOWJUDGE_1P[csv.val[1]].timer = 46; //???mistake?
					}
					else if (fBuf.left(16).isSame("#SRC_NOWJUDGE_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadSRC(&sk->src_NOWJUDGE_2P[csv.val[1]], &csv, sk);
							sk->src_NOWJUDGE_2P[csv.val[1]].timer = 47;
						}
					}
					else if (fBuf.left(16).isSame("#DST_NOWJUDGE_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadDST(&sk->dst_NOWJUDGE_2P[csv.val[1]], &csv, tSkin_num);
							sk->dst_NOWJUDGE_2P[csv.val[1]].timer = 47;
						}
					}
					else if (fBuf.left(16).isSame("#SRC_NOWCOMBO_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadSRC(&sk->src_NOWCOMBO_2P[csv.val[1]], &csv, sk);
							sk->src_NOWCOMBO_2P[csv.val[1]].timer = 47;
						}
					}
					else if (fBuf.left(16).isSame("#DST_NOWCOMBO_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadDST(&sk->dst_NOWCOMBO_2P[csv.val[1]], &csv, tSkin_num);
							sk->dst_NOWJUDGE_2P[csv.val[1]].timer = 47; //???mistake?
						}
					}
					else if (fBuf.left(16).isSame("#SRC_GROOVEGAUGE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_GROOVEGAUGE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(16).isSame("#DST_GROOVEGAUGE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_GROOVEGAUGE[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(18).isSame("#SRC_GAUGECHART_1P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_GAUGECHART_1P[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(18).isSame("#DST_GAUGECHART_1P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_GAUGECHART_1P[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(18).isSame("#SRC_GAUGECHART_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_GAUGECHART_2P[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(18).isSame("#DST_GAUGECHART_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_GAUGECHART_2P[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(15).isSame("#SRC_SCORECHART")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 2, line, pFbuf)) {
							ReadSRC(&sk->src_SCORECHART[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(15).isSame("#DST_SCORECHART")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 2, line, pFbuf)) {
							ReadDST(&sk->dst_SCORECHART[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(9).isSame("#SRC_LINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_LINE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(9).isSame("#DST_LINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_LINE[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_JUDGELINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_JUDGELINE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#DST_JUDGELINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_JUDGELINE[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(13).isSame("#SRC_BAR_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_LAMP[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(13).isSame("#DST_BAR_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_LAMP[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(16).isSame("#SRC_BAR_MY_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_MY_LAMP[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(16).isSame("#DST_BAR_MY_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_MY_LAMP[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(19).isSame("#SRC_BAR_RIVAL_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_RIVAL_LAMP[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(19).isSame("#DST_BAR_RIVAL_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_RIVAL_LAMP[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(13).isSame("#SRC_BAR_STAR")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_STAR[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(13).isSame("#DST_BAR_STAR")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_STAR[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_THUMBNAIL")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_THUMBNAIL, &csv, sk);
					}
					else if (fBuf.left(14).isSame("#DST_THUMBNAIL")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_THUMBNAIL, &csv, tSkin_num);
					}
					else if (fBuf.left(11).isSame("#SRC_README")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC_BAR_TITLE(&sk->src_README[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(11).isSame("#DST_README")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_README[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(20).isSame("#DST_EVENT_LOADINGBG")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 4, line, pFbuf)) {
							ReadDST(&sk->dst_EVENT_LOADINGBG[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(22).isSame("#SRC_EVENT_MODE_CURSOR")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_EVENT_MODE_CURSOR, &csv, sk);
					}
					else if (fBuf.left(25).isSame("#DST_EVENT_MODE_CURSOR_ON")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 10, line, pFbuf)) {
							ReadDST(&sk->dst_EVENT_MODE_CURSOR_ON[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(26).isSame("#DST_EVENT_MODE_CURSOR_OFF")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 10, line, pFbuf)) {
							ReadDST(&sk->dst_EVENT_MODE_CURSOR_OFF[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(17).isSame("#EVENT_STARTINPUT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->event_STARTINPUT[0] = csv.val[1];
						sk->event_STARTINPUT[1] = csv.val[2];
						sk->event_STARTINPUT[2] = csv.val[3];
						sk->event_STARTINPUT[3] = csv.val[4];
						sk->event_STARTINPUT[4] = csv.val[5];
						sk->event_STARTINPUT[5] = csv.val[6];
						sk->event_STARTINPUT[6] = csv.val[7];
						sk->event_STARTINPUT[7] = csv.val[8];
						sk->event_STARTINPUT[8] = csv.val[9];
						sk->event_STARTINPUT[9] = csv.val[10];
					}
					else if (fBuf.left(14).isSame("#EVENT_FADEOUT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->event_FADEOUT[0] = csv.val[1];
						sk->event_FADEOUT[1] = csv.val[2];
						sk->event_FADEOUT[2] = csv.val[3];
						sk->event_FADEOUT[3] = csv.val[4];
						sk->event_FADEOUT[4] = csv.val[5];
						sk->event_FADEOUT[5] = csv.val[6];
						sk->event_FADEOUT[6] = csv.val[7];
						sk->event_FADEOUT[7] = csv.val[8];
						sk->event_FADEOUT[8] = csv.val[9];
						sk->event_FADEOUT[9] = csv.val[10];
					}
					else if (fBuf.left(8).isSame("#LR2FONT") && !flag_skipFont) {
						SplitCSV(fBuf, &csv, ",");
						if (sk->num_of_ImageFont == 10) {
							ErrorLogFmtAdd(	"スキン読み込みエラー %d行目\n%s\nこれ以上の登録はできま せん。\n", line, fBuf);
						}
						else if (csv.val[2] == 1 || sk->disableimagefont == 0) {
							if (csv.str[1].isDiff("CONTINUE")) {
								for (int i = 0; i < sk->customfile_count; i++) {
									if(sk->customfileRANDOM[i].isSame(csv.str[1].left(sk->customfileRANDOM[i].length())) && sk->customfile[i].isDiff("RANDOM") && sk->customfile[i].isDiff("ERROR") && sk->customfile[i].length() > 0){
										csv.str[1].replace("*", sk->customfile[i]);
										//line?
										break;
									}
								}
								ReadImageFont(GetRandomFileNoError(csv.str[1], dir), &sk->ImageFonts[sk->num_of_ImageFont]);
							}
							sk->num_of_ImageFont++;
						}
						else {
							InitImageFont(&sk->ImageFonts[sk->num_of_ImageFont]);
							sk->num_of_ImageFont++;
						}
					}
					else if (fBuf.left(9).isSame("#HELPFILE")) {
						SplitCSV(fBuf, &csv, ",");
						if (sk->helpfileCount < 10) {
							sk->helpfilePath[sk->helpfileCount].assign(&csv.str[1]);
							sk->helpfileCount = sk->helpfileCount + 1;
						}
					}
					else if (fBuf.left(6).isSame("#NOBGA")) {
						sk->flag_BGA = 0;
					}
					else if (fBuf.left(11).isSame("#FLIPRESULT")) {
						sk->flag_flip = true;
						sk->op[350] = false;
						sk->op[351] = true;
					}
					else if (fBuf.left(9).isSame("#FLIPSIDE")) {
						flipside = true;
					}
					else if (fBuf.left(12).isSame("#DISABLEFLIP")) {
						sk->flag_flip = false;
						sk->op[350] = true;
						sk->op[351] = false;
					}
					else if (fBuf.left(11).isSame("#TEXTMERGIN")) {
						SplitCSV(fBuf, &csv, ",");
						sk->textmergin_1 = csv.val[1];
						sk->textmergin_2 = csv.val[2];
					}
					else if (fBuf.left(12).isSame("#SCRATCHSIDE")) {
						SplitCSV(fBuf, &csv, ",");
						sk->scratchside_1 = csv.val[1];
						sk->scratchside_2 = csv.val[2];
					}
					else if (fBuf.left(8).isSame("#INCLUDE")) {
						SplitCSV(fBuf, &csv, ",");
						for (int i = 0; i < sk->customfile_count; i++) {
							if (sk->customfileRANDOM[i].isSame(csv.str[1].left(sk->customfileRANDOM[i].length()))
								&& sk->customfile[i].isDiff("RANDOM") && sk->customfile[i].isDiff("ERROR")
								&& (sk->customfile[i].length() > 0)) {
								
								csv.str[1].replace("*", sk->customfile[i]);
								break;
							}
						}
						if (tSkin_num == 0) tSkin_num = 1;
						tSkin_num += ReadSkin(sk, GetRandomFileNoError(csv.str[1], dir), unused, tSkin_num, sku, flag_skipFont);
					}
					else if (fBuf.left(13).isSame("#CUSTOMOPTION")) {
						sk->customfile_count++;
					}
					else if (fBuf.left(11).isSame("#CUSTOMFILE")) {
						SplitCSV(fBuf, &csv, ",");
						sk->customfileRANDOM[sk->customfile_count].assign(&csv.str[2]);
						sk->customfile[sk->customfile_count].assign(&sku->customize_filename[sk->customfile_count]);
						if (sk->customfile[sk->customfile_count].isSame("RANDOM")) {
							sk->customfile[sk->customfile_count].assign( GetRandomFile(sk->customfileRANDOM[sk->customfile_count], 1) );
						}
						sk->customfile_count++;
					}
					else if (fBuf.left(13).isSame("#CUSTOMFOLDER")) {
						SplitCSV(fBuf, &csv, ",");
						sk->customfileRANDOM[sk->customfile_count].assign(&csv.str[2]);
						sk->customfile[sk->customfile_count].assign(&sku->customize_filename[sk->customfile_count]);
						sk->customfile_count++;
					}
					else if (fBuf.left(13).isSame("#RELOADBANNER")) {
						sk->reloadbanner = 1;
					}
					else if (fBuf.left(10).isSame("#SETOPTION")) {
						SplitCSV(fBuf, &csv, ",");
						if (csv.val[1] < 1000) {
							sk->op[csv.val[1]] = (csv.val[2] != 0);
						}
						else {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n#SETOPTIONの第一引数(オプション値)は900〜999の範囲内にして下さい。\n", line, fBuf);
						}
					}
					else if (fBuf.left(13).isSame("#SRC_BAR_RANK")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_RANK[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(13).isSame("#DST_BAR_RANK")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_RANK[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_BAR_RIVAL")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_RIVAL[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#DST_BAR_RIVAL")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_RIVAL[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(11).isSame("#HORIZONTAL")) {
						sk->horizontal = 1;
					}
				}
				tSkin_num++;
			}
		}
		*fBuf.atPos(0) = '\0';
	}
	fclose(pFile);
	if (skin_num == 0) {
		for (int i = sk->count; i < 100; i++) {
			DeleteGraph(sk->GrHandle[i]);
			sk->GrHandle[i] = -1;
			sk->caption[i].fillzero();
		}
		ProcessMessage();
		for (int i = 0; i < sk->count; i++) {
			if (sk->grIsMovie[i]) {
				SeekMovieToGraph(sk->GrHandle[i], 0);
				PlayMovieToGraph(sk->GrHandle[i], 3, 0);
				SetMovieVolumeToGraph(0, sk->GrHandle[i]);
			}
		}
		ErrorLogFmtAdd("スキンの読み込みに成功しました。 %s\n", FilePath);
		ErrorLogTabSub();
		SetTransColor(0, 0xff, 0);
		if (flipside != '\0') {
			ApplyFlipside(sk);
		}
		return 1;
	}
	ErrorLogFmtAdd("子スキンの読み込みに成功しました。 %s\n", FilePath);
	ErrorLogTabSub();
	return tSkin_num;
}

//4a7390
int InitSkinData(SkinManage *skm){
	skm->Max = 100;
	skm->Data = (SkinHeader *)malloc(sizeof(SkinHeader) * 100);
	skm->Count = 0;
	memset(skm->Data, 0, skm->Max * sizeof(SkinHeader));
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			skm->Data[i].customs[j].dst_op_start = 0xffffffff;
			skm->Data[i].customs[j].labelCapacity = 0;
			skm->Data[i].customs[j].op_label = (CSTR*)malloc(sizeof(CSTR)*100);
			memset(skm->Data[i].customs[j].op_label + sizeof(CSTR)*skm->Data[i].customs[j].labelCapacity, 0, sizeof(CSTR) * 100);
			skm->Data[i].customs[j].labelCapacity += 100;
		}
	}
	return 1;
}

//4a7450
int ExpandSkinMax(SkinManage *skm){
	skm->Data = (SkinHeader *)realloc(skm->Data, (skm->Max + 100) * 0xb14);
	memset(skm->Data + skm->Max, 0, sizeof(SkinHeader) * 100);
	if (skm->Max < skm->Max + 100) {
		for (int i = skm->Max; i < skm->Max + 100; i++) {
			for (int j = 0; j < 100; j++) {
				skm->Data[i].customs[j].dst_op_start = 0xffffffff;
				skm->Data[i].customs[j].labelCapacity = 0;
				skm->Data[i].customs[j].op_label = (CSTR*)malloc(sizeof(CSTR) * 100);
				memset(skm->Data[i].customs[j].op_label + sizeof(CSTR)*skm->Data[i].customs[j].labelCapacity, 0, sizeof(CSTR) * 100);
				skm->Data[i].customs[j].labelCapacity += 100;
			}
		}
	}
	skm->Max = skm->Max + 100;
	return 1;
}

//4a7520 LoadScene
int LoadScene(skstruct *sk, CSTR skinfile, int p5, char font) {
	SkinUser tsku;
	CSTR tStr;
	InitSkin(sk, p5, font);
	sk->skinMD5.assign(MD5str(skinfile));
	cstrSprintf(&tStr, "LR2files\\SkinCustomize\\%s.xml",sk->skinMD5.body);
	ReadSkinCustomize(&tsku, tStr);
	(sk->adjust).shift_x = tsku.adjust.shift_x;
	(sk->adjust).shift_y = tsku.adjust.shift_y;
	(sk->adjust).rate_x = tsku.adjust.rate_x;
	(sk->adjust).rate_y = tsku.adjust.rate_y;
	(sk->adjust).judge_x = tsku.adjust.judge_x;
	(sk->adjust).judge_y = tsku.adjust.judge_y;
	(sk->adjust).size_x = tsku.adjust.size_x;
	(sk->adjust).size_y = tsku.adjust.size_y;
	(sk->adjust).dark_type = tsku.adjust.dark_type;
	(sk->adjust).note_1p_x = tsku.adjust.note_1p_x;
	(sk->adjust).note_1p_y = tsku.adjust.note_1p_y;
	(sk->adjust).note_2p_x = tsku.adjust.note_2p_x;
	(sk->adjust).note_2p_y = tsku.adjust.note_2p_y;
	for (int i = 0; i < 20; i++) {
		int t = tsku.customize_value[i * 5];
		if (t > 899 && t < 1000) sk->op[t] = 1;
		t = tsku.customize_value[i * 5 + 1];
		if (t > 899 && t < 1000) sk->op[t] = 1;
		t = tsku.customize_value[i * 5 + 2];
		if (t > 899 && t < 1000) sk->op[t] = 1;
		t = tsku.customize_value[i * 5 + 3];
		if (t > 899 && t < 1000) sk->op[t] = 1;
		t = tsku.customize_value[i * 5 + 4];
		if (t > 899 && t < 1000) sk->op[t] = 1;
	}
	return ReadSkin(sk, skinfile, p5, 0, &tsku, font);
}

//4a77a0 ParsLR2SkinCustom
int ParseLR2SkinCustom(SkinManage *skm, CSTR filepath) {
	CSVbuf csvBuf;
	SkinUser skCustom;
	CSTR buffer = CSTR(260);
	CSTR md5Filepath;
	CSTR fName;
	FILE *pFile;
	char* pBuffer;

	cstrSprintf(&md5Filepath, "LR2files\\SkinCustomize\\%s.xml", MD5str(filepath) );
	ReadSkinCustomize(&skCustom, md5Filepath);
	pFile = fopen(filepath, "r");
	if (!pFile) return 0;

	pBuffer = buffer.outstr();
	for (pBuffer = fgets(pBuffer,256,pFile) ; pBuffer ; pBuffer = fgets(pBuffer, 256, pFile)) {
		buffer.trimWhiteSpace();
		DealWhiteSpace(&buffer);
		if (buffer.left(12).isSame("#INFORMATION")) {
			SplitCSV(buffer, &csvBuf, ",");
			if (csvBuf.val[1] < 0) {
				fclose(pFile);
				return 0;
			}
			skm->Data[skm->Count].type = (SKINTYPE)csvBuf.val[1];
			skm->Data[skm->Count].skinFile.assign( &filepath);
			skm->Data[skm->Count].title.assign(&csvBuf.str[2]);
			skm->Data[skm->Count].maker.assign(&csvBuf.str[3]);
			skm->Data[skm->Count].thumbnail.assign(&csvBuf.str[4]);
			skm->Data[skm->Count].informationP5 = csvBuf.val[5];
			skm->Data[skm->Count].undefined2 = -1; //TODO:rename
			skm->Count ++;
			if (skm->Count == skm->Max) {
				ExpandSkinMax(skm);
			}
		}
		else if (buffer.left(13).isSame("#CUSTOMOPTION")) {
			SplitCSV(buffer, &csvBuf, ",");
			if (csvBuf.val[2] < 1000) {
				SkinHeader &rSkin = skm->Data[skm->Count - 1];
				SkinCustom &rCustom = rSkin.customs[rSkin.custom_count];

				rCustom.title.assign(&csvBuf.str[1]);
				rCustom.dst_op_start = csvBuf.val[2];
				while (csvBuf.str[rCustom.dst_op_count + 3].length() > 0) {
					rCustom.op_label[rCustom.dst_op_count].assign(&csvBuf.str[rCustom.dst_op_count + 3]);
					rCustom.dst_op_count++;
				}

				if (skCustom.customize_value[rSkin.custom_count] < rCustom.dst_op_start	|| rCustom.dst_op_start + rCustom.dst_op_count <= skCustom.customize_value[rSkin.custom_count]) 
					skCustom.customize_value[rSkin.custom_count] = rCustom.dst_op_start;

				skCustom.customize_filename[rSkin.custom_count].assign("ERROR");
				rSkin.custom_count++;
			}
		}
		else if (buffer.left(11).isSame("#CUSTOMFILE")) {
			FILE *pFile;
			HANDLE hFindFile, hFindFileOld;
			_WIN32_FIND_DATAA findFileData;
			int flag;
			SkinHeader &rSkin = skm->Data[skm->Count - 1];
			SkinCustom &rCustom = rSkin.customs[rSkin.custom_count];
			
			SplitCSV(buffer, &csvBuf, ",");
			rCustom.title.assign(&csvBuf.str[1]);
			rCustom.dst_op_start = 0;
			hFindFile = FindFirstFileA(csvBuf.str[2], &findFileData);
			do {
				flag = 0; //logic arranged
				if ( (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) 	flag = 2;
				else if (strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName)) flag = 1;

				fName.assign(findFileData.cFileName);
				if (fName.right(3).isDiff("txt") && flag) {
					if (flag == 2) fName.nullAtPos( fName.findStrPos(".") );
					rCustom.op_label[rCustom.dst_op_count].assign(&fName);
					rCustom.dst_op_count++;
				}
				if (rCustom.dst_op_count == rCustom.labelCapacity-2) {
					rCustom.op_label = (CSTR*)realloc( rCustom.op_label, sizeof(CSTR)*(rCustom.labelCapacity + 100) );
					memset(&rCustom.op_label[rCustom.labelCapacity],0,sizeof(CSTR)*100);
					rCustom.labelCapacity += 100;
				}
			} while (FindNextFileA(hFindFile,&findFileData));
			FindClose(hFindFile);
			rCustom.op_label[rCustom.dst_op_count].assign("RANDOM");
			rCustom.dst_op_count++;

			bool a = true;
			if (rCustom.dst_op_count < 1) {
				skCustom.customize_filename[rSkin.custom_count].assign(&csvBuf.str[3]);
			}
			else {
				for (int i = 0; i < rCustom.dst_op_count; i++) {
					if (rCustom.op_label[i].isSame(&skCustom.customize_filename[rSkin.custom_count]))
						a = false;
				}
				if(a) skCustom.customize_filename[rSkin.custom_count].assign(&csvBuf.str[3]);
			}
			skCustom.customize_value[rSkin.custom_count] = 0;
			rSkin.custom_count++;
		}
		else if (buffer.left(11).isSame("#ENDOFHEADER")) {
			break;
		}
		pBuffer = buffer.outstr();
	}
	fclose(pFile);
	WriteSkinCustomizeXml(&skCustom, md5Filepath);
	return 0;
}

//4a8230 MakeSkinList
int MakeSkinList(SkinManage *skm, CSTR dir) {
	HANDLE hFindFile, hFindFileOld;
	_WIN32_FIND_DATAA findFileData;
	CSTR filter;
	bool isLR2Skin;
	if (dir.right(1).isDiff("\\")) 
		dir.add("\\");
	
	filter.assign(&dir).add("*");
	hFindFile = FindFirstFileA(filter, &findFileData);
	do{
		if ( (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			filter.assign(&dir).add(findFileData.cFileName);
			isLR2Skin = false;
			if (filter.right(8).isSame(".lr2skin"))		isLR2Skin = true;
			else if (filter.right(6).isSame(".lr2ss"))	isLR2Skin = true;
			
			if (isLR2Skin) ParseLR2SkinCustom(skm, filter); 
		}
		else {  //logic arranged
		if (strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName)) {
			filter.assign(&dir).add(findFileData.cFileName).add("\\");
			MakeSkinList(skm, filter);
		}
		}
		hFindFileOld = hFindFile;
		if (FindNextFileA(hFindFile, &findFileData) == 0) {
			FindClose(hFindFileOld);
			return 0;
		}
	} while (true);
}


////PLAYSCORE
//4a8550
PLAYSCORE::PLAYSCORE() {
	name.fillzero();
	totalnotes = 0;
	nownote = 0;
	exscore = 0;
	rate = 0;
	judgecount = 0;
	judgeExpect[0] = 0;
	judge[0] = 0;
	judgeExpect[1] = 0;
	judge[1] = 0;
	judgeExpect[2] = 0;
	judge[2] = 0;
	judgeExpect[3] = 0;
	judge[3] = 0;
	judgeExpect[4] = 0;
	judge[4] = 0;
	judgeExpect[5] = 0;
	judge[5] = 0;
	ghostReadCount = 0;
	judge_queue_count = 0;
	judge_queue = NULL;
}

//4a8600
int PLAYSCORE::InitJudgeQueue(void){
	this->name.fillzero();
	this->totalnotes = 0;
	this->nownote = 0;
	this->exscore = 0;
	this->rate = 0;
	this->judgecount = 0;
	this->judgeExpect[0] = 0;
	this->judge[0] = 0;
	this->judgeExpect[1] = 0;
	this->judge[1] = 0;
	this->judgeExpect[2] = 0;
	this->judge[2] = 0;
	this->judgeExpect[3] = 0;
	this->judge[3] = 0;
	this->judgeExpect[4] = 0;
	this->judge[4] = 0;
	this->judgeExpect[5] = 0;
	this->judge[5] = 0;
	this->ghostReadCount = 0;
	this->judge_queue_count = 0;
	if (this->judge_queue != NULL) {
		free(this->judge_queue);
	}
	this->judge_queue = (char *)0x0;
	this->judge_queue_count = 0;
	return 0;
}

//4a8660
int PLAYSCORE::ResetJudgeQueue(int size){
	char *pcVar1;
	int iVar2;

	if (this->judge_queue != (char *)0x0) {
		free(this->judge_queue);
	}
	this->judge_queue = (char *)0x0;
	this->judge_queue_count = 0;
	this->judge_queue = (char *)malloc(size);
	for (int i = 0; i < size; i++) {
		this->judge_queue[i] = -1;
	}
	this->judge_queue_count = size;
	return 1;
}

//4a86c0
int PLAYSCORE::ResizeJudgeQueue(size_t size){
	char *pcVar1;
	int iVar2;

	if (this->judge_queue == (char *)0x0) {
		this->judge_queue = (char *)0x0;
		this->judge_queue_count = 0;
		this->judge_queue = (char *)malloc(1000);
		for (int i = 0; i < 1000; i++) {
			this->judge_queue[i] = -1;
		}
		this->judge_queue_count = 1000;
	}
	this->judge_queue = (char *)realloc(this->judge_queue, size);
	for (int i = this->judge_queue_count; i < this->judge_queue_count + size; i++) {
		this->judge_queue[i] = -1;
	}
	this->judge_queue_count = this->judge_queue_count + size;
	return 1;
}

//4a8740
int PLAYSCORE::AddJudgeQueue(char judge){
	if (this->judgecount == this->judge_queue_count) {
		ResizeJudgeQueue(1000);
	}
	this->judge_queue[this->judgecount] = judge;
	this->judgecount = this->judgecount + 1;
	return 1;
}

//4a8770 //TODO:rename variabale
int PLAYSCORE::DealJudgeFromQueue(void){
	byte val;
	int note;
	int ret;

	if (this->totalnotes < 1) {
		return 1;
	}
	if (this->judge_queue_count != 0) {
		note = this->judgecount;
		ret = 1;
		if ((note != this->judge_queue_count) && (val = this->judge_queue[note], val != 0xff)) {
			if ((val != 0) && (val < 6)) {
				this->judge[val]++;
				this->judgecount++;
				this->nownote++;
				this->rate = this->judge[4] + this->judge[5] * 2;
				return ret;
			}
			if (val == 0) {
				this->judge[1]++;
				this->rate = this->judge[4] + this->judge[5] * 2;
				this->judgecount++;
				ret = 0;
			}
		}
		return ret;
	}
	if (this->nownote > 0) {
		this->judge[0] = ((this->nownote + 1) * this->judgeExpect[0]) / this->totalnotes;
	}
	if (this->nownote > 0) {
		this->judge[1] = ((this->nownote + 1) * this->judgeExpect[1]) / this->totalnotes;
	}
	if (this->nownote > 0) {
		this->judge[2] = ((this->nownote + 1) * this->judgeExpect[2]) / this->totalnotes;
	}
	if (this->nownote > 0) {
		this->judge[3] = ((this->nownote + 1) * this->judgeExpect[3]) / this->totalnotes;
	}
	if (this->nownote > 0) {
		this->judge[4] = ((this->nownote + 1) * this->judgeExpect[4]) / this->totalnotes;
	}
	if (this->nownote > 0) {
		this->judge[5] = ((this->nownote + 1) * this->judgeExpect[5]) / this->totalnotes;
	}
	this->rate = this->judge[4] + this->judge[5] * 2;
	this->nownote++;
	return 1;
}

//4a8870
char PLAYSCORE::GetJudgeFromQueue(void){
	if (this->judge_queue_count == 0) {
		return -1;
	}
	if (this->judgecount <= 0) {
		return *this->judge_queue;
	}
	return this->judge_queue[this->judgecount + -1];
}

//4a88a0
int PLAYSCORE::SetDefaultGhost(int target, int notes){
	int tgPerfect;

	ErrorLogFmtAdd("デフォルトゴーストを設定します...");
	InitJudgeQueue();
	this->totalnotes = notes;
	if (target == 88) {
		tgPerfect = ((double)notes * 7.0) / 9.0;
		this->judgeExpect[5] = tgPerfect;
		this->judgeExpect[4] = notes - tgPerfect;
		this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
		this->name = "RANK AAA";
	}
	else if (target == 77) {
		tgPerfect = ((double)notes * 5.0) / 9.0;
		this->judgeExpect[5] = tgPerfect;
		this->judgeExpect[4] = notes - tgPerfect;
		this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
		this->name = "RANK AA";
	}
	else if (target == 66) {
		tgPerfect = ((double)notes * 3.0) / 9.0;
		this->judgeExpect[5] = tgPerfect;
		this->judgeExpect[4] = notes - tgPerfect;
		this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
		this->name = "RANK A";
	}
	else {
		if (50 <= target && target <= 100) {
			tgPerfect = (((double)target + (double)target) - 100.0) * (double)notes;
			tgPerfect = tgPerfect / 100;
			this->judgeExpect[4] = notes - tgPerfect;
			this->judgeExpect[5] = tgPerfect;
			this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
			cstrSprintf(&this->name, "%d%%", target);
		}
		else {
			this->name = "NO TARGET";
		}
	}
	ErrorLogFmtAdd("ゴーストを設定しました\n");
	return 1;
}

//4a89d0
int PLAYSCORE::SetGhost(int exscore, int notes, CSTR name){

	ErrorLogFmtAdd("exスコアからゴーストを設定します...");
	InitJudgeQueue();
	this->totalnotes = notes;
	this->name = name;
	this->judgeExpect[5] = RoundUp(((double)exscore / (double)notes - 1.0) * (double)this->totalnotes);
	this->judgeExpect[4] = this->totalnotes - this->judgeExpect[5];
	this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
	ErrorLogFmtAdd("ゴーストを設定しました\n");
	return 1;
}

//TOFIX : ghostdata last note problem
//4a8a90 
CSTR PLAYSCORE::EncodeGhostData(void) {
	
	if (this->judgecount == 0) return "GHOST_ERROR";

	int rep = 0;
	CSTR buf;
	CSTR data(this->judgecount + 3);
	char last = this->judge_queue[0];
	for (int i = 0; i < judgecount; i++) {
		if (last == this->judge_queue[i]) {
			rep++;
		}
		else {
			switch (last) {
				case 0:
					data.add("@");
					break;
				case 1:
					data.add("A");
					break;
				case 2:
					data.add("B");
					break;
				case 3:
					data.add("C");
					break;
				case 4:
					data.add("D");
					break;
				case 5:
					data.add("E");
					break;
				default:
					data.add("-");
					break;
			}
			if (rep >= 2) {
				cstrSprintf(&buf, "%d", rep);
				data.add(buf);
			}
			last = this->judge_queue[i];
			rep = 1;
		}
	}

	switch (last) {
		case 0:
			data.add("@");
			break;
		case 1:
			data.add("A");
			break;
		case 2:
			data.add("B");
			break;
		case 3:
			data.add("C");
			break;
		case 4:
			data.add("D");
			break;
		case 5:
			data.add("E");
			break;
		default:
			data.add("-");
			break;
	}
	if (rep >= 2) {
		cstrSprintf(&buf, "%d", rep);
		data.add(buf);
	}
	data.add("ZZZ");

	int o = 0;
	CSTR str(data.length() + 3);
	for (int i = 1; i < data.length(); i++) {
		if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '1') {
			str.add("F");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '2') {
			str.add("G");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '3') {
			str.add("H");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '4') {
			str.add("I");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '5') {
			str.add("J");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '6') {
			str.add("K");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '7') {
			str.add("L");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '8') {
			str.add("M");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '9') {
			str.add("N");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == 'C') {
			str.add("P");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == 'B') {
			str.add("Q");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == 'A') {
			str.add("R");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '2') {
			str.add("S");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '3') {
			str.add("T");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '4') {
			str.add("U");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '5') {
			str.add("V");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '6') {
			str.add("W");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == 'E') {
			str.add("X");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == 'C') {
			str.add("Y");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == 'B') {
			str.add("a");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == 'A') {
			str.add("b");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == '2') {
			str.add("c");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == '3') {
			str.add("d");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == '4') {
			str.add("e");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == '5') {
			str.add("f");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == 'E') {
			str.add("g");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == 'D') {
			str.add("h");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == 'B') {
			str.add("i");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == 'A') {
			str.add("j");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == 'B') {
			str.add("k");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == 'C') {
			str.add("l");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == 'D') {
			str.add("m");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == 'E') {
			str.add("n");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == '2') {
			str.add("o");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == '3') {
			str.add("p");
			i++;
		}
		else {
			*str.atPos(o) = *data.atPos(i - 1);
		}
		o++;
	}

	data = str;
	str.fillzero();
	str.resize2(data.length() + 3);
	o = 0;
	for (int i = 1; i < data.length(); i++) {
		if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == 'X') {
			str.add("q");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '1') {
			str.add("r");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '2') {
			str.add("s");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '3') {
			str.add("t");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '4') {
			str.add("u");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '5') {
			str.add("v");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '6') {
			str.add("w");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '7') {
			str.add("x");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '8') {
			str.add("y");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '9') {
			str.add("z");
			i++;
		}
		else {
			*str.atPos(o) = *data.atPos(i - 1);
		}

		o++;
	}

	this->name.fillzero();
	this->totalnotes = 0;
	this->nownote = 0;
	this->exscore = 0;
	this->rate = 0;
	this->judgecount = 0;
	this->judgeExpect[0] = 0;
	this->judge[0] = 0;
	this->judgeExpect[1] = 0;
	this->judge[1] = 0;
	this->judgeExpect[2] = 0;
	this->judge[2] = 0;
	this->judgeExpect[3] = 0;
	this->judge[3] = 0;
	this->judgeExpect[4] = 0;
	this->judge[4] = 0;
	this->judgeExpect[5] = 0;
	this->judge[5] = 0;
	this->ghostReadCount = 0;
	this->judge_queue_count = 0;
	if (this->judge_queue != NULL) {
		free(this->judge_queue);
	}
	this->judge_queue = NULL;
	this->judge_queue_count = 0;

	return str;
}

//4a96b0
int PLAYSCORE::DecodeGhostData(CSTR data) {
	
	CSTR decode;

	for (int i = 0; i < data.length(); i++) {
		switch (*data.atPos(i)) {
			case 'q':
				decode.add("XX");
				break;
			case 'r':
				decode.add("X1");
				break;
			case 's':
				decode.add("X2");
				break;
			case 't':
				decode.add("X3");
				break;
			case 'u':
				decode.add("X4");
				break;
			case 'v':
				decode.add("X5");
				break;
			case 'w':
				decode.add("X6");
				break;
			case 'x':
				decode.add("X7");
				break;
			case 'y':
				decode.add("X8");
				break;
			case 'z':
				decode.add("X9");
				break;
			default:
				decode.add(data.getSliced(i, 1));
				break;
		}
	}
	data = decode;
	decode.fillzero();
	for (int i = 0; i < data.length(); i++) {
		switch (*data.atPos(i)) {
			case 'F':
				decode.add("E1");
				break;
			case 'G':
				decode.add("E2");
				break;
			case 'H':
				decode.add("E3");
				break;
			case 'I':
				decode.add("E4");
				break;
			case 'J':
				decode.add("E5");
				break;
			case 'K':
				decode.add("E6");
				break;
			case 'L':
				decode.add("E7");
				break;
			case 'M':
				decode.add("E8");
				break;
			case 'N':
				decode.add("E9");
				break;
			case 'P':
				decode.add("EC");
				break;
			case 'Q':
				decode.add("EB");
				break;
			case 'R':
				decode.add("EA");
				break;
			case 'S':
				decode.add("D2");
				break;
			case 'T':
				decode.add("D3");
				break;
			case 'U':
				decode.add("D4");
				break;
			case 'V':
				decode.add("D5");
				break;
			case 'W':
				decode.add("D6");
				break;
			case 'X':
				decode.add("DE");
				break;
			case 'Y':
				decode.add("DC");
				break;
			case 'a':
				decode.add("DB");
				break;
			case 'b':
				decode.add("DA");
				break;
			case 'c':
				decode.add("C2");
				break;
			case 'd':
				decode.add("C3");
				break;
			case 'e':
				decode.add("C4");
				break;
			case 'f':
				decode.add("C5");
				break;
			case 'g':
				decode.add("CE");
				break;
			case 'h':
				decode.add("CD");
				break;
			case 'i':
				decode.add("CB");
				break;
			case 'j':
				decode.add("CA");
				break;
			case 'k':
				decode.add("AB");
				break;
			case 'l':
				decode.add("AC");
				break;
			case 'm':
				decode.add("AD");
				break;
			case 'n':
				decode.add("AE");
				break;
			case 'o':
				decode.add("A2");
				break;
			case 'p':
				decode.add("A3");
				break;
			default:
				decode.add(data.getSliced(i, 1));
				break;
		}
	}
	data = decode;
	decode.fillzero();
	int rep = -1;
	int pos = 0;
	for (int i = 0; i < data.length(); i++) {
		if (*data.atPos(i) >= 0x40 && *data.atPos(i) <= 0x45) {
			if (rep >= 0) {
				if (rep == 0) rep = 1;

				while (rep > 0) {
					decode.add(data.getSliced(pos, 1));
					rep--;
				}
			}
			rep = 0;
			pos = i;
		}
		else if (*data.atPos(i) >= '0' && *data.atPos(i) <= '9') {
			if (rep == 0) rep = *data.atPos(i) - 0x30;
			else rep = rep*10 + (*data.atPos(i) - 0x30);
		}

	}
	while(rep > 0) {
		decode.add(data.getSliced(pos, 1));
		rep--;
	}

	this->name.fillzero();
	this->totalnotes = 0;
	this->nownote = 0;
	this->exscore = 0;
	this->rate = 0;
	this->judgecount = 0;
	this->judgeExpect[0] = 0;
	this->judge[0] = 0;
	this->judgeExpect[1] = 0;
	this->judge[1] = 0;
	this->judgeExpect[2] = 0;
	this->judge[2] = 0;
	this->judgeExpect[3] = 0;
	this->judge[3] = 0;
	this->judgeExpect[4] = 0;
	this->judge[4] = 0;
	this->judgeExpect[5] = 0;
	this->judge[5] = 0;
	this->ghostReadCount = 0;
	this->judge_queue_count = 0;
	if (this->judge_queue != NULL) {
		free(this->judge_queue);
	}
	this->judge_queue = NULL;
	this->judge_queue_count = 0;
	if (this->judge_queue != NULL) {
		free(this->judge_queue);
	}
	this->judge_queue = NULL;
	this->judge_queue_count = 0;
	this->judge_queue = (char *)malloc(decode.length() + 1);

	for (int i = 0; i < decode.length() + 1; i++) {
		this->judge_queue[i] = -1;
	}

	this->judge_queue_count = decode.length() + 1;

	for (int i = 0; i < decode.length(); i++) {
		this->judge_queue[i] = *decode.atPos(i) + -0x40;
		this->ghostReadCount++;
	}

	for (int i = 0; i < this->ghostReadCount; i++) {
		char judge = this->judge_queue[i];
		if (judge == 0) {
			this->judgeExpect[1]++;
		}
		else if ((judge != 0) && (judge < 6)) {
			this->judgeExpect[(byte)this->judge_queue[i]]++;
			this->totalnotes++;
		}
	}

	this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
	ErrorLogFmtAdd("ゴーストデータ復元完了 totalnotes %d exscore_max %d\n", this->totalnotes, this->exscore);

	return 1;
}

//4a9eb0
int PLAYSCORE::SetScore(PLAYERSTATUS *pstat, char flagExpect) {
	if (this->totalnotes <= 0) {
		ErrorLogAdd("SetScore::TOTALNOTESが0です\n");
		return 1;
	}
	if (flagExpect) {
		pstat->judgecount[0] = this->judgeExpect[0];
		pstat->judgecount[1] = this->judgeExpect[1];
		pstat->judgecount[2] = this->judgeExpect[2];
		pstat->judgecount[3] = this->judgeExpect[3];
		pstat->judgecount[4] = this->judgeExpect[4];
		pstat->judgecount[5] = this->judgeExpect[5];
		pstat->exscore = this->exscore;
		pstat->score = ((pstat->judgecount[3] + (pstat->judgecount[4] + pstat->judgecount[5] * 2) * 2) * 50000) / this->totalnotes;
		pstat->now_combo = this->totalnotes;
		pstat->rate = (double)((pstat->exscore * 100) / (this->totalnotes * 2));
		return 1;
	}
	pstat->judgecount[0] = this->judge[0];
	pstat->judgecount[1] = this->judge[1];
	pstat->judgecount[2] = this->judge[2];
	pstat->judgecount[3] = this->judge[3];
	pstat->judgecount[4] = this->judge[4];
	pstat->judgecount[5] = this->judge[5];
	pstat->exscore = this->rate;
	pstat->score = ((pstat->judgecount[3] + (pstat->judgecount[4] + pstat->judgecount[5] * 2) * 2) * 50000) / this->totalnotes;
	pstat->now_combo = this->nownote;
	pstat->rate = (double)((pstat->exscore * 100) / (this->totalnotes * 2));
	return 1;
}



////
//4a9fd0
bool isVisibleNote(int ch){
	if (10 <= ch && ch < 30) {
		return true;
	}
	return (50 <= ch && ch < 70);
}


int hBackImage;
//4a9ff0
int SetBackground(int hImage) {
	hBackImage = hImage;
	return 1;
}

//4aa000
int InitBMSMETA(BMSMETA *meta) {
	meta->title.fillzero();
	meta->subtitle.fillzero();
	meta->artist.fillzero();
	meta->subartist.fillzero();
	meta->hash.fillzero();
	meta->filename.fillzero();
	meta->stagefilepath.fillzero();
	meta->bannerpath.fillzero();
	meta->parentfolderpath.fillzero();
	meta->folderpath.fillzero();
	meta->tag.fillzero();
	meta->backBMPpath.fillzero();
	meta->filepath.fillzero();
	meta->selLevel = 0;
	meta->exlevel = 0;
	meta->notecount = 0;
	meta->maxbpm = 0;
	meta->minbpm = 0;
	meta->longnote = 0;
	meta->random = 0;
	meta->bga = 0;
	meta->hasTxt = 0;
	meta->keymode = 5;
	meta->difficulty = -1;
	meta->judge = 2;
	return 1;
}

//4aa0b0
int ParseBMSMETA(BMSMETA *meta, CSTR filepath, char flag) {
	FILE *pFile;
	float notes;
	int lnobj = -12345;
	bool flagIf;

	if (flag) {
		printfDx("Now Loading...\n%s\n",filepath);
		if (hBackImage > 0) {
			DrawGraph(0, 0, hBackImage, 0);
		}
		ScreenFlip();
		ClsDrawScreen();
		clsDx();
		ProcessMessage();
	}

	InitBMSMETA(meta);
	notes = 0.0;
	flagIf = 0;
	pFile = fopen(filepath, "r");
	if (pFile == NULL) return 0;

	CSTR dir(filepath.getDirectory()); // check this works as intended

	if (filepath.right(4).isSame(".pms")) meta->keymode = 9;

	CSTR buffer(102401);
	char* pBuffer = buffer.outstr();
	for (pBuffer = fgets(pBuffer, 102400, pFile); pBuffer; pBuffer = fgets(pBuffer, 102400, pFile)) {

		if (buffer.left(3).isSame("#IF") && buffer.left(5).isDiff("#IF 1")) {
			flagIf = 1;
		}
		else if (buffer.left(6).isSame("#ENDIF")) {
			flagIf = 0;
		}
		else if (buffer.left(1).isDiff("#") || flagIf) {
			pBuffer = buffer.atPos(0);
		}
		else {
			buffer.trimWhiteSpace();
			DealWhiteSpace(&buffer);
			if (GetDifficulty(&buffer, CSTR("#TITLE"), &meta->title, &meta->subtitle, &meta->difficulty)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#GENRE"), &meta->genre)) {
				CSTR none;
				GetDifficulty(&buffer, CSTR("#GENRE"), &none, &none, &meta->difficulty);
			}
			else if (GetStringBodyStr(&buffer, CSTR("#CATEGORY"), &meta->genre)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#SUBTITLE"), &meta->subtitle)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#ARTIST"), &meta->artist)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#SUBARTIST"), &meta->subartist)) {}
			else if (GetStringBodyInt(&buffer, CSTR("#PLAYLEVEL"), &meta->selLevel)) {}
			else if (GetStringBodyInt(&buffer, CSTR("#EXLEVEL"), &meta->exlevel)) {}
			else if (GetStringBodyInt(&buffer, CSTR("#MAXTRACKS"), &meta->selLevel)) {}
			else if (GetStringBodyInt(&buffer, CSTR("#DIFFICULTY"), &meta->difficulty)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#INFORMATION_A"), &meta->artist)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#INFORMATION_B"), &meta->subartist)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#TAG"), &meta->tag)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#COMMAND"), &meta->tag)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#STAGEFILE"), &meta->stagefilepath)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#BANNER"), &meta->bannerpath)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#BACKBMP"), &meta->backBMPpath)) {}
			else if (buffer.left(13).isSame("#CUSTOMFOLDER")) {
				meta->judge = 2;
			}
			else if (buffer.left(7).isSame("#FP/DSC")) {
				meta->keymode = 9;
			}
			else if (GetStringBodyInt(&buffer, CSTR("#RANK"), &meta->judge)) {}
			else if (buffer.left(6).isSame("#LNOBJ")) {
				meta->longnote = 1;
				lnobj = Base36ToInt(*buffer.atPos(7), *buffer.atPos(8));
			}
			else if (buffer.left(4).isSame("#BMP")) {
				meta->bga = 1;
			}
			else if (buffer.left(5).isSame("#RAND")) {
				meta->random = 1;
			}
			else if (buffer.left(4).isSame("#BPM")) {
				int bpm;
				if (buffer.left(5).isSame("#BPM ")) {
					bpm = atol(buffer.right(buffer.length() - 5));
				}
				else {
					bpm = atol(buffer.right(buffer.length() - 7));
				}

				if (bpm > 0) {
					if (meta->maxbpm == 0) {
						meta->maxbpm = bpm;
						meta->minbpm = bpm;
					}
					else if (bpm > meta->maxbpm) {
						meta->maxbpm = bpm;
					}
					else if (bpm < meta->minbpm) {
						meta->minbpm = bpm;
					}
				}
			}
			else if( isdigit(*(buffer.atPos(1))) && isdigit(*(buffer.atPos(2))) && isdigit(*(buffer.atPos(3))) && isdigit(*(buffer.atPos(4))) && isdigit(*(buffer.atPos(5))) ){
				//TOFIX : There is no need to decrease variable data, still it does. it makes karinote include invisible and longnote(0.5 to 1)
				int data = atol(buffer.getSliced(4, 2));
				if (51 <= data && data <= 69) {
					data -= 40;
					meta->longnote = 1;
				}
				if (31 <= data && data <= 49) {
					data -= 20;
				}
				if (meta->keymode != 9) {
					if (data == 18 || data == 19 || data == 38 || data == 39 || data == 58 || data == 59) {
						if (meta->keymode == 5) meta->keymode = 7;
						if (meta->keymode == 10) meta->keymode = 14;
					}
					else if (data == 28 || data == 29 || data == 48 || data == 49 || data == 68 || data == 69) {
						meta->keymode = 14;
					}
					else if ((data > 20 && data < 30) || (data > 40 && data < 50) || (61 <= data && data < 70)) {
						if (meta->keymode == 5) meta->keymode = 10;
						else if(meta->keymode == 7) meta->keymode = 14;	
					}
				}
				
				if (data == 3) {
					int c = (buffer.length() - 7) / 2;
					for (int cur = 7, i = 0; i < c; cur += 2, i++) {
						int d = HEXcharToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1));
						if (d > 0) {
							if (meta->maxbpm == 0) {
								meta->maxbpm = d;
								meta->minbpm = d;
							}
							else if (d > meta->maxbpm) {
								meta->maxbpm = d;
							}
							else if (d < meta->minbpm) {
								meta->minbpm = d;
							}
						}
					}
				}
				else if (11 <= data && data <= 29) {
					int c = (buffer.length() - 7) / 2;
					for (int cur = 7, i = 0; i < c; cur += 2, i++) {
						if (Base36ToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1)) && Base36ToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1)) != lnobj ) {
							notes++;
						}
					}
				}
				else if (51 <= data && data <= 69) {
					int c = (buffer.length() - 7) / 2;
					for (int cur = 7, i = 0; i < c; cur += 2, i++) {
						if (Base36ToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1)) && Base36ToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1)) != lnobj ) {
							notes += 0.5;
						}
					}
				}
				
			}
		}
	}

	fclose(pFile);
	if (meta->difficulty == -1) {
		CSTR sdsd(meta->filepath);
		sdsd.left(sdsd.length() - 4); //after all, not used
		if (sdsd.left(2).isSame("_a")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("_h")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("_m")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("_e")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("_b")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("_l")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_sa")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("_sh")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("_sm")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("_se")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_sb")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_sl")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_da")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("_dh")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("_dm")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("_de")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_db")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_dl")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_ex")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("7a")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("7h")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("7m")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("7e")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("7b")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("7l")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("a7")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("h7")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("m7")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("e7")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("n7")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("l7")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("5a")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("5h")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("5m")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("5e")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("5b")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("5l")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("a5")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("h5")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("m5")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("e5")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("n5")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("l5")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("a14")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("h14")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("m14")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("e14")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("b14")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("l14")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("a10")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("h10")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("m10")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("e10")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("b10")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("l10")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("14a")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("14h")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("14m")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("14e")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("14b")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("10l")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("10a")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("10h")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("10m")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("10e")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("10b")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("10l")) meta->difficulty = 1;
	} // TOFIX: meta->filepath is not loaded yet, so above this is useless hahahaha
	//TOFIX: find difficulty at left???
	makeFileHash(filepath, meta->hash); //test : CSTR to char* as oBuf : possible
	meta->parentfolderpath = filepath.getParaentDirectory();
	meta->filepath = filepath;
	meta->filename = filepath.getFilename();
	meta->folderpath = filepath.getDirectory();
	dir.add("*.txt");
	if (IsFileExist(dir)) meta->hasTxt = 1;
	meta->notecount = notes;
	return 1;
}

//4ac140
int InitNoteBuffer(LaneStruct *lane, int count){

	lane->count = count;
	lane->notes = (NoteStruct *)malloc(count * sizeof(NoteStruct));
	
	lane->size = 0;
	lane->autoplay = 0;

	for (int i = 0; i < lane->count; i++) {
		lane->notes[i].bmsTiming_ln = -1.0;
		lane->notes[i].realTiming_ln = -1.0;
		lane->notes[i].active = -1;
		lane->notes[i].val = -1.0;
		lane->notes[i].bmsTiming = -1.0;
		lane->notes[i].realTiming = -1.0;
		lane->notes[i].op = -1;
		lane->notes[i].channel = -1;
	}
	return 1;
}


//4ac1c0
int ExpandNoteBuffer(LaneStruct *lane, int addsize){

	int oldCount;

	oldCount = lane->count;
	lane->count += addsize;
	lane->notes = (NoteStruct*)realloc(lane->notes, (lane->count) * sizeof(NoteStruct));

	for (int i = oldCount; i < lane->count; i++) {
		lane->notes[i].bmsTiming_ln = -1.0;
		lane->notes[i].realTiming_ln = -1.0;
		lane->notes[i].active = -1;
		lane->notes[i].val = -1.0;
		lane->notes[i].bmsTiming = -1.0;
		lane->notes[i].realTiming = -1.0;
		lane->notes[i].op = -1;
		lane->notes[i].channel = -1;
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
	if (gp->bmsobj.size == 0) InitNoteBuffer(&gp->bmsobj, 1000);
	for (int i = 0; i < gp->bmsobj.count; i++) {
		gp->bmsobj.notes[i].bmsTiming_ln = -1.0;
		gp->bmsobj.notes[i].realTiming_ln = -1.0;
		gp->bmsobj.notes[i].active = -1;
		gp->bmsobj.notes[i].val = -1.0;
		gp->bmsobj.notes[i].bmsTiming = -1.0;
		gp->bmsobj.notes[i].realTiming = -1.0;
		gp->bmsobj.notes[i].op = -1;
		gp->bmsobj.notes[i].channel = -1;
		gp->bmsobj.notes[i].stage = 0;
	}
	gp->bmsobj.size = 0;
	gp->bmsobj.draw_count = 0;
	gp->bmsobj.note_count = 0;
	gp->bmsobj.autoplay = 0;
	gp->bmsobj.noteVal = -1;

	for (int lane = 0; lane < 20; lane++) {
		if (gp->bmsobj_note[lane].size == 0) InitNoteBuffer(&gp->bmsobj_note[lane], 100);
		for (int i = 0; i < gp->bmsobj_note[lane].count; i++) {
			gp->bmsobj_note[lane].notes[i].bmsTiming_ln = -1.0;
			gp->bmsobj_note[lane].notes[i].realTiming_ln = -1.0;
			gp->bmsobj_note[lane].notes[i].active = -1;
			gp->bmsobj_note[lane].notes[i].val = -1.0;
			gp->bmsobj_note[lane].notes[i].bmsTiming = -1.0;
			gp->bmsobj_note[lane].notes[i].realTiming = -1.0;
			gp->bmsobj_note[lane].notes[i].op = -1;
			gp->bmsobj_note[lane].notes[i].channel = -1;
			gp->bmsobj_note[lane].notes[i].stage = 0;
		}
		gp->bmsobj_note[lane].size = 0;
		gp->bmsobj_note[lane].draw_count = 0;
		gp->bmsobj_note[lane].note_count = 0;
		gp->bmsobj_note[lane].autoplay = 0;
		gp->bmsobj_note[lane].noteVal = -1;
	}

	if (gp->bmsobj_line.size == 0) InitNoteBuffer(&gp->bmsobj_line, 100);
	for (int i = 0; i < gp->bmsobj_line.count; i++) {
		gp->bmsobj_line.notes[i].bmsTiming_ln = -1.0;
		gp->bmsobj_line.notes[i].realTiming_ln = -1.0;
		gp->bmsobj_line.notes[i].active = -1;
		gp->bmsobj_line.notes[i].val = -1.0;
		gp->bmsobj_line.notes[i].bmsTiming = -1.0;
		gp->bmsobj_line.notes[i].realTiming = -1.0;
		gp->bmsobj_line.notes[i].op = -1;
		gp->bmsobj_line.notes[i].channel = -1;
		gp->bmsobj_line.notes[i].stage = 0;
	}
	gp->bmsobj_line.size = 0;
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
		double prevHP = gp->player[p].HP;

		memset(&gp->player[p], 0, sizeof(PLAYERSTATUS));

		if (gp->courseStageNow > 0) {
			gp->player[p].HP = prevHP;
			gp->player[p].HP_print = prevHP;
			gp->player[p].now_combo_course = prevNowcombo;
			gp->player[p].max_combo_course = prevMaxcombo;
			gp->player[p].combo_draw = prevCombodraw;
			memcpy(gp->player[p].judgecount2, prevJudge, 6 * sizeof(int));
			gp->player[p].total_note = prevTotalnote;
			gp->player[p].note_current2 = prevNotecurrent;
		}
	}

	gp->player[0].flag_active = 1;
	gp->player[1].flag_active = 0;
	memset(&gp->statgraph[0], 0, sizeof(GRAPHDATA));
	memset(&gp->statgraph[1], 0, sizeof(GRAPHDATA));

	for (int p = 0; p < 2; p++) {
		if (gp->courseStageNow < 1) {
			if (gp->is2Pplay == 0 && (cfg->gaugeOption[p] == 0 || cfg->gaugeOption[p] == 3)) {
				gp->player[p].HP = 20.0;
				gp->player[p].HP_old = 20.0;
				gp->player[p].HP_print = 20.0;
			}
			else {
				gp->player[p].HP = 100.0;
				gp->player[p].HP_old = 100.0;
				gp->player[p].HP_print = 100.0;
			}
			gp->statgraph[p].hp[0] = gp->player[p].HP;
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
	gp->songStartTime[0] = -1;
	gp->songEndTime[0] = -1;
	gp->fadeEndTime[0] = -1;
	gp->connection_unkA[0] = -1;
	gp->connection_unkB[0] = -1;
	gp->connection_unkD[0] = -1;
	gp->songStartTime[1] = -1;
	gp->songEndTime[1] = -1;
	gp->fadeEndTime[1] = -1;
	gp->connection_unkA[1] = -1;
	gp->connection_unkB[1] = -1;
	gp->connection_unkD[1] = -1;
	gp->songStartTime[2] = -1;
	gp->songEndTime[2] = -1;
	gp->fadeEndTime[2] = -1;
	gp->connection_unkA[2] = -1;
	gp->connection_unkB[2] = -1;
	gp->connection_unkD[2] = -1;
	gp->songStartTime[3] = -1;
	gp->songEndTime[3] = -1;
	gp->fadeEndTime[3] = -1;
	gp->connection_unkA[3] = -1;
	gp->connection_unkB[3] = -1;
	gp->connection_unkD[3] = -1;
	gp->songStartTime[4] = -1;
	gp->songEndTime[4] = -1;
	gp->fadeEndTime[4] = -1;
	gp->connection_unkA[4] = -1;
	gp->connection_unkB[4] = -1;
	gp->connection_unkD[4] = -1;
	gp->songStartTime[5] = -1;
	gp->songEndTime[5] = -1;
	gp->fadeEndTime[5] = -1;
	gp->connection_unkA[5] = -1;
	gp->connection_unkB[5] = -1;
	gp->connection_unkD[5] = -1;
	gp->songStartTime[6] = -1;
	gp->songEndTime[6] = -1;
	gp->fadeEndTime[6] = -1;
	gp->connection_unkA[6] = -1;
	gp->connection_unkB[6] = -1;
	gp->connection_unkD[6] = -1;
	gp->songStartTime[7] = -1;
	gp->songEndTime[7] = -1;
	gp->fadeEndTime[7] = -1;
	gp->connection_unkA[7] = -1;
	gp->connection_unkB[7] = -1;
	gp->connection_unkD[7] = -1;
	gp->songStartTime[8] = -1;
	gp->songEndTime[8] = -1;
	gp->fadeEndTime[8] = -1;
	gp->connection_unkA[8] = -1;
	gp->connection_unkB[8] = -1;
	gp->connection_unkD[8] = -1;
	gp->songStartTime[9] = -1;
	gp->songEndTime[9] = -1;
	gp->fadeEndTime[9] = -1;
	gp->connection_unkA[9] = -1;
	gp->connection_unkB[9] = -1;
	gp->connection_unkD[9] = -1;
	gp->stageMixer[1] = 0;
	gp->stageMixer[2] = 0;
	gp->stageMixer[3] = 0;
	gp->stageMixer[4] = 0;
	gp->stageMixer[5] = 0;
	gp->stageMixer[6] = 0;
	gp->stageMixer[7] = 0;
	gp->stageMixer[8] = 0;
	gp->stageMixer[9] = 0;
	gp->stageMixer[0] = 100;

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

	EnterCriticalSection(&gp->criticalSection);

	for (int i = 0; i < 6480; i++) {
		if (gp->keysound_filename[i].length() > 0) {
			LoadSound(aud, &gp->keysound[i], gp->keysound_filename[i], 0, cfg->sound.disabledsp, (gp->isPreviewLoad != 0));
			if (gp->keysound[i].length > 60000 && gp->keysound[i].load) gp->flag_longsound = 1;
			gp->loadObject_loaded++;
		}
		if (gp->flag_closingPhase || (gp->previewStatus != 1 && noVideo)) {
			LeaveCriticalSection(&gp->criticalSection);
			return 1;
		}
	}

	gp->flag_0note = 1;
	for (int i = 0; i < gp->bmsobj.size; i++) {
		if ( (10 <= gp->bmsobj.notes[i].op && gp->bmsobj.notes[i].op < 30)
			&& (0 < gp->bmsobj.notes[i].val && gp->bmsobj.notes[i].val < 6480.0)
			&& gp->keysound[(int)gp->bmsobj.notes[i].val].load) {

			gp->flag_0note = 0;
			break;
		}
	}

	if (noVideo) {
		gp->loadObject_loaded = gp->loadObject_total;
		LeaveCriticalSection(&gp->criticalSection);
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
			LeaveCriticalSection(&gp->criticalSection);
			return 1;
		}
	}

	SetTransColor(Rtmp, Gtmp, Btmp);
	if (cfg->system.isablebmsthread == 0) CoUninitialize();
	if (gp->bgaHandle[0] != -1) gp->missLayer = 0;

	if (gp->isAutoplay == 1) {

		for (int i = 0; i < gp->bmsobj.size; i++) {
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
	LeaveCriticalSection(&gp->criticalSection);
	return 0;
}

//4ad200
int InitGameplay_retry(gameplay *gp, AUDIO *snd, game *g) {
	
	PlayerCheckAndSwap(gp);
	if (gp->replay.status != 2 && gp->replay.status == 1) {
		AddReplayData(&gp->replay, 0, 200, gp->randomseed);
	}

	gp->stageMixer[1] = 0;
	gp->stageMixer[2] = 0;
	gp->stageMixer[3] = 0;
	gp->stageMixer[4] = 0;
	gp->stageMixer[5] = 0;
	gp->stageMixer[6] = 0;
	gp->stageMixer[7] = 0;
	gp->stageMixer[8] = 0;
	gp->stageMixer[9] = 0;
	gp->stageMixer[0] = 100;

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

	for (int i = 0; i < gp->bmsobj.count; i++) {
		gp->bmsobj.notes[i].active = 0;
	}
	gp->bmsobj.draw_count = 0;
	gp->bmsobj.note_count = 0;
	gp->bmsobj.noteVal = -1;

	for (int lane = 0; lane < 20; lane++) {
		for (int i = 0; i < gp->bmsobj_note[lane].count; i++) {
			gp->bmsobj_note[lane].notes[i].active = 0;
		}
		gp->bmsobj_note[lane].draw_count = 0;
		gp->bmsobj_note[lane].note_count = 0;
		gp->bmsobj_note[lane].noteVal = -1;
	}

	for (int i = 0; i < gp->bmsobj_line.count; i++) {
		gp->bmsobj_line.notes[i].active = 0;
	}
	gp->bmsobj_line.draw_count = 1;
	gp->bmsobj_line.note_count = 0;
	gp->bmsobj_line.noteVal = -1;
	gp->delayDetectedCount = 0;
	gp->delayCheckCount = 0;

	double tempDmg[6];
	int tempTime[6], tempCount;
	tempCount = gp->player[0].totalnotes;
	memcpy(tempDmg, &gp->player[0].judge_damage, sizeof(tempDmg));
	memcpy(tempTime, &gp->player[0].judgetime, sizeof(tempTime));
	memset(&gp->player[0], 0, sizeof(PLAYERSTATUS));
	memcpy(&gp->player[0].judge_damage, tempDmg, sizeof(tempDmg));
	memcpy(&gp->player[0].judgetime, tempTime, sizeof(tempTime));
	gp->player[0].totalnotes = tempCount;

	tempCount = gp->player[1].totalnotes;
	memcpy(tempDmg, &gp->player[1].judge_damage, sizeof(tempDmg));
	memcpy(tempTime, &gp->player[1].judgetime, sizeof(tempTime));
	memset(&gp->player[1], 0, sizeof(PLAYERSTATUS));
	memcpy(&gp->player[1].judge_damage, tempDmg, sizeof(tempDmg));
	memcpy(&gp->player[1].judgetime, tempTime, sizeof(tempTime));
	gp->player[1].totalnotes = tempCount;

	memset(&gp->statgraph[0], 0, sizeof(GRAPHDATA));
	memset(&gp->statgraph[1], 0, sizeof(GRAPHDATA));
	
	gp->player[0].flag_active = 1;
	gp->player[1].flag_active = 0;

	for (int p = 0; p < 2; p++) {
		if (gp->is2Pplay) {
			gp->player[p].HP = 100.0;
			gp->player[p].HP_old = 100.0;
			gp->player[p].HP_print = 100.0;
		}
		else if (g->config.play.gaugeOption[p] == 0 || g->config.play.gaugeOption[p] == 3) {
			gp->player[p].HP = 20.0;
			gp->player[p].HP_old = 20.0;
			gp->player[p].HP_print = 20.0;
		}
		else{
			gp->player[p].HP = 100.0;
			gp->player[p].HP_old = 100.0;
			gp->player[p].HP_print = 100.0;
		}
		gp->statgraph->hp[p] = gp->player[p].HP;
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
		if(gp->bmsobj_note[i].size <= 0)
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

	for (int i = start; i < lane->size; i++) {
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
	
	for (int i = start; i < lane->size; i++) {
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

	for (int i = start; i < lane->size; i++) {
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

	for (int i = start; i < lane->size; i++) {
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

	for (int i = start; i < lane->size; i++) {
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
		for (int i = start; i < lane->size; i++) {
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
	qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);

	for (int i = 0; i < gp->bmsobj.size; i++) {
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

		for (int j = 0; j < gp->bmsobj.size; j++) {
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
	for (int i = 0; i < gp->bmsobj.size; i++) {
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

			for (int j = i; j < gp->bmsobj.size; j++) {
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

	qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
}

//4ae8d0
void DPtoSP(gameplay *gp) { //test completed

	int mingap;
	char laneA[10], laneB[10];

	qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
	
	if (gp->BPM_fix <= 0.0 || (int)(30000.0 / gp->BPM_fix) < 125)
		mingap = 125;
	else
		mingap = (30000.0 / gp->BPM_fix);

	for (int i = 0; i < gp->bmsobj.size; i++) {

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
			

			for (int next = i+1; next < gp->bmsobj.size; next++) {
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
	qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
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

	qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
	int prev = 0; //prevMeasureStart
	int measure = 0;
	memset(laneA, 0, sizeof(laneA));
	left = 9;
	right = 1;
	measureLaneCount = 0;

	for (int i = 0; i < gp->bmsobj.size; i++) {

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


						for (int next = j + 1; next < gp->bmsobj.size; next++) {
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
							gp->bmsobj.notes[j].op = (gp->bmsobj.notes[j].channel <= 0) ? 1 : -1;
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

	qsort(gp->bmsobj.notes, gp->bmsobj.size, sizeof(NoteStruct), CMP_NotesByRealTimingOp);
	measure = 0;
	for (int i = 0; i < gp->bmsobj.size; i++) {
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

	for (int i = start; i < lane->size; i++) {
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

	for (int i = start; i < lane->size; i++) {

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

						for (int j = scratchNoteID; j < lane->size; j++) {
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

						for (int j = scratchNoteID; j < lane->size; j++) {
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
	for (int i = baseNoteID + 1; i < lane->size; i++) {
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
	for (int i = baseNoteID + 1; i < lane->size; i++) {

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

//4b0690 ParseBmsFile()



//TIMER
double ClockTime;
bool ableClockFlag;
bool ableClockCount;

//4b6710
int SetBGATimer(Timer *T, double newTime){
	ClockTime = newTime;
	T->bga = newTime;
	return 1;
}

//4b6730
int SetAbleClockCount(char val){
	ableClockCount = val;
	return 1;
}

//4b6740
int SetClockFlag(Timer *T, char flag){
	T->flag = flag;
	ClockTime = 0.0;
	ableClockFlag = flag;
	return 1;
}

//4b6770
int NONE_004b6770(void){
	return 1;
}

//4b6780
int BGATimer(Timer *T){
	if (T->bgaFramerate <= 0.0) {
		T->bga = T->bga + 33.33333333333334;
	}
	else {
		T->bga = 1000.0 / T->bgaFramerate + T->bga;
	}
	ClockTime = T->bga;
	return 1;
}

//4b67d0
int ResetTimeLapse(int timerID, Timer *T){
	T->clock[timerID] = -1.0;
	if (timerID == 140) {
		T->Rhythm = -1.0;
		return 1;
	}
	return 1;
}

//4b6800 //not reliable
double GetTime(void){
	long long lVar1;
	DWORD time;
	BOOL BVar2;
	LARGE_INTEGER pfc_time;
	LARGE_INTEGER pfc_freq;
	double ret;

	if (ableClockCount == '\0') {
		time = timeGetTime();
		ret = (double)(time & 0x7fffffff);
	}
	else {
		BVar2 = QueryPerformanceFrequency(&pfc_freq);
		if (BVar2 == 0) {
			time = timeGetTime();
			ret = (double)(time & 0x7fffffff);
		}
		else {
			QueryPerformanceCounter(&pfc_time);
			ret = (double)pfc_time.QuadPart*1000 / (double)pfc_freq.QuadPart;
		}
	}
	return ret;
}

//4b6890
double GetTimeWrap(void) {
	double dVar1;

	if (ableClockFlag != '\0') {
		return ClockTime;
	}
	timeBeginPeriod(1);
	dVar1 = GetTime();
	timeEndPeriod(1);
	return dVar1;
}
/*
bool ableClockCount;
bool ableClockFlag;
double ClockTime;

//4b6800
double GetTime(){
	LARGE_INTEGER timer, t1;
	long long lVar1;
	DWORD time;
	BOOL BVar2;
	undefined4 local_10;
	undefined4 uStack12;
	long long local_8;
	double ret;

	if (ableClockCount == '\0') {
		time = timeGetTime();
		ret = (double)(time & 0x7fffffff);
	}
	else {
		BVar2 = QueryPerformanceFrequency(&timer);
		if (BVar2 == 0) {
			time = timeGetTime();
			ret = (double)(time & 0x7fffffff);
		}
		else {
			QueryPerformanceCounter(&t1);
			ret = t1.QuadPart / timer.QuadPart;
		}
	}
	return ret;
}

//4b6890
double GetTimeWrap() {
	double ret;
	if (ableClockFlag) {
		return ClockTime;
	}
	timeBeginPeriod(1);
	ret = GetTime();
	timeEndPeriod(1);
	return ret;
}
*/

//4b68d0
int InitTimer(Timer *T) {
	double dVar4;

	T->clock[0] = -1.0;
	for (int i = 0; i < 499; i++) {
		T->clock[i + 1] = T->clock[i];
	}

	dVar4 = ClockTime;
	if (ableClockFlag == '\0') {
		timeBeginPeriod(1);
		dVar4 = GetTime();
		timeEndPeriod(1);
	}
	T->scratch = dVar4;
	dVar4 = ClockTime;
	if (ableClockFlag == '\0') {
		timeBeginPeriod(1);
		dVar4 = GetTime();
		timeEndPeriod(1);
	}
	T->rhythmTick = dVar4;
	dVar4 = ClockTime;
	if (ableClockFlag == '\0') {
		timeBeginPeriod(1);
		dVar4 = GetTime();
		timeEndPeriod(1);
	}
	T->gameTick = dVar4;
	T->tickTime = 0.0;
	T->Rhythm = -1.0;
	dVar4 = ClockTime;
	if (ableClockFlag == '\0') {
		timeBeginPeriod(1);
		dVar4 = GetTime();
		timeEndPeriod(1);
	}
	T->vSyncTick = dVar4;
	dVar4 = ClockTime;
	if (ableClockFlag == '\0') {
		timeBeginPeriod(1);
		dVar4 = GetTime();
		timeEndPeriod(1);
	}
	T->FPSclock = dVar4;
	T->FPScount = 0.0;
	ErrorLogFmtAdd("タイマーを初期化しました\n");
	return 1;
}

//4b6a10
int CalcFPS(Timer *t){
	double dVar1;

	t->FPScount = t->FPScount + 1.0;
	dVar1 = ClockTime;
	if (ableClockFlag == '\0') {
		timeBeginPeriod(1);
		dVar1 = GetTime();
		timeEndPeriod(1);
	}
	dVar1 = dVar1 - t->FPSclock;
	if ( dVar1 > 1000.0) {
		dVar1 = ClockTime;
		if (ableClockFlag == '\0') {
			timeBeginPeriod(1);
			dVar1 = GetTime();
			timeEndPeriod(1);
		}
		t->FPS = (t->FPScount * 1000.0 + 499.0) / (dVar1 - t->FPSclock);
		t->FPScount = 0.0;
		if (ableClockFlag != '\0') {
			t->FPSclock = ClockTime;
			return 1;
		}
		timeBeginPeriod(1);
		dVar1 = GetTime();
		timeEndPeriod(1);
		t->FPSclock = dVar1;
	}
	return 1;
}

//4b6b10
double GetTimeLapse(uint timerID, Timer *T) {
	double ret;

	if (500 < timerID) return -1.0;
	if (timerID == 140) return T->Rhythm;

	ret = -1.0;
	if (ret != T->clock[timerID]) {
		if ((T->flag == '\0') || (T->bgaFramerate <= 0.0)) {
			return GetTimeWrap() - T->clock[timerID];
		}
		ret = T->bga - T->clock[timerID];
	}
	return ret;
}

//4b6b80
int SetTimeLapse(int timerID, Timer *T){
	double dVar1;

	if ((*(char *)&T->flag != '\0') && (0.0 < T->bgaFramerate)) {
		T->clock[timerID] = T->bga;
	}
	if (ableClockFlag != '\0') {
		T->clock[timerID] = ClockTime;
		return 1;
	}
	timeBeginPeriod(1);
	dVar1 = GetTime();
	timeEndPeriod(1);
	T->clock[timerID] = dVar1;
	return 1;
}


////STRING
//4b6c00
int InitObjectString(TextStruct *txt){
	for (int i = 0; i < 300; i++) {
		txt->objectStr[i].fillzero();
	}
	txt->objectStr[0].assign("これはテストこれはテストこれはテスト");
	txt->st_text_num = -1;
	return 1;
}

//4b6c40
int SetObjectString(uint num, CSTR string, CSTR *objectList){
	if (num > 299) {
		return 0;
	}
	if (string.isDiff("(null)") == 0) {
		if (string.length() == 0) {
			objectList[num].fillzero();
			return 0;
		}
	}
	objectList[num].assign(&string);
	return 1;
}

//4b6d00
int SetObjectStringInt(int at, int val, CSTR *arr){
	cstrSprintf(&arr[at], "%d", val);
	return 1;
}

//4b6d30
CSTR GetStringFromArray(int num, CSTR *strings) {
	return strings[num];
}

//4b6d60
int DefineOptionStrNum(OptionString *arrOpStr){
	arrOpStr[19].count = 4;
	arrOpStr[5].count = 4;
	arrOpStr[14].count = 4;
	arrOpStr[15].count = 4;
	arrOpStr[22].count = 4;
	arrOpStr[7].count = 9;
	arrOpStr[8].count = 2;
	arrOpStr[9].count = 3;
	arrOpStr[13].count = 2;
	arrOpStr[2].count = 6;
	arrOpStr[6].count = 2;
	arrOpStr[3].count = 6;
	arrOpStr[0].count = 8;
	arrOpStr[4].count = 6;
	arrOpStr[16].count = 2;
	arrOpStr[12].count = 2;
	arrOpStr[10].count = 5;
	arrOpStr[1].count = 6;
	arrOpStr[11].count = 2;
	arrOpStr[17].count = 3;
	arrOpStr[18].count = 5;
	arrOpStr[20].count = 6;
	arrOpStr[23].count = 2;
	arrOpStr[21].count = 2;
	arrOpStr[24].count = 2;
	return 1;
}

//4b6e30
int ReadOptionstr(OptionString *opStr, CSVbuf csv) {
	for (int i = 0; i < opStr->count; i++) {
		opStr->str[i].assign(&csv.str[1+i]);
	}
	for (int i = opStr->count; i < 10; i++) {
		opStr->str[i].fillzero();
	}
	return 1;
}

//4b6ee0 : CSVbuf copy
//4b6f20
int ReadOptionstrFile(OptionString *arrOpStr, CSTR filepath) {
	int bufSize = 256;

	FILE *pFile;
	CSTR fBuf = CSTR(bufSize);
	char* pFbuf;
	CSVbuf csv;

	DefineOptionStrNum(arrOpStr);
	pFile = fopen(filepath, "r");
	if (pFile == 0) {
		ErrorLogAdd("オプション文字列リストが見つかりません。\n");
		return 0;
	}

	pFbuf = fBuf.outstr();
	for (pFbuf = fgets(pFbuf, bufSize, pFile); pFbuf; pFbuf = fgets(pFbuf, bufSize, pFile)) {
		if (*fBuf.atPos(0) == '#') {
			SplitCSV(fBuf, &csv, ",");

			if (fBuf.left(5).isSame("#MODE")) {
				ReadOptionstr(&arrOpStr[0], csv);
			}
			else if (fBuf.left(5).isSame("#SORT")) {
				ReadOptionstr(&arrOpStr[1], csv);
			}
			else if (fBuf.left(11).isSame("#DIFFICULTY")) {
				ReadOptionstr(&arrOpStr[2], csv);
			}
			else if (fBuf.left(6).isSame("#GAUGE")) {
				ReadOptionstr(&arrOpStr[3], csv);
			}
			else if (fBuf.left(7).isSame("#RANDOM")) {
				ReadOptionstr(&arrOpStr[4], csv);
			}
			else if (fBuf.left(7).isSame("#ASSIST")) {
				ReadOptionstr(&arrOpStr[19], csv);
			}
			else if (fBuf.left(5).isSame("#FLIP")) {
				ReadOptionstr(&arrOpStr[6], csv);
			}
			else if (fBuf.left(7).isSame("#BATTLE")) {
				ReadOptionstr(&arrOpStr[7], csv);
			}
			else if (fBuf.left(7).isSame("#EFFECT")) {
				ReadOptionstr(&arrOpStr[5], csv);
			}
			else if (fBuf.left(6).isSame("#HSFIX")) {
				ReadOptionstr(&arrOpStr[10], csv);
			}
			else if (fBuf.left(7).isSame("#SCREEN")) {
				ReadOptionstr(&arrOpStr[12], csv);
			}
			else if (fBuf.left(6).isSame("#COLOR")) {
				ReadOptionstr(&arrOpStr[13], csv);
			}
			else if (fBuf.left(8).isSame("#BGASIZE")) {
				ReadOptionstr(&arrOpStr[8], csv);
			}
			else if (fBuf.left(8).isSame("#BGADRAW")) {
				ReadOptionstr(&arrOpStr[9], csv);
			}
			else if (fBuf.left(11).isSame("#SCOREGRAPH")) {
				ReadOptionstr(&arrOpStr[16], csv);
			}
			else if (fBuf.left(6).isSame("#GHOST")) {
				ReadOptionstr(&arrOpStr[14], csv);
			}
			else if (fBuf.left(10).isSame("#LANECOVER")) {
				ReadOptionstr(&arrOpStr[15], csv);
			}
			else if (fBuf.left(6).isSame("#VSYNC")) {
				ReadOptionstr(&arrOpStr[11], csv);
			}
			else if (fBuf.left(11).isSame("#AUTOADJUST")) {
				ReadOptionstr(&arrOpStr[17], csv);
			}
			else if (fBuf.left(11).isSame("#REPLAYSAVE")) {
				ReadOptionstr(&arrOpStr[18], csv);
			}
			else if (fBuf.left(15).isSame("#COURSE_TSUNAGI")) {
				ReadOptionstr(&arrOpStr[20], csv);
			}
			else if (fBuf.left(13).isSame("#COURSE_GAUGE")) {
				ReadOptionstr(&arrOpStr[22], csv);
			}
			else if (fBuf.left(12).isSame("#COURSE_TYPE")) {
				ReadOptionstr(&arrOpStr[21], csv);
			}
			else if (fBuf.left(17).isSame("#COURSE_OPTIONBAN")) {
				ReadOptionstr(&arrOpStr[23], csv);
			}
			else if (fBuf.left(10).isSame("#COURSE_IR")) {
				ReadOptionstr(&arrOpStr[24], csv);
			}
			*fBuf.atPos(0) = '\0';
		}
		pFbuf = fBuf.outstr();
	}

	ErrorLogAdd("オプション文字列リストを読み込みました。\n");
	return 1;
}


////SOUND section
//we got the fmodex 4.13.04. so we have no version problem now.
//still there are writings for migration, for future

// 140611 1.04.00 FMOD_HARDWARE and FMOD_SOFTWARE flags have been removed. All voices are software mixed in FMOD Studio.
// 190314 2.00.00 The minimum supported version of Windows has been lifted from Windows XP to Windows 7. Coupled with this change we have removed the legacy DirectSound and WinMM output modes.
//https://www.fmod.com/docs/2.02/api/white-papers-transitioning-from-fmodex.html#system_adddsp-removed-from-the-system-api
//https://www.fmod.com/docs/2.02/api/welcome-revision-history.html
//http://upstream.rosalinux.ru/changelogs/fmod/44418/changelog.html


//4b7b80
const char* GetFMODerror(int errCode){
	switch (errCode) {
	case 0:
		return "No errors.";
	case 1:
		return "Tried to call lock a second time before unlock was called. ";
	case 2:
		return "Tried to call a function on a data type that does not allow this type of functionality (ie call ing Sound::lock on a streaming sound). "			;
	case 3:
		return "Neither NTSCSI nor ASPI could be initialised. ";
	case 4:
		return "An error occurred while initialising the CDDA subsystem. ";
	case 5:
		return "Couldn\'t find the specified device. ";
	case 6:
		return "No audio tracks on the specified disc. ";
	case 7:
		return "No CD/DVD devices were found. ";
	case 8:
		return "No disc present in the specified drive. ";
	case 9:
		return "A CDDA read error occurred. ";
	case 10:
		return "Error trying to allocate a channel. ";
	case 0xb:
		return "The specified channel has been reused to play another sound. ";
	case 0xc:
		return "A Win32 COM related error occured. COM failed to initialize or a QueryInterface failed meaning  a Windows codec or driver was not installed properly. ";
	case 0xd:
		return "DMA Failure.  See debug output for more information. ";
	case 0xe:
		return "DSP connection error.  Connection possibly caused a cyclic dependancy.  Or tried to connect a t ree too many units deep (more than 128). ";
	case 0xf:
		return "DSP Format error.  A DSP unit may have attempted to connect to this network with the wrong form at. ";
	case 0x10:
		return "DSP connection error.  Couldn\'t find the DSP unit specified. ";
	case 0x11:
		return "DSP error.  Cannot perform this operation while the network is in the middle of running.  This  will most likely happen if a connection or disconnection is attempted in a DSP callback. ";
	case 0x12:
		return "DSP connection error.  The unit being connected to or disconnected should only have 1 input or  output. ";
	case 0x13:
		return "Error loading file. ";
	case 0x14:
		return "Couldn\'t perform seek operation.  This is a limitation of the medium (ie netstreams) or the fi le format. ";
	case 0x15:
		return "Media was ejected while reading. ";
	case 0x16:
		return "End of file unexpectedly reached while trying to read essential data (truncated data?). ";
	case 0x17:
		return "File not found. ";
	case 0x18:
		return "Unwanted file access occured. ";
	case 0x19:
		return "Unsupported file or audio format. ";
	case 0x1a:
		return "A HTTP error occurred. This is a catch-all for HTTP errors not listed elsewhere. ";
	case 0x1b:
		return "The specified resource requires authentication or is forbidden. ";
	case 0x1c:
		return "Proxy authentication is required to access the specified resource. ";
	case 0x1d:
		return "A HTTP server error occurred. ";
	case 0x1e:
		return "The HTTP request timed out. ";
	case 0x1f:
		return "FMOD was not initialized correctly to support this function. ";
	case 0x20:
		return "Cannot call this command after System::init. ";
	case 0x21:
		return "An error occured that wasn\'t supposed to.  Contact support. ";
	case 0x22:
		return "On Xbox 360, this memory address passed to FMOD must be physical, (ie allocated with XPhysicalA lloc.) ";
	case 0x23:
		return "Value passed in was a NaN, Inf or denormalized float. ";
	case 0x24:
		return "An invalid object handle was used. ";
	case 0x25:
		return "An invalid parameter was passed to this function. ";
	case 0x26:
		return "An invalid speaker was passed to this function based on the current speaker mode. ";
	case 0x27:
		return "The syncpoint did not come from this sound handle. ";
	case 0x28:
		return "The vectors passed in are not unit length, or perpendicular. ";
	case 0x29:
		return "PS2 only.  fmodex.irx failed to initialize.  This is most likely because you forgot to load it.  ";
	case 0x2a:
		return "Reached maximum audible playback count for this sound\'s soundgroup. ";
	case 0x2b:
		return "Not enough memory or resources. ";
	case 0x2c:
		return "Can\'t use FMOD_OPENMEMORY_POINT on non PCM source data, or non mp3/xma/adpcm data if FMOD_CREA TECOMPRESSEDSAMPLE was used. ";
	case 0x2d:
		return "PS2 only.  Not enough memory or resources on PlayStation 2 IOP ram. ";
	case 0x2e:
		return "Not enough memory or resources on console sound ram. ";
	case 0x2f:
		return "Tried to call a command on a 3d sound when the command was meant for 2d sound. ";
	case 0x30:
		return "Tried to call a command on a 2d sound when the command was meant for 3d sound. ";
	case 0x31:
		return "Tried to use a feature that requires hardware support.  (ie trying to play a VAG compressed sou nd in software on PS2). ";
	case 0x32:
		return "Tried to use a feature that requires the software engine.  Software engine has either been turn ed off, or command was executed on a hardware channel which does not support this feature. ";
	case 0x33:
		return "Couldn\'t connect to the specified host. ";
	case 0x34:
		return "A socket error occurred.  This is a catch-all for socket-related errors not listed elsewhere. ";
	case 0x35:
		return "The specified URL couldn\'t be resolved. ";
	case 0x36:
		return "Operation on a non-blocking socket could not complete immediately. ";
	case 0x37:
		return "Operation could not be performed because specified sound is not ready. ";
	case 0x38:
		return "Error initializing output device, but more specifically, the output device is already in use an d cannot be reused. ";
	case 0x39:
		return "Error creating hardware sound buffer. ";
	case 0x3a:
		return "A call to a standard soundcard driver failed, which could possibly mean a bug in the driver or  resources were missing or exhausted. ";
	case 0x3b:
		return "Error enumerating the available driver list. List may be inconsistent due to a recent device ad dition or removal. ";
	case 0x3c:
		return "Soundcard does not support the minimum features needed for this soundsystem (16bit stereo outpu t). ";
	case 0x3d:
		return "Error initializing output device. ";
	case 0x3e:
		return "FMOD_HARDWARE was specified but the sound card does not have the resources necessary to play it . ";
	case 0x3f:
		return "Attempted to create a software sound but no software channels were specified in System::init. ";
	case 0x40:
		return "Panning only works with mono or stereo sound sources. ";
	case 0x41:
		return "An unspecified error has been returned from a 3rd party plugin. ";
	case 0x42:
		return "The number of allowed instances of a plugin has been exceeded. ";
	case 0x43:
		return "A requested output, dsp unit type or codec was not available. ";
	case 0x44:
		return "A resource that the plugin requires cannot be found. (ie the DLS file for MIDI playback) ";
	case 0x45:
		return "An error occured trying to initialize the recording device. ";
	case 0x46:
		return "Specified Instance in FMOD_REVERB_PROPERTIES couldn\'t be set. Most likely because it is an inv alid instance number, or another application has locked the EAX4 FX slot. ";
	case 0x47:
		return "This subsound is already being used by another sound, you cannot have more than one parent to a  sound.  Null out the other parent\'s entry first. ";
	case 0x48:
		return "Shared subsounds cannot be replaced or moved from their parent stream, such as when the parent  stream is an FSB file. ";
	case 0x49:
		return "The subsound\'s mode bits do not match with the parent sound\'s mode bits.  See documentation f or function that it was called with. ";
	case 0x4a:
		return "The error occured because the sound referenced contains subsounds.  The operation cannot be per formed on a parent sound, or a parent sound was played without setting up a sentence first. ";
	case 0x4b:
		return "The specified tag could not be found or there are no tags. ";
	case 0x4c:
		return "The sound created exceeds the allowable input channel count.  This can be increased using the m axinputchannels parameter in System::setSoftwareFormat. ";
	case 0x4d:
		return "Something in FMOD hasn\'t been implemented when it should be! contact support! ";
	case 0x4e:
		return "This command failed because System::init or System::setDriver was not called. ";
	case 0x4f:
		return "A command issued was not supported by this object.  Possibly a plugin without certain callbacks  specified. ";
	case 0x50:
		return "An error caused by System::update occured. ";
	case 0x51:
		return "The version number of this file format is not supported. ";
	case 0x52:
		return "An Event failed to be retrieved, most likely due to \'just fail\' being specified as the max pl aybacks behavior. ";
	case 0x53:
		return "Can\'t execute this command on an EVENT_INFOONLY event. ";
	case 0x54:
		return "An error occured that wasn\'t supposed to.  See debug log for reason. ";
	case 0x55:
		return "Event failed because \'Max streams\' was hit when FMOD_INIT_FAIL_ON_MAXSTREAMS was specified. ";
	case 0x56:
		return "FSB mismatches the FEV it was compiled with, the stream/sample mode it was meant to be created  with was different, or the FEV was built for a different platform. ";
	case 0x57:
		return "A category with the same name already exists. ";
	case 0x58:
		return "The requested event, event group, event category or event property could not be found. ";
	case 0x59:
		return "Tried to call a function on a complex event that\'s only supported by simple events. ";
	case 0x5a:
		return "An event with the same GUID already exists. ";
	case 0x5b:
		return "Music system is not initialized probably because no music data is loaded. ";
	default:
		return "Unknown error.";
	}
}

//4b7f30
int IsAltSoundExist(CSTR *filepath){
	const char *str;
	CSTR tStr(*filepath); //somewhat starnge

	filepath->lower();
	if (tStr.findStrPos("wav")) {
		filepath->nullAtPos(filepath->length() - 3);
		str = "ogg";
	}
	else if (tStr.findStrPos("ogg")) {
		filepath->nullAtPos(filepath->length() - 3);
		str = "wav";
	}
	else return -1;
	
	filepath->add(str);

	return IsFileExist(*filepath) ? 1 : -1;
}


//4b8040
int ReleaseSound(AUDIO *aud, SOUNDDATA *sound){
	CSTR &filepath = sound->filename;

	if (sound->load == '\0') {
		return -1;
	}
	if (filepath.length() < 2) {
		filepath.fillzero();
		sound->length = 0;
		sound->load = '\0';
		return -1;
	}
	if (aud->is_fmod_disabled == 1) {
		StopSoundMem(sound->soundHandle);
		DeleteSoundMem(sound->soundHandle, 0);
	}
	else {
		FMOD_Sound_Release(sound->fmod_sound);
	}
	filepath.fillzero();
	sound->length = 0;
	sound->load = '\0';
	return 0;
}

//4b80c0 COPY

//4b8140
int StopSound(AUDIO *aud, SOUNDDATA *sound){
	FMOD_SOUND *tmpSnd;

	if (sound->load == '\0') {
		return -1;
	}
	if (aud->is_fmod_disabled == 1) {
		StopSoundMem(sound->soundHandle);
	}
	FMOD_Channel_GetCurrentSound(sound->fmod_channel, &tmpSnd);
	if (tmpSnd == sound->fmod_sound) {
		FMOD_Channel_Stop(sound->fmod_channel);
	}
	return 0;
}

//4b8190
int SetSoundVolume(AUDIO *aud, SOUNDDATA *sound, float volume){
	FMOD_SOUND *tmpSnd;

	if (sound->load == '\0') {
		return -1;
	}
	if (aud->is_fmod_disabled == 1) {
		ChangeVolumeSoundMem(volume * 255.0, sound->soundHandle);
	}
	FMOD_Channel_GetCurrentSound(sound->fmod_channel, &tmpSnd);
	if (tmpSnd == sound->fmod_sound) {
		FMOD_Channel_SetVolume(sound->fmod_channel, volume);
	}
	return 0;
}

//4b8200
int SoundGetCurrentTime(AUDIO *aud, SOUNDDATA *sound){
	FMOD_SOUND *tmpSnd;
	uint pos;

	if (sound->load != '\0') {
		if (aud->is_fmod_disabled == 1) {
			return GetSoundCurrentTime(sound->soundHandle);
		}
		FMOD_Channel_GetCurrentSound(sound->fmod_channel, &tmpSnd);
		if (tmpSnd == sound->fmod_sound) {
			FMOD_Channel_GetPosition(sound->fmod_channel, &pos, 1);
			return pos;
		}
	}
	return -1;
}

//4b8260 endsound
int EndSound(AUDIO *aud){

	if (aud->is_fmod_disabled != 1) {
		for (int i = 0; i < 7; i++) {
			FMOD_DSP_Release(aud->param.DSP_eq[i]);
		}
		for (int i = 0; i < 3; i++) {
			FMOD_DSP_Release(aud->param.DSP_fx[i]);
		}
		FMOD_DSP_Release(aud->param.DSP_pitch);
		FMOD_ChannelGroup_Release(aud->chnBgm);
		FMOD_ChannelGroup_Release(aud->chnKey);
		FMOD_ChannelGroup_Release(aud->chnMaster);
		FMOD_System_Close(aud->fmodSys);
		FMOD_System_Release(aud->fmodSys);
	}
	return 0;
}

//4b82f0
int SOUND_dxlibFx(SOUNDDATA sound, int v_master, int v_BGA, int pitch, double freq){

	double dMul;
	
	if (sound.load != '\0') {
		ChangeVolumeSoundMem(v_master, sound.soundHandle);
		dMul = 1.0;
		if (pitch > 0) {
			for (; pitch > 0; pitch--) {
				dMul *= 1.0594631433486938;
			}
		}
		else if (pitch < 0) {
			for (pitch = -pitch; pitch > 0; pitch--) {
				dMul /= 1.0594631433486938;
			}
		}
		SetFrequencySoundMem((double)GetFrequencySoundMem(sound.soundHandle) * dMul * freq, sound.soundHandle);
	}
	return 0;
}

//4b83d0
int SetFadeOut(AUDIO *aud, int fadetime){

	if (fadetime < 1) {
		return -1;
	}
	aud->param.fadeout_volume = 1.0;
	aud->param.time_fadeout_start = timeGetTime();
	aud->param.time_fadeout_end = timeGetTime() + fadetime;
	return 1;
}

//4b8410
int SetFadePreview(AUDIO *aud, int fadeintime, char flag){
	DWORD DVar1;

	if (fadeintime < 1) {
		return -1;
	}
	aud->param.time_fadePreview_start = timeGetTime();
	aud->param.fadePreviewStartVolume = aud->param.fadePreviewCurrentVolume;
	aud->param.time_fadePreview_end = timeGetTime() + fadeintime;
	aud->param.fadePreviewVolumeFlag = flag;
	if (flag != '\0') {
		aud->param.fadePreviewTargetVolume = 0.025;
		return 1;
	}
	aud->param.fadePreviewTargetVolume = 1.0;
	return 1;
}

//4b8480
char sndBufUnk;
int GetSoundBuffer(AUDIO *aud, double runtime, int volume) {

	int len;
	aud->bitRate = 0;
	len = (runtime / 1000.0 + 1.0) * 44100.0 * 2;
	aud->size = ((len / 4096) + 1) * 4096; //to avoid size 0?
	aud->volume = volume / 100.0;
	if (aud->volume >= 1.0) {
		aud->volume = 1.0;
	}
	if (aud->volume < 0.0) {
		aud->volume = 0.0;
	}
	ErrorLogFmtAdd("音量設定 %d", aud->volume * 100.0);
	ErrorLogFmtAdd("バッファを取得します。サイズ%d\n", aud->size);

	aud->buffer = (short*)malloc(aud->size*2); //big number problem solution was in original code, I skipped it
	if(aud->buffer == NULL) {
		ErrorLogFmtAdd("バッファ取得に失敗。サイズ%d\n", aud->size);
		sndBufUnk = 0;
		return 1;
	}

	ErrorLogFmtAdd("バッファ取得が完了しました。サイズ%d\n", aud->size);
	memset(aud->buffer, '\0', aud->size * 2);
	ErrorLogFmtAdd("0で埋めました\n");
	sndBufUnk = 0;
	return 1;
}

//4b85b0 debug test needed
void WriteSoundFile(AUDIO *aud, CSTR filename, uint size) {
	
	FILE* _File;
	char header1[12], header2[24], header3[8];

	_File = fopen(filename, "wb");
	if ((size != 0) && (size < aud->size || size == aud->size)) {
		aud->size = size;
	}
	if (_File) {
		fseek(_File, 0, 0);
		header1[0] = 'R';
		header1[1] = 'I';
		header1[2] = 'F';
		header1[3] = 'F';
		memset(&header1[4], aud->size * 2 + 0x20, 4);
		header1[8] = 'W';
		header1[9] = 'A';
		header1[10] = 'V';
		header1[11] = 'E';

		header2[0] = 'f';
		header2[1] = 'm';
		header2[2] = 't';
		header2[3] = ' ';
		memset(&header2[4], 16, 4);
		memset(&header2[8], 1, 2);
		memset(&header2[10], 2, 2);
		memset(&header2[12], 44100, 4);
		memset(&header2[16], 176400, 4);
		memset(&header2[20], 4, 2);
		memset(&header2[22], 16, 2);

		header3[0] = 'd';
		header3[1] = 'a';
		header3[2] = 't';
		header3[3] = 'a';
		memset(&header3[4], aud->size * 2, 4);

		fwrite(header1, 0xc, 1, _File);
		fwrite(header2, 0x18, 1, _File);
		fwrite(header3, 8, 1, _File);
		fwrite(aud->buffer, aud->size * 2, 1, _File);
		fclose(_File);
	}
	if (aud->buffer) {
		delete(aud->buffer);
	}
	ErrorLogFmtAdd("データを保存しました。\n");
}

//4b8770
int SOUND_normalize(AUDIO *aud, SOUNDDATA *sound){
	uint len;
	int bitdepth;
	int channels;
	float samples;

	len = 0;
	FMOD_Sound_GetLength(sound->fmod_sound, &len, 4);
	samples = 44100.0;
	channels = 2;
	bitdepth = 16;
	FMOD_Sound_GetFormat(sound->fmod_sound, NULL, NULL, &channels, &bitdepth);
	FMOD_Sound_GetDefaults(sound->fmod_sound, &samples, NULL, NULL, NULL);

	void* ptr1 = 0;
	void* ptr2 = 0;
	uint len1 = 0;
	uint len2 = 0;

	FMOD_Sound_Lock(sound->fmod_sound, 0, len, &ptr1, &ptr2, &len1, &len2);

	sound->raw.bits = bitdepth;
	sound->raw.channels = channels;
	sound->raw.length = len;
	sound->raw.dataSize = len;
	sound->raw.samples = samples;
	sound->raw.data = (byte *)malloc(len);
	
	memcpy(sound->raw.data, sound, len);

	if (sound->raw.bits == 8 && sound->flag2c) { //TODO : flag2c rename
		for (int i = 0; i < len; i++) {
			sound->raw.data[i] += 0x80;
		}
	}

	FMOD_Sound_Unlock(sound->fmod_sound, ptr1, ptr2, len1, len2);
	
	sound->raw.MakeBitDepth16();
	sound->raw.MakeStereo();
	sound->raw.MakeSampleRate44100();

	return 1;
}

//4b88b0 //TODO: need test
int RecordSound(AUDIO *aud, SOUNDDATA *sound, double time, double len) {

	if (sound->load && len >= 0.0) {

		int paramlen = time * 44100.0 / 1000.0 * 2.0;
		int soundlen = sound->raw.length / 2;
		if (len > 0.0) {
			soundlen = len * 44100.0 / 1000.0 * 2.0;
			if (soundlen > (sound->raw.length / 2)) soundlen = (sound->raw.length / 2);
		}

		for (int i = 0; i < soundlen; i++) {
			double fade = 1.0;
			if (i < 300) {
				fade = (i + 1) / 300.0;
			}
			if (sound->raw.length - i < 300) {
				fade = (sound->raw.length - i) / 300.0;
			}
			
			if (aud->size <= paramlen + i) {
				//exapnd aud buffer size (*2)
				short* newbuffer = (short*)malloc(aud->size * 2 * 2);
				memset(newbuffer, 0, aud->size * 4);
				memcpy(newbuffer, aud->buffer, aud->size * 2);
				if (aud->buffer != NULL) {
					delete(aud->buffer);
					aud->buffer = NULL;
				}
				aud->buffer = newbuffer;
				aud->size = aud->size * 2;
			}

			int newval = *(short*)((int)aud->buffer + (paramlen + i) * 2) + (float)(*(short*)(sound->raw.data + i * 2)) * aud->volume * fade;

			if (newval >= 0x7fff) *(short*)((int)aud->buffer + (paramlen + i) * 2) = 0x7fff;
			else if (newval < -0x7fff) *(short*)((int)aud->buffer + (paramlen + i) * 2) = -0x8000;
			else *(short*)((int)aud->buffer + (paramlen + i) * 2) = (short)newval;
		}
		return 1;
	}
	return 0;
}

//4b8ad0
int RecordFadeout(AUDIO *aud, double from, double length) {

	if (length <= 0.0) return 0;

	int start = from * 44100.0 / 1000.0 * 2.0;
	int len = length * 44100.0 / 1000.0 * 2.0;
	for (int i = start, c = 0; i < aud->size; i++, c++) {

		if (i < start + len) {
			aud->buffer[i] = aud->buffer[i] * (1 - c / len);
		}
		else {
			aud->buffer[i] = 0;
		}
	}
	return 1;
}

//4b8bb0
int LoadSound(AUDIO *aud, SOUNDDATA *sound, CSTR filepath, int loop, int disableDSP, int previewFlag) {

	CSTR path;
	path.assign(&filepath);

	if (IsFileExist(filepath) == false && IsAltSoundExist(&filepath) == -1) {
		ErrorLogFmtAdd("音声ファイルが見つかりません。%s...\n",path);
		if (sound->load == '\0') {
			sound->filename.fillzero();
			sound->load = '\0';
			return -2;
		}
		return 0;
	}

	if (sound->load == 1) {
		if (filepath.isSame(&sound->filename)) return 1;
		ReleaseSound(aud, sound);
	}

	if (aud->is_fmod_disabled != 1) {
		FMOD_MODE mode = 0;
		FMOD_RESULT result;
		if (aud->cmd_mediaOut) {
			mode = FMOD_ACCURATETIME | FMOD_LOOP_OFF | FMOD_HARDWARE;
			result = FMOD_System_CreateSound(aud->fmodSys, filepath, mode, NULL, &sound->fmod_sound); 
			SOUND_normalize(aud, sound);
		}
		else { //AFTER RELEASE: It can be shortened.
			if (previewFlag == 0) {
				if (loop == 0) {
					if (disableDSP == 0) {
						mode = FMOD_LOOP_OFF | FMOD_SOFTWARE;
					}
					else {
						mode = FMOD_LOOP_OFF | FMOD_HARDWARE;
					}
				}
				else {
					if (disableDSP == 0) {
						mode = FMOD_LOOP_NORMAL | FMOD_SOFTWARE;
					}
					else {
						mode = FMOD_LOOP_NORMAL | FMOD_HARDWARE;
					}
				}
			}
			else {
				if (loop == 0) {
					if (disableDSP == 0) {
						mode = FMOD_CREATESTREAM | FMOD_LOOP_OFF | FMOD_SOFTWARE;
					}
					else {
						mode = FMOD_CREATESTREAM | FMOD_LOOP_OFF | FMOD_HARDWARE;
					}
				}
				else {
					if (disableDSP == 0) {
						mode = FMOD_CREATESTREAM | FMOD_LOOP_NORMAL | FMOD_SOFTWARE;
					}
					else {
						mode = FMOD_CREATESTREAM | FMOD_LOOP_NORMAL | FMOD_HARDWARE;
					}
				}
			}
			result = FMOD_System_CreateSound(aud->fmodSys, filepath, mode, NULL, &sound->fmod_sound);
		}

		sound->flag2c = 0;
		sound->loop = (loop != 0);
		if (result == FMOD_OK) {
			sound->filename.assign(&filepath);
			sound->load = 1;
			FMOD_Sound_GetLength(sound->fmod_sound, &sound->length, 1);
			if (sound->length >= 240000) {
				sound->length = 240000;
			}
		}
		else {
			sound->filename.fillzero();
			sound->load = 0;
			sound->length = -1;
		}
		return 1;
	}

	if (previewFlag) SetCreateSoundDataType(3);
	sound->soundHandle = LoadSoundMem(filepath, 3, -1);
	if (previewFlag) SetCreateSoundDataType(0);
	if (sound->soundHandle == -1) {
		sound->filename.fillzero();
		sound->load = 0;
		return -1;
	}
	sound->filename.assign(&filepath);
	sound->length = GetSoundTotalTime(sound->soundHandle);
	sound->load = 1;
	sound->loop = (loop != 0);
	sound->unused0C = 0; //TODO : rename
	return 1;
}

//4b8f20
int PlaySound(AUDIO *aud, SOUNDDATA *sound, FMOD_CHANNELGROUP *channelgroup, int stage) {

	if (aud->cmd_mediaOut == false) {
		if (sound->load == 0) return -1;
		if (aud->is_fmod_disabled == 1) {
			int v_master;
			int v_bga;
			int pitch;
			double freq = 1;

			StopSoundMem(sound->soundHandle);
			if(sound->loop == 0) PlaySoundMem(sound->soundHandle, 1, 1);
			else PlaySoundMem(sound->soundHandle, 3, 1);

			pitch = (aud->param.pitch_on == 0) ? 0 : aud->param.pitch_amount;
			if (stage < 5) freq = aud->param.stagePitch[stage];

			if (aud->param.fx_volume_on == 0) {
				v_master = aud->param.fadeout_volume * 100.0; // fadeout_volume
				v_bga = 100;
			}
			else {
				v_master = aud->param.volume_master;
				v_bga = aud->param.volume_key;
			}

			SOUND_dxlibFx(*sound, v_master, v_bga, pitch, freq);
			return 0;
		}

		FMOD_SOUND* tempSound;
		
		FMOD_Channel_GetCurrentSound(sound->fmod_channel, &tempSound);
		if (tempSound == sound->fmod_sound) FMOD_Channel_Stop(sound->fmod_channel);
		FMOD_System_PlaySound(aud->fmodSys, FMOD_CHANNEL_FREE, sound->fmod_sound, false, &sound->fmod_channel);
		FMOD_Channel_SetChannelGroup(sound->fmod_channel, channelgroup);
	}
	else if(aud->replay2avi){
		RecordSound(aud, sound, aud->aviTimer, 0);
		return 0;
	}
	return 0;
}


//4b9070
int SOUND_FmodToDxlib(AUDIO *aud) {
	
	int pitch, v_master, _volume_BGM, _volume_key;

	if (aud->param.pitch_on == 0) {
		pitch = 0;
	}
	else {
		pitch = aud->param.pitch_amount;
	}

	if (aud->param.fx_volume_on == 0) {
		v_master = 100;
		_volume_BGM = 100;
		_volume_key = 100;
	}
	else {
		_volume_key = aud->param.volume_key;
		_volume_BGM = aud->param.volume_BGM;
		v_master = aud->param.volume_master;
	}

	SOUND_dxlibFx(aud->sysSound.select, v_master, _volume_BGM, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.folder_open, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.folder_close, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.panel_open, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.option_change, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.decide, v_master, _volume_BGM, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.clear, v_master, _volume_BGM, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.fail, v_master, _volume_BGM, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.stop, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.mine, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.scratch, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.courseclear, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.coursefail, v_master, _volume_key, pitch, 1.0);
	return 0;
}

//4b9340
int ApplySoundFX(AUDIO *aud, int flag, char disable) {

	float _volume,_pitch;
	FMOD_CHANNELGROUP* _chnKey;
	FMOD_CHANNELGROUP* _chnMaster;

	if(aud->cmd_mediaOut) return 0;
	if (aud->is_fmod_disabled == 1) {
		aud->param.eq_on = 0;
		aud->param.fx_on[0] = 0;
		aud->param.fx_on[1] = 0;
		aud->param.fx_on[2] = 0;
		aud->param.pitch_type = 0;
		SOUND_FmodToDxlib(aud);
		return 0;
	}

	if (aud->param.fx_volume_on) {
		FMOD_ChannelGroup_SetVolume(aud->chnBgm, (aud->param.volume_BGM / 100.0) * (aud->param.volume_master / 100.0) * aud->param.fadeout_volume);
		_volume = (aud->param.volume_master / 100.0) * (aud->param.volume_key / 100.0) * aud->param.fadeout_volume;
		_chnKey = aud->chnKey;
	}
	else {
		FMOD_ChannelGroup_SetVolume(aud->chnBgm, aud->param.fadeout_volume);
		_volume = aud->param.fadeout_volume;
		_chnKey = aud->chnKey;
	}
	FMOD_ChannelGroup_SetVolume(_chnKey, _volume);

	for (int i = 0; i < 5; i++) {
		FMOD_ChannelGroup_SetPitch(aud->chnStageBgm[i], aud->param.stagePitch[i]);
		FMOD_ChannelGroup_SetPitch(aud->chnStageKey[i], aud->param.stagePitch[i]);
		FMOD_ChannelGroup_SetVolume(aud->chnStageKey[i], aud->param.stageKeyVolume[i]* aud->param.stageKeyVolume[i]);
		FMOD_ChannelGroup_SetVolume(aud->chnStageBgm[i], aud->param.stageBgmVolume[i]* aud->param.stageBgmVolume[i]);
	}
	
	if (aud->param.DSP_pitch == NULL) {
		if ((aud->param.pitch_type == 1 || aud->param.pitch_type == 2) && (aud->param.pitch_amount != 0) && (aud->param.pitch_on == 1)) {
			FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_PITCHSHIFT, &aud->param.DSP_pitch);
			FMOD_DSP_SetActive(aud->param.DSP_pitch, 1);
			FMOD_System_AddDSP(aud->fmodSys, aud->param.DSP_pitch, NULL);
		}
	}
	else if (aud->param.pitch_type == 0 || aud->param.pitch_amount == 0 || aud->param.pitch_on == 0) {
		FMOD_DSP_Release(aud->param.DSP_pitch);
		aud->param.DSP_pitch = NULL;
	}

	if (aud->param.pitch_on == 0) {
		FMOD_ChannelGroup_SetPitch(aud->chnBgm, 1.0);
		FMOD_ChannelGroup_SetPitch(aud->chnKey, 1.0);
	}
	else if ((aud->param.pitch_type == 0) || (aud->param.pitch_type == 2)) {
		int pitch = aud->param.pitch_amount;
		double dMul;
		dMul = 1.0;
		if (pitch > 0) {
			for (; pitch > 0; pitch--) {
				dMul *= 1.0594631433486938; //check is done. it's 0x3ff0f38fa0000000 same with original
			} //TOFIX? : sound freq/speed doesn't match game note speed (on +-12, 2.00000111 and 2.0 or 0.4999997226 and 0.5)
		}
		else if (pitch < 0) {
			for (pitch = -pitch; pitch > 0; pitch--) {
				dMul /= 1.0594631433486938;
			}
		}
		FMOD_ChannelGroup_SetPitch(aud->chnBgm, dMul);
		FMOD_ChannelGroup_SetPitch(aud->chnKey, dMul);
	}
	else {
		FMOD_ChannelGroup_SetPitch(aud->chnBgm, 1.0);
		FMOD_ChannelGroup_SetPitch(aud->chnKey, 1.0);
	}

	if (aud->param.pitch_on) {
		if (aud->param.pitch_type == 1 && aud->param.DSP_pitch) {
			int pitch = aud->param.pitch_amount;
			double dMul;
			dMul = 1.0;
			if (pitch > 0) {
				for (; pitch > 0; pitch--) {
					dMul *= 1.059463094359; //same with ParseBmsFile()
				}
			}
			else if (pitch < 0) {
				for (pitch = -pitch; pitch > 0; pitch--) {
					dMul /= 1.059463094359;
				}
			}
			FMOD_DSP_SetParameter(aud->param.DSP_pitch, FMOD_DSP_PITCHSHIFT_PITCH, dMul);
		}
		else if (aud->param.pitch_type == 2 && aud->param.DSP_pitch) {
			int pitch = aud->param.pitch_amount;
			double dMul;
			dMul = 1.0;
			if (pitch > 0) {
				for (; pitch > 0; pitch--) {
					dMul /= 1.059463094359;
				}
			}
			else if (pitch < 0) {
				for (pitch = -pitch; pitch > 0; pitch--) {
					dMul *= 1.059463094359;
				}
			}
			FMOD_DSP_SetParameter(aud->param.DSP_pitch, FMOD_DSP_PITCHSHIFT_PITCH, dMul);
		}
	}

	//apply EQ
	for (int i = 0; i < 6; i++) { //TOFIX : 6 to 7
		if (aud->param.DSP_eq[i] == NULL) {
			if (aud->param.eq_gain[i] && aud->param.eq_on == 1) {
				FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_PARAMEQ, &aud->param.DSP_eq[i]);
				FMOD_DSP_SetActive(aud->param.DSP_eq[i], 1);
				FMOD_System_AddDSP(aud->fmodSys, aud->param.DSP_eq[i], NULL);
			}
		}
		else if (aud->param.eq_gain[i] == 0 || aud->param.eq_on == 0) {
			FMOD_DSP_Release(aud->param.DSP_eq[i]);
			aud->param.DSP_eq[i] = NULL;
		}
		if (aud->param.DSP_eq[i]) {
			int center = 0;
			float bandwidth = 0.0;
			switch (i) {
				case 0:
					center = 63;
					bandwidth = 1.5;
					break;
				case 1:
					center = 160;
					bandwidth = 1.5;
					break;
				case 2:
					center = 400;
					bandwidth = 1.5;
					break;
				case 3:
					center = 1000;
					bandwidth = 1.5;
					break;
				case 4:
					center = 2500;
					bandwidth = 1.5;
					break;
				case 5:
					center = 6300;
					bandwidth = 1.5;
					break;
				case 6:
					center = 16000;
					bandwidth = 1.5;
					break;
			}

			int pitch = aud->param.eq_gain[i];
			float dMul = 1.0;
			if (pitch > 0) {
				for (; pitch > 0; pitch--) {
					dMul *= 1.0352649688720703;
				}
			}
			else if (pitch < 0) {
				for (pitch = -pitch; pitch > 0; pitch--) {
					dMul /= 1.0352649688720703;
				}
			}
			
			if (aud->param.eq_gain[i]) {
				FMOD_DSP_SetParameter(aud->param.DSP_eq[i], FMOD_DSP_PARAMEQ_CENTER, center);
				FMOD_DSP_SetParameter(aud->param.DSP_eq[i], FMOD_DSP_PARAMEQ_BANDWIDTH, bandwidth);
				FMOD_DSP_SetParameter(aud->param.DSP_eq[i], FMOD_DSP_PARAMEQ_GAIN, dMul);
			}
		}
	}

	//apply FX
	for (int i = 0; i < 3; i++) {
		FMOD_DSP_TYPE type;
		bool isSameType;
		if (aud->param.DSP_fx[i] == 0)
			type = FMOD_DSP_TYPE_UNKNOWN;
		else
			FMOD_DSP_GetType(aud->param.DSP_fx[i], &type);

		switch (aud->param.fxType[i]) {
			case 1:
				isSameType = type == FMOD_DSP_TYPE_REVERB;
				break;
			case 2:
				isSameType = type == FMOD_DSP_TYPE_ECHO;
				break;
			case 3:
				isSameType = type == FMOD_DSP_TYPE_LOWPASS;
				break;
			case 4:
				isSameType = type == FMOD_DSP_TYPE_HIGHPASS;
				break;
			case 5:
				isSameType = type == FMOD_DSP_TYPE_FLANGE;
				break;
			case 6:
				isSameType = type == FMOD_DSP_TYPE_CHORUS;
				break;
			case 7:
				isSameType = type == FMOD_DSP_TYPE_DISTORTION;
				break;
			default:
				isSameType = type == FMOD_DSP_TYPE_UNKNOWN;
		}
		if (!isSameType || flag) {
			if (aud->param.DSP_fx[i]) FMOD_DSP_Release(aud->param.DSP_fx[i]);

			switch (aud->param.fxType[i]) {
			case 1:
				FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_REVERB, &aud->param.DSP_fx[i]);
				break;
			case 2:
				FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_ECHO, &aud->param.DSP_fx[i]);
				break;
			case 3:
				FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_LOWPASS, &aud->param.DSP_fx[i]);
				break;
			case 4:
				FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_HIGHPASS, &aud->param.DSP_fx[i]);
				break;
			case 5:
				FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_FLANGE, &aud->param.DSP_fx[i]);
				break;
			case 6:
				FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_CHORUS, &aud->param.DSP_fx[i]);
				break;
			case 7:
				FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_DISTORTION, &aud->param.DSP_fx[i]);
				break;
			default:
				aud->param.DSP_fx[i] = NULL;
				continue;
			}
			FMOD_DSP_SetActive(aud->param.DSP_fx[i], 0);
		}
		if (aud->param.DSP_fx[i]) {
			FMOD_BOOL isActive;
			FMOD_DSP_GetActive(aud->param.DSP_fx[i], &isActive);
			if (isActive == 0) {
				if (aud->param.fx_on[i] == 1 && aud->param.fxType[i]) {
					FMOD_DSP_SetActive(aud->param.DSP_fx[i], 1);
					switch (aud->param.fxChannel[i]) {
						default:
						case 0 :
							FMOD_System_AddDSP(aud->fmodSys, aud->param.DSP_fx[i], NULL);
							break;
						case 1:
							FMOD_ChannelGroup_AddDSP(aud->chnKey, aud->param.DSP_fx[i], NULL);
							break;
						case 2:
							FMOD_ChannelGroup_AddDSP(aud->chnBgm, aud->param.DSP_fx[i], NULL);
							break;
					}
				}
			}
			else if (aud->param.fx_on[i] == 0 || aud->param.fxType[i] == 0) {
				FMOD_DSP_Remove(aud->param.DSP_fx[i]);
				FMOD_DSP_SetActive(aud->param.DSP_fx[i], 0);
			}

			if (aud->param.fx_on[i]) {
				float fTmp;
				switch (aud->param.fxType[i]) {
					case 1:
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_REVERB_ROOMSIZE, aud->param.fxParam[i][0] / 100.0);
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_REVERB_WIDTH, 1.0);
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_REVERB_WETMIX, aud->param.fxParam[i][1] / 100.0);
						break;
					case 2:
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_ECHO_DELAY, aud->param.fxParam[i][0] * 10.0);
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_ECHO_WETMIX, aud->param.fxParam[i][1] / 100.0);
						break;
					case 3:
						fTmp = 22000.0;
						for (int j = aud->param.fxParam[i][0]; j < 100; j++) {
							fTmp /= 1.06;
						}
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_LOWPASS_CUTOFF, fTmp);
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_LOWPASS_RESONANCE, aud->param.fxParam[i][1] * 9.0 / 100.0 + 1.0);
						break;
					case 4:
						fTmp = 22.0;
						for (int j = aud->param.fxParam[i][0]; j > 0 ; j--) {
							fTmp *= 1.06;
						}
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_HIGHPASS_CUTOFF, fTmp);
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_HIGHPASS_RESONANCE, aud->param.fxParam[i][1] * 9.0 / 100.0 + 1.0);
						break;
					case 5:
						fTmp = 0.02;
						for (int j = aud->param.fxParam[i][1]; j > 0; j--) {
							fTmp *= 1.06;
						}
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_FLANGE_DEPTH, aud->param.fxParam[i][0] / 100.0);
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_FLANGE_RATE, fTmp);
						break;
					case 6:
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_CHORUS_RATE, aud->param.fxParam[i][0] / 5.0);
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_CHORUS_DELAY, aud->param.fxParam[i][1]);
						break;
					case 7:
						FMOD_DSP_SetParameter(aud->param.DSP_fx[i], FMOD_DSP_DISTORTION_LEVEL, aud->param.fxParam[i][0] / 100.0);
						break;
				}
			}
		}
	}

	FMOD_System_Update(aud->fmodSys);
	return 1;
}

//4b9d70
int InitFade(AUDIO *aud){
	aud->param.fadeout_volume = 1.0;
	aud->param.fadePreviewCurrentVolume = 1.0;
	aud->param.time_fadeout_start = -1;
	aud->param.time_fadeout_end = -1;
	aud->param.time_fadePreview_start = -1;
	aud->param.time_fadePreview_end = -1;
	ApplySoundFX(aud, 0, '\0');
	return 1;
}

//4b9db0
int SetVolumeByFade(AUDIO *aud){
	float fVar1;
	DWORD timeNow;

	timeNow = timeGetTime();
	if (aud->param.time_fadeout_start != -1) {
		if (timeNow < aud->param.time_fadeout_end) {
			fVar1 = (float)(aud->param.time_fadeout_end - timeNow) / (float)(aud->param.time_fadeout_end - aud->param.time_fadeout_start);
		}
		else {
			fVar1 = 0.0;
		}
		aud->param.fadeout_volume = fVar1;
		ApplySoundFX(aud, 0, '\0');
	}
	if (aud->param.time_fadePreview_start != -1) {
		if (timeNow < aud->param.time_fadePreview_end) {
			fVar1 = (float)(timeNow - aud->param.time_fadePreview_start) / (float)(aud->param.time_fadePreview_end - aud->param.time_fadePreview_start);
			fVar1 = (1.0 - fVar1) * aud->param.fadePreviewStartVolume + aud->param.fadePreviewTargetVolume * fVar1;
		}
		else {
			fVar1 = aud->param.fadePreviewTargetVolume;
		}
		aud->param.fadePreviewCurrentVolume = fVar1;
		if (aud->param.fadePreviewCurrentVolume < 0.05) {
			aud->param.fadePreviewCurrentVolume = 0.025;
		}
		if (1.0 < aud->param.fadePreviewCurrentVolume) {
			aud->param.fadePreviewCurrentVolume = 1.0;
		}
		SetSoundVolume(aud, &(aud->sysSound).select, aud->param.fadePreviewCurrentVolume);
		SetSoundVolume(aud, &(aud->sysSound).exselect, aud->param.fadePreviewCurrentVolume);
	}
	return 1;
}

//4b9ed0
int InitSound(AUDIO *aud, uint bufferLength, int numBuffer, char fDisable, int outputType, int driver){

	FMOD_SYSTEM* pFmodSystem;
	int numDrivers;
	char driverName[256];
	int chn2D, chn3D, chnTotal;
	int numHardwareChannels;

	if (aud->cmd_mediaOut) {
		FMOD_System_Create(&aud->fmodSys);
		FMOD_System_Init(aud->fmodSys, 1, 0, NULL);
		return 1;
	}
	else if (aud->is_fmod_disabled != true) {
		ErrorLogAdd("サウンドシステムの初期化を行います。\n");
		FMOD_System_Create(&aud->fmodSys);
		if (outputType == 0) {
			FMOD_System_SetOutput(aud->fmodSys, FMOD_OUTPUTTYPE_DSOUND);
			ErrorLogAdd("OUTPUT TYPE:DIRECTSOUND\n");
		}
		else if (outputType == 1) {
			FMOD_System_SetOutput(aud->fmodSys, FMOD_OUTPUTTYPE_WASAPI);
			ErrorLogAdd("OUTPUT TYPE:WASAPI\n");
		}
		else if (outputType == 2) {
			FMOD_System_SetOutput(aud->fmodSys, FMOD_OUTPUTTYPE_ASIO);
			ErrorLogAdd("OUTPUT TYPE:ASIO\n");
		}
		else{
			FMOD_System_SetOutput(aud->fmodSys, FMOD_OUTPUTTYPE_DSOUND);
			ErrorLogAdd("OUTPUT TYPE:DIRECTSOUND\n");
		}

		FMOD_System_GetNumDrivers(aud->fmodSys, &numDrivers);
		if (driver > numDrivers - 1) {
			driver = 0;
		}
		FMOD_System_GetDriverInfo(aud->fmodSys, driver, driverName, sizeof(driverName), NULL);
		ErrorLogFmtAdd("PLAYBACK DRIVER:%s\n", driverName);
		FMOD_System_SetDriver(aud->fmodSys, driver);
		ErrorLogAdd("バッファサイズの設定を行います...");
		ErrorLogAdd(GetFMODerror(FMOD_System_SetDSPBufferSize(aud->fmodSys, bufferLength, numBuffer)));
		ErrorLogAdd("\n");
		FMOD_System_SetSoftwareChannels(aud->fmodSys, 0x100);
		FMOD_System_SetHardwareChannels(aud->fmodSys, 0x100, 0x100, 0x100, 0x100);
		FMOD_System_Init(aud->fmodSys, 0x100, 0, NULL);
		FMOD_System_GetHardwareChannels(aud->fmodSys, &chn2D, &chn3D, &chnTotal);
		ErrorLogFmtAdd("2Dチャンネル%d 3Dチャンネル%d 合計%d\n", chn2D, chn3D, chnTotal);
		FMOD_System_CreateChannelGroup(aud->fmodSys, "bgm", &aud->chnBgm);
		FMOD_System_CreateChannelGroup(aud->fmodSys, "key", &aud->chnKey);
		FMOD_System_GetMasterChannelGroup(aud->fmodSys, &aud->chnMaster);
		
		CSTR buf;
		for (int i = 0; i < 5; i++) {
			cstrSprintf(&buf, "stage%d_bgm", i);
			FMOD_System_CreateChannelGroup(aud->fmodSys, "chname", &aud->chnStageBgm[i]);
			FMOD_ChannelGroup_AddGroup(aud->chnBgm, aud->chnStageBgm[i]);
			cstrSprintf(&buf, "stage%d_key", i);
			FMOD_System_CreateChannelGroup(aud->fmodSys, "chname", &aud->chnStageKey[i]);
			FMOD_ChannelGroup_AddGroup(aud->chnKey, aud->chnStageKey[i]);
		}
		aud->param.DSP_eq[0] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[1] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[2] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[3] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[4] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[5] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[6] = (FMOD_DSP *)0x0;
		aud->param.DSP_fx[0] = (FMOD_DSP *)0x0;
		aud->param.DSP_fx[1] = (FMOD_DSP *)0x0;
		aud->param.DSP_fx[2] = (FMOD_DSP *)0x0;
		aud->param.DSP_pitch = (FMOD_DSP *)0x0;
		aud->param.stagePitch[0] = 1.0;
		aud->param.stagePitch[1] = 1.0;
		aud->param.stagePitch[2] = 1.0;
		aud->param.stagePitch[3] = 1.0;
		aud->param.stagePitch[4] = 1.0;
		ApplySoundFX(aud, 1, fDisable);
		ErrorLogFmtAdd("サウンドの初期化が成功しました。\n");
		aud->param.fadeout_volume = 1.0;
		aud->param.fadePreviewCurrentVolume = 1.0;
		aud->param.time_fadeout_start = -1;
		aud->param.time_fadeout_end = -1;
		aud->param.time_fadePreview_start = -1;
		aud->param.time_fadePreview_end = -1;
		ApplySoundFX(aud, 0, '\0');
		return 0;
	}
	return 1;
}

//4ba240
RAWSOUND::RAWSOUND() {
	channels = 0;
	samples = 0;
	bits = 0;
	length = 0;
	dataSize = 0;
	data = NULL;
}

//4ba270
void RAWSOUND::ExpandBuffer(int minSize) { 

	size_t newSize = this->dataSize;
	byte* buf;
	if (newSize < minSize) {
		do {
			newSize = newSize * 2;
		} while (newSize < minSize);
		buf = (byte*)malloc(newSize);
		memcpy(buf, this->data, this->dataSize);
		memset(&buf[this->dataSize], 0, newSize - this->dataSize);
		delete(this->data);
		this->data = buf;
		this->dataSize = newSize;
	}
	return;
}

//4ba2d0
void RAWSOUND::MakeBitDepth16(void){

	if (this->bits == 8) {
		ExpandBuffer(this->length * 2);
		for (int i = this->length - 1; i >= 0; i--) {
			*(word*)(&this->data[i * 2]) = (this->data[i] - 0x80) * 0x100;
		}
		this->bits = 16;
		this->length = this->length * 2;
	}
	return;
}

//4ba330
void RAWSOUND::MakeStereo(void){

	if (this->channels == 1) {
		ExpandBuffer(this->length * 2);

		for (int i = this->length/2 - 1; i >= 0; i--) {
			*(word*)(&this->data[i * 4]) = *(word*)(&this->data[i * 2]);
			*(word*)(&this->data[i * 4] + 2) = *(word*)(&this->data[i * 2]);
		}
		this->channels = 2;
		this->length = this->length * 2;
	}
	return;
}

//4ba380 //TODO: I cannot understand this... replace with library would be good
void RAWSOUND::MakeSampleRate44100(void) {
	
	dword* unkd[20];

	if (this->samples != 44100) {
		int size = (int)(44100.0 / (double)this->samples * this->length) / 4 * 4;  // are there any loss??
		this->ExpandBuffer(size);

		int count = 0;
		if (this->samples < 44100) {
			word* unk = NULL;
			for (int i = size - 1; i >= 0; i--) {
				
				int aapos = round(i / (44100.0 / (double)this->samples));
				if (unk == (word*)(this->data + aapos*4)) {
					unkd[count] = (dword*)(this->data + i*4);
					count++;
				}
				else {
					for (int j = 0; j < count; j++) {
						int aupos = i / (44100.0 / (double)this->samples);
						DWORD val = *(dword*)(this->data + aupos * 4);
						WORD hi = (int)((double)(unk[1] - HIWORD(val)) * (count / (double)(count + 1))) + HIWORD(val);
						WORD lo = (int)((double)(unk[0] - LOWORD(val)) * (count / (double)(count + 1))) + LOWORD(val);

						if ((int)hi >= 0x8000) hi = 0x7fff;
						else if ((int)hi < -0x8000) hi = 0xffff8000;

						if ((int)lo >= 0x8000) lo = 0x7fff;
						else if ((int)lo < -0x8000) lo = 0xffff8000;

						*unkd[j] = (hi << 16) | (lo && 0xffff);
					}

					int aupos = i / (44100.0 / (double)this->samples);
					*(dword*)(this->data + i * 4) = *(dword*)(this->data + aupos * 4);
					count = 0;
					unk = (word*)(this->data + aupos * 4);
				}

			}
		}
		else {
			for (int i = 0; i < size; i++) {
				int aupos = i / (44100.0 / (double)this->samples);
				*(dword*)(this->data + i*4) = *(dword*)(this->data + aupos*4);
			}
		}

		this->length = size;
		this->samples = 44100;
	}
	return;
}

////Internet Ranking section
//4ba5e0
int CMP_PlayerByExscore(const void *p1, const void *p2) {
	
	RANKINGPLAYER* s1 = (RANKINGPLAYER*)p1;
	RANKINGPLAYER* s2 = (RANKINGPLAYER*)p2;

	return (s2->pg * 2 + s2->gr) - (s1->pg * 2 + s1->gr);
}

//4ba670
void RANKING::ExpandRankingBuffer(int add) {
	
	this->ranking = (RANKINGPLAYER*)realloc(this->ranking, (this->rankingMax + add) * sizeof(RANKINGPLAYER));

	for (int i = this->rankingMax; i < this->rankingMax + add; i++) {
		memset(&this->ranking[i], 0, sizeof(RANKINGPLAYER));
	}

	this->rankingMax += add;
	return;
}

//4ba6e0
void RANKING::Init() {
	this->unk_4.fillzero();
	this->unk_0.fillzero();
	this->lastupdate.fillzero();
	this->clearPlayers[0] = 0;
	this->clearPlayers[1] = 0;
	this->clearPlayers[2] = 0;
	this->clearPlayers[3] = 0;
	this->clearPlayers[4] = 0;
	this->clearPlayers[5] = 0;
	this->unused30 = NULL;
	this->rankingCursor = 0;
	this->totalPlaycount = 0;
	this->rankingCount = 0;
	this->myRanking = 0;
	this->showRanking = '\0';
	this->unused48 = NULL;
	this->unused44 = NULL;
	for (int i = 0; i < this->rankingMax; i++) {
		this->ranking[i].name.fillzero();
		this->ranking[i].id = 0;
		this->ranking[i].sp = 0;
		this->ranking[i].dp = 0;
		this->ranking[i].clear = 0;
		this->ranking[i].notes = 0;
		this->ranking[i].combo = 0;
		this->ranking[i].pg = 0;
		this->ranking[i].gr = 0;
		this->ranking[i].gd = 0;
		this->ranking[i].bd = 0;
		this->ranking[i].pr = 0;
		this->ranking[i].minbp = 0;
		this->ranking[i].option = 0;
		this->ranking[i].sussussuspected = 0;
		this->ranking[i].playcount = 0;
		this->ranking[i].ranking = 0;
		this->ranking[i].comment.fillzero();
	}

	return;
}

//4ba790
int RANKING::ParseXML(CSTR path) {
	
	TiXmlDocument *hXml;
	TiXmlElement *cur;
	
	Init();
	ErrorLogFmtAdd("ランキングデータのパースを開始します。パス%s\n", path);

	hXml = new TiXmlDocument(path);
	if (hXml->LoadFile(TIXML_ENCODING_UNKNOWN) == false) {
		if (hXml) {
			delete(hXml);
		}
		ErrorLogFmtAdd("ランキングファイルが見つかりません\n");
		return 0;
	}

	cur = hXml->FirstChildElement("lastupdate");
	if (cur && cur->ToElement()) {
		cstrSprintf(&this->lastupdate, "%s", cur->ToElement()->GetText());
	}

	cur = hXml->FirstChildElement("ranking");
	if (!cur) {
		if (hXml) delete(hXml);
		ErrorLogFmtAdd("ランキングの読み込みに失敗しました。xmlの書式をミスってるかも。\n");
		return 0;
	}

	cur = cur->FirstChildElement("score");
	if (!cur) {
		if (hXml) delete(hXml);
		ErrorLogFmtAdd("スコアの更新がありません。\n");
		return 0;
	}

	while (cur) {
		TiXmlElement *val;
		val = cur->FirstChildElement("name");
		if (val && val->ToElement()) {
			cstrSprintf(&ranking[rankingCount].name, "%s", val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("id");
		if (val) {
			ranking[rankingCount].id = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("sp");
		if (val) {
			ranking[rankingCount].sp = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("dp");
		if (val) {
			ranking[rankingCount].dp = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("clear");
		if (val) {
			ranking[rankingCount].clear = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("notes");
		if (val) {
			ranking[rankingCount].notes = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("combo");
		if (val) {
			ranking[rankingCount].combo = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("pg");
		if (val) {
			ranking[rankingCount].pg = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("gr");
		if (val) {
			ranking[rankingCount].gr = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("gd");
		if (val) {
			ranking[rankingCount].gd = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("bd");
		if (val) {
			ranking[rankingCount].bd = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("pr");
		if (val) {
			ranking[rankingCount].pr = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("minbp");
		if (val) {
			ranking[rankingCount].minbp = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("option");
		if (val) {
			ranking[rankingCount].option = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("comment");
		if (val && val->ToElement()) {
			cstrSprintf(&ranking[rankingCount].comment, "%s", val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("playcount");
		if (val) {
			ranking[rankingCount].playcount = atol(val->ToElement()->GetText());
		}

		this->rankingCount++;
		if (this->rankingCount == this->rankingMax) ExpandRankingBuffer(1000);

		cur = cur->NextSiblingElement();
	}

	if (hXml) delete(hXml);
	ErrorLogFmtAdd("ランキングデータのパースに成功しました。 合計プレイヤー %d\n", rankingCount);
	
	qsort(ranking, rankingCount, sizeof(RANKINGPLAYER), CMP_PlayerByExscore);

	for (int i = 0; i < rankingCount; i++) {
		totalPlaycount += ranking[i].playcount;
		if (0 <= ranking[i].clear && ranking[i].clear <= 5) clearPlayers[ranking[i].clear]++;
	}

	int fpg = ranking[0].pg;
	int fgr = ranking[0].gr;
	int rank = 1;
	for (int i = 0; i < rankingCount; i++) {
		if (ranking[i].gr + ranking[i].pg * 2 < fgr + fpg * 2) rank = i + 1;
		ranking[i].ranking = rank;
		if (ranking[i].id == myID) myRanking = rank;
		if (ranking[i].id == rivalID) rivalRanking = rank;
	}
	return 1;
}

//4bad70
int CheckRivaldataNew(int rivalID) {

	sqlite3 *pDb;
	sqlite3_stmt *pStmt;
	CSTR path;
	int ret = 0;
	cstrSprintf(&path, "LR2files/Rival/%d.db", rivalID);
	
	if (!IsFileExist(path)) return 0;

	sqlite3_open(path, &pDb);

	SQL_prepare("SELECT r_lastupdate FROM rival ORDER BY r_lastupdate DESC LIMIT 0,1", pDb, &pStmt);
	if (sqlite3_step(pStmt) == 100) {
		ret = sqlite3_column_int(pStmt, 0);
	}
	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
	return ret;
}

//4baea0
int ParseRivalData(long ID) {

	CSTR name;
	CSTR path;
	TiXmlDocument *hXml;
	TiXmlElement *cur, *val;
	sqlite3 *pRivalDB;

	cstrSprintf(&path, "LR2files/Rival/%d.db", ID);
	sqlite3_open(path, &pRivalDB);
	SQL_Run("CREATE TABLE rival(hash TEXT primary key,r_clear INTEGER,r_totalnotes INTEGER,r_maxcombo INTEGER,r_perfect INTEGER,r_great INTEGER,r_good INTEGER,r_bad INTEGER,r_poor INTEGER,r_minbp INTEGER,r_option INTEGER,r_lastupdate INGEGER)", pRivalDB);
	
	cstrSprintf(&path, "LR2files/Rival/%d.xml", ID);
	hXml = new TiXmlDocument(path);
	if (hXml->LoadFile(TIXML_ENCODING_UNKNOWN) == false) {
		if (hXml) {
			delete(hXml);
		}
		printfDx("ID%4d:ライバルデータにアクセスできません。\n", ID);
		return 0;
	}

	cur = hXml->FirstChildElement("rivalname");
	if(cur){
		if (cur->ToElement()) {
			cstrSprintf(&name, "%s", cur->ToElement()->GetText());
		}
		ErrorLogFmtAdd("ライバル名:%s\n", name);
	}

	cur = hXml->FirstChildElement("scorelist");
	if (!cur) {
		if (hXml) {
			delete(hXml);
		}
		ErrorLogFmtAdd("ライバルデータ読み込みに失敗しました。xmlが変かも\n");
		printfDx("ライバルデータの読み込みに失敗しました。ID%06d\n", ID);
		return 0;
	}

	cur = cur->FirstChildElement("score");
	if (!cur) {
		if (hXml) {
			delete(hXml);
		}
		ErrorLogFmtAdd("ライバルデータの読み込みに失敗しました。スコアが存在しないかも\n");
		printfDx("ID%06d:ライバルデータ[%s]の更新はありません。\n", ID,	name);
		return 0;
	}

	CSTR hash, query;
	int count = 0;
	int clear, notes, combo, pg, gr, gd, bd, pr, minbp, option, lastupdate;
	SQL_Run("BEGIN", pRivalDB);

	while (cur) {
		count++;

		val = cur->FirstChildElement("hash");
		if(val && val->ToElement()){
			cstrSprintf(&hash, "%s", val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("clear");
		if (val && val->ToElement()) {
			clear = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("notes");
		if (val && val->ToElement()) {
			notes = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("combo");
		if (val && val->ToElement()) {
			combo = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("pg");
		if (val && val->ToElement()) {
			pg = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("gr");
		if (val && val->ToElement()) {
			gr = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("gd");
		if (val && val->ToElement()) {
			gd = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("bd");
		if (val && val->ToElement()) {
			bd = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("pr");
		if (val && val->ToElement()) {
			pr = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("minbp");
		if (val && val->ToElement()) {
			minbp = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("option");
		if (val && val->ToElement()) {
			option = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("lastupdate");
		if (val && val->ToElement()) {
			lastupdate = atol(val->ToElement()->GetText());
		}

		cstrSprintf(&query, "INSERT INTO rival (hash,r_clear,r_totalnotes,r_maxcombo,r_perfect,r_great,r_good,r_bad,r_poor,r_minbp,r_option,r_lastupdate) VALUES(\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
			hash, clear, notes, combo, pg, gr, gd, bd, pr, minbp, option, lastupdate);
		if (SQL_Run(query, pRivalDB)) {
			cstrSprintf(&query, "UPDATE rival SET r_clear=%d,r_totalnotes=%d,r_maxcombo=%d,r_perfect=%d,r_great=%d,r_good=%d,r_bad=%d,r_poor=%d,r_minbp=%d,r_option=%d,r_lastupdate=%d WHERE hash=\'%s\'",
				clear, notes, combo, pg, gr, gd, bd, pr, minbp, option, lastupdate, hash);
			SQL_Run(query, pRivalDB);
		}

		cur = cur->NextSiblingElement();
	}
	if (hXml) {
		delete(hXml);
	}
	remove(path);

	SQL_Run("COMMIT", pRivalDB);
	sqlite3_close(pRivalDB);

	CSTR cfolder;
	cstrSprintf(&cfolder, "#COMMAND __RIVAL__\n#MAXTRACKS %d\n#CATEGORY ライバルフォルダ\n#TITLE %s\n#INFORMATION_A %sのプレイした曲を表示します\n#INFORMAION_B\n", ID, name, name);
	cstrSprintf(&path, "LR2files/Rival/%d.lr2folder", ID);
	cfolder.toFile(path);
	printfDx("ID%06d:ライバルデータ[%s]を更新しました。更新スコア数%d\n", ID, name, count);
	return 1;
}

//4bb500
int NETWORK::GetInsaneList() {

	TiXmlDocument *hXml;
	TiXmlElement *cur;
	sqlite3 *pSongDB;

	cstrSprintf(&this->param," ");
	this->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/getinsanelist.cgi";
	if (HTTPrequest() == 1) {
		this->httpResult.toFile("LR2files/Database/exlevel.xml");
		printfDx("発狂難度リストをダウンロードしました。\n");
		ErrorLogFmtAdd("発狂難度リストをダウンロードしました。\n");
	}
	else{
		printfDx("発狂難度リストの更新に失敗しました。");
		ErrorLogFmtAdd("発狂難度リストの更新に失敗しました。\n");
	}
	ScreenFlip();

	hXml = new TiXmlDocument("LR2files/Database/exlevel.xml");
	if (hXml->LoadFile(TIXML_ENCODING_UNKNOWN) == false) {
		if (hXml) {
			delete(hXml);
		}
		printfDx("発狂レベルリストにアクセスできません。\n");
		ErrorLogFmtAdd("発狂難度リストにアクセスできません。\n");
		return 0;
	}
	cur = hXml->FirstChildElement("list");
	if (!cur) {
		if (hXml) {
			delete(hXml);
		}
		printfDx("発狂レベルリストの読み込みに失敗しました。\n");
		return 0;
	}
	cur = cur->FirstChildElement("song");
	if (!cur) {
		if (hXml) {
			delete(hXml);
		}
		printfDx("発狂レベルリストの更新はありません。\n");
		return 0;
	}

	CSTR query;
	CSTR hash;
	sqlite3_open("LR2files/Database/song.db", &pSongDB);
	SQL_Run("BEGIN", pSongDB);
	int exlevel;
	while (cur) {
		TiXmlElement *val;
		val = cur->FirstChildElement("hash");
		if (val && val->ToElement()) {
			cstrSprintf(&hash, "%s", val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("exlevel");
		if (val) {
			exlevel = atol(val->ToElement()->GetText());
		}

		cstrSprintf(&query, "UPDATE song SET exlevel=%d WHERE hash=\'%s\'",exlevel,hash);
		SQL_Run(query, pSongDB);

		cur = cur->NextSiblingElement();
	}
	
	if (hXml) delete(hXml);
	SQL_Run("COMMIT", pSongDB);
	sqlite3_close(pSongDB);
	return 1;
}

//4bb8d0
CSTR UrlEncode(CSTR in) {

	CSTR ret;

	for (int i = 0; i < in.length(); i++) {
		CSTR buf;
		if (isalnum((uchar)*in.atPos(i)) || (*in.atPos(i) == '_') || (*in.atPos(i) == '.') || (*in.atPos(i) == '-')) {
			*buf.atPos(0) = *in.atPos(i);
			*buf.atPos(1) = '\0';

		}
		else if ((*in.atPos(i) == ' ')) {
			*buf.atPos(0) = '+';
			*buf.atPos(1) = '\0';
		}
		else {
			cstrSprintf(&buf, "%%%02X", (uchar)*in.atPos(i));
		}

		ret.add(buf);
	}
	return ret;
}

//4bb820
RANKING::RANKING() {
	ranking = (RANKINGPLAYER*)malloc(sizeof(RANKINGPLAYER) * 1000);
	rankingCount = 0;
	rankingMax = 1000;
	for (int i = 0; i < rankingMax; i++) memset(&ranking[i], 0, sizeof(RANKINGPLAYER));
	Init();
}

//4bbb80
int NETWORK::Init() {

	InitializeCriticalSection((LPCRITICAL_SECTION)&this->criticalSection);
	this->unused234 = 0;
	this->waitForHandle = '\0';
	this->domain = "www.dream-pro.info";
	this->timeout = 15000;
	this->IRstatus = 0;
	this->hHandle = NULL;
	this->isOnline = 0;
	this->IR_ID = 0;
	this->rankingData.target_ID = 0;
	this->waitTime = 10000;
	this->rankingData.Init();
	return 1;
}

//4bbd00
void NETWORK::Lock() {
	EnterCriticalSection(&this->criticalSection);
}

//4bbd10
void NETWORK::Unlock() {
	LeaveCriticalSection(&this->criticalSection);
}

//4bbd20
int NETWORK::HTTPrequest() {
	SOCKET s;
	struct sockaddr_in server;
	hostent* host;
	CSTR request(2048);
	CSTR recvBuf(2048);

	EnterCriticalSection(&this->criticalSection);

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (s == 0xffffffff) {
		cstrSprintf(&this->request_debug, "ソケットエラー : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		closesocket(s);
		LeaveCriticalSection(&this->criticalSection);
		return this->isRequestSuccess = 0;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	server.sin_addr.S_un.S_addr = inet_addr(this->domain);
	if (server.sin_addr.S_un.S_addr == 0xffffffff) {
		host = gethostbyname(this->domain);
		if (host == NULL) {
			if (WSAGetLastError() == 11001) cstrSprintf(&this->request_debug, "ホストが見つかりません : %d\n", WSAGetLastError());
			else cstrSprintf(&this->request_debug, "その他のエラーです : %d\n", WSAGetLastError());
			ErrorLogAdd(this->request_debug);
			closesocket(s);
			LeaveCriticalSection(&this->criticalSection);
			return this->isRequestSuccess = 0;
		}

		for (int i = 0; host->h_addr_list[i]; i++) {
			memcpy(&server.sin_addr.S_un,host->h_addr_list[i],4);
			if (connect(s, (sockaddr*)&server, sizeof(struct sockaddr_in)) == 0) {
				if (host->h_addr_list[i] == NULL) {
					cstrSprintf(&this->request_debug, "接続に失敗しました : %d\n", WSAGetLastError());
					ErrorLogAdd(this->request_debug);
					closesocket(s);
					LeaveCriticalSection(&this->criticalSection);
					return this->isRequestSuccess = 0;
				}
				break;
			}
		}
	}
	else {
		if (connect(s, (sockaddr*)&server, sizeof(struct sockaddr_in)) == 0) {
			cstrSprintf(&this->request_debug, "接続に失敗しました : %d\n", WSAGetLastError());
			ErrorLogAdd(this->request_debug);
			closesocket(s);
			LeaveCriticalSection(&this->criticalSection);
			return this->isRequestSuccess = 0;
		}
	}
		
	unsigned long argp = 1;
	ioctlsocket(s, 0x8004667e, &argp);
		
	request.fillzero();
	cstrSprintf(&request, "POST %s HTTP/1.0\r\nContent-Length:%d\n\n%s", this->target_URL, this->param.length(), this->param);

	if (send(s, request, request.length() + 1, 0) < 0) {
		cstrSprintf(&this->request_debug, "データの送信に失敗しました : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		closesocket(s);
		LeaveCriticalSection(&this->criticalSection);
		return this->isRequestSuccess = 0;
	}

	request.fillzero(); //from this time, request is used as response result

	int timeout = this->timeout / 10;
	int i = 0;
	for (; i != timeout; i++) {
		recvBuf = "";
		int recvSize = recv(s, recvBuf, recvBuf.msize() - 1, 0);
		if (this->waitForHandle == 1) {
			request = "DISCONNECT";
			break;
		}

		if (recvSize < 0) {
			if (WSAGetLastError() != 10035) { //WSAEWOULDBLOCK
				cstrSprintf(&this->request_debug, "データの受信に失敗しました : %d\n", WSAGetLastError());
				if(this->rankingData.myID == 1) ErrorLogAdd(this->request_debug);
				request.fillzero();
				break;
			}
		}
		else if (recvSize == 0) {
			break;
		}
		else {
			request.add(recvBuf);
		}
		Sleep(10);
	}
	if (i == timeout) {
		request = "TIMEOUT";
	}

	closesocket(s);
	LeaveCriticalSection(&this->criticalSection);

	if (request.findStrPos("#") != -1) {
		this->httpResult = request.right(request.length() - (request.findStrPos("#") + 1));
		return this->isRequestSuccess = 1;
	}

	if (request.findStrPos("503") != -1) {
		cstrSprintf(&this->request_debug, "503エラーです : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		return this->isRequestSuccess = 0;
	}
	if (request.findStrPos("404") != -1) {
		cstrSprintf(&this->request_debug, "404エラーです : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		return this->isRequestSuccess = 0;
	}
	if (this->waitForHandle) {
		cstrSprintf(&this->request_debug, "接続を中断しました : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		return this->isRequestSuccess = -1;
	}
	if (request.isSame("TIMEOUT")) {
		cstrSprintf(&this->request_debug, "タイムアウト : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		return 0;
	}
	cstrSprintf(&this->request_debug, "その他のエラーです : %d\n", WSAGetLastError());
	ErrorLogAdd(this->request_debug);
	return this->isRequestSuccess = 0;
}

//4bc2b0
void NETWORK::WaitAndInitRanking() {
	GetTimeWrap();
	this->waitForHandle = 1;
	if (this->hHandle != NULL) {
		WaitForSingleObject(this->hHandle, 5000);
	}
	this->waitForHandle = 0;
	this->rankingData.Init();
	return;
}

//4bc2f0
int NETWORK::GetRanking(CSTR hash, char flagInit) {

	CSTR path;
	
	ErrorLogAdd("IRxmlをダウンロードします\n");
	if (hash.length() <= 50) {
		cstrSprintf(&path, "LR2files/Ir/%s.xml", hash);
	}
	else {
		cstrSprintf(&path, "LR2files/Ir/%s.xml", hash.makeCRCstr());
	}

	if (flagInit) this->rankingData.Init();

	cstrSprintf(&this->param, "songmd5=%s&id=%d&lastupdate=%s", hash, this->IR_ID, this->rankingData.lastupdate);
	this->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/getrankingxml.cgi";
	if (HTTPrequest() == 1) {
		ErrorLogAdd("xmlを保存します\n");
		this->httpResult.toFile(path);
		rankingData.ParseXML(path);
		return 1;
	}
	else if (this->rankingData.lastupdate.length() <= 0) {
		rankingData.Init();
		return -1;
	}
	return 1;
}

//4bc4c0
int NETWORK::GetRivalInfo(int rivalID) {
	CSTR pathXML;
	cstrSprintf(&pathXML, "LR2files/Rival/%d.xml", rivalID);
	CSTR pathDB;
	cstrSprintf(&pathDB, "LR2files/Rival/%d.db", rivalID);
	cstrSprintf(&this->param, "id=%d&lastupdate=%d", rivalID, CheckRivaldataNew(rivalID));
	this->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/getplayerxml.cgi";
	if (this->HTTPrequest() == 1) {
		this->httpResult.toFile(pathXML);
		ParseRivalData(rivalID);
		ErrorLogFmtAdd("ライバルデータを更新しました。ID:%06d\n", rivalID);
	}
	else {
		printfDx("ライバルデータのダウンロードに失敗しました。ID:%06d\n", rivalID);
		ErrorLogFmtAdd("ライバルデータのダウンロードに失敗しました。ID:%06d\n", rivalID);
	}
	ScreenFlip();
	ProcessMessage();
	return 1;
}

//4bc600
int OpenWebRanking(CSTR songmd5){
	CSTR url;

	cstrSprintf(&url, "\"http://www.dream-pro.info/~lavalse/LR2IR/search.cgi?mode=ranking&bmsmd5=%s#status&\"", songmd5.body);
	ShellExecuteA(NULL, "open", url, NULL, NULL, 1);
	return 1;
}

//4bc6b0 //
void IRsendScore(NETWORK *ir) {
	int httpResponse;
	CSTR sData;

	ErrorLogAdd("LR2IRにスコアを送信します。\n");
	cstrSprintf(&sData, "%s%s%d%d", ir->IR_passMD5.body, ir->myRanking.songMD5.body,ir->myRanking.exscore, ir->myRanking.clear);
	sData = MD5str(sData);

	cstrSprintf(&ir->param, "songmd5=%s&id=%d&passmd5=%s&title=%s&genre=%s&artist=%s&maxbpm=%d&minbpm=%d&&playlevel=%d&clear=%d&exscore=%d&pg=%d&gr=%d&gd=%d&bd=%d&pr=%d&maxcombo=%d&playcount=%d&clearcount=%d&rate=%d&minbp=%d&totalnotes=%d&opt_history=%d&opt_this=%d&line=%d&judge=%d&inputtype=%d&ghost=%s&rseed=%d&clear_db=%d&clear_ex=%d&clear_sd=%d&scorehash=%s"
		, ir->myRanking.songMD5.body, ir->IR_ID, ir->IR_passMD5.body, UrlEncode(ir->myRanking.title), UrlEncode(ir->myRanking.genre), UrlEncode(ir->myRanking.artist),
		ir->myRanking.maxbpm, ir->myRanking.minbpm, ir->myRanking.playlevel, ir->myRanking.clear, ir->myRanking.exscore, ir->myRanking.pg, ir->myRanking.gr, ir->myRanking.gd, ir->myRanking.bd, ir->myRanking.pr, ir->myRanking.maxcombo,
		ir->myRanking.playcount, ir->myRanking.clearcount, ir->myRanking.rate, ir->myRanking.minbp, ir->myRanking.totalnotes, ir->myRanking.opt_history, ir->myRanking.opt_this, ir->myRanking.line, ir->myRanking.judge,
		ir->myRanking.inputtype, ir->myRanking.ghost.body, ir->myRanking.rseed,	ir->myRanking.clear_db, ir->myRanking.clear_ex, ir->myRanking.clear_sd, UrlEncode(sData));

	ir->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/score.cgi";
	httpResponse = 0; //DEBUG: do not send IR before test is done enough. ir->HTTPrequest();
	if (httpResponse == 1) {
		ir->IRresultMessage = "スコアを送信しました";
		ir->GetRanking(ir->myRanking.songMD5, 1);
	}
	else if (httpResponse == 0) {
		ir->IRresultMessage = "サーバーとの接続に失敗しました";
	}
	ErrorLogAdd("送信終了\n");
	ir->hHandle = NULL;

	return;
}

//4bc970
int NETWORK::GetTargetInfo(int mode, CSTR songmd5, CSTR *oData, CSTR *oName, int *oDigit1, int *oDigit2, int *oDigit3, int *oDigit4, int *oSeed, int *oExscore) {

	CSTR search("top");

	if (mode == 6) {
		search = "top";
	}
	else if (mode == 7) {
		search = "next";
	}
	else if (mode == 8) {
		search = "average";
	}

	cstrSprintf(&param, "songmd5=%s&mode=%s&playerid=%d&targetid=%d", songmd5, search, IR_ID, rankingData.target_ID);
	target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/getghost.cgi";

	if (HTTPrequest() != 1) {
		ErrorLogAdd("ゴーストのダウンロードに失敗しました\n");
		return 0;
	}

	CSVbuf csv;
	SplitCSV(httpResult, &csv, ",");
	if (csv.str[1].length()) {
		if (mode == 8) {
			*oName = "AVERAGE";
			*oExscore = csv.val[1];
			ErrorLogFmtAdd("EXスコアを取得しました:%d\n", csv.val[1]);
		}
		else {
			*oName = csv.str[0];
			*oData = csv.str[3];
			*oDigit1 = GetDigitNum(csv.val[1], 1);
			*oDigit2 = GetDigitNum(csv.val[1], 2);
			*oDigit3 = GetDigitNum(csv.val[1], 3);
			*oDigit4 = GetDigitNum(csv.val[1], 4);
			*oSeed = csv.val[2];
			if (oName->isSame("NOPLAYER")) {
				ErrorLogAdd("ゴーストのダウンロードに失敗しました\n");
				return 0;
			}
			ErrorLogFmtAdd("ゴーストをダウンロードしました\nプレイヤー名%s\n", oName);
		}
		return 1;
	}
	return 0;
}

//4bcc50
NETWORK::NETWORK(){
	InitializeCriticalSection(&criticalSection);
	unused234 = 0;
	waitForHandle = '\0';
	domain = "www.dream-pro.info";
	timeout = 15000;
	IRstatus = 0;
	hHandle = NULL;
	rankUpdateDelayLevel = 0;
	IR_ID = 0;
}

//4bcda0
int NETWORK::WS_clean() {
	EnterCriticalSection(&this->criticalSection);
	LeaveCriticalSection(&this->criticalSection);
	WSACleanup();
	return 1;
}

//4bcdd0
int NETWORK::Login(int isDirectPlay) {

	if (WSAStartup(2, &this->wsa)) {
		this->request_debug = "WinSockの初期化に失敗しました\n";
		this->request_result = "WinSockの初期化に失敗しました。ネットワーク機能は使用できません・\n";
		this->loginResult = -99;
		this->isOnline = 0;
		ErrorLogAdd(this->request_debug);
		WSACleanup();
		return this->loginResult;
	}

	cstrSprintf(&this->param, "passmd5=%s&id=%d&name=%s&version=%d", this->IR_passMD5, this->IR_ID, this->IR_name, 100130); //version 100130
	this->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/login.cgi";
	if (this->HTTPrequest() != 1) {
		this->request_result = "サーバーとの接続に失敗しました。\n";
		this->isOnline = 0;
		this->loginResult = -99;
		return this->loginResult;
	}

	if (this->httpResult.left(3).isSame("NEW")) {
		this->loginResult = 1;
		this->isOnline = 1;
		this->rankingData.myID = atol(this->httpResult.right(this->httpResult.length() - 4));
		cstrSprintf(&this->request_result, "LR2IRに新規登録しました。\nLR2ID:%06d\n\n", this->rankingData.myID);
		this->IR_ID = this->rankingData.myID;
		return this->loginResult;
	}

	if (this->httpResult.left(2).isSame("OK") || this->httpResult.left(2).isSame("B1") || this->httpResult.left(2).isSame("B2") || this->httpResult.left(2).isSame("B3")){
		this->loginResult = 1;
		this->isOnline = 1;
		this->rankingData.myID = atol(this->httpResult.right(this->httpResult.length() - 3));

		if (this->httpResult.left(2).isSame("B1")) {
			cstrSprintf(&this->request_result, "LR2IRにログインしました。\nLR2ID:%06d\nアクセス集中のため、ランキング自動更新の待ち時間は20秒となります。\n\n", this->rankingData.myID);
			this->rankUpdateDelayLevel = 1;
			this->waitTime = 20000;
		}
		else if (this->httpResult.left(2).isSame("B2")) {
			cstrSprintf(&this->request_result, "LR2IRにログインしました。\nLR2ID:%06d\nアクセス集中のため、ランキング自動更新の待ち時間は30秒となります。\n\n", this->rankingData.myID);
			this->rankUpdateDelayLevel = 2;
			this->waitTime = 30000;
		}
		else if (this->httpResult.left(2).isSame("B3")) {
			cstrSprintf(&this->request_result, "LR2IRにログインしました。\nLR2ID:%06d\nアクセス集中のため、ランキング自動更新は行われません。\n\n", this->rankingData.myID);
			this->rankUpdateDelayLevel = 3;
			this->waitTime = -1;
		}
		else {
			cstrSprintf(&this->request_result, "LR2IRにログインしました。\nLR2ID:%06d\n\n", this->rankingData.myID);
		}

		this->IR_ID = this->rankingData.myID;

		if (isDirectPlay == 0) {
			CSVbuf csv;
			SplitCSV(this->httpResult, &csv, ",");
			for (int i = 0; i < 20; i++) this->rivals[i] = 0;
			this->rivalcount = 0;

			if (this->getrival == 1) {
				for(int cur= 0 ; cur < 20; cur++) {
					if (GetMouseInput()) {
						printfDx("ライバルデータの取得をスキップしました。\n");
						break;
					}
					if (cur == 20 || csv.val[2 + cur] < 1) break;
					if (cur == 0) printfDx("ライバルデータの取得は左クリック押しっぱなしでスキップできます。\n");

					ErrorLogFmtAdd("ライバル登録されています:%d\n", this->rivals[cur]); //TOFIX: 
					this->rivals[cur] = csv.val[2+cur];
					this->rivalcount++;
					this->GetRivalInfo(this->rivals[cur]);
					if (this->getrival != 1) break;
				}
			}
		}
		return this->loginResult;
	}

	else if (this->httpResult.left(4).isSame("MAIL")) {
		this->request_result = "無効なメールアドレスです。\n";
		this->isOnline = 0;
		return this->loginResult = -2;
	}
	else if (this->httpResult.left(2).isSame("DB")) {
		this->request_result = "データベースに接続できません。\n";
		this->isOnline = 0;
		return this->loginResult = -99;
	}
	else if (this->httpResult.left(7).isSame("VERSION")) {
		this->request_result = "最新版に更新して下さい。\n";
		this->isOnline = 0;
		return this->loginResult = -99;
	}
	else if (this->httpResult.left(3).isSame("BAN")) {
		this->request_result = "あなたのアカウントは凍結されました。\n";
		this->isOnline = 0;
		return this->loginResult = -99;
	}
	else if (this->httpResult.left(5).isSame("SORRY")) {
		this->request_result = "現在サーバーメンテナンス中です。\n";
		this->isOnline = 0;
		return this->loginResult = -99;
	}
	else if (this->httpResult.left(3).isSame("END")) {
		this->request_result = "LR2IRは終了しました。\n";
		this->isOnline = 0;
		return this->loginResult = -99;
	}
	cstrSprintf(&this->request_result, "パスワードが違うか、その他のエラーです。\nLR2ID:%d\nLR2IDがいつの間にか変わってログイン出来ない！という方は#lunaticraveまでどうぞ。個別に対応します。", this->IR_ID);
	this->isOnline = 0;
	return this->loginResult = -3;
}

//4bd640
int NETWORK::MakeIRsendScoreThread() {
	GetTimeWrap();
	this->waitForHandle = true;
	if (this->hHandle) {
		WaitForSingleObject(this->hHandle, 5000);
	}
	this->waitForHandle = false;
	this->rankingData.Init();
	this->IRstatus = 1;
	this->hHandle = (HANDLE)_beginthread((void(*)(void*))IRsendScore, 0, this);
	return 0;
}

////LR2input.cpp

////Recording
//4bf4f0
RECORDING::RECORDING() {
	memset(this, 0, sizeof(RECORDING));
}

//4bf510
bool RECORDING::RefreshCurFrame() {
	this->curFrame = this->writeSamplePos;
	return true;
}

//4bf520
int RECORDING::GetCurTime() {
	return this->curFrame * 1000 / this->framerate;
}

//4bf540
int RECORDING::CpyScreenToAVI() {
	this->srcHDC = GetDC(this->hwnd);
	BitBlt(this->dstHDC, 0, 0, this->bmiHeader.biWidth, this->bmiHeader.biHeight, this->srcHDC, 0, 0, SRCCOPY);
	ReleaseDC(this->hwnd, this->srcHDC);
	return AVIStreamWrite(this->pAVIstream, this->writeSamplePos++, 1, this->buf, this->bmiHeader.biSizeImage, AVIIF_KEYFRAME, 0, 0); //watch out that ++
}

//4bf5b0
bool RECORDING::Release() {
	
	if (this->compvars.hic) {
		ICCompressorFree(&this->compvars);
		this->compvars.hic = NULL;
	}

	if (this->pAVIstream) {
		AVIStreamRelease(this->pAVIstream);
		this->pAVIstream = NULL;
	}

	if (this->pAVIFILE) {
		AVIFileRelease(this->pAVIFILE);
		this->pAVIFILE = NULL;
	}
	return true;
}

//4bf610
int REC_CpyAVIStreamToFile(PAVIFILE pfile, PAVISTREAM pavi, int unused) {

	unsigned int lpos;
	AVISTREAMINFOA si;
	PAVISTREAM pNewavi;
	long sz;
	long samples;
	LPVOID lpFormat;
	int totalLength;

	lpos = AVIStreamStart(pavi);
	totalLength = lpos + AVIStreamLength(pavi) - 1;

	if (AVIStreamInfoA(pavi, &si, sizeof(AVISTREAMINFOA)) != 0) return 0;
	if (AVIFileCreateStreamA(pfile, &pNewavi, &si) != 0) return 0;
	if (AVIStreamReadFormat(pavi, lpos, NULL, &sz) != 0) {
		AVIStreamRelease(pNewavi);
		return 0;
	}

	lpFormat = malloc(sz);
	if (lpFormat == NULL) {
		AVIStreamRelease(pNewavi);
		return 0;
	}

	if (AVIStreamReadFormat(pavi, lpos, lpFormat, &sz) == 0 && AVIStreamSetFormat(pNewavi, lpos, lpFormat, sz) == 0) {
		for (; lpos <= totalLength; lpos += samples) {
			if (!(AVIStreamRead(pavi, lpos, -1, NULL, 0, &sz, &samples) == 0 && sz > 0 && samples > 0)) break;
			if (AVIStreamRead(pavi, lpos, 4096, NULL, 0, &sz, &samples) != 0) break;
			lpFormat = realloc(lpFormat, sz);
			if (lpFormat == NULL) break;
			if (AVIStreamRead(pavi, lpos, samples, lpFormat, sz, &sz, &samples) != 0) break;
			if (AVIStreamWrite(pNewavi, lpos, samples, lpFormat, sz, 0, NULL, NULL) != 0) break;
		}
	}

	AVIStreamRelease(pNewavi);
	free(lpFormat);
	return 0;
}

//4bf7f0
int CreateStream(CSTR filename, int framerate, COMPVARS *compvars, BITMAPINFOHEADER* lpbmi, PAVIFILE* pAVIFILE, PAVISTREAM* pAVIstream) {

	IAVIFile* pFile;
	AVISTREAMINFOA si;
	PAVISTREAM pAviCompressed;
	PAVISTREAM pavi;
	AVICOMPRESSOPTIONS options;

	if (AVIFileOpenA(&pFile, filename, OF_CREATE | OF_WRITE, 0) != 0) {
		MessageBoxA(NULL, "ファイルの作成またはオープンに失敗しました。", NULL, MB_ICONEXCLAMATION);
		return 0;
	}
	memset(&si, 0, sizeof(AVISTREAMINFOA));
	si.fccHandler = compvars->fccHandler;
	si.dwRate = framerate;
	si.fccType = 0x73646976; //'vids'
	si.dwScale = 1;
	SetRect(&si.rcFrame, 0, 0, lpbmi->biWidth, lpbmi->biHeight);

	if (AVIFileCreateStreamA(pFile, &pavi, &si) != 0) {
		MessageBoxA(NULL, "ストリームの作成に失敗しました。", NULL, MB_ICONEXCLAMATION);
		AVIFileRelease(pFile);
		return 0;
	}

	options.dwKeyFrameEvery = compvars->lKey;
	options.fccHandler = compvars->fccHandler;
	options.dwQuality = compvars->lQ;
	options.dwBytesPerSecond = compvars->lDataRate;
	options.fccType = 0x73646976; //'vids'
	options.dwFlags = AVICOMPRESSF_KEYFRAMES | AVICOMPRESSF_DATARATE;
	options.lpFormat = NULL;
	options.cbFormat = 0;
	options.lpParms = NULL;
	options.cbParms = 0;
	options.dwInterleaveEvery = 0;

	if (AVIMakeCompressedStream(&pAviCompressed, pavi, &options, NULL) != 0) {
		AVIStreamRelease(pavi);
		AVIFileRelease(pFile);
		return 0;
	}

	AVIStreamSetFormat(pAviCompressed, 0, lpbmi, sizeof(BITMAPINFOHEADER));
	*pAVIFILE = pFile;
	*pAVIstream = pAviCompressed;
	AVIStreamRelease(pavi);
	return 1;
}

//4bfa10
bool RECORDING::PrepareAVIRecord(double framerate, int bit, CSTR filename, uint frameLen, HWND hwnd) {
	
	tagRECT cRect;

	this->framerate = framerate;
	this->bitdepth = bit;
	this->filename = filename;
	this->framelen = frameLen;
	this->writeSamplePos = 0;
	this->curFrame = 0;

	remove(filename);
	CSTR dat("LR2files\\Config\\compvars.dat");

	if (dat.canOpenFile()) {
		FILE* pFile = fopen(dat, "rb");
		fread(&this->compvars, sizeof(COMPVARS), 1, pFile);
		fclose(pFile);
	}
	else {
		SetMouseDispFlag(1);
		ProcessMessage();
		memset(&this->compvars, 0, sizeof(COMPVARS));
		this->compvars.cbSize = 64;
		if (ICCompressorChoose(NULL,0,NULL,NULL,&this->compvars,NULL) == 0) return false;
		FILE *pFile = fopen("LR2files\\Config\\compvars.dat", "wb");
		fwrite(&this->compvars, sizeof(COMPVARS), 1, pFile);
		fclose(pFile);
		MessageBoxA(NULL, "圧縮設定が保存されました。\n設定の変更はJUKEBOXタブ→詳細設定→動画圧縮設定で行えます。", "報告", 0);
		SetMouseDispFlag(0);
	}

	if (hwnd == NULL) hwnd = GetDesktopWindow();
	this->hwnd = hwnd;
	GetClientRect(this->hwnd, &cRect);
	this->bmiHeader.biWidth = 0;
	this->bmiHeader.biHeight = 0;
	this->bmiHeader.biPlanes = 0;
	this->bmiHeader.biBitCount = 0;
	this->bmiHeader.biCompression = 0;
	this->bmiHeader.biSizeImage = 0;
	this->bmiHeader.biXPelsPerMeter = 0;
	this->bmiHeader.biYPelsPerMeter = 0;
	this->bmiHeader.biClrUsed = 0;
	this->bmiHeader.biClrImportant = 0;
	
	this->bmiHeader.biWidth = cRect.right;
	this->bmiHeader.biPlanes = 1;
	this->bmiHeader.biBitCount = 0x18;
	this->bmiHeader.biSizeImage = (cRect.right + 1) * 3 * cRect.bottom * 4;
	this->bmiHeader.biHeight = cRect.bottom;
	this->bmiHeader.biSize = 40;
	this->bmiHeader.biCompression = 0;
	CreateStream(filename, framerate, &this->compvars, &this->bmiHeader, &this->pAVIFILE, &this->pAVIstream);
	this->dstHDC = CreateCompatibleDC(NULL);
	this->hBIT = CreateDIBSection(NULL, (BITMAPINFO*)&this->bmiHeader, 0, &this->buf, NULL, 0); //it needs bitmapInfo(44bytes), not bitmapInfoHeader(40bytes)
	this->hGDI = SelectObject(this->dstHDC, this->hBIT);
	return 1;
}

//4bfcb0
int RECORDING::InsertAudioToMovie(CSTR pathAudio, bool deleteFlag) {

	CSTR path(pathAudio);
	bool isMp3 = path.lower().right(3).isSame("mp3");

	if (isMp3) {
		path = path.left(path.length() - 3);
		path.add("wav");
		Mp3toWavP(pathAudio, path);
		pathAudio = path;
	}

	if (this->compvars.hic) {
		ICCompressorFree(&this->compvars);
		this->compvars.hic = NULL;
	}
	if (this->pAVIstream) {
		AVIStreamRelease(this->pAVIstream);
		this->pAVIstream = NULL;
	}

	if (AVIStreamOpenFromFileA(&this->pAVIstream, pathAudio, 0x73647561, 0, OF_SHARE_DENY_NONE, NULL) != 0) { //'auds'
		MessageBoxA(NULL, "オーディオファイルの読み込みに失敗しました", "エラー", 0);
	}
	else {
		if (REC_CpyAVIStreamToFile(this->pAVIFILE, this->pAVIstream, this->curFrame) == 0) {
			MessageBoxA(NULL, "ストリーム追加に失敗しました", "エラー", 0);
		}
		AVIStreamRelease(this->pAVIstream);
		this->pAVIstream = NULL;
	}

	if (isMp3) remove(path);
	if (deleteFlag) remove(pathAudio);

	return 1;
}

//4bfec0
int REC_COPYFILE(FILE *oFile, FILE *iFile, uint size){
	void *buf;
	uint bufSize;
	uint _Count;

	bufSize = size;
	if (size > 0x100000) {
		do {
			bufSize >>= 1;
		} while (bufSize > 0x100000);
	}

	buf = malloc(bufSize);
	while (buf == NULL) {
		bufSize >>= 1;
		if (bufSize < 0x400) {
			ErrorLogAdd("コピー用バッファが確保できません。\n");
			return 0;
		}
		buf = malloc(bufSize);
	}

	while (size != 0) {
		_Count = bufSize;
		if (size <= bufSize) {
			_Count = size;
		}
		if (fread(buf, 1, _Count, iFile) != _Count) {
			ErrorLogAdd("ファイルが読めません。\n");
			return 0;
		}
		if (fwrite(buf, 1, _Count, oFile) != _Count) {
			ErrorLogAdd("ファイルに書けません。\n");
			return 0;
		}
		size -= _Count;
	}

	if (fflush(oFile) == 0) {
		free(buf);
		return 1;
	}

	ErrorLogAdd("ファイルに書けません。\n");
	return 0;
}

//4bffb0
char WAVhead[70] = {0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20, 0x1e, 0x00, 0x00, 0x00, 0x55, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x71, 0x05, 0x66, 0x61, 0x63, 0x74, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00 };
int WAVstereo[4] = { 2,2,2,1 }; 
char MP3VersionString[2][8] = { "mpeg1","mpeg2" }; //TODO : need different declare for exact same with original binary
char MP3StereoString[4][12] = {"stereo","j-stereo","dual","mono" };
int MP3BitArray[2][4] = { {22050, 24000, 16000, 99999}, {44100, 48000, 32000, 99999} };
int MP3FrequencyArray[2][16] = { {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 99999}, {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 99999} };
int Mp3toWavF(FILE *iFile, FILE *oFile) { //TODO : need test

	byte buf[1024];
	
	fseek(iFile, 0, 2);
	int iSize = ftell(iFile);
	bool odd = (iSize % 2 != 0);
	if (odd) iSize++;

	rewind(iFile);
	int read = fread(buf, 1, 0x400, iFile);
	if (read != 0x400) {
		ErrorLogAdd("入力ファイルが読めません。\n");
		return 0;
	}

	if( !(buf[0] == 0xff && (buf[1] & 0xf0) == 0xf0) ){ //file signature check
		ErrorLogAdd("入力ファイルはMP3データではないようです。\n");
		return 0;
	}

	byte MP3Version = (byte)buf[1] >> 3 & 1;
	int MP3Frequency = MP3FrequencyArray[MP3Version][(byte)buf[2] >> 4];
	int MP3Bitrate = MP3BitArray[MP3Version][(byte)buf[2] >> 2 & 3];
	byte MP3Stereo = (byte)buf[3] >> 6;
	ErrorLogFmtAdd("%s, %ldkbit/s, %ldHz, %s", MP3VersionString[MP3Version], MP3Frequency, MP3Bitrate, MP3StereoString[MP3Stereo]);

	int i;
	for (i = 2; i < 0x400; i++) {
		byte MP3VersionNew = (byte)buf[i + 1] >> 3 & 1;
		if ((buf[i] == (byte)0xff && (buf[i + 1] & 0xf0) == 0xf0) && MP3VersionNew == MP3Version && MP3FrequencyArray[MP3VersionNew][(byte)buf[i+2] >> 4] == MP3Frequency && MP3BitArray[MP3VersionNew][(byte)buf[i + 2] >> 2 & 3] == MP3Bitrate && (byte)buf[i+3] >> 6 == MP3Stereo) 
			break;
	}
	int AAUpos = i;
	ErrorLogFmtAdd("$%08lX\n", i & 0xffff);

	*(DWORD*)&WAVhead[4] = iSize + 0x3e; //62
	*(WORD*)&WAVhead[22] = WAVstereo[MP3Stereo];
	*(DWORD*)&WAVhead[24] = MP3Bitrate;
	*(DWORD*)&WAVhead[28] = MP3Frequency * 125;
	*(WORD*)&WAVhead[44] = AAUpos;
	*(DWORD*)&WAVhead[58] = (((MP3Version!=0) + 1) * 0x240) * (iSize / (ulonglong)(AAUpos & 0xffff));
	*(DWORD*)&WAVhead[66] = iSize - odd;

	if (fwrite(WAVhead,1,70,oFile) != 70) {
		ErrorLogAdd("ファイルに書けません。\n");
		return 0;
	}
	rewind(iFile);
	if (REC_COPYFILE(oFile, iFile, iSize - odd) == 0)
		return 0; 

	if (fwrite(WAVhead, 1, odd, oFile) != odd) {
		ErrorLogAdd("ファイルに書けません。\n");
		return 0;
	}
	
	if (fflush(oFile) != 0) {
		ErrorLogAdd("ファイルに書けません。\n");
		return 0; 
	}
	
	return 1;
}

//4c0350
bool Mp3toWavP(char *iPath, char *oPath) {
	FILE *iFile, *oFile;

	iFile = fopen(iPath, "rb");
	if (iFile == NULL) {
		ErrorLogFmtAdd("入力ファイルが開けません(%s)。\n", iPath);
		_exit(1);
	}

	oFile = fopen(oPath, "wb");
	if (oFile == NULL) {
		ErrorLogFmtAdd("出力ファイルが開けません(%s)。\n", oPath);
		_exit(1);
	}

	return Mp3toWavF(iFile, oFile) == 0;
}


////REPLAY
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
	int iVar1;
	ReplayData *pRVar2;

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
	AddReplayData(rp, 0, 0x6a, *(short *)&cfg->assist[0]);
	AddReplayData(rp, 0, 0x9c, *(short *)&cfg->assist[1]);
	AddReplayData(rp, 0, 0x69, *(short *)&cfg->rand[0]);
	AddReplayData(rp, 0, 0x9b, *(short *)&cfg->rand[1]);
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
int REPLAY_ApplyJudgeToScore(gameplay *gp, Timer *T, game *g, uint judge, int player, int dp) {

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
	if (hp <= 30 && (!(g->config.play.gaugeOption[player] == 0 || g->config.play.gaugeOption[player] == 3) || gp->is2Pplay != 0) && judge <= 2) {
		damage = gp->player[player].judge_damage[judge] * 0.6;
	}
	else {
		damage = gp->player[player].judge_damage[judge];
	}
	gp->player[player].HP += damage;

	if (gp->is2Pplay == 0 && gp->player[player].HP <= 2.0 && (g->config.play.gaugeOption[player] == 0 || g->config.play.gaugeOption[player] == 3)) {
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

	if (gp->player[player].note_current > 0) gp->player[player].rate = gp->player[player].exscore * 100 / (gp->player[player].note_current * 2);

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
int REPLAY_ApplyJudge(gameplay *gp, Timer *T, game *g, int dmg, int player, int dp) {

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
			g->config.play.rand[0] = data->value;
			break;
		case 0x6a:
			g->config.play.assist[0] = data->value;
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
			g->config.play.rand[1] = data->value;
			break;
		case 0x9c:
			g->config.play.assist[1] = data->value;
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
			REPLAY_ApplyJudgeToScore(gp, T, g, data->value, 0, 0);
			break;
		case 0xd3:
			REPLAY_ApplyJudgeToScore(gp, T, g, data->value, 0, 1);
			break;
		case 0xd4:
			REPLAY_ApplyJudgeToScore(gp, T, g, data->value, 1, 0);
			break;
		case 0xd5:
			REPLAY_ApplyJudgeToScore(gp, T, g, data->value, 1, 1);
			break;
		case 0xd6:
			REPLAY_ApplyJudge(gp, T, g, data->value, 0, 0);
			break;
		case 0xd7:
			REPLAY_ApplyJudge(gp, T, g, data->value, 0, 1);
			break;
		case 0xd8:
			REPLAY_ApplyJudge(gp, T, g, data->value, 1, 0);
			break;
		case 0xd9:
			REPLAY_ApplyJudge(gp, T, g, data->value, 1, 1);
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

	while (re->data[re->count].timing == 0 && re->count < re->max) {
		ReplayDataToInput(&re->data[re->count], g, aud, gp, in, T);
		re->count++;
	}

	return 1;
}

//4c2310
int ReplayToInput(REPLAY *rp, game *g, AUDIO *aud, gameplay *gp, inputStructure *is, Timer *T){
	if (rp->count < rp->max) {
		while (rp->data[rp->count].timing < GetTimeLapse(41, T) && rp->count < rp->max) {
			ReplayDataToInput(rp->data + rp->count, g, aud, gp, is, T);
			rp->count++;
		}
		return 1;
	}
	return 0;
}

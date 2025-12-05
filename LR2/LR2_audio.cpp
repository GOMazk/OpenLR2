#include "LR2_audio.h"
#include "Engine.h"
#include "LR2_replay.h"
#include "LR2_skinobject.h"
#include "LR2_configsave.h"
#include "filesystem.h"
#include "filesystem.h"

int FxByMIDI(game *g) {
	bool change = false;
	
	if (g->KeyInput.is_drag_now >= 0 && g->KeyInput.is_drag_now < g->skstruct.otherObject[2].srcSize) {
		if (g->KeyInput.midi_n <= 0) {
			if (g->KeyInput.inputID[KEY_INPUT_DELETE]) g->config.input.midi_control[g->KeyInput.is_drag_now] = 0;
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
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), 63, (short)g->audio.param.fxParam[0][1]);
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

static void adjust_input_filepath(CSTR& path)
{
#ifndef _WIN32
	path.replace("\\", "/");
#endif // _WIN32
}

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
	cstrSprintf(&path, fs::make_preferred("LR2files/SkinCustomize/%s.xml").data(), MD5str(filepath));
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
	hFile = fopen(filepath.body, "r");
	if (!hFile) return 0;
	CSTR fBuf(260);
	char* pFbuf;
	pFbuf = fBuf.outstr();
	for (pFbuf = fgets(pFbuf, 256, hFile); pFbuf; pFbuf = fgets(pFbuf, 256, hFile)) {
		fBuf = ansi2utf(pFbuf, 932).c_str();
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
				adjust_input_filepath(csv.str[2]);
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
				adjust_input_filepath(csv.str[2]);
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

			adjust_input_filepath(csv.str[1]); // TODO: move up?
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

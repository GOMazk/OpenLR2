#include "LR2_makemp3.h"
#include "Engine.h"
#include "LR2_bmsload.h"

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
	CreateProcessA(NULL, cmd, 0, 0, 0, 0, 0, 0, (LPSTARTUPINFOA)&sinfo, &pinfo);
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


	for (int i = 0; i < g->gameplay.bmsobj.count; i++) {
		double len = 0;
		g->gameplay.bmsobj.notes[i].op;
		if (g->gameplay.bmsobj.notes[i].op == 1 || (10 <= g->gameplay.bmsobj.notes[i].op  && g->gameplay.bmsobj.notes[i].op < 30)) {
			if (i + 1 < g->gameplay.bmsobj.count) {
				double endtime = g->gameplay.keysound[(int)g->gameplay.bmsobj.notes[i].val].length;
				if ((int)endtime < 0) {
					endtime += 4294967296.0;
				}
				endtime += g->gameplay.bmsobj.notes[i].realTiming;

				for (int j = i + 1; j < g->gameplay.bmsobj.count; j++) {
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

		for (int i = 0; i < g->gameplay.bmsobj.count; i++) {
			double len = 0;
			if (g->gameplay.bmsobj.notes[i].op == 1 || (10 <= g->gameplay.bmsobj.notes[i].op && g->gameplay.bmsobj.notes[i].op < 30)) {
				if (i + 1 < g->gameplay.bmsobj.count) {
					double endtime = g->gameplay.keysound[(int)g->gameplay.bmsobj.notes[i].val].length;
					if ((int)endtime < 0) {
						endtime += 4294967296.0;
					}
					endtime += g->gameplay.bmsobj.notes[i].realTiming;

					for (int j = i + 1; j < g->gameplay.bmsobj.count; j++) {
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

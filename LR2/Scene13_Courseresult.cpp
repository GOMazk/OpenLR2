#include "Scene13_Courseresult.h"
#include "LR2.h"

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
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->gameplay.player[0].gaugeType + g->config.play.random[0] * 10;
		}
		else {
			g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->gameplay.player[0].gaugeType + g->config.play.random[0] * 10 + g->config.play.random[1] * 100 + g->config.play.dpflip * 1000;
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
	if (g->sSelect.bmsList[g->sSelect.cur_song].courseIR == 0 && g->net.isOnline) {
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = 0;
		g->sSelect.oldIRrank = 0;

		g->net.IRresultMessage = "このコースはIRに対応していません。";
		SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = 0;
		g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = 0;
		g->net.rankingData.Init();
	}

	if (g->net.isOnline && g->is_starter == 0 && g->sSelect.bmsList[g->sSelect.cur_song].courseIR == 1) {
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
			std::string ghostString = "Z";
			g->net.MakeIRsendScoreThread(ghostString);

			if (!g->config.network.displayIr.length()) {
				g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = g->net.rankingData.myRanking;
				g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = g->net.rankingData.rankingCount;
				if (g->net.rankingData.rankingCount > 0) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate = (g->net.rankingData.rankingCount + g->net.rankingData.clearPlayers[1] - g->net.rankingData.clearPlayers[0]) / g->net.rankingData.rankingCount;
				}
			}
		}
		SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
		return 1;
	}

	ErrorLogAdd("IR機能は利用しません\n");
	return 1;
}

int ProcS_CourseResult(game *g, sqlite3 *sql) {
	PlayerCheckAndSwap(&g->gameplay);
	CheckCourseClear(g);
	ProcS_subCourseResult(g, sql);
	g->net.customIR.BeginResultIr(*g, sql, 0);
	LoadSceneG(g, &g->skstruct, SKINTYPE_COURSERESULT);
	
	if (g->skstruct.flag_flip) {
		FlipScore(g);
	}

	if ((g->gameplay).player[0].clearType > 1) 
		PlaySound(&g->audio, &g->audio.sysSound.courseclear, g->audio.chnBgm, -1); 
	else 
		PlaySound(&g->audio, &g->audio.sysSound.coursefail, g->audio.chnBgm, -1);
	
	return 1;
}

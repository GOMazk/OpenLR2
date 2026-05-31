#include "LR2_statplay.h"
#include "Engine.h"
#include "LR2_statlong.h"
#include "LR2.h"
#include "Scene02_Songselect.h" //objstr in CheckMission()

bool CheckScoreSaveConditon(game *g){ //TOFIX : p2_assist == 1 but no battle, doesn't match with actual condition
	if ( (g->config.play.battle == 0 || g->config.play.battle == 4) && g->config.play.is_extra != 1
		&& g->config.play.m_addlong == 0 && g->config.play.m_loudness <= 0
		&& g->config.play.m_lunaris == 0 && g->config.play.hsfix != 4 && g->config.play.m_addmine == 0 
		&& (g->config.play.m_addnote == 0 && g->config.play.is_extra == 0) && g->config.play.autokey == 0
		&& (1 || g->sSelect.metaSelected.keymode < 10)
		&& g->config.play.p1_assist == 0 && (g->config.play.p2_assist == 0 || g->sSelect.metaSelected.keymode < 10) 
		&& g->config.play.random[0] < 4 && g->config.play.random[1] < 4) {
		return true;
	}
	return false;
}

int CheckClearLampChallenge(game *g){ //TOFIX : p2_assist == 1 but no battle, doesn't match with actual condition
	int gauge;

	if (g->config.play.m_addlong == 1 || g->config.play.m_loudness > 0
		|| g->config.play.m_lunaris || g->config.play.m_addlong > 0
		|| g->config.play.m_addmine || g->config.play.m_addnote || g->config.play.battle == 1) {
		return 0;
	}

	if (g->config.play.random[0] < 4 && g->config.play.random[1] < 4 && g->config.play.hsfix != 4
		&& g->config.play.autokey == 0 && (1 || g->sSelect.metaSelected.keymode < 10)
		&& g->config.play.p1_assist == 0 && (g->config.play.p2_assist == 0 || g->sSelect.metaSelected.keymode < 10)) {

		gauge = g->gameplay.player[0].gaugeType;
		if (gauge == 1) return 3;
		else if (gauge == 2) return 4;
		else if (gauge == 3) return 1;
		else if (gauge == 4) return 4;
		else if (gauge == 5) return 3;
		else return 2;
	}
	return 1;
}

uint ConvertOptionHistory(game *g){

	int clear;
	int gauge;
	uint ret;

	ret = 0;
	if ( g->config.play.p1_assist == 1 || g->config.play.p2_assist == 1 ) {
		if (g->gameplay.player[0].clearType > 1) {
			ret = 0x1000000;
		}
	}
	else {
		clear = g->gameplay.player[0].clearType;
		gauge = g->gameplay.player[0].gaugeType;
		if (clear == 2) {
			if (gauge == 3) {
				ret = 8;
			}
			return ret;
		}
		if (clear > 2) {
			ret = 0;
			switch (gauge) {
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

int LogGraphPlayData(GRAPHDATA *grp, PLAYERSTATUS *pstat, int time, int endtime){
	if (grp->cursor < 1000 && time <= endtime && 0 < endtime) {
		do {
			if ((time * 1000) / endtime < grp->cursor) {
				return 1;
			}
			for (int i = 0; i < 6; i++) {
				grp->hp[i][grp->cursor] = pstat->HP[i];
			}
			grp->combo[grp->cursor] = pstat->now_combo;
			grp->exscore[grp->cursor] = pstat->exscore;
			grp->cursor++;
		} while (grp->cursor < 1000);
		return 1;
	}
	return 0;
}

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

int LogGraphPlayerDataToEnd(GRAPHDATA *grp, PLAYERSTATUS *pstat){

	if (grp->cursor >= 1) {
		for (int i = grp->cursor; i < 1000; i++) {
			for (int gauge = 0; gauge < 6; gauge++) {
				grp->hp[gauge][i] = pstat->HP[gauge];
			}
			grp->combo[i] = pstat->now_combo;
			grp->exscore[i] = pstat->exscore;
		}
		return 1; 	
	}
	else {
		for (int i = grp->cursor; i < 1000; i++) {
			for (int gauge = 0; gauge < 6; gauge++) {
				grp->hp[gauge][i] = grp->hp[gauge][0];
			}
			grp->combo[i] = grp->combo[0];
			grp->exscore[i] = grp->exscore[0];
		}
		return 1;
	}
}


int CheckClear(PLAYERSTATUS *pstat, int gaugeType, char isCourse){

	pstat->clearType = 1;
	if (pstat->totalnotes == pstat->max_combo) {
		pstat->clearType = 5;
		return pstat->clearType;
	}
	if (gaugeType == 1 || gaugeType == 5 || gaugeType == 4) {
		if ( pstat->note_current == pstat->totalnotes && pstat->HP[gaugeType] >= 2.0) {
			pstat->clearType = 4;
		}
	}
	else {
		if (isCourse) {
			if (pstat->note_current != pstat->totalnotes)
				return pstat->clearType;
			if (pstat->HP[gaugeType] >= 2.0) {
				pstat->clearType = (gaugeType != 3) + 2;
				return pstat->clearType;
			}
		}
		if (pstat->note_current == pstat->totalnotes && pstat->HP[gaugeType] >= 80.0) {
			pstat->clearType = (gaugeType != 3) + 2;
			return pstat->clearType;
		}
	}
	return pstat->clearType;
}


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

static int GetBestClearedGauge(gameplay& gameplay, int playerIdx, CONFIG_PLAY& cfg, bool limitToCourse) {
	PLAYERSTATUS& player = gameplay.player[playerIdx];
	if (playerIdx == 1 && gameplay.ghostBattle) return player.gaugeType;
	if (cfg.gaugeOption[playerIdx] == 5) return 5;
	constexpr std::array<int, 5> gaugeArr({ 4, 2, 1, 0, 3 });
	unsigned int i = 0;
	if (limitToCourse)
		for (; i < gaugeArr.size(); i++)
			if (gaugeArr[i] == gameplay.player[playerIdx].clearGaugeTypeCourse)
				break;
	auto is_gauge_alive = [](int gaugeIdx, double hp) {
		switch (gaugeIdx) {
		case 0:
		case 3:
			return hp >= 80.;
		default: return hp >= 2.;
		}
		};
	for (; i < gaugeArr.size(); i++)
		if (is_gauge_alive(gaugeArr[i], player.HP[gaugeArr[i]]))
			return gaugeArr[i];
	if (gameplay.courseType == 2) return 0;
	return 3;
}

int CheckCourseClear(game* g) {
	if (g->gameplay.courseStageNow < g->gameplay.courseStageCount - 1) {
		for (int i = g->gameplay.courseStageNow - 1; i < g->gameplay.courseStageCount; i++) {
			g->gameplay.player[0].total_note += g->sSelect.bmsList[g->sSelect.cur_song].courseTotalnote[i];
			g->gameplay.player[1].total_note += g->sSelect.bmsList[g->sSelect.cur_song].courseTotalnote[i];
		}
	}

	std::array<int, 2> gauge = { g->gameplay.player[0].clearGaugeTypeCourse, g->gameplay.player[1].clearGaugeTypeCourse };
	for (int p = 0; p < 2; p++) {
		memcpy(g->gameplay.player[p].judgecount, g->gameplay.player[p].judgecount2, sizeof(int) * 6);
		g->gameplay.player[p].exscore = g->gameplay.player[p].judgecount[4] + g->gameplay.player[p].judgecount[5] * 2;
		g->gameplay.player[p].note_current = g->gameplay.player[p].note_current2;
		g->gameplay.player[p].totalnotes = g->gameplay.player[p].total_note;
		g->gameplay.player[p].max_combo = g->gameplay.player[p].max_combo_course;
		
		g->gameplay.player[p].clearType = 1;

		if (g->gameplay.player[p].HP[gauge[p]] < 2.0 || g->gameplay.courseStageNow < g->gameplay.courseStageCount - 1) {
			g->gameplay.player[p].clearType = 1;
		}
		else if (g->gameplay.player[p].total_note == g->gameplay.player[p].max_combo_course) {
			g->gameplay.player[p].clearType = 5;
		}
		else if (gauge[p] == 1 || gauge[p] == 5 || gauge[p] == 4) {
			if (g->gameplay.player[p].note_current2 == g->gameplay.player[p].total_note && g->gameplay.player[p].HP[gauge[p]] > 2.0) {
				g->gameplay.player[p].clearType = 4;
			}
		}
		else if (g->gameplay.player[p].note_current2 == g->gameplay.player[p].total_note && g->gameplay.player[p].HP[gauge[p]] > 2.0) {
			g->gameplay.player[p].clearType = (gauge[p] != 3) + 2;
		}
	}

	return 1;
}

int CheckMission(game *g){
	int level;
	int gauge;

	if (g->config.play.battle) 
		return 0;

	if (g->config.play.p1_assist == 1) 
		return 0;
	if (g->config.play.p1_assist == 2) 
		return 0;

	gauge = g->gameplay.player[0].gaugeType;

	if (g->gameplay.player[0].gaugeType == 3)
		return 0;
	if (g->gameplay.player[1].gaugeType == 3)
		return 0;

	//converge 7 14 25 35 40
	//constant 15 23 24 34 39
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
			if (gauge == 0 && 80.0 <= g->gameplay.player[0].HP[gauge] && g->gameplay.player[0].HP[gauge] < 86.0) {
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
		case 6:
			if (g->gameplay.player[0].totalnotes >= 100 && g->audio.param.pitch_on == 1 && (g->audio.param.pitch_type == 0 || g->audio.param.pitch_type == 2)) {
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
			if (g->gameplay.player[0].exscore >= (g->gameplay.player[0].totalnotes * 2)* 8/9) {
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
			if (g->gameplay.player[0].totalnotes >= 100) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 20:
			if (gauge == 0 && g->gameplay.player[0].HP[gauge] >= 80.0 && g->gameplay.player[0].HP[gauge] < 82.0) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 21:
			if (gauge == 1 && g->gameplay.player[0].HP[gauge] < 4.0) {
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
			if (gauge == 2 && 998 <= g->gameplay.player[0].exscore && g->gameplay.player[0].exscore <= 1002) {
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
			if (g->gameplay.player[0].totalnotes >= 1500 && g->gameplay.song_runtime < 150000.0 && g->config.play.random[0] == 5 && 0 < g->config.play.randFix[0]) {
				g->gameplay.playerstat.trial = level + 1;
			}
	}
	SetObjectStrings_SongSelect(g);
	g->gameplay.trialClear = level < g->gameplay.playerstat.trial;
	return 0;
}

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
	if (g->config.play.m_gas) {
		g->gameplay.player[0].lastCourseGaugeType = g->gameplay.player[0].gaugeType; // If you finish a course stage with exscore 0, this code won't run and you may start the next stage with same gauge as one displayed on result... First hit note will reset it back to normal.
		g->gameplay.player[0].gaugeType = GetBestClearedGauge(g->gameplay, 0, g->config.play, g->gameplay.courseStageNow != 0);
	}
	auto is_gauge_better = [](int gauge1, int gauge2) {
		if (gauge1 == 5) return true;
		if (gauge2 == 5) return false;
		constexpr std::array<int, 6> gaugeWeight({ 1, 2, 3, 0, 4 });
		return gaugeWeight[gauge1] > gaugeWeight[gauge2];
	};
	if (g->gameplay.courseStageNow == 0 || is_gauge_better(g->gameplay.player[0].clearGaugeTypeCourse, g->gameplay.player[0].gaugeType)) {
		g->gameplay.player[0].clearGaugeTypeCourse = g->gameplay.player[0].gaugeType;
	}
	CheckClear(&g->gameplay.player[0], g->gameplay.player[0].gaugeType, g->gameplay.isCourse);
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
				bms.mybest.op_best = g->gameplay.player[0].gaugeType + g->config.play.random[0] * 10;
			}
			else {
				bms.mybest.op_best = g->gameplay.player[0].gaugeType + g->config.play.random[0] * 10 + g->config.play.random[1] * 100 + g->config.play.dpflip * 1000;
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

		if (g->net.isOnline && g->is_starter == 0) {
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
			if (g->config.play.m_gas) {
				g->gameplay.player[1].lastCourseGaugeType = g->gameplay.player[1].gaugeType;
				g->gameplay.player[1].gaugeType = GetBestClearedGauge(g->gameplay, 1, g->config.play, g->gameplay.courseStageNow != 0);
			}
			if (g->gameplay.courseStageNow == 0 || is_gauge_better(g->gameplay.player[1].clearGaugeTypeCourse, g->gameplay.player[1].gaugeType)) {
				g->gameplay.player[1].clearGaugeTypeCourse = g->gameplay.player[1].gaugeType;
			}
			CheckClear(&g->gameplay.player[1], g->gameplay.player[1].gaugeType, g->gameplay.isCourse);
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

			else if (((g->config.play.p1_assist == 1 || g->config.play.p2_assist == 1) || g->config.play.hsfix == 4 || (g->config.play.random[0] > 3 || g->config.play.random[1] > 3)) && g->is_starter == 0) { 
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
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->gameplay.player[0].gaugeType + g->config.play.random[0] * 10;
					}
					else {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->gameplay.player[0].gaugeType + g->config.play.random[0] * 10 + g->config.play.random[1] * 100 + g->config.play.dpflip * 1000;
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

				if (g->net.isOnline && g->is_starter == 0) {
					if (g->gameplay.flag_longsound) {
						g->net.IRresultMessage = "この曲はIRに登録できません";
					}
					else {
						if (g->gameplay.isCourse == 0) {
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


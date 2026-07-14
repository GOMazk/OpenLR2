#include "LR2_statplay.h"
#include "Engine.h"
#include "LR2_statlong.h"
#include "LR2.h"
#include "Scene02_Songselect.h" //objstr in CheckMission()

bool CheckScoreSaveConditon(game *g){ //TOFIX : p2_assist == 1 but no battle, doesn't match with actual condition
	if (g->config.play.battle != OPTION_BATTLE_OFF && g->config.play.battle != OPTION_BATTLE_GBATTLE)	return false;
	if (g->config.play.m_isExtra)									return false;
	if (g->config.play.m_addlong != 0)								return false;
	if (g->config.play.m_loudness > 0)								return false;
	if (g->config.play.m_isLunaris)									return false;
	if (g->config.play.hsfix == OPTION_HSFIX_CONSTANT)				return false;
	if (g->config.play.m_addmine != 0)								return false;
	if (g->config.play.m_addnote != 0)								return false;
	if (g->config.play.autokey)										return false;
	if (g->config.play.assist[PLAYER_1] != 0)						return false;
	if (g->config.play.assist[PLAYER_2] != 0 && g->sSelect.metaSelected.keymode >= 10)	return false;
	if (g->config.play.random[PLAYER_1] >= OPTION_RANDOM_SCATTER)	return false;
	if (g->config.play.random[PLAYER_2] >= OPTION_RANDOM_SCATTER)	return false;

	return true;
}

int CheckClearLampChallenge(game *g){ //TOFIX : assist[PLAYER_2] == 1 but no battle, doesn't match with actual condition
	if (g->config.play.m_addlong == 1) 								return 0;
	if (g->config.play.m_loudness > 0) 								return 0;
	if (g->config.play.m_isLunaris) 								return 0;
	if (g->config.play.m_addlong > 0) 								return 0;
	if (g->config.play.m_addmine) 									return 0;
	if (g->config.play.m_addnote) 									return 0;
	if (g->config.play.battle == OPTION_BATTLE_BATTLE) 				return 0;

	if (g->config.play.random[PLAYER_1] >= OPTION_RANDOM_SCATTER) 	return 1;
	if (g->config.play.random[PLAYER_2] >= OPTION_RANDOM_SCATTER) 	return 1;
	if (g->config.play.hsfix == OPTION_HSFIX_CONSTANT) 				return 1;
	if (g->config.play.autokey) 									return 1;
	if (g->config.play.assist[PLAYER_1] != 0)						return 1;
	if (g->config.play.assist[PLAYER_2] != 0 && g->sSelect.metaSelected.keymode >= 10)	return 1;

	//not assist
	switch (g->procSelecter == 4 || g->procSelecter == 5 || g->procSelecter == 13
		? g->gameplay.player[PLAYER_1].gaugeType
		: g->config.play.gaugeType[PLAYER_1]) {
	case OPTION_GAUGE_EASY:
		return 1;
	default: //TODO: change undefined gauge to assist
	case OPTION_GAUGE_GROOVE:
		return 2;
	case OPTION_GAUGE_GATTACK:
	case OPTION_GAUGE_HARD:
		return 3;
	case OPTION_GAUGE_DEATH:
	case OPTION_GAUGE_PATTACK:
		return 4;
	}
}

uint ConvertOptionHistory(game *g){

	int clear;
	int gauge;
	uint ret;

	ret = 0;
	if ( g->config.play.assist[PLAYER_1] == 1 || g->config.play.assist[PLAYER_2] == 1 ) {
		if (g->gameplay.player[PLAYER_1].clearType > 1) {
			ret = 0x1000000;
		}
	}
	else {
		clear = g->gameplay.player[PLAYER_1].clearType;
		gauge = g->gameplay.player[PLAYER_1].gaugeType;
		if (clear == 2) {
			if (gauge == 3) {
				ret = 8;
			}
			return ret;
		}
		if (clear > 2) {
			static_assert(OPTION_GAUGE_END < 8);
			static_assert(OPTION_RANDOM_END < 8);
			//static_assert(OPTION_HIDSUD_END < 8); //TODO
			ret = 0;
			if (gauge <= OPTION_GAUGE_END)
				ret = ret | 0x1 << gauge ;
			else
				ErrorLogFmtAdd("BUG: invalid gauge: %d", gauge);
			if (g->config.play.random[PLAYER_1] <= OPTION_RANDOM_END)
				ret = ret | 0x100 << g->config.play.random[PLAYER_1];
			else
				ErrorLogFmtAdd("BUG: invalid random: %d", g->config.play.random);
			if (g->config.play.m_HIDSUD[PLAYER_1] <= 3)
				ret = ret | 0x10000 << g->config.play.m_HIDSUD[PLAYER_1];
			else
				ErrorLogFmtAdd("BUG: invalid m_HIDSUD[PLAYER_1]: %d", g->config.play.m_HIDSUD[PLAYER_1]);
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
	if (gaugeType == OPTION_GAUGE_HARD || gaugeType == OPTION_GAUGE_GATTACK || gaugeType == OPTION_GAUGE_PATTACK) {
		if ( pstat->note_current == pstat->totalnotes && pstat->HP[gaugeType] >= 2.0) {
			pstat->clearType = 4;
		}
	}
	else {
		if (isCourse) {
			if (pstat->note_current != pstat->totalnotes)
				return pstat->clearType;
			if (pstat->HP[gaugeType] >= 2.0) {
				pstat->clearType = (gaugeType != OPTION_GAUGE_EASY) + 2;
				return pstat->clearType;
			}
		}
		if (pstat->note_current == pstat->totalnotes && pstat->HP[gaugeType] >= 80.0) {
			pstat->clearType = (gaugeType != OPTION_GAUGE_EASY) + 2;
			return pstat->clearType;
		}
	}
	return pstat->clearType;
}


int FlipScore(game *g){
	
	PLAYERSTATUS tmp;
	GRAPHDATA tmp2;

	ErrorLogAdd("左右のスコアを反転させました\n");
	
	memcpy(&tmp, &g->gameplay.player[PLAYER_1], sizeof(PLAYERSTATUS));
	memcpy(&g->gameplay.player[PLAYER_1], &g->gameplay.player[PLAYER_2], sizeof(PLAYERSTATUS));
	memcpy(&g->gameplay.player[PLAYER_2], &tmp, sizeof(PLAYERSTATUS));

	memcpy(&tmp2, &g->gameplay.statgraph[PLAYER_1], sizeof(GRAPHDATA));
	memcpy(&g->gameplay.statgraph[PLAYER_1], &g->gameplay.statgraph[PLAYER_2], sizeof(GRAPHDATA));
	memcpy(&g->gameplay.statgraph[PLAYER_2], &tmp2, sizeof(GRAPHDATA));

	g->gameplay.player[PLAYER_1].clearType = g->gameplay.player[PLAYER_2].clearType;

	return 1;
}

// Also see PerformGAS
static int GetBestClearedGauge(const gameplay& gameplay, int playerIdx, const CONFIG_PLAY& cfg, bool limitToCourse) {
	const PLAYERSTATUS& player = gameplay.player[playerIdx];
	if (gameplay.isAutoplay) return player.gaugeType;
	if (playerIdx == PLAYER_2 && gameplay.ghostBattle) return player.gaugeType;
	if (cfg.gaugeType[playerIdx] == OPTION_GAUGE_GATTACK) return OPTION_GAUGE_GATTACK;
	constexpr std::array<int, 5> gaugeArr({ OPTION_GAUGE_PATTACK, OPTION_GAUGE_DEATH, OPTION_GAUGE_HARD, OPTION_GAUGE_GROOVE, OPTION_GAUGE_EASY });
	unsigned int i = 0;
	if (limitToCourse)
		for (; i < gaugeArr.size(); i++)
			if (gaugeArr[i] == gameplay.player[playerIdx].clearGaugeTypeCourse)
				break;
	auto is_gauge_alive = [](int gaugeIdx, double hp) {
		switch (gaugeIdx) {
		case OPTION_GAUGE_GROOVE:
		case OPTION_GAUGE_EASY:
			return hp >= 80.;
		default: return hp >= 2.;
		}
	};
	for (; i < gaugeArr.size(); i++)
		if (is_gauge_alive(gaugeArr[i], player.HP[gaugeArr[i]]))
			return gaugeArr[i];
	if (gameplay.courseType == 2) return OPTION_GAUGE_GROOVE;
	return OPTION_GAUGE_EASY;
}

int CheckCourseClear(game* g) {
	if (g->gameplay.courseStageNow < g->gameplay.courseStageCount - 1) {
		for (int i = g->gameplay.courseStageNow - 1; i < g->gameplay.courseStageCount; i++) {
			g->gameplay.player[PLAYER_1].total_note += g->sSelect.bmsList[g->sSelect.cur_song].courseTotalnote[i];
			g->gameplay.player[PLAYER_2].total_note += g->sSelect.bmsList[g->sSelect.cur_song].courseTotalnote[i];
		}
	}

	std::array<int, 2> gauge = { g->gameplay.player[PLAYER_1].clearGaugeTypeCourse, g->gameplay.player[PLAYER_2].clearGaugeTypeCourse };
	for (int p : { PLAYER_1, PLAYER_2 }) {
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
		else if (gauge[p] == OPTION_GAUGE_HARD || gauge[p] == OPTION_GAUGE_GATTACK || gauge[p] == OPTION_GAUGE_PATTACK) {
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

	if (g->config.play.assist[PLAYER_1] == 1) 
		return 0;

	gauge = g->gameplay.player[PLAYER_1].gaugeType;

	if (g->gameplay.player[PLAYER_1].gaugeType == OPTION_GAUGE_EASY)
		return 0;
	if (g->gameplay.player[PLAYER_2].gaugeType == OPTION_GAUGE_EASY)
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
			if (g->gameplay.player[PLAYER_1].totalnotes >= 100 && gauge == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 2:
			if ( (g->config.play.m_HIDSUD[PLAYER_1] == g->config.play.m_HIDSUD[PLAYER_2] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) 
				&& g->gameplay.player[PLAYER_1].totalnotes >= 100 && g->config.play.m_HIDSUD[PLAYER_1] == 1) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 3:
			if ((g->config.play.m_HIDSUD[PLAYER_1] == g->config.play.m_HIDSUD[PLAYER_2] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10)
				&& g->gameplay.player[PLAYER_1].totalnotes >= 100 && g->config.play.m_HIDSUD[PLAYER_1] == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 4:
			if (gauge == 0 && 80.0 <= g->gameplay.player[PLAYER_1].HP[gauge] && g->gameplay.player[PLAYER_1].HP[gauge] < 86.0) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 5:
			if (g->config.play.random[PLAYER_1] != g->config.play.random[PLAYER_2]) {
				if (9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) {
					level = g->gameplay.playerstat.trial;
					break;
				}
				level = g->gameplay.playerstat.trial;
			}
			if (g->gameplay.player[PLAYER_1].totalnotes >= 100 && gauge == 1 && g->config.play.random[PLAYER_1] == 3) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 6:
			if (g->gameplay.player[PLAYER_1].totalnotes >= 100 && g->audio.param.pitch_on == 1 && (g->audio.param.pitch_type == 0 || g->audio.param.pitch_type == 2)) {
				if (g->audio.param.pitch_amount >= 3) {
					g->gameplay.playerstat.trial = level + 1;
				}
			}
			break;
		case 7:
			gauge = g->config.play.random[PLAYER_1];
			if ((g->config.play.random[PLAYER_1] == g->config.play.random[PLAYER_2] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) 
				&& g->gameplay.player[PLAYER_1].totalnotes >= 100 && g->config.play.random[PLAYER_1] == OPTION_RANDOM_CONVERGE) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 8:
			gauge = g->config.play.m_HIDSUD[PLAYER_1];
			if ((g->config.play.m_HIDSUD[PLAYER_1] == g->config.play.m_HIDSUD[PLAYER_2] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) 
				&& g->gameplay.player[PLAYER_1].totalnotes >= 100 && g->config.play.m_HIDSUD[PLAYER_1] == 3) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 9:
			if (g->gameplay.player[PLAYER_1].exscore >= (g->gameplay.player[PLAYER_1].totalnotes * 2)* 8/9) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 10:
			if (g->gameplay.playerstat.combo >= 2000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 11:
			if (g->gameplay.player[PLAYER_1].totalnotes >= 500 && gauge == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 12:
			if ((g->config.play.m_HIDSUD[PLAYER_1] == g->config.play.m_HIDSUD[PLAYER_2] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) 
				&& 3 < g->gameplay.player[PLAYER_1].clearType && g->gameplay.player[PLAYER_1].totalnotes >= 500 && gauge == 1 && g->config.play.m_HIDSUD[PLAYER_1] == 1) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 13:
			if ((g->config.play.m_HIDSUD[PLAYER_1] == g->config.play.m_HIDSUD[PLAYER_2] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10)
				&& 3 < g->gameplay.player[PLAYER_1].clearType && g->gameplay.player[PLAYER_1].totalnotes >= 500 && gauge == 1 && g->config.play.m_HIDSUD[PLAYER_1] == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 14:
			if ((g->config.play.random[PLAYER_1] == g->config.play.random[PLAYER_2] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10)
				&& g->gameplay.player[PLAYER_1].totalnotes >= 500 && g->config.play.random[PLAYER_1] == OPTION_RANDOM_CONVERGE) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 15:
			if (g->gameplay.isSpeedChanged == false && g->config.play.hiSpeed[PLAYER_1] == 50 &&
				g->config.play.hsfix == OPTION_HSFIX_CONSTANT && g->gameplay.player[PLAYER_1].totalnotes >= 500) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 16:
			if (g->config.play.random[PLAYER_1] != g->config.play.random[PLAYER_2] && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) break;

			if (g->config.play.m_HIDSUD[PLAYER_1] != g->config.play.m_HIDSUD[PLAYER_2]) {
				if (9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) {
					level = g->gameplay.playerstat.trial;
					break;
				}
				level = g->gameplay.playerstat.trial;
			}
			if (g->gameplay.player[PLAYER_1].totalnotes >= 500 && g->config.play.random[PLAYER_1] == OPTION_RANDOM_RANDOM && g->config.play.m_HIDSUD[PLAYER_1] == 3) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 17:
			if (g->gameplay.player[PLAYER_1].max_combo == 333) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 18:
			if (gauge == 4 && g->gameplay.player[PLAYER_1].totalnotes >= 100) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 19:
			if (gauge != 5) break;
			if (g->gameplay.player[PLAYER_1].totalnotes >= 100) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 20:
			if (gauge == 0 && g->gameplay.player[PLAYER_1].HP[gauge] >= 80.0 && g->gameplay.player[PLAYER_1].HP[gauge] < 82.0) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 21:
			if (gauge == 1 && g->gameplay.player[PLAYER_1].HP[gauge] < 4.0) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 22:
			if ((g->config.play.random[PLAYER_1] != g->config.play.random[PLAYER_2] && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) || gauge != 2) break;
			if (g->config.play.random[PLAYER_1] == OPTION_RANDOM_SRANDOM) {
				if (g->gameplay.player[PLAYER_1].totalnotes >= 1000) {
					g->gameplay.playerstat.trial = level + 1;
				}
			}
			break;
		case 23:
			if ((g->config.play.m_HIDSUD[PLAYER_1] != g->config.play.m_HIDSUD[PLAYER_2] && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode)
				|| g->config.play.m_HIDSUD[PLAYER_1] != 1 || g->config.play.hsfix != OPTION_HSFIX_CONSTANT || g->config.play.hiSpeed[PLAYER_1] != 150)
				break;
			if (g->gameplay.player[PLAYER_1].totalnotes >= 1000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 24:
			if ((g->config.play.m_HIDSUD[PLAYER_1] != g->config.play.m_HIDSUD[PLAYER_2] && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode)
				|| g->config.play.m_HIDSUD[PLAYER_1] != 2 || g->config.play.hsfix != OPTION_HSFIX_CONSTANT) break;
			if (g->config.play.hiSpeed[PLAYER_1] == 250) {
				if (g->gameplay.player[PLAYER_1].totalnotes >= 1000) {
					g->gameplay.playerstat.trial = level + 1;
				}
			}
			break;
		case 25:
			if (g->config.play.random[PLAYER_1] != g->config.play.random[PLAYER_2] && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) break;
			if (g->config.play.random[PLAYER_1] == OPTION_RANDOM_CONVERGE) {
				if (g->gameplay.player[PLAYER_1].totalnotes >= 1000) {
					g->gameplay.playerstat.trial = level + 1;
				}
			}
			break;
		case 26:
			if (g->config.play.m_HIDSUD[PLAYER_1] != g->config.play.m_HIDSUD[PLAYER_2] && 9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) break;
			if (g->config.play.random[PLAYER_1] != g->config.play.random[PLAYER_2]) {
				if (9 < g->sSelect.bmsList[g->sSelect.cur_song].keymode) {
					level = g->gameplay.playerstat.trial;
					break;
				}
				level = g->gameplay.playerstat.trial;
			}
			if (g->config.play.random[PLAYER_1] == OPTION_RANDOM_RANDOM && g->config.play.m_HIDSUD[PLAYER_1] == 3 && g->gameplay.player[PLAYER_1].totalnotes >= 1000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 27:
			if (g->audio.param.pitch_on != 1 || (g->audio.param.pitch_type != 0 && g->audio.param.pitch_type != 2) || g->audio.param.pitch_amount < 6) break;
			if (g->gameplay.player[PLAYER_1].totalnotes >= 1000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 28:
			if (g->gameplay.isSpeedChanged == false && g->config.play.hsfix == OPTION_HSFIX_CONSTANT && g->config.play.hiSpeed[PLAYER_1] == 600 && g->gameplay.player[PLAYER_1].totalnotes >= 300) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 29:
			if ((g->gameplay.player[PLAYER_1].exscore == g->gameplay.player[PLAYER_1].totalnotes *2 *8 / 9) && g->gameplay.player[PLAYER_1].totalnotes >= 1000) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 30:
			if (g->gameplay.player[PLAYER_1].totalnotes == 9 && g->gameplay.player[PLAYER_1].judgetime[5] == 8 && g->gameplay.player[PLAYER_1].judgecount[5] == 9) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 31:
			if (g->gameplay.player[PLAYER_1].totalnotes >= 1000 && g->gameplay.song_runtime < 150000.0 && gauge == 4) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 32:
			if (g->gameplay.player[PLAYER_1].totalnotes >= 1000 && g->gameplay.song_runtime < 150000.0 && gauge == 5) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 33:
			if (g->gameplay.player[PLAYER_1].totalnotes >= 1000 && g->gameplay.song_runtime < 150000.0 && g->gameplay.player[PLAYER_1].max_combo < 40) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 34:
			if (g->gameplay.isSpeedChanged == false && g->gameplay.player[PLAYER_1].totalnotes >= 1200 && g->config.play.hsfix == OPTION_HSFIX_CONSTANT && g->gameplay.song_runtime < 150000.0 && g->config.play.hiSpeed[PLAYER_1] == 30) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 35:
			if (g->gameplay.player[PLAYER_1].totalnotes >= 1200 && g->gameplay.song_runtime < 150000.0 && gauge == 1 && g->config.play.random[PLAYER_1] == OPTION_RANDOM_CONVERGE) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 36:
			if (gauge == 2 && 998 <= g->gameplay.player[PLAYER_1].exscore && g->gameplay.player[PLAYER_1].exscore <= 1002) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 37:
			if (g->audio.param.pitch_on == 1 && (g->audio.param.pitch_type == 0 || g->audio.param.pitch_type == 2) &&
				g->audio.param.pitch_amount == 12 && g->gameplay.player[PLAYER_1].totalnotes >= 1000 && g->gameplay.song_runtime < 150000.0) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 38:
			if (g->gameplay.player[PLAYER_1].totalnotes >= 1500 && g->gameplay.song_runtime < 150000.0 && gauge == 2) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 39:
			if (g->gameplay.isSpeedChanged == false && (g->config.play.m_HIDSUD[PLAYER_1] == g->config.play.m_HIDSUD[PLAYER_2] || g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10)
				&& g->gameplay.player[PLAYER_1].totalnotes >= 1500 && g->gameplay.song_runtime < 150000.0 && g->config.play.hiSpeed[PLAYER_1] == 100
				&& g->config.play.m_HIDSUD[PLAYER_1] == 1 && g->config.play.hsfix == OPTION_HSFIX_CONSTANT) {
				g->gameplay.playerstat.trial = level + 1;
			}
			break;
		case 40:
			if (g->gameplay.player[PLAYER_1].totalnotes >= 1500 && g->gameplay.song_runtime < 150000.0 && g->config.play.random[PLAYER_1] == OPTION_RANDOM_CONVERGE && 0 < g->config.play.randFix[PLAYER_1]) {
				g->gameplay.playerstat.trial = level + 1;
			}
	}
	SetObjectStrings_SongSelect(g);
	g->gameplay.trialClear = level < g->gameplay.playerstat.trial;
	return 0;
}

int SaveResult(game *g, sqlite3* sql) {
	g->net.WaitForRankingHandle();
	g->net.rankingData.Init();
	PlayerCheckAndSwap(&g->gameplay);
	SetObjectString(20, g->net.IRresultMessage.fillzero(), g->txtStruct.objectStr);

	memcpy(&g->sSelect.old, &g->sSelect.bmsList[g->sSelect.cur_song].mybest,sizeof(STATUS));
	if (g->is_starter) {
		g->gameplay.isForceEasy = 0;
		g->gameplay.isNosave = 0;
	}

	g->gameplay.player[PLAYER_1].lastCourseGaugeType = g->gameplay.player[PLAYER_1].gaugeType; // If you finish a course stage with exscore 0, this code won't run and you may start the next stage with same gauge as one displayed on result... First hit note will reset it back to normal.

	if (g->gameplay.isAutoplay) return -1;

	if (g->config.play.m_gas && g->gameplay.replay.status != 2) {
		g->gameplay.player[PLAYER_1].gaugeType = GetBestClearedGauge(g->gameplay, 0, g->config.play, g->gameplay.courseStageNow != 0);
	}
	auto is_gauge_better = [](int gauge1, int gauge2) {
		if (gauge1 == 5) return true;
		if (gauge2 == 5) return false;
		constexpr std::array<int, 6> gaugeWeight({ 1, 2, 3, 0, 4 });
		return gaugeWeight[gauge1] > gaugeWeight[gauge2];
	};
	if (g->gameplay.courseStageNow == 0 || is_gauge_better(g->gameplay.player[PLAYER_1].clearGaugeTypeCourse, g->gameplay.player[PLAYER_1].gaugeType)) {
		g->gameplay.player[PLAYER_1].clearGaugeTypeCourse = g->gameplay.player[PLAYER_1].gaugeType;
	}
	CheckClear(&g->gameplay.player[PLAYER_1], g->gameplay.player[PLAYER_1].gaugeType, g->gameplay.isCourse);
	if (g->gameplay.courseType == 0 || g->gameplay.courseType == 2) {
		ErrorLogFmtAdd("エキスパ用のスコア保存処理を行います\n");
		if (CheckScoreSaveConditon(g) == 0) return -1;
		SONGDATA bms;

		GetSongData(g->sSelect.bmsList[g->sSelect.cur_song].courseHash[g->gameplay.courseStageNow], &bms, sql, &g->sSelect);
		memcpy(&g->sSelect.old, &bms.mybest, sizeof(STATUS));

		g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;
		if (bms.mybest.total_notes == 0) {
			bms.mybest.total_notes = g->gameplay.player[PLAYER_1].totalnotes;
			if (bms.mybest.total_notes > 0) {
				bms.mybest.rank = g->gameplay.player[PLAYER_1].exscore * 9 / (bms.mybest.total_notes * 2);
				if (bms.mybest.rank > 8) bms.mybest.rank = 8;
				if (bms.mybest.rank < 1 && g->gameplay.player[PLAYER_1].exscore > 0) bms.mybest.rank = 1;
			}
		}

		if (g->gameplay.player[PLAYER_1].totalnotes <= g->gameplay.player[PLAYER_1].note_current) {
			bms.mybest.complete = 1;
		}

		if (g->gameplay.player[PLAYER_1].clearType == 5) {
			bms.mybest.clear = 5;
			if (1 <= bms.difficulty && bms.difficulty <= 5) {
				bms.difficultyLevelBarLamp[bms.difficulty - 1] = 5;
			}
		}

		bool isNewRecord = false;
		if (bms.mybest.stat_great + bms.mybest.stat_pgreat * 2 < g->gameplay.player[PLAYER_1].exscore) {
			bms.mybest.total_notes = g->gameplay.player[PLAYER_1].totalnotes;
			bms.mybest.stat_pgreat = g->gameplay.player[PLAYER_1].judgecount[5];
			bms.mybest.stat_great = g->gameplay.player[PLAYER_1].judgecount[4];
			bms.mybest.stat_good = g->gameplay.player[PLAYER_1].judgecount[3];
			bms.mybest.stat_bad = g->gameplay.player[PLAYER_1].judgecount[2];
			bms.mybest.stat_poor = g->gameplay.player[PLAYER_1].judgecount[1];
			bms.mybest.stat_score = g->gameplay.player[PLAYER_1].score;
			bms.mybest.stat_exscore = g->gameplay.player[PLAYER_1].exscore;
			bms.mybest.rate = (bms.mybest.stat_exscore * 100) / (bms.mybest.total_notes * 2);
			bms.mybest.rank = (bms.mybest.stat_exscore * 9) / (bms.mybest.total_notes * 2);


			if (bms.mybest.rank > 8)
				bms.mybest.rank = 8;
			if (bms.mybest.rank < 1 && bms.mybest.stat_exscore > 0)
				bms.mybest.rank = 1;

			bms.mybest.rseed = g->gameplay.randomseed;

			if (bms.keymode < 10) {
				bms.mybest.op_best = g->gameplay.player[PLAYER_1].gaugeType + g->config.play.random[PLAYER_1] * 10;
			}
			else {
				bms.mybest.op_best = g->gameplay.player[PLAYER_1].gaugeType + g->config.play.random[PLAYER_1] * 10 + g->config.play.random[PLAYER_2] * 100 + (int)g->config.play.dpFlip * 1000;
			}

			isNewRecord = true;
		}

		if (bms.mybest.stat_maxcombo < g->gameplay.player[PLAYER_1].max_combo)
			bms.mybest.stat_maxcombo = g->gameplay.player[PLAYER_1].max_combo;

		if (bms.mybest.total_notes == g->gameplay.player[PLAYER_1].note_current) {
			if (bms.mybest.minbp == -1 || g->gameplay.player[PLAYER_1].judgecount[2] + g->gameplay.player[PLAYER_1].judgecount[1] < bms.mybest.minbp) {
				bms.mybest.minbp = g->gameplay.player[PLAYER_1].judgecount[2] + g->gameplay.player[PLAYER_1].judgecount[1];
			}
		}
		else {
			if (bms.mybest.minbp == -1 || g->gameplay.player[PLAYER_1].judgecount[2] + g->gameplay.player[PLAYER_1].judgecount[1] + (bms.mybest.total_notes - g->gameplay.player[PLAYER_1].note_current) < bms.mybest.minbp) {
				bms.mybest.minbp = g->gameplay.player[PLAYER_1].judgecount[2] + g->gameplay.player[PLAYER_1].judgecount[1] + (bms.mybest.total_notes - g->gameplay.player[PLAYER_1].note_current);
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
				const CSTR ghostString = ReadGhost(sql, bms.hash);
				g->net.MakeIRsendScoreThread(ghostString.body);

				if (!g->config.network.displayIr.length()) {
					bms.mybest.IRranking = g->net.rankingData.myRanking;
					bms.mybest.IRplayercount = g->net.rankingData.rankingCount;
					if (g->net.rankingData.rankingCount > 0) {
						bms.mybest.IRclearRate = (g->net.rankingData.rankingCount + g->net.rankingData.clearPlayers[1] - g->net.rankingData.clearPlayers[0]) / g->net.rankingData.rankingCount;
					}
				}
			}
			SetObjectString(20, g->net.IRresultMessage, g->txtStruct.objectStr);
		}
		return 1;
	}
	else {
		ErrorLogFmtAdd("通常のスコア保存処理を行います\n");
		
		if (g->config.play.battle == OPTION_BATTLE_BATTLE) {
			g->gameplay.player[PLAYER_2].lastCourseGaugeType = g->gameplay.player[PLAYER_2].gaugeType;
			if (g->config.play.m_gas && g->gameplay.replay.status != 2) {
				g->gameplay.player[PLAYER_2].gaugeType = GetBestClearedGauge(g->gameplay, 1, g->config.play, g->gameplay.courseStageNow != 0);
			}
			if (g->gameplay.courseStageNow == 0 || is_gauge_better(g->gameplay.player[PLAYER_2].clearGaugeTypeCourse, g->gameplay.player[PLAYER_2].gaugeType)) {
				g->gameplay.player[PLAYER_2].clearGaugeTypeCourse = g->gameplay.player[PLAYER_2].gaugeType;
			}
			CheckClear(&g->gameplay.player[PLAYER_2], g->gameplay.player[PLAYER_2].gaugeType, g->gameplay.isCourse);
		}
		else {
			g->gameplay.player[PLAYER_2].clearType = g->gameplay.player[PLAYER_1].clearType;
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
		
		if (g->gameplay.isForceEasy && g->gameplay.player[PLAYER_1].clearType > 2) {
			if (g->gameplay.player[PLAYER_1].clearType == 5 && (g->config.play.assist[PLAYER_1] == 0 && g->config.play.assist[PLAYER_2] == 0) && ((g->config.play.random[PLAYER_1] != OPTION_RANDOM_SCATTER && g->config.play.random[PLAYER_2] != OPTION_RANDOM_SCATTER) || g->gameplay.minBPM == g->gameplay.maxBPM)) {
				g->gameplay.isForceEasy = 0;
			}
			else {
				g->gameplay.player[PLAYER_1].clearType = 2;
			}
		}

		if (g->config.play.battle != OPTION_BATTLE_BATTLE || g->is_starter) {

			if ((g->gameplay.freqSpeedMultiplier < 1.0 || g->config.play.m_isLunaris) && g->is_starter == 0) {
				if (g->gameplay.replay.status == 2) return -1;

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType < 2) g->gameplay.playerstat.fail++;
				else g->gameplay.playerstat.clear++;

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				return 0;
			}

			else if (g->config.play.battle == OPTION_BATTLE_DBATTLE && g->is_starter == 0) {
				if (g->gameplay.replay.status == 2) return -1;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_db < g->gameplay.player[PLAYER_1].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_db = g->gameplay.player[PLAYER_1].clearType;
				}

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType < 2) g->gameplay.playerstat.fail++;
				else {
					g->gameplay.playerstat.clear++;
					if (g->gameplay.player[PLAYER_1].clearType > 2) g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history |= 0x4000000;
				}

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				g->gameplay.isNosave = 1;
				return 0;
			}

			else if (g->config.play.battle == OPTION_BATTLE_SP2DP && g->is_starter == 0) {
				if (g->gameplay.replay.status == 2) return -1;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_sd < g->gameplay.player[PLAYER_1].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_sd = g->gameplay.player[PLAYER_1].clearType;
				}

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType < 2) g->gameplay.playerstat.fail++;
				else {
					g->gameplay.playerstat.clear++;
					if (g->gameplay.player[PLAYER_1].clearType > 2) g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history |= 0x8000000;
				}

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				g->gameplay.isNosave = 1;
				return 0;
			}

			else if (g->config.play.m_isExtra && g->is_starter == 0){
				if (g->gameplay.replay.status == 2) return -1;
				
				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_ex < g->gameplay.player[PLAYER_1].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear_ex = g->gameplay.player[PLAYER_1].clearType;
				}

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType < 2) g->gameplay.playerstat.fail++;
				else {
					g->gameplay.playerstat.clear++;
					if (g->gameplay.player[PLAYER_1].clearType > 2) g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_history |= 0x2000000;
				}

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;
				
				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType >= 2)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount++;
				else
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.failcount++;

				UpdateScoreDB(g->sSelect.bmsList[g->sSelect.cur_song].hash, &g->sSelect.bmsList[g->sSelect.cur_song].mybest, sql, &g->sSelect.playerPassMD5);
				UpdatePlayerStat(&g->gameplay.playerstat, sql);
				g->gameplay.isNosave = 1;
				return 0;
			}

			else if (((g->config.play.assist[PLAYER_1] == 1 || g->config.play.assist[PLAYER_2] == 1) || g->config.play.hsfix == OPTION_HSFIX_CONSTANT || (g->config.play.random[PLAYER_1] > OPTION_RANDOM_SRANDOM || g->config.play.random[PLAYER_2] > OPTION_RANDOM_SRANDOM)) && g->is_starter == 0) { 
				if (g->gameplay.replay.status == 2) return -1;

				if (g->gameplay.player[PLAYER_1].clearType > 2) g->gameplay.player[PLAYER_1].clearType = 2;

				g->gameplay.playerstat.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType < 2) g->gameplay.playerstat.fail++;
				else g->gameplay.playerstat.clear++;

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear < g->gameplay.player[PLAYER_1].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear = g->gameplay.player[PLAYER_1].clearType;
					if (1 <= g->sSelect.bmsList[g->sSelect.cur_song].difficulty && g->sSelect.bmsList[g->sSelect.cur_song].difficulty <= 5) {
						g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevelBarLamp[g->sSelect.bmsList[g->sSelect.cur_song].difficulty - 1] = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear;
					}
				}

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType >= 2)
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
				if (g->gameplay.player[PLAYER_1].clearType < 2) g->gameplay.playerstat.fail++;
				else g->gameplay.playerstat.clear++;

				g->gameplay.playerstat.playtime += (int)GetTimeLapse(41, &g->timer1) / 1000;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes == 0) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes = g->gameplay.player[PLAYER_1].totalnotes;
					if (g->gameplay.player[PLAYER_1].totalnotes > 0) {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = g->gameplay.player[PLAYER_1].exscore * 9 / (g->gameplay.player[PLAYER_1].totalnotes * 2);
						if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank > 8) g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 8;
						if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank < 2 && g->gameplay.player[PLAYER_1].exscore > 0) g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 1;
					}
				}

				if (g->gameplay.player[PLAYER_1].totalnotes <= g->gameplay.player[PLAYER_1].note_current) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.complete = 1;
				}

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear < g->gameplay.player[PLAYER_1].clearType) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear = g->gameplay.player[PLAYER_1].clearType;
					if (1 <= g->sSelect.bmsList[g->sSelect.cur_song].difficulty && g->sSelect.bmsList[g->sSelect.cur_song].difficulty <= 5) {
						g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevelBarLamp[g->sSelect.bmsList[g->sSelect.cur_song].difficulty - 1] = g->sSelect.bmsList[g->sSelect.cur_song].mybest.clear;
					}
				}

				bool isNewRecord = false;
				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great + g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat * 2 < g->gameplay.player[PLAYER_1].exscore) {
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes = g->gameplay.player[PLAYER_1].totalnotes;
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat = g->gameplay.player[PLAYER_1].judgecount[5];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great = g->gameplay.player[PLAYER_1].judgecount[4];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good = g->gameplay.player[PLAYER_1].judgecount[3];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad = g->gameplay.player[PLAYER_1].judgecount[2];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor = g->gameplay.player[PLAYER_1].judgecount[1];
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score = g->gameplay.player[PLAYER_1].score;
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore = g->gameplay.player[PLAYER_1].exscore;
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.rate = (g->gameplay.player[PLAYER_1].exscore * 100) / (g->gameplay.player[PLAYER_1].totalnotes * 2);
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = (g->gameplay.player[PLAYER_1].exscore * 9) / (g->gameplay.player[PLAYER_1].totalnotes * 2);


					if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank > 8)
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 8;
					if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank < 1 && g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore > 0)
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.rank = 1;

					g->sSelect.bmsList[g->sSelect.cur_song].mybest.rseed = g->gameplay.randomseed;

					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode < 10) {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->gameplay.player[PLAYER_1].gaugeType + g->config.play.random[PLAYER_1] * 10;
					}
					else {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.op_best = g->gameplay.player[PLAYER_1].gaugeType + g->config.play.random[PLAYER_1] * 10 + g->config.play.random[PLAYER_2] * 100 + (int)g->config.play.dpFlip * 1000;
					}

					isNewRecord = true;
				}

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo < g->gameplay.player[PLAYER_1].max_combo)
					g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo = g->gameplay.player[PLAYER_1].max_combo;

				if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes == g->gameplay.player[PLAYER_1].note_current) {
					if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp == -1 || g->gameplay.player[PLAYER_1].judgecount[2] + g->gameplay.player[PLAYER_1].judgecount[1] < g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp) {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp = g->gameplay.player[PLAYER_1].judgecount[2] + g->gameplay.player[PLAYER_1].judgecount[1];
					}
				}
				else {
					if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp == -1 || g->gameplay.player[PLAYER_1].judgecount[2] + g->gameplay.player[PLAYER_1].judgecount[1] + (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes - g->gameplay.player[PLAYER_1].note_current) < g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp) {
						g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp = g->gameplay.player[PLAYER_1].judgecount[2] + g->gameplay.player[PLAYER_1].judgecount[1] + (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes - g->gameplay.player[PLAYER_1].note_current);
					}
				}

				g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount++;
				if (g->gameplay.player[PLAYER_1].clearType >= 2)
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
							const CSTR ghostString = ReadGhost(sql, g->sSelect.bmsList[g->sSelect.cur_song].hash);
							g->net.MakeIRsendScoreThread(ghostString.body);

							if (!g->config.network.displayIr.length()) {
								g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = g->net.rankingData.myRanking;
								g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = g->net.rankingData.rankingCount;
								if (g->net.rankingData.rankingCount > 0) {
									g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate = (g->net.rankingData.rankingCount + g->net.rankingData.clearPlayers[1] - g->net.rankingData.clearPlayers[0]) / g->net.rankingData.rankingCount;
								}
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
							const CSTR ghostString = ReadGhost(sql, g->sSelect.bmsList[g->sSelect.cur_song].hash);
							g->net.MakeIRsendScoreThread(ghostString.body);

							if (!g->config.network.displayIr.length()) {
								g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking = g->net.rankingData.myRanking;
								g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount = g->net.rankingData.rankingCount;
								if (g->net.rankingData.rankingCount > 0) {
									g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate = (g->net.rankingData.rankingCount + g->net.rankingData.clearPlayers[1] - g->net.rankingData.clearPlayers[0]) / g->net.rankingData.rankingCount;
								}
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

		if (g->gameplay.player[PLAYER_2].clearType > g->gameplay.player[PLAYER_1].clearType) g->gameplay.player[PLAYER_1].clearType = g->gameplay.player[PLAYER_2].clearType;
		
		if (g->gameplay.replay.status != 2) {
			g->gameplay.playerstat.playcount++;

			if (g->gameplay.player[PLAYER_1].clearType >= 2)
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


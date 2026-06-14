#include "Scene05_Result.h"
#include "LR2.h"
#include "Scenes.h"

int ProcS_Result(game *g) {

	LoadSceneG(g, &g->skstruct, SKINTYPE_RESULT);
	
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

int Proc_Result(game *g, skstruct *sk, Timer *T) {

	if (GetTimeLapse(0, T) > sk->src_GAUGECHART_1P[0].op3 && GetTimeLapse(150, T) < 0.0) {
		SetTimeLapse(150, T);
	}
	if (GetTimeLapse(0, T) > sk->src_GAUGECHART_1P[0].op4 && GetTimeLapse(151, T) < 0.0) {
		SetTimeLapse(151, T);
	}

	if (g->config.play.battle == 1 || sk->flag_flip == 0 || g->gameplay.ghostBattle) {
		
		if (sk->src_GAUGECHART_1P[0].op1 <= 0) return 0;

		int gauge = g->gameplay.player[0].gaugeType;
		int val = g->gameplay.statgraph[0].hp[gauge][0];
		int length = ChangeValueByTime(0.0, sk->src_GAUGECHART_1P[0].op1, sk->src_GAUGECHART_1P[0].op3, sk->src_GAUGECHART_1P[0].op4, GetTimeLapse(0, T), 0);
		for (int i = 0; i < length; i += sk->dst_GAUGECHART_1P[0].draw->w) {
			int axis = i * 1000 / sk->src_GAUGECHART_1P[0].op1;

			int last = 0;
			while (val != g->gameplay.statgraph[0].hp[gauge][axis]) {
				
				if (g->gameplay.isCourse == 0 && gauge != 1 && gauge != 2 && gauge != 5 && gauge != 4 && val < 80) {
					
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

				if (val < g->gameplay.statgraph[0].hp[gauge][axis]) val++;
				if (val > g->gameplay.statgraph[0].hp[gauge][axis]) val--;
			}

			if (g->gameplay.isCourse == 0 && gauge != 1 && gauge != 2 && gauge != 5 && gauge != 4 && g->gameplay.statgraph[0].hp[gauge][axis] < 80) {
				AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_1P[0], &sk->dst_GAUGECHART_1P[0], T, i, sk->src_GAUGECHART_1P[0].op2 * g->gameplay.statgraph[0].hp[gauge][axis] / (-100));
			}
			else {
				AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_1P[1], &sk->dst_GAUGECHART_1P[1], T, i, sk->src_GAUGECHART_1P[1].op2 * g->gameplay.statgraph[0].hp[gauge][axis] / (-100));
			}
		}
	}

	if (g->config.play.battle == 1 || sk->flag_flip != 0 || g->gameplay.ghostBattle) {
		int p = 1;
		if (g->config.play.battle != 1 && sk->flag_flip != 0) p = 0;

		if (sk->src_GAUGECHART_2P[0].op1 <= 0) return 0;

		int gauge = g->gameplay.player[1].gaugeType;
		int val = g->gameplay.statgraph[1].hp[gauge][0];
		int length = ChangeValueByTime(0.0, sk->src_GAUGECHART_2P[0].op1, sk->src_GAUGECHART_2P[0].op3, sk->src_GAUGECHART_2P[0].op4, GetTimeLapse(0, T), 0);
		for (int i = 0; i < length; i += sk->dst_GAUGECHART_2P[0].draw->w) {
			int axis = i * 1000 / sk->src_GAUGECHART_2P[0].op1;

			int last = 0;
			while (val != g->gameplay.statgraph[1].hp[gauge][axis]) {

				if (g->gameplay.isCourse == 0 && gauge != 1 && gauge != 2 && val < 80) {

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

				if (val < g->gameplay.statgraph[1].hp[gauge][axis]) val++;
				if (val > g->gameplay.statgraph[1].hp[gauge][axis]) val--;
			}

			if (g->gameplay.isCourse == 0 && gauge != 1 && gauge != 2 && g->gameplay.statgraph[1].hp[gauge][axis] < 80) {
				AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_2P[0], &sk->dst_GAUGECHART_2P[0], T, i, sk->src_GAUGECHART_2P[0].op2 * g->gameplay.statgraph[1].hp[gauge][axis] / (-100));
			}
			else {
				AddDrawingBuffer_Object(&sk->drBuf, &sk->src_GAUGECHART_2P[1], &sk->dst_GAUGECHART_2P[1], T, i, sk->src_GAUGECHART_2P[1].op2 * g->gameplay.statgraph[1].hp[gauge][axis] / (-100));
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

static void QuickRestart(game& game, bool newRandom) {
	game.procPhase = 0;
	game.procSelecter = 4;
	game.gameplay.flag_retry = newRandom ? 0 : 1;
	game.gameplay.randomseed = newRandom ? 0 : game.gameplay.randomseed;

	if ((game.gameplay.courseType == 0 || game.gameplay.courseType == 2) && game.gameplay.courseStageNow != 0) {
		game.gameplay.courseStageNow = 0;
		game.gameplay.flag_retry = 0;
	}

	if (game.gameplay.flag_retry == 0) ReleaseBGA(&game); // ugly place for this
	StopAllKeysound(&game);
}

char fWaitHiScoreUpdateInput = 0;
int ProcI_Result(game *g) {

	auto switch_gauge_display = [](gameplay& gameplay, int buttonVal, PLAYERSTATUS& player) {
		if (buttonVal == 1 && player.gaugeType != 5) {
			int& gauge = player.gaugeType;
			if (gameplay.courseType != 2) [[likely]] {
				switch (gauge) {
				case 0: gauge = 1; break;
				case 1: gauge = 2; break;
				case 2: gauge = 4; break;
				case 3: gauge = 0; break;
				case 4: gauge = 3; break;
				default: break;
				}
			}
			else {
				switch (gauge) {
				case 0: gauge = 1; break;
				case 1: gauge = 2; break;
				case 2: gauge = 4; break;
				case 4: gauge = 0; break;
				default: break;
				}
			}
		}
	};
	if (g->config.play.m_gas && g->procSelecter != 13 && g->gameplay.replay.status != 2) {
		switch_gauge_display(g->gameplay, g->KeyInput.p1_buttonInput[13], g->gameplay.player[0]);
		if (g->config.play.battle == 1) {
			switch_gauge_display(g->gameplay, g->KeyInput.p2_buttonInput[13], g->gameplay.player[1]);
		}
		else {
			switch_gauge_display(g->gameplay, g->KeyInput.p2_buttonInput[13], g->gameplay.player[0]);
		}
	}

	if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1 || g->KeyInput.inputID[KEY_INPUT_RETURN] == 1
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
			if (GetTimeLapse(151, &g->timer1) <= g->skstruct.startinput_rank || GetTimeLapse(152, &g->timer1) != -1.0 || (g->net.isOnline && (g->net.isOnline != 1 || g->net.hHandle.joinable()))) {
				if (GetTimeLapse(151, &g->timer1) > g->skstruct.startinput_rank && GetTimeLapse(152, &g->timer1) == -1.0 && g->net.isOnline && g->net.hHandle.joinable()) {
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
				&& g->gameplay.replay.status != 2 && g->config.play.m_lunaris == 0) {

				if (g->KeyInput.p1_buttonInput[2] == 2 || g->KeyInput.p2_buttonInput[2] == 2) QuickRestart(*g, true);
				else QuickRestart(*g, false);

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

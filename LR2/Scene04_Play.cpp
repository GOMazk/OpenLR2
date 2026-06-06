#include "Scene04_Play.h"
#include "Scene08_Lunaris.h"
//drawing part
#include <math.h>
#define pi 3.1415926525
#define pi2 6.2831853
#define pi_half 1.570796

#include "Engine.h"
#include "LR2.h"

#ifndef _WIN32
#include <iostream>
#define GetMainWindowHandle() nullptr
static void MessageBoxA(const char*,const char* title,const char*desc,const char*)
{
	std::cout << "\n" << title << "\n\n" << desc << "\n" << std::flush;
}
#endif // _WIN32

static int PerformGAS(gameplay& gameplay, int playerIdx, CONFIG_PLAY& cfg) {
	PLAYERSTATUS& player = gameplay.player[playerIdx];
	if (playerIdx == 1 && gameplay.ghostBattle) return player.gaugeType;
	if (cfg.gaugeOption[playerIdx] == 5) return 5;
	constexpr std::array<int, 5> gaugeArr({ 4, 2, 1, 0, 3 });
	unsigned int i = 0;
	for (; i < gaugeArr.size(); i++)
		if (gaugeArr[i] == cfg.gaugeOption[playerIdx])
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

int ApplyJudgeNote(int judge, game *g, int _player, int lane, Timer *T, char isReplay) {

	if (g->gameplay.replay.status == 2) return 0; //playing replay
	if (g->gameplay.replay.status == 1 && isReplay == 0) { //recording replay, not playing replay??
		AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), _player * 2 + (lane >= 10) + 210, judge);
	}

	auto& player = g->gameplay.player[_player];
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
		if (_player == 0) {
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
		player.note_current++ ;
		player.note_current2++ ;
		if (player.note_current == player.totalnotes) SetTimeLapse(143 + _player, T); //final note timer

	}
	else {
		if (judge == 0) {
			if (isReplay == 0 && _player == 0) g->gameplay.p1Score.AddJudgeQueue(_player); //0
		}
	}

	player.recent_judge = judge;
	if (judge < 3) {
		if (g->gameplay.ghostBattle == 0 || _player == 0) {
			g->gameplay.lastMissTime = GetTimeWrap();
		}
		g->gameplay.misslayerTime[_player]= GetTimeWrap();
	}

	if (judge == 0) {
		player.judgecount[1]++;
		player.judgecount2[1]++;
	}
	else {
		player.judgecount[judge]++;
		player.judgecount2[judge]++;
	}

	int oldHp = ((int)player.HP[player.gaugeType] / 2) * 2;
	for (int gauge = 0; gauge < 6; gauge++) {
		int hp = ((int)player.HP[gauge] / 2) * 2;
		double damage;
		if (hp <= 30 && (!(gauge == 0 || gauge == 3) || g->gameplay.isCourse != 0) && judge <= 2) {
			damage = player.judge_damage[gauge][judge] * 0.6;
		}
		else {
			damage = player.judge_damage[gauge][judge];
		}
		player.HP[gauge] += damage;

		if (g->gameplay.isCourse == 0 && player.HP[gauge] <= 2.0 && (gauge == 0 || gauge == 3)) {
			player.HP[gauge] = 2.0;
		}

		if (player.HP[gauge] >= 100.0) player.HP[gauge] = 100.0;
		if (player.HP[gauge] < 0.0) player.HP[gauge] = 0.0;

		if (hp <= 0) player.HP[gauge] = 0; //prevents revive after death
	}
	if (g->config.play.m_gas)
		player.gaugeType = PerformGAS(g->gameplay, _player, g->config.play);

	int newHp = ((int)player.HP[player.gaugeType] / 2) * 2;
	if (oldHp <= 0) {
		newHp = 0;
		ResetTimeLapse(44 + _player, T);
	}
	else if (newHp != 100)
		ResetTimeLapse(44 + _player, T);

	if (oldHp < newHp) {
		if (newHp == 100) {
			ResetTimeLapse(42 + _player, T);
			SetTimeLapse(44 + _player, T);
		}
		else {
			SetTimeLapse(42 + _player, T);
		}
	}

	if (player.totalnotes > 0) {
		player.score = (player.judgecount[3] + (player.judgecount[4] + player.judgecount[5] * 2) * 2) * 50000 / player.totalnotes;
	}
	player.exscore = player.judgecount[4] + player.judgecount[5] * 2;

	if (player.note_current > 0) player.rate = player.exscore * 100 / (double)(player.note_current * 2);

	switch (judge) {
		case 1:
		case 2:
			player.now_combo = 0;
			player.now_combo_course = 0;
			break;
		case 3:
		case 4:
		case 5:
			player.now_combo++;
			if (player.now_combo > player.max_combo) player.max_combo = player.now_combo;
			player.now_combo_course++;
			if (player.now_combo_course > player.max_combo_course) player.max_combo_course = player.now_combo_course;
			break;
	}

	if (player.now_combo == player.totalnotes) SetTimeLapse(48 + _player, T); //fullcombo timer

	if (lane >= 10) {
		if (_player == 0) {
			g->gameplay.player[1].judge_draw = g->gameplay.player[0].recent_judge;
			g->gameplay.player[1].combo_song_draw = g->gameplay.player[0].now_combo;
			g->gameplay.player[1].combo_draw = g->gameplay.player[0].now_combo_course;
			SetTimeLapse(47, T);
			return 1;
		}
	}
	else if (_player == 0) {
		g->gameplay.player[0].judge_draw = g->gameplay.player[0].recent_judge;
		g->gameplay.player[0].combo_song_draw = g->gameplay.player[0].now_combo;
		g->gameplay.player[0].combo_draw = g->gameplay.player[0].now_combo_course;
		SetTimeLapse(46, T);
		return 1;
	}
	if (_player == 1 && lane >= 10) {
		g->gameplay.player[1].judge_draw = g->gameplay.player[1].recent_judge;
		g->gameplay.player[1].combo_song_draw = g->gameplay.player[1].now_combo;
		g->gameplay.player[1].combo_draw = g->gameplay.player[1].now_combo_course;
		SetTimeLapse(47, T);
	}
	return 1;
}

int ApplyJudgeMine(int judge, game *g, int _player, int lane, int damage) {

	if (g->gameplay.replay.status == 2) return 0; //playing replay
	if (g->gameplay.replay.status == 1) {
		AddReplayData(&g->gameplay.replay, GetTimeLapse(41, &g->timer1), _player*2 + (lane>=10) + 214, damage); //214 = 0xD6
	}

	auto& player = g->gameplay.player[_player];
	if (judge >= 0) {
		if (judge == 0) player.judgecount[1]++;
		else player.judgecount[judge]++;
	}
	player.judgecount[judge]++;

	for (int gauge = 0; gauge < 6; gauge++) {
		player.HP[gauge] -= damage;

		if (player.HP[gauge] <= 2.0 && (gauge == 0 || gauge == 3)) {
			player.HP[gauge] = 2.0;
		}
		if (player.HP[gauge] >= 100.0) {
			player.HP[gauge] = 100.0;
		}
		if (player.HP[gauge] < 0.0) {
			player.HP[gauge] = 0.0;
		}
	}
	if (g->config.play.m_gas)
		player.gaugeType = PerformGAS(g->gameplay, _player, g->config.play);

	if ((int)player.HP[player.gaugeType] / 2 != 50) ResetTimeLapse(44 + _player, &g->timer1);

	if (lane >= 10) {
		if (_player == 0) {
			g->gameplay.player[1].judge_draw = g->gameplay.player[0].recent_judge;
			g->gameplay.player[1].combo_song_draw = g->gameplay.player[0].now_combo;
			g->gameplay.player[1].combo_draw = g->gameplay.player[0].now_combo_course;
		}
		else if (_player == 1) {
			g->gameplay.player[1].judge_draw = g->gameplay.player[1].recent_judge;
			g->gameplay.player[1].combo_song_draw = g->gameplay.player[1].now_combo;
			g->gameplay.player[1].combo_draw = g->gameplay.player[1].now_combo_course;
		}

		SetTimeLapse(47, &g->timer1);
	}
	else if (_player == 0) {
		g->gameplay.player[0].judge_draw = g->gameplay.player[0].recent_judge;
		g->gameplay.player[0].combo_song_draw = g->gameplay.player[0].now_combo;
		g->gameplay.player[0].combo_draw = g->gameplay.player[0].now_combo_course;

		SetTimeLapse(46, &g->timer1);
	}

	return 1;
}


int DrawNotes(game *g, skstruct *sk, Timer *T, CONFIG_PLAY *cfg) {
	//TODO : refactor, test maniac mode
	DSTdraw tDdraw;

	SoundGetCurrentTime(&g->audio, &g->gameplay.muon);
	NONE_004b6770();
	int drawStartHeight = 0;

	if (sk->dst_NOTE[1].dstCount > 0) {
		if (sk->horizontal == 0) {
			drawStartHeight = -( abs(g->skstruct.adjust.size_y) + (int)sk->dst_NOTE[1].draw->h );
		}
		else {
			drawStartHeight = -( abs(g->skstruct.adjust.size_x) + (int)sk->dst_NOTE[1].draw->w );
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
	for (int i = g->gameplay.bmsobj_line.draw_count; i < g->gameplay.bmsobj_line.count; i++) {
		
		if (lineCount == 300) break; //max 300 measure_lines on screen
		lineCount++;

		int speed = cfg->hiSpeed[0];
		if (cfg->hiSpeed[1] < cfg->hiSpeed[0] && cfg->battle == 1) {
			speed = cfg->hiSpeed[1];
		}
		
		if (sk->dst_LINE[0].draw == nullptr) break; // Empty skin
		if ((sk->horizontal == 0 && sk->dst_LINE[0].draw->y + (songtimer - g->gameplay.bmsobj_line.notes[i].bmsTiming)* speed * g->gameplay.speedmultiplier * (cfg->basespeed / 100.0) / 600.0 > drawStartHeight)
			|| (sk->horizontal == 1 && (g->gameplay.bmsobj_line.notes[i].bmsTiming - songtimer)* speed * g->gameplay.speedmultiplier * (cfg->basespeed / 100.0) / 640.0 > drawStartHeight)) { 
						
			float p1_y = sk->adjust.note_1p_y + (songtimer - g->gameplay.bmsobj_line.notes[i].bmsTiming)* cfg->hiSpeed[0] * g->gameplay.speedmultiplier * (cfg->basespeed / 100.0) / 600.0;
			float p2_y = sk->adjust.note_2p_y + (songtimer - g->gameplay.bmsobj_line.notes[i].bmsTiming)* cfg->hiSpeed[0] * g->gameplay.speedmultiplier * (cfg->basespeed / 100.0) / 600.0;
			//if (p2_y > 0.0) p2_y = 0; //TOFIX : delete these for skinadjust
			//if (p1_y > 0.0) p1_y = 0.0;

			p2_y += g->gameplay.nabeatsu_y; //TOFIX : 1p 2p doesn't match (no nabeatsu_y on battle 2p measure_line). move it to below
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

		for (int i = g->gameplay.bmsobj_note[key].draw_count; i < g->gameplay.bmsobj_note[key].count; i++) {
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
					pos_y = -note_y / sk->dst_NOTE[key].draw->y;
				}
				float pos_yl = 0.0;
				if (sk->dst_NOTE[key].draw->y != 0.0) {
					pos_yl = -noteL_y / sk->dst_NOTE[key].draw->y;
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
				
				if (key < 10) {
					note_x = sk->adjust.note_1p_x + g->gameplay.nabeatsu_x + 0.0;
					note_y = sk->adjust.note_1p_y + g->gameplay.nabeatsu_y + note_y;
					noteL_y = sk->adjust.note_1p_y + g->gameplay.nabeatsu_y + noteL_y;
				}
				else {
					note_x = sk->adjust.note_2p_x + g->gameplay.nabeatsu_x + 0.0;
					note_y = sk->adjust.note_2p_y + g->gameplay.nabeatsu_y + note_y;
					noteL_y = sk->adjust.note_2p_y + g->gameplay.nabeatsu_y + noteL_y;
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
					if (g->gameplay.bmsobj_note[key].notes[i].mine > 0) {
						AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_MINE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
					}
					else if (g->gameplay.bmsobj_note[key].notes[i].realTiming_ln <= g->gameplay.bmsobj_note[key].notes[i].realTiming || g->gameplay.bmsobj_note[key].notes[i].realTiming_ln < 0.0) {
						AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_NOTE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
					}
					else {
						AddDrawingBuffer_LN(&sk->drBuf, &sk->src_LN_START[key], &sk->src_LN_END[key], &sk->src_LN_BODY[key], &sk->dst_NOTE[key], T, note_x, note_y, noteL_y, notesize_x, notesize_y, g->gameplay.bmsobj_note[key].notes[i].active);
					}

					if ((g->gameplay.bmsobj_note[key].notes[i].active == -1 || (g->gameplay.player[0].totalnotes <= g->gameplay.player[0].note_current && g->gameplay.replay.status == 2)) && g->gameplay.bmsobj_note[key].notes[i].bmsTiming <= songtimer && g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln <= songtimer) {
						g->gameplay.bmsobj_note[key].draw_count++;
					}
				}
				else {
					bool isAuto = g->gameplay.bmsobj_note[key].autoplay;
					bool isLnActive = g->gameplay.bmsobj_note[key].notes[i].bmsTiming <= songtimer && g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln > songtimer;

					if (isAuto && !isDpGbattle) {
						if (g->gameplay.bmsobj_note[key].notes[i].mine > 0) {
							AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_AUTO_MINE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
						}
						else if ((g->gameplay.bmsobj_note[key].notes[i].realTiming_ln <= g->gameplay.bmsobj_note[key].notes[i].realTiming || g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln <= g->gameplay.bmsobj_note[key].notes[i].bmsTiming || g->gameplay.bmsobj_note[key].notes[i].realTiming_ln < 0.0)) {
							AddDrawingBuffer_PlayArea(&sk->drBuf, &sk->src_AUTO_NOTE[key], &sk->dst_NOTE[key], T, note_x, note_y, 255, notesize_x, notesize_y, 1);
						}
						else {
							AddDrawingBuffer_LN(&sk->drBuf, &sk->src_AUTO_LN_START[key], &sk->src_AUTO_LN_END[key], &sk->src_AUTO_LN_BODY[key], &sk->dst_NOTE[key], T, note_x, note_y, noteL_y, notesize_x, notesize_y, isLnActive ? 5 : 0);
						}

						if (g->gameplay.bmsobj_note[key].notes[i].bmsTiming <= songtimer && g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln <= songtimer) {
							if (g->gameplay.bmsobj_note[key].notes[i].mine <= 0) {
								int p = 0;
								if (cfg->battle == 1) p = key / 10;

								g->gameplay.player[p].note_current++;
								g->gameplay.player[p].note_current2++;
							}
							g->gameplay.bmsobj_note[key].draw_count++;
						}
					}
					else {
						if (g->gameplay.bmsobj_note[key].notes[i].mine > 0){
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
								AddDrawingBuffer_LN(&sk->drBuf, &sk->src_AUTO_LN_START[key], &sk->src_AUTO_LN_END[key], &sk->src_AUTO_LN_BODY[key], &sk->dst_NOTE[key], T, note_x, note_y, noteL_y, notesize_x, notesize_y, isLnActive ? 5 : 0);
							}
							else {
								AddDrawingBuffer_LN(&sk->drBuf, &sk->src_LN_START[key], &sk->src_LN_END[key], &sk->src_LN_BODY[key], &sk->dst_NOTE[key], T, note_x, note_y, noteL_y, notesize_x, notesize_y, isLnActive ? 5 : 0);
							}

							if (g->gameplay.bmsobj_note[key].notes[i].bmsTiming <= songtimer && g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln >= songtimer && GetTimeLapse(70 + key, T) == -1.0) {
								ResetTimeLapse(50 + key, T);
								SetTimeLapse(70 + key, T);
								SetTimeLapse(100 + key, T);
								ResetTimeLapse(120 + key, T);
							}
						}
						

						if (g->gameplay.bmsobj_note[key].notes[i].bmsTiming <= songtimer && g->gameplay.bmsobj_note[key].notes[i].bmsTiming_ln <= songtimer) {
							if (g->gameplay.bmsobj_note[key].notes[i].mine <= 0) {
								SetTimeLapse(50 + key, T);
								ResetTimeLapse(70 + key, T);

								if (GetTimeLapse(100 + key, T) == -1.0) {
									SetTimeLapse(100 + key, T);
									ResetTimeLapse(120 + key, T);
								}

								if (isDpGbattle) {
									do {
										ApplyJudgeNote(g->gameplay.targetScore.GetJudgeFromQueue(), g, 1, key, T, 0);
									} while (g->gameplay.targetScore.DealJudgeFromQueue() == 0);
									g->gameplay.bmsobj_note[key].draw_count++;
								}
								else if (cfg->battle == 1 || (g->skinData.select > 11 && g->procSelecter == 7)) {
									if (g->net.rankingData.target_ID <= 0 || g->gameplay.targetScore.ghostReadCount <= 0) {
										ApplyJudgeNote(5, g, key / 10, key, T, 0);
										g->gameplay.bmsobj_note[key].draw_count++;
									}
									else {
										do {
											ApplyJudgeNote(g->gameplay.targetScore.GetJudgeFromQueue(), g, key/10, key, T, 0);
										} while (g->gameplay.targetScore.DealJudgeFromQueue() == 0);
										g->gameplay.bmsobj_note[key].draw_count++;
									}
								}
								else {
									if (g->net.rankingData.target_ID <= 0 || g->gameplay.targetScore.ghostReadCount <= 0) {
										ApplyJudgeNote(5, g, 0, key, T, 0);
										g->gameplay.bmsobj_note[key].draw_count++;
									}
									else {
										do {
											ApplyJudgeNote(g->gameplay.targetScore.GetJudgeFromQueue(), g, 0, key, T, 0);
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

int DrawHPgauge(game *g){
	char survival;

	for (int i = 0; i < 2; i++) {
		if (g->gameplay.isCourse == 0 && (g->gameplay.player[i].gaugeType == 0 || g->gameplay.player[i].gaugeType == 3)) {
			survival = 0;
		}
		else {
			survival = 1;
		}
		AddDrawingBuffer_Gauge(&g->skstruct.drBuf, &g->skstruct.src_GROOVEGAUGE[i], &g->skstruct.dst_GROOVEGAUGE[i], &g->timer1, g->gameplay.player[i].HP_print / 2, survival);
	}
	return 1;
}





int JudgeToScore(int judge, game *g, int player, int lane, char isReplay) {
	return ApplyJudgeNote(judge, g, player, lane, &g->timer1, isReplay);
}

int ProcSinglenote(game *g, int lane, int keypress, int timing, int player) {
	NoteStruct &note = g->gameplay.bmsobj_note[lane].notes[g->gameplay.bmsobj_note[lane].note_count];
	EXTENDEDPLAYERSTATS& extendedStats = g->gameplay.player[player].extendedStats;
	EXTENDEDPLAYERSTATS& extendedColumnStats = g->gameplay.player[player].extendedColumnStats[lane];
	EXTENDEDPLAYERSTATS& extendedStatsCourse = g->gameplay.player[player].extendedStatsCourse;
	EXTENDEDPLAYERSTATS& extendedColumnStatsCourse = g->gameplay.player[player].extendedColumnStatsCourse[lane];
	int& lastOffsetColumnIdx = g->gameplay.player[player].lastJudgedColumnIdx;
	if (note.mine > 0) {
		if (keypress == 1 && note.realTiming - (double)timing < (double)g->gameplay.player[player].judgetime[4]) {
			ApplyJudgeMine(0, g, player, lane, note.mine);

			if (g->gameplay.keysound[0].load)
				PlaySound(&g->audio, &g->gameplay.keysound[0], g->audio.chnKey, -1);
			else
				PlaySound(&g->audio, &g->audio.sysSound.mine, g->audio.chnKey, -1);
		}
		else if ((double)timing <= note.realTiming)
			return 0;
		else if (keypress == 1 || keypress == 2) {
			ApplyJudgeMine(0, g, player, lane, note.mine);

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
			ApplyJudgeNote(1, g, player, lane, &g->timer1, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			note.active = -1;
			g->gameplay.bmsobj_note[lane].note_count++;
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats) {
				stats.lpr++;
				stats.cb++;
				stats.noteCount++;
				stats.lastFastSlow = 2;
			};
			increment_extended(extendedStats);
			increment_extended(extendedColumnStats);
			increment_extended(extendedStatsCourse);
			increment_extended(extendedColumnStatsCourse);
			return 0;
		}
		if (keypress != 1) return 0;

		int offset = timing - (int)note.realTiming;
		int gap = std::abs(offset);
		bool isFast = offset < 0;

		if (gap <= g->gameplay.player[player].judgetime[5] && g->gameplay.player[player].note_current < g->gameplay.player[player].totalnotes) {
			g->gameplay.autojudge_midcount++;
			g->gameplay.autojudge_midsum += timing - (int)note.realTiming;
			JudgeToScore(5, g, player, lane, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = -1;
			g->gameplay.bmsobj_note[lane].note_count++;
			SetTimeLapse(50 + lane, &g->timer1);
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, bool isFast, int offset) {
				isFast ? stats.epg++ : stats.lpg++;
				stats.lastFastSlow = 0;
				stats.noteCount++;
				stats.lastHitOffset = offset;
			};
			increment_extended(extendedStats, isFast, offset);
			increment_extended(extendedColumnStats, isFast, offset);
			increment_extended(extendedStatsCourse, isFast, offset);
			increment_extended(extendedColumnStatsCourse, isFast, offset);
			lastOffsetColumnIdx = lane;
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
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, bool isFast, int offset) {
				isFast ? stats.egr++ : stats.lgr++;
				isFast ? stats.fast++ : stats.slow++;
				stats.lastFastSlow = isFast ? 1 : 2;
				stats.noteCount++;
				stats.lastHitOffset = offset;
			};
			increment_extended(extendedStats, isFast, offset);
			increment_extended(extendedColumnStats, isFast, offset);
			increment_extended(extendedStatsCourse, isFast, offset);
			increment_extended(extendedColumnStatsCourse, isFast, offset);
			lastOffsetColumnIdx = lane;
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
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, bool isFast, int offset) {
				isFast ? stats.egd++ : stats.lgd++;
				isFast ? stats.fast++ : stats.slow++;
				stats.lastFastSlow = isFast ? 1 : 2;
				stats.noteCount++;
				stats.lastHitOffset = offset;
			};
			increment_extended(extendedStats, isFast, offset);
			increment_extended(extendedColumnStats, isFast, offset);
			increment_extended(extendedStatsCourse, isFast, offset);
			increment_extended(extendedColumnStatsCourse, isFast, offset);
			lastOffsetColumnIdx = lane;
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
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, bool isFast, int offset) {
				isFast ? stats.ebd++ : stats.lbd++;
				isFast ? stats.fast++ : stats.slow++;
				stats.lastFastSlow = isFast ? 1 : 2;
				stats.noteCount++;
				stats.lastHitOffset = offset;
			};
			increment_extended(extendedStats, isFast, offset);
			increment_extended(extendedColumnStats, isFast, offset);
			increment_extended(extendedStatsCourse, isFast, offset);
			increment_extended(extendedColumnStatsCourse, isFast, offset);
			lastOffsetColumnIdx = lane;

			if (g->gameplay.bmsobj_note[lane].note_count < g->gameplay.bmsobj_note[lane].size && abs(timing - (int)g->gameplay.bmsobj_note[lane].notes[g->gameplay.bmsobj_note[lane].note_count].realTiming) <= g->gameplay.player[player].judgetime[2]) {
				ProcSinglenote(g, lane, 1, timing, player);
				return 1;
			}
			return 1;
		}
		if ((int)note.realTiming - timing < g->gameplay.player[player].judgetime[1] && g->gameplay.player[player].note_current < g->gameplay.player[player].totalnotes) {
			JudgeToScore(0, g, player, lane, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats) {
				stats.epr++;
				stats.lastFastSlow = 1;
			};
			increment_extended(extendedStats);
			increment_extended(extendedColumnStats);
			increment_extended(extendedStatsCourse);
			increment_extended(extendedColumnStatsCourse);
			return 1;
		}

		if(0 < g->gameplay.bmsobj_note[lane].noteVal && g->gameplay.bmsobj_note[lane].noteVal < SLOTS)
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
		return 1;
	}
}

int ProcLongnote(game *g, int lane, int keypress, int timing, int player) {
	
	NoteStruct &note = g->gameplay.bmsobj_note[lane].notes[g->gameplay.bmsobj_note[lane].note_count];
	EXTENDEDPLAYERSTATS& extendedStats = g->gameplay.player[player].extendedStats;
	EXTENDEDPLAYERSTATS& extendedColumnStats = g->gameplay.player[player].extendedColumnStats[lane];
	EXTENDEDPLAYERSTATS& extendedStatsCourse = g->gameplay.player[player].extendedStatsCourse;
	EXTENDEDPLAYERSTATS& extendedColumnStatsCourse = g->gameplay.player[player].extendedColumnStatsCourse[lane];
	int& lastOffsetColumnIdx = g->gameplay.player[player].lastJudgedColumnIdx;

	if ((double)timing - note.realTiming > (double)g->gameplay.player[player].judgetime[3] && note.active <= 0) {
		ApplyJudgeNote(1, g, player, lane, &g->timer1, 0);
		g->gameplay.bmsobj_note[lane].noteVal = note.val;
		note.active = -1;
		g->gameplay.bmsobj_note[lane].note_count++;
		ResetTimeLapse(70 + lane, &g->timer1);
		auto increment_extended = [](EXTENDEDPLAYERSTATS& stats) {
			stats.lpr++;
			stats.cb++;
			stats.noteCount++;
			stats.lastFastSlow = 2;
		};
		increment_extended(extendedStats);
		increment_extended(extendedColumnStats);
		increment_extended(extendedStatsCourse);
		increment_extended(extendedColumnStatsCourse);
		return 0;
	}

	if (keypress == 1) {
		int offset = timing - (int)note.realTiming;
		int gap = std::abs(offset);
		bool isFast = offset < 0;

		if (gap <= g->gameplay.player[player].judgetime[5]) {
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = 5;
			note.lnHeadFast = isFast;
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, int offset) {
				stats.lastFastSlow = 0;
				stats.lastHitOffset = offset;
			};
			increment_extended(extendedStats, offset);
			increment_extended(extendedColumnStats, offset);
			increment_extended(extendedStatsCourse, offset);
			increment_extended(extendedColumnStatsCourse, offset);
			lastOffsetColumnIdx = lane;

			SetTimeLapse(70 + lane, &g->timer1);
			return 1;
		}
		else if (gap <= g->gameplay.player[player].judgetime[4]) {
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = 4;
			note.lnHeadFast = isFast;
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, int offset, bool isFast) {
				isFast ? stats.fast++ : stats.slow++;
				stats.lastFastSlow = isFast ? 1 : 2;
				stats.lastHitOffset = offset;
			};
			increment_extended(extendedStats, offset, isFast);
			increment_extended(extendedColumnStats, offset, isFast);
			increment_extended(extendedStatsCourse, offset, isFast);
			increment_extended(extendedColumnStatsCourse, offset, isFast);
			lastOffsetColumnIdx = lane;

			SetTimeLapse(70 + lane, &g->timer1);
			return 1;
		}
		else if (gap <= g->gameplay.player[player].judgetime[3]) {
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = 3;
			note.lnHeadFast = isFast;
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, int offset, bool isFast) {
				isFast ? stats.fast++ : stats.slow++;
				stats.lastFastSlow = isFast ? 1 : 2;
				stats.lastHitOffset = offset;
			};
			increment_extended(extendedStats, offset, isFast);
			increment_extended(extendedColumnStats, offset, isFast);
			increment_extended(extendedStatsCourse, offset, isFast);
			increment_extended(extendedColumnStatsCourse, offset, isFast);
			lastOffsetColumnIdx = lane;

			SetTimeLapse(70 + lane, &g->timer1);
			return 1;
		}
		else if (gap <= g->gameplay.player[player].judgetime[2]) {
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			note.active = 2;
			note.lnHeadFast = isFast;
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, int offset, bool isFast) {
				isFast ? stats.fast++ : stats.slow++;
				stats.lastFastSlow = isFast ? 1 : 2;
				stats.lastHitOffset = offset;
			};
			increment_extended(extendedStats, offset, isFast);
			increment_extended(extendedColumnStats, offset, isFast);
			increment_extended(extendedStatsCourse, offset, isFast);
			increment_extended(extendedColumnStatsCourse, offset, isFast);
			lastOffsetColumnIdx = lane;
			return 1;
		}
		else if ((int)note.realTiming - timing < g->gameplay.player[player].judgetime[1]) {
			JudgeToScore(0, g, player, lane, 0);
			g->gameplay.bmsobj_note[lane].noteVal = note.val;
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats) {
				stats.epr++;
				stats.lastFastSlow = 1;
			};
			increment_extended(extendedStats);
			increment_extended(extendedColumnStats);
			increment_extended(extendedStatsCourse);
			increment_extended(extendedColumnStatsCourse);
			return 1;
		}

		if (0 < g->gameplay.bmsobj_note[lane].noteVal && g->gameplay.bmsobj_note[lane].noteVal < SLOTS)
			PlaySound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal], g->audio.chnStageKey[note.stage], note.stage);

		return 1;
	}

	else if (keypress == 2) {
		if ((int)note.realTiming_ln < timing && note.active > 0) {
			bool& isFast = note.lnHeadFast;
			if (note.active < 4) {
				ResetTimeLapse(70 + lane, &g->timer1);
			}
			else {
				ResetTimeLapse(70 + lane, &g->timer1);
				SetTimeLapse(50 + lane, &g->timer1);
			}
			JudgeToScore(note.active, g, player, lane, 0);
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, bool isFast, int judge) {
				switch (judge) {
				case 5: isFast ? stats.epg++ : stats.lpg++; break;
				case 4: isFast ? stats.egr++ : stats.lgr++; break;
				case 3: isFast ? stats.egd++ : stats.lgd++; break;
				case 2: isFast ? stats.ebd++ : stats.lbd++; stats.cb++; break;
				}
				stats.noteCount++;
			};
			increment_extended(extendedStats, isFast, note.active);
			increment_extended(extendedColumnStats, isFast, note.active);
			increment_extended(extendedStatsCourse, isFast, note.active);
			increment_extended(extendedColumnStatsCourse, isFast, note.active);
			note.active = -1;
			g->gameplay.bmsobj_note[lane].note_count++;
			return 1;
		}
	}
	else if (keypress == 3 && note.active > 0) {
		bool& isFast = note.lnHeadFast;
		if (g->gameplay.player[player].judgetime[3] + timing < (int)note.realTiming_ln) {
			StopSound(&g->audio, &g->gameplay.keysound[g->gameplay.bmsobj_note[lane].noteVal]);
			ResetTimeLapse(70 + lane, &g->timer1);
			JudgeToScore(2, g, player, lane, 0);
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, bool isFast) {
				isFast ? stats.ebd++ : stats.lbd++;
				stats.cb++;
				stats.noteCount++;
			};
			increment_extended(extendedStats, isFast);
			increment_extended(extendedColumnStats, isFast);
			increment_extended(extendedStatsCourse, isFast);
			increment_extended(extendedColumnStatsCourse, isFast);
		}
		else {
			ResetTimeLapse(70 + lane, &g->timer1);
			JudgeToScore(note.active, g, player, lane, 0);
			auto increment_extended = [](EXTENDEDPLAYERSTATS& stats, bool isFast, int judge) {
				switch (judge) {
				case 5: isFast ? stats.epg++ : stats.lpg++; break;
				case 4: isFast ? stats.egr++ : stats.lgr++; break;
				case 3: isFast ? stats.egd++ : stats.lgd++; break;
				case 2: isFast ? stats.ebd++ : stats.lbd++; stats.cb++; break;
				}
				stats.noteCount++;
			};
			increment_extended(extendedStats, isFast, note.active);
			increment_extended(extendedColumnStats, isFast, note.active);
			increment_extended(extendedStatsCourse, isFast, note.active);
			increment_extended(extendedColumnStatsCourse, isFast, note.active);
		}
		note.active = -1;
		g->gameplay.bmsobj_note[lane].note_count++;
	}

	return 1;
}

int ProcNoteOnTiming(game *g, int lane, int keypress, int timing, int player) {

	NoteStruct &note = g->gameplay.bmsobj_note[lane].notes[g->gameplay.bmsobj_note[lane].note_count];

	if (!(0 <= note.stage && note.stage < 5)) note.stage = 0;
	
	if (g->gameplay.bmsobj_note[lane].count == 0 || g->gameplay.bmsobj_note[lane].autoplay != 0) return 0;

	if (g->gameplay.bmsobj_note[lane].note_count == g->gameplay.bmsobj_note[lane].count && g->gameplay.bmsobj_note[lane].noteVal > 0) {
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

	if( ((g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 2 || (g->KeyInput.mouse_buttonR == 2 && g->config.play.disableleftclickexit == 0) ) 
			|| (g->KeyInput.p1_buttonInput[13] == 2 && g->KeyInput.p1_buttonInput[12] == 2)
			|| (g->KeyInput.p2_buttonInput[13] == 2 && g->KeyInput.p2_buttonInput[12] == 2)
			|| (g->gameplay.player[0].totalnotes <= g->gameplay.player[0].note_current && (g->KeyInput.p1_buttonInput[13] == 2 || g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2 || g->KeyInput.p2_buttonInput[12] == 2))) 
		&& g->procPhase == 1){
		SetTimeLapse(2, &g->timer1);
		g->procPhase = 2;
		g->gameplay.flag_closingPhase = 1;
		return 1;
	}

	auto proc_quickrestart = [](game& game) {
		if (game.gameplay.replay.status == 2 || game.gameplay.player[0].totalnotes <= game.gameplay.player[0].note_current || game.config.play.m_lunaris != 0) return;
		if (game.KeyInput.p1_buttonInput[12] == 1 || game.KeyInput.p2_buttonInput[12] == 1) return QuickRestart(game, true);
		if (game.KeyInput.p1_buttonInput[13] == 1 || game.KeyInput.p2_buttonInput[13] == 1) return QuickRestart(game, false);
	};

	if (g->procPhase == 2) {
		timeLimit = g->skstruct.fadeout;
		gameTime = GetTimeLapse(2, &g->timer1);
		if (timeLimit < gameTime || timeLimit == 0) g->procSelecter = 5;
		proc_quickrestart(*g);
	}
	else if (g->procPhase == 3) {
		timeLimit = g->skstruct.close;
		gameTime = GetTimeLapse(3, &g->timer1);
		if (timeLimit < gameTime || timeLimit == 0) g->procSelecter = 5;
		proc_quickrestart(*g);
	}
	return 1;
}

#define POLLGAP 2
int ProcGame(game *g) {

	double oldt142;

	if (GetTimeWrap() < g->gameplay.timetick + POLLGAP) return 1; //pollrate 500 -> 1000
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

	//TEST:
	g->timer1.GAPtick++;
	if (t41 - g->timer1.GAPclock > POLLGAP) {
		g->timer1.GAPcount++;
		g->timer1.avgOnlyGAP = ((g->timer1.avgOnlyGAP * ((double)g->timer1.GAPcount - 1.0)) + (t41 - g->timer1.GAPclock) * 1.0) / (double)g->timer1.GAPcount;
	}
	g->timer1.maxGAP = (t41 - g->timer1.GAPclock > g->timer1.maxGAP) ? (t41 - g->timer1.GAPclock) : g->timer1.maxGAP;
	if (t41 > 0) g->timer1.avgGAP = t41 / (double)g->timer1.GAPtick;
	g->timer1.GAPclock = t41;
	//TEST END

	for (int i = 0; i < 5; i++) {
		if (g->gameplay.fadeinSOUNDstart[i] > 0) {
			if (g->gameplay.fadeinSOUNDend[i] > 0 && g->gameplay.fadeinSOUNDstart[i] - 100 <= t41 && t41 <= g->gameplay.fadeinSOUNDend[i] + 100) {
				double vol = ChangeValueByTime(0.0, 1.0, g->gameplay.fadeinSOUNDstart[i], g->gameplay.fadeinSOUNDend[i], t41, 0);
				g->audio.param.stageBgmVolume[i] = vol;
				g->audio.param.stageKeyVolume[i] = vol;
				fx = true;
			}
		}

		if (g->gameplay.fadeoutSOUNDstart[i] > 0) {
			if (g->gameplay.fadeoutSOUNDstart[i] > 0 && g->gameplay.fadeoutSOUNDend[i] > 0 && g->gameplay.fadeoutSOUNDstart[i] - 100 <= t41 && t41 <= g->gameplay.fadeoutSOUNDend[i] + 100) {
				double vol = ChangeValueByTime(1.0, 0.0, g->gameplay.fadeoutSOUNDstart[i], g->gameplay.fadeoutSOUNDend[i], t41, 0);
				g->audio.param.stageBgmVolume[i] = vol;
				g->audio.param.stageKeyVolume[i] = vol;
				fx = true;
			}
		}
	}
	if (fx) ApplySoundFX(&g->audio, 0, 0);

	for (int i = 0; i < 5; i++) {
		if (g->gameplay.fadeinBGAstart[i] > 0 && g->gameplay.fadeinBGAend[i] > 0 && g->gameplay.fadeinBGAstart[i] - 100 <= t41 && t41 <= g->gameplay.fadeinBGAend[i] + 100) {
			g->gameplay.bgaMixer[i] = ChangeValueByTime(0.0, 100.0, g->gameplay.fadeinBGAstart[i], g->gameplay.fadeinBGAend[i], t41, 0);
		}
		if (g->gameplay.fadeoutBGAstart[i] > 0 && g->gameplay.fadeoutBGAend[i] > 0 && g->gameplay.fadeoutBGAstart[i] - 100 <= t41 && t41 <= g->gameplay.fadeoutBGAend[i] + 100) {
			g->gameplay.bgaMixer[i] = ChangeValueByTime(100.0, 0.0, g->gameplay.fadeoutBGAstart[i], g->gameplay.fadeoutBGAend[i], t41, 0);
		}
	}
		
	while (true) {
		int gaugeType[2] = { g->gameplay.player[0].gaugeType, g->gameplay.player[1].gaugeType };
		if (g->gameplay.bmsobj.notes[g->gameplay.bmsobj.note_count].realTiming >= t142 || g->gameplay.bpmChangedBmstime >= 0) {
			SoundGetCurrentTime(&g->audio, &g->gameplay.muon); //anti cheat
			NONE_004b6770();
			t142 = GetTimeLapse(142, &g->timer1);
			if (g->gameplay.isPreviewLoad == '\0') {
				ReactInput(g);
			}

			if (g->is_starter || (g->procSelecter == 4 && g->procPhase != 2 && g->procPhase != 3)) {
				if ((g->gameplay.player[0].HP[gaugeType[0]] >= 2.0 || g->config.play.battle == 1) && (g->gameplay.player[0].HP[gaugeType[0]] >= 2.0 || g->gameplay.player[1].HP[gaugeType[1]] >= 2.0 || g->config.play.battle != 1) || g->gameplay.isPreviewLoad) {

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
						if (g->gameplay.player[p].HP_unk != g->gameplay.player[p].HP[gaugeType[p]]) {
							g->gameplay.player[p].HP_old = g->gameplay.player[p].HP_print;
							g->gameplay.player[p].time_oldHP = GetTimeWrap();
							g->gameplay.player[p].time_newHP = (int)GetTimeWrap() + abs((int)(g->gameplay.player[p].HP_old - g->gameplay.player[p].HP[gaugeType[p]])) * 10;
						}
						g->gameplay.player[p].HP_unk = g->gameplay.player[p].HP[gaugeType[p]];
						if (g->gameplay.player[p].score != g->gameplay.player[p].score_unk) {
							g->gameplay.player[p].score_old = g->gameplay.player[p].score_print;
							g->gameplay.player[p].time_oldScore = GetTimeWrap();
							g->gameplay.player[p].time_newScore = GetTimeWrap() + 500;
						}
						g->gameplay.player[p].score_unk = g->gameplay.player[p].score;

						g->gameplay.player[p].HP_print = ChangeValueByTime(g->gameplay.player[p].HP_old, g->gameplay.player[p].HP[gaugeType[p]], g->gameplay.player[p].time_oldHP, g->gameplay.player[p].time_newHP, GetTimeWrap(), 0);
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
					for (int i = 0; i < SLOTS; i++) {
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
		else if (op == 1001) { //NONSTOP MIX #RANK

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
				cstrSprintf(&tmp, "%s", g->sSelect.bmsList[g->sSelect.cur_song].courseTitle[stage].body);
			}
			else {
				cstrSprintf(&tmp, "%s", g->sSelect.bmsList[g->sSelect.cur_song].courseTitle[stage].body);
			}
			SetObjectString(10, tmp, g->txtStruct.objectStr);
		}

		g->gameplay.bmsobj.note_count++;

		if (g->gameplay.bmsobj.count <= g->gameplay.bmsobj.note_count && g->gameplay.replay.status != 2) {
			break;
		}
		if (g->gameplay.song_runtime < t142) {
			ErrorLogFmtAdd("endtime\n");
			break;
		}
		if (g->is_starter) {
			if (g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 2 || (g->KeyInput.p1_buttonInput[12] == 2 && g->KeyInput.p1_buttonInput[13] == 2)) {
				for (int i = 0; i < SLOTS; i++) {
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
		
		if ((g->gameplay.player[0].HP[gaugeType[0]] < 2.0 && g->config.play.battle != 1 && g->gameplay.ghostBattle == 0) || (g->gameplay.player[0].HP[gaugeType[0]] < 2.0 && g->gameplay.player[1].HP[gaugeType[1]] < 2.0 && (g->config.play.battle == 1 || g->gameplay.ghostBattle)) && (g->gameplay.isPreviewLoad == 0)) {
			SetTimeLapse(3, &g->timer1);
			g->procPhase = 3;
			for (int i = 0; i < SLOTS; i++) {
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
			int gaugeType[2] = { g->gameplay.player[0].gaugeType, g->gameplay.player[1].gaugeType };
			for (int p = 0; p < 2; p++) {
				if (g->gameplay.player[p].HP_unk != g->gameplay.player[p].HP[gaugeType[p]]) {
					g->gameplay.player[p].HP_old = g->gameplay.player[p].HP_print;
					g->gameplay.player[p].time_oldHP = GetTimeWrap();
					g->gameplay.player[p].time_newHP = (int)GetTimeWrap() + abs((int)(g->gameplay.player[p].HP_old - g->gameplay.player[p].HP[gaugeType[p]])) * 10;
				}
				g->gameplay.player[p].HP_unk = g->gameplay.player[p].HP[gaugeType[p]];
				if (g->gameplay.player[p].score != g->gameplay.player[p].score_unk) {
					g->gameplay.player[p].score_old = g->gameplay.player[p].score_print;
					g->gameplay.player[p].time_oldScore = GetTimeWrap();
					g->gameplay.player[p].time_newScore = GetTimeWrap() + 500;
				}
				g->gameplay.player[p].score_unk = g->gameplay.player[p].score;

				g->gameplay.player[p].HP_print = ChangeValueByTime(g->gameplay.player[p].HP_old, g->gameplay.player[p].HP[gaugeType[p]], g->gameplay.player[p].time_oldHP, g->gameplay.player[p].time_newHP, GetTimeWrap(), 0);
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
				if (g->gameplay.player[p].totalnotes > g->gameplay.player[p].note_current) {
					for (int gauge = 0; gauge < 6; gauge++)
						g->gameplay.player[p].HP[gauge] = 0;
				}
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
					else if (g->gameplay.player[0].note_current == g->gameplay.player[0].totalnotes && g->gameplay.player[0].HP[gaugeType[0]] >= 80.0) {
						//if(g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] < 10) //TOFIX: BP under 10 is considered as hard clear, but not working. now, do we want it? // it's starter mode or plugout4 code, don't mind it
						//	g->gameplay.player[0].clearType = 4;
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

void ProcGameThread(game *g) {

	g->gameplay.flag_threadExist = 1;
	g->gameplay.flag_closingPhase = 0;
	
	while (g->procPhase == 0 || GetTimeLapse(0,&g->timer1) < g->skstruct.loadstart) {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		if (g->gameplay.flag_closingPhase) {
			g->gameplay.flag_threadExist = 0;
			return;
		}
	}

	if (g->gameplay.flag_retry == 0 && g->config.system.isablebmsthread == 0) {
		std::jthread(ProcLoadBmsResource, g).detach();
	}
	else {
		g->gameplay.bmsResourceLoaded = 1;
	}

	while (GetTimeLapse(0, &g->timer1) < g->skstruct.loadstart + g->skstruct.loadend || g->gameplay.bmsResourceLoaded == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		if (g->gameplay.flag_closingPhase) {
			g->gameplay.flag_threadExist = 0;
			return;
		}
	}

	SetTimeLapse(40, &g->timer1);

	while (GetTimeLapse(40,&g->timer1) < g->skstruct.playstart 
		|| g->KeyInput.inputID[2] || g->KeyInput.inputID[3] || g->KeyInput.inputID[4] || g->KeyInput.inputID[5] || g->KeyInput.inputID[6] || g->KeyInput.inputID[7] || g->KeyInput.inputID[8] || g->KeyInput.inputID[9]	|| g->KeyInput.inputID[11] 
		|| g->KeyInput.p1_buttonInput[12] || g->KeyInput.p1_buttonInput[13] || g->KeyInput.p2_buttonInput[12] || g->KeyInput.p2_buttonInput[13]) {

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		ReactInput(g);
		if (g->gameplay.flag_closingPhase) {
			g->gameplay.flag_threadExist = 0;
			return;
		}
	}

	SetTimeLapse(41, &g->timer1);
	SetTimeLapse(142, &g->timer1);

	//TEST:
	g->timer1.GAPtick = 0;
	g->timer1.GAPclock = 0;
	g->timer1.maxGAP = 0;
	g->timer1.avgGAP = 0;
	g->timer1.GAPcount = 0;
	g->timer1.avgOnlyGAP = 0;
	//TEST END

	if (g->is_recordmode) {
		g->rec.RefreshCurFrame();
	}

	if (g->rec.recMode == 2) {
		ErrorLogFmtAdd("リプレイ録画なので、あらかじめBGMのみmixします\n");
		double recordnow = GetTimeWrap();
		
		//didn't check
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

	int gauge = g->gameplay.player[0].gaugeType;
	if (gauge == 1 || gauge == 2 || gauge == 4 || gauge == 5) {
		SetTimeLapse(44, &g->timer1);
	}
	gauge = g->gameplay.player[1].gaugeType;
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
}

int ProcS_Play(game *g, sqlite3* sql) {

	int iTemp;

	g->gameplay.hThreadPreview = {};
	g->gameplay.flag_closingPhase = 0;
	
	CSTR gData, gName;
	CSTR md5;
	if (g->gameplay.courseType == 0 || g->gameplay.courseType == 2) {
		md5 = g->sSelect.bmsList[g->sSelect.cur_song].courseHash[g->gameplay.courseStageNow];
	}
	else {
		md5 = g->sSelect.bmsList[g->sSelect.cur_song].hash;
	}

	if (g->net.rankingData.target_ID > 0 && g->net.isOnline) {
		g->gameplay.targetScore.InitJudgeQueue();
		g->net.WaitAndInitRanking();
		if ((g->gameplay.ghostBattle == 0 && g->gameplay.isAutoplay != 1) || g->gameplay.replay.status == 2) {
			g->net.GetTargetInfo(0, md5, &gData, &gName, &iTemp, &iTemp, &iTemp, &iTemp, &iTemp, &iTemp);
		}
		else {
			int origGauge = g->config.play.gaugeOption[0];
			//TOFIX : seed is not putted into replaydata, when use ghostbattle. (retry puts seed) (see also ParseBmsFile())
			if(g->sSelect.bmsList[g->sSelect.cur_song].keymode > 9)
				g->net.GetTargetInfo(0, md5, &gData, &gName, &g->config.play.gaugeOption[0], &g->config.play.random[0], &g->config.play.random[1], &g->config.play.dpflip, &g->gameplay.randomseed, &iTemp);
			else
				g->net.GetTargetInfo(0, md5, &gData, &gName, &g->config.play.gaugeOption[0], &g->config.play.random[0], &g->config.play.random[1], &iTemp, &g->gameplay.randomseed, &iTemp);
			
			g->config.play.gaugeOption[1] = g->config.play.gaugeOption[0];
			g->config.play.random[1] = g->config.play.random[0];

			if (g->config.play.m_gas) {
				g->config.play.gaugeOption[0] = origGauge;
			}
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
		if (g->sSelect.metaSelected.keymode == 10 && g->config.play.battle == 0 && g->skinData.Data[g->skinData.skinID[13]].type == SKINTYPE_7KEYSBATTLE) {
			scratchside = g->skstruct.scratchside_1 + g->skstruct.scratchside_2 * 2;
		}
		if (g->sSelect.metaSelected.keymode == 5 && g->config.play.battle == 1 && g->skinData.Data[g->skinData.skinID[13]].type == SKINTYPE_7KEYSBATTLE) {
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
				cstrSprintf(&g->directoryPath, "%smovie.avi", g->baseDirectory.body);
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
		g->timer1.flagMovieTimer = 1;
		g->timer1.movieFramerate = g->config.tools.movie_framerate;
		g->timer1.movieTimer = 0.0;
		if (g->is_recordmode) SetManualTimerFlag(&g->timer1, 1);
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

	if (g->net.rankingData.target_ID > 0 && g->net.isOnline) {
		if (gData.length() <= 0 || gData.isDiff("Z") == 0) {
			ErrorLogFmtAdd("ゴースト無しでターゲットを設定します ターゲット番号 %d ターゲット名 %s\n", g->net.rankingData.target_number, g->net.rankingData.ranking[g->net.rankingData.target_number].name.body);

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
	std::jthread(ProcGameThread, g).detach(); // removed SetThreadPriority(hG, -1);
	return 1;
}


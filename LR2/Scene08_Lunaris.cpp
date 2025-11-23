#include "Scene08_Lunaris.h"
#include "DxLib/DxLib.h"
#include "Engine.h"
#include "LR2.h"

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
		for (int i = 0; i < 3; i++) { //TOFIX: 1~3 rotate is same (lun_nowblockshape doesn't change)
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

	for (int i = 0; i < count; i++) { //TOFIX: 1~3 rotate is same (lun_nowblockshape doesn't change)
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

	if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1) {
		LUNARIS_MOVE(2);
		return 1;
	}
	if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1 || g->KeyInput.p1_buttonInput[7] == 1) {
		LUNARIS_MOVE(4);
		return 1;
	}
	if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1 || g->KeyInput.p1_buttonInput[2] == 1) {
		LUNARIS_MOVE(8);
		return 1;
	}
	if (g->KeyInput.inputID[KEY_INPUT_UP] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[6] == 1) {
		LUNARIS_ROTATE(0);
	}
	else if (g->KeyInput.p1_buttonInput[1] == 1) {
		LUNARIS_ROTATE(1);
		return 1;
	}

	return 1;
}
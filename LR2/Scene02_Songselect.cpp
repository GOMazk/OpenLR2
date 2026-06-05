#include "Scene02_Songselect.h"
#include "Engine.h"
#include "LR2.h"
#include <stdio.h>
#include "filesystem.h"
#ifndef _WIN32
#include "En_dxlibstub.h"
#endif // _WIN32

struct glb_dbgame {
	struct sqlite3 * pSql;
	struct game * pGame;
};

int SetTarget(game *g) {

	if (g->net.rankingData.showRanking == 1 && g->procSelecter == 2) {
		SetObjectString(1, g->sSelect.bmsList[g->sSelect.cur_song].title, g->txtStruct.objectStr);
		return 1;
	}

	if (g->sSelect.stack_query[g->sSelect.cur].findStrPos("__RIVAL__") >= 0 && g->procSelecter == 2) {
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

int SetBmsFilter(game *g, sqlite3 */*sql*/){

	g->sSelect.searchType = 0;
	g->sSelect.searchFocused = 1;
	g->sSelect.queryCount = 1;
	g->sSelect.curQuery[0] = g->sSelect.stack_query[g->sSelect.cur];
	g->sSelect.filterSort = g->config.select.sort;
	g->sSelect.searchMax = g->sSelect.stack_rivalID[g->sSelect.cur];
	g->sSelect.filterKey = g->config.select.key;
	g->sSelect.filterDifficulty = g->config.select.difficulty;
	int cur = g->sSelect.cur_song;
	SONGDATA *bms = g->sSelect.bmsList;
	g->sSelect.selTitle = bms[cur].title;
	g->sSelect.selFilepath = bms[cur].filepath;
	g->sSelect.selKey = bms[cur].keymode;
	g->sSelect.selFolder = bms[cur].folder;
	return 1;
}


int Print_ManiacOptions(game *g) {
	int* pOpVal;

	printfDx("MANIAC OPTIONS\n");
	printfDx("変態オプションの仮置き場です。\n");
	printfDx("トライアル改訂の際はこの手のオプションを多数取り入れる予定です。\n");
	printfDx("これらのオプションは、基本的にリプレイに反映されません。\n");
	printfDx("カーソルキーで選択・変更ができます。\n%03d/%03d\n\n\n",g->sSelect.maniac_cursor + 1, 22);

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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 3, -1, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 3, 1, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 3, -1, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 3, 1, pOpVal);
				}
				break;

			case 2:
				printfDx("EXTRA MODE LEVEL    ");
				pOpVal = &g->config.play.m_extra;
				if (*pOpVal == 0) printfDx("LEVEL 1");
				if (*pOpVal == 1) printfDx("LEVEL 2");
				if (*pOpVal == 2) printfDx("LEVEL 3");

				if (g->sSelect.maniac_cursor == 2) {
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 2, -1, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 2, 1, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 3, -1, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 3, 1, pOpVal);
				}
				break;
			case 7:
				printfDx("SOFTLANDING         ");
				pOpVal = &g->config.play.m_softlanding;
				if (*pOpVal == 0) printfDx("OFF");
				if (*pOpVal == 1) printfDx("LEVEL 1");
				if (*pOpVal == 2) printfDx("LEVEL 2");

				if (g->sSelect.maniac_cursor == 7) {
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 2, -1, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 2, 1, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 11:
				printfDx("GAMBOL              ");
				pOpVal = &g->config.play.m_gambol;
				if (*pOpVal == 0) printfDx("OFF");
				if (*pOpVal == 1) printfDx("LEVEL 1");
				if (*pOpVal == 2) printfDx("LEVEL 2");

				if (g->sSelect.maniac_cursor == 11) {
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 2, -1, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 2, 1, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
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
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 100, -10, pOpVal);
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 100, 10, pOpVal);
				}
				break;
			case 20:
				printfDx("LUNARIS             ");
				if (g->config.play.m_lunaris == 0) printfDx("OFF");
				if (g->config.play.m_lunaris == 1) printfDx("ON");

				if (g->sSelect.maniac_cursor == 20) {
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) g->config.play.m_lunaris = g->config.play.m_lunaris == 0;
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) g->config.play.m_lunaris = g->config.play.m_lunaris == 0;
				}
				break;
			case 21:
				printfDx("GAUGE AUTO SHIFT    ");
				if (g->config.play.m_gas == 0) printfDx("OFF");
				if (g->config.play.m_gas == 1) printfDx("ON");

				if (g->sSelect.maniac_cursor == 21) {
					if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) g->config.play.m_gas = g->config.play.m_gas == 0;
					if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) g->config.play.m_gas = g->config.play.m_gas == 0;
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
	case 21:
		printfDx("GAS automatically changes the gauge to the easier one during gameplay.\n");
		printfDx("Will not shift to the gauge harder than originally selected (Start with easy -> groove).\n");
		printfDx("If enabled, will save the score with the best survived gauge regardless of what's displayed.\n");
		break;
	}

	if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) {
		LoopInRange(0, 21, -1, &g->sSelect.maniac_cursor);
	}

	if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) {
		LoopInRange(0, 21, 1, &g->sSelect.maniac_cursor);
	}

	return 1;
}

int GetSongCursor(game *g) {
	if (g->cmd_directplay) return 0;
	
	return ((g->sSelect.listSelectedBarFromScreenTop - g->skstruct.BAR_CENTER)
		+ (g->sSelect.listCalculatedBar / 1000 + (g->sSelect.listCalculatedBar % 1000 && g->sSelect.scrollDirection == 2 ? 1 : 0)) 
		+ g->sSelect.bmsListCount * 30) 
		% g->sSelect.bmsListCount;
}

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



int ShowReadmes(game *g) {
#ifdef _WIN32
	CSTR search;
	HANDLE hFindFile;
	WIN32_FIND_DATAW FindFileData;
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
	hFindFile = FindFirstFileW(utf2ws(search.body).c_str(), &FindFileData);
	if (hFindFile == (HANDLE)-1) {
		ErrorLogFmtAdd("テキストファイルが見つからない。%s\n", search.body);
		return -1;
	}

	do {
		g->txtStruct.readme.file_count++;
	} while (FindNextFileW(hFindFile, &FindFileData));
	FindClose(hFindFile);

	if (g->txtStruct.readme.file_count == 0) return 0;

	if (g->txtStruct.readme.current >= g->txtStruct.readme.file_count) 
		g->txtStruct.readme.current = 0;
	else if(g->txtStruct.readme.current < 0)
		g->txtStruct.readme.current = g->txtStruct.readme.file_count-1;

	int currentFileNum = 0;
	hFindFile = FindFirstFileW(utf2ws(search.body).c_str(), &FindFileData);
	do {
		std::string filename = ws2utf(FindFileData.cFileName);
		g->txtStruct.readme.path = g->txtStruct.readme.folderpath;
		g->txtStruct.readme.path.add(filename.c_str());

		pFile = fopen(g->txtStruct.readme.path.body, "r");

		char *pFbuf = fBuf.outstr();
		if (pFile != NULL) {
			
			currentFileNum++;
			if (g->txtStruct.readme.file_count == 1) {
				g->txtStruct.readme.body[g->txtStruct.readme.lines] = filename.c_str();
			}
			else {
				std::string line = std::format("{}/{} {}", currentFileNum, g->txtStruct.readme.file_count, filename);
				g->txtStruct.readme.body[g->txtStruct.readme.lines] = line.c_str();
			}
			g->txtStruct.readme.lines+=2;

			g->txtStruct.readme.show = 1;
			pFbuf = fBuf.outstr();
			for (pFbuf = fgets(pFbuf, 2048, pFile); pFbuf; pFbuf = fgets(pFbuf, 2048, pFile)) {
				fBuf = ansi2utf(fBuf.body, 932).c_str();
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
	} while (FindNextFileW(hFindFile, &FindFileData));
	FindClose(hFindFile);

	g->txtStruct.readme.y = g->skstruct.src_README[0].op1 * g->txtStruct.readme.lines;
	return 1;
#else
	// TODO: reimplement with std::filesystem
	return {};
#endif // _WIN32
}

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

	pFile = fopen(g->txtStruct.readme.path, "r");

	char *pFbuf = fBuf.outstr();
	if (pFile != NULL) {
		g->txtStruct.readme.show = 1;
		pFbuf = fBuf.outstr();
		for (pFbuf = fgets(pFbuf, 0x3FC, pFile); pFbuf; pFbuf = fgets(pFbuf, 0x3FC, pFile)) {
			fBuf = ansi2utf(pFbuf, 932).c_str();
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

CSTR GetMissonString(int missionLevel, int line) {
	switch (missionLevel) {
		case 1:
			if (line == 0) 
				return "NOTES>=100";
			else
				return "DEATH";

		case 2:
			if (line == 0)
				return "NOTES>=100"; 
			else
				return "HIDDEN";

		case 3:
			if (line == 0) return "NOTES>=100";
			else return "SUDDEN";

		case 4:
			if (line == 0) 
				return "GROOVE, GAUGE80%-84%";
			else
				return "";

		case 5:
			if (line == 0)
				return "NOTES>=100"; 
			else
				return "S-RANDOM, SURVIVAL";

		case 6:
			if (line == 0)
				return "NOTES>=100, ";
			else
				return "FREQ/SPEEDFX>=+3";

		case 7:
			if (line == 0)
				return "NOTES>=100"; 
			else
				return "CONVERGE";

		case 8:
			if (line == 0)
				return "NOTES>=100";
			else
				return "HID+SUD";

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
			if (line == 0)
				return "NOTES>=100";
			else
				return "P.ATTACK";
			break;

		case 19:
			if (line == 0)
				return "NOTES>=100";
			else
				return "G.ATTACK";
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
}

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
	if (g->txtStruct.option_str[10].count <= g->config.play.hsfix) SetObjectString(74, HSFIXSTRINGS[g->config.play.hsfix], g->txtStruct.objectStr);
	else SetObjectString(74, g->txtStruct.option_str[10].str[g->config.play.hsfix], g->txtStruct.objectStr);
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
	else if (cmd->isSame("/irupdateall")) { 
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
				g->net.myRanking.inputtype = 2; //TOFIX: input device MIDI??

				g->net.myRanking.ghost = ReadGhost(sql, g->sSelect.bmsList[g->sSelect.cur_song].hash);
				CSTR scorehash;
				cstrSprintf(&scorehash, "%s%s%d%d", g->net.IR_passMD5.body, g->net.myRanking.songMD5.body, g->net.myRanking.exscore, g->net.myRanking.clear);
				scorehash = MD5str(scorehash);
				// TODO: consolidate with the other score sending function.
				cstrSprintf(
						&g->net.param,
						"songmd5=%s&id=%d&passmd5=%s&title=%s&genre=%s&artist=%s&maxbpm=%d&minbpm=%d&&playlevel=%d&clear=%d&exscore=%d&pg=%d&gr=%d&gd=%d&bd=%d&pr=%d&maxcombo=%d&playcount=%d&clearcount=%d&rate=%d&minbp=%d&totalnotes=%d&opt_history=%d&opt_this=%d&line=%d&judge=%d&inputtype=%d&ghost=%s&rseed=%d&clear_db=%d&clear_ex=%d&clear_sd=%d&scorehash=%s",
						g->net.myRanking.songMD5.body, g->net.IR_ID,
						g->net.IR_passMD5.body,
						UrlEncode(utf2ansi(g->net.myRanking.title.body, 932).c_str()).body,
						UrlEncode(utf2ansi(g->net.myRanking.genre.body, 932).c_str()).body,
						UrlEncode(utf2ansi(g->net.myRanking.artist.body, 932).c_str()).body,
						g->net.myRanking.maxbpm,
						g->net.myRanking.minbpm,
						g->net.myRanking.playlevel,
						g->net.myRanking.clear,
						g->net.myRanking.exscore,
						g->net.myRanking.pg, g->net.myRanking.gr,
						g->net.myRanking.gd, g->net.myRanking.bd,
						g->net.myRanking.pr,
						g->net.myRanking.maxcombo,
						g->net.myRanking.playcount,
						g->net.myRanking.clearcount,
						g->net.myRanking.rate,
						g->net.myRanking.minbp,
						g->net.myRanking.totalnotes,
						g->net.myRanking.opt_history,
						g->net.myRanking.opt_this,
						g->net.myRanking.line,
						g->net.myRanking.judge,
						g->net.myRanking.inputtype,
						g->net.myRanking.ghost.body,
						g->net.myRanking.rseed,
						g->net.myRanking.clear_db,
						g->net.myRanking.clear_ex,
						g->net.myRanking.clear_sd, scorehash.body);
				g->net.target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/score.cgi";
				g->net.HTTPrequest();
				printfDx("%sを送信中です…", g->net.myRanking.title.body);
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

	return 1;
}


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
	if (g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[3] == 1) {
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
		LoopInRange(0, 5, 1, &g->config.play.hsfix);
		SetObjectStrings_SongSelect(g);
	}
	if ((g->KeyInput.p1_buttonInput[5] == 2 && g->KeyInput.p1_buttonInput[7] == 1) || (g->KeyInput.p2_buttonInput[5] == 2 && g->KeyInput.p2_buttonInput[7] == 1)) {
		if (g->config.play.battle == 1) {
			PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
			LoopInRange(0, 5, -1, &g->config.play.hsfix);
		}
		else {
			LoopInRange(0, 5, 1, &g->config.play.hsfix);
		}
		SetObjectStrings_SongSelect(g);
	}
	if (g->KeyInput.p1_buttonInput[13] == 1 || g->KeyInput.p2_buttonInput[13] == 1 || g->KeyInput.p1_buttonInput[8] == 1 || g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		LoopInRange(0, 8, 1, &g->config.play.p1_target);
		SetTarget(g);
	}
	else if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) {
		PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
		LoopInRange(0, 8, -1, &g->config.play.p1_target);
		SetTarget(g);
	}

	return 1;
}

void CheckNewSong(glb_dbgame *glb) {
	CONFIG_JUKEBOX jb;
	sqlite3_stmt *pStmt;
	char buf[1024];
	int filDiff, filKey;
	bool err = false;

	std::unique_lock l{g_db_lock};
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
			sqlite3_exec(glb->pSql, fs::make_preferred("DELETE FROM folder WHERE path=\'LR2files/CustomFolder/newsong.lr2folder\'").data(), 0, 0, 0);
			sqlite3_snprintf(1024, buf, "SELECT * FROM song WHERE adddate > %d", GetNowUnixtime() - jb.titleflash * 3600);
			sqlite3_prepare(glb->pSql, buf, -1, &pStmt, NULL);
			if (sqlite3_step(pStmt) == 100) {
				jb.path[jb.numOfPath] = fs::make_preferred("LR2files/CustomFolder/newsong.lr2folder").data();
				GetFolderDataFromPath(jb.path[jb.numOfPath], glb->pSql);
			}
			sqlite3_finalize(pStmt);
		}
	}

	glb->pGame->sSelect.filterDifficulty = filDiff;
	glb->pGame->sSelect.filterKey = filKey;
	glb->pGame->sSelect.isFolder = 0;
	int i = 0;
	for (i = 0; i < glb->pGame->sSelect.queryCount; i++) {
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
		glb->pGame->sSelect.filterDifficulty = filDiff;
		glb->pGame->sSelect.filterKey = filKey;
		glb->pGame->sSelect.isFolder = i + 1;
	}
	if (i >= glb->pGame->sSelect.queryCount) glb->pGame->sSelect.isFolder = -1;

	if (glb->pGame->sSelect.isFolder != -1 && glb->pGame->config.skin.disableimagefont == 0) {
		LoadFontForSongs(glb->pGame, 1);
	}
	glb->pGame->sSelect.searchFocused = 4;
	(glb->pGame->sSelect).unk4fc4[0] = '\0';
	(glb->pGame->sSelect).unk4fc4[1] = '\0';
	(glb->pGame->sSelect).unk4fc4[2] = '\0';
}

static void ThreadProc_RankingAutoUpdate(game* g) {
	CSTR hash = g->sSelect.bmsList[g->sSelect.cur_song].hash;
	g->net.rankingData.Init();
	g->net.rankingData.rivalID = 0;
	g->net.rankingData.rivalRanking = 0;

	if (g->sSelect.stack_query[g->sSelect.cur].findStrPos("__RIVAL__") >= 0) {
		g->net.rankingData.rivalID = g->sSelect.stack_rivalID[g->sSelect.cur];
	}
	CSTR path;
	if (hash.length() < 50) cstrSprintf(&path, fs::make_preferred("LR2files/Ir/%s.xml").data(), hash.body);
	else cstrSprintf(&path, fs::make_preferred("LR2files/Ir/%s.xml").data(), AssignCRC32(hash).body);
	
	if (path.canOpenFile()) {
		if (hash.isSame(g->sSelect.bmsList[g->sSelect.cur_song].hash)) {
			g->net.ParseRankingXml(path);
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

		if (GetNowUnixtime() - GetFileUnixtime(path) < 86400 || g->config.network.autoupdate == 0) { //86400 is 24hours
			return;
		}
	}

	bool isIR2 = g->net.IRstatus == 2;

	if (!g->config.network.autoupdate) {
		if (!isIR2) g->net.IRstatus = 0;
		return;
	}

	if (g->net.rankUpdateDelayLevel >= 3) {
		if (!isIR2) g->net.IRstatus = 5;
		return;
	}

	SetTimeLapse(177, &g->timer1);
	g->net.IRstatus = 3;
	while (GetTimeLapse(177, &g->timer1) <g->net.waitTime) {
		std::this_thread::sleep_for(std::chrono::milliseconds(4));
		if (g->net.waitForHandle || (isIR2 && (g->KeyInput.p1_buttonInput[4] == 2 || g->KeyInput.p2_buttonInput[4] == 2))) {
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
}

static void ThreadProc_LoadPreview(game *g) {
	BMSMETA meta;

	if (!IsFileExist(g->gameplay.previewBMSfilepath)) {
		g->gameplay.isPreviewLoad = 0;
		g->gameplay.previewStatus = 0;
		return;
	}

	meta = g->sSelect.metaSelected;

	g->gameplay.bmsResourceLoaded = 0;
	g->gameplay.flag_closingPhase = '\0';
	g->gameplay.flag_gameinput = '\0';
	g->gameplay.isAutoplay = 1;
	g->gameplay.courseType = -1;
	g->gameplay.isCourse = 0;
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
}

// handle == -1 means an error
// \return input paths and newly made sihandle
static std::pair<std::pair<std::string, std::string>, int> MyLoadImage(CSTR dir, CSTR banner) {
	CSTR image_path;
	if (FindAltImage(banner, dir, &image_path) != 1)
		return {std::pair{dir.c_str(), banner.c_str()}, -1};
	// QUESTION: Skipped initial 300x80 creation, is that fine?
	return {std::pair{dir.c_str(), banner.c_str()}, LoadSoftImageWithStrLen(image_path.c_str(), image_path.length())};
}

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
		SetObjectString(170, g->sSelect.course.name, g->txtStruct.objectStr);
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
		g->hThreadBanner.push_back(std::async(std::launch::async, MyLoadImage,
					g->sSelect.bmsList[g->sSelect.cur_song].filepath.getDirectory(),
					g->sSelect.bmsList[g->sSelect.cur_song].banner));
	}
	if (g->net.isOnline && g->procSelecter == 2) {
		g->net.WaitAndInitRanking();
		if (g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5 && (g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount < 1 || g->sSelect.bmsList[g->sSelect.cur_song].courseIR)) {
			g->net.IRstatus = 1;
			g->net.hHandle = std::jthread(ThreadProc_RankingAutoUpdate, g);
			SetObjectStrings_SongSelect(g);
			return 1;
		}
	}
	g->net.IRstatus = 0;
	SetObjectStrings_SongSelect(g);
	return 1;
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
		&& g->KeyInput.inputID[KEY_INPUT_F3] != 2) {

		if (GetTimeLapse(10, &g->timer1) == -1.0) {
			if (g->net.rankingData.showRanking == 0 && g->net.IRstatus == 2) {
				if ((g->KeyInput.p1_buttonInput[4] == 2 || g->KeyInput.p2_buttonInput[4] == 2) && GetTimeLapse(175, &g->timer1) == -1.0 && g->KeyInput.p1_buttonInput[11] == 0 && g->KeyInput.p1_buttonInput[10] == 0 && g->KeyInput.inputID[KEY_INPUT_UP] == 0 && g->KeyInput.inputID[KEY_INPUT_DOWN] == 0 && g->sSelect.panel != 1) {
					SetTimeLapse(175, &g->timer1);
					return;
				}
				else if (GetTimeLapse(175, &g->timer1) > 120.0) {
					ResetTimeLapse(175, &g->timer1);
					SetTimeLapse(176, &g->timer1);
					g->sSelect.prevListCount = std::min(g->net.rankingData.rankingCount, 999);
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

		if (g->KeyInput.inputID[KEY_INPUT_F8] == 1) {
			if (g->sSelect.stack_query[g->sSelect.cur].findStrPos("parent") != -1) {
				SetBmsFilter(g, sql);
				g->sSelect.unk4fa4[0] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM song WHERE parent = \'%s\'", g->sSelect.stack_query[g->sSelect.cur].right(9).left(8).body);
				if (g->sSelect.bmsList[g->sSelect.cur_song].keymode < 1 || g->sSelect.cur < 1) {
					g->sSelect.reloadType = 3;
					g->sSelect.unk4fa4[1] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", g->sSelect.stack_query[g->sSelect.cur].right(9).left(8).body);
					g->sSelect.unk4fa4[2] = sqlite3_snprintf(1024, buf, "SELECT path, date FROM song WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath).body);
					g->sSelect.unk4fa4[3] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath).body);
				}
				else {
					g->sSelect.reloadType = 2;
					g->sSelect.unk4fa4[1] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", g->sSelect.stack_query[g->sSelect.cur].right(9).left(8).body);
					g->sSelect.unk4fa4[2] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", g->sSelect.stack_query[g->sSelect.cur - 1].right(9).left(8).body);
				}
				g->sSelect.filter_clicked = 4;
			}

			if (g->sSelect.panel == 1) {
				SetPlayOption(g, sql);
				if (g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.inputID[KEY_INPUT_SPACE])
					return;
			}
		}
		else if (g->KeyInput.p1_buttonInput[12] == 1 || g->KeyInput.p2_buttonInput[12] == 1 || g->KeyInput.p1_buttonInput[9] == 1 || g->KeyInput.inputID[KEY_INPUT_SPACE] == 1) {
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
				if (g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.inputID[KEY_INPUT_SPACE])
					return;
			}
		}
		else if (g->sSelect.panel == 1) {
			if (g->KeyInput.p1_buttonInput[12] == 3 || g->KeyInput.p2_buttonInput[12] == 3 || g->KeyInput.p1_buttonInput[9] == 3 || g->KeyInput.inputID[KEY_INPUT_SPACE] == 3) {
				ResetTimeLapse(21, &g->timer1);
				SetTimeLapse(30 + g->sSelect.panel, &g->timer1);
				g->sSelect.panel = -1;
				PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
			}

			if (g->sSelect.panel == 1) {
				SetPlayOption(g, sql);
				if (g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.inputID[KEY_INPUT_SPACE])
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
			|| g->KeyInput.p1_buttonInput[10] == 1 || g->KeyInput.p2_buttonInput[10] == 1 || g->KeyInput.inputID[KEY_INPUT_UP] == 1)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

			g->sSelect.nowBar -= 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 1;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
		}
		else if ((((g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[3] == 1) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[6] == 1 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[11] == 1 || g->KeyInput.p2_buttonInput[11] == 1 || g->KeyInput.inputID[KEY_INPUT_DOWN] == 1)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

			g->sSelect.nowBar += 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 2;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
		}
		else if ((((g->KeyInput.p1_buttonInput[1] == 2 || g->KeyInput.p2_buttonInput[1] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[4] == 2 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[10] == 2 || g->KeyInput.p2_buttonInput[10] == 2 || g->KeyInput.inputID[KEY_INPUT_UP] == 2)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime - 20) {

			g->sSelect.nowBar -= 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 1;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speednext;
		}
		else if ((((g->KeyInput.p1_buttonInput[3] == 2 || g->KeyInput.p2_buttonInput[3] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[6] == 2 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[11] == 2 || g->KeyInput.p2_buttonInput[11] == 2 || g->KeyInput.inputID[KEY_INPUT_DOWN] == 2)
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
				&& ((g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1 || g->KeyInput.p1_buttonInput[13] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.select.control == 0)
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
				&& ((g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1 || g->KeyInput.p1_buttonInput[13] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.select.control == 0)
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
				((MouseOnDSTD(&dstd, &g->KeyInput.mouse_oldX, &g->KeyInput.mouse_oldY) == 0 || g->KeyInput.mouse_buttonL != 1) && g->KeyInput.inputID[KEY_INPUT_RIGHT] != 1)
				&& (g->KeyInput.inputID[KEY_INPUT_RETURN] != 1 || g->txtStruct.st_text_num != -1 || (GetTimeLapse(4, &g->timer1) != -1.0 && GetTimeLapse(4, &g->timer1) < 200.0)) 
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
						g->sSelect.curQuery[0] = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE %s", g->sSelect.bmsList[g->sSelect.cur_song].tag.body);
						g->sSelect.curQuery[2] = sqlite3_snprintf(1024, buf, "SELECT * FROM folder WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath.body).body);
						g->sSelect.curQuery[1] = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath).body);
						(g->sSelect).unk4fb8[0] = 0;
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
						g->sSelect.unk4fa4[1] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM folder WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath).body);
						g->sSelect.unk4fa4[0] = sqlite3_snprintf(1024, buf, "SELECT path,date FROM song WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath).body);
						g->sSelect.curQuery[1] = sqlite3_snprintf(1024, buf, "SELECT * FROM folder WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath).body);
						g->sSelect.curQuery[0] = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE parent = \'%s\'", AssignCRC32(g->sSelect.bmsList[g->sSelect.cur_song].filepath).body);
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
			&& (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1
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

// ProcI_Select
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
							if (g->sSelect.bmsList[bar].exlevel > 0 && (g->config.jukebox.customfolder & 0x80) != 0) 
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
		if (GetTimeLapse(11, &g->timer1) >= 500.0 && g->gameplay.previewStatus == 0 &&
				g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5 &&
				g->sSelect.bmsList[g->sSelect.cur_song].courseStageCount == 0 &&
				(!g->gameplay.hThreadPreview.valid() ||
				 g->gameplay.hThreadPreview.wait_for(std::chrono::seconds(0)) ==
				 std::future_status::ready)) {
			g->gameplay.flag_closingPhase = 1;
			g->gameplay.isPreviewLoad = 0;
			g->gameplay.previewStatus = 1;
			g->gameplay.previewBMShash = g->sSelect.bmsList[g->sSelect.cur_song].hash;
			g->gameplay.previewBMSfilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
			g->gameplay.hThreadPreview = std::async(std::launch::async, ThreadProc_LoadPreview, g);
		}
		else if (g->gameplay.previewStatus) {
			if (g->gameplay.previewBMShash.isDiff(g->sSelect.bmsList[g->sSelect.cur_song].hash)) {
				g->gameplay.flag_closingPhase = 1;
				g->gameplay.isPreviewLoad = 0;
				g->gameplay.flag_gameinput = 0;
				for (int i = 0; i < SLOTS; i++) {
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

	for (auto& banner_future : g->hThreadBanner)
	{
		if (!banner_future.valid() || banner_future.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
			continue;
		const auto& [dir_and_banner, sihandle] = banner_future.get();
		const auto& [dir, banner] = dir_and_banner;
		if (dir != std::string_view{ g->sSelect.bmsList[g->sSelect.cur_song].filepath.getDirectory() }
			|| banner != std::string_view{ g->sSelect.bmsList[g->sSelect.cur_song].banner })
		{
			DeleteSoftImage(sihandle);
			continue;
		}
		if (sihandle == -1)
		{
			g->sSelect.bmsList[g->sSelect.cur_song].isBanner = 0;
			continue;
		}
		if (ReCreateGraphFromSoftImage(sihandle, g->skstruct.GrHandle[GrH_Banner]) == -1)
			g->sSelect.bmsList[g->sSelect.cur_song].isBanner = 0;
		DeleteSoftImage(sihandle);
	}
	std::erase_if(g->hThreadBanner, std::not_fn(&std::future<std::pair<std::pair<std::string, std::string>, int>>::valid));

	return 1;
}



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

// COURSESELECT_AUTOCOPY
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
				if (g->config.select.ignorekeydouble == 1 && g->sSelect.filterKey == 4) g->sSelect.filterKey = 5;
				if (g->config.select.ignoredp == 1 && (4 <= g->sSelect.filterKey && g->sSelect.filterKey <= 6)) g->sSelect.filterKey = 7;
				if (g->config.select.ignorepms == 1 && g->sSelect.filterKey == 7) g->sSelect.filterKey = 0;
				if (g->config.select.ignorekeyall == 1 && g->sSelect.filterKey == 0) g->sSelect.filterKey = 1;
			}
		}

		if (g->config.select.disabledifficultyfilter == 1) {
			g->sSelect.filterDifficulty = 0;
			g->config.select.difficulty = 0;
		}
		glb_dbgame glb {sql, g};
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

				if (g->sSelect.bmsListCount < 2) cstrSprintf(&msg, "%s (1 HIT)", g->sSelect.searchInput.body);
				else cstrSprintf(&msg, "%s (%d HITS)", g->sSelect.searchInput.body, g->sSelect.bmsListCount);

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

int InitBmsList(SONGSELECT *ss) {
	// FIXME: 1) move to SONGSELECT constructor 2) move to std::vector. This leaks memory.
	ss->bmsListSize = 1000;
	ss->cur = 0;
	ss->bmsList = (SONGDATA*)malloc(sizeof(SONGDATA) * 1000);
	assert(ss->bmsList != nullptr);
	for (int i = 0; i < ss->bmsListSize; i++) {
		memset(&ss->bmsList[i], 0, sizeof(SONGDATA));
	}

	ss->prevListSize = 1000;
	ss->prevList = (SONGDATA*)malloc(sizeof(SONGDATA) * 1000);
	assert(ss->prevList != nullptr);
	for (int i = 0; i < ss->prevListSize; i++) {
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

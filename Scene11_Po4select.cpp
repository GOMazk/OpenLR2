#include "Scene11_Po4select.h"
#include "Scenes.h"
#include "LR2.h"

//40d600
void ThreadProc_PO4parseBMS(game *g) {
	
	g->gameplay.bmsResourceLoaded = 0;
	g->gameplay.flag_closingPhase = 0;
	g->gameplay.isCourse = 0;
	g->gameplay.courseStageCount = 0;
	InitGameplay(&g->gameplay, &g->config.play);
	
	ParseBmsFile(&g->gameplay, g->sSelect.metaSelected.filepath, &g->audio, &g->config, &g->sSelect.metaSelected, g->skstruct.flag_BGA, 0);
	g->po4_hThread_ParseBMS = 0;
	g->gameplay.bmsResourceLoaded = 1;
	return;
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
			|| g->KeyInput.p1_buttonInput[10] == 1 || g->KeyInput.p2_buttonInput[10] == 1 || g->KeyInput.inputID[KEY_INPUT_UP] == 1 || g->KeyInput.inputID[KEY_INPUT_LEFT] == 1)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

			g->sSelect.nowBar -= 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 1;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
		}
		else if ((((g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[3] == 1) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[6] == 1 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[11] == 1 || g->KeyInput.p2_buttonInput[11] == 1 || g->KeyInput.inputID[KEY_INPUT_DOWN] == 1 || g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime) {

			g->sSelect.nowBar += 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 2;
			g->sSelect.barMoveEndTime = GetTimeWrap() + g->config.select.speedfirst;
		}
		else if ((((g->KeyInput.p1_buttonInput[1] == 2 || g->KeyInput.p2_buttonInput[1] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[4] == 2 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[10] == 2 || g->KeyInput.p2_buttonInput[10] == 2 || g->KeyInput.inputID[KEY_INPUT_UP] == 2 || g->KeyInput.inputID[KEY_INPUT_LEFT] == 2)
			&& GetTimeWrap() > g->sSelect.barMoveEndTime - 20) {

			g->sSelect.nowBar -= 1000;
			g->sSelect.oldBar = g->sSelect.listTopbar;
			g->sSelect.barMoveStartTime = GetTimeWrap();
			g->sSelect.scrollDirection = 1;
			g->sSelect.barMoveEndTime = GetTimeWrap() + 150;
		}
		else if ((((g->KeyInput.p1_buttonInput[3] == 2 || g->KeyInput.p2_buttonInput[3] == 2) && g->config.select.buttonselect == 1 && g->config.select.control == 0)
			|| (g->KeyInput.p1_buttonInput[6] == 2 && g->config.select.control == 1)
			|| g->KeyInput.p1_buttonInput[11] == 2 || g->KeyInput.p2_buttonInput[11] == 2 || g->KeyInput.inputID[KEY_INPUT_DOWN] == 2 || g->KeyInput.inputID[KEY_INPUT_RIGHT] == 2)
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
		if (g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1 && g->po4flagSceneStart == 0) {
			g->procSelecter = 0;
			return 1;
		}
		if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[KEY_INPUT_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
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
				if (g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1 && g->po4flagSceneStart == 0) {
					g->procSelecter = 0;
					return 1;
				}

				if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[KEY_INPUT_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
					&& g->po4flagSceneStart == 0) {

					//g->sSelect.listCalculatedBar / 1000;
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

			if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[6] == 1)
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
			else if ((g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[KEY_INPUT_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
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
			else if (g->KeyInput.inputID[KEY_INPUT_F1] == 1 && g->po4flagSceneStart == 0 && g->txtStruct.readme.show == 0) {
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
			else if (g->KeyInput.inputID[KEY_INPUT_F3] == 1 && g->po4flagSceneStart == 0 && g->txtStruct.readme.show == 0) {
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
			
			if (g->KeyInput.inputID[KEY_INPUT_F2] == 1 && g->po4flagSceneStart == 0 && g->txtStruct.readme.show == 0) {
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

				if (g->KeyInput.inputID[KEY_INPUT_F2] == 1 || g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1) {
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
			if ((g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 || g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1 || g->KeyInput.inputID[KEY_INPUT_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
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

			if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) {
				g->config.play.hiSpeed[0] += 10;
				if (g->config.play.hiSpeed[0] >= 400) g->config.play.hiSpeed[0] = 400;
			}
			else if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) {
				g->config.play.hiSpeed[0] -= 10;
				if (g->config.play.hiSpeed[0] <= 50) g->config.play.hiSpeed[0] = 50;
			}

			if (GetTimeLapse(412, &g->timer1) < 0.0) {
				if (g->po4_23da8 == 0) {
					if (g->gameplay.flag_gameinput) {
						ProcGame(g);
					}
					else if (g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1) {
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
					if ( (g->KeyInput.mouse_buttonR == 2 || g->KeyInput.mouse_buttonL == 2 ||g->KeyInput.inputID[KEY_INPUT_ESCAPE] == 1 || g->KeyInput.inputID[KEY_INPUT_RETURN] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1 || g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p1_buttonInput[7] == 1 || g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
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
					((g->sSelect.bmsList[curmod].level - 1) * (int)g->skstruct.dst_BAR_STAR[g->po4cur_song].draw->w /2);
					
					for (int i = 0; i < g->sSelect.bmsList[curmod].level; i++) {
						AddDrawingBuffer_ObjectAlpha(&g->skstruct.drBuf, &g->skstruct.src_BAR_STAR[g->po4cur_song], &g->skstruct.dst_BAR_STAR[g->po4cur_song], &g->timer1, dstd3.x - ((g->sSelect.bmsList[curmod].level - 1) * (int)g->skstruct.dst_BAR_STAR[g->po4cur_song].draw->w / 2) + i, dstd3.y, 255);
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
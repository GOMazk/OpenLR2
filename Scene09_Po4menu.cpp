#include "Scene09_Po4menu.h"
#include "Scene02_Songselect.h"
#include "LR2.h"

//422210
int ProcI_PO4Menu(game *g, sqlite3 *sql) { //not tested

	int selectedBar = g->sSelect.listSelectedBarFromScreenTop;

	if (GetTimeLapse(0, &g->timer1) > g->skstruct.startinput_start
		&& g->txtStruct.readme.show == 0
		&& g->txtStruct.st_text_num < 0
		&& g->sSelect.buttonObjClicked != 1
		&& g->sSelect.searchFocused == 0
		&& !(GetTimeLapse(4, &g->timer1) >= 0 && GetTimeLapse(4, &g->timer1) <= 100.0)
		&& g->sSelect.flag_maniacPanel == 0
		&& g->KeyInput.inputID[KEY_INPUT_F3] != 2) {

		if (GetTimeLapse(175, &g->timer1) == -1.0 && GetTimeLapse(176, &g->timer1) == -1.0) {
			
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
				DSTdraw dstd;
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
					}
				}
				else if (g->sSelect.is_clicked_autoplay_replay) {
					g->sSelect.is_clicked_autoplay_replay = 0;
				}
				else if ((((g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[6] == 1) && g->config.select.control == 0)
					|| (g->KeyInput.p1_buttonInput[1] == 1 && g->config.select.control == 1))
					&& g->sSelect.panel != 1) {
					//do nothing
				}
				else if (GetTimeLapse(172, &g->timer1) != -1.0) {
					//do nothing
				}
				else if (g->sSelect.panel != 1
					&& ((g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1 || g->KeyInput.p1_buttonInput[13] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.select.control == 0)
					&& g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5) {
					//do nothing
				}
				else if (g->sSelect.panel != 1
					&& ((g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[8] == 1) && g->config.select.control == 1)
					&& g->sSelect.bmsList[g->sSelect.cur_song].keymode >= 5) {
					//do nothing
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
				else {
					g->po4MainMenuCursor = g->sSelect.bmsList[g->sSelect.cur_song].level;
					g->procSelecter = (g->po4MainMenuCursor != 6) ? 10 : 0;
				}
			}
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

	selectedBar = g->sSelect.listCalculatedBar % 1000;
	GetNowUnixtime();

	for (int i = 1; i < 30; i++) {
		if (g->skstruct.dst_BAR_BODY_OFF[i - 1].dstCount > 0 && g->skstruct.dst_BAR_BODY_OFF[i].dstCount > 0) {
			DSTdraw dstd1, dstd2, dstd3;
			InitDSTdraw(&dstd1);
			InitDSTdraw(&dstd2);
			InitDSTdraw(&dstd3);

			int bar = (g->sSelect.listCalculatedBar / 1000 - g->skstruct.BAR_CENTER) + i;
			while (bar < 0) {
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
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_ON[i - 1], GetTimeLapse(0, &g->timer1));
			}
			else {
				dstd2 = SetDSTdrawByTime(g->skstruct.dst_BAR_BODY_OFF[i - 1], GetTimeLapse(0, &g->timer1));
			}

			if (selectedBar == 0) {
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

				AddDrawingBuffer_TextXY(&g->skstruct.drBuf, &g->skstruct.src_BAR_TITLE[0], &g->skstruct.dst_BAR_TITLE[0], &g->timer1, bar + 10000, dstd3.x, dstd3.y);
			}
		}
	}

	return 1;
}
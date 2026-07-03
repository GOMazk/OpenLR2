#include "LR2_gameloop.h"
#include "Engine.h"
#include "LR2_replay.h"
#include "LR2_audio.h"
#include "LR2_skinobject.h"

#include "Scenes.h"

void ReactInput(game *g) {

	GetTimeWrap();
	if (g->txtStruct.readme.show > 0 && g->is_starter == 0) {

		g->txtStruct.readme.h += g->KeyInput.mousewheel * g->skstruct.src_README[0].op1;

		if ( (g->KeyInput.mouse_buttonR == 3
			|| (g->KeyInput.inputID[KEY_INPUT_F2] == 1 || g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[2] == 1)
			|| (g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[4] == 1 || g->KeyInput.p1_buttonInput[5] == 1)
			|| (g->KeyInput.p1_buttonInput[6] == 1 || g->KeyInput.p1_buttonInput[7] == 1))
			|| (g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[4] == 1 || g->KeyInput.p2_buttonInput[5] == 1 || g->KeyInput.p2_buttonInput[6] == 1 || g->KeyInput.p2_buttonInput[7] == 1)
			|| g->KeyInput.inputID[KEY_INPUT_RETURN] == 1) {
			
			ResetTimeLapse(15, &g->timer1);
			SetTimeLapse(16, &g->timer1);
			g->txtStruct.readme.show = 0;
		}
		else if (g->KeyInput.mouse_buttonL == 2) { //original is very slow, it is adequately fast. It's misbehavior...but isn't it good?
			if (g->KeyInput.mouse_oldX < 200) {
				g->txtStruct.readme.w += g->timer1.tickTime * 600.0 / 1000.0;
			}
			else if (g->KeyInput.mouse_oldX > 440) {
				g->txtStruct.readme.w += g->timer1.tickTime * 600.0 / -1000.0;
			}
			
			if (g->KeyInput.mouse_oldY < 150) {
				g->txtStruct.readme.h -= g->timer1.tickTime * 600.0 / -1000.0;
			}
			else if (g->KeyInput.mouse_oldY > 330) {
				g->txtStruct.readme.h -= g->timer1.tickTime * 600.0 / 1000.0;
			}
		}

		if (g->KeyInput.inputID[KEY_INPUT_UP] == 2) {
			g->txtStruct.readme.h += g->timer1.tickTime * 600.0 / 1000.0;
		}
		if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 2) {
			g->txtStruct.readme.h += 600.0 * g->timer1.tickTime / -1000.0;
		}
		//TODO_RESOULUTION
		if (g->txtStruct.readme.h < 480 - g->txtStruct.readme.y) g->txtStruct.readme.h = 480 - g->txtStruct.readme.y;
		if (g->txtStruct.readme.w < 640 - g->txtStruct.readme.x) g->txtStruct.readme.w = 640 - g->txtStruct.readme.x;
		if (g->txtStruct.readme.w > 0) g->txtStruct.readme.w = 0;
		if (g->txtStruct.readme.h > 0) g->txtStruct.readme.h = 0;
	}
	GetTimeWrap();

	GetTimeWrap();
	if (g->audio.is_fmod_disabled== 0) FMOD_System_Update(g->audio.fmodSys);
	SetVolumeByFade(&g->audio);
	GetTimeWrap();

	GetTimeWrap();
	if (g->sSelect.metaSelected.keymode >= 10) {
		if (g->procSelecter == 4 && g->config.play.battle == OPTION_BATTLE_SP2DP) {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 1, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 1, 0);
		}
		else {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 0, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 0, 0);
		}
	}
	else {
		if (g->procSelecter == 4 && g->config.play.battle == OPTION_BATTLE_OFF) {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 1, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 1, 0);
		}
		else if (g->sSelect.metaSelected.keymode <= 8) {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 0, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 0, 0);
		}
		else if (g->procSelecter == 4 && g->config.play.battle == OPTION_BATTLE_SP2DP) {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 1, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 1, 0);
		}
		else {
			if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0)
				InputToButton(&g->KeyInput, &g->config.input, 0, 1);
			else
				InputToButton(&g->KeyInput, &g->config.input, 0, 0);
		}
	}
	GetTimeWrap();

	GetTimeWrap();

	if (g->gameplay.replay.status == 1 && GetTimeLapse(41, &g->timer1) != -1.0 && g->procSelecter == 4) {
		InputToReplay(&g->gameplay.replay, &g->KeyInput, GetTimeLapse(41, &g->timer1), g->gameplay.scratchSide);
	}
	else if (g->gameplay.replay.status == 2 && GetTimeLapse(41, &g->timer1) != -1.0 && g->procSelecter == 4) {
		ReplayToInput(&g->gameplay.replay, g, &g->audio, &g->gameplay, &g->KeyInput, &g->timer1);
	}
	GetTimeWrap();

	GetTimeWrap();
	if (g->txtStruct.st_text_num != -1) {
		std::ranges::fill(g->KeyInput.p1_buttonInput, 0);
		std::ranges::fill(g->KeyInput.p2_buttonInput, 0);
	}
	if (GetTimeLapse(1, &g->timer1) >= 0.0) {
		for (int i = 0; i < 10; i++) {
			if (g->KeyInput.p1_buttonInput[i] == 1) {
				SetTimeLapse(100 + i, &g->timer1);
				ResetTimeLapse(120 + i, &g->timer1);
			}
			if (g->KeyInput.p1_buttonInput[i] == 3) {
				ResetTimeLapse(100 + i, &g->timer1);
				SetTimeLapse(120 + i, &g->timer1);
			}
		}
		
		for (int i = 0; i < 10; i++) {
			if (g->KeyInput.p2_buttonInput[i] == 1) {
				SetTimeLapse(110 + i, &g->timer1);
				ResetTimeLapse(130 + i, &g->timer1);
			}
			if (g->KeyInput.p2_buttonInput[i] == 3) {
				ResetTimeLapse(110 + i, &g->timer1);
				SetTimeLapse(130 + i, &g->timer1);
			}
		}
	}
	GetTimeWrap();

	GetTimeWrap();
	FxByMIDI(g);
	GetTimeWrap();

	GetTimeWrap();
	SetObjectValue_Slider(g, &g->skstruct, &g->timer1, 0);
	GetTimeWrap();

	GetTimeWrap();
	SetObjectValue_Button(g, &g->skstruct, &g->timer1, 0);
	GetTimeWrap();

	GetTimeWrap();
	if (g->procSelecter == 7) {
		SetObjectValue_Slider(g, &g->skstruct2, &g->timer2, 1);
		SetObjectValue_Button(g, &g->skstruct2, &g->timer2, 1);
	}

	if (g->procSelecter == 4) {
		if (g->config.play.p1_lanecover == 1) g->config.play.p1_lanecoverv -= g->KeyInput.mousewheel;
		
		if (g->is_starter == 0) {
			if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
				&& (g->KeyInput.p1_buttonInput[1] == 1 || g->KeyInput.p1_buttonInput[3] == 1 || g->KeyInput.p1_buttonInput[5] == 1)) {

				g->config.play.hiSpeed[0] -= g->config.play.hsmargin;
				g->gameplay.isSpeedChanged = true;
			}

			if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
				&& (g->KeyInput.p1_buttonInput[2] == 1 || g->KeyInput.p1_buttonInput[4] == 1)) {

				g->config.play.hiSpeed[0] += g->config.play.hsmargin;
				g->gameplay.isSpeedChanged = true;
			}

			if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
				&& (g->KeyInput.p2_buttonInput[1] == 1 || g->KeyInput.p2_buttonInput[3] == 1 || g->KeyInput.p2_buttonInput[5] == 1)) {

				if (g->config.play.battle == OPTION_BATTLE_BATTLE) {
					g->config.play.hiSpeed[1] -= g->config.play.hsmargin;
					g->gameplay.isSpeedChanged = true;
				}
				else {
					g->config.play.hiSpeed[0] -= g->config.play.hsmargin;
					g->gameplay.isSpeedChanged = true;
				}
			}

			if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
				&& (g->KeyInput.p2_buttonInput[2] == 1 || g->KeyInput.p2_buttonInput[4] == 1)) {

				if (g->config.play.battle == OPTION_BATTLE_BATTLE) {
					g->config.play.hiSpeed[1] += g->config.play.hsmargin;
					g->gameplay.isSpeedChanged = true;
				}
				else {
					g->config.play.hiSpeed[0] += g->config.play.hsmargin;
					g->gameplay.isSpeedChanged = true;
				}
			}

			if (g->config.play.p1_lanecover && g->gameplay.lanecoverDisplayP1 == 1) {
				if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
					&& (g->KeyInput.p1_buttonInput[6] == 1)) {
					g->config.play.p1_lanecoverv -= g->config.play.shuttermargin;
				}

				if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
					&& (g->KeyInput.p1_buttonInput[7] == 1)) {
					g->config.play.p1_lanecoverv += g->config.play.shuttermargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[6] == 1) && g->config.play.battle != OPTION_BATTLE_BATTLE) {
					g->config.play.p1_lanecoverv -= g->config.play.shuttermargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[7] == 1) && g->config.play.battle != OPTION_BATTLE_BATTLE) {
					g->config.play.p1_lanecoverv += g->config.play.shuttermargin;
				}
			}
			else {
				if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
					&& (g->KeyInput.p1_buttonInput[6] == 1)) {
					g->config.play.hiSpeed[0] += g->config.play.hsmargin;
				}

				if ((g->KeyInput.p1_buttonInput[12] == 2 || g->KeyInput.p1_buttonInput[13] == 2)
					&& (g->KeyInput.p1_buttonInput[7] == 1)) {
					g->config.play.hiSpeed[0] -= g->config.play.hsmargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[6] == 1) && g->config.play.battle != OPTION_BATTLE_BATTLE) {
					g->config.play.hiSpeed[0] += g->config.play.hsmargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[7] == 1) && g->config.play.battle != OPTION_BATTLE_BATTLE) {
					g->config.play.hiSpeed[0] -= g->config.play.hsmargin;
				}
			}

			if (g->config.play.p2_lanecover && g->gameplay.lanecoverDisplayP2 == 1) {
				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[6] == 1) && g->config.play.battle == OPTION_BATTLE_BATTLE) {
					g->config.play.p2_lanecoverv -= g->config.play.shuttermargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[7] == 1) && g->config.play.battle == OPTION_BATTLE_BATTLE) {
					g->config.play.p2_lanecoverv += g->config.play.shuttermargin;
				}
			}
			else {
				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[6] == 1) && g->config.play.battle == OPTION_BATTLE_BATTLE) {
					g->config.play.hiSpeed[1] += g->config.play.hsmargin;
				}

				if ((g->KeyInput.p2_buttonInput[12] == 2 || g->KeyInput.p2_buttonInput[13] == 2)
					&& (g->KeyInput.p2_buttonInput[7] == 1) && g->config.play.battle == OPTION_BATTLE_BATTLE) {
					g->config.play.hiSpeed[1] -= g->config.play.hsmargin;
				}
			}
		}
	}

	if (g->config.play.p1_lanecoverv < 0) g->config.play.p1_lanecoverv = 0;
	if (g->config.play.p1_lanecoverv > 100) g->config.play.p1_lanecoverv = 100;

	if (g->config.play.hiSpeed[0] < g->config.play.hsmin) g->config.play.hiSpeed[0] = g->config.play.hsmin;
	if (g->config.play.hiSpeed[0] > g->config.play.hsmax) g->config.play.hiSpeed[0] = g->config.play.hsmax;

	if (g->config.play.p2_lanecoverv < 0) g->config.play.p2_lanecoverv = 0;
	if (g->config.play.p2_lanecoverv > 100) g->config.play.p2_lanecoverv = 100;

	if (g->config.play.hiSpeed[1] < g->config.play.hsmin) g->config.play.hiSpeed[1] = g->config.play.hsmin;
	if (g->config.play.hiSpeed[1] > g->config.play.hsmax) g->config.play.hiSpeed[1] = g->config.play.hsmax;

	if (g->KeyInput.p1_buttonInput[10] == 2) {
		g->skstruct.scratchAngle_1 -= (GetTimeWrap() - g->timer1.scratch) * 2 * 360.0 / 1000.0;
	}
	else if (g->KeyInput.p1_buttonInput[11] == 2) {
		g->skstruct.scratchAngle_1 += (GetTimeWrap() - g->timer1.scratch) * 2 * 360.0 / 1000.0;
	}
	else if (GetTimeLapse(41, &g->timer1) >= 0.0) {
		g->skstruct.scratchAngle_1 += (GetTimeWrap() - g->timer1.scratch) * 360.0 / 1000.0;
	}

	if (g->KeyInput.p2_buttonInput[10] == 2) {
		g->skstruct.scratchAngle_2 -= (GetTimeWrap() - g->timer1.scratch) * 2 * 360.0 / 1000.0;
	}
	else if (g->KeyInput.p2_buttonInput[11] == 2) {
		g->skstruct.scratchAngle_2 += (GetTimeWrap() - g->timer1.scratch) * 2 * 360.0 / 1000.0;
	}
	else if (GetTimeLapse(41, &g->timer1) >= 0.0) {
		g->skstruct.scratchAngle_2 += (GetTimeWrap() - g->timer1.scratch) * 360.0 / 1000.0;
	}

	if (GetTimeLapse(41, &g->timer1) >= 0) {
		g->timer1.Rhythm = ( (GetTimeWrap() - g->timer1.rhythmTick) * g->gameplay.BPM) / 60.0 + g->timer1.Rhythm;
	}

	if(g->procSelecter==4){
		if ((g->KeyInput.p1_buttonInput[12] == 1 || g->KeyInput.p1_buttonInput[13] == 1) || ((g->KeyInput.p2_buttonInput[12] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.play.battle != OPTION_BATTLE_BATTLE)) {
			if (g->gameplay.lanecoverDoubleclickTimeP1 == 0 || GetTimeWrap() - g->gameplay.lanecoverDoubleclickTimeP1 > 250.0) {
				g->gameplay.lanecoverDoubleclickTimeP1 = GetTimeWrap();
			}
			else {
				g->gameplay.lanecoverDoubleclickTimeP1 = 0;
				g->gameplay.lanecoverDisplayP1 = (g->gameplay.lanecoverDisplayP1 == 0); //means, toggle
			}
		}

		if ((g->KeyInput.p2_buttonInput[12] == 1 || g->KeyInput.p2_buttonInput[13] == 1) && g->config.play.battle == OPTION_BATTLE_BATTLE) {
			if (g->gameplay.lanecoverDoubleclickTimeP2 == 0 || GetTimeWrap() - g->gameplay.lanecoverDoubleclickTimeP2 > 250.0) {
				g->gameplay.lanecoverDoubleclickTimeP2 = GetTimeWrap();
			}
			else {
				g->gameplay.lanecoverDoubleclickTimeP2 = 0;
				g->gameplay.lanecoverDisplayP2 = (g->gameplay.lanecoverDisplayP2 == 0); //means, toggle
			}
		}

		if (g->KeyInput.mouse_buttonL == 1) {
			if (g->gameplay.lanecoverDoubleclickTimeP1 == 0 || GetTimeWrap() - g->gameplay.lanecoverDoubleclickTimeP1 > 250.0) {
				g->gameplay.lanecoverDoubleclickTimeP1 = GetTimeWrap();
			}
			else {
				g->gameplay.lanecoverDoubleclickTimeP1 = 0;
				g->gameplay.lanecoverDisplayP1 = (g->gameplay.lanecoverDisplayP1 == 0); //means, toggle
			}
		}
	}

	if(g->config.play.m_lunaris==1 && GetTimeLapse(41,&g->timer1) > 0.0 && g->procPhase == 1){
		ProcI_Lunaris(g);
	}
	if (g->procSelecter == 4 && g->config.play.m_lunaris == 0) {
		
		if (g->KeyInput.inputID[KEY_INPUT_1] == 2) {
			if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) g->skstruct.adjust.shift_y -= 10;
			if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) g->skstruct.adjust.shift_y += 10;
			if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) g->skstruct.adjust.shift_x -= 10;
			if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) g->skstruct.adjust.shift_x += 10;
		}
		else if (g->KeyInput.inputID[KEY_INPUT_2] == 2) {
			if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) g->skstruct.adjust.rate_y -= 5;
			if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) g->skstruct.adjust.rate_y += 5;
			if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) g->skstruct.adjust.rate_x -= 5;
			if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) g->skstruct.adjust.rate_x += 5;
		}
		else if (g->KeyInput.inputID[KEY_INPUT_3] == 2) {
			if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) g->skstruct.adjust.judge_y -= 10;
			if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) g->skstruct.adjust.judge_y += 10;
			if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) g->skstruct.adjust.judge_x -= 10;
			if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) g->skstruct.adjust.judge_x += 10;
		}
		else if (g->KeyInput.inputID[KEY_INPUT_4] == 2) {
			if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) g->skstruct.adjust.size_y += 1;
			if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) g->skstruct.adjust.size_y -= 1;
			if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) g->skstruct.adjust.size_x -= 1;
			if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) g->skstruct.adjust.size_x += 1;
		}
		else if (g->KeyInput.inputID[KEY_INPUT_5] == 2) {
			if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) {
				LoopInRange(0, 2, 1, &g->skstruct.adjust.dark_type); //original code doesn't call function, but it's same...
				/*g->skstruct.adjust.dark_type++;
				if (g->skstruct.adjust.dark_type > 2) g->skstruct.adjust.dark_type = 0;
				else if (g->skstruct.adjust.dark_type < 0) g->skstruct.adjust.dark_type = 2;*/
			}
			if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) LoopInRange(0, 2, -1, &g->skstruct.adjust.dark_type);
			if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) LoopInRange(0, 2, 1, &g->skstruct.adjust.dark_type);
			if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(0, 2, -1, &g->skstruct.adjust.dark_type);
		}
		else if (g->KeyInput.inputID[KEY_INPUT_6] == 2) {
			if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) g->skstruct.adjust.note_1p_y -= 10;
			if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) g->skstruct.adjust.note_1p_y += 10;
			if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) g->skstruct.adjust.note_1p_x -= 10;
			if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) g->skstruct.adjust.note_1p_x += 10;
		}
		else if (g->KeyInput.inputID[KEY_INPUT_7] == 2) {
			if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) g->skstruct.adjust.note_2p_y -= 10;
			if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) g->skstruct.adjust.note_2p_y += 10;
			if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1) g->skstruct.adjust.note_2p_x -= 10;
			if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) g->skstruct.adjust.note_2p_x += 10;
		}
		else if (g->config.play.disablecurspeedchange != 1) {
			if (g->KeyInput.inputID[KEY_INPUT_UP] == 1) {
				LoopInRange(g->config.play.hsmin, g->config.play.hsmax, g->config.play.hsmargin, &g->config.play.hiSpeed[0]); //original code doesn't call function, but it's same...
			}
			if (g->KeyInput.inputID[KEY_INPUT_DOWN] == 1) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, -g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
			if (g->KeyInput.inputID[KEY_INPUT_LEFT] == 1){
				LoopInRange(-1000, 1000, g->config.play.shuttermargin, &g->config.play.p1_lanecoverv); //original code doesn't call function, but it's same...
			}
			if (g->KeyInput.inputID[KEY_INPUT_RIGHT] == 1) LoopInRange(-1000, 1000, -g->config.play.shuttermargin, &g->config.play.p1_lanecoverv);

			if (g->config.play.p1_lanecoverv > 100) g->config.play.p1_lanecoverv = 100;
			if (g->config.play.p1_lanecoverv < 0) g->config.play.p1_lanecoverv = 0;
		}
	}
	g->timer1.scratch = GetTimeWrap();
	g->timer1.rhythmTick = GetTimeWrap();
	if (g->procSelecter == 7) {
		g->timer2.scratch = g->timer1.scratch;
		g->timer2.rhythmTick = g->timer1.rhythmTick;
	}
	if (g->KeyInput.inputID[KEY_INPUT_F6] == 1) {
		g->flag_Screenshot = true;
	}
	if (g->KeyInput.inputID[KEY_INPUT_F7] == 1) {
		g->flag_showFPS = !g->flag_showFPS;
	}
}







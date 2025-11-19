#include "LR2_skinobject.h"
#include "Engine.h"
#include "LR2_skindraw.h"
#include "LR2_replay.h"
#include "LR2_audio.h"
#include "LR2_configsave.h"
#include "LR2_statplay.h"
#include "Scenes.h"

//40e790 // _mRet __mRet : return ret, ret : return !ret
bool GetOptionFlag_dst(game *gs, int option) {
	int t = 0;
	bool ret = (option >= 0);
	if (!ret) option = -option;


	switch (option) {
		case 0:
			return true;
		case 1:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 1) return ret;
			if (t == 2) return ret;
			if (t == 3) return ret;
			if (t == 4) return ret;
			if (t == 6) return ret;
			return !ret;
			break;

		case 2:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 0) return ret;
			if (t == 5) return ret;
			break;

		case 3:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 8) return ret;
			break;

		case 4:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 7) return ret;
			if (t == 9) return ret;
			break;

		case 5:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].folderType;
			if (t == 0) return ret;
			if (t == 5) return ret;
			if (t == 8) return ret;
			if (t == 9) return ret;
			break;

		case 10:
			t = gs->config.select.key;
			if (t == 4) return ret;
			if (t == 5) return ret;
			if (t == 6) return ret;
			t = gs->config.play.battle;
			if (t == 2) return ret;
			if (t == 3) return ret;
			break;

		case 11:
			t = gs->config.play.battle;
			if (t == 1) return ret;
			break;

		case 12:
			t = gs->config.select.key;
			if (t == 4) return ret;
			if (t == 5) return ret;
			if (t == 6) return ret;
			t = gs->config.play.battle;
			if (t == 1) return ret;
			if (t == 2) return ret;
			if (t == 3) return ret;
			break;

		case 13:
			t = gs->config.play.battle;
			if (t == 1) return ret;
			if (gs->gameplay.ghostBattle) return ret;
			break;

		case 20:
			if (gs->sSelect.panel == 0) return ret;
			if (gs->sSelect.panel == -1) return ret;
			break;

		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
			if (gs->sSelect.panel == option - 20) return ret;
			break;

		case 30:
			if (gs->config.play.bgasize == 0) return ret;
			break;
		case 31:
			if (gs->config.play.bgasize == 1) return ret;
			break;
		case 32:
			if (gs->procSelecter == 7) return ret;
			if (gs->gameplay.isAutoplay == 0) return ret;
			break;
		case 33:
			if (gs->procSelecter == 7) return ret;
			if (gs->gameplay.isAutoplay == 1) return ret;
			break;
		case 34:
			if (gs->config.play.play_ghost == 0) return ret;
			if (gs->config.play.autojudge == 2) return ret;
			break;
		case 35:
			if (gs->config.play.play_ghost == 1 && gs->config.play.autojudge != 2) return ret;
			break;
		case 36:
			if (gs->config.play.play_ghost == 2 && gs->config.play.autojudge != 2) return ret;
			break;
		case 37:
			if (gs->config.play.play_ghost == 3 && gs->config.play.autojudge != 2) return ret;
			break;
		case 38:
			if (gs->config.play.scoregraph == 0) return ret;
			break;
		case 39:
			if (gs->config.play.scoregraph == 1) return ret;
			break;
		case 40:
			if(gs->config.play.bga == 0) return ret;
			break;
		case 41:
			if (gs->config.play.bga == 1) return ret;
			if (gs->config.play.bga == 3) return ret;
			if (gs->config.play.bga == 2 && (gs->gameplay.isAutoplay == 1 || gs->gameplay.replay.status == 2)) return ret;
			break;

		case 42:
			if (gs->config.play.gaugeOption[0] != 0 && gs->config.play.gaugeOption[0] != 3) return !ret;
			if (gs->gameplay.isCourse == 0) return ret;
			break;
		case 43:
			if (gs->config.play.gaugeOption[0] != 0 && gs->config.play.gaugeOption[0] != 3) return ret;
			if (gs->gameplay.isCourse == 0) return !ret;
			return ret;
		case 44:
			if (gs->config.play.gaugeOption[1] != 0 && gs->config.play.gaugeOption[1] != 3) return !ret;
			if (gs->gameplay.isCourse == 0) return ret;
			break;
		case 45:
			if (gs->config.play.gaugeOption[1] != 0 && gs->config.play.gaugeOption[1] != 3) return ret;
			if (gs->gameplay.isCourse == 0) return !ret;
			return ret;

		case 46:
			if (gs->config.select.disabledifficultyfilter == 0) return ret;
			break;

		case 47:
			if (gs->config.select.disabledifficultyfilter == 1) return ret;
			break;

		case 50:
			if (gs->net.isOnline != 1) return ret;
			break;

		case 51:
			if (gs->net.isOnline == 1) return ret;
			break;

		case 52:
			if (gs->config.play.is_extra == 0) return ret;
			break;

		case 53:
			if (gs->config.play.is_extra == 1) return ret;
			break;

		case 54:
			if (gs->config.play.p1_assist == 0) return ret;
			break;

		case 55:
			if (gs->config.play.p1_assist == 1) return ret;
			break;

		case 56:
			if (gs->config.play.p2_assist == 0) return ret;
			break;

		case 57:
			if (gs->config.play.p2_assist == 1) return ret;
			break;

		case 60:
			if (CheckScoreSaveConditon(gs) == 0) return ret;
			break;
		case 61:
			if (CheckScoreSaveConditon(gs) == 1) return ret;
			break;
		case 62:
			if (CheckClearLampChallenge(gs) == 0) return ret;
			break;
		case 63:
			if (CheckClearLampChallenge(gs) == 1) return ret;
			break;
		case 64:
			if (CheckClearLampChallenge(gs) == 2) return ret;
			break;
		case 65:
			if (CheckClearLampChallenge(gs) == 3) return ret;
			break;
		case 66:
			if (CheckClearLampChallenge(gs) == 4) return ret;
			break;

		case 70:
		case 71:
		case 72:
		case 73:
		case 74:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].keymode;
			if (t == 5 || t == 10) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 70] <= gs->config.select.levelbarflash_5) return ret;
			}
			else if (t == 7 || t == 14) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 70] <= gs->config.select.levelbarflash_7) return ret;
			}
			else if (t == 9) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 70] <= gs->config.select.levelbarflash_9) return ret;
			}
			break;

		case 75:
		case 76:
		case 77:
		case 78:
		case 79:
			t = gs->sSelect.bmsList[gs->sSelect.cur_song].keymode;
			if (t == 5 || t == 10) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 75] > gs->config.select.levelbarflash_5) return ret;
			}
			else if (t == 7 || t == 14) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 75] > gs->config.select.levelbarflash_7) return ret;
			}
			else if (t == 9) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 75] > gs->config.select.levelbarflash_9) return ret;
			}
			break;

		case 80:
			if (GetTimeLapse(40, (gs->procSelecter == 7) ? &gs->timer2 : &gs->timer1) == -1.0)
				return ret;
			break;
		case 81:
			if (GetTimeLapse(40, (gs->procSelecter == 7) ? &gs->timer2 : &gs->timer1) != -1.0)
				return ret;
			break;
		case 82:
			if (gs->gameplay.replay.status == 0) return ret;
			break;
		case 83:
			if (gs->gameplay.replay.status == 1) return ret;
			break;
		case 84:
			if (gs->gameplay.replay.status == 2) return ret;
			break;

		case 90:
			if (gs->gameplay.player[0].clearType >= 2 && gs->skstruct.flag_flip == 0) return ret;
			if (gs->gameplay.player[1].clearType >= 2 && gs->skstruct.flag_flip != 0) return ret;
			break;
		case 91:
			if (gs->gameplay.player[0].clearType < 2 && gs->skstruct.flag_flip == 0) return ret;
			if (gs->gameplay.player[1].clearType < 2 && gs->skstruct.flag_flip != 0) return ret;
			break;
			
		case 100: //TODO : make more readable
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 0) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 0) return ret;
			}
			else if(gs->config.play.battle == 3){
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 0) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 0) return ret;
			}
			break;
		case 101:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 1) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 1) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 1) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 1) return ret;
			}
			break;
		case 102:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 2) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 2) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 2) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 2) return ret;
			}
			break;
		case 103:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 3) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 3) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 3) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 3) return ret;
			}
			break;
		case 104:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 4) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 4) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 4) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 4) return ret;
			}
			break;
		case 105:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_db == 5) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_ex == 5) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear_sd == 5) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear == 5) return ret;
			}
			break;

		case 110:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 7)
				return ret;
			break;
		
		case 111:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 6 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 8) return ret; //why???
			break;
		case 112:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 5 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 7) return ret;
			break;
		case 113:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 4 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 6) return ret;
			break;
		case 114:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 3 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 5) return ret;
			break;
		case 115:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 2 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 4) return ret;
			break;
		case 116:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode <= 0) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear < 1) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank > 1 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 3) return ret;
			break;
		case 117:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.clear > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.rank < 2)
				return ret;
			break;

		case 118:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 1) return ret;
			break;
		case 119:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 2) return ret;
			break;
		case 120:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 4) return ret;
			break;
		case 121:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 8) return ret;
			break;
		case 122:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x10) return ret;
			break;
		case 123:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x20) return ret;
			break;
		case 124:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x40) return ret;
			break;
		case 125:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x80) return ret;
			break;
		case 126:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x100) return ret;
			break;
		case 127:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x200) return ret;
			break;
		case 128:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x400) return ret;
			break;
		case 129:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x800) return ret;
			break;
		case 130:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x1000) return ret;
			break;
		case 131:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x2000) return ret;
			break;
		case 132:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x4000) return ret;
			break;
		case 133:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x8000) return ret;
			break;
		case 134:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x10000) return ret;
			break;
		case 135:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x20000) return ret;
			break;
		case 136:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x40000) return ret;
			break;
		case 137:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x80000) return ret;
			break;
		case 138:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x100000) return ret;
			break;
		case 139:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x200000) return ret;
			break;
		case 140:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x400000) return ret;
			break;
		case 141:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x800000) return ret;
			break;
		case 142:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x1000000) return ret;
			break;
		case 143:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x2000000) return ret;
			break;
		case 144:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x4000000) return ret;
			break;
		case 145:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x8000000) return ret;
			break;
		case 146:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x10000000) return ret;
			break;
		case 147:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x20000000) return ret;
			break;
		case 148:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x40000000) return ret;
			break;
		case 149:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.op_history & 0x80000000) return ret;
			break;

		case 150:
			if (gs->config.select.disabledifficultyfilter == 1) return ret;
			if (gs->sSelect.metaSelected.difficulty == 0) return ret;
			break;
		case 151:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 1) return ret;
			break;
		case 152:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 2) return ret;
			break;
		case 153:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 3) return ret;
			break;
		case 154:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 4) return ret;
			break;
		case 155:
			if (gs->config.select.disabledifficultyfilter) return !ret;
			if (gs->sSelect.metaSelected.difficulty == 5) return ret;
			break;

		case 160:
			if(gs->sSelect.metaSelected.keymode == 7) return ret;
			break;
		case 161:
			if (gs->sSelect.metaSelected.keymode == 5) return ret;
			break;
		case 162:
			if (gs->sSelect.metaSelected.keymode == 14) return ret;
			if (gs->sSelect.metaSelected.keymode == 7 && gs->config.play.battle > 1) return ret;
			break;
		case 163:
			if (gs->sSelect.metaSelected.keymode == 10) return ret;
			if (gs->sSelect.metaSelected.keymode == 5 && gs->config.play.battle > 1) return ret;
			break;
		case 164:
			if (gs->sSelect.metaSelected.keymode == 9) return ret;
			break;

		case 170:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].bga == 0) return ret;
			break;
		case 171:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].bga == 1)return ret;
			break;
		case 172:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].longnote == 0) return ret;
			break;
		case 173:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].longnote == 1)return ret;
			break;
		case 174:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].txt == 0)return ret;
			break;
		case 175:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].txt > 0) return ret;
			break;
		case 176:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].maxBPM == gs->sSelect.bmsList[gs->sSelect.cur_song].minBPM) return ret;
			break;
		case 177:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].maxBPM != gs->sSelect.bmsList[gs->sSelect.cur_song].minBPM) return ret;
			break;
		case 178:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].random == 0) return ret;
			break;
		case 179:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].random == 1) return ret;
			break;
			
		case 180:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].judge < 1) return ret;
			break;
		case 181:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].judge == 1) return ret;
			break;
		case 182:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].judge == 2) return ret;
			break;
		case 183:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].judge > 2) return ret;
			break;

		case 185:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 5 || gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 10) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level < 10) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 7 || gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 14) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level < 13) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 9) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level < 43) return ret;
			}
			break;
		case 186:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 5 || gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 10) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level > 9) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 7 || gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 14) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level > 12) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode == 9) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].level > 42) return ret;
			}
			break;

		case 190:
			if (gs->procSelecter == 7) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isStagefile == 0)return ret;
			break;
		case 191:
			if (gs->procSelecter == 7) return !ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isStagefile == 1)return ret;
			break;
		case 192:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isBanner == 0)return ret;
			break;
		case 193:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isBanner == 1)return ret;
			break;
		case 194:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isBackBMP == 0)return ret;
			break;
		case 195:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].isBackBMP == 1) return ret;
			break;
		case 196:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].replayExist == 0) return ret;
			break;
		case 197:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].replayExist == 1)return ret;
			break;

		case 200:
			if (gs->gameplay.player[0].note_current == 0) return !ret;
			if ((gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 8) return !ret;
			return ret;
		case 201:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 8
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 6)
				return ret;
			break;
		case 202:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 7
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 5)
				return ret;
			break;
		case 203:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 6
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 4)
				return ret;
			break;
		case 204:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 5
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 3)
				return ret;
			break;
		case 205:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 4
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 2)
				return ret;
			break;
		case 206:
			if (gs->gameplay.player[0].note_current
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) < 3
				&& (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) > 1)
				return ret;
			break;
		case 207:
			if (gs->gameplay.player[0].note_current == 0) return !ret;
			if ((gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].note_current * 2) >= 2) return !ret;
			return ret;

		case 210:
			if (gs->gameplay.player[1].note_current == 0) return !ret;
			if ((gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 8) return !ret;
			return ret;
		case 211:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 8
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 6)
				return ret;
			break;
		case 212:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 7
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 5)
				return ret;
			break;
		case 213:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 6
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 4)
				return ret;
			break;
		case 214:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 5
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 3)
				return ret;
			break;
		case 215:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 4
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 2)
				return ret;
			break;
		case 216:
			if (gs->gameplay.player[1].note_current
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) < 3
				&& (gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) > 1)
				return ret;
			break;
		case 217:
			if (gs->gameplay.player[1].note_current == 0) return !ret;
			if ((gs->gameplay.player[1].exscore * 9) / (gs->gameplay.player[1].note_current * 2) >= 2) return !ret;
			return ret;

		case 220:
		case 300:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if ((gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) < 8) return !ret;
			return ret;
		case 221:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 6) return ret;
			break;
		case 222:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 5) return ret;
			break;
		case 223:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 4) return ret;
			break;
		case 224:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 3) return ret;
			break;
		case 225:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 2) return ret;
			break;
		case 226:
			if (gs->gameplay.player[0].totalnotes && (gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 1) return ret;
			break;
		case 227:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if ((gs->gameplay.player[0].exscore * 9) / (gs->gameplay.player[0].totalnotes * 2) > 1) return !ret;
			return ret;

		case 230:
			if (gs->gameplay.player[0].HP < 0.0) return !ret;
			if (gs->gameplay.player[0].HP < 10.0) return ret;
			break;
		case 231:
			if (gs->gameplay.player[0].HP < 10.0) return !ret;
			if (gs->gameplay.player[0].HP < 20.0) return ret;
			break;
		case 232:
			if (gs->gameplay.player[0].HP < 20.0) return !ret;
			if (gs->gameplay.player[0].HP < 30.0) return ret;
			break;
		case 233:
			if (gs->gameplay.player[0].HP < 30.0) return !ret;
			if (gs->gameplay.player[0].HP < 40.0) return ret;
			break;
		case 234:
			if (gs->gameplay.player[0].HP < 40.0) return !ret;
			if (gs->gameplay.player[0].HP < 50.0) return ret;
			break;
		case 235:
			if (gs->gameplay.player[0].HP < 50.0) return !ret;
			if (gs->gameplay.player[0].HP < 60.0) return ret;
			break;
		case 236:
			if (gs->gameplay.player[0].HP < 60.0) return !ret;
			if (gs->gameplay.player[0].HP < 70.0) return ret;
			break;
		case 237:
			if (gs->gameplay.player[0].HP < 70.0) return !ret;
			if (gs->gameplay.player[0].HP < 80.0) return ret;
			break;
		case 238:
			if (gs->gameplay.player[0].HP < 80.0) return !ret;
			if (gs->gameplay.player[0].HP < 90.0) return ret;
			break;
		case 239:
			if (gs->gameplay.player[0].HP < 90.0) return !ret;
			if (gs->gameplay.player[0].HP < 100.0) return ret;
			break;
		case 240:
			if (gs->gameplay.player[0].HP >= 100.0) return ret;
			break;
		case 241:
			if (gs->gameplay.player[0].judge_draw == 5) return ret;
			break;
		case 242:
			if (gs->gameplay.player[0].judge_draw == 4) return ret;
			break;
		case 243:
			if (gs->gameplay.player[0].judge_draw == 3) return ret;
			break;
		case 244:
			if (gs->gameplay.player[0].judge_draw == 2) return ret;
			break;
		case 245:
			if (gs->gameplay.player[0].judge_draw == 1) return ret;
			break;
		case 246:
			if (gs->gameplay.player[0].judge_draw == 0) return ret;
			break;
		case 247:
			if (GetTimeWrap() - gs->gameplay.misslayerTime[0] >= 1000.0) return ret;
			break;
		case 248:
			if (!(GetTimeWrap() - gs->gameplay.misslayerTime[0] >= 1000.0)) return ret;
			break;

		case 250:
			if (gs->gameplay.player[1].HP < 0.0) return !ret;
			if (gs->gameplay.player[1].HP < 10.0) return ret;
			break;
		case 251:
			if (gs->gameplay.player[1].HP < 10.0) return !ret;
			if (gs->gameplay.player[1].HP < 20.0) return ret;
			break;
		case 252:
			if (gs->gameplay.player[1].HP < 20.0) return !ret;
			if (gs->gameplay.player[1].HP < 30.0) return ret;
			break;
		case 253:
			if (gs->gameplay.player[1].HP < 30.0) return !ret;
			if (gs->gameplay.player[1].HP < 40.0) return ret;
			break;
		case 254:
			if (gs->gameplay.player[1].HP < 40.0) return !ret;
			if (gs->gameplay.player[1].HP < 50.0) return ret;
			break;
		case 255:
			if (gs->gameplay.player[1].HP < 50.0) return !ret;
			if (gs->gameplay.player[1].HP < 60.0) return ret;
			break;
		case 256:
			if (gs->gameplay.player[1].HP < 60.0) return !ret;
			if (gs->gameplay.player[1].HP < 70.0) return ret;
			break;
		case 257:
			if (gs->gameplay.player[1].HP < 70.0) return !ret;
			if (gs->gameplay.player[1].HP < 80.0) return ret;
			break;
		case 258:
			if (gs->gameplay.player[1].HP < 80.0) return !ret;
			if (gs->gameplay.player[1].HP < 90.0) return ret;
			break;
		case 259:
			if (gs->gameplay.player[1].HP < 90.0) return !ret;
			if (gs->gameplay.player[1].HP < 100.0) return ret;
			break;
		case 260:
			if (gs->gameplay.player[1].HP >= 100.0) return ret;
			break;
		case 261:
			if (gs->gameplay.player[1].judge_draw == 5) return ret;
			break;
		case 262:
			if (gs->gameplay.player[1].judge_draw == 4) return ret;
			break;
		case 263:
			if (gs->gameplay.player[1].judge_draw == 3) return ret;
			break;
		case 264:
			if (gs->gameplay.player[1].judge_draw == 2) return ret;
			break;
		case 265:
			if (gs->gameplay.player[1].judge_draw == 1) return ret;
			break;
		case 266:
			if (gs->gameplay.player[1].judge_draw == 0) return ret;
			break;
		case 267:
			if (GetTimeWrap() - gs->gameplay.misslayerTime[1] >= 1000.0) return ret;
			break;
		case 268:
			if (!(GetTimeWrap() - gs->gameplay.misslayerTime[1] >= 1000.0)) return ret;
			break;

		case 270:
			if (gs->procSelecter == 4) {
				if ((gs->KeyInput.p1_buttonInput[12] == 2 || gs->KeyInput.p1_buttonInput[13] == 2) && gs->config.play.p1_lanecover && gs->gameplay.lanecoverDisplayP1) return ret;
				if ((gs->KeyInput.p2_buttonInput[12] == 2 || gs->KeyInput.p2_buttonInput[13] == 2) && gs->config.play.p1_lanecover && gs->gameplay.lanecoverDisplayP1 && gs->config.play.battle != 1 && gs->config.play.battle != 4) return ret;
			}
			break;
		case 271:
			if (gs->procSelecter == 4) {
				if (gs->KeyInput.p2_buttonInput[12] == 2) return ret;
				if (gs->KeyInput.p2_buttonInput[13] == 2) return ret;
			}
			break;

		case 280:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 0 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 281:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 1 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 282:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 2 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 283:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 3 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 284:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 4 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 285:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 5 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 286:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 6 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 287:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 7 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 288:
			if ((gs->gameplay.courseStageNow != gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageNow == 8 && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 289:
			if ((gs->gameplay.courseStageNow == gs->gameplay.courseStageCount - 1) && gs->gameplay.courseStageCount > 0) return ret;
			break;
		case 290:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseType != -1) return ret;
			break;
		case 291:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseType == 1) return ret;
			break;
		case 292:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseType == 0) return ret;
			break;
		case 293:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseType == 2) return ret;
			break;

			//300 is smae 220
		case 301:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 7) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 7) return !ret;
			return ret;
			break;
		case 302:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 6) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 6) return !ret;
			return ret;
			break;
		case 303:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 5) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 5) return !ret;
			return ret;
			break;
		case 304:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 4) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 4) return !ret;
			return ret;
			break;
		case 305:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 3) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 3) return !ret;
			return ret;
			break;
		case 306:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 2) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) < 2) return !ret;
			return ret;
			break;
		case 307:
			if (gs->gameplay.player[0].totalnotes == 0) return !ret;
			if (gs->gameplay.player[0].exscore == 0) return !ret;
			if (gs->gameplay.player[0].exscore * 9 / (gs->gameplay.player[0].totalnotes * 2) > 1) return !ret;
			return ret;
			break;
		case 308:
			if (gs->gameplay.player[0].exscore == 0) return ret;
			if (gs->gameplay.player[0].totalnotes == 0) return ret;
			break;

		case 310:
			if (gs->gameplay.player[1].totalnotes == 0) return !ret;
			if (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2) < 8) return !ret;
			return ret;
			break;
		case 311:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 8
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 6) return ret;
			break;
		case 312:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 7
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 5) return ret;
			break;
		case 313:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 6
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 4) return ret;
			break;
		case 314:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 5
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 3) return ret;
			break;
		case 315:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 4
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 2) return ret;
			break;
		case 316:
			if (gs->gameplay.player[1].totalnotes != 0
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) < 3
				&& (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2)) > 1) return ret;
			break;
		case 317:
			if (gs->gameplay.player[1].totalnotes == 0) return !ret;
			if (gs->gameplay.player[1].exscore == 0) return !ret;
			if (gs->gameplay.player[1].exscore * 9 / (gs->gameplay.player[1].totalnotes * 2) > 1) return !ret;
			return ret;
		case 318:
			if (gs->gameplay.player[1].exscore == 0) return ret;
			if (gs->gameplay.player[1].totalnotes == 0) return ret;
			break;

		case 320:
			if (gs->sSelect.old.total_notes == 0) return !ret;
			if (gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 8) return !ret;
			return ret;
			break;
		case 321:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 6
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 8) return ret;
			break;
		case 322:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 5
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 7) return ret;
			break;
		case 323:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 4
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 6) return ret;
			break;
		case 324:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 3
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 5) return ret;
			break;
		case 325:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 2
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 4) return ret;
			break;
		case 326:
			if (gs->sSelect.old.total_notes != 0
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 1
				&& gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) < 3) return ret;
			break;
		case 327:
			if (gs->sSelect.old.total_notes == 0) return !ret;
			if (gs->sSelect.old.stat_exscore * 9 / (gs->sSelect.old.total_notes * 2) > 1) return !ret;
			return ret;
			break;

		case 330:
			if (gs->skstruct.flag_flip == 0) {
				if (gs->gameplay.player[0].exscore <= gs->sSelect.old.stat_exscore) return !ret;
				return ret;
			}
			if (gs->gameplay.player[1].exscore > gs->sSelect.old.stat_exscore) return ret;
			break;
		case 331:
			if (gs->skstruct.flag_flip == 0) {
				if (gs->gameplay.player[0].max_combo > gs->sSelect.old.stat_maxcombo) {
					return ret;
				}
			}
			else if (gs->gameplay.player[1].max_combo > gs->sSelect.old.stat_maxcombo) {
				return ret;
			}
			break;
		case 332:
			if (gs->skstruct.flag_flip == 0) {
				if (gs->gameplay.player[0].totalnotes - gs->gameplay.player[0].note_current + gs->gameplay.player[0].judgecount[2] + gs->gameplay.player[0].judgecount[1] < gs->sSelect.old.minbp) return ret;
			}
			else if (gs->gameplay.player[1].totalnotes - gs->gameplay.player[1].note_current + gs->gameplay.player[1].judgecount[2] + gs->gameplay.player[1].judgecount[1] < gs->sSelect.old.minbp) return ret;
			break;
		case 333:
			if (gs->gameplay.trialClear) return ret;
			break;
		case 334:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.IRranking == 0) {
				if (gs->sSelect.oldIRrank > 0) return ret;
			}
			else if (gs->sSelect.bmsList[gs->sSelect.cur_song].mybest.IRranking < gs->sSelect.oldIRrank) return ret;
			break;
		case 335:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes != 0
				&& gs->sSelect.old.total_notes != 0
				&& (gs->sSelect.old.stat_exscore*9 / (gs->sSelect.old.total_notes*2) < gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes*2))) 
				return ret;
			break;

		case 340:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 8) return !ret;
			return ret;
			break;
		case 341:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 7) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 7) return !ret;
			return ret;
			break;
		case 342:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 6) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 6) return !ret;
			return ret;
			break;
		case 343:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 5) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 5) return !ret;
			return ret;
			break;
		case 344:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 4) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 4) return !ret;
			return ret;
			break;
		case 345:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 3) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 3) return !ret;
			return ret;
			break;
		case 346:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) < 2) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 2) return !ret;
			return ret;
			break;
		case 347:
			if (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes == 0) return !ret;
			if (gs->gameplay.player[gs->skstruct.flag_flip].exscore * 9 / (gs->gameplay.player[gs->skstruct.flag_flip].totalnotes * 2) > 1) return !ret;
			return ret;
			break;

		case 350:
			if (gs->skstruct.flag_flip == 0) return ret;
			break;
		case 351:
			if (gs->skstruct.flag_flip != 0) return ret;
			break;
		case 352:
			if (gs->gameplay.player[0].exscore > gs->gameplay.player[1].exscore) return ret;
			break;
		case 353:
			if (gs->gameplay.player[0].exscore < gs->gameplay.player[1].exscore) return ret;
			break;
		case 354:
			if (gs->gameplay.player[0].exscore == gs->gameplay.player[1].exscore) return ret;
			break;

		case 400:
			if (gs->KeyInput.config_keymode == 0) return ret;
			break;
		case 401:
			if (gs->KeyInput.config_keymode == 1) return ret;
			break;
		case 402:
			if (gs->KeyInput.config_keymode == 2) return ret;
			break;

		case 500:
		case 501:
		case 502:
		case 503:
		case 504:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 500] == -1)
				return ret;
			break;
		case 505:
		case 506:
		case 507:
		case 508:
		case 509:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevel[option - 505] != -1)
				return ret;
			break;
		case 510:
		case 511:
		case 512:
		case 513:
		case 514:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyExist[option - 510] == 1)
				return ret;
			break;
		case 515:
		case 516:
		case 517:
		case 518:
		case 519:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyExist[option - 515] > 1)
				return ret;
			break;
		case 520:
		case 521:
		case 522:
		case 523:
		case 524:
		case 525:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[0] == option - 520)
				return ret;
			break;
		case 530:
		case 531:
		case 532:
		case 533:
		case 534:
		case 535:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[1] == option - 530)
				return ret;
			break;
		case 540:
		case 541:
		case 542:
		case 543:
		case 544:
		case 545:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[2] == option - 540)
				return ret;
			break;
		case 550:
		case 551:
		case 552:
		case 553:
		case 554:
		case 555:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[3] == option - 550)
				return ret;
			break;
		case 560:
		case 561:
		case 562:
		case 563:
		case 564:
		case 565:
			if ((gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 0 || gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 5) && gs->config.play.is_extra != 1 && gs->config.play.battle != 2 && gs->config.play.battle != 3
				&& gs->sSelect.bmsList[gs->sSelect.cur_song].difficultyLevelBarLamp[4] == option - 560)
				return ret;
			break;

		case 570: //TOFIX : forgot break?
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 7)return ret;
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].folderType == 9)return ret;
		case 580:
		case 581:
		case 582:
		case 583:
		case 584:
		case 585:
		case 586:
		case 587:
		case 588:
		case 589:
			if (gs->sSelect.course.isMakingCourse == 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageCount > option - 580) return ret;
			break;
		case 571: //TOFIX : forgot break?
			if (gs->sSelect.course.isMakingCourse == 1) return ret;
		case 572: //TOFIX : forgot break?
			if (gs->sSelect.course.isMakingCourse == 0) return ret;
			if(gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[0] == option - 700) return ret;
			break;
			
		case 590:
		case 591:
		case 592:
		case 593:
		case 594:
		case 595:
		case 596:
		case 597:
		case 598:
		case 599:
			if (gs->sSelect.course.isMakingCourse != 1) return !ret;
			if (gs->sSelect.course.count == option - 590) return ret;
			break;

		case 600:
			if (gs->net.IRstatus == 0) return ret;
			break;
		case 601:
			if (gs->net.IRstatus == 1) return ret;
			break;
		case 602:
			if (gs->net.IRstatus == 2) return ret;
			break;
		case 603:
			if (gs->net.IRstatus == -1) return ret;
			break;
		case 604:
			if (gs->net.IRstatus == -3) return ret;
			break;
		case 605:
			if (gs->net.IRstatus == -2) return ret;
			break;
		case 606:
			if (gs->net.IRstatus == 3) return ret;
			break;
		case 607:
			if (gs->net.IRstatus == 4) return ret;
			break;
		case 608:
			if (gs->net.IRstatus == 5) return ret;
			break;

		case 610: //TOFIX : seems like meaningless...
			if (gs->net.rankingData.rankingCount == 0) return ret;
			break;
		case 611:
			if (gs->net.rankingData.rankingCount > 0) return ret;
			break;
		case 612:
			if (gs->net.rankingData.rankingCount > 1) return ret;
			break;
		case 613:
			if (gs->net.rankingData.rankingCount > 2) return ret;
			break;
		case 614:
			if (gs->net.rankingData.rankingCount > 3) return ret;
			break;
		case 615:
			if (gs->net.rankingData.rankingCount > 4) return ret;
			break;
		case 616:
			if (gs->net.rankingData.rankingCount > 5) return ret;
			break;

		case 620:
			if (gs->net.rankingData.showRanking == 0) return ret;
			break;
		case 621:
			if (gs->net.rankingData.showRanking != 0) return ret;
			break;
		case 622:
			if (gs->gameplay.ghostBattle == 0) return ret;
			break;
		case 623: //original place is between case 13 and 20
			if (gs->gameplay.ghostBattle != 0) return ret;
			break;

		case 624:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.stat_exscore == 0) return ret;
			break;
		case 625:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.stat_exscore > 0) return ret;
			break;

		case 640:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 0) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 0) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 0) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 0) return ret;
			}
			break;
		case 641:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 1) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 1) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 1) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 1) return ret;
			}
			break;
		case 642:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 2) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 2) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 2) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 2) return ret;
			}
			break;
		case 643:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 3) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 3) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 3) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 3) return ret;
			}
			break;
		case 644:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 4) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 4) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 4) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 4) return ret;
			}
			break;
		case 645:
			if (gs->config.play.battle == 2) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_db == 5) return ret;
			}
			else if (gs->config.play.is_extra == 1) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_ex == 5) return ret;
			}
			else if (gs->config.play.battle == 3) {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear_sd == 5) return ret;
			}
			else {
				if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear == 5) return ret;
			}
			break;
			
		case 650:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank >= 8) return ret;
			break;
		case 651:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 7 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 8) return ret;
			break;
		case 652:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 6 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 7) return ret;
			break;
		case 653:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 5 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 6) return ret;
			break;
		case 654:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 4 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 5) return ret;
			break;
		case 655:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 3 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 4) return ret;
			break;
		case 656:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && 2 <= gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 3) return ret;
			break;
		case 657:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].keymode > 0 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.clear >= 1 && gs->sSelect.bmsList[gs->sSelect.cur_song].rivalRecord.rank < 2) return ret;
			break;

		case 700:
		case 701:
		case 702:
		case 703:
		case 704:
		case 705:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[0] == option - 700) return ret;
			break;
		case 710:
		case 711:
		case 712:
		case 713:
		case 714:
		case 715:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[1] == option - 710) return ret;
			break;
		case 720:
		case 721:
		case 722:
		case 723:
		case 724:
		case 725:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[2] == option - 720) return ret;
			break;
		case 730:
		case 731:
		case 732:
		case 733:
		case 734:
		case 735:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[3] == option - 730) return ret;
			break;
		case 740:
		case 741:
		case 742:
		case 743:
		case 744:
		case 745:
			if (gs->sSelect.bmsList[gs->sSelect.cur_song].courseStageDifficulty[4] == option - 740) return ret;
			break;

		case 800:
		case 801:
		case 802:
		case 803:
		case 804:
		case 805:
		case 806:
		case 807:
		case 808:
		case 809:
			if (gs->po4cur_song == option - 800) return ret;
			break;

		case 810:
		case 811:
		case 812:
		case 813:
		case 814:
		case 815:
		case 816:
		case 817:
		case 818:
		case 819:
		case 820:
		case 821:
		case 822:
		case 823:
		case 824:
		case 825:
		case 826:
		case 827:
		case 828:
		case 829:
			if (gs->po4procSelecter == option - 810) return ret;
			break;
		case 830:
			if (gs->gameplay.bmsResourceLoaded) return ret;
			break;

		default:
			if (option - 900 < 100) {
				if (gs->procSelecter == 7) {
					if (gs->skstruct2.op[option]) return ret;
				}
				else if (gs->skstruct.op[option]) return ret;
			}
			break;
	}

	return !ret;
}


//4024d0
uint SetObjectValue_Num(game *g, int op) {
	DATEDATA Date;

	switch (op) {
		case 10:
			return g->config.play.hiSpeed[0];
		case 11:
			return g->config.play.hiSpeed[1];
		case 12:
			return g->config.play.judgetiming;
		case 13:
			return g->config.play.target_percent;
		case 14:
			return g->config.play.p1_lanecoverv;
		case 15:
			return g->config.play.p2_lanecoverv;
		default:
			return -1;

		case 20:
			return g->timer1.FPS;
		case 21:
			GetDateTime(&Date);
			return Date.Year;
		case 22:
			GetDateTime(&Date);
			return Date.Mon;
		case 23:
			GetDateTime(&Date);
			return Date.Day;
		case 24:
			GetDateTime(&Date);
			return Date.Hour;
		case 25:
			GetDateTime(&Date);
			return Date.Min;
		case 26:
			GetDateTime(&Date);
			return Date.Sec;

		case 30:
			return g->gameplay.playerstat.playcount;
		case 31:
			return g->gameplay.playerstat.clear;
		case 32:
			return g->gameplay.playerstat.fail;
		case 33:
			return g->gameplay.playerstat.perfect;
		case 34:
			return g->gameplay.playerstat.great;
		case 35:
			return g->gameplay.playerstat.good;
		case 36:
			return g->gameplay.playerstat.bad;
		case 37:
			return g->gameplay.playerstat.poor;
		case 38:
			return g->gameplay.playerstat.combo;
		case 39:
			return g->gameplay.playerstat.maxcombo;
		case 40:
			return g->gameplay.playerstat.trial;
		case 41:
			return g->gameplay.playerstat.trial - 1;
		case 42:
			return g->sSelect.bmsList[g->sSelect.cur_song].level;
		case 45:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[0];
		case 46:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[1];
		case 47:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[2];
		case 48:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[3];
		case 49:
			return g->sSelect.bmsList[g->sSelect.cur_song].difficultyLevel[4];

		case 50:
			return g->audio.param.eq_gain[0];
		case 51:
			return g->audio.param.eq_gain[1];
		case 52:
			return g->audio.param.eq_gain[2];
		case 53:
			return g->audio.param.eq_gain[3];
		case 54:
			return g->audio.param.eq_gain[4];
		case 55:
			return g->audio.param.eq_gain[5];
		case 56:
			return g->audio.param.eq_gain[6];
		case 57:
			return g->audio.param.volume_master;
		case 58:
			return g->audio.param.volume_key;
		case 59:
			return g->audio.param.volume_BGM;
		case 60:
			return g->audio.param.fxParam[0][0];
		case 61:
			return g->audio.param.fxParam[0][1];
		case 62:
			return g->audio.param.fxParam[1][0];
		case 63:
			return g->audio.param.fxParam[1][1];
		case 64:
			return g->audio.param.fxParam[2][0];
		case 65:
			return g->audio.param.fxParam[2][1];
		case 66:
			return g->audio.param.pitch_amount;

		case 70:
			if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14)
				return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score; 
			else 
				return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score / 20 * 10;
		case 71:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore;
		case 72:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes * 2;
		case 73:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore * 100) / (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes * 2);
			}
			break;
		case 74:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
		case 75:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo;
		case 76:
			return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp != -1) * g->sSelect.bmsList[g->sSelect.cur_song].mybest.minbp;
		case 77:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount;
		case 78:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount;
		case 79:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.playcount - g->sSelect.bmsList[g->sSelect.cur_song].mybest.clearcount;
		case 80:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat;
		case 81:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great;
		case 82:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good;
		case 83:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad;
		case 84:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor;
		case 85:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes) 
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 86:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes)
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 87:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes)
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 88:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes)
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 89:
			if (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes)
				return (g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor * 100) / g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
			break;
		case 90:
		case 290:
			return (g->sSelect.bmsList[g->sSelect.cur_song].maxBPM) ? g->sSelect.bmsList[g->sSelect.cur_song].maxBPM : -1;
		case 91:
		case 291:
			return (g->sSelect.bmsList[g->sSelect.cur_song].minBPM) ? g->sSelect.bmsList[g->sSelect.cur_song].minBPM : -1;
		case 92:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRranking;
		case 93:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRplayercount;
		case 94:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.IRclearRate;
		case 95:
			return g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore - g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_exscore;
		case 96:
			return g->sSelect.bmsList[g->sSelect.cur_song].level;

		case 100:
			if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14)
				return g->gameplay.player[0].score_print;
			else
				return g->gameplay.player[0].score_print / 20 * 10;
		case 101:
			return g->gameplay.player[0].exscore;
		case 102:
			return g->gameplay.player[0].rate;
		case 103:
			return (int)((g->gameplay.player[0].rate) * 100.0) % 100;
		case 104:
			if ((g->gameplay.courseType == 0 || g->gameplay.courseType == 2) && g->procSelecter == 4) {
				return g->gameplay.player[0].now_combo_course;
			}
			return g->gameplay.player[0].now_combo;
		case 105:
			if ((g->gameplay.courseType == 0 || g->gameplay.courseType == 2) && g->procSelecter == 4) {
				return g->gameplay.player[0].max_combo_course;
			}
			return g->gameplay.player[0].max_combo;
		case 106:
			return g->gameplay.player[0].totalnotes;
		case 107:
			return (int)g->gameplay.player[0].HP_print / 2 * 2;
		case 108:
			return g->gameplay.player[0].exscore - g->gameplay.player[1].exscore;
		case 109:
		case 154:{
			if (g->gameplay.player[0].exscore == g->gameplay.player[0].totalnotes * 2) return 0;
			if (g->gameplay.player[0].totalnotes == 0) return 0;
			
			int rank = (g->gameplay.player[0].exscore * 9) / (g->gameplay.player[0].totalnotes * 2);

			if (rank < 1) rank = 1;
			else if (rank > 8) rank = 8;
			return g->gameplay.player[0].exscore - (g->gameplay.player[0].totalnotes * 2 * (rank + 1) / 9);
		}
		case 110:
			return g->gameplay.player[0].judgecount[5];
		case 111:
			return g->gameplay.player[0].judgecount[4];
		case 112:
			return g->gameplay.player[0].judgecount[3];
		case 113:
			return g->gameplay.player[0].judgecount[2];
		case 114:
			return g->gameplay.player[0].judgecount[0] + g->gameplay.player[0].judgecount[1];
		case 115:
			if (g->gameplay.player[0].totalnotes > 0) {
				return g->gameplay.player[0].exscore * 100 / (g->gameplay.player[0].totalnotes * 2);
			}
			break;
		case 116:
			if (g->gameplay.player[0].totalnotes > 0) {
				return g->gameplay.player[0].exscore * 10000 / (g->gameplay.player[0].totalnotes * 2) % 100;
			}
			break;

		case 120:
			if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14)
				return g->gameplay.player[1].score_print;
			else
				return g->gameplay.player[1].score_print / 20 * 10;
		case 121:
			return g->gameplay.player[1].exscore;
		case 122:
			return g->gameplay.player[1].rate;
		case 123:
			return (int)((g->gameplay.player[1].rate) * 100.0) % 100;
		case 124:
			if ((g->gameplay.courseType == 0 || g->gameplay.courseType == 2) && g->procSelecter == 4) {
				return g->gameplay.player[1].now_combo_course;
			}
			return g->gameplay.player[1].now_combo;
		case 125:
			if ((g->gameplay.courseType == 0 || g->gameplay.courseType == 2) && g->procSelecter == 4) {
				return g->gameplay.player[1].max_combo_course;
			}
			return g->gameplay.player[1].max_combo;
		case 126:
			return g->gameplay.player[1].totalnotes;
		case 127:
			return (int)g->gameplay.player[1].HP_print / 2 * 2;
		case 128:
			return g->gameplay.player[1].exscore - g->gameplay.player[0].exscore;
		case 129:{
			if (g->gameplay.player[1].exscore == g->gameplay.player[1].totalnotes * 2) return 0;
			if (g->gameplay.player[1].totalnotes == 0) return 0;

			int rank = (g->gameplay.player[1].exscore * 9) / (g->gameplay.player[1].totalnotes * 2);

			if (rank < 1) rank = 1;
			else if (rank > 8) rank = 8;
			return g->gameplay.player[1].exscore - (g->gameplay.player[1].totalnotes * 2 * (rank + 1) / 9);
		}
		case 130:
			return g->gameplay.player[1].judgecount[5];
		case 131:
			return g->gameplay.player[1].judgecount[4];
		case 132:
			return g->gameplay.player[1].judgecount[3];
		case 133:
			return g->gameplay.player[1].judgecount[2];
		case 134:
			return g->gameplay.player[1].judgecount[0] + g->gameplay.player[1].judgecount[1];
		case 135:
			if (g->gameplay.player[1].totalnotes > 0) {
				return g->gameplay.player[1].exscore * 100 / (g->gameplay.player[1].totalnotes * 2);
			}
			break;
		case 136:
			if (g->gameplay.player[1].totalnotes > 0) {
				return g->gameplay.player[1].exscore * 10000 / (g->gameplay.player[1].totalnotes * 2) % 100;
			}
			break;

		case 150:
			return g->gameplay.highScore.rate;
		case 151:
			return g->gameplay.targetScore.rate;
		case 152:
			return g->gameplay.player[0].exscore - g->gameplay.highScore.rate;
		case 153:
			return g->gameplay.player[0].exscore - g->gameplay.targetScore.rate;
			// 154 is same 109
		case 155:
			if (g->gameplay.player[0].totalnotes) {
				return g->gameplay.highScore.rate * 100 / (g->gameplay.player[0].totalnotes * 2);
			}
			break;
		case 156:
			if (g->gameplay.player[0].totalnotes) {
				return g->gameplay.highScore.rate * 10000 / (g->gameplay.player[0].totalnotes * 2) % 100;
			}
			break;
		case 157:
			if (g->gameplay.player[0].totalnotes) {
				return g->gameplay.targetScore.rate * 100 / (g->gameplay.player[0].totalnotes * 2);
			}
			break;
		case 158:
			if (g->gameplay.player[0].totalnotes) {
				return g->gameplay.targetScore.rate * 10000 / (g->gameplay.player[0].totalnotes * 2) % 100;
			}
			break;

		case 160:
			if (g->gameplay.BPM <= 0.0) return 1 + GetRand(998);
			else return g->gameplay.BPM;
		case 161:{
			int time = GetTimeLapse(41, &g->timer1);
			if (time / 1000 > 0) return time / 1000 / 60;
			break;
		}
		case 162:{
			int time = GetTimeLapse(41, &g->timer1);
			if (time / 1000 > 0) return time / 1000 % 60;
			break;
		}
		case 163:{
			int time = GetTimeLapse(41, &g->timer1) - g->gameplay.song_runtime;
			if (time / 1000 > 0) return time / 1000 / 60;
			break;
		}
		case 164:{
			int time = GetTimeLapse(41, &g->timer1) - g->gameplay.song_runtime;
			if (time / 1000 > 0) return time / 1000 % 60;
			break;
		}
		case 165:
			if (g->gameplay.loadObject_total > 0)
				return g->gameplay.loadObject_loaded * 100 / g->gameplay.loadObject_total;
			break;

		case 170:
			return g->sSelect.old.stat_exscore;
		case 171:
			if (g->skstruct.flag_flip) return g->gameplay.player[1].exscore;
			else return g->gameplay.player[0].exscore;
		case 172:
			if (g->skstruct.flag_flip) return g->gameplay.player[1].exscore - g->sSelect.old.stat_exscore;
			else return g->gameplay.player[0].exscore - g->sSelect.old.stat_exscore;
		case 173:
			return g->sSelect.old.stat_maxcombo;
		case 174:
			if (g->skstruct.flag_flip) return g->gameplay.player[1].max_combo;
			else return g->gameplay.player[0].max_combo;
		case 175:
			if (g->skstruct.flag_flip) return g->gameplay.player[1].max_combo- g->sSelect.old.stat_maxcombo;
			else return g->gameplay.player[0].max_combo - g->sSelect.old.stat_maxcombo;
		case 176:
			if (g->sSelect.old.minbp < 0) return 0;
			return g->sSelect.old.minbp;
		case 177:
			if (g->skstruct.flag_flip == 0) 
				return g->gameplay.player[0].totalnotes - g->gameplay.player[0].note_current + g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1];
			else
				return g->gameplay.player[1].totalnotes - g->gameplay.player[1].note_current + g->gameplay.player[1].judgecount[2] + g->gameplay.player[1].judgecount[1];
		case 178:{
			int oldbp = g->sSelect.old.minbp;
			if (oldbp < 0) oldbp = 0;
			if (g->skstruct.flag_flip == 0)
				return g->gameplay.player[0].totalnotes - g->gameplay.player[0].note_current + g->gameplay.player[0].judgecount[2] + g->gameplay.player[0].judgecount[1] - oldbp;
			else
				return g->gameplay.player[1].totalnotes - g->gameplay.player[1].note_current + g->gameplay.player[1].judgecount[2] + g->gameplay.player[1].judgecount[1] - oldbp;
		}
		case 179:
			return g->net.rankingData.myRanking;
		case 180:
		case 293:
			return g->net.rankingData.rankingCount;
		case 181:
			if (g->net.rankingData.rankingCount > 0)
				return (g->net.rankingData.clearPlayers[5] + g->net.rankingData.clearPlayers[4] + g->net.rankingData.clearPlayers[3] + g->net.rankingData.clearPlayers[2]) * 100 / g->net.rankingData.rankingCount;
			break;
		case 182:
			return g->sSelect.oldIRrank;
		case 183:
			if (g->skstruct.flag_flip == 0) {
				if (g->gameplay.player[0].totalnotes > 0) return g->sSelect.old.stat_exscore * 100 / (g->gameplay.player[0].totalnotes * 2);
				else return 0;
			}
			else{
				if (g->gameplay.player[1].totalnotes > 0) return g->sSelect.old.stat_exscore * 100 / (g->gameplay.player[1].totalnotes * 2);
				else return 0;
			}
		case 184:
			if (g->skstruct.flag_flip == 0) {
				if (g->gameplay.player[0].totalnotes > 0) return g->sSelect.old.stat_exscore * 10000 / (g->gameplay.player[0].totalnotes * 2) % 100;
				else return 0;
			}
			else {
				if (g->gameplay.player[1].totalnotes > 0) return g->sSelect.old.stat_exscore * 10000 / (g->gameplay.player[1].totalnotes * 2) % 100;
				else return 0;
			}

		case 200:
			return g->net.rankingData.rankingCount;
		case 201:
			return g->net.rankingData.totalPlaycount;

		case 210:
			return g->net.rankingData.clearPlayers[1];
		case 211:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[1] * 100) / g->net.rankingData.rankingCount;
			}
			break;
		case 212:
			return g->net.rankingData.clearPlayers[2];
		case 213:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[2] * 100) / g->net.rankingData.rankingCount;
			}
			break;
		case 214:
			return g->net.rankingData.clearPlayers[3];
		case 215:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[3] * 100) / g->net.rankingData.rankingCount;
			}
			break;
		case 216:
			return g->net.rankingData.clearPlayers[4];
		case 217:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[4] * 100) / g->net.rankingData.rankingCount;
			}
			break;
		case 218:
			return g->net.rankingData.clearPlayers[5];
		case 219:
			if (g->net.rankingData.rankingCount != 0) {
				return (g->net.rankingData.clearPlayers[5] * 100) / g->net.rankingData.rankingCount;
			}
			break;

		case 220:
			if (g->net.IRstatus != 3) return  -1;
			return (g->net.waitTime - GetTimeLapse(177, &g->timer1)) / 1000.0;

		case 250:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[0];
		case 251:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[1];
		case 252:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[2];
		case 253:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[3];
		case 254:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[4];
		case 255:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[5];
		case 256:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[6];
		case 257:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[7];
		case 258:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[8];
		case 259:
			return g->sSelect.bmsList[g->sSelect.cur_song].courseLevel[9];

		case 270:
			if ((g->sSelect.metaSelected.keymode != 7) && (g->sSelect.metaSelected.keymode != 14)) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_score / 20) * 10;
			}
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_score;
		case 271:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_exscore;
		case 272:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes * 2;
		case 273:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0)
				return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_exscore * 100 / (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes * 2);
			break;
		case 274:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
		case 275:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_maxcombo;
		case 276:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.minbp == -1) return 0;
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.minbp;
		case 277:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.playcount;
		case 278:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.clearcount;
		case 279:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.playcount - g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.clearcount;
		case 280:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_pgreat;
		case 281:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_great;
		case 282:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_good;
		case 283:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_bad;
		case 284:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_poor;
		case 285:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_pgreat * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
		case 286:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_great * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
		case 287:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_good * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
		case 288:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_bad * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
		case 289:
			if (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes != 0) {
				return (g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.stat_poor * 100) / g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.total_notes;
			}
			break;
			//290 is same 90
			//291 is same 91
		case 292:
			return g->sSelect.bmsList[g->sSelect.cur_song].rivalRecord.IRranking;
			break;
			//293 is same 180
		case 294: //TOFIX : IR clear rate
			break;
	}
	return 0;
}



//416a10
int SetObjectValue_Bargraph(game *g) {

	float max, val;

	for (int i = 0; i < g->skstruct.otherObject[5].srcSize; i++) {

		if (GetOptionFlag_dst(g, g->skstruct.otherObject[5].dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct.otherObject[5].dst[i].opt2) && GetOptionFlag_dst(g, g->skstruct.otherObject[5].dst[i].opt3)) {

			switch (g->skstruct.otherObject[5].src[i].op1) {
				
				default:
					continue;

				case 1:
					val = GetTimeLapse(41, &g->timer1);
					max = g->gameplay.song_runtime;
					break;

				case 2:
					val = g->gameplay.loadObject_loaded;
					max = g->gameplay.loadObject_total;
					break;

				case 3:
					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 5 || g->sSelect.bmsList[g->sSelect.cur_song].keymode == 10) {
						max = 9.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].level;
					}
					else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 7 || g->sSelect.bmsList[g->sSelect.cur_song].keymode == 14) {
						max = 12.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].level;
					}
					else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 9) {
						max = 42.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].level;
					}
					else continue;
					break;

				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 5 || g->sSelect.bmsList[g->sSelect.cur_song].keymode == 10) {
						max = g->config.select.levelbarflash_5;
						val = g->sSelect.levelIndicatorAnimation[g->skstruct.otherObject[5].src[i].op1 - 5];
					}
					else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 7 || g->sSelect.bmsList[g->sSelect.cur_song].keymode == 14) {
						max = g->config.select.levelbarflash_7;
						val = g->sSelect.levelIndicatorAnimation[g->skstruct.otherObject[5].src[i].op1 - 5];
					}
					else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 9) {
						max = g->config.select.levelbarflash_9;
						val = g->sSelect.levelIndicatorAnimation[g->skstruct.otherObject[5].src[i].op1 - 5];
					}
					else continue;
					break;

				case 10:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.player[0].exscore;
					break;

				case 11:
					max = g->gameplay.player[0].totalnotes * 2;
					if (g->gameplay.player[0].note_current <= 0) continue;
					val = g->gameplay.player[0].exscore * g->gameplay.player[0].totalnotes / g->gameplay.player[0].note_current;
					break;

				case 12:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.highScore.rate;
					break;

				case 13:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.highScore.exscore;
					break;

				case 14:
					if (g->config.play.battle == 1) {
						max = g->gameplay.player[1].totalnotes * 2;
						val = g->gameplay.player[1].exscore;
					}
					else {
						max = g->gameplay.player[0].totalnotes * 2;
						val = g->gameplay.targetScore.rate;
					}
					break;

				case 15:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.targetScore.exscore;
					break;

				case 20:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[5];
					break;

				case 21:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[4];
					break;

				case 22:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[3];
					break;

				case 23:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[2];
					break;

				case 24:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].judgecount[1];
					break;

				case 25:
					max = g->gameplay.player[0].totalnotes;
					val = g->gameplay.player[0].max_combo;
					break;

				case 26:
					if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14) {
						max = 20000.0;
						val = g->gameplay.player[0].score;
					}
					else {
						max = 10000.0;
						val = g->gameplay.player[0].score;
					}
					break;

				case 27:
					max = g->gameplay.player[0].totalnotes * 2;
					val = g->gameplay.player[0].exscore;
					break;

				case 30:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[5];
					break;

				case 31:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[4];
					break;

				case 32:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[3];
					break;

				case 33:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[2];
					break;

				case 34:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].judgecount[1];
					break;

				case 35:
					max = g->gameplay.player[1].totalnotes;
					val = g->gameplay.player[1].max_combo;
					break;

				case 36:
					if (g->sSelect.metaSelected.keymode == 7 || g->sSelect.metaSelected.keymode == 14) {
						max = 20000.0;
						val = g->gameplay.player[1].score;
					}
					else {
						max = 10000.0;
						val = g->gameplay.player[1].score;
					}
					break;

				case 37:
					max = g->gameplay.player[1].totalnotes * 2;
					val = g->gameplay.player[1].exscore;
					break;

				case 40:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_pgreat;
					break;

				case 41:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_great;
					break;

				case 42:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_good;
					break;

				case 43:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_bad;
					break;

				case 44:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_poor;
					break;

				case 45:
					max = g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes;
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_maxcombo;
					break;

				case 46:
					if ((g->sSelect.bmsList[g->sSelect.cur_song].keymode == 7) || (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 0xe)) {
						max = 20000.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score;
					}
					else {
						max = 10000.0;
						val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_score;
					}
					break;

				case 47:
					max = (g->sSelect.bmsList[g->sSelect.cur_song].mybest.total_notes * 2);
					val = g->sSelect.bmsList[g->sSelect.cur_song].mybest.stat_exscore;
					break;
			}

			if (val > 0) {
				if (val >= max) val = max;
				AddDrawingBuffer_BarGraph(&g->skstruct.drBuf, &g->skstruct.otherObject[5].src[i], &g->skstruct.otherObject[5].dst[i], &g->timer1, 0, max, val);
			}
		}		
	}
	return 1;
}

//4134b0
int SetObjectValue_Slider(game *g, skstruct *sk, Timer *T, char flag) {
	int mouseL, mouseR;
	int iBuf, iTmp;

	mouseL = g->KeyInput.mouse_buttonL;
	mouseR = g->KeyInput.mouse_buttonR;

	if (g->txtStruct.readme.show > 0 || flag || g->sSelect.searchFocused) {
		g->KeyInput.mouse_buttonL = 0;
		g->KeyInput.mouse_buttonR = 0;
	}

	for (int i = 0; i < sk->otherObject[2].srcSize; i++) {
		if (GetOptionFlag_dst(g, sk->otherObject[2].dst[i].opt1) == 0 || GetOptionFlag_dst(g, sk->otherObject[2].dst[i].opt2) == 0 || GetOptionFlag_dst(g, sk->otherObject[2].dst[i].opt3) == 0) continue;
		if (sk->otherObject[2].src[i].op2 < 1) continue;

		iBuf = 0;

		switch (sk->otherObject[2].src[i].op3) {
			case 1:
				iBuf = g->sSelect.listCalculatedBar;
				SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, g->sSelect.bmsListCount*1000 - 1, &iBuf, &g->KeyInput, i);
				if (iBuf != g->sSelect.listCalculatedBar) {
					g->sSelect.listTopbar = iBuf;
					g->sSelect.oldBar = iBuf;
					iTmp = iBuf / 1000;
					if (iBuf % 1000 > 499) {
						iTmp++;
					}
					g->sSelect.nowBar = iTmp * 1000;
					g->sSelect.barMoveStartTime = GetTimeWrap();
					g->sSelect.barMoveEndTime = GetTimeWrap() + 100;
				}
				break;

			case 2:
				SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 10, 990, &g->config.play.hiSpeed[0], &g->KeyInput, i);
				break;

			case 3:
				SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 10, 990, &g->config.play.hiSpeed[1], &g->KeyInput, i);
				break;

			case 4:
				if (g->config.play.p1_lanecover == 1 && g->gameplay.lanecoverDisplayP1 == 1)
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 100, &g->config.play.p1_lanecoverv, &g->KeyInput, i);
				else
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 100, &iBuf, &g->KeyInput, i);
				break;

			case 5:
				if (g->config.play.p2_lanecover == 1 && g->gameplay.lanecoverDisplayP2 == 1)
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 100, &g->config.play.p2_lanecoverv, &g->KeyInput, i);
				else
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 100, &iBuf, &g->KeyInput, i);
					
				break;

			case 6:
				iBuf = GetTimeLapse(41, T);
				if (iBuf >= 0) {
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, g->gameplay.song_runtime, &iBuf, &g->KeyInput, i);
				}
				else SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 1, &iBuf, &g->KeyInput, i); 
				break;

			case 7:
				if (g->skinData.Data[g->skinData.previewID].custom_count - 5 > 0) {
					if (SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, g->skinData.Data[g->skinData.previewID].custom_count - 5, &g->skinData.previewCustomID, &g->KeyInput, i)) {
						for (int j = 0; j < 5; j++) {
							SetObjectString(100 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].title, g->txtStruct.objectStr);
							SetObjectString(110 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].
								op_label[g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_selected - g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_start], g->txtStruct.objectStr);

						}
					}
				}
				else SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 1, &iBuf, &g->KeyInput, i);
				break;

			case 8:
				if (g->net.rankingData.rankingCount < 6) {
					SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 1, &iBuf, &g->KeyInput, i);
				}
				else {
					if (SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], T, 0, 1, &iBuf, &g->KeyInput, i) == 1) {
						SetObjectStrings_SongSelect(g);
					}
				}
				break;

			case 10:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[0], i, -12, 12) && GetTimeLapse(41,T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '3', g->audio.param.eq_gain[0]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 11:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[1], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '4', g->audio.param.eq_gain[1]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 12:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[2], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '5', g->audio.param.eq_gain[2]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 13:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[3], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '3', g->audio.param.eq_gain[3]); //TOFIX : 3 -> 6, looks like mistyping
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 14:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[4], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '7', g->audio.param.eq_gain[4]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 15:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[5], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '8', g->audio.param.eq_gain[5]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 16:
				if (UpdateSoundFX(g, &g->audio.param.eq_gain[6], i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '9', g->audio.param.eq_gain[6]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 17:
				if (UpdateSoundFX(g, &g->audio.param.volume_master, i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), ')', g->audio.param.volume_master);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 18:
				if (UpdateSoundFX(g, &g->audio.param.volume_key, i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '*', g->audio.param.volume_key);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 19:
				if (UpdateSoundFX(g, &g->audio.param.volume_BGM, i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '+', g->audio.param.volume_BGM);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 20:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[0][0], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '>', g->audio.param.fxParam[0][0]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 21:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[0][1], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '?', g->audio.param.fxParam[0][1]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 22:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[1][0], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 'H', g->audio.param.fxParam[1][0]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 23:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[1][1], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 'I', g->audio.param.fxParam[1][1]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 24:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[2][0], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 'R', g->audio.param.fxParam[2][0]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 25:
				if (UpdateSoundFX(g, &g->audio.param.fxParam[2][1], i, 0, 100) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 'S', g->audio.param.fxParam[2][1]);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
			case 26:
				if (UpdateSoundFX(g, &g->audio.param.pitch_amount, i, -12, 12) && GetTimeLapse(41, T) > 0 && g->gameplay.replay.status == 1) {
					AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), '[', g->audio.param.pitch_amount);
					g->gameplay.fxChangeInRecording = 1;
				}
				break;
		}
	}
	g->KeyInput.mouse_buttonL = mouseL;
	g->KeyInput.mouse_buttonR = mouseR;
	return 1;
}


//41dc30
int Proc_Text(game *g, sqlite3 *sql, char flag) {

	char str[1024];
	uint mouseL = g->KeyInput.mouse_buttonL;
	uint mouseR = g->KeyInput.mouse_buttonR;

	if (g->txtStruct.readme.show > 0 || flag || g->sSelect.searchFocused || (0.0 <= GetTimeLapse(5, &g->timer1) && GetTimeLapse(5, &g->timer1) <= 1000.0)) {
		g->KeyInput.mouse_buttonL = 0;
		g->KeyInput.mouse_buttonR = 0;
		g->sSelect.is_mouseOnTextInput = 1;
	}

	if ( (g->KeyInput.mouse_buttonL == 3) || (g->KeyInput.mouse_buttonR == 3) ) {
		DeleteKeyInput(g->txtStruct.hKeyInput);
		g->txtStruct.st_text_num = -1;
	}

	for (int i = 0; i < g->skstruct.otherObject[0].srcSize; i++) {
		if (GetOptionFlag_dst(g, g->skstruct.otherObject[0].dst[i].opt1) && GetOptionFlag_dst(g, g->skstruct.otherObject[0].dst[i].opt2)
			&& GetOptionFlag_dst(g, g->skstruct.otherObject[0].dst[i].opt3)) {

			if (flag == 0 && g->skstruct.otherObject[0].src[i].op1 == 1 && (g->skstruct.otherObject[0].src[i].op2 == 0 || g->skstruct.otherObject[0].src[i].op2 == g->sSelect.panel) && g->txtStruct.st_text_num == -1
				&& MouseOnObject(&g->skstruct.otherObject[0].dst[i], &g->timer1, &g->KeyInput.mouse_oldX, &g->KeyInput.mouse_oldY)) {
				g->sSelect.is_mouseOnTextInput = 1;
				if (g->KeyInput.mouse_buttonL == 3) {
					InitKeyInput();

					int st = g->skstruct.otherObject[0].src[i].st;
					if (st == 27) {
						if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 0) {
							g->txtStruct.hKeyInput = MakeKeyInput(4, 1, 0, 1);
						}
						else {
							g->txtStruct.hKeyInput = MakeKeyInput(2, 1, 0, 1);
						}
					}
					else if (st == 29) {
						g->txtStruct.hKeyInput = MakeKeyInput(2, 1, 0, 1);
					}
					else if (st == 28) {
						g->txtStruct.hKeyInput = MakeKeyInput(1, 1, 0, 1);
					}
					else {
						g->txtStruct.hKeyInput = MakeKeyInput(64, 1, 0, 0);
					}

					SetActiveKeyInput(g->txtStruct.hKeyInput);
					g->txtStruct.st_text_num = g->skstruct.otherObject[0].src[i].st;

					if (g->txtStruct.st_text_num != 30) {
						SetKeyInputString(GetStringFromArray(g->txtStruct.st_text_num, g->txtStruct.objectStr), g->txtStruct.hKeyInput);
					}
				}
			}
			AddDrawingBuffer_Text(&g->skstruct.drBuf, &g->skstruct.otherObject[0].src[i], &g->skstruct.otherObject[0].dst[i], &g->timer1);

		}
	}

	if (g->txtStruct.st_text_num != -1) {
		g->sSelect.is_mouseOnTextInput = 1;

		if (CheckKeyInput(g->txtStruct.hKeyInput) >= 1 && flag == 0) {
			//CheckKeyInput() 0:doing 1:done 2:cancle -1:error
			if (CheckKeyInput(g->txtStruct.hKeyInput) == 1) {

				CSTR buf;
				GetKeyInputString(buf, g->txtStruct.hKeyInput);
				CSTR query;

				CSTR unused_path;
				CSTR unused_type;
				unused_path = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
				if (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 0) {
					unused_type = "folder";
				}
				else if (g->sSelect.bmsList[g->sSelect.cur_song].keymode > 0) {
					unused_type = "song";
				}

				switch (g->txtStruct.st_text_num) {
				case 20:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].title = buf;
					if (g->sSelect.bmsList[g->sSelect.cur_song].subtitle.isDiff("(null)")) {
						cstrSprintf(&g->sSelect.bmsList[g->sSelect.cur_song].fulltitle, "%s %s", buf, g->sSelect.bmsList[g->sSelect.cur_song].subtitle);
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
					}
					else {
						g->sSelect.bmsList[g->sSelect.cur_song].fulltitle = buf;
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
					}
					break;
				case 21:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].title = buf;
					cstrSprintf(&g->sSelect.bmsList[g->sSelect.cur_song].fulltitle, "%s %s", g->sSelect.bmsList[g->sSelect.cur_song].title, buf);
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 23:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].genre = buf;
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 24:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].artist = buf;
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 25:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].subartist = buf;
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 26:
					SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
					SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					g->sSelect.bmsList[g->sSelect.cur_song].tag = buf;
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
					break;
				case 27:
					atol(buf);
					if ((0 <= atol(buf) && atol(buf) < 100) || (g->sSelect.bmsList[g->sSelect.cur_song].keymode == 0)) {
						SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
						SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
						g->sSelect.bmsList[g->sSelect.cur_song].level = atol(buf);
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
					}
					break;
				case 28:
					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode != 0 && (0 <= atol(buf) && atol(buf) <= 5)) {
						SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
						SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
						g->sSelect.bmsList[g->sSelect.cur_song].difficulty = atol(buf);
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
						if (g->config.select.difficulty != g->sSelect.bmsList[g->sSelect.cur_song].difficulty && g->config.select.difficulty != 0) {
							g->config.select.difficulty = g->sSelect.bmsList[g->sSelect.cur_song].difficulty;
						}
					}
					break;
				case 29:
					if (g->sSelect.bmsList[g->sSelect.cur_song].keymode != 0 && atol(buf) >= 0) {
						SetObjectString(g->txtStruct.st_text_num - 10, buf, g->txtStruct.objectStr);
						SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
						g->sSelect.bmsList[g->sSelect.cur_song].exlevel = atol(buf);
						g->sSelect.is_tag_edited = 1;
						g->sSelect.is_coursemaking_done = 1;
					}
					break;
				case 30:
					if (*(buf.atPos(0)) == '/') {
						CmdSearch(g, &buf, sql);
						SetObjectString(g->txtStruct.st_text_num, buf, g->txtStruct.objectStr);
					}
					else {
						query = "(null)";

						if (buf.findStrPos("-") > 0) {
							int from = atol(buf.left(buf.findStrPos("-")));
							int to = atol(buf.right(buf.length() - buf.findStrPos("-") - 1));

							if (from > 0) {
								if (0 < to && 50 <= to) {
									cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE %d<=minbpm AND maxbpm<=%d", from, to);
									buf.add(" BPM");
								}
								else if (1 <= to && to < 50) {
									cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE %d<=level AND level<=%d", from, to);
									cstrSprintf(&buf, "LEVEL %d-%d", from, to);
								}

							}
						}
						else if (atol(buf) >= 50) {
							cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE maxbpm=%d OR minbpm=%d", atol(buf), atol(buf));
							buf.add(" BPM");
						}
						else if (atol(buf) > 0) {
							cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE level = %d", atol(buf));
							cstrSprintf(&buf, "LEVEL %d", atol(buf));
						}

						if (query.isSame("(null)")) {
							sqlite3_snprintf(1024, str, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE title LIKE \'%%%s%%\' OR genre LIKE \'%%%s%%\'  OR artist LIKE \'%%%s%%\' OR tag LIKE \'%%%s%%\' OR path LIKE \'%%%s%%\' OR subtitle LIKE \'%%%s%%\' OR subartist LIKE \'%%%s%%\'", buf, buf, buf, buf, buf, buf, buf);
							query.assign(str);
						}

						g->config.select.difficulty = 0;
						g->sSelect.searchFocused = 1;
						g->sSelect.filterDifficulty = 0;
						g->sSelect.filterKey = g->config.select.key;
						g->sSelect.curQuery[0] = query;
						g->sSelect.queryCount = 1;
						g->sSelect.searchType = 3;
						g->sSelect.searchMax = g->config.select.searchmax;
						g->sSelect.filterSort = g->config.select.sort;
						g->sSelect.unk4fb8[0] = 0;
						g->sSelect.selTitle = g->sSelect.bmsList[g->sSelect.cur_song].title;
						g->sSelect.selFilepath = g->sSelect.bmsList[g->sSelect.cur_song].filepath;
						g->sSelect.selFolder = g->sSelect.bmsList[g->sSelect.cur_song].folder;
						g->sSelect.searchInput = buf;
						g->sSelect.text_num = g->txtStruct.st_text_num;
					}
					break;
				case 170:
					SetObjectString(170, buf, g->txtStruct.objectStr);
					g->sSelect.course.name = buf;
					if (g->sSelect.course.isMakingCourse == 0) {
						g->sSelect.course.isChangingTitle = 1;
					}
					ProcS_Select(g);
					break;

				}

			}
			DeleteKeyInput(g->txtStruct.hKeyInput);
			g->txtStruct.st_text_num = -1;
			SetTimeLapse(4, &g->timer1);
		}
	}

	g->KeyInput.mouse_buttonL = mouseL;
	g->KeyInput.mouse_buttonR = mouseR;
	return 1;
}

//423450
int SetObjectValue_Button(game *g, skstruct *sk, Timer *T, char flag) {
	int mouseL, mouseR;
	int isClickSuccess;
	int iTmp = 0;
	int filterChanged;

	mouseL = g->KeyInput.mouse_buttonL;
	mouseR = g->KeyInput.mouse_buttonR;

	if (g->txtStruct.readme.show > 0 || flag || g->sSelect.searchFocused) {
		g->KeyInput.mouse_buttonL = '\0';
		g->KeyInput.mouse_buttonR = '\0';
	}
	filterChanged = 0;

	for (int i = 0; i < sk->otherObject[1].srcSize; i++) {
		isClickSuccess = 0;
		if (GetOptionFlag_dst(g, sk->otherObject[1].dst[i].opt1)==0 || GetOptionFlag_dst(g, sk->otherObject[1].dst[i].opt2)==0 || GetOptionFlag_dst(g, sk->otherObject[1].dst[i].opt3)==0) continue;

		iTmp = 0;
		int op = sk->otherObject[1].src[i].op1;
		switch (op) {
			//Panels
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				iTmp = (g->sSelect.panel == op);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2	&& g->sSelect.course.isMakingCourse != 1) {
					if (g->sSelect.panel == op) {
						g->sSelect.panel = -1;
						ResetTimeLapse(op + 20, T);
						SetTimeLapse(op + 30, T);
						PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
					}
					else {
						if (g->sSelect.panel >= 0) {
							ResetTimeLapse(g->sSelect.panel + 20, T);
							SetTimeLapse(g->sSelect.panel + 30, T);
						}
						SetObjectStrings_SongSelect(g);
						SetTimeLapse(op + 20, T);
						g->sSelect.panel = op;
						g->sSelect.panel_unk = op;
						PlaySound(&g->audio, &g->audio.sysSound.panel_open, g->audio.chnKey, -1);
					}
					if(g->sSelect.panel == -1) g->sSelect.panel_unk = op;
				}
				else continue;
				break;

			//case 9 not exist

			//SongSelect Option
			case 10:
				iTmp = g->config.select.difficulty;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				if (isClickSuccess == 2) {
					filterChanged = 1;
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 0;
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 11:
				iTmp = g->config.select.key;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				if (isClickSuccess == 2) {
					filterChanged = 1;
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 2;
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 12:
				iTmp = g->config.select.sort;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				if (isClickSuccess == 2) {
					filterChanged = 1;
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 1;
					SetObjectStrings_SongSelect(g);
				}
				break;

			//Buttons
			case 13:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						g->sSelect.is_clicked_keyconfig = 1;
					}
				}
				break;

			case 14:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						g->sSelect.is_clicked_skinselect = 1;
					}
				}
				break;

			case 15:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						if (g->sSelect.bmsList[g->sSelect.cur_song].keymode > 0 || g->sSelect.bmsList[g->sSelect.cur_song].folderType == 9) {
							g->gameplay.isAutoplay = 0;
							g->sSelect.is_clicked_autoplay_replay = 1;
						}
					}
				}
				break;

			case 16:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						if (g->sSelect.bmsList[g->sSelect.cur_song].keymode > 0 || g->sSelect.bmsList[g->sSelect.cur_song].folderType == 9) {
							g->gameplay.isAutoplay = 1;
							g->sSelect.is_clicked_autoplay_replay = 1;
						}
					}
				}
				break;

			case 17:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						ShowReadmes(g);
						if (g->txtStruct.readme.show) {
							ResetTimeLapse(16, T);
							SetTimeLapse(15, T);
							PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						}
					}
				}
				break;

			case 18:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						g->sSelect.is_clicked_tagedit = 1;
					}
				}
				break;

			case 19:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2 && g->sSelect.bmsList[g->sSelect.cur_song].keymode > 0) {
						g->gameplay.replay.status = 2;
						g->gameplay.isAutoplay = 0;
						g->sSelect.is_clicked_autoplay_replay = 1;
					}
				}
				break;
			
			//Option FX
			case 20:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxType[0], 0, 7, g->sSelect.panel);
				if (isClickSuccess == 2) {
					InitFxParam(g, 0);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 61, g->audio.param.fxType[0]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 21:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxType[1], 0, 7, g->sSelect.panel);
				if (isClickSuccess == 2) {
					InitFxParam(g, 1);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 71, g->audio.param.fxType[1]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 22:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxType[2], 0, 7, g->sSelect.panel);
				if (isClickSuccess == 2) {
					InitFxParam(g, 2);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 81, g->audio.param.fxType[2]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 23:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fx_on[0], 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 60, g->audio.param.fx_on[0]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 24:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fx_on[1], 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 70, g->audio.param.fx_on[1]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 25:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fx_on[2], 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 80, g->audio.param.fx_on[2]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 26:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxChannel[0], 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 64, g->audio.param.fxChannel[0]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 27:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxChannel[1], 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 74, g->audio.param.fxChannel[1]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 28:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fxChannel[2], 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 84, g->audio.param.fxChannel[2]);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 29:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.eq_on, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 50, g->audio.param.eq_on);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 30:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.eq_preset, 0, 5, g->sSelect.panel); //EQ_PRESET is not implemented yet
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
				}
				break;

			case 31:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.fx_volume_on, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 40, g->audio.param.fx_volume_on);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 32:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.pitch_on, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 90, g->audio.param.pitch_on);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			case 33:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->audio.param.pitch_type, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					ApplySoundFX(&g->audio, 1, g->config.sound.disabledsp);
					if (GetTimeLapse(41, T) > 0.0 && g->gameplay.replay.status == 1) {
						AddReplayData(&g->gameplay.replay, GetTimeLapse(41, T), 92, g->audio.param.pitch_type);
						g->gameplay.fxChangeInRecording = 1;
					}
				}
				break;

			//Option Play
			case 40:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.gaugeOption[0], 0, 5, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 41:
				if (g->config.play.battle == 1) {
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.gaugeOption[1], 0, 5, g->sSelect.panel);
					if (isClickSuccess == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				else continue;
				break;

			case 42:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.random[0], 0, 5, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 43:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.random[1], 0, 5, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 44:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.p1_assist, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 45:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.p2_assist, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break; 
				
			case 46:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.p1_lanecover, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->config.play.p2_lanecover = g->config.play.p1_lanecover;
					SetObjectStrings_SongSelect(g);
				}
				break;
			
			case 50:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.m_HIDSUD1, 0, 3, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 51:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.m_HIDSUD2, 0, 3, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 54:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.dpflip, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 55:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.hsfix, 0, 4, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 56:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.battle, 0, 4, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 57:
				iTmp = 1;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
				}

				if (iTmp == 2) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
				else if(iTmp == 0) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, -g->config.play.hsmargin, &g->config.play.hiSpeed[0]);
				break;
			case 58:
				iTmp = 1;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
				}

				if (iTmp == 2) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, g->config.play.hsmargin, &g->config.play.hiSpeed[1]);
				else if (iTmp == 0) LoopInRange(g->config.play.hsmin, g->config.play.hsmax, -g->config.play.hsmargin, &g->config.play.hiSpeed[1]);
				break;

			case 70:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.scoregraph, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 71:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.play_ghost, 0, 3, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 72:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.bga, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 73:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.bgasize, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 74:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.judgetiming, -99, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 75:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.autojudge, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 76:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.target_percent, 50, 100, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 77:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.p1_target, 0, 8, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetTarget(g);
					SetObjectStrings_SongSelect(g);
				}
				break;

			//Option etc
			case 80:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.system.screenmode, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->is_clicked_screenModeChange = 1;
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 81:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.system.highcolor, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 82:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.system.vsync, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetWaitVSyncFlag(g->config.system.vsync);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 83:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.play.replay, 0, 4, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SetObjectStrings_SongSelect(g);
				}
				break;

			case 90:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->sSelect.bmsList[g->sSelect.cur_song].favorite, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->sSelect.is_tag_edited = 1;
					g->sSelect.is_coursemaking_done = 1;
				}
				break;

			//SongSelect Difficulty
			case 91:
			case 92:
			case 93:
			case 94:
			case 95:
			case 96:
				iTmp = g->config.select.difficulty;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				if (isClickSuccess == 2) {
					g->sSelect.is_clicked_filter = 1;
					g->sSelect.filter_clicked = 0;
					g->sSelect.isDifficultyFilterOn = 0;
					g->sSelect.unk5004_difficultycount = 0;
					filterChanged = 1;
					if (op != 91) {
						if (g->sSelect.bmsList[g->sSelect.cur_song].difficulty == op - 91) {
							g->sSelect.isDifficultyFilterOn = 1;
							g->sSelect.unk5004_difficultycount = g->sSelect.bmsList[g->sSelect.cur_song].difficultyCount;
							LoopInRange(0, g->sSelect.bmsList[g->sSelect.cur_song].difficultyExist[g->sSelect.bmsList[g->sSelect.cur_song].difficulty - 1], 1, &g->sSelect.unk5004_difficultycount);
						}
					}
					g->config.select.difficulty = op - 92;
					SetObjectStrings_SongSelect(g);
				}
				break;

			//KeyConfig
			case 100:
			case 101:
			case 102:
			case 103:
			case 104:
			case 105:
			case 106:
			case 107:
			case 108:
			case 109:
			case 110:
			case 111:
			case 112:
			case 113:
			case 114:
			case 115:
			case 116:
			case 117:
			case 118:
			case 119:
			case 120:
			case 121:
			case 122:
			case 123:
			case 124:
			case 125:
			case 126:
			case 127:
			case 128:
			case 129:
			case 130:
			case 131:
			case 132:
			case 133:
			case 134:
			case 135:
			case 136:
			case 137:
			case 138:
			case 139:
				iTmp = (g->KeyInput.config_button_inMap == op - 100);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap = op - 100;
					if (g->KeyInput.config_keymode == 0) {
						g->KeyInput.config_button = ConfigButtonFromKeyID7(op - 100);
					}
					else if (g->KeyInput.config_keymode == 1) {
						g->KeyInput.config_button = ConfigButtonFromKeyID9(op - 100);
					}
					else if (g->KeyInput.config_keymode == 2) {
						g->KeyInput.config_button = ConfigButtonFromKeyID5(op - 100);
					}
					ProcS_Keyconfig(g);
				}
				break;

			case 140:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->KeyInput.config_button, 0, 22, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap  = ConfigButtonToKeyID7(g->KeyInput.config_button);
					ProcS_Keyconfig(g);
				}
				break;

			case 141:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->KeyInput.config_button, 0, 11, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap = ConfigButtonToKeyID9(g->KeyInput.config_button);
					ProcS_Keyconfig(g);
				}
				break;

			case 142:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->KeyInput.config_button, 0, 18, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap = ConfigButtonToKeyID5(g->KeyInput.config_button);
					ProcS_Keyconfig(g);
				}
				break;

			case 143:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->KeyInput.config_keymode, 0, 2, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_button_inMap = 0;
					g->KeyInput.config_button = 0;
					g->KeyInput.config_key = -1;
					switch (g->KeyInput.config_keymode) {
						case 0:
							ReadKeyConfig(g, "LR2files\\Config\\keyconfig.xml");
							break;
						case 1:
							ReadKeyConfig(g, "LR2files\\Config\\keyconfig_p.xml");
							break;
						case 2:
							ReadKeyConfig(g, "LR2files\\Config\\keyconfig_5.xml");
							break;
					}
					ProcS_Keyconfig(g);
				}
				break;

			case 150:
			case 151:
			case 152:
			case 153:
			case 154:
			case 155:
			case 156:
			case 157: 
				//TOFIX : keyconfig slot is 10 but clickable button is only 8??
				iTmp = (g->KeyInput.config_key == op - 150);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->KeyInput.config_key = op - 150;
				}
				break;

			//SkinSelect
			case 170:
			case 171:
			case 172:
			case 173:
			case 174:
			case 175:
			case 176:
			case 177:
			case 178:
			case 179:
			case 180:
			case 181:
			case 182:
			case 183:
			case 184:
			case 185:
			case 186:
			case 187:
			case 188: {
				iTmp = (g->skinData.select == op - 170);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 1, g->sSelect.panel);
				if(isClickSuccess == 2){
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					g->skinData.select = op - 170;
					g->skinData.previewID = g->skinData.skinID[op - 170];
					if (g->skinData.select == 10) {
						ReadLR2SoundSet(g, g->skinData.Data[g->skinData.previewID].skinFile, 0);
						PlaySound(&g->audio, &g->audio.sysSound.select, g->audio.chnBgm, -1);
					}

					if (g->skinData.select == 5) { 
						g->sSelect.toRoot = 1;
					}
					g->skinData.previewCustomID = 0;
					CSTR tcstr;
					SkinUser sku;
					cstrSprintf(&tcstr, "LR2files\\SkinCustomize\\%s.xml", MD5str(g->skinData.Data[g->skinData.previewID].skinFile));
					ReadSkinCustomize(&sku, tcstr);

					for (int j = 0; j < 100; j++) { // VULNERABILITY : out of index sku (array size 40, but access to 100)
						if (g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start == 0) {
							for (int k = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start; k < g->skinData.Data[g->skinData.previewID].customs[j].dst_op_count; k++) {
								if (g->skinData.Data[g->skinData.previewID].customs[j].op_label[k].isSame(&sku.customize_filename[j])) {
									g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = k;
								}
							}
						}
						else {
							if (sku.customize_value[j] < g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start
								|| g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start + g->skinData.Data[g->skinData.previewID].customs[j].dst_op_count - 1 < sku.customize_value[j]) {
								g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start;
							}
							else {
								g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = sku.customize_value[j];
							}
						}
					}

					SetObjectString(50, g->skinData.Data[g->skinData.previewID].title, g->txtStruct.objectStr);
					SetObjectString(51, g->skinData.Data[g->skinData.previewID].maker, g->txtStruct.objectStr);
					for (int j = 0; j < 5; j++) {
						SetObjectString(100 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID+j].title, g->txtStruct.objectStr);
						SetObjectString(110 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].
							op_label[ g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_selected - g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_start ], g->txtStruct.objectStr);
					}
					MakeSkinPreview(g, &g->skstruct2, &g->skinData);
					PlayPreviewSample(g);
					InitTimer(&g->timer2);
					SetTimeLapse(0, &g->timer2);
				}
				break;
			}

			case 190: {
				sk->otherObject[1].src[i];
				iTmp = (g->KeyInput.config_key == op - 150);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, -1, 1, g->sSelect.panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					if (iTmp == 1) SkinPreviewNext(&g->skinData, (SKINTYPE)g->skinData.select);
					else if (iTmp == -1) SkinPreviewPrev(&g->skinData, (SKINTYPE)g->skinData.select);
					SelectSkin(&g->skinData, &g->config.skin.skinFilePath[g->skinData.select]);
					if (g->skinData.select == 10) { // SOUNDSET
						ReadLR2SoundSet(g, g->skinData.Data[g->skinData.previewID].skinFile, 0);
						PlaySound(&g->audio, &g->audio.sysSound.select, g->audio.chnBgm, -1);
					}
					else if (g->skinData.select == 11) { // THEME
						SkinSelect_SoundSet(g,g->skinData.Data[g->skinData.previewID].skinFile);
						ReadLR2SoundSet(g, g->config.skin.skinFilePath[10], 0);
						PlaySound(&g->audio, &g->audio.sysSound.select, g->audio.chnBgm, -1);
					}

					g->skinData.previewCustomID = 0;
					CSTR tcstr;
					SkinUser sku;
					cstrSprintf(&tcstr, "LR2files\\SkinCustomize\\%s.xml", MD5str(g->skinData.Data[g->skinData.previewID].skinFile));
					ReadSkinCustomize(&sku, tcstr);

					for (int j = 0; j < 100; j++) { // VULNERABILITY : out of index sku (array size 40, but access to 100)
						if (g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start == 0) {
							for (int k = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start; k < g->skinData.Data[g->skinData.previewID].customs[j].dst_op_count; k++) {
								if (g->skinData.Data[g->skinData.previewID].customs[j].op_label[k].isSame(&sku.customize_filename[j])) {
									g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = k;
								}
							}
						}
						else {
							if (sku.customize_value[j] < g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start
								|| g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start + g->skinData.Data[g->skinData.previewID].customs[j].dst_op_count - 1 < sku.customize_value[j]) {
								g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start;
							}
							else {
								g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected = sku.customize_value[j];
							}
						}
					}

					SetObjectString(50, g->skinData.Data[g->skinData.previewID].title, g->txtStruct.objectStr);
					SetObjectString(51, g->skinData.Data[g->skinData.previewID].maker, g->txtStruct.objectStr);
					for (int j = 0; j < 5; j++) {
						SetObjectString(100 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].title, g->txtStruct.objectStr);
						SetObjectString(110 + j, g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].
							op_label[g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_selected - g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + j].dst_op_start], g->txtStruct.objectStr);
					}
					MakeSkinPreview(g, &g->skstruct2, &g->skinData);
					PlayPreviewSample(g);
					InitTimer(&g->timer2);
					SetTimeLapse(0, &g->timer2);
				}
				break;
			}
			case 191:
				iTmp = (g->KeyInput.config_key == op - 150);
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 5, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
					SelectSkin(&g->skinData, g->config.skin.skinFilePath + g->skinData.select);
				}
				break;
			case 200:
			case 201:
			case 202:
			case 203:
			case 204:
			case 205:
			case 206:
			case 207:
			case 208:
			case 209:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						ShowReadme(g,sk->helpfilePath[op-200]);
						if (g->txtStruct.readme.show) {
							ResetTimeLapse(16, &g->timer1);
							SetTimeLapse(15, &g->timer1);
							PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						}
					}
				}
				break;

			case 210:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 0, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						g->sSelect.is_buttonIRpage = 1;
					}
				}
				break;

			case 220:
			case 221:
			case 222:
			case 223:
			case 224:
			case 225:
			case 226:
			case 227:
			case 228:
			case 229: {
				int customIDThis = op - 220;
				SkinCustom &customThis = g->skinData.Data[g->skinData.previewID].customs[g->skinData.previewCustomID + customIDThis];
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &customThis.dst_op_selected, customThis.dst_op_start, customThis.dst_op_start + customThis.dst_op_count -1, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					SetObjectString(100 + customIDThis, customThis.title, g->txtStruct.objectStr);
					SetObjectString(110 + customIDThis, customThis.op_label[customThis.dst_op_selected - customThis.dst_op_start], g->txtStruct.objectStr);

					CSTR tcstr;
					SkinUser sku;
					cstrSprintf(&tcstr, "LR2files\\SkinCustomize\\%s.xml", MD5str(g->skinData.Data[g->skinData.previewID].skinFile));
					ReadSkinCustomize(&sku, tcstr);

					for (int j = 0; j < 100; j++) { // VULNERABILITY : out of index sku (array size 40, but access to 100)
						if (g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start == 0) {
							sku.customize_filename[j].assign(&g->skinData.Data[g->skinData.previewID].customs[j].op_label[g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected]);
						}
						else if(g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start > 899 && g->skinData.Data[g->skinData.previewID].customs[j].dst_op_start < 1000){
							sku.customize_value[j] = g->skinData.Data[g->skinData.previewID].customs[j].dst_op_selected;
						}
					}

					WriteSkinCustomizeXml(&sku, tcstr);
					MakeSkinPreview(g, &g->skstruct2, &g->skinData);
					PlayPreviewSample(g);
					InitTimer(&g->timer2);
					SetTimeLapse(0, &g->timer2);
					if (g->skinData.select == 10) {
						ReadLR2SoundSet(g, g->skinData.Data[g->skinData.previewID].skinFile, 0);
						PlaySound(&g->audio, &g->audio.sysSound.select, g->audio.chnBgm, -1);
					}
					PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
				}
				break;
			}

			case 230:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, -1, 1, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->sSelect.course.count > 0 && g->sSelect.course.isMakingCourse == 1) {
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
						filterChanged = 1;
						g->sSelect.is_coursemaking_done = 1;
						g->sSelect.filter_clicked = 6;
						g->sSelect.course.isCourseCreated = 1;
						g->sSelect.course.isMakingCourse = 0;
					}
				}
				break;

			case 231:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, -1, 1, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->sSelect.course.count > 0 && g->sSelect.course.isMakingCourse == 1) {
						ResetTimeLapse(17, &g->timer1);
						SetTimeLapse(18, &g->timer1);
						PlaySound(&g->audio, &g->audio.sysSound.panel_close, g->audio.chnKey, -1);
						g->sSelect.course.count = -1;
						g->sSelect.course.isMakingCourse = 0;
						SetTimeLapse(5, &g->timer1);
						g->sSelect.cur = 1;
						g->sSelect.stack_query[1] = g->sSelect.course.return_query;
						g->sSelect.stack_folderPath[1] = g->sSelect.course.return_filepath;
						g->sSelect.stack_isFolder[1] = g->sSelect.course.return_isFolder;
						g->sSelect.stack_searchTitle[1] = g->sSelect.course.return_searchText;
						g->sSelect.stack_rivalID[1] = g->sSelect.course.return_rivalID;
						filterChanged = 1;
						g->sSelect.is_coursemaking_done = 1;
						g->sSelect.filter_clicked = 5;
					}
				}
				break;
			//case 232 doesn't exist
			case 233:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, -1, 1, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->sSelect.bmsList[g->sSelect.cur_song].folderType == 8) {
						g->sSelect.course.isDeletingCourse = 1;
					}
				}
				break;

			case 240:
			case 241:
			case 242:
			case 243:
			case 244:
			case 245:
			case 246:
			case 247:
			case 248:
				if (g->sSelect.bmsList[g->sSelect.cur_song].courseIR == 1 || g->sSelect.bmsList[g->sSelect.cur_song].courseType != 1) {
					iTmp = g->sSelect.bmsList[g->sSelect.cur_song].courseKeys[op - 240];
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				}
				else {
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->sSelect.bmsList[g->sSelect.cur_song].courseKeys[op-240], 0, 5, g->sSelect.panel);
					if (isClickSuccess == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						g->sSelect.course.isCourseCreated = 1;
					}
				}
				break;

			//249 250 251 252 not found

			case 253:
				if (g->sSelect.bmsList[g->sSelect.cur_song].courseIR == 1) {
					iTmp = 1;
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, iTmp, iTmp, g->sSelect.panel);
				}
				else {
					isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->sSelect.bmsList[g->sSelect.cur_song].courseIR, 0, 1, g->sSelect.panel);
					if (isClickSuccess == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						g->sSelect.course.isCourseCreated = 1;
					}
				}
				break;

			case 260:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.optimumlevel_7, 0, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 261:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.maxlevel, 0, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 262:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.minlevel, 0, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 263:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.bpmrange, 0, 99, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 264:
				iTmp = g->config.course.maxbpm / 10;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 99, g->sSelect.panel);
				g->config.course.maxbpm = iTmp * 10;
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 265:
				iTmp = g->config.course.minbpm / 10;
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &iTmp, 0, 99, g->sSelect.panel);
				g->config.course.minbpm = iTmp * 10;
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			case 266:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.stage, 0, 5, g->sSelect.panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;

			case 268:
				isClickSuccess = ButtonByInput(&sk->drBuf, &sk->otherObject[1].src[i], &sk->otherObject[1].dst[i], T, &g->KeyInput, &g->config.course.defaultconnection, 0, 5, (g->sSelect).panel);
				if (isClickSuccess == 2) {
					if (g->procSelecter == 2) {
						PlaySound(&g->audio, &g->audio.sysSound.option_change, g->audio.chnKey, -1);
						SetObjectStrings_SongSelect(g);
					}
				}
				break;
			default:
				continue;
		}

		if (isClickSuccess) g->sSelect.is_mouseOnTextInput = 1;
		if (isClickSuccess == 2) g->sSelect.buttonObjClicked = 1;
	}

	g->KeyInput.mouse_buttonL = mouseL;
	g->KeyInput.mouse_buttonR = mouseR;
	g->sSelect.is_filter_changed = filterChanged;
	return 1;
}

//49bfc0
int MouseOnDSTD(DSTdraw *dstd, int *x, int *y){ //1 right 2 left
	float fxv;
	float half;
	int xv;
	int yv;

	xv = *x;
	yv = *y;
	fxv = (float)xv;
	if ( xv < 640 && 0 <= xv && yv < 480 && 0 <= yv 
		&& dstd->x <= fxv && fxv <= dstd->w + dstd->x
		&& dstd->y <= (float)yv && (float)yv <= dstd->h + dstd->y) {
		
		half = dstd->w * 0.5 + dstd->x;
		
		if (fxv >= half) return 1;
		else return 2;
	}
	return 0;
}

//49c070
int MouseOnObject(DSTstruct *dst, Timer *T, int *x, int *y){
	DSTdraw _dstd;
	DSTdraw local_a0;
	double time;
	DSTstruct _dst;

	time = GetTimeLapse(dst->timer, T);
	memcpy(&_dst, dst, sizeof(DSTstruct));
	_dstd = SetDSTdrawByTime(_dst, time);
	return MouseOnDSTD(&_dstd, x, y);
}

//49c0e0 maybe done
int SliderByTime(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int min, int max, int *value, inputStructure *input, int objectID) {
	DSTdraw dstdTemp;
	DSTstruct dstsTemp;
	int newVal, newX=0, newY=0;
	int moveX, moveY;

	if (dst->dstCount < 1) return 0;
	if (dst->dataSize < 1) return 0;
	if (src->op2 < 1) return 0; //op2:range
	if (GetTimeLapse(dst->timer, T) == -1) return 0;
	
	memcpy(&dstsTemp, dst, sizeof(DSTstruct));
	dstdTemp = SetDSTdrawByTime(dstsTemp,GetTimeLapse(dst->timer,T));

	newX = 0;
	newY = 0;
	newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
	switch (src->op1) { //op1:muki(direction) 0up 1right 2down 3left
		case 0:
			dstdTemp.y = dstdTemp.y - src->op2;
			dstdTemp.h = src->op2 + dstdTemp.h;
			break;
		case 1:
			dstdTemp.w = src->op2 + dstdTemp.w;
			break;
		case 2:
			dstdTemp.h = src->op2 + dstdTemp.h;
			break;
		case 3:
			dstdTemp.w = src->op2 + dstdTemp.w;
			dstdTemp.x = dstdTemp.x - src->op2;
			break;
		default:
			return 0;
	}

	if ( ((objectID == input->is_drag_now && input->mouse_moveflag == 1) || (MouseOnDSTD(&dstdTemp, &input->mouse_oldX, &input->mouse_oldY) != 0 && input->mouse_buttonL == 1)) && src->op4 == 0) { //op4:disable
		moveX = (dstdTemp.w - src->op2) / 2;
		moveY = (dstdTemp.h - src->op2) / 2;
		input->is_drag_now = objectID;
		switch (src->op1) {
			case 0:
				*value = ChangeValueByTime(max, min, dstdTemp.y + moveY, dstdTemp.y + dstdTemp.h - moveY, input->mouse_oldY, 0);
				newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
				break;
			case 1:
				*value = ChangeValueByTime(min, max, dstdTemp.x + moveX, dstdTemp.x + dstdTemp.w - moveX, input->mouse_oldX, 0);
				newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
				break;
			case 2:
				*value = ChangeValueByTime(min, max, dstdTemp.y + moveY, dstdTemp.y + dstdTemp.h - moveY, input->mouse_oldY, 0);
				newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
				break;
			case 3:
				*value = ChangeValueByTime(max, min, dstdTemp.x + moveX, dstdTemp.x + dstdTemp.w - moveX, input->mouse_oldX, 0);
				newVal = ChangeValueByTime(0.0, src->op2, min, max, *value, 0);
				break;
			default:
				break;
		}
	}

	switch (src->op1) {
		case 0:
			newY = -newVal;
			break;
		case 1:
			newX = newVal;
			break;
		case 2:
			newY = newVal;
			break;
		case 3:
			newX = -newVal;
			break;
		default:
			return 0;
	}
	//old code
	//switch (src->op1) {
	//	case 0:
	//		newVal = -newVal;
	//	case 2:
	//		newY = newVal;
	//		newVal = newX;
	//		break;
	//	case 1:
	//		break;
	//	case 3:
	//		newVal = -newVal;
	//		break;
	//	default:
	//		return 0;
	//}
	//newX = newVal;
	src->sy = newY;
	src->sx = newX;
	return 1;
}

//49c560
int ButtonByInput(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, inputStructure *input, int *target, int min, int max, int panel) { //return 1:just clicked 2:changed 0:not changed
	DSTdraw dstd;
	int mouse, ret;
	
	if ((dst->dstCount < 1) || (dst->dataSize < 1)) return 0;
	if (GetTimeLapse(dst->timer, T) == -1.0) return 0;

	ret = 0;
	dstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	mouse = MouseOnDSTD(&dstd, &input->mouse_oldX, &input->mouse_oldY);
	if ( (mouse == 1 && src->op4 == 0) || (mouse != 0 && src->op4 == 1) ) { // right side or plusonly
		if (src->op2 != 1 || input->mouse_buttonL != 1 || (src->op3 != panel && src->op3 != 0)) {
			ret = 1;
	}
		else {
			if (min < max) (*target)++;
			ret = 2;
		}
	}
	else if ( (mouse == 2 && src->op4 == 0) || (mouse != 0 && src->op4 == 2) ) { // left side or plusonly(-)
		if (src->op2 != 1 || input->mouse_buttonL != 1 || (src->op3 != panel && src->op3 != 0)) {
			ret = 1;
		}
		else {
			if (min < max) (*target)--;
			ret = 2;
		}
	}

	if (max < *target) *target = min;
	else if (min > *target) *target = max;

	src->inArray = *target;
	if (src->graphcount <= *target) src->inArray = src->graphcount - 1;
	if (*target < 0) src->inArray = 0;
	if (min < 0) src->inArray = 0;

	return ret;
}

//4b6c00
int InitObjectString(TextStruct *txt){
	for (int i = 0; i < 300; i++) {
		txt->objectStr[i].fillzero();
	}
	txt->objectStr[0].assign("これはテストこれはテストこれはテスト");
	txt->st_text_num = -1;
	return 1;
}

//4b6c40
int SetObjectString(uint num, CSTR string, CSTR *objectList){
	if (num > 299) {
		return 0;
	}
	if (string.isDiff("(null)") == 0) {
		if (string.length() == 0) {
			objectList[num].fillzero();
			return 0;
		}
	}
	objectList[num].assign(&string);
	return 1;
}

//4b6d00
int SetObjectStringInt(int at, int val, CSTR *arr){
	cstrSprintf(&arr[at], "%d", val);
	return 1;
}

//4b6d30
CSTR GetStringFromArray(int num, CSTR *strings) {
	return strings[num];
}


//4b6d60
int DefineOptionStrNum(OptionString *arrOpStr){
	arrOpStr[19].count = 4;
	arrOpStr[5].count = 4;
	arrOpStr[14].count = 4;
	arrOpStr[15].count = 4;
	arrOpStr[22].count = 4;
	arrOpStr[7].count = 9;
	arrOpStr[8].count = 2;
	arrOpStr[9].count = 3;
	arrOpStr[13].count = 2;
	arrOpStr[2].count = 6;
	arrOpStr[6].count = 2;
	arrOpStr[3].count = 6;
	arrOpStr[0].count = 8;
	arrOpStr[4].count = 6;
	arrOpStr[16].count = 2;
	arrOpStr[12].count = 2;
	arrOpStr[10].count = 5;
	arrOpStr[1].count = 6;
	arrOpStr[11].count = 2;
	arrOpStr[17].count = 3;
	arrOpStr[18].count = 5;
	arrOpStr[20].count = 6;
	arrOpStr[23].count = 2;
	arrOpStr[21].count = 2;
	arrOpStr[24].count = 2;
	return 1;
}

//4b6e30
int ReadOptionstr(OptionString *opStr, CSVbuf csv) {
	for (int i = 0; i < opStr->count; i++) {
		opStr->str[i].assign(&csv.str[1+i]);
	}
	for (int i = opStr->count; i < 10; i++) {
		opStr->str[i].fillzero();
	}
	return 1;
}

//4b6ee0 : CSVbuf copy
//4b6f20
int ReadOptionstrFile(OptionString *arrOpStr, CSTR filepath) {
	int bufSize = 256;

	FILE *pFile;
	CSTR fBuf(bufSize);
	char* pFbuf;
	CSVbuf csv;

	DefineOptionStrNum(arrOpStr);
	pFile = fopen(filepath, "r");
	if (pFile == 0) {
		ErrorLogAdd("オプション文字列リストが見つかりません。\n");
		return 0;
	}

	pFbuf = fBuf.outstr();
	for (pFbuf = fgets(pFbuf, bufSize, pFile); pFbuf; pFbuf = fgets(pFbuf, bufSize, pFile)) {
		if (*(fBuf.atPos(0)) == '#') {
			SplitCSV(fBuf, &csv, ",");

			if (fBuf.left(5).isSame("#MODE")) {
				ReadOptionstr(&arrOpStr[0], csv);
			}
			else if (fBuf.left(5).isSame("#SORT")) {
				ReadOptionstr(&arrOpStr[1], csv);
			}
			else if (fBuf.left(11).isSame("#DIFFICULTY")) {
				ReadOptionstr(&arrOpStr[2], csv);
			}
			else if (fBuf.left(6).isSame("#GAUGE")) {
				ReadOptionstr(&arrOpStr[3], csv);
			}
			else if (fBuf.left(7).isSame("#RANDOM")) {
				ReadOptionstr(&arrOpStr[4], csv);
			}
			else if (fBuf.left(7).isSame("#ASSIST")) {
				ReadOptionstr(&arrOpStr[19], csv);
			}
			else if (fBuf.left(5).isSame("#FLIP")) {
				ReadOptionstr(&arrOpStr[6], csv);
			}
			else if (fBuf.left(7).isSame("#BATTLE")) {
				ReadOptionstr(&arrOpStr[7], csv);
			}
			else if (fBuf.left(7).isSame("#EFFECT")) {
				ReadOptionstr(&arrOpStr[5], csv);
			}
			else if (fBuf.left(6).isSame("#HSFIX")) {
				ReadOptionstr(&arrOpStr[10], csv);
			}
			else if (fBuf.left(7).isSame("#SCREEN")) {
				ReadOptionstr(&arrOpStr[12], csv);
			}
			else if (fBuf.left(6).isSame("#COLOR")) {
				ReadOptionstr(&arrOpStr[13], csv);
			}
			else if (fBuf.left(8).isSame("#BGASIZE")) {
				ReadOptionstr(&arrOpStr[8], csv);
			}
			else if (fBuf.left(8).isSame("#BGADRAW")) {
				ReadOptionstr(&arrOpStr[9], csv);
			}
			else if (fBuf.left(11).isSame("#SCOREGRAPH")) {
				ReadOptionstr(&arrOpStr[16], csv);
			}
			else if (fBuf.left(6).isSame("#GHOST")) {
				ReadOptionstr(&arrOpStr[14], csv);
			}
			else if (fBuf.left(10).isSame("#LANECOVER")) {
				ReadOptionstr(&arrOpStr[15], csv);
			}
			else if (fBuf.left(6).isSame("#VSYNC")) {
				ReadOptionstr(&arrOpStr[11], csv);
			}
			else if (fBuf.left(11).isSame("#AUTOADJUST")) {
				ReadOptionstr(&arrOpStr[17], csv);
			}
			else if (fBuf.left(11).isSame("#REPLAYSAVE")) {
				ReadOptionstr(&arrOpStr[18], csv);
			}
			else if (fBuf.left(15).isSame("#COURSE_TSUNAGI")) {
				ReadOptionstr(&arrOpStr[20], csv);
			}
			else if (fBuf.left(13).isSame("#COURSE_GAUGE")) {
				ReadOptionstr(&arrOpStr[22], csv);
			}
			else if (fBuf.left(12).isSame("#COURSE_TYPE")) {
				ReadOptionstr(&arrOpStr[21], csv);
			}
			else if (fBuf.left(17).isSame("#COURSE_OPTIONBAN")) {
				ReadOptionstr(&arrOpStr[23], csv);
			}
			else if (fBuf.left(10).isSame("#COURSE_IR")) {
				ReadOptionstr(&arrOpStr[24], csv);
			}
			*fBuf.atPos(0) = '\0';
		}
		pFbuf = fBuf.outstr();
	}

	ErrorLogAdd("オプション文字列リストを読み込みました。\n");
	return 1;
}
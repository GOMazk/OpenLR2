#include "LR2_ghost.h"
#include "structure.h"

int PLAYSCORE::InitJudgeQueue(void){
	// TODO: replace this->judge_queue with std::vector, then remove this function
	this->name.fillzero();
	this->totalnotes = 0;
	this->nownote = 0;
	this->exscore = 0;
	this->rate = 0;
	this->judge_queue_len = 0;
	this->judgeExpect[0] = 0;
	this->judge[0] = 0;
	this->judgeExpect[1] = 0;
	this->judge[1] = 0;
	this->judgeExpect[2] = 0;
	this->judge[2] = 0;
	this->judgeExpect[3] = 0;
	this->judge[3] = 0;
	this->judgeExpect[4] = 0;
	this->judge[4] = 0;
	this->judgeExpect[5] = 0;
	this->judge[5] = 0;
	this->ghostReadCount = 0;
	this->judge_queue_capacity = 0;
	free(this->judge_queue);
	this->judge_queue = NULL;
	this->judge_queue_capacity = 0;
	return 0;
}

int PLAYSCORE::ResetJudgeQueue(int size){
	free(this->judge_queue);
	this->judge_queue = NULL;
	this->judge_queue_capacity = 0;
	this->judge_queue = (char *)malloc(size);
	for (int i = 0; i < size; i++) {
		this->judge_queue[i] = -1;
	}
	this->judge_queue_capacity = size;
	return 1;
}

int PLAYSCORE::ResizeJudgeQueue(size_t size){
	this->judge_queue = (char *)realloc(this->judge_queue, size);
	for (int i = this->judge_queue_capacity; i < size; i++) {
		this->judge_queue[i] = -1;
	}
	this->judge_queue_capacity = this->judge_queue_capacity + size;
	return 1;
}

int PLAYSCORE::AddJudgeQueue(char judge){
	if (this->judge_queue_len == this->judge_queue_capacity) {
		ResizeJudgeQueue(this->judge_queue_capacity + 1000);
	}
	this->judge_queue[this->judge_queue_len] = judge;
	this->judge_queue_len = this->judge_queue_len + 1;
	return 1;
}

// TODO:rename variabale
int PLAYSCORE::DealJudgeFromQueue(void){
	byte val;
	int note;
	int ret;

	if (this->totalnotes < 1) {
		return 1;
	}
	if (this->judge_queue_capacity != 0) {
		note = this->judge_queue_len;
		ret = 1;
		if ((note != this->judge_queue_capacity) && (val = this->judge_queue[note], val != 0xff)) {
			if ((val != 0) && (val < 6)) {
				this->judge[val]++;
				this->judge_queue_len++;
				this->nownote++;
				this->rate = this->judge[4] + this->judge[5] * 2;
				return ret;
			}
			if (val == 0) {
				this->judge[1]++;
				this->rate = this->judge[4] + this->judge[5] * 2;
				this->judge_queue_len++;
				ret = 0;
			}
		}
		return ret;
	}
	if (this->nownote > 0) {
		this->judge[0] = ((this->nownote + 1) * this->judgeExpect[0]) / this->totalnotes;
		this->judge[1] = ((this->nownote + 1) * this->judgeExpect[1]) / this->totalnotes;
		this->judge[2] = ((this->nownote + 1) * this->judgeExpect[2]) / this->totalnotes;
		this->judge[3] = ((this->nownote + 1) * this->judgeExpect[3]) / this->totalnotes;
		this->judge[4] = ((this->nownote + 1) * this->judgeExpect[4]) / this->totalnotes;
		this->judge[5] = ((this->nownote + 1) * this->judgeExpect[5]) / this->totalnotes;
	}
	this->rate = this->judge[4] + this->judge[5] * 2;
	this->nownote++;
	return 1;
}

char PLAYSCORE::GetJudgeFromQueue(void){
	if (this->judge_queue_capacity == 0) {
		return -1;
	}
	if (this->judge_queue_len <= 0) {
		return *this->judge_queue;
	}
	return this->judge_queue[this->judge_queue_len + -1];
}

int PLAYSCORE::SetDefaultGhost(int target, int notes){
	int tgPerfect;

	ErrorLogFmtAdd("デフォルトゴーストを設定します...");
	InitJudgeQueue();
	this->totalnotes = notes;
	if (target == 88) {
		tgPerfect = ((double)notes * 7.0) / 9.0;
		this->judgeExpect[5] = tgPerfect;
		this->judgeExpect[4] = notes - tgPerfect;
		this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
		this->name = "RANK AAA";
	}
	else if (target == 77) {
		tgPerfect = ((double)notes * 5.0) / 9.0;
		this->judgeExpect[5] = tgPerfect;
		this->judgeExpect[4] = notes - tgPerfect;
		this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
		this->name = "RANK AA";
	}
	else if (target == 66) {
		tgPerfect = ((double)notes * 3.0) / 9.0;
		this->judgeExpect[5] = tgPerfect;
		this->judgeExpect[4] = notes - tgPerfect;
		this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
		this->name = "RANK A";
	}
	else {
		if (50 <= target && target <= 100) {
			tgPerfect = (((double)target + (double)target) - 100.0) * (double)notes;
			tgPerfect = tgPerfect / 100;
			this->judgeExpect[4] = notes - tgPerfect;
			this->judgeExpect[5] = tgPerfect;
			this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
			cstrSprintf(&this->name, "%d%%", target);
		}
		else {
			this->name = "NO TARGET";
		}
	}
	ErrorLogFmtAdd("ゴーストを設定しました\n");
	return 1;
}

int PLAYSCORE::SetGhost(int exscore, int notes, CSTR name){
	ErrorLogFmtAdd("exスコアからゴーストを設定します...");
	InitJudgeQueue();
	this->totalnotes = notes;
	this->name = name;
	this->judgeExpect[5] = RoundUp(((double)exscore / (double)notes - 1.0) * (double)this->totalnotes);
	this->judgeExpect[4] = this->totalnotes - this->judgeExpect[5];
	this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
	ErrorLogFmtAdd("ゴーストを設定しました\n");
	return 1;
}

CSTR PLAYSCORE::EncodeGhostData(void) {
	if (this->judge_queue_len == 0) return "GHOST_ERROR";

	int rep = 0;
	CSTR buf;
	CSTR data(this->judge_queue_len + 3);
	char last = this->judge_queue[0];
	for (int i = 0; i < judge_queue_len; i++) {
		if (last == this->judge_queue[i]) {
			rep++;
		}
		else {
			switch (last) {
				case 0:
					data.add("@");
					break;
				case 1:
					data.add("A");
					break;
				case 2:
					data.add("B");
					break;
				case 3:
					data.add("C");
					break;
				case 4:
					data.add("D");
					break;
				case 5:
					data.add("E");
					break;
				default:
					data.add("-");
					break;
			}
			if (rep >= 2) {
				cstrSprintf(&buf, "%d", rep);
				data.add(buf);
			}
			last = this->judge_queue[i];
			rep = 1;
		}
	}

	switch (last) {
		case 0:
			data.add("@");
			break;
		case 1:
			data.add("A");
			break;
		case 2:
			data.add("B");
			break;
		case 3:
			data.add("C");
			break;
		case 4:
			data.add("D");
			break;
		case 5:
			data.add("E");
			break;
		default:
			data.add("-");
			break;
	}
	if (rep >= 2) {
		cstrSprintf(&buf, "%d", rep);
		data.add(buf);
	}
	data.add("ZZZ");

	int o = 0;
	CSTR str(data.length() + 3);
	for (int i = 1; i < data.length(); i++) {
		if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '1') {
			str.add("F");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '2') {
			str.add("G");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '3') {
			str.add("H");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '4') {
			str.add("I");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '5') {
			str.add("J");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '6') {
			str.add("K");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '7') {
			str.add("L");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '8') {
			str.add("M");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == '9') {
			str.add("N");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == 'C') {
			str.add("P");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == 'B') {
			str.add("Q");
			i++;
		}
		else if (*data.atPos(i - 1) == 'E' && *data.atPos(i) == 'A') {
			str.add("R");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '2') {
			str.add("S");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '3') {
			str.add("T");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '4') {
			str.add("U");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '5') {
			str.add("V");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == '6') {
			str.add("W");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == 'E') {
			str.add("X");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == 'C') {
			str.add("Y");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == 'B') {
			str.add("a");
			i++;
		}
		else if (*data.atPos(i - 1) == 'D' && *data.atPos(i) == 'A') {
			str.add("b");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == '2') {
			str.add("c");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == '3') {
			str.add("d");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == '4') {
			str.add("e");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == '5') {
			str.add("f");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == 'E') {
			str.add("g");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == 'D') {
			str.add("h");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == 'B') {
			str.add("i");
			i++;
		}
		else if (*data.atPos(i - 1) == 'C' && *data.atPos(i) == 'A') {
			str.add("j");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == 'B') {
			str.add("k");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == 'C') {
			str.add("l");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == 'D') {
			str.add("m");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == 'E') {
			str.add("n");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == '2') {
			str.add("o");
			i++;
		}
		else if (*data.atPos(i - 1) == 'A' && *data.atPos(i) == '3') {
			str.add("p");
			i++;
		}
		else {
			*str.atPos(o) = *data.atPos(i - 1);
		}
		o++;
	}

	data = str;
	str.fillzero();
	str.resize2(data.length() + 3);
	o = 0;
	for (int i = 1; i < data.length(); i++) {
		if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == 'X') {
			str.add("q");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '1') {
			str.add("r");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '2') {
			str.add("s");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '3') {
			str.add("t");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '4') {
			str.add("u");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '5') {
			str.add("v");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '6') {
			str.add("w");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '7') {
			str.add("x");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '8') {
			str.add("y");
			i++;
		}
		else if (*data.atPos(i - 1) == 'X' && *data.atPos(i) == '9') {
			str.add("z");
			i++;
		}
		else {
			*str.atPos(o) = *data.atPos(i - 1);
		}

		o++;
	}

	this->name.fillzero();
	this->totalnotes = 0;
	this->nownote = 0;
	this->exscore = 0;
	this->rate = 0;
	this->judge_queue_len = 0;
	this->judgeExpect[0] = 0;
	this->judge[0] = 0;
	this->judgeExpect[1] = 0;
	this->judge[1] = 0;
	this->judgeExpect[2] = 0;
	this->judge[2] = 0;
	this->judgeExpect[3] = 0;
	this->judge[3] = 0;
	this->judgeExpect[4] = 0;
	this->judge[4] = 0;
	this->judgeExpect[5] = 0;
	this->judge[5] = 0;
	this->ghostReadCount = 0;
	this->judge_queue_capacity = 0;
	free(this->judge_queue);
	this->judge_queue = NULL;
	this->judge_queue_capacity = 0;

	return str;
}

int PLAYSCORE::DecodeGhostData(CSTR data) {
	CSTR decode;

	for (int i = 0; i < data.length(); i++) {
		switch (*data.atPos(i)) {
			case 'q':
				decode.add("XX");
				break;
			case 'r':
				decode.add("X1");
				break;
			case 's':
				decode.add("X2");
				break;
			case 't':
				decode.add("X3");
				break;
			case 'u':
				decode.add("X4");
				break;
			case 'v':
				decode.add("X5");
				break;
			case 'w':
				decode.add("X6");
				break;
			case 'x':
				decode.add("X7");
				break;
			case 'y':
				decode.add("X8");
				break;
			case 'z':
				decode.add("X9");
				break;
			default:
				decode.add(data.getSliced(i, 1));
				break;
		}
	}
	data = decode;
	decode.fillzero();
	for (int i = 0; i < data.length(); i++) {
		switch (*data.atPos(i)) {
			case 'F':
				decode.add("E1");
				break;
			case 'G':
				decode.add("E2");
				break;
			case 'H':
				decode.add("E3");
				break;
			case 'I':
				decode.add("E4");
				break;
			case 'J':
				decode.add("E5");
				break;
			case 'K':
				decode.add("E6");
				break;
			case 'L':
				decode.add("E7");
				break;
			case 'M':
				decode.add("E8");
				break;
			case 'N':
				decode.add("E9");
				break;
			case 'P':
				decode.add("EC");
				break;
			case 'Q':
				decode.add("EB");
				break;
			case 'R':
				decode.add("EA");
				break;
			case 'S':
				decode.add("D2");
				break;
			case 'T':
				decode.add("D3");
				break;
			case 'U':
				decode.add("D4");
				break;
			case 'V':
				decode.add("D5");
				break;
			case 'W':
				decode.add("D6");
				break;
			case 'X':
				decode.add("DE");
				break;
			case 'Y':
				decode.add("DC");
				break;
			case 'a':
				decode.add("DB");
				break;
			case 'b':
				decode.add("DA");
				break;
			case 'c':
				decode.add("C2");
				break;
			case 'd':
				decode.add("C3");
				break;
			case 'e':
				decode.add("C4");
				break;
			case 'f':
				decode.add("C5");
				break;
			case 'g':
				decode.add("CE");
				break;
			case 'h':
				decode.add("CD");
				break;
			case 'i':
				decode.add("CB");
				break;
			case 'j':
				decode.add("CA");
				break;
			case 'k':
				decode.add("AB");
				break;
			case 'l':
				decode.add("AC");
				break;
			case 'm':
				decode.add("AD");
				break;
			case 'n':
				decode.add("AE");
				break;
			case 'o':
				decode.add("A2");
				break;
			case 'p':
				decode.add("A3");
				break;
			default:
				decode.add(data.getSliced(i, 1));
				break;
		}
	}
	data = decode;
	decode.fillzero();
	int rep = -1;
	int pos = 0;
	for (int i = 0; i < data.length(); i++) {
		if (*data.atPos(i) >= 0x40 && *data.atPos(i) <= 0x45) {
			if (rep >= 0) {
				if (rep == 0) rep = 1;
				while (rep > 0) {
					decode.add(data.getSliced(pos, 1));
					rep--;
				}
			}
			rep = 0;
			pos = i;
		}
		else if (*data.atPos(i) >= '0' && *data.atPos(i) <= '9') {
			if (rep == 0) rep = *data.atPos(i) - 0x30;
			else rep = rep*10 + (*data.atPos(i) - 0x30);
		}

	}
	if (rep == 0) rep = 1;
	while(rep > 0) {
		decode.add(data.getSliced(pos, 1));
		rep--;
	}

	InitJudgeQueue();

	this->judge_queue = static_cast<char *>(calloc(1, decode.length() + 1));

	for (int i = 0; i < decode.length() + 1; i++) {
		this->judge_queue[i] = -1;
	}

	this->judge_queue_capacity = decode.length() + 1;

	for (int i = 0; i < decode.length(); i++) {
		this->judge_queue[i] = *decode.atPos(i) + -0x40;
		this->ghostReadCount++;
	}

	for (int i = 0; i < this->ghostReadCount; i++) {
		char judge = this->judge_queue[i];
		if (judge == 0) {
			this->judgeExpect[1]++;
		}
		else if ((judge != 0) && (judge < 6)) {
			this->judgeExpect[(byte)this->judge_queue[i]]++;
			this->totalnotes++;
		}
	}

	this->exscore = this->judgeExpect[4] + this->judgeExpect[5] * 2;
	ErrorLogFmtAdd("ゴーストデータ復元完了 totalnotes %d exscore_max %d\n", this->totalnotes, this->exscore);

	return 1;
}

int PLAYSCORE::SetScore(PLAYERSTATUS *pstat, char flagExpect) {
	if (this->totalnotes <= 0) {
		ErrorLogAdd("SetScore::TOTALNOTESが0です\n");
		return 1;
	}
	if (flagExpect) {
		pstat->judgecount[0] = this->judgeExpect[0];
		pstat->judgecount[1] = this->judgeExpect[1];
		pstat->judgecount[2] = this->judgeExpect[2];
		pstat->judgecount[3] = this->judgeExpect[3];
		pstat->judgecount[4] = this->judgeExpect[4];
		pstat->judgecount[5] = this->judgeExpect[5];
		pstat->exscore = this->exscore;
		pstat->score = ((pstat->judgecount[3] + (pstat->judgecount[4] + pstat->judgecount[5] * 2) * 2) * 50000) / this->totalnotes;
		pstat->now_combo = this->totalnotes;
		pstat->rate = (double)((pstat->exscore * 100) / (double)(this->totalnotes * 2));
		return 1;
	}
	pstat->judgecount[0] = this->judge[0];
	pstat->judgecount[1] = this->judge[1];
	pstat->judgecount[2] = this->judge[2];
	pstat->judgecount[3] = this->judge[3];
	pstat->judgecount[4] = this->judge[4];
	pstat->judgecount[5] = this->judge[5];
	pstat->exscore = this->rate;
	pstat->score = ((pstat->judgecount[3] + (pstat->judgecount[4] + pstat->judgecount[5] * 2) * 2) * 50000) / this->totalnotes;
	pstat->now_combo = this->nownote;
	pstat->rate = (double)((pstat->exscore * 100) / (double)(this->totalnotes * 2));
	return 1;
}


int WriteGhostInDatabase(sqlite3 *sql, CSTR songMD5, PLAYSCORE *score) {
	ErrorLogAdd("データベースにゴーストを書き込みます\n");

	CSTR ghostdata = score->EncodeGhostData();

	sqlite3_stmt *pStmt;
	if (sqlite3_prepare_v3(sql, "UPDATE score SET ghost = ? WHERE hash = ?", -1, 0, &pStmt, nullptr) != SQLITE_OK) {
		ErrorLogAdd("sqlite3_prepare_v3 error\n");
	}
	if (sqlite3_bind_text(pStmt, 1, ghostdata.body, ghostdata.length(), nullptr) != SQLITE_OK) {
		ErrorLogAdd("sqlite3_bind_text error\n");
	}
	if (sqlite3_bind_text(pStmt, 2, songMD5.body, songMD5.length(), nullptr) != SQLITE_OK) {
		ErrorLogAdd("sqlite3_bind_text error\n");
	}
	if (sqlite3_step(pStmt) != SQLITE_DONE) {
		ErrorLogAdd("sqlite3_step error\n");
	}
	if (sqlite3_finalize(pStmt) != SQLITE_OK) {
		ErrorLogAdd("sqlite3_finalize error\n");
	}

	ErrorLogAdd("ゴーストの書き込みが終了しました\n");
	return 0;
}

CSTR ReadGhost(sqlite3 *sql, CSTR songMD5, bool useImportedGhost) {
	ErrorLogAdd("データベースからゴーストを読み込みます\n");

	const std::string query = std::format("SELECT ghost FROM {} WHERE hash = ?", useImportedGhost ? "imported_score" : "score");
	
	sqlite3_stmt *pStmt;
	if (sqlite3_prepare_v3(sql, query.c_str(), -1, 0, &pStmt, nullptr) != SQLITE_OK) {
		ErrorLogAdd("sqlite3_prepare_v3 error\n");
	}
	if (sqlite3_bind_text(pStmt, 1, songMD5.body, songMD5.length(), nullptr) != SQLITE_OK) {
		ErrorLogAdd("sqlite3_bind_text error\n");
	}

	CSTR ghostdata;
	if (sqlite3_step(pStmt) == SQLITE_ROW) {
		ghostdata.resize(sqlite3_column_bytes(pStmt, 0));
		// Cast safety: it's safe to cast from unsigned char* to char*
		ghostdata = reinterpret_cast<const char*>(sqlite3_column_text(pStmt, 0));
	}
	if (sqlite3_finalize(pStmt) != SQLITE_OK) {
		ErrorLogAdd("sqlite3_finalize error\n");
	}

	return ghostdata;
}

int ReadGhostToScore(sqlite3 *sql, CSTR songMD5, PLAYSCORE *score, bool useImportedGhost) {
	CSTR ghostdata = ReadGhost(sql, songMD5, useImportedGhost);

	if (ghostdata.length() == 0) {
		score->InitJudgeQueue();
		return 1;
	}
	
	score->DecodeGhostData(ghostdata);
	return 1;
}

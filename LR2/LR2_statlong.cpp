#include "LR2_statlong.h"
#include "filesystem.h"

#ifndef _WIN32
#include <iostream>
static void MessageBoxA(const char*,const char* title,const char*desc,const char*)
{
	std::cout << "\n" << title << "\n\n" << desc << "\n" << std::flush;
}
#endif // _WIN32

CSTR MakePlayerStatHash(PLAYERSTATISTIC *ps) {
	CSTR tmp;
	cstrSprintf(&tmp, "%d%d%d%d%d%d%d%d%d%d%d%s%d%d%d%d%d%d%d%d%d%d%d", ps->bad, ps->clear, ps->combo, ps->fail, ps->good, ps->great, ps->maxcombo, ps->perfect, ps->playcount, ps->playtime, ps->poor, ps->passMD5.body,
		ps->grade9, ps->grade10, ps->grade14, ps->grade5, ps->grade7, ps->gradeversion, ps->trial, ps->trialversion, ps->systemversion, ps->option, 1);
	return MD5str(tmp);
}

int UpdatePlayerStat(PLAYERSTATISTIC *ps, sqlite3 *sql) {
	char query[1024];
	CSTR hash = MakePlayerStatHash(ps);
	sqlite3_snprintf(1024, query, "UPDATE player SET playcount= %d , clear = %d , fail = %d , perfect = %d , great = %d , good = %d , bad = %d , poor = %d , playtime = %d , combo = %d , maxcombo = %d,scorehash=\'%q\' ,grade_7=%d,grade_5=%d,grade_14=%d,grade_10=%d,grade_9=%d,trial = %d , option=%d,systemversion=%d,gradeversion=%d,trialversion=%d",
		ps->playcount, ps->clear, ps->fail, ps->perfect, ps->great, ps->good, ps->bad, ps->poor, ps->playtime, ps->combo, ps->maxcombo, hash.body,
		ps->grade7, ps->grade5, ps->grade14, ps->grade10, ps->grade9, ps->trial, ps->option, ps->systemversion, ps->gradeversion, ps->trialversion);
	SQL_Run(query, sql);
	return 1;
}

int ReadPlayerScore(CSTR id, CSTR pass, PLAYERSTATISTIC *pstat) {

	CSTR dbPath, passMD5, query, MD5inDB;
	char str[256];
	sqlite3 *scoreDB;
	sqlite3_stmt *stmt;

	cstrSprintf(&dbPath, fs::make_preferred("LR2files/Database/Score/%s.db").data(), id.body);
	passMD5 = MD5str(pass);
	
	sqlite3_open(dbPath, &scoreDB);
	ErrorLogFmtAdd("成功\n");

	sqlite3_snprintf(256, str, "SELECT * FROM player WHERE id = \'%q\'", id.body);
	query = str;

	SQL_prepare(query, scoreDB, &stmt);
	
	if (sqlite3_step(stmt) != 100) {
		MessageBoxA(NULL, "スコアデータベースが壊れています。config.exeで再作成して下さい。", "エラー", 0);
		sqlite3_finalize(stmt);
		sqlite3_close(scoreDB);
		return 0;
	}

	constexpr const char* createTableQuery =
		"CREATE TABLE IF NOT EXISTS imported_score ("
		"hash TEXT PRIMARY KEY, "
		"clear INTEGER, "
		"perfect INTEGER, "
		"great INTEGER, "
		"good INTEGER, "
		"bad INTEGER, "
		"poor INTEGER, "
		"totalnotes INTEGER, "
		"maxcombo INTEGER, "
		"minbp INTEGER, "
		"playcount INTEGER, "
		"clearcount INTEGER, "
		"failcount INTEGER, "
		"rank INTEGER, "
		"rate INTEGER, "
		"clear_db INTEGER, "
		"op_history INTEGER, "
		"scorehash TEXT, "
		"ghost TEXT, "
		"clear_sd INTEGER, "
		"clear_ex INTEGER, "
		"op_best INTEGER, "
		"rseed INTEGER, "
		"complete INTEGER"
		");";

	if (sqlite3_exec(scoreDB, createTableQuery, nullptr, nullptr, nullptr) != SQLITE_OK) {
		MessageBoxA(NULL, "Error creating IR Derived Score Table", "Error", 0);
		sqlite3_close(scoreDB);
		return 0;
	}

	MD5inDB = SQL_GetColumn(1, stmt);
	if (passMD5.isSame(&MD5inDB) == 0) {
		MessageBoxA(NULL, "パスワードが違います。", "エラー", 0);
		sqlite3_finalize(stmt);
		sqlite3_close(scoreDB);
		return 0;
	}

	pstat->passMD5 = MD5inDB;
	pstat->irid = sqlite3_column_int(stmt, 3);
	pstat->irname = SQL_GetColumn(4, stmt);
	pstat->playcount = sqlite3_column_int(stmt, 5);
	pstat->clear = sqlite3_column_int(stmt, 6);
	pstat->fail = sqlite3_column_int(stmt, 7);
	pstat->perfect = sqlite3_column_int(stmt, 8);
	pstat->great = sqlite3_column_int(stmt, 9);
	pstat->good = sqlite3_column_int(stmt, 10);
	pstat->bad = sqlite3_column_int(stmt, 11);
	pstat->poor = sqlite3_column_int(stmt, 12);
	pstat->playtime = sqlite3_column_int(stmt, 13);
	pstat->combo = sqlite3_column_int(stmt, 14);
	pstat->maxcombo = sqlite3_column_int(stmt, 15);
	pstat->grade7 = sqlite3_column_int(stmt, 16);
	pstat->grade5 = sqlite3_column_int(stmt, 17);
	pstat->grade14 = sqlite3_column_int(stmt, 18);
	pstat->grade10 = sqlite3_column_int(stmt, 19);
	pstat->grade9 = sqlite3_column_int(stmt, 20);
	pstat->trial = sqlite3_column_int(stmt, 21);
	pstat->option = sqlite3_column_int(stmt, 22);
	pstat->systemversion = sqlite3_column_int(stmt, 23);
	pstat->gradeversion = sqlite3_column_int(stmt, 24);
	pstat->trialversion = sqlite3_column_int(stmt, 25);
	MD5inDB = SQL_GetColumn(26, stmt);//scorehash
	
	if (pstat->systemversion != 1) {
		MessageBoxA(NULL, "旧バージョンに上書きは出来ません。", "エラー", 0);
		sqlite3_finalize(stmt);
		sqlite3_close(scoreDB);
		return 0;
	}

	if (MakePlayerStatHash(pstat).isDiff(&MD5inDB)) {
		pstat->playcount = 0;
		pstat->clear = 0;
		pstat->fail = 0;
		pstat->perfect = 0;
		pstat->great = 0;
		pstat->good = 0;
		pstat->bad = 0;
		pstat->poor = 0;
		pstat->playtime = 0;
		pstat->combo = 0;
		pstat->maxcombo = 0;
		pstat->grade7 = 0;
		pstat->grade5 = 0;
		pstat->grade14 = 0;
		pstat->grade10 = 0;
		pstat->grade9 = 0;
		pstat->trial = 1;
		pstat->option = 0;
		pstat->systemversion = 1;
		pstat->gradeversion = 0;
		pstat->trialversion = 0;
		ErrorLogAdd("ハッシュが不正です。プレイヤースコアデータを初期化しました。\n");
	}
	else {
		if (pstat->gradeversion != 0) {
			pstat->grade7 = 0;
			pstat->grade5 = 0;
			pstat->grade14 = 0;
			pstat->grade10 = 0;
			pstat->grade9 = 0;
			pstat->gradeversion = 0;
			ErrorLogAdd("段位認定が更新されたのでデータを初期化しました。\n");
			if (pstat->gradeversion != 0) {
				pstat->trial = 1;
				pstat->trialversion = 0;
				ErrorLogAdd("トライアルが更新されたのでデータを初期化しました。\n");
			}
		}
		if (pstat->trial < 1) {
			pstat->trial = 1;
		}
		pstat->systemversion = 1;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(scoreDB);
	return 1;
}

CSTR MakeScoreHash(STATUS *stat, CSTR *passMD5, CSTR *songMD5) {
	CSTR str;

	cstrSprintf(&str, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%s%s%d", stat->playcount, stat->stat_pgreat,
		stat->stat_poor, stat->stat_good, stat->stat_great, stat->stat_bad, stat->total_notes,
		stat->clear, stat->stat_maxcombo, stat->clearcount, stat->failcount, stat->rank,
		stat->minbp, stat->rate, stat->clear_db, stat->op_history, passMD5->body, songMD5->body, 1);

	return MD5str(str);
}

bool isSameScoreHash(STATUS *stat, CSTR *passMD5, CSTR *songMD5, CSTR *besthash) {
	CSTR newhash, althash;

	newhash = MakeScoreHash(stat, passMD5, songMD5);
	if (newhash.isSame(besthash)) return true;

	if (stat->stat_pgreat == stat->total_notes && stat->rank == 8) {
		stat->rank = 9;
		newhash = MakeScoreHash(stat, passMD5, songMD5);
		if (newhash.isSame(besthash)) {
			stat->rank = 8;
			return true;
		}
	}
	if (stat->stat_pgreat == stat->total_notes && stat->rank == 9) {
		stat->rank = 8;
		newhash = MakeScoreHash(stat, passMD5, songMD5);
		if (newhash.isSame(besthash)) {
			return true;
		}
	}

	if (stat->rank == 1) {
		stat->rank = 0;
		newhash = MakeScoreHash(stat, passMD5, songMD5);
		if (newhash.isSame(besthash)) {
			stat->rank = 1;
			return true;
		}
	}
	if (stat->rank == 0) {
		stat->rank = 1;
		newhash = MakeScoreHash(stat, passMD5, songMD5);
		if (newhash.isSame(besthash)) {
			return true;
		}
	}

	return false;
}

int UpdateScoreDB(CSTR hash, STATUS *stat, sqlite3 *sql, CSTR *passMD5) {

	if (stat->rank > 7) stat->rank = 8;
	if (stat->rank < 1 && stat->stat_great + stat->stat_pgreat > 0) stat->rank = 1;
	CSTR query;
	CSTR scoreMD5(MakeScoreHash(stat, passMD5, &hash));
	sqlite3_exec(sql, "BEGIN", 0, 0, 0);
	cstrSprintf(&query, "INSERT INTO score (hash,clear ,perfect ,great ,good ,bad ,poor ,totalnotes ,maxcombo ,minbp ,playcount ,clearcount ,failcount ,op_history , rank , rate , clear_db , scorehash , clear_sd , clear_ex , op_best , rseed , complete) VALUES(\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d)"
		, hash.body, stat->clear, stat->stat_pgreat, stat->stat_great, stat->stat_good, stat->stat_bad, stat->stat_poor, stat->total_notes, stat->stat_maxcombo, stat->minbp, stat->playcount, stat->clearcount, stat->failcount, stat->op_history, stat->rank, stat->rate, stat->clear_db, scoreMD5.body, stat->clear_sd, stat->clear_ex, stat->op_best, stat->rseed, stat->complete);
	if (SQL_Run(query, sql)){
		cstrSprintf(&query, "UPDATE score SET clear=%d ,perfect=%d ,great=%d ,good=%d ,bad=%d ,poor=%d ,totalnotes=%d ,maxcombo=%d ,minbp=%d ,playcount=%d ,clearcount=%d ,failcount=%d ,op_history=%d ,scorehash=\'%s\',rank=%d,rate=%d,clear_db=%d,clear_sd=%d,clear_ex=%d,op_best=%d,rseed=%d,complete=%d WHERE hash = \'%s\'"
			, stat->clear, stat->stat_pgreat, stat->stat_great, stat->stat_good, stat->stat_bad, stat->stat_poor, stat->total_notes, stat->stat_maxcombo, stat->minbp, stat->playcount, stat->clearcount, stat->failcount, stat->op_history, scoreMD5.body, stat->rank, stat->rate, stat->clear_db, stat->clear_sd, stat->clear_ex, stat->op_best, stat->rseed, stat->complete, hash.body);
		SQL_Run(query, sql);
	}

	// delete imported score from IR
	CSTR delQuery;
	cstrSprintf(&delQuery, "DELETE FROM imported_score WHERE hash = \'%s\'", hash.body);
	SQL_Run(delQuery, sql);
	sqlite3_exec(sql, "COMMIT", 0, 0, 0);
	return 1;
}


int DeleteScoreFromDB(CSTR hash, sqlite3 *sql){
	
	char str[256];

	if (hash.length() == 32) {
		sqlite3_snprintf(0x100, str, "DELETE FROM score WHERE hash=\'%q\'", hash.body);
		SQL_Run(str, (sqlite3 *)sql);
		return 1;
	}
	else return 0;
}

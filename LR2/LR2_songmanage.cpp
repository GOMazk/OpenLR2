#include "LR2_songmanage.h"
#include "Engine.h"
#include "LR2_statlong.h"
#include "filesystem.h"
#include "filesystem.h"
#include <iterator>
#include <vector>

#ifdef _WIN32
#include <shellapi.h>
#endif // _WIN32

int EnabledInsane;
// thiscall in original code
SONGDATA * COPY_SONGDATA(SONGDATA *s1, SONGDATA *s2){
	s1->title = s2->title;
	s1->subtitle = s2->subtitle;
	s1->genre = s2->genre;
	s1->artist = s2->artist;
	s1->subartist = s2->subartist;
	s1->filepath = s2->filepath;
	s1->fulltitle = s2->fulltitle;
	s1->tag = s2->tag;
	s1->hash = s2->hash;
	s1->folder = s2->folder;
	s1->stagefile = s2->stagefile;
	s1->banner = s2->banner;
	s1->backBMP = s2->backBMP;
	s1->isStagefile = s2->isStagefile;
	s1->isBanner = s2->isBanner;
	s1->isBackBMP = s2->isBackBMP;
	s1->difficulty = s2->difficulty;
	s1->level = s2->level;
	s1->exlevel = s2->exlevel;
	s1->keymode = s2->keymode;
	s1->folderType = s2->folderType;
	s1->maxBPM = s2->maxBPM;
	s1->minBPM = s2->minBPM;
	s1->bga = s2->bga;
	s1->txt = s2->txt;
	s1->longnote = s2->longnote;
	s1->random = s2->random;
	s1->judge = s2->judge;
	s1->replayExist = s2->replayExist;
	s1->favorite = s2->favorite;
	s1->adddate = s2->adddate;
	s1->difficultyLevel[0] = s2->difficultyLevel[0];
	s1->difficultyLevel[1] = s2->difficultyLevel[1];
	s1->difficultyLevel[2] = s2->difficultyLevel[2];
	s1->difficultyLevel[3] = s2->difficultyLevel[3];
	s1->difficultyLevel[4] = s2->difficultyLevel[4];
	s1->difficultyExist[0] = s2->difficultyExist[0];
	s1->difficultyExist[1] = s2->difficultyExist[1];
	s1->difficultyExist[2] = s2->difficultyExist[2];
	s1->difficultyExist[3] = s2->difficultyExist[3];
	s1->difficultyExist[4] = s2->difficultyExist[4];
	s1->difficultyLevelBarLamp[0] = s2->difficultyLevelBarLamp[0];
	s1->difficultyLevelBarLamp[1] = s2->difficultyLevelBarLamp[1];
	s1->difficultyLevelBarLamp[2] = s2->difficultyLevelBarLamp[2];
	s1->difficultyLevelBarLamp[3] = s2->difficultyLevelBarLamp[3];
	s1->difficultyLevelBarLamp[4] = s2->difficultyLevelBarLamp[4];
	s1->difficultyCount = s2->difficultyCount;

	for (int i = 0; i < 10; i++) {
		s1->courseTitle[i] = s2->courseTitle[i];
	}
	for (int i = 0; i < 10; i++) {
		s1->courseSubtitle[i] = s2->courseSubtitle[i];
	}
	for (int i = 0; i < 10; i++) {
		s1->courseHash[i] = s2->courseHash[i];
	}

	s1->courseLevel[0] = s2->courseLevel[0];
	s1->courseLevel[1] = s2->courseLevel[1];
	s1->courseLevel[2] = s2->courseLevel[2];
	s1->courseLevel[3] = s2->courseLevel[3];
	s1->courseLevel[4] = s2->courseLevel[4];
	s1->courseLevel[5] = s2->courseLevel[5];
	s1->courseLevel[6] = s2->courseLevel[6];
	s1->courseLevel[7] = s2->courseLevel[7];
	s1->courseLevel[8] = s2->courseLevel[8];
	s1->courseLevel[9] = s2->courseLevel[9];
	s1->courseStageDifficulty[0] = s2->courseStageDifficulty[0];
	s1->courseStageDifficulty[1] = s2->courseStageDifficulty[1];
	s1->courseStageDifficulty[2] = s2->courseStageDifficulty[2];
	s1->courseStageDifficulty[3] = s2->courseStageDifficulty[3];
	s1->courseStageDifficulty[4] = s2->courseStageDifficulty[4];
	s1->courseStageDifficulty[5] = s2->courseStageDifficulty[5];
	s1->courseStageDifficulty[6] = s2->courseStageDifficulty[6];
	s1->courseStageDifficulty[7] = s2->courseStageDifficulty[7];
	s1->courseStageDifficulty[8] = s2->courseStageDifficulty[8];
	s1->courseStageDifficulty[9] = s2->courseStageDifficulty[9];
	s1->courseTotalnote[0] = s2->courseTotalnote[0];
	s1->courseTotalnote[1] = s2->courseTotalnote[1];
	s1->courseTotalnote[2] = s2->courseTotalnote[2];
	s1->courseTotalnote[3] = s2->courseTotalnote[3];
	s1->courseTotalnote[4] = s2->courseTotalnote[4];
	s1->courseTotalnote[5] = s2->courseTotalnote[5];
	s1->courseTotalnote[6] = s2->courseTotalnote[6];
	s1->courseTotalnote[7] = s2->courseTotalnote[7];
	s1->courseTotalnote[8] = s2->courseTotalnote[8];
	s1->courseTotalnote[9] = s2->courseTotalnote[9];
	s1->courseID = s2->courseID;
	s1->courseStageCount = s2->courseStageCount;
	s1->coursePlayable = s2->coursePlayable;
	s1->courseReadOnly = s2->courseReadOnly;
	s1->courseKeys[0] = s2->courseKeys[0];
	s1->courseKeys[1] = s2->courseKeys[1];
	s1->courseKeys[2] = s2->courseKeys[2];
	s1->courseKeys[3] = s2->courseKeys[3];
	s1->courseKeys[4] = s2->courseKeys[4];
	s1->courseKeys[5] = s2->courseKeys[5];
	s1->courseKeys[6] = s2->courseKeys[6];
	s1->courseKeys[7] = s2->courseKeys[7];
	s1->courseKeys[8] = s2->courseKeys[8];
	s1->courseKeys[9] = s2->courseKeys[9];
	s1->courseType = s2->courseType;
	s1->courseIR = s2->courseIR;
	s1->grHandle = s2->grHandle;
	
	memcpy(&s1->mybest, &s2->mybest, sizeof(STATUS));
	memcpy(&s1->rivalRecord, &s2->rivalRecord, sizeof(STATUS));
	return s1;
}

int InitSongData(SONGDATA *song){
	song->artist.fillzero();
	song->genre.fillzero();
	song->level = 0;
	song->filepath.fillzero();
	song->subartist.fillzero();
	song->subtitle.fillzero();
	song->title.fillzero();
	song->fulltitle.fillzero();
	song->tag.fillzero();
	song->hash.fillzero();
	song->stagefile.fillzero();
	song->banner.fillzero();
	song->backBMP.fillzero();
	song->folder.fillzero();
	song->difficulty = 0;
	song->folderType = 0;
	song->keymode = 0;
	song->maxBPM = 0;
	song->minBPM = 0;
	song->bga = 0;
	song->txt = 0;
	song->longnote = 0;
	song->random = 0;
	song->judge = 0;
	song->isStagefile = 0;
	song->isBanner = 0;
	song->isBackBMP = 0;
	song->replayExist = 0;
	song->favorite = 0;
	song->adddate = 0;
	song->difficultyCount = 0;
	song->exlevel = 0;
	song->difficultyLevel[0] = -1;
	song->difficultyExist[0] = 0;
	song->difficultyLevelBarLamp[0] = -1;
	song->difficultyLevel[1] = -1;
	song->difficultyExist[1] = 0;
	song->difficultyLevelBarLamp[1] = -1;
	song->difficultyLevel[2] = -1;
	song->difficultyExist[2] = 0;
	song->difficultyLevelBarLamp[2] = -1;
	song->difficultyLevel[3] = -1;
	song->difficultyExist[3] = 0;
	song->difficultyLevelBarLamp[3] = -1;
	song->difficultyLevel[4] = -1;
	song->difficultyExist[4] = 0;
	song->difficultyLevelBarLamp[4] = -1;
	for (int i = 0; i < 10; i++) {
		song->courseTitle[i].fillzero();
		song->courseSubtitle[i].fillzero();
		song->courseHash[i].fillzero();
		song->courseLevel[i] = -1;
		song->courseTotalnote[i] = 0;
		song->courseStageDifficulty[i] = 0;
		song->courseKeys[i] = 0;
	}
	song->courseID = -1;
	song->courseStageCount = 0;
	song->coursePlayable = 0;
	memset(&song->mybest, 0, sizeof(STATUS));
	memset(&song->rivalRecord, 0, sizeof(STATUS));
	(song->mybest).minbp = -1;
	song->courseType = -1;
	song->courseIR = 0;
	song->courseReadOnly = 0;
	if (song->grHandle != -1) {
		DeleteGraph(song->grHandle);
	}
	song->grHandle = -1;
	return 1;
}

int LoadBMSMETAFromDB(BMSMETA *meta, sqlite3 *sql) {
	char str[1024];
	sqlite3_stmt *pStmt;
	sqlite3_snprintf(1024, str, "SELECT * FROM tag WHERE hash = \'%q\'", meta->hash.body);
	SQL_prepare(str, sql, &pStmt);
	if(sqlite3_step(pStmt) == 100){
		meta->title = SQL_GetColumn(1, pStmt);
		meta->subtitle = SQL_GetColumn(2, pStmt);
		meta->genre = SQL_GetColumn(3, pStmt);
		meta->artist = SQL_GetColumn(4, pStmt);
		meta->subartist = SQL_GetColumn(5, pStmt);
		meta->tag = SQL_GetColumn(6, pStmt);
		meta->selLevel = sqlite3_column_int(pStmt, 7);
		meta->difficulty = sqlite3_column_int(pStmt, 8);
		meta->keymode = sqlite3_column_int(pStmt, 9);
		meta->exlevel = sqlite3_column_int(pStmt, 10);
		return 1;
	}
	sqlite3_finalize(pStmt);
	return 0;
}

int SetUndefinedDifficulty(sqlite3 *sql) {
	char str[1024];
	sqlite3_stmt *pStmt;
	CSTR folder;
	int mode, difficulty;

	sqlite3_prepare(sql, "SELECT difficulty,folder,mode,path FROM song ORDER BY folder,mode,karinotes", -1, &pStmt, NULL);
	while (sqlite3_step(pStmt) == 100) {
		if (0 <= sqlite3_column_int(pStmt, 0) && sqlite3_column_int(pStmt, 0) <= 5) {
			cstrSprintf(&folder, "%s", sqlite3_column_text(pStmt, 1));
			mode = sqlite3_column_int(pStmt, 2);
			difficulty = sqlite3_column_int(pStmt, 0);
		}
		else {
			CSTR newfolder;
			cstrSprintf(&newfolder, "%s", sqlite3_column_text(pStmt, 1));
			if (folder.isSame(newfolder) && (mode == sqlite3_column_int(pStmt, 2))) {
				difficulty++;
				if (difficulty == 5) difficulty = 4;
				else if (difficulty < 0) difficulty = 2;
				else if (difficulty > 5) difficulty = 5;
			}
			else {
				difficulty = 2;
			}
			SQL_Run(sqlite3_snprintf(1024, str, "UPDATE song SET difficulty = %d WHERE path = \'%q\'", difficulty, SQL_GetColumn(3, pStmt).body), sql);
			folder = SQL_GetColumn(1, pStmt);
			mode = sqlite3_column_int(pStmt, 2);
		}
	}
	sqlite3_finalize(pStmt);
	return 1;
}

int CMP_SongDataByExLevel(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	if (s1->keymode != s2->keymode) {
		return s1->keymode - s2->keymode;
	}

	if(s1->exlevel > 0 && s2->exlevel > 0) return s1->exlevel - s2->exlevel;
	if(s1->exlevel > 0 && s2->exlevel <= 0) return 1;
	if(s1->exlevel <= 0 && s2->exlevel > 0) return -1;
	if(s1->exlevel <= 0 && s2->exlevel <= 0) return s1->level - s2->level;

	return 1;//not neccessary
}

int CMP_SongDataByTitle(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	CSTR c1(s1->title);
	c1.upper();
	CSTR c2(s2->title);
	c2.upper();

	if (c1.isDiffHigh(&c2)) return 1;
	if (c1.isSame(&c2)) {
		if (s1->difficulty == 5 && s2->difficulty != 5) return 1;
		if (s1->difficulty != 5 && s2->difficulty != 5) return s1->level - s2->level;
		if (s1->difficulty == 5 && s2->difficulty == 5) return s1->level - s2->level;
		if (s1->difficulty != 5 && s2->difficulty == 5) return -1;
	}
	if (c1.isDiffLow(&c2)) return -1;

	return 0;
}

int CMP_SongDataByScore(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	return ((s1->mybest.total_notes) ? s1->mybest.stat_exscore * 10000 / s1->mybest.total_notes * 2 : 0)
		 - ((s2->mybest.total_notes) ? s2->mybest.stat_exscore * 10000 / s2->mybest.total_notes * 2 : 0);
}

int CMP_SongDataByRivalScore(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	return (s2->rivalRecord.stat_exscore - s2->mybest.stat_exscore) - (s1->rivalRecord.stat_exscore - s1->mybest.stat_exscore);
}

int CMP_SongDataByDirectory(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	CSTR c1(s1->folder);
	c1.upper();
	CSTR c2(s2->folder);
	c2.upper();

	if (c1.isDiffHigh(&c2)) return 1;
	if (c1.isSame(&c2)) {
		if (s1->keymode == s2->keymode)
			return (s1->level - s2->level) + (s1->difficulty == 5) * 100 - (s2->difficulty == 5) * 100;
		else
			return s1->keymode - s2->keymode;
	}
	if (c1.isDiffLow(&c2)) return -1;

	return 0;
}

int CMP_SongDataByPath(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	CSTR c1(s1->filepath);
	c1.upper();
	CSTR c2(s2->filepath);
	c2.upper();

	if (c1.isDiffHigh(&c2)) return 1;
	if (c1.isDiffLow(&c2)) return -1;

	return 0;
}

int UpdateSongDataTag(SONGDATA *song, sqlite3 *sql){
	char query[1024];

	SQL_Run("BEGIN", sql);
	if (song->keymode >= 5) {
		sqlite3_snprintf(0x400, query, "UPDATE song SET title=\'%q\',subtitle=\'%q\',genre=\'%q\',artist=\'%q\',subartist=\'%q\',tag=\'%q\',level=%d,difficulty=%d,mode=%d,favorite=%d,exlevel=%d WHERE hash = \'%q\'",
			song->title.body, song->subtitle.body, song->genre.body, song->artist.body, song->subartist.body, song->tag.body, song->level, song->difficulty, song->keymode, song->favorite, song->exlevel, song->hash.body);
		SQL_Run(query, sql);

		sqlite3_snprintf(0x400, query, "INSERT INTO tag (hash,title,subtitle,genre,artist,subartist,tag,level,difficulty,mode,exlevel) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,%d,%d)",
			song->hash.body, song->title.body, song->subtitle.body, song->genre.body, song->artist.body, song->subartist.body, song->tag.body, song->level, song->difficulty, song->keymode, song->exlevel);
		if (SQL_Run(query, sql) != 0) {
			sqlite3_snprintf(0x400, query, "UPDATE tag SET title=\'%q\',subtitle=\'%q\',genre=\'%q\',artist=\'%q\',subartist=\'%q\',tag=\'%q\',level=%d,difficulty=%d,mode=%d WHERE hash = \'%q\'",
				song->title.body, song->subtitle.body, song->genre.body, song->artist.body, song->subartist.body, song->tag.body, song->level, song->filepath.body, song->keymode, song->hash.body);
			SQL_Run(query, sql);
		}
	}
	else {
		sqlite3_snprintf(0x400, query, "UPDATE folder SET title=\'%q\',subtitle=\'%q\',category=\'%q\',info_a=\'%q\',info_b=\'%q\',command=\'%q\',max=%d WHERE path = \'%q\'",
			song->title.body, song->subtitle.body, song->genre.body, song->artist.body, song->subartist.body, song->tag.body, song->level, song->filepath.body);
		SQL_Run(query, sql);
	}
	SQL_Run("COMMIT", sql);
	return 1;
}

int EditTag(SONGDATA *song, sqlite3 *sql) {
	sqlite3_stmt *stmt;
	char query[1024];

	int wtime = GetFileUnixtime(song->filepath);
	int ntime = GetNowUnixtime();
	int temp;
	if (song->folderType == 0 || song->folderType == 5) {

		sqlite3_snprintf(1024, query, "SELECT adddate FROM song WHERE path = \'%q\'", song->filepath.body);
		SQL_prepare(query, sql, &stmt);
		temp = 0;
		if (sqlite3_step(stmt) == 100) {
			temp = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
		BMSMETA meta;
		ParseBMSMETA(&meta, song->filepath, 0);

		sqlite3_snprintf(1024, query, "DELETE FROM song WHERE path=\'%q\'", song->filepath.body);
		SQL_Run(query, sql);
		sqlite3_snprintf(1024, query, "DELETE FROM tag WHERE hash=\'%q\'", song->hash.body);
		SQL_Run(query, sql);

		sqlite3_snprintf(1024, query, "INSERT INTO song (hash,title,subtitle,genre,artist,subartist,level,date,path,folder,stagefile,banner,backbmp,parent,maxbpm,minbpm,random,longnote,judge,mode,bga,difficulty,favorite,type,txt,karinotes,adddate,exlevel) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,%d)",
			meta.hash.body, meta.title.body, meta.subtitle.body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.selLevel, wtime, song->filepath.body, AssignCRC32(meta.folderpath).body, meta.stagefilepath.body,meta.bannerpath.body,meta.backBMPpath.body, AssignCRC32(meta.parentfolderpath).body, meta.maxbpm,meta.minbpm, meta.random,meta.longnote,meta.judge,meta.keymode,meta.bga,meta.difficulty,meta.hasTxt,meta.notecount,temp,meta.exlevel);
		SQL_Run(query, sql);

		if (meta.difficulty <= 0 || meta.difficulty > 5) {
			SetUndefinedDifficulty(sql);
		}

		sqlite3_snprintf(1024, query, "SELECT difficulty FROM song WHERE path = \'%q\'", song->filepath.body);
		SQL_prepare(query, sql, &stmt);
		temp = 0;
		if (sqlite3_step(stmt) == 100) {
			temp = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);

		song->title = meta.title;
		song->artist = meta.artist;
		song->genre = meta.genre;
		song->subtitle = meta.subtitle;
		song->subartist = meta.subartist;
		song->keymode = meta.keymode;
		song->tag = meta.tag;
		song->level = meta.selLevel;
		song->favorite = 0;
		song->difficulty = temp;
	}
	else if (IsLR2Folder(song->filepath)) {
		sqlite3_snprintf(1024, query, "DELETE FROM folder WHERE path=\'%q\'", song->filepath.body);
		SQL_Run(query, sql);
		BMSMETA meta;
		ParseBMSMETA(&meta, song->filepath, 0);
		sqlite3_snprintf(1024, query, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , adddate ) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , %d) ",
			song->filepath.body, meta.title.body, song->folder.body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.tag.body, meta.selLevel, wtime, song->folderType, ntime);
		SQL_Run(query, sql);

		song->title = meta.title;
		song->artist = meta.artist;
		song->genre = meta.genre;
		song->subtitle = meta.subtitle;
		song->subartist = meta.subartist;
		song->keymode = meta.keymode;
		song->tag = meta.tag;
		song->level = meta.selLevel;
	}
	else if (song->folderType == 1 || song->folderType == 2) {
		sqlite3_snprintf(1024, query, "UPDATE folder SET title=\'%q\',subtitle=\'\',category=\'\',info_a=\'\',info_b=\'\',command=\'\',max=0 WHERE path = \'%q\'", song->filepath.getFilename().body, song->filepath.body);
		SQL_Run(query, sql);

		song->title = song->filepath.getFilename();
		song->artist.fillzero();
		song->genre.fillzero();
		song->subtitle.fillzero();
		song->subartist.fillzero();
		song->tag.fillzero();
		song->level = 0;
	}
	return 1;
}

// TODO : I need to understand this and rename variables
int LoadFolderDataFromDB(CSTR query, SONGDATA *song, sqlite3 *sql, int difficulty, int key, int /*sort*/, int maxCount, CONFIG_SELECT *cfg_select, char flag) {
	sqlite3_stmt *stmt;
	SONGDATA* slist;
	SONGDATA sd;
	int keys = 0;
	size_t nowsize, maxsize;
	int slistCount;
	CSTR nowFolder, newFolder, workingFolder;
	int nowMode, nowDifficulty;

	//key 0:ALL 1:SINGLE 2:7keys 3:5keys 4:DOUBLE 5:14keys 6:10keys 7:9buttons

	if ((cfg_select->ignorekeyall == 1) && (key == 0)) key = 1;
	if ((cfg_select->ignorekeysingle == 1) && (key == 1)) key = 2;
	if ((cfg_select->ignorekeydouble == 1) && (key == 4)) key = 5;
	if ((cfg_select->ignoredp == 1) && (key < 7)) key = 7; //TOFIX? : && key > 3
	if ((cfg_select->ignorepms == 1) && (key == 7)) key = 0;
	if ((cfg_select->ignorekeyall == 1) && (key == 0)) key = 1;
	if ((cfg_select->ignoredifficultyall == 1) && (difficulty == 0)) difficulty = 1;

	if (query.findStrPos("__NEWSONG__") > -1) {
		cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE adddate > %d", GetNowUnixtime() - cfg_select->titleflash * 3600);
	}

	if (query.left(6).isDiff("SELECT")) return -1;

	if (maxCount == 1 && query.findStrPos("random()") == -1) return 0;

	if (query.findStrPos("ORDER BY") != -1) {
		difficulty = 0;
		key = 0;
	}
	if (flag) {
		difficulty = 0;
	}

	if (query.findStrPos("ORDER BY") == -1) {
		switch (key) {
			case 2:
				keys = 7;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC");
				break;
			case 3:
				keys = 5;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC");
				break;
			case 5:
				keys = 14;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC");
				break;
			case 6:
				keys = 10;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC");
				break;
			case 7:
				keys = 9;
				query.add(" ORDER BY folder ASC,difficulty ASC");
				break;
		}
	}

	int keymode_A = 0;
	slist = (SONGDATA*)malloc(sizeof(SONGDATA) * 1000);
	memset(slist, 0, sizeof(SONGDATA) * 1000); // FIXME: bad memset
	slistCount = 0;
	SQL_prepare(query, sql, &stmt);

	int difficultyCount = 0;
	int folderSongCount = 0;
	int keymode_B = 0;

	nowsize = sizeof(SONGDATA) * 1000;
	maxsize = 2000;

	while (sqlite3_step(stmt) == 100) {
		if (maxCount != 0 && slistCount == maxCount) break;

		int mode = sqlite3_column_int(stmt, 18);
		if (cfg_select->ignoredp == 1 && mode >= 10) continue;
		if (cfg_select->ignorepms == 1 && mode == 9) continue;

		sd.keymode = mode;

		if( (maxsize - 1001) == slistCount){
			int newsize = nowsize + sizeof(SONGDATA) * 1000;
			slist = (SONGDATA*)realloc(slist, newsize);
			assert(slist != nullptr);
			if (maxsize - 1000 < maxsize) {
				memset((void*)((uintptr_t)slist + nowsize), 0, sizeof(SONGDATA) * 1000);
			}
			maxsize += 1000;
			keymode_A = keymode_B;
			nowsize = newsize;
		}

		sd.difficulty = sqlite3_column_int(stmt, 15);
		sd.favorite = sqlite3_column_int(stmt, 24);
		if (cfg_select->ignore5key == 1) {
			cstrSprintf(&sd.folder, "%s", sqlite3_column_text(stmt, 9));
			if (nowFolder.isDiff(&sd.folder)) {
				folderSongCount = 0;
			}
			else {
				if ((keymode_A != 5 || sd.keymode != 7) && (keymode_A != 10 || sd.keymode != 14)) {
					if (keymode_A != sd.keymode) {
						folderSongCount = 0;
					}
				}
				else {
					slistCount -= folderSongCount;
					folderSongCount = 0;
				}
			}
		}

		if (slistCount < 0)	slistCount = 0;

		if (sd.favorite == 2) continue;

		auto keymode_allowed_for_playstyle = [](int keymode, int playstyle, int playstyle_keys) {
			if (playstyle == 0 /*all*/) {
				return true;
			}
			switch(keymode) {
			case 5: return keymode == playstyle_keys || playstyle == 1 /*single*/;
			case 7: return keymode == playstyle_keys || playstyle == 1 /*single*/;
			case 9: return keymode == playstyle_keys;
			case 10: return keymode == playstyle_keys || playstyle == 4 /*double*/;
			case 14: return keymode == playstyle_keys || playstyle == 4 /*double*/;
			default: return false; // unreachable?
			}
		};
		if (!keymode_allowed_for_playstyle(sd.keymode, key, keys)) continue;

		sd.difficulty = sqlite3_column_int(stmt, 15);
		cstrSprintf(&newFolder, "%s", sqlite3_column_text(stmt, 9));
		if (sd.keymode > 0) {
			sd.mybest.clear = sqlite3_column_int(stmt, 30);
			sd.mybest.stat_pgreat = sqlite3_column_int(stmt, 31);
			sd.mybest.stat_great = sqlite3_column_int(stmt, 32);
			sd.mybest.stat_good = sqlite3_column_int(stmt, 33);
			sd.mybest.stat_bad = sqlite3_column_int(stmt, 34);
			sd.mybest.stat_poor = sqlite3_column_int(stmt, 35);
			sd.mybest.total_notes = sqlite3_column_int(stmt, 36);
			sd.mybest.stat_maxcombo = sqlite3_column_int(stmt, 37);
			sd.mybest.minbp = sqlite3_column_int(stmt, 38);
			sd.mybest.playcount = sqlite3_column_int(stmt, 39);
			sd.mybest.clearcount = sqlite3_column_int(stmt, 40);
			sd.mybest.failcount = sqlite3_column_int(stmt, 41);
			sd.mybest.rank = sqlite3_column_int(stmt, 42);
			sd.mybest.rate = sqlite3_column_int(stmt, 43);
			sd.mybest.clear_db = sqlite3_column_int(stmt, 44);
			sd.mybest.op_history = sqlite3_column_int(stmt, 45);
			sd.mybest.clear_sd = sqlite3_column_int(stmt, 48);
			sd.mybest.clear_ex = sqlite3_column_int(stmt, 49);
			sd.mybest.op_best = sqlite3_column_int(stmt, 50);
			sd.mybest.rseed = sqlite3_column_int(stmt, 51);
			sd.mybest.complete = sqlite3_column_int(stmt, 52);
		}
		if (mode == 0) {
			workingFolder = newFolder;
			COPY_SONGDATA(&slist[slistCount], &sd);
			nowMode = mode;
			difficultyCount = 1;

			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		else if (newFolder.isDiff(workingFolder)) {
			workingFolder = newFolder;
			nowMode = mode;
			nowDifficulty = sd.difficulty;
			difficultyCount = 1;
			COPY_SONGDATA(&slist[slistCount], &sd);

			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		else if (mode != nowMode) {
			nowMode = mode;
			nowDifficulty = sd.difficulty;
			difficultyCount = 1;
			COPY_SONGDATA(&slist[slistCount], &sd);

			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		else if (nowDifficulty < sd.difficulty && sd.difficulty <= difficulty && difficulty != 0) {
			folderSongCount -= difficultyCount;
			slistCount -= difficultyCount;
			if (folderSongCount < 1) folderSongCount = 0;
			nowDifficulty = sd.difficulty;
			difficultyCount = 1;
			COPY_SONGDATA(&slist[slistCount], &sd);

			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		else if (nowDifficulty == sd.difficulty || difficulty == 0) {
			difficultyCount++;
			COPY_SONGDATA(&slist[slistCount], &sd);

			slistCount++;
			folderSongCount++;
			nowFolder = sd.folder;
			keymode_B = sd.keymode;
			keymode_A = sd.keymode;
		}
		InitSongData(&sd);
	}
	sqlite3_finalize(stmt);
	if (slistCount) { 
		song->mybest.clear = 5;
		song->mybest.clear_db = 5;
		for (int i = 0; i < slistCount; i++) { 
			song->mybest.playcount += slist[i].mybest.playcount;
			song->mybest.clearcount += slist[i].mybest.clearcount;
			song->mybest.failcount += slist[i].mybest.failcount;
			if (slist[i].mybest.clear != song->mybest.clear && slist[i].mybest.clear <= song->mybest.clear)
				song->mybest.clear = slist[i].mybest.clear;
			if (slist[i].mybest.clear_db != song->mybest.clear_db && slist[i].mybest.clear_db <= song->mybest.clear_db)
				song->mybest.clear_db = slist[i].mybest.clear_db;
		}
	}
	free(slist);
	return 1;
}

int UninstallSong(CSTR path, sqlite3 *sql) {
	char query[2048];

	CSTR dir(path.getDirectory());
	CSTR crc(AssignCRC32(dir));

	sqlite3_snprintf(2048, query, "DELETE FROM song WHERE folder=\'%q\'", crc.body);
	SQL_Run(query, sql);

	sqlite3_snprintf(2048, query, "DELETE FROM folder WHERE path=\'%q\'", dir.body);
	SQL_Run(query, sql);

	dir.nullAtPos(dir.length()-1);

#ifdef _WIN32
	SHFILEOPSTRUCTA sh{};
	sh.wFunc = FO_DELETE; //3
	sh.pFrom = dir;
	sh.pTo = NULL;
	sh.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT; //0x414

	return (SHFileOperationA(&sh) == 0);
#else // TODO: check if error handling matches and then consolidate the two.
	// std::error_code ec; // not tested. dear game, pls never delete my stuff
	// std::filesystem::remove_all(dir.body, ec);
	return {};
#endif // _WIN32
}

int Rename(CSTR path, sqlite3 *sql) {
	char query[2048];

	CSTR newpath(path);
	newpath.add("_");

	sqlite3_snprintf(2048, query, "DELETE FROM song WHERE path=\'%q\'", path.body);
	SQL_Run(query, sql);

#ifdef _WIN32
	SHFILEOPSTRUCTA sh{};
	sh.wFunc = FO_RENAME; //4
	sh.pFrom = path;
	sh.pTo = newpath;
	sh.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT; //0x414

	return (SHFileOperationA(&sh) == 0);
#else // TODO: check if error handling matches and then consolidate the two.
	std::error_code ec;
	std::filesystem::rename(path.body, newpath.body, ec);
	return {};
#endif // _WIN32
}

int GetSongData(CSTR songMD5, SONGDATA *song, sqlite3 *sql, SONGSELECT *ss) {
	CSTR query;
	CSTR replayPath;
	CSTR besthash;
	char buf[1024];
	sqlite3_stmt *stmt;
	int ret = 0;

	InitSongData(song);
	query = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE song.hash = \'%s\'", songMD5.body);
	SQL_prepare(query, sql, &stmt);
	if (sqlite3_step(stmt) == 100) {
		song->maxBPM = sqlite3_column_int(stmt, 16);
		song->minBPM = sqlite3_column_int(stmt, 17);
		song->hash = SQL_GetColumn(0, stmt);
		if (song->hash.isSame("9cbea4427d7e8caff0d28f9d7600cdec"))
			song->title = AutomationFactory();
		else
			song->title = SQL_GetColumn(1, stmt);
		song->subtitle = SQL_GetColumn(2, stmt);
		song->genre = SQL_GetColumn(3, stmt);
		song->filepath = SQL_GetColumn(7, stmt);
		song->folderType = sqlite3_column_int(stmt, 8);
		song->level = sqlite3_column_int(stmt, 14);
		song->artist = SQL_GetColumn(4, stmt);
		song->subartist = SQL_GetColumn(5, stmt);
		song->keymode = sqlite3_column_int(stmt, 18);
		song->judge = sqlite3_column_int(stmt, 19);
		song->longnote = sqlite3_column_int(stmt, 20);
		song->bga = sqlite3_column_int(stmt, 21);
		song->random = sqlite3_column_int(stmt, 22);
		song->folder = SQL_GetColumn(9, stmt);
		song->txt = sqlite3_column_int(stmt, 25);
		song->favorite = sqlite3_column_int(stmt, 24);
		song->adddate = sqlite3_column_int(stmt, 27);
		
		song->stagefile = SQL_GetColumn(10, stmt);
		if (song->stagefile.isDiff("(null)") && song->stagefile.length() > 4)
			song->isStagefile = 1;
		song->banner = SQL_GetColumn(11, stmt);
		if (song->banner.isDiff("(null)") && song->banner.length() > 4)
			song->isBanner = 1;
		song->backBMP = SQL_GetColumn(12, stmt);
		if (song->backBMP.isDiff("(null)") && song->backBMP.length() > 4)
			song->isBackBMP = 1;

		if(song->subtitle.isDiff("(null)"))
			cstrSprintf(&song->fulltitle,"%s %s", song->title.body, song->subtitle.body);
		else
			song->fulltitle = song->title;

		song->tag = SQL_GetColumn(6, stmt);
		song->difficulty = sqlite3_column_int(stmt, 15);
		cstrSprintf(&replayPath, fs::make_preferred("LR2files/Replay/%s/%s.lr2rep").data(), ss->playerID.body, song->hash.body);
		song->replayExist = IsFileExist(replayPath);
		if (song->keymode > 0) {
			song->mybest.clear = sqlite3_column_int(stmt, 30);
			song->mybest.stat_pgreat = sqlite3_column_int(stmt, 31);
			song->mybest.stat_great = sqlite3_column_int(stmt, 32);
			song->mybest.stat_good = sqlite3_column_int(stmt, 33);
			song->mybest.stat_bad = sqlite3_column_int(stmt, 34);
			song->mybest.stat_poor = sqlite3_column_int(stmt, 35);
			song->mybest.total_notes = sqlite3_column_int(stmt, 36);
			song->mybest.stat_maxcombo = sqlite3_column_int(stmt, 37);
			song->mybest.minbp = sqlite3_column_int(stmt, 38);
			song->mybest.playcount = sqlite3_column_int(stmt, 39);
			song->mybest.clearcount = sqlite3_column_int(stmt, 40);
			song->mybest.failcount = sqlite3_column_int(stmt, 41);
			song->mybest.rank = sqlite3_column_int(stmt, 42);
			song->mybest.rate = sqlite3_column_int(stmt, 43);
			if (song->mybest.rank > 8) song->mybest.rank = 8;
			if (song->mybest.rank == 0 && (song->mybest.stat_pgreat + song->mybest.stat_great > 0)) song->mybest.rank = 1;
			song->mybest.clear_db = sqlite3_column_int(stmt, 44);
			song->mybest.op_history = sqlite3_column_int(stmt, 45);
			song->mybest.clear_sd = sqlite3_column_int(stmt, 48);
			song->mybest.clear_ex = sqlite3_column_int(stmt, 49);
			song->mybest.op_best = sqlite3_column_int(stmt, 50);
			song->mybest.rseed = sqlite3_column_int(stmt, 51);
			song->mybest.complete = sqlite3_column_int(stmt, 52);

			besthash = SQL_GetColumn(46, stmt);
			if (isSameScoreHash(&song->mybest, &ss->playerPassMD5, &song->hash, &besthash)) {
				song->mybest.stat_exscore = song->mybest.stat_great + song->mybest.stat_pgreat * 2;
				if (song->mybest.total_notes < 1) 
					song->mybest.total_notes = sqlite3_column_int(stmt, 26);

				if (song->mybest.total_notes > 0)
					song->mybest.stat_score = (song->mybest.stat_good + song->mybest.stat_great * 2 + song->mybest.stat_pgreat * 4) * 50000 / song->mybest.total_notes;

				if (song->mybest.minbp == 0 && song->mybest.clear != 5)
					song->mybest.minbp = -1; 
			}
			else {
				memset(&song->mybest, 0, sizeof(STATUS));
				song->mybest.minbp = -1;
				song->mybest.total_notes = sqlite3_column_int(stmt, 26);
			}
		}
		ret = 1;
	}
	else {
		ErrorLogFmtAdd("曲データの取得に失敗 %s\n", songMD5.body);
	}
	sqlite3_finalize(stmt);
	return ret;
}

int WriteCourse(sqlite3 *sql, COURSESELECT course, SONGDATA *song, CSTR passmd5, int connection, int /*gauge*/) {

	char t[1024];

	if (song->courseReadOnly) return 1;

	CSTR hash;
	CSTR query;

	if (course.id < 0) {
		if (course.type == 1) 
			cstrSprintf(&hash, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d00000000000000%s", connection, connection, connection, connection, connection, 0, 0, 0, 0, 0, 1, 0, 0, 0, passmd5.left(4).body);
		else 
			cstrSprintf(&hash, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d00000000000000%s", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, course.type, 0, 0, 0, passmd5.left(4).body);

		for (int i = 0; i < course.count; i++) hash.add(course.data[i].hash);

		if (course.type == 0) 
			query = sqlite3_snprintf(1024, t, "INSERT INTO expert (title,hash,line,ir) VALUES (\'%q\' , \'%q\', %d,0)", course.name.body, hash.body, course.data[0].keymode);
		else if (course.type == 1) 
			query = sqlite3_snprintf(1024, t, "INSERT INTO nonstop (title,hash,line,ir) VALUES (\'%q\' , \'%q\', %d,0)", course.name.body, hash.body, course.data[0].keymode);
		else if (course.type == 2) 
			query = sqlite3_snprintf(1024, t, "INSERT INTO grade (title,hash,line,ir) VALUES (\'%q\' , \'%q\', %d,0)", course.name.body, hash.body, course.data[0].keymode);
	}
	else {
		cstrSprintf(&hash, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d00000000000000%s", song->courseKeys[0], song->courseKeys[1], song->courseKeys[2], song->courseKeys[3], song->courseKeys[4], song->courseKeys[5],
			song->courseKeys[6], song->courseKeys[7], song->courseKeys[8], song->courseKeys[9], song->courseType, 0, 0, 0, passmd5.left(4).body);

		for (int i = 0; i < song->courseStageCount; i++) hash.add(song->courseHash[i]);

		if (course.type == 0) 
			query = sqlite3_snprintf(1024, t, "UPDATE expert SET title=\'%q\',hash=\'%q\',ir=%d WHERE id = %d", song->title.body, hash.body, song->courseIR, course.id);
		else if (course.type == 1) 
			query = sqlite3_snprintf(1024, t, "UPDATE nonstop SET title=\'%q\',hash=\'%q\',ir=%d WHERE id = %d", song->title.body, hash.body, song->courseIR, course.id);
		else if (course.type == 2) 
			query = sqlite3_snprintf(1024, t, "UPDATE grade SET title=\'%q\',hash=\'%q\',ir=%d WHERE id = %d", song->title.body, hash.body, song->courseIR, course.id);
	}

	SQL_Run(query, sql);
	for (int i = 0; i < 10; i++) InitSongData(&course.data[i]);

	return 1;
}

int ChangeCourseTitle(sqlite3* sql, CSTR title, int id, int coursetype) {

	char str[1024];
	CSTR query;

	if (coursetype == 0) {
		sqlite3_snprintf(0x400, str, "UPDATE expert SET title=\'%q\' WHERE id = %d",title.body, id);
	}
	else if (coursetype == 1) {
		sqlite3_snprintf(0x400, str, "UPDATE nonstop SET title=\'%q\' WHERE id = %d", title.body, id);
	}
	else if (coursetype == 2) {
		sqlite3_snprintf(0x400, str, "UPDATE grade SET title=\'%q\' WHERE id = %d", title.body, id);
	}

	query = str;
	SQL_Run(query, sql);
	return 1;
}

int DeleteCourse(sqlite3* sql, int id, int courseType) {

	char str[1024];
	CSTR query;

	if (courseType == 0) {
		sqlite3_snprintf(0x400, str,"DELETE FROM expert WHERE id = %d" , id);
	}
	else if (courseType == 1) {
		sqlite3_snprintf(0x400, str, "DELETE FROM nonstop WHERE id = %d", id);
	}
	else if (courseType == 2){
		sqlite3_snprintf(0x400, str, "DELETE FROM grade WHERE id = %d", id);
	}

	query = str;
	SQL_Run(query, sql);
	return 1;
}

CSTR GetRandomSongHash(CSTR ioDefault, CSTR folder, int levelStd, int mode, sqlite3 *sql, int levelMax, int levelMin) {

	sqlite3_stmt *pStmt;

	if (levelStd < 1) return ioDefault;

	CSTR query;

	cstrSprintf(&query, "SELECT hash FROM song WHERE folder=\'%s\' AND mode=%d AND level<=%d AND level>=%d ORDER BY abs(level - %d)", folder.body, mode, levelMax, levelMin, levelStd);
	SQL_prepare(query, sql, &pStmt);
	if (sqlite3_step(pStmt) == 100) {
		ioDefault = SQL_GetColumn(0, pStmt);
	}
	sqlite3_finalize(pStmt);
	return ioDefault;
}

int GetSongDataFromPath(CSTR filepath, SONGDATA *song, sqlite3 *sql, SONGSELECT *ss) {
	BMSMETA meta;
	CSTR hash;

	InitSongData(song);
	if (!filepath.canOpenFile()) {
		ErrorLogFmtAdd("GetSongDataFromPath() bmsファイルが見つかりません\n");
		return -1;
	}

	makeFileHash(filepath, hash);
	if (GetSongData(hash,song,sql,ss) == 1) {
		ErrorLogFmtAdd("GetSongDataFromPath() データベースに登録済みなのでそちらを利用します\n");
		return 1;
	}
	
	ParseBMSMETA(&meta, filepath, 0);
	song->artist = meta.artist;
	song->backBMP = meta.backBMPpath;
	song->stagefile = meta.stagefilepath;
	song->banner = meta.bannerpath;
	song->bga = meta.bga;
	song->difficulty = meta.difficulty;
	song->exlevel = 0;
	cstrSprintf(&song->fulltitle, "%s %s", meta.title.body, meta.subartist.body);
	song->folder = meta.folderpath;
	song->genre = meta.genre;
	song->hash = meta.hash;
	song->longnote = meta.longnote;
	song->level = meta.selLevel;
	song->keymode = meta.keymode;
	song->maxBPM = meta.maxbpm;
	song->minBPM = meta.minbpm;
	song->filepath.assign(&filepath);
	song->random = meta.random;
	song->judge = meta.judge;
	song->subartist = meta.subartist;
	song->subtitle = meta.subtitle;
	song->folderType = 0;
	song->title = meta.title;
	song->folderType = 0;
	ErrorLogFmtAdd("GetSongDataFromPath() 曲をロードしました\n");
	return 2;
}

int ChangeCourseID(sqlite3 *sql, int newID, int oldID, int type) {

	char str[1024];

	if (newID <= 0 || oldID <= 0) return 0;

	if (type == 0) {
		sqlite3_snprintf(0x400, str, "UPDATE expert SET id = -1 WHERE id = %d", newID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE expert SET id = %d WHERE id = %d", newID, oldID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE expert SET id = %d WHERE id = -1", oldID);
		SQL_Run(str, sql);
	}
	else if (type == 1) {
		sqlite3_snprintf(0x400, str, "UPDATE nonstop SET id = -1 WHERE id = %d", newID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE nonstop SET id = %d WHERE id = %d", newID, oldID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE nonstop SET id = %d WHERE id = -1", oldID);
		SQL_Run(str, sql);
	}
	else if (type == 2) {
		sqlite3_snprintf(0x400, str, "UPDATE grade SET id = -1 WHERE id = %d", newID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE grade SET id = %d WHERE id = %d", newID, oldID);
		SQL_Run(str, sql);
		sqlite3_snprintf(0x400, str, "UPDATE grade SET id = %d WHERE id = -1", oldID);
		SQL_Run(str, sql);
	}
	return 1;
}


// Returns the stored timestamp for a song/folder path, or -1 when no row exists.
// SearchSongsFromPath uses this as the old "missing row" signal before insert/update.
int GetRegisteredDateFromPath(const char *table, sqlite3 *sql, CSTR path) {
	sqlite3_stmt *pStmt = NULL;
	char str[1024];
	int date = -1;

	sqlite3_snprintf(1024, str, "SELECT date FROM %s WHERE path = \'%q\'", table, path.body);
	if (SQL_prepare(str, sql, &pStmt) != 0) {
		return -1;
	}
	if (sqlite3_step(pStmt) == 100) {
		date = sqlite3_column_int(pStmt, 0);
	}
	sqlite3_finalize(pStmt);
	return date;
}

// Checks whether an existing song row can be kept without reparsing the BMS file.
// The timestamp check keeps the fast path for unchanged charts, while the CRC checks
// repair legacy rows created with a stale parent/folder relationship.
int IsRegisteredSongCurrent(sqlite3 *sql, CSTR path, int filetime) {
	sqlite3_stmt *pStmt = NULL;
	char str[1024];
	const char *folder;
	const char *parent;
	CSTR expectedFolder;
	CSTR expectedParent;
	int ret = 0;

	// folder is the physical chart directory; parent is where the chart appears in select.
	expectedFolder = AssignCRC32(path.getDirectory());
	expectedParent = AssignCRC32(path.getParentDirectory());

	sqlite3_snprintf(1024, str, "SELECT date,folder,parent FROM song WHERE path = \'%q\'", path.body);
	if (SQL_prepare(str, sql, &pStmt) != 0) {
		return 0;
	}
	if (sqlite3_step(pStmt) == 100) {
		folder = (const char *)sqlite3_column_text(pStmt, 1);
		parent = (const char *)sqlite3_column_text(pStmt, 2);
		if (sqlite3_column_int(pStmt, 0) >= filetime && folder && parent && expectedFolder.isSame(folder) && expectedParent.isSame(parent)) {
			ret = 1;
		}
	}
	sqlite3_finalize(pStmt);
	return ret;
}

int SearchSongsFromPath(CSTR root, sqlite3 *sql, CSTR path) {
#ifdef _WIN32
	HANDLE hFindFile;
	_WIN32_FIND_DATAA findFileData;
	int now,filetime,oldtime,ret;
	char str[2048];
	int count = 0;
	

	ErrorLogFmtAdd("曲の検索を行います。パス%s\n", root.body);
	ErrorLogTabAdd();
	if (root.right(1).isDiff("\\")) root.add("\\");

	CSTR searchPath;
	BMSMETA meta; 
	searchPath = root;
	searchPath.add("*.*");
	hFindFile = FindFirstFileA(searchPath, &findFileData);
	if (hFindFile == INVALID_HANDLE_VALUE) {
		ErrorLogTabSub();
		return count;
	}
	now = GetNowUnixtime();
	while (1) {
		if ( ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) || !(strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName)) ) { // not directory
			if (((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) && IsBmsFile(findFileData.cFileName)) {
				searchPath = root;
				searchPath.add(findFileData.cFileName);
				filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
				// Skip expensive metadata parsing only when date and hierarchy CRCs are valid.
				if (IsRegisteredSongCurrent(sql, searchPath, filetime) == 0) {
					ErrorLogFmtAdd("曲を発見しました。　パス:%s\n", searchPath.body);
					SQL_Run(sqlite3_snprintf(2048, str, "DELETE FROM song WHERE path = \'%q\'", searchPath.body), sql);
					ParseBMSMETA(&meta, searchPath, 1);
					LoadBMSMETAFromDB(&meta, sql);
					sqlite3_snprintf(2048, str, "INSERT INTO song (hash,title,subtitle,genre,artist,subartist,level,date,path,folder,stagefile,banner,backbmp,parent,maxbpm,minbpm,random,longnote,judge,mode,bga,difficulty,favorite,type,txt,karinotes,adddate,exlevel) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,%d)",
						meta.hash.body, meta.title.body, meta.subtitle.body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.selLevel, filetime, searchPath.body, AssignCRC32(meta.folderpath).body, meta.stagefilepath.body, meta.bannerpath.body, meta.backBMPpath.body, AssignCRC32(meta.parentfolderpath).body, meta.maxbpm, meta.minbpm, meta.random, meta.longnote, meta.judge, meta.keymode, meta.bga, meta.difficulty, meta.hasTxt, meta.notecount, now, meta.exlevel);
					SQL_Run(str, sql);
				}
				count++;
			}
			else if (((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) && IsLR2Folder(findFileData.cFileName)){
				searchPath = root;
				searchPath.add(findFileData.cFileName);
				filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
				oldtime = GetRegisteredDateFromPath("folder", sql, searchPath);
				if (oldtime < filetime) {
					ErrorLogFmtAdd("カスタムフォルダを発見しました。　パス:%s\n", searchPath.body);
					if (oldtime >= 0) {
						SQL_Run(sqlite3_snprintf(2048, str, "DELETE FROM folder WHERE path = \'%q\'", searchPath.body), sql);
					}
					ParseBMSMETA(&meta, searchPath, 1);
					sqlite3_snprintf(2048, str, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , 2 , \'%q\' , %d) ",
						searchPath.body, meta.title.body, AssignCRC32(path).body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.tag.body, meta.selLevel, filetime, meta.bannerpath.body, now);
					SQL_Run(str, sql);
				}
				count++;
			}

		}
		else {
			searchPath = root;
			searchPath.add(findFileData.cFileName).add("\\");
			ErrorLogFmtAdd("フォルダを発見しました。　パス:%s\n", searchPath.body);
			
			CSTR folderinfo(searchPath);
			folderinfo.add("folderinfo.txt");
			filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
			oldtime = GetRegisteredDateFromPath("folder", sql, searchPath);
			// The folder row timestamp only describes this folder entry.
			// Child charts may still be missing, changed, or require parent/folder repair.
			if (IsFileExist(folderinfo)) {
				ret = 0;
				if (oldtime < filetime) {
					ErrorLogAdd("ついでにフォルダインフォを発見しました\n");
					ParseBMSMETA(&meta, folderinfo, 0);
					if (meta.judge != 2) meta.judge = 1;

					if (oldtime >= 0) {
						SQL_Run(sqlite3_snprintf(2048, str, "DELETE FROM folder WHERE path = \'%q\'", searchPath.body), sql);
					}
					sqlite3_snprintf(2048, str, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , \'%q\' , %d) ",
						searchPath.body, meta.title.body, AssignCRC32(path).body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.tag.body, meta.selLevel, filetime, meta.judge, meta.bannerpath.body, now);
					ret = SQL_Run(str, sql);
				}
				if (ret == 0) {
					ErrorLogAdd("再帰検索を行います。\n");
					CSTR subPath(root);
					subPath.add(findFileData.cFileName).add("\\");
					count += SearchSongsFromPath(searchPath, sql, subPath);
				}
			}
			else {
				ret = 0;
				if (oldtime < filetime) {
					if (oldtime >= 0) {
						sqlite3_snprintf(2048, str, "UPDATE folder SET title = \'%q\' , parent = \'%q\' , date = %d , type = 1 , adddate = %d WHERE path = \'%q\'", findFileData.cFileName, AssignCRC32(path).body, filetime, now, searchPath.body);
					}
					else {
						sqlite3_snprintf(2048, str, "INSERT INTO folder (path , title , parent , date , type  , adddate) VALUES(\'%q\',\'%q\',\'%q\',%d , 1 , %d )", searchPath.body, findFileData.cFileName, AssignCRC32(path).body, filetime, now);
					}
					ret = SQL_Run(str, sql);
				}
				if (ret == 0) {
					ErrorLogAdd("再帰検索を行います。\n");
					CSTR subPath(root);
					subPath.add(findFileData.cFileName).add("\\");
					count += SearchSongsFromPath(searchPath, sql, subPath);
				}
			}
		}

		if (FindNextFileA(hFindFile, &findFileData) == 0) {
			FindClose(hFindFile);
			ErrorLogTabSub();
			return count;
		}
	}
#else
	// FIXME(linux): stub
	return {};
#endif // _WIN32
}

// TODO:arrange duplicated code
int ReloadSongsByQuery(CSTR query, sqlite3 *sql, CONFIG_JUKEBOX *jb) {

	sqlite3_stmt *pStmt;
	char sBuf[1024];
	int cAlready = 0, cNot = 0, cChange = 0;
	// This query often reads song/folder, and the branches below can delete or insert
	// into those same tables. Snapshot first, then finalize the SELECT before mutating.
	std::vector<CSTR> pathList;
	std::vector<int> timeList;

	GetTimeWrap();
	SQL_prepare(query, sql, &pStmt);
	const int now = GetNowUnixtime();

	while (sqlite3_step(pStmt) == 100) {
		pathList.push_back(SQL_GetColumn(0, pStmt));
		timeList.push_back(sqlite3_column_int(pStmt, 1));
	}
	sqlite3_finalize(pStmt);

	for (size_t i = 0; i < pathList.size(); i++) {
		const int time = timeList[i];
		CSTR str = pathList[i];
		if (!str.left(8).isSame("LR2files")) {
			const bool is_bms_file = IsBmsFile(str);
			const bool is_lr2folder = IsLR2Folder(str);

			int is_path_in_jukebox = 0;
			for (int i = 0; i < jb->numOfPath; i++) {
				if (str.left(jb->path[i].length()).isSame(&jb->path[i])) {
					is_path_in_jukebox = 1;
				}
			}

			if (jb->numOfPath == 0 || is_path_in_jukebox){
				int newTime;
				int chg = IsFileChanged(time, str, &newTime);
				// IsFileChanged only compares timestamps; force reinsert when hierarchy CRCs are stale.
				if (chg == 0 && is_bms_file && IsRegisteredSongCurrent(sql, str, newTime) == 0) {
					chg = 2;
				}
				if (chg == 0) {
					cAlready++;
				}
				else if (chg == 1) {
					cNot++;
					if (is_bms_file) {
						SQL_Run(sqlite3_snprintf(512, sBuf, "DELETE FROM song WHERE path=\'%q\'", str.body), sql); 
					}
					else {
						SQL_Run(sqlite3_snprintf(512, sBuf, "DELETE FROM folder WHERE path=\'%q\'", str.body), sql);
					}
				}
				else if (chg == 2) {
					cChange++;
					if (is_bms_file) {
						SQL_Run(sqlite3_snprintf(1024, sBuf, "DELETE FROM song WHERE path=\'%q\'", str.body), sql);
						BMSMETA meta;
						ParseBMSMETA(&meta, str, 1);
						LoadBMSMETAFromDB(&meta, sql);
						SQL_Run(sqlite3_snprintf(1024, sBuf, "INSERT INTO song (hash,title,subtitle,genre,artist,subartist,level,date,path,folder,stagefile,banner,backbmp,parent,maxbpm,minbpm,random,longnote,judge,mode,bga,difficulty,favorite,type,txt,karinotes,adddate,exlevel) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,%d)",
							meta.hash.body, meta.title.body, meta.subtitle.body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.selLevel, newTime, str.body, AssignCRC32(meta.folderpath).body, meta.stagefilepath.body, meta.bannerpath.body, meta.backBMPpath.body,AssignCRC32(meta.parentfolderpath).body,meta.maxbpm,meta.minbpm,meta.random,meta.longnote,meta.judge,meta.keymode,meta.bga,meta.difficulty,meta.hasTxt,meta.notecount,now,meta.exlevel), sql);
					}
					else if (is_lr2folder) {
						SQL_Run(sqlite3_snprintf(1024, sBuf, "DELETE FROM folder WHERE path=\'%q\'", str.body), sql);
						BMSMETA meta;
						ParseBMSMETA(&meta, str, 1);
						SQL_Run(sqlite3_snprintf(1024, sBuf, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , 2 , \'%q\' , %d) ",
							str.body, meta.title.body, AssignCRC32(meta.folderpath).body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.tag.body, meta.selLevel, newTime, meta.bannerpath.body, now), sql);
					}
					else {
						CSTR folderinfo(str);
						folderinfo.add("folderinfo.txt");
						if (IsFileExist(folderinfo)) {
							BMSMETA meta;
							ParseBMSMETA(&meta, folderinfo, 0);
							if (meta.judge != 2) meta.judge = 1;
							if (SQL_Run(sqlite3_snprintf(512, sBuf, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type  , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , \'%q\' , %d) ",
								str.body, meta.title.body, AssignCRC32(meta.folderpath).body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.tag.body, meta.selLevel, newTime, meta.judge, meta.bannerpath.body, now), sql) == 0) {
								SearchSongsFromPath(str, sql, str);
							}
						}
						else {
							if (SQL_Run(sqlite3_snprintf(512, sBuf, "UPDATE folder SET date = %d , adddate = %d WHERE path = \'%q\'", newTime, now, str.body), sql) == 0) {
								SearchSongsFromPath(str, sql, str);
							}
						}
					}
				}
				
			}
			else {
				if (is_bms_file) {
					SQL_Run(sqlite3_snprintf(512, sBuf, "DELETE FROM song WHERE path=\'%q\'", str.body), sql);
				}
				else {
					SQL_Run(sqlite3_snprintf(512, sBuf, "DELETE FROM folder WHERE path=\'%q\'", str.body), sql);
				}
			}
		}
	}
	GetTimeWrap();

	if (cAlready == 0 && cNot == 0) {
		if(cChange == 0){
			ErrorLogFmtAdd("曲が見つかりません / No matching songs found: %s\n", query.body);
			return 0;
		}
	}
	else if (cChange == 0) {
		ErrorLogFmtAdd("更新はありません / No updates\n");
		return 1;
	}
	ErrorLogFmtAdd("更新しました / Updated. missing %d changed %d\n", cNot, cChange);
	return 2;
}

int CMP_SongDataByDifficulty(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	if (s1->keymode != s2->keymode) {
		return s1->keymode - s2->keymode;
	}

	if ((s1->difficulty == 5) == (s2->difficulty == 5)) {
		if (s1->level != s2->level) return s1->level - s2->level;
		return CMP_SongDataByTitle(p1, p2);
	}
	if (s1->difficulty == 5 && s2->difficulty != 5) return 1;
	if (s1->difficulty != 5 && s2->difficulty == 5) return -1;

	return 0;
}

int CMP_SongDataByClear(const void *p1, const void *p2) {
	SONGDATA* s1 = (SONGDATA*)p1;
	SONGDATA* s2 = (SONGDATA*)p2;

	if (s1->folderType != s2->folderType) {
		return s1->folderType - s2->folderType;
	}

	if (s1->mybest.clear != s2->mybest.clear) return s1->mybest.clear - s2->mybest.clear;

	return CMP_SongDataByDifficulty(p1, p2);
}

int SearchCourseFromDB(sqlite3 *sql, SONGSELECT *ss, int keys, int multistagemode) {

	sqlite3_stmt *pStmt;
	CSTR str;

	if (ss->filter.ignorekeyall == 1 && keys == 0) keys = 1;
	if (ss->filter.ignorekeysingle == 1 && keys == 1) keys = 2;
	if (ss->filter.ignorekeydouble == 1 && keys == 4) keys = 5;
	if (ss->filter.ignoredp == 1 && keys < 7) keys = 7;
	if (ss->filter.ignorepms == 1 && keys == 7) keys = 0;
	if (ss->filter.ignorekeyall == 1 && keys == 0) keys = 1;

	GetNowUnixtime();
	ss->rivalID = 0;

	CSTR query;
	if (multistagemode == 0) {
		query = "SELECT * FROM expert LEFT JOIN score ON expert.hash = score.hash";
		ss->course.type = 0;
	}
	else if (multistagemode == 1) {
		query = "SELECT * FROM nonstop LEFT JOIN score ON nonstop.hash = score.hash";
		ss->course.type = 1;
	}
	else if (multistagemode == 2) {
		query = "SELECT * FROM grade LEFT JOIN score ON grade.hash = score.hash";
		ss->course.type = 2;
	}

	switch (keys) {
		case 0:
			query.add(" ");
			break;
		case 1:
			query.add(" WHERE line=7 OR line=5");
			break;
		case 2:
			query.add(" WHERE line=7");
			break;
		case 3:
			query.add(" WHERE line=5");
			break;
		case 4:
			query.add(" WHERE line=14 OR line=10");
			break;
		case 5:
			query.add(" WHERE line=14");
			break;
		case 6:
			query.add(" WHERE line=10");
			break;
		case 7:
			query.add(" WHERE line=9");
			break;
	}
	query.add(" ORDER BY id DESC");

	SQL_prepare(query, sql, &pStmt);
	ss->prevListCount = 0;
	SONGDATA song;
	while (sqlite3_step(pStmt) == 100) {

		if (ss->prevListSize - 1 == ss->prevListCount) {
			ss->prevList = (SONGDATA*)realloc(ss->prevList, (ss->prevListSize + 1000) * sizeof(SONGDATA));
			assert(ss->prevList != nullptr);
			for (int i = ss->prevListSize; i < ss->prevListSize + 1000; i++) {
				memset(&ss->prevList[i], 0, sizeof(SONGDATA));
			}
			ss->prevListSize += 1000;
		}
		InitSongData(&song);
		song.folderType = 8;
		cstrSprintf(&song.filepath, "%d", ss->prevListCount);
		song.courseID = sqlite3_column_int(pStmt, 0);
		song.title = SQL_GetColumn(1, pStmt);
		song.fulltitle = song.title;
		song.keymode = sqlite3_column_int(pStmt, 2);
		song.hash = SQL_GetColumn(3, pStmt);
		song.courseStageCount = song.hash.length()/32 - 1;
		song.courseIR = sqlite3_column_int(pStmt, 4);
		if (song.courseIR == 1) song.courseReadOnly = 1;
		if (song.courseStageCount > 9) song.courseStageCount = 10;
		if (song.hash.length() > 63) {
			for (int i = 0; i < 10; i++) {
				song.courseKeys[i] = *song.hash.atPos(i) - 0x30;
			}
			song.courseType = *song.hash.atPos(10) - 0x30;
			song.coursePlayable = 1;
		}

		for (int i = 1; i <= song.courseStageCount; i++) {
			SONGDATA newsong;
			if (GetSongData(song.hash.getSliced(i * 32, 32),&newsong, sql, ss) == 1) {
				song.courseHash[i - 1] = newsong.hash;
				song.courseTitle[i - 1] = newsong.title;
				song.courseSubtitle[i - 1] = newsong.subtitle;
				song.courseLevel[i - 1] = newsong.level;
				song.courseTotalnote[i - 1] = newsong.mybest.total_notes;
				song.courseStageDifficulty[i - 1] = newsong.difficulty;
			}
			else {
				song.courseHash[i - 1].fillzero();
				song.courseTitle[i - 1] = "この曲を所持していません";
				song.courseLevel[i - 1] = -1;
				song.coursePlayable = 0;
			}

		}
		if (song.courseStageCount == 0) {
			song.artist = "曲が設定されていないのでプレイできません";
			song.coursePlayable = 0;
		}
		else if (song.coursePlayable == 0) {
			song.artist = "曲が設定されていないのでプレイできません";
		}

		if (multistagemode == 1) { //nonstop
			cstrSprintf(&str, fs::make_preferred("LR2files/Replay/%s/%s.lr2rep").data(), ss->playerID.body, MD5str(song.hash));
		}
		else {
			cstrSprintf(&str, fs::make_preferred("LR2files/Replay/%s/c/%s").data(), ss->playerID.body, MD5str(song.hash));
		}
		song.replayExist = IsFileExist(str);
		song.mybest.clear = sqlite3_column_int(pStmt, 6);
		song.mybest.stat_pgreat = sqlite3_column_int(pStmt, 7);
		song.mybest.stat_great = sqlite3_column_int(pStmt, 8);
		song.mybest.stat_good = sqlite3_column_int(pStmt, 9);
		song.mybest.stat_bad = sqlite3_column_int(pStmt, 10);
		song.mybest.stat_poor = sqlite3_column_int(pStmt, 11);
		song.mybest.total_notes = sqlite3_column_int(pStmt, 12);
		song.mybest.stat_maxcombo = sqlite3_column_int(pStmt, 13);;
		song.mybest.minbp = sqlite3_column_int(pStmt, 14);
		song.mybest.playcount = sqlite3_column_int(pStmt, 15);
		song.mybest.clearcount = sqlite3_column_int(pStmt, 16);
		song.mybest.failcount = sqlite3_column_int(pStmt, 17);
		song.mybest.rank = sqlite3_column_int(pStmt, 18);
		song.mybest.rate = sqlite3_column_int(pStmt, 19);
		if (song.mybest.rank < 9) {
			if (song.mybest.rank == 0 && song.mybest.stat_pgreat + song.mybest.stat_great > 0)
				song.mybest.rank = 1;
		}
		else {
			song.mybest.rank = 8;
		}
		song.mybest.clear_db = sqlite3_column_int(pStmt, 20);
		song.mybest.op_history = sqlite3_column_int(pStmt, 21);
		song.mybest.clear_sd = sqlite3_column_int(pStmt, 24);
		song.mybest.clear_ex = sqlite3_column_int(pStmt, 25);
		song.mybest.op_best = sqlite3_column_int(pStmt, 26);
		song.mybest.rseed = sqlite3_column_int(pStmt, 27);
		song.mybest.complete = sqlite3_column_int(pStmt, 28);
		str = SQL_GetColumn(22, pStmt);
		if (isSameScoreHash(&song.mybest, &ss->playerPassMD5, &song.hash, &str)) {
			song.mybest.stat_exscore = song.mybest.stat_great + song.mybest.stat_pgreat * 2;
			if (song.mybest.stat_score > 0) {
				song.mybest.stat_score = (song.mybest.stat_good + song.mybest.stat_exscore) * 2 * 50000 / song.mybest.total_notes;
			}
			if (song.mybest.minbp == 0 && song.mybest.clear != 5) song.mybest.minbp = -1;
		}
		else {
			memset(&song.mybest, 0, sizeof(STATUS));
			song.mybest.total_notes = 0;
			song.mybest.minbp = -1;
		}

		COPY_SONGDATA(&ss->prevList[ss->prevListCount], &song);
		ss->prevListCount++;
	}
	InitSongData(&song);
	song.title = "NEW COURSE";
	song.fulltitle = "NEW COURSE";
	song.artist = "新規コースを作成します";
	song.hash = "newcourse";
	song.filepath = "newcourse";
	song.folderType = 7;
	song.level = 0;
	COPY_SONGDATA(&ss->prevList[ss->prevListCount], &song);
	ss->prevListCount++;
	if (ss->course.type != 2) {
		song.title = "RANDOM MIX";
		song.fulltitle = "RANDOM MIX";
		song.artist = "ランダムでコースを作成します";
		song.hash = "randomcourse";
		song.filepath = "randomcourse";
		song.folderType = 9;
		song.level = 0;
		COPY_SONGDATA(&ss->prevList[ss->prevListCount], &song);
		ss->prevListCount++;
	}
	return ss->prevListCount;
}

int LoadBmsListFromDB(CSTR query, sqlite3 *sql, SONGSELECT *ss, int *difficulty, int *key, int sort, int count) {
	
	sqlite3_stmt *stmt;
	SONGDATA song;
	char buf[1024];


	if (query.left(6).isDiff("SELECT")) return -1;

	if (ss->filter.ignorekeyall == 1 && *key == 0) *key = 1;
	if (ss->filter.ignorekeysingle == 1 && *key == 1) *key = 2;
	if (ss->filter.ignorekeydouble == 1 && *key == 4) *key = 5;
	if (ss->filter.ignoredp == 1 && 3 < *key && *key < 7) *key = 7;
	if (ss->filter.ignorepms == 1 && *key == 7) *key = 0;
	if (ss->filter.ignorekeyall == 1 && *key == 0) *key = 1;
	if (ss->filter.ignoredifficultyall == 1 && *difficulty == 0) *difficulty = 1;

	SQL_prepare(query, sql, &stmt);
	ErrorLogFmtAdd("結果…%s\n", sqlite3_errmsg(sql));
	int i = 0;
	while (sqlite3_step(stmt) == 100) {
		if (count != 0 && i == count) break;
		if (ss->prevListSize - 1 == i) {
			ss->prevList = (SONGDATA*)realloc(ss->prevList, (ss->prevListSize + 1000) * sizeof(SONGDATA));
			assert(ss->prevList != nullptr);
			for (int j = ss->prevListSize; j < ss->prevListSize + 1000; j++) {
				memset(&ss->prevList[j], 0, sizeof(SONGDATA)); // FIXME: bad memset
			}
			ss->prevListSize += 1000;
		}
		InitSongData(&song);
		song.title = SQL_GetColumn(0, stmt);
		song.subtitle = SQL_GetColumn(1, stmt);
		song.genre = SQL_GetColumn(2, stmt);
		song.artist = SQL_GetColumn(3, stmt);
		song.subartist = SQL_GetColumn(4, stmt);
		song.tag = SQL_GetColumn(5, stmt);
		song.filepath = SQL_GetColumn(6, stmt);
		song.folderType = sqlite3_column_int(stmt, 7);
		song.folder = SQL_GetColumn(9, stmt);
		song.level = sqlite3_column_int(stmt, 11);
		song.adddate = sqlite3_column_int(stmt, 12);
		song.keymode = 0;
		song.fulltitle = song.title;
		COPY_SONGDATA(&ss->prevList[i], &song);
		i++;
	}

	if (i != 0) {
		sqlite3_finalize(stmt);
		ss->prevListCount = i;
		ss->is_loading_bmslist = 1;
		qsort(ss->prevList,ss->prevListCount,sizeof(SONGDATA),CMP_SongDataByPath);
		if (ss->flag_folderlamp) {
			for (int i = 0; i < ss->prevListCount; i++) {
				CSTR query(1024);
				if (ss->prevList[i].tag.isDiff("(null)") && ss->prevList[i].tag.length() >= 5) {
					query = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE %s", ss->prevList[i].tag.body);
					if (query.findStrPos("__NEWSONG__") > -1) {
						query = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE adddate > %d", GetNowUnixtime() - ss->titleflash * 3600);
					}
					LoadFolderDataFromDB(query, &ss->prevList[i], sql, *difficulty, *key, sort, ss->prevList[i].level, &ss->filter, 1);
				}
				else {
					query = sqlite3_snprintf(1024, buf, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE parent = \'%s\'", AssignCRC32(ss->prevList[i].filepath).body);
					LoadFolderDataFromDB(query, &ss->prevList[i], sql, *difficulty, *key, sort, 0, &ss->filter, 0);
				}
			}
		}
		ErrorLogFmtAdd("成功　フォルダ数は%dです。\n", ss->prevListCount);
		return ss->prevListCount;
	}

	ErrorLogFmtAdd("フォルダが見つかりません。%s...\n", query.body);
	return -1;
}

int WriteRandomCourse(sqlite3 *sql, COURSESELECT *course, SONGSELECT *ss, CONFIG_COURSE cfg, int key) {
	
	sqlite3_stmt *pStmt;

	ErrorLogAdd("ランダムコースを作成します\n");
	CSTR query;

	if (ss->course.type == 0) {
		SQL_Run("DELETE FROM expert WHERE title = \'RANDOM MIX\'", sql);
	}
	else if (ss->course.type == 2) {
		SQL_Run("DELETE FROM grade WHERE title = \'RANDOM MIX\'", sql);
	}
	else if (ss->course.type == 1) {
		SQL_Run("DELETE FROM nonstop WHERE title = \'RANDOM MIX\'", sql);
	}

	course->name = "RANDOM MIX";
	course->count = 0;
	course->id = -1;

	int stages = cfg.stage;
	if (cfg.stage <= 0 || cfg.stage > 5) {
		stages = GetRand(3)+2;
	}

	int maxlevel = 999999;
	int minlevel = -999999;
	if (cfg.maxlevel > 0) maxlevel = cfg.maxlevel;
	if (cfg.minlevel > 0) minlevel = cfg.minlevel;

	int maxbpm = 999999;
	int minbpm = -999999;
	if (cfg.maxbpm > 0) maxbpm = cfg.maxbpm;
	if (cfg.minbpm > 0) minbpm = cfg.minbpm;

	if (cfg.bpmrange <= 0) {
		cstrSprintf(&query, "SELECT hash,folder FROM song WHERE mode=%d AND %d<=minbpm AND maxbpm<=%d AND %d<=level AND level<=%d ORDER BY random()", key, minbpm, maxbpm, minlevel, maxlevel);
	}
	else {
		cstrSprintf(&query, "SELECT hash,folder FROM song WHERE mode=%d AND maxbpm=minbpm AND %d<=maxbpm AND maxbpm<=%d AND %d<=level AND level<=%d ORDER BY random()", key, minbpm, maxbpm, minlevel, maxlevel);
	}

	SQL_prepare(query, sql, &pStmt);
	if (sqlite3_step(pStmt) == 100) {
		CSTR songhash = GetRandomSongHash(SQL_GetColumn(0, pStmt), SQL_GetColumn(1, pStmt), cfg.optimumlevel_7, key, sql, maxlevel, minlevel);
		if (GetSongData(songhash, &course->data[course->count], sql, ss) == 1) {
			course->count++;
			sqlite3_finalize(pStmt);

			int rangeminbpm = minbpm;
			int rangemaxbpm = maxbpm;
			if (cfg.bpmrange > 0) {
				if (minbpm < course->data[0].maxBPM - cfg.bpmrange)
					rangeminbpm = course->data[0].maxBPM - cfg.bpmrange;
				if (course->data[0].maxBPM + cfg.bpmrange < maxbpm)
					rangemaxbpm = course->data[0].maxBPM + cfg.bpmrange;
			}
			cstrSprintf(&query, "SELECT hash,folder FROM song WHERE mode=%d AND %d<=minbpm AND maxbpm<=%d  AND %d<=level AND level<=%d ORDER BY random()", key, rangeminbpm, rangemaxbpm, minlevel, maxlevel);
			SQL_prepare(query, sql, &pStmt);
			while (sqlite3_step(pStmt) == 100) {

				bool isSamesong = 0;
				for (int i = 0; i < course->count; i++) {
					if (SQL_GetColumn(1, pStmt).isSame(&course->data[i].folder))
						isSamesong = true;
				}

				if (!isSamesong) {
					CSTR songhash = GetRandomSongHash(SQL_GetColumn(0, pStmt), SQL_GetColumn(1, pStmt), cfg.optimumlevel_7, key, sql, maxlevel, minlevel);
					if (GetSongData(songhash, &course->data[course->count], sql, ss) == 1) {
						course->count++;
					}
				}

				if (course->count == stages) break;
			}
			sqlite3_finalize(pStmt);
			return 1;
		}
		sqlite3_finalize(pStmt);
	}
	else {
		sqlite3_finalize(pStmt);
	}
	return 0;
}


// TOFIX: unneccessary codes
int GetFolderDataFromPath(CSTR path, sqlite3 *sql) {
#ifdef _WIN32
	HANDLE hFindFile;
	_WIN32_FIND_DATAA findFileData;
	char str[1024];
	int filetime, now;

	ErrorLogFmtAdd("ルートの読み込みを行います。パス%s\n", path.body);
	ErrorLogTabAdd();
	BMSMETA meta;
	CSTR searchPath(path);
	if (searchPath.right(1).isSame("\\") || searchPath.right(1).isSame("/")) {
		*searchPath.atPos(searchPath.length() - 1) = 0;
	}

	hFindFile = FindFirstFileA(searchPath, &findFileData);
	if (hFindFile == (HANDLE)-1) {
		FindClose((HANDLE)-1);
		ErrorLogFmtAdd("フォルダもしくはファイルが見つかりません。\n");
		ErrorLogTabSub();
		return -1;
	}
	
	filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
	now = GetNowUnixtime();
	CSTR folderinfo(path);
	folderinfo.add("folderinfo.txt");
	if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName)) && IsFileExist(folderinfo)) {
		ErrorLogFmtAdd("フォルダインフォのある通常フォルダです。\n");
		BMSMETA meta;
		ParseBMSMETA(&meta, folderinfo, 0);
		if (meta.judge != 2) meta.judge = 1;

		sqlite3_snprintf(1024, str, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , \'%q\' , %d) ",
			path.body, meta.title.body, AssignCRC32("ROOT").body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.tag.body, meta.selLevel, filetime, meta.judge, meta.bannerpath.body, now);
		if (SQL_Run(str, sql) == 0) {
			ErrorLogFmtAdd("新規登録なので検索を行います。\n");
			SearchSongsFromPath(path, sql, path);
		}
	}
	else { // folderinfo not exist
		if (IsLR2Folder(path)) {
			ErrorLogFmtAdd("カスタムフォルダです。\n");
			filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
			ParseBMSMETA(&meta, path, 0);

			meta.judge = 2;
			if (meta.tag.isSame("__NEWSONG__")) meta.judge = 3;
			else if (meta.tag.isSame("__RIVAL__")) meta.judge = 4;
			else if (meta.tag.isSame("__EXPERT__") || meta.tag.isSame("__NONSTOP__") || meta.tag.isSame("__GRADE__")) meta.judge = 6;

			sqlite3_snprintf(1024, str, "INSERT INTO folder (path , title , parent , category , info_a , info_b , command , max , date , type , banner , adddate) VALUES(\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',\'%q\',%d , %d , %d , \'%q\' , %d) ",
				path.body, meta.title.body, AssignCRC32("ROOT").body, meta.genre.body, meta.artist.body, meta.subartist.body, meta.tag.body, meta.selLevel, filetime, meta.judge, meta.bannerpath.body, now);
			SQL_Run(str, sql);
		}
		else if (((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) && (strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName))) {
			ErrorLogFmtAdd("通常フォルダです。\n");

			sqlite3_snprintf(1024, str, "INSERT INTO folder (path , title , parent , date , type , adddate) VALUES(\'%q\',\'%q\',\'%q\',%d , 1 , %d )",
				path.body, path.getFilename().body, AssignCRC32("ROOT").body, filetime, now);
			if (SQL_Run(str, sql) == 0) {
				ErrorLogFmtAdd("新規登録なので検索を行います。\n");
				SearchSongsFromPath(path, sql, path);
			}
		}
	}
	FindClose(hFindFile);
	ErrorLogFmtAdd("ルートの読み込みを終了しました。\n");
	ErrorLogTabSub();
	return 1;
#else
	// FIXME(linux): stub
	return {};
#endif // _WIN32
}

// TODO : rename variables
int LoadFilteredBmsListFromDB(CSTR query, sqlite3 *sql, SONGSELECT *ss, int *diffFilter, int *mode, uint sort, int rivalID, char flag) {

	sqlite3_stmt *pStmt;
	int lastreadDiff;
	int recentKeymode;

	ss->isExLevel = 0;
	if (query.findStrPos("exlevel") >= 0) ss->isExLevel = 1;

	if (ss->filter.ignorekeyall == 1 && *mode == 0) *mode = 1;
	if (ss->filter.ignorekeysingle == 1 && *mode == 1) *mode = 2;
	if (ss->filter.ignorekeydouble == 1 && *mode == 4) *mode = 5;
	if (ss->filter.ignoredp == 1 && 3 < *mode && *mode < 7) *mode = 7;
	if (ss->filter.ignorepms == 1 && *mode == 7) *mode = 0;
	if (ss->filter.ignorekeyall == 1 && *mode == 0) *mode = 1;
	
	if (ss->filter.ignoredifficultyall == 1 && *diffFilter == 0 && flag == 0) *diffFilter = 1;
	
	bool isIgnore = 0, isRival = 0;
	int now = GetNowUnixtime();

	ss->rivalID = 0;
	if (query.findStrPos("__EXPERT__") >= 0) {
		ss->course.type = 0;
		return SearchCourseFromDB(sql, ss, *mode, 0);
	}
	if (query.findStrPos("__NONSTOP__") >= 0) {
		ss->course.type = 1;
		return SearchCourseFromDB(sql, ss, *mode, 1);
	}
	if (query.findStrPos("__GRADE__") >= 0) {
		ss->course.type = 2;
		return SearchCourseFromDB(sql, ss, *mode, 2);
	}
	
	if (query.findStrPos("__RIVAL__") >= 0) {
		isRival = 1;
		SQL_Run("DETACH rivaldb", sql);
		CSTR str;
		cstrSprintf(&str, fs::make_preferred("ATTACH \'LR2files/Rival/%d.db\' AS rivaldb").data(), rivalID);
		SQL_Run(str, sql);
		ss->rivalID = rivalID;
		rivalID = 0;
		cstrSprintf(&query, "SELECT * FROM (song LEFT JOIN score ON song.hash = score.hash) INNER JOIN rival ON song.hash = rival.hash");
	}
	else if (query.findStrPos("INNER JOIN rival") >= 0) {
		isRival = 1;
	}
	else if (query.findStrPos("__NEWSONG__") >= 0) {
		cstrSprintf(&query, "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE adddate > %d", now - ss->titleflash * 3600);
	}

	if (query.findStrPos("favorite = 2") >= 0) {
		isIgnore = 1;
	}

	if (query.left(6).isDiff("SELECT")) return -1;

	int isRandom = ss->isRandomFolder;
	if (rivalID == 1 && query.findStrPos("random()") == -1) {
		isRandom = 1;
		*diffFilter = 0;
		query.add(" ORDER BY random()");
		*mode = 0;
	}
	if (query.findStrPos("ORDER BY") != -1) {
		isRandom = 1;
		*diffFilter = 0;
	}
	if (flag) *diffFilter = 0;
	int keyFilter = 0;
	int k5count = 0, k7count = 0, k10count = 0, k14count = 0, k9count = 0;
	int diffcount[6] = { 0, };
	CSTR queryCpy(query);
	if (isRandom == 0) {
		switch (*mode) {
			case 1:
				keyFilter = 1;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 2:
				keyFilter = 7;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 3:
				keyFilter = 5;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 4:
				keyFilter = 2;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 5:
				keyFilter = 14;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 6:
				keyFilter = 10;
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
			case 7:
				keyFilter = 9;
				query.add(" ORDER BY folder ASC,difficulty ASC,level ASC");
				break;
			default:
				query.add(" ORDER BY folder ASC,mode ASC,difficulty ASC,level ASC");
				break;
		}
	}

	GetTimeWrap();
	SQL_prepare(query, sql, &pStmt);
	int count = 0;
	SONGDATA song;
	CSTR lastreadFolder, thisFolder;
	CSTR replayPath;
	//
	int countSameDiff = 0;
	int lastKeymode = -1;
	CSTR lastFolder;
	int diffLevel[5] = { -1, };
	int diffBarLamp[5] = { -1, };
	int diffExist[5] = { -1, };
	for (int i = 0; i < 5; i++) diffLevel[i] = -1;
	for (int i = 0; i < 5; i++) diffBarLamp[i] = -1;
	for (int i = 0; i < 5; i++) diffExist[i] = -1;
	InitSongData(&song);
	CSTR nowFolder;
	int nowCount = 0;
	int lastreadKeymode = 0;

	while (sqlite3_step(pStmt) == 100 && (rivalID == 0 || count != rivalID)) {
		int t = sqlite3_column_int(pStmt, 18);
		if ((ss->filter.ignoredp != 1 || t < 10) && (ss->filter.ignorepms != 1 || t != 9)) {

			if (count == ss->prevListSize - 1) {
				ss->prevList = (SONGDATA*)realloc(ss->prevList, (ss->prevListSize + 1000) * sizeof(SONGDATA));
				assert(ss->prevList != nullptr);
				for (int i = ss->prevListSize; i < ss->prevListSize + 1000; i++) {
					memset(&ss->prevList[i], 0, sizeof(SONGDATA));
				}
				ss->prevListSize += 1000;
			}
			
			song.keymode = t;
			song.difficulty = sqlite3_column_int(pStmt, 15);
			song.favorite = sqlite3_column_int(pStmt, 24);
			if (ss->filter.ignore5key == 1) {
				
				song.folder = SQL_GetColumn(9, pStmt);
				if (nowFolder.isDiff(song.folder)) {
					nowCount = 0;
				}
				else {
					if (lastreadKeymode == 5 && song.keymode == 7) {
						count -= nowCount;
						nowCount = 0;
					}
					else if (lastreadKeymode == 10 && song.keymode == 14) {
						count -= nowCount;
						nowCount = 0;
					}
					else if (lastreadKeymode != song.keymode) {
						nowCount = 0;
					}
				}
			}
			if (isIgnore == 0 && song.favorite != 2) {
				if (song.keymode == 5) k5count++;
				else if (song.keymode == 7) k7count++;
				else if (song.keymode == 10) k10count++;
				else if (song.keymode == 14) k14count++;
				else if (song.keymode == 9) k9count++;
			}
			if (count < 0) count = 0;
						
			if ((song.favorite != 2 && ((song.keymode == 7 && keyFilter == 7) || (song.keymode <= 8 && *mode == 1 && keyFilter == 1) || (song.keymode == 14 && keyFilter >= 14) || (song.keymode >= 10 && *mode == 4 && keyFilter == 2) || ((song.keymode == 9 || song.keymode == 10 || song.keymode == 5) && song.keymode == keyFilter) || keyFilter == 0)) || song.keymode == 0 || isIgnore == 1) {
				if (song.difficulty == 1) {
					diffcount[1] ++;
				}
				else if (song.difficulty == 2) {
					diffcount[2]++;
				}
				else if (song.difficulty == 3) {
					diffcount[3]++;
				}
				else if (song.difficulty == 4) {
					diffcount[4]++;
				}
				else if (song.difficulty == 5) {
					diffcount[5]++;
				}
				else {
					diffcount[0]++;
				}
				song.maxBPM = sqlite3_column_int(pStmt, 16);
				song.minBPM = sqlite3_column_int(pStmt, 17);
				song.hash = SQL_GetColumn(0, pStmt);
			
				//not same code with original. logic is same
				bool unique = 1;
				for (int i = 0; i < count; i++) {
					if (song.hash.isSame(ss->prevList[i].hash)) {
						unique = 0;
						i = count;
					}
				}
				if(unique) {
					if (song.hash.isSame("9cbea4427d7e8caff0d28f9d7600cdec")) 
						song.title = AutomationFactory();
					else 
						song.title = SQL_GetColumn(1, pStmt);
					song.subtitle = SQL_GetColumn(2, pStmt);
					song.genre = SQL_GetColumn(3, pStmt);
					song.filepath = SQL_GetColumn(7, pStmt);
					song.folderType = sqlite3_column_int(pStmt, 8);
					song.level = sqlite3_column_int(pStmt, 14);
					song.artist = SQL_GetColumn(4, pStmt);
					song.subartist = SQL_GetColumn(5, pStmt);
					song.keymode = sqlite3_column_int(pStmt, 18);
					song.judge = sqlite3_column_int(pStmt, 19);
					song.longnote = sqlite3_column_int(pStmt, 20);
					song.bga = sqlite3_column_int(pStmt, 21);
					song.random = sqlite3_column_int(pStmt, 22);
					song.txt = sqlite3_column_int(pStmt, 25);
					song.favorite = sqlite3_column_int(pStmt, 24);
					song.adddate = sqlite3_column_int(pStmt, 27);
					song.exlevel = sqlite3_column_int(pStmt, 28);
					song.stagefile = SQL_GetColumn(10, pStmt);
					if (song.stagefile.isDiff("(null)") && song.stagefile.length() > 4) song.isStagefile = 1;
					song.banner = SQL_GetColumn(11, pStmt);
					if (song.banner.isDiff("(null)") && song.banner.length() > 4) song.isBanner = 1;
					song.backBMP = SQL_GetColumn(12, pStmt);
					if (song.backBMP.isDiff("(null)") && song.backBMP.length() > 4) song.isBackBMP = 1;

					if (song.subtitle.isDiff("(null)"))
						cstrSprintf(&song.fulltitle, "%s %s", song.title.body, song.subtitle.body);
					else
						song.fulltitle = song.title;

					song.tag = SQL_GetColumn(6, pStmt);
					song.difficulty = sqlite3_column_int(pStmt, 15);
					cstrSprintf(&replayPath, fs::make_preferred("LR2files/Replay/%s/%s.lr2rep").data(), ss->playerID.body, song.hash.body);
					song.replayExist = IsFileExist(replayPath);
					thisFolder = SQL_GetColumn(9, pStmt);
					song.folder = thisFolder;

					if (song.keymode > 0) {
						song.mybest.clear = sqlite3_column_int(pStmt, 30);
						song.mybest.stat_pgreat = sqlite3_column_int(pStmt, 31);
						song.mybest.stat_great = sqlite3_column_int(pStmt, 32);
						song.mybest.stat_good = sqlite3_column_int(pStmt, 33);
						song.mybest.stat_bad = sqlite3_column_int(pStmt, 34);
						song.mybest.stat_poor = sqlite3_column_int(pStmt, 35);
						song.mybest.total_notes = sqlite3_column_int(pStmt, 36);
						song.mybest.stat_maxcombo = sqlite3_column_int(pStmt, 37);
						song.mybest.minbp = sqlite3_column_int(pStmt, 38);
						song.mybest.playcount = sqlite3_column_int(pStmt, 39);
						song.mybest.clearcount = sqlite3_column_int(pStmt, 40);
						song.mybest.failcount = sqlite3_column_int(pStmt, 41);
						song.mybest.rank = sqlite3_column_int(pStmt, 42);
						song.mybest.rate = sqlite3_column_int(pStmt, 43);
						if (song.mybest.rank < 8) {
							if (song.mybest.rank == 0 && (0 < song.mybest.stat_great + song.mybest.stat_pgreat)) {
								song.mybest.rank = 1;
							}
						}
						else {
							song.mybest.rank = 8;
						}
						song.mybest.clear_db = sqlite3_column_int(pStmt, 44);
						song.mybest.op_history = sqlite3_column_int(pStmt, 45);
						song.mybest.clear_sd = sqlite3_column_int(pStmt, 48);
						song.mybest.clear_ex = sqlite3_column_int(pStmt, 49);
						song.mybest.op_best = sqlite3_column_int(pStmt, 50);
						song.mybest.rseed = sqlite3_column_int(pStmt, 51);
						song.mybest.complete = sqlite3_column_int(pStmt, 52);
						if (isRival) {
							song.folderType = 5;
							song.rivalRecord.clear = sqlite3_column_int(pStmt, 54);
							song.rivalRecord.total_notes = sqlite3_column_int(pStmt, 55);
							song.rivalRecord.stat_maxcombo = sqlite3_column_int(pStmt, 56);
							song.rivalRecord.stat_pgreat = sqlite3_column_int(pStmt, 57);
							song.rivalRecord.stat_great = sqlite3_column_int(pStmt, 58);
							song.rivalRecord.stat_good = sqlite3_column_int(pStmt, 59);
							song.rivalRecord.stat_bad = sqlite3_column_int(pStmt, 60);
							song.rivalRecord.stat_poor = sqlite3_column_int(pStmt, 61);
							song.rivalRecord.minbp = sqlite3_column_int(pStmt, 62);
							song.rivalRecord.op_best = sqlite3_column_int(pStmt, 63);
							song.rivalRecord.stat_exscore = song.rivalRecord.stat_great + song.rivalRecord.stat_pgreat * 2;
							if (0 < song.rivalRecord.total_notes) {
								song.rivalRecord.stat_score = ((song.rivalRecord.stat_good + song.rivalRecord.stat_exscore * 2) * 50000) / song.rivalRecord.total_notes;
								song.rivalRecord.rate = (song.rivalRecord.stat_exscore * 50) / song.rivalRecord.total_notes;
								song.rivalRecord.rank = (song.rivalRecord.stat_exscore * 9) / (song.rivalRecord.total_notes * 2);
								if (song.rivalRecord.rank < 9) {
									if (song.rivalRecord.rank == 0 && (0 < song.rivalRecord.stat_great + song.rivalRecord.stat_pgreat)) {
										song.rivalRecord.rank = 1;
									}
								}
								else {
									song.rivalRecord.rank = 8;
								}
							}
						}
						CSTR bestHash;
						bestHash = SQL_GetColumn(46, pStmt);
						if (isSameScoreHash(&song.mybest, &ss->playerPassMD5, &song.hash, &bestHash)) {
							song.mybest.stat_exscore = song.mybest.stat_great + song.mybest.stat_pgreat * 2;
							if (song.mybest.total_notes > 0) {
								song.mybest.stat_score = ((song.mybest.stat_good + song.mybest.stat_exscore * 2) * 50000) /	song.mybest.total_notes;
							}
							if ((song.mybest.minbp == 0) && (song.mybest.clear != 5)) {
								song.mybest.minbp = -1;
							}
						}
						else {
							memset(&song.mybest, 0, sizeof(STATUS));
							song.mybest.minbp = -1;
							song.mybest.total_notes = sqlite3_column_int(pStmt, 26);
						}
					}
					if (lastFolder.isDiff(song.folder) || lastKeymode != song.keymode) {
						for (int i = count - 1; i >= 0; i--) {
							if (ss->prevList[i].folder.isDiff(lastFolder) || ss->prevList[i].keymode != lastKeymode)	break;

							if (ss->prevList[i].difficulty == 1) {
								ss->prevList[i].difficultyLevel[0] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[0] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[0] = diffLevel[0];
								ss->prevList[i].difficultyLevelBarLamp[0] = diffBarLamp[0];
							}
							ss->prevList[i].difficultyExist[0] = diffExist[0];

							if (ss->prevList[i].difficulty == 2) {
								ss->prevList[i].difficultyLevel[1] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[1] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[1] = diffLevel[1];
								ss->prevList[i].difficultyLevelBarLamp[1] = diffBarLamp[1];
							}
							ss->prevList[i].difficultyExist[1] = diffExist[1];

							if (ss->prevList[i].difficulty == 3) {
								ss->prevList[i].difficultyLevel[2] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[2] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[2] = diffLevel[2];
								ss->prevList[i].difficultyLevelBarLamp[2] = diffBarLamp[2];
							}
							ss->prevList[i].difficultyExist[2] = diffExist[2];

							if (ss->prevList[i].difficulty == 4) {
								ss->prevList[i].difficultyLevel[3] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[3] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[3] = diffLevel[3];
								ss->prevList[i].difficultyLevelBarLamp[3] = diffBarLamp[3];
							}
							ss->prevList[i].difficultyExist[3] = diffExist[3];

							if (ss->prevList[i].difficulty == 5) {
								ss->prevList[i].difficultyLevel[4] = ss->prevList[i].level;
								ss->prevList[i].difficultyLevelBarLamp[4] = ss->prevList[i].mybest.clear;
							}
							else {
								ss->prevList[i].difficultyLevel[4] = diffLevel[4];
								ss->prevList[i].difficultyLevelBarLamp[4] = diffBarLamp[4];
							}
							ss->prevList[i].difficultyExist[4] = diffExist[4];
						}
						diffBarLamp[0] = -1;
						diffBarLamp[1] = -1;
						diffBarLamp[2] = -1;
						diffBarLamp[3] = -1;
						diffBarLamp[4] = -1;
						diffExist[0] = 0;
						diffExist[1] = 0;
						diffExist[2] = 0;
						diffExist[3] = 0;
						diffExist[4] = 0;
						diffLevel[0] = -1;
						diffLevel[1] = -1;
						diffLevel[2] = -1;
						diffLevel[3] = -1;
						diffLevel[4] = -1;
					}
					lastFolder = song.folder;
					lastKeymode = song.keymode;
					if (1 <= song.difficulty && song.difficulty < 6) {
						if (diffLevel[song.difficulty - 1] == -1) {
							diffLevel[song.difficulty - 1] = song.level;
							diffBarLamp[song.difficulty - 1] = song.mybest.clear;
						}
						song.difficultyCount = diffExist[song.difficulty - 1];
						diffExist[song.difficulty - 1] = song.difficultyCount + 1;
					}
					if (song.keymode == 0 || ss->isExLevel == 1) {
						lastreadFolder = thisFolder;
						COPY_SONGDATA(&ss->prevList[count], &song);
						recentKeymode = song.keymode;
						countSameDiff = 1;
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
					else if (thisFolder.isDiff(lastreadFolder)) {
						lastreadFolder = thisFolder;
						recentKeymode = song.keymode;
						lastreadDiff = song.difficulty;
						countSameDiff = 1;
						COPY_SONGDATA(&ss->prevList[count], &song);
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
					else if (song.keymode != recentKeymode) {
						recentKeymode = song.keymode;
						lastreadDiff = song.difficulty;
						countSameDiff = 1;
						COPY_SONGDATA(&ss->prevList[count], &song);
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
					else if (lastreadDiff < song.difficulty && song.difficulty <= *diffFilter && *diffFilter) {
						nowCount -= countSameDiff;
						count -= countSameDiff;
						if (nowCount < 1) nowCount = 0;
						lastreadDiff = song.difficulty;
						countSameDiff = 1;
						COPY_SONGDATA(&ss->prevList[count], &song);
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
					else if (lastreadDiff == song.difficulty || *diffFilter == 0) {
						countSameDiff++;
						COPY_SONGDATA(&ss->prevList[count], &song);
						nowCount++;
						count++;
						nowFolder = song.folder;
						lastreadKeymode = song.keymode;
					}
				}
				InitSongData(&song);
			}
		}
	}

	for (int i = count-1; i >= 0; i--) {

		if (ss->prevList[i].folder.isDiff(lastFolder) || ss->prevList[i].keymode != lastKeymode)	break;

		if (ss->prevList[i].difficulty == 1) {
			ss->prevList[i].difficultyLevel[0] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[0] = ss->prevList[i].mybest.clear;
		}
		else {
			ss->prevList[i].difficultyLevel[0] = diffLevel[0];
			ss->prevList[i].difficultyLevelBarLamp[0] = diffBarLamp[0];
		}
		ss->prevList[i].difficultyExist[0] = diffExist[0];

		if (ss->prevList[i].difficulty == 2) {
			ss->prevList[i].difficultyLevel[1] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[1] = ss->prevList[i].mybest.clear;
		}
		else {
			ss->prevList[i].difficultyLevel[1] = diffLevel[1];
			ss->prevList[i].difficultyLevelBarLamp[1] = diffBarLamp[1];
		}
		ss->prevList[i].difficultyExist[1] = diffExist[1];

		if (ss->prevList[i].difficulty == 3) {
			ss->prevList[i].difficultyLevel[2] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[2] = ss->prevList[i].mybest.clear;
		}
		else {
			ss->prevList[i].difficultyLevel[2] = diffLevel[2];
			ss->prevList[i].difficultyLevelBarLamp[2] = diffBarLamp[2];
		}
		ss->prevList[i].difficultyExist[2] = diffExist[2];

		if (ss->prevList[i].difficulty == 4) {
			ss->prevList[i].difficultyLevel[3] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[3] = ss->prevList[i].mybest.clear;
		}
		else {
			ss->prevList[i].difficultyLevel[3] = diffLevel[3];
			ss->prevList[i].difficultyLevelBarLamp[3] = diffBarLamp[3];
		}
		ss->prevList[i].difficultyExist[3] = diffExist[3];

		if (ss->prevList[i].difficulty == 5) {
			ss->prevList[i].difficultyLevel[4] = ss->prevList[i].level;
			ss->prevList[i].difficultyLevelBarLamp[4] = ss->prevList[i].mybest.clear;
		}
		else{
			ss->prevList[i].difficultyLevel[4] = diffLevel[4];
			ss->prevList[i].difficultyLevelBarLamp[4] = diffBarLamp[4];
		}
		ss->prevList[i].difficultyExist[4] = diffExist[4];
	}
	sqlite3_finalize(pStmt);
	GetTimeWrap();

	if (ss->unk5000 == 1 && count <= 0) {
		ss->unk5000 = 0;
		ErrorLogAdd("適正な曲が見つかりません。リトライも行いません。\n");
		return 0;
	}
	else if (count > 0) {
		if (!(ss->unk5000 || count <= 0 || *diffFilter <= 0 || diffcount[*diffFilter] || (k5count == 0 && k7count == 0 && k10count == 0 && k14count == 0 && k9count == 0))) {
			int diffCycle = *diffFilter;
			do {
				if (*diffFilter == 0) break;
				*diffFilter++;
				if (*diffFilter >= 6) *diffFilter = 0;
				if (ss->filter.ignoredifficultyall && *diffFilter == 0) *diffFilter = 1;
			} while (diffcount[*diffFilter] == 0);

			if (*diffFilter == diffCycle) {
				ErrorLogAdd("適正な曲が見つかりません 5\n");
				return -1;
			}
			ErrorLogFmtAdd("難度を変更します。　%d→%d\n", diffCycle, *diffFilter);
			flag = 0;
			return LoadFilteredBmsListFromDB(queryCpy, sql, ss, diffFilter, mode, sort, rivalID, flag);
		}
	}
	else if (ss->unk5000 == 0) {
		if (!k5count && !k7count && !k10count && !k14count && !k9count) {
			ErrorLogAdd("適正な曲が見つかりません 0\n");
			return -1;
		}
		else if (!k5count && !k7count && (k10count || k14count) && !k9count && ss->filter.ignoredp && !ss->filter.ignorepms) {
			ErrorLogAdd("適正な曲が見つかりません 1\n");
			return -1;
		}
		else if (!k5count && !k7count && !k10count && !k14count && k9count && !ss->filter.ignoredp && ss->filter.ignorepms) {
			ErrorLogAdd("適正な曲が見つかりません 2\n");
			return -1;
		}
		else if (!k5count && !k7count && (k10count || k14count || k9count) && ss->filter.ignoredp && ss->filter.ignorepms) {
			ErrorLogAdd("適正な曲が見つかりません 3\n");
			return -1;
		}
		else{
			int modeCycle = *mode;
			for (*mode = modeCycle + 1; *mode != modeCycle; *mode++) {
				if (*mode == 8) *mode = 0;
				if (ss->filter.ignorekeyall == 1 && *mode == 0) *mode = 1;
				if (ss->filter.ignorekeysingle == 1 && *mode == 1) *mode = 2;
				if (ss->filter.ignorekeydouble == 1 && *mode == 4) *mode = 5;
				if (ss->filter.ignoredp == 1 && (3 < *mode && *mode < 7)) *mode = 7;
				if (ss->filter.ignorepms == 1 && *mode == 7) *mode = 0;
				if (ss->filter.ignorekeyall == 1 && *mode == 0) *mode = 1;

				bool exit = 1;
				switch (*mode) {
				case 1:
					if (k5count == 0) {
						if (k7count == 0) exit = 0;
						break;
					}
					break;
				case 2:
					if (k7count == 0) exit = 0;
					break;
				case 3:
					if (k5count == 0) exit = 0;
					break;
				case 4:
					if (k10count == 0) {
						if (k14count == 0) exit = 0;
						break;
					}
					break;
				case 5:
					if (k14count == 0) exit = 0;
					break;
				case 6:
					if (k10count == 0) exit = 0;
					break;
				case 7:
					if (k9count == 0) exit = 0;
					break;
				default:
					break;
				}
				if (exit) break;
			}
			if (*mode == modeCycle) {
				ErrorLogAdd("適正な曲が見つかりません 4\n");
				return -1;
			}
			ErrorLogFmtAdd("鍵盤数を変更します %d→%d\n", modeCycle, *mode);
			return LoadFilteredBmsListFromDB(queryCpy, sql, ss, diffFilter, mode, sort, rivalID, flag);
		}
		return -1;
	}

	ss->unk5000 = 0;
	ss->prevListCount = count;
	if(isRandom == 0) {
		switch (sort) {
			case 0:
				for (int i = 0; i < ss->prevListCount; i++) {
					ss->prevList[i].folder = ss->prevList[i].filepath.getDirectory();
				}
				qsort(ss->prevList, ss->prevListCount, sizeof(SONGDATA), CMP_SongDataByDirectory);
				break;
			case 1:
				if ((EnabledInsane == 0 || ss->filter.disabledifficultyfilter == 0) && ss->isExLevel == 0) {
					qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByDifficulty);
				}
				else {
					qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByExLevel);
				}
				break;
			case 2:
				qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByTitle);
				break;
			case 3:
				qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByClear);
				break;
			case 4:
				if (isRival == 0) {
					qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByScore);
				}
				else {
					qsort(ss->prevList, count, sizeof(SONGDATA), CMP_SongDataByRivalScore);
				}
				break;
			default:
				break;
		}
	}

	ss->is_loading_bmslist = 1;
	ErrorLogFmtAdd("成功　曲数は%dです。\n",ss->prevListCount);
	return ss->prevListCount;
}

// LoadLR2CustomFolder
int LoadLR2CustomFolder(sqlite3 *sql, CONFIG_JUKEBOX *jb, CSTR scoreDBpath, char flag_starter, char flag_direct) {

	sqlite3 *scoreDB, *tagDB;
	char query[1024], query2[256];

	int folderAddCount = 0;
	EnabledInsane = 0;
	if (flag_starter) {
		SQL_Run("CREATE TABLE player(id TEXT primary key,hash TEXT,name TEXT,irid TEXT,irhash TEXT,playcount INTEGER,clear INTEGER,fail INTEGER,perfect INTEGER,great INTEGER,good INTEGER,bad INTEGER,poor INTEGER,playtime INTEGER,combo INTEGER,maxcombo INTEGER,grade_7 INTEGER,grade_5 INTEGER,grade_14 INTEGER,grade_10 INTEGER,grade_9 INTEGER,trial INTEGER,option INTEGER,systemversion INTEGER,gradeversion INTEGER, trialversion INTEGER,scorehash TEXT)", sql);
		SQL_Run("CREATE TABLE folder(title TEXT ,subtitle TEXT ,category TEXT,info_a TEXT,info_b TEXT,command TEXT,path TEXT primary key,type INTEGER,banner TEXT,parent TEXT,date INTEGER,max INTEGER,adddate INTEGER)", sql);
		SQL_Run("CREATE TABLE song(hash TEXT ,title TEXT ,subtitle TEXT ,genre TEXT,artist TEXT,subartist TEXT,tag TEXT ,path TEXT primary key ,type INTEGER,folder TEXT,stagefile TEXT,banner TEXT,backbmp TEXT,parent TEXT,level INTEGER,difficulty INTEGER,maxbpm INTEGER,minbpm INTEGER,mode INTEGER,judge INTEGER,longnote INTEGER,bga INTEGER,random INTEGER,date INTEGER,favorite INTEGER,txt INTEGER,karinotes INTEGER,adddate INTEGER)", sql);
		SQL_Run("CREATE TABLE course(id INTEGER primary key,title TEXT,line INTEGER,hash TEXT)", sql);
		SQL_Run("CREATE TABLE tag(hash TEXT primary key,title TEXT ,subtitle TEXT ,genre TEXT,artist TEXT,subartist TEXT,tag TEXT ,level INTEGER,difficulty INTEGER,mode INTEGER)", sql);
		SQL_Run("CREATE TABLE score(hash TEXT primary key,clear INTEGER,perfect INTEGER,great INTEGER,good INTEGER,bad INTEGER,poor INTEGER,totalnotes INTEGER,maxcombo INTEGER,minbp INTEGER,playcount INTEGER,clearcount INTEGER,failcount INTEGER,rank INTEGER,rate INTEGER,clear_db INTEGER,op_history INTEGER,scorehash TEXT,ghost TEXT,clear_sd INTEGER,clear_ex INTEGER,op_best INTEGER, rseed INTEGER, complete INTEGER)", sql);
	}
	else {
		if (flag_direct == 0) {
			// FIXME: out of bounds write if jb->numOfPath == jb->path.size(). Make jb->path a vector.
			//TODO : make define customfolderoption constant
			if (jb->customfolder & 1) {
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/RANDOM/").data();
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 2) {
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/favorite.lr2folder").data();
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 4) {
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/TOP10.lr2folder").data();
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 8) {
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/PLAYLEVEL/").data();
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 0x10) {
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/CLEAR/").data();
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 0x20) {
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/RANK/").data();
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 0x40) {
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/ignore.lr2folder").data();
				jb->numOfPath++;
				folderAddCount++;
			}
			if (jb->customfolder & 0x80) {
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/INSANE01/").data();
				jb->numOfPath++;
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/INSANE02/").data();
				jb->numOfPath++;
				folderAddCount+=2;
				EnabledInsane = 1;
			}
			jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/course1.lr2folder").data();
			jb->numOfPath++;
			jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/course2.lr2folder").data();
			jb->numOfPath++;
			jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/course3.lr2folder").data();
			jb->numOfPath++;
			folderAddCount += 3;
		}

		sqlite3_open(scoreDBpath,&scoreDB);
		sqlite3_open(fs::make_preferred("LR2files/Database/tag.db").data(), &tagDB);
		SQL_Run("CREATE TABLE folder(title TEXT ,subtitle TEXT ,category TEXT,info_a TEXT,info_b TEXT,command TEXT,path TEXT primary key,type INTEGER,banner TEXT,parent TEXT,date INTEGER,max INTEGER,adddate INTEGER)", sql);
		SQL_Run("CREATE TABLE song(hash TEXT ,title TEXT ,subtitle TEXT ,genre TEXT,artist TEXT,subartist TEXT,tag TEXT ,path TEXT primary key ,type INTEGER,folder TEXT,stagefile TEXT,banner TEXT,backbmp TEXT,parent TEXT,level INTEGER,difficulty INTEGER,maxbpm INTEGER,minbpm INTEGER,mode INTEGER,judge INTEGER,longnote INTEGER,bga INTEGER,random INTEGER,date INTEGER,favorite INTEGER,txt INTEGER,karinotes INTEGER,adddate INTEGER,exlevel INTEGER)", sql);
		SQL_Run("CREATE INDEX hashidx ON song (hash)", sql);
		SQL_Run("CREATE INDEX parentidx ON song (parent)", sql);
		SQL_Run("DROP TABLE course", sql);
		SQL_Run("CREATE TABLE nonstop(id INTEGER primary key,title TEXT,line INTEGER,hash TEXT,ir INTEGER)", sql);
		SQL_Run("CREATE TABLE expert(id INTEGER primary key,title TEXT,line INTEGER,hash TEXT,ir INTEGER)", sql);
		SQL_Run("CREATE TABLE grade(id INTEGER primary key,title TEXT,line INTEGER,hash TEXT,ir INTEGER)", sql);
		SQL_Run("CREATE TABLE tag(hash TEXT primary key,title TEXT ,subtitle TEXT ,genre TEXT,artist TEXT,subartist TEXT,tag TEXT ,level INTEGER,difficulty INTEGER,mode INTEGER,exlevel INTEGER)", tagDB);
		SQL_Run("CREATE TABLE score(hash TEXT primary key,clear INTEGER,perfect INTEGER,great INTEGER,good INTEGER,bad INTEGER,poor INTEGER,totalnotes INTEGER,maxcombo INTEGER,minbp INTEGER,playcount INTEGER,clearcount INTEGER,failcount INTEGER,rank INTEGER,rate INTEGER,clear_db INTEGER,op_history INTEGER,scorehash TEXT,ghost TEXT,clear_sd INTEGER,clear_ex INTEGER,op_best INTEGER, rseed INTEGER, complete INTEGER)", scoreDB);
		SQL_Run("CREATE INDEX hashidx ON score (hash)", scoreDB);
		sqlite3_close(scoreDB);
		sqlite3_close(tagDB);

		sqlite3_snprintf(256, query2, "ATTACH \'%q\' AS scoredb", scoreDBpath.body);
		if (SQL_Run(query2, sql) != 0) {
			ErrorLogAdd("スコアデータベースの接続に失敗しました。\n");
			return -1;
		}
		if (SQL_Run(fs::make_preferred("ATTACH \'LR2files/Database/tag.db\' AS tagdb").data(), sql) != 0) {
			ErrorLogAdd("タグとかデータベースの接続に失敗しました。\n");
			return -1;
		}
	}

	if (flag_direct == 0) {
		sqlite3_stmt *pStmt;

		SQL_Run("BEGIN", sql);
		ErrorLogAdd("エラーフォルダの検索を行います。\n");

		sqlite3_snprintf(1024, query, "SELECT * FROM folder WHERE parent = \'%s\'", AssignCRC32("ROOT").body);
		SQL_prepare(query, sql, &pStmt);
		while (sqlite3_step(pStmt) == 100) {
			CSTR path;
			path = SQL_GetColumn(6, pStmt);

			bool is_path_not_in_jukebox = 1;

			for (int i = 0; i < jb->numOfPath; i++) {
				if (path.isSame(&jb->path[i]))
					is_path_not_in_jukebox = 0;
			}

			if(is_path_not_in_jukebox) {
				ErrorLogFmtAdd("エラーフォルダを削除します。%s\n", path.body);
				sqlite3_snprintf(1024, query, "DELETE FROM folder WHERE path = \'%s\'", path.body);
				SQL_Run(query, sql);
			}
			else {
				ErrorLogFmtAdd("ジュークボックスに登録されています。%s\n", path.body);
			}
		}
		sqlite3_finalize(pStmt);

		if (flag_starter) {
			GetFolderDataFromPath(jb->path[0], sql);
		}
		else {
			for (int i = 0; i < jb->numOfPath; i++) {
				GetFolderDataFromPath(jb->path[i], sql);
			}
		}

		if (jb->autoreload == 2 || flag_starter) {
			ErrorLogAdd("フォルダ更新チェックを行います。\n");
			ReloadSongsByQuery("SELECT path,date FROM folder", sql, jb);
			ErrorLogAdd("ファイル更新チェックを行います。\n");
			ReloadSongsByQuery("SELECT path,date FROM song", sql, jb);
		}
		else {
			ErrorLogAdd("フォルダ更新チェック(ルートのみ)を行います。\n");
			sqlite3_snprintf(1024, query, "SELECT path,date FROM folder WHERE parent = \'%s\' OR date = 0", AssignCRC32("ROOT").body);
			ReloadSongsByQuery(query, sql, jb);
		}

		ErrorLogAdd("未定義#DIFFICULTYの自動設定を行います。\n");
		SetUndefinedDifficulty(sql);
		ErrorLogAdd("データベースチェックは終了しました。\n");

		if (flag_starter == 0) {
			SQL_Run(fs::make_preferred("DELETE FROM folder WHERE path LIKE \'LR2files/Rival/%\'").data(), sql);

			for (int i = 0; i < 20; i++) {
				if (jb->rival[i] < 1) break;
				cstrSprintf(&jb->path[jb->numOfPath], fs::make_preferred("LR2files/Rival/%d.lr2folder").data(), jb->rival[i]);
				GetFolderDataFromPath(jb->path[jb->numOfPath], sql);
				jb->numOfPath++;
				folderAddCount++;
			}

			SQL_Run(fs::make_preferred("DELETE FROM folder WHERE path=\'LR2files/CustomFolder/newsong.lr2folder\'").data(), sql);
			sqlite3_snprintf(1024, query, "SELECT * FROM song WHERE adddate > %d", GetNowUnixtime() - jb->titleflash * 3600);
			sqlite3_stmt *pStmt;
			SQL_prepare(query, sql, &pStmt);
			if (sqlite3_step(pStmt) == 100) {
				jb->path[jb->numOfPath] = fs::make_preferred("LR2files/CustomFolder/newsong.lr2folder").data();
				GetFolderDataFromPath(jb->path[jb->numOfPath], sql);
				jb->numOfPath++;
				folderAddCount++;
			}
			sqlite3_finalize(pStmt);
		}
		sqlite3_exec(sql, "COMMIT", 0, 0, 0);
		jb->numOfPath -= folderAddCount;
		for (int i = jb->numOfPath; i < std::size(jb->path); i++) {
			jb->path[i].fillzero();
		}
	}
	return 1;
}

int InitBMSMETA(BMSMETA *meta) {
	meta->title.fillzero();
	meta->subtitle.fillzero();
	meta->artist.fillzero();
	meta->subartist.fillzero();
	meta->hash.fillzero();
	meta->filename.fillzero();
	meta->stagefilepath.fillzero();
	meta->bannerpath.fillzero();
	meta->parentfolderpath.fillzero();
	meta->folderpath.fillzero();
	meta->tag.fillzero();
	meta->backBMPpath.fillzero();
	meta->filepath.fillzero();
	meta->selLevel = 0;
	meta->exlevel = 0;
	meta->notecount = 0;
	meta->maxbpm = 0;
	meta->minbpm = 0;
	meta->longnote = 0;
	meta->random = 0;
	meta->bga = 0;
	meta->hasTxt = 0;
	meta->keymode = 5;
	meta->difficulty = -1;
	meta->judge = 2;
	return 1;
}

int ParseBMSMETA(BMSMETA *meta, CSTR filepath, char flag) {
	FILE *pFile;
	float notes;
	int lnobj = -12345;
	bool flagIf;

	if (flag) {
		printfDx("Now Loading...\n%s\n",filepath.body);
		if (hBackImage > 0) {
			DrawGraph(0, 0, hBackImage, 0);
		}
		ScreenFlip();
		ClsDrawScreen();
		clsDx();
		ProcessMessage();
	}

	InitBMSMETA(meta);
	notes = 0.0;
	flagIf = 0;
	pFile = fopen(filepath.body, "r");
	if (pFile == NULL) return 0;

	CSTR dir(filepath.getDirectory()); // check this works as intended

	if (filepath.right(4).isSame(".pms")) meta->keymode = 9;

	CSTR buffer(102401);
	char* pBuffer = buffer.outstr();
	for (pBuffer = fgets(pBuffer, 102400, pFile); pBuffer; pBuffer = fgets(pBuffer, 102400, pFile)) {
		buffer = ansi2utf(pBuffer, 932).c_str();

		if (buffer.left(3).isSame("#IF") && buffer.left(5).isDiff("#IF 1")) {
			flagIf = 1;
		}
		else if (buffer.left(6).isSame("#ENDIF")) {
			flagIf = 0;
		}
		else if (buffer.left(1).isDiff("#") || flagIf) {
			pBuffer = buffer.atPos(0);
		}
		else {
			buffer.trimWhiteSpace();
			DealWhiteSpace(&buffer);
			if (GetDifficulty(&buffer, CSTR("#TITLE"), &meta->title, &meta->subtitle, &meta->difficulty)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#GENRE"), &meta->genre)) {
				CSTR none;
				GetDifficulty(&buffer, CSTR("#GENRE"), &none, &none, &meta->difficulty);
			}
			else if (GetStringBodyStr(&buffer, CSTR("#CATEGORY"), &meta->genre)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#SUBTITLE"), &meta->subtitle)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#ARTIST"), &meta->artist)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#SUBARTIST"), &meta->subartist)) {}
			else if (GetStringBodyInt(&buffer, CSTR("#PLAYLEVEL"), &meta->selLevel)) {}
			else if (GetStringBodyInt(&buffer, CSTR("#EXLEVEL"), &meta->exlevel)) {}
			else if (GetStringBodyInt(&buffer, CSTR("#MAXTRACKS"), &meta->selLevel)) {}
			else if (GetStringBodyInt(&buffer, CSTR("#DIFFICULTY"), &meta->difficulty)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#INFORMATION_A"), &meta->artist)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#INFORMATION_B"), &meta->subartist)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#TAG"), &meta->tag)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#COMMAND"), &meta->tag)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#STAGEFILE"), &meta->stagefilepath)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#BANNER"), &meta->bannerpath)) {}
			else if (GetStringBodyStr(&buffer, CSTR("#BACKBMP"), &meta->backBMPpath)) {}
			else if (buffer.left(13).isSame("#CUSTOMFOLDER")) {
				meta->judge = 2;
			}
			else if (buffer.left(7).isSame("#FP/DSC")) {
				meta->keymode = 9;
			}
			else if (GetStringBodyInt(&buffer, CSTR("#RANK"), &meta->judge)) {}
			else if (buffer.left(6).isSame("#LNOBJ")) {
				meta->longnote = 1;
				lnobj = Base36ToInt(*buffer.atPos(7), *buffer.atPos(8));
			}
			else if (buffer.left(4).isSame("#BMP")) {
				meta->bga = 1;
			}
			else if (buffer.left(5).isSame("#RAND")) {
				meta->random = 1;
			}
			else if (buffer.left(4).isSame("#BPM")) {
				int bpm;
				if (buffer.left(5).isSame("#BPM ")) {
					bpm = atol(buffer.right(buffer.length() - 5));
				}
				else {
					bpm = atol(buffer.right(buffer.length() - 7));
				}

				if (bpm > 0) {
					if (meta->maxbpm == 0) {
						meta->maxbpm = bpm;
						meta->minbpm = bpm;
					}
					else if (bpm > meta->maxbpm) {
						meta->maxbpm = bpm;
					}
					else if (bpm < meta->minbpm) {
						meta->minbpm = bpm;
					}
				}
			}
			else if( isdigit(*(buffer.atPos(1))) && isdigit(*(buffer.atPos(2))) && isdigit(*(buffer.atPos(3))) && isdigit(*(buffer.atPos(4))) && isdigit(*(buffer.atPos(5))) ){
				//TOFIX : There is no need to decrease variable data, still it does. it makes karinote include invisible and longnote(0.5 to 1)
				int data = atol(buffer.getSliced(4, 2));
				if (51 <= data && data <= 69) {
					data -= 40;
					meta->longnote = 1;
				}
				if (31 <= data && data <= 49) {
					data -= 20;
				}
				if (meta->keymode != 9) {
					if (data == 18 || data == 19 || data == 38 || data == 39 || data == 58 || data == 59) {
						if (meta->keymode == 5) meta->keymode = 7;
						if (meta->keymode == 10) meta->keymode = 14;
					}
					else if (data == 28 || data == 29 || data == 48 || data == 49 || data == 68 || data == 69) {
						meta->keymode = 14;
					}
					else if ((data > 20 && data < 30) || (data > 40 && data < 50) || (61 <= data && data < 70)) {
						if (meta->keymode == 5) meta->keymode = 10;
						else if(meta->keymode == 7) meta->keymode = 14;	
					}
				}
				
				if (data == 3) {
					int c = (buffer.length() - 7) / 2;
					for (int cur = 7, i = 0; i < c; cur += 2, i++) {
						int d = HEXcharToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1));
						if (d > 0) {
							if (meta->maxbpm == 0) {
								meta->maxbpm = d;
								meta->minbpm = d;
							}
							else if (d > meta->maxbpm) {
								meta->maxbpm = d;
							}
							else if (d < meta->minbpm) {
								meta->minbpm = d;
							}
						}
					}
				}
				else if (11 <= data && data <= 29) {
					int c = (buffer.length() - 7) / 2;
					for (int cur = 7, i = 0; i < c; cur += 2, i++) {
						if (Base36ToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1)) && Base36ToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1)) != lnobj ) {
							notes++;
						}
					}
				}
				else if (51 <= data && data <= 69) {
					int c = (buffer.length() - 7) / 2;
					for (int cur = 7, i = 0; i < c; cur += 2, i++) {
						if (Base36ToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1)) && Base36ToInt(*buffer.atPos(cur), *buffer.atPos(cur + 1)) != lnobj ) {
							notes += 0.5;
						}
					}
				}
				
			}
		}
	}

	fclose(pFile);
	if (meta->difficulty == -1) {
		CSTR sdsd(meta->filepath);
		sdsd.left(sdsd.length() - 4); //after all, not used
		if (sdsd.left(2).isSame("_a")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("_h")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("_m")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("_e")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("_b")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("_l")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_sa")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("_sh")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("_sm")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("_se")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_sb")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_sl")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_da")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("_dh")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("_dm")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("_de")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_db")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_dl")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("_ex")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("7a")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("7h")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("7m")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("7e")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("7b")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("7l")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("a7")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("h7")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("m7")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("e7")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("n7")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("l7")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("5a")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("5h")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("5m")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("5e")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("5b")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("5l")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("a5")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("h5")) meta->difficulty = 3;
		if (sdsd.left(2).isSame("m5")) meta->difficulty = 4;
		if (sdsd.left(2).isSame("e5")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("n5")) meta->difficulty = 1;
		if (sdsd.left(2).isSame("l5")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("a14")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("h14")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("m14")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("e14")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("b14")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("l14")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("a10")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("h10")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("m10")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("e10")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("b10")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("l10")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("14a")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("14h")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("14m")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("14e")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("14b")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("10l")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("10a")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("10h")) meta->difficulty = 3;
		if (sdsd.left(3).isSame("10m")) meta->difficulty = 4;
		if (sdsd.left(3).isSame("10e")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("10b")) meta->difficulty = 1;
		if (sdsd.left(3).isSame("10l")) meta->difficulty = 1;
	} // TOFIX: meta->filepath is not loaded yet, so above this is useless hahahaha
	//TOFIX: find difficulty at left???
	makeFileHash(filepath, meta->hash); //test : CSTR to char* as oBuf : possible
	meta->parentfolderpath = filepath.getParentDirectory();
	meta->filepath = filepath;
	meta->filename = filepath.getFilename();
	meta->folderpath = filepath.getDirectory();
	dir.add("*.txt");
	if (IsFileExist(dir)) meta->hasTxt = 1;
	meta->notecount = notes;
	return 1;
}

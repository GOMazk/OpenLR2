#pragma once

#include "structure.h"
#include "Engine.h"

// Checks which full-reload pass is running, so ReloadSongsByQuery can show the right
// loading-screen feedback without having to recognise the SQL text.
enum class ReloadProgress { None, FolderPass, SongPass };

SONGDATA * COPY_SONGDATA(SONGDATA *s1, SONGDATA *s2);
int InitSongData(SONGDATA * song);

int CMP_SongDataByExLevel(const void * p1, const void * p2);
int CMP_SongDataByTitle(const void * p1, const void * p2);
int CMP_SongDataByScore(const void * p1, const void * p2);
int CMP_SongDataByRivalScore(const void * p1, const void * p2);
int CMP_SongDataByDirectory(const void * p1, const void * p2);
int CMP_SongDataByPath(const void * p1, const void * p2);
int CMP_SongDataByDifficulty(const void * p1, const void * p2);
int CMP_SongDataByClear(const void * p1, const void * p2);

//into DB
int InitBMSMETA(BMSMETA * meta);
int ParseBMSMETA(BMSMETA * meta, CSTR filepath, char flag);

int SearchSongsFromPath(CSTR root, sqlite3 * sql, CSTR path); //into DB
int ReloadSongsByQuery(CSTR query, sqlite3 * sql, CONFIG_JUKEBOX * jb, ReloadProgress progress = ReloadProgress::None); //check reload condition and run
int GetFolderDataFromPath(CSTR path, sqlite3 * sql);
int LoadLR2CustomFolder(sqlite3 * sql, CONFIG_JUKEBOX * jb, CSTR scoreDBpath, char flag_starter, char flag_direct); //not coustomfolder only, but init DB and manage it

int SetUndefinedDifficulty(sqlite3 * sql);

//songdata & tag manage
int LoadBMSMETAFromDB(BMSMETA * meta, sqlite3 * sql); //load tag
int UpdateSongDataTag(SONGDATA * song, sqlite3 * sql);
int EditTag(SONGDATA * song, sqlite3 * sql); //tag reset
int UninstallSong(CSTR path, sqlite3 * sql);
int Rename(CSTR path, sqlite3 * sql);

//course manage
int WriteCourse(sqlite3 * sql, COURSESELECT course, SONGDATA * song, CSTR passmd5, int connection, int gauge);
int ChangeCourseTitle(sqlite3 * sql, CSTR title, int id, int coursetype);
int DeleteCourse(sqlite3 * sql, int id, int courseType);
int ChangeCourseID(sqlite3 * sql, int newID, int oldID, int type);
CSTR GetRandomSongHash(CSTR ioDefault, CSTR folder, int levelStd, int mode, sqlite3 *sql, int levelMax, int levelMin);
int WriteRandomCourse(sqlite3 * sql, COURSESELECT * course, SONGSELECT * ss, CONFIG_COURSE cfg, int key);

//select from DB
int GetSongDataFromPath(CSTR filepath, SONGDATA * song, sqlite3 * sql, SONGSELECT * ss);
int GetSongData(CSTR songMD5, SONGDATA * song, sqlite3 * sql, SONGSELECT * ss); //GetSongDataFromDB and score

int SearchCourseFromDB(sqlite3 * sql, SONGSELECT * ss, int keys, int multistagemode); //course version of GetSongData

int LoadBmsListFromDB(CSTR query, sqlite3 * sql, SONGSELECT * ss, int * difficulty, int * key, int sort, int count);

int LoadFolderDataFromDB(CSTR query, SONGDATA * song, sqlite3 * sql, int difficulty, int key, int sort, int maxCount, CONFIG_SELECT * cfg_select, char flag);

int LoadFilteredBmsListFromDB(CSTR query, sqlite3 * sql, SONGSELECT * ss, int * diffFilter, int * mode, uint sort, int rivalID, char flag);


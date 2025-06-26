#pragma once
#include "structure.h"

extern "C" {
#include "sqlite/sqlite3.h"
}
#include "tinyxml/tinyxml.h"
#include "md5.h" // for LR2startup.cpp

//typedef struct GLB {
//	sqlite3* sql;
//	game* g;
//};
extern glb_dbgame glb;

bool CheckScoreSaveConditon(game * g);
int CheckClearLampChallenge(game * g);

int SetTarget(game * g);
int ProcS_Keyconfig(game *g);
int LoopInRange(int min, int max, int add, int *num);

void ThreadProc_LoadBanner(void * param);
int SetBmsFilter(game * g, sqlite3 * sql);

uint SetObjectValue_Num(game * g, int op);


int StopSysSound(game * g);
int Print_ManiacOptions(game * g);
int GetSongCursor(game * g);
SONGDATA * COPY_SONGDATA(SONGDATA * s1, SONGDATA * s2);

int LoadFontForSongs(game * gs, char flag);
int ProcI_Decide(game * g);

int LogGraphPlayData(GRAPHDATA * grp, PLAYERSTATUS * pstat, int time, int endtime);
int LogGraphData(GRAPHDATAB * grp, int val, int time, int endtime);
int LogGraphPlayerDataToEnd(GRAPHDATA * grp, PLAYERSTATUS * pstat);
int CheckClear(PLAYERSTATUS * pstat, int gaugeType, char is2p);



int DrawHPgauge(game * g);

int ProcS_Result(game * g);
int Proc_Result(game * g, skstruct * sk, Timer * T);
int ProcI_Result(game * g);

int ProcI_Keyconfig(game *g);


int SelectSkin(SkinManage * sm, CSTR * obuf);
int SetFirstSkin(SkinManage * sm, SKINTYPE type, CSTR * skinName);
int SetFirstSkin_5k(SkinManage * sm, SKINTYPE type, CSTR * skinName);
int SetFirstSkin_10k(SkinManage * sm, SKINTYPE type, CSTR * skinName);
int SetFirstSkin_5kb(SkinManage * sm, SKINTYPE type, CSTR * skinName);
int SetFirstSkins(game * g);


int LUNARIS_GETBLOCKUP(void);
int LUNARIS_GETBLOCKDOWN(void);
int LUNARIS_GETBLOCKLEFT(void);
int LUNARIS_GETBLOCKRIGHT(void);
void LUNARIS_LAND();
int LUNARIS_MOVE(int direction);
int LUNARIS_ROTATE(char direction);
int LUNARIS_NEXTRANDOMROTATE(void);
int LUNARIS_CHECKLINE(void);
void LUNARIS_NEXT(void);
int DrawLunaris(game * g);
int LUNARIS_START(game * g);
int LUNARIS_JUDGE(game * g);
int ProcI_PO4Decide(game * g);

int RunMP3Encoder(ConfigStruct * cfg, CSTR wavPath, CSTR mp3Path, char deleteWav, char movie);
int Proc_Auto2avi(game * g, CSTR directory, CSTR filename);

int RecordBmsSound(game * g, CSTR oPath);

bool GetOptionFlag_dst(game *gs, int option);

int SetObjectStrings_SongSelect(game * g);

int CmdSearch(game * g, CSTR * cmd, sqlite3 * sql);

int ReleaseSysSound(game * g);
int ReadLR2SoundSet(game * g, CSTR filepath, char reFlag);

int SetObjectValue_Bargraph(game * g);
int SetPlayOption(game * g, sqlite3 * sql);
void CheckNewSong(glb_dbgame * glb);
void ThreadProc_RankingAutoUpdate(void * param);
int ProcI_Play(game * g);
int SkinSelect_SoundSet(game * g, CSTR filepath);
int ProcI_SkinSelect(game * g);

int MakeSkinPreview(game * g, skstruct * sk, SkinManage * sm);


int ProcI_Lunaris(game * g);
int ProcS_Select(game * g);

int Proc_Text(game *g, sqlite3 *sql, char flag);

int InitSelectBySearchResult(game * g, sqlite3 * sql);

int SaveResult(game * g, sqlite3 * sql);

int ProcS_CourseResult(game * g, sqlite3 * sql);

int ProcI_PO4Menu(game * g, sqlite3 * sql);


void ReactInput(game * g);

int ProcI_Select(game * g, sqlite3 * sql);

int ProcGame(game * g);

void ProcGameThread(game * g);

int ProcS_Play(game * g, sqlite3 * sql);


int ProcS_SkinSelect(game * g);

int ProcI_PO4Select(game * g, sqlite3 * sql);

int GetNowUnixtime(void);
int GetUnixtimeFromFiletime(FILETIME & filetime);
int GetFileUnixtime(CSTR str);
CSTR GetRandomFileOnDir(CSTR path, char fOnlyName);
bool CheckStringHead(CSTR* bigS, CSTR* head);
bool GetStringBodyStr(CSTR* str, CSTR head, CSTR* oBuf);
bool GetStringBodyInt(CSTR* str, CSTR haed, int * oBuf);
//
//
bool IsBmsFile(CSTR str);
bool IsMediaFile(CSTR str);
bool IsSndFile(CSTR str);
bool IsAviFile(CSTR str);
bool IsLR2Folder(CSTR str);
//
//
bool IsFileExist(CSTR path);
int IsFileChanged(unsigned int oldUnixtime, CSTR filepath, int * oNewtime);
int DealWhiteSpace(CSTR * str);
int SplitCSV(CSTR csvStr, CSVbuf * oBuf, const char * splitter);
int Base36ToInt(char ch1, char ch2);
int HEXcharToInt(char ch1, char ch2);
int RoundUp(double val);
CSTR AssignCRC32(CSTR str);
CSTR AutomationFactory();int CountDigit(int num);
int GetDigitNum(int num, int pos);
int FindAltImage(CSTR filename, CSTR dir, CSTR * oBuf);
int FindAltSound(CSTR filename, CSTR dir, CSTR * oBuf);
CSTR GetRandomFile(CSTR path, char fOnlyName);
CSTR GetRandomFileNoError(CSTR path, CSTR dir);

//CSTR : 43ad60 -
//see strclass.h

//LR2Startup : 43c060 -
//43c060
int ReadXml_Int(const char *level1, const char *level2, const char *level3, int initvalue, int *oBuf, TiXmlDocument *xmlData);
int ReadXml_Str(const char *level1, const char *level2, const char *level3, const CSTR initvalue, CSTR* oBuf, TiXmlDocument *xmlData);
int Read_JukeboxPath(CONFIG_JUKEBOX *box, TiXmlDocument *xml);
int ReadXml_Int_Multi(const char * level1, const char * level2, const char * level3, int * oBuf, TiXmlDocument * xmlData);
int ReadKeyConfig(game * game, const char * FilePath);
int ReadMIDI(game *gs, const char *filepath);
void WriteXML_Tab2Int(FILE * hFile, const char * tag, int value);
void WriteXML_Tab2Str(FILE * hFile, const char * tag, CSTR str);
int WriteConfigXml(game * g, const char * filename);
void WriteXML_KeyConfig(FILE * hFile, CONFIG_INPUT cfg_in, char * tag, int num);
int WriteKeyConfig(game * param_1, const char * filepath, int key);
int WriteMidiXml(game * g, const char * filename);
//43f6d0
int ReadSkinCustomize(SkinUser * sku, char * FilePath);
//441340
int WriteSkinCustomizeXml(SkinUser * sku, char * filepath);
//441d30
int ReadConfig(game* gs, const char* filepath);
//443550 _ need simplification
void MD5byte(char **iStr, uint len, char *oByte);
//443ff0
char* MD5str(char *iStr);

////DB interact
extern int EnabledInsane;
extern CRITICAL_SECTION DB_lock;
int DB_EnterCriticalSection();
int DB_LeaveCriticalSection();
bool ANSItoUTF8(LPCSTR str, char * oBuf, size_t * oSize);
bool UTF8toANSI(LPCSTR str, void * oBuf, size_t * oSize);
int SQL_Run(CSTR queryStr, sqlite3 * sql);
int SQL_prepare(CSTR queryStr, sqlite3 * sql, sqlite3_stmt ** ppStmt);
CSTR SQL_GetColumn(int i, sqlite3_stmt * pStmt);

int SwapBmsList(SONGSELECT * ss);
int InitBmsList(SONGSELECT * ss);
int InitSongData(SONGDATA * song);
int WriteGhostInDatabase(sqlite3 * sql, CSTR songMD5, PLAYSCORE * score);
int ReadGhostToScore(sqlite3 * sql, CSTR songMD5, PLAYSCORE * score);
CSTR ReadGhost(sqlite3 * sql, CSTR songMD5);
int LoadBMSMETAFromDB(BMSMETA * meta, sqlite3 * sql);
int SetUndefinedDifficulty(sqlite3 * sql);
int CMP_SongDataByExLevel(const void * p1, const void * p2);
int CMP_SongDataByTitle(const void * p1, const void * p2);
int CMP_SongDataByScore(const void * p1, const void * p2);
int CMP_SongDataByRivalScore(const void * p1, const void * p2);
int CMP_SongDataByDirectory(const void * p1, const void * p2);
int CMP_SongDataByPath(const void * p1, const void * p2);
CSTR MakeScoreHash(STATUS * stat, CSTR * passMD5, CSTR * songMD5);
bool isSameScoreHash(STATUS * stat, CSTR * passMD5, CSTR * songMD5, CSTR * besthash);
int UpdateScoreDB(CSTR hash, STATUS * stat, sqlite3 * sql, CSTR * passMD5);
int UpdateSongDataTag(SONGDATA * song, sqlite3 * sql);
CSTR MakePlayerStatHash(PLAYERSTATISTIC * ps);
int UpdatePlayerStat(PLAYERSTATISTIC * ps, sqlite3 * sql);
int SaveIRID(int IRID, CSTR ID);

int EditTag(SONGDATA * song, sqlite3 * sql);
int DeleteScoreFromDB(CSTR hash, sqlite3 * sql);
//
//
int LoadFolderDataFromDB(CSTR query, SONGDATA * song, sqlite3 * sql, int difficulty, int key, int sort, int maxCount, CONFIG_SELECT * cfg_select, char flag);
int UninstallSong(CSTR path, sqlite3 * sql);
int Rename(CSTR path, sqlite3 * sql);
//
//
int GetSongData(CSTR songMD5, SONGDATA * song, sqlite3 * sql, SONGSELECT * ss);
int WriteCourse(sqlite3 * sql, COURSESELECT course, SONGDATA * song, CSTR passmd5, int connection, int gauge);
int ChangeCourseTitle(sqlite3 * sql, CSTR title, int id, int coursetype);
int DeleteCourse(sqlite3 * sql, int id, int courseType);
//
//
int GetSongDataFromPath(CSTR filepath, SONGDATA * song, sqlite3 * sql, SONGSELECT * ss);
int ChangeCourseID(sqlite3 * sql, int newID, int oldID, int type);
//
int SearchSongsFromPath(CSTR root, sqlite3 * sql, CSTR path);
int ReloadSongsByQuery(CSTR query, sqlite3 * sql, CONFIG_JUKEBOX * jb);
int CMP_SongDataByDifficulty(const void * p1, const void * p2);
int CMP_SongDataByClear(const void * p1, const void * p2);
int SearchCourseFromDB(sqlite3 * sql, SONGSELECT * ss, int keys, int multistagemode);
//
int LoadBmsListFromDB(CSTR query, sqlite3 * sql, SONGSELECT * ss, int * difficulty, int * key, int sort, int count);

int ReadPlayerScore(CSTR id, CSTR pass, PLAYERSTATISTIC * pstat);

int WriteRandomCourse(sqlite3 * sql, COURSESELECT * course, SONGSELECT * ss, CONFIG_COURSE cfg, int key);

int GetFolderDataFromPath(CSTR path, sqlite3 * sql);

int LoadFilteredBmsListFromDB(CSTR query, sqlite3 * sql, SONGSELECT * ss, int * diffFilter, int * mode, uint sort, int rivalID, char flag);
int LoadLR2CustomFolder(sqlite3 * sql, CONFIG_JUKEBOX * jb, CSTR scoreDBpath, char flag_starter, char flag_direct);

////LR2graphic_draw : 49a770-
//ismultibyte?
double ChangeValueByTime(double val1, double val2, double time1, double time2, double timenow, int type);
DSTdraw SetDSTdrawByTime(DSTstruct dst, double time);
int GetSRCcycleNow(SRCstruct src, double time);
int LRDrawImg(int * grHandle, DSTdraw * dstD);
int GetTextGraphLength(CSTR * str, ImageFont * imF);
void LRDrawText(int * grHandle, DSTdraw * dstd, CSTR * str, ImageFont * imF);
void LRDrawTextInput(int * hFont, DSTdraw * dstd, int * hInput, ImageFont * imgfont);
int AddDrawingBufferT(DrawingBuf * drb, int grHandle, DSTdraw * dstd);
int sortDST(const void * a, const void * b);
int LRDraw(DrawingBuf * drBuf, TextStruct * txt, SONGSELECT * sSel, skstruct * sks, int targetBufNum, int x, int y);
int InitDrawingBuffer(DrawingBuf * drb);
int MouseOnDSTD(DSTdraw * dstd, int * x, int * y);
int MouseOnObject(DSTstruct * dst, Timer * T, int * x, int * y);
int SliderByTime(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int min, int max, int * value, inputStructure * input, int objectID);
int ButtonByInput(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, inputStructure * input, int * target, int min, int max, int panel);
int DrawBGA(int grHandle);
int AllocDrawingBuffer(DrawingBuf * drb);
int ReallocDrawingBuffer(DrawingBuf * drb);
double ByTime(double v1, double v2, double t1, double t2, double tO);
DSTdraw DSTDbyTime(DSTdraw * dstd1, DSTdraw * dstd2, double t1, double t2, double tO);
int AddDrawingBuffer_Text(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T);
int AddDrawingBuffer_TextXY(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int handle, int x, int y);
int AddDrawingBuffer(DrawingBuf * drb, int grHandle, DSTdraw * dstd);
int AddDrawingBuffer_BarGraph(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, double min, double max, double val);
int AddDrawingBuffer_Image(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T);
int AddDrawingBuffer_OnMouse(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, inputStructure * input, int panel);
int AddDrawingBuffer_LN(DrawingBuf * drb, SRCstruct * srcLs, SRCstruct * srcLe, SRCstruct * srcLb, DSTstruct * dst, Timer * T, float shiftX, float shiftY, float longY, int alpha, float sizeX, float sizeY);
int AddDrawingBuffer_PlayArea(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, float shiftX, float shiftY, int alpha, float sizeX, float sizeY, char flag);
int AddDrawingBuffer_Gauge(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int hp, char isSurvival);
int AddDrawingBuffer_BGA(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int grHandle, char flag);
int AddDrawingBuffer_Scratch(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int angle);
int AddDrawingBuffer_Image2(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T);
int AddDrawingBuffer_Object(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int x, int y);
int AddDrawingBuffer_ObjectAlpha(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int x, int y, int a);
int AddDrawingBuffer_EventLoading(DrawingBuf * drb, int grHandle, DSTstruct * dst, Timer * T, int x, int y);
int AddDrawingBuffer_Lunaris(DrawingBuf * drb, SRCstruct * src, DSTdraw * dstd, Timer * T);
int AddDrawingBuffer_Numbers(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int number, int x, int y);
int AddDrawingBuffer_Slider(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T);
int AddDrawingBuffer_JudgeCombo(DrawingBuf * drb, SRCstruct * jSrc, DSTstruct * jDst, SRCstruct * cSrc, DSTstruct * cDst, Timer * T, int combo, int optX, int optY);



////LR2graphic_load : 49e780-
int ReloadImage(CSTR filename, int * grHandle);
int InitSRC(SRCstruct * src);
int InitDST(DSTstruct * dst);
int InitDSTdraw(DSTdraw * dstd);
int ReadDST(DSTstruct * dst, CSVbuf * csv, int order);
int ReadSRC(SRCstruct * src, CSVbuf * csv, skstruct * sk);
bool CheckIndexRange(int index, int min, int max, int line, char * str);
int ReadSRC_BAR_TITLE(SRCstruct * src, CSVbuf * csv, skstruct * sk);
int ScreenCapture(uint iGrHandle);
int InitSkin(skstruct * sk, int p5, char font);
int InitImageFont(ImageFont * imgfont);
int ReadImageFont(CSTR filename, ImageFont * imgfont);
int LoadFontGraph(ImageFont * imgfont, int * fontNum);
int LoadFontCharGraph(ImageFont * imgfont, ushort vChar);
int LoadFontForText(ImageFont * imgfont, CSTR * str);
int FlipSide_Timer(int * n);
int ApplyFlipside(skstruct * sk);
int ClearSkinGraph(skstruct * sk);
int ExpandSkinObjectMax(SkinObject * so, int add);
int ReadSkin(skstruct * sk, CSTR FilePath, int unused, int skin_num, SkinUser * sku, char flag_skipFont);
int InitSkinData(SkinManage * skm);
int ExpandSkinMax(SkinManage * skm);
int LoadScene(skstruct * sk, CSTR skinfile, int p5, char font);
int ParseLR2SkinCustom(SkinManage * skm, CSTR filepath);
int MakeSkinList(SkinManage * skm, CSTR dir);

//PLAYSCORE : 4a8550 -

//?? : 4a9fd0 -

bool isVisibleNote(int ch);

int SetBackground(int hImage);
int InitBMSMETA(BMSMETA * meta);
int ParseBMSMETA(BMSMETA * meta, CSTR filepath, char flag);

int ExpandNoteBuffer(LaneStruct * lane, int addsize);
int CMP_NotesByBmsTiming(const void * p1, const void * p2);
int CMP_NotesByRealTiming(const void * p1, const void * p2);
int CMP_NotesByRealTimingOp(const void * p1, const void * p2);

int PlayerCheckAndSwap(gameplay * gp);




int SPtoDP(LaneStruct * lane, int baseNoteID, CHARTCONVERTER * cc);
int ParseBmsFile(gameplay * gp, CSTR filename, AUDIO * aud, ConfigStruct * cfg, BMSMETA * meta, int bgaFlag, int scratchSide);

//TIMER : 4b6710 - 
int SetBGATimer(Timer * T, double newTime);
int SetAbleClockCount(char val);
int SetClockFlag(Timer * T, char flag);
int NONE_004b6770(void);
int BGATimer(Timer * T);
int ResetTimeLapse(int timerID, Timer * T);
double GetTime(void);
double GetTimeWrap(void);
int InitTimer(Timer * T);
int CalcFPS(Timer * t);
double GetTimeLapse(uint timerID, Timer * T);
int SetTimeLapse(int timerID, Timer * T);

//STRING : 4b6c00 - 
int InitObjectString(TextStruct * txt);
int SetObjectString(uint num, CSTR string, CSTR * objectList);
int SetObjectStringInt(int at, int val, CSTR * arr);
CSTR GetStringFromArray(int num, CSTR * strings);
int DefineOptionStrNum(OptionString * arrOpStr);
int ReadOptionstr(OptionString * opStr, CSVbuf csv);
int ReadOptionstrFile(OptionString * arrOpStr, CSTR filepath);

//SOUND : 4b7b80 -
const char * GetFMODerror(int errCode);
int IsAltSoundExist(CSTR * filepath);
int ReleaseSound(AUDIO * aud, SOUNDDATA * sound);
int StopSound(AUDIO * aud, SOUNDDATA * sound);
int SetSoundVolume(AUDIO * aud, SOUNDDATA * sound, float volume);
int SoundGetCurrentTime(AUDIO * aud, SOUNDDATA * sound);
int EndSound(AUDIO * aud);
int SOUND_dxlibFx(SOUNDDATA sound, int v_master, int v_BGA, int pitch, double freq);
int SetFadeOut(AUDIO * aud, int fadetime);
int SetFadePreview(AUDIO * aud, int fadeintime, char flag);
int GetSoundBuffer(AUDIO * aud, double runtime, int volume);
void WriteSoundFile(AUDIO * aud, CSTR filename, uint size);
int SOUND_normalize(AUDIO * aud, SOUNDDATA * sound);
int RecordSound(AUDIO * aud, SOUNDDATA * sound, double time, double len);
int RecordFadeout(AUDIO * aud, double from, double length);
int LoadSound(AUDIO * aud, SOUNDDATA * sound, CSTR filepath, int loop, int disableDSP, int previewFlag);
int PlaySound(AUDIO * aud, SOUNDDATA * sound, FMOD_CHANNELGROUP *channelgroup, int stage);
int SOUND_FmodToDxlib(AUDIO * aud);
int ApplySoundFX(AUDIO * aud, int flag, char disable);
int InitFade(AUDIO * aud);
int SetVolumeByFade(AUDIO * aud);
int InitSound(AUDIO *aud, uint bufferLength, int numBuffer, char fDisable, int outputType, int driver);

int ParseRivalData(long ID);

//void RAWSOUND::ExpandBuffer(int newSize);



//INPUT : 4bd6a0 -
extern MIDI midi;
int InitInputStructure2(inputStructure * is);
void EndMIDIInput(void);
void GetMidiInput(dword msg, dword timestamp);
CSTR GetKeyIDname(int keyID);
int ConfigButtonToKeyID7(int buttonID);
int ConfigButtonToKeyID5(int buttonID);
int ConfigButtonToKeyID9(int buttonID);
int ConfigButtonFromKeyID7(int keyID);
int ConfigButtonFromKeyID5(int keyID);
int ConfigButtonFromKeyID9(int keyID);
int FindPressedKey(inputStructure * is);
int ResetPressCount(inputStructure * is);
int DetermineResultPlayDevice(inputStructure * is);
int CloseMIDI(void);
void ProcessInput(inputStructure *is, int interval);
void CALLBACK MIDIInProc(HMIDIIN hMidiIn, uint wMsg, dword dwInstance, dword dwParam1, dword dwParam2);
int WaitInput(inputStructure *is);
int InputToButton(inputStructure *is, CONFIG_INPUT *cfg_input, int player, int isReplay);
void InitMIDIInput(void);
int InitInputStructure(inputStructure *is);


int Mp3toWavF(FILE * iFile, FILE * oFile);
bool Mp3toWavP(char * iPath, char * oPath);

//REPLAY : 4c03c0 -
int MoveReplayFile(CSTR songMD5, CSTR localID);
int LoadReplayFileCourse(REPLAY * rp, CSTR songMD5, int stage, CSTR localID);
int LoadReplayFile(REPLAY * rp, CSTR songMD5, CSTR localID);
int SaveReplay(REPLAY * rp, CSTR songMD5, CSTR localID);
int AllocReplayBuffer(REPLAY * rp);
int ReleaseReplayBuffer(REPLAY * rp);
int AddReplayData(REPLAY * rp, int timing, char op, short value);
int AddReplayDataHeader(CONFIG_PLAY * cfg, REPLAY * rp, AUDIO * snd, gameplay * gp);
int InputToReplay(REPLAY * rp, inputStructure * is, int timing, int scratchSide);
int REPLAY_ApplyJudgeNote(gameplay * gp, Timer * T, game * g, uint judge, int player, int dp);
int REPLAY_ApplyJudgeMine(gameplay * gp, Timer * T, game * g, int dmg, int player, int dp);
int ReplayDataToInput(ReplayData * data, game * g, AUDIO * aud, gameplay * gp, inputStructure * is, Timer * T);
int SetReplayConfig(REPLAY * re, game * g, AUDIO * aud, gameplay * gp, inputStructure * in, Timer * T);
int ReplayToInput(REPLAY * rp, game * g, AUDIO * aud, gameplay * gp, inputStructure * is, Timer * T);



int InitGameplay(gameplay * gp, CONFIG_PLAY * cfg);

int LoadBmsResource(gameplay * gp, CSTR BMSfilepath, AUDIO * aud, ConfigStruct * cfg, BMSMETA * meta, char bga, char flip, char noVideo);
int InitGameplay_retry(gameplay * gp, AUDIO * snd, game * g);
double RealTimeToBMSTime(gameplay * gp, double time);

void MakeExtraChart(gameplay * gp, CHARTCONVERTER * cc);

void DPtoSP(gameplay * gp);
void PMStoSP(gameplay * gp);


int DrawNotes(game * g, skstruct * sk, Timer * T, CONFIG_PLAY * cfg);

int OpenWebRanking(CSTR songmd5);

int REC_CpyAVIStreamToFile(PAVIFILE pfile, PAVISTREAM pavi, int unused);

int CreateStream(CSTR filename, int framerate, COMPVARS * compvars, BITMAPINFOHEADER * lpbmi, PAVIFILE * pAVIFILE, PAVISTREAM * pAVIstream);

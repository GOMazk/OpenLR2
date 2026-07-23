#pragma once

#include <array>
#include <future>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "strclass.h"
#include "LR2_customir.h"

#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#include <vfw.h>
#endif // _WIN32

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 26495) // 'always initialize a member variable' - usually zeroed with memset
#endif

#define SINGLESLOTS 3844
#define SLOTS SINGLESLOTS*10
//36*36*5(6480) -> 62*62*10(38440)


struct sqlite3;
struct game;

typedef unsigned char   undefined;
typedef unsigned int    ImageBaseOffset32;
//typedef unsigned char    bool;
typedef unsigned char    byte;
typedef unsigned int    dword;
//float10
//typedef long long    longlong;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined3;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined6;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
//typedef short    wchar_t;
typedef unsigned short    word;

typedef long LONG;
typedef ulong DWORD;
typedef void * LPVOID;
typedef ushort WORD;
typedef uchar BYTE;

#ifndef _WIN32
using LPCSTR = const char*;
using HANDLE = void*;
using HWND = HANDLE;
#endif // _WIN32

typedef enum SKINTYPE {
	SKINTYPE_7KEYS = 0,
	SKINTYPE_5KEYS = 1,
	SKINTYPE_14KEYS = 2,
	SKINTYPE_10KEYS = 3,
	SKINTYPE_9KEYS = 4,
	SKINTYPE_SELECT = 5,
	SKINTYPE_DECIDE = 6,
	SKINTYPE_RESULT = 7,
	SKINTYPE_KEYCONFIG = 8,
	SKINTYPE_SKINSELECT = 9,
	SKINTYPE_SOUNDSET = 10,
	SKINTYPE_THEME = 11,
	SKINTYPE_7KEYSBATTLE = 12, //.RED skin manual doesn't match with LR2beta3 100201 -
	SKINTYPE_5KEYSBATTLE = 13,
	SKINTYPE_9KEYSBATTLE = 14,
	SKINTYPE_COURSERESULT = 15,
	SKINTYPE_OPENING = 16,
	SKINTYPE_MODESELECT = 17,
	SKINTYPE_MODEDECIDE = 18,
	SKINTYPE_COURSESELECT = 19,
	SKINTYPE_COURSEEDIT = 20
} SKINTYPE;

typedef enum GRHANDLETYPE {
	GRHTYPE_STAGE = 100,
	GRHTYPE_BACKBMP = 101,
	GRHTYPE_BANNER = 102,

	GRHTYPE_PREVIEW = 105,
	GRHTYPE_PREVIEW_106 = 106,

	GRHTYPE_BLACK = 110,
	GRHTYPE_WHITE = 111,
}GRHANDLETYPE;

enum CHANNEL {
    CHANNEL_BGM = 1,
    CHANNEL_MEASURE_LENGTH,
    CHANNEL_BPM,
    CHANNEL_BGABASE,
    CHANNEL_EXCHR,
    CHANNEL_BGAPOOR,
    CHANNEL_BGALAYER,
    CHANNEL_EXBPM,
    CHANNEL_STOP,
    //CHANNEL_BGALAYER2 = 0x0A,         //from nanasigroove
    //CHANNEL_BGABASE_OPACITY = 0x0B,   //from nanasigroove
    //CHANNEL_BGALAYER_OPACITY = 0x0C,  //from nanasigroove
    //CHANNEL_BGALAYER2_OPACITY = 0x0D, //from nanasigroove
    //CHANNEL_BGAPOOR_OPACITY = 0x0E,   //from nanasigroove

    //channels are converted to handle easily on LR2.
    //Ex) 1P SC is 10, not 16, and freezone(x7) is not supported
    CHANNEL_1P_NOTE_SC = 10,
    CHANNEL_1P_NOTE_1,
    CHANNEL_1P_NOTE_2,
    CHANNEL_1P_NOTE_3,
    CHANNEL_1P_NOTE_4,
    CHANNEL_1P_NOTE_5,
    CHANNEL_1P_NOTE_6,
    CHANNEL_1P_NOTE_7,
    CHANNEL_1P_NOTE_END = 19,//not actual channel
    CHANNEL_2P_NOTE_SC = 20,
    CHANNEL_2P_NOTE_1,
    CHANNEL_2P_NOTE_2,
    CHANNEL_2P_NOTE_3,
    CHANNEL_2P_NOTE_4,
    CHANNEL_2P_NOTE_5,
    CHANNEL_2P_NOTE_6,
    CHANNEL_2P_NOTE_7,
    CHANNEL_2P_NOTE_END = 29,//not actual channel
    CHANNEL_1P_HIDDEN_SC = 30,
    CHANNEL_1P_HIDDEN_1,
    CHANNEL_1P_HIDDEN_2,
    CHANNEL_1P_HIDDEN_3,
    CHANNEL_1P_HIDDEN_4,
    CHANNEL_1P_HIDDEN_5,
    CHANNEL_1P_HIDDEN_6,
    CHANNEL_1P_HIDDEN_7,
    CHANNEL_1P_HIDDEN_END = 39,//not actual channel
    CHANNEL_2P_HIDDEN_SC = 40,
    CHANNEL_2P_HIDDEN_1,
    CHANNEL_2P_HIDDEN_2,
    CHANNEL_2P_HIDDEN_3,
    CHANNEL_2P_HIDDEN_4,
    CHANNEL_2P_HIDDEN_5,
    CHANNEL_2P_HIDDEN_6,
    CHANNEL_2P_HIDDEN_7,
    CHANNEL_2P_HIDDEN_END = 49,//not actual channel
    CHANNEL_1P_LN_SC= 50,
    CHANNEL_1P_LN_1,
    CHANNEL_1P_LN_2,
    CHANNEL_1P_LN_3,
    CHANNEL_1P_LN_4,
    CHANNEL_1P_LN_5,
    CHANNEL_1P_LN_6,
    CHANNEL_1P_LN_7,
    CHANNEL_1P_LN_END = 59,//not actual channel
    CHANNEL_2P_LN_SC = 60,
    CHANNEL_2P_LN_1,
    CHANNEL_2P_LN_2,
    CHANNEL_2P_LN_3,
    CHANNEL_2P_LN_4,
    CHANNEL_2P_LN_5,
    CHANNEL_2P_LN_6,
    CHANNEL_2P_LN_7,
    CHANNEL_2P_LN_END = 69,//not actual channel

    CHANNEL_1P_MINE_SC = 130,
    CHANNEL_1P_MINE_1,
    CHANNEL_1P_MINE_2,
    CHANNEL_1P_MINE_3,
    CHANNEL_1P_MINE_4,
    CHANNEL_1P_MINE_5,
    CHANNEL_1P_MINE_6,
    CHANNEL_1P_MINE_7,
    CHANNEL_1P_MINE_END = 139,

    CHANNEL_2P_MINE_SC = 140,
    CHANNEL_2P_MINE_1,
    CHANNEL_2P_MINE_2,
    CHANNEL_2P_MINE_3,
    CHANNEL_2P_MINE_4,
    CHANNEL_2P_MINE_5,
    CHANNEL_2P_MINE_6,
    CHANNEL_2P_MINE_7,
    CHANNEL_2P_MINE_END = 149,

    //popn
    CHANNEL_PMS_NOTE_1 = 11,    //DSC: L-SC
    CHANNEL_PMS_NOTE_2,
    CHANNEL_PMS_NOTE_3,
    CHANNEL_PMS_NOTE_4,
    CHANNEL_PMS_NOTE_5,
    CHANNEL_PMS_NOTE_6,
    CHANNEL_PMS_NOTE_7,
    CHANNEL_PMS_NOTE_8,
    CHANNEL_PMS_NOTE_9,         //DSC: R-SC
    CHANNEL_PMS_HIDDEN_1 = 31,
    CHANNEL_PMS_HIDDEN_2,
    CHANNEL_PMS_HIDDEN_3,
    CHANNEL_PMS_HIDDEN_4,
    CHANNEL_PMS_HIDDEN_5,
    CHANNEL_PMS_HIDDEN_6,
    CHANNEL_PMS_HIDDEN_7,
    CHANNEL_PMS_HIDDEN_8,
    CHANNEL_PMS_HIDDEN_9,
    CHANNEL_PMS_LN_1 = 51,
    CHANNEL_PMS_LN_2,
    CHANNEL_PMS_LN_3,
    CHANNEL_PMS_LN_4,
    CHANNEL_PMS_LN_5,
    CHANNEL_PMS_LN_6,
    CHANNEL_PMS_LN_7,
    CHANNEL_PMS_LN_8,
    CHANNEL_PMS_LN_9,
    CHANNEL_PMS_MINE_1 = 131,
    CHANNEL_PMS_MINE_2,
    CHANNEL_PMS_MINE_3,
    CHANNEL_PMS_MINE_4,
    CHANNEL_PMS_MINE_5,
    CHANNEL_PMS_MINE_6,
    CHANNEL_PMS_MINE_7,
    CHANNEL_PMS_MINE_8,
    CHANNEL_PMS_MINE_9,

    //CHANNEL_BGM_VOL = 97,         //from forgetalia
    //CHANNEL_KEY_VOL = 98,         //from forgetalia
    //CHANNEL_TEXT = 99,            //from pomu
    //CHANNEL_EXRANK = 100,         //from nanasi
    //CHANNEL_BGABASE_ARGB = 101,   //from nanasi
    //CHANNEL_BGALAYER_ARGB  102,   //from nanasi
    //CHANNEL_BGALAYER2_ARGB 103,   //from nanasi
    //CHANNEL_BGAPOOR_ARGB = 104,   //from nanasi
    //CHANNEL_BGABOUND = 105,       //from nanasi
    //CHANNEL_OPTION = 106,         //from nanasi

    CHANNEL_SCROLL = 2888,          //SC:out of base16 //from bemuse
    //CHANNEL_SPEED = 2889,          //SP:out of base16 //from bemuse
};

enum OPTION {
	OPTION_HSFIX_OFF = 0,
	OPTION_HSFIX_MAXBPM,
	OPTION_HSFIX_MINBPM,
	OPTION_HSFIX_AVERAGE,
	OPTION_HSFIX_CONSTANT,
	OPTION_HSFIX_MAINBPM,
	OPTION_HSFIX_END = OPTION_HSFIX_MAINBPM,

	OPTION_BATTLE_OFF = 0,
	OPTION_BATTLE_BATTLE,
	OPTION_BATTLE_DBATTLE,
	OPTION_BATTLE_SP2DP, //not only sp-to-dp, dp-to-sp and pms-to-sp
	OPTION_BATTLE_GBATTLE,
	OPTION_BATTLE_END = OPTION_BATTLE_GBATTLE,

	OPTION_GAUGE_GROOVE = 0,
	OPTION_GAUGE_HARD,
	OPTION_GAUGE_DEATH,
	OPTION_GAUGE_EASY,
	OPTION_GAUGE_PATTACK,
	OPTION_GAUGE_GATTACK,
	OPTION_GAUGE_END = OPTION_GAUGE_GATTACK,

	OPTION_RANDOM_OFF = 0,
	OPTION_RANDOM_MIRROR,
	OPTION_RANDOM_RANDOM,
	OPTION_RANDOM_SRANDOM,
	OPTION_RANDOM_SCATTER,
	OPTION_RANDOM_CONVERGE,
	OPTION_RANDOM_END = OPTION_RANDOM_CONVERGE,
};

enum PLAYER {
	PLAYER_1 = 0,
	PLAYER_2,
	PLAYER_MAX
};

struct BMSMETA {
	CSTR hash;
	CSTR title;
	CSTR subtitle;
	CSTR artist;
	CSTR subartist;
	CSTR genre;
	CSTR filepath;
	CSTR filename;
	CSTR stagefilepath;
	CSTR bannerpath;
	CSTR backBMPpath;
	CSTR parentfolderpath;
	CSTR folderpath;
	CSTR tag;
	int notecount;
	int maxbpm;
	int minbpm;
	int keymode;
	int longnote;
	int selLevel;
	int exlevel;
	int judge;
	int difficulty;
	int random;
	int bga;
	bool hasTxt;//unused
};

struct BPMtiming {
	double converted;
	double render_converted;
	double BPM;
	double realtime;
};

struct CONFIG_COURSE {
	int defaultgauge{};
	int defaultconnection{};
	int maxbpm{};
	int minbpm{};
	int bpmrange{};
	int optimumlevel_5{};
	int optimumlevel_7{};
	int optimumlevel_9{};
	int optimumlevel_10{};
	int optimumlevel_14{};
	int maxlevel{};
	int minlevel{};
	int stage{};
};

struct CONFIG_INPUT {
	int buttonMap[40][16]{};
	int sys_inputinterval{};
	int midi_control[40]{};
};

struct CONFIG_JUKEBOX {
	CSTR path[1000];
	int numOfPath{};
	int autoreload{};
	int customfolder{};
	CSTR newsongfolder;
	int titleflash{};
	int rival[20]{};
};

struct CONFIG_NETWORK {
	int lr1ir{};//unused
	CSTR lr1id;//unused
	CSTR lr1pass;//unused
	int lr2ir{};
	CSTR mail;//unused
	bool isAutoUpdate{};
	bool getRival{};
	CSTR displayIr;
};

struct CONFIG_PLAY {
	int hiSpeed[PLAYER_MAX]{};
	int gaugeType[PLAYER_MAX]{}; /* 0:groove 1:survival 2:death 3:easy 4:pattack 5:gattack */
	int random[PLAYER_MAX]{}; // 1:mirror 2:random 3:s-random 4:scatter 5:converge
	int m_HIDSUD[PLAYER_MAX]{};
	int lanecoverv[PLAYER_MAX]{};
	int lanecover[PLAYER_MAX]{};
	bool autokey{};
	int assist[PLAYER_MAX]{};
	bool dpFlip{};
	int hsfix{}; // 1:max 2:min 3:average 4:constant 5:main
	int battle{}; // 1:battle 2:d-battle 3:sp-to-dp 4:g-battle
	int autojudge{};
	int judgetiming{};
	int bga{};
	int bgasize{};
	int poorbga{};
	bool scoreGraph{};
	int play_ghost{};
	int target_percent{};
	int p1_target{};
	int randSC[PLAYER_MAX]{}; // include scratch to random by impossible command
	int randFix[PLAYER_MAX]{}; // fix a lane by impossible command
	int replay{};
	int hsmargin{};
	int hsmax{};
	int hsmin{};
	int shuttermargin{};
	int basespeed{};
	int m_softlanding{};
	int m_addmine{};
	int m_addlong{};
	int m_earthquake{};
	int m_tornado{};
	int m_superloop{};
	int m_gambol{};
	int m_char{};
	int m_heartbeat{};
	int m_loudness{};
	int m_addnote{};
	int m_nabeatsu{};
	int m_accel{};
	int m_sincurve{};
	int m_wave{};
	int m_spiral{};
	int m_sidejump{};
	bool m_isExtra{};//TOFIX?: Not bug, but whynot use m_extra=0 as OFF like m_gambol
	int m_extra{};
	bool m_isLunaris{};
	bool m_gas{};
	int gomiscore{};//unused
	bool disableCurSpeedChange{};
	bool disableLeftClickExit{};
};

struct CONFIG_PLAYER {
	CSTR id;
	CSTR pass;
	CSTR passMD5;
	CSTR lrid;
	CSTR irpass;
	CSTR irpassMD5;
	CSTR name;
};

struct CONFIG_SELECT {
	int difficulty{}; // 0:ALL,1:EASY,2:STANDARD,3:HARD,4:EXPERT,5:ULTIMATE
	int sort{}; // 0:OFF,1:TITLE,2:LEVEL,3:FOLDER,4:CLEAR,5:RANK
	int key{}; // 0:ALL KEYS,1:SINGLE,2:7KEYS,3:5KEYS,4:DOUBLE,5:14KEYS,6:10KEYS,7:9KEYS
	int searchmax{};
	int randomclose{};//unused
	int speedfirst{};
	int speednext{};
	bool control{};//force 9key?
	bool buttonselect{};
	int folderlamp{};
	bool difficultyChangeType{};
	bool ignoreKeyAll{};
	bool ignoreKeySingle{};
	bool ignoreKeyDouble{};
	bool ignoreDP{};
	bool ignorePMS{};
	bool ignoreDifficultyAll{};
	bool ignore5key{};
	int titleflash{};
	int levelbarflash_7{};
	int levelbarflash_5{};
	int levelbarflash_9{};
	bool disableDifficultyFilter{};
	bool isPreview{};
	bool disableSubtitle{};
};

struct CONFIG_SKIN {
	bool disableImageFont{};
	CSTR fontname;
	CSTR skinFilePath[30];
};

struct CONFIG_SOUND {
	bool disableDSP{};
	int bufferlength{};
	int numbuffers{};
	int output{};
	int driver{};
	bool disableFmod{};
	int volumemaster{};
	int volumekey{};
	int volumebgm{};
	int volumeflag{};
	int eqflag{};
	int eqp0{};
	int eqp1{};
	int eqp2{};
	int eqp3{};
	int eqp4{};
	int eqp5{};
	int eqp6{};
	int pitchflag{};
	int pitchp{};
	int pitchtype{};
	int fxflag_0{};
	int fxflag_1{};
	int fxflag_2{};
	int fxtype_0{};
	int fxtype_1{};
	int fxtype_2{};
	int fxtarget_0{};
	int fxtarget_1{};
	int fxtarget_2{};
	int fxp1_0{};
	int fxp1_1{};
	int fxp1_2{};
	int fxp2_0{};
	int fxp2_1{};
	int fxp2_2{};
};

struct CONFIG_SYSTEM {
	int fullscreenfilter{};
	int screenmode{};
	int vsync{};
	int directdraw{};
	int mainsleep{};
	int bmssleep{};
	int screenexrate{};
	int highcolor{};
	int disablesystemkey{};
	int outputlog{};
	int eventmode{};
	int thread{};
	int disableskinpreview{};
	int hptimer{};
	int isablebmsthread{};
	int disablefolderthread{};
	int language{};
	int windowsize_x{};
	int windowsize_y{};
	int maindisplay{};
	int softwarerendering{};
	int resolution{};   // 0=SD 640x480, 1=HD 1280x720, 2=FHD 1920x1080
	unsigned int coreCount = 0;
	bool fullscreenfitstretch{};
};

struct CONFIG_TOOLS {
	CSTR mp3enc_body{};
	CSTR mp3enc_option_normal{};
	CSTR mp3enc_option_movie{};
	int movie_audio{};
	int mp3_volume{};
	int movie_framerate{};
	CSTR oggenc_body{};
	CSTR oggenc_option{};
	CSTR oggdec_body{};
	CSTR oggdec_option{};
	CSTR bmse_body{};
	CSTR bmse_option{};
	int autowavtoogg{};
	int autobmptopng{};
	int autofumensearch{};
};

struct ConfigStruct {
	struct CONFIG_PLAY play{};
	struct CONFIG_SYSTEM system{};
	struct CONFIG_SELECT select{};
	struct CONFIG_INPUT input{};
	struct CONFIG_SOUND sound{};
	struct CONFIG_SKIN skin{};
	struct CONFIG_JUKEBOX jukebox{};
	struct CONFIG_PLAYER player{};
	struct CONFIG_NETWORK network{};
	struct CONFIG_COURSE course{};
	struct CONFIG_TOOLS tools{};
};

struct STATUS {
	int stat_pgreat{};
	int stat_great{};
	int stat_good{};
	int stat_bad{};
	int stat_poor{};
	int clear{};
	int stat_score{};
	int stat_exscore{};
	int rank{};
	int rate{};
	int minbp{};
	int stat_maxcombo{};
	int playcount{};
	int clearcount{};
	int failcount{};
	int clear_db{};
	int clear_sd{};
	int clear_ex{};
	int total_notes{};
	int op_history{};
	int op_best{};
	int IRranking{};
	int IRclearRate{};
	int IRplayercount{};
	char IRthreadEnd{};
	int complete{};
	int rseed{};
};

struct SONGDATA { /* 712bytes */
	CSTR title;
	CSTR subtitle;
	CSTR genre;
	CSTR artist;
	CSTR subartist;
	CSTR filepath;
	CSTR fulltitle; /* tilte+subtitle */
	CSTR tag;
	CSTR hash;
	CSTR folder;
	CSTR stagefile;
	CSTR banner;
	CSTR backBMP;
	bool isStagefile{};
	bool isBanner{};
	int isBackBMP{};
	uint difficulty{};
	int level{};
	int exlevel{};
	int keymode{};
	int folderType{}; /* 0,5:song 1:folder 2:levelfolder 3,4,6:folder 7:newcourse 8:course 9:randomcourse */
	int maxBPM{};
	int minBPM{};
	int bga{};
	int txt{};
	int longnote{};
	int random{};
	int judge{};
	int replayExist{};
	int favorite{};
	int adddate{};
	int difficultyLevel[5]{}; /* 0:beginner 1:nomal 2:hyper 3:another 4:insane */
	int difficultyExist[5]{}; /* 0:beginner 1:nomal 2:hyper 3:another 4:insane */
	int difficultyLevelBarLamp[5]{}; /* 0:beginner 1:nomal 2:hyper 3:another 4:insane */
	int difficultyCount{}; /* 0:beginner 1:nomal 2:hyper 3:another 4:insane */
	CSTR courseTitle[10];
	CSTR courseSubtitle[10];
	CSTR courseHash[10];
	int courseLevel[10]{};
	int courseStageDifficulty[10]{};
	int courseTotalnote[10]{};
	int courseID{};
	int courseStageCount{};
	int coursePlayable{}; /* have all songs */
	char courseReadOnly{};
	int courseKeys[10]{};
	int courseType{}; /* 0:expert 1:nonstop 2:class */
	int courseIR{}; /* IR flag */
	int grHandle{};

	// If present, this will contain score for the song imported from the IR with best properties mixed in with 'mybest'.
	std::optional<STATUS> myIRbest;
	struct STATUS mybest;
	struct STATUS rivalRecord;
};

struct COURSESELECT {
	struct SONGDATA data[10]; /* //size 0x1bfc struct? */
	int count;
	CSTR name;
	int id;
	char isCourseCreated;
	char isMakingCourse;
	char isDeletingCourse;
	char isChangingTitle;
	int return_Topbar;
	CSTR return_query;
	CSTR return_filepath;
	CSTR return_searchText;
	int return_isFolder;
	int return_rivalID;
	int type; /* 0:expert 1:nonstop 2:grade */
};

struct CSVbuf {
	int val[30]{};
	CSTR str[30]{};
};
struct DSTdraw { /* 80bytes,4*0x14 */
	float x{0};
	float y{0};
	float w{0};
	float h{0};
	int sortID{0};
	int time{-1};
	int acc{0};
	int blend{0};
	int filter{0};
	int a{0};
	int r{0};
	int g{0};
	int b{0};
	float angle{0};
	int center{0};
	int grHandle{-1};
	int fontHandle{-1};
	int subHandle{-1};
	int align{0};
	char isDrawBackbox{0};
};

struct DrawingBuf {
	struct DSTdraw * dstd;
	int max;
	int count;
	int isHidSud[20]; /* 40680b/49b4a5 */
	int top[20]; /* sud */
	int bottom[20]; /* hid */
	bool disableImageFont;
	char isDisabled;
	char unkFE;
	char unkFF;
};

struct DSTstruct { /* 44bytes.4*0x0b */
	int n; /* (NULL) on file */
	int opt1; /* dst_option */
	int opt2; /* and dst_option */
	int opt3; /* and dst_option */
	int opt4; /* scratch */
	int opt5;
	int timer;
	struct DSTdraw * draw;
	int dataSize;
	int loop;
	int dstCount;
};

inline DSTstruct CloneDSTstruct(const DSTstruct* src) {
	DSTstruct clone = *src;
	if (src->dstCount > 0 && src->draw) {
		clone.draw = (DSTdraw*)malloc(src->dstCount * sizeof(DSTdraw));
		memcpy(clone.draw, src->draw, src->dstCount * sizeof(DSTdraw));
	} else {
		clone.draw = nullptr;
	}
	return clone;
}

inline void FreeDSTstructClone(DSTstruct* clone) {
	if (clone && clone->draw) {
		free(clone->draw);
		clone->draw = nullptr;
	}
}

struct FontChar {
	int srcX{};
	int srcY{};
	int width{};
	int height{};
	int ImageNum{};
	int grHandle = -1;
};

struct FontImage {
	char filename[32]{};
	int grHandle = -1;
};

struct RECORDING {
	double framerate{};
	int writeSamplePos{};
	int curFrame{};
	int recMode{}; /* 1:auto2avi 2:replay2avi 3:bga2avi 4:movie */

#ifdef WIN32
	HDC srcHDC{}; /* struct_entry */
	int bitdepth{};
	CSTR filename{};
	uint framelen{};
	COMPVARS compvars{}; /* from vfw.h */
	BITMAPINFOHEADER bmiHeader{};
	PAVISTREAM pAVIstream{}; /* from vfw.h */
	PAVIFILE pAVIFILE{}; /* from vfw.h */
	void * buf{};
	HWND hwnd{};
	HDC dstHDC{};
	HBITMAP hBIT{};
	HGDIOBJ hGDI{};
#endif

	RECORDING() = default;
	bool RefreshCurFrame();
	int GetCurTime();
	int CpyScreenToAVI();
	bool Release();
	bool PrepareAVIRecord(double framerate, int bit, CSTR filename, uint frameLen, HWND hwnd);
	int InsertAudioToMovie(CSTR pathAudio, bool deleteFlag);

};

struct OptionString {
	CSTR str[10];
};

struct LaneStruct {
    struct NoteStruct * notes; /* array pointer */
    int count;
    int size;
    int note_count;
    int draw_count; //count of no need for draw(passed/processed)
    int noteVal;
    int autoplay;
};

struct RANKING {
	struct RANKINGPLAYER * ranking;
	int rankingCount;
	int rankingMax;
	int clearPlayers[6]; // 5fc 4hd 3gr 2ez 1fail 0no?
	int totalPlaycount;
	int rankingCursor;
	CSTR lastupdate;
	int target_ID;
	int target_number;
	char showRanking; /* char */
	int myRanking;
	int myID;
	int rivalRanking;
	int rivalID;

	void ExpandRankingBuffer(int add);
	void Init();
	int ParseXML(const char* path);
	RANKING();
};

struct RAWSOUND {
	int channels{};
	int samples{};
	int bits{};
	int length{};
	int dataSize{};
	byte* data{};

	void ExpandBuffer(int minSize);
	void MakeBitDepth16(void);
	void MakeStereo(void);
	void MakeSampleRate44100(void);
};

struct SOUNDDATA {
	struct RAWSOUND raw{};
	struct FMOD_SOUND * fmod_sound{};
	struct FMOD_CHANNEL * fmod_channel{};
	CSTR filename{};
	unsigned int length{};
	int soundHandle{};
	bool load{};
	bool loop{};
	bool streaming = false;
};

struct inputStructure {
	unsigned char inputID[2048]{}; /* 0:keybd 0x100:joypad 0x200:MIDI */
	int inputTime[2048]{}; /* 0:keybd 0x100:joypad 0x200:MIDI */
	unsigned char p1_buttonInput[20]{}; /* byte100 */
	unsigned char p2_buttonInput[20]{};
	unsigned char otherbuttons[60]{};
	char mouse_buttonL{}; /* 0:not pressed 1:just pressed 2:pressed 3:released */
	char mouse_buttonR{};
	char mouse_buttonW{};
	char mouse_button4{};
	int mouse_oldX{};
	int mouse_oldY{};
	int mouse_moveflag{};
	int mouse_moveX{};
	int mouse_moveY{};
	int mousewheel{};
	int drag_start_time{};
	int is_doubleclick{};
	int is_drag_now{};
	int midi_n{};
	int midi_v{};
	int config_keymode{}; /* 0:7k 1:9k 2:5k */
	int config_button{}; /* for selection */
	int config_button_inMap{}; /* for use */
	int config_key{};
	int mouse_recentMoveTime{};
	int keyboard_presscount{};
	int joypad_presscount{};
	int MIDI_presscount{};
};

struct SkinAdjust {
	int rate_x;
	int rate_y;
	int shift_x;
	int shift_y;
	int judge_x;
	int judge_y;
	int size_x;
	int size_y;
	int note_x[PLAYER_MAX];
	int note_y[PLAYER_MAX];
	int dark_type;
};

struct ImageFont {
	int size{};
	int kerning{};
	char filepath[260]{};
	std::vector<FontImage> images; /* est.array size 1000 */
	std::unordered_map<char32_t, FontChar> chars;
};

struct SRCstruct { /* 68bytes,4*0x11 */
	int n; /* (NULL) in file */
	int * grHandles; /* =fontHandle */
	int graphcount;
	int cycle; /* =font */
	int op1;
	int op2;
	int op3;
	int op4;
	int op5;
	int count;
	int timer;
	int fontHandle;
	int align;
	int st;
	int inArray;
	int sx;
	int sy;
};

struct SkinObject {
	int srcSize;
	int dstSize;
	struct SRCstruct * src; /* dynamic,200d */
	struct DSTstruct * dst; /* dynamic,200d * 0x2C */
};

struct skstruct {
	char op[1000]{};
	int startinput_start{};
	int startinput_rank{};
	int startinput_update{};
	int scenetime{};
	int loadstart{};
	int loadend{};
	int playstart{};
	int fadeout{};
	int close{}; /* =skip */
	int GrHandle[200]{}; /* //101_gr_title */
	int grIsMovie[200]{};
	int count{};
	CSTR caption[200]{};
	int fontHandle[10]{};
	int num_of_struct{};
	struct ImageFont ImageFonts[10]{};
	int num_of_ImageFont{};
	struct SkinObject image {}; /* 200//Image */
	struct SkinObject otherObject[8]{}; /* 20//0:text 1:buttoon 2:slider 3:onmouse 4:BGA 5:bargraph 6:number 7:mask */
	int BAR_CENTER{};
	struct SRCstruct src_BAR_BODY[10]{};
	struct DSTstruct dst_BAR_BODY_OFF[30]{};
	struct DSTstruct dst_BAR_BODY_ON[30]{};
	struct SRCstruct src_BAR_TITLE[5]{};
	struct DSTstruct dst_BAR_TITLE[5]{};
	int bar_availabe_from{};
	int bar_availabe_to{};
	struct SRCstruct src_BAR_FLASH {};
	struct DSTstruct dst_BAR_FLASH {};
	struct SRCstruct src_BAR_LEVEL[10]{};
	struct DSTstruct dst_BAR_LEVEL[10]{};
	struct SRCstruct src_BAR_LAMP[10]{};
	struct DSTstruct dst_BAR_LAMP[10]{};
	struct SRCstruct src_BAR_MY_LAMP[10]{};
	struct DSTstruct dst_BAR_MY_LAMP[10]{};
	struct SRCstruct src_BAR_RIVAL_LAMP[10]{};
	struct DSTstruct dst_BAR_RIVAL_LAMP[10]{};
	struct SRCstruct src_BAR_RANK[10]{};
	struct DSTstruct dst_BAR_RANK[10]{};
	struct SRCstruct src_BAR_RIVAL[10]{};
	struct DSTstruct dst_BAR_RIVAL[10]{};
	struct SRCstruct src_BAR_STAR[10]{};
	struct DSTstruct dst_BAR_STAR[10]{};
	struct DSTstruct dst_BAR_STAGEFILE {};
	struct SRCstruct src_MOUSECURSOR {};
	struct DSTstruct dst_MOUSECURSOR {};
	struct SRCstruct src_NOTE[20]{};
	struct SRCstruct src_MINE[20]{};
	struct SRCstruct src_LN_START[20]{};
	struct SRCstruct src_LN_END[20]{};
	struct SRCstruct src_LN_BODY[20]{};
	struct SRCstruct src_AUTO_NOTE[20]{};
	struct SRCstruct src_AUTO_MINE[20]{};
	struct SRCstruct src_AUTO_LN_START[20]{};
	struct SRCstruct src_AUTO_LN_END[20]{};
	struct SRCstruct src_AUTO_LN_BODY[20]{};
	struct DSTstruct dst_NOTE[20]{};
	struct SRCstruct src_LINE[PLAYER_MAX]{};
	struct DSTstruct dst_LINE[PLAYER_MAX]{};
	struct SRCstruct src_JUDGELINE[PLAYER_MAX]{};
	struct DSTstruct dst_JUDGELINE[PLAYER_MAX]{};
	struct SRCstruct src_NOWJUDGE[PLAYER_MAX][6]{};
	struct DSTstruct dst_NOWJUDGE[PLAYER_MAX][6]{};
	struct SRCstruct src_NOWCOMBO[PLAYER_MAX][6]{};
	struct DSTstruct dst_NOWCOMBO[PLAYER_MAX][6]{};
	struct SRCstruct src_GROOVEGAUGE[PLAYER_MAX]{};
	struct DSTstruct dst_GROOVEGAUGE[PLAYER_MAX]{};
	double scratchAngle_1{};
	double scratchAngle_2{};
	struct SRCstruct src_GAUGECHART[PLAYER_MAX][2]{};
	struct DSTstruct dst_GAUGECHART[PLAYER_MAX][2]{};
	struct SRCstruct src_SCORECHART[3]{};
	struct DSTstruct dst_SCORECHART[3]{};
	struct SRCstruct src_THUMBNAIL {};
	struct DSTstruct dst_THUMBNAIL {};
	struct SRCstruct src_README[2]{};
	struct DSTstruct dst_README[2]{};
	CSTR helpfilePath[10]{};
	int helpfileCount{};
	int flag_BGA{};
	int scratchside_1{};
	int scratchside_2{};
	int flag_flip{};
	int textmergin_1{};
	int textmergin_2{};
	struct DrawingBuf drBuf {};
	CSTR fontname{};
	struct SkinAdjust adjust {};
	CSTR skinMD5{};
	CSTR skFontname{};
	bool disableImageFont{};
	CSTR customfileRANDOM[100]{};
	CSTR customfile[100]{};
	int customfile_count{};
	int reloadbanner{};
	struct SRCstruct src_EVENT_MODE_CURSOR {};
	struct DSTstruct dst_EVENT_MODE_CURSOR_ON[10]{};
	struct DSTstruct dst_EVENT_MODE_CURSOR_OFF[10]{};
	int event_STARTINPUT[10]{};
	int event_FADEOUT[10]{};
	struct DSTstruct dst_EVENT_LOADINGBG[5]{};
	int horizontal{};
	struct SRCstruct src_HITERROR[2] {};
	struct DSTstruct dst_HITERROR[2] {};
	struct SRCstruct src_HITERROR_CENTER {};
	struct DSTstruct dst_HITERROR_CENTER[2] {};
	struct SRCstruct src_HITERROR_PGREAT {};
	struct DSTstruct dst_HITERROR_PGREAT {};
	struct SRCstruct src_HITERROR_GREAT {};
	struct DSTstruct dst_HITERROR_GREAT {};
	struct SRCstruct src_HITERROR_GOOD {};
	struct DSTstruct dst_HITERROR_GOOD {};
	struct SRCstruct src_HITERROR_BAD {};
	struct DSTstruct dst_HITERROR_BAD {};
	struct SRCstruct src_HITERROR_EMA {};
	struct DSTstruct dst_HITERROR_EMA {};
};

struct MYRANKING {
	CSTR songMD5; /* struct from */
	CSTR passMD5;
	CSTR title;
	CSTR genre;
	CSTR artist;
	int maxbpm;
	int minbpm;
	int playlevel;
	int clear;
	int exscore;
	int pg;
	int gr;
	int gd;
	int bd;
	int pr;
	int maxcombo;
	int playcount;
	int clearcount;
	int rate;
	int minbp;
	int totalnotes;
	int opt_history;
	int line;
	int judge;
	int inputtype;
	int opt_this;
	int clear_ex;
	int clear_sd;
	int clear_db;
	int rseed;

	void InitRanking();
};

struct SYSTEMSOUND {
	struct SOUNDDATA exselect;
	struct SOUNDDATA exdecide;
	struct SOUNDDATA select;
	struct SOUNDDATA folder_open;
	struct SOUNDDATA folder_close;
	struct SOUNDDATA scratch;
	struct SOUNDDATA panel_open;
	struct SOUNDDATA panel_close;
	struct SOUNDDATA screenshot;
	struct SOUNDDATA option_change;
	struct SOUNDDATA decide;
	struct SOUNDDATA clear;
	struct SOUNDDATA fail;
	struct SOUNDDATA stop;
	struct SOUNDDATA mine;
	struct SOUNDDATA courseclear;
	struct SOUNDDATA coursefail;
	struct SOUNDDATA difficulty;
};

struct AUDIO_PARAM {
	struct FMOD_DSP * DSP_fx[3]{};
	struct FMOD_DSP * DSP_eq[7]{};
	struct FMOD_DSP * DSP_pitch{};
	int fx_volume_on{};
	int fx_on[3]{};
	int pitch_on{};
	int eq_on{};
	int eq_preset{};
	int fxType[3]{};
	int fxParam[3][PLAYER_MAX]{};
	int fxChannel[3]{};
	int eq_gain[7]{};
	int pitch_amount{};
	int pitch_type{}; //0:freq 1:pitch 2:speed
	int volume_key{};
	int volume_BGM{};
	int volume_master{};
	double unk4e0[5]{};
	double stagePitch[5]{};
	double stageBgmVolume[5]{};
	double stageKeyVolume[5]{};
	int time_fadeout_start{};
	int time_fadeout_end{};
	float fadeout_volume{};
	int time_fadePreview_start{};
	int time_fadePreview_end{};
	float fadePreviewCurrentVolume{};
	float fadePreviewStartVolume{};
	float fadePreviewTargetVolume{};
	char fadePreviewIsQuiet{};
};

struct AUDIO {
	struct FMOD_SYSTEM * fmodSys; /* FMOD_SOUND */
	struct FMOD_CHANNELGROUP * chnBgm;
	struct FMOD_CHANNELGROUP * chnKey;
	struct FMOD_CHANNELGROUP * chnMaster;
	struct FMOD_CHANNELGROUP * chnStageBgm[5];
	struct FMOD_CHANNELGROUP * chnStageKey[5];
	uint size;
	short * buffer;
	float volume;
	int bitRate; /* Kbps */
	struct SYSTEMSOUND sysSound;
	struct AUDIO_PARAM param;
	bool disableFmod;
	bool cmd_mediaOut;
	bool replay2avi;
	double aviTimer;
};

struct NoteStruct {
	double bmsTiming{};
	double realTiming{};
	double renderTiming{};
	double val{};
	int active{};
	bool lnHeadFast{};
	double bmsTiming_ln{};
	double realTiming_ln{};
	double renderTiming_ln{};
	int op{}; /* channel */
	int mine{}; /* soundchannel/mine */
	int stage{};
};

struct SONGSELECT {
	int bmsListCount; /* //start of struct */
	struct SONGDATA * bmsList; /* //array 1000 */
	int bmsListSize;
	int prevListCount;
	struct SONGDATA * prevList;
	int prevListSize;
	int listCalculatedBar;
	int prevCalculatedBar;
	int listTopbar;
	int prevTopbar;
	int nowBar;
	int oldBar;
	int barMoveStartTime;
	int barMoveEndTime;
	int scrollDirection; /* up:1 down:2 */
	int listSelectedBarFromScreenTop;
	int prevSelectedBarFromScreenTop; /* //? */
	int cur_song;
	int cur;
	CSTR stack_query[1000];
	CSTR stack_folderPath[1000];
	CSTR stack_searchTitle[1000];
	int stack_isFolder[1000];
	int stack_rivalID[1000];
	CSTR ROOT;
	CSTR directory;
	struct BMSMETA metaSelected;
	CSTR newsongfolder;
	int panel_unk;
	int panel;
	bool is_mouseOnSongBars;
	bool is_mouseOnSelected;
	bool is_mouseOnTextInput;
	bool is_filter_changed;
	bool is_clicked_filter;
	bool is_coursemaking_done; /* and tag edit */
	bool is_clicked_autoplay_replay;
	bool is_clicked_keyconfig;
	bool is_clicked_skinselect;
	CSTR playerPassMD5;
	CSTR playerID;
	char is_clicked_tagedit;
	char is_tag_edited;
	char is_buttonIRpage;
	char is_loading_bmslist;
	struct STATUS old;
	char flag_folderlamp;
	char flag_maniacPanel;
	int maniac_cursor; 
	char fExtraCmdDone;
	int oldIRrank;
	int titleflash;
	int queryCount; /* struct Head */
	int searchType; /* 0: 1: 2: 3:search 4:course */
	CSTR curQuery[3];
	CSTR unk4fa4[4];
	int unk4fb8[2]; /* folder map? */
	char unk4fc4[3];
	int searchFocused; /* about active? 2,4:multithread */
	int filterDifficulty; // \ref CONFIG_SELECT::difficulty
	int filterKey; // \ref CONFIG_SELECT::key
	int filterSort; // \ref CONFIG_SELECT::sort
	int searchMax; /* and rivalID */
	int isFolder;
	CSTR selFilepath; 
	CSTR selTitle;
	CSTR selFolder; 
	int selKey; 
	CSTR searchInput; /* struct finish here or far */
	int text_num;
	int filter_clicked;
	char unk5000;
	char isDifficultyFilterOn;
	int unk5004_difficultycount;
	int isRandomFolder;
	int reloadType;
	char buttonObjClicked;
	char toRoot;
	double levelsOfSong[5];
	double levelBarGraph[5];
	double levelIndicatorAnimation[5];
	struct CONFIG_SELECT filter;
	int rivalID;
	struct COURSESELECT course;
	int isExLevel;
};

struct ReplayData {
	int timing{};
	uchar op{};
	int value{};
};

struct README {
	int show;
	CSTR folderpath;
	CSTR path;
	int file_count;
	int current;
	int lines;
	CSTR body[1000];
	int x;
	int y;
	int w;
	int h;
};

struct TextStruct {
	CSTR objectStr[400];
	int hKeyInput;
	int st_text_num;
	struct README readme;
	struct OptionString option_str[25];
	std::vector<std::string> option_fullscreenfilter;
	std::vector<std::string> option_fullscreenfitstretch;
};

struct REPLAY {
	ReplayData* data{};
	int max{};
	int count{};
	int status{}; /* 0:off 1:recording 2:playing */
	struct CONFIG_PLAY cfg;
	struct AUDIO_PARAM aud;
};

struct EXTENDEDPLAYERSTATS {
	unsigned int epg = 0;
	unsigned int lpg = 0;
	unsigned int egr = 0;
	unsigned int lgr = 0;
	unsigned int egd = 0;
	unsigned int lgd = 0;
	unsigned int ebd = 0;
	unsigned int lbd = 0;
	unsigned int epr = 0;
	unsigned int lpr = 0;
	unsigned int cb = 0;
	unsigned int fast = 0;
	unsigned int slow = 0;
	unsigned int noteCount = 0;
	int lastHitOffset = 0;
	int lastFastSlow = 0;
};

struct EMA {
	double value = 0.0;
	static constexpr double alpha = 0.07;
	void add(double x) { value = alpha * x + (1.0 - alpha) * value; }
};

template<typename T>
class CircularBuffer {
	std::vector<T> buf;
	size_t head = 0;

public:
	CircularBuffer() = default;

	void push(const T& val) {
		if (buf.empty()) return;
		buf[head] = val;
		head = (head + 1) % buf.size();
	}

	void push(T&& val) {
		if (buf.empty()) return;
		buf[head] = std::move(val);
		head = (head + 1) % buf.size();
	}

	size_t size() const { return buf.size(); }
	void reset(size_t capacity) { buf.clear(); buf.resize(capacity); }

	const T& operator[](size_t i) const { return buf[i]; }
};

enum class Judgement {
	AIR_POOR,
	MISS_POOR,
	BAD,
	GOOD,
	GREAT,
	PGREAT
};

struct JudgeData {
	int offset = 0.0;
	int timeHit = 0.0;
	Judgement judge;
};

struct HITERRORDATA {
	EMA ema;
	CircularBuffer<JudgeData> notes;
};

struct PLAYERSTATUS {
	int flag_active = 0;
	int judgecount[6] = {}; /* 0unknown 1poor 2bad 3good 4great 5pgreat */
	int max_combo = 0;
	int now_combo = 0;
	int combo_song_draw = 0;
	int max_combo_course = 0;
	int now_combo_course = 0;
	int combo_draw = 0;
	int judgecount2[6] = {}; /* 0unknown 1poor 2bad 3good 4great 5pgreat */
	int total_note = 0;
	int note_current2 = 0;
	std::array<double, 6> HP = {};
	double HP_unk = 0.;
	double HP_print = 0.;
	double HP_old = 0.;
	uint time_oldHP = 0;
	uint time_newHP = 0;
	int recent_judge = 0;
	int judge_draw = 0;
	int gaugeType = OPTION_GAUGE_GROOVE;
	int lastCourseGaugeType = OPTION_GAUGE_GROOVE;
	std::array<std::array<double, 6>, 6> judge_damage = {}; 
	int judgetime[6] = {}; /* 0unknown 1poor 2bad 3good 4great 5pgreat */
	int totalnotes = 0;
	int score = 0;
	int exscore = 0;
	double rate = 0.;
	int score_unk = 0;
	int score_print = 0;
	int score_old = 0;
	int time_oldScore = 0;
	int time_newScore = 0;
	int note_current = 0;
	int clearType = 0;
	int clearGaugeTypeCourse = OPTION_GAUGE_GROOVE;
	EXTENDEDPLAYERSTATS extendedStats = {};
	std::array<EXTENDEDPLAYERSTATS, 20> extendedColumnStats = {};
	EXTENDEDPLAYERSTATS extendedStatsCourse = {};
	std::array<EXTENDEDPLAYERSTATS, 20> extendedColumnStatsCourse = {};
	int lastJudgedColumnIdx = 0;
	HITERRORDATA hiterror;
};

struct PLAYSCORE {
	CSTR name;
	char* judge_queue{};
	int judge_queue_capacity{};
	int ghostReadCount{};
	int judge_queue_len{};
	int nownote{};
	int totalnotes{};
	int judgeExpect[6]{};
	int judge[6]{};
	int rate{};
	int exscore{};

	int InitJudgeQueue(void);
	int ResetJudgeQueue(int size);
	int ResizeJudgeQueue(size_t size);
	int AddJudgeQueue(char judge);
	int DealJudgeFromQueue(void);
	char GetJudgeFromQueue(void);
	int SetDefaultGhost(int target, int notes);
	int SetGhost(int exscore, int notes, CSTR name);
	CSTR EncodeGhostData(void) const;
	int DecodeGhostData(CSTR data);
	int SetScore(PLAYERSTATUS *pstat, char flagExpect);
};

struct GRAPHDATA {
	std::array<std::array<int, 1000>, 6> hp = {};
	int combo[1000] = {};
	int exscore[1000] = {};
	int cursor = 0;
};

struct PLAYERSTATISTIC {
	int perfect;
	int great;
	int good;
	int bad;
	int poor;
	int playtime;
	int playcount;
	int clear;
	int fail;
	int combo;
	int maxcombo;
	CSTR passMD5;
	int grade7;
	int grade5;
	int grade14;
	int grade10;
	int grade9;
	int gradeversion;
	int trial;
	int trialversion;
	int option;
	int systemversion;
	int irid;
	CSTR irname;
};

struct GRAPHDATAB {
	int val[1000];
	int cursor;
};

struct gameplay {
	int keymode;
	int extramode_level;
	struct LaneStruct bmsobj;
	struct LaneStruct bmsobj_note[20];
	struct LaneStruct bmsobj_line;
	struct BPMtiming * bpmt_data;
	int bpmt_count;
	int bpmt_buffersize;
	int bpmt_start;
	struct SOUNDDATA keysound[SLOTS];
	CSTR keysound_filename[SLOTS];
	CSTR BMP_filename[SLOTS];
	struct SOUNDDATA muon;
	int bgaHandle[SLOTS];
	int bgaLayer1;
	int bgaLayer2;
	int missLayer;
	int courseBgaLayer1[10];
	int courseBgaLayer2[10];
	int courseMissLayer[10];
	int layer1ChangeTime;
	int layer2ChangeTime;
	int courseLayer1ChangeTime[10];
	int courseLayer2ChangeTime[10]; /* not used. */
	char isBgaPlaying;
	int lastMissTime;
	int misslayerTime[PLAYER_MAX]; 
	int lastMeasure;
	double BPM_fix;
	int loadObject_loaded;
	int loadObject_total;
	double BPM;
	struct PLAYERSTATUS player[PLAYER_MAX];
	double song_runtime; 
	char flag_threadExist;
	char flag_closingPhase;
	char bmsResourceLoaded; 
	int autojudge_midsum;//TODO : need to init = 0;
	int autojudge_midcount;//TODO : need to init = 0;
	int isAutoplay;
	int flag_retry;
	struct GRAPHDATA statgraph[PLAYER_MAX];
	struct GRAPHDATAB rategraph[2]; /* 0:high 1:target? */
	struct PLAYERSTATISTIC playerstat;
	int randomseed;
	struct REPLAY replay;
	struct PLAYSCORE p1Score;
	struct PLAYSCORE highScore;
	struct PLAYSCORE targetScore;
	char isGhostDisabled;
	int targetType;
	char soundonly;
	double freqSpeedMultiplier;
	char fxChangeInRecording;
	int procGameCallCount;
	bool isSpeedChanged; 
	char trialClear; 
	int lanecoverDoubleclickTimeP1; 
	int lanecoverDoubleclickTimeP2; 
	double speedmultiplier;
	double maxBPM;
	double minBPM;
	char lanecoverDisplayP1; 
	char lanecoverDisplayP2;
	char isNosave;
	char isForceEasy;
	int scratchSide;
	int timetick;
	char flag_gameinput;
	char flag_threadDoingProcGame;
	char flag_longsound;
	char flag_0note;
	float nabeatsu_x;
	float nabeatsu_y;
	float earthquake_x;
	float earthquake_y;
	int bpmChangedRealtime; /* timer142 */
	int bpmChangedBmstime; /* bpm change timing */
	char ghostBattle;
	struct CONFIG_PLAY targetCfg; /* //1p_speed ~ struct */
	int delayDetectedCount;
	int delayCheckCount;
	char isCourse; 
	int courseStageCount;
	CSTR courseFilepath[5];
	int courseType; //-1:not_course 0:course 1:nonstop 2:grade
	int courseConnection[10]; /* //nonstop 0:FADE 1:FADE+FIT 2:CUT 3:CUT+FIT 4:BALNK1 5:BALNK2 */
	int courseStageNow;
	int fadeinSOUNDstart[10];
	int fadeinSOUNDend[10];
	int fadeoutSOUNDstart[10];
	int fadeoutSOUNDend[10];
	int fadeinBGAstart[10];
	int fadeinBGAend[10];
	int fadeoutBGAstart[10];
	int fadeoutBGAend[10];
	int bgaMixer[10];
	char isPreviewLoad;
	int randomLayoutForDisplay[PLAYER_MAX]{};
	unsigned int forceRandomLayout{0}; // 0 = disabled
	std::future<void> hThreadPreview;
	int previewStatus; /* 1:start 2:loaded */
	CSTR previewBMShash;
	CSTR previewBMSfilepath;
	std::mutex criticalSection;
};

struct SkinManage {
	struct SkinHeader * Data; /* 453d0=b14*64 */
	int Count = 0;
	int Max = 0;
	int skinID[100] = {};
	int select = 0;
	int previewID = 0;
	int previewCustomID = 0;
};

struct SkinCustom {
	CSTR title;
	CSTR * op_label; /* *100d */
	int dst_op_selected;
	int dst_op_start;
	int dst_op_count;
	int labelCapacity; /* 100 */
};

struct Timer {
	double clock[500];
	double scratch; /* //for graphic */
	double gameTick; /* //game tick */
	double vSyncTick; /* //VSync */
	double tickTime; /* //elapsed time */
	double FPS;
	double FPScount;
	double FPSclock;
	double rhythmTick;
	double Rhythm; /* //1beat=1000,reset on line */
	bool flagMovieTimer;
	double movieTimer;
	double movieFramerate;

	//TEST: input process performance
	double maxGAP;
	double avgGAP;
	double GAPclock;
	int GAPtick;
	int GAPcount;
	double avgOnlyGAP;
};

struct NETWORK {
#ifdef _WIN32
	WSADATA wsa;
#endif // _WIN32
	bool isOnline{false};
	int rankUpdateDelayLevel{0};
	int waitTime{10000};
	std::mutex criticalSection;
	CSTR param;
	CSTR httpResult;
	CSTR request_debug;
	CSTR target_URL;
	CSTR IR_pass;
	CSTR IR_passMD5;
	CSTR IR_name;
	int IR_ID{0};
	int rivals[20]{};
	int rivalcount{};
	bool getRival{};
	CSTR domain = "www.dream-pro.info";
	bool waitForHandle {false};
	int timeout {15000};
	CSTR request_result;
	struct RANKING rankingData;
	struct MYRANKING myRanking;
	std::jthread hHandle;
	int IRstatus {0}; /* 0:notIR 1:loading 2:loaded -1:playerNotExist -3:connection_fail -2:bansong 3:waitUpadate 4:connection 5:IRbusy */
	CSTR IRresultMessage;
	CUSTOMIR_MANAGER customIR;

	int GetInsaneList();
	int ApplyInsaneList();

	void ParseRankingXml(const char* path);

	int HTTPrequest();
	void WaitForRankingHandle();
	int GetRanking(CSTR hash, char flagInit);

	int GetRivalInfo(int ID_rival);

	int LR2IR_GetTargetInfo(int mode, CSTR songmd5, CSTR * oStr1, CSTR * oStr2, int * oDigit1, int * oDigit2, int * oDigit3, int * oDigit4, int * oUnk, int * oExscore);
	bool GetTargetInfo(int mode, CSTR songmd5, CSTR * oStr1, CSTR * oStr2, int * oDigit1, int * oDigit2, int * oDigit3, int * oDigit4, int * oUnk, int * oExscore);
	NETWORK();
	int WS_clean();
	int LR2IR_Login(int isDirectPlay);
	int MakeIRsendScoreThread(std::string ghostString);
};

struct game {
	struct ConfigStruct config;
	struct SONGSELECT sSelect;
	struct skstruct skstruct {};
	struct skstruct skstruct2 {};
	struct SkinManage skinData;
	struct inputStructure KeyInput;
	struct Timer timer1;
	struct Timer timer2;
	struct RECORDING rec;
	struct TextStruct txtStruct;
	struct AUDIO audio;
	struct NETWORK net;
	std::vector<std::future<std::pair<std::pair<std::string, std::string>, int>>> hThreadBanner;
	int po4procSelecter;
	int po4nextProc;
	int po4sceneTimerID	;
	int po4sceneTimerIDNext;
	int po4sceneFadeout;
	int po4cur_song;
	int po4MainMenuCursor;
	int procSelecter; /* 2:select 3:deciide 4:play 5:result 6:keyconfig 7:skinselect */
	int procPhase;
	struct gameplay gameplay;
	int isSkipDrawTick; /* skip frame? */
	CSTR directoryPath;
	CSTR directoryFilename;
	CSTR baseDirectory;
	int is_starter;
	bool is_clicked_screenModeChange;
	bool flag_Screenshot{};
	bool flag_showFPS{};
	bool cmd_nosave;
	bool cmd_directplay;
	bool cmd_auto;
	bool is_recordmode;
	bool auto2avi;
	bool po4_23da8;
	bool po4flagSceneStart;
	bool po4flagSceneEnd;
};

struct SkinHeader { /* SkinInfo */
	CSTR skinFile;
	CSTR thumbnail;
	CSTR title;
	CSTR maker;
	enum SKINTYPE type;
	int informationP5;
	struct SkinCustom customs[100];
	int custom_count;

	//RESOLUTION
	int targetX = 640;
	int targetY = 480;
	bool hasResolutionTag = false; // true if the skin declared #RESOLUTION; else fall back to config global
};

struct RANKINGPLAYER {
	CSTR name;
	CSTR comment;
	int id{};
	int clear{};
	int notes{};
	int combo{};
	int pg{};
	int gr{};
	int gd{};
	int bd{};
	int pr{};
	int minbp{};
	int playcount{};
	int ranking{};
};

struct SkinUser {
	struct SkinAdjust adjust;
	int customize_value[100];
	CSTR customize_filename[100];
};

struct struct_0x14 {
	CSTR filenameHead;
	int ID;
	int count;
	int side;
	int field4_0x10;
};

struct struct_0x14_2 {
	int soundLoadID;
	int field1_0x4;
	int field2_0x8;
	int field3_0xc;
	int field4_0x10;
};

struct CHARTCONVERTER {
	struct struct_0x14 arr1[SINGLESLOTS];
	struct struct_0x14 arr2[SINGLESLOTS];
	int arr1count;
	struct struct_0x14_2 arr3[SINGLESLOTS];
	int noteCountPerLane[8];
	int laneCount;
	int unk14428;
	int unk1442c;
	int unk14430;
	int unk14438;
	int RealTimingSplitScratch;
	int assist[PLAYER_MAX];
	int playlevel;
	bool flagSplitScratch;
	bool flagSplitUnknown;
	bool flagSplit;
};

#ifdef _MSC_VER
#pragma warning (pop)
#endif

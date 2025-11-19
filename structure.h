#pragma once
#include <winsock2.h>
#include <Windows.h>
#include <vfw.h>
#include "FMODex/fmod.h"
#include "strclass.h"
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
	SKINTYPE_7KEYSBATTLE = 12, //.RED skin manual doesn't match with LR2beta3 100201
	SKINTYPE_5KEYSBATTLE = 13,
	SKINTYPE_9KEYSBATTLE = 14,
	SKINTYPE_COURSERESULT = 15,
	SKINTYPE_OPENING = 16,
	SKINTYPE_MODESELECT = 17,
	SKINTYPE_MODEDECIDE = 18,
	SKINTYPE_COURSESELECT = 19,
	SKINTYPE_COURSEEDIT = 20
} SKINTYPE;

typedef struct BMSMETA BMSMETA, *PBMSMETA;

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
	int hasTxt;
};

typedef struct BPMtiming BPMtiming, *PBPMtiming;

struct BPMtiming {
	double converted;
	double BPM;
	double realtime;
};

typedef struct CONFIG_COURSE CONFIG_COURSE, *PCONFIG_COURSE;

struct CONFIG_COURSE {
	int defaultgauge;
	int defaultconnection;
	int maxbpm;
	int minbpm;
	int bpmrange;
	int optimumlevel_5;
	int optimumlevel_7;
	int optimumlevel_9;
	int optimumlevel_10;
	int optimumlevel_14;
	int maxlevel;
	int minlevel;
	int stage;
};

typedef struct CONFIG_INPUT CONFIG_INPUT, *PCONFIG_INPUT;

struct CONFIG_INPUT {
	int buttonMap[40][16];
	int sys_inputinterval; /* Created by retype action */
	int midi_control[40];
};

typedef struct CONFIG_JUKEBOX CONFIG_JUKEBOX, *PCONFIG_JUKEBOX;

struct CONFIG_JUKEBOX {
	CSTR path[1000]; /* Created by retype action */
	int numOfPath;
	int autoreload;
	int customfolder;
	CSTR newsongfolder;
	int titleflash;
	int unused_fb4;
	int rival[20];
};

typedef struct CONFIG_NETWORK CONFIG_NETWORK, *PCONFIG_NETWORK;

struct CONFIG_NETWORK {
	int lr1ir; /* Created by retype action */
	CSTR lr1id; /* Created by retype action */
	CSTR lr1pass; /* Created by retype action */
	int lr2ir; /* Created by retype action */
	CSTR mail; /* Created by retype action */
	int autoupdate; /* Created by retype action */
	int getrival; /* Created by retype action */
};

typedef struct CONFIG_PLAY CONFIG_PLAY, *PCONFIG_PLAY;

struct CONFIG_PLAY {
	int hiSpeed[2];
	int gaugeOption[2]; /* 0:groove 1:survival 2:death 3:easy 4:pattack 5:gattack */
	int random[2]; // 1:mirror 2:random 3:s-random 4:scatter 5:converge
	int m_HIDSUD1;
	int m_HIDSUD2;
	int p1_lanecoverv;
	int p2_lanecoverv;
	int p1_lanecover;
	int p2_lanecover;
	int autokey;
	int unknown_1;
	int p1_assist;
	int p2_assist;
	int dpflip;
	int unknown_2;
	int hsfix; // 1:max 2:min 3:average 4:constant
	int battle; // 1:battle 2:d-battle 3:sp-to-dp 4:g-battle
	int autojudge;
	int judgetiming;
	int bga;
	int bgasize;
	int poorbga;
	int scoregraph;
	int play_ghost;
	int target_percent;
	int p1_target;
	int randSC[2]; // include scratch to random by impossible command
	int randFix[2]; // fix a lane by impossible command
	int replay;
	int hsmargin; /* Created by retype action */
	int hsmax; /* Created by retype action */
	int hsmin; /* Created by retype action */
	int shuttermargin; /* Created by retype action */
	int basespeed; /* Created by retype action */
	int m_softlanding;
	int m_addmine;
	int m_addlong;
	int m_earthquake;
	int m_tornado;
	int m_superloop;
	int m_gambol;
	int m_char;
	int m_heartbeat;
	int unk_c0;
	int m_loudness;
	int m_addnote;
	int m_nabeatsu;
	int m_accel;
	int m_sincurve;
	int m_wave;
	int m_spiral;
	int m_sidejump;
	int is_extra;
	int m_extra;
	undefined4 sssssss;
	char m_lunaris;
	char unk_f1;
	char unused_f2;
	undefined field59_0xf3;
	int gomiscore; /* Created by retype action */
	int disablecurspeedchange; /* Created by retype action */
	int disableleftclickexit; /* Created by retype action */
};

typedef struct CONFIG_PLAYER CONFIG_PLAYER, *PCONFIG_PLAYER;

struct CONFIG_PLAYER {
	CSTR id; /* Created by retype action */
	CSTR pass; /* Created by retype action */
	CSTR passMD5; /* Created by retype action */
	CSTR lrid; /* Created by retype action */
	CSTR irpass; /* Created by retype action */
	CSTR irpassMD5;
	CSTR name; /* Created by retype action */
};

typedef struct CONFIG_SELECT CONFIG_SELECT, *PCONFIG_SELECT;

struct CONFIG_SELECT {
	int difficulty;
	int sort;
	int key;
	int searchmax; /* Created by retype action */
	int randomclose;
	int speedfirst; /* Created by retype action */
	int speednext; /* Created by retype action */
	int control; /* Created by retype action */
	int buttonselect; /* Created by retype action */
	int folderlamp;
	undefined4 unused_28;
	int difficultychangetype;
	int ignorekeyall; /* Created by retype action */
	int ignorekeysingle; /* Created by retype action */
	int ignorekeydouble; /* Created by retype action */
	int ignoredp; /* Created by retype action */
	int ignorepms; /* Created by retype action */
	int ignoredifficultyall; /* Created by retype action */
	int ignore5key; /* Created by retype action */
	int titleflash;
	int levelbarflash_7;
	int levelbarflash_5;
	int levelbarflash_9;
	int disabledifficultyfilter;
	int preview;
	int disablesubtitle;
};

typedef struct CONFIG_SKIN CONFIG_SKIN, *PCONFIG_SKIN;

struct CONFIG_SKIN {
	int disableimagefont; /* Created by retype action */
	CSTR fontname; /* Created by retype action */
	CSTR skinFilePath[30];
};

typedef struct CONFIG_SOUND CONFIG_SOUND, *PCONFIG_SOUND;

struct CONFIG_SOUND {
	int disabledsp;
	int bufferlength; /* Created by retype action */
	int numbuffers; /* Created by retype action */
	int output; /* Created by retype action */
	int driver; /* Created by retype action */
	int disablefmod; /* Created by retype action */
	int volumemaster; /* Created by retype action */
	int volumekey; /* Created by retype action */
	int volumebgm; /* Created by retype action */
	int volumeflag; /* Created by retype action */
	int eqflag; /* Created by retype action */
	int eqp0; /* Created by retype action */
	int eqp1;
	int eqp2;
	int eqp3;
	int eqp4;
	int eqp5;
	int eqp6;
	int pitchflag;
	int pitchp;
	int pitchtype;
	int fxflag_0;
	int fxflag_1;
	int fxflag_2;
	int fxtype_0;
	int fxtype_1;
	int fxtype_2;
	int fxtarget_0;
	int fxtarget_1;
	int fxtarget_2;
	int fxp1_0;
	int fxp1_1;
	int fxp1_2;
	int fxp2_0;
	int fxp2_1;
	int fxp2_2;
};

typedef struct CONFIG_SYSTEM CONFIG_SYSTEM, *PCONFIG_SYSTEM;

struct CONFIG_SYSTEM {
	int screenmode;
	int vsync; /* Created by retype action */
	int directdraw; /* Created by retype action */
	int mainsleep; /* Created by retype action */
	int bmssleep; /* Created by retype action */
	int screenexrate; /* Created by retype action */
	int highcolor; /* Created by retype action */
	int disablesystemkey; /* Created by retype action */
	int outputlog; /* Created by retype action */
	int eventmode; /* Created by retype action */
	int thread;
	int disableskinpreview; /* Created by retype action */
	int hptimer; /* Created by retype action */
	int isablebmsthread; /* Created by retype action */
	int disablefolderthread; /* Created by retype action */
	int language; /* Created by retype action */
	int windowsize_x; /* Created by retype action */
	int windowsize_y; /* Created by retype action */
	int maindisplay; /* Created by retype action */
	int softwarerendering; /* Created by retype action */
};

typedef struct CONFIG_TOOLS CONFIG_TOOLS, *PCONFIG_TOOLS;

struct CONFIG_TOOLS {
	CSTR mp3enc_body; /* Created by retype action */
	CSTR mp3enc_option_normal; /* Created by retype action */
	CSTR mp3enc_option_movie; /* Created by retype action */
	int movie_audio;
	int mp3_volume;
	int movie_framerate;
	CSTR oggenc_body; /* Created by retype action */
	CSTR oggenc_option; /* Created by retype action */
	CSTR oggdec_body; /* Created by retype action */
	CSTR oggdec_option; /* Created by retype action */
	CSTR bmse_body; /* Created by retype action */
	CSTR bmse_option; /* Created by retype action */
	int autowavtoogg;
	int autobmptopng;
	int autofumensearch;
	undefined4 field15_0x3c;
};

typedef struct ConfigStruct ConfigStruct, *PConfigStruct;

struct ConfigStruct {
	struct CONFIG_PLAY play;
	struct CONFIG_SYSTEM system;
	struct CONFIG_SELECT select;
	struct CONFIG_INPUT input;
	struct CONFIG_SOUND sound;
	struct CONFIG_SKIN skin;
	struct CONFIG_JUKEBOX jukebox;
	struct CONFIG_PLAYER player;
	struct CONFIG_NETWORK network;
	struct CONFIG_COURSE course;
	struct CONFIG_TOOLS tools;
};

typedef struct COURSESELECT COURSESELECT, *PCOURSESELECT;

typedef struct SONGDATA SONGDATA, *PSONGDATA;

typedef struct STATUS STATUS, *PSTATUS;

struct STATUS {
	int stat_pgreat;
	int stat_great;
	int stat_good;
	int stat_bad;
	int stat_poor;
	int clear;
	int stat_score;
	int stat_exscore;
	int rank;
	int rate;
	int minbp;
	int stat_maxcombo;
	int playcount; /* Created by retype action */
	int clearcount; /* Created by retype action */
	int failcount; /* Created by retype action */
	int clear_db; /* Created by retype action */
	int clear_sd; /* Created by retype action */
	int clear_ex; /* Created by retype action */
	int total_notes;
	int op_history; /* Created by retype action */
	int op_best; /* Created by retype action */
	int IRranking;
	int IRclearRate;
	int IRplayercount;
	char IRthreadEnd;
	int complete; /* Created by retype action */
	int rseed; /* Created by retype action */
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
	int isStagefile;
	int isBanner;
	int isBackBMP;
	uint difficulty;
	int level;
	int exlevel;
	int keymode;
	int folderType; /* 0,5:song 1:folder 2:levelfolder 3,4,6:folder 7:newcourse 8:course 9:randomcourse */
	int maxBPM;
	int minBPM;
	int bga;
	int txt;
	int longnote;
	int random;
	int judge;
	int replayExist;
	int favorite;
	int adddate;
	int difficultyLevel[5]; /* 0:beginner 1:nomal 2:hyper 3:another 4:insane */
	int difficultyExist[5]; /* 0:beginner 1:nomal 2:hyper 3:another 4:insane */
	int difficultyLevelBarLamp[5]; /* 0:beginner 1:nomal 2:hyper 3:another 4:insane */
	int difficultyCount; /* 0:beginner 1:nomal 2:hyper 3:another 4:insane */
	CSTR courseTitle[10];
	CSTR courseSubtitle[10];
	CSTR courseHash[10];
	int courseLevel[10];
	int courseStageDifficulty[10];
	int courseTotalnote[10];
	int courseID;
	int courseStageCount;
	int coursePlayable; /* have all songs */
	char courseReadOnly;
	int courseKeys[10];
	int courseType; /* 0:expert 1:nonstop 2:class */
	int courseIR; /* IR flag */
	int grHandle;
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

typedef struct CSVbuf CSVbuf, *PCSVbuf;

struct CSVbuf {
	int val[30];
	CSTR str[30];
};

typedef struct DrawingBuf DrawingBuf, *PDrawingBuf;

typedef struct DSTdraw DSTdraw, *PDSTdraw;

struct DSTdraw { /* 80bytes,4*0x14 */
	float x;
	float y;
	float w;
	float h;
	int sortID;
	int time;
	int acc;
	int blend;
	int filter;
	int a;
	int r;
	int g;
	int b;
	float angle;
	int center;
	int grHandle;
	int fontHandle;
	int subHandle;
	int align;
	char isDrawBackbox;
};

struct DrawingBuf {
	struct DSTdraw * dstd;
	int max;
	int count;
	int isHidSud[20]; /* 40680b/49b4a5 */
	int top[20]; /* sud */
	int bottom[20]; /* hid */
	char flagImageFont;
	char isDisabled;
	char unkFE;
	char unkFF;
};

typedef struct DSTstruct DSTstruct, *PDSTstruct;

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

typedef struct Event Event, *PEvent;

struct Event {
	CSTR field0_0x0[2];
	int field1_0x8[8];
};

typedef struct FontChar FontChar, *PFontChar;

struct FontChar {
	int srcX;
	int srcY;
	int width;
	int height;
	int ImageNum;
	int grHandle;
};

typedef struct FontImage FontImage, *PFontImage;

struct FontImage {
	char filename[32];
	int grHandle;
};

typedef struct game game, *Pgame;

typedef struct SONGSELECT SONGSELECT, *PSONGSELECT;

typedef struct skstruct skstruct, *Pskstruct;

typedef struct SkinManage SkinManage, *PSkinManage;

typedef struct inputStructure inputStructure, *PinputStructure;

typedef struct Timer Timer, *PTimer;

typedef struct RECORDING RECORDING, *PRECORDING;

typedef struct TextStruct TextStruct, *PTextStruct;

typedef struct AUDIO AUDIO, *PAUDIO;

typedef struct AUDIO_PARAM AUDIO_PARAM, *PAUDIO_PARAM;

typedef struct gameplay gameplay, *Pgameplay;

typedef struct NETWORK NETWORK, *PNETWORK;

typedef struct ImageFont ImageFont, *PImageFont;

typedef struct SkinObject SkinObject, *PSkinObject;

typedef struct SRCstruct SRCstruct, *PSRCstruct;

typedef struct SkinAdjust SkinAdjust, *PSkinAdjust;

typedef struct SkinHeader SkinHeader, *PSkinHeader;

typedef struct README README, *PREADME;

typedef struct OptionString OptionString, *POptionString;

typedef struct SYSTEMSOUND SYSTEMSOUND, *PSYSTEMSOUND;

typedef struct LaneStruct LaneStruct, *PLaneStruct;

typedef struct SOUNDDATA SOUNDDATA, *PSOUNDDATA;

typedef struct PLAYERSTATUS PLAYERSTATUS, *PPLAYERSTATUS;

typedef struct GRAPHDATA GRAPHDATA, *PGRAPHDATA;

typedef struct GRAPHDATAB GRAPHDATAB, *PGRAPHDATAB;

typedef struct PLAYERSTATISTIC PLAYERSTATISTIC, *PPLAYERSTATISTIC;

typedef struct REPLAY REPLAY, *PREPLAY;

typedef struct PLAYSCORE PLAYSCORE, *PPLAYSCORE;

typedef long LONG;

typedef struct RANKING RANKING, *PRANKING;

typedef struct MYRANKING MYRANKING, *PMYRANKING;

typedef struct SkinCustom SkinCustom, *PSkinCustom;

typedef ulong DWORD;

typedef void * LPVOID;

typedef struct NoteStruct NoteStruct, *PNoteStruct;

typedef struct RAWSOUND RAWSOUND, *PRAWSOUND;

typedef struct ReplayData ReplayData, *PReplayData;

typedef ushort WORD;

typedef struct RANKINGPLAYER RANKINGPLAYER, *PRANKINGPLAYER;

typedef uchar BYTE;

struct RECORDING {
	HDC srcHDC; /* struct_entry */
	undefined4 unk4;
	double framerate;
	int bitdepth;
	CSTR filename;
	uint framelen;
	int writeSamplePos;
	int curFrame;
	COMPVARS compvars; /* from vfw.h */
	BITMAPINFOHEADER bmiHeader;
	PAVISTREAM pAVIstream; /* from vfw.h */
	PAVIFILE pAVIFILE; /* from vfw.h */
	void * buf;
	HWND hwnd;
	HDC dstHDC;
	HBITMAP hBIT;
	HGDIOBJ hGDI;
	int recMode; /* 1:auto2avi 2:replay2avi 3:bga2avi 4:movie */
	undefined4 unkac;

	RECORDING();
	bool RefreshCurFrame();
	int GetCurTime();
	int CpyScreenToAVI();
	bool Release();
	bool PrepareAVIRecord(double framerate, int bit, CSTR filename, uint frameLen, HWND hwnd);
	int InsertAudioToMovie(CSTR pathAudio, bool deleteFlag);

};

struct OptionString {
	CSTR str[10];
	int count;
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
	CSTR unk_0;
	CSTR unk_4;
	struct RANKINGPLAYER * ranking;
	int rankingCount;
	int rankingMax;
	int clearPlayers[6]; // 5fc 4hd 3gr 2ez 1fail 0no?
	int totalPlaycount;
	void * unused30;
	int rankingCursor;
	CSTR lastupdate;
	int target_ID;
	int target_number;
	void * unused48;
	void * unused44;
	char showRanking; /* char */
	undefined field23_0x4d;
	undefined field24_0x4e;
	undefined field25_0x4f;
	int myRanking;
	int myID;
	int rivalRanking;
	int rivalID;

	void ExpandRankingBuffer(int add);
	void Init();
	int ParseXML(CSTR path);
	RANKING();
};

struct RAWSOUND {
	int channels;
	int samples;
	int bits;
	int length;
	int dataSize;
	byte* data;

	RAWSOUND();
	void ExpandBuffer(int minSize);
	void MakeBitDepth16(void);
	void MakeStereo(void);
	void MakeSampleRate44100(void);
};


struct SOUNDDATA {
	char load;
	char loop;
	undefined field2_0x2;
	undefined field3_0x3;
	CSTR filename;
	uint length;
	int unused0C;
	int soundHandle;
	struct RAWSOUND raw;
	char flag2c;
	undefined field10_0x2d;
	undefined field11_0x2e;
	undefined field12_0x2f;
	struct FMOD_SOUND * fmod_sound;
	struct FMOD_CHANNEL * fmod_channel;
};

struct inputStructure {
	unsigned char inputID[2048]; /* 0:keybd 0x100:joypad 0x200:MIDI */
	int inputTime[2048]; /* 0:keybd 0x100:joypad 0x200:MIDI */
	unsigned char p1_buttonInput[20]; /* byte100 */
	unsigned char p2_buttonInput[20];
	unsigned char otherbuttons[60];
	char mouse_buttonL; /* 0:not pressed 1:just pressed 2:pressed 3:released */
	char mouse_buttonR;
	char mouse_buttonW;
	char mouse_button4;
	int mouse_oldX;
	int mouse_oldY;
	int mouse_moveflag;
	int mouse_moveX;
	int mouse_moveY;
	int mousewheel;
	int drag_start_time;
	int is_doubleclick;
	int is_drag_now;
	int midi_n;
	int midi_v;
	int config_keymode; /* 0:7k 1:9k 2:5k */
	int config_button; /* for selection */
	int config_button_inMap; /* for use */
	int config_key; /* Created by retype action */
	int mouse_recentMoveTime;
	int keyboard_presscount;
	int joypad_presscount;
	int MIDI_presscount;
};

struct SkinAdjust {
	int rate_x;
	int rate_y;
	int shift_x;
	int shift_y;
	int judge_x;
	int judge_y;
	int unk18;
	int unk1c;
	int size_x;
	int size_y;
	int note_1p_x;
	int note_1p_y;
	int note_2p_x;
	int note_2p_y;
	int dark_type;
};

struct ImageFont {
	int size;
	int kerning;
	char filepath[260];
	struct FontImage * images; /* est.array size 1000 */
	struct FontChar * chars; /* est.array size 0x3bce */
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
	char op[1000];
	int startinput_start;
	int startinput_rank;
	int startinput_update;
	int scenetime;
	int loadstart;
	int loadend;
	int playstart;
	int fadeout;
	int close; /* =skip */
	int GrHandle[200]; /* //101_gr_title */
	int grIsMovie[200];
	int count;
	CSTR caption[200];
	int fontHandle[10];
	int num_of_struct;
	struct ImageFont ImageFonts[10];
	int num_of_ImageFont;
	struct SkinObject image; /* 200//Image */
	struct SkinObject otherObject[8]; /* 20//0:text 1:buttoon 2:slider 3:onmouse 4:BGA 5:bargraph 6:number 7:mask */
	int BAR_CENTER;
	struct SRCstruct src_BAR_BODY[10];
	struct DSTstruct dst_BAR_BODY_OFF[30];
	struct DSTstruct dst_BAR_BODY_ON[30];
	struct SRCstruct src_BAR_TITLE[5];
	struct DSTstruct dst_BAR_TITLE[5];
	int bar_availabe_from;
	int bar_availabe_to;
	struct SRCstruct src_BAR_FLASH;
	struct DSTstruct dst_BAR_FLASH;
	struct SRCstruct src_BAR_LEVEL[10];
	struct DSTstruct dst_BAR_LEVEL[10];
	struct SRCstruct src_BAR_LAMP[10];
	struct DSTstruct dst_BAR_LAMP[10];
	undefined unused[2240];
	struct SRCstruct src_BAR_MY_LAMP[10];
	struct DSTstruct dst_BAR_MY_LAMP[10];
	struct SRCstruct src_BAR_RIVAL_LAMP[10];
	struct DSTstruct dst_BAR_RIVAL_LAMP[10];
	struct SRCstruct src_BAR_RANK[10];
	struct DSTstruct dst_BAR_RANK[10];
	struct SRCstruct src_BAR_RIVAL[10];
	struct DSTstruct dst_BAR_RIVAL[10];
	struct SRCstruct src_BAR_STAR[10];
	struct DSTstruct dst_BAR_STAR[10];
	struct DSTstruct dst_BAR_STAGEFILE;
	struct SRCstruct src_MOUSECURSOR;
	struct DSTstruct dst_MOUSECURSOR;
	struct SRCstruct src_NOTE[20];
	struct SRCstruct src_MINE[20];
	struct SRCstruct src_LN_START[20];
	struct SRCstruct src_LN_END[20];
	struct SRCstruct src_LN_BODY[20];
	struct SRCstruct src_AUTO_NOTE[20];
	struct SRCstruct src_AUTO_MINE[20];
	struct SRCstruct src_AUTO_LN_START[20];
	struct SRCstruct src_AUTO_LN_END[20];
	struct SRCstruct src_AUTO_LN_BODY[20];
	struct DSTstruct dst_NOTE[20];
	struct SRCstruct src_LINE[2];
	struct DSTstruct dst_LINE[2];
	struct SRCstruct src_JUDGELINE[2];
	struct DSTstruct dst_JUDGELINE[2];
	struct SRCstruct src_NOWJUDGE_1P[6];
	struct DSTstruct dst_NOWJUDGE_1P[6];
	struct SRCstruct src_NOWCOMBO_1P[6];
	struct DSTstruct dst_NOWCOMBO_1P[6];
	struct SRCstruct src_NOWJUDGE_2P[6];
	struct DSTstruct dst_NOWJUDGE_2P[6];
	struct SRCstruct src_NOWCOMBO_2P[6];
	struct DSTstruct dst_NOWCOMBO_2P[6];
	struct SRCstruct src_GROOVEGAUGE[2];
	struct DSTstruct dst_GROOVEGAUGE[2];
	double scratchAngle_1;
	double scratchAngle_2;
	struct SRCstruct src_GAUGECHART_1P[2];
	struct DSTstruct dst_GAUGECHART_1P[2];
	struct SRCstruct src_GAUGECHART_2P[2];
	struct DSTstruct dst_GAUGECHART_2P[2];
	struct SRCstruct src_SCORECHART[3];
	struct DSTstruct dst_SCORECHART[3];
	struct SRCstruct src_THUMBNAIL;
	struct DSTstruct dst_THUMBNAIL;
	struct SRCstruct src_README[2];
	struct DSTstruct dst_README[2];
	CSTR helpfilePath[10];
	int helpfileCount;
	int flag_BGA;
	int scratchside_1;
	int scratchside_2;
	int flag_flip;
	int textmergin_1;
	int textmergin_2;
	struct DrawingBuf drBuf;
	int unused_disableimagefont;
	CSTR fontname;
	struct SkinAdjust adjust;
	CSTR skinMD5;
	CSTR skFontname;
	int disableimagefont; /* bool */
	CSTR customfileRANDOM[100];
	CSTR customfile[100];
	int customfile_count;
	CSTR field103_0x9f70;
	int reloadbanner;
	struct SRCstruct src_EVENT_MODE_CURSOR;
	struct DSTstruct dst_EVENT_MODE_CURSOR_ON[10];
	struct DSTstruct dst_EVENT_MODE_CURSOR_OFF[10];
	int event_STARTINPUT[10];
	int event_FADEOUT[10];
	struct DSTstruct dst_EVENT_LOADINGBG[5];
	int horizontal;
	undefined4 sussus;
};

struct MYRANKING {
	CSTR songMD5; /* struct from */
	CSTR unused; /* ID?name? but unused */
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
	CSTR ghost; /* struct to? */

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
	struct FMOD_DSP * DSP_fx[3];
	struct FMOD_DSP * DSP_eq[7];
	struct FMOD_DSP * DSP_pitch;
	int fx_volume_on;
	int fx_on[3];
	int pitch_on;
	int eq_on;
	int eq_preset;
	int fxType[3];
	int fxParam[3][2];
	int fxChannel[3];
	int eq_gain[7];
	int pitch_amount;
	int pitch_type; //0:freq 1:pitch 2:speed
	int volume_key;
	int volume_BGM;
	int volume_master;
	double unk4e0[5];
	double stagePitch[5];
	double stageBgmVolume[5];
	double stageKeyVolume[5];
	int time_fadeout_start;
	int time_fadeout_end;
	float fadeout_volume;
	int time_fadePreview_start;
	int time_fadePreview_end;
	float fadePreviewCurrentVolume;
	float fadePreviewStartVolume;
	float fadePreviewTargetVolume;
	char fadePreviewIsQuiet;
	undefined field30_0x169;
	undefined field31_0x16a;
	undefined field32_0x16b;
	undefined4 unk5a4;
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
	int is_fmod_disabled;
	bool cmd_mediaOut;
	bool replay2avi;
	undefined field15_0x5ae;
	undefined field16_0x5af;
	double aviTimer;
};

struct NoteStruct {
	double bmsTiming;
	double realTiming;
	double val;
	int active;
	undefined4 unk1c;
	double bmsTiming_ln;
	double realTiming_ln;
	int op; /* channel */
	int mine; /* soundchannel/mine */
	int stage;
	undefined4 unk3c;
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
	char is_mouseOnSongBars;
	char is_mouseOnSelected;
	char is_mouseOnTextInput;
	char is_filter_changed;
	char is_clicked_filter;
	char is_coursemaking_done; /* and tag edit */
	char is_clicked_autoplay_replay;
	char is_clicked_keyconfig;
	char is_clicked_skinselect;
	char unused4ef1;
	char unused4ef2;
	char unused4ef3;
	CSTR playerPassMD5;
	CSTR playerID;
	char is_clicked_tagedit;
	char is_tag_edited;
	char is_buttonIRpage;
	char is_loading_bmslist;
	struct STATUS old;
	char flag_folderlamp;
	char flag_maniacPanel;
	char unused4f6e;
	char unused4f6f;
	int maniac_cursor; /* Created by retype action */
	char unk4f74; /* //clear screen flag? */
	char unused4f75;
	char unused4f76;
	char unused4f77;
	int unk4f78;
	char unk4f7c;
	char unk4f7d;
	char fExtraCmdDone;
	char unk4f7e;
	int unused4f80;
	int unk4f84;
	int oldIRrank;
	int titleflash;
	int queryCount; /* struct Head */
	int searchType; /* 0: 1: 2: 3:search 4:course */
	CSTR curQuery[3];
	CSTR unk4fa4[4];
	CSTR unk4fb4;
	int unk4fb8[2]; /* folder map? */
	int unk4fc0;
	char unk4fc4[3];
	undefined field75_0x4fc7;
	int searchFocused; /* about active? 2,4:multithread */
	int filterDifficulty; /* Created by retype action */
	int filterKey; /* Created by retype action */
	int filterSort; /* Created by retype action */
	int searchMax; /* and rivalID */
	int isFolder;
	CSTR selFilepath; /* Created by retype action */
	CSTR selTitle;
	CSTR selFolder; /* Created by retype action */
	int selKey; /* Created by retype action */
	CSTR searchInput; /* struct finish here or far */
	int text_num;
	int unk4ff8;
	int filter_clicked;
	char unk5000;
	char isDifficultyFilterOn;
	undefined field92_0x5002;
	undefined field93_0x5003;
	int unk5004_difficultycount;
	int isRandomFolder;
	int reloadType;
	char buttonObjClicked;
	char toRoot;
	undefined field99_0x5012;
	undefined field100_0x5013;
	undefined field101_0x5014;
	undefined field102_0x5015;
	undefined field103_0x5016;
	undefined field104_0x5017;
	double levelsOfSong[5];
	double levelBarGraph[5];
	double levelIndicatorAnimation[5];
	struct CONFIG_SELECT filter;
	char isRankingAutoUpdateThread; /* IR */
	undefined field110_0x50f9;
	undefined field111_0x50fa;
	undefined field112_0x50fb;
	int rivalID;
	struct COURSESELECT course;
	int isExLevel;
};

struct ReplayData {
	int timing;
	uchar op;
	int value;
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
	CSTR objectStr[300];
	int hKeyInput;
	int st_text_num;
	struct README readme;
	struct OptionString option_str[25];
	int unused;
};

struct REPLAY {
	struct ReplayData * data;
	int max;
	int count;
	int status; /* 0:off 1:recording 2:playing */
	struct CONFIG_PLAY cfg;
	struct AUDIO_PARAM aud;
};

struct PLAYERSTATUS {
	int flag_active;
	int judgecount[6]; /* 0unknown 1poor 2bad 3good 4great 5pgreat */
	int max_combo; /* Created by retype action */
	int now_combo;
	int combo_song_draw;
	int max_combo_course;
	int now_combo_course;
	int combo_draw;
	int judgecount2[6]; /* 0unknown 1poor 2bad 3good 4great 5pgreat */
	int total_note;
	int note_current2;
	int field11_0x54;
	double HP;
	double HP_unk;
	double HP_print;
	double HP_old;
	uint time_oldHP;
	uint time_newHP;
	int recent_judge;
	int judge_draw;
	double judge_damage[6];
	int judgetime[6]; /* 0unknown 1poor 2bad 3good 4great 5pgreat */
	int totalnotes;
	int score;
	int exscore;
	undefined field25_0xdc;
	undefined field26_0xdd;
	undefined field27_0xde;
	undefined field28_0xdf;
	double rate;
	int score_unk;
	int score_print;
	int score_old;
	int time_oldScore;
	int time_newScore;
	int note_current;
	int clearType;
	undefined field49_0x104;
	undefined field50_0x105;
	undefined field51_0x106;
	undefined field52_0x107;
	undefined field53_0x108;
	undefined field54_0x109;
	undefined field55_0x10a;
	undefined field56_0x10b;
	undefined field57_0x10c;
	undefined field58_0x10d;
	undefined field59_0x10e;
	undefined field60_0x10f;
	undefined field61_0x110;
	undefined field62_0x111;
	undefined field63_0x112;
	undefined field64_0x113;
	undefined field65_0x114;
	undefined field66_0x115;
	undefined field67_0x116;
	undefined field68_0x117;
};

struct PLAYSCORE {
	CSTR name;
	char * judge_queue;
	int judge_queue_count;
	int ghostReadCount;
	int judgecount;
	int nownote;
	int totalnotes;
	int judgeExpect[6];
	int judge[6];
	int rate;
	int exscore;

	PLAYSCORE();
	int InitJudgeQueue(void);
	int ResetJudgeQueue(int size);
	int ResizeJudgeQueue(size_t size);
	int AddJudgeQueue(char judge);
	int DealJudgeFromQueue(void);
	char GetJudgeFromQueue(void);
	int SetDefaultGhost(int target, int notes);
	int SetGhost(int exscore, int notes, CSTR name);
	CSTR EncodeGhostData(void);
	int DecodeGhostData(CSTR data);
	int SetScore(PLAYERSTATUS *pstat, char flagExpect);
};

struct GRAPHDATA {
	int hp[1000];
	int combo[1000];
	int exscore[1000];
	int cursor;
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
	CSTR field11_0x2c;
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
	struct SOUNDDATA keysound[6480];
	CSTR keysound_filename[6480];
	CSTR BMP_filename[6480];
	struct SOUNDDATA muon;
	char bgaUnused656b8[6480];
	int bgaHandle[6480];
	int bgaHandleHandle[6480];
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
	undefined field27_0x73b65;
	undefined field28_0x73b66;
	undefined field29_0x73b67;
	int unused_73b68;
	int lastMissTime;
	int misslayerTime[2]; /* Created by retype action */
	int lastMeasure;
	undefined field34_0x73b7c;
	undefined field35_0x73b7d;
	undefined field36_0x73b7e;
	undefined field37_0x73b7f;
	double BPM_fix;
	int loadObject_loaded;
	int loadObject_total;
	double BPM;
	undefined field42_0x73b98;
	undefined field43_0x73b99;
	undefined field44_0x73b9a;
	undefined field45_0x73b9b;
	undefined field46_0x73b9c;
	undefined field47_0x73b9d;
	undefined field48_0x73b9e;
	undefined field49_0x73b9f;
	undefined field50_0x73ba0;
	undefined field51_0x73ba1;
	undefined field52_0x73ba2;
	undefined field53_0x73ba3;
	undefined field54_0x73ba4;
	undefined field55_0x73ba5;
	undefined field56_0x73ba6;
	undefined field57_0x73ba7;
	undefined field58_0x73ba8;
	undefined field59_0x73ba9;
	undefined field60_0x73baa;
	undefined field61_0x73bab;
	undefined field62_0x73bac;
	undefined field63_0x73bad;
	undefined field64_0x73bae;
	undefined field65_0x73baf;
	struct PLAYERSTATUS player[2];
	double song_runtime; /* Created by retype action */
	char flag_threadExist;
	char flag_closingPhase;
	undefined1 bmsResourceLoaded; /* Created by retype action */
	undefined field71_0x73deb;
	int autojudge_midsum;//TODO : need to init = 0;
	int autojudge_midcount;//TODO : need to init = 0;
	int isAutoplay;
	int flag_retry;
	struct GRAPHDATA statgraph[2];
	struct GRAPHDATAB rategraph[2]; /* 0:high 1:target? */
	struct PLAYERSTATISTIC playerstat;
	int randomseed;
	undefined field80_0x7bb74;
	undefined field81_0x7bb75;
	undefined field82_0x7bb76;
	undefined field83_0x7bb77;
	struct REPLAY replay;
	struct PLAYSCORE p1Score;
	struct PLAYSCORE highScore;
	struct PLAYSCORE targetScore;
	char isGhostDisabled;
	undefined field89_0x7bef5;
	undefined field90_0x7bef6;
	undefined field91_0x7bef7;
	int targetType;
	char soundonly;
	undefined field94_0x7befd;
	undefined field95_0x7befe;
	undefined field96_0x7beff;
	double freqSpeedMultiplier;
	char fxChangeInRecording;
	undefined field99_0x7bf09;
	undefined field100_0x7bf0a;
	undefined field101_0x7bf0b;
	int procGameCallCount;
	bool isSpeedChanged; /* Created by retype action */
	undefined1 trialClear; /* Created by retype action */
	undefined field105_0x7bf12;
	undefined field106_0x7bf13;
	int lanecoverDoubleclickTimeP1; 
	int lanecoverDoubleclickTimeP2; 
	undefined field109_0x7bf1c;
	undefined field110_0x7bf1d;
	undefined field111_0x7bf1e;
	undefined field112_0x7bf1f;
	double speedmultiplier;
	double maxBPM;
	double minBPM;
	char lanecoverDisplayP1; /* Created by retype action */
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
	float unusedX_7bf50;
	float unusedY_7bf54;
	float earthquake_x;
	float earthquake_y;
	int bpmChangedRealtime; /* timer142 */
	int bpmChangedBmstime; /* bpm change timing */
	char unused_7bf68;
	char ghostBattle; /* Created by retype action */
	undefined field136_0x7bf6a;
	undefined field137_0x7bf6b;
	struct CONFIG_PLAY targetCfg; /* //1p_speed ~ struct */
	int delayDetectedCount;
	int delayCheckCount;
	undefined1 isCourse; /* Created by retype action */
	undefined field142_0x7c075;
	undefined field143_0x7c076;
	undefined field144_0x7c077;
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
	char padding1;
	char padding2;
	char padding3;
	HANDLE hThreadPreview;
	int previewStatus; /* 1:start 2:loaded */
	CSTR previewBMShash;
	CSTR previewBMSfilepath;
	CRITICAL_SECTION criticalSection;
};

struct SkinManage {
	struct SkinHeader * Data; /* 453d0=b14*64 */
	int Count;
	int Max;
	int skinID[100];
	int select;
	int previewID;
	int previewCustomID;
};

struct SkinCustom {
	CSTR title;
	CSTR * op_label; /* *100d */
	int dst_op_selected;
	int dst_op_start;
	int dst_op_count;
	int labelCapacity; /* 100 */
	int undefined; /* 0 */
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
	char flagMovieTimer; /* //char,bool */
	undefined padding1;
	undefined padding2;
	undefined padding3;
	int unused;
	double movieTimer;
	double movieFramerate;
};

struct NETWORK {
	WSADATA wsa;
	int isOnline;
	int rankUpdateDelayLevel;
	int waitTime;
	CRITICAL_SECTION criticalSection;
	CSTR param;
	CSTR httpResult;
	int isRequestSuccess;
	CSTR request_debug;
	CSTR target_URL;
	CSTR IR_pass;
	CSTR IR_passMD5;
	CSTR IR_name;
	int IR_ID;
	int rivals[20];
	int rivalcount;
	int getrival;
	CSTR domain;
	undefined unused234;
	char waitForHandle;
	undefined field20_0x236;
	undefined field21_0x237;
	int timeout;
	CSTR request_result;
	int loginResult;
	struct RANKING rankingData;
	struct MYRANKING myRanking;
	HANDLE hHandle;
	int IRstatus; /* 0:notIR 1:loading 2:loaded -1:playerNotExist -3:connection_fail -2:bansong 3:waitUpadate 4:connection 5:IRbusy */
	CSTR IRresultMessage;


	int GetInsaneList();

	int Init();

	void Lock();
	void Unlock();
	int HTTPrequest();
	void WaitAndInitRanking();
	int GetRanking(CSTR hash, char flagInit);

	int GetRivalInfo(int ID_rival);

	int GetTargetInfo(int mode, CSTR songmd5, CSTR * oStr1, CSTR * oStr2, int * oDigit1, int * oDigit2, int * oDigit3, int * oDigit4, int * oUnk, int * oExscore);
	NETWORK();
	int WS_clean();
	int Login(int isDirectPlay);
	int MakeIRsendScoreThread();
};

struct game {
	struct ConfigStruct config;
	struct SONGSELECT sSelect;
	struct skstruct skstruct;
	struct skstruct skstruct2;
	struct SkinManage skinData;
	undefined4 unk1d588;
	struct inputStructure KeyInput;
	undefined field7_0x1fe40;
	undefined field8_0x1fe41;
	undefined field9_0x1fe42;
	undefined field10_0x1fe43;
	undefined field11_0x1fe44;
	undefined field12_0x1fe45;
	undefined field13_0x1fe46;
	undefined field14_0x1fe47;
	struct Timer timer1;
	struct Timer timer2;
	struct RECORDING rec;
	struct TextStruct txtStruct;
	struct AUDIO audio;
	int po4procSelecter;
	int po4_unk23d84;
	int po4_unk23d88;
	char po4flagSceneStart;
	char po4flagSceneEnd;
	undefined field26_0x23d8e;
	undefined field27_0x23d8f;
	int po4nextProc;
	int po4sceneTimerID	;
	int po4sceneTimerIDNext;
	int po4sceneFadeout;
	int po4cur_song;
	undefined field33_0x23da4;
	undefined field34_0x23da5;
	undefined field35_0x23da6;
	undefined field36_0x23da7;
	char po4_23da8;
	char po4_23da9;
	undefined field39_0x23daa;
	undefined field40_0x23dab;
	int po4_hThread_ParseBMS;
	int po4MainMenuCursor;
	int procSelecter; /* 2:select 3:deciide 4:play 5:result 6:keyconfig 7:skinselect */
	int procPhase;
	int hThreadBanner;
	struct gameplay gameplay;
	CRITICAL_SECTION criticalSection;
	undefined field48_0xa002c;
	undefined field49_0xa002d;
	undefined field50_0xa002e;
	undefined field51_0xa002f;
	char is_clicked_screenModeChange;
	undefined field53_0xa0031;
	undefined field54_0xa0032;
	char field55_0xa0033;
	int isSkipDrawTick; /* skip frame? */
	int flag_Screenshot; /* char */
	char flag_unk420;
	char flag_showFPS;
	char cmd_nosave;
	int cmd_directplay;
	char field62_0xa0043;
	char cmd_auto;
	char cmd_n;
	char is_recordmode;
	char auto2avi;
	char flag_unkA0048;
	char flag_unkA0049;
	undefined field69_0xa004a;
	undefined field70_0xa004b;
	CSTR field71_0xa004c;
	CSTR directoryPath;
	CSTR directoryFilename;
	CSTR field74_0xa0058[10];
	CSTR baseDirectory;
	int is_starter;
	struct NETWORK net;
};

struct SkinHeader { /* SkinInfo */
	CSTR skinFile;
	CSTR thumbnail;
	CSTR title;
	CSTR maker;
	enum SKINTYPE type;
	int undefined1;
	int unused18;
	int informationP5;
	struct SkinCustom customs[100];
	int custom_count;
};

struct RANKINGPLAYER {
	CSTR name;
	int id;
	int sp;
	int dp;
	int clear;
	int notes;
	int combo;
	int pg;
	int gr;
	int gd;
	int bd;
	int pr;
	int minbp;
	int option;
	int sussussuspected;
	int playcount;
	int ranking;
	CSTR comment; /* hash */
};

typedef struct glb_dbgame glb_dbgame, *Pglb_dbgame;

struct glb_dbgame {
	struct sqlite3 * pSql;
	struct game * pGame;
};

typedef struct SkinUser SkinUser, *PSkinUser;

struct SkinUser {
	struct SkinAdjust adjust;
	int customize_value[40];
	CSTR customize_filename[40];
};

typedef struct CHARTCONVERTER CHARTCONVERTER, *PCHARTCONVERTER;

typedef struct struct_0x14 struct_0x14, *Pstruct_0x14;

typedef struct struct_0x14_2 struct_0x14_2, *Pstruct_0x14_2;

struct struct_0x14 {
	int ID;
	CSTR filenameHead;
	int count;
	int field3_0xc;
	int field4_0x10;
};

struct struct_0x14_2 {
	int soundLoadID;
	undefined4 field1_0x4;
	undefined4 field2_0x8;
	undefined4 field3_0xc;
	undefined4 field4_0x10;
};

struct CHARTCONVERTER {
	struct struct_0x14 arr1[1296];
	struct struct_0x14 arr2[1296];
	int arr1count;
	int field3_0xca84[1296];
	struct struct_0x14_2 arr3[1296];
	int unused14404;
	int noteCountPerLane[7];
	int laneCount;
	int unk14428;
	int unk1442c;
	int unk14430;
	char flagSplitScratch;
	undefined field12_0x14435;
	undefined field13_0x14436;
	undefined field14_0x14437;
	int unk14438;
	int RealTimingSplitScratch;
	char flagSplitUnknown;
	char flagSplit;
	undefined field19_0x14442;
	undefined field20_0x14443;
	int assist1p;
	int assist2p;
	int playlevel;
};

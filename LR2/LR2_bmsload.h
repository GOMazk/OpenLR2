#include "structure.h"

int InitNoteBuffer(LaneStruct *lane, int count);
int ExpandNoteBuffer(LaneStruct * lane, int addsize);

int PlayerCheckAndSwap(gameplay * gp);
int InitGameplay(gameplay *gp, CONFIG_PLAY *cfg);
int InitGameplay_retry(gameplay *gp, AUDIO *snd, game *g);

//resource
int LoadBmsResource(gameplay *gp, CSTR BMSfilepath, AUDIO *aud, ConfigStruct *cfg, BMSMETA *meta, char bga, char flip, char noVideo);
void ProcLoadBmsResource(game *g);

int StopAllKeysound(game *g);
int InitKeysound(game *g);
int ReleaseBGA(game *g);


//notes
bool isVisibleNote(int ch);
double RealTimeToBMSTime(gameplay *gp, double time);
double RealTimeToRenderTime(gameplay *gp, double time);

int CMP_NotesByBmsTiming(const void * p1, const void * p2);
int CMP_NotesByRealTiming(const void * p1, const void * p2);
int CMP_NotesByRealTimingOp(const void * p1, const void * p2);

int CMP_CCARRbyCount(const void *p1, const void *p2);
int CMP_CCARRbyID(const void *p1, const void *p2);

int SplitNotesToDP(LaneStruct *lane, int start, CHARTCONVERTER *cc, int end);
int RightLaneTo2P(LaneStruct *lane, int start, CHARTCONVERTER *cc);
int Move3rdLaneTo2P(LaneStruct *lane, int start, CHARTCONVERTER *cc);
int DPsplitLane(LaneStruct *lane, int start, CHARTCONVERTER *cc);
int DPsplit(LaneStruct *lane, int start, CHARTCONVERTER *cc);
void MakeExtraChart(gameplay *gp, CHARTCONVERTER *cc);
void DPtoSP(gameplay *gp);
void PMStoSP(gameplay *gp);
int DPsplitLaneScratch(LaneStruct *lane, int start, CHARTCONVERTER *cc);
int SPtoDP(LaneStruct * lane, int baseNoteID, CHARTCONVERTER * cc);

int ParseBmsFile(gameplay * gp, CSTR filename, AUDIO * aud, ConfigStruct * cfg, BMSMETA * meta, int bgaFlag, int scratchSide);

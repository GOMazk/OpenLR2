#include "structure.h"

//DrawingBuf collects what will be drawn
int InitDrawingBuffer(DrawingBuf * drb);
int AllocDrawingBuffer(DrawingBuf * drb);
int ReallocDrawingBuffer(DrawingBuf * drb);

//calc draw position
int InitDSTdraw(DSTdraw * dstd);
DSTdraw DSTDbyTime(DSTdraw * dstd1, DSTdraw * dstd2, double t1, double t2, double tO);
DSTdraw SetDSTdrawByTime(DSTstruct dst, double time);

//skinobj into DrawingBuf
int GetSRCcycleNow(SRCstruct src, double time);

int AddDrawingBufferT(DrawingBuf * drb, int grHandle, DSTdraw * dstd);
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

//DrawingBuf to backscreen (later ScreenFlip() will push backscreen into screen)
int LRDrawImg(int * grHandle, DSTdraw * dstD);

bool IsMultibyte(byte ch);
int GetTextGraphLength(CSTR * str, ImageFont * imF);
void LRDrawText(int * grHandle, DSTdraw * dstd, CSTR * str, ImageFont * imF);
void LRDrawTextInput(int * hFont, DSTdraw * dstd, int * hInput, ImageFont * imgfont);

int sortDST(const void * a, const void * b);
int LRDraw(DrawingBuf * drBuf, TextStruct * txt, SONGSELECT * sSel, skstruct * sks, int targetBufNum, int x, int y);

int DrawBGA(int grHandle);
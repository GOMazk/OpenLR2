#include "structure.h"

//imagefont
int InitImageFont(ImageFont * imgfont);
int ReadImageFont(CSTR filename, ImageFont * imgfont);

//imagefont img load on draw
int LoadFontGraph(ImageFont * imgfont, int * fontNum);
int LoadFontCharGraph(ImageFont * imgfont, ushort vChar);
int LoadFontForText(ImageFont * imgfont, CSTR * str);

//skinobj
int InitSRC(SRCstruct * src);
int InitDST(DSTstruct * dst);

int ReadSRC(SRCstruct * src, CSVbuf * csv, skstruct * sk);
int ReadSRC_BAR_TITLE(SRCstruct * src, CSVbuf * csv, skstruct * sk);
int ReadDST(DSTstruct * dst, CSVbuf * csv, int order);

//skin / scene
int InitSkin(skstruct * sk, int p5, char font);
bool CheckIndexRange(int index, int min, int max, int line, char * str);
int ExpandSkinObjectMax(SkinObject * so, int add);
int FlipSide_Timer(int * n);
int ApplyFlipside(skstruct * sk);
int ReadSkin(skstruct * sk, CSTR FilePath, int unused, int skin_num, SkinUser * sku, char flag_skipFont);

int LoadScene(skstruct * sk, CSTR skinfile, int p5, char font);

int ClearSkinGraph(skstruct * sk);



// //drawing
// bool IsMultibyte(byte ch);
// double ChangeValueByTime(double val1, double val2, double time1, double time2, double timenow, int type);
// DSTdraw SetDSTdrawByTime(DSTstruct dst, double time);
// int GetSRCcycleNow(SRCstruct src, double time);
// int LRDrawImg(int * grHandle, DSTdraw * dstD);
// int GetTextGraphLength(CSTR * str, ImageFont * imF);
// void LRDrawText(int * grHandle, DSTdraw * dstd, CSTR * str, ImageFont * imF);
// void LRDrawTextInput(int * hFont, DSTdraw * dstd, int * hInput, ImageFont * imgfont);
// int AddDrawingBufferT(DrawingBuf * drb, int grHandle, DSTdraw * dstd);
// int sortDST(const void * a, const void * b);
// int LRDraw(DrawingBuf * drBuf, TextStruct * txt, SONGSELECT * sSel, skstruct * sks, int targetBufNum, int x, int y);
// int InitDrawingBuffer(DrawingBuf * drb);
// int MouseOnDSTD(DSTdraw * dstd, int * x, int * y);
// int MouseOnObject(DSTstruct * dst, Timer * T, int * x, int * y);
// int SliderByTime(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int min, int max, int * value, inputStructure * input, int objectID);
// int ButtonByInput(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, inputStructure * input, int * target, int min, int max, int panel);
// int DrawBGA(int grHandle);
// int AllocDrawingBuffer(DrawingBuf * drb);
// int ReallocDrawingBuffer(DrawingBuf * drb);
// double ByTime(double v1, double v2, double t1, double t2, double tO);
// DSTdraw DSTDbyTime(DSTdraw * dstd1, DSTdraw * dstd2, double t1, double t2, double tO);
// int AddDrawingBuffer_Text(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T);
// int AddDrawingBuffer_TextXY(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int handle, int x, int y);
// int AddDrawingBuffer(DrawingBuf * drb, int grHandle, DSTdraw * dstd);
// int AddDrawingBuffer_BarGraph(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, double min, double max, double val);
// int AddDrawingBuffer_Image(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T);
// int AddDrawingBuffer_OnMouse(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, inputStructure * input, int panel);
// int AddDrawingBuffer_LN(DrawingBuf * drb, SRCstruct * srcLs, SRCstruct * srcLe, SRCstruct * srcLb, DSTstruct * dst, Timer * T, float shiftX, float shiftY, float longY, int alpha, float sizeX, float sizeY);
// int AddDrawingBuffer_PlayArea(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, float shiftX, float shiftY, int alpha, float sizeX, float sizeY, char flag);
// int AddDrawingBuffer_Gauge(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int hp, char isSurvival);
// int AddDrawingBuffer_BGA(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int grHandle, char flag);
// int AddDrawingBuffer_Scratch(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int angle);
// int AddDrawingBuffer_Image2(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T);
// int AddDrawingBuffer_Object(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int x, int y);
// int AddDrawingBuffer_ObjectAlpha(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int x, int y, int a);
// int AddDrawingBuffer_EventLoading(DrawingBuf * drb, int grHandle, DSTstruct * dst, Timer * T, int x, int y);
// int AddDrawingBuffer_Lunaris(DrawingBuf * drb, SRCstruct * src, DSTdraw * dstd, Timer * T);
// int AddDrawingBuffer_Numbers(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int number, int x, int y);
// int AddDrawingBuffer_Slider(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T);
// int AddDrawingBuffer_JudgeCombo(DrawingBuf * drb, SRCstruct * jSrc, DSTstruct * jDst, SRCstruct * cSrc, DSTstruct * cDst, Timer * T, int combo, int optX, int optY);

// //loading
// int ReloadImage(CSTR filename, int * grHandle);
// int InitSRC(SRCstruct * src);
// int InitDST(DSTstruct * dst);
// int InitDSTdraw(DSTdraw * dstd);
// int ReadDST(DSTstruct * dst, CSVbuf * csv, int order);
// int ReadSRC(SRCstruct * src, CSVbuf * csv, skstruct * sk);
// bool CheckIndexRange(int index, int min, int max, int line, char * str);
// int ReadSRC_BAR_TITLE(SRCstruct * src, CSVbuf * csv, skstruct * sk);
// int ScreenCapture(uint iGrHandle);
// int InitSkin(skstruct * sk, int p5, char font);
// int InitImageFont(ImageFont * imgfont);
// int ReadImageFont(CSTR filename, ImageFont * imgfont);
// int LoadFontGraph(ImageFont * imgfont, int * fontNum);
// int LoadFontCharGraph(ImageFont * imgfont, ushort vChar);
// int LoadFontForText(ImageFont * imgfont, CSTR * str);
// int FlipSide_Timer(int * n);
// int ApplyFlipside(skstruct * sk);
// int ClearSkinGraph(skstruct * sk);
// int ExpandSkinObjectMax(SkinObject * so, int add);
// int ReadSkin(skstruct * sk, CSTR FilePath, int unused, int skin_num, SkinUser * sku, char flag_skipFont);


// int LoadScene(skstruct * sk, CSTR skinfile, int p5, char font);
#include "structure.h"

//draw if
bool GetOptionFlag_dst(game *gs, int option);

//only drawing
uint SetObjectValue_Num(game * g, int op);
int SetObjectValue_Bargraph(game * g);

//interactive
int SetObjectValue_Slider(game *g, skstruct *sk, Timer *T, char flag);
int SetObjectValue_Button(game *g, skstruct *sk, Timer *T, char flag);

//
int MouseOnDSTD(DSTdraw * dstd, int * x, int * y);
int MouseOnObject(DSTstruct * dst, Timer * T, int * x, int * y);
int SliderByTime(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, int min, int max, int * value, inputStructure * input, int objectID);
int ButtonByInput(DrawingBuf * drb, SRCstruct * src, DSTstruct * dst, Timer * T, inputStructure * input, int * target, int min, int max, int panel);

//text input
int Proc_Text(game *g, sqlite3 *sql, char flag);

//text
int InitObjectString(TextStruct * txt);
int SetObjectString(uint num, CSTR string, CSTR * objectList);
int SetObjectStringInt(int at, int val, CSTR * arr);
CSTR GetStringFromArray(int num, CSTR * strings);

//panel constant text (options in songselect)
int DefineOptionStrNum(OptionString * arrOpStr);
int ReadOptionstr(OptionString * opStr, CSVbuf csv);
int ReadOptionstrFile(OptionString * arrOpStr, CSTR filepath);

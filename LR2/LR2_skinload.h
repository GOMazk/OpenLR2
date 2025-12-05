#include "structure.h"

//imagefont
int InitImageFont(ImageFont * imgfont);
int ReadImageFont(CSTR filename, ImageFont * imgfont);

//imagefont img load on draw
int LoadFontGraph(ImageFont * imgfont, int fontNum);
int LoadFontCharGraph(ImageFont * imgfont, char32_t vChar);
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

int LoadScene(skstruct * sk, CSTR skinfile, int p5 = 0, char font = 0);
int LoadSceneG(game* g, skstruct* sk, int skinNum, int font = 0);


int ClearSkinGraph(skstruct * sk);
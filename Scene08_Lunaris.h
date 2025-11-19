#include "structure.h"

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

int ProcI_Lunaris(game * g);
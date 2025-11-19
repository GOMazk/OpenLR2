#include "Engine.h"
int ProcS_Keyconfig(game *g);
int ProcI_Keyconfig(game *g);

//ui and button mapping
CSTR GetKeyIDname(int keyID);

int ConfigButtonToKeyID7(int buttonID);
int ConfigButtonToKeyID5(int buttonID);
int ConfigButtonToKeyID9(int buttonID);

int ConfigButtonFromKeyID7(int keyID);
int ConfigButtonFromKeyID5(int keyID);
int ConfigButtonFromKeyID9(int keyID);
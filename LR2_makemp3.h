#include "structure.h"

int RunMP3Encoder(ConfigStruct * cfg, CSTR wavPath, CSTR mp3Path, char deleteWav, char movie);
int Proc_Auto2avi(game * g, CSTR directory, CSTR filename); //not avi, this is song record
int RecordBmsSound(game * g, CSTR oPath);
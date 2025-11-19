#include "structure.h"

int SkinSelect_SoundSet(game * g, CSTR filepath);
int ProcI_SkinSelect(game * g);
int MakeSkinPreview(game * g, skstruct * sk, SkinManage * sm);
int PlayPreviewSample(game *g);
int ProcS_SkinSelect(game * g);

int SkinPreviewNext(SkinManage *sm, SKINTYPE type);
int SkinPreviewPrev(SkinManage *sm, SKINTYPE type);
int SelectSkin(SkinManage * sm, CSTR * obuf);

#pragma once
#include "structure.h"

int InitSkinData(SkinManage * skm);
int ExpandSkinMax(SkinManage * skm);
//find and make list
int ParseLR2SkinCustom(SkinManage * skm, CSTR filepath);
int MakeSkinList(SkinManage * skm, CSTR dir);

//set initial skins
int SetFirstSkin(SkinManage * sm, SKINTYPE type, CSTR * skinName);
int SetFirstSkin_5k(SkinManage * sm, SKINTYPE type, CSTR * skinName);
int SetFirstSkin_10k(SkinManage * sm, SKINTYPE type, CSTR * skinName);
int SetFirstSkin_5kb(SkinManage * sm, SKINTYPE type, CSTR * skinName);
int SetFirstSkins(game * g);
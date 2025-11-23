#include "LR2_skinmanage.h"
#include "Engine.h"
#include "LR2_configsave.h"


//maybe deleted by compiler, restored it for convenience
int SetFirstSkin(SkinManage *sm, SKINTYPE type, CSTR *skinName) {

	sm->skinID[type] = -1;
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].skinFile.isSame(skinName) && sm->Data[i].type == type) {
			sm->skinID[type] = i;
			ErrorLogFmtAdd("SetFirstSkin 正しくスキン番号を設定しました。種別%d パス%s 番号%d\n", type, skinName->body, i);
			return 1;
		}
	}

	ErrorLogFmtAdd("SetFirstSkin()で該当のタイプのスキンが見つかりませんでした。種別%d パス%s\n", type, skinName->body);
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].type == type) {
			skinName->assign(&sm->Data[i].skinFile);
			sm->skinID[type] = i;
			return 1;
		}
	}
	return -1;
}

//409c80
int SetFirstSkin_5k(SkinManage *sm, SKINTYPE type, CSTR *skinName){
	
	sm->skinID[type] = -1;
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].skinFile.isSame(skinName) &&
			(sm->Data[i].type == type || (sm->Data[i].type == type - 1 && (type == SKINTYPE_5KEYS || type == SKINTYPE_10KEYS || type == SKINTYPE_7KEYSBATTLE)))) {
			sm->skinID[type] = i;
			ErrorLogFmtAdd("SetFirstSkin 正しくスキン番号を設定しました。種別%d パス%s 番号%d\n", type, skinName->body, i);
			return 1;
		}
	}
	ErrorLogFmtAdd("SetFirstSkin()で該当のタイプのスキンが見つかりませんでした。種別%d パス%s\n", type, skinName->body);
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].type == type || (sm->Data[i].type == type -1 && (type == SKINTYPE_5KEYS || type == SKINTYPE_10KEYS || type == SKINTYPE_7KEYSBATTLE) )) {
			skinName->assign(&sm->Data[i].skinFile);
			sm->skinID[type] = i;
			return 1;
		}
	}
	return -1;
}

//maybe deleted by compiler, restored it for convenience
int SetFirstSkin_10k(SkinManage *sm, SKINTYPE type, CSTR *skinName) {

	sm->skinID[type] = -1;
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].skinFile.isSame(skinName) && (sm->Data[i].type == SKINTYPE_10KEYS || sm->Data[i].type == SKINTYPE_14KEYS)) {
			sm->skinID[type] = i;
			ErrorLogFmtAdd("SetFirstSkin 正しくスキン番号を設定しました。種別%d パス%s 番号%d\n", type, skinName->body, i);
			return 1;
		}
	}
	ErrorLogFmtAdd("SetFirstSkin()で該当のタイプのスキンが見つかりませんでした。種別%d パス%s\n", type, skinName->body);
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].type == SKINTYPE_10KEYS || sm->Data[i].type == SKINTYPE_14KEYS) {
			skinName->assign(&sm->Data[i].skinFile);
			sm->skinID[type] = i;
			return 1;
		}
	}
	return -1;
}

//maybe deleted by compiler, restored it for convenience
int SetFirstSkin_5kb(SkinManage *sm, SKINTYPE type, CSTR *skinName) {

	sm->skinID[type] = -1;
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].skinFile.isSame(skinName) && (sm->Data[i].type == 13 || sm->Data[i].type == 12)) {
			sm->skinID[type] = i;
			ErrorLogFmtAdd("SetFirstSkin 正しくスキン番号を設定しました。種別%d パス%s 番号%d\n", type, skinName->body, i);
			return 1;
		}
	}
	ErrorLogFmtAdd("SetFirstSkin()で該当のタイプのスキンが見つかりませんでした。種別%d パス%s\n", type, skinName->body);
	for (int i = 0; i < sm->Count; i++) {
		if (sm->Data[i].type == 13 || sm->Data[i].type == 12) {
			skinName->assign(&sm->Data[i].skinFile);
			sm->skinID[type] = i;
			return 1;
		}
	}
	return -1;
}

//409d80
int SetFirstSkins(game *g){
	SkinManage *sm;

	ErrorLogAdd("スキンを列挙します。\n");
	sm = &g->skinData;
	InitSkinData(sm);
	MakeSkinList(sm, CSTR("LR2files\\Theme\\"));
	MakeSkinList(sm, CSTR("LR2files\\Sound\\"));
	ErrorLogAdd("スキンの列挙を終了しました。\n");

	if (SetFirstSkin(sm, SKINTYPE_7KEYS, &g->config.skin.skinFilePath[0]) == -1) {
		ErrorLogAdd("7keysスキンが有りません。\n");
	}
	if (SetFirstSkin_5k(sm, SKINTYPE_5KEYS, &g->config.skin.skinFilePath[1]) == -1) {
		ErrorLogAdd("5keysスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_14KEYS, &g->config.skin.skinFilePath[2]) == -1) {
		ErrorLogAdd("14keysスキンが有りません。\n");
	}
	if (SetFirstSkin_10k(sm, SKINTYPE_10KEYS, &g->config.skin.skinFilePath[3]) == -1) {
		ErrorLogAdd("14keysスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_9KEYS, &g->config.skin.skinFilePath[4]) == -1) {
		ErrorLogAdd("9keysスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_SELECT, &g->config.skin.skinFilePath[5]) == -1) {
		ErrorLogAdd("selectスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_DECIDE, &g->config.skin.skinFilePath[6]) == -1) {
		ErrorLogAdd("decideスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_RESULT, &g->config.skin.skinFilePath[7]) == -1) {
		ErrorLogAdd("resultスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_KEYCONFIG, &g->config.skin.skinFilePath[8]) == -1) {
		ErrorLogAdd("keyconfigスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_SKINSELECT, &g->config.skin.skinFilePath[9]) == -1) {
		ErrorLogAdd("skinselectスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_SOUNDSET, &g->config.skin.skinFilePath[10]) == -1) {
		ErrorLogAdd("soundsetが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_THEME, &g->config.skin.skinFilePath[11]) == -1) {
		ErrorLogAdd("themeが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_7KEYSBATTLE, &g->config.skin.skinFilePath[12]) == -1) {
		ErrorLogAdd("7keysバトルスキンが有りません。\n");
	}
	if (SetFirstSkin_5kb(sm, SKINTYPE_5KEYSBATTLE, &g->config.skin.skinFilePath[13]) == -1) {
		ErrorLogAdd("5keysバトルスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_9KEYSBATTLE, &g->config.skin.skinFilePath[14]) == -1) {
		ErrorLogAdd("9keysバトルスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_COURSERESULT, &g->config.skin.skinFilePath[15]) == -1) {
		ErrorLogAdd("コースリザルトスキンが有りません。\n");
	}
	if (SetFirstSkin(sm, SKINTYPE_OPENING, &g->config.skin.skinFilePath[16]) == -1) { 
		ErrorLogAdd("オープニングスキンが有りません。\n"); //opening skin
	}
	if (SetFirstSkin(sm, SKINTYPE_MODESELECT, &g->config.skin.skinFilePath[17]) == -1) {
		ErrorLogAdd("モードセレクトスキンが有りません。\n"); //mode select skin
	}
	if (SetFirstSkin(sm, SKINTYPE_MODEDECIDE, &g->config.skin.skinFilePath[18]) == -1) {
		ErrorLogAdd("モード決定スキンが有りません。\n"); //mode decide
	}
	if (SetFirstSkin(sm, SKINTYPE_COURSESELECT, &g->config.skin.skinFilePath[19]) == -1) {
		ErrorLogAdd("コースセレクトスキンが有りません。\n"); //course select
	}
	if (SetFirstSkin(sm, SKINTYPE_COURSEEDIT, &g->config.skin.skinFilePath[20]) == -1) {
		ErrorLogAdd("コース編集スキンが有りません。\n"); //course edit
	}
	//15:course result was here, but I moved it on the correct order
	return 1;
}


//4a7390
int InitSkinData(SkinManage *skm){
	skm->Max = 100;
	skm->Data = (SkinHeader *)malloc(sizeof(SkinHeader) * 100);
	skm->Count = 0;
	memset(skm->Data, 0, skm->Max * sizeof(SkinHeader));
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			skm->Data[i].customs[j].dst_op_start = 0xffffffff;
			skm->Data[i].customs[j].labelCapacity = 0;
			skm->Data[i].customs[j].op_label = (CSTR*)malloc(sizeof(CSTR)*100);
			memset(skm->Data[i].customs[j].op_label + sizeof(CSTR)*skm->Data[i].customs[j].labelCapacity, 0, sizeof(CSTR) * 100);
			skm->Data[i].customs[j].labelCapacity += 100;
		}
	}
	return 1;
}

//4a7450
int ExpandSkinMax(SkinManage *skm){
	skm->Data = (SkinHeader *)realloc(skm->Data, (skm->Max + 100) * 0xb14);
	memset(skm->Data + skm->Max, 0, sizeof(SkinHeader) * 100);
	if (skm->Max < skm->Max + 100) {
		for (int i = skm->Max; i < skm->Max + 100; i++) {
			for (int j = 0; j < 100; j++) {
				skm->Data[i].customs[j].dst_op_start = 0xffffffff;
				skm->Data[i].customs[j].labelCapacity = 0;
				skm->Data[i].customs[j].op_label = (CSTR*)malloc(sizeof(CSTR) * 100);
				memset(skm->Data[i].customs[j].op_label + sizeof(CSTR)*skm->Data[i].customs[j].labelCapacity, 0, sizeof(CSTR) * 100);
				skm->Data[i].customs[j].labelCapacity += 100;
			}
		}
	}
	skm->Max = skm->Max + 100;
	return 1;
}

//4a77a0 ParsLR2SkinCustom
int ParseLR2SkinCustom(SkinManage *skm, CSTR filepath) {
	CSVbuf csvBuf;
	SkinUser skCustom;
	CSTR buffer(260);
	CSTR md5Filepath;
	CSTR fName;
	FILE *pFile;
	char* pBuffer;

	cstrSprintf(&md5Filepath, "LR2files\\SkinCustomize\\%s.xml", MD5str(filepath) );
	ReadSkinCustomize(&skCustom, md5Filepath);
	pFile = fopen(filepath, "r");
	if (!pFile) return 0;

	pBuffer = buffer.outstr();
	for (pBuffer = fgets(pBuffer,256,pFile) ; pBuffer ; pBuffer = fgets(pBuffer, 256, pFile)) {
		buffer.trimWhiteSpace();
		DealWhiteSpace(&buffer);
		if (buffer.left(12).isSame("#INFORMATION")) {
			SplitCSV(buffer, &csvBuf, ",");
			if (csvBuf.val[1] < 0) {
				fclose(pFile);
				return 0;
			}
			skm->Data[skm->Count].type = (SKINTYPE)csvBuf.val[1];
			skm->Data[skm->Count].skinFile.assign( &filepath);
			skm->Data[skm->Count].title.assign(&csvBuf.str[2]);
			skm->Data[skm->Count].maker.assign(&csvBuf.str[3]);
			skm->Data[skm->Count].thumbnail.assign(&csvBuf.str[4]);
			skm->Data[skm->Count].informationP5 = csvBuf.val[5];
			skm->Data[skm->Count].unused18 = -1;
			skm->Count ++;
			if (skm->Count == skm->Max) {
				ExpandSkinMax(skm);
			}
		}
		else if (buffer.left(13).isSame("#CUSTOMOPTION")) {
			SplitCSV(buffer, &csvBuf, ",");
			if (csvBuf.val[2] < 1000) {
				SkinHeader &rSkin = skm->Data[skm->Count - 1];
				SkinCustom &rCustom = rSkin.customs[rSkin.custom_count];

				rCustom.title.assign(&csvBuf.str[1]);
				rCustom.dst_op_start = csvBuf.val[2];
				while (csvBuf.str[rCustom.dst_op_count + 3].length() > 0) {
					rCustom.op_label[rCustom.dst_op_count].assign(&csvBuf.str[rCustom.dst_op_count + 3]);
					rCustom.dst_op_count++;
				}

				if (skCustom.customize_value[rSkin.custom_count] < rCustom.dst_op_start	|| rCustom.dst_op_start + rCustom.dst_op_count <= skCustom.customize_value[rSkin.custom_count]) 
					skCustom.customize_value[rSkin.custom_count] = rCustom.dst_op_start;

				skCustom.customize_filename[rSkin.custom_count].assign("ERROR");
				rSkin.custom_count++;
			}
		}
		else if (buffer.left(11).isSame("#CUSTOMFILE")) {
			FILE *pFile;
			HANDLE hFindFile, hFindFileOld;
			_WIN32_FIND_DATAA findFileData;
			int flag;
			SkinHeader &rSkin = skm->Data[skm->Count - 1];
			SkinCustom &rCustom = rSkin.customs[rSkin.custom_count];
			
			SplitCSV(buffer, &csvBuf, ",");
			rCustom.title.assign(&csvBuf.str[1]);
			rCustom.dst_op_start = 0;
			hFindFile = FindFirstFileA(csvBuf.str[2], &findFileData);
			do {
				flag = 0; //logic arranged
				if ( (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) 	flag = 2;
				else if (strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName)) flag = 1;

				fName.assign(findFileData.cFileName);
				if (fName.right(3).isDiff("txt") && flag) {
					if (flag == 2) fName.nullAtPos( fName.findStrPos(".") );
					rCustom.op_label[rCustom.dst_op_count].assign(&fName);
					rCustom.dst_op_count++;
				}
				if (rCustom.dst_op_count == rCustom.labelCapacity-2) {
					rCustom.op_label = (CSTR*)realloc( rCustom.op_label, sizeof(CSTR)*(rCustom.labelCapacity + 100) );
					memset(&rCustom.op_label[rCustom.labelCapacity],0,sizeof(CSTR)*100);
					rCustom.labelCapacity += 100;
				}
			} while (FindNextFileA(hFindFile,&findFileData));
			FindClose(hFindFile);
			rCustom.op_label[rCustom.dst_op_count].assign("RANDOM");
			rCustom.dst_op_count++;

			bool a = true;
			if (rCustom.dst_op_count < 1) {
				skCustom.customize_filename[rSkin.custom_count].assign(&csvBuf.str[3]);
			}
			else {
				for (int i = 0; i < rCustom.dst_op_count; i++) {
					if (rCustom.op_label[i].isSame(&skCustom.customize_filename[rSkin.custom_count]))
						a = false;
				}
				if(a) skCustom.customize_filename[rSkin.custom_count].assign(&csvBuf.str[3]);
			}
			skCustom.customize_value[rSkin.custom_count] = 0;
			rSkin.custom_count++;
		}
		else if (buffer.left(11).isSame("#ENDOFHEADER")) {
			break;
		}
		pBuffer = buffer.outstr();
	}
	fclose(pFile);
	WriteSkinCustomizeXml(&skCustom, md5Filepath);
	return 0;
}

//4a8230 MakeSkinList
int MakeSkinList(SkinManage *skm, CSTR dir) {
	HANDLE hFindFile, hFindFileOld;
	_WIN32_FIND_DATAA findFileData;
	CSTR filter;
	bool isLR2Skin;
	if (dir.right(1).isDiff("\\")) 
		dir.add("\\");
	
	filter.assign(&dir).add("*");
	hFindFile = FindFirstFileA(filter, &findFileData);
	do{
		if ( (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			filter.assign(&dir).add(findFileData.cFileName);
			isLR2Skin = false;
			if (filter.right(8).isSame(".lr2skin"))		isLR2Skin = true;
			else if (filter.right(6).isSame(".lr2ss"))	isLR2Skin = true;
			
			if (isLR2Skin) ParseLR2SkinCustom(skm, filter); 
		}
		else {  //logic arranged
		if (strcmp("..", findFileData.cFileName) && strcmp(".", findFileData.cFileName)) {
			filter.assign(&dir).add(findFileData.cFileName).add("\\");
			MakeSkinList(skm, filter);
		}
		}
		hFindFileOld = hFindFile;
		if (FindNextFileA(hFindFile, &findFileData) == 0) {
			FindClose(hFindFileOld);
			return 0;
		}
	} while (true);
}
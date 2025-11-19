#include "En_fileutil.h"
#include "lib/md5.h"
#include "DxLib/DxLib.h" //log

//437210
//437260
//4372c0
//437300
//437a40
//437bf0
//437c90
//437cf0
int makeFileHash(LPCSTR filepath, LPCSTR oBuf) {
	//HANDLE hFile;
	//hFile = CreateFileA(filepath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
	//if (hFile == (HANDLE)-1) return -1;

	////not original code
	//BY_HANDLE_FILE_INFORMATION FileInformation;
	//GetFileInformationByHandle(hFile, &FileInformation);
	//unsigned int size = FileInformation.nFileSizeLow;
	//char* buffer = (char*)malloc(size);
	//DWORD readsize;
	//if (buffer != NULL) {
	//	ReadFile(hFile, buffer, size, &readsize, 0);
	//	CloseHandle(hFile);
	//	
	//	MD5byte(&buffer, readsize, (char*)oBuf);

	//	free(buffer);
	//}
	//return 1;

	FILE* pFile;
	pFile = fopen(filepath, "rb");
	if (!pFile)  return -1;
	unsigned char* md5buf = (unsigned char*)md5File(pFile);
	fclose(pFile);

	sprintf((char*)oBuf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", md5buf[0], md5buf[1], md5buf[2], md5buf[3],
		md5buf[4], md5buf[5], md5buf[6], md5buf[7], md5buf[8], md5buf[9], md5buf[10], md5buf[11], md5buf[12], md5buf[13], md5buf[14], md5buf[15]);
	return 1;
}

//TOFIX : convert unixtime
/*
//don't compute with SYSTEMTIME
time_t convertWindowsTimeToUnixTime(_FILETIME ft){
	ULARGE_INTEGER ul{ ft.dwLowDateTime, ft.dwHighDateTime };
	return (unsigned int)((ul.QuadPart - 116444736000000000ULL)/10000000);
}
*/

//437de0
int GetNowUnixtime(void){
	SYSTEMTIME systime;
	_FILETIME filetime;
	int hVal, lVal;
	double dTemp;

	GetLocalTime((LPSYSTEMTIME)&systime);
	systime.wYear = systime.wYear - 369;
	SystemTimeToFileTime(&systime, &filetime);

	dTemp = filetime.dwHighDateTime;
	hVal = (dTemp * -429.4836225);

	dTemp = filetime.dwLowDateTime;
	lVal = (dTemp / 10000000.0);

	return lVal - hVal;
}

//437e90
int GetUnixtimeFromFiletime(FILETIME &filetime) {
	SYSTEMTIME systime;
	int hVal, lVal;
	double dTemp;

	FileTimeToSystemTime(&filetime, &systime);
	systime.wYear = systime.wYear - 369;
	SystemTimeToFileTime(&systime, &filetime);

	dTemp = filetime.dwHighDateTime;
	hVal = (dTemp * -429.4836225);

	dTemp = filetime.dwLowDateTime;
	lVal = (dTemp / 10000000.0);

	return lVal - hVal;
}

//437f00
int GetFileUnixtime(CSTR str) {
	WIN32_FIND_DATA FindFileData;
	LPWIN32_FIND_DATAA lpFindFileData;
	HANDLE hFindFile;

	if ( str.right(1).isSame("\\") ) {
		str.nullAtPos( str.length() - 1 );
	}

	lpFindFileData = (LPWIN32_FIND_DATAA)&FindFileData;
	hFindFile = FindFirstFileA(str, lpFindFileData);
	if (hFindFile == (HANDLE)-1) {
		ErrorLogFmtAdd("ファイルのLR2TIME取得エラー:%sが見つからない\n", str);
		return -1;
	}
	else {
		FindClose(hFindFile);
		return GetUnixtimeFromFiletime(FindFileData.ftLastWriteTime);
	}
}

//438040
CSTR GetRandomFileOnDir(CSTR path, char fOnlyName) {
	CSTR oBuf;
	//CSTR str1,str2,str3;
	WIN32_FIND_DATA FindFileData;
	LPWIN32_FIND_DATAA lpFindFileData;
	HANDLE hFindFile;
	int fileCount = 0;
	CSTR str1( path.left(path.findStrPos("*")) );
	CSTR str2( path.right(path.length() - str1.length() - 1) );
	CSTR str3( str1 );
	str3.add("*");
	hFindFile = FindFirstFileA(str3, (LPWIN32_FIND_DATAA)&FindFileData);
	if (hFindFile == (HANDLE)-1) {
		//oBuf = CSTR("ERROR");
		return CSTR("ERROR");
	}
	else {
		do {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp("..", (char*)FindFileData.cFileName) && strcmp(".", (char*)FindFileData.cFileName)) fileCount++;
			}
		} while (FindNextFileA(hFindFile, (LPWIN32_FIND_DATAA)&FindFileData));
		FindClose(hFindFile);
		if (fileCount > 0) {
			fileCount = GetRand(fileCount - 1);

			hFindFile = FindFirstFileA(str3, (LPWIN32_FIND_DATAA)&FindFileData);
			if (hFindFile != (HANDLE)-1) {
				do {
					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						if (strcmp("..", (char*)FindFileData.cFileName) && strcmp(".", (char*)FindFileData.cFileName)) {
							//LAB_00438327
							int i = 0;
							while (i < fileCount) {
								FindNextFileA(hFindFile, (LPWIN32_FIND_DATAA)&FindFileData);
								if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
									if (strcmp("..", (char*)FindFileData.cFileName) && strcmp(".", (char*)FindFileData.cFileName)) i++;
								}
							}
							FindClose(hFindFile);
							path.assign(&str1);
							path.add((char*)FindFileData.cFileName);
							path.add(&str2);
							if (fOnlyName) {
								//oBuf = CSTR(FindFileData.cFileName);
								return CSTR((char*)FindFileData.cFileName);
							}
							else {
								//oBuf = CSTR(path);
								return CSTR(path);
							}
							return oBuf;
						}
					}
					FindNextFileA(hFindFile, (LPWIN32_FIND_DATAA)&FindFileData);
				} while (true);
			}
		}
		//oBuf = CSTR("ERROR");
		return CSTR("ERROR");
	}
	return oBuf;
}

//438540
bool CheckStringHead(CSTR *bigS, CSTR *head){
	if (bigS->length() <= head->length()) {
		return false;
	}
	return (bigS->left(head->length()).icmp(head) == 0);
}

//4385f0
bool GetStringBodyStr(CSTR *str, CSTR head, CSTR *oBuf){
	if (CheckStringHead(str, &head)) {
		oBuf->assign(str->right(str->length() - head.length() - 1) );
		return true;
	}
	return false;
}

//4386c0
bool GetStringBodyInt(CSTR *str, CSTR haed, int *oBuf) {
	if (CheckStringHead(str, &haed)) {
		*oBuf = atol(str->right(str->length() - haed.length() - 1));
		return true;
	}
	return false;
}

//4387a0
bool GetDifficultyFromToken(CSTR str, CSTR *oLeft, CSTR *oRight, CSTR tokenL, CSTR tokenR, int *oBuf) {
	
	int posL = -1;
	CSTR str1(str);

	//check token existence from rightside
	for (str1.nullAtPos(str1.length() - 1); str1.length() > 1; str1.nullAtPos(str1.length() - 1)) {
		if (posL != -1) break;
		if (str1.right(tokenL.length()).isSame(&tokenL)) {
			posL = str1.length() - tokenL.length();
		}
	}
	if (posL < 1) return false;
	if (!str.right(tokenR.length()).isSame(&tokenR)) return false;

	oLeft->assign( str.left(posL).trimWhiteSpace() );
	oRight->assign( str.right(str.length() - oLeft->length()).trimWhiteSpace() );
	str1.assign(oRight);
	str1.upper();
	if (str1.findStrPos("BEG") > 0) *oBuf = 1;
	else if (str1.findStrPos("HARD") > 0) *oBuf = 3;
	else if (str1.findStrPos("HYPE") > 0) *oBuf = 3;
	else if (str1.findStrPos("HD") > 0) *oBuf = 3;
	else if (str1.findStrPos("5H") > 0) *oBuf = 3;
	else if (str1.findStrPos("7H") > 0) *oBuf = 3;
	else if (str1.findStrPos("10H") > 0) *oBuf = 3;
	else if (str1.findStrPos("14H") > 0) *oBuf = 3;
	else if (str1.findStrPos("9H") > 0) *oBuf = 3;
	else if (str1.findStrPos("DIF") > 0) *oBuf = 3;
	else if (str1.findStrPos("VERYHARD") > 0) *oBuf = 4;
	else if (str1.findStrPos("EX") > 0) *oBuf = 4;
	else if (str1.findStrPos("AN") > 0) *oBuf = 4;
	else if (str1.findStrPos("SHD") > 0) *oBuf = 4;
	else if (str1.findStrPos("5A") > 0) *oBuf = 4;
	else if (str1.findStrPos("7A") > 0) *oBuf = 4;
	else if (str1.findStrPos("10A") > 0) *oBuf = 4;
	else if (str1.findStrPos("14A") > 0) *oBuf = 4;
	else if (str1.findStrPos("9A") > 0) *oBuf = 4;
	else if (str1.findStrPos("ULT") > 0) *oBuf = 4;
	else if (str1.findStrPos("MANI") > 0) *oBuf = 4;
	else if (str1.findStrPos("LUNA") > 0) *oBuf = 4;
	else if (str1.findStrPos("AHO") > 0) *oBuf = 4;
	else if (str1.findStrPos("AFO") > 0) *oBuf = 4;
	else if (str1.findStrPos("ASDF") > 0) *oBuf = 4;
	else if (str1.findStrPos("HELL") > 0) *oBuf = 4;

	return 1;
}

//438c80
bool GetDifficulty(CSTR *str, CSTR head, CSTR *oLeft, CSTR *oRight, int *pDifficulty) {
	
	if (!CheckStringHead(str, &head)) return false;

	CSTR upstr(*str);
	upstr.upper();

	if (upstr.right(4).isSame("HARD")) *pDifficulty = 2;
	if (upstr.right(5).isSame("HYPER")) *pDifficulty = 3;
	if (upstr.right(7).isSame("ANOTHER")) *pDifficulty = 4;
	if (upstr.right(4).isSame("EASY")) *pDifficulty = 1;
	if (upstr.right(2).isSame("EX")) *pDifficulty = 4;
	if (upstr.right(6).isSame("MANIAC")) *pDifficulty = 4;

	str->lastCut(str->length() - head.length() - 1);
	if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("("), CSTR(")"), pDifficulty)) {} //test this method
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("["), CSTR("]"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("-"), CSTR("-"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("\""), CSTR("\""), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("<"), CSTR(">"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("～"), CSTR("～"), pDifficulty)) {} //? 81 60
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("【"), CSTR("】"), pDifficulty)) {} //【 81 79, 】81 7a
	else { 
		oLeft->assign(str);
		oRight->fillzero();
	}
	return true;
}

//439150
bool IsBmsFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".bme")) return true;
		if (str.isSame(".bms")) return true;
		if (str.isSame(".bml")) return true;
		if (str.isSame(".pms")) return true;
	}
	return false;
}

//439230
bool IsMediaFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".mp3")) return true;
		if (str.isSame(".wav")) return true;
		if (str.isSame(".avi")) return true;
	}
	return false;
}

//439300
bool IsSndFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".mp3")) return true;
		if (str.isSame(".wav")) return true;
	}
	return false;
}

//4393b0
bool IsAviFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".avi")) return true;
	}
	return false;
}

//439450
bool IsLR2Folder(CSTR str) {
	if (str.length() > 10) {
		str.lastCut(10);
		str.lower();
		if (str.isSame(".lr2folder")) return true;
	}
	return false;
}

//439510
bool IsFileExist(CSTR path) {
	HANDLE hFindFile;
	_WIN32_FIND_DATAA findFileData;
	bool isDir = false;
	char *cur;
	char dirFlag = 0;

	if (path.right(1).isSame("\\") == true) {
		isDir = true;
	}
	else if (path.right(1).isSame("/") == true) {
		isDir = true;
	}

	if (isDir) {
		cur = path.atPos(path.length() - 1);
		*cur = 0;
	}
	hFindFile = FindFirstFileA(path, &findFileData);
	FindClose(hFindFile);
	/*if (hFindFile != (HANDLE)-1) {
		FindClose(hFindFile);
	}
	else {
		FindClose(-1);
	}*/
	return hFindFile != (HANDLE)-1;
}

//4396b0 //0:already_exist 1:not_exist 2:changed
int IsFileChanged(unsigned int oldUnixtime, CSTR filepath, int *oNewtime) { 

	HANDLE hFindFile;
	char* lpFileName;
	_WIN32_FIND_DATAA findFileData;

	if (filepath.right(1).isSame("\\")) {
		filepath.nullAtPos(filepath.length() - 1);
	}
	lpFileName = filepath;
	hFindFile = FindFirstFileA(lpFileName, &findFileData);
	if (hFindFile == (HANDLE)-1) {
		*oNewtime = -1;
		FindClose((HANDLE)-1);
		return 1;
	}
	
	unsigned int filetime = GetUnixtimeFromFiletime(findFileData.ftLastWriteTime);
	if (oldUnixtime < filetime) {
		*oNewtime = filetime;
		FindClose(hFindFile);
		return 2;
	}
	else {
		*oNewtime = oldUnixtime;
		FindClose(hFindFile);
		return 0;
	}
}

//439820
int DealWhiteSpace(CSTR *str) {
	bool bFlag = false;
	do{
		if (str->right(1).isSame(" ")) 	bFlag = true;
		else if (str->right(1).isSame("\t")) bFlag = true;
		else if (str->right(1).isSame("\n")) bFlag = true;
		else if (str->right(1).isSame("\r")) bFlag = true;
		else bFlag = false;

		if (bFlag == false) break;
		str->nullAtPos(str->length() - 1);
	} while (true);
	while (true) {
		if (str->left(1).isSame(",")) {
			if (str->length() < 3) return 1;
		}
		else {
			return 1;
		}
		str->lastCut(str->length() - 1);
	}
}

//439a30 SplitCSV
int SplitCSV(CSTR csvStr, CSVbuf *oBuf, const char *splitter) {
	int pos,i;
	bool bEnd = false;
	
	for (int i = 0; i < 30; i++) {
		oBuf->val[i] = 0;
		oBuf->str[i].fillzero();
	}

	i = 0;
	do {
		pos = csvStr.findStrPos(",");
		if (pos == 0) {
			oBuf->val[i] = 0;
			csvStr.lastCut(csvStr.length() - 1);
		}
		else if (pos < 0) {
			oBuf->str[i].assign(&csvStr);
			oBuf->str[i].left(1).isSame("\"");
			if (oBuf->str[i].left(1).isSame("\"")) {
				if (oBuf->str[i].right(1).isSame("\"")) {
					oBuf->str[i].nullAtPos(oBuf->str[i].length() - 1);
					oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				}
			}
			if (oBuf->str[i].left(1).isSame("!")) {
				oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				oBuf->val[i] = -atol(oBuf->str[i]);
			}
			else {
				oBuf->val[i] = atol(oBuf->str[i]);
			}
			csvStr.lastCut(csvStr.length() - pos - 1);
			bEnd = true;
		}
		else {
			//logic arranged
			oBuf->str[i].assign( csvStr.left(pos) );
			if ( oBuf->str[i].left(1).isSame("\"") ){
				if (oBuf->str[i].right(1).isSame("\"")) {
					oBuf->str[i].nullAtPos(oBuf->str[i].length() - 1);
					oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				}
			}
			if (oBuf->str[i].left(1).isSame("!")) {
				oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				oBuf->val[i] = -atol(oBuf->str[i]);
			}
			else {
				oBuf->val[i] = atol(oBuf->str[i]);
			}
			csvStr.lastCut(csvStr.length() - pos - 1);
		}
		i++;
		if(i>29 || bEnd){
			return 1;
		}
	} while (true);
}

//439dc0
int Base36ToInt(char ch1, char ch2) {
	int ret = 0;

	if (0x61 <= ch1 && ch1 < 0x61 + 26) ret = ch1 - 0x61 + 10;
	else if (0x31 <= ch1 && ch1 < 0x31 + 10) ret = ch1 - 0x30;
	else if (0x41 <= ch1 && ch1 < 0x41 + 26) ret = ch1 - 0x41 + 10;
	ret = ret * 36;

	if (0x61 <= ch2 && ch2 < 0x61 + 26) ret += ch2 - 0x61 + 10;
	else if (0x31 <= ch2 && ch2 < 0x31 + 10) ret += ch2 - 0x30;
	else if (0x41 <= ch2 && ch2 < 0x41 + 26) ret += ch2 - 0x41 + 10;

	return ret;
}

//439e40
int HEXcharToInt(char ch1, char ch2) {
	int ret = 0;

	if (0x61 <= ch1 && ch1 < 0x61 + 6) ret = ch1 - 0x61 + 10;
	else if (0x31 <= ch1 && ch1 < 0x31 + 10) ret = ch1 - 0x30;
	else if (0x41 <= ch1 && ch1 < 0x41 + 6) ret = ch1 - 0x41 + 10;
	ret = ret << 4;

	if (0x61 <= ch2 && ch2 < 0x61 + 6) ret += ch2 - 0x61 + 10;
	else if (0x31 <= ch2 && ch2 < 0x31 + 10) ret += ch2 - 0x30;
	else if (0x41 <= ch2 && ch2 < 0x41 + 6) ret += ch2 - 0x41 + 10;

	return ret;
}

//439ec0
int RoundUp(double val) {
	int ret = val;
	if (ret != val) ret++; //TEST : if not work properly, use ceil from math.h
	return ret;
}

//439ef0
CSTR AssignCRC32(CSTR str) {
	CSTR tmp;
	cstrSprintf(&tmp, "%x", str.CRC32());
	return tmp;
}

//439fa0
CSTR AutomationFactory(){
	switch (GetRand(15)) {
		case 0:
			return CSTR("職権を乱用するRainbow");
		case 1:
			return CSTR("またお前らただし女性限定");
		case 2:
			return CSTR("我こそは通訳つきスピリチュアルズ");
		case 3:
			return CSTR("End of the は食後に使おう");
		case 4:
			return CSTR("ジャンバラヤとyou");
		case 5:
			return CSTR("錆び付いたホタル狩り");
		case 6:
			return CSTR("：！寛大な IN COLOR");
		case 7:
			return CSTR("蹴りたい転落");
		case 8:
			return CSTR("Rainy Placement");
		case 9:
			return CSTR("デビュー戦で奥手");
		case 10:
			return CSTR("秋葉never been mellow");
		case 11:
			return CSTR("静電気焼き鳥ムービー");
		case 12:
			return CSTR("人生とビル・ゲイツ");
		case 13:
			return CSTR("黒鍵の上も歩くRED");
		case 14:
			return CSTR("風雲！寝る");
		case 15:
			return CSTR("オペラ：達人、または解雇");
		default:
			return CSTR("※無断使用ばっかりですが訴えないでください");
	}
}

//43a190
int CountDigit(int num){
	uint ret;

	if (num == 0) {
		return 1;
	}
	if (num < 0) num = -num;
	ret = (num < 0);
	for (; num != 0; num = num / 10) {
		ret++;
	}
	return ret;
}

//43a1d0
int GetDigitNum(int num, int pos) {
	if ((0 < num) && (0 < pos)) {
		if (1 < pos) {
			pos = pos + -1;
			do {
				num = num / 10;
				pos = pos + -1;
			} while (pos != 0);
		}
		return num % 10;
	}
	return 0;
}

//43a230
int FindAltImage(CSTR filename, CSTR dir, CSTR *oBuf) {

	CSTR path;

	path.assign(dir).add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".png") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("png");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".jpg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("jpg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".bmp") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("bmp");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	path.assign(dir).add("..\\").add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".png") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("png");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".jpg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("jpg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".bmp") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("bmp");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	oBuf->fillzero();
	return 0;
}

//43a5f0
int FindAltSound(CSTR filename, CSTR dir, CSTR *oBuf) {

	CSTR path;

	path.assign(dir).add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".ogg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("ogg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".wav") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("wav");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	path.assign(dir).add("..\\").add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".wav") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("wav");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".ogg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("ogg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	oBuf->fillzero();
	return 0;
}

//43a900
CSTR GetRandomFile(CSTR path, char fOnlyName) {
	WIN32_FIND_DATA FindFileData;
	LPWIN32_FIND_DATAA lpFindFileData;
	HANDLE hFindFile;
	CSTR oBuf;
	int count;

	//call function if wildcard is on directory
	if (path.findStrPos("*/") != -1 || path.findStrPos("*\\") != -1 || path.right(1).isSame("*")) {
		return CSTR(GetRandomFileOnDir(path, fOnlyName));
	}

	//count files for random
	hFindFile = FindFirstFileA(path, (LPWIN32_FIND_DATAA)&FindFileData);
	if (hFindFile == (HANDLE)-1) return CSTR("ERROR");
	
	count = 0;
	do {
		count++;
	} while (FindNextFileA(hFindFile, (LPWIN32_FIND_DATAA)&FindFileData));
	if (count < 1) return CSTR("ERROR");

	count = GetRand(count - 1);

	//get file by random
	hFindFile = FindFirstFileA(path, (LPWIN32_FIND_DATAA)&FindFileData);
	if (hFindFile == (HANDLE)-1) return CSTR("ERROR");

	for (int i = 0; i < count; i++) {
		FindNextFile(hFindFile, &FindFileData);
	}
	FindClose(hFindFile);
	path.assign(path.getDirectory());
	path.add((char*)FindFileData.cFileName);
	if (fOnlyName) {
		path.assign((char*)FindFileData.cFileName);
		path.nullAtPos(path.findStrPos("."));
	}
	return CSTR(path);
}

//43abe0
CSTR GetRandomFileNoError(CSTR path, CSTR dir) {
	CSTR filepath;
	filepath.assign(GetRandomFile(path, 0));
	if (filepath.isDiff("ERROR")) return CSTR(filepath);
	dir.add(&path);
	filepath.assign(GetRandomFile(path, 0));
	if (filepath.isDiff("ERROR")) return CSTR(filepath);
	return CSTR(path);
}

//443550 _ need simplification
void MD5byte(char **iStr, uint len, char *oByte){
	int iVar1;
	int iVar2;
	int iVar3;
	int iVar4;
	int iVar5;
	int iVar6;
	int iVar7;
	int iVar8;
	int iVar9;
	int iVar10;
	int iVar11;
	int *piVar12;
	uint uVar13;
	uint uVar14;
	uint uVar15;
	int *piVar16;
	uint uVar17;
	uint uVar18;
	uint uVar19;
	int iVar20;
	int iVar21;
	uint uVar22;
	uint uVar23;
	uint uVar24;

	iVar20 = 0 -(uint)(((len & 0x3f) < 0x38) & 0xffffffc0) - (len & 0x3f);
	iVar21 = iVar20 + 0x78;
	uVar13 = iVar20 + 0x80 + len;
	piVar12 = (int *)realloc(*iStr, uVar13);
	*iStr = (char *)piVar12;
	memset((undefined *)((int)piVar12 + len), 0, iVar21);
	piVar16 = (int *)((int)piVar12 + iVar21 + len);
	*(undefined *)((int)piVar12 + len) = 0x80;
	*piVar16 = len * 8;
	piVar16[1] = len >> 0x1d;
	iVar20 = 0x67452301;
	uVar23 = 0xefcdab89;
	uVar19 = 0x98badcfe;
	uVar17 = 0x10325476;
	for (uVar13 = uVar13 >> 6; uVar13 != 0; uVar13 = uVar13 - 1) {
		iVar21 = iVar20 + (~uVar23 & uVar17 | uVar19 & uVar23) + *piVar12;
		uVar14 = (iVar21 + 0xd76aa478U >> 0x19 | (iVar21 + -0x955b88) * 0x80) + uVar23;
		iVar21 = uVar17 + (~uVar14 & uVar19 | uVar23 & uVar14) + piVar12[1];
		uVar15 = (iVar21 + 0xe8c7b756U >> 0x14 | (iVar21 + 0x7b756) * 0x1000) + uVar14;
		iVar21 = uVar19 + (~uVar15 & uVar23 | uVar15 & uVar14) + piVar12[2];
		uVar22 = (iVar21 + 0x242070dbU >> 0xf | (iVar21 + -0xf25) * 0x20000) + uVar15;
		iVar21 = uVar23 + (~uVar22 & uVar14 | uVar15 & uVar22) + piVar12[3];
		uVar18 = ((iVar21 + -0x112) * 0x400000 | iVar21 + 0xc1bdceeeU >> 10) + uVar22;
		iVar21 = uVar14 + (~uVar18 & uVar15 | uVar22 & uVar18) + piVar12[4];
		uVar24 = (iVar21 + 0xf57c0fafU >> 0x19 | (iVar21 + -0x83f051) * 0x80) + uVar18;
		iVar21 = uVar15 + (~uVar24 & uVar22 | uVar18 & uVar24) + piVar12[5];
		uVar14 = (iVar21 + 0x4787c62aU >> 0x14 | (iVar21 + 0x7c62a) * 0x1000) + uVar24;
		iVar21 = uVar22 + (~uVar14 & uVar18 | uVar14 & uVar24) + piVar12[6];
		uVar15 = (iVar21 + 0xa8304613U >> 0xf | (iVar21 + -0x39ed) * 0x20000) + uVar14;
		iVar21 = uVar18 + (~uVar15 & uVar24 | uVar14 & uVar15) + piVar12[7];
		uVar18 = ((iVar21 + 0x101) * 0x400000 | iVar21 + 0xfd469501U >> 10) + uVar15;
		iVar21 = uVar24 + (~uVar18 & uVar14 | uVar15 & uVar18) + piVar12[8];
		uVar22 = (iVar21 + 0x698098d8U >> 0x19 | (iVar21 + -0x7f6728) * 0x80) + uVar18;
		iVar21 = uVar14 + (~uVar22 & uVar15 | uVar18 & uVar22) + piVar12[9];
		uVar24 = (iVar21 + 0x8b44f7afU >> 0x14 | (iVar21 + 0x4f7af) * 0x1000) + uVar22;
		iVar21 = uVar15 + (~uVar24 & uVar18 | uVar24 & uVar22) + piVar12[10];
		uVar14 = (iVar21 - 0xa44fU >> 0xf | (iVar21 + -0x244f) * 0x20000) + uVar24;
		iVar21 = uVar18 + (~uVar14 & uVar22 | uVar24 & uVar14) + piVar12[0xb];
		uVar15 = (iVar21 + 0x895cd7beU >> 10 | (iVar21 + -0x42) * 0x400000) + uVar14;
		iVar21 = uVar22 + (~uVar15 & uVar24 | uVar14 & uVar15) + piVar12[0xc];
		uVar18 = (iVar21 + 0x6b901122U >> 0x19 | (iVar21 + -0x6feede) * 0x80) + uVar15;
		iVar21 = uVar24 + (~uVar18 & uVar14 | uVar15 & uVar18) + piVar12[0xd];
		uVar22 = ((iVar21 + -0x78e6d) * 0x1000 | iVar21 + 0xfd987193U >> 0x14) + uVar18;
		iVar21 = uVar14 + (~uVar22 & uVar15 | uVar22 & uVar18) + piVar12[0xe];
		uVar24 = (iVar21 + 0xa679438eU >> 0xf | (iVar21 + -0x3c72) * 0x20000) + uVar22;
		iVar21 = uVar15 + (~uVar24 & uVar18 | uVar22 & uVar24) + piVar12[0xf];
		uVar14 = (iVar21 + 0x49b40821U >> 10 | (iVar21 + 0x21) * 0x400000) + uVar24;
		iVar21 = uVar18 + (~uVar22 & uVar24 | uVar22 & uVar14) + piVar12[1];
		uVar18 = ((iVar21 + -0x1e1da9e) * 0x20 | iVar21 + 0xf61e2562U >> 0x1b) + uVar14;
		iVar21 = uVar22 + (~uVar24 & uVar14 | uVar24 & uVar18) + piVar12[6];
		uVar15 = (iVar21 + 0xc040b340U >> 0x17 | (iVar21 + -0x3f4cc0) * 0x200) + uVar18;
		iVar21 = uVar24 + (~uVar14 & uVar18 | uVar15 & uVar14) + piVar12[0xb];
		uVar22 = ((iVar21 + -0x1a5af) * 0x4000 | iVar21 + 0x265e5a51U >> 0x12) + uVar15;
		iVar21 = uVar14 + (~uVar18 & uVar15 | uVar22 & uVar18) + *piVar12;
		uVar24 = (iVar21 + 0xe9b6c7aaU >> 0xc | (iVar21 + 0x7aa) * 0x100000) + uVar22;
		iVar1 = piVar12[5];
		iVar21 = uVar18 + (~uVar15 & uVar22 | uVar15 & uVar24) + iVar1;
		uVar14 = ((iVar21 + -0x1d0efa3) * 0x20 | iVar21 + 0xd62f105dU >> 0x1b) + uVar24;
		iVar2 = piVar12[10];
		iVar21 = uVar15 + (~uVar22 & uVar24 | uVar22 & uVar14) + iVar2;
		uVar15 = ((iVar21 + -0x3bebad) * 0x200 | iVar21 + 0x2441453U >> 0x17) + uVar14;
		iVar3 = piVar12[0xf];
		iVar21 = uVar22 + (~uVar24 & uVar14 | uVar15 & uVar24) + iVar3;
		uVar18 = (iVar21 + 0xd8a1e681U >> 0x12 | (iVar21 + 0x1e681) * 0x4000) + uVar15;
		iVar4 = piVar12[4];
		iVar21 = uVar24 + (~uVar14 & uVar15 | uVar18 & uVar14) + iVar4;
		uVar22 = (iVar21 + 0xe7d3fbc8U >> 0xc | (iVar21 + -0x438) * 0x100000) + uVar18;
		iVar21 = uVar14 + (~uVar15 & uVar18 | uVar15 & uVar22) + piVar12[9];
		uVar14 = (iVar21 + 0x21e1cde6U >> 0x1b | (iVar21 + 0x1e1cde6) * 0x20) + uVar22;
		iVar5 = piVar12[0xe];
		iVar21 = uVar15 + (~uVar18 & uVar22 | uVar18 & uVar14) + iVar5;
		uVar24 = (iVar21 + 0xc33707d6U >> 0x17 | (iVar21 + 0x3707d6) * 0x200) + uVar14;
		iVar6 = piVar12[3];
		iVar21 = uVar18 + (~uVar22 & uVar14 | uVar24 & uVar22) + iVar6;
		uVar15 = ((iVar21 + 68999) * 0x4000 | iVar21 + 0xf4d50d87U >> 0x12) + uVar24;
		iVar7 = piVar12[8];
		iVar21 = uVar22 + (~uVar14 & uVar24 | uVar15 & uVar14) + iVar7;
		uVar18 = (iVar21 + 0x455a14edU >> 0xc | (iVar21 + 0x4ed) * 0x100000) + uVar15;
		iVar8 = piVar12[0xd];
		iVar21 = (~uVar24 & uVar15 | uVar24 & uVar18) + iVar8 + uVar14;
		uVar14 = ((iVar21 + 0x1e3e905) * 0x20 | iVar21 + 0xa9e3e905U >> 0x1b) + uVar18;
		iVar9 = piVar12[2];
		iVar21 = uVar24 + (~uVar15 & uVar18 | uVar15 & uVar14) + iVar9;
		uVar22 = (iVar21 + 0xfcefa3f8U >> 0x17 | (iVar21 + -0x105c08) * 0x200) + uVar14;
		iVar10 = piVar12[7];
		iVar21 = uVar15 + (~uVar18 & uVar14 | uVar22 & uVar18) + iVar10;
		uVar24 = (iVar21 + 0x676f02d9U >> 0x12 | (iVar21 + -0xfd27) * 0x4000) + uVar22;
		iVar11 = piVar12[0xc];
		iVar21 = uVar18 + (~uVar14 & uVar22 | uVar24 & uVar14) + iVar11;
		uVar15 = (iVar21 + 0x8d2a4c8aU >> 0xc | (iVar21 + -0x376) * 0x100000) + uVar24;
		uVar14 = (uVar22 ^ uVar24 ^ uVar15) + iVar1 + -0x5c6be + uVar14;
		uVar18 = (uVar14 * 0x10 | uVar14 >> 0x1c) + uVar15;
		iVar21 = uVar22 + (uVar24 ^ uVar15 ^ uVar18) + iVar7;
		uVar22 = ((iVar21 + -0xe097f) * 0x800 | iVar21 + 0x8771f681U >> 0x15) + uVar18;
		iVar21 = uVar24 + (uVar22 ^ uVar15 ^ uVar18) + piVar12[0xb];
		uVar24 = ((iVar21 + 0x6122) * 0x10000 | iVar21 + 0x6d9d6122U >> 0x10) + uVar22;
		iVar21 = (uVar22 ^ uVar24 ^ uVar18) + iVar5 + uVar15;
		uVar14 = (iVar21 + 0xfde5380cU >> 9 | (iVar21 + 0xc) * 0x800000) + uVar24;
		iVar21 = (uVar22 ^ uVar24 ^ uVar14) + piVar12[1] + uVar18;
		uVar15 = (iVar21 + 0xa4beea44U >> 0x1c | (iVar21 + 0x4beea44) * 0x10) + uVar14;
		iVar21 = uVar22 + (uVar24 ^ uVar14 ^ uVar15) + iVar4;
		uVar22 = ((iVar21 + -0x13057) * 0x800 | iVar21 + 0x4bdecfa9U >> 0x15) + uVar15;
		iVar21 = uVar24 + (uVar22 ^ uVar14 ^ uVar15) + iVar10;
		uVar18 = ((iVar21 + 0x4b60) * 0x10000 | iVar21 + 0xf6bb4b60U >> 0x10) + uVar22;
		iVar21 = (uVar22 ^ uVar18 ^ uVar15) + iVar2 + uVar14;
		uVar14 = (iVar21 + 0xbebfbc70U >> 9 | (iVar21 + 0x70) * 0x800000) + uVar18;
		iVar21 = (uVar22 ^ uVar18 ^ uVar14) + iVar8 + uVar15;
		uVar15 = (iVar21 + 0x289b7ec6U >> 0x1c | (iVar21 + -0x764813a) * 0x10) + uVar14;
		iVar21 = uVar22 + (uVar18 ^ uVar14 ^ uVar15) + *piVar12;
		uVar22 = (iVar21 + 0xeaa127faU >> 0x15 | (iVar21 + 0x127fa) * 0x800) + uVar15;
		iVar21 = uVar18 + (uVar22 ^ uVar14 ^ uVar15) + iVar6;
		uVar18 = ((iVar21 + 0x3085) * 0x10000 | iVar21 + 0xd4ef3085U >> 0x10) + uVar22;
		iVar21 = uVar14 + (uVar22 ^ uVar18 ^ uVar15) + piVar12[6];
		uVar14 = (iVar21 + 0x4881d05U >> 9 | (iVar21 + -0xfb) * 0x800000) + uVar18;
		iVar21 = (uVar22 ^ uVar18 ^ uVar14) + piVar12[9] + uVar15;
		uVar15 = ((iVar21 + -0x62b2fc7) * 0x10 | iVar21 + 0xd9d4d039U >> 0x1c) + uVar14;
		iVar21 = (uVar18 ^ uVar14 ^ uVar15) + iVar11 + uVar22;
		uVar22 = (iVar21 + 0xe6db99e5U >> 0x15 | (iVar21 + -0x4661b) * 0x800) + uVar15;
		iVar21 = (uVar22 ^ uVar14 ^ uVar15) + iVar3 + uVar18;
		uVar24 = ((iVar21 + 0x7cf8) * 0x10000 | iVar21 + 0x1fa27cf8U >> 0x10) + uVar22;
		iVar21 = uVar14 + (uVar22 ^ uVar24 ^ uVar15) + iVar9;
		uVar18 = (iVar21 + 0xc4ac5665U >> 9 | (iVar21 + 0x65) * 0x800000) + uVar24;
		iVar21 = uVar15 + ((~uVar22 | uVar18) ^ uVar24) + *piVar12;
		uVar14 = (iVar21 + 0xf4292244U >> 0x1a | (iVar21 + 0x292244) * 0x40) + uVar18;
		iVar21 = uVar22 + ((~uVar24 | uVar14) ^ uVar18) + iVar10;
		uVar15 = (iVar21 + 0x432aff97U >> 0x16 | (iVar21 + -0x150069) * 0x400) + uVar14;
		iVar21 = uVar24 + ((~uVar18 | uVar15) ^ uVar14) + iVar5;
		uVar22 = ((iVar21 + 0x23a7) * 0x8000 | iVar21 + 0xab9423a7U >> 0x11) + uVar15;
		iVar21 = uVar18 + ((~uVar14 | uVar22) ^ uVar15) + iVar1;
		uVar24 = ((iVar21 + 0x39) * 0x200000 | iVar21 + 0xfc93a039U >> 0xb) + uVar22;
		iVar21 = uVar14 + ((~uVar15 | uVar24) ^ uVar22) + iVar11;
		uVar18 = (iVar21 + 0x655b59c3U >> 0x1a | (iVar21 + 0x15b59c3) * 0x40) + uVar24;
		iVar21 = uVar15 + ((~uVar22 | uVar18) ^ uVar24) + iVar6;
		uVar14 = ((iVar21 + 0xccc92) * 0x400 | iVar21 + 0x8f0ccc92U >> 0x16) + uVar18;
		iVar21 = uVar22 + ((~uVar24 | uVar14) ^ uVar18) + iVar2;
		uVar15 = (iVar21 - 0x100b83U >> 0x11 | (iVar21 + -0xb83) * 0x8000) + uVar14;
		iVar21 = uVar24 + ((~uVar18 | uVar15) ^ uVar14) + piVar12[1];
		uVar22 = ((iVar21 + -0x22f) * 0x200000 | iVar21 + 0x85845dd1U >> 0xb) + uVar15;
		iVar21 = uVar18 + ((~uVar14 | uVar22) ^ uVar15) + iVar7;
		uVar24 = (iVar21 + 0x6fa87e4fU >> 0x1a | (iVar21 + -0x5781b1) * 0x40) + uVar22;
		iVar21 = uVar14 + ((~uVar15 | uVar24) ^ uVar22) + iVar3;
		uVar18 = ((iVar21 + -0x131920) * 0x400 | iVar21 + 0xfe2ce6e0U >> 0x16) + uVar24;
		iVar21 = uVar15 + ((~uVar22 | uVar18) ^ uVar24) + piVar12[6];
		uVar14 = ((iVar21 + -0xbcec) * 0x8000 | iVar21 + 0xa3014314U >> 0x11) + uVar18;
		iVar21 = uVar22 + ((~uVar24 | uVar14) ^ uVar18) + iVar8;
		uVar15 = ((iVar21 + 0x1a1) * 0x200000 | iVar21 + 0x4e0811a1U >> 0xb) + uVar14;
		iVar21 = uVar24 + ((~uVar18 | uVar15) ^ uVar14) + iVar4;
		uVar22 = (iVar21 + 0xf7537e82U >> 0x1a | (iVar21 + -0xac817e) * 0x40) + uVar15;
		iVar21 = uVar18 + ((~uVar14 | uVar22) ^ uVar15) + piVar12[0xb];
		uVar18 = ((iVar21 + -0x50dcb) * 0x400 | iVar21 + 0xbd3af235U >> 0x16) + uVar22;
		iVar21 = uVar14 + ((~uVar15 | uVar18) ^ uVar22) + iVar9;
		uVar14 = ((iVar21 + -0x2d45) * 0x8000 | iVar21 + 0x2ad7d2bbU >> 0x11) + uVar18;
		iVar20 = uVar22 + iVar20;
		uVar17 = uVar18 + uVar17;
		iVar21 = uVar15 + ((~uVar22 | uVar14) ^ uVar18) + piVar12[9];
		uVar23 = (iVar21 + 0xeb86d391U >> 0xb | (iVar21 + 0x391) * 0x200000) + uVar14 + uVar23;
		uVar19 = uVar14 + uVar19;
		piVar12 = piVar12 + 0x10;
	}
	*(uint *)(oByte + 4) = uVar23;
	*(int *)oByte = iVar20;
	*(uint *)(oByte + 8) = uVar19;
	*(uint *)(oByte + 0xc) = uVar17;
	return;
}

//443ff0
char md5str[32];
char* MD5str(char *iStr) {
	char* buf;
	unsigned char md5buf[16];

	/*buf = (char*)malloc(strlen(iStr));
	strcpy(buf, iStr);
	MD5byte(&buf, strlen(buf), (char*)md5buf);
	sprintf(md5str,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", md5buf[0], md5buf[1], md5buf[2], md5buf[3], 
		md5buf[4], md5buf[5], md5buf[6], md5buf[7], md5buf[8], md5buf[9], md5buf[10], md5buf[11], md5buf[12], md5buf[13], md5buf[14], md5buf[15]);
	free(buf);*/
	buf = (char*)md5String(iStr);
	memcpy(md5buf, buf, 16);
	sprintf(md5str, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", md5buf[0], md5buf[1], md5buf[2], md5buf[3],
		md5buf[4], md5buf[5], md5buf[6], md5buf[7], md5buf[8], md5buf[9], md5buf[10], md5buf[11], md5buf[12], md5buf[13], md5buf[14], md5buf[15]);

	return md5str;
}

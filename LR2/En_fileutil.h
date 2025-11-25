#pragma once

#include "structure.h"
#include "strclass.h"

#ifdef _WIN32

#include <windows.h>

#else

#define ULONGLONG unsigned long long

typedef union _ULARGE_INTEGER {
  struct {
    DWORD LowPart;
    DWORD HighPart;
  } DUMMYSTRUCTNAME;
  struct {
    DWORD LowPart;
    DWORD HighPart;
  } u;
  ULONGLONG QuadPart;
} ULARGE_INTEGER;

typedef struct _FILETIME {
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

#endif // _WIN32

//hash
int makeFileHash(LPCSTR filepath, LPCSTR oBuf);
//convertTime 
time_t GetNowUnixtime(void);
time_t GetUnixtimeFromFiletime(FILETIME & filetime);
time_t GetFileUnixtime(CSTR str);
//files
CSTR GetRandomFileOnDir(CSTR path, char fOnlyName);
bool CheckStringHead(CSTR* bigS, CSTR* head);
bool GetStringBodyStr(CSTR* str, CSTR head, CSTR* oBuf);
bool GetStringBodyInt(CSTR* str, CSTR haed, int * oBuf);
bool GetDifficultyFromToken(CSTR str, CSTR *oLeft, CSTR *oRight, CSTR tokenL, CSTR tokenR, int *oBuf);
bool GetDifficulty(CSTR *str, CSTR head, CSTR *oLeft, CSTR *oRight, int *pDifficulty);

bool IsBmsFile(CSTR str);
bool IsMediaFile(CSTR str);
bool IsSndFile(CSTR str);
bool IsAviFile(CSTR str);
bool IsLR2Folder(CSTR str);

bool IsFileExist(CSTR path);
int IsFileChanged(unsigned int oldUnixtime, CSTR filepath, int * oNewtime);

//parseutil
int DealWhiteSpace(CSTR * str);
int SplitCSV(CSTR csvStr, CSVbuf * oBuf, const char * splitter);
int Base36ToInt(char ch1, char ch2);
int HEXcharToInt(char ch1, char ch2);
int RoundUp(double val);
CSTR AssignCRC32(CSTR str);
CSTR AutomationFactory();
int CountDigit(int num);
int GetDigitNum(int num, int pos);

int FindAltImage(CSTR filename, CSTR dir, CSTR * oBuf);
int FindAltSound(CSTR filename, CSTR dir, CSTR * oBuf);
CSTR GetRandomFile(CSTR path, char fOnlyName);
CSTR GetRandomFileNoError(CSTR path, CSTR dir);

//md5
void MD5byte(char **iStr, uint len, char *oByte);
char* MD5str(char *iStr);

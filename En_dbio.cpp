#include "En_dbio.h"

CRITICAL_SECTION DB_lock;
//4440f0
int DB_EnterCriticalSection() {
	EnterCriticalSection(&DB_lock);
	return 1;
}

//444110
int DB_LeaveCriticalSection() {
	LeaveCriticalSection(&DB_lock);
	return 1;
}

//444130 //TODO more readable
//ANSI >> UTF-16(unicode) >> UTF-8
bool ANSItoUTF8(LPCSTR str, char *oBuf, size_t *oSize){
	int cchWideChar;
	LPWSTR lpWideCharStr;
	LPSTR lpMultiByteStr;
	size_t size;

	*oSize = 0;
	cchWideChar = MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)0x0, 0);
	lpWideCharStr = (LPWSTR)malloc(cchWideChar * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, str, -1, lpWideCharStr, cchWideChar);
	size = WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, (LPSTR)0x0, 0, (LPCSTR)0x0, (LPBOOL)0x0);
	if (oBuf == NULL) {
		*oSize = size;
		delete(lpWideCharStr);
		return true;
	}
	lpMultiByteStr = (LPSTR)malloc(size * 2);
	memset(lpMultiByteStr, '\0', size * 2);
	WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, lpMultiByteStr, size, (LPCSTR)0x0, (LPBOOL)0x0);
	size = lstrlenA(lpMultiByteStr);
	*oSize = size;
	memcpy(oBuf, lpMultiByteStr, size);
	delete(lpWideCharStr);
	delete(lpMultiByteStr);
	return true;
}

//444210
//UTF-8 >> UTF-16(unicode) >> ANSI
bool UTF8toANSI(LPCSTR str, char *oBuf, size_t *oSize){
	int cchWideChar;
	LPWSTR lpWideCharStr;
	LPSTR lpMultiByteStr;
	size_t size;
	
	*oSize = 0;
	cchWideChar = MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)0x0, 0);
	lpWideCharStr = (LPWSTR)malloc(cchWideChar * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, lpWideCharStr, cchWideChar);
	size = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, (LPSTR)0x0, 0, (LPCSTR)0x0, (LPBOOL)0x0);
	if (oBuf == NULL) {
		*oSize = size;
		delete(lpWideCharStr);
		return true;
	}
	lpMultiByteStr = (LPSTR)malloc(size * 2);
	memset(lpMultiByteStr, '\0', size * 2);
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, lpMultiByteStr, size, (LPCSTR)0x0, (LPBOOL)0x0);
	size = lstrlenA(lpMultiByteStr);
	*oSize = size;
	memcpy(oBuf, lpMultiByteStr, size);
	delete(lpWideCharStr);
	delete(lpMultiByteStr);
	return true;
}

//4442f0
int SQL_Run(CSTR queryStr, sqlite3 *sql){
	
	int result;
	int cchWideChar;
	LPCWSTR lpWideCharStr;
	LPCSTR lpMultiByteStr;
	size_t size;
	char *oBuf;
	size_t newsize;

	lpMultiByteStr = queryStr;

	cchWideChar = MultiByteToWideChar(CP_ACP, 0, queryStr, -1, (LPWSTR)0x0, 0);
	lpWideCharStr = (LPWSTR)malloc(cchWideChar * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, -1, (LPWSTR)lpWideCharStr, cchWideChar);
	size = WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, (LPSTR)0x0, 0, (LPCSTR)0x0, (LPBOOL)0x0);
	newsize = size;
	delete(lpWideCharStr);

	oBuf = (char *)malloc(size + 1);
	memset(oBuf, '\0', size + 1);
	ANSItoUTF8(queryStr, oBuf, &newsize);
	oBuf[newsize] = '\0';
	result = sqlite3_exec(sql, oBuf, NULL, NULL, NULL);
	delete(oBuf);
	return result;
}

//4443f0
int SQL_prepare(CSTR queryStr, sqlite3 *sql, sqlite3_stmt **ppStmt){
	
	int cchWideChar;
	LPCWSTR lpWideCharStr;
	LPCSTR lpMultiByteStr;
	size_t size;
	char *oBuf;
	size_t newsize;
	int result;

	lpMultiByteStr = queryStr;

	cchWideChar = MultiByteToWideChar(CP_ACP, 0, queryStr, -1, (LPWSTR)0x0, 0);
	lpWideCharStr = (LPCWSTR)malloc(cchWideChar * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, -1, (LPWSTR)lpWideCharStr, cchWideChar);
	size = WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, (LPSTR)0x0, 0, (LPCSTR)0x0, (LPBOOL)0x0);
	newsize = size;
	delete(lpWideCharStr);

	oBuf = (char *)malloc(size + 1);
	memset(oBuf, '\0', size + 1);
	ANSItoUTF8(queryStr, oBuf, &newsize);
	oBuf[newsize] = '\0';
	result = sqlite3_prepare(sql, oBuf, -1, ppStmt, NULL);
	delete(oBuf);
	return result;
}

//4444f0
CSTR SQL_GetColumn(int i, sqlite3_stmt *pStmt){
	CSTR oBuf;
	size_t size;
	LPCSTR columnText;

	if (sqlite3_column_type(pStmt, i) == SQLITE_NULL) {
		oBuf.fillzero();
	}
	else {
		sqlite3_column_bytes(pStmt, i);
		columnText = (LPCSTR)sqlite3_column_text(pStmt, i);
		UTF8toANSI(columnText, NULL, &size);
		oBuf.resize2(size + 1);
		memset(oBuf.body, 0, size + 1);
		UTF8toANSI(columnText, oBuf.body, &size);
		*oBuf.atPos(size) = 0;
	}
	return oBuf;
}

#include "En_dbio.h"

#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
using LPCSTR = const char*;
enum {
	CP_ACP,
	CP_UTF8,
};
#endif // _WIN32

// \param oBuf If nullptr, just count the required for conversion size into oSize
// 'from' >> UTF-16(unicode) >> 'to'
static bool convert(LPCSTR str, char *oBuf, size_t *oSize, int from, int to){
#ifdef _WIN32
	int cchWideChar;
	LPWSTR lpWideCharStr;
	LPSTR lpMultiByteStr;
	size_t size;

	*oSize = 0;
	cchWideChar = MultiByteToWideChar(from, 0, str, -1, (LPWSTR)0x0, 0);
	lpWideCharStr = (LPWSTR)malloc(cchWideChar * 2 + 2);
	MultiByteToWideChar(from, 0, str, -1, lpWideCharStr, cchWideChar);
	size = WideCharToMultiByte(to, 0, lpWideCharStr, -1, (LPSTR)0x0, 0, (LPCSTR)0x0, (LPBOOL)0x0);
	if (oBuf == nullptr) {
		*oSize = size;
		free(lpWideCharStr);
		return true;
	}
	lpMultiByteStr = (LPSTR)malloc(size * 2);
	memset(lpMultiByteStr, '\0', size * 2);
	WideCharToMultiByte(to, 0, lpWideCharStr, -1, lpMultiByteStr, size, (LPCSTR)0x0, (LPBOOL)0x0);
	size = lstrlenA(lpMultiByteStr);
	*oSize = size;
	memcpy(oBuf, lpMultiByteStr, size);
	free(lpWideCharStr);
	free(lpMultiByteStr);
	return true;
#else
	(void)from;
	(void)to;
	*oSize = strlen(str);
	if (oBuf != nullptr) {
		memcpy(oBuf, str, *oSize);
	}
	return false; // FIXME(linux): stub
#endif // _WIN32
}

static bool ANSItoUTF8(LPCSTR str, char *oBuf, size_t *oSize){
	return convert(str, oBuf, oSize, CP_ACP, CP_UTF8);
}

static bool UTF8toANSI(LPCSTR str, char *oBuf, size_t *oSize){
	return convert(str, oBuf, oSize, CP_UTF8, CP_ACP);
}

int SQL_Run(CSTR queryStr, sqlite3 *sql) {
	return sqlite3_exec(sql, queryStr.body, nullptr, nullptr, nullptr);
}

int SQL_prepare(CSTR queryStr, sqlite3 *sql, sqlite3_stmt **ppStmt) {
	return sqlite3_prepare(sql, queryStr.body, -1, ppStmt, nullptr);
}

CSTR SQL_GetColumn(int i, sqlite3_stmt *pStmt){
	if (sqlite3_column_type(pStmt, i) == SQLITE_NULL) {
		return {};
	}

	// sqlite3_column_bytes(pStmt, i);

	// Cast safety: it's safe to cast from unsigned char* to char*
	const char* columnText = reinterpret_cast<const char*>(sqlite3_column_text(pStmt, i));

	CSTR oBuf(columnText);
	return oBuf;
}

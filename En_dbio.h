#include "strclass.h"
extern "C" {
#include "sqlite/sqlite3.h"
}

extern int EnabledInsane;
extern CRITICAL_SECTION DB_lock;
int DB_EnterCriticalSection();
int DB_LeaveCriticalSection();

bool ANSItoUTF8(LPCSTR str, char * oBuf, size_t * oSize);
bool UTF8toANSI(LPCSTR str, char * oBuf, size_t * oSize);

int SQL_Run(CSTR queryStr, sqlite3 * sql);
int SQL_prepare(CSTR queryStr, sqlite3 * sql, sqlite3_stmt ** ppStmt);
CSTR SQL_GetColumn(int i, sqlite3_stmt * pStmt);
#pragma once

#include "structure.h"
#include "Engine.h"

int WriteGhostInDatabase(sqlite3 * sql, CSTR songMD5, PLAYSCORE * score);
int ReadGhostToScore(sqlite3 * sql, CSTR songMD5, PLAYSCORE * score);
int ReadIRGhostToScore(sqlite3* sql, CSTR songMD5, PLAYSCORE* score);
CSTR ReadGhost(sqlite3 * sql, CSTR songMD5);
CSTR ReadIRGhost(sqlite3 * sql, CSTR songMD5);

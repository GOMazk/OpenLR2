#pragma once

#include "structure.h"

struct sqlite3;
int ProcB_Result(game * g, sqlite3 * sql);
int Proc_Result(game * g, skstruct * sk, Timer * T);
int ProcI_Result(game * g);
int ProcE_Result(game* g);

#pragma once
//250703 memo : I tried to seperate files into 3 groups, Engine, game, scene.
//three days after, I concluded that I cannot seperate them now.
//so included almost every files into here, 

//250706 memo : feature/function based struct seperation is almost IMPOSSIBLE (due to game struct)
//so I bring structure.h again



////IO
//AUDIO IO
//engine is DXLIB and FMODEX
#include "En_audio.h"
//GRAPHIC IO
//engine id DXLiB
//manage with skin
#include "En_graphic.h" //dxlib misc

//make movie output (gr/aud)
#include "En_recordmovie.h"

//INPUT IO
//engine is DXLIB and originalMIDIcode
#include "En_input.h"
//DB IO
//engine is sqlite3
#include "En_dbio.h"
//FILE IO
//xml, custom csv
#include "En_fileutil.h"
#include "En_xml.h"
//TIMER
#include "En_timer.h"

//value range util
#include "En_value.h"


#include "DxLib/DxLib.h"


//temp DEBUG
#include "dbgtool.h"

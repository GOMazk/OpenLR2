// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		データタイプ定義ヘッダファイル
// 
// 				Ver 3.24f
// 
// -------------------------------------------------------------------------------

#ifndef DX_DATATYPE_H
#define DX_DATATYPE_H

// インクルード ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include <stdio.h>

#ifdef WINDOWS_DESKTOP_OS
#include "DxDataTypeWin.h"
#endif // WINDOWS_DESKTOP_OS

#ifdef __ANDROID__
#include "DxDataTypeAndroid.h"
#endif // __ANDROID__

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
		#include "DxDataTypeiOS.h"
	#endif
#endif // __APPLE__




#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#ifndef DX_COMPILE_TYPE_C_LANGUAGE
	#define DEFAULTPARAM( param )					param
#else
	#define DEFAULTPARAM( param )
#endif

#define SETRECT( Dest, Left, Top, Right, Bottom )	\
	( Dest ).left   = Left ;\
	( Dest ).top    = Top ;\
	( Dest ).right  = Right ;\
	( Dest ).bottom = Bottom ;

#ifdef USE_ULL
#define ULL_NUM( x )				( x##ULL )
#define LL_NUM( x )					( x##LL  )
#define ULL_PARAM( x )				x##ULL
#define LL_PARAM( x )				x##LL
#else
#define ULL_NUM( x )				( ( ULONGLONG )x )
#define LL_NUM( x )					(  ( LONGLONG )x )
#define ULL_PARAM( x )				x
#define LL_PARAM( x )				x
#endif

// 構造体定義 --------------------------------------------------------------------

// ＲＧＢＡ色構造体
struct RGBCOLOR
{
	unsigned char			Blue, Green, Red, Alpha ;
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_DATATYPE_H

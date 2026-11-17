// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用入力情報プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_INPUTIOS_H
#define DX_INPUTIOS_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_INPUT

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define IOS_ICADE_INPUT_UP			(0)
#define IOS_ICADE_INPUT_RIGHT		(1)
#define IOS_ICADE_INPUT_DOWN		(2)
#define IOS_ICADE_INPUT_LEFT		(3)
#define IOS_ICADE_INPUT_0			(4)
#define IOS_ICADE_INPUT_1			(5)
#define IOS_ICADE_INPUT_2			(6)
#define IOS_ICADE_INPUT_3			(7)
#define IOS_ICADE_INPUT_4			(8)
#define IOS_ICADE_INPUT_5			(9)
#define IOS_ICADE_INPUT_6			(10)
#define IOS_ICADE_INPUT_7			(11)
#define IOS_ICADE_INPUT_NUM			(12)

// 構造体定義 --------------------------------------------------------------------

// 振動の環境依存情報
struct INPUTVIBRATIONDATA_PF
{
	int						Dummy ;
} ;

// ゲームパッドの環境依存情報
struct INPUTPADDATA_PF
{
	int						Dummy ;
} ;

// 入力システム用環境依存データ構造体型
struct INPUTSYSTEMDATA_PF
{
	int						iCadeEnable ;
	int						iCadeInputState[ IOS_ICADE_INPUT_NUM ] ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

extern int iCadeInputSet( char InputChar ) ;



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_INPUT

#endif // DX_INPUTIOS_H

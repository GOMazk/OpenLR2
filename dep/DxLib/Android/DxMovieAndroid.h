// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用動画プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_MOVIEANDROID_H
#define DX_MOVIEANDROID_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_MOVIE

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxFile.h"
//#include <videodec.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// ムービーグラフィック環境依存データ型
struct MOVIEGRAPH_PF
{
	int							Dummy ;
} ;

// ムービーデータ環境依存管理構造体
struct MOVIEGRAPHMANAGE_PF
{
	int						Dummy ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MOVIE

#endif // DX_MOVIEANDROID_H

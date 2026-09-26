// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Live2D Cubism4関係ヘッダファイル( Windows )
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_LIVE2DCUBISM4WIN_H
#define DX_LIVE2DCUBISM4WIN_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_LIVE2D_CUBISM4

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"

// #ifndef DX_NON_NAMESPACE
// 
// namespace DxLib
// {
// 
// #endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define LIVE2D_CUBISM4_FUNC		

// 構造体定義 --------------------------------------------------------------------

// Live2D Cubism4 システム環境依存用データ構造体
struct LIVE2DCUBISM4SYSTEMDATA_PF
{
	HMODULE					Live2DCubismCoreDLL ;			// Live2D Cubism Core DLL
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

// #ifndef DX_NON_NAMESPACE
// 
// }
// 
// #endif // DX_NON_NAMESPACE

#endif // DX_NON_LIVE2D_CUBISM4

#endif // DX_LIVE2DCUBISM4WIN_H

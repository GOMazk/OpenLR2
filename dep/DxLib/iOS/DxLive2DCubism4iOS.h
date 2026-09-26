// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Live2D Cubism4関係ヘッダファイル( iOS )
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_LIVE2DCUBISM4IOS_H
#define DX_LIVE2DCUBISM4IOS_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_LIVE2D_CUBISM4

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"
#include "DxGraphicsiOS.h"

// #ifndef DX_NON_NAMESPACE
// 
// namespace DxLib
// {
// 
// #endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define LIVE2D_CUBISM4_FUNC		

#define LIVE2D_SHADER_SetupMask_SetupMask										0
#define LIVE2D_SHADER_NormalMasked_NormalMaskedPremultipliedAlpha				1
#define LIVE2D_SHADER_NormalMasked_NormalMaskedInvertedPremultipliedAlpha		2
#define LIVE2D_SHADER_NormalMasked_NormalMasked									3
#define LIVE2D_SHADER_NormalMasked_NormalMaskedInverted							4
#define LIVE2D_SHADER_Normal_NormalPremultipliedAlpha							5
#define LIVE2D_SHADER_Normal_Normal												6
#define LIVE2D_SHADER_Num														7

// 構造体定義 --------------------------------------------------------------------

// Live2D Cubism4 システム環境依存用データ構造体
struct LIVE2DCUBISM4SYSTEMDATA_PF
{
	GRAPHICS_IOS_SHADER Shader[ LIVE2D_SHADER_Num ] ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

// Live2D Cubism4 で使用している全てのシェーダーを開放する
extern int Live2D_Cubism4_iOS_ReleaseShaderAll( void ) ;

// #ifndef DX_NON_NAMESPACE
// 
// }
// 
// #endif // DX_NON_NAMESPACE

#endif // DX_NON_LIVE2D_CUBISM4

#endif // DX_LIVE2DCUBISM4IOS_H

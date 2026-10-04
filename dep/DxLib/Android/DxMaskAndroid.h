// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用マスクデータ管理プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_MASKANDROID_H
#define DX_MASKANDROID_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_MASK

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxGraphics.h"
#include "../DxArchive_.h"
#include <GLES2/gl2.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define MASKANDR					MaskManageData_Android

// 構造体定義 --------------------------------------------------------------------

// Android 用のマスク処理情報構造体
struct MASKMANAGEDATA_ANDR
{
	int								MaskTextureSizeX ;				// マスクテクスチャのサイズ
	int								MaskTextureSizeY ;

	GLuint							MaskScreenFrameBuffer ;			// マスク処理用仮画面フレームバッファ
	GLuint							MaskScreenTextureBuffer ;		// マスク処理用仮画面テクスチャバッファ
	GLuint							MaskScreenDepthBuffer ;			// マスク処理用仮画面深度バッファ

	GLuint							MaskScreenOldFrameBuffer ;		// 古いマスク処理用仮画面フレームバッファ
	GLuint							MaskScreenOldTextureBuffer ;	// 古いマスク処理用仮画面テクスチャバッファ
	GLuint							MaskScreenOldDepthBuffer ;		// 古いマスク処理用仮画面深度バッファ

	GLuint							MaskImageTextureBuffer ;		// マスクイメージテクスチャバッファ

	GLuint							DestTargetFrameBuffer ;			// 描画対象のフレームバッファ
	GLuint							DestTargetTextureBuffer ;		// 描画対象のテクスチャバッファ
	GLuint							DestTargetDepthBuffer ;			// 描画対象の深度バッファ
	GLuint							DestTargetWidth ;				// 描画対象の幅
	GLuint							DestTargetHeight ;				// 描画対象の高さ
} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern MASKMANAGEDATA_ANDR MaskManageData_Android ;

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MASK

#endif // DX_MASKANDROID_H

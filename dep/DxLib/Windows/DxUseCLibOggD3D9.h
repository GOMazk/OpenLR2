// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Ogg関係ヘッダファイル( Direct3D9 )
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_USECLIBOGGD3D9_H
#define DX_USECLIBOGGD3D9_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D9

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxUseCLibOgg.h"
#include "DxDirectX.h"

#if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

//namespace DxLib
//{

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

#ifndef DX_NON_OGGTHEORA

// Direct3D9用 Ogg Theora デコード処理用環境依存データ構造体
struct DECODE_THEORA_DIRECT3D9
{
	D_IDirect3DSurface9		*YUVSurface ;				// カレントフレームが格納されたフレームスタック中の YUV フォーマットサーフェス
} ;

#endif // DX_NON_OGGTHEORA

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_OGGTHEORA

// 環境依存関数
extern	int		TheoraDecode_D3D9_CreateSurface_PF(	 DECODE_THEORA *DT ) ;																// ハードウエア機能が有効な場合は YUV フォーマットの一時保存用テクスチャを作成する
extern	void	TheoraDecode_D3D9_ReleaseSurface_PF( DECODE_THEORA *DT ) ;
extern	int		TheoraDecode_D3D9_SetupImage_PF( DECODE_THEORA *DT, volatile THEORA_STOCKFRAME *Stock, int ASyncThread ) ;				// カレントフレームのRGB画像を作成する( 戻り値  1:作成された  0:されなかった )
extern	const void *TheoraDecode_D3D9_GetYUVImage_PF( DECODE_THEORA *DT ) ;												// 一時バッファの YUV フォーマットのテクスチャを得る

#endif // DX_NON_OGGTHEORA

//}

#endif // #if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

#endif // DX_NON_DIRECT3D9

#endif // DX_NON_GRAPHICS

#endif // DX_USECLIBOGGD3D9_H

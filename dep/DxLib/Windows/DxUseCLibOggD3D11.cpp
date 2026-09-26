//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Ogg関係プログラム( Direct3D11 )
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_DIRECT3D11

#if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

// インクルード----------------------------------------------------------------
#include "../DxLib.h"
#include "../DxStatic.h"
#include "../DxUseCLib.h"
#include "../DxGraphics.h"
#include "../DxLog.h"
#include "DxGraphicsAPIWin.h"
#include "DxUseCLibOggD3D11.h"
#include "DxUseCLibOggWin.h"

//namespace DxLib
//{

// マクロ定義------------------------------------------------------------------

// 型定義----------------------------------------------------------------------

// データ宣言------------------------------------------------------------------


// 関数プロトタイプ宣言 -------------------------------------------------------

// プログラム------------------------------------------------------------------

// 環境依存関数

#ifndef DX_NON_OGGTHEORA

// ハードウエア機能が有効な場合は YUV フォーマットの一時保存用テクスチャを作成する
extern	int	TheoraDecode_D3D11_CreateSurface_PF( DECODE_THEORA *DT )
{
	// 今のところYUVフォーマットには対応しない
	DT->YUVImageFourCC = 0 ;
	DT->ValidYUVSurface = FALSE ;

	// 正常終了
	return 0 ;
}

extern	void	TheoraDecode_D3D11_ReleaseSurface_PF( DECODE_THEORA * /*DT*/ )
{
}

// カレントフレームのRGB画像を作成する( 戻り値  1:作成された  0:されなかった )
extern	int		TheoraDecode_D3D11_SetupImage_PF( DECODE_THEORA * /*DT*/, volatile THEORA_STOCKFRAME * /*Stock*/, int /*ASyncThread*/ )
{
	// 正常終了
	return 0 ;
}

// 一時バッファの YUV フォーマットのテクスチャを得る
extern	const void *TheoraDecode_D3D11_GetYUVImage_PF( DECODE_THEORA * /*DT*/ )
{
	return NULL ;
}

#endif // DX_NON_OGGTHEORA



//}

#endif // #if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

#endif // DX_NON_DIRECT3D11


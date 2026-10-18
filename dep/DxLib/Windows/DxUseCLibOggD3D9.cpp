//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Ogg関係プログラム( Direct3D9 )
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_NAMESPACE
#ifdef DX_NON_USING_NAMESPACE_DXLIB

using namespace DxLib ;

#endif // DX_NON_USING_NAMESPACE_DXLIB
#endif // DX_NON_NAMESPACE

#ifndef DX_NON_DIRECT3D9

#if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

// インクルード----------------------------------------------------------------
#include "../DxLib.h"
#include "../DxStatic.h"
#include "../DxUseCLib.h"
#include "../DxGraphics.h"
#include "../DxLog.h"
#include "DxGraphicsAPIWin.h"
#include "DxUseCLibOggD3D9.h"
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
extern	int	TheoraDecode_D3D9_CreateSurface_PF( DECODE_THEORA *DT )
{
//	ogg_uint32_t w, h ;

	// YUVフォーマットの対応を無効化
	DT->YUVImageFourCC = 0 ;
	DT->ValidYUVSurface = FALSE ;

	return 0 ;

//	// ハードウエア機能が有効な場合は YUV フォーマットの一時保存用テクスチャを作成する
//	if( DT->NotUseYUVFormatSurface || GSYS.Setting.ValidHardware == FALSE || Graphics_Hardware_CheckValid_PF() == 0 )
//	{
//		return 0 ;
//	}
//
//	DT->PF->D3D9.YUVSurface = NULL ;
//
//	w = DT->TheoraInfo.width ;
//	h = DT->TheoraInfo.height ;
//
//	// 最初は YV12 形式を試す
//	DT->YUVImageFourCC = MAKEFOURCC( 'Y', 'V', '1', '2' ) ;
//	Direct3DDevice9_CreateOffscreenPlainSurface( w, h, ( D_D3DFORMAT )DT->YUVImageFourCC, D_D3DPOOL_DEFAULT, ( D_IDirect3DSurface9 ** )&DT->PF->D3D9.YUVSurface, NULL ) ;
//
//	// 作成に失敗したら YUY2 フォーマットを試してみる
//	if( DT->PF->D3D9.YUVSurface == NULL )
//	{
//		DT->YUVImageFourCC = MAKEFOURCC( 'Y', 'U', 'Y', '2' ) ;
//		Direct3DDevice9_CreateOffscreenPlainSurface( w, h, ( D_D3DFORMAT )DT->YUVImageFourCC, D_D3DPOOL_DEFAULT, ( D_IDirect3DSurface9 ** )&DT->PF->D3D9.YUVSurface, NULL ) ;
//	}
//
//	// それでも駄目なら UYVY フォーマットを試す
//	if( DT->PF->D3D9.YUVSurface == NULL )
//	{
//		DT->YUVImageFourCC = MAKEFOURCC( 'U', 'Y', 'V', 'Y' ) ;
//		Direct3DDevice9_CreateOffscreenPlainSurface( w, h, ( D_D3DFORMAT )DT->YUVImageFourCC, D_D3DPOOL_DEFAULT, ( D_IDirect3DSurface9 ** )&DT->PF->D3D9.YUVSurface, NULL ) ;
//	}
//
//	// それでも駄目なら YUV フォーマットを諦める
//	if( DT->PF->D3D9.YUVSurface == NULL )
//	{
//		DT->YUVImageFourCC = 0 ;
//		DT->ValidYUVSurface = FALSE ;
//	}
//	else
//	{
//		DT->ValidYUVSurface = TRUE ;
//	}
//
//	// 正常終了
//	return 0 ;
}

extern	void	TheoraDecode_D3D9_ReleaseSurface_PF( DECODE_THEORA *DT )
{
	DT->YUVSurfaceReleaseRequest = FALSE ;

	if( DT->PF->D3D9.YUVSurface != NULL )
	{
		DT->PF->D3D9.YUVSurface->Release() ;
		DT->PF->D3D9.YUVSurface = NULL ;
	}
}

// カレントフレームのRGB画像を作成する( 戻り値  1:作成された  0:されなかった )
extern	int		TheoraDecode_D3D9_SetupImage_PF( DECODE_THEORA *DT, volatile THEORA_STOCKFRAME *Stock, int ASyncThread )
{
	D_D3DLOCKED_RECT LockRect = { 0 } ;
	int              Ok ;
	unsigned char   *d ;
	unsigned char   *ys ;
	unsigned char   *uvs ;
	int              i ;
	int              j ;
	int              ysadd ;
	int              dadd ;
	int              uvadd ;
	int              w ;
	int              h ;
	int              dpitch ;

	// ロックに失敗したらサーフェスの作成しなおし
	Ok = 0 ;
	for( i = 0 ; i < 3 ; i ++ )
	{
		if( DT->ValidYUVSurface == FALSE || DT->YUVSurfaceReleaseRequest || Direct3DSurface9_LockRect_ASync( DT->PF->D3D9.YUVSurface, &LockRect, NULL, 0, ASyncThread ) != D_DD_OK )
		{
			TheoraDecode_CreateSurface( DT, ASyncThread ) ;
		}
		else
		{
			Ok = 1 ;
			break ;
		}
	}

	// 作成しなおしても駄目だったら諦め
	if( Ok == 1 )
	{
		// フォーマットによって処理を分岐
		d   = ( unsigned char * )LockRect.pBits ;
		ys  = ( unsigned char * )Stock->YBuffer ;
		uvs = ( unsigned char * )Stock->UVBuffer ;
		dpitch = LockRect.Pitch ;

		// YV12 の場合
		if( DT->YUVImageFourCC == MAKEFOURCC( 'Y', 'V', '1', '2' ) )
		{
			if( Stock->YWidth  == Stock->UVWidth  * 2 &&
				Stock->YHeight == Stock->UVHeight * 2 )
			{
				unsigned char *us ;
				unsigned char *vs ;

				us = uvs ;
				vs = uvs + 1 ;

				ysadd  = Stock->YStride  * 2 - Stock->YWidth ;
				uvadd  = Stock->UVStride     - Stock->UVWidth * 2 ;
				dadd   = dpitch          * 2 - Stock->YWidth ;
				w = Stock->YWidth  / 2 ;
				h = Stock->YHeight / 2 ;

				for( i = 0 ; i < Stock->YHeight ; i ++, d += dpitch, ys += Stock->YStride )
				{
					_MEMCPY( d, ys, ( size_t )Stock->YWidth ) ; 
				}
				for( i = 0 ; i < Stock->UVHeight ; i ++, d += dpitch / 2, vs += Stock->UVStride )
				{
					for( j = 0 ; j < Stock->UVWidth ; j ++ )
					{
						d[ j ] = vs[ j << 1 ] ;
					}
				}
				for( i = 0 ; i < Stock->UVHeight ; i ++, d += dpitch / 2, us += Stock->UVStride )
				{
					for( j = 0 ; j < Stock->UVWidth ; j ++ )
					{
						d[ j ] = us[ j << 1 ] ;
					}
				}
			}
		}
		else
		// YUY2 の場合
		if( DT->YUVImageFourCC == MAKEFOURCC( 'Y', 'U', 'Y', '2' ) )
		{
			if( Stock->YWidth  == Stock->UVWidth  * 2 &&
				Stock->YHeight == Stock->UVHeight * 2 )
			{
				ysadd  = Stock->YStride  * 2 - Stock->YWidth ;
				uvadd  = Stock->UVStride     - Stock->UVWidth * 2 ;
				dadd   = dpitch          * 2 - Stock->YWidth  * 2 ;
				w = Stock->YWidth  / 2 ;
				h = Stock->YHeight / 2 ;
				for( i = 0; i < h; i++, d += dadd, ys += ysadd, uvs += uvadd )
				{
					for( j = 0; j < w; j ++, d += 4, ys += 2, uvs += 2 )
					{
						d[          0 ] = ys[                  0 ] ;
						d[          2 ] = ys[                  1 ] ;
						d[ dpitch     ] = ys[ Stock->YStride     ] ;
						d[ dpitch + 2 ] = ys[ Stock->YStride + 1 ] ;

						d[          1 ] = uvs[ 0 ] ;
						d[ dpitch + 1 ] = uvs[ 0 ] ;

						d[          3 ] = uvs[ 1 ] ;
						d[ dpitch + 3 ] = uvs[ 1 ] ;
					}
				}
			}
		}
		else
		// UYVY の場合
		if( DT->YUVImageFourCC == MAKEFOURCC( 'U', 'Y', 'V', 'Y' ) )
		{
			if( Stock->YWidth  == Stock->UVWidth  * 2 &&
				Stock->YHeight == Stock->UVHeight * 2 )
			{
				ysadd  = Stock->YStride  * 2 - Stock->YWidth ;
				uvadd  = Stock->UVStride     - Stock->UVWidth * 2 ;
				dadd   = dpitch          * 2 - Stock->YWidth  * 2 ;
				w = Stock->YWidth  / 2 ;
				h = Stock->YHeight / 2 ;
				for( i = 0; i < h; i++, d += dadd, ys += ysadd, uvs += uvadd )
				{
					for( j = 0; j < w; j ++, d += 4, ys += 2, uvs += 2 )
					{
						d[          1 ] = ys[                  0 ] ;
						d[          3 ] = ys[                  1 ] ;
						d[ dpitch + 1 ] = ys[ Stock->YStride     ] ;
						d[ dpitch + 3 ] = ys[ Stock->YStride + 1 ] ;

						d[          0 ] = uvs[ 0 ] ;
						d[ dpitch + 0 ] = uvs[ 0 ] ;

						d[          2 ] = uvs[ 1 ] ;
						d[ dpitch + 2 ] = uvs[ 1 ] ;
					}
				}
			}
		}

		// ロックを解除
		Direct3DSurface9_UnlockRect_ASync( DT->PF->D3D9.YUVSurface, ASyncThread ) ;

		// セットアップフラグを立てる
		DT->YUVImageSetup = 1 ;
	}

	// 正常終了
	return 0 ;
}

// 一時バッファの YUV フォーマットのテクスチャを得る
extern	const void *TheoraDecode_D3D9_GetYUVImage_PF( DECODE_THEORA *DT )
{
	// アドレスを返す
	return DT->PF->D3D9.YUVSurface ;
}

#endif // DX_NON_OGGTHEORA


//}

#endif // #if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

#endif // DX_NON_DIRECT3D9


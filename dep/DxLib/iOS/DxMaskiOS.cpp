//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用マスクデータ管理プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_MASK

// インクルード ---------------------------------------------------------------
#include "DxGraphicsiOS.h"
#include "DxMaskiOS.h"
#include "../DxModel.h"
#include "../DxSystem.h"
#include "../DxLog.h"
#include "../DxMask.h"
#include "../DxMemory.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

MASKMANAGEDATA_IOS MaskManageData_iOS ;

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

// 環境依存関数

// マスクスクリーンを作成する関数
extern	int			Mask_CreateScreenFunction_Timing0_PF( void )
{
	MASKIOS.MaskScreenOldFrameBuffer   = MASKIOS.MaskScreenFrameBuffer ;
	MASKIOS.MaskScreenOldTextureBuffer = MASKIOS.MaskScreenTextureBuffer ;
	MASKIOS.MaskScreenOldDepthBuffer   = MASKIOS.MaskScreenDepthBuffer ;

	if( MASKIOS.MaskImageTextureBuffer != 0 )
	{
		glDeleteTextures( 1, &MASKIOS.MaskImageTextureBuffer ) ;
		MASKIOS.MaskImageTextureBuffer = 0 ;
	}

	// 正常終了
	return 0 ;
}

// マスクスクリーンを作成する関数
extern	int			Mask_CreateScreenFunction_Timing1_PF( int Width, int Height )
{
	MASKIOS.MaskTextureSizeX = Width ;
	MASKIOS.MaskTextureSizeY = Height ;

	// マスク用イメージテクスチャの作成
	if( MASKIOS.MaskImageTextureBuffer == 0 )
	{
		PIXELFORMAT_INFO_IOS *PixelFormat ;

		PixelFormat = &g_iOSPixelFormat[ GIOS.Device.Caps.MaskAlphaFormat ] ;
		glGenTextures( 1, &MASKIOS.MaskImageTextureBuffer ) ;
		glActiveTexture( GL_TEXTURE0 ) ;
		glBindTexture( GL_TEXTURE_2D, MASKIOS.MaskImageTextureBuffer ) ;

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) ;

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;

		if( PixelFormat->IsExtFormat )
		{
			glCompressedTexImage2D(
				GL_TEXTURE_2D,
				0,
				PixelFormat->InternalFormat,
				MASKIOS.MaskTextureSizeX,
				MASKIOS.MaskTextureSizeY,
				0,
				0,
				0
			) ;
		}
		else
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				PixelFormat->InternalFormat,
				MASKIOS.MaskTextureSizeX,
				MASKIOS.MaskTextureSizeY,
				0,
				PixelFormat->Format,
				PixelFormat->Type,
				0
			) ;
		}

		// マスク用イメージの転送
		{
			RECT SrcRect ;
			RECT DestRect ;
			BASEIMAGE MaskBaseImage ;
			BASEIMAGE AlphaBaseImage ;

			SrcRect.left   = 0 ;
			SrcRect.top    = 0 ;
			SrcRect.right  = MASKIOS.MaskTextureSizeX ;
			SrcRect.bottom = MASKIOS.MaskTextureSizeY ;
			DestRect = SrcRect ;

			_MEMSET( &AlphaBaseImage, 0, sizeof( AlphaBaseImage ) ) ;
			MaskBaseImage.ColorData			= *Graphics_iOS_GetDataFormatColorData( GIOS.Device.Caps.MaskAlphaFormat ) ;
			MaskBaseImage.Width				= MASKIOS.MaskTextureSizeX ;
			MaskBaseImage.Height			= MASKIOS.MaskTextureSizeY ;
			MaskBaseImage.Pitch				= MASKD.MaskBufferPitch ;
			MaskBaseImage.GraphData			= MASKD.MaskBuffer ;
			MaskBaseImage.MipMapCount		= 0 ;
			MaskBaseImage.GraphDataCount	= 0 ;

			Graphics_iOS_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
				&SrcRect,
				&DestRect,
				MASKIOS.MaskImageTextureBuffer,
				1,
				MASKIOS.MaskTextureSizeX,
				MASKIOS.MaskTextureSizeY,
				MASKIOS.MaskTextureSizeX,
				MASKIOS.MaskTextureSizeY,
				&MaskBaseImage,
				&AlphaBaseImage,
				GIOS.Device.Caps.MaskAlphaFormat,
				FALSE,
				FALSE,
				0,
				FALSE
			) ;
		}
	}

	// マスク用スクリーンテクスチャの作成
	if( MASKIOS.MaskScreenFrameBuffer == 0 )
	{
		// Offscreen position framebuffer texture target
		glGenTextures( 1, &MASKIOS.MaskScreenTextureBuffer ) ;
		glActiveTexture( GL_TEXTURE0 ) ;
		glBindTexture( GL_TEXTURE_2D, MASKIOS.MaskScreenTextureBuffer ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) ;
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, MASKIOS.MaskTextureSizeX, MASKIOS.MaskTextureSizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 ) ;

		glGenFramebuffers( 1, &MASKIOS.MaskScreenFrameBuffer ) ;
		glBindFramebuffer( GL_FRAMEBUFFER, MASKIOS.MaskScreenFrameBuffer ) ;

		// レンダーバッファ識別子を生成します。
		glGenRenderbuffers( 1, &MASKIOS.MaskScreenDepthBuffer ) ;

		// レンダーバッファ識別子に対応したレンダーバッファオブジェクトを生成します。
		glBindRenderbuffer( GL_RENDERBUFFER, MASKIOS.MaskScreenDepthBuffer ) ;

		// レンダーバッファの幅と高さを指定します。
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, MASKIOS.MaskTextureSizeX, MASKIOS.MaskTextureSizeY ) ;

		// フレームバッファのアタッチメントとしてレンダーバッファをアタッチします。
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, MASKIOS.MaskScreenDepthBuffer ) ;

		// フレームバッファのアタッチメントとして 2D テクスチャをアタッチします。
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, MASKIOS.MaskScreenTextureBuffer, 0 ) ;

		// フレームバッファが完全かどうかチェックします。
		GLint status = glCheckFramebufferStatus( GL_FRAMEBUFFER ) ;
		if( status != GL_FRAMEBUFFER_COMPLETE )
		{
			DXST_LOGFILE_ADDUTF16LE( "\xde\x30\xb9\x30\xaf\x30\x28\x75\xd5\x30\xec\x30\xfc\x30\xe0\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"マスク用フレームバッファの作成に失敗しました\n" @*/ ) ;
			return -1 ;
		}
		glBindFramebuffer( GL_FRAMEBUFFER, 0 ) ;
	}

	// 正常終了
	return 0 ;
}

// マスクスクリーンを作成する関数
extern	int			Mask_CreateScreenFunction_Timing2_PF( int MaskBufferSizeXOld, int MaskBufferSizeYOld )
{
	RECT UpdateRect ;

	// 描画を終了しておく
	Graphics_iOS_RenderEnd() ;

	// マスク用イメージの転送
	{
		RECT SrcRect ;
		RECT DestRect ;
		BASEIMAGE MaskBaseImage ;
		BASEIMAGE AlphaBaseImage ;

		SrcRect.left   = 0 ;
		SrcRect.top    = 0 ;
		SrcRect.right  = MASKIOS.MaskTextureSizeX ;
		SrcRect.bottom = MASKIOS.MaskTextureSizeY ;
		DestRect = SrcRect ;

		_MEMSET( &AlphaBaseImage, 0, sizeof( AlphaBaseImage ) ) ;
		MaskBaseImage.ColorData			= *Graphics_iOS_GetDataFormatColorData( GIOS.Device.Caps.MaskAlphaFormat ) ;
		MaskBaseImage.Width				= MASKIOS.MaskTextureSizeX ;
		MaskBaseImage.Height			= MASKIOS.MaskTextureSizeY ;
		MaskBaseImage.Pitch				= MASKD.MaskBufferPitch ;
		MaskBaseImage.GraphData			= MASKD.MaskBuffer ;
		MaskBaseImage.MipMapCount		= 0 ;
		MaskBaseImage.GraphDataCount	= 0 ;

		Graphics_iOS_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
			&SrcRect,
			&DestRect,
			MASKIOS.MaskImageTextureBuffer,
			1,
			MASKIOS.MaskTextureSizeX,
			MASKIOS.MaskTextureSizeY,
			MASKIOS.MaskTextureSizeX,
			MASKIOS.MaskTextureSizeY,
			&MaskBaseImage,
			&AlphaBaseImage,
			GIOS.Device.Caps.MaskAlphaFormat,
			FALSE,
			FALSE,
			0,
			FALSE
		) ;
	}

	// マスク用スクリーンテクスチャに今までの内容を転送
	UpdateRect.left   = 0 ;
	UpdateRect.top    = 0 ;
	UpdateRect.right  = MaskBufferSizeXOld ;
	UpdateRect.bottom = MaskBufferSizeYOld ;
	Graphics_iOS_StretchRect(
		MASKIOS.MaskScreenOldTextureBuffer, MaskBufferSizeXOld,        MaskBufferSizeYOld,        &UpdateRect,
		MASKIOS.MaskScreenFrameBuffer,      MASKIOS.MaskTextureSizeX, MASKIOS.MaskTextureSizeY, &UpdateRect
	) ;

	// レンダーターゲットにされていたら変更する
	if( GIOS.Device.State.TargetFrameBuffer == MASKIOS.MaskScreenOldFrameBuffer )
	{
		Graphics_iOS_DeviceState_SetRenderTarget( MASKIOS.MaskScreenFrameBuffer, MASKIOS.MaskTextureSizeX, MASKIOS.MaskTextureSizeY ) ;
	}

	// 以前のマスクスクリーン情報を開放
	{
		if( MASKIOS.MaskScreenOldFrameBuffer != 0 )
		{
			glDeleteFramebuffers( 1, &MASKIOS.MaskScreenOldFrameBuffer ) ;
			MASKIOS.MaskScreenOldFrameBuffer = 0 ;
		}

		if( MASKIOS.MaskScreenOldTextureBuffer != 0 )
		{
			glDeleteTextures( 1, &MASKIOS.MaskScreenOldTextureBuffer ) ;
			MASKIOS.MaskScreenOldTextureBuffer = 0 ;
		}

		if( MASKIOS.MaskScreenOldDepthBuffer != 0 )
		{
			glDeleteRenderbuffers( 1, &MASKIOS.MaskScreenOldDepthBuffer ) ;
			MASKIOS.MaskScreenOldDepthBuffer = 0 ;
		}
	}

	// 正常終了
	return 0 ;
}

// マスクスクリーンを一時削除する
extern	int			Mask_ReleaseSurface_PF( void )
{
	if( MASKIOS.MaskImageTextureBuffer != 0 )
	{
		glDeleteTextures( 1, &MASKIOS.MaskImageTextureBuffer ) ;
		MASKIOS.MaskImageTextureBuffer = 0 ;
	}

	if( MASKIOS.MaskScreenFrameBuffer != 0 )
	{
		glDeleteFramebuffers( 1, &MASKIOS.MaskScreenFrameBuffer ) ;
		MASKIOS.MaskScreenFrameBuffer = 0 ;
	}

	if( MASKIOS.MaskScreenTextureBuffer != 0 )
	{
		glDeleteTextures( 1, &MASKIOS.MaskScreenTextureBuffer ) ;
		MASKIOS.MaskScreenTextureBuffer = 0 ;
	}

	if( MASKIOS.MaskScreenDepthBuffer != 0 )
	{
		glDeleteRenderbuffers( 1, &MASKIOS.MaskScreenDepthBuffer ) ;
		MASKIOS.MaskScreenDepthBuffer = 0 ;
	}

	return 0 ;
}

// マスク使用モードを変更
extern	int			Mask_SetUseMaskScreenFlag_PF( void )
{
	IMAGEDATA     *Image     = NULL ;
	SHADOWMAPDATA *ShadowMap = NULL ;

	// 描画先の画像データアドレスを取得する
	if( GSYS.DrawSetting.ShadowMapDrawSetupRequest )
	{
		ShadowMap = Graphics_ShadowMap_GetData( GSYS.DrawSetting.ShadowMapDrawHandle ) ;
	}
	else
	{
		Image = Graphics_Image_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了する
	Graphics_iOS_RenderEnd() ;

	// 描画先を変更する

	// マスクサーフェスが存在していて且つ有効な場合はマスクサーフェスを描画対象にする
	if( MASKD.MaskValidFlag && MASKIOS.MaskScreenFrameBuffer )
	{
		Graphics_iOS_DeviceState_SetRenderTarget( MASKIOS.MaskScreenFrameBuffer, MASKIOS.MaskTextureSizeX, MASKIOS.MaskTextureSizeY ) ;
	}
	else
	// シャドウマップが有効な場合はシャドウマップを描画対象にする
	if( ShadowMap )
	{
		Graphics_iOS_DeviceState_SetRenderTarget( ShadowMap->PF->FrameBuffer, ShadowMap->PF->Texture.Width, ShadowMap->PF->Texture.Height ) ;
	}
	else
	// 描画可能画像が有効な場合は描画可能画像を描画対象にする
	if( Image )
	{
		if( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget != 0 )
		{
			Graphics_iOS_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
		}
		else
		{
			Graphics_iOS_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->FrameBuffer, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
		}
	}
	else
	// それ以外の場合はサブバックバッファを描画対象にする
	{
		Graphics_iOS_DeviceState_SetRenderTarget( GIOS.Device.Screen.SubBackBufferFrameBuffer, GIOS.Device.Screen.SubBackBufferTextureSizeX, GIOS.Device.Screen.SubBackBufferTextureSizeY ) ;
	}

	// 使用するＺバッファのセットアップ
	Graphics_Screen_SetupUseZBuffer() ;

	// ビューポートを元に戻す
	GIOS.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;
	Graphics_iOS_DeviceState_SetViewportEasy(
		GSYS.DrawSetting.DrawArea.left,
		GSYS.DrawSetting.DrawArea.top,
		GSYS.DrawSetting.DrawArea.right,
		GSYS.DrawSetting.DrawArea.bottom
	) ;
	GIOS.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// 正常終了
	return 0 ;
}

// マスクを使用した描画の前に呼ぶ関数( ついでにサブバッファを使用した描画エリア機能を使用している場合の処理もいれてしまっているよ )
extern	int			Mask_DrawBeginFunction_PF( RECT *Rect )
{
	IMAGEDATA     *Image     = NULL ;
	SHADOWMAPDATA *ShadowMap = NULL ;

	// 描画先の画像データアドレスを取得する
	if( GSYS.DrawSetting.ShadowMapDraw == FALSE )
	{
		ShadowMap = Graphics_ShadowMap_GetData( GSYS.DrawSetting.ShadowMapDrawHandle ) ;
	}
	else
	{
		Image = Graphics_Image_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;
	}

	// マスクを使用しているときのみ特別な処理をする
	if( MASKD.MaskValidFlag && MASKIOS.MaskScreenFrameBuffer )
	{
		// 最終出力先の決定
		
		// 描画可能画像が描画先の場合はそれを、それ以外の場合はサブバックバッファを使用している場合はサブバックバッファを、
		// そうではない場合はバックバッファを出力先にする
		if( ShadowMap )
		{
			MASKIOS.DestTargetTextureBuffer = ShadowMap->PF->Texture.TextureBuffer ;
			MASKIOS.DestTargetFrameBuffer   = ShadowMap->PF->FrameBuffer ;
			MASKIOS.DestTargetWidth         = ShadowMap->PF->Texture.Width ;
			MASKIOS.DestTargetHeight        = ShadowMap->PF->Texture.Height ;
		}
		else
		if( Image )
		{
			MASKIOS.DestTargetTextureBuffer = Image->Hard.Draw[ 0 ].Tex->PF->Texture.TextureBuffer ;
			if( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget != 0 )
			{
				MASKIOS.DestTargetFrameBuffer = Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget ;
			}
			else
			{
				MASKIOS.DestTargetFrameBuffer = Image->Hard.Draw[ 0 ].Tex->PF->FrameBuffer ;
			}
			MASKIOS.DestTargetWidth  = Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width ;
			MASKIOS.DestTargetHeight = Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ;
		}
		else
		{
			MASKIOS.DestTargetTextureBuffer = GIOS.Device.Screen.SubBackBufferTexture ;
			MASKIOS.DestTargetFrameBuffer   = GIOS.Device.Screen.SubBackBufferFrameBuffer ;
			MASKIOS.DestTargetWidth         = GIOS.Device.Screen.SubBackBufferTextureSizeX ;
			MASKIOS.DestTargetHeight        = GIOS.Device.Screen.SubBackBufferTextureSizeY ;
		}

		// 最終出力先からマスクスクリーンに現在の描画状況をコピーする
		Graphics_iOS_StretchRect(
			MASKIOS.DestTargetTextureBuffer, MASKIOS.DestTargetWidth,  MASKIOS.DestTargetHeight, Rect,
			MASKIOS.MaskScreenFrameBuffer,   MASKIOS.MaskTextureSizeX, MASKIOS.MaskTextureSizeY, Rect
		) ; 
	}

	// 正常終了
	return 0 ;
}

// マスクを使用した描画の後に呼ぶ関数( ついでにサブバッファを使用した描画エリア機能を使用している場合の処理もいれてしまっているよ )
extern	int			Mask_DrawAfterFunction_PF( RECT *Rect )
{
	// マスクを使用している場合のみマスク画像と合成して転送
	if( MASKD.MaskValidFlag && MASKIOS.MaskScreenFrameBuffer )
	{
		IMAGEDATA               *MaskScreenImage = NULL ;
		GLuint                  MaskImageTextureBuffer ;
		int						MaskImageTextureWidth ;
		int						MaskImageTextureHeight ;
		GRAPHICS_IOS_SHADER *MaskShader ;
		int                     AlphaBlend ;

		// マスク用グラフィックハンドルが有効な場合はマスク用グラフィックハンドルの情報を取得する
		if( MASKD.MaskScreenGraphHandle != 0 )
		{
			MaskScreenImage = Graphics_Image_GetData( MASKD.MaskScreenGraphHandle ) ;

			// 既に無効になっていたら設定も 0 にする
			if( MaskScreenImage == NULL )
			{
				MASKD.MaskScreenGraphHandle = 0 ;
			}
		}

		// 使用するマスクイメージテクスチャをセット
		if( MaskScreenImage != NULL )
		{
			MaskImageTextureBuffer = MaskScreenImage->Hard.Draw[ 0 ].Tex->PF->Texture.TextureBuffer ;
			MaskImageTextureWidth  = MaskScreenImage->Hard.Draw[ 0 ].Tex->PF->Texture.Width ;
			MaskImageTextureHeight = MaskScreenImage->Hard.Draw[ 0 ].Tex->PF->Texture.Height ;
			MaskShader     = MASKD.MaskReverseEffectFlag ?
				&GIOS.Device.Shader.Base.MaskEffect_UseGraphHandle_ReverseEffect_Shader[ MASKD.MaskScreenGraphHandleUseChannel ] :
				&GIOS.Device.Shader.Base.MaskEffect_UseGraphHandle_Shader[ MASKD.MaskScreenGraphHandleUseChannel ] ;
			AlphaBlend             = TRUE ;
		}
		else
		{
			MaskImageTextureBuffer = MASKIOS.MaskImageTextureBuffer ;
			MaskImageTextureWidth  = MASKIOS.MaskTextureSizeX ;
			MaskImageTextureHeight = MASKIOS.MaskTextureSizeY ;
			MaskShader             = MASKD.MaskReverseEffectFlag ? &GIOS.Device.Shader.Base.MaskEffect_ReverseEffect_Shader : &GIOS.Device.Shader.Base.MaskEffect_Shader ;
			AlphaBlend             = FALSE ;
		}

		Graphics_iOS_StretchRect(
			MASKIOS.MaskScreenTextureBuffer, MASKIOS.MaskTextureSizeX, MASKIOS.MaskTextureSizeY, Rect,
			MASKIOS.DestTargetFrameBuffer,   MASKIOS.DestTargetWidth,  MASKIOS.DestTargetHeight, Rect,
			GL_NEAREST,
			AlphaBlend,
			MaskShader,
			MaskImageTextureBuffer,           MaskImageTextureWidth,     MaskImageTextureHeight,    Rect
		) ;
	}

	return 0 ;
}

// マスクスクリーンを指定の色で塗りつぶす
extern	int			Mask_FillMaskScreen_PF( int Flag )
{
//	int i ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了する
	Graphics_iOS_RenderEnd() ;

	// マスク用イメージの転送
	{
		RECT SrcRect ;
		RECT DestRect ;
		BASEIMAGE MaskBaseImage ;
		BASEIMAGE AlphaBaseImage ;

		SrcRect.left   = 0 ;
		SrcRect.top    = 0 ;
		SrcRect.right  = MASKIOS.MaskTextureSizeX ;
		SrcRect.bottom = MASKIOS.MaskTextureSizeY ;
		DestRect = SrcRect ;

		_MEMSET( &AlphaBaseImage, 0, sizeof( AlphaBaseImage ) ) ;
		MaskBaseImage.ColorData			= *Graphics_iOS_GetDataFormatColorData( GIOS.Device.Caps.MaskAlphaFormat ) ;
		MaskBaseImage.Width				= MASKIOS.MaskTextureSizeX ;
		MaskBaseImage.Height			= MASKIOS.MaskTextureSizeY ;
		MaskBaseImage.Pitch				= MASKD.MaskBufferPitch ;
		MaskBaseImage.GraphData			= MASKD.MaskBuffer ;
		MaskBaseImage.MipMapCount		= 0 ;
		MaskBaseImage.GraphDataCount	= 0 ;

		Graphics_iOS_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
			&SrcRect,
			&DestRect,
			MASKIOS.MaskImageTextureBuffer,
			1,
			MASKIOS.MaskTextureSizeX,
			MASKIOS.MaskTextureSizeY,
			MASKIOS.MaskTextureSizeX,
			MASKIOS.MaskTextureSizeY,
			&MaskBaseImage,
			&AlphaBaseImage,
			GIOS.Device.Caps.MaskAlphaFormat,
			FALSE,
			FALSE,
			0,
			FALSE
		) ;
	}

	return 0 ;
}

// 指定領域のマスクイメージテクスチャを更新する
extern	int			Mask_UpdateMaskImageTexture_PF( RECT *Rect )
{
//	int i ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了する
	Graphics_iOS_RenderEnd() ;

	// マスク用イメージの転送
	{
		BASEIMAGE MaskBaseImage ;
		BASEIMAGE AlphaBaseImage ;

		_MEMSET( &AlphaBaseImage, 0, sizeof( AlphaBaseImage ) ) ;
		MaskBaseImage.ColorData			= *Graphics_iOS_GetDataFormatColorData( GIOS.Device.Caps.MaskAlphaFormat ) ;
		MaskBaseImage.Width				= MASKIOS.MaskTextureSizeX ;
		MaskBaseImage.Height			= MASKIOS.MaskTextureSizeY ;
		MaskBaseImage.Pitch				= MASKD.MaskBufferPitch ;
		MaskBaseImage.GraphData			= MASKD.MaskBuffer ;
		MaskBaseImage.MipMapCount		= 0 ;
		MaskBaseImage.GraphDataCount	= 0 ;

		Graphics_iOS_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
			Rect,
			Rect,
			MASKIOS.MaskImageTextureBuffer,
			1,
			MASKIOS.MaskTextureSizeX,
			MASKIOS.MaskTextureSizeY,
			MASKIOS.MaskTextureSizeX,
			MASKIOS.MaskTextureSizeY,
			&MaskBaseImage,
			&AlphaBaseImage,
			GIOS.Device.Caps.MaskAlphaFormat,
			FALSE,
			FALSE,
			0,
			FALSE
		) ;
	}

	return 0 ;
}






#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MASK

//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用マスクデータ管理プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_MASK

// インクルード ---------------------------------------------------------------
#include "DxGraphicsAndroid.h"
#include "DxMaskAndroid.h"
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

MASKMANAGEDATA_ANDR MaskManageData_Android ;

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

// 環境依存関数

// マスクスクリーンを作成する関数
extern	int			Mask_CreateScreenFunction_Timing0_PF( void )
{
	MASKANDR.MaskScreenOldFrameBuffer   = MASKANDR.MaskScreenFrameBuffer ;
	MASKANDR.MaskScreenOldTextureBuffer = MASKANDR.MaskScreenTextureBuffer ;
	MASKANDR.MaskScreenOldDepthBuffer   = MASKANDR.MaskScreenDepthBuffer ;

	if( MASKANDR.MaskImageTextureBuffer != 0 )
	{
		glDeleteTextures( 1, &MASKANDR.MaskImageTextureBuffer ) ;
		MASKANDR.MaskImageTextureBuffer = 0 ;
	}

	// 正常終了
	return 0 ;
}

// マスクスクリーンを作成する関数
extern	int			Mask_CreateScreenFunction_Timing1_PF( int Width, int Height )
{
	MASKANDR.MaskTextureSizeX = Width ;
	MASKANDR.MaskTextureSizeY = Height ;

	// マスク用イメージテクスチャの作成
	if( MASKANDR.MaskImageTextureBuffer == 0 )
	{
		PIXELFORMAT_INFO_ANDR *PixelFormat ;

		PixelFormat = &g_AndroidPixelFormat[ GANDR.Device.Caps.MaskAlphaFormat ] ;
		glGenTextures( 1, &MASKANDR.MaskImageTextureBuffer ) ;
		glActiveTexture( GL_TEXTURE0 ) ;
		glBindTexture( GL_TEXTURE_2D, MASKANDR.MaskImageTextureBuffer ) ;

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
				MASKANDR.MaskTextureSizeX,
				MASKANDR.MaskTextureSizeY,
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
				MASKANDR.MaskTextureSizeX,
				MASKANDR.MaskTextureSizeY,
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
			SrcRect.right  = MASKANDR.MaskTextureSizeX ;
			SrcRect.bottom = MASKANDR.MaskTextureSizeY ;
			DestRect = SrcRect ;

			_MEMSET( &AlphaBaseImage, 0, sizeof( AlphaBaseImage ) ) ;
			MaskBaseImage.ColorData			= *Graphics_Android_GetDataFormatColorData( GANDR.Device.Caps.MaskAlphaFormat ) ;
			MaskBaseImage.Width				= MASKANDR.MaskTextureSizeX ;
			MaskBaseImage.Height			= MASKANDR.MaskTextureSizeY ;
			MaskBaseImage.Pitch				= MASKD.MaskBufferPitch ;
			MaskBaseImage.GraphData			= MASKD.MaskBuffer ;
			MaskBaseImage.MipMapCount		= 0 ;
			MaskBaseImage.GraphDataCount	= 0 ;

			Graphics_Android_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
				&SrcRect,
				&DestRect,
				MASKANDR.MaskImageTextureBuffer,
				1,
				MASKANDR.MaskTextureSizeX,
				MASKANDR.MaskTextureSizeY,
				MASKANDR.MaskTextureSizeX,
				MASKANDR.MaskTextureSizeY,
				&MaskBaseImage,
				&AlphaBaseImage,
				GANDR.Device.Caps.MaskAlphaFormat,
				FALSE,
				FALSE,
				0,
				FALSE
			) ;
		}
	}

	// マスク用スクリーンテクスチャの作成
	if( MASKANDR.MaskScreenFrameBuffer == 0 )
	{
		// Offscreen position framebuffer texture target
		glGenTextures( 1, &MASKANDR.MaskScreenTextureBuffer ) ;
		glActiveTexture( GL_TEXTURE0 ) ;
		glBindTexture( GL_TEXTURE_2D, MASKANDR.MaskScreenTextureBuffer ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) ;
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, MASKANDR.MaskTextureSizeX, MASKANDR.MaskTextureSizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 ) ;

		glGenFramebuffers( 1, &MASKANDR.MaskScreenFrameBuffer ) ;
		glBindFramebuffer( GL_FRAMEBUFFER, MASKANDR.MaskScreenFrameBuffer ) ;

		// レンダーバッファ識別子を生成します。
		glGenRenderbuffers( 1, &MASKANDR.MaskScreenDepthBuffer ) ;

		// レンダーバッファ識別子に対応したレンダーバッファオブジェクトを生成します。
		glBindRenderbuffer( GL_RENDERBUFFER, MASKANDR.MaskScreenDepthBuffer ) ;

		// レンダーバッファの幅と高さを指定します。
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, MASKANDR.MaskTextureSizeX, MASKANDR.MaskTextureSizeY ) ;

		// フレームバッファのアタッチメントとしてレンダーバッファをアタッチします。
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, MASKANDR.MaskScreenDepthBuffer ) ;

		// フレームバッファのアタッチメントとして 2D テクスチャをアタッチします。
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, MASKANDR.MaskScreenTextureBuffer, 0 ) ;

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
	Graphics_Android_RenderEnd() ;

	// マスク用イメージの転送
	{
		RECT SrcRect ;
		RECT DestRect ;
		BASEIMAGE MaskBaseImage ;
		BASEIMAGE AlphaBaseImage ;

		SrcRect.left   = 0 ;
		SrcRect.top    = 0 ;
		SrcRect.right  = MASKANDR.MaskTextureSizeX ;
		SrcRect.bottom = MASKANDR.MaskTextureSizeY ;
		DestRect = SrcRect ;

		_MEMSET( &AlphaBaseImage, 0, sizeof( AlphaBaseImage ) ) ;
		MaskBaseImage.ColorData			= *Graphics_Android_GetDataFormatColorData( GANDR.Device.Caps.MaskAlphaFormat ) ;
		MaskBaseImage.Width				= MASKANDR.MaskTextureSizeX ;
		MaskBaseImage.Height			= MASKANDR.MaskTextureSizeY ;
		MaskBaseImage.Pitch				= MASKD.MaskBufferPitch ;
		MaskBaseImage.GraphData			= MASKD.MaskBuffer ;
		MaskBaseImage.MipMapCount		= 0 ;
		MaskBaseImage.GraphDataCount	= 0 ;

		Graphics_Android_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
			&SrcRect,
			&DestRect,
			MASKANDR.MaskImageTextureBuffer,
			1,
			MASKANDR.MaskTextureSizeX,
			MASKANDR.MaskTextureSizeY,
			MASKANDR.MaskTextureSizeX,
			MASKANDR.MaskTextureSizeY,
			&MaskBaseImage,
			&AlphaBaseImage,
			GANDR.Device.Caps.MaskAlphaFormat,
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
	Graphics_Android_StretchRect(
		MASKANDR.MaskScreenOldTextureBuffer, MaskBufferSizeXOld,        MaskBufferSizeYOld,        &UpdateRect,
		MASKANDR.MaskScreenFrameBuffer,      MASKANDR.MaskTextureSizeX, MASKANDR.MaskTextureSizeY, &UpdateRect
	) ;

	// レンダーターゲットにされていたら変更する
	if( GANDR.Device.State.TargetFrameBuffer == MASKANDR.MaskScreenOldFrameBuffer )
	{
		Graphics_Android_DeviceState_SetRenderTarget( MASKANDR.MaskScreenFrameBuffer, MASKANDR.MaskTextureSizeX, MASKANDR.MaskTextureSizeY ) ;
	}

	// 以前のマスクスクリーン情報を開放
	{
		if( MASKANDR.MaskScreenOldFrameBuffer != 0 )
		{
			glDeleteFramebuffers( 1, &MASKANDR.MaskScreenOldFrameBuffer ) ;
			MASKANDR.MaskScreenOldFrameBuffer = 0 ;
		}

		if( MASKANDR.MaskScreenOldTextureBuffer != 0 )
		{
			glDeleteTextures( 1, &MASKANDR.MaskScreenOldTextureBuffer ) ;
			MASKANDR.MaskScreenOldTextureBuffer = 0 ;
		}

		if( MASKANDR.MaskScreenOldDepthBuffer != 0 )
		{
			glDeleteRenderbuffers( 1, &MASKANDR.MaskScreenOldDepthBuffer ) ;
			MASKANDR.MaskScreenOldDepthBuffer = 0 ;
		}
	}

	// 正常終了
	return 0 ;
}

// マスクスクリーンを一時削除する
extern	int			Mask_ReleaseSurface_PF( void )
{
	if( MASKANDR.MaskImageTextureBuffer != 0 )
	{
		glDeleteTextures( 1, &MASKANDR.MaskImageTextureBuffer ) ;
		MASKANDR.MaskImageTextureBuffer = 0 ;
	}

	if( MASKANDR.MaskScreenFrameBuffer != 0 )
	{
		glDeleteFramebuffers( 1, &MASKANDR.MaskScreenFrameBuffer ) ;
		MASKANDR.MaskScreenFrameBuffer = 0 ;
	}

	if( MASKANDR.MaskScreenTextureBuffer != 0 )
	{
		glDeleteTextures( 1, &MASKANDR.MaskScreenTextureBuffer ) ;
		MASKANDR.MaskScreenTextureBuffer = 0 ;
	}

	if( MASKANDR.MaskScreenDepthBuffer != 0 )
	{
		glDeleteRenderbuffers( 1, &MASKANDR.MaskScreenDepthBuffer ) ;
		MASKANDR.MaskScreenDepthBuffer = 0 ;
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
	Graphics_Android_RenderEnd() ;

	// 描画先を変更する

	// マスクサーフェスが存在していて且つ有効な場合はマスクサーフェスを描画対象にする
	if( MASKD.MaskValidFlag && MASKANDR.MaskScreenFrameBuffer )
	{
		Graphics_Android_DeviceState_SetRenderTarget( MASKANDR.MaskScreenFrameBuffer, MASKANDR.MaskTextureSizeX, MASKANDR.MaskTextureSizeY ) ;
	}
	else
	// シャドウマップが有効な場合はシャドウマップを描画対象にする
	if( ShadowMap )
	{
		Graphics_Android_DeviceState_SetRenderTarget( ShadowMap->PF->FrameBuffer, ShadowMap->PF->Texture.Width, ShadowMap->PF->Texture.Height ) ;
	}
	else
	// 描画可能画像が有効な場合は描画可能画像を描画対象にする
	if( Image )
	{
		if( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget != 0 )
		{
			Graphics_Android_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
		}
		else
		{
			Graphics_Android_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->FrameBuffer, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
		}
	}
	else
	// それ以外の場合はサブバックバッファを描画対象にする
	{
		Graphics_Android_DeviceState_SetRenderTarget( GANDR.Device.Screen.SubBackBufferFrameBuffer, GANDR.Device.Screen.SubBackBufferTextureSizeX, GANDR.Device.Screen.SubBackBufferTextureSizeY ) ;
	}

	// 使用するＺバッファのセットアップ
	Graphics_Screen_SetupUseZBuffer() ;

	// ビューポートを元に戻す
	GANDR.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;
	Graphics_Android_DeviceState_SetViewportEasy(
		GSYS.DrawSetting.DrawArea.left,
		GSYS.DrawSetting.DrawArea.top,
		GSYS.DrawSetting.DrawArea.right,
		GSYS.DrawSetting.DrawArea.bottom
	) ;
	GANDR.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

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
	if( MASKD.MaskValidFlag && MASKANDR.MaskScreenFrameBuffer )
	{
		// 最終出力先の決定
		
		// 描画可能画像が描画先の場合はそれを、それ以外の場合はサブバックバッファを使用している場合はサブバックバッファを、
		// そうではない場合はバックバッファを出力先にする
		if( ShadowMap )
		{
			MASKANDR.DestTargetTextureBuffer = ShadowMap->PF->Texture.TextureBuffer ;
			MASKANDR.DestTargetFrameBuffer   = ShadowMap->PF->FrameBuffer ;
			MASKANDR.DestTargetWidth         = ShadowMap->PF->Texture.Width ;
			MASKANDR.DestTargetHeight        = ShadowMap->PF->Texture.Height ;
		}
		else
		if( Image )
		{
			MASKANDR.DestTargetTextureBuffer = Image->Hard.Draw[ 0 ].Tex->PF->Texture.TextureBuffer ;
			if( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget != 0 )
			{
				MASKANDR.DestTargetFrameBuffer = Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget ;
			}
			else
			{
				MASKANDR.DestTargetFrameBuffer = Image->Hard.Draw[ 0 ].Tex->PF->FrameBuffer ;
			}
			MASKANDR.DestTargetWidth  = Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width ;
			MASKANDR.DestTargetHeight = Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ;
		}
		else
		{
			MASKANDR.DestTargetTextureBuffer = GANDR.Device.Screen.SubBackBufferTexture ;
			MASKANDR.DestTargetFrameBuffer   = GANDR.Device.Screen.SubBackBufferFrameBuffer ;
			MASKANDR.DestTargetWidth         = GANDR.Device.Screen.SubBackBufferTextureSizeX ;
			MASKANDR.DestTargetHeight        = GANDR.Device.Screen.SubBackBufferTextureSizeY ;
		}

		// 最終出力先からマスクスクリーンに現在の描画状況をコピーする
		Graphics_Android_StretchRect(
			MASKANDR.DestTargetTextureBuffer, MASKANDR.DestTargetWidth,  MASKANDR.DestTargetHeight, Rect,
			MASKANDR.MaskScreenFrameBuffer,   MASKANDR.MaskTextureSizeX, MASKANDR.MaskTextureSizeY, Rect
		) ; 
	}

	// 正常終了
	return 0 ;
}

// マスクを使用した描画の後に呼ぶ関数( ついでにサブバッファを使用した描画エリア機能を使用している場合の処理もいれてしまっているよ )
extern	int			Mask_DrawAfterFunction_PF( RECT *Rect )
{
	// マスクを使用している場合のみマスク画像と合成して転送
	if( MASKD.MaskValidFlag && MASKANDR.MaskScreenFrameBuffer )
	{
		IMAGEDATA               *MaskScreenImage = NULL ;
		GLuint                  MaskImageTextureBuffer ;
		int						MaskImageTextureWidth ;
		int						MaskImageTextureHeight ;
		GRAPHICS_ANDROID_SHADER *MaskShader ;
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
				&GANDR.Device.Shader.Base.MaskEffect_UseGraphHandle_ReverseEffect_Shader[ MASKD.MaskScreenGraphHandleUseChannel ] :
				&GANDR.Device.Shader.Base.MaskEffect_UseGraphHandle_Shader[ MASKD.MaskScreenGraphHandleUseChannel ] ;
			AlphaBlend             = TRUE ;
		}
		else
		{
			MaskImageTextureBuffer = MASKANDR.MaskImageTextureBuffer ;
			MaskImageTextureWidth  = MASKANDR.MaskTextureSizeX ;
			MaskImageTextureHeight = MASKANDR.MaskTextureSizeY ;
			MaskShader             = MASKD.MaskReverseEffectFlag ? &GANDR.Device.Shader.Base.MaskEffect_ReverseEffect_Shader : &GANDR.Device.Shader.Base.MaskEffect_Shader ;
			AlphaBlend             = FALSE ;
		}

		Graphics_Android_StretchRect(
			MASKANDR.MaskScreenTextureBuffer, MASKANDR.MaskTextureSizeX, MASKANDR.MaskTextureSizeY, Rect,
			MASKANDR.DestTargetFrameBuffer,   MASKANDR.DestTargetWidth,  MASKANDR.DestTargetHeight, Rect,
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
	Graphics_Android_RenderEnd() ;

	// マスク用イメージの転送
	{
		RECT SrcRect ;
		RECT DestRect ;
		BASEIMAGE MaskBaseImage ;
		BASEIMAGE AlphaBaseImage ;

		SrcRect.left   = 0 ;
		SrcRect.top    = 0 ;
		SrcRect.right  = MASKANDR.MaskTextureSizeX ;
		SrcRect.bottom = MASKANDR.MaskTextureSizeY ;
		DestRect = SrcRect ;

		_MEMSET( &AlphaBaseImage, 0, sizeof( AlphaBaseImage ) ) ;
		MaskBaseImage.ColorData			= *Graphics_Android_GetDataFormatColorData( GANDR.Device.Caps.MaskAlphaFormat ) ;
		MaskBaseImage.Width				= MASKANDR.MaskTextureSizeX ;
		MaskBaseImage.Height			= MASKANDR.MaskTextureSizeY ;
		MaskBaseImage.Pitch				= MASKD.MaskBufferPitch ;
		MaskBaseImage.GraphData			= MASKD.MaskBuffer ;
		MaskBaseImage.MipMapCount		= 0 ;
		MaskBaseImage.GraphDataCount	= 0 ;

		Graphics_Android_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
			&SrcRect,
			&DestRect,
			MASKANDR.MaskImageTextureBuffer,
			1,
			MASKANDR.MaskTextureSizeX,
			MASKANDR.MaskTextureSizeY,
			MASKANDR.MaskTextureSizeX,
			MASKANDR.MaskTextureSizeY,
			&MaskBaseImage,
			&AlphaBaseImage,
			GANDR.Device.Caps.MaskAlphaFormat,
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
	Graphics_Android_RenderEnd() ;

	// マスク用イメージの転送
	{
		BASEIMAGE MaskBaseImage ;
		BASEIMAGE AlphaBaseImage ;

		_MEMSET( &AlphaBaseImage, 0, sizeof( AlphaBaseImage ) ) ;
		MaskBaseImage.ColorData			= *Graphics_Android_GetDataFormatColorData( GANDR.Device.Caps.MaskAlphaFormat ) ;
		MaskBaseImage.Width				= MASKANDR.MaskTextureSizeX ;
		MaskBaseImage.Height			= MASKANDR.MaskTextureSizeY ;
		MaskBaseImage.Pitch				= MASKD.MaskBufferPitch ;
		MaskBaseImage.GraphData			= MASKD.MaskBuffer ;
		MaskBaseImage.MipMapCount		= 0 ;
		MaskBaseImage.GraphDataCount	= 0 ;

		Graphics_Android_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
			Rect,
			Rect,
			MASKANDR.MaskImageTextureBuffer,
			1,
			MASKANDR.MaskTextureSizeX,
			MASKANDR.MaskTextureSizeY,
			MASKANDR.MaskTextureSizeX,
			MASKANDR.MaskTextureSizeY,
			&MaskBaseImage,
			&AlphaBaseImage,
			GANDR.Device.Caps.MaskAlphaFormat,
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

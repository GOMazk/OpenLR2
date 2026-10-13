//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用フォント関係プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_FONT

// インクルード ---------------------------------------------------------------
#include "DxFontiOS.h"
#include "DxBaseFunciOS.h"
#include "DxSystemiOS.h"
#include "DxSystemiOS_ObjC.h"
#include "../DxFont.h"
#include "../DxMemory.h"
#include "../DxBaseFunc.h"
#include "../DxSystem.h"
#include "../DxArchive_.h"
#include "../DxLog.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

FONTSYSTEM_IOS FontSystemiOS ;

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

// InitFontManage の環境依存処理を行う関数
extern int InitFontManage_PF( void )
{
	// 特に何もしない
	return 0 ;
}

// TermFontManage の環境依存処理を行う関数
extern int TermFontManage_PF( void )
{
	// 特に何もしない
	return 0 ;
}

// CreateFontToHandle の環境依存処理を行う関数
extern int CreateFontToHandle_PF( CREATEFONTTOHANDLE_GPARAM *GParam, FONTMANAGE *ManageData, int DefaultCharSet )
{
	// フォントデータファイルを使用する場合は何もせずに終了
	if( ManageData->UseFontDataFile )
	{
		return 0 ;
	}

	int res = -1 ;

	// フォントデータの作成
	{
		// フォント名を UTF8 に変換する
		char UTF8_FontName[ 128 * 4 ] ;
		int UTF8_FontNameLength ;

		UTF8_FontNameLength = ConvString( ( char * )ManageData->FontName, -1, WCHAR_T_CHARCODEFORMAT, UTF8_FontName, sizeof( UTF8_FontName ), DX_CHARCODEFORMAT_UTF8 ) ;
		ManageData->PF->FontData = CreateDxiOSFontData( UTF8_FontNameLength > 0 ? UTF8_FontName : NULL, ManageData->BaseInfo.FontSize ) ;
		if( ManageData->PF->FontData == NULL )
		{
			return -1 ;
		}
	}

	res = 0 ;

	// 成功時はパラメータをセット
	{
		int ImageWidth, ImageHeight, ImagePitch ;
		float FontAscent, FontDescent, FontLeading, FontUnderlinePosition, FontUnderlineThickness, FontCapHeight, FontXHeight ;

		GetDxiOSFontBaseInfo( ManageData->PF->FontData, &ImageWidth, &ImageHeight, &ImagePitch, &FontAscent, &FontDescent, &FontLeading, &FontUnderlinePosition, &FontUnderlineThickness, &FontCapHeight, &FontXHeight ) ;
        
		ManageData->BaseInfo.FontHeight    = ImageHeight ;
		ManageData->BaseInfo.FontAddHeight = 0 ;
		ManageData->BaseInfo.MaxWidth      = ImageWidth > ImageHeight ? ImageWidth : ImageHeight ;
		ManageData->BaseInfo.Ascent        = 0 /* -ManageData->PF->PaintFontMetrics_ascent */ ;
	}

	// 戻り値を返す
	return res ;
}

// CreateFontToHandle の環境依存エラー処理を行う関数
extern int CreateFontToHandle_Error_PF( FONTMANAGE * ManageData )
{
	// フォントデータの解放
	if( ManageData->PF->FontData != NULL )
	{
		DeleteDxiOSFontData( ManageData->PF->FontData ) ;
		ManageData->PF->FontData = NULL ;
	}

	// 終了
	return 0 ;
}

// TerminateFontHandle の環境依存処理を行う関数
extern int TerminateFontHandle_PF( FONTMANAGE *ManageData )
{
    // フォントデータの解放
	if( ManageData->PF->FontData != NULL )
	{
		DeleteDxiOSFontData( ManageData->PF->FontData ) ;
		ManageData->PF->FontData = NULL ;
	}

	// 終了
	return 0 ;
}

// SetupFontCache の環境依存処理を行う関数
extern int SetupFontCache_PF( CREATEFONTTOHANDLE_GPARAM * /* GParam */, FONTMANAGE * /* ManageData */, int /* ASyncThread */ )
{
	// 特に何もしない
	return 0 ;
}

// FontCacheCharaAddToHandleの環境依存処理を行う関数( 実行箇所区別 0 )
extern int FontCacheCharAddToHandle_Timing0_PF( FONTMANAGE *ManageData )
{
	// とくにすること無し
	return 0 ;
}

// FontCacheCharaAddToHandleの環境依存処理を行う関数( 実行箇所区別 1 )
extern int FontCacheCharAddToHandle_Timing1_PF( FONTMANAGE *ManageData, FONTCHARDATA *CharData, DWORD CharCode, DWORD IVSCode, int TextureCacheUpdate )
{
	char UTF8CharCode[ 32 ] ;
	wchar_t WCharCode[ 8 ] ;
	int CharNum ;
	int	Space ;
	void *ImageData ;
	int ImageDrawX, ImageDrawY, ImageSizeX, ImageSizeY, ImagePitch, ImageAddX ;

	// UTF8文字列に変換する
	CharNum = PutCharCode( CharCode, IOS_WCHAR_CHARCODEFORMAT, ( char * )WCharCode, sizeof( WCharCode ) ) / sizeof( wchar_t ) ;
	ConvString( ( char * )WCharCode, CharNum, IOS_WCHAR_CHARCODEFORMAT, UTF8CharCode, sizeof( UTF8CharCode ), DX_CHARCODEFORMAT_UTF8 ) ;

	// 文字のセットアップを行う
	SetupDxiOSFontImage( ManageData->PF->FontData, UTF8CharCode ) ;

	// スペースかどうかを取得しておく
	Space = CharCode == L' ' ? 1 : ( CharCode == FSYS.DoubleByteSpaceCharCode ? 2 : 0 ) ;

	// 情報を取得
	ImageData = GetDxiOSFontImage( ManageData->PF->FontData, &ImageDrawX, &ImageDrawY, &ImageSizeX, &ImageSizeY, &ImagePitch, &ImageAddX ) ;

	// スペース文字だった場合
	if( Space != 0 )
	{
		FontCacheCharImageBltToHandle(
			ManageData,
			CharData,
			CharCode, 
			IVSCode,
			TRUE,
			DX_FONT_SRCIMAGETYPE_8BIT_MAX255,
			NULL,
			0,
			0,
			0,
			0,
			0,
			Space * ManageData->BaseInfo.FontSize / 2,
			FALSE
		) ;
	}
	else
	{
        int ImageWidth, ImageHeight, ImagePitch ;
        int UnitHeight ;
        float FontAscent, FontDescent, FontLeading, FontUnderlinePosition, FontUnderlineThickness, FontCapHeight, FontXHeight ;
        
        GetDxiOSFontBaseInfo( ManageData->PF->FontData, &ImageWidth, &ImageHeight, &ImagePitch, &FontAscent, &FontDescent, &FontLeading, &FontUnderlinePosition, &FontUnderlineThickness, &FontCapHeight, &FontXHeight ) ;
        
        UnitHeight = ( int )( FontAscent + FontDescent ) ;
        UnitHeight = UnitHeight + UnitHeight / 4 ;
        if( UnitHeight > ImageHeight - ImageDrawY )
        {
            UnitHeight = ImageHeight - ImageDrawY ;
        }
        
		// イメージを転送
		FontCacheCharImageBltToHandle(
			ManageData,
			CharData,
			CharCode, 
			IVSCode,
			FALSE,
			DX_FONT_SRCIMAGETYPE_8BIT_MAX255,
			( BYTE * )ImageData + ImageDrawX + ImageDrawY * ImagePitch,
			ImageSizeX,
			UnitHeight,
			ImagePitch,
			ImageDrawX,
			ImageDrawY,
			ImageAddX,
			TextureCacheUpdate
		) ;
	}

END :

	// 戻り値を返す
	return 0 ;
}

// FontCacheCharaAddToHandleの環境依存処理を行う関数( 実行箇所区別 2 )
extern int FontCacheCharAddToHandle_Timing2_PF( FONTMANAGE *ManageData )
{
	// 特に何もしない
	return 0 ;
}

// EnumFontName の環境依存処理を行う関数
extern int EnumFontName_PF( ENUMFONTDATA *EnumFontData, int IsEx, int CharSet )
{
	// 正常終了
	return 0 ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


#endif // DX_NON_FONT


//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Linux用フォント関係プログラム
// 
//  	Ver 3.24f
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_FONT

// インクルード ---------------------------------------------------------------
#include "DxFontLinux.h"
#include "DxBaseFuncLinux.h"
#include "DxSystemLinux.h"
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

struct DXLIB_LINUX_FONTINFO
{
    void*/*CGColorSpaceRef*/                 ColorSpace ;
    void*/*CGContextRef*/                    BitmapContext ;
    void*/*CGMutablePathRef*/                RectPath ;
    void*/*CTFontRef*/                       Font ;
    float                           FontAscent ;
    float                           FontDescent ;
    float                           FontLeading ;
    float                           FontUnderlinePosition ;
    float                           FontUnderlineThickness ;
    float                           FontCapHeight ;
    float                           FontXHeight ;
    void *                          ImageData ;
    int                             ImageWidth ;
    int                             ImagePitch ;
    int                             ImageHeight ;
    int                             ImageSizeX ;
    int                             ImageSizeY ;
    int                             ImageDrawX ;
    int                             ImageDrawY ;
    int                             ImageAddX ;
} ;

// データ定義 -----------------------------------------------------------------

FONTSYSTEM_LINUX FontSystemLinux ;

// 関数宣言 -------------------------------------------------------------------

void * GetDxLinuxFontImage( void *FontData, int *ImageDrawX, int *ImageDrawY, int *ImageSizeX, int *ImageSizeY, int *ImagePitch, int *ImageAddX )
{
    DXLIB_LINUX_FONTINFO *Font = ( DXLIB_LINUX_FONTINFO * )FontData ;

    if( ImageDrawX ) *ImageDrawX = Font->ImageDrawX ;
    if( ImageDrawY ) *ImageDrawY = Font->ImageDrawY ;
    if( ImageSizeX ) *ImageSizeX = Font->ImageSizeX ;
    if( ImageSizeY ) *ImageSizeY = Font->ImageSizeY ;
    if( ImagePitch ) *ImagePitch = Font->ImagePitch ;
    if( ImageAddX  ) *ImageAddX  = Font->ImageAddX ;

    return Font->ImageData ;
}


// プログラム -----------------------------------------------------------------

// フォントデータの作成
void * CreateDxLinuxFontData( const char *FontName, int FontSize )
{
    return NULL;  // TODO
}


// フォントデータの削除
int DeleteDxLinuxFontData( void *FontData )
{
    return -1 ;  // TODO
}

// フォントデータの基本情報を取得
void GetDxLinuxFontBaseInfo( void *FontData, int *ImageWidth, int *ImageHeight, int *ImagePitch, float *FontAscent, float *FontDescent, float *FontLeading, float *FontUnderlinePosition, float *FontUnderlineThickness, float *FontCapHeight, float *FontXHeight )
{
    DXLIB_LINUX_FONTINFO *Font = ( DXLIB_LINUX_FONTINFO * )FontData ;

    if( ImageWidth              ) *ImageWidth               = Font->ImageWidth ;
    if( ImageHeight             ) *ImageHeight              = Font->ImageHeight ;
    if( ImagePitch              ) *ImagePitch               = Font->ImagePitch ;
    if( FontAscent              ) *FontAscent               = Font->FontAscent ;
    if( FontDescent             ) *FontDescent              = Font->FontDescent ;
    if( FontLeading             ) *FontLeading              = Font->FontLeading ;
    if( FontUnderlinePosition   ) *FontUnderlinePosition    = Font->FontUnderlinePosition ;
    if( FontUnderlineThickness  ) *FontUnderlineThickness   = Font->FontUnderlineThickness ;
    if( FontCapHeight           ) *FontCapHeight            = Font->FontCapHeight ;
    if( FontXHeight             ) *FontXHeight              = Font->FontXHeight ;
}

// フォントデータを下に文字画像のセットアップ
int SetupDxLinuxFontImage( void *FontData, const char *Chara )
{
    return -1 ;  // TODO
}


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

	return -1;  // TODO
}

// CreateFontToHandle の環境依存エラー処理を行う関数
extern int CreateFontToHandle_Error_PF( FONTMANAGE * ManageData )
{
	// フォントデータの解放
	if( ManageData->PF->FontData != NULL )
	{
		DeleteDxLinuxFontData( ManageData->PF->FontData ) ;
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
		DeleteDxLinuxFontData( ManageData->PF->FontData ) ;
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
	CharNum = PutCharCode( CharCode, LINUX_WCHAR_CHARCODEFORMAT, ( char * )WCharCode, sizeof( WCharCode ) ) / sizeof( wchar_t ) ;
	ConvString( ( char * )WCharCode, CharNum, LINUX_WCHAR_CHARCODEFORMAT, UTF8CharCode, sizeof( UTF8CharCode ), DX_CHARCODEFORMAT_UTF8 ) ;

	// 文字のセットアップを行う
	SetupDxLinuxFontImage( ManageData->PF->FontData, UTF8CharCode ) ;

	// スペースかどうかを取得しておく
	Space = CharCode == L' ' ? 1 : ( CharCode == FSYS.DoubleByteSpaceCharCode ? 2 : 0 ) ;

	// 情報を取得
	ImageData = GetDxLinuxFontImage( ManageData->PF->FontData, &ImageDrawX, &ImageDrawY, &ImageSizeX, &ImageSizeY, &ImagePitch, &ImageAddX ) ;

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
        
        GetDxLinuxFontBaseInfo( ManageData->PF->FontData, &ImageWidth, &ImageHeight, &ImagePitch, &FontAscent, &FontDescent, &FontLeading, &FontUnderlinePosition, &FontUnderlineThickness, &FontCapHeight, &FontXHeight ) ;
        
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


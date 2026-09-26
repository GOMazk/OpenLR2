//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		WindowsOS用BaseImageプログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

// インクルード ---------------------------------------------------------------
#include "DxBaseImageWin.h"
#include "DxGuid.h"
#include "DxWinAPI.h"
#include "../DxFile.h"
#include "../DxBaseFunc.h"
#include "../DxMemory.h"
#include "../DxBaseImage.h"
#include "../DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

typedef struct tagBASEIMAGEMANAGE_WIN
{
	D_IWICImagingFactory *WICImagingFactory ;
} BASEIMAGEMANAGE_WIN ;

// 関数宣言 -------------------------------------------------------------------

static	int LoadWICProcess( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly ) ;						// WICによる読み込み

// データ定義 -----------------------------------------------------------------

int ( *DefaultImageLoadFunc_PF[] )( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly ) =
{
	LoadWICProcess ,
	NULL
} ;

BASEIMAGEMANAGE_WIN BaseImageManageWin ;

// プログラム -----------------------------------------------------------------

// 環境依存初期化・終了関数

// 基本イメージ管理情報の環境依存処理の初期化
extern int InitializeBaseImageManage_PF( void )
{
	// IWICImagingFactory の生成
	WinAPIData.Win32Func.CoCreateInstanceFunc( CLSID_WICIMAGINGFACTORY, NULL, CLSCTX_INPROC_SERVER, IID_IWICIMAGINGFACTORY, ( void ** )&BaseImageManageWin.WICImagingFactory ) ;

	// 環境依存の読み込み関数は後に実行するようにする
	BASEIM.PlatformLoadFunctionAfterFlag = TRUE ;

	// 終了
	return 0 ;
}

// 基本イメージ管理情報の環境依存処理の後始末
extern int TerminateBaseImageManage_PF( void )
{
	// IWICImagingFactory の解放
	if( BaseImageManageWin.WICImagingFactory != NULL )
	{
		BaseImageManageWin.WICImagingFactory->Release() ;
		BaseImageManageWin.WICImagingFactory = NULL ;
	}

	// 終了
	return 0 ;
}

// WICによる読み込み
static int LoadWICProcess( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly )
{
	HRESULT hr ;
	D_IWICStream *WICStream ;
	D_IWICBitmapDecoder *WICBitmapDecoder = NULL ;
	D_IWICBitmapFrameDecode *WICBitmapFrameDecode = NULL ;
	D_IWICFormatConverter *WICFormatConverter = NULL ;
	D_WICPixelFormatGUID WICPixelFormatGUID ;
	BYTE *SrcP = NULL ;
	size_t FileBytes ;
	int UseConvert = FALSE ;

	// IWICImagingFactory がなければ何もせず終了
	if( BaseImageManageWin.WICImagingFactory == NULL )
	{
		return -1 ;
	}

	// メモリに読み込まれたデータではない場合は処理しない
	if( GetMemStreamDataShredStruct()->Read != Src->ReadShred.Read )
	{
		return -1 ;
	}

	// メモリの読み込まれたデータのアドレスを取得
	SrcP = *( ( BYTE ** )Src->DataPoint ) ;
	FileBytes = *( ( size_t * )( ( BYTE ** )Src->DataPoint + 1 ) ) ;

	// IWICStream の作成とセットアップ
	hr = BaseImageManageWin.WICImagingFactory->CreateStream( &WICStream ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}
	hr = WICStream->InitializeFromMemory( SrcP, ( DWORD )FileBytes ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

    // デコーダーの作成
	hr = BaseImageManageWin.WICImagingFactory->CreateDecoderFromStream( WICStream, NULL, D_WICDecodeMetadataCacheOnDemand, &WICBitmapDecoder ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

	// フレームの取得
	hr = WICBitmapDecoder->GetFrame( 0, &WICBitmapFrameDecode );
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

	// サイズの取得
	hr = WICBitmapFrameDecode->GetSize( ( UINT * )&BaseImage->Width, ( UINT * )&BaseImage->Height ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

	// ピクセルフォーマットの取得
	hr = WICBitmapFrameDecode->GetPixelFormat( &WICPixelFormatGUID ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

	// ピクセルフォーマットの判定
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT32bppBGRA, sizeof( GUID ) ) == 0 )
	{
		NS_CreateARGB8ColorData( &BaseImage->ColorData ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT32bppRGBA, sizeof( GUID ) ) == 0 )
	{
		NS_CreateABGR8ColorData( &BaseImage->ColorData ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT32bppBGR, sizeof( GUID ) ) == 0 )
	{
		NS_CreateXRGB8ColorData( &BaseImage->ColorData ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT32bppRGB, sizeof( GUID ) ) == 0 )
	{
		NS_CreateColorData( &BaseImage->ColorData, 32, 0x0000ff, 0x00ff00, 0xff0000, 0, 0, 0, FALSE ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT24bppBGR, sizeof( GUID ) ) == 0 )
	{
		NS_CreateFullColorData( &BaseImage->ColorData ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT24bppRGB, sizeof( GUID ) ) == 0 )
	{
		NS_CreateBGR8ColorData( &BaseImage->ColorData ) ;
	}
	else
	{
		UseConvert = TRUE ;

		// 非対応のフォーマットの場合はコンバート
		NS_CreateARGB8ColorData( &BaseImage->ColorData ) ;
	}

	// 画像を保存するメモリ領域の確保
	BaseImage->Pitch = BaseImage->ColorData.PixelByte * BaseImage->Width ;
	BaseImage->GraphData = DXALLOC( BaseImage->Pitch * BaseImage->Height ) ;
	if( BaseImage->GraphData == NULL )
	{
		goto ERR ;
	}
	BaseImage->MipMapCount    = 0 ;
	BaseImage->GraphDataCount = 0 ; 

	// コンバートするかどうかで処理を分岐
	if( UseConvert )
	{
		BOOL CanConvertResult = FALSE ;

		// ARGB8 フォーマットにコンバートする
		NS_CreateARGB8ColorData( &BaseImage->ColorData ) ;

		// コンバーターの作成
		hr = BaseImageManageWin.WICImagingFactory->CreateFormatConverter( &WICFormatConverter ) ;
		if( FAILED( hr ) )
		{
			goto ERR ;
		}

		// コンバート可能かチェック
		hr = WICFormatConverter->CanConvert( WICPixelFormatGUID, GUID_WICPIXELFORMAT32bppBGRA, &CanConvertResult ) ;
		if( FAILED( hr ) || CanConvertResult == FALSE )
		{
			goto ERR ;
		}

		// コンバーターの初期化
		hr = WICFormatConverter->Initialize( WICBitmapFrameDecode, GUID_WICPIXELFORMAT32bppBGRA, D_WICBitmapDitherTypeErrorDiffusion, NULL, 0, D_WICBitmapPaletteTypeMedianCut ) ;
		if( FAILED( hr ) )
		{
			goto ERR ;
		}

		// ピクセル情報を取得
		hr = WICFormatConverter->CopyPixels( NULL, BaseImage->Pitch, BaseImage->Pitch * BaseImage->Height, ( BYTE * )BaseImage->GraphData ) ;
		if( FAILED( hr ) )
		{
			goto ERR ;
		}
	}
	else
	{
		// ピクセル情報を取得
		hr = WICBitmapFrameDecode->CopyPixels( NULL, BaseImage->Pitch, BaseImage->Pitch * BaseImage->Height, ( BYTE * )BaseImage->GraphData ) ;
		if( FAILED( hr ) )
		{
			goto ERR ;
		}
	}

	if( WICFormatConverter != NULL )
	{
		WICFormatConverter->Release() ;
		WICFormatConverter = NULL ;
	}

	if( WICBitmapFrameDecode != NULL )
	{
		WICBitmapFrameDecode->Release() ;
		WICBitmapFrameDecode = NULL ;
	}

	if( WICBitmapDecoder != NULL )
	{
		WICBitmapDecoder->Release() ;
		WICBitmapDecoder = NULL ;
	}

	if( WICStream != NULL )
	{
		WICStream->Release() ;
		WICStream = NULL ;
	}

	// 正常終了
	return 0 ;

ERR :

	if( BaseImage->GraphData != NULL )
	{
		DXFREE( BaseImage->GraphData ) ;
		BaseImage->GraphData = NULL ;
	}

	if( WICFormatConverter != NULL )
	{
		WICFormatConverter->Release() ;
		WICFormatConverter = NULL ;
	}

	if( WICBitmapFrameDecode != NULL )
	{
		WICBitmapFrameDecode->Release() ;
		WICBitmapFrameDecode = NULL ;
	}

	if( WICBitmapDecoder != NULL )
	{
		WICBitmapDecoder->Release() ;
		WICBitmapDecoder = NULL ;
	}

	if( WICStream != NULL )
	{
		WICStream->Release() ;
		WICStream = NULL ;
	}

	// エラー終了
	return -1 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


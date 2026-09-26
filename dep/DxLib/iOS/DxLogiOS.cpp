//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用ログプログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

// インクルード ---------------------------------------------------------------
#ifndef DX_NON_LOG

#include "DxLogiOS.h"
#include "DxFileiOS.h"
#include "DxSystemiOS.h"
#include "../DxStatic.h"
#include "../DxLib.h"
#include "../DxLog.h"
#include "../DxBaseFunc.h"
#include "../DxMemory.h"
#include "../DxGraphics.h"
#include "DxGraphicsiOS.h"
#include "DxObjectiveCPP.h"
#include <wchar.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

// ログファイルを初期化する処理の環境依存部分
extern int LogFileInitialize_PF( const wchar_t *LogFilePath )
{
	int Result ;

	// 既に初期化済みの場合は何もしない
	if( LogData.PF.InitializeFlag == TRUE )
	{
		return 0 ;
	}

	// 初期化フラグを立てる
	LogData.PF.InitializeFlag = TRUE ;

	// エラーログファイルのパスをコピー
	LogData.PF.LogDataPath[ 0 ] = '\0' ;
	GetDocumentsDirPathForChar( LogData.PF.LogDataPath, sizeof( LogData.PF.LogDataPath ) ) ;

	// エラーログファイルを再作成する
	if( LogData.PF.LogDataPath[ 0 ] != '\0' )
	{
		char TempLogFilePath[ 1024 ] ;
		char LogFileNameUTF8[ 256 ] ;
		int Length ;

		ConvString( ( char * )LogData.LogFileName, -1, WCHAR_T_CHARCODEFORMAT, LogFileNameUTF8, sizeof( LogFileNameUTF8 ), DX_CHARCODEFORMAT_UTF8 ) ;

		Length = CL_strlen( DX_CHARCODEFORMAT_UTF8, LogData.PF.LogDataPath ) ;
		if( Length + CL_strlen( DX_CHARCODEFORMAT_UTF8, LogFileNameUTF8 ) + 1 < sizeof( TempLogFilePath ) - 16 )
		{
			FILE *fp ;

			CL_strcpy( DX_CHARCODEFORMAT_UTF8, TempLogFilePath, LogData.PF.LogDataPath ) ;
			if( TempLogFilePath[ Length - 1 ] != '/' )
			{
				TempLogFilePath[ Length ] = '/' ;
				TempLogFilePath[ Length + 1 ] = '\0' ;
			}
			CL_strcat( DX_CHARCODEFORMAT_UTF8, TempLogFilePath, LogFileNameUTF8 ) ;
			remove( TempLogFilePath ) ;
			fp = fopen( TempLogFilePath, "wb" ) ;
			if( fp != NULL )
			{
				BYTE HEAD[ 3 ] = { 0xef, 0xbb, 0xbf } ;
				fwrite( &HEAD, 3, 1, fp ) ;

				fclose( fp ) ;
			}
		}
	}

	// 終了
	return 0 ;
}

// ログファイルの後始末の環境依存部分
extern int LogFileTerminate_PF( void )
{
	return 0 ;
}

// ログファイルへ文字列を書き出す処理の環境依存部分
extern int LogFileAdd_WCHAR_T_PF( const wchar_t *LogFilePath, const wchar_t *ErrorStr )
{
	char ErrorStrDefaultBuffer[ 1024 ] ;
	char *ErrorStrTempBuffer = NULL ;
	char *ErrorStrUseBuffer = NULL ;
	int ErrorStrUseBufferBytes ;
	int Bytes ;
	int StringBytes ;

	// UTF8 の文字列に変換する
	StringBytes = ( ( int )_WCSLEN( ErrorStr ) + 1 ) * sizeof( wchar_t ) ;
	if( StringBytes > sizeof( ErrorStrDefaultBuffer ) )
	{
		ErrorStrTempBuffer = ( char * )DXALLOC( StringBytes ) ;
		if( ErrorStrTempBuffer == NULL )
		{
			return -1 ;
		}
		ErrorStrUseBuffer = ErrorStrTempBuffer ;
		ErrorStrUseBufferBytes = StringBytes ;
	}
	else
	{
		ErrorStrUseBuffer = ErrorStrDefaultBuffer ;
		ErrorStrUseBufferBytes = sizeof( ErrorStrDefaultBuffer ) ;
	}

	Bytes = ConvString( ( char * )ErrorStr, -1, WCHAR_T_CHARCODEFORMAT, ErrorStrUseBuffer, ErrorStrUseBufferBytes, DX_CHARCODEFORMAT_UTF8 ) ;

	// エラーログファイルを開く
	if( Bytes > 1 &&
		LogData.PF.LogDataPath[ 0 ] != '\0' )
	{
		char TempLogFilePath[ 1024 ] ;
		char LogFileNameUTF8[ 256 ] ;
		int Length ;

		ConvString( ( char * )LogData.LogFileName, -1, WCHAR_T_CHARCODEFORMAT, LogFileNameUTF8, sizeof( LogFileNameUTF8 ), DX_CHARCODEFORMAT_UTF8 ) ;

		Length = CL_strlen( DX_CHARCODEFORMAT_UTF8, LogData.PF.LogDataPath ) ;
		if( Length + CL_strlen( DX_CHARCODEFORMAT_UTF8, LogFileNameUTF8 ) + 1 < sizeof( TempLogFilePath ) - 16 )
		{
			FILE *fp ;

			CL_strcpy( DX_CHARCODEFORMAT_UTF8, TempLogFilePath, LogData.PF.LogDataPath ) ;
			if( TempLogFilePath[ Length - 1 ] != '/' )
			{
				TempLogFilePath[ Length ] = '/' ;
				TempLogFilePath[ Length + 1 ] = '\0' ;
			}
			CL_strcat( DX_CHARCODEFORMAT_UTF8, TempLogFilePath, LogFileNameUTF8 ) ;
			fp = fopen( TempLogFilePath, "ab" ) ;
			if( fp != NULL )
			{
				// エラーログファイルに書き出す
				fwrite( ErrorStrUseBuffer, ( DWORD )( Bytes - 1 ), 1, fp ) ;

				// ファイルを閉じる
				fclose( fp ) ;
			}
		}
	}

	// コンソールにも出力
	DxObjectiveCPP_Class::NSLog( "%s", ErrorStrUseBuffer ) ;

	if( ErrorStrTempBuffer != NULL )
	{
		DXFREE( ErrorStrTempBuffer ) ;
		ErrorStrTempBuffer = NULL ;
	}

	// 終了
	return 0 ;
}


// ログ機能の初期化を行うかどうかを取得する
extern int IsInitializeLog( void )
{
	return TRUE ;
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_LOG


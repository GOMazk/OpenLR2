// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		システムプログラム
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

// インクルード ------------------------------------------------------------------
#include "DxSystem.h"
#include "DxMemory.h"
#include "DxBaseFunc.h"
#include "DxLog.h"
#include "DxUseCLib.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

DXSYSTEMDATA DxSysData ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// プログラム --------------------------------------------------------------------

// 初期化・終了関係

// DxSysData 関係の初期化を行う
extern int DxLib_SysInit( void )
{
	// 終了リクエストのフラグを倒す
	DxSysData.EndRequestFlag = FALSE ;

	// 終了
	return 0 ;
}

// 終了リクエストを行う
extern int DxLib_EndRequest( void )
{
	DxSysData.EndRequestFlag = TRUE ;

	// 終了
	return 0 ;
}

// 終了リクエストの状態を取得する
extern int DxLib_GetEndRequest( void )
{
	return DxSysData.EndRequestFlag ;
}







// ライブラリが初期化されているかどうかを取得する( 戻り値: TRUE=初期化されている  FALSE=されていない )
extern int NS_DxLib_IsInit( void )
{
	return DxSysData.DxLib_InitializeFlag ;
}






// エラー処理関数

// 最後に発生したエラーのエラーコードを取得する( 戻り値　0:エラーが発生していない、又はエラーコード出力に対応したエラーが発生していない　　0以外：エラーコード、DX_ERRORCODE_WIN_DESKTOP_24BIT_COLOR など )
extern int NS_GetLastErrorCode( void )
{
	return DxSysData.LastErrorCode ;
}

// 最後に発生したエラーのエラーメッセージを指定の文字列バッファに取得する
extern int NS_GetLastErrorMessage( TCHAR *StringBuffer, int StringBufferBytes )
{
	ConvString( ( char * )DxSysData.LastErrorMessage, -1, WCHAR_T_CHARCODEFORMAT, ( char * )StringBuffer, StringBufferBytes, _TCHARCODEFORMAT ) ;

	return 0 ;
}

// エラーコード・メッセージを設定する
extern int DxLib_SetLastError( int ErrorCode/* DX_ERRORCODE_WIN_24BIT_COLOR など */, const wchar_t *ErrorMessage )
{
	DxSysData.LastErrorCode = ErrorCode ;

	if( ErrorMessage == NULL )
	{
		DxSysData.LastErrorMessage[ 0 ] = L'\0' ;
	}
	else
	{
		CL_strcpy_s( WCHAR_T_CHARCODEFORMAT, ( char * )DxSysData.LastErrorMessage, sizeof( DxSysData.LastErrorMessage ), ( char * )ErrorMessage ) ;
	}

	return 0 ;
}

// 書式付きライブラリのエラー処理を行う
extern int DxLib_FmtError( const wchar_t *FormatString , ... )
{
	va_list VaList ;
	wchar_t String[ 1024 ];

	// ログ出力用のリストをセットする
	va_start( VaList , FormatString ) ;

	// 編集後の文字列を取得する
	_VSWNPRINTF( String , sizeof( String ) / 2, FormatString , VaList ) ;

	// 可変長リストのポインタをリセットする
	va_end( VaList ) ;

	// エラー処理にまわす
	return DxLib_Error( String ) ;
}

// 書式付きライブラリのエラー処理を行う
extern int DxLib_FmtErrorUTF16LE( const char *FormatString , ... )
{
	va_list VaList ;
	char String[ 2048 ];

	// ログ出力用のリストをセットする
	va_start( VaList , FormatString ) ;

	// 編集後の文字列を取得する
	CL_vsnprintf( DX_CHARCODEFORMAT_UTF16LE, TRUE, CHAR_CHARCODEFORMAT, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ) / 2, FormatString, VaList ) ;

	// 可変長リストのポインタをリセットする
	va_end( VaList ) ;

	// エラー処理にまわす
	return DxLib_ErrorUTF16LE( String ) ;
}

















// クリップボード関係

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int NS_GetClipboardText( TCHAR *DestBuffer, int DestBufferBytes )
{
	return GetClipboardText_PF( DestBuffer, DestBufferBytes ) ;
}

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int GetClipboardText_WCHAR_T( wchar_t *DestBuffer, int DestBufferBytes )
{
	return GetClipboardText_WCHAR_T_PF( DestBuffer, DestBufferBytes ) ;
}

// クリップボードにテキストデータを格納する
extern int NS_SetClipboardText( const TCHAR *Text )
{
#ifdef UNICODE
	return SetClipboardText_WCHAR_T_PF( Text ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( Text, return -1 )

	Result = SetClipboardText_WCHAR_T_PF( UseTextBuffer ) ;

	TCHAR_TO_WCHAR_T_STRING_END( Text )

	return Result ;
#endif
}

// クリップボードにテキストデータを格納する
extern int SetClipboardText_WCHAR_T( const wchar_t *Text )
{
	return SetClipboardText_WCHAR_T_PF( Text ) ;
}

// クリップボードにテキストデータを格納する
extern int NS_SetClipboardTextWithStrLen( const TCHAR *Text, size_t TextLength )
{
	int Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( Text, TextLength, return -1 )
	Result = SetClipboardText_WCHAR_T( UseTextBuffer ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( Text )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( Text, TextLength, return -1 )
	Result = SetClipboardText_WCHAR_T( UseTextBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( Text )
#endif
	return Result ;
}

// クリップボードにテキストデータを格納する
extern int SetClipboardTextWithStrLen_WCHAR_T( const wchar_t *Text, size_t TextLength )
{
	int Result ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( Text, TextLength, return -1 )
	Result = SetClipboardText_WCHAR_T( UseTextBuffer ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( Text )
	return Result ;
}


























// iniファイル関係

// GetPrivateProfileString のＤＸライブラリ版
struct GetPrivateProfileStringDxInfo
{
	BYTE		*FileBuffer ;
	size_t		FileSize ;
	DWORD		FileAddr ;
	DWORD		Semicolon ;
	DWORD		BracketL ;
	DWORD		BracketR ;
	DWORD		Equal ;
	DWORD		Space ;
	DWORD		Tab ;
	DWORD		CR ;
	DWORD		LF ;
	int			IniFileCharCodeFormat ;
	DWORD		UnitBytes ;
	TCHAR		*ReturnedStringBuffer ;
	size_t		ReturnedStringBufferBytes ;
	DWORD		DestBytes ;
} ;

__inline DWORD GetPrivateProfileStringDx_NextChar( GetPrivateProfileStringDxInfo &Info, int *pCharBytes, int EnableSemicolon, int LFEnd )
{
	*pCharBytes = 0 ;
	while( Info.FileAddr < Info.FileSize )
	{
		DWORD CharCode ;
		int CharBytes ;

		CharCode = GetCharCode( ( char * )( Info.FileBuffer + Info.FileAddr ), Info.IniFileCharCodeFormat, &CharBytes ) ;
		if( CharCode == Info.LF && LFEnd )
		{
			return 0 ;
		}
		else
		if( CharCode == Info.Space || CharCode == Info.Tab || CharCode == Info.CR || CharCode == Info.LF )
		{
			Info.FileAddr += CharBytes ;
		}
		else
		if( CharCode == Info.Semicolon && EnableSemicolon )
		{
			Info.FileAddr += CharBytes ;
			while( Info.FileAddr < Info.FileSize )
			{
				CharCode = GetCharCode( ( char * )( Info.FileBuffer + Info.FileAddr ), Info.IniFileCharCodeFormat, &CharBytes ) ;
				Info.FileAddr += CharBytes ;
				if( CharCode == Info.LF )
				{
					if( LFEnd )
					{
						return 0 ;
					}
					break ;
				}
			}
		}
		else
		{
			*pCharBytes = CharBytes ;
			return CharCode ;
		}
	}

	return 0 ;
}

__inline void GetPrivateProfileStringDx_NextLine( GetPrivateProfileStringDxInfo &Info )
{
	while( Info.FileAddr < Info.FileSize )
	{
		DWORD CharCode ;
		int CharBytes ;

		CharCode = GetCharCode( ( char * )( Info.FileBuffer + Info.FileAddr ), Info.IniFileCharCodeFormat, &CharBytes ) ;
		Info.FileAddr += CharBytes ;
		if( CharCode == Info.LF )
		{
			return ;
		}
	}
}

__inline DWORD GetPrivateProfileStringDx_GetName( GetPrivateProfileStringDxInfo &Info, DWORD &NameBytes, int *pCharBytes, int EqualEnd )
{
	NameBytes = 0 ;
	while( Info.FileAddr + NameBytes < Info.FileSize )
	{
		DWORD CharCode ;
		int CharBytes ;

		CharCode = GetCharCode( ( char * )( Info.FileBuffer + Info.FileAddr + NameBytes ), Info.IniFileCharCodeFormat, &CharBytes ) ;
		if( ( EqualEnd && CharCode == Info.Equal ) ||
			CharCode == Info.Space    ||
			CharCode == Info.Tab      ||
			CharCode == Info.CR       ||
			CharCode == Info.LF       ||
			CharCode == Info.BracketR )
		{
			*pCharBytes = CharBytes ;
			return CharCode ;
		}
		else
		{
			NameBytes++ ;
		}
	}

	return 0 ;
}


__inline void GetPrivateProfileStringDx_PutString( GetPrivateProfileStringDxInfo &Info, const TCHAR *PutString, DWORD PutBytes )
{
	int i, j ;
	DWORD CharCode ;
	int CharBytes = 0 ;
	int PutCharBytes ;
	char CharString[ 18 ] ;
	int PutCharCodeFormat = _TCHARCODEFORMAT ;

	for( i = 0 ; ( DWORD )i < PutBytes ; i += CharBytes )
	{
		CharCode = GetCharCode( ( char * )( ( BYTE * )PutString + i ), Info.IniFileCharCodeFormat, &CharBytes ) ;
		PutCharBytes = PutCharCode( ConvCharCode( CharCode, Info.IniFileCharCodeFormat, PutCharCodeFormat ), PutCharCodeFormat, CharString, sizeof( CharString ) ) ;

		for( j = 0 ; j < PutCharBytes ; j += Info.UnitBytes )
		{
			if( Info.ReturnedStringBufferBytes < Info.DestBytes + Info.UnitBytes )
			{
				return ;
			}

			switch( Info.UnitBytes )
			{
			case 1 : *( ( BYTE  * )( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes ) ) = ( ( BYTE  * )CharString )[ j ] ; break ;
			case 2 : *( ( WORD  * )( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes ) ) = ( ( WORD  * )CharString )[ j ] ; break ;
			case 4 : *( ( DWORD * )( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes ) ) = ( ( DWORD * )CharString )[ j ] ; break ;
			}
			Info.DestBytes += Info.UnitBytes ;
		}
	}
}

extern int NS_GetPrivateProfileStringDx( const TCHAR *AppName, const TCHAR *KeyName, const TCHAR *Default, TCHAR *ReturnedStringBuffer, size_t ReturnedStringBufferBytes, const TCHAR *IniFilePath, int IniFileCharCodeFormat )
{
	int Return = -1 ;
	void *IniFileImage = NULL ;
	size_t IniFileImageBytes = 0 ;
	DWORD_PTR fp = 0 ;
	TCHAR_TO_WCHAR_T_STRING_BEGIN( IniFilePath )
	TCHAR_TO_WCHAR_T_STRING_SETUP( IniFilePath, goto END )

	// ファイルを丸ごと読み込み
	{
		fp = DX_FOPEN( UseIniFilePathBuffer ) ;
		if( fp == 0 )
		{
			goto END ;
		}
		DX_FSEEK( fp, 0, SEEK_END ) ;
		IniFileImageBytes = ( size_t )DX_FTELL( fp ) ;
		DX_FSEEK( fp, 0, SEEK_SET ) ;
		IniFileImage = DXALLOC( IniFileImageBytes ) ;
		if( IniFileImage == NULL )
		{
			goto END ;
		}
		DX_FREAD( IniFileImage, IniFileImageBytes, 1, fp ) ;
		DX_FCLOSE( fp ) ;
		fp = 0 ;
	}

	Return = NS_GetPrivateProfileStringDxForMem( AppName, KeyName, Default, ReturnedStringBuffer, ReturnedStringBufferBytes, IniFileImage, IniFileImageBytes, IniFileCharCodeFormat ) ;

END :

	TCHAR_TO_WCHAR_T_STRING_END( IniFilePath )

	if( fp != 0 )
	{
		DX_FCLOSE( fp ) ;
		fp = 0 ;
	}

	if( IniFileImage != NULL )
	{
		DXFREE( IniFileImage ) ;
		IniFileImage = NULL ;
	}

	return Return ;
}

// GetPrivateProfileString のＤＸライブラリ版
extern int NS_GetPrivateProfileStringDxWithStrLen( const TCHAR *AppName, size_t AppNameLength, const TCHAR *KeyName, size_t KeyNameLength, const TCHAR *Default, size_t DefaultLength, TCHAR *ReturnedString, size_t ReturnedStringBufferBytes, const TCHAR *IniFilePath, size_t IniFilePathLength, int IniFileCharCodeFormat )
{
	int Result = -1 ;

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( AppName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( KeyName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( Default     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( IniFilePath )

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( AppName,     AppNameLength,     goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( KeyName,     KeyNameLength,     goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( Default,     DefaultLength,     goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( IniFilePath, IniFilePathLength, goto ERR )

	Result = NS_GetPrivateProfileStringDx( AppNameBuffer, KeyNameBuffer, Default, ReturnedString, ReturnedStringBufferBytes, IniFilePathBuffer, IniFileCharCodeFormat ) ;

ERR :
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( AppName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( KeyName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( Default     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( IniFilePath )

	return Result ;
}

// GetPrivateProfileInt のＤＸライブラリ版
extern int NS_GetPrivateProfileIntDx( const TCHAR *AppName, const TCHAR *KeyName, int Default, const TCHAR *IniFilePath, int IniFileCharCodeFormat )
{
	TCHAR TempString[ 2048 ] ;
	int Size ;

	if( AppName == NULL || KeyName == NULL )
	{
		return Default ;
	}

	Size = NS_GetPrivateProfileStringDx( AppName, KeyName, NULL, TempString, sizeof( TempString ), IniFilePath, IniFileCharCodeFormat ) ;
	if( Size <= 0 )
	{
		return Default ;
	}

	return CL_atoi( _TCHARCODEFORMAT, ( char * )TempString ) ;
}

// GetPrivateProfileInt のＤＸライブラリ版
extern int NS_GetPrivateProfileIntDxWithStrLen( const TCHAR *AppName, size_t AppNameLength, const TCHAR *KeyName, size_t KeyNameLength, int Default, const TCHAR *IniFilePath, size_t IniFilePathLength, int IniFileCharCodeFormat )
{
	int Result = -1 ;

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( AppName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( KeyName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( IniFilePath )

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( AppName,     AppNameLength,     goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( KeyName,     KeyNameLength,     goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( IniFilePath, IniFilePathLength, goto ERR )

	Result = NS_GetPrivateProfileIntDx( AppNameBuffer, KeyNameBuffer, Default, IniFilePathBuffer, IniFileCharCodeFormat ) ;

ERR :
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( AppName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( KeyName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( IniFilePath )

	return Result ;
}

// GetPrivateProfileStringDx のメモリから読み込む版
extern int NS_GetPrivateProfileStringDxForMem( const TCHAR *AppName, const TCHAR *KeyName, const TCHAR *Default, TCHAR *ReturnedStringBuffer, size_t ReturnedStringBufferBytes, const void *IniFileImage, size_t IniFileImageBytes, int IniFileCharCodeFormat )
{
	int Return = -1 ;
	DWORD CharCode ;
	int CharBytes ;
	DWORD NameBytes ;
	GetPrivateProfileStringDxInfo Info = { NULL, 0, 0 } ;
	DWORD AppNameBytes = 0 ;
	DWORD KeyNameBytes = 0 ;
	TCHAR_TO_CHAR_STRING_BEGIN( AppName )
	TCHAR_TO_CHAR_STRING_BEGIN( KeyName )
	TCHAR_TO_CHAR_STRING_BEGIN( Default )

	Info.FileSize = IniFileImageBytes ;
	Info.FileBuffer = ( BYTE * )IniFileImage ;

	// ファイルの文字コードをチェック
	{
		if( IniFileCharCodeFormat < 0 )
		{
			IniFileCharCodeFormat = _TCHARCODEFORMAT ;
		}

		if( Info.FileSize >= 3 )
		{
			if( Info.FileBuffer[ 0 ] == 0xef && Info.FileBuffer[ 1 ] == 0xbb && Info.FileBuffer[ 2 ] == 0xbf )
			{
				IniFileCharCodeFormat = DX_CHARCODEFORMAT_UTF8 ;
				Info.FileAddr = 3 ;
			}
		}
		else
		if( Info.FileSize >= 2 )
		{
			if( Info.FileBuffer[ 0 ] == 0xff && Info.FileBuffer[ 1 ] == 0xfe )
			{
				IniFileCharCodeFormat = DX_CHARCODEFORMAT_UTF16LE ;
				Info.FileAddr = 2 ;
			}
			else
			if( Info.FileBuffer[ 0 ] == 0xfe && Info.FileBuffer[ 1 ] == 0xff )
			{
				IniFileCharCodeFormat = DX_CHARCODEFORMAT_UTF16BE ;
				Info.FileAddr = 2 ;
			}
		}
		Info.IniFileCharCodeFormat = IniFileCharCodeFormat ;
	}

	// 一文字辺りのバイト数を代入
	Info.UnitBytes = GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) ;

	// 出力先を初期化
	Info.ReturnedStringBuffer = ReturnedStringBuffer ;
	Info.ReturnedStringBufferBytes = ReturnedStringBufferBytes ;
	Info.DestBytes = 0 ;

	// ファイルフォーマットに合った文字列の準備
	TCHAR_TO_CHAR_STRING_SETUP( AppName, goto END, IniFileCharCodeFormat )
	TCHAR_TO_CHAR_STRING_SETUP( KeyName, goto END, IniFileCharCodeFormat )
	TCHAR_TO_CHAR_STRING_SETUP( Default, goto END, IniFileCharCodeFormat )
	Info.Semicolon = ConvCharCode( GetCharCode( ";",  DX_CHARCODEFORMAT_ASCII, NULL ), DX_CHARCODEFORMAT_ASCII, IniFileCharCodeFormat ) ;
	Info.BracketL  = ConvCharCode( GetCharCode( "[",  DX_CHARCODEFORMAT_ASCII, NULL ), DX_CHARCODEFORMAT_ASCII, IniFileCharCodeFormat ) ;
	Info.BracketR  = ConvCharCode( GetCharCode( "]",  DX_CHARCODEFORMAT_ASCII, NULL ), DX_CHARCODEFORMAT_ASCII, IniFileCharCodeFormat ) ;
	Info.Equal     = ConvCharCode( GetCharCode( "=",  DX_CHARCODEFORMAT_ASCII, NULL ), DX_CHARCODEFORMAT_ASCII, IniFileCharCodeFormat ) ;
	Info.Space     = ConvCharCode( GetCharCode( " ",  DX_CHARCODEFORMAT_ASCII, NULL ), DX_CHARCODEFORMAT_ASCII, IniFileCharCodeFormat ) ;
	Info.Tab       = ConvCharCode( GetCharCode( "\t", DX_CHARCODEFORMAT_ASCII, NULL ), DX_CHARCODEFORMAT_ASCII, IniFileCharCodeFormat ) ;
	Info.CR        = ConvCharCode( GetCharCode( "\r", DX_CHARCODEFORMAT_ASCII, NULL ), DX_CHARCODEFORMAT_ASCII, IniFileCharCodeFormat ) ;
	Info.LF        = ConvCharCode( GetCharCode( "\n", DX_CHARCODEFORMAT_ASCII, NULL ), DX_CHARCODEFORMAT_ASCII, IniFileCharCodeFormat ) ;

	// セクションとキーのバイト数を算出
	if( AppName != NULL )
	{
		AppNameBytes = ( DWORD )CL_strlen( IniFileCharCodeFormat, ( char * )UseAppNameBuffer ) * GetCharCodeFormatUnitSize( IniFileCharCodeFormat ) ;
	}
	if( KeyName != NULL )
	{
		KeyNameBytes = ( DWORD )CL_strlen( IniFileCharCodeFormat, ( char * )UseKeyNameBuffer ) * GetCharCodeFormatUnitSize( IniFileCharCodeFormat ) ;
	}

	// セクションのリストアップの場合
	if( AppName == NULL )
	{
		if( Info.ReturnedStringBufferBytes < Info.UnitBytes * 2 )
		{
			Return = 0 ;
			goto END ;
		}

		while( Info.FileAddr < Info.FileSize )
		{
			CharCode = GetPrivateProfileStringDx_NextChar( Info, &CharBytes, TRUE, FALSE ) ;
			if( Info.FileAddr >= Info.FileSize )
			{
				break ;
			}

			// 大括弧だったらセクション
			if( CharCode == Info.BracketL )
			{
				Info.FileAddr += ( DWORD )CharBytes ;

				CharCode = GetPrivateProfileStringDx_NextChar( Info, &CharBytes, FALSE, TRUE ) ;
				if( Info.FileAddr >= Info.FileSize )
				{
					break ;
				}

				if( CharBytes > 0 )
				{
					GetPrivateProfileStringDx_GetName( Info, NameBytes, &CharBytes, FALSE ) ;
					if( NameBytes > 0 )
					{
						GetPrivateProfileStringDx_PutString( Info, ( TCHAR * )( Info.FileBuffer + Info.FileAddr ), NameBytes ) ;
						Info.FileAddr += NameBytes ;
						if( Info.DestBytes + Info.UnitBytes * 2 >= Info.ReturnedStringBufferBytes )
						{
							_MEMSET( Info.ReturnedStringBuffer + Info.ReturnedStringBufferBytes - Info.UnitBytes * 2, 0, Info.UnitBytes * 2 ) ;
							Return = ( int )( Info.ReturnedStringBufferBytes - Info.UnitBytes * 2 ) ;
							goto END ;
						}
						_MEMSET( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes, 0, Info.UnitBytes ) ;
						Info.DestBytes += Info.UnitBytes ;
						if( Info.DestBytes + Info.UnitBytes * 3 > Info.ReturnedStringBufferBytes )
						{
							_MEMSET( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes, 0, Info.UnitBytes ) ;
							Return = ( int )Info.ReturnedStringBufferBytes ;
							goto END ;
						}
					}
				}
			}

			// 次の行へ移動
			GetPrivateProfileStringDx_NextLine( Info ) ;
		}
		_MEMSET( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes, 0, Info.UnitBytes ) ;
		Return = ( int )( Info.DestBytes / Info.UnitBytes ) ;
	}
	else
	{
		// セクションの検索
		while( Info.FileAddr < Info.FileSize )
		{
			CharCode = GetPrivateProfileStringDx_NextChar( Info, &CharBytes, TRUE, FALSE ) ;
			if( Info.FileAddr >= Info.FileSize )
			{
				goto DEFAULT_END ;
			}

			if( CharCode == Info.BracketL )
			{
				Info.FileAddr += ( DWORD )CharBytes ;

				CharCode = GetPrivateProfileStringDx_NextChar( Info, &CharBytes, FALSE, TRUE ) ;
				if( Info.FileAddr >= Info.FileSize )
				{
					goto DEFAULT_END ;
				}

				if( CharBytes > 0 )
				{
					CharCode = GetPrivateProfileStringDx_GetName( Info, NameBytes, &CharBytes, FALSE ) ;
					if( AppNameBytes == NameBytes && _MEMCMP( Info.FileBuffer + Info.FileAddr, UseAppNameBuffer, NameBytes ) == 0 )
					{
						break ;
					}
					Info.FileAddr += NameBytes ;
				}
			}

			// 次の行へ移動
			GetPrivateProfileStringDx_NextLine( Info ) ;
		}
		if( Info.FileAddr >= Info.FileSize )
		{
			goto DEFAULT_END ;
		}
		GetPrivateProfileStringDx_NextLine( Info ) ;

		// キーのリストアップの場合
		if( KeyName == NULL )
		{
			if( Info.ReturnedStringBufferBytes < Info.UnitBytes * 2 )
			{
				Return = 0 ;
				goto END ;
			}

			while( Info.FileAddr < Info.FileSize )
			{
				CharCode = GetPrivateProfileStringDx_NextChar( Info, &CharBytes, TRUE, FALSE ) ;
				if( Info.FileAddr >= Info.FileSize || CharCode == Info.BracketL )
				{
					break ;
				}

				GetPrivateProfileStringDx_GetName( Info, NameBytes, &CharBytes, TRUE ) ;
				if( NameBytes > 0 )
				{
					BYTE *Addr = Info.FileBuffer + Info.FileAddr ;
					Info.FileAddr += NameBytes ;

					// 次の文字が = かどうかを確認
					CharCode = GetPrivateProfileStringDx_NextChar( Info, &CharBytes, TRUE, TRUE ) ;
					if( Info.FileAddr < Info.FileSize && CharCode == Info.Equal )
					{
						Info.FileAddr += CharBytes ;

						GetPrivateProfileStringDx_PutString( Info, ( TCHAR * )Addr, NameBytes ) ;
						if( Info.DestBytes + Info.UnitBytes * 2 > Info.ReturnedStringBufferBytes )
						{
							_MEMSET( Info.ReturnedStringBuffer + Info.ReturnedStringBufferBytes - Info.UnitBytes * 2, 0, Info.UnitBytes * 2 ) ;
							Return = ( int )( Info.ReturnedStringBufferBytes / Info.UnitBytes - 2 ) ;
							goto END ;
						}
						_MEMSET( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes, 0, Info.UnitBytes ) ;
						Info.DestBytes += Info.UnitBytes ;
						if( Info.DestBytes + Info.UnitBytes * 3 > Info.ReturnedStringBufferBytes )
						{
							_MEMSET( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes, 0, Info.UnitBytes ) ;
							Return = ( int )( Info.ReturnedStringBufferBytes / Info.UnitBytes ) - 1 ;
							goto END ;
						}
					}
				}

				// 次の行へ移動
				GetPrivateProfileStringDx_NextLine( Info ) ;
			}
			_MEMSET( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes, 0, Info.UnitBytes ) ;
			Return = ( int )( Info.DestBytes / Info.UnitBytes ) ;
		}
		else
		// キーの取得の場合
		{
			if( Info.ReturnedStringBufferBytes <= Info.UnitBytes )
			{
				if( Info.ReturnedStringBufferBytes == Info.UnitBytes )
				{
					_MEMSET( Info.ReturnedStringBuffer, 0, Info.ReturnedStringBufferBytes ) ;
				}

				Return = 0 ;
				goto END ;
			}

			// キーの検索
			while( Info.FileAddr < Info.FileSize )
			{
				CharCode = GetPrivateProfileStringDx_NextChar( Info, &CharBytes, TRUE, FALSE ) ;
				if( Info.FileAddr >= Info.FileSize || CharCode == Info.BracketL )
				{
					goto DEFAULT_END ;
				}

				CharCode = GetPrivateProfileStringDx_GetName( Info, NameBytes, &CharBytes, TRUE ) ;
				if( KeyNameBytes == NameBytes && _MEMCMP( Info.FileBuffer + Info.FileAddr, UseKeyNameBuffer, NameBytes ) == 0 )
				{
					Info.FileAddr += NameBytes ;
					break ;
				}
				Info.FileAddr += NameBytes ;

				// 次の行へ移動
				GetPrivateProfileStringDx_NextLine( Info ) ;
			}
			if( Info.FileAddr >= Info.FileSize )
			{
				goto DEFAULT_END ;
			}

			// 次の文字が = かどうかを確認
			CharCode = GetPrivateProfileStringDx_NextChar( Info, &CharBytes, TRUE, TRUE ) ;
			if( Info.FileAddr >= Info.FileSize || CharCode != Info.Equal )
			{
				goto DEFAULT_END ;
			}
			Info.FileAddr += CharBytes ;

			// 文字列の取得
			CharCode = GetPrivateProfileStringDx_NextChar( Info, &CharBytes, FALSE, TRUE ) ;
			if( CharCode == 0 )
			{
				Return = 0 ;
				_MEMSET( Info.ReturnedStringBuffer, 0, Info.UnitBytes ) ;
			}
			else
			{
				NameBytes = 0 ;
				while( Info.FileAddr + NameBytes < Info.FileSize )
				{
					CharCode = GetCharCode( ( char * )( Info.FileBuffer + Info.FileAddr + NameBytes ), Info.IniFileCharCodeFormat, &CharBytes ) ;
					if( CharCode == Info.CR || CharCode == Info.LF )
					{
						break ;
					}
					NameBytes += CharBytes ;
				}
				if( NameBytes == 0 )
				{
					Return = 0 ;
					_MEMSET( Info.ReturnedStringBuffer, 0, Info.UnitBytes ) ;
				}
				else
				{
					GetPrivateProfileStringDx_PutString( Info, ( TCHAR * )( Info.FileBuffer + Info.FileAddr ), NameBytes ) ;
					if( Info.DestBytes > Info.ReturnedStringBufferBytes - Info.UnitBytes )
					{
						Info.DestBytes = ( DWORD )( Info.ReturnedStringBufferBytes - Info.UnitBytes ) ;
					}
					_MEMSET( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes, 0, Info.UnitBytes ) ;
					Return = ( int )( Info.DestBytes / Info.UnitBytes ) ;
				}
			}
		}
	}

END :

	TCHAR_TO_CHAR_STRING_END( AppName )
	TCHAR_TO_CHAR_STRING_END( KeyName )
	TCHAR_TO_CHAR_STRING_END( Default )

	return Return ;

DEFAULT_END :
	if( Default != NULL )
	{
		GetPrivateProfileStringDx_PutString( Info, ( TCHAR * )UseDefaultBuffer, ( DWORD )CL_strlen( IniFileCharCodeFormat, ( char * )UseDefaultBuffer ) * GetCharCodeFormatUnitSize( IniFileCharCodeFormat ) ) ;
		if( Info.DestBytes > Info.ReturnedStringBufferBytes - Info.UnitBytes )
		{
			Info.DestBytes = ( DWORD )( Info.ReturnedStringBufferBytes - Info.UnitBytes ) ;
		}
	}
	_MEMSET( ( BYTE * )Info.ReturnedStringBuffer + Info.DestBytes, 0, Info.UnitBytes ) ;
	Return = ( int )( Info.DestBytes / Info.UnitBytes ) ;
	goto END ;
}

// GetPrivateProfileStringDx のメモリから読み込む版
extern int NS_GetPrivateProfileStringDxForMemWithStrLen( const TCHAR *AppName, size_t AppNameLength, const TCHAR *KeyName, size_t KeyNameLength, const TCHAR *Default, size_t DefaultLength, TCHAR *ReturnedString, size_t ReturnedStringBufferBytes, const void *IniFileImage, size_t IniFileImageBytes, int IniFileCharCodeFormat )
{
	int Result = -1 ;

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( AppName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( KeyName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( Default     )

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( AppName,     AppNameLength,     goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( KeyName,     KeyNameLength,     goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( Default,     DefaultLength,     goto ERR )

	Result = NS_GetPrivateProfileStringDxForMem( AppNameBuffer, KeyNameBuffer, Default, ReturnedString, ReturnedStringBufferBytes, IniFileImage, IniFileImageBytes, IniFileCharCodeFormat ) ;

ERR :
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( AppName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( KeyName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( Default     )

	return Result ;
}

// GetPrivateProfileIntDx のメモリから読み込む版
extern int NS_GetPrivateProfileIntDxForMem( const TCHAR *AppName, const TCHAR *KeyName, int Default, const void *IniFileImage, size_t IniFileImageBytes, int IniFileCharCodeFormat )
{
	TCHAR TempString[ 2048 ] ;
	int Size ;

	if( AppName == NULL || KeyName == NULL )
	{
		return Default ;
	}

	Size = NS_GetPrivateProfileStringDxForMem( AppName, KeyName, NULL, TempString, sizeof( TempString ), IniFileImage, IniFileImageBytes, IniFileCharCodeFormat ) ;
	if( Size <= 0 )
	{
		return Default ;
	}

	return CL_atoi( _TCHARCODEFORMAT, ( char * )TempString ) ;
}

// GetPrivateProfileIntDx のメモリから読み込む版
extern int NS_GetPrivateProfileIntDxForMemWithStrLen( const TCHAR *AppName, size_t AppNameLength, const TCHAR *KeyName, size_t KeyNameLength, int Default, const void *IniFileImage, size_t IniFileImageBytes, int IniFileCharCodeFormat )
{
	int Result = -1 ;

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( AppName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( KeyName     )

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( AppName,     AppNameLength,     goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( KeyName,     KeyNameLength,     goto ERR )

	Result = NS_GetPrivateProfileIntDxForMem( AppNameBuffer, KeyNameBuffer, Default, IniFileImage, IniFileImageBytes, IniFileCharCodeFormat ) ;

ERR :
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( AppName     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( KeyName     )

	return Result ;
}

















#if defined( __APPLE__ ) || defined( __ANDROID__ )



// メールアプリを送信メール編集状態で起動する
// MailAddr    : 宛先( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// MainCCAddr  : CC の宛先( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// MainBCCAddr : BCC の宛先( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// Subject     : タイトル( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// Text        : 本文( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
extern int MailApp_Send_WCHAR_T( const wchar_t *MailAddr, const wchar_t *MailCCAddr, const wchar_t *MailBCCAddr, const wchar_t *Subject, const wchar_t *Text )
{
	// 環境依存関数を呼ぶ
	return MailApp_Send_WCHAR_T_PF( MailAddr, MailCCAddr, MailBCCAddr, Subject, Text ) ;
}

extern int MailApp_Send( const TCHAR *MailAddr, const TCHAR *MailCCAddr, const TCHAR *MailBCCAddr, const TCHAR *Subject, const TCHAR *Text )
{
#ifdef UNICODE
	return MailApp_Send_WCHAR_T( MailAddr, MailCCAddr, MailBCCAddr, Subject, Text ) ;
#else
	int Result = -1 ;
	CHAR_TO_WCHAR_T_STRING_BEGIN( MailAddr    )
	CHAR_TO_WCHAR_T_STRING_BEGIN( MailCCAddr  )
	CHAR_TO_WCHAR_T_STRING_BEGIN( MailBCCAddr )
	CHAR_TO_WCHAR_T_STRING_BEGIN( Subject     )
	CHAR_TO_WCHAR_T_STRING_BEGIN( Text        )
	CHAR_TO_WCHAR_T_STRING_SETUP( MailAddr,    goto END, _TCHARCODEFORMAT )
	CHAR_TO_WCHAR_T_STRING_SETUP( MailCCAddr,  goto END, _TCHARCODEFORMAT )
	CHAR_TO_WCHAR_T_STRING_SETUP( MailBCCAddr, goto END, _TCHARCODEFORMAT )
	CHAR_TO_WCHAR_T_STRING_SETUP( Subject,     goto END, _TCHARCODEFORMAT )
	CHAR_TO_WCHAR_T_STRING_SETUP( Text,        goto END, _TCHARCODEFORMAT )

	Result = MailApp_Send_WCHAR_T( UseMailAddrBuffer, UseMailCCAddrBuffer, UseMailBCCAddrBuffer, UseSubjectBuffer, UseTextBuffer ) ;

END :
	TCHAR_TO_WCHAR_T_STRING_END( MailAddr    )
	TCHAR_TO_WCHAR_T_STRING_END( MailCCAddr  )
	TCHAR_TO_WCHAR_T_STRING_END( MailBCCAddr )
	TCHAR_TO_WCHAR_T_STRING_END( Subject     )
	TCHAR_TO_WCHAR_T_STRING_END( Text        )

	return Result ;
#endif
}

extern int MailApp_SendWithStrLen( const TCHAR *MailAddr, size_t MailAddrLength, const TCHAR *MailCCAddr, size_t MailCCAddrLength, const TCHAR *MailBCCAddr, size_t MailBCCAddrLength, const TCHAR *Subject, size_t SubjectLength, const TCHAR *Text, size_t TextLength )
{
	int Result = -1 ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( MailAddr    )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( MailCCAddr  )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( MailBCCAddr )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( Subject     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( Text        )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( MailAddr,    MailAddrLength,    goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( MailCCAddr,  MailCCAddrLength,  goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( MailBCCAddr, MailBCCAddrLength, goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( Subject,     SubjectLength,     goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( Text,        TextLength,        goto ERR )

	Result = MailApp_Send( UseMailAddrBuffer, UseMailCCAddrBuffer, UseMailBCCAddrBuffer, UseSubjectBuffer, UseTextBuffer ) ;

ERR :
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( MailAddr    )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( MailCCAddr  )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( MailBCCAddr )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( Subject     )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( Text        )

	return Result ;
}

#endif // defined( __APPLE__ ) || defined( __ANDROID__ )

















// 読み込み処理系の関数で非同期読み込みを行うかどうかを設定する( 非同期読み込みに対応している関数のみ有効 )( TRUE:非同期読み込みを行う  FALSE:非同期読み込みを行わない( デフォルト ) )
extern int NS_SetUseASyncLoadFlag( int Flag )
{
	DxSysData.ASyncLoadFlag = Flag ;

	// 終了
	return 0 ;
}

// 読み込み処理系の関数で非同期読み込みを行うかどうかを取得する( 非同期読み込みに対応している関数のみ有効 )( TRUE:非同期読み込みを行う  FALSE:非同期読み込みを行わない( デフォルト ) )
extern int NS_GetUseASyncLoadFlag( void )
{
	return GetASyncLoadFlag() ;
}

// 非同期読み込みを行うかどうかを取得する( TRUE:非同期読み込みを行う   FALSE:非同期読み込みを行わない )
extern int GetASyncLoadFlag( void )
{
	return DxSysData.ASyncLoadFlag ? TRUE : FALSE ;
}

// ＤＸライブラリのウインドウ関連の機能を使用しないフラグ
extern int NS_SetNotWinFlag( int Flag )
{
	if( Flag == TRUE ) DxSysData.NotDrawFlag = TRUE;
	DxSysData.NotWinFlag = Flag ;

	return 0 ;
}

// 描画機能を使うかどうかのフラグをセットする
extern int NS_SetNotDrawFlag( int Flag )
{
	DxSysData.NotDrawFlag = Flag ;

	return 0 ;
}

// 描画機能を使うかどうかのフラグを取得する
extern int NS_GetNotDrawFlag( void )
{
	return DxSysData.NotDrawFlag ;
}

// サウンド機能を使うかどうかのフラグをセットする
extern int NS_SetNotSoundFlag( int Flag )
{
	DxSysData.NotSoundFlag = Flag ;

	return 0;
}

// 入力状態取得機能を使うかどうかのフラグをセットする
extern int NS_SetNotInputFlag( int Flag )
{
	DxSysData.NotInputFlag = Flag ;

	return 0;
}














// ウエイト系関数

// 指定の時間だけ処理をとめる
extern int NS_WaitTimer( int WaitTime )
{
	ULONGLONG StartTime, EndTime ;

	StartTime = NS_GetNowSysPerformanceCount() ;

	// 4msec前まで寝る
	if( WaitTime > 4 )
	{
		// 指定時間の間メッセージループ
		EndTime = StartTime + NS_ConvMilliSecondsToSysPerformanceCount( WaitTime - 4 ) ;
		while( EndTime > NS_GetNowSysPerformanceCount() )
		{
			// ウィンドウが閉じられた場合は即座に終了
			if( NS_ProcessMessage() != 0 )
			{
				return 0 ;
			}
			Thread_Sleep( 1 ) ;
		}

	}

	// 4msec以下の分は正確に待つ
	EndTime = StartTime + NS_ConvMilliSecondsToSysPerformanceCount( WaitTime ) ;
	while( EndTime > NS_GetNowSysPerformanceCount() ){}

	// 終了
	return 0 ;
}

// 指定の時間だけスレッドを眠らせる
extern int NS_SleepThread( int WaitTime )
{
	if( WaitTime < 0 ) WaitTime = 0 ;

	Thread_Sleep( ( DWORD )WaitTime ) ;

	// 終了
	return 0 ;
}

#ifndef DX_NON_INPUT

// キーの入力待ち
extern int NS_WaitKey( void )
{
	int BackCode = 0 ;

	while( ProcessMessage() == 0 && CheckHitKeyAll( DX_CHECKINPUT_ALL ) != 0 )
	{
		Thread_Sleep( 1 ) ;
	}

	while( ProcessMessage() == 0 && ( BackCode = CheckHitKeyAll( DX_CHECKINPUT_ALL ) ) == 0 )
	{
		Thread_Sleep( 1 ) ;
	}

//	while( ProcessMessage() == 0 && CheckHitKeyAll() != 0 )
//	{
//		Thread_Sleep( 1 ) ;
//	}

	return BackCode ;
}

#endif // DX_NON_INPUT






// 乱数取得

#ifndef DX_NON_MERSENNE_TWISTER

// 乱数の初期値を設定する
extern int NS_SRand( int Seed )
{
	// 初期値セット
	srandMT( ( unsigned int )Seed ) ;

	// 終了
	return 0 ;
}

// 乱数を取得する( RandMax : 返って来る値の最大値 )
extern int NS_GetRand( int RandMax )
{
	int Result ;
	LONGLONG RandMaxLL ;

	RandMaxLL = RandMax ;
	RandMaxLL ++ ;
	Result = ( int )( ( ( LONGLONG )randMT() * RandMaxLL ) >> 32 ) ;

	return Result ;
}

// メルセンヌ・ツイスターアルゴリズムで生成された乱数値を無加工で取得する
extern DWORD NS_GetMersenneTwisterRand( void )
{
	return randMT() ;
}

// 乱数ハンドルを作成する( 戻り値　0以外:乱数ハンドル　0:エラー )
extern DWORD_PTR NS_CreateRandHandle( int Seed )
{
	MERSENNE_TWISTER_DATA *MTData ;

	// 新しい乱数ハンドル用のメモリを確保する
	MTData = ( MERSENNE_TWISTER_DATA * )DXALLOC( sizeof( MERSENNE_TWISTER_DATA ) ) ;
	if( MTData == NULL )
	{
		DXST_LOGFILEFMT_ADDUTF16LE(( "\x71\x4e\x70\x65\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"乱数ハンドル用のメモリの確保に失敗しました" @*/ )) ;
		return 0 ;
	}

	// 乱数情報を初期化
	NS_SRandHandle( ( DWORD_PTR )MTData, ( unsigned int )( Seed >= 0 ? Seed : NS_GetRand( 0x7fffffff ) ) ) ;

	// ハンドルを返す
	return ( DWORD_PTR )MTData ;
}

// 乱数ハンドルを削除する
extern int NS_DeleteRandHandle( DWORD_PTR RandHandle )
{
	// メモリを解放する
	if( RandHandle != 0 )
	{
		DXFREE( ( void * )RandHandle ) ;
	}

	// 終了
	return 0 ;
}

// 乱数ハンドルの初期値を再設定する
extern int NS_SRandHandle( DWORD_PTR RandHandle, int Seed )
{
	// エラーチェック
	if( RandHandle == 0 )
	{
		return -1 ;
	}

	// 乱数情報を初期化
	initMTData( ( MERSENNE_TWISTER_DATA * )RandHandle, ( unsigned int )Seed ) ;

	// 終了
	return 0 ;
}

// 乱数ハンドルを使用して乱数を取得する( RandMax : 返って来る値の最大値 )
extern int NS_GetRandHandle( DWORD_PTR RandHandle, int RandMax )
{
	int Result ;
	LONGLONG RandMaxLL ;

	// エラーチェック
	if( RandHandle == 0 )
	{
		return -1 ;
	}

	RandMaxLL = RandMax ;
	RandMaxLL ++ ;
	Result = ( int )( ( ( LONGLONG )randMTData( ( MERSENNE_TWISTER_DATA * )RandHandle ) * RandMaxLL ) >> 32 ) ;

	return Result ;
}

// 乱数ハンドルを使用してメルセンヌ・ツイスターアルゴリズムで生成された乱数値を無加工で取得する
extern DWORD NS_GetMersenneTwisterRandHandle( DWORD_PTR RandHandle )
{
	return randMTData( ( MERSENNE_TWISTER_DATA * )RandHandle ) ;
}

#else // DX_NON_MERSENNE_TWISTER

// 乱数の初期値を設定する
extern int NS_SRand( int Seed )
{
	// 初期値セット
	srand( Seed ) ;

	// 終了
	return 0 ;
}

// 乱数を取得する( RandMax : 返って来る値の最大値 )
extern int NS_GetRand( int RandMax )
{
	int Result ;
	LONGLONG RandMaxLL ;

	RandMaxLL = RandMax ;
	RandMaxLL ++ ;
	Result = ( int )( ( ( LONGLONG )rand() * RandMaxLL ) / ( ( LONGLONG )RAND_MAX + 1 ) ) ;

	return Result ;
}

#endif // DX_NON_MERSENNE_TWISTER









#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE






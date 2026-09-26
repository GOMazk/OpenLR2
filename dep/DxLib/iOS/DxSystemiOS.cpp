// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用システムプログラム
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

// インクルード ------------------------------------------------------------------
#include "DxSystemiOS.h"
#include "DxSystemiOS_ObjC.h"
#include "DxGraphicsiOS.h"
#include "DxGraphicsFilteriOS.h"
#include "DxLogiOS.h"
#include "DxMaskiOS.h"
#include "DxModeliOS.h"
#include "../DxLib.h"
#include "../DxArchive_.h"
#include "../DxSystem.h"
#include "../DxGraphics.h"
#include "../DxModel.h"
#include "../DxMask.h"
#include "../DxFile.h"
#include "../DxNetwork.h"
#include "../DxInputString.h"
#include "../DxInput.h"
#include "../DxSound.h"
#include "../DxSoundConvert.h"
#include "../DxBaseImage.h"
#include "../DxSoftImage.h"
#include "../DxMovie.h"
#include "../DxFont.h"
#include "../DxLog.h"
#include "../DxASyncLoad.h"
#include "../DxLive2DCubism4.h"
#include "../DxUseCLib.h"
#include <time.h>
#include <mach/mach_time.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "../DxBaseFunc.h"
#include "../DxMemory.h"


extern int ios_main( void ) ;


#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define TIME_DISTANCE( now, time )			( (now) < (time) ? 0x7fffffff - (time) + (now) : (now) - (time) )

// 構造体定義 --------------------------------------------------------------------

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

DXLIB_IOS_SYSTEMINFO g_iOSSys ;
int g_iOSRunFlag ;

// 関数プロトタイプ宣言-----------------------------------------------------------
   
// プログラム --------------------------------------------------------------------

// UTF16LE の書式文字列と UTF8 のパラメータ文字列をログ出力する
static void OutputiOSOSInfo_LogAddUTF8( const char *UTF16LEFormatStr, const char *UTF8Str )
{
//	char TempStr[ 1024 ] ;
//
//	ConvString( UTF8Str, -1, DX_CHARCODEFORMAT_UTF8, TempStr, sizeof( TempStr ), DX_CHARCODEFORMAT_UTF16LE ) ;
//	DXST_LOGFILEFMT_ADDUTF16LE(( UTF16LEFormatStr, TempStr )) ;
}

// ライブラリ初期化関数
extern int NS_DxLib_Init( void )
{
	// 既に初期化済みの場合は何もせず終了
	if( DxSysData.DxLib_InitializeFlag == TRUE )
	{
		return 0 ;
	}

	DXST_LOGFILE_ADDA( "Start initialization processing of DX library\n" /*"ＤＸライブラリの初期化処理開始"*/ ) ;
	DXST_LOGFILE_TABADD ;

	// 初期化中フラグを立てる
	DxSysData.DxLib_RunInitializeFlag = TRUE ;

#ifndef DX_NON_LITERAL_STRING
	// ＤＸライブラリのバージョンを出力する
	DXST_LOGFILEFMT_ADDA(( "DX library Ver%s\n", DXLIB_VERSION_STR_T /*"ＤＸライブラリ Ver%s\n"*/ )) ;
#endif

	// OS情報出力
	{
	}

	// DxSysData の共通初期化処理
	DxLib_SysInit() ;

	// DxBaseFunc の初期化
	_INIT_BASEFUNC() ;

	// キャラクターコード関係の初期化を行う
	InitCharCode() ;

	// 使用する文字セットをセット
	_SET_DEFAULT_CHARCODEFORMAT() ;

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理の初期化
	InitializeASyncLoad( Thread_GetCurrentId() ) ;
#endif // DX_NON_ASYNCLOAD

	// ファイルアクセス処理の初期化
	InitializeFile() ;

#ifndef DX_NON_OGGTHEORA
	// Theora 用の初期化
	TheoraDecode_GrobalInitialize() ;
#endif

	// アーカイブファイルアクセス用のデータを初期化
#ifndef DX_NON_DXA
	DXA_DIR_Initialize() ;
#endif

	// ストリームデータ読み込み制御用ポインタ構造体のデフォルト値をセット
	NS_ChangeStreamFunction( NULL ) ;

#ifndef DX_NON_LOG
	// ログファイルの初期化
	LogFileInitialize() ;
#endif

	// システムログを出力
//	OutSystemInfo() ;

#ifndef DX_NON_GRAPHICS
	// デフォルトのグラフィック復元関数を登録
	NS_SetRestoreGraphCallback( NULL ) ;
#endif // DX_NON_GRAPHICS

	// 各処理系の初期化
	if( DxSysData.NotInputFlag == FALSE )
	{
#ifndef DX_NON_INPUT
		if( InitializeInputSystem() == -1 ) goto ERROR_DX ;			// 入力システムの初期化
#endif // DX_NON_INPUT
	}

	if( DxSysData.NotSoundFlag == FALSE )
	{
#ifndef DX_NON_SOUND
		InitializeSoundConvert() ;									// サウンド変換処理の初期化
		InitializeSoundSystem() ;									// サウンドシステムのの初期化
#endif // DX_NON_SOUND
	}
	if( DxSysData.NotDrawFlag == FALSE )
	{
		InitializeBaseImageManage() ;
#ifndef DX_NON_SOFTIMAGE
		InitializeSoftImageManage() ;
#endif // DX_NON_SOFTIMAGE
#ifndef DX_NON_MOVIE
		InitializeMovieManage() ;
#endif

#ifndef DX_NON_GRAPHICS
		if( Graphics_Initialize() < 0 ) goto ERROR_DX ;
#endif // DX_NON_GRAPHICS
	}
#ifndef DX_NON_INPUTSTRING
	InitializeInputCharBuf() ;									// 文字コードバッファの初期化
#endif // DX_NON_INPUTSTRING

	// ＤＸライブラリ初期化完了フラグをたてる
	DxSysData.DxLib_InitializeFlag = TRUE ;

	// ＶＳＹＮＣ待ちをする
//	NS_SetWaitVSyncFlag( TRUE ) ;

#if !defined( DX_NON_LOG ) && !defined( DX_NON_PRINTF_DX )
	// ログ出力処理の初期化を行う
	InitializeLog() ;
#endif

#ifndef DX_NON_GRAPHICS
	// 描画先の変更
	NS_SetDrawScreen( DX_SCREEN_BACK ) ;
	NS_SetDrawScreen( DX_SCREEN_FRONT ) ;
#endif // DX_NON_GRAPHICS

	if( DxSysData.NotDrawFlag == FALSE )
	{
#ifndef DX_NON_MODEL
		// モデルバージョン１の初期化
		if( MV1Initialize() < 0 )
		{
			goto ERROR_DX ;
		}
#endif

#ifndef DX_NON_LIVE2D_CUBISM4
		// Live2D Cubism4 関連の初期化
		Live2DCubism4_Initialize() ;
#endif // DX_NON_LIVE2D_CUBISM4
	}

	// ランダム係数を初期化
#ifndef DX_NON_MERSENNE_TWISTER
	srandMT( ( unsigned int )NS_GetNowCount( FALSE ) ) ;
#else
	srand( NS_GetNowCount( FALSE ) ) ;
#endif

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理を行うスレッドを立てる
	if( SetupASyncLoadThread( 3 ) < 0 )
	{
		DXST_LOGFILE_ADDA( "Startup of asynchronous read processing thread failed\n"/*"非同期読み込み処理を行うスレッドの立ち上げに失敗しました\n"*/ ) ;
		goto ERROR_DX ;
	}
#endif // DX_NON_ASYNCLOAD

	// 初期化中フラグを倒す
	DxSysData.DxLib_RunInitializeFlag = FALSE ;

	DXST_LOGFILE_TABSUB ;
	DXST_LOGFILEFMT_ADDA(( "DX library initialization processing end" /*"ＤＸライブラリの初期化処理終了"*/ ) ) ;

	// 終了
	return 0 ;

ERROR_DX:
	NS_DxLib_End() ;

	DXST_LOGFILE_TABSUB ;
	DXST_LOGFILEFMT_ADDA(( "Failed to initialize the DX library" /*"ＤＸライブラリの初期化処理失敗"*/ )) ;

	// 初期化中フラグを倒す
	DxSysData.DxLib_RunInitializeFlag = FALSE ;

	return -1 ;
} 

// ライブラリ使用の終了関数
extern int NS_DxLib_End( void )
{
	// 既に終了処理が行われているか、そもそも初期化されていない場合は何もしない
	if( DxSysData.DxLib_InitializeFlag == FALSE )
	{
		return 0 ;
	}

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込みをしない
	NS_SetUseASyncLoadFlag( FALSE ) ;

	// 全ての非同期読み込みの終了待ち
	while( NS_GetASyncLoadNum() > 0 )
	{
		// 削除リクエストが来ているハンドルを削除する
		DeleteRequestHandleDelete( FALSE ) ;

		// メインスレッドが処理する非同期読み込みの処理を行う
		ProcessASyncLoadRequestMainThread() ;
		Thread_Sleep( 1 ) ;
	}

	// 削除リクエストが来ているハンドルを全て削除する
	DeleteRequestHandleDelete( TRUE ) ;
#endif // DX_NON_ASYNCLOAD

#ifndef DX_NON_SOFTIMAGE
	// 登録した全てのソフトイメージを削除
	InitSoftImage() ;
#endif // DX_NON_SOFTIMAGE

	// 各処理系の終了
#if !defined( DX_NON_LOG ) && !defined( DX_NON_PRINTF_DX )
	TerminateLog() ;			// ログ処理の後始末
#endif

#ifndef DX_NON_NETWORK
	TerminateNetWork() ;		// ＷｉｎＳｏｃｋｅｔｓ関係の終了
#endif

#ifndef DX_NON_LIVE2D_CUBISM4
	// Live2D Cubism4 関連の後始末
	Live2DCubism4_Terminate() ;
#endif // DX_NON_LIVE2D_CUBISM4

#ifndef DX_NON_SOUND
	NS_StopMusic() ;			// ＭＩＤＩが演奏されている状態の場合それを止める
#endif // DX_NON_SOUND

#ifndef DX_NON_MODEL
	MV1Terminate() ;			// モデルバージョン１の後始末
#endif

#ifndef DX_NON_GRAPHICS
	Graphics_Terminate() ;
#endif // DX_NON_GRAPHICS

	TerminateBaseImageManage() ;

#ifndef DX_NON_SOFTIMAGE
	TerminateSoftImageManage() ;
#endif // DX_NON_SOFTIMAGE

#ifndef DX_NON_MOVIE
	TerminateMovieManage() ;
#endif

#ifndef DX_NON_INPUT
	TerminateInputSystem() ;	// 入力システムの終了
#endif // DX_NON_INPUT

#ifndef DX_NON_SOUND
	TerminateSoundSystem() ;	// サウンドシステムの後始末
	TerminateSoundConvert() ;	// サウンド変換処理の終了
#endif // DX_NON_SOUND

	// ＤＸライブラリ初期化完了フラグを倒す
	DxSysData.DxLib_InitializeFlag = FALSE ;

	// アーカイブファイルアクセス用のデータの後始末
#ifndef DX_NON_DXA
	DXA_DIR_Terminate() ;
#endif

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理用のスレッドを閉じる
	CloseASyncLoadThread() ;
#endif // DX_NON_ASYNCLOAD

	// ファイルアクセス処理の後始末
	TerminateFile() ;

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理の後始末
	TerminateASyncLoad() ;
#endif // DX_NON_ASYNCLOAD

#ifdef DX_USE_DXLIB_MEM_DUMP
	// メモリダンプを行う
	NS_DxDumpAlloc() ;
#endif

#ifndef DX_NON_LOG
	// ログファイルの後始末
	LogFileTerminate() ;
#endif

	// メモリの後始末を行う
	MemoryTerminate() ;

	// 終了
	return 0 ;
}

// ライブラリの内部で使用している構造体をゼロ初期化して、DxLib_Init の前に行った設定を無効化する( DxLib_Init の前でのみ有効 )
extern int NS_DxLib_GlobalStructInitialize( void )
{
//	_MEMSET( &GRA2, 0, sizeof( GRA2 ) ) ;

#ifndef DX_NON_GRAPHICS
	_MEMSET( &GraphicsSysData, 0, sizeof( GraphicsSysData ) );
#endif // DX_NON_GRAPHICS

	_MEMSET( &BaseImageManage, 0, sizeof( BaseImageManage ) ) ;
#ifndef DX_NON_SOUND
	_MEMSET( &SoundSysData, 0, sizeof( SoundSysData ) );
#endif // DX_NON_SOUND

	return 0;
}











// エラー処理関数

// エラー処理
extern int DxLib_Error( const wchar_t *ErrorStr )
{
	// エラーログの排出
	DXST_LOGFILE_ADDW( ErrorStr ) ;
	DXST_LOGFILE_ADDW( L"\n" ) ;

	// 各処理系の終了
	NS_DxLib_End() ;

	exit( -1 ) ;

	return -1 ;
}

// ライブラリのエラー処理を行う( UTF16LE版 )
extern int DxLib_ErrorUTF16LE( const char *ErrorStr )
{
	int Result ;

	CHAR_TO_WCHAR_T_STRING_BEGIN( ErrorStr )
	CHAR_TO_WCHAR_T_STRING_SETUP( ErrorStr, return -1, DX_CHARCODEFORMAT_UTF16LE )

	Result = DxLib_Error( UseErrorStrBuffer ) ;

	CHAR_TO_WCHAR_T_STRING_END( ErrorStr )

	return Result ;
}



























// カウンタ及び時刻取得系関数

// ミリ秒単位の精度を持つカウンタの現在値を得る
extern int NS_GetNowCount( int /*UseRDTSCFlag*/ )
{
	LONGLONG ResultLL ;
	int Result ;

	ResultLL  = NS_GetNowHiPerformanceCount( FALSE ) / 1000 ;
	ResultLL &= 0x7fffffff ;
	Result    = ( int )ResultLL ;

	return Result ;
}

// GetNowTimeの高精度バージョン
extern LONGLONG NS_GetNowHiPerformanceCount( int /*UseRDTSCFlag*/ )
{
	ULONGLONG NowTime ;
	mach_timebase_info_data_t BaseInfo ;

	mach_timebase_info( &BaseInfo ) ;

	NowTime = mach_absolute_time() ;

	NowTime = NowTime * BaseInfo.numer / BaseInfo.denom / 1000 ;

	return NowTime ;
}

// OSが提供する高精度カウンタの現在の値を得る
extern ULONGLONG NS_GetNowSysPerformanceCount( void )
{
	return mach_absolute_time() ;
}

// OSが提供する高精度カウンタの周波数( 1秒辺りのカウント数 )を得る
extern ULONGLONG NS_GetSysPerformanceFrequency( void )
{
	mach_timebase_info_data_t BaseInfo ;
	mach_timebase_info( &BaseInfo ) ;
	return BaseInfo.denom / BaseInfo.numer ;
}

// OSが提供する高精度カウンタの値を秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToSeconds( ULONGLONG Count )
{
	mach_timebase_info_data_t BaseInfo ;
	mach_timebase_info( &BaseInfo ) ;
	return Count * BaseInfo.numer / BaseInfo.denom / 1000000000 ;
}

// OSが提供する高精度カウンタの値をミリ秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToMilliSeconds( ULONGLONG Count )
{
	mach_timebase_info_data_t BaseInfo ;
	mach_timebase_info( &BaseInfo ) ;
	return Count * BaseInfo.numer / BaseInfo.denom / 1000000 ;
}

// OSが提供する高精度カウンタの値をマイクロ秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToMicroSeconds( ULONGLONG Count )
{
	mach_timebase_info_data_t BaseInfo ;
	mach_timebase_info( &BaseInfo ) ;
	return Count * BaseInfo.numer / BaseInfo.denom / 1000 ;
}

// OSが提供する高精度カウンタの値をナノ秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToNanoSeconds( ULONGLONG Count )
{
	mach_timebase_info_data_t BaseInfo ;
	mach_timebase_info( &BaseInfo ) ;
	return Count * BaseInfo.numer / BaseInfo.denom ;
}

// 秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvSecondsToSysPerformanceCount( ULONGLONG Seconds )
{
	LONGLONG Temp[ 2 ] ;
	LONGLONG MulNum ;
	mach_timebase_info_data_t BaseInfo ;
	mach_timebase_info( &BaseInfo ) ;
	MulNum = BaseInfo.denom * 1000000000 ;
	_MUL128_1( ( DWORD * )&MulNum, ( DWORD * )&Seconds, ( DWORD * )Temp ) ;
	if( Temp[ 1 ] != 0 )
	{
		LONGLONG Result ;
		_DIV128_1( ( DWORD * )Temp, ( DWORD * )&BaseInfo.numer, ( DWORD * )&Result ) ;
		return Result ;
	}
	else
	{
		return ( ULONGLONG )Temp[ 0 ] / BaseInfo.numer ;
	}
}

// ミリ秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvMilliSecondsToSysPerformanceCount( ULONGLONG MilliSeconds )
{
	LONGLONG Temp[ 2 ] ;
	LONGLONG MulNum ;
	mach_timebase_info_data_t BaseInfo ;
	mach_timebase_info( &BaseInfo ) ;
	MulNum = BaseInfo.denom * 1000000 ;
	_MUL128_1( ( DWORD * )&MulNum, ( DWORD * )&MilliSeconds, ( DWORD * )Temp ) ;
	if( Temp[ 1 ] != 0 )
	{
		LONGLONG Result ;
		_DIV128_1( ( DWORD * )Temp, ( DWORD * )&BaseInfo.numer, ( DWORD * )&Result ) ;
		return Result ;
	}
	else
	{
		return ( ULONGLONG )Temp[ 0 ] / BaseInfo.numer ;
	}
}

// マイクロ秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvMicroSecondsToSysPerformanceCount( ULONGLONG MicroSeconds )
{
	LONGLONG Temp[ 2 ] ;
	LONGLONG MulNum ;
	mach_timebase_info_data_t BaseInfo ;
	mach_timebase_info( &BaseInfo ) ;
	MulNum = BaseInfo.denom * 1000 ;
	_MUL128_1( ( DWORD * )&MulNum, ( DWORD * )&MicroSeconds, ( DWORD * )Temp ) ;
	if( Temp[ 1 ] != 0 )
	{
		LONGLONG Result ;
		_DIV128_1( ( DWORD * )Temp, ( DWORD * )&BaseInfo.numer, ( DWORD * )&Result ) ;
		return Result ;
	}
	else
	{
		return ( ULONGLONG )Temp[ 0 ] / BaseInfo.numer ;
	}
}

// ナノ秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvNanoSecondsToSysPerformanceCount( ULONGLONG NanoSeconds )
{
	LONGLONG Temp[ 2 ] ;
	LONGLONG MulNum ;
	mach_timebase_info_data_t BaseInfo ;
	mach_timebase_info( &BaseInfo ) ;
	MulNum = BaseInfo.denom ;
	_MUL128_1( ( DWORD * )&MulNum, ( DWORD * )&NanoSeconds, ( DWORD * )Temp ) ;
	if( Temp[ 1 ] != 0 )
	{
		LONGLONG Result ;
		_DIV128_1( ( DWORD * )Temp, ( DWORD * )&BaseInfo.numer, ( DWORD * )&Result ) ;
		return Result ;
	}
	else
	{
		return ( ULONGLONG )Temp[ 0 ] / BaseInfo.numer ;
	}
}

// 現在時刻を取得する
extern int NS_GetDateTime( DATEDATA *DateBuf )
{
	time_t nowtime ;
	tm *datetime ;

	time( &nowtime ) ;

	datetime = localtime( &nowtime ) ;

	// ローカル時刻データを元に専用のデータ型データに時刻を繁栄させる
	DateBuf->Year	= datetime->tm_year + 1900 ;
	DateBuf->Mon	= datetime->tm_mon + 1 ;
	DateBuf->Day	= datetime->tm_mday ;
	DateBuf->Hour	= datetime->tm_hour ;
	DateBuf->Min	= datetime->tm_min ;
	DateBuf->Sec	= datetime->tm_sec ;

	// 終了
	return 0 ;
}




// バッテリー関連

// 電池の残量を % で取得する( 戻り値： 100=フル充電状態  0=充電残量無し )
extern int NS_GetBatteryLifePercent( void )
{
	// 未実装
	return -1 ;
}







// クリップボード関係

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int GetClipboardText_PF( TCHAR *DestBuffer, int DestBufferBytes )
{
	// 未実装
	return -1 ;
}

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int GetClipboardText_WCHAR_T_PF( wchar_t *DestBuffer, int DestBufferBytes )
{
	// 未実装
	return -1 ;
}

// クリップボードにテキストデータを格納する
extern int SetClipboardText_WCHAR_T_PF( const wchar_t *Text )
{
	// 未実装
	return -1 ;
}










// メールアプリを送信メール編集状態で起動する
// MailAddr    : 宛先( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// MainCCAddr  : CC の宛先( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// MainBCCAddr : BCC の宛先( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// Subject     : タイトル( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// Text        : 本文( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
extern int MailApp_Send_WCHAR_T_PF( const wchar_t *MailAddr, const wchar_t *MailCCAddr, const wchar_t *MailBCCAddr, const wchar_t *Subject, const wchar_t *Text )
{
	int Result = -1 ;
	char *MailAddrUTF8 = NULL ;
	char *MailCCAddrUTF8 = NULL ;
	char *MailBCCAddrUTF8 = NULL ;
	char *SubjectUTF8 = NULL ;
	char *TextUTF8 = NULL ;
	int Bytes ;

	if( MailAddr != NULL )
	{
		Bytes = ConvString( ( char * )MailAddr, -1, WCHAR_T_CHARCODEFORMAT, NULL, 0, DX_CHARCODEFORMAT_UTF8 ) ;
		MailAddrUTF8 = ( char * )DXALLOC( Bytes ) ;
		if( MailAddrUTF8 == NULL )
		{
			goto END ;
		}
		ConvString( ( char * )MailAddr, -1, WCHAR_T_CHARCODEFORMAT, MailAddrUTF8, Bytes, DX_CHARCODEFORMAT_UTF8 ) ;
	}

	if( MailCCAddr != NULL )
	{
		Bytes = ConvString( ( char * )MailCCAddr, -1, WCHAR_T_CHARCODEFORMAT, NULL, 0, DX_CHARCODEFORMAT_UTF8 ) ;
		MailCCAddrUTF8 = ( char * )DXALLOC( Bytes ) ;
		if( MailCCAddrUTF8 == NULL )
		{
			goto END ;
		}
		ConvString( ( char * )MailCCAddr, -1, WCHAR_T_CHARCODEFORMAT, MailCCAddrUTF8, Bytes, DX_CHARCODEFORMAT_UTF8 ) ;
	}

	if( MailBCCAddr != NULL )
	{
		Bytes = ConvString( ( char * )MailBCCAddr, -1, WCHAR_T_CHARCODEFORMAT, NULL, 0, DX_CHARCODEFORMAT_UTF8 ) ;
		MailBCCAddrUTF8 = ( char * )DXALLOC( Bytes ) ;
		if( MailBCCAddrUTF8 == NULL )
		{
			goto END ;
		}
		ConvString( ( char * )MailBCCAddr, -1, WCHAR_T_CHARCODEFORMAT, MailBCCAddrUTF8, Bytes, DX_CHARCODEFORMAT_UTF8 ) ;
	}

	if( Subject != NULL )
	{
		Bytes = ConvString( ( char * )Subject, -1, WCHAR_T_CHARCODEFORMAT, NULL, 0, DX_CHARCODEFORMAT_UTF8 ) ;
		SubjectUTF8 = ( char * )DXALLOC( Bytes ) ;
		if( SubjectUTF8 == NULL )
		{
			goto END ;
		}
		ConvString( ( char * )Subject, -1, WCHAR_T_CHARCODEFORMAT, SubjectUTF8, Bytes, DX_CHARCODEFORMAT_UTF8 ) ;
	}

	if( Text != NULL )
	{
		Bytes = ConvString( ( char * )Text, -1, WCHAR_T_CHARCODEFORMAT, NULL, 0, DX_CHARCODEFORMAT_UTF8 ) ;
		TextUTF8 = ( char * )DXALLOC( Bytes ) ;
		if( TextUTF8 == NULL )
		{
			goto END ;
		}
		ConvString( ( char * )Text, -1, WCHAR_T_CHARCODEFORMAT, TextUTF8, Bytes, DX_CHARCODEFORMAT_UTF8 ) ;
	}

	Result = MailApp_Send_iOS( MailAddrUTF8, MailCCAddrUTF8, MailBCCAddrUTF8, SubjectUTF8, TextUTF8 ) ;

END :
	if( MailAddrUTF8 != NULL )
	{
		DXFREE( MailAddrUTF8 ) ;
		MailAddrUTF8 = NULL ;
	}

	if( MailCCAddrUTF8 != NULL )
	{
		DXFREE( MailCCAddrUTF8 ) ;
		MailCCAddrUTF8 = NULL ;
	}

	if( MailBCCAddrUTF8 != NULL )
	{
		DXFREE( MailBCCAddrUTF8 ) ;
		MailBCCAddrUTF8 = NULL ;
	}

	if( SubjectUTF8 != NULL )
	{
		DXFREE( SubjectUTF8 ) ;
		SubjectUTF8 = NULL ;
	}

	if( TextUTF8 != NULL )
	{
		DXFREE( TextUTF8 ) ;
		TextUTF8 = NULL ;
	}

	// 戻り値を返す
	return Result ;
}









#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
































#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// ウインドウズのメッセージループに代わる処理を行う
extern int NS_ProcessMessage( void )
{
	static int EndFlag = FALSE ;

	// もしフラグがたっていたらなにもせず終了
	if( EndFlag )
	{
		return 0 ;
	}

	// ファイル処理の周期的処理を行う
//	ReadOnlyFileAccessProcessAll() ;

#ifndef DX_NON_SOUND
	{
		// サウンドの周期的処理を行う
//		NS_ProcessStreamSoundMemAll() ;
//		ST_SoftSoundPlayerProcessAll() ;
		ProcessPlayFinishDeleteSoundMemAll() ;
//		SoundBuffer_Apply_StopSoundBufferList() ;
//		ProcessPlay3DSoundMemAll() ;
	}
#endif // DX_NON_SOUND

#ifndef DX_NON_ASYNCLOAD
	// 削除リクエストが来ているハンドルを削除する
	DeleteRequestHandleDelete( FALSE ) ;

	// メインスレッドが処理する非同期読み込みの処理を行う
	ProcessASyncLoadRequestMainThread() ;
#endif // DX_NON_ASYNCLOAD

	// 演奏の周期的処理を行う
#ifndef DX_NON_SOUND
	NS_ProcessMusicMem() ;
#endif // DX_NON_SOUND

#ifndef DX_NON_INPUT
	// キーボード入力の更新処理を行う
	UpdateKeyboardInputState( FALSE ) ;

	// パッドの周期的処理を行う
	JoypadEffectProcess() ;
#endif // DX_NON_INPUT

#ifndef DX_NON_NETWORK
	// 通信関係のメッセージ処理を行う
	NS_ProcessNetMessage( TRUE ) ;
#endif

	// メモリ関係の周期的処理を行う
	MemoryProcess() ;

#ifndef DX_NON_SOUND
	// サウンド関係の周期的処理を行う
	ProcessSoundSystem() ;
#endif // DX_NON_SOUND

#ifndef DX_NON_GRAPHICS
	// 画面関係の周期処理を行う
	Graphics_iOS_FrontScreenProcess() ;
#endif // DX_NON_GRAPHICS

#ifndef DX_NON_KEYEX
	// キー入力処理を行う
	{
		// フラグをたてる
		EndFlag = TRUE ;

		NS_ProcessActKeyInput() ;

		// フラグを倒す
		EndFlag = FALSE ;
	}
#endif

	// イベント処理ループ
	for(;;)
	{
		break ;
	}

	// 通常終了
	return 0 ;
}

// アプリがアクティブではない状態でも処理を続行するか、フラグをセットする
extern int NS_SetAlwaysRunFlag( int Flag )
{
	// フラグをセット
	g_iOSSys.NonActiveRunFlag = Flag ;
	
	// 終了
	return 0 ;
}

// ソフトが非アクティブになった際に呼ばれるコールバック関数を登録する
extern int SetiOSLostFocusCallbackFunction( void (* Callback )( void *Data ), void *CallbackData )
{
	g_iOSSys.LostFocusCallbackFunction     = ( volatile void ( * )( void * ) )Callback ;
	g_iOSSys.LostFocusCallbackFunctionData = ( volatile void * )CallbackData ;

	return 0 ;
}

// ソフトがアクティブになった際に呼ばれるコールバック関数を登録する
extern int SetiOSGainedFocusCallbackFunction( void (* Callback )( void *Data ), void *CallbackData )
{
	g_iOSSys.GainedFocusCallbackFunction     = ( volatile void ( * )( void * ) )Callback ;
	g_iOSSys.GainedFocusCallbackFunctionData = ( volatile void * )CallbackData ;

	return 0 ;
}

// スクリーン座標をＤＸライブラリ画面座標に変換する
extern int ConvScreenPositionToDxScreenPosition( int ScreenX, int ScreenY, int *DxScreenX, int *DxScreenY )
{
#ifdef DX_NON_GRAPHICS
	*DxScreenX = ScreenX ;
	*DxScreenY = ScreenY ;
#else // DX_NON_GRAPHICS

	RECT DestRect ;
	DWORD DestW ;
	DWORD DestH ;

	if( GSYS.Screen.FullScreenFitScalingFlag )
	{
		if( DxScreenX != NULL )
		{
			*DxScreenX = ScreenX * GIOS.Device.Screen.SubBackBufferTextureSizeX / GIOS.Device.Screen.Width ;
		}

		if( DxScreenY != NULL )
		{
			*DxScreenY = ScreenY * GIOS.Device.Screen.SubBackBufferTextureSizeY / GIOS.Device.Screen.Height ;
		}
	}
	else
	{
		DestW = GIOS.Device.Screen.Width ;
		DestH = GIOS.Device.Screen.Width * GIOS.Device.Screen.SubBackBufferTextureSizeY / GIOS.Device.Screen.SubBackBufferTextureSizeX ;
		if( DestH > GIOS.Device.Screen.Height )
		{
			DestW = GIOS.Device.Screen.Height * GIOS.Device.Screen.SubBackBufferTextureSizeX / GIOS.Device.Screen.SubBackBufferTextureSizeY ;
			DestH = GIOS.Device.Screen.Height ;
		}

		DestRect.left   = ( GIOS.Device.Screen.Width  - DestW ) / 2 ;
		DestRect.top    = ( GIOS.Device.Screen.Height - DestH ) / 2 ;
		DestRect.right  = DestRect.left + DestW ;
		DestRect.bottom = DestRect.top  + DestH ;

		if( DxScreenX != NULL )
		{
			*DxScreenX = ( ScreenX - DestRect.left ) * GIOS.Device.Screen.SubBackBufferTextureSizeX / ( int )DestW ;
		}

		if( DxScreenY != NULL )
		{
			*DxScreenY = ( ScreenY - DestRect.top  ) * GIOS.Device.Screen.SubBackBufferTextureSizeY / ( int )DestH ;
		}
	}

#endif // DX_NON_GRAPHICS

	// 終了
	return 0 ;
}

// アクティブになるまで何もしない
extern void DxActiveWait_iOS( void )
{
//	while(
//		g_iOSSys.NonActiveRunFlag == FALSE
//		)
//	)
//	{
//		if( NS_ProcessMessage() < 0 )
//		{
//			break ;
//		}
//		Thread_Sleep( 32 ) ;
//	}
}

// ディレクトリパスを返す関数の共通処理用の関数
static int DirPathCommonFunction( const char *AddPath, TCHAR *PathBuffer, int PathBufferBytes )
{
	const char *HomeDir = GetHomeDirectory() ;
	char DirPath[ 2048 ] ;
	int Result ;
	size_t Length = CL_strlen( DX_CHARCODEFORMAT_UTF8, HomeDir ) + CL_strlen( DX_CHARCODEFORMAT_UTF8, AddPath ) ;

	if( Length + 1 > sizeof( DirPath ) )
	{
		return -1 ;
	}

	CL_strcpy( DX_CHARCODEFORMAT_UTF8, DirPath, HomeDir ) ;
	CL_strcat( DX_CHARCODEFORMAT_UTF8, DirPath, AddPath ) ;

	if( PathBuffer == NULL )
	{
		TCHAR *TempBuffer ;
		size_t BufferSize ;

		BufferSize = ( Length + 1 ) * 16 ;
		TempBuffer = ( TCHAR * )DXALLOC( BufferSize ) ;
		if( TempBuffer == NULL )
		{
			return -1 ;
		}
		Result = ConvString( DirPath, -1, DX_CHARCODEFORMAT_UTF8, TempBuffer, BufferSize, _TCHARCODEFORMAT ) ;
		DXFREE( TempBuffer ) ;
		TempBuffer = NULL ;
	}
	else
	{
		Result = ConvString( DirPath, -1, DX_CHARCODEFORMAT_UTF8, PathBuffer, PathBufferBytes, _TCHARCODEFORMAT ) ;
	}

	// 終了
	return Result ;
}

// ソフトの外部データ保存用のディレクトリパスを取得する
extern int GetDocumentsDirPath( TCHAR *PathBuffer, size_t PathBufferBytes )
{
	return DirPathCommonFunction( "/Documents", PathBuffer, PathBufferBytes ) ;
}

// ソフトのデータ保存用のディレクトリパスを取得する
extern int GetLibraryPreferencesDirPath( TCHAR *PathBuffer, size_t PathBufferBytes )
{
	return DirPathCommonFunction( "/Library/Preferences", PathBuffer, PathBufferBytes ) ;
}

// ソフトのキャッシュファイル保存用のディレクトリパスを取得する
extern int GetLibraryCachesDirPath( TCHAR *PathBuffer, size_t PathBufferBytes )
{
	return DirPathCommonFunction( "/Library/Caches", PathBuffer, PathBufferBytes ) ;
}

// ディレクトリパスを返す関数の共通処理用の関数
static int DirPathCommonFunctionForChar( const char *AddPath, char *PathBuffer, int PathBufferBytes )
{
	const char *HomeDir = GetHomeDirectory() ;
	size_t Length = CL_strlen( DX_CHARCODEFORMAT_UTF8, HomeDir ) + CL_strlen( DX_CHARCODEFORMAT_UTF8, AddPath ) ;

	if( Length + 1 > PathBufferBytes )
	{
		return Length + 1 ;
	}

	if( PathBuffer != NULL )
	{
		CL_strcpy( DX_CHARCODEFORMAT_UTF8, PathBuffer, HomeDir ) ;
		CL_strcat( DX_CHARCODEFORMAT_UTF8, PathBuffer, AddPath ) ;
	}

	// 終了
	return Length + 1 ;
}

// ソフトの外部データ保存用のディレクトリパスを取得する
extern int GetDocumentsDirPathForChar( char *PathBuffer, size_t PathBufferBytes )
{
	return DirPathCommonFunctionForChar( "/Documents", PathBuffer, PathBufferBytes ) ;
}

// ソフトのデータ保存用のディレクトリパスを取得する
extern int GetLibraryPreferencesDirPathForChar( char *PathBuffer, size_t PathBufferBytes )
{
	return DirPathCommonFunctionForChar( "/Library/Preferences", PathBuffer, PathBufferBytes ) ;
}

// ソフトのキャッシュファイル保存用のディレクトリパスを取得する
extern int GetLibraryCachesDirPathForChar( char *PathBuffer, size_t PathBufferBytes )
{
	return DirPathCommonFunctionForChar( "/Library/Caches", PathBuffer, PathBufferBytes ) ;
}

// ソフトの一時ファイル保存用のディレクトリパスを取得する
extern int GetTmpDirPathForChar( char *PathBuffer, size_t PathBufferBytes )
{
	const char *TmpDirPath = GetTmpDirectory() ;
	int Length = CL_strlen( DX_CHARCODEFORMAT_UTF8, TmpDirPath ) ;

	if( Length + 1 > PathBufferBytes )
	{
		return Length + 1 ;
	}

	if( PathBuffer != NULL )
	{
		CL_strcpy_s( DX_CHARCODEFORMAT_UTF8, PathBuffer, PathBufferBytes, TmpDirPath ) ;
	}

	// 終了
	return Length + 1 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

extern void DrawFunction( void );
extern void SetAddPos( float x, float y );



// ソフト用スレッドのエントリーポイント
static void* ios_app_entry( void * )
{
	// カレントコンテキストのセット
    GraphicsSetCurrentContext() ;
    
#ifndef DX_NON_FONT
    // フォントタイプはアンチエイリアスタイプに固定
    SetAntialiasingFontOnlyFlag( TRUE ) ;
#endif // DX_NON_FONT
    
#ifndef DX_NON_DXA
    // DXアーカイブのパスを大文字にしないようにする
    DXA_DIR_SetNotArchivePathCharUp( TRUE ) ;
#endif // DX_NON_DXA

	ios_main() ;
    
    return NULL ;
}

// C++ 側での初期化を行う
extern int DxSystemInitialize_CPP( void )
{
	// ソフト実行用スレッド生成
	pthread_attr_t attr ;
	pthread_attr_init( &attr ) ;
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED ) ;
	pthread_attr_setstacksize( &attr, 4 * 1024 * 1024 ) ;	// スタックサイズは 4MB
	pthread_create( &g_iOSSys.SoftThread, &attr, ios_app_entry, NULL ) ;
    
    return 0 ;
}


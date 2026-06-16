// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Linux用システムプログラム
// 
// 				Ver 3.24f
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "DxSystemLinux.h"

// インクルード ------------------------------------------------------------------
#include "DxGraphicsLinux.h"
#include "DxGraphicsFilterLinux.h"
#include "DxLogLinux.h"
#include "DxMaskLinux.h"
#include "DxModelLinux.h"
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
#include "../DxHandle.h"
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xlib.h>
#include <GLES2/gl2.h>

#include "../DxBaseFunc.h"
#include "../DxMemory.h"



#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define TIME_DISTANCE( now, time )			( (now) < (time) ? 0x7fffffff - (time) + (now) : (now) - (time) )

// 構造体定義 --------------------------------------------------------------------


// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

DXLIB_LINUX_SYSTEMINFO g_LinuxSys ;
int g_LinuxRunFlag ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// プログラム --------------------------------------------------------------------

// UTF16LE の書式文字列と UTF8 のパラメータ文字列をログ出力する
static void OutputLinuxOSInfo_LogAddUTF8( const char *UTF16LEFormatStr, const char *UTF8Str )
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
	LONGLONG NowTime ;
	timeval ltimeval ;

	gettimeofday( &ltimeval, NULL ) ;

	NowTime = ( LONGLONG )ltimeval.tv_sec * 1000000 + ltimeval.tv_usec ;

	return NowTime ;
}

// OSが提供する高精度カウンタの現在の値を得る
extern ULONGLONG NS_GetNowSysPerformanceCount( void )
{
	return ( ULONGLONG )NS_GetNowHiPerformanceCount( FALSE ) ;
}

// OSが提供する高精度カウンタの周波数( 1秒辺りのカウント数 )を得る
extern ULONGLONG NS_GetSysPerformanceFrequency( void )
{
	return 1000000 ;
}

// OSが提供する高精度カウンタの値を秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToSeconds( ULONGLONG Count )
{
	return Count / 1000000 ;
}

// OSが提供する高精度カウンタの値をミリ秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToMilliSeconds( ULONGLONG Count )
{
	return Count / 1000 ;
}

// OSが提供する高精度カウンタの値をマイクロ秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToMicroSeconds( ULONGLONG Count )
{
	return Count ;
}

// OSが提供する高精度カウンタの値をナノ秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToNanoSeconds( ULONGLONG Count )
{
	return Count * 1000 ;
}

// 秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvSecondsToSysPerformanceCount( ULONGLONG Seconds )
{
	return Seconds * 1000000 ;
}

// ミリ秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvMilliSecondsToSysPerformanceCount( ULONGLONG MilliSeconds )
{
	return MilliSeconds * 1000 ;
}

// マイクロ秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvMicroSecondsToSysPerformanceCount( ULONGLONG MicroSeconds )
{
	return MicroSeconds ;
}

// ナノ秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvNanoSecondsToSysPerformanceCount( ULONGLONG NanoSeconds )
{
	return NanoSeconds / 1000 ;
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

Bool PredicateClipboardTargetsNotify(Display *d, XEvent *ev, XPointer arg){
    return (ev->type == SelectionNotify)
        && (ev->xselection.requestor == GLINUX.Device.Screen.XWindow)
        && (ev->xselection.selection == GLINUX.Device.Screen._atom_CLIPBOARD)
        && (ev->xselection.target == GLINUX.Device.Screen._atom_TARGETS)
        && (ev->xselection.property == GLINUX.Device.Screen._atom_CLIPBOARD);
}
Bool PredicateClipboardUtf8StringNotify(Display *d, XEvent *ev, XPointer arg){
    return (ev->type == SelectionNotify)
        && (ev->xselection.requestor == GLINUX.Device.Screen.XWindow)
        && (ev->xselection.selection == GLINUX.Device.Screen._atom_CLIPBOARD)
        && (ev->xselection.target == GLINUX.Device.Screen._atom_UTF8_STRING)
        && (ev->xselection.property == GLINUX.Device.Screen._atom_CLIPBOARD);
}
size_t CopyClipboardTextToInternal(void* buf, size_t len){
    DXFREE(GLINUX.Device.Screen._clipboard_get_bufPtr);
    // lenはゼロ終端を含まないので注意
    GLINUX.Device.Screen._clipboard_get_bufPtr = (char*)DXALLOC(len+1);
    GLINUX.Device.Screen._clipboard_get_bufLen = len+1;
    if( GLINUX.Device.Screen._clipboard_get_bufPtr == NULL ) {
        GLINUX.Device.Screen._clipboard_get_bufLen = 0;
        return -1;
    }
    _MEMCPY(GLINUX.Device.Screen._clipboard_get_bufPtr, buf, len);
    GLINUX.Device.Screen._clipboard_get_bufPtr[len] = '\0';
    return len+1;
}
int GetWindowPropertyBulk(Display *d, Window w, Atom property, size_t(bulk)(void*, size_t)){
    Atom retType;
    int retFormat;
    unsigned long retNum;
    unsigned long leftByte;
    unsigned char* p;
    int ret;
    XGetWindowProperty(d, w, property, 0, 0x7fffffff, False, AnyPropertyType, &retType, &retFormat, &retNum, &leftByte, &p);
    //printf("prop: rT:%lu rF:%d rN:%lu lB:%lu p:%p\n", retType, retFormat, retNum, leftByte, p);
    if(bulk){
        ret = (int)bulk(p, retNum); // TODO: 2GB+
    }
    XFree(p);
    return ret;
}

int GetClipboardText_PF_LinuxCommon(void *DestBuffer, int DestBufferBytes, int isUnicode){
    // isUnicodeは現状未対応
    if(!DestBuffer){
        // 第一引数がNULL、内部バッファにコピー
        Atom owner = XGetSelectionOwner(GLINUX.Device.Screen.XDisplay, GLINUX.Device.Screen._atom_CLIPBOARD);
        if(owner == None){
            // クリップボード所有者なし
            return -1;
        }
        if(owner == GLINUX.Device.Screen.XWindow){
            // クリップボード所有者が自分、クリップボード設定用バッファからコピー
            DXFREE(GLINUX.Device.Screen._clipboard_get_bufPtr);
            GLINUX.Device.Screen._clipboard_get_bufPtr = (char*)DXALLOC(GLINUX.Device.Screen._clipboard_set_bufLen);
            GLINUX.Device.Screen._clipboard_get_bufLen = GLINUX.Device.Screen._clipboard_set_bufLen;
            if( GLINUX.Device.Screen._clipboard_get_bufPtr == NULL ) {
                GLINUX.Device.Screen._clipboard_get_bufLen = 0;
                return -1;
            }
            _MEMCPY(GLINUX.Device.Screen._clipboard_get_bufPtr, GLINUX.Device.Screen._clipboard_set_bufPtr, GLINUX.Device.Screen._clipboard_get_bufLen);
            return GLINUX.Device.Screen._clipboard_get_bufLen;
        }
        XEvent ev;
        XConvertSelection(GLINUX.Device.Screen.XDisplay, GLINUX.Device.Screen._atom_CLIPBOARD, GLINUX.Device.Screen._atom_TARGETS, GLINUX.Device.Screen._atom_CLIPBOARD, GLINUX.Device.Screen.XWindow, CurrentTime);
        XIfEvent(GLINUX.Device.Screen.XDisplay, &ev, &PredicateClipboardTargetsNotify, NULL);
        XConvertSelection(GLINUX.Device.Screen.XDisplay, GLINUX.Device.Screen._atom_CLIPBOARD, GLINUX.Device.Screen._atom_UTF8_STRING, GLINUX.Device.Screen._atom_CLIPBOARD, GLINUX.Device.Screen.XWindow, CurrentTime);
        XIfEvent(GLINUX.Device.Screen.XDisplay, &ev, &PredicateClipboardUtf8StringNotify, NULL);
        return GetWindowPropertyBulk(GLINUX.Device.Screen.XDisplay, ev.xselection.requestor, GLINUX.Device.Screen._atom_CLIPBOARD, &CopyClipboardTextToInternal);
    }else{
        // 第一引数がポインタ、内部バッファからコピー
        if(GLINUX.Device.Screen._clipboard_get_bufPtr == NULL || GLINUX.Device.Screen._clipboard_get_bufLen == 0){
            // ポインタが無効か空であれば失敗
            return -1;
        }
        if(DestBufferBytes != GLINUX.Device.Screen._clipboard_get_bufLen){
            // 引数とバッファサイズと一致しない場合は失敗
            DXFREE(GLINUX.Device.Screen._clipboard_get_bufPtr);
            GLINUX.Device.Screen._clipboard_get_bufPtr = NULL;
            GLINUX.Device.Screen._clipboard_get_bufLen = 0;
            return -1;
        }
        _MEMCPY(DestBuffer, GLINUX.Device.Screen._clipboard_get_bufPtr, DestBufferBytes);
        // 第一引数がポインタでの二回連続呼び出し抑止
        DXFREE(GLINUX.Device.Screen._clipboard_get_bufPtr);
        GLINUX.Device.Screen._clipboard_get_bufPtr = NULL;
        GLINUX.Device.Screen._clipboard_get_bufLen = 0;
        return 0;
    }
}

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int GetClipboardText_PF( TCHAR *DestBuffer, int DestBufferBytes )
{
	return GetClipboardText_PF_LinuxCommon(DestBuffer, DestBufferBytes, 0) ;
}

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int GetClipboardText_WCHAR_T_PF( wchar_t *DestBuffer, int DestBufferBytes )
{
	return GetClipboardText_PF_LinuxCommon(DestBuffer, DestBufferBytes, 1) ;
}


// クリップボードにテキストデータを格納する
extern int SetClipboardText_WCHAR_T_PF( const wchar_t *Text )
{
	XSetSelectionOwner(GLINUX.Device.Screen.XDisplay, GLINUX.Device.Screen._atom_CLIPBOARD, GLINUX.Device.Screen.XWindow, CurrentTime);
	auto len = ConvertStringCharCodeFormat(WCHAR_T_CHARCODEFORMAT, Text, DX_CHARCODEFORMAT_UTF8, NULL);
	DXFREE(GLINUX.Device.Screen._clipboard_set_bufPtr);
	GLINUX.Device.Screen._clipboard_set_bufPtr = (char*)DXALLOC(len);
	GLINUX.Device.Screen._clipboard_set_bufLen = len;
	if( GLINUX.Device.Screen._clipboard_set_bufPtr == NULL ) {
		GLINUX.Device.Screen._clipboard_set_bufLen = 0;
		return -1;
	}

	auto ret = ConvertStringCharCodeFormat(WCHAR_T_CHARCODEFORMAT, Text, DX_CHARCODEFORMAT_UTF8, GLINUX.Device.Screen._clipboard_set_bufPtr);
	return 0;
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
	Graphics_Linux_FrontScreenProcess() ;
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
	Display * xdpy = GLINUX.Device.Screen.XDisplay ;
	while(XPending(xdpy)){
	//while(1){
		//XPending(xdpy);
		XEvent ev ;
		XNextEvent( xdpy, &ev ) ;
		switch( ev.type )
		{
		case ClientMessage :
			if( ev.xclient.message_type == GLINUX.Device.Screen._atom_WM_PROTOCOLS
					&& ev.xclient.data.l[0] == GLINUX.Device.Screen._atom_WM_DELETE_WINDOW )
			{
				return -1 ;
			}
			else if( ev.xclient.message_type == GLINUX.Device.Screen._atom_XdndEnter )
			{
				GLINUX.Device.Screen._xdnd_cache_sourceWindow = ev.xclient.data.l[0];
				if( ev.xclient.data.l[1] & 0x1 ){
					Atom retType;
					int retFormat;
					unsigned long retNum;
					unsigned long leftByte;
					unsigned char* rets;
					XGetWindowProperty( xdpy, ev.xclient.data.l[0], GLINUX.Device.Screen._atom_XdndTypeList, 0, 0x7fffffff, False, AnyPropertyType, &retType, &retFormat, &retNum, &leftByte, &rets );
					Atom* atoms = (Atom*)rets;
					for( int i=0; i<retNum; i++ ){
						GLINUX.Device.Screen._xdnd_cache_itsForMe |= ( atoms[i] == GLINUX.Device.Screen._atom_text_uri_list ? 1 : 0 );
					}
					XFree( rets );
				}else{
					for( int i=2; i<=4; i++ ){
						if( ev.xclient.data.l[i] ){
							GLINUX.Device.Screen._xdnd_cache_itsForMe |= ( ev.xclient.data.l[i] == GLINUX.Device.Screen._atom_text_uri_list ? 1 : 0 );
						}
					}
				}
			}
			else if( ev.xclient.message_type == GLINUX.Device.Screen._atom_XdndLeave )
			{
				GLINUX.Device.Screen._xdnd_cache_itsForMe = 0;
				GLINUX.Device.Screen._xdnd_cache_sourceWindow = 0;
			}
			else if( ev.xclient.message_type == GLINUX.Device.Screen._atom_XdndPosition )
			{
				XClientMessageEvent reply;
				reply.type = ClientMessage;
				reply.serial = 0;
				reply.send_event = 0;
				reply.display = xdpy;
				reply.window = ev.xclient.data.l[0];
				reply.message_type = GLINUX.Device.Screen._atom_XdndStatus;
				reply.format = 32;
				reply.data.l[0] = GLINUX.Device.Screen.XWindow;
				reply.data.l[1] = 1;
				reply.data.l[2] = 0;
				reply.data.l[3] = 0;
				reply.data.l[4] = ev.xclient.data.l[4];  // TODO: only allow XdndActionCopy
				XSendEvent( xdpy, ev.xclient.data.l[0], False, NoEventMask, (XEvent*)&reply );
				XFlush( xdpy );
			}
			else if( ev.xclient.message_type == GLINUX.Device.Screen._atom_XdndDrop )
			{
				if( GLINUX.Device.Screen._xdnd_cache_itsForMe ){
					GLINUX.Device.Screen._xdnd_cache_itsForMe = 0;
					XConvertSelection( xdpy, GLINUX.Device.Screen._atom_XdndSelection, GLINUX.Device.Screen._atom_text_uri_list, XA_PRIMARY, GLINUX.Device.Screen.XWindow, ev.xclient.data.l[2] );
				}else{
					XClientMessageEvent reply;
					reply.type = ClientMessage;
					reply.serial = 0;
					reply.send_event = 0;
					reply.display = xdpy;
					reply.window = ev.xclient.data.l[0];
					reply.message_type = GLINUX.Device.Screen._atom_XdndFinished;
					reply.format = 32;
					reply.data.l[0] = GLINUX.Device.Screen.XWindow;
					reply.data.l[1] = 0;
					reply.data.l[2] = 0;
					reply.data.l[3] = 0;
					reply.data.l[4] = 0;
					XSendEvent( xdpy, ev.xclient.data.l[0], False, NoEventMask, (XEvent*)&reply );
					XFlush( xdpy );
				}
			}
			else
			{
				//printf("xclient %d %d %08x %08x %08x %08x %08x\n", ev.xclient.message_type, ev.xclient.format, ev.xclient.data.l[0], ev.xclient.data.l[1], ev.xclient.data.l[2], ev.xclient.data.l[3], ev.xclient.data.l[4]); fflush(stdout);
			}
			break ;
		case SelectionNotify :
			//printf("selnotifyev: reqwin:%d asel:%d tgt:%d prop:%d\n", ev.xselection.requestor, ev.xselection.selection, ev.xselection.target, ev.xselection.property);
			{
				Atom retType;
				int retFormat;
				unsigned long retNum;
				unsigned long leftByte;
				unsigned char* rets;
				XGetWindowProperty( xdpy, ev.xselection.requestor, XA_PRIMARY, 0, 0x7fffffff, False, AnyPropertyType, &retType, &retFormat, &retNum, &leftByte, &rets );
				if( GLINUX.Device.Screen._xdnd_bufPtr ){
					GLINUX.Device.Screen._xdnd_bufLen += (size_t)retNum;
					GLINUX.Device.Screen._xdnd_bufPtr = (char*)DXREALLOC( GLINUX.Device.Screen._xdnd_bufPtr, GLINUX.Device.Screen._xdnd_bufLen );
				}else{
					GLINUX.Device.Screen._xdnd_bufLen = (size_t)retNum;
					GLINUX.Device.Screen._xdnd_bufPtr = (char*)DXALLOC( GLINUX.Device.Screen._xdnd_bufLen );
				}
				if( !GLINUX.Device.Screen._xdnd_bufPtr ){
					return -1;
				}
				_MEMCPY( GLINUX.Device.Screen._xdnd_bufPtr+GLINUX.Device.Screen._xdnd_bufLen-(size_t)retNum, rets, (size_t)retNum );
				for( int i=0; i<retNum; i++ ){
					GLINUX.Device.Screen._xdnd_bufNum += rets[i] == '\n' ? 1 : 0;
				}
				//printf("---(%zu)\n%.*s---\n", GLINUX.Device.Screen._xdnd_bufLen, GLINUX.Device.Screen._xdnd_bufLen, GLINUX.Device.Screen._xdnd_bufPtr);

				XClientMessageEvent reply;
				reply.type = ClientMessage;
				reply.serial = 0;
				reply.send_event = 0;
				reply.display = xdpy;
				reply.window = GLINUX.Device.Screen._xdnd_cache_sourceWindow;
				reply.message_type = GLINUX.Device.Screen._atom_XdndFinished;
				reply.format = 32;
				reply.data.l[0] = GLINUX.Device.Screen.XWindow;
				reply.data.l[1] = 1;
				reply.data.l[2] = 0;
				reply.data.l[3] = 0;
				reply.data.l[4] = 0;
				XSendEvent( xdpy, GLINUX.Device.Screen._xdnd_cache_sourceWindow, False, NoEventMask, (XEvent*)&reply );
				XFlush( xdpy );
				XFree( rets );
			}
			break ;
		case SelectionClear:
			DXFREE( GLINUX.Device.Screen._clipboard_set_bufPtr );
			GLINUX.Device.Screen._clipboard_set_bufPtr = NULL;
			GLINUX.Device.Screen._clipboard_set_bufLen = 0;
			break;
		case SelectionRequest:
			XSelectionEvent r;
			r.type      = SelectionNotify;
			r.requestor = ev.xselectionrequest.requestor;
			r.selection = ev.xselectionrequest.selection;
			r.target    = ev.xselectionrequest.target;
			r.property  = ev.xselectionrequest.property;
			r.time      = ev.xselectionrequest.time;
			//printf("selrequestev: owner:%d reqwin:%d sel:%d[%s] tgt:%d[%s] prop:%d[%s]\n",
			//	ev.xselectionrequest.owner, r.requestor,
			//	r.selection, r.selection?XGetAtomName(xdpy, r.selection):"0",
			//	r.target,    r.target   ?XGetAtomName(xdpy, r.target   ):"0",
			//	r.property,  r.property ?XGetAtomName(xdpy, r.property ):"0");
			if(r.selection != GLINUX.Device.Screen._atom_CLIPBOARD){
			//	printf("not CLIPBOARD selection");
				r.property = None;
			}else if(r.property == None){
			//	printf("property is None\n");
			}else if(r.target == GLINUX.Device.Screen._atom_TARGETS){
			//	printf("target is TARGETS\n");
				XChangeProperty( xdpy, r.requestor, r.property, XA_ATOM, 32, PropModeReplace, (unsigned char*)&GLINUX.Device.Screen._atom_UTF8_STRING, 1 );
			}else if(r.target == GLINUX.Device.Screen._atom_UTF8_STRING){
			//	printf("target is UTF8_STRING\n");
			//	printf("len:%d ptr:%s\n", GLINUX.Device.Screen._clipboard_set_bufLen, GLINUX.Device.Screen._clipboard_set_bufPtr);
				XChangeProperty( xdpy, r.requestor, r.property, GLINUX.Device.Screen._atom_UTF8_STRING, 8, PropModeReplace, (unsigned char*)GLINUX.Device.Screen._clipboard_set_bufPtr, GLINUX.Device.Screen._clipboard_set_bufLen-1 );
			}else{
			//	printf("unsupported target: %d[%s]\n", r.target,
			//		ev.xselectionrequest.target?XGetAtomName(xdpy, ev.xselectionrequest.target):"0");
				r.property = None;
			}
			XSendEvent( xdpy, r.requestor, False, NoEventMask, (XEvent*)&r );
			break;
		// マウスの状態
		case ButtonPress :
		case ButtonRelease :
		case MotionNotify :
		//case EnterNotify :
		//case LeaveNotify :
#ifndef DX_NON_INPUT
			ProcessMouseInputEvent( ev );
#endif // DX_NON_INPUT
			break ;
		// キーボードの状態
		case KeyPress :
		case KeyRelease :
		//case FocusIn :
		//case FocusOut :
		//case KeymapNotify :
		//case MappingNotify :
#ifndef DX_NON_INPUT
			ProcessKeyboardInputEvent( ev, xdpy );
#endif // DX_NON_INPUT
			break ;
		// StructureNotifyMask
		case ConfigureNotify :
			if(GLINUX.Device.Screen.Width != ev.xconfigure.width || GLINUX.Device.Screen.Height != ev.xconfigure.height) { // resize detect
				GLINUX.Device.Screen.Width = ev.xconfigure.width;
				GLINUX.Device.Screen.Height = ev.xconfigure.height;
				Graphics_RestoreOrChangeSetupGraphSystem(1, ev.xconfigure.width, ev.xconfigure.height, 0, 0);

				eglDestroySurface( GLINUX.Device.Screen.Display, GLINUX.Device.Screen.Surface ) ;
				GLINUX.Device.Screen.Surface = eglCreateWindowSurface( GLINUX.Device.Screen.Display, GLINUX.Device.Screen.Config, GLINUX.Device.Screen.XWindow, NULL ) ;
				if( eglMakeCurrent( GLINUX.Device.Screen.Display, GLINUX.Device.Screen.Surface, GLINUX.Device.Screen.Surface, GLINUX.Device.Screen.Context ) == EGL_FALSE )
				{
					DXST_LOGFILE_ADDUTF16LE( "\x65\x00\x67\x00\x6c\x00\x4d\x00\x61\x00\x6b\x00\x65\x00\x43\x00\x75\x00\x72\x00\x72\x00\x65\x00\x6e\x00\x74\x00\x20\x00\x4c\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"eglMakeCurrent が失敗しました\n" @*/ ) ;
					return -1 ;
				}
			}
			break ;
		default:
			//printf("w%d ", ev.type); fflush(stdout);
			break ;
		}
	}

	// 通常終了
	return 0 ;
}

// アプリがアクティブではない状態でも処理を続行するか、フラグをセットする
extern int NS_SetAlwaysRunFlag( int Flag )
{
	// フラグをセット
	g_LinuxSys.NonActiveRunFlag = Flag ;
	
	// 終了
	return 0 ;
}



// ソフトが非アクティブになった際に呼ばれるコールバック関数を登録する
extern int SetLinuxLostFocusCallbackFunction( void (* Callback )( void *Data ), void *CallbackData )
{
	g_LinuxSys.LostFocusCallbackFunction     = ( volatile void ( * )( void * ) )Callback ;
	g_LinuxSys.LostFocusCallbackFunctionData = ( volatile void * )CallbackData ;

	return 0 ;
}

// ソフトがアクティブになった際に呼ばれるコールバック関数を登録する
extern int SetLinuxGainedFocusCallbackFunction( void (* Callback )( void *Data ), void *CallbackData )
{
	g_LinuxSys.GainedFocusCallbackFunction     = ( volatile void ( * )( void * ) )Callback ;
	g_LinuxSys.GainedFocusCallbackFunctionData = ( volatile void * )CallbackData ;

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
			*DxScreenX = ScreenX * GLINUX.Device.Screen.SubBackBufferTextureSizeX / GLINUX.Device.Screen.Width ;
		}

		if( DxScreenY != NULL )
		{
			*DxScreenY = ScreenY * GLINUX.Device.Screen.SubBackBufferTextureSizeY / GLINUX.Device.Screen.Height ;
		}
	}
	else
	{
		DestW = GLINUX.Device.Screen.Width ;
		DestH = GLINUX.Device.Screen.Width * GLINUX.Device.Screen.SubBackBufferTextureSizeY / GLINUX.Device.Screen.SubBackBufferTextureSizeX ;
		if( DestH > ( DWORD )GLINUX.Device.Screen.Height )
		{
			DestW = GLINUX.Device.Screen.Height * GLINUX.Device.Screen.SubBackBufferTextureSizeX / GLINUX.Device.Screen.SubBackBufferTextureSizeY ;
			DestH = GLINUX.Device.Screen.Height ;
		}

		DestRect.left   = ( GLINUX.Device.Screen.Width  - DestW ) / 2 ;
		DestRect.top    = ( GLINUX.Device.Screen.Height - DestH ) / 2 ;
		DestRect.right  = DestRect.left + DestW ;
		DestRect.bottom = DestRect.top  + DestH ;

		if( DxScreenX != NULL )
		{
			*DxScreenX = ( ScreenX - DestRect.left ) * GLINUX.Device.Screen.SubBackBufferTextureSizeX / ( int )DestW ;
		}

		if( DxScreenY != NULL )
		{
			*DxScreenY = ( ScreenY - DestRect.top  ) * GLINUX.Device.Screen.SubBackBufferTextureSizeY / ( int )DestH ;
		}
	}

#endif // DX_NON_GRAPHICS

	// 終了
	return 0 ;
}

// アクティブになるまで何もしない
extern void DxActiveWait_Linux( void )
{
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

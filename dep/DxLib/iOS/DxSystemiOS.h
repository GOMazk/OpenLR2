// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用システムプログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_SYSTEMIOS_H
#define DX_SYSTEMIOS_H

// インクルード ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include <pthread.h>
#include <sched.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// ソフトがアクティブかどうかをチェックする
#define CheckActiveState()					\
		if( g_iOSSys.NonActiveRunFlag == FALSE )\
		{\
			DxActiveWait_iOS() ;\
		}

// 構造体定義 --------------------------------------------------------------------

struct DXLIB_IOS_SYSTEMINFO
{
	volatile int				NonActiveRunFlag ;					// アプリがアクティブではなくても処理を実行するかどうかのフラグ

	pthread_t					SoftThread ;						// ソフト実行用スレッド
	volatile int				SoftThreadRunning ;					// ソフト実行用スレッドの実行が開始されたかどうか( 0:開始されていない  1:開始された )
	volatile int				SoftThreadDestroyedStart ;			// ソフト実行用スレッドの終了処理を開始したかどうか( 0:開始されていない  1:開始された )
	volatile int				SoftThreadDestroyed ;				// ソフト実行用スレッドが終了したかどうか( 0:終了していない  1:終了した )
	volatile int				SoftThreadWait ;					// ソフト実行用スレッドが待ち状態かどうか( 1:待ち状態  0:実行状態 )

	volatile int				SoundAndMoviePause ;				// ウインドウが非アクティブになりサウンドと動画が一時停止しているかどうか( TRUE:一時停止している  FALSE:一時停止していない )

	volatile void				( *LostFocusCallbackFunction )( void *Data ) ;		// アプリのフォーカスが失われた際に呼ばれるコールバック関数
	volatile void				*LostFocusCallbackFunctionData ;					// アプリのフォーカスが失われた際に呼ばれるコールバック関数に渡すポインタ
	volatile void				( *GainedFocusCallbackFunction )( void *Data ) ;	// アプリのフォーカスが得られた際に呼ばれるコールバック関数
	volatile void				*GainedFocusCallbackFunctionData ;					// アプリのフォーカスが得られた際に呼ばれるコールバック関数に渡すポインタ
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

extern DXLIB_IOS_SYSTEMINFO g_iOSSys ;
extern int g_iOSRunFlag ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// アクティブになるまで何もしない
extern void DxActiveWait_iOS( void ) ;

// アプリの外部データ保存用のディレクトリパスを取得する
extern int GetDocumentsDirPathForChar( char *PathBuffer, size_t PathBufferBytes ) ;

// アプリのデータ保存用のディレクトリパスを取得する
extern int GetLibraryPreferencesDirPathForChar( char *PathBuffer, size_t PathBufferBytes ) ;

// アプリのキャッシュファイル保存用のディレクトリパスを取得する
extern int GetLibraryCachesDirPathForChar( char *PathBuffer, size_t PathBufferBytes ) ;

// アプリの一時ファイル保存用のディレクトリパスを取得する
extern int GetTmpDirPathForChar( char *PathBuffer, size_t PathBufferBytes ) ;

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

// C++ 側での初期化を行う
extern int DxSystemInitialize_CPP( void ) ;



#endif // DX_SYSTEMIOS_H

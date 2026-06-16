// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Linux用システムプログラムヘッダファイル
// 
// 				Ver 3.24f
// 
// -------------------------------------------------------------------------------

#ifndef DX_SYSTEM_LINUX_H
#define DX_SYSTEM_LINUX_H

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
		if( g_LinuxSys.NonActiveRunFlag == FALSE )\
		{\
			DxActiveWait_Linux() ;\
		}

// 構造体定義 --------------------------------------------------------------------

struct DXLIB_LINUX_SYSTEMINFO
{

	volatile int				MutexLockIndex ;
	pthread_mutex_t				Mutex ;								// メインスレッドとソフト実行用スレッドでお互いに使用する変数アクセス時等に使用するミューテックス
	pthread_cond_t				Cond ;								// メインスレッドとソフト実行用スレッドでお互いの処理の実行完了待ちなどに使用する条件変数

	volatile int				NonActiveRunFlag ;					// アプリがアクティブではなくても処理を実行するかどうかのフラグ
	volatile int				IsFullScreen ;						// フルスクリーンかどうか( TRUE:フルスクリーン  FALSE:タイトルバーとナビゲーションバーあり )

	volatile int				MessageRead ;						// メインスレッドからソフト実行用スレッドへのメッセージ受け取り用ファイルディスクリプタ
	volatile int				MessageWrite ;						// メインスレッドからソフト実行用スレッドへのメッセージ書き込み用ファイルディスクリプタ

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

extern DXLIB_LINUX_SYSTEMINFO g_LinuxSys ;
extern int g_LinuxRunFlag ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// アクティブになるまで何もしない
extern void DxActiveWait_Linux( void ) ;

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_SYSTEM_LINUX_H

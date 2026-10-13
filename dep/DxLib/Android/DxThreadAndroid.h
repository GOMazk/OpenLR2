// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用スレッド関係プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_THREADANDROID_H
#define DX_THREADANDROID_H

// インクルード ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include <pthread.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// クリティカルセクション
struct DX_CRITICAL_SECTION
{
	int						Mutex_valid ;								// Mutex が有効かどうか
	pthread_mutex_t			Mutex ;										// 休止状態処理用のミューテックス
	int						Mutexaddr_valid ;							// Mutexaddr が有効かどうか
	pthread_mutexattr_t		Mutexattr ;									// 休止状態処理用のミューテックスの属性情報
#if defined( _DEBUG )
	char					FilePath[ 512 ] ;
	int						LineNo ;
	DWORD_PTR				ThreadID ;
#endif
} ;

// 一つのスレッドの情報
struct THREAD_INFO
{
	void					( *pFunction )( THREAD_INFO *, void * ) ;	// 実行する関数
	void					*pParam ;									// 実行する関数に渡すパラメータ
	int						Thread_valid ;								// Thread が有効かどうか
	pthread_t				Thread ;									// スレッド
	int						Threadattr_valid ;							// Threadattr が有効かどうか
	pthread_attr_t			Threadattr ;								// スレッドの属性情報
	int						Cond_valid ;								// Cond が有効かどうか
	pthread_cond_t			Cond ;										// 休止状態処理用の条件変数
	int						Mutex_valid ;								// Mutex が有効かどうか
	pthread_mutex_t			Mutex ;										// 休止状態処理用のミューテックス
	int						Mutexaddr_valid ;							// Mutexaddr が有効かどうか
	pthread_mutexattr_t		Mutexattr ;									// 休止状態処理用のミューテックスの属性情報
	int						SuspendFlag ;								// 休止状態かどうかのフラグ
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_THREADANDROID_H

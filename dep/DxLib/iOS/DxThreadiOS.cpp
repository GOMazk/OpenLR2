//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用スレッド関係プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

// インクルード ---------------------------------------------------------------
#include "DxThreadiOS.h"
#include "../DxMemory.h"
#include "../DxBaseFunc.h"
#include <unistd.h>
#include <sched.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

#define THREAD_STACK_SIZE				(128 * 1024)			// スレッドのスタックサイズ

#define THREAD_SUSPEND_EVENT_BIT		(0x00000001)			// 休止状態処理用のイベントのビット

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

// スレッドの処理を初期化する
extern void Thread_Initialize( void )
{
	return ;
}

// スレッドを実行する関数
static void * ThreadRunFunction( void *pArgBlock )
{
	THREAD_INFO *pInfo = ( THREAD_INFO * )pArgBlock ;

	Thread_Suspend( pInfo ) ;

	// スレッド作成時に渡された関数を実行
	pInfo->pFunction( pInfo, pInfo->pParam ) ;

	return NULL ;
}

// スレッドを作成する
extern int Thread_Create( THREAD_INFO *pThreadInfo, void ( *pFunction )( THREAD_INFO *, void * ), void *pParam )
{
	// 値を初期化
	pThreadInfo->Thread_valid		= 0 ;
	pThreadInfo->Threadattr_valid	= 0 ;
	pThreadInfo->Cond_valid			= 0 ;
	pThreadInfo->Mutex_valid		= 0 ;
	pThreadInfo->Mutexaddr_valid	= 0 ;
	pThreadInfo->SuspendFlag		= FALSE ;

	// パラメータの保存
	pThreadInfo->pFunction	= pFunction ;
	pThreadInfo->pParam		= pParam ;

	// ミューテックス属性の初期化
	if( pthread_mutexattr_init( &pThreadInfo->Mutexattr ) != 0 )
	{
		goto ERR ;
	}
	pThreadInfo->Mutexaddr_valid = 1 ;

	// 再帰ロック可能にする
	if( pthread_mutexattr_settype( &pThreadInfo->Mutexattr, PTHREAD_MUTEX_RECURSIVE ) != 0 )
	{
		goto ERR ;
	}

	// ミューテックスの作成
	if( pthread_mutex_init( &pThreadInfo->Mutex, &pThreadInfo->Mutexattr ) != 0 )
	{
		goto ERR ;
	}
	pThreadInfo->Mutex_valid = 1 ;

	// 条件変数の作成
	if( pthread_cond_init( &pThreadInfo->Cond, NULL ) != 0 )
	{
		goto ERR ;
	}
	pThreadInfo->Cond_valid = 1 ;

	// スレッド属性の初期化
	if( pthread_attr_init( &pThreadInfo->Threadattr ) != 0 )
	{
		goto ERR ;
	}
	pThreadInfo->Threadattr_valid = 1 ;

	// スレッド属性のセット
//	if( pthread_attr_setdetachstate( &pThreadInfo->Threadattr, PTHREAD_CREATE_DETACHED ) != 0 )
//	{
//		goto ERR ;
//	}

	// スタックサイズのセット
	if( pthread_attr_setstacksize( &pThreadInfo->Threadattr, THREAD_STACK_SIZE ) != 0 )
	{
		goto ERR ;
	}

	// スレッドの作成
	if( pthread_create( &pThreadInfo->Thread, &pThreadInfo->Threadattr, ThreadRunFunction, pThreadInfo ) != 0 )
	{
		goto ERR ;
	}
	pThreadInfo->Thread_valid = 1 ;

	// 正常終了
	return 0 ;

ERR :

	if( pThreadInfo->Mutex_valid )
	{
		pthread_mutex_destroy( &pThreadInfo->Mutex ) ;
		pThreadInfo->Mutex_valid = 0 ;
	}

	if( pThreadInfo->Mutexaddr_valid )
	{
		pthread_mutexattr_destroy( &pThreadInfo->Mutexattr ) ;
		pThreadInfo->Mutexaddr_valid = 0 ;
	}

	if( pThreadInfo->Cond_valid )
	{
		pthread_cond_destroy( &pThreadInfo->Cond ) ;
		pThreadInfo->Cond_valid = 0 ;
	}

	if( pThreadInfo->Threadattr_valid )
	{
		pthread_attr_destroy( &pThreadInfo->Threadattr ) ;
		pThreadInfo->Threadattr_valid = 0 ;
	}

	if( pThreadInfo->Thread_valid )
	{
		pthread_detach( pThreadInfo->Thread ) ;
		pThreadInfo->Thread_valid = 0 ;
	}

	// エラー終了
	return -1 ;
}

// スレッドの後始末を行う
extern void Thread_Delete( THREAD_INFO *pThreadInfo )
{
	if( pThreadInfo->Mutex_valid )
	{
		pthread_mutex_destroy( &pThreadInfo->Mutex ) ;
		pThreadInfo->Mutex_valid = 0 ;
	}

	if( pThreadInfo->Mutexaddr_valid )
	{
		pthread_mutexattr_destroy( &pThreadInfo->Mutexattr ) ;
		pThreadInfo->Mutexaddr_valid = 0 ;
	}

	if( pThreadInfo->Cond_valid )
	{
		pthread_cond_destroy( &pThreadInfo->Cond ) ;
		pThreadInfo->Cond_valid = 0 ;
	}

	if( pThreadInfo->Threadattr_valid )
	{
		pthread_attr_destroy( &pThreadInfo->Threadattr ) ;
		pThreadInfo->Threadattr_valid = 0 ;
	}

	if( pThreadInfo->Thread_valid )
	{
		pthread_detach( pThreadInfo->Thread ) ;
		pThreadInfo->Thread_valid = 0 ;
	}
}

// スレッドが有効かどうかを取得する( 1:有効  0:無効 )
extern int Thread_IsValid( THREAD_INFO *pThreadInfo )
{
	return pThreadInfo->Thread_valid ;
}


// スレッドの実行優先順位を設定する
extern void Thread_SetPriority( THREAD_INFO *pThreadInfo, int Priority /* DX_THREAD_PRIORITY_LOWEST など */ )
{
	sched_param param ;
	volatile static int InitPriorityTable = 0 ;
	volatile static int PriorityTable[ DX_THREAD_PRIORITY_NUM ] ;

	if( Priority < 0 || Priority >= DX_THREAD_PRIORITY_NUM )
	{
		return ;
	}

	if( InitPriorityTable == 0 )
	{
		int MinPrio ;
		int MaxPrio ;

		MaxPrio = sched_get_priority_max( 0 /* SCHED_NORMAL */ ) ;
		MinPrio = sched_get_priority_min( 0 /* SCHED_NORMAL */ ) ;

		PriorityTable[ DX_THREAD_PRIORITY_LOWEST       ] = ( MaxPrio - MinPrio ) * 0 / 3 + MinPrio ;
		PriorityTable[ DX_THREAD_PRIORITY_BELOW_NORMAL ] = ( MaxPrio - MinPrio ) * 1 / 3 + MinPrio ;
		PriorityTable[ DX_THREAD_PRIORITY_NORMAL       ] = ( MaxPrio - MinPrio ) * 2 / 3 + MinPrio ;
		PriorityTable[ DX_THREAD_PRIORITY_HIGHEST      ] = ( MaxPrio - MinPrio ) * 3 / 3 + MinPrio ;

		InitPriorityTable = 1 ;
	}

	_MEMSET( &param, 0, sizeof( param ) ) ;
	param.sched_priority = PriorityTable[ Priority ] ;
	pthread_setschedparam( pThreadInfo->Thread, 0 /* SCHED_NORMAL */, &param ) ;
}

// カレントスレッドのＩＤを取得する
extern DWORD_PTR Thread_GetCurrentId( void )
{
	return ( DWORD_PTR )pthread_self() ;
}

// スレッドのＩＤを取得する
extern DWORD_PTR Thread_GetId( THREAD_INFO *pThreadInfo )
{
	return ( DWORD_PTR )pThreadInfo->Thread ;
}

// スレッドを休止状態にする
extern void Thread_Suspend( THREAD_INFO *pThreadInfo )
{
	// ミューテックスをロック
	pthread_mutex_lock( &pThreadInfo->Mutex ) ;

	// 休止状態のフラグを立てる
	pThreadInfo->SuspendFlag = TRUE ;

	// シグナル待ちにする
	pthread_cond_wait( &pThreadInfo->Cond, &pThreadInfo->Mutex ) ;

	// ミューテックスをアンロック
	pthread_mutex_unlock( &pThreadInfo->Mutex ) ;
}

// スレッドの休止状態を解除する( 0:休止状態じゃなかった  1:休止状態だった )
extern int Thread_Resume( THREAD_INFO *pThreadInfo )
{
	// ミューテックスをロック
	pthread_mutex_lock( &pThreadInfo->Mutex ) ;

	// 休止状態ではなかったらすぐに抜ける
	if( pThreadInfo->SuspendFlag == FALSE )
	{
		// ミューテックスをアンロック
		pthread_mutex_unlock( &pThreadInfo->Mutex ) ;

		// 休止状態ではなかったの戻り値
		return 0 ;
	}

	// 休止状態のフラグを倒す
	pThreadInfo->SuspendFlag = FALSE ;

	// シグナルを送る
	pthread_cond_signal( &pThreadInfo->Cond ) ; 

	// ミューテックスをアンロック
	pthread_mutex_unlock( &pThreadInfo->Mutex ) ;

	// 休止状態だったの戻り値
	return 1 ;
}

// 指定時間スレッドを停止する
extern void Thread_Sleep( DWORD MiliSecond )
{
	usleep( MiliSecond * 1000 ) ;
}





// クリティカルセクションの初期化
extern int CriticalSection_Initialize( DX_CRITICAL_SECTION *pCSection )
{
	pCSection->Mutex_valid		= 0 ;
	pCSection->Mutexaddr_valid	= 0 ;

	// ミューテックス属性の初期化
	if( pthread_mutexattr_init( &pCSection->Mutexattr ) != 0 )
	{
		goto ERR ;
	}
	pCSection->Mutexaddr_valid = 1 ;

	// 再帰ロック可能にする
	if( pthread_mutexattr_settype( &pCSection->Mutexattr, PTHREAD_MUTEX_RECURSIVE ) != 0 )
	{
		goto ERR ;
	}

	// ミューテックスの作成
	if( pthread_mutex_init( &pCSection->Mutex, &pCSection->Mutexattr ) != 0 )
	{
		goto ERR ;
	}
	pCSection->Mutex_valid = 1 ;

#if defined( _DEBUG )
	int i ;
	for( i = 0 ; i < 256 ; i ++ )
	{
		pCSection->FilePath[ 0 ] = '\0' ;
	}
#endif

	return 0 ;

ERR :

	if( pCSection->Mutex_valid )
	{
		pthread_mutex_destroy( &pCSection->Mutex ) ;
		pCSection->Mutex_valid = 0 ;
	}

	if( pCSection->Mutexaddr_valid )
	{
		pthread_mutexattr_destroy( &pCSection->Mutexattr ) ;
		pCSection->Mutexaddr_valid = 0 ;
	}

	return -1 ;
}

// クリティカルセクションの削除
extern int CriticalSection_Delete( DX_CRITICAL_SECTION *pCSection )
{
	if( pCSection->Mutex_valid )
	{
		pthread_mutex_destroy( &pCSection->Mutex ) ;
		pCSection->Mutex_valid = 0 ;
	}

	if( pCSection->Mutexaddr_valid )
	{
		pthread_mutexattr_destroy( &pCSection->Mutexattr ) ;
		pCSection->Mutexaddr_valid = 0 ;
	}

	return 0 ;
}

// クリティカルセクションのロックの取得
//#if defined( _DEBUG )
#if 1
extern int CriticalSection_Lock( DX_CRITICAL_SECTION *pCSection, const char *FilePath, int LineNo )
#else
extern int CriticalSection_Lock( DX_CRITICAL_SECTION *pCSection )
#endif
{
#if defined( _DEBUG )
	DWORD_PTR ThreadID = Thread_GetCurrentId() ;
#endif

	// ミューテックスをロック
	pthread_mutex_lock( &pCSection->Mutex ) ;

#if defined( _DEBUG )
	int Length = ( int )_STRLEN( FilePath ) ;
	if( Length >= 512 ) Length = 511 ;
	_MEMCPY( pCSection->FilePath, FilePath, ( size_t )Length ) ;
	pCSection->FilePath[ Length ] = '\0' ;
	pCSection->LineNo = LineNo ;
	pCSection->ThreadID = ThreadID ;
#endif

	return 0 ;
}

// クリティカルセクションのロックを解放する
extern int CriticalSection_Unlock( DX_CRITICAL_SECTION *pCSection )
{
	// ミューテックスを解除
	pthread_mutex_unlock( &pCSection->Mutex ) ;

	return 0 ;
}




#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


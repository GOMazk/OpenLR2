//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用メモリ関係プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

// インクルード ---------------------------------------------------------------
#include "../DxMemory.h"
#include "../DxLog.h"
#include "../DxBaseFunc.h"
#include <stdlib.h>
#include <new>
#include <cstdlib>
#include <cerrno>

#ifndef DX_NON_NAMESPACE

using namespace DxLib ;

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

// メモリ処理の初期化を行う関数の環境依存処理
extern int MemoryInitialize_PF( void )
{
	return 0 ;
}

// メモリ処理の後始末を行う関数の環境依存処理
extern int MemoryTerminate_PF( void )
{
	return 0 ;
}

// メモリ処理の周期的処理の環境依存処理
extern int MemoryProcess_PF( void )
{
	return 0 ;
}

// メモリのエラーチェックの環境依存処理
extern int MemoryErrorCheck_PF( void )
{
	return 0 ;
}

// Android のヒープ用のメモリ領域を確保するためのコールバック関数群の AutoAlloc_CreateHeapCallback
extern int NormalMemory_AutoAlloc_CreateHeapCallback_PF( int Param, void *Buffer, ALLOCMEM_SIZE_TYPE Size )
{
	PLATFORMHEAP_ANDR *			PlatformHeap = ( PLATFORMHEAP_ANDR * )Buffer ;

	// バッファサイズチェック
	if( sizeof( PLATFORMHEAP_ANDR ) > ALLOCHEAP_CALLBACK_INFO_SIZE )
	{
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// メモリを確保
	PlatformHeap->AllocAddress = malloc( Size ) ;
	if( PlatformHeap->AllocAddress == NULL )
	{
		return -1 ;
	}
	PlatformHeap->AllocSize = Size ;

	return 0 ;
}

// 環境依存の一般的なヒープ用のメモリ領域を確保するためのコールバック関数群の AutoAlloc_GetHeapAddressCallback
extern void * NormalMemory_AutoAlloc_GetHeapAddressCallback_PF( int Param, void *Buffer )
{
	PLATFORMHEAP_ANDR *			PlatformHeap = ( PLATFORMHEAP_ANDR * )Buffer ;

	return PlatformHeap->AllocAddress ;
}

// 環境依存の一般的なヒープ用のメモリ領域を確保するためのコールバック関数群の AutoAlloc_GetHeapSizeCallback
extern ALLOCMEM_SIZE_TYPE NormalMemory_AutoAlloc_GetHeapSizeCallback_PF( int Param, void *Buffer )
{
	PLATFORMHEAP_ANDR *			PlatformHeap = ( PLATFORMHEAP_ANDR * )Buffer ;

	return ( ALLOCMEM_SIZE_TYPE )PlatformHeap->AllocSize ;
}

// 環境依存の一般的なヒープ用のメモリ領域を確保するためのコールバック関数群の AutoAlloc_DeleteHeapCallback
extern int NormalMemory_AutoAlloc_DeleteHeapCallback_PF( int Param, void *Buffer )
{
	PLATFORMHEAP_ANDR *			PlatformHeap = ( PLATFORMHEAP_ANDR * )Buffer ;

	// メモリを解放
	free( PlatformHeap->AllocAddress ) ;

	return 0 ;
}

// 確保しているメモリを列挙する関数の環境依存処理
extern int DxDumpAlloc_PF( void )
{
	return 0 ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


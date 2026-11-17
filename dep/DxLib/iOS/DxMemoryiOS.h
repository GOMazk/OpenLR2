// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用メモリ関係プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_MEMORYIOS_H
#define DX_MEMORYIOS_H

// インクルード ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include "../DxHeap.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define DXMEMORY_SMALLHEAP_UNITSIZE		( 4 * 1024 * 1024)			// 小さいメモリ確保用のヒープの単位確保サイズ
#define DXMEMORY_BIGHEAP_UNITSIZE		(32 * 1024 * 1024)			// 大きいメモリ確保用のヒープの単位確保サイズ
#define DXMEMORY_SMALL_MAX_SIZE			(64 * 1024)					// 小さいメモリ確保用のヒープを使用する最大サイズ

// 構造体定義 --------------------------------------------------------------------

#ifndef DX_NON_NAMESPACE
 
}

#endif // DX_NON_NAMESPACE

// iOS用ヒープの情報
struct PLATFORMHEAP_IOS
{
	void *					AllocAddress ;						// 確保したメモリ領域のアドレス
	size_t					AllocSize ;							// 確保したメモリ領域のサイズ
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE
 
namespace DxLib
{

#endif // DX_NON_NAMESPACE


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_MEMORYIOS_H

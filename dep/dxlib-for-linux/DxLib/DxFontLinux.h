// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Linux用フォント関係プログラムヘッダファイル
// 
// 				Ver 3.24f
// 
// -------------------------------------------------------------------------------

#ifndef DX_FONT_LINUX_H
#define DX_FONT_LINUX_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_FONT

// インクルード ------------------------------------------------------------------
#include "../DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define FSYSLINUX			FontSystemLinux

// 構造体定義 --------------------------------------------------------------------

// Linux用 フォントキャッシュの環境依存管理データ
struct FONTMANAGE_PF
{
	void *					FontData ;								// Linux用フォントデータ
} ;

// Linux用 フォントシステム用構造体
struct FONTSYSTEM_LINUX
{
	int						Dummy ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern FONTSYSTEM_LINUX FontSystemLinux ;

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FONT

#endif // DX_FONT_LINUX_H

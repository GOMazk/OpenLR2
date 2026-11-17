// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用フォント関係プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_FONT_IOS_H
#define DX_FONT_IOS_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_FONT

// インクルード ------------------------------------------------------------------
#include "../DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define FSYSIOS			FontSystemiOS

// 構造体定義 --------------------------------------------------------------------

// iOS用 フォントキャッシュの環境依存管理データ
struct FONTMANAGE_PF
{
	void *					FontData ;								// iOS用フォントデータ
} ;

// iOS用 フォントシステム用構造体
struct FONTSYSTEM_IOS
{
	int						Dummy ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern FONTSYSTEM_IOS FontSystemiOS ;

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FONT

#endif // DX_FONT_IOS_H

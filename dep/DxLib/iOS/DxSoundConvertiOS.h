// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用サウンドデータ変換プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_SOUNDCONVERTIOS_H
#define DX_SOUNDCONVERTIOS_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_SOUND

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxSoundConvert.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// サウンド変換処理全体で使用するデータ構造体で IOS に依存している情報の構造体
struct SOUNDCONVERTDATA_IOS
{
	int							Dummy ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_SOUND

#endif // DX_SOUNDCONVERTIOS_H

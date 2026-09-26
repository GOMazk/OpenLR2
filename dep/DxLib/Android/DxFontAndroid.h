// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用フォント関係プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_FONTANDROID_H
#define DX_FONTANDROID_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_FONT

// インクルード ------------------------------------------------------------------
#include "../DxFile.h"
#include <jni.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define FSYSANDR			FontSystemAndroid

// 構造体定義 --------------------------------------------------------------------

// Android用 フォントキャッシュの環境依存管理データ
struct FONTMANAGE_PF
{
	int						ReferenceNumUp ;						// Javaオブジェクトの参照数を増やしているか

	jobject					object_Bitmap ;							// 文字の出力結果を一時的に保存する Bitmap
	jobject					object_Canvas ;							// 文字の出力を行うための Canvas
	jobject					object_Paint ;							// 文字の出力を行うための Paint
	jobject					object_Typeface ;						// 文字のフォントタイプなどを指定するための Typeface
	jobject					object_OldTypeface ;					// Paint に最初から設定されていた Typeface

	jint					BitmapSizeX ;							// 文字の出力結果を一時的に保存する Bitmap のサイズX
	jint					BitmapSizeY ;							// 文字の出力結果を一時的に保存する Bitmap のサイズY
	jintArray				intArray_ClearBitmapBuffer ;			// 文字の出力結果の Bitmap をクリアするための int配列
	jintArray				intArray_BitmapBuffer ;					// 文字の出力結果を取得するための int配列
	void					*ImageBuffer ;							// 文字の出力結果をテクスチャに転送する形にしたものを保存するためのバッファ

	float					PaintFontMetrics_ascent ;				// 使用するフォントの ascent
	float					PaintFontMetrics_bottom ;				// 使用するフォントの bottom
	float					PaintFontMetrics_descent ;				// 使用するフォントの descent
	float					PaintFontMetrics_leading ;				// 使用するフォントの leading
	float					PaintFontMetrics_top ;					// 使用するフォントの top
} ;

// Android用 フォントシステム用構造体
struct FONTSYSTEM_ANDR
{
	int						Dummy ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern FONTSYSTEM_ANDR FontSystemAndroid ;

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FONT

#endif // DX_FONTANDROID_H

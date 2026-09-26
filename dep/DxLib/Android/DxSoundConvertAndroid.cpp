//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用サウンドデータ変換プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_SOUND

// インクルード----------------------------------------------------------------
#include "DxSoundConvertAndroid.h"
#include "../DxSoundConvert.h"
#include "../DxSystem.h"
#include "../DxHeap.h"
#include "../DxMemory.h"
#include "../DxLog.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// 型定義----------------------------------------------------------------------

// データ宣言------------------------------------------------------------------

SOUNDCONVERTDATA_ANDR GSoundConvertData_Android ;

// 関数プロトタイプ宣言 -------------------------------------------------------

// プログラム------------------------------------------------------------------

// 初期化・終了関数

// サウンドデータ変換処理の環境依存の初期化を行う
extern	int InitializeSoundConvert_PF( void )
{
	// 正常終了
	return 0 ;
}

// サウンドデータ変換処理の環境依存の終了処理を行う
extern	int TerminateSoundConvert_PF( void )
{
	// 正常終了
	return 0 ;
}









// (環境依存処理)変換処理のセットアップ( [戻] -1:エラー )
extern	int SetupSoundConvert_PF( SOUNDCONV *SoundConv, STREAMDATA *Stream, int DisableReadSoundFunctionMask )
{
	return -1 ;
}

// (環境依存処理)変換処理の位置を変更する( サンプル単位 )
extern	int SetSampleTimeSoundConvert_PF(    SOUNDCONV *SoundConv, LONGLONG SampleTime )
{
	int res = -1 ;

	return res ;
}

// (環境依存処理)変換後のバッファにデータを補充する
extern	int ConvertProcessSoundConvert_PF(  SOUNDCONV *SoundConv )
{
	int res = -1 ;

	return res ;
}

// (環境依存処理)変換処理の後始末を行う
extern	int TerminateSoundConvert_PF(        SOUNDCONV *SoundConv )
{
	return 0 ;
}

// (環境依存処理)変換後の大凡のデータサイズを得る
extern	LONGLONG GetSoundConvertDestSize_Fast_PF( SOUNDCONV *SoundConv )
{
	return 0 ;
}





















#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_SOUND


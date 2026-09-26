//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用動画プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_MOVIE

// インクルード----------------------------------------------------------------
#include "DxMovieiOS.h"
#include "DxFileiOS.h"
#include "DxMemoryiOS.h"
#include "../DxLog.h"
#include "../DxMovie.h"
#include "../DxSystem.h"
#include "../DxMemory.h"
#include "../DxBaseFunc.h"
// #include <videodec.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// 型定義----------------------------------------------------------------------

// データ宣言------------------------------------------------------------------

// 関数プロトタイプ宣言 -------------------------------------------------------

// プログラム------------------------------------------------------------------

// ムービー関連の管理処理の初期化の環境依存処理
extern int InitializeMovieManage_PF( void )
{
	return 0 ;
}

// ムービー関連の管理処理の後始末の環境依存処理
extern int TerminateMovieManage_PF( void )
{
	return 0 ;
}

// OpenMovie のグローバル変数にアクセスしないバージョンの環境依存処理
extern int OpenMovie_UseGParam_PF( MOVIEGRAPH * Movie, OPENMOVIE_GPARAM *GParam, const wchar_t *FileName, int *Width, int *Height, int SurfaceMode, int ImageSizeGetOnly, int ASyncThread )
{
	return -1 ;
}

// ムービー用のグラフィックハンドル作成用のパラメータをカスタマイズする
extern int OpenMovie_CustomGraphHandleGParam_PF( MOVIEGRAPH * Movie, struct SETUP_GRAPHHANDLE_GPARAM *InitGraphHandleGParam )
{
	return 0 ;
}

// ムービーハンドルの後始末を行う
extern int TerminateMovieHandle_PF( HANDLEINFO *HandleInfo )
{
	// 正常終了
	return 0 ;
}

// ムービーの再生を開始する処理の環境依存処理
extern int PlayMovie__PF( MOVIEGRAPH * Movie, int PlayType, int SysPlay )
{
	// 正常終了
	return 0 ;
}

// ムービーの再生をストップする処理の環境依存処理
extern 	int PauseMovie_PF( MOVIEGRAPH * Movie, int SysPause )
{
	// 正常終了
	return 0 ;
}

// ムービーの再生位置を設定する(ミリ秒単位)処理の環境依存処理
extern int SeekMovie_PF( MOVIEGRAPH * Movie, int Time )
{
	// 正常終了
	return 0 ;
}

// ムービーの再生速度を設定する( 1.0 = 等倍速  2.0 = ２倍速 )処理の環境依存処理
extern int SetPlaySpeedRateMovie_PF( MOVIEGRAPH * Movie, double SpeedRate )
{
	// 正常終了
	return 0 ;
}

// ムービーの再生状態を得る処理の環境依存処理
extern int GetMovieState_PF( MOVIEGRAPH * Movie )
{
	return Movie->PlayFlag ;
}

// ムービーのボリュームをセットする(0～10000)処理の環境依存処理
extern int SetMovieVolume_PF( MOVIEGRAPH * Movie, int Volume )
{
	// 正常終了
	return 0 ;
}

// ムービーの基本イメージデータを取得する処理の環境依存処理
extern	BASEIMAGE *GetMovieBaseImage_PF( MOVIEGRAPH * Movie, int *ImageUpdateFlag, int ImageUpdateFlagSetOnly )
{
	return NULL ;
}

// ムービーの総フレーム数を得る
extern int GetMovieTotalFrame_PF( MOVIEGRAPH * Movie )
{
	return -1 ;
}

// ムービーの再生位置を取得する(ミリ秒単位)処理の環境依存処理
extern int TellMovie_PF( MOVIEGRAPH * Movie )
{
//	return Movie->PF.PlayTime / 1000 ;
	return 0 ;
}

// ムービーの再生位置を取得する(フレーム単位)処理の環境依存処理
extern int TellMovieToFrame_PF( MOVIEGRAPH * Movie )
{
//	return Movie->PF.FrameCount < 0 ? 0 : Movie->PF.FrameCount ;
	return 0 ;
}

// ムービーの再生位置を設定する(フレーム単位)処理の環境依存処理
extern int SeekMovieToFrame_PF( MOVIEGRAPH * Movie, int Frame )
{
	// 正常終了
	return 0 ;
}

// ムービーの１フレームあたりの時間を得る処理の環境依存処理
extern LONGLONG GetOneFrameTimeMovie_PF( MOVIEGRAPH * Movie )
{
	return ( LONGLONG )( 1000000.0f / 29.970f ) ;
}





// ムービーの更新を行う処理の環境依存処理
extern int UpdateMovie_PF( MOVIEGRAPH * Movie, int AlwaysFlag )
{
	// 正常終了
	return 0 ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MOVIE


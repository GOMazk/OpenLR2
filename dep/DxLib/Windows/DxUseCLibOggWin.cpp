//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Windows用Ogg関係プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_NAMESPACE
#ifdef DX_NON_USING_NAMESPACE_DXLIB

using namespace DxLib ;

#endif // DX_NON_USING_NAMESPACE_DXLIB
#endif // DX_NON_NAMESPACE

#if !defined( DX_NON_OGGVORBIS ) && !defined( DX_NON_OGGTHEORA )

// インクルード----------------------------------------------------------------
#include "../DxLib.h"
#include "../DxStatic.h"
#include "../DxUseCLib.h"
#include "../DxLog.h"
#include "DxGraphicsWin.h"
#include "DxUseCLibOggWin.h"
#include "DxUseCLibOggD3D9.h"
#include "DxUseCLibOggD3D11.h"

//namespace DxLib
//{

// マクロ定義------------------------------------------------------------------

// 型定義----------------------------------------------------------------------

// データ宣言------------------------------------------------------------------


// 関数プロトタイプ宣言 -------------------------------------------------------

static DWORD WINAPI	TheoraDecode_Thread( LPVOID Data ) ;

// プログラム------------------------------------------------------------------

// 環境依存関数

// ハードウエア機能が有効な場合は YUV フォーマットの一時保存用テクスチャを作成する
extern	int	TheoraDecode_CreateSurface_PF( DECODE_THEORA *DT )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return TheoraDecode_D3D11_CreateSurface_PF( DT ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return TheoraDecode_D3D9_CreateSurface_PF( DT ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	void	TheoraDecode_ReleaseSurface_PF(	DECODE_THEORA *DT )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		TheoraDecode_D3D11_ReleaseSurface_PF( DT ) ;
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		TheoraDecode_D3D9_ReleaseSurface_PF( DT ) ;
		break ;
#endif // DX_NON_DIRECT3D9

	default :
		break ;
	}
}

// カレントフレームのRGB画像を作成する( 戻り値  1:作成された  0:されなかった )
extern	int		TheoraDecode_SetupImage_PF( DECODE_THEORA *DT, volatile THEORA_STOCKFRAME *Stock, int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return TheoraDecode_D3D11_SetupImage_PF( DT, Stock, ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return TheoraDecode_D3D9_SetupImage_PF( DT, Stock, ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// 一時バッファの YUV フォーマットのテクスチャを得る
extern	const void *TheoraDecode_GetYUVImage_PF( DECODE_THEORA *DT )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return TheoraDecode_D3D11_GetYUVImage_PF( DT ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return TheoraDecode_D3D9_GetYUVImage_PF( DT ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// デコードスレッド
/*
DWORD WINAPI TheoraDecode_Thread( LPVOID Data )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Data ;

	while( TheoraDecode_Thread_LoopProcess( DT ) != 2 ){}

	// スレッド終了
	WinAPIData.Win32Func.ExitThreadFunc( 0 ) ;

	// 終了
	return 0 ;
}
*/
void TheoraDecode_Thread( THREAD_INFO * /*pThreadInfo*/, void *ASyncLoadThreadData )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )ASyncLoadThreadData ;

	while( TheoraDecode_Thread_LoopProcess( DT ) != 2 ){}
}

// Ogg Theora の読み込み処理の準備を行う処理の環境依存処理を行う関数
extern int TheoraDecode_InitializeStream_PF( DECODE_THEORA *DT )
{
	// デコード処理スレッドの作成
	DT->ThreadStopRequest = 1 ;
	DT->ThreadState = THEORAT_STATE_IDLE ;
	DT->ThreadStandbyTime = NS_GetNowCount( FALSE ) ;
//	DT->DecodeThreadHandle = WinAPIData.Win32Func.CreateThreadFunc( NULL, 0, TheoraDecode_Thread, DT, 0, ( LPDWORD )&DT->DecodeThreadID ) ;
//	if( DT->DecodeThreadHandle == NULL )
//	{
//		DXST_LOGFILE_ADDA( "Theora \x83\x80\x81\x5b\x83\x72\x81\x5b\x83\x66\x83\x52\x81\x5b\x83\x68\x97\x70\x83\x58\x83\x8c\x83\x62\x83\x68\x82\xcc\x8d\xec\x90\xac\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora ムービーデコード用スレッドの作成に失敗しました\n" @*/ );
//		return -1 ;
//	}
	if( Thread_Create( &DT->DecodeThreadInfo, TheoraDecode_Thread, DT ) == -1 )
	{
		DXST_LOGFILE_ADDA( "Theora \x83\x80\x81\x5b\x83\x72\x81\x5b\x83\x66\x83\x52\x81\x5b\x83\x68\x97\x70\x83\x58\x83\x8c\x83\x62\x83\x68\x82\xcc\x8d\xec\x90\xac\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora ムービーデコード用スレッドの作成に失敗しました\n" @*/ );
		return -1 ;
	}
//	Thread_SetPriority( &DT->DecodeThreadInfo, DX_THREAD_PRIORITY_NORMAL ) ;
	while( Thread_Resume( &DT->DecodeThreadInfo ) != 1 )
	{
		Thread_Sleep( 0 ) ;
	}

	return 0 ;
}



//}

#endif // #if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )



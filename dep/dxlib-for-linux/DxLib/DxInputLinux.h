// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Linux用入力情報プログラムヘッダファイル
// 
// 				Ver 3.24f
// 
// -------------------------------------------------------------------------------

#ifndef DX_INPUTLINUX_H
#define DX_INPUTLINUX_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_INPUT

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// 振動の環境依存情報
struct INPUTVIBRATIONDATA_PF
{
	int						Dummy ;
} ;

// ゲームパッドの環境依存情報
struct INPUTPADDATA_PF
{
	int						Dummy ;
} ;

// 一つの入力デバイスの入力情報
struct INPUT_LINUX_DEVICE_INFO
{
	int						Dummy ;
} ;

// 入力システム用環境依存データ構造体型
struct INPUTSYSTEMDATA_PF
{
	int						Dummy ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

extern int ProcessMouseInputEvent( XEvent ev );
extern int ProcessKeyboardInputEvent( XEvent ev, Display *dpy );

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_INPUT

#endif // DX_INPUTLINUX_H

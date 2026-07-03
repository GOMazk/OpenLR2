//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Linux用入力情報プログラム
// 
//  	Ver 3.24f
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_INPUT

// インクルード----------------------------------------------------------------
#include "DxInputLinux.h"
#include "DxSystemLinux.h"
#include "../DxLog.h"
#include "../DxInput.h"
#include "../DxSystem.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// 型定義----------------------------------------------------------------------

// 定数定義 ----------------------------------------------------------------------

// データ宣言------------------------------------------------------------------

// 関数プロトタイプ宣言 -------------------------------------------------------

const unsigned char LinuxKeySymToDXInputKey(KeySym k0, KeySym k1)
{
	switch(k0)
	{
	case XK_BackSpace         : return KEY_INPUT_BACK ;        // BackSpaceキー
	case XK_Tab               : return KEY_INPUT_TAB ;         // Tabキー
	case XK_Return            : return KEY_INPUT_RETURN ;      // Enterキー

	case XK_Shift_L           : return KEY_INPUT_LSHIFT ;      // 左Shiftキー
	case XK_Shift_R           : return KEY_INPUT_RSHIFT ;      // 右Shiftキー
	case XK_Control_L         : return KEY_INPUT_LCONTROL ;    // 左Ctrlキー
	case XK_Control_R         : return KEY_INPUT_RCONTROL ;    // 右Ctrlキー
	case XK_Escape            : return KEY_INPUT_ESCAPE ;      // Escキー
	case XK_space             : return KEY_INPUT_SPACE ;       // スペースキー
	case XK_Page_Up           : return KEY_INPUT_PGUP ;        // PageUpキー
	case XK_Page_Down         : return KEY_INPUT_PGDN ;        // PageDownキー
	case XK_End               : return KEY_INPUT_END ;         // Endキー
	case XK_Home              : return KEY_INPUT_HOME ;        // Homeキー
	case XK_Left              : return KEY_INPUT_LEFT ;        // 左キー
	case XK_Up                : return KEY_INPUT_UP ;          // 上キー
	case XK_Right             : return KEY_INPUT_RIGHT ;       // 右キー
	case XK_Down              : return KEY_INPUT_DOWN ;        // 下キー
	case XK_Insert            : return KEY_INPUT_INSERT ;      // Insertキー
	case XK_Delete            : return KEY_INPUT_DELETE ;      // Deleteキー

	case XK_minus             : return KEY_INPUT_MINUS ;       // −キー
	//case XK_backslash         : return KEY_INPUT_YEN ;         // ¥キー rk:132(0x84) k0:92(0x5c)[backslash] k1:124(0x7c)[bar]
	case XK_asciicircum       : return KEY_INPUT_PREVTRACK ;   // ＾キー
	case XK_period            : return KEY_INPUT_PERIOD ;      // ．キー
	case XK_slash             : return KEY_INPUT_SLASH ;       // ／キー
	case XK_Alt_L             : return KEY_INPUT_LALT ;        // 左Altキー
	case XK_Alt_R             : return KEY_INPUT_RALT ;        // 右Altキー
	case XK_Scroll_Lock       : return KEY_INPUT_SCROLL ;      // ScrollLockキー
	case XK_semicolon         : return KEY_INPUT_SEMICOLON ;   // ；キー
	case XK_colon             : return KEY_INPUT_COLON ;       // ：キー
	case XK_bracketleft       : return KEY_INPUT_LBRACKET ;    // ［キー
	case XK_bracketright      : return KEY_INPUT_RBRACKET ;    // ］キー
	case XK_at                : return KEY_INPUT_AT ;          // ＠キー
	//case XK_backslash         : return KEY_INPUT_BACKSLASH ;   // ＼キー rk:97(0x61) k0:92(0x5c)[backslash] k1:95(0x5f)[underscore]
	case XK_comma             : return KEY_INPUT_COMMA ;       // ，キー
	case XK_Zenkaku_Hankaku   : return KEY_INPUT_KANJI ;       // 漢字キー
	case XK_Henkan            : return KEY_INPUT_CONVERT ;     // 変換キー
	case XK_Muhenkan          : return KEY_INPUT_NOCONVERT ;   // 無変換キー
	case XK_Hiragana_Katakana : return KEY_INPUT_KANA ;        // カナキー
	case XK_Menu              : return KEY_INPUT_APPS ;        // アプリケーションメニューキー
	case XK_Eisu_toggle       : return KEY_INPUT_CAPSLOCK ;    // CaspLockキー (not XK_Caps_Lock?)
	case XK_Print             : return KEY_INPUT_SYSRQ ;       // PrintScreenキー
	case XK_Pause             : return KEY_INPUT_PAUSE ;       // PauseBreakキー
	case XK_Super_L           : return KEY_INPUT_LWIN ;        // 左Winキー
	case XK_Super_R           : return KEY_INPUT_RWIN ;        // 右Winキー

	case XK_Num_Lock          : return KEY_INPUT_NUMLOCK ;     // テンキーNumLockキー
	case XK_KP_Insert         : return KEY_INPUT_NUMPAD0 ;     // テンキー０
	case XK_KP_End            : return KEY_INPUT_NUMPAD1 ;     // テンキー１
	case XK_KP_Down           : return KEY_INPUT_NUMPAD2 ;     // テンキー２
	case XK_KP_Page_Down      : return KEY_INPUT_NUMPAD3 ;     // テンキー３
	case XK_KP_Left           : return KEY_INPUT_NUMPAD4 ;     // テンキー４
	case XK_KP_Begin          : return KEY_INPUT_NUMPAD5 ;     // テンキー５
	case XK_KP_Right          : return KEY_INPUT_NUMPAD6 ;     // テンキー６
	case XK_KP_Home           : return KEY_INPUT_NUMPAD7 ;     // テンキー７
	case XK_KP_Up             : return KEY_INPUT_NUMPAD8 ;     // テンキー８
	case XK_KP_Page_Up        : return KEY_INPUT_NUMPAD9 ;     // テンキー９
	case XK_KP_Multiply       : return KEY_INPUT_MULTIPLY ;    // テンキー＊キー
	case XK_KP_Add            : return KEY_INPUT_ADD ;         // テンキー＋キー
	case XK_KP_Subtract       : return KEY_INPUT_SUBTRACT ;    // テンキー−キー
	case XK_KP_Delete         : return KEY_INPUT_DECIMAL ;     // テンキー．キー
	case XK_KP_Divide         : return KEY_INPUT_DIVIDE ;      // テンキー／キー
	case XK_KP_Enter          : return KEY_INPUT_NUMPADENTER ; // テンキーのエンターキー

	case XK_F1                : return KEY_INPUT_F1 ;          // Ｆ１キー
	case XK_F2                : return KEY_INPUT_F2 ;          // Ｆ２キー
	case XK_F3                : return KEY_INPUT_F3 ;          // Ｆ３キー
	case XK_F4                : return KEY_INPUT_F4 ;          // Ｆ４キー
	case XK_F5                : return KEY_INPUT_F5 ;          // Ｆ５キー
	case XK_F6                : return KEY_INPUT_F6 ;          // Ｆ６キー
	case XK_F7                : return KEY_INPUT_F7 ;          // Ｆ７キー
	case XK_F8                : return KEY_INPUT_F8 ;          // Ｆ８キー
	case XK_F9                : return KEY_INPUT_F9 ;          // Ｆ９キー
	case XK_F10               : return KEY_INPUT_F10 ;         // Ｆ１０キー
	case XK_F11               : return KEY_INPUT_F11 ;         // Ｆ１１キー
	case XK_F12               : return KEY_INPUT_F12 ;         // Ｆ１２キー

	case XK_a                 : return KEY_INPUT_A ;           // Ａキー
	case XK_b                 : return KEY_INPUT_B ;           // Ｂキー
	case XK_c                 : return KEY_INPUT_C ;           // Ｃキー
	case XK_d                 : return KEY_INPUT_D ;           // Ｄキー
	case XK_e                 : return KEY_INPUT_E ;           // Ｅキー
	case XK_f                 : return KEY_INPUT_F ;           // Ｆキー
	case XK_g                 : return KEY_INPUT_G ;           // Ｇキー
	case XK_h                 : return KEY_INPUT_H ;           // Ｈキー
	case XK_i                 : return KEY_INPUT_I ;           // Ｉキー
	case XK_j                 : return KEY_INPUT_J ;           // Ｊキー
	case XK_k                 : return KEY_INPUT_K ;           // Ｋキー
	case XK_l                 : return KEY_INPUT_L ;           // Ｌキー
	case XK_m                 : return KEY_INPUT_M ;           // Ｍキー
	case XK_n                 : return KEY_INPUT_N ;           // Ｎキー
	case XK_o                 : return KEY_INPUT_O ;           // Ｏキー
	case XK_p                 : return KEY_INPUT_P ;           // Ｐキー
	case XK_q                 : return KEY_INPUT_Q ;           // Ｑキー
	case XK_r                 : return KEY_INPUT_R ;           // Ｒキー
	case XK_s                 : return KEY_INPUT_S ;           // Ｓキー
	case XK_t                 : return KEY_INPUT_T ;           // Ｔキー
	case XK_u                 : return KEY_INPUT_U ;           // Ｕキー
	case XK_v                 : return KEY_INPUT_V ;           // Ｖキー
	case XK_w                 : return KEY_INPUT_W ;           // Ｗキー
	case XK_x                 : return KEY_INPUT_X ;           // Ｘキー
	case XK_y                 : return KEY_INPUT_Y ;           // Ｙキー
	case XK_z                 : return KEY_INPUT_Z ;           // Ｚキー

	case XK_0                 : return KEY_INPUT_0 ;           // ０キー
	case XK_1                 : return KEY_INPUT_1 ;           // １キー
	case XK_2                 : return KEY_INPUT_2 ;           // ２キー
	case XK_3                 : return KEY_INPUT_3 ;           // ３キー
	case XK_4                 : return KEY_INPUT_4 ;           // ４キー
	case XK_5                 : return KEY_INPUT_5 ;           // ５キー
	case XK_6                 : return KEY_INPUT_6 ;           // ６キー
	case XK_7                 : return KEY_INPUT_7 ;           // ７キー
	case XK_8                 : return KEY_INPUT_8 ;           // ８キー
	case XK_9                 : return KEY_INPUT_9 ;           // ９キー
	case XK_backslash         : return ( k1 == XK_bar ) ? KEY_INPUT_YEN : KEY_INPUT_BACKSLASH ;
	default                   : return 0 ;
	}
} ;

// プログラム------------------------------------------------------------------

// 入力システムを初期化する処理の環境依存処理
extern int InitializeInputSystem_PF_Timing0( void )
{
	static int NowInitialize = FALSE ;
//	int i, j ;

	// 既に初期化処理が開始されている場合も何もせずに終了
	if( NowInitialize )
	{
		return 0 ;
	}

	// 初期化中フラグを立てる
	NowInitialize = TRUE ;


	// ジョイパッドの数を増やす
	InputSysData.PadNum = 1 ;

	// 初期化中フラグを倒す
	NowInitialize = FALSE ;

	// 正常終了
	return 0 ;
}

// 入力システムの後始末をする処理の環境依存処理
extern int TerminateInputSystem_PF_Timing0( void )
{
	// 正常終了
	return 0 ;
}

// 自動初期化を行う環境依存処理
extern int AutoInitialize_PF( void )
{
	return 0 ;
}

// ジョイパッドのセットアップの環境依存処理
extern int SetupJoypad_PF( void )
{
	// 正常終了
	return 0 ;
}

// ジョイパッドの後始末を行う処理の環境依存処理
extern int TerminateJoypad_PF( void )
{
	// 正常終了
	return 0 ;
}

// 入力状態の更新の環境依存処理
extern int UpdateKeyboardInputState_PF( int UseProcessMessage )
{
	//_MEMSET( &InputSysData.KeyInputBuf, 0, sizeof( InputSysData.KeyInputBuf ) ) ;
//	if( InputSysData.PF.SourceNum[ LINUX_INPUT_SOURCE_KEYBOARD ] > 0 )
//	{
//		int i, j ;
//
//		for( j = 0 ; j < InputSysData.PF.SourceNum[ LINUX_INPUT_SOURCE_KEYBOARD ] ; j ++ )
//		{
//			INPUT_LINUX_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.SourceNoToInputInfoTable[ LINUX_INPUT_SOURCE_KEYBOARD ][ j ] ] ;
//
//			for( i = 0 ; g_LinuxKeyToDXInputKey[i][0] != 0xffff ; i ++ )
//			{
//				InputSysData.KeyInputBuf[ g_LinuxKeyToDXInputKey[ i ][ 1 ] ] |= Info->KeyState[ g_LinuxKeyToDXInputKey[ i ][ 0 ] ] ? 0x80 : 0x00 ;
//			}
//		}
//	}

	return 0 ;
}

// パッドの入力状態の更新の環境依存処理
extern int UpdateJoypadInputState_PF( int PadNo )
{

	// 終了
	return 0 ;
}

// パッドエフェクトの再生状態を更新する関数の環境依存処理
extern int RefreshEffectPlayState_PF( int AlwaysRunFlag )
{
	// 終了
	return 0 ;
}

// 指定のパッドが振動に対応しているかどうかを取得する( TRUE:対応している  FALSE:対応していない )
extern int CheckJoypadVibrationEnable_PF( INPUTPADDATA *pad, int EffectIndex )
{
	return FALSE ;
}

// 指定の入力デバイスが XInput に対応しているかどうかを取得する処理の環境依存処理( 戻り値  TRUE:XInput対応の入力デバイス  FALSE:XInput非対応の入力デバイス   -1:エラー )( DX_INPUT_KEY や DX_INPUT_KEY_PAD1 など、キーボードが絡むタイプを InputType に渡すとエラーとなり -1 を返す )の環境依存処理
extern int CheckJoypadXInput_PF( int InputType )
{
	return FALSE ;
}

// ジョイパッドのタイプを取得する( 戻り値  -1:エラー  0以上:ジョイパッドタイプ( DX_PADTYPE_XBOX_360 など ) )
extern int GetJoypadType_PF( int InputType )
{
	return DX_PADTYPE_OTHER ;
}

// マウスのボタンの状態を得る処理の環境依存処理
extern int GetMouseInput_PF( void )
{
	int res = 0 ;

	res |= InputSysData.MouseInputBuf[0] ? ( 1 << 0 ) : 0;
	res |= InputSysData.MouseInputBuf[1] ? ( 1 << 1 ) : 0;
	res |= InputSysData.MouseInputBuf[2] ? ( 1 << 2 ) : 0;
	// res |= InputSysData.MouseInputBuf[3] ? ( 1 << 3 ) : 0;
	// res |= InputSysData.MouseInputBuf[4] ? ( 1 << 4 ) : 0;
	// res |= InputSysData.MouseInputBuf[5] ? ( 1 << 5 ) : 0;
	// res |= InputSysData.MouseInputBuf[6] ? ( 1 << 6 ) : 0;
	// res |= InputSysData.MouseInputBuf[7] ? ( 1 << 7 ) : 0;

	return res ;
}

// 垂直マウスホイールの回転量を取得する
extern int GetMouseWheelRotVol_PF( int CounterReset )
{
	int Vol ;

	Vol = InputSysData.MouseMoveZ ;
	if( CounterReset )
	{
		InputSysData.MouseMoveZ -= Vol ;
	}

	return Vol ;
}

// 水平マウスホイールの回転量を取得する
extern int GetMouseHWheelRotVol_PF( int CounterReset )
{
	int Vol ;

	Vol = InputSysData.MouseMoveHZ ;
	if( CounterReset )
	{
		InputSysData.MouseMoveHZ -= Vol ;
	}

	return Vol ;
}

// 垂直マウスホイールの回転量を取得する( 戻り値が float 型 )
extern float GetMouseWheelRotVolF_PF( int CounterReset )
{
	float Vol ;

	Vol = ( float )InputSysData.MouseMoveZ ;
	if( CounterReset )
	{
		InputSysData.MouseMoveZ = 0 ;
	}

	return Vol ;
}

// 水平マウスホイールの回転量を取得する( 戻り値が float 型 )
extern float GetMouseHWheelRotVolF_PF( int CounterReset )
{
	float Vol ;

	Vol = ( float )InputSysData.MouseMoveHZ ;
	if( CounterReset )
	{
		InputSysData.MouseMoveHZ = 0 ;
	}

	return Vol ;
}

// マウスの位置を取得する
extern int GetMousePoint_PF( int *XBuf, int *YBuf )
{
	if( XBuf != NULL )
	{
		*XBuf = InputSysData.MouseInputLog[ 0 ].ClickX ;
	}

	if( YBuf != NULL )
	{
		*YBuf = InputSysData.MouseInputLog[ 0 ].ClickY ;
	}

//	if( InputSysData.PF.SourceNum[ LINUX_INPUT_SOURCE_MOUSE ] > 0 )
//	{
//		int ScreenX, ScreenY ;
//		INPUT_LINUX_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.SourceNoToInputInfoTable[ LINUX_INPUT_SOURCE_MOUSE ][ 0 ] ] ;
//
//		ScreenX = ( int )Info->AxisX ;
//		ScreenY = ( int )Info->AxisY ;
//		ConvScreenPositionToDxScreenPosition( ScreenX, ScreenY, XBuf, YBuf ) ;
//	}

	return 0 ;
}

// マウスの位置をセットする
extern int SetMousePoint_PF( int PointX , int PointY )
{
	return 0 ;
}

// ジョイパッドの無効ゾーンの設定を行う関数の環境依存処理
extern int SetJoypadDeadZone_PF( INPUTPADDATA *pad )
{
	// 正常終了
	return 0 ;
}

// デバイスＩＤから値を代入すべき入力情報番号を取得する
extern int GetLinuxDeviceIdToInputInfoNo( int32_t Source, int32_t DeviceId )
{
	return 0 ;
}


// ゲームパッドの番号と入力情報との対応テーブルを更新する
extern int RefreshLinuxGamePadSourceNoToInputInfoTable( void )
{
	// 終了
	return 0 ;
}


extern int ProcessMouseInputEvent( XEvent ev )
{
	switch( ev.type )
	{
	// 2と3が逆(DxLib:123:左右中 / Xlib:123:左中右)
	// 4と5はホイール上と下
	// 6と7はホイール左と右
	case ButtonPress :
	case ButtonRelease :
		if ( ev.xbutton.button == 1 ) { InputSysData.MouseInputBuf[ 0 ] = ( ev.type == ButtonPress ) ? 1 : 0 ; }
		if ( ev.xbutton.button == 3 ) { InputSysData.MouseInputBuf[ 1 ] = ( ev.type == ButtonPress ) ? 1 : 0 ; }
		if ( ev.xbutton.button == 2 ) { InputSysData.MouseInputBuf[ 2 ] = ( ev.type == ButtonPress ) ? 1 : 0 ; }
		if ( ev.xbutton.button == 4 || ev.type == ButtonPress ) { InputSysData.MouseMoveZ++ ; }
		if ( ev.xbutton.button == 5 || ev.type == ButtonPress ) { InputSysData.MouseMoveZ-- ; }
		if ( ev.xbutton.button == 6 || ev.type == ButtonPress ) { InputSysData.MouseMoveHZ-- ; }
		if ( ev.xbutton.button == 7 || ev.type == ButtonPress ) { InputSysData.MouseMoveHZ++ ; }
		break ;
	case MotionNotify :
		// TODO: 仮、InputLog対応するなら変化するかも
		InputSysData.MouseInputLog[ 0 ].ClickX = ev.xmotion.x ;
		InputSysData.MouseInputLog[ 0 ].ClickY = ev.xmotion.y ;
		break ;
	default :
		break ;
	}
	return 0 ;
}

extern int ProcessKeyboardInputEvent( XEvent ev, Display *dpy )
{
	KeySym k0, k1;
	switch( ev.type )
	{
	case KeyPress :
	case KeyRelease :
		k0 = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 0);
		k1 = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 1);
		//printf("rk:%1$d(0x%1$x) k0:%2$ld(0x%2$lx) k1:%3$ld(0x%3$lx) %4$c\n", ev.xkey.keycode, k0, k1, (ev.type==KeyPress)?'v':'^');
		InputSysData.KeyInputBuf[LinuxKeySymToDXInputKey(k0, k1)] = ( ev.type == KeyPress ) ? 0x80 : 0 ;
		break ;
	default :
		break ;
	}
	return 0;
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_INPUT



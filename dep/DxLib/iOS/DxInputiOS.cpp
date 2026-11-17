//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用入力情報プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_INPUT

// インクルード----------------------------------------------------------------
#include "DxInputiOS.h"
#include "DxSystemiOS.h"
#include "DxSystemiOS_ObjC.h"
#include "../DxLog.h"
#include "../DxInput.h"
#include "../DxSystem.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// #define DEADZONE_D							(0.35)
// #define DEADZONE							(DWORD)( DEADZONE_D * 65536 )
// #define DEADZONE_XINPUT( ZONE )				(short)( 32767 * (ZONE) / 65536)
// #define DEADZONE_XINPUT_TRIGGER( ZONE )		(short)(   255 * (ZONE) / 65536)
// #define VALIDRANGE_XINPUT( ZONE )			( 32767 - DEADZONE_XINPUT(ZONE))
// #define VALIDRANGE_XINPUT_TRIGGER( ZONE )	(   255 - DEADZONE_XINPUT_TRIGGER(ZONE))

// 型定義----------------------------------------------------------------------

// 定数定義 ----------------------------------------------------------------------

// データ宣言------------------------------------------------------------------

// const static int32_t g_iOSInputSourceTable[ IOS_INPUT_SOURCE_NUM ] =
// {
// 	AINPUT_SOURCE_UNKNOWN,
// 	AINPUT_SOURCE_KEYBOARD,
// 	AINPUT_SOURCE_DPAD,
// 	AINPUT_SOURCE_GAMEPAD,
// 	AINPUT_SOURCE_TOUCHSCREEN,
// 	AINPUT_SOURCE_MOUSE,
// 	AINPUT_SOURCE_STYLUS,
// 	AINPUT_SOURCE_TRACKBALL,
// 	AINPUT_SOURCE_TOUCHPAD,
// 	AINPUT_SOURCE_TOUCH_NAVIGATION,
// 	AINPUT_SOURCE_JOYSTICK,
// } ;
// 
// const static unsigned short g_iOSKeyToDXInputKey[][ 2 /* 0:iOSキーコード  1:DirectInputキーコード  */ ] =
// {
// 	{ AKEYCODE_A,				KEY_INPUT_A },			// Ａキー
// 	{ AKEYCODE_B,				KEY_INPUT_B },			// Ｂキー
// 	{ AKEYCODE_C,				KEY_INPUT_C },			// Ｃキー
// 	{ AKEYCODE_D,				KEY_INPUT_D },			// Ｄキー
// 	{ AKEYCODE_E,				KEY_INPUT_E },			// Ｅキー
// 	{ AKEYCODE_F,				KEY_INPUT_F },			// Ｆキー
// 	{ AKEYCODE_G,				KEY_INPUT_G },			// Ｇキー
// 	{ AKEYCODE_H,				KEY_INPUT_H },			// Ｈキー
// 	{ AKEYCODE_I,				KEY_INPUT_I },			// Ｉキー
// 	{ AKEYCODE_J,				KEY_INPUT_J },			// Ｊキー
// 	{ AKEYCODE_K,				KEY_INPUT_K },			// Ｋキー
// 	{ AKEYCODE_L,				KEY_INPUT_L },			// Ｌキー
// 	{ AKEYCODE_M,				KEY_INPUT_M },			// Ｍキー
// 	{ AKEYCODE_N,				KEY_INPUT_N },			// Ｎキー
// 	{ AKEYCODE_O,				KEY_INPUT_O },			// Ｏキー
// 	{ AKEYCODE_P,				KEY_INPUT_P },			// Ｐキー
// 	{ AKEYCODE_Q,				KEY_INPUT_Q },			// Ｑキー
// 	{ AKEYCODE_R,				KEY_INPUT_R },			// Ｒキー
// 	{ AKEYCODE_S,				KEY_INPUT_S },			// Ｓキー
// 	{ AKEYCODE_T,				KEY_INPUT_T },			// Ｔキー
// 	{ AKEYCODE_U,				KEY_INPUT_U },			// Ｕキー
// 	{ AKEYCODE_V,				KEY_INPUT_V },			// Ｖキー
// 	{ AKEYCODE_W,				KEY_INPUT_W },			// Ｗキー
// 	{ AKEYCODE_X,				KEY_INPUT_X },			// Ｘキー
// 	{ AKEYCODE_Y,				KEY_INPUT_Y },			// Ｙキー
// 	{ AKEYCODE_Z,				KEY_INPUT_Z },			// Ｚキー
// 	{ AKEYCODE_0,				KEY_INPUT_0 },			// ０キー
// 	{ AKEYCODE_1,				KEY_INPUT_1 },			// １キー
// 	{ AKEYCODE_2,				KEY_INPUT_2 },			// ２キー
// 	{ AKEYCODE_3,				KEY_INPUT_3 },			// ３キー
// 	{ AKEYCODE_4,				KEY_INPUT_4 },			// ４キー
// 	{ AKEYCODE_5,				KEY_INPUT_5 },			// ５キー
// 	{ AKEYCODE_6,				KEY_INPUT_6 },			// ６キー
// 	{ AKEYCODE_7,				KEY_INPUT_7 },			// ７キー
// 	{ AKEYCODE_8,				KEY_INPUT_8 },			// ８キー
// 	{ AKEYCODE_9,				KEY_INPUT_9 },			// ９キー
// 
// 	{ AKEYCODE_DPAD_LEFT,		KEY_INPUT_LEFT },		// 左キー
// 	{ AKEYCODE_DPAD_UP,			KEY_INPUT_UP },			// 上キー
// 	{ AKEYCODE_DPAD_RIGHT,		KEY_INPUT_RIGHT },		// 右キー
// 	{ AKEYCODE_DPAD_DOWN,		KEY_INPUT_DOWN },		// 下キー
// 
// 	{ AKEYCODE_SEMICOLON,		KEY_INPUT_SEMICOLON },	// ；キー
// 	{ AKEYCODE_LEFT_BRACKET,	KEY_INPUT_LBRACKET },	// ［キー
// 	{ AKEYCODE_RIGHT_BRACKET,	KEY_INPUT_RBRACKET },	// ］キー
// 	{ AKEYCODE_AT,				KEY_INPUT_AT },			// ＠キー
// 	{ AKEYCODE_COMMA,			KEY_INPUT_COMMA },		// ，キー
// 
// 	{ AKEYCODE_MINUS,			KEY_INPUT_MINUS },		// －キー
// 	{ AKEYCODE_BACKSLASH,		KEY_INPUT_YEN },		// ￥キー
// 	{ AKEYCODE_PERIOD,			KEY_INPUT_PERIOD },		// ．キー
// 	{ AKEYCODE_SLASH,			KEY_INPUT_SLASH },		// ／キー
// 
// 	{ AKEYCODE_DEL,				KEY_INPUT_DELETE },		// デリートキー
// 
// 	{ AKEYCODE_BACK,			KEY_INPUT_ESCAPE },		// エスケープキー
// 	{ AKEYCODE_SPACE,			KEY_INPUT_SPACE },		// スペースキー
// 
// 	{ AKEYCODE_TAB, 			KEY_INPUT_TAB },		// タブキー
// 	{ AKEYCODE_ENTER,			KEY_INPUT_RETURN },		// エンターキー
// 	{ AKEYCODE_HOME,			KEY_INPUT_HOME },		// ホームキー
// 
// 	{ 0xffff,					0xffff },
// } ;

// 関数プロトタイプ宣言 -------------------------------------------------------

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

//	// 無効ゾーンのセット
//	for( i = 0 ; i < MAX_JOYPAD_NUM ; i ++ )
//	{
//		InputSysData.Pad[ i ].DeadZone = DEADZONE ;
//		InputSysData.Pad[ i ].DeadZoneD = DEADZONE_D ;
//	}
//
//	// キーボードとジョイパッドの入力のデフォルトの対応表を設定する
//	if( InputSysData.KeyToJoypadInputInitializeFlag == FALSE )
//	{
//		InputSysData.KeyToJoypadInputInitializeFlag = TRUE ;
//
//		for( i = 0 ; i < MAX_JOYPAD_NUM ; i ++ )
//		{
//			for ( j = 0 ; j < 32 ; j ++ )
//			{
//				InputSysData.KeyToJoypadInput[ i ][ j ][ 0 ] = -1 ;
//				InputSysData.KeyToJoypadInput[ i ][ j ][ 1 ] = -1 ;
//			}
//		}
//		InputSysData.KeyToJoypadInput[ 0 ][  0 ][ 0 ] = KEY_INPUT_NUMPAD2 ;
//		InputSysData.KeyToJoypadInput[ 0 ][  0 ][ 1 ] = KEY_INPUT_DOWN ;
//		InputSysData.KeyToJoypadInput[ 0 ][  1 ][ 0 ] = KEY_INPUT_NUMPAD4 ;
//		InputSysData.KeyToJoypadInput[ 0 ][  1 ][ 1 ] = KEY_INPUT_LEFT ;
//		InputSysData.KeyToJoypadInput[ 0 ][  2 ][ 0 ] = KEY_INPUT_NUMPAD6 ;
//		InputSysData.KeyToJoypadInput[ 0 ][  2 ][ 1 ] = KEY_INPUT_RIGHT ;
//		InputSysData.KeyToJoypadInput[ 0 ][  3 ][ 0 ] = KEY_INPUT_NUMPAD8 ;
//		InputSysData.KeyToJoypadInput[ 0 ][  3 ][ 1 ] = KEY_INPUT_UP ;
//		InputSysData.KeyToJoypadInput[ 0 ][  4 ][ 0 ] = KEY_INPUT_Z ;
//		InputSysData.KeyToJoypadInput[ 0 ][  5 ][ 0 ] = KEY_INPUT_X ;
//		InputSysData.KeyToJoypadInput[ 0 ][  6 ][ 0 ] = KEY_INPUT_C ;
//		InputSysData.KeyToJoypadInput[ 0 ][  7 ][ 0 ] = KEY_INPUT_A ;
//		InputSysData.KeyToJoypadInput[ 0 ][  8 ][ 0 ] = KEY_INPUT_S ;
//		InputSysData.KeyToJoypadInput[ 0 ][  9 ][ 0 ] = KEY_INPUT_D ;
//		InputSysData.KeyToJoypadInput[ 0 ][ 10 ][ 0 ] = KEY_INPUT_Q ;
//		InputSysData.KeyToJoypadInput[ 0 ][ 11 ][ 0 ] = KEY_INPUT_W ;
//		InputSysData.KeyToJoypadInput[ 0 ][ 12 ][ 0 ] = KEY_INPUT_RETURN ;
//		InputSysData.KeyToJoypadInput[ 0 ][ 13 ][ 0 ] = KEY_INPUT_SPACE ;
//	}
//
//	InputSysData.PF.UseInputInfoNum = 1 ;

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
	_MEMSET( &InputSysData.KeyInputBuf, 0, sizeof( InputSysData.KeyInputBuf ) ) ;
//	if( InputSysData.PF.SourceNum[ IOS_INPUT_SOURCE_KEYBOARD ] > 0 )
//	{
//		int i, j ;
//
//		for( j = 0 ; j < InputSysData.PF.SourceNum[ IOS_INPUT_SOURCE_KEYBOARD ] ; j ++ )
//		{
//			INPUT_IOS_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.SourceNoToInputInfoTable[ IOS_INPUT_SOURCE_KEYBOARD ][ j ] ] ;
//
//			for( i = 0 ; g_iOSKeyToDXInputKey[i][0] != 0xffff ; i ++ )
//			{
//				InputSysData.KeyInputBuf[ g_iOSKeyToDXInputKey[ i ][ 1 ] ] |= Info->KeyState[ g_iOSKeyToDXInputKey[ i ][ 0 ] ] ? 0x80 : 0x00 ;
//			}
//		}
//	}

	return 0 ;
}

// パッドの入力状態の更新の環境依存処理
extern int UpdateJoypadInputState_PF( int PadNo )
{
	UpdateControllerInfo( PadNo ) ;

	if( InputSysData.PF.iCadeEnable )
	{
		INPUTPADDATA *pad = &InputSysData.Pad[ 0 ] ;

		_MEMSET( &pad->State, 0, sizeof( pad->State ) ) ;

		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_UP    ] )		pad->State.Y = -1000 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_RIGHT ] )		pad->State.X =  1000 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_DOWN  ] )		pad->State.Y =  1000 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_LEFT  ] )		pad->State.X = -1000 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_0     ] )		pad->State.Buttons[ 0 ] = 0x80 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_1     ] )		pad->State.Buttons[ 1 ] = 0x80 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_2     ] )		pad->State.Buttons[ 2 ] = 0x80 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_3     ] )		pad->State.Buttons[ 3 ] = 0x80 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_4     ] )		pad->State.Buttons[ 4 ] = 0x80 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_5     ] )		pad->State.Buttons[ 5 ] = 0x80 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_6     ] )		pad->State.Buttons[ 6 ] = 0x80 ;
		if( InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_7     ] )		pad->State.Buttons[ 7 ] = 0x80 ;

		pad->State.POV[ 0 ]      = 0xffffffff ;
		pad->State.POV[ 1 ]      = 0xffffffff ;
		pad->State.POV[ 2 ]      = 0xffffffff ;
		pad->State.POV[ 3 ]      = 0xffffffff ;
	}

//	INPUTPADDATA *pad = &InputSysData.Pad[ PadNo ] ;
//	INPUT_IOS_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.GamePadSourceNoToInputInfoTable[ PadNo ] ] ;
//	float DeadZone = pad->DeadZone / 65536.0f ;
//
//	if( InputSysData.PF.GamePadSourceNum <= 0 )
//	{
//		return 0 ;
//	}
//
//	pad->State.X  = Info->AxisX  < DeadZone && Info->AxisX  > -DeadZone ? 0 : ( int )( Info->AxisX  * 1000.0f ) ;
//	pad->State.Y  = Info->AxisY  < DeadZone && Info->AxisY  > -DeadZone ? 0 : ( int )( Info->AxisY  * 1000.0f ) ;
//	pad->State.Z  = Info->AxisZ  < DeadZone && Info->AxisZ  > -DeadZone ? 0 : ( int )( Info->AxisZ  * 1000.0f ) ;
//	pad->State.Rx = Info->AxisRx < DeadZone && Info->AxisRx > -DeadZone ? 0 : ( int )( Info->AxisRx * 1000.0f ) ;
//	pad->State.Ry = Info->AxisRy < DeadZone && Info->AxisRy > -DeadZone ? 0 : ( int )( Info->AxisRy * 1000.0f ) ;
//	pad->State.Rz = Info->AxisRz < DeadZone && Info->AxisRz > -DeadZone ? 0 : ( int )( Info->AxisRz * 1000.0f ) ;
//	pad->State.Slider[ 0 ]   = ( int )( Info->AxisLTrigger * 1000.0f ) ;
//	pad->State.Slider[ 1 ]   = ( int )( Info->AxisRTrigger * 1000.0f ) ;
//	if( Info->AxisHatX > 0.5f )
//	{
//		if( Info->AxisHatY > 0.5f )
//		{
//			pad->State.POV[ 0 ] = 13500 ;
//		}
//		else
//		if( Info->AxisHatY < -0.5f )
//		{
//			pad->State.POV[ 0 ] = 4500 ;
//		}
//		else
//		{
//			pad->State.POV[ 0 ] = 9000 ;
//		}
//	}
//	else
//	if( Info->AxisHatX < -0.5f )
//	{
//		if( Info->AxisHatY > 0.5f )
//		{
//			pad->State.POV[ 0 ] = 22500 ;
//		}
//		else
//		if( Info->AxisHatY < -0.5f )
//		{
//			pad->State.POV[ 0 ] = 31500 ;
//		}
//		else
//		{
//			pad->State.POV[ 0 ] = 27000 ;
//		}
//	}
//	else
//	{
//		if( Info->AxisHatY > 0.5f )
//		{
//			pad->State.POV[ 0 ] = 18000 ;
//		}
//		else
//		if( Info->AxisHatY < -0.5f )
//		{
//			pad->State.POV[ 0 ] = 0 ;
//		}
//		else
//		{
//			pad->State.POV[ 0 ] = 0xffffffff ;
//		}
//	}
//	pad->State.POV[ 1 ]      = 0xffffffff ;
//	pad->State.POV[ 2 ]      = 0xffffffff ;
//	pad->State.POV[ 3 ]      = 0xffffffff ;
//	pad->State.Buttons[  0 ] = Info->KeyState[ AKEYCODE_BUTTON_A      ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[  1 ] = Info->KeyState[ AKEYCODE_BUTTON_B      ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[  2 ] = Info->KeyState[ AKEYCODE_BUTTON_X      ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[  3 ] = Info->KeyState[ AKEYCODE_BUTTON_Y      ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[  4 ] = Info->KeyState[ AKEYCODE_BUTTON_L1     ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[  5 ] = Info->KeyState[ AKEYCODE_BUTTON_R1     ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[  6 ] = Info->KeyState[ AKEYCODE_BUTTON_L2     ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[  7 ] = Info->KeyState[ AKEYCODE_BUTTON_R2     ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[  8 ] = Info->KeyState[ AKEYCODE_BUTTON_THUMBL ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[  9 ] = Info->KeyState[ AKEYCODE_BUTTON_THUMBR ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[ 10 ] = Info->KeyState[ AKEYCODE_BUTTON_SELECT ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[ 11 ] = Info->KeyState[ AKEYCODE_BUTTON_START  ] ? 0x80 : 0x00 ;
//	pad->State.Buttons[ 12 ] = 0 ;
//	pad->State.Buttons[ 13 ] = 0 ;
//	pad->State.Buttons[ 14 ] = 0 ;
//	pad->State.Buttons[ 15 ] = 0 ;
//	pad->State.Buttons[ 16 ] = 0 ;
//	pad->State.Buttons[ 17 ] = 0 ;
//	pad->State.Buttons[ 18 ] = 0 ;
//	pad->State.Buttons[ 19 ] = 0 ;
//	pad->State.Buttons[ 20 ] = 0 ;
//	pad->State.Buttons[ 21 ] = 0 ;
//	pad->State.Buttons[ 22 ] = 0 ;
//	pad->State.Buttons[ 23 ] = 0 ;
//	pad->State.Buttons[ 24 ] = 0 ;
//	pad->State.Buttons[ 25 ] = 0 ;
//	pad->State.Buttons[ 26 ] = 0 ;
//	pad->State.Buttons[ 27 ] = 0 ;
//	pad->State.Buttons[ 28 ] = 0 ;
//	pad->State.Buttons[ 29 ] = 0 ;
//	pad->State.Buttons[ 30 ] = 0 ;
//	pad->State.Buttons[ 31 ] = 0 ;

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

//	if( InputSysData.PF.SourceNum[ IOS_INPUT_SOURCE_MOUSE ] > 0 )
//	{
//		INPUT_IOS_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.SourceNoToInputInfoTable[ IOS_INPUT_SOURCE_MOUSE ][ 0 ] ] ;
//
//		if( ( Info->ButtonState & 0x01 ) != 0 ) res |= MOUSE_INPUT_1 ;
//		if( ( Info->ButtonState & 0x02 ) != 0 ) res |= MOUSE_INPUT_2 ;
//		if( ( Info->ButtonState & 0x04 ) != 0 ) res |= MOUSE_INPUT_3 ;
//		if( ( Info->ButtonState & 0x08 ) != 0 ) res |= MOUSE_INPUT_4 ;
//		if( ( Info->ButtonState & 0x10 ) != 0 ) res |= MOUSE_INPUT_5 ;
//		if( ( Info->ButtonState & 0x20 ) != 0 ) res |= MOUSE_INPUT_6 ;
//		if( ( Info->ButtonState & 0x40 ) != 0 ) res |= MOUSE_INPUT_7 ;
//		if( ( Info->ButtonState & 0x80 ) != 0 ) res |= MOUSE_INPUT_8 ;
//	}

	return res ;
}

// 垂直マウスホイールの回転量を取得する
extern int GetMouseWheelRotVol_PF( int CounterReset )
{
	// 未対応
	return 0 ;
}

// 水平マウスホイールの回転量を取得する
extern int GetMouseHWheelRotVol_PF( int CounterReset )
{
	// 未対応
	return 0 ;
}

// 垂直マウスホイールの回転量を取得する( 戻り値が float 型 )
extern float GetMouseWheelRotVolF_PF( int CounterReset )
{
	// 未対応
	return 0 ;
}

// 水平マウスホイールの回転量を取得する( 戻り値が float 型 )
extern float GetMouseHWheelRotVolF_PF( int CounterReset )
{
	// 未対応
	return 0 ;
}

// マウスの位置を取得する
extern int GetMousePoint_PF( int *XBuf, int *YBuf )
{
	if( XBuf != NULL )
	{
		*XBuf = 0 ;
	}

	if( YBuf != NULL )
	{
		*YBuf = 0 ;
	}

//	if( InputSysData.PF.SourceNum[ IOS_INPUT_SOURCE_MOUSE ] > 0 )
//	{
//		int ScreenX, ScreenY ;
//		INPUT_IOS_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.SourceNoToInputInfoTable[ IOS_INPUT_SOURCE_MOUSE ][ 0 ] ] ;
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
// extern int GetiOSDeviceIdToInputInfoNo( int32_t Source, int32_t DeviceId )
// {
//	int i ;
//
//	for( i = 0 ; i < InputSysData.PF.UseInputInfoNum ; i ++ )
//	{
//		if( InputSysData.PF.InputInfo[ i ].DeviceId == DeviceId )
//		{
//			int UpdateFlag = FALSE ;
//
//			if( ( InputSysData.PF.InputInfo[ i ].Source & Source ) != Source )
//			{
//				UpdateFlag = TRUE ;
//			}
//
//			InputSysData.PF.InputInfo[ i ].Source |= Source ;
//			InputSysData.PF.InputInfo[ i ].UpdateCount = InputSysData.PF.UpdateCount ;
//			InputSysData.PF.UpdateCount ++ ;
//
//			if( UpdateFlag )
//			{
//				RefreshiOSSourceNoToInputInfoTable( Source ) ;
//				RefreshiOSGamePadSourceNoToInputInfoTable() ;
//			}
//			return i ;
//		}
//	}
//
//	if( InputSysData.PF.UseInputInfoNum == IOS_DEVICE_MAX_NUM )
//	{
//		int MinUpdateCountNo ;
//
//		MinUpdateCountNo = 0 ;
//		for( i = 1 ; i < InputSysData.PF.UseInputInfoNum ; i ++ )
//		{
//			if( InputSysData.PF.InputInfo[ MinUpdateCountNo ].UpdateCount > InputSysData.PF.InputInfo[ i ].UpdateCount )
//			{
//				MinUpdateCountNo = i ;
//			}
//		}
//
//		_MEMSET( &InputSysData.PF.InputInfo[ MinUpdateCountNo ], 0, sizeof( InputSysData.PF.InputInfo[ MinUpdateCountNo ] ) ) ;
//		InputSysData.PF.InputInfo[ MinUpdateCountNo ].Source = Source ;
//		InputSysData.PF.InputInfo[ MinUpdateCountNo ].DeviceId = DeviceId ;
//		InputSysData.PF.InputInfo[ MinUpdateCountNo ].UpdateCount = InputSysData.PF.UpdateCount ;
//		InputSysData.PF.UpdateCount ++ ;
//
//		RefreshiOSSourceNoToInputInfoTable( Source ) ;
//		RefreshiOSGamePadSourceNoToInputInfoTable() ;
//
//		return MinUpdateCountNo ;
//	}
//
//	i = InputSysData.PF.UseInputInfoNum ;
//	InputSysData.PF.UseInputInfoNum ++ ;
//
//	_MEMSET( &InputSysData.PF.InputInfo[ i ], 0, sizeof( InputSysData.PF.InputInfo[ i ] ) ) ;
//	InputSysData.PF.InputInfo[ i ].Source = Source ;
//	InputSysData.PF.InputInfo[ i ].DeviceId = DeviceId ;
//	InputSysData.PF.InputInfo[ i ].UpdateCount = InputSysData.PF.UpdateCount ;
//	InputSysData.PF.UpdateCount ++ ;
//
//	RefreshiOSSourceNoToInputInfoTable( Source ) ;
//	RefreshiOSGamePadSourceNoToInputInfoTable() ;
//	
//	return i ;
// }

// 入力ソース番号と入力情報との対応テーブルを更新する
// extern int RefreshiOSSourceNoToInputInfoTable( int32_t Source )
// {
//	INPUT_IOS_DEVICE_INFO *Info ;
//	int *SourceNoTable ;
//	int i, j ;
//	int Num ;
//	int NoTable[ IOS_DEVICE_MAX_NUM ] ;
//	INPUT_IOS_DEVICE_INFO *InfoTable[ IOS_DEVICE_MAX_NUM ] ;
//	int Src ;
//
//	for( Src = 0 ; Src < IOS_INPUT_SOURCE_NUM ; Src ++ )
//	{
//		if( Source == g_iOSInputSourceTable[ Src ] )
//		{
//			break ;
//		}
//	}
//	if( Src == IOS_INPUT_SOURCE_NUM )
//	{
//		return -1 ;
//	}
//
//	Num = 0 ;
//	Info = InputSysData.PF.InputInfo ;
//	for( i = 0 ; i < InputSysData.PF.UseInputInfoNum ; i ++, Info ++ )
//	{
//		if( ( Info->Source & Source ) == Source )
//		{
//			InfoTable[ Num ] = Info ;
//			Num ++ ;
//		}
//	}
//	InputSysData.PF.SourceNum[ Src ] = Num ;
//
//	if( Num == 0 )
//	{
//		return 0 ;
//	}
//
//	NoTable[ 0 ] = 0 ;
//	for( i = 1 ; i < Num ; i ++ )
//	{
//		for( j = 0 ; j < i ; j ++ )
//		{
//			if( InfoTable[ NoTable[ j ] ]->DeviceId < InfoTable[ i ]->DeviceId )
//			{
//				_MEMMOVE( &NoTable[ j + 1 ], &NoTable[ j ], sizeof( int ) * ( i - j ) ) ;
//				NoTable[ j ] = i ;
//				break ;
//			}
//		}
//		if( j == i )
//		{
//			NoTable[ j ] = i ;
//		}
//	}
//
//	SourceNoTable = InputSysData.PF.SourceNoToInputInfoTable[ Src ] ;
//	for( i = 0 ; i < Num ; i ++ )
//	{
//		SourceNoTable[ i ] = InfoTable[ NoTable[ i ] ] - InputSysData.PF.InputInfo ;
//	}
// 
// 	// 終了
// 	return 0 ;
// }

// ゲームパッドの番号と入力情報との対応テーブルを更新する
extern int RefreshiOSGamePadSourceNoToInputInfoTable( void )
{
//	INPUT_IOS_DEVICE_INFO *Info ;
//	int *SourceNoTable ;
//	int i, j ;
//	int Num ;
//	int NoTable[ IOS_DEVICE_MAX_NUM ] ;
//	INPUT_IOS_DEVICE_INFO *InfoTable[ IOS_DEVICE_MAX_NUM ] ;
//
//	Num = 0 ;
//	Info = InputSysData.PF.InputInfo ;
//	for( i = 0 ; i < InputSysData.PF.UseInputInfoNum ; i ++, Info ++ )
//	{
//		// ＤＸライブラリがゲームパッドとして扱う要素を持っている場合はゲームパッドとする
//		if( ( ( Info->Source & AINPUT_SOURCE_GAMEPAD  ) == AINPUT_SOURCE_GAMEPAD ) ||
//			( ( Info->Source & AINPUT_SOURCE_JOYSTICK ) == AINPUT_SOURCE_JOYSTICK ) )
//		{
//			InfoTable[ Num ] = Info ;
//			Num ++ ;
//		}
//	}
//	InputSysData.PF.GamePadSourceNum = Num ;
//
//	if( Num == 0 )
//	{
//		return 0 ;
//	}
//
//	NoTable[ 0 ] = 0 ;
//	for( i = 1 ; i < Num ; i ++ )
//	{
//		for( j = 0 ; j < i ; j ++ )
//		{
//			if( InfoTable[ NoTable[ j ] ]->DeviceId < InfoTable[ i ]->DeviceId )
//			{
//				_MEMMOVE( &NoTable[ j + 1 ], &NoTable[ j ], sizeof( int ) * ( i - j ) ) ;
//				NoTable[ j ] = i ;
//				break ;
//			}
//		}
//		if( j == i )
//		{
//			NoTable[ j ] = i ;
//		}
//	}
//
//	SourceNoTable = InputSysData.PF.GamePadSourceNoToInputInfoTable ;
//	for( i = 0 ; i < Num ; i ++ )
//	{
//		SourceNoTable[ i ] = InfoTable[ NoTable[ i ] ] - InputSysData.PF.InputInfo ;
//	}
//
//	InputSysData.PadNum = Num > MAX_JOYPAD_NUM ? MAX_JOYPAD_NUM : Num ;

	// 終了
	return 0 ;
}

extern int iCadeInputSet( char InputChar )
{
	InputSysData.PF.iCadeEnable = TRUE ;
    switch( InputChar )
	{
	case 'w':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_UP ] = 1 ;		break ;
	case 'e':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_UP ] = 0 ;		break ;
	case 'd':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_RIGHT ] = 1 ;	break ;
	case 'c':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_RIGHT ] = 0 ;	break ;
	case 'x':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_DOWN ] = 1 ;	break ;
	case 'z':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_DOWN ] = 0 ;	break ;
	case 'a':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_LEFT ] = 1 ;	break ;
	case 'q':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_LEFT ] = 0 ;	break ;
	case 'y':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_0 ] = 1 ;		break ;
	case 't':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_0 ] = 0 ;		break ;
	case 'h':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_1 ] = 1 ;		break ;
	case 'r':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_1 ] = 0 ;		break ;
	case 'u':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_2 ] = 1 ;		break ;
	case 'f':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_2 ] = 0 ;		break ;
	case 'j':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_3 ] = 1 ;		break ;
	case 'n':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_3 ] = 0 ;		break ;
	case 'i':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_4 ] = 1 ;		break ;
	case 'm':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_4 ] = 0 ;		break ;
	case 'k':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_5 ] = 1 ;		break ;
	case 'p':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_5 ] = 0 ;		break ;
	case 'o':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_6 ] = 1 ;		break ;
	case 'g':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_6 ] = 0 ;		break ;
	case 'l':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_7 ] = 1 ;		break ;
	case 'v':	InputSysData.PF.iCadeInputState[ IOS_ICADE_INPUT_7 ] = 0 ;		break ;
	}

	return 0 ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_INPUT



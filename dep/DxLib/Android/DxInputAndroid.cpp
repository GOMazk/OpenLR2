//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用入力情報プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_INPUT

// インクルード----------------------------------------------------------------
#include "DxInputAndroid.h"
#include "DxSystemAndroid.h"
#include "../DxLog.h"
#include "../DxInput.h"
#include "../DxSystem.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

#define DEADZONE_XINPUT( ZONE )				(short)( 32767 * (ZONE) / 65536)
#define DEADZONE_XINPUT_TRIGGER( ZONE )		(short)(   255 * (ZONE) / 65536)
#define VALIDRANGE_XINPUT( ZONE )			( 32767 - DEADZONE_XINPUT(ZONE))
#define VALIDRANGE_XINPUT_TRIGGER( ZONE )	(   255 - DEADZONE_XINPUT_TRIGGER(ZONE))

// 型定義----------------------------------------------------------------------

// 定数定義 ----------------------------------------------------------------------

// データ宣言------------------------------------------------------------------

const static int32_t g_AndroidInputSourceTable[ ANDR_INPUT_SOURCE_NUM ] =
{
	AINPUT_SOURCE_UNKNOWN,
	AINPUT_SOURCE_KEYBOARD,
	AINPUT_SOURCE_DPAD,
	AINPUT_SOURCE_GAMEPAD,
	AINPUT_SOURCE_TOUCHSCREEN,
	AINPUT_SOURCE_MOUSE,
	AINPUT_SOURCE_STYLUS,
	AINPUT_SOURCE_TRACKBALL,
	AINPUT_SOURCE_TOUCHPAD,
	AINPUT_SOURCE_TOUCH_NAVIGATION,
	AINPUT_SOURCE_JOYSTICK,
} ;

const static unsigned short g_AndroidKeyToDXInputKey[][ 2 /* 0:Androidキーコード  1:DirectInputキーコード  */ ] =
{
	{ AKEYCODE_DEL,				KEY_INPUT_BACK },		// BackSpaceキー
	{ AKEYCODE_TAB,				KEY_INPUT_TAB },		// Tabキー
	{ AKEYCODE_ENTER,			KEY_INPUT_RETURN },		// Enterキー

	{ AKEYCODE_SHIFT_LEFT,		KEY_INPUT_LSHIFT },		// 左Shiftキー
	{ AKEYCODE_SHIFT_RIGHT,		KEY_INPUT_RSHIFT },		// 右Shiftキー
	{ AKEYCODE_CTRL_LEFT,		KEY_INPUT_LCONTROL },	// 左Ctrlキー
	{ AKEYCODE_CTRL_RIGHT,		KEY_INPUT_RCONTROL },	// 右Ctrlキー
	{ AKEYCODE_ESCAPE,			KEY_INPUT_ESCAPE },		// Escキー
	{ AKEYCODE_SPACE,			KEY_INPUT_SPACE },		// スペースキー
	{ AKEYCODE_PAGE_UP,			KEY_INPUT_PGUP },		// PageUpキー
	{ AKEYCODE_PAGE_DOWN,		KEY_INPUT_PGDN },		// PageDownキー
	{ AKEYCODE_MOVE_END,		KEY_INPUT_END },		// Endキー
	{ AKEYCODE_MOVE_HOME,		KEY_INPUT_HOME },		// Homeキー
	{ AKEYCODE_DPAD_LEFT,		KEY_INPUT_LEFT },		// 左キー
	{ AKEYCODE_DPAD_UP,			KEY_INPUT_UP },			// 上キー
	{ AKEYCODE_DPAD_RIGHT,		KEY_INPUT_RIGHT },		// 右キー
	{ AKEYCODE_DPAD_DOWN,		KEY_INPUT_DOWN },		// 下キー
	{ AKEYCODE_INSERT,			KEY_INPUT_INSERT },		// Insertキー
	{ AKEYCODE_FORWARD_DEL,		KEY_INPUT_DELETE },		// Deleteキー

	{ AKEYCODE_MINUS,			KEY_INPUT_MINUS },		// －キー
	{ AKEYCODE_YEN,				KEY_INPUT_YEN },		// ￥キー
	{ AKEYCODE_EQUALS,			KEY_INPUT_PREVTRACK },	// ＾キー
	{ AKEYCODE_PERIOD,			KEY_INPUT_PERIOD },		// ．キー
	{ AKEYCODE_SLASH,			KEY_INPUT_SLASH },		// ／キー
	{ AKEYCODE_ALT_LEFT,		KEY_INPUT_LALT },		// 左Altキー
	{ AKEYCODE_ALT_RIGHT,		KEY_INPUT_RALT },		// 右Altキー
	{ AKEYCODE_SCROLL_LOCK,		KEY_INPUT_SCROLL },		// ScrollLockキー
	{ AKEYCODE_SEMICOLON,		KEY_INPUT_SEMICOLON },	// ；キー
	{ AKEYCODE_APOSTROPHE,		KEY_INPUT_COLON },		// ：キー
	{ AKEYCODE_LEFT_BRACKET,	KEY_INPUT_LBRACKET },	// ［キー
	{ AKEYCODE_RIGHT_BRACKET,	KEY_INPUT_RBRACKET },	// ］キー
	{ AKEYCODE_AT,				KEY_INPUT_AT },			// ＠キー
	{ AKEYCODE_RO,				KEY_INPUT_BACKSLASH },	// ＼キー
	{ AKEYCODE_COMMA,			KEY_INPUT_COMMA },		// ，キー
	{ AKEYCODE_ZENKAKU_HANKAKU,	KEY_INPUT_KANJI },		// 漢字キー
	{ AKEYCODE_HENKAN,			KEY_INPUT_CONVERT },	// 変換キー
	{ AKEYCODE_MUHENKAN,		KEY_INPUT_NOCONVERT },	// 無変換キー
	{ AKEYCODE_KANA,			KEY_INPUT_KANA },		// カナキー
	{ AKEYCODE_SEARCH,			KEY_INPUT_APPS },		// アプリケーションメニューキー
	{ AKEYCODE_CAPS_LOCK,		KEY_INPUT_CAPSLOCK },	// CaspLockキー
	{ AKEYCODE_SYSRQ,			KEY_INPUT_SYSRQ },		// PrintScreenキー
	{ AKEYCODE_BREAK,			KEY_INPUT_PAUSE },		// PauseBreakキー
//	{							KEY_INPUT_LWIN },		// 左Winキー
//	{							KEY_INPUT_RWIN },		// 右Winキー

	{ AKEYCODE_NUM_LOCK,		KEY_INPUT_NUMLOCK },	// テンキーNumLockキー
	{ AKEYCODE_NUMPAD_0,		KEY_INPUT_NUMPAD0 },	// テンキー０
	{ AKEYCODE_NUMPAD_1,		KEY_INPUT_NUMPAD1 },	// テンキー１
	{ AKEYCODE_NUMPAD_2,		KEY_INPUT_NUMPAD2 },	// テンキー２
	{ AKEYCODE_NUMPAD_3,		KEY_INPUT_NUMPAD3 },	// テンキー３
	{ AKEYCODE_NUMPAD_4,		KEY_INPUT_NUMPAD4 },	// テンキー４
	{ AKEYCODE_NUMPAD_5,		KEY_INPUT_NUMPAD5 },	// テンキー５
	{ AKEYCODE_NUMPAD_6,		KEY_INPUT_NUMPAD6 },	// テンキー６
	{ AKEYCODE_NUMPAD_7,		KEY_INPUT_NUMPAD7 },	// テンキー７
	{ AKEYCODE_NUMPAD_8,		KEY_INPUT_NUMPAD8 },	// テンキー８
	{ AKEYCODE_NUMPAD_9,		KEY_INPUT_NUMPAD9 },	// テンキー９
	{ AKEYCODE_NUMPAD_MULTIPLY,	KEY_INPUT_MULTIPLY },	// テンキー＊キー
	{ AKEYCODE_NUMPAD_ADD,		KEY_INPUT_ADD },		// テンキー＋キー
	{ AKEYCODE_NUMPAD_SUBTRACT,	KEY_INPUT_SUBTRACT },	// テンキー－キー
	{ AKEYCODE_NUMPAD_DOT,		KEY_INPUT_DECIMAL },	// テンキー．キー
	{ AKEYCODE_NUMPAD_DIVIDE,	KEY_INPUT_DIVIDE },		// テンキー／キー
	{ AKEYCODE_NUMPAD_ENTER,	KEY_INPUT_NUMPADENTER },// テンキーのエンターキー

	{ AKEYCODE_F1,				KEY_INPUT_F1 },			// Ｆ１キー
	{ AKEYCODE_F2,				KEY_INPUT_F2 },			// Ｆ２キー
	{ AKEYCODE_F3,				KEY_INPUT_F3 },			// Ｆ３キー
	{ AKEYCODE_F4,				KEY_INPUT_F4 },			// Ｆ４キー
	{ AKEYCODE_F5,				KEY_INPUT_F5 },			// Ｆ５キー
	{ AKEYCODE_F6,				KEY_INPUT_F6 },			// Ｆ６キー
	{ AKEYCODE_F7,				KEY_INPUT_F7 },			// Ｆ７キー
	{ AKEYCODE_F8,				KEY_INPUT_F8 },			// Ｆ８キー
	{ AKEYCODE_F9,				KEY_INPUT_F9 },			// Ｆ９キー
	{ AKEYCODE_F10,				KEY_INPUT_F10 },		// Ｆ１０キー
	{ AKEYCODE_F11,				KEY_INPUT_F11 },		// Ｆ１１キー
	{ AKEYCODE_F12,				KEY_INPUT_F12 },		// Ｆ１２キー

	{ AKEYCODE_A,				KEY_INPUT_A },			// Ａキー
	{ AKEYCODE_B,				KEY_INPUT_B },			// Ｂキー
	{ AKEYCODE_C,				KEY_INPUT_C },			// Ｃキー
	{ AKEYCODE_D,				KEY_INPUT_D },			// Ｄキー
	{ AKEYCODE_E,				KEY_INPUT_E },			// Ｅキー
	{ AKEYCODE_F,				KEY_INPUT_F },			// Ｆキー
	{ AKEYCODE_G,				KEY_INPUT_G },			// Ｇキー
	{ AKEYCODE_H,				KEY_INPUT_H },			// Ｈキー
	{ AKEYCODE_I,				KEY_INPUT_I },			// Ｉキー
	{ AKEYCODE_J,				KEY_INPUT_J },			// Ｊキー
	{ AKEYCODE_K,				KEY_INPUT_K },			// Ｋキー
	{ AKEYCODE_L,				KEY_INPUT_L },			// Ｌキー
	{ AKEYCODE_M,				KEY_INPUT_M },			// Ｍキー
	{ AKEYCODE_N,				KEY_INPUT_N },			// Ｎキー
	{ AKEYCODE_O,				KEY_INPUT_O },			// Ｏキー
	{ AKEYCODE_P,				KEY_INPUT_P },			// Ｐキー
	{ AKEYCODE_Q,				KEY_INPUT_Q },			// Ｑキー
	{ AKEYCODE_R,				KEY_INPUT_R },			// Ｒキー
	{ AKEYCODE_S,				KEY_INPUT_S },			// Ｓキー
	{ AKEYCODE_T,				KEY_INPUT_T },			// Ｔキー
	{ AKEYCODE_U,				KEY_INPUT_U },			// Ｕキー
	{ AKEYCODE_V,				KEY_INPUT_V },			// Ｖキー
	{ AKEYCODE_W,				KEY_INPUT_W },			// Ｗキー
	{ AKEYCODE_X,				KEY_INPUT_X },			// Ｘキー
	{ AKEYCODE_Y,				KEY_INPUT_Y },			// Ｙキー
	{ AKEYCODE_Z,				KEY_INPUT_Z },			// Ｚキー

	{ AKEYCODE_0,				KEY_INPUT_0 },			// ０キー
	{ AKEYCODE_1,				KEY_INPUT_1 },			// １キー
	{ AKEYCODE_2,				KEY_INPUT_2 },			// ２キー
	{ AKEYCODE_3,				KEY_INPUT_3 },			// ３キー
	{ AKEYCODE_4,				KEY_INPUT_4 },			// ４キー
	{ AKEYCODE_5,				KEY_INPUT_5 },			// ５キー
	{ AKEYCODE_6,				KEY_INPUT_6 },			// ６キー
	{ AKEYCODE_7,				KEY_INPUT_7 },			// ７キー
	{ AKEYCODE_8,				KEY_INPUT_8 },			// ８キー
	{ AKEYCODE_9,				KEY_INPUT_9 },			// ９キー

	{ AKEYCODE_BACK,			KEY_INPUT_ESCAPE },		// Android の Backボタン

	{ 0xffff,					0xffff },
} ;

// 関数プロトタイプ宣言 -------------------------------------------------------

// プログラム------------------------------------------------------------------

// 入力システムを初期化する処理の環境依存処理
extern int InitializeInputSystem_PF_Timing0( void )
{
	static int NowInitialize = FALSE ;
	int i, j ;

	// 既に初期化処理が開始されている場合も何もせずに終了
	if( NowInitialize )
	{
		return 0 ;
	}

	// 初期化中フラグを立てる
	NowInitialize = TRUE ;

	// 無効ゾーンのセット
	for( i = 0 ; i < MAX_JOYPAD_NUM ; i ++ )
	{
		InputSysData.Pad[ i ].DeadZone = InputSysData.PadDefaultDeadZone ;
		InputSysData.Pad[ i ].DeadZoneD = InputSysData.PadDefaultDeadZoneD ;
	}

	// キーボードとジョイパッドの入力のデフォルトの対応表を設定する
	if( InputSysData.KeyToJoypadInputInitializeFlag == FALSE )
	{
		InputSysData.KeyToJoypadInputInitializeFlag = TRUE ;

		for( i = 0 ; i < MAX_JOYPAD_NUM ; i ++ )
		{
			for ( j = 0 ; j < 32 ; j ++ )
			{
				InputSysData.KeyToJoypadInput[ i ][ j ][ 0 ] = -1 ;
				InputSysData.KeyToJoypadInput[ i ][ j ][ 1 ] = -1 ;
			}
		}
		InputSysData.KeyToJoypadInput[ 0 ][  0 ][ 0 ] = KEY_INPUT_NUMPAD2 ;
		InputSysData.KeyToJoypadInput[ 0 ][  0 ][ 1 ] = KEY_INPUT_DOWN ;
		InputSysData.KeyToJoypadInput[ 0 ][  1 ][ 0 ] = KEY_INPUT_NUMPAD4 ;
		InputSysData.KeyToJoypadInput[ 0 ][  1 ][ 1 ] = KEY_INPUT_LEFT ;
		InputSysData.KeyToJoypadInput[ 0 ][  2 ][ 0 ] = KEY_INPUT_NUMPAD6 ;
		InputSysData.KeyToJoypadInput[ 0 ][  2 ][ 1 ] = KEY_INPUT_RIGHT ;
		InputSysData.KeyToJoypadInput[ 0 ][  3 ][ 0 ] = KEY_INPUT_NUMPAD8 ;
		InputSysData.KeyToJoypadInput[ 0 ][  3 ][ 1 ] = KEY_INPUT_UP ;
		InputSysData.KeyToJoypadInput[ 0 ][  4 ][ 0 ] = KEY_INPUT_Z ;
		InputSysData.KeyToJoypadInput[ 0 ][  5 ][ 0 ] = KEY_INPUT_X ;
		InputSysData.KeyToJoypadInput[ 0 ][  6 ][ 0 ] = KEY_INPUT_C ;
		InputSysData.KeyToJoypadInput[ 0 ][  7 ][ 0 ] = KEY_INPUT_A ;
		InputSysData.KeyToJoypadInput[ 0 ][  8 ][ 0 ] = KEY_INPUT_S ;
		InputSysData.KeyToJoypadInput[ 0 ][  9 ][ 0 ] = KEY_INPUT_D ;
		InputSysData.KeyToJoypadInput[ 0 ][ 10 ][ 0 ] = KEY_INPUT_Q ;
		InputSysData.KeyToJoypadInput[ 0 ][ 11 ][ 0 ] = KEY_INPUT_W ;
		InputSysData.KeyToJoypadInput[ 0 ][ 12 ][ 0 ] = KEY_INPUT_RETURN ;
		InputSysData.KeyToJoypadInput[ 0 ][ 13 ][ 0 ] = KEY_INPUT_SPACE ;
	}

	InputSysData.PF.UseInputInfoNum = 0 ;

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
	if( InputSysData.PF.SourceNum[ ANDR_INPUT_SOURCE_KEYBOARD ] > 0 )
	{
		int i, j ;

		for( j = 0 ; j < InputSysData.PF.SourceNum[ ANDR_INPUT_SOURCE_KEYBOARD ] ; j ++ )
		{
			INPUT_ANDROID_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.SourceNoToInputInfoTable[ ANDR_INPUT_SOURCE_KEYBOARD ][ j ] ] ;

			for( i = 0 ; g_AndroidKeyToDXInputKey[i][0] != 0xffff ; i ++ )
			{
				InputSysData.KeyInputBuf[ g_AndroidKeyToDXInputKey[ i ][ 1 ] ] |= Info->KeyState[ g_AndroidKeyToDXInputKey[ i ][ 0 ] ] ? 0x80 : 0x00 ;
			}
		}
	}

	return 0 ;
}

// パッドの入力状態の更新の環境依存処理
extern int UpdateJoypadInputState_PF( int PadNo )
{
	INPUTPADDATA *pad = &InputSysData.Pad[ PadNo ] ;
//	INPUT_ANDROID_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.SourceNoToInputInfoTable[ ANDR_INPUT_SOURCE_GAMEPAD ][ PadNo ] ] ;
	INPUT_ANDROID_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.GamePadSourceNoToInputInfoTable[ PadNo ] ] ;
	float DeadZone = pad->DeadZone / 65536.0f ;

//	if( InputSysData.PF.SourceNum[ ANDR_INPUT_SOURCE_GAMEPAD ] <= 0 )
	if( InputSysData.PF.GamePadSourceNum <= 0 )
	{
		return 0 ;
	}

	pad->State.X  = Info->AxisX  < DeadZone && Info->AxisX  > -DeadZone ? 0 : ( int )( Info->AxisX  * 1000.0f ) ;
	pad->State.Y  = Info->AxisY  < DeadZone && Info->AxisY  > -DeadZone ? 0 : ( int )( Info->AxisY  * 1000.0f ) ;
	pad->State.Z  = Info->AxisZ  < DeadZone && Info->AxisZ  > -DeadZone ? 0 : ( int )( Info->AxisZ  * 1000.0f ) ;
	pad->State.Rx = Info->AxisRx < DeadZone && Info->AxisRx > -DeadZone ? 0 : ( int )( Info->AxisRx * 1000.0f ) ;
	pad->State.Ry = Info->AxisRy < DeadZone && Info->AxisRy > -DeadZone ? 0 : ( int )( Info->AxisRy * 1000.0f ) ;
	pad->State.Rz = Info->AxisRz < DeadZone && Info->AxisRz > -DeadZone ? 0 : ( int )( Info->AxisRz * 1000.0f ) ;
	pad->State.Slider[ 0 ]   = ( int )( Info->AxisLTrigger * 1000.0f ) ;
	pad->State.Slider[ 1 ]   = ( int )( Info->AxisRTrigger * 1000.0f ) ;
	if( Info->AxisHatX > 0.5f )
	{
		if( Info->AxisHatY > 0.5f )
		{
			pad->State.POV[ 0 ] = 13500 ;
		}
		else
		if( Info->AxisHatY < -0.5f )
		{
			pad->State.POV[ 0 ] = 4500 ;
		}
		else
		{
			pad->State.POV[ 0 ] = 9000 ;
		}
	}
	else
	if( Info->AxisHatX < -0.5f )
	{
		if( Info->AxisHatY > 0.5f )
		{
			pad->State.POV[ 0 ] = 22500 ;
		}
		else
		if( Info->AxisHatY < -0.5f )
		{
			pad->State.POV[ 0 ] = 31500 ;
		}
		else
		{
			pad->State.POV[ 0 ] = 27000 ;
		}
	}
	else
	{
		if( Info->AxisHatY > 0.5f )
		{
			pad->State.POV[ 0 ] = 18000 ;
		}
		else
		if( Info->AxisHatY < -0.5f )
		{
			pad->State.POV[ 0 ] = 0 ;
		}
		else
		{
			pad->State.POV[ 0 ] = 0xffffffff ;
		}
	}
	pad->State.POV[ 1 ]      = 0xffffffff ;
	pad->State.POV[ 2 ]      = 0xffffffff ;
	pad->State.POV[ 3 ]      = 0xffffffff ;
	pad->State.Buttons[  0 ] = Info->KeyState[ AKEYCODE_BUTTON_A      ] ? 0x80 : 0x00 ;
	pad->State.Buttons[  1 ] = Info->KeyState[ AKEYCODE_BUTTON_B      ] ? 0x80 : 0x00 ;
	pad->State.Buttons[  2 ] = Info->KeyState[ AKEYCODE_BUTTON_X      ] ? 0x80 : 0x00 ;
	pad->State.Buttons[  3 ] = Info->KeyState[ AKEYCODE_BUTTON_Y      ] ? 0x80 : 0x00 ;
	pad->State.Buttons[  4 ] = Info->KeyState[ AKEYCODE_BUTTON_L1     ] ? 0x80 : 0x00 ;
	pad->State.Buttons[  5 ] = Info->KeyState[ AKEYCODE_BUTTON_R1     ] ? 0x80 : 0x00 ;
	pad->State.Buttons[  6 ] = Info->KeyState[ AKEYCODE_BUTTON_L2     ] ? 0x80 : 0x00 ;
	pad->State.Buttons[  7 ] = Info->KeyState[ AKEYCODE_BUTTON_R2     ] ? 0x80 : 0x00 ;
	pad->State.Buttons[  8 ] = Info->KeyState[ AKEYCODE_BUTTON_THUMBL ] ? 0x80 : 0x00 ;
	pad->State.Buttons[  9 ] = Info->KeyState[ AKEYCODE_BUTTON_THUMBR ] ? 0x80 : 0x00 ;
	pad->State.Buttons[ 10 ] = Info->KeyState[ AKEYCODE_BUTTON_SELECT ] ? 0x80 : 0x00 ;
	pad->State.Buttons[ 11 ] = Info->KeyState[ AKEYCODE_BUTTON_START  ] ? 0x80 : 0x00 ;
	pad->State.Buttons[ 12 ] = 0 ;
	pad->State.Buttons[ 13 ] = 0 ;
	pad->State.Buttons[ 14 ] = 0 ;
	pad->State.Buttons[ 15 ] = 0 ;
	pad->State.Buttons[ 16 ] = 0 ;
	pad->State.Buttons[ 17 ] = 0 ;
	pad->State.Buttons[ 18 ] = 0 ;
	pad->State.Buttons[ 19 ] = 0 ;
	pad->State.Buttons[ 20 ] = 0 ;
	pad->State.Buttons[ 21 ] = 0 ;
	pad->State.Buttons[ 22 ] = 0 ;
	pad->State.Buttons[ 23 ] = 0 ;
	pad->State.Buttons[ 24 ] = 0 ;
	pad->State.Buttons[ 25 ] = 0 ;
	pad->State.Buttons[ 26 ] = 0 ;
	pad->State.Buttons[ 27 ] = 0 ;
	pad->State.Buttons[ 28 ] = 0 ;
	pad->State.Buttons[ 29 ] = 0 ;
	pad->State.Buttons[ 30 ] = 0 ;
	pad->State.Buttons[ 31 ] = 0 ;

//	if( Info->KeyState[ AKEYCODE_DPAD_LEFT  ] != 0 ) pad->State.X = -1000 ;
//	if( Info->KeyState[ AKEYCODE_DPAD_RIGHT ] != 0 ) pad->State.X =  1000 ;
//	if( Info->KeyState[ AKEYCODE_DPAD_UP    ] != 0 ) pad->State.Y = -1000 ;
//	if( Info->KeyState[ AKEYCODE_DPAD_DOWN  ] != 0 ) pad->State.Y =  1000 ;
//
//	if( Info->KeyState[ AKEYCODE_J          ] != 0 ) pad->State.Z  = -1000 ;
//	if( Info->KeyState[ AKEYCODE_L          ] != 0 ) pad->State.Z  =  1000 ;
//	if( Info->KeyState[ AKEYCODE_I          ] != 0 ) pad->State.Rz = -1000 ;
//	if( Info->KeyState[ AKEYCODE_K          ] != 0 ) pad->State.Rz =  1000 ;
//
//	if( Info->KeyState[ AKEYCODE_2          ] != 0 ) pad->State.Buttons[  0 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_3          ] != 0 ) pad->State.Buttons[  1 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_1          ] != 0 ) pad->State.Buttons[  2 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_4          ] != 0 ) pad->State.Buttons[  3 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_5          ] != 0 ) pad->State.Buttons[  4 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_6          ] != 0 ) pad->State.Buttons[  5 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_7          ] != 0 ) pad->State.Buttons[  6 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_8          ] != 0 ) pad->State.Buttons[  7 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_9          ] != 0 ) pad->State.Buttons[  8 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_0          ] != 0 ) pad->State.Buttons[  9 ] = 0x80 ;
//
//	if( Info->KeyState[ AKEYCODE_DEL        ] != 0 ) pad->State.Buttons[ 10 ] = 0x80 ;
//	if( Info->KeyState[ AKEYCODE_ENTER      ] != 0 ) pad->State.Buttons[ 11 ] = 0x80 ;
//
//	if( Info->KeyState[ AKEYCODE_D ] )
//	{
//		if( Info->KeyState[ AKEYCODE_S ] )
//		{
//			pad->State.POV[ 0 ] = 13500 ;
//		}
//		else
//		if( Info->KeyState[ AKEYCODE_W ] )
//		{
//			pad->State.POV[ 0 ] = 4500 ;
//		}
//		else
//		{
//			pad->State.POV[ 0 ] = 9000 ;
//		}
//	}
//	else
//	if( Info->KeyState[ AKEYCODE_A ] )
//	{
//		if( Info->KeyState[ AKEYCODE_S ] )
//		{
//			pad->State.POV[ 0 ] = 22500 ;
//		}
//		else
//		if( Info->KeyState[ AKEYCODE_W ] )
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
//		if( Info->KeyState[ AKEYCODE_S ] )
//		{
//			pad->State.POV[ 0 ] = 18000 ;
//		}
//		else
//		if( Info->KeyState[ AKEYCODE_W ] )
//		{
//			pad->State.POV[ 0 ] = 0 ;
//		}
//	}

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
	int i ;

	for( i = 0 ; i < InputSysData.PF.SourceNum[ ANDR_INPUT_SOURCE_MOUSE ] ; i ++ )
	{
		INPUT_ANDROID_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.SourceNoToInputInfoTable[ ANDR_INPUT_SOURCE_MOUSE ][ i ] ] ;

		if( ( Info->ButtonState & 0x01 ) != 0 ) res |= MOUSE_INPUT_1 ;
		if( ( Info->ButtonState & 0x02 ) != 0 ) res |= MOUSE_INPUT_2 ;
		if( ( Info->ButtonState & 0x04 ) != 0 ) res |= MOUSE_INPUT_3 ;
		if( ( Info->ButtonState & 0x08 ) != 0 ) res |= MOUSE_INPUT_4 ;
		if( ( Info->ButtonState & 0x10 ) != 0 ) res |= MOUSE_INPUT_5 ;
		if( ( Info->ButtonState & 0x20 ) != 0 ) res |= MOUSE_INPUT_6 ;
		if( ( Info->ButtonState & 0x40 ) != 0 ) res |= MOUSE_INPUT_7 ;
		if( ( Info->ButtonState & 0x80 ) != 0 ) res |= MOUSE_INPUT_8 ;
	}

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
		*XBuf = 0 ;
	}

	if( YBuf != NULL )
	{
		*YBuf = 0 ;
	}

	if( InputSysData.PF.SourceNum[ ANDR_INPUT_SOURCE_MOUSE ] > 0 )
	{
		int ScreenX, ScreenY ;
		INPUT_ANDROID_DEVICE_INFO *Info = &InputSysData.PF.InputInfo[ InputSysData.PF.SourceNoToInputInfoTable[ ANDR_INPUT_SOURCE_MOUSE ][ 0 ] ] ;

		ScreenX = ( int )Info->AxisX ;
		ScreenY = ( int )Info->AxisY ;

		ScreenX = ( int )InputSysData.PF.MouseX ;
		ScreenY = ( int )InputSysData.PF.MouseY ;
		ConvScreenPositionToDxScreenPosition( ScreenX, ScreenY, XBuf, YBuf ) ;
	}

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
extern int GetAndroidDeviceIdToInputInfoNo( int32_t Source, int32_t DeviceId )
{
	int i ;
//	int Src ;
//
//	for( Src = 0 ; Src < ANDR_INPUT_SOURCE_NUM ; Src ++ )
//	{
//		if( g_AndroidInputSourceTable[ Src ] == Source )
//		{
//			break ;
//		}
//	}
//	if( Src == ANDR_INPUT_SOURCE_NUM )
//	{
//		return -1 ;
//	}

	for( i = 0 ; i < InputSysData.PF.UseInputInfoNum ; i ++ )
	{
		if( /* InputSysData.PF.InputInfo[ i ].Source   == Source   && */
			   InputSysData.PF.InputInfo[ i ].DeviceId == DeviceId )
		{
			int UpdateFlag = FALSE ;

			if( ( InputSysData.PF.InputInfo[ i ].Source & Source ) != Source )
			{
				UpdateFlag = TRUE ;
			}

			InputSysData.PF.InputInfo[ i ].Source |= Source ;
			InputSysData.PF.InputInfo[ i ].UpdateCount = InputSysData.PF.UpdateCount ;
			InputSysData.PF.UpdateCount ++ ;

			if( UpdateFlag )
			{
				RefreshAndroidSourceNoToInputInfoTable( Source ) ;
				RefreshAndroidGamePadSourceNoToInputInfoTable() ;
			}
			return i ;
		}
	}

	if( InputSysData.PF.UseInputInfoNum == ANDR_DEVICE_MAX_NUM )
	{
		int MinUpdateCountNo ;

		MinUpdateCountNo = 0 ;
		for( i = 1 ; i < InputSysData.PF.UseInputInfoNum ; i ++ )
		{
			if( InputSysData.PF.InputInfo[ MinUpdateCountNo ].UpdateCount > InputSysData.PF.InputInfo[ i ].UpdateCount )
			{
				MinUpdateCountNo = i ;
			}
		}

		_MEMSET( &InputSysData.PF.InputInfo[ MinUpdateCountNo ], 0, sizeof( InputSysData.PF.InputInfo[ MinUpdateCountNo ] ) ) ;
		InputSysData.PF.InputInfo[ MinUpdateCountNo ].Source = Source ;
		InputSysData.PF.InputInfo[ MinUpdateCountNo ].DeviceId = DeviceId ;
		InputSysData.PF.InputInfo[ MinUpdateCountNo ].UpdateCount = InputSysData.PF.UpdateCount ;
		InputSysData.PF.UpdateCount ++ ;

		RefreshAndroidSourceNoToInputInfoTable( Source ) ;
		RefreshAndroidGamePadSourceNoToInputInfoTable() ;

		return MinUpdateCountNo ;
	}

	i = InputSysData.PF.UseInputInfoNum ;
	InputSysData.PF.UseInputInfoNum ++ ;

	_MEMSET( &InputSysData.PF.InputInfo[ i ], 0, sizeof( InputSysData.PF.InputInfo[ i ] ) ) ;
	InputSysData.PF.InputInfo[ i ].Source = Source ;
	InputSysData.PF.InputInfo[ i ].DeviceId = DeviceId ;
	InputSysData.PF.InputInfo[ i ].UpdateCount = InputSysData.PF.UpdateCount ;
	InputSysData.PF.UpdateCount ++ ;

	RefreshAndroidSourceNoToInputInfoTable( Source ) ;
	RefreshAndroidGamePadSourceNoToInputInfoTable() ;
	
	return i ;
}

// 入力ソース番号と入力情報との対応テーブルを更新する
extern int RefreshAndroidSourceNoToInputInfoTable( int32_t Source )
{
	INPUT_ANDROID_DEVICE_INFO *Info ;
	int *SourceNoTable ;
	int i, j ;
	int Num ;
	int NoTable[ ANDR_DEVICE_MAX_NUM ] ;
	INPUT_ANDROID_DEVICE_INFO *InfoTable[ ANDR_DEVICE_MAX_NUM ] ;
	int Src ;

	for( Src = 0 ; Src < ANDR_INPUT_SOURCE_NUM ; Src ++ )
	{
		if( Source == g_AndroidInputSourceTable[ Src ] )
		{
			break ;
		}
	}
	if( Src == ANDR_INPUT_SOURCE_NUM )
	{
		return -1 ;
	}

	Num = 0 ;
	Info = InputSysData.PF.InputInfo ;
	for( i = 0 ; i < InputSysData.PF.UseInputInfoNum ; i ++, Info ++ )
	{
		if( ( Info->Source & Source ) == Source )
		{
			InfoTable[ Num ] = Info ;
			Num ++ ;
		}
	}
	InputSysData.PF.SourceNum[ Src ] = Num ;

	if( Num == 0 )
	{
		return 0 ;
	}

	NoTable[ 0 ] = 0 ;
	for( i = 1 ; i < Num ; i ++ )
	{
		for( j = 0 ; j < i ; j ++ )
		{
			if( InfoTable[ NoTable[ j ] ]->DeviceId < InfoTable[ i ]->DeviceId )
			{
				_MEMMOVE( &NoTable[ j + 1 ], &NoTable[ j ], sizeof( int ) * ( i - j ) ) ;
				NoTable[ j ] = i ;
				break ;
			}
		}
		if( j == i )
		{
			NoTable[ j ] = i ;
		}
	}

	SourceNoTable = InputSysData.PF.SourceNoToInputInfoTable[ Src ] ;
	for( i = 0 ; i < Num ; i ++ )
	{
		SourceNoTable[ i ] = InfoTable[ NoTable[ i ] ] - InputSysData.PF.InputInfo ;
	}

	// 終了
	return 0 ;
}

// ゲームパッドの番号と入力情報との対応テーブルを更新する
extern int RefreshAndroidGamePadSourceNoToInputInfoTable( void )
{
	INPUT_ANDROID_DEVICE_INFO *Info ;
	int *SourceNoTable ;
	int i, j ;
	int Num ;
	int NoTable[ ANDR_DEVICE_MAX_NUM ] ;
	INPUT_ANDROID_DEVICE_INFO *InfoTable[ ANDR_DEVICE_MAX_NUM ] ;

	Num = 0 ;
	Info = InputSysData.PF.InputInfo ;
	for( i = 0 ; i < InputSysData.PF.UseInputInfoNum ; i ++, Info ++ )
	{
		// ＤＸライブラリがゲームパッドとして扱う要素を持っている場合はゲームパッドとする
		if( ( ( Info->Source & AINPUT_SOURCE_GAMEPAD  ) == AINPUT_SOURCE_GAMEPAD ) ||
			( ( Info->Source & AINPUT_SOURCE_JOYSTICK ) == AINPUT_SOURCE_JOYSTICK ) )
		{
			InfoTable[ Num ] = Info ;
			Num ++ ;
		}
	}
	InputSysData.PF.GamePadSourceNum = Num ;

	if( Num == 0 )
	{
		return 0 ;
	}

	NoTable[ 0 ] = 0 ;
	for( i = 1 ; i < Num ; i ++ )
	{
		for( j = 0 ; j < i ; j ++ )
		{
			if( InfoTable[ NoTable[ j ] ]->DeviceId < InfoTable[ i ]->DeviceId )
			{
				_MEMMOVE( &NoTable[ j + 1 ], &NoTable[ j ], sizeof( int ) * ( i - j ) ) ;
				NoTable[ j ] = i ;
				break ;
			}
		}
		if( j == i )
		{
			NoTable[ j ] = i ;
		}
	}

	SourceNoTable = InputSysData.PF.GamePadSourceNoToInputInfoTable ;
	for( i = 0 ; i < Num ; i ++ )
	{
		SourceNoTable[ i ] = InfoTable[ NoTable[ i ] ] - InputSysData.PF.InputInfo ;
	}

	InputSysData.PadNum = Num > MAX_JOYPAD_NUM ? MAX_JOYPAD_NUM : Num ;

	// 終了
	return 0 ;
}

// 入力イベントを処理する
extern int32_t ProcessInputEvent( AInputEvent* event )
{
	int32_t InputType ;
	int32_t Source ;
	int32_t DeviceId ;
	int InputNo ;

	Source		= AInputEvent_getSource( event ) ;
	InputType	= AInputEvent_getType( event ) ;
	DeviceId	= AInputEvent_getDeviceId( event ) ;

	InputNo = GetAndroidDeviceIdToInputInfoNo( Source, DeviceId ) ;

	if( InputType == AINPUT_EVENT_TYPE_MOTION )
	{
		TOUCHINPUTDATA TouchInputData ;
		int32_t PointerCount ;
		int32_t Action ;
		int32_t PIndex ;
		Action = AMotionEvent_getAction( event ) ;
		PIndex = ( Action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK ) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT ;
		Action &= AMOTION_EVENT_ACTION_MASK ;

		if( ( Source & AINPUT_SOURCE_TOUCHSCREEN ) == AINPUT_SOURCE_TOUCHSCREEN ||
			( Source & AINPUT_SOURCE_TOUCHPAD    ) == AINPUT_SOURCE_TOUCHPAD )
		{
			TouchInputData.PointNum = 0 ;
			TouchInputData.Time = ( LONGLONG )AMotionEvent_getEventTime( event ) ;
			TouchInputData.Source = Source ;

			if( Action != AMOTION_EVENT_ACTION_UP &&
				Action != AMOTION_EVENT_ACTION_HOVER_ENTER &&
				Action != AMOTION_EVENT_ACTION_HOVER_EXIT &&
				Action != AMOTION_EVENT_ACTION_HOVER_MOVE )
			{
				PointerCount = AMotionEvent_getPointerCount( event ) ;
				if( PointerCount > 0 )
				{
					int32_t i ;
					TOUCHINPUTPOINT *TouchPoint ;

					TouchPoint = TouchInputData.Point ;
					for( i = 0; i < PointerCount; i++ )
					{
						int ScreenX, ScreenY ;

						if( PIndex == i && Action == AMOTION_EVENT_ACTION_POINTER_UP )
						{
							continue ;
						}

						TouchPoint->Device = 0 ;
						TouchPoint->ID = AMotionEvent_getPointerId( event, i ) ;
						ScreenX = AMotionEvent_getX( event, i ) ;
						ScreenY = AMotionEvent_getY( event, i ) ;
						ConvScreenPositionToDxScreenPosition( ScreenX, ScreenY, &TouchPoint->PositionX, &TouchPoint->PositionY ) ;

						TouchPoint->Pressure    = AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_PRESSURE,    i ) ;
						TouchPoint->Orientation = AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_ORIENTATION, i ) ;
						TouchPoint->Tilt        = AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_TILT,        i ) ;
						TouchPoint->ToolType    = AMotionEvent_getToolType( event, i ) ;

						TouchInputData.PointNum ++ ;
						TouchPoint ++ ;
					}
				}
			}

			AddTouchInputData( &TouchInputData ) ;
		}

		if( ( Source & AINPUT_SOURCE_STYLUS      ) == AINPUT_SOURCE_STYLUS &&
			( Source & AINPUT_SOURCE_TOUCHSCREEN ) != AINPUT_SOURCE_TOUCHSCREEN )
		{
			TouchInputData.PointNum = 0 ;
			TouchInputData.Time = ( LONGLONG )AMotionEvent_getEventTime( event ) ;
			TouchInputData.Source = Source ;

			if( Action != AMOTION_EVENT_ACTION_HOVER_EXIT )
			{
				PointerCount = AMotionEvent_getPointerCount( event ) ;
				if( PointerCount > 0 )
				{
					int32_t i ;
					TOUCHINPUTPOINT *TouchPoint ;

					TouchPoint = TouchInputData.Point ;
					for( i = 0; i < PointerCount; i++ )
					{
						int ScreenX, ScreenY ;

						if( PIndex == i && Action == AMOTION_EVENT_ACTION_POINTER_UP )
						{
							continue ;
						}

						TouchPoint->Device = 0 ;
						TouchPoint->ID = AMotionEvent_getPointerId( event, i ) ;
						ScreenX = AMotionEvent_getX( event, i ) ;
						ScreenY = AMotionEvent_getY( event, i ) ;
						ConvScreenPositionToDxScreenPosition( ScreenX, ScreenY, &TouchPoint->PositionX, &TouchPoint->PositionY ) ;

						TouchPoint->Pressure    = AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_PRESSURE,    i ) ;
						TouchPoint->Orientation = AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_ORIENTATION, i ) ;
						TouchPoint->Tilt        = AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_TILT,        i ) ;
						TouchPoint->ToolType    = AMotionEvent_getToolType( event, i ) ;

						if( TouchPoint->ToolType != 0 )
						{
							TouchInputData.PointNum ++ ;
							TouchPoint ++ ;
						}
					}
				}
			}

			AddTouchInputData( &TouchInputData ) ;
		}

		if( ( Source & AINPUT_SOURCE_MOUSE ) == AINPUT_SOURCE_MOUSE )
		{
			if( InputNo >= 0 && InputNo < ANDR_DEVICE_MAX_NUM )
			{
				int LogType = -1 ;

				if( PIndex >= 0 && PIndex < ANDR_POINTER_MAX_NUM )
				{
					if( Action == AMOTION_EVENT_ACTION_POINTER_DOWN )
					{
						LogType = MOUSE_INPUT_LOG_DOWN ;
						InputSysData.PF.InputInfo[ InputNo ].PointerState[ PIndex ] = 1 ;
					}
					else
					if( Action == AMOTION_EVENT_ACTION_POINTER_UP )
					{
						LogType = MOUSE_INPUT_LOG_UP ;
						InputSysData.PF.InputInfo[ InputNo ].PointerState[ PIndex ] = 0 ;
					}
				}

				InputSysData.PF.InputInfo[ InputNo ].ButtonState	= AMotionEvent_getButtonState( event ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisX			= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_X,			0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisY			= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_Y,			0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].Wheel			= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_WHEEL,		0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].VScroll		= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_VSCROLL,		0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].HScroll		= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_HSCROLL,		0 ) ;

				InputSysData.PF.MouseX = InputSysData.PF.InputInfo[ InputNo ].AxisX ;
				InputSysData.PF.MouseY = InputSysData.PF.InputInfo[ InputNo ].AxisY ;

				InputSysData.MouseMoveZ  += InputSysData.PF.InputInfo[ InputNo ].VScroll ;
				InputSysData.MouseMoveHZ += InputSysData.PF.InputInfo[ InputNo ].HScroll ;

				// ログを追加
				if( LogType != -1 )
				{
					int MouseX, MouseY ;
					int i ;
					int Button ;

					ConvScreenPositionToDxScreenPosition(
						InputSysData.PF.InputInfo[ InputNo ].AxisX,
						InputSysData.PF.InputInfo[ InputNo ].AxisY,
						&MouseX,
						&MouseY
					) ;

					for( i = 0 ; i < 8 ; i ++ )
					{
						Button = 1 << i ;
						if( ( InputSysData.PF.InputInfo[ InputNo ].ButtonState & Button ) != 0 )
						{
							if( LogType == MOUSE_INPUT_LOG_DOWN )
							{
								StockMouseInputLogData( Button, MouseX, MouseY ) ;
							}
							StockMouseInputLogData2( Button, MouseX, MouseY, LogType ) ;
						}
					}
				}
			}

			// マウス入力があった際はタップの入力を無効化する( スタイラスペンの場合は無効化しない )
			if( ( Source & AINPUT_SOURCE_STYLUS ) == 0 )
			{
				TouchInputData.PointNum = 0 ;
				TouchInputData.Time = ( LONGLONG )AMotionEvent_getEventTime( event ) ;

				AddTouchInputData( &TouchInputData ) ;
			}
		}

		if( ( Source & AINPUT_SOURCE_JOYSTICK ) == AINPUT_SOURCE_JOYSTICK )
		{
			if( InputNo >= 0 && InputNo < ANDR_DEVICE_MAX_NUM )
			{
				InputSysData.PF.InputInfo[ InputNo ].AxisX			= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_X,			0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisY			= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_Y,			0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisZ			= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_Z,			0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisRx			= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_RX,			0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisRy			= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_RY,			0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisRz			= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_RZ,			0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisHatX		= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_HAT_X,		0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisHatY		= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_HAT_Y,		0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisLTrigger	= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_LTRIGGER,	0 ) ;
				InputSysData.PF.InputInfo[ InputNo ].AxisRTrigger	= AMotionEvent_getAxisValue( event, AMOTION_EVENT_AXIS_RTRIGGER,	0 ) ;
			}
		}

		return 1 ;
	}
	else
	if( InputType == AINPUT_EVENT_TYPE_KEY )
	{
		int32_t Action ;
		int32_t KeyCode ;
		int32_t res = 1 ; 

		Action = AKeyEvent_getAction( event ) ;
		KeyCode = AKeyEvent_getKeyCode( event ) ;
		if( InputNo >= 0 && InputNo < ANDR_DEVICE_MAX_NUM )
		{
			if( KeyCode < ANDR_KEYCODE_MAX )
			{
				if( Action == AKEY_EVENT_ACTION_DOWN )
				{
					InputSysData.PF.InputInfo[ InputNo ].KeyState[ KeyCode ] = 1 ;
				}
				else
				if( Action == AKEY_EVENT_ACTION_UP )
				{
					InputSysData.PF.InputInfo[ InputNo ].KeyState[ KeyCode ] = 0 ;
				}
			}
		}

		if( KeyCode == AKEYCODE_VOLUME_UP   ||
			KeyCode == AKEYCODE_VOLUME_DOWN ||
			KeyCode == AKEYCODE_POWER       ||
			KeyCode == AKEYCODE_HOME        ||
			KeyCode == AKEYCODE_CAMERA )
		{
			res = 0 ;
		}

		return res ;
	}

	return 0 ;
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_INPUT



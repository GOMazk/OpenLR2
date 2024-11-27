// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		ヘッダファイル
// 
// 				Ver 3.02
// 
// -------------------------------------------------------------------------------

#ifndef __DXLIB
#define __DXLIB

// ＤＸライブラリのバージョン
#define DXLIB_VERSION 0x3020
#define DXLIB_VERSION_STR "3.02"

// 設定 -----------------------------------------------------------------------

// ＤＸライブラリに必要な lib ファイルを、プロジェクトのカレントフォルダや
// コンパイラのデフォルト LIB パスに設定せずに使用される場合は以下の
// コメントを外してください
//#define DX_LIB_NOT_DEFAULTPATH

#ifndef __DX_MAKE

// 描画関連の関数を一切使用されない場合は以下のコメントを外して下さい
//#define DX_NOTUSE_DRAWFUNCTION

#endif

// 型定義 ---------------------------------------------------------------------

// ＤｉｒｅｃｔＩｎｐｕｔのバージョン設定
#define DIRECTINPUT_VERSION 0x700

#include <windows.h>
#include <stdio.h>

#ifndef DWORD_PTR
#define DWORD_PTR	DWORD
#endif

#ifndef LONG_PTR
#define LONG_PTR	DWORD
#endif

// ＤｉｒｅｃｔＸ関連定義部 ---------------------------------------------------

#include "DxDirectX.h"
/*
#ifdef DX_USE_DIRECTX_SDK_FILE
	#include <dinput.h>
	#include <ddraw.h>
	#include <d3d.h>
	#ifndef DX_NON_MOVIE
		#include <dshow.h>
	#endif
	#include <qedit.h>
	#include <stdio.h>
	#include <dsound.h>
	#include <dmusici.h>
#else
	#include "DxDirectX.h"
#endif
*/

// ライブラリリンク定義--------------------------------------------------------

#ifndef __DX_MAKE
	#ifndef DX_LIB_NOT_DEFAULTPATH
		#ifdef _DEBUG
			#pragma comment( lib, "DxLib_d.lib"		)			//  ＤＸライブラリ使用指定
			#pragma comment( lib, "DxUseCLib_d.lib"		)		//  標準Ｃライブラリを使用する部分の lib ファイルの使用指定
			#pragma comment( linker, "/NODEFAULTLIB:libcmt.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
//			#pragma comment( linker, "/NODEFAULTLIB:msvcrt.lib" )
//			#pragma comment( linker, "/NODEFAULTLIB:msvcrtd.lib" )
		#else
			#pragma comment( lib, "DxLib.lib"		)			//  ＤＸライブラリ使用指定
			#pragma comment( lib, "DxUseCLib.lib"		)		//  標準Ｃライブラリを使用する部分の lib ファイルの使用指定
			#pragma comment( linker, "/NODEFAULTLIB:libcmtd.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
//			#pragma comment( linker, "/NODEFAULTLIB:msvcrt.lib" )
//			#pragma comment( linker, "/NODEFAULTLIB:msvcrtd.lib" )
		#endif
		//#pragma comment( lib, "GetDirectXVer" )
		//#pragma comment( lib, "ddraw.lib"		)				//  DirectDrawライブラリ
		//#pragma comment( lib, "dinput.lib"		)			//  DirectInputライブラリ
		//#pragma comment( lib, "dxguid.lib"		)			//  DirectX GUIDライブラリ
		//#pragma comment( lib, "dsound.lib"		)			//  DirectSoundライブラリ
		//#pragma comment( lib, "libcmt.lib"		)				//  C標準マルチスレッド対応ライブラリ
		#pragma comment( lib, "kernel32.lib"		)			//  Win32カーネルライブラリ
		#pragma comment( lib, "comctl32.lib"		)			//　Win32API用ライブラリ
		#pragma comment( lib, "user32.lib"		)				//  Win32API用ライブラリ
		#pragma comment( lib, "gdi32.lib"		)				//  Win32API用ライブラリ
		#pragma comment( lib, "advapi32.lib"		)			//  Win32API用ライブラリ
		#pragma comment( lib, "ole32.lib"		)				//  Win32API用ライブラリ
		#pragma comment( lib, "shell32.lib"		)				//  マルチメディアライブラリ
		#pragma comment( lib, "winmm.lib"		)				//  マルチメディアライブラリ
		#ifndef DX_NON_MOVIE
			//#pragma comment( lib, "Strmiids.lib" )			//　DirectShow用ライブラリ
		#endif
		#ifndef DX_NON_NETWORK
			#pragma comment( lib, "wsock32.lib" )				//  WinSockets用ライブラリ
		#endif
		#ifndef DX_NON_KEYEX
			#pragma comment( lib, "imm32.lib" )					// ＩＭＥ操作用ライブラリ
		#endif
		#ifndef DX_NON_ACM
			#pragma comment( lib, "msacm32.lib" )				// ＡＣＭ操作用ライブラリ 
		#endif
		#ifndef DX_NON_PNGREAD
			#ifdef _DEBUG
				#pragma comment( lib, "libpng_d.lib" )			// ＰＮＧ用ライブラリ
				#pragma comment( lib, "zlib_d.lib" )
			#else
				#pragma comment( lib, "libpng.lib" )			// ＰＮＧ用ライブラリ
				#pragma comment( lib, "zlib.lib" )
			#endif
		#endif
		#ifndef DX_NON_JPEGREAD
			#ifdef _DEBUG
				#pragma comment( lib, "libjpeg_d.lib" )			// ＪＰＥＧ用ライブラリ
			#else
				#pragma comment( lib, "libjpeg.lib" )			// ＪＰＥＧ用ライブラリ
			#endif
		#endif
		#ifndef DX_NON_OGGVORBIS								// ＯｇｇＶｏｒｂｉｓ用ライブラリ
			#ifdef _DEBUG
				#pragma comment( lib, "ogg_static_d.lib" )
				#pragma comment( lib, "vorbis_static_d.lib" )
				#pragma comment( lib, "vorbisfile_static_d.lib" )
			#else
				#pragma comment( lib, "ogg_static.lib" )
				#pragma comment( lib, "vorbis_static.lib" )
				#pragma comment( lib, "vorbisfile_static.lib" )
			#endif
		#endif
		#ifndef DX_NON_OGGTHEORA								// ＯｇｇＴｈｅｏｒａ用ライブラリ
			#ifdef _DEBUG
				#pragma comment( lib, "ogg_static_d.lib" )
				#pragma comment( lib, "vorbis_static_d.lib" )
				#pragma comment( lib, "vorbisfile_static_d.lib" )

				#pragma comment( lib, "libtheora_static_d.lib"		)		//  ＤＸライブラリ使用指定
			#else
				#pragma comment( lib, "ogg_static.lib" )
				#pragma comment( lib, "vorbis_static.lib" )
				#pragma comment( lib, "vorbisfile_static.lib" )

				#pragma comment( lib, "libtheora_static.lib"		)		//  ＤＸライブラリ使用指定
			#endif
		#endif
	#endif
#endif

/*
#ifndef __DX_MAKE
	#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
	#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
	#pragma comment( linker, "/NODEFAULTLIB:libcmtd.lib" )
	#pragma comment( linker, "/NODEFAULTLIB:msvcrt.lib" )
	#pragma comment( linker, "/NODEFAULTLIB:msvcrtd.lib" )
#endif
*/

/*
#ifndef __DX_MAKE
	#ifdef _DEBUG
		#ifndef DX_USE_VISUALC_MEM_DEBUG
			#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
//			#pragma comment( linker, "/NODEFAULTLIB:libcmt.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:libcmtd.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:msvcrt.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:msvcrtd.lib" )
		#else
			#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:libcmt.lib" )
		#endif
	#else
		#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
		#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
//		#pragma comment( linker, "/NODEFAULTLIB:libcmt.lib" )
		#pragma comment( linker, "/NODEFAULTLIB:libcmtd.lib" )
		#pragma comment( linker, "/NODEFAULTLIB:msvcrt.lib" )
		#pragma comment( linker, "/NODEFAULTLIB:msvcrtd.lib" )
	#endif
#endif
*/

/*
#ifndef __DX_MAKE
	#ifdef _DEBUG
		#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
//		#pragma comment( linker, "/NODEFAULTLIB:libcmt.lib" )
		#pragma comment( linker, "/NODEFAULTLIB:msvcrt.lib" )
		#ifdef DX_USE_MULTITASK_LIB
			#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
		#else
			#pragma comment( linker, "/NODEFAULTLIB:libcmtd.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:msvcrtd.lib" )
		#endif
	#else
		#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
		#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
		#pragma comment( linker, "/NODEFAULTLIB:libcmtd.lib" )
		#pragma comment( linker, "/NODEFAULTLIB:msvcrtd.lib" )
		#ifdef DX_USE_MULTITASK_LIB
			#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
		#else
//			#pragma comment( linker, "/NODEFAULTLIB:libcmt.lib" )
			#pragma comment( linker, "/NODEFAULTLIB:msvcrt.lib" )
		#endif
	#endif
#endif
*/


#ifdef DX_USE_DIRECTX_SDK_FILE
	#ifndef __DX_MAKE
		#ifdef __BCC
			#ifdef strcpy
			#undef strcpy
			#endif
			
			#ifdef strcat
			#undef strcat
			#endif
			
			#ifdef sprintf
			#undef sprintf
			#endif
			
			#ifdef vsprintf
			#undef vsprintf
			#endif
		#endif
	#endif

	// strsafe.h への対抗策
	#undef lstrcpy
	#undef lstrcat
	#undef wsprintf
	#undef wvsprintf
	#undef StrCpy
	#undef StrCat
	#undef StrNCat
	#undef StrCatN

	#undef lstrcpyA
	#undef lstrcpyW
	#undef lstrcatA
	#undef lstrcatW
	#undef wsprintfA
	#undef wsprintfW

	#undef StrCpyW
	#undef StrCatW
	#undef StrNCatA
	#undef StrNCatW
	#undef StrCatNA
	#undef StrCatNW
	#undef wvsprintfA
	#undef wvsprintfW

	#undef _tcscpy
	#undef _ftcscpy
	#undef _tcscat
	#undef _ftcscat
	#undef _stprintf
	#undef _sntprintf
	#undef _vstprintf
	#undef _vsntprintf
	#undef _getts

	#undef strcpy
	#undef wcscpy
	#undef strcat
	#undef wcscat
	#undef sprintf
	#undef swprintf
	#undef vsprintf
	#undef vswprintf
	#undef _snprintf
	#undef _snwprintf
	#undef _vsnprintf
	#undef _vsnwprintf
	#undef gets
	#undef _getws

	#ifdef UNICODE
	#define lstrcpy    lstrcpyW
	#define lstrcat    lstrcatW
	#define wsprintf   wsprintfW
	#define wvsprintf  wvsprintfW
	#else
	#define lstrcpy    lstrcpyA
	#define lstrcat    lstrcatA
	#define wsprintf   wsprintfA
	#define wvsprintf  wvsprintfA
	#endif

	#ifdef UNICODE
	#define StrCpy  StrCpyW
	#define StrCat  StrCatW
	#define StrNCat StrNCatW
	#else
	#define StrCpy  lstrcpyA
	#define StrCat  lstrcatA
	#define StrNCat StrNCatA
	#endif

	#ifdef _UNICODE
	#define _tcscpy     wcscpy
	#define _ftcscpy    wcscpy
	#define _tcscat     wcscat
	#define _ftcscat    wcscat
	#define _stprintf   swprintf
	#define _sntprintf  _snwprintf
	#define _vstprintf  vswprintf
	#define _vsntprintf _vsnwprintf
	#define _getts      _getws
	#else
	#define _tcscpy     strcpy
	#define _ftcscpy    strcpy
	#define _tcscat     strcat
	#define _ftcscat    strcat
	#define _stprintf   sprintf
	#define _sntprintf  _snprintf
	#define _vstprintf  vsprintf
	#define _vsntprintf _vsnprintf
	#define _getts      gets
	#endif

	#define strcpy	_STRCPY
	#define strcat	_STRCAT
#endif

// 定義---------------------------------------------------------------------------

// π
#define PHI							(3.1415926535897932384626433832795 )
#define PHI_F						(3.1415926535897932384626433832795f)
#define TWO_PHI						(3.1415926535897932384626433832795  * 2.0 )
#define TWO_PHI_F					(3.1415926535897932384626433832795f * 2.0f)

#define DX_CHAR										char

#define DX_DEFINE_START

#define MAX_IMAGE_NUM								(32768)				// 同時に持てるグラフィックハンドルの最大数( ハンドルエラーチェックのマスクに使用しているので 65536 以下の 2 のべき乗にして下さい )
#define MAX_2DSURFACE_NUM							(32768)				// ２Ｄサーフェスデータの最大数( ハンドルエラーチェックのマスクに使用しているので 65536 以下の 2 のべき乗にして下さい )
#define MAX_3DSURFACE_NUM							(65536)				// ３Ｄサーフェスデータの最大数( ハンドルエラーチェックのマスクに使用しているので 65536 以下の 2 のべき乗にして下さい )
#define MAX_IMAGE_DIVNUM							(64)				// 画像分割の最大数
#define MAX_SURFACE_NUM								(65536)				// サーフェスデータの最大数
#define MAX_SOFTIMAGE_NUM							(8192)				// 同時に持てる祖父とイメージハンドルの最大数( ハンドルエラーチェックのマスクに使用しているので 65536 以下の 2 のべき乗にして下さい )

#define MAX_SOUND_NUM								(4096)				// 同時に持てるサウンドハンドルの最大数
#define MAX_MUSIC_NUM								(256)				// 同時に持てるミュージックハンドルの最大数
#define MAX_MOVIE_NUM								(100)				// 同時に持てるムービーハンドルの最大数
#define MAX_MASK_NUM								(512)				// 同時に持てるマスクハンドルの最大数
#define MAX_FONT_NUM								(40)				// 同時に持てるフォントハンドルの最大数
#define MAX_INPUT_NUM								(256)				// 同時に持てる文字列入力ハンドルの最大数
#define MAX_SOCKET_NUM								(4096)				// 同時に持てる通信ハンドルの最大数
#define MAX_LIGHT_NUM								(4096)				// 同時に持てるライトハンドルの最大数
#define MAX_SHADER_NUM								(4096)				// 同時に持てるシェーダーハンドルの最大数

#define MAX_JOYPAD_NUM								(16)				// ジョイパッドの最大数
#define MAX_EVENTPROCESS_NUM						(5)					// 一度に処理するイベントの最大数

#define DEFAULT_SCREEN_SIZE_X						(640)				// デフォルトの画面の幅
#define DEFAULT_SCREEN_SIZE_Y						(480)				// デフォルトの画面の高さ
#define DEFAULT_COLOR_BITDEPTH						(16)				// デフォルトの色ビット深度

#define DEFAULT_FOV									(60.0F * 3.1415926535897932384626433832795F / 180.0F)	// デフォルトの視野角
#define DEFAULT_TAN_FOV_HALF						(0.57735026918962576450914878050196F) // tan( FOV * 0.5 )
#define DEFAULT_NEAR								(0.0F)				// NEARクリップ面
#define DEFAULT_FAR									(20000.0F)			// FARクリップ面

#define DEFAULT_FONT_SIZE							(16)				// フォントのデフォルトのサイズ
#define DEFAULT_FONT_THINCK							(6)					// フォントのデフォルトの太さ
#define DEFAULT_FONT_TYPE							( DX_FONTTYPE_NORMAL )	// フォントのデフォルトの形態
#define DEFAULT_FONT_EDGESIZE						(1)					// フォントのデフォルトの太さ

#define FONT_CACHE_MAXNUM							(2024)				// フォントキャッシュに格納できる最大文字数
#define FONT_CACHE_MEMORYSIZE						(0x50000)			// フォントキャッシュの最大容量
#define FONT_CACHE_MAX_YLENGTH						(0x4000)			// フォントキャッシュサーフェスの最大縦幅

#define MAX_USERIMAGEREAD_FUNCNUM					(10)				// ユーザーが登録できるグラフィックロード関数の最大数


// ハンドルの内訳
#define DX_HANDLEINDEX_MASK							(0x0000ffff)		// ハンドル配列インデックスマスク
#define DX_HANDLECHECKBIT_MASK						(0x07ff0000)		// ハンドルインデックスエラーチェック用マスク
#define DX_HANDLECHECKBIT_ADDRESS					(16)				// ハンドルインデックスエラーチェック用マスクの開始アドレス
#define DX_HANDLETYPE_MASK							(0x78000000)		// ハンドルタイプマスク
#define DX_HANDLEERROR_MASK							(0x80000000)		// エラーチェックマスク( ０ではなかったらエラー )
#define DX_HANDLEERROR_OR_TYPE_MASK					(0xf8000000)		// DX_HANDLETYPE_MASK と DX_HANDLEERROR_MASK を掛け合わせたもの

// ハンドルタイプ定義
#define DX_HANDLETYPE_GRAPH							(0x08000000)		// グラフィックハンドル
#define DX_HANDLETYPE_SOUND							(0x10000000)		// サウンドハンドル
#define DX_HANDLETYPE_MOVIE							(0x18000000)		// ムービーハンドル
#define DX_HANDLETYPE_FONT							(0x20000000)		// フォントハンドル
#define DX_HANDLETYPE_2DSURFACE						(0x28000000)		// ２Ｄサーフェスハンドル
#define DX_HANDLETYPE_3DSURFACE						(0x30000000)		// ３Ｄサーフェスハンドル
#define DX_HANDLETYPE_SURFACE						(0x38000000)		// サーフェスハンドル
#define DX_HANDLETYPE_GMASK							(0x40000000)		// マスクハンドル
#define DX_HANDLETYPE_NETWORK						(0x48000000)		// ネットワークハンドル
#define DX_HANDLETYPE_KEYINPUT						(0x50000000)		// 文字列入力ハンドル
#define DX_HANDLETYPE_MUSIC							(0x58000000)		// ミュージックハンドル
#define DX_HANDLETYPE_LIGHT							(0x60000000)		// ライト
#define DX_HANDLETYPE_MODEL							(0x68000000)		// ３Ｄモデル
#define DX_HANDLETYPE_SOFTIMAGE						(0x70000000)		// ソフトウエアで扱うイメージハンドル
#define DX_HANDLETYPE_SHADER						(0x78000000)		// シェーダーハンドル

// ＷＩＮＤＯＷＳのバージョンマクロ
#define DX_WINDOWSVERSION_31						(0x000)
#define DX_WINDOWSVERSION_95						(0x001)
#define DX_WINDOWSVERSION_98						(0x002)
#define DX_WINDOWSVERSION_ME						(0x003)
#define DX_WINDOWSVERSION_NT31						(0x104)
#define DX_WINDOWSVERSION_NT40						(0x105)
#define DX_WINDOWSVERSION_2000						(0x106)
#define DX_WINDOWSVERSION_XP						(0x107)
#define DX_WINDOWSVERSION_VISTA						(0x108)
#define DX_WINDOWSVERSION_7							(0x109)
#define DX_WINDOWSVERSION_NT_TYPE					(0x100)

// ＤｉｒｅｃｔＸのバージョンマクロ
#define DX_DIRECTXVERSION_NON						(0)
#define DX_DIRECTXVERSION_1							(0x10000)
#define DX_DIRECTXVERSION_2							(0x20000)
#define DX_DIRECTXVERSION_3							(0x30000)
#define DX_DIRECTXVERSION_4							(0x40000)
#define DX_DIRECTXVERSION_5							(0x50000)
#define DX_DIRECTXVERSION_6							(0x60000)
#define DX_DIRECTXVERSION_6_1						(0x60100)
#define DX_DIRECTXVERSION_7							(0x70000)
#define DX_DIRECTXVERSION_8							(0x80000)
#define DX_DIRECTXVERSION_8_1						(0x80100)

// 文字セット
#define DX_CHARSET_DEFAULT							(0)				// デフォルト文字セット
#define DX_CHARSET_SHFTJIS							(1)				// 日本語文字セット
#define DX_CHARSET_HANGEUL							(2)				// 韓国語文字セット

// ＭＩＤＩの再生モード定義
#define DX_MIDIMODE_MCI								(0)				// ＭＣＩによる再生
#define DX_MIDIMODE_DM								(1)				// ＤｉｒｅｃｔＭｕｓｉｃによる再生

// 描画モード定義
#define DX_DRAWMODE_NEAREST							(0)				// ネアレストネイバー法で描画
#define DX_DRAWMODE_BILINEAR						(1)				// バイリニア法で描画する
#define DX_DRAWMODE_ANISOTROPIC						(2)				// 異方性フィルタリング法で描画する
#define DX_DRAWMODE_OTHER							(3)				// それ以外

// フォントのタイプ
#define DX_FONTTYPE_NORMAL							(0)				// ノーマルフォント
#define DX_FONTTYPE_EDGE							(1)				// エッジつきフォント
#define DX_FONTTYPE_ANTIALIASING					(2)				// アンチエイリアスフォント
#define DX_FONTTYPE_ANTIALIASING_EDGE				(3)				// アンチエイリアス＆エッジ付きフォント

// 描画ブレンドモード定義
#define DX_BLENDMODE_NOBLEND						(0)				// ノーブレンド
#define DX_BLENDMODE_ALPHA							(1)				// αブレンド
#define DX_BLENDMODE_ADD							(2)				// 加算ブレンド
#define DX_BLENDMODE_SUB							(3)				// 減算ブレンド
#define DX_BLENDMODE_MUL							(4)				// 乗算ブレンド
   // (内部処理用)
#define DX_BLENDMODE_SUB2							(5)				// 内部処理用減算ブレンド子１
//#define DX_BLENDMODE_BLINEALPHA						(7)				// 境界線ぼかし
#define DX_BLENDMODE_XOR							(6)				// XORブレンド
#define DX_BLENDMODE_DESTCOLOR						(8)				// カラーは更新されない
#define DX_BLENDMODE_INVDESTCOLOR					(9)				// 描画先の色の反転値を掛ける
#define DX_BLENDMODE_INVSRC							(10)			// 描画元の色を反転する
#define DX_BLENDMODE_MULA							(11)			// アルファチャンネル考慮付き乗算ブレンド

// 描画先画面指定用定義
#define DX_SCREEN_FRONT								(0xfffffffc)
#define DX_SCREEN_BACK								(0xfffffffe) 
#define DX_SCREEN_WORK 								(0xfffffffd)
#define DX_SCREEN_TEMPFRONT							(0xfffffffb)

#define DX_NONE_GRAPH								(0xfffffffb)	// グラフィックなしハンドル

// グラフィック減色時の画像劣化緩和処理モード
#define DX_SHAVEDMODE_NONE							(0)				// 画像劣化緩和処理を行わない
#define DX_SHAVEDMODE_DITHER						(1)				// ディザリング
#define DX_SHAVEDMODE_DIFFUS						(2)				// 誤差拡散

// 画像の保存タイプ
#define DX_IMAGESAVETYPE_BMP						(0)				// bitmap
#define DX_IMAGESAVETYPE_JPEG						(1)				// jpeg
#define DX_IMAGESAVETYPE_PNG						(2)				// Png

// サウンド再生形態指定用定義
#define DX_PLAYTYPE_LOOPBIT							(0x0002)		// ループ再生ビット
#define DX_PLAYTYPE_BACKBIT							(0x0001)		// バックグラウンド再生ビット
	
#define DX_PLAYTYPE_NORMAL							(0)												// ノーマル再生
#define DX_PLAYTYPE_BACK				  			( DX_PLAYTYPE_BACKBIT )							// バックグラウンド再生
#define DX_PLAYTYPE_LOOP							( DX_PLAYTYPE_LOOPBIT | DX_PLAYTYPE_BACKBIT )	// ループ再生

// 動画再生タイプ定義
#define DX_MOVIEPLAYTYPE_BCANCEL					(0)				// ボタンキャンセルあり
#define DX_MOVIEPLAYTYPE_NORMAL						(1)				// ボタンキャンセルなし

// サウンドのタイプ
#define DX_SOUNDTYPE_NORMAL							(0)				// ノーマルサウンド形式
#define DX_SOUNDTYPE_STREAMSTYLE					(1)				// ストリーム風サウンド形式

// ストリームサウンド再生データタイプのマクロ
#define DX_SOUNDDATATYPE_MEMNOPRESS					(0)				// 圧縮された全データは再生が始まる前にサウンドメモリにすべて解凍され、格納される
#define DX_SOUNDDATATYPE_MEMNOPRESS_PLUS			(1)				// 圧縮された全データはシステムメモリに格納され、再生しながら逐次解凍され、最終的にすべてサウンドメモリに格納される(その後システムメモリに存在する圧縮データは破棄される)
#define DX_SOUNDDATATYPE_MEMPRESS					(2)				// 圧縮された全データはシステムメモリに格納され、再生する部分だけ逐次解凍しながらサウンドメモリに格納する(鳴らし終わると解凍したデータは破棄されるので何度も解凍処理が行われる)
#define DX_SOUNDDATATYPE_FILE						(3)				// 圧縮されたデータの再生する部分だけファイルから逐次読み込み解凍され、サウンドメモリに格納される(鳴らし終わると解凍したデータは破棄されるので何度も解凍処理が行われる)

// マスク透過色モード
#define DX_MASKTRANS_WHITE							(0)				// マスク画像の白い部分を透過色とする
#define DX_MASKTRANS_BLACK							(1)				// マスク画像の黒い部分を透過色とする
#define DX_MASKTRANS_NONE							(2) 			// 透過色なし

// Ｚバッファ書き込みモード
#define DX_ZWRITE_MASK								(0)				// 書き込めないようにマスクする
#define DX_ZWRITE_CLEAR								(1)				// 書き込めるようにマスクをクリアする

// 比較モード
#define DX_CMP_NEVER								(1)				// FALSE
#define DX_CMP_LESS									(2)				// Src <  Dest
#define DX_CMP_EQUAL								(3)				// Src == Dest
#define DX_CMP_LESSEQUAL							(4)				// Src <= Dest
#define DX_CMP_GREATER								(5)				// Src >  Dest
#define DX_CMP_NOTEQUAL								(6)				// Src != Dest
#define DX_CMP_GREATEREQUAL							(7)				// Src >= Dest
#define DX_CMP_ALWAYS								(8)				// TRUE
#define DX_ZCMP_DEFAULT								( DX_CMP_LESSEQUAL )
#define DX_ZCMP_REVERSE								( DX_CMP_GREATEREQUAL )

// シェーディングモード
#define DX_SHADEMODE_FLAT							D_D3DSHADE_FLAT
#define DX_SHADEMODE_GOURAUD						D_D3DSHADE_GOURAUD

// フォグモード
#define DX_FOGMODE_NONE								D_D3DFOG_NONE
#define DX_FOGMODE_EXP								D_D3DFOG_EXP
#define DX_FOGMODE_EXP2								D_D3DFOG_EXP2
#define DX_FOGMODE_LINEAR							D_D3DFOG_LINEAR

// マテリアルタイプ
#define DX_MATERIAL_TYPE_NORMAL						(0)				// 標準マテリアル
#define DX_MATERIAL_TYPE_TOON						(1)				// トゥーンレンダリング用マテリアル

// マテリアルブレンドタイプ
#define DX_MATERIAL_BLENDTYPE_TRANSLUCENT			(0)				// アルファ合成
#define DX_MATERIAL_BLENDTYPE_ADDITIVE				(1)				// 加算
#define DX_MATERIAL_BLENDTYPE_MODULATE				(2)				// 乗算

// テクスチャアドレスタイプ
#define DX_TEXADDRESS_WRAP							D_D3DTADDRESS_WRAP
#define DX_TEXADDRESS_MIRROR						D_D3DTADDRESS_MIRROR
#define DX_TEXADDRESS_CLAMP							D_D3DTADDRESS_CLAMP
#define DX_TEXADDRESS_BORDER						D_D3DTADDRESS_BORDER

// ポリゴン描画タイプ
#define DX_PRIMTYPE_POINTLIST						D_D3DPT_POINTLIST
#define DX_PRIMTYPE_LINELIST						D_D3DPT_LINELIST
#define DX_PRIMTYPE_LINESTRIP						D_D3DPT_LINESTRIP
#define DX_PRIMTYPE_TRIANGLELIST					D_D3DPT_TRIANGLELIST
#define DX_PRIMTYPE_TRIANGLESTRIP					D_D3DPT_TRIANGLESTRIP
#define DX_PRIMTYPE_TRIANGLEFAN						D_D3DPT_TRIANGLEFAN

// ライトタイプ
#define DX_LIGHTTYPE_D3DLIGHT_POINT					D_D3DLIGHT_POINT
#define DX_LIGHTTYPE_D3DLIGHT_SPOT					D_D3DLIGHT_SPOT
#define DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL			D_D3DLIGHT_DIRECTIONAL
#define DX_LIGHTTYPE_D3DLIGHT_FORCEDWORD			D_D3DLIGHT_FORCE_DWORD
#define DX_LIGHTTYPE_POINT							D_D3DLIGHT_POINT
#define DX_LIGHTTYPE_SPOT							D_D3DLIGHT_SPOT
#define DX_LIGHTTYPE_DIRECTIONAL					D_D3DLIGHT_DIRECTIONAL

// グラフィックイメージフォーマットの定義
#define DX_GRAPHICSIMAGE_FORMAT_3D_RGB16					(0)		// １６ビットカラー標準
#define DX_GRAPHICSIMAGE_FORMAT_3D_RGB32					(1)		// ３２ビットカラー標準
#define DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16				(2)		// αチャンネル付き１６ビットカラー
#define DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32				(3)		// αチャンネル付き３２ビットカラー
#define DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16			(4)		// αテスト付き１６ビットカラー
#define DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32			(5)		// αテスト付き３２ビットカラー
#define DX_GRAPHICSIMAGE_FORMAT_3D_DXT1						(6)		// ＤＸＴ１
#define DX_GRAPHICSIMAGE_FORMAT_3D_DXT2						(7)		// ＤＸＴ２
#define DX_GRAPHICSIMAGE_FORMAT_3D_DXT3						(8)		// ＤＸＴ３
#define DX_GRAPHICSIMAGE_FORMAT_3D_DXT4						(9)		// ＤＸＴ４
#define DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 					(10)	// ＤＸＴ５
#define DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16			(11)	// 描画可能１６ビットカラー
#define DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32			(12)	// 描画可能３２ビットカラー
#define DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32	(13)	// 描画可能α付き３２ビットカラー
#define DX_GRAPHICSIMAGE_FORMAT_3D_NUM						(14)
#define DX_GRAPHICSIMAGE_FORMAT_2D							(14)	// 標準( DirectDrawSurface の場合はこれのみ )
#define DX_GRAPHICSIMAGE_FORMAT_R5G6B5						(15)	// R5G6B5( MEMIMG 用 )
#define DX_GRAPHICSIMAGE_FORMAT_X8A8R5G6B5					(16)	// X8A8R5G6B5( MEMIMG 用 )
#define DX_GRAPHICSIMAGE_FORMAT_X8R8G8B8					(17)	// X8R8G8B8( MEMIMG 用 )
#define DX_GRAPHICSIMAGE_FORMAT_A8R8G8B8					(18)	// A8R8G8B8( MEMIMG 用 )

#define DX_GRAPHICSIMAGE_FORMAT_NUM							(19)	// グラフィックフォーマットの種類の数

// 基本イメージのピクセルフォーマット
#define DX_BASEIMAGE_FORMAT_NORMAL			(0)			// 普通の画像
#define DX_BASEIMAGE_FORMAT_DXT1			(1)			// ＤＸＴ１
#define DX_BASEIMAGE_FORMAT_DXT2			(2)			// ＤＸＴ２
#define DX_BASEIMAGE_FORMAT_DXT3			(3)			// ＤＸＴ３
#define DX_BASEIMAGE_FORMAT_DXT4			(4)			// ＤＸＴ４
#define DX_BASEIMAGE_FORMAT_DXT5			(5)			// ＤＸＴ５

// ツールバーのボタンの状態
#define TOOLBUTTON_STATE_ENABLE				(0)			// 入力可能な状態
#define TOOLBUTTON_STATE_PRESSED			(1)			// 押されている状態
#define TOOLBUTTON_STATE_DISABLE			(2)			// 入力不可能な状態
#define TOOLBUTTON_STATE_PRESSED_DISABLE	(3)			// 押されている状態で、入力不可能な状態
#define TOOLBUTTON_STATE_NUM				(4)			// ツールバーのボタンの状態の数

// ツールバーのボタンのタイプ
#define TOOLBUTTON_TYPE_NORMAL			(0)			// 普通のボタン
#define TOOLBUTTON_TYPE_CHECK			(1)			// 押すごとにＯＮ／ＯＦＦが切り替わるボタン
#define TOOLBUTTON_TYPE_GROUP			(2)			// 別の TOOLBUTTON_TYPE_GROUP タイプのボタンが押されるとＯＦＦになるタイプのボタン(グループの区切りは隙間で)
#define TOOLBUTTON_TYPE_SEP				(3)			// 隙間(ボタンではありません)
#define TOOLBUTTON_TYPE_NUM				(4)			// ツールバーのボタンのタイプの数

// 親メニューのＩＤ
#define MENUITEM_IDTOP								(0xabababab)

// メニューに追加する際のタイプ
#define MENUITEM_ADD_CHILD							(0)				// 指定の項目の子として追加する
#define MENUITEM_ADD_INSERT							(1)				// 指定の項目と指定の項目より一つ上の項目の間に追加する

// メニューの横に付くマークタイプ
#define MENUITEM_MARK_NONE							(0)				// 何も付け無い
#define MENUITEM_MARK_CHECK							(1)				// チェックマーク
#define MENUITEM_MARK_RADIO							(2)				// ラジオボタン

// 文字変換タイプ定義
#define DX_NUMMODE_10								(0)				// １０進数
#define DX_NUMMODE_16								(1)				// １６進数
#define DX_STRMODE_NOT0								(2)				// 空きを０で埋めない
#define DX_STRMODE_USE0								(3)				// 空きを０で埋める

// CheckHitKeyAll で調べる入力タイプ
#define DX_CHECKINPUT_KEY							(0x0001)		// キー入力を調べる
#define DX_CHECKINPUT_PAD							(0x0002)		// パッド入力を調べる
#define DX_CHECKINPUT_MOUSE							(0x0004)		// マウスボタン入力を調べる
#define DX_CHECKINPUT_ALL							(DX_CHECKINPUT_KEY | DX_CHECKINPUT_PAD | DX_CHECKINPUT_MOUSE)	// すべての入力を調べる

// パッド入力取得パラメータ
#define DX_INPUT_KEY_PAD1							(0x1001)		// キー入力とパッド１入力
#define DX_INPUT_PAD1								(0x0001)		// パッド１入力
#define DX_INPUT_PAD2								(0x0002)		// パッド２入力
#define DX_INPUT_PAD3								(0x0003)		// パッド３入力
#define DX_INPUT_PAD4								(0x0004)		// パッド４入力
#define DX_INPUT_PAD5								(0x0005)		// パッド５入力
#define DX_INPUT_PAD6								(0x0006)		// パッド６入力
#define DX_INPUT_PAD7								(0x0007)		// パッド７入力
#define DX_INPUT_PAD8								(0x0008)		// パッド８入力
#define DX_INPUT_PAD9								(0x0009)		// パッド９入力
#define DX_INPUT_PAD10								(0x000a)		// パッド１０入力
#define DX_INPUT_PAD11								(0x000b)		// パッド１１入力
#define DX_INPUT_PAD12								(0x000c)		// パッド１２入力
#define DX_INPUT_PAD13								(0x000d)		// パッド１３入力
#define DX_INPUT_PAD14								(0x000e)		// パッド１４入力
#define DX_INPUT_PAD15								(0x000f)		// パッド１５入力
#define DX_INPUT_PAD16								(0x0010)		// パッド１６入力
#define DX_INPUT_KEY								(0x1000)		// キー入力

// ムービーのサーフェスモード
#define DX_MOVIESURFACE_NORMAL						(0)
#define DX_MOVIESURFACE_OVERLAY						(1)
#define DX_MOVIESURFACE_FULLCOLOR					(2)

// パッド入力定義
#define PAD_INPUT_DOWN								(0x00000001)	// ↓チェックマスク
#define PAD_INPUT_LEFT								(0x00000002)	// ←チェックマスク
#define PAD_INPUT_RIGHT								(0x00000004)	// →チェックマスク
#define PAD_INPUT_UP								(0x00000008)	// ↑チェックマスク
#define PAD_INPUT_A									(0x00000010)	// Ａボタンチェックマスク
#define PAD_INPUT_B									(0x00000020)	// Ｂボタンチェックマスク
#define PAD_INPUT_C									(0x00000040)	// Ｃボタンチェックマスク
#define PAD_INPUT_X									(0x00000080)	// Ｘボタンチェックマスク
#define PAD_INPUT_Y									(0x00000100)	// Ｙボタンチェックマスク
#define PAD_INPUT_Z									(0x00000200)	// Ｚボタンチェックマスク
#define PAD_INPUT_L									(0x00000400)	// Ｌボタンチェックマスク
#define PAD_INPUT_R									(0x00000800)	// Ｒボタンチェックマスク
#define PAD_INPUT_START								(0x00001000)	// ＳＴＡＲＴボタンチェックマスク
#define PAD_INPUT_M									(0x00002000)	// Ｍボタンチェックマスク
#define PAD_INPUT_D									(0x00004000)
#define PAD_INPUT_F									(0x00008000)
#define PAD_INPUT_G									(0x00010000)
#define PAD_INPUT_H									(0x00020000)
#define PAD_INPUT_I									(0x00040000)
#define PAD_INPUT_J									(0x00080000)
#define PAD_INPUT_K									(0x00100000)
#define PAD_INPUT_LL								(0x00200000)
#define PAD_INPUT_N									(0x00400000)
#define PAD_INPUT_O									(0x00800000)
#define PAD_INPUT_P									(0x01000000)
#define PAD_INPUT_RR								(0x02000000)
#define PAD_INPUT_S									(0x04000000)
#define PAD_INPUT_T									(0x08000000)
#define PAD_INPUT_U									(0x10000000)
#define PAD_INPUT_V									(0x20000000)
#define PAD_INPUT_W									(0x40000000)
#define PAD_INPUT_XX								(0x80000000)

// マウス入力定義
#define MOUSE_INPUT_LEFT							(0x0001)			// マウス左ボタン
#define MOUSE_INPUT_RIGHT							(0x0002)			// マウス右ボタン
#define MOUSE_INPUT_MIDDLE							(0x0004)			// マウス中央ボタン
#define MOUSE_INPUT_1								(0x0001)			// マウス１ボタン
#define MOUSE_INPUT_2								(0x0002)			// マウス２ボタン
#define MOUSE_INPUT_3								(0x0004)			// マウス３ボタン
#define MOUSE_INPUT_4								(0x0008)			// マウス４ボタン
#define MOUSE_INPUT_5								(0x0010)			// マウス５ボタン
#define MOUSE_INPUT_6								(0x0020)			// マウス６ボタン
#define MOUSE_INPUT_7								(0x0040)			// マウス７ボタン
#define MOUSE_INPUT_8								(0x0080)			// マウス８ボタン

// キー定義
#define KEY_INPUT_BACK								D_DIK_BACK			// バックスペースキー
#define KEY_INPUT_TAB								D_DIK_TAB			// タブキー
#define KEY_INPUT_RETURN							D_DIK_RETURN		// エンターキー

#define KEY_INPUT_LSHIFT							D_DIK_LSHIFT		// 左シフトキー
#define KEY_INPUT_RSHIFT							D_DIK_RSHIFT		// 右シフトキー
#define KEY_INPUT_LCONTROL							D_DIK_LCONTROL		// 左コントロールキー
#define KEY_INPUT_RCONTROL							D_DIK_RCONTROL		// 右コントロールキー
#define KEY_INPUT_ESCAPE							D_DIK_ESCAPE		// エスケープキー
#define KEY_INPUT_SPACE								D_DIK_SPACE			// スペースキー
#define KEY_INPUT_PGUP								D_DIK_PGUP			// ＰａｇｅＵＰキー
#define KEY_INPUT_PGDN								D_DIK_PGDN			// ＰａｇｅＤｏｗｎキー
#define KEY_INPUT_END								D_DIK_END			// エンドキー
#define KEY_INPUT_HOME								D_DIK_HOME			// ホームキー
#define KEY_INPUT_LEFT								D_DIK_LEFT			// 左キー
#define KEY_INPUT_UP								D_DIK_UP			// 上キー
#define KEY_INPUT_RIGHT								D_DIK_RIGHT			// 右キー
#define KEY_INPUT_DOWN								D_DIK_DOWN			// 下キー
#define KEY_INPUT_INSERT							D_DIK_INSERT		// インサートキー
#define KEY_INPUT_DELETE							D_DIK_DELETE		// デリートキー

#define KEY_INPUT_MINUS								D_DIK_MINUS			// －キー
#define KEY_INPUT_YEN								D_DIK_YEN			// ￥キー
#define KEY_INPUT_PREVTRACK							D_DIK_PREVTRACK		// ＾キー
#define KEY_INPUT_PERIOD							D_DIK_PERIOD		// ．キー
#define KEY_INPUT_SLASH								D_DIK_SLASH			// ／キー
#define KEY_INPUT_LALT								D_DIK_LALT			// 左ＡＬＴキー
#define KEY_INPUT_RALT								D_DIK_RALT			// 右ＡＬＴキー
#define KEY_INPUT_SCROLL							D_DIK_SCROLL		// ScrollLockキー
#define KEY_INPUT_SEMICOLON							D_DIK_SEMICOLON		// ；キー
#define KEY_INPUT_COLON								D_DIK_COLON			// ：キー
#define KEY_INPUT_LBRACKET							D_DIK_LBRACKET		// ［キー
#define KEY_INPUT_RBRACKET							D_DIK_RBRACKET		// ］キー
#define KEY_INPUT_AT								D_DIK_AT			// ＠キー
#define KEY_INPUT_BACKSLASH							D_DIK_BACKSLASH		// ＼キー
#define KEY_INPUT_COMMA								D_DIK_COMMA			// ，キー
#define KEY_INPUT_KANJI								D_DIK_KANJI			// 漢字キー
#define KEY_INPUT_CONVERT							D_DIK_CONVERT		// 変換キー
#define KEY_INPUT_NOCONVERT							D_DIK_NOCONVERT		// 無変換キー
#define KEY_INPUT_KANA								D_DIK_KANA			// カナキー
#define KEY_INPUT_APPS								D_DIK_APPS			// アプリケーションメニューキー
#define KEY_INPUT_CAPSLOCK							D_DIK_CAPSLOCK		// CaspLockキー
#define KEY_INPUT_SYSRQ								D_DIK_SYSRQ			// PrintScreenキー
#define KEY_INPUT_PAUSE								D_DIK_PAUSE			// PauseBreakキー
#define KEY_INPUT_LWIN								D_DIK_LWIN			// 左Ｗｉｎキー
#define KEY_INPUT_RWIN								D_DIK_RWIN			// 右Ｗｉｎキー

#define KEY_INPUT_NUMLOCK							D_DIK_NUMLOCK		// テンキー０
#define KEY_INPUT_NUMPAD0							D_DIK_NUMPAD0		// テンキー０
#define KEY_INPUT_NUMPAD1							D_DIK_NUMPAD1		// テンキー１
#define KEY_INPUT_NUMPAD2							D_DIK_NUMPAD2		// テンキー２
#define KEY_INPUT_NUMPAD3							D_DIK_NUMPAD3		// テンキー３
#define KEY_INPUT_NUMPAD4							D_DIK_NUMPAD4		// テンキー４
#define KEY_INPUT_NUMPAD5							D_DIK_NUMPAD5		// テンキー５
#define KEY_INPUT_NUMPAD6							D_DIK_NUMPAD6		// テンキー６
#define KEY_INPUT_NUMPAD7							D_DIK_NUMPAD7		// テンキー７
#define KEY_INPUT_NUMPAD8							D_DIK_NUMPAD8		// テンキー８
#define KEY_INPUT_NUMPAD9							D_DIK_NUMPAD9		// テンキー９
#define KEY_INPUT_MULTIPLY							D_DIK_MULTIPLY		// テンキー＊キー
#define KEY_INPUT_ADD								D_DIK_ADD			// テンキー＋キー
#define KEY_INPUT_SUBTRACT							D_DIK_SUBTRACT		// テンキー－キー
#define KEY_INPUT_DECIMAL							D_DIK_DECIMAL		// テンキー．キー
#define KEY_INPUT_DIVIDE							D_DIK_DIVIDE		// テンキー／キー
#define KEY_INPUT_NUMPADENTER						D_DIK_NUMPADENTER	// テンキーのエンターキー

#define KEY_INPUT_F1								D_DIK_F1			// Ｆ１キー
#define KEY_INPUT_F2								D_DIK_F2			// Ｆ２キー
#define KEY_INPUT_F3								D_DIK_F3			// Ｆ３キー
#define KEY_INPUT_F4								D_DIK_F4			// Ｆ４キー
#define KEY_INPUT_F5								D_DIK_F5			// Ｆ５キー
#define KEY_INPUT_F6								D_DIK_F6			// Ｆ６キー
#define KEY_INPUT_F7								D_DIK_F7			// Ｆ７キー
#define KEY_INPUT_F8								D_DIK_F8			// Ｆ８キー
#define KEY_INPUT_F9								D_DIK_F9			// Ｆ９キー
#define KEY_INPUT_F10								D_DIK_F10			// Ｆ１０キー
#define KEY_INPUT_F11								D_DIK_F11			// Ｆ１１キー
#define KEY_INPUT_F12								D_DIK_F12			// Ｆ１２キー

#define KEY_INPUT_A									D_DIK_A			// Ａキー
#define KEY_INPUT_B									D_DIK_B			// Ｂキー
#define KEY_INPUT_C									D_DIK_C			// Ｃキー
#define KEY_INPUT_D									D_DIK_D			// Ｄキー
#define KEY_INPUT_E									D_DIK_E			// Ｅキー
#define KEY_INPUT_F									D_DIK_F			// Ｆキー
#define KEY_INPUT_G									D_DIK_G			// Ｇキー
#define KEY_INPUT_H									D_DIK_H			// Ｈキー
#define KEY_INPUT_I									D_DIK_I			// Ｉキー
#define KEY_INPUT_J									D_DIK_J			// Ｊキー
#define KEY_INPUT_K									D_DIK_K			// Ｋキー
#define KEY_INPUT_L									D_DIK_L			// Ｌキー
#define KEY_INPUT_M									D_DIK_M			// Ｍキー
#define KEY_INPUT_N									D_DIK_N			// Ｎキー
#define KEY_INPUT_O									D_DIK_O			// Ｏキー
#define KEY_INPUT_P									D_DIK_P			// Ｐキー
#define KEY_INPUT_Q									D_DIK_Q			// Ｑキー
#define KEY_INPUT_R									D_DIK_R			// Ｒキー
#define KEY_INPUT_S									D_DIK_S			// Ｓキー
#define KEY_INPUT_T									D_DIK_T			// Ｔキー
#define KEY_INPUT_U									D_DIK_U			// Ｕキー
#define KEY_INPUT_V									D_DIK_V			// Ｖキー
#define KEY_INPUT_W									D_DIK_W			// Ｗキー
#define KEY_INPUT_X									D_DIK_X			// Ｘキー
#define KEY_INPUT_Y									D_DIK_Y			// Ｙキー
#define KEY_INPUT_Z									D_DIK_Z			// Ｚキー

#define KEY_INPUT_0 								D_DIK_0			// ０キー
#define KEY_INPUT_1									D_DIK_1			// １キー
#define KEY_INPUT_2									D_DIK_2			// ２キー
#define KEY_INPUT_3									D_DIK_3			// ３キー
#define KEY_INPUT_4									D_DIK_4			// ４キー
#define KEY_INPUT_5									D_DIK_5			// ５キー
#define KEY_INPUT_6									D_DIK_6			// ６キー
#define KEY_INPUT_7									D_DIK_7			// ７キー
#define KEY_INPUT_8									D_DIK_8			// ８キー
#define KEY_INPUT_9									D_DIK_9			// ９キー

// アスキーコントロールキーコード
#define CTRL_CODE_BS								(0x08)				// バックスペース
#define CTRL_CODE_TAB								(0x09)				// タブ
#define CTRL_CODE_CR								(0x0d)				// 改行
#define CTRL_CODE_DEL								(0x10)				// ＤＥＬキー

#define CTRL_CODE_COPY								(0x03)				// コピー
#define CTRL_CODE_PASTE								(0x16)				// ペースト
#define CTRL_CODE_CUT								(0x18)				// カット
#define CTRL_CODE_ALL								(0x01)				// 全て選択

#define CTRL_CODE_LEFT								(0x1d)				// ←キー
#define CTRL_CODE_RIGHT								(0x1c)				// →キー
#define CTRL_CODE_UP								(0x1e)				// ↑キー
#define CTRL_CODE_DOWN								(0x1f)				// ↓キー

#define CTRL_CODE_HOME								(0x1a)				// ＨＯＭＥボタン
#define CTRL_CODE_END								(0x19)				// ＥＮＤボタン
#define CTRL_CODE_PAGE_UP							(0x17)				// ＰＡＧＥ ＵＰ
#define CTRL_CODE_PAGE_DOWN							(0x15)				// ＰＡＧＥ ＤＯＷＮ

#define CTRL_CODE_ESC								(0x1b)				// ＥＳＣキー
#define CTRL_CODE_CMP								(0x20)				// 制御コード敷居値

// SetGraphMode 戻り値定義
#define DX_CHANGESCREEN_OK							(0)					// 画面変更は成功した
#define DX_CHANGESCREEN_RETURN						(-1)				// 画面の変更は失敗し、元の画面モードに戻された
#define DX_CHANGESCREEN_DEFAULT						(-2)				// 画面の変更は失敗しデフォルトの画面モードに変更された
#define DX_CHANGESCREEN_REFRESHNORMAL				(-3)				// 画面の変更は成功したが、リフレッシュレートの変更は失敗した

// ストリームデータ読み込み処理コード簡略化関連
#define STTELL( st )								((st)->ReadShred.Tell( (st)->DataPoint ))
#define STSEEK( st, pos, type )						((st)->ReadShred.Seek( (st)->DataPoint, (pos), (type) ))
#define STREAD( buf, length, num, st )				((st)->ReadShred.Read( (buf), (length), (num), (st)->DataPoint ))
#define STWRITE( buf, length, num, st )				((st)->ReadShred.Write( (buf), (length), (num), (st)->DataPoint ))
#define STEOF( st )									((st)->ReadShred.Eof( (st)->DataPoint ))
#define STCLOSE( st )								((st)->ReadShred.Close( (st)->DataPoint ))

// ストリームデータ制御のシークタイプ定義
#define STREAM_SEEKTYPE_SET							(SEEK_SET)
#define STREAM_SEEKTYPE_END							(SEEK_END)
#define STREAM_SEEKTYPE_CUR							(SEEK_CUR)

// グラフィックロード時のイメージタイプ
#define LOADIMAGE_TYPE_FILE							(0)				// イメージはファイルである
#define LOADIMAGE_TYPE_MEM							(1)				// イメージはメモリである
#define LOADIMAGE_TYPE_NONE							(-1)			// イメージは無い

// DrawPreparation 関数に渡すフラグ
#define DRAWPREP_TRANS			(0x0001)
#define DRAWPREP_VECTORINT		(0x0002)
#define DRAWPREP_GOURAUDSHADE	(0x0008)
#define DRAWPREP_PERSPECTIVE	(0x0010)
#define DRAWPREP_DIFFUSERGB		(0x0020)
#define DRAWPREP_DIFFUSEALPHA	(0x0040)
#define DRAWPREP_FOG			(0x0080)
#define DRAWPREP_NOBLENDSETTING	(0x0100)
#define DRAWPREP_LIGHTING		(0x0200)
#define DRAWPREP_SPECULAR		(0x0400)
#define DRAWPREP_3D				(0x0800)
#define DRAWPREP_TEXADDRESS		(0x1000)
#define DRAWPREP_NOTSHADERRESET	(0x2000)


#ifndef DX_NON_NETWORK

// HTTP エラー
#define HTTP_ERR_SERVER								(0)				// サーバーエラー
#define HTTP_ERR_NOTFOUND							(1)				// ファイルが見つからなかった
#define HTTP_ERR_MEMORY								(2)				// メモリ確保の失敗
#define HTTP_ERR_LOST								(3)				// 途中で切断された
#define HTTP_ERR_NONE								(-1)			// エラーは報告されていない

// HTTP 処理の結果
#define HTTP_RES_COMPLETE							(0)				// 処理完了
#define HTTP_RES_STOP								(1)				// 処理中止
#define HTTP_RES_ERROR								(2)				// エラー終了
#define HTTP_RES_NOW								(-1)			// 現在進行中

#endif

#define DX_DEFINE_END

// データ型定義-------------------------------------------------------------------

namespace DxLib
{

#define DX_STRUCT_START

// ＩＭＥ入力文字列の描画に必要な情報の内の文節情報
typedef struct tagIMEINPUTCLAUSEDATA
{
	int						Position ;				// 何バイト目から
	int						Length ;				// 何バイトか
} IMEINPUTCLAUSEDATA, *LPIMEINPUTCLAUSEDATA ;

// ＩＭＥ入力文字列の描画に必要な情報
typedef struct tagIMEINPUTDATA
{
	const char *				InputString ;			// 入力中の文字列

	int							CursorPosition ;		// カーソルの入力文字列中の位置(バイト単位)

	const IMEINPUTCLAUSEDATA *	ClauseData ;			// 文節情報
	int							ClauseNum ;				// 文節情報の数
	int							SelectClause ;			// 選択中の分節( -1 の場合はどの文節にも属していない( 末尾にカーソルがある ) )

	int							CandidateNum ;			// 変換候補の数( 0の場合は変換中ではない )
	const char **				CandidateList ;			// 変換候補文字列リスト( 例：ｎ番目の候補を描画する場合  DrawString( 0, 0, data.CandidateList[ n ], GetColor(255,255,255) ); )
	int							SelectCandidate ;		// 選択中の変換候補
} IMEINPUTDATA, *LPIMEINPUTDATA ;

// タイムデータ型
typedef struct tagDATEDATA
{
	int						Year ;							// 年
	int						Mon ;							// 月
	int						Day ;							// 日
	int						Hour ;							// 時間
	int						Min ;							// 分
	int						Sec ;							// 秒
} DATEDATA, *LPDATEDATA ;

// 画面モード情報データ型
typedef struct tagDISPLAYMODEDATA
{
	int						Width ;				// 水平解像度
	int						Height ;			// 垂直解像度
	int						ColorBitDepth ;		// 色ビット深度
	int						RefreshRate ;		// リフレッシュレート( -1 の場合は規定値 )
} DISPLAYMODEDATA, *LPDISPLAYMODEDATA ;

// ファイル情報構造体
typedef struct tagFILEINFO
{
	char					Name[260] ;			// オブジェクト名
	int						DirFlag ;			// ディレクトリかどうか( TRUE:ディレクトリ  FALSE:ファイル )
	LONGLONG				Size ;				// サイズ
	DATEDATA				CreationTime ;		// 作成時刻
	DATEDATA				LastWriteTime ;		// 最終更新時刻
} FILEINFO, *LPFILEINFO ;

#ifndef DX_NOTUSE_DRAWFUNCTION

// 行列構造体
typedef struct tagMATRIX
{
	float					m[4][4] ;
} MATRIX, *LPMATRIX ;

// ベクトルデータ型
typedef struct tagVECTOR
{
	float					x, y, z ;
} VECTOR, *LPVECTOR, XYZ, *LPXYZ ;

// FLOAT2個データ型
typedef struct tagFLOAT2
{
	float					u, v ;
} UV ;

// float 型のカラー値
typedef struct tagCOLOR_F
{
	float					r, g, b, a ;
} COLOR_F, *LPCOLOR_F ;

// unsigned char 型のカラー値
typedef struct tagCOLOR_U8
{
	BYTE					b, g, r, a ;
} COLOR_U8 ;

// FLOAT4個データ型
typedef struct tagFLOAT4
{
	float					x, y, z, w ;
} FLOAT4, *LPFLOAT4 ;

// INT4個データ型
typedef struct tagINT4
{
	int						x, y, z, w ;
} INT4 ;

// ２Ｄ描画用頂点構造体(テクスチャ無し)
typedef struct tagVERTEX_NOTEX_2D
{
	VECTOR					pos ;
	float					rhw ;
	int						color ;
} VERTEX_NOTEX_2D, *LPVERTEX_NOTEX_2D ; 

// 主に２Ｄ描画に使用する頂点データ型
typedef struct tagVERTEX_2D
{
	VECTOR					pos ;
	float					rhw ;
	int						color ;
	float					u, v ;
} VERTEX_2D, *LPVERTEX_2D ; 

// ２Ｄ描画に使用する頂点データ型(DrawPrimitive2D用)
typedef struct tagVERTEX2D
{
	VECTOR					pos ;
	float					rhw ;
	COLOR_U8				dif ;
	float					u, v ;
} VERTEX2D, *LPVERTEX2D ; 

// ２Ｄ描画に使用する頂点データ型(DrawPrimitive2DToShader用)
typedef struct tagVERTEX2DSHADER
{
	VECTOR					pos ;
	float					rhw ;
	COLOR_U8				dif ;
	COLOR_U8				spc ;
	float					u, v ;
	float					su, sv ;
} VERTEX2DSHADER, *LPVERTEX2DSHADER ; 

// ２Ｄ描画に使用する頂点データ型(公開用)
typedef struct tagVERTEX
{
	float					x, y ;
	float					u, v ;
	unsigned char			b, g, r, a ;
} VERTEX ;

// ３Ｄ描画に使用する頂点データ型( 旧バージョンのもの )
typedef struct tagVERTEX_3D
{
	VECTOR					pos ;
	unsigned char			b, g, r, a ;
	float					u, v ;
} VERTEX_3D, *LPVERTEX_3D ;

// ３Ｄ描画に使用する頂点データ型
typedef struct tagVERTEX3D
{
	VECTOR					pos ;
	VECTOR					norm ;
	COLOR_U8				dif ;
	COLOR_U8				spc ;
	float					u, v ;
	float					su, sv ;
} VERTEX3D, *LPVERTEX3D ;

// ３Ｄ描画に使用する頂点データ型( DrawPrimitive3DToShader用 )
// 注意…メンバ変数に追加があるかもしれませんので、宣言時の初期化( VERTEX3DSHADER Vertex = { 0.0f, 0.0f, ... というようなもの )はしない方が良いです
typedef struct tagVERTEX3DSHADER
{
	VECTOR					pos ;
	VECTOR					norm ;
	COLOR_U8				dif ;
	COLOR_U8				spc ;
	float					u, v ;
	float					su, sv ;
} VERTEX3DSHADER, *LPVERTEX3DSHADER ;

// ライトパラメータ
typedef struct tagLIGHTPARAM
{
	int						LightType ;					// ライトのタイプ( DX_LIGHTTYPE_D3DLIGHT_POINT 等 )
	COLOR_F					Diffuse ;					// ディフューズカラー
	COLOR_F					Specular ;					// スペキュラカラー
	COLOR_F					Ambient ;					// アンビエント色
	VECTOR					Position ;					// 位置
	VECTOR					Direction ;					// 方向
	float					Range ;						// 有効距離
	float					Falloff ;					// フォールオフ 1.0f にしておくのが好ましい
	float					Attenuation0 ;				// 距離による減衰係数０
	float					Attenuation1 ;				// 距離による減衰係数１
	float					Attenuation2 ;				// 距離による減衰係数２
	float					Theta ;						// スポットライトの内部コーンの照明角度( ラジアン )
	float					Phi ;						// スポットライトの外部コーンの照明角度
} LIGHTPARAM ;

// マテリアルパラメータ
typedef struct tagMATERIALPARAM
{
	COLOR_F					Diffuse ;					// ディフューズカラー
	COLOR_F					Ambient ;					// アンビエントカラー
	COLOR_F					Specular ;					// スペキュラカラー
	COLOR_F					Emissive ;					// エミッシブカラー
	float					Power ;						// スペキュラハイライトの鮮明度
} MATERIALPARAM ;

// ラインヒットチェック結果格納用構造体
typedef struct tagHITRESULT_LINE
{
	int						HitFlag ;					// 当たったかどうか( 1:当たった  0:当たらなかった )
	VECTOR					Position ;					// 当たった座標
} HITRESULT_LINE ;

// コリジョン結果代入用ポリゴン
typedef struct tagMV1_COLL_RESULT_POLY
{
	int						HitFlag ;							// ( MV1CollCheck_Line でのみ有効 )ヒットフラグ( 1:ヒットした  0:ヒットしなかった )
	VECTOR					HitPosition ;						// ( MV1CollCheck_Line でのみ有効 )ヒット座標

	int						FrameIndex ;						// 当たったポリゴンが含まれるフレームの番号
	int						PolygonIndex ;						// 当たったポリゴンの番号
	int						MaterialIndex ;						// 当たったポリゴンが使用しているマテリアルの番号
	VECTOR					Position[ 3 ] ;						// 当たったポリゴンを形成する三点の座標
	VECTOR					Normal ;							// 当たったポリゴンの法線
} MV1_COLL_RESULT_POLY ;

// コリジョン結果代入用ポリゴン配列
typedef struct tagMV1_COLL_RESULT_POLY_DIM
{
	int						HitNum ;							// ヒットしたポリゴンの数
	MV1_COLL_RESULT_POLY *	Dim ;								// ヒットしたポリゴンの配列( HitNum個分存在する )
} MV1_COLL_RESULT_POLY_DIM ;

// 参照用頂点構造体
typedef struct tagMV1_REF_VERTEX
{
	VECTOR					Position ;							// 位置
	VECTOR					Normal ;							// 法線
	UV						TexCoord[ 2 ] ;						// テクスチャ座標
	COLOR_U8				DiffuseColor ;						// ディフューズカラー
	COLOR_U8				SpecularColor ;						// スペキュラカラー
} MV1_REF_VERTEX ;

// 参照用ポリゴン構造体
typedef struct tagMV1_REF_POLYGON
{
	unsigned short			FrameIndex ;						// このポリゴンが属しているフレーム
	unsigned short			MaterialIndex ;						// 使用しているマテリアル
	int						VIndexTarget ;						// VIndex が指すインデックスの参照先( 1:フレーム  0:モデル全体 )
	int						VIndex[ 3 ] ;						// ３角形ポリゴンを成す参照頂点のインデックス
	VECTOR					MinPosition ;						// ポリゴンを成す頂点座標の最小値
	VECTOR					MaxPosition ;						// ポリゴンを成す頂点座標の最大値
} MV1_REF_POLYGON ;

// 参照用ポリゴンデータ構造体
typedef struct tagMV1_REF_POLYGONLIST
{
	int						PolygonNum ;						// 参照用ポリゴンの数
	int						VertexNum ;							// 頂点の数
	VECTOR					MinPosition ;						// 頂点座標の最小値
	VECTOR					MaxPosition ;						// 頂点座標の最大値
	MV1_REF_POLYGON	*		Polygons ;							// 参照用ポリゴン配列
	MV1_REF_VERTEX *		Vertexs ;							// 参照用頂点配列
} MV1_REF_POLYGONLIST ;

#endif // DX_NOTUSE_DRAWFUNCTION






// ストリームデータ制御用関数ポインタ構造体タイプ２
typedef struct tagSTREAMDATASHREDTYPE2
{
	int						(*Open)( const char *Path, int UseCacheFlag, int BlockReadFlag, int UseASyncReadFlag ) ;
	int						(*Close)( int Handle ) ;
	long					(*Tell)( int Handle ) ;
	int						(*Seek)( int Handle, long SeekPoint, int SeekType ) ;
	size_t					(*Read)( void *Buffer, size_t BlockSize, size_t DataNum, int Handle ) ;
	int						(*Eof)( int Handle ) ;
	int						(*IdleCheck)( int Handle ) ;
	int						(*ChDir)( const char *Path ) ;
	int						(*GetDir)( char *Buffer ) ;
	int						(*FindFirst)( const char *FilePath, FILEINFO *Buffer ) ;	// 戻り値: -1=エラー  -1以外=FindHandle
	int						(*FindNext)( int FindHandle, FILEINFO *Buffer ) ;			// 戻り値: -1=エラー  0=成功
	int						(*FindClose)( int FindHandle ) ;							// 戻り値: -1=エラー  0=成功
} STREAMDATASHREDTYPE2 ;

// ストリームデータ制御用関数ポインタ構造体
typedef struct tagSTREAMDATASHRED
{
	long					(*Tell)( void *StreamDataPoint ) ;
	int						(*Seek)( void *StreamDataPoint, long SeekPoint, int SeekType ) ;
	size_t					(*Read)( void *Buffer, size_t BlockSize, size_t DataNum, void *StreamDataPoint ) ;
//	size_t					(*Write)( void *Buffer, size_t BlockSize, size_t DataNum, void *StreamDataPoint ) ;
	int						(*Eof)( void *StreamDataPoint ) ;
	int						(*IdleCheck)( void *StreamDataPoint ) ;
	int						(*Close)( void *StreamDataPoint ) ;
} STREAMDATASHRED, *LPSTREAMDATASHRED ;

// ストリームデータ制御用データ構造体
typedef struct tagSTREAMDATA
{
	STREAMDATASHRED			ReadShred ;
	void					*DataPoint ;
} STREAMDATA ;




#ifndef DX_NOTUSE_DRAWFUNCTION

// メモリに置かれたデータをファイルとして扱うためのデータ構造体
typedef struct tagMEMSTREAMDATA
{
	unsigned char			*DataBuffer ;
	unsigned int			DataSize ;
	int						DataPoint ;
	int						EOFFlag ;
} MEMSTREAMDATA, *LPMEMSTREAMDATA ;

// パレット情報構造体
typedef struct tagCOLORPALETTEDATA
{
	unsigned char			Blue ;
	unsigned char			Green ;
	unsigned char			Red ;
	unsigned char			Alpha ;
} COLORPALETTEDATA ;

// カラー構造情報構造体
typedef struct tagCOLORDATA
{
	unsigned short			ColorBitDepth ;									// ビット深度( この値が０の場合は Format が有効になる )
	unsigned short			PixelByte ;										// １ピクセルあたりのバイト数
	unsigned char			RedWidth, GreenWidth, BlueWidth, AlphaWidth ;	// 各色のビット幅
	unsigned char			RedLoc	, GreenLoc  , BlueLoc  , AlphaLoc   ;	// 各色の配置されているビットアドレス
	unsigned int			RedMask , GreenMask , BlueMask , AlphaMask  ;	// 各色のビットマスク
	unsigned int			NoneMask ;										// 使われていないビットのマスク
	COLORPALETTEDATA		Palette[256] ;									// パレット(ビット深度が８以下の場合のみ有効)
	unsigned char			NoneLoc, NoneWidth;								// 使われていないビットのアドレスと幅
	int						Format ;										// フォーマット( DX_BASEIMAGE_FORMAT_NORMAL 等 )
} COLORDATA, *LPCOLORDATA ;

// 基本イメージデータ構造体
typedef struct tagBASEIMAGE
{
	COLORDATA				ColorData ;							// 色情報
	int						Width, Height, Pitch ;				// 幅、高さ、ピッチ
	void					*GraphData ;						// グラフィックイメージ
	int						MipMapCount ;						// ミップマップの数
} BASEIMAGE, GRAPHIMAGE, *LPGRAPHIMAGE ;

// ラインデータ型
typedef struct tagLINEDATA
{
	int						x1, y1, x2, y2 ;					// 座標
	int						color ;								// 色
	int						pal ;								// パラメータ
} LINEDATA, *LPLINEDATA ;

// 座標データ型
typedef struct tagPOINTDATA
{
	int						x, y ;								// 座標
	int						color ;								// 色
	int						pal ;								// パラメータ
} POINTDATA, *LPPOINTDATA ;

// イメージフォーマットデータ
typedef struct tagIMAGEFORMATDESC
{
	unsigned char			TextureFlag ;					// テクスチャか、フラグ( TRUE:テクスチャ  FALSE:標準サーフェス )
	unsigned char			AlphaChFlag ;					// αチャンネルはあるか、フラグ	( TRUE:ある  FALSE:ない )
	unsigned char			DrawValidFlag ;					// 描画可能か、フラグ( TRUE:可能  FALSE:不可能 )
	unsigned char			SystemMemFlag ;					// システムメモリ上に存在しているか、フラグ( TRUE:システムメモリ上  FALSE:ＶＲＡＭ上 )( 標準サーフェスの時のみ有効 )
	unsigned char			NotManagedTextureFlag ;			// マネージドテクスチャを使用しないか、フラグ

	unsigned char			BaseFormat ;					// 基本フォーマット( DX_BASEIMAGE_FORMAT_NORMAL 等 )
	unsigned char			MipMapCount ;					// ミップマップの数
	unsigned char			AlphaTestFlag ;					// αテストチャンネルはあるか、フラグ( TRUE:ある  FALSE:ない )( テクスチャの場合のみ有効 )
	unsigned char			ColorBitDepth ;					// 色深度( テクスチャの場合のみ有効 )
	unsigned char			BlendGraphFlag ;				// ブレンド用画像か、フラグ
	unsigned char			UsePaletteFlag ;				// パレットを使用しているか、フラグ( SystemMemFlag が TRUE の場合のみ有効 )
} IMAGEFORMATDESC ;

#endif // DX_NOTUSE_DRAWFUNCTION







// WinSockets使用時のアドレス指定用構造体
typedef struct tagIPDATA
{
	union
	{
		struct
		{
			unsigned char	d1, d2, d3, d4 ;				// アドレス値
		} ;
		unsigned int		dall ;
	} ;
} IPDATA, *LPIPDATA ;

#define DX_STRUCT_END

}

// 関数プロトタイプ宣言------------------------------------------------------------------

namespace DxLib
{

#define DX_FUNCTION_START

// DxWin.cpp関数プロトタイプ宣言

// 初期化終了系関数
extern	int			DxLib_Init( void ) ;													// ライブラリ初期化関数
extern	int			DxLib_End( void ) ;														// ライブラリ使用の終了関数

extern	int			DxLib_GlobalStructInitialize( void ) ;									// ライブラリの内部で使用している構造体をゼロ初期化して、DxLib_Init の前に行った設定を無効化する( DxLib_Init の前でのみ有効 )
extern	int			DxLib_IsInit( void ) ;													// ライブラリが初期化されているかどうかを取得する( 戻り値: TRUE=初期化されている  FALSE=されていない )

// エラー処理関数
extern	BOOL		ErrorLogAdd( const char *ErrorStr ) ;									// エラー文書を書き出す
extern 	BOOL		ErrorLogFmtAdd( const char *FormatString , ... ) ;						// 書式付きエラー文書を書き出す
extern	BOOL		ErrorLogTabAdd( void ) ;												// タブ数を増やす
extern	BOOL		ErrorLogTabSub( void ) ;												// タブ数を減らす
extern	BOOL		SetUseTimeStampFlag( int UseFlag ) ;									// タイムスタンプの有無を設定する
extern 	BOOL		AppLogAdd( const char *String , ... ) ;									// 書式付きログ文字列を書き出す

// メモリ確保系関数
extern	void*		DxAlloc( size_t AllocSize , const char *File = NULL , int Line = -1 ) ;	// メモリを確保する
extern	void*		DxCalloc( size_t AllocSize , const char *File = NULL , int Line = -1 ) ;	// メモリを確保して０で初期化する
extern	void*		DxRealloc( void *Memory , size_t AllocSize , const char *File = NULL , int Line = -1 ) ;	// メモリの再確保を行う
extern	void		DxFree( void *Memory ) ;												// メモリを解放する
extern	size_t		DxSetAllocSizeTrap( size_t Size ) ;										// 列挙対象にするメモリの確保容量をセットする
extern	int			DxSetAllocPrintFlag( int Flag ) ;										// ＤＸライブラリ内でメモリ確保が行われる時に情報を出力するかどうかをセットする
extern	size_t		DxGetAllocSize( void ) ;												// DxAlloc や DxCalloc で確保しているメモリサイズを取得する
extern	int			DxGetAllocNum( void ) ;													// DxAlloc や DxCalloc で確保しているメモリの数を取得する
extern	void		DxDumpAlloc( void ) ;													// DxAlloc や DxCalloc で確保しているメモリを列挙する
extern	int			DxErrorCheckAlloc( void ) ;												// 確保したメモリ情報が破壊されていないか調べる( -1:破壊あり  0:なし )
extern	int			DxSetAllocSizeOutFlag( int Flag ) ;										// メモリが確保、解放が行われる度に確保しているメモリの容量を出力するかどうかのフラグをセットする
extern	int			DxSetAllocMemoryErrorCheckFlag( int Flag ) ;							// メモリの確保、解放が行われる度に確保しているメモリ確保情報が破損していないか調べるかどうかのフラグをセットする

// ログ出力機能関数
extern	int			SetLogDrawOutFlag( int DrawFlag ) ;										// ログ出力フラグをセットする
extern 	int			GetLogDrawFlag( void ) ;												// ログ出力をするかフラグの取得

// 簡易画面出力関数
extern 	int			printfDx( const char *FormatString , ... ) ;							// 簡易画面出力
extern	int			clsDx( void ) ;															// 簡易画面出力をクリアする

// ファイルアクセス関数
extern	int			FileRead_open( const char *FilePath , int ASync = FALSE ) ;			// ファイルを開く
extern	int			FileRead_size( const char *FilePath ) ;								// ファイルのサイズを得る
extern	int			FileRead_close( int FileHandle ) ;									// ファイルを閉じる
extern	int			FileRead_tell( int FileHandle ) ;									// ファイルポインタの現在位置を得る
extern	int			FileRead_seek( int FileHandle , int Offset , int Origin ) ;			// ファイルポインタの位置を変更する
extern	int			FileRead_read( void *Buffer , int ReadSize , int FileHandle ) ;		// ファイルからデータを読み込む
extern	int			FileRead_idle_chk( int FileHandle ) ;								// ファイル読み込みが完了しているかどうかを取得する
extern	int			FileRead_eof( int FileHandle ) ;									// ファイルの終端かどうかを得る
extern	int			FileRead_gets( char *Buffer , int BufferSize , int FileHandle ) ;	// ファイルから文字列を読み出す
extern	int			FileRead_getc( int FileHandle ) ;									// ファイルから一文字読み出す
extern	int			FileRead_scanf( int FileHandle , const char *Format , ... ) ;		// ファイルから書式化されたデータを読み出す

extern	int			FileRead_createInfo( const char *ObjectPath ) ;						// ファイル情報ハンドルを作成する( 戻り値  -1:エラー  -1以外:ファイル情報ハンドル )
extern	int			FileRead_getInfoNum( int FileInfoHandle ) ;							// ファイル情報ハンドル中のファイルの数を取得する
extern	int			FileRead_getInfo( int Index , FILEINFO *Buffer , int FileInfoHandle ) ;	// ファイル情報ハンドル中のファイルの情報を取得する
extern	int			FileRead_deleteInfo( int FileInfoHandle ) ;							// ファイル情報ハンドルを削除する

extern	int			FileRead_findFirst( const char *FilePath, FILEINFO *Buffer ) ;		// 指定のファイル又はフォルダの情報を取得し、ファイル検索ハンドルも作成する( 戻り値: -1=エラー  -1以外=ファイル検索ハンドル )
extern	int			FileRead_findNext( int FindHandle, FILEINFO *Buffer ) ;				// 条件の合致する次のファイルの情報を取得する( 戻り値: -1=エラー  0=成功 )
extern	int			FileRead_findClose( int FindHandle ) ;								// ファイル検索ハンドルを閉じる( 戻り値: -1=エラー  0=成功 )

// 便利関数
extern	int			GetResourceInfo( const char *ResourceName , const char *ResourceType , void **DataPointerP , int *DataSizeP ) ;		// 指定のリソースを取得する( -1:失敗  0:成功 )

// メッセージ処理関数
extern	int			ProcessMessage( void ) ;												// ウインドウズのメッセージループに代わる処理を行う

// ウインドウ関係情報取得関数
extern	int			GetWindowCRect( RECT *RectBuf ) ;										// ウインドウのクライアント領域を取得する
extern	int			GetWindowActiveFlag( void ) ;											// ウインドウのアクティブフラグを取得
extern	HWND		GetMainWindowHandle( void ) ;											// メインウインドウのハンドルを取得する
extern	int			GetWindowModeFlag( void ) ;												// ウインドウモードで起動しているか、のフラグを取得する
extern	int			GetDefaultState( int *SizeX , int *SizeY , int *ColorBitDepth ) ;		// 起動時のデスクトップの画面モードを取得する
extern	int			GetActiveFlag( void ) ;													// ソフトがアクティブかどうかを取得する
extern	int			GetNoActiveState( int ResetFlag = TRUE ) ;								// 非アクティブになり、処理が一時停止していたかどうかを取得する(引数 ResetFlag=TRUE:状態をリセット FALSE:状態をリセットしない    戻り値: 0=一時停止はしていない  1=一時停止していた )
extern	int			GetMouseDispFlag( void ) ;												// マウスを表示するかどうかのフラグを取得する
extern	int			GetAlwaysRunFlag( void ) ;												// ウインドウがアクティブではない状態でも処理を続行するか、フラグを取得する
extern	int			_GetSystemInfo( int *DxLibVer , int *DirectXVer , int *WindowsVer ) ;	// ＤＸライブラリと DirectX のバージョンと Windows のバージョンを得る
extern	int			GetPcInfo( char *OSString , char *DirectXString ,
								char *CPUString , int *CPUSpeed /* 単位MHz */ ,
								double *FreeMemorySize /* 単位MByte */ , double *TotalMemorySize ,
								char *VideoDriverFileName , char *VideoDriverString ,
								double *FreeVideoMemorySize /* 単位MByte */ , double *TotalVideoMemorySize ) ;	// ＰＣの情報を得る
extern	int			GetUseMMXFlag( void ) ;													// ＭＭＸが使えるかどうかの情報を得る
extern	int			GetUseSSEFlag( void ) ;													// ＳＳＥが使えるかどうかの情報を得る
extern	int			GetUseSSE2Flag( void ) ;												// ＳＳＥ２が使えるかどうかの情報を得る
extern	int			GetWindowCloseFlag( void ) ;											// ウインドウを閉じようとしているかの情報を得る
extern	HINSTANCE	GetTaskInstance( void ) ;												// ソフトのインスタンスを取得する
extern	int			GetUseWindowRgnFlag( void ) ;											// リージョンを使っているかどうかを取得する
extern	int			GetWindowSizeChangeEnableFlag( int *FitScreen = NULL ) ;				// ウインドウのサイズを変更できるかどうかのフラグを取得する
extern	double		GetWindowSizeExtendRate( double *ExRateX = NULL , double *ExRateY = NULL ) ;	// 描画画面のサイズに対するウインドウサイズの比率を取得する( 戻り値は ExRateX に代入される値と同じです )
extern	int			GetWindowSize( int *Width, int *Height ) ;								// ウインドウモードのウインドウののクライアント領域のサイズを取得する
extern	int			GetWindowPosition( int *x, int *y ) ;									// ウインドウモードのウインドウの位置を取得する( 枠も含めた左上座標 )
extern	int			GetWindowUserCloseFlag( int StateResetFlag = FALSE ) ;					// ウインドウの閉じるボタンが押されたかどうかを取得する
extern	int			GetNotDrawFlag( void ) ;												// 描画機能を使うかどうかのフラグを取得する
extern	int			GetPaintMessageFlag( void ) ;											// WM_PAINT メッセージが来たかどうかを取得する(戻り値  TRUE:WM_PAINTメッセージが来た(一度取得すると以後、再び WM_PAINTメッセージが来るまで FALSE が返ってくるようになる)  FALSE:WM_PAINT メッセージは来ていない)
extern	int			GetValidHiPerformanceCounter( void ) ;									// パフォーマンスカウンタが有効かどうかを取得する(戻り値  TRUE:有効  FALSE:無効)

// 設定系関数
extern	int			ChangeWindowMode( int Flag ) ;											// ウインドウモードを変更する
extern	int			LoadPauseGraph( const char *FileName ) ;								// アクティブウインドウが他のソフトに移っている際に表示する画像のロード(NULL で解除)
extern	int			LoadPauseGraphFromMem( void *MemImage , int MemImageSize ) ;			// アクティブウインドウが他のソフトに移っている際に表示する画像のロード(NULL で解除)
extern	int			SetActiveStateChangeCallBackFunction( int (* CallBackFunction )( int ActiveState , void * UserData ) , void *UserData ) ;	// ウインドウのアクティブ状態に変化があったときに呼ばれるコールバック関数をセットする( NULL をセットすると呼ばれなくなる )
extern	int			SetWindowText( const char *WindowText ) ;								// メインウインドウのウインドウテキストを変更する
extern	int			SetMainWindowText( const char *WindowText ) ;							// メインウインドウのウインドウテキストを変更する
extern	int			SetMainWindowClassName( const char *ClassName ) ;						// メインウインドウのクラス名を設定する
extern	int			SetOutApplicationLogValidFlag( int Flag ) ;								// ログ出力を行うか否かのセット
extern	int			SetAlwaysRunFlag( int Flag ) ;											// ウインドウがアクティブではない状態でも処理を続行するか、フラグをセットする
extern	int			SetWindowIconID( int ID ) ;												// 使用するアイコンのＩＤをセットする
extern	int			SetUseASyncChangeWindowModeFunction( int Flag ,
										 void (* CallBackFunction )( void * ) , void *Data ) ;	// 最大化ボタンやALT+ENTERキーによる非同期なウインドウモードの変更の機能の設定を行う
extern	int			SetWindowStyleMode( int Mode ) ;										// ウインドウのスタイルを変更する
extern	int			SetWindowSizeChangeEnableFlag( int Flag, int FitScreen = TRUE ) ;		// ウインドウのサイズを変更できるかどうかのフラグをセットする( NotFitScreen:ウインドウのクライアント領域に画面をフィットさせる(拡大させる)かどうか  TRUE:フィットさせる  FALSE:フィットさせない )
extern	int			SetWindowSizeExtendRate( double ExRateX, double ExRateY = -1.0 ) ;		// 描画画面のサイズに対するウインドウサイズの比率を設定する( ExRateY がマイナスの場合は ExRateX の値が ExRateY にも使用されます )
extern	int			SetWindowSize( int Width, int Height ) ;								// ウインドウモード時のウインドウのクライアント領域のサイズを設定する
extern	int			SetWindowPosition( int x, int y ) ;										// ウインドウモードのウインドウの位置を設定する( 枠も含めた左上座標 )
extern	int			SetSysCommandOffFlag( int Flag , const char *HookDllPath = NULL ) ;	// タスクスイッチを有効にするかどうかを設定する
extern	int			SetHookWinProc( WNDPROC WinProc ) ;										// メッセージをフックするウインドウプロージャを登録する
extern	int			SetDoubleStartValidFlag( int Flag ) ;									// ２重起動を許すかどうかのフラグをセットする
extern	int			AddMessageTakeOverWindow( HWND Window ) ;								// メッセージ処理をＤＸライブラリに肩代わりしてもらうウインドウを追加する
extern	int			SubMessageTakeOverWindow( HWND Window ) ;								// メッセージ処理をＤＸライブラリに肩代わりしてもらうウインドウを減らす

extern	int			SetWindowInitPosition( int x , int y ) ;									// ウインドウの初期位置を設定する
extern	int			SetNotWinFlag( int Flag ) ;												// ＤＸライブラリのウインドウ関連の機能を使用しないフラグ
extern	int			SetNotDrawFlag( int Flag ) ;											// 描画機能を使うかどうかのフラグをセットする
extern	int			SetNotSoundFlag( int Flag ) ;											// サウンド機能を使うかどうかのフラグをセットする
extern	int			SetNotInputFlag( int Flag ) ;											// 入力状態の取得機能を使うかどうかのフラグをセットする
extern	int			SetDialogBoxHandle( HWND WindowHandle ) ;								// ＤＸライブラリでメッセージ処理を行うダイアログボックスを登録する
extern	int			ChangeStreamFunction( STREAMDATASHREDTYPE2 *StreamThread ) ;			// ＤＸライブラリでストリームデータアクセスに使用する関数を変更する
extern	int			GetStreamFunctionDefault( void ) ;										// ＤＸライブラリでストリームデータアクセスに使用する関数がデフォルトのものか調べる( TRUE:デフォルト  FALSE:デフォルトではない )
extern	int			SetWindowVisibleFlag( int Flag ) ;										// メインウインドウを表示するかどうかのフラグをセットする
extern	int			SetWindowUserCloseEnableFlag( int Flag ) ;								// メインウインドウの×ボタンを押した時にライブラリが自動的にウインドウを閉じるかどうかのフラグをセットする
extern	int			SetDxLibEndPostQuitMessageFlag( int Flag ) ;							// ＤＸライブラリ終了時に PostQuitMessage を呼ぶかどうかのフラグをセットする
extern	int			SetUserWindow( HWND WindowHandle ) ;									// ＤＸライブラリで利用するウインドウのハンドルをセットする(DxLib_Init を実行する以前でのみ有効)
extern	int			SetUserWindowMessageProcessDXLibFlag( int Flag ) ;						// SetUseWindow で設定したウインドウのメッセージループ処理をＤＸライブラリで行うかどうか、フラグをセットする
extern	int			SetUseDXArchiveFlag( int Flag ) ;										// ＤＸアーカイブファイルの読み込み機能を使うかどうかを設定する( FALSE:使用しない  TRUE:使用する )
extern	int			SetDXArchiveExtension( const char *Extension = NULL ) ;					// 検索するＤＸアーカイブファイルの拡張子を変更する
extern	int			SetDXArchiveKeyString( const char *KeyString = NULL ) ;					// ＤＸアーカイブファイルの鍵文字列を設定する
extern	int			SetUseDateNameLogFile( int Flag ) ;										// ログファイル名に日付をつけるかどうかをセットする
extern	int			SetBackgroundColor( int Red, int Green, int Blue ) ;					// メインウインドウのバックグラウンドカラーを設定する
extern	int			SetLogFontSize( int Size ) ;											// printfDx で画面に出力するログフォントのサイズを変更する
extern	int			SetUseFPUPreserveFlag( int Flag ) ;										// FPUの精度を落とさない設定を使用するかどうかを設定する、DxLib_Init を呼び出す前のみ有効( TRUE:使用する(精度が落ちない)  FALSE:使用しない(精度を落とす(デフォルト) )
extern	int			SetValidMousePointerWindowOutClientAreaMoveFlag( int Flag ) ;			// マウスポインタがウインドウのクライアントエリアの外にいけるかどうかを設定する( TRUE:いける( デフォルト設定 )  FALSE:いけない )
extern	int			SetUseBackBufferTransColorFlag( int Flag ) ;							// バックバッファの透過色の部分を透過させるかどうかを設定する( TRUE:透過させる  FALSE:透過させない )
extern	int			SetResourceModule( HMODULE ResourceModule ) ;							// リソースを読み込む際に使用するモジュールを設定する( NULL を指定すると初期状態に戻ります、デフォルトでは NULL )

// クリップボード関係
extern	int			GetClipboardText( char *DestBuffer ) ;									// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern	int			SetClipboardText( const char *Text ) ;									// クリップボードにテキストデータを格納する

// ドラッグ＆ドロップされたファイル関係
extern	int			SetDragFileValidFlag( int Flag ) ;										// ファイルのドラッグ＆ドロップ機能を有効にするかどうかのフラグをセットする
extern	int			DragFileInfoClear( void ) ;												// ドラッグ＆ドロップされたファイルの情報を初期化する
extern	int			GetDragFilePath( char *FilePathBuffer ) ;								// ドラッグ＆ドロップされたファイル名を取得する( -1:取得できなかった  0:取得できた )
extern	int			GetDragFileNum( void ) ;												// ドラッグ＆ドロップされたファイルの数を取得する

// ウインドウ描画領域設定系関数
extern	HRGN		CreateRgnFromGraph( int Width , int Height , void *MaskData , int Pitch , int Byte ) ; // 任意のグラフィックからRGNハンドルを作成する
extern	HRGN		CreateRgnFromBaseImage( BASEIMAGE *BaseImage, int TransColorR, int TransColorG, int TransColorB ) ;		// 任意の基本イメージデータと透過色からRGNハンドルを作成する
extern	int			SetWindowRgnGraph( const char *FileName ) ;								// 任意のグラフィックからＲＧＮをセットする
extern	int			UpdateTransColorWindowRgn( void ) ;										// 描画先の画面の透過色の部分を透過させるＲＧＮをセットする

// ツールバー関係
extern	int			SetupToolBar( const char *BitmapName , int DivNum , int ResourceID = -1 ) ;				// ツールバーの準備( NULL を指定するとツールバーを解除、ResourceID は BitmapName が NULL の際に使用される )
extern	int			AddToolBarButton( int Type /* TOOLBUTTON_TYPE_NORMAL 等 */ , int State /* TOOLBUTTON_STATE_ENABLE 等 */ , int ImageIndex, int ID ) ;		// ツールバーにボタンを追加
extern	int			AddToolBarSep( void ) ;												// ツールバーに隙間を追加
extern	int			GetToolBarButtonState( int ID ) ;									// ツールバーのボタンの状態を取得( TRUE:押されている or 押された  FALSE:押されていない )
extern	int			SetToolBarButtonState( int ID , int State ) ;						// ツールバーのボタンの状態を設定
extern	int			DeleteAllToolBarButton( void ) ;									// ツールバーのボタンを全て削除

// メニュー関係
extern	int			SetUseMenuFlag( int Flag ) ;												// メニューを有効にするかどうかを設定する
extern	int			SetUseKeyAccelFlag( int Flag ) ;											// キーボードアクセラレーターを使用するかどうかを設定する

extern	int			AddKeyAccel( const char *ItemName , int ItemID , int KeyCode , int CtrlFlag , int AltFlag , int ShiftFlag ) ;	// ショートカットキーを追加する
extern	int			AddKeyAccel_Name( const char *ItemName , int KeyCode , int CtrlFlag , int AltFlag , int ShiftFlag ) ;	// ショートカットキーを追加する
extern	int			AddKeyAccel_ID( int ItemID, int KeyCode, int CtrlFlag, int AltFlag, int ShiftFlag ) ;	// ショートカットキーを追加する
extern	int			ClearKeyAccel( void ) ;														// ショートカットキーの情報を初期化する

extern	int			AddMenuItem( int AddType /* MENUITEM_ADD_CHILD等 */ , const char *ItemName, int ItemID,
									int SeparatorFlag, const char *NewItemName = NULL , int NewItemID = -1 ) ;	// メニューに項目を追加する
extern	int			DeleteMenuItem( const char *ItemName, int ItemID ) ;						// メニューから選択項目を削除する
extern	int			CheckMenuItemSelect( const char *ItemName, int ItemID ) ;					// メニューが選択されたかどうかを取得する( 0:選択されていない  1:選択された )
extern	int			SetMenuItemEnable( const char *ItemName, int ItemID, int EnableFlag ) ;		// メニューの項目を選択出来るかどうかを設定する
extern	int			SetMenuItemMark( const char *ItemName, int ItemID, int Mark ) ;				// メニューの項目にチェックマークやラジオボタンを表示するかどうかを設定する( MENUITEM_MARK_NONE 等 )
extern	int			CheckMenuItemSelectAll( void ) ;											// メニューの項目がどれか選択されたかどうかを取得する( 戻り値  TRUE:どれか選択された  FALSE:選択されていない )

extern	int			AddMenuItem_Name( const char *ParentItemName, const char *NewItemName ) ;	// メニューに選択項目を追加する
extern	int			AddMenuLine_Name( const char *ParentItemName ) ;							// メニューのリストに区切り線を追加する
extern	int			InsertMenuItem_Name( const char *ItemName, const char *NewItemName ) ;		// 指定の項目と、指定の項目の一つ上の項目との間に新しい項目を追加する
extern	int			InsertMenuLine_Name( const char *ItemName ) ;								// 指定の項目と、指定の項目の一つ上の項目との間に区切り線を追加する
extern	int			DeleteMenuItem_Name( const char *ItemName ) ;								// メニューから選択項目を削除する
extern	int			CheckMenuItemSelect_Name( const char *ItemName ) ;							// メニューが選択されたかどうかを取得する( 0:選択されていない  1:選択された )
extern	int			SetMenuItemEnable_Name( const char *ItemName, int EnableFlag ) ;			// メニューの項目を選択出来るかどうかを設定する( 1:選択できる  0:選択できない )
extern	int			SetMenuItemMark_Name( const char *ItemName, int Mark ) ;					// メニューの項目にチェックマークやラジオボタンを表示するかどうかを設定する( MENUITEM_MARK_NONE 等 )

extern	int			AddMenuItem_ID( int ParentItemID, const char *NewItemName, int NewItemID = -1 ) ;	// メニューに選択項目を追加する
extern	int			AddMenuLine_ID( int ParentItemID ) ;										// メニューのリストに区切り線を追加する
extern	int			InsertMenuItem_ID( int ItemID, int NewItemID ) ;							// 指定の項目と、指定の項目の一つ上の項目との間に新しい項目を追加する
extern	int			InsertMenuLine_ID( int ItemID, int NewItemID ) ;							// 指定の項目と、指定の項目の一つ上の項目との間に区切り線を追加する
extern	int			DeleteMenuItem_ID( int ItemID ) ;											// メニューから選択項目を削除する
extern	int			CheckMenuItemSelect_ID( int ItemID ) ;										// メニューが選択されたかどうかを取得する( 0:選択されていない  1:選択された )
extern	int			SetMenuItemEnable_ID( int ItemID, int EnableFlag ) ;						// メニューの項目を選択出来るかどうかを設定する
extern	int			SetMenuItemMark_ID( int ItemID, int Mark ) ;								// メニューの項目にチェックマークやラジオボタンを表示するかどうかを設定する( MENUITEM_MARK_NONE 等 )

extern	int			DeleteMenuItemAll( void ) ;													// メニューの全ての選択項目を削除する
extern	int			ClearMenuItemSelect( void ) ;												// メニューが選択されたかどうかの情報を初期化
extern	int			GetMenuItemID( const char *ItemName ) ;										// メニューの項目名からＩＤを取得する
extern	int			GetMenuItemName( int ItemID, char *NameBuffer ) ;							// メニューの項目名からＩＤを取得する
extern	int 		LoadMenuResource( int MenuResourceID ) ;									// メニューをリソースから読み込む
extern	int			SetMenuItemSelectCallBackFunction( void (* CallBackFunction )( const char *ItemName, int ItemID ) ) ; // メニューの選択項目が選択されたときに呼ばれるコールバック関数を設定する

extern	int			SetWindowMenu( int MenuID, int (* MenuProc )( WORD ID ) ) ;				// (古い関数)ウインドウにメニューを設定する
extern	int			SetDisplayMenuFlag( int Flag ) ;										// メニューを表示するかどうかをセットする
extern	int			GetDisplayMenuFlag( void ) ;											// メニューを表示しているかどうかを取得する
extern	int			GetUseMenuFlag( void ) ;												// メニューを使用しているかどうかを得る
extern	int			SetAutoMenuDisplayFlag( int Flag ) ;									// フルスクリーン時にメニューを自動で表示したり非表示にしたりするかどうかのフラグをセットする

// マウス関係関数
extern	int			SetMouseDispFlag( int DispFlag ) ;										// マウスの表示フラグのセット
extern	int			GetMousePoint( int *XBuf, int *YBuf ) ;									// マウスの位置を取得する
extern	int			SetMousePoint( int PointX, int PointY ) ;								// マウスの位置をセットする
extern	int			GetMouseInput( void ) ;													// マウスのボタンの状態を得る 
extern	int			GetMouseWheelRotVol( int CounterReset = TRUE ) ;						// マウスホイールの回転量を得る

// ウエイト系関数
extern	int			WaitTimer( int WaitTime ) ;												// 指定の時間だけ処理をとめる
extern	int			WaitKey( void ) ;														// キーの入力待ち

// カウンタ及び時刻取得系関数
extern	int			GetNowCount( int UseRDTSCFlag = FALSE ) ;								// ミリ秒単位の精度を持つカウンタの現在値を得る
extern	LONGLONG	GetNowHiPerformanceCount( int UseRDTSCFlag = FALSE ) ;					// GetNowCountの高精度バージョン
extern	int			GetDateTime( DATEDATA *DateBuf ) ;										// 現在時刻を取得する 

// 乱数取得
extern	int			GetRand( int RandMax ) ;												// 乱数を取得する( RandMax : 返って来る値の最大値 )
extern	int			SRand( int Seed ) ;														// 乱数の初期値を設定する

#ifndef DX_NON_NETWORK

// 通信関係
extern	int			ProcessNetMessage( int RunReleaseProcess = FALSE ) ;					// 通信メッセージの処理をする関数

extern	int			GetHostIPbyName( const char *HostName, IPDATA *IPDataBuf ) ;			// ＤＮＳサーバーを使ってホスト名からＩＰアドレスを取得する
extern 	int			ConnectNetWork( IPDATA IPData, int Port = -1 ) ;							// 他マシンに接続する
extern 	int			PreparationListenNetWork( int Port = -1 ) ;								// 接続を受けられる状態にする
extern 	int			StopListenNetWork( void ) ;												// 接続を受けつけ状態の解除
extern 	int			CloseNetWork( int NetHandle ) ;											// 接続を終了する

extern 	int			GetNetWorkAcceptState( int NetHandle ) ;								// 接続状態を取得する
extern 	int			GetNetWorkDataLength( int NetHandle ) ;									// 受信データの量を得る
extern	int			GetNetWorkSendDataLength( int NetHandle ) ;								// 未送信のデータの量を得る 
extern 	int			GetNewAcceptNetWork( void ) ;											// 新たに接続した通信回線を得る
extern 	int			GetLostNetWork( void ) ;												// 接続を切断された通信回線を得る
extern 	int			GetNetWorkIP( int NetHandle, IPDATA *IpBuf ) ;							// 接続先のＩＰを得る
extern	int			GetMyIPAddress( IPDATA *IpBuf ) ;										// 自分のＩＰを得る
extern	int			SetConnectTimeOutWait( int Time ) ;										// 接続のタイムアウトまでの時間を設定する
extern	int			SetUseDXNetWorkProtocol( int Flag ) ;									// ＤＸライブラリの通信形態を使うかどうかをセットする
extern	int			GetUseDXNetWorkProtocol( void ) ; 										// ＤＸライブラリの通信形態を使うかどうかを取得する
extern	int			SetUseDXProtocol( int Flag ) ;											// SetUseDXNetWorkProtocol の別名
extern	int			GetUseDXProtocol( void ) ; 												// GetUseDXNetWorkProtocol の別名
extern	int			SetNetWorkCloseAfterLostFlag( int Flag ) ;								// 接続が切断された直後に接続ハンドルを解放するかどうかのフラグをセットする
extern	int			GetNetWorkCloseAfterLostFlag( void ) ;									// 接続が切断された直後に接続ハンドルを解放するかどうかのフラグを取得する
extern	int			SetProxySetting( int UseFlag, char *Address, int Port ) ;						// ＨＴＴＰ通信で使用するプロキシ設定を行う
extern	int			GetProxySetting( int *UseFlagBuffer, char *AddressBuffer, int *PortBuffer ) ;	// ＨＴＴＰ通信で使用するプロキシ設定を取得する
extern	int			SetIEProxySetting( void ) ;												// ＩＥのプロキシ設定を適応する


extern 	int			NetWorkRecv( int NetHandle, void *Buffer, int Length ) ;				// 受信したデータを読み込む
extern	int			NetWorkRecvToPeek( int NetHandle, void *Buffer, int Length ) ;			// 受信したデータを読み込む、読み込んだデータはバッファから削除されない
extern	int			NetWorkRecvBufferClear( int NetHandle ) ;								// 受信したデータをクリアする
extern 	int			NetWorkSend( int NetHandle, void *Buffer, int Length ) ;				// データを送信する

/*	使用不可
extern	int			HTTP_FileDownload( const char *FileURL, const char *SavePath = NULL ,
										 void **SaveBufferP = NULL , int *FileSize = NULL ,
										 char **ParamList = NULL ) ;						// HTTP を使用してネットワーク上のファイルをダウンロードする
extern	int			HTTP_GetFileSize( const char *FileURL ) ;								// HTTP を使用してネットワーク上のファイルのサイズを得る

extern	int			HTTP_StartFileDownload( const char *FileURL, const char *SavePath, void **SaveBufferP = NULL , char **ParamList = NULL ) ;	// HTTP を使用したネットワーク上のファイルをダウンロードする処理を開始する
extern	int			HTTP_StartGetFileSize( const char *FileURL ) ;							// HTTP を使用したネットワーク上のファイルのサイズを得る処理を開始する
extern	int			HTTP_Close( int HttpHandle ) ;											// HTTP の処理を終了し、ハンドルを解放する
extern	int			HTTP_CloseAll( void ) ;													// 全てのハンドルに対して HTTP_Close を行う
extern	int			HTTP_GetState( int HttpHandle ) ;										// HTTP 処理の現在の状態を得る( NET_RES_COMPLETE 等 )
extern	int			HTTP_GetError( int HttpHandle ) ;										// HTTP 処理でエラーが発生した場合、エラーの内容を得る( HTTP_ERR_NONE 等 )
extern	int			HTTP_GetDownloadFileSize( int HttpHandle ) ;							// HTTP 処理で対象となっているファイルのサイズを得る( 戻り値: -1 = エラー・若しくはまだファイルのサイズを取得していない  0以上 = ファイルのサイズ )
extern	int			HTTP_GetDownloadedFileSize( int HttpHandle ) ;							// HTTP 処理で既にダウンロードしたファイルのサイズを取得する

extern	int			fgetsForNetHandle( int NetHandle, char *strbuffer ) ;					// fgets のネットワークハンドル版( -1:取得できず 0:取得できた )
extern	int			URLAnalys( const char *URL, char *HostBuf = NULL , char *PathBuf = NULL ,
												 char *FileNameBuf = NULL , int *PortBuf = NULL ) ;	// ＵＲＬを解析する
extern	int			URLConvert( char *URL, int ParamConvert = TRUE , int NonConvert = FALSE ) ;	// HTTP に渡せない記号が使われた文字列を渡せるような文字列に変換する( 戻り値: -1 = エラー  0以上 = 変換後の文字列のサイズ )
extern	int			URLParamAnalysis( char **ParamList, char **ParamStringP ) ;				// HTTP 用パラメータリストから一つのパラメータ文字列を作成する( 戻り値:  -1 = エラー  0以上 = パラメータの文字列の長さ )
*/

#endif


// 文字コードバッファ操作関係
extern	int			StockInputChar( char CharCode ) ;																												// バッファにコードをストックする
extern	int			ClearInputCharBuf( void ) ;																														// 文字コードバッファをクリアする
extern	char		GetInputChar( int DeleteFlag ) ;																												// 文字コードバッファに溜まったデータから文字コードを取得する
extern	char		GetInputCharWait( int DeleteFlag ) ;																											// 文字コードバッファに溜まったデータから１バイト分取得する、バッファになにも文字コードがない場合はキーが押されるまで待つ

extern	int			GetOneChar( char *CharBuffer, int DeleteFlag ) ;																								// 文字コードバッファに溜まったデータから１文字分取得する
extern	int			GetOneCharWait( char *CharBuffer, int DeleteFlag ) ;																							// 文字コードバッファに溜まったデータから１文字分取得する、バッファに何も文字コードがない場合はキーが押されるまで待つ
extern	int			GetCtrlCodeCmp( char Char ) ;																													// アスキーコントロールコードか調べる

#ifndef DX_NON_KEYEX

extern	int			DrawIMEInputString( int x, int y, int SelectStringNum ) ;																						// 画面上に入力中の文字列を描画する
extern	int			SetUseIMEFlag( int UseFlag ) ;																													// ＩＭＥを使用するかどうかをセットする
extern	int			SetInputStringMaxLengthIMESync( int Flag ) ;																									// ＩＭＥで入力できる最大文字数を MakeKeyInput の設定に合わせるかどうかをセットする( TRUE:あわせる  FALSE:あわせない(デフォルト) )
extern	int			SetIMEInputStringMaxLength( int Length ) ;																										// ＩＭＥで一度に入力できる最大文字数を設定する( 0:制限なし  1以上:指定の文字数で制限 )

#endif

extern	int			GetStringPoint( const char *String, int Point ) ;																								// 全角文字、半角文字入り乱れる中から指定の文字数での半角文字数を得る
extern	int			GetStringPoint2( const char *String, int Point ) ;																								// 全角文字、半角文字入り乱れる中から指定の文字数での全角文字数を得る

extern	int			DrawObtainsString( int x, int y, int AddY, const char *String, int StrColor, int StrEdgeColor = 0 , int FontHandle = -1 , int SelectBackColor = -1 , int SelectStrColor = 0 , int SelectStrEdgeColor = -1 , int SelectStart = -1 , int SelectEnd = -1 ) ;					// 規定領域に収めたかたちで文字列を描画
extern	int			DrawObtainsBox( int x1, int y1, int x2, int y2, int AddY, int Color, int FillFlag ) ;															// 規定領域に収めたかたちで矩形を描画 

#ifndef DX_NON_KEYEX

extern	int			InputStringToCustom( int x, int y, int BufLength, char *StrBuffer, int CancelValidFlag, int SingleCharOnlyFlag, int NumCharOnlyFlag ) ;			// 文字列の入力取得

extern	int			KeyInputString( int x, int y, int CharMaxLength, char *StrBuffer, int CancelValidFlag ) ;														// 文字列の入力取得
extern	int			KeyInputSingleCharString( int x, int y, int CharMaxLength, char *StrBuffer, int CancelValidFlag ) ;												// 半角文字列のみの入力取得
extern	int			KeyInputNumber( int x, int y, int MaxNum, int MinNum, int CancelValidFlag ) ;																	// 数値の入力

extern	int			GetIMEInputModeStr( char *GetBuffer ) ;																											// IMEの入力モード文字列の取得
extern	IMEINPUTDATA* GetIMEInputData( void ) ;																														// IMEで入力中の文字列の情報を取得する
extern	int			SetKeyInputStringColor( ULONGLONG NmlStr, ULONGLONG NmlCur, ULONGLONG IMEStr, ULONGLONG IMECur, ULONGLONG IMELine, ULONGLONG IMESelectStr, ULONGLONG IMEModeStr , ULONGLONG NmlStrE = 0 , ULONGLONG IMESelectStrE = 0 , ULONGLONG IMEModeStrE = 0 , ULONGLONG IMESelectWinE = 0xffffffffffffffff ,	ULONGLONG IMESelectWinF = 0xffffffffffffffff , ULONGLONG SelectStrBackColor = 0xffffffffffffffff , ULONGLONG SelectStrColor = 0xffffffffffffffff , ULONGLONG SelectStrEdgeColor = 0xffffffffffffffff ) ;	// InputString関数使用時の文字の各色を変更する
extern	int			SetKeyInputStringFont( int FontHandle ) ;																										// キー入力文字列描画関連で使用するフォントのハンドルを変更する(-1でデフォルトのフォントハンドル)
extern	int			DrawKeyInputModeString( int x, int y ) ;																										// 入力モード文字列を描画する

extern	int			InitKeyInput( void ) ;																															// キー入力データ初期化
extern	int			MakeKeyInput( int MaxStrLength, int CancelValidFlag, int SingleCharOnlyFlag, int NumCharOnlyFlag ) ;											// 新しいキー入力データの作成
extern	int			DeleteKeyInput( int InputHandle ) ;																												// キー入力データの削除
extern	int			SetActiveKeyInput( int InputHandle ) ;																											// 指定のキー入力をアクティブにする( -1 を指定するとアクティブなキー入力ハンドルが無い状態になります )
extern	int			GetActiveKeyInput( void ) ;																														// 現在アクティブになっているキー入力ハンドルを取得する
extern	int			CheckKeyInput( int InputHandle ) ;																												// 入力が終了しているか取得する
extern	int			ReStartKeyInput( int InputHandle ) ;																											// 入力が完了したキー入力を再度編集状態に戻す
extern	int			ProcessActKeyInput( void ) ;																													// キー入力処理関数
extern	int			DrawKeyInputString( int x, int y, int InputHandle ) ;																							// キー入力中データの描画

extern	int			SetKeyInputSelectArea( int SelectStart, int SelectEnd, int InputHandle ) ;																		// キー入力データの指定の領域を選択状態にする( SelectStart と SelectEnd に -1 を指定すると選択状態が解除されます )
extern	int			GetKeyInputSelectArea( int *SelectStart, int *SelectEnd, int InputHandle ) ;																	// キー入力データの選択領域を取得する
extern	int			SetKeyInputDrawStartPos( int DrawStartPos, int InputHandle ) ;																					// キー入力の描画開始文字位置を設定する
extern	int			GetKeyInputDrawStartPos( int InputHandle ) ;																									// キー入力の描画開始文字位置を取得する
extern	int			SetKeyInputCursorBrinkTime( int Time ) ;																										// キー入力時のカーソルの点滅する早さをセットする
extern	int			SetKeyInputCursorBrinkFlag( int Flag ) ;																										// キー入力時のカーソルを点滅させるかどうかをセットする
extern	int			SetKeyInputString( const char *String, int InputHandle ) ;																						// キー入力データに指定の文字列をセットする
extern	int			SetKeyInputNumber( int Number, int InputHandle ) ;																								// キー入力データに指定の数値を文字に置き換えてセットする
extern	int			SetKeyInputNumberToFloat( float Number, int InputHandle ) ;																						// キー入力データに指定の浮動小数点値を文字に置き換えてセットする
extern	int			GetKeyInputString( char *StrBuffer, int InputHandle ) ;																							// 入力データの文字列を取得する
extern	int			GetKeyInputNumber( int InputHandle ) ;																											// 入力データの文字列を整数値として取得する
extern	float		GetKeyInputNumberToFloat( int InputHandle ) ;																									// 入力データの文字列を浮動小数点値として取得する
extern	int			SetKeyInputCursorPosition( int Position, int InputHandle ) ;																									// キー入力の現在のカーソル位置を設定する
extern	int			GetKeyInputCursorPosition( int InputHandle ) ;																									// キー入力の現在のカーソル位置を取得する

#endif


// メモリに置かれたデータをファイルのデータに例えてつかうための関数
extern	void*		MemStreamOpen( void *DataBuffer, unsigned int DataSize ) ;
extern	int			MemStreamClose( void *StreamDataPoint ) ;












// DxInput.cpp関数プロトタイプ宣言

// 入力状態取得関数
extern	int			CheckHitKey( int KeyCode ) ;											// キーボードの入力状態取得
extern	int			CheckHitKeyAll( int CheckType = DX_CHECKINPUT_ALL ) ;					// 全キーの押下状態を取得
extern	int			GetHitKeyStateAll( DX_CHAR *KeyStateBuf ) ;								// すべてのキーの押下状態を取得する
extern	int			SetKeyExclusiveCooperativeLevelFlag( int Flag ) ;						// キーボードの協調レベルを排他レベルにするかどうかのフラグをセットする
extern	int			GetJoypadNum( void ) ;													// ジョイパッドが接続されている数を取得する
extern	int			GetJoypadInputState( int InputType ) ;									// ジョイバッドの入力状態取得
extern	int			GetJoypadAnalogInput( int *XBuf, int *YBuf, int InputType ) ;			// ジョイパッドのアナログ的なスティック入力情報を得る
extern	int			GetJoypadAnalogInputRight( int *XBuf, int *YBuf, int InputType ) ;		// ジョイパッドのアナログ的なスティック入力情報を得る(右スティック用)
extern	int			KeyboradBufferProcess( void ) ;											// キーボードのバッファからデータを取得する処理
extern	int			GetJoypadGUID( int PadIndex, GUID *GuidBuffer ) ;						// ジョイパッドのＧＵIＤを得る
extern	int			ConvertKeyCodeToVirtualKey( int KeyCode ) ;								// ＤＸライブラリのキーコードから Windows の仮想キーコードを取得する
extern	int			SetJoypadInputToKeyInput( int InputType, int PadInput, int KeyInput1, int KeyInput2 = -1 , int KeyInput3 = -1 , int KeyInput4 = -1  ) ; // ジョイパッドの入力に対応したキーボードの入力を設定する
extern	int			SetJoypadDeadZone( int InputType, double Zone ) ;						// ジョイパッドの無効ゾーンの設定を行う
extern	int			StartJoypadVibration( int InputType, int Power, int Time ) ;			// ジョイパッドの振動を開始する
extern	int			StopJoypadVibration( int InputType ) ;									// ジョイパッドの振動を停止する
extern	int			GetJoypadPOVState( int InputType, int POVNumber ) ;						// ジョイパッドのＰＯＶ入力の状態を得る( 単位は角度の１００倍  中心位置にある場合は -1 が返る )
extern	int			GetJoypadName( int InputType, char *InstanceNameBuffer, char *ProductNameBuffer ) ;	// ジョイパッドのデバイス登録名と製品登録名を取得する
extern	int			ReSetupJoypad( void ) ;													// ジョイパッドの再セットアップを行う( 新たに接続されたジョイパッドがある場合に検出される )

extern	int			SetKeyboardNotDirectInputFlag( int Flag ) ;								// キーボードの入力処理に DirectInput を使わないか、フラグをセットする
extern	int			SetUseDirectInputFlag( int Flag ) ;										// 入力処理に DirectInput を使用するかどうかのフラグをセットする








#ifndef DX_NOTUSE_DRAWFUNCTION

// DxDraw.cpp関数プロトタイプ宣言

// 設定関係関数
extern	int			Set2D3DKyouzonFlag( int Flag ) ;							// ３Ｄと２Ｄの共存率を上げるフラグのセット
extern	int			SetNotUse3DFlag( int Flag ) ;								// ３Ｄ機能を使わないフラグのセット
extern	int			SetBasicBlendFlag( int Flag ) ;								// 簡略化ブレンド処理を行うか否かのフラグをセットする
extern	int			SetScreenMemToVramFlag( int Flag ) ;						// 画面データをＶＲＡＭに置くか、フラグ
extern	int			SetUseSoftwareRenderModeFlag( int Flag ) ;					// ソフトウエアレンダリングモードを使用するかどうかをセットする
extern	int			SetUseDirectDrawFlag( int Flag ) ;							// ＤｉｒｅｃｔＤｒａｗを使用するかどうかをセットする
extern	int			SetUseGDIFlag( int Flag ) ;									// ＧＤＩ描画を必要とするか、を変更する
extern	int			SetDDrawUseGuid( GUID *Guid ) ;								// ＤｉｒｅｃｔＤｒａｗが使用するＧＵＤＩを設定する
extern	int			SetDisplayRefreshRate( int RefreshRate ) ;					// フルスクリーン時の画面のリフレッシュレートを変更する(Windows2000以降のみ有効)
extern	int			SetMultiThreadFlag( int Flag ) ;							// DirectDraw や Direct3D の協調レベルをマルチスレッド対応にするかどうかをセットする
extern	int			SetUseDirectDrawDeviceIndex( int Index ) ;					// 使用する DirectDraw デバイスのインデックスを設定する
extern	int			SetUseTempFrontScreen( int Flag ) ;							// 一時退避用表画面を使用するかどうかを設定する( TRUE:使用する  FALSE:使用しない )

// ＤｉｒｅｃｔＤｒａｗ関係情報提供関数
extern	int						GetDrawScreenSize( int *XBuf, int *YBuf ) ;							// 描画サイズを取得する
extern	int						GetScreenBitDepth( void ) ;											// 使用色ビット数を返す
extern	int						GetBmpSurf3DRenderingValidState( int BmpIndex ) ;					// 指定のインデックスのビットマップデータが３Ｄデバイスによる描画が可能かどうかを取得する( TRUE:出来る  FALSE:出来ない )
extern	void*					GetDrawTargetSurface( void ) ;	/* 戻り値を IDirectDrawSurface7 * にキャストして下さい */		// 描画対象となっているサーフェスを取得
extern	void*					GetPrimarySurface( void ) ;		/* 戻り値を IDirectDrawSurface7 * にキャストして下さい */		// プライマリサーフェスを取得
extern	void*					GetBackSurface( void ) ;		/* 戻り値を IDirectDrawSurface7 * にキャストして下さい */		// バックサーフェスを取得
extern	void*					GetWorkSurface( void ) ;		/* 戻り値を IDirectDrawSurface7 * にキャストして下さい */		// 作業用サーフェスを取得
extern	int						GetDesktopDrawCmp( void ) ;											// デスクトップ画面への描画かの判定情報
extern	void*					GetUseDDrawObj( void ) ;		/* 戻り値を IDirectDraw7 * にキャストして下さい */			// 現在使用しているＤｉｒｅｃｔＤｒａｗオブジェクトのアドレスを取得する
extern  int						GetUseDirectDrawFlag( void ) ;										// ＤｉｒｅｃｔＤｒａｗを使用するかどうかを取得する
extern	int						GetColorBitDepth( void ) ;											// 画面の色ビット深度を得る
extern	int						GetChangeDisplayFlag( void ) ;										// 画面モードが変更されているかどうかのフラグを取得する
extern	COLORDATA*				GetDispColorData( void ) ;											// ディスプレーのカラーデータポインタを得る
extern	const D_DDPIXELFORMAT*	GetPixelFormat( void ) ;											// ピクセルフォーマットの取得
extern	int						GetScreenMemToSystemMemFlag( void ) ;								// 画面グラフィックデータがシステムメモリに存在するかフラグの取得
extern	const D_DDPIXELFORMAT*	GetOverlayPixelFormat( void ) ;										// 使用できるオーバーレイのピクセルフォーマットを得る
extern	D_DDCAPS				GetDirectDrawCaps( void ) ;											// DirectDraw の情報を得る
extern	int						GetVideoMemorySize( int *AllSize, int *FreeSize ) ;					// ビデオメモリの容量を得る
extern	int						GetUseGDIFlag( void ) ;												// ＧＤＩ描画を必要とするかどうかを取得する
extern	int						GetNotDraw3DFlag( void ) ;											// NotDraw3DFlag を取得する
extern	HDC						GetDrawScreenDC( void ) ;											// 描画先になっている画面のＤＣを取得する
extern	int						ReleaseDrawScreenDC( HDC Dc ) ;										// GetScreenDC で取得したＤＣを解放する
extern	GUID*					GetDirectDrawDeviceGUID( int Number ) ;								// 有効な DirectDraw デバイスの GUID を取得する
extern	int						GetDirectDrawDeviceDescription( int Number, char *StringBuffer ) ;	// 有効な DirectDraw デバイスの名前を得る
extern	int						GetDirectDrawDeviceNum( void ) ;									// 有効な DirectDraw デバイスの数を取得する
extern	int						GetUseMEMIMGFlag( void ) ;											// MEMIMG 構造体を使用するかどうかを取得する
extern	int						GetVSyncTime( void ) ;												// 垂直同期信号一回に付きかかる時間をミリ秒単位で取得する
extern	int						GetRefreshRate( void ) ;											// 現在の画面のリフレッシュレートを取得する
extern	int						GetDisplayModeNum( void ) ;											// 変更可能なディスプレイモードの数を取得する
extern	DISPLAYMODEDATA			GetDisplayMode( int ModeIndex ) ;									// 変更可能なディスプレイモードの情報を取得する( ModeIndex は 0 ～ GetDisplayModeNum の戻り値-1 )

// パレット操作関係関数
extern 	int			SetPalette( int PalIndex, int Red, int Green, int Blue ) ;							// パレットのセット
extern 	int			ReflectionPalette( void ) ;															// セットしたパレットをハードウエアに反映させる
extern 	int			GetPalette( int PalIndex, int *Red, int *Green, int *Blue ) ;						// パレットの取得
extern	int			SetBmpPal( const char *FileName ) ;													// ＢＭＰファイルのパレットを反映させる
extern	int			SetBmpPalPart( const char *FileName, int StartNum, int GetNum, int SetNum ) ;		// ＢＭＰファイルからのパレットの部分取得
extern	int			GetGraphPalette( int GrHandle, int ColorIndex, int *Red, int *Green, int *Blue ) ;	// メモリ上に読み込んだ画像のパレットを取得する(フルカラー画像の場合は無効)
extern  int			GetGraphOriginalPalette( int GrHandle, int ColorIndex, int *Red, int *Green, int *Blue ) ; // メモリ上に読み込んだ画像の SetGraphPalette で変更する前のパレットを取得する
extern	int			SetGraphPalette( int GrHandle, int ColorIndex, int Color ) ;							// メモリ上に読み込んだ画像のパレットを変更する(フルカラー画像の場合は無効)
extern	int			ResetGraphPalette( int GrHandle ) ;													// SetGraphPalette で変更したパレットを全て元に戻す

// 簡易グラフィック関係関数
extern	int			GetPixel( int x, int y ) ;																											// 指定座標の色を取得する
extern	int			Paint( int x, int y, int FillColor, int BoundaryColor = -1 ) ;																		// 指定点から境界色があるところまで塗りつぶす(境界色を -1 にすると指定点の色の領域を塗りつぶす)
extern	int			BltFastOrBitBlt( D_IDirectDrawSurface7 *Dest, D_IDirectDrawSurface7 *Src, int DestX, int DestY, RECT *SrcRect, int BltType = -1 ) ;	// 指定のサーフェスから指定のサーフェスへグラフィックデータを転送する

// ウエイト関係関数
extern	int			WaitVSync( int SyncNum ) ;																												// 垂直同期信号を待つ

// 画面操作関係関数
extern	int			ScreenFlip( void ) ;																													// 裏画面と表画面を交換する
extern 	int			ScreenCopy( RECT *CopyRect = NULL ) ;																									// 裏画面の内容を表画面に描画する
extern	int			GraphCopy( RECT *SrcRect, RECT *DestRect, int SrcHandle, int DestHandle = DX_SCREEN_BACK ) ;											// 画像の内容をコピーする
extern	int			SetGraphMode( int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int RefreshRate = 60 ) ;												// 画面モードのセット
extern	int			SetChangeScreenModeGraphicsSystemResetFlag( int Flag ) ;																				// 画面モード変更時( とウインドウモード変更時 )にグラフィックスシステムの設定やグラフィックハンドルをリセットするかどうかを設定する( TRUE:リセットする  FALSE:リセットしない )

// ＢＭＰ保存関数
// Jpeg_Quality         = 0:低画質～100:高画質
// Png_CompressionLevel = 0:無圧縮～  9:最高圧縮
extern	int			SaveDrawScreen( int x1, int y1, int x2, int y2, const char *FileName, int SaveType = DX_IMAGESAVETYPE_BMP , int Jpeg_Quality = 80 , int Jpeg_Sample2x1 = TRUE , int Png_CompressionLevel = -1 ) ;		// 現在描画対象になっている画面を保存する
extern	int			SaveDrawScreenToBMP( int x1, int y1, int x2, int y2, const char *FileName ) ;														// 現在描画対象になっている画面をＢＭＰ形式で保存する
extern	int			SaveDrawScreenToJPEG( int x1, int y1, int x2, int y2, const char *FileName, int Quality = 80 , int Sample2x1 = TRUE ) ;				// 現在描画対象になっている画面をＪＰＥＧ形式で保存する Quality = 画質、値が大きいほど低圧縮高画質,0～100 
extern	int			SaveDrawScreenToPNG( int x1, int y1, int x2, int y2, const char *FileName, int CompressionLevel = -1 ) ;							// 現在描画対象になっている画面をＰＮＧ形式で保存する CompressionLevel = 圧縮率、値が大きいほど高圧縮率高負荷、０は無圧縮,0～9









// Dx3D.cpp関数プロトタイプ宣言

// 設定関係
extern	D_IDirect3DDevice7* GetUseD3DDevObj( void ) ;																// 使用中のＤｉｒｅｃｔ３ＤＤｅｖｉｃｅオブジェクトを得る
extern	int			SetUseDivGraphFlag( int Flag ) ;																// 必要ならグラフィックの分割を行うか、フラグのセット
extern	int			SetUseMaxTextureSize( int Size ) ;																// 使用するテクスチャーの最大サイズをセット(0でデフォルト)
extern	int			SetUseVertexBufferFlag( int Flag ) ;															// 頂点バッファを使用するかどうかを設定する
//extern int		RenderVertexBuffer( void ) ;																	// 頂点バッファに溜まった頂点データを吐き出す
//extern VERTEX_2D*	GetVertexBuffer( int VertexNum = 6 , int PrimType = DX_PRIMTYPE_TRIANGLELIST ) ;				// 追加頂点バッファの頂点追加アドレスを得る( 戻り値はテクスチャなしの場合は VERTEX_NOTEX_2D となる )
//extern void		_DrawPreparation( int GrHandle = DX_NONE_GRAPH , int ParamFlag = 0 ) ;							// 描画準備を行う( ParamFlag は DRAWPREP_TRANS 等 )
//extern void		Add4VertexBuffer( void ) ;																		// GetVertexTo3DDevice によって４頂点追加されたことを前提に頂点追加処理を行う
//extern void		AddVertexBuffer( int VertexNum = 6 ) ;															// GetVertexTo3DDevice によって指定数頂点が追加されたことを前提に頂点追加処理を行う
//extern void		SetGraphTexture( int GrHandle ) ;																// Ｄｉｒｅｃｔ３Ｄデバイスにテクスチャをセットする
extern	int			SetUseOldDrawModiGraphCodeFlag( int Flag ) ;													// 以前の DrawModiGraph 関数のコードを使用するかどうかのフラグをセットする

// その他補助関数
extern D_DDPIXELFORMAT*	GetTexPixelFormat( int AlphaCh, int AlphaTest, int ColorBitDepth, int DrawValid = FALSE ) ;	// ピクセルフォーマットを得る
extern COLORDATA*		GetTexColorData( int AlphaCh, int AlphaTest, int ColorBitDepth, int DrawValid = FALSE ) ;	// カラーデータを得る
extern D_DDPIXELFORMAT*	GetTexPixelFormat( IMAGEFORMATDESC *Format ) ;												// フォーマットに基づいたカラーフォーマットを得る
extern COLORDATA*		GetTexColorData( IMAGEFORMATDESC *Format ) ;												// フォーマットに基づいたカラーデータを得る
extern D_DDPIXELFORMAT*	GetTexPixelFormat( int FormatIndex ) ;														// 指定のフォーマットインデックスのカラーフォーマットを得る
extern COLORDATA*		GetTexColorData( int FormatIndex ) ;														// 指定のフォーマットインデックスのカラーデータを得る
extern D_DDPIXELFORMAT*	GetZBufferPixelFormat( int BitDepth ) ;														// 指定のビット深度のＺバッファーのピクセルフォーマットを得る( 現在のところ 16 ビットのみ )








// DxGraphics関数プロトタイプ宣言

// グラフィック制御関係関数
extern	int			MakeGraph( int SizeX, int SizeY, int NotUse3DFlag = FALSE ) ;																								// 空のグラフィックを作成
extern	int			MakeScreen( int SizeX, int SizeY ) ;																														// 描画可能な画面を作成
extern	int			DeleteGraph( int GrHandle, int LogOutFlag = FALSE ) ;																										// 指定のグラフィックデータを削除する
extern	int			GetGraphNum( void ) ;																																		// 有効なグラフィックの数を取得する
extern	int			SetGraphLostFlag( int GrHandle, int *LostFlag ) ;																											// 解放時に立てるフラグのポインタをセットする
extern	int			InitGraph( int LogOutFlag = FALSE ) ;																														// 画像データの初期化

extern	int			BltBmpToGraph( COLORDATA *SrcColor, HBITMAP Bmp, HBITMAP AlphaMask, const char *GraphName, int CopyPointX, int CopyPointY, int GrHandle, int ReverseFlag ) ; // 画像データの転送
extern	int			BltBmpToDivGraph( COLORDATA *SrcColor, HBITMAP Bmp, HBITMAP AlphaMask, const char *GraphName, int AllNum, int XNum, int YNum, int Width, int Height, int *GrHandle, int ReverseFlag ) ;	// 分割画像へのＢＭＰの転送
extern	int			BltBmpOrGraphImageToGraph( COLORDATA *SrcColorData, HBITMAP Bmp, HBITMAP AlphaMask, const char *GraphName,
										int BmpFlag, BASEIMAGE *RgbImage, BASEIMAGE *AlphaImage,
										int CopyPointX, int CopyPointY, int GrHandle, int ReverseFlag ) ;																		// ＢＭＰ か GraphImage を画像に転送
extern	int			BltBmpOrGraphImageToGraph2( COLORDATA *SrcColorData, HBITMAP Bmp, HBITMAP AlphaMask, const char *GraphName,
										int BmpFlag, BASEIMAGE *RgbImage, BASEIMAGE *AlphaImage,
										RECT *SrcRect, int DestX, int DestY, int GrHandle, int ReverseFlag ) ;																	// ＢＭＰ か GraphImage を画像に転送
extern	int			BltBmpOrGraphImageToDivGraph( COLORDATA *SrcColor, HBITMAP Bmp, HBITMAP AlphaMask, const char *GraphName,
										int BmpFlag, BASEIMAGE *RgbImage, BASEIMAGE *AlphaImage,
										int AllNum, int XNum, int YNum, int Width, int Height, int *GrHandle, int ReverseFlag ) ;												// 分割画像への ＢＭＰ か GraphImage の転送

#ifndef DX_NON_MOVIE
extern	int			OpenMovieToOverlay( const char *FileName ) ;																												// オーバーレイサーフェスを使用したムービーファイルのオープン
extern	int			UpdateMovieToOverlay( int x, int y, int ExRate, int ShowFlag, int MovieHandle ) ;																			// オーバーレイサーフェスを使用したムービーの表示ステータスセット
extern	int			CloseMovieToOverlay( int MovieHandle ) ;																													// オーバーレイサーフェスを使用したムービーファイルを閉じる
#endif
extern	int			ReloadFileGraphAll( void ) ;																																// ファイルから読み込んだ画像情報を再度読み込む
extern	int			SetGraphTransColor( int GrHandle, int Red, int Green, int Blue ) ;																							// 画像の透過色を変更する(アルファチャンネル使用時は無効)

extern	int			RestoreGraph( int GrHandle ) ;																																// 画像データのリストア
extern	int			AllRestoreGraph( void ) ;																																	// すべての画像データのリストア


// グラフィック描画関係関数
extern	int			ClearDrawScreen( void ) ;																															// 画面の状態を初期化する
extern	int			ClsDrawScreen( void ) ;																																// 画面の状態を初期化する(ClearDrawScreenの旧名称)

extern	int			LoadGraphScreen( int x, int y, const char *GraphName, int TransFlag ) ;																				// ＢＭＰファイルを読みこんで画面に描画する
extern	int			DrawGraph( int x, int y, int GrHandle, int TransFlag ) ;																							// グラフィックの描画
extern	int			DrawGraphF( float xf, float yf, int GrHandle, int TransFlag ) ;																						// グラフィックの描画
extern	int			DrawExtendGraph( int x1, int y1, int x2, int y2, int GrHandle, int TransFlag ) ;																	// グラフィックの拡大縮小描画
extern	int			DrawExtendGraphF( float x1f, float y1f, float x2f, float y2, int GrHandle, int TransFlag ) ;														// グラフィックの拡大縮小描画
extern	int			DrawRotaGraph( int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE  ) ;									// グラフィックの回転描画
extern	int			DrawRotaGraphF( float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE  ) ;								// グラフィックの回転描画
extern	int			DrawRotaGraph2( int x, int y, int cx, int cy, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE  ) ;					// グラフィックの回転描画２
extern	int			DrawRotaGraph2F( float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE  ) ;		// グラフィックの回転描画２
extern	int			DrawModiGraph( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int TransFlag ) ;										// 画像の自由変形描画
extern	int			DrawModiGraphF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag ) ;						// 画像の自由変形描画( float 版 )
extern	int			DrawTurnGraph( int x, int y, int GrHandle, int TransFlag ) ;																						// 画像の左右反転描画
extern	int			DrawTurnGraphF( float xf, float yf, int GrHandle, int TransFlag ) ;																					// 画像の左右反転描画
extern	int			DrawChipMap( int Sx, int Sy, int XNum, int YNum, int *MapData, int ChipTypeNum, int MapDataPitch, int *GrHandle, int TransFlag ) ;					// チップグラフィックを使ったマップ描画
extern	int			DrawChipMap( int MapWidth, int MapHeight, int *MapData, int ChipTypeNum, int *ChipGrHandle, int TransFlag,
										int MapDrawPointX, int MapDrawPointY, int MapDrawWidth, int MapDrawHeight, int ScreenX, int ScreenY ) ;							// チップグラフィックを使ったマップ描画
extern	int			DrawTile( int x1, int y1, int x2, int y2, int Tx, int Ty,
								double ExtRate, double Angle, int GrHandle, int TransFlag ) ;																			// グラフィックを指定領域にタイル状に描画する
extern	int			DrawRectGraph( int DestX, int DestY, int SrcX, int SrcY, int Width, int Height, int GraphHandle, int TransFlag, int TurnFlag ) ;					// グラフィックの指定矩形部分のみを描画
extern	int			DrawRectExtendGraph( int DestX1, int DestY1, int DestX2, int DestY2, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int GraphHandle, int TransFlag ) ;	// グラフィックの指定矩形部分のみを拡大描画
extern	int			DrawRectRotaGraph( int X, int Y, int SrcX, int SrcY, int Width, int Height, double ExtRate, double Angle, int GraphHandle, int TransFlag, int TurnFlag ) ; 
extern	int			DrawBlendGraph( int x, int y, int GrHandle, int TransFlag, int BlendGraph, int BorderParam, int BorderRange ) ;										// ブレンド画像と合成して画像を描画する
extern	int			DrawBlendGraphPos( int x, int y, int GrHandle, int TransFlag, int bx, int by, int BlendGraph, int BorderParam, int BorderRange ) ;					// ブレンド画像と合成して画像を描画する( ブレンド画像の起点座標を指定する版 )
extern	int			DrawCircleGauge( int CenterX, int CenterY, double Percent, int GrHandle ) ;																			// 円グラフ的な描画を行う
extern	int			DrawGraphToZBuffer( int X, int Y, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;															// Ｚバッファに対して画像の描画を行う
extern	int			DrawTurnGraphToZBuffer( int x, int y, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;														// Ｚバッファに対して画像の左右反転描画
extern	int			DrawExtendGraphToZBuffer( int x1, int y1, int x2, int y2, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;									// Ｚバッファに対して画像の拡大縮小描画
extern	int			DrawRotaGraphToZBuffer( int x, int y, double ExRate, double Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */ , int TurnFlag = FALSE  ) ;	// Ｚバッファに対して画像の回転描画
extern	int			DrawModiGraphToZBuffer( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;	// Ｚバッファに対して画像の自由変形描画
extern	int			DrawBoxToZBuffer( int x1, int y1, int x2, int y2, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;											// Ｚバッファに対して矩形の描画を行う
extern	int			DrawCircleToZBuffer( int x, int y, int r, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;													// Ｚバッファに対して円の描画を行う
extern	int			DrawPolygonBase( VERTEX *Vertex, int VertexNum, int PrimitiveType, int GrHandle, int TransFlag, int UVScaling = FALSE ) ;							// ２Ｄポリゴンを描画する
extern	int			DrawPolygon( VERTEX *Vertex, int PolygonNum, int GrHandle, int TransFlag, int UVScaling = FALSE ) ;													// ２Ｄポリゴンを描画する
extern	int			DrawPrimitive2D( VERTEX2D *Vertex, int VertexNum, int PrimitiveType, int GrHandle, int TransFlag ) ;												// ２Ｄプリミティブを描画する
extern	int			DrawPrimitiveIndexed2D( VERTEX2D *Vertex, int VertexNum, unsigned short *Indices, int IndexNum, int PrimitiveType, int GrHandle, int TransFlag ) ;	// ２Ｄプリミティブを描画する(インデックス)
extern	int			DrawPrimitive3D( VERTEX3D *Vertex, int VertexNum, int PrimitiveType, int GrHandle, int TransFlag ) ;												// ３Ｄプリミティブを描画する
extern	int			DrawPrimitiveIndexed3D( VERTEX3D *Vertex, int VertexNum, unsigned short *Indices, int IndexNum, int PrimitiveType, int GrHandle, int TransFlag ) ;	// ３Ｄプリミティブを描画する(インデックス)
extern	int			DrawPolygon3D( VERTEX3D *Vertex, int PolygonNum, int GrHandle, int TransFlag ) ;																	// ３Ｄポリゴンを描画する
extern	int			DrawPolygonIndexed3D( VERTEX3D *Vertex, int VertexNum, unsigned short *Indices, int PolygonNum, int GrHandle, int TransFlag ) ;						// ３Ｄポリゴンを描画する(インデックス)
extern	int			DrawPolygon3DBase( VERTEX_3D *Vertex, int VertexNum, int PrimitiveType, int GrHandle, int TransFlag ) ;												// ３Ｄポリゴンを描画する( 旧バージョン用 )
extern	int			DrawPolygon3D( VERTEX_3D *Vertex, int PolygonNum, int GrHandle, int TransFlag ) ;																	// ３Ｄポリゴンを描画する( 旧バージョン用 )
extern	int			DrawGraph3D( float x, float y, float z, int GrHandle, int TransFlag ) ;																				// グラフィックの３Ｄ描画
extern	int			DrawExtendGraph3D( float x, float y, float z, double ExRateX, double ExRateY, int GrHandle, int TransFlag ) ;										// グラフィックの拡大３Ｄ描画
extern	int			DrawRotaGraph3D( float x, float y, float z, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE ) ;						// グラフィックの回転３Ｄ描画
extern	int			DrawRota2Graph3D( float x, float y, float z, float cx, float cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE ) ;	// グラフィックの回転３Ｄ描画(回転中心指定型)
extern	int			DrawModiBillboard3D( VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag ) ;	// グラフィックの自由変形３Ｄ描画
extern	int			DrawBillboard3D( VECTOR Pos, float cx, float cy, float Size, float Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE ) ;						// ３Ｄ空間上にグラフィックを描画
extern	int			FillGraph( int GrHandle, int Red, int Green, int Blue, int Alpha = 255 ) ;																			// グラフィックを特定の色で塗りつぶす
extern	int			DrawLine( int x1, int y1, int x2, int y2, int Color, int Thickness = 1 ) ;																			// 線を描画
extern	int			DrawBox( int x1, int y1, int x2, int y2, int Color, int FillFlag ) ;																				// 四角形の描画
extern	int			DrawFillBox( int x1, int y1, int x2, int y2, int Color ) ;																							// 中身のある四角を描画
extern	int			DrawLineBox( int x1, int y1, int x2, int y2, int Color ) ;																							// 四角形の描画 
extern	int			DrawCircle( int x, int y, int r, int Color, int FillFlag = TRUE ) ;																					// 円を描く
extern	int			DrawOval( int x, int y, int rx, int ry, int Color, int FillFlag ) ;																					// 楕円を描く
extern	int			DrawTriangle( int x1, int y1, int x2, int y2, int x3, int y3, int Color, int FillFlag ) ;															// 三角形の描画
extern	int			DrawQuadrangle( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Color, int FillFlag ) ;											// 四角形の描画
extern 	int			DrawPixel( int x, int y, int Color ) ;																												// 点を描画する
extern 	int			DrawPixelSet( POINTDATA *PointData, int Num ) ;																										// 点の集合を描く
extern	int			DrawLineSet( LINEDATA *LineData, int Num ) ;																										// 線の集合を描く
extern	int			DrawLine3D( VECTOR Pos1, VECTOR Pos2, int Color ) ;																									// ３Ｄの線分を描画する
extern	int			DrawTriangle3D( VECTOR Pos1, VECTOR Pos2, VECTOR Pos3, int Color, int FillFlag ) ;																	// ３Ｄの三角形を描画する
extern	int			DrawCube3D( VECTOR Pos1, VECTOR Pos2, int DifColor, int SpcColor, int FillFlag ) ;																	// ３Ｄの立方体を描画する
extern	int			DrawSphere3D( VECTOR CenterPos, float r, int DivNum, int DifColor, int SpcColor, int FillFlag ) ;													// ３Ｄの球体を描画する
extern	int			DrawCapsule3D( VECTOR Pos1, VECTOR Pos2, float r, int DivNum, int DifColor, int SpcColor, int FillFlag ) ;											// ３Ｄのカプセルを描画する
extern	int			DrawCone3D( VECTOR TopPos, VECTOR BottomPos, float r, int DivNum, int DifColor, int SpcColor, int FillFlag ) ;										// ３Ｄの円錐を描画する
extern	int			DrawString( int x, int y, const char *String, int Color, int EdgeColor = 0 ) ;																		// 文字列の描画
extern	int			DrawVString( int x, int y, const char *String, int Color, int EdgeColor = 0 ) ;																		// 文字列の描画
extern	int			DrawStringToHandle( int x, int y, const char *String, int Color, int FontHandle, int EdgeColor = 0 , int VerticalFlag = FALSE ) ;					// 文字列を描画する
extern	int			DrawVStringToHandle( int x, int y, const char *String, int Color, int FontHandle, int EdgeColor = 0 ) ;												// 文字列を描画する
extern	int			DrawFormatString( int x, int y, int Color, const char *FormatString, ... ) ;																		// 書式指定文字列を描画する
extern	int			DrawFormatVString( int x, int y, int Color, const char *FormatString, ... ) ;																		// 書式指定文字列を描画する
extern	int			DrawFormatStringToHandle( int x, int y, int Color, int FontHandle, const char *FormatString, ... ) ;												// 書式指定文字列を描画する
extern	int			DrawFormatVStringToHandle( int x, int y, int Color, int FontHandle, const char *FormatString, ... ) ;												// 書式指定文字列を描画する
extern	int			DrawExtendString( int x, int y, double ExRateX, double ExRateY, const char *String, int Color, int EdgeColor = 0 ) ;													// 文字列の拡大描画
extern	int			DrawExtendVString( int x, int y, double ExRateX, double ExRateY, const char *String, int Color, int EdgeColor = 0 ) ;													// 文字列の拡大描画
extern	int			DrawExtendStringToHandle( int x, int y, double ExRateX, double ExRateY, const char *String, int Color, int FontHandle, int EdgeColor = 0 , int VerticalFlag = FALSE ) ;	// 文字列を拡大描画する
extern	int			DrawExtendVStringToHandle( int x, int y, double ExRateX, double ExRateY, const char *String, int Color, int FontHandle, int EdgeColor = 0 ) ;							// 文字列を拡大描画する
extern	int			DrawExtendFormatString( int x, int y, double ExRateX, double ExRateY, int Color, const char *FormatString, ... ) ;														// 書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVString( int x, int y, double ExRateX, double ExRateY, int Color, const char *FormatString, ... ) ;														// 書式指定文字列を拡大描画する
extern	int			DrawExtendFormatStringToHandle( int x, int y, double ExRateX, double ExRateY, int Color, int FontHandle, const char *FormatString, ... ) ;								// 書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVStringToHandle( int x, int y, double ExRateX, double ExRateY, int Color, int FontHandle, const char *FormatString, ... ) ;								// 書式指定文字列を拡大描画する

extern	int			DrawNumberToI( int x, int y, int Num, int RisesNum, int Color, int EdgeColor = 0 ) ;																// 整数型の数値を描画する
extern	int			DrawNumberToF( int x, int y, double Num, int Length, int Color, int EdgeColor = 0 ) ;																// 浮動小数点型の数値を描画する
extern	int			DrawNumberPlusToI( int x, int y, const char *NoteString, int Num, int RisesNum, int Color, int EdgeColor = 0 ) ;									// 整数型の数値とその説明の文字列を一度に描画する
extern 	int			DrawNumberPlusToF( int x, int y, const char *NoteString, double Num, int Length, int Color, int EdgeColor = 0 ) ;									// 浮動小数点型の数値とその説明の文字列を一度に描画する

extern	int			DrawNumberToIToHandle( int x, int y, int Num, int RisesNum, int Color, int FontHandle, int EdgeColor = 0 ) ;										// 整数型の数値を描画する
extern	int			DrawNumberToFToHandle( int x, int y, double Num, int Length, int Color, int FontHandle, int EdgeColor = 0 ) ;										// 浮動小数点型の数値を描画する
extern	int			DrawNumberPlusToIToHandle( int x, int y, const char *NoteString, int Num, int RisesNum, int Color, int FontHandle, int EdgeColor = 0 ) ;			// 整数型の数値とその説明の文字列を一度に描画する
extern	int			DrawNumberPlusToFToHandle( int x, int y, const char *NoteString, double Num, int Length, int Color, int FontHandle, int EdgeColor = 0 ) ;			// 浮動小数点型の数値とその説明の文字列を一度に描画する

#ifndef DX_NON_DIRECTX9

// シェーダー関係関数
extern	int			GetValidShaderVersion( void ) ;										// 使用できるシェーダーのバージョンを取得する( 0=使えない  200=シェーダーモデル２．０が使用可能  300=シェーダーモデル３．０が使用可能 )

extern	int			LoadVertexShader( const char *FileName ) ;							// 頂点シェーダーバイナリをファイルから読み込み頂点シェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
extern	int			LoadVertexShaderFromMem( void *ImageAddress, int ImageSize ) ;		// メモリ空間上に存在する頂点シェーダーバイナリから頂点シェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
extern	int			LoadPixelShader( const char *FileName ) ;							// ピクセルシェーダーバイナリをファイルから読み込みピクセルシェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
extern	int			LoadPixelShaderFromMem( void *ImageAddress, int ImageSize ) ;		// メモリ空間上に存在するピクセルシェーダーバイナリからピクセルシェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
extern	int			DeleteShader( int ShaderHandle ) ;									// シェーダーハンドルの削除
extern	int			InitShader( void ) ;												// シェーダーハンドルを全て削除する

extern	int			GetConstIndexToShader( const char *ConstantName, int ShaderHandle ) ;		// 指定の名前を持つ定数の番号を取得する
extern	int			SetVSConstF( int ConstantIndex, FLOAT4 Param ) ;							// 頂点シェーダーの float 型定数を設定する
extern	int			SetVSConstFMtx( int ConstantIndex, MATRIX Param ) ;							// 頂点シェーダーの float 型定数に行列を設定する
extern	int			SetVSConstI( int ConstantIndex, INT4 Param ) ;								// 頂点シェーダーの int 型定数を設定する
extern	int			SetVSConstB( int ConstantIndex, BOOL Param ) ;								// 頂点シェーダーの BOOL 型定数を設定する
extern	int			SetVSConstFArray( int ConstantIndex, FLOAT4 *ParamArray, int ParamNum ) ;	// 頂点シェーダーの float 型定数を設定する( 配列を使って連番インデックスに一度に設定 )
extern	int			SetVSConstFMtxArray( int ConstantIndex, MATRIX *ParamArray, int ParamNum ) ; // 頂点シェーダーの float 型定数に行列を設定する( 配列を使って連番インデックスに一度に設定 )
extern	int			SetVSConstIArray( int ConstantIndex, INT4 *ParamArray, int ParamNum ) ;		// 頂点シェーダーの int 型定数を設定する( 配列を使って連番インデックスに一度に設定 )
extern	int			SetVSConstBArray( int ConstantIndex, BOOL *ParamArray, int ParamNum ) ;		// 頂点シェーダーの BOOL 型定数を設定する( 配列を使って連番インデックスに一度に設定 )

extern	int			SetPSConstF( int ConstantIndex, FLOAT4 Param ) ;							// ピクセルシェーダーの float 型定数を設定する
extern	int			SetPSConstFMtx( int ConstantIndex, MATRIX Param ) ;							// ピクセルシェーダーの float 型定数に行列を設定する
extern	int			SetPSConstI( int ConstantIndex, INT4 Param ) ;								// ピクセルシェーダーの int 型定数を設定する
extern	int			SetPSConstB( int ConstantIndex, BOOL Param ) ;								// ピクセルシェーダーの BOOL 型定数を設定する
extern	int			SetPSConstFArray( int ConstantIndex, FLOAT4 *ParamArray, int ParamNum ) ;	// ピクセルシェーダーの float 型定数を設定する( 配列を使って連番インデックスに一度に設定 )
extern	int			SetPSConstFMtxArray( int ConstantIndex, MATRIX *ParamArray, int ParamNum ) ; // ピクセルシェーダーの float 型定数に行列を設定する( 配列を使って連番インデックスに一度に設定 )
extern	int			SetPSConstIArray( int ConstantIndex, INT4 *ParamArray, int ParamNum ) ;		// ピクセルシェーダーの int 型定数を設定する( 配列を使って連番インデックスに一度に設定 )
extern	int			SetPSConstBArray( int ConstantIndex, BOOL *ParamArray, int ParamNum ) ;		// ピクセルシェーダーの BOOL 型定数を設定する( 配列を使って連番インデックスに一度に設定 )

//extern int		SetRenderTargetToShader( int TargetIndex, int DrawScreen ) ;		// シェーダー描画での描画先を設定する
extern	int			SetUseTextureToShader( int StageIndex, int GraphHandle ) ;			// シェーダー描画で使用するグラフィックを設定する
extern	int			SetUseVertexShader( int ShaderHandle ) ;							// シェーダー描画に使用する頂点シェーダーを設定する( -1を渡すと解除 )
extern	int			SetUsePixelShader( int ShaderHandle ) ;								// シェーダー描画に使用するピクセルシェーダーを設定する( -1を渡すと解除 )

extern	int			DrawPrimitive2DToShader( VERTEX2DSHADER *Vertex, int VertexNum, int PrimitiveType ) ;												// シェーダーを使って２Ｄプリミティブを描画する
extern	int			DrawPrimitive3DToShader( VERTEX3DSHADER *Vertex, int VertexNum, int PrimitiveType ) ;												// シェーダーを使って３Ｄプリミティブを描画する
extern	int			DrawPrimitiveIndexed2DToShader( VERTEX2DSHADER *Vertex, int VertexNum, unsigned short *Indices, int IndexNum, int PrimitiveType ) ;	// シェーダーを使って２Ｄプリミティブを描画する(インデックス)
extern	int			DrawPrimitiveIndexed3DToShader( VERTEX3DSHADER *Vertex, int VertexNum, unsigned short *Indices, int IndexNum, int PrimitiveType ) ;	// シェーダーを使って３Ｄプリミティブを描画する(インデックス)

#endif // DX_NON_DIRECTX9

// ３Ｄ描画関係関数
//extern void			SetColorVertexState( LPVERTEXDATA Vertex, float x, float y, float z, float rhw, int red, int green, int blue, int alpha, float tu, float tv ) ;	// ３Ｄ頂点データをセットする  

// 描画設定関係関数
extern	int			SetDrawMode( int DrawMode ) ;										// 描画モードをセットする
extern	int			SetMaxAnisotropy( int MaxAnisotropy ) ;								// 最大異方性の値をセットする
extern	int			SetDrawBlendMode( int BlendMode, int BlendParam ) ;					// 描画ブレンドモードをセットする
extern	int			SetBlendGraph( int BlendGraph, int BorderParam, int BorderRange ) ;	// 描画処理時に描画する画像とブレンドするαチャンネル付き画像をセットする( BlendGraph を -1 でブレンド機能を無効 )
extern	int			SetBlendGraphPosition( int x, int y ) ;								// ブレンド画像の起点座標をセットする
extern	int			SetDrawBright( int RedBright, int GreenBright, int BlueBright ) ;	// 描画輝度をセット
extern	int			SetDrawScreen( int DrawScreen ) ;									// 描画先画面のセット
extern	int			SetDrawArea( int x1, int y1, int x2, int y2 ) ;						// 描画可能領域のセット
extern	int			SetUse3DFlag( int Flag ) ;											// ３Ｄ機能を使うか、のフラグをセット
extern	int			SetUseHardwareVertexProcessing( int Flag ) ;						// ハードウエアの頂点演算処理機能を使用するかどうかを設定する
extern	int			SetRestoreShredPoint( void (* ShredPoint )( void ) ) ;				// SetRestoreGraphCallback の旧名
extern	int			SetRestoreGraphCallback( void (* Callback )( void ) ) ;				// グラフィックハンドル復元関数の登録
extern	int			RunRestoreShred( void ) ;											// グラフィック復元関数の実行
extern	int			SetTransformToWorld( MATRIX *Matrix ) ;								// ワールド変換用行列をセットする
extern	int			SetTransformToView( MATRIX *Matrix ) ;								// ビュー変換用行列をセットする
extern	int			SetTransformToProjection( MATRIX *Matrix ) ;						// 投影変換用行列をセットする
extern	int			SetTransformToViewport( MATRIX *Matrix ) ;							// ビューポート行列をセットする
extern	int			SetUseCullingFlag( int Flag ) ;										// SetUseBackCulling の旧名称
extern	int			SetUseBackCulling( int Flag ) ;										// ポリゴンカリングの有効、無効をセットする
extern	int			SetTextureAddressMode( int Mode /* DX_TEXADDRESS_WRAP 等 */ , int Stage = -1 ) ;		// テクスチャアドレスモードを設定する
extern	int			SetTextureAddressModeUV( int ModeU, int ModeV, int Stage = -1 ) ;	// テクスチャアドレスモードを設定する
extern	int			SetTextureAddressTransform( float TransU, float TransV, float ScaleU, float ScaleV, float RotCenterU, float RotCenterV, float Rotate ) ;	// テクスチャ座標変換パラメータをセットする
extern	int			SetTextureAddressTransformMatrix( MATRIX Matrix ) ;					// テクスチャ座標変換行列をセットする
extern	int			ResetTextureAddressTransform( void ) ;								// テクスチャ座標変換パラメータをリセットする
extern	int			SetFogEnable( int Flag ) ;											// フォグを有効にするかどうかを設定する( TRUE:有効  FALSE:無効 )
extern	int			SetFogMode( int Mode /* DX_FOGMODE_NONE 等 */ ) ;					// フォグモードを設定する
extern	int			SetFogColor( int r, int g, int b ) ;								// フォグカラーを変更する
extern	int			SetFogStartEnd( float start, float end ) ;							// フォグが始まる距離と終了する距離を設定する( 0.0f ～ 1.0f )
extern	int			SetFogDensity( float density ) ;									// フォグの密度を設定する( 0.0f ～ 1.0f )
extern	int			SetUseSystemMemGraphCreateFlag( int Flag ) ;						// システムメモリ上にグラフィックを作成するかどうかのフラグをセットする( TRUE:システムメモリ上に作成  FALSE:ＶＲＡＭ上に作成 )
extern	int			SetUseVramFlag( int Flag ) ;										// ＶＲＡＭを使用するかのフラグをセットする
extern	int			RestoreGraphSystem( void ) ;										// ＤＸライブラリのグラフィック関連の復帰処理を行う
extern	int			SetAeroDisableFlag( int Flag ) ;									// Vista以降の Windows Aero を無効にするかどうかをセットする、TRUE:無効にする  FALSE:有効にする( DxLib_Init の前に呼ぶ必要があります )

// グラフィック情報取得関係関数

//extern	int			GetGraphHandle( IMAGEDATA *GraphData ) ;													// 自分のグラフィックインデックス番号を取得する
//extern	IMAGEDATA 	*GetGraphData( int GrHandle ) ;																// グラフィックのデータをインデックス値から取り出す

extern	int			GetDrawScreenGraph( int x1, int y1, int x2, int y2, int GrHandle, int UseClientFlag = TRUE ) ;	// アクティブになっている画面から指定領域のグラフィックを取得する
extern	DWORD*		GetFullColorImage( int GrHandle ) ;																// 指定の画像のＡＲＧＢ８のフルカラーイメージを取得する

extern	int			GraphLock( int GrHandle, int *PitchBuf, void **DataPointBuf, COLORDATA **ColorDataPP = NULL ) ;	// グラフィックメモリ領域のロック
extern	int			GraphUnLock( int GrHandle ) ;																	// グラフィックメモリ領域のロック解除

extern	int			SetUseGraphZBuffer( int GrHandle, int UseFlag ) ;												// グラフィックにＺバッファを使用するかどうかを設定する

extern	int			GetGraphSize( int GrHandle, int *SizeXBuf, int *SizeYBuf ) ;									// グラフィックのサイズを得る
extern	int			GetScreenState( int *SizeX, int *SizeY, int *ColorBitDepth ) ;									// 現在の画面の大きさとカラービット数を得る 
extern	int			GetUse3DFlag( void ) ;																			// 描画に３Ｄ機能を使うかフラグを取得
extern	int			GetValidRestoreShredPoint( void ) ;																// グラフィック復元関数の有無を取得																
extern	char*		GetResourceIDString( int ResourceID ) ;															// リソースＩＤからリソースＩＤ文字列を得る 
extern	int			GetTransformToViewMatrix( MATRIX *MatBuf ) ;													// ビュー行列を取得する
extern	int			GetTransformToWorldMatrix( MATRIX *MatBuf ) ;													// ワールド行列を取得する
extern	int			GetTransformToProjectionMatrix( MATRIX *MatBuf ) ;												// 射影行列を取得する
extern	int			GetTransformPosition( VECTOR *LocalPos, float *x, float *y ) ;									// ローカル座標から画面座標を取得する
extern	VECTOR		ConvWorldPosToScreenPos( VECTOR WorldPos ) ;													// ワールド座標をスクリーン座標に変換する
extern	VECTOR		ConvScreenPosToWorldPos( VECTOR ScreenPos ) ;													// スクリーン座標をワールド座標に変換する
extern	int			GetUseVramFlag( void ) ;																		// ２Ｄグラフィックサーフェス作成時にシステムメモリーを使用するかのフラグ取得

// 補助関係
extern	int			GetCreateGraphColorData( COLORDATA *ColorData, IMAGEFORMATDESC *Format ) ;									// これから新たにグラフィックを作成する場合に使用するカラー情報を取得する

// 画像作成用関係
//extern int		SetGraphName( int Handle, const char *GraphName ) ;																																																// 特定のファイルから画像を読み込んだ場合のファイルパスをセットする
extern	int			CreateDXGraph( BASEIMAGE *RgbImage, BASEIMAGE *AlphaImage, int TextureFlag ) ;																																									// GraphImage データからサイズを割り出し、それに合ったグラフィックハンドルを作成する
//extern int		CreateDXDivGraph( BASEIMAGE *RgbImage, BASEIMAGE *AlphaImage, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag ) ;																							// GraphImage データに合ったサイズの分割グラフィックハンドルを作成する
extern	int			DerivationGraph( int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle ) ;																																								// 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックハンドルを作成する

#ifndef DX_NON_MOVIE
// ムービーグラフィック関係関数
extern	int			PlayMovie( const char *FileName, int ExRate, int PlayType ) ;								// 動画ファイルの再生
extern	int			OpenMovieToGraph( const char *FileName, int FullColor = TRUE ) ;							// ムービーを開く
extern 	int			PlayMovieToGraph( int GraphHandle, int PlayType = DX_PLAYTYPE_BACK , int SysPlay = 0 ) ;	// ムービーグラフィックに含まれるムービーの再生を開始する
extern 	int			PauseMovieToGraph( int GraphHandle, int SysPause = 0 ) ;									// ムービーグラフィックに含まれるムービーの再生をストップする
extern	int			AddMovieFrameToGraph( int GraphHandle, unsigned int FrameNum ) ;							// ムービーのフレームを進める、戻すことは出来ない( ムービーが停止状態で、且つ Ogg Theora のみ有効 )
extern	int			SeekMovieToGraph( int GraphHandle, int Time ) ;												// ムービーの再生位置を設定する(ミリ秒単位)
extern 	int			GetMovieStateToGraph( int GraphHandle ) ;													// ムービーの再生状態を得る
extern	int			SetMovieVolumeToGraph( int Volume, int GraphHandle ) ;										// ムービーのボリュームをセットする(0～10000)
extern	BASEIMAGE*	GetMovieBaseImageToGraph( int GraphHandle ) ;												// ムービーの基本イメージデータを取得する
extern	int			GetMovieTotalFrameToGraph( int GraphHandle ) ;												// ムービーの総フレーム数を得る( Ogg Theora でのみ有効 )
extern	int			TellMovieToGraph( int GraphHandle ) ;														// ムービーの再生位置を取得する(ミリ秒単位)
extern	int			TellMovieToGraphToFrame( int GraphHandle ) ;												// ムービーの再生位置を取得する(フレーム単位)
extern	int			SeekMovieToGraphToFrame( int GraphHandle, int Frame ) ;										// ムービーの再生位置を設定する(フレーム単位)
extern	LONGLONG	GetOneFrameTimeMovieToGraph( int GraphHandle ) ;											// ムービーの１フレームあたりの時間を得る
extern	int			GetLastUpdateTimeMovieToGraph( int GraphHandle ) ;											// ムービーのイメージを最後に更新した時間を得る(ミリ秒単位)

#endif








// DxGraphicsBase.cpp 関数プロトタイプ宣言

// グラフィックハンドル作成系
extern	int			LoadBmpToGraph( const char *GraphName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL ) ;										// 画像を読みこむ 
extern	int			LoadGraph( const char *FileName, int NotUse3DFlag = FALSE ) ;																								// 画像ファイルのメモリへの読みこみ
extern	int			LoadReverseGraph( const char *FileName, int NotUse3DFlag = FALSE ) ;																						// 画像ファイルのメモリへの反転読み込み
extern	int			LoadDivGraph( const char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf, int NotUse3DFlag = FALSE ) ;						// 画像の分割読みこみ
extern	int			LoadDivBmpToGraph( const char *FileName, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag, int ReverseFlag ) ;			// 画像ファイルの分割読みこみ
extern	int			LoadReverseDivGraph( const char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf, int NotUse3DFlag = FALSE ) ;				// 画像の反転分割読みこみ
extern	int			LoadBlendGraph( const char *FileName ) ;																													// 画像ファイルからブレンド用画像を読み込む
extern	int			LoadGraphToResource( int ResourceID ) ;																														// リソースから画像データを読み込む
extern	int			LoadDivGraphToResource( int ResourceID, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf ) ;											// リソースから画像データを分割読み込みする
extern	int			LoadGraphToResource( const char *ResourceName, const char *ResourceType ) ;																					// リソースから画像データを読み込む
extern	int			LoadDivGraphToResource( const char *ResourceName, const char *ResourceType, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf ) ;		// リソースから画像データを分割読み込みする

extern	int			CreateGraphFromMem( void *MemImage, int MemImageSize, void *AlphaImage = NULL , int AlphaImageSize = 0 , int TextureFlag = TRUE , int ReverseFlag = FALSE ) ;																						// メモリ上のグラフィックイメージからグラフィックハンドルを作成する
extern	int			ReCreateGraphFromMem( void *MemImage, int MemImageSize, int GrHandle, void *AlphaImage = NULL , int AlphaImageSize = 0 , int TextureFlag = TRUE , int ReverseFlag = FALSE ) ;																		// メモリ上のグラフィックイメージから既存のグラフィックハンドルにデータを転送する
extern	int			CreateDivGraphFromMem( void *MemImage, int MemImageSize, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag = TRUE , int ReverseFlag = FALSE , void *AlphaImage = NULL , int AlphaImageSize = 0 ) ;				// メモリ上のグラフィックイメージから分割グラフィックハンドルを作成する
extern	int			ReCreateDivGraphFromMem( void *MemImage, int MemImageSize, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag = TRUE , int ReverseFlag = FALSE , void *AlphaImage = NULL , int AlphaImageSize = 0 ) ;			// メモリ上のグラフィックイメージから既存の分割グラフィックハンドルにデータを転送する
extern	int			CreateGraphFromBmp( BITMAPINFO *BmpInfo, void *GraphData, BITMAPINFO *AlphaInfo = NULL , void *AlphaData = NULL , int TextureFlag = TRUE , int ReverseFlag = FALSE ) ;																				// ビットマップデータからグラフィックハンドルを作成する
extern	int			ReCreateGraphFromBmp( BITMAPINFO *BmpInfo, void *GraphData, int GrHandle, BITMAPINFO *AlphaInfo = NULL , void *AlphaData = NULL , int TextureFlag = TRUE , int ReverseFlag = FALSE ) ;																// ビットマップデータから既存のグラフィックハンドルにデータを転送する
extern	int			CreateDivGraphFromBmp( BITMAPINFO *BmpInfo, void *GraphData, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag = TRUE , int ReverseFlag = FALSE , BITMAPINFO *AlphaInfo = NULL , void *AlphaData = NULL ) ;		// ビットマップデータから分割グラフィックハンドルを作成する
extern	int			ReCreateDivGraphFromBmp( BITMAPINFO *BmpInfo, void *GraphData, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag = TRUE , int ReverseFlag = FALSE , BITMAPINFO *AlphaInfo = NULL , void *AlphaData = NULL ) ;	// ビットマップデータから既存の分割グラフィックハンドルにデータを転送する
extern	int			CreateGraphFromGraphImage( BASEIMAGE *Image, int TextureFlag = TRUE , int ReverseFlag = FALSE , int DataReverseFlag = TRUE ) ;																														// GraphImageデータからグラフィックハンドルを作成する
extern	int			CreateGraphFromGraphImage( BASEIMAGE *Image, BASEIMAGE *AlphaImage, int TextureFlag = TRUE , int ReverseFlag = FALSE , int DataReverseFlag = TRUE ) ;																								// GraphImageデータからグラフィックハンドルを作成する
extern	int			ReCreateGraphFromGraphImage( BASEIMAGE *Image, int GrHandle, int TextureFlag = TRUE , int ReverseFlag = FALSE , int DataReverseFlag = TRUE ) ;																										// GraphImageデータから既存のグラフィックハンドルにデータを転送する
extern	int			ReCreateGraphFromGraphImage( BASEIMAGE *Image, BASEIMAGE *AlphaImage, int GrHandle, int TextureFlag = TRUE , int ReverseFlag = FALSE , int DataReverseFlag = TRUE ) ;																				// GraphImageデータから既存のグラフィックハンドルにデータを転送する
extern	int			CreateDivGraphFromGraphImage( BASEIMAGE *Image, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag = TRUE , int ReverseFlag = FALSE , int DataReverseFlag = TRUE ) ;												// GraphImageデータから分割グラフィックハンドルを作成する
extern	int			CreateDivGraphFromGraphImage( BASEIMAGE *Image, BASEIMAGE *AlphaImage, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag = TRUE , int ReverseFlag = FALSE , int DataReverseFlag = TRUE ) ;						// GraphImageデータから分割グラフィックハンドルを作成する
extern	int			ReCreateDivGraphFromGraphImage( BASEIMAGE *Image, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag = TRUE , int ReverseFlag = FALSE , int DataReverseFlag = TRUE ) ;											// GraphImageデータから既存の分割グラフィックハンドルにデータを転送する
extern	int			ReCreateDivGraphFromGraphImage( BASEIMAGE *Image, BASEIMAGE *AlphaImage, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag = TRUE , int ReverseFlag = FALSE , int DataReverseFlag = TRUE ) ;					// GraphImageデータから既存の分割グラフィックハンドルにデータを転送する
extern	int			CreateGraph( int Width, int Height, int Pitch, void *GraphData, void *AlphaData = NULL , int GrHandle = -1 ) ;																																		// メモリ上のグラフィックデータからグラフィックハンドルを作成する
extern	int			CreateDivGraph( int Width, int Height, int Pitch, void *GraphData, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, void *AlphaData = NULL ) ;																					// メモリ上のグラフィックデータから分割グラフィックハンドルを作成する
extern	int			ReCreateGraph( int Width, int Height, int Pitch, void *GraphData, int GrHandle, void *AlphaData = NULL ) ;																																			// メモリ上のグラフィックデータからグラフィックハンドルを再作成する
extern	int			CreateGraphFromSoftImage( int SIHandle ) ;																													// ソフトウエアで扱うイメージからグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
extern	int			CreateGraphFromRectSoftImage( int SIHandle, int x, int y, int SizeX, int SizeY ) ;																			// ソフトウエアで扱うイメージの指定の領域を使ってグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
extern	int			ReCreateGraphFromSoftImage( int SIHandle, int GrHandle ) ;																									// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
extern	int			ReCreateGraphFromRectSoftImage( int SIHandle, int x, int y, int SizeX, int SizeY, int GrHandle ) ;															// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
extern	int			CreateDivGraphFromSoftImage( int SIHandle, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf ) ;											// ソフトウエアで扱うイメージから分割グラフィックハンドルを作成する
extern	int			CreateGraphFromBaseImage( BASEIMAGE *BaseImage ) ;																											// 基本イメージデータからグラフィックハンドルを作成する
extern	int			CreateGraphFromRectBaseImage( BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY ) ;																	// 基本イメージの指定の領域を使ってグラフィックハンドルを作成する
extern	int			ReCreateGraphFromBaseImage( BASEIMAGE *BaseImage, int GrHandle ) ;																							// 基本イメージデータから既存のグラフィックハンドルに画像データを転送する
extern	int			ReCreateGraphFromRectBaseImage( BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, int GrHandle ) ;													// 基本イメージの指定の領域を使って既存のグラフィックハンドルに画像データを転送する
extern	int			CreateDivGraphFromBaseImage( BASEIMAGE *BaseImage, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf ) ;									// 基本イメージデータから分割グラフィックハンドルを作成する
extern	int			ReloadGraph( const char *FileName, int GrHandle, int ReverseFlag = FALSE ) ;																				// 画像への画像データの読み込み
extern	int			ReloadDivGraph( const char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf, int ReverseFlag = FALSE ) ;						// 画像への画像データの分割読み込み
extern	int			ReloadReverseGraph( const char *FileName, int GrHandle ) ;																									// 画像への画像データの読み込み
extern	int			ReloadReverseDivGraph( const char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf ) ;										// 画像への画像データの分割読み込み

// 基本イメージ描画
extern	int			DrawBaseImage( int x, int y, BASEIMAGE *BaseImage ) ;																																															// 基本イメージデータを描画する

// カメラ関係関数
extern	int			SetCameraNearFar( float Near, float Far ) ;																			// カメラの Nearクリップ面と Farクリップ面の距離を設定する
extern	int			SetCameraPositionAndTarget_UpVecY( VECTOR Position, VECTOR Target ) ;												// カメラの設定をする、視点、注視点、アップベクトルはＹ軸版
extern	int			SetCameraPositionAndTargetAndUpVec( VECTOR Position, VECTOR Target, VECTOR Up ) ;									// カメラの設定をする、視点、注視点、アップベクトル指定版
extern	int			SetCameraPositionAndAngle( VECTOR Position, float VRotate, float HRotate, float TRotate ) ;							// カメラの設定をする、垂直回転角度、水平回転角度、捻り回転角度、視点指定版
extern	int			SetCameraViewMatrix( MATRIX ViewMatrix ) ;																			// ビュー行列を直接設定する
extern	int			SetCameraScreenCenter( float x, float y ) ;																			// 画面上におけるカメラが見ている映像の中心の座標を設定する

extern	int			SetupCamera_Perspective( float Fov ) ;																				// 遠近法カメラをセットアップする
extern	int			SetupCamera_Ortho( float Size ) ;																					// 正射影カメラをセットアップする
extern	int			SetupCamera_ProjectionMatrix( MATRIX ProjectionMatrix ) ;															// 射影行列を設定する
extern	int			SetCameraDotAspect( float DotAspect ) ;																				// カメラのドットアスペクト比を設定する

extern	float		GetCameraNear( void ) ;																								// カメラの Near クリップ面の距離を取得する
extern	float		GetCameraFar( void ) ;																								// カメラの Far クリップ面の距離を取得する

extern	VECTOR		GetCameraPosition( void ) ;																							// カメラの位置を取得する
extern	VECTOR		GetCameraTarget( void ) ;																							// カメラの注視点を取得する
extern	VECTOR		GetCameraUpVector( void ) ;																							// カメラの視線に垂直な上方向のベクトルを得る
extern	float		GetCameraAngleHRotate( void ) ;																						// カメラの水平方向の向きを取得する
extern	float		GetCameraAngleVRotate( void ) ;																						// カメラの垂直方向の向きを取得する
extern	float		GetCameraAngleTRotate( void ) ;																						// カメラの向きの捻り角度を取得する

extern	MATRIX		GetCameraViewMatrix( void ) ;																						// ビュー行列を取得する
extern	float		GetCameraFov( void ) ;																								// カメラの視野角を取得する
extern	float		GetCameraSize( void ) ;																								// カメラの視野サイズを取得する
extern	MATRIX		GetCameraProjectionMatrix( void ) ;																					// 射影行列を取得する
extern	float		GetCameraDotAspect( void ) ;																						// カメラのドットアスペクト比を得る

// ライト関係関数
extern	int			SetUseLighting( int Flag ) ;																						// ライティング計算機能を使用するかどうかを設定する
extern	int			SetMaterialUseVertDifColor( int UseFlag ) ;																			// ３Ｄ図形描画のライティング計算に頂点カラーのディフューズカラーを使用するかどうかを設定する
extern	int			SetMaterialUseVertSpcColor( int UseFlag ) ;																			// ３Ｄ図形描画のライティング計算に頂点カラーのスペキュラカラーを使用するかどうかを設定する
extern	int			SetMaterialParam( MATERIALPARAM Material ) ;																		// ３Ｄ図形描画のライティング計算に使用するマテリアルパラメータを設定する
extern	int			SetUseSpecular( int UseFlag ) ;																						// ３Ｄ図形描画にスペキュラを使用するかどうかを設定する
extern	int			SetGlobalAmbientLight( COLOR_F Color ) ;																			// グローバルアンビエントライトカラーを設定する

extern	int			ChangeLightTypeDir( VECTOR Direction ) ;																			// ライトのタイプをディレクショナルライトにする
extern	int			ChangeLightTypeSpot( VECTOR Position, VECTOR Direction, float OutAngle, float InAngle, float Range, float Atten0, float Atten1, float Atten2 ) ;	// ライトのタイプをスポットライトにする
extern	int			ChangeLightTypePoint( VECTOR Position, float Range, float Atten0, float Atten1, float Atten2 ) ;					// ライトのタイプをポイントライトにする
extern	int			SetLightEnable( int EnableFlag ) ;																					// ライトの有効、無効をセットする
extern	int			SetLightDifColor( COLOR_F Color ) ;																					// ライトのディフューズカラーを設定する
extern	int			SetLightSpcColor( COLOR_F Color ) ;																					// ライトのスペキュラカラーを設定する
extern	int			SetLightAmbColor( COLOR_F Color ) ;																					// ライトのアンビエントカラーを設定する
extern	int			SetLightDirection( VECTOR Direction ) ;																				// ライトの方向を設定する
extern	int			SetLightPosition( VECTOR Position ) ;																				// ライトの位置を設定する
extern	int			SetLightRangeAtten( float Range, float Atten0, float Atten1, float Atten2 ) ;										// ライトの距離減衰パラメータを設定する( 有効距離、距離減衰係数０、１、２ )
extern	int			SetLightAngle( float OutAngle, float InAngle ) ;																	// ライトのスポットライトのパラメータを設定する( 外部コーン角度、内部コーン角度 )
extern	int			GetLightType( void ) ;																								// ライトのタイプを取得する( 戻り値は DX_LIGHTTYPE_DIRECTIONAL 等 )
extern	int			GetLightEnable( void ) ;																							// ライト効果の有効、無効を取得する( TRUE:有効  FALSE:無効 )
extern	COLOR_F		GetLightDifColor( void ) ;																							// ライトのディフューズカラーを取得する
extern	COLOR_F		GetLightSpcColor( void ) ;																							// ライトのスペキュラカラーを取得する
extern	COLOR_F		GetLightAmbColor( void ) ;																							// ライトのアンビエントカラーを取得する
extern	VECTOR		GetLightDirection( void ) ;																							// ライトの方向を取得する
extern	VECTOR		GetLightPosition( void ) ;																							// ライトの位置を取得する
extern	int			GetLightRangeAtten( float *Range, float *Atten0, float *Atten1, float *Atten2 )	;									// ライトの距離減衰パラメータを取得する( 有効距離、距離減衰係数０、１、２ )
extern	int			GetLightAngle( float *OutAngle, float *InAngle ) ;																	// ライトのスポットライトのパラメータを取得する( 外部コーン角度、内部コーン角度 )

extern	int			CreateDirLightHandle( VECTOR Direction ) ;																			// ディレクショナルライトハンドルを作成する
extern	int			CreateSpotLightHandle( VECTOR Position, VECTOR Direction, float OutAngle, float InAngle, float Range, float Atten0, float Atten1, float Atten2 ) ;	// スポットライトハンドルを作成する
extern	int			CreatePointLightHandle( VECTOR Position, float Range, float Atten0, float Atten1, float Atten2 ) ;					// ポイントライトハンドルを作成する
extern	int			DeleteLightHandle( int LHandle ) ;																					// ライトハンドルを削除する
extern	int			DeleteLightHandleAll( void ) ;																						// ライトハンドルを全て削除する
extern	int			SetLightTypeHandle( int LHandle, int LightType ) ;																	// ライトハンドルのライトのタイプを変更する( DX_LIGHTTYPE_DIRECTIONAL 等 )
extern	int			SetLightEnableHandle( int LHandle, int EnableFlag ) ;																// ライトハンドルのライト効果の有効、無効を設定する( TRUE:有効  FALSE:無効 )
extern	int			SetLightDifColorHandle( int LHandle, COLOR_F Color ) ;																// ライトハンドルのライトのディフューズカラーを設定する
extern	int			SetLightSpcColorHandle( int LHandle, COLOR_F Color ) ;																// ライトハンドルのライトのスペキュラカラーを設定する
extern	int			SetLightAmbColorHandle( int LHandle, COLOR_F Color ) ;																// ライトハンドルのライトのアンビエントカラーを設定する
extern	int			SetLightDirectionHandle( int LHandle, VECTOR Direction ) ;															// ライトハンドルのライトの方向を設定する
extern	int			SetLightPositionHandle( int LHandle, VECTOR Position ) ;															// ライトハンドルのライトの位置を設定する
extern	int			SetLightRangeAttenHandle( int LHandle, float Range, float Atten0, float Atten1, float Atten2 ) ;					// ライトハンドルのライトの距離減衰パラメータを設定する( 有効距離、距離減衰係数０、１、２ )
extern	int			SetLightAngleHandle( int LHandle, float OutAngle, float InAngle ) ;													// ライトハンドルのライトのスポットライトのパラメータを設定する( 外部コーン角度、内部コーン角度 )
extern	int			GetLightTypeHandle( int LHandle ) ;																					// ライトハンドルのライトのタイプを取得する( 戻り値は DX_LIGHTTYPE_DIRECTIONAL 等 )
extern	int			GetLightEnableHandle( int LHandle ) ;																				// ライトハンドルのライト効果の有効、無効を取得する( TRUE:有効  FALSE:無効 )
extern	COLOR_F		GetLightDifColorHandle( int LHandle ) ;																				// ライトハンドルのライトのディフューズカラーを取得する
extern	COLOR_F		GetLightSpcColorHandle( int LHandle ) ;																				// ライトハンドルのライトのスペキュラカラーを取得する
extern	COLOR_F		GetLightAmbColorHandle( int LHandle ) ;																				// ライトハンドルのライトのアンビエントカラーを取得する
extern	VECTOR		GetLightDirectionHandle( int LHandle ) ;																			// ライトハンドルのライトの方向を取得する
extern	VECTOR		GetLightPositionHandle( int LHandle ) ;																				// ライトハンドルのライトの位置を取得する
extern	int			GetLightRangeAttenHandle( int LHandle, float *Range, float *Atten0, float *Atten1, float *Atten2 ) ;				// ライトハンドルのライトの距離減衰パラメータを取得する( 有効距離、距離減衰係数０、１、２ )
extern	int			GetLightAngleHandle( int LHandle, float *OutAngle, float *InAngle ) ;												// ライトハンドルのライトのスポットライトのパラメータを取得する( 外部コーン角度、内部コーン角度 )

// グラフィック関係設定関数
extern	int			SetGraphColorBitDepth( int ColorBitDepth ) ;						// SetCreateGraphColorBitDepth の旧名称
extern	int			SetCreateGraphColorBitDepth( int BitDepth ) ;						// 作成するグラフィックの色深度を設定する
extern	int			GetCreateGraphColorBitDepth( void ) ;								// 作成するグラフィックの色深度を設定する
extern 	int			GetGraphColorBitDepth( void ) ;										// GetCreateGraphColorBitDepth の旧名称
extern	int			SetDrawValidGraphCreateFlag( int Flag ) ;							// 描画可能なグラフィックを作成するかどうかのフラグをセットする( TRUE:描画可能  FALSE:描画不可能 )
extern	int			GetDrawValidGraphCreateFlag( void ) ;								// 描画可能なグラフィックを作成するかどうかのフラグを取得する
extern	int			SetLeftUpColorIsTransColorFlag( int Flag ) ;						// 画像左上の色を透過色にするかどうかのフラグをセットする
extern	int			GetUseBlendGraphCreateFlag( void ) ;								// ブレンド処理用画像を作成するかどうかのフラグを取得する
extern	int			GetUseSystemMemGraphCreateFlag( void ) ;							// システムメモリ上にグラフィックを作成するかどうかのフラグを取得する( TRUE:システムメモリ上に作成  FALSE:ＶＲＡＭ上に作成 )
extern	int			SetUseBlendGraphCreateFlag( int Flag ) ;							// ブレンド処理用画像を作成するかどうかのフラグをセットする
extern	int			SetUseAlphaTestGraphCreateFlag( int Flag ) ;						// アルファテストを使用するグラフィックを作成するかどうかのフラグをセットする
extern	int			SetUseAlphaTestFlag( int Flag ) ;									// SetUseAlphaTestGraphCreateFlag の旧名称
extern	int			SetUseNoBlendModeParam( int Flag ) ;								// SetDrawBlendMode 関数の第一引数に DX_BLENDMODE_NOBLEND を代入した際に、デフォルトでは第二引数は内部で２５５を指定したことになるが、その自動２５５化をしないかどうかを設定する( TRUE:しない(第二引数の値が使用される)   FALSE:する(第二引数の値は無視されて 255 が常に使用される)(デフォルト) )αチャンネル付き画像に対して描画を行う場合のみ意味がある関数
extern	int			SetDrawValidFlagOf3DGraph( int Flag ) ;								// SetDrawValidGraphCreateFlag の旧名称
extern	int			GetUseAlphaTestFlag( void ) ;										// GetUseAlphaTestGraphCreateFlag の旧名称
extern	int			GetUseAlphaTestGraphCreateFlag( void ) ;							// アルファテストを使用するグラフィックを作成するかどうかのフラグを取得する
extern	int			SetDrawValidAlphaChannelGraphCreateFlag( int Flag ) ;				// 描画可能なαチャンネル付き画像を作成するかどうかのフラグをセットする,SetDrawValidGraphCreateFlag 関数で描画可能画像を作成するように設定されていないと効果がない( TRUE:αチャンネル付き FALSE:αチャンネルなし )
extern	int			SetUseZBufferFlag( int Flag ) ;										// Ｚバッファを有効にするか、フラグをセットする( ２Ｄと３Ｄ描画に影響 )
extern	int			SetWriteZBufferFlag( int Flag ) ;									// Ｚバッファに書き込みを行うか、フラグをセットする( ２Ｄと３Ｄ描画に影響 )
extern	int			SetZBufferCmpType( int CmpType /* DX_CMP_NEVER 等 */ ) ;			// Ｚ値の比較モードをセットする( ２Ｄと３Ｄ描画に影響 )
extern	int			SetZBias( int Bias ) ;												// Ｚバイアスをセットする( ２Ｄと３Ｄ描画に影響 )
extern	int			SetUseZBuffer3D( int Flag ) ;										// Ｚバッファを有効にするか、フラグをセットする( ３Ｄ図形描画のみに影響 )
extern	int			SetWriteZBuffer3D( int Flag ) ;										// Ｚバッファに書き込みを行うか、フラグをセットする( ３Ｄ図形描画のみに影響 )
extern	int			SetZBufferCmpType3D( int CmpType /* DX_CMP_NEVER 等 */ ) ;			// Ｚ値の比較モードをセットする( ３Ｄ図形描画のみに影響 )
extern	int			SetZBias3D( int Bias ) ;											// Ｚバイアスをセットする( ３Ｄ図形描画のみに影響 )
extern	int			SetDrawZ( float Z ) ;												// ２Ｄ描画時にＺバッファに書き込むＺ値を変更する
extern	int			SetUseTransColor( int Flag ) ;										// 透過色機能を使用するかどうかを設定する
extern	int			SetUseTransColorGraphCreateFlag( int Flag ) ;						// 透過色機能を使用することを前提とした画像の読み込み処理を行うかどうかを設定する( TRUE にすると SetDrawMode( DX_DRAWMODE_BILINEAR ); をした状態で DrawGraphF 等の浮動小数点型座標を受け取る関数で小数点以下の値を指定した場合に発生する描画結果の不自然を緩和する効果がある ( デフォルトは FALSE ) )
extern 	int			SetUseAlphaChannelGraphCreateFlag( int Flag ) ;						// αチャンネル付きグラフィックを作成するかどうかのフラグをセットする( TRUE:αチャンネル付き   FALSE:αチャンネル無し )
extern 	int			SetUseGraphAlphaChannel( int Flag ) ;								// SetUseAlphaChannelGraphCreateFlag の旧名称
extern 	int			GetUseAlphaChannelGraphCreateFlag( void ) ;							// αチャンネル付きグラフィックを作成するかどうかのフラグを取得する( TRUE:αチャンネル付き   FALSE:αチャンネル無し )
extern 	int			GetUseGraphAlphaChannel( void ) ;									// GetUseAlphaChannelGraphCreateFlag の旧名称
extern	int			SetUseNotManageTextureFlag( int Flag ) ;							// 非管理テクスチャを使用するか、のフラグをセット( TRUE:使用する  FALSE:使用しない )
extern	int			SetTransColor( int Red, int Green, int Blue ) ;						// グラフィックに設定する透過色をセットする
extern	int			GetTransColor( int *Red, int *Green, int *Blue ) ;					// 透過色を得る
extern	int			GetDrawArea( RECT *Rect ) ;											// 描画可能領域を得る
extern	int			GetUseNotManageTextureFlag( void ) ;								// 非管理テクスチャを使用するか、のフラグを取得する( TRUE:使用する  FALSE:使用しない )
extern	int			SetUseBasicGraphDraw3DDeviceMethodFlag( int Flag ) ;				// 単純図形の描画に３Ｄデバイスの機能を使用するかどうかのフラグをセットする
extern	int			SetWindowDrawRect( RECT *DrawRect ) ;								// 通常使用しない
extern	int			GetDrawBlendMode( int *BlendMode, int *BlendParam ) ;				// 描画ブレンドモードを取得する
extern	int			GetDrawMode( void ) ;												// 描画モードを取得する
extern	int			GetDrawBright( int *Red, int *Green, int *Blue ) ;					// 描画輝度を取得する
extern	int			GetActiveGraph( void ) ;											// アクティブになっているグラフィックのハンドルを得る
extern	int			GetTexFormatIndex( IMAGEFORMATDESC *Format ) ;						// テクスチャフォーマットのインデックスを得る
extern	int			SetWaitVSyncFlag( int Flag ) ;										// ＶＳＹＮＣ待ちをするかのフラグセット
extern	int			GetWaitVSyncFlag( void ) ;											// ＶＳＹＮＣ待ちをする設定になっているかどうかを取得する
extern	int			ColorKaiseki( void *PixelData, COLORDATA* ColorData ) ;				// 色ビット情報解析
extern	int			SetDefTransformMatrix( void ) ;										// デフォルトの変換行列をセットする
extern	int			CreatePixelFormat( D_DDPIXELFORMAT *PixelFormatBuf, int ColorBitDepth,
										 DWORD RedMask, DWORD GreenMask, DWORD BlueMask, DWORD AlphaMask ) ;		// DDPIXELFORMATデータを作成する
extern	int			SetEmulation320x240( int Flag ) ;									// ６４０ｘ４８０の画面で３２０ｘ２４０の画面解像度にするかどうかのフラグをセットする、６４０ｘ４８０以外の解像度では無効( TRUE:有効  FALSE:無効 )








// DxMask.cpp 関数プロトタイプ宣言

#ifndef DX_NON_MASK

// マスク関係 
extern	int			CreateMaskScreen( void ) ;																				// マスクスクリーンを作成する
extern	int			DeleteMaskScreen( void ) ;																				// マスクスクリーンを削除する
extern	void*		GetMaskSurface( void ) ; /* 戻り値を IDirectDrawSurface7 * にキャストして下さい */						// マスクサーフェスを取得する
//extern	MEMIMG*	GetMaskMemImg( void ) ;																				// マスクサーフェスの代わりの MEMIMG を取得する
extern	int			DrawMaskToDirectData( int x, int y, int Width, int Height, void *MaskData , int TransMode ) ;			// マスクのデータを直接セット
extern	int			DrawFillMaskToDirectData( int x1, int y1, int x2, int y2,  int Width, int Height, void *MaskData ) ;	// マスクのデータを直接マスク画面全体に描画する

extern	int			SetUseMaskScreenFlag( int ValidFlag ) ;																	// マスク使用モードを変更
extern	int			GetUseMaskScreenFlag( void ) ;																			// マスク使用モードの取得
extern	int			FillMaskScreen( int Flag ) ;																			// マスクスクリーンを指定の色で塗りつぶす

extern	int			InitMask( void ) ;																						// マスクデータを初期化する
extern	int			MakeMask( int Width, int Height ) ;																		// マスクデータの追加
extern	int			GetMaskSize( int *WidthBuf, int *HeightBuf, int MaskHandle ) ;											// マスクの大きさを得る 
extern	int			SetDataToMask( int Width, int Height, void *MaskData, int MaskHandle ) ;								// マスクのデータをマスクに転送する
extern	int			DeleteMask( int MaskHandle ) ;																			// マスクデータを削除
extern	int			BmpBltToMask( HBITMAP Bmp, int BmpPointX, int BmpPointY, int MaskHandle ) ;								// マスクデータにＢＭＰデータをマスクデータと見たてて転送
extern	int			LoadMask( const char *FileName ) ;																		// マスクデータをロードする
extern	int			LoadDivMask( const char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf ) ;	// マスクを画像から分割読みこみ
extern	int			DrawMask( int x, int y, int MaskHandle, int TransMode ) ;												// マスクをセットする
extern	int			DrawFormatStringMask( int x, int y, int Flag, const char *FormatString, ... ) ;							// 書式指定ありの文字列をマスクスクリーンに描画する
extern	int			DrawFormatStringMaskToHandle( int x, int y, int Flag, int FontHandle, const char *FormatString, ... ) ;	// 書式指定ありの文字列をマスクスクリーンに描画する(フォントハンドル指定版)
extern	int			DrawStringMask( int x, int y, int Flag, const char *String ) ;											// 文字列をマスクスクリーンに描画する
extern	int			DrawStringMaskToHandle( int x, int y, int Flag, int FontHandle, const char *String ) ;					// 文字列をマスクスクリーンに描画する(フォントハンドル指定版)
extern	int			DrawFillMask( int x1, int y1, int x2, int y2, int MaskHandle ) ;										// 指定のマスクを画面いっぱいに展開する	
extern	int			SetMaskTransColor( int ColorCode ) ;																	// マスクに使う透過色のセット
extern	int			SetMaskReverseEffectFlag( int ReverseFlag ) ;															// マスクの数値に対する効果を逆転させる

extern 	int			GetMaskScreenData( int x1, int y1, int x2, int y2, int MaskHandle ) ;									// マスク画面上の描画状態を取得する
extern	int			GetMaskUseFlag( void ) ;																				// マスクスクリーンを使用中かフラグの取得

#endif










// DxMovie.cpp 関数プロトタイプ宣言
extern	int			SetMovieRightImageAlphaFlag( int Flag ) ;							// 動画ファイルの右半分をアルファ情報として扱うかどうかをセットする( TRUE:アルファ情報として扱う )













// DxFont.cpp 関数プロトタイプ宣言

// フォント、文字列描画関係関数
extern	int			EnumFontName( char *NameBuffer, int NameBufferNum, int JapanOnlyFlag = TRUE ) ;								// 使用可能なフォントの名前をすべて列挙する
extern	int			InitFontToHandle( void ) ;																					// フォントのステータスをデフォルトに戻す
extern	int			CreateFontToHandle( const char *FontName, int Size, int Thick, int FontType = -1 , int CharSet = -1 , int EdgeSize = -1 , int Italic = FALSE , int DataIndex = -1 , int ID = -1 ) ;			// 新しいフォントデータを作成
extern	int			SetFontSpaceToHandle( int Point, int FontHandle ) ;															// 字間を変更する
extern	int			SetDefaultFontState( const char *FontName, int Size, int Thick ) ;											// デフォルトフォントのステータスを一括設定する
extern	int			DeleteFontToHandle( int FontHandle ) ;																		// フォントキャッシュの制御を終了する
extern	int			SetFontLostFlag( int FontHandle, int *LostFlag ) ;															// 解放時に TRUE にするフラグへのポインタを設定する
extern	int			SetFontSize( int FontSize ) ;																				// 描画するフォントのサイズをセットする
extern	int			SetFontThickness( int ThickPal ) ;																			// フォントの太さをセット
extern	int			SetFontSpace( int Point ) ;																					// 字間を変更する
extern	int			SetFontCacheToTextureFlag( int Flag ) ;																		// フォントのキャッシュにテクスチャを使用するか、フラグをセットする
extern	int			SetFontChacheToTextureFlag( int Flag ) ;																	// フォントのキャッシュにテクスチャを使用するか、フラグをセットする(誤字版)
extern	int			SetFontCacheCharNum( int CharNum ) ;																		// フォントキャッシュでキャッシュできる文字数を指定する
extern	int			ChangeFont( const char *FontName, int CharSet = -1 /* DX_CHARSET_SHFTJIS */ ) ;								// フォントを変更
extern	int			ChangeFontType( int FontType ) ;																			// フォントタイプの変更
// FontCacheStringDraw は DrawString を使ってください
extern	int			FontCacheStringDrawToHandle( int x, int y, const char *StrData, int Color, int EdgeColor,
													BASEIMAGE *DestImage, RECT *ClipRect /* NULL 可 */ , int FontHandle,
													int VerticalFlag = FALSE , SIZE *DrawSizeP = NULL ) ;
extern	int			FontBaseImageBlt( 
						int x, int y, const char *StrData,
						BASEIMAGE *DestImage, BASEIMAGE *DestEdgeImage,
						int VerticalFlag = FALSE ) ;																// 基本イメージに文字列イメージを転送する
extern	int			FontBaseImageBltToHandle( 
						int x, int y, const char *StrData,
						BASEIMAGE *DestImage, BASEIMAGE *DestEdgeImage,
						int FontHandle, int VerticalFlag = FALSE ) ;																// 基本イメージに文字列イメージを転送する

extern	int			GetFontMaxWidth( void ) ;																					// 文字の最大幅を得る
extern	int			GetFontMaxWidthToHandle( int FontHandle ) ;																	// 文字の最大幅を得る
extern	int			GetDrawStringWidth( const char *String, int StrLen, int VerticalFlag = FALSE ) ;							// 文字列の幅を得る
extern	int			GetDrawFormatStringWidth( const char *FormatString, ... ) ;													// 書式付き文字列の描画幅を得る
extern	int			GetDrawStringWidthToHandle( const char *String, int StrLen, int FontHandle, int VerticalFlag = FALSE ) ;	// 文字列の幅を得る
extern	int			GetDrawFormatStringWidthToHandle( int FontHandle, const char *FormatString, ... ) ;							// 書式付き文字列の描画幅を得る
extern	int			GetDrawExtendStringWidth( double ExRateX, const char *String, int StrLen, int VerticalFlag = FALSE ) ;							// 文字列の幅を得る
extern	int			GetDrawExtendFormatStringWidth( double ExRateX, const char *FormatString, ... ) ;												// 書式付き文字列の描画幅を得る
extern	int			GetDrawExtendStringWidthToHandle( double ExRateX, const char *String, int StrLen, int FontHandle, int VerticalFlag = FALSE ) ;	// 文字列の幅を得る
extern	int			GetDrawExtendFormatStringWidthToHandle( double ExRateX, int FontHandle, const char *FormatString, ... ) ;						// 書式付き文字列の描画幅を得る
extern	int			GetFontStateToHandle( char *FontName, int *Size, int *Thick, int FontHandle) ;								// フォントの情報を得る
extern	int			GetDefaultFontHandle( void ) ;																				// デフォルトのフォントのハンドルを得る
extern	int			GetFontChacheToTextureFlag( void ) ;																		// フォントにテクスチャキャッシュを使用するかどうかを取得する
extern	int			CheckFontChacheToTextureFlag( int FontHandle ) ;															// 指定のフォントがテクスチャキャッシュを使用しているかどうかを得る
//extern	LPFONTMANAGE GetFontManageDataToHandle( int FontHandle ) ;																// フォント管理データの取得
extern	int			CheckFontHandleValid( int FontHandle ) ;																	// 指定のフォントハンドルが有効か否か調べる
extern	int			MultiByteCharCheck( const char *Buf, int CharSet /* DX_CHARSET_SHFTJIS */ ) ;									// ２バイト文字か調べる( TRUE:２バイト文字  FALSE:１バイト文字 )
extern	int			GetFontCacheCharNum( void ) ;																				// フォントキャッシュでキャッシュできる文字数を取得する( 戻り値  0:デフォルト  1以上:指定文字数 )
extern	int			GetFontSize( void ) ;																						// フォントのサイズを得る
extern	int			GetFontSizeToHandle( int FontHandle ) ;																		// フォントのサイズを得る









// DxMath.cpp 関数プロトタイプ宣言

// 演算ライブラリ
extern	int			CreateIdentityMatrix( MATRIX *Out ) ;														// 単位行列を作成する
extern	int			CreateLookAtMatrix( MATRIX *Out, VECTOR *Eye, VECTOR *At, VECTOR *Up ) ;					// ビュー行列を作成する
extern	int			CreateLookAtMatrix2( MATRIX *Out, VECTOR *Eye, double XZAngle, double Oira ) ;				// ビュー行列を作成する
extern	int			CreateLookAtMatrixRH( MATRIX *Out, VECTOR *Eye, VECTOR *At, VECTOR *Up ) ;					// ビュー行列を作成する(右手座標系用)
extern	int			CreateMultiplyMatrix( MATRIX *Out, MATRIX *In1, MATRIX *In2 ) ;								// 行列の積を求める
extern	int			CreatePerspectiveFovMatrix( MATRIX *Out, float fov, float zn, float zf, float aspect = -1.0f ) ;	// 射影行列を作成する
extern	int			CreatePerspectiveFovMatrixRH( MATRIX *Out, float fov, float zn, float zf, float aspect = -1.0f ) ;	// 射影行列を作成する(右手座標系用)
extern	int			CreateOrthoMatrix( MATRIX *Out, float size, float zn, float zf, float aspect = -1.0f ) ;	// 正射影行列を作成する
extern	int			CreateOrthoMatrixRH( MATRIX *Out, float size, float zn, float zf, float aspect = -1.0f ) ;	// 正射影行列を作成する(右手座標系用)
extern	int			CreateScalingMatrix( MATRIX *Out, float sx, float sy, float sz ) ;							// スケーリング行列を作成する
extern	int			CreateRotationXMatrix( MATRIX *Out, float Angle ) ;											// Ｘ軸を中心とした回転行列を作成する
extern	int			CreateRotationYMatrix( MATRIX *Out, float Angle ) ;											// Ｙ軸を中心とした回転行列を作成する
extern	int			CreateRotationZMatrix( MATRIX *Out, float Angle ) ;											// Ｚ軸を中心とした回転行列を作成する
extern	int			CreateTranslationMatrix( MATRIX *Out, float x, float y, float z ) ;							// 平行移動行列を作成する
extern	int			CreateTransposeMatrix( MATRIX *Out, MATRIX *In ) ;											// 転置行列を作成する
extern	int			CreateInverseMatrix( MATRIX *Out, MATRIX *In ) ;											// 逆行列を作成する
extern	int			CreateViewportMatrix( MATRIX *Out, float CenterX, float CenterY, float Width, float Height ) ; // ビューポート行列を作成する
extern	int			CreateRotationXYZMatrix( MATRIX *Out, float XRot, float YRot, float ZRot ) ;				// Ｘ軸回転→Ｙ軸回転→Ｚ軸回転を合成した行列を作成する
extern	int			CreateRotationXZYMatrix( MATRIX *Out, float XRot, float YRot, float ZRot ) ;				// Ｘ軸回転→Ｚ軸回転→Ｙ軸回転を合成した行列を作成する
extern	int			CreateRotationYXZMatrix( MATRIX *Out, float XRot, float YRot, float ZRot ) ;				// Ｙ軸回転→Ｘ軸回転→Ｚ軸回転を合成した行列を作成する
extern	int			CreateRotationYZXMatrix( MATRIX *Out, float XRot, float YRot, float ZRot ) ;				// Ｙ軸回転→Ｚ軸回転→Ｘ軸回転を合成した行列を作成する
extern	int			CreateRotationZXYMatrix( MATRIX *Out, float XRot, float YRot, float ZRot ) ;				// Ｚ軸回転→Ｘ軸回転→Ｙ軸回転を合成した行列を作成する
extern	int			CreateRotationZYXMatrix( MATRIX *Out, float XRot, float YRot, float ZRot ) ;				// Ｚ軸回転→Ｙ軸回転→Ｘ軸回転を合成した行列を作成する
extern	int			GetMatrixXYZRotation( MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot ) ;		// 行列からＸＹＺ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
extern	int			GetMatrixXZYRotation( MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot ) ;		// 行列からＸＺＹ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
extern	int			GetMatrixYXZRotation( MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot ) ;		// 行列からＹＸＺ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
extern	int			GetMatrixYZXRotation( MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot ) ;		// 行列からＹＺＸ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
extern	int			GetMatrixZXYRotation( MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot ) ;		// 行列からＺＸＹ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
extern	int			GetMatrixZYXRotation( MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot ) ;		// 行列からＺＹＸ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
extern	int			VectorNormalize( VECTOR *Out, VECTOR *In ) ;												// ベクトルを正規化する
extern	int			VectorScale( VECTOR *Out, VECTOR *In, float Scale ) ;										// ベクトルをスカラー倍する
extern	int			VectorMultiply( VECTOR *Out, VECTOR *In1, VECTOR *In2 ) ;									// ベクトルの掛け算をする
extern	int			VectorSub( VECTOR *Out, VECTOR *In1, VECTOR *In2 ) ;										// Out = In1 - In2 のベクトル計算をする 
extern	int			VectorAdd( VECTOR *Out, VECTOR *In1, VECTOR *In2 ) ;										// Out = In1 + In2 のベクトル計算をする 
extern	int			VectorOuterProduct( VECTOR *Out, VECTOR *In1, VECTOR *In2 ) ;								// In1とIn2の外積を計算する
extern	float		VectorInnerProduct( VECTOR *In1, VECTOR *In2 ) ;											// In1とIn2の内積を計算する
extern	int			VectorRotationX( VECTOR *Out, VECTOR *In, double Angle ) ;									// ベクトルのＸ軸を軸にした回転を行う
extern	int			VectorRotationY( VECTOR *Out, VECTOR *In, double Angle ) ;									// ベクトルのＹ軸を軸にした回転を行う
extern	int			VectorRotationZ( VECTOR *Out, VECTOR *In, double Angle ) ;									// ベクトルのＺ軸を軸にした回転を行う
extern	int			VectorTransform( VECTOR *Out, VECTOR *InVec, MATRIX *InMatrix ) ;							// ベクトル行列と4x4正方行列を乗算する( w は 1 と仮定 )
extern	int			VectorTransform4( VECTOR *Out, float *V4Out, VECTOR *InVec, float *V4In, MATRIX *InMatrix ) ; // ベクトル行列と4x4正方行列を乗算する( w の要素を渡す )

extern	void		TriangleBarycenter( VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3, VECTOR Position, float *u, float *v, float *w ) ;	// 指定の座標から三角形の重心を求める

extern	float		Segment_Segment_MinLength( VECTOR SegmentAPos1, VECTOR SegmentAPos2, VECTOR SegmentBPos1, VECTOR SegmentBPos2 ) ;								// 二つの線分の最近点間の距離を得る
extern	float		Segment_Segment_MinLength_Square( VECTOR SegmentAPos1, VECTOR SegmentAPos2, VECTOR SegmentBPos1, VECTOR SegmentBPos2 ) ;						// 二つの線分の最近点間の距離の二乗を得る
extern	float		Segment_Triangle_MinLength( VECTOR SegmentPos1, VECTOR SegmentPos2, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 ) ;			// 線分と三角形の最近点間の距離を得る 
extern	float		Segment_Triangle_MinLength_Square( VECTOR SegmentPos1, VECTOR SegmentPos2, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 ) ;	// 線分と三角形の最近点間の距離の二乗を得る 
extern	float		Segment_Point_MinLength( VECTOR SegmentPos1, VECTOR SegmentPos2, VECTOR PointPos ) ;															// 線分と点の一番近い距離を得る
extern	float		Segment_Point_MinLength_Square( VECTOR SegmentPos1, VECTOR SegmentPos2, VECTOR PointPos ) ;														// 線分と点の一番近い距離の二乗を得る
extern	float		Triangle_Point_MinLength( VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3, VECTOR PointPos ) ;									// 三角形と点の一番近い距離を得る
extern	float		Triangle_Point_MinLength_Square( VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3, VECTOR PointPos ) ;								// 三角形と点の一番近い距離の二乗を得る

extern	HITRESULT_LINE HitCheck_Line_Triangle( VECTOR LinePos1, VECTOR LinePos2, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 ) ;					// 三角形と線の当たり判定
extern	int			HitCheck_Triangle_Triangle( VECTOR Triangle1Pos1, VECTOR Triangle1Pos2, VECTOR Triangle1Pos3, 
											    VECTOR Triangle2Pos1, VECTOR Triangle2Pos2, VECTOR Triangle2Pos3 ) ;												// 三角形と三角形の当たり判定( TRUE:当たっている  FALSE:当たっていない )
extern	int			HitCheck_Line_Sphere( VECTOR LinePos1, VECTOR LinePos2, VECTOR SphereCenterPos, float SphereR ) ;												// 線と球の当たり判定( TRUE:当たっている  FALSE:当たっていない )
extern	int			HitCheck_Sphere_Sphere( VECTOR Sphere1CenterPos, float Sphere1R, VECTOR Sphere2CenterPos, float Sphere2R ) ;									// 球と球の当たり判定( TRUE:当たっている  FALSE:当たっていない )
extern	int			HitCheck_Sphere_Triangle( VECTOR SphereCenterPos, float SphereR, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 ) ;				// 球と三角形の当たり判定( TRUE:当たっている  FALSE:当たっていない )
extern	int			HitCheck_Capsule_Capsule( VECTOR Cap1Pos1, VECTOR Cap1Pos2, float Cap1R, VECTOR Cap2Pos1, VECTOR Cap2Pos2, float Cap2R ) ;						// カプセル同士の当たり判定( TRUE:当たっている  FALSE:当たっていない )
extern	int			HitCheck_Capsule_Triangle( VECTOR CapPos1, VECTOR CapPos2, float CapR, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 ) ;		// カプセルと三角形の当たり判定( TRUE:当たっている  FALSE:当たっていない )

// その他
extern	int			RectClipping( RECT *MotoRect, RECT *ClippuRect ) ;											// 矩形のクリッピング
extern	int			RectAdjust( RECT *Rect ) ;																	// 矩形の left が right より値が大きい場合などの誤りを補正する
extern	int			GetRectSize( RECT *Rect, int *Width, int *Height ) ;										// 矩形の幅と高さを求める

// 表記簡略版
extern	MATRIX		MGetIdent( void ) ;														// 単位行列を得る
extern	MATRIX		MMult( MATRIX In1, MATRIX In2 ) ;										// 行列の乗算を行う
extern	MATRIX		MScale( MATRIX InM, float Scale ) ;										// 行列のスケーリングを行う
extern	MATRIX		MAdd( MATRIX In1, MATRIX In2 ) ;										// 行列の足し算を行う
extern	MATRIX		MGetScale( VECTOR Scale ) ;												// 拡大行列を得る
extern	MATRIX		MGetRotX( float XAxisRotate ) ;											// Ｘ軸回転行列を得る
extern	MATRIX		MGetRotY( float YAxisRotate ) ;											// Ｙ軸回転行列を得る
extern	MATRIX		MGetRotZ( float ZAxisRotate ) ;											// Ｚ軸回転行列を得る
extern	MATRIX		MGetRotAxis( VECTOR RotateAxis, float Rotate ) ;						// 指定軸で指定角度回転する行列を得る
extern	MATRIX		MGetRotVec2( VECTOR In1, VECTOR In2 ) ;									// In1 の向きから In2 の向きへ変換する回転行列を得る( In2 と In1 が真逆のベクトルの場合は回転軸は不定 )
extern	MATRIX		MGetTranslate( VECTOR Trans ) ;											// 平行移動行列を得る
extern	MATRIX		MGetAxis1( VECTOR XAxis, VECTOR YAxis, VECTOR ZAxis, VECTOR Pos ) ;		// 指定の３軸ローカルのベクトルを基本軸上のベクトルに変換する行列を得る( x' = XAxis.x * x + YAixs.x * y + ZAxis.z * z + Pos.x   y' = XAxis.y * x + YAixs.y * y + ZAxis.y * z + Pos.y      z' = XAxis.z * x + YAixs.z * y + ZAxis.z * z + Pos.z )
extern	MATRIX		MGetAxis2( VECTOR XAxis, VECTOR YAxis, VECTOR ZAxis, VECTOR Pos ) ;		// 基本軸上のベクトルを指定の３軸上に投影したベクトルに変換する行列を得る( x' = XAxis.x * ( x - Pos.x ) + XAxis.y * ( x - Pos.x ) + XAxis.z * ( x - Pos.x )    y' = YAxis.x * ( x - Pos.x ) + YAxis.y * ( x - Pos.x ) + YAxis.z * ( x - Pos.x )    z' = ZAxis.x * ( x - Pos.x ) + ZAxis.y * ( x - Pos.x ) + ZAxis.z * ( x - Pos.x ) )
extern	MATRIX		MTranspose( MATRIX InM ) ;												// 転置行列を得る
extern	MATRIX		MInverse( MATRIX InM ) ;												// 逆行列を得る

// ベクトル値の生成
__inline VECTOR VGet( float x, float y, float z )
{
	VECTOR Result = { x, y, z } ;
	return Result ;
}

// ベクトルの加算
__inline VECTOR		VAdd( const VECTOR &In1, const VECTOR &In2 )
{
	VECTOR Result = { In1.x + In2.x, In1.y + In2.y, In1.z + In2.z } ;
	return Result ;
}

// ベクトルの減算
__inline VECTOR		VSub( const VECTOR &In1, const VECTOR &In2 )
{
	VECTOR Result = { In1.x - In2.x, In1.y - In2.y, In1.z - In2.z } ;
	return Result ;
}

// ベクトルの内積
__inline float		VDot( const VECTOR &In1, const VECTOR &In2 )
{
	return In1.x * In2.x + In1.y * In2.y + In1.z * In2.z ;
}

// ベクトルの外積
__inline VECTOR		VCross( const VECTOR &In1, const VECTOR &In2 )
{
	VECTOR Result =
	{
		In1.y * In2.z - In1.z * In2.y,
		In1.z * In2.x - In1.x * In2.z,
		In1.x * In2.y - In1.y * In2.x
	} ;
	return Result ;
}

// ベクトルのスケーリング
__inline VECTOR		VScale( const VECTOR &In, float Scale )
{
	VECTOR Result = { In.x * Scale, In.y * Scale, In.z * Scale } ;
	return Result ;
}

// ベクトルの正規化
extern VECTOR		VNorm( VECTOR In ) ;

// ベクトルのサイズ
extern float		VSize( VECTOR In ) ;

// ベクトルのサイズの２乗
__inline float		VSquareSize( const VECTOR &In )
{
	return In.x * In.x + In.y * In.y + In.z * In.z ;
}

// 行列を使った座標変換
__inline VECTOR		VTransform( const VECTOR &InV, const MATRIX &InM )
{
	VECTOR Result =
	{
		InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] + InM.m[3][0],
		InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] + InM.m[3][1],
		InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] + InM.m[3][2]
	} ;
	return Result ;
}

// 行列を使った座標変換( スケーリング＋回転成分のみ )
__inline VECTOR		VTransformSR( const VECTOR &InV, const MATRIX &InM )
{
	VECTOR Result =
	{
		InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0],
		InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1],
		InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2]
	} ;
	return Result ;
}

// 二つのベクトルが成す角のコサイン値を得る
extern	float		VCos( VECTOR In1, VECTOR In2 ) ;

// 二つのベクトルが成す角の角度を得る( 単位：ラジアン )
extern	float		VRad( VECTOR In1, VECTOR In2 ) ;








// DxBaseImage.cpp 関数プロトタイプ宣言

// 基本イメージデータのロード＋ＤＩＢ関係
extern	int			CreateGraphImageOrDIBGraph( const char *FileName, void *DataImage, int DataImageSize, int DataImageType, int BmpFlag, int ReverseFlag, BASEIMAGE *Image, BITMAPINFO **BmpInfo, void **GraphData ) ;												// 登録されている各種グラフィックローダ関数から、ＢＭＰデータもしくは GraphImageデータを構築する
extern	int			CreateGraphImageType2( STREAMDATA *Src, BASEIMAGE *Dest ) ;																																														// 汎用読み込み処理によるグラフィックイメージ構築関数( 0:成功  -1:失敗 )

extern	int			CreateBmpInfo( BITMAPINFO *BmpInfo, int Width, int Height, int Pitch, void *SrcGrData, void **DestGrData ) ;																																	// フルカラー形式のBITMAPINFO構造体を作成する
extern	HBITMAP		CreateDIBGraphVer2( const char *FileName, void *MemImage, int MemImageSize, int ImageType, int ReverseFlag, COLORDATA *SrcColor ) ;																												// ＤＩＢグラフィックを作成する(バージョン２)
extern	int			CreateDIBGraphVer2_plus_Alpha( const char *FileName, void *MemImage, int MemImageSize, void *AlphaImage, int AlphaImageSize, int ImageType, HBITMAP *RGBBmp, HBITMAP *AlphaBmp, int ReverseFlag, COLORDATA *SrcColor ) ;						// ＤＩＢグラフィックを作成する
extern	DWORD		GetGraphImageFullColorCode( BASEIMAGE *GraphImage, int x, int y ) ;																																												// BASEIMAGE 構造体の画像情報から指定の座標のフルカラーコードを取得する
extern	int			CreateGraphImage_plus_Alpha( const char *FileName, void *RgbImage, int RgbImageSize, int RgbImageType,
													void *AlphaImage, int AlphaImageSize, int AlphaImageType,
													BASEIMAGE *RgbGraphImage, BASEIMAGE *AlphaGraphImage, int ReverseFlag ) ;																																	// 各種グラフィックデータからグラフィックイメージデータとアルファマップ用イメージデータを構築する
extern	int			ReverseGraphImage( BASEIMAGE *GraphImage ) ;																																																	// 指定の GraphImage を左右反転する
extern	int			ConvBitmapToGraphImage( BITMAPINFO *BmpInfo, void *GraphData, BASEIMAGE *GraphImage, int CopyFlag ) ;																																			// ＢＭＰ を GraphImage に変換する( Ret 0:正常終了  1:コピーを行った  -1:エラー )
extern	int			ConvGraphImageToBitmap( BASEIMAGE *GraphImage, BITMAPINFO *BmpInfo, void **GraphData, int CopyFlag, int FullColorConv = TRUE ) ;																												// GraphImage を ＢＭＰ に変換する(アルファデータはあっても無視される)( Ret 0:正常終了  1:コピーを行った  -1:エラー )
extern	HBITMAP		CreateDIBGraph( const char *FileName, int ReverseFlag, COLORDATA *SrcColor) ;																																									// ＤＩＢグラフィックを作成する
extern	HBITMAP		CreateDIBGraphToMem( BITMAPINFO *BmpInfo, void *GraphData, int ReverseFlag, COLORDATA *SrcColor ) ;																																				// ＤＩＢグラフィックをメモリイメージから作成する
extern	int			CreateDIBGraph_plus_Alpha( const char *FileName, HBITMAP *RGBBmp, HBITMAP *AlphaBmp, int ReverseFlag = FALSE , COLORDATA *SrcColor = NULL ) ;																									// ファイルからＤＩＢグラフィックとマスクグラフィックを作成する

//extern int		AddUserGraphLoadFunction( int ( *UserLoadFunc )( FILE *fp, BITMAPINFO **BmpInfo, void **GraphData ) ) ;																					// ユーザー定義のグラフィックロード関数を登録する
//extern int		AddUserGraphLoadFunction2( int ( *UserLoadFunc )( void *Image, int ImageSize, int ImageType, BITMAPINFO **BmpInfo, void **GraphData ) ) ;												// ユーザー定義のグラフィックロード関数を登録する
//extern int		AddUserGraphLoadFunction3( int ( *UserLoadFunc )( void *DataImage, int DataImageSize, int DataImageType, int BmpFlag, BASEIMAGE *Image, BITMAPINFO **BmpInfo, void **GraphData ) ) ;	// ユーザー定義のグラフィックロード関数Ver3を登録する
extern	int			AddUserGraphLoadFunction4( int (* UserLoadFunc )( STREAMDATA *Src, BASEIMAGE *Image ) ) ; 																								// ユーザー定義のグラフィックロード関数Ver4を登録する
//extern int		SubUserGraphLoadFunction( int ( *UserLoadFunc )( FILE *fp, BITMAPINFO **BmpInfo, void **GraphData ) ) ;																					// ユーザー定義のグラフィックロード関数を登録から抹消する
//extern int		SubUserGraphLoadFunction2( int ( *UserLoadFunc )( void *Image, int ImageSize, int ImageType, BITMAPINFO **BmpInfo, void **GraphData ) ) ;												// ユーザー定義のグラフィックロード関数を登録から抹消する
//extern int		SubUserGraphLoadFunction3( int ( *UserLoadFunc )( void *DataImage, int DataImageSize, int DataImageType, int BmpFlag, BASEIMAGE *Image, BITMAPINFO **BmpInfo, void **GraphData ) ) ;	// ユーザー定義のグラフィックロード関数Ver3を登録から抹消する
extern	int			SubUserGraphLoadFunction4( int (* UserLoadFunc )( STREAMDATA *Src, BASEIMAGE *Image ) ) ; 																								// ユーザー定義のグラフィックロード関数Ver4を登録から抹消する

extern	int			SetUseFastLoadFlag( int Flag ) ;																							// 高速読み込みルーチンを使うか否かのフラグをセットする
extern	int			GetGraphDataShavedMode( void ) ;																							// グラフィック減色時の画像劣化緩和処理モードの取得
extern	int			SetGraphDataShavedMode( int ShavedMode ) ;																					// グラフィック減色時の画像劣化緩和処理モードの変更

// 基本イメージデータ機能公開用関数
extern	int			InitSoftImage( void ) ;																										// ソフトウエアで扱うイメージを全て解放する
extern	int			LoadSoftImage( const char *FileName ) ;																						// ソフトウエアで扱うイメージの読み込み( -1:エラー  -1以外:イメージハンドル )
extern	int			LoadSoftImageToMem( void *FileImage, int FileImageSize ) ;																	// ソフトウエアで扱うイメージのメモリからの読み込み( -1:エラー  -1以外:イメージハンドル )
extern	int			MakeSoftImage( int SizeX, int SizeY ) ;																						// ソフトウエアで扱うイメージの作成( -1:エラー  -1以外:イメージハンドル )
extern	int			MakeARGB8ColorSoftImage( int SizeX, int SizeY ) ;																			// ソフトウエアで扱うイメージの作成( RGBA8 カラー )
extern	int			MakeXRGB8ColorSoftImage( int SizeX, int SizeY ) ;																			// ソフトウエアで扱うイメージの作成( XRGB8 カラー )
extern	int			MakeRGB8ColorSoftImage( int SizeX, int SizeY ) ;																			// ソフトウエアで扱うイメージの作成( RGB8 カラー )
extern	int			MakePAL8ColorSoftImage( int SizeX, int SizeY ) ;																			// ソフトウエアで扱うイメージの作成( パレット２５６色 カラー )

extern	int			DeleteSoftImage( int SIHandle ) ;																							// ソフトウエアで扱うイメージの解放

extern	int			GetSoftImageSize( int SIHandle, int *Width, int *Height ) ;																	// ソフトウエアで扱うイメージのサイズを取得する
extern	int			CheckPaletteSoftImage( int SIHandle ) ;																						// ソフトウエアで扱うイメージがパレット画像かどうかを取得する( TRUE:パレット画像  FALSE:パレット画像じゃない )
extern	int			CheckAlphaSoftImage( int SIHandle ) ;																						// ソフトウエアで扱うイメージのフォーマットにα要素があるかどうかを取得する( TRUE:ある  FALSE:ない )

extern	int			GetDrawScreenSoftImage( int x1, int y1, int x2, int y2, int SIHandle ) ;													// 描画対象の画面から指定領域のグラフィックをソフトウエアで扱うイメージに転送する
extern	int			UpdateLayerdWindowForSoftImage( int SIHandle ) ;																			// ソフトウエアで扱うイメージを使用して UpdateLayerdWindow を行う( イメージはウインドウのクライアント領域と同じサイズである必要があります )
extern	int			FillSoftImage( int SIHandle, int r, int g, int b, int a ) ;																	// ソフトウエアで扱うイメージを指定色で塗りつぶす(各色要素は０～２５５)
extern	int			GetPaletteSoftImage( int SIHandle, int PaletteNo, int *r, int *g, int *b, int *a ) ;										// ソフトウエアで扱うイメージのパレットを取得する(各色要素は０～２５５)
extern	int			SetPaletteSoftImage( int SIHandle, int PaletteNo, int  r, int  g, int  b, int  a ) ;										// ソフトウエアで扱うイメージのパレットをセットする(各色要素は０～２５５)
extern	int			DrawPixelPalCodeSoftImage( int SIHandle, int x, int y, int palNo ) ;														// ソフトウエアで扱うイメージの指定座標にドットを描画する(パレット画像用、有効値は０～２５５)
extern	int			GetPixelPalCodeSoftImage( int SIHandle, int x, int y ) ;																	// ソフトウエアで扱うイメージの指定座標の色コードを取得する(パレット画像用、戻り値は０～２５５)
extern	void		*GetImageAddressSoftImage( int SIHandle ) ;																					// ソフトウエアで扱うイメージの実イメージが格納されているメモリ領域の先頭アドレスを取得する
extern	int			DrawPixelSoftImage( int SIHandle, int x, int y, int  r, int  g, int  b, int  a ) ;											// ソフトウエアで扱うイメージの指定座標にドットを描画する(各色要素は０～２５５)
extern	int			GetPixelSoftImage(  int SIHandle, int x, int y, int *r, int *g, int *b, int *a ) ;											// ソフトウエアで扱うイメージの指定座標の色を取得する(各色要素は０～２５５)
extern	int			DrawLineSoftImage(  int SIHandle, int x1, int y1, int x2, int y2, int r, int g, int b, int a ) ;							// ソフトウエアで扱うイメージの指定座標に線を描画する(各色要素は０～２５５)
extern	int			BltSoftImage( int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int SrcSIHandle, int DestX, int DestY, int DestSIHandle ) ;	// ソフトウエアで扱うイメージを別のイメージ上に転送する
extern	int			BltSoftImageWithTransColor( int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int SrcSIHandle, int DestX, int DestY, int DestSIHandle, int Tr, int Tg, int Tb, int Ta ) ;	// ソフトウエアで扱うイメージを透過色処理付きで転送する
extern	int			BltSoftImageWithAlphaBlend( int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int SrcSIHandle, int DestX, int DestY, int DestSIHandle, int Opacity = 255 ) ;	// ソフトウエアで扱うイメージをアルファ値のブレンドを考慮した上で転送する( Opacity は透明度 : 0( 完全透明 ) ～ 255( 完全不透明 ) )( 出力先が ARGB8 形式以外の場合はエラーになります )

extern	int			BltStringSoftImage( 
						int x, int y, const char *StrData,
						int DestSIHandle, int DestEdgeSIHandle = -1 ,
						int VerticalFlag = FALSE ) ;																							// ソフトウエアで扱うイメージに文字列イメージを転送する
extern	int			BltStringSoftImageToHandle( 
						int x, int y, const char *StrData,
						int DestSIHandle, int DestEdgeSIHandle /* 縁が必要ない場合は -1 */ ,
						int FontHandle, int VerticalFlag = FALSE ) ;																			// ソフトウエアで扱うイメージに文字列イメージを転送する( フォントハンドル使用版 )

extern	int			DrawSoftImage( int x, int y, int SIHandle ) ;																				// ソフトウエアで扱うイメージを画面に描画する

extern	int			SaveSoftImageToBmp( const char *FilePath, int SIHandle ) ;																	// ソフトウエアで扱うイメージをＢＭＰ画像ファイルとして保存する
#ifndef DX_NON_PNGREAD
extern	int			SaveSoftImageToPng( const char *FilePath, int SIHandle, int CompressionLevel ) ;											// ソフトウエアで扱うイメージをＰＮＧ画像ファイルとして保存する CompressionLevel = 圧縮率、値が大きいほど高圧縮率高負荷、０は無圧縮,0～9
#endif
#ifndef DX_NON_JPEGREAD
extern	int			SaveSoftImageToJpeg( const char *FilePath, int SIHandle, int Quality, int Sample2x1 ) ;										// ソフトウエアで扱うイメージをＪＰＥＧ画像ファイルとして保存する Quality = 画質、値が大きいほど低圧縮高画質,0～100 
#endif

// 基本イメージデータ構造体関係
extern	int			CreateBaseImage(  const char *FileName, void *FileImage, int FileImageSize, int DataType /*=LOADIMAGE_TYPE_FILE*/ , BASEIMAGE *BaseImage, int ReverseFlag ) ;			// 各種グラフィックデータから基本イメージデータを構築する
extern	int			CreateGraphImage( const char *FileName, void *DataImage, int DataImageSize, int DataImageType, BASEIMAGE *GraphImage, int ReverseFlag ) ;							// CreateBaseImage の旧名称
extern	int			CreateBaseImageToFile( const char *FileName,               BASEIMAGE *BaseImage, int ReverseFlag = FALSE ) ;														// 画像ファイルから基本イメージデータを構築する
extern	int			CreateBaseImageToMem(  void *FileImage, int FileImageSize, BASEIMAGE *BaseImage, int ReverseFlag = FALSE ) ;														// メモリ上に展開された画像ファイルから基本イメージデータを構築する
extern	int			CreateARGB8ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage ) ;																							// ＡＲＧＢ８カラーの空の基本イメージデータを作成する
extern	int			CreateXRGB8ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage ) ;																							// ＸＲＧＢ８カラーの空の基本イメージデータを作成する
extern	int			CreateRGB8ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage ) ;																							// ＲＧＢ８カラーの空の基本イメージデータを作成する
extern	int			CreatePAL8ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage ) ;																							// パレット８ビットカラーの空の基本イメージデータを作成する
extern	int			CreateColorDataBaseImage( int SizeX, int SizeY, COLORDATA *ColorData, BASEIMAGE *BaseImage ) ;																		// 指定のカラーフォーマットの殻の基本イメージデータを作成する

extern	int			ReleaseBaseImage(  BASEIMAGE *BaseImage ) ;																															// 基本イメージデータの後始末を行う
extern	int			ReleaseGraphImage( BASEIMAGE *GraphImage ) ;																														// ReleaseBaseImage の旧名称

extern	int			ConvertNormalFormatBaseImage( BASEIMAGE *BaseImage ) ;																												// DX_BASEIMAGE_FORMAT_NORMAL 以外の形式のイメージデータを DX_BASEIMAGE_FORMAT_NORMAL 形式のイメージに変換する

extern	int			GetDrawScreenBaseImage( int x1, int y1, int x2, int y2, BASEIMAGE *BaseImage ) ;																					// 描画対象の画面から指定領域のグラフィックを基本イメージデータに転送する
extern	int			UpdateLayerdWindowForBaseImage( BASEIMAGE *BaseImage ) ;																											// 基本イメージを使用して UpdateLayerdWindow を行う
extern	int			FillBaseImage( BASEIMAGE *BaseImage, int r, int g, int b, int a ) ;																									// 基本イメージデータを指定の色で塗りつぶす
extern	int			ClearRectBaseImage( BASEIMAGE *BaseImage, int x, int y, int w, int h ) ;																									// 基本イメージデータの指定の領域を０クリアする
extern	int			GetPaletteBaseImage( BASEIMAGE *BaseImage, int PaletteNo, int *r, int *g, int *b, int *a ) ;																		// 基本イメージデータのパレットを取得する
extern	int			SetPaletteBaseImage( BASEIMAGE *BaseImage, int PaletteNo, int  r, int  g, int  b, int  a ) ;																		// 基本イメージデータのパレットをセットする
extern	int			SetPixelPalCodeBaseImage( BASEIMAGE *BaseImage, int x, int y, int palNo ) ;																							// 基本イメージデータの指定の座標の色コードを変更する(パレット画像用)
extern	int			GetPixelPalCodeBaseImage( BASEIMAGE *BaseImage, int x, int y ) ;																									// 基本イメージデータの指定の座標の色コードを取得する(パレット画像用)
extern	int			SetPixelBaseImage( BASEIMAGE *BaseImage, int x, int y, int  r, int  g, int  b, int  a ) ;																			// 基本イメージデータの指定の座標の色を変更する(各色要素は０～２５５)
extern	int			GetPixelBaseImage( BASEIMAGE *BaseImage, int x, int y, int *r, int *g, int *b, int *a ) ;																			// 基本イメージデータの指定の座標の色を取得する(各色要素は０～２５５)
extern	int			DrawLineBaseImage( BASEIMAGE *BaseImage, int x1, int y1, int x2, int y2, int r, int g, int b, int a ) ;																// 基本イメージデータの指定の座標に線を描画する(各色要素は０～２５５)
extern	int			BltBaseImage( int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int DestX, int DestY, BASEIMAGE *SrcBaseImage, BASEIMAGE *DestBaseImage ) ;							// 基本イメージデータを転送する
extern	int			BltBaseImage( int DestX, int DestY, BASEIMAGE *SrcBaseImage, BASEIMAGE *DestBaseImage ) ;																			// 基本イメージデータを転送する
extern	int			BltBaseImageWithTransColor( int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int DestX, int DestY, BASEIMAGE *SrcBaseImage, BASEIMAGE *DestBaseImage, int Tr, int Tg, int Tb, int Ta ) ;	// 基本イメージデータを透過色処理付きで転送する
extern	int			BltBaseImageWithAlphaBlend( int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int DestX, int DestY, BASEIMAGE *SrcBaseImage, BASEIMAGE *DestBaseImage, int Opacity = 255 ) ;	// 基本イメージデータをアルファ値のブレンドを考慮した上で転送する( Opacity は透明度 : 0( 完全透明 ) ～ 255( 完全不透明 ) )( 出力先が ARGB8 形式以外の場合はエラーになります )

extern	int			SaveBaseImageToBmp( const char *FilePath, BASEIMAGE *BaseImage ) ;																										// 基本画像データをＢＭＰ画像として保存する
#ifndef DX_NON_PNGREAD
extern	int			SaveBaseImageToPng( const char *FilePath, BASEIMAGE *BaseImage, int CompressionLevel ) ;																					// 基本画像データをＰＮＧ画像として保存する
#endif
#ifndef DX_NON_JPEGREAD
extern	int			SaveBaseImageToJpeg( const char *FilePath, BASEIMAGE *BaseImage, int Quality, int Sample2x1 ) ;																			// 基本画像データをＪＰＥＧ画像として保存する
#endif

// カラーマッチングしながらグラフィックデータ間転送を行う Ver2
extern int			GraphColorMatchBltVer2( void *DestGraphData, int DestPitch, COLORDATA *DestColorData,
											void *SrcGraphData, int SrcPitch, COLORDATA *SrcColorData,
											void *AlphaMask, int AlphaPitch, COLORDATA *AlphaColorData,
											POINT DestPoint, RECT *SrcRect, int ReverseFlag,
											int TransColorAlphaTestFlag, unsigned int TransColor,
											int ImageShavedMode, int AlphaOnlyFlag = FALSE ,
											int RedIsAlphaFlag = FALSE , int TransColorNoMoveFlag = FALSE ,
											int Pal8ColorMatch = FALSE ) ;


// 色情報取得関係
extern	COLOR_F		GetColorF( float Red, float Green, float Blue, float Alpha ) ;										// 不動小数点型のカラー値を作成する
extern	COLOR_U8	GetColorU8( int Red, int Green, int Blue, int Alpha ) ;												// 符号なし整数８ビットのカラー値を作成する
extern	DWORD		GetColor( int Red, int Green, int Blue ) ;															// ３原色値から現在の画面モードに対応した色データ値を得る
extern	int			GetColor2( int Color, int *Red, int *Green, int *Blue ) ;											// 画面モードに対応した色データ値から個々の３原色データを抜き出す
extern	int			GetColor3( COLORDATA *ColorData, int Red, int Green, int Blue, int Alpha = 255 ) ;					// ３原色値から指定のピクセルフォーマットに対応した色データ値を得る
extern	int			GetColor4( COLORDATA *DestColorData, COLORDATA* SrcColorData, int SrcColor ) ;						// ２つのカラーフォーマット間のデータ変換を行った情報を得る 
extern	int			GetColor5( COLORDATA *ColorData, int Color, int *Red, int *Green, int *Blue, int *Alpha = NULL ) ;	// 指定カラーフォーマットに対応した色データ値から個々の３原色データを抜き出す
extern	int			CreatePaletteColorData( COLORDATA *ColorDataBuf ) ;													// パレットカラーのカラー情報を構築する
extern	int			CreateXRGB8ColorData( COLORDATA *ColorDataBuf ) ;													// ＸＲＧＢ８カラーのカラー情報を構築する
extern	int			CreateARGB8ColorData( COLORDATA *ColorDataBuf ) ;													// ＡＲＧＢ８カラーのカラー情報を構築する
extern	int			CreateFullColorData( COLORDATA *ColorDataBuf ) ;													// フルカラーＤＩＢのカラー情報を構築する
extern	int			CreateGrayColorData( COLORDATA *ColorDataBuf ) ;													// グレースケールのカラー情報を構築する
extern	int			CreatePal8ColorData( COLORDATA *ColorDataBuf ) ;													// パレット２５６色のカラー情報を構築する
extern	int			CreateColorData( COLORDATA *ColorDataBuf, int ColorBitDepth,
									 DWORD RedMask, DWORD GreenMask, DWORD BlueMask, DWORD AlphaMask ) ;				// カラーデータを作成する
extern	void		SetColorDataNoneMask( COLORDATA *ColorData ) ;														// NoneMask 以外の要素を埋めた COLORDATA 構造体の情報を元に NoneMask をセットする
extern	int			CmpColorData( COLORDATA *ColorData1, COLORDATA *ColorData2 ) ;										// 二つのカラーデータが等しいかどうか調べる( TRUE:等しい  FALSE:等しくない )

#endif // DX_NOTUSE_DRAWFUNCTION












// DxSound.cpp関数プロトタイプ宣言

// サウンドデータ管理系関数
extern	int			InitSoundMem( int LogOutFlag = FALSE ) ;																// メモリに読みこんだWAVEデータを削除し、初期化する

extern	int			AddSoundData( int Handle = -1 ) ;																		// 新しいサウンドデータ領域を確保する
extern	int			AddStreamSoundMem( STREAMDATA *Stream, int LoopNum,  int SoundHandle, int StreamDataType, int *CanStreamCloseFlag, int UnionHandle = -1 ) ;	// ストリーム風サウンドデータにサウンドデータを追加する
extern	int			AddStreamSoundMemToMem( void *FileImageBuffer, int ImageSize, int LoopNum,  int SoundHandle, int StreamDataType, int UnionHandle = -1 ) ;	// ストリーム風サウンドデータにサウンドデータを追加する
extern	int			AddStreamSoundMemToFile( const char *WaveFile, int LoopNum,  int SoundHandle, int StreamDataType, int UnionHandle = -1 ) ;	// ストリーム風サウンドデータにサウンドデータを追加する
extern	int			SetupStreamSoundMem( int SoundHandle ) ;																// ストリーム風サウンドデータの再生準備を行う
extern	int			PlayStreamSoundMem( int SoundHandle, int PlayType = DX_PLAYTYPE_LOOP , int TopPositionFlag = TRUE ) ;	// ストリーム風サウンドデータの再生開始
extern	int			CheckStreamSoundMem( int SoundHandle ) ;																// ストリーム風サウンドデータの再生状態を得る
extern	int			StopStreamSoundMem( int SoundHandle ) ;																	// ストリーム風サウンドデータの再生終了
extern	int			SetStreamSoundCurrentPosition( int Byte, int SoundHandle ) ;											// サウンドハンドルの再生位置をバイト単位で変更する(再生が止まっている時のみ有効)
extern	int			GetStreamSoundCurrentPosition( int SoundHandle ) ;														// サウンドハンドルの再生位置をバイト単位で取得する
extern	int			SetStreamSoundCurrentTime( int Time, int SoundHandle ) ;												// サウンドハンドルの再生位置をミリ秒単位で設定する(圧縮形式の場合は正しく設定されない場合がある)
extern	int			GetStreamSoundCurrentTime( int SoundHandle ) ;															// サウンドハンドルの再生位置をミリ秒単位で取得する(圧縮形式の場合は正しい値が返ってこない場合がある)
extern	int			ProcessStreamSoundMem( int SoundHandle ) ;																// ストリームサウンドの再生処理関数
extern	int			ProcessStreamSoundMemAll( void ) ;																		// 有効なストリームサウンドのすべて再生処理関数にかける


extern	int			LoadSoundMem2( const char *WaveName1, const char *WaveName2 ) ;											// 前奏部とループ部に分かれたサウンドデータの作成
extern	int			LoadBGM( const char *WaveName ) ;																		// 主にＢＧＭを読み込むのに適した関数

extern	int			LoadSoundMemBase( const char *WaveName, int BufferNum, int UnionHandle = -1 ) ;								// サウンドデータを追加する
extern	int			LoadSoundMem( const char *WaveName, int BufferNum = 3 , int UnionHandle = -1 ) ;						// サウンドデータを追加する
extern	int			LoadSoundMemToBufNumSitei( const char *WaveName, int BufferNum ) ;										// 同時再生数指定型サウンド追加関数
extern	int			LoadSoundMemByResource( const char *ResourceName, const char *ResourceType, int BufferNum = 1 ) ;		// サウンドをリソースから読み込む

extern	int			LoadSoundMemByMemImageBase( void *FileImageBuffer, int ImageSize, int BufferNum, int UnionHandle = -1 ) ; // メモリ上に展開されたファイルイメージからハンドルを作成する(ベース関数)
extern	int			LoadSoundMemByMemImage( void *FileImageBuffer, int ImageSize, int UnionHandle = -1 ) ;					// メモリ上に展開されたファイルイメージからハンドルを作成する(バッファ数指定あり) 
extern	int			LoadSoundMemByMemImage2( void *UData, int UDataSize, WAVEFORMATEX *UFormat, int UHeaderSize ) ;			// メモリ上に展開されたファイルイメージからハンドルを作成する2
extern	int			LoadSoundMemByMemImageToBufNumSitei( void *FileImageBuffer, int ImageSize, int BufferNum ) ;			// メモリ上に展開されたファイルイメージからハンドルを作成する
extern	int			LoadSoundMem2ByMemImage( void *FileImageBuffer1, int ImageSize1, void *FileImageBuffer2, int ImageSize2 ) ;	// メモリ上に展開されたファイルイメージから前奏部とループ部に分かれたハンドルを作成する

extern	int			DeleteSoundMem( int SoundHandle, int LogOutFlag = FALSE ) ;												// メモリに読み込んだWAVEデータを削除する

extern	int			PlaySoundMem( int SoundHandle, int PlayType, int TopPositionFlag = TRUE ) ;								// メモリに読みこんだWAVEデータを再生する
extern	int			StopSoundMem( int SoundHandle ) ;																		// メモリに読み込んだWAVEデータの再生を止める
extern	int			CheckSoundMem( int SoundHandle ) ;																		// メモリに読みこんだWAVEデータが再生中か調べる
extern	int			SetPanSoundMem( int PanPal, int SoundHandle ) ;															// メモリに読みこんだWAVEデータの再生にパンを設定する
extern	int			SetVolumeSoundMem( int VolumePal, int SoundHandle ) ;													// メモリに読みこんだWAVEデータの再生にボリュームを設定する( 100分の1デシベル単位 )
extern	int			ChangeVolumeSoundMem( int VolumePal, int SoundHandle ) ;												// メモリに読みこんだWAVEデータの再生にボリュームを設定する( パーセント指定 )
extern	int			GetVolumeSoundMem( int SoundHandle ) ;																	// メモリに読みこんだWAVEデータの再生のボリュームを取得する
extern	int			SetFrequencySoundMem( int FrequencyPal, int SoundHandle ) ;												// メモリに読み込んだWAVEデータの再生周波数を設定する
extern	int			GetFrequencySoundMem( int SoundHandle ) ;																// メモリに読み込んだWAVEデータの再生周波数を取得する

extern	int			SetCurrentPositionSoundMem( int SamplePosition, int SoundHandle ) ;										// サウンドハンドルの再生位置をサンプル単位で変更する(再生が止まっている時のみ有効)
extern	int			GetCurrentPositionSoundMem( int SoundHandle ) ;															// サウンドハンドルの再生位置をサンプル単位で取得する
extern	int			SetSoundCurrentPosition( int Byte, int SoundHandle ) ;													// サウンドハンドルの再生位置をバイト単位で変更する(再生が止まっている時のみ有効)
extern	int			GetSoundCurrentPosition( int SoundHandle ) ;															// サウンドハンドルの再生位置をバイト単位で取得する
extern	int			SetSoundCurrentTime( int Time, int SoundHandle ) ;														// サウンドハンドルの再生位置をミリ秒単位で設定する(圧縮形式の場合は正しく設定されない場合がある)
extern	int			GetSoundCurrentTime( int SoundHandle ) ;																// サウンドハンドルの再生位置をミリ秒単位で取得する(圧縮形式の場合は正しい値が返ってこない場合がある)
extern	int			GetSoundTotalSample( int SoundHandle ) ;																// サウンドハンドルの音の総時間を取得する(単位はサンプル)
extern	int			GetSoundTotalTime( int SoundHandle ) ;																	// サウンドハンドルの音の総時間を取得する(単位はミリ秒)

extern	int			SetLoopPosSoundMem( int LoopTime, int SoundHandle ) ;													// サウンドハンドルにループ位置を設定する
extern	int			SetLoopTimePosSoundMem( int LoopTime, int SoundHandle ) ;												// サウンドハンドルにループ位置を設定する
extern	int			SetLoopSamplePosSoundMem( int LoopSamplePosition, int SoundHandle ) ;									// サウンドハンドルにループ位置を設定する

extern	int			SetLoopStartTimePosSoundMem( int LoopStartTime, int SoundHandle ) ;										// サウンドハンドルにループ開始位置を設定する
extern	int			SetLoopStartSamplePosSoundMem( int LoopStartSamplePosition, int SoundHandle ) ;							// サウンドハンドルにループ開始位置を設定する

// 設定関係関数
extern	int			SetCreateSoundDataType( int SoundDataType ) ;															// 作成するサウンドのデータ形式を設定する( DX_SOUNDDATATYPE_MEMNOPRESS 等 )
extern	int			GetCreateSoundDataType( void ) ;																		// 作成するサウンドのデータ形式を取得する( DX_SOUNDDATATYPE_MEMNOPRESS 等 )
extern	int			SetEnableSoundCaptureFlag( int Flag ) ;																	// サウンドキャプチャを前提とした動作をするかどうかを設定する

// 情報取得系関数
extern	void*		GetDSoundObj( void ) ;	/* 戻り値を IDirectSound * にキャストして下さい */							// ＤＸライブラリが使用している DirectSound オブジェクトを取得する

#ifndef DX_NON_BEEP
// BEEP音再生用命令
extern	int			SetBeepFrequency( int Freq ) ;																			// ビープ音周波数設定関数
extern	int			PlayBeep( void ) ;																						// ビープ音を再生する
extern	int			StopBeep( void ) ;																						// ビープ音を止める
#endif

// ラッパー関数
extern	int			PlaySoundFile( const char *FileName, int PlayType ) ;													// WAVEファイルを再生する
extern	int			PlaySound( const char *FileName, int PlayType ) ;														// PlaySoundFile の旧名称
extern	int			CheckSoundFile( void ) ;																				// WAVEファイルの再生中か調べる
extern	int			CheckSound( void ) ;																					// CheckSoundFile の旧名称
extern	int			StopSoundFile( void ) ;																					// WAVEファイルの再生を止める
extern	int			StopSound( void ) ;																						// StopSoundFile の旧名称
extern	int			SetVolumeSoundFile( int VolumePal ) ;																	// WAVEファイルの音量をセットする
extern	int			SetVolumeSound( int VolumePal ) ;																		// SetVolumeSound の旧名称


// ＭＩＤＩ制御関数
extern	int			AddMusicData( void ) ;																					// 新しいＭＩＤＩハンドルを取得する
extern	int			DeleteMusicMem( int MusicHandle ) ;																		// ＭＩＤＩハンドルを削除する
extern	int			LoadMusicMem( const char *FileName ) ;																	// ＭＩＤＩファイルを読み込む
extern	int			LoadMusicMemByMemImage( void *FileImageBuffer, int FileImageSize ) ;									// メモリ上に展開されたＭＩＤＩファイルを読み込む
extern	int			LoadMusicMemByResource( const char *ResourceName, const char *ResourceType ) ;							// リソース上のＭＩＤＩファイルを読み込む
extern	int			PlayMusicMem( int MusicHandle, int PlayType ) ;															// 読み込んだＭＩＤＩデータの演奏を開始する
extern	int			StopMusicMem( int MusicHandle ) ;																		// ＭＩＤＩデータの演奏を停止する
extern	int			CheckMusicMem( int MusicHandle ) ;																		// ＭＩＤＩデータが演奏中かどうかを取得する( TRUE:演奏中  FALSE:停止中 )
extern	int			GetMusicMemPosition( int MusicHandle ) ;																// ＭＩＤＩデータの現在の再生位置を取得する
extern	int			InitMusicMem( void ) ;																					// ＭＩＤＩデータハンドルをすべて削除する
extern	int			ProcessMusicMem( void ) ;																				// ＭＩＤＩデータの周期的処理

extern	int			PlayMusic( const char *FileName, int PlayType ) ;														// ＭＩＤＩファイルを演奏する
extern	int			PlayMusicByMemImage( void *FileImageBuffer, int FileImageSize, int PlayType ) ;							// メモリ上に展開されているＭＩＤＩファイルを演奏する
extern	int			PlayMusicByResource( const char *ResourceName, const char *ResourceType, int PlayType ) ;				// リソースからＭＩＤＩファイルを読み込んで演奏する
extern	int			SetVolumeMusic( int Volume ) ;																			// ＭＩＤＩの再生音量をセットする
extern	int			StopMusic( void ) ;																						// ＭＩＤＩファイルの演奏停止
extern	int			CheckMusic( void ) ;																					// ＭＩＤＩファイルが演奏中か否か情報を取得する
extern	int			GetMusicPosition( void ) ;																				// ＭＩＤＩの現在の再生位置を取得する

extern	int			SelectMidiMode( int Mode ) ;																			// ＭＩＤＩの再生形式をセットする











// DxArchive_.cpp 関数 プロトタイプ宣言
extern	int			DXArchivePreLoad( const char *FilePath , int ASync = FALSE ) ;				// 指定のＤＸＡファイルを丸ごとメモリに読み込む( 戻り値: -1=エラー  0=成功 )
extern	int			DXArchiveCheckIdle( const char *FilePath ) ;								// 指定のＤＸＡファイルの事前読み込みが完了したかどうかを取得する( 戻り値： TRUE=完了した FALSE=まだ )
extern	int			DXArchiveRelease( const char *FilePath ) ;									// 指定のＤＸＡファイルをメモリから解放する
extern	int			DXArchiveCheckFile( const char *FilePath, const char *TargetFilePath ) ;	// ＤＸＡファイルの中に指定のファイルが存在するかどうかを調べる、TargetFilePath はＤＸＡファイルをカレントフォルダとした場合のパス( 戻り値:  -1=エラー  0:無い  1:ある )





















// DxModel.cpp 関数 プロトタイプ宣言

#ifndef DX_NON_MODEL

// モデルの読み込み・複製関係
extern	int			MV1LoadModel( const char *FileName ) ;														// モデルの読み込み( -1:エラー  0以上:モデルハンドル )
extern	int			MV1DuplicateModel( int SrcMHandle ) ;														// 指定のモデルと同じモデル基本データを使用してモデルを作成する( -1:エラー  0以上:モデルハンドル )

extern	int			MV1DeleteModel( int MHandle ) ;																// モデルを削除する

extern	int			MV1SetLoadModelReMakeNormal( int Flag ) ;													// モデルを読み込む際に法線の再計算を行うかどうかを設定する( TRUE:行う  FALSE:行わない )
extern	int			MV1SetLoadModelReMakeNormalSmoothingAngle( float SmoothingAngle = 1.562069f ) ;				// モデルを読み込む際に行う法泉の再計算で使用するスムージング角度を設定する( 単位はラジアン )
extern	int			MV1SetLoadModelPositionOptimize( int Flag ) ;												// モデルを読み込む際に座標データの最適化を行うかどうかを設定する( TRUE:行う  FALSE:行わない )

// モデル描画関係
extern	int			MV1DrawModel( int MHandle ) ;																// モデルを描画する
extern	int			MV1DrawFrame( int MHandle, int FrameIndex ) ;												// モデルの指定のフレームを描画する
extern	int			MV1DrawMesh( int MHandle, int MeshIndex ) ;													// モデルの指定のメッシュを描画する
extern	int			MV1DrawModelDebug( int MHandle, int Color, int IsNormalLine, float NormalLineLength, int IsPolyLine, int IsCollisionBox ) ;	// モデルのデバッグ描画

// モデル基本制御関係
extern	MATRIX		MV1GetLocalWorldMatrix( int MHandle ) ;														// モデルのローカル座標からワールド座標に変換する行列を得る
extern	int			MV1SetPosition( int MHandle, VECTOR Position ) ;											// モデルの座標をセット
extern	VECTOR		MV1GetPosition( int MHandle ) ;																// モデルの座標を取得
extern	int			MV1SetScale( int MHandle, VECTOR Scale ) ;													// モデルの拡大値をセット
extern	VECTOR		MV1GetScale( int MHandle ) ;																// モデルの拡大値を取得
extern	int			MV1SetRotationXYZ( int MHandle, VECTOR Rotate ) ;											// モデルの回転値をセット( X軸回転→Y軸回転→Z軸回転方式 )
extern	VECTOR		MV1GetRotationXYZ( int MHandle ) ;															// モデルの回転値を取得( X軸回転→Y軸回転→Z軸回転方式 )
extern	int			MV1SetRotationZYAxis( int MHandle, VECTOR ZAxisDirection, VECTOR YAxisDirection, float ZAxisTwistRotate ) ;	// モデルのＺ軸とＹ軸の向きをセットする
extern	int			MV1SetRotationMatrix( int MHandle, MATRIX Matrix ) ;										// モデルの回転用行列をセットする
extern	MATRIX		MV1GetRotationMatrix( int MHandle ) ;														// モデルの回転用行列を取得する
extern	int			MV1SetMatrix( int MHandle, MATRIX Matrix ) ;												// モデルの変形用行列をセットする
extern	MATRIX		MV1GetMatrix( int MHandle ) ;																// モデルの変形用行列を取得する
extern	int			MV1SetVisible( int MHandle, int VisibleFlag ) ;												// モデルの表示、非表示状態を変更する( TRUE:表示  FALSE:非表示 )
extern	int			MV1GetVisible( int MHandle ) ;																// モデルの表示、非表示状態を取得する( TRUE:表示  FALSE:非表示 )
extern	int			MV1SetDifColorScale( int MHandle, COLOR_F Scale ) ;											// モデルのディフューズカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetDifColorScale( int MHandle ) ;														// モデルのディフューズカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	int			MV1SetSpcColorScale( int MHandle, COLOR_F Scale ) ;											// モデルのスペキュラカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetSpcColorScale( int MHandle ) ;														// モデルのスペキュラカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	int			MV1SetEmiColorScale( int MHandle, COLOR_F Scale ) ;											// モデルのエミッシブカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetEmiColorScale( int MHandle ) ;														// モデルのエミッシブカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	int			MV1SetAmbColorScale( int MHandle, COLOR_F Scale ) ;											// モデルのアンビエントカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetAmbColorScale( int MHandle ) ;														// モデルのアンビエントカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	int			MV1GetSemiTransState( int MHandle ) ;														// モデルに半透明要素があるかどうかを取得する( 戻り値 TRUE:ある  FALSE:ない )
extern	int			MV1SetOpacityRate( int MHandle, float Rate ) ;												// モデルの不透明度を設定する( 不透明 1.0f ～ 透明 0.0f )
extern	float		MV1GetOpacityRate( int MHandle ) ;															// モデルの不透明度を取得する( 不透明 1.0f ～ 透明 0.0f )
extern	int			MV1SetUseZBuffer( int MHandle, int Flag ) ;													// モデルを描画する際にＺバッファを使用するかどうかを設定する
extern	int			MV1SetWriteZBuffer( int MHandle, int Flag ) ;												// モデルを描画する際にＺバッファに書き込みを行うかどうかを設定する
extern	int			MV1SetZBufferCmpType( int MHandle, int CmpType /* DX_CMP_NEVER 等 */ ) ;					// モデルの描画時のＺ値の比較モードを設定する
extern	int			MV1SetZBias( int MHandle, int Bias ) ;														// モデルの描画時の書き込むＺ値のバイアスを設定する
extern	int			MV1SetUseVertDifColor( int MHandle, int UseFlag ) ;											// モデルの含まれるメッシュの頂点ディフューズカラーをマテリアルのディフューズカラーの代わりに使用するかどうかを設定する( TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
extern	int			MV1SetUseVertSpcColor( int MHandle, int UseFlag ) ;											// モデルに含まれるメッシュの頂点スペキュラカラーをマテリアルのスペキュラカラーの代わりに使用するかどうかを設定する( TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
extern	int			MV1SetSampleFilterMode( int MHandle, int FilterMode ) ;										// モデルのテクスチャのサンプルフィルターモードを変更する( FilterMode は DX_DRAWMODE_NEAREST 等 )
extern	int			MV1SetMaxAnisotropy( int MHandle, int MaxAnisotropy ) ;										// モデルの異方性フィルタリングの最大次数を設定する

// アニメーション関係
extern	int			MV1AttachAnim( int MHandle, int AnimIndex, int AnimSrcMHandle = -1 , int NameCheck = TRUE ) ;	// アニメーションをアタッチする( 戻り値  -1:エラー  0以上:アタッチインデックス )
extern	int			MV1DetachAnim( int MHandle, int AttachIndex ) ;												// アニメーションをデタッチする
extern	int			MV1SetAttachAnimTime( int MHandle, int AttachIndex, float Time ) ;							// アタッチしているアニメーションの再生時間を設定する
extern	float		MV1GetAttachAnimTime( int MHandle, int AttachIndex ) ;										// アタッチしているアニメーションの再生時間を取得する
extern	float		MV1GetAttachAnimTotalTime( int MHandle, int AttachIndex ) ;									// アタッチしているアニメーションの総時間を得る
extern	int			MV1SetAttachAnimBlendRate( int MHandle, int AttachIndex, float Rate = 1.0f ) ;				// アタッチしているアニメーションのブレンド率を設定する
extern	float		MV1GetAttachAnimBlendRate( int MHandle, int AttachIndex ) ;									// アタッチしているアニメーションのブレンド率を取得する
extern	int			MV1SetAttachAnimBlendRateToFrame( int MHandle, int AttachIndex, int FrameIndex, float Rate, int SetChild = TRUE ) ;	// アタッチしているアニメーションのブレンド率を設定する( フレーム単位 )
extern	float		MV1GetAttachAnimBlendRateToFrame( int MHandle, int AttachIndex, int FrameIndex ) ;			// アタッチしているアニメーションのブレンド率を設定する( フレーム単位 )
extern	int			MV1GetAttachAnim( int MHandle, int AttachIndex ) ;											// アタッチしているアニメーションのアニメーションインデックスを取得する

extern	int			MV1GetAnimNum( int MHandle ) ;																// アニメーションの数を取得する
extern	const char *MV1GetAnimName( int MHandle, int AnimIndex ) ;												// 指定番号のアニメーション名を取得する( NULL:エラー )
extern	int			MV1GetAnimIndex( int MHandle, const char *AnimName ) ;										// 指定名のアニメーション番号を取得する( -1:エラー )
extern	float		MV1GetAnimTotalTime( int MHandle, int AnimIndex ) ;											// 指定番号のアニメーションの総時間を得る
extern	int			MV1GetAnimTargetFrameNum( int MHandle, int AnimIndex ) ;									// 指定のアニメーションがターゲットとするフレームの数を取得する
extern	const char *MV1GetAnimTargetFrameName( int MHandle, int AnimIndex, int AnimFrameIndex ) ;				// 指定のアニメーションがターゲットとするフレームの名前を取得する
extern	int			MV1GetAnimTargetFrame( int MHandle, int AnimIndex, int AnimFrameIndex ) ;					// 指定のアニメーションがターゲットとするフレームの番号を取得する
extern	int			MV1GetAnimTargetFrameKeySetNum( int MHandle, int AnimIndex, int AnimFrameIndex ) ;			// 指定のアニメーションがターゲットとするフレーム用のアニメーションキーセットの数を取得する
extern	int			MV1GetAnimTargetFrameKeySet( int MHandle, int AnimIndex, int AnimFrameIndex, int Index ) ;	// 指定のアニメーションがターゲットとするフレーム用のアニメーションキーセットキーセットインデックスを取得する

extern	int			MV1GetAnimKeySetNum( int MHandle ) ;														// モデルに含まれるアニメーションキーセットの総数を得る
extern	int			MV1GetAnimKeySetType( int MHandle, int AnimKeySetIndex ) ;									// 指定のアニメーションキーセットのタイプを取得する( MV1_ANIMKEY_TYPE_QUATERNION 等 )
extern	int			MV1GetAnimKeySetDataType( int MHandle, int AnimKeySetIndex ) ;								// 指定のアニメーションキーセットのデータタイプを取得する( MV1_ANIMKEY_DATATYPE_ROTATE 等 )
extern	int			MV1GetAnimKeySetTimeType( int MHandle, int AnimKeySetIndex ) ;								// 指定のアニメーションキーセットのキーの時間データタイプを取得する( MV1_ANIMKEY_TIME_TYPE_ONE 等 )
extern	int			MV1GetAnimKeySetDataNum( int MHandle, int AnimKeySetIndex ) ;								// 指定のアニメーションキーセットのキーの数を取得する
extern	float		MV1GetAnimKeyDataTime( int MHandle, int AnimKeySetIndex, int Index ) ;						// 指定のアニメーションキーセットのキーの時間を取得する
extern	FLOAT4		MV1GetAnimKeyDataToQuaternion( int MHandle, int AnimKeySetIndex, int Index ) ;				// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_QUATERNION では無かった場合は失敗する
extern	VECTOR		MV1GetAnimKeyDataToVector( int MHandle, int AnimKeySetIndex, int Index ) ;					// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_VECTOR では無かった場合は失敗する
extern	MATRIX		MV1GetAnimKeyDataToMatrix( int MHandle, int AnimKeySetIndex, int Index ) ;					// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_MATRIX4X4C か MV1_ANIMKEY_TYPE_MATRIX3X3 では無かった場合は失敗する
extern	float		MV1GetAnimKeyDataToFlat( int MHandle, int AnimKeySetIndex, int Index ) ;					// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_FLAT では無かった場合は失敗する
extern	float		MV1GetAnimKeyDataToLinear( int MHandle, int AnimKeySetIndex, int Index ) ;					// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_LINEAR では無かった場合は失敗する

// マテリアル関係
extern	int			MV1GetMaterialNum( int MHandle ) ;															// モデルで使用しているマテリアルの数を取得する
extern	const char *MV1GetMaterialName( int MHandle, int MaterialIndex ) ;										// 指定のマテリアルの名前を取得する
extern	int			MV1SetMaterialType( int MHandle, int MaterialIndex, int Type ) ;							// 指定のマテリアルのタイプを変更する( Type : DX_MATERIAL_TYPE_NORMAL など )
extern	int			MV1GetMaterialType( int MHandle, int MaterialIndex ) ;										// 指定のマテリアルのタイプを取得する( 戻り値 : DX_MATERIAL_TYPE_NORMAL など )
extern	int			MV1SetMaterialDifColor( int MHandle, int MaterialIndex, COLOR_F Color ) ;					// 指定のマテリアルのディフューズカラーを設定する
extern	COLOR_F		MV1GetMaterialDifColor( int MHandle, int MaterialIndex ) ;									// 指定のマテリアルのディフューズカラーを取得する
extern	int			MV1SetMaterialSpcColor( int MHandle, int MaterialIndex, COLOR_F Color ) ;					// 指定のマテリアルのスペキュラカラーを設定する
extern	COLOR_F		MV1GetMaterialSpcColor( int MHandle, int MaterialIndex ) ;									// 指定のマテリアルのスペキュラカラーを取得する
extern	int			MV1SetMaterialEmiColor( int MHandle, int MaterialIndex, COLOR_F Color ) ;					// 指定のマテリアルのエミッシブカラーを設定する
extern	COLOR_F		MV1GetMaterialEmiColor( int MHandle, int MaterialIndex ) ;									// 指定のマテリアルのエミッシブカラーを取得する
extern	int			MV1SetMaterialAmbColor( int MHandle, int MaterialIndex, COLOR_F Color ) ;					// 指定のマテリアルのアンビエントカラーを設定する
extern	COLOR_F		MV1GetMaterialAmbColor( int MHandle, int MaterialIndex ) ;									// 指定のマテリアルのアンビエントカラーを取得する
extern	int			MV1SetMaterialSpcPower( int MHandle, int MaterialIndex, float Power ) ;						// 指定のマテリアルのスペキュラの強さを設定する
extern	float		MV1GetMaterialSpcPower( int MHandle, int MaterialIndex ) ;									// 指定のマテリアルのスペキュラの強さを取得する
extern	int			MV1SetMaterialDifMapTexture( int MHandle, int MaterialIndex, int TexIndex ) ;				// 指定のマテリアルでディフューズマップとして使用するテクスチャを指定する
extern	int			MV1GetMaterialDifMapTexture( int MHandle, int MaterialIndex ) ;								// 指定のマテリアルでディフューズマップとして使用されているテクスチャのインデックスを取得する
extern	int			MV1SetMaterialSpcMapTexture( int MHandle, int MaterialIndex, int TexIndex ) ;				// 指定のマテリアルでスペキュラマップとして使用するテクスチャを指定する
extern	int			MV1GetMaterialSpcMapTexture( int MHandle, int MaterialIndex ) ;								// 指定のマテリアルでスペキュラマップとして使用されているテクスチャのインデックスを取得する
extern	int			MV1GetMaterialNormalMapTexture( int MHandle, int MaterialIndex ) ;							// 指定のマテリアルで法線マップとして使用されているテクスチャのインデックスを取得する
extern	int			MV1SetMaterialDifGradTexture( int MHandle, int MaterialIndex, int TexIndex ) ;				// 指定のマテリアルでトゥーンレンダリングのディフューズグラデーションマップとして使用するテクスチャを設定する
extern	int			MV1GetMaterialDifGradTexture( int MHandle, int MaterialIndex ) ;							// 指定のマテリアルでトゥーンレンダリングのディフューズグラデーションマップとして使用するテクスチャを取得する
extern	int			MV1SetMaterialSpcGradTexture( int MHandle, int MaterialIndex, int TexIndex ) ;				// 指定のマテリアルでトゥーンレンダリングのスペキュラグラデーションマップとして使用するテクスチャを設定する
extern	int			MV1GetMaterialSpcGradTexture( int MHandle, int MaterialIndex ) ;							// 指定のマテリアルでトゥーンレンダリングのスペキュラグラデーションマップとして使用するテクスチャを取得する
extern	int			MV1SetMaterialDifGradBlendType( int MHandle, int MaterialIndex, int BlendType ) ;			// 指定のマテリアルのトゥーンレンダリングで使用するディフューズグラデーションマップとディフューズカラーの合成方法を設定する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
extern	int			MV1GetMaterialDifGradBlendType( int MHandle, int MaterialIndex ) ;							// 指定のマテリアルのトゥーンレンダリングで使用するディフューズグラデーションマップとディフューズカラーの合成方法を取得する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
extern	int			MV1SetMaterialSpcGradBlendType( int MHandle, int MaterialIndex, int BlendType ) ;			// 指定のマテリアルのトゥーンレンダリングで使用するスペキュラグラデーションマップとスペキュラカラーの合成方法を設定する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
extern	int			MV1GetMaterialSpcGradBlendType( int MHandle, int MaterialIndex ) ;							// 指定のマテリアルのトゥーンレンダリングで使用するスペキュラグラデーションマップとスペキュラカラーの合成方法を取得する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
extern	int			MV1SetMaterialOutLineWidth( int MHandle, int MaterialIndex, float Width ) ;					// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線の太さを設定する( 0.0f ～ 1.0f )
extern	float		MV1GetMaterialOutLineWidth( int MHandle, int MaterialIndex ) ;								// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線の太さを取得する( 0.0f ～ 1.0f )
extern	int			MV1SetMaterialOutLineColor( int MHandle, int MaterialIndex, COLOR_F Color ) ;				// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線の色を設定する
extern	COLOR_F		MV1GetMaterialOutLineColor( int MHandle, int MaterialIndex ) ;								// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線の色を取得する
extern	int			MV1SetMaterialDrawBlendMode( int MHandle, int MaterialIndex, int BlendMode ) ;				// 指定のマテリアルの描画ブレンドモードを設定する( DX_BLENDMODE_ALPHA 等 )
extern	int			MV1GetMaterialDrawBlendMode( int MHandle, int MaterialIndex ) ;								// 指定のマテリアルの描画ブレンドモードを取得する( DX_BLENDMODE_ALPHA 等 )
extern	int			MV1SetMaterialDrawBlendParam( int MHandle, int MaterialIndex, int BlendParam ) ;			// 指定のマテリアルの描画ブレンドパラメータを設定する
extern	int			MV1GetMaterialDrawBlendParam( int MHandle, int MaterialIndex ) ;							// 指定のマテリアルの描画ブレンドパラメータを設定する

// テクスチャ関係
extern	int			MV1GetTextureNum( int MHandle ) ;															// テクスチャの数を取得
extern	const char *MV1GetTextureName( int MHandle, int TexIndex ) ;											// テクスチャの名前を取得
extern	int			MV1SetTextureColorFilePath( int MHandle, int TexIndex, const char *FilePath ) ;				// カラーテクスチャのファイルパスを変更する
extern	const char *MV1GetTextureColorFilePath( int MHandle, int TexIndex ) ;									// カラーテクスチャのファイルパスを取得
extern	int			MV1SetTextureAlphaFilePath( int MHandle, int TexIndex, const char *FilePath ) ;				// アルファテクスチャのファイルパスを変更する
extern	const char *MV1GetTextureAlphaFilePath( int MHandle, int TexIndex ) ;									// アルファテクスチャのファイルパスを取得
extern	int			MV1SetTextureGraphHandle( int MHandle, int TexIndex, int GrHandle, int SemiTransFlag ) ;	// テクスチャで使用するグラフィックハンドルを変更する( GrHandle を -1 にすると解除 )
extern	int			MV1GetTextureGraphHandle( int MHandle, int TexIndex ) ;										// テクスチャのグラフィックハンドルを取得する
extern	int			MV1SetTextureAddressMode( int MHandle, int TexIndex, int AddrUMode, int AddrVMode ) ;		// テクスチャのアドレスモードを設定する( AddUMode の値は DX_TEXADDRESS_WRAP 等 )
extern	int			MV1GetTextureAddressModeU( int MHandle, int TexIndex ) ;									// テクスチャのＵ値のアドレスモードを取得する( 戻り値:DX_TEXADDRESS_WRAP 等 )
extern	int			MV1GetTextureAddressModeV( int MHandle, int TexIndex ) ;									// テクスチャのＶ値のアドレスモードを取得する( 戻り値:DX_TEXADDRESS_WRAP 等 )
extern	int			MV1GetTextureWidth( int MHandle, int TexIndex ) ;											// テクスチャの幅を取得する
extern	int			MV1GetTextureHeight( int MHandle, int TexIndex ) ;											// テクスチャの高さを取得する
extern	int			MV1GetTextureSemiTransState( int MHandle, int TexIndex ) ;									// テクスチャに半透明要素があるかどうかを取得する( 戻り値  TRUE:ある  FALSE:ない )
extern	int			MV1SetTextureBumpImageFlag( int MHandle, int TexIndex, int Flag ) ;							// テクスチャで使用している画像がバンプマップかどうかを設定する
extern	int			MV1GetTextureBumpImageFlag( int MHandle, int TexIndex ) ;									// テクスチャがバンプマップかどうかを取得する( 戻り値  TRUE:バンプマップ  FALSE:違う )
extern	int			MV1SetTextureBumpImageNextPixelLength( int MHandle, int TexIndex, float Length ) ;			// バンプマップ画像の場合の隣のピクセルとの距離を設定する
extern	float		MV1GetTextureBumpImageNextPixelLength( int MHandle, int TexIndex ) ;						// バンプマップ画像の場合の隣のピクセルとの距離を取得する
extern	int			MV1SetTextureSampleFilterMode( int MHandle, int TexIndex, int FilterMode ) ;				// テクスチャのフィルタリングモードを設定する
extern	int			MV1GetTextureSampleFilterMode( int MHandle, int TexIndex ) ;								// テクスチャのフィルタリングモードを取得する( 戻り値  DX_DRAWMODE_BILINEAR等 )

// フレーム関係
extern	int			MV1GetFrameNum( int MHandle ) ;																// フレームの数を取得する
extern	int			MV1SearchFrame( int MHandle, const char *FrameName ) ;										// フレームの名前からモデル中のフレームのフレームインデックスを取得する( 無かった場合は戻り値が-1 )
extern	int			MV1SearchFrameChild( int MHandle, int FrameIndex = -1 , const char *ChildName = NULL ) ;	// フレームの名前から指定のフレームの子フレームのフレームインデックスを取得する( 名前指定版 )( FrameIndex を -1 にすると親を持たないフレームを ChildIndex で指定する )( 無かった場合は戻り値が-1 )
extern	const char *MV1GetFrameName( int MHandle, int FrameIndex ) ;											// 指定のフレームの名前を取得する( エラーの場合は戻り値が NULL )
extern	int			MV1GetFrameParent( int MHandle, int FrameIndex ) ;											// 指定のフレームの親フレームのインデックスを得る( 親がいない場合は -2 が返る )
extern	int			MV1GetFrameChildNum( int MHandle, int FrameIndex = -1 ) ;									// 指定のフレームの子フレームの数を取得する( FrameIndex を -1 にすると親を持たないフレームの数が返ってくる )
extern	int			MV1GetFrameChild( int MHandle, int FrameIndex = -1 , int ChildIndex = 0 ) ;					// 指定のフレームの子フレームのフレームインデックスを取得する( 番号指定版 )( FrameIndex を -1 にすると親を持たないフレームを ChildIndex で指定する )( エラーの場合は戻り値が-1 )
extern	VECTOR		MV1GetFramePosition( int MHandle, int FrameIndex ) ;										// 指定のフレームの座標を取得する
extern	MATRIX		MV1GetFrameBaseLocalMatrix( int MHandle, int FrameIndex ) ;									// 指定のフレームの初期状態での座標変換行列を取得する
extern	MATRIX		MV1GetFrameLocalMatrix( int MHandle, int FrameIndex ) ;										// 指定のフレームの座標変換行列を取得する
extern	MATRIX		MV1GetFrameLocalWorldMatrix( int MHandle, int FrameIndex ) ;								// 指定のフレームのローカル座標からワールド座標に変換する行列を得る
extern	int			MV1SetFrameUserLocalMatrix( int MHandle, int FrameIndex, MATRIX Matrix ) ;					// 指定のフレームの座標変換行列を設定する
extern	int			MV1ResetFrameUserLocalMatrix( int MHandle, int FrameIndex ) ;								// 指定のフレームの座標変換行列をデフォルトに戻す
extern	VECTOR		MV1GetFrameMaxVertexLocalPosition( int MHandle, int FrameIndex ) ;							// 指定のフレームが持つメッシュ頂点のローカル座標での最大値を得る
extern	VECTOR		MV1GetFrameMinVertexLocalPosition( int MHandle, int FrameIndex ) ;							// 指定のフレームが持つメッシュ頂点のローカル座標での最小値を得る
extern	VECTOR		MV1GetFrameAvgVertexLocalPosition( int MHandle, int FrameIndex ) ;							// 指定のフレームが持つメッシュ頂点のローカル座標での平均値を得る
extern	int			MV1GetFrameTriangleNum( int MHandle, int FrameIndex ) ;										// 指定のフレームに含まれるポリゴンの数を取得する
extern	int			MV1GetFrameMeshNum( int MHandle, int FrameIndex ) ;											// 指定のフレームが持つメッシュの数を取得する
extern	int			MV1GetFrameMesh( int MHandle, int FrameIndex, int Index ) ;									// 指定のフレームが持つメッシュのメッシュインデックスを取得する
extern	int			MV1SetFrameVisible( int MHandle, int FrameIndex, int VisibleFlag ) ;						// 指定のフレームの表示、非表示状態を変更する( TRUE:表示  FALSE:非表示 )
extern	int			MV1GetFrameVisible( int MHandle, int FrameIndex ) ;											// 指定のフレームの表示、非表示状態を取得する( TRUE:表示  FALSE:非表示 )
extern	int			MV1SetFrameDifColorScale( int MHandle, int FrameIndex, COLOR_F Scale ) ;					// 指定のフレームのディフューズカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	int			MV1SetFrameSpcColorScale( int MHandle, int FrameIndex, COLOR_F Scale ) ;					// 指定のフレームのスペキュラカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	int			MV1SetFrameEmiColorScale( int MHandle, int FrameIndex, COLOR_F Scale ) ;					// 指定のフレームのエミッシブカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	int			MV1SetFrameAmbColorScale( int MHandle, int FrameIndex, COLOR_F Scale ) ;					// 指定のフレームのアンビエントカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetFrameDifColorScale( int MHandle, int FrameIndex ) ;									// 指定のフレームのディフューズカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetFrameSpcColorScale( int MHandle, int FrameIndex ) ;									// 指定のフレームのスペキュラカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetFrameEmiColorScale( int MHandle, int FrameIndex ) ;									// 指定のフレームのエミッシブカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetFrameAmbColorScale( int MHandle, int FrameIndex ) ;									// 指定のフレームのアンビエントカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	int			MV1GetFrameSemiTransState( int MHandle, int FrameIndex ) ;									// 指定のフレームに半透明要素があるかどうかを取得する( 戻り値 TRUE:ある  FALSE:ない )
extern	int			MV1SetFrameOpacityRate( int MHandle, int FrameIndex, float Rate ) ;							// 指定のフレームの不透明度を設定する( 不透明 1.0f ～ 透明 0.0f )
extern	float		MV1GetFrameOpacityRate( int MHandle, int FrameIndex ) ;										// 指定のフレームの不透明度を取得する( 不透明 1.0f ～ 透明 0.0f )
extern	int			MV1SetFrameBaseVisible( int MHandle, int FrameIndex, int VisibleFlag ) ;					// 指定のフレームの初期表示状態を設定する( TRUE:表示  FALSE:非表示 )
extern	int			MV1GetFrameBaseVisible( int MHandle, int FrameIndex ) ;										// 指定のフレームの初期表示状態を取得する( TRUE:表示  FALSE:非表示 )
extern	int			MV1SetFrameTextureAddressTransform( int MHandle, int FrameIndex, float TransU, float TransV, float ScaleU, float ScaleV, float RotCenterU, float RotCenterV, float Rotate ) ;	// 指定のフレームのテクスチャ座標変換パラメータを設定する
extern	int			MV1SetFrameTextureAddressTransformMatrix( int MHandle, int FrameIndex, MATRIX Matrix ) ;			// 指定のフレームのテクスチャ座標変換行列をセットする
extern	int			MV1ResetFrameTextureAddressTransform( int MHandle, int FrameIndex ) ;							// 指定のフレームのテクスチャ座標変換パラメータをリセットする

// メッシュ関係
extern	int			MV1GetMeshNum( int MHandle ) ;																// モデルに含まれるメッシュの数を取得する
extern	int			MV1GetMeshMaterial( int MHandle, int MeshIndex ) ;											// 指定メッシュが使用しているマテリアルのインデックスを取得する
extern	int			MV1GetMeshTriangleNum( int MHandle, int MeshIndex ) ;										// 指定メッシュに含まれる三角形ポリゴンの数を取得する
extern	int			MV1SetMeshVisible( int MHandle, int MeshIndex, int VisibleFlag ) ;							// 指定メッシュの表示、非表示状態を変更する( TRUE:表示  FALSE:非表示 )
extern	int			MV1GetMeshVisible( int MHandle, int MeshIndex ) ;											// 指定メッシュの表示、非表示状態を取得する( TRUE:表示  FALSE:非表示 )
extern	int			MV1SetMeshDifColorScale( int MHandle, int MeshIndex, COLOR_F Scale ) ;						// 指定のメッシュのディフューズカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	int			MV1SetMeshSpcColorScale( int MHandle, int MeshIndex, COLOR_F Scale ) ;						// 指定のメッシュのスペキュラカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	int			MV1SetMeshEmiColorScale( int MHandle, int MeshIndex, COLOR_F Scale ) ;						// 指定のメッシュのエミッシブカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	int			MV1SetMeshAmbColorScale( int MHandle, int MeshIndex, COLOR_F Scale ) ;						// 指定のメッシュのアンビエントカラーのスケール値を設定する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetMeshDifColorScale( int MHandle, int MeshIndex ) ;										// 指定のメッシュのディフューズカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetMeshSpcColorScale( int MHandle, int MeshIndex ) ;										// 指定のメッシュのスペキュラカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetMeshEmiColorScale( int MHandle, int MeshIndex ) ;										// 指定のメッシュのエミッシブカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	COLOR_F		MV1GetMeshAmbColorScale( int MHandle, int MeshIndex ) ;										// 指定のメッシュのアンビエントカラーのスケール値を取得する( デフォルト値は 1.0f )
extern	int			MV1SetMeshOpacityRate( int MHandle, int MeshIndex, float Rate ) ;							// 指定のメッシュの不透明度を設定する( 不透明 1.0f ～ 透明 0.0f )
extern	float		MV1GetMeshOpacityRate( int MHandle, int MeshIndex ) ;										// 指定のメッシュの不透明度を取得する( 不透明 1.0f ～ 透明 0.0f )
extern	int			MV1SetMeshDrawBlendMode( int MHandle, int MeshIndex, int BlendMode ) ;						// 指定のメッシュの描画ブレンドモードを設定する( DX_BLENDMODE_ALPHA 等 )
extern	int			MV1SetMeshDrawBlendParam( int MHandle, int MeshIndex, int BlendParam ) ;					// 指定のメッシュの描画ブレンドパラメータを設定する
extern	int			MV1GetMeshDrawBlendMode( int MHandle, int MeshIndex ) ;										// 指定のメッシュの描画ブレンドモードを取得する( DX_BLENDMODE_ALPHA 等 )
extern	int			MV1GetMeshDrawBlendParam( int MHandle, int MeshIndex ) ;									// 指定のメッシュの描画ブレンドパラメータを設定する
extern	int			MV1SetMeshBaseVisible( int MHandle, int MeshIndex, int VisibleFlag ) ;						// 指定のメッシュの初期表示状態を設定する( TRUE:表示  FALSE:非表示 )
extern	int			MV1GetMeshBaseVisible( int MHandle, int MeshIndex ) ;										// 指定のメッシュの初期表示状態を取得する( TRUE:表示  FALSE:非表示 )
extern	int			MV1SetMeshBackCulling( int MHandle, int MeshIndex, int CullingFlag ) ;						// 指定のメッシュのバックカリングを行うかどうかを設定する( TRUE:する  FALSE:しない )
extern	int			MV1GetMeshBackCulling( int MHandle, int MeshIndex ) ;										// 指定のメッシュのバックカリングを行うかどうかを取得する( TRUE:する  FALSE:しない )
extern	VECTOR		MV1GetMeshMaxPosition( int MHandle, int MeshIndex ) ;										// 指定のメッシュに含まれるポリゴンの最大ローカル座標を取得する
extern	VECTOR		MV1GetMeshMinPosition( int MHandle, int MeshIndex ) ;										// 指定のメッシュに含まれるポリゴンの最小ローカル座標を取得する
extern	int			MV1GetMeshTListNum( int MHandle, int MeshIndex ) ;											// 指定のメッシュに含まれるトライアングルリストの数を取得する
extern	int			MV1GetMeshTList( int MHandle, int MeshIndex, int Index ) ;									// 指定のメッシュに含まれるトライアングルリストのインデックスを取得する
extern	int			MV1GetMeshSemiTransState( int MHandle, int MeshIndex ) ;									// 指定のメッシュに半透明要素があるかどうかを取得する( 戻り値 TRUE:ある  FALSE:ない )
extern	int			MV1SetMeshUseVertDifColor( int MHandle, int MeshIndex, int UseFlag ) ;						// 指定のメッシュの頂点ディフューズカラーをマテリアルのディフューズカラーの代わりに使用するかどうかを設定する( TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
extern	int			MV1SetMeshUseVertSpcColor( int MHandle, int MeshIndex, int UseFlag ) ;						// 指定のメッシュの頂点スペキュラカラーをマテリアルのスペキュラカラーの代わりに使用するかどうかを設定する( TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
extern	int			MV1GetMeshUseVertDifColor( int MHandle, int MeshIndex ) ;									// 指定のメッシュの頂点ディフューズカラーをマテリアルのディフューズカラーの代わりに使用するかどうかの設定を取得する( 戻り値  TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
extern	int			MV1GetMeshUseVertSpcColor( int MHandle, int MeshIndex ) ;									// 指定のメッシュの頂点スペキュラカラーをマテリアルのスペキュラカラーの代わりに使用するかどうかの設定を取得する( 戻り値  TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )

// トライアングルリスト関係
extern	int			MV1GetTriangleListNum( int MHandle ) ;														// トライアングルリストの数を取得する
extern	int			MV1GetTriangleListVertexType( int MHandle, int TListIndex ) ;								// 指定のトライアングルリストの頂点データタイプを取得する( MV1_VERTEX_TYPE_NORMAL 等 )
extern	int			MV1GetTriangleListPolygonNum( int MHandle, int TListIndex ) ;								// 指定のトライアングルリストに含まれるポリゴンの数を取得する
extern	int			MV1GetTriangleListVertexNum( int MHandle, int TListIndex ) ;								// 指定のトライアングルリストに含まれる頂点データの数を取得する

// コリジョン関係
extern	int			MV1SetupCollInfo( int MHandle, int FrameIndex = -1 , int XDivNum = 32 , int YDivNum = 8 , int ZDivNum = 32 ) ;	// コリジョン情報を構築する
extern	int			MV1TerminateCollInfo( int MHandle, int FrameIndex = -1 ) ;									// コリジョン情報の後始末
extern	int			MV1RefreshCollInfo( int MHandle, int FrameIndex = -1 ) ;									// コリジョン情報を更新する
extern	MV1_COLL_RESULT_POLY     MV1CollCheck_Line( int MHandle, int FrameIndex, VECTOR PosStart, VECTOR PosEnd ) ; // 線とモデルの当たり判定
extern	MV1_COLL_RESULT_POLY_DIM MV1CollCheck_Sphere( int MHandle, int FrameIndex, VECTOR CenterPos, float r ) ;	// 球とモデルの当たり判定
extern	MV1_COLL_RESULT_POLY_DIM MV1CollCheck_Capsule( int MHandle, int FrameIndex, VECTOR Pos1, VECTOR Pos2, float r ) ;	// カプセルとモデルの当たり判定
extern	MV1_COLL_RESULT_POLY     MV1CollCheck_GetResultPoly( MV1_COLL_RESULT_POLY_DIM ResultPolyDim, int PolyNo ) ;	// コリジョン結果ポリゴン配列から指定番号のポリゴン情報を取得する
extern	int			MV1CollResultPolyDimTerminate( MV1_COLL_RESULT_POLY_DIM ResultPolyDim ) ;					// コリジョン結果ポリゴン配列の後始末をする

// 参照用メッシュ関係
extern	int			MV1SetupReferenceMesh( int MHandle, int FrameIndex, int IsTransform ) ;						// 参照用メッシュのセットアップ
extern	int			MV1TerminateReferenceMesh( int MHandle, int FrameIndex, int IsTransform ) ;					// 参照用メッシュの後始末
extern	int			MV1RefreshReferenceMesh( int MHandle, int FrameIndex, int IsTransform ) ;					// 参照用メッシュの更新
extern	MV1_REF_POLYGONLIST	MV1GetReferenceMesh( int MHandle, int FrameIndex, int IsTransform ) ;				// 参照用メッシュを取得する

#endif

#define DX_FUNCTION_END

}

// ネームスペース DxLib を使用する ------------------------------------------------------
using namespace DxLib ;

// ＤＸライブラリ内部でのみ使用するヘッダファイルのインクルード -------------------------

#ifdef __DX_MAKE
	#include "DxStatic.h"
#endif

#endif



// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用サウンドプログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_SOUNDIOS_H
#define DX_SOUNDIOS_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_SOUND

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxThread.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// ストリーム関係の定義
#define STS_BUFSEC_FILE						(256)			// ストリーム風サウンド再生時に確保するサウンドバッファの時間( 単位は STS_DIVNUM分の1秒 )( ファイルからのストリーム再生用 )
#define STS_ADVANCECOPYSEC_FILE				(192)			// ストリーム風サウンド再生時に音声データを先行展開しておく最大時間( 単位は STS_DIVNUM分の1秒 )( ファイルからのストリーム再生用 )
#define STS_MINADVANCECOPYSEC_FILE			(96)			// ストリーム風サウンド再生時に音声データを先行展開しておく最小時間( 単位は STS_DIVNUM分の1秒 )( ファイルからのストリーム再生用 )

// 一つのサウンドソースのバッファ数
#define SOUNDSOURCE_BUFFER_NUM				(6)

// サウンドバッファ再生処理用情報の最大数
#define SOUNDBUFFERPLAYINFO_MAX_NUM			(512)

// 構造体定義 --------------------------------------------------------------------

// サウンド破棄処理用構造体
struct SOUNDRELEASE_INFO
{
	int								UseFlag ;
	int								NextIndex ;

	ALuint							ALSource ;
	ALuint							ALBuffer[ SOUNDSOURCE_BUFFER_NUM ] ;

	DWORD							ReleaseTime ;
} ;

// サウンドバッファ環境依存情報構造体
struct SOUNDBUFFER_PF
{
	DX_CRITICAL_SECTION				CriticalSection ;					// クリティカルセクション

	int								BufferEndPlayEnqueueNum ;			// バッファキューに再生終了後のキューを積んだ回数

	int								PlaySetupComp ;						// 再生準備が済んでいるかどうか
 	ALuint							ALBuffer[ SOUNDSOURCE_BUFFER_NUM ] ; // ALバッファ
	void *							ALBufferImage[ SOUNDSOURCE_BUFFER_NUM ] ; // ALバッファイメージ
	DWORD							ALBufferValidBytes[ SOUNDSOURCE_BUFFER_NUM ] ;	// ALバッファの有効なデータのバイト数
	DWORD							ALBufferEndPos[ SOUNDSOURCE_BUFFER_NUM ] ;	// ALバッファの有効データの終了位置がサウンドバッファのどの位置に相当するか、の情報
 	ALuint							ALSource ;							// ALソース

	int								StopSoundBufferValid ;
	struct SOUNDBUFFER *			StopSoundBufferPrev ;
	struct SOUNDBUFFER *			StopSoundBufferNext ;

	int								PlaySoundBufferValid ;
	struct SOUNDBUFFER *			PlaySoundBufferPrev ;
	struct SOUNDBUFFER *			PlaySoundBufferNext ;

	struct SOUNDBUFFER *			RePlaySoundBufferPrev ;
	struct SOUNDBUFFER *			RePlaySoundBufferNext ;
} ;

// サウンドシステム用環境依存データ構造体
struct SOUNDSYSTEMDATA_PF
{
	int							InitializeFlag ;						// 初期化済みかどうか

	ALCdevice *					ALCdeviceObject ;						// ALデバイスオブジェクト
	ALCcontext *				ALCcontectObject ;						// ALコンテキストオブジェクト

	pthread_t					ProcessALBufferThread ;					// ALBufferの再生処理を行うスレッドのID
	volatile int				ProcessALBufferThreadEndRequest ;		// ALBufferの再生処理を行うスレッドを終了するかどうか

	pthread_t					ProcessSoundThread ;					// ProcessStreamSoundMemAll 等を呼ぶスレッドのID
	volatile int				ProcessSoundThreadEndRequest ;			// ProcessStreamSoundMemAll 等を呼ぶスレッドを終了するかどうか

	int							DXSoundProcessStart ;					// ＤＸライブラリのサウンド処理を開始したかどうか

	DX_CRITICAL_SECTION			StopSoundBufferCriticalSection ;		// クリティカルセクション
	struct SOUNDBUFFER *		StopSoundBuffer ;

	DX_CRITICAL_SECTION			PlaySoundBufferCriticalSection ;		// クリティカルセクション
	struct SOUNDBUFFER *		PlaySoundBuffer ;

	int							SoundReleaseInitialize ;
	DX_CRITICAL_SECTION			SoundReleaseCriticalSection ;
	int							SoundReleaseInfoMaxNum ;
	int							SoundReleaseInfoNum ;
	SOUNDRELEASE_INFO *			SoundReleaseInfo ;
	int							SoundReleaseInfoFirstIndex ;
	int							SoundReleaseInfoNextUseIndex ;
} ;

// ＭＩＤＩデータ環境依存情報
struct MIDIHANDLEDATA_PF
{
	int							Dummy ;
} ;

// ＭＩＤＩシステム用環境依存データ構造体
struct MIDISYSTEMDATA_PF
{
	int							Dummy ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

extern	int		MidiCallBackProcess( void ) ;																			// ＭＩＤＩ演奏終了時呼ばれるコールバック関数

extern	void	SoundBuffer_Apply_StopSoundBufferList( void ) ;							// 停止待ちサウンドバッファリストに登録されているサウンドバッファを停止する


// ALデバイスとALコンテキストをセットする
extern	void	SoundSystem_iOS_SetALDeviceAndALContext( void *ALCDevice, void *ALCContext ) ;

// 再生中の ALBuffer を再作成して再再生する
extern	void	SoundSystem_iOS_ALBuffer_RePlay( void ) ;


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_SOUND

#endif // DX_SOUNDIOS_H

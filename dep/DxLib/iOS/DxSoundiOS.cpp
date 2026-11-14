//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用サウンドプログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_SOUND

// インクルード----------------------------------------------------------------
#include "DxSoundiOS.h"
#include "DxFileiOS.h"
#include "DxSystemiOS_ObjC.h"
#include "../DxSound.h"
#include "../DxSystem.h"
#include "../DxMemory.h"
#include "../DxLog.h"
#include <unistd.h>
#include <sched.h>
#include <pthread.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// ストリーム再生用のバッファ一つ辺りのサンプル数
//#define STREAM_SOUND_BUFFER_UNIT_SAPMLES	(512)
//#define STREAM_SOUND_BUFFER_UNIT_SAPMLES	(735)
#define STREAM_SOUND_BUFFER_UNIT_SAPMLES	(1024)
//#define STREAM_SOUND_BUFFER_UNIT_SAPMLES	(2048)
//#define STREAM_SOUND_BUFFER_UNIT_SAPMLES  (3072)
//#define STREAM_SOUND_BUFFER_UNIT_SAPMLES	(4096)
//#define STREAM_SOUND_BUFFER_UNIT_SAPMLES	(8192)

// 破棄依頼があってから実際に破棄するまでの時間
#define SOUND_RELEASE_WAIT		(200)

// 型定義----------------------------------------------------------------------

// typedef ALvoid AL_APIENTRY ( *alBufferDataStaticProcPtr )( const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq ) ;
// static alBufferDataStaticProcPtr alBufferDataStaticProc ;

// データ宣言------------------------------------------------------------------

// 8bit波形を16bit波形に変換するためのテーブル
static short Bit8To16Table[ 256 ] ;

// 無音データ
static BYTE g_NoneSound8bit[  STREAM_SOUND_BUFFER_UNIT_SAPMLES ] ;
static WORD g_NoneSound16bit[ STREAM_SOUND_BUFFER_UNIT_SAPMLES ] ;

// 関数プロトタイプ宣言 -------------------------------------------------------

static void SetupSoundBuffer( SOUNDBUFFER *Buffer, int *IsNext, int TargetBuffer ) ;	// サウンドバッファにデータをセットする
static void SourceQueueSoundBuffer( SOUNDBUFFER *Buffer ) ;							// ALバッファをALソースに追加する
static void *StreamSoundThreadFunction( void *argc ) ;

static void SoundBuffer_Add_StopSoundBufferList( SOUNDBUFFER *Buffer ) ;			// 停止待ちサウンドバッファリストに指定のサウンドバッファを加える
static void SoundBuffer_Sub_StopSoundBufferList( SOUNDBUFFER *Buffer ) ;			// 停止待ちサウンドバッファリストから指定のサウンドバッファを外す

static void SoundBuffer_Add_PlaySoundBufferList( SOUNDBUFFER *Buffer ) ;			// 再生中サウンドバッファリストに指定のサウンドバッファを加える
static void SoundBuffer_Sub_PlaySoundBufferList( SOUNDBUFFER *Buffer ) ;			// 再生中サウンドバッファリストから指定のサウンドバッファを外す

static int SoundReleaseInfo_Initialize( void ) ;									// ALリソース解放処理の初期化
static int SoundReleaseInfo_Terminate( void ) ;										// ALリソース解放処理の後始末
static int SoundReleaseInfo_Process( void ) ;										// ALリソース解放処理の毎フレーム行う処理
static int SoundReleaseInfo_Add( SOUNDBUFFER *Buffer ) ;							// ALリソース解放リストに追加

// プログラム------------------------------------------------------------------

// サウンドバッファにデータをセットする
static void SetupSoundBuffer( SOUNDBUFFER *Buffer, int *IsNext, int TargetBuffer )
{
	int CompPos = Buffer->CompPos ;
	int Loop    = Buffer->Loop ;
	DWORD BufferSetSamples ;
	DWORD SamplePerBytes ;
	void *SrcBuffer ;

	SrcBuffer = Buffer->Wave->DoubleSizeBuffer ? Buffer->Wave->DoubleSizeBuffer : Buffer->Wave->Buffer ;
	SamplePerBytes = Buffer->Format.nChannels * Buffer->Format.wBitsPerSample / 8 ;

	BufferSetSamples = 0 ;
	while( BufferSetSamples < STREAM_SOUND_BUFFER_UNIT_SAPMLES )
	{
		if( CompPos >= Buffer->SampleNum )
		{
			if( Loop )
			{
				CompPos = 0 ;
				goto COPYDATA ;
			}
			else
			{
				break ;
			}
		}
		else
		{
			DWORD CopySamples ;

COPYDATA :
			CopySamples = ( DWORD )( Buffer->SampleNum - CompPos ) ;
			if( CopySamples > STREAM_SOUND_BUFFER_UNIT_SAPMLES - BufferSetSamples )
			{
				CopySamples = STREAM_SOUND_BUFFER_UNIT_SAPMLES - BufferSetSamples ;
			}

			_MEMCPY(
				( BYTE * )Buffer->PF.ALBufferImage[ TargetBuffer ] + BufferSetSamples * SamplePerBytes,
				( BYTE * )SrcBuffer                                + CompPos          * SamplePerBytes,
				CopySamples * SamplePerBytes
			) ;

			CompPos          += CopySamples ;
			BufferSetSamples += CopySamples ;
		}
	}
	Buffer->CompPos = CompPos ;

	Buffer->PF.ALBufferValidBytes[ TargetBuffer ] = BufferSetSamples * SamplePerBytes ;
	Buffer->PF.ALBufferEndPos[ TargetBuffer ] = Buffer->CompPos ;

	if( Loop == FALSE && Buffer->SampleNum == Buffer->CompPos )
	{
		*IsNext = FALSE ;
	}
	else
	{
		*IsNext = TRUE ;
	}
}

// 停止待ちサウンドバッファリストに指定のサウンドバッファを加える
static void SoundBuffer_Add_StopSoundBufferList( SOUNDBUFFER *Buffer )
{
	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &SoundSysData.PF.StopSoundBufferCriticalSection ) ;

	// リストに追加
	if( Buffer->PF.StopSoundBufferValid == FALSE )
	{
		Buffer->PF.StopSoundBufferValid = TRUE ;

		Buffer->PF.StopSoundBufferPrev = NULL ;
		Buffer->PF.StopSoundBufferNext = SoundSysData.PF.StopSoundBuffer ;
		if( SoundSysData.PF.StopSoundBuffer != NULL )
		{
			SoundSysData.PF.StopSoundBuffer->PF.StopSoundBufferPrev = Buffer ;
		}
		SoundSysData.PF.StopSoundBuffer = Buffer ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &SoundSysData.PF.StopSoundBufferCriticalSection ) ;
}

// 停止待ちサウンドバッファリストから指定のサウンドバッファを外す
static void SoundBuffer_Sub_StopSoundBufferList( SOUNDBUFFER *Buffer )
{
	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &SoundSysData.PF.StopSoundBufferCriticalSection ) ;

	if( Buffer->PF.StopSoundBufferValid )
	{
		Buffer->PF.StopSoundBufferValid = FALSE ;

		if( Buffer->PF.StopSoundBufferNext != NULL )
		{
			Buffer->PF.StopSoundBufferNext->PF.StopSoundBufferPrev = Buffer->PF.StopSoundBufferPrev ;
		}

		if( Buffer->PF.StopSoundBufferPrev != NULL )
		{
			Buffer->PF.StopSoundBufferPrev->PF.StopSoundBufferNext = Buffer->PF.StopSoundBufferNext ;
		}
		else
		{
			SoundSysData.PF.StopSoundBuffer = Buffer->PF.StopSoundBufferNext ;
		}

		Buffer->PF.StopSoundBufferNext = NULL ;
		Buffer->PF.StopSoundBufferPrev = NULL ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &SoundSysData.PF.StopSoundBufferCriticalSection ) ;
}

// 停止待ちサウンドバッファリストに登録されているサウンドバッファを停止する
extern void SoundBuffer_Apply_StopSoundBufferList( void )
{
	for(;;)
	{
		SOUNDBUFFER *StopSoundBuffer = NULL ;

		// クリティカルセクションの取得
		CRITICALSECTION_LOCK( &SoundSysData.PF.StopSoundBufferCriticalSection ) ;

		StopSoundBuffer = SoundSysData.PF.StopSoundBuffer ;

		// クリティカルセクションの解放
		CriticalSection_Unlock( &SoundSysData.PF.StopSoundBufferCriticalSection ) ;

		// 登録されているサウンドバッファが無ければ終了
		if( StopSoundBuffer == NULL )
		{
			break ;
		}

		// 再生の停止
		SoundBuffer_Stop( StopSoundBuffer ) ;
	}

}

// 再生中サウンドバッファリストに指定のサウンドバッファを加える
static void SoundBuffer_Add_PlaySoundBufferList( SOUNDBUFFER *Buffer )
{
	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &SoundSysData.PF.PlaySoundBufferCriticalSection ) ;

	// リストに追加
	if( Buffer->PF.PlaySoundBufferValid == FALSE )
	{
		Buffer->PF.PlaySoundBufferValid = TRUE ;

		Buffer->PF.PlaySoundBufferPrev = NULL ;
		Buffer->PF.PlaySoundBufferNext = SoundSysData.PF.PlaySoundBuffer ;
		if( SoundSysData.PF.PlaySoundBuffer != NULL )
		{
			SoundSysData.PF.PlaySoundBuffer->PF.PlaySoundBufferPrev = Buffer ;
		}
		SoundSysData.PF.PlaySoundBuffer = Buffer ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &SoundSysData.PF.PlaySoundBufferCriticalSection ) ;
}

// 再生中サウンドバッファリストから指定のサウンドバッファを外す
static void SoundBuffer_Sub_PlaySoundBufferList( SOUNDBUFFER *Buffer )
{
	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &SoundSysData.PF.PlaySoundBufferCriticalSection ) ;

	if( Buffer->PF.PlaySoundBufferValid )
	{
		Buffer->PF.PlaySoundBufferValid = FALSE ;

		if( Buffer->PF.PlaySoundBufferNext != NULL )
		{
			Buffer->PF.PlaySoundBufferNext->PF.PlaySoundBufferPrev = Buffer->PF.PlaySoundBufferPrev ;
		}

		if( Buffer->PF.PlaySoundBufferPrev != NULL )
		{
			Buffer->PF.PlaySoundBufferPrev->PF.PlaySoundBufferNext = Buffer->PF.PlaySoundBufferNext ;
		}
		else
		{
			SoundSysData.PF.PlaySoundBuffer = Buffer->PF.PlaySoundBufferNext ;
		}

		Buffer->PF.PlaySoundBufferNext = NULL ;
		Buffer->PF.PlaySoundBufferPrev = NULL ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &SoundSysData.PF.PlaySoundBufferCriticalSection ) ;
}

// ALリソース解放処理の初期化
static int SoundReleaseInfo_Initialize( void )
{
	// 既に初期化済みの場合は何もせずに終了
	if( SoundSysData.PF.SoundReleaseInitialize )
	{
		return 0 ;
	}

	// クリティカルセクションを初期化
	if( CriticalSection_Initialize( &SoundSysData.PF.SoundReleaseCriticalSection ) < 0 )
	{
		DXST_LOGFILEFMT_ADDUTF16LE(( "\x4f\x00\x70\x00\x65\x00\x6e\x00\x41\x00\x4c\x00\x20\x00\x6e\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xea\x30\xbd\x30\xfc\x30\xb9\x30\xe3\x89\x3e\x65\xe6\x51\x06\x74\x28\x75\x6e\x30\xaf\x30\xea\x30\xc6\x30\xa3\x30\xab\x30\xeb\x30\xbb\x30\xaf\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"OpenAL のサウンドリソース解放処理用のクリティカルセクションの作成に失敗しました\n" @*/ )) ;
		DXST_LOGFILE_TABSUB ;
		return -1 ;
	}

	// サウンド破棄処理用のメモリを確保
	SoundSysData.PF.SoundReleaseInfoMaxNum = 256 ;
	SoundSysData.PF.SoundReleaseInfoNum = 0 ;
	SoundSysData.PF.SoundReleaseInfo = ( SOUNDRELEASE_INFO * )DXCALLOC( SoundSysData.PF.SoundReleaseInfoMaxNum * sizeof( SOUNDRELEASE_INFO ) ) ;
	if( SoundSysData.PF.SoundReleaseInfo == NULL )
	{
		// サウンド破棄処理用のクリティカルセクションを削除
		CriticalSection_Delete( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

		DXST_LOGFILEFMT_ADDUTF16LE(( "\x4f\x00\x70\x00\x65\x00\x6e\x00\x41\x00\x4c\x00\x20\x00\x6e\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xea\x30\xbd\x30\xfc\x30\xb9\x30\xe3\x89\x3e\x65\xe6\x51\x06\x74\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"OpenAL のサウンドリソース解放処理用のメモリ確保に失敗しました\n" @*/ )) ;
		DXST_LOGFILE_TABSUB ;
		return -1 ;
	}

	// その他の情報を初期化
	SoundSysData.PF.SoundReleaseInfoFirstIndex = -1 ;
	SoundSysData.PF.SoundReleaseInfoNextUseIndex = 0 ;

	// 初期化フラグを立てる
	SoundSysData.PF.SoundReleaseInitialize = TRUE ;

	// 正常終了
	return 0 ;
}

// ALリソース解放処理の後始末
static int SoundReleaseInfo_Terminate( void )
{
	int ind ;

	// 初期化されていなかったら何もしない
	if( SoundSysData.PF.SoundReleaseInitialize == FALSE )
	{
		return 0 ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

	// 破棄予定のサウンドをすべて削除する
	for( ind = SoundSysData.PF.SoundReleaseInfoFirstIndex ; ind != -1 ; ind = SoundSysData.PF.SoundReleaseInfo[ ind ].NextIndex )
	{
		// ALソースを削除
		if( SoundSysData.PF.SoundReleaseInfo[ ind ].ALSource != 0 )
		{
			alSourceStop( SoundSysData.PF.SoundReleaseInfo[ ind ].ALSource ) ;
			alDeleteSources( 1, &SoundSysData.PF.SoundReleaseInfo[ ind ].ALSource ) ;
			SoundSysData.PF.SoundReleaseInfo[ ind ].ALSource = 0 ;
		}

		// ALバッファを削除
		if( SoundSysData.PF.SoundReleaseInfo[ ind ].ALBuffer[ 0 ] != 0 )
		{
			alDeleteBuffers( SOUNDSOURCE_BUFFER_NUM, SoundSysData.PF.SoundReleaseInfo[ ind ].ALBuffer );
			_MEMSET( SoundSysData.PF.SoundReleaseInfo[ ind ].ALBuffer, 0, sizeof( SoundSysData.PF.SoundReleaseInfo[ ind ].ALBuffer ) ) ;
		}

		// 使用中フラグを倒す
		SoundSysData.PF.SoundReleaseInfo[ ind ].UseFlag = FALSE ;
	}

	// サウンド破棄処理用に確保していたメモリを解放する
	DXFREE( SoundSysData.PF.SoundReleaseInfo ) ;
	SoundSysData.PF.SoundReleaseInfo = NULL ;

	// 初期化フラグを倒す
	SoundSysData.PF.SoundReleaseInitialize = FALSE ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

	// サウンド破棄処理用のクリティカルセクションを削除
	CriticalSection_Delete( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

	// 正常終了
	return 0 ;
}

// ALリソース解放処理の毎フレーム行う処理
static int SoundReleaseInfo_Process( void )
{
	int NowTime ;
	int prev_ind ;
	int ind ;

	// 現在の時間を取得
	NowTime = NS_GetNowCount( FALSE ) ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

	// 破棄する前に待つ時間を経過しているサウンドがあるか確認する
	prev_ind = -1 ;
	for( ind = SoundSysData.PF.SoundReleaseInfoFirstIndex ; ind != -1 && SoundSysData.PF.SoundReleaseInfo[ ind ].ReleaseTime + SOUND_RELEASE_WAIT > NowTime ; ind = SoundSysData.PF.SoundReleaseInfo[ ind ].NextIndex )
	{
		prev_ind = ind ;
	}

	// 破棄すべきサウンドが無い場合は何もせずに終了
	if( ind == -1 )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

		return 0 ;
	}

	// 破棄すべきサウンドをリストから外す
	if( ind == SoundSysData.PF.SoundReleaseInfoFirstIndex )
	{
		SoundSysData.PF.SoundReleaseInfoFirstIndex = -1 ;
	}
	else
	if( prev_ind != -1 )
	{
		SoundSysData.PF.SoundReleaseInfo[ prev_ind ].NextIndex = -1 ;
	}

	// 破棄すべきサウンドを破棄する
	for( ; ind != -1 ; ind = SoundSysData.PF.SoundReleaseInfo[ ind ].NextIndex )
	{
		// ALソースを削除
		if( SoundSysData.PF.SoundReleaseInfo[ ind ].ALSource != 0 )
		{
			alSourceStop( SoundSysData.PF.SoundReleaseInfo[ ind ].ALSource ) ;
			alDeleteSources( 1, &SoundSysData.PF.SoundReleaseInfo[ ind ].ALSource ) ;
			SoundSysData.PF.SoundReleaseInfo[ ind ].ALSource = 0 ;
		}

		// ALバッファを削除
		if( SoundSysData.PF.SoundReleaseInfo[ ind ].ALBuffer[ 0 ] != 0 )
		{
			alDeleteBuffers( SOUNDSOURCE_BUFFER_NUM, SoundSysData.PF.SoundReleaseInfo[ ind ].ALBuffer );
			_MEMSET( SoundSysData.PF.SoundReleaseInfo[ ind ].ALBuffer, 0, sizeof( SoundSysData.PF.SoundReleaseInfo[ ind ].ALBuffer ) ) ;
		}

		// 使用中フラグを倒す
		SoundSysData.PF.SoundReleaseInfo[ ind ].UseFlag = FALSE ;

		// 破棄情報の数を減らす
		SoundSysData.PF.SoundReleaseInfoNum -- ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

	// 正常終了
	return 0 ;
}

// ALリソース解放リストに追加
static int SoundReleaseInfo_Add( SOUNDBUFFER *Buffer )
{
	int ind ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

	// 既に数が最大数に達していたらバッファを大きくする
	if( SoundSysData.PF.SoundReleaseInfoNum >= SoundSysData.PF.SoundReleaseInfoMaxNum )
	{
		int OldMaxNum = SoundSysData.PF.SoundReleaseInfoMaxNum ;

		SoundSysData.PF.SoundReleaseInfoMaxNum *= 2 ;
		SoundSysData.PF.SoundReleaseInfo = ( SOUNDRELEASE_INFO * )DXREALLOC( SoundSysData.PF.SoundReleaseInfo, sizeof( SOUNDRELEASE_INFO ) * SoundSysData.PF.SoundReleaseInfoMaxNum ) ;
		if( SoundSysData.PF.SoundReleaseInfo == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\x34\x78\xc4\x68\xe6\x51\x06\x74\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xe1\x62\x35\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"サウンド破棄処理用のメモリ領域の拡張に失敗しました\n" @*/ )) ;

			// クリティカルセクションの解放
			CriticalSection_Unlock( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

			return -1 ;
		}

		// 拡張分のメモリを初期化
		_MEMSET( SoundSysData.PF.SoundReleaseInfo + OldMaxNum, 0, sizeof( SOUNDRELEASE_INFO ) * ( SoundSysData.PF.SoundReleaseInfoMaxNum - OldMaxNum ) ) ;
	}

	// 使用されていない要素を探す
	ind = SoundSysData.PF.SoundReleaseInfoNextUseIndex ;
	while( SoundSysData.PF.SoundReleaseInfo[ ind ].UseFlag )
	{
		ind = ind == SoundSysData.PF.SoundReleaseInfoMaxNum ? 0 : ind + 1 ;
	}
	SoundSysData.PF.SoundReleaseInfoNextUseIndex = ind == SoundSysData.PF.SoundReleaseInfoMaxNum ? 0 : ind + 1 ;

	// データをセット
	SoundSysData.PF.SoundReleaseInfo[ ind ].UseFlag = TRUE ;
	SoundSysData.PF.SoundReleaseInfo[ ind ].ALSource = Buffer->PF.ALSource ;
	_MEMCPY( SoundSysData.PF.SoundReleaseInfo[ ind ].ALBuffer, Buffer->PF.ALBuffer, sizeof( Buffer->PF.ALBuffer ) ) ;
	SoundSysData.PF.SoundReleaseInfo[ ind ].ReleaseTime = NS_GetNowCount( FALSE ) ;

	// リストに追加
	SoundSysData.PF.SoundReleaseInfo[ ind ].NextIndex = SoundSysData.PF.SoundReleaseInfoFirstIndex ;
	SoundSysData.PF.SoundReleaseInfoFirstIndex = ind ;
    
    // サウンド破棄情報の数を増やす
    SoundSysData.PF.SoundReleaseInfoNum ++ ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &SoundSysData.PF.SoundReleaseCriticalSection ) ;

	// 正常終了
	return 0 ;
}

// ALバッファをALソースに追加する
static void SourceQueueSoundBuffer( SOUNDBUFFER *Buffer )
{
	DWORD BytesRequired ;
	int Processed ;
    ALenum Format ;
	int i ;
	ALint state ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &Buffer->PF.CriticalSection ) ;

	if( Buffer->State == FALSE )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &Buffer->PF.CriticalSection ) ;
		return ;
	}

    if( Buffer->Format.wBitsPerSample == 8 )
    {
        Format = Buffer->Format.nChannels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8 ;
    }
    else
    {
        Format = Buffer->Format.nChannels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16 ;
    }

	// 再生が止まってしまっていたら再度再生する
//	alGetSourcei( Buffer->PF.ALSource, AL_SOURCE_STATE, &state ) ;
//	if( state != AL_PLAYING )
//	{
//		if( state == AL_PAUSED || state == AL_STOPPED )
//		{
//			alSourcePlay( Buffer->PF.ALSource ) ;
//		}
//	}

	// 再生が終了したキューが無ければ何もしない
	alGetSourcei( Buffer->PF.ALSource, AL_BUFFERS_PROCESSED, &Processed );
	if( Processed == 0 )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &Buffer->PF.CriticalSection ) ;
		return ;
	}

	for( i = 0 ; i < Processed ; i ++ )
	{
		int TargetBuffer ;
		ALuint ALBuffer ;
		int IsNext ;

		// 再生が完了したバッファを取得
		alSourceUnqueueBuffers( Buffer->PF.ALSource, 1, &ALBuffer );

		for( TargetBuffer = 0; TargetBuffer < SOUNDSOURCE_BUFFER_NUM && ALBuffer != Buffer->PF.ALBuffer[ TargetBuffer ]; TargetBuffer ++ ){}
		if( TargetBuffer == SOUNDSOURCE_BUFFER_NUM )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x61\x00\x6c\x00\x53\x00\x6f\x00\x75\x00\x72\x00\x63\x00\x65\x00\x55\x00\x6e\x00\x71\x00\x75\x00\x65\x00\x75\x00\x65\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x73\x00\x20\x00\x4c\x30\x7f\x4f\x28\x75\x57\x30\x66\x30\x44\x30\x6a\x30\x44\x30\x20\x00\x41\x00\x4c\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x20\x00\x92\x30\xd4\x8f\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"alSourceUnqueueBuffers が使用していない ALBuffer を返しました\n" @*/ )) ;

			// クリティカルセクションの解放
			CriticalSection_Unlock( &Buffer->PF.CriticalSection ) ;
			return ;
		}

		// 再生位置を更新
		Buffer->PlayPos = Buffer->PF.ALBufferEndPos[ TargetBuffer ] ;

		// ループせず、且つバッファの最後までセットが終わっている場合は再生終了処理をして終了
		if( Buffer->Loop == FALSE && Buffer->SampleNum == Buffer->CompPos )
		{
			// 再生状態を終了
			Buffer->State = FALSE ;

			// 再生しているサウンドのリストから外す
			SoundBuffer_Sub_PlaySoundBufferList( Buffer ) ;

			// サウンド破棄依頼に追加
			SoundReleaseInfo_Add( Buffer ) ;

			// 再生準備完了状態を解除
			Buffer->PF.PlaySetupComp = FALSE ;

			goto END ;
		}

		// バッファをセットアップ
		SetupSoundBuffer( Buffer, &IsNext, TargetBuffer ) ;
		alBufferData( Buffer->PF.ALBuffer[ TargetBuffer ], Format, Buffer->PF.ALBufferImage[ TargetBuffer ], Buffer->PF.ALBufferValidBytes[ TargetBuffer ], Buffer->Format.nSamplesPerSec ) ;

		// バッファをキューに追加
		alSourceQueueBuffers( Buffer->PF.ALSource, 1, &Buffer->PF.ALBuffer[ TargetBuffer ] ) ;
	}

END :

	// 再生が止まってしまっていたら再度再生する
	alGetSourcei( Buffer->PF.ALSource, AL_SOURCE_STATE, &state ) ;
	if( state != AL_PLAYING )
	{
		if( state == AL_PAUSED || state == AL_STOPPED )
		{
			alSourcePlay( Buffer->PF.ALSource ) ;
		}
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &Buffer->PF.CriticalSection ) ;
}

// サウンドバッファ再生処理用スレッド
static void *ALBufferPlayThreadFunction( void *argc )
{
	while( SoundSysData.PF.ProcessALBufferThreadEndRequest == FALSE )
	{
		// クリティカルセクションの取得
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		// 再生終了処理を行う
		SoundReleaseInfo_Process() ;

		// 再生処理を行う
		{
			SOUNDBUFFER *Buffer ;
			SOUNDBUFFER *NextBuffer ;

			// クリティカルセクションの取得
			CRITICALSECTION_LOCK( &SoundSysData.PF.PlaySoundBufferCriticalSection ) ;

			Buffer = SoundSysData.PF.PlaySoundBuffer ;
			while( Buffer != NULL )
			{
				NextBuffer = Buffer->PF.PlaySoundBufferNext ;
				SourceQueueSoundBuffer( Buffer ) ;
				Buffer = NextBuffer ;
			}

			// クリティカルセクションの解放
			CriticalSection_Unlock( &SoundSysData.PF.PlaySoundBufferCriticalSection ) ;
		}

		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		// 待ち
		usleep( 1000 ) ;
	}

	return NULL ;
}

// ストリームサウンド処理用スレッド
static void *StreamSoundThreadFunction( void *argc )
{
	while( SoundSysData.PF.ProcessSoundThreadEndRequest == FALSE )
	{
		// クリティカルセクションの取得
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		// 停止待ちサウンドバッファリストに登録されているサウンドバッファを停止する
		SoundBuffer_Apply_StopSoundBufferList() ;

		// ストリーミング処理
		NS_ProcessStreamSoundMemAll() ;

		// 再生が終了したらハンドルを削除する処理を行う
//		ProcessPlayFinishDeleteSoundMemAll() ;

		// ３Ｄサウンドを再生しているサウンドハンドルに対する処理を行う
		ProcessPlay3DSoundMemAll() ;

		// 再生しているサウンドハンドルに対する処理を行う
		ProcessPlaySoundMemAll() ;

		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;


		// クリティカルセクションの取得
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

		// ストリーミング処理
		ST_SoftSoundPlayerProcessAll() ;

		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

		// 待ち
		Thread_Sleep( 10 ) ;
	}

	return NULL ;
}

// サウンドシステムを初期化する関数の環境依存処理を行う関数
extern int InitializeSoundSystem_PF_Timing0( void )
{
	// 初期化済みの場合は何もせずに終了
	if( SoundSysData.PF.InitializeFlag )
	{
		return 0 ;
	}

	// 自前ミキシングを使用する
	SoundSysData.EnableSelfMixingFlag = TRUE ;

	if( SoundSysData.EnableSelfMixingFlag )
	{
		// 自前ミキシングのセットアップ
		SelfMixingPlayer_Setup() ;
	}
	else
	{
		int i ;

		if( SoundSysData.PF.ALCdeviceObject != NULL )
		{
			return 0 ;
		}

		DXST_LOGFILE_ADDUTF16LE( "\x4f\x00\x70\x00\x65\x00\x6e\x00\x41\x00\x4c\x00\x1d\x52\x1f\x67\x16\x53\x8b\x95\xcb\x59\x0a\x00\x00"/*@ L"OpenAL初期化開始\n" @*/ ) ;

		DXST_LOGFILE_TABADD ;

		// プレイサウンドバッファ用のクリティカルセクションを初期化
		if( CriticalSection_Initialize( &SoundSysData.PF.PlaySoundBufferCriticalSection ) < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x4f\x00\x70\x00\x65\x00\x6e\x00\x41\x00\x4c\x00\x20\x00\x3a\x7e\x6e\xff\x67\x7e\x75\xff\x67\x7e\x66\xff\x5d\x7e\x73\xff\x5d\x7e\xe8\x5c\xf0\x30\x5d\x7e\x00\x00\xf5\x30\x67\x7e\x61\xff\x00\x87\xb2\x54\xfd\x51\x03\x87\x66\xff\x05\x90\x00\x00\x51\x75\x3a\x7e\x6e\xff\x67\x7e\x6f\xff\x5d\x7e\x6a\xff\x5d\x7e\x00\x00\x45\x30\x67\x7e\x6b\xff\x5d\x7e\x6b\xff\x67\x7e\x7b\xff\x67\x7e\x6f\xff\x67\x7e\x77\xff\x5d\x7e\x67\xff\x5d\x7e\x73\xff\x3a\x7e\x6e\xff\xf4\x83\xc8\x61\x00\x00\x3a\x7e\x6b\xff\x9f\x87\x71\xff\x28\x8b\xb1\x52\x20\xff\x3a\x7e\x7e\xff\x3a\x7e\xb1\x52\xc6\x25\x0a\x00\x00"/*@ L"OpenAL のサウンドバッファ再生処理用のクリティカルセクションの作成に失敗しました\n" @*/ )) ;
			DXST_LOGFILE_TABSUB ;
			return -1 ;
		}

		// ストップサウンドバッファ用のクリティカルセクションを初期化
		if( CriticalSection_Initialize( &SoundSysData.PF.StopSoundBufferCriticalSection ) < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x4f\x00\x70\x00\x65\x00\x6e\x00\x41\x00\x4c\x00\x20\x00\x6e\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x5c\x50\x62\x6b\xe6\x51\x06\x74\x28\x75\x6e\x30\xaf\x30\xea\x30\xc6\x30\xa3\x30\xab\x30\xeb\x30\xbb\x30\xaf\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"OpenAL のサウンドバッファ停止処理用のクリティカルセクションの作成に失敗しました\n" @*/ )) ;
			DXST_LOGFILE_TABSUB ;
			return -1 ;
		}

		// 8bit波形を16bit波形に変換するためのテーブルを初期化
		for( i = 0 ; i < 256 ; i ++ )
		{
			Bit8To16Table[ i ] = ( short )( ( ( int )i * 65535 ) / 255 - 32768 ) ;
		}

		// 無音データの初期化
		for( i = 0 ; i < STREAM_SOUND_BUFFER_UNIT_SAPMLES ; i ++ )
		{
			g_NoneSound8bit[ i ]  = 128 ;
			g_NoneSound16bit[ i ] = 0 ;
		}

		// API のアドレスを取得
//		alBufferDataStaticProc = ( alBufferDataStaticProcPtr )alcGetProcAddress( NULL, "alBufferDataStatic" ) ;

		// ALデバイスの作成
		SoundSysData.PF.ALCdeviceObject = alcOpenDevice( alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER ) ) ;
		if( SoundSysData.PF.ALCdeviceObject == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x4f\x00\x70\x00\x65\x00\x6e\x00\x41\x00\x4c\x00\x20\x00\x6e\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"OpenAL のデバイスオブジェクトのオープンに失敗しました\n" @*/ )) ;
			DXST_LOGFILE_TABSUB ;
			return -1 ;
		}

		// ALコンテキストの作成
		SoundSysData.PF.ALCcontectObject = alcCreateContext( SoundSysData.PF.ALCdeviceObject, NULL ) ;
		if( SoundSysData.PF.ALCcontectObject == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x4f\x00\x70\x00\x65\x00\x6e\x00\x41\x00\x4c\x00\x20\x00\x6e\x30\xb3\x30\xf3\x30\xc6\x30\xad\x30\xb9\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"OpenAL のコンテキストの作成に失敗しました\n" @*/ )) ;
			DXST_LOGFILE_TABSUB ;
			return -1 ;
		}

//		// マイク許可確認が完了するまで待つ
//		while( CheckRecordPermissionProcessEnd() == FALSE )
//		{
//			usleep( 1000 ) ;
//		}

		// カレントALコンテキストの設定
		alcMakeContextCurrent( SoundSysData.PF.ALCcontectObject ) ;

		// サウンド破棄処理の初期化を行う
		SoundReleaseInfo_Initialize() ;

		// ALBuffer の再生処理を行うスレッドの開始
		{
			pthread_attr_t attr ;
			sched_param param ;
			int returnCode ;

			pthread_attr_init( &attr ) ;
			pthread_attr_setstacksize( &attr, 128 * 1024 ) ;

			returnCode = pthread_create(
				&SoundSysData.PF.ProcessALBufferThread,
				&attr,
				ALBufferPlayThreadFunction,
				NULL
			) ;
			if( returnCode != 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\x41\x00\x4c\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x20\x00\x6e\x30\x8d\x51\x1f\x75\xe6\x51\x06\x74\x92\x30\x4c\x88\x46\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x43\x00\x6f\x00\x64\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x30\x00\x78\x00\x25\x00\x30\x00\x38\x00\x58\x00\x0a\x00\x00"/*@ L"ALBuffer の再生処理を行うスレッドの作成に失敗しました Error Code : 0x%08X\n" @*/, returnCode )) ;
				DXST_LOGFILE_TABSUB ;
				return -1 ;
			}

			_MEMSET( &param, 0, sizeof( param ) ) ;
			param.sched_priority = sched_get_priority_max( 0 /* SCHED_NORMAL */ ) ;
			pthread_setschedparam( SoundSysData.PF.ProcessALBufferThread, 0 /* SCHED_NORMAL */, &param ) ;
		}

		DXST_LOGFILE_TABSUB ;

		DXST_LOGFILE_ADDUTF16LE( "\x4f\x00\x70\x00\x65\x00\x6e\x00\x41\x00\x4c\x00\x1d\x52\x1f\x67\x16\x53\x8c\x5b\x86\x4e\x0a\x00\x00"/*@ L"OpenAL初期化完了\n" @*/ ) ;
	}
 
	// ProcessStreamSoundMemAll 等を呼ぶスレッドの開始
	{
		pthread_attr_t attr ;
		int returnCode ;

		pthread_attr_init( &attr ) ;
		pthread_attr_setstacksize( &attr, 128 * 1024 ) ;

		returnCode = pthread_create(
			&SoundSysData.PF.ProcessSoundThread,
			&attr,
			StreamSoundThreadFunction,
			NULL
		) ;
		if( returnCode != 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x50\x00\x72\x00\x6f\x00\x63\x00\x65\x00\x73\x00\x73\x00\x53\x00\x74\x00\x72\x00\x65\x00\x61\x00\x6d\x00\x53\x00\x6f\x00\x75\x00\x6e\x00\x64\x00\x4d\x00\x65\x00\x6d\x00\x41\x00\x6c\x00\x6c\x00\x20\x00\x49\x7b\x92\x30\x7c\x54\x76\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x43\x00\x6f\x00\x64\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x30\x00\x78\x00\x25\x00\x30\x00\x38\x00\x58\x00\x0a\x00\x00"/*@ L"ProcessStreamSoundMemAll 等を呼ぶスレッドの作成に失敗しました Error Code : 0x%08X\n" @*/, returnCode )) ;
			DXST_LOGFILE_TABSUB ;
			return -1 ;
		}
	}

	// 初期化フラグを立てる
	SoundSysData.PF.InitializeFlag = TRUE ;

	// 終了
	return 0 ;
}


// サウンドシステムを初期化する関数の環境依存処理を行う関数( 実行箇所区別１ )
extern	int		InitializeSoundSystem_PF_Timing1( void )
{
#ifndef DX_NON_MULTITHREAD
	SoundSysData.PF.DXSoundProcessStart = TRUE ;
#endif // DX_NON_MULTITHREAD

	// 正常終了
	return 0 ;
}




// サウンドシステムの後始末をする関数の環境依存処理を行う関数( 実行箇所区別０ )
extern	int		TerminateSoundSystem_PF_Timing0( void )
{
#ifndef DX_NON_MULTITHREAD
	SoundSysData.PF.DXSoundProcessStart = FALSE ;
#endif // DX_NON_MULTITHREAD

	// ProcessStreamSoundMemAll 等を呼ぶスレッドを終了する
	SoundSysData.PF.ProcessSoundThreadEndRequest = TRUE ;
	pthread_join( SoundSysData.PF.ProcessSoundThread, NULL ) ;

	if( SoundSysData.EnableSelfMixingFlag == FALSE )
	{
		// ALBuffer の再生処理を行うスレッドを終了する
		SoundSysData.PF.ProcessALBufferThreadEndRequest = TRUE ;
		pthread_join( SoundSysData.PF.ProcessALBufferThread, NULL ) ;
	}

	// 正常終了
	return 0 ;
}


// サウンドシステムの後始末をする関数の環境依存処理を行う関数( 実行箇所区別１ )
extern	int		TerminateSoundSystem_PF_Timing1( void )
{
	// サウンド破棄処理の後始末を行う
	SoundReleaseInfo_Terminate() ;

	// 自前ミキシングかどうかで処理を分岐
	if( SoundSysData.EnableSelfMixingFlag )
	{
		SelfMixingPlayer_Terminate() ;
	}
	else
	{
		// カレントALコンテキストの解除
		alcMakeContextCurrent( NULL ) ;

		// ALコンテキストの後始末
		if( SoundSysData.PF.ALCcontectObject )
		{
			alcDestroyContext( SoundSysData.PF.ALCcontectObject ) ;
			SoundSysData.PF.ALCcontectObject = NULL ;
		}

		// ALデバイスの後始末
		if( SoundSysData.PF.ALCdeviceObject )
		{
			alcCloseDevice( SoundSysData.PF.ALCdeviceObject ) ;
			SoundSysData.PF.ALCdeviceObject = NULL ;
		}

		// ストップサウンドバッファ用のクリティカルセクションを削除
		CriticalSection_Delete( &SoundSysData.PF.StopSoundBufferCriticalSection ) ;

		// プレイサウンドバッファ用のクリティカルセクションを削除
		CriticalSection_Delete( &SoundSysData.PF.PlaySoundBufferCriticalSection ) ;
	}

	// 初期化フラグを倒す
	SoundSysData.PF.InitializeFlag = FALSE ;

	// 正常終了
	return 0 ;
}

// サウンドシステムで周期的に行う処理用の関数の環境依存処理を行う関数
extern	int		ProcessSoundSystem_PF( void )
{
	return 0 ;
}

// サウンドシステムの初期化チェックの環境依存処理を行う関数( TRUE:初期化されている  FALSE:初期化されていない )
extern	int		CheckSoundSystem_Initialize_PF( void )
{
	return SoundSysData.PF.ALCdeviceObject != NULL || SoundSysData.PF.InitializeFlag ? TRUE : FALSE ;
}

// サウンドシステムの総再生時間を取得する
extern	int		GetSoundSystemTotalPlaySamples_PF( ULONGLONG *TotalPlaySamples, ULONGLONG *Frequency )
{
	// 非対応
	return -1 ;
}

// ＭＩＤＩハンドルの後始末を行う関数の環境依存処理
extern	int		TerminateMidiHandle_PF( MIDIHANDLEDATA *MusicData )
{
	// 正常終了
	return 0 ;
}

// プリセットの３Ｄサウンド用のリバーブパラメータを取得する処理の環境依存処理を行う関数
extern	int		Get3DPresetReverbParamSoundMem_PF( SOUND3D_REVERB_PARAM *ParamBuffer, int PresetNo /* DX_REVERB_PRESET_DEFAULT 等 */ )
{
	return 0 ;
}

// ３Ｄサウンドのリスナーの位置とリスナーの前方位置とリスナーの上方向位置を設定する処理の環境依存処理を行う関数
extern int Set3DSoundListenerPosAndFrontPosAndUpVec_PF( VECTOR Position, VECTOR FrontPosition, VECTOR UpVector )
{
	return 0 ;
}

// ３Ｄサウンドのリスナーの移動速度を設定する処理の環境依存処理を行う関数
extern int Set3DSoundListenerVelocity_PF( VECTOR Velocity )
{
	return 0 ;
}

// ３Ｄサウンドのリスナーの可聴角度範囲を設定する処理の環境依存処理を行う関数
extern int Set3DSoundListenerConeAngle_PF( float InnerAngle, float OuterAngle )
{
	return 0 ;
}

// ３Ｄサウンドのリスナーの可聴角度範囲の音量倍率を設定する処理の環境依存処理を行う関数
extern int Set3DSoundListenerConeVolume_PF( float InnerAngleVolume, float OuterAngleVolume )
{
	return 0 ;
}

// LoadMusicMemByMemImage の実処理関数の環境依存処理を行う関数
extern int LoadMusicMemByMemImage_Static_PF( MIDIHANDLEDATA *MusicData, int ASyncThread )
{
	return 0 ;
}

// 読み込んだＭＩＤＩデータの演奏を開始する処理の環境依存処理を行う関数
extern int PlayMusicMem_PF( MIDIHANDLEDATA *MusicData, int PlayType )
{
	return 0 ;
}

// ＭＩＤＩデータの演奏を停止する処理の環境依存処理を行う
extern int StopMusicMem_PF( MIDIHANDLEDATA *MusicData )
{
	return 0 ;
}

// ＭＩＤＩデータが演奏中かどうかを取得する( TRUE:演奏中  FALSE:停止中 )処理の環境依存処理を行う関数
extern int CheckMusicMem_PF( MIDIHANDLEDATA *MusicData )
{
	return 0 ;
}

// ＭＩＤＩデータの周期的処理の環境依存処理を行う関数
extern int ProcessMusicMem_PF( MIDIHANDLEDATA *MusicData )
{
	return 0 ;
}

// ＭＩＤＩデータの現在の再生位置を取得する処理の環境依存処理を行う関数
extern int GetMusicMemPosition_PF( MIDIHANDLEDATA *MusicData )
{
	return 0 ;
}

// ＭＩＤＩの再生音量をセットする処理の環境依存処理を行う関数
extern int SetVolumeMusic_PF( int Volume )
{
	return 0 ;
}

// ＭＩＤＩの現在の再生位置を取得する処理の環境依存処理を行う関数
extern int GetMusicPosition_PF( void )
{
	return 0 ;
}









extern int SoundBuffer_Initialize_Timing0_PF( SOUNDBUFFER *Buffer, DWORD Bytes, WAVEFORMATEX *Format, SOUNDBUFFER *Src, int Is3DSound )
{
	int i ;
	int InitializeCriticalSection = FALSE ;

	// ゼロ初期化
	_MEMSET( &Buffer->PF, 0, sizeof( Buffer->PF ) ) ;

	// クリティカルセクションを初期化
	if( CriticalSection_Initialize( &Buffer->PF.CriticalSection ) < 0 )
	{
		goto ERR ;
	}
	InitializeCriticalSection = TRUE ;

	// ALソースを作成
//	alGetError() ;
//	alGenSources( 1, &Buffer->PF.ALSource ) ;
//	if( alGetError() != AL_NO_ERROR )
//	{
//		DXST_LOGFILEFMT_ADDUTF16LE(( "\x67\x7e\x75\xff\x67\x7e\x66\xff\x5d\x7e\x73\xff\x5d\x7e\xe8\x5c\xf0\x30\x5d\x7e\x00\x00\xf5\x30\x67\x7e\x61\xff\x15\x90\x68\xff\x3a\x7e\x6e\xff\x41\x00\x4c\x00\x67\x7e\x7d\xff\x5d\x7e\x7c\xff\x67\x7e\x79\xff\x3a\x7e\x6e\xff\xf4\x83\xc8\x61\x00\x00\x3a\x7e\x6b\xff\x9f\x87\x71\xff\x28\x8b\xb1\x52\x20\xff\x3a\x7e\x7e\xff\x3a\x7e\xb1\x52\xc6\x25\x0a\x00\x00"/*@ L"サウンドバッファ用のALソースの作成に失敗しました\n" @*/ )) ;
//		goto ERR ;
//	}
//
//	// AL用バッファを作成
//	alGetError() ;
//	alGenBuffers( SOUNDSOURCE_BUFFER_NUM, Buffer->PF.ALBuffer ) ;
//	if( alGetError() != AL_NO_ERROR )
//	{
//		DXST_LOGFILEFMT_ADDUTF16LE(( "\x67\x7e\x75\xff\x67\x7e\x66\xff\x5d\x7e\x73\xff\x5d\x7e\xe8\x5c\xf0\x30\x5d\x7e\x00\x00\xf5\x30\x67\x7e\x61\xff\x15\x90\x68\xff\x3a\x7e\x6e\xff\x41\x00\x4c\x00\x5d\x7e\x0c\x82\xe3\x30\x5d\x7e\x14\x8f\x43\x30\x3a\x7e\x6e\xff\xf4\x83\xc8\x61\x00\x00\x3a\x7e\x6b\xff\x9f\x87\x71\xff\x28\x8b\xb1\x52\x20\xff\x3a\x7e\x7e\xff\x3a\x7e\xb1\x52\xc6\x25\x0a\x00\x00"/*@ L"サウンドバッファ用のALバッファの作成に失敗しました\n" @*/ )) ;
//		goto ERR ;
//	}

	// サウンドバッファ用のメモリを確保
	if( Src != NULL )
	{
		Buffer->SampleNum	= Src->SampleNum ;
		Buffer->Format		= Src->Format ;
		Buffer->Wave		= DuplicateWaveData( Src->Wave ) ;
	}
	else
	{
		Buffer->Format		= *Format ;
		Buffer->SampleNum	= ( int )( Bytes / Format->nBlockAlign ) ;
		Buffer->Wave		= AllocWaveData( ( int )Bytes, FALSE ) ;
	}
	if( Buffer->Wave == NULL )
	{
		DXST_LOGFILEFMT_ADDUTF16LE(( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"サウンドバッファ用のメモリの確保に失敗しました\n" @*/ )) ;
		goto ERR ;
	}

	// AL用バッファのメモリの確保
	for( i = 0 ; i < SOUNDSOURCE_BUFFER_NUM ; i ++ )
	{
		Buffer->PF.ALBufferImage[ i ] = DXALLOC( STREAM_SOUND_BUFFER_UNIT_SAPMLES * Buffer->Format.wBitsPerSample * Buffer->Format.nChannels / 8 ) ;
		if( Buffer->PF.ALBufferImage[ i ] == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x41\x00\x4c\x00\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ALサウンドバッファ用のメモリの確保に失敗しました\n" @*/ )) ;
			goto ERR ;
		}
	}

	// 正常終了
	return 0 ;

	// エラー処理
ERR :

//	if( Buffer->PF.ALSource != 0 )
//	{
//		alSourceStop( Buffer->PF.ALSource ) ;
//		alDeleteSources( 1, &Buffer->PF.ALSource ) ;
//		Buffer->PF.ALSource = 0 ;
//	}
//
//	if( Buffer->PF.ALBuffer[ 0 ] != 0 )
//	{
//		alDeleteBuffers( SOUNDSOURCE_BUFFER_NUM, Buffer->PF.ALBuffer );
//		_MEMSET( Buffer->PF.ALBuffer, 0, sizeof( Buffer->PF.ALBuffer ) ) ;
//	}

	for( i = 0 ; i < SOUNDSOURCE_BUFFER_NUM ; i ++ )
	{
		if( Buffer->PF.ALBufferImage[ i ] != NULL )
		{
			DXFREE( Buffer->PF.ALBufferImage[ i ] ) ;
			Buffer->PF.ALBufferImage[ i ] = NULL ;
		}
	}

	if( InitializeCriticalSection )
	{
		CriticalSection_Delete( &Buffer->PF.CriticalSection ) ;
	}

	return -1 ;
}

extern int SoundBuffer_Initialize_Timing1_PF( SOUNDBUFFER *Buffer, SOUNDBUFFER *Src, int Is3DSound )
{
	return 0 ;
}

extern int SoundBuffer_Terminate_PF( SOUNDBUFFER *Buffer )
{
    int i ;

	// ALソースを解放
//	if( Buffer->PF.ALSource != 0 )
//	{
//		alSourceStop( Buffer->PF.ALSource ) ;
//		alDeleteSources( 1, &Buffer->PF.ALSource ) ;
//		Buffer->PF.ALSource = 0 ;
//	}
//
//	// ALバッファを解放
//	if( Buffer->PF.ALBuffer[ 0 ] != 0 )
//	{
//		alDeleteBuffers( SOUNDSOURCE_BUFFER_NUM, Buffer->PF.ALBuffer );
//		_MEMSET( Buffer->PF.ALBuffer, 0, sizeof( Buffer->PF.ALBuffer ) ) ;
//	}

	// ALバッファ用のメモリを解放
	for( i = 0 ; i < SOUNDSOURCE_BUFFER_NUM ; i ++ )
	{
		if( Buffer->PF.ALBufferImage[ i ] != NULL )
		{
			DXFREE( Buffer->PF.ALBufferImage[ i ] ) ;
			Buffer->PF.ALBufferImage[ i ] = NULL ;
		}
	}

	// クリティカルセクションの削除
	CriticalSection_Delete( &Buffer->PF.CriticalSection ) ;

	return 0 ;
}

extern int SoundBuffer_CheckEnable_PF( SOUNDBUFFER *Buffer )
{
	return TRUE ;
}

extern int SoundBuffer_Play_PF( SOUNDBUFFER *Buffer, int Loop )
{
	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &Buffer->PF.CriticalSection ) ;

	// 停止待ちサウンドバッファリストからサウンドバッファを外す
	SoundBuffer_Sub_StopSoundBufferList( Buffer ) ;

	// 再生準備がされていなかったら準備を行う
	if( Buffer->PF.PlaySetupComp == FALSE )
	{
		// ALソースを作成
		alGetError() ;
		alGenSources( 1, &Buffer->PF.ALSource ) ;
		if( alGetError() != AL_NO_ERROR )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\x6e\x30\x41\x00\x4c\x00\xbd\x30\xfc\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"サウンドバッファ用のALソースの作成に失敗しました\n" @*/ )) ;
			goto ERR ;
		}

		// AL用バッファを作成
		alGetError() ;
		alGenBuffers( SOUNDSOURCE_BUFFER_NUM, Buffer->PF.ALBuffer ) ;
		if( alGetError() != AL_NO_ERROR )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\x6e\x30\x41\x00\x4c\x00\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"サウンドバッファ用のALバッファの作成に失敗しました\n" @*/ )) ;
			goto ERR ;
		}

		// 再生準備完了
		Buffer->PF.PlaySetupComp = TRUE ;

		// 音量の設定
		SoundBuffer_RefreshVolume_PF( Buffer ) ;

		// ピッチの設定
		if( Buffer->Frequency >= 0 )
		{
			SoundBuffer_SetFrequency_PF( Buffer, Buffer->Frequency ) ;
		}
	}

	// 再生状態にセット
	Buffer->State = TRUE ;

	// ALサウンドバッファをALサウンドソースに追加
	{
		int IsNext ;
		ALenum Format ;
		DWORD AddBufferCount = 0 ;

		if( Buffer->Format.wBitsPerSample == 8 )
		{
			Format = Buffer->Format.nChannels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8 ;
		}
		else
		{
			Format = Buffer->Format.nChannels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16 ;
		}

		// 一つ目のサウンドバッファをセットアップ
		SetupSoundBuffer( Buffer, &IsNext, 0 ) ;
		alBufferData( Buffer->PF.ALBuffer[ 0 ], Format, Buffer->PF.ALBufferImage[ 0 ], Buffer->PF.ALBufferValidBytes[ 0 ], Buffer->Format.nSamplesPerSec ) ;
		AddBufferCount ++ ;

		if( Buffer->CompPos < Buffer->SampleNum )
		{
			// 二つ目のサウンドバッファをセットアップ
			SetupSoundBuffer( Buffer, &IsNext, 1 ) ;
			alBufferData( Buffer->PF.ALBuffer[ 1 ], Format, Buffer->PF.ALBufferImage[ 1 ], Buffer->PF.ALBufferValidBytes[ 1 ], Buffer->Format.nSamplesPerSec ) ;
			AddBufferCount ++ ;

			if( Buffer->CompPos < Buffer->SampleNum )
			{
				// 三つ目のサウンドバッファをセットアップ
				SetupSoundBuffer( Buffer, &IsNext, 2 ) ;
				alBufferData( Buffer->PF.ALBuffer[ 2 ], Format, Buffer->PF.ALBufferImage[ 2 ], Buffer->PF.ALBufferValidBytes[ 2 ], Buffer->Format.nSamplesPerSec ) ;
				AddBufferCount ++ ;

				if( Buffer->CompPos < Buffer->SampleNum )
				{
					// 四つ目のサウンドバッファをセットアップ
					SetupSoundBuffer( Buffer, &IsNext, 3 ) ;
					alBufferData( Buffer->PF.ALBuffer[ 3 ], Format, Buffer->PF.ALBufferImage[ 3 ], Buffer->PF.ALBufferValidBytes[ 3 ], Buffer->Format.nSamplesPerSec ) ;
					AddBufferCount ++ ;

					if( Buffer->CompPos < Buffer->SampleNum )
					{
						// 五つ目のサウンドバッファをセットアップ
						SetupSoundBuffer( Buffer, &IsNext, 4 ) ;
						alBufferData( Buffer->PF.ALBuffer[ 4 ], Format, Buffer->PF.ALBufferImage[ 4 ], Buffer->PF.ALBufferValidBytes[ 4 ], Buffer->Format.nSamplesPerSec ) ;
						AddBufferCount ++ ;

						if( Buffer->CompPos < Buffer->SampleNum )
						{
							// 六つ目のサウンドバッファをセットアップ
							SetupSoundBuffer( Buffer, &IsNext, 5 ) ;
							alBufferData( Buffer->PF.ALBuffer[ 5 ], Format, Buffer->PF.ALBufferImage[ 5 ], Buffer->PF.ALBufferValidBytes[ 5 ], Buffer->Format.nSamplesPerSec ) ;
							AddBufferCount ++ ;
						}
					}
				}
			}
		}

		// バッファをキューに追加
		alSourceQueueBuffers( Buffer->PF.ALSource, AddBufferCount, Buffer->PF.ALBuffer ) ;
	}

	Buffer->StopTimeState = 1 ;
	Buffer->StopTime = 0 ;

	// 再生を開始
	alSourcePlay( Buffer->PF.ALSource ) ;

	// 再生しているサウンドのリストに加える
	SoundBuffer_Add_PlaySoundBufferList( Buffer ) ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &Buffer->PF.CriticalSection ) ;

	return 0 ;

ERR :
	if( Buffer->PF.ALSource != 0 )
	{
		alSourceStop( Buffer->PF.ALSource ) ;
		alDeleteSources( 1, &Buffer->PF.ALSource ) ;
		Buffer->PF.ALSource = 0 ;
	}

	if( Buffer->PF.ALBuffer[ 0 ] != 0 )
	{
		alDeleteBuffers( SOUNDSOURCE_BUFFER_NUM, Buffer->PF.ALBuffer );
		_MEMSET( Buffer->PF.ALBuffer, 0, sizeof( Buffer->PF.ALBuffer ) ) ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &Buffer->PF.CriticalSection ) ;

	return -1 ;
}

extern int SoundBuffer_Stop_PF(	SOUNDBUFFER *Buffer, int EffectStop )
{
	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &Buffer->PF.CriticalSection ) ;

	// 再生の準備が行われている場合のみ処理
	if( Buffer->PF.PlaySetupComp )
	{
		if( Buffer->PF.ALSource != 0 )
		{
			alSourceStop( Buffer->PF.ALSource ) ;
			alDeleteSources( 1, &Buffer->PF.ALSource ) ;
			Buffer->PF.ALSource = 0 ;
		}

		if( Buffer->PF.ALBuffer[ 0 ] != 0 )
		{
			alDeleteBuffers( SOUNDSOURCE_BUFFER_NUM, Buffer->PF.ALBuffer ) ;
			_MEMSET( Buffer->PF.ALBuffer, 0, sizeof( Buffer->PF.ALBuffer ) ) ;
		}

		Buffer->PF.PlaySetupComp = FALSE ;
	}

	Buffer->State = FALSE ;

	// 再生しているサウンドのリストから外す
	SoundBuffer_Sub_PlaySoundBufferList( Buffer ) ;

	// 停止待ちサウンドバッファリストからサウンドバッファを外す
	SoundBuffer_Sub_StopSoundBufferList( Buffer ) ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &Buffer->PF.CriticalSection ) ;

	return 0 ;
}

extern int SoundBuffer_CheckPlay_PF( SOUNDBUFFER *Buffer )
{
	return Buffer->State ;
}

extern int SoundBuffer_Lock_PF( SOUNDBUFFER *Buffer, DWORD WritePos , DWORD WriteSize, void **LockPos1, DWORD *LockSize1, void **LockPos2, DWORD *LockSize2 )
{
	return 2 ;
}

extern int SoundBuffer_Unlock_PF( SOUNDBUFFER *Buffer, void *LockPos1, DWORD LockSize1, void *LockPos2, DWORD LockSize2 )
{
	DWORD i ;

	// ダブルサイズの場合はステレオデータにする
	if( Buffer->Wave->DoubleSizeBuffer != NULL )
	{
		switch( Buffer->Format.wBitsPerSample )
		{
		case 8 :
			{
				BYTE *Src8bit ;
				WORD *Dest8bit ;
				DWORD SampleNum ;

				Src8bit = ( BYTE * )LockPos1 ;
				Dest8bit = ( WORD * )Buffer->Wave->DoubleSizeBuffer + ( ( BYTE * )LockPos1 - ( BYTE * )Buffer->Wave->Buffer ) ;
				SampleNum = LockSize1 ;
				for( i = 0 ; i < SampleNum ; i ++ )
				{
					Dest8bit[ i ] = ( WORD )( Src8bit[ i ] + ( Src8bit[ i ] << 8 ) ) ;
				}

				Src8bit = ( BYTE * )LockPos2 ;
				Dest8bit = ( WORD * )Buffer->Wave->DoubleSizeBuffer + ( ( BYTE * )LockPos2 - ( BYTE * )Buffer->Wave->Buffer ) ;
				SampleNum = LockSize2 ;
				for( i = 0 ; i < SampleNum ; i ++ )
				{
					Dest8bit[ i ] = ( WORD )( Src8bit[ i ] + ( Src8bit[ i ] << 8 ) ) ;
				}
			}
			break ;

		case 16 :
			{
				WORD *Src16bit ;
				DWORD *Dest16bit ;
				DWORD SampleNum ;

				Src16bit = ( WORD * )LockPos1 ;
				Dest16bit = ( DWORD * )Buffer->Wave->DoubleSizeBuffer + ( ( WORD * )LockPos1 - ( WORD * )Buffer->Wave->Buffer ) ;
				SampleNum = LockSize1 / 2 ;
				for( i = 0 ; i < SampleNum ; i ++ )
				{
					Dest16bit[ i ] = ( DWORD )( Src16bit[ i ] + ( Src16bit[ i ] << 16 ) ) ;
				}

				Src16bit = ( WORD * )LockPos2 ;
				Dest16bit = ( DWORD * )Buffer->Wave->DoubleSizeBuffer + ( ( WORD * )LockPos2 - ( WORD * )Buffer->Wave->Buffer ) ;
				SampleNum = LockSize2 / 2 ;
				for( i = 0 ; i < SampleNum ; i ++ )
				{
					Dest16bit[ i ] = ( DWORD )( Src16bit[ i ] + ( Src16bit[ i ] << 16 ) ) ;
				}
			}
			break ;
		}
	}

	return 0 ;
}

extern int SoundBuffer_SetFrequency_PF( SOUNDBUFFER *Buffer, DWORD Frequency )
{
	// 再生準備が成功した場合のみ処理を行う
	if( Buffer->PF.PlaySetupComp )
	{
		alSourcef( Buffer->PF.ALSource, AL_PITCH, ( float )Frequency / Buffer->Format.nSamplesPerSec ) ;
	}

	return 0 ;
}

extern int SoundBuffer_GetFrequency_PF( SOUNDBUFFER *Buffer, LPDWORD Frequency )
{
	return 2 ;
}

extern int SoundBuffer_RefreshVolume_PF( SOUNDBUFFER *Buffer )
{
	// 再生準備が成功した場合のみ処理を行う
	if( Buffer->PF.PlaySetupComp )
	{
		int db ;
		float Position[ 3 ] ;

		Position[ 0 ] = Buffer->Pan / 10000.0f ;
		Position[ 1 ] = 0.0f ;
		Position[ 2 ] = 0.0f ;
		alSourcefv( Buffer->PF.ALSource, AL_POSITION, Position ) ;
		alSourcei( Buffer->PF.ALSource, AL_SOURCE_RELATIVE, AL_FALSE ) ;
		alSourcef( Buffer->PF.ALSource, AL_MAX_DISTANCE, 1.0f ) ;
		alSourcef( Buffer->PF.ALSource, AL_REFERENCE_DISTANCE, 0.5f ) ;

		db = Buffer->Volume[ 0 ] ;
		if( db <= -10000 )
		{
			db = -10000 ;
		}
		else
		if( db >= 0 )
		{
			db = 0 ;
		}
		alSourcef( Buffer->PF.ALSource, AL_GAIN, D_XAudio2DecibelsToAmplitudeRatio( db / 100.0f ) ) ;
	}

	return 0 ;
}

extern int SoundBuffer_GetCurrentPosition_PF(	SOUNDBUFFER *Buffer, LPDWORD PlayPos, LPDWORD WritePos )
{
	return 2 ;
}

extern int SoundBuffer_SetCurrentPosition_PF( SOUNDBUFFER *Buffer, DWORD NewPos )
{
	int OldState = Buffer->State ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &Buffer->PF.CriticalSection ) ;

	if( OldState == TRUE )
	{
		SoundBuffer_Stop( Buffer ) ;
	}

	Buffer->PlayPos  = ( int )( NewPos / Buffer->Format.nBlockAlign ) ;
	Buffer->CompPos  = Buffer->PlayPos ;
	Buffer->CompPosF = _FTOL( Buffer->CompPos ) ;
	if( Buffer->Frequency < 0 || Buffer->Frequency == Buffer->Format.nSamplesPerSec )
	{
		Buffer->EnableComPosF = FALSE ;
	}

	if( OldState == TRUE )
	{
		SoundBuffer_Play( Buffer, Buffer->Loop ) ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &Buffer->PF.CriticalSection ) ;

	return 0 ;
}

extern int SoundBuffer_CycleProcess_PF( SOUNDBUFFER *Buffer )
{
	return -1 ;
}

extern int SoundBuffer_Set3DPosition_PF( SOUNDBUFFER *Buffer, VECTOR *Position )
{
	return 0 ;
}

extern int SoundBuffer_Set3DRadius_PF( SOUNDBUFFER *Buffer, float Radius )
{
	return 0 ;
}

extern int SoundBuffer_Set3DInnerRadius_PF(	SOUNDBUFFER *Buffer, float Radius )
{
	return 0 ;
}

extern int SoundBuffer_Set3DVelocity_PF( SOUNDBUFFER *Buffer, VECTOR *Velocity )
{
	return 0 ;
}

extern int SoundBuffer_Set3DFrontPosition_PF( SOUNDBUFFER *Buffer, VECTOR *FrontPosition, VECTOR *UpVector )
{
	return 0 ;
}

extern int SoundBuffer_Set3DConeAngle_PF( SOUNDBUFFER *Buffer, float InnerAngle, float OuterAngle )
{
	return 0 ;
}

extern int SoundBuffer_Set3DConeVolume_PF( SOUNDBUFFER *Buffer, float InnerAngleVolume, float OuterAngleVolume )
{
	return 0 ;
}

extern int SoundBuffer_Refresh3DSoundParam_PF(	SOUNDBUFFER *Buffer, int AlwaysFlag )
{
	// 終了
	return 0 ;
}

extern int SoundBuffer_SetReverbParam_PF( SOUNDBUFFER *Buffer, SOUND3D_REVERB_PARAM *Param )
{
	return 0 ;
}

extern int SoundBuffer_SetPresetReverbParam_PF( SOUNDBUFFER *Buffer, int PresetNo )
{
	return 0 ;
}





















// ＭＩＤＩ演奏終了時呼ばれるコールバック関数
extern int MidiCallBackProcess( void )
{
	return 0 ;
}






// ALデバイスとALコンテキストをセットする
extern void SoundSystem_iOS_SetALDeviceAndALContext( void *ALCDevice, void *ALCContext )
{
	SoundSysData.PF.ALCdeviceObject = ( ALCdevice * )ALCDevice ;
	SoundSysData.PF.ALCcontectObject = ( ALCcontext * )ALCContext ;
}








// 再生中の ALBuffer を再作成して再再生する
extern void SoundSystem_iOS_ALBuffer_RePlay( void )
{
	SOUNDBUFFER *RePlayBufferFirst ;
	SOUNDBUFFER *Buffer ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &SoundSysData.PF.PlaySoundBufferCriticalSection ) ;

	// 再生中のサウンドバッファを保存
	RePlayBufferFirst = SoundSysData.PF.PlaySoundBuffer ;
	Buffer = RePlayBufferFirst ;
	while( Buffer != NULL )
	{
		Buffer->PF.RePlaySoundBufferPrev = Buffer->PF.PlaySoundBufferPrev ;
		Buffer->PF.RePlaySoundBufferNext = Buffer->PF.PlaySoundBufferNext ;

		Buffer = Buffer->PF.PlaySoundBufferNext ;
	}

	// 再生中のサウンドバッファの再生を停止
	Buffer = RePlayBufferFirst ;
	while( Buffer != NULL )
	{
		SoundBuffer_Stop( Buffer ) ;

		Buffer = Buffer->PF.RePlaySoundBufferNext ;
	}

	// 再生中のサウンドバッファを再度再生
	Buffer = RePlayBufferFirst ;
	while( Buffer != NULL )
	{
		SoundBuffer_Play( Buffer, Buffer->Loop ) ;

		Buffer = Buffer->PF.RePlaySoundBufferNext ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &SoundSysData.PF.PlaySoundBufferCriticalSection ) ;
}










#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_SOUND


// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		WindowsOS用サウンドプログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_SOUNDWIN_H
#define DX_SOUNDWIN_H

#include "../DxCompileConfig.h"

#ifndef DX_NON_SOUND

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"
#include "DxDirectX.h"
#include "DxUseCStrmBaseFilter.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// ストリーム関係の定義( Windows環境はHDDがスリープするので、ファイルから直接再生する場合はバッファを多めに確保 )
#define STS_BUFSEC_FILE						(256 * 13)			// ストリーム風サウンド再生時に確保するサウンドバッファの時間( 単位は STS_DIVNUM分の1秒 )( ファイルからのストリーム再生用 )
#define STS_ADVANCECOPYSEC_FILE				(256 * 12 + 192)	// ストリーム風サウンド再生時に音声データを先行展開しておく最大時間( 単位は STS_DIVNUM分の1秒 )( ファイルからのストリーム再生用 )
#define STS_MINADVANCECOPYSEC_FILE			(256 * 12)			// ストリーム風サウンド再生時に音声データを先行展開しておく最小時間( 単位は STS_DIVNUM分の1秒 )( ファイルからのストリーム再生用 )

#define D_X3DAUDIO_INPUTCHANNELS			(8)				// X3DAudio の計算で使用する最大入力チャンネル数

#ifndef DX_NON_ASIO
#define ASIO_DRV_DESC						L"description"
#define ASIO_PATH							L"software\\asio"
#define ASIO_MAX_DRIVER_NUM					(32)
#define ASIO_MAX_BUFFER_NUM					(16)
#define ASIO_MAX_PATH_LEN					(512)
#define ASIO_MAX_DRVNAME_LEN				(256)
#define ASIO_MAX_DRVNAME2_LEN				(32)
#endif // DX_NON_ASIO

// 構造体定義 --------------------------------------------------------------------

// WASAPI用通知処理用クラス
class D_CMMNotificationClient : public D_IMMNotificationClient
{
	LONG _cRef ;
	D_IMMDeviceEnumerator *_pEnumerator ;

public:
	D_CMMNotificationClient() ;
	~D_CMMNotificationClient() ;

	// IUnknown methods -- AddRef, Release, and QueryInterface

	ULONG	__stdcall AddRef					( void ) ;
	ULONG	__stdcall Release					( void ) ;
	HRESULT	__stdcall QueryInterface			( REFIID riid, VOID **ppvInterface ) ;

	// Callback methods for device-event notifications.

	HRESULT	__stdcall OnDefaultDeviceChanged	( D_EDataFlow flow, D_ERole role, LPCWSTR pwstrDeviceId ) ;
	HRESULT	__stdcall OnDeviceAdded				( LPCWSTR pwstrDeviceId ) ;
	HRESULT	__stdcall OnDeviceRemoved			( LPCWSTR pwstrDeviceId ) ;
	HRESULT	__stdcall OnDeviceStateChanged		( LPCWSTR pwstrDeviceId, DWORD dwNewState ) ;
	HRESULT	__stdcall OnPropertyValueChanged	( LPCWSTR pwstrDeviceId, const D_PROPERTYKEY key ) ;
} ;

// XAudio2用コールバック処理用構造体
struct SOUNDBUFFER_CALLBACK : public D_IXAudio2VoiceCallback
{
	virtual void    __stdcall OnVoiceProcessingPassStart( DWORD BytesRequired ) ;
	virtual void    __stdcall OnVoiceProcessingPassEnd	() ;
	virtual void    __stdcall OnStreamEnd				() ;
	virtual void    __stdcall OnBufferStart				( void* pBufferContext ) ;
	virtual void    __stdcall OnBufferEnd				( void* pBufferContext ) ;
	virtual void    __stdcall OnLoopEnd					( void* pBufferContext ) ;
	virtual void    __stdcall OnVoiceError				( void* pBufferContext, HRESULT Error ) ;

	struct SOUNDBUFFER			*Buffer ;
} ;

// サウンドバッファ環境依存情報構造体
struct SOUNDBUFFER_PF
{
	union
	{
		D_IDirectSoundBuffer 		*DSBuffer ;				// DirectSoundBuffer
		D_IXAudio2SourceVoice		*XA2SourceVoice ;		// XAudio2SourceVoice
		D_IXAudio2_8SourceVoice		*XA2_8SourceVoice ;		// XAudio2_8SourceVoice
	} ;
	union
	{
		D_IXAudio2SubmixVoice		*XA2SubmixVoice ;		// XAudio2SubmixVoice
		D_IXAudio2_8SubmixVoice		*XA2_8SubmixVoice ;		// XAudio2_8SubmixVoice
	} ;
	D_IUnknown					*XA2ReverbEffect ;		// リバーブエフェクト
	SOUNDBUFFER_CALLBACK		*XA2Callback ;			// XAudio2用のコールバック

	union
	{
		D_XAUDIO2FX_REVERB_PARAMETERS    XAudio2ReverbParameter ;	// リバーブ計算用パラメータ
		D_XAUDIO2FX_REVERB_PARAMETERS2_8 XAudio2_8ReverbParameter ;	// リバーブ計算用パラメータXAudio2.8用
	} ;

	int							XAudioChannels ;		// XAudio 側でのチャンネル数

	// ３Ｄサウンド再生用データ
	D_X3DAUDIO_EMITTER			X3DAudioEmitterData ;									// ３Ｄサウンド用音源情報
	D_X3DAUDIO_CONE				X3DAudioEmitterConeData ;								// ３Ｄサウンド用音源情報で使用するコーン情報
	float						X3DAudioEmitterChannelAzimuths[ D_X3DAUDIO_INPUTCHANNELS ] ;	// ３Ｄサウンド用音源情報で使用するチャンネル位置テーブル
} ;

// サウンドシステム用環境依存データ構造体
struct SOUNDSYSTEMDATA_PF
{
	D_IDirectMusicLoader8		*DirectMusicLoaderObject ;		// DirectMusicLoader8 オブジェクト
	D_IDirectMusicPerformance8	*DirectMusicPerformanceObject ;	// DirectMusicPerformance8 オブジェクト

	HMODULE						XAudio2_8DLL ;					// XAudio2_8.dll
	HRESULT						( WINAPI *XAudio2CreateFunc )( D_IXAudio2_8 ** ppXAudio2, DWORD Flags, D_XAUDIO2_PROCESSOR XAudio2Processor );
	HRESULT						( WINAPI *CreateAudioVolumeMeterFunc )( D_IUnknown** ppApo ) ;
	HRESULT						( WINAPI *CreateAudioReverbFunc )( D_IUnknown** ppApo ) ;

	HMODULE						X3DAudioDLL ;					// X3DAudio.dll
	void						( __cdecl *X3DAudioInitializeFunc )( DWORD SpeakerChannelMask, float SpeedOfSound, D_X3DAUDIO_HANDLE Instance ) ;
	void						( __cdecl *X3DAudioCalculateFunc )( const D_X3DAUDIO_HANDLE Instance, const D_X3DAUDIO_LISTENER* pListener, const D_X3DAUDIO_EMITTER* pEmitter, DWORD Flags, D_X3DAUDIO_DSP_SETTINGS* pDSPSettings ) ;

	union
	{
		D_IXAudio2				*XAudio2Object ;				// XAudio2オブジェクト
		D_IXAudio2_8			*XAudio2_8Object ;				// XAudio2_8オブジェクト
		D_IDirectSound			*DirectSoundObject ;			// ＤｉｒｅｃｔＳｏｕｎｄオブジェクト
		DWORD_PTR				InitializeFlag ;				// 自前ミキシング用初期化フラグ
	} ;

	D_XAUDIO2FX_REVERB_PARAMETERS    XAudio2ReverbParameters[ D_XAUDIO2FX_PRESET_NUM ] ;	// リバーブ計算用パラメータ
	D_XAUDIO2FX_REVERB_PARAMETERS2_8 XAudio2_8ReverbParameters[ D_XAUDIO2FX_PRESET_NUM ] ;	// リバーブ計算用パラメータXAudio2.8用
	D_IXAudio2MasteringVoice	*XAudio2MasteringVoiceObject ;	// XAudio2MasteringVoiceオブジェクト
	D_IXAudio2_8MasteringVoice	*XAudio2_8MasteringVoiceObject ;// XAudio2_8MasteringVoiceオブジェクト
	D_XAUDIO2_EFFECT_CHAIN		XAudio2MasteringEffectChain ;	// マスターボイス用のエフェクトチェイン

	DWORD						XAudio2OutputChannelMask ;		// 出力チャンネルマスク

	D_X3DAUDIO_HANDLE			X3DAudioInstance ;				// ３Ｄサウンド計算用インスタンスハンドル
	D_X3DAUDIO_CONE				X3DAudioListenerConeData ;		// ３Ｄサウンドのリスナー情報に使用するコーン情報
	D_X3DAUDIO_LISTENER			X3DAudioListenerData ;			// ３Ｄサウンドのリスナー情報

	D_WAVEFORMATEXTENSIBLE		SelfMixingFormatEx ;			// 自前ミキシングフォーマットの WAVEFORMATEXTENSIBLE 版

	D_IMMDeviceEnumerator		*IMMDeviceEnumerator ;			// マルチメディアデバイス列挙インターフェース
	D_CMMNotificationClient		*CMMNotificationClient ;		// デバイスのコールバックインターフェイス
	D_IMMDevice					*IMMDevice ;					// デバイスインターフェース
	D_IAudioClient				*AudioClient ;					// オーディオクライアントインターフェース
	D_IAudioClock				*AudioClock ;					// オーディオクロックインターフェース
	D_IAudioRenderClient		*AudioRenderClient ;			// レンダークライアントインターフェース
	HANDLE						WASAPI_Event ;					// WASAPI用イベントハンドル
	HANDLE						WASAPI_ThreadHandle ;			// WASAPI用スレッドハンドル
	BOOL						WASAPI_ThreadLoop ;				// WASAPI用スレッド処理中か
	D_REFERENCE_TIME			WASAPI_DevicePeriod ;			// WASAPIのデバイスピリオド
	D_REFERENCE_TIME			WASAPI_DefaultDevicePeriod ;	// WASAPIのデフォルトデバイスピリオド
	D_REFERENCE_TIME			WASAPI_MinimumDevicePeriod ;	// WASAPIの最小デバイスピリオド
	DWORD						WASAPI_BufferSamples ;			// WASAPIのバッファのサンプル数
	int							WASAPI_IsExclusiveFlag ;		// WASAPIを排他モードで使用するかどうかのフラグ( TRUE:排他モード  FALSE:共有モード )
	int							WASAPI_DefaultDeviceChange ;	// WASAPIのデフォルトデバイスが変化したら立つフラグ

	int							WASAPI_EnableUserParam ;		// WASAPIのユーザー設定パラメータが有効か
	int							WASAPI_UserParam_DevicePeriod ;	// WASAPIのデバイスピリオド
	int							WASAPI_UserParam_SamplePerSec ;	// WASAPIのサンプリングレート

#ifndef DX_NON_ASIO
	long						ASIO_DriverNum ;				// ASIOドライバーの数
	int							ASIO_DriverUseIndex ;			// 使用するASIOドライバ
	char						ASIO_DriverPaths[ ASIO_MAX_DRIVER_NUM ][ ASIO_MAX_PATH_LEN ] ;		// ASIOドライバーのパス
	wchar_t						ASIO_DriverNames[ ASIO_MAX_DRIVER_NUM ][ ASIO_MAX_DRVNAME_LEN ] ;	// ASIOドライバーの名前
	char						ASIO_DriverNames2[ ASIO_MAX_DRIVER_NUM ][ ASIO_MAX_DRVNAME2_LEN ] ;	// ASIOドライバーの名前その２
	long						ASIO_DriverVersion[ ASIO_MAX_DRIVER_NUM ] ;	// ASIOドライバーのバージョン
	GUID						ASIO_DriverGUID[ ASIO_MAX_DRIVER_NUM ] ;	// ASIOドライバのGUID
	D_IASIO						*ASIO_Driver[ ASIO_MAX_DRIVER_NUM ] ;		// ASIOドライバオブジェクト
	long						ASIO_BufferMiniNum ;			// バッファの最小サイズ
	long						ASIO_BufferMaxiNum ;			// バッファの最大サイズ
	long						ASIO_Preferred ;				// 優先バッファサイズ（サイズパフォーマンスとハードウェアの要件に最適）
	long						ASIO_BufferSize ;				// 使用する実際のバッファサイズ
	long						ASIO_Granularity ;				// バッファサイズの粒度
	D_ASIOBufferInfo			ASIO_BufferInfo[ ASIO_MAX_BUFFER_NUM ] ;	// バッファ情報
	int							ASIO_EnableUserParam ;			// ASIOのユーザー設定パラメータが有効か
	int							ASIO_UserParam_BufferSize ;		// ASIOのデバイスピリオド
	int							ASIO_UserParam_SamplePerSec ;	// ASIOのサンプリングレート
#endif // DX_NON_ASIO

	int							waveOut_EnableUserParam ;			// waveOutのユーザー設定パラメータが有効か
	int							waveOut_UserParam_BufferSamples ;	// waveOutのバッファーサイズ
	int							waveOut_UserParam_SamplePerSec ;	// waveOutのサンプリングレート
	DWORD						waveOut_BufferSamples ;				// WASAPIのバッファのサンプル数
	HWAVEOUT					waveOut_WaveOut ;					// waveOutハンドル
	WAVEHDR						waveOut_WaveHeader[ 2 ] ;			// waveOutの波形出力用構造体
	int							waveOut_EndFlag ;					// waveOutを終了するかどうかのフラグ

	D_IDirectSoundBuffer		*PrimarySoundBuffer ;			// プライマリサウンドバッファ

	D_IDirectSoundBuffer		*NoSoundBuffer ;				// 無音バッファ

	int							EnableXAudioFlag ;				// XAudioを使用するかどうかのフラグ( TRUE:使用する  FALSE:使用しない ) 
	int							DisableWASAPIFlag ;				// WASAPIを使用しないかどうかのフラグ( TRUE:使用する  FALSE:使用しない ) 
#ifndef DX_NON_ASIO
	int							EnableASIOFlag ;				// ASIOを使用するかどうかのフラグ( TRUE:使用する  FALSE:使用しない ) 
#endif // DX_NON_ASIO
	int							EnableMMEwaveOutFlag ;			// waveOutを使用するかどうかのフラグ( TRUE:使用する  FALSE:使用しない ) 
	int							UseSoftwareMixing ;				// ソフトウエアミキシングを行うかどうかのフラグ( TRUE:ソフトウエア  FALSE:ハードウエア )

	HANDLE						StreamSoundThreadHandle ;		// ストリームサウンドを再生する処理を行うスレッドのハンドル
	DWORD						StreamSoundThreadID ;			// ストリームサウンドを再生する処理を行うスレッドのＩＤ
	DWORD						StreamSoundThreadEndFlag ;		// ストリームサウンドを再生する処理を終了するかどうかのフラグ
} ;

// ＭＩＤＩデータ環境依存情報
struct MIDIHANDLEDATA_PF
{
	D_IDirectMusicSegment8		*DirectMusicSegmentObject ;		// DirectMusicSegment8 オブジェクト
} ;

// ＭＩＤＩシステム用環境依存データ構造体
struct MIDISYSTEMDATA_PF
{
	UINT						MidiDeviceID ;					// ＭＩＤＩ演奏時のデバイスＩＤ
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

extern	int		MidiCallBackProcess( void ) ;																			// ＭＩＤＩ演奏終了時呼ばれるコールバック関数


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_SOUND

#endif // DX_SOUNDWIN_H

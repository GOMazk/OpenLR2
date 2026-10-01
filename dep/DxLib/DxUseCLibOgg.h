// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		標準Ｃライブラリ使用コード　Ogg関係ヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_USECLIBOGG_H
#define DX_USECLIBOGG_H

// インクルード ------------------------------------------------------------------
#include "DxCompileConfig.h"

#ifndef DX_NON_OGGTHEORA
#include "DxMovie.h"
#endif

#if !defined( DX_NON_OPUS ) || !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_MOVIE )
#include "DxSoundConvert.h"
#endif

#include "DxFile.h"
#include "DxBaseFunc.h"

#if defined( DX_NON_OGGVORBIS ) && defined( DX_NON_OGGTHEORA )
#else
    #include "vorbis/codec.h"
    #include "vorbis/vorbisfile.h"
    
    #include "os.h"
    #include "misc.h"
#endif

#ifndef DX_NON_OGGVORBIS
	//#include "codec.h"
	#include "vorbis/vorbisfile.h"
	
    #include "vorbis/codec.h"
#endif

#ifndef DX_NON_OGGTHEORA
	//#include "codec.h"
	#include "vorbis/vorbisfile.h"
	
    #include "theora/theora.h"
#endif

#ifndef DX_NON_OPUS
	#include "opusfile.h"
#endif

//namespace DxLib
//{

// マクロ定義 --------------------------------------------------------------------

// Theora デコードスレッドの状態
#define THEORAT_STATE_IDLE		(0)				// 何もしていない
#define THEORAT_STATE_DECODE	(1)				// フレームデコード中
#define THEORAT_STATE_SEEK		(2)				// シーク中
#define THEORAT_STATE_EXIT		(3)				// スレッド終了
#define THEORAT_STATE_STOP		(4)				// スレッドストップ中

// 構造体定義 --------------------------------------------------------------------

#ifndef DX_NON_OGGTHEORA
// Ogg ページヘッダ( ヘッダのサイズの求め方は Segments + 27 )
typedef struct tagOGG_HEADER
{
	char					ID[ 4 ] ;					// 識別ID "OggS"
	BYTE					Version ;					// バージョン
	BYTE					HeadType ;					// ヘッダータイプ
	BYTE					GranulePosition[ 8 ] ;		// アドレス( リトルエンディアン )
	BYTE					SerialNumber[ 4 ] ;			// シリアルナンバー
	BYTE					PageNumber[ 4 ] ;			// ページナンバー
	BYTE					CRCCehckSum[ 4 ] ;			// CRCチェックサム
	BYTE					Segments ;					// セグメント数
	BYTE					SegmentTable[ 1 ] ;			// セグメントテーブル
} OGG_HEADER ;

// ページの情報
typedef struct tagTHEORA_PAGEINFO
{
	int						StreamAddres ;				// ストリーム上のアドレス
	int						FrameAddres ;				// Theora 動画中のフレーム
	int						KeyFrame ;					// キーフレームが含まれているかどうか
	int						FrameNum ;					// ページに含まれるフレーム数
} THEORA_PAGEINFO ;

// ストックフレームの情報
typedef struct tagTHEORA_STOCKFRAME
{
	volatile int			UseFlag ;					// 使用しているかどうか( 1:使用している  0:使用していない )
	volatile int			FrameNumber ;				// 保存している画像のフレーム

	void					*YBuffer ;					// Ｙイメージへのアドレス
	void					*UVBuffer ;					// ＵＶイメージへのアドレス
	int						YWidth, YHeight ;			// Ｙイメージの幅と高さ
	int						YStride ;					// Ｙバッファのピッチ
	int						UVWidth, UVHeight ;			// ＵＶイメージの幅と高さ
	int						UVStride ;					// ＵＶバッファのピッチ
} THEORA_STOCKFRAME ;

// Ogg Theora デコード処理用データ構造体
typedef struct tagDECODE_THEORA
{
#ifndef DX_NON_NAMESPACE
	DxLib::DX_CRITICAL_SECTION	CriticalSection ;		// 複数スレッドでの競合防止用のクリティカルセクション
#else // DX_NON_NAMESPACE
	DX_CRITICAL_SECTION		CriticalSection ;			// 複数スレッドでの競合防止用のクリティカルセクション
#endif // DX_NON_NAMESPACE

	volatile int			ThreadState ;				// スレッドの状態( THEORAT_STATE_IDLE 等 )
	volatile int			ThreadStopRequest ;			// スレッドに止まってほしいときに 1 にする
	volatile int			ThreadExitRequest ;			// スレッドに終了してほしいときに 1 にする
	volatile int			ThreadSeekRequest ;			// スレッドにシークして欲しいときに 1 にする
	volatile int			ThreadEnableCacheRead ;		// スレッドに特にタスクがない場合に先読みを許可するかどうか( 1:許可  0:許可しない )
	volatile int			ThreadSeekFrame  ;			// スレッドにシークして貰うときのシーク先フレーム
	volatile int			ThreadPacketEnd ;			// データを全て読み終わったフラグ
	volatile int			ThreadStandbyTime ;			// 待機状態を維持する目安とする時間
//	volatile HANDLE			DecodeThreadHandle ;		// デコード処理スレッド
//	volatile DWORD			DecodeThreadID ;			// デコード処理スレッドのＩＤ 
#ifndef DX_NON_NAMESPACE
	DxLib::THREAD_INFO		DecodeThreadInfo ;			// デコードスレッド情報
#else // DX_NON_NAMESPACE
	THREAD_INFO				DecodeThreadInfo ;			// デコードスレッド情報
#endif // DX_NON_NAMESPACE

#ifndef DX_NON_NAMESPACE
	DxLib::STREAMDATASHRED	StreamShred ;				// ストリーム関数
#else // DX_NON_NAMESPACE
	STREAMDATASHRED			StreamShred ;				// ストリーム関数
#endif // DX_NON_NAMESPACE
	DWORD_PTR				StreamData ;				// ストリームデータ

	ogg_sync_state			OggSyncState ;				// Ogg ベースデータ
	ogg_page				OggPage ;					// Ogg ページ
	ogg_stream_state		OggTheoraStream ;			// Ogg ストリーム
	ogg_packet				OggPacket ;					// Ogg パケット

	theora_info				TheoraInfo ;				// Theora 情報
	theora_comment			TheoraComment ;				// Theora Comment
	theora_state			TheoraState ;				// Theora State
	int						TheoraSerialNo ;			// Theora データのシリアルナンバー

	THEORA_PAGEINFO			*TheoraPageInfo ;			// ページの情報
	int						TheoraPageInfoNum ;			// ページの情報の数
	int						*TheoraTimeFrameToDataFrame ;	// 再生時間フレームに対応するデータフレームとの対応配列( 再生時間フレームの箇所にデータフレーム番号が入っている )
	unsigned char			*TheoraKeyFrameInfo ;		// キーフレーム情報配列( キーフレームの配列要素が 1 になっている )
	ogg_int64_t				TheoraTotalDataFrame ;		// 総データフレーム数
	int						TheoraTotalTimeFrame ;		// 総再生時間フレーム数
	volatile int			TheoraCurrentTimeFrame ;	// 対外的なカレントタイムフレーム
	volatile int			TheoraCurrentDataFrame ;	// 対外的なカレントデータフレーム
	volatile int			TheoraInCurrentDataFrame ;	// 実際のカレントデータフレーム

	int						NumOfTheoraHeader ;			// Theora のヘッダの数

	volatile THEORA_STOCKFRAME *StockFrame ;			// ストックフレーム
	volatile int			StockFrameMaxNum ;			// ストックフレームの最大数

#ifndef DX_NON_NAMESPACE
	DxLib::BASEIMAGE		BaseImage ;					// カレントフレームが格納されたフレームスタック中のイメージのコピー
#else // DX_NON_NAMESPACE
	BASEIMAGE				BaseImage ;					// カレントフレームが格納されたフレームスタック中のイメージのコピー
#endif // DX_NON_NAMESPACE
	volatile int			BaseImageSetup ;			// カレントフレームの RGB イメージが構築されているかどうか( 1:されている  0:されていない )

	volatile int			NotUseYUVFormatSurface ;	// ＹＵＶフォーマットのサーフェスを使用しないかどうか
	volatile int			ValidYUVSurface ;			// ＹＵＶサーフェスが有効かどうか( TRUE:有効  FALSE:無効 )
	volatile int			YUVSurfaceReleaseRequest ;	// ＹＵＶフォーマットサーフェスの解放依頼フラグ
	volatile DWORD			YUVImageFourCC ;			// ＹＵＶサーフェスに使用されている FourCC フォーマット
	volatile int			YUVImageSetup ;				// カレントフレームの YUV イメージが構築されているかどうか( 1:されている  0:されていない )

	volatile int			NotUseYUVGrHandle ;			// YUVカラーのグラフィックハンドルを使用しないかどうか
	volatile int			YGrHandle ;					// YUVカラーのY成分のみのグラフィックハンドル
	volatile int			UVGrHandle ;				// YUVカラーのUV成分のみのグラフィックハンドル
	volatile int			YUVGrHandleSetup ;			// YUVカラーのグラフィックハンドルのセットアップが完了しているかどうか( 1:完了している  0:完了していない )

	volatile struct DECODE_THEORA_PF *PF ;						// 環境依存データ
} DECODE_THEORA ;

#endif

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------


#ifndef DX_NON_OGGTHEORA

extern	int		TheoraDecode_Thread_LoopProcess( DECODE_THEORA *DT ) ;														// Ogg Theora のデコードスレッドで行うループ処理を行う( 戻り値 2 で終了 )

extern	void	TheoraDecode_CreateSurface( DECODE_THEORA *DT, int ASyncThread ) ;

// 環境依存関数
extern	int		TheoraDecode_CreateSurface_PF(  DECODE_THEORA *DT ) ;														// ハードウエア機能が有効な場合は YUV フォーマットの一時保存用テクスチャを作成する
extern	void	TheoraDecode_ReleaseSurface_PF(	DECODE_THEORA *DT ) ;
extern	int		TheoraDecode_SetupImage_PF( DECODE_THEORA *DT, volatile THEORA_STOCKFRAME *Stock, int ASyncThread ) ;		// カレントフレームのRGB画像を作成する( 戻り値  1:作成された  0:されなかった )
extern	const void * TheoraDecode_GetYUVImage_PF( DECODE_THEORA *DT ) ;														// 一時バッファの YUV フォーマットのテクスチャを得る
extern	int		TheoraDecode_InitializeStream_PF( DECODE_THEORA *DT ) ;														// Ogg Theora の読み込み処理の準備を行う処理の環境依存処理を行う関数

#endif // DX_NON_OGGTHEORA

#ifndef DX_NON_OGGVORBIS

#ifndef DX_NON_NAMESPACE
extern	int		GetOggCommentNumBase( DxLib::STREAMDATA *Stream ) ;
extern	int		GetOggCommentBase( DxLib::STREAMDATA *Stream, int CommentIndex, char *CommentNameBuffer, size_t CommentNameBufferBytes, char *CommentBuffer, size_t CommentBufferBytes ) ;
#else // DX_NON_NAMESPACE
extern	int		GetOggCommentNumBase( STREAMDATA *Stream ) ;
extern	int		GetOggCommentBase( STREAMDATA *Stream, int CommentIndex, char *CommentNameBuffer, size_t CommentNameBufferBytes, char *CommentBuffer, size_t CommentBufferBytes ) ;
#endif // DX_NON_NAMESPACE

#endif // DX_NON_OGGVORBIS

//}

#endif // DX_USECLIBOGG_H

// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用システムプログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_SYSTEMANDROID_H
#define DX_SYSTEMANDROID_H

// インクルード ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <android/sensor.h>
#include <poll.h>
#include <pthread.h>
#include <sched.h>
#include <EGL/egl.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// ソフトがアクティブかどうかをチェックする
#define CheckActiveState()					\
		if( g_AndroidSys.NonActiveRunFlag == FALSE &&\
			( ( g_AndroidSys.ActivityState != DX_ANDR_CMD_START &&\
			  g_AndroidSys.ActivityState != DX_ANDR_CMD_RESUME ) ||\
			  g_AndroidSys.NativeWindow == NULL ) )\
		{\
			DxActiveWait_Android() ;\
		}

// Looper のイベントID
#define DX_LOOPER_ID_MAIN							(1)				// メインスレッドから送るコマンドイベント
#define DX_LOOPER_ID_INPUT							(2)				// 入力イベント
#define DX_LOOPER_ID_SENSOR_ACCELEROMETER			(3)				// 加速度センサーのイベント
#define DX_LOOPER_ID_SENSOR_MAGNETIC_FIELD			(4)				// 磁界センサーのイベント
#define DX_LOOPER_ID_SENSOR_GYROSCOPE				(5)				// ジャイロスコープセンサーのイベント
#define DX_LOOPER_ID_SENSOR_LIGHT					(6)				// 照度センサーのイベント
#define DX_LOOPER_ID_SENSOR_PROXIMITY				(7)				// 近接センサーのイベント
#define DX_LOOPER_ID_SENSOR_PRESSURE				(8)				// 加圧センサーのイベント
#define DX_LOOPER_ID_SENSOR_AMBIENT_TEMPERATURE		(9)				// 温度センサーのイベント

// ソフト実行用スレッドに送るコマンド
#define DX_ANDR_CMD_START							(0)				// onStart が呼ばれた
#define DX_ANDR_CMD_RESUME							(1)				// onResume が呼ばれた
#define DX_ANDR_CMD_PAUSE							(2)				// onPause が呼ばれた
#define DX_ANDR_CMD_STOP							(3)				// onStop が呼ばれた
#define DX_ANDR_CMD_DESTROY							(4)				// onDestroy が呼ばれた
#define DX_ANDR_CMD_GAINED_FOCUS					(5)				// onWindowFocusChanged が呼ばれ、フォーカスONになった
#define DX_ANDR_CMD_LOST_FOCUS						(6)				// onWindowFocusChanged が呼ばれ、フォーカスOFFになった
#define DX_ANDR_CMD_WINDOW_CHANGED					(7)				// ウインドウの変更
#define DX_ANDR_CMD_INPUT_CHANGED					(8)				// 入力イベントを受け取るキューの変更

// 構造体定義 --------------------------------------------------------------------

struct ANDROID_SENSOR_INFO
{
	const ASensor				*Sensor ;							// センサー
	ASensorEventQueue			*SensorEventQueue ;					// センサーイベントキュー
	ASensorEvent				SensorEvent ;						// センサーイベント
} ;

struct DXLIB_ANDROID_SYSTEMINFO
{
	volatile int				NativeActivityBufferLength ;		// NativeActivity アドレス格納用バッファに格納できる要素数
	volatile int				NativeActivityNum ;					// NativeActivity アドレス格納用バッファに格納されている要素の数
	ANativeActivity				**NativeActivityBuffer ;			// NativeActivity アドレス格納用バッファ
	ANativeActivity				*NativeActivity ;					// ソフトの Activity
	pthread_mutex_t				NativeActivityMutex ;				// NativeActivity アクセス時用ミューテックス
	ALooper						*Looper ;							// ソフトのイベント処理用 Looper

	volatile AInputQueue		*InputQueue ;						// 入力イベントを受け取るためのキュー
	volatile AInputQueue		*NewInputQueue ;					// 新しい入力イベントを受け取るためのキュー

	int32_t ( *UserInputEventCallbackFunction )( AInputEvent *, void * ) ;	// 入力イベント発生時に呼ばれるコールバック関数
	void *						UserInputEventCallbackFunctionData ;// 入力イベント発生時に呼ばれるコールバック関数に渡すアドレス

	volatile ANativeWindow		*NativeWindow ;						// ソフトのウインドウ
	volatile ANativeWindow		*NewNativeWindow ;					// 新しいソフトのウインドウ

	volatile int				ActivityState ;						// 現在の Activity の状態( DX_APP_CMD_RESUME, DX_APP_CMD_START, DX_APP_CMD_PAUSE, DX_APP_CMD_STOP のいずれか )
	volatile int				DestroyRequested ;					// onDestroy コマンドをソフト実行用スレッドが受け取ったかどうか( 1:受け取った  0:受け取っていない )
	volatile int				DestroyRequestedTime ;				// onDestroy コマンドが発生した時刻
	volatile int				onDestroyEnd ;						// onDestroy コマンドの処理が終了したかどうか( 0:終了していない  1:終了した )

	volatile int				AppKillProcessFlag ;				// アプリ終了時にプロセスを完全終了するかどうかのフラグ

	volatile int				MutexLockIndex ;
	pthread_mutex_t				Mutex ;								// メインスレッドとソフト実行用スレッドでお互いに使用する変数アクセス時等に使用するミューテックス
	pthread_cond_t				Cond ;								// メインスレッドとソフト実行用スレッドでお互いの処理の実行完了待ちなどに使用する条件変数

	volatile int				NonActiveRunFlag ;					// アプリがアクティブではなくても処理を実行するかどうかのフラグ
	volatile int				IsFullScreen ;						// フルスクリーンかどうか( TRUE:フルスクリーン  FALSE:タイトルバーとナビゲーションバーあり )

	volatile int				MessageRead ;						// メインスレッドからソフト実行用スレッドへのメッセージ受け取り用ファイルディスクリプタ
	volatile int				MessageWrite ;						// メインスレッドからソフト実行用スレッドへのメッセージ書き込み用ファイルディスクリプタ

	pthread_t					SoftThread ;						// ソフト実行用スレッド
	volatile int				SoftThreadRunning ;					// ソフト実行用スレッドの実行が開始されたかどうか( 0:開始されていない  1:開始された )
	volatile int				SoftThreadDestroyedStart ;			// ソフト実行用スレッドの終了処理を開始したかどうか( 0:開始されていない  1:開始された )
	volatile int				SoftThreadDestroyed ;				// ソフト実行用スレッドが終了したかどうか( 0:終了していない  1:終了した )
	volatile int				SoftThreadWait ;					// ソフト実行用スレッドが待ち状態かどうか( 1:待ち状態  0:実行状態 )

	volatile int				SoundAndMoviePause ;				// ウインドウが非アクティブになりサウンドと動画が一時停止しているかどうか( TRUE:一時停止している  FALSE:一時停止していない )

	volatile void				( *LostFocusCallbackFunction )( void *Data ) ;		// アプリのフォーカスが失われた際に呼ばれるコールバック関数
	volatile void				*LostFocusCallbackFunctionData ;					// アプリのフォーカスが失われた際に呼ばれるコールバック関数に渡すポインタ
	volatile void				( *GainedFocusCallbackFunction )( void *Data ) ;	// アプリのフォーカスが得られた際に呼ばれるコールバック関数
	volatile void				*GainedFocusCallbackFunctionData ;					// アプリのフォーカスが得られた際に呼ばれるコールバック関数に渡すポインタ

	ASensorManager				*SensorManager ;					// センサーマネージャー
	ANDROID_SENSOR_INFO			SensorInfos[ DX_ANDROID_SENSOR_NUM ] ;	// 各センサーの情報

	jobject						object_EditText ;					// 文字列入力を行うための EditText
	jobject						object_AlertDialog_Builder ;		// 文字列入力ダイアログの AlertDialog.Builder
	jobject						object_Dialog ;						// 文字列入力ダイアログの Dialog
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

extern DXLIB_ANDROID_SYSTEMINFO g_AndroidSys ;
extern int g_AndroidRunFlag ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// アクティブになるまで何もしない
extern void DxActiveWait_Android( void ) ;

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_SYSTEMANDROID_H

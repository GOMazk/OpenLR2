// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用システムプログラム
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "DxSystemAndroid.h"

// インクルード ------------------------------------------------------------------
#include "DxGraphicsAndroid.h"
#include "DxGraphicsFilterAndroid.h"
#include "DxLogAndroid.h"
#include "DxMaskAndroid.h"
#include "DxModelAndroid.h"
#include "DxJavaAndroid.h"
#include "../DxLib.h"
#include "../DxArchive_.h"
#include "../DxSystem.h"
#include "../DxGraphics.h"
#include "../DxModel.h"
#include "../DxMask.h"
#include "../DxFile.h"
#include "../DxNetwork.h"
#include "../DxInputString.h"
#include "../DxInput.h"
#include "../DxSound.h"
#include "../DxSoundConvert.h"
#include "../DxBaseImage.h"
#include "../DxSoftImage.h"
#include "../DxMovie.h"
#include "../DxFont.h"
#include "../DxLog.h"
#include "../DxASyncLoad.h"
#include "../DxLive2DCubism4.h"
#include "../DxUseCLib.h"
#include "../DxHandle.h"
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <android/window.h>
#include <android/log.h>
#include <GLES2/gl2.h>

#include "../DxBaseFunc.h"
#include "../DxMemory.h"



#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "threaded_app", __VA_ARGS__))

#ifndef NDEBUG
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "threaded_app", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif

#define TIME_DISTANCE( now, time )			( (now) < (time) ? 0x7fffffff - (time) + (now) : (now) - (time) )

// 構造体定義 --------------------------------------------------------------------

// 各センサーの固定情報
struct ANDROID_SENSOR_BASE_INFO
{
	int TypeID ;				// タイプID ( ASENSOR_TYPE_ACCELEROMETER など )
	int LooperEventID ;			// Looper のイベントID( DX_LOOPER_ID_SENSOR_ACCELEROMETER など )
} ;

// テーブル-----------------------------------------------------------------------

static ANDROID_SENSOR_BASE_INFO g_AndroidSensorBaseInfos[ DX_ANDROID_SENSOR_NUM ] =
{
	{ ASENSOR_TYPE_ACCELEROMETER,				DX_LOOPER_ID_SENSOR_ACCELEROMETER			},				// 加速度センサー
	{ ASENSOR_TYPE_MAGNETIC_FIELD,				DX_LOOPER_ID_SENSOR_MAGNETIC_FIELD			},				// 磁界センサー
	{ ASENSOR_TYPE_GYROSCOPE,					DX_LOOPER_ID_SENSOR_GYROSCOPE				},				// ジャイロスコープセンサー
	{ ASENSOR_TYPE_LIGHT,						DX_LOOPER_ID_SENSOR_LIGHT					},				// 照度センサー
	{ ASENSOR_TYPE_PROXIMITY,					DX_LOOPER_ID_SENSOR_PROXIMITY				},				// 近接センサー
	{ 6/*ASENSOR_TYPE_PRESSURE*/,				DX_LOOPER_ID_SENSOR_PRESSURE				},				// 加圧センサー
	{ 13/*ASENSOR_TYPE_AMBIENT_TEMPERATURE*/,	DX_LOOPER_ID_SENSOR_AMBIENT_TEMPERATURE		},				// 温度センサー
} ;

// 内部大域変数宣言 --------------------------------------------------------------

DXLIB_ANDROID_SYSTEMINFO g_AndroidSys ;
int g_AndroidRunFlag ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// 文字列入力ダイアログを表示する
static int StartInputStringDialogStatic( JNIEnv *env, const TCHAR *Title ) ;

// GetAndroidMetaData_String や GetAndroidMetaData_Boolean のミューテックスのロックと JavaVM とスレッドの関連付け以外を抜き出した関数
static int GetAndroidMetaData_Base( JNIEnv *env, const TCHAR *ValueName, TCHAR *StringBuffer, int StringBufferBytes, int *Boolean ) ;

// プログラム --------------------------------------------------------------------

// UTF16LE の書式文字列と UTF8 のパラメータ文字列をログ出力する
static void OutputAndroidOSInfo_LogAddUTF8( const char *UTF16LEFormatStr, const char *UTF8Str )
{
	char TempStr[ 1024 ] ;

	ConvString( UTF8Str, -1, DX_CHARCODEFORMAT_UTF8, TempStr, sizeof( TempStr ), DX_CHARCODEFORMAT_UTF16LE ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( UTF16LEFormatStr, TempStr )) ;
}

// ＯＳ情報を出力する
extern int OutputAndroidOSInfo( JNIEnv *env )
{
	// 戻り値の初期値は -1
	int res = -1 ;

	if( env == NULL )
	{
		return res ;
	}

	jclass class_Build        = env->FindClass( "android/os/Build" ) ;
	jclass class_BuildVERSION = env->FindClass( "android/os/Build$VERSION" ) ;
	if( class_Build        != NULL &&
		class_BuildVERSION != NULL )
	{
		jfieldID field_BOARD		= env->GetStaticFieldID( class_Build,		 "BOARD"		, "Ljava/lang/String;"	) ;
		jfieldID field_BOOTLOADER	= env->GetStaticFieldID( class_Build,		 "BOOTLOADER"	, "Ljava/lang/String;"	) ;
		jfieldID field_BRAND		= env->GetStaticFieldID( class_Build,		 "BRAND"		, "Ljava/lang/String;"	) ;
		jfieldID field_CPU_ABI		= env->GetStaticFieldID( class_Build,		 "CPU_ABI"		, "Ljava/lang/String;"	) ;
		jfieldID field_CPU_ABI2		= env->GetStaticFieldID( class_Build,		 "CPU_ABI2"		, "Ljava/lang/String;"	) ;
		jfieldID field_DEVICE		= env->GetStaticFieldID( class_Build,		 "DEVICE"		, "Ljava/lang/String;"	) ;
		jfieldID field_DISPLAY		= env->GetStaticFieldID( class_Build,		 "DISPLAY"		, "Ljava/lang/String;"	) ;
		jfieldID field_FINGERPRINT	= env->GetStaticFieldID( class_Build,		 "FINGERPRINT"	, "Ljava/lang/String;"	) ;
		jfieldID field_HARDWARE		= env->GetStaticFieldID( class_Build,		 "HARDWARE"		, "Ljava/lang/String;"	) ;
		jfieldID field_HOST			= env->GetStaticFieldID( class_Build,		 "HOST"			, "Ljava/lang/String;"	) ;
		jfieldID field_ID			= env->GetStaticFieldID( class_Build,		 "ID"			, "Ljava/lang/String;"	) ;
		jfieldID field_MANUFACTURER	= env->GetStaticFieldID( class_Build,		 "MANUFACTURER"	, "Ljava/lang/String;"	) ;
		jfieldID field_MODEL		= env->GetStaticFieldID( class_Build,		 "MODEL"		, "Ljava/lang/String;"	) ;
		jfieldID field_PRODUCT		= env->GetStaticFieldID( class_Build,		 "PRODUCT"		, "Ljava/lang/String;"	) ;
		jfieldID field_RADIO		= env->GetStaticFieldID( class_Build,		 "RADIO"		, "Ljava/lang/String;"	) ;
		jfieldID field_TAGS			= env->GetStaticFieldID( class_Build,		 "TAGS"			, "Ljava/lang/String;"	) ;
		jfieldID field_TIME			= env->GetStaticFieldID( class_Build,		 "TIME"			, "J"					) ;
		jfieldID field_TYPE			= env->GetStaticFieldID( class_Build,		 "TYPE"			, "Ljava/lang/String;"	) ;
		jfieldID field_UNKNOWN		= env->GetStaticFieldID( class_Build,		 "UNKNOWN"		, "Ljava/lang/String;"	) ;
		jfieldID field_USER			= env->GetStaticFieldID( class_Build,		 "USER"			, "Ljava/lang/String;"	) ;
		jfieldID field_SDK_INT		= env->GetStaticFieldID( class_BuildVERSION, "SDK_INT"		, "I"					) ;
		if( field_BOARD				!= NULL &&
			field_BOOTLOADER		!= NULL &&
			field_BRAND				!= NULL &&
			field_CPU_ABI			!= NULL &&
			field_CPU_ABI2			!= NULL &&
			field_DEVICE			!= NULL &&
			field_DISPLAY			!= NULL &&
			field_FINGERPRINT		!= NULL &&
			field_HARDWARE			!= NULL &&
			field_HOST				!= NULL &&
			field_ID				!= NULL &&
			field_MANUFACTURER		!= NULL &&
			field_MODEL				!= NULL &&
			field_PRODUCT			!= NULL &&
			field_RADIO				!= NULL &&
			field_TAGS				!= NULL &&
			field_TIME				!= NULL &&
			field_TYPE				!= NULL &&
			field_UNKNOWN			!= NULL &&
			field_USER				!= NULL &&
			field_SDK_INT			!= NULL )
		{
			jstring stringfield_BOARD			= ( jstring )env->GetStaticObjectField( class_Build,		field_BOARD			) ;
			jstring stringfield_BOOTLOADER		= ( jstring )env->GetStaticObjectField( class_Build,		field_BOOTLOADER	) ;
			jstring stringfield_BRAND			= ( jstring )env->GetStaticObjectField( class_Build,		field_BRAND			) ;
			jstring stringfield_CPU_ABI			= ( jstring )env->GetStaticObjectField( class_Build,		field_CPU_ABI		) ;
			jstring stringfield_CPU_ABI2		= ( jstring )env->GetStaticObjectField( class_Build,		field_CPU_ABI2		) ;
			jstring stringfield_DEVICE			= ( jstring )env->GetStaticObjectField( class_Build,		field_DEVICE		) ;
			jstring stringfield_DISPLAY			= ( jstring )env->GetStaticObjectField( class_Build,		field_DISPLAY		) ;
			jstring stringfield_FINGERPRINT		= ( jstring )env->GetStaticObjectField( class_Build,		field_FINGERPRINT	) ;
			jstring stringfield_HARDWARE		= ( jstring )env->GetStaticObjectField( class_Build,		field_HARDWARE		) ;
			jstring stringfield_HOST			= ( jstring )env->GetStaticObjectField( class_Build,		field_HOST			) ;
			jstring stringfield_ID				= ( jstring )env->GetStaticObjectField( class_Build,		field_ID			) ;
			jstring stringfield_MANUFACTURER	= ( jstring )env->GetStaticObjectField( class_Build,		field_MANUFACTURER	) ;
			jstring stringfield_MODEL			= ( jstring )env->GetStaticObjectField( class_Build,		field_MODEL			) ;
			jstring stringfield_PRODUCT			= ( jstring )env->GetStaticObjectField( class_Build,		field_PRODUCT		) ;
			jstring stringfield_RADIO			= ( jstring )env->GetStaticObjectField( class_Build,		field_RADIO			) ;
			jstring stringfield_TAGS			= ( jstring )env->GetStaticObjectField( class_Build,		field_TAGS			) ;
//			jlong   longfield_TIME				=            env->GetStaticLongField(   class_Build,		field_TIME			) ;
			jstring stringfield_TYPE			= ( jstring )env->GetStaticObjectField( class_Build,		field_TYPE			) ;
			jstring stringfield_UNKNOWN			= ( jstring )env->GetStaticObjectField( class_Build,		field_UNKNOWN		) ;
			jstring stringfield_USER			= ( jstring )env->GetStaticObjectField( class_Build,		field_USER			) ;
			jint    intfield_SDK_INT			=            env->GetStaticIntField(    class_BuildVERSION,	field_SDK_INT		) ;

			if( stringfield_BOARD			!= NULL &&
				stringfield_BOOTLOADER		!= NULL &&
				stringfield_BRAND			!= NULL &&
				stringfield_CPU_ABI			!= NULL &&
				stringfield_CPU_ABI2		!= NULL &&
				stringfield_DEVICE			!= NULL &&
				stringfield_DISPLAY			!= NULL &&
				stringfield_FINGERPRINT		!= NULL &&
				stringfield_HARDWARE		!= NULL &&
				stringfield_HOST			!= NULL &&
				stringfield_ID				!= NULL &&
				stringfield_MANUFACTURER	!= NULL &&
				stringfield_MODEL			!= NULL &&
				stringfield_PRODUCT			!= NULL &&
				stringfield_RADIO			!= NULL &&
				stringfield_TAGS			!= NULL &&
				stringfield_TYPE			!= NULL &&
				stringfield_UNKNOWN			!= NULL &&
				stringfield_USER			!= NULL )
			{
				const char *charp_BOARD				= env->GetStringUTFChars( stringfield_BOARD			, NULL ) ;
				const char *charp_BOOTLOADER		= env->GetStringUTFChars( stringfield_BOOTLOADER	, NULL ) ;		
				const char *charp_BRAND				= env->GetStringUTFChars( stringfield_BRAND			, NULL ) ;
				const char *charp_CPU_ABI			= env->GetStringUTFChars( stringfield_CPU_ABI		, NULL ) ;		
				const char *charp_CPU_ABI2			= env->GetStringUTFChars( stringfield_CPU_ABI2		, NULL ) ;
				const char *charp_DEVICE			= env->GetStringUTFChars( stringfield_DEVICE		, NULL ) ;		
				const char *charp_DISPLAY			= env->GetStringUTFChars( stringfield_DISPLAY		, NULL ) ;		
				const char *charp_FINGERPRINT		= env->GetStringUTFChars( stringfield_FINGERPRINT	, NULL ) ;		
				const char *charp_HARDWARE			= env->GetStringUTFChars( stringfield_HARDWARE		, NULL ) ;
				const char *charp_HOST				= env->GetStringUTFChars( stringfield_HOST			, NULL ) ;
				const char *charp_ID				= env->GetStringUTFChars( stringfield_ID			, NULL ) ;		
				const char *charp_MANUFACTURER		= env->GetStringUTFChars( stringfield_MANUFACTURER	, NULL ) ;
				const char *charp_MODEL				= env->GetStringUTFChars( stringfield_MODEL			, NULL ) ;
				const char *charp_PRODUCT			= env->GetStringUTFChars( stringfield_PRODUCT		, NULL ) ;		
				const char *charp_RADIO				= env->GetStringUTFChars( stringfield_RADIO			, NULL ) ;
				const char *charp_TAGS				= env->GetStringUTFChars( stringfield_TAGS			, NULL ) ;
				const char *charp_TYPE				= env->GetStringUTFChars( stringfield_TYPE			, NULL ) ;	
				const char *charp_UNKNOWN			= env->GetStringUTFChars( stringfield_UNKNOWN		, NULL ) ;	
				const char *charp_USER				= env->GetStringUTFChars( stringfield_USER			, NULL ) ;
	
				if( charp_BOARD			!= NULL &&
					charp_BOOTLOADER	!= NULL &&
					charp_BRAND			!= NULL &&
					charp_CPU_ABI		!= NULL &&
					charp_CPU_ABI2		!= NULL &&
					charp_DEVICE		!= NULL &&
					charp_DISPLAY		!= NULL &&
					charp_FINGERPRINT	!= NULL &&
					charp_HARDWARE		!= NULL &&
					charp_HOST			!= NULL &&
					charp_ID			!= NULL &&
					charp_MANUFACTURER	!= NULL &&
					charp_MODEL			!= NULL &&
					charp_PRODUCT		!= NULL &&
					charp_RADIO			!= NULL &&
					charp_TAGS			!= NULL &&
					charp_TYPE			!= NULL &&
					charp_UNKNOWN		!= NULL &&
					charp_USER			!= NULL )
				{
					DXST_LOGFILE_ADDUTF16LE(( "\x4f\x00\x53\x00\xc5\x60\x31\x58\xfa\x51\x9b\x52\x0a\x00\x00"/*@ L"OS情報出力\n" @*/ )) ;
					NS_LogFileTabAdd() ;

					DXST_LOGFILEFMT_ADDUTF16LE((   "\x41\x00\x50\x00\x49\x00\x20\x00\x4c\x00\x65\x00\x76\x00\x65\x00\x6c\x00\x1a\xff\x25\x00\x64\x00\x00"/*@ L"API Level：%d" @*/, intfield_SDK_INT )) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xdc\x30\xfc\x30\xc9\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ボード：%s" @*/,				charp_BOARD			) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xd6\x30\xfc\x30\xc8\x30\xed\x30\xfc\x30\xc0\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ブートローダ：%s" @*/,		charp_BOOTLOADER	) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xd6\x30\xe9\x30\xf3\x30\xc9\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ブランド：%s" @*/,			charp_BRAND			) ;
					OutputAndroidOSInfo_LogAddUTF8( "\x7d\x54\xe4\x4e\xbb\x30\xc3\x30\xc8\x30\x11\xff\x1a\xff\x25\x00\x73\x00\x00"/*@ L"命令セット１：%s" @*/,		charp_CPU_ABI		) ;
					OutputAndroidOSInfo_LogAddUTF8( "\x7d\x54\xe4\x4e\xbb\x30\xc3\x30\xc8\x30\x12\xff\x1a\xff\x25\x00\x73\x00\x00"/*@ L"命令セット２：%s" @*/,		charp_CPU_ABI2		) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"デバイス：%s" @*/,			charp_DEVICE		) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xc7\x30\xa3\x30\xb9\x30\xd7\x30\xec\x30\xa4\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ディスプレイ：%s" @*/,		charp_DISPLAY		) ;
	//				OutputAndroidOSInfo_LogAddUTF8( "\x58\x8b\x25\x52\x50\x5b\x1a\xff\x25\x00\x73\x00\x00"/*@ L"識別子：%s" @*/,				charp_FINGERPRINT	) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xcf\x30\xfc\x30\xc9\x30\xa6\x30\xa7\x30\xa2\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ハードウェア：%s" @*/,		charp_HARDWARE		) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xdb\x30\xb9\x30\xc8\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ホスト：%s" @*/,				charp_HOST			) ;
					OutputAndroidOSInfo_LogAddUTF8( "\x29\xff\x24\xff\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ＩＤ：%s" @*/,				charp_ID			) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xfd\x88\x20\x90\x05\x80\x0d\x54\x1a\xff\x25\x00\x73\x00\x00"/*@ L"製造者名：%s" @*/,			charp_MANUFACTURER	) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xe2\x30\xc7\x30\xeb\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"モデル：%s" @*/,				charp_MODEL			) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xfd\x88\xc1\x54\x0d\x54\x1a\xff\x25\x00\x73\x00\x00"/*@ L"製品名：%s" @*/,				charp_PRODUCT		) ;
					OutputAndroidOSInfo_LogAddUTF8( "\x21\x71\xda\x7d\xd5\x30\xa1\x30\xfc\x30\xe0\x30\xa6\x30\xa7\x30\xa2\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"無線ファームウェア：%s" @*/,	charp_RADIO			) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xd3\x30\xeb\x30\xc9\x30\xbf\x30\xb0\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ビルドタグ：%s" @*/,			charp_TAGS			) ;
	//				DXST_LOGFILEFMT_ADDUTF16LE((   "\xbf\x30\xa4\x30\xe0\x30\x1a\xff\x25\x00\x64\x00\x00"/*@ L"タイム：%d" @*/,              ( int )longfield_TIME )) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xd3\x30\xeb\x30\xc9\x30\xbf\x30\xa4\x30\xd7\x30\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ビルドタイプ：%s" @*/,		charp_TYPE			) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xc5\x60\x31\x58\x0d\x4e\x0e\x66\x42\x66\x6e\x30\x58\x8b\x25\x52\x50\x5b\x1a\xff\x25\x00\x73\x00\x00"/*@ L"情報不明時の識別子：%s" @*/,	charp_UNKNOWN		) ;
					OutputAndroidOSInfo_LogAddUTF8( "\xe6\x30\xfc\x30\xb6\x30\xc5\x60\x31\x58\x1a\xff\x25\x00\x73\x00\x00"/*@ L"ユーザ情報：%s" @*/,			charp_USER			) ;

					NS_LogFileTabSub() ;

					res = 0 ;
				}

				if( charp_BOARD			!= NULL ){ env->ReleaseStringUTFChars(	stringfield_BOARD			, charp_BOARD			) ; }
				if( charp_BOOTLOADER	!= NULL ){ env->ReleaseStringUTFChars(	stringfield_BOOTLOADER		, charp_BOOTLOADER		) ;	}
				if( charp_BRAND			!= NULL ){ env->ReleaseStringUTFChars(	stringfield_BRAND			, charp_BRAND			) ;	}
				if( charp_CPU_ABI		!= NULL ){ env->ReleaseStringUTFChars(	stringfield_CPU_ABI			, charp_CPU_ABI			) ;	}
				if( charp_CPU_ABI2		!= NULL ){ env->ReleaseStringUTFChars(	stringfield_CPU_ABI2		, charp_CPU_ABI2		) ;	}
				if( charp_DEVICE		!= NULL ){ env->ReleaseStringUTFChars(	stringfield_DEVICE			, charp_DEVICE			) ;	}
				if( charp_DISPLAY		!= NULL ){ env->ReleaseStringUTFChars(	stringfield_DISPLAY			, charp_DISPLAY			) ;	}
				if( charp_FINGERPRINT	!= NULL ){ env->ReleaseStringUTFChars(	stringfield_FINGERPRINT		, charp_FINGERPRINT		) ;	}
				if( charp_HARDWARE		!= NULL ){ env->ReleaseStringUTFChars(	stringfield_HARDWARE		, charp_HARDWARE		) ;	}
				if( charp_HOST			!= NULL ){ env->ReleaseStringUTFChars(	stringfield_HOST			, charp_HOST			) ;	}
				if( charp_ID			!= NULL ){ env->ReleaseStringUTFChars(	stringfield_ID				, charp_ID				) ;	}
				if( charp_MANUFACTURER	!= NULL ){ env->ReleaseStringUTFChars(	stringfield_MANUFACTURER	, charp_MANUFACTURER	) ;	}
				if( charp_MODEL			!= NULL ){ env->ReleaseStringUTFChars(	stringfield_MODEL			, charp_MODEL			) ;	}
				if( charp_PRODUCT		!= NULL ){ env->ReleaseStringUTFChars(	stringfield_PRODUCT			, charp_PRODUCT			) ;	}
				if( charp_RADIO			!= NULL ){ env->ReleaseStringUTFChars(	stringfield_RADIO			, charp_RADIO			) ;	}
				if( charp_TAGS			!= NULL ){ env->ReleaseStringUTFChars(	stringfield_TAGS			, charp_TAGS			) ;	}
				if( charp_TYPE			!= NULL ){ env->ReleaseStringUTFChars(	stringfield_TYPE			, charp_TYPE			) ;	}
				if( charp_UNKNOWN		!= NULL ){ env->ReleaseStringUTFChars(	stringfield_UNKNOWN			, charp_UNKNOWN			) ;	}
				if( charp_USER			!= NULL ){ env->ReleaseStringUTFChars(	stringfield_USER			, charp_USER			) ;	}
			}

			if( stringfield_BOARD			!= NULL ){ env->DeleteLocalRef( stringfield_BOARD			) ;	}
			if( stringfield_BOOTLOADER		!= NULL ){ env->DeleteLocalRef( stringfield_BOOTLOADER		) ;	}
			if( stringfield_BRAND			!= NULL ){ env->DeleteLocalRef( stringfield_BRAND			) ;	}
			if( stringfield_CPU_ABI			!= NULL ){ env->DeleteLocalRef( stringfield_CPU_ABI			) ;	}
			if( stringfield_CPU_ABI2		!= NULL ){ env->DeleteLocalRef( stringfield_CPU_ABI2		) ;	}
			if( stringfield_DEVICE			!= NULL ){ env->DeleteLocalRef( stringfield_DEVICE			) ;	}
			if( stringfield_DISPLAY			!= NULL ){ env->DeleteLocalRef( stringfield_DISPLAY			) ;	}
			if( stringfield_FINGERPRINT		!= NULL ){ env->DeleteLocalRef( stringfield_FINGERPRINT		) ;	}
			if( stringfield_HARDWARE		!= NULL ){ env->DeleteLocalRef( stringfield_HARDWARE		) ;	}
			if( stringfield_HOST			!= NULL ){ env->DeleteLocalRef( stringfield_HOST			) ;	}
			if( stringfield_ID				!= NULL ){ env->DeleteLocalRef( stringfield_ID				) ;	}
			if( stringfield_MANUFACTURER	!= NULL ){ env->DeleteLocalRef( stringfield_MANUFACTURER	) ;	}
			if( stringfield_MODEL			!= NULL ){ env->DeleteLocalRef( stringfield_MODEL			) ;	}
			if( stringfield_PRODUCT			!= NULL ){ env->DeleteLocalRef( stringfield_PRODUCT			) ;	}
			if( stringfield_RADIO			!= NULL ){ env->DeleteLocalRef( stringfield_RADIO			) ;	}
			if( stringfield_TAGS			!= NULL ){ env->DeleteLocalRef( stringfield_TAGS			) ;	}
			if( stringfield_TYPE			!= NULL ){ env->DeleteLocalRef( stringfield_TYPE			) ;	}
			if( stringfield_UNKNOWN			!= NULL ){ env->DeleteLocalRef( stringfield_UNKNOWN			) ;	}
			if( stringfield_USER			!= NULL ){ env->DeleteLocalRef( stringfield_USER			) ; }
		}
	}

	if( class_Build        != NULL ){ env->DeleteLocalRef( class_Build        ) ; }
	if( class_BuildVERSION != NULL ){ env->DeleteLocalRef( class_BuildVERSION ) ; }

	// 戻り値を返す
	return res ;
}

// ライブラリ初期化関数
extern int NS_DxLib_Init( void )
{
	// 既に初期化済みの場合は何もせず終了
	if( DxSysData.DxLib_InitializeFlag == TRUE )
	{
		return 0 ;
	}

	DXST_LOGFILEFMT_ADDUTF16LE(( "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x8b\x95\xcb\x59\x00"/*@ L"ＤＸライブラリの初期化処理開始" @*/ )) ;
	DXST_LOGFILE_TABADD ;

	// 初期化中フラグを立てる
	DxSysData.DxLib_RunInitializeFlag = TRUE ;

#ifndef DX_NON_LITERAL_STRING
	// ＤＸライブラリのバージョンを出力する
	{
		char UTF16LE_Buffer[ 128 ] ;
		char DestBuffer[ 128 ] ;
		ConvString( ( const char * )DXLIB_VERSION_STR_W, -1, WCHAR_T_CHARCODEFORMAT, UTF16LE_Buffer, sizeof( UTF16LE_Buffer ), DX_CHARCODEFORMAT_UTF16LE ) ;
		CL_snprintf( DX_CHARCODEFORMAT_UTF16LE, TRUE, DX_CHARCODEFORMAT_SHIFTJIS, DX_CHARCODEFORMAT_UTF16LE, DestBuffer, sizeof( DestBuffer ) / 2, "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x20\x00\x56\x00\x65\x00\x72\x00\x25\x00\x73\x00\x0a\x00\x00"/*@ L"ＤＸライブラリ Ver%s\n" @*/, UTF16LE_Buffer ) ;

		DXST_LOGFILE_ADDUTF16LE( DestBuffer ) ;
	}
#endif

	// OS情報出力
	{
		pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

		if( g_AndroidSys.NativeActivity == NULL )
		{
			pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
			return -1 ;
		}

		// JavaVM とソフト実行用スレッドを関連付け
		JNIEnv *env ;
		if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
		{
			pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
			return -1 ;
		}

		OutputAndroidOSInfo( env ) ;

		// JavaVM とこのスレッドの関連付け終了
		g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
	}

	// DxSysData の共通初期化処理
	DxLib_SysInit() ;

	// DxBaseFunc の初期化
	_INIT_BASEFUNC() ;

	// キャラクターコード関係の初期化を行う
	InitCharCode() ;

	// 使用する文字セットをセット
	_SET_DEFAULT_CHARCODEFORMAT() ;

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理の初期化
	InitializeASyncLoad( Thread_GetCurrentId() ) ;
#endif // DX_NON_ASYNCLOAD

	// ファイルアクセス処理の初期化
	InitializeFile() ;

#ifndef DX_NON_OGGTHEORA
	// Theora 用の初期化
	TheoraDecode_GrobalInitialize() ;
#endif

	// アーカイブファイルアクセス用のデータを初期化
#ifndef DX_NON_DXA
	DXA_DIR_Initialize() ;
#endif

	// ストリームデータ読み込み制御用ポインタ構造体のデフォルト値をセット
	NS_ChangeStreamFunction( NULL ) ;

#ifndef DX_NON_LOG
	// ログファイルの初期化
	LogFileInitialize() ;
#endif

	// システムログを出力
//	OutSystemInfo() ;

#ifndef DX_NON_GRAPHICS
	// デフォルトのグラフィック復元関数を登録
	NS_SetRestoreGraphCallback( NULL ) ;
#endif // DX_NON_GRAPHICS

	// 各処理系の初期化
	if( DxSysData.NotInputFlag == FALSE )
	{
#ifndef DX_NON_INPUT
		if( InitializeInputSystem() == -1 ) goto ERROR_DX ;			// 入力システムの初期化
#endif // DX_NON_INPUT
	}

	if( DxSysData.NotSoundFlag == FALSE )
	{
#ifndef DX_NON_SOUND
		InitializeSoundConvert() ;									// サウンド変換処理の初期化
		InitializeSoundSystem() ;									// サウンドシステムのの初期化
#endif // DX_NON_SOUND
	}
	if( DxSysData.NotDrawFlag == FALSE )
	{
		InitializeBaseImageManage() ;
#ifndef DX_NON_SOFTIMAGE
		InitializeSoftImageManage() ;
#endif // DX_NON_SOFTIMAGE
#ifndef DX_NON_MOVIE
		InitializeMovieManage() ;
#endif

#ifndef DX_NON_GRAPHICS
		if( Graphics_Initialize() < 0 ) goto ERROR_DX ;
#endif // DX_NON_GRAPHICS
	}
#ifndef DX_NON_INPUTSTRING
	InitializeInputCharBuf() ;									// 文字コードバッファの初期化
#endif // DX_NON_INPUTSTRING

	// ＤＸライブラリ初期化完了フラグをたてる
	DxSysData.DxLib_InitializeFlag = TRUE ;

	// ＶＳＹＮＣ待ちをする
//	NS_SetWaitVSyncFlag( TRUE ) ;

#if !defined( DX_NON_LOG ) && !defined( DX_NON_PRINTF_DX )
	// ログ出力処理の初期化を行う
	InitializeLog() ;
#endif

#ifndef DX_NON_GRAPHICS
	// 描画先の変更
	NS_SetDrawScreen( DX_SCREEN_BACK ) ;
	NS_SetDrawScreen( DX_SCREEN_FRONT ) ;
#endif // DX_NON_GRAPHICS

	if( DxSysData.NotDrawFlag == FALSE )
	{
#ifndef DX_NON_MODEL
		// モデルバージョン１の初期化
		if( MV1Initialize() < 0 )
		{
			goto ERROR_DX ;
		}
#endif

#ifndef DX_NON_LIVE2D_CUBISM4
		// Live2D Cubism4 関連の初期化
		Live2DCubism4_Initialize() ;
#endif // DX_NON_LIVE2D_CUBISM4
	}

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理を行うスレッドを立てる
	if( SetupASyncLoadThread( 3 ) < 0 )
	{
		DXST_LOGFILE_ADDUTF16LE( "\x5e\x97\x0c\x54\x1f\x67\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xe6\x51\x06\x74\x92\x30\x4c\x88\x46\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x6e\x30\xcb\x7a\x61\x30\x0a\x4e\x52\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"非同期読み込み処理を行うスレッドの立ち上げに失敗しました\n" @*/ ) ;
		goto ERROR_DX ;
	}
#endif // DX_NON_ASYNCLOAD

	// 初期化中フラグを倒す
	DxSysData.DxLib_RunInitializeFlag = FALSE ;

	DXST_LOGFILE_TABSUB ;
	DXST_LOGFILEFMT_ADDUTF16LE(( "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x42\x7d\x86\x4e\x00"/*@ L"ＤＸライブラリの初期化処理終了" @*/ ) ) ;

	// 終了
	return 0 ;

ERROR_DX:
	NS_DxLib_End() ;

	DXST_LOGFILE_TABSUB ;
	DXST_LOGFILEFMT_ADDUTF16LE(( "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x31\x59\x57\x65\x00"/*@ L"ＤＸライブラリの初期化処理失敗" @*/ ) ) ;

	// 初期化中フラグを倒す
	DxSysData.DxLib_RunInitializeFlag = FALSE ;

	return -1 ;
} 

// ライブラリ使用の終了関数
extern int NS_DxLib_End( void )
{
	// 既に終了処理が行われているか、そもそも初期化されていない場合は何もしない
	if( DxSysData.DxLib_InitializeFlag == FALSE )
	{
		return 0 ;
	}

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込みをしない
	NS_SetUseASyncLoadFlag( FALSE ) ;

	// 全ての非同期読み込みの終了待ち
	while( NS_GetASyncLoadNum() > 0 )
	{
		// 削除リクエストが来ているハンドルを削除する
		DeleteRequestHandleDelete( FALSE ) ;

		// メインスレッドが処理する非同期読み込みの処理を行う
		ProcessASyncLoadRequestMainThread() ;
		Thread_Sleep( 1 ) ;
	}

	// 削除リクエストが来ているハンドルを全て削除する
	DeleteRequestHandleDelete( TRUE ) ;
#endif // DX_NON_ASYNCLOAD

#ifndef DX_NON_SOFTIMAGE
	// 登録した全てのソフトイメージを削除
	InitSoftImage() ;
#endif // DX_NON_SOFTIMAGE

	// 各処理系の終了
#if !defined( DX_NON_LOG ) && !defined( DX_NON_PRINTF_DX )
	TerminateLog() ;			// ログ処理の後始末
#endif

#ifndef DX_NON_NETWORK
	TerminateNetWork() ;		// ＷｉｎＳｏｃｋｅｔｓ関係の終了
#endif

#ifndef DX_NON_LIVE2D_CUBISM4
	// Live2D Cubism4 関連の後始末
	Live2DCubism4_Terminate() ;
#endif // DX_NON_LIVE2D_CUBISM4

#ifndef DX_NON_SOUND
	NS_StopMusic() ;			// ＭＩＤＩが演奏されている状態の場合それを止める
#endif // DX_NON_SOUND

#ifndef DX_NON_MODEL
	MV1Terminate() ;			// モデルバージョン１の後始末
#endif

#ifndef DX_NON_GRAPHICS
	Graphics_Terminate() ;
#endif // DX_NON_GRAPHICS

	TerminateBaseImageManage() ;

#ifndef DX_NON_SOFTIMAGE
	TerminateSoftImageManage() ;
#endif // DX_NON_SOFTIMAGE

#ifndef DX_NON_MOVIE
	TerminateMovieManage() ;
#endif

#ifndef DX_NON_INPUT
	TerminateInputSystem() ;	// 入力システムの終了
#endif // DX_NON_INPUT

#ifndef DX_NON_SOUND
	TerminateSoundSystem() ;	// サウンドシステムの後始末
	TerminateSoundConvert() ;	// サウンド変換処理の終了
#endif // DX_NON_SOUND

	// ＤＸライブラリ初期化完了フラグを倒す
	DxSysData.DxLib_InitializeFlag = FALSE ;

	// アーカイブファイルアクセス用のデータの後始末
#ifndef DX_NON_DXA
	DXA_DIR_Terminate() ;
#endif

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理用のスレッドを閉じる
	CloseASyncLoadThread() ;
#endif // DX_NON_ASYNCLOAD

	// ファイルアクセス処理の後始末
	TerminateFile() ;

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理の後始末
	TerminateASyncLoad() ;
#endif // DX_NON_ASYNCLOAD

#ifdef DX_USE_DXLIB_MEM_DUMP
	// メモリダンプを行う
	NS_DxDumpAlloc() ;
#endif

#ifndef DX_NON_LOG
	// ログファイルの後始末
	LogFileTerminate() ;
#endif

	// メモリの後始末を行う
	MemoryTerminate() ;

	// 終了
	return 0 ;
}

// ライブラリの内部で使用している構造体をゼロ初期化して、DxLib_Init の前に行った設定を無効化する( DxLib_Init の前でのみ有効 )
extern int NS_DxLib_GlobalStructInitialize( void )
{
//	_MEMSET( &GRA2, 0, sizeof( GRA2 ) ) ;

#ifndef DX_NON_GRAPHICS
	_MEMSET( &GraphicsSysData, 0, sizeof( GraphicsSysData ) );
#endif // DX_NON_GRAPHICS

	_MEMSET( &BaseImageManage, 0, sizeof( BaseImageManage ) ) ;
#ifndef DX_NON_SOUND
	_MEMSET( &SoundSysData, 0, sizeof( SoundSysData ) );
#endif // DX_NON_SOUND

	return 0;
}











// エラー処理関数

// エラー処理
extern int DxLib_Error( const wchar_t *ErrorStr )
{
	// エラーログの排出
	DXST_LOGFILE_ADDW( ErrorStr ) ;
	DXST_LOGFILE_ADDW( L"\n" ) ;

	// 各処理系の終了
	NS_DxLib_End() ;

	exit( -1 ) ;

	return -1 ;
}

// ライブラリのエラー処理を行う( UTF16LE版 )
extern int DxLib_ErrorUTF16LE( const char *ErrorStr )
{
	int Result ;

	CHAR_TO_WCHAR_T_STRING_BEGIN( ErrorStr )
	CHAR_TO_WCHAR_T_STRING_SETUP( ErrorStr, return -1, DX_CHARCODEFORMAT_UTF16LE )

	Result = DxLib_Error( UseErrorStrBuffer ) ;

	CHAR_TO_WCHAR_T_STRING_END( ErrorStr )

	return Result ;
}



























// カウンタ及び時刻取得系関数

// ミリ秒単位の精度を持つカウンタの現在値を得る
extern int NS_GetNowCount( int /*UseRDTSCFlag*/ )
{
	LONGLONG ResultLL ;
	int Result ;

	ResultLL  = NS_GetNowHiPerformanceCount( FALSE ) / 1000 ;
	ResultLL &= 0x7fffffff ;
	Result    = ( int )ResultLL ;

	return Result ;
}

// GetNowTimeの高精度バージョン
extern LONGLONG NS_GetNowHiPerformanceCount( int /*UseRDTSCFlag*/ )
{
	LONGLONG NowTime ;
	timeval ltimeval ;

	gettimeofday( &ltimeval, NULL ) ;

	NowTime = ( LONGLONG )ltimeval.tv_sec * 1000000 + ltimeval.tv_usec ;

	return NowTime ;
}

// OSが提供する高精度カウンタの現在の値を得る
extern ULONGLONG NS_GetNowSysPerformanceCount( void )
{
	return ( ULONGLONG )NS_GetNowHiPerformanceCount( FALSE ) ;
}

// OSが提供する高精度カウンタの周波数( 1秒辺りのカウント数 )を得る
extern ULONGLONG NS_GetSysPerformanceFrequency( void )
{
	return 1000000 ;
}

// OSが提供する高精度カウンタの値を秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToSeconds( ULONGLONG Count )
{
	return Count / 1000000 ;
}

// OSが提供する高精度カウンタの値をミリ秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToMilliSeconds( ULONGLONG Count )
{
	return Count / 1000 ;
}

// OSが提供する高精度カウンタの値をマイクロ秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToMicroSeconds( ULONGLONG Count )
{
	return Count ;
}

// OSが提供する高精度カウンタの値をナノ秒の値に変換する
extern ULONGLONG NS_ConvSysPerformanceCountToNanoSeconds( ULONGLONG Count )
{
	return Count * 1000 ;
}

// 秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvSecondsToSysPerformanceCount( ULONGLONG Seconds )
{
	return Seconds * 1000000 ;
}

// ミリ秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvMilliSecondsToSysPerformanceCount( ULONGLONG MilliSeconds )
{
	return MilliSeconds * 1000 ;
}

// マイクロ秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvMicroSecondsToSysPerformanceCount( ULONGLONG MicroSeconds )
{
	return MicroSeconds ;
}

// ナノ秒の値をOSが提供する高精度カウンタの値に変換する
extern ULONGLONG NS_ConvNanoSecondsToSysPerformanceCount( ULONGLONG NanoSeconds )
{
	return NanoSeconds / 1000 ;
}

// 現在時刻を取得する
extern int NS_GetDateTime( DATEDATA *DateBuf )
{
	time_t nowtime ;
	tm *datetime ;

	time( &nowtime ) ;

	datetime = localtime( &nowtime ) ;

	// ローカル時刻データを元に専用のデータ型データに時刻を繁栄させる
	DateBuf->Year	= datetime->tm_year + 1900 ;
	DateBuf->Mon	= datetime->tm_mon + 1 ;
	DateBuf->Day	= datetime->tm_mday ;
	DateBuf->Hour	= datetime->tm_hour ;
	DateBuf->Min	= datetime->tm_min ;
	DateBuf->Sec	= datetime->tm_sec ;

	// 終了
	return 0 ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE






























// メタデータ( fullscreen )を取得する
extern int GetAndroidMetaDataFullScreen( JNIEnv *env )
{
	TCHAR *StringBuffer = NULL ;
	int StringBufferBytes ;

	StringBufferBytes = GetAndroidMetaData_Base( env, "fullscreen", NULL, 0, NULL ) ;
	if( StringBufferBytes < 0 )
	{
		// API Level の定数を持つクラスの取得
		jclass class_BuildVERSION = env->FindClass( "android/os/Build$VERSION" ) ;
		if( class_BuildVERSION == NULL )
		{
			return -1 ;
		}

		// API Level の定数のIDを取得
		jfieldID field_SDK_INT = env->GetStaticFieldID( class_BuildVERSION, "SDK_INT", "I" ) ;
		if( field_SDK_INT == NULL )
		{
			return -1 ;
		}

		// API Level の値を取得
		jint intfield_SDK_INT = env->GetStaticIntField( class_BuildVERSION, field_SDK_INT ) ;

		// もし API Level が 19 未満の場合はフルスクリーンモードにしない
		g_AndroidSys.IsFullScreen = intfield_SDK_INT >= 19 ? TRUE : FALSE ;
	}
	else
	{
		StringBuffer = ( TCHAR * )DXALLOC( StringBufferBytes ) ;
		if( StringBuffer == NULL )
		{
			g_AndroidSys.IsFullScreen = TRUE ;
		}
		else
		{
			GetAndroidMetaData_Base( env, "fullscreen", StringBuffer, StringBufferBytes, NULL ) ;

			g_AndroidSys.IsFullScreen = CL_strcmp( _TCHARCODEFORMAT, StringBuffer, "off" ) == 0 ? FALSE : TRUE ;

			DXFREE( StringBuffer ) ;
		}
	}

	return 0 ;
}


// ステータスバーとナビゲーションバーを非表示にする
extern int SetAndroidWindowStyle( JNIEnv *env )
{
	// 戻り値の初期値は -1
	int res = -1 ;

	if( env == NULL )
	{
		return res ;
	}

	// API Level の定数を持つクラスの取得
	jclass class_BuildVERSION = env->FindClass( "android/os/Build$VERSION" ) ;
	if( class_BuildVERSION == NULL )
	{
		return res ;
	}

	// API Level の定数のIDを取得
	jfieldID field_SDK_INT = env->GetStaticFieldID( class_BuildVERSION, "SDK_INT", "I" ) ;
	if( field_SDK_INT == NULL )
	{
		return res ;
	}

	// API Level の値を取得
	jint intfield_SDK_INT = env->GetStaticIntField( class_BuildVERSION, field_SDK_INT ) ;

	// setSystemUiVisibility 呼び出しに必要なクラスを取得
	jclass class_NativeActivity = env->GetObjectClass( g_AndroidSys.NativeActivity->clazz ) ;
	jclass class_Window         = env->FindClass( "android/view/Window" ) ;
	jclass class_View           = env->FindClass( "android/view/View"   ) ;

	// クラスの取得ができた場合のみ if 文の中に入る
	if( class_NativeActivity != NULL &&
		class_Window         != NULL &&
		class_View           != NULL )
	{
		// setSystemUiVisibility 呼び出しに必要な関数を取得する
		jmethodID methodID_getWindow             = env->GetMethodID( class_NativeActivity, "getWindow",             "()Landroid/view/Window;" ) ;
		jmethodID methodID_getDecorView          = env->GetMethodID( class_Window,         "getDecorView",          "()Landroid/view/View;"   ) ;
		jmethodID methodID_setSystemUiVisibility = env->GetMethodID( class_View,           "setSystemUiVisibility", "(I)V"                    ) ;

		// 関数の取得ができた場合のみ if 文の中に入る
		if( methodID_getWindow             != NULL &&
			methodID_getDecorView          != NULL && 
			methodID_setSystemUiVisibility != NULL )
		{
			// ソフトの window の取得
			jobject object_Window = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, methodID_getWindow ) ;
			if( object_Window != NULL )
			{
				// ソフトの DecorView を取得
				jobject object_DecorView = env->CallObjectMethod( object_Window, methodID_getDecorView ) ;
				if( object_DecorView != NULL )
				{
					// API Level に応じて setSystemUiVisibility に渡すフラグを変更する
					jint flags = 0 ;

					// API Level 14
					if( intfield_SDK_INT >= 14 )
					{
						jfieldID field_SYSTEM_UI_FLAG_HIDE_NAVIGATION = env->GetStaticFieldID( class_View, "SYSTEM_UI_FLAG_HIDE_NAVIGATION", "I" ) ;
						jfieldID field_SYSTEM_UI_FLAG_LOW_PROFILE     = env->GetStaticFieldID( class_View, "SYSTEM_UI_FLAG_LOW_PROFILE",     "I" ) ;
						if( field_SYSTEM_UI_FLAG_HIDE_NAVIGATION != NULL &&
							field_SYSTEM_UI_FLAG_LOW_PROFILE     != NULL )
						{
							jint intfield_SYSTEM_UI_FLAG_HIDE_NAVIGATION = env->GetStaticIntField( class_View, field_SYSTEM_UI_FLAG_HIDE_NAVIGATION ) ;
							jint intfield_SYSTEM_UI_FLAG_LOW_PROFILE     = env->GetStaticIntField( class_View, field_SYSTEM_UI_FLAG_LOW_PROFILE ) ;
							flags |= intfield_SYSTEM_UI_FLAG_HIDE_NAVIGATION |
									 intfield_SYSTEM_UI_FLAG_LOW_PROFILE ;
						}
					}

					// API Level 16
					if( intfield_SDK_INT >= 16 )
					{
						jfieldID field_SYSTEM_UI_FLAG_FULLSCREEN = env->GetStaticFieldID( class_View, "SYSTEM_UI_FLAG_FULLSCREEN", "I" ) ;
						if( field_SYSTEM_UI_FLAG_FULLSCREEN != NULL )
						{
							jint intfield_SYSTEM_UI_FLAG_FULLSCREEN = env->GetStaticIntField( class_View, field_SYSTEM_UI_FLAG_FULLSCREEN ) ;
							flags |= intfield_SYSTEM_UI_FLAG_FULLSCREEN ;
						}
					}

					// API Level 19
					if( intfield_SDK_INT >= 19 )
					{
						jfieldID field_SYSTEM_UI_FLAG_IMMERSIVE_STICKY = env->GetStaticFieldID( class_View, "SYSTEM_UI_FLAG_IMMERSIVE_STICKY", "I" ) ;
						jfieldID field_SYSTEM_UI_FLAG_IMMERSIVE        = env->GetStaticFieldID( class_View, "SYSTEM_UI_FLAG_IMMERSIVE",        "I" ) ;
						if( field_SYSTEM_UI_FLAG_IMMERSIVE_STICKY != NULL &&
							field_SYSTEM_UI_FLAG_IMMERSIVE        != NULL )
						{
							jint intfield_SYSTEM_UI_FLAG_IMMERSIVE_STICKY = env->GetStaticIntField( class_View, field_SYSTEM_UI_FLAG_IMMERSIVE_STICKY ) ;
							jint intfield_SYSTEM_UI_FLAG_IMMERSIVE        = env->GetStaticIntField( class_View, field_SYSTEM_UI_FLAG_IMMERSIVE        ) ;
							flags |= intfield_SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
									 intfield_SYSTEM_UI_FLAG_IMMERSIVE ;
						}
					}

					// setSystemUiVisibility の呼び出し
					env->CallVoidMethod( object_DecorView, methodID_setSystemUiVisibility, flags ) ;

					// ここまでこれた場合のみ戻り値を 0 にする
					res = 0 ;

					// 取得した参照の後始末
					env->DeleteLocalRef( object_DecorView ) ;
					object_DecorView = NULL ;
				}

				// 取得した参照の後始末
				env->DeleteLocalRef( object_Window ) ;
				object_Window = NULL ;
			}
		}
	}

	// 取得した参照の後始末
	if( class_BuildVERSION != NULL )
	{
		env->DeleteLocalRef( class_BuildVERSION ) ;
		class_BuildVERSION = NULL ;
	}

	if( class_NativeActivity != NULL )
	{
		env->DeleteLocalRef( class_NativeActivity ) ;
		class_NativeActivity = NULL ;
	}

	if( class_Window != NULL )
	{
		env->DeleteLocalRef( class_Window ) ;
		class_Window = NULL ;
	}

	if( class_View != NULL )
	{
		env->DeleteLocalRef( class_View ) ;
		class_View = NULL ;
	}

	// 戻り値を返す
	return res ;
}

// android_main 関数
#ifdef DX_COMPILE_TYPE_C_LANGUAGE
extern "C" 
{
	extern int android_main( void ) ;
}
#else // DX_COMPILE_TYPE_C_LANGUAGE
extern int android_main( void ) ;
#endif // DX_COMPILE_TYPE_C_LANGUAGE

// ソフト用スレッドのエントリーポイント
static void* android_app_entry( void * )
{
	// ＤＸライブラリのファイルシステムにアセットマネージャーを登録
	SetAssetManager( g_AndroidSys.NativeActivity->assetManager ) ;

	// ＤＸライブラリのファイルシステムに InternalDataPath と ExternalDataPath を登録
	SetInternalAndExternalDataPath(
		g_AndroidSys.NativeActivity->internalDataPath,
		g_AndroidSys.NativeActivity->externalDataPath
	) ;

#ifndef DX_NON_FONT
	// フォントタイプはアンチエイリアスタイプに固定
	SetAntialiasingFontOnlyFlag( TRUE ) ;
#endif // DX_NON_FONT


#ifndef DX_NON_DXA
	// ＤＸアーカイブのパスを大文字にしないようにする
	DXA_DIR_SetNotArchivePathCharUp( TRUE ) ;
#endif // DX_NON_DXA

	// コールバック無し Looper の取得
    g_AndroidSys.Looper = ALooper_prepare( ALOOPER_PREPARE_ALLOW_NON_CALLBACKS ) ;

	// メインスレッドからのメッセージ受け取り登録
    ALooper_addFd(
		g_AndroidSys.Looper,
		g_AndroidSys.MessageRead,
		DX_LOOPER_ID_MAIN,
		ALOOPER_EVENT_INPUT,
		NULL,
		NULL
	) ;

	// センサーの初期化
	{
		int i ;

		g_AndroidSys.SensorManager					= ASensorManager_getInstance() ;
		for( i = 0 ; i < DX_ANDROID_SENSOR_NUM ; i ++ )
		{
			g_AndroidSys.SensorInfos[ i ].Sensor			= ASensorManager_getDefaultSensor( g_AndroidSys.SensorManager, g_AndroidSensorBaseInfos[ i ].TypeID ) ;
			g_AndroidSys.SensorInfos[ i ].SensorEventQueue	= ASensorManager_createEventQueue( g_AndroidSys.SensorManager, g_AndroidSys.Looper, g_AndroidSensorBaseInfos[ i ].LooperEventID, NULL, NULL ) ;
		}
	}

	// ソフト用スレッド開始したかフラグを立てる
    pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
	g_AndroidSys.MutexLockIndex = 1 << 7 ;
    g_AndroidSys.SoftThreadRunning = 1 ;
    pthread_cond_broadcast( &g_AndroidSys.Cond ) ;
	g_AndroidSys.MutexLockIndex &= ~( 1 << 7 ) ;
    pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;

	// ソフト用スレッドが待ち状態かどうかのフラグを倒す
	g_AndroidSys.SoftThreadWait = 0 ;

	// ステータスバーとナビゲーションバーを非表示にする
	{
		JNIEnv *env ;
		int res = -1 ;

		// JavaVM とソフト実行用スレッドを関連付け
		if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) == JNI_OK )
		{
			// フルスクリーンにするかどうかのメタデータの取得
			GetAndroidMetaDataFullScreen( env ) ;

			// フルスクリーンの場合はステータスバーとナビゲーションバーを非表示にする関数呼び出し
			if( g_AndroidSys.IsFullScreen )
			{
				res = SetAndroidWindowStyle( env ) ;
			}

			// JavaVM とこのスレッドの関連付け終了
			g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;
		}

		// フルスクリーンの場合で、ステータスバーとナビゲーションバーの非表示処理に失敗したら、旧バージョン用のフルスクリーン化処理を実行する
		if( g_AndroidSys.IsFullScreen && res < 0 )
		{
			ANativeActivity_setWindowFlags( g_AndroidSys.NativeActivity, AWINDOW_FLAG_FULLSCREEN, AWINDOW_FLAG_FULLSCREEN ) ;
		}
	}

	// android_main の呼び出し
	android_main() ;

	// onDestroy が発生していない場合は finish を呼び出して onDestroy が発生するまで待つ
	if( g_AndroidSys.DestroyRequested == 0 )
	{
		// 完全終了する指定がある場合は killProcess を呼ぶ
		if( g_AndroidSys.AppKillProcessFlag /* && JAVAANDR.fieldint_Build_VERSION_SDK_INT >= 21 */ )
		{
			JNIEnv *env ;

			// JavaVM とソフト実行用スレッドを関連付け
			if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) == JNI_OK )
			{
//				env->CallVoidMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Activity_finishAndRemoveTask );
//				env->CallBooleanMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Activity_releaseInstance );
				int MyPID = env->CallStaticIntMethod( JAVAANDR.class_Process, JAVAANDR.methodID_Process_myPid ) ;
				env->CallStaticVoidMethod( JAVAANDR.class_Process, JAVAANDR.methodID_Process_killProcess, MyPID ) ;

				// JavaVM とこのスレッドの関連付け終了
				g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;
			}
			else
			{
				ANativeActivity_finish( g_AndroidSys.NativeActivity ) ;
			}
		}
		else
		{
			ANativeActivity_finish( g_AndroidSys.NativeActivity ) ;
		}
		while( NS_ProcessMessage() == 0 )
		{
			Thread_Sleep( 10 ) ;
		}
	}

	// 入力イベントキューの後始末
    pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
	g_AndroidSys.MutexLockIndex = 1 << 8 ;
    if( g_AndroidSys.InputQueue != NULL )
	{
		AInputQueue_detachLooper( ( AInputQueue * )g_AndroidSys.InputQueue ) ;
		g_AndroidSys.InputQueue = NULL ;
    }
	
	// センサーイベントキューの後始末
	{
		int i ;

		for( i = 0 ; i < DX_ANDROID_SENSOR_NUM ; i ++ )
		{
			if( g_AndroidSys.SensorInfos[ i ].SensorEventQueue != NULL )
			{
				ASensorManager_destroyEventQueue( g_AndroidSys.SensorManager, g_AndroidSys.SensorInfos[ i ].SensorEventQueue ) ;
				g_AndroidSys.SensorInfos[ i ].SensorEventQueue = NULL ;
			}
		}
	}

    pthread_cond_broadcast( &g_AndroidSys.Cond ) ;
	g_AndroidSys.MutexLockIndex &= ~( 1 << 8 ) ;
    pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;

	// onDestroy が終了するのを待つ
	while( g_AndroidSys.onDestroyEnd == 0 )
	{
		Thread_Sleep( 10 ) ;
	}

	// ソフト実行用スレッドとの通信用のパイプを閉じる
	if( g_AndroidSys.MessageRead != 0 )
	{
		close( g_AndroidSys.MessageRead ) ;
		g_AndroidSys.MessageRead = 0 ;
	}
	if( g_AndroidSys.MessageWrite != 0 )
	{
		close( g_AndroidSys.MessageWrite ) ;
		g_AndroidSys.MessageWrite = 0 ;
	}

//	// Java の参照なども解放
//	TerminateJavaAndroidInfo( g_AndroidSys.NativeActivity->env ) ;

	// ミューテックスと条件変数の使用も終了
	pthread_cond_destroy( &g_AndroidSys.Cond ) ;
	g_AndroidSys.MutexLockIndex &= ~( 1 << 20 ) ;
	pthread_mutex_destroy( &g_AndroidSys.Mutex ) ;
	pthread_mutex_destroy( &g_AndroidSys.NativeActivityMutex ) ;

	// ソフト実行用スレッドが終了したかどうかのフラグを立てる
    g_AndroidSys.SoftThreadDestroyed = 1 ;

    return NULL ;
}

// ソフト実行用スレッドにコマンドを送信する
static void AndroidWriteCommand( int8_t cmd )
{
	write( g_AndroidSys.MessageWrite, &cmd, sizeof( cmd ) ) ;
}

// ソフト実行用スレッドに ActiveState を変更するコマンドを送信する
static void AndroidSetActivityState( int8_t cmd )
{
	pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
	g_AndroidSys.MutexLockIndex = 1 << 1 ;

	// ソフト実行用スレッドに新しい ActivityState としてコマンドをそのまま送信
    AndroidWriteCommand( cmd ) ;

	// ActivityState が更新されるまで待つ
    while( g_AndroidSys.ActivityState != cmd )
	{
        pthread_cond_wait( &g_AndroidSys.Cond, &g_AndroidSys.Mutex ) ;
    }
	g_AndroidSys.MutexLockIndex &= ~( 1 << 1 ) ;
    pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
}

// onDestroy のコールバック関数
static void onDestroy( ANativeActivity* NativeActivity )
{
	int i ;
	
	pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
	g_AndroidSys.MutexLockIndex = 1 << 9 ;

	// リストから外す
	for( i = 0; i < g_AndroidSys.NativeActivityNum; i++ )
	{
		if( g_AndroidSys.NativeActivityBuffer[ i ] == NativeActivity )
		{
			g_AndroidSys.NativeActivityNum-- ;
			if( g_AndroidSys.NativeActivityNum > i )
			{
				_MEMMOVE( &g_AndroidSys.NativeActivityBuffer[ i ], &g_AndroidSys.NativeActivityBuffer[ i + 1 ], sizeof( ANativeActivity * ) * ( g_AndroidSys.NativeActivityNum - i ) ) ;
			}
			break ;
		}
	}

	if( g_AndroidSys.NativeActivity == NativeActivity )
	{
		// onDestroy が呼ばれたコマンドをソフト実行用スレッドに送信
		AndroidWriteCommand( DX_ANDR_CMD_DESTROY ) ;

		// メッセージが届くまで待つ
		while( g_AndroidSys.DestroyRequested == 0 )
		{
			pthread_cond_wait( &g_AndroidSys.Cond, &g_AndroidSys.Mutex ) ;
		}

		// ソフト実行用スレッドが終了するまで待つ
	//	while( g_AndroidSys.SoftThreadDestroyed == 0 )
	//	{
	//		pthread_cond_wait( &g_AndroidSys.Cond, &g_AndroidSys.Mutex ) ;
	//	}
	//
	//	g_AndroidSys.SoftThreadDestroyed = 0 ;

		g_AndroidSys.MutexLockIndex &= ~( 1 << 9 ) ;
		pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;

	//	// ソフト実行用スレッドとの通信用のパイプを閉じる
	//	if( g_AndroidSys.MessageRead != 0 )
	//	{
	//		close( g_AndroidSys.MessageRead ) ;
	//		g_AndroidSys.MessageRead = 0 ;
	//	}
	//	if( g_AndroidSys.MessageWrite != 0 )
	//	{
	//		close( g_AndroidSys.MessageWrite ) ;
	//		g_AndroidSys.MessageWrite = 0 ;
	//	}
	//
	//	// ミューテックスと条件変数の使用も終了
	//	pthread_cond_destroy( &g_AndroidSys.Cond ) ;
	//	g_AndroidSys.MutexLockIndex &= ~( 1 << 20 ) ;
	//	pthread_mutex_destroy( &g_AndroidSys.Mutex ) ;

		pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

		// Java の参照なども解放
		TerminateJavaAndroidInfo( NativeActivity->env ) ;

		g_AndroidSys.NativeActivity = NULL ;

		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

		g_AndroidSys.onDestroyEnd = 1 ;
	}
	else
	{
		g_AndroidSys.MutexLockIndex &= ~( 1 << 9 ) ;
		pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
	}
}

// onStart のコールバック関数
static void onStart( ANativeActivity *NativeActivity )
{
	// ソフト実行用スレッドの ActivityState を DX_ANDR_CMD_START にする
	AndroidSetActivityState( DX_ANDR_CMD_START ) ;
}

// onResume のコールバック関数
static void onResume( ANativeActivity *NativeActivity )
{
	// ソフト実行用スレッドの ActivityState を DX_ANDR_CMD_RESUME にする
    AndroidSetActivityState( DX_ANDR_CMD_RESUME ) ;

	// フルスクリーンにするかどうかのメタデータの取得
	GetAndroidMetaDataFullScreen( NativeActivity->env ) ;

	// フルスクリーンの場合はステータスバーとナビゲーションバーを非表示にする
	if( g_AndroidSys.IsFullScreen )
	{
		if( SetAndroidWindowStyle( NativeActivity->env ) < 0 )
		{
			// 失敗したら旧バージョン用のフルスクリーン化処理を実行する
			ANativeActivity_setWindowFlags( NativeActivity, AWINDOW_FLAG_FULLSCREEN, AWINDOW_FLAG_FULLSCREEN ) ;
		}
	}

	ANativeActivity_showSoftInput( NativeActivity, ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT ) ;

//	StartInputStringDialogStatic( NativeActivity->env, "test" ) ;
}

// onPause のコールバック関数
static void onPause( ANativeActivity *NativeActivity )
{
	// ソフト実行用スレッドの ActivityState を DX_ANDR_CMD_PAUSE にする
	if( g_AndroidSys.NativeActivity == NativeActivity )
	{
		AndroidSetActivityState( DX_ANDR_CMD_PAUSE ) ;
	}
}

// onStop のコールバック関数
static void onStop( ANativeActivity *NativeActivity )
{
	// ソフト実行用スレッドの ActivityState を DX_ANDR_CMD_STOP にする
	if( g_AndroidSys.NativeActivity == NativeActivity )
	{
		AndroidSetActivityState( DX_ANDR_CMD_STOP ) ;
	}
}

// onWindowFocusChanged のコールバック関数
static void onWindowFocusChanged( ANativeActivity *NativeActivity, int focused )
{
	// focused によってソフト実行用スレッドに送信するコマンドを分ける
    AndroidWriteCommand( focused ? DX_ANDR_CMD_GAINED_FOCUS : DX_ANDR_CMD_LOST_FOCUS ) ;
}

// onNativeWindowCreated や onNativeWindowDestroyed の処理を行う関数
static void onNativeWindowBase( ANativeWindow *NativeWindow )
{
    pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
	g_AndroidSys.MutexLockIndex = 1 << 10 ;

	// 新しいウインドウのアドレスをセット
    g_AndroidSys.NewNativeWindow = NativeWindow ;

	// ソフト実行用スレッドにウインドウの変更コマンドを送信
	AndroidWriteCommand( DX_ANDR_CMD_WINDOW_CHANGED ) ;

	// ウインドウの変更が反映されるまで待つ
    while( g_AndroidSys.NativeWindow != g_AndroidSys.NewNativeWindow )
	{
        pthread_cond_wait( &g_AndroidSys.Cond, &g_AndroidSys.Mutex ) ;
    }
	g_AndroidSys.MutexLockIndex &= ~( 1 << 10 ) ;
    pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
}

// onNativeWindowCreated のコールバック関数
static void onNativeWindowCreated( ANativeActivity *NativeActivity, ANativeWindow* NativeWindow )
{
	// onNativeWindowBase 関数で処理する
	onNativeWindowBase( NativeWindow ) ;
}

// onNativeWindowDestroyed のコールバック関数
static void onNativeWindowDestroyed( ANativeActivity *NativeActivity, ANativeWindow* NativeWindow )
{
	// onNativeWindowBase 関数で処理する
	if( g_AndroidSys.NativeActivity == NativeActivity )
	{
		onNativeWindowBase( NULL ) ;
	}
}

// onInputQueueCreated や onInputQueueDestroyed の処理を行う関数
static void onInputQueueBase( AInputQueue *InputQueue )
{
    pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
	g_AndroidSys.MutexLockIndex = 1 << 2 ;

	// 新しい入力イベントを受け取るためのキューのアドレスをセット
    g_AndroidSys.NewInputQueue = InputQueue ;

	// ソフト実行用スレッドに入力イベントを受け取るためのキューの変更コマンドを送信
    AndroidWriteCommand( DX_ANDR_CMD_INPUT_CHANGED ) ;

	// 入力イベントを受け取るためのキューの変更が反映されるまで待つ
    while( g_AndroidSys.InputQueue != g_AndroidSys.NewInputQueue )
	{
        pthread_cond_wait( &g_AndroidSys.Cond, &g_AndroidSys.Mutex ) ;
    }
	g_AndroidSys.MutexLockIndex &= ~( 1 << 2 ) ;
    pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
}

// onInputQueueCreated のコールバック関数
static void onInputQueueCreated( ANativeActivity *NativeActivity, AInputQueue *queue )
{
	// onInputQueueBase で処理する
	onInputQueueBase( queue ) ;
}

// onInputQueueDestroyed のコールバック関数
static void onInputQueueDestroyed( ANativeActivity *NativeActivity, AInputQueue *queue )
{
	// onInputQueueBase で処理する
	if( g_AndroidSys.NativeActivity == NativeActivity )
	{
		onInputQueueBase( NULL ) ;
	}
}

// グローバル変数をゼロ初期化する
static void InitializeGlobalData( void )
{
	static int Flag = 0 ;

	// 初回は実行しない
	if( Flag == 0 )
	{
		Flag = 1 ;
		return ;
	}

	_MEMSET( &g_AndroidSys, 0, sizeof( g_AndroidSys ) ) ;
	_MEMSET( &g_JavaAndroidInfo, 0, sizeof( g_JavaAndroidInfo ) ) ;
	_MEMSET( &DxSysData, 0, sizeof( DxSysData ) ) ;

#ifndef DX_NON_DXA
    _MEMSET( &DX_ArchiveDirData, 0, sizeof( DX_ArchiveDirData ) ) ;
#endif // DX_NON_DXA

#ifndef DX_NON_ASYNCLOAD
    _MEMSET( &GASyncLoadData, 0, sizeof( GASyncLoadData ) ) ;
#endif // DX_NON_ASYNCLOAD

#ifndef DX_NON_FONT
	_MEMSET( &FontSystem, 0, sizeof( FontSystem ) ) ;
#endif // DX_NON_FONT

#ifndef DX_NON_GRAPHICS
    _MEMSET( &GraphicsHardDataAndroid, 0, sizeof( GraphicsHardDataAndroid ) ) ;
	_MEMSET( &GraphicsSysData, 0, sizeof( GraphicsSysData ) ) ;

	#ifndef DX_NON_FILTER
		_MEMSET( &GraphFilterSystemInfoAndroid, 0, sizeof( GraphFilterSystemInfoAndroid ) ) ;
		_MEMSET( &GraphFilterShaderHandle, 0, sizeof( GraphFilterShaderHandle ) ) ;
	#endif // DX_NON_FILTER

	#ifndef DX_NON_MODEL
		_MEMSET( &MV1Man_Android, 0, sizeof( MV1Man_Android ) ) ;
		_MEMSET( &MV1Man, 0, sizeof( MV1Man ) ) ;
	#endif // DX_NON_MODEL

#endif // DX_NON_GRAPHICS

#ifndef DX_NON_LOG
	_MEMSET( &LogData, 0, sizeof( LogData ) ) ;
#endif // DX_NON_LOG

#ifndef DX_NON_MASK
    _MEMSET( &MaskManageData_Android, 0, sizeof( MaskManageData_Android ) ) ;
	_MEMSET( &MaskManageData, 0, sizeof( MaskManageData ) ) ;
#endif // DX_NON_MASK

#ifndef DX_NON_NETWORK
	_MEMSET( &SockData, 0, sizeof( SockData ) ) ;
	_MEMSET( &HttpData, 0, sizeof( HttpData ) ) ;
#endif // DX_NON_NETWORK

#ifndef DX_NON_SOFTIMAGE
	_MEMSET( &SoftImageManage, 0, sizeof( SoftImageManage ) ) ;
#endif // DX_NON_SOFTIMAGE

#ifndef DX_NON_MOVIE
	_MEMSET( &MovieGraphManageData, 0, sizeof( MovieGraphManageData ) ) ;
#endif // DX_NON_MOVIE

#ifndef DX_NON_SOUND
	_MEMSET( &SoundSysData, 0, sizeof( SoundSysData ) ) ;
	_MEMSET( &MidiSystemData, 0, sizeof( MidiSystemData ) ) ;	
	_MEMSET( &GSoundConvertData, 0, sizeof( GSoundConvertData ) ) ;
#endif // DX_NON_SOUND

	_MEMSET( &MemImgManage, 0, sizeof( MemImgManage ) ) ;
	_MEMSET( &MemData, 0, sizeof( MemData ) ) ;

    _MEMSET( &g_BaseFuncSystem, 0, sizeof( g_BaseFuncSystem ) ) ;
    _MEMSET( &BaseImageManage, 0, sizeof( BaseImageManage ) ) ;
	_MEMSET( &g_CharCodeSystem, 0, sizeof( g_CharCodeSystem ) ) ;

	_MEMSET( &HandleManageArray, 0, sizeof( HandleManageArray ) ) ;

#ifndef DX_NON_INPUT
	_MEMSET( &InputSysData, 0, sizeof( InputSysData ) ) ;
#endif // DX_NON_INPUT	

#ifndef DX_NON_INPUTSTRING
	_MEMSET( &CharBuf, 0, sizeof( CharBuf ) ) ;
#endif // DX_NON_INPUTSTRING

#ifndef DX_NON_LIVE2D_CUBISM4
	_MEMSET( &Live2DCubism4SysData, 0, sizeof( Live2DCubism4SysData ) ) ;
	_MEMSET( &Live2DCubism4DLLData, 0, sizeof( Live2DCubism4DLLData ) ) ;
#endif // DX_NON_LIVE2D_CUBISM4

//	size_t g_AndroidSysSize = sizeof( g_AndroidSys )  ;
//	size_t DxSysDataSize = sizeof( DxSysData )  ;
//	size_t DX_ArchiveDirDataSize = sizeof( DX_ArchiveDirData )  ;
//	size_t FontSystemSize = sizeof( FontSystem )  ;
//	size_t GraphicsHardDataAndroidSize = sizeof( GraphicsHardDataAndroid )  ;
//	size_t GraphicsSysDataSize = sizeof( GraphicsSysData )  ;
//	size_t GraphFilterSystemInfoAndroidSize = sizeof( GraphFilterSystemInfoAndroid )  ;
//	size_t GraphFilterShaderHandleSize = sizeof( GraphFilterShaderHandle )  ;
//	size_t MV1Man_AndroidSize = sizeof( MV1Man_Android )  ;
//	size_t MV1ManSize = sizeof( MV1Man )  ;
//	size_t LogData_AndroidSize = sizeof( LogData_Android )  ;
//	size_t LogDataSize = sizeof( LogData )  ;
//	size_t MaskManageData_AndroidSize = sizeof( MaskManageData_Android )  ;
//	size_t MaskManageDataSize = sizeof( MaskManageData )  ;
//	size_t SoftImageManageSize = sizeof( SoftImageManage )  ;
//	size_t MovieGraphManageDataSize = sizeof( MovieGraphManageData )  ;
//	size_t SoundSysDataSize = sizeof( SoundSysData )  ;
//	size_t MidiSystemDataSize = sizeof( MidiSystemData )  ;	
//	size_t GSoundConvertDataSize = sizeof( GSoundConvertData )  ;
//	size_t MemImgManageSize = sizeof( MemImgManage )  ;
//	size_t MemDataSize = sizeof( MemData )  ;
//	size_t g_BaseFuncSystemSize = sizeof( g_BaseFuncSystem )  ;
//	size_t BaseImageManageSize = sizeof( BaseImageManage )  ;
//	size_t g_CharCodeSystemSize = sizeof( g_CharCodeSystem )  ;
//	size_t HandleManageArraySize = sizeof( HandleManageArray )  ;
//	size_t InputSysDataSize = sizeof( InputSysData )  ;
//	size_t GRAPHICS_HARDWARE_ANDROID_SHADER_MODELSize = sizeof( GRAPHICS_HARDWARE_ANDROID_SHADER_MODEL ) ;
//	size_t GRAPHICS_HARDWARE_ANDROID_SHADER_BASE3DSize = sizeof( GRAPHICS_HARDWARE_ANDROID_SHADER_BASE3D ) ;
//	size_t GRAPHICS_HARDWARE_ANDROID_SHADER_BASESize = sizeof( GRAPHICS_HARDWARE_ANDROID_SHADER_BASE ) ;
}

// ソフトの新しい Activity が作成されたときに呼ばれる関数、実質のエントリーポイント
void ANativeActivity_onCreate( ANativeActivity *NativeActivity, void *savedState, size_t savedStateSize )
{
	int lDoubleStartFlag = g_AndroidRunFlag ;

	g_AndroidRunFlag = TRUE ;

	if( lDoubleStartFlag )
	{
		// メインスレッドの終了処理が開始されていたら終了するのを待つ
		if( g_AndroidSys.SoftThreadDestroyedStart != 0 ||
			g_AndroidSys.SoftThreadDestroyed != 0 )
		{
			while( g_AndroidSys.SoftThreadDestroyed == 0 )
			{
				Thread_Sleep( 10 ) ;
			}

			lDoubleStartFlag = FALSE ;
		}
	}

	// グローバル変数の初期化
	if( lDoubleStartFlag == FALSE )
	{
		InitializeGlobalData() ;
	}

	// コールバックの登録
    NativeActivity->callbacks->onStart					= onStart ;
    NativeActivity->callbacks->onResume					= onResume ;
    NativeActivity->callbacks->onPause					= onPause ;
    NativeActivity->callbacks->onStop					= onStop ;
    NativeActivity->callbacks->onDestroy				= onDestroy ;
    NativeActivity->callbacks->onWindowFocusChanged		= onWindowFocusChanged ;
    NativeActivity->callbacks->onNativeWindowCreated	= onNativeWindowCreated ;
    NativeActivity->callbacks->onNativeWindowDestroyed	= onNativeWindowDestroyed ;
    NativeActivity->callbacks->onInputQueueCreated		= onInputQueueCreated ;
    NativeActivity->callbacks->onInputQueueDestroyed	= onInputQueueDestroyed ;

	// シングルインスタンスのみ対応なので、instance には NULL を代入
	NativeActivity->instance = NULL ;

	if( lDoubleStartFlag )
	{
		pthread_mutex_lock( &g_AndroidSys.Mutex ) ;

		// NativeActivity のアドレスを保存
		pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

		g_AndroidSys.NativeActivity = NativeActivity ;

		// Java情報を初期化
		SetupJavaAndroidInfo( NativeActivity->env ) ;

		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

		g_AndroidSys.DestroyRequested = 0 ;

		// NativeActivity アドレスを追加
		if( g_AndroidSys.NativeActivityNum >= g_AndroidSys.NativeActivityBufferLength )
		{
			ANativeActivity **NewNativeActivityBuffer ;
			int NewNativeActivityBufferLength = g_AndroidSys.NativeActivityBufferLength * 2 ;

			NewNativeActivityBuffer = ( ANativeActivity ** )malloc( sizeof( ANativeActivity * ) * NewNativeActivityBufferLength ) ;
			_MEMCPY( NewNativeActivityBuffer, g_AndroidSys.NativeActivityBuffer, sizeof( ANativeActivity * ) * g_AndroidSys.NativeActivityBufferLength ) ;
			free( g_AndroidSys.NativeActivityBuffer ) ;

			g_AndroidSys.NativeActivityBuffer = NewNativeActivityBuffer ;
			g_AndroidSys.NativeActivityBufferLength = NewNativeActivityBufferLength ;
		}
		g_AndroidSys.NativeActivityBuffer[ g_AndroidSys.NativeActivityNum ] = NativeActivity ;
		g_AndroidSys.NativeActivityNum ++ ;

		// フルスクリーンにするかどうかのメタデータの取得
		GetAndroidMetaDataFullScreen( NativeActivity->env ) ;

		// フルスクリーンの場合はステータスバーとナビゲーションバーを非表示にする
		if( g_AndroidSys.IsFullScreen )
		{
			if( SetAndroidWindowStyle( NativeActivity->env ) < 0 )
			{
				// 失敗したら旧バージョン用のフルスクリーン化処理を実行する
				ANativeActivity_setWindowFlags( NativeActivity, AWINDOW_FLAG_FULLSCREEN, AWINDOW_FLAG_FULLSCREEN ) ;
			}
		}

		pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
	}
	else
	{
		// NativeActivity のアドレスを保存
		g_AndroidSys.NativeActivity = NativeActivity ;

		// NativeActivity アドレス格納用バッファの確保
		g_AndroidSys.NativeActivityBufferLength = 16 ;
		g_AndroidSys.NativeActivityBuffer = ( ANativeActivity ** )malloc( sizeof( ANativeActivity * ) * g_AndroidSys.NativeActivityBufferLength ) ;
		g_AndroidSys.NativeActivityBuffer[ 0 ] = NativeActivity ;
		g_AndroidSys.NativeActivityNum = 1 ;

		// Java情報を初期化
		SetupJavaAndroidInfo( NativeActivity->env ) ;

		// ランダム係数を初期化
#ifndef DX_NON_MERSENNE_TWISTER
		srandMT( ( unsigned int )NS_GetNowCount( FALSE ) ) ;
#else
		srand( NS_GetNowCount( FALSE ) ) ;
#endif

		// ミューテックスト条件変数を初期化
	    pthread_mutex_init( &g_AndroidSys.Mutex, NULL ) ;
		pthread_cond_init(  &g_AndroidSys.Cond,  NULL ) ;
		pthread_mutex_init( &g_AndroidSys.NativeActivityMutex, NULL ) ;

		// ソフト実行用スレッドとの通信用のパイプ生成
		int msgpipe[ 2 ] ;
		if( pipe( msgpipe ) )
		{
			// パイプ生成失敗
//			LOGE( "could not create pipe: %s", strerror( errno ) ) ;
			return ;
		}
		g_AndroidSys.MessageRead  = msgpipe[ 0 ] ;
		g_AndroidSys.MessageWrite = msgpipe[ 1 ] ;

		// ソフト実行用スレッド生成
		pthread_attr_t attr ;
		pthread_attr_init( &attr ) ;
		pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED ) ;
		pthread_attr_setstacksize( &attr, 4 * 1024 * 1024 ) ;	// スタックサイズは 4MB
		pthread_create( &g_AndroidSys.SoftThread, &attr, android_app_entry, NULL ) ;

		// スレッドの開始を待機
		pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
		g_AndroidSys.MutexLockIndex = 1 << 3 ;
		while( g_AndroidSys.SoftThreadRunning == 0 )
		{
			pthread_cond_wait( &g_AndroidSys.Cond, &g_AndroidSys.Mutex ) ;
		}
		g_AndroidSys.MutexLockIndex &= ~( 1 << 3 ) ;
		pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
	}
}












#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// ウインドウズのメッセージループに代わる処理を行う
extern int NS_ProcessMessage( void )
{
	static int EndFlag = FALSE ;

	// もしフラグがたっていたらなにもせず終了
	if( EndFlag )
	{
		return 0 ;
	}

	// ファイル処理の周期的処理を行う
//	ReadOnlyFileAccessProcessAll() ;

#ifndef DX_NON_SOUND
	{
		// サウンドの周期的処理を行う
//		NS_ProcessStreamSoundMemAll() ;
//		ST_SoftSoundPlayerProcessAll() ;
		ProcessPlayFinishDeleteSoundMemAll() ;
//		SoundBuffer_Apply_StopSoundBufferList() ;
//		ProcessPlay3DSoundMemAll() ;
	}
#endif // DX_NON_SOUND

#ifndef DX_NON_ASYNCLOAD
	// 削除リクエストが来ているハンドルを削除する
	DeleteRequestHandleDelete( FALSE ) ;

	// メインスレッドが処理する非同期読み込みの処理を行う
	ProcessASyncLoadRequestMainThread() ;
#endif // DX_NON_ASYNCLOAD

	// 演奏の周期的処理を行う
#ifndef DX_NON_SOUND
	NS_ProcessMusicMem() ;
#endif // DX_NON_SOUND

#ifndef DX_NON_INPUT
	// キーボード入力の更新処理を行う
	UpdateKeyboardInputState( FALSE ) ;

	// パッドの周期的処理を行う
	JoypadEffectProcess() ;
#endif // DX_NON_INPUT

#ifndef DX_NON_NETWORK
	// 通信関係のメッセージ処理を行う
	NS_ProcessNetMessage( TRUE ) ;
#endif

	// メモリ関係の周期的処理を行う
	MemoryProcess() ;

#ifndef DX_NON_SOUND
	// サウンド関係の周期的処理を行う
	ProcessSoundSystem() ;
#endif // DX_NON_SOUND

#ifndef DX_NON_GRAPHICS
	// 画面関係の周期処理を行う
	Graphics_Android_FrontScreenProcess() ;
#endif // DX_NON_GRAPHICS

#ifndef DX_NON_KEYEX
	// キー入力処理を行う
	{
		// フラグをたてる
		EndFlag = TRUE ;

		NS_ProcessActKeyInput() ;

		// フラグを倒す
		EndFlag = FALSE ;
	}
#endif

	// イベント処理ループ
	for(;;)
	{
		int   events ;
		void *source ;

		// イベントの取得、ソフト実行用スレッドが待ち状態の場合はイベントが来るまで延々と待つ
		int ident = ALooper_pollOnce( ( g_AndroidSys.SoftThreadWait && g_AndroidSys.NonActiveRunFlag == FALSE ) ? -1 : 0, NULL, &events, &source ) ;
		if( ident < 0 ) 
		{
			break ;
		}

		// イベントID 毎に処理を分岐
		switch( ident )
		{
		// メインスレッドから送られてきたコマンドの場合
		case DX_LOOPER_ID_MAIN :
			{
				int8_t cmd ;

				// メインスレッドから送られてきたコマンドを取得
				if( read( g_AndroidSys.MessageRead, &cmd, sizeof( cmd ) ) == sizeof( cmd ) )
				{
					// コマンドの種類によって処理を分岐
					switch( cmd ) 
					{
					// 入力イベントを受け取るキューの変更
					case DX_ANDR_CMD_INPUT_CHANGED :
						pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
						g_AndroidSys.MutexLockIndex = 1 << 4 ;

						// 今までの入力イベント受け取りキューを Looper からデタッチ
						if( g_AndroidSys.InputQueue != NULL )
						{
							AInputQueue_detachLooper( ( AInputQueue * )g_AndroidSys.InputQueue ) ;
						}

						// 新しい入力イベント受け取りキューのアドレスを保存
						g_AndroidSys.InputQueue = g_AndroidSys.NewInputQueue ;

						// 新しい入力イベント受け取りキューを Looper に登録
						if( g_AndroidSys.InputQueue != NULL )
						{
							AInputQueue_attachLooper(
								( AInputQueue * )g_AndroidSys.InputQueue,
								g_AndroidSys.Looper,
								DX_LOOPER_ID_INPUT,
								NULL,
								NULL
							) ;
						}

						pthread_cond_broadcast( &g_AndroidSys.Cond ) ;
						g_AndroidSys.MutexLockIndex &= ~( 1 << 4 ) ;
						pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
						break;

					// ActivityState を変更するだけのコマンド
					case DX_ANDR_CMD_RESUME :
					case DX_ANDR_CMD_START :
					case DX_ANDR_CMD_PAUSE :
					case DX_ANDR_CMD_STOP :
						pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
						g_AndroidSys.MutexLockIndex = 1 << 5 ;

			#ifndef DX_NON_INPUT
						// 何もタッチしていないことにする
						{
							TOUCHINPUTDATA TouchInputData ;

							TouchInputData.PointNum = 0 ;
							TouchInputData.Time = ( LONGLONG )NS_GetNowCount( FALSE ) ;

							AddTouchInputData( &TouchInputData ) ;
						}
			#endif // DX_NON_INPUT

						// 新しい ActivityState を保存
						g_AndroidSys.ActivityState = cmd ;

						if( cmd == DX_ANDR_CMD_RESUME ||
							cmd == DX_ANDR_CMD_START )
						{
							if( g_AndroidSys.SoundAndMoviePause == TRUE )
							{
								g_AndroidSys.SoundAndMoviePause = FALSE ;

					#ifndef DX_NON_MOVIE
								// ムービーグラフィックの再生状態を戻す
								PlayMovieAll() ;
					#endif

					#ifndef DX_NON_SOUND
								// サウンドの再生を再開する
								PauseSoundMemAll( FALSE ) ;
								PauseSoftSoundAll( FALSE ) ;
					#endif // DX_NON_SOUND
							}
						}
						else
						{
							if( g_AndroidSys.SoundAndMoviePause == FALSE )
							{
								g_AndroidSys.SoundAndMoviePause = TRUE ;

					#ifndef DX_NON_MOVIE
								// ムービーグラフィックの再生状態をとめる
								PauseMovieAll() ;
					#endif

					#ifndef DX_NON_SOUND
								// サウンドの再生を止める
								PauseSoundMemAll( TRUE ) ;
								PauseSoftSoundAll( TRUE ) ;
					#endif // DX_NON_SOUND
							}
						}

						pthread_cond_broadcast( &g_AndroidSys.Cond ) ;
						g_AndroidSys.MutexLockIndex &= ~( 1 << 5 ) ;
						pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
						break;

					// onDestroy が呼ばれた
					case DX_ANDR_CMD_DESTROY :
						// onDestroy が呼ばれたフラグを立てる
						pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
						g_AndroidSys.DestroyRequested = 1 ;
						pthread_cond_broadcast( &g_AndroidSys.Cond ) ;
						pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;

						// onDestroy が呼ばれた時刻を記録
						g_AndroidSys.DestroyRequestedTime = NS_GetNowCount( FALSE ) ;
						break ;

					// ウインドウの変更
					case DX_ANDR_CMD_WINDOW_CHANGED :
						pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
						g_AndroidSys.MutexLockIndex = 1 << 6 ;

						// 既存のウインドウがある場合はウインドウの後始末を行う
						if( g_AndroidSys.NativeWindow != NULL )
						{
							pthread_cond_broadcast( &g_AndroidSys.Cond ) ;

					#ifndef DX_NON_GRAPHICS
							if( DxSysData.NotDrawFlag == FALSE )
							{
					#ifndef DX_NON_MASK
								// マスクサーフェスを一時削除
								Mask_ReleaseSurface() ;
					#endif // DX_NON_MASK
								// グラフィックハンドルが持つ OpenGL ES オブジェクトの解放
								Graphics_Android_ReleaseObjectAll() ;

								// システムが持つ OpenGL ES オブジェクトの解放
								Graphics_Android_Terminate() ;
							}
					#endif // DX_NON_GRAPHICS

							g_AndroidSys.NativeWindow = NULL ;
						}

						// 新しいウインドウがある場合はウインドウの初期化を行う
						if( g_AndroidSys.NewNativeWindow != NULL )
						{
							g_AndroidSys.NativeWindow = g_AndroidSys.NewNativeWindow ;

				#ifndef DX_NON_GRAPHICS
							if( DxSysData.DxLib_InitializeFlag )
							{
								// グラフィックシステムの復帰処理
								NS_RestoreGraphSystem() ;

//								if( g_AndroidSys.SoundAndMoviePause == TRUE )
//								{
//									g_AndroidSys.SoundAndMoviePause = FALSE ;
//
//					#ifndef DX_NON_MOVIE
//									// ムービーグラフィックの再生状態を戻す
//									PlayMovieAll() ;
//					#endif
//
//					#ifndef DX_NON_SOUND
//									// サウンドの再生を再開する
//									PauseSoundMemAll( FALSE ) ;
//									PauseSoftSoundAll( FALSE ) ;
//					#endif // DX_NON_SOUND
//
//									// コールバック関数が登録されている場合は呼ぶ
//									if( g_AndroidSys.GainedFocusCallbackFunction != NULL )
//									{
//										g_AndroidSys.GainedFocusCallbackFunction( ( void * )g_AndroidSys.GainedFocusCallbackFunctionData ) ;
//									}
//								}

								// ソフト実行用スレッドを待ち状態から解除
								g_AndroidSys.SoftThreadWait = 0 ;
							}
				#endif // DX_NON_GRAPHICS
						}

						pthread_cond_broadcast( &g_AndroidSys.Cond ) ;
						g_AndroidSys.MutexLockIndex &= ~( 1 << 6 ) ;
						pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
						break ;

					// ウインドウがアクティブになった
					case DX_ANDR_CMD_GAINED_FOCUS :
						// センサーを有効にする
						for( int i = 0 ; i < DX_ANDROID_SENSOR_NUM ; i ++ )
						{
							if( g_AndroidSys.SensorInfos[ i ].Sensor           != NULL &&
								g_AndroidSys.SensorInfos[ i ].SensorEventQueue != NULL )
							{
								ASensorEventQueue_enableSensor(
									g_AndroidSys.SensorInfos[ i ].SensorEventQueue,
									g_AndroidSys.SensorInfos[ i ].Sensor
								) ;

								ASensorEventQueue_setEventRate(
									g_AndroidSys.SensorInfos[ i ].SensorEventQueue,
									g_AndroidSys.SensorInfos[ i ].Sensor, 
									( 1000L / 60 ) * 1000
								) ;
							}
						}

//						if( g_AndroidSys.SoundAndMoviePause == TRUE )
//						{
//							g_AndroidSys.SoundAndMoviePause = FALSE ;
//
//			#ifndef DX_NON_MOVIE
//							// ムービーグラフィックの再生状態を戻す
//							PlayMovieAll() ;
//			#endif
//
//			#ifndef DX_NON_SOUND
//							// サウンドの再生を再開する
//							PauseSoundMemAll( FALSE ) ;
//							PauseSoftSoundAll( FALSE ) ;
//			#endif // DX_NON_SOUND
//
							// コールバック関数が登録されている場合は呼ぶ
							if( g_AndroidSys.GainedFocusCallbackFunction != NULL )
							{
								g_AndroidSys.GainedFocusCallbackFunction( ( void * )g_AndroidSys.GainedFocusCallbackFunctionData ) ;
							}
//						}

						// ソフト実行用スレッドを待ち状態から解除
						g_AndroidSys.SoftThreadWait = 0 ;
						break ;

					// ウインドウが非アクティブになった
					case DX_ANDR_CMD_LOST_FOCUS :
						// センサーを無効にする
						for( int i = 0 ; i < DX_ANDROID_SENSOR_NUM ; i ++ )
						{
							if( g_AndroidSys.SensorInfos[ i ].Sensor           != NULL &&
								g_AndroidSys.SensorInfos[ i ].SensorEventQueue != NULL )
							{
								ASensorEventQueue_disableSensor(
									g_AndroidSys.SensorInfos[ i ].SensorEventQueue,
									g_AndroidSys.SensorInfos[ i ].Sensor
								) ;
							}
						}

//						if( g_AndroidSys.SoundAndMoviePause == FALSE )
//						{
//							g_AndroidSys.SoundAndMoviePause = TRUE ;
//
//				#ifndef DX_NON_MOVIE
//							// ムービーグラフィックの再生状態をとめる
//							PauseMovieAll() ;
//				#endif
//
//				#ifndef DX_NON_SOUND
//							// サウンドの再生を止める
//							PauseSoundMemAll( TRUE ) ;
//							PauseSoftSoundAll( TRUE ) ;
//				#endif // DX_NON_SOUND
//
							// コールバック関数が登録されている場合は呼ぶ
							if( g_AndroidSys.LostFocusCallbackFunction != NULL )
							{
								g_AndroidSys.LostFocusCallbackFunction( ( void * )g_AndroidSys.LostFocusCallbackFunctionData ) ;
							}
//						}

						// ソフト実行用スレッドを待ち状態にする
						g_AndroidSys.SoftThreadWait = 1 ;
						break ;
					}
				}
			}
			break ;

		// 入力イベントの場合
		case DX_LOOPER_ID_INPUT :
			{
				AInputEvent *event = NULL ;

				// 入力イベントがある場合はループ
				while( AInputQueue_getEvent( ( AInputQueue * )g_AndroidSys.InputQueue, &event ) >= 0 )
				{
					// 入力イベントをディスパッチ
					if( AInputQueue_preDispatchEvent( ( AInputQueue * )g_AndroidSys.InputQueue, event ) )
					{
						continue ;
					}
					int32_t handled = 0 ;

					if( g_AndroidSys.UserInputEventCallbackFunction != NULL )
					{
						handled = g_AndroidSys.UserInputEventCallbackFunction( event, g_AndroidSys.UserInputEventCallbackFunctionData ) ;
					}

			#ifndef DX_NON_INPUT
					// 入力イベントを処理
					if( handled == 0 )
					{
						handled = ProcessInputEvent( event ) ;
					}
			#endif // DX_NON_INPUT

					// 入力イベントを完了状態にする
					AInputQueue_finishEvent( ( AInputQueue * )g_AndroidSys.InputQueue, event, handled ) ;
				}
			}
			break ;

		// センサーイベントの場合
		case DX_LOOPER_ID_SENSOR_ACCELEROMETER :
		case DX_LOOPER_ID_SENSOR_MAGNETIC_FIELD :
		case DX_LOOPER_ID_SENSOR_GYROSCOPE :
		case DX_LOOPER_ID_SENSOR_LIGHT :
		case DX_LOOPER_ID_SENSOR_PROXIMITY :
		case DX_LOOPER_ID_SENSOR_PRESSURE :
		case DX_LOOPER_ID_SENSOR_AMBIENT_TEMPERATURE :
			{
				int SensorType = ident - DX_LOOPER_ID_SENSOR_ACCELEROMETER ;

				if( g_AndroidSys.SensorInfos[ SensorType ].Sensor != NULL &&
					g_AndroidSys.SensorInfos[ SensorType ].SensorEventQueue != NULL )
				{
					// イベント情報を取得
					while( ASensorEventQueue_getEvents( g_AndroidSys.SensorInfos[ SensorType ].SensorEventQueue, &g_AndroidSys.SensorInfos[ SensorType ].SensorEvent, 1 ) > 0 ){}
				}
			}
			break ;
		}

		// onDestroy が呼ばれているか、g_AndroidSys.SoftThreadDestroyedStart が 1 の場合はループを抜ける
		if( g_AndroidSys.DestroyRequested != 0 ||
			g_AndroidSys.SoftThreadDestroyedStart != 0 )
		{
			break ;
		}
	}

	// g_AndroidSys.SoftThreadDestroyedStart が 1 だったら -1 を返す
	if( g_AndroidSys.SoftThreadDestroyedStart != 0 )
	{
		return -1 ;
	}
	else
	// onDestroy が呼ばれて次の NativeActivity が実行されず、且つ 0.5秒以上経過した場合は -1 を返す
	if( g_AndroidSys.DestroyRequested != 0 )
	{
		int ActivityNum = 0 ;

		pthread_mutex_lock( &g_AndroidSys.Mutex ) ;
		ActivityNum = g_AndroidSys.NativeActivityNum ;

		if( ActivityNum == 0 )
		{
			int NowTime = NS_GetNowCount( FALSE ) ;
			if( TIME_DISTANCE( NowTime, g_AndroidSys.DestroyRequestedTime ) > 500 )
			{
				// 終了開始フラグを立てる
				g_AndroidSys.SoftThreadDestroyedStart = 1 ;

				pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
				return -1 ;
			}
		}
		else
		{
			pthread_cond_broadcast( &g_AndroidSys.Cond ) ;
		}

		pthread_mutex_unlock( &g_AndroidSys.Mutex ) ;
	}

	// 通常終了
	return 0 ;
}

// アプリがアクティブではない状態でも処理を続行するか、フラグをセットする
extern int NS_SetAlwaysRunFlag( int Flag )
{
	// フラグをセット
	g_AndroidSys.NonActiveRunFlag = Flag ;
	
	// 終了
	return 0 ;
}

// Android の APIレベルを取得する
extern int GetAndroidAPILevel( void )
{
	return JAVAANDR.fieldint_Build_VERSION_SDK_INT ;
}

// ソフトのデータ保存用のディレクトリパスを取得する
extern int GetInternalDataPath( TCHAR *PathBuffer, int PathBufferBytes )
{
	TCHAR *TempBuffer ;
	int Result ;

	if( g_AndroidSys.NativeActivity == NULL ||
		g_AndroidSys.NativeActivity->internalDataPath == NULL )
	{
		return -1 ;
	}

	if( PathBuffer == NULL )
	{
		size_t Length ;
		size_t BufferSize ;

		Length = CL_strlen( DX_CHARCODEFORMAT_UTF8, g_AndroidSys.NativeActivity->internalDataPath ) ;
		BufferSize = ( Length + 1 ) * 16 ;
		TempBuffer = ( TCHAR * )DXALLOC( BufferSize ) ;
		if( TempBuffer == NULL )
		{
			return -1 ;
		}
		Result = ConvString( g_AndroidSys.NativeActivity->internalDataPath, -1, DX_CHARCODEFORMAT_UTF8, TempBuffer, BufferSize, _TCHARCODEFORMAT ) ;
		DXFREE( TempBuffer ) ;
		TempBuffer = NULL ;
	}
	else
	{
		Result = ConvString( g_AndroidSys.NativeActivity->internalDataPath, -1, DX_CHARCODEFORMAT_UTF8, PathBuffer, PathBufferBytes, _TCHARCODEFORMAT ) ;
	}

	// 終了
	return Result ;
}

// ソフトの外部データ保存用のディレクトリパスを取得する
// PathBuffer : パスを保存するバッファ
// PathBufferBytes : パスを保存するバッファのサイズ( 単位：バイト )
// PathIndex : 外部データ保存用のディレクトリパスの番号( 複数個ある場合があるので )
// IsRemovableFlag : PathIndex で指定したパスが取り外し可能なデバイスかどうかを保存する int型変数のアドレス( 格納された値　TRUE:取り外し可能( SDカード等 )  FALSE:取り外し不可能( 内蔵ストレージ等 ) )
extern int GetExternalDataPath( TCHAR *PathBuffer, int PathBufferBytes, int PathIndex, int *IsRemovableFlag )
{
	if( PathIndex == 0 && IsRemovableFlag == NULL )
	{
		TCHAR *TempBuffer ;
		int Result ;

		if( g_AndroidSys.NativeActivity == NULL ||
			g_AndroidSys.NativeActivity->externalDataPath == NULL )
		{
			return -1 ;
		}

		if( PathBuffer == NULL )
		{
			size_t Length ;
			size_t BufferSize ;

			Length = CL_strlen( DX_CHARCODEFORMAT_UTF8, g_AndroidSys.NativeActivity->externalDataPath ) ;
			BufferSize = ( Length + 1 ) * 16 ;
			TempBuffer = ( TCHAR * )DXALLOC( BufferSize ) ;
			if( TempBuffer == NULL )
			{
				return -1 ;
			}
			Result = ConvString( g_AndroidSys.NativeActivity->externalDataPath, -1, DX_CHARCODEFORMAT_UTF8, TempBuffer, BufferSize, _TCHARCODEFORMAT ) ;
			DXFREE( TempBuffer ) ;
			TempBuffer = NULL ;
		}
		else
		{
			Result = ConvString( g_AndroidSys.NativeActivity->externalDataPath, -1, DX_CHARCODEFORMAT_UTF8, PathBuffer, PathBufferBytes, _TCHARCODEFORMAT ) ;
		}

		// 終了
		return Result ;
	}
	else
	{
		JNIEnv *env ;
		jobjectArray objectArray_File = NULL ;
		jobject object_File = NULL ;
		jstring string_DirPath = NULL ;
		jsize dirNum = 0 ;
		int Result = -1 ;
		TCHAR *DirPathTemp = NULL ;

		if( IsRemovableFlag != NULL )
		{
			*IsRemovableFlag = FALSE ;
		}

		pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
		if( g_AndroidSys.NativeActivity == NULL )
		{
			pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
			return -1 ;
		}

		// JavaVM とソフト実行用スレッドを関連付け
		if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
		{
			pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
			return -1 ;
		}

		// getExternalFilesDirs が使用できない場合はエラー
		if( JAVAANDR.methodID_Context_getExternalFilesDirs == NULL )
		{
			goto END ;
		}

		// ディレクトリリストを取得
		objectArray_File = ( jobjectArray )env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getExternalFilesDirs, NULL ) ;
		if( objectArray_File == NULL )
		{
			goto END ;
		}

		// ディレクトリの数を取得
		dirNum = env->GetArrayLength( objectArray_File ) ;
		if( dirNum <= PathIndex )
		{
			goto END ;
		}

		// 指定の要素の取り出し
		object_File = env->GetObjectArrayElement( objectArray_File, PathIndex ) ;
		if( object_File == NULL )
		{
			goto END ;
		}

		// 取り外し可能かどうかを取得する
		if( IsRemovableFlag != NULL && JAVAANDR.methodID_Environment_isExternalStorageRemovable != NULL )
		{
			*IsRemovableFlag = env->CallStaticBooleanMethod( JAVAANDR.class_Environment, JAVAANDR.methodID_Environment_isExternalStorageRemovable, object_File ) == JNI_TRUE ? TRUE : FALSE ;
		}

		// ディレクトリパスの取得
		string_DirPath = ( jstring )env->CallObjectMethod( object_File, JAVAANDR.methodID_File_getAbsolutePath ) ;
		if( string_DirPath == NULL )
		{
			goto END ;
		}

		// TCHAR の文字列に変換
		if( Java_Create_TCHAR_string_From_jstring( env, string_DirPath, &DirPathTemp ) < 0 )
		{
			goto END ;
		}

		Result = ( CL_strlen( _TCHARCODEFORMAT, DirPathTemp ) + 1 ) * GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) ;
		if( PathBuffer != NULL )
		{
			CL_strcpy_s( _TCHARCODEFORMAT, PathBuffer, PathBufferBytes, DirPathTemp ) ;
		}

END :
		if( DirPathTemp != NULL )
		{
			DXFREE( DirPathTemp ) ;
			DirPathTemp = NULL ;
		}

		if( objectArray_File != NULL )
		{
			env->DeleteLocalRef( objectArray_File ) ;
			objectArray_File = NULL ;
		}

		if( object_File != NULL )
		{
			env->DeleteLocalRef( object_File ) ;
			object_File = NULL ;
		}

		if( string_DirPath != NULL )
		{
			env->DeleteLocalRef( string_DirPath ) ;
			string_DirPath = NULL ;
		}

		// JavaVM とこのスレッドの関連付け終了
		g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

		return Result ;
	}
}

// ソフトの外部データ保存用のディレクトリパスの数を取得する
extern int GetExternalDataPathNum( void )
{
	JNIEnv *env ;
	jobjectArray objectArray_File = NULL ;
	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// getExternalFilesDirs が使用できない場合はエラー
	if( JAVAANDR.methodID_Context_getExternalFilesDirs == NULL )
	{
		goto END ;
	}

	// ディレクトリリストを取得
	objectArray_File = ( jobjectArray )env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getExternalFilesDirs, NULL ) ;
	if( objectArray_File == NULL )
	{
		goto END ;
	}

	// ディレクトリの数を取得
	Result = env->GetArrayLength( objectArray_File ) ;

END :
	if( objectArray_File != NULL )
	{
		env->DeleteLocalRef( objectArray_File ) ;
		objectArray_File = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

// 端末に設定されている言語を取得する
extern int GetLanguage( TCHAR *StringBuffer, int StringBufferSize )
{
	JNIEnv *env ;
	jobject object_Locale = NULL ;
	jstring jstring_Language = NULL ;
	int res = -1 ;
	TCHAR *LanguageName = NULL ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// ロケールの取得
	object_Locale = env->CallStaticObjectMethod( JAVAANDR.class_Locale, JAVAANDR.methodID_Locale_getDefault ) ;
	if( object_Locale == NULL )
	{
		goto END ;
	}

	// 言語の取得
	jstring_Language = ( jstring )env->CallObjectMethod( object_Locale, JAVAANDR.methodID_Locale_getLanguage );
	if( jstring_Language == NULL )
	{
		goto END ;
	}

	// TCHAR 文字列に変換
	if( Java_Create_TCHAR_string_From_jstring( env, jstring_Language, &LanguageName ) < 0 )
	{
		goto END ;
	}

	// バッファにコピーする
	if( StringBuffer != NULL )
	{
		CL_strcpy_s( _TCHARCODEFORMAT, StringBuffer, StringBufferSize, LanguageName ) ;  
	}

	res = ( CL_strlen( _TCHARCODEFORMAT, StringBuffer ) + 1 ) * GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) ;

END :

	if( LanguageName != NULL )
	{
		DXFREE( LanguageName ) ;
		LanguageName = NULL ;
	}

	if( object_Locale != NULL )
	{
		env->DeleteLocalRef( object_Locale ) ;
		object_Locale = NULL ;
	}

	if( jstring_Language != NULL )
	{
		env->DeleteLocalRef( jstring_Language ) ;
		jstring_Language = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return res ;
}

// 端末に設定されている国を取得する( 戻り値　-1：エラー　0以上：国名文字列の格納に必要なバイト数 )
extern int GetCountry( TCHAR *StringBuffer, int StringBufferSize )
{
	JNIEnv *env ;
	jobject object_Locale = NULL ;
	jstring jstring_Country = NULL ;
	int res = -1 ;
	TCHAR *CountryName = NULL ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// ロケールの取得
	object_Locale = env->CallStaticObjectMethod( JAVAANDR.class_Locale, JAVAANDR.methodID_Locale_getDefault ) ;
	if( object_Locale == NULL )
	{
		goto END ;
	}

	// 言語の取得
	jstring_Country = ( jstring )env->CallObjectMethod( object_Locale, JAVAANDR.methodID_Locale_getCountry );
	if( jstring_Country == NULL )
	{
		goto END ;
	}

	// TCHAR 文字列に変換
	if( Java_Create_TCHAR_string_From_jstring( env, jstring_Country, &CountryName ) < 0 )
	{
		goto END ;
	}

	// バッファにコピーする
	if( StringBuffer != NULL )
	{
		CL_strcpy_s( _TCHARCODEFORMAT, StringBuffer, StringBufferSize, CountryName ) ;  
	}

	res = ( CL_strlen( _TCHARCODEFORMAT, StringBuffer ) + 1 ) * GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) ;

END :

	if( CountryName != NULL )
	{
		DXFREE( CountryName ) ;
		CountryName = NULL ;
	}

	if( object_Locale != NULL )
	{
		env->DeleteLocalRef( object_Locale ) ;
		object_Locale = NULL ;
	}

	if( jstring_Country != NULL )
	{
		env->DeleteLocalRef( jstring_Country ) ;
		jstring_Country = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return res ;
}

// メモリ情報を取得する
// TotalMemory : 総メモリ容量を代入する変数( 単位：byte )、( 情報の取得が不要な場合は NULL を渡す )
// FreeMrmoyy : 空きメモリ容量を代入する変数( 単位：byte )、( 情報の取得が不要な場合は NULL を渡す )
// UseMemory : アプリが使用しているメモリ容量を代入する変数( 単位:byte )、( 情報の取得が不要な場合は NULL を渡す )、( 使用メモリ量の取得は非常に処理負荷が高いので、不要な場合は必ず NULL を渡すようにしてください )
extern int GetAndroidMemoryInfo( LONGLONG *TotalMemory, LONGLONG *FreeMemory, LONGLONG *UseMemory )
{
	JNIEnv *env ;
	int res = -1 ;
//	jobject object_Runtime = NULL ;
//	jlong long_TotalMemory = 0 ;
//	jlong long_FreeMemory = 0 ;
	jobject object_ActivityManager = NULL ;
	jobject object_MemoryInfo = NULL ;
	jobjectArray object_Debug_MemoryInfos = NULL ;
	jobject object_Debug_MemoryInfo = NULL ;
	jlong long_MemoryInfo_availMem = 0 ;
//	jboolean boolean_MemoryInfo_lowMemory = 0 ;
//	jlong long_MemoryInfo_threshold = 0 ;
	jlong long_MemoryInfo_totalMem = 0 ;
	jintArray intArray_pids = NULL ;
	jint* intArray_Element = NULL ;
	jint int_TotalPss = 0 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// ActivityManagerの取得
	object_ActivityManager = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_ACTIVITY_SERVICE ) ;
	if( object_ActivityManager == NULL )
	{
		goto END ;
	}

	// 使用メモリ量の取得
	if( UseMemory != NULL )
	{
		// PIDを代入する長さ１のint型配列を作成する
		intArray_pids = env->NewIntArray( 1 ) ;
		if( intArray_pids == NULL )
		{
			goto END ;
		}

		// PID を代入
		intArray_Element = env->GetIntArrayElements( intArray_pids, NULL ) ;
		if( intArray_Element == NULL )
		{
			goto END ;
		}
		intArray_Element[ 0 ] = env->CallStaticIntMethod( JAVAANDR.class_Process, JAVAANDR.methodID_Process_myPid ) ;
		env->ReleaseIntArrayElements( intArray_pids, intArray_Element, 0 ) ;

		// アプリのメモリ情報を取得
		object_Debug_MemoryInfos = ( jobjectArray )env->CallObjectMethod( object_ActivityManager, JAVAANDR.methodID_ActivityManager_getProcessMemoryInfo, intArray_pids ) ;
		if( object_Debug_MemoryInfos == NULL )
		{
			goto END ;
		}

		// アプリの使用メモリ量を取得
		if( env->GetArrayLength( object_Debug_MemoryInfos ) < 1 )
		{
			goto END ;
		}
		object_Debug_MemoryInfo = env->GetObjectArrayElement( object_Debug_MemoryInfos, 0 ) ;
		if( object_Debug_MemoryInfo == NULL )
		{
			goto END ;
		}
		int_TotalPss = env->CallIntMethod( object_Debug_MemoryInfo, JAVAANDR.methodID_Debug_MemoryInfo_getTotalPss ) ;
	}

	// MemoryInfo の作成
	object_MemoryInfo = env->NewObject( JAVAANDR.class_ActivityManager_MemoryInfo, JAVAANDR.methodID_ActivityManager_MemoryInfo_newActivityManager_MemoryInfo ) ;
	if( object_MemoryInfo == NULL )
	{
		goto END ;
	}

	// MemoryInfo の取得
	env->CallVoidMethod( object_ActivityManager, JAVAANDR.methodID_ActivityManager_getMemoryInfo, object_MemoryInfo ) ;

	// メモリ情報の取得
	long_MemoryInfo_availMem = env->GetLongField( object_MemoryInfo, JAVAANDR.fieldID_ActivityManager_MemoryInfo_availMem ) ;
//	boolean_MemoryInfo_lowMemory = env->GetBooleanField( object_MemoryInfo, JAVAANDR.fieldID_ActivityManager_MemoryInfo_lowMemory ) ;
//	long_MemoryInfo_threshold = env->GetLongField( object_MemoryInfo, JAVAANDR.fieldID_ActivityManager_MemoryInfo_threshold ) ;
	if( JAVAANDR.fieldint_Build_VERSION_SDK_INT >= 16 )
	{
		long_MemoryInfo_totalMem = env->GetLongField( object_MemoryInfo, JAVAANDR.fieldID_ActivityManager_MemoryInfo_totalMem ) ;
	}
	else
	{
		long_MemoryInfo_totalMem = long_MemoryInfo_availMem ;
	}

//	// Runtimeオブジェクトを取得
//	object_Runtime = env->CallStaticObjectMethod( JAVAANDR.class_Runtime, JAVAANDR.methodID_Runtime_getRuntime ) ;
//	if( object_Runtime == NULL )
//	{
//		goto END ;
//	}
//
//	// 総メモリ容量の取得
//	long_TotalMemory = env->CallLongMethod( object_Runtime, JAVAANDR.methodID_Runtime_totalMemory ) ;
//
//	// 空きメモリ容量の取得
//	long_FreeMemory = env->CallLongMethod( object_Runtime, JAVAANDR.methodID_Runtime_freeMemory ) ;

	// 値を代入
	if( TotalMemory != NULL )
	{
		*TotalMemory = long_MemoryInfo_totalMem ;
	}
	if( FreeMemory != NULL )
	{
		*FreeMemory = long_MemoryInfo_availMem ;
	}
	if( UseMemory != NULL )
	{
		*UseMemory = int_TotalPss * 1024 ;
	}

	res = 0 ;

END :

//	if( object_Runtime != NULL )
//	{
//		env->DeleteLocalRef( object_Runtime ) ;
//		object_Runtime = NULL ;
//	}

	if( object_ActivityManager != NULL )
	{
		env->DeleteLocalRef( object_ActivityManager ) ;
		object_ActivityManager = NULL ;
	}

	if( object_MemoryInfo != NULL )
	{
		env->DeleteLocalRef( object_MemoryInfo ) ;
		object_MemoryInfo = NULL ;
	}

	if( object_Debug_MemoryInfo != NULL )
	{
		env->DeleteLocalRef( object_Debug_MemoryInfo ) ;
		object_Debug_MemoryInfo = NULL ;
	}

	if( object_Debug_MemoryInfos != NULL )
	{
		env->DeleteLocalRef( object_Debug_MemoryInfos ) ;
		object_Debug_MemoryInfos = NULL ;
	}

	if( intArray_pids != NULL )
	{
		env->DeleteLocalRef( intArray_pids ) ;
		intArray_pids = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return res ;
}

// 文字列入力ダイアログを表示する
static int StartInputStringDialogStatic( JNIEnv *env, const TCHAR *Title )
{
	int res = -1 ;

	// EditText の作成
	{
		jobject object_EditText = env->NewObject(
			g_JavaAndroidInfo.class_EditText,
			g_JavaAndroidInfo.methodID_EditText_newEditText,
			g_AndroidSys.NativeActivity->clazz
		) ;
		if( object_EditText != NULL )
		{
			g_AndroidSys.object_EditText = env->NewGlobalRef( object_EditText ) ;
			env->DeleteLocalRef( object_EditText ) ;
			object_EditText = NULL ;
		}
		if( g_AndroidSys.object_EditText == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x45\x00\x64\x00\x69\x00\x74\x00\x54\x00\x65\x00\x78\x00\x74\x00\x20\x00\x6e\x30\x20\x00\x6e\x00\x65\x00\x77\x00\x20\x00\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"EditText の new に失敗しました" @*/ )) ;
			goto END ;
		}
	}

	// AlertDialog.Builder の作成
	{
		jobject object_AlertDialog_Builder = env->NewObject(
			g_JavaAndroidInfo.class_AlertDialog_Builder,
			g_JavaAndroidInfo.methodID_AlertDialog_Builder_newAlertDialog_Builder,
			g_AndroidSys.NativeActivity->clazz
		) ;
		if( object_AlertDialog_Builder != NULL )
		{
			g_AndroidSys.object_AlertDialog_Builder = env->NewGlobalRef( object_AlertDialog_Builder ) ;
			env->DeleteLocalRef( object_AlertDialog_Builder ) ;
			object_AlertDialog_Builder = NULL ;
		}
		if( g_AndroidSys.object_AlertDialog_Builder == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x41\x00\x6c\x00\x65\x00\x72\x00\x74\x00\x44\x00\x69\x00\x61\x00\x6c\x00\x6f\x00\x67\x00\x2e\x00\x42\x00\x75\x00\x69\x00\x6c\x00\x64\x00\x65\x00\x72\x00\x20\x00\x6e\x30\x20\x00\x6e\x00\x65\x00\x77\x00\x20\x00\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"AlertDialog.Builder の new に失敗しました" @*/ )) ;
			goto END ;
		}
	}

	// AlertDialog.Builder の設定
	{
		jobject object_AlertDialog_Builder ;

		// タイトルの設定
		{
			char UTF16LE_Title[ 256 * 4 ] ;
			int UTF16LE_TitleLength ;
		
			UTF16LE_TitleLength = ConvString( ( char * )Title, -1, _TCHARCODEFORMAT, UTF16LE_Title, sizeof( UTF16LE_Title ), DX_CHARCODEFORMAT_UTF16LE ) ;
			UTF16LE_TitleLength = UTF16LE_TitleLength / 2 - 1 ;
			if( UTF16LE_TitleLength > 0 )
			{
				jstring jstring_Title = env->NewString( ( jchar * )UTF16LE_Title, ( jsize )UTF16LE_TitleLength ) ;
				if( jstring_Title != NULL )
				{
					object_AlertDialog_Builder = env->CallObjectMethod(
						g_AndroidSys.object_AlertDialog_Builder,
						g_JavaAndroidInfo.methodID_AlertDialog_Builder_setTitle,
						jstring_Title
					) ;
					if( object_AlertDialog_Builder != NULL )
					{
						env->DeleteLocalRef( object_AlertDialog_Builder ) ;
						object_AlertDialog_Builder = NULL ;
					}

					env->DeleteLocalRef( jstring_Title ) ;
					jstring_Title = NULL ;
				}
			}
		}

		// ビューの設定
		object_AlertDialog_Builder = env->CallObjectMethod(
			g_AndroidSys.object_AlertDialog_Builder,
			g_JavaAndroidInfo.methodID_AlertDialog_Builder_setView,
			g_AndroidSys.object_EditText
		) ;
		if( object_AlertDialog_Builder != NULL )
		{
			env->DeleteLocalRef( object_AlertDialog_Builder ) ;
			object_AlertDialog_Builder = NULL ;
		}

		// OKボタンの設定
		{
			jstring jstring_OK = env->NewStringUTF( "OK" ) ;
			if( jstring_OK != NULL )
			{
				object_AlertDialog_Builder = env->CallObjectMethod(
					g_AndroidSys.object_AlertDialog_Builder,
					g_JavaAndroidInfo.methodID_AlertDialog_Builder_setPositiveButton,
					jstring_OK,
					NULL
				) ;
				if( object_AlertDialog_Builder != NULL )
				{
					env->DeleteLocalRef( object_AlertDialog_Builder ) ;
					object_AlertDialog_Builder = NULL ;
				}

				env->DeleteLocalRef( jstring_OK ) ;
				jstring_OK = NULL ;
			}
		}
	}

	// ダイアログを表示
	{
		jobject object_Dialog = env->CallObjectMethod(
			g_AndroidSys.object_AlertDialog_Builder,
			g_JavaAndroidInfo.methodID_AlertDialog_Builder_show
		) ;
		if( object_Dialog != NULL )
		{
			g_AndroidSys.object_Dialog = env->NewGlobalRef( object_Dialog ) ;
			env->DeleteLocalRef( object_Dialog ) ;
			object_Dialog = NULL ;
		}
		if( g_AndroidSys.object_Dialog == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x41\x00\x6c\x00\x65\x00\x72\x00\x74\x00\x44\x00\x69\x00\x61\x00\x6c\x00\x6f\x00\x67\x00\x2e\x00\x42\x00\x75\x00\x69\x00\x6c\x00\x64\x00\x65\x00\x72\x00\x20\x00\x6e\x30\x20\x00\x73\x00\x68\x00\x6f\x00\x77\x00\x20\x00\x4c\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"AlertDialog.Builder の show が失敗しました" @*/ )) ;
			goto END ;
		}
	}

	res = 0 ;

END :

	if( res < 0 )
	{
		if( g_AndroidSys.object_EditText != NULL )
		{
			env->DeleteGlobalRef( g_AndroidSys.object_EditText ) ;
			g_AndroidSys.object_EditText = NULL ;
		}

		if( g_AndroidSys.object_Dialog != NULL )
		{
			env->DeleteGlobalRef( g_AndroidSys.object_Dialog ) ;
			g_AndroidSys.object_Dialog = NULL ;
		}

		if( g_AndroidSys.object_AlertDialog_Builder != NULL )
		{
			env->DeleteGlobalRef( g_AndroidSys.object_AlertDialog_Builder ) ;
			g_AndroidSys.object_AlertDialog_Builder = NULL ;
		}
	}

	return res ;
}

// 文字列入力ダイアログを表示する
extern int StartInputStringDialog( const TCHAR *Title )
{
	JNIEnv *env ;
	int res = -1 ;

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		return res ;
	}

	res = StartInputStringDialogStatic( env, Title ) ;

//	jclass class_AppNativeActivityClass = env->GetObjectClass( g_AndroidSys.NativeActivity->clazz ) ;
//
//	jmethodID methodID_StartInputDialog = env->GetMethodID( class_AppNativeActivityClass, "StartInputDialog", "()V" ) ;
//	env->CallVoidMethod( g_AndroidSys.NativeActivity->clazz, methodID_StartInputDialog ) ;
//
//	env->DeleteLocalRef( class_AppNativeActivityClass ) ;

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	return res ;
}

// アプリで使用している NativeActivity を取得する
extern const ANativeActivity *GetNativeActivity( void )
{
	return g_AndroidSys.NativeActivity ;
}

// アプリの ANativeWindow を取得する
extern const ANativeWindow *GetNativeWindow( void )
{
	return ( const ANativeWindow * )g_AndroidSys.NativeWindow ;
}

// アプリの入力イベントをフックするコールバック関数を登録する 
// CallbackFunction : 入力イベント発生時に呼ばれるコールバック関数、NULL を渡すと設定解除( コールバック関数の戻り値( 1:ＤＸライブラリの入力イベント処理も行う　0:ＤＸライブラリの入力イベントリ処理は行わない ) )
// InputEvent : 発生した入力イベントの AInputEvent
// Data : コールバック関数に渡すアドレス、不要な場合は NULL
extern int SetAndroidInputEventFookFunction( int32_t ( *CallbackFunction )( AInputEvent *InputEvent, void *Data ), void *Data )
{
	// 値を保存
	g_AndroidSys.UserInputEventCallbackFunction = CallbackFunction ;
	g_AndroidSys.UserInputEventCallbackFunctionData = Data ;

	return 0 ;
}

// ディスプレイに設定されている解像度を取得する
extern int GetAndroidDisplayResolution( int *SizeX, int *SizeY )
{
#ifdef DX_NON_GRAPHICS
	return -1 ;
#else // DX_NON_GRAPHICS
	// サイズ取得前の場合はエラー
	if( GANDR.Device.Screen.Width  == 0 ||
		GANDR.Device.Screen.Height == 0 )
	{
		return -1 ;
	}

	if( SizeX != NULL ) *SizeX = ( int )GANDR.Device.Screen.Width ;
	if( SizeY != NULL ) *SizeY = ( int )GANDR.Device.Screen.Height ;

	return 0 ;
#endif // DX_NON_GRAPHICS
}

// ディスプレイをスリープ状態にさせないかどうかを設定する( Flag  TRUE = スリープ状態にさせない  FALSE = スリープ状態を許可する( デフォルト ) )
extern int SetKeepScreenOnFlag( int Flag )
{
	ANativeActivity_setWindowFlags( g_AndroidSys.NativeActivity, Flag != 0 ? AWINDOW_FLAG_KEEP_SCREEN_ON : 0, AWINDOW_FLAG_KEEP_SCREEN_ON ) ;

	// 終了
	return 0 ;
}

// ディスプレイカットアウト領域の数を取得する
extern int GetDisplayCutoutRectCount( void )
{
	JNIEnv *env ;
	jobject object_View = NULL ;
	jobject object_Window = NULL ;
	jobject object_WindowInsets = NULL ;
	jobject object_DisplayCutout = NULL ;
	jobject object_List = NULL ;
	int Result = -1 ;

	if( JAVAANDR.fieldint_Build_VERSION_SDK_INT < 28 )
	{
		return -1 ;
	}

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// Windowを取得する
	object_Window = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Activity_getWindow ) ;
	if( object_Window == NULL )
	{
		goto END ;
	}

	// Viewを取得する
	object_View = env->CallObjectMethod( object_Window, JAVAANDR.methodID_Window_getDecorView ) ;
	if( object_View == NULL )
	{
		goto END ;
	}

	// WindowInsets を取得する
	object_WindowInsets = env->CallObjectMethod( object_View, JAVAANDR.methodID_View_getRootWindowInsets ) ;
	if( object_WindowInsets == NULL )
	{
		goto END ;
	}

	// DisplayCutout を取得する
	object_DisplayCutout = env->CallObjectMethod( object_WindowInsets, JAVAANDR.methodID_WindowInsets_getDisplayCutout ) ;
	if( object_DisplayCutout == NULL )
	{
		Result = 0 ;
		goto END ;
	}

	// 矩形リストを取得する
	object_List = env->CallObjectMethod( object_DisplayCutout, JAVAANDR.methodID_DisplayCutout_getBoundingRects ) ;
	if( object_List == NULL )
	{
		goto END ;
	}

	// 矩形の数を取得する
	Result = env->CallIntMethod( object_List, JAVAANDR.methodID_List_size ) ;

END :

	if( object_Window != NULL )
	{
		env->DeleteLocalRef( object_Window ) ;
		object_Window = NULL ;
	}

	if( object_View != NULL )
	{
		env->DeleteLocalRef( object_View ) ;
		object_View = NULL ;
	}

	if( object_WindowInsets != NULL )
	{
		env->DeleteLocalRef( object_WindowInsets ) ;
		object_WindowInsets = NULL ;
	}

	if( object_DisplayCutout != NULL )
	{
		env->DeleteLocalRef( object_DisplayCutout ) ;
		object_DisplayCutout = NULL ;
	}

	if( object_List != NULL )
	{
		env->DeleteLocalRef( object_List ) ;
		object_List = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

// ディスプレイカットアウト領域の矩形を取得する( 引数の No はカットアウト領域の番号 )
extern RECT GetDisplayCutoutRect( int No )
{
	JNIEnv *env ;
	jobject object_View = NULL ;
	jobject object_Window = NULL ;
	jobject object_WindowInsets = NULL ;
	jobject object_DisplayCutout = NULL ;
	jobject object_List = NULL ;
	jobject object_Rect = NULL ;
	int RectNum ;
	RECT Result = { -1, -1, -1, -1 } ;

	if( JAVAANDR.fieldint_Build_VERSION_SDK_INT < 28 )
	{
		return Result ;
	}

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return Result ;
	}

	// JavaVM とソフト実行用スレッドを関連付け 
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return Result ;
	}

	// Windowを取得する
	object_Window = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Activity_getWindow ) ;
	if( object_Window == NULL )
	{
		goto END ;
	}

	// Viewを取得する 
	object_View = env->CallObjectMethod( object_Window, JAVAANDR.methodID_Window_getDecorView ) ;
	if( object_View == NULL )
	{
		goto END ;
	}

	// WindowInsets を取得する 
	object_WindowInsets = env->CallObjectMethod( object_View, JAVAANDR.methodID_View_getRootWindowInsets ) ;
	if( object_WindowInsets == NULL )
	{
		goto END ;
	}

	// DisplayCutout を取得する 
	object_DisplayCutout = env->CallObjectMethod( object_WindowInsets, JAVAANDR.methodID_WindowInsets_getDisplayCutout ) ;
	if( object_DisplayCutout == NULL )
	{
		goto END ;
	}

	// 矩形リストを取得する 
	object_List = env->CallObjectMethod( object_DisplayCutout, JAVAANDR.methodID_DisplayCutout_getBoundingRects ) ;
	if( object_List == NULL )
	{
		goto END ;
	}

	// 矩形の数を取得する 
	RectNum = env->CallIntMethod( object_List, JAVAANDR.methodID_List_size ) ;
	if( No < 0 || No >= RectNum )
	{
		goto END ;
	}

	// 矩形を取得する 
	object_Rect = env->CallObjectMethod( object_List, JAVAANDR.methodID_List_get, No ) ;
	if( object_Rect == NULL )
	{
		goto END ;
	}

	// 矩形の要素を取得する 
	Result.bottom = env->GetIntField( object_Rect, JAVAANDR.fieldID_Rect_bottom ) ;
	Result.left   = env->GetIntField( object_Rect, JAVAANDR.fieldID_Rect_left ) ;
	Result.right  = env->GetIntField( object_Rect, JAVAANDR.fieldID_Rect_right ) ;
	Result.top    = env->GetIntField( object_Rect, JAVAANDR.fieldID_Rect_top ) ;

END :

	if( object_Window != NULL )
	{
		env->DeleteLocalRef( object_Window ) ;
		object_Window = NULL ;
	}

	if( object_View != NULL )
	{
		env->DeleteLocalRef( object_View ) ;
		object_View = NULL ;
	}

	if( object_WindowInsets != NULL )
	{
		env->DeleteLocalRef( object_WindowInsets ) ;
		object_WindowInsets = NULL ;
	}

	if( object_DisplayCutout != NULL )
	{
		env->DeleteLocalRef( object_DisplayCutout ) ;
		object_DisplayCutout = NULL ;
	}

	if( object_List != NULL )
	{
		env->DeleteLocalRef( object_List ) ;
		object_List = NULL ;
	}

	if( object_Rect != NULL )
	{
		env->DeleteLocalRef( object_Rect ) ;
		object_Rect = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

// 加速度センサーのベクトル値を取得する
extern VECTOR GetAccelerometerVector( void )
{
	VECTOR Result ;

	if( g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].Sensor != NULL &&
		g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].SensorEventQueue != NULL )
	{
		Result.x = g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].SensorEvent.acceleration.x ;
		Result.y = g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].SensorEvent.acceleration.y ;
		Result.z = g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].SensorEvent.acceleration.z ;
	}
	else
	{
		Result.x = 0.0f ;
		Result.y = 0.0f ;
		Result.z = 0.0f ;
	}

	return Result ;
}

// センサーのベクトル値を取得する
extern VECTOR GetAndroidSensorVector( int SensorType /* DX_ANDROID_SENSOR_ACCELEROMETER など */ )
{
	VECTOR Result = { -1.0f, -1.0f, -1.0f } ;

	if( SensorType < 0 || SensorType >= DX_ANDROID_SENSOR_NUM )
	{
		return Result ;
	}

	if( g_AndroidSys.SensorInfos[ SensorType ].Sensor != NULL &&
		g_AndroidSys.SensorInfos[ SensorType ].SensorEventQueue != NULL )
	{
		Result.x = g_AndroidSys.SensorInfos[ SensorType ].SensorEvent.acceleration.x ;
		Result.y = g_AndroidSys.SensorInfos[ SensorType ].SensorEvent.acceleration.y ;
		Result.z = g_AndroidSys.SensorInfos[ SensorType ].SensorEvent.acceleration.z ;
	}
	else
	{
		Result.x = 0.0f ;
		Result.y = 0.0f ;
		Result.z = 0.0f ;
	}

	return Result ;
}

// センサーが有効かどうかを取得する( 戻り値　TRUE：有効　FALSE：無効 )
extern int CheckAndroidSensor( int SensorType /* DX_ANDROID_SENSOR_ACCELEROMETER など */ )
{
	if( SensorType < 0 || SensorType >= DX_ANDROID_SENSOR_NUM )
	{
		return FALSE ;
	}

	if( g_AndroidSys.SensorInfos[ SensorType ].Sensor != NULL &&
		g_AndroidSys.SensorInfos[ SensorType ].SensorEventQueue != NULL )
	{
		return TRUE ;
	}
	else
	{
		return FALSE ;
	}
}


// 振動機能があるか取得する( 戻り値　TRUE : 振動機能あり  FALSE : 振動機能なし )
extern int Vibrator_hasVibrator( void )
{
	JNIEnv *env ;
	jobject object_Vibrator = NULL ;
	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// Vibratorの取得
	object_Vibrator = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_VIBRATOR_SERVICE ) ;
	if( object_Vibrator == NULL )
	{
		goto END ;
	}

	// 振動機能が使用できるか調べる
	Result = env->CallBooleanMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_hasVibrator ) == JNI_TRUE ? TRUE : FALSE ;

END :

	if( object_Vibrator != NULL )
	{
		env->DeleteLocalRef( object_Vibrator ) ;
		object_Vibrator = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

// 振動機能に振幅制御( 振動の強さ制御 )機能があるかどうかを取得する( 戻り値　TRUE : 振幅制御ができる  FALSE : 振幅制御はできない )
extern int Vibrator_hasAmplitudeControl( void )
{
	JNIEnv *env ;
	jobject object_Vibrator = NULL ;
	int Result = -1 ;

	// 関数が無い場合は振幅制御ができないということ
	if( JAVAANDR.methodID_Vibrator_hasAmplitudeControl == NULL )
	{
		return FALSE ;
	}

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// Vibratorの取得
	object_Vibrator = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_VIBRATOR_SERVICE ) ;
	if( object_Vibrator == NULL )
	{
		goto END ;
	}

	// 振幅制御ができるか調べる
	Result = env->CallBooleanMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_hasAmplitudeControl ) == JNI_TRUE ? TRUE : FALSE ;

END :

	if( object_Vibrator != NULL )
	{
		env->DeleteLocalRef( object_Vibrator ) ;
		object_Vibrator = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

// 振動を開始する
// Milliseconds : 振動させる時間( 単位：ミリ秒 )
// Amplitude : 振動の強さ( 0( 振動なし ) ～ 255( 最大振幅 ) )、振動の強さの制御に対応していない場合は無視されます
extern int Vibrator_vibrate( int Milliseconds, int Amplitude )
{
	JNIEnv *env ;
	jobject object_Vibrator = NULL ;
	jobject object_VibrationEffect = NULL ;
	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// Vibratorの取得
	object_Vibrator = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_VIBRATOR_SERVICE ) ;
	if( object_Vibrator == NULL )
	{
		goto END ;
	}

	// 振幅制御ができる場合とできない場合で処理を分岐
	if( JAVAANDR.methodID_Vibrator_hasAmplitudeControl != NULL &&
		JAVAANDR.methodID_VibrationEffect_createOneShot != NULL &&
		env->CallBooleanMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_hasAmplitudeControl ) == JNI_TRUE )
	{
		// 振動オブジェクトの作成
		object_VibrationEffect = env->CallStaticObjectMethod( JAVAANDR.class_VibrationEffect, JAVAANDR.methodID_VibrationEffect_createOneShot, ( jlong )Milliseconds, ( jint )Amplitude ) ;
		if( object_VibrationEffect == NULL )
		{
			goto END ;
		}

		// 振動開始
		env->CallVoidMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_vibrate_vibEffect, object_VibrationEffect ) ;
	}
	else
	{
		// 振動開始
		env->CallVoidMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_vibrate_long, ( jlong )Milliseconds ) ;
	}

	Result = 0 ;

END :

	if( object_VibrationEffect != NULL )
	{
		env->DeleteLocalRef( object_VibrationEffect ) ;
		object_VibrationEffect = NULL ;
	}

	if( object_Vibrator != NULL )
	{
		env->DeleteLocalRef( object_Vibrator ) ;
		object_Vibrator = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

// 振動パターン付きの振動を開始する
// Timings : 振動パターンを定義する時間( 単位：ミリ秒 )の配列( Timings[0]:振動を開始するまでの待ち時間、Timings[1]:振動し続ける時間、Timings[2]:振動を停止している時間、Timings[3]:振動し続ける時間、Timings[4]:振動を停止している時間　　以下繰り返し )
// TimingsLength : Timings の配列の長さ
// Repeat : 振動パターンを繰り返す回数、-1 で繰り返し無し
extern int Vibrator_vibrate_with_OnOffTimings( int *Timings, int TimingsLength, int Repeat )
{
	JNIEnv *env ;
	jobject object_Vibrator = NULL ;
	jlongArray longArray_Timings = NULL ;
	jobject object_VibrationEffect = NULL ;
	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// Vibratorの取得
	object_Vibrator = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_VIBRATOR_SERVICE ) ;
	if( object_Vibrator == NULL )
	{
		goto END ;
	}

	// タイミングのlong配列を作成する
	longArray_Timings = Java_Create_longArray_From_intArray( env, Timings, TimingsLength ) ;
	if( longArray_Timings == NULL )
	{
		goto END ;
	}

	// 振幅制御ができる場合とできない場合で処理を分岐
	if( JAVAANDR.methodID_Vibrator_hasAmplitudeControl != NULL &&
		JAVAANDR.methodID_VibrationEffect_createWaveform_withTimings != NULL &&
		env->CallBooleanMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_hasAmplitudeControl ) == JNI_TRUE )
	{
		// 振動オブジェクトの作成
		object_VibrationEffect = env->CallStaticObjectMethod( JAVAANDR.class_VibrationEffect, JAVAANDR.methodID_VibrationEffect_createWaveform_withTimings, longArray_Timings, ( jint )Repeat ) ;
		if( object_VibrationEffect == NULL )
		{
			goto END ;
		}

		// 振動開始
		env->CallVoidMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_vibrate_vibEffect, object_VibrationEffect ) ;
	}
	else
	{
		// 振動開始
		env->CallVoidMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_vibrate_longArray_int, longArray_Timings, ( jint )Repeat ) ;
	}

	Result = 0 ;

END :

	if( longArray_Timings != NULL )
	{
		env->DeleteLocalRef( longArray_Timings ) ;
		longArray_Timings = NULL ;
	}

	if( object_VibrationEffect != NULL )
	{
		env->DeleteLocalRef( object_VibrationEffect ) ;
		object_VibrationEffect = NULL ;
	}

	if( object_Vibrator != NULL )
	{
		env->DeleteLocalRef( object_Vibrator ) ;
		object_Vibrator = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

// 振幅制御付き( 振動の強さ指定付き )振動を開始する( Vibrator_hasAmplitudeControl の戻り値が TRUE の場合のみ使用可能 )
// Timings : 振動の強さを変更するタイミング( 単位：ミリ秒 )の配列
// Amplitudes : 振動の強さ( 0( 振動なし ) ～ 255( 最大振幅 ) )の配列
// ArrayLength : Timings と Amplitudes の配列の長さ( 配列の長さはどちらも同じである必要があります )
// Repeat : Timings と Amplitudes で示される振動パターンを繰り返す回数、-1 で繰り返し無し
extern int Vibrator_vibrate_with_Amplitudes( int *Timings, int *Amplitudes, int ArrayLength, int Repeat )
{
	JNIEnv *env ;
	jobject object_Vibrator = NULL ;
	jlongArray longArray_Timings = NULL ;
	jintArray intArray_Amplitudes = NULL ;
	jobject object_VibrationEffect = NULL ;
	int Result = -1 ;

	// 振幅制御のAPIが使用できない場合は何もせず終了
	if( JAVAANDR.methodID_Vibrator_hasAmplitudeControl == NULL )
	{
		return -1 ;
	}

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// Vibratorの取得
	object_Vibrator = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_VIBRATOR_SERVICE ) ;
	if( object_Vibrator == NULL )
	{
		goto END ;
	}

	// タイミングのlong配列を作成する
	longArray_Timings = Java_Create_longArray_From_intArray( env, Timings, ArrayLength ) ;
	if( longArray_Timings == NULL )
	{
		goto END ;
	}

	// 振幅のint配列を作成する
	intArray_Amplitudes = Java_Create_intArray_From_intArray( env, Amplitudes, ArrayLength ) ;
	if( intArray_Amplitudes == NULL )
	{
		goto END ;
	}

	// 振動制御ができない場合も何もせず終了
	if( env->CallBooleanMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_hasAmplitudeControl ) == JNI_FALSE )
	{
		goto END ;
	}

	// 振動オブジェクトの作成
	object_VibrationEffect = env->CallStaticObjectMethod( JAVAANDR.class_VibrationEffect, JAVAANDR.methodID_VibrationEffect_createWaveform_withAmplitudes, longArray_Timings, intArray_Amplitudes, ( jint )Repeat ) ;
	if( object_VibrationEffect == NULL )
	{
		goto END ;
	}

	// 振動開始
	env->CallVoidMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_vibrate_vibEffect, object_VibrationEffect ) ;

	Result = 0 ;

END :

	if( intArray_Amplitudes != NULL )
	{
		env->DeleteLocalRef( intArray_Amplitudes ) ;
		intArray_Amplitudes = NULL ;
	}

	if( longArray_Timings != NULL )
	{
		env->DeleteLocalRef( longArray_Timings ) ;
		longArray_Timings = NULL ;
	}

	if( object_VibrationEffect != NULL )
	{
		env->DeleteLocalRef( object_VibrationEffect ) ;
		object_VibrationEffect = NULL ;
	}

	if( object_Vibrator != NULL )
	{
		env->DeleteLocalRef( object_Vibrator ) ;
		object_Vibrator = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

// 振動を中断する
extern int Vibrator_cancel( void )
{
	JNIEnv *env ;
	jobject object_Vibrator = NULL ;
	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
	
	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// Vibratorの取得
	object_Vibrator = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_VIBRATOR_SERVICE ) ;
	if( object_Vibrator == NULL )
	{
		goto END ;
	}

	// 振動を停止する
	env->CallVoidMethod( object_Vibrator, JAVAANDR.methodID_Vibrator_cancel ) ;

	Result = 0 ;

END :

	if( object_Vibrator != NULL )
	{
		env->DeleteLocalRef( object_Vibrator ) ;
		object_Vibrator = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

// 曜日や週に関する情報を取得する
// DayOfWeek : 曜日( 1:日曜日 2:月曜日 3:火曜日 4:水曜日 5:木曜日 6:金曜日 7:土曜日 )
// WeekOfMonth : 今日が今月の何週目なのかの値( 1:１週目  2:２週目  3:３週目  4:４週目  5:５週目 )
// DayOfWeekInMonth : 今日の曜日が今月何回目に当たるか、の値( 1:１回目  2:２回目  3:３回目  4:４回目 ... )
extern int GetAndroidWeekInfo( int *DayOfWeek, int *WeekOfMonth, int *DayOfWeekInMonth )
{
	int Result = -1 ;
	jobject jobject_Calendar = NULL ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return Result ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return Result ;
	}

	// Calendar Instance の取得
	jobject_Calendar = env->CallStaticObjectMethod( JAVAANDR.class_Calendar, JAVAANDR.methodID_Calendar_getInstance );
	if( jobject_Calendar == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return Result ;
	}
	
	if( DayOfWeek != NULL )
	{
		*DayOfWeek = env->CallIntMethod( jobject_Calendar, JAVAANDR.methodID_Calendar_get, JAVAANDR.fieldint_Calendar_DAY_OF_WEEK );
	}

	if( WeekOfMonth != NULL )
	{
		*WeekOfMonth = env->CallIntMethod( jobject_Calendar, JAVAANDR.methodID_Calendar_get, JAVAANDR.fieldint_Calendar_WEEK_OF_MONTH );
	}

	if( DayOfWeekInMonth != NULL )
	{
		*DayOfWeekInMonth = env->CallIntMethod( jobject_Calendar, JAVAANDR.methodID_Calendar_get, JAVAANDR.fieldint_Calendar_DAY_OF_WEEK_IN_MONTH );
	}

	if( jobject_Calendar != NULL )
	{
		env->DeleteLocalRef( jobject_Calendar ) ;
		jobject_Calendar = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	Result = 0 ;

	// 戻り値を返す
	return Result ;
}

// res/values/strings.xml の string リソースを取得する
// ValueName：string 情報名
// StringBuffer：ValueName が示す文字列を格納するバッファの先頭アドレス
// StringBufferBytes：StringBuffer のサイズ( 単位：バイト )
// 戻り値　　-1：指定の string 情報は無かった　　-1以外：StringBuffer に必要なバッファのサイズ
extern int GetAndroidResource_Strings_String( const TCHAR *ValueName, TCHAR *StringBuffer, int StringBufferBytes )
{
	int Result = -1 ;
	jobject object_Resources = NULL ;
	jstring string_PackageName = NULL ;
	jstring string_string = NULL ;
	jstring string_name = NULL ;
	jstring string_ResourceString = NULL ;
	TCHAR *ResourceString = NULL ;
	int stringID = -1 ;
//	int StrLength ;
//	int i ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// リソースの取得
	object_Resources = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getResources ) ;
	if( object_Resources == NULL )
	{
		goto END ;
	}

	// パッケージ名の取得
	string_PackageName = ( jstring )env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getPackageName ) ;
	if( string_PackageName == NULL )
	{
		goto END ;
	}

	// IDの取得
	string_string = Java_Create_jstring_From_wchar_t( env, L"string" ) ;
	if( string_string == NULL )
	{
		goto END ;
	}
	string_name = Java_Create_jstring_From_TCHAR( env, ValueName ) ;
	if( string_name == NULL )
	{
		goto END ;
	}
	stringID = env->CallIntMethod( object_Resources, JAVAANDR.methodID_Resources_getIdentifier, string_name, string_string, string_PackageName ) ;
	if( stringID < 0 )
	{
		goto END ;
	}

	// 文字列リソースの取得
	string_ResourceString = ( jstring )env->CallObjectMethod( object_Resources, JAVAANDR.methodID_Resources_getString, stringID ) ;
	if( string_ResourceString == NULL )
	{
		goto END ;
	}

	// 文字列リソースを TCHAR 文字列に変換
	if( Java_Create_TCHAR_string_From_jstring( env, string_ResourceString, &ResourceString ) < 0 )
	{
		goto END ;
	}

	// 文字列の格納に必要なバッファのサイズを取得
	Result = ( CL_strlen( _TCHARCODEFORMAT, ResourceString ) + 1 ) * GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) ;

	// バッファのアドレスが NULL では無い場合はコピー
	if( StringBuffer != NULL )
	{
		CL_strncpy_s( _TCHARCODEFORMAT, StringBuffer, StringBufferBytes, ResourceString, StringBufferBytes / GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) - 1 ) ;
	}

END :

	if( object_Resources != NULL )
	{
		env->DeleteLocalRef( object_Resources ) ;
		object_Resources = NULL ;
	}

	if( string_PackageName != NULL )
	{
		env->DeleteLocalRef( string_PackageName ) ;
		string_PackageName = NULL ;
	}

	if( string_ResourceString != NULL )
	{
		env->DeleteLocalRef( string_ResourceString ) ;
		string_ResourceString = NULL ;
	}

	if( string_string != NULL )
	{
		env->DeleteLocalRef( string_string ) ;
		string_string = NULL ;
	}

	if( string_name != NULL )
	{
		env->DeleteLocalRef( string_name ) ;
		string_name = NULL ;
	}

	if( ResourceString != NULL )
	{
		DXFREE( ResourceString ) ;
		ResourceString = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

extern int GetAndroidResource_Strings_StringWithStrLen( const TCHAR *ValueName, size_t ValueNameLength, TCHAR *StringBuffer, int StringBufferBytes )
{
	int Result ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_ONE_BEGIN( ValueName, ValueNameLength, return -1 )
	Result = GetAndroidResource_Strings_String( UseValueNameBuffer, StringBuffer, StringBufferBytes ) ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( ValueName )
	return Result ;
}

// GetAndroidMetaData_String や GetAndroidMetaData_Boolean のミューテックスのロックと JavaVM とスレッドの関連付け以外を抜き出した関数
static int GetAndroidMetaData_Base( JNIEnv *env, const TCHAR *ValueName, TCHAR *StringBuffer, int StringBufferBytes, int *Boolean )
{
	int Result = -1 ;
	jobject object_PackageManager = NULL ;
	jobject object_ApplicationInfo = NULL ;
	jobject object_metaData = NULL ;
	jstring string_PackageName = NULL ;
	jstring string_string = NULL ;
	jstring data_name = NULL ;
	TCHAR *String = NULL ;
//	int StrLength ;
//	int i ;

	// パッケージマネージャーの取得
	object_PackageManager = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getPackageManager ) ;
	if( object_PackageManager == NULL )
	{
		goto END ;
	}

	// パッケージ名の取得
	string_PackageName = ( jstring )env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getPackageName ) ;
	if( string_PackageName == NULL )
	{
		goto END ;
	}

	// アプリケーション情報の取得
	object_ApplicationInfo = env->CallObjectMethod( object_PackageManager, JAVAANDR.methodID_PackageManager_getApplicationInfo, string_PackageName, JAVAANDR.fieldint_PackageManager_GET_META_DATA ) ;
	if( object_ApplicationInfo == NULL )
	{
		goto END ;
	}

	// メタデータの取得
	object_metaData = env->GetObjectField( object_ApplicationInfo, JAVAANDR.fieldID_PackageItemInfo_metaData ) ;
	if( object_metaData == NULL )
	{
		goto END ;
	}

	// メタデータ名の jstring を作成
	data_name = Java_Create_jstring_From_TCHAR( env, ValueName ) ;
	if( data_name == NULL )
	{
		goto END ;
	}

	// 文字列または Boolean値の取得
	if( Boolean != NULL )
	{
		if( JAVAANDR.fieldint_Build_VERSION_SDK_INT >= 21 )
		{
			Result = env->CallBooleanMethod( object_metaData, JAVAANDR.methodID_BaseBundle_getBoolean, data_name ) == JNI_TRUE ? TRUE : FALSE ;
		}
		else
		{
			Result = env->CallBooleanMethod( object_metaData, JAVAANDR.methodID_Bundle_getBoolean, data_name ) == JNI_TRUE ? TRUE : FALSE ;
		}
	}
	else
	{
		if( JAVAANDR.fieldint_Build_VERSION_SDK_INT >= 21 )
		{
			string_string = ( jstring )env->CallObjectMethod( object_metaData, JAVAANDR.methodID_BaseBundle_getString, data_name ) ;
		}
		else
		{
			string_string = ( jstring )env->CallObjectMethod( object_metaData, JAVAANDR.methodID_Bundle_getString, data_name ) ;
		}
		if( string_string == NULL )
		{
			goto END ;
		}

		// 文字列を TCHAR 文字列に変換
		if( Java_Create_TCHAR_string_From_jstring( env, string_string, &String ) < 0 )
		{
			goto END ;
		}

		// 文字列の格納に必要なバッファのサイズを取得
		Result = ( CL_strlen( _TCHARCODEFORMAT, String ) + 1 ) * GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) ;

		// バッファのアドレスが NULL では無い場合はコピー
		if( StringBuffer != NULL )
		{
			CL_strncpy_s( _TCHARCODEFORMAT, StringBuffer, StringBufferBytes, String, StringBufferBytes / GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) - 1 ) ;
		}
	}

END :

	if( object_PackageManager != NULL )
	{
		env->DeleteLocalRef( object_PackageManager ) ;
		object_PackageManager = NULL ;
	}

	if( string_PackageName != NULL )
	{
		env->DeleteLocalRef( string_PackageName ) ;
		string_PackageName = NULL ;
	}

	if( object_ApplicationInfo != NULL )
	{
		env->DeleteLocalRef( object_ApplicationInfo ) ;
		object_ApplicationInfo = NULL ;
	}

	if( object_metaData != NULL )
	{
		env->DeleteLocalRef( object_metaData ) ;
		object_metaData = NULL ;
	}

	if( string_string != NULL )
	{
		env->DeleteLocalRef( string_string ) ;
		string_string = NULL ;
	}

	if( data_name != NULL )
	{
		env->DeleteLocalRef( data_name ) ;
		data_name = NULL ;
	}

	if( String != NULL )
	{
		DXFREE( String ) ;
		String = NULL ;
	}

	return Result ;
}

// AndroidManifest.xml に書かれた <meta-data android:name="test0" android:value="abcde" /> などのメタデータの文字列を取得する
// ValueName：メタデータ名
// StringBuffer：ValueName が示す文字列を格納するバッファの先頭アドレス
// StringBufferBytes：StringBuffer のサイズ( 単位：バイト )
// 戻り値　　-1：指定のメタデータは無かった　　-1以外：StringBuffer に必要なバッファのサイズ( 単位：バイト )
extern int GetAndroidMetaData_String( const TCHAR *ValueName, TCHAR *StringBuffer, int StringBufferBytes )
{
	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	Result = GetAndroidMetaData_Base( env, ValueName, StringBuffer, StringBufferBytes, NULL ) ;

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result ;
}

extern int GetAndroidMetaData_StringWithStrLen( const TCHAR *ValueName, size_t ValueNameLength, TCHAR *StringBuffer, int StringBufferBytes )
{
	int Result ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_ONE_BEGIN( ValueName, ValueNameLength, return -1 )
	Result = GetAndroidMetaData_String( UseValueNameBuffer, StringBuffer, StringBufferBytes ) ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( ValueName )
	return Result ;
}

// AndroidManifest.xml に書かれた <meta-data android:name="bool0" android:value="true" /> などの Boolean型のメタデータを取得する
// ValueName：メタデータ名
// 戻り値　　-1：指定のメタデータは無かった　　-1以外：StringBuffer に必要なバッファのサイズ( 単位：バイト )
extern int GetAndroidMetaData_Boolean(           const TCHAR *ValueName                        )
{
	int Result = -1 ;
	int Boolean = 0 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	Result = GetAndroidMetaData_Base( env, ValueName, NULL, 0, &Boolean ) ;

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	return Result < 0 ? -1 : Boolean ;
}

extern int GetAndroidMetaData_BooleanWithStrLen( const TCHAR *ValueName, size_t ValueNameLength )
{
	int Result ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_ONE_BEGIN( ValueName, ValueNameLength, return -1 )
	Result = GetAndroidMetaData_Boolean( UseValueNameBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( ValueName )
	return Result ;
}

// センサーから得られる方角を取得する
extern VECTOR GetOrientationVector( void )
{
	VECTOR Result = { -1.0f, -1.0f, -1.0f } ;
	jfloatArray floatArray_AccelerometerValues = NULL ;
	jfloatArray floatArray_MagneticFieldValues = NULL ;
	jfloatArray floatArray_rotationMatrix = NULL ;
	jfloatArray floatArray_inclinationMatrix = NULL ;
	jfloatArray floatArray_remapedMatrix = NULL ;
	jfloatArray floatArray_orientationValues = NULL ;
	jobject object_Temp = NULL ;

	if( g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].Sensor            == NULL ||
		g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].SensorEventQueue  == NULL ||
		g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_MAGNETIC_FIELD ].Sensor           == NULL ||
		g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_MAGNETIC_FIELD ].SensorEventQueue == NULL )
	{
		return Result ;
	}

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return Result ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return Result ;
	}

	// 加速度センサーの値を準備
	{
		VECTOR TempVector ;

		TempVector.x = g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].SensorEvent.acceleration.x ;
		TempVector.y = g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].SensorEvent.acceleration.y ;
		TempVector.z = g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_ACCELEROMETER ].SensorEvent.acceleration.z ;

		floatArray_AccelerometerValues = Java_Create_floatArray_From_VECTOR( env, &TempVector ) ;
		if( floatArray_AccelerometerValues == NULL )
		{
			goto END ;
		}
	}

	// 磁界センサーの値を準備
	{
		VECTOR TempVector ;

		TempVector.x = g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_MAGNETIC_FIELD ].SensorEvent.magnetic.x ;
		TempVector.y = g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_MAGNETIC_FIELD ].SensorEvent.magnetic.y ;
		TempVector.z = g_AndroidSys.SensorInfos[ DX_ANDROID_SENSOR_MAGNETIC_FIELD ].SensorEvent.magnetic.z ;

		floatArray_MagneticFieldValues = Java_Create_floatArray_From_VECTOR( env, &TempVector ) ;
		if( floatArray_MagneticFieldValues == NULL )
		{
			goto END ;
		}
	}

	// 演算用の配列を準備
	{
		floatArray_rotationMatrix    = env->NewFloatArray( 16 ) ;
		floatArray_inclinationMatrix = env->NewFloatArray( 16 ) ;
		floatArray_remapedMatrix     = env->NewFloatArray( 16 ) ;
		floatArray_orientationValues = env->NewFloatArray( 3 ) ;
		if( floatArray_rotationMatrix    == NULL ||
			floatArray_inclinationMatrix == NULL ||
			floatArray_remapedMatrix     == NULL ||
			floatArray_orientationValues == NULL )
		{
			goto END ;
		}
	}

	// 加速度センサーの値と磁気センサーの値から回転行列を取得
	env->CallStaticBooleanMethod(
		JAVAANDR.class_SensorManager,
		JAVAANDR.methodID_SensorManager_getRotationMatrix,
		floatArray_rotationMatrix,
		floatArray_inclinationMatrix,
		floatArray_AccelerometerValues,
		floatArray_MagneticFieldValues
	) ;

	// 回転行列から方角情報を取得する
	env->CallStaticBooleanMethod(
		JAVAANDR.class_SensorManager,
		JAVAANDR.methodID_SensorManager_remapCoordinateSystem,
		floatArray_rotationMatrix,
		JAVAANDR.fieldint_SensorManager_AXIS_X,
		JAVAANDR.fieldint_SensorManager_AXIS_Z,
		floatArray_remapedMatrix
	) ;
	object_Temp = env->CallStaticObjectMethod(
		JAVAANDR.class_SensorManager,
		JAVAANDR.methodID_SensorManager_getOrientation,
		floatArray_remapedMatrix,
		floatArray_orientationValues
	) ;

	// float Array から値を取得
	if( Java_Get_VECTOR_From_floatArray( env, floatArray_orientationValues, &Result ) < 0 )
	{
		goto END ;
	}

END :

	// 取得した参照の後始末
	if( object_Temp != NULL )
	{
		env->DeleteLocalRef( object_Temp ) ;
		object_Temp = NULL ;
	}

	if( floatArray_AccelerometerValues != NULL )
	{
		env->DeleteLocalRef( floatArray_AccelerometerValues ) ;
		floatArray_AccelerometerValues = NULL ;
	}

	if( floatArray_MagneticFieldValues != NULL )
	{
		env->DeleteLocalRef( floatArray_MagneticFieldValues ) ;
		floatArray_MagneticFieldValues = NULL ;
	}

	if( floatArray_rotationMatrix != NULL )
	{
		env->DeleteLocalRef( floatArray_rotationMatrix ) ;
		floatArray_rotationMatrix = NULL ;
	}

	if( floatArray_inclinationMatrix != NULL )
	{
		env->DeleteLocalRef( floatArray_inclinationMatrix ) ;
		floatArray_inclinationMatrix = NULL ;
	}

	if( floatArray_remapedMatrix != NULL )
	{
		env->DeleteLocalRef( floatArray_remapedMatrix ) ;
		floatArray_remapedMatrix = NULL ;
	}

	if( floatArray_orientationValues != NULL )
	{
		env->DeleteLocalRef( floatArray_orientationValues ) ;
		floatArray_orientationValues = NULL ;
	}


	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 戻り値を返す
	return Result ;
}

// 通知を発行する
static int AndroidNotification_WCHAR_T(
	const wchar_t *Title, const wchar_t *SubTitle, int Icon, int ShowWhen,
	int AutoCancel, int NotifyId, int *Vibrate, int VibrateLength,
	unsigned int LightColor, int LightOnTime, int LightOffTime )
{
	jobject object_ApplicationContext = NULL ;
//	jobjectArray objectArray_Intent = NULL ;
	jobject object_Intent = NULL ;
//	jobject object_Intent2 = NULL ;
	jobject object_PendingIntent = NULL ;
	jobject object_Notification = NULL ;
	jobject object_NotificationManager = NULL ;
	jobject object_Notification_Builder = NULL ;
	jobject object_Temp = NULL ;
//	jstring string_Title = NULL ;
//	jstring string_SubTitle = NULL ;
//	jstring string_TickerText = NULL ;
	jlongArray longArray_Vibrate = NULL ;
	jobject object_TitleCharSeq = NULL ;
	jobject object_SubTitleCharSeq = NULL ;
	jobject object_TickerTextCharSeq = NULL ;
	jclass class_NativeActivity = NULL ;
	jint int_Icon = 0 ;
	DWORD LightColorARGB = 0 ;
	int LightColorR, LightColorG, LightColorB ;
	int Result = -1 ;
//	int StrLength ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// アイコンの値を準備
	if( Icon >= 0 )
	{
		int_Icon = Icon ;
	}
	else
	{
		int_Icon = JAVAANDR.fieldint_R_mipmap_sym_def_app_icon ;
	}

	// Title と SubTitle を CharSequence に変換
	object_TitleCharSeq      = Java_Create_CharSequence_From_wchar_t( env, Title ) ;
	object_SubTitleCharSeq   = Java_Create_CharSequence_From_wchar_t( env, SubTitle ) ;
	object_TickerTextCharSeq = Java_Create_CharSequence_From_wchar_t( env, L"" ) ;

	// アプリケーションコンテキストの取得
	object_ApplicationContext = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getApplicationContext ) ;
	if( object_ApplicationContext == NULL )
	{
		goto END ;
	}

	// Native Activity のクラスオブジェクトを取得
	class_NativeActivity = env->GetObjectClass( g_AndroidSys.NativeActivity->clazz ) ;
	if( class_NativeActivity == NULL )
	{
		goto END ;
	}

	// 振動用の long配列の準備
	if( Vibrate != NULL && VibrateLength > 0 )
	{
		longArray_Vibrate = Java_Create_longArray_From_intArray( env, Vibrate, VibrateLength ) ;
		if( longArray_Vibrate == NULL )
		{
			goto END ;
		}
	}

	// ライトパラメータの準備
	{
		NS_GetColor2( LightColor, &LightColorR, &LightColorG, &LightColorB ) ;
		LightColorARGB = 0xff000000 | ( LightColorR << 16 ) | ( LightColorG << 8 ) | LightColorB ;

		if( LightOnTime <= 0 )
		{
			LightOnTime = 0 ;
		}
		if( LightOffTime <= 0 )
		{
			LightOffTime = 0 ;
		}
	}

//	objectArray_Intent = env->NewObjectArray( 2, JAVAANDR.class_Intent, NULL ) ;
//	if( objectArray_Intent == NULL )
//	{
//		goto END ;
//	}

	// Intent の作成
	object_Intent = env->NewObject( JAVAANDR.class_Intent, JAVAANDR.methodID_Intent_newIntent, /*g_AndroidSys.NativeActivity->clazz*/object_ApplicationContext, class_NativeActivity ) ;
	if( object_Intent == NULL )
	{
		goto END ;
	}

//	object_Intent2 = env->NewObject( JAVAANDR.class_Intent, JAVAANDR.methodID_Intent_newIntent, object_ApplicationContext, class_NativeActivity ) ;
//	if( object_Intent2 == NULL )
//	{
//		goto END ;
//	}

//	env->SetObjectArrayElement( objectArray_Intent, 0, object_Intent ) ;
//	env->SetObjectArrayElement( objectArray_Intent, 1, object_Intent2 ) ;

//	object_Intent = env->GetObjectArrayElement( objectArray_Intent, 0 );
//	object_Intent2 = env->GetObjectArrayElement( objectArray_Intent, 1 );

	// Intent のフラグをセット
	object_Temp = env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_setFlags, JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_NEW_TASK | JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_CLEAR_TASK | JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_NO_ANIMATION ) ;
//	object_Temp = env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_setFlags, JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_NEW_TASK /* | JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_CLEAR_TOP */ ) ;
//	object_Temp = env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_setFlags, JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_NO_HISTORY ) ;
	if( object_Temp != NULL )
	{
		env->DeleteLocalRef( object_Temp ) ;
		object_Temp = NULL ;
	}

	// PendingIntent の作成
	object_PendingIntent = env->CallStaticObjectMethod( JAVAANDR.class_PendingIntent, JAVAANDR.methodID_PendingIntent_getActivity, g_AndroidSys.NativeActivity->clazz, 0, object_Intent, JAVAANDR.fieldint_PendingIntent_FLAG_UPDATE_CURRENT ) ;
//	object_PendingIntent = env->CallStaticObjectMethod( JAVAANDR.class_PendingIntent, JAVAANDR.methodID_PendingIntent_getActivities, g_AndroidSys.NativeActivity->clazz, 0, objectArray_Intent, JAVAANDR.fieldint_PendingIntent_FLAG_UPDATE_CURRENT ) ;
	if( object_PendingIntent == NULL )
	{
		goto END ;
	}

	// API レベルによって分岐
	if( JAVAANDR.fieldint_Build_VERSION_SDK_INT < 11 )
	{
		// Notification の作成
		object_Notification = env->NewObject( JAVAANDR.class_Notification, JAVAANDR.methodID_Notification_newNotification ) ;
		if( object_Notification == NULL )
		{
			goto END ;
		}

		// アイコンの設定
		env->SetIntField( object_Notification, JAVAANDR.fieldID_Notification_icon, int_Icon ) ;

		// tickerTextの設定
		env->SetObjectField( object_Notification, JAVAANDR.fieldID_Notification_tickerText, object_TickerTextCharSeq ) ;

		// 振動の設定
		if( Vibrate != NULL && VibrateLength > 0 )
		{
			env->SetObjectField( object_Notification, JAVAANDR.fieldID_Notification_vibrate, longArray_Vibrate ) ;
		}

		// ライトの設定
		if( LightOnTime > 0 && LightOffTime > 0 )
		{
			env->SetIntField( object_Notification, JAVAANDR.fieldID_Notification_ledARGB, LightColorARGB ) ;
			env->SetIntField( object_Notification, JAVAANDR.fieldID_Notification_ledOnMS, LightOnTime ) ;
			env->SetIntField( object_Notification, JAVAANDR.fieldID_Notification_ledOffMS, LightOffTime ) ;
		}

		// タイトルテキストの設定
		env->CallVoidMethod( object_Notification, JAVAANDR.methodID_Notification_setLatestEventInfo, object_ApplicationContext, object_TitleCharSeq, object_SubTitleCharSeq, object_PendingIntent ) ;
	}
	else
	{
		// Notification.Builder を作成
		object_Notification_Builder = env->NewObject( JAVAANDR.class_Notification_Builder, JAVAANDR.methodID_Notification_Builder_newNotification_Builder, g_AndroidSys.NativeActivity->clazz ) ;
		if( object_Notification_Builder == NULL )
		{
			goto END ;
		}

		// AutoCancel をセット
		if( AutoCancel )
		{
			object_Temp = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_setAutoCancel, JNI_TRUE ) ;
			if( object_Temp != NULL )
			{
				env->DeleteLocalRef( object_Temp ) ;
				object_Temp = NULL ;
			}
		}

		// タイトルをセット
		object_Temp = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_setContentTitle, object_TitleCharSeq ) ;
		if( object_Temp != NULL )
		{
			env->DeleteLocalRef( object_Temp ) ;
			object_Temp = NULL ;
		}

		// サブタイトルをセット
		object_Temp = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_setContentText, object_SubTitleCharSeq ) ;
		if( object_Temp != NULL )
		{
			env->DeleteLocalRef( object_Temp ) ;
			object_Temp = NULL ;
		}

		// tickerTextをセット
		object_Temp = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_setTicker, object_TickerTextCharSeq ) ;
		if( object_Temp != NULL )
		{
			env->DeleteLocalRef( object_Temp ) ;
			object_Temp = NULL ;
		}

		// アイコンをセット
		object_Temp = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_setSmallIcon, int_Icon ) ;
		if( object_Temp != NULL )
		{
			env->DeleteLocalRef( object_Temp ) ;
			object_Temp = NULL ;
		}

		// 時刻表示の指定
		if( ShowWhen == FALSE && JAVAANDR.fieldint_Build_VERSION_SDK_INT >= 19 )
		{
			object_Temp = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_setShowWhen, JNI_FALSE ) ;
			if( object_Temp != NULL )
			{
				env->DeleteLocalRef( object_Temp ) ;
				object_Temp = NULL ;
			}
		}

		// 振動の設定
		if( Vibrate != NULL && VibrateLength > 0 )
		{
			object_Temp = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_setVibrate, longArray_Vibrate ) ;
			if( object_Temp != NULL )
			{
				env->DeleteLocalRef( object_Temp ) ;
				object_Temp = NULL ;
			}
		}

		// ライトの設定
		if( LightOnTime > 0 && LightOffTime > 0 )
		{
			object_Temp = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_setLights, LightColorARGB, LightOnTime, LightOffTime ) ;
			if( object_Temp != NULL )
			{
				env->DeleteLocalRef( object_Temp ) ;
				object_Temp = NULL ;
			}
		}

		// PendingIntent セット
		object_Temp = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_setContentIntent, object_PendingIntent ) ;
		if( object_Temp != NULL )
		{
			env->DeleteLocalRef( object_Temp ) ;
			object_Temp = NULL ;
		}

		// SDKのバージョンによって処理を分岐
		if( JAVAANDR.fieldint_Build_VERSION_SDK_INT >= 16 )
		{
			object_Notification = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_build ) ;
		}
		else
		{
			object_Notification = env->CallObjectMethod( object_Notification_Builder, JAVAANDR.methodID_Notification_Builder_getNotification ) ;
		}
	}

	// NotificationManager の取得
	object_NotificationManager = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_NOTIFICATION_SERVICE ) ;
	if( object_NotificationManager == NULL )
	{
		goto END ;
	}

	// Notification を通知
	env->CallVoidMethod( object_NotificationManager, JAVAANDR.methodID_NotificationManager_notify, NotifyId, object_Notification ) ;

	Result = 0 ;

END :

	// 取得した参照の後始末
	if( longArray_Vibrate != NULL )
	{
		env->DeleteLocalRef( longArray_Vibrate ) ;
		longArray_Vibrate = NULL ;
	}

	if( object_Notification_Builder != NULL )
	{
		env->DeleteLocalRef( object_Notification_Builder ) ;
		object_Notification_Builder = NULL ;
	}

	if( object_Temp != NULL )
	{
		env->DeleteLocalRef( object_Temp ) ;
		object_Temp = NULL ;
	}

	if( object_NotificationManager != NULL )
	{
		env->DeleteLocalRef( object_NotificationManager ) ;
		object_NotificationManager = NULL ;
	}

	if( object_TitleCharSeq != NULL )
	{
		env->DeleteLocalRef( object_TitleCharSeq ) ;
		object_TitleCharSeq = NULL ;
	}

	if( object_SubTitleCharSeq != NULL )
	{
		env->DeleteLocalRef( object_SubTitleCharSeq ) ;
		object_SubTitleCharSeq = NULL ;
	}

	if( object_TickerTextCharSeq != NULL )
	{
		env->DeleteLocalRef( object_TickerTextCharSeq ) ;
		object_TickerTextCharSeq = NULL ;
	}

//	if( string_Title != NULL )
//	{
//		env->DeleteLocalRef( string_Title ) ;
//		string_Title = NULL ;
//	}
//
//	if( string_SubTitle != NULL )
//	{
//		env->DeleteLocalRef( string_SubTitle ) ;
//		string_SubTitle = NULL ;
//	}
//
//	if( string_TickerText != NULL )
//	{
//		env->DeleteLocalRef( string_TickerText ) ;
//		string_TickerText = NULL ;
//	}

	if( object_Notification != NULL )
	{
		env->DeleteLocalRef( object_Notification ) ;
		object_Notification = NULL ;
	}

	if( object_PendingIntent != NULL )
	{
		env->DeleteLocalRef( object_PendingIntent ) ;
		object_PendingIntent = NULL ;
	}

	if( object_Intent != NULL )
	{
		env->DeleteLocalRef( object_Intent ) ;
		object_Intent = NULL ;
	}

//	if( object_Intent2 != NULL )
//	{
//		env->DeleteLocalRef( object_Intent2 ) ;
//		object_Intent2 = NULL ;
//	}

//	if( objectArray_Intent != NULL )
//	{
//		env->DeleteLocalRef( objectArray_Intent ) ;
//		objectArray_Intent = NULL ;
//	}

	if( class_NativeActivity != NULL )
	{
		env->DeleteLocalRef( class_NativeActivity ) ;
		class_NativeActivity = NULL ;
	}

	if( object_ApplicationContext != NULL )
	{
		env->DeleteLocalRef( object_ApplicationContext ) ;
		object_ApplicationContext = NULL ;
	}


	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 戻り値を返す
	return Result ;
}

extern int AndroidNotificationWithStrLen(
	const TCHAR *Title, size_t TitleLength, const TCHAR *SubTitle, size_t SubTitleLength, int Icon, int ShowWhen,
	int AutoCancel, int NotifyId, int *Vibrate, int VibrateLength,
	unsigned int LightColor, int LightOnTime, int LightOffTime )
{
	int Result = -1 ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( Title    )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( SubTitle )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( Title,    TitleLength,    goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( SubTitle, SubTitleLength, goto ERR )

	Result = AndroidNotification(
		UseTitleBuffer, UseSubTitleBuffer, Icon, ShowWhen,
		AutoCancel, NotifyId, Vibrate, VibrateLength,
		LightColor, LightOnTime, LightOffTime ) ;

ERR :

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( Title    )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( SubTitle )
	return Result ;
}

extern int AndroidNotification(
	const TCHAR *Title, const TCHAR *SubTitle, int Icon, int ShowWhen,
	int AutoCancel, int NotifyId, int *Vibrate, int VibrateLength,
	unsigned int LightColor, int LightOnTime, int LightOffTime )
{
#ifdef UNICODE
	return AndroidNotification_WCHAR_T( Title, SubTitle, Icon, ShowWhen, AutoCancel, NotifyId, Vibrate, VibrateLength, LightColor, LightOnTime, LightOffTime ) ;
#else
	int Result = -1 ;
	CHAR_TO_WCHAR_T_STRING_BEGIN( Title )
	CHAR_TO_WCHAR_T_STRING_BEGIN( SubTitle )
	CHAR_TO_WCHAR_T_STRING_SETUP( Title,    goto END, _TCHARCODEFORMAT )
	CHAR_TO_WCHAR_T_STRING_SETUP( SubTitle, goto END, _TCHARCODEFORMAT )

	Result = AndroidNotification_WCHAR_T( UseTitleBuffer, UseSubTitleBuffer, Icon, ShowWhen, AutoCancel, NotifyId, Vibrate, VibrateLength, LightColor, LightOnTime, LightOffTime ) ;

END :

	TCHAR_TO_WCHAR_T_STRING_END( SubTitle )
	TCHAR_TO_WCHAR_T_STRING_END( Title )

	return Result ;
#endif
}

// 通知をキャンセルする
// NotifyID : 通知ID
extern int AndroidNotificationCancel( int NotifyId )
{
	jobject object_NotificationManager = NULL ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// NotificationManager の取得
	object_NotificationManager = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_NOTIFICATION_SERVICE ) ;
	if( object_NotificationManager == NULL )
	{
		goto END ;
	}

	// 通知をキャンセル
	env->CallVoidMethod( object_NotificationManager, JAVAANDR.methodID_NotificationManager_cancel, NotifyId ) ;

END :

	if( object_NotificationManager != NULL )
	{
		env->DeleteLocalRef( object_NotificationManager ) ;
		object_NotificationManager = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 終了
	return 0 ;
}

// 全ての通知をキャンセルする
extern int AndroidNotificationCancelAll( void )
{
	jobject object_NotificationManager = NULL ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// NotificationManager の取得
	object_NotificationManager = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_NOTIFICATION_SERVICE ) ;
	if( object_NotificationManager == NULL )
	{
		goto END ;
	}

	// 通知を全てキャンセル
	env->CallVoidMethod( object_NotificationManager, JAVAANDR.methodID_NotificationManager_cancelAll ) ;

END :

	if( object_NotificationManager != NULL )
	{
		env->DeleteLocalRef( object_NotificationManager ) ;
		object_NotificationManager = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 終了
	return 0 ;
}


// 指定の URL をブラウザで開く( BrowserAppPackageName か BrowserAppClassName が NULL の場合は標準ブラウザで開く )
// URL                   : 開くURL
// BrowserAppPackageName : ブラウザのパッケージ名( NULL で標準ブラウザ )
// BrowserAppClassName   : ブラウザのクラス名( NULL で標準ブラウザ )
static int AndroidJumpURL_WCHAR_T( const wchar_t *URL, const wchar_t *BrowserAppPackageName, const wchar_t *BrowserAppClassName )
{
	jstring string_URL = NULL ;
	jstring string_BrowserAppPackageName = NULL ;
	jstring string_BrowserAppClassName = NULL ;
	jobject object_Intent = NULL ;
	jobject object_Uri = NULL ;
	jobject object_Temp = NULL ;
	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// jstring の準備
	string_URL                   = Java_Create_jstring_From_wchar_t( env, URL ) ;
	string_BrowserAppPackageName = Java_Create_jstring_From_wchar_t( env, BrowserAppPackageName ) ;
	string_BrowserAppClassName   = Java_Create_jstring_From_wchar_t( env, BrowserAppClassName ) ;

	// Uri の作成
	object_Uri = env->CallStaticObjectMethod( JAVAANDR.class_Uri, JAVAANDR.methodID_Uri_parse, string_URL ) ;
	if( object_Uri == NULL )
	{
		goto END ;
	}

	// Intent の作成
	object_Intent = env->NewObject( JAVAANDR.class_Intent, JAVAANDR.methodID_Intent_newIntent_Uri, JAVAANDR.fieldstring_Intent_ACTION_VIEW, object_Uri ) ;
	if( object_Intent == NULL )
	{
		goto END ;
	}

	// クラス名の設定
	if( BrowserAppPackageName != NULL && BrowserAppClassName != NULL )
	{
		object_Temp = env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_setClassName, string_BrowserAppPackageName, string_BrowserAppClassName ) ;
		if( object_Temp != NULL )
		{
			env->DeleteLocalRef( object_Temp ) ;
			object_Temp = NULL ;
		}
	}

	// Intent の開始
	env->CallVoidMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Activity_startActivity, object_Intent ) ;

	Result = 0 ;

END :

	// 取得した参照の後始末
	if( object_Intent != NULL )
	{
		env->DeleteLocalRef( object_Intent ) ;
		object_Intent = NULL ;
	}

	if( object_Uri != NULL )
	{
		env->DeleteLocalRef( object_Uri ) ;
		object_Uri = NULL ;
	}

	if( string_URL != NULL )
	{
		env->DeleteLocalRef( string_URL ) ;
		string_URL = NULL ;
	}

	if( string_BrowserAppPackageName != NULL )
	{
		env->DeleteLocalRef( string_BrowserAppPackageName ) ;
		string_BrowserAppPackageName = NULL ;
	}

	if( string_BrowserAppClassName != NULL )
	{
		env->DeleteLocalRef( string_BrowserAppClassName ) ;
		string_BrowserAppClassName = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 戻り値を返す
	return Result ;
}
extern int AndroidJumpURL( const TCHAR *URL, const TCHAR *BrowserAppPackageName, const TCHAR *BrowserAppClassName )
{
#ifdef UNICODE
	return AndroidJumpURL_WCHAR_T( URL, BrowserAppPackageName, BrowserAppClassName ) ;
#else
	int Result = -1 ;
	CHAR_TO_WCHAR_T_STRING_BEGIN( URL )
	CHAR_TO_WCHAR_T_STRING_BEGIN( BrowserAppPackageName )
	CHAR_TO_WCHAR_T_STRING_BEGIN( BrowserAppClassName )
	CHAR_TO_WCHAR_T_STRING_SETUP( URL,                   goto END, _TCHARCODEFORMAT )
	CHAR_TO_WCHAR_T_STRING_SETUP( BrowserAppPackageName, goto END, _TCHARCODEFORMAT )
	CHAR_TO_WCHAR_T_STRING_SETUP( BrowserAppClassName,   goto END, _TCHARCODEFORMAT )

	Result = AndroidJumpURL_WCHAR_T( UseURLBuffer, UseBrowserAppPackageNameBuffer, UseBrowserAppClassNameBuffer ) ;

END :

	TCHAR_TO_WCHAR_T_STRING_END( URL )
	TCHAR_TO_WCHAR_T_STRING_END( BrowserAppPackageName )
	TCHAR_TO_WCHAR_T_STRING_END( BrowserAppClassName )

	return Result ;
#endif
}

extern int AndroidJumpURLWithStrLen( const TCHAR *URL, size_t URLLength, const TCHAR *BrowserAppPackageName, size_t BrowserAppPackageNameLength, const TCHAR *BrowserAppClassName, size_t BrowserAppClassNameLength )
{
	int Result = -1 ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( URL                   )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( BrowserAppPackageName )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( BrowserAppClassName   )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( URL,                   URLLength,                   goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( BrowserAppPackageName, BrowserAppPackageNameLength, goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( BrowserAppClassName,   BrowserAppClassNameLength,   goto ERR )

	Result = AndroidJumpURL( UseURLBuffer, UseBrowserAppPackageNameBuffer, UseBrowserAppClassNameBuffer ) ;

ERR :

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( URL                   )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( BrowserAppPackageName )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( BrowserAppClassName   )
	return Result ;
}













// メールアプリを送信メール編集状態で起動する
// MailAddr    : 宛先( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// MainCCAddr  : CC の宛先( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// MainBCCAddr : BCC の宛先( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// Subject     : タイトル( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
// Text        : 本文( NULL で無効 )、メールアドレスが複数ある場合はカンマ『,』で区切ってください
extern int MailApp_Send_WCHAR_T_PF( const wchar_t *MailAddr, const wchar_t *MailCCAddr, const wchar_t *MailBCCAddr, const wchar_t *Subject, const wchar_t *Text )
{
	jstring string_MailAddr = NULL ;
	jstring string_MailCCAddr = NULL ;
	jstring string_MailBCCAddr = NULL ;
	jstring string_Subject = NULL ;
	jstring string_Text = NULL ;
	jstring string_text_plane = NULL ;
	jobject object_Intent = NULL ;
	jobject object_Uri = NULL ;
	jobjectArray objectArray_MailCCAddr = NULL ;
	jobjectArray objectArray_MailBCCAddr = NULL ;
	wchar_t *FixMailAddrBuffer = NULL ;
	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// メールアドレスの準備
	if( MailAddr != NULL )
	{
		size_t length = CL_strlen( WCHAR_T_CHARCODEFORMAT, ( const char * )MailAddr ) ;
		FixMailAddrBuffer = ( wchar_t * )DXALLOC( ( length + 8 ) * sizeof( wchar_t ) ) ;
		if( FixMailAddrBuffer == NULL )
		{
			goto END ;
		}
		_MEMCPY( FixMailAddrBuffer,     L"mailto:", sizeof( wchar_t ) * 7 ) ;
		_MEMCPY( FixMailAddrBuffer + 7, MailAddr, ( length + 1 ) * sizeof( wchar_t ) ) ;
	}

	// jstring の準備
	if( FixMailAddrBuffer != NULL )
	{
		string_MailAddr = Java_Create_jstring_From_wchar_t( env, FixMailAddrBuffer ) ;
	}
	if( Subject != NULL )
	{
		string_Subject = Java_Create_jstring_From_wchar_t( env, Subject ) ;
	}
	if( Text != NULL )
	{
		string_Text = Java_Create_jstring_From_wchar_t( env, Text ) ;
	}
	string_text_plane  = Java_Create_jstring_From_wchar_t( env, L"text/plain"     ) ;
	if( MailCCAddr != NULL )
	{
		string_MailCCAddr = Java_Create_jstring_From_wchar_t( env, MailCCAddr ) ;
	}
	if( MailBCCAddr != NULL )
	{
		string_MailBCCAddr = Java_Create_jstring_From_wchar_t( env, MailBCCAddr ) ;
	}

	// CC の文字列配列の準備
	if( string_MailCCAddr != NULL )
	{
		objectArray_MailCCAddr = env->NewObjectArray( 1, JAVAANDR.class_String, NULL ) ;
		env->SetObjectArrayElement( objectArray_MailCCAddr, 0, string_MailCCAddr ) ;
	}

	// BCC の文字列配列の準備
	if( string_MailBCCAddr != NULL )
	{
		objectArray_MailBCCAddr = env->NewObjectArray( 1, JAVAANDR.class_String, NULL ) ;
		env->SetObjectArrayElement( objectArray_MailBCCAddr, 0, string_MailBCCAddr ) ;
	}

	// Uri の作成
	if( string_MailAddr != NULL )
	{
		object_Uri = env->CallStaticObjectMethod( JAVAANDR.class_Uri, JAVAANDR.methodID_Uri_parse, string_MailAddr ) ;
		if( object_Uri == NULL )
		{
			goto END ;
		}
	}

	// Intent の作成
	object_Intent = env->NewObject( JAVAANDR.class_Intent, JAVAANDR.methodID_Intent_newIntent_NoParam ) ;
	if( object_Intent == NULL )
	{
		goto END ;
	}

	// アクションを設定
	env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_setAction, JAVAANDR.fieldstring_Intent_ACTION_SENDTO ) ;

	// テキストタイプを設定
	env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_setType, string_text_plane ) ;

	// 宛先を設定
	if( object_Uri != NULL )
	{
		env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_setData, object_Uri ) ;
	}

	// タイトルをセット
	if( string_Subject != NULL )
	{
		env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_putExtra_string, JAVAANDR.fieldstring_Intent_EXTRA_SUBJECT, string_Subject ) ;
	}

	// 本文をセット
	if( string_Text != NULL )
	{
		env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_putExtra_string, JAVAANDR.fieldstring_Intent_EXTRA_TEXT, string_Text ) ;
	}

	// CCをセット
	if( objectArray_MailCCAddr != NULL )
	{
		env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_putExtra_stringArray, JAVAANDR.fieldstring_Intent_EXTRA_CC, objectArray_MailCCAddr ) ;
	}

	// BCCをセット
	if( objectArray_MailBCCAddr != NULL )
	{
		env->CallObjectMethod( object_Intent, JAVAANDR.methodID_Intent_putExtra_stringArray, JAVAANDR.fieldstring_Intent_EXTRA_BCC, objectArray_MailBCCAddr ) ;
	}

	// Intent の開始
	env->CallVoidMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Activity_startActivity, object_Intent ) ;

	Result = 0 ;

END :

	// 取得した参照の後始末
	if( object_Intent != NULL )
	{
		env->DeleteLocalRef( object_Intent ) ;
		object_Intent = NULL ;
	}

	if( object_Uri != NULL )
	{
		env->DeleteLocalRef( object_Uri ) ;
		object_Uri = NULL ;
	}

	if( string_MailAddr != NULL )
	{
		env->DeleteLocalRef( string_MailAddr ) ;
		string_MailAddr = NULL ;
	}

	if( string_MailCCAddr != NULL )
	{
		env->DeleteLocalRef( string_MailCCAddr ) ;
		string_MailCCAddr = NULL ;
	}

	if( string_MailBCCAddr != NULL )
	{
		env->DeleteLocalRef( string_MailBCCAddr ) ;
		string_MailBCCAddr = NULL ;
	}

	if( string_Subject != NULL )
	{
		env->DeleteLocalRef( string_Subject ) ;
		string_Subject = NULL ;
	}

	if( string_Text != NULL )
	{
		env->DeleteLocalRef( string_Text ) ;
		string_Text = NULL ;
	}

	if( string_text_plane != NULL )
	{
		env->DeleteLocalRef( string_text_plane ) ;
		string_text_plane = NULL ;
	}

	if( objectArray_MailCCAddr != NULL )
	{
		env->DeleteLocalRef( objectArray_MailCCAddr ) ;
		objectArray_MailCCAddr = NULL ;
	}

	if( objectArray_MailBCCAddr != NULL )
	{
		env->DeleteLocalRef( objectArray_MailBCCAddr ) ;
		objectArray_MailBCCAddr = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	if( FixMailAddrBuffer != NULL )
	{
		DXFREE( FixMailAddrBuffer ) ;
		FixMailAddrBuffer = NULL ;
	}

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 戻り値を返す
	return Result ;
}




// Wifi の電波強度を取得する( 戻り値：電波の強度 0 ～ 100 )
extern int GetWifiSignalLevel( void )
{
	jobject object_WifiManager = NULL ;
	jobject object_WifiInfo = NULL ;
	jint int_Rssi ;
	jint int_Result = 0 ;
//	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// WifiManagerの取得
	object_WifiManager = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_WIFI_SERVICE ) ;
	if( object_WifiManager == NULL )
	{
		goto END ;
	}

	// WifiInfo の取得
	object_WifiInfo = env->CallObjectMethod( object_WifiManager, JAVAANDR.methodID_WifiManager_getConnectionInfo ) ;
	if( object_WifiInfo == NULL )
	{
		goto END ;
	}

	// 信号強度の取得
	int_Rssi = env->CallIntMethod( object_WifiInfo, JAVAANDR.methodID_WifiInfo_getRssi ) ;

	// 信号強度の解析
	int_Result = env->CallStaticIntMethod( JAVAANDR.class_WifiManager, JAVAANDR.methodID_WifiManager_calculateSignalLevel, int_Rssi, 101 ) ;

END :

	if( object_WifiManager != NULL )
	{
		env->DeleteLocalRef( object_WifiManager ) ;
		object_WifiManager = NULL ;
	}

	if( object_WifiInfo != NULL )
	{
		env->DeleteLocalRef( object_WifiInfo ) ;
		object_WifiInfo = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 戻り値を返す
	return int_Result ;
}

// バッテリー関連

// 電池の残量を % で取得する( 戻り値： 100=フル充電状態  0=充電残量無し )
extern int NS_GetBatteryLifePercent( void )
{
	jobject object_IntentFilter = NULL ;
	jobject object_BatteryStatus = NULL ;
	jint int_BatteryLevel ;
	jint int_BatteryScale ;
	int Result = -1 ;

	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;

	if( g_AndroidSys.NativeActivity == NULL )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	// JavaVM とソフト実行用スレッドを関連付け
	JNIEnv *env ;
	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
	{
		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
		return -1 ;
	}

	object_IntentFilter = env->NewObject( JAVAANDR.class_IntentFilter, JAVAANDR.methodID_IntentFilter_newIntentFilter, JAVAANDR.fieldstring_Intent_ACTION_BATTERY_CHANGED ) ;
	if( object_IntentFilter == NULL )
	{
		goto END ;
	}

	object_BatteryStatus = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_registerReceiver, NULL, object_IntentFilter ) ;
	if( object_BatteryStatus == NULL )
	{
		goto END ;
	}

	int_BatteryLevel = env->CallIntMethod( object_BatteryStatus, JAVAANDR.methodID_Intent_getIntExtra, JAVAANDR.fieldstring_BatteryManager_EXTRA_LEVEL, -1 ) ;
	int_BatteryScale = env->CallIntMethod( object_BatteryStatus, JAVAANDR.methodID_Intent_getIntExtra, JAVAANDR.fieldstring_BatteryManager_EXTRA_SCALE, -1 ) ;

	Result = int_BatteryLevel * 100 / int_BatteryScale ;

END :

	if( object_IntentFilter != NULL )
	{
		env->DeleteLocalRef( object_IntentFilter ) ;
		object_IntentFilter = NULL ;
	}

	if( object_BatteryStatus != NULL )
	{
		env->DeleteLocalRef( object_BatteryStatus ) ;
		object_BatteryStatus = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 戻り値を返す
	return Result ;
}







// クリップボード関係

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int GetClipboardText_PF( TCHAR *DestBuffer, int DestBufferBytes )
{
	// 未実装
	return -1 ;

//	jobject object_ClipboardManager = NULL ;
//	jobject object_ClipData = NULL ;
//	jstring object_ClipString = NULL ;
//	TCHAR *ClipStringP = NULL ;
//	int Result = -1 ;
//
//	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
//
//	if( g_AndroidSys.NativeActivity == NULL )
//	{
//		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
//		return -1 ;
//	}
//
//	// JavaVM とソフト実行用スレッドを関連付け
//	JNIEnv *env ;
//	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
//	{
//		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
//		return -1 ;
//	}
//
//	// ClipboardManagerの取得
//	object_ClipboardManager = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_CLIPBOARD_SERVICE ) ;
//	if( object_ClipboardManager == NULL )
//	{
//		goto END ;
//	}
//
//	// ClipData の取得
//	object_ClipData = env->CallObjectMethod( object_ClipboardManager, JAVAANDR.methodID_ClipboardManager_getPrimaryClip ) ;
//	if( object_ClipData == NULL )
//	{
//		goto END ;
//	}
//
//	// 文字列の取得
//	object_ClipString = ( jstring )env->CallObjectMethod( object_ClipData, JAVAANDR.methodID_ClipData_toString ) ;
//	if( object_ClipString == NULL )
//	{
//		goto END ;
//	}
//
//	// 文字列を TCHAR 文字列に変換
//	if( Java_Create_TCHAR_string_From_jstring( env, object_ClipString, &ClipStringP ) < 0 )
//	{
//		goto END ;
//	}
//
//	// 出力バッファが NULL の場合はテキストデータのサイズを返す
//	if( DestBuffer == NULL )
//	{
//		// 文字列の格納に必要なバッファのサイズを取得
//		Result = ( CL_strlen( _TCHARCODEFORMAT, ClipStringP ) + 1 ) * GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) ;
//	}
//	else
//	{
//		// バッファが指定してある場合はコピー
//		CL_strcpy( _TCHARCODEFORMAT, DestBuffer, ClipStringP ) ;
//
//		// 正常終了
//		Result = 0 ;
//	}
//
//END :
//
//	if( object_ClipboardManager != NULL )
//	{
//		env->DeleteLocalRef( object_ClipboardManager ) ;
//		object_ClipboardManager = NULL ;
//	}
//
//	if( object_ClipData != NULL )
//	{
//		env->DeleteLocalRef( object_ClipData ) ;
//		object_ClipData = NULL ;
//	}
//
//	if( object_ClipString != NULL )
//	{
//		env->DeleteLocalRef( object_ClipString ) ;
//		object_ClipString = NULL ;
//	}
//
//	if( ClipStringP != NULL )
//	{
//		DXFREE( ClipStringP ) ;
//		ClipStringP = NULL ;
//	}
//
//	// JavaVM とこのスレッドの関連付け終了
//	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;
//
//	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
//
//	// 終了
//	return Result ;
}

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int GetClipboardText_WCHAR_T_PF( wchar_t *DestBuffer, int DestBufferBytes )
{
	// 未実装
	return -1 ;

//	jobject object_ClipboardManager = NULL ;
//	jobject object_ClipData = NULL ;
//	jstring object_ClipString = NULL ;
//	wchar_t *ClipStringP = NULL ;
//	int Result = -1 ;
//
//	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
//
//	if( g_AndroidSys.NativeActivity == NULL )
//	{
//		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
//		return -1 ;
//	}
//
//	// JavaVM とソフト実行用スレッドを関連付け
//	JNIEnv *env ;
//	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
//	{
//		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
//		return -1 ;
//	}
//
//	// ClipboardManagerの取得
//	object_ClipboardManager = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_CLIPBOARD_SERVICE ) ;
//	if( object_ClipboardManager == NULL )
//	{
//		goto END ;
//	}
//
//	// ClipData の取得
//	object_ClipData = env->CallObjectMethod( object_ClipboardManager, JAVAANDR.methodID_ClipboardManager_getPrimaryClip ) ;
//	if( object_ClipData == NULL )
//	{
//		goto END ;
//	}
//
//	// 文字列の取得
//	object_ClipString = ( jstring )env->CallObjectMethod( object_ClipData, JAVAANDR.methodID_ClipData_toString ) ;
//	if( object_ClipString == NULL )
//	{
//		goto END ;
//	}
//
//	// 文字列を wchar_t 文字列に変換
//	if( Java_Create_wchar_t_string_From_jstring( env, object_ClipString, &ClipStringP ) < 0 )
//	{
//		goto END ;
//	}
//
//	// 出力バッファが NULL の場合はテキストデータのサイズを返す
//	if( DestBuffer == NULL )
//	{
//		// 文字列の格納に必要なバッファのサイズを取得
//		Result = ( CL_strlen( WCHAR_T_CHARCODEFORMAT, ( char * )ClipStringP ) + 1 ) * GetCharCodeFormatUnitSize( WCHAR_T_CHARCODEFORMAT ) ;
//	}
//	else
//	{
//		// バッファが指定してある場合はコピー
//		CL_strcpy( WCHAR_T_CHARCODEFORMAT, ( char * )DestBuffer, ( char * )ClipStringP ) ;
//
//		// 正常終了
//		Result = 0 ;
//	}
//
//END :
//
//	if( object_ClipboardManager != NULL )
//	{
//		env->DeleteLocalRef( object_ClipboardManager ) ;
//		object_ClipboardManager = NULL ;
//	}
//
//	if( object_ClipData != NULL )
//	{
//		env->DeleteLocalRef( object_ClipData ) ;
//		object_ClipData = NULL ;
//	}
//
//	if( object_ClipString != NULL )
//	{
//		env->DeleteLocalRef( object_ClipString ) ;
//		object_ClipString = NULL ;
//	}
//
//	if( ClipStringP != NULL )
//	{
//		DXFREE( ClipStringP ) ;
//		ClipStringP = NULL ;
//	}
//
//	// JavaVM とこのスレッドの関連付け終了
//	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;
//
//	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
//
//	// 終了
//	return Result ;
}

// クリップボードにテキストデータを格納する
extern int SetClipboardText_WCHAR_T_PF( const wchar_t *Text )
{
	// 未実装
	return -1 ;

//	jobject object_ClipboardManager = NULL ;
//	jobject object_ClipData = NULL ;
//	jobject object_TextCharSequence = NULL ;
//	jobject object_LabelCharSequence = NULL ;
//	int Result = -1 ;
//
//	pthread_mutex_lock( &g_AndroidSys.NativeActivityMutex ) ;
//
//	if( g_AndroidSys.NativeActivity == NULL )
//	{
//		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
//		return -1 ;
//	}
//
//	// JavaVM とソフト実行用スレッドを関連付け
//	JNIEnv *env ;
//	if( g_AndroidSys.NativeActivity->vm->AttachCurrentThreadAsDaemon( &env, NULL ) != JNI_OK )
//	{
//		pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
//		return -1 ;
//	}
//
//	// Text を CharSequence に変換
//	object_TextCharSequence = Java_Create_CharSequence_From_wchar_t( env, Text ) ;
//	if( object_TextCharSequence == NULL )
//	{
//		goto END ;
//	}
//
//	// ラベルを CharSequence に変換
//	object_LabelCharSequence = Java_Create_CharSequence_From_wchar_t( env, L"" ) ;
//	if( object_TextCharSequence == NULL )
//	{
//		goto END ;
//	}
//
//	// ClipData を作成
//	object_ClipData = env->CallStaticObjectMethod( JAVAANDR.class_ClipData, JAVAANDR.methodID_ClipData_newPlainText, object_LabelCharSequence, object_TextCharSequence ) ;
//	if( object_ClipData == NULL )
//	{
//		goto END ;
//	}
//
//	// ClipboardManagerの取得
//	object_ClipboardManager = env->CallObjectMethod( g_AndroidSys.NativeActivity->clazz, JAVAANDR.methodID_Context_getSystemService, JAVAANDR.fieldobject_Context_CLIPBOARD_SERVICE ) ;
//	if( object_ClipboardManager == NULL )
//	{
//		goto END ;
//	}
//
//	// ClipData の設定
//	env->CallVoidMethod( object_ClipboardManager, JAVAANDR.methodID_ClipboardManager_setPrimaryClip, object_ClipData ) ;
//
//	// 正常終了
//	Result = 0 ;
//
//END :
//
//	if( object_ClipboardManager != NULL )
//	{
//		env->DeleteLocalRef( object_ClipboardManager ) ;
//		object_ClipboardManager = NULL ;
//	}
//
//	if( object_ClipData != NULL )
//	{
//		env->DeleteLocalRef( object_ClipData ) ;
//		object_ClipData = NULL ;
//	}
//
//	if( object_TextCharSequence != NULL )
//	{
//		env->DeleteLocalRef( object_TextCharSequence ) ;
//		object_TextCharSequence = NULL ;
//	}
//
//	if( object_LabelCharSequence != NULL )
//	{
//		env->DeleteLocalRef( object_LabelCharSequence ) ;
//		object_LabelCharSequence = NULL ;
//	}
//
//	// JavaVM とこのスレッドの関連付け終了
//	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;
//
//	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;
//
//	// 終了
//	return Result ;
}







// ソフトが非アクティブになった際に呼ばれるコールバック関数を登録する
extern int SetAndroidLostFocusCallbackFunction( void (* Callback )( void *Data ), void *CallbackData )
{
	g_AndroidSys.LostFocusCallbackFunction     = ( volatile void ( * )( void * ) )Callback ;
	g_AndroidSys.LostFocusCallbackFunctionData = ( volatile void * )CallbackData ;

	return 0 ;
}

// ソフトがアクティブになった際に呼ばれるコールバック関数を登録する
extern int SetAndroidGainedFocusCallbackFunction( void (* Callback )( void *Data ), void *CallbackData )
{
	g_AndroidSys.GainedFocusCallbackFunction     = ( volatile void ( * )( void * ) )Callback ;
	g_AndroidSys.GainedFocusCallbackFunctionData = ( volatile void * )CallbackData ;

	return 0 ;
}

// アプリを終了した際に、プロセスを完全に終了するかを設定する( KillProcessFlag  TRUE : アプリを終了したら、プロセスを完全に終了する  FALSE : アプリを終了しても、プロセスは完全には終了されない( デフォルト ) )
extern int SetUseAndroidKillProcessFlag( int KillProcessFlag )
{
	g_AndroidSys.AppKillProcessFlag = KillProcessFlag ;

	return 0 ;
}

// スクリーン座標をＤＸライブラリ画面座標に変換する
extern int ConvScreenPositionToDxScreenPosition( int ScreenX, int ScreenY, int *DxScreenX, int *DxScreenY )
{
#ifdef DX_NON_GRAPHICS
	*DxScreenX = ScreenX ;
	*DxScreenY = ScreenY ;
#else // DX_NON_GRAPHICS

	RECT DestRect ;
	DWORD DestW ;
	DWORD DestH ;

	if( GSYS.Screen.FullScreenFitScalingFlag )
	{
		if( DxScreenX != NULL )
		{
			*DxScreenX = ScreenX * GANDR.Device.Screen.SubBackBufferTextureSizeX / GANDR.Device.Screen.Width ;
		}

		if( DxScreenY != NULL )
		{
			*DxScreenY = ScreenY * GANDR.Device.Screen.SubBackBufferTextureSizeY / GANDR.Device.Screen.Height ;
		}
	}
	else
	{
		DestW = GANDR.Device.Screen.Width ;
		DestH = GANDR.Device.Screen.Width * GANDR.Device.Screen.SubBackBufferTextureSizeY / GANDR.Device.Screen.SubBackBufferTextureSizeX ;
		if( DestH > ( DWORD )GANDR.Device.Screen.Height )
		{
			DestW = GANDR.Device.Screen.Height * GANDR.Device.Screen.SubBackBufferTextureSizeX / GANDR.Device.Screen.SubBackBufferTextureSizeY ;
			DestH = GANDR.Device.Screen.Height ;
		}

		DestRect.left   = ( GANDR.Device.Screen.Width  - DestW ) / 2 ;
		DestRect.top    = ( GANDR.Device.Screen.Height - DestH ) / 2 ;
		DestRect.right  = DestRect.left + DestW ;
		DestRect.bottom = DestRect.top  + DestH ;

		if( DxScreenX != NULL )
		{
			*DxScreenX = ( ScreenX - DestRect.left ) * GANDR.Device.Screen.SubBackBufferTextureSizeX / ( int )DestW ;
		}

		if( DxScreenY != NULL )
		{
			*DxScreenY = ( ScreenY - DestRect.top  ) * GANDR.Device.Screen.SubBackBufferTextureSizeY / ( int )DestH ;
		}
	}

#endif // DX_NON_GRAPHICS

	// 終了
	return 0 ;
}

// アクティブになるまで何もしない
extern void DxActiveWait_Android( void )
{
	while(
		g_AndroidSys.NonActiveRunFlag == FALSE &&
		(
			( g_AndroidSys.ActivityState != DX_ANDR_CMD_START &&
			  g_AndroidSys.ActivityState != DX_ANDR_CMD_RESUME ) ||
			g_AndroidSys.NativeWindow == NULL
	#ifndef DX_NON_GRAPHICS
			|| GANDR.Device.Screen.Context == EGL_NO_CONTEXT
	#endif // DX_NON_GRAPHICS
		)
	)
	{
		if( NS_ProcessMessage() < 0 )
		{
			break ;
		}
		Thread_Sleep( 32 ) ;
	}
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

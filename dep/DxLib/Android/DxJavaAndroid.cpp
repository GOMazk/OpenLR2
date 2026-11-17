// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用Java関係プログラム
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

// インクルード ------------------------------------------------------------------
#include "DxJavaAndroid.h"
#include "DxBaseFuncAndroid.h"
#include "DxSystemAndroid.h"
#include "../DxFont.h"
#include "../DxMemory.h"
#include "../DxBaseFunc.h"
#include "../DxSystem.h"
#include "../DxArchive_.h"
#include "../DxLog.h"



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

// 構造体定義 --------------------------------------------------------------------

DXLIB_JAVA_ANDROID_INFO g_JavaAndroidInfo ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

// プログラム --------------------------------------------------------------------

// UTF16LE の書式文字列と UTF8 のパラメータ文字列をログ出力する
static void JavaAndroid_LogAddUTF8( const char *UTF16LEFormatStr, const char *UTF8Str )
{
	char TempStr[ 1024 ] ;

	ConvString( UTF8Str, -1, DX_CHARCODEFORMAT_UTF8, TempStr, sizeof( TempStr ), DX_CHARCODEFORMAT_UTF16LE ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( UTF16LEFormatStr, TempStr )) ;
}

// Java のクラスのグローバル参照を取得する
extern jclass Java_FindClass_Global( JNIEnv *env, const char *name )
{
	jclass class_local = env->FindClass( name ) ;
	if( class_local == NULL )
	{
		return NULL ;
	}

	jclass class_global = ( jclass )env->NewGlobalRef( class_local ) ;
	env->DeleteLocalRef( class_local ) ;
	class_local = NULL ;

	return class_global ;
}

// Java のスタティックオブジェクトフィールドのグローバル参照を取得する
extern jobject Java_GetStaticObjectField_Global( JNIEnv *env, jclass clazz, jfieldID fieldID )
{
	jobject fieldobject_local = env->GetStaticObjectField( clazz, fieldID ) ;
	if( fieldobject_local == NULL )
	{
		return NULL ;
	}

	jobject fieldobject_global = env->NewGlobalRef( fieldobject_local ) ;
	env->DeleteLocalRef( fieldobject_local ) ;
	fieldobject_local = NULL ;

	return fieldobject_global ;
}

// Java のスタティック文字列フィールドのグローバル参照を取得する
extern jstring Java_GetStaticStringField_Global( JNIEnv *env, jclass clazz, jfieldID fieldID )
{
	jobject fieldobject_local = env->GetStaticObjectField( clazz, fieldID ) ;
	if( fieldobject_local == NULL )
	{
		return NULL ;
	}

	jobject fieldobject_global = env->NewGlobalRef( fieldobject_local ) ;
	env->DeleteLocalRef( fieldobject_local ) ;
	fieldobject_local = NULL ;

	return ( jstring )fieldobject_global ;
}

// TCHAR の文字列から jstring を作成する( Local Ref )
extern jstring Java_Create_jstring_From_TCHAR( JNIEnv *env, const TCHAR *tchar_string )
{
	char UTF16LE_String[ 1024 * 2 ] ;
	int UTF16LE_StringLength ;

	if( tchar_string == NULL )
	{
		return NULL ;
	}

	UTF16LE_StringLength = ConvString( ( char * )tchar_string, -1, _TCHARCODEFORMAT, UTF16LE_String, sizeof( UTF16LE_String ), DX_CHARCODEFORMAT_UTF16LE ) ;
	UTF16LE_StringLength = UTF16LE_StringLength / 2 - 1 ;
	if( UTF16LE_StringLength < 0 )
	{
		return NULL ;
	}

	return env->NewString( ( jchar * )UTF16LE_String, ( jsize )UTF16LE_StringLength ) ;
}

// wchar_t の文字列から jstring を作成する( Local Ref )
extern jstring Java_Create_jstring_From_wchar_t( JNIEnv *env, const wchar_t *wchar_t_string )
{
	char UTF16LE_String[ 1024 * 2 ] ;
	int UTF16LE_StringLength ;

	if( wchar_t_string == NULL )
	{
		return NULL ;
	}

	UTF16LE_StringLength = ConvString( ( char * )wchar_t_string, -1, WCHAR_T_CHARCODEFORMAT, UTF16LE_String, sizeof( UTF16LE_String ), DX_CHARCODEFORMAT_UTF16LE ) ;
	UTF16LE_StringLength = UTF16LE_StringLength / 2 - 1 ;
	if( UTF16LE_StringLength < 0 )
	{
		return NULL ;
	}

	return env->NewString( ( jchar * )UTF16LE_String, ( jsize )UTF16LE_StringLength ) ;
}

// wchar_t の文字列から CharSequence を作成する( Local Ref )
extern jobject Java_Create_CharSequence_From_wchar_t( JNIEnv *env, const wchar_t *wchar_t_string )
{
	char UTF16LE_String[ 1024 * 2 ] ;
	int UTF16LE_StringLength ;
	jstring jstring_Temp = NULL ;
	jobject jobject_Result = NULL ;

	if( wchar_t_string == NULL )
	{
		return NULL ;
	}

	UTF16LE_StringLength = ConvString( ( char * )wchar_t_string, -1, WCHAR_T_CHARCODEFORMAT, UTF16LE_String, sizeof( UTF16LE_String ), DX_CHARCODEFORMAT_UTF16LE ) ;
	UTF16LE_StringLength = UTF16LE_StringLength / 2 - 1 ;
	if( UTF16LE_StringLength > 0 )
	{
		jsize StrLength = 0 ;

		jstring_Temp = env->NewString( ( jchar * )UTF16LE_String, ( jsize )UTF16LE_StringLength ) ;
		if( jstring_Temp == NULL )
		{
			goto END ;
		}

		StrLength = env->GetStringLength( jstring_Temp ) ;
		jobject_Result = env->CallObjectMethod( jstring_Temp, JAVAANDR.methodID_String_subSequence, 0, StrLength ) ;
	}

END :
	if( jstring_Temp != NULL )
	{
		env->DeleteLocalRef( jstring_Temp ) ;
		jstring_Temp = NULL ;
	}

	return jobject_Result ;
}

// jstring から wchar_t の文字列を作成する( DXFREE で解放する )
extern int Java_Create_wchar_t_string_From_jstring( JNIEnv *env, jstring _jstring, wchar_t **wchar_t_stringP )
{
	const char *utf8_charp = NULL ;
	int CharNum = 0 ;
	size_t BufferBytes = 0 ;

	// ポインタを初期化
	*wchar_t_stringP = NULL ;

	// UTF-8 の文字列を取得
	utf8_charp = env->GetStringUTFChars( _jstring, NULL ) ;
	if( utf8_charp == NULL )
	{
		return -1 ;
	}

	// 文字数を取得
	CharNum = CL_strlen( DX_CHARCODEFORMAT_UTF8, utf8_charp ) ;

	// メモリの確保
	BufferBytes = ( CharNum + 1 ) * sizeof( wchar_t ) * 2 ;
	*wchar_t_stringP = ( wchar_t * )DXALLOC( BufferBytes ) ;

	// UTF-8 を wchar_t に変換
	ConvString( utf8_charp, -1, DX_CHARCODEFORMAT_UTF8, ( char * )*wchar_t_stringP, BufferBytes, WCHAR_T_CHARCODEFORMAT ) ;

	// UTF-8 の文字列を解放
	env->ReleaseStringUTFChars( _jstring, utf8_charp ) ;

	// 正常終了
	return 0 ;
}

// jstring から TCHAR の文字列を作成する( DXFREE で解放する )
extern int Java_Create_TCHAR_string_From_jstring( JNIEnv *env, jstring _jstring, TCHAR **tchar_stringP )
{
	const char *utf8_charp = NULL ;
	int CharNum = 0 ;
	size_t BufferBytes = 0 ;

	// ポインタを初期化
	*tchar_stringP = NULL ;

	// UTF-8 の文字列を取得
	utf8_charp = env->GetStringUTFChars( _jstring, NULL ) ;
	if( utf8_charp == NULL )
	{
		return -1 ;
	}

	// 文字数を取得
	CharNum = CL_strlen( DX_CHARCODEFORMAT_UTF8, utf8_charp ) ;

	// メモリの確保
	BufferBytes = ( CharNum + 1 ) * sizeof( TCHAR ) * 6 ;
	*tchar_stringP = ( TCHAR * )DXALLOC( BufferBytes ) ;

	// UTF-8 を TCHAR に変換
	ConvString( utf8_charp, -1, DX_CHARCODEFORMAT_UTF8, ( char * )*tchar_stringP, BufferBytes, _TCHARCODEFORMAT ) ;

	// UTF-8 の文字列を解放
	env->ReleaseStringUTFChars( _jstring, utf8_charp ) ;

	// 正常終了
	return 0 ;
}

// jstring から char の文字列を作成する( DXFREE で解放する )
extern int Java_Create_char_string_From_jstring( JNIEnv *env, jstring _jstring, char **char_stringP )
{
	const char *utf8_charp = NULL ;
	int CharNum = 0 ;
	size_t BufferBytes = 0 ;

	// ポインタを初期化
	*char_stringP = NULL ;

	// UTF-8 の文字列を取得
	utf8_charp = env->GetStringUTFChars( _jstring, NULL ) ;
	if( utf8_charp == NULL )
	{
		return -1 ;
	}

	// 文字数を取得
	CharNum = CL_strlen( DX_CHARCODEFORMAT_UTF8, utf8_charp ) ;

	// メモリの確保
	BufferBytes = ( CharNum + 1 ) * sizeof( char ) * 6 ;
	*char_stringP = ( char * )DXALLOC( BufferBytes ) ;

	// UTF-8 を char に変換
	ConvString( utf8_charp, -1, DX_CHARCODEFORMAT_UTF8, ( char * )*char_stringP, BufferBytes, CHAR_CHARCODEFORMAT ) ;

	// UTF-8 の文字列を解放
	env->ReleaseStringUTFChars( _jstring, utf8_charp ) ;

	// 正常終了
	return 0 ;
}

// VECTOR から長さ 3 の float Array を作成する( Local Ref )
extern jfloatArray Java_Create_floatArray_From_VECTOR( JNIEnv *env, const VECTOR *Vector )
{
	jfloat *jfloat_Element = NULL ;
	jfloatArray jfloatArray_Result = NULL ;

	jfloatArray_Result = env->NewFloatArray( 3 ) ;
	if( jfloatArray_Result == NULL )
	{
		return NULL ;
	}

	jfloat_Element = env->GetFloatArrayElements( jfloatArray_Result, NULL ) ;
	if( jfloat_Element == NULL )
	{
		env->DeleteLocalRef( jfloatArray_Result ) ;
		return NULL ;
	}

	jfloat_Element[ 0 ] = Vector->x ;
	jfloat_Element[ 1 ] = Vector->y ;
	jfloat_Element[ 2 ] = Vector->z ;

	env->ReleaseFloatArrayElements( jfloatArray_Result, jfloat_Element, 0 ) ;

	return jfloatArray_Result ;
}

// 長さ 3 以上の float Array から VECTOR の値を取得する( 0:正常終了  -1:エラー )
extern int Java_Get_VECTOR_From_floatArray( JNIEnv *env, jfloatArray floatArray, VECTOR *Buffer )
{
	jfloat *jfloat_Element = NULL ;

	if( env->GetArrayLength( floatArray ) < 3 )
	{
		return -1 ;
	}

	jfloat_Element = env->GetFloatArrayElements( floatArray, NULL ) ;
	if( jfloat_Element == NULL )
	{
		return -1 ;
	}

	Buffer->x = jfloat_Element[ 0 ] ;
	Buffer->y = jfloat_Element[ 1 ] ;
	Buffer->z = jfloat_Element[ 2 ] ;

	env->ReleaseFloatArrayElements( floatArray, jfloat_Element, 0 ) ;

	return 0 ;
}

// intの配列から jlongArray を作成する
extern jlongArray Java_Create_longArray_From_intArray( JNIEnv *env, const int *intArray, int intArrayLength )
{
	jlong *jlong_Element = NULL ;
	jlongArray jlongArray_Result = NULL ;
	int i ;

	if( intArray == NULL && intArrayLength <= 0 )
	{
		return NULL ;
	}

	jlongArray_Result = env->NewLongArray( intArrayLength ) ;
	if( jlongArray_Result == NULL )
	{
		return NULL ;
	}

	jlong_Element = env->GetLongArrayElements( jlongArray_Result, NULL ) ;
	if( jlong_Element == NULL )
	{
		env->DeleteLocalRef( jlongArray_Result ) ;
		return NULL ;
	}

	for( i = 0 ; i < intArrayLength ; i ++ )
	{
		jlong_Element[ i ] = ( jlong )intArray[ i ] ;
	}

	env->ReleaseLongArrayElements( jlongArray_Result, jlong_Element, 0 ) ;

	return jlongArray_Result ;
}

// intの配列から jintArray を作成する
extern jintArray Java_Create_intArray_From_intArray( JNIEnv *env, const int *intArray, int intArrayLength )
{
	jint *jint_Element = NULL ;
	jintArray jintArray_Result = NULL ;
	int i ;

	if( intArray == NULL && intArrayLength <= 0 )
	{
		return NULL ;
	}

	jintArray_Result = env->NewIntArray( intArrayLength ) ;
	if( jintArray_Result == NULL )
	{
		return NULL ;
	}

	jint_Element = env->GetIntArrayElements( jintArray_Result, NULL ) ;
	if( jint_Element == NULL )
	{
		env->DeleteLocalRef( jintArray_Result ) ;
		return NULL ;
	}

	for( i = 0 ; i < intArrayLength ; i ++ )
	{
		jint_Element[ i ] = intArray[ i ] ;
	}

	env->ReleaseIntArrayElements( jintArray_Result, jint_Element, 0 ) ;

	return jintArray_Result ;
}

// floatの配列から jfloatArray を作成する
extern jfloatArray Java_Create_floatArray_From_floatArray( JNIEnv *env, const int *floatArray, int floatArrayLength )
{
	jfloat *jfloat_Element = NULL ;
	jfloatArray jfloatArray_Result = NULL ;
	int i ;

	if( floatArray == NULL && floatArrayLength <= 0 )
	{
		return NULL ;
	}

	jfloatArray_Result = env->NewFloatArray( floatArrayLength ) ;
	if( jfloatArray_Result == NULL )
	{
		return NULL ;
	}

	jfloat_Element = env->GetFloatArrayElements( jfloatArray_Result, NULL ) ;
	if( jfloat_Element == NULL )
	{
		env->DeleteLocalRef( jfloatArray_Result ) ;
		return NULL ;
	}

	for( i = 0 ; i < floatArrayLength ; i ++ )
	{
		jfloat_Element[ i ] = floatArray[ i ] ;
	}

	env->ReleaseFloatArrayElements( jfloatArray_Result, jfloat_Element, 0 ) ;

	return jfloatArray_Result ;
}

// doubleの配列から jdoubleArray を作成する
extern jdoubleArray Java_Create_doubleArray_From_doubleArray( JNIEnv *env, const int *doubleArray, int doubleArrayLength )
{
	jdouble *jdouble_Element = NULL ;
	jdoubleArray jdoubleArray_Result = NULL ;
	int i ;

	if( doubleArray == NULL && doubleArrayLength <= 0 )
	{
		return NULL ;
	}

	jdoubleArray_Result = env->NewDoubleArray( doubleArrayLength ) ;
	if( jdoubleArray_Result == NULL )
	{
		return NULL ;
	}

	jdouble_Element = env->GetDoubleArrayElements( jdoubleArray_Result, NULL ) ;
	if( jdouble_Element == NULL )
	{
		env->DeleteLocalRef( jdoubleArray_Result ) ;
		return NULL ;
	}

	for( i = 0 ; i < doubleArrayLength ; i ++ )
	{
		jdouble_Element[ i ] = doubleArray[ i ] ;
	}

	env->ReleaseDoubleArrayElements( jdoubleArray_Result, jdouble_Element, 0 ) ;

	return jdoubleArray_Result ;
}

// Java のクラスやメソッドの参照を取得する
#define FINDCLASS( obj, name )			\
	{\
		obj = Java_FindClass_Global( env, name ) ;\
		if( obj == NULL )\
		{\
			JavaAndroid_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x43\x00\x6c\x00\x61\x00\x73\x00\x73\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Class %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaAndroidInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETMETHOD( obj, class_, name, sig )		\
	{\
		obj = env->GetMethodID( class_, name, sig ) ;\
		if( obj == NULL )\
		{\
			JavaAndroid_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x4d\x00\x65\x00\x74\x00\x68\x00\x6f\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Method %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaAndroidInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETSTATICMETHOD( obj, class_, name, sig )		\
	{\
		obj = env->GetStaticMethodID( class_, name, sig ) ;\
		if( obj == NULL )\
		{\
			JavaAndroid_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x69\x00\x63\x00\x20\x00\x4d\x00\x65\x00\x74\x00\x68\x00\x6f\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Static Method %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaAndroidInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETFIELD( obj, class_, name, sig )		\
	{\
		obj = env->GetFieldID( class_, name, sig ) ;\
		if( obj == NULL )\
		{\
			JavaAndroid_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x46\x00\x69\x00\x65\x00\x6c\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Field %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaAndroidInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETSTATICFIELD( obj, class_, name, sig )		\
	{\
		obj = env->GetStaticFieldID( class_, name, sig ) ;\
		if( obj == NULL )\
		{\
			JavaAndroid_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x69\x00\x63\x00\x20\x00\x46\x00\x69\x00\x65\x00\x6c\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Static Field %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaAndroidInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETSTATICOBJECTFIELD( obj, class_, field_, name )		\
	{\
		obj = Java_GetStaticObjectField_Global( env, class_, field_ ) ;\
		if( obj == NULL )\
		{\
			JavaAndroid_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x69\x00\x63\x00\x20\x00\x4f\x00\x62\x00\x6a\x00\x65\x00\x63\x00\x74\x00\x20\x00\x46\x00\x69\x00\x65\x00\x6c\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Static Object Field %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaAndroidInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETSTATICSTRINGFIELD( obj, class_, field_, name )		\
	{\
		obj = Java_GetStaticStringField_Global( env, class_, field_ ) ;\
		if( obj == NULL )\
		{\
			JavaAndroid_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x69\x00\x63\x00\x20\x00\x53\x00\x74\x00\x72\x00\x69\x00\x6e\x00\x67\x00\x20\x00\x46\x00\x69\x00\x65\x00\x6c\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Static String Field %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaAndroidInfo( env ) ;\
			return -1 ;\
		}\
	}
	
extern int SetupJavaAndroidInfo( JNIEnv *env )
{
	jint API_LV ;

	if( JAVAANDR.InitializeFlag == TRUE )
	{
		return -1 ;
	}

	if( env == NULL )
	{
		return -1 ;
	}

	// APIレベルだけ先に取得する
	FINDCLASS( JAVAANDR.class_Build_VERSION, "android/os/Build$VERSION" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Build_VERSION_SDK_INT, JAVAANDR.class_Build_VERSION, "SDK_INT", "I" ) ;
	JAVAANDR.fieldint_Build_VERSION_SDK_INT = env->GetStaticIntField( JAVAANDR.class_Build_VERSION, JAVAANDR.fieldID_Build_VERSION_SDK_INT ) ;
	API_LV = JAVAANDR.fieldint_Build_VERSION_SDK_INT ;

	if( API_LV >= 11 )
	{
		FINDCLASS( JAVAANDR.class_Notification_Builder, "android/app/Notification$Builder" ) ;

		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_newNotification_Builder, JAVAANDR.class_Notification_Builder, "<init>",              "(Landroid/content/Context;)V" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setWhen,                 JAVAANDR.class_Notification_Builder, "setWhen",             "(J)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 19 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setShowWhen,             JAVAANDR.class_Notification_Builder, "setShowWhen",         "(Z)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setUsesChronometer,      JAVAANDR.class_Notification_Builder, "setUsesChronometer",  "(Z)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setSmallIcon,            JAVAANDR.class_Notification_Builder, "setSmallIcon",        "(II)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setContentTitle,         JAVAANDR.class_Notification_Builder, "setContentTitle",     "(Ljava/lang/CharSequence;)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setContentText,          JAVAANDR.class_Notification_Builder, "setContentText",      "(Ljava/lang/CharSequence;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setSubText,              JAVAANDR.class_Notification_Builder, "setSubText",          "(Ljava/lang/CharSequence;)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setNumber,               JAVAANDR.class_Notification_Builder, "setNumber",           "(I)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setContentInfo,          JAVAANDR.class_Notification_Builder, "setContentInfo",      "(Ljava/lang/CharSequence;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 14 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setProgress,             JAVAANDR.class_Notification_Builder, "setProgress",         "(IIZ)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setContent,              JAVAANDR.class_Notification_Builder, "setContent",          "(Landroid/widget/RemoteViews;)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setContentIntent,        JAVAANDR.class_Notification_Builder, "setContentIntent",    "(Landroid/app/PendingIntent;)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setDeleteIntent,         JAVAANDR.class_Notification_Builder, "setDeleteIntent",     "(Landroid/app/PendingIntent;)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setFullScreenIntent,     JAVAANDR.class_Notification_Builder, "setFullScreenIntent", "(Landroid/app/PendingIntent;Z)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setTicker,               JAVAANDR.class_Notification_Builder, "setTicker",           "(Ljava/lang/CharSequence;Landroid/widget/RemoteViews;)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setLargeIcon,            JAVAANDR.class_Notification_Builder, "setLargeIcon",        "(Landroid/graphics/Bitmap;)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setSound,                JAVAANDR.class_Notification_Builder, "setSound",            "(Landroid/net/Uri;I)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setVibrate,              JAVAANDR.class_Notification_Builder, "setVibrate",          "([J)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setLights,               JAVAANDR.class_Notification_Builder, "setLights",           "(III)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setOngoing,              JAVAANDR.class_Notification_Builder, "setOngoing",          "(Z)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setOnlyAlertOnce,        JAVAANDR.class_Notification_Builder, "setOnlyAlertOnce",    "(Z)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setAutoCancel,           JAVAANDR.class_Notification_Builder, "setAutoCancel",       "(Z)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setLocalOnly,            JAVAANDR.class_Notification_Builder, "setLocalOnly",        "(Z)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_setDefaults,             JAVAANDR.class_Notification_Builder, "setDefaults",         "(I)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setPriority,             JAVAANDR.class_Notification_Builder, "setPriority",         "(I)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setCategory,             JAVAANDR.class_Notification_Builder, "setCategory",         "(Ljava/lang/String;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_addPerson,               JAVAANDR.class_Notification_Builder, "addPerson",           "(Ljava/lang/String;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setGroup,                JAVAANDR.class_Notification_Builder, "setGroup",            "(Ljava/lang/String;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setGroupSummary,         JAVAANDR.class_Notification_Builder, "setGroupSummary",     "(Z)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setSortKey,              JAVAANDR.class_Notification_Builder, "setSortKey",          "(Ljava/lang/String;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_addExtras,               JAVAANDR.class_Notification_Builder, "addExtras",           "(Landroid/os/Bundle;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 19 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setExtras,               JAVAANDR.class_Notification_Builder, "setExtras",           "(Landroid/os/Bundle;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_getExtras,               JAVAANDR.class_Notification_Builder, "getExtras",           "()Landroid/os/Bundle;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_addAction,               JAVAANDR.class_Notification_Builder, "addAction",           "(ILjava/lang/CharSequence;Landroid/app/PendingIntent;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 22 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setStyle,                JAVAANDR.class_Notification_Builder, "setStyle",            "(Landroid/app/Notification$Style;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setVisibility,           JAVAANDR.class_Notification_Builder, "setVisibility",       "(I)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setPublicVersion,        JAVAANDR.class_Notification_Builder, "setPublicVersion",    "(Landroid/app/Notification;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 22 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_extend,                  JAVAANDR.class_Notification_Builder, "extend",              "(Landroid/app/Notification$Extender;)Landroid/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_setColor,                JAVAANDR.class_Notification_Builder, "setColor",            "(I)Landroid/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVAANDR.methodID_Notification_Builder_getNotification,         JAVAANDR.class_Notification_Builder, "getNotification",     "()Landroid/app/Notification;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVAANDR.methodID_Notification_Builder_build,                   JAVAANDR.class_Notification_Builder, "build",               "()Landroid/app/Notification;" ) ;
	}

	FINDCLASS( JAVAANDR.class_Bitmap,               "android/graphics/Bitmap"            ) ;
	FINDCLASS( JAVAANDR.class_BitmapConfig,         "android/graphics/Bitmap$Config"     ) ;
	FINDCLASS( JAVAANDR.class_Canvas,               "android/graphics/Canvas"            ) ;
	FINDCLASS( JAVAANDR.class_Typeface,             "android/graphics/Typeface"          ) ;
	FINDCLASS( JAVAANDR.class_Rect,                 "android/graphics/Rect"              ) ;
	FINDCLASS( JAVAANDR.class_Paint,                "android/graphics/Paint"             ) ;
	FINDCLASS( JAVAANDR.class_PaintFontMetrics,     "android/graphics/Paint$FontMetrics" ) ;
	FINDCLASS( JAVAANDR.class_Color,                "android/graphics/Color"             ) ;

	if( API_LV >= 21 )
	{
		FINDCLASS( JAVAANDR.class_BaseBundle,       "android/os/BaseBundle"              ) ;
	}
	FINDCLASS( JAVAANDR.class_Bundle,               "android/os/Bundle"                  ) ;
	FINDCLASS( JAVAANDR.class_Debug_MemoryInfo,     "android/os/Debug$MemoryInfo"        ) ;
	FINDCLASS( JAVAANDR.class_BatteryManager,       "android/os/BatteryManager"          ) ;
	FINDCLASS( JAVAANDR.class_Environment,          "android/os/Environment"             ) ;
	FINDCLASS( JAVAANDR.class_Vibrator,             "android/os/Vibrator"                ) ;
	if( API_LV >= 26 )
	{
		FINDCLASS( JAVAANDR.class_VibrationEffect,  "android/os/VibrationEffect"         ) ;
	}
	FINDCLASS( JAVAANDR.class_Process,              "android/os/Process"                 ) ;
	FINDCLASS( JAVAANDR.class_View,                 "android/view/View"                  ) ;
	if( API_LV >= 28 )
	{
		FINDCLASS( JAVAANDR.class_DisplayCutout,    "android/view/DisplayCutout"         ) ;
	}
	FINDCLASS( JAVAANDR.class_Window,               "android/view/Window"                ) ;
	FINDCLASS( JAVAANDR.class_TextView,             "android/widget/TextView"            ) ;
	FINDCLASS( JAVAANDR.class_TextView_BufferType,  "android/widget/TextView$BufferType" ) ;
	FINDCLASS( JAVAANDR.class_EditText,             "android/widget/EditText"            ) ;
	FINDCLASS( JAVAANDR.class_InputStream,          "java/io/InputStream"                ) ;
	FINDCLASS( JAVAANDR.class_File,                 "java/io/File"                       ) ;
	FINDCLASS( JAVAANDR.class_CharSequence,         "java/lang/CharSequence"             ) ;
	FINDCLASS( JAVAANDR.class_Integer,              "java/lang/Integer"                  ) ;
	FINDCLASS( JAVAANDR.class_String,               "java/lang/String"                   ) ;
	FINDCLASS( JAVAANDR.class_Runtime,              "java/lang/Runtime"                  ) ;
	FINDCLASS( JAVAANDR.class_List,                 "java/util/List"                     ) ;
	FINDCLASS( JAVAANDR.class_Locale,               "java/util/Locale"                   ) ;
	FINDCLASS( JAVAANDR.class_Calendar,             "java/util/Calendar"                 ) ;
	FINDCLASS( JAVAANDR.class_URLConnection,        "java/net/URLConnection"             ) ;
	FINDCLASS( JAVAANDR.class_HttpURLConnection,    "java/net/HttpURLConnection"         ) ;
	FINDCLASS( JAVAANDR.class_URL,                  "java/net/URL"                       ) ;
	FINDCLASS( JAVAANDR.class_ActivityManager,      "android/app/ActivityManager" ) ;
	FINDCLASS( JAVAANDR.class_ActivityManager_MemoryInfo,  "android/app/ActivityManager$MemoryInfo" ) ;
	FINDCLASS( JAVAANDR.class_Dialog,               "android/app/Dialog"                 ) ;
	FINDCLASS( JAVAANDR.class_AlertDialog,          "android/app/AlertDialog"            ) ;
	FINDCLASS( JAVAANDR.class_AlertDialog_Builder,  "android/app/AlertDialog$Builder"    ) ;
	FINDCLASS( JAVAANDR.class_Notification,         "android/app/Notification"           ) ;
	FINDCLASS( JAVAANDR.class_NotificationManager,  "android/app/NotificationManager"    ) ;
	FINDCLASS( JAVAANDR.class_PendingIntent,        "android/app/PendingIntent"          ) ;
	FINDCLASS( JAVAANDR.class_Intent,               "android/content/Intent"             ) ;
	FINDCLASS( JAVAANDR.class_IntentFilter,         "android/content/IntentFilter"       ) ;
	FINDCLASS( JAVAANDR.class_BroadcastReceiver,    "android/content/BroadcastReceiver"  ) ;
	FINDCLASS( JAVAANDR.class_ClipboardManager,     "android/content/ClipboardManager"   ) ;
	FINDCLASS( JAVAANDR.class_ClipData,             "android/content/ClipData"           ) ;
	FINDCLASS( JAVAANDR.class_Context,              "android/content/Context"            ) ;
	FINDCLASS( JAVAANDR.class_Resources,            "android/content/res/Resources"      ) ;
	FINDCLASS( JAVAANDR.class_ApplicationInfo,      "android/content/pm/ApplicationInfo" ) ;
	FINDCLASS( JAVAANDR.class_PackageInfo,          "android/content/pm/PackageInfo"     ) ;
	FINDCLASS( JAVAANDR.class_PackageItemInfo,      "android/content/pm/PackageItemInfo" ) ;
	FINDCLASS( JAVAANDR.class_PackageManager,       "android/content/pm/PackageManager"  ) ;
	FINDCLASS( JAVAANDR.class_SensorManager,        "android/hardware/SensorManager"     ) ;
	FINDCLASS( JAVAANDR.class_R_mipmap,             "android/R$mipmap"                   ) ;
	FINDCLASS( JAVAANDR.class_Uri,                  "android/net/Uri"                    ) ;
	FINDCLASS( JAVAANDR.class_WifiInfo,             "android/net/wifi/WifiInfo"          ) ;
	FINDCLASS( JAVAANDR.class_WifiManager,          "android/net/wifi/WifiManager"       ) ;
	FINDCLASS( JAVAANDR.class_Activity,             "android/app/Activity"               ) ;
	FINDCLASS( JAVAANDR.class_AudioManager,         "android/media/AudioManager"         ) ;
	if( API_LV >= 20 )
	{
		FINDCLASS( JAVAANDR.class_WindowInsets,     "android/view/WindowInsets"          ) ;
	}


	GETSTATICMETHOD( JAVAANDR.methodID_Bitmap_createBitmap                        , JAVAANDR.class_Bitmap,               "createBitmap",          "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Bitmap_setPixels                           , JAVAANDR.class_Bitmap,               "setPixels",             "([IIIIIII)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Bitmap_getPixels                           , JAVAANDR.class_Bitmap,               "getPixels",             "([IIIIIII)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Canvas_newCanvas                           , JAVAANDR.class_Canvas,               "<init>",                "(Landroid/graphics/Bitmap;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Canvas_drawText                            , JAVAANDR.class_Canvas,               "drawText",              "([CIIFFLandroid/graphics/Paint;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Canvas_drawARGB                            , JAVAANDR.class_Canvas,               "drawARGB",              "(IIII)V" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_Typeface_create                            , JAVAANDR.class_Typeface,             "create",                "(Ljava/lang/String;I)Landroid/graphics/Typeface;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Paint_newPaint                             , JAVAANDR.class_Paint,                "<init>",                "()V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Paint_getFontMetrics                       , JAVAANDR.class_Paint,                "getFontMetrics",        "()Landroid/graphics/Paint$FontMetrics;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Paint_setTextSize                          , JAVAANDR.class_Paint,                "setTextSize",           "(F)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Paint_setTypeface                          , JAVAANDR.class_Paint,                "setTypeface",           "(Landroid/graphics/Typeface;)Landroid/graphics/Typeface;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Paint_setAntiAlias                         , JAVAANDR.class_Paint,                "setAntiAlias",          "(Z)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Paint_setARGB                              , JAVAANDR.class_Paint,                "setARGB",               "(IIII)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Paint_measureText                          , JAVAANDR.class_Paint,                "measureText",           "([CII)F" ) ;

	GETMETHOD(       JAVAANDR.methodID_EditText_newEditText                       , JAVAANDR.class_EditText,             "<init>",                "(Landroid/content/Context;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_EditText_setText                           , JAVAANDR.class_EditText,             "setText",               "(Ljava/lang/CharSequence;Landroid/widget/TextView$BufferType;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_TextView_getText                           , JAVAANDR.class_TextView,             "getText",               "()Ljava/lang/CharSequence;" ) ;
	if( API_LV >= 23 )
	{
		GETMETHOD(   JAVAANDR.methodID_View_getRootWindowInsets                   , JAVAANDR.class_View,                 "getRootWindowInsets",   "()Landroid/view/WindowInsets;" ) ;
	}
	if( API_LV >= 28 )
	{
		GETMETHOD(   JAVAANDR.methodID_DisplayCutout_getBoundingRects             , JAVAANDR.class_DisplayCutout,        "getBoundingRects",      "()Ljava/util/List;" ) ;
	}
	GETMETHOD(       JAVAANDR.methodID_Window_getCurrentFocus                     , JAVAANDR.class_Window,               "getCurrentFocus",       "()Landroid/view/View;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Window_getDecorView                        , JAVAANDR.class_Window,               "getDecorView",          "()Landroid/view/View;" ) ;
	if( API_LV >= 28 )
	{
		GETMETHOD(   JAVAANDR.methodID_WindowInsets_getDisplayCutout              , JAVAANDR.class_WindowInsets,         "getDisplayCutout",      "()Landroid/view/DisplayCutout;" ) ;
	}
	if( API_LV >= 21 )
	{
		GETSTATICMETHOD( JAVAANDR.methodID_Environment_isExternalStorageRemovable , JAVAANDR.class_Environment,          "isExternalStorageRemovable",   "(Ljava/io/File;)Z" ) ;
	}
	GETMETHOD(       JAVAANDR.methodID_Vibrator_hasVibrator                       , JAVAANDR.class_Vibrator,             "hasVibrator",           "()Z" ) ;
	if( API_LV >= 26 )
	{
		GETMETHOD(   JAVAANDR.methodID_Vibrator_hasAmplitudeControl               , JAVAANDR.class_Vibrator,             "hasAmplitudeControl",   "()Z" ) ;
		GETMETHOD(   JAVAANDR.methodID_Vibrator_vibrate_vibEffect                 , JAVAANDR.class_Vibrator,             "vibrate",               "(Landroid/os/VibrationEffect;)V" ) ;
	}
	GETMETHOD(       JAVAANDR.methodID_Vibrator_vibrate_long                      , JAVAANDR.class_Vibrator,             "vibrate",               "(J)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Vibrator_vibrate_longArray_int             , JAVAANDR.class_Vibrator,             "vibrate",               "([JI)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Vibrator_cancel                            , JAVAANDR.class_Vibrator,             "cancel",                "()V" ) ;
	if( API_LV >= 26 )
	{
		GETSTATICMETHOD( JAVAANDR.methodID_VibrationEffect_createOneShot                  , JAVAANDR.class_VibrationEffect,   "createOneShot",    "(JI)Landroid/os/VibrationEffect;" ) ;
		GETSTATICMETHOD( JAVAANDR.methodID_VibrationEffect_createWaveform_withTimings     , JAVAANDR.class_VibrationEffect,   "createWaveform",   "([JI)Landroid/os/VibrationEffect;" ) ;
		GETSTATICMETHOD( JAVAANDR.methodID_VibrationEffect_createWaveform_withAmplitudes  , JAVAANDR.class_VibrationEffect,   "createWaveform",   "([J[II)Landroid/os/VibrationEffect;" ) ;
	}
	GETSTATICMETHOD( JAVAANDR.methodID_Process_myPid                              , JAVAANDR.class_Process,              "myPid",                 "()I" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_Process_killProcess                        , JAVAANDR.class_Process,              "killProcess",           "(I)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_InputStream_read_0param                    , JAVAANDR.class_InputStream,          "read",                  "()I" ) ;
	GETMETHOD(       JAVAANDR.methodID_InputStream_read_1param                    , JAVAANDR.class_InputStream,          "read",                  "([B)I" ) ;
	GETMETHOD(       JAVAANDR.methodID_InputStream_close                          , JAVAANDR.class_InputStream,          "close",                 "()V" ) ;
	GETMETHOD(       JAVAANDR.methodID_File_getAbsolutePath                       , JAVAANDR.class_File,                 "getAbsolutePath",       "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_CharSequence_toString                      , JAVAANDR.class_CharSequence,         "toString",              "()Ljava/lang/String;" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_Integer_parseInt                           , JAVAANDR.class_Integer,              "parseInt",              "(Ljava/lang/String;)I" ) ;
	GETMETHOD(       JAVAANDR.methodID_String_subSequence                         , JAVAANDR.class_String,               "subSequence",           "(II)Ljava/lang/CharSequence;" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_Runtime_getRuntime                         , JAVAANDR.class_Runtime,              "getRuntime",            "()Ljava/lang/Runtime;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Runtime_freeMemory                         , JAVAANDR.class_Runtime,              "freeMemory",            "()J" ) ;
	GETMETHOD(       JAVAANDR.methodID_Runtime_totalMemory                        , JAVAANDR.class_Runtime,              "totalMemory",           "()J" ) ;
	GETMETHOD(       JAVAANDR.methodID_Runtime_maxMemory                          , JAVAANDR.class_Runtime,              "maxMemory",             "()J" ) ;
	GETMETHOD(       JAVAANDR.methodID_List_size                                  , JAVAANDR.class_List,                 "size",                  "()I" ) ;
	GETMETHOD(       JAVAANDR.methodID_List_get                                   , JAVAANDR.class_List,                 "get",                   "(I)Ljava/lang/Object;" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_Locale_getDefault                          , JAVAANDR.class_Locale,               "getDefault",            "()Ljava/util/Locale;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Locale_getLanguage                         , JAVAANDR.class_Locale,               "getLanguage",           "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Locale_getCountry                          , JAVAANDR.class_Locale,               "getCountry",            "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Calendar_get                               , JAVAANDR.class_Calendar,             "get",                   "(I)I" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_Calendar_getInstance                       , JAVAANDR.class_Calendar,             "getInstance",           "()Ljava/util/Calendar;" ) ;
	GETMETHOD(       JAVAANDR.methodID_URLConnection_setDoInput                   , JAVAANDR.class_URLConnection,        "setDoInput",            "(Z)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_URLConnection_setDoOutput                  , JAVAANDR.class_URLConnection,        "setDoOutput",           "(Z)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_URLConnection_connect                      , JAVAANDR.class_URLConnection,        "connect",               "()V" ) ;
	GETMETHOD(       JAVAANDR.methodID_URLConnection_getInputStream               , JAVAANDR.class_URLConnection,        "getInputStream",        "()Ljava/io/InputStream;" ) ;
	GETMETHOD(       JAVAANDR.methodID_HttpURLConnection_newHttpURLConnection     , JAVAANDR.class_HttpURLConnection,    "<init>",                "(Ljava/net/URL;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_HttpURLConnection_setRequestMethod         , JAVAANDR.class_HttpURLConnection,    "setRequestMethod",      "(Ljava/lang/String;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_HttpURLConnection_setInstanceFollowRedirects, JAVAANDR.class_HttpURLConnection,   "setInstanceFollowRedirects", "(Z)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_HttpURLConnection_getResponseCode          , JAVAANDR.class_HttpURLConnection,    "getResponseCode",       "()I" ) ;
	GETMETHOD(       JAVAANDR.methodID_URL_newURL                                 , JAVAANDR.class_URL,                  "<init>",                "(Ljava/lang/String;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_URL_openConnection                         , JAVAANDR.class_URL,                  "openConnection",        "()Ljava/net/URLConnection;" ) ;
	GETMETHOD(       JAVAANDR.methodID_ActivityManager_getMemoryInfo              , JAVAANDR.class_ActivityManager,      "getMemoryInfo",         "(Landroid/app/ActivityManager$MemoryInfo;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_ActivityManager_getProcessMemoryInfo       , JAVAANDR.class_ActivityManager,      "getProcessMemoryInfo",  "([I)[Landroid/os/Debug$MemoryInfo;" ) ;
	GETMETHOD(       JAVAANDR.methodID_ActivityManager_MemoryInfo_newActivityManager_MemoryInfo , JAVAANDR.class_ActivityManager_MemoryInfo, "<init>", "()V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Dialog_isShowing                           , JAVAANDR.class_Dialog,               "isShowing",             "()Z" ) ;
	GETMETHOD(       JAVAANDR.methodID_AlertDialog_Builder_newAlertDialog_Builder , JAVAANDR.class_AlertDialog_Builder,  "<init>",                "(Landroid/content/Context;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_AlertDialog_Builder_setIcon                , JAVAANDR.class_AlertDialog_Builder,  "setIcon",               "(Landroid/graphics/drawable/Drawable;)Landroid/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVAANDR.methodID_AlertDialog_Builder_setTitle               , JAVAANDR.class_AlertDialog_Builder,  "setTitle",              "(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVAANDR.methodID_AlertDialog_Builder_setView                , JAVAANDR.class_AlertDialog_Builder,  "setView",               "(Landroid/view/View;)Landroid/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVAANDR.methodID_AlertDialog_Builder_setPositiveButton      , JAVAANDR.class_AlertDialog_Builder,  "setPositiveButton",     "(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVAANDR.methodID_AlertDialog_Builder_setNegativeButton      , JAVAANDR.class_AlertDialog_Builder,  "setNegativeButton",     "(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVAANDR.methodID_AlertDialog_Builder_show                   , JAVAANDR.class_AlertDialog_Builder,  "show",                  "()Landroid/app/AlertDialog;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Notification_newNotification               , JAVAANDR.class_Notification,         "<init>",                "()V" ) ;
	if( API_LV < 11 )
	{
		GETMETHOD(   JAVAANDR.methodID_Notification_setLatestEventInfo            , JAVAANDR.class_Notification,         "setLatestEventInfo",    "(Landroid/content/Context;Ljava/lang/CharSequence;Ljava/lang/CharSequence;Landroid/app/PendingIntent;)V" ) ;
	}
	GETMETHOD(       JAVAANDR.methodID_NotificationManager_notify                 , JAVAANDR.class_NotificationManager,  "notify",                "(ILandroid/app/Notification;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_NotificationManager_cancel                 , JAVAANDR.class_NotificationManager,  "cancel",                "(I)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_NotificationManager_cancelAll              , JAVAANDR.class_NotificationManager,  "cancelAll",             "()V" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_PendingIntent_getActivity                  , JAVAANDR.class_PendingIntent,        "getActivity",           "(Landroid/content/Context;ILandroid/content/Intent;I)Landroid/app/PendingIntent;" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_PendingIntent_getActivities                , JAVAANDR.class_PendingIntent,        "getActivities",         "(Landroid/content/Context;I[Landroid/content/Intent;I)Landroid/app/PendingIntent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_newIntent                           , JAVAANDR.class_Intent,               "<init>",                "(Landroid/content/Context;Ljava/lang/Class;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_newIntent_NoParam                   , JAVAANDR.class_Intent,               "<init>",                "()V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_newIntent_Uri                       , JAVAANDR.class_Intent,               "<init>",                "(Ljava/lang/String;Landroid/net/Uri;)V" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_Intent_createChooser                       , JAVAANDR.class_Intent,               "createChooser",         "(Landroid/content/Intent;Ljava/lang/CharSequence;)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_setFlags                            , JAVAANDR.class_Intent,               "setFlags",              "(I)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_setClassName                        , JAVAANDR.class_Intent,               "setClassName",          "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_getIntExtra                         , JAVAANDR.class_Intent,               "getIntExtra",           "(Ljava/lang/String;I)I" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_setAction                           , JAVAANDR.class_Intent,               "setAction",             "(Ljava/lang/String;)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_setData                             , JAVAANDR.class_Intent,               "setData",               "(Landroid/net/Uri;)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_setType                             , JAVAANDR.class_Intent,               "setType",               "(Ljava/lang/String;)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_addCategory                         , JAVAANDR.class_Intent,               "addCategory",           "(Ljava/lang/String;)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_boolean                    , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;Z)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_byte                       , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;B)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_char                       , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;C)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_short                      , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;S)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_int                        , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;I)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_long                       , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;J)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_float                      , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;F)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_double                     , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;D)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_string                     , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Intent_putExtra_stringArray                , JAVAANDR.class_Intent,               "putExtra",              "(Ljava/lang/String;[Ljava/lang/String;)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_IntentFilter_newIntentFilter               , JAVAANDR.class_IntentFilter,         "<init>",                "(Ljava/lang/String;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_ClipboardManager_setPrimaryClip            , JAVAANDR.class_ClipboardManager,     "setPrimaryClip",        "(Landroid/content/ClipData;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_ClipboardManager_getPrimaryClip            , JAVAANDR.class_ClipboardManager,     "getPrimaryClip",        "()Landroid/content/ClipData;" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_ClipData_newPlainText                      , JAVAANDR.class_ClipData,             "newPlainText",          "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;" ) ;
	GETMETHOD(       JAVAANDR.methodID_ClipData_toString                          , JAVAANDR.class_ClipData,             "toString",              "()Ljava/lang/String;" ) ;
	if( API_LV >= 21 )
	{
		GETMETHOD(   JAVAANDR.methodID_BaseBundle_getString                       , JAVAANDR.class_BaseBundle,           "getString",             "(Ljava/lang/String;)Ljava/lang/String;" ) ;
		GETMETHOD(   JAVAANDR.methodID_BaseBundle_getBoolean                      , JAVAANDR.class_BaseBundle,           "getBoolean",            "(Ljava/lang/String;)Z" ) ;
	}
	else
	{
		GETMETHOD(   JAVAANDR.methodID_Bundle_getString                           , JAVAANDR.class_Bundle,               "getString",             "(Ljava/lang/String;)Ljava/lang/String;" ) ;
		GETMETHOD(   JAVAANDR.methodID_Bundle_getBoolean                          , JAVAANDR.class_Bundle,               "getBoolean",            "(Ljava/lang/String;)Z" ) ;
	}
	GETMETHOD(       JAVAANDR.methodID_Debug_MemoryInfo_getTotalPss               , JAVAANDR.class_Debug_MemoryInfo,     "getTotalPss",            "()I" ) ;
	GETMETHOD(       JAVAANDR.methodID_Context_getResources                       , JAVAANDR.class_Context,              "getResources",          "()Landroid/content/res/Resources;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Context_getPackageManager                  , JAVAANDR.class_Context,              "getPackageManager",     "()Landroid/content/pm/PackageManager;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Context_getPackageName                     , JAVAANDR.class_Context,              "getPackageName",        "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Context_getApplicationContext              , JAVAANDR.class_Context,              "getApplicationContext", "()Landroid/content/Context;" ) ;
	if( API_LV >= 19 )
	{
		GETMETHOD(   JAVAANDR.methodID_Context_getExternalFilesDirs               , JAVAANDR.class_Context,              "getExternalFilesDirs",  "(Ljava/lang/String;)[Ljava/io/File;" ) ;
	}
	GETMETHOD(       JAVAANDR.methodID_Context_getSystemService                   , JAVAANDR.class_Context,              "getSystemService",      "(Ljava/lang/String;)Ljava/lang/Object;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Context_registerReceiver                   , JAVAANDR.class_Context,              "registerReceiver",      "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Resources_getString                        , JAVAANDR.class_Resources,            "getString",             "(I)Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Resources_getResourceName                  , JAVAANDR.class_Resources,            "getResourceName",       "(I)Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Resources_getIdentifier                    , JAVAANDR.class_Resources,            "getIdentifier",         "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I" ) ;
	GETMETHOD(       JAVAANDR.methodID_PackageManager_getApplicationInfo          , JAVAANDR.class_PackageManager,       "getApplicationInfo",    "(Ljava/lang/String;I)Landroid/content/pm/ApplicationInfo;" ) ;
	GETMETHOD(       JAVAANDR.methodID_PackageManager_hasSystemFeature            , JAVAANDR.class_PackageManager,       "hasSystemFeature",      "(Ljava/lang/String;)Z" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_SensorManager_getRotationMatrix            , JAVAANDR.class_SensorManager,        "getRotationMatrix",     "([F[F[F[F)Z" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_SensorManager_remapCoordinateSystem        , JAVAANDR.class_SensorManager,        "remapCoordinateSystem", "([FII[F)Z" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_SensorManager_getOrientation               , JAVAANDR.class_SensorManager,        "getOrientation",        "([F[F)[F" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_Uri_parse                                  , JAVAANDR.class_Uri,                  "parse",                 "(Ljava/lang/String;)Landroid/net/Uri;" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_getSSID                           , JAVAANDR.class_WifiInfo,             "getSSID",               "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_getBSSID                          , JAVAANDR.class_WifiInfo,             "getBSSID",              "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_getRssi                           , JAVAANDR.class_WifiInfo,             "getRssi",               "()I" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_getLinkSpeed                      , JAVAANDR.class_WifiInfo,             "getLinkSpeed",          "()I" ) ;
	if( API_LV >= 21 )
	{
		GETMETHOD(   JAVAANDR.methodID_WifiInfo_getFrequency                      , JAVAANDR.class_WifiInfo,             "getFrequency",          "()I" ) ;
	}
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_getMacAddress                     , JAVAANDR.class_WifiInfo,             "getMacAddress",         "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_getNetworkId                      , JAVAANDR.class_WifiInfo,             "getNetworkId",          "()I" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_getIpAddress                      , JAVAANDR.class_WifiInfo,             "getIpAddress",          "()I" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_getHiddenSSID                     , JAVAANDR.class_WifiInfo,             "getHiddenSSID",         "()Z" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_toString                          , JAVAANDR.class_WifiInfo,             "toString",              "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiInfo_describeContents                  , JAVAANDR.class_WifiInfo,             "describeContents",      "()I" ) ;
	GETSTATICMETHOD( JAVAANDR.methodID_WifiManager_calculateSignalLevel           , JAVAANDR.class_WifiManager,          "calculateSignalLevel",  "(II)I" ) ;
	GETMETHOD(       JAVAANDR.methodID_WifiManager_getConnectionInfo              , JAVAANDR.class_WifiManager,          "getConnectionInfo",     "()Landroid/net/wifi/WifiInfo;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Activity_startActivity                     , JAVAANDR.class_Activity,             "startActivity",         "(Landroid/content/Intent;)V" ) ;
	GETMETHOD(       JAVAANDR.methodID_Activity_finish                            , JAVAANDR.class_Activity,             "finish",                "()V" ) ;
	if( API_LV >= 21 )
	{
		GETMETHOD(   JAVAANDR.methodID_Activity_finishAndRemoveTask               , JAVAANDR.class_Activity,             "finishAndRemoveTask",   "()V" ) ;
		GETMETHOD(   JAVAANDR.methodID_Activity_releaseInstance                   , JAVAANDR.class_Activity,             "releaseInstance",       "()Z" ) ;
	}
	GETMETHOD(       JAVAANDR.methodID_Activity_getCurrentFocus                   , JAVAANDR.class_Activity,             "getCurrentFocus",       "()Landroid/view/View;" ) ;
	GETMETHOD(       JAVAANDR.methodID_Activity_getWindow                         , JAVAANDR.class_Activity,             "getWindow",             "()Landroid/view/Window;" ) ;
	if( API_LV >= 17 )
	{
		GETMETHOD(   JAVAANDR.methodID_AudioManager_getProperty                   , JAVAANDR.class_AudioManager,         "getProperty",           "(Ljava/lang/String;)Ljava/lang/String;" ) ;
	}


	GETSTATICFIELD( JAVAANDR.fieldID_BitmapConfig_ARGB_8888                     , JAVAANDR.class_BitmapConfig,            "ARGB_8888",                           "Landroid/graphics/Bitmap$Config;" ) ;
	GETFIELD(       JAVAANDR.fieldID_PaintFontMetrics_ascent                    , JAVAANDR.class_PaintFontMetrics,        "ascent",                              "F" ) ;
	GETFIELD(       JAVAANDR.fieldID_PaintFontMetrics_bottom                    , JAVAANDR.class_PaintFontMetrics,        "bottom",                              "F" ) ;
	GETFIELD(       JAVAANDR.fieldID_PaintFontMetrics_descent                   , JAVAANDR.class_PaintFontMetrics,        "descent",                             "F" ) ;
	GETFIELD(       JAVAANDR.fieldID_PaintFontMetrics_leading                   , JAVAANDR.class_PaintFontMetrics,        "leading",                             "F" ) ;
	GETFIELD(       JAVAANDR.fieldID_PaintFontMetrics_top                       , JAVAANDR.class_PaintFontMetrics,        "top",                                 "F" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Typeface_NORMAL                            , JAVAANDR.class_Typeface,                "NORMAL",                              "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Typeface_BOLD                              , JAVAANDR.class_Typeface,                "BOLD",                                "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Typeface_ITALIC                            , JAVAANDR.class_Typeface,                "ITALIC",                              "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Typeface_BOLD_ITALIC                       , JAVAANDR.class_Typeface,                "BOLD_ITALIC",                         "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_BLACK                                , JAVAANDR.class_Color,                   "BLACK",                               "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_BLUE                                 , JAVAANDR.class_Color,                   "BLUE",                                "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_CYAN                                 , JAVAANDR.class_Color,                   "CYAN",                                "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_DKGRAY                               , JAVAANDR.class_Color,                   "DKGRAY",                              "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_GRAY                                 , JAVAANDR.class_Color,                   "GRAY",                                "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_GREEN                                , JAVAANDR.class_Color,                   "GREEN",                               "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_LTGRAY                               , JAVAANDR.class_Color,                   "LTGRAY",                              "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_MAGENTA                              , JAVAANDR.class_Color,                   "MAGENTA",                             "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_RED                                  , JAVAANDR.class_Color,                   "RED",                                 "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_TRANSPARENT                          , JAVAANDR.class_Color,                   "TRANSPARENT",                         "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_WHITE                                , JAVAANDR.class_Color,                   "WHITE",                               "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Color_YELLOW                               , JAVAANDR.class_Color,                   "YELLOW",                              "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Rect_bottom                                , JAVAANDR.class_Rect,                    "bottom",                              "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Rect_left                                  , JAVAANDR.class_Rect,                    "left",                                "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Rect_right                                 , JAVAANDR.class_Rect,                    "right",                               "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Rect_top                                   , JAVAANDR.class_Rect,                    "top",                                 "I" ) ;

	GETSTATICFIELD( JAVAANDR.fieldID_BatteryManager_EXTRA_LEVEL                 , JAVAANDR.class_BatteryManager,          "EXTRA_LEVEL",                         "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_BatteryManager_EXTRA_SCALE                 , JAVAANDR.class_BatteryManager,          "EXTRA_SCALE",                         "Ljava/lang/String;" ) ;

	GETSTATICFIELD( JAVAANDR.fieldID_TextView_BufferType_EDITABLE               , JAVAANDR.class_TextView_BufferType,     "EDITABLE",                            "Landroid/widget/TextView$BufferType;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_TextView_BufferType_NORMAL                 , JAVAANDR.class_TextView_BufferType,     "NORMAL",                              "Landroid/widget/TextView$BufferType;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_TextView_BufferType_SPANNABLE              , JAVAANDR.class_TextView_BufferType,     "SPANNABLE",                           "Landroid/widget/TextView$BufferType;" ) ;

	GETFIELD(       JAVAANDR.fieldID_ActivityManager_MemoryInfo_availMem        , JAVAANDR.class_ActivityManager_MemoryInfo, "availMem",                         "J" ) ;
	GETFIELD(       JAVAANDR.fieldID_ActivityManager_MemoryInfo_lowMemory       , JAVAANDR.class_ActivityManager_MemoryInfo, "lowMemory",                        "Z" ) ;
	GETFIELD(       JAVAANDR.fieldID_ActivityManager_MemoryInfo_threshold       , JAVAANDR.class_ActivityManager_MemoryInfo, "threshold",                        "J" ) ;
	if( API_LV >= 16 )
	{
		GETFIELD(   JAVAANDR.fieldID_ActivityManager_MemoryInfo_totalMem        , JAVAANDR.class_ActivityManager_MemoryInfo, "totalMem",                         "J" ) ;
	}

	GETSTATICFIELD( JAVAANDR.fieldID_Notification_DEFAULT_LIGHTS                , JAVAANDR.class_Notification,            "DEFAULT_LIGHTS",                      "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Notification_DEFAULT_VIBRATE               , JAVAANDR.class_Notification,            "DEFAULT_VIBRATE",                     "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Notification_FLAG_SHOW_LIGHTS              , JAVAANDR.class_Notification,            "FLAG_SHOW_LIGHTS",                    "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Notification_icon                          , JAVAANDR.class_Notification,            "icon",                                "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Notification_tickerText                    , JAVAANDR.class_Notification,            "tickerText",                          "Ljava/lang/CharSequence;" ) ;
	GETFIELD(       JAVAANDR.fieldID_Notification_defaults                      , JAVAANDR.class_Notification,            "defaults",                            "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Notification_vibrate                       , JAVAANDR.class_Notification,            "vibrate",                             "[J" ) ;
	GETFIELD(       JAVAANDR.fieldID_Notification_ledARGB                       , JAVAANDR.class_Notification,            "ledARGB",                             "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Notification_ledOffMS                      , JAVAANDR.class_Notification,            "ledOffMS",                            "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Notification_ledOnMS                       , JAVAANDR.class_Notification,            "ledOnMS",                             "I" ) ;
	GETFIELD(       JAVAANDR.fieldID_Notification_flags                         , JAVAANDR.class_Notification,            "flags",                               "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_PendingIntent_FLAG_CANCEL_CURRENT	        , JAVAANDR.class_PendingIntent,           "FLAG_CANCEL_CURRENT",                 "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_PendingIntent_FLAG_NO_CREATE               , JAVAANDR.class_PendingIntent,           "FLAG_NO_CREATE",                      "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_PendingIntent_FLAG_ONE_SHOT                , JAVAANDR.class_PendingIntent,           "FLAG_ONE_SHOT",                       "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_PendingIntent_FLAG_UPDATE_CURRENT          , JAVAANDR.class_PendingIntent,           "FLAG_UPDATE_CURRENT",                 "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_ACTION_BATTERY_CHANGED              , JAVAANDR.class_Intent,                  "ACTION_BATTERY_CHANGED",              "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_ACTION_OPEN_DOCUMENT                , JAVAANDR.class_Intent,                  "ACTION_OPEN_DOCUMENT",                "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_ACTION_SEND                         , JAVAANDR.class_Intent,                  "ACTION_SEND",                         "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_ACTION_SENDTO                       , JAVAANDR.class_Intent,                  "ACTION_SENDTO",                       "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_ACTION_VIEW                         , JAVAANDR.class_Intent,                  "ACTION_VIEW",                         "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_EXTRA_EMAIL                         , JAVAANDR.class_Intent,                  "EXTRA_EMAIL",                         "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_EXTRA_STREAM                        , JAVAANDR.class_Intent,                  "EXTRA_STREAM",                        "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_EXTRA_SUBJECT                       , JAVAANDR.class_Intent,                  "EXTRA_SUBJECT",                       "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_EXTRA_TEXT                          , JAVAANDR.class_Intent,                  "EXTRA_TEXT",                          "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_EXTRA_CC                            , JAVAANDR.class_Intent,                  "EXTRA_CC",                            "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_EXTRA_BCC                           , JAVAANDR.class_Intent,                  "EXTRA_BCC",                           "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_CATEGORY_OPENABLE                   , JAVAANDR.class_Intent,                  "CATEGORY_OPENABLE",                   "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_NEW_TASK              , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_NEW_TASK",              "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_SINGLE_TOP            , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_SINGLE_TOP",            "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_CLEAR_TOP             , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_CLEAR_TOP",             "I" ) ;
	if( API_LV >= 11 )
	{
		GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_CLEAR_TASK            , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_CLEAR_TASK",            "I" ) ;
	}
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_NO_ANIMATION          , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_NO_ANIMATION",          "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_NO_HISTORY            , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_NO_HISTORY",            "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY", "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS  , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS",  "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_PREVIOUS_IS_TOP       , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_PREVIOUS_IS_TOP",       "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_REORDER_TO_FRONT      , JAVAANDR.class_Intent,                  "FLAG_ACTIVITY_REORDER_TO_FRONT",      "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Context_NOTIFICATION_SERVICE               , JAVAANDR.class_Context,                 "NOTIFICATION_SERVICE",                "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Context_AUDIO_SERVICE                      , JAVAANDR.class_Context,                 "AUDIO_SERVICE",                       "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Context_ACTIVITY_SERVICE                   , JAVAANDR.class_Context,                 "ACTIVITY_SERVICE",                    "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Context_VIBRATOR_SERVICE                   , JAVAANDR.class_Context,                 "VIBRATOR_SERVICE",                    "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Context_SENSOR_SERVICE                     , JAVAANDR.class_Context,                 "SENSOR_SERVICE",                      "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Context_WIFI_SERVICE                       , JAVAANDR.class_Context,                 "WIFI_SERVICE",                        "Ljava/lang/String;" ) ;
	if( API_LV >= 18 )
	{
		GETSTATICFIELD( JAVAANDR.fieldID_Context_BLUETOOTH_SERVICE                  , JAVAANDR.class_Context,                 "BLUETOOTH_SERVICE",                   "Ljava/lang/String;" ) ;
	}
	if( API_LV >= 21 )
	{
		GETSTATICFIELD( JAVAANDR.fieldID_Context_CAMERA_SERVICE                     , JAVAANDR.class_Context,                 "CAMERA_SERVICE",                      "Ljava/lang/String;" ) ;
	}
	if( API_LV >= 19 )
	{
		GETSTATICFIELD( JAVAANDR.fieldID_Context_CAPTIONING_SERVICE                 , JAVAANDR.class_Context,                 "CAPTIONING_SERVICE",                  "Ljava/lang/String;" ) ;
	}
	GETSTATICFIELD( JAVAANDR.fieldID_Context_CLIPBOARD_SERVICE                  , JAVAANDR.class_Context,                 "CLIPBOARD_SERVICE",                   "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Context_CONNECTIVITY_SERVICE               , JAVAANDR.class_Context,                 "CONNECTIVITY_SERVICE",                "Ljava/lang/String;" ) ;
	if( API_LV >= 19 )
	{
		GETSTATICFIELD( JAVAANDR.fieldID_Context_CONSUMER_IR_SERVICE                , JAVAANDR.class_Context,                 "CONSUMER_IR_SERVICE",                 "Ljava/lang/String;" ) ;
	}
	GETFIELD(       JAVAANDR.fieldID_PackageItemInfo_metaData                   , JAVAANDR.class_PackageItemInfo,         "metaData",                            "Landroid/os/Bundle;" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_PackageManager_GET_META_DATA               , JAVAANDR.class_PackageManager,          "GET_META_DATA",                       "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_SensorManager_AXIS_MINUS_X                 , JAVAANDR.class_SensorManager,           "AXIS_MINUS_X",                        "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_SensorManager_AXIS_MINUS_Y                 , JAVAANDR.class_SensorManager,           "AXIS_MINUS_Y",                        "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_SensorManager_AXIS_MINUS_Z                 , JAVAANDR.class_SensorManager,           "AXIS_MINUS_Z",                        "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_SensorManager_AXIS_X                       , JAVAANDR.class_SensorManager,           "AXIS_X",                              "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_SensorManager_AXIS_Y                       , JAVAANDR.class_SensorManager,           "AXIS_Y",                              "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_SensorManager_AXIS_Z                       , JAVAANDR.class_SensorManager,           "AXIS_Z",                              "I" ) ;
	if( API_LV >= 26 )
	{
		GETSTATICFIELD( JAVAANDR.fieldID_VibrationEffect_DEFAULT_AMPLITUDE      , JAVAANDR.class_VibrationEffect,         "DEFAULT_AMPLITUDE",                   "I" ) ;
	}
	GETSTATICFIELD( JAVAANDR.fieldID_R_mipmap_sym_def_app_icon                  , JAVAANDR.class_R_mipmap,                "sym_def_app_icon",                    "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Calendar_DAY_OF_WEEK                       , JAVAANDR.class_Calendar,                "DAY_OF_WEEK",                         "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Calendar_WEEK_OF_MONTH                     , JAVAANDR.class_Calendar,                "WEEK_OF_MONTH",                       "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_Calendar_DAY_OF_WEEK_IN_MONTH              , JAVAANDR.class_Calendar,                "DAY_OF_WEEK_IN_MONTH",                "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_OK                  , JAVAANDR.class_HttpURLConnection,       "HTTP_OK",                             "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_NO_CONTENT          , JAVAANDR.class_HttpURLConnection,       "HTTP_NO_CONTENT",                     "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_NOT_FOUND           , JAVAANDR.class_HttpURLConnection,       "HTTP_NOT_FOUND",                      "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_BAD_METHOD          , JAVAANDR.class_HttpURLConnection,       "HTTP_BAD_METHOD",                     "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_NOT_ACCEPTABLE      , JAVAANDR.class_HttpURLConnection,       "HTTP_NOT_ACCEPTABLE",                 "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_BAD_GATEWAY         , JAVAANDR.class_HttpURLConnection,       "HTTP_BAD_GATEWAY",                    "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_NOT_IMPLEMENTED     , JAVAANDR.class_HttpURLConnection,       "HTTP_NOT_IMPLEMENTED",                "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_SERVER_ERROR        , JAVAANDR.class_HttpURLConnection,       "HTTP_SERVER_ERROR",                   "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_GATEWAY_TIMEOUT     , JAVAANDR.class_HttpURLConnection,       "HTTP_GATEWAY_TIMEOUT",                "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_UNAVAILABLE         , JAVAANDR.class_HttpURLConnection,       "HTTP_UNAVAILABLE",                    "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_UNSUPPORTED_TYPE    , JAVAANDR.class_HttpURLConnection,       "HTTP_UNSUPPORTED_TYPE",               "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_PRECON_FAILED       , JAVAANDR.class_HttpURLConnection,       "HTTP_PRECON_FAILED",                  "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_CONFLICT            , JAVAANDR.class_HttpURLConnection,       "HTTP_CONFLICT",                       "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_UNAUTHORIZED        , JAVAANDR.class_HttpURLConnection,       "HTTP_UNAUTHORIZED",                   "I" ) ;
	GETSTATICFIELD( JAVAANDR.fieldID_HttpURLConnection_HTTP_BAD_REQUEST         , JAVAANDR.class_HttpURLConnection,       "HTTP_BAD_REQUEST",                    "I" ) ;
	if( API_LV >= 17 )
	{
		GETSTATICFIELD( JAVAANDR.fieldID_AudioManager_PROPERTY_OUTPUT_FRAMES_PER_BUFFER , JAVAANDR.class_AudioManager,    "PROPERTY_OUTPUT_FRAMES_PER_BUFFER",   "Ljava/lang/String;" ) ;
		GETSTATICFIELD( JAVAANDR.fieldID_AudioManager_PROPERTY_OUTPUT_SAMPLE_RATE       , JAVAANDR.class_AudioManager,    "PROPERTY_OUTPUT_SAMPLE_RATE",         "Ljava/lang/String;" ) ;
	}
	


	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_BitmapConfig_ARGB_8888,        JAVAANDR.class_BitmapConfig,        JAVAANDR.fieldID_BitmapConfig_ARGB_8888, "BitmapConfig_ARGB_8888" ) ;
	JAVAANDR.fieldint_Typeface_NORMAL           = env->GetStaticIntField(     JAVAANDR.class_Typeface,            JAVAANDR.fieldID_Typeface_NORMAL      ) ;
	JAVAANDR.fieldint_Typeface_BOLD             = env->GetStaticIntField(     JAVAANDR.class_Typeface,            JAVAANDR.fieldID_Typeface_BOLD        ) ;
	JAVAANDR.fieldint_Typeface_ITALIC           = env->GetStaticIntField(     JAVAANDR.class_Typeface,            JAVAANDR.fieldID_Typeface_ITALIC      ) ;
	JAVAANDR.fieldint_Typeface_BOLD_ITALIC      = env->GetStaticIntField(     JAVAANDR.class_Typeface,            JAVAANDR.fieldID_Typeface_BOLD_ITALIC ) ;
	JAVAANDR.fieldint_Color_BLACK               = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_BLACK          ) ;
	JAVAANDR.fieldint_Color_BLUE                = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_BLUE           ) ;
	JAVAANDR.fieldint_Color_CYAN                = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_CYAN           ) ;
	JAVAANDR.fieldint_Color_DKGRAY              = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_DKGRAY         ) ;
	JAVAANDR.fieldint_Color_GRAY                = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_GRAY           ) ;
	JAVAANDR.fieldint_Color_GREEN               = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_GREEN          ) ;
	JAVAANDR.fieldint_Color_LTGRAY              = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_LTGRAY         ) ;
	JAVAANDR.fieldint_Color_MAGENTA             = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_MAGENTA        ) ;
	JAVAANDR.fieldint_Color_RED                 = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_RED            ) ;
	JAVAANDR.fieldint_Color_TRANSPARENT         = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_TRANSPARENT    ) ;
	JAVAANDR.fieldint_Color_WHITE               = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_WHITE          ) ;
	JAVAANDR.fieldint_Color_YELLOW              = env->GetStaticIntField(     JAVAANDR.class_Color,               JAVAANDR.fieldID_Color_YELLOW         ) ;

	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_BatteryManager_EXTRA_LEVEL,    JAVAANDR.class_BatteryManager,      JAVAANDR.fieldID_BatteryManager_EXTRA_LEVEL, "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_BatteryManager_EXTRA_SCALE,    JAVAANDR.class_BatteryManager,      JAVAANDR.fieldID_BatteryManager_EXTRA_SCALE, "Ljava/lang/String;" ) ;

	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_TextView_BufferType_EDITABLE,  JAVAANDR.class_TextView_BufferType, JAVAANDR.fieldID_TextView_BufferType_EDITABLE,  "TextView_BufferType_EDITABLE"  ) ;
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_TextView_BufferType_NORMAL,    JAVAANDR.class_TextView_BufferType, JAVAANDR.fieldID_TextView_BufferType_NORMAL,    "TextView_BufferType_NORMAL"    ) ;
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_TextView_BufferType_SPANNABLE, JAVAANDR.class_TextView_BufferType, JAVAANDR.fieldID_TextView_BufferType_SPANNABLE, "TextView_BufferType_SPANNABLE" ) ;

	JAVAANDR.fieldint_Notification_DEFAULT_LIGHTS                = env->GetStaticIntField( JAVAANDR.class_Notification,        JAVAANDR.fieldID_Notification_DEFAULT_LIGHTS   ) ;
	JAVAANDR.fieldint_Notification_DEFAULT_VIBRATE               = env->GetStaticIntField( JAVAANDR.class_Notification,        JAVAANDR.fieldID_Notification_DEFAULT_VIBRATE  ) ;
	JAVAANDR.fieldint_Notification_FLAG_SHOW_LIGHTS              = env->GetStaticIntField( JAVAANDR.class_Notification,        JAVAANDR.fieldID_Notification_FLAG_SHOW_LIGHTS ) ;
	JAVAANDR.fieldint_PendingIntent_FLAG_CANCEL_CURRENT          = env->GetStaticIntField( JAVAANDR.class_PendingIntent,       JAVAANDR.fieldID_PendingIntent_FLAG_CANCEL_CURRENT ) ;
	JAVAANDR.fieldint_PendingIntent_FLAG_NO_CREATE               = env->GetStaticIntField( JAVAANDR.class_PendingIntent,       JAVAANDR.fieldID_PendingIntent_FLAG_NO_CREATE      ) ;
	JAVAANDR.fieldint_PendingIntent_FLAG_ONE_SHOT                = env->GetStaticIntField( JAVAANDR.class_PendingIntent,       JAVAANDR.fieldID_PendingIntent_FLAG_ONE_SHOT       ) ;
	JAVAANDR.fieldint_PendingIntent_FLAG_UPDATE_CURRENT          = env->GetStaticIntField( JAVAANDR.class_PendingIntent,       JAVAANDR.fieldID_PendingIntent_FLAG_UPDATE_CURRENT ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_ACTION_BATTERY_CHANGED,   JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_ACTION_BATTERY_CHANGED, "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_ACTION_OPEN_DOCUMENT,     JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_ACTION_OPEN_DOCUMENT,   "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_ACTION_SEND,              JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_ACTION_SEND,            "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_ACTION_SENDTO,            JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_ACTION_SENDTO,          "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_ACTION_VIEW,              JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_ACTION_VIEW,            "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_EXTRA_EMAIL,              JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_EXTRA_EMAIL,            "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_EXTRA_STREAM,             JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_EXTRA_STREAM,           "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_EXTRA_SUBJECT,            JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_EXTRA_SUBJECT,          "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_EXTRA_TEXT,               JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_EXTRA_TEXT,             "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_EXTRA_CC,                 JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_EXTRA_CC,               "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_EXTRA_BCC,                JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_EXTRA_BCC,              "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_Intent_CATEGORY_OPENABLE,        JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_CATEGORY_OPENABLE,      "Ljava/lang/String;" ) ;
	JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_NEW_TASK              = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_NEW_TASK     ) ;
	JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_SINGLE_TOP            = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_SINGLE_TOP   ) ;
	JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_CLEAR_TOP             = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_CLEAR_TOP    ) ;
	if( API_LV >= 11 )
	{
		JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_CLEAR_TASK        = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_CLEAR_TASK            ) ;
	}
	JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_NO_ANIMATION          = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_NO_ANIMATION          ) ;
	JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_NO_HISTORY            = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_NO_HISTORY            ) ;
	JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY ) ;
	JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS  = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS  ) ;
	JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_PREVIOUS_IS_TOP       = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_PREVIOUS_IS_TOP       ) ;
	JAVAANDR.fieldint_Intent_FLAG_ACTIVITY_REORDER_TO_FRONT      = env->GetStaticIntField( JAVAANDR.class_Intent,              JAVAANDR.fieldID_Intent_FLAG_ACTIVITY_REORDER_TO_FRONT      ) ;
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_NOTIFICATION_SERVICE,    JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_NOTIFICATION_SERVICE,  "Context_NOTIFICATION_SERVICE" ) ;
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_AUDIO_SERVICE,           JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_AUDIO_SERVICE,         "Context_AUDIO_SERVICE" ) ;
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_ACTIVITY_SERVICE,        JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_ACTIVITY_SERVICE,      "Context_ACTIVITY_SERVICE" ) ;
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_VIBRATOR_SERVICE,        JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_VIBRATOR_SERVICE,      "Context_VIBRATOR_SERVICE"     ) ;
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_SENSOR_SERVICE,          JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_SENSOR_SERVICE,        "Context_SENSOR_SERVICE"       ) ;
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_WIFI_SERVICE,            JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_WIFI_SERVICE,          "Context_WIFI_SERVICE"         ) ;
	if( API_LV >= 18 )
	{
		GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_BLUETOOTH_SERVICE,       JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_BLUETOOTH_SERVICE,     "Context_BLUETOOTH_SERVICE"    ) ;
	}
	if( API_LV >= 21 )
	{
		GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_CAMERA_SERVICE,          JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_CAMERA_SERVICE,        "Context_CAMERA_SERVICE"       ) ;
	}
	if( API_LV >= 19 )
	{
		GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_CAPTIONING_SERVICE,      JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_CAPTIONING_SERVICE,    "Context_CAPTIONING_SERVICE"   ) ;
	}
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_CLIPBOARD_SERVICE,       JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_CLIPBOARD_SERVICE,     "Context_CLIPBOARD_SERVICE"    ) ;
	GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_CONNECTIVITY_SERVICE,    JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_CONNECTIVITY_SERVICE,  "Context_CONNECTIVITY_SERVICE" ) ;
	if( API_LV >= 19 )
	{
		GETSTATICOBJECTFIELD( JAVAANDR.fieldobject_Context_CONSUMER_IR_SERVICE,     JAVAANDR.class_Context,             JAVAANDR.fieldID_Context_CONSUMER_IR_SERVICE,   "Context_CONSUMER_IR_SERVICE"  ) ;
	}
	JAVAANDR.fieldint_PackageManager_GET_META_DATA               = env->GetStaticIntField( JAVAANDR.class_PackageManager,      JAVAANDR.fieldID_PackageManager_GET_META_DATA ) ;
	JAVAANDR.fieldint_SensorManager_AXIS_MINUS_X                 = env->GetStaticIntField( JAVAANDR.class_SensorManager,       JAVAANDR.fieldID_SensorManager_AXIS_MINUS_X   ) ;
	JAVAANDR.fieldint_SensorManager_AXIS_MINUS_Y                 = env->GetStaticIntField( JAVAANDR.class_SensorManager,       JAVAANDR.fieldID_SensorManager_AXIS_MINUS_Y   ) ;
	JAVAANDR.fieldint_SensorManager_AXIS_MINUS_Z                 = env->GetStaticIntField( JAVAANDR.class_SensorManager,       JAVAANDR.fieldID_SensorManager_AXIS_MINUS_Z   ) ;
	JAVAANDR.fieldint_SensorManager_AXIS_X                       = env->GetStaticIntField( JAVAANDR.class_SensorManager,       JAVAANDR.fieldID_SensorManager_AXIS_X         ) ;
	JAVAANDR.fieldint_SensorManager_AXIS_Y                       = env->GetStaticIntField( JAVAANDR.class_SensorManager,       JAVAANDR.fieldID_SensorManager_AXIS_Y         ) ;
	JAVAANDR.fieldint_SensorManager_AXIS_Z                       = env->GetStaticIntField( JAVAANDR.class_SensorManager,       JAVAANDR.fieldID_SensorManager_AXIS_Z         ) ;
	if( API_LV >= 26 )
	{
		JAVAANDR.fieldint_VibrationEffect_DEFAULT_AMPLITUDE      = env->GetStaticIntField( JAVAANDR.class_VibrationEffect,     JAVAANDR.fieldID_VibrationEffect_DEFAULT_AMPLITUDE ) ;
	}
	JAVAANDR.fieldint_R_mipmap_sym_def_app_icon                  = env->GetStaticIntField( JAVAANDR.class_R_mipmap,            JAVAANDR.fieldID_R_mipmap_sym_def_app_icon  ) ;
	JAVAANDR.fieldint_Calendar_DAY_OF_WEEK                       = env->GetStaticIntField( JAVAANDR.class_Calendar,            JAVAANDR.fieldID_Calendar_DAY_OF_WEEK          ) ;
	JAVAANDR.fieldint_Calendar_WEEK_OF_MONTH                     = env->GetStaticIntField( JAVAANDR.class_Calendar,            JAVAANDR.fieldID_Calendar_WEEK_OF_MONTH        ) ;
	JAVAANDR.fieldint_Calendar_DAY_OF_WEEK_IN_MONTH              = env->GetStaticIntField( JAVAANDR.class_Calendar,            JAVAANDR.fieldID_Calendar_DAY_OF_WEEK_IN_MONTH ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_OK                  = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_OK               ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_NO_CONTENT          = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_NO_CONTENT       ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_NOT_FOUND           = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_NOT_FOUND        ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_BAD_METHOD          = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_BAD_METHOD       ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_NOT_ACCEPTABLE      = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_NOT_ACCEPTABLE   ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_BAD_GATEWAY         = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_BAD_GATEWAY      ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_NOT_IMPLEMENTED     = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_NOT_IMPLEMENTED  ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_SERVER_ERROR        = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_SERVER_ERROR     ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_GATEWAY_TIMEOUT     = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_GATEWAY_TIMEOUT  ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_UNAVAILABLE         = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_UNAVAILABLE      ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_UNSUPPORTED_TYPE    = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_UNSUPPORTED_TYPE ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_PRECON_FAILED       = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_PRECON_FAILED    ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_CONFLICT            = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_CONFLICT         ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_UNAUTHORIZED        = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_UNAUTHORIZED     ) ;
	JAVAANDR.fieldint_HttpURLConnection_HTTP_BAD_REQUEST         = env->GetStaticIntField( JAVAANDR.class_HttpURLConnection,   JAVAANDR.fieldID_HttpURLConnection_HTTP_BAD_REQUEST      ) ;
	if( API_LV >= 17 )
	{
		GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_AudioManager_PROPERTY_OUTPUT_FRAMES_PER_BUFFER,JAVAANDR.class_AudioManager, JAVAANDR.fieldID_AudioManager_PROPERTY_OUTPUT_FRAMES_PER_BUFFER, "Ljava/lang/String;" ) ;
		GETSTATICSTRINGFIELD( JAVAANDR.fieldstring_AudioManager_PROPERTY_OUTPUT_SAMPLE_RATE,      JAVAANDR.class_AudioManager, JAVAANDR.fieldID_AudioManager_PROPERTY_OUTPUT_SAMPLE_RATE,       "Ljava/lang/String;" ) ;
	}


	JAVAANDR.InitializeFlag = TRUE ;

	// 正常終了
	return 0 ;
}
#undef FINDCLASS
#undef GETMETHOD
#undef GETSTATICMETHOD
#undef GETFIELD
#undef GETSTATICFIELD

// Java のクラスやメソッドの参照の後始末をする
#define JAVA_DELETE_GLOBAL_REF( object )\
	if( object != NULL )\
	{\
		env->DeleteGlobalRef( object ) ;\
		object = NULL ;\
	}
extern int TerminateJavaAndroidInfo( JNIEnv *env )
{
	if( JAVAANDR.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Build_VERSION ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Bitmap ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_BitmapConfig ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Canvas ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Typeface ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Paint ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_PaintFontMetrics ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Color ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_BaseBundle ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Bundle ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_BatteryManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Environment ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Vibrator ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_VibrationEffect ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Process ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_TextView ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_TextView_BufferType ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_EditText ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Integer ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_String ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Runtime ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Locale ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_File ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_CharSequence ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_ActivityManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_ActivityManager_MemoryInfo ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Dialog ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_AlertDialog ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_AlertDialog_Builder ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Notification ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_NotificationManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Notification_Builder ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_PendingIntent ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Intent ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_IntentFilter ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_BroadcastReceiver ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Context ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Resources ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_PackageInfo ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_PackageItemInfo ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_ApplicationInfo ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_PackageManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_SensorManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_R_mipmap ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Uri ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_Activity ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.class_AudioManager ) ;

	JAVA_DELETE_GLOBAL_REF( JAVAANDR.fieldobject_BitmapConfig_ARGB_8888 ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.fieldobject_TextView_BufferType_EDITABLE ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.fieldobject_TextView_BufferType_NORMAL ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.fieldobject_TextView_BufferType_SPANNABLE ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.fieldobject_Context_NOTIFICATION_SERVICE ) ;
	JAVA_DELETE_GLOBAL_REF( JAVAANDR.fieldobject_Context_ACTIVITY_SERVICE ) ;

	JAVAANDR.InitializeFlag = FALSE ;

	return 0 ;
}
#undef JAVA_DELETE_GLOBAL_REF

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

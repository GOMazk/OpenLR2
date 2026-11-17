//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用フォント関係プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_FONT

// インクルード ---------------------------------------------------------------
#include "DxFontAndroid.h"
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

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

FONTSYSTEM_ANDR FontSystemAndroid ;

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

// フォントの Android 専用データを解放する
static int TerminateFontHandle_Android( JNIEnv *env, FONTMANAGE *ManageData )
{
	// オブジェクトの解放
	if( ManageData->PF->object_Canvas != NULL )
	{
		env->DeleteGlobalRef( ManageData->PF->object_Canvas ) ;
		ManageData->PF->object_Canvas = NULL ;
	}

	if( ManageData->PF->object_Paint != NULL )
	{
		env->DeleteGlobalRef( ManageData->PF->object_Paint ) ;
		ManageData->PF->object_Paint = NULL ;
	}

	if( ManageData->PF->object_Typeface != NULL )
	{
		env->DeleteGlobalRef( ManageData->PF->object_Typeface ) ;
		ManageData->PF->object_Typeface = NULL ;
	}

	if( ManageData->PF->object_OldTypeface != NULL )
	{
		env->DeleteGlobalRef( ManageData->PF->object_OldTypeface ) ;
		ManageData->PF->object_OldTypeface = NULL ;
	}

	if( ManageData->PF->object_Bitmap != NULL )
	{
		env->DeleteGlobalRef( ManageData->PF->object_Bitmap ) ;
		ManageData->PF->object_Bitmap = NULL ;
	}
	
	if( ManageData->PF->intArray_ClearBitmapBuffer != NULL )
	{
		env->DeleteGlobalRef( ManageData->PF->intArray_ClearBitmapBuffer ) ;
		ManageData->PF->intArray_ClearBitmapBuffer = NULL ;
	}

	if( ManageData->PF->intArray_BitmapBuffer != NULL )
	{
		env->DeleteGlobalRef( ManageData->PF->intArray_BitmapBuffer ) ;
		ManageData->PF->intArray_BitmapBuffer = NULL ;
	}

	if( ManageData->PF->ImageBuffer != NULL )
	{
		DXFREE( ManageData->PF->ImageBuffer ) ;
		ManageData->PF->ImageBuffer = NULL ;
	}

	// 終了
	return 0 ;
}

// InitFontManage の環境依存処理を行う関数
extern int InitFontManage_PF( void )
{
	// 特に何もしない
	return 0 ;
}

// TermFontManage の環境依存処理を行う関数
extern int TermFontManage_PF( void )
{
	// 特に何もしない
	return 0 ;
}

// CreateFontToHandle の環境依存処理を行う関数
extern int CreateFontToHandle_PF( CREATEFONTTOHANDLE_GPARAM *GParam, FONTMANAGE *ManageData, int DefaultCharSet )
{
	// フォントデータファイルを使用する場合は何もせずに終了
	if( ManageData->UseFontDataFile )
	{
		return 0 ;
	}

	int res = -1 ;

	ManageData->PF->ReferenceNumUp = 0 ;

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

	ManageData->PF->ReferenceNumUp = 1 ;
	
	ManageData->PF->object_Bitmap				= NULL ;
	ManageData->PF->object_Canvas				= NULL ;
	ManageData->PF->object_Paint				= NULL ;
	ManageData->PF->object_Typeface				= NULL ;
	ManageData->PF->object_OldTypeface			= NULL ;
	ManageData->PF->intArray_ClearBitmapBuffer	= NULL ;
	ManageData->PF->intArray_BitmapBuffer		= NULL ;
	ManageData->PF->ImageBuffer					= NULL ;

	// Paint の作成
	{
		jobject object_Paint = env->NewObject( JAVAANDR.class_Paint, JAVAANDR.methodID_Paint_newPaint ) ;
		if( object_Paint != NULL )
		{
			ManageData->PF->object_Paint = env->NewGlobalRef( object_Paint ) ;
			env->DeleteLocalRef( object_Paint ) ;
			object_Paint = NULL ;
		}
	}

	// Typeface の作成
	{
		char UTF16LE_FontName[ 128 * 4 ] ;
		int UTF16LE_FontNameLength ;

		UTF16LE_FontNameLength = ConvString( ( char * )ManageData->FontName, -1, WCHAR_T_CHARCODEFORMAT, UTF16LE_FontName, sizeof( UTF16LE_FontName ), DX_CHARCODEFORMAT_UTF16LE ) ;
		UTF16LE_FontNameLength = UTF16LE_FontNameLength / 2 - 1 ;
		if( UTF16LE_FontNameLength > 0 )
		{
			jstring jstring_FontName = env->NewString( ( jchar * )UTF16LE_FontName, ( jsize )UTF16LE_FontNameLength ) ;
			if( jstring_FontName != NULL )
			{
				jint style = ManageData->BaseInfo.Italic ? JAVAANDR.fieldint_Typeface_ITALIC : JAVAANDR.fieldint_Typeface_NORMAL ;

				jobject object_Typeface = env->CallStaticObjectMethod( JAVAANDR.class_Typeface, JAVAANDR.methodID_Typeface_create, jstring_FontName, style ) ;
				if( object_Typeface != NULL )
				{
					ManageData->PF->object_Typeface = env->NewGlobalRef( object_Typeface ) ;
					env->DeleteLocalRef( object_Typeface ) ;
					object_Typeface = NULL ;
				}

				env->DeleteLocalRef( jstring_FontName ) ;
				jstring_FontName = NULL ;
			}
		}
	}

	if( ManageData->PF->object_Paint  != NULL )
	{
		// フォント描画の設定
		env->CallVoidMethod( ManageData->PF->object_Paint, JAVAANDR.methodID_Paint_setTextSize,  ( jfloat   )ManageData->BaseInfo.FontSize ) ;
		env->CallVoidMethod( ManageData->PF->object_Paint, JAVAANDR.methodID_Paint_setAntiAlias, ( jboolean )JNI_TRUE ) ;
		env->CallVoidMethod( ManageData->PF->object_Paint, JAVAANDR.methodID_Paint_setARGB, 255, 255, 255, 255 ) ;
		if( ManageData->PF->object_Typeface != NULL )
		{
			jobject object_OldTypeface = env->CallObjectMethod(
				ManageData->PF->object_Paint,
				JAVAANDR.methodID_Paint_setTypeface,
				ManageData->PF->object_Typeface
			) ;
			if( object_OldTypeface != NULL )
			{
				ManageData->PF->object_OldTypeface = env->NewGlobalRef( object_OldTypeface ) ;
				env->DeleteLocalRef( object_OldTypeface ) ;
				object_OldTypeface = NULL ;
			}
		}

		// フォントの情報取得
		{
			jobject object_FontMetrics = env->CallObjectMethod( ManageData->PF->object_Paint, JAVAANDR.methodID_Paint_getFontMetrics ) ;
			if( object_FontMetrics != NULL )
			{
				ManageData->PF->PaintFontMetrics_ascent  = env->GetFloatField( object_FontMetrics, JAVAANDR.fieldID_PaintFontMetrics_ascent  ) ;
				ManageData->PF->PaintFontMetrics_bottom  = env->GetFloatField( object_FontMetrics, JAVAANDR.fieldID_PaintFontMetrics_bottom  ) ;
				ManageData->PF->PaintFontMetrics_descent = env->GetFloatField( object_FontMetrics, JAVAANDR.fieldID_PaintFontMetrics_descent ) ;
				ManageData->PF->PaintFontMetrics_leading = env->GetFloatField( object_FontMetrics, JAVAANDR.fieldID_PaintFontMetrics_leading ) ;
				ManageData->PF->PaintFontMetrics_top     = env->GetFloatField( object_FontMetrics, JAVAANDR.fieldID_PaintFontMetrics_top     ) ;

				env->DeleteLocalRef( object_FontMetrics ) ;
				object_FontMetrics = NULL ;
			}
		}

		// Bitmap の作成
		{
			ManageData->PF->BitmapSizeX = ( int )( ManageData->BaseInfo.FontSize * 3 / 2 ) ;
			ManageData->PF->BitmapSizeY = ( int )( ManageData->PF->PaintFontMetrics_bottom - ManageData->PF->PaintFontMetrics_top + 4.0f ) ;
			jobject object_Bitmap = env->CallStaticObjectMethod(
				JAVAANDR.class_Bitmap,
				JAVAANDR.methodID_Bitmap_createBitmap,
				ManageData->PF->BitmapSizeX,
				ManageData->PF->BitmapSizeY,
				JAVAANDR.fieldobject_BitmapConfig_ARGB_8888
			) ;
			if( object_Bitmap != NULL )
			{
				ManageData->PF->object_Bitmap = env->NewGlobalRef( object_Bitmap ) ;
				env->DeleteLocalRef( object_Bitmap ) ;
				object_Bitmap = NULL ;
			}
		}

		// Canvas の作成
		if( ManageData->PF->object_Bitmap != NULL )
		{
			jobject object_Canvas = env->NewObject( JAVAANDR.class_Canvas, JAVAANDR.methodID_Canvas_newCanvas, ManageData->PF->object_Bitmap ) ;
			if( object_Canvas != NULL )
			{
				ManageData->PF->object_Canvas = env->NewGlobalRef( object_Canvas ) ;
				env->DeleteLocalRef( object_Canvas ) ;
				object_Canvas = NULL ;
			}
		}

		// setPixels用のバッファを作成
		{
			jintArray intArray_ClearBitmapBuffer = env->NewIntArray( ManageData->PF->BitmapSizeX * ManageData->PF->BitmapSizeY ) ;
			if( intArray_ClearBitmapBuffer != NULL )
			{
				ManageData->PF->intArray_ClearBitmapBuffer = ( jintArray )env->NewGlobalRef( intArray_ClearBitmapBuffer ) ;
				env->DeleteLocalRef( intArray_ClearBitmapBuffer ) ;
				intArray_ClearBitmapBuffer = NULL ;

				// ゼロクリアする
				{
					jint *jint_Element = env->GetIntArrayElements( ManageData->PF->intArray_ClearBitmapBuffer, NULL ) ;
					if( jint_Element != NULL )
					{
						_MEMSET( jint_Element, 0, sizeof( jint ) * ManageData->PF->BitmapSizeX * ManageData->PF->BitmapSizeY ) ;
						env->ReleaseIntArrayElements( ManageData->PF->intArray_ClearBitmapBuffer, jint_Element, 0 ) ;
						jint_Element = NULL ;
					}
				}
			}
		}

		// getPixels用のバッファを作成
		{
			jintArray intArray_BitmapBuffer = env->NewIntArray( ManageData->PF->BitmapSizeX * ManageData->PF->BitmapSizeY ) ;
			if( intArray_BitmapBuffer != NULL )
			{
				ManageData->PF->intArray_BitmapBuffer = ( jintArray )env->NewGlobalRef( intArray_BitmapBuffer ) ;
				env->DeleteLocalRef( intArray_BitmapBuffer ) ;
				intArray_BitmapBuffer = NULL ;
			}
		}

		// 転送用のデータを格納するメモリの確保
		ManageData->PF->ImageBuffer = DXALLOC( ManageData->PF->BitmapSizeX * ManageData->PF->BitmapSizeY ) ;

		// オブジェクトの確保に成功していたら成功終了
		if( ManageData->PF->object_Bitmap              != NULL &&
			ManageData->PF->ImageBuffer                != NULL &&
			ManageData->PF->intArray_ClearBitmapBuffer != NULL &&
			ManageData->PF->intArray_BitmapBuffer      != NULL )
		{
			res = 0 ;
		}
	}

	// 失敗時は解放
	if( res == -1 )
	{
		TerminateFontHandle_Android( env, ManageData ) ;
	}
	else
	{
		// 成功時はパラメータをセット
		ManageData->BaseInfo.FontHeight    = ManageData->PF->BitmapSizeY ;
		ManageData->BaseInfo.FontAddHeight = 0 ;
		ManageData->BaseInfo.MaxWidth      = ManageData->PF->BitmapSizeX > ManageData->PF->BitmapSizeY ? ManageData->PF->BitmapSizeX : ManageData->PF->BitmapSizeY ;
		ManageData->BaseInfo.Ascent        = -ManageData->PF->PaintFontMetrics_ascent ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 戻り値を返す
	return res ;
}

// CreateFontToHandle の環境依存エラー処理を行う関数
extern int CreateFontToHandle_Error_PF( FONTMANAGE * ManageData )
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

	// 解放処理を実行
	TerminateFontHandle_Android( env, ManageData ) ;

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 終了
	return 0 ;
}

// TerminateFontHandle の環境依存処理を行う関数
extern int TerminateFontHandle_PF( FONTMANAGE *ManageData )
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

	// 解放処理を実行
	TerminateFontHandle_Android( env, ManageData ) ;

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 終了
	return 0 ;
}

// SetupFontCache の環境依存処理を行う関数
extern int SetupFontCache_PF( CREATEFONTTOHANDLE_GPARAM * /* GParam */, FONTMANAGE * /* ManageData */, int /* ASyncThread */ )
{
	// 特に何もしない
	return 0 ;
}

// FontCacheCharaAddToHandleの環境依存処理を行う関数( 実行箇所区別 0 )
extern int FontCacheCharAddToHandle_Timing0_PF( FONTMANAGE *ManageData )
{
	// とくにすること無し
	return 0 ;
}

// FontCacheCharaAddToHandleの環境依存処理を行う関数( 実行箇所区別 1 )
extern int FontCacheCharAddToHandle_Timing1_PF( FONTMANAGE *ManageData, FONTCHARDATA *CharData, DWORD CharCode, DWORD IVSCode, int TextureCacheUpdate )
{
	int	Space ;
	jchar DrawChar[ 4 ] = { 0 } ;
	jcharArray charArray_DrawChar = NULL ;
	jsize CharNum ;
	jfloat DrawWidth ;
	int res = -1 ;
	
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

	// スペースかどうかを取得しておく
	Space = CharCode == L' ' ? 1 : ( CharCode == ( DWORD )FSYS.DoubleByteSpaceCharCode ? 2 : 0 ) ;

	// 文字コードをセット
	if( CharCode > 0xffff )
	{
		DrawChar[ 0 ] = ( jchar )( CharCode & 0xffff ) ;
		DrawChar[ 1 ] = ( jchar )( CharCode >> 16 ) ;
		CharNum = 2 ;
	}
	else
	{
		DrawChar[ 0 ] = ( jchar )CharCode ;
		CharNum = 1 ;
	}

	// 描画文字の準備
	{
		charArray_DrawChar = env->NewCharArray( ( jsize )CharNum ) ;
		if( charArray_DrawChar == NULL )
		{
			goto END ;
		}

		jchar *jchar_Element = env->GetCharArrayElements( charArray_DrawChar, NULL ) ;
		if( jchar_Element == NULL )
		{
			goto END ;
		}

		jchar_Element[ 0 ] = DrawChar[ 0 ] ;
		if( CharNum == 2 )
		{
			jchar_Element[ 1 ] = DrawChar[ 1 ] ;
		}

		env->ReleaseCharArrayElements( charArray_DrawChar, jchar_Element, 0 ) ;
		jchar_Element = NULL ;
	}

	// 文字の横幅を取得
	DrawWidth = env->CallFloatMethod( ManageData->PF->object_Paint, JAVAANDR.methodID_Paint_measureText, charArray_DrawChar, 0, ( jint )CharNum ) ;

	// スペース文字だった場合
	if( Space != 0 )
	{
		FontCacheCharImageBltToHandle(
			ManageData,
			CharData,
			CharCode, 
			IVSCode,
			TRUE,
			DX_FONT_SRCIMAGETYPE_8BIT_MAX255,
			NULL,
			0,
			0,
			0,
			0,
			0,
			DrawWidth,
			FALSE
		) ;

		res = 0 ;
	}
	else
	{
		// バッファをクリア
		env->CallVoidMethod(
			ManageData->PF->object_Bitmap,
			JAVAANDR.methodID_Bitmap_setPixels,
			ManageData->PF->intArray_ClearBitmapBuffer,
			0, ManageData->PF->BitmapSizeX,
			0, 0,
			ManageData->PF->BitmapSizeX, ManageData->PF->BitmapSizeY
		) ;

		// 文字を描画
		env->CallVoidMethod(
			ManageData->PF->object_Canvas,
			JAVAANDR.methodID_Canvas_drawText,
			charArray_DrawChar,
			0, CharNum,
			0.0f,
			-ManageData->PF->PaintFontMetrics_top + 2.0f,
			ManageData->PF->object_Paint
		) ;

		// 描画結果を取得
		env->CallVoidMethod(
			ManageData->PF->object_Bitmap,
			JAVAANDR.methodID_Bitmap_getPixels,
			ManageData->PF->intArray_BitmapBuffer, 
			0, ManageData->PF->BitmapSizeX,
			0, 0,
			ManageData->PF->BitmapSizeX, ManageData->PF->BitmapSizeY
		) ;

		// 描画結果からフォントデータを転送
		{
			jint *jint_Element = env->GetIntArrayElements( ManageData->PF->intArray_BitmapBuffer, NULL ) ;
			if( jint_Element != NULL )
			{
				BYTE *DestBase ;
				jint *SrcBase ;
				int i, j ;
				int BltWidth ;

				// 転送用データを構築
				BltWidth = DrawWidth + 4 ;
				if( BltWidth > ManageData->PF->BitmapSizeX )
				{
					BltWidth = ManageData->PF->BitmapSizeX ;
				}
				DestBase = ( BYTE * )ManageData->PF->ImageBuffer ;
				SrcBase  = jint_Element ;
				for( i = 0 ; i < ManageData->PF->BitmapSizeY ; i ++, DestBase += ManageData->PF->BitmapSizeX, SrcBase += ManageData->PF->BitmapSizeX )
				{
					BYTE *Dest = DestBase ;
					jint *Src  = SrcBase ;

					for( j = 0 ; j < BltWidth ; j ++, Dest ++, Src ++ )
					{
						Dest[ 0 ] = ( BYTE )( ( Src[ 0 ] >> 24 ) & 0xff ) ;
					}
				}

				// イメージを転送
				FontCacheCharImageBltToHandle(
					ManageData,
					CharData,
					CharCode, 
					IVSCode,
					FALSE,
					DX_FONT_SRCIMAGETYPE_8BIT_MAX255,
					ManageData->PF->ImageBuffer,
					BltWidth,
					ManageData->PF->BitmapSizeY,
					ManageData->PF->BitmapSizeX,
					0,
					-2 - ( ManageData->PF->PaintFontMetrics_ascent - ManageData->PF->PaintFontMetrics_top ),
					DrawWidth,
					TextureCacheUpdate
				) ;

				env->ReleaseIntArrayElements( ManageData->PF->intArray_BitmapBuffer, jint_Element, 0 ) ;
				jint_Element = NULL ;

				res = 0 ;
			}
		}
	}

END :

	if( charArray_DrawChar != NULL )
	{
		env->DeleteLocalRef( charArray_DrawChar ) ;
		charArray_DrawChar = NULL ;
	}

	// JavaVM とこのスレッドの関連付け終了
	g_AndroidSys.NativeActivity->vm->DetachCurrentThread() ;

	pthread_mutex_unlock( &g_AndroidSys.NativeActivityMutex ) ;

	// 戻り値を返す
	return res ;
}

// FontCacheCharaAddToHandleの環境依存処理を行う関数( 実行箇所区別 2 )
extern int FontCacheCharAddToHandle_Timing2_PF( FONTMANAGE *ManageData )
{
	// 特に何もしない
	return 0 ;
}

// EnumFontName の環境依存処理を行う関数
extern int EnumFontName_PF( ENUMFONTDATA *EnumFontData, int IsEx, int CharSet )
{
	// 正常終了
	return 0 ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


#endif // DX_NON_FONT


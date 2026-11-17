// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用Java関係ヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_JAVAANDROID_H
#define DX_JAVAANDROID_H

// インクルード ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include <android/configuration.h>
#include <android/native_activity.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define JAVAANDR		g_JavaAndroidInfo	

// 構造体定義 --------------------------------------------------------------------

struct DXLIB_JAVA_ANDROID_INFO
{
	int						InitializeFlag ;

	jclass					class_Build_VERSION ;
	jclass					class_Bitmap ;
	jclass					class_BitmapConfig ;
	jclass					class_Canvas ;
	jclass					class_Typeface ;
	jclass					class_Rect ;
	jclass					class_Paint ;
	jclass					class_PaintFontMetrics ;
	jclass					class_Color ;
	jclass					class_BaseBundle ;
	jclass					class_Bundle ;
	jclass					class_Debug_MemoryInfo ;
	jclass					class_BatteryManager ;
	jclass					class_Environment ;
	jclass					class_Vibrator ;
	jclass					class_VibrationEffect ;
	jclass					class_Process ;
	jclass					class_TextView ;
	jclass					class_TextView_BufferType ;
	jclass					class_View ;
	jclass					class_DisplayCutout ;
	jclass					class_Window ;
	jclass					class_WindowInsets ;
	jclass					class_EditText ;
	jclass					class_Integer ;
	jclass					class_String ;
	jclass					class_Runtime ;
	jclass					class_List ;
	jclass					class_Locale ;
	jclass					class_Calendar ;
	jclass					class_URLConnection ;
	jclass					class_HttpURLConnection ;
	jclass					class_URL ;
	jclass					class_InputStream ;
	jclass					class_File ;
	jclass					class_CharSequence ;
	jclass					class_ActivityManager ;
	jclass					class_ActivityManager_MemoryInfo ;
	jclass					class_Dialog ;
	jclass					class_AlertDialog ;
	jclass					class_AlertDialog_Builder ;
	jclass					class_Notification ;
	jclass					class_NotificationManager ;
	jclass					class_Notification_Builder ;
	jclass					class_PendingIntent ;
	jclass					class_Intent ;
	jclass					class_IntentFilter ;
	jclass					class_BroadcastReceiver ;
	jclass					class_ClipboardManager ;
	jclass					class_ClipData ;
	jclass					class_Context ;
	jclass					class_Resources ;
	jclass					class_ApplicationInfo ;
	jclass					class_PackageInfo ;
	jclass					class_PackageItemInfo ;
	jclass					class_PackageManager ;
	jclass					class_SensorManager ;
	jclass					class_R_mipmap ;
	jclass					class_Uri ;
	jclass					class_WifiInfo ;
	jclass					class_WifiManager ;
	jclass					class_Activity ;
	jclass					class_AudioManager ;


	jmethodID				methodID_Bitmap_createBitmap ;
	jmethodID				methodID_Bitmap_setPixels ;
	jmethodID				methodID_Bitmap_getPixels ;
	jmethodID				methodID_Canvas_newCanvas ;
	jmethodID				methodID_Canvas_drawText ;
	jmethodID				methodID_Canvas_drawARGB ;
	jmethodID				methodID_Typeface_create ;
	jmethodID				methodID_Paint_newPaint ;
	jmethodID				methodID_Paint_getFontMetrics ;
	jmethodID				methodID_Paint_setTextSize ;
	jmethodID				methodID_Paint_setTypeface ;
	jmethodID				methodID_Paint_setAntiAlias ;
	jmethodID				methodID_Paint_setARGB ;
	jmethodID				methodID_Paint_measureText ;

	jmethodID				methodID_EditText_newEditText ;
	jmethodID				methodID_EditText_setText ;
	jmethodID				methodID_TextView_getText ;
	jmethodID				methodID_View_getRootWindowInsets ;
	jmethodID				methodID_DisplayCutout_getBoundingRects ;
	jmethodID				methodID_Window_getCurrentFocus ;
	jmethodID				methodID_Window_getDecorView ;
	jmethodID				methodID_WindowInsets_getDisplayCutout ;
	jmethodID				methodID_Environment_isExternalStorageRemovable ;
	jmethodID				methodID_Vibrator_hasVibrator ;
	jmethodID				methodID_Vibrator_hasAmplitudeControl ;
	jmethodID				methodID_Vibrator_vibrate_long ;
	jmethodID				methodID_Vibrator_vibrate_longArray_int ;
	jmethodID				methodID_Vibrator_vibrate_vibEffect ;
	jmethodID				methodID_Vibrator_cancel ;
	jmethodID				methodID_VibrationEffect_createOneShot ;
	jmethodID				methodID_VibrationEffect_createWaveform_withTimings ;
	jmethodID				methodID_VibrationEffect_createWaveform_withAmplitudes ;
	jmethodID				methodID_Process_myPid ;
	jmethodID				methodID_Process_killProcess ;
	jmethodID				methodID_InputStream_read_0param ;
	jmethodID				methodID_InputStream_read_1param ;
	jmethodID				methodID_InputStream_close ;
	jmethodID				methodID_File_getAbsolutePath ;
	jmethodID				methodID_CharSequence_toString ;
	jmethodID				methodID_Integer_parseInt ;
	jmethodID				methodID_String_subSequence ;
	jmethodID				methodID_Runtime_getRuntime ;
	jmethodID				methodID_Runtime_freeMemory ;
	jmethodID				methodID_Runtime_totalMemory ;
	jmethodID				methodID_Runtime_maxMemory ;
	jmethodID				methodID_List_size ;
	jmethodID				methodID_List_get ;
	jmethodID				methodID_Locale_getDefault ;
	jmethodID				methodID_Locale_getLanguage ;
	jmethodID				methodID_Locale_getCountry ;
	jmethodID				methodID_Calendar_getInstance ;
	jmethodID				methodID_Calendar_get ;
	jmethodID				methodID_URLConnection_setDoInput ;
	jmethodID				methodID_URLConnection_setDoOutput ;
	jmethodID				methodID_URLConnection_connect ;
	jmethodID				methodID_URLConnection_getInputStream ;
	jmethodID				methodID_HttpURLConnection_newHttpURLConnection ;
	jmethodID				methodID_HttpURLConnection_setRequestMethod ;
	jmethodID				methodID_HttpURLConnection_setInstanceFollowRedirects ;
	jmethodID				methodID_HttpURLConnection_getResponseCode ;
	jmethodID				methodID_URL_newURL ;
	jmethodID				methodID_URL_openConnection ;
	jmethodID				methodID_ActivityManager_getMemoryInfo ;
	jmethodID				methodID_ActivityManager_getProcessMemoryInfo ;
	jmethodID				methodID_ActivityManager_MemoryInfo_newActivityManager_MemoryInfo ;
	jmethodID				methodID_Dialog_isShowing ;
	jmethodID				methodID_AlertDialog_Builder_newAlertDialog_Builder ;
	jmethodID				methodID_AlertDialog_Builder_setIcon ;
	jmethodID				methodID_AlertDialog_Builder_setTitle ;
	jmethodID				methodID_AlertDialog_Builder_setView ;
	jmethodID				methodID_AlertDialog_Builder_setPositiveButton ;
	jmethodID				methodID_AlertDialog_Builder_setNegativeButton ;
	jmethodID				methodID_AlertDialog_Builder_show ;
	jmethodID				methodID_Notification_newNotification ;
	jmethodID				methodID_Notification_setLatestEventInfo ;
	jmethodID				methodID_NotificationManager_notify ;
	jmethodID				methodID_NotificationManager_cancel ;
	jmethodID				methodID_NotificationManager_cancelAll ;
	jmethodID				methodID_Notification_Builder_newNotification_Builder ;
	jmethodID				methodID_Notification_Builder_setWhen ;
	jmethodID				methodID_Notification_Builder_setShowWhen ;
	jmethodID				methodID_Notification_Builder_setUsesChronometer ;
	jmethodID				methodID_Notification_Builder_setSmallIcon ;
	jmethodID				methodID_Notification_Builder_setContentTitle ;
	jmethodID				methodID_Notification_Builder_setContentText ;
	jmethodID				methodID_Notification_Builder_setSubText ;
	jmethodID				methodID_Notification_Builder_setNumber ;
	jmethodID				methodID_Notification_Builder_setContentInfo ;
	jmethodID				methodID_Notification_Builder_setProgress ;
	jmethodID				methodID_Notification_Builder_setContent ;
	jmethodID				methodID_Notification_Builder_setContentIntent ;
	jmethodID				methodID_Notification_Builder_setDeleteIntent ;
	jmethodID				methodID_Notification_Builder_setFullScreenIntent ;
	jmethodID				methodID_Notification_Builder_setTicker ;
	jmethodID				methodID_Notification_Builder_setLargeIcon ;
	jmethodID				methodID_Notification_Builder_setSound ;
	jmethodID				methodID_Notification_Builder_setVibrate ;
	jmethodID				methodID_Notification_Builder_setLights ;
	jmethodID				methodID_Notification_Builder_setOngoing ;
	jmethodID				methodID_Notification_Builder_setOnlyAlertOnce ;
	jmethodID				methodID_Notification_Builder_setAutoCancel ;
	jmethodID				methodID_Notification_Builder_setLocalOnly ;
	jmethodID				methodID_Notification_Builder_setDefaults ;
	jmethodID				methodID_Notification_Builder_setPriority ;
	jmethodID				methodID_Notification_Builder_setCategory ;
	jmethodID				methodID_Notification_Builder_addPerson ;
	jmethodID				methodID_Notification_Builder_setGroup ;
	jmethodID				methodID_Notification_Builder_setGroupSummary ;
	jmethodID				methodID_Notification_Builder_setSortKey ;
	jmethodID				methodID_Notification_Builder_addExtras ;
	jmethodID				methodID_Notification_Builder_setExtras ;
	jmethodID				methodID_Notification_Builder_getExtras ;
	jmethodID				methodID_Notification_Builder_addAction ;
	jmethodID				methodID_Notification_Builder_setStyle ;
	jmethodID				methodID_Notification_Builder_setVisibility ;
	jmethodID				methodID_Notification_Builder_setPublicVersion ;
	jmethodID				methodID_Notification_Builder_extend ;
	jmethodID				methodID_Notification_Builder_setColor ;
	jmethodID				methodID_Notification_Builder_getNotification ;
	jmethodID				methodID_Notification_Builder_build ;
	jmethodID				methodID_PendingIntent_getActivity ;
	jmethodID				methodID_PendingIntent_getActivities ;
	jmethodID				methodID_Intent_newIntent ;
	jmethodID				methodID_Intent_newIntent_NoParam ;
	jmethodID				methodID_Intent_newIntent_Uri ;
	jmethodID				methodID_Intent_createChooser ;
	jmethodID				methodID_Intent_setFlags ;
	jmethodID				methodID_Intent_setClassName ;
	jmethodID				methodID_Intent_getIntExtra ;
	jmethodID				methodID_Intent_setAction ;
	jmethodID				methodID_Intent_setData ;
	jmethodID				methodID_Intent_setType ;
	jmethodID				methodID_Intent_addCategory ;
	jmethodID				methodID_Intent_putExtra_boolean ;
	jmethodID				methodID_Intent_putExtra_byte ;
	jmethodID				methodID_Intent_putExtra_char ;
	jmethodID				methodID_Intent_putExtra_short ;
	jmethodID				methodID_Intent_putExtra_int ;
	jmethodID				methodID_Intent_putExtra_long ;
	jmethodID				methodID_Intent_putExtra_float ;
	jmethodID				methodID_Intent_putExtra_double ;
	jmethodID				methodID_Intent_putExtra_string ;
	jmethodID				methodID_Intent_putExtra_stringArray ;
	jmethodID				methodID_IntentFilter_newIntentFilter ;
	jmethodID				methodID_ClipboardManager_setPrimaryClip ;
	jmethodID				methodID_ClipboardManager_getPrimaryClip ;
	jmethodID				methodID_ClipData_newPlainText ;
	jmethodID				methodID_ClipData_toString ;
	jmethodID				methodID_BaseBundle_getString ;
	jmethodID				methodID_BaseBundle_getBoolean ;
	jmethodID				methodID_Bundle_getString ;
	jmethodID				methodID_Bundle_getBoolean ;
	jmethodID				methodID_Debug_MemoryInfo_getTotalPss ;
	jmethodID				methodID_Context_getResources ;
	jmethodID				methodID_Context_getPackageManager ;
	jmethodID				methodID_Context_getPackageName ;
	jmethodID				methodID_Context_getApplicationContext ;
	jmethodID				methodID_Context_getExternalFilesDirs ;
	jmethodID				methodID_Context_getSystemService ;
	jmethodID				methodID_Context_registerReceiver ;
	jmethodID				methodID_Resources_getString ;
	jmethodID				methodID_Resources_getResourceName ;
	jmethodID				methodID_Resources_getIdentifier ;
	jmethodID				methodID_PackageManager_getApplicationInfo ;
	jmethodID				methodID_PackageManager_hasSystemFeature ;
	jmethodID				methodID_SensorManager_getRotationMatrix ;
	jmethodID				methodID_SensorManager_remapCoordinateSystem ;
	jmethodID				methodID_SensorManager_getOrientation ;
	jmethodID				methodID_Uri_parse ;
	jmethodID				methodID_WifiInfo_getSSID ;
	jmethodID				methodID_WifiInfo_getBSSID ;
	jmethodID				methodID_WifiInfo_getRssi ;
	jmethodID				methodID_WifiInfo_getLinkSpeed ;
	jmethodID				methodID_WifiInfo_getFrequency ;
	jmethodID				methodID_WifiInfo_getMacAddress ;
	jmethodID				methodID_WifiInfo_getNetworkId ;
	jmethodID				methodID_WifiInfo_getIpAddress ;
	jmethodID				methodID_WifiInfo_getHiddenSSID ;
	jmethodID				methodID_WifiInfo_toString ;
	jmethodID				methodID_WifiInfo_describeContents ;
	jmethodID				methodID_WifiManager_calculateSignalLevel ;
	jmethodID				methodID_WifiManager_getConnectionInfo ;
	jmethodID				methodID_Activity_startActivity ;
	jmethodID				methodID_Activity_finish ;
	jmethodID				methodID_Activity_finishAndRemoveTask ;
	jmethodID				methodID_Activity_releaseInstance ;
	jmethodID				methodID_Activity_getCurrentFocus ;
	jmethodID				methodID_Activity_getWindow ;
	jmethodID				methodID_AudioManager_getProperty ;

	jfieldID				fieldID_Build_VERSION_SDK_INT ;
	jfieldID				fieldID_BitmapConfig_ARGB_8888 ;
	jfieldID				fieldID_PaintFontMetrics_ascent ;
	jfieldID				fieldID_PaintFontMetrics_bottom ;
	jfieldID				fieldID_PaintFontMetrics_descent ;
	jfieldID				fieldID_PaintFontMetrics_leading ;
	jfieldID				fieldID_PaintFontMetrics_top ;
	jfieldID				fieldID_Typeface_NORMAL ;
	jfieldID				fieldID_Typeface_BOLD ;
	jfieldID				fieldID_Typeface_ITALIC ;
	jfieldID				fieldID_Typeface_BOLD_ITALIC ;
	jfieldID				fieldID_Color_BLACK ;
	jfieldID				fieldID_Color_BLUE ;
	jfieldID				fieldID_Color_CYAN ;
	jfieldID				fieldID_Color_DKGRAY ;
	jfieldID				fieldID_Color_GRAY ;
	jfieldID				fieldID_Color_GREEN ;
	jfieldID				fieldID_Color_LTGRAY ;
	jfieldID				fieldID_Color_MAGENTA ;
	jfieldID				fieldID_Color_RED ;
	jfieldID				fieldID_Color_TRANSPARENT ;
	jfieldID				fieldID_Color_WHITE ;
	jfieldID				fieldID_Color_YELLOW ;
	jfieldID				fieldID_Rect_bottom ;
	jfieldID				fieldID_Rect_left ;
	jfieldID				fieldID_Rect_right ;
	jfieldID				fieldID_Rect_top ;

	jfieldID				fieldID_BatteryManager_EXTRA_LEVEL ;
	jfieldID				fieldID_BatteryManager_EXTRA_SCALE ;

	jfieldID				fieldID_TextView_BufferType_EDITABLE ;
	jfieldID				fieldID_TextView_BufferType_NORMAL ;
	jfieldID				fieldID_TextView_BufferType_SPANNABLE ;

	jfieldID				fieldID_ActivityManager_MemoryInfo_availMem ;
	jfieldID				fieldID_ActivityManager_MemoryInfo_lowMemory ;
	jfieldID				fieldID_ActivityManager_MemoryInfo_threshold ;
	jfieldID				fieldID_ActivityManager_MemoryInfo_totalMem ;

	jfieldID				fieldID_Notification_DEFAULT_LIGHTS ;
	jfieldID				fieldID_Notification_DEFAULT_VIBRATE ;
	jfieldID				fieldID_Notification_FLAG_SHOW_LIGHTS ;
	jfieldID				fieldID_Notification_icon ;
	jfieldID				fieldID_Notification_tickerText ;
	jfieldID				fieldID_Notification_defaults ;
	jfieldID				fieldID_Notification_vibrate ;
	jfieldID				fieldID_Notification_ledARGB ;
	jfieldID				fieldID_Notification_ledOffMS ;
	jfieldID				fieldID_Notification_ledOnMS ;
	jfieldID				fieldID_Notification_flags ;
	jfieldID				fieldID_PendingIntent_FLAG_CANCEL_CURRENT ;
	jfieldID				fieldID_PendingIntent_FLAG_NO_CREATE ;
	jfieldID				fieldID_PendingIntent_FLAG_ONE_SHOT ;
	jfieldID				fieldID_PendingIntent_FLAG_UPDATE_CURRENT ;
	jfieldID				fieldID_Intent_ACTION_BATTERY_CHANGED ;
	jfieldID				fieldID_Intent_ACTION_OPEN_DOCUMENT ;
	jfieldID				fieldID_Intent_ACTION_SEND ;
	jfieldID				fieldID_Intent_ACTION_SENDTO ;
	jfieldID				fieldID_Intent_ACTION_VIEW ;
	jfieldID				fieldID_Intent_EXTRA_EMAIL ;
	jfieldID				fieldID_Intent_EXTRA_STREAM ;
	jfieldID				fieldID_Intent_EXTRA_SUBJECT ;
	jfieldID				fieldID_Intent_EXTRA_TEXT ;
	jfieldID				fieldID_Intent_EXTRA_CC ;
	jfieldID				fieldID_Intent_EXTRA_BCC ;
	jfieldID				fieldID_Intent_CATEGORY_OPENABLE ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_NEW_TASK ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_SINGLE_TOP ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_CLEAR_TOP ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_CLEAR_TASK ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_NO_ANIMATION ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_NO_HISTORY ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_PREVIOUS_IS_TOP ;
	jfieldID				fieldID_Intent_FLAG_ACTIVITY_REORDER_TO_FRONT ;      
	jfieldID				fieldID_Context_NOTIFICATION_SERVICE ;
	jfieldID				fieldID_Context_AUDIO_SERVICE ;
	jfieldID				fieldID_Context_ACTIVITY_SERVICE ;
	jfieldID				fieldID_Context_VIBRATOR_SERVICE ;
	jfieldID				fieldID_Context_SENSOR_SERVICE ;
	jfieldID				fieldID_Context_WIFI_SERVICE ;
	jfieldID				fieldID_Context_BLUETOOTH_SERVICE ;
	jfieldID				fieldID_Context_CAMERA_SERVICE ;
	jfieldID				fieldID_Context_CAPTIONING_SERVICE ;
	jfieldID				fieldID_Context_CLIPBOARD_SERVICE ;
	jfieldID				fieldID_Context_CONNECTIVITY_SERVICE ;
	jfieldID				fieldID_Context_CONSUMER_IR_SERVICE ;
	jfieldID				fieldID_PackageItemInfo_metaData ;
	jfieldID				fieldID_PackageManager_GET_META_DATA ;
	jfieldID				fieldID_SensorManager_AXIS_MINUS_X ;
	jfieldID				fieldID_SensorManager_AXIS_MINUS_Y ;
	jfieldID				fieldID_SensorManager_AXIS_MINUS_Z ;
	jfieldID				fieldID_SensorManager_AXIS_X ;
	jfieldID				fieldID_SensorManager_AXIS_Y ;
	jfieldID				fieldID_SensorManager_AXIS_Z ;
	jfieldID				fieldID_VibrationEffect_DEFAULT_AMPLITUDE ;
	jfieldID				fieldID_R_mipmap_sym_def_app_icon ;
	jfieldID				fieldID_Calendar_DAY_OF_WEEK ;
	jfieldID				fieldID_Calendar_WEEK_OF_MONTH ;
	jfieldID				fieldID_Calendar_DAY_OF_WEEK_IN_MONTH ;
	jfieldID				fieldID_HttpURLConnection_HTTP_OK ;
	jfieldID				fieldID_HttpURLConnection_HTTP_NO_CONTENT ;
	jfieldID				fieldID_HttpURLConnection_HTTP_NOT_FOUND ;
	jfieldID				fieldID_HttpURLConnection_HTTP_BAD_METHOD ;
	jfieldID				fieldID_HttpURLConnection_HTTP_NOT_ACCEPTABLE ;
	jfieldID				fieldID_HttpURLConnection_HTTP_BAD_GATEWAY ;
	jfieldID				fieldID_HttpURLConnection_HTTP_NOT_IMPLEMENTED ;
	jfieldID				fieldID_HttpURLConnection_HTTP_SERVER_ERROR ;
	jfieldID				fieldID_HttpURLConnection_HTTP_GATEWAY_TIMEOUT ;
	jfieldID				fieldID_HttpURLConnection_HTTP_UNAVAILABLE ;
	jfieldID				fieldID_HttpURLConnection_HTTP_UNSUPPORTED_TYPE ;
	jfieldID				fieldID_HttpURLConnection_HTTP_PRECON_FAILED ;
	jfieldID				fieldID_HttpURLConnection_HTTP_CONFLICT ;
	jfieldID				fieldID_HttpURLConnection_HTTP_UNAUTHORIZED ;
	jfieldID				fieldID_HttpURLConnection_HTTP_BAD_REQUEST ;
	jfieldID				fieldID_AudioManager_PROPERTY_OUTPUT_FRAMES_PER_BUFFER ;
	jfieldID				fieldID_AudioManager_PROPERTY_OUTPUT_SAMPLE_RATE ;


	jint					fieldint_Build_VERSION_SDK_INT ;
	jobject					fieldobject_BitmapConfig_ARGB_8888 ;
	jint					fieldint_Typeface_NORMAL ;
	jint					fieldint_Typeface_BOLD ;
	jint					fieldint_Typeface_ITALIC ;
	jint					fieldint_Typeface_BOLD_ITALIC ;
	jint					fieldint_Color_BLACK ;
	jint					fieldint_Color_BLUE ;
	jint					fieldint_Color_CYAN ;
	jint					fieldint_Color_DKGRAY ;
	jint					fieldint_Color_GRAY ;
	jint					fieldint_Color_GREEN ;
	jint					fieldint_Color_LTGRAY ;
	jint					fieldint_Color_MAGENTA ;
	jint					fieldint_Color_RED ;
	jint					fieldint_Color_TRANSPARENT ;
	jint					fieldint_Color_WHITE ;
	jint					fieldint_Color_YELLOW ;

	jstring					fieldstring_BatteryManager_EXTRA_LEVEL ;
	jstring					fieldstring_BatteryManager_EXTRA_SCALE ;

	jobject					fieldobject_TextView_BufferType_EDITABLE ;
	jobject					fieldobject_TextView_BufferType_NORMAL ;
	jobject					fieldobject_TextView_BufferType_SPANNABLE ;

	jint					fieldint_Notification_DEFAULT_LIGHTS ;
	jint					fieldint_Notification_DEFAULT_VIBRATE ;
	jint					fieldint_Notification_FLAG_SHOW_LIGHTS ;
	jint					fieldint_PendingIntent_FLAG_CANCEL_CURRENT ;
	jint					fieldint_PendingIntent_FLAG_NO_CREATE ;
	jint					fieldint_PendingIntent_FLAG_ONE_SHOT ;
	jint					fieldint_PendingIntent_FLAG_UPDATE_CURRENT ;
	jstring					fieldstring_Intent_ACTION_BATTERY_CHANGED ;
	jstring					fieldstring_Intent_ACTION_OPEN_DOCUMENT ;
	jstring					fieldstring_Intent_ACTION_SEND ;
	jstring					fieldstring_Intent_ACTION_SENDTO ;
	jstring					fieldstring_Intent_ACTION_VIEW ;
	jstring					fieldstring_Intent_EXTRA_EMAIL ;
	jstring					fieldstring_Intent_EXTRA_STREAM ;
	jstring					fieldstring_Intent_EXTRA_SUBJECT ;
	jstring					fieldstring_Intent_EXTRA_TEXT ;
	jstring					fieldstring_Intent_EXTRA_CC ;
	jstring					fieldstring_Intent_EXTRA_BCC ;
	jstring					fieldstring_Intent_CATEGORY_OPENABLE ;
	jint					fieldint_Intent_FLAG_ACTIVITY_NEW_TASK ;
	jint					fieldint_Intent_FLAG_ACTIVITY_SINGLE_TOP ;
	jint					fieldint_Intent_FLAG_ACTIVITY_CLEAR_TOP ;
	jint					fieldint_Intent_FLAG_ACTIVITY_CLEAR_TASK ;
	jint					fieldint_Intent_FLAG_ACTIVITY_NO_ANIMATION ;
	jint					fieldint_Intent_FLAG_ACTIVITY_NO_HISTORY ;
	jint					fieldint_Intent_FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY ;
	jint					fieldint_Intent_FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS ;
	jint					fieldint_Intent_FLAG_ACTIVITY_PREVIOUS_IS_TOP ;
	jint					fieldint_Intent_FLAG_ACTIVITY_REORDER_TO_FRONT ;
	jobject					fieldobject_Context_NOTIFICATION_SERVICE ;
	jobject					fieldobject_Context_AUDIO_SERVICE ;
	jobject					fieldobject_Context_ACTIVITY_SERVICE ;
	jobject					fieldobject_Context_VIBRATOR_SERVICE ;
	jobject					fieldobject_Context_SENSOR_SERVICE ;
	jobject					fieldobject_Context_WIFI_SERVICE ;
	jobject					fieldobject_Context_BLUETOOTH_SERVICE ;
	jobject					fieldobject_Context_CAMERA_SERVICE ;
	jobject					fieldobject_Context_CAPTIONING_SERVICE ;
	jobject					fieldobject_Context_CLIPBOARD_SERVICE ;
	jobject					fieldobject_Context_CONNECTIVITY_SERVICE ;
	jobject					fieldobject_Context_CONSUMER_IR_SERVICE ;
	jint					fieldint_PackageManager_GET_META_DATA ;
	jint					fieldint_SensorManager_AXIS_MINUS_X ;
	jint					fieldint_SensorManager_AXIS_MINUS_Y ;
	jint					fieldint_SensorManager_AXIS_MINUS_Z ;
	jint					fieldint_SensorManager_AXIS_X ;
	jint					fieldint_SensorManager_AXIS_Y ;
	jint					fieldint_SensorManager_AXIS_Z ;
	jint					fieldint_VibrationEffect_DEFAULT_AMPLITUDE ;
	jint					fieldint_R_mipmap_sym_def_app_icon ;
	jint					fieldint_Calendar_DAY_OF_WEEK ;
	jint					fieldint_Calendar_WEEK_OF_MONTH ;
	jint					fieldint_Calendar_DAY_OF_WEEK_IN_MONTH ;
	jint					fieldint_HttpURLConnection_HTTP_OK ;
	jint					fieldint_HttpURLConnection_HTTP_NO_CONTENT ;
	jint					fieldint_HttpURLConnection_HTTP_NOT_FOUND ;
	jint					fieldint_HttpURLConnection_HTTP_BAD_METHOD ;
	jint					fieldint_HttpURLConnection_HTTP_NOT_ACCEPTABLE ;
	jint					fieldint_HttpURLConnection_HTTP_BAD_GATEWAY ;
	jint					fieldint_HttpURLConnection_HTTP_NOT_IMPLEMENTED ;
	jint					fieldint_HttpURLConnection_HTTP_SERVER_ERROR ;
	jint					fieldint_HttpURLConnection_HTTP_GATEWAY_TIMEOUT ;
	jint					fieldint_HttpURLConnection_HTTP_UNAVAILABLE ;
	jint					fieldint_HttpURLConnection_HTTP_UNSUPPORTED_TYPE ;
	jint					fieldint_HttpURLConnection_HTTP_PRECON_FAILED ;
	jint					fieldint_HttpURLConnection_HTTP_CONFLICT ;
	jint					fieldint_HttpURLConnection_HTTP_UNAUTHORIZED ;
	jint					fieldint_HttpURLConnection_HTTP_BAD_REQUEST ;      
	jstring					fieldstring_AudioManager_PROPERTY_OUTPUT_FRAMES_PER_BUFFER ;
	jstring					fieldstring_AudioManager_PROPERTY_OUTPUT_SAMPLE_RATE ;
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

extern DXLIB_JAVA_ANDROID_INFO g_JavaAndroidInfo ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// Java のクラスやメソッドの参照を取得する
extern int SetupJavaAndroidInfo( JNIEnv *env ) ;

// Java のクラスやメソッドの参照の後始末をする
extern int TerminateJavaAndroidInfo( JNIEnv *env ) ;

// Java のクラスのグローバル参照を取得する
extern jclass Java_FindClass_Global( JNIEnv *env, const char *name ) ;

// Java のスタティックオブジェクトフィールドのグローバル参照を取得する
extern jobject Java_GetStaticObjectField_Global( JNIEnv *env, jclass clazz, jfieldID fieldID ) ;

// Java のスタティック文字列フィールドのグローバル参照を取得する
extern jstring Java_GetStaticStringField_Global( JNIEnv *env, jclass clazz, jfieldID fieldID ) ;

// TCHAR の文字列から jstring を作成する( Local Ref )
extern jstring Java_Create_jstring_From_TCHAR( JNIEnv *env, const TCHAR *tchar_string ) ;

// wchar_t の文字列から jstring を作成する( Local Ref )
extern jstring Java_Create_jstring_From_wchar_t( JNIEnv *env, const wchar_t *wchar_t_string ) ;

// wchar_t の文字列から CharSequence を作成する( Local Ref )
extern jobject Java_Create_CharSequence_From_wchar_t( JNIEnv *env, const wchar_t *wchar_t_string ) ;

// jstring から wchar_t の文字列を作成する( DXFREE で解放する )
extern int Java_Create_wchar_t_string_From_jstring( JNIEnv *env, jstring _jstring, wchar_t **wchar_t_stringP ) ;

// jstring から TCHAR の文字列を作成する( DXFREE で解放する )
extern int Java_Create_TCHAR_string_From_jstring( JNIEnv *env, jstring _jstring, TCHAR **tchar_stringP ) ;

// jstring から char の文字列を作成する( DXFREE で解放する )
extern int Java_Create_char_string_From_jstring( JNIEnv *env, jstring _jstring, char **char_stringP ) ;

// VECTOR から長さ 3 の float Array を作成する( Local Ref )
extern jfloatArray Java_Create_floatArray_From_VECTOR( JNIEnv *env, const struct tagVECTOR *Vector ) ;

// 長さ 3 以上の float Array から VECTOR の値を取得する( 0:正常終了  -1:エラー )
extern int Java_Get_VECTOR_From_floatArray( JNIEnv *env, jfloatArray floatArray, struct tagVECTOR *Buffer ) ;

// intの配列から jlongArray を作成する
extern jlongArray Java_Create_longArray_From_intArray( JNIEnv *env, const int *intArray, int intArrayLength ) ;

// intの配列から jintArray を作成する
extern jintArray Java_Create_intArray_From_intArray( JNIEnv *env, const int *intArray, int intArrayLength ) ;

// floatの配列から jfloatArray を作成する
extern jfloatArray Java_Create_floatArray_From_floatArray( JNIEnv *env, const int *floatArray, int floatArrayLength ) ;

// doubleの配列から jdoubleArray を作成する
extern jdoubleArray Java_Create_doubleArray_From_doubleArray( JNIEnv *env, const int *doubleArray, int doubleArrayLength ) ;

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_JAVAANDROID_H

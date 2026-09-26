// ----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Live2D Cubism4 関係プログラム
// 
// 				Ver 3.25a
// 
// ----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "DxCompileConfig.h"

#ifndef DX_NON_LIVE2D_CUBISM4

// インクルード----------------------------------------------------------------
#include "DxMemory.h"
#include "DxHandle.h"
#include "DxLive2DCubism4.h"
#include "DxSystem.h"
#include "DxLog.h"
#include "DxASyncLoad.h"
#include "DxUseCLibLive2DCubism4.h"

// ----------------------------------------------------------------------------

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// Live2D Cubism4 モデルハンドルの有効性チェック
#define LIVE2DCUBISM4MODELCHK( HAND, MPOINT )		HANDLECHK(       DX_HANDLETYPE_LIVE2D_CUBISM4_MODEL, HAND, *( ( HANDLEINFO ** )&MPOINT ) )
#define LIVE2DCUBISM4MODELCHK_ASYNC( HAND, MPOINT )	HANDLECHK_ASYNC( DX_HANDLETYPE_LIVE2D_CUBISM4_MODEL, HAND, *( ( HANDLEINFO ** )&MPOINT ) )

// 構造体宣言------------------------------------------------------------------

// 関数宣言--------------------------------------------------------------------

// ハンドル初期化・後始末
static	int			Live2DCubism4_Model_InitializeHandle( HANDLEINFO *HandleInfo ) ;						// Live2D Cubism 4 モデルハンドルの初期化
static	int			Live2DCubism4_Model_TerminateHandle( HANDLEINFO *HandleInfo ) ;							// Live2D Cubism 4 モデルハンドルの後始末

// データ宣言------------------------------------------------------------------

// Live2D Cubism4 システムデータ
LIVE2DCUBISM4SYSTEMDATA Live2DCubism4SysData ;

// Live2D Cubism 4 DLL
LIVE2DCUBISM4DLL Live2DCubism4DLLData ;

// プログラムコード------------------------------------------------------------

// Live2D Cubism4 関連の初期化と後始末

// Live2D Cubism4 の初期化
extern int Live2DCubism4_Initialize( void )
{
	// 既に初期化済みの場合は何もしない
	if( LIVE2DSYS.InitializeFlag == TRUE ) return 0 ;

	// Live2D Cubism4 Model ハンドル管理情報の初期化
	InitializeHandleManage( DX_HANDLETYPE_LIVE2D_CUBISM4_MODEL, sizeof( LIVE2DCUBISM4MODEL ), MAX_LIVE2D_CUBISM4_MODEL_NUM, Live2DCubism4_Model_InitializeHandle, Live2DCubism4_Model_TerminateHandle, NULL, L"Live2DModel" ) ;

	//---- static 初期化 ----
	D_JsonValue::StaticInitializeNotForClientCall() ;

	// IDManager を初期化
	LIVE2DSYS.s_cubismIdManager = new_D_CubismIdManager() ;

	// 環境依存の初期化
	if( Live2DCubism4_Initialize_PF() < 0 )
	{
		goto ERR ;
	}

	// 初期化済みフラグを立てる
	LIVE2DSYS.InitializeFlag = TRUE ;

	// 終了
	return 0 ;

	// エラー処理
ERR:
	Live2DCubism4_Terminate() ;

	return -1;
}

// Live2D Cubism4 の後始末
extern int Live2DCubism4_Terminate( void )
{
	// Live2D Cubism4 Model ハンドルを初期化
	AllHandleSub( DX_HANDLETYPE_LIVE2D_CUBISM4_MODEL ) ;

	// Live2D Cubism4 Model ハンドル管理情報の後始末
	TerminateHandleManage( DX_HANDLETYPE_LIVE2D_CUBISM4_MODEL ) ;

	// 環境依存の後始末
	if( Live2DCubism4_Terminate_PF() < 0 )
	{
		return -1 ;
	}

	//---- static 後始末 ----
	D_JsonValue::StaticReleaseNotForClientCall() ;

	// IDManager を削除
    delete_D_CubismIdManager( LIVE2DSYS.s_cubismIdManager ) ;
	LIVE2DSYS.s_cubismIdManager = NULL ;

	//レンダラの静的リソース（シェーダプログラム他）を解放する
	D_CubismRenderer::StaticRelease() ;

	// シェーダーオブジェクトファイルＤＸＡの後始末
	DXA_Terminate( &LIVE2DSYS.ShaderBinDxa ) ;
	if( LIVE2DSYS.ShaderBinDxaImage )
	{
		DXFREE( LIVE2DSYS.ShaderBinDxaImage ) ;
		LIVE2DSYS.ShaderBinDxaImage = NULL ;
	}

	// 初期化済みフラグを倒す
	LIVE2DSYS.InitializeFlag = FALSE ;

	// 正常終了
	return 0 ;
}








// シェーダーDXAファイルのセットアップ
extern int Live2DCubism4_SetupShaderCode( int *ShaderCodeBinConvert, BYTE *ShaderCodeBin )
{
	int Size ;

	if( ShaderCodeBinConvert != NULL && *ShaderCodeBinConvert == 0 )
	{
		*ShaderCodeBinConvert = 1 ;
		Base64ToBin( ShaderCodeBin, ShaderCodeBin ) ;
	}

	// シェーダーオブジェクトファイルＤＸＡを圧縮したデータを解凍する
	Size = DXA_Decode( ShaderCodeBin, NULL ) ;
	LIVE2DSYS.ShaderBinDxaImage = DXALLOC( ( size_t )Size ) ;
	if( LIVE2DSYS.ShaderBinDxaImage == NULL )
	{
		goto ERR ;
	}

	DXA_Decode( ShaderCodeBin, LIVE2DSYS.ShaderBinDxaImage ) ;

	// ＤＸＡファイルをオープンする
	DXA_Initialize( &LIVE2DSYS.ShaderBinDxa ) ;
	if( DXA_OpenArchiveFromMem( &LIVE2DSYS.ShaderBinDxa, LIVE2DSYS.ShaderBinDxaImage, Size, FALSE, FALSE ) != 0 )
	{
		goto ERR ;
	}

	// 正常終了
	return 0 ;

ERR :
	if( LIVE2DSYS.ShaderBinDxaImage != NULL )
	{
		DXFREE( LIVE2DSYS.ShaderBinDxaImage ) ;
		LIVE2DSYS.ShaderBinDxaImage = NULL ;
	}

	return -1 ;
}

// 指定名のシェーダーを読み込む
extern int Live2DCubism4_LoadShaderCode( const char *ShaderName, int ShaderType )
{
	int Addr, Size ;
	BYTE *FileImage ;
	int Ret ;

	if( DXA_GetFileInfo( &LIVE2DSYS.ShaderBinDxa, DX_CHARCODEFORMAT_ASCII, ShaderName, &Addr, &Size ) < 0 )
	{
		return -1 ;
	}

	FileImage = ( BYTE * )DXA_GetFileImage( &LIVE2DSYS.ShaderBinDxa ) + Addr ;

	Ret = Graphics_Shader_CreateHandle_UseGParam( ShaderType, FileImage, Size, FALSE, FALSE ) ;

	return Ret ;
}









// ハンドル初期化・後始末

// Live2D Cubism 4 モデルハンドルの初期化
static int Live2DCubism4_Model_InitializeHandle( HANDLEINFO * HandleInfo )
{
	LIVE2DCUBISM4MODEL *Model = ( LIVE2DCUBISM4MODEL * )HandleInfo ;

	// トランスレーションパラメータを初期化
	Model->PosX = 0.0f ;
	Model->PosY = 0.0f ;
	Model->ExRateX = 1.0f ;
	Model->ExRateY = 1.0f ;
	Model->RotAngle = 0.0f ;

	// 最後に再生したモーションのグループ内の番号を初期化
	Model->LastPlayMotionNo = -1 ;

	// 終了
	return 0 ;
}

// Live2D Cubism 4 モデルハンドルの後始末
static int Live2DCubism4_Model_TerminateHandle( HANDLEINFO *HandleInfo )
{
	LIVE2DCUBISM4MODEL *Model = ( LIVE2DCUBISM4MODEL * )HandleInfo ;

	delete_D_LAppModel( Model->AppModel ) ;
	Model->AppModel = NULL ;

	// 終了
	return 0 ;
}

// LIVE2DCUBISM4_LOADMODEL_GPARAM のデータをセットする
extern void Live2DCubism4_InitLoadModelGParam( LIVE2DCUBISM4_LOADMODEL_GPARAM *GParam )
{
	_MEMSET( GParam, 0, sizeof( *GParam ) ) ;
}







// Live2DCubismCore.dll のファイルパスを設定する
extern int NS_Live2D_SetCubism4CoreDLLPath( const TCHAR *CoreDLLFilePath )
{
#ifdef UNICODE
	return Live2D_SetCubism4CoreDLLPath_WCHAR_T(
		CoreDLLFilePath
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( CoreDLLFilePath, return -1 )

	Result = Live2D_SetCubism4CoreDLLPath_WCHAR_T(
		UseCoreDLLFilePathBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( CoreDLLFilePath )

	return Result ;
#endif
}

// Live2DCubismCore.dll のファイルパスを設定する
extern int NS_Live2D_SetCubism4CoreDLLPathWithStrLen( const TCHAR *CoreDLLFilePath, size_t CoreDLLFilePathLength )
{
	int Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( CoreDLLFilePath, CoreDLLFilePathLength, return -1 )
	Result = Live2D_SetCubism4CoreDLLPath_WCHAR_T( UseCoreDLLFilePathBuffer ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( CoreDLLFilePath )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( CoreDLLFilePath, CoreDLLFilePathLength, return -1 )
	Result = Live2D_SetCubism4CoreDLLPath_WCHAR_T( UseCoreDLLFilePathBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( CoreDLLFilePath )
#endif
	return Result ;
}

// Live2DCubismCore.dll のファイルパスを設定する
extern int NS_Live2D_SetCubism3CoreDLLPath( const TCHAR *CoreDLLFilePath )
{
#ifdef UNICODE
	return Live2D_SetCubism4CoreDLLPath_WCHAR_T(
		CoreDLLFilePath
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( CoreDLLFilePath, return -1 )

	Result = Live2D_SetCubism4CoreDLLPath_WCHAR_T(
		UseCoreDLLFilePathBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( CoreDLLFilePath )

	return Result ;
#endif
}

// Live2DCubismCore.dll のファイルパスを設定する
extern int NS_Live2D_SetCubism3CoreDLLPathWithStrLen( const TCHAR *CoreDLLFilePath, size_t CoreDLLFilePathLength )
{
	int Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( CoreDLLFilePath, CoreDLLFilePathLength, return -1 )
	Result = Live2D_SetCubism4CoreDLLPath_WCHAR_T( UseCoreDLLFilePathBuffer ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( CoreDLLFilePath )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( CoreDLLFilePath, CoreDLLFilePathLength, return -1 )
	Result = Live2D_SetCubism4CoreDLLPath_WCHAR_T( UseCoreDLLFilePathBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( CoreDLLFilePath )
#endif
	return Result ;
}

extern int Live2D_SetCubism4CoreDLLPath_WCHAR_T( const wchar_t *CoreDLLFilePath )
{
	// パスが長すぎたらエラー
	if( CL_strlen( WCHAR_T_CHARCODEFORMAT, ( char * )CoreDLLFilePath ) >= sizeof( LIVE2DDLL.Live2DCubismCoreDLLPath ) / sizeof( wchar_t ) )
	{
		return -1 ;
	}

	// パスを保存
	CL_strcpy( WCHAR_T_CHARCODEFORMAT, ( char * )LIVE2DDLL.Live2DCubismCoreDLLPath, ( char * )CoreDLLFilePath ) ;

	// 正常終了
	return 0 ;
}

// Live2D の描画処理を開始
extern int NS_Live2D_RenderBegin( void )
{
	int windowWidth, windowHeight ;

	// 初期化がされていなければエラー
	if( LIVE2DSYS.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// 各種描画設定情報を取得
	Graphics_DrawSetting_GetScreenDrawSettingInfo( &LIVE2DSYS.ScreenDrawSettingInfo ) ;

	// D3D11 フレーム先頭処理 
	// 各フレームでの、Cubismの処理前にコール 
	NS_GetGraphSize( NS_GetDrawScreen(), &windowWidth, &windowHeight ) ;
	D_CubismRenderer_DxLib::StartFrame( windowWidth, windowHeight ) ;

	// 正常終了
	return 0 ;
}

// Live2D の描画処理を終了
extern int NS_Live2D_RenderEnd( void )
{
	// 初期化がされていなければエラー
	if( LIVE2DSYS.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// D3D11 フレーム終了処理 
	// 各フレームでの、Cubismの処理後にコール
	D_CubismRenderer_DxLib::EndFrame() ;

	// ＤＸライブラリ用の設定に戻す
	Graphics_Hardware_RefreshSetting_PF() ;
	Graphics_DrawSetting_SetScreenDrawSettingInfo( &LIVE2DSYS.ScreenDrawSettingInfo ) ;

	// 正常終了
	return 0 ;
}

// Live2D のモデルファイルを読み込む
extern int NS_Live2D_LoadModel( const TCHAR *FilePath )
{
#ifdef UNICODE
	return Live2D_LoadModel_WCHAR_T(
		FilePath
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FilePath, return -1 )

	Result = Live2D_LoadModel_WCHAR_T(
		UseFilePathBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FilePath )

	return Result ;
#endif
}

// Live2D のモデルファイルを読み込む
extern int NS_Live2D_LoadModelWithStrLen( const TCHAR *FilePath, size_t FilePathLength )
{
	int Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( FilePath, FilePathLength, return -1 )
	Result = Live2D_LoadModel_WCHAR_T( UseFilePathBuffer ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( FilePath )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( FilePath, FilePathLength, return -1 )
	Result = Live2D_LoadModel_WCHAR_T( UseFilePathBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( FilePath )
#endif
	return Result ;
}

extern int Live2D_LoadModel_WCHAR_T( const wchar_t *FilePath )
{
	LIVE2DCUBISM4_LOADMODEL_GPARAM GParam ;

	Live2DCubism4_InitLoadModelGParam( &GParam ) ;

	return Live2DCubism4_LoadModel_UseGParam( &GParam, FilePath, GetASyncLoadFlag() ) ;
}

// Live2D_LoadModel の実処理関数
extern int Live2DCubism4_LoadModel_Static(
	LIVE2DCUBISM4_LOADMODEL_GPARAM * /*GParam*/,
	int Live2DModelHandle,
	const wchar_t *FilePath,
	int ASyncThread
)
{
	LIVE2DCUBISM4MODEL * Model ;
	wchar_t DirPath[ 1024 ], FileName[ 256 ] ;

	// エラー判定
	if( ASyncThread )
	{
		if( LIVE2DCUBISM4MODELCHK_ASYNC( Live2DModelHandle, Model ) )
			return -1 ;
	}
	else
	{
		if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
			return -1 ;
	}

	// D_LAppModel の作成
	Model->AppModel = new_D_LAppModel() ;

	// ファイルの読み込み
	AnalysisFileNameAndDirPathW_( FilePath, FileName, sizeof( FileName ), DirPath, sizeof( DirPath ) ) ;
	SetEnMarkW_( DirPath, sizeof( DirPath ) ) ;
	if( Model->AppModel->LoadAssets( ( BYTE * )DirPath, ( BYTE * )FileName, ASyncThread ) == false )
	{
		DXST_LOGFILEFMT_ADDUTF16LE(( "\x4c\x00\x69\x00\x76\x00\x65\x00\x32\x00\x44\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe2\x30\xc7\x30\xeb\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Live2D Error : モデルファイル %s の読み込みに失敗しました" @*/, DirPath )) ;
		return -1 ;
	}

	// 正常終了
	return 0 ;
}

#ifndef DX_NON_ASYNCLOAD

// Live2D_LoadModel の非同期読み込みスレッドから呼ばれる関数
static void Live2DCubism4_LoadModel_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	LIVE2DCUBISM4_LOADMODEL_GPARAM *GParam ;
	int Live2DModelHandle ;
	const wchar_t *FilePath ;
	int Addr ;
	int Result ;
	LIVE2DCUBISM4MODEL *Model ;

	Addr = 0 ;
	GParam = ( LIVE2DCUBISM4_LOADMODEL_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	Live2DModelHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FilePath = GetASyncLoadParamString( AParam->Data, &Addr ) ;

	Result = Live2DCubism4_LoadModel_Static( GParam, Live2DModelHandle, FilePath, TRUE ) ;
	if( !LIVE2DCUBISM4MODELCHK_ASYNC( Live2DModelHandle, Model ) )
	{
		Model->HandleInfo.ASyncLoadResult = Result ;
	}
	DecASyncLoadCount( Live2DModelHandle ) ;
	if( Result < 0 )
	{
		SubHandle( Live2DModelHandle, FALSE, FALSE ) ;
	}
}

#endif // DX_NON_ASYNCLOAD

// Live2D_LoadModel のグローバル変数にアクセスしないバージョン
extern int Live2DCubism4_LoadModel_UseGParam(
	LIVE2DCUBISM4_LOADMODEL_GPARAM *GParam,
	const wchar_t *FilePath,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	int Live2DModelHandle = -1 ;
	wchar_t FullPath[ 1024 ] ;

	if( LIVE2DSYS.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	Live2DModelHandle = AddHandle( DX_HANDLETYPE_LIVE2D_CUBISM4_MODEL, ASyncThread, -1 );
	if( Live2DModelHandle == -1 )
		goto ERR ;

	ConvertFullPathW_( FilePath, FullPath, sizeof( FullPath ) ) ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag && ASyncThread == FALSE )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// パラメータに必要なメモリのサイズを算出
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, Live2DModelHandle ) ;
		AddASyncLoadParamString( NULL, &Addr, FullPath ) ;

		// メモリの確保
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// 処理に必要な情報をセット
		AParam->ProcessFunction = Live2DCubism4_LoadModel_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Live2DModelHandle ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, FullPath ) ;

		// データを追加
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// 非同期読み込みカウントをインクリメント
		IncASyncLoadCount( Live2DModelHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Live2DCubism4_LoadModel_Static( GParam, Live2DModelHandle, FullPath, ASyncThread ) < 0 )
			goto ERR ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( Live2DModelHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// ハンドルを返す
	return Live2DModelHandle ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( Live2DModelHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	SubHandle( Live2DModelHandle, ASyncLoadFlag, ASyncThread ) ;
	Live2DModelHandle = -1 ;

	// 終了
	return -1 ;
}

// Live2D のモデルを削除する
extern int NS_Live2D_DeleteModel( int Live2DModelHandle )
{
	return SubHandle( Live2DModelHandle, GetASyncLoadFlag(), FALSE ) ;
}

// すべての Live2D のモデルを削除する
extern int NS_Live2D_InitModel( void )
{
	return AllHandleSub( DX_HANDLETYPE_LIVE2D_CUBISM4_MODEL ) ;
}

// Live2D のモデル描画で使用するシェーダーを設定する( ShaderHandle に -1 を渡すと解除 )
extern int NS_Live2D_SetUserShader( int TargetShader /* DX_LIVE2D_SHADER_NORMAL_PIXEL 等 */ , int ShaderHandle )
{
	if( TargetShader < 0 || TargetShader >= DX_LIVE2D_SHADER_NUM )
	{
		return -1 ;
	}

	// シェーダーハンドルを保存
	LIVE2DSYS.UserShader[ TargetShader ] = ShaderHandle ;

	// 正常終了
	return 0 ;
}

// Live2D のモデル描画の前に呼ばれるコールバック関数を設定する Callback に NULL を渡すと設定を解除 )
extern int NS_Live2D_DrawCallback( void ( *Callback )( int Live2DModelHandle, int TextureIndex, void *UserData ), void *UserData )
{
	// 値を保存
	LIVE2DSYS.DrawUserCallback = Callback ;
	LIVE2DSYS.DrawUserCallbackData = UserData ;

	// 正常終了
	return 0 ;

}

// Live2D のモデル描画をする際に、画面サイズに応じたスケーリングを行うかを設定する( UseFlag  TRUE:スケーリングを行う( デフォルト )  FALSE:スケーリングを行わない )
extern int NS_Live2D_SetUseAutoScaling( int UseFlag )
{
	// 値を保存
	LIVE2DSYS.NotUseAutoScaling = UseFlag != 0 ? FALSE : TRUE ;

	// 正常終了
	return 0 ;
}

// Live2D のモデルを画面の中心に描画するかを設定する( UseFlag   TRUE:画面の中心に描画する( デフォルト )   FALSE:画面の中心に描画しない )
extern int NS_Live2D_SetUseAutoCentering( int UseFlag )
{
	// 値を保存
	LIVE2DSYS.NotUseAutoCentering = UseFlag != 0 ? FALSE : TRUE ;

	// 正常終了
	return 0 ;
}

// Live2D_Model_SetTranslate で指定する平行移動値の y の向きを反転するかを設定する( UseFlag   TRUE:反転する( デフォルト )   FALSE:反転しない )
extern int NS_Live2D_SetUseReverseYAxis( int UseFlag )
{
	// 値を保存
	LIVE2DSYS.NotUseReverseYAxis = UseFlag != 0 ? FALSE : TRUE ;

	// 正常終了
	return 0 ;
}

// Live2D のモデルの状態を更新する
extern int NS_Live2D_Model_Update( int Live2DModelHandle, float DeltaTimeSeconds )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// Cubismモデルの更新
	Model->AppModel->Update( DeltaTimeSeconds ) ;

	// 正常終了
	return 0 ;
}

// Live2D のモデルの位置を設定する
extern int NS_Live2D_Model_SetTranslate( int Live2DModelHandle, float x, float y )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値の保存
	Model->PosX = x ;
	Model->PosY = y ;

	// 正常終了
	return 0 ;
}

// Live2D のモデルの拡大率を設定する
extern int NS_Live2D_Model_SetExtendRate( int Live2DModelHandle, float ExRateX, float ExRateY )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値の保存
	Model->ExRateX = ExRateX ;
	Model->ExRateY = ExRateY ;

	// 正常終了
	return 0 ;
}

// Live2D のモデルの回転を設定する
extern int NS_Live2D_Model_SetRotate( int Live2DModelHandle, float RotAngle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値の保存
	Model->RotAngle = RotAngle ;

	// 正常終了
	return 0 ;
}

// Live2D のモデルを描画する
extern int NS_Live2D_Model_Draw( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;
	int windowWidth, windowHeight ;
	D_CubismVector2 SizeInPixels ;
	D_CubismVector2 OriginInPixels ;
	float PixelsPerUnit ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// キャンバスサイズの取得
	Model->AppModel->_model->GetCanvasInfo( &SizeInPixels, &OriginInPixels, &PixelsPerUnit ) ;

	// 描画に使用しているハンドルを保存
	LIVE2DSYS.NowDrawLive2DModelHandle = Live2DModelHandle ;

	// 投影用マトリックス 
	D_CubismMatrix44 *projection = new_D_CubismMatrix44() ;
	NS_GetGraphSize( NS_GetDrawScreen(), &windowWidth, &windowHeight ) ;
	if( LIVE2DSYS.NotUseAutoScaling )
	{
		projection->Scale(
			Model->ExRateX * PixelsPerUnit / ( float )windowWidth * 2.0f,
			Model->ExRateY * PixelsPerUnit / ( float )windowWidth * 2.0f * ( ( float )windowWidth / ( float )windowHeight )
		) ;
	}
	else
	{
		projection->Scale( Model->ExRateX, Model->ExRateY * ( ( float )windowWidth / ( float )windowHeight ) ) ;
	}

	if( LIVE2DSYS.NotUseAutoCentering )
	{
		if( LIVE2DSYS.NotUseReverseYAxis )
		{
			projection->Translate(
				 ( Model->PosX - ( windowWidth  - SizeInPixels.X ) / 2.0f ) / windowWidth  * 2.0f ,
				-( Model->PosY - ( windowHeight - SizeInPixels.Y ) / 2.0f ) / windowHeight * 2.0f
			) ;
		}
		else
		{
			projection->Translate(
				( Model->PosX - ( windowWidth  - SizeInPixels.X ) / 2.0f ) / windowWidth  * 2.0f ,
				( Model->PosY - ( windowHeight - SizeInPixels.Y ) / 2.0f ) / windowHeight * 2.0f
			) ;
		}
	}
	else
	{
		if( LIVE2DSYS.NotUseReverseYAxis )
		{
			projection->Translate( Model->PosX / windowWidth * 2.0f, -Model->PosY / windowHeight * 2.0f ) ;
		}
		else
		{
			projection->Translate( Model->PosX / windowWidth * 2.0f, Model->PosY / windowHeight * 2.0f ) ;
		}
	}

	projection->RotateRelative( -Model->RotAngle ) ;

	// 描画
	Model->AppModel->Draw( *projection, LIVE2DSYS.NotUseAutoScaling == FALSE ) ;

	delete_D_CubismMatrix44( projection ) ;

	// 描画に使用しているモデルハンドルをリセット
	LIVE2DSYS.NowDrawLive2DModelHandle = 0 ;

	// 正常終了
	return 0 ;
}

// Live2D のモデルの指定のモーションを再生する
extern int NS_Live2D_Model_StartMotion( int Live2DModelHandle, const TCHAR *group, int no, float fadeInSeconds, float fadeOutSeconds, int isLoopFadeIn, int isLoop )
{
#ifdef UNICODE
	return Live2D_Model_StartMotion_WCHAR_T(
		Live2DModelHandle, group, no, fadeInSeconds, fadeOutSeconds, isLoopFadeIn, isLoop
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( group, return -1 )

	Result = Live2D_Model_StartMotion_WCHAR_T(
		Live2DModelHandle, UsegroupBuffer, no, fadeInSeconds, fadeOutSeconds, isLoopFadeIn, isLoop
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( group )

	return Result ;
#endif
}

// Live2D のモデルの指定のモーションを再生する
extern int NS_Live2D_Model_StartMotionWithStrLen( int Live2DModelHandle, const TCHAR *group, size_t groupLength, int no, float fadeInSeconds, float fadeOutSeconds, int isLoopFadeIn, int isLoop )
{
	int Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( group, groupLength, return -1 )
	Result = Live2D_Model_StartMotion_WCHAR_T( Live2DModelHandle, UsegroupBuffer, no, fadeInSeconds, fadeOutSeconds, isLoopFadeIn, isLoop ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( group )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( group, groupLength, return -1 )
	Result = Live2D_Model_StartMotion_WCHAR_T( Live2DModelHandle, UsegroupBuffer, no, fadeInSeconds, fadeOutSeconds, isLoopFadeIn, isLoop ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( group )
#endif
	return Result ;
}

extern int Live2D_Model_StartMotion_WCHAR_T( int Live2DModelHandle, const wchar_t *group, int no, float fadeInSeconds, float fadeOutSeconds, int isLoopFadeIn, int isLoop )
{
	LIVE2DCUBISM4MODEL * Model ;
	D_CubismMotionQueueEntryHandle Result ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
	{
		return -1 ;
	}

	// モーションの再生
	WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( group, return -1, DX_CHARCODEFORMAT_UTF8 )
	Result = Model->AppModel->StartMotion( UsegroupBuffer, no, D_CubismMotion_PriorityForce, fadeInSeconds, fadeOutSeconds, isLoopFadeIn != FALSE, isLoop != FALSE ) ;
	WCHAR_T_TO_CHAR_STRING_END( group )

	if( Result != InvalidMotionQueueEntryHandleValue )
	{
		Model->LastPlayMotionNo = no ;
	}
	else
	{
		Model->LastPlayMotionNo = -1 ;
	}

	// 終了
	return Result == InvalidMotionQueueEntryHandleValue ? -1 : 0 ;
}

// Live2D のモデルで最後に再生したモーションのグループ内の番号を取得する
extern int NS_Live2D_Model_GetLastPlayMotionNo( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
	{
		return -1 ;
	}

	// 最後に再生したモーション番号を取得する
	return Model->LastPlayMotionNo ;
}

// Live2D のモデルのモーション再生が終了しているかを取得する
extern int NS_Live2D_Model_IsMotionFinished( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// モーションの再生が終了しているかどうか返す再生
	return Model->AppModel->_motionManager->IsFinished() ? TRUE : FALSE ;
}

// Live2D のモデルのモーション再生時間を取得する
extern float NS_Live2D_Model_GetMotionPlayTime( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	return Model->AppModel->_motionManager->GetMotionPlayTime() ;
}

// Live2D のモデルの指定の表情モーションをセットする
extern int NS_Live2D_Model_SetExpression( int Live2DModelHandle, const TCHAR *expressionID )
{
#ifdef UNICODE
	return Live2D_Model_SetExpression_WCHAR_T(
		Live2DModelHandle, expressionID
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( expressionID, return -1 )

	Result = Live2D_Model_SetExpression_WCHAR_T(
		Live2DModelHandle, UseexpressionIDBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( expressionID )

	return Result ;
#endif
}

// Live2D のモデルの指定の表情モーションをセットする
extern int NS_Live2D_Model_SetExpressionWithStrLen(	int Live2DModelHandle, const TCHAR *expressionID, size_t expressionIDLength )
{
	int Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( expressionID, expressionIDLength, return -1 )
	Result = Live2D_Model_SetExpression_WCHAR_T( Live2DModelHandle, UseexpressionIDBuffer ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( expressionID )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( expressionID, expressionIDLength, return -1 )
	Result = Live2D_Model_SetExpression_WCHAR_T( Live2DModelHandle, UseexpressionIDBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( expressionID )
#endif
	return Result ;
}

extern int Live2D_Model_SetExpression_WCHAR_T( int Live2DModelHandle, const wchar_t *expressionID )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 表情モーションの設定
	Model->AppModel->SetExpression( ( BYTE * )expressionID ) ;

	// 正常終了
	return 0 ;
}

// 指定の座標が Live2D のモデルの指定の当たり判定の矩形範囲内か判定する( TRUE:矩形範囲内  FALSE:矩形範囲外 )
extern int NS_Live2D_Model_HitTest( int Live2DModelHandle, const TCHAR *hitAreaName, float x, float y )
{
#ifdef UNICODE
	return Live2D_Model_HitTest_WCHAR_T(
		Live2DModelHandle, hitAreaName, x, y
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( hitAreaName, return -1 )

	Result = Live2D_Model_HitTest_WCHAR_T(
		Live2DModelHandle, UsehitAreaNameBuffer, x, y
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( hitAreaName )

	return Result ;
#endif
}

// 指定の座標が Live2D のモデルの指定の当たり判定の矩形範囲内か判定する( TRUE:矩形範囲内  FALSE:矩形範囲外 )
extern int NS_Live2D_Model_HitTestWithStrLen( int Live2DModelHandle, const TCHAR *hitAreaName, size_t hitAreaNameLength, float x, float y )
{
	int Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( hitAreaName, hitAreaNameLength, return -1 )
	Result = Live2D_Model_HitTest_WCHAR_T( Live2DModelHandle, UsehitAreaNameBuffer, x, y ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( hitAreaName )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( hitAreaName, hitAreaNameLength, return -1 )
	Result = Live2D_Model_HitTest_WCHAR_T( Live2DModelHandle, UsehitAreaNameBuffer, x, y ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( hitAreaName )
#endif
	return Result ;
}

extern int Live2D_Model_HitTest_WCHAR_T( int Live2DModelHandle, const wchar_t *hitAreaName, float x, float y )
{
	LIVE2DCUBISM4MODEL * Model ;
	int windowWidth, windowHeight;
	float tx, ty ;
	bool Result ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// スクリーン座標をLive2D座標に変換
	NS_GetGraphSize( NS_GetDrawScreen(), &windowWidth, &windowHeight ) ;
	tx = ( ( x - Model->PosX ) - windowWidth  * 0.5f ) /  windowWidth * 2.0f / Model->ExRateX ;
	ty = ( ( y - Model->PosY ) - windowHeight * 0.5f ) / -windowWidth * 2.0f / Model->ExRateY ;
	if( Model->RotAngle != 0.0f )
	{
		float Sin, Cos ;
		float tmp ;
		_SINCOS_PLATFORM( Model->RotAngle, &Sin, &Cos ) ;
		tmp = Cos * tx - Sin * ty ;
		ty  = Sin * tx + Cos * ty ;
		tx  = tmp ;
	}

	// 表情モーションの設定
	WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( hitAreaName, return -1, DX_CHARCODEFORMAT_UTF8 )
	Result = Model->AppModel->HitTest( UsehitAreaNameBuffer, tx, ty ) ;
	WCHAR_T_TO_CHAR_STRING_END( hitAreaName )

	return Result ? TRUE : FALSE ;
}

// Live2D のモデルに設定されているパラメータの数を取得する
extern int NS_Live2D_Model_GetParameterCount( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値を返す
	return Model->AppModel->GetModel()->GetParameterCount() ;
}

// Live2D のモデルに設定されているパラメータのIDを取得する
extern const TCHAR *NS_Live2D_Model_GetParameterId( int Live2DModelHandle, int index )
{
	LIVE2DCUBISM4MODEL * Model ;
	D_CubismIdHandle IdHandle ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

	// 値を返す
	IdHandle = Model->AppModel->GetModel()->GetParameterId( index ) ;
#ifdef UNICODE
	return ( wchar_t * )( IdHandle != NULL ? IdHandle->GetString().GetRawStringW() : NULL ) ;
#else
	return IdHandle != NULL ? IdHandle->GetString().GetRawStringA() : NULL ;
#endif
}

// Live2D のモデルに設定されているパラメータを取得する
extern float NS_Live2D_Model_GetParameterValue( int Live2DModelHandle, const TCHAR *parameterId )
{
#ifdef UNICODE
	return Live2D_Model_GetParameterValue_WCHAR_T(
		Live2DModelHandle, parameterId
	) ;
#else
	float Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( parameterId, return -1 )

	Result = Live2D_Model_GetParameterValue_WCHAR_T(
		Live2DModelHandle, UseparameterIdBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( parameterId )

	return Result ;
#endif
}

// Live2D のモデルに設定されているパラメータを取得する
extern float NS_Live2D_Model_GetParameterValueWithStrLen( int Live2DModelHandle, const TCHAR *parameterId, size_t parameterIdLength )
{
	float Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( parameterId, parameterIdLength, return -1 )
	Result = Live2D_Model_GetParameterValue_WCHAR_T( Live2DModelHandle, UseparameterIdBuffer ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( parameterId )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( parameterId, parameterIdLength, return -1 )
	Result = Live2D_Model_GetParameterValue_WCHAR_T( Live2DModelHandle, UseparameterIdBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( parameterId )
#endif
	return Result ;
}

extern float Live2D_Model_GetParameterValue_WCHAR_T( int Live2DModelHandle, const wchar_t *parameterId )
{
	LIVE2DCUBISM4MODEL * Model ;
	float Result = -1.0f ;
	int Count ;
	int i ;
	D_CubismModel *CModel ;
	D_CubismIdHandle IdHandle ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1.0f ;

	CModel = Model->AppModel->GetModel() ;
	Count = CModel->GetParameterCount() ;

	// 値を返す
	WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( parameterId, return -1.0f, DX_CHARCODEFORMAT_UTF8 )
	IdHandle = LIVE2DSYS.s_cubismIdManager->FindId( UseparameterIdBuffer ) ;
	if( IdHandle != NULL )
	{
		for( i = 0 ; i < Count ; i ++ )
		{
			if( CModel->GetParameterId( i ) == IdHandle )
			{
				Result = Model->AppModel->GetModel()->GetParameterValue( i ) ;
				break ;
			}
		}
	}
	WCHAR_T_TO_CHAR_STRING_END( parameterId )

	return Result ;
}

// Live2D のモデルに設定されているパラメータを設定する
extern int NS_Live2D_Model_SetParameterValue( int Live2DModelHandle, const TCHAR *parameterId, float value )
{
#ifdef UNICODE
	return Live2D_Model_SetParameterValue_WCHAR_T(
		Live2DModelHandle, parameterId, value
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( parameterId, return -1 )

	Result = Live2D_Model_SetParameterValue_WCHAR_T(
		Live2DModelHandle, UseparameterIdBuffer, value
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( parameterId )

	return Result ;
#endif
}

// Live2D のモデルに設定されているパラメータを設定する
extern int NS_Live2D_Model_SetParameterValueWithStrLen( int Live2DModelHandle, const TCHAR *parameterId, size_t parameterIdLength, float value )
{
	int Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( parameterId, parameterIdLength, return -1 )
	Result = Live2D_Model_SetParameterValue_WCHAR_T( Live2DModelHandle, UseparameterIdBuffer, value ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( parameterId )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( parameterId, parameterIdLength, return -1 )
	Result = Live2D_Model_SetParameterValue_WCHAR_T( Live2DModelHandle, UseparameterIdBuffer, value ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( parameterId )
#endif
	return Result ;
}

extern int Live2D_Model_SetParameterValue_WCHAR_T( int Live2DModelHandle, const wchar_t *parameterId, float value )
{
	LIVE2DCUBISM4MODEL * Model ;
	int Count ;
	int i ;
	D_CubismModel *CModel ;
	D_CubismIdHandle IdHandle ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	CModel = Model->AppModel->GetModel() ;
	Count = CModel->GetParameterCount() ;

	// パラメータを設定する
	WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( parameterId, return -1, DX_CHARCODEFORMAT_UTF8 )
	IdHandle = LIVE2DSYS.s_cubismIdManager->FindId( UseparameterIdBuffer ) ;
	if( IdHandle != NULL )
	{
		for( i = 0 ; i < Count ; i ++ )
		{
			if( CModel->GetParameterId( i ) == IdHandle )
			{
				Model->AppModel->SetUserParameter( i, value ) ;
				break ;
			}
		}
	}
	WCHAR_T_TO_CHAR_STRING_END( parameterId )

	return 0 ;
}

// Live2D のモデルに設定された当たり判定の数を取得する
extern int NS_Live2D_Model_GetHitAreasCount( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値を返す
	return Model->AppModel->_modelSetting->GetHitAreasCount() ;
}

// Live2D のモデルの当たり判定に設定された名前を取得する
extern const TCHAR *NS_Live2D_Model_GetHitAreaName( int Live2DModelHandle, int index )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

#ifdef UNICODE
	return ( wchar_t * )Model->AppModel->_modelSetting->GetHitAreaNameW( index ) ;
#else
	return Model->AppModel->_modelSetting->GetHitAreaNameA( index ) ;
#endif 
}

// Live2D のモデルの物理演算設定ファイルの名前を取得する
extern const TCHAR *NS_Live2D_Model_GetPhysicsFileName( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

#ifdef UNICODE
	return ( wchar_t * )Model->AppModel->_modelSetting->GetPhysicsFileNameW() ;
#else
	return Model->AppModel->_modelSetting->GetPhysicsFileNameA() ;
#endif 
}

// Live2D のモデルのパーツ切り替え設定ファイルの名前を取得する
extern const TCHAR *NS_Live2D_Model_GetPoseFileName( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

#ifdef UNICODE
	return ( wchar_t * )Model->AppModel->_modelSetting->GetPoseFileNameW() ;
#else
	return Model->AppModel->_modelSetting->GetPoseFileNameA() ;
#endif 
}

// Live2D のモデルの表情設定ファイルの数を取得する
extern int NS_Live2D_Model_GetExpressionCount( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値を返す
	return Model->AppModel->_modelSetting->GetExpressionCount() ;
}

// Live2D のモデルの表情設定ファイルを識別するIDを取得する
extern const TCHAR *NS_Live2D_Model_GetExpressionName( int Live2DModelHandle, int index )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

#ifdef UNICODE
	return ( wchar_t * )Model->AppModel->_modelSetting->GetExpressionNameW( index ) ;
#else
	return Model->AppModel->_modelSetting->GetExpressionNameA( index ) ;
#endif 
}

// Live2D のモデルの表情設定ファイルの名前を取得する
extern const TCHAR *NS_Live2D_Model_GetExpressionFileName( int Live2DModelHandle, int index )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

#ifdef UNICODE
	return ( wchar_t * )Model->AppModel->_modelSetting->GetExpressionFileNameW( index ) ;
#else
	return Model->AppModel->_modelSetting->GetExpressionFileNameA( index ) ;
#endif 
}

// Live2D のモデルのモーショングループの数を取得する
extern int NS_Live2D_Model_GetMotionGroupCount( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値を返す
	return Model->AppModel->_modelSetting->GetMotionGroupCount() ;
}

// Live2D のモデルのモーショングループの名前を取得する
extern const TCHAR *NS_Live2D_Model_GetMotionGroupName( int Live2DModelHandle, int index )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

#ifdef UNICODE
	return ( wchar_t * )Model->AppModel->_modelSetting->GetMotionGroupNameW( index ) ;
#else
	return Model->AppModel->_modelSetting->GetMotionGroupNameA( index ) ;
#endif 
}

// Live2D のモデルのモーショングループに含まれるモーションの数を取得する
extern int NS_Live2D_Model_GetMotionCount( int Live2DModelHandle, const TCHAR *groupName )
{
#ifdef UNICODE
	return Live2D_Model_GetMotionCount_WCHAR_T(
		Live2DModelHandle, groupName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, return -1 )

	Result = Live2D_Model_GetMotionCount_WCHAR_T(
		Live2DModelHandle, UsegroupNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( groupName )

	return Result ;
#endif
}

// Live2D のモデルのモーショングループに含まれるモーションの数を取得する
extern int NS_Live2D_Model_GetMotionCountWithStrLen( int Live2DModelHandle, const TCHAR *groupName, size_t groupNameLength )
{
	int Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return -1 )
	Result = Live2D_Model_GetMotionCount_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return -1 )
	Result = Live2D_Model_GetMotionCount_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#endif
	return Result ;
}

extern int Live2D_Model_GetMotionCount_WCHAR_T( int Live2DModelHandle, const wchar_t *groupName )
{
	LIVE2DCUBISM4MODEL * Model ;
	int Result ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値を返す
	WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( groupName, return -1, DX_CHARCODEFORMAT_UTF8 )
	Result = Model->AppModel->_modelSetting->GetMotionCount( UsegroupNameBuffer ) ;
	WCHAR_T_TO_CHAR_STRING_END( groupName )

	return Result ;
}

// Live2D のモデルのグループ名とインデックス値からモーションファイルの名前を取得する
extern const TCHAR *NS_Live2D_Model_GetMotionFileName( int Live2DModelHandle, const TCHAR *groupName, int index )
{
#ifdef UNICODE
	return Live2D_Model_GetMotionFileName_WCHAR_T(
		Live2DModelHandle, groupName, index
	) ;
#else
	const TCHAR *Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, return NULL )

	Result = Live2D_Model_GetMotionFileName_WCHAR_T(
		Live2DModelHandle, UsegroupNameBuffer, index
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( groupName )

	return Result ;
#endif
}

// Live2D のモデルのグループ名とインデックス値からモーションファイルの名前を取得する
extern const TCHAR *NS_Live2D_Model_GetMotionFileNameWithStrLen( int Live2DModelHandle, const TCHAR *groupName, size_t groupNameLength, int index )
{
	const TCHAR *Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return NULL )
	Result = Live2D_Model_GetMotionFileName_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer, index ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return NULL )
	Result = Live2D_Model_GetMotionFileName_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer, index ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#endif
	return Result ;
}

extern const TCHAR *Live2D_Model_GetMotionFileName_WCHAR_T( int Live2DModelHandle, const wchar_t *groupName, int index )
{
	LIVE2DCUBISM4MODEL * Model ;
	const TCHAR *Result ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

	WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( groupName, return NULL, DX_CHARCODEFORMAT_UTF8 )
#ifdef UNICODE
	Result = ( wchar_t * )Model->AppModel->_modelSetting->GetMotionFileNameW( UsegroupNameBuffer, index ) ;
#else
	Result = Model->AppModel->_modelSetting->GetMotionFileNameA( UsegroupNameBuffer, index ) ;
#endif 
	WCHAR_T_TO_CHAR_STRING_END( groupName )

	return Result ;
}

// Live2D のモデルのモーションに対応するサウンドファイルの名前を取得する
extern const TCHAR *NS_Live2D_Model_GetMotionSoundFileName( int Live2DModelHandle, const TCHAR *groupName, int index )
{
#ifdef UNICODE
	return Live2D_Model_GetMotionSoundFileName_WCHAR_T(
		Live2DModelHandle, groupName, index
	) ;
#else
	const TCHAR *Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, return NULL )

	Result = Live2D_Model_GetMotionSoundFileName_WCHAR_T(
		Live2DModelHandle, UsegroupNameBuffer, index
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( groupName )

	return Result ;
#endif
}

// Live2D のモデルのモーションに対応するサウンドファイルの名前を取得する
extern const TCHAR *NS_Live2D_Model_GetMotionSoundFileNameWithStrLen( int Live2DModelHandle, const TCHAR *groupName, size_t groupNameLength, int index )
{
	const TCHAR * Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return NULL )
	Result = Live2D_Model_GetMotionSoundFileName_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer, index ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return NULL )
	Result = Live2D_Model_GetMotionSoundFileName_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer, index ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#endif
	return Result ;
}

extern const TCHAR *Live2D_Model_GetMotionSoundFileName_WCHAR_T( int Live2DModelHandle, const wchar_t *groupName, int index )
{
	LIVE2DCUBISM4MODEL * Model ;
	const TCHAR *Result ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

	WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( groupName, return NULL, DX_CHARCODEFORMAT_UTF8 )
#ifdef UNICODE
	Result = ( wchar_t * )Model->AppModel->_modelSetting->GetMotionSoundFileNameW( UsegroupNameBuffer, index ) ;
#else
	Result = Model->AppModel->_modelSetting->GetMotionSoundFileNameA( UsegroupNameBuffer, index ) ;
#endif 
	WCHAR_T_TO_CHAR_STRING_END( groupName )

	return Result ;
}

// Live2D のモデルのモーション開始時のフェードイン処理時間を取得する
extern float NS_Live2D_Model_GetMotionFadeInTimeValue( int Live2DModelHandle, const TCHAR *groupName, int index )
{
#ifdef UNICODE
	return Live2D_Model_GetMotionFadeInTimeValue_WCHAR_T(
		Live2DModelHandle, groupName, index
	) ;
#else
	float Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, return -1.0f )

	Result = Live2D_Model_GetMotionFadeInTimeValue_WCHAR_T(
		Live2DModelHandle, UsegroupNameBuffer, index
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( groupName )

	return Result ;
#endif
}

// Live2D のモデルのモーション開始時のフェードイン処理時間を取得する
extern float NS_Live2D_Model_GetMotionFadeInTimeValueWithStrLen( int Live2DModelHandle, const TCHAR *groupName, size_t groupNameLength, int index )
{
	float Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return -1.0f )
	Result = Live2D_Model_GetMotionFadeInTimeValue_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer, index ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return -1.0f )
	Result = Live2D_Model_GetMotionFadeInTimeValue_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer, index ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#endif
	return Result ;
}

extern float Live2D_Model_GetMotionFadeInTimeValue_WCHAR_T(	int Live2DModelHandle, const wchar_t *groupName, int index )
{
	LIVE2DCUBISM4MODEL * Model ;
	float Result ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1.0f ;

	// 値を返す
	WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( groupName, return -1.0f, DX_CHARCODEFORMAT_UTF8 )
	Result = Model->AppModel->_modelSetting->GetMotionFadeInTimeValue( UsegroupNameBuffer, index ) ;
	WCHAR_T_TO_CHAR_STRING_END( groupName )

	return Result ;
}

// Live2D のモデルのモーション終了時のフェードアウト処理時間を取得する
extern float NS_Live2D_Model_GetMotionFadeOutTimeValue( int Live2DModelHandle, const TCHAR *groupName, int index )
{
#ifdef UNICODE
	return Live2D_Model_GetMotionFadeOutTimeValue_WCHAR_T(
		Live2DModelHandle, groupName, index
	) ;
#else
	float Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, return -1.0f )

	Result = Live2D_Model_GetMotionFadeOutTimeValue_WCHAR_T(
		Live2DModelHandle, UsegroupNameBuffer, index
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( groupName )

	return Result ;
#endif
}

// Live2D のモデルのモーション終了時のフェードアウト処理時間を取得する
extern float NS_Live2D_Model_GetMotionFadeOutTimeValueWithStrLen( int Live2DModelHandle, const TCHAR *groupName, size_t groupNameLength, int index )
{
	float Result ;
#ifdef UNICODE
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return -1.0f )
	Result = Live2D_Model_GetMotionFadeOutTimeValue_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer, index ) ;
	WCHAR_T_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#else
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_ONE_BEGIN( groupName, groupNameLength, return -1.0f )
	Result = Live2D_Model_GetMotionFadeOutTimeValue_WCHAR_T( Live2DModelHandle, UsegroupNameBuffer, index ) ;
	TCHAR_STRING_WITH_STRLEN_TO_WCHAR_T_STRING_END( groupName )
#endif
	return Result ;
}

extern float Live2D_Model_GetMotionFadeOutTimeValue_WCHAR_T( int Live2DModelHandle, const wchar_t *groupName, int index )
{
	LIVE2DCUBISM4MODEL * Model ;
	float Result ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1.0f ;

	// 値を返す
	WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( groupName, return -1.0f, DX_CHARCODEFORMAT_UTF8 )
	Result = Model->AppModel->_modelSetting->GetMotionFadeOutTimeValue( UsegroupNameBuffer, index ) ;
	WCHAR_T_TO_CHAR_STRING_END( groupName )

	return Result ;
}

// Live2D のモデルのユーザデータのファイル名を取得する
extern const TCHAR *NS_Live2D_Model_GetUserDataFile( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

#ifdef UNICODE
	return ( wchar_t * )Model->AppModel->_modelSetting->GetUserDataFileW() ;
#else
	return Model->AppModel->_modelSetting->GetUserDataFileA() ;
#endif 
}

// Live2D のモデルの目パチに関連付けられたパラメータの数を取得する
extern int NS_Live2D_Model_GetEyeBlinkParameterCount( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値を返す
	return Model->AppModel->_modelSetting->GetEyeBlinkParameterCount() ;
}

// Live2D のモデルの目パチに関連付けられたパラメータのIDを取得する
extern const TCHAR *NS_Live2D_Model_GetEyeBlinkParameterId( int Live2DModelHandle, int index )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

#ifdef UNICODE
	return ( wchar_t * )Model->AppModel->_modelSetting->GetEyeBlinkParameterId( index )->GetString().GetRawStringW() ;
#else
	return Model->AppModel->_modelSetting->GetEyeBlinkParameterId( index )->GetString().GetRawStringA() ;
#endif 
}

// Live2D のモデルのリップシンクに関連付けられたパラメータの数を取得する
extern int NS_Live2D_Model_GetLipSyncParameterCount( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1 ;

	// 値を返す
	return Model->AppModel->_modelSetting->GetLipSyncParameterCount() ;
}

// Live2D のモデルのリップシンクに関連付けられたパラメータのIDを取得する
extern const TCHAR *NS_Live2D_Model_GetLipSyncParameterId( int Live2DModelHandle, int index )
{
	LIVE2DCUBISM4MODEL * Model ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return NULL ;

#ifdef UNICODE
	return ( wchar_t * )Model->AppModel->_modelSetting->GetLipSyncParameterId( index )->GetString().GetRawStringW() ;
#else
	return Model->AppModel->_modelSetting->GetLipSyncParameterId( index )->GetString().GetRawStringA() ;
#endif 
}

// Live2D のモデルのキャンバスの横幅を取得する
extern float NS_Live2D_Model_GetCanvasWidth( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;
	D_CubismVector2 SizeInPixels ;
	D_CubismVector2 OriginInPixels ;
	float PixelsPerUnit ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1.0f ;

	Model->AppModel->_model->GetCanvasInfo( &SizeInPixels, &OriginInPixels, &PixelsPerUnit ) ;

	return SizeInPixels.X ;
}

// Live2D のモデルのキャンバスの縦幅を取得する
extern float NS_Live2D_Model_GetCanvasHeight( int Live2DModelHandle )
{
	LIVE2DCUBISM4MODEL * Model ;
	D_CubismVector2 SizeInPixels ;
	D_CubismVector2 OriginInPixels ;
	float PixelsPerUnit ;

	// エラー判定
	if( LIVE2DCUBISM4MODELCHK( Live2DModelHandle, Model ) )
		return -1.0f ;

	Model->AppModel->_model->GetCanvasInfo( &SizeInPixels, &OriginInPixels, &PixelsPerUnit ) ;

	return SizeInPixels.Y ;
}







#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_LIVE2D_CUBISM4

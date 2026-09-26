// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Live2D Cubism4 関係プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DXLIVE2D_CUBISM4_H
#define DXLIVE2D_CUBISM4_H

// インクルード ------------------------------------------------------------------
#include "DxCompileConfig.h"

#ifndef DX_NON_LIVE2D_CUBISM4
#include "DxUseCLibLive2DCubism4.h"
#include "DxLib.h"
#include "DxHandle.h"
#include "DxThread.h"
#include "DxGraphics.h"
#include "DxBaseFunc.h"
#include "DxStatic.h"

#ifdef WINDOWS_DESKTOP_OS
	#include "Windows/DxLive2DCubism4Win.h"
#endif // WINDOWS_DESKTOP_OS

#ifdef __ANDROID__
	#include "Android/DxLive2DCubism4Android.h"
#endif // __ANDROID__

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
		#include "iOS/DxLive2DCubism4iOS.h"
    #endif // TARGET_OS_IPHONE
#endif // __APPLE__






#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif// DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define LIVE2DSYS					Live2DCubism4SysData
#define LIVE2DDLL					Live2DCubism4DLLData

#ifdef WINDOWS_DESKTOP_OS
#define CALL_csmGetVersion( x )						( Live2DCubism4DLLData.csmGetVersion					== NULL ? Live2DCubism4DLLData.std_csmGetVersion x				 		:	Live2DCubism4DLLData.csmGetVersion x				 	)
#define CALL_csmGetLatestMocVersion( x )			( Live2DCubism4DLLData.csmGetLatestMocVersion			== NULL ? Live2DCubism4DLLData.std_csmGetLatestMocVersion x		 		:	Live2DCubism4DLLData.csmGetLatestMocVersion x		 	)
#define CALL_csmGetMocVersion( x )					( Live2DCubism4DLLData.csmGetMocVersion					== NULL ? Live2DCubism4DLLData.std_csmGetMocVersion x					:	Live2DCubism4DLLData.csmGetMocVersion x					)
#define CALL_csmHasMocConsistency( x )				( Live2DCubism4DLLData.csmHasMocConsistency				== NULL ? Live2DCubism4DLLData.std_csmHasMocConsistency x				:	Live2DCubism4DLLData.csmHasMocConsistency x				)
#define CALL_csmGetLogFunction( x )					( Live2DCubism4DLLData.csmGetLogFunction				== NULL ? Live2DCubism4DLLData.std_csmGetLogFunction x			 		:	Live2DCubism4DLLData.csmGetLogFunction x			 	)
#define CALL_csmSetLogFunction( x )					( Live2DCubism4DLLData.csmSetLogFunction				== NULL ? Live2DCubism4DLLData.std_csmSetLogFunction x			 		:	Live2DCubism4DLLData.csmSetLogFunction x			 	)
#define CALL_csmReviveMocInPlace( x )				( Live2DCubism4DLLData.csmReviveMocInPlace				== NULL ? Live2DCubism4DLLData.std_csmReviveMocInPlace x				:	Live2DCubism4DLLData.csmReviveMocInPlace x				)
#define CALL_csmGetSizeofModel( x )					( Live2DCubism4DLLData.csmGetSizeofModel				== NULL ? Live2DCubism4DLLData.std_csmGetSizeofModel x			 		:	Live2DCubism4DLLData.csmGetSizeofModel x			 	)
#define CALL_csmInitializeModelInPlace( x )			( Live2DCubism4DLLData.csmInitializeModelInPlace		== NULL ? Live2DCubism4DLLData.std_csmInitializeModelInPlace x	 		:	Live2DCubism4DLLData.csmInitializeModelInPlace x	 	)
#define CALL_csmUpdateModel( x )					( Live2DCubism4DLLData.csmUpdateModel					== NULL ? Live2DCubism4DLLData.std_csmUpdateModel x				 		:	Live2DCubism4DLLData.csmUpdateModel x				 	)
#define CALL_csmReadCanvasInfo( x )					( Live2DCubism4DLLData.csmReadCanvasInfo				== NULL ? Live2DCubism4DLLData.std_csmReadCanvasInfo x			 		:	Live2DCubism4DLLData.csmReadCanvasInfo x			 	)
#define CALL_csmGetParameterCount( x )				( Live2DCubism4DLLData.csmGetParameterCount				== NULL ? Live2DCubism4DLLData.std_csmGetParameterCount x				:	Live2DCubism4DLLData.csmGetParameterCount x				)
#define CALL_csmGetParameterIds( x )				( Live2DCubism4DLLData.csmGetParameterIds				== NULL ? Live2DCubism4DLLData.std_csmGetParameterIds x			 		:	Live2DCubism4DLLData.csmGetParameterIds x			 	)
#define CALL_csmGetParameterTypes( x )				( Live2DCubism4DLLData.csmGetParameterTypes				== NULL ? Live2DCubism4DLLData.std_csmGetParameterTypes x				:	Live2DCubism4DLLData.csmGetParameterTypes x				)
#define CALL_csmGetParameterCount( x )				( Live2DCubism4DLLData.csmGetParameterCount				== NULL ? Live2DCubism4DLLData.std_csmGetParameterCount x				:	Live2DCubism4DLLData.csmGetParameterCount x				)
#define CALL_csmGetParameterMinimumValues( x )		( Live2DCubism4DLLData.csmGetParameterMinimumValues		== NULL ? Live2DCubism4DLLData.std_csmGetParameterMinimumValues x		:	Live2DCubism4DLLData.csmGetParameterMinimumValues x		)
#define CALL_csmGetParameterMaximumValues( x )		( Live2DCubism4DLLData.csmGetParameterMaximumValues		== NULL ? Live2DCubism4DLLData.std_csmGetParameterMaximumValues x		:	Live2DCubism4DLLData.csmGetParameterMaximumValues x		)
#define CALL_csmGetParameterDefaultValues( x )		( Live2DCubism4DLLData.csmGetParameterDefaultValues		== NULL ? Live2DCubism4DLLData.std_csmGetParameterDefaultValues x		:	Live2DCubism4DLLData.csmGetParameterDefaultValues x		)
#define CALL_csmGetParameterValues( x )				( Live2DCubism4DLLData.csmGetParameterValues			== NULL ? Live2DCubism4DLLData.std_csmGetParameterValues x		 		:	Live2DCubism4DLLData.csmGetParameterValues x		 	)
#define CALL_csmGetParameterKeyCounts( x )			( Live2DCubism4DLLData.csmGetParameterKeyCounts			== NULL ? Live2DCubism4DLLData.std_csmGetParameterKeyCounts x			:	Live2DCubism4DLLData.csmGetParameterKeyCounts x		 	)
#define CALL_csmGetParameterKeyValues( x )			( Live2DCubism4DLLData.csmGetParameterKeyValues			== NULL ? Live2DCubism4DLLData.std_csmGetParameterKeyValues x			:	Live2DCubism4DLLData.csmGetParameterKeyValues x		 	)
#define CALL_csmGetPartCount( x )					( Live2DCubism4DLLData.csmGetPartCount					== NULL ? Live2DCubism4DLLData.std_csmGetPartCount x					:	Live2DCubism4DLLData.csmGetPartCount x					)
#define CALL_csmGetPartIds( x )						( Live2DCubism4DLLData.csmGetPartIds					== NULL ? Live2DCubism4DLLData.std_csmGetPartIds x				 		:	Live2DCubism4DLLData.csmGetPartIds x				 	)
#define CALL_csmGetPartOpacities( x )				( Live2DCubism4DLLData.csmGetPartOpacities				== NULL ? Live2DCubism4DLLData.std_csmGetPartOpacities x				:	Live2DCubism4DLLData.csmGetPartOpacities x				)
#define CALL_csmGetPartParentPartIndices( x )		( Live2DCubism4DLLData.csmGetPartParentPartIndices		== NULL ? Live2DCubism4DLLData.std_csmGetPartParentPartIndices x		:	Live2DCubism4DLLData.csmGetPartParentPartIndices x		)
#define CALL_csmGetDrawableCount( x )				( Live2DCubism4DLLData.csmGetDrawableCount				== NULL ? Live2DCubism4DLLData.std_csmGetDrawableCount x				:	Live2DCubism4DLLData.csmGetDrawableCount x				)
#define CALL_csmGetDrawableIds( x )					( Live2DCubism4DLLData.csmGetDrawableIds				== NULL ? Live2DCubism4DLLData.std_csmGetDrawableIds x			 		:	Live2DCubism4DLLData.csmGetDrawableIds x			 	)
#define CALL_csmGetDrawableConstantFlags( x )		( Live2DCubism4DLLData.csmGetDrawableConstantFlags		== NULL ? Live2DCubism4DLLData.std_csmGetDrawableConstantFlags x		:	Live2DCubism4DLLData.csmGetDrawableConstantFlags x		)
#define CALL_csmGetDrawableDynamicFlags( x )		( Live2DCubism4DLLData.csmGetDrawableDynamicFlags		== NULL ? Live2DCubism4DLLData.std_csmGetDrawableDynamicFlags x	 		:	Live2DCubism4DLLData.csmGetDrawableDynamicFlags x	 	)
#define CALL_csmGetDrawableTextureIndices( x )		( Live2DCubism4DLLData.csmGetDrawableTextureIndices		== NULL ? Live2DCubism4DLLData.std_csmGetDrawableTextureIndices x		:	Live2DCubism4DLLData.csmGetDrawableTextureIndices x		)
#define CALL_csmGetDrawableDrawOrders( x )			( Live2DCubism4DLLData.csmGetDrawableDrawOrders			== NULL ? Live2DCubism4DLLData.std_csmGetDrawableDrawOrders x			:	Live2DCubism4DLLData.csmGetDrawableDrawOrders x			)
#define CALL_csmGetDrawableRenderOrders( x )		( Live2DCubism4DLLData.csmGetDrawableRenderOrders		== NULL ? Live2DCubism4DLLData.std_csmGetDrawableRenderOrders x	 		:	Live2DCubism4DLLData.csmGetDrawableRenderOrders x	 	)
#define CALL_csmGetDrawableOpacities( x )			( Live2DCubism4DLLData.csmGetDrawableOpacities			== NULL ? Live2DCubism4DLLData.std_csmGetDrawableOpacities x			:	Live2DCubism4DLLData.csmGetDrawableOpacities x			)
#define CALL_csmGetDrawableMaskCounts( x )			( Live2DCubism4DLLData.csmGetDrawableMaskCounts			== NULL ? Live2DCubism4DLLData.std_csmGetDrawableMaskCounts x	 		:	Live2DCubism4DLLData.csmGetDrawableMaskCounts x	 		)
#define CALL_csmGetDrawableMasks( x )				( Live2DCubism4DLLData.csmGetDrawableMasks				== NULL ? Live2DCubism4DLLData.std_csmGetDrawableMasks x				:	Live2DCubism4DLLData.csmGetDrawableMasks x				)
#define CALL_csmGetDrawableVertexCounts( x )		( Live2DCubism4DLLData.csmGetDrawableVertexCounts		== NULL ? Live2DCubism4DLLData.std_csmGetDrawableVertexCounts x	 		:	Live2DCubism4DLLData.csmGetDrawableVertexCounts x	 	)
#define CALL_csmGetDrawableVertexPositions( x )		( Live2DCubism4DLLData.csmGetDrawableVertexPositions	== NULL ? Live2DCubism4DLLData.std_csmGetDrawableVertexPositions x 		:	Live2DCubism4DLLData.csmGetDrawableVertexPositions x 	)
#define CALL_csmGetDrawableVertexUvs( x )			( Live2DCubism4DLLData.csmGetDrawableVertexUvs			== NULL ? Live2DCubism4DLLData.std_csmGetDrawableVertexUvs x			:	Live2DCubism4DLLData.csmGetDrawableVertexUvs x			)
#define CALL_csmGetDrawableIndexCounts( x )			( Live2DCubism4DLLData.csmGetDrawableIndexCounts		== NULL ? Live2DCubism4DLLData.std_csmGetDrawableIndexCounts x	 		:	Live2DCubism4DLLData.csmGetDrawableIndexCounts x	 	)
#define CALL_csmGetDrawableIndices( x )				( Live2DCubism4DLLData.csmGetDrawableIndices			== NULL ? Live2DCubism4DLLData.std_csmGetDrawableIndices x		 		:	Live2DCubism4DLLData.csmGetDrawableIndices x		 	)
#define CALL_csmGetDrawableMultiplyColors( x )		( Live2DCubism4DLLData.csmGetDrawableMultiplyColors		== NULL ? Live2DCubism4DLLData.std_csmGetDrawableMultiplyColors x		:	Live2DCubism4DLLData.csmGetDrawableMultiplyColors x		)
#define CALL_csmGetDrawableScreenColors( x )		( Live2DCubism4DLLData.csmGetDrawableScreenColors		== NULL ? Live2DCubism4DLLData.std_csmGetDrawableScreenColors x		 	:	Live2DCubism4DLLData.csmGetDrawableScreenColors x		)
#define CALL_csmGetDrawableParentPartIndices( x )	( Live2DCubism4DLLData.csmGetDrawableParentPartIndices	== NULL ? Live2DCubism4DLLData.std_csmGetDrawableParentPartIndices x	:	Live2DCubism4DLLData.csmGetDrawableParentPartIndices x	)
#define CALL_csmResetDrawableDynamicFlags( x )		( Live2DCubism4DLLData.csmResetDrawableDynamicFlags		== NULL ? Live2DCubism4DLLData.std_csmResetDrawableDynamicFlags x	:	Live2DCubism4DLLData.csmResetDrawableDynamicFlags x		)
#else // WINDOWS_DESKTOP_OS
#define CALL_csmGetVersion( x )						( Live2DCubism4DLLData.csmGetVersion x				 		)
#define CALL_csmGetLatestMocVersion( x )			( Live2DCubism4DLLData.csmGetLatestMocVersion x		 		)
#define CALL_csmGetMocVersion( x )					( Live2DCubism4DLLData.csmGetMocVersion x					)
#define CALL_csmHasMocConsistency( x )				( Live2DCubism4DLLData.csmHasMocConsistency x				)
#define CALL_csmGetLogFunction( x )					( Live2DCubism4DLLData.csmGetLogFunction x			 		)
#define CALL_csmSetLogFunction( x )					( Live2DCubism4DLLData.csmSetLogFunction x			 		)
#define CALL_csmReviveMocInPlace( x )				( Live2DCubism4DLLData.csmReviveMocInPlace x				)
#define CALL_csmGetSizeofModel( x )					( Live2DCubism4DLLData.csmGetSizeofModel x			 		)
#define CALL_csmInitializeModelInPlace( x )			( Live2DCubism4DLLData.csmInitializeModelInPlace x	 		)
#define CALL_csmUpdateModel( x )					( Live2DCubism4DLLData.csmUpdateModel x				 		)
#define CALL_csmReadCanvasInfo( x )					( Live2DCubism4DLLData.csmReadCanvasInfo x			 		)
#define CALL_csmGetParameterCount( x )				( Live2DCubism4DLLData.csmGetParameterCount x				)
#define CALL_csmGetParameterIds( x )				( Live2DCubism4DLLData.csmGetParameterIds x			 		)
#define CALL_csmGetParameterTypes( x )				( Live2DCubism4DLLData.csmGetParameterTypes x				)
#define CALL_csmGetParameterMinimumValues( x )		( Live2DCubism4DLLData.csmGetParameterMinimumValues x		)
#define CALL_csmGetParameterMaximumValues( x )		( Live2DCubism4DLLData.csmGetParameterMaximumValues x		)
#define CALL_csmGetParameterDefaultValues( x )		( Live2DCubism4DLLData.csmGetParameterDefaultValues x		)
#define CALL_csmGetParameterValues( x )				( Live2DCubism4DLLData.csmGetParameterValues x		 		)
#define CALL_csmGetParameterKeyCounts( x )			( Live2DCubism4DLLData.csmGetParameterKeyCounts				)
#define CALL_csmGetParameterKeyValues( x )			( Live2DCubism4DLLData.csmGetParameterKeyValues				)
#define CALL_csmGetPartCount( x )					( Live2DCubism4DLLData.csmGetPartCount x					)
#define CALL_csmGetPartIds( x )						( Live2DCubism4DLLData.csmGetPartIds x				 		)
#define CALL_csmGetPartOpacities( x )				( Live2DCubism4DLLData.csmGetPartOpacities x				)
#define CALL_csmGetPartParentPartIndices( x )		( Live2DCubism4DLLData.csmGetPartParentPartIndices x		)
#define CALL_csmGetDrawableCount( x )				( Live2DCubism4DLLData.csmGetDrawableCount x				)
#define CALL_csmGetDrawableIds( x )					( Live2DCubism4DLLData.csmGetDrawableIds x			 		)
#define CALL_csmGetDrawableConstantFlags( x )		( Live2DCubism4DLLData.csmGetDrawableConstantFlags x		)
#define CALL_csmGetDrawableDynamicFlags( x )		( Live2DCubism4DLLData.csmGetDrawableDynamicFlags x	 		)
#define CALL_csmGetDrawableTextureIndices( x )		( Live2DCubism4DLLData.csmGetDrawableTextureIndices x		)
#define CALL_csmGetDrawableDrawOrders( x )			( Live2DCubism4DLLData.csmGetDrawableDrawOrders x			)
#define CALL_csmGetDrawableRenderOrders( x )		( Live2DCubism4DLLData.csmGetDrawableRenderOrders x	 		)
#define CALL_csmGetDrawableOpacities( x )			( Live2DCubism4DLLData.csmGetDrawableOpacities x			)
#define CALL_csmGetDrawableMaskCounts( x )			( Live2DCubism4DLLData.csmGetDrawableMaskCounts x	 		)
#define CALL_csmGetDrawableMasks( x )				( Live2DCubism4DLLData.csmGetDrawableMasks x				)
#define CALL_csmGetDrawableVertexCounts( x )		( Live2DCubism4DLLData.csmGetDrawableVertexCounts x	 		)
#define CALL_csmGetDrawableVertexPositions( x )		( Live2DCubism4DLLData.csmGetDrawableVertexPositions x 		)
#define CALL_csmGetDrawableVertexUvs( x )			( Live2DCubism4DLLData.csmGetDrawableVertexUvs x			)
#define CALL_csmGetDrawableIndexCounts( x )			( Live2DCubism4DLLData.csmGetDrawableIndexCounts x	 		)
#define CALL_csmGetDrawableIndices( x )				( Live2DCubism4DLLData.csmGetDrawableIndices x		 		)
#define CALL_csmGetDrawableMultiplyColors( x )		( Live2DCubism4DLLData.csmGetDrawableMultiplyColors x		)
#define CALL_csmGetDrawableScreenColors( x )		( Live2DCubism4DLLData.csmGetDrawableScreenColors x			)
#define CALL_csmGetDrawableParentPartIndices( x )	( Live2DCubism4DLLData.csmGetDrawableParentPartIndices x	)
#define CALL_csmResetDrawableDynamicFlags( x )		( Live2DCubism4DLLData.csmResetDrawableDynamicFlags x		)
#endif // WINDOWS_DESKTOP_OS

// 構造体定義 --------------------------------------------------------------------

// ファイルからLive2Dモデルハンドルを作成する処理に必要なグローバルデータを纏めたもの
struct LIVE2DCUBISM4_LOADMODEL_GPARAM
{
	int							Dummy ;
} ;

// Live2D Cubism4 モデル情報
struct LIVE2DCUBISM4MODEL
{
	HANDLEINFO					HandleInfo ;				// ハンドル共通データ

	float						PosX ;						// トランスレーションパラメータ
	float						PosY ;
	float						ExRateX ;
	float						ExRateY ;
	float						RotAngle ;

	int							LastPlayMotionNo ;			// 最後に再生したモーションのグループ内の番号

	D_LAppModel *				AppModel ;
} ;

// Live2D Cubism4 DLL の関数ポインタ構造体
struct LIVE2DCUBISM4DLL
{
	wchar_t					Live2DCubismCoreDLLPath[ 1024 ] ;	// Live2D Cubism Core DLL のファイルパス

	DWORD						( *csmGetVersion )( void ) ;
	DWORD						( *csmGetLatestMocVersion )( void ) ;
	DWORD						( *csmGetMocVersion )( const void* address, const DWORD size ) ;
	int							( *csmHasMocConsistency )( void* address, const unsigned int size ) ;
	csmLogFunctionP				( *csmGetLogFunction )( void ) ;
	void						( *csmSetLogFunction )( csmLogFunctionP handler ) ;
	void*						( *csmReviveMocInPlace )( void* address, const DWORD size ) ;
	DWORD 						( *csmGetSizeofModel )( const void* moc ) ;
	void* 						( *csmInitializeModelInPlace )( const void* moc, void* address, const DWORD size ) ;
	void 						( *csmUpdateModel )( void* model ) ;
	void 						( *csmReadCanvasInfo )( const void* model, D_CubismVector2* outSizeInPixels, D_CubismVector2* outOriginInPixels, float* outPixelsPerUnit ) ;
	int 						( *csmGetParameterCount )( const void* model ) ;
	const char** 				( *csmGetParameterIds )( const void* model ) ;
	const D_csmParameterType*	( *csmGetParameterTypes )( const void* model ) ;
	const float*				( *csmGetParameterMinimumValues )( const void* model ) ;
	const float*				( *csmGetParameterMaximumValues )( const void* model ) ;
	const float*				( *csmGetParameterDefaultValues )( const void* model ) ;
	float* 						( *csmGetParameterValues )( void* model ) ;
	const int*					( *csmGetParameterKeyCounts )( const void* model ) ;
	const float**				( *csmGetParameterKeyValues )( const void* model ) ;
	int 						( *csmGetPartCount )( const void* model ) ;
	const char**				( *csmGetPartIds )( const void* model ) ;
	float*						( *csmGetPartOpacities ) (void* model ) ;
	const int*					( *csmGetPartParentPartIndices )( const void* model ) ;
	int 						( *csmGetDrawableCount )( const void* model ) ;
	const char**				( *csmGetDrawableIds )( const void* model ) ;
	const BYTE*					( *csmGetDrawableConstantFlags )( const void* model ) ;
	const BYTE*					( *csmGetDrawableDynamicFlags )( const void* model ) ;
	const int*					( *csmGetDrawableTextureIndices )( const void* model ) ;
	const int*					( *csmGetDrawableDrawOrders )( const void* model ) ;
	const int*					( *csmGetDrawableRenderOrders )( const void* model ) ;
	const float*				( *csmGetDrawableOpacities )( const void* model ) ;
	const int*					( *csmGetDrawableMaskCounts )( const void* model ) ;
	const int**					( *csmGetDrawableMasks )( const void* model ) ;
	const int*					( *csmGetDrawableVertexCounts )( const void* model ) ;
	const D_CubismVector2**		( *csmGetDrawableVertexPositions )( const void* model ) ;
	const D_CubismVector2**		( *csmGetDrawableVertexUvs )( const void* model ) ;
	const int*					( *csmGetDrawableIndexCounts )( const void* model ) ;
	const unsigned short**		( *csmGetDrawableIndices )( const void* model ) ;
	const D_CubismVector4*		( *csmGetDrawableMultiplyColors )( const void* model ) ;
	const D_CubismVector4*		( *csmGetDrawableScreenColors )( const void* model ) ;
	const int*					( *csmGetDrawableParentPartIndices )( const void* model ) ;
	void						( *csmResetDrawableDynamicFlags )( void* model ) ;

#ifdef WINDOWS_DESKTOP_OS
	DWORD						( __stdcall *std_csmGetVersion )( void ) ;
	DWORD						( __stdcall *std_csmGetLatestMocVersion )( void ) ;
	DWORD						( __stdcall *std_csmGetMocVersion )( const void* address, const DWORD size ) ;
	int							( __stdcall *std_csmHasMocConsistency )( void* address, const unsigned int size ) ;
	csmLogFunctionP				( __stdcall *std_csmGetLogFunction )( void ) ;
	void						( __stdcall *std_csmSetLogFunction )( csmLogFunctionP handler ) ;
	void*						( __stdcall *std_csmReviveMocInPlace )( void* address, const DWORD size ) ;
	DWORD 						( __stdcall *std_csmGetSizeofModel )( const void* moc ) ;
	void* 						( __stdcall *std_csmInitializeModelInPlace )( const void* moc, void* address, const DWORD size ) ;
	void 						( __stdcall *std_csmUpdateModel )( void* model ) ;
	void 						( __stdcall *std_csmReadCanvasInfo )( const void* model, D_CubismVector2* outSizeInPixels, D_CubismVector2* outOriginInPixels, float* outPixelsPerUnit ) ;
	int 						( __stdcall *std_csmGetParameterCount )( const void* model ) ;
	const char** 				( __stdcall *std_csmGetParameterIds )( const void* model ) ;
	const D_csmParameterType*	( __stdcall *std_csmGetParameterTypes )( const void* model ) ;
	const float*				( __stdcall *std_csmGetParameterMinimumValues )( const void* model ) ;
	const float*				( __stdcall *std_csmGetParameterMaximumValues )( const void* model ) ;
	const float*				( __stdcall *std_csmGetParameterDefaultValues )( const void* model ) ;
	float* 						( __stdcall *std_csmGetParameterValues )( void* model ) ;
	const int*					( __stdcall *std_csmGetParameterKeyCounts )( const void* model ) ;
	const float**				( __stdcall *std_csmGetParameterKeyValues )( const void* model ) ;
	int 						( __stdcall *std_csmGetPartCount )( const void* model ) ;
	const char**				( __stdcall *std_csmGetPartIds )( const void* model ) ;
	float*						( __stdcall *std_csmGetPartOpacities ) (void* model ) ;
	const int*					( __stdcall *std_csmGetPartParentPartIndices )( const void* model ) ;
	int 						( __stdcall *std_csmGetDrawableCount )( const void* model ) ;
	const char**				( __stdcall *std_csmGetDrawableIds )( const void* model ) ;
	const BYTE*					( __stdcall *std_csmGetDrawableConstantFlags )( const void* model ) ;
	const BYTE*					( __stdcall *std_csmGetDrawableDynamicFlags )( const void* model ) ;
	const int*					( __stdcall *std_csmGetDrawableTextureIndices )( const void* model ) ;
	const int*					( __stdcall *std_csmGetDrawableDrawOrders )( const void* model ) ;
	const int*					( __stdcall *std_csmGetDrawableRenderOrders )( const void* model ) ;
	const float*				( __stdcall *std_csmGetDrawableOpacities )( const void* model ) ;
	const int*					( __stdcall *std_csmGetDrawableMaskCounts )( const void* model ) ;
	const int**					( __stdcall *std_csmGetDrawableMasks )( const void* model ) ;
	const int*					( __stdcall *std_csmGetDrawableVertexCounts )( const void* model ) ;
	const D_CubismVector2**		( __stdcall *std_csmGetDrawableVertexPositions )( const void* model ) ;
	const D_CubismVector2**		( __stdcall *std_csmGetDrawableVertexUvs )( const void* model ) ;
	const int*					( __stdcall *std_csmGetDrawableIndexCounts )( const void* model ) ;
	const unsigned short**		( __stdcall *std_csmGetDrawableIndices )( const void* model ) ;
	const D_CubismVector4*		( __stdcall *std_csmGetDrawableMultiplyColors )( const void* model ) ;
	const D_CubismVector4*		( __stdcall *std_csmGetDrawableScreenColors )( const void* model ) ;
	const int*					( __stdcall *std_csmGetDrawableParentPartIndices )( const void* model ) ;
	void						( __stdcall *std_csmResetDrawableDynamicFlags )( void* model ) ;
#endif // WINDOWS_DESKTOP_OS
} ;

// Live2D Cubism4 システム用データ構造体
struct LIVE2DCUBISM4SYSTEMDATA
{
	int						InitializeFlag ;						// 初期化フラグ

	int						EnableConstantBuffer ;					// コンスタントバッファが使用できるかどうか

	SCREENDRAWSETTINGINFO	ScreenDrawSettingInfo ;					// Live2D描画開始前の描画設定

	D_CubismIdManager *		s_cubismIdManager ;

	DXARC					ShaderBinDxa ;							// シェーダーオブジェクトファイルＤＸＡ構造体
	void					*ShaderBinDxaImage ;					// シェーダーオブジェクトファイルＤＸＡのバイナリイメージ

	int						NowDrawLive2DModelHandle ;				// 現在描画を行っている Live2Dモデルのハンドル

	int						UserShader[ DX_LIVE2D_SHADER_NUM ] ;	// ユーザーシェーダー

	void					( *DrawUserCallback )( int Live2DModelHandle, int TextureIndex, void *UserData ) ;		// 描画の前に呼ぶコールバック関数
	void					*DrawUserCallbackData ;					// 描画の前に呼ぶコールバック関数に渡すアドレス

	int						NotUseAutoScaling ;						// 画面サイズに応じた自動スケーリングを行なわないかどうか( TRUE:行なわない  FALSE:行う )
	int						NotUseAutoCentering ;					// 自動で画面の中心に描画するかどうか( TRUE:画面の中心に描画しない   FALSE:画面の中心に描画する )
	int						NotUseReverseYAxis ;					// Live2D_Model_SetTranslate で指定する平行移動値の y の向きを反転しないかどうか( TRUE:反転しない   FALSE:反転する )

	LIVE2DCUBISM4SYSTEMDATA_PF	PF ;								// 環境依存データ
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// Live2D Cubism4 システムデータ
extern LIVE2DCUBISM4SYSTEMDATA Live2DCubism4SysData ;

// Live2D Cubism 4 DLL
extern LIVE2DCUBISM4DLL Live2DCubism4DLLData ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// Live2D Cubism4 関連の初期化と後始末
extern	int		Live2DCubism4_Initialize( void ) ;					// Live2D Cubism4 の初期化
extern	int		Live2DCubism4_Terminate( void ) ;					// Live2D Cubism4 の後始末


extern	int		Live2DCubism4_SetupShaderCode( int *ShaderCodeBinConvert, BYTE *ShaderCodeBin ) ;	// シェーダーDXAファイルのセットアップ
extern	int		Live2DCubism4_LoadShaderCode( const char *ShaderName, int ShaderType ) ;			// 指定名のシェーダーを読み込む

extern	void	Live2DCubism4_InitLoadModelGParam( LIVE2DCUBISM4_LOADMODEL_GPARAM *GParam ) ;		// LIVE2DCUBISM4_LOADMODEL_GPARAM のデータをセットする

extern	int		Live2DCubism4_LoadModel_UseGParam( LIVE2DCUBISM4_LOADMODEL_GPARAM *GParam, const wchar_t *FilePath, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;				// Live2D_LoadModel のグローバル変数にアクセスしないバージョン




// wchar_t版関数
extern	int			Live2D_SetCubism4CoreDLLPath_WCHAR_T(			const wchar_t *CoreDLLFilePath ) ;
extern	int			Live2D_LoadModel_WCHAR_T(						const wchar_t *FilePath ) ;
extern	int			Live2D_Model_StartMotion_WCHAR_T(				int Live2DModelHandle, const wchar_t *group, int no, float fadeInSeconds, float fadeOutSeconds, int isLoopFadeIn, int isLoop ) ;
extern	int			Live2D_Model_SetExpression_WCHAR_T(				int Live2DModelHandle, const wchar_t *expressionID ) ;
extern	int			Live2D_Model_HitTest_WCHAR_T(					int Live2DModelHandle, const wchar_t *hitAreaName, float x, float y ) ;
extern	float		Live2D_Model_GetParameterValue_WCHAR_T(			int Live2DModelHandle, const wchar_t *parameterId ) ;
extern	int			Live2D_Model_SetParameterValue_WCHAR_T(			int Live2DModelHandle, const wchar_t *parameterId, float value ) ;
extern	int			Live2D_Model_GetMotionCount_WCHAR_T(			int Live2DModelHandle, const wchar_t *groupName ) ;
extern	const TCHAR *Live2D_Model_GetMotionFileName_WCHAR_T(		int Live2DModelHandle, const wchar_t *groupName, int index ) ;
extern	const TCHAR *Live2D_Model_GetMotionSoundFileName_WCHAR_T(	int Live2DModelHandle, const wchar_t *groupName, int index ) ;
extern	float		Live2D_Model_GetMotionFadeInTimeValue_WCHAR_T(	int Live2DModelHandle, const wchar_t *groupName, int index ) ;
extern	float		Live2D_Model_GetMotionFadeOutTimeValue_WCHAR_T(	int Live2DModelHandle, const wchar_t *groupName, int index ) ;



#ifndef DX_NON_NAMESPACE

}

#endif// DX_NON_NAMESPACE



// 環境依存関数
extern	int		Live2DCubism4_Initialize_PF( void ) ;										// Live2D Cubism4 関連の初期化する関数の環境依存処理を行う関数
extern	int		Live2DCubism4_Terminate_PF( void ) ;										// Live2D Cubism4 関連の後始末をする関数の環境依存処理を行う関数

extern	int		Live2DCubism4_GenerateShaders_PF( void ) ;									// Live2D Cubism4 のシェーダー作成の環境依存処理を行う
extern	int		Live2DCubism4_SetupShader_PF( int ConstantBufferHandle, D_CubismConstantBufferDxLib *ConstantBuffer, D_ShaderNames VertexShader, D_ShaderNames PixelShader ) ;	// Live2D Cubism4 のシェーダーのセットアップを行う
extern	int		Live2DCubism4_DrawAfter_PF( void ) ;										// Live2D Cubism4 の描画の後に呼ばれる環境依存関数



#endif// DX_NON_LIVE2D_CUBISM4

#endif// DXLIVE2D_CUBISM4_H

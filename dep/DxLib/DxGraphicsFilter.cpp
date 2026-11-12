//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		GraphFilter系プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "DxGraphicsFilter.h"

#ifndef DX_NON_FILTER

// インクルード ---------------------------------------------------------------
#include "DxGraphics.h"
#include "DxBaseFunc.h"
#include "DxBaseImage.h"

#ifdef WINDOWS_DESKTOP_OS
#include "Windows/DxGraphicsFilterWin.h"
#endif // WINDOWS_DESKTOP_OS

#ifdef __ANDROID__
#include "Android/DxGraphicsFilterAndroid.h"
#endif // __ANDROID__

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
		#include "iOS/DxGraphicsFilteriOS.h"
    #endif // TARGET_OS_IPHONE
#endif // __APPLE__




#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

// SSAOで使用するテーブル
float SSAO_Kernel[ /*32*//*18*/64 * 4 ] =
{
	// Test
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 
//	 0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,1.0f,   0.0f,0.0f,1.0f,1.0f, 

	// 64
#if 1
//	 0.079068f,-0.061033f,0.004829f,1.0f,   0.088802f, 0.007856f,0.045787f,1.0f,  -0.066347f, 0.075220f,0.010797f,1.0f,   0.087373f, 0.051257f,0.011747f,1.0f, 
//	 0.022086f, 0.090119f,0.045894f,1.0f,  -0.086149f,-0.047601f,0.037965f,1.0f,   0.069357f,-0.006099f,0.082444f,1.0f,   0.081398f, 0.052729f,0.053510f,1.0f, 
//	 0.100919f, 0.019009f,0.049642f,1.0f,   0.054793f,-0.067827f,0.079205f,1.0f,  -0.078364f,-0.032960f,0.087465f,1.0f,  -0.087768f,-0.013016f,0.090286f,1.0f, 
//	 0.089742f,-0.048213f,0.083374f,1.0f,  -0.085865f, 0.054050f,0.092257f,1.0f,   0.098048f, 0.008306f,0.103854f,1.0f,  -0.052445f,-0.104453f,0.093119f,1.0f, 
//	-0.131469f, 0.009649f,0.083886f,1.0f,   0.030284f,-0.048769f,0.153092f,1.0f,  -0.068839f,-0.128206f,0.090172f,1.0f,  -0.067695f,-0.106563f,0.127350f,1.0f, 
//	-0.092890f, 0.158142f,0.040810f,1.0f,  -0.026363f,-0.172454f,0.091290f,1.0f,   0.113523f, 0.104587f,0.136943f,1.0f,  -0.179878f, 0.098818f,0.068093f,1.0f, 
//	-0.075024f, 0.175141f,0.122587f,1.0f,  -0.062065f, 0.223030f,0.052257f,1.0f,  -0.096074f, 0.190451f,0.127545f,1.0f,   0.200913f,-0.041736f,0.159957f,1.0f, 
//	 0.117637f,-0.105415f,0.221761f,1.0f,   0.235148f,-0.120643f,0.106094f,1.0f,  -0.019841f, 0.274496f,0.113647f,1.0f,  -0.288735f,-0.005961f,0.115825f,1.0f, 
//	-0.078650f,-0.310732f,0.053709f,1.0f,   0.211926f, 0.138345f,0.225966f,1.0f,   0.016945f, 0.102317f,0.338471f,1.0f,   0.013567f,-0.274950f,0.245970f,1.0f, 
//	-0.213590f,-0.006966f,0.319961f,1.0f,   0.043073f,-0.312902f,0.246743f,1.0f,   0.105721f,-0.262788f,0.306418f,1.0f,   0.134798f,-0.287434f,0.296217f,1.0f, 
//	-0.379413f,-0.209961f,0.125979f,1.0f,   0.023023f,-0.273852f,0.380492f,1.0f,   0.253063f, 0.056610f,0.412924f,1.0f,  -0.337204f, 0.272495f,0.261446f,1.0f, 
//	 0.322091f, 0.027004f,0.414202f,1.0f,  -0.328004f,-0.427422f,0.081795f,1.0f,   0.177652f, 0.268093f,0.464462f,1.0f,   0.104768f,-0.420147f,0.393911f,1.0f, 
//	-0.318617f, 0.449787f,0.252415f,1.0f,   0.022194f, 0.327304f,0.534991f,1.0f,   0.054752f, 0.249678f,0.596888f,1.0f,   0.661276f, 0.113052f,0.029280f,1.0f, 
//	 0.433450f,-0.280157f,0.464181f,1.0f,   0.523474f, 0.315522f,0.375252f,1.0f,   0.445749f, 0.448290f,0.386023f,1.0f,   0.606867f, 0.092769f,0.455885f,1.0f, 
//	 0.688324f,-0.328488f,0.202300f,1.0f,  -0.284446f,-0.760966f,0.049400f,1.0f,   0.731907f, 0.194433f,0.361521f,1.0f,   0.840791f,-0.197655f,0.044729f,1.0f, 
//	 0.591202f,-0.281017f,0.604499f,1.0f,   0.368437f, 0.243613f,0.804302f,1.0f,  -0.511968f, 0.592588f,0.528254f,1.0f,   0.171697f, 0.331818f,0.897433f,1.0f, 
	-0.079099f,0.057714f,0.020306f,1.0f,
	-0.026312f,0.096703f,0.000505f,1.0f,
	0.071966f,-0.070681f,0.001288f,1.0f,
	-0.053523f,-0.057718f,0.064833f,1.0f,
	0.064712f,-0.080588f,0.005786f,1.0f,
	0.033722f,-0.052830f,0.084856f,1.0f,
	-0.065673f,0.073963f,0.043141f,1.0f,
	0.044412f,-0.099931f,0.017622f,1.0f,
	-0.010885f,-0.103967f,0.045635f,1.0f,
	0.077922f,-0.034647f,0.081266f,1.0f,
	-0.088051f,-0.001752f,0.084388f,1.0f,
	-0.087715f,-0.091073f,0.006010f,1.0f,
	0.076902f,0.048684f,0.095107f,1.0f,
	-0.067486f,0.079612f,0.088956f,1.0f,
	0.015905f,0.132110f,0.052554f,1.0f,
	-0.068279f,0.100856f,0.086590f,1.0f,
	-0.112232f,0.092746f,0.056712f,1.0f,
	-0.099363f,0.113554f,0.062967f,1.0f,
	-0.133694f,0.106921f,0.000659f,1.0f,
	-0.106566f,-0.062643f,0.129906f,1.0f,
	-0.013430f,-0.111159f,0.150885f,1.0f,
	0.145546f,0.122977f,0.049623f,1.0f,
	0.011064f,-0.015398f,0.205475f,1.0f,
	-0.098219f,0.076651f,0.176735f,1.0f,
	0.098395f,0.170603f,0.111998f,1.0f,
	0.007097f,-0.050355f,0.231817f,1.0f,
	-0.017557f,0.215829f,0.121981f,1.0f,
	-0.199463f,-0.159672f,0.049126f,1.0f,
	0.180786f,-0.126147f,0.159787f,1.0f,
	-0.093663f,0.215554f,0.160839f,1.0f,
	-0.291474f,0.004361f,0.060672f,1.0f,
	0.138985f,-0.213374f,0.178811f,1.0f,
	0.004501f,0.324601f,0.015450f,1.0f,
	0.207738f,-0.072312f,0.258318f,1.0f,
	-0.278626f,-0.156248f,0.152554f,1.0f,
	-0.059223f,0.124739f,0.342368f,1.0f,
	0.237070f,0.122049f,0.277392f,1.0f,
	0.267817f,0.218805f,0.202593f,1.0f,
	0.097056f,-0.385746f,0.126124f,1.0f,
	-0.378364f,0.082673f,0.196314f,1.0f,
	-0.384379f,0.236321f,0.017709f,1.0f,
	-0.335343f,-0.282067f,0.168173f,1.0f,
	-0.112523f,-0.180551f,0.438738f,1.0f,
	-0.195773f,-0.134521f,0.447091f,1.0f,
	0.140121f,-0.439026f,0.252305f,1.0f,
	-0.379424f,0.182610f,0.345916f,1.0f,
	0.455955f,0.246192f,0.225062f,1.0f,
	-0.398463f,0.065455f,0.423802f,1.0f,
	0.461951f,-0.046237f,0.389875f,1.0f,
	0.073274f,-0.618926f,0.073466f,1.0f,
	-0.199092f,-0.543999f,0.293325f,1.0f,
	0.162849f,0.361667f,0.541850f,1.0f,
	-0.305910f,-0.179970f,0.596541f,1.0f,
	-0.475366f,0.045428f,0.535123f,1.0f,
	0.221703f,-0.399602f,0.582954f,1.0f,
	-0.644313f,-0.288174f,0.294179f,1.0f,
	-0.728241f,0.197237f,0.231045f,1.0f,
	0.743003f,-0.252921f,0.215401f,1.0f,
	0.783607f,-0.295855f,0.051180f,1.0f,
	0.040123f,0.666639f,0.549527f,1.0f,
	-0.555305f,-0.126276f,0.685274f,1.0f,
	0.240279f,0.532035f,0.707954f,1.0f,
	-0.787613f,0.309148f,0.420021f,1.0f,
	-0.230385f,0.924516f,0.192770f,1.0f,
#endif

// 18
//	-0.006764,0.089016,0.045060,1.0f,
//	 0.078754,-0.004709,0.065871,1.0f,
//	 0.083185,0.008207,0.073203,1.0f,
//	-0.112828,0.026121,0.047037,1.0f,
//	 0.113017,-0.081257,0.038582,1.0f,
//	-0.115213,-0.051956,0.112863,1.0f,
//	-0.041668,0.098002,0.169291,1.0f,
//	 0.080895,-0.217625,0.042943,1.0f,
//	 0.196641,0.168555,0.100410,1.0f,
//	 0.251132,0.019389,0.205382,1.0f,
//	-0.292462,0.063575,0.230521,1.0f,
//	-0.378961,0.116118,0.181929,1.0f,
//	 0.407358,-0.289321,0.018788,1.0f,
//	-0.278269,0.288370,0.404569,1.0f,
//	 0.020318,-0.459381,0.451514,1.0f,
//	 0.390888,-0.360341,0.492935,1.0f,
//	-0.427792,-0.251549,0.641575,1.0f,
//	 0.565014,0.534988,0.457771,1.0f,

// 32
#if 0
	-0.063990f,0.054229f,0.054447f,1.0f,
	0.097591f,0.005415f,0.024964f,1.0f,
	-0.063190f,-0.033164f,0.074984f,1.0f,
	-0.069243f,-0.037253f,0.073907f,1.0f,
	0.078232f,0.054965f,0.062200f,1.0f,
	-0.024700f,-0.049494f,0.108709f,1.0f,
	-0.014299f,0.128157f,0.026470f,1.0f,
	-0.125487f,0.056316f,0.039363f,1.0f,
	0.071505f,0.076791f,0.115777f,1.0f,
	-0.060993f,-0.061909f,0.147491f,1.0f,
	-0.174131f,-0.065266f,0.026862f,1.0f,
	-0.042194f,-0.201548f,0.013325f,1.0f,
	-0.033302f,0.085941f,0.206968f,1.0f,
	-0.054703f,0.224604f,0.091271f,1.0f,
	0.089358f,0.180714f,0.182992f,1.0f,
	-0.158719f,0.179356f,0.176910f,1.0f,
	0.166529f,0.030741f,0.277395f,1.0f,
	0.250608f,-0.230862f,0.096005f,1.0f,
	0.157476f,0.187386f,0.296871f,1.0f,
	0.236033f,0.280546f,0.199272f,1.0f,
	-0.102363f,0.420644f,0.128411f,1.0f,
	-0.156400f,0.407878f,0.216624f,1.0f,
	-0.497085f,-0.041590f,0.164960f,1.0f,
	0.419587f,-0.378226f,0.007113f,1.0f,
	0.193081f,0.469978f,0.330725f,1.0f,
	-0.403121f,0.358757f,0.361108f,1.0f,
	-0.215141f,-0.366754f,0.548668f,1.0f,
	-0.664691f,-0.311057f,0.100497f,1.0f,
	0.230200f,0.568795f,0.496085f,1.0f,
	-0.171249f,0.467654f,0.675398f,1.0f,
	-0.300641f,-0.655962f,0.522722f,1.0f,
	-0.004219f,0.601655f,0.728228f,1.0f,
#endif
} ;


GRAPHFILTER_SHADER_HANDLE GraphFilterShaderHandle ;
GRAPHFILTER_SYSTEM_DATA GraphFilterSystemData ;

// 関数宣言 -------------------------------------------------------------------

static int	GraphFilter_GetSoftImage( int GrHandle, BASEIMAGE *BaseImage, RECT *GetRect = NULL ) ;
static int	GraphFilter_SoftImageSetup( GRAPHFILTER_INFO *Info, int DestDiscard = TRUE, int DestWidth = -1, int DestHeight = -1, RECT *SrcRect = NULL ) ;
static int	GraphFilter_SoftImageTerminate( GRAPHFILTER_INFO *Info ) ;

// プログラム -----------------------------------------------------------------

// フィルター処理の初期化
extern int GraphFilter_Initialize( void )
{
	// シェーダーハンドル値をリセットする
	_MEMSET( &GraphFilterShaderHandle, 0xff, sizeof( GraphFilterShaderHandle ) ) ;

	// 環境依存処理
	GraphFilter_Initialize_PF() ;

	// 終了
	return 0 ;
}

// フィルター処理の後始末
extern int GraphFilter_Terminate( void )
{
	// 環境依存処理
	GraphFilter_Terminate_PF() ;

	// 終了
	return 0 ;
}

// GraphFilterBlt や GraphBlendBlt の結果を転送先に転送する際のブレンドモードを設定する( 現状で対応しているのは DX_BLENDMODE_NOBLEND と DX_BLENDMODE_ALPHA のみ )
extern int NS_SetGraphFilterBltBlendMode( int BlendMode /* DX_BLENDMODE_ALPHA など */ )
{
	// 未対応のブレンドモードの場合はエラー
	if( BlendMode != DX_BLENDMODE_ALPHA && BlendMode != DX_BLENDMODE_NOBLEND )
	{
		return -1 ;
	}

	// ブレンドモードを保存
	GraphFilterSystemData.BltBlendMode = BlendMode ;

	// 終了
	return 0 ;
}

// GraphBlend で GrHandle と BlendGrHandle のサイズが異なる場合に適用される拡大フィルターモードを設定する
extern int NS_SetGraphBlendScalingFilterMode( int IsBilinearFilter )
{
	// フラグを保存
	GraphFilterSystemData.BlendGraphScalingFilterIsNotBilinear = IsBilinearFilter ? FALSE : TRUE ;

	// 終了
	return 0 ;
}

// 画像にフィルター処理を行う
extern int NS_GraphFilter( int GrHandle, int FilterType /* DX_GRAPH_FILTER_GAUSS_H 等 */ , ... )
{
	int Result ;
	va_list VaList ;
	int W, H ;

	va_start( VaList, FilterType ) ;

	NS_GetGraphSize( GrHandle, &W, &H ) ;

	Result = GraphFilter_RectBltBase( FALSE, GrHandle, -1, GrHandle, 0, FilterType, 0, 0, W, H, 0, 0, FALSE, 0, 0, FALSE, 0, 0, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}

// 画像のフィルター付き転送を行う
extern int NS_GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, ... )
{
	int Result ;
	va_list VaList ;
	int SrcW, SrcH ;

	va_start( VaList, FilterType ) ;

	NS_GetGraphSize( SrcGrHandle, &SrcW, &SrcH ) ;

	Result = GraphFilter_RectBltBase( FALSE, SrcGrHandle, -1, DestGrHandle, 0, FilterType, 0, 0, SrcW, SrcH, 0, 0, FALSE, 0, 0, FALSE, 0, 0, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}

// 画像のフィルター付き転送を行う( 矩形指定 )
extern int NS_GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, ... )
{
	int Result ;
	va_list VaList ;

	va_start( VaList, FilterType ) ;

	Result = GraphFilter_RectBltBase( FALSE, SrcGrHandle, -1, DestGrHandle, 0, FilterType, SrcX1, SrcY1, SrcX2, SrcY2, 0, 0, FALSE, 0, 0, FALSE, DestX, DestY, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}

// 二つの画像をブレンドする
extern	int	NS_GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, ... )
{
	int Result ;
	va_list VaList ;
	int W, H ;

	va_start( VaList, BlendType ) ;

	NS_GetGraphSize( GrHandle, &W, &H ) ;

	Result = GraphFilter_RectBltBase( TRUE, GrHandle, BlendGrHandle, GrHandle, BlendRatio, BlendType, 0, 0, W, H, 0, 0, FALSE, 0, 0, FALSE, 0, 0, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}

// 二つの画像をブレンドして結果を指定の画像に出力する
extern	int	NS_GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, ... )
{
	int Result ;
	va_list VaList ;
	int SrcW, SrcH ;

	va_start( VaList, BlendType ) ;

	NS_GetGraphSize( SrcGrHandle, &SrcW, &SrcH ) ;

	Result = GraphFilter_RectBltBase( TRUE, SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, 0, 0, SrcW, SrcH, 0, 0, FALSE, 0, 0, FALSE, 0, 0, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}

// 二つの画像をブレンドして結果を指定の画像に出力する( 矩形指定 )
extern	int	NS_GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, ... )
{
	int Result ;
	va_list VaList ;

	va_start( VaList, BlendType ) ;

	Result = GraphFilter_RectBltBase( TRUE, SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, TRUE, 0, 0, FALSE, DestX, DestY, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}

// 二つの画像をブレンドして結果を指定の画像に出力する( 矩形指定、ブレンド画像の矩形も指定 )
extern	int	NS_GraphBlendRectBlt2( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX1, int BlendY1, int BlendX2, int BlendY2, int DestX, int DestY, int BlendRatio, int BlendType, ... )
{
	int Result ;
	va_list VaList ;

	va_start( VaList, BlendType ) ;

	Result = GraphFilter_RectBltBase( TRUE, SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, SrcX1, SrcY1, SrcX2, SrcY2, BlendX1, BlendY1, TRUE, BlendX2, BlendY2, TRUE, DestX, DestY, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}


static int GraphFilter_GetSoftImage( int GrHandle, BASEIMAGE *BaseImage, RECT *GetRect )
{
	RECT TempRect ;
	int Width, Height ;
	BASEIMAGE SurfaceImage ;
	IMAGEDATA *Image = NULL ;
	IMAGEDATA_HARD_DRAW *HardDraw ;
	RECT SrcRect ;
	RECT DestRect ;
	int i ;
	int temp ;

	if( GrHandle != DX_SCREEN_BACK && GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	if( GetRect == NULL )
	{
		NS_GetGraphSize( GrHandle, &Width, &Height ) ;

		TempRect.left = 0 ;
		TempRect.right = Width ;
		TempRect.top = 0 ;
		TempRect.bottom = Height ;

		GetRect = &TempRect ;
	}
	else
	{
		Width = GetRect->right - GetRect->left ;
		Height = GetRect->bottom - GetRect->top ;
	}

	if( NS_CreateARGB8ColorBaseImage( Width, Height, BaseImage ) < 0 )
		return -1 ;

	if( GrHandle != DX_SCREEN_BACK && Image->Orig->FormatDesc.TextureFlag )
	{
		HardDraw = Image->Hard.Draw ;
		for( i = 0 ; i < Image->Hard.DrawNum ; i ++, HardDraw ++ )
		{
			SrcRect.left   = HardDraw->UsePosXI ;
			SrcRect.right  = HardDraw->UsePosXI + HardDraw->WidthI ;
			SrcRect.top    = HardDraw->UsePosYI ;
			SrcRect.bottom = HardDraw->UsePosYI + HardDraw->HeightI ;

			DestRect.left   = HardDraw->DrawPosXI ;
			DestRect.right  = HardDraw->DrawPosXI + HardDraw->WidthI ;
			DestRect.top    = HardDraw->DrawPosYI ;
			DestRect.bottom = HardDraw->DrawPosYI + HardDraw->HeightI ;

			if( DestRect.left   >= GetRect->right ||
				DestRect.right  <= GetRect->left ||
				DestRect.top    >= GetRect->bottom ||
				DestRect.bottom <= GetRect->top )
				continue ;

			if( DestRect.left < GetRect->left )
			{
				temp = GetRect->left - DestRect.left ;
				SrcRect.left += temp ;
				DestRect.left += temp ;
			}
			if( DestRect.right > GetRect->right )
			{
				temp = DestRect.right - GetRect->right ;
				SrcRect.right -= temp ;
				DestRect.right -= temp ;
			}
			if( DestRect.top < GetRect->top )
			{
				temp = GetRect->top - DestRect.top ;
				SrcRect.top += temp ;
				DestRect.top += temp ;
			}
			if( DestRect.bottom > GetRect->bottom )
			{
				temp = DestRect.bottom - GetRect->bottom ;
				SrcRect.bottom -= temp ;
				DestRect.bottom -= temp ;
			}

			if( Graphics_Screen_LockDrawScreen( &SrcRect, &SurfaceImage, GrHandle, 0, 0, TRUE, i ) < 0 )
			{
				NS_ReleaseBaseImage( BaseImage ) ;
				return -1 ;
			}

			NS_BltBaseImage( 0, 0, SrcRect.right - SrcRect.left, SrcRect.bottom - SrcRect.top, DestRect.left, DestRect.top, &SurfaceImage, BaseImage ) ;
			Graphics_Screen_UnlockDrawScreen() ;
		}
	}
	else
	{
		if( Graphics_Screen_LockDrawScreen( GetRect, &SurfaceImage, GrHandle, 0, 0, TRUE, 0 ) < 0 )
		{
			NS_ReleaseBaseImage( BaseImage ) ;
			return -1 ;
		}

		NS_BltBaseImage( 0, 0, Width, Height, 0, 0, &SurfaceImage, BaseImage ) ;
		Graphics_Screen_UnlockDrawScreen() ;
	}


	// 正常終了
	return 0 ;
}

static int	GraphFilter_SoftImageSetup( GRAPHFILTER_INFO *Info, int DestDiscard, int DestWidth, int DestHeight, RECT *SrcRect )
{
	RECT Rect ;
	int SrcWidth ;
	int SrcHeight ;

	if( SrcRect != NULL )
	{
		SrcWidth  = SrcRect->right  - SrcRect->left ;
		SrcHeight = SrcRect->bottom - SrcRect->top ;
	}
	else
	{
		SrcWidth = Info->SrcX2 - Info->SrcX1 ;
		SrcHeight = Info->SrcY2 - Info->SrcY1 ;
	}

	if( DestWidth < 0 || DestHeight < 0 )
	{
		DestWidth = SrcWidth ;
		DestHeight = SrcHeight ;
	}
	Info->DestWidth = DestWidth ;
	Info->DestHeight = DestHeight ;

	Info->SrcBaseImage.GraphData   = NULL ;
	Info->BlendBaseImage.GraphData = NULL ;
	Info->DestBaseImage.GraphData  = NULL ;

	if( SrcRect != NULL )
	{
		Rect = *SrcRect ;
	}
	else
	{
		Rect.left   = Info->SrcX1 ;
		Rect.right  = Info->SrcX2 ;
		Rect.top    = Info->SrcY1 ;
		Rect.bottom = Info->SrcY2 ;
	}

	if( GraphFilter_GetSoftImage( Info->SrcGrHandle, &Info->SrcBaseImage, &Rect ) < 0 )
		goto ERR ;

	if( Info->IsBlend )
	{
		if( GraphFilter_GetSoftImage( Info->BlendGrHandle, &Info->BlendBaseImage, &Rect ) < 0 )
			goto ERR ;
	}

	if( DestDiscard )
	{
		if( NS_CreateARGB8ColorBaseImage( Info->DestWidth, Info->DestHeight, &Info->DestBaseImage ) < 0 )
			goto ERR ;
	}
	else
	{
		Rect.left   = Info->DestX ;
		Rect.right  = Info->DestX + Info->DestWidth ;
		Rect.top    = Info->DestY ;
		Rect.bottom = Info->DestY + Info->DestHeight ;
		if( GraphFilter_GetSoftImage( Info->DestGrHandle, &Info->DestBaseImage, &Rect ) < 0 )
			goto ERR ;
	}

	// 正常終了
	return 0 ;

ERR :
	if( Info->SrcBaseImage.GraphData )
		NS_ReleaseBaseImage( &Info->SrcBaseImage ) ;

	if( Info->BlendBaseImage.GraphData )
		NS_ReleaseBaseImage( &Info->BlendBaseImage ) ;

	if( Info->DestBaseImage.GraphData )
		NS_ReleaseBaseImage( &Info->DestBaseImage ) ;

	return -1 ;
}

static int	GraphFilter_SoftImageTerminate( GRAPHFILTER_INFO *Info )
{
	if( Info->SrcBaseImage.GraphData )
		NS_ReleaseBaseImage( &Info->SrcBaseImage ) ;

	if( Info->BlendBaseImage.GraphData )
		NS_ReleaseBaseImage( &Info->BlendBaseImage ) ;

	if( Info->DestBaseImage.GraphData )
	{
		if( Info->DestGrHandle == DX_SCREEN_BACK )
		{
			int BlendMode, BlendParam ;
			NS_GetDrawBlendMode( &BlendMode, &BlendParam ) ;
			NS_SetDrawBlendMode( DX_BLENDMODE_SRCCOLOR, 255 ) ;
			NS_DrawBaseImage( Info->DestX, Info->DestY, &Info->DestBaseImage ) ;
			NS_SetDrawBlendMode( BlendMode, BlendParam ) ;
		}
		else
		{
			RECT Rect ;

			if( Info->SrcEqualDestClearFlag && Info->SrcGrHandle == Info->DestGrHandle && Info->Pass == 0 )
			{
				BASEIMAGE TempImage ;

				if( NS_CreateARGB8ColorBaseImage( Info->SrcX2 - Info->SrcX1, Info->SrcY2 - Info->SrcY1, &TempImage ) == 0 )
				{
					NS_FillBaseImage( &TempImage, 0, 0, 0, 0 ) ;
					NS_BltBmpOrGraphImageToGraph( NULL, NULL, NULL, FALSE, &TempImage, NULL, Info->SrcX1, Info->SrcY1, Info->SrcGrHandle ) ;
					NS_ReleaseBaseImage( &TempImage ) ;
				}
			}

			Rect.left   = 0 ;
			Rect.top    = 0 ;
			Rect.right  = Info->DestWidth ;
			Rect.bottom = Info->DestHeight ;
			Graphics_Image_BltBmpOrBaseImageToGraph3(
				&Rect,
				Info->DestX, Info->DestY,
				Info->DestGrHandle,
				&Info->DestBaseImage,
				NULL,
				FALSE,
				FALSE,
				FALSE
			) ;
	/*
			if( Info->UseTempDestImage )
			{
				if( Info->TempDestImage.GraphData )
				{
					NS_BltBaseImage( 0, 0, Info->DestWidth, Info->DestHeight, 0, 0, &Info->TempDestImage, &Info->DestBaseImage ) ;
				}
			}

			Graphics_Screen_UnlockDrawScreen() ;
	*/
		}
	}

	if( Info->DestBaseImage.GraphData )
		NS_ReleaseBaseImage( &Info->DestBaseImage ) ;

	return 0 ;
}

extern int GraphFilter_RectBltBase(
   int IsBlend,
   int SrcGrHandle, int BlendGrHandle, int DestGrHandle,
   int BlendRatio,
   int FilterOrBlendType,
   int SrcX1, int SrcY1,
   int SrcX2, int SrcY2,
   int BlendX, int BlendY, int BlendPosEnable,
   int BlendX2, int BlendY2, int BlendPos2Enable,
   int DestX, int DestY,
   va_list ParamList )
{
	SHADOWMAPDATA *                     SrcShadowMap       = NULL ;
	SHADOWMAPDATA *                     DestShadowMap      = NULL ;
	IMAGEDATA *                         SrcImage           = NULL ;
	IMAGEDATA *                         DestImage          = NULL ;
	IMAGEDATA *                         BlendImage         = NULL ;
	IMAGEDATA *                         TargetScreenImage  = NULL ;
	int                                 UseDestGrHandle    = -1 ;
	int                                 UseSrcGrHandle ;
	int                                 SrcImageWidth ;
	int                                 SrcImageHeight ;
	int                                 DestImageWidth ;
	int                                 DestImageHeight ;
	double                              BlendScaleX = 1.0 ;
	double                              BlendScaleY = 1.0 ;
	int                                 Width ;
	int                                 Height ;
	int                                 FilterResult       = -1 ;
	GRAPHFILTER_INFO                    Info ;
	GRAPHFILTER_PARAM					Param ;
	float                               BlendRatioF ;
	int									TempR = 0 ;
	int									TempG = 0 ;
	int									TempB = 0 ;
	int									TempA = 0 ;
	int									IsPMA = 0 ;

	// エラー判定
	if( !GRAPHCHK( SrcGrHandle, SrcImage ) )
	{
		SrcImageWidth  = SrcImage->WidthI ;
		SrcImageHeight = SrcImage->HeightI ;

#ifndef DX_NON_MOVIE
		if( SrcImage->Orig->MovieHandle != -1 )
			UpdateMovie( SrcImage->Orig->MovieHandle, FALSE ) ;
#endif
	}
	else
	if( !SHADOWMAPCHK( SrcGrHandle, SrcShadowMap ) )
	{
		SrcImageWidth = SrcShadowMap->BaseSizeX ;
		SrcImageHeight = SrcShadowMap->BaseSizeY ;
	}
	else
	{
		return -1 ;
	}

	if( DestGrHandle == DX_SCREEN_BACK )
	{
		NS_GetDrawScreenSize( &DestImageWidth, &DestImageHeight ) ;
	}
	else
	if( !GRAPHCHK(     DestGrHandle, DestImage    ) )
	{
		DestImageWidth = DestImage->WidthI ;
		DestImageHeight = DestImage->HeightI ;
	}
	else
	if( !SHADOWMAPCHK( DestGrHandle, DestShadowMap ) )
	{
		DestImageWidth = DestShadowMap->BaseSizeX ;
		DestImageHeight = DestShadowMap->BaseSizeY ;
	}
	else
	{
		return -1 ;
	}

	if( Graphics_Image_CheckMultiSampleDrawValid( GSYS.DrawSetting.TargetScreen[ 0 ] ) )
	{
		GRAPHCHK( GSYS.DrawSetting.TargetScreen[ 0 ], TargetScreenImage ) ;
	}

	if( SrcX2 <= SrcX1 ||
		SrcY2 <= SrcY1 ) return -1 ;

	if( IsBlend )
	{
		if( GRAPHCHK( BlendGrHandle, BlendImage ) )
			return -1 ;

		if( FilterOrBlendType < 0 || FilterOrBlendType >= DX_GRAPH_BLEND_NUM )
			return -1;

#ifndef DX_NON_MOVIE
		if( BlendImage->Orig->MovieHandle != -1 )
			UpdateMovie( BlendImage->Orig->MovieHandle, FALSE ) ;
#endif

		if( BlendPosEnable && BlendPos2Enable )
		{
			BlendScaleX = ( double )( BlendX2 - BlendX ) / ( SrcX2 - SrcX1 ) ;
			BlendScaleY = ( double )( BlendY2 - BlendY ) / ( SrcY2 - SrcY1 ) ;
		}
	}
	else
	{
		if( FilterOrBlendType < 0 || FilterOrBlendType >= DX_GRAPH_FILTER_NUM )
			return -1;
	}

	// 値の補正
	if( BlendRatio < 0 )
	{
		BlendRatio = 0 ;
	}
	else
	if( BlendRatio > 255 )
	{
		BlendRatio = 255 ;
	}
	BlendRatioF = BlendRatio / 255.0f ;

	// 座標の補正
	if( SrcX1 < 0 )
	{
		DestX += -SrcX1 ;
		if( BlendPosEnable )
		{
			if( BlendPos2Enable )
			{
				BlendX += _DTOL( -SrcX1 * BlendScaleX ) ;
			}
			else
			{
				BlendX += -SrcX1 ;
			}
		}
		SrcX1 = 0 ;
	}
	if( SrcY1 < 0 )
	{
		DestY += -SrcY1 ;
		if( BlendPosEnable )
		{
			if( BlendPos2Enable )
			{
				BlendY += _DTOL( -SrcY1 * BlendScaleY ) ;
			}
			else
			{
				BlendY += -SrcY1 ;
			}
		}
		SrcY1 = 0 ;
	}
	if( DestX < 0 )
	{
		SrcX1 += -DestX ;
		if( BlendPosEnable )
		{
			if( BlendPos2Enable )
			{
				BlendX += _DTOL( -DestX * BlendScaleX ) ;
			}
			else
			{
				BlendX += -DestX ;
			}
		}
		DestX = 0 ;
	}
	if( DestY < 0 )
	{
		SrcY1 += -DestY ;
		if( BlendPosEnable )
		{
			if( BlendPos2Enable )
			{
				BlendY += _DTOL( -DestY * BlendScaleY ) ;
			}
			else
			{
				BlendY += -DestY ;
			}
		}
		DestY = 0 ;
	}
	if( SrcX2 > SrcImageWidth )
	{
		if( BlendPosEnable )
		{
			if( BlendPos2Enable )
			{
				BlendX2 -= _DTOL( ( SrcX2 - SrcImageWidth ) * BlendScaleX ) ;
			}
			else
			{
				BlendX2 -= SrcX2 - SrcImageWidth ;
			}
		}
		SrcX2 = SrcImageWidth ;
	}
	if( SrcY2 > SrcImageHeight )
	{
		if( BlendPosEnable )
		{
			if( BlendPos2Enable )
			{
				BlendY2 -= _DTOL( ( SrcY2 - SrcImageHeight ) * BlendScaleY ) ;
			}
			else
			{
				BlendY2 -= SrcY2 - SrcImageHeight ;
			}
		}
		SrcY2 = SrcImageHeight ;
	}
	if( SrcX2 <= SrcX1 ||
		SrcY2 <= SrcY1 ) return 0 ;
	if( SrcX1 >= SrcImageWidth ||
		SrcY1 >= SrcImageHeight ||
		SrcX2 <= 0 ||
		SrcY2 <= 0 ||
		DestX >= DestImageWidth ||
		DestY >= DestImageHeight )
		return 0 ;

	Width  = SrcX2 - SrcX1 ;
	Height = SrcY2 - SrcY1 ;
	if( Width <= 0 ||
		Height <= 0 )
		return 0 ;

	_MEMSET( &Param, 0, sizeof( Param ) ) ;

	Info.SrcEqualDestClearFlag = FALSE ;
	Info.SrcDivNum = 1 ;
	if( IsBlend == FALSE && FilterOrBlendType == DX_GRAPH_FILTER_DOWN_SCALE )
	{
		Info.SrcDivNum = va_arg( ParamList, int ) ;

		// パラメータを補正
		if( Info.SrcDivNum <= 1 )
		{
			Info.SrcDivNum = 1 ;
		}
		else
		if( Info.SrcDivNum <= 2 )
		{
			Info.SrcDivNum = 2 ;
		}
		else
		if( Info.SrcDivNum <= 4 )
		{
			Info.SrcDivNum = 4 ;
		}
		else
		{
			Info.SrcDivNum = 8 ;
		}

		if( DestX + Width / Info.SrcDivNum > DestImageWidth )
		{
			Width = ( DestImageWidth - DestX ) * Info.SrcDivNum ;
			SrcX2 = SrcX1 + Width ;
		}
		if( DestY + Height / Info.SrcDivNum > DestImageHeight )
		{
			Height = ( DestImageHeight - DestY ) * Info.SrcDivNum ;
			SrcY2 = SrcY1 + Height ;
		}
	}
	else
	{
		if( DestX + Width > DestImageWidth )
		{
			Width = DestImageWidth - DestX ;
			SrcX2 = SrcX1 + Width ;
		}
		if( DestY + Height > DestImageHeight )
		{
			Height = DestImageHeight - DestY ;
			SrcY2 = SrcY1 + Height ;
		}
	}
	if( Width <= 0 ||
		Height <= 0 )
		return 0 ;
	
	// 情報のセット
	Info.BlendGraphScalingFilterIsBilinear = GraphFilterSystemData.BlendGraphScalingFilterIsNotBilinear ? FALSE : TRUE ;
	Info.BltBlendMode      = SrcGrHandle == DestGrHandle ? DX_BLENDMODE_NOBLEND : GraphFilterSystemData.BltBlendMode ;
	Info.IsBlend           = IsBlend ;
	Info.FilterOrBlendType = FilterOrBlendType ;
	Info.BlendGrHandle     = BlendGrHandle ;
	Info.BlendRatio        = BlendRatioF ;
	Info.SrcX1             = SrcX1 ;
	Info.SrcY1             = SrcY1 ;
	Info.SrcX2             = SrcX2 ;
	Info.SrcY2             = SrcY2 ;
	Info.BlendX            = BlendX ;
	Info.BlendY            = BlendY ;
	Info.BlendPosEnable    = BlendPosEnable ;
	Info.BlendX2           = BlendX2 ;
	Info.BlendY2           = BlendY2 ;
	Info.BlendPos2Enable   = BlendPos2Enable ;
	Info.DestX             = DestX ;
	Info.DestY             = DestY ;

	Info.SrcImage          = SrcImage ;
	Info.DestImage         = DestImage ;
	Info.BlendImage        = BlendImage ;
	Info.TargetScreenImage = TargetScreenImage ;

	// フィルター毎の引数を取得
	if( IsBlend )
	{
		switch( FilterOrBlendType )
		{
		case DX_GRAPH_BLEND_PMA_RGBA_SELECT_MIX :
			IsPMA = 1 ;
		case DX_GRAPH_BLEND_RGBA_SELECT_MIX :
			Param.RGBA_R = va_arg( ParamList, int ) ;
			Param.RGBA_G = va_arg( ParamList, int ) ;
			Param.RGBA_B = va_arg( ParamList, int ) ;
			Param.RGBA_A = va_arg( ParamList, int ) ;
			break ;

		case DX_GRAPH_BLEND_PMA_NORMAL :
		case DX_GRAPH_BLEND_PMA_MULTIPLE :
		case DX_GRAPH_BLEND_PMA_DIFFERENCE :
		case DX_GRAPH_BLEND_PMA_ADD :
		case DX_GRAPH_BLEND_PMA_SCREEN :
		case DX_GRAPH_BLEND_PMA_OVERLAY :
		case DX_GRAPH_BLEND_PMA_DODGE :
		case DX_GRAPH_BLEND_PMA_BURN :
		case DX_GRAPH_BLEND_PMA_DARKEN :
		case DX_GRAPH_BLEND_PMA_LIGHTEN :
		case DX_GRAPH_BLEND_PMA_SOFTLIGHT :
		case DX_GRAPH_BLEND_PMA_HARDLIGHT :
		case DX_GRAPH_BLEND_PMA_EXCLUSION :
		case DX_GRAPH_BLEND_PMA_NORMAL_ALPHACH :
		case DX_GRAPH_BLEND_PMA_ADD_ALPHACH :
		case DX_GRAPH_BLEND_PMA_MULTIPLE_A_ONLY :
			IsPMA = 1 ;
			break ;
		}
	}
	else
	{
		switch( FilterOrBlendType )
		{
		case DX_GRAPH_FILTER_MONO :
			Param.Mono_Cb = ( float )va_arg( ParamList, int ) * 100.0f / 255.0f ;
			Param.Mono_Cr = ( float )va_arg( ParamList, int ) * 100.0f / 255.0f ;
			break ;

		case DX_GRAPH_FILTER_GAUSS :
			Param.Gauss_PixelWidth =          va_arg( ParamList, int ) ;
			Param.Gauss_Param      = ( float )va_arg( ParamList, int ) / 100.0f ;
			break ;

		case DX_GRAPH_FILTER_DOWN_SCALE :
			Info.SrcEqualDestClearFlag = TRUE ;
			break ;

		case DX_GRAPH_FILTER_PMA_BRIGHT_CLIP :
			IsPMA = 1 ;
		case DX_GRAPH_FILTER_BRIGHT_CLIP :
			Param.Bright_Clip_CmpType      =          va_arg( ParamList, int ) ;
			Param.Bright_Clip_CmpParam     = ( float )va_arg( ParamList, int ) ;
			Param.Bright_Clip_ClipFillFlag =          va_arg( ParamList, int ) ;
			if( Param.Bright_Clip_ClipFillFlag )
			{
				NS_GetColor2( va_arg( ParamList, unsigned int ), &TempR, &TempG, &TempB ) ;
				TempA                           = va_arg( ParamList, int ) ;
				Param.Bright_Clip_ClipFillColor = NS_GetColorF( TempR / 255.0f, TempG / 255.0f, TempB / 255.0f, TempA / 255.0f ) ;
			}
			break ;

		case DX_GRAPH_FILTER_PMA_BRIGHT_SCALE :
			IsPMA = 1 ;
		case DX_GRAPH_FILTER_BRIGHT_SCALE :
			Param.Bright_Scale_Min = va_arg( ParamList, int ) ;
			Param.Bright_Scale_Max = va_arg( ParamList, int ) ;
			break ;

		case DX_GRAPH_FILTER_PMA_HSB :
			IsPMA = 1 ;
		case DX_GRAPH_FILTER_HSB :
			Param.Hsb_HueType    =          va_arg( ParamList, int ) ;
			Param.Hsb_Hue        = ( float )va_arg( ParamList, int ) ;
			Param.Hsb_Saturation = ( float )va_arg( ParamList, int ) * 100.0f / 255.0f ;
			Param.Hsb_Bright     = ( float )va_arg( ParamList, int ) * 100.0f / 255.0f ;
			break ;

		case DX_GRAPH_FILTER_PMA_INVERT :
			IsPMA = 1 ;
			break ;

		case DX_GRAPH_FILTER_PMA_LEVEL :
			IsPMA = 1 ;
		case DX_GRAPH_FILTER_LEVEL :
			Param.Lv_Min      = ( float )va_arg( ParamList, int ) ;
			Param.Lv_Max      = ( float )va_arg( ParamList, int ) ;
			Param.Lv_Gamma    = ( float )va_arg( ParamList, int ) / 100.0f ;
			Param.Lv_AfterMin = ( float )va_arg( ParamList, int ) ;
			Param.Lv_AfterMax = ( float )va_arg( ParamList, int ) ;
			break ;

		case DX_GRAPH_FILTER_PMA_TWO_COLOR :
			IsPMA = 1 ;
		case DX_GRAPH_FILTER_TWO_COLOR :
			Param.TC_Threshold = ( float )va_arg( ParamList, int ) ;

			NS_GetColor2( va_arg( ParamList, DWORD ), &TempR, &TempG, &TempB ) ;
			TempA             = va_arg( ParamList, int ) ;
			Param.TC_LowColor = NS_GetColorF( TempR / 255.0f, TempG / 255.0f, TempB / 255.0f, TempA / 255.0f ) ;

			NS_GetColor2( va_arg( ParamList, DWORD ), &TempR, &TempG, &TempB ) ;
			TempA              = va_arg( ParamList, int ) ;
			Param.TC_HighColor = NS_GetColorF( TempR / 255.0f, TempG / 255.0f, TempB / 255.0f, TempA / 255.0f ) ;
			break ;

		case DX_GRAPH_FILTER_PMA_GRADIENT_MAP :
			IsPMA = 1 ;
		case DX_GRAPH_FILTER_GRADIENT_MAP :
			Param.GM_MapGrHandle = va_arg( ParamList, int ) ;
			Param.GM_Reverse     = va_arg( ParamList, int ) ;
			break ;

		case DX_GRAPH_FILTER_PMA_REPLACEMENT:
			IsPMA = 1 ;
		case DX_GRAPH_FILTER_REPLACEMENT:
			Param.TargetColor.r = ( BYTE )va_arg( ParamList, int ) ;
			Param.TargetColor.g = ( BYTE )va_arg( ParamList, int ) ;
			Param.TargetColor.b = ( BYTE )va_arg( ParamList, int ) ;
			Param.TargetColor.a = ( BYTE )va_arg( ParamList, int ) ;
			Param.NextColor.r = ( BYTE )va_arg( ParamList, int ) ;
			Param.NextColor.g = ( BYTE )va_arg( ParamList, int ) ;
			Param.NextColor.b = ( BYTE )va_arg( ParamList, int ) ;
			Param.NextColor.a = ( BYTE )va_arg( ParamList, int ) ;
			break ;

		case DX_GRAPH_FILTER_PREMUL_ALPHA :
			break ;

		case DX_GRAPH_FILTER_INTERP_ALPHA :
			break ;

		case DX_GRAPH_FILTER_YUV_TO_RGB :
		case DX_GRAPH_FILTER_YUV_TO_RGB_RRA :
			Param.UVGrHandle     = -1 ;
			break ;

		case DX_GRAPH_FILTER_Y2UV1_TO_RGB :
		case DX_GRAPH_FILTER_Y2UV1_TO_RGB_RRA :
			{
				int YImgWidth, YImgHeight ;
				int UVImgWidth, UVImgHeight ;

				Param.UVGrHandle = va_arg( ParamList, int ) ;

				NS_GetGraphSize( SrcGrHandle,      &YImgWidth,  &YImgHeight  ) ;
				NS_GetGraphSize( Param.UVGrHandle, &UVImgWidth, &UVImgHeight ) ;

				if( ( YImgWidth  != UVImgWidth  && YImgWidth  / 2 != UVImgWidth  && YImgWidth  / 4 != UVImgWidth  ) ||
					( YImgHeight != UVImgHeight && YImgHeight / 2 != UVImgHeight && YImgHeight / 4 != UVImgHeight ) )
				{
					return -1 ;
				}
			}
			break ;

		case DX_GRAPH_FILTER_BICUBIC_SCALE:
		case DX_GRAPH_FILTER_LANCZOS3_SCALE:
			Param.DestSizeX = va_arg( ParamList, int ) ;
			Param.DestSizeY = va_arg( ParamList, int ) ;
			break;

		case DX_GRAPH_FILTER_SSAO:
			Param.DepthMapGrHandle    = va_arg( ParamList, int ) ;
			Param.SSAO_KernelRadius   = ( float )va_arg( ParamList, double ) ;
			Param.SSAO_MinDistance    = ( float )va_arg( ParamList, double ) ;
			Param.SSAO_MaxDistance    = ( float )va_arg( ParamList, double ) ;
			Param.SSAO_Strength       = ( float )va_arg( ParamList, double ) ;
			Param.SSAO_OcclusionColor = va_arg( ParamList, int ) ;
			Param.SSAO_OcclusionPower = ( float )va_arg( ParamList, double ) ;
			Param.SSAO_ColorMapHandle = Param.DepthMapGrHandle ;
			break ;

		case DX_GRAPH_FILTER_FLOAT_COLOR_SCALE:
			Param.ColorScale.r  = ( float )va_arg( ParamList, double ) ;
			Param.ColorScale.g  = ( float )va_arg( ParamList, double ) ;
			Param.ColorScale.b  = ( float )va_arg( ParamList, double ) ;
			Param.ColorScale.a  = ( float )va_arg( ParamList, double ) ;
			Param.ColorPreSub.r = ( float )va_arg( ParamList, double ) ;
			Param.ColorPreSub.g = ( float )va_arg( ParamList, double ) ;
			Param.ColorPreSub.b = ( float )va_arg( ParamList, double ) ;
			Param.ColorPreSub.a = ( float )va_arg( ParamList, double ) ;
			break ;
		}
	}

//	GSYS.HardInfo.UseShader = FALSE ;

	GraphFilter_RectBltBase_Timing0_PF( &Info, &Param ) ;

	// ループ処理の開始
	UseSrcGrHandle = SrcGrHandle ;
	for( Info.Pass = 0 ; Info.Pass < Info.PassNum ; Info.Pass ++ )
	{
		// 出力先のセットアップ
		Info.SrcGrHandle  = SrcGrHandle ;
		Info.DestGrHandle = DestGrHandle ;
		if( GraphFilter_DestGraphSetup_PF( &Info, &UseSrcGrHandle, &UseDestGrHandle ) < 0 )
			break ;

		DestImage     = NULL ;
		DestShadowMap = NULL ;
		if( UseDestGrHandle != DX_SCREEN_BACK &&
			GRAPHCHK(     UseDestGrHandle, DestImage ) &&
			SHADOWMAPCHK( UseDestGrHandle, DestShadowMap  ) )
			break ;

		// 情報のセット
		Info.SrcGrHandle  = UseSrcGrHandle ;
		Info.DestGrHandle = UseDestGrHandle ;

		if( IsBlend )
		{
			// ブレンドタイプによって処理を分岐
			switch( FilterOrBlendType )
			{
			case DX_GRAPH_BLEND_PMA_RGBA_SELECT_MIX :
			case DX_GRAPH_BLEND_RGBA_SELECT_MIX :
				FilterResult = GraphBlend_RGBA_Select_Mix( &Info, Param.RGBA_R, Param.RGBA_G, Param.RGBA_B, Param.RGBA_A, IsPMA ) ; 
				break ;

			case DX_GRAPH_BLEND_PMA_NORMAL :
			case DX_GRAPH_BLEND_PMA_MULTIPLE :
			case DX_GRAPH_BLEND_PMA_DIFFERENCE :
			case DX_GRAPH_BLEND_PMA_ADD :
			case DX_GRAPH_BLEND_PMA_SCREEN :
			case DX_GRAPH_BLEND_PMA_OVERLAY :
			case DX_GRAPH_BLEND_PMA_DODGE :
			case DX_GRAPH_BLEND_PMA_BURN :
			case DX_GRAPH_BLEND_PMA_DARKEN :
			case DX_GRAPH_BLEND_PMA_LIGHTEN :
			case DX_GRAPH_BLEND_PMA_SOFTLIGHT :
			case DX_GRAPH_BLEND_PMA_HARDLIGHT :
			case DX_GRAPH_BLEND_PMA_EXCLUSION :
			case DX_GRAPH_BLEND_PMA_NORMAL_ALPHACH :
			case DX_GRAPH_BLEND_PMA_ADD_ALPHACH :
			case DX_GRAPH_BLEND_PMA_MULTIPLE_A_ONLY :
			case DX_GRAPH_BLEND_PMA_MASK :

			case DX_GRAPH_BLEND_NORMAL :
			case DX_GRAPH_BLEND_MULTIPLE :
			case DX_GRAPH_BLEND_DIFFERENCE :
			case DX_GRAPH_BLEND_ADD :
			case DX_GRAPH_BLEND_SCREEN :
			case DX_GRAPH_BLEND_OVERLAY :
			case DX_GRAPH_BLEND_DODGE :
			case DX_GRAPH_BLEND_BURN :
			case DX_GRAPH_BLEND_DARKEN :
			case DX_GRAPH_BLEND_LIGHTEN :
			case DX_GRAPH_BLEND_SOFTLIGHT :
			case DX_GRAPH_BLEND_HARDLIGHT :
			case DX_GRAPH_BLEND_EXCLUSION :
			case DX_GRAPH_BLEND_NORMAL_ALPHACH :
			case DX_GRAPH_BLEND_ADD_ALPHACH :
			case DX_GRAPH_BLEND_MULTIPLE_A_ONLY :
			case DX_GRAPH_BLEND_MASK :
				FilterResult = GraphBlend_Basic( &Info, IsPMA ) ;
				break ;
			}
		}
		else
		{
			// フィルターによって処理を分岐
			switch( FilterOrBlendType )
			{
			case DX_GRAPH_FILTER_MONO :
				FilterResult = GraphFilter_Mono( &Info, Param.Mono_Cb, Param.Mono_Cr ) ;
				break ;

			case DX_GRAPH_FILTER_GAUSS :
				FilterResult = GraphFilter_Gauss( &Info, Param.Gauss_PixelWidth, Param.Gauss_Param ) ; 
				break ;

			case DX_GRAPH_FILTER_DOWN_SCALE :
				FilterResult = GraphFilter_Down_Scale( &Info, Info.SrcDivNum ) ;
				break ;

			case DX_GRAPH_FILTER_PMA_BRIGHT_CLIP :
			case DX_GRAPH_FILTER_BRIGHT_CLIP :
				FilterResult = GraphFilter_Bright_Clip( &Info, Param.Bright_Clip_CmpType, Param.Bright_Clip_CmpParam, Param.Bright_Clip_ClipFillFlag, &Param.Bright_Clip_ClipFillColor, IsPMA ) ;
				break ;

			case DX_GRAPH_FILTER_PMA_BRIGHT_SCALE :
			case DX_GRAPH_FILTER_BRIGHT_SCALE :
				FilterResult = GraphFilter_Bright_Scale( &Info, Param.Bright_Scale_Min, Param.Bright_Scale_Max, IsPMA ) ;
				break ;

			case DX_GRAPH_FILTER_PMA_HSB :
			case DX_GRAPH_FILTER_HSB :
				FilterResult = GraphFilter_HSB( &Info, Param.Hsb_HueType, Param.Hsb_Hue, Param.Hsb_Saturation, Param.Hsb_Bright, IsPMA ) ;
				break ;

			case DX_GRAPH_FILTER_PMA_INVERT :
			case DX_GRAPH_FILTER_INVERT :
				FilterResult = GraphFilter_Invert( &Info, IsPMA ) ;
				break ;

			case DX_GRAPH_FILTER_PMA_LEVEL :
			case DX_GRAPH_FILTER_LEVEL :
				FilterResult = GraphFilter_Level( &Info, Param.Lv_Min, Param.Lv_Max, Param.Lv_Gamma, Param.Lv_AfterMin, Param.Lv_AfterMax, IsPMA ) ;
				break ;

			case DX_GRAPH_FILTER_PMA_TWO_COLOR :
			case DX_GRAPH_FILTER_TWO_COLOR :
				FilterResult = GraphFilter_TwoColor( &Info, Param.TC_Threshold, &Param.TC_LowColor, &Param.TC_HighColor, IsPMA ) ;
				break ;

			case DX_GRAPH_FILTER_PMA_GRADIENT_MAP :
			case DX_GRAPH_FILTER_GRADIENT_MAP :
				FilterResult = GraphFilter_GradientMap( &Info, Param.GM_MapGrHandle, Param.GM_Reverse, IsPMA ) ;
				break ;

			case DX_GRAPH_FILTER_PMA_REPLACEMENT:
			case DX_GRAPH_FILTER_REPLACEMENT :
				FilterResult = GraphFilter_Replacement( &Info, Param.TargetColor, Param.NextColor, IsPMA ) ;
				break ;

			case DX_GRAPH_FILTER_PMA_BILATERAL_BLUR:
			case DX_GRAPH_FILTER_BILATERAL_BLUR :
				FilterResult = GraphFilter_BilateralBlur( &Info, IsPMA ) ;
				break ;

			case DX_GRAPH_FILTER_PREMUL_ALPHA :
				FilterResult = GraphFilter_PremulAlpha( &Info ) ;
				break ;

			case DX_GRAPH_FILTER_INTERP_ALPHA :
				FilterResult = GraphFilter_InterpAlpha( &Info ) ;
				break ;

			case DX_GRAPH_FILTER_YUV_TO_RGB :
			case DX_GRAPH_FILTER_YUV_TO_RGB_RRA :
			case DX_GRAPH_FILTER_Y2UV1_TO_RGB :
			case DX_GRAPH_FILTER_Y2UV1_TO_RGB_RRA :
				FilterResult = GraphFilter_YUVtoRGB( &Info, Param.UVGrHandle ) ;
				break ;

			case DX_GRAPH_FILTER_BICUBIC_SCALE :
				FilterResult = GraphFilter_BicubicScale( &Info, Param.DestSizeX, Param.DestSizeY ) ;
				break ;

			case DX_GRAPH_FILTER_LANCZOS3_SCALE :
				FilterResult = GraphFilter_Lanczos3Scale( &Info, Param.DestSizeX, Param.DestSizeY ) ;
				break ;

			case DX_GRAPH_FILTER_SSAO:
				FilterResult = GraphFilter_SSAO( &Info, Param.DepthMapGrHandle, Param.SSAO_KernelRadius, Param.SSAO_MinDistance, Param.SSAO_MaxDistance, Param.SSAO_Strength, Param.SSAO_OcclusionColor, Param.SSAO_OcclusionPower, Param.SSAO_ColorMapHandle ) ;
				break ;

			case DX_GRAPH_FILTER_FLOAT_COLOR_SCALE:
				FilterResult = GraphFilter_FloatColorScale( &Info, Param.ColorScale, Param.ColorPreSub ) ;
				break ;
			}
		}

		// 出力元と先が同じ場合で、出力元と先が同じ場合は出力元をクリアするフラグが立っていた場合は
		// 最初のパスが終わった時点で出力元の矩形をクリアする
		if( GSYS.HardInfo.UseShader == TRUE && Info.SrcEqualDestClearFlag && SrcGrHandle == DestGrHandle && Info.Pass == 0 )
		{
			BASEIMAGE TempImage ;

			if( NS_CreateARGB8ColorBaseImage( Width, Height, &TempImage ) == 0 )
			{
				NS_FillBaseImage( &TempImage, 0, 0, 0, 0 ) ;
				NS_BltBmpOrGraphImageToGraph( NULL, NULL, NULL, FALSE, &TempImage, NULL, SrcX1, SrcY1, SrcGrHandle ) ;
				NS_ReleaseBaseImage( &TempImage ) ;
			}
		}

		// 出力元の変更
		UseSrcGrHandle = UseDestGrHandle ;
	}

	GraphFilter_RectBltBase_Timing1_PF() ;

	// 出力先の更新
	Info.SrcGrHandle  = SrcGrHandle ;
	Info.DestGrHandle = DestGrHandle ;
	GraphFilter_DestGraphUpdate_PF( &Info, UseDestGrHandle, FilterResult ) ;

//	GSYS.HardInfo.UseShader = TRUE ;

	// 終了
	return Info.Pass == Info.PassNum ? 0 : -1 ;
}

extern int GraphFilter_Mono( GRAPHFILTER_INFO *Info, float Cb, float Cr )
{
	// 値を補正
	Cb /= 200.0f ;
	if( Cb < -0.5f )
	{
		Cb = -0.5f ;
	}
	else
	if( Cb > 0.5f )
	{
		Cb = 0.5f ;
	}

	Cr /= 200.0f ;
	if( Cr < -0.5f )
	{
		Cr = -0.5f ;
	}
	else
	if( Cr > 0.5f )
	{
		Cr = 0.5f ;
	}

	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_Mono_PF( Info, Cb, Cr ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;
		int Y ;
		int RAdd ;
		int GAdd ;
		int BAdd ;
		int R, G, B ;

		RAdd = _FTOL( ( 1.40200f * Cr                 ) * 255.0f * 4096.0f ) ;
		GAdd = _FTOL( ( 0.34414f * Cb - 0.71414f * Cr ) * 255.0f * 4096.0f ) ;
		BAdd = _FTOL( ( 1.77200f * Cb                 ) * 255.0f * 4096.0f ) ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		do
		{
			i = Width ;
			do
			{
				Y = Src[ 0 ] * ( int )( 0.114f * 4096.0f ) +
					Src[ 1 ] * ( int )( 0.587f * 4096.0f ) +
					Src[ 2 ] * ( int )( 0.299f * 4096.0f ) ;
				B = ( Y + BAdd ) >> 12 ;
				G = ( Y + GAdd ) >> 12 ;
				R = ( Y + RAdd ) >> 12 ;
				if( B < 0 ) B = 0 ; else if( B > 255 ) B = 255 ;
				if( G < 0 ) G = 0 ; else if( G > 255 ) G = 255 ;
				if( R < 0 ) R = 0 ; else if( R > 255 ) R = 255 ;

				Dest[ 0 ] = ( BYTE )B ;
				Dest[ 1 ] = ( BYTE )G ;
				Dest[ 2 ] = ( BYTE )R ;
				Dest[ 3 ] = Src[ 3 ] ;

				Src  += 4 ;
				Dest += 4 ;
			}while( -- i ) ;

			Src  += SrcAddPitch ;
			Dest += DestAddPitch ;
		}while( -- Height ) ;

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int GraphFilter_Gauss( GRAPHFILTER_INFO *Info, int PixelWidth, float Param )
{
	static int   PrevPixelWidth ;
	static float PrevParam = -10000000.0f ;
	static float Table[ 16 ] ;
	int WeightNum ;

	// PixelWidth の補正
	if( PixelWidth <= 8 )
	{
		PixelWidth = 8 ;
	}
	else
	if( PixelWidth <= 16 )
	{
		PixelWidth = 16 ;
	}
	else
	{
		PixelWidth = 32 ;
	}
	WeightNum = PixelWidth / 2 ;

//	if( Param >= 0.0000001f )
	{
		float Temp ;

		Temp = PrevParam - Param ;
		if( Temp > 0.0002f || Temp < -0.0002f || PrevPixelWidth != PixelWidth )
		{
			float Total ;
			int i ;
			float TempParam ;

			TempParam = Param ;
			if( TempParam < 0.0000000001f )
			{
				TempParam = 0.0000000001f ;
			}

			PrevParam = Param ;
			PrevPixelWidth = PixelWidth ;

			Total = 0.0f ;
			for( i = 0 ; i < WeightNum ; i ++ )
			{
				Table[ i ] = _EXPF( -0.5f * ( float )( i * i ) / TempParam ) ;
				if( i == 0 )
				{
					Total += Table[ i ] ;
				}
				else
				{
					Total += 2.0f * Table[ i ] ;
				}
			}

			for( i = 0 ; i < WeightNum ; i ++ )
				Table[ i ] /= Total ;
		}
	}

	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_Gauss_PF( Info, PixelWidth, Param, Table ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		BYTE *MinSrc ;
		BYTE *MaxSrc ;
		BYTE *MinSrcStart ;
		BYTE *MaxSrcStart ;
		DWORD i, j ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcPitch ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;
		int iTable[ 16 ] ;
		BYTE RGBA[ 64 ][ 4 ] ;
		BASEIMAGE WorkImage ;

		for( i = 0 ; i < ( DWORD )WeightNum ; i ++ )
		{
			iTable[ i ] = _FTOL( Table[ i ] * 65536.0f ) ;
		}

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
		{
			return -1 ;
		}

		if( Param < 0.0000001f )
		{
			Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
			Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

			SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
			DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

			do
			{
				i = Width ;
				do
				{
					*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}
		else
		{
			if( NS_CreateARGB8ColorBaseImage( ( int )Width, ( int )Height, &WorkImage ) >= 0 )
			{
				Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
				Dest = ( BYTE * )WorkImage.GraphData ;

				SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
				DestAddPitch = WorkImage.Pitch       - Width * 4 ;

				switch( PixelWidth )
				{
				case 8 :
					for( j = 0 ; j < Height ; j ++ )
					{
						MinSrc = Src ;
						MaxSrc = Src + 4 * ( Width - 1 ) ;
						for( i = 0 ; i < Width ; i ++ )
						{
							*( ( DWORD * )RGBA[  0 ] ) = *( ( DWORD * )Src ) ;
							*( ( DWORD * )RGBA[  1 ] ) = i < 1          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 1 ) ;
							*( ( DWORD * )RGBA[  2 ] ) = i < 2          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 2 ) ;
							*( ( DWORD * )RGBA[  3 ] ) = i + 1 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 1 ) ;
							*( ( DWORD * )RGBA[  4 ] ) = i + 2 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 2 ) ;
							*( ( DWORD * )RGBA[  5 ] ) = i < 3          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 3 ) ;
							*( ( DWORD * )RGBA[  6 ] ) = i < 4          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 4 ) ;
							*( ( DWORD * )RGBA[  7 ] ) = i + 3 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 3 ) ;
							*( ( DWORD * )RGBA[  8 ] ) = i + 4 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 4 ) ;
							*( ( DWORD * )RGBA[  9 ] ) = i < 5          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 5 ) ;
							*( ( DWORD * )RGBA[ 10 ] ) = i < 6          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 6 ) ;
							*( ( DWORD * )RGBA[ 11 ] ) = i + 5 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 5 ) ;
							*( ( DWORD * )RGBA[ 12 ] ) = i + 6 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 6 ) ;

							Dest[ 0 ] = ( BYTE )(
								( RGBA[ 0 ][ 0 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 0 ] + RGBA[  2 ][ 0 ] + RGBA[  3 ][ 0 ] + RGBA[  4 ][ 0 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 0 ] + RGBA[  6 ][ 0 ] + RGBA[  7 ][ 0 ] + RGBA[  8 ][ 0 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 0 ] + RGBA[ 10 ][ 0 ] + RGBA[ 11 ][ 0 ] + RGBA[ 12 ][ 0 ] ) * iTable[ 3 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 1 ] = ( BYTE )(
								( RGBA[ 0 ][ 1 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 1 ] + RGBA[  2 ][ 1 ] + RGBA[  3 ][ 1 ] + RGBA[  4 ][ 1 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 1 ] + RGBA[  6 ][ 1 ] + RGBA[  7 ][ 1 ] + RGBA[  8 ][ 1 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 1 ] + RGBA[ 10 ][ 1 ] + RGBA[ 11 ][ 1 ] + RGBA[ 12 ][ 1 ] ) * iTable[ 3 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 2 ] = ( BYTE )(
								( RGBA[ 0 ][ 2 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 2 ] + RGBA[  2 ][ 2 ] + RGBA[  3 ][ 2 ] + RGBA[  4 ][ 2 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 2 ] + RGBA[  6 ][ 2 ] + RGBA[  7 ][ 2 ] + RGBA[  8 ][ 2 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 2 ] + RGBA[ 10 ][ 2 ] + RGBA[ 11 ][ 2 ] + RGBA[ 12 ][ 2 ] ) * iTable[ 3 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 3 ] = ( BYTE )(
								( RGBA[ 0 ][ 3 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 3 ] + RGBA[  2 ][ 3 ] + RGBA[  3 ][ 3 ] + RGBA[  4 ][ 3 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 3 ] + RGBA[  6 ][ 3 ] + RGBA[  7 ][ 3 ] + RGBA[  8 ][ 3 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 3 ] + RGBA[ 10 ][ 3 ] + RGBA[ 11 ][ 3 ] + RGBA[ 12 ][ 3 ] ) * iTable[ 3 ] ) >> 1 ) ) >> 16 ) ;

							Src  += 4 ;
							Dest += 4 ;
						}

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}
					break ;

				case 16 :
					for( j = 0 ; j < Height ; j ++ )
					{
						MinSrc = Src ;
						MaxSrc = Src + 4 * ( Width - 1 ) ;
						for( i = 0 ; i < Width ; i ++ )
						{
							*( ( DWORD * )RGBA[  0 ] ) = *( ( DWORD * )Src ) ;
							*( ( DWORD * )RGBA[  1 ] ) = i <  1          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  1 ) ;
							*( ( DWORD * )RGBA[  2 ] ) = i <  2          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  2 ) ;
							*( ( DWORD * )RGBA[  3 ] ) = i +  1 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  1 ) ;
							*( ( DWORD * )RGBA[  4 ] ) = i +  2 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  2 ) ;
							*( ( DWORD * )RGBA[  5 ] ) = i <  3          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  3 ) ;
							*( ( DWORD * )RGBA[  6 ] ) = i <  4          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  4 ) ;
							*( ( DWORD * )RGBA[  7 ] ) = i +  3 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  3 ) ;
							*( ( DWORD * )RGBA[  8 ] ) = i +  4 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  4 ) ;
							*( ( DWORD * )RGBA[  9 ] ) = i <  5          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  5 ) ;
							*( ( DWORD * )RGBA[ 10 ] ) = i <  6          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  6 ) ;
							*( ( DWORD * )RGBA[ 11 ] ) = i +  5 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  5 ) ;
							*( ( DWORD * )RGBA[ 12 ] ) = i +  6 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  6 ) ;
							*( ( DWORD * )RGBA[ 13 ] ) = i <  7          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  7 ) ;
							*( ( DWORD * )RGBA[ 14 ] ) = i <  8          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  8 ) ;
							*( ( DWORD * )RGBA[ 15 ] ) = i +  7 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  7 ) ;
							*( ( DWORD * )RGBA[ 16 ] ) = i +  8 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  8 ) ;
							*( ( DWORD * )RGBA[ 17 ] ) = i <  9          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  9 ) ;
							*( ( DWORD * )RGBA[ 18 ] ) = i < 10          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 10 ) ;
							*( ( DWORD * )RGBA[ 19 ] ) = i +  9 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  9 ) ;
							*( ( DWORD * )RGBA[ 20 ] ) = i + 10 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 10 ) ;
							*( ( DWORD * )RGBA[ 21 ] ) = i < 11          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 11 ) ;
							*( ( DWORD * )RGBA[ 22 ] ) = i < 12          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 12 ) ;
							*( ( DWORD * )RGBA[ 23 ] ) = i + 11 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 11 ) ;
							*( ( DWORD * )RGBA[ 24 ] ) = i + 12 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 12 ) ;
							*( ( DWORD * )RGBA[ 25 ] ) = i < 13          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 13 ) ;
							*( ( DWORD * )RGBA[ 26 ] ) = i < 14          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 14 ) ;
							*( ( DWORD * )RGBA[ 27 ] ) = i + 13 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 13 ) ;
							*( ( DWORD * )RGBA[ 28 ] ) = i + 14 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 14 ) ;

							Dest[ 0 ] = ( BYTE )(
								( RGBA[ 0 ][ 0 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 0 ] + RGBA[  2 ][ 0 ] + RGBA[  3 ][ 0 ] + RGBA[  4 ][ 0 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 0 ] + RGBA[  6 ][ 0 ] + RGBA[  7 ][ 0 ] + RGBA[  8 ][ 0 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 0 ] + RGBA[ 10 ][ 0 ] + RGBA[ 11 ][ 0 ] + RGBA[ 12 ][ 0 ] ) * iTable[ 3 ] +
									  ( RGBA[ 13 ][ 0 ] + RGBA[ 14 ][ 0 ] + RGBA[ 15 ][ 0 ] + RGBA[ 16 ][ 0 ] ) * iTable[ 4 ] +
									  ( RGBA[ 17 ][ 0 ] + RGBA[ 18 ][ 0 ] + RGBA[ 19 ][ 0 ] + RGBA[ 20 ][ 0 ] ) * iTable[ 5 ] +
									  ( RGBA[ 21 ][ 0 ] + RGBA[ 22 ][ 0 ] + RGBA[ 23 ][ 0 ] + RGBA[ 24 ][ 0 ] ) * iTable[ 6 ] +
									  ( RGBA[ 25 ][ 0 ] + RGBA[ 26 ][ 0 ] + RGBA[ 27 ][ 0 ] + RGBA[ 28 ][ 0 ] ) * iTable[ 7 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 1 ] = ( BYTE )(
								( RGBA[ 0 ][ 1 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 1 ] + RGBA[  2 ][ 1 ] + RGBA[  3 ][ 1 ] + RGBA[  4 ][ 1 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 1 ] + RGBA[  6 ][ 1 ] + RGBA[  7 ][ 1 ] + RGBA[  8 ][ 1 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 1 ] + RGBA[ 10 ][ 1 ] + RGBA[ 11 ][ 1 ] + RGBA[ 12 ][ 1 ] ) * iTable[ 3 ] +
									  ( RGBA[ 13 ][ 1 ] + RGBA[ 14 ][ 1 ] + RGBA[ 15 ][ 1 ] + RGBA[ 16 ][ 1 ] ) * iTable[ 4 ] +
									  ( RGBA[ 17 ][ 1 ] + RGBA[ 18 ][ 1 ] + RGBA[ 19 ][ 1 ] + RGBA[ 20 ][ 1 ] ) * iTable[ 5 ] +
									  ( RGBA[ 21 ][ 1 ] + RGBA[ 22 ][ 1 ] + RGBA[ 23 ][ 1 ] + RGBA[ 24 ][ 1 ] ) * iTable[ 6 ] +
									  ( RGBA[ 25 ][ 1 ] + RGBA[ 26 ][ 1 ] + RGBA[ 27 ][ 1 ] + RGBA[ 28 ][ 1 ] ) * iTable[ 7 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 2 ] = ( BYTE )(
								( RGBA[ 0 ][ 2 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 2 ] + RGBA[  2 ][ 2 ] + RGBA[  3 ][ 2 ] + RGBA[  4 ][ 2 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 2 ] + RGBA[  6 ][ 2 ] + RGBA[  7 ][ 2 ] + RGBA[  8 ][ 2 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 2 ] + RGBA[ 10 ][ 2 ] + RGBA[ 11 ][ 2 ] + RGBA[ 12 ][ 2 ] ) * iTable[ 3 ] +
									  ( RGBA[ 13 ][ 2 ] + RGBA[ 14 ][ 2 ] + RGBA[ 15 ][ 2 ] + RGBA[ 16 ][ 2 ] ) * iTable[ 4 ] +
									  ( RGBA[ 17 ][ 2 ] + RGBA[ 18 ][ 2 ] + RGBA[ 19 ][ 2 ] + RGBA[ 20 ][ 2 ] ) * iTable[ 5 ] +
									  ( RGBA[ 21 ][ 2 ] + RGBA[ 22 ][ 2 ] + RGBA[ 23 ][ 2 ] + RGBA[ 24 ][ 2 ] ) * iTable[ 6 ] +
									  ( RGBA[ 25 ][ 2 ] + RGBA[ 26 ][ 2 ] + RGBA[ 27 ][ 2 ] + RGBA[ 28 ][ 2 ] ) * iTable[ 7 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 3 ] = ( BYTE )(
								( RGBA[ 0 ][ 3 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 3 ] + RGBA[  2 ][ 3 ] + RGBA[  3 ][ 3 ] + RGBA[  4 ][ 3 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 3 ] + RGBA[  6 ][ 3 ] + RGBA[  7 ][ 3 ] + RGBA[  8 ][ 3 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 3 ] + RGBA[ 10 ][ 3 ] + RGBA[ 11 ][ 3 ] + RGBA[ 12 ][ 3 ] ) * iTable[ 3 ] +
									  ( RGBA[ 13 ][ 3 ] + RGBA[ 14 ][ 3 ] + RGBA[ 15 ][ 3 ] + RGBA[ 16 ][ 3 ] ) * iTable[ 4 ] +
									  ( RGBA[ 17 ][ 3 ] + RGBA[ 18 ][ 3 ] + RGBA[ 19 ][ 3 ] + RGBA[ 20 ][ 3 ] ) * iTable[ 5 ] +
									  ( RGBA[ 21 ][ 3 ] + RGBA[ 22 ][ 3 ] + RGBA[ 23 ][ 3 ] + RGBA[ 24 ][ 3 ] ) * iTable[ 6 ] +
									  ( RGBA[ 25 ][ 3 ] + RGBA[ 26 ][ 3 ] + RGBA[ 27 ][ 3 ] + RGBA[ 28 ][ 3 ] ) * iTable[ 7 ] ) >> 1 ) ) >> 16 ) ;

							Src  += 4 ;
							Dest += 4 ;
						}

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}
					break ;

				case 32 :
					for( j = 0 ; j < Height ; j ++ )
					{
						MinSrc = Src ;
						MaxSrc = Src + 4 * ( Width - 1 ) ;
						for( i = 0 ; i < Width ; i ++ )
						{
							*( ( DWORD * )RGBA[  0 ] ) = *( ( DWORD * )Src ) ;
							*( ( DWORD * )RGBA[  1 ] ) = i <  1          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  1 ) ;
							*( ( DWORD * )RGBA[  2 ] ) = i <  2          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  2 ) ;
							*( ( DWORD * )RGBA[  3 ] ) = i +  1 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  1 ) ;
							*( ( DWORD * )RGBA[  4 ] ) = i +  2 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  2 ) ;
							*( ( DWORD * )RGBA[  5 ] ) = i <  3          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  3 ) ;
							*( ( DWORD * )RGBA[  6 ] ) = i <  4          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  4 ) ;
							*( ( DWORD * )RGBA[  7 ] ) = i +  3 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  3 ) ;
							*( ( DWORD * )RGBA[  8 ] ) = i +  4 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  4 ) ;
							*( ( DWORD * )RGBA[  9 ] ) = i <  5          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  5 ) ;
							*( ( DWORD * )RGBA[ 10 ] ) = i <  6          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  6 ) ;
							*( ( DWORD * )RGBA[ 11 ] ) = i +  5 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  5 ) ;
							*( ( DWORD * )RGBA[ 12 ] ) = i +  6 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  6 ) ;
							*( ( DWORD * )RGBA[ 13 ] ) = i <  7          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  7 ) ;
							*( ( DWORD * )RGBA[ 14 ] ) = i <  8          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  8 ) ;
							*( ( DWORD * )RGBA[ 15 ] ) = i +  7 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  7 ) ;
							*( ( DWORD * )RGBA[ 16 ] ) = i +  8 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  8 ) ;
							*( ( DWORD * )RGBA[ 17 ] ) = i <  9          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src -  9 ) ;
							*( ( DWORD * )RGBA[ 18 ] ) = i < 10          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 10 ) ;
							*( ( DWORD * )RGBA[ 19 ] ) = i +  9 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src +  9 ) ;
							*( ( DWORD * )RGBA[ 20 ] ) = i + 10 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 10 ) ;
							*( ( DWORD * )RGBA[ 21 ] ) = i < 11          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 11 ) ;
							*( ( DWORD * )RGBA[ 22 ] ) = i < 12          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 12 ) ;
							*( ( DWORD * )RGBA[ 23 ] ) = i + 11 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 11 ) ;
							*( ( DWORD * )RGBA[ 24 ] ) = i + 12 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 12 ) ;
							*( ( DWORD * )RGBA[ 25 ] ) = i < 13          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 13 ) ;
							*( ( DWORD * )RGBA[ 26 ] ) = i < 14          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 14 ) ;
							*( ( DWORD * )RGBA[ 27 ] ) = i + 13 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 13 ) ;
							*( ( DWORD * )RGBA[ 28 ] ) = i + 14 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 14 ) ;
							*( ( DWORD * )RGBA[ 29 ] ) = i < 15          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 15 ) ;
							*( ( DWORD * )RGBA[ 30 ] ) = i < 16          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 16 ) ;
							*( ( DWORD * )RGBA[ 31 ] ) = i + 15 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 15 ) ;
							*( ( DWORD * )RGBA[ 32 ] ) = i + 16 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 16 ) ;
							*( ( DWORD * )RGBA[ 33 ] ) = i < 17          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 17 ) ;
							*( ( DWORD * )RGBA[ 34 ] ) = i < 18          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 18 ) ;
							*( ( DWORD * )RGBA[ 35 ] ) = i + 17 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 17 ) ;
							*( ( DWORD * )RGBA[ 36 ] ) = i + 18 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 18 ) ;
							*( ( DWORD * )RGBA[ 37 ] ) = i < 19          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 19 ) ;
							*( ( DWORD * )RGBA[ 38 ] ) = i < 20          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 20 ) ;
							*( ( DWORD * )RGBA[ 39 ] ) = i + 19 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 19 ) ;
							*( ( DWORD * )RGBA[ 40 ] ) = i + 20 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 20 ) ;
							*( ( DWORD * )RGBA[ 41 ] ) = i < 21          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 21 ) ;
							*( ( DWORD * )RGBA[ 42 ] ) = i < 22          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 22 ) ;
							*( ( DWORD * )RGBA[ 43 ] ) = i + 21 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 21 ) ;
							*( ( DWORD * )RGBA[ 44 ] ) = i + 22 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 22 ) ;
							*( ( DWORD * )RGBA[ 45 ] ) = i < 23          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 23 ) ;
							*( ( DWORD * )RGBA[ 46 ] ) = i < 24          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 24 ) ;
							*( ( DWORD * )RGBA[ 47 ] ) = i + 23 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 23 ) ;
							*( ( DWORD * )RGBA[ 48 ] ) = i + 24 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 24 ) ;
							*( ( DWORD * )RGBA[ 49 ] ) = i < 25          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 25 ) ;
							*( ( DWORD * )RGBA[ 50 ] ) = i < 26          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 26 ) ;
							*( ( DWORD * )RGBA[ 51 ] ) = i + 25 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 25 ) ;
							*( ( DWORD * )RGBA[ 52 ] ) = i + 26 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 26 ) ;
							*( ( DWORD * )RGBA[ 53 ] ) = i < 27          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 27 ) ;
							*( ( DWORD * )RGBA[ 54 ] ) = i < 28          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 28 ) ;
							*( ( DWORD * )RGBA[ 55 ] ) = i + 27 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 27 ) ;
							*( ( DWORD * )RGBA[ 56 ] ) = i + 28 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 28 ) ;
							*( ( DWORD * )RGBA[ 57 ] ) = i < 29          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 29 ) ;
							*( ( DWORD * )RGBA[ 58 ] ) = i < 30          ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )Src - 30 ) ;
							*( ( DWORD * )RGBA[ 59 ] ) = i + 29 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 29 ) ;
							*( ( DWORD * )RGBA[ 60 ] ) = i + 30 >= Width ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )Src + 30 ) ;

							Dest[ 0 ] = ( BYTE )(
								( RGBA[ 0 ][ 0 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 0 ] + RGBA[  2 ][ 0 ] + RGBA[  3 ][ 0 ] + RGBA[  4 ][ 0 ] ) * iTable[  1 ] +
									  ( RGBA[  5 ][ 0 ] + RGBA[  6 ][ 0 ] + RGBA[  7 ][ 0 ] + RGBA[  8 ][ 0 ] ) * iTable[  2 ] +
									  ( RGBA[  9 ][ 0 ] + RGBA[ 10 ][ 0 ] + RGBA[ 11 ][ 0 ] + RGBA[ 12 ][ 0 ] ) * iTable[  3 ] +
									  ( RGBA[ 13 ][ 0 ] + RGBA[ 14 ][ 0 ] + RGBA[ 15 ][ 0 ] + RGBA[ 16 ][ 0 ] ) * iTable[  4 ] +
									  ( RGBA[ 17 ][ 0 ] + RGBA[ 18 ][ 0 ] + RGBA[ 19 ][ 0 ] + RGBA[ 20 ][ 0 ] ) * iTable[  5 ] +
									  ( RGBA[ 21 ][ 0 ] + RGBA[ 22 ][ 0 ] + RGBA[ 23 ][ 0 ] + RGBA[ 24 ][ 0 ] ) * iTable[  6 ] +
									  ( RGBA[ 25 ][ 0 ] + RGBA[ 26 ][ 0 ] + RGBA[ 27 ][ 0 ] + RGBA[ 28 ][ 0 ] ) * iTable[  7 ] +
									  ( RGBA[ 29 ][ 0 ] + RGBA[ 30 ][ 0 ] + RGBA[ 31 ][ 0 ] + RGBA[ 32 ][ 0 ] ) * iTable[  8 ] +
									  ( RGBA[ 33 ][ 0 ] + RGBA[ 34 ][ 0 ] + RGBA[ 35 ][ 0 ] + RGBA[ 36 ][ 0 ] ) * iTable[  9 ] +
									  ( RGBA[ 37 ][ 0 ] + RGBA[ 38 ][ 0 ] + RGBA[ 39 ][ 0 ] + RGBA[ 40 ][ 0 ] ) * iTable[ 10 ] +
									  ( RGBA[ 41 ][ 0 ] + RGBA[ 42 ][ 0 ] + RGBA[ 43 ][ 0 ] + RGBA[ 44 ][ 0 ] ) * iTable[ 11 ] +
									  ( RGBA[ 45 ][ 0 ] + RGBA[ 46 ][ 0 ] + RGBA[ 47 ][ 0 ] + RGBA[ 48 ][ 0 ] ) * iTable[ 12 ] +
									  ( RGBA[ 49 ][ 0 ] + RGBA[ 50 ][ 0 ] + RGBA[ 51 ][ 0 ] + RGBA[ 52 ][ 0 ] ) * iTable[ 13 ] +
									  ( RGBA[ 53 ][ 0 ] + RGBA[ 54 ][ 0 ] + RGBA[ 55 ][ 0 ] + RGBA[ 56 ][ 0 ] ) * iTable[ 14 ] +
									  ( RGBA[ 57 ][ 0 ] + RGBA[ 58 ][ 0 ] + RGBA[ 59 ][ 0 ] + RGBA[ 60 ][ 0 ] ) * iTable[ 15 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 1 ] = ( BYTE )(
								( RGBA[ 0 ][ 1 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 1 ] + RGBA[  2 ][ 1 ] + RGBA[  3 ][ 1 ] + RGBA[  4 ][ 1 ] ) * iTable[  1 ] +
									  ( RGBA[  5 ][ 1 ] + RGBA[  6 ][ 1 ] + RGBA[  7 ][ 1 ] + RGBA[  8 ][ 1 ] ) * iTable[  2 ] +
									  ( RGBA[  9 ][ 1 ] + RGBA[ 10 ][ 1 ] + RGBA[ 11 ][ 1 ] + RGBA[ 12 ][ 1 ] ) * iTable[  3 ] +
									  ( RGBA[ 13 ][ 1 ] + RGBA[ 14 ][ 1 ] + RGBA[ 15 ][ 1 ] + RGBA[ 16 ][ 1 ] ) * iTable[  4 ] +
									  ( RGBA[ 17 ][ 1 ] + RGBA[ 18 ][ 1 ] + RGBA[ 19 ][ 1 ] + RGBA[ 20 ][ 1 ] ) * iTable[  5 ] +
									  ( RGBA[ 21 ][ 1 ] + RGBA[ 22 ][ 1 ] + RGBA[ 23 ][ 1 ] + RGBA[ 24 ][ 1 ] ) * iTable[  6 ] +
									  ( RGBA[ 25 ][ 1 ] + RGBA[ 26 ][ 1 ] + RGBA[ 27 ][ 1 ] + RGBA[ 28 ][ 1 ] ) * iTable[  7 ] +
									  ( RGBA[ 29 ][ 1 ] + RGBA[ 30 ][ 1 ] + RGBA[ 31 ][ 1 ] + RGBA[ 32 ][ 1 ] ) * iTable[  8 ] +
									  ( RGBA[ 33 ][ 1 ] + RGBA[ 34 ][ 1 ] + RGBA[ 35 ][ 1 ] + RGBA[ 36 ][ 1 ] ) * iTable[  9 ] +
									  ( RGBA[ 37 ][ 1 ] + RGBA[ 38 ][ 1 ] + RGBA[ 39 ][ 1 ] + RGBA[ 40 ][ 1 ] ) * iTable[ 10 ] +
									  ( RGBA[ 41 ][ 1 ] + RGBA[ 42 ][ 1 ] + RGBA[ 43 ][ 1 ] + RGBA[ 44 ][ 1 ] ) * iTable[ 11 ] +
									  ( RGBA[ 45 ][ 1 ] + RGBA[ 46 ][ 1 ] + RGBA[ 47 ][ 1 ] + RGBA[ 48 ][ 1 ] ) * iTable[ 12 ] +
									  ( RGBA[ 49 ][ 1 ] + RGBA[ 50 ][ 1 ] + RGBA[ 51 ][ 1 ] + RGBA[ 52 ][ 1 ] ) * iTable[ 13 ] +
									  ( RGBA[ 53 ][ 1 ] + RGBA[ 54 ][ 1 ] + RGBA[ 55 ][ 1 ] + RGBA[ 56 ][ 1 ] ) * iTable[ 14 ] +
									  ( RGBA[ 57 ][ 1 ] + RGBA[ 58 ][ 1 ] + RGBA[ 59 ][ 1 ] + RGBA[ 60 ][ 1 ] ) * iTable[ 15 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 2 ] = ( BYTE )(
								( RGBA[ 0 ][ 2 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 2 ] + RGBA[  2 ][ 2 ] + RGBA[  3 ][ 2 ] + RGBA[  4 ][ 2 ] ) * iTable[  1 ] +
									  ( RGBA[  5 ][ 2 ] + RGBA[  6 ][ 2 ] + RGBA[  7 ][ 2 ] + RGBA[  8 ][ 2 ] ) * iTable[  2 ] +
									  ( RGBA[  9 ][ 2 ] + RGBA[ 10 ][ 2 ] + RGBA[ 11 ][ 2 ] + RGBA[ 12 ][ 2 ] ) * iTable[  3 ] +
									  ( RGBA[ 13 ][ 2 ] + RGBA[ 14 ][ 2 ] + RGBA[ 15 ][ 2 ] + RGBA[ 16 ][ 2 ] ) * iTable[  4 ] +
									  ( RGBA[ 17 ][ 2 ] + RGBA[ 18 ][ 2 ] + RGBA[ 19 ][ 2 ] + RGBA[ 20 ][ 2 ] ) * iTable[  5 ] +
									  ( RGBA[ 21 ][ 2 ] + RGBA[ 22 ][ 2 ] + RGBA[ 23 ][ 2 ] + RGBA[ 24 ][ 2 ] ) * iTable[  6 ] +
									  ( RGBA[ 25 ][ 2 ] + RGBA[ 26 ][ 2 ] + RGBA[ 27 ][ 2 ] + RGBA[ 28 ][ 2 ] ) * iTable[  7 ] +
									  ( RGBA[ 29 ][ 2 ] + RGBA[ 30 ][ 2 ] + RGBA[ 31 ][ 2 ] + RGBA[ 32 ][ 2 ] ) * iTable[  8 ] +
									  ( RGBA[ 33 ][ 2 ] + RGBA[ 34 ][ 2 ] + RGBA[ 35 ][ 2 ] + RGBA[ 36 ][ 2 ] ) * iTable[  9 ] +
									  ( RGBA[ 37 ][ 2 ] + RGBA[ 38 ][ 2 ] + RGBA[ 39 ][ 2 ] + RGBA[ 40 ][ 2 ] ) * iTable[ 10 ] +
									  ( RGBA[ 41 ][ 2 ] + RGBA[ 42 ][ 2 ] + RGBA[ 43 ][ 2 ] + RGBA[ 44 ][ 2 ] ) * iTable[ 11 ] +
									  ( RGBA[ 45 ][ 2 ] + RGBA[ 46 ][ 2 ] + RGBA[ 47 ][ 2 ] + RGBA[ 48 ][ 2 ] ) * iTable[ 12 ] +
									  ( RGBA[ 49 ][ 2 ] + RGBA[ 50 ][ 2 ] + RGBA[ 51 ][ 2 ] + RGBA[ 52 ][ 2 ] ) * iTable[ 13 ] +
									  ( RGBA[ 53 ][ 2 ] + RGBA[ 54 ][ 2 ] + RGBA[ 55 ][ 2 ] + RGBA[ 56 ][ 2 ] ) * iTable[ 14 ] +
									  ( RGBA[ 57 ][ 2 ] + RGBA[ 58 ][ 2 ] + RGBA[ 59 ][ 2 ] + RGBA[ 60 ][ 2 ] ) * iTable[ 15 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 3 ] = ( BYTE )(
								( RGBA[ 0 ][ 3 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 3 ] + RGBA[  2 ][ 3 ] + RGBA[  3 ][ 3 ] + RGBA[  4 ][ 3 ] ) * iTable[  1 ] +
									  ( RGBA[  5 ][ 3 ] + RGBA[  6 ][ 3 ] + RGBA[  7 ][ 3 ] + RGBA[  8 ][ 3 ] ) * iTable[  2 ] +
									  ( RGBA[  9 ][ 3 ] + RGBA[ 10 ][ 3 ] + RGBA[ 11 ][ 3 ] + RGBA[ 12 ][ 3 ] ) * iTable[  3 ] +
									  ( RGBA[ 13 ][ 3 ] + RGBA[ 14 ][ 3 ] + RGBA[ 15 ][ 3 ] + RGBA[ 16 ][ 3 ] ) * iTable[  4 ] +
									  ( RGBA[ 17 ][ 3 ] + RGBA[ 18 ][ 3 ] + RGBA[ 19 ][ 3 ] + RGBA[ 20 ][ 3 ] ) * iTable[  5 ] +
									  ( RGBA[ 21 ][ 3 ] + RGBA[ 22 ][ 3 ] + RGBA[ 23 ][ 3 ] + RGBA[ 24 ][ 3 ] ) * iTable[  6 ] +
									  ( RGBA[ 25 ][ 3 ] + RGBA[ 26 ][ 3 ] + RGBA[ 27 ][ 3 ] + RGBA[ 28 ][ 3 ] ) * iTable[  7 ] +
									  ( RGBA[ 29 ][ 3 ] + RGBA[ 30 ][ 3 ] + RGBA[ 31 ][ 3 ] + RGBA[ 32 ][ 3 ] ) * iTable[  8 ] +
									  ( RGBA[ 33 ][ 3 ] + RGBA[ 34 ][ 3 ] + RGBA[ 35 ][ 3 ] + RGBA[ 36 ][ 3 ] ) * iTable[  9 ] +
									  ( RGBA[ 37 ][ 3 ] + RGBA[ 38 ][ 3 ] + RGBA[ 39 ][ 3 ] + RGBA[ 40 ][ 3 ] ) * iTable[ 10 ] +
									  ( RGBA[ 41 ][ 3 ] + RGBA[ 42 ][ 3 ] + RGBA[ 43 ][ 3 ] + RGBA[ 44 ][ 3 ] ) * iTable[ 11 ] +
									  ( RGBA[ 45 ][ 3 ] + RGBA[ 46 ][ 3 ] + RGBA[ 47 ][ 3 ] + RGBA[ 48 ][ 3 ] ) * iTable[ 12 ] +
									  ( RGBA[ 49 ][ 3 ] + RGBA[ 50 ][ 3 ] + RGBA[ 51 ][ 3 ] + RGBA[ 52 ][ 3 ] ) * iTable[ 13 ] +
									  ( RGBA[ 53 ][ 3 ] + RGBA[ 54 ][ 3 ] + RGBA[ 55 ][ 3 ] + RGBA[ 56 ][ 3 ] ) * iTable[ 14 ] +
									  ( RGBA[ 57 ][ 3 ] + RGBA[ 58 ][ 3 ] + RGBA[ 59 ][ 3 ] + RGBA[ 60 ][ 3 ] ) * iTable[ 15 ] ) >> 1 ) ) >> 16 ) ;

							Src  += 4 ;
							Dest += 4 ;
						}

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}
					break ;
				}

				Src = ( BYTE * )WorkImage.GraphData ;
				Dest = ( BYTE * )Info->DestBaseImage.GraphData ;
				MinSrcStart = Src ;
				MaxSrcStart = Src + WorkImage.Pitch * ( Height - 1 ) ;

				SrcPitch     = ( DWORD )WorkImage.Pitch ;
				SrcAddPitch  = WorkImage.Pitch       - Width * 4 ;
				DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

				switch( PixelWidth )
				{
				case 8 :
					for( j = 0 ; j < Height ; j ++ )
					{
						MinSrc = MinSrcStart ;
						MaxSrc = MaxSrcStart ;

						for( i = 0 ; i < Width ; i ++ )
						{
							*( ( DWORD * )RGBA[  0 ] ) = *( ( DWORD * )Src ) ;
							*( ( DWORD * )RGBA[  1 ] ) = j < 1           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 1 ) ) ;
							*( ( DWORD * )RGBA[  2 ] ) = j < 2           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 2 ) ) ;
							*( ( DWORD * )RGBA[  3 ] ) = j + 1 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 1 ) ) ;
							*( ( DWORD * )RGBA[  4 ] ) = j + 2 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 2 ) ) ;
							*( ( DWORD * )RGBA[  5 ] ) = j < 3           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 3 ) ) ;
							*( ( DWORD * )RGBA[  6 ] ) = j < 4           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 4 ) ) ;
							*( ( DWORD * )RGBA[  7 ] ) = j + 3 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 3 ) ) ;
							*( ( DWORD * )RGBA[  8 ] ) = j + 4 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 4 ) ) ;
							*( ( DWORD * )RGBA[  9 ] ) = j < 5           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 5 ) ) ;
							*( ( DWORD * )RGBA[ 10 ] ) = j < 6           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 6 ) ) ;
							*( ( DWORD * )RGBA[ 11 ] ) = j + 5 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 5 ) ) ;
							*( ( DWORD * )RGBA[ 12 ] ) = j + 6 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 6 ) ) ;

							Dest[ 0 ] = ( BYTE )(
								( RGBA[ 0 ][ 0 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 0 ] + RGBA[  2 ][ 0 ] + RGBA[  3 ][ 0 ] + RGBA[  4 ][ 0 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 0 ] + RGBA[  6 ][ 0 ] + RGBA[  7 ][ 0 ] + RGBA[  8 ][ 0 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 0 ] + RGBA[ 10 ][ 0 ] + RGBA[ 11 ][ 0 ] + RGBA[ 12 ][ 0 ] ) * iTable[ 3 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 1 ] = ( BYTE )(
								( RGBA[ 0 ][ 1 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 1 ] + RGBA[  2 ][ 1 ] + RGBA[  3 ][ 1 ] + RGBA[  4 ][ 1 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 1 ] + RGBA[  6 ][ 1 ] + RGBA[  7 ][ 1 ] + RGBA[  8 ][ 1 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 1 ] + RGBA[ 10 ][ 1 ] + RGBA[ 11 ][ 1 ] + RGBA[ 12 ][ 1 ] ) * iTable[ 3 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 2 ] = ( BYTE )(
								( RGBA[ 0 ][ 2 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 2 ] + RGBA[  2 ][ 2 ] + RGBA[  3 ][ 2 ] + RGBA[  4 ][ 2 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 2 ] + RGBA[  6 ][ 2 ] + RGBA[  7 ][ 2 ] + RGBA[  8 ][ 2 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 2 ] + RGBA[ 10 ][ 2 ] + RGBA[ 11 ][ 2 ] + RGBA[ 12 ][ 2 ] ) * iTable[ 3 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 3 ] = ( BYTE )(
								( RGBA[ 0 ][ 3 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 3 ] + RGBA[  2 ][ 3 ] + RGBA[  3 ][ 3 ] + RGBA[  4 ][ 3 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 3 ] + RGBA[  6 ][ 3 ] + RGBA[  7 ][ 3 ] + RGBA[  8 ][ 3 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 3 ] + RGBA[ 10 ][ 3 ] + RGBA[ 11 ][ 3 ] + RGBA[ 12 ][ 3 ] ) * iTable[ 3 ] ) >> 1 ) ) >> 16 ) ;

							MinSrc += 4 ;
							MaxSrc += 4 ;
							Src  += 4 ;
							Dest += 4 ;
						}

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}
					break ;

				case 16 :
					for( j = 0 ; j < Height ; j ++ )
					{
						MinSrc = MinSrcStart ;
						MaxSrc = MaxSrcStart ;

						for( i = 0 ; i < Width ; i ++ )
						{
							*( ( DWORD * )RGBA[  0 ] ) = *( ( DWORD * )Src ) ;
							*( ( DWORD * )RGBA[  1 ] ) = j <  1           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  1 ) ) ;
							*( ( DWORD * )RGBA[  2 ] ) = j <  2           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  2 ) ) ;
							*( ( DWORD * )RGBA[  3 ] ) = j +  1 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  1 ) ) ;
							*( ( DWORD * )RGBA[  4 ] ) = j +  2 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  2 ) ) ;
							*( ( DWORD * )RGBA[  5 ] ) = j <  3           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  3 ) ) ;
							*( ( DWORD * )RGBA[  6 ] ) = j <  4           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  4 ) ) ;
							*( ( DWORD * )RGBA[  7 ] ) = j +  3 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  3 ) ) ;
							*( ( DWORD * )RGBA[  8 ] ) = j +  4 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  4 ) ) ;
							*( ( DWORD * )RGBA[  9 ] ) = j <  5           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  5 ) ) ;
							*( ( DWORD * )RGBA[ 10 ] ) = j <  6           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  6 ) ) ;
							*( ( DWORD * )RGBA[ 11 ] ) = j +  5 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  5 ) ) ;
							*( ( DWORD * )RGBA[ 12 ] ) = j +  6 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  6 ) ) ;
							*( ( DWORD * )RGBA[ 13 ] ) = j <  7           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  7 ) ) ;
							*( ( DWORD * )RGBA[ 14 ] ) = j <  8           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  8 ) ) ;
							*( ( DWORD * )RGBA[ 15 ] ) = j +  7 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  7 ) ) ;
							*( ( DWORD * )RGBA[ 16 ] ) = j +  8 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  8 ) ) ;
							*( ( DWORD * )RGBA[ 17 ] ) = j <  9           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  9 ) ) ;
							*( ( DWORD * )RGBA[ 18 ] ) = j < 10           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 10 ) ) ;
							*( ( DWORD * )RGBA[ 19 ] ) = j +  9 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  9 ) ) ;
							*( ( DWORD * )RGBA[ 20 ] ) = j + 10 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 10 ) ) ;
							*( ( DWORD * )RGBA[ 21 ] ) = j < 11           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 11 ) ) ;
							*( ( DWORD * )RGBA[ 22 ] ) = j < 12           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 12 ) ) ;
							*( ( DWORD * )RGBA[ 23 ] ) = j + 11 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 11 ) ) ;
							*( ( DWORD * )RGBA[ 24 ] ) = j + 12 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 12 ) ) ;
							*( ( DWORD * )RGBA[ 25 ] ) = j < 13           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 13 ) ) ;
							*( ( DWORD * )RGBA[ 26 ] ) = j < 14           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 14 ) ) ;
							*( ( DWORD * )RGBA[ 27 ] ) = j + 13 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 13 ) ) ;
							*( ( DWORD * )RGBA[ 28 ] ) = j + 14 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 14 ) ) ;

							Dest[ 0 ] = ( BYTE )(
								( RGBA[ 0 ][ 0 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 0 ] + RGBA[  2 ][ 0 ] + RGBA[  3 ][ 0 ] + RGBA[  4 ][ 0 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 0 ] + RGBA[  6 ][ 0 ] + RGBA[  7 ][ 0 ] + RGBA[  8 ][ 0 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 0 ] + RGBA[ 10 ][ 0 ] + RGBA[ 11 ][ 0 ] + RGBA[ 12 ][ 0 ] ) * iTable[ 3 ] +
									  ( RGBA[ 13 ][ 0 ] + RGBA[ 14 ][ 0 ] + RGBA[ 15 ][ 0 ] + RGBA[ 16 ][ 0 ] ) * iTable[ 4 ] +
									  ( RGBA[ 17 ][ 0 ] + RGBA[ 18 ][ 0 ] + RGBA[ 19 ][ 0 ] + RGBA[ 20 ][ 0 ] ) * iTable[ 5 ] +
									  ( RGBA[ 21 ][ 0 ] + RGBA[ 22 ][ 0 ] + RGBA[ 23 ][ 0 ] + RGBA[ 24 ][ 0 ] ) * iTable[ 6 ] +
									  ( RGBA[ 25 ][ 0 ] + RGBA[ 26 ][ 0 ] + RGBA[ 27 ][ 0 ] + RGBA[ 28 ][ 0 ] ) * iTable[ 7 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 1 ] = ( BYTE )(
								( RGBA[ 0 ][ 1 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 1 ] + RGBA[  2 ][ 1 ] + RGBA[  3 ][ 1 ] + RGBA[  4 ][ 1 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 1 ] + RGBA[  6 ][ 1 ] + RGBA[  7 ][ 1 ] + RGBA[  8 ][ 1 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 1 ] + RGBA[ 10 ][ 1 ] + RGBA[ 11 ][ 1 ] + RGBA[ 12 ][ 1 ] ) * iTable[ 3 ] +
									  ( RGBA[ 13 ][ 1 ] + RGBA[ 14 ][ 1 ] + RGBA[ 15 ][ 1 ] + RGBA[ 16 ][ 1 ] ) * iTable[ 4 ] +
									  ( RGBA[ 17 ][ 1 ] + RGBA[ 18 ][ 1 ] + RGBA[ 19 ][ 1 ] + RGBA[ 20 ][ 1 ] ) * iTable[ 5 ] +
									  ( RGBA[ 21 ][ 1 ] + RGBA[ 22 ][ 1 ] + RGBA[ 23 ][ 1 ] + RGBA[ 24 ][ 1 ] ) * iTable[ 6 ] +
									  ( RGBA[ 25 ][ 1 ] + RGBA[ 26 ][ 1 ] + RGBA[ 27 ][ 1 ] + RGBA[ 28 ][ 1 ] ) * iTable[ 7 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 2 ] = ( BYTE )(
								( RGBA[ 0 ][ 2 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 2 ] + RGBA[  2 ][ 2 ] + RGBA[  3 ][ 2 ] + RGBA[  4 ][ 2 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 2 ] + RGBA[  6 ][ 2 ] + RGBA[  7 ][ 2 ] + RGBA[  8 ][ 2 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 2 ] + RGBA[ 10 ][ 2 ] + RGBA[ 11 ][ 2 ] + RGBA[ 12 ][ 2 ] ) * iTable[ 3 ] +
									  ( RGBA[ 13 ][ 2 ] + RGBA[ 14 ][ 2 ] + RGBA[ 15 ][ 2 ] + RGBA[ 16 ][ 2 ] ) * iTable[ 4 ] +
									  ( RGBA[ 17 ][ 2 ] + RGBA[ 18 ][ 2 ] + RGBA[ 19 ][ 2 ] + RGBA[ 20 ][ 2 ] ) * iTable[ 5 ] +
									  ( RGBA[ 21 ][ 2 ] + RGBA[ 22 ][ 2 ] + RGBA[ 23 ][ 2 ] + RGBA[ 24 ][ 2 ] ) * iTable[ 6 ] +
									  ( RGBA[ 25 ][ 2 ] + RGBA[ 26 ][ 2 ] + RGBA[ 27 ][ 2 ] + RGBA[ 28 ][ 2 ] ) * iTable[ 7 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 3 ] = ( BYTE )(
								( RGBA[ 0 ][ 3 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 3 ] + RGBA[  2 ][ 3 ] + RGBA[  3 ][ 3 ] + RGBA[  4 ][ 3 ] ) * iTable[ 1 ] +
									  ( RGBA[  5 ][ 3 ] + RGBA[  6 ][ 3 ] + RGBA[  7 ][ 3 ] + RGBA[  8 ][ 3 ] ) * iTable[ 2 ] +
									  ( RGBA[  9 ][ 3 ] + RGBA[ 10 ][ 3 ] + RGBA[ 11 ][ 3 ] + RGBA[ 12 ][ 3 ] ) * iTable[ 3 ] +
									  ( RGBA[ 13 ][ 3 ] + RGBA[ 14 ][ 3 ] + RGBA[ 15 ][ 3 ] + RGBA[ 16 ][ 3 ] ) * iTable[ 4 ] +
									  ( RGBA[ 17 ][ 3 ] + RGBA[ 18 ][ 3 ] + RGBA[ 19 ][ 3 ] + RGBA[ 20 ][ 3 ] ) * iTable[ 5 ] +
									  ( RGBA[ 21 ][ 3 ] + RGBA[ 22 ][ 3 ] + RGBA[ 23 ][ 3 ] + RGBA[ 24 ][ 3 ] ) * iTable[ 6 ] +
									  ( RGBA[ 25 ][ 3 ] + RGBA[ 26 ][ 3 ] + RGBA[ 27 ][ 3 ] + RGBA[ 28 ][ 3 ] ) * iTable[ 7 ] ) >> 1 ) ) >> 16 ) ;

							MinSrc += 4 ;
							MaxSrc += 4 ;
							Src  += 4 ;
							Dest += 4 ;
						}

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}
					break ;

				case 32 :
					for( j = 0 ; j < Height ; j ++ )
					{
						MinSrc = MinSrcStart ;
						MaxSrc = MaxSrcStart ;

						for( i = 0 ; i < Width ; i ++ )
						{
							*( ( DWORD * )RGBA[  0 ] ) = *( ( DWORD * )Src ) ;
							*( ( DWORD * )RGBA[  1 ] ) = j <  1           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  1 ) ) ;
							*( ( DWORD * )RGBA[  2 ] ) = j <  2           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  2 ) ) ;
							*( ( DWORD * )RGBA[  3 ] ) = j +  1 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  1 ) ) ;
							*( ( DWORD * )RGBA[  4 ] ) = j +  2 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  2 ) ) ;
							*( ( DWORD * )RGBA[  5 ] ) = j <  3           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  3 ) ) ;
							*( ( DWORD * )RGBA[  6 ] ) = j <  4           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  4 ) ) ;
							*( ( DWORD * )RGBA[  7 ] ) = j +  3 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  3 ) ) ;
							*( ( DWORD * )RGBA[  8 ] ) = j +  4 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  4 ) ) ;
							*( ( DWORD * )RGBA[  9 ] ) = j <  5           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  5 ) ) ;
							*( ( DWORD * )RGBA[ 10 ] ) = j <  6           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  6 ) ) ;
							*( ( DWORD * )RGBA[ 11 ] ) = j +  5 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  5 ) ) ;
							*( ( DWORD * )RGBA[ 12 ] ) = j +  6 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  6 ) ) ;
							*( ( DWORD * )RGBA[ 13 ] ) = j <  7           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  7 ) ) ;
							*( ( DWORD * )RGBA[ 14 ] ) = j <  8           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  8 ) ) ;
							*( ( DWORD * )RGBA[ 15 ] ) = j +  7 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  7 ) ) ;
							*( ( DWORD * )RGBA[ 16 ] ) = j +  8 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  8 ) ) ;
							*( ( DWORD * )RGBA[ 17 ] ) = j <  9           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch *  9 ) ) ;
							*( ( DWORD * )RGBA[ 18 ] ) = j < 10           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 10 ) ) ;
							*( ( DWORD * )RGBA[ 19 ] ) = j +  9 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch *  9 ) ) ;
							*( ( DWORD * )RGBA[ 20 ] ) = j + 10 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 10 ) ) ;
							*( ( DWORD * )RGBA[ 21 ] ) = j < 11           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 11 ) ) ;
							*( ( DWORD * )RGBA[ 22 ] ) = j < 12           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 12 ) ) ;
							*( ( DWORD * )RGBA[ 23 ] ) = j + 11 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 11 ) ) ;
							*( ( DWORD * )RGBA[ 24 ] ) = j + 12 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 12 ) ) ;
							*( ( DWORD * )RGBA[ 25 ] ) = j < 13           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 13 ) ) ;
							*( ( DWORD * )RGBA[ 26 ] ) = j < 14           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 14 ) ) ;
							*( ( DWORD * )RGBA[ 27 ] ) = j + 13 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 13 ) ) ;
							*( ( DWORD * )RGBA[ 28 ] ) = j + 14 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 14 ) ) ;
							*( ( DWORD * )RGBA[ 29 ] ) = j < 15           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 15 ) ) ;
							*( ( DWORD * )RGBA[ 30 ] ) = j < 16           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 16 ) ) ;
							*( ( DWORD * )RGBA[ 31 ] ) = j + 15 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 15 ) ) ;
							*( ( DWORD * )RGBA[ 32 ] ) = j + 16 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 16 ) ) ;
							*( ( DWORD * )RGBA[ 33 ] ) = j < 17           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 17 ) ) ;
							*( ( DWORD * )RGBA[ 34 ] ) = j < 18           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 18 ) ) ;
							*( ( DWORD * )RGBA[ 35 ] ) = j + 17 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 17 ) ) ;
							*( ( DWORD * )RGBA[ 36 ] ) = j + 18 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 18 ) ) ;
							*( ( DWORD * )RGBA[ 37 ] ) = j < 19           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 19 ) ) ;
							*( ( DWORD * )RGBA[ 38 ] ) = j < 20           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 20 ) ) ;
							*( ( DWORD * )RGBA[ 39 ] ) = j + 19 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 19 ) ) ;
							*( ( DWORD * )RGBA[ 40 ] ) = j + 20 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 20 ) ) ;
							*( ( DWORD * )RGBA[ 41 ] ) = j < 21           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 21 ) ) ;
							*( ( DWORD * )RGBA[ 42 ] ) = j < 22           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 22 ) ) ;
							*( ( DWORD * )RGBA[ 43 ] ) = j + 21 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 21 ) ) ;
							*( ( DWORD * )RGBA[ 44 ] ) = j + 22 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 22 ) ) ;
							*( ( DWORD * )RGBA[ 45 ] ) = j < 23           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 23 ) ) ;
							*( ( DWORD * )RGBA[ 46 ] ) = j < 24           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 24 ) ) ;
							*( ( DWORD * )RGBA[ 47 ] ) = j + 23 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 23 ) ) ;
							*( ( DWORD * )RGBA[ 48 ] ) = j + 24 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 24 ) ) ;
							*( ( DWORD * )RGBA[ 49 ] ) = j < 25           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 25 ) ) ;
							*( ( DWORD * )RGBA[ 50 ] ) = j < 26           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 26 ) ) ;
							*( ( DWORD * )RGBA[ 51 ] ) = j + 25 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 25 ) ) ;
							*( ( DWORD * )RGBA[ 52 ] ) = j + 26 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 26 ) ) ;
							*( ( DWORD * )RGBA[ 53 ] ) = j < 27           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 27 ) ) ;
							*( ( DWORD * )RGBA[ 54 ] ) = j < 28           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 28 ) ) ;
							*( ( DWORD * )RGBA[ 55 ] ) = j + 27 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 27 ) ) ;
							*( ( DWORD * )RGBA[ 56 ] ) = j + 28 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 28 ) ) ;
							*( ( DWORD * )RGBA[ 57 ] ) = j < 29           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 29 ) ) ;
							*( ( DWORD * )RGBA[ 58 ] ) = j < 30           ? *( ( DWORD * )MinSrc ) : *( ( DWORD * )( Src - SrcPitch * 30 ) ) ;
							*( ( DWORD * )RGBA[ 59 ] ) = j + 29 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 29 ) ) ;
							*( ( DWORD * )RGBA[ 60 ] ) = j + 30 >= Height ? *( ( DWORD * )MaxSrc ) : *( ( DWORD * )( Src + SrcPitch * 30 ) ) ;

							Dest[ 0 ] = ( BYTE )(
								( RGBA[ 0 ][ 0 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 0 ] + RGBA[  2 ][ 0 ] + RGBA[  3 ][ 0 ] + RGBA[  4 ][ 0 ] ) * iTable[  1 ] +
									  ( RGBA[  5 ][ 0 ] + RGBA[  6 ][ 0 ] + RGBA[  7 ][ 0 ] + RGBA[  8 ][ 0 ] ) * iTable[  2 ] +
									  ( RGBA[  9 ][ 0 ] + RGBA[ 10 ][ 0 ] + RGBA[ 11 ][ 0 ] + RGBA[ 12 ][ 0 ] ) * iTable[  3 ] +
									  ( RGBA[ 13 ][ 0 ] + RGBA[ 14 ][ 0 ] + RGBA[ 15 ][ 0 ] + RGBA[ 16 ][ 0 ] ) * iTable[  4 ] +
									  ( RGBA[ 17 ][ 0 ] + RGBA[ 18 ][ 0 ] + RGBA[ 19 ][ 0 ] + RGBA[ 20 ][ 0 ] ) * iTable[  5 ] +
									  ( RGBA[ 21 ][ 0 ] + RGBA[ 22 ][ 0 ] + RGBA[ 23 ][ 0 ] + RGBA[ 24 ][ 0 ] ) * iTable[  6 ] +
									  ( RGBA[ 25 ][ 0 ] + RGBA[ 26 ][ 0 ] + RGBA[ 27 ][ 0 ] + RGBA[ 28 ][ 0 ] ) * iTable[  7 ] +
									  ( RGBA[ 29 ][ 0 ] + RGBA[ 30 ][ 0 ] + RGBA[ 31 ][ 0 ] + RGBA[ 32 ][ 0 ] ) * iTable[  8 ] +
									  ( RGBA[ 33 ][ 0 ] + RGBA[ 34 ][ 0 ] + RGBA[ 35 ][ 0 ] + RGBA[ 36 ][ 0 ] ) * iTable[  9 ] +
									  ( RGBA[ 37 ][ 0 ] + RGBA[ 38 ][ 0 ] + RGBA[ 39 ][ 0 ] + RGBA[ 40 ][ 0 ] ) * iTable[ 10 ] +
									  ( RGBA[ 41 ][ 0 ] + RGBA[ 42 ][ 0 ] + RGBA[ 43 ][ 0 ] + RGBA[ 44 ][ 0 ] ) * iTable[ 11 ] +
									  ( RGBA[ 45 ][ 0 ] + RGBA[ 46 ][ 0 ] + RGBA[ 47 ][ 0 ] + RGBA[ 48 ][ 0 ] ) * iTable[ 12 ] +
									  ( RGBA[ 49 ][ 0 ] + RGBA[ 50 ][ 0 ] + RGBA[ 51 ][ 0 ] + RGBA[ 52 ][ 0 ] ) * iTable[ 13 ] +
									  ( RGBA[ 53 ][ 0 ] + RGBA[ 54 ][ 0 ] + RGBA[ 55 ][ 0 ] + RGBA[ 56 ][ 0 ] ) * iTable[ 14 ] +
									  ( RGBA[ 57 ][ 0 ] + RGBA[ 58 ][ 0 ] + RGBA[ 59 ][ 0 ] + RGBA[ 60 ][ 0 ] ) * iTable[ 15 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 1 ] = ( BYTE )(
								( RGBA[ 0 ][ 1 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 1 ] + RGBA[  2 ][ 1 ] + RGBA[  3 ][ 1 ] + RGBA[  4 ][ 1 ] ) * iTable[  1 ] +
									  ( RGBA[  5 ][ 1 ] + RGBA[  6 ][ 1 ] + RGBA[  7 ][ 1 ] + RGBA[  8 ][ 1 ] ) * iTable[  2 ] +
									  ( RGBA[  9 ][ 1 ] + RGBA[ 10 ][ 1 ] + RGBA[ 11 ][ 1 ] + RGBA[ 12 ][ 1 ] ) * iTable[  3 ] +
									  ( RGBA[ 13 ][ 1 ] + RGBA[ 14 ][ 1 ] + RGBA[ 15 ][ 1 ] + RGBA[ 16 ][ 1 ] ) * iTable[  4 ] +
									  ( RGBA[ 17 ][ 1 ] + RGBA[ 18 ][ 1 ] + RGBA[ 19 ][ 1 ] + RGBA[ 20 ][ 1 ] ) * iTable[  5 ] +
									  ( RGBA[ 21 ][ 1 ] + RGBA[ 22 ][ 1 ] + RGBA[ 23 ][ 1 ] + RGBA[ 24 ][ 1 ] ) * iTable[  6 ] +
									  ( RGBA[ 25 ][ 1 ] + RGBA[ 26 ][ 1 ] + RGBA[ 27 ][ 1 ] + RGBA[ 28 ][ 1 ] ) * iTable[  7 ] +
									  ( RGBA[ 29 ][ 1 ] + RGBA[ 30 ][ 1 ] + RGBA[ 31 ][ 1 ] + RGBA[ 32 ][ 1 ] ) * iTable[  8 ] +
									  ( RGBA[ 33 ][ 1 ] + RGBA[ 34 ][ 1 ] + RGBA[ 35 ][ 1 ] + RGBA[ 36 ][ 1 ] ) * iTable[  9 ] +
									  ( RGBA[ 37 ][ 1 ] + RGBA[ 38 ][ 1 ] + RGBA[ 39 ][ 1 ] + RGBA[ 40 ][ 1 ] ) * iTable[ 10 ] +
									  ( RGBA[ 41 ][ 1 ] + RGBA[ 42 ][ 1 ] + RGBA[ 43 ][ 1 ] + RGBA[ 44 ][ 1 ] ) * iTable[ 11 ] +
									  ( RGBA[ 45 ][ 1 ] + RGBA[ 46 ][ 1 ] + RGBA[ 47 ][ 1 ] + RGBA[ 48 ][ 1 ] ) * iTable[ 12 ] +
									  ( RGBA[ 49 ][ 1 ] + RGBA[ 50 ][ 1 ] + RGBA[ 51 ][ 1 ] + RGBA[ 52 ][ 1 ] ) * iTable[ 13 ] +
									  ( RGBA[ 53 ][ 1 ] + RGBA[ 54 ][ 1 ] + RGBA[ 55 ][ 1 ] + RGBA[ 56 ][ 1 ] ) * iTable[ 14 ] +
									  ( RGBA[ 57 ][ 1 ] + RGBA[ 58 ][ 1 ] + RGBA[ 59 ][ 1 ] + RGBA[ 60 ][ 1 ] ) * iTable[ 15 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 2 ] = ( BYTE )(
								( RGBA[ 0 ][ 2 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 2 ] + RGBA[  2 ][ 2 ] + RGBA[  3 ][ 2 ] + RGBA[  4 ][ 2 ] ) * iTable[  1 ] +
									  ( RGBA[  5 ][ 2 ] + RGBA[  6 ][ 2 ] + RGBA[  7 ][ 2 ] + RGBA[  8 ][ 2 ] ) * iTable[  2 ] +
									  ( RGBA[  9 ][ 2 ] + RGBA[ 10 ][ 2 ] + RGBA[ 11 ][ 2 ] + RGBA[ 12 ][ 2 ] ) * iTable[  3 ] +
									  ( RGBA[ 13 ][ 2 ] + RGBA[ 14 ][ 2 ] + RGBA[ 15 ][ 2 ] + RGBA[ 16 ][ 2 ] ) * iTable[  4 ] +
									  ( RGBA[ 17 ][ 2 ] + RGBA[ 18 ][ 2 ] + RGBA[ 19 ][ 2 ] + RGBA[ 20 ][ 2 ] ) * iTable[  5 ] +
									  ( RGBA[ 21 ][ 2 ] + RGBA[ 22 ][ 2 ] + RGBA[ 23 ][ 2 ] + RGBA[ 24 ][ 2 ] ) * iTable[  6 ] +
									  ( RGBA[ 25 ][ 2 ] + RGBA[ 26 ][ 2 ] + RGBA[ 27 ][ 2 ] + RGBA[ 28 ][ 2 ] ) * iTable[  7 ] +
									  ( RGBA[ 29 ][ 2 ] + RGBA[ 30 ][ 2 ] + RGBA[ 31 ][ 2 ] + RGBA[ 32 ][ 2 ] ) * iTable[  8 ] +
									  ( RGBA[ 33 ][ 2 ] + RGBA[ 34 ][ 2 ] + RGBA[ 35 ][ 2 ] + RGBA[ 36 ][ 2 ] ) * iTable[  9 ] +
									  ( RGBA[ 37 ][ 2 ] + RGBA[ 38 ][ 2 ] + RGBA[ 39 ][ 2 ] + RGBA[ 40 ][ 2 ] ) * iTable[ 10 ] +
									  ( RGBA[ 41 ][ 2 ] + RGBA[ 42 ][ 2 ] + RGBA[ 43 ][ 2 ] + RGBA[ 44 ][ 2 ] ) * iTable[ 11 ] +
									  ( RGBA[ 45 ][ 2 ] + RGBA[ 46 ][ 2 ] + RGBA[ 47 ][ 2 ] + RGBA[ 48 ][ 2 ] ) * iTable[ 12 ] +
									  ( RGBA[ 49 ][ 2 ] + RGBA[ 50 ][ 2 ] + RGBA[ 51 ][ 2 ] + RGBA[ 52 ][ 2 ] ) * iTable[ 13 ] +
									  ( RGBA[ 53 ][ 2 ] + RGBA[ 54 ][ 2 ] + RGBA[ 55 ][ 2 ] + RGBA[ 56 ][ 2 ] ) * iTable[ 14 ] +
									  ( RGBA[ 57 ][ 2 ] + RGBA[ 58 ][ 2 ] + RGBA[ 59 ][ 2 ] + RGBA[ 60 ][ 2 ] ) * iTable[ 15 ] ) >> 1 ) ) >> 16 ) ;

							Dest[ 3 ] = ( BYTE )(
								( RGBA[ 0 ][ 3 ] * iTable[ 0 ] +
								  ( ( ( RGBA[  1 ][ 3 ] + RGBA[  2 ][ 3 ] + RGBA[  3 ][ 3 ] + RGBA[  4 ][ 3 ] ) * iTable[  1 ] +
									  ( RGBA[  5 ][ 3 ] + RGBA[  6 ][ 3 ] + RGBA[  7 ][ 3 ] + RGBA[  8 ][ 3 ] ) * iTable[  2 ] +
									  ( RGBA[  9 ][ 3 ] + RGBA[ 10 ][ 3 ] + RGBA[ 11 ][ 3 ] + RGBA[ 12 ][ 3 ] ) * iTable[  3 ] +
									  ( RGBA[ 13 ][ 3 ] + RGBA[ 14 ][ 3 ] + RGBA[ 15 ][ 3 ] + RGBA[ 16 ][ 3 ] ) * iTable[  4 ] +
									  ( RGBA[ 17 ][ 3 ] + RGBA[ 18 ][ 3 ] + RGBA[ 19 ][ 3 ] + RGBA[ 20 ][ 3 ] ) * iTable[  5 ] +
									  ( RGBA[ 21 ][ 3 ] + RGBA[ 22 ][ 3 ] + RGBA[ 23 ][ 3 ] + RGBA[ 24 ][ 3 ] ) * iTable[  6 ] +
									  ( RGBA[ 25 ][ 3 ] + RGBA[ 26 ][ 3 ] + RGBA[ 27 ][ 3 ] + RGBA[ 28 ][ 3 ] ) * iTable[  7 ] +
									  ( RGBA[ 29 ][ 3 ] + RGBA[ 30 ][ 3 ] + RGBA[ 31 ][ 3 ] + RGBA[ 32 ][ 3 ] ) * iTable[  8 ] +
									  ( RGBA[ 33 ][ 3 ] + RGBA[ 34 ][ 3 ] + RGBA[ 35 ][ 3 ] + RGBA[ 36 ][ 3 ] ) * iTable[  9 ] +
									  ( RGBA[ 37 ][ 3 ] + RGBA[ 38 ][ 3 ] + RGBA[ 39 ][ 3 ] + RGBA[ 40 ][ 3 ] ) * iTable[ 10 ] +
									  ( RGBA[ 41 ][ 3 ] + RGBA[ 42 ][ 3 ] + RGBA[ 43 ][ 3 ] + RGBA[ 44 ][ 3 ] ) * iTable[ 11 ] +
									  ( RGBA[ 45 ][ 3 ] + RGBA[ 46 ][ 3 ] + RGBA[ 47 ][ 3 ] + RGBA[ 48 ][ 3 ] ) * iTable[ 12 ] +
									  ( RGBA[ 49 ][ 3 ] + RGBA[ 50 ][ 3 ] + RGBA[ 51 ][ 3 ] + RGBA[ 52 ][ 3 ] ) * iTable[ 13 ] +
									  ( RGBA[ 53 ][ 3 ] + RGBA[ 54 ][ 3 ] + RGBA[ 55 ][ 3 ] + RGBA[ 56 ][ 3 ] ) * iTable[ 14 ] +
									  ( RGBA[ 57 ][ 3 ] + RGBA[ 58 ][ 3 ] + RGBA[ 59 ][ 3 ] + RGBA[ 60 ][ 3 ] ) * iTable[ 15 ] ) >> 1 ) ) >> 16 ) ;

							MinSrc += 4 ;
							MaxSrc += 4 ;
							Src  += 4 ;
							Dest += 4 ;
						}

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}
					break ;
				}

				NS_ReleaseBaseImage( &WorkImage ) ;
			}
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int GraphFilter_Down_Scale( GRAPHFILTER_INFO *Info, int DivNum )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_Down_Scale_PF( Info, DivNum ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD SrcWidth ;
		DWORD SrcHeight ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcPitch ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;

		SrcWidth  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		SrcHeight = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;
		Width  = SrcWidth  / DivNum ;
		Height = SrcHeight / DivNum ;

		if( GraphFilter_SoftImageSetup( Info, TRUE, ( int )Width, ( int )Height ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		SrcPitch     = ( DWORD )Info->SrcBaseImage.Pitch ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		switch( DivNum )
		{
		case 1 :
			SrcAddPitch  = Info->SrcBaseImage.Pitch - Width * 4 ;

			do
			{
				i = Width ;
				do
				{
					Dest[ 0 ] = Src[ 0 ] ;
					Dest[ 1 ] = Src[ 1 ] ;
					Dest[ 2 ] = Src[ 2 ] ;
					Dest[ 3 ] = Src[ 3 ] ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
			break ;

		case 2 :
			SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 * 2 + SrcPitch ;

			do
			{
				i = Width ;
				do
				{
					Dest[ 0 ] = ( BYTE )( ( Src[ 0 ] + Src[ 4 ] + Src[ SrcPitch + 0 ] + Src[ SrcPitch + 4 ] ) >> 2 ) ;
					Dest[ 1 ] = ( BYTE )( ( Src[ 1 ] + Src[ 5 ] + Src[ SrcPitch + 1 ] + Src[ SrcPitch + 5 ] ) >> 2 ) ;
					Dest[ 2 ] = ( BYTE )( ( Src[ 2 ] + Src[ 6 ] + Src[ SrcPitch + 2 ] + Src[ SrcPitch + 6 ] ) >> 2 ) ;
					Dest[ 3 ] = ( BYTE )( ( Src[ 3 ] + Src[ 7 ] + Src[ SrcPitch + 3 ] + Src[ SrcPitch + 7 ] ) >> 2 ) ;

					Src  += 4 * 2 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
			break ;

		case 4 :
			SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 * 4 + SrcPitch * 3 ;

			do
			{
				i = Width ;
				do
				{
					Dest[ 0 ] = ( BYTE )( (
						Src[                0 ] + Src[                4 ] + Src[                 8 ] + Src[                12 ] +
						Src[ SrcPitch     + 0 ] + Src[ SrcPitch     + 4 ] +	Src[ SrcPitch     +  8 ] + Src[ SrcPitch     + 12 ] +
						Src[ SrcPitch * 2 + 0 ] + Src[ SrcPitch * 2 + 4 ] +	Src[ SrcPitch * 2 +  8 ] + Src[ SrcPitch * 2 + 12 ] +
						Src[ SrcPitch * 3 + 0 ] + Src[ SrcPitch * 3 + 4 ] +	Src[ SrcPitch * 3 +  8 ] + Src[ SrcPitch * 3 + 12 ] ) >> 4 ) ;
					Dest[ 1 ] = ( BYTE )( (
						Src[                1 ] + Src[                5 ] + Src[                 9 ] + Src[                13 ] +
						Src[ SrcPitch     + 1 ] + Src[ SrcPitch     + 5 ] +	Src[ SrcPitch     +  9 ] + Src[ SrcPitch     + 13 ] +
						Src[ SrcPitch * 2 + 1 ] + Src[ SrcPitch * 2 + 5 ] +	Src[ SrcPitch * 2 +  9 ] + Src[ SrcPitch * 2 + 13 ] +
						Src[ SrcPitch * 3 + 1 ] + Src[ SrcPitch * 3 + 5 ] +	Src[ SrcPitch * 3 +  9 ] + Src[ SrcPitch * 3 + 13 ] ) >> 4 ) ;
					Dest[ 2 ] = ( BYTE )( (
						Src[                2 ] + Src[                6 ] + Src[                10 ] + Src[                14 ] +
						Src[ SrcPitch     + 2 ] + Src[ SrcPitch     + 6 ] +	Src[ SrcPitch     + 10 ] + Src[ SrcPitch     + 14 ] +
						Src[ SrcPitch * 2 + 2 ] + Src[ SrcPitch * 2 + 6 ] +	Src[ SrcPitch * 2 + 10 ] + Src[ SrcPitch * 2 + 14 ] +
						Src[ SrcPitch * 3 + 2 ] + Src[ SrcPitch * 3 + 6 ] +	Src[ SrcPitch * 3 + 10 ] + Src[ SrcPitch * 3 + 14 ] ) >> 4 ) ;
					Dest[ 3 ] = ( BYTE )( (
						Src[                3 ] + Src[                7 ] + Src[                11 ] + Src[                15 ] +
						Src[ SrcPitch     + 3 ] + Src[ SrcPitch     + 7 ] +	Src[ SrcPitch     + 11 ] + Src[ SrcPitch     + 15 ] +
						Src[ SrcPitch * 2 + 3 ] + Src[ SrcPitch * 2 + 7 ] +	Src[ SrcPitch * 2 + 11 ] + Src[ SrcPitch * 2 + 15 ] +
						Src[ SrcPitch * 3 + 3 ] + Src[ SrcPitch * 3 + 7 ] +	Src[ SrcPitch * 3 + 11 ] + Src[ SrcPitch * 3 + 15 ] ) >> 4 ) ;

					Src  += 4 * 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
			break ;

		case 8 :
			SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 * 8 + SrcPitch * 7 ;

			do
			{
				i = Width ;
				do
				{
					Dest[ 0 ] = ( BYTE )( (
						Src[                0 ] + Src[                4 ] + Src[                 8 ] + Src[                12 ] + Src[                16 ] + Src[                20 ] + Src[                24 ] + Src[                28 ] +
						Src[ SrcPitch     + 0 ] + Src[ SrcPitch     + 4 ] +	Src[ SrcPitch     +  8 ] + Src[ SrcPitch     + 12 ] + Src[ SrcPitch     + 16 ] + Src[ SrcPitch     + 20 ] + Src[ SrcPitch     + 24 ] + Src[ SrcPitch     + 28 ] +
						Src[ SrcPitch * 2 + 0 ] + Src[ SrcPitch * 2 + 4 ] +	Src[ SrcPitch * 2 +  8 ] + Src[ SrcPitch * 2 + 12 ] + Src[ SrcPitch * 2 + 16 ] + Src[ SrcPitch * 2 + 20 ] + Src[ SrcPitch * 2 + 24 ] + Src[ SrcPitch * 2 + 28 ] +
						Src[ SrcPitch * 3 + 0 ] + Src[ SrcPitch * 3 + 4 ] +	Src[ SrcPitch * 3 +  8 ] + Src[ SrcPitch * 3 + 12 ] + Src[ SrcPitch * 3 + 16 ] + Src[ SrcPitch * 3 + 20 ] + Src[ SrcPitch * 3 + 24 ] + Src[ SrcPitch * 3 + 28 ] +
						Src[ SrcPitch * 4 + 0 ] + Src[ SrcPitch * 4 + 4 ] +	Src[ SrcPitch * 4 +  8 ] + Src[ SrcPitch * 4 + 12 ] + Src[ SrcPitch * 4 + 16 ] + Src[ SrcPitch * 4 + 20 ] + Src[ SrcPitch * 4 + 24 ] + Src[ SrcPitch * 4 + 28 ] +
						Src[ SrcPitch * 5 + 0 ] + Src[ SrcPitch * 5 + 4 ] +	Src[ SrcPitch * 5 +  8 ] + Src[ SrcPitch * 5 + 12 ] + Src[ SrcPitch * 5 + 16 ] + Src[ SrcPitch * 5 + 20 ] + Src[ SrcPitch * 5 + 24 ] + Src[ SrcPitch * 5 + 28 ] +
						Src[ SrcPitch * 6 + 0 ] + Src[ SrcPitch * 6 + 4 ] +	Src[ SrcPitch * 6 +  8 ] + Src[ SrcPitch * 6 + 12 ] + Src[ SrcPitch * 6 + 16 ] + Src[ SrcPitch * 6 + 20 ] + Src[ SrcPitch * 6 + 24 ] + Src[ SrcPitch * 6 + 28 ] +
						Src[ SrcPitch * 7 + 0 ] + Src[ SrcPitch * 7 + 4 ] +	Src[ SrcPitch * 7 +  8 ] + Src[ SrcPitch * 7 + 12 ] + Src[ SrcPitch * 7 + 16 ] + Src[ SrcPitch * 7 + 20 ] + Src[ SrcPitch * 7 + 24 ] + Src[ SrcPitch * 7 + 28 ] ) >> 6 ) ;
					Dest[ 1 ] = ( BYTE )( (
						Src[                1 ] + Src[                5 ] + Src[                 9 ] + Src[                13 ] + Src[                17 ] + Src[                21 ] + Src[                25 ] + Src[                29 ] +
						Src[ SrcPitch     + 1 ] + Src[ SrcPitch     + 5 ] +	Src[ SrcPitch     +  9 ] + Src[ SrcPitch     + 13 ] + Src[ SrcPitch     + 17 ] + Src[ SrcPitch     + 21 ] + Src[ SrcPitch     + 25 ] + Src[ SrcPitch     + 29 ] +
						Src[ SrcPitch * 2 + 1 ] + Src[ SrcPitch * 2 + 5 ] +	Src[ SrcPitch * 2 +  9 ] + Src[ SrcPitch * 2 + 13 ] + Src[ SrcPitch * 2 + 17 ] + Src[ SrcPitch * 2 + 21 ] + Src[ SrcPitch * 2 + 25 ] + Src[ SrcPitch * 2 + 29 ] +
						Src[ SrcPitch * 3 + 1 ] + Src[ SrcPitch * 3 + 5 ] +	Src[ SrcPitch * 3 +  9 ] + Src[ SrcPitch * 3 + 13 ] + Src[ SrcPitch * 3 + 17 ] + Src[ SrcPitch * 3 + 21 ] + Src[ SrcPitch * 3 + 25 ] + Src[ SrcPitch * 3 + 29 ] +
						Src[ SrcPitch * 4 + 1 ] + Src[ SrcPitch * 4 + 5 ] +	Src[ SrcPitch * 4 +  9 ] + Src[ SrcPitch * 4 + 13 ] + Src[ SrcPitch * 4 + 17 ] + Src[ SrcPitch * 4 + 21 ] + Src[ SrcPitch * 4 + 25 ] + Src[ SrcPitch * 4 + 29 ] +
						Src[ SrcPitch * 5 + 1 ] + Src[ SrcPitch * 5 + 5 ] +	Src[ SrcPitch * 5 +  9 ] + Src[ SrcPitch * 5 + 13 ] + Src[ SrcPitch * 5 + 17 ] + Src[ SrcPitch * 5 + 21 ] + Src[ SrcPitch * 5 + 25 ] + Src[ SrcPitch * 5 + 29 ] +
						Src[ SrcPitch * 6 + 1 ] + Src[ SrcPitch * 6 + 5 ] +	Src[ SrcPitch * 6 +  9 ] + Src[ SrcPitch * 6 + 13 ] + Src[ SrcPitch * 6 + 17 ] + Src[ SrcPitch * 6 + 21 ] + Src[ SrcPitch * 6 + 25 ] + Src[ SrcPitch * 6 + 29 ] +
						Src[ SrcPitch * 7 + 1 ] + Src[ SrcPitch * 7 + 5 ] +	Src[ SrcPitch * 7 +  9 ] + Src[ SrcPitch * 7 + 13 ] + Src[ SrcPitch * 7 + 17 ] + Src[ SrcPitch * 7 + 21 ] + Src[ SrcPitch * 7 + 25 ] + Src[ SrcPitch * 7 + 29 ] ) >> 6 ) ;
					Dest[ 2 ] = ( BYTE )( (
						Src[                2 ] + Src[                6 ] + Src[                10 ] + Src[                14 ] + Src[                18 ] + Src[                22 ] + Src[                26 ] + Src[                30 ] +
						Src[ SrcPitch     + 2 ] + Src[ SrcPitch     + 6 ] +	Src[ SrcPitch     + 10 ] + Src[ SrcPitch     + 14 ] + Src[ SrcPitch     + 18 ] + Src[ SrcPitch     + 22 ] + Src[ SrcPitch     + 26 ] + Src[ SrcPitch     + 30 ] +
						Src[ SrcPitch * 2 + 2 ] + Src[ SrcPitch * 2 + 6 ] +	Src[ SrcPitch * 2 + 10 ] + Src[ SrcPitch * 2 + 14 ] + Src[ SrcPitch * 2 + 18 ] + Src[ SrcPitch * 2 + 22 ] + Src[ SrcPitch * 2 + 26 ] + Src[ SrcPitch * 2 + 30 ] +
						Src[ SrcPitch * 3 + 2 ] + Src[ SrcPitch * 3 + 6 ] +	Src[ SrcPitch * 3 + 10 ] + Src[ SrcPitch * 3 + 14 ] + Src[ SrcPitch * 3 + 18 ] + Src[ SrcPitch * 3 + 22 ] + Src[ SrcPitch * 3 + 26 ] + Src[ SrcPitch * 3 + 30 ] +
						Src[ SrcPitch * 4 + 2 ] + Src[ SrcPitch * 4 + 6 ] +	Src[ SrcPitch * 4 + 10 ] + Src[ SrcPitch * 4 + 14 ] + Src[ SrcPitch * 4 + 18 ] + Src[ SrcPitch * 4 + 22 ] + Src[ SrcPitch * 4 + 26 ] + Src[ SrcPitch * 4 + 30 ] +
						Src[ SrcPitch * 5 + 2 ] + Src[ SrcPitch * 5 + 6 ] +	Src[ SrcPitch * 5 + 10 ] + Src[ SrcPitch * 5 + 14 ] + Src[ SrcPitch * 5 + 18 ] + Src[ SrcPitch * 5 + 22 ] + Src[ SrcPitch * 5 + 26 ] + Src[ SrcPitch * 5 + 30 ] +
						Src[ SrcPitch * 6 + 2 ] + Src[ SrcPitch * 6 + 6 ] +	Src[ SrcPitch * 6 + 10 ] + Src[ SrcPitch * 6 + 14 ] + Src[ SrcPitch * 6 + 18 ] + Src[ SrcPitch * 6 + 22 ] + Src[ SrcPitch * 6 + 26 ] + Src[ SrcPitch * 6 + 30 ] +
						Src[ SrcPitch * 7 + 2 ] + Src[ SrcPitch * 7 + 6 ] +	Src[ SrcPitch * 7 + 10 ] + Src[ SrcPitch * 7 + 14 ] + Src[ SrcPitch * 7 + 18 ] + Src[ SrcPitch * 7 + 22 ] + Src[ SrcPitch * 7 + 26 ] + Src[ SrcPitch * 7 + 30 ] ) >> 6 ) ;
					Dest[ 3 ] = ( BYTE )( (
						Src[                3 ] + Src[                7 ] + Src[                11 ] + Src[                15 ] + Src[                19 ] + Src[                23 ] + Src[                27 ] + Src[                31 ] +
						Src[ SrcPitch     + 3 ] + Src[ SrcPitch     + 7 ] +	Src[ SrcPitch     + 11 ] + Src[ SrcPitch     + 15 ] + Src[ SrcPitch     + 19 ] + Src[ SrcPitch     + 23 ] + Src[ SrcPitch     + 27 ] + Src[ SrcPitch     + 31 ] +
						Src[ SrcPitch * 2 + 3 ] + Src[ SrcPitch * 2 + 7 ] +	Src[ SrcPitch * 2 + 11 ] + Src[ SrcPitch * 2 + 15 ] + Src[ SrcPitch * 2 + 19 ] + Src[ SrcPitch * 2 + 23 ] + Src[ SrcPitch * 2 + 27 ] + Src[ SrcPitch * 2 + 31 ] +
						Src[ SrcPitch * 3 + 3 ] + Src[ SrcPitch * 3 + 7 ] +	Src[ SrcPitch * 3 + 11 ] + Src[ SrcPitch * 3 + 15 ] + Src[ SrcPitch * 3 + 19 ] + Src[ SrcPitch * 3 + 23 ] + Src[ SrcPitch * 3 + 27 ] + Src[ SrcPitch * 3 + 31 ] +
						Src[ SrcPitch * 4 + 3 ] + Src[ SrcPitch * 4 + 7 ] +	Src[ SrcPitch * 4 + 11 ] + Src[ SrcPitch * 4 + 15 ] + Src[ SrcPitch * 4 + 19 ] + Src[ SrcPitch * 4 + 23 ] + Src[ SrcPitch * 4 + 27 ] + Src[ SrcPitch * 4 + 31 ] +
						Src[ SrcPitch * 5 + 3 ] + Src[ SrcPitch * 5 + 7 ] +	Src[ SrcPitch * 5 + 11 ] + Src[ SrcPitch * 5 + 15 ] + Src[ SrcPitch * 5 + 19 ] + Src[ SrcPitch * 5 + 23 ] + Src[ SrcPitch * 5 + 27 ] + Src[ SrcPitch * 5 + 31 ] +
						Src[ SrcPitch * 6 + 3 ] + Src[ SrcPitch * 6 + 7 ] +	Src[ SrcPitch * 6 + 11 ] + Src[ SrcPitch * 6 + 15 ] + Src[ SrcPitch * 6 + 19 ] + Src[ SrcPitch * 6 + 23 ] + Src[ SrcPitch * 6 + 27 ] + Src[ SrcPitch * 6 + 31 ] +
						Src[ SrcPitch * 7 + 3 ] + Src[ SrcPitch * 7 + 7 ] +	Src[ SrcPitch * 7 + 11 ] + Src[ SrcPitch * 7 + 15 ] + Src[ SrcPitch * 7 + 19 ] + Src[ SrcPitch * 7 + 23 ] + Src[ SrcPitch * 7 + 27 ] + Src[ SrcPitch * 7 + 31 ] ) >> 6 ) ;

					Src  += 4 * 8 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
			break ;
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

// CmpType:比較タイプ( DX_CMP_LESS と DX_CMP_GREATER のみ指定できます )
// CmpParam:比較値( 0.0f～255.0f )
// ClipFillFlag:クリップしたピクセルを特定の色で塗りつぶすか( TRUE:塗りつぶす  FALSE:塗りつぶさない )
// ClipFillColor:クリップしたピクセルに塗る色( GetColorFで取得するもの )
extern int GraphFilter_Bright_Clip( GRAPHFILTER_INFO *Info, int CmpType, float CmpParam, int ClipFillFlag, COLOR_F *ClipFillColor, int IsPMA )
{
	// 値の補正
	if( CmpType != DX_CMP_LESS && CmpType != DX_CMP_GREATER )
		return -1 ;

	if( ClipFillFlag == TRUE && ClipFillColor == NULL )
		return -1 ;

	CmpParam /= 255.0f ;
	if( CmpParam < 0.0f )
	{
		CmpParam = 0.0f ;
	}
	else
	if( CmpParam > 1.0f )
	{
		CmpParam = 1.0f ;
	}

	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_Bright_Clip_PF( Info, CmpType, CmpParam, ClipFillFlag, ClipFillColor, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;
		int Temp ;
		BYTE iClipFillColor[ 4 ] ;
		int iCmpParam ;

		iCmpParam = _FTOL( CmpParam * 255.0f * 4096.0f ) ;

		if( ClipFillColor != NULL )
		{
			Temp = _FTOL( ClipFillColor->b * 255.0f ) ;
			if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
			iClipFillColor[ 0 ] = ( BYTE )Temp ;

			Temp = _FTOL( ClipFillColor->g * 255.0f ) ;
			if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
			iClipFillColor[ 1 ] = ( BYTE )Temp ;

			Temp = _FTOL( ClipFillColor->r * 255.0f ) ;
			if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
			iClipFillColor[ 2 ] = ( BYTE )Temp ;

			Temp = _FTOL( ClipFillColor->a * 255.0f ) ;
			if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
			iClipFillColor[ 3 ] = ( BYTE )Temp ;
		}

		if( GraphFilter_SoftImageSetup( Info, FALSE ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		if( ClipFillFlag )
		{
			if( IsPMA )
			{
				iClipFillColor[ 0 ] = BASEIM.RgbToPmaTable[ iClipFillColor[ 0 ] ][ iClipFillColor[ 3 ] ] ;
				iClipFillColor[ 1 ] = BASEIM.RgbToPmaTable[ iClipFillColor[ 1 ] ][ iClipFillColor[ 3 ] ] ;
				iClipFillColor[ 2 ] = BASEIM.RgbToPmaTable[ iClipFillColor[ 2 ] ][ iClipFillColor[ 3 ] ] ;
			}

			if( CmpType == DX_CMP_GREATER )
			{
				if( IsPMA )
				{
					do
					{
						i = Width ;
						do
						{
							if( BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] * ( int )( 0.114f * 4096.0f ) +
								BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] * ( int )( 0.587f * 4096.0f ) +
								BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] * ( int )( 0.299f * 4096.0f ) > iCmpParam )
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )iClipFillColor ) ;
							}
							else
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;
							}

							Src  += 4 ;
							Dest += 4 ;
						}while( -- i ) ;

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}while( -- Height ) ;
				}
				else
				{
					do
					{
						i = Width ;
						do
						{
							if( Src[ 0 ] * ( int )( 0.114f * 4096.0f ) +
								Src[ 1 ] * ( int )( 0.587f * 4096.0f ) +
								Src[ 2 ] * ( int )( 0.299f * 4096.0f ) > iCmpParam )
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )iClipFillColor ) ;
							}
							else
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;
							}

							Src  += 4 ;
							Dest += 4 ;
						}while( -- i ) ;

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}while( -- Height ) ;
				}
			}
			else
			{
				if( IsPMA )
				{
					do
					{
						i = Width ;
						do
						{
							if( BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] * ( int )( 0.114f * 4096.0f ) +
								BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] * ( int )( 0.587f * 4096.0f ) +
								BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] * ( int )( 0.299f * 4096.0f ) < iCmpParam )
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )iClipFillColor ) ;
							}
							else
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;
							}

							Src  += 4 ;
							Dest += 4 ;
						}while( -- i ) ;

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}while( -- Height ) ;
				}
				else
				{
					do
					{
						i = Width ;
						do
						{
							if( Src[ 0 ] * ( int )( 0.114f * 4096.0f ) +
								Src[ 1 ] * ( int )( 0.587f * 4096.0f ) +
								Src[ 2 ] * ( int )( 0.299f * 4096.0f ) < iCmpParam )
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )iClipFillColor ) ;
							}
							else
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;
							}

							Src  += 4 ;
							Dest += 4 ;
						}while( -- i ) ;

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}while( -- Height ) ;
				}
			}
		}
		else
		{
			if( CmpType == DX_CMP_GREATER )
			{
				if( IsPMA )
				{
					do
					{
						i = Width ;
						do
						{
							if( BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] * ( int )( 0.114f * 4096.0f ) +
								BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] * ( int )( 0.587f * 4096.0f ) +
								BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] * ( int )( 0.299f * 4096.0f ) > iCmpParam )
							{
							}
							else
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;
							}

							Src  += 4 ;
							Dest += 4 ;
						}while( -- i ) ;

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}while( -- Height ) ;
				}
				else
				{
					do
					{
						i = Width ;
						do
						{
							if( Src[ 0 ] * ( int )( 0.114f * 4096.0f ) +
								Src[ 1 ] * ( int )( 0.587f * 4096.0f ) +
								Src[ 2 ] * ( int )( 0.299f * 4096.0f ) > iCmpParam )
							{
							}
							else
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;
							}

							Src  += 4 ;
							Dest += 4 ;
						}while( -- i ) ;

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}while( -- Height ) ;
				}
			}
			else
			{
				if( IsPMA )
				{
					do
					{
						i = Width ;
						do
						{
							if( BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] * ( int )( 0.114f * 4096.0f ) +
								BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] * ( int )( 0.587f * 4096.0f ) +
								BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] * ( int )( 0.299f * 4096.0f ) < iCmpParam )
							{
							}
							else
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;
							}

							Src  += 4 ;
							Dest += 4 ;
						}while( -- i ) ;

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}while( -- Height ) ;
				}
				else
				{
					do
					{
						i = Width ;
						do
						{
							if( Src[ 0 ] * ( int )( 0.114f * 4096.0f ) +
								Src[ 1 ] * ( int )( 0.587f * 4096.0f ) +
								Src[ 2 ] * ( int )( 0.299f * 4096.0f ) < iCmpParam )
							{
							}
							else
							{
								*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;
							}

							Src  += 4 ;
							Dest += 4 ;
						}while( -- i ) ;

						Src  += SrcAddPitch ;
						Dest += DestAddPitch ;
					}while( -- Height ) ;
				}
			}
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

// BrightMin:変換後に真っ黒になる明るさ
// BrightMax:変換後に真っ白になる明るさ
extern int GraphFilter_Bright_Scale( GRAPHFILTER_INFO *Info, int BrightMin, int BrightMax, int IsPMA )
{
	// 値の補正
	if( BrightMin < 0 )
	{
		BrightMin = 0 ;
	}
	if( BrightMin >= 255 )
	{
		BrightMin = 254 ;
	}

	if( BrightMax < 1 )
	{
		BrightMax = 1 ;
	}
	if( BrightMax > 255 )
	{
		BrightMax = 255 ;
	}

	if( BrightMin == BrightMax )
	{
		BrightMax = BrightMin + 1 ;
	}

	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_Bright_Scale_PF( Info, BrightMin, BrightMax, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;
		int iMinB ;
		int iScaleB ;
		int iS, iB ;
		int Max ;
		int Min ;
		int Delta ;
		int R, G, B ;

		iMinB   = BrightMin ;
		iScaleB = ( 255 * 4096 ) / ( BrightMax - BrightMin ) ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		do
		{
			i = Width ;
			do
			{
				if( IsPMA )
				{
					B = BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] ;
					G = BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] ;
					R = BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] ;
				}
				else
				{
					B = Src[ 0 ] ;
					G = Src[ 1 ] ;
					R = Src[ 2 ] ;
				}

				Max = R > G ? ( R > B ? R : B ) : ( G > B ? G : B ) ;
				Min = R < G ? ( R < B ? R : B ) : ( G < B ? G : B ) ;
				Delta = Max - Min;

				iB = ( Max + Min ) >> 1 ;
				if( Max == Min )
				{
					iS = 0 ;

					R = 255 ;
					G = 0 ;
					B = 0 ;
				}
				else
				{
					int tR, tG, tB ;
					int t ;

					if( iB > 128 )
					{
						iS = 512 - Max - Min; 
					}
					else
					{
						iS = Max + Min;
					}
					iS = ( Delta << 8 ) / iS;

					tB = ( ( Max - B ) << 8 ) / Delta ;
					tG = ( ( Max - G ) << 8 ) / Delta ;
					tR = ( ( Max - R ) << 8 ) / Delta ;
					if( R == Max )
					{
						t = tB - tG ;
						R = 255 ;
						if( tB > tG ) 
						{
							B = 0 ;
							G = t ;
						}
						else
						{
							B = -t ;
							G = 0 ;
						}
					}
					else
					if( G == Max )
					{
						t = tR - tB ;
						G = 255 ;
						if( tR > tB ) 
						{
							R = 0 ;
							B = t ;
						}
						else
						{
							R = -t ;
							B = 0 ;
						}
					}
					else
					{
						t = tG - tR ;
						B = 255 ;
						if( tG > tR ) 
						{
							G = 0 ;
							R = t ;
						}
						else
						{
							G = -t ;
							R = 0 ;
						}
					}
				}

				B += ( ( 128 - B ) * ( 256 - iS ) ) >> 8 ;
				G += ( ( 128 - G ) * ( 256 - iS ) ) >> 8 ;
				R += ( ( 128 - R ) * ( 256 - iS ) ) >> 8 ;

				iB = ( ( iB - iMinB ) * iScaleB ) >> 12 ;
				if( iB < 0 ){ iB = 0 ; }else{ if( iB > 255 ){ iB = 255 ; } }

				if( iB <= 128 )
				{
					Dest[ 0 ] = ( BYTE )( ( B * ( iB << 1 ) ) >> 8 ) ;
					Dest[ 1 ] = ( BYTE )( ( G * ( iB << 1 ) ) >> 8 ) ;
					Dest[ 2 ] = ( BYTE )( ( R * ( iB << 1 ) ) >> 8 ) ;
				}
				else
				{
					Dest[ 0 ] = ( BYTE )( B + ( ( ( 255 - B ) * ( ( iB - 128 ) << 1 ) ) >> 8 ) ) ;
					Dest[ 1 ] = ( BYTE )( G + ( ( ( 255 - G ) * ( ( iB - 128 ) << 1 ) ) >> 8 ) ) ;
					Dest[ 2 ] = ( BYTE )( R + ( ( ( 255 - R ) * ( ( iB - 128 ) << 1 ) ) >> 8 ) ) ;
				}

				if( IsPMA )
				{
					Dest[ 0 ] = BASEIM.RgbToPmaTable[ Dest[ 0 ] ][ Src[ 3 ] ] ;
					Dest[ 1 ] = BASEIM.RgbToPmaTable[ Dest[ 1 ] ][ Src[ 3 ] ] ;
					Dest[ 2 ] = BASEIM.RgbToPmaTable[ Dest[ 2 ] ][ Src[ 3 ] ] ;
				}
				Dest[ 3 ] = Src[ 3 ] ;

				Src  += 4 ;
				Dest += 4 ;
			}while( -- i ) ;

			Src  += SrcAddPitch ;
			Dest += DestAddPitch ;
		}while( -- Height ) ;

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int GraphFilter_HSB( GRAPHFILTER_INFO *Info, int HueType, float Hue, float Saturation, float Bright, int IsPMA )
{
	// 値を補正
	Hue = Hue / 360.0f ;
	if( HueType == 0 )
	{
		if( Hue < -0.5f )
		{
			Hue = -0.5f;
		}
		else
		if( Hue > 0.5f )
		{
			Hue = 0.5f;
		}
	}
	else
	{
		if( Hue < 0.0f )
		{
			Hue = 0.0f;
		}
		else
		if( Hue > 1.0f )
		{
			Hue = 1.0f;
		}
	}
	Hue *= 6.0f ;
	Saturation /= 100.0f;
	if( Saturation < -1.0f )
	{
		Saturation = -1.0f;
	}
	else
	if( Saturation > 1.0f )
	{
		Saturation = 1.0f;
	}
	Bright /= 100.0f;

	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_HSB_PF( Info, HueType, Hue, Saturation, Bright, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;
		int iHue ;
		int iSaturation ;
		int iBright ;
		int iH, iS, iB ;
		int Max ;
		int Min ;
		int Delta ;
		int R, G, B ;

		iHue        = _FTOL( Hue        * 256.0f ) ;
		iSaturation = _FTOL( ( Saturation > 0.0f ? Saturation * 5.0f : Saturation ) * 256.0f ) ;
		iBright     = _FTOL( Bright     * 256.0f ) ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		if( HueType == 1 )
		{
			if( iHue >= 0x600 )
			{
				iHue -= 0x600 ;
			}
			do
			{
				i = Width ;
				do
				{
					if( IsPMA )
					{
						B = BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] ;
						G = BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] ;
						R = BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] ;
					}
					else
					{
						B = Src[ 0 ] ;
						G = Src[ 1 ] ;
						R = Src[ 2 ] ;
					}

					if( iBright > 0 )
					{
						B += ( ( 255 - B ) * iBright ) >> 8 ;
						G += ( ( 255 - G ) * iBright ) >> 8 ;
						R += ( ( 255 - R ) * iBright ) >> 8 ;
					}
					else
					{
						B = ( B * ( 256 + iBright ) ) >> 8 ;
						G = ( G * ( 256 + iBright ) ) >> 8 ;
						R = ( R * ( 256 + iBright ) ) >> 8 ;
					}

					Max = R > G ? ( R > B ? R : B ) : ( G > B ? G : B ) ;
					Min = R < G ? ( R < B ? R : B ) : ( G < B ? G : B ) ;
					Delta = Max - Min;

					iB = ( Max + Min ) >> 1 ;
					if( Max == Min )
					{
						iS = 0 ;
					}
					else
					{
						if( iB > 128 )
						{
							iS = 512 - Max - Min; 
						}
						else
						{
							iS = Max + Min;
						}
						iS = ( Delta << 8 ) / iS;
					}

					iH = iHue ;

					iS = ( iS * ( 256 + iSaturation ) ) >> 8 ;
					if( iS > 256 ) iS = 256 ;

					int iHM ;
					iHM = iH & 0xff ;

					if( iH < 0x100 )
					{
						R = 255 ;
						G = iHM ;
						B = 0 ;
					}
					else
					if( iH < 0x200 )
					{
						R = 255 - iHM ;
						G = 255 ;
						B = 0 ;
					}
					else
					if( iH < 0x300 )
					{
						R = 0 ;
						G = 255 ;
						B = iHM ;
					}
					else
					if( iH < 0x400 )
					{
						R = 0 ;
						G = 255 - iHM ;
						B = 255 ;
					}
					else
					if( iH < 0x500 )
					{
						R = iHM ;
						G = 0 ;
						B = 255 ;
					}
					else
					{
						R = 255 ;
						G = 0 ;
						B = 255 - iHM ;
					}

					B += ( ( 128 - B ) * ( 256 - iS ) ) >> 8 ;
					G += ( ( 128 - G ) * ( 256 - iS ) ) >> 8 ;
					R += ( ( 128 - R ) * ( 256 - iS ) ) >> 8 ;

					if( iB <= 128 )
					{
						Dest[ 0 ] = ( BYTE )( ( B * ( iB << 1 ) ) >> 8 ) ;
						Dest[ 1 ] = ( BYTE )( ( G * ( iB << 1 ) ) >> 8 ) ;
						Dest[ 2 ] = ( BYTE )( ( R * ( iB << 1 ) ) >> 8 ) ;
					}
					else
					{
						Dest[ 0 ] = ( BYTE )( B + ( ( ( 255 - B ) * ( ( iB - 128 ) << 1 ) ) >> 8 ) ) ;
						Dest[ 1 ] = ( BYTE )( G + ( ( ( 255 - G ) * ( ( iB - 128 ) << 1 ) ) >> 8 ) ) ;
						Dest[ 2 ] = ( BYTE )( R + ( ( ( 255 - R ) * ( ( iB - 128 ) << 1 ) ) >> 8 ) ) ;
					}

					if( IsPMA )
					{
						Dest[ 0 ] = BASEIM.RgbToPmaTable[ Dest[ 0 ] ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ Dest[ 1 ] ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ Dest[ 2 ] ][ Src[ 3 ] ] ;
					}
					Dest[ 3 ] = Src[ 3 ] ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}
		else
		{
			do
			{
				i = Width ;
				do
				{
					if( IsPMA )
					{
						B = BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] ;
						G = BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] ;
						R = BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] ;
					}
					else
					{
						B = Src[ 0 ] ;
						G = Src[ 1 ] ;
						R = Src[ 2 ] ;
					}

					if( iBright > 0 )
					{
						B += ( ( 255 - B ) * iBright ) >> 8 ;
						G += ( ( 255 - G ) * iBright ) >> 8 ;
						R += ( ( 255 - R ) * iBright ) >> 8 ;
					}
					else
					{
						B = ( B * ( 256 + iBright ) ) >> 8 ;
						G = ( G * ( 256 + iBright ) ) >> 8 ;
						R = ( R * ( 256 + iBright ) ) >> 8 ;
					}

					Max = R > G ? ( R > B ? R : B ) : ( G > B ? G : B ) ;
					Min = R < G ? ( R < B ? R : B ) : ( G < B ? G : B ) ;
					Delta = Max - Min;

					iB = ( Max + Min ) >> 1 ;
					if( Max == Min )
					{
						iH = 0 ;
						iS = 0 ;
					}
					else
					{
						int tR, tG, tB ;

						if( iB > 128 )
						{
							iS = 512 - Max - Min; 
						}
						else
						{
							iS = Max + Min;
						}
						iS = ( Delta << 8 ) / iS;

						tB = ( ( Max - B ) << 8 ) / Delta ;
						tG = ( ( Max - G ) << 8 ) / Delta ;
						tR = ( ( Max - R ) << 8 ) / Delta ;
						if( R == Max )
						{
							iH = tB - tG ;
						}
						else
						if( G == Max )
						{
							iH = 0x200 + tR - tB ;
						}
						else
						{
							iH = 0x400 + tG - tR ;
						}
						if( iH < 0 )
						{
							iH += 0x600 ;
						}
					}

					iH += iHue ;
					if( iH < 0 )
					{
						iH += 0x600 ;
					}
					else
					if( iH >= 0x600 )
					{
						iH -= 0x600 ;
					}

					iS = ( iS * ( 256 + iSaturation ) ) >> 8 ;
					if( iS > 256 ) iS = 256 ;

					int iHM ;
					iHM = iH & 0xff ;

					if( iH < 0x100 )
					{
						R = 255 ;
						G = iHM ;
						B = 0 ;
					}
					else
					if( iH < 0x200 )
					{
						R = 255 - iHM ;
						G = 255 ;
						B = 0 ;
					}
					else
					if( iH < 0x300 )
					{
						R = 0 ;
						G = 255 ;
						B = iHM ;
					}
					else
					if( iH < 0x400 )
					{
						R = 0 ;
						G = 255 - iHM ;
						B = 255 ;
					}
					else
					if( iH < 0x500 )
					{
						R = iHM ;
						G = 0 ;
						B = 255 ;
					}
					else
					{
						R = 255 ;
						G = 0 ;
						B = 255 - iHM ;
					}

					B += ( ( 128 - B ) * ( 256 - iS ) ) >> 8 ;
					G += ( ( 128 - G ) * ( 256 - iS ) ) >> 8 ;
					R += ( ( 128 - R ) * ( 256 - iS ) ) >> 8 ;

					if( iB <= 128 )
					{
						Dest[ 0 ] = ( BYTE )( ( B * ( iB << 1 ) ) >> 8 ) ;
						Dest[ 1 ] = ( BYTE )( ( G * ( iB << 1 ) ) >> 8 ) ;
						Dest[ 2 ] = ( BYTE )( ( R * ( iB << 1 ) ) >> 8 ) ;
					}
					else
					{
						Dest[ 0 ] = ( BYTE )( B + ( ( ( 255 - B ) * ( ( iB - 128 ) << 1 ) ) >> 8 ) ) ;
						Dest[ 1 ] = ( BYTE )( G + ( ( ( 255 - G ) * ( ( iB - 128 ) << 1 ) ) >> 8 ) ) ;
						Dest[ 2 ] = ( BYTE )( R + ( ( ( 255 - R ) * ( ( iB - 128 ) << 1 ) ) >> 8 ) ) ;
					}

					if( IsPMA )
					{
						Dest[ 0 ] = BASEIM.RgbToPmaTable[ Dest[ 0 ] ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ Dest[ 1 ] ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ Dest[ 2 ] ][ Src[ 3 ] ] ;
					}
					Dest[ 3 ] = Src[ 3 ] ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int GraphFilter_Invert( GRAPHFILTER_INFO *Info, int IsPMA )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_Invert_PF( Info, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		if( IsPMA )
		{
			do
			{
				i = Width ;
				do
				{
					Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( 255 - BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] ) ][ Src[ 3 ] ] ;
					Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( 255 - BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] ) ][ Src[ 3 ] ] ;
					Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( 255 - BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] ) ][ Src[ 3 ] ] ;
					Dest[ 3 ] = Src[ 3 ] ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}
		else
		{
			do
			{
				i = Width ;
				do
				{
					Dest[ 0 ] = ( BYTE )( 255 - Src[ 0 ] ) ;
					Dest[ 1 ] = ( BYTE )( 255 - Src[ 1 ] ) ;
					Dest[ 2 ] = ( BYTE )( 255 - Src[ 2 ] ) ;
					Dest[ 3 ] = Src[ 3 ] ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int GraphFilter_Level( GRAPHFILTER_INFO *Info, float Min, float Max, float Gamma, float AfterMin, float AfterMax, int IsPMA )
{
	// 値を補正
	Min /= 255.0f ;
	Max /= 255.0f ;
	AfterMin /= 255.0f ;
	AfterMax /= 255.0f ;
	if( Min < 0.0f )
	{
		Min = 0.0f ;
	}
	else
	if( Min > 1.0f )
	{
		Min = 1.0f ;
	}
	if( Max < 0.0f )
	{
		Max = 0.0f ;
	}
	else
	if( Max > 1.0f )
	{
		Max = 1.0f ;
	}
	if( Gamma < 0.001f )
	{
		Gamma = 0.001f ;
	}
	if( AfterMin < 0.0f )
	{
		AfterMin = 0.0f ;
	}
	else
	if( AfterMin > 1.0f )
	{
		AfterMin = 1.0f ;
	}
	if( AfterMax < 0.0f )
	{
		AfterMax = 0.0f ;
	}
	else
	if( AfterMax > 1.0f )
	{
		AfterMax = 1.0f ;
	}
	if( Min > Max ) Min = Max ;
	if( AfterMin > AfterMax ) AfterMin = AfterMax ;

	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		int  Always ;

		// ガンマ補正テーブルのセットアップ
		Always = FALSE ;
		if( GraphFilterShaderHandle.GammaTex < 0 )
		{
			SETUP_GRAPHHANDLE_GPARAM GParam ;

			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam, 32, FALSE, FALSE ) ;

			GraphFilterShaderHandle.GammaTex = Graphics_Image_MakeGraph_UseGParam( &GParam, 256, 1, FALSE, FALSE, 0, FALSE ) ;
			NS_SetDeleteHandleFlag( GraphFilterShaderHandle.GammaTex, &GraphFilterShaderHandle.GammaTex ) ;
			GraphFilterShaderHandle.PrevGamma = -10000000.0f ;
			Always = TRUE ;
		}
		if( Always == TRUE || _FABS( GraphFilterShaderHandle.PrevGamma - Gamma ) > 0.0001f )
		{
			BASEIMAGE GammaBaseImage ;
			NS_CreateXRGB8ColorBaseImage( 256, 1, &GammaBaseImage ) ;
			if( GammaBaseImage.GraphData != NULL )
			{
				int i ;
				float Param ;
				float GammaRev ;
				BYTE *Dest ;

				Dest = ( BYTE * )GammaBaseImage.GraphData ;
				GammaRev = 1.0f / Gamma ;
				for( i = 0 ; i < 256 ; i ++, Dest += 4 )
				{
					Param = _POW( ( float )i / 255.0f, GammaRev ) * 255.0f ;
					if( Param > 255.0f )
					{
						*Dest = 255 ;
					}
					else
					{
						*Dest = ( BYTE )_FTOL( Param ) ;
					}
					Dest[ 1 ] = Dest[ 0 ] ;
					Dest[ 2 ] = Dest[ 0 ] ;
					Dest[ 3 ] = Dest[ 0 ] ;
				}
				NS_BltBmpOrGraphImageToGraph( NULL, NULL, NULL, FALSE, &GammaBaseImage, NULL, 0, 0, GraphFilterShaderHandle.GammaTex ) ;

				GraphFilterShaderHandle.PrevGamma = Gamma ;

				NS_ReleaseBaseImage( &GammaBaseImage ) ;
			}
		}

		GraphFilter_Level_PF( Info, Min, Max, Gamma, AfterMin, AfterMax, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;
		int iMin ;
		int iMax ;
		int iMinMaxLen ;
		int iAfterMin ;
		int iAfterMax ;
		int iAfterMinMaxLen ;
		int iAfterMin256 ;
		int R, G, B ;
		static float PrevGamma = -10000000.0f ;
		static int GammaTable[ 256 ] ;

		if( _FABS( PrevGamma - Gamma ) > 0.0001f )
		{
			int j ;
			float Param ;
			float GammaRev ;

			GammaRev = 1.0f / Gamma ;
			for( j = 0 ; j < 256 ; j ++ )
			{
				Param = _POW( ( float )j / 255.0f, GammaRev ) * 255.0f ;
				if( Param > 255.0f )
				{
					GammaTable[ j ] = 255 ;
				}
				else
				{
					GammaTable[ j ] = _FTOL( Param ) ;
				}
			}

			PrevGamma = Gamma ;
		}

		iMin = _FTOL( Min * 255 ) ;
		iMax = _FTOL( Max * 255 ) ;
		iMinMaxLen = iMax - iMin ;
		iAfterMin = _FTOL( AfterMin * 255 ) ;
		iAfterMax = _FTOL( AfterMax * 255 ) ;
		iAfterMinMaxLen = iAfterMax - iAfterMin ;
		iAfterMin256 = iAfterMin << 8 ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		if( IsPMA )
		{
			do
			{
				i = Width ;
				do
				{
					B = ( ( BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] - iMin ) << 8 ) / iMinMaxLen;
					B = B < 0 ? GammaTable[ 0 ] : ( B > 255 ?  GammaTable[ 255 ] : GammaTable[ B ] ) ;

					G = ( ( BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] - iMin ) << 8 ) / iMinMaxLen;
					G = G < 0 ? GammaTable[ 0 ] : ( G > 255 ?  GammaTable[ 255 ] : GammaTable[ G ] ) ;

					R = ( ( BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] - iMin ) << 8 ) / iMinMaxLen;
					R = R < 0 ? GammaTable[ 0 ] : ( R > 255 ?  GammaTable[ 255 ] : GammaTable[ R ] ) ;

					Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( iAfterMin256 + iAfterMinMaxLen * B ) >> 8 ) ][ Src[ 3 ] ] ;
					Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( iAfterMin256 + iAfterMinMaxLen * G ) >> 8 ) ][ Src[ 3 ] ] ;
					Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( iAfterMin256 + iAfterMinMaxLen * R ) >> 8 ) ][ Src[ 3 ] ] ;
					Dest[ 3 ] = Src[ 3 ] ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}
		else
		{
			do
			{
				i = Width ;
				do
				{
					B = ( ( Src[ 0 ] - iMin ) << 8 ) / iMinMaxLen;
					B = B < 0 ? GammaTable[ 0 ] : ( B > 255 ?  GammaTable[ 255 ] : GammaTable[ B ] ) ;

					G = ( ( Src[ 1 ] - iMin ) << 8 ) / iMinMaxLen;
					G = G < 0 ? GammaTable[ 0 ] : ( G > 255 ?  GammaTable[ 255 ] : GammaTable[ G ] ) ;

					R = ( ( Src[ 2 ] - iMin ) << 8 ) / iMinMaxLen;
					R = R < 0 ? GammaTable[ 0 ] : ( R > 255 ?  GammaTable[ 255 ] : GammaTable[ R ] ) ;

					Dest[ 0 ] = ( BYTE )( ( iAfterMin256 + iAfterMinMaxLen * B ) >> 8 ) ;
					Dest[ 1 ] = ( BYTE )( ( iAfterMin256 + iAfterMinMaxLen * G ) >> 8 ) ;
					Dest[ 2 ] = ( BYTE )( ( iAfterMin256 + iAfterMinMaxLen * R ) >> 8 ) ;
					Dest[ 3 ] = Src[ 3 ] ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int GraphFilter_TwoColor( GRAPHFILTER_INFO *Info, float Threshold, COLOR_F *LowColor, COLOR_F *HighColor, int IsPMA )
{
	// 値を補正
	if( LowColor == NULL || HighColor == NULL )
		return -1 ;

	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_TwoColor_PF( Info, Threshold, LowColor, HighColor, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;
		int Temp ;
		int iThreshold ;
		BYTE iLowColor[ 4 ] ;
		BYTE iHighColor[ 4 ] ;

		iThreshold = _FTOL( Threshold * 4096.0f ) ;

		Temp = _FTOL( LowColor->b * 255.0f ) ;
		if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
		iLowColor[ 0 ] = ( BYTE )Temp ;

		Temp = _FTOL( LowColor->g * 255.0f ) ;
		if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
		iLowColor[ 1 ] = ( BYTE )Temp ;

		Temp = _FTOL( LowColor->r * 255.0f ) ;
		if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
		iLowColor[ 2 ] = ( BYTE )Temp ;

		Temp = _FTOL( LowColor->a * 255.0f ) ;
		if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
		iLowColor[ 3 ] = ( BYTE )Temp ;


		Temp = _FTOL( HighColor->b * 255.0f ) ;
		if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
		iHighColor[ 0 ] = ( BYTE )Temp ;

		Temp = _FTOL( HighColor->g * 255.0f ) ;
		if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
		iHighColor[ 1 ] = ( BYTE )Temp ;

		Temp = _FTOL( HighColor->r * 255.0f ) ;
		if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
		iHighColor[ 2 ] = ( BYTE )Temp ;

		Temp = _FTOL( HighColor->a * 255.0f ) ;
		if( Temp < 0 ) Temp = 0 ; else if( Temp > 255 ) Temp = 255 ;
		iHighColor[ 3 ] = ( BYTE )Temp ;


		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		if( IsPMA )
		{
			iLowColor[ 0 ] = BASEIM.RgbToPmaTable[ iLowColor[ 0 ] ][ iLowColor[ 3 ] ] ;
			iLowColor[ 1 ] = BASEIM.RgbToPmaTable[ iLowColor[ 1 ] ][ iLowColor[ 3 ] ] ;
			iLowColor[ 2 ] = BASEIM.RgbToPmaTable[ iLowColor[ 2 ] ][ iLowColor[ 3 ] ] ;
			iHighColor[ 0 ] = BASEIM.RgbToPmaTable[ iHighColor[ 0 ] ][ iHighColor[ 3 ] ] ;
			iHighColor[ 1 ] = BASEIM.RgbToPmaTable[ iHighColor[ 1 ] ][ iHighColor[ 3 ] ] ;
			iHighColor[ 2 ] = BASEIM.RgbToPmaTable[ iHighColor[ 2 ] ][ iHighColor[ 3 ] ] ;

			do
			{
				i = Width ;
				do
				{
					if( BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] * ( int )( 0.114f * 4096.0f ) +
						BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] * ( int )( 0.587f * 4096.0f ) +
						BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] * ( int )( 0.299f * 4096.0f ) < iThreshold )
					{
						*( ( DWORD * )Dest ) = *( ( DWORD * )iLowColor ) ;
					}
					else
					{
						*( ( DWORD * )Dest ) = *( ( DWORD * )iHighColor ) ;
					}

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}
		else
		{
			do
			{
				i = Width ;
				do
				{
					if( Src[ 0 ] * ( int )( 0.114f * 4096.0f ) +
						Src[ 1 ] * ( int )( 0.587f * 4096.0f ) +
						Src[ 2 ] * ( int )( 0.299f * 4096.0f ) < iThreshold )
					{
						*( ( DWORD * )Dest ) = *( ( DWORD * )iLowColor ) ;
					}
					else
					{
						*( ( DWORD * )Dest ) = *( ( DWORD * )iHighColor ) ;
					}

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int	GraphFilter_GradientMap( GRAPHFILTER_INFO *Info, int MapGrHandle, int Reverse, int IsPMA )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_GradientMap_PF( Info, MapGrHandle, Reverse, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;
		BASEIMAGE GradImage ;
		DWORD GradMap[ 256 ] ;

		// グラデーションマップの配列をセットアップする
		{
			int GradWidth, GradHeight ;

			if( NS_GetGraphSize( MapGrHandle, &GradWidth, &GradHeight ) < 0 )
				return -1 ;

			if( GraphFilter_GetSoftImage( MapGrHandle, &GradImage, NULL ) < 0 )
				return -1 ;

			if( Reverse )
			{
				for( i = 0 ; i < 256 ; i ++ )
					GradMap[ 255 - i ] = *( ( DWORD * )( ( BYTE * )GradImage.GraphData + GradWidth * i / 256 * 4 ) ) ;
			}
			else
			{
				for( i = 0 ; i < 256 ; i ++ )
					GradMap[ i ] = *( ( DWORD * )( ( BYTE * )GradImage.GraphData + GradWidth * i / 256 * 4 ) ) ;
			}

			if( IsPMA )
			{
				for( i = 0 ; i < 256 ; i ++ )
				{
					( ( BYTE * )&GradMap[ i ] )[ 0 ] = BASEIM.RgbToPmaTable[ ( ( BYTE * )&GradMap[ i ] )[ 0 ] ][ ( ( BYTE * )&GradMap[ i ] )[ 3 ] ] ;
					( ( BYTE * )&GradMap[ i ] )[ 1 ] = BASEIM.RgbToPmaTable[ ( ( BYTE * )&GradMap[ i ] )[ 1 ] ][ ( ( BYTE * )&GradMap[ i ] )[ 3 ] ] ;
					( ( BYTE * )&GradMap[ i ] )[ 2 ] = BASEIM.RgbToPmaTable[ ( ( BYTE * )&GradMap[ i ] )[ 2 ] ][ ( ( BYTE * )&GradMap[ i ] )[ 3 ] ] ;
				}
			}

			NS_ReleaseBaseImage( &GradImage ) ;
		}

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		if( IsPMA )
		{
			do
			{
				i = Width ;
				do
				{
					*( ( DWORD * )Dest ) = GradMap[ ( BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] * ( int )( 0.114f * 4096.0f ) +
													  BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] * ( int )( 0.587f * 4096.0f ) +
													  BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] * ( int )( 0.299f * 4096.0f ) ) >> 12 ] ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}
		else
		{
			do
			{
				i = Width ;
				do
				{
					*( ( DWORD * )Dest ) = GradMap[ ( Src[ 0 ] * ( int )( 0.114f * 4096.0f ) +
													  Src[ 1 ] * ( int )( 0.587f * 4096.0f ) +
													  Src[ 2 ] * ( int )( 0.299f * 4096.0f ) ) >> 12 ] ;

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int GraphFilter_Replacement( GRAPHFILTER_INFO *Info, COLOR_U8 TargetColor, COLOR_U8 NextColor, int IsPMA )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_Replacement_PF( Info, TargetColor, NextColor, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		if( IsPMA )
		{
			NextColor.r = BASEIM.RgbToPmaTable[ NextColor.r ][ NextColor.a ] ;
			NextColor.g = BASEIM.RgbToPmaTable[ NextColor.g ][ NextColor.a ] ;
			NextColor.b = BASEIM.RgbToPmaTable[ NextColor.b ][ NextColor.a ] ;

			do
			{
				i = Width ;
				do
				{
					if( BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] == TargetColor.b &&
						BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] == TargetColor.g &&
						BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] == TargetColor.r &&
						Src[ 3 ] == TargetColor.a )
					{
						Dest[ 0 ] = NextColor.b ;
						Dest[ 1 ] = NextColor.g ;
						Dest[ 2 ] = NextColor.r ;
						Dest[ 3 ] = NextColor.a ;
					}
					else
					{
						Dest[ 0 ] = Src[ 0 ] ;
						Dest[ 1 ] = Src[ 1 ] ;
						Dest[ 2 ] = Src[ 2 ] ;
						Dest[ 3 ] = Src[ 3 ] ;
					}

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}
		else
		{
			do
			{
				i = Width ;
				do
				{
					if( Src[ 0 ] == TargetColor.b &&
						Src[ 1 ] == TargetColor.g &&
						Src[ 2 ] == TargetColor.r &&
						Src[ 3 ] == TargetColor.a )
					{
						Dest[ 0 ] = NextColor.b ;
						Dest[ 1 ] = NextColor.g ;
						Dest[ 2 ] = NextColor.r ;
						Dest[ 3 ] = NextColor.a ;
					}
					else
					{
						Dest[ 0 ] = Src[ 0 ] ;
						Dest[ 1 ] = Src[ 1 ] ;
						Dest[ 2 ] = Src[ 2 ] ;
						Dest[ 3 ] = Src[ 3 ] ;
					}

					Src  += 4 ;
					Dest += 4 ;
				}while( -- i ) ;

				Src  += SrcAddPitch ;
				Dest += DestAddPitch ;
			}while( -- Height ) ;
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int GraphFilter_BilateralBlur( GRAPHFILTER_INFO *Info, int IsPMA )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_BilateralBlur_PF( Info, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合
	}

	// 終了
	return 0 ;
}

extern int	GraphFilter_PremulAlpha( GRAPHFILTER_INFO *Info )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_PremulAlpha_PF( Info ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		do
		{
			i = Width ;
			do
			{
				Dest[ 0 ] = ( BYTE )( ( Src[ 0 ] * Src[ 3 ] ) >> 8 ) ;
				Dest[ 1 ] = ( BYTE )( ( Src[ 1 ] * Src[ 3 ] ) >> 8 ) ;
				Dest[ 2 ] = ( BYTE )( ( Src[ 2 ] * Src[ 3 ] ) >> 8 ) ;
				Dest[ 3 ] = Src[ 3 ] ;

				Src  += 4 ;
				Dest += 4 ;
			}while( -- i ) ;

			Src  += SrcAddPitch ;
			Dest += DestAddPitch ;
		}while( -- Height ) ;

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int	GraphFilter_InterpAlpha( GRAPHFILTER_INFO *Info )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_InterpAlpha_PF( Info ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Dest ;
		DWORD tmp ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
		DWORD DestAddPitch ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch  = Info->SrcBaseImage.Pitch  - Width * 4 ;
		DestAddPitch = Info->DestBaseImage.Pitch - Width * 4 ;

		do
		{
			i = Width ;
			do
			{
				if( Src[ 3 ] == 0 )
				{
					Dest[ 0 ] = 0 ;
					Dest[ 1 ] = 0 ;
					Dest[ 2 ] = 0 ;
				}
				else
				{
					tmp = ( DWORD )( Src[ 0 ] * 255 / Src[ 3 ] ) ; Dest[ 0 ] = ( BYTE )( tmp > 255 ? 255 : tmp ) ;
					tmp = ( DWORD )( Src[ 1 ] * 255 / Src[ 3 ] ) ; Dest[ 1 ] = ( BYTE )( tmp > 255 ? 255 : tmp ) ;
					tmp = ( DWORD )( Src[ 2 ] * 255 / Src[ 3 ] ) ; Dest[ 2 ] = ( BYTE )( tmp > 255 ? 255 : tmp ) ;
				}
				Dest[ 3 ] = Src[ 3 ] ;

				Src  += 4 ;
				Dest += 4 ;
			}while( -- i ) ;

			Src  += SrcAddPitch ;
			Dest += DestAddPitch ;
		}while( -- Height ) ;

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int	GraphFilter_YUVtoRGB( GRAPHFILTER_INFO *Info, int UVGrHandle )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_YUVtoRGB_PF( Info, UVGrHandle ) ;
	}
	else
	{
		// シェーダーが使えない場合
		BYTE *SrcY ;
		BYTE *SrcUV ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcYPitch ;
		DWORD SrcY2Pitch ;
		DWORD SrcUVPitch ;
		DWORD DestPitch ;
		DWORD Dest2Pitch ;
		BASEIMAGE UVImage ;
		int YGraphWidth ;
		int YGraphHeight ;
		int U, V, Y ;
		int UB, VUG, VR, C ;
		int AV, AY ;
		int AVR ;
		DWORD AddY ;
		DWORD AddUV ;
		int RRAFlag = ( Info->FilterOrBlendType == DX_GRAPH_FILTER_YUV_TO_RGB_RRA || Info->FilterOrBlendType == DX_GRAPH_FILTER_Y2UV1_TO_RGB_RRA ) ? TRUE : FALSE ;

		if( RRAFlag )
		{
			RECT SrcRect ;
			int SrcGrWidth ;
			int SrcGrHeight ;
			int UVGrWidth ;
			int UVGrHeight ;

			NS_GetGraphSize( Info->SrcGrHandle, &SrcGrWidth, &SrcGrHeight ) ;
			NS_GetGraphSize( UVGrHandle,        &UVGrWidth,  &UVGrHeight  ) ;

			if( Info->SrcX1 != 0              || Info->SrcY1 != 0 ||
				Info->SrcX2 != SrcGrWidth / 2 || Info->SrcY2 != SrcGrHeight )
				return -1 ;

			SrcRect.left   = 0 ;
			SrcRect.top    = 0 ;
			SrcRect.right  = SrcGrWidth ;
			SrcRect.bottom = SrcGrHeight ;
			if( GraphFilter_SoftImageSetup( Info, 1, -1, -1, &SrcRect ) < 0 )
				return -1 ;

			SrcRect.left   = 0 ;
			SrcRect.top    = 0 ;
			SrcRect.right  = UVGrWidth ;
			SrcRect.bottom = UVGrHeight ;
			if( GraphFilter_GetSoftImage( UVGrHandle, &UVImage, NULL ) < 0 )
				return -1 ;
		}
		else
		{
			if( GraphFilter_SoftImageSetup( Info ) < 0 )
				return -1 ;

			if( GraphFilter_GetSoftImage( UVGrHandle, &UVImage, NULL ) < 0 )
				return -1 ;
		}

		SrcY  = ( BYTE * )Info->SrcBaseImage.GraphData ;
		SrcUV = ( BYTE * )UVImage.GraphData ;
		Dest  = ( BYTE * )Info->DestBaseImage.GraphData ;

		NS_GetGraphSize( Info->SrcGrHandle, &YGraphWidth, &YGraphHeight ) ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcYPitch  = Info->SrcBaseImage.Pitch ;
		SrcY2Pitch = SrcYPitch * 2 ;
		SrcUVPitch = UVImage.Pitch ;
		DestPitch  = Info->DestBaseImage.Pitch ;
		Dest2Pitch = DestPitch * 2 ;

		AddY  = Info->SrcBaseImage.Width / 2 * 4 ;
		AddUV = UVImage.Width            / 2 * 4 ;

		if( YGraphWidth  == UVImage.Width &&
			YGraphHeight == UVImage.Height )
		{
			if( RRAFlag )
			{
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402


						AV  = ( int )SrcUVTemp[ 1 + AddUV ] - 128 ;
						AVR = AV *  45940 ;			//  1.402


						Y  = SrcYTemp[ 0 ] << 15 ;
						AY = SrcYTemp[ AddY ] << 15 ;
						C =  Y +  UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y +  VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y +  VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR  ; DestTemp[ 3 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						SrcYTemp  += 4 ;
						SrcUVTemp += 4 ;
						DestTemp += 4 ;
					}while( -- i ) ;

					SrcY  += SrcYPitch ;
					SrcUV += SrcUVPitch ;
					Dest  += DestPitch ;
				}while( -- Height ) ;
			}
			else
			{
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402

						Y = SrcYTemp[ 0 ] << 15 ;
						C = Y + UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 3 ] = 255 ;

						SrcYTemp  += 4 ;
						SrcUVTemp += 4 ;
						DestTemp += 4 ;
					}while( -- i ) ;

					SrcY  += SrcYPitch ;
					SrcUV += SrcUVPitch ;
					Dest  += DestPitch ;
				}while( -- Height ) ;
			}
		}
		else
		if( YGraphWidth  / 2 == UVImage.Width &&
			YGraphHeight / 2 == UVImage.Height )
		{
			if( RRAFlag )
			{
				Height /= 2 ;
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width / 2 ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402


						AV = ( int )SrcUVTemp[ 1 + AddUV ] - 128 ;

						AVR  = AV *  45940 ;				//  1.402


						Y  = SrcYTemp[ 0    ] << 15 ;
						AY = SrcYTemp[ AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ 3 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						Y  = SrcYTemp[ 4        ] << 15 ;
						AY = SrcYTemp[ 4 + AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[ 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[ 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ 7 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						Y  = SrcYTemp[ SrcYPitch + 0    ] << 15 ;
						AY = SrcYTemp[ SrcYPitch + AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[ DestPitch + 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[ DestPitch + 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ DestPitch + 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ DestPitch + 3 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						Y  = SrcYTemp[ SrcYPitch + 4        ] << 15 ;
						AY = SrcYTemp[ SrcYPitch + 4 + AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[ DestPitch + 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[ DestPitch + 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ DestPitch + 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ DestPitch + 7 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						SrcYTemp  += 8 ;
						SrcUVTemp += 4 ;
						DestTemp += 8 ;
					}while( -- i ) ;

					SrcY  += SrcY2Pitch ;
					SrcUV += SrcUVPitch ;
					Dest  += Dest2Pitch ;
				}while( -- Height ) ;
			}
			else
			{
				Height /= 2 ;
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width / 2 ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402

						Y = SrcYTemp[ 0 ] << 15 ;
						C = Y + UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 3 ] = 255 ;

						Y = SrcYTemp[ 4 ] << 15 ;
						C = Y + UB  ; DestTemp[ 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 7 ] = 255 ;

						Y = SrcYTemp[ SrcYPitch + 0 ] << 15 ;
						C = Y + UB  ; DestTemp[ DestPitch + 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ DestPitch + 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ DestPitch + 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ DestPitch + 3 ] = 255 ;

						Y = SrcYTemp[ SrcYPitch + 4 ] << 15 ;
						C = Y + UB  ; DestTemp[ DestPitch + 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ DestPitch + 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ DestPitch + 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ DestPitch + 7 ] = 255 ;

						SrcYTemp  += 8 ;
						SrcUVTemp += 4 ;
						DestTemp += 8 ;
					}while( -- i ) ;

					SrcY  += SrcY2Pitch ;
					SrcUV += SrcUVPitch ;
					Dest  += Dest2Pitch ;
				}while( -- Height ) ;
			}
		}
		else
		if( YGraphWidth  / 2 == UVImage.Width &&
			YGraphHeight     == UVImage.Height )
		{
			if( RRAFlag )
			{
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width / 2 ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402


						AV = ( int )SrcUVTemp[ 1 + AddUV ] - 128 ;

						AVR  = AV *  45940 ;				//  1.402


						Y  = SrcYTemp[ 0    ] << 15 ;
						AY = SrcYTemp[ AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ 3 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						Y  = SrcYTemp[ 4        ] << 15 ;
						AY = SrcYTemp[ 4 + AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[ 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[ 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ 7 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						SrcYTemp  += 8 ;
						SrcUVTemp += 4 ;
						DestTemp += 8 ;
					}while( -- i ) ;

					SrcY  += SrcYPitch ;
					SrcUV += SrcUVPitch ;
					Dest  += DestPitch ;
				}while( -- Height ) ;
			}
			else
			{
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width / 2 ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402

						Y = SrcYTemp[ 0 ] << 15 ;
						C = Y + UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 3 ] = 255 ;

						Y = SrcYTemp[ 4 ] << 15 ;
						C = Y + UB  ; DestTemp[ 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 7 ] = 255 ;

						SrcYTemp  += 8 ;
						SrcUVTemp += 4 ;
						DestTemp += 8 ;
					}while( -- i ) ;

					SrcY  += SrcYPitch ;
					SrcUV += SrcUVPitch ;
					Dest  += DestPitch ;
				}while( -- Height ) ;
			}
		}
		else
		if( YGraphWidth      == UVImage.Width &&
			YGraphHeight / 2 == UVImage.Height )
		{
			if( RRAFlag )
			{
				Height /= 2 ;
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402


						AV = ( int )SrcUVTemp[ 1 + AddUV ] - 128 ;

						AVR  = AV *  45940 ;				//  1.402


						Y  = SrcYTemp[ 0    ] << 15 ;
						AY = SrcYTemp[ AddY ] << 15 ;
						C =  Y +  UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y +  VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y +  VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR  ; DestTemp[ 3 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						Y  = SrcYTemp[ SrcYPitch + 0    ] << 15 ;
						AY = SrcYTemp[ SrcYPitch + AddY ] << 15 ;
						C =  Y +  UB  ; DestTemp[ DestPitch + 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y +  VUG ; DestTemp[ DestPitch + 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y +  VR  ; DestTemp[ DestPitch + 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR  ; DestTemp[ DestPitch + 3 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						SrcYTemp  += 4 ;
						SrcUVTemp += 4 ;
						DestTemp += 4 ;
					}while( -- i ) ;

					SrcY  += SrcY2Pitch ;
					SrcUV += SrcUVPitch ;
					Dest  += Dest2Pitch ;
				}while( -- Height ) ;
			}
			else
			{
				Height /= 2 ;
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402

						Y = SrcYTemp[ 0 ] << 15 ;
						C = Y + UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 3 ] = 255 ;

						Y = SrcYTemp[ SrcYPitch + 0 ] << 15 ;
						C = Y + UB  ; DestTemp[ DestPitch + 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ DestPitch + 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ DestPitch + 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ DestPitch + 3 ] = 255 ;

						SrcYTemp  += 4 ;
						SrcUVTemp += 4 ;
						DestTemp += 4 ;
					}while( -- i ) ;

					SrcY  += SrcY2Pitch ;
					SrcUV += SrcUVPitch ;
					Dest  += Dest2Pitch ;
				}while( -- Height ) ;
			}
		}
		else
		if( YGraphWidth  / 4 == UVImage.Width &&
			YGraphHeight     == UVImage.Height )
		{
			if( RRAFlag )
			{
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width / 4 ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402


						AV = ( int )SrcUVTemp[ 1 + AddUV ] - 128 ;

						AVR  = AV *  45940 ;				//  1.402


						Y  = SrcYTemp[ 0    ] << 15 ;
						AY = SrcYTemp[ AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ 3 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						Y  = SrcYTemp[ 4        ] << 15 ;
						AY = SrcYTemp[ 4 + AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[ 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[ 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ 7 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						Y  = SrcYTemp[ 8        ] << 15 ;
						AY = SrcYTemp[ 8 + AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[  8 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[  9 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ 10 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ 11 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						Y  = SrcYTemp[ 12        ] << 15 ;
						AY = SrcYTemp[ 12 + AddY ] << 15 ;
						C =  Y + UB  ; DestTemp[ 12 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C =  Y + VUG ; DestTemp[ 13 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C =  Y + VR  ; DestTemp[ 14 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						C = AY + AVR ; DestTemp[ 15 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // A

						SrcYTemp  += 16 ;
						SrcUVTemp += 4 ;
						DestTemp += 16 ;
					}while( -- i ) ;

					SrcY  += SrcYPitch ;
					SrcUV += SrcUVPitch ;
					Dest  += DestPitch ;
				}while( -- Height ) ;
			}
			else
			{
				do
				{
					BYTE *SrcYTemp ;
					BYTE *SrcUVTemp ;
					BYTE *DestTemp ;

					SrcYTemp  = SrcY ;
					SrcUVTemp = SrcUV ;
					DestTemp  = Dest ;

					i = Width / 4 ;
					do
					{
						U = ( int )SrcUVTemp[ 0 ] - 128 ;
						V = ( int )SrcUVTemp[ 1 ] - 128 ;

						UB  = U *  58064 ;				//  1.772
						VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
						VR  = V *  45940 ;				//  1.402

						Y = SrcYTemp[ 0 ] << 15 ;
						C = Y + UB  ; DestTemp[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 3 ] = 255 ;

						Y = SrcYTemp[ 4 ] << 15 ;
						C = Y + UB  ; DestTemp[ 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 7 ] = 255 ;

						Y = SrcYTemp[ 8 ] << 15 ;
						C = Y + UB  ; DestTemp[  8 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[  9 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 10 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 11 ] = 255 ;

						Y = SrcYTemp[ 12 ] << 15 ;
						C = Y + UB  ; DestTemp[ 12 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
						C = Y + VUG ; DestTemp[ 13 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
						C = Y + VR  ; DestTemp[ 14 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
						DestTemp[ 15 ] = 255 ;

						SrcYTemp  += 16 ;
						SrcUVTemp += 4 ;
						DestTemp += 16 ;
					}while( -- i ) ;

					SrcY  += SrcYPitch ;
					SrcUV += SrcUVPitch ;
					Dest  += DestPitch ;
				}while( -- Height ) ;
			}
		}

		NS_ReleaseBaseImage( &UVImage ) ;

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int	GraphFilter_BicubicScale( GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_BicubicScale_PF( Info, DestSizeX, DestSizeY ) ;
	}
	else
	{
		// シェーダーが使えない場合( 未実装 )
	}

	// 終了
	return 0 ;
}

extern int	GraphFilter_Lanczos3Scale( GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_Lanczos3Scale_PF( Info, DestSizeX, DestSizeY ) ;
	}
	else
	{
		// シェーダーが使えない場合( 未実装 )
	}

	// 終了
	return 0 ;
}

extern int	GraphFilter_SSAO( GRAPHFILTER_INFO *Info, int DepthMapGrHandle, float KernelRadius, float MinDistance, float MaxDistance, float Strength, int OcclusionColor, float OcclusionPower, int ColorMapGrHandle )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_SSAO_PF( Info, DepthMapGrHandle, KernelRadius, MinDistance, MaxDistance, Strength, OcclusionColor, OcclusionPower, ColorMapGrHandle ) ;
	}
	else
	{
		// シェーダーが使えない場合( 未実装 )
	}

	// 終了
	return 0 ;
}

extern int	GraphFilter_FloatColorScale( GRAPHFILTER_INFO *Info, COLOR_F ColorScale, COLOR_F ColorPreSub )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphFilter_FloatColorScale_PF( Info, ColorScale, ColorPreSub ) ;
	}
	else
	{
		// シェーダーが使えない場合( 未実装 )
	}

	// 終了
	return 0 ;
}

extern int	GraphBlend_Basic( GRAPHFILTER_INFO *Info, int IsPMA )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphBlend_Basic_PF( Info, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Blend ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
//		DWORD BlendAddPitch ;
		DWORD DestAddPitch ;
		int R, G, B, A ;
		int TR, TG, TB ;
		int TBR, TBG, TBB ;
		int BlendRatio ;
		int TempBlendRatio ;
		float fRGB[ 4 ] ;
		int iRGB[ 4 ] ;

		fRGB[ 3 ] = 0.0f ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		BlendRatio = _FTOL( Info->BlendRatio * 256.0f ) ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Blend = ( BYTE * )Info->BlendBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch   = Info->SrcBaseImage.Pitch   - Width * 4 ;
//		BlendAddPitch = Info->BlendBaseImage.Pitch - Width * 4 ;
		DestAddPitch  = Info->DestBaseImage.Pitch  - Width * 4 ;

		switch( Info->FilterOrBlendType )
		{
		case DX_GRAPH_BLEND_PMA_NORMAL :
			{
DX_GRAPH_BLEND_PMA_NORMAL_LABEL :
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;
						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( TBB - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( TBG - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( TBR - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_NORMAL :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_NORMAL_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( Blend[ 0 ] - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( Blend[ 1 ] - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( Blend[ 2 ] - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_MULTIPLE :
			{
DX_GRAPH_BLEND_PMA_MULTIPLE_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;
						B = ( TB * TBB ) >> 8 ;
						G = ( TG * TBG ) >> 8 ;
						R = ( TR * TBR ) >> 8 ;
						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_MULTIPLE :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_MULTIPLE_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						B = ( Src[ 0 ] * Blend[ 0 ] ) >> 8 ;
						G = ( Src[ 1 ] * Blend[ 1 ] ) >> 8 ;
						R = ( Src[ 2 ] * Blend[ 2 ] ) >> 8 ;
						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_DIFFERENCE :
			{
DX_GRAPH_BLEND_PMA_DIFFERENCE_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;
						B = TB - TBB ;
						G = TG - TBG ;
						R = TR - TBR ;
						if( B < 0 ) B = 0 ;
						if( G < 0 ) G = 0 ;
						if( R < 0 ) R = 0 ;
						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_DIFFERENCE :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_DIFFERENCE_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						B = Src[ 0 ] - Blend[ 0 ] ;
						G = Src[ 1 ] - Blend[ 1 ] ;
						R = Src[ 2 ] - Blend[ 2 ] ;
						if( B < 0 ) B = 0 ;
						if( G < 0 ) G = 0 ;
						if( R < 0 ) R = 0 ;
						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_ADD :
			{
DX_GRAPH_BLEND_PMA_ADD_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;
						B = TB + TBB ;
						G = TG + TBG ;
						R = TR + TBR ;
						if( B > 255 ) B = 255 ;
						if( G > 255 ) G = 255 ;
						if( R > 255 ) R = 255 ;
						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_ADD :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_ADD_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						B = Src[ 0 ] + Blend[ 0 ] ;
						G = Src[ 1 ] + Blend[ 1 ] ;
						R = Src[ 2 ] + Blend[ 2 ] ;
						if( B > 255 ) B = 255 ;
						if( G > 255 ) G = 255 ;
						if( R > 255 ) R = 255 ;
						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_SCREEN :
			{
DX_GRAPH_BLEND_PMA_SCREEN_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;
						B = 255 - ( ( ( 255 - TB ) * ( 255 - TBB ) ) >> 8 ) ;
						G = 255 - ( ( ( 255 - TG ) * ( 255 - TBG ) ) >> 8 ) ;
						R = 255 - ( ( ( 255 - TR ) * ( 255 - TBR ) ) >> 8 ) ;
						if( B > 255 ) B = 255 ;
						if( G > 255 ) G = 255 ;
						if( R > 255 ) R = 255 ;
						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_SCREEN :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_SCREEN_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						B = 255 - ( ( ( 255 - Src[ 0 ] ) * ( 255 - Blend[ 0 ] ) ) >> 8 ) ;
						G = 255 - ( ( ( 255 - Src[ 1 ] ) * ( 255 - Blend[ 1 ] ) ) >> 8 ) ;
						R = 255 - ( ( ( 255 - Src[ 2 ] ) * ( 255 - Blend[ 2 ] ) ) >> 8 ) ;
						if( B > 255 ) B = 255 ;
						if( G > 255 ) G = 255 ;
						if( R > 255 ) R = 255 ;
						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_OVERLAY :
			{
DX_GRAPH_BLEND_PMA_OVERLAY_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						if( TB < 128 )
						{
							B = ( TB * TBB * 2 ) >> 8 ;
						}
						else
						{
							B = ( ( 2 * ( ( ( TB + TBB ) << 8 ) - TB * TBB ) ) >> 8 ) - 255 ;
						}
						if( B > 255 ) B = 255 ;

						if( TG < 128 )
						{
							G = ( TG * TBG * 2 ) >> 8 ;
						}
						else
						{
							G = ( ( 2 * ( ( ( TG + TBG ) << 8 ) - TG * TBG ) ) >> 8 ) - 255 ;
						}
						if( G > 255 ) G = 255 ;

						if( TR < 128 )
						{
							R = ( TR * TBR * 2 ) >> 8 ;
						}
						else
						{
							R = ( ( 2 * ( ( ( TR + TBR ) << 8 ) - TR * TBR ) ) >> 8 ) - 255 ;
						}
						if( R > 255 ) R = 255 ;

						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_OVERLAY :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_OVERLAY_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;

						if( Src[ 0 ] < 128 )
						{
							B = ( Src[ 0 ] * Blend[ 0 ] * 2 ) >> 8 ;
						}
						else
						{
							B = ( ( 2 * ( ( ( Src[ 0 ] + Blend[ 0 ] ) << 8 ) - Src[ 0 ] * Blend[ 0 ] ) ) >> 8 ) - 255 ;
						}
						if( B > 255 ) B = 255 ;

						if( Src[ 1 ] < 128 )
						{
							G = ( Src[ 1 ] * Blend[ 1 ] * 2 ) >> 8 ;
						}
						else
						{
							G = ( ( 2 * ( ( ( Src[ 1 ] + Blend[ 1 ] ) << 8 ) - Src[ 1 ] * Blend[ 1 ] ) ) >> 8 ) - 255 ;
						}
						if( G > 255 ) G = 255 ;

						if( Src[ 2 ] < 128 )
						{
							R = ( Src[ 2 ] * Blend[ 2 ] * 2 ) >> 8 ;
						}
						else
						{
							R = ( ( 2 * ( ( ( Src[ 2 ] + Blend[ 2 ] ) << 8 ) - Src[ 2 ] * Blend[ 2 ] ) ) >> 8 ) - 255 ;
						}
						if( R > 255 ) R = 255 ;

						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_DODGE :
			{
DX_GRAPH_BLEND_PMA_DODGE_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						if( TBB < 255 )
						{
							B = ( TB << 8 ) / ( 255 - TBB ) ;
							if( B > 255 ) B = 255 ;
						}
						else
						{
							B = 255 ;
						}

						if( TBG < 255 )
						{
							G = ( TG << 8 ) / ( 255 - TBG ) ;
							if( G > 255 ) G = 255 ;
						}
						else
						{
							G = 255 ;
						}

						if( TBR < 255 )
						{
							R = ( TR << 8 ) / ( 255 - TBR ) ;
							if( R > 255 ) R = 255 ;
						}
						else
						{
							R = 255 ;
						}

						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_DODGE :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_DODGE_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;

						if( Blend[ 0 ] < 255 )
						{
							B = ( Src[ 0 ] << 8 ) / ( 255 - Blend[ 0 ] ) ;
							if( B > 255 ) B = 255 ;
						}
						else
						{
							B = 255 ;
						}

						if( Blend[ 1 ] < 255 )
						{
							G = ( Src[ 1 ] << 8 ) / ( 255 - Blend[ 1 ] ) ;
							if( G > 255 ) G = 255 ;
						}
						else
						{
							G = 255 ;
						}

						if( Blend[ 2 ] < 255 )
						{
							R = ( Src[ 2 ] << 8 ) / ( 255 - Blend[ 2 ] ) ;
							if( R > 255 ) R = 255 ;
						}
						else
						{
							R = 255 ;
						}

						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_BURN :
			{
DX_GRAPH_BLEND_PMA_BURN_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						B = 255 - ( ( 255 - TB ) << 8 ) / ( TBB + 1 ) ;
						if( B < 0 ) B = 0 ;

						G = 255 - ( ( 255 - TG ) << 8 ) / ( TBG + 1 ) ;
						if( G < 0 ) G = 0 ;

						R = 255 - ( ( 255 - TR ) << 8 ) / ( TBR + 1 ) ;
						if( R < 0 ) R = 0 ;

						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_BURN :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_BURN_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;

						B = 255 - ( ( 255 - Src[ 0 ] ) << 8 ) / ( Blend[ 0 ] + 1 ) ;
						if( B < 0 ) B = 0 ;

						G = 255 - ( ( 255 - Src[ 1 ] ) << 8 ) / ( Blend[ 1 ] + 1 ) ;
						if( G < 0 ) G = 0 ;

						R = 255 - ( ( 255 - Src[ 2 ] ) << 8 ) / ( Blend[ 2 ] + 1 ) ;
						if( R < 0 ) R = 0 ;

						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_DARKEN :
			{
DX_GRAPH_BLEND_PMA_DARKEN_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						B = TB < TBB ? TB : TBB ;
						G = TG < TBG ? TG : TBG ;
						R = TR < TBR ? TR : TBR ;

						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_DARKEN :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_DARKEN_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;

						B = Src[ 0 ] < Blend[ 0 ] ? Src[ 0 ] : Blend[ 0 ] ;
						G = Src[ 1 ] < Blend[ 1 ] ? Src[ 1 ] : Blend[ 1 ] ;
						R = Src[ 2 ] < Blend[ 2 ] ? Src[ 2 ] : Blend[ 2 ] ;

						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_LIGHTEN :
			{
DX_GRAPH_BLEND_PMA_LIGHTEN_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						B = TB > TBB ? TB : TBB ;
						G = TG > TBG ? TG : TBG ;
						R = TR > TBR ? TR : TBR ;

						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_LIGHTEN :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_LIGHTEN_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;

						B = Src[ 0 ] > Blend[ 0 ] ? Src[ 0 ] : Blend[ 0 ] ;
						G = Src[ 1 ] > Blend[ 1 ] ? Src[ 1 ] : Blend[ 1 ] ;
						R = Src[ 2 ] > Blend[ 2 ] ? Src[ 2 ] : Blend[ 2 ] ;

						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_SOFTLIGHT :
			{
DX_GRAPH_BLEND_PMA_SOFTLIGHT_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						if( TBB < 128 )
						{
							fRGB[ 0 ] = _POW( TB / 255.0f, ( 1.0f - TBB / 255.0f ) * 2.0f ) ;
						}
						else
						{
							fRGB[ 0 ] = _POW( TB / 255.0f, 0.5f / ( TBB / 255.0f ) ) ;
						}

						if( TBG < 128 )
						{
							fRGB[ 1 ] = _POW( TG / 255.0f, ( 1.0f - TBG / 255.0f ) * 2.0f ) ;
						}
						else
						{
							fRGB[ 1 ] = _POW( TG / 255.0f, 0.5f / ( TBG / 255.0f ) ) ;
						}

						if( TBR < 128 )
						{
							fRGB[ 2 ] = _POW( TR / 255.0f, ( 1.0f - TBR / 255.0f ) * 2.0f ) ;
						}
						else
						{
							fRGB[ 2 ] = _POW( TR / 255.0f, 0.5f / ( TBR / 255.0f ) ) ;
						}

						fRGB[ 0 ] *= 255.0f ;
						fRGB[ 1 ] *= 255.0f ;
						fRGB[ 2 ] *= 255.0f ;
						_FTOL4( fRGB, iRGB ) ;

						B = iRGB[ 0 ] < 0 ? 0 : ( iRGB[ 0 ] > 255 ? 255 : iRGB[ 0 ] ) ;
						G = iRGB[ 1 ] < 0 ? 0 : ( iRGB[ 1 ] > 255 ? 255 : iRGB[ 1 ] ) ;
						R = iRGB[ 2 ] < 0 ? 0 : ( iRGB[ 2 ] > 255 ? 255 : iRGB[ 2 ] ) ;

						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_SOFTLIGHT :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_SOFTLIGHT_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;

						if( Blend[ 0 ] < 128 )
						{
							fRGB[ 0 ] = _POW( Src[ 0 ] / 255.0f, ( 1.0f - Blend[ 0 ] / 255.0f ) * 2.0f ) ;
						}
						else
						{
							fRGB[ 0 ] = _POW( Src[ 0 ] / 255.0f, 0.5f / ( Blend[ 0 ] / 255.0f ) ) ;
						}

						if( Blend[ 1 ] < 128 )
						{
							fRGB[ 1 ] = _POW( Src[ 1 ] / 255.0f, ( 1.0f - Blend[ 1 ] / 255.0f ) * 2.0f ) ;
						}
						else
						{
							fRGB[ 1 ] = _POW( Src[ 1 ] / 255.0f, 0.5f / ( Blend[ 1 ] / 255.0f ) ) ;
						}

						if( Blend[ 2 ] < 128 )
						{
							fRGB[ 2 ] = _POW( Src[ 2 ] / 255.0f, ( 1.0f - Blend[ 2 ] / 255.0f ) * 2.0f ) ;
						}
						else
						{
							fRGB[ 2 ] = _POW( Src[ 2 ] / 255.0f, 0.5f / ( Blend[ 2 ] / 255.0f ) ) ;
						}

						fRGB[ 0 ] *= 255.0f ;
						fRGB[ 1 ] *= 255.0f ;
						fRGB[ 2 ] *= 255.0f ;
						_FTOL4( fRGB, iRGB ) ;

						B = iRGB[ 0 ] < 0 ? 0 : ( iRGB[ 0 ] > 255 ? 255 : iRGB[ 0 ] ) ;
						G = iRGB[ 1 ] < 0 ? 0 : ( iRGB[ 1 ] > 255 ? 255 : iRGB[ 1 ] ) ;
						R = iRGB[ 2 ] < 0 ? 0 : ( iRGB[ 2 ] > 255 ? 255 : iRGB[ 2 ] ) ;

						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_HARDLIGHT :
			{
DX_GRAPH_BLEND_PMA_HARDLIGHT_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						if( TBB < 128 )
						{
							B = ( TB * TBB * 2 ) >> 8 ;
						}
						else
						{
							B = ( ( 2 * ( ( ( TB + TBB ) << 8 ) - ( TB * TBB ) ) ) >> 8 ) - 255 ;
						}
						if( B > 255 ) B = 255 ;

						if( TBG < 128 )
						{
							G = ( TG * TBG * 2 ) >> 8 ;
						}
						else
						{
							G = ( ( 2 * ( ( ( TG + TBG ) << 8 ) - ( TG * TBG ) ) ) >> 8 ) - 255 ;
						}
						if( G > 255 ) G = 255 ;

						if( TBR < 128 )
						{
							R = ( TR * TBR * 2 ) >> 8 ;
						}
						else
						{
							R = ( ( 2 * ( ( ( TR + TBR ) << 8 ) - ( TR * TBR ) ) ) >> 8 ) - 255 ;
						}
						if( R > 255 ) R = 255 ;

						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_HARDLIGHT :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_HARDLIGHT_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;

						if( Blend[ 0 ] < 128 )
						{
							B = ( Src[ 0 ] * Blend[ 0 ] * 2 ) >> 8 ;
						}
						else
						{
							B = ( ( 2 * ( ( ( Src[ 0 ] + Blend[ 0 ] ) << 8 ) - ( Src[ 0 ] * Blend[ 0 ] ) ) ) >> 8 ) - 255 ;
						}
						if( B > 255 ) B = 255 ;

						if( Blend[ 1 ] < 128 )
						{
							G = ( Src[ 1 ] * Blend[ 1 ] * 2 ) >> 8 ;
						}
						else
						{
							G = ( ( 2 * ( ( ( Src[ 1 ] + Blend[ 1 ] ) << 8 ) - ( Src[ 1 ] * Blend[ 1 ] ) ) ) >> 8 ) - 255 ;
						}
						if( G > 255 ) G = 255 ;

						if( Blend[ 2 ] < 128 )
						{
							R = ( Src[ 2 ] * Blend[ 2 ] * 2 ) >> 8 ;
						}
						else
						{
							R = ( ( 2 * ( ( ( Src[ 2 ] + Blend[ 2 ] ) << 8 ) - ( Src[ 2 ] * Blend[ 2 ] ) ) ) >> 8 ) - 255 ;
						}
						if( R > 255 ) R = 255 ;

						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_EXCLUSION :
			{
DX_GRAPH_BLEND_PMA_EXCLUSION_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						B = ( ( ( TB + TBB ) << 8 ) - 2 * ( TB * TBB ) ) >> 8 ;
						if( B > 255 ) B = 255 ;

						G = ( ( ( TG + TBG ) << 8 ) - 2 * ( TG * TBG ) ) >> 8 ;
						if( G > 255 ) G = 255 ;

						R = ( ( ( TR + TBR ) << 8 ) - 2 * ( TR * TBR ) ) >> 8 ;
						if( R > 255 ) R = 255 ;

						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TB << 16 ) + ( B - TB ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TG << 16 ) + ( G - TG ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ( TR << 16 ) + ( R - TR ) * TempBlendRatio ) >> 16 ) ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_EXCLUSION :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_EXCLUSION_LABEL ;
			}
			else
			{
				do
				{
					i = Width ;
					do
					{
						TempBlendRatio = BlendRatio * Blend[ 3 ] ;

						B = ( ( ( Src[ 0 ] + Blend[ 0 ] ) << 8 ) - 2 * ( Src[ 0 ] * Blend[ 0 ] ) ) >> 8 ;
						if( B > 255 ) B = 255 ;

						G = ( ( ( Src[ 1 ] + Blend[ 1 ] ) << 8 ) - 2 * ( Src[ 1 ] * Blend[ 1 ] ) ) >> 8 ;
						if( G > 255 ) G = 255 ;

						R = ( ( ( Src[ 2 ] + Blend[ 2 ] ) << 8 ) - 2 * ( Src[ 2 ] * Blend[ 2 ] ) ) >> 8 ;
						if( R > 255 ) R = 255 ;

						Dest[ 0 ] = ( BYTE )( ( ( Src[ 0 ] << 16 ) + ( B - Src[ 0 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( Src[ 1 ] << 16 ) + ( G - Src[ 1 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( Src[ 2 ] << 16 ) + ( R - Src[ 2 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_NORMAL_ALPHACH :
			{
DX_GRAPH_BLEND_PMA_NORMAL_ALPHACH_LABEL : 
				DWORD TempAlpha ;
				DWORD ReverseBlendAlpha ;
				DWORD BlendAlpha ;

				do
				{
					i = Width ;
					do
					{
						BlendAlpha = ( DWORD )( ( Blend[ 3 ] * BlendRatio ) >> 8 ) ;
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						ReverseBlendAlpha = 256 - BlendAlpha ;
						TempAlpha = ReverseBlendAlpha * Src[ 3 ] + ( BlendAlpha << 8 ) ;
						if( TempAlpha != 0 )
						{
							Dest[ 3 ] = ( BYTE )( TempAlpha >> 8 ) ;
							Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ReverseBlendAlpha * Src[ 3 ] * TB + ( ( BlendAlpha * TBB ) << 8 ) ) / TempAlpha ) ][ Dest[ 3 ] ] ;
							Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ReverseBlendAlpha * Src[ 3 ] * TG + ( ( BlendAlpha * TBG ) << 8 ) ) / TempAlpha ) ][ Dest[ 3 ] ] ;
							Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( ( ReverseBlendAlpha * Src[ 3 ] * TR + ( ( BlendAlpha * TBR ) << 8 ) ) / TempAlpha ) ][ Dest[ 3 ] ] ;
						}

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_NORMAL_ALPHACH :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_NORMAL_ALPHACH_LABEL ;
			}
			else
			{
				DWORD TempAlpha ;
				DWORD ReverseBlendAlpha ;
				DWORD BlendAlpha ;

				do
				{
					i = Width ;
					do
					{
						BlendAlpha = ( DWORD )( ( Blend[ 3 ] * BlendRatio ) >> 8 ) ;

						ReverseBlendAlpha = 256 - BlendAlpha ;
						TempAlpha = ReverseBlendAlpha * Src[ 3 ] + ( BlendAlpha << 8 ) ;
						if( TempAlpha != 0 )
						{
							Dest[ 0 ] = ( BYTE )( ( ReverseBlendAlpha * Src[ 3 ] * Src[ 0 ] + ( ( BlendAlpha * Blend[ 0 ] ) << 8 ) ) / TempAlpha ) ;
							Dest[ 1 ] = ( BYTE )( ( ReverseBlendAlpha * Src[ 3 ] * Src[ 1 ] + ( ( BlendAlpha * Blend[ 1 ] ) << 8 ) ) / TempAlpha ) ;
							Dest[ 2 ] = ( BYTE )( ( ReverseBlendAlpha * Src[ 3 ] * Src[ 2 ] + ( ( BlendAlpha * Blend[ 2 ] ) << 8 ) ) / TempAlpha ) ;
							Dest[ 3 ] = ( BYTE )( TempAlpha >> 8 ) ;
						}

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_ADD_ALPHACH :
			{
DX_GRAPH_BLEND_PMA_ADD_ALPHACH_LABEL : 
				DWORD TempAlpha ;
				DWORD TempC ;
				DWORD BlendAlpha ;

				do
				{
					i = Width ;
					do
					{
						TB  = BASEIM.PmaToRgbTable[ Src  [ 0 ] ][ Src  [ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src  [ 1 ] ][ Src  [ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src  [ 2 ] ][ Src  [ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						BlendAlpha = ( DWORD )( ( Blend[ 3 ] * BlendRatio ) >> 8 ) ;
						TempAlpha  = ( ( 256 - BlendAlpha ) * Src[ 3 ] + ( BlendAlpha << 8 ) ) >> 8 ;
						if( TempAlpha != 0 )
						{
							TempC = ( Src[ 3 ] * TB + BlendAlpha * TBB ) / TempAlpha ;
							Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( BYTE )( TempC > 255 ? 255 : TempC ) ][ TempAlpha ] ;

							TempC = ( Src[ 3 ] * TG + BlendAlpha * TBG ) / TempAlpha ;
							Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( BYTE )( TempC > 255 ? 255 : TempC ) ][ TempAlpha ] ;

							TempC = ( Src[ 3 ] * TR + BlendAlpha * TBR ) / TempAlpha ;
							Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( BYTE )( TempC > 255 ? 255 : TempC ) ][ TempAlpha ] ;

							Dest[ 3 ] = ( BYTE )( TempAlpha ) ;
						}

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_ADD_ALPHACH :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_ADD_ALPHACH_LABEL ;
			}
			else
			{
				DWORD TempAlpha ;
				DWORD TempC ;
				DWORD BlendAlpha ;

				do
				{
					i = Width ;
					do
					{
						BlendAlpha = ( DWORD )( ( Blend[ 3 ] * BlendRatio ) >> 8 ) ;
						TempAlpha  = ( ( 256 - BlendAlpha ) * Src[ 3 ] + ( BlendAlpha << 8 ) ) >> 8 ;
						if( TempAlpha != 0 )
						{
							TempC = ( Src[ 3 ] * Src[ 0 ] + BlendAlpha * Blend[ 0 ] ) / TempAlpha ;
							Dest[ 0 ] = ( BYTE )( TempC > 255 ? 255 : TempC ) ;

							TempC = ( Src[ 3 ] * Src[ 1 ] + BlendAlpha * Blend[ 1 ] ) / TempAlpha ;
							Dest[ 1 ] = ( BYTE )( TempC > 255 ? 255 : TempC ) ;

							TempC = ( Src[ 3 ] * Src[ 2 ] + BlendAlpha * Blend[ 2 ] ) / TempAlpha ;
							Dest[ 2 ] = ( BYTE )( TempC > 255 ? 255 : TempC ) ;

							Dest[ 3 ] = ( BYTE )( TempAlpha ) ;
						}

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_MULTIPLE_A_ONLY :
			{
DX_GRAPH_BLEND_PMA_MULTIPLE_A_ONLY_LABEL : 
				TempBlendRatio = BlendRatio * 255 ;
				do
				{
					i = Width ;
					do
					{
						TB  = BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] ;

						A = ( Src[ 3 ] * Blend[ 3 ] ) >> 8 ;
						Dest[ 3 ] = ( BYTE )( ( ( Src[ 3 ] << 16 ) + ( A - Src[ 3 ] ) * TempBlendRatio ) >> 16 ) ;
						Dest[ 0 ] = BASEIM.RgbToPmaTable[ TB ][ Dest[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ TG ][ Dest[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ TR ][ Dest[ 3 ] ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_MULTIPLE_A_ONLY :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_MULTIPLE_A_ONLY_LABEL ;
			}
			else
			{
				TempBlendRatio = BlendRatio * 255 ;
				do
				{
					i = Width ;
					do
					{
						A = ( Src[ 3 ] * Blend[ 3 ] ) >> 8 ;
						Dest[ 0 ] = Src[ 0 ] ;
						Dest[ 1 ] = Src[ 1 ] ;
						Dest[ 2 ] = Src[ 2 ] ;
						Dest[ 3 ] = ( BYTE )( ( ( Src[ 3 ] << 16 ) + ( A - Src[ 3 ] ) * TempBlendRatio ) >> 16 ) ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_PMA_MASK :
			{
				BYTE CalcR, CalcG, CalcB ;
DX_GRAPH_BLEND_PMA_MASK_LABEL : 
				do
				{
					i = Width ;
					do
					{
						TB  = BASEIM.PmaToRgbTable[ Src[ 0 ] ][ Src[ 3 ] ] ;
						TG  = BASEIM.PmaToRgbTable[ Src[ 1 ] ][ Src[ 3 ] ] ;
						TR  = BASEIM.PmaToRgbTable[ Src[ 2 ] ][ Src[ 3 ] ] ;
						TBB = BASEIM.PmaToRgbTable[ Blend[ 0 ] ][ Blend[ 3 ] ] ;
						TBG = BASEIM.PmaToRgbTable[ Blend[ 1 ] ][ Blend[ 3 ] ] ;
						TBR = BASEIM.PmaToRgbTable[ Blend[ 2 ] ][ Blend[ 3 ] ] ;

						CalcB = ( BYTE )( ( ( TBB - TB ) * Blend[ 3 ] + ( TB << 8 ) ) >> 8 ) ;
						CalcG = ( BYTE )( ( ( TBG - TG ) * Blend[ 3 ] + ( TG << 8 ) ) >> 8 ) ;
						CalcR = ( BYTE )( ( ( TBR - TR ) * Blend[ 3 ] + ( TR << 8 ) ) >> 8 ) ;

						Dest[ 0 ] = BASEIM.RgbToPmaTable[ ( ( CalcB - TB ) * BlendRatio + ( TB << 8 ) ) >> 8 ][ Src[ 3 ] ] ;
						Dest[ 1 ] = BASEIM.RgbToPmaTable[ ( ( CalcG - TG ) * BlendRatio + ( TG << 8 ) ) >> 8 ][ Src[ 3 ] ] ;
						Dest[ 2 ] = BASEIM.RgbToPmaTable[ ( ( CalcR - TR ) * BlendRatio + ( TR << 8 ) ) >> 8 ][ Src[ 3 ] ] ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;

		case DX_GRAPH_BLEND_MASK :
			if( IsPMA )
			{
				goto DX_GRAPH_BLEND_PMA_MASK_LABEL ;
			}
			else
			{
				BYTE CalcR, CalcG, CalcB ;

				do
				{
					i = Width ;
					do
					{
						CalcB = ( BYTE )( ( ( Blend[ 0 ] - Src[ 0 ] ) * Blend[ 3 ] + ( Src[ 0 ] << 8 ) ) >> 8 ) ;
						CalcG = ( BYTE )( ( ( Blend[ 1 ] - Src[ 1 ] ) * Blend[ 3 ] + ( Src[ 1 ] << 8 ) ) >> 8 ) ;
						CalcR = ( BYTE )( ( ( Blend[ 2 ] - Src[ 2 ] ) * Blend[ 3 ] + ( Src[ 2 ] << 8 ) ) >> 8 ) ;

						Dest[ 0 ] = ( BYTE )( ( ( CalcB - Src[ 0 ] ) * BlendRatio + ( Src[ 0 ] << 8 ) ) >> 8 ) ;
						Dest[ 1 ] = ( BYTE )( ( ( CalcG - Src[ 1 ] ) * BlendRatio + ( Src[ 1 ] << 8 ) ) >> 8 ) ;
						Dest[ 2 ] = ( BYTE )( ( ( CalcR - Src[ 2 ] ) * BlendRatio + ( Src[ 2 ] << 8 ) ) >> 8 ) ;
						Dest[ 3 ] = Src[ 3 ] ;

						Src   += 4 ;
						Blend += 4 ;
						Dest  += 4 ;
					}while( -- i ) ;

					Src   += SrcAddPitch ;
					Blend += SrcAddPitch ;
					Dest  += DestAddPitch ;
				}while( -- Height ) ;
			}
			break ;
		}

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}

extern int	GraphBlend_RGBA_Select_Mix( GRAPHFILTER_INFO *Info, int SelectR, int SelectG, int SelectB, int SelectA, int IsPMA )
{
	// シェーダーが使えるかどうかで処理を分岐
	if( GSYS.HardInfo.UseShader == TRUE )
	{
		GraphBlend_RGBA_Select_Mix_PF( Info, SelectR, SelectG, SelectB, SelectA, IsPMA ) ;
	}
	else
	{
		// シェーダーが使えない場合

		BYTE *Src ;
		BYTE *Blend ;
		BYTE *Dest ;
		DWORD i ;
		DWORD Width ;
		DWORD Height ;
		DWORD SrcAddPitch ;
//		DWORD BlendAddPitch ;
		DWORD DestAddPitch ;

		if( GraphFilter_SoftImageSetup( Info ) < 0 )
			return -1 ;

		Src = ( BYTE * )Info->SrcBaseImage.GraphData ;
		Blend = ( BYTE * )Info->BlendBaseImage.GraphData ;
		Dest = ( BYTE * )Info->DestBaseImage.GraphData ;

		Width  = ( DWORD )( Info->SrcX2 - Info->SrcX1 ) ;
		Height = ( DWORD )( Info->SrcY2 - Info->SrcY1 ) ;

		SrcAddPitch   = Info->SrcBaseImage.Pitch   - Width * 4 ;
//		BlendAddPitch = Info->BlendBaseImage.Pitch - Width * 4 ;
		DestAddPitch  = Info->DestBaseImage.Pitch  - Width * 4 ;

		do
		{
			i = Width ;
			do
			{
				switch( SelectB )
				{
				case DX_RGBA_SELECT_SRC_R :   Dest[ 0 ] = Src[ 2 ] ;   break ;
				case DX_RGBA_SELECT_SRC_G :   Dest[ 0 ] = Src[ 1 ] ;   break ;
				case DX_RGBA_SELECT_SRC_B :   Dest[ 0 ] = Src[ 0 ] ;   break ;
				case DX_RGBA_SELECT_SRC_A :   Dest[ 0 ] = Src[ 3 ] ;   break ;
				case DX_RGBA_SELECT_BLEND_R : Dest[ 0 ] = Blend[ 2 ] ; break ;
				case DX_RGBA_SELECT_BLEND_G : Dest[ 0 ] = Blend[ 1 ] ; break ;
				case DX_RGBA_SELECT_BLEND_B : Dest[ 0 ] = Blend[ 0 ] ; break ;
				case DX_RGBA_SELECT_BLEND_A : Dest[ 0 ] = Blend[ 3 ] ; break ;
				}

				switch( SelectG )
				{
				case DX_RGBA_SELECT_SRC_R :   Dest[ 1 ] = Src[ 2 ] ;   break ;
				case DX_RGBA_SELECT_SRC_G :   Dest[ 1 ] = Src[ 1 ] ;   break ;
				case DX_RGBA_SELECT_SRC_B :   Dest[ 1 ] = Src[ 0 ] ;   break ;
				case DX_RGBA_SELECT_SRC_A :   Dest[ 1 ] = Src[ 3 ] ;   break ;
				case DX_RGBA_SELECT_BLEND_R : Dest[ 1 ] = Blend[ 2 ] ; break ;
				case DX_RGBA_SELECT_BLEND_G : Dest[ 1 ] = Blend[ 1 ] ; break ;
				case DX_RGBA_SELECT_BLEND_B : Dest[ 1 ] = Blend[ 0 ] ; break ;
				case DX_RGBA_SELECT_BLEND_A : Dest[ 1 ] = Blend[ 3 ] ; break ;
				}

				switch( SelectR )
				{
				case DX_RGBA_SELECT_SRC_R :   Dest[ 2 ] = Src[ 2 ] ;   break ;
				case DX_RGBA_SELECT_SRC_G :   Dest[ 2 ] = Src[ 1 ] ;   break ;
				case DX_RGBA_SELECT_SRC_B :   Dest[ 2 ] = Src[ 0 ] ;   break ;
				case DX_RGBA_SELECT_SRC_A :   Dest[ 2 ] = Src[ 3 ] ;   break ;
				case DX_RGBA_SELECT_BLEND_R : Dest[ 2 ] = Blend[ 2 ] ; break ;
				case DX_RGBA_SELECT_BLEND_G : Dest[ 2 ] = Blend[ 1 ] ; break ;
				case DX_RGBA_SELECT_BLEND_B : Dest[ 2 ] = Blend[ 0 ] ; break ;
				case DX_RGBA_SELECT_BLEND_A : Dest[ 2 ] = Blend[ 3 ] ; break ;
				}

				switch( SelectA )
				{
				case DX_RGBA_SELECT_SRC_R :   Dest[ 3 ] = Src[ 2 ] ;   break ;
				case DX_RGBA_SELECT_SRC_G :   Dest[ 3 ] = Src[ 1 ] ;   break ;
				case DX_RGBA_SELECT_SRC_B :   Dest[ 3 ] = Src[ 0 ] ;   break ;
				case DX_RGBA_SELECT_SRC_A :   Dest[ 3 ] = Src[ 3 ] ;   break ;
				case DX_RGBA_SELECT_BLEND_R : Dest[ 3 ] = Blend[ 2 ] ; break ;
				case DX_RGBA_SELECT_BLEND_G : Dest[ 3 ] = Blend[ 1 ] ; break ;
				case DX_RGBA_SELECT_BLEND_B : Dest[ 3 ] = Blend[ 0 ] ; break ;
				case DX_RGBA_SELECT_BLEND_A : Dest[ 3 ] = Blend[ 3 ] ; break ;
				}

				Src   += 4 ;
				Blend += 4 ;
				Dest  += 4 ;
			}while( -- i ) ;

			Src   += SrcAddPitch ;
			Blend += SrcAddPitch ;
			Dest  += DestAddPitch ;
		}while( -- Height ) ;

		GraphFilter_SoftImageTerminate( Info ) ;
	}

	// 終了
	return 0 ;
}








// va_list 関数

// 画像にフィルター処理を行う
extern int GraphFilter_VaList( int GrHandle, int FilterType /* DX_GRAPH_FILTER_GAUSS 等 */, va_list VaList )
{
	int Result ;
	int W, H ;

	NS_GetGraphSize( GrHandle, &W, &H ) ;

	Result = GraphFilter_RectBltBase( FALSE, GrHandle, -1, GrHandle, 0, FilterType, 0, 0, W, H, 0, 0, FALSE, 0, 0, FALSE, 0, 0, VaList ) ;

	return Result ;
}

// 画像のフィルター付き転送を行う
extern int GraphFilterBlt_VaList( int SrcGrHandle, int DestGrHandle, int FilterType /* DX_GRAPH_FILTER_GAUSS 等 */, va_list VaList )
{
	int Result ;
	int SrcW, SrcH ;

	NS_GetGraphSize( SrcGrHandle, &SrcW, &SrcH ) ;

	Result = GraphFilter_RectBltBase( FALSE, SrcGrHandle, -1, DestGrHandle, 0, FilterType, 0, 0, SrcW, SrcH, 0, 0, FALSE, 0, 0, FALSE, 0, 0, VaList ) ;

	return Result ;
}

// 画像のフィルター付き転送を行う( 矩形指定 )
extern int GraphFilterRectBlt_VaList( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType /* DX_GRAPH_FILTER_GAUSS 等 */, va_list VaList )
{
	int Result ;

	Result = GraphFilter_RectBltBase( FALSE, SrcGrHandle, -1, DestGrHandle, 0, FilterType, SrcX1, SrcY1, SrcX2, SrcY2, 0, 0, FALSE, 0, 0, FALSE, DestX, DestY, VaList ) ;

	return Result ;
}

// 二つの画像をブレンドする
extern int GraphBlend_VaList( int GrHandle, int BlendGrHandle, int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */, int BlendType /* DX_GRAPH_BLEND_ADD 等 */, va_list VaList )
{
	int Result ;
	int W, H ;

	NS_GetGraphSize( GrHandle, &W, &H ) ;

	Result = GraphFilter_RectBltBase( TRUE, GrHandle, BlendGrHandle, GrHandle, BlendRatio, BlendType, 0, 0, W, H, 0, 0, FALSE, 0, 0, FALSE, 0, 0, VaList ) ;

	return Result ;
}

// 二つの画像をブレンドして結果を指定の画像に出力する
extern int GraphBlendBlt_VaList( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */, int BlendType /* DX_GRAPH_BLEND_ADD 等 */, va_list VaList )
{
	int Result ;
	int SrcW, SrcH ;

	NS_GetGraphSize( SrcGrHandle, &SrcW, &SrcH ) ;

	Result = GraphFilter_RectBltBase( TRUE, SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, 0, 0, SrcW, SrcH, 0, 0, FALSE, 0, 0, FALSE, 0, 0, VaList ) ;

	return Result ;
}

// 二つの画像をブレンドして結果を指定の画像に出力する( 矩形指定 )
extern int GraphBlendRectBlt_VaList( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */, int BlendType /* DX_GRAPH_BLEND_ADD 等 */, va_list VaList )
{
	int Result ;

	Result = GraphFilter_RectBltBase( TRUE, SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, TRUE, 0, 0, FALSE, DestX, DestY, VaList ) ;

	return Result ;
}

// 二つの画像をブレンドして結果を指定の画像に出力する( 矩形指定、ブレンド画像の矩形も指定 )
extern	int	GraphBlendRectBlt2_VaList(	int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX1, int BlendY1, int BlendX2,  int BlendY2, int DestX, int DestY, int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */ , int BlendType /* DX_GRAPH_BLEND_ADD 等 */ , va_list VaList )
{
	int Result ;

	Result = GraphFilter_RectBltBase( TRUE, SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, SrcX1, SrcY1, SrcX2, SrcY2, BlendX1, BlendY1, TRUE, BlendX2, BlendY2, TRUE, DestX, DestY, VaList ) ;

	return Result ;
}








#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FILTER



// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		GraphFilter系プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_GRAPHICSFILTER_H
#define DX_GRAPHICSFILTER_H

#include "DxCompileConfig.h"

#ifndef DX_NON_FILTER

// インクルード ------------------------------------------------------------------
#include "DxLib.h"
#include "DxGraphics.h"
#include <stdarg.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------


// 構造体定義 --------------------------------------------------------------------

// フィルター情報構造体
struct GRAPHFILTER_INFO
{
	int						BlendGraphScalingFilterIsBilinear ;
	int						BltBlendMode ;
	int						IsBlend ;
	int						FilterOrBlendType ;
	int						SrcGrHandle ;
	int						BlendGrHandle ;
	int						DestGrHandle ;
	int						SrcEqualDestClearFlag ;
	float					BlendRatio ;
	int						Pass ;
	int						SrcX1 ;
	int						SrcY1 ;
	int						SrcX2 ;
	int						SrcY2 ;
	int						SrcDivNum ;
	int						BlendX ;
	int						BlendY ;
	int						BlendPosEnable ;
	int						BlendX2 ;
	int						BlendY2 ;
	int						BlendPos2Enable ;
	int						DestX ;
	int						DestY ;

	int						PassNum ;
	int						UseWorkScreen ;

	int						DestWidth ;
	int						DestHeight ;
	BASEIMAGE				SrcBaseImage ;
	BASEIMAGE				BlendBaseImage ;
	BASEIMAGE				DestBaseImage ;

	IMAGEDATA				*SrcImage ;
	IMAGEDATA				*DestImage ;
	IMAGEDATA				*BlendImage ;
	IMAGEDATA				*TargetScreenImage ;
} ;

// フィルター毎情報
struct GRAPHFILTER_PARAM
{
	int						Gauss_PixelWidth ;
	float					Gauss_Param ;
	int						Bright_Clip_CmpType ;
	float					Bright_Clip_CmpParam ;
	int						Bright_Clip_ClipFillFlag ;
	COLOR_F					Bright_Clip_ClipFillColor ;
	int						Bright_Scale_Min ;
	int						Bright_Scale_Max ;
	int						Hsb_HueType ;
	float					Hsb_Hue ;
	float					Hsb_Saturation ;
	float					Hsb_Bright ;
	float					Mono_Cb ;
	float					Mono_Cr ;
	float					Lv_Min ;
	float					Lv_Max ;
	float					Lv_Gamma ;
	float					Lv_AfterMin ;
	float					Lv_AfterMax ;
	float					TC_Threshold ;
	COLOR_F					TC_LowColor ;
	COLOR_F					TC_HighColor ;
	int						GM_MapGrHandle ;
	int						GM_Reverse ;
	int						UVGrHandle ;
	int						DestSizeX ;
	int						DestSizeY ;
	COLOR_U8				TargetColor ;
	COLOR_U8				NextColor ;
	int						DepthMapGrHandle ;
	float					SSAO_KernelRadius ;
	float					SSAO_MinDistance ;
	float					SSAO_MaxDistance ;
	float					SSAO_Strength ;
	int						SSAO_OcclusionColor ;
	float					SSAO_OcclusionPower ;
	int						SSAO_ColorMapHandle ;
	COLOR_F					ColorScale ;
	COLOR_F					ColorPreSub ;

	int						RGBA_R ;
	int						RGBA_G ;
	int						RGBA_B ;
	int						RGBA_A ;
} ;

// フィルターのシェーダーハンドル
struct GRAPHFILTER_SHADER_HANDLE
{
	int						Gauss_PS[ 3 ] ;							// ガウスフィルタのピクセルシェーダー
	int						BrightClipPS[ 2 ][ 2 ][ 2 ] ;			// 明るさでクリップするフィルタのピクセルシェーダー[ 1:一定以上クリップ  0:一定以下クリップ ][ 1:クリップ塗りつぶしあり 0:塗りつぶしなし ][ 0:通常用  1:乗算済みアルファ用 ]
	int						BrightScalePS[ 2 ] ;					// 指定の明るさの領域を拡大するフィルタのピクセルシェーダー[ 0:通常用  1:乗算済みアルファ用 ]
	int						DownScalePS[ 3 ] ;						// 縮小フィルター( 0:X2 1:X4 2:X8 )
	int						HsbPS[ 3 ][ 2 ] ;						// ＨＳＢフィルター[ 0:RGB to HSI  1:HSI to RGB  2:HSB の HLock ][ 0:通常用  1:乗算済みアルファ用 ]
	int						MonoPS ;								// モノトーンフィルター
	int						InvertPS[ 2 ] ;							// 階調反転フィルター[ 0:通常用  1:乗算済みアルファ用 ]
	int						LevelPS[ 2 ] ;							// レベル補正フィルター[ 0:通常用  1:乗算済みアルファ用 ]
	int						GammaTex ;								// ガンマ補正に使用する画像
	float					PrevGamma ;								// 前回のガンマフィルタの際のガンマ値
	int						TwoColorPS[ 2 ] ;						// ２値化フィルター[ 0:通常用  1:乗算済みアルファ用 ]
	int						GradientMapPS[ 2 ][ 2 ] ;				// グラデーションマップフィルター[ 0:通常用  1:乗算済みアルファ用 ]
	int						ReplacementPS[ 2 ] ;					// 色の置換[ 0:通常用  1:乗算済みアルファ用 ]
	int						BilateralBlurPS[ 2 ] ;					// バイラテラルブラー[ 0:通常用  1:乗算済みアルファ用 ]
	int						PreMulAlphaPS ;							// 通常画像から乗算済みアルファ画像を作成する為のフィルター
	int						InterpAlphaPS ;							// 乗算済みアルファ画像から通常画像を作成する為のフィルター
	int						YUVtoRGBPS[ 4 ] ;						// YUVカラーをRGBカラーに変換するフィルター
	int						BicubicPS ;								// バイキュービック補間フィルター
	int						Lanczos3PS ;							// Lanczos-3補間フィルター
	int						SSAOPS[ 3 ] ;							// SSAOフィルター
	int						SSAOMixPS ;								// SSAOの隠蔽係数とカラーバッファを合成するフィルター
	int						FloatColorScalePS ;						// FloatColorScaleフィルター

	int						BasicBlendPS[ DX_GRAPH_BLEND_NUM ] ;	// 基本的なブレンドフィルター
	int						RgbaMixBasePS[ 2 ] ;					// RGBAMixブレンド基本[ 0:通常用  1:乗算済みアルファ用 ]
	int						RgbaMixSRGBB[ 4 ][ 2 ] ;				// RGBAMixブレンドの A だけ BRGBA の４つ[ 0:通常用  1:乗算済みアルファ用 ]
	int						RgbaMixSRRRB[ 4 ][ 4 ][ 2 ] ;			// RGBAMixブレンドの SYYY BX の１６こ [ Y ][ X ][ 0:通常用  1:乗算済みアルファ用 ]
	int						RgbaMixS[ 4 ][ 4 ][ 4 ][ 4 ][ 2 ] ;		// RGBAMixブレンドの S だけの組み合わせ256個[ R ][ G ][ B ][ A ][ 0:通常用  1:乗算済みアルファ用 ]
} ;

// フィルター処理のシステムデータ
struct GRAPHFILTER_SYSTEM_DATA
{
	int						BlendGraphScalingFilterIsNotBilinear ;	// SrcGrHandle と BlendGrHandle のサイズが異なった場合のスケーリングフィルターモードはバイリニア方式ではないかどうか
	int						BltBlendMode ;							// 転送先とのブレンドモード
} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern GRAPHFILTER_SHADER_HANDLE GraphFilterShaderHandle ;
extern GRAPHFILTER_SYSTEM_DATA GraphFilterSystemData ;
extern float SSAO_Kernel[ /*32*//*18*/64 * 4 ] ;

// 関数プロトタイプ宣言-----------------------------------------------------------

extern int	GraphFilter_Initialize( void ) ;	// フィルター処理の初期化
extern int	GraphFilter_Terminate( void ) ;		// フィルター処理の後始末

extern int	GraphFilter_Mono(            GRAPHFILTER_INFO *Info, float Cb, float Cr ) ;
extern int	GraphFilter_Gauss(           GRAPHFILTER_INFO *Info, int PixelWidth, float Param ) ;
extern int	GraphFilter_Down_Scale(      GRAPHFILTER_INFO *Info, int DivNum ) ;
extern int	GraphFilter_Bright_Clip(     GRAPHFILTER_INFO *Info, int CmpType, float CmpParam, int ClipFillFlag, COLOR_F *ClipFillColor, int IsPMA ) ;
extern int	GraphFilter_Bright_Scale(    GRAPHFILTER_INFO *Info, int BrightMin, int BrightMax, int IsPMA ) ;
extern int	GraphFilter_HSB(             GRAPHFILTER_INFO *Info, int HueType, float Hue, float Saturation, float Bright, int IsPMA ) ;
extern int	GraphFilter_Invert(          GRAPHFILTER_INFO *Info, int IsPMA ) ;
extern int	GraphFilter_Level(           GRAPHFILTER_INFO *Info, float Min, float Max, float Gamma, float AfterMin, float AfterMax, int IsPMA ) ;
extern int	GraphFilter_TwoColor(        GRAPHFILTER_INFO *Info, float Threshold, COLOR_F *LowColor, COLOR_F *HighColor, int IsPMA ) ;
extern int	GraphFilter_GradientMap(     GRAPHFILTER_INFO *Info, int MapGrHandle, int Reverse, int IsPMA ) ;
extern int	GraphFilter_Replacement(     GRAPHFILTER_INFO *Info, COLOR_U8 TargetColor, COLOR_U8 NextColor, int IsPMA ) ;
extern int	GraphFilter_BilateralBlur(   GRAPHFILTER_INFO *Info, int IsPMA ) ;
extern int	GraphFilter_PremulAlpha(     GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_InterpAlpha(     GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_YUVtoRGB(        GRAPHFILTER_INFO *Info, int UVGrHandle ) ;
extern int	GraphFilter_BicubicScale(    GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;
extern int	GraphFilter_Lanczos3Scale(   GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;
extern int	GraphFilter_SSAO(            GRAPHFILTER_INFO *Info, int DepthMapGrHandle, float KernelRadius, float MinDistance, float MaxDistance, float Strength, int OcclusionColor, float OcclusionPower, int ColorMapGrHandle ) ;
extern int	GraphFilter_FloatColorScale( GRAPHFILTER_INFO *Info, COLOR_F ColorScale, COLOR_F ColorPreSub ) ;

extern int	GraphFilter_RectBltBase( int IsBlend, int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int FilterOrBlendType, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int BlendPosEnable, int BlendX2, int BlendY2, int BlendPos2Enable, int DestX, int DestY, va_list ParamList ) ;

extern int	GraphBlend_Basic(           GRAPHFILTER_INFO *Info, int IsPMA ) ;
extern int	GraphBlend_RGBA_Select_Mix( GRAPHFILTER_INFO *Info, int SelectR, int SelectG, int SelectB, int SelectA, int IsPMA ) ;



// va_list 関数
extern	int	GraphFilter_VaList(			int    GrHandle,                                                                                                               int FilterType /* DX_GRAPH_FILTER_GAUSS 等 */ , va_list VaList ) ;		// 画像にフィルター処理を行う
extern	int	GraphFilterBlt_VaList(		int SrcGrHandle, int DestGrHandle,                                                                                             int FilterType /* DX_GRAPH_FILTER_GAUSS 等 */ , va_list VaList ) ;		// 画像のフィルター付き転送を行う
extern	int	GraphFilterRectBlt_VaList(	int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX,  int DestY,                          int FilterType /* DX_GRAPH_FILTER_GAUSS 等 */ , va_list VaList ) ;		// 画像のフィルター付き転送を行う( 矩形指定 )
extern	int	GraphBlend_VaList(			int    GrHandle, int BlendGrHandle,                                                                                                                                          int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */ , int BlendType /* DX_GRAPH_BLEND_ADD 等 */ , va_list VaList ) ;	// 二つの画像をブレンドする
extern	int	GraphBlendBlt_VaList(		int SrcGrHandle, int BlendGrHandle, int DestGrHandle,                                                                                                                        int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */ , int BlendType /* DX_GRAPH_BLEND_ADD 等 */ , va_list VaList ) ;	// 二つの画像をブレンドして結果を指定の画像に出力する
extern	int	GraphBlendRectBlt_VaList(	int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX,  int BlendY,                             int DestX, int DestY, int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */ , int BlendType /* DX_GRAPH_BLEND_ADD 等 */ , va_list VaList ) ;	// 二つの画像をブレンドして結果を指定の画像に出力する( 矩形指定 )
extern	int	GraphBlendRectBlt2_VaList(	int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX1, int BlendY1, int BlendX2,  int BlendY2, int DestX, int DestY, int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */ , int BlendType /* DX_GRAPH_BLEND_ADD 等 */ , va_list VaList ) ;	// 二つの画像をブレンドして結果を指定の画像に出力する( 矩形指定、ブレンド画像の矩形も指定 )

// 環境依存関係
extern int	GraphFilter_Initialize_PF( void ) ;
extern int	GraphFilter_Terminate_PF( void ) ;

extern int	GraphFilter_Mono_PF(                GRAPHFILTER_INFO *Info, float Cb, float Cr ) ;
extern int	GraphFilter_Gauss_PF(               GRAPHFILTER_INFO *Info, int PixelWidth, float Param, float *Table ) ;
extern int	GraphFilter_Down_Scale_PF(          GRAPHFILTER_INFO *Info, int DivNum ) ;
extern int	GraphFilter_Bright_Clip_PF(         GRAPHFILTER_INFO *Info, int CmpType, float CmpParam, int ClipFillFlag, COLOR_F *ClipFillColor, int IsPMA ) ;
extern int	GraphFilter_Bright_Scale_PF(        GRAPHFILTER_INFO *Info, int BrightMin, int BrightMax, int IsPMA ) ;
extern int	GraphFilter_HSB_PF(                 GRAPHFILTER_INFO *Info, int HueType, float Hue, float Saturation, float Bright, int IsPMA ) ;
extern int	GraphFilter_Invert_PF(              GRAPHFILTER_INFO *Info, int IsPMA ) ;
extern int	GraphFilter_Level_PF(               GRAPHFILTER_INFO *Info, float Min, float Max, float Gamma, float AfterMin, float AfterMax, int IsPMA ) ;
extern int	GraphFilter_TwoColor_PF(            GRAPHFILTER_INFO *Info, float Threshold, COLOR_F *LowColor, COLOR_F *HighColor, int IsPMA ) ;
extern int	GraphFilter_GradientMap_PF(         GRAPHFILTER_INFO *Info, int MapGrHandle, int Reverse, int IsPMA ) ;
extern int	GraphFilter_Replacement_PF(         GRAPHFILTER_INFO *Info, COLOR_U8 TargetColor, COLOR_U8 NextColor, int IsPMA ) ;
extern int	GraphFilter_BilateralBlur_PF(       GRAPHFILTER_INFO *Info, int IsPMA ) ;
extern int	GraphFilter_PremulAlpha_PF(         GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_InterpAlpha_PF(         GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_YUVtoRGB_PF(            GRAPHFILTER_INFO *Info, int UVGrHandle ) ;
extern int	GraphFilter_BicubicScale_PF(        GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;
extern int	GraphFilter_Lanczos3Scale_PF(       GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;
extern int	GraphFilter_SSAO_PF(                GRAPHFILTER_INFO *Info, int DepthMapGrHandle, float KernelRadius, float MinDistance, float MaxDistance, float Strength, int OcclusionColor, float OcclusionPower, int ColorMapGrHandle ) ;
extern int	GraphFilter_FloatColorScale_PF(     GRAPHFILTER_INFO *Info, COLOR_F ColorScale, COLOR_F ColorPreSub ) ;
extern int	GraphFilter_RectBltBase_Timing0_PF( GRAPHFILTER_INFO *Info, GRAPHFILTER_PARAM *Param ) ;
extern int	GraphFilter_RectBltBase_Timing1_PF( void ) ;
extern int	GraphFilter_DestGraphSetup_PF(      GRAPHFILTER_INFO *Info, int *UseSrcGrHandle, int *UseDestGrHandle ) ;
extern int	GraphFilter_DestGraphUpdate_PF(     GRAPHFILTER_INFO *Info, int UseDestGrHandle, int FilterResult ) ;

extern int	GraphBlend_Basic_PF(                GRAPHFILTER_INFO *Info, int IsPMA ) ;
extern int	GraphBlend_RGBA_Select_Mix_PF(      GRAPHFILTER_INFO *Info, int SelectR, int SelectG, int SelectB, int SelectA, int IsPMA ) ;

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FILTER

#endif // DX_GRAPHICSFILTER_H


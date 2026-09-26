// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		ピクセルシェーダー関連定義
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_SHADER_PS_D3D11_H
#define DX_SHADER_PS_D3D11_H

// インクルード ---------------------------------

#include "DxShader_Common_D3D11.h"

// マクロ定義 -----------------------------------

#define DX_VERTEXLIGHTING_LIGHT_NUM			(3)		// 頂点単位ライティングで同時に使用できるライトの最大数
#define DX_PIXELLIGHTING_LIGHT_NUM			(6)		// ピクセル単位ライティングで同時に使用できるライトの最大数

#define DX_D3D11_PS_CONST_FILTER_SIZE		(1280)	// フィルター用定数バッファのサイズ

// データ型定義 ---------------------------------

// シャドウマップパラメータ
struct DX_D3D11_PS_CONST_SHADOWMAP
{
	DX_D3D11_SHADER_FLOAT		AdjustDepth ;			// 閾値深度補正値
	DX_D3D11_SHADER_FLOAT		GradationParam ;		// グラデーション範囲
	DX_D3D11_SHADER_FLOAT		Enable_Light0 ;			// ライト０への適用情報
	DX_D3D11_SHADER_FLOAT		Enable_Light1 ;			// ライト１への適用情報

	DX_D3D11_SHADER_FLOAT		Enable_Light2 ;			// ライト２への適用情報
	DX_D3D11_SHADER_FLOAT3		Padding ;				// パディング
} ;

// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	DX_D3D11_SHADER_FLOAT4		FactorColor ;			// アルファ値等

	DX_D3D11_SHADER_FLOAT		MulAlphaColor ;			// カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
	DX_D3D11_SHADER_FLOAT		AlphaTestRef ;			// アルファテストで使用する比較値
	DX_D3D11_SHADER_FLOAT2		Padding1 ;

	DX_D3D11_SHADER_INT			AlphaTestCmpMode ;		// アルファテスト比較モード( DX_CMP_NEVER など )
	DX_D3D11_SHADER_INT			NoLightAngleAttenuation ;	// ライトの角度減衰を行わないか( 0:減衰を行う   1:減衰を行わない )
	DX_D3D11_SHADER_INT			UseHalfLambert ;		// ハーフランバートを使用するか( 0:使用しない   1:使用する )
	DX_D3D11_SHADER_INT			Padding2 ;

	DX_D3D11_SHADER_FLOAT4		IgnoreTextureColor ;	// テクスチャカラー無視処理用カラー

	DX_D3D11_SHADER_FLOAT4		DrawAddColor ;			// 加算する色
} ;


// 定数バッファシャドウマップパラメータ
struct DX_D3D11_PS_CONST_BUFFER_SHADOWMAP
{
	DX_D3D11_PS_CONST_SHADOWMAP	Data[ 3 ] ;
} ;


#endif // DX_SHADER_PS_D3D11_H

// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Direct3D11 ピクセルシェーダー定義コード
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// インクルード ------------------------------------------------------------------

#include "DataType.h"
#include "..\..\..\Main\Windows\DxShader_PS_D3D11.h"

// 構造体定義 --------------------------------------------------------------------

// 単純転送用頂点シェーダーの出力
struct STRETCHRECT_VS_OUTPUT
{
	float4 Position        : SV_POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
} ;

// テクスチャ座標8個の単純転送用頂点シェーダーの出力
struct STRETCHRECT_TEX8_VS_OUTPUT
{
	float4 Position        : SV_POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
	float2 TexCoords2      : TEXCOORD2 ;
	float2 TexCoords3      : TEXCOORD3 ;
	float2 TexCoords4      : TEXCOORD4 ;
	float2 TexCoords5      : TEXCOORD5 ;
	float2 TexCoords6      : TEXCOORD6 ;
	float2 TexCoords7      : TEXCOORD7 ;
} ;

// 定数定義 ----------------------------------------------------------------------

// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register( b0 )
{
	DX_D3D11_CONST_BUFFER_COMMON		g_Common ;
} ;

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register( b1 )
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base ;
} ;

// シャドウマップパラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_SHADOWMAP			: register( b2 )
{
	DX_D3D11_PS_CONST_BUFFER_SHADOWMAP	g_ShadowMap ;
} ;

// 関数定義 ----------------------------------------------------------------------

void AlphaTest( float DrawAlpha )
{
#ifdef ALPHATEST_NEVER			// FALSE
	discard ;
#endif // ALPHA_CMP_NEVER

#ifdef ALPHATEST_LESS			// DrawAlpha <  TestParam
	if( DrawAlpha >= g_Base.AlphaTestRef )
	{
		discard ;
	}
#endif // ALPHATEST_LESS

#ifdef ALPHATEST_EQUAL			// DrawAlpha == TestParam
	if( DrawAlpha != g_Base.AlphaTestRef )
	{
		discard ;
	}
#endif // ALPHATEST_EQUAL

#ifdef ALPHATEST_LESSEQUAL		// DrawAlpha <= TestParam
	if( DrawAlpha > g_Base.AlphaTestRef )
	{
		discard ;
	}
#endif // ALPHATEST_LESSEQUAL

#ifdef ALPHATEST_GREATER		// DrawAlpha >  TestParam
	if( DrawAlpha <= g_Base.AlphaTestRef )
	{
		discard ;
	}
#endif // ALPHATEST_GREATER

#ifdef ALPHATEST_NOTEQUAL		// DrawAlpha != TestParam
	if( DrawAlpha == g_Base.AlphaTestRef )
	{
		discard ;
	}
#endif // ALPHATEST_NOTEQUAL

#ifdef ALPHATEST_GREATEREQUAL	// DrawAlpha >= TestParam
	if( DrawAlpha < g_Base.AlphaTestRef )
	{
		discard ;
	}
#endif // ALPHATEST_GREATEREQUAL

#ifdef ALPHATEST_ALWAYS			// TRUE
#endif // ALPHATEST_ALWAYS

#ifdef ALPHATEST_MIX // 使用しない箇所はコメントアウト
//	if( g_Base.AlphaTestCmpMode == 1 )	// DX_CMP_NEVER
//	{
//		discard ;
//	}
//	else
	if( g_Base.AlphaTestCmpMode == 2 )	// DX_CMP_LESS
	{
		if( DrawAlpha >= g_Base.AlphaTestRef )
		{
			discard ;
		}
	}
	else
	if( g_Base.AlphaTestCmpMode == 3 )	// DX_CMP_EQUAL
	{
		if( DrawAlpha != g_Base.AlphaTestRef )
		{
			discard ;
		}
	}
	else
	if( g_Base.AlphaTestCmpMode == 4 )	// DX_CMP_LESSEQUAL
	{
		if( DrawAlpha > g_Base.AlphaTestRef )
		{
			discard ;
		}
	}
	else
	if( g_Base.AlphaTestCmpMode == 5 )	// DX_CMP_GREATER
	{
		if( DrawAlpha <= g_Base.AlphaTestRef )
		{
			discard ;
		}
	}
	else
	if( g_Base.AlphaTestCmpMode == 6 )	// DX_CMP_NOTEQUAL
	{
		if( DrawAlpha == g_Base.AlphaTestRef )
		{
			discard ;
		}
	}
	else
	if( g_Base.AlphaTestCmpMode == 7 )	// DX_CMP_GREATEREQUAL
	{
		if( DrawAlpha < g_Base.AlphaTestRef )
		{
			discard ;
		}
	}
//	else
//	if( g_Base.AlphaTestCmpMode == 8 )	// DX_CMP_ALWAYS
//	{
//		break ;
//	}
#endif // ALPHATEST_MIX
}

#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 Min_LenRev_AfterMin_AfterMax ;
} ;

SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;

SamplerState g_GammaSampler     : register( s1 ) ;
Texture2D    g_GammaTexture     : register( t1 ) ;

float4 Level_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;
	float3 Rgb;
	float Ratio;

	Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	
#ifdef PMA_IMAGE
	if( Color.a < 0.001f )
	{
		Color.r = 0.0f ;
		Color.g = 0.0f ;
		Color.b = 0.0f ;
	}
	else
	{
		Color.rgb = Color.rgb / Color.a;
	}
#endif

	Rgb.rgb = saturate( ( Color.rgb - Min_LenRev_AfterMin_AfterMax.xxx ) * Min_LenRev_AfterMin_AfterMax.y );

	Ratio = g_GammaTexture.Sample( g_GammaSampler, float2( Rgb.r, 0.0 ) ).r ;
	Color.r = lerp( Min_LenRev_AfterMin_AfterMax.z, Min_LenRev_AfterMin_AfterMax.w, Ratio );

	Ratio = g_GammaTexture.Sample( g_GammaSampler, float2( Rgb.g, 0.0 ) ).r ;
	Color.g = lerp( Min_LenRev_AfterMin_AfterMax.z, Min_LenRev_AfterMin_AfterMax.w, Ratio );

	Ratio = g_GammaTexture.Sample( g_GammaSampler, float2( Rgb.b, 0.0 ) ).r ;
	Color.b = lerp( Min_LenRev_AfterMin_AfterMax.z, Min_LenRev_AfterMin_AfterMax.w, Ratio );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}


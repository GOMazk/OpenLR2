#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 RGBToY     ;
	float4 YCbCrToRGB ;
} ;

SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;

float4 Mono_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;
	float3 Rgb;
	float3 RGBToY_F3;
	float3 YCbCrToRGB_F3;

	RGBToY_F3     = RGBToY.rgb;
	YCbCrToRGB_F3 = YCbCrToRGB.rgb;

	Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	Rgb = Color.rgb;

	Rgb.rgb   = dot( Rgb, RGBToY_F3 );
	Color.rgb = Rgb + YCbCrToRGB_F3;

	return Color;
}

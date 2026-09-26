#include "../PixelShader.h"

SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;

float4 PremultipliedAlpha_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;

	Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;

	Color.rgb = Color.rgb * Color.a;
	
	return Color;
}

float4 InterpolatedAlpha_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;

	Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;

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
	
	return Color;
}

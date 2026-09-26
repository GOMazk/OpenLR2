#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 RGBToY_Cmp ;
	float4 LowColor ;
	float4 HighColor ;
} ;

SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;

float4 TwoColor_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;

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

	if( dot( Color.rgb, RGBToY_Cmp.rgb ) > RGBToY_Cmp.w )
	{
		Color = HighColor;
	}
	else
	{
		Color = LowColor;
	}

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

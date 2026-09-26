#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 RGBToY ;
} ;

SamplerState g_SrcSampler           : register( s0 ) ;
Texture2D    g_SrcTexture           : register( t0 ) ;

SamplerState g_GradientMapSampler   : register( s1 ) ;
Texture2D    g_GradientMapTexture   : register( t1 ) ;

float4 GradientMap_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 DestColor ;
	float4 SrcColor ;

	SrcColor = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	
#ifdef PMA_IMAGE
	if( SrcColor.a < 0.001f )
	{
		SrcColor.r = 0.0f ;
		SrcColor.g = 0.0f ;
		SrcColor.b = 0.0f ;
	}
	else
	{
		SrcColor.rgb = SrcColor.rgb / SrcColor.a;
	}
#endif
	
	DestColor = g_GradientMapTexture.Sample( g_GradientMapSampler, float2( dot( SrcColor.rgb, RGBToY.rgb ), 0 ) ) ;
	
#ifdef PMA_IMAGE
	if( DestColor.a < 0.001f )
	{
		DestColor.r = 0.0f ;
		DestColor.g = 0.0f ;
		DestColor.b = 0.0f ;
	}
	else
	{
		DestColor.rgb = DestColor.rgb / DestColor.a;
	}
#endif
	
	DestColor.a *= SrcColor.a ;

#ifdef PMA_IMAGE
	DestColor.rgb *= DestColor.a;
#endif

	return DestColor ;
}

float4 GradientMapRev_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 DestColor ;
	float4 SrcColor ;

	SrcColor = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;

#ifdef PMA_IMAGE
	if( SrcColor.a < 0.001f )
	{
		SrcColor.r = 0.0f ;
		SrcColor.g = 0.0f ;
		SrcColor.b = 0.0f ;
	}
	else
	{
		SrcColor.rgb = SrcColor.rgb / SrcColor.a;
	}
#endif
	
	DestColor = g_GradientMapTexture.Sample( g_GradientMapSampler, float2( 1.0f - dot( SrcColor.rgb, RGBToY.rgb ), 0 ) ) ;
	
#ifdef PMA_IMAGE
	if( DestColor.a < 0.001f )
	{
		DestColor.r = 0.0f ;
		DestColor.g = 0.0f ;
		DestColor.b = 0.0f ;
	}
	else
	{
		DestColor.rgb = DestColor.rgb / DestColor.a;
	}
#endif
	
	DestColor.a *= SrcColor.a ;

#ifdef PMA_IMAGE
	DestColor.rgb *= DestColor.a;
#endif

	return DestColor ;
}

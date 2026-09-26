#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4  g_ClipBrightness ;
	float4  g_RGBToY ;
	float4  g_ClipFillColor ;
} ;
	
SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;


float4 PS_Less(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
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
    
    clip( dot( g_RGBToY, Color ) - g_ClipBrightness.r ) ;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 PS_Less_ClipFill(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
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
    
    if( dot( g_RGBToY, Color ) < g_ClipBrightness.r )
    {
        Color = g_ClipFillColor;
    }
    
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 PS_Greater(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
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
    
    clip( g_ClipBrightness.r - dot( g_RGBToY, Color ) ) ;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 PS_Greater_ClipFill(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
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
    
    if( dot( g_RGBToY, Color ) > g_ClipBrightness.r )
    {
        Color = g_ClipFillColor;
    }
    
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

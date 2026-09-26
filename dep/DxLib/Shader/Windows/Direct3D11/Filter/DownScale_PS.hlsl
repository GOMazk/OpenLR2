#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 PixelOffset[8] ;
} ;

SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;

float4 Down_2_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;

    Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[0].xy ) ;

    return Color;
}


float4 Down_4_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;

    Color  = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[0].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[0].zw ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[1].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[1].zw ) ;

    return Color / 4;
}


float4 Down_8_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;

    Color  = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[0].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[0].zw ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[1].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[1].zw ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[2].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[2].zw ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[3].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[3].zw ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[4].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[4].zw ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[5].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[5].zw ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[6].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[6].zw ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[7].xy ) ;
    Color += g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[7].zw ) ;

    return Color / 16;
}


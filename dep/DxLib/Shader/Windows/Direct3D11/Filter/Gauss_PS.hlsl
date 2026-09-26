#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 PixelWeight[4]  ;
	float4 PixelOffset[8] ;
} ;

SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;

float4 PS_8pixel(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;
	
    Color  = PixelWeight[0].x *   g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 );
    Color += PixelWeight[0].y * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 0 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 0 ].zw ) );
    Color += PixelWeight[0].z * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 1 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 1 ].xy ) );
    Color += PixelWeight[0].w * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 1 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 1 ].zw ) );
	
    return Color;
}

float4 PS_16pixel(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;
	
    Color  = PixelWeight[0].x *   g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 );
    Color += PixelWeight[0].y * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 0 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 0 ].zw ) );
    Color += PixelWeight[0].z * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 1 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 1 ].xy ) );
    Color += PixelWeight[0].w * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 1 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 1 ].zw ) );
    Color += PixelWeight[1].x * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 2 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 2 ].xy ) );
    Color += PixelWeight[1].y * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 2 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 2 ].zw ) );
    Color += PixelWeight[1].z * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 3 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 3 ].xy ) );
    Color += PixelWeight[1].w * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 3 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 3 ].zw ) );
	
    return Color;
}

float4 PS_32pixel(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;
	
    Color  = PixelWeight[0].x *   g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 );
    Color += PixelWeight[0].y * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 0 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 0 ].zw ) );
    Color += PixelWeight[0].z * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 1 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 1 ].xy ) );
    Color += PixelWeight[0].w * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 1 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 1 ].zw ) );
    Color += PixelWeight[1].x * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 2 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 2 ].xy ) );
    Color += PixelWeight[1].y * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 2 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 2 ].zw ) );
    Color += PixelWeight[1].z * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 3 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 3 ].xy ) );
    Color += PixelWeight[1].w * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 3 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 3 ].zw ) );
    Color += PixelWeight[2].x * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 4 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 4 ].xy ) );
    Color += PixelWeight[2].y * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 4 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 4 ].zw ) );
    Color += PixelWeight[2].z * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 5 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 5 ].xy ) );
    Color += PixelWeight[2].w * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 5 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 5 ].zw ) );
    Color += PixelWeight[3].x * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 6 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 6 ].xy ) );
    Color += PixelWeight[3].y * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 6 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 6 ].zw ) );
    Color += PixelWeight[3].z * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 7 ].xy ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 7 ].xy ) );
    Color += PixelWeight[3].w * ( g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 + PixelOffset[ 7 ].zw ) +  g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 - PixelOffset[ 7 ].zw ) );

    return Color;
}


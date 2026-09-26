float2 TextureSize     : register( c0  ) ;
float4 PixelWeight[4]  : register( c1  ) ;
float2 PixelOffset[16] : register( c5  ) ;
sampler SrcTexture     : register( s0  ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 PS_8pixel(VS_OUTPUT In) : COLOR
{   
    float4 Color;
	
    Color  = PixelWeight[0].x *   tex2D( SrcTexture, In.TexCoords0 );
    Color += PixelWeight[0].y * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  1 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  1 ] ) );
    Color += PixelWeight[0].z * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  2 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  2 ] ) );
    Color += PixelWeight[0].w * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  3 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  3 ] ) );
	
    return Color;
}

float4 PS_16pixel(VS_OUTPUT In) : COLOR
{   
    float4 Color;
	
    Color  = PixelWeight[0].x *   tex2D( SrcTexture, In.TexCoords0 );
    Color += PixelWeight[0].y * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  1 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  1 ] ) );
    Color += PixelWeight[0].z * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  2 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  2 ] ) );
    Color += PixelWeight[0].w * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  3 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  3 ] ) );
    Color += PixelWeight[1].x * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  4 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  4 ] ) );
    Color += PixelWeight[1].y * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  5 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  5 ] ) );
    Color += PixelWeight[1].z * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  6 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  6 ] ) );
    Color += PixelWeight[1].w * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  7 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  7 ] ) );
	
    return Color;
}

float4 PS_32pixel(VS_OUTPUT In) : COLOR
{   
    float4 Color;
	
    Color  = PixelWeight[0].x *   tex2D( SrcTexture, In.TexCoords0 );
    Color += PixelWeight[0].y * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  1 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  1 ] ) );
    Color += PixelWeight[0].z * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  2 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  2 ] ) );
    Color += PixelWeight[0].w * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  3 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  3 ] ) );
    Color += PixelWeight[1].x * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  4 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  4 ] ) );
    Color += PixelWeight[1].y * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  5 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  5 ] ) );
    Color += PixelWeight[1].z * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  6 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  6 ] ) );
    Color += PixelWeight[1].w * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  7 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  7 ] ) );
    Color += PixelWeight[2].x * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  8 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  8 ] ) );
    Color += PixelWeight[2].y * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[  9 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[  9 ] ) );
    Color += PixelWeight[2].z * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[ 10 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[ 10 ] ) );
    Color += PixelWeight[2].w * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[ 11 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[ 11 ] ) );
    Color += PixelWeight[3].x * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[ 12 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[ 12 ] ) );
    Color += PixelWeight[3].y * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[ 13 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[ 13 ] ) );
    Color += PixelWeight[3].z * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[ 14 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[ 14 ] ) );
    Color += PixelWeight[3].w * ( tex2D( SrcTexture, In.TexCoords0 + PixelOffset[ 15 ] ) +  tex2D( SrcTexture, In.TexCoords0 - PixelOffset[ 15 ] ) );
	
    return Color;
}


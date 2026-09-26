float2 PixelOffset[16] : register( c0 ) ;
sampler SrcTexture     : register( s0 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 Down_2_PS(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color = tex2D( SrcTexture, In.TexCoords0 + PixelOffset[0] ) ;

    return Color;
}


float4 Down_4_PS(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color  = tex2D( SrcTexture, In.TexCoords0 + PixelOffset[0] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[1] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[2] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[3] ) ;

    return Color / 4;
}


float4 Down_8_PS(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color  = tex2D( SrcTexture, In.TexCoords0 + PixelOffset[0] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[1] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[2] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[3] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[4] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[5] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[6] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[7] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[8] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[9] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[10] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[11] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[12] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[13] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[14] ) ;
    Color += tex2D( SrcTexture, In.TexCoords0 + PixelOffset[15] ) ;

    return Color / 16;
}


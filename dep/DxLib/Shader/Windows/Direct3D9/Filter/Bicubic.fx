// 今給黎 隆様の「DirectX 9.0:双３次補間サンプリング」のコードを元にしています

float2 TextureSize     : register( c0  ) ;
float2 X2PixelU        : register( c1  ) ;
sampler SrcTexture     : register( s0  ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
	float2 TexCoords2      : TEXCOORD2 ;
	float2 TexCoords3      : TEXCOORD3 ;
	float2 TexCoords4      : TEXCOORD4 ;
	float2 TexCoords5      : TEXCOORD5 ;
	float2 TexCoords6      : TEXCOORD6 ;
	float2 TexCoords7      : TEXCOORD7 ;
} ;

float4 BicubicScale_PS(VS_OUTPUT In) : COLOR
{   
#if 0
	// Catmull-Rom patch
    const float4 w3 = float4( -0.5,  1.5, -1.5,  0.5 ) ;
    const float4 w2 = float4(  1.0, -2.5,  2.0, -0.5 ) ;
    const float4 w1 = float4( -0.5,  0.0,  0.5,  0.0 ) ;
    const float4 w0 = float4(  0.0,  1.0,  0.0,  0.0 ) ;
#else
    const float4 w3 = float4( -1,  1, -1,  1 ) ;
    const float4 w2 = float4(  2, -2,  1, -1 ) ;
    const float4 w1 = float4( -1,  0,  1,  0 ) ;
    const float4 w0 = float4(  0,  1,  0,  0 ) ;
#endif
	
	float2 fUV  = frac( In.TexCoords5 * TextureSize ) ;
	float2 fUV2 = fUV * fUV ;
	float2 fUV3 = fUV * fUV2 ;
	float4 u, v ;
	u = fUV3.x * w3 + fUV2.x * w2 + fUV.x * w1 + w0 ;
	v = fUV3.y * w3 + fUV2.y * w2 + fUV.y * w1 + w0 ;
	
	float4 col00 = tex2D( SrcTexture, In.TexCoords0 ) ;
	float4 col01 = tex2D( SrcTexture, In.TexCoords1 ) ;
	float4 col02 = tex2D( SrcTexture, In.TexCoords2 ) ;
	float4 col03 = tex2D( SrcTexture, In.TexCoords3 ) ;

	float4 col10 = tex2D( SrcTexture, In.TexCoords4 ) ;
	float4 col11 = tex2D( SrcTexture, In.TexCoords5 ) ;
	float4 col12 = tex2D( SrcTexture, In.TexCoords6 ) ;
	float4 col13 = tex2D( SrcTexture, In.TexCoords7 ) ;

	float4 col20 = tex2D( SrcTexture, In.TexCoords0 + X2PixelU ) ;
	float4 col21 = tex2D( SrcTexture, In.TexCoords1 + X2PixelU ) ;
	float4 col22 = tex2D( SrcTexture, In.TexCoords2 + X2PixelU ) ;
	float4 col23 = tex2D( SrcTexture, In.TexCoords3 + X2PixelU ) ;

	float4 col30 = tex2D( SrcTexture, In.TexCoords4 + X2PixelU ) ;
	float4 col31 = tex2D( SrcTexture, In.TexCoords5 + X2PixelU ) ;
	float4 col32 = tex2D( SrcTexture, In.TexCoords6 + X2PixelU ) ;
	float4 col33 = tex2D( SrcTexture, In.TexCoords7 + X2PixelU ) ;

	float4 col0 = u.x * col00 + u.y * col10 + u.z * col20 + u.w * col30 ;
	float4 col1 = u.x * col01 + u.y * col11 + u.z * col21 + u.w * col31 ;
	float4 col2 = u.x * col02 + u.y * col12 + u.z * col22 + u.w * col32 ;
	float4 col3 = u.x * col03 + u.y * col13 + u.z * col23 + u.w * col33 ;
	
    return v.x * col0 + v.y * col1 + v.z * col2 + v.w * col3 ;
}


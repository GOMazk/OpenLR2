// 音宮志久様の「PixelShaderを使った画像補間 (3) Lanczosフィルタによる補間」のコードを元にしています

float2 TextureSize     : register( c0  ) ;
float2 X1PixelU        : register( c1  ) ;
sampler SrcTexture     : register( s0  ) ;

static float  M_PI = 3.14159265358979324 ;
static float2 TexDiff = { 1.0, 1.0 } ;

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

float4 Lanczos3Scale_PS(VS_OUTPUT In) : COLOR
{
	float4 clr ;
	float2 w[ 6 ] ;
	float2 p[ 6 ] ;
	float2 t0, t1 ;
	int i ;

	p[ 2 ] = -frac( In.TexCoords2 * TextureSize ) ;
	p[ 3 ] = p[ 2 ] + TexDiff ;
	p[ 1 ] = p[ 2 ] - TexDiff ;
	p[ 4 ] = p[ 3 ] + TexDiff ;
	p[ 0 ] = p[ 1 ] - TexDiff ;
	p[ 5 ] = p[ 4 ] + TexDiff ;

	t0 = p[ 0 ] * M_PI ;
	t1 = t0 / 3.0 ;
	w[ 0 ].x = ( t0.x != 0.0 ? ( ( sin( t0.x ) / t0.x ) * ( sin( t1.x ) / t1.x ) ) : 1.0 ) ;
	w[ 0 ].y = ( t0.y != 0.0 ? ( ( sin( t0.y ) / t0.y ) * ( sin( t1.y ) / t1.y ) ) : 1.0 ) ;
//	w[ 0 ] = ( t0 != 0.0 ? ( ( sin( t0 ) / t0 ) * ( sin( t1 ) / t1 ) ) : 1.0 ) ;
	p[ 0 ] = X1PixelU * ( 0.0f - 2.0 ) ;

	t0 = p[ 1 ] * M_PI ;
	t1 = t0 / 3.0 ;
	w[ 1 ].x = ( t0.x != 0.0 ? ( ( sin( t0.x ) / t0.x ) * ( sin( t1.x ) / t1.x ) ) : 1.0 ) ;
	w[ 1 ].y = ( t0.y != 0.0 ? ( ( sin( t0.y ) / t0.y ) * ( sin( t1.y ) / t1.y ) ) : 1.0 ) ;
//	w[ 1 ] = ( t0 != 0.0 ? ( ( sin( t0 ) / t0 ) * ( sin( t1 ) / t1 ) ) : 1.0 ) ;
	p[ 1 ] = X1PixelU * ( 1.0f - 2.0 ) ;

	t0 = p[ 2 ] * M_PI ;
	t1 = t0 / 3.0 ;
	w[ 2 ].x = ( t0.x != 0.0 ? ( ( sin( t0.x ) / t0.x ) * ( sin( t1.x ) / t1.x ) ) : 1.0 ) ;
	w[ 2 ].y = ( t0.y != 0.0 ? ( ( sin( t0.y ) / t0.y ) * ( sin( t1.y ) / t1.y ) ) : 1.0 ) ;
//	w[ 2 ] = ( t0 != 0.0 ? ( ( sin( t0 ) / t0 ) * ( sin( t1 ) / t1 ) ) : 1.0 ) ;
	p[ 2 ] = X1PixelU * ( 2.0f - 2.0 ) ;

	t0 = p[ 3 ] * M_PI ;
	t1 = t0 / 3.0 ;
	w[ 3 ].x = ( t0.x != 0.0 ? ( ( sin( t0.x ) / t0.x ) * ( sin( t1.x ) / t1.x ) ) : 1.0 ) ;
	w[ 3 ].y = ( t0.y != 0.0 ? ( ( sin( t0.y ) / t0.y ) * ( sin( t1.y ) / t1.y ) ) : 1.0 ) ;
//	w[ 3 ] = ( t0 != 0.0 ? ( ( sin( t0 ) / t0 ) * ( sin( t1 ) / t1 ) ) : 1.0 ) ;
	p[ 3 ] = X1PixelU * ( 3.0f - 2.0 ) ;

	t0 = p[ 4 ] * M_PI ;
	t1 = t0 / 3.0 ;
	w[ 4 ].x = ( t0.x != 0.0 ? ( ( sin( t0.x ) / t0.x ) * ( sin( t1.x ) / t1.x ) ) : 1.0 ) ;
	w[ 4 ].y = ( t0.y != 0.0 ? ( ( sin( t0.y ) / t0.y ) * ( sin( t1.y ) / t1.y ) ) : 1.0 ) ;
//	w[ 4 ] = ( t0 != 0.0 ? ( ( sin( t0 ) / t0 ) * ( sin( t1 ) / t1 ) ) : 1.0 ) ;
	p[ 4 ] = X1PixelU * ( 4.0f - 2.0 ) ;

	t0 = p[ 5 ] * M_PI ;
	t1 = t0 / 3.0 ;
	w[ 5 ].x = ( t0.x != 0.0 ? ( ( sin( t0.x ) / t0.x ) * ( sin( t1.x ) / t1.x ) ) : 1.0 ) ;
	w[ 5 ].y = ( t0.y != 0.0 ? ( ( sin( t0.y ) / t0.y ) * ( sin( t1.y ) / t1.y ) ) : 1.0 ) ;
//	w[ 5 ] = ( t0 != 0.0 ? ( ( sin( t0 ) / t0 ) * ( sin( t1 ) / t1 ) ) : 1.0 ) ;
	p[ 5 ] = X1PixelU * ( 5.0f - 2.0 ) ;

	return saturate(
				(	tex2D( SrcTexture, In.TexCoords0 + p[ 0 ] ) * w[ 0 ].x +
					tex2D( SrcTexture, In.TexCoords0 + p[ 1 ] ) * w[ 1 ].x +
					tex2D( SrcTexture, In.TexCoords0          ) * w[ 2 ].x +
					tex2D( SrcTexture, In.TexCoords0 + p[ 3 ] ) * w[ 3 ].x +
					tex2D( SrcTexture, In.TexCoords0 + p[ 4 ] ) * w[ 4 ].x +
					tex2D( SrcTexture, In.TexCoords0 + p[ 5 ] ) * w[ 5 ].x ) * w[ 0 ].y +
	
				(	tex2D( SrcTexture, In.TexCoords1 + p[ 0 ] ) * w[ 0 ].x +
					tex2D( SrcTexture, In.TexCoords1 + p[ 1 ] ) * w[ 1 ].x +
					tex2D( SrcTexture, In.TexCoords1          ) * w[ 2 ].x +
					tex2D( SrcTexture, In.TexCoords1 + p[ 3 ] ) * w[ 3 ].x +
					tex2D( SrcTexture, In.TexCoords1 + p[ 4 ] ) * w[ 4 ].x +
					tex2D( SrcTexture, In.TexCoords1 + p[ 5 ] ) * w[ 5 ].x ) * w[ 1 ].y +
	
				(	tex2D( SrcTexture, In.TexCoords2 + p[ 0 ] ) * w[ 0 ].x +
					tex2D( SrcTexture, In.TexCoords2 + p[ 1 ] ) * w[ 1 ].x +
					tex2D( SrcTexture, In.TexCoords2          ) * w[ 2 ].x +
					tex2D( SrcTexture, In.TexCoords2 + p[ 3 ] ) * w[ 3 ].x +
					tex2D( SrcTexture, In.TexCoords2 + p[ 4 ] ) * w[ 4 ].x +
					tex2D( SrcTexture, In.TexCoords2 + p[ 5 ] ) * w[ 5 ].x ) * w[ 2 ].y +
	
				(	tex2D( SrcTexture, In.TexCoords3 + p[ 0 ] ) * w[ 0 ].x +
					tex2D( SrcTexture, In.TexCoords3 + p[ 1 ] ) * w[ 1 ].x +
					tex2D( SrcTexture, In.TexCoords3          ) * w[ 2 ].x +
					tex2D( SrcTexture, In.TexCoords3 + p[ 3 ] ) * w[ 3 ].x +
					tex2D( SrcTexture, In.TexCoords3 + p[ 4 ] ) * w[ 4 ].x +
					tex2D( SrcTexture, In.TexCoords3 + p[ 5 ] ) * w[ 5 ].x ) * w[ 3 ].y +
		
				(	tex2D( SrcTexture, In.TexCoords4 + p[ 0 ] ) * w[ 0 ].x +
					tex2D( SrcTexture, In.TexCoords4 + p[ 1 ] ) * w[ 1 ].x +
					tex2D( SrcTexture, In.TexCoords4          ) * w[ 2 ].x +
					tex2D( SrcTexture, In.TexCoords4 + p[ 3 ] ) * w[ 3 ].x +
					tex2D( SrcTexture, In.TexCoords4 + p[ 4 ] ) * w[ 4 ].x +
					tex2D( SrcTexture, In.TexCoords4 + p[ 5 ] ) * w[ 5 ].x ) * w[ 4 ].y +
		
				(	tex2D( SrcTexture, In.TexCoords5 + p[ 0 ] ) * w[ 0 ].x +
					tex2D( SrcTexture, In.TexCoords5 + p[ 1 ] ) * w[ 1 ].x +
					tex2D( SrcTexture, In.TexCoords5          ) * w[ 2 ].x +
					tex2D( SrcTexture, In.TexCoords5 + p[ 3 ] ) * w[ 3 ].x +
					tex2D( SrcTexture, In.TexCoords5 + p[ 4 ] ) * w[ 4 ].x +
					tex2D( SrcTexture, In.TexCoords5 + p[ 5 ] ) * w[ 5 ].x ) * w[ 5 ].y
	) ;
}


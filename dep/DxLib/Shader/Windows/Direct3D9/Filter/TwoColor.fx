float4 RGBToY_Cmp : register( c0 ) ;
float4 LowColor   : register( c1 ) ;
float4 HighColor  : register( c2 ) ;

sampler SrcTexture      : register( s0 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 TwoColor_PS(VS_OUTPUT In) : COLOR
{
	float4 Color;

	Color = tex2D( SrcTexture, In.TexCoords0 ) ;

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

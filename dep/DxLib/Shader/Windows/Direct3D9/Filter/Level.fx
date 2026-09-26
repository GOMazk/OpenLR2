float4 Min_LenRev_AfterMin_AfterMax : register( c0 ) ;

sampler SrcTexture   : register( s0 ) ;
sampler GammaTexture : register( s1 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 Level_PS(VS_OUTPUT In) : COLOR
{
	float4 Color;
	float3 Rgb;
	float Ratio;

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

	Rgb.rgb = saturate( ( Color.rgb - Min_LenRev_AfterMin_AfterMax.xxx ) * Min_LenRev_AfterMin_AfterMax.y );

	Ratio = tex2D( GammaTexture, float2( Rgb.r, 0.0 ) ).r ;
	Color.r = lerp( Min_LenRev_AfterMin_AfterMax.z, Min_LenRev_AfterMin_AfterMax.w, Ratio );

	Ratio = tex2D( GammaTexture, float2( Rgb.g, 0.0 ) ).r ;
	Color.g = lerp( Min_LenRev_AfterMin_AfterMax.z, Min_LenRev_AfterMin_AfterMax.w, Ratio );

	Ratio = tex2D( GammaTexture, float2( Rgb.b, 0.0 ) ).r ;
	Color.b = lerp( Min_LenRev_AfterMin_AfterMax.z, Min_LenRev_AfterMin_AfterMax.w, Ratio );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}


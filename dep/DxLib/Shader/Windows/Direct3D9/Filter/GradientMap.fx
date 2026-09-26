float3 RGBToY : register( c0 ) ;

sampler SrcTexture         : register( s0 ) ;
sampler GradientMapTexture : register( s1 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 GradientMap_PS(VS_OUTPUT In) : COLOR
{
	float4 DestColor ;
	float4 SrcColor ;

	SrcColor = tex2D( SrcTexture, In.TexCoords0 ) ;

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
	
	DestColor = tex2D( GradientMapTexture, float2( dot( SrcColor.rgb, RGBToY.rgb ), 0 ) ) ;

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

float4 GradientMapRev_PS(VS_OUTPUT In) : COLOR
{
	float4 DestColor ;
	float4 SrcColor ;

	SrcColor = tex2D( SrcTexture, In.TexCoords0 ) ;

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
	
	DestColor = tex2D( GradientMapTexture, float2( 1.0f - dot( SrcColor.rgb, RGBToY.rgb ), 0 ) ) ;

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

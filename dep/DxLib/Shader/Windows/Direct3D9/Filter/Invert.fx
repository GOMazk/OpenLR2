sampler SrcTexture   : register( s0 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 Invert_PS(VS_OUTPUT In) : COLOR
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

	Color.rgb = float3(1,1,1) - Color.rgb;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

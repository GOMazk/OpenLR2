sampler SrcTexture   : register( s0 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 PremultipliedAlpha_PS(VS_OUTPUT In) : COLOR
{
	float4 Color;

	Color = tex2D( SrcTexture, In.TexCoords0 ) ;

	Color.rgb = Color.rgb * Color.a;
	
	return Color;
}

float4 InterpolatedAlpha_PS(VS_OUTPUT In) : COLOR
{
	float4 Color;

	Color = tex2D( SrcTexture, In.TexCoords0 ) ;

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
	
	return Color;
}

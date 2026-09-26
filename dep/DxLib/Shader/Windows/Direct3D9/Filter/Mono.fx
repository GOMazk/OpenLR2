float3 RGBToY     : register( c0 ) ;
float3 YCbCrToRGB : register( c1 ) ;

sampler SrcTexture      : register( s0 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 Mono_PS(VS_OUTPUT In) : COLOR
{
	float4 Color;
	float3 Rgb;

	Color = tex2D( SrcTexture, In.TexCoords0 ) ;
	Rgb = Color.rgb;

	Rgb.rgb = dot( Rgb, RGBToY );
	Color.rgb = Rgb + YCbCrToRGB;

	return Color;
}

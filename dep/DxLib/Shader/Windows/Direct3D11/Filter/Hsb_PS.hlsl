#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 Param ;
} ;

SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;

SamplerState g_BlendSampler     : register( s1 ) ;
Texture2D    g_BlendTexture     : register( t1 ) ;

float4 RGB_To_HSB_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;
	float3 Hsb;
	float3 Rgb;
	float Max;
	float Min;
	float Delta;
	float3 One;

	One  = 1.0f;

	Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	
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
	
	Rgb = Color.rgb;
	
	if( Param.z > 0.0 )
	{
		Rgb += ( One - Rgb ) * Param.z;
	}
	else
	{
		Rgb *= 1.0 + Param.z;
	}
	Rgb = saturate( Rgb );

	Max = max( Rgb.r, max( Rgb.g, Rgb.b ) );
	Min = min( Rgb.r, min( Rgb.g, Rgb.b ) );
	Delta = Max - Min;

	Hsb.z = ( Max + Min ) / 2.0;
	if( Max == Min )
	{
		Hsb.x = 0.0f;
		Hsb.y = 0.0f;
	}
	else
	{
		float3 tRGB;

		if( Hsb.z > 0.5 )
		{
			Hsb.y = 2 - Max - Min; 
		}
		else
		{
			Hsb.y = Max + Min;
		}
		Hsb.y = Delta / Hsb.y;
		
		tRGB = ( float3( Max, Max, Max ) - Rgb ) / Delta;
		if( Rgb.r == Max )
		{
			Hsb.x = tRGB.b - tRGB.g;
		}
		else
		if( Rgb.g == Max )
		{
			Hsb.x = 2.0 + tRGB.r - tRGB.b;
		}
		else
		{
			Hsb.x = 4.0 + tRGB.g - tRGB.r;
		}
		if( Hsb.x < 0.0 )
		{
			Hsb.x += 6.0;
		}
		Hsb.x /= 6.0;
	}

	Color.r = Hsb.x;
	Color.g = Hsb.y;
	Color.b = Hsb.z;

	return Color;
}

float4 HSB_HLOCK_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;
	float3 Hsb;
	float3 Rgb;
	float Max;
	float Min;
	float Delta;
	float3 Half;
	float3 One;

	Half = 0.5f;
	One  = 1.0f;

	Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;

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
	
	Rgb = Color.rgb;
	
	if( Param.z > 0.0 )
	{
		Rgb += ( One - Rgb ) * Param.z;
	}
	else
	{
		Rgb *= 1.0 + Param.z;
	}
	Rgb = saturate( Rgb );

	Max = max( Rgb.r, max( Rgb.g, Rgb.b ) );
	Min = min( Rgb.r, min( Rgb.g, Rgb.b ) );
	Delta = Max - Min;

	Hsb.z = ( Max + Min ) / 2.0;
	if( Max == Min )
	{
		Hsb.y = 0.0f;
	}
	else
	{
		float3 tRGB;

		if( Hsb.z > 0.5 )
		{
			Hsb.y = 2 - Max - Min; 
		}
		else
		{
			Hsb.y = Max + Min;
		}
		Hsb.y = Delta / Hsb.y;
	}
	
	Hsb.y = saturate( Hsb.y * ( 1.0 + Param.y ) );

	if( Param.x < 1.0f )
	{
		Rgb.r = 1.0f ;
		Rgb.g = Param.x ;
		Rgb.b = 0.0f ;
	}
	else
	if( Param.x < 2.0f )
	{
		Rgb.r = 1.0f - ( Param.x - 1.0f ) ;
		Rgb.g = 1.0f ;
		Rgb.b = 0.0f ;
	}
	else
	if( Param.x < 3.0f )
	{
		Rgb.r = 0.0f ;
		Rgb.g = 1.0f ;
		Rgb.b = Param.x - 2.0f ;
	}
	else
	if( Param.x < 4.0f )
	{
		Rgb.r = 0.0f ;
		Rgb.g = 1.0f - ( Param.x - 3.0f ) ;
		Rgb.b = 1.0f ;
	}
	else
	if( Param.x < 5.0f )
	{
		Rgb.r = Param.x - 4.0f ;
		Rgb.g = 0.0f ;
		Rgb.b = 1.0f ;
	}
	else
	{
		Rgb.r = 1.0f ;
		Rgb.g = 0.0f ;
		Rgb.b = 1.0f - ( Param.x - 5.0f ) ;
	}

	Rgb += ( Half - Rgb ) * ( 1.0f - Hsb.y );

	if( Hsb.z <= 0.5f )
	{
		Rgb *= Hsb.z * 2.0f;
	}
	else
	{
		Rgb += ( One - Rgb ) * ( ( Hsb.z - 0.5f ) * 2.0f );
	}

	Color.r = Rgb.r;
	Color.g = Rgb.g;
	Color.b = Rgb.b;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 HSB_To_RGB_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;
	float3 Hsb;
	float3 Rgb;
	float Max;
	float Min;
	float Delta;
	float3 Half;
	float3 One;

	Half = 0.5f;
	One  = 1.0f;

	Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	Hsb = Color.rgb;

	Hsb.x *= 6.0f;
	Hsb.x += Param.x;
	if( Hsb.x < 0.0f )
	{
		Hsb.x += 6.0;
	}
	else
	if( Hsb.x >= 6.0f )
	{
		Hsb.x -= 6.0;
	}

	Hsb.y = saturate( Hsb.y * ( 1.0 + Param.y ) );

	if( Hsb.x < 1.0f )
	{
		Rgb.r = 1.0f ;
		Rgb.g = Hsb.x ;
		Rgb.b = 0.0f ;
	}
	else
	if( Hsb.x < 2.0f )
	{
		Rgb.r = 1.0f - ( Hsb.x - 1.0f ) ;
		Rgb.g = 1.0f ;
		Rgb.b = 0.0f ;
	}
	else
	if( Hsb.x < 3.0f )
	{
		Rgb.r = 0.0f ;
		Rgb.g = 1.0f ;
		Rgb.b = Hsb.x - 2.0f ;
	}
	else
	if( Hsb.x < 4.0f )
	{
		Rgb.r = 0.0f ;
		Rgb.g = 1.0f - ( Hsb.x - 3.0f ) ;
		Rgb.b = 1.0f ;
	}
	else
	if( Hsb.x < 5.0f )
	{
		Rgb.r = Hsb.x - 4.0f ;
		Rgb.g = 0.0f ;
		Rgb.b = 1.0f ;
	}
	else
	{
		Rgb.r = 1.0f ;
		Rgb.g = 0.0f ;
		Rgb.b = 1.0f - ( Hsb.x - 5.0f ) ;
	}

	Rgb += ( Half - Rgb ) * ( 1.0f - Hsb.y );

	if( Hsb.z <= 0.5f )
	{
		Rgb *= Hsb.z * 2.0f;
	}
	else
	{
		Rgb += ( One - Rgb ) * ( ( Hsb.z - 0.5f ) * 2.0f );
	}
	
	Color.r = Rgb.r;
	Color.g = Rgb.g;
	Color.b = Rgb.b;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}






float4 HSB_Base_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;
	float3 Hsb;
	float3 Rgb;
	float Max;
	float Min;
	float Delta;
	float3 Half;
	float3 One;

	Half = 0.5f;
	One  = 1.0f;

	Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	
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
	
	Rgb = Color.rgb;
	
	if( Param.z > 0.0 )
	{
		Rgb += ( One - Rgb ) * Param.z;
	}
	else
	{
		Rgb *= 1.0 + Param.z;
	}
	Rgb = saturate( Rgb );

	Max = max( Rgb.r, max( Rgb.g, Rgb.b ) );
	Min = min( Rgb.r, min( Rgb.g, Rgb.b ) );
	Delta = Max - Min;

	Hsb.z = ( Max + Min ) / 2.0;
	if( Max == Min )
	{
		Hsb.x = 0.0f;
		Hsb.y = 0.0f;
	}
	else
	{
		float3 tRGB;

		if( Hsb.z > 0.5 )
		{
			Hsb.y = 2 - Max - Min; 
		}
		else
		{
			Hsb.y = Max + Min;
		}
		Hsb.y = Delta / Hsb.y;
		
		tRGB = ( float3( Max, Max, Max ) - Rgb ) / Delta;
		if( Rgb.r == Max )
		{
			Hsb.x = tRGB.b - tRGB.g;
		}
		else
		if( Rgb.g == Max )
		{
			Hsb.x = 2.0 + tRGB.r - tRGB.b;
		}
		else
		{
			Hsb.x = 4.0 + tRGB.g - tRGB.r;
		}
		if( Hsb.x < 0.0 )
		{
			Hsb.x += 6.0;
		}
	}

	Hsb.x += Param.x;
	if( Hsb.x < 0.0f )
	{
		Hsb.x += 6.0;
	}
	else
	if( Hsb.x >= 6.0f )
	{
		Hsb.x -= 6.0;
	}

	Hsb.y = saturate( Hsb.y * ( 1.0 + Param.y ) );

	if( Hsb.x < 1.0f )
	{
		Rgb.r = 1.0f ;
		Rgb.g = Hsb.x ;
		Rgb.b = 0.0f ;
	}
	else
	if( Hsb.x < 2.0f )
	{
		Rgb.r = 1.0f - ( Hsb.x - 1.0f ) ;
		Rgb.g = 1.0f ;
		Rgb.b = 0.0f ;
	}
	else
	if( Hsb.x < 3.0f )
	{
		Rgb.r = 0.0f ;
		Rgb.g = 1.0f ;
		Rgb.b = Hsb.x - 2.0f ;
	}
	else
	if( Hsb.x < 4.0f )
	{
		Rgb.r = 0.0f ;
		Rgb.g = 1.0f - ( Hsb.x - 3.0f ) ;
		Rgb.b = 1.0f ;
	}
	else
	if( Hsb.x < 5.0f )
	{
		Rgb.r = Hsb.x - 4.0f ;
		Rgb.g = 0.0f ;
		Rgb.b = 1.0f ;
	}
	else
	{
		Rgb.r = 1.0f ;
		Rgb.g = 0.0f ;
		Rgb.b = 1.0f - ( Hsb.x - 5.0f ) ;
	}

	Rgb += ( Half - Rgb ) * ( 1.0f - Hsb.y );

	if( Hsb.z <= 0.5f )
	{
		Rgb *= Hsb.z * 2.0f;
	}
	else
	{
		Rgb += ( One - Rgb ) * ( ( Hsb.z - 0.5f ) * 2.0f );
	}
	
	Color.rgb = saturate( Rgb.rgb );
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}






float4 HSB_Blend_Hue_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;
	float3 Hsb;
	float3 Rgb;
	float3 DestRgb;
	float2 Max;
	float2 Min;
	float2 Delta;
	float3 Half;
	float3 One;

	Half = 0.5f;
	One  = 1.0f;

	Color      = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 ) ;
	
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
	
	if( BlendColor.a < 0.001f )
	{
		BlendColor.r = 0.0f ;
		BlendColor.g = 0.0f ;
		BlendColor.b = 0.0f ;
	}
	else
	{
		BlendColor.rgb = BlendColor.rgb / BlendColor.a;
	}
#endif
	
	Rgb = Color.rgb;
	
	Max.x = max( Rgb.r, max( Rgb.g, Rgb.b ) );
	Min.x = min( Rgb.r, min( Rgb.g, Rgb.b ) );
	Delta.x = Max.x - Min.x;

	Max.y = max( BlendColor.r, max( BlendColor.g, BlendColor.b ) );
	Min.y = min( BlendColor.r, min( BlendColor.g, BlendColor.b ) );
	Delta.y = Max.y - Min.y;

	Hsb.z = ( Max.x + Min.x ) / 2.0;
	if( Max.x != Min.x )
	{
		if( Hsb.z > 0.5 )
		{
			Hsb.y = 2 - Max.x - Min.x;
		}
		else
		{
			Hsb.y = Max.x + Min.x;
		}
		Hsb.y = Delta.x / Hsb.y;
	}
	
	if( Max.y != Min.y )
	{
		float3 tRGB;

		tRGB = ( Max.yyy - BlendColor.rgb ) / Delta.y;
		if( BlendColor.r == Max.y )
		{
			Hsb.x = tRGB.b - tRGB.g;
		}
		else
		if( BlendColor.g == Max.y )
		{
			Hsb.x = 2.0 + tRGB.r - tRGB.b;
		}
		else
		{
			Hsb.x = 4.0 + tRGB.g - tRGB.r;
		}
		if( Hsb.x < 0.0 )
		{
			Hsb.x += 6.0;
		}
	}

	if( Hsb.x < 1.0f )
	{
		DestRgb.r = 1.0f ;
		DestRgb.g = Hsb.x ;
	}
	else
	if( Hsb.x < 2.0f )
	{
		DestRgb.r = 1.0f - ( Hsb.x - 1.0f ) ;
		DestRgb.g = 1.0f ;
	}
	else
	if( Hsb.x < 3.0f )
	{
		DestRgb.g = 1.0f ;
		DestRgb.b = Hsb.x - 2.0f ;
	}
	else
	if( Hsb.x < 4.0f )
	{
		DestRgb.g = 1.0f - ( Hsb.x - 3.0f ) ;
		DestRgb.b = 1.0f ;
	}
	else
	if( Hsb.x < 5.0f )
	{
		DestRgb.r = Hsb.x - 4.0f ;
		DestRgb.b = 1.0f ;
	}
	else
	{
		DestRgb.r = 1.0f ;
		DestRgb.b = 1.0f - ( Hsb.x - 5.0f ) ;
	}

	DestRgb.rgb += ( Half.rgb - DestRgb.rgb ) * ( 1.0f - Hsb.y );

	if( Hsb.z <= 0.5f )
	{
		DestRgb.rgb *= Hsb.z * 2.0f;
	}
	else
	{
		DestRgb.rgb += ( One.rgb - DestRgb.rgb ) * ( ( Hsb.z - 0.5f ) * 2.0f );
	}
	
	Color.rgb = lerp( Color.rgb, DestRgb.rgb, Param.x/*BlendRatio*/ * BlendColor.a );
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 HSB_Blend_Saturation_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;
	float3 Hsb;
	float3 Rgb;
	float3 DestRgb;
	float2 Max;
	float2 Min;
	float2 Delta;
	float3 Half;
	float3 One;

	Half = 0.5f;
	One  = 1.0f;

	Color      = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 ) ;

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
	
	if( BlendColor.a < 0.001f )
	{
		BlendColor.r = 0.0f ;
		BlendColor.g = 0.0f ;
		BlendColor.b = 0.0f ;
	}
	else
	{
		BlendColor.rgb = BlendColor.rgb / BlendColor.a;
	}
#endif
	
	Rgb = Color.rgb;
	
	Max.x = max( Rgb.r, max( Rgb.g, Rgb.b ) );
	Min.x = min( Rgb.r, min( Rgb.g, Rgb.b ) );
	Delta.x = Max.x - Min.x;

	Max.y = max( BlendColor.r, max( BlendColor.g, BlendColor.b ) );
	Min.y = min( BlendColor.r, min( BlendColor.g, BlendColor.b ) );
	Delta.y = Max.y - Min.y;

	Hsb.z = Max.x + Min.x;
	if( Max.x != Min.x )
	{
		float3 tRGB;

		tRGB = ( Max.xxx - Color.rgb ) / Delta.x;
		if( Color.r == Max.x )
		{
			Hsb.x = tRGB.b - tRGB.g;
		}
		else
		if( Color.g == Max.x )
		{
			Hsb.x = 2.0 + tRGB.r - tRGB.b;
		}
		else
		{
			Hsb.x = 4.0 + tRGB.g - tRGB.r;
		}
		if( Hsb.x < 0.0 )
		{
			Hsb.x += 6.0;
		}
	}
	
	if( Max.y != Min.y )
	{
		if( Max.y + Min.y > 1.0 )
		{
			Hsb.y = 2 - ( Max.y + Min.y );
		}
		else
		{
			Hsb.y = Max.y + Min.y;
		}
		Hsb.y = Delta.y / Hsb.y;
	}

	if( Hsb.x < 1.0f )
	{
		DestRgb.r = 1.0f ;
		DestRgb.g = Hsb.x ;
	}
	else
	if( Hsb.x < 2.0f )
	{
		DestRgb.r = 1.0f - ( Hsb.x - 1.0f ) ;
		DestRgb.g = 1.0f ;
	}
	else
	if( Hsb.x < 3.0f )
	{
		DestRgb.g = 1.0f ;
		DestRgb.b = Hsb.x - 2.0f ;
	}
	else
	if( Hsb.x < 4.0f )
	{
		DestRgb.g = 1.0f - ( Hsb.x - 3.0f ) ;
		DestRgb.b = 1.0f ;
	}
	else
	if( Hsb.x < 5.0f )
	{
		DestRgb.r = Hsb.x - 4.0f ;
		DestRgb.b = 1.0f ;
	}
	else
	{
		DestRgb.r = 1.0f ;
		DestRgb.b = 1.0f - ( Hsb.x - 5.0f ) ;
	}

	DestRgb.rgb += ( Half.rgb - DestRgb.rgb ) * ( 1.0f - Hsb.y );

	if( Hsb.z < 1.0f )
	{
		DestRgb.rgb *= Hsb.z;
	}
	else
	{
		DestRgb.rgb += ( One.rgb - DestRgb.rgb ) * ( Hsb.z - 1.0f );
	}
	
	Color.rgb = lerp( Color.rgb, DestRgb.rgb, Param.x/*BlendRatio*/ * BlendColor.a );
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

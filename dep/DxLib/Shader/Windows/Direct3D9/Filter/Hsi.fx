float4 Param : register( c0 ) ;
float3 Half : register( c1 ) ;
float3 One : register( c2 ) ;
sampler SrcTexture      : register( s0 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 RGB_To_HSI_PS(VS_OUTPUT In) : COLOR
{
	float4 Color;
	float3 Hsv;
	float3 Rgb;
	float Max;
	float Min;
	float Delta;

	Color = tex2D( SrcTexture, In.TexCoords0 ) ;
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

	Hsv.z = ( Max + Min ) / 2.0;
	if( Max == Min )
	{
		Hsv.x = 0.0f;
		Hsv.y = 0.0f;
	}
	else
	{
		float3 tRGB;

		if( Hsv.z > 0.5 )
		{
			Hsv.y = 2 - Max - Min; 
		}
		else
		{
			Hsv.y = Max + Min;
		}
		Hsv.y = Delta / Hsv.y;
		
		tRGB = ( float3( Max, Max, Max ) - Rgb ) / Delta;
		if( Rgb.r == Max )
		{
			Hsv.x = tRGB.b - tRGB.g;
		}
		else
		if( Rgb.g == Max )
		{
			Hsv.x = 2.0 + tRGB.r - tRGB.b;
		}
		else
		{
			Hsv.x = 4.0 + tRGB.g - tRGB.r;
		}
		if( Hsv.x < 0.0 )
		{
			Hsv.x += 6.0;
		}
		Hsv.x /= 6.0;
	}

	Color.r = Hsv.x;
	Color.g = Hsv.y;
	Color.b = Hsv.z;

	return Color;
}

float4 HSB_HLOCK_PS(VS_OUTPUT In) : COLOR
{
	float4 Color;
	float3 Hsv;
	float3 Rgb;
	float Max;
	float Min;
	float Delta;

	Color = tex2D( SrcTexture, In.TexCoords0 ) ;
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

	Hsv.z = ( Max + Min ) / 2.0;
	if( Max == Min )
	{
		Hsv.y = 0.0f;
	}
	else
	{
		float3 tRGB;

		if( Hsv.z > 0.5 )
		{
			Hsv.y = 2 - Max - Min; 
		}
		else
		{
			Hsv.y = Max + Min;
		}
		Hsv.y = Delta / Hsv.y;
	}
	
	Hsv.y = saturate( Hsv.y * ( 1.0 + Param.y ) );

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

	Rgb += ( Half - Rgb ) * ( 1.0f - Hsv.y );

	if( Hsv.z <= 0.5f )
	{
		Rgb *= Hsv.z * 2.0f;
	}
	else
	{
		Rgb += ( One - Rgb ) * ( ( Hsv.z - 0.5f ) * 2.0f );
	}

	Color.r = Rgb.r;
	Color.g = Rgb.g;
	Color.b = Rgb.b;

	return Color;
}

float4 HSI_To_RGB_PS(VS_OUTPUT In) : COLOR
{
	float4 Color;
	float3 Hsv;
	float3 Rgb;
	float Max;
	float Min;
	float Delta;

	Color = tex2D( SrcTexture, In.TexCoords0 ) ;
	Hsv = Color.rgb;

	Hsv.x *= 6.0f;
	Hsv.x += Param.x;
	if( Hsv.x < 0.0f )
	{
		Hsv.x += 6.0;
	}
	else
	if( Hsv.x >= 6.0f )
	{
		Hsv.x -= 6.0;
	}

	Hsv.y = saturate( Hsv.y * ( 1.0 + Param.y ) );

	if( Hsv.x < 1.0f )
	{
		Rgb.r = 1.0f ;
		Rgb.g = Hsv.x ;
		Rgb.b = 0.0f ;
	}
	else
	if( Hsv.x < 2.0f )
	{
		Rgb.r = 1.0f - ( Hsv.x - 1.0f ) ;
		Rgb.g = 1.0f ;
		Rgb.b = 0.0f ;
	}
	else
	if( Hsv.x < 3.0f )
	{
		Rgb.r = 0.0f ;
		Rgb.g = 1.0f ;
		Rgb.b = Hsv.x - 2.0f ;
	}
	else
	if( Hsv.x < 4.0f )
	{
		Rgb.r = 0.0f ;
		Rgb.g = 1.0f - ( Hsv.x - 3.0f ) ;
		Rgb.b = 1.0f ;
	}
	else
	if( Hsv.x < 5.0f )
	{
		Rgb.r = Hsv.x - 4.0f ;
		Rgb.g = 0.0f ;
		Rgb.b = 1.0f ;
	}
	else
	{
		Rgb.r = 1.0f ;
		Rgb.g = 0.0f ;
		Rgb.b = 1.0f - ( Hsv.x - 5.0f ) ;
	}

	Rgb += ( Half - Rgb ) * ( 1.0f - Hsv.y );

	if( Hsv.z <= 0.5f )
	{
		Rgb *= Hsv.z * 2.0f;
	}
	else
	{
		Rgb += ( One - Rgb ) * ( ( Hsv.z - 0.5f ) * 2.0f );
	}
	
	Color.r = Rgb.r;
	Color.g = Rgb.g;
	Color.b = Rgb.b;

	return Color;
}



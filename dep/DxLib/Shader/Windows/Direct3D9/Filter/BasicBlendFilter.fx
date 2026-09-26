float BlendRatio     : register( c0 ) ;
float4 One           : register( c1 ) ;
sampler SrcTexture   : register( s0 ) ;
sampler BlendTexture : register( s1 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
} ;

float4 Normal_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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
	
	Color.rgb = lerp( Color.rgb, BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Normal_AlphaCh_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;
	float Ta;
	float RSa;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	BlendColor.a *= BlendRatio;
	RSa = ( 1.0f - BlendColor.a );
	Ta = RSa * Color.a + BlendColor.a;
	Color = ( RSa * Color.a * Color + BlendColor.a * BlendColor ) / Ta;
	Color.a = Ta;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Add_AlphaCh_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;
	float Ta;
	float RSa;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	BlendColor.a *= BlendRatio;
	RSa = ( 1.0f - BlendColor.a );
	Ta = RSa * Color.a + BlendColor.a;
	Color = ( Color.a * Color + BlendColor.a * BlendColor ) / Ta;
	Color.a = Ta;
	Color = saturate( Color );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Multiple_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	Color.rgb = lerp( Color.rgb, Color.rgb * BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Difference_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	Color.rgb = lerp( Color.rgb, saturate( Color.rgb - BlendColor.rgb ), BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Add_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	Color.rgb = lerp( Color.rgb, saturate( Color.rgb + BlendColor.rgb ), BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Screen_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	Color.rgb = lerp( Color.rgb, One.rgb - (( One.rgb - Color.rgb ) * ( One.rgb - BlendColor.rgb )), BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Overlay_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	if( Color.r < 0.5 )
	{
		BlendColor.r = Color.r * BlendColor.r * 2.0;
	}
	else
	{
		BlendColor.r = 2.0 * ( Color.r + BlendColor.r - Color.r * BlendColor.r ) - 1.0;
	}

	if( Color.g < 0.5 )
	{
		BlendColor.g = Color.g * BlendColor.g * 2.0;
	}
	else
	{
		BlendColor.g = 2.0 * ( Color.g + BlendColor.g - Color.g * BlendColor.g ) - 1.0;
	}

	if( Color.b < 0.5 )
	{
		BlendColor.b = Color.b * BlendColor.b * 2.0;
	}
	else
	{
		BlendColor.b = 2.0 * ( Color.b + BlendColor.b - Color.b * BlendColor.b ) - 1.0;
	}
	BlendColor = saturate( BlendColor );

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Dodge_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	if( BlendColor.r <= 0.999 )
	{
		BlendColor.r = Color.r / ( 1.0 - BlendColor.r );
	}
	else
	{
		BlendColor.r = 1.0;
	}

	if( BlendColor.g <= 0.999 )
	{
		BlendColor.g = Color.g / ( 1.0 - BlendColor.g );
	}
	else
	{
		BlendColor.g = 1.0;
	}

	if( BlendColor.b <= 0.999 )
	{
		BlendColor.b = Color.b / ( 1.0 - BlendColor.b );
	}
	else
	{
		BlendColor.b = 1.0;
	}

	BlendColor = saturate( BlendColor );

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Burn_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	if( BlendColor.r > 0.0001 )
	{
		BlendColor.r = 1.0 - ( 1.0 - Color.r ) / BlendColor.r ;
	}

	if( BlendColor.g > 0.0001 )
	{
		BlendColor.g = 1.0 - ( 1.0 - Color.g ) / BlendColor.g ;
	}

	if( BlendColor.b > 0.0001 )
	{
		BlendColor.b = 1.0 - ( 1.0 - Color.b ) / BlendColor.b ;
	}

	BlendColor = saturate( BlendColor );

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Darken_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	if( Color.r < BlendColor.r )
	{
		BlendColor.r = Color.r ;
	}

	if( Color.g < BlendColor.g )
	{
		BlendColor.g = Color.g ;
	}

	if( Color.b < BlendColor.b )
	{
		BlendColor.b = Color.b ;
	}

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Lighten_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	if( Color.r > BlendColor.r )
	{
		BlendColor.r = Color.r ;
	}

	if( Color.g > BlendColor.g )
	{
		BlendColor.g = Color.g ;
	}

	if( Color.b > BlendColor.b )
	{
		BlendColor.b = Color.b ;
	}

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 SoftLight_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	if( BlendColor.r < 0.5 )
	{
		BlendColor.r = pow( Color.r, ( 1.0f - BlendColor.r ) * 2.0 ) ;
	}
	else
	{
		BlendColor.r = pow( Color.r, 0.5 / BlendColor.r );
	}

	if( BlendColor.g < 0.5 )
	{
		BlendColor.g = pow( Color.g, ( 1.0f - BlendColor.g ) * 2.0 ) ;
	}
	else
	{
		BlendColor.g = pow( Color.g, 0.5 / BlendColor.g );
	}

	if( BlendColor.b < 0.5 )
	{
		BlendColor.b = pow( Color.b, ( 1.0f - BlendColor.b ) * 2.0 ) ;
	}
	else
	{
		BlendColor.b = pow( Color.b, 0.5 / BlendColor.b );
	}

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 HardLight_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	if( BlendColor.r < 0.5 )
	{
		BlendColor.r = Color.r * BlendColor.r * 2.0;
	}
	else
	{
		BlendColor.r = 2.0 * ( Color.r + BlendColor.r - Color.r * BlendColor.r ) - 1.0;
	}

	if( BlendColor.g < 0.5 )
	{
		BlendColor.g = Color.g * BlendColor.g * 2.0;
	}
	else
	{
		BlendColor.g = 2.0 * ( Color.g + BlendColor.g - Color.g * BlendColor.g ) - 1.0;
	}

	if( BlendColor.b < 0.5 )
	{
		BlendColor.b = Color.b * BlendColor.b * 2.0;
	}
	else
	{
		BlendColor.b = 2.0 * ( Color.b + BlendColor.b - Color.b * BlendColor.b ) - 1.0;
	}

	BlendColor = saturate( BlendColor );

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Exclusion_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	Color.rgb = lerp( Color.rgb, Color.rgb + BlendColor.rgb - 2.0 * Color.rgb * BlendColor.rgb, BlendRatio * BlendColor.a );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 Multiple_AOnly_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	Color.a = lerp( Color.a, BlendColor.a * Color.a, BlendRatio );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}
/*
float4 PMA_Multiple_AOnly_PS( VS_OUTPUT In ) : Color
{
	float4 PremulColor;
	float4 LinearColor;
	float4 PremulBlendColor;
	float4 LinearBlendColor;
	float4 OutputLinearColor;
	float4 OutputPremulColor;

	PremulColor         = tex2D( SrcTexture,   In.TexCoords0 );
	PremulBlendColor    = tex2D( BlendTexture, In.TexCoords1 );

	LinearColor         = PremulColor      / PremulColor.a;
	LinearColor.a       = PremulColor.a;
	LinearBlendColor    = PremulBlendColor / PremulBlendColor.a;
	LinearBlendColor.a  = PremulBlendColor.a;
	
	OutputLinearColor   = LinearColor;
	OutputLinearColor.a = lerp( LinearColor.a, LinearBlendColor.a * LinearColor.a, BlendRatio );

	OutputPremulColor   = OutputLinearColor * OutputLinearColor.a;
	OutputPremulColor.a = OutputLinearColor.a;

	return OutputPremulColor;
}
*/

float4 Mask_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;
	float4 CalcColor;

	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );

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

	CalcColor.r = BlendColor.a * BlendColor.r + ( 1.0f - BlendColor.a ) * Color.r;
	CalcColor.g = BlendColor.a * BlendColor.g + ( 1.0f - BlendColor.a ) * Color.g;
	CalcColor.b = BlendColor.a * BlendColor.b + ( 1.0f - BlendColor.a ) * Color.b;
	CalcColor.a = Color.a;

	Color.rgb = lerp( Color.rgb, CalcColor.rgb, BlendRatio );

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}







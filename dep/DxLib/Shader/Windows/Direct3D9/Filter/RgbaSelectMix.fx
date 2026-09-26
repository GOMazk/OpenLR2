float BlendRatio     : register( c0 ) ;
float4 RGBASelect    : register( c1 ) ;
sampler SrcTexture   : register( s0 ) ;
sampler BlendTexture : register( s1 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
} ;


float4 Base_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;
	float4 DestColor;
	
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
	
	if( RGBASelect.r < 1.0 )
	{
		DestColor.r = Color.r;
	}
	else
	if( RGBASelect.r < 2.0 )
	{
		DestColor.r = Color.g;
	}
	else
	if( RGBASelect.r < 3.0 )
	{
		DestColor.r = Color.b;
	}
	else
	if( RGBASelect.r < 4.0 )
	{
		DestColor.r = Color.a;
	}
	else
	if( RGBASelect.r < 5.0 )
	{
		DestColor.r = BlendColor.r;
	}
	else
	if( RGBASelect.r < 6.0 )
	{
		DestColor.r = BlendColor.g;
	}
	else
	if( RGBASelect.r < 7.0 )
	{
		DestColor.r = BlendColor.b;
	}
	else
	if( RGBASelect.r < 8.0 )
	{
		DestColor.r = BlendColor.a;
	}
	else
	if( RGBASelect.r < 9.0 )
	{
		DestColor.r = 1.0 - Color.r;
	}
	else
	if( RGBASelect.r < 10.0 )
	{
		DestColor.r = 1.0 - Color.g;
	}
	else
	if( RGBASelect.r < 11.0 )
	{
		DestColor.r = 1.0 - Color.b;
	}
	else
	if( RGBASelect.r < 12.0 )
	{
		DestColor.r = 1.0 - Color.a;
	}
	else
	if( RGBASelect.r < 13.0 )
	{
		DestColor.r = 1.0 - BlendColor.r;
	}
	else
	if( RGBASelect.r < 14.0 )
	{
		DestColor.r = 1.0 - BlendColor.g;
	}
	else
	if( RGBASelect.r < 15.0 )
	{
		DestColor.r = 1.0 - BlendColor.b;
	}
	else
	{
		DestColor.r = 1.0 - BlendColor.a;
	}
	
	if( RGBASelect.g < 1.0 )
	{
		DestColor.g = Color.r;
	}
	else
	if( RGBASelect.g < 2.0 )
	{
		DestColor.g = Color.g;
	}
	else
	if( RGBASelect.g < 3.0 )
	{
		DestColor.g = Color.b;
	}
	else
	if( RGBASelect.g < 4.0 )
	{
		DestColor.g = Color.a;
	}
	else
	if( RGBASelect.g < 5.0 )
	{
		DestColor.g = BlendColor.r;
	}
	else
	if( RGBASelect.g < 6.0 )
	{
		DestColor.g = BlendColor.g;
	}
	else
	if( RGBASelect.g < 7.0 )
	{
		DestColor.g = BlendColor.b;
	}
	else
	if( RGBASelect.g < 8.0 )
	{
		DestColor.g = BlendColor.a;
	}
	else
	if( RGBASelect.g < 9.0 )
	{
		DestColor.g = 1.0 - Color.r;
	}
	else
	if( RGBASelect.g < 10.0 )
	{
		DestColor.g = 1.0 - Color.g;
	}
	else
	if( RGBASelect.g < 11.0 )
	{
		DestColor.g = 1.0 - Color.b;
	}
	else
	if( RGBASelect.g < 12.0 )
	{
		DestColor.g = 1.0 - Color.a;
	}
	else
	if( RGBASelect.g < 13.0 )
	{
		DestColor.g = 1.0 - BlendColor.r;
	}
	else
	if( RGBASelect.g < 14.0 )
	{
		DestColor.g = 1.0 - BlendColor.g;
	}
	else
	if( RGBASelect.g < 15.0 )
	{
		DestColor.g = 1.0 - BlendColor.b;
	}
	else
	{
		DestColor.g = 1.0 - BlendColor.a;
	}
	
	if( RGBASelect.b < 1.0 )
	{
		DestColor.b = Color.r;
	}
	else
	if( RGBASelect.b < 2.0 )
	{
		DestColor.b = Color.g;
	}
	else
	if( RGBASelect.b < 3.0 )
	{
		DestColor.b = Color.b;
	}
	else
	if( RGBASelect.b < 4.0 )
	{
		DestColor.b = Color.a;
	}
	else
	if( RGBASelect.b < 5.0 )
	{
		DestColor.b = BlendColor.r;
	}
	else
	if( RGBASelect.b < 6.0 )
	{
		DestColor.b = BlendColor.g;
	}
	else
	if( RGBASelect.b < 7.0 )
	{
		DestColor.b = BlendColor.b;
	}
	else
	if( RGBASelect.b < 8.0 )
	{
		DestColor.b = BlendColor.a;
	}
	else
	if( RGBASelect.b < 9.0 )
	{
		DestColor.b = 1.0 - Color.r;
	}
	else
	if( RGBASelect.b < 10.0 )
	{
		DestColor.b = 1.0 - Color.g;
	}
	else
	if( RGBASelect.b < 11.0 )
	{
		DestColor.b = 1.0 - Color.b;
	}
	else
	if( RGBASelect.b < 12.0 )
	{
		DestColor.b = 1.0 - Color.a;
	}
	else
	if( RGBASelect.b < 13.0 )
	{
		DestColor.b = 1.0 - BlendColor.r;
	}
	else
	if( RGBASelect.b < 14.0 )
	{
		DestColor.b = 1.0 - BlendColor.g;
	}
	else
	if( RGBASelect.b < 15.0 )
	{
		DestColor.b = 1.0 - BlendColor.b;
	}
	else
	{
		DestColor.b = 1.0 - BlendColor.a;
	}
	
	if( RGBASelect.a < 1.0 )
	{
		DestColor.a = Color.r;
	}
	else
	if( RGBASelect.a < 2.0 )
	{
		DestColor.a = Color.g;
	}
	else
	if( RGBASelect.a < 3.0 )
	{
		DestColor.a = Color.b;
	}
	else
	if( RGBASelect.a < 4.0 )
	{
		DestColor.a = Color.a;
	}
	else
	if( RGBASelect.a < 5.0 )
	{
		DestColor.a = BlendColor.r;
	}
	else
	if( RGBASelect.a < 6.0 )
	{
		DestColor.a = BlendColor.g;
	}
	else
	if( RGBASelect.a < 7.0 )
	{
		DestColor.a = BlendColor.b;
	}
	else
	if( RGBASelect.a < 8.0 )
	{
		DestColor.a = BlendColor.a;
	}
	else
	if( RGBASelect.a < 9.0 )
	{
		DestColor.a = 1.0 - Color.r;
	}
	else
	if( RGBASelect.a < 10.0 )
	{
		DestColor.a = 1.0 - Color.g;
	}
	else
	if( RGBASelect.a < 11.0 )
	{
		DestColor.a = 1.0 - Color.b;
	}
	else
	if( RGBASelect.a < 12.0 )
	{
		DestColor.a = 1.0 - Color.a;
	}
	else
	if( RGBASelect.a < 13.0 )
	{
		DestColor.a = 1.0 - BlendColor.r;
	}
	else
	if( RGBASelect.a < 14.0 )
	{
		DestColor.a = 1.0 - BlendColor.g;
	}
	else
	if( RGBASelect.a < 15.0 )
	{
		DestColor.a = 1.0 - BlendColor.b;
	}
	else
	{
		DestColor.a = 1.0 - BlendColor.a;
	}
	
#ifdef PMA_IMAGE
	DestColor.rgb *= DestColor.a;
#endif
	
	return DestColor;
}






float4 SR_SG_SB_BR_PS( VS_OUTPUT In ) : Color
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
	
	Color.a = BlendColor.r;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 SR_SG_SB_BG_PS( VS_OUTPUT In ) : Color
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

	Color.a = BlendColor.g;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 SR_SG_SB_BB_PS( VS_OUTPUT In ) : Color
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

	Color.a = BlendColor.b;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SR_SG_SB_BA_PS( VS_OUTPUT In ) : Color
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

	Color.a = BlendColor.a;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}
























float4 SR_SR_SR_BR_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.rrrr;
	Color.a = BlendColor.r;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SG_BR_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.gggg;
	Color.a = BlendColor.r;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SB_BR_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.bbbb;
	Color.a = BlendColor.r;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}




float4 SR_SR_SR_BG_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.rrrr;
	Color.a = BlendColor.g;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SG_BG_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.gggg;
	Color.a = BlendColor.g;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SB_BG_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.bbbb;
	Color.a = BlendColor.g;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}




float4 SR_SR_SR_BB_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.rrrr;
	Color.a = BlendColor.b;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SG_BB_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.gggg;
	Color.a = BlendColor.b;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SB_BB_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.bbbb;
	Color.a = BlendColor.b;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}




float4 SR_SR_SR_BA_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.rrrr;
	Color.a = BlendColor.a;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SG_BA_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.gggg;
	Color.a = BlendColor.a;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SB_BA_PS( VS_OUTPUT In ) : Color
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
	
	Color = Color.bbbb;
	Color.a = BlendColor.a;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}




























float4 SR_SR_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abrr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rarr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.garr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.barr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SR_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aarr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}













float4 SR_SR_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrgr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grgr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brgr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.argr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rggr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gggr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bggr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aggr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbgr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbgr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbgr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abgr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ragr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gagr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bagr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SG_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aagr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}















float4 SR_SR_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abbr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rabr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gabr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.babr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SB_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aabr;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}




















float4 SR_SR_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.raar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gaar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.baar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SA_SR_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aaar;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}





































float4 SR_SR_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abrg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rarg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.garg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.barg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SR_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aarg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}













float4 SR_SR_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrgg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grgg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brgg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.argg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rggg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gggg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bggg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aggg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbgg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbgg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbgg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abgg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ragg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gagg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bagg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SG_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aagg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}















float4 SR_SR_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abbg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rabg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gabg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.babg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SB_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aabg;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}




















float4 SR_SR_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.raag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gaag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.baag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SA_SG_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aaag;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}





































float4 SR_SR_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abrb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rarb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.garb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.barb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SR_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aarb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}













float4 SR_SR_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrgb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grgb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brgb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.argb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rggb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gggb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bggb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aggb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbgb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbgb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbgb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abgb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ragb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gagb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bagb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SG_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aagb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}















float4 SR_SR_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abbb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rabb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gabb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.babb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SB_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aabb;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}




















float4 SR_SR_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.raab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gaab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.baab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SA_SB_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aaab;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}





































float4 SR_SR_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abra;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rara;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gara;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bara;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SR_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aara;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}













float4 SR_SR_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.raga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gaga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.baga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SG_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aaga;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}















float4 SR_SR_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rrba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.grba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.brba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.arba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.raba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gaba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.baba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SB_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aaba;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}




















float4 SR_SR_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rraa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SR_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.graa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SR_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.braa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SR_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.araa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SG_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rgaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SG_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.ggaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SG_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bgaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SG_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.agaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SB_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.rbaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SB_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gbaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SB_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.bbaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SB_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.abaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}



float4 SR_SA_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.raaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SG_SA_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.gaaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SB_SA_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.baaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}

float4 SA_SA_SA_SA_PS( VS_OUTPUT In ) : Color
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

	Color = Color.aaaa;
	
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
	return Color;
}






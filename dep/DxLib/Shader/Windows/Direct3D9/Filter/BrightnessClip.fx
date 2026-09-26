float   ClipBrightness : register( c0 ) ;
float4  RGBToY         : register( c1 ) ;
float4  ClipFillColor  : register( c2 ) ;
sampler SrcTexture     : register( s0 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

struct VS_INPUT 
{
	float4 Position        : POSITION ;
	float4 Normal          : NORMAL ;
	float4 Diffuse         : COLOR0 ;
	float4 Specular        : COLOR1 ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
} ;

VS_OUTPUT VS_main( VS_INPUT In )
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
    
    Out.Position = In.Position;
    Out.TexCoords0 = In.TexCoords0;

    return Out;
}

float4 PS_Less(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color = tex2D( SrcTexture, In.TexCoords0 ) ;
    
#ifdef PMA_IMAGE
	if( Color.a < 0.001f )
	{
		Color.r = 0.0f ;
		Color.g = 0.0f ;
		Color.b = 0.0f ;
	}
	else
	{
		Color.rgb = Color.rgb / Color.a;
	}
#endif
    
    clip( dot( RGBToY, Color ) - ClipBrightness ) ;
    
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif
	
    return Color;
}

float4 PS_Less_ClipFill(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color = tex2D( SrcTexture, In.TexCoords0 ) ;

#ifdef PMA_IMAGE
	if( Color.a < 0.001f )
	{
		Color.r = 0.0f ;
		Color.g = 0.0f ;
		Color.b = 0.0f ;
	}
	else
	{
		Color.rgb = Color.rgb / Color.a;
	}
#endif
    
    if( dot( RGBToY, Color ) < ClipBrightness )
    {
        Color = ClipFillColor;
    }
    
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

float4 PS_Greater(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color = tex2D( SrcTexture, In.TexCoords0 ) ;
    
#ifdef PMA_IMAGE
	if( Color.a < 0.001f )
	{
		Color.r = 0.0f ;
		Color.g = 0.0f ;
		Color.b = 0.0f ;
	}
	else
	{
		Color.rgb = Color.rgb / Color.a;
	}
#endif

    clip( ClipBrightness - dot( RGBToY, Color ) ) ;

#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

    return Color;
}

float4 PS_Greater_ClipFill(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color = tex2D( SrcTexture, In.TexCoords0 ) ;

#ifdef PMA_IMAGE
	if( Color.a < 0.001f )
	{
		Color.r = 0.0f ;
		Color.g = 0.0f ;
		Color.b = 0.0f ;
	}
	else
	{
		Color.rgb = Color.rgb / Color.a;
	}
#endif
    
    if( dot( RGBToY, Color ) > ClipBrightness )
    {
		Color = ClipFillColor;
    }
    
#ifdef PMA_IMAGE
	Color.rgb *= Color.a;
#endif

	return Color;
}

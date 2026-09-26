float4x4 projectMatrix : register( c0  ) ;
float4x4 clipMatrix    : register( c4  ) ;
float4   baseColor     : register( c8  ) ;
float4   channelFlag   : register( c9  ) ;
sampler  mainSampler   : register( s0  ) ;
sampler  maskSampler   : register( s1  ) ;

struct VS_IN 
{
    float3 pos : POSITION0;
    float4 spos : POSITION1;
    float3 norm : NORMAL;
    float3 tan    : TANGENT0;
    float3 binorm : BINORMAL0;
    float4 dif : COLOR0;
    float4 spc : COLOR1;
    float2 uv  : TEXCOORD0;
    float2 suv : TEXCOORD1;
};
struct VS_OUT 
{
	float4 Position     : POSITION0;
	float2 uv           : TEXCOORD0;
	float4 clipPosition : TEXCOORD1;
};

/* Setup mask shader */
VS_OUT VertSetupMask( VS_IN In )
{
	VS_OUT Out = ( VS_OUT )0;
	Out.Position     = mul( float4( In.pos, 1.0f ), projectMatrix );
	Out.clipPosition = mul( float4( In.pos, 1.0f ), projectMatrix );
	Out.uv.x = In.uv.x;
	Out.uv.y = 1.0 - In.uv.y;
	return Out;
}
float4 PixelSetupMask( VS_OUT In ) : COLOR0
{
	float isInside =
		  step( baseColor.x, In.clipPosition.x / In.clipPosition.w )
		* step( baseColor.y, In.clipPosition.y / In.clipPosition.w )
		* step(              In.clipPosition.x / In.clipPosition.w, baseColor.z )
		* step(              In.clipPosition.y / In.clipPosition.w, baseColor.w );
	return channelFlag * tex2D( mainSampler, In.uv ).a * isInside;
//	float4 color = channelFlag * tex2D( mainSampler, In.uv ).a * isInside;
//	float4 color = tex2D( mainSampler, In.uv );
//	float4 color;
//	color.r = In.uv.x;
//	color.g = In.uv.y;
//	color.b = .a;
//	color.r = isInside;
//	color.g = 0.0;
//	color.b = 0.0;
//	color.r = channelFlag.r;
//	color.g = channelFlag.g;
//	color.b = channelFlag.b;
//	color.r = color.a;
//	color.g = 0.0;
//	color.b = 0.0;
//	color.a = 1.0f;
//	return color;
}

/* Vertex Shader */
/* normal */
VS_OUT VertNormal( VS_IN In )
{
	VS_OUT Out = ( VS_OUT )0;
	Out.Position = mul( float4( In.pos, 1.0f ), projectMatrix );
	Out.uv.x =        In.uv.x;
	Out.uv.y = 1.0 - +In.uv.y;
	return Out;
}
/* masked */
VS_OUT VertMasked( VS_IN In ) 
{
	VS_OUT Out = ( VS_OUT )0;
	Out.Position = mul( float4( In.pos, 1.0f ), projectMatrix );
	Out.clipPosition = mul( float4( In.pos, 1.0f ), clipMatrix );
	Out.uv.x =       In.uv.x;
	Out.uv.y = 1.0 - In.uv.y;
	return Out;
}

/* Pixel Shader */
/* normal */
float4 PixelNormal( VS_OUT In ) : COLOR0
{
	float4 color = tex2D( mainSampler, In.uv ) * baseColor;
	color.xyz *= color.w;
	return color;
}

/* normal premult alpha */
float4 PixelNormalPremult( VS_OUT In ) : COLOR0
{
	float4 color = tex2D( mainSampler, In.uv ) * baseColor;
	return color;
}

/* masked */
float4 PixelMasked( VS_OUT In ) : COLOR0
{
	float4 color    = tex2D( mainSampler, In.uv ) * baseColor;
	color.xyz      *= color.w;
	float4 clipMask = ( 1.0 - tex2D( maskSampler, In.clipPosition.xy / In.clipPosition.w ) ) * channelFlag;
	float maskVal   = clipMask.r + clipMask.g + clipMask.b + clipMask.a;
	color           = color * maskVal;
	return color;
}

/* masked inverted*/
float4 PixelMaskedInverted( VS_OUT In ) : COLOR0
{
	float4 color    = tex2D( mainSampler, In.uv ) * baseColor;
	color.xyz      *= color.w;
	float4 clipMask = ( 1.0 - tex2D( maskSampler, In.clipPosition.xy / In.clipPosition.w ) ) * channelFlag;
	float maskVal   = clipMask.r + clipMask.g + clipMask.b + clipMask.a;
	color           = color * ( 1.0 - maskVal );
	return color;
}

/* masked premult alpha */
float4 PixelMaskedPremult( VS_OUT In ) : COLOR0
{
	float4 color    = tex2D( mainSampler, In.uv ) * baseColor;
	float4 clipMask = ( 1.0 - tex2D( maskSampler, In.clipPosition.xy / In.clipPosition.w ) ) * channelFlag;
	float maskVal   = clipMask.r + clipMask.g + clipMask.b + clipMask.a;
	color           = color * maskVal;
	return color;
}

/* masked inverted premult alpha */
float4 PixelMaskedInvertedPremult( VS_OUT In ) : SV_Target
{
	float4 color    = tex2D( mainSampler, In.uv ) * baseColor;
	float4 clipMask = ( 1.0 - tex2D( maskSampler, In.clipPosition.xy / In.clipPosition.w ) ) * channelFlag;
	float maskVal   = clipMask.r + clipMask.g + clipMask.b + clipMask.a;
	color           = color * ( 1.0 - maskVal );
	return color;
};


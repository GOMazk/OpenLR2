float4 UVTextureCoordScale_RRA_Add_U : register( c0 ) ;

sampler SrcTexture : register( s0 ) ;
sampler UVTexture  : register( s1 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 Y2UV2toRGB_PS(VS_OUTPUT In) : COLOR
{
	float4 DestColor ;
	float4 YUVColor ;

	YUVColor = tex2D( SrcTexture, In.TexCoords0 ) ;

	DestColor.r = saturate( 1.164f * ( YUVColor.r - 0.0625f ) + 1.596f * ( YUVColor.b - 0.5f ) ) ;
	DestColor.g = saturate( 1.164f * ( YUVColor.r - 0.0625f ) - 0.391f * ( YUVColor.g - 0.5f ) - 0.813f * ( YUVColor.b - 0.5f ) ) ;
	DestColor.b = saturate( 1.164f * ( YUVColor.r - 0.0625f ) + 2.018f * ( YUVColor.g - 0.5f ) ) ;
	DestColor.a = 1.0f ;

	return DestColor ;
}

float4 Y2UV2toRGB_RRA_PS(VS_OUTPUT In) : COLOR
{
	float4 DestColor ;
	float4 YUVColor ;
	float4 A_YUVColor ;
	float2 A_TexCoords ;

	YUVColor = tex2D( SrcTexture, In.TexCoords0 ) ;

	A_TexCoords = In.TexCoords0 ;
	A_TexCoords.x += UVTextureCoordScale_RRA_Add_U.z ;
	A_YUVColor = tex2D( SrcTexture, A_TexCoords ) ;

	DestColor.r = saturate( 1.164f * ( YUVColor.r   - 0.0625f ) + 1.596f * ( YUVColor.b   - 0.5f ) ) ;
	DestColor.g = saturate( 1.164f * ( YUVColor.r   - 0.0625f ) - 0.391f * ( YUVColor.g   - 0.5f ) - 0.813f * ( YUVColor.b - 0.5f ) ) ;
	DestColor.b = saturate( 1.164f * ( YUVColor.r   - 0.0625f ) + 2.018f * ( YUVColor.g   - 0.5f ) ) ;
	DestColor.a = saturate( 1.164f * ( A_YUVColor.r - 0.0625f ) + 1.596f * ( A_YUVColor.b - 0.5f ) ) ;

	return DestColor ;
}

float4 Y2UV1toRGB_PS(VS_OUTPUT In) : COLOR
{
	float4 DestColor ;
	float4 YColor ;
	float4 UVColor ;
	float2 UVTexCoord ;

	YColor  = tex2D( SrcTexture, In.TexCoords0 ) ;
	UVTexCoord = In.TexCoords0 ;
	UVTexCoord.x *= UVTextureCoordScale_RRA_Add_U.x ;
	UVTexCoord.y *= UVTextureCoordScale_RRA_Add_U.y ;
	UVColor = tex2D( UVTexture, UVTexCoord ) ;

	DestColor.r = saturate( 1.164f * ( YColor.r - 0.0625f ) + 1.596f * ( UVColor.a - 0.5f ) ) ;
	DestColor.g = saturate( 1.164f * ( YColor.r - 0.0625f ) - 0.391f * ( UVColor.r - 0.5f ) - 0.813f * ( UVColor.a - 0.5f ) ) ;
	DestColor.b = saturate( 1.164f * ( YColor.r - 0.0625f ) + 2.018f * ( UVColor.r - 0.5f ) ) ;
	DestColor.a = 1.0f ;

	return DestColor ;
}

float4 Y2UV1toRGB_RRA_PS(VS_OUTPUT In) : COLOR
{
	float4 DestColor ;
	float4 YColor ;
	float4 UVColor ;
	float2 UVTexCoord ;
	float4 A_YColor ;
	float4 A_UVColor ;
	float2 A_TexCoords ;

	YColor   = tex2D( SrcTexture, In.TexCoords0 ) ;
	UVTexCoord = In.TexCoords0 ;
	UVTexCoord.x *= UVTextureCoordScale_RRA_Add_U.x ;
	UVTexCoord.y *= UVTextureCoordScale_RRA_Add_U.y ;
	UVColor = tex2D( UVTexture, UVTexCoord ) ;
	
	A_TexCoords = In.TexCoords0 ;
	A_TexCoords.x += UVTextureCoordScale_RRA_Add_U.z ;
	A_YColor = tex2D( SrcTexture, A_TexCoords ) ;
	A_TexCoords = In.TexCoords0 ;
	A_TexCoords.x *= UVTextureCoordScale_RRA_Add_U.x ;
	A_TexCoords.y *= UVTextureCoordScale_RRA_Add_U.y ;
	A_TexCoords.x += UVTextureCoordScale_RRA_Add_U.w ;
	A_UVColor = tex2D( UVTexture, A_TexCoords ) ;

	DestColor.r = saturate( 1.164f * ( YColor.r   - 0.0625f ) + 1.596f * ( UVColor.a   - 0.5f ) ) ;
	DestColor.g = saturate( 1.164f * ( YColor.r   - 0.0625f ) - 0.391f * ( UVColor.r   - 0.5f ) - 0.813f * ( UVColor.a - 0.5f ) ) ;
	DestColor.b = saturate( 1.164f * ( YColor.r   - 0.0625f ) + 2.018f * ( UVColor.r   - 0.5f ) ) ;
	DestColor.a = saturate( 1.164f * ( A_YColor.r - 0.0625f ) + 1.596f * ( A_UVColor.a - 0.5f ) ) ;

	return DestColor ;
}

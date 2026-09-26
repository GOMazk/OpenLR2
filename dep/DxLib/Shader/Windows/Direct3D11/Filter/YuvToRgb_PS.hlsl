#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 UVTextureCoordScale_RRA_Add_U ;
} ;

SamplerState g_SrcSampler  : register( s0 ) ;
Texture2D    g_SrcTexture  : register( t0 ) ;

SamplerState g_UVSampler   : register( s1 ) ;
Texture2D    g_UVTexture   : register( t1 ) ;

float4 Y2UV2toRGB_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 DestColor ;
	float4 YUVColor ;

	YUVColor = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;

	DestColor.r = saturate( 1.164f * ( YUVColor.r - 0.0625f ) + 1.596f * ( YUVColor.b - 0.5f ) ) ;
	DestColor.g = saturate( 1.164f * ( YUVColor.r - 0.0625f ) - 0.391f * ( YUVColor.g - 0.5f ) - 0.813f * ( YUVColor.b - 0.5f ) ) ;
	DestColor.b = saturate( 1.164f * ( YUVColor.r - 0.0625f ) + 2.018f * ( YUVColor.g - 0.5f ) ) ;
	DestColor.a = 1.0f ;

	return DestColor ;
}

float4 Y2UV2toRGB_RRA_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 DestColor ;
	float4 YUVColor ;
	float4 A_YUVColor ;
	float2 A_TexCoords ;

	YUVColor = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;

	A_TexCoords = In.TexCoords0 ;
	A_TexCoords.x += UVTextureCoordScale_RRA_Add_U.z ;
	A_YUVColor = g_SrcTexture.Sample( g_SrcSampler, A_TexCoords ) ;

	DestColor.r = saturate( 1.164f * ( YUVColor.r   - 0.0625f ) + 1.596f * ( YUVColor.b   - 0.5f ) ) ;
	DestColor.g = saturate( 1.164f * ( YUVColor.r   - 0.0625f ) - 0.391f * ( YUVColor.g   - 0.5f ) - 0.813f * ( YUVColor.b - 0.5f ) ) ;
	DestColor.b = saturate( 1.164f * ( YUVColor.r   - 0.0625f ) + 2.018f * ( YUVColor.g   - 0.5f ) ) ;
	DestColor.a = saturate( 1.164f * ( A_YUVColor.r - 0.0625f ) + 1.596f * ( A_YUVColor.b - 0.5f ) ) ;

	return DestColor ;
}

float4 Y2UV1toRGB_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 DestColor ;
	float4 YColor ;
	float4 UVColor ;
	float2 UVTexCoord ;

	YColor  = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	UVTexCoord = In.TexCoords0 ;
	UVTexCoord.x *= UVTextureCoordScale_RRA_Add_U.x ;
	UVTexCoord.y *= UVTextureCoordScale_RRA_Add_U.y ;
	UVColor = g_UVTexture.Sample(  g_UVSampler, UVTexCoord ) ;

	DestColor.r = saturate( 1.164f * ( YColor.r - 0.0625f ) + 1.596f * ( UVColor.g - 0.5f ) ) ;
	DestColor.g = saturate( 1.164f * ( YColor.r - 0.0625f ) - 0.391f * ( UVColor.r - 0.5f ) - 0.813f * ( UVColor.g - 0.5f ) ) ;
	DestColor.b = saturate( 1.164f * ( YColor.r - 0.0625f ) + 2.018f * ( UVColor.r - 0.5f ) ) ;
	DestColor.a = 1.0f ;

	return DestColor ;
}

float4 Y2UV1toRGB_RRA_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 DestColor ;
	float4 YColor ;
	float4 UVColor ;
	float2 UVTexCoord ;
	float4 A_YColor ;
	float4 A_UVColor ;
	float2 A_TexCoords ;

	YColor  = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	UVTexCoord = In.TexCoords0 ;
	UVTexCoord.x *= UVTextureCoordScale_RRA_Add_U.x ;
	UVTexCoord.y *= UVTextureCoordScale_RRA_Add_U.y ;
	UVColor = g_UVTexture.Sample(  g_UVSampler, UVTexCoord ) ;

	A_TexCoords = In.TexCoords0 ;
	A_TexCoords.x += UVTextureCoordScale_RRA_Add_U.z ;
	A_YColor = g_SrcTexture.Sample( g_SrcSampler, A_TexCoords ) ;
	A_TexCoords = In.TexCoords0 ;
	A_TexCoords.x *= UVTextureCoordScale_RRA_Add_U.x ;
	A_TexCoords.y *= UVTextureCoordScale_RRA_Add_U.y ;
	A_TexCoords.x += UVTextureCoordScale_RRA_Add_U.w ;
	A_UVColor = g_UVTexture.Sample(  g_UVSampler, A_TexCoords ) ;

	DestColor.r = saturate( 1.164f * ( YColor.r   - 0.0625f ) + 1.596f * ( UVColor.g   - 0.5f ) ) ;
	DestColor.g = saturate( 1.164f * ( YColor.r   - 0.0625f ) - 0.391f * ( UVColor.r   - 0.5f ) - 0.813f * ( UVColor.g - 0.5f ) ) ;
	DestColor.b = saturate( 1.164f * ( YColor.r   - 0.0625f ) + 2.018f * ( UVColor.r   - 0.5f ) ) ;
	DestColor.a = saturate( 1.164f * ( A_YColor.r - 0.0625f ) + 1.596f * ( A_UVColor.g - 0.5f ) ) ;

	return DestColor ;
}

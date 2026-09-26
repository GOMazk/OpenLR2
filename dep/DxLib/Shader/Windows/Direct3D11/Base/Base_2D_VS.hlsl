#include "../VertexShader.h"

// 頂点シェーダーの入力
struct VS_INPUT
{
	float4 Position        : POSITION0 ;		// 座標( トランスフォーム済み )

#ifdef USE_DIFFUSE
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
#endif // USE_DIFFUSE

#ifdef USE_SPECULAR
	float4 Specular        : COLOR1 ;			// スペキュラカラー
#endif // USE_SPECULAR

#ifdef USE_TEXCOORD0
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標０
#endif // USE_TEXCOORD0

#ifdef USE_TEXCOORD1
	float2 TexCoords1      : TEXCOORD1 ;		// テクスチャ座標１
#endif // USE_TEXCOORD1
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Position        : SV_POSITION ;
	float4 Diffuse         : COLOR0 ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
} ;

// 2D用
VS_OUTPUT VS2D_Normal( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 Position;
	

	Position = VSInput.Position ;
	Position.w = 1.0f / Position.w ;
	Position.x *= Position.w ;
	Position.y *= Position.w ;
	Position.z *= Position.w ;


	// 座標変換
	VSOutput.Position.x = dot( Position, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( Position, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( Position, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( Position, g_Base.ProjectionMatrix[ 3 ] ) ;


	// パラメータセット
#ifdef USE_DIFFUSE
	VSOutput.Diffuse    = VSInput.Diffuse ;
#else  // USE_DIFFUSE
	VSOutput.Diffuse    = 1.0f ;
#endif // USE_DIFFUSE


#ifdef USE_TEXCOORD0
	VSOutput.TexCoords0 = VSInput.TexCoords0 ;
#else  // USE_TEXCOORD0
	VSOutput.TexCoords0 = 0.0f ;
#endif // USE_TEXCOORD0


#ifdef USE_TEXCOORD1
	VSOutput.TexCoords1 = VSInput.TexCoords1 ;
#else  // USE_TEXCOORD1
	VSOutput.TexCoords1 = 0.0f ;
#endif // USE_TEXCOORD1

	
	return VSOutput ;
}





// DrawPrimitive2DToShader用頂点シェーダーの入力
struct VS_SHADER2D_INPUT
{
	float4 Position        : POSITION0 ;		// 座標( トランスフォーム済み )
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
	float4 Specular        : COLOR1 ;			// スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標０
	float2 TexCoords1      : TEXCOORD1 ;		// テクスチャ座標１
} ;

// DrawPrimitive2DToShader用頂点シェーダーの出力
struct VS_SHADER2D_OUTPUT
{
	float4 Position        : SV_POSITION ;
	float4 Diffuse         : COLOR0 ;
	float4 Specular        : COLOR1 ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
} ;

// DrawPrimitive2DToShader用
VS_SHADER2D_OUTPUT VS_Shader2D( VS_SHADER2D_INPUT VSInput )
{
	VS_SHADER2D_OUTPUT VSOutput ;
	float4 Position;
	

	Position = VSInput.Position ;
	Position.w = 1.0f / Position.w ;
	Position.x *= Position.w ;
	Position.y *= Position.w ;
	Position.z *= Position.w ;


	// 座標変換
	VSOutput.Position.x = dot( Position, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( Position, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( Position, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( Position, g_Base.ProjectionMatrix[ 3 ] ) ;


	// パラメータセット
	VSOutput.Diffuse    = VSInput.Diffuse ;
	VSOutput.Specular   = VSInput.Specular ;
	VSOutput.TexCoords0 = VSInput.TexCoords0 ;
	VSOutput.TexCoords1 = VSInput.TexCoords1 ;
	
	return VSOutput ;
}





// 単純転送用頂点シェーダーの入力
struct STRETCHRECT_VS_INPUT
{
	float2 Position        : POSITION0 ;		// 座標
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標0
	float2 TexCoords1      : TEXCOORD1 ;		// テクスチャ座標1
} ;

// 単純転送用頂点シェーダーの出力
struct STRETCHRECT_VS_OUTPUT
{
	float4 Position        : SV_POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
} ;

// 単純転送用頂点シェーダー
STRETCHRECT_VS_OUTPUT StretchRect_VS( STRETCHRECT_VS_INPUT VSInput )
{
	STRETCHRECT_VS_OUTPUT VSOutput ;

	VSOutput.Position.x = VSInput.Position.x ;
	VSOutput.Position.y = VSInput.Position.y ;
	VSOutput.Position.z = 0.0f ;
	VSOutput.Position.w = 1.0f ;

	VSOutput.TexCoords0 = VSInput.TexCoords0 ;
	VSOutput.TexCoords1 = VSInput.TexCoords1 ;
	
	return VSOutput ;
}







// テクスチャ座標8個単純転送用頂点シェーダーの入力
struct STRETCHRECT_TEX8_VS_INPUT
{
	float2 Position        : POSITION0 ;		// 座標
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標0
	float2 TexCoords1      : TEXCOORD1 ;		// テクスチャ座標1
	float2 TexCoords2      : TEXCOORD2 ;		// テクスチャ座標2
	float2 TexCoords3      : TEXCOORD3 ;		// テクスチャ座標3
	float2 TexCoords4      : TEXCOORD4 ;		// テクスチャ座標4
	float2 TexCoords5      : TEXCOORD5 ;		// テクスチャ座標5
	float2 TexCoords6      : TEXCOORD6 ;		// テクスチャ座標6
	float2 TexCoords7      : TEXCOORD7 ;		// テクスチャ座標7
} ;

// テクスチャ座標8個単純転送用頂点シェーダーの出力
struct STRETCHRECT_TEX8_VS_OUTPUT
{
	float4 Position        : SV_POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
	float2 TexCoords2      : TEXCOORD2 ;
	float2 TexCoords3      : TEXCOORD3 ;
	float2 TexCoords4      : TEXCOORD4 ;
	float2 TexCoords5      : TEXCOORD5 ;
	float2 TexCoords6      : TEXCOORD6 ;
	float2 TexCoords7      : TEXCOORD7 ;
} ;

// テクスチャ座標8個単純転送用頂点シェーダー
STRETCHRECT_TEX8_VS_OUTPUT StretchRect_Tex8_VS( STRETCHRECT_TEX8_VS_INPUT VSInput )
{
	STRETCHRECT_TEX8_VS_OUTPUT VSOutput ;

	VSOutput.Position.x = VSInput.Position.x ;
	VSOutput.Position.y = VSInput.Position.y ;
	VSOutput.Position.z = 0.0f ;
	VSOutput.Position.w = 1.0f ;

	VSOutput.TexCoords0 = VSInput.TexCoords0 ;
	VSOutput.TexCoords1 = VSInput.TexCoords1 ;
	VSOutput.TexCoords2 = VSInput.TexCoords2 ;
	VSOutput.TexCoords3 = VSInput.TexCoords3 ;
	VSOutput.TexCoords4 = VSInput.TexCoords4 ;
	VSOutput.TexCoords5 = VSInput.TexCoords5 ;
	VSOutput.TexCoords6 = VSInput.TexCoords6 ;
	VSOutput.TexCoords7 = VSInput.TexCoords7 ;
	
	return VSOutput ;
}

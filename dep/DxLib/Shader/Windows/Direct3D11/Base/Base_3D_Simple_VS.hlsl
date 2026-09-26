#include "../VertexShader.h"

// 頂点シェーダーの入力
struct VS_INPUT
{
	float3 Position        : POSITION0 ;		// 座標( ローカル空間 )

#if USE_VERTEX3DSHADER
	float4 SubPosition     : POSITION1 ;		// 補助座標
#endif // USE_VERTEX3DSHADER

#if USE_VERTEX3DSHADER || USE_VERTEX3D
	float3 Normal          : NORMAL ;			// 法線
#endif // USE_VERTEX3DSHADER || USE_VERTEX3D

#if USE_VERTEX3DSHADER
	float3 Tangent         : TANGENT ;			// 接線
	float3 Binormal        : BINORMAL ;			// 従法線
#endif // USE_VERTEX3DSHADER

	float4 Diffuse         : COLOR0 ;			// ディフューズカラー

#if USE_VERTEX3DSHADER || USE_VERTEX3D
	float4 Specular        : COLOR1 ;			// スペキュラカラー
#endif // USE_VERTEX3DSHADER || USE_VERTEX3D

#if USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標０
#endif // USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D

#if USE_VERTEX3DSHADER || USE_VERTEX3D
	float2 TexCoords1      : TEXCOORD1 ;		// テクスチャ座標１
#endif // USE_VERTEX3DSHADER || USE_VERTEX3D
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Position        : SV_POSITION ;		// 座標( プロジェクション空間 )
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標０
	float2 TexCoords1      : TEXCOORD1 ;		// テクスチャ座標１
} ;

// 3D用
VS_OUTPUT VS3D_Normal( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4    LocalPosition ;
	float4    WorldPosition ;
	float4    ViewPosition ;
	float4    lTextureCoordTemp ;


	// ローカル座標の準備
	LocalPosition.xyz   = VSInput.Position ;
	LocalPosition.w     = 1.0f ;


	// 座標変換
	WorldPosition.x     = dot( LocalPosition, g_Base.LocalWorldMatrix[ 0 ] ) ;
	WorldPosition.y     = dot( LocalPosition, g_Base.LocalWorldMatrix[ 1 ] ) ;
	WorldPosition.z     = dot( LocalPosition, g_Base.LocalWorldMatrix[ 2 ] ) ;
	WorldPosition.w     = 1.0f;
	
	ViewPosition.x      = dot( WorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	ViewPosition.y      = dot( WorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	ViewPosition.z      = dot( WorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	ViewPosition.w      = 1.0f;
	
	VSOutput.Position.x = dot( ViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( ViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( ViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( ViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;


	// パラメータセット
	VSOutput.Diffuse    = VSInput.Diffuse ;

	lTextureCoordTemp.zw = 1.0f ;
#if USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D
	lTextureCoordTemp.xy = VSInput.TexCoords0 ;
	VSOutput.TexCoords0.x = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0.y = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;
#else  // USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D
	VSOutput.TexCoords0 = 0.0f ;
#endif // USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D

#if USE_VERTEX3DSHADER || USE_VERTEX3D
	lTextureCoordTemp.xy = VSInput.TexCoords1 ;
	VSOutput.TexCoords1.x = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords1.y = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;
#else  // USE_VERTEX3DSHADER || USE_VERTEX3D
	VSOutput.TexCoords1 = 0.0f ;
#endif // USE_VERTEX3DSHADER || USE_VERTEX3D
	
	return VSOutput ;
}

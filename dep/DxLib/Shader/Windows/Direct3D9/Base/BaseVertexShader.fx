
// 頂点シェーダーの入力
struct VS_INPUT
{
	float4 Position        : POSITION ;			// 座標( ローカル空間 )
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
	float4 Specular        : COLOR1 ;			// スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標
	float2 TexCoords1      : TEXCOORD1 ;
	float2 TexCoords2      : TEXCOORD2 ;
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float4 Diffuse         : COLOR0 ;
	float4 Specular        : COLOR1 ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
	float2 TexCoords2      : TEXCOORD2 ;
} ;

float4              cfProjectionMatrix[ 4 ]                                    : register( c2  /* DX_VS_CONSTF_PROJECTION_MAT                  */ ) ;	// プロジェクション行列
float4              cfViewMatrix[ 3 ]                                          : register( c6  /* DX_VS_CONSTF_VIEW_MAT                        */ ) ;	// ワールド　→　ビュー行列
float4              cfLocalWorldMatrix[ 3 ]                                    : register( c94 /* DX_VS_CONSTF_WORLD_MAT_START                 */ ) ;	// ローカル　→　ワールド行列

// 2D用
VS_OUTPUT VS2D_Normal( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 Position;
	
	// 座標変換
	Position   = VSInput.Position;
	Position.w = 1.0f;
	VSOutput.Position.x = dot( Position, cfProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( Position, cfProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( Position, cfProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( Position, cfProjectionMatrix[ 3 ] ) ;

	// パラメータセット
	VSOutput.Diffuse    = VSInput.Diffuse ;
	VSOutput.Specular   = VSInput.Specular ;
	VSOutput.TexCoords0 = VSInput.TexCoords0 ;
	VSOutput.TexCoords1 = VSInput.TexCoords1 ;
	VSOutput.TexCoords2 = VSInput.TexCoords2 ;
	
	return VSOutput ;
}

// 3D用
VS_OUTPUT VS3D_Normal( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 WorldPosition;
	float4 ViewPosition;
	
	// 座標変換
	WorldPosition.x = dot( VSInput.Position, cfLocalWorldMatrix[ 0 ] ) ;
	WorldPosition.y = dot( VSInput.Position, cfLocalWorldMatrix[ 1 ] ) ;
	WorldPosition.z = dot( VSInput.Position, cfLocalWorldMatrix[ 2 ] ) ;
	WorldPosition.w = 1.0f;
	
	ViewPosition.x = dot( WorldPosition, cfViewMatrix[ 0 ] ) ;
	ViewPosition.y = dot( WorldPosition, cfViewMatrix[ 1 ] ) ;
	ViewPosition.z = dot( WorldPosition, cfViewMatrix[ 2 ] ) ;
	ViewPosition.w = 1.0f;
	
	VSOutput.Position.x = dot( ViewPosition, cfProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( ViewPosition, cfProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( ViewPosition, cfProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( ViewPosition, cfProjectionMatrix[ 3 ] ) ;

	// パラメータセット
	VSOutput.Diffuse    = VSInput.Diffuse ;
	VSOutput.Specular   = VSInput.Specular ;
	VSOutput.TexCoords0 = VSInput.TexCoords0 ;
	VSOutput.TexCoords1 = VSInput.TexCoords1 ;
	VSOutput.TexCoords2 = VSInput.TexCoords2 ;
	
	return VSOutput ;
}

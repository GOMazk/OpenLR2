#include "../VertexShader.h"

// 頂点シェーダーの入力
struct VS_INPUT
{
	float3 Position        : POSITION0 ;		// 座標( ローカル空間 )
	float3 Normal          : NORMAL ;			// 法線
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
	float4 Specular        : COLOR1 ;			// スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標０
	float2 TexCoords1      : TEXCOORD1 ;		// テクスチャ座標１

#ifdef BUMPMAP
	// バンプマップ
	float3 Tan             : TANGENT0 ;			// 接線( ローカル空間 )
	float3 Bin             : BINORMAL0 ;		// 従法線( ローカル空間 )
#endif // BUMPMAP

#ifdef SKINMESH
	// スキニングメッシュ
	int4   BlendIndices0   : BLENDINDICES0 ;	// ボーン処理用 Float型定数配列インデックス０
	float4 BlendWeight0    : BLENDWEIGHT0 ;		// ボーン処理用ウエイト値０

	#ifdef BONE8

		int4   BlendIndices1   : BLENDINDICES1 ;	// ボーン処理用 Float型定数配列インデックス１
		float4 BlendWeight1    : BLENDWEIGHT1 ;		// ボーン処理用ウエイト値１

	#endif // BONE8

#endif // SKINMESH
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Diffuse         : COLOR0 ;		// ディフューズカラー
	float4 Specular        : COLOR1 ;		// スペキュラカラー
	float4 TexCoords0_1    : TEXCOORD0 ;	// xy:テクスチャ座標 zw:サブテクスチャ座標
	float3 VPosition       : TEXCOORD1 ;	// 座標( ビュー空間 )
	float3 VNormal         : TEXCOORD2 ;	// 法線( ビュー空間 )
#ifdef BUMPMAP
	float3 VTan            : TEXCOORD3 ;    // 接線( ビュー空間 )
	float3 VBin            : TEXCOORD4 ;    // 従法線( ビュー空間 )
#endif // BUMPMAP
	float2 Fog             : TEXCOORD5 ;	// フォグパラメータ( x )　高さフォグパラメータ( y )

#if SHADOWMAP
	float3 ShadowMap0Pos   : TEXCOORD6 ;	// シャドウマップ０のライト座標( x, y, z )
	float3 ShadowMap1Pos   : TEXCOORD7 ;	// シャドウマップ１のライト座標( x, y, z )
	float3 ShadowMap2Pos   : TEXCOORD8 ;	// シャドウマップ２のライト座標( x, y, z )
#endif // SHADOWMAP

	float4 Position        : SV_POSITION ;	// 座標( プロジェクション空間 )
} ;



#ifdef SKINMESH
#define LOCAL_WORLD_MAT         lLocalWorldMatrix
#else
#define LOCAL_WORLD_MAT         g_Base.LocalWorldMatrix
#endif






// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT	VSOutput ;
	int4		lBoneFloatIndex ;
	float4		lLocalWorldMatrix[ 3 ] ;
	float4		lLocalPosition ;
	float4		lWorldPosition ;
	float4		lViewPosition ;
	float3		lWorldNrm ;
	float3		lWorldTan ;
	float3		lWorldBin ;
	float3		lViewNrm ;
	float3		lViewTan ;
	float3		lViewBin ;
	float		lVerticalFogY ;
	float		lFogDensity ;
	float4		lTextureCoordTemp ;
#if SHADOWMAP
	float4		lLViewPosition ;
#endif


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	#ifdef SKINMESH

		// スキンメッシュ

		// ブレンド行列の作成
		lBoneFloatIndex = VSInput.BlendIndices0 ;
		lLocalWorldMatrix[ 0 ]  = g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 0 ] * VSInput.BlendWeight0.xxxx;
		lLocalWorldMatrix[ 1 ]  = g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 1 ] * VSInput.BlendWeight0.xxxx;
		lLocalWorldMatrix[ 2 ]  = g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 2 ] * VSInput.BlendWeight0.xxxx;

		lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 0 ] * VSInput.BlendWeight0.yyyy;
		lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 1 ] * VSInput.BlendWeight0.yyyy;
		lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 2 ] * VSInput.BlendWeight0.yyyy;

		lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 0 ] * VSInput.BlendWeight0.zzzz;
		lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 1 ] * VSInput.BlendWeight0.zzzz;
		lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 2 ] * VSInput.BlendWeight0.zzzz;

		lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 0 ] * VSInput.BlendWeight0.wwww;
		lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 1 ] * VSInput.BlendWeight0.wwww;
		lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 2 ] * VSInput.BlendWeight0.wwww;

		#ifdef BONE8

			lBoneFloatIndex = VSInput.BlendIndices1 ;
			lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 0 ] * VSInput.BlendWeight1.xxxx;
			lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 1 ] * VSInput.BlendWeight1.xxxx;
			lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 2 ] * VSInput.BlendWeight1.xxxx;

			lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 0 ] * VSInput.BlendWeight1.yyyy;
			lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 1 ] * VSInput.BlendWeight1.yyyy;
			lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 2 ] * VSInput.BlendWeight1.yyyy;

			lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 0 ] * VSInput.BlendWeight1.zzzz;
			lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 1 ] * VSInput.BlendWeight1.zzzz;
			lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 2 ] * VSInput.BlendWeight1.zzzz;

			lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 0 ] * VSInput.BlendWeight1.wwww;
			lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 1 ] * VSInput.BlendWeight1.wwww;
			lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 2 ] * VSInput.BlendWeight1.wwww;

		#endif // BONE8

	#endif	// SKINMESH

	// ローカル座標のセット
	lLocalPosition.xyz = VSInput.Position ;
	lLocalPosition.w = 1.0f ;

	// 座標計算( ローカル→ビュー→プロジェクション )
	lWorldPosition.x = dot( lLocalPosition, LOCAL_WORLD_MAT[ 0 ] ) ;
	lWorldPosition.y = dot( lLocalPosition, LOCAL_WORLD_MAT[ 1 ] ) ;
	lWorldPosition.z = dot( lLocalPosition, LOCAL_WORLD_MAT[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	VSOutput.Position.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;
	
	// 座標( ビュー空間 )を保存
	VSOutput.VPosition = lViewPosition.xyz;
	
	// 法線を計算
	lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
	lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
	lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

	lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

	// 法線( ビュー空間 )を保存
	VSOutput.VNormal = lViewNrm;

	#ifdef BUMPMAP

		// 従法線、接線をビュー空間に投影する
		lWorldTan.x = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldTan.y = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldTan.z = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lWorldBin.x = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldBin.y = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldBin.z = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lViewTan.x = dot( lWorldTan, g_Base.ViewMatrix[ 0 ].xyz ) ;
		lViewTan.y = dot( lWorldTan, g_Base.ViewMatrix[ 1 ].xyz ) ;
		lViewTan.z = dot( lWorldTan, g_Base.ViewMatrix[ 2 ].xyz ) ;

		lViewBin.x = dot( lWorldBin, g_Base.ViewMatrix[ 0 ].xyz ) ;
		lViewBin.y = dot( lWorldBin, g_Base.ViewMatrix[ 1 ].xyz ) ;
		lViewBin.z = dot( lWorldBin, g_Base.ViewMatrix[ 2 ].xyz ) ;
		
		// 従法線、接線( ビュー空間 )を保存
		VSOutput.VTan = lViewTan;
		VSOutput.VBin = lViewBin;

	#endif	// BUMPMAP

	// ディフューズカラーをセット
	VSOutput.Diffuse  = g_Base.DiffuseSource  > 0.5f ? VSInput.Diffuse  : g_Common.Material.Diffuse ;
	
	// スペキュラカラーをセット
	VSOutput.Specular = ( g_Base.SpecularSource > 0.5f ? VSInput.Specular : g_Common.Material.Specular ) * g_Base.MulSpecularColor ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )





	// フォグ計算 =============================================( 開始 )

	#if FOG_LINEAR || FOG_EXP || FOG_EXP2

		#ifdef FOG_LINEAR

			// 線形フォグ計算
			VSOutput.Fog.x = lViewPosition.z * g_Common.Fog.LinearDiv + g_Common.Fog.LinearAdd ;

		#endif

		#ifdef FOG_EXP

			// 指数フォグ計算 1.0f / pow( e, 距離 * density )
			VSOutput.Fog.x = 1.0f / pow( abs( g_Common.Fog.E ), lViewPosition.z * g_Common.Fog.Density ) ;

		#endif

		#ifdef FOG_EXP2

			// 指数フォグ２計算 1.0f / pow( e, ( 距離 * density ) * ( 距離 * density ) )
			VSOutput.Fog.x = 1.0f / pow( abs( g_Common.Fog.E ), ( lViewPosition.z * g_Common.Fog.Density ) * ( lViewPosition.z * g_Common.Fog.Density ) ) ;

		#endif

	#else // FOG_LINEAR || FOG_EXP || FOG_EXP2
	
		VSOutput.Fog.x = 1.0f;

	#endif // FOG_LINEAR || FOG_EXP || FOG_EXP2

	VSOutput.Fog.y = 1.0f ;
	if( g_Common.VerticalFog.Mode == 1 || g_Common.VerticalFog.Mode == 2 )
	{
		if( g_Common.VerticalFog.Density < 0.0 )
		{
			lVerticalFogY = lWorldPosition.y - g_Common.VerticalFog.DensityStart;
			lFogDensity = -g_Common.VerticalFog.Density ;
		}
		else
		{
			lVerticalFogY = g_Common.VerticalFog.DensityStart - lWorldPosition.y ;
			lFogDensity = g_Common.VerticalFog.Density ;
		}
		if( lVerticalFogY > 0.0f )
		{
			if( g_Common.VerticalFog.Mode == 1 )
			{
				// 指数フォグ計算 1.0f / pow( e, 距離 * density )
				VSOutput.Fog.y = 1.0f / pow( abs( g_Common.VerticalFog.E ), lVerticalFogY * lFogDensity ) ;
			}
			else
			if( g_Common.VerticalFog.Mode == 2 )
			{
				// 指数フォグ２計算 1.0f / pow( e, ( 距離 * density ) * ( 距離 * density ) )
				VSOutput.Fog.y = 1.0f / pow( abs( g_Common.VerticalFog.E ), ( lVerticalFogY * lFogDensity ) * ( lVerticalFogY * lFogDensity ) ) ;
			}
		}
	}
	else
	if( g_Common.VerticalFog.Mode == 3 )
	{
		// 線形フォグ計算
		VSOutput.Fog.y = lWorldPosition.y * g_Common.VerticalFog.LinearDiv + g_Common.VerticalFog.LinearAdd ;
	}

	// フォグ計算 =============================================( 終了 )

#if SHADOWMAP
	// 深度影用のライトから見た射影座標を算出 =================( 開始 )

	// ワールド座標をシャドウマップ０のライト設定の射影座標に変換
	VSOutput.ShadowMap0Pos.x = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 0 ][ 0 ] ) ;
	VSOutput.ShadowMap0Pos.y = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 0 ][ 1 ] ) ;
	VSOutput.ShadowMap0Pos.z = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 0 ][ 2 ] ) ;

	// ワールド座標をシャドウマップ１のライト設定の射影座標に変換
	VSOutput.ShadowMap1Pos.x = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 1 ][ 0 ] ) ;
	VSOutput.ShadowMap1Pos.y = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 1 ][ 1 ] ) ;
	VSOutput.ShadowMap1Pos.z = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 1 ][ 2 ] ) ;

	// ワールド座標をシャドウマップ２のライト設定の射影座標に変換
	VSOutput.ShadowMap2Pos.x = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 2 ][ 0 ] ) ;
	VSOutput.ShadowMap2Pos.y = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 2 ][ 1 ] ) ;
	VSOutput.ShadowMap2Pos.z = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 2 ][ 2 ] ) ;

	// 深度影用のライトから見た射影座標を算出 =================( 終了 )
#endif

	// テクスチャ座標のセット
	lTextureCoordTemp.zw = 1.0f ;

	lTextureCoordTemp.xy = VSInput.TexCoords0 ;
	VSOutput.TexCoords0_1.x = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.y = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	lTextureCoordTemp.xy = VSInput.TexCoords1 ;
	VSOutput.TexCoords0_1.z = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.w = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	return VSOutput ;
}


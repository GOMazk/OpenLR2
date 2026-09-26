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
	float4 TexCoords0_1    : TEXCOORD0 ;	// テクスチャ座標とサブテクスチャ座標( x:テクスチャ座標x  y:テクスチャ座標y  z:サブテクスチャ座標x  w:サブテクスチャ座標y )

	float3 V_to_Eye        : TEXCOORD1 ;	// 頂点座標から視線へのベクトル( ビュー空間 )とシャドウマップ２のライトZ座標( w )
	float4 Normal_Fog      : TEXCOORD2 ;	// x.y.z:法線( ビュー空間 )  w:SM3.0用のフォグパラメータ

#if LG0_USE
	float4 Light0_Dir_Gen  : TEXCOORD3 ;	// xyz:ライト０の逆ベクトル( ビュー空間 ) w:ライト０減衰情報
#endif
#if LG1_USE
	float4 Light1_Dir_Gen  : TEXCOORD4 ;	// xyz:ライト１の逆ベクトル( ビュー空間 ) w:ライト１減衰情報
#endif
#if LG2_USE
	float4 Light2_Dir_Gen  : TEXCOORD5 ;	// xyz:ライト２の逆ベクトル( ビュー空間 ) w:ライト２減衰情報
#endif

#if SHADOWMAP || SHADOWMAP_DRAW
	float3 ShadowMap0Pos   : TEXCOORD6 ;	// シャドウマップ０のライト座標( x, y, z )
#endif
#if SHADOWMAP
	float3 ShadowMap1Pos   : TEXCOORD7 ;	// シャドウマップ１のライト座標( x, y, z )
	float3 ShadowMap2Pos   : TEXCOORD8 ;	// シャドウマップ２のライト座標( x, y, z )
#endif
	float  VerticalFog     : TEXCOORD9 ;	// 高さフォグパラメータ

	float4 Position        : SV_POSITION ;	// 座標( プロジェクション空間 )
} ;


#ifdef SKINMESH
#define LOCAL_WORLD_MAT         lLocalWorldMatrix
#else
#define LOCAL_WORLD_MAT         g_Base.LocalWorldMatrix
#endif

#define FOG_OUT                 VSOutput.Normal_Fog.w
#define VERTICAL_FOG_OUT        VSOutput.VerticalFog

// 3D用
VS_OUTPUT VS3D_Normal( VS_INPUT VSInput )
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
	float3		lBumpTemp ;
	float4		lTotalDiffuse ;
	float4		lTotalSpecular ;
	float3		lLightDir ;
	float		lTotalGen ;
	float3		lLightTemp ;
	float		lLightDistancePow2 ;
	float		lLightGen ;
	float		lLightDirectionCosA ;
	float3		lLightHalfVec ;
	float4		lLightLitParam ;
	float4		lLightLitDest ;
	float		lVerticalFogY ;
	float		lFogDensity ;
	float4		lDifColor;
	float4		lTextureCoordTemp ;
#if USE_SPE
	float4		lSpeColor;
#endif
#if SHADOWMAP
	float4		lLViewPosition ;
#endif

	lDifColor = g_Base.DiffuseSource  > 0.5f ? VSInput.Diffuse  : g_Common.Material.Diffuse ;
	
#if USE_SPE
	lSpeColor = ( g_Base.SpecularSource > 0.5f ? VSInput.Specular : g_Common.Material.Specular ) * g_Base.MulSpecularColor ;
#endif

	VSOutput.V_to_Eye   = float3( 0.0f, 0.0f, 0.0f ) ;
	VSOutput.Specular   = float4( 0.0f, 0.0f, 0.0f, 0.0f ) ;
	VSOutput.Normal_Fog = float4( 0.0f, 0.0f, 0.0f, 1.0f ) ;
	VERTICAL_FOG_OUT    = 1.0f ;

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

	// ローカル座標準備
	lLocalPosition.xyz  = VSInput.Position ;
	lLocalPosition.w    = 1.0f ;

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

	#ifdef BUMPMAP

		// 従法線、接線、法線をビュー空間に投影する
		lWorldTan.x = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldTan.y = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldTan.z = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lWorldBin.x = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldBin.y = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldBin.z = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lViewTan.x = dot( lWorldTan, g_Base.ViewMatrix[ 0 ].xyz ) ;
		lViewTan.y = dot( lWorldTan, g_Base.ViewMatrix[ 1 ].xyz ) ;
		lViewTan.z = dot( lWorldTan, g_Base.ViewMatrix[ 2 ].xyz ) ;

		lViewBin.x = dot( lWorldBin, g_Base.ViewMatrix[ 0 ].xyz ) ;
		lViewBin.y = dot( lWorldBin, g_Base.ViewMatrix[ 1 ].xyz ) ;
		lViewBin.z = dot( lWorldBin, g_Base.ViewMatrix[ 2 ].xyz ) ;

		lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
		lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
		lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

	#endif	// BUMPMAP


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	#if LG_USE == 0 || BUMPMAP == 1 || PHONG == 1

		// ライトを使用しないか、バンプマップ又はフォンシェーディングの場合はマテリアル( or 頂点 )カラーをそのまま出力
		VSOutput.Diffuse  = lDifColor ;

		#if USE_SPE
			VSOutput.Specular = lSpeColor ;
		#endif
		
		#if BUMPMAP == 0 && PHONG
		
			// 法線をビュー空間の角度に変換( LOCAL_WORLD_MAT はスキニングメッシュ の場合は r0 を使うので mov r0, 0.0f より先に実行する必要がある )
			lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
			lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
			lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

			lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
			lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
			lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

			// 法線を保存
			VSOutput.Normal_Fog.xyz = lViewNrm ;

		#endif // PHONG

	#endif // LG_USE == 0 || BUMPMAP == 1 || PHONG == 1





	// ライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
	#if LG_USE

		#ifdef BUMPMAP

			#if USE_SPE
				// 頂点座標から視点へのベクトルを接底空間に投影した後正規化して保存
				lBumpTemp.x = dot( lViewTan, -lViewPosition.xyz ) ;
				lBumpTemp.y = dot( lViewBin, -lViewPosition.xyz ) ;
				lBumpTemp.z = dot( lViewNrm, -lViewPosition.xyz ) ;
				VSOutput.V_to_Eye = normalize( lBumpTemp ) ;
			#endif

		#else // BUMPMAP 

			// 法線をビュー空間の角度に変換( LOCAL_WORLD_MAT はスキニングメッシュ の場合は r0 を使うので mov r0, 0.0f より先に実行する必要がある )
			lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
			lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
			lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

			lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
			lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
			lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

			// ディフューズカラーとスペキュラカラーの蓄積値の初期化
			lTotalDiffuse  = float4( 0, 0, 0, 0 ) ;
			lTotalSpecular = float4( 0, 0, 0, 0 ) ;

			#ifdef PHONG

				#if USE_SPE
					// 頂点座標から視点へのベクトルを正規化して保存
					VSOutput.V_to_Eye = normalize( -lViewPosition.xyz ) ;
				#endif

				// 法線を保存
				VSOutput.Normal_Fog.xyz = lViewNrm ;

			#endif // PHONG

		#endif // BUMPMAP







		// ライト0の処理 ***********************************************************************************************************************************( 開始 )
#if LG0_USE

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 開始 )

	#if LG0_DIR

		// ライトの方向計算
		lLightDir = g_Common.Light[ 0 ].Direction ;

	#else  // LG0_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( lViewPosition.xyz - g_Common.Light[ 0 ].Position ) ;

	#endif  // LG0_DIR

	// ライトの情報を初期化
	VSOutput.Light0_Dir_Gen = 0.0f ;

	// ライトの向きの逆ベクトルを接地空間に変換
	#ifdef BUMPMAP

		lLightTemp.x = dot( lViewTan, -lLightDir ) ;
		lLightTemp.y = dot( lViewBin, -lLightDir ) ;
		lLightTemp.z = dot( lViewNrm, -lLightDir ) ;
		VSOutput.Light0_Dir_Gen.xyz = normalize( lLightTemp.xyz ) ;

	#elif PHONG // BUMPMAP

		// ライトの向きの逆ベクトルを保存
		VSOutput.Light0_Dir_Gen.xyz = -lLightDir ;

	#endif // BUMPMAP

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 終了 )


	// 距離・スポットライト減衰値計算 ==========================( 開始 )
	#if LG0_SPOT || LG0_POINT

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める lLightTemp.x = ( POS - L0_POS ) * ( POS - L0_POS )
		lLightTemp = lViewPosition.xyz - g_Common.Light[ 0 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = 1.0f / ( g_Common.Light[ 0 ].Attenuation0 + g_Common.Light[ 0 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 0 ].Attenuation2 * lLightDistancePow2 ) ;

		// スポットライト減衰計算 --------

		#if LG0_SPOT

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 0 ].Direction.xyz ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 0 ].SpotParam0, 0.0f ) * g_Common.Light[ 0 ].SpotParam1 ), g_Common.Light[ 0 ].FallOff ) ) ;

		#endif // LG0_SPOT

		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 0 ].RangePow2 ) ;

		// バンプマップ or フォンシェーディング用に減衰率を保存する
		#if PHONG || BUMPMAP
			VSOutput.Light0_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

	#endif // LG0_SPOT || LG0_POINT
	// 距離・スポットライト減衰値計算 =========================( 終了 )




	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 開始 )
	#if BUMPMAP == 0 && PHONG == 0

		// 法線とライトの逆方向ベクトルとの内積を lLightLitParam.x にセット
		lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

		#ifdef USE_SPE

			// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
			lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

			// 法線とハーフベクトルの内積を lLightLitParam.y にセット
			lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

			// スペキュラ反射率を lLightLitParam.w にセット
			lLightLitParam.w = g_Common.Material.Power ;

			// ライト計算
			lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

		#else // USE_SPE

			lLightLitDest.y = max( lLightLitParam.x, 0.0f ) ;

		#endif // USE_SPE

	#endif // BUMPMAP == 0 && PHONG == 0
	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 終了 )





	// ディフューズカラー計算 =======================================( 開始 )

	#if BUMPMAP == 0 && PHONG == 0

		#if LG0_SPOT || LG0_POINT

			// ディフューズライト蓄積値 += 距離・スポットライト角度減衰値 * ( ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの )
			lTotalDiffuse.xyz += lLightGen * ( lLightLitDest.y * g_Common.Light[ 0 ].Diffuse * lDifColor.xyz + g_Common.Light[ 0 ].Ambient.xyz ) ;

		#else  // LG0_SPOT || LG0_POINT

			// ディフューズライト蓄積値 += ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
			lTotalDiffuse.xyz += lLightLitDest.y * g_Common.Light[ 0 ].Diffuse * lDifColor.xyz + g_Common.Light[ 0 ].Ambient.xyz ;

		#endif // LG0_SPOT || LG0_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// ディフューズカラー計算 =======================================( 終了 )





	// スペキュラカラー計算 =======================================( 開始 )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG0_SPOT || LG0_POINT

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * 距離・スポットライト減衰 * ライトのスペキュラカラー
				lTotalSpecular.xyz += lLightGen * lLightLitDest.z * g_Common.Light[ 0 ].Specular ;

			#else	// LG0_SPOT || LG0_POINT

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
				lTotalSpecular.xyz += lLightLitDest.z * g_Common.Light[ 0 ].Specular ;

			#endif	// LG0_SPOT || LG0_POINT

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// スペキュラカラー計算 =======================================( 終了 )




#endif  // LG0_USE
// ライト0の処理 ***********************************************************************************************************************************( 終了 )








		// ライト1の処理 ***********************************************************************************************************************************( 開始 )
#if LG1_USE

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 開始 )

	#if LG1_DIR

		// ライトの方向計算
		lLightDir = g_Common.Light[ 1 ].Direction ;

	#else  // LG1_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( lViewPosition.xyz - g_Common.Light[ 1 ].Position ) ;

	#endif  // LG1_DIR

	// ライトの情報を初期化
	VSOutput.Light1_Dir_Gen = 0.0f ;

	// ライトの向きの逆ベクトルを接地空間に変換
	#ifdef BUMPMAP

		lLightTemp.x = dot( lViewTan, -lLightDir ) ;
		lLightTemp.y = dot( lViewBin, -lLightDir ) ;
		lLightTemp.z = dot( lViewNrm, -lLightDir ) ;
		VSOutput.Light1_Dir_Gen.xyz = normalize( lLightTemp.xyz ) ;

	#elif PHONG // BUMPMAP

		// ライトの向きの逆ベクトルを保存
		VSOutput.Light1_Dir_Gen.xyz = -lLightDir ;

	#endif // BUMPMAP

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 終了 )


	// 距離・スポットライト減衰値計算 ==========================( 開始 )
	#if LG1_SPOT || LG1_POINT

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める lLightTemp.x = ( POS - L1_POS ) * ( POS - L1_POS )
		lLightTemp = lViewPosition.xyz - g_Common.Light[ 1 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = 1.0f / ( g_Common.Light[ 1 ].Attenuation0 + g_Common.Light[ 1 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 1 ].Attenuation2 * lLightDistancePow2 ) ;

		// スポットライト減衰計算 --------

		#if LG1_SPOT

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 1 ].Direction.xyz ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 1 ].SpotParam0, 0.0f ) * g_Common.Light[ 1 ].SpotParam1 ), g_Common.Light[ 1 ].FallOff ) ) ;

		#endif // LG1_SPOT

		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 1 ].RangePow2 ) ;

		// バンプマップ or フォンシェーディング用に減衰率を保存する
		#if PHONG || BUMPMAP
			VSOutput.Light1_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

	#endif // LG1_SPOT || LG1_POINT
	// 距離・スポットライト減衰値計算 =========================( 終了 )




	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 開始 )
	#if BUMPMAP == 0 && PHONG == 0

		// 法線とライトの逆方向ベクトルとの内積を lLightLitParam.x にセット
		lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

		#ifdef USE_SPE

			// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
			lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

			// 法線とハーフベクトルの内積を lLightLitParam.y にセット
			lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

			// スペキュラ反射率を lLightLitParam.w にセット
			lLightLitParam.w = g_Common.Material.Power ;

			// ライト計算
			lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

		#else // USE_SPE

			lLightLitDest.y = max( lLightLitParam.x, 0.0f ) ;

		#endif // USE_SPE

	#endif // BUMPMAP == 0 && PHONG == 0
	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 終了 )





	// ディフューズカラー計算 =======================================( 開始 )

	#if BUMPMAP == 0 && PHONG == 0

		#if LG1_SPOT || LG1_POINT

			// ディフューズライト蓄積値 += 距離・スポットライト角度減衰値 * ( ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの )
			lTotalDiffuse.xyz += lLightGen * ( lLightLitDest.y * g_Common.Light[ 1 ].Diffuse * lDifColor.xyz + g_Common.Light[ 1 ].Ambient.xyz ) ;

		#else  // LG1_SPOT || LG1_POINT

			// ディフューズライト蓄積値 += ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
			lTotalDiffuse.xyz += lLightLitDest.y * g_Common.Light[ 1 ].Diffuse * lDifColor.xyz + g_Common.Light[ 1 ].Ambient.xyz ;

		#endif // LG1_SPOT || LG1_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// ディフューズカラー計算 =======================================( 終了 )





	// スペキュラカラー計算 =======================================( 開始 )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG1_SPOT || LG1_POINT

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * 距離・スポットライト減衰 * ライトのスペキュラカラー
				lTotalSpecular.xyz += lLightGen * lLightLitDest.z * g_Common.Light[ 1 ].Specular ;

			#else	// LG1_SPOT || LG1_POINT

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
				lTotalSpecular.xyz += lLightLitDest.z * g_Common.Light[ 1 ].Specular ;

			#endif	// LG1_SPOT || LG1_POINT

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// スペキュラカラー計算 =======================================( 終了 )




#endif  // LG1_USE
// ライト1の処理 ***********************************************************************************************************************************( 終了 )








		// ライト2の処理 ***********************************************************************************************************************************( 開始 )
#if LG2_USE

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 開始 )

	#if LG2_DIR

		// ライトの方向計算
		lLightDir = g_Common.Light[ 2 ].Direction ;

	#else  // LG2_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( lViewPosition.xyz - g_Common.Light[ 2 ].Position ) ;

	#endif  // LG2_DIR

	// ライトの情報を初期化
	VSOutput.Light2_Dir_Gen = 0.0f ;

	// ライトの向きの逆ベクトルを接地空間に変換
	#ifdef BUMPMAP

		lLightTemp.x = dot( lViewTan, -lLightDir ) ;
		lLightTemp.y = dot( lViewBin, -lLightDir ) ;
		lLightTemp.z = dot( lViewNrm, -lLightDir ) ;
		VSOutput.Light2_Dir_Gen.xyz = normalize( lLightTemp.xyz ) ;

	#elif PHONG // BUMPMAP

		// ライトの向きの逆ベクトルを保存
		VSOutput.Light2_Dir_Gen.xyz = -lLightDir ;

	#endif // BUMPMAP

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 終了 )


	// 距離・スポットライト減衰値計算 ==========================( 開始 )
	#if LG2_SPOT || LG2_POINT

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める lLightTemp.x = ( POS - L2_POS ) * ( POS - L2_POS )
		lLightTemp = lViewPosition.xyz - g_Common.Light[ 2 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = 1.0f / ( g_Common.Light[ 2 ].Attenuation0 + g_Common.Light[ 2 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 2 ].Attenuation2 * lLightDistancePow2 ) ;

		// スポットライト減衰計算 --------

		#if LG2_SPOT

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 2 ].Direction.xyz ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 2 ].SpotParam0, 0.0f ) * g_Common.Light[ 2 ].SpotParam1 ), g_Common.Light[ 2 ].FallOff ) ) ;

		#endif // LG2_SPOT

		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 2 ].RangePow2 ) ;

		// バンプマップ or フォンシェーディング用に減衰率を保存する
		#if PHONG || BUMPMAP
			VSOutput.Light2_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

	#endif // LG2_SPOT || LG2_POINT
	// 距離・スポットライト減衰値計算 =========================( 終了 )




	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 開始 )
	#if BUMPMAP == 0 && PHONG == 0

		// 法線とライトの逆方向ベクトルとの内積を lLightLitParam.x にセット
		lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

		#ifdef USE_SPE

			// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
			lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

			// 法線とハーフベクトルの内積を lLightLitParam.y にセット
			lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

			// スペキュラ反射率を lLightLitParam.w にセット
			lLightLitParam.w = g_Common.Material.Power ;

			// ライト計算
			lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

		#else // USE_SPE

			lLightLitDest.y = max( lLightLitParam.x, 0.0f ) ;

		#endif // USE_SPE

	#endif // BUMPMAP == 0 && PHONG == 0
	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 終了 )





	// ディフューズカラー計算 =======================================( 開始 )

	#if BUMPMAP == 0 && PHONG == 0

		#if LG2_SPOT || LG2_POINT

			// ディフューズライト蓄積値 += 距離・スポットライト角度減衰値 * ( ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの )
			lTotalDiffuse.xyz += lLightGen * ( lLightLitDest.y * g_Common.Light[ 2 ].Diffuse * lDifColor.xyz + g_Common.Light[ 2 ].Ambient.xyz ) ;

		#else  // LG2_SPOT || LG2_POINT

			// ディフューズライト蓄積値 += ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
			lTotalDiffuse.xyz += lLightLitDest.y * g_Common.Light[ 2 ].Diffuse * lDifColor.xyz + g_Common.Light[ 2 ].Ambient.xyz ;

		#endif // LG2_SPOT || LG2_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// ディフューズカラー計算 =======================================( 終了 )





	// スペキュラカラー計算 =======================================( 開始 )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG2_SPOT || LG2_POINT

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * 距離・スポットライト減衰 * ライトのスペキュラカラー
				lTotalSpecular.xyz += lLightGen * lLightLitDest.z * g_Common.Light[ 2 ].Specular ;

			#else	// LG2_SPOT || LG2_POINT

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
				lTotalSpecular.xyz += lLightLitDest.z * g_Common.Light[ 2 ].Specular ;

			#endif	// LG2_SPOT || LG2_POINT

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// スペキュラカラー計算 =======================================( 終了 )




#endif  // LG2_USE
// ライト2の処理 ***********************************************************************************************************************************( 終了 )















		// 全てのディフューズ要素を合わせて出力レジスタにセット =====( 開始 )

		#if BUMPMAP == 0 && PHONG == 0

			// 出力ディフューズカラー = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
			VSOutput.Diffuse = lTotalDiffuse + g_Common.Material.Ambient_Emissive ;

			// アルファはディフューズカラーのアルファをそのまま使う
			VSOutput.Diffuse.w = lDifColor.w ;

		#endif // BUMPMAP == 0 && PHONG == 0

		// 全てのディフューズ要素を合わせて出力レジスタにセット =====( 終了 )





		// 全てのスペキュラ要素を合わせて出力レジスタにセット =====( 開始 )

		#if USE_SPE && BUMPMAP == 0 && PHONG == 0

			// 出力スペキュラカラー = ライトスペキュラカラー蓄積値 * スペキュラカラー
			VSOutput.Specular = lTotalSpecular * lSpeColor ;

		#endif // USE_SPE && BUMPMAP == 0 && PHONG == 0

		// 全てのスペキュラ要素を合わせて出力レジスタにセット =====( 終了 )






	#endif // LG_USE
	// ライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )















	// フォグ計算 =============================================( 開始 )

	#if FOG_LINEAR || FOG_EXP || FOG_EXP2

		#ifdef FOG_LINEAR

			// 線形フォグ計算
			FOG_OUT = lViewPosition.z * g_Common.Fog.LinearDiv + g_Common.Fog.LinearAdd ;

		#endif

		#ifdef FOG_EXP

			// 指数フォグ計算 1.0f / pow( e, 距離 * density )
			FOG_OUT = 1.0f / pow( abs( g_Common.Fog.E ), lViewPosition.z * g_Common.Fog.Density ) ;

		#endif

		#ifdef FOG_EXP2

			// 指数フォグ２計算 1.0f / pow( e, ( 距離 * density ) * ( 距離 * density ) )
			FOG_OUT = 1.0f / pow( abs( g_Common.Fog.E ), ( lViewPosition.z * g_Common.Fog.Density ) * ( lViewPosition.z * g_Common.Fog.Density ) ) ;

		#endif

	#endif // FOG_LINEAR || FOG_EXP || FOG_EXP2

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
				VERTICAL_FOG_OUT = 1.0f / pow( abs( g_Common.VerticalFog.E ), lVerticalFogY * lFogDensity ) ;
			}
			else
			if( g_Common.VerticalFog.Mode == 2 )
			{
				// 指数フォグ２計算 1.0f / pow( e, ( 距離 * density ) * ( 距離 * density ) )
				VERTICAL_FOG_OUT = 1.0f / pow( abs( g_Common.VerticalFog.E ), ( lVerticalFogY * lFogDensity ) * ( lVerticalFogY * lFogDensity ) ) ;
			}
		}
	}
	else
	if( g_Common.VerticalFog.Mode == 3 )
	{
		// 線形フォグ計算
		VERTICAL_FOG_OUT = lWorldPosition.y * g_Common.VerticalFog.LinearDiv + g_Common.VerticalFog.LinearAdd ;
	}

	// フォグ計算 =============================================( 終了 )

#if SHADOWMAP_DRAW
	// 射影座標をテクスチャ座標として出力する =================( 開始 )

	VSOutput.ShadowMap0Pos = VSOutput.Position.xyz ;

	// 射影座標をテクスチャ座標として出力する =================( 終了 )
#endif


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

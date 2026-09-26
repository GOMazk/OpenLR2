#include "../VertexLighting_VS.h"

// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	int4 lBoneFloatIndex ;
	float4 lLocalWorldMatrix[ 3 ] ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float3 lWorldNrm ;
	float3 lWorldTan ;
	float3 lWorldBin ;
	float3 lViewNrm ;
	float3 lViewTan ;
	float3 lViewBin ;
	float3 lBumpTemp ;
	float4 lTotalDiffuse ;
	float4 lTotalSpecular ;
	float3 lLightDir ;
	float lTotalGen ;
	float4 lLightTemp ;
	float lLightDistancePow2 ;
	float lLightGen ;
	float lLightDirectionCosA ;
	float3 lLightHalfVec ;
	float4 lLightLitParam ;
	float4 lLightLitDest ;
	float4 lDifColor;
#if USE_SPE
	float4 lSpeColor;
#endif
#if SHADOWMAP
	float4 lLViewPosition ;
#endif

#if ( ( BUMPMAP || PHONG ) && USE_SPE ) || SHADOWMAP
	VSOutput.V_to_Eye_ShadowMap3PosZ.xyzw = cfZeroOne.x;
#endif
	
	lDifColor = cfDifSpeSource.x > 0.5f ? VSInput.Diffuse : cfMaterial.Diffuse;
	
#if USE_SPE
	lSpeColor = cfDifSpeSource.y > 0.5f ? VSInput.Specular : cfMaterial.Specular;
#endif

#if ( BUMPMAP == 0 && PHONG ) || SM_3
	VSOutput.Normal_Fog = float4( 0.0f, 0.0f, 0.0f, 1.0f ) ;
#endif

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	#ifdef SKINMESH

		// スキンメッシュ

		// ブレンド行列の作成
		lBoneFloatIndex = VSInput.BlendIndices0 ;
		lLocalWorldMatrix[ 0 ]  = cfLocalWorldMatrix[ lBoneFloatIndex.x + 0 ] * VSInput.BlendWeight0.xxxx;
		lLocalWorldMatrix[ 1 ]  = cfLocalWorldMatrix[ lBoneFloatIndex.x + 1 ] * VSInput.BlendWeight0.xxxx;
		lLocalWorldMatrix[ 2 ]  = cfLocalWorldMatrix[ lBoneFloatIndex.x + 2 ] * VSInput.BlendWeight0.xxxx;

		lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 0 ] * VSInput.BlendWeight0.yyyy;
		lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 1 ] * VSInput.BlendWeight0.yyyy;
		lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 2 ] * VSInput.BlendWeight0.yyyy;

		lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 0 ] * VSInput.BlendWeight0.zzzz;
		lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 1 ] * VSInput.BlendWeight0.zzzz;
		lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 2 ] * VSInput.BlendWeight0.zzzz;

		lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 0 ] * VSInput.BlendWeight0.wwww;
		lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 1 ] * VSInput.BlendWeight0.wwww;
		lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 2 ] * VSInput.BlendWeight0.wwww;

		#ifdef BONE8

			lBoneFloatIndex = VSInput.BlendIndices1 ;
			lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.x + 0 ] * VSInput.BlendWeight1.xxxx;
			lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.x + 1 ] * VSInput.BlendWeight1.xxxx;
			lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.x + 2 ] * VSInput.BlendWeight1.xxxx;

			lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 0 ] * VSInput.BlendWeight1.yyyy;
			lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 1 ] * VSInput.BlendWeight1.yyyy;
			lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 2 ] * VSInput.BlendWeight1.yyyy;

			lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 0 ] * VSInput.BlendWeight1.zzzz;
			lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 1 ] * VSInput.BlendWeight1.zzzz;
			lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 2 ] * VSInput.BlendWeight1.zzzz;

			lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 0 ] * VSInput.BlendWeight1.wwww;
			lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 1 ] * VSInput.BlendWeight1.wwww;
			lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 2 ] * VSInput.BlendWeight1.wwww;

		#endif // BONE8

	#endif	// SKINMESH

	#ifdef TOONOUTLINE

		// 座標計算( ローカル→ワールド )
		lWorldPosition.x = dot( VSInput.Position, LOCAL_WORLD_MAT[ 0 ] ) ;
		lWorldPosition.y = dot( VSInput.Position, LOCAL_WORLD_MAT[ 1 ] ) ;
		lWorldPosition.z = dot( VSInput.Position, LOCAL_WORLD_MAT[ 2 ] ) ;
		lWorldPosition.w = 1.0f ;

		lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		// 輪郭線の太さ分法線方向に移動する
		lWorldPosition.xyz += lWorldNrm * cfToonOutLineSize.x ;

		// ワールド→ビュー
		lViewPosition.x = dot( lWorldPosition, cfViewMatrix[ 0 ] ) ;
		lViewPosition.y = dot( lWorldPosition, cfViewMatrix[ 1 ] ) ;
		lViewPosition.z = dot( lWorldPosition, cfViewMatrix[ 2 ] ) ;
		lViewPosition.w = 1.0f ;

		// ビュー→プロジェクション
		VSOutput.Position.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
		VSOutput.Position.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
		VSOutput.Position.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
		VSOutput.Position.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;

	#else  // TOONOUTLINE

		// 座標計算( ローカル→ビュー→プロジェクション )
		lWorldPosition.x = dot( VSInput.Position, LOCAL_WORLD_MAT[ 0 ] ) ;
		lWorldPosition.y = dot( VSInput.Position, LOCAL_WORLD_MAT[ 1 ] ) ;
		lWorldPosition.z = dot( VSInput.Position, LOCAL_WORLD_MAT[ 2 ] ) ;
		lWorldPosition.w = 1.0f ;

		lViewPosition.x = dot( lWorldPosition, cfViewMatrix[ 0 ] ) ;
		lViewPosition.y = dot( lWorldPosition, cfViewMatrix[ 1 ] ) ;
		lViewPosition.z = dot( lWorldPosition, cfViewMatrix[ 2 ] ) ;
		lViewPosition.w = 1.0f ;

		VSOutput.Position.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
		VSOutput.Position.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
		VSOutput.Position.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
		VSOutput.Position.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;

	#endif // TOONOUTLINE

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

		lViewTan.x = dot( lWorldTan, cfViewMatrix[ 0 ].xyz ) ;
		lViewTan.y = dot( lWorldTan, cfViewMatrix[ 1 ].xyz ) ;
		lViewTan.z = dot( lWorldTan, cfViewMatrix[ 2 ].xyz ) ;

		lViewBin.x = dot( lWorldBin, cfViewMatrix[ 0 ].xyz ) ;
		lViewBin.y = dot( lWorldBin, cfViewMatrix[ 1 ].xyz ) ;
		lViewBin.z = dot( lWorldBin, cfViewMatrix[ 2 ].xyz ) ;

		lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
		lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
		lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

	#endif	// BUMPMAP


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	#if LG_USE == 0 || BUMPMAP == 1 || PHONG == 1

		// ライトを使用しないか、バンプマップ又はフォンシェーディングの場合はマテリアル( or 頂点 )カラーをそのまま出力
		VSOutput.Diffuse  = lDifColor ;

		#if USE_SPE
			VSOutput.Specular = lSpeColor ;
		#endif
		
		#if BUMPMAP == 0 && PHONG
		
			// 法線をビュー空間の角度に変換( LOCAL_WORLD_MAT はスキニングメッシュ の場合は r0 を使うので mov r0, ZERO より先に実行する必要がある )
			lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
			lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
			lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

			lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
			lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
			lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

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
				VSOutput.V_to_Eye_ShadowMap3PosZ.xyz = normalize( lBumpTemp ) ;
			#endif

		#else // BUMPMAP 

			// 法線をビュー空間の角度に変換( LOCAL_WORLD_MAT はスキニングメッシュ の場合は r0 を使うので mov r0, ZERO より先に実行する必要がある )
			lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
			lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
			lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

			lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
			lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
			lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

			// ディフューズカラーとスペキュラカラーの蓄積値の初期化
			lTotalDiffuse = float4( 0, 0, 0, 0 ) ;
			lTotalSpecular = float4( 0, 0, 0, 0 ) ;

			#ifdef PHONG

				#if USE_SPE
					// 頂点座標から視点へのベクトルを正規化して保存
					VSOutput.V_to_Eye_ShadowMap3PosZ.xyz = normalize( -lViewPosition.xyz ) ;
				#endif

				// 法線を保存
				VSOutput.Normal_Fog.xyz = lViewNrm ;

			#endif // PHONG

		#endif // BUMPMAP







		// ライト0の処理 ***********************************************************************************************************************************( 開始 )
#if LG0_USE

	#ifdef ALL_VS
		if( cbLightBool[ 0 ].Use )
		{
	#endif

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 開始 )

	#if LG0_DIR

		// ライトの方向計算
		lLightDir = cfLight[ 0 ].Direction ;

	#else  // LG0_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( lViewPosition.xyz - cfLight[ 0 ].Position.xyz ) ;

	#endif  // LG0_DIR

	// ライトの情報を初期化
	VSOutput.Light0_Dir_Gen = ZERO ;

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

		#ifdef ALL_VS
			lTotalGen = 1.0f;
			if( cbLightBool[ 0 ].Point_Or_Spot )
			{
		#endif // ALL_VS

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める lLightTemp.x = ( POS - L0_POS ) * ( POS - L0_POS )
		lLightTemp = lViewPosition - cfLight[ 0 ].Position ;
		lLightTemp.w = 0.0f;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = 1.0f / ( cfLight[ 0 ].Range_FallOff_AT0_AT1.z + cfLight[ 0 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 0 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// スポットライト減衰計算 --------

		#if LG0_SPOT

			#ifdef ALL_VS
				if( cbLightBool[ 0 ].Spot )
				{
			#endif // ALL_VS

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 0 ].Direction ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 0 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 0 ].AT2_SpotP0_SpotP1.z ), cfLight[ 0 ].Range_FallOff_AT0_AT1.y ) ) ;

			#ifdef ALL_VS
				} // if( cbLightBool[ 0 ].Spot )
			#endif

		#endif // LG0_SPOT

		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, cfLight[ 0 ].Range_FallOff_AT0_AT1.x ) ;

		// バンプマップ or フォンシェーディング用に減衰率を保存する
		#if PHONG || BUMPMAP
			VSOutput.Light0_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

		#ifdef ALL_VS
			} // if( cbLightBool[ 0 ].Point_Or_Spot )
		#endif

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
			lLightLitParam.w = cfMaterial.Power.x ;

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
			lTotalDiffuse += lLightGen * ( lLightLitDest.y * cfLight[ 0 ].Diffuse * lDifColor + cfLight[ 0 ].Ambient ) ;

		#else  // LG0_SPOT || LG0_POINT

			// ディフューズライト蓄積値 += ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
			lTotalDiffuse += lLightLitDest.y * cfLight[ 0 ].Diffuse * lDifColor + cfLight[ 0 ].Ambient ;

		#endif // LG0_SPOT || LG0_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// ディフューズカラー計算 =======================================( 終了 )





	// スペキュラカラー計算 =======================================( 開始 )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG0_SPOT || LG0_POINT || ALL_VS

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * 距離・スポットライト減衰 * ライトのスペキュラカラー
				lTotalSpecular += lLightGen * lLightLitDest.z * cfLight[ 0 ].Specular ;

			#else	// LG0_SPOT || LG0_POINT || ALL_VS

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
				lTotalSpecular += lLightLitDest.z * cfLight[ 0 ].Specular ;

			#endif	// LG0_SPOT || LG0_POINT || ALL_VS

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// スペキュラカラー計算 =======================================( 終了 )




	#ifdef ALL_VS
		} // if( cbLightBool[ 0 ].Use )
	#endif

#endif  // LG0_USE
// ライト0の処理 ***********************************************************************************************************************************( 終了 )








		// ライト1の処理 ***********************************************************************************************************************************( 開始 )
#if LG1_USE

	#ifdef ALL_VS
		if( cbLightBool[ 1 ].Use )
		{
	#endif

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 開始 )

	#if LG1_DIR

		// ライトの方向計算
		lLightDir = cfLight[ 1 ].Direction ;

	#else  // LG1_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( lViewPosition.xyz - cfLight[ 1 ].Position.xyz ) ;

	#endif  // LG1_DIR

	// ライトの情報を初期化
	VSOutput.Light1_Dir_Gen = ZERO ;

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

		#ifdef ALL_VS
			lTotalGen = 1.0f;
			if( cbLightBool[ 1 ].Point_Or_Spot )
			{
		#endif // ALL_VS

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める lLightTemp.x = ( POS - L1_POS ) * ( POS - L1_POS )
		lLightTemp = lViewPosition - cfLight[ 1 ].Position ;
		lLightTemp.w = 0.0f;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = 1.0f / ( cfLight[ 1 ].Range_FallOff_AT0_AT1.z + cfLight[ 1 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 1 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// スポットライト減衰計算 --------

		#if LG1_SPOT

			#ifdef ALL_VS
				if( cbLightBool[ 1 ].Spot )
				{
			#endif // ALL_VS

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 1 ].Direction ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 1 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 1 ].AT2_SpotP0_SpotP1.z ), cfLight[ 1 ].Range_FallOff_AT0_AT1.y ) ) ;

			#ifdef ALL_VS
				} // if( cbLightBool[ 1 ].Spot )
			#endif

		#endif // LG1_SPOT

		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, cfLight[ 1 ].Range_FallOff_AT0_AT1.x ) ;

		// バンプマップ or フォンシェーディング用に減衰率を保存する
		#if PHONG || BUMPMAP
			VSOutput.Light1_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

		#ifdef ALL_VS
			} // if( cbLightBool[ 1 ].Point_Or_Spot )
		#endif

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
			lLightLitParam.w = cfMaterial.Power.x ;

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
			lTotalDiffuse += lLightGen * ( lLightLitDest.y * cfLight[ 1 ].Diffuse * lDifColor + cfLight[ 1 ].Ambient ) ;

		#else  // LG1_SPOT || LG1_POINT

			// ディフューズライト蓄積値 += ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
			lTotalDiffuse += lLightLitDest.y * cfLight[ 1 ].Diffuse * lDifColor + cfLight[ 1 ].Ambient ;

		#endif // LG1_SPOT || LG1_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// ディフューズカラー計算 =======================================( 終了 )





	// スペキュラカラー計算 =======================================( 開始 )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG1_SPOT || LG1_POINT || ALL_VS

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * 距離・スポットライト減衰 * ライトのスペキュラカラー
				lTotalSpecular += lLightGen * lLightLitDest.z * cfLight[ 1 ].Specular ;

			#else	// LG1_SPOT || LG1_POINT || ALL_VS

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
				lTotalSpecular += lLightLitDest.z * cfLight[ 1 ].Specular ;

			#endif	// LG1_SPOT || LG1_POINT || ALL_VS

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// スペキュラカラー計算 =======================================( 終了 )




	#ifdef ALL_VS
		} // if( cbLightBool[ 1 ].Use )
	#endif

#endif  // LG1_USE
// ライト1の処理 ***********************************************************************************************************************************( 終了 )








		// ライト2の処理 ***********************************************************************************************************************************( 開始 )
#if LG2_USE

	#ifdef ALL_VS
		if( cbLightBool[ 2 ].Use )
		{
	#endif

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 開始 )

	#if LG2_DIR

		// ライトの方向計算
		lLightDir = cfLight[ 2 ].Direction ;

	#else  // LG2_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( lViewPosition.xyz - cfLight[ 2 ].Position.xyz ) ;

	#endif  // LG2_DIR

	// ライトの情報を初期化
	VSOutput.Light2_Dir_Gen = ZERO ;

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

		#ifdef ALL_VS
			lTotalGen = 1.0f;
			if( cbLightBool[ 2 ].Point_Or_Spot )
			{
		#endif // ALL_VS

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める lLightTemp.x = ( POS - L2_POS ) * ( POS - L2_POS )
		lLightTemp = lViewPosition - cfLight[ 2 ].Position ;
		lLightTemp.w = 0.0f;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = 1.0f / ( cfLight[ 2 ].Range_FallOff_AT0_AT1.z + cfLight[ 2 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 2 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// スポットライト減衰計算 --------

		#if LG2_SPOT

			#ifdef ALL_VS
				if( cbLightBool[ 2 ].Spot )
				{
			#endif // ALL_VS

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 2 ].Direction ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 2 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 2 ].AT2_SpotP0_SpotP1.z ), cfLight[ 2 ].Range_FallOff_AT0_AT1.y ) ) ;

			#ifdef ALL_VS
				} // if( cbLightBool[ 2 ].Spot )
			#endif

		#endif // LG2_SPOT

		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, cfLight[ 2 ].Range_FallOff_AT0_AT1.x ) ;

		// バンプマップ or フォンシェーディング用に減衰率を保存する
		#if PHONG || BUMPMAP
			VSOutput.Light2_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

		#ifdef ALL_VS
			} // if( cbLightBool[ 2 ].Point_Or_Spot )
		#endif

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
			lLightLitParam.w = cfMaterial.Power.x ;

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
			lTotalDiffuse += lLightGen * ( lLightLitDest.y * cfLight[ 2 ].Diffuse * lDifColor + cfLight[ 2 ].Ambient ) ;

		#else  // LG2_SPOT || LG2_POINT

			// ディフューズライト蓄積値 += ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
			lTotalDiffuse += lLightLitDest.y * cfLight[ 2 ].Diffuse * lDifColor + cfLight[ 2 ].Ambient ;

		#endif // LG2_SPOT || LG2_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// ディフューズカラー計算 =======================================( 終了 )





	// スペキュラカラー計算 =======================================( 開始 )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG2_SPOT || LG2_POINT || ALL_VS

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * 距離・スポットライト減衰 * ライトのスペキュラカラー
				lTotalSpecular += lLightGen * lLightLitDest.z * cfLight[ 2 ].Specular ;

			#else	// LG2_SPOT || LG2_POINT || ALL_VS

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
				lTotalSpecular += lLightLitDest.z * cfLight[ 2 ].Specular ;

			#endif	// LG2_SPOT || LG2_POINT || ALL_VS

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// スペキュラカラー計算 =======================================( 終了 )




	#ifdef ALL_VS
		} // if( cbLightBool[ 2 ].Use )
	#endif

#endif  // LG2_USE
// ライト2の処理 ***********************************************************************************************************************************( 終了 )















		// 全てのディフューズ要素を合わせて出力レジスタにセット =====( 開始 )

		#if BUMPMAP == 0 && PHONG == 0

			// 出力ディフューズカラー = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
			VSOutput.Diffuse = lTotalDiffuse + cfAmbient_Emissive ;

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
			FOG_OUT = lViewPosition.z * FOG_LINEAR_DIV + FOG_LINEAR_ADD ;

		#endif

		#ifdef FOG_EXP

			// 指数フォグ計算 1.0f / pow( e, 距離 * density )
			FOG_OUT = 1.0f / pow( abs( FOG_EXP_E ), lViewPosition.z * FOG_EXP_DENSITY ) ;

		#endif

		#ifdef FOG_EXP2

			// 指数フォグ２計算 1.0f / pow( e, ( 距離 * density ) * ( 距離 * density ) )
			FOG_OUT = 1.0f / pow( abs( FOG_EXP_E ), ( lViewPosition.z * FOG_EXP_DENSITY ) * ( lViewPosition.z * FOG_EXP_DENSITY ) ) ;

		#endif

	#endif // FOG_LINEAR || FOG_EXP || FOG_EXP2

	// フォグ計算 =============================================( 終了 )

#if SHADOWMAP_DRAW
	// 射影座標をテクスチャ座標として出力する =================( 開始 )

	VSOutput.ShadowMap1Pos_ShadowMap3PosX.xyz = VSOutput.Position.xyz ;
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.w   = cfZeroOne.y;

	// 射影座標をテクスチャ座標として出力する =================( 終了 )
#endif


#if SHADOWMAP
	// 深度影用のライトから見た射影座標を算出 =================( 開始 )

	// ワールド座標をライト１の射影座標に変換
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.x = dot( lWorldPosition, cfShadowMap1LightViewProjectionMatrix[ 0 ] ) ;
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.y = dot( lWorldPosition, cfShadowMap1LightViewProjectionMatrix[ 1 ] ) ;
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.z = dot( lWorldPosition, cfShadowMap1LightViewProjectionMatrix[ 2 ] ) ;

	// ワールド座標をライト２の射影座標に変換
	VSOutput.ShadowMap2Pos_ShadowMap3PosY.x = dot( lWorldPosition, cfShadowMap2LightViewProjectionMatrix[ 0 ] ) ;
	VSOutput.ShadowMap2Pos_ShadowMap3PosY.y = dot( lWorldPosition, cfShadowMap2LightViewProjectionMatrix[ 1 ] ) ;
	VSOutput.ShadowMap2Pos_ShadowMap3PosY.z = dot( lWorldPosition, cfShadowMap2LightViewProjectionMatrix[ 2 ] ) ;

	// ワールド座標をライト３の射影座標に変換
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.w = dot( lWorldPosition, cfShadowMap3LightViewProjectionMatrix[ 0 ] ) ;
	VSOutput.ShadowMap2Pos_ShadowMap3PosY.w = dot( lWorldPosition, cfShadowMap3LightViewProjectionMatrix[ 1 ] ) ;
	VSOutput.V_to_Eye_ShadowMap3PosZ.w      = dot( lWorldPosition, cfShadowMap3LightViewProjectionMatrix[ 2 ] ) ;

	// 深度影用のライトから見た射影座標を算出 =================( 終了 )
#endif

	// テクスチャ座標のセット
	VSOutput.TexCoords0_1.x = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.y = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 1 ] ) ;

//#ifdef SUBTEXTURE
	VSOutput.TexCoords0_1.z = dot( VSInput.TexCoords1, cfTextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.w = dot( VSInput.TexCoords1, cfTextureMatrix[ 0 ][ 1 ] ) ;
//#endif // SUBTEXTURE


	return VSOutput ;
}


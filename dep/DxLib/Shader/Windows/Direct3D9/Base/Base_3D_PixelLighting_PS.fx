#include "../PixelLighting_PS.h"

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float4 TextureSpecularColor ;
	float4 ToonSpecularGradColor ;
	float4 ToonDiffuseGradColor ;
	float4 ToonSphereMapColor ;
	float4 ToonRGBtoVMaxRGBVolumeColor ;
	
	float DiffuseAngleGen ;
	float4 TotalDiffuse ;
	float4 TotalSpecular ;
	float4 TotalLightDiffuse ;
	float3 TotalAmbient ;
	float TotalAngleGen ;
	float Temp ;
	float3 TempF3 ;
	float4 TempF4 ;
	float3 Normal ;
	float3 V_to_Eye ;
	float3 VNrm ;
	float3 VTan ;
	float3 VBin ;
	float3 lLightTemp ;
	float lLightDot ;
	float lLightDistancePow2 ;
	float lLightGen ;
	float3 lLightDir ;
	float lLightDirectionCosA ;
	float3 ShadowRate ;
	float ShadowGen ;

#if SHADOWMAP
	float2 DepthTexCoord ;
	float4 TextureDepth ;
//	float4 variance_md_p_pmax ;
#endif
	
#if SUBTEXTUREMODE != 0
	float4 SubTextureColor;
#endif  // SUBTEXTUREMODE != 0

	// 接線・従法線・法線を正規化
	VNrm = normalize( PSInput.VNormal ) ;
#ifdef BUMPMAP
	VTan = normalize( PSInput.VTan ) ;
	VBin = normalize( PSInput.VBin ) ;
#endif // BUMPMAP

#ifdef BUMPMAP
	// 頂点座標から視点へのベクトルを接底空間に投影した後正規化して保存
	TempF3.x = dot( VTan, -PSInput.VPosition.xyz ) ;
	TempF3.y = dot( VBin, -PSInput.VPosition.xyz ) ;
	TempF3.z = dot( VNrm, -PSInput.VPosition.xyz ) ;
	V_to_Eye = normalize( TempF3 ) ;
#else // BUMPMAP
	// 頂点座標から視点へのベクトルを正規化
	V_to_Eye = normalize( -PSInput.VPosition ) ;
#endif // BUMPMAP

	// 法線の準備
	#if BUMPMAP
		// 法線の 0～1 の値を -1.0～1.0 に変換する
		Normal = ( tex2D( NormalMapTexture, PSInput.TexCoords0_1.xy ).rgb - cfZeroHalfOneTwo.y ) * cfZeroHalfOneTwo.w ;
	#else // BUMPMAP
		Normal = normalize( PSInput.VNormal ) ;
	#endif // BUMPMAP

	// ディフューズテクスチャカラーを取得
	TextureDiffuseColor = max( tex2D( DiffuseMapTexture, PSInput.TexCoords0_1.xy ), cfIgnoreTextureColor ) ;

// #ifdef IGNORE_COLOR
// 	TextureDiffuseColor.rgb = 1.0f;
// #endif
// 
// #ifdef IGNORE_TEXALPHA
// 	TextureDiffuseColor.a = 1.0f ;
// #endif
	
#ifdef INVERSE_COLOR
	TextureDiffuseColor.rgb = 1.0f - TextureDiffuseColor.rgb;
#endif

#ifdef USE_SPE
	#ifdef USE_SPETEX
	// デスペキュラテクスチャカラーを取得
	TextureSpecularColor = tex2D( SpecularMapTexture, PSInput.TexCoords0_1.xy ) ;
	#endif // USE_SPETEX
#endif // USE_SPE

#if SUBTEXTUREMODE != 0
	// サブテクスチャカラーを取得
	SubTextureColor = tex2D( SubTexture, PSInput.TexCoords0_1.zw );
#endif  // SUBTEXTUREMODE != 0

	// サブテクスチャカラーとディフューズテクスチャカラーの掛け合わせ
#if SUBTEXTUREMODE == 1  // αブレンド( MV1_LAYERBLEND_TYPE_TRANSLUCENT + 1 )
	TextureDiffuseColor.rgb = lerp( TextureDiffuseColor.rgb, SubTextureColor.rgb, SubTextureColor.a );
#endif // SUBTEXTUREMODE == 1

#if SUBTEXTUREMODE == 2  // 加算( MV1_LAYERBLEND_TYPE_ADDITIVE + 1 )
	TextureDiffuseColor.rgb += SubTextureColor.rgb;
#endif // SUBTEXTUREMODE == 2

#if SUBTEXTUREMODE == 3  // 乗算( MV1_LAYERBLEND_TYPE_MODULATE + 1 )
	TextureDiffuseColor.rgb *= SubTextureColor.rgb;
#endif // SUBTEXTUREMODE == 3

#if SUBTEXTUREMODE == 4  // 乗算×２( MV1_LAYERBLEND_TYPE_MODULATE2 + 1 )
	TextureDiffuseColor.rgb *= SubTextureColor.rgb * 2.0f;
#endif // SUBTEXTUREMODE == 4

	// ライトを使う場合 *******************************************************************( 開始 )

	// ディフューズカラーとスペキュラカラーの蓄積値を初期化
	TotalDiffuse = cfZeroHalfOneTwo.x ;
#ifdef USE_SPE
	TotalSpecular = cfZeroHalfOneTwo.x ;
#endif // USE_SPE
	
#if TOON
	#if TOON == 1
		TotalLightDiffuse = cfZeroHalfOneTwo.x ;
	#else // TOON == 1
		TotalAmbient = cfZeroHalfOneTwo.x ;
		TotalAngleGen = cfZeroHalfOneTwo.x ;
	#endif
#endif // TOON

	#if	SHADOWMAP
		// 頂点のテクスチャ座標値が範囲内の場合のみ処理する
		if( PSInput.ShadowMap1Pos_ShadowMap3PosX.x < -1.0f || PSInput.ShadowMap1Pos_ShadowMap3PosX.x > 1.0f ||
		    PSInput.ShadowMap1Pos_ShadowMap3PosX.y < -1.0f || PSInput.ShadowMap1Pos_ShadowMap3PosX.y > 1.0f ||
		    PSInput.ShadowMap1Pos_ShadowMap3PosX.z <  0.0f || PSInput.ShadowMap1Pos_ShadowMap3PosX.z > 1.0f )
		{
			ShadowRate.x = 1.0f;
		}
		else
		{
			// 深度テクスチャの座標を算出
			// PSInput.ShadowMap1Pos_ShadowMap3PosX.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
			DepthTexCoord.x = ( PSInput.ShadowMap1Pos_ShadowMap3PosX.x + 1.0f ) / 2.0f;

			// yは更に上下反転
			DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap1Pos_ShadowMap3PosX.y + 1.0f ) / 2.0f;

			// 深度バッファテクスチャから深度を取得
			TextureDepth = tex2D( ShadowMap1Texture, DepthTexCoord );

			// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで減衰率を最大にする
			ShadowRate.x = smoothstep( PSInput.ShadowMap1Pos_ShadowMap3PosX.z - cfShadowMap1_DAdj_Grad_Enbl0_1.y, PSInput.ShadowMap1Pos_ShadowMap3PosX.z, TextureDepth.r + cfShadowMap1_DAdj_Grad_Enbl0_1.x ) ;
		}

		// 頂点のテクスチャ座標値が範囲内の場合のみ処理する
		if( PSInput.ShadowMap2Pos_ShadowMap3PosY.x < -1.0f || PSInput.ShadowMap2Pos_ShadowMap3PosY.x > 1.0f ||
		    PSInput.ShadowMap2Pos_ShadowMap3PosY.y < -1.0f || PSInput.ShadowMap2Pos_ShadowMap3PosY.y > 1.0f ||
		    PSInput.ShadowMap2Pos_ShadowMap3PosY.z <  0.0f || PSInput.ShadowMap2Pos_ShadowMap3PosY.z > 1.0f )
		{
			ShadowRate.y = 1.0f;
		}
		else
		{
			// 深度テクスチャの座標を算出
			// PSInput.ShadowMap2Pos_ShadowMap3PosX.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
			DepthTexCoord.x = ( PSInput.ShadowMap2Pos_ShadowMap3PosY.x + 1.0f ) / 2.0f;

			// yは更に上下反転
			DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap2Pos_ShadowMap3PosY.y + 1.0f ) / 2.0f;

			// 深度バッファテクスチャから深度を取得
			TextureDepth = tex2D( ShadowMap2Texture, DepthTexCoord );

			// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで減衰率を最大にする
			ShadowRate.y = smoothstep( PSInput.ShadowMap2Pos_ShadowMap3PosY.z - cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0.z, PSInput.ShadowMap2Pos_ShadowMap3PosY.z, TextureDepth.r + cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0.y ) ;
		}

		// 頂点のテクスチャ座標値が範囲内の場合のみ処理する
		if( PSInput.ShadowMap1Pos_ShadowMap3PosX.w < -1.0f || PSInput.ShadowMap1Pos_ShadowMap3PosX.w > 1.0f ||
		    PSInput.ShadowMap2Pos_ShadowMap3PosY.w < -1.0f || PSInput.ShadowMap2Pos_ShadowMap3PosY.w > 1.0f ||
		    PSInput.Fog_ShadowMap3PosZ.y           <  0.0f || PSInput.Fog_ShadowMap3PosZ.y           > 1.0f )
		{
			ShadowRate.z = 1.0f;
		}
		else
		{
			// 深度テクスチャの座標を算出
			// PSInput.ShadowMap1Pos_ShadowMap3PosX.w と PSInput.ShadowMap2Pos_ShadowMap3PosY.w は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
			DepthTexCoord.x = ( PSInput.ShadowMap1Pos_ShadowMap3PosX.w + 1.0f ) / 2.0f;

			// yは更に上下反転
			DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap2Pos_ShadowMap3PosY.w + 1.0f ) / 2.0f;

			// 深度バッファテクスチャから深度を取得
			TextureDepth = tex2D( ShadowMap3Texture, DepthTexCoord );

			// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで減衰率を最大にする
			ShadowRate.z = smoothstep( PSInput.Fog_ShadowMap3PosZ.y - cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.w, PSInput.Fog_ShadowMap3PosZ.y, TextureDepth.r + cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.z ) ;
		}
	#else // SHADOWMAP
		ShadowRate.x = 1.0f ;
		ShadowRate.y = 1.0f ;
		ShadowRate.z = 1.0f ;
	#endif // SHADOWMAP


	
// ライト0の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG0_USE

	#if LG0_DIR

		// ライトの方向計算
		lLightDir = cfLight[ 0 ].Direction ;

	#else  // LG0_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 0 ].Position.xyz ) ;

	#endif  // LG0_DIR
	

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG0_SPOT || LG0_POINT

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 0 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 0 ].Range_FallOff_AT0_AT1.z + cfLight[ 0 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 0 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// スポットライト減衰計算 --------
		#if LG0_SPOT

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 0 ].Direction ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 0 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 0 ].AT2_SpotP0_SpotP1.z ), cfLight[ 0 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG0_SPOT
		// --------------------------------


		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, cfLight[ 0 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG0_SPOT || LG0_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG0_SPOT || LG0_POINT
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT0 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT0 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT0 ) ;


	// ディフューズ色計算

	if( LIGHT_NO_DIR_GEN > 0.5f )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
		lLightDot = dot( Normal, -lLightDir ) ;
		if( HALF_LAMBERT > 0.5f )
		{
			lLightDot = lLightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = lLightDot * lLightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( lLightDot ) ;
		}
	}
	

	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF4 = cfLight[ 0 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 0 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse += cfLight[ 0 ].Diffuse ;

			TotalAmbient += cfLight[ 0 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( cfLight[ 0 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 0 ].Ambient ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * cfLight[ 0 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG0_USE
// ライト0の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト1の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG1_USE

	#if LG1_DIR

		// ライトの方向計算
		lLightDir = cfLight[ 1 ].Direction ;

	#else  // LG1_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 1 ].Position.xyz ) ;

	#endif  // LG1_DIR
	

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG1_SPOT || LG1_POINT

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 1 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 1 ].Range_FallOff_AT0_AT1.z + cfLight[ 1 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 1 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// スポットライト減衰計算 --------
		#if LG1_SPOT

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 1 ].Direction ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 1 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 1 ].AT2_SpotP0_SpotP1.z ), cfLight[ 1 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG1_SPOT
		// --------------------------------


		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, cfLight[ 1 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG1_SPOT || LG1_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG1_SPOT || LG1_POINT
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT1 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT1 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT1 ) ;


	// ディフューズ色計算

	if( LIGHT_NO_DIR_GEN > 0.5f )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
		lLightDot = dot( Normal, -lLightDir ) ;
		if( HALF_LAMBERT > 0.5f )
		{
			lLightDot = lLightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = lLightDot * lLightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( lLightDot ) ;
		}
	}
	

	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF4 = cfLight[ 1 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 1 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse += cfLight[ 1 ].Diffuse ;

			TotalAmbient += cfLight[ 1 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( cfLight[ 1 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 1 ].Ambient ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * cfLight[ 1 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG1_USE
// ライト1の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト2の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG2_USE

	#if LG2_DIR

		// ライトの方向計算
		lLightDir = cfLight[ 2 ].Direction ;

	#else  // LG2_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 2 ].Position.xyz ) ;

	#endif  // LG2_DIR
	

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG2_SPOT || LG2_POINT

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 2 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 2 ].Range_FallOff_AT0_AT1.z + cfLight[ 2 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 2 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// スポットライト減衰計算 --------
		#if LG2_SPOT

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 2 ].Direction ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 2 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 2 ].AT2_SpotP0_SpotP1.z ), cfLight[ 2 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG2_SPOT
		// --------------------------------


		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, cfLight[ 2 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG2_SPOT || LG2_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG2_SPOT || LG2_POINT
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT2 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT2 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT2 ) ;


	// ディフューズ色計算

	if( LIGHT_NO_DIR_GEN > 0.5f )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
		lLightDot = dot( Normal, -lLightDir ) ;
		if( HALF_LAMBERT > 0.5f )
		{
			lLightDot = lLightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = lLightDot * lLightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( lLightDot ) ;
		}
	}
	

	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF4 = cfLight[ 2 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 2 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse += cfLight[ 2 ].Diffuse ;

			TotalAmbient += cfLight[ 2 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( cfLight[ 2 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 2 ].Ambient ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * cfLight[ 2 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG2_USE
// ライト2の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト3の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG3_USE

	#if LG3_DIR

		// ライトの方向計算
		lLightDir = cfLight[ 3 ].Direction ;

	#else  // LG3_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 3 ].Position.xyz ) ;

	#endif  // LG3_DIR
	

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG3_SPOT || LG3_POINT

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 3 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 3 ].Range_FallOff_AT0_AT1.z + cfLight[ 3 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 3 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// スポットライト減衰計算 --------
		#if LG3_SPOT

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 3 ].Direction ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 3 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 3 ].AT2_SpotP0_SpotP1.z ), cfLight[ 3 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG3_SPOT
		// --------------------------------


		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, cfLight[ 3 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG3_SPOT || LG3_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG3_SPOT || LG3_POINT
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT3 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT3 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT3 ) ;


	// ディフューズ色計算

	if( LIGHT_NO_DIR_GEN > 0.5f )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
		lLightDot = dot( Normal, -lLightDir ) ;
		if( HALF_LAMBERT > 0.5f )
		{
			lLightDot = lLightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = lLightDot * lLightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( lLightDot ) ;
		}
	}
	

	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF4 = cfLight[ 3 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 3 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse += cfLight[ 3 ].Diffuse ;

			TotalAmbient += cfLight[ 3 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( cfLight[ 3 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 3 ].Ambient ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * cfLight[ 3 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG3_USE
// ライト3の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト4の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG4_USE

	#if LG4_DIR

		// ライトの方向計算
		lLightDir = cfLight[ 4 ].Direction ;

	#else  // LG4_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 4 ].Position.xyz ) ;

	#endif  // LG4_DIR
	

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG4_SPOT || LG4_POINT

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 4 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 4 ].Range_FallOff_AT0_AT1.z + cfLight[ 4 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 4 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// スポットライト減衰計算 --------
		#if LG4_SPOT

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 4 ].Direction ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 4 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 4 ].AT2_SpotP0_SpotP1.z ), cfLight[ 4 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG4_SPOT
		// --------------------------------


		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, cfLight[ 4 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG4_SPOT || LG4_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG4_SPOT || LG4_POINT
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT4 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT4 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT4 ) ;


	// ディフューズ色計算

	if( LIGHT_NO_DIR_GEN > 0.5f )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
		lLightDot = dot( Normal, -lLightDir ) ;
		if( HALF_LAMBERT > 0.5f )
		{
			lLightDot = lLightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = lLightDot * lLightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( lLightDot ) ;
		}
	}
	

	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF4 = cfLight[ 4 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 4 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse += cfLight[ 4 ].Diffuse ;

			TotalAmbient += cfLight[ 4 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( cfLight[ 4 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 4 ].Ambient ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * cfLight[ 4 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG4_USE
// ライト4の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト5の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG5_USE

	#if LG5_DIR

		// ライトの方向計算
		lLightDir = cfLight[ 5 ].Direction ;

	#else  // LG5_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 5 ].Position.xyz ) ;

	#endif  // LG5_DIR
	

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG5_SPOT || LG5_POINT

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 5 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 5 ].Range_FallOff_AT0_AT1.z + cfLight[ 5 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 5 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// スポットライト減衰計算 --------
		#if LG5_SPOT

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 5 ].Direction ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 5 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 5 ].AT2_SpotP0_SpotP1.z ), cfLight[ 5 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG5_SPOT
		// --------------------------------


		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistancePow2, cfLight[ 5 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG5_SPOT || LG5_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG5_SPOT || LG5_POINT
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT5 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT5 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT5 ) ;


	// ディフューズ色計算

	if( LIGHT_NO_DIR_GEN > 0.5f )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
		lLightDot = dot( Normal, -lLightDir ) ;
		if( HALF_LAMBERT > 0.5f )
		{
			lLightDot = lLightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = lLightDot * lLightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( lLightDot ) ;
		}
	}
	

	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF4 = cfLight[ 5 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 5 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse += cfLight[ 5 ].Diffuse ;

			TotalAmbient += cfLight[ 5 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( cfLight[ 5 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 5 ].Ambient ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * cfLight[ 5 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG5_USE
// ライト5の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )















	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	#if TOON != 2
		// TotalDiffuse = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
		TotalDiffuse += cfAmbient_Emissive ;
	#else // TOON != 2
		// アンビエントカラーの蓄積値 += マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの
		TotalAmbient += cfAmbient_Emissive.rgb ;
	#endif  // TOON != 2

	#ifdef USE_SPE
		#if TOON
			#if TOON == 1
				// トゥーン１の時の出力計算
				ToonSpecularGradColor = tex2D( ToonSpecularGradTexture, max( TotalSpecular.b, max( TotalSpecular.r, TotalSpecular.g ) ) ) ;
				
				#ifdef USE_SPETEX
					TextureSpecularColor.rgb *= ToonSpecularGradColor.rgb ;
				#else  // USE_SPETEX
					TextureSpecularColor = ToonSpecularGradColor ;
				#endif // USE_SPETEX
				
				TextureSpecularColor.rgb *= tex3D( ToonRGBtoVMaxRGBVolumeTexture, TotalSpecular.rgb ).rgb * PSInput.Specular.rgb ;

				TextureDiffuseColor.rgb *= tex3D( ToonRGBtoVMaxRGBVolumeTexture, TotalLightDiffuse.rgb ).rgb * PSInput.Diffuse.rgb ;

				ToonDiffuseGradColor = tex2D( ToonDiffuseGradTexture, max( TotalDiffuse.b, max( TotalDiffuse.r, TotalDiffuse.g ) ) ) ;

				#if TOON_DIFOP_MUL
					PSOutput.Color0 = TextureDiffuseColor * ToonDiffuseGradColor ;
				#endif // TOON_DIFOP_MUL
				#if TOON_DIFOP_TRANS
					PSOutput.Color0 = lerp( TextureDiffuseColor, ToonDiffuseGradColor, ToonDiffuseGradColor.a ) ;
				#endif // TOON_DIFOP_TRANS

				#if TOON_SPEOP_ADD
					PSOutput.Color0 = PSOutput.Color0 + TextureSpecularColor ;
				#endif // TOON_SPEOP_ADD
				#if TOON_SPEOP_TRANS
					PSOutput.Color0 = lerp( PSOutput.Color0, TextureSpecularColor, TextureSpecularColor.a ) ;
				#endif // TOON_SPEOP_TRANS

				PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

				#if TOON_DIFOP_MUL
					PSOutput.Color0.a *= ToonDiffuseGradColor.a ;
				#endif // TOON_DIFOP_MUL
				
			#else  // TOON == 1
				// トゥーン２の時の出力計算
				ToonDiffuseGradColor = tex2D( ToonDiffuseGradTexture, TotalAngleGen ) ;

				// 出力 = saturate( saturate( アンビエントカラーの蓄積値 + マテリアルのディフューズカラー * ライトのディフューズカラーの蓄積値 ) * テクスチャグラデーションカラー + スペキュラカラー蓄積値 * マテリアルのスペキュラカラー ) * テクスチャカラー
				PSOutput.Color0.rgb = saturate( saturate( TotalDiffuse.rgb * PSInput.Diffuse.rgb + TotalAmbient ) * ToonDiffuseGradColor.rgb + PSInput.Specular.rgb * TotalSpecular.rgb ) * TextureDiffuseColor.rgb ;
				
				#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
					// スフィアマップの色を取得
					TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + cfZeroHalfOneTwo.yyy ;
					ToonSphereMapColor = tex2D( ToonSphereMapTexture, TempF3.xy ) ;
					
					#if TOON_SPHEREOP_MUL
						// スフィアマップの色を出力に乗算
						PSOutput.Color0.rgb *= ToonSphereMapColor.rgb ;
					#endif // TOON_SPHEREOP_MUL
					
					#if TOON_SPHEREOP_ADD
						// スフィアマップの色を出力に加算
						PSOutput.Color0.rgb += ToonSphereMapColor.rgb ;
					#endif // TOON_SPHEREOP_ADD
					
				#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD

				#if TOON_SPHEREOP_MUL

					// 出力アルファ = テクスチャアルファ * Factorアルファ * 頂点アルファ * スフィアマップアルファ
					PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a * ToonSphereMapColor.a ;

				#else // TOON_SPHEREOP_MUL

					// 出力アルファ = テクスチャアルファ * Factorアルファ * 頂点アルファ
					PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

				#endif // TOON_SPHEREOP_MUL

			#endif   // TOON == 1
		#else  // TOON

			#ifdef USE_SPETEX
				// TextureSpecularColor = ライトのスペキュラカラー蓄積値 * スペキュラカラー * スペキュラテクスチャカラー
				TextureSpecularColor = tex2D( SpecularMapTexture, PSInput.TexCoords0_1.xy ) * TotalSpecular * PSInput.Specular ;
			#else  // USE_SPETEX
				// TextureSpecularColor = ライトのスペキュラカラー蓄積値 * スペキュラカラー
				TextureSpecularColor = TotalSpecular * PSInput.Specular ;
			#endif // USE_SPETEX

			// 出力カラー = TotalDiffuse * テクスチャカラー + TextureSpecularColor

			PSOutput.Color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse.rgb + TextureSpecularColor.rgb ;
			PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

		#endif // TOON

	#else  // USE_SPE

		#if TOON
			#if TOON == 1
				ToonDiffuseGradColor = tex2D( ToonDiffuseGradTexture, max( TotalDiffuse.b, max( TotalDiffuse.r, TotalDiffuse.g ) ) ) ;
				TextureDiffuseColor.rgb *= tex3D( ToonRGBtoVMaxRGBVolumeTexture, TotalLightDiffuse.rgb ).rgb * PSInput.Diffuse.rgb ;

				#if TOON_DIFOP_MUL
					// 出力カラー = テクスチャカラー * ディフューズカラー蓄積値 * グラデーションカラー
					PSOutput.Color0 = TextureDiffuseColor * ToonDiffuseGradColor ;
				#endif // TOON_DIFOP_MUL
				#if TOON_DIFOP_TRANS
					// 出力カラー = グラデーションカラー * グラデーションα + ( 1 - グラデーションα ) * ( テクスチャカラー * ディフューズカラー蓄積値 )
					PSOutput.Color0 = lerp( TextureDiffuseColor, ToonDiffuseGradColor, ToonDiffuseGradColor.a ) ;
				#endif // TOON_DIFOP_TRANS

				// 出力α = テクスチャα * ディフューズα * 大域α
				PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

				// 乗算の場合はα値もグラデーションαと乗算する
				#if TOON_DIFOP_MUL
					PSOutput.Color0.a *= ToonDiffuseGradColor.a ;
				#endif // TOON_DIFOP_MUL

			#else	// TOON == 1
				// トゥーン２の時の出力計算
				ToonDiffuseGradColor = tex2D( ToonDiffuseGradTexture, TotalAngleGen ) ;

				// 出力 = ( アンビエントカラーの蓄積値 + マテリアルのディフューズカラー * ライトのディフューズカラーの蓄積値 ) * テクスチャグラデーションカラー * テクスチャカラー
				PSOutput.Color0.rgb = saturate( TotalDiffuse.rgb * PSInput.Diffuse.rgb + TotalAmbient.rgb ) * ToonDiffuseGradColor.rgb * TextureDiffuseColor.rgb ;

				#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
					// スフィアマップの色を取得
					TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + cfZeroHalfOneTwo.yyy ;
					ToonSphereMapColor = tex2D( ToonSphereMapTexture, TempF3.xy ) ;
					
					#if TOON_SPHEREOP_MUL
						// スフィアマップの色を出力に乗算
						PSOutput.Color0.rgb *= ToonSphereMapColor.rgb ;
					#endif // TOON_SPHEREOP_MUL
					
					#if TOON_SPHEREOP_ADD
						// スフィアマップの色を出力に加算
						PSOutput.Color0.rgb += ToonSphereMapColor.rgb ;
					#endif // TOON_SPHEREOP_ADD
					
				#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD

				#if TOON_SPHEREOP_MUL

					PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a * ToonSphereMapColor.a ;

				#else // TOON_SPHEREOP_MUL

					PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

				#endif // TOON_SPHEREOP_MUL

			#endif		// TOON == 1
		#else  // TOON
			// 出力カラー = TotalDiffuse * テクスチャカラー
			PSOutput.Color0.rgb = TotalDiffuse.rgb * TextureDiffuseColor.rgb ;

			// 出力α = テクスチャα * ディフューズα * 大域α
			PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;
		#endif // TOON

	#endif // USE_SPE

	// フォグ効果
	PSOutput.Color0.rgb = lerp( cfFogColor.rgb, PSOutput.Color0.rgb, saturate( PSInput.Fog_ShadowMap3PosZ.x ) );

	// アルファ乗算カラー
	if( cfMulAlphaColorAddColor.x > 0.5f )
	{
		PSOutput.Color0.rgb *= PSOutput.Color0.a;
	}

	// 色単純加算
	PSOutput.Color0.r += cfMulAlphaColorAddColor.y ;
	PSOutput.Color0.g += cfMulAlphaColorAddColor.z ;
	PSOutput.Color0.b += cfMulAlphaColorAddColor.w ;

	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// ライトを使う場合 *******************************************************************( 終了 )

	return PSOutput ;
}


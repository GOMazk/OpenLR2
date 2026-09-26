#include "../VertexLighting_PS.h"

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float4 TextureSpecularColor ;
	float3 Normal ;
	float4 TotalDiffuse ;
	float4 TotalSpecular ;
	float TotalAngleGen ;
	float DiffuseAngleGen ;
	float3 TempF3 ;
	float Temp ;
	float LightDot ;
	float3 ShadowRate ;
	float ShadowGen ;
	float ParamAlpha;

#if SHADOWMAP
	float2 DepthTexCoord ;
	float4 TextureDepth ;
#endif

	ParamAlpha = PSInput.Diffuse.a ;

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
	TextureSpecularColor = tex2D( SpecularMapTexture, PSInput.TexCoords0_1.xy ) ;
#ifdef INVERSE_COLOR
	TextureSpecularColor.rgb = 1.0f - TextureSpecularColor.rgb;
#endif
	#endif // USE_SPETEX
#endif // USE_SPE

	// ライトを使わない場合はカラーをそのまま出力 *****************************************( 開始 )
	#ifndef LG_USE

		// シャドウマップへの描画 *********************************************************( 開始 )
		#if SHADOWMAP_DRAW
			// 出力α = テクスチャα * ディフューズα * 大域α
			PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;
			
			// Ｚ値を色として出力
			PSOutput.Color0.r = PSInput.ShadowMap1Pos_ShadowMap3PosX.z;
			PSOutput.Color0.g = 0.0f ;
			PSOutput.Color0.b = 0.0f ;
		
		// シャドウマップへの描画 *********************************************************( 終了 )
		#else // SHADOWMAP_DRAW
		
			#ifdef USE_SPE

				#ifdef USE_SPETEX

					PSOutput.Color0 = PSInput.Specular * TextureSpecularColor + PSInput.Diffuse * TextureDiffuseColor ;

				#else  // USE_SPETEX

					PSOutput.Color0 = PSInput.Specular + PSInput.Diffuse * TextureDiffuseColor ;

				#endif // USE_SPETEX

			#else  // USE_SPE

				PSOutput.Color0 = PSInput.Diffuse * TextureDiffuseColor ;

			#endif // USE_SPE

			PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;
			
		#endif // SHADOWMAP_DRAW

	// ライトを使わない場合はカラーをそのまま出力 *****************************************( 終了 )
	#else // LG_USE
	// ライトを使う場合 *******************************************************************( 開始 )

		// バンプマップ or フォンシェーディングの場合 =========================================( 開始 )
		#if BUMPMAP || PHONG

			#if BUMPMAP

				// 法線の 0～1 の値を -1.0～1.0 に変換する
				Normal = ( tex2D( NormalMapTexture, PSInput.TexCoords0_1.xy ).rgb - cfZeroHalfOneTwo.y ) * cfZeroHalfOneTwo.w ;

			#else // BUMPMAP

				Normal = PSInput.Normal_Fog.xyz ;

			#endif // BUMPMAP


			#ifdef    PHONG
				Normal = normalize( Normal ) ;
			#endif

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
					// PSInput.ShadowMap2Pos_ShadowMap3PosY.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
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
					PSInput.V_to_Eye_ShadowMap3PosZ.w      <  0.0f || PSInput.V_to_Eye_ShadowMap3PosZ.w      > 1.0f )
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
					ShadowRate.z = smoothstep( PSInput.V_to_Eye_ShadowMap3PosZ.w - cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.w, PSInput.V_to_Eye_ShadowMap3PosZ.w, TextureDepth.r + cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.z ) ;
				}

			#else // SHADOWMAP
				ShadowRate.x = 1.0f ;
				ShadowRate.y = 1.0f ;
				ShadowRate.z = 1.0f ;
			#endif // SHADOWMAP

			// ディフューズカラーとスペキュラカラーの蓄積値を初期化
			TotalDiffuse = cfZeroHalfOneTwo.x ;
			TotalSpecular = cfZeroHalfOneTwo.x ;




			
// ライト0の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG0_USE

	// ディフューズ色計算

	if( LIGHT_NO_DIR_GEN > 0.5f )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// DiffuseAngleGen = ディフューズ角度減衰率計算
		LightDot = dot( Normal, PSInput.Light0_Dir_Gen.xyz ) ;
		if( HALF_LAMBERT > 0.5f )
		{
			LightDot = LightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = LightDot * LightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( LightDot ) ;
		}
	}

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT0 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT0 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT0 ) ;

	#if LG0_DIR

		// ディフューズカラー蓄積値 += ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
		TotalDiffuse += cfLight[ 0 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 0 ].Ambient ;

	#else // LG0_DIR

		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( cfLight[ 0 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 0 ].Ambient ) * PSInput.Light0_Dir_Gen.w ;

	#endif // LG0_DIR


	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = PSInput.V_to_Eye_ShadowMap3PosZ.xyz + PSInput.Light0_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		#if LG0_DIR

			// スペキュラカラー蓄積値 += Temp * ライトのスペキュラカラー
			TotalSpecular += Temp * cfLight[ 0 ].Specular * ShadowGen ;

		#else // LG0_DIR

			// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
			TotalSpecular += Temp * PSInput.Light0_Dir_Gen.w * cfLight[ 0 ].Specular * ShadowGen ;

		#endif // LG0_DIR

	#endif // USE_SPE

#endif // LG0_USE
// ライト0の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









			
// ライト1の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG1_USE

	// ディフューズ色計算

	if( LIGHT_NO_DIR_GEN > 0.5f )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// DiffuseAngleGen = ディフューズ角度減衰率計算
		LightDot = dot( Normal, PSInput.Light1_Dir_Gen.xyz ) ;
		if( HALF_LAMBERT > 0.5f )
		{
			LightDot = LightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = LightDot * LightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( LightDot ) ;
		}
	}

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT1 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT1 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT1 ) ;

	#if LG1_DIR

		// ディフューズカラー蓄積値 += ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
		TotalDiffuse += cfLight[ 1 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 1 ].Ambient ;

	#else // LG1_DIR

		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( cfLight[ 1 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 1 ].Ambient ) * PSInput.Light1_Dir_Gen.w ;

	#endif // LG1_DIR


	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = PSInput.V_to_Eye_ShadowMap3PosZ.xyz + PSInput.Light1_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		#if LG1_DIR

			// スペキュラカラー蓄積値 += Temp * ライトのスペキュラカラー
			TotalSpecular += Temp * cfLight[ 1 ].Specular * ShadowGen ;

		#else // LG1_DIR

			// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
			TotalSpecular += Temp * PSInput.Light1_Dir_Gen.w * cfLight[ 1 ].Specular * ShadowGen ;

		#endif // LG1_DIR

	#endif // USE_SPE

#endif // LG1_USE
// ライト1の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









			
// ライト2の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG2_USE

	// ディフューズ色計算

	if( LIGHT_NO_DIR_GEN > 0.5f )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// DiffuseAngleGen = ディフューズ角度減衰率計算
		LightDot = dot( Normal, PSInput.Light2_Dir_Gen.xyz ) ;
		if( HALF_LAMBERT > 0.5f )
		{
			LightDot = LightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = LightDot * LightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( LightDot ) ;
		}
	}

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT2 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT2 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT2 ) ;

	#if LG2_DIR

		// ディフューズカラー蓄積値 += ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
		TotalDiffuse += cfLight[ 2 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 2 ].Ambient ;

	#else // LG2_DIR

		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( cfLight[ 2 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 2 ].Ambient ) * PSInput.Light2_Dir_Gen.w ;

	#endif // LG2_DIR


	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = PSInput.V_to_Eye_ShadowMap3PosZ.xyz + PSInput.Light2_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		#if LG2_DIR

			// スペキュラカラー蓄積値 += Temp * ライトのスペキュラカラー
			TotalSpecular += Temp * cfLight[ 2 ].Specular * ShadowGen ;

		#else // LG2_DIR

			// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
			TotalSpecular += Temp * PSInput.Light2_Dir_Gen.w * cfLight[ 2 ].Specular * ShadowGen ;

		#endif // LG2_DIR

	#endif // USE_SPE

#endif // LG2_USE
// ライト2の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


















			// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

			// TotalDiffuse = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
			TotalDiffuse += cfAmbient_Emissive ;

			#ifdef USE_SPE
				#ifdef USE_SPETEX
					// TextureSpecularColor = ライトのスペキュラカラー蓄積値 * スペキュラカラー * スペキュラテクスチャカラー
					TextureSpecularColor = tex2D( SpecularMapTexture, PSInput.TexCoords0_1.xy ) * TotalSpecular * PSInput.Specular ;
				#else  // USE_SPETEX
					// TextureSpecularColor = ライトのスペキュラカラー蓄積値 * スペキュラカラー
					TextureSpecularColor = TotalSpecular * PSInput.Specular ;
				#endif // USE_SPETEX

				// 出力カラー = TotalDiffuse * テクスチャカラー + TextureSpecularColor
				PSOutput.Color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse.rgb + TextureSpecularColor.rgb ;

				// 出力α = テクスチャα * ディフューズα * 大域α
				PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

			#else  // USE_SPE

				// 出力カラー = TotalDiffuse * テクスチャカラー
				PSOutput.Color0.rgb = TotalDiffuse.rgb * TextureDiffuseColor.rgb ;

				// 出力α = テクスチャα * ディフューズα * 大域α
				PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

			#endif // USE_SPE

			// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


		// バンプマップ or フォンシェーディングの場合 =========================================( 終了 )
		#else  // BUMPMAP || PHONG
			// バンプマップ or フォンシェーディングではない場合 ===================================( 開始 )

			#ifdef USE_SPE

				#ifdef USE_SPETEX

					PSOutput.Color0.rgb = TextureDiffuseColor.rgb * PSInput.Diffuse.rgb + TextureSpecularColor.rgb * PSInput.Specular.rgb ;

				#else  // USE_SPETEX

					PSOutput.Color0.rgb = TextureDiffuseColor.rgb * PSInput.Diffuse.rgb + PSInput.Specular.rgb ;

				#endif // USE_SPETEX

			#else  // USE_SPE

				PSOutput.Color0.rgb = TextureDiffuseColor.rgb * PSInput.Diffuse.rgb ;

			#endif // USE_SPE

			PSOutput.Color0.a   = TextureDiffuseColor.a   * ParamAlpha ;

		#endif // BUMPMAP || PHONG
		// バンプマップ or フォンシェーディングではない場合 ===================================( 終了 )

	#endif // LG_USE
	// ライトを使う場合 *******************************************************************( 終了 )

#ifdef PRE_MUL_ALPHA
	PSOutput.Color0.rgb *= PSInput.Diffuse.a ;
#endif

#ifdef MUL_X_4
	PSOutput.Color0.rgb *= 4.0f ;
#endif

#ifdef MUL_BLEND
	PSOutput.Color0 = lerp( 1.0f, PSOutput.Color0, PSOutput.Color0.a ) ;
#endif

#if SM_3
	// フォグ効果
	PSOutput.Color0.rgb = lerp( cfFogColor.rgb, PSOutput.Color0.rgb, saturate( PSInput.Normal_Fog.w ) );
#endif // SM_3

	// 色単純加算
	PSOutput.Color0.r += cfMulAlphaColorAddColor.y ;
	PSOutput.Color0.g += cfMulAlphaColorAddColor.z ;
	PSOutput.Color0.b += cfMulAlphaColorAddColor.w ;

	return PSOutput ;
}


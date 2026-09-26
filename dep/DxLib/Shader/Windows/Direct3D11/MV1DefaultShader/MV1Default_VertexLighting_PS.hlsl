#include "../PixelShader.h"


// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;		// ディフューズカラー
	float4 Specular        : COLOR1 ;		// スペキュラカラー
	float4 TexCoords0_1    : TEXCOORD0 ;	// テクスチャ座標とサブテクスチャ座標( x:テクスチャ座標x  y:テクスチャ座標y  z:サブテクスチャ座標x  w:サブテクスチャ座標y )

	float3 V_to_Eye        : TEXCOORD1 ;	// 頂点座標から視線へのベクトル( ビュー空間 )
	float4 Normal_Fog      : TEXCOORD2 ;	// x.y.z:法線( ビュー空間 )  w:フォグパラメータ

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
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;	// 色
} ;





SamplerState g_DiffuseMapSampler            : register( s0 ) ;		// ディフューズマップテクスチャ
Texture2D    g_DiffuseMapTexture            : register( t0 ) ;		// ディフューズマップテクスチャ

#if BUMPMAP
SamplerState g_NormalMapSampler             : register( s1 ) ;		// 法線マップテクスチャ
Texture2D    g_NormalMapTexture             : register( t1 ) ;		// 法線マップテクスチャ
#endif // BUMPMAP

#if USE_SPETEX
SamplerState g_SpecularMapSampler           : register( s2 ) ;		// スペキュラマップテクスチャ
Texture2D    g_SpecularMapTexture           : register( t2 ) ;		// スペキュラマップテクスチャ
#endif // USE_SPETEX

#if TOON
SamplerState g_ToonDiffuseGradSampler       : register( s3 ) ;		// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ
Texture2D    g_ToonDiffuseGradTexture       : register( t3 ) ;		// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ

SamplerState g_ToonSpecularGradSampler      : register( s4 ) ;		// トゥーンレンダリング用スペキュラカラーグラデーションテクスチャ
Texture2D    g_ToonSpecularGradTexture      : register( t4 ) ;		// トゥーンレンダリング用スペキュラカラーグラデーションテクスチャ

#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
SamplerState g_ToonSphereMapSampler         : register( s5 ) ;		// トゥーンレンダリング用スフィアマップテクスチャ
Texture2D    g_ToonSphereMapTexture         : register( t5 ) ;		// トゥーンレンダリング用スフィアマップテクスチャ
#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
SamplerState g_ToonRGBtoVMaxRGBVolumeSampler: register( s6 ) ;		// トゥーンレンダリング用RGB輝度飽和処理用ボリュームテクスチャ
Texture3D    g_ToonRGBtoVMaxRGBVolumeTexture: register( t6 ) ;		// トゥーンレンダリング用RGB輝度飽和処理用ボリュームテクスチャ
#endif // TOON

#if SUBTEXTUREMODE != 0
SamplerState g_SubSampler                   : register( s7 ) ;		// サブテクスチャ
Texture2D    g_SubTexture                   : register( t7 ) ;		// サブテクスチャ
#endif // SUBTEXTUREMODE != 0

#if SHADOWMAP
SamplerState g_ShadowMap0Sampler            : register( s8  ) ;		// シャドウマップ０テクスチャ
Texture2D    g_ShadowMap0Texture            : register( t8  ) ;		// シャドウマップ０テクスチャ

SamplerState g_ShadowMap1Sampler            : register( s9  ) ;		// シャドウマップ１テクスチャ
Texture2D    g_ShadowMap1Texture            : register( t9  ) ;		// シャドウマップ１テクスチャ

SamplerState g_ShadowMap2Sampler            : register( s10 ) ;		// シャドウマップ２テクスチャ
Texture2D    g_ShadowMap2Texture            : register( t10 ) ;		// シャドウマップ２テクスチャ
#endif




// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT	PSOutput ;
	float4		TextureDiffuseColor ;
	float4		TextureSpecularColor ;
	float4		ToonSpecularGradColor ;
	float4		ToonDiffuseGradColor ;
	float4		ToonSphereMapColor ;
	float4		ToonRGBtoVMaxRGBVolumeColor ;
	float3		Normal ;
	float3		TotalDiffuse ;
	float3		TotalSpecular ;
	float3		TotalLightDiffuse ;
	float3		TotalAmbient ;
	float		TotalAngleGen ;
	float		DiffuseAngleGen ;
	float3		TempF3 ;
	float		Temp ;
	float		lLightDot ;
	float3		ShadowRate ;
	float		ShadowGen ;
	float		ParamAlpha;

#if SHADOWMAP
	float2		DepthTexCoord ;
	float4		TextureDepth ;
//	float4		variance_md_p_pmax ;
#endif

	ParamAlpha = PSInput.Diffuse.a * g_Base.FactorColor.a;

#if SUBTEXTUREMODE != 0
	float4 SubTextureColor;
#endif  // SUBTEXTUREMODE != 0

	TextureDiffuseColor =  g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords0_1.xy ) ;

#ifdef MAT_TYPE

	#if MAT_PARAM_SPEC_LUMINANCE

		Temp = 0.29900f * g_Common.Material.Specular.r + 0.58700f * g_Common.Material.Specular.g + 0.11400f * g_Common.Material.Specular.b ;

	#endif // MAT_PARAM_SPEC_LUMINANCE

	#if MAT_PARAM_SPEC_POWER

		Temp = g_Common.Material.Power ;

	#endif // MAT_PARAM_SPEC_POWER

	#if MAT_CALC_UNORM

		PSOutput.Color0.rgb = saturate( ( Temp - g_Common.Material.TypeParam0 ) / ( g_Common.Material.TypeParam1 - g_Common.Material.TypeParam0 ) );

	#endif // MAT_CALC_UNORM

	#if MAT_CALC_CLIP_UNORM

		if( Temp < g_Common.Material.TypeParam2 )
		{
			PSOutput.Color0.rgb = 0.0f ;
		}
		else
		{
			PSOutput.Color0.rgb = saturate( ( Temp - g_Common.Material.TypeParam0 ) / ( g_Common.Material.TypeParam1 - g_Common.Material.TypeParam0 ) );
		}

	#endif // MAT_CALC_CLIP_UNORM

	#if MAT_CALC_CMP_GREATEREQUAL

		if( Temp >= g_Common.Material.TypeParam0 )
		{
			PSOutput.Color0.rgb = 1.0f ;
		}
		else
		{
			PSOutput.Color0.rgb = 0.0f ;
		}

	#endif // MAT_CALC_CMP_GREATEREQUAL


	PSOutput.Color0.a = TextureDiffuseColor.a ;

	// アルファテスト
	AlphaTest( PSOutput.Color0.a ) ;

#else  // MAT_TYPE
	

#ifdef USE_SPE
	#ifdef USE_SPETEX
	TextureSpecularColor = g_SpecularMapTexture.Sample( g_SpecularMapSampler, PSInput.TexCoords0_1.xy ) ;
	#endif // USE_SPETEX
#endif // USE_SPE

#if SUBTEXTUREMODE != 0
	SubTextureColor = g_SubTexture.Sample( g_SubSampler, PSInput.TexCoords0_1.zw ) ;
#endif  // SUBTEXTUREMODE != 0

#if SUBTEXTUREMODE == 1  // αブレンド( MV1_LAYERBLEND_TYPE_TRANSLUCENT + 1 )
	TextureDiffuseColor = lerp( TextureDiffuseColor, SubTextureColor, SubTextureColor.a ) ;
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

	// ライトを使わない場合はカラーをそのまま出力 *****************************************( 開始 )
	#ifndef LG_USE

		// シャドウマップへの描画 *********************************************************( 開始 )
		#if SHADOWMAP_DRAW
			#if TOON
				#if TOON == 1
					// 出力α = テクスチャα * ディフューズα * 大域α
					PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

				#else	// TOON == 1
					#if TOON_SPHEREOP_MUL
						Normal = normalize( PSInput.Normal_Fog.xyz ) ;
					
						// スフィアマップの色を取得
						TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + 0.5f ;
						ToonSphereMapColor = g_ToonSphereMapTexture.Sample( g_ToonSphereMapSampler, TempF3.xy ) ;

						PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha * ToonSphereMapColor.a ;

					#else // TOON_SPHEREOP_MUL

						PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

					#endif // TOON_SPHEREOP_MUL

				#endif		// TOON == 1
			#else  // TOON
			
				// 出力α = テクスチャα * ディフューズα * 大域α
				PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;
				
			#endif // TOON
			
			// Ｚ値を色として出力
			PSOutput.Color0.r = PSInput.ShadowMap0Pos.z;
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
				Normal = ( g_NormalMapTexture.Sample( g_NormalMapSampler, PSInput.TexCoords0_1.xy ).rgb - 0.5f ) * 2.0f ;

			#else // BUMPMAP

				Normal = PSInput.Normal_Fog.xyz ;

			#endif // BUMPMAP


			#ifdef    PHONG
				Normal = normalize( Normal ) ;
			#endif

			#if	SHADOWMAP
				// 頂点のテクスチャ座標値が範囲内の場合のみ処理する
				if( PSInput.ShadowMap0Pos.x < -1.0f || PSInput.ShadowMap0Pos.x > 1.0f ||
					PSInput.ShadowMap0Pos.y < -1.0f || PSInput.ShadowMap0Pos.y > 1.0f ||
					PSInput.ShadowMap0Pos.z <  0.0f || PSInput.ShadowMap0Pos.z > 1.0f )
				{
					ShadowRate.x = 1.0f;
				}
				else
				{
					// 深度テクスチャの座標を算出
					// PSInput.ShadowMap0Pos.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
					DepthTexCoord.x = ( PSInput.ShadowMap0Pos.x + 1.0f ) / 2.0f;

					// yは更に上下反転
					DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap0Pos.y + 1.0f ) / 2.0f;

					// 深度バッファテクスチャから深度を取得
					TextureDepth = g_ShadowMap0Texture.Sample( g_ShadowMap0Sampler, DepthTexCoord );

					// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで減衰率を最大にする
					ShadowRate.x = smoothstep( PSInput.ShadowMap0Pos.z - g_ShadowMap.Data[ 0 ].GradationParam, PSInput.ShadowMap0Pos.z, TextureDepth.r + g_ShadowMap.Data[ 0 ].AdjustDepth ) ;
				}

				// 頂点のテクスチャ座標値が範囲内の場合のみ処理する
				if( PSInput.ShadowMap1Pos.x < -1.0f || PSInput.ShadowMap1Pos.x > 1.0f ||
					PSInput.ShadowMap1Pos.y < -1.0f || PSInput.ShadowMap1Pos.y > 1.0f ||
					PSInput.ShadowMap1Pos.z <  0.0f || PSInput.ShadowMap1Pos.z > 1.0f )
				{
					ShadowRate.y = 1.0f;
				}
				else
				{
					// 深度テクスチャの座標を算出
					// PSInput.ShadowMap1Pos.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
					DepthTexCoord.x = ( PSInput.ShadowMap1Pos.x + 1.0f ) / 2.0f;

					// yは更に上下反転
					DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap1Pos.y + 1.0f ) / 2.0f;

					// 深度バッファテクスチャから深度を取得
					TextureDepth = g_ShadowMap1Texture.Sample( g_ShadowMap1Sampler, DepthTexCoord );

					// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで減衰率を最大にする
					ShadowRate.y = smoothstep( PSInput.ShadowMap1Pos.z - g_ShadowMap.Data[ 1 ].GradationParam, PSInput.ShadowMap1Pos.z, TextureDepth.r + g_ShadowMap.Data[ 1 ].AdjustDepth ) ;
				}

				// 頂点のテクスチャ座標値が範囲内の場合のみ処理する
				if( PSInput.ShadowMap2Pos.x < -1.0f || PSInput.ShadowMap2Pos.x > 1.0f ||
					PSInput.ShadowMap2Pos.y < -1.0f || PSInput.ShadowMap2Pos.y > 1.0f ||
					PSInput.ShadowMap2Pos.z <  0.0f || PSInput.ShadowMap2Pos.z > 1.0f )
				{
					ShadowRate.z = 1.0f;
				}
				else
				{
					// 深度テクスチャの座標を算出
					// PSInput.ShadowMap2Pos.x と PSInput.ShadowMap2Pos.y は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
					DepthTexCoord.x = ( PSInput.ShadowMap2Pos.x + 1.0f ) / 2.0f;

					// yは更に上下反転
					DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap2Pos.y + 1.0f ) / 2.0f;

					// 深度バッファテクスチャから深度を取得
					TextureDepth = g_ShadowMap2Texture.Sample( g_ShadowMap2Sampler, DepthTexCoord );

					// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで減衰率を最大にする
					ShadowRate.z = smoothstep( PSInput.ShadowMap2Pos.z - g_ShadowMap.Data[ 2 ].GradationParam, PSInput.ShadowMap2Pos.z, TextureDepth.r + g_ShadowMap.Data[ 2 ].AdjustDepth ) ;
				}

			#else // SHADOWMAP
				ShadowRate.x = 1.0f ;
				ShadowRate.y = 1.0f ;
				ShadowRate.z = 1.0f ;
			#endif // SHADOWMAP

			// ディフューズカラーとスペキュラカラーの蓄積値を初期化
			TotalDiffuse  = 0.0f ;
			TotalSpecular = 0.0f ;
			#if TOON
				#if TOON == 1
					TotalLightDiffuse = 0.0f ;
				#else // TOON == 1
					TotalAmbient = 0.0f ;
					TotalAngleGen = 0.0f ;
				#endif
			#endif // TOON




			
// ライト0の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG0_USE

	// ディフューズ色計算

	if( g_Base.NoLightAngleAttenuation == 1 )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// DiffuseAngleGen = ディフューズ角度減衰率計算
		lLightDot = dot( Normal, PSInput.Light0_Dir_Gen.xyz ) ;
		if( g_Base.UseHalfLambert == 1 )
		{
			lLightDot = lLightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = lLightDot * lLightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( lLightDot ) ;
		}
	}

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, g_ShadowMap.Data[ 0 ].Enable_Light0 ) *
	            max( ShadowRate.y, g_ShadowMap.Data[ 1 ].Enable_Light0 ) *
	            max( ShadowRate.z, g_ShadowMap.Data[ 2 ].Enable_Light0 ) ;

	#if LG0_DIR

		#if TOON
			#if TOON == 1
				// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
				// ディフューズカラー蓄積値 += ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
				TempF3             = g_Common.Light[ 0 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 ;
				TotalDiffuse      += TempF3 + g_Common.Light[ 0 ].Ambient.xyz ;

			#else // TOON == 1
				// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
				// ディフューズカラー蓄積値   += ライトのディフューズカラー
				// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
				// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
				TotalDiffuse  += g_Common.Light[ 0 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 0 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen ;

			#endif // TOON == 1
		#else // TOON
			// ディフューズカラー蓄積値 += ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
			TotalDiffuse += g_Common.Light[ 0 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 0 ].Ambient.xyz ;

		#endif //TOON

	#else // LG0_DIR

		#if TOON
			#if TOON == 1
				// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
				// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
				TempF3             = g_Common.Light[ 0 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 * PSInput.Light0_Dir_Gen.w ;
				TotalDiffuse      += ( TempF3 + g_Common.Light[ 0 ].Ambient.xyz ) * PSInput.Light0_Dir_Gen.w ;

			#else // TOON == 1
				// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
				// ディフューズカラー蓄積値           += ライトのディフューズカラー
				// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
				// ライトの角度減衰率の蓄積値         += ライトの角度減衰率
				TotalDiffuse  += g_Common.Light[ 0 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 0 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen * PSInput.Light0_Dir_Gen.w ;

			#endif // TOON == 1
		#else // TOON
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TotalDiffuse += ( g_Common.Light[ 0 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 0 ].Ambient.xyz ) * PSInput.Light0_Dir_Gen.w ;
			
		#endif // TOON

	#endif // LG0_DIR


	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = PSInput.V_to_Eye + PSInput.Light0_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		#if LG0_DIR

			// スペキュラカラー蓄積値 += Temp * ライトのスペキュラカラー
			TotalSpecular += Temp * g_Common.Light[ 0 ].Specular * ShadowGen ;

		#else // LG0_DIR

			// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
			TotalSpecular += Temp * PSInput.Light0_Dir_Gen.w * g_Common.Light[ 0 ].Specular * ShadowGen ;

		#endif // LG0_DIR

	#endif // USE_SPE

#endif // LG0_USE
// ライト0の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









			
// ライト1の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG1_USE

	// ディフューズ色計算

	if( g_Base.NoLightAngleAttenuation == 1 )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// DiffuseAngleGen = ディフューズ角度減衰率計算
		lLightDot = dot( Normal, PSInput.Light1_Dir_Gen.xyz ) ;
		if( g_Base.UseHalfLambert == 1 )
		{
			lLightDot = lLightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = lLightDot * lLightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( lLightDot ) ;
		}
	}

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, g_ShadowMap.Data[ 0 ].Enable_Light1 ) *
	            max( ShadowRate.y, g_ShadowMap.Data[ 1 ].Enable_Light1 ) *
	            max( ShadowRate.z, g_ShadowMap.Data[ 2 ].Enable_Light1 ) ;

	#if LG1_DIR

		#if TOON
			#if TOON == 1
				// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
				// ディフューズカラー蓄積値 += ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
				TempF3             = g_Common.Light[ 1 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 ;
				TotalDiffuse      += TempF3 + g_Common.Light[ 1 ].Ambient.xyz ;

			#else // TOON == 1
				// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
				// ディフューズカラー蓄積値   += ライトのディフューズカラー
				// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
				// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
				TotalDiffuse  += g_Common.Light[ 1 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 1 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen ;

			#endif // TOON == 1
		#else // TOON
			// ディフューズカラー蓄積値 += ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
			TotalDiffuse += g_Common.Light[ 1 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 1 ].Ambient.xyz ;

		#endif //TOON

	#else // LG1_DIR

		#if TOON
			#if TOON == 1
				// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
				// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
				TempF3             = g_Common.Light[ 1 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 * PSInput.Light1_Dir_Gen.w ;
				TotalDiffuse      += ( TempF3 + g_Common.Light[ 1 ].Ambient.xyz ) * PSInput.Light1_Dir_Gen.w ;

			#else // TOON == 1
				// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
				// ディフューズカラー蓄積値           += ライトのディフューズカラー
				// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
				// ライトの角度減衰率の蓄積値         += ライトの角度減衰率
				TotalDiffuse  += g_Common.Light[ 1 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 1 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen * PSInput.Light1_Dir_Gen.w ;

			#endif // TOON == 1
		#else // TOON
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TotalDiffuse += ( g_Common.Light[ 1 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 1 ].Ambient.xyz ) * PSInput.Light1_Dir_Gen.w ;
			
		#endif // TOON

	#endif // LG1_DIR


	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = PSInput.V_to_Eye + PSInput.Light1_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		#if LG1_DIR

			// スペキュラカラー蓄積値 += Temp * ライトのスペキュラカラー
			TotalSpecular += Temp * g_Common.Light[ 1 ].Specular * ShadowGen ;

		#else // LG1_DIR

			// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
			TotalSpecular += Temp * PSInput.Light1_Dir_Gen.w * g_Common.Light[ 1 ].Specular * ShadowGen ;

		#endif // LG1_DIR

	#endif // USE_SPE

#endif // LG1_USE
// ライト1の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









			
// ライト2の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG2_USE

	// ディフューズ色計算

	if( g_Base.NoLightAngleAttenuation == 1 )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// DiffuseAngleGen = ディフューズ角度減衰率計算
		lLightDot = dot( Normal, PSInput.Light2_Dir_Gen.xyz ) ;
		if( g_Base.UseHalfLambert == 1 )
		{
			lLightDot = lLightDot * 0.5f + 0.5 ;
			DiffuseAngleGen = lLightDot * lLightDot ;
		}
		else
		{
			DiffuseAngleGen = saturate( lLightDot ) ;
		}
	}

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, g_ShadowMap.Data[ 0 ].Enable_Light2 ) *
	            max( ShadowRate.y, g_ShadowMap.Data[ 1 ].Enable_Light2 ) *
	            max( ShadowRate.z, g_ShadowMap.Data[ 2 ].Enable_Light2 ) ;

	#if LG2_DIR

		#if TOON
			#if TOON == 1
				// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
				// ディフューズカラー蓄積値 += ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
				TempF3             = g_Common.Light[ 2 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 ;
				TotalDiffuse      += TempF3 + g_Common.Light[ 2 ].Ambient.xyz ;

			#else // TOON == 1
				// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
				// ディフューズカラー蓄積値   += ライトのディフューズカラー
				// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
				// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
				TotalDiffuse  += g_Common.Light[ 2 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 2 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen ;

			#endif // TOON == 1
		#else // TOON
			// ディフューズカラー蓄積値 += ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
			TotalDiffuse += g_Common.Light[ 2 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 2 ].Ambient.xyz ;

		#endif //TOON

	#else // LG2_DIR

		#if TOON
			#if TOON == 1
				// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
				// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
				TempF3             = g_Common.Light[ 2 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 * PSInput.Light2_Dir_Gen.w ;
				TotalDiffuse      += ( TempF3 + g_Common.Light[ 2 ].Ambient.xyz ) * PSInput.Light2_Dir_Gen.w ;

			#else // TOON == 1
				// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
				// ディフューズカラー蓄積値           += ライトのディフューズカラー
				// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
				// ライトの角度減衰率の蓄積値         += ライトの角度減衰率
				TotalDiffuse  += g_Common.Light[ 2 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 2 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen * PSInput.Light2_Dir_Gen.w ;

			#endif // TOON == 1
		#else // TOON
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TotalDiffuse += ( g_Common.Light[ 2 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 2 ].Ambient.xyz ) * PSInput.Light2_Dir_Gen.w ;
			
		#endif // TOON

	#endif // LG2_DIR


	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		TempF3 = PSInput.V_to_Eye + PSInput.Light2_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		#if LG2_DIR

			// スペキュラカラー蓄積値 += Temp * ライトのスペキュラカラー
			TotalSpecular += Temp * g_Common.Light[ 2 ].Specular * ShadowGen ;

		#else // LG2_DIR

			// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
			TotalSpecular += Temp * PSInput.Light2_Dir_Gen.w * g_Common.Light[ 2 ].Specular * ShadowGen ;

		#endif // LG2_DIR

	#endif // USE_SPE

#endif // LG2_USE
// ライト2の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


















			// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

			#if TOON != 2
				// TotalDiffuse = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
				TotalDiffuse += g_Common.Material.Ambient_Emissive.xyz ;
			#else // TOON != 2
				// アンビエントカラーの蓄積値 += マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの
				TotalAmbient += g_Common.Material.Ambient_Emissive.rgb ;
			#endif  // TOON != 2

			#ifdef USE_SPE
				#if TOON
					#if TOON == 1
						// トゥーン１の時の出力計算
						ToonSpecularGradColor = g_ToonSpecularGradTexture.Sample( g_ToonSpecularGradSampler, max( TotalSpecular.b, max( TotalSpecular.r, TotalSpecular.g ) ) ) ;
					
						#ifdef USE_SPETEX
							TextureSpecularColor.rgb *= ToonSpecularGradColor.rgb ;
						#else  // USE_SPETEX
							TextureSpecularColor = ToonSpecularGradColor ;
						#endif // USE_SPETEX
					
						TextureSpecularColor.rgb *= g_ToonRGBtoVMaxRGBVolumeTexture.Sample( g_ToonRGBtoVMaxRGBVolumeSampler, TotalSpecular ).rgb * PSInput.Specular.rgb ;

						TextureDiffuseColor.rgb *= g_ToonRGBtoVMaxRGBVolumeTexture.Sample( g_ToonRGBtoVMaxRGBVolumeSampler, TotalLightDiffuse ).rgb * PSInput.Diffuse.rgb ;

						ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, max( TotalDiffuse.b, max( TotalDiffuse.r, TotalDiffuse.g ) ) ) ;

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

						#if TOON_DIFOP_MUL
							ParamAlpha *= ToonDiffuseGradColor.a ;
						#endif // TOON_DIFOP_MUL

						PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;
					
					#else  // TOON == 1
						// トゥーン２の時の出力計算
						ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, TotalAngleGen ) ;

						// 出力 = saturate( saturate( アンビエントカラーの蓄積値 + マテリアルのディフューズカラー * ライトのディフューズカラーの蓄積値 ) * テクスチャグラデーションカラー + スペキュラカラー蓄積値 * マテリアルのスペキュラカラー ) * テクスチャカラー
						PSOutput.Color0.rgb = saturate( saturate( TotalDiffuse * PSInput.Diffuse.rgb + TotalAmbient ) * ToonDiffuseGradColor.rgb + PSInput.Specular.rgb * TotalSpecular ) * TextureDiffuseColor.rgb ;
					
						#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
							// スフィアマップの色を取得
							TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + 0.5f ;
							ToonSphereMapColor = g_ToonSphereMapTexture.Sample( g_ToonSphereMapSampler, TempF3.xy ) ;
						
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
							PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha * ToonSphereMapColor.a ;

						#else // TOON_SPHEREOP_MUL

							// 出力アルファ = テクスチャアルファ * Factorアルファ * 頂点アルファ
							PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

						#endif // TOON_SPHEREOP_MUL

					#endif   // TOON == 1
				#else  // TOON

					#ifdef USE_SPETEX
						// TextureSpecularColor = ライトのスペキュラカラー蓄積値 * スペキュラカラー * スペキュラテクスチャカラー
						TextureSpecularColor.xyz = g_SpecularMapTexture.Sample( g_SpecularMapSampler, PSInput.TexCoords0_1.xy ).xyz * TotalSpecular * PSInput.Specular.xyz ;
					#else  // USE_SPETEX
						// TextureSpecularColor = ライトのスペキュラカラー蓄積値 * スペキュラカラー
						TextureSpecularColor.xyz = TotalSpecular.xyz * PSInput.Specular.xyz ;
					#endif // USE_SPETEX

					// 出力カラー = TotalDiffuse * テクスチャカラー + TextureSpecularColor

					PSOutput.Color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse + TextureSpecularColor.rgb ;
					PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

				#endif // TOON

			#else  // USE_SPE

				#if TOON
					#if TOON == 1
						ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, max( TotalDiffuse.b, max( TotalDiffuse.r, TotalDiffuse.g ) ) ) ;
						TextureDiffuseColor.rgb *= g_ToonRGBtoVMaxRGBVolumeTexture.Sample( g_ToonRGBtoVMaxRGBVolumeSampler, TotalLightDiffuse ).rgb * PSInput.Diffuse.rgb ;

						#if TOON_DIFOP_MUL
							// 出力カラー = テクスチャカラー * ディフューズカラー蓄積値 * グラデーションカラー
							PSOutput.Color0 = TextureDiffuseColor * ToonDiffuseGradColor ;
						#endif // TOON_DIFOP_MUL
						#if TOON_DIFOP_TRANS
							// 出力カラー = グラデーションカラー * グラデーションα + ( 1 - グラデーションα ) * ( テクスチャカラー * ディフューズカラー蓄積値 )
							PSOutput.Color0 = lerp( TextureDiffuseColor, ToonDiffuseGradColor, ToonDiffuseGradColor.a ) ;
						#endif // TOON_DIFOP_TRANS

						// 乗算の場合はα値もグラデーションαと乗算する
						#if TOON_DIFOP_MUL
							ParamAlpha *= ToonDiffuseGradColor.a ;
						#endif // TOON_DIFOP_MUL

						// 出力α = テクスチャα * ディフューズα * 大域α
						PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

					#else	// TOON == 1
						// トゥーン２の時の出力計算
						ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, TotalAngleGen ) ;

						// 出力 = ( アンビエントカラーの蓄積値 + マテリアルのディフューズカラー * ライトのディフューズカラーの蓄積値 ) * テクスチャグラデーションカラー * テクスチャカラー
						PSOutput.Color0.rgb = saturate( TotalDiffuse * PSInput.Diffuse.rgb + TotalAmbient.rgb ) * ToonDiffuseGradColor.rgb * TextureDiffuseColor.rgb ;

						#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
							// スフィアマップの色を取得
							TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + 0.5f ;
							ToonSphereMapColor = g_ToonSphereMapTexture.Sample( g_ToonSphereMapSampler, TempF3.xy ) ;
						
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

							PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha * ToonSphereMapColor.a ;

						#else // TOON_SPHEREOP_MUL

							PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

						#endif // TOON_SPHEREOP_MUL

					#endif		// TOON == 1
				#else  // TOON
					// 出力カラー = TotalDiffuse * テクスチャカラー
					PSOutput.Color0.rgb = TotalDiffuse * TextureDiffuseColor.rgb ;

					// 出力α = テクスチャα * ディフューズα * 大域α
					PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;
				#endif // TOON

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

	// アルファテスト
	AlphaTest( PSOutput.Color0.a ) ;

	// フォグ効果
	PSOutput.Color0.rgb = lerp( g_Common.Fog.Color.rgb, PSOutput.Color0.rgb, saturate( PSInput.Normal_Fog.w ) );

	// 高さフォグ効果
	PSOutput.Color0.rgb = lerp( g_Common.VerticalFog.Color.rgb, PSOutput.Color0.rgb, saturate( PSInput.VerticalFog ) );

	// 単純色加算
	PSOutput.Color0.r += g_Base.DrawAddColor.r ;
	PSOutput.Color0.g += g_Base.DrawAddColor.g ;
	PSOutput.Color0.b += g_Base.DrawAddColor.b ;

	// アルファ乗算カラー
	if( g_Base.MulAlphaColor.x > 0.5f )
	{
		PSOutput.Color0.rgb *= PSOutput.Color0.a;
	}

#endif // MAT_TYPE

	return PSOutput ;
}


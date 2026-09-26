#include "../PixelShader.h"


// ピクセルシェーダーの入力
struct PS_INPUT
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
	float2 Fog             : TEXCOORD5 ;	// フォグパラメータ( x )   高さフォグパラメータ( y )

#if SHADOWMAP
	float3 ShadowMap0Pos   : TEXCOORD6 ;	// シャドウマップ０のライト座標( x, y, z )
	float3 ShadowMap1Pos   : TEXCOORD7 ;	// シャドウマップ１のライト座標( x, y, z )
	float3 ShadowMap2Pos   : TEXCOORD8 ;	// シャドウマップ２のライト座標( x, y, z )
#endif // SHADOWMAP
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
#ifndef NORMAL_DEPTH_DRAW
	float4 Color0          : SV_TARGET0 ;	// 色
	float4 Normal          : SV_TARGET1 ;	// 法線( ビュー空間 )
	float4 Depth           : SV_TARGET2 ;	// 深度
#else // NORMAL_DEPTH_DRAW
	float4 Normal          : SV_TARGET0 ;	// 法線( ビュー空間 )
	float4 Position        : SV_TARGET1 ;	// 座標( ビュー空間 )
#endif // NORMAL_DEPTH_DRAW
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


#define SHADOWMAP0_ENABLE_LGT0 g_ShadowMap.Data[ 0 ].Enable_Light0
#define SHADOWMAP0_ENABLE_LGT1 g_ShadowMap.Data[ 0 ].Enable_Light1
#define SHADOWMAP0_ENABLE_LGT2 g_ShadowMap.Data[ 0 ].Enable_Light2
#define SHADOWMAP0_ENABLE_LGT3 0.0f
#define SHADOWMAP0_ENABLE_LGT4 0.0f
#define SHADOWMAP0_ENABLE_LGT5 0.0f

#define SHADOWMAP1_ENABLE_LGT0 g_ShadowMap.Data[ 1 ].Enable_Light0
#define SHADOWMAP1_ENABLE_LGT1 g_ShadowMap.Data[ 1 ].Enable_Light1
#define SHADOWMAP1_ENABLE_LGT2 g_ShadowMap.Data[ 1 ].Enable_Light2
#define SHADOWMAP1_ENABLE_LGT3 0.0f
#define SHADOWMAP1_ENABLE_LGT4 0.0f
#define SHADOWMAP1_ENABLE_LGT5 0.0f

#define SHADOWMAP2_ENABLE_LGT0 g_ShadowMap.Data[ 2 ].Enable_Light0
#define SHADOWMAP2_ENABLE_LGT1 g_ShadowMap.Data[ 2 ].Enable_Light1
#define SHADOWMAP2_ENABLE_LGT2 g_ShadowMap.Data[ 2 ].Enable_Light2
#define SHADOWMAP2_ENABLE_LGT3 0.0f
#define SHADOWMAP2_ENABLE_LGT4 0.0f
#define SHADOWMAP2_ENABLE_LGT5 0.0f





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
	
	float		DiffuseAngleGen ;
	float3		TotalDiffuse ;
	float3		TotalSpecular ;
	float3		TotalLightDiffuse ;
	float3		TotalAmbient ;
	float		TotalAngleGen ;
	float		Temp ;
	float3		TempF3 ;
	float3		Normal ;
	float3		TextureNormal ;
	float3		V_to_Eye ;
	float3		VNrm ;
	float3		VTan ;
	float3		VBin ;
	float3		lLightTemp ;
	float		lLightDot ;
	float		lLightDistancePow2 ;
	float		lLightGen ;
	float3		lLightDir ;
	float		lLightDirectionCosA ;
	float3		lLightBumpDir ;
	float3		ShadowRate ;
	float		ShadowGen ;
	float		OutputAlpha ;

#if SHADOWMAP
	float2		DepthTexCoord ;
	float4		TextureDepth ;
//	float4		variance_md_p_pmax ;
#endif
	
#if SUBTEXTUREMODE != 0
	float4 SubTextureColor;
#endif  // SUBTEXTUREMODE != 0

	OutputAlpha = 0.0 ;
	TextureSpecularColor.xyzw = 0.0 ;

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
		TextureNormal = ( g_NormalMapTexture.Sample( g_NormalMapSampler, PSInput.TexCoords0_1.xy ).rgb - 0.5f ) * 2.0f ;

		// テクスチャ法線をビュー空間法線に変換する
		Normal.x = TextureNormal.x * VTan.x + TextureNormal.y * VBin.x + TextureNormal.z * VNrm.x ;
		Normal.y = TextureNormal.x * VTan.y + TextureNormal.y * VBin.y + TextureNormal.z * VNrm.y ;
		Normal.z = TextureNormal.x * VTan.z + TextureNormal.y * VBin.z + TextureNormal.z * VNrm.z ;
		Normal = normalize( Normal ) ;
	#else // BUMPMAP
		Normal = normalize( PSInput.VNormal ) ;
	#endif // BUMPMAP

	// ディフューズテクスチャカラーを取得
	TextureDiffuseColor = g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords0_1.xy ) ;
	
#ifdef USE_SPE
	#ifdef USE_SPETEX
	// デスペキュラテクスチャカラーを取得
	TextureSpecularColor = g_SpecularMapTexture.Sample( g_SpecularMapSampler, PSInput.TexCoords0_1.xy ) ;
	#endif // USE_SPETEX
#endif // USE_SPE

#if SUBTEXTUREMODE != 0
	// サブテクスチャカラーを取得
	SubTextureColor = g_SubTexture.Sample( g_SubSampler, PSInput.TexCoords0_1.zw );
#endif  // SUBTEXTUREMODE != 0

	// サブテクスチャカラーとディフューズテクスチャカラーの掛け合わせ
#if SUBTEXTUREMODE == 1  // αブレンド( MV1_LAYERBLEND_TYPE_TRANSLUCENT + 1 )
	TextureDiffuseColor = lerp( TextureDiffuseColor, SubTextureColor, SubTextureColor.a );
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
	TotalDiffuse = 0.0f ;
#ifdef USE_SPE
	TotalSpecular = 0.0f ;
#endif // USE_SPE
	
#if TOON
	#if TOON == 1
		TotalLightDiffuse = 0.0f ;
	#else // TOON == 1
		TotalAmbient = 0.0f ;
		TotalAngleGen = 0.0f ;
	#endif
#endif // TOON

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
			// PSInput.ShadowMap2Pos_ShadowMap3PosX.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
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


	
// ライト0の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG0_USE

	#if LG_USE_IF
	if( g_Common.Light[ 0 ].Type == 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG0_DIR || LG_USE_IF

			// ライトの方向計算
			lLightDir = g_Common.Light[ 0 ].Direction ;

		#endif // LG0_DIR

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if LG0_DIR == 0 || LG_USE_IF

			// ライト方向ベクトルの計算
			lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 0 ].Position ) ;

		#endif  // LG0_DIR == 0 || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF

	// 法線マップ使用時のライト角度計算
	#ifdef BUMPMAP

	lLightBumpDir.x = dot( VTan, -lLightDir ) ;
	lLightBumpDir.y = dot( VBin, -lLightDir ) ;
	lLightBumpDir.z = dot( VNrm, -lLightDir ) ;
	lLightBumpDir = normalize( lLightBumpDir ) ;

	#endif // BUMPMAP		

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG_USE_IF
	if( g_Common.Light[ 0 ].Type != 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG0_SPOT || LG0_POINT || LG_USE_IF

			// 距離減衰計算 ------------------

			// 頂点とライト位置との距離の二乗を求める
			lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 0 ].Position ;
			lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

			// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
			lLightGen = 1.0f / ( g_Common.Light[ 0 ].Attenuation0 + g_Common.Light[ 0 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 0 ].Attenuation2 * lLightDistancePow2 ) ;

			// --------------------------------


			// スポットライト減衰計算 --------
			#if LG_USE_IF
			if( g_Common.Light[ 0 ].Type == 2 /* DX_LIGHTTYPE_SPOT */ )
			{
			#endif // LG_USE_IF

				#if LG0_SPOT || LG_USE_IF

					// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
					lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 0 ].Direction ) ;

					// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
					lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 0 ].SpotParam0, 0.0f ) * g_Common.Light[ 0 ].SpotParam1 ), g_Common.Light[ 0 ].FallOff ) ) ;

				#endif // LG0_SPOT || LG_USE_IF

			#if LG_USE_IF
			}
			#endif // LG_USE_IF
			// --------------------------------

			// 有効距離外だったら減衰率を最大にする処理
			lLightGen *= step( lLightDistancePow2, g_Common.Light[ 0 ].RangePow2 ) ;

		#endif // LG0_SPOT || LG0_POINT || LG_USE_IF

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if ( LG0_SPOT == 0 && LG0_POINT == 0 ) || LG_USE_IF
		
			lLightGen = 1.0f ;
			
		#endif // ( LG0_SPOT == 0 && LG0_POINT == 0 ) || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT0 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT0 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT0 ) ;


	// ディフューズ色計算
	if( g_Base.NoLightAngleAttenuation == 1 )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
	#ifdef BUMPMAP

		lLightDot = dot( TextureNormal, lLightBumpDir ) ;

	#else // BUMPMAP

		lLightDot = dot( Normal, -lLightDir ) ;

	#endif // BUMPMAP

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
	
	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF3             = g_Common.Light[ 0 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF3 * lLightGen * ShadowGen ;
			TotalDiffuse      += ( TempF3 + g_Common.Light[ 0 ].Ambient.xyz ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse  += g_Common.Light[ 0 ].Diffuse ;

			TotalAmbient  += g_Common.Light[ 0 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( g_Common.Light[ 0 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 0 ].Ambient.xyz ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		#ifdef BUMPMAP
			TempF3 = normalize( V_to_Eye + lLightBumpDir ) ;
		#else // BUMPMAP
			TempF3 = normalize( V_to_Eye - lLightDir ) ;
		#endif // BUMPMAP
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		#ifdef BUMPMAP
			Temp = pow( max( 0.0f, dot( TextureNormal, TempF3 ) ), g_Common.Material.Power ) ;
		#else // BUMPMAP
			Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;
		#endif // BUMPMAP

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 0 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG0_USE
// ライト0の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト1の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG1_USE

	#if LG_USE_IF
	if( g_Common.Light[ 1 ].Type == 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG1_DIR || LG_USE_IF

			// ライトの方向計算
			lLightDir = g_Common.Light[ 1 ].Direction ;

		#endif // LG1_DIR

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if LG1_DIR == 0 || LG_USE_IF

			// ライト方向ベクトルの計算
			lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 1 ].Position ) ;

		#endif  // LG1_DIR == 0 || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF

	// 法線マップ使用時のライト角度計算
	#ifdef BUMPMAP

	lLightBumpDir.x = dot( VTan, -lLightDir ) ;
	lLightBumpDir.y = dot( VBin, -lLightDir ) ;
	lLightBumpDir.z = dot( VNrm, -lLightDir ) ;
	lLightBumpDir = normalize( lLightBumpDir ) ;

	#endif // BUMPMAP		

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG_USE_IF
	if( g_Common.Light[ 1 ].Type != 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG1_SPOT || LG1_POINT || LG_USE_IF

			// 距離減衰計算 ------------------

			// 頂点とライト位置との距離の二乗を求める
			lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 1 ].Position ;
			lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

			// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
			lLightGen = 1.0f / ( g_Common.Light[ 1 ].Attenuation0 + g_Common.Light[ 1 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 1 ].Attenuation2 * lLightDistancePow2 ) ;

			// --------------------------------


			// スポットライト減衰計算 --------
			#if LG_USE_IF
			if( g_Common.Light[ 1 ].Type == 2 /* DX_LIGHTTYPE_SPOT */ )
			{
			#endif // LG_USE_IF

				#if LG1_SPOT || LG_USE_IF

					// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
					lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 1 ].Direction ) ;

					// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
					lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 1 ].SpotParam0, 0.0f ) * g_Common.Light[ 1 ].SpotParam1 ), g_Common.Light[ 1 ].FallOff ) ) ;

				#endif // LG1_SPOT || LG_USE_IF

			#if LG_USE_IF
			}
			#endif // LG_USE_IF
			// --------------------------------

			// 有効距離外だったら減衰率を最大にする処理
			lLightGen *= step( lLightDistancePow2, g_Common.Light[ 1 ].RangePow2 ) ;

		#endif // LG1_SPOT || LG1_POINT || LG_USE_IF

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if ( LG1_SPOT == 0 && LG1_POINT == 0 ) || LG_USE_IF
		
			lLightGen = 1.0f ;
			
		#endif // ( LG1_SPOT == 0 && LG1_POINT == 0 ) || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT1 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT1 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT1 ) ;


	// ディフューズ色計算
	if( g_Base.NoLightAngleAttenuation == 1 )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
	#ifdef BUMPMAP

		lLightDot = dot( TextureNormal, lLightBumpDir ) ;

	#else // BUMPMAP

		lLightDot = dot( Normal, -lLightDir ) ;

	#endif // BUMPMAP

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
	
	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF3             = g_Common.Light[ 1 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF3 * lLightGen * ShadowGen ;
			TotalDiffuse      += ( TempF3 + g_Common.Light[ 1 ].Ambient.xyz ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse  += g_Common.Light[ 1 ].Diffuse ;

			TotalAmbient  += g_Common.Light[ 1 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( g_Common.Light[ 1 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 1 ].Ambient.xyz ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		#ifdef BUMPMAP
			TempF3 = normalize( V_to_Eye + lLightBumpDir ) ;
		#else // BUMPMAP
			TempF3 = normalize( V_to_Eye - lLightDir ) ;
		#endif // BUMPMAP
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		#ifdef BUMPMAP
			Temp = pow( max( 0.0f, dot( TextureNormal, TempF3 ) ), g_Common.Material.Power ) ;
		#else // BUMPMAP
			Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;
		#endif // BUMPMAP

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 1 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG1_USE
// ライト1の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト2の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG2_USE

	#if LG_USE_IF
	if( g_Common.Light[ 2 ].Type == 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG2_DIR || LG_USE_IF

			// ライトの方向計算
			lLightDir = g_Common.Light[ 2 ].Direction ;

		#endif // LG2_DIR

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if LG2_DIR == 0 || LG_USE_IF

			// ライト方向ベクトルの計算
			lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 2 ].Position ) ;

		#endif  // LG2_DIR == 0 || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF

	// 法線マップ使用時のライト角度計算
	#ifdef BUMPMAP

	lLightBumpDir.x = dot( VTan, -lLightDir ) ;
	lLightBumpDir.y = dot( VBin, -lLightDir ) ;
	lLightBumpDir.z = dot( VNrm, -lLightDir ) ;
	lLightBumpDir = normalize( lLightBumpDir ) ;

	#endif // BUMPMAP		

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG_USE_IF
	if( g_Common.Light[ 2 ].Type != 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG2_SPOT || LG2_POINT || LG_USE_IF

			// 距離減衰計算 ------------------

			// 頂点とライト位置との距離の二乗を求める
			lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 2 ].Position ;
			lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

			// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
			lLightGen = 1.0f / ( g_Common.Light[ 2 ].Attenuation0 + g_Common.Light[ 2 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 2 ].Attenuation2 * lLightDistancePow2 ) ;

			// --------------------------------


			// スポットライト減衰計算 --------
			#if LG_USE_IF
			if( g_Common.Light[ 2 ].Type == 2 /* DX_LIGHTTYPE_SPOT */ )
			{
			#endif // LG_USE_IF

				#if LG2_SPOT || LG_USE_IF

					// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
					lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 2 ].Direction ) ;

					// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
					lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 2 ].SpotParam0, 0.0f ) * g_Common.Light[ 2 ].SpotParam1 ), g_Common.Light[ 2 ].FallOff ) ) ;

				#endif // LG2_SPOT || LG_USE_IF

			#if LG_USE_IF
			}
			#endif // LG_USE_IF
			// --------------------------------

			// 有効距離外だったら減衰率を最大にする処理
			lLightGen *= step( lLightDistancePow2, g_Common.Light[ 2 ].RangePow2 ) ;

		#endif // LG2_SPOT || LG2_POINT || LG_USE_IF

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if ( LG2_SPOT == 0 && LG2_POINT == 0 ) || LG_USE_IF
		
			lLightGen = 1.0f ;
			
		#endif // ( LG2_SPOT == 0 && LG2_POINT == 0 ) || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT2 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT2 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT2 ) ;


	// ディフューズ色計算
	if( g_Base.NoLightAngleAttenuation == 1 )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
	#ifdef BUMPMAP

		lLightDot = dot( TextureNormal, lLightBumpDir ) ;

	#else // BUMPMAP

		lLightDot = dot( Normal, -lLightDir ) ;

	#endif // BUMPMAP

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
	
	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF3             = g_Common.Light[ 2 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF3 * lLightGen * ShadowGen ;
			TotalDiffuse      += ( TempF3 + g_Common.Light[ 2 ].Ambient.xyz ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse  += g_Common.Light[ 2 ].Diffuse ;

			TotalAmbient  += g_Common.Light[ 2 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( g_Common.Light[ 2 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 2 ].Ambient.xyz ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		#ifdef BUMPMAP
			TempF3 = normalize( V_to_Eye + lLightBumpDir ) ;
		#else // BUMPMAP
			TempF3 = normalize( V_to_Eye - lLightDir ) ;
		#endif // BUMPMAP
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		#ifdef BUMPMAP
			Temp = pow( max( 0.0f, dot( TextureNormal, TempF3 ) ), g_Common.Material.Power ) ;
		#else // BUMPMAP
			Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;
		#endif // BUMPMAP

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 2 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG2_USE
// ライト2の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト3の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG3_USE

	#if LG_USE_IF
	if( g_Common.Light[ 3 ].Type == 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG3_DIR || LG_USE_IF

			// ライトの方向計算
			lLightDir = g_Common.Light[ 3 ].Direction ;

		#endif // LG3_DIR

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if LG3_DIR == 0 || LG_USE_IF

			// ライト方向ベクトルの計算
			lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 3 ].Position ) ;

		#endif  // LG3_DIR == 0 || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF

	// 法線マップ使用時のライト角度計算
	#ifdef BUMPMAP

	lLightBumpDir.x = dot( VTan, -lLightDir ) ;
	lLightBumpDir.y = dot( VBin, -lLightDir ) ;
	lLightBumpDir.z = dot( VNrm, -lLightDir ) ;
	lLightBumpDir = normalize( lLightBumpDir ) ;

	#endif // BUMPMAP		

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG_USE_IF
	if( g_Common.Light[ 3 ].Type != 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG3_SPOT || LG3_POINT || LG_USE_IF

			// 距離減衰計算 ------------------

			// 頂点とライト位置との距離の二乗を求める
			lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 3 ].Position ;
			lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

			// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
			lLightGen = 1.0f / ( g_Common.Light[ 3 ].Attenuation0 + g_Common.Light[ 3 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 3 ].Attenuation2 * lLightDistancePow2 ) ;

			// --------------------------------


			// スポットライト減衰計算 --------
			#if LG_USE_IF
			if( g_Common.Light[ 3 ].Type == 2 /* DX_LIGHTTYPE_SPOT */ )
			{
			#endif // LG_USE_IF

				#if LG3_SPOT || LG_USE_IF

					// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
					lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 3 ].Direction ) ;

					// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
					lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 3 ].SpotParam0, 0.0f ) * g_Common.Light[ 3 ].SpotParam1 ), g_Common.Light[ 3 ].FallOff ) ) ;

				#endif // LG3_SPOT || LG_USE_IF

			#if LG_USE_IF
			}
			#endif // LG_USE_IF
			// --------------------------------

			// 有効距離外だったら減衰率を最大にする処理
			lLightGen *= step( lLightDistancePow2, g_Common.Light[ 3 ].RangePow2 ) ;

		#endif // LG3_SPOT || LG3_POINT || LG_USE_IF

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if ( LG3_SPOT == 0 && LG3_POINT == 0 ) || LG_USE_IF
		
			lLightGen = 1.0f ;
			
		#endif // ( LG3_SPOT == 0 && LG3_POINT == 0 ) || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT3 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT3 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT3 ) ;


	// ディフューズ色計算
	if( g_Base.NoLightAngleAttenuation == 1 )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
	#ifdef BUMPMAP

		lLightDot = dot( TextureNormal, lLightBumpDir ) ;

	#else // BUMPMAP

		lLightDot = dot( Normal, -lLightDir ) ;

	#endif // BUMPMAP

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
	
	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF3             = g_Common.Light[ 3 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF3 * lLightGen * ShadowGen ;
			TotalDiffuse      += ( TempF3 + g_Common.Light[ 3 ].Ambient.xyz ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse  += g_Common.Light[ 3 ].Diffuse ;

			TotalAmbient  += g_Common.Light[ 3 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( g_Common.Light[ 3 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 3 ].Ambient.xyz ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		#ifdef BUMPMAP
			TempF3 = normalize( V_to_Eye + lLightBumpDir ) ;
		#else // BUMPMAP
			TempF3 = normalize( V_to_Eye - lLightDir ) ;
		#endif // BUMPMAP
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		#ifdef BUMPMAP
			Temp = pow( max( 0.0f, dot( TextureNormal, TempF3 ) ), g_Common.Material.Power ) ;
		#else // BUMPMAP
			Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;
		#endif // BUMPMAP

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 3 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG3_USE
// ライト3の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト4の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG4_USE

	#if LG_USE_IF
	if( g_Common.Light[ 4 ].Type == 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG4_DIR || LG_USE_IF

			// ライトの方向計算
			lLightDir = g_Common.Light[ 4 ].Direction ;

		#endif // LG4_DIR

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if LG4_DIR == 0 || LG_USE_IF

			// ライト方向ベクトルの計算
			lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 4 ].Position ) ;

		#endif  // LG4_DIR == 0 || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF

	// 法線マップ使用時のライト角度計算
	#ifdef BUMPMAP

	lLightBumpDir.x = dot( VTan, -lLightDir ) ;
	lLightBumpDir.y = dot( VBin, -lLightDir ) ;
	lLightBumpDir.z = dot( VNrm, -lLightDir ) ;
	lLightBumpDir = normalize( lLightBumpDir ) ;

	#endif // BUMPMAP		

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG_USE_IF
	if( g_Common.Light[ 4 ].Type != 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG4_SPOT || LG4_POINT || LG_USE_IF

			// 距離減衰計算 ------------------

			// 頂点とライト位置との距離の二乗を求める
			lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 4 ].Position ;
			lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

			// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
			lLightGen = 1.0f / ( g_Common.Light[ 4 ].Attenuation0 + g_Common.Light[ 4 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 4 ].Attenuation2 * lLightDistancePow2 ) ;

			// --------------------------------


			// スポットライト減衰計算 --------
			#if LG_USE_IF
			if( g_Common.Light[ 4 ].Type == 2 /* DX_LIGHTTYPE_SPOT */ )
			{
			#endif // LG_USE_IF

				#if LG4_SPOT || LG_USE_IF

					// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
					lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 4 ].Direction ) ;

					// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
					lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 4 ].SpotParam0, 0.0f ) * g_Common.Light[ 4 ].SpotParam1 ), g_Common.Light[ 4 ].FallOff ) ) ;

				#endif // LG4_SPOT || LG_USE_IF

			#if LG_USE_IF
			}
			#endif // LG_USE_IF
			// --------------------------------

			// 有効距離外だったら減衰率を最大にする処理
			lLightGen *= step( lLightDistancePow2, g_Common.Light[ 4 ].RangePow2 ) ;

		#endif // LG4_SPOT || LG4_POINT || LG_USE_IF

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if ( LG4_SPOT == 0 && LG4_POINT == 0 ) || LG_USE_IF
		
			lLightGen = 1.0f ;
			
		#endif // ( LG4_SPOT == 0 && LG4_POINT == 0 ) || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT4 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT4 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT4 ) ;


	// ディフューズ色計算
	if( g_Base.NoLightAngleAttenuation == 1 )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
	#ifdef BUMPMAP

		lLightDot = dot( TextureNormal, lLightBumpDir ) ;

	#else // BUMPMAP

		lLightDot = dot( Normal, -lLightDir ) ;

	#endif // BUMPMAP

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
	
	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF3             = g_Common.Light[ 4 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF3 * lLightGen * ShadowGen ;
			TotalDiffuse      += ( TempF3 + g_Common.Light[ 4 ].Ambient.xyz ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse  += g_Common.Light[ 4 ].Diffuse ;

			TotalAmbient  += g_Common.Light[ 4 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( g_Common.Light[ 4 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 4 ].Ambient.xyz ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		#ifdef BUMPMAP
			TempF3 = normalize( V_to_Eye + lLightBumpDir ) ;
		#else // BUMPMAP
			TempF3 = normalize( V_to_Eye - lLightDir ) ;
		#endif // BUMPMAP
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		#ifdef BUMPMAP
			Temp = pow( max( 0.0f, dot( TextureNormal, TempF3 ) ), g_Common.Material.Power ) ;
		#else // BUMPMAP
			Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;
		#endif // BUMPMAP

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 4 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG4_USE
// ライト4の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )









	
// ライト5の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
#if LG5_USE

	#if LG_USE_IF
	if( g_Common.Light[ 5 ].Type == 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG5_DIR || LG_USE_IF

			// ライトの方向計算
			lLightDir = g_Common.Light[ 5 ].Direction ;

		#endif // LG5_DIR

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if LG5_DIR == 0 || LG_USE_IF

			// ライト方向ベクトルの計算
			lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 5 ].Position ) ;

		#endif  // LG5_DIR == 0 || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF

	// 法線マップ使用時のライト角度計算
	#ifdef BUMPMAP

	lLightBumpDir.x = dot( VTan, -lLightDir ) ;
	lLightBumpDir.y = dot( VBin, -lLightDir ) ;
	lLightBumpDir.z = dot( VNrm, -lLightDir ) ;
	lLightBumpDir = normalize( lLightBumpDir ) ;

	#endif // BUMPMAP		

	
	// 距離・スポットライト減衰値計算 =======================================( 開始 )
	#if LG_USE_IF
	if( g_Common.Light[ 5 ].Type != 3 /* DX_LIGHTTYPE_DIRECTIONAL */ )
	{
	#endif // LG_USE_IF

		#if LG5_SPOT || LG5_POINT || LG_USE_IF

			// 距離減衰計算 ------------------

			// 頂点とライト位置との距離の二乗を求める
			lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 5 ].Position ;
			lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

			// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
			lLightGen = 1.0f / ( g_Common.Light[ 5 ].Attenuation0 + g_Common.Light[ 5 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 5 ].Attenuation2 * lLightDistancePow2 ) ;

			// --------------------------------


			// スポットライト減衰計算 --------
			#if LG_USE_IF
			if( g_Common.Light[ 5 ].Type == 2 /* DX_LIGHTTYPE_SPOT */ )
			{
			#endif // LG_USE_IF

				#if LG5_SPOT || LG_USE_IF

					// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
					lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 5 ].Direction ) ;

					// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
					lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 5 ].SpotParam0, 0.0f ) * g_Common.Light[ 5 ].SpotParam1 ), g_Common.Light[ 5 ].FallOff ) ) ;

				#endif // LG5_SPOT || LG_USE_IF

			#if LG_USE_IF
			}
			#endif // LG_USE_IF
			// --------------------------------

			// 有効距離外だったら減衰率を最大にする処理
			lLightGen *= step( lLightDistancePow2, g_Common.Light[ 5 ].RangePow2 ) ;

		#endif // LG5_SPOT || LG5_POINT || LG_USE_IF

	#if LG_USE_IF
	}
	else
	{
	#endif // LG_USE_IF

		#if ( LG5_SPOT == 0 && LG5_POINT == 0 ) || LG_USE_IF
		
			lLightGen = 1.0f ;
			
		#endif // ( LG5_SPOT == 0 && LG5_POINT == 0 ) || LG_USE_IF

	#if LG_USE_IF
	}
	#endif // LG_USE_IF
	
	// 距離・スポットライト減衰値計算 =======================================( 終了 )

	
	// 影による減衰率算出
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT5 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT5 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT5 ) ;


	// ディフューズ色計算
	if( g_Base.NoLightAngleAttenuation == 1 )
	{
		// 角度減衰を行わない場合は1をセット
		DiffuseAngleGen = 1.0f ;
	}
	else
	{
		// ディフューズ角度減衰率計算
	#ifdef BUMPMAP

		lLightDot = dot( TextureNormal, lLightBumpDir ) ;

	#else // BUMPMAP

		lLightDot = dot( Normal, -lLightDir ) ;

	#endif // BUMPMAP

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
	
	#if TOON
		#if TOON == 1
			// トゥーンの場合はこの時点では頂点orマテリアルのディフューズカラーは使用しない
			// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
			TempF3             = g_Common.Light[ 5 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF3 * lLightGen * ShadowGen ;
			TotalDiffuse      += ( TempF3 + g_Common.Light[ 5 ].Ambient.xyz ) * lLightGen ;

		#else // TOON == 1
			// トゥーン２の場合はディフューズカラーとライトの角度減衰は別々に保存しておく
			// ディフューズカラー蓄積値 += ライトのディフューズカラー
			// ライトのアンビエントカラーの蓄積値 += ライトのアンビエントカラー
			// ライトの角度減衰率の蓄積値 += ライトの角度減衰率
			TotalDiffuse  += g_Common.Light[ 5 ].Diffuse ;

			TotalAmbient  += g_Common.Light[ 5 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * ディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
		TotalDiffuse += ( g_Common.Light[ 5 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 5 ].Ambient.xyz ) * lLightGen ;

	#endif // TOON




	// スペキュラカラー計算
	#ifdef USE_SPE

		// ハーフベクトルの計算
		#ifdef BUMPMAP
			TempF3 = normalize( V_to_Eye + lLightBumpDir ) ;
		#else // BUMPMAP
			TempF3 = normalize( V_to_Eye - lLightDir ) ;
		#endif // BUMPMAP
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		#ifdef BUMPMAP
			Temp = pow( max( 0.0f, dot( TextureNormal, TempF3 ) ), g_Common.Material.Power ) ;
		#else // BUMPMAP
			Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;
		#endif // BUMPMAP

		// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 5 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG5_USE
// ライト5の処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )















	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	#if TOON != 2
		// TotalDiffuse = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
		TotalDiffuse += g_Common.Material.Ambient_Emissive.rgb ;
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

				#ifndef NORMAL_DEPTH_DRAW
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
				#endif // NORMAL_DEPTH_DRAW

				OutputAlpha = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a ;

				#if TOON_DIFOP_MUL
					OutputAlpha *= ToonDiffuseGradColor.a ;
				#endif // TOON_DIFOP_MUL
				
			#else  // TOON == 1
				// トゥーン２の時の出力計算

				#ifndef NORMAL_DEPTH_DRAW

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
				#endif // NORMAL_DEPTH_DRAW

				#if TOON_SPHEREOP_MUL

					// 出力アルファ = テクスチャアルファ * Factorアルファ * 頂点アルファ * スフィアマップアルファ
					OutputAlpha = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a * ToonSphereMapColor.a ;

				#else // TOON_SPHEREOP_MUL

					// 出力アルファ = テクスチャアルファ * Factorアルファ * 頂点アルファ
					OutputAlpha = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a ;

				#endif // TOON_SPHEREOP_MUL

			#endif   // TOON == 1
		#else  // TOON

			#ifndef NORMAL_DEPTH_DRAW

				#ifdef USE_SPETEX
					// TextureSpecularColor = スペキュラカラー * スペキュラテクスチャカラー
					TextureSpecularColor.xyz = g_SpecularMapTexture.Sample( g_SpecularMapSampler, PSInput.TexCoords0_1.xy ).xyz * PSInput.Specular.xyz ;
				#else  // USE_SPETEX
					// TextureSpecularColor = スペキュラカラー
					TextureSpecularColor.xyz = PSInput.Specular.xyz ;
				#endif // USE_SPETEX

				// 出力カラー = TotalDiffuse * テクスチャカラー + TextureSpecularColor * ライトのスペキュラカラー蓄積値
				PSOutput.Color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse + TextureSpecularColor.rgb * TotalSpecular ;

			#endif // NORMAL_DEPTH_DRAW

			OutputAlpha = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a ;

		#endif // TOON

	#else  // USE_SPE

		#if TOON
			#if TOON == 1

				ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, max( TotalDiffuse.b, max( TotalDiffuse.r, TotalDiffuse.g ) ) ) ;

				#ifndef NORMAL_DEPTH_DRAW

					TextureDiffuseColor.rgb *= g_ToonRGBtoVMaxRGBVolumeTexture.Sample( g_ToonRGBtoVMaxRGBVolumeSampler, TotalLightDiffuse ).rgb * PSInput.Diffuse.rgb ;

					#if TOON_DIFOP_MUL
						// 出力カラー = テクスチャカラー * ディフューズカラー蓄積値 * グラデーションカラー
						PSOutput.Color0 = TextureDiffuseColor * ToonDiffuseGradColor ;
					#endif // TOON_DIFOP_MUL
					#if TOON_DIFOP_TRANS
						// 出力カラー = グラデーションカラー * グラデーションα + ( 1 - グラデーションα ) * ( テクスチャカラー * ディフューズカラー蓄積値 )
						PSOutput.Color0 = lerp( TextureDiffuseColor, ToonDiffuseGradColor, ToonDiffuseGradColor.a ) ;
					#endif // TOON_DIFOP_TRANS

				#endif // NORMAL_DEPTH_DRAW

				// 出力α = テクスチャα * ディフューズα * 大域α
				OutputAlpha = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a ;

				// 乗算の場合はα値もグラデーションαと乗算する
				#if TOON_DIFOP_MUL
					OutputAlpha *= ToonDiffuseGradColor.a ;
				#endif // TOON_DIFOP_MUL

			#else	// TOON == 1
				// トゥーン２の時の出力計算

				#ifndef NORMAL_DEPTH_DRAW

					ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, TotalAngleGen ) ;

					// 出力 = ( アンビエントカラーの蓄積値 + マテリアルのディフューズカラー * ライトのディフューズカラーの蓄積値 ) * テクスチャグラデーションカラー * テクスチャカラー
					PSOutput.Color0.rgb = saturate( TotalDiffuse * PSInput.Diffuse.rgb + TotalAmbient ) * ToonDiffuseGradColor.rgb * TextureDiffuseColor.rgb ;

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

				#endif // NORMAL_DEPTH_DRAW

				#if TOON_SPHEREOP_MUL

					OutputAlpha = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a * ToonSphereMapColor.a ;

				#else // TOON_SPHEREOP_MUL

					OutputAlpha = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a ;

				#endif // TOON_SPHEREOP_MUL

			#endif		// TOON == 1
		#else  // TOON
			#ifndef NORMAL_DEPTH_DRAW

				// 出力カラー = TotalDiffuse * テクスチャカラー
				PSOutput.Color0.rgb = TotalDiffuse * TextureDiffuseColor.rgb ;

			#endif // NORMAL_DEPTH_DRAW

			// 出力α = テクスチャα * ディフューズα * 大域α
			OutputAlpha = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a ;
		#endif // TOON

	#endif // USE_SPE

	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	// ライトを使う場合 *******************************************************************( 終了 )

	// アルファテスト
	AlphaTest( OutputAlpha ) ;

	#ifndef NORMAL_DEPTH_DRAW

		// フォグ効果
		PSOutput.Color0.rgb = lerp( g_Common.Fog.Color.rgb, PSOutput.Color0.rgb, saturate( PSInput.Fog.x ) );

		// 高さフォグ効果
		PSOutput.Color0.rgb = lerp( g_Common.VerticalFog.Color.rgb, PSOutput.Color0.rgb, saturate( PSInput.Fog.y ) );

		// 単純色加算
		PSOutput.Color0.r += g_Base.DrawAddColor.r ;
		PSOutput.Color0.g += g_Base.DrawAddColor.g ;
		PSOutput.Color0.b += g_Base.DrawAddColor.b ;

		// アルファ乗算カラー
		if( g_Base.MulAlphaColor.x > 0.5f )
		{
			PSOutput.Color0.rgb *= OutputAlpha ;
		}

		PSOutput.Color0.a = OutputAlpha ;

		PSOutput.Depth.x  = PSInput.VPosition.z ;
		#ifdef USE_SPE
			PSOutput.Depth.y  = TextureSpecularColor.r * 0.299 + TextureSpecularColor.g * 0.587 + TextureSpecularColor.b * 0.114 ;
		#else // USE_SPE
			PSOutput.Depth.y  = 0.0 ;
		#endif // USE_SPE
		PSOutput.Depth.z  = 0.0 ;
		PSOutput.Depth.w  = 1.0 ;

	#else // NORMAL_DEPTH_DRAW

		PSOutput.Position.x = PSInput.VPosition.x ;
		PSOutput.Position.y = PSInput.VPosition.y ;
		PSOutput.Position.z = PSInput.VPosition.z ;
		PSOutput.Position.w = 1.0 ;

	#endif // NORMAL_DEPTH_DRAW

	PSOutput.Normal.x = ( Normal.x + 1.0 ) / 2.0 ;
	PSOutput.Normal.y = ( Normal.y + 1.0 ) / 2.0 ;
	PSOutput.Normal.z = ( Normal.z + 1.0 ) / 2.0 ;
	PSOutput.Normal.w = 1.0 ;

	return PSOutput ;
}


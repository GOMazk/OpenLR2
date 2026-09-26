// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		ピクセルシェーダー定義コード
// 
// 				Ver 3.17 
// 
// -------------------------------------------------------------------------------

// 共通部分

// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;		// ディフューズカラー
#if USE_SPE
	float4 Specular        : COLOR1 ;		// スペキュラカラー
#endif // USE_SPE
	float4 TexCoords0_1    : TEXCOORD0 ;	// xy:テクスチャ座標 zw:サブテクスチャ座標
	float3 VPosition       : TEXCOORD1 ;	// 頂点座標から視線へのベクトル( ビュー空間 )
	float3 VNormal         : TEXCOORD2 ;	// 法線( ビュー空間 )
#ifdef BUMPMAP
	float3 VTan            : TEXCOORD3 ;    // 接線( ビュー空間 )
	float3 VBin            : TEXCOORD4 ;    // 従法線( ビュー空間 )
#endif // BUMPMAP
	float2 Fog_ShadowMap3PosZ             : TEXCOORD5 ;	// フォグパラメータ( x )とシャドウマップ３のライト座標( y )

#if SHADOWMAP
	float4 ShadowMap1Pos_ShadowMap3PosX   : TEXCOORD6 ;	// シャドウマップ１のライト座標( x, y, z )とシャドウマップ３のライトX座標( w )
	float4 ShadowMap2Pos_ShadowMap3PosY   : TEXCOORD7 ;	// シャドウマップ２のライト座標( x, y, z )とシャドウマップ３のライトY座標( w )
#endif // SHADOWMAP
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;	// 色
} ;

// マテリアルパラメータ
struct MATERIAL
{
	float4 Diffuse ;      // ディフューズカラー
	float4 Specular ;     // スペキュラカラー
	float4 Power ;        // スペキュラの強さ
} ;

// ライトパラメータ
struct LIGHT
{
	float4 Position ;               // 座標( ビュー空間 )
	float3 Direction ;              // 方向( ビュー空間 )
	float4 Diffuse ;                // ディフューズカラー
	float4 Specular ;               // スペキュラカラー
	float4 Ambient ;                // アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
	float4 Range_FallOff_AT0_AT1 ;  // x:有効距離  y:スポットライト用FallOff  z:距離による減衰処理用パラメータ０  w:距離による減衰処理用パラメータ１
	float4 AT2_SpotP0_SpotP1 ;      // x:距離による減衰処理用パラメータ２  y:スポットライト用パラメータ０( cos( Phi / 2.0f ) )  z:スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )
} ;



// C++ 側で設定する定数の定義
sampler  DiffuseMapTexture             : register( s0 ) ;		// ディフューズマップテクスチャ

#if BUMPMAP
sampler  NormalMapTexture              : register( s1 ) ;		// 法線マップテクスチャ
#endif // BUMPMAP

#if USE_SPETEX
sampler  SpecularMapTexture            : register( s2 ) ;		// スペキュラマップテクスチャ
#endif // USE_SPETEX

#if TOON
sampler  ToonDiffuseGradTexture        : register( s3 ) ;		// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ
sampler  ToonSpecularGradTexture       : register( s4 ) ;		// トゥーンレンダリング用スペキュラカラーグラデーションテクスチャ
#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
sampler  ToonSphereMapTexture          : register( s5 ) ;		// トゥーンレンダリング用スフィアマップテクスチャ
#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
sampler  ToonRGBtoVMaxRGBVolumeTexture : register( s6 ) ;		// トゥーンレンダリング用RGB輝度飽和処理用ボリュームテクスチャ
#endif // TOON

#if SUBTEXTUREMODE != 0
sampler  SubTexture					   : register( s7 ) ;		// サブテクスチャ
#endif // SUBTEXTUREMODE != 0

#if SHADOWMAP
sampler  ShadowMap1Texture             : register( s8 ) ;		// シャドウマップ１テクスチャ
sampler  ShadowMap2Texture             : register( s9 ) ;		// シャドウマップ２テクスチャ
sampler  ShadowMap3Texture             : register( s10 ) ;		// シャドウマップ３テクスチャ
#endif

float4   cfZeroHalfOneTwo              : register( c0 /* DX_PS_CONSTF_ZERO_HALF_ONE_TWO           */ ) ;		// x=0.0f, y=0.5f, z=1.0f, w=2.0f
float4   cfAmbient_Emissive            : register( c1 /* DX_PS_CONSTF_AMBIENT_EMISSIVE            */ ) ;		// エミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

MATERIAL cfMaterial                    : register( c2 /* DX_PS_CONSTF_MATERIAL_START              */ ) ;		// マテリアルパラメータ

float4   cfFactorColor                 : register( c5 /* DX_PS_CONSTF_FACTORCOLOR                 */ ) ;		// アルファ値等
float4   cfToonOutLineColor            : register( c6 /* DX_PS_CONSTF_TOON_OUTLINE_COLOR          */ ) ;		// トゥーンレンダリングの輪郭線の色
float4   cfToonOutLineWidth            : register( c7 /* DX_PS_CONSTF_TOON_OUTLINE_WIDTH          */ ) ;		// トゥーンレンダリングの輪郭線の太さ
float4   cfFogColor                    : register( c8 /* DX_PS_CONSTF_FOG_COLOR                   */ ) ;		// フォグカラー

float4   cfShadowMap1_DAdj_Grad_Enbl0_1               : register( c18 /* DX_PS_CONSTF_SHADOW1_DADJ_GRAD_ENBL0_1             */ ) ;		// シャドウマップ１用の情報( x:閾値深度補正値  y:グラデーション範囲  zw:ライト０・１への適用情報)
float4   cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0 : register( c19 /* DX_PS_CONSTF_SHADOW1_ENBL2_SHADOW2_DADJ_GRAD_ENBL0 */ ) ;		// シャドウマップ１用の情報( x:ライト２への適用情報 )、シャドウマップ２用の情報( y:閾値深度補正値  z:グラデーション範囲  w:ライト０への適用情報 )
float4   cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad    : register( c20 /* DX_PS_CONSTF_SHADOW2_ENBL1_2_SHADOW3_DADJ_GRAD     */ ) ;		// シャドウマップ２用の情報( xy:ライト１・２への適用情報)、シャドウマップ３用の情報( z:閾値深度補正値  w:グラデーション範囲 )
float4   cfShadowMap3_Enbl0_1_2_LgtNoGen              : register( c21 /* DX_PS_CONSTF_SHADOW3_ENBL0_1_2_LGT_NO_GEN          */ ) ;		// シャドウマップ３用の情報( xyz:ライト０～２への適用情報 )
float4   cfMulAlphaColorAddColor                      : register( c22 /* DX_PS_CONSTF_MUL_ALPHA_COLOR_ADD_COLOR             */ ) ;		// カラーにアルファ値を乗算するかどうか( x( 0.0f:乗算しない  1.0f:乗算する ) ), 加算する色( RGB=yzw )
float4   cfIgnoreTextureColor          : register( c23 /* DX_PS_CONSTF_IGNORE_TEX_COLOR          */ ) ;		// テクスチャカラー無視処理用カラー
float4   cfHalfLambert                 : register( c24 /* DX_PS_CONSTF_USE_HALFLAMBERT                       */ ) ;		// テクスチャカラー無視処理用カラー

LIGHT    cfLight[ 6 ]                  : register( c32 /* DX_PS_CONSTF_LIGHT2_START               */ ) ;		// ライトパラメータ

#define SHADOWMAP1_ENABLE_LGT0 cfShadowMap1_DAdj_Grad_Enbl0_1.z
#define SHADOWMAP1_ENABLE_LGT1 cfShadowMap1_DAdj_Grad_Enbl0_1.w
#define SHADOWMAP1_ENABLE_LGT2 cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0.x
#define SHADOWMAP1_ENABLE_LGT3 cfZeroHalfOneTwo.x
#define SHADOWMAP1_ENABLE_LGT4 cfZeroHalfOneTwo.x
#define SHADOWMAP1_ENABLE_LGT5 cfZeroHalfOneTwo.x

#define SHADOWMAP2_ENABLE_LGT0 cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0.w
#define SHADOWMAP2_ENABLE_LGT1 cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.x
#define SHADOWMAP2_ENABLE_LGT2 cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.y
#define SHADOWMAP2_ENABLE_LGT3 cfZeroHalfOneTwo.x
#define SHADOWMAP2_ENABLE_LGT4 cfZeroHalfOneTwo.x
#define SHADOWMAP2_ENABLE_LGT5 cfZeroHalfOneTwo.x

#define SHADOWMAP3_ENABLE_LGT0 cfShadowMap3_Enbl0_1_2_LgtNoGen.x
#define SHADOWMAP3_ENABLE_LGT1 cfShadowMap3_Enbl0_1_2_LgtNoGen.y
#define SHADOWMAP3_ENABLE_LGT2 cfShadowMap3_Enbl0_1_2_LgtNoGen.z
#define SHADOWMAP3_ENABLE_LGT3 cfZeroHalfOneTwo.x
#define SHADOWMAP3_ENABLE_LGT4 cfZeroHalfOneTwo.x
#define SHADOWMAP3_ENABLE_LGT5 cfZeroHalfOneTwo.x

#define LIGHT_NO_DIR_GEN       cfShadowMap3_Enbl0_1_2_LgtNoGen.w
#define HALF_LAMBERT           cfHalfLambert.x




// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		頂点シェーダー定義コード
// 
// 				Ver 3.17 
// 
// -------------------------------------------------------------------------------

// 共通部分

#include "../../../Main/Windows/DxShader_VS_D3D9.h"

// 頂点シェーダーの入力
struct VS_INPUT
{
	float4 Position        : POSITION ;			// 座標( ローカル空間 )
	float3 Normal          : NORMAL0 ;			// 法線( ローカル空間 )
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
	float4 Specular        : COLOR1 ;			// スペキュラカラー
	float4 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標
	float4 TexCoords1      : TEXCOORD1 ;		// サブテクスチャ座標

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
	float4 Position        : POSITION ;		// 座標( プロジェクション空間 )
	float4 Diffuse         : COLOR0 ;		// ディフューズカラー
#if USE_SPE
	float4 Specular        : COLOR1 ;		// スペキュラカラー
#endif
	float4 TexCoords0_1    : TEXCOORD0 ;	// テクスチャ座標とサブテクスチャ座標( x:テクスチャ座標x  y:テクスチャ座標y  z:サブテクスチャ座標x  w:サブテクスチャ座標y )
#if ( ( BUMPMAP || PHONG ) && USE_SPE ) || SHADOWMAP
	float4 V_to_Eye_ShadowMap3PosZ : TEXCOORD1 ;	// 頂点座標から視線へのベクトル( ビュー空間 )とシャドウマップ３のライトZ座標( w )
#endif
#if ( BUMPMAP == 0 && PHONG ) || SM_3
	float4 Normal_Fog      : TEXCOORD2 ;	// x.y.z:法線( ビュー空間 )  w:SM3.0用のフォグパラメータ
#endif
#if LG0_USE && ( BUMPMAP || PHONG )
	float4 Light0_Dir_Gen  : TEXCOORD3 ;	// xyz:ライト０の逆ベクトル( ビュー空間 ) w:ライト０減衰情報
#endif
#if LG1_USE && ( BUMPMAP || PHONG )
	float4 Light1_Dir_Gen  : TEXCOORD4 ;	// xyz:ライト１の逆ベクトル( ビュー空間 ) w:ライト１減衰情報
#endif
#if LG2_USE && ( BUMPMAP || PHONG )
	float4 Light2_Dir_Gen  : TEXCOORD5 ;	// xyz:ライト２の逆ベクトル( ビュー空間 ) w:ライト２減衰情報
#endif

#if SHADOWMAP || SHADOWMAP_DRAW
	float4 ShadowMap1Pos_ShadowMap3PosX   : TEXCOORD6 ;	// シャドウマップ１のライト座標( x, y, z )とシャドウマップ３のライトX座標( w )
#endif
#if SHADOWMAP
	float4 ShadowMap2Pos_ShadowMap3PosY   : TEXCOORD7 ;	// シャドウマップ２のライト座標( x, y, z )とシャドウマップ３のライトY座標( w )
#endif

#if SM_3 == 0 && ( FOG_LINEAR || FOG_EXP || FOG_EXP2 )
	float  Fog             : FOG ;			// フォグ
#endif
} ;

// 頂点シェーダーのマテリアルパラメータ
struct VS_CONST_MATERIAL
{
	float4 Diffuse ;				// ディフューズカラー
	float4 Specular ;				// スペキュラカラー
	float4 Power ;					// スペキュラの強さ
} ;

// 頂点シェーダーのライトパラメータ
struct VS_CONST_LIGHT
{
	float4 Position ;				// 座標( ビュー空間 )
	float3 Direction ;				// 方向( ビュー空間 )
	float4 Diffuse ;				// ディフューズカラー
	float4 Specular ;				// スペキュラカラー
	float4 Ambient ;				// アンビエントカラー
	float4 Range_FallOff_AT0_AT1 ;	// x:有効距離  y:スポットライト用FllOff  z:距離による減衰処理用パラメータ０  w:距離による減衰処理用パラメータ１
	float4 AT2_SpotP0_SpotP1 ;		// x:距離による減衰処理用パラメータ２  y:スポットライト用パラメータ０( cos( Phi / 2.0f ) )  z:スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )
} ;

// 頂点シェーダーのライト処理分岐用パラメータ
struct VS_CONST_LIGHT_BOOL
{
	bool Use ;						// 使用しているかどうか
	bool Point_Or_Spot ;			// ポイントライト若しくはスポットライトか
	bool Spot ;						// スポットライトか
} ;



// C++ 側で設定する定数の定義
float4              cfZeroOne                                                  : register( c0  /* DX_VS_CONSTF_ZERO_ONE                        */ ) ;	// ０と１の定数( x:0.0f  y:1.0f )
float4              cfAmbient_Emissive                                         : register( c1  /* DX_VS_CONSTF_AMBIENT_EMISSIVE                */ ) ;	// エミッシブカラー＋アンビエントカラー( エミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー )
float4              cfProjectionMatrix[ 4 ]                                    : register( c2  /* DX_VS_CONSTF_PROJECTION_MAT                  */ ) ;	// ビュー　→　プロジェクション行列
float4              cfViewMatrix[ 3 ]                                          : register( c6  /* DX_VS_CONSTF_VIEW_MAT                        */ ) ;	// ワールド　→　ビュー行列
float4              cfFog                                                      : register( c10 /* DX_VS_CONSTF_FOG                             */ ) ;	// フォグ用パラメータ( x:end/(end - start)  y:-1/(end - start)  z:density  w:自然対数の低 )

VS_CONST_MATERIAL   cfMaterial                                                 : register( c11 /* DX_VS_CONSTF_MATERIAL_START                  */ ) ;	// マテリアルパラメータ
VS_CONST_LIGHT      cfLight[ DX_VS_CONSTF_LIGHT_NUM ]                          : register( c14 /* DX_VS_CONSTF_LIGHT_START                     */ ) ;	// ライトパラメータ
float4              cfToonOutLineSize                                          : register( c42 /* DX_VS_CONSTF_TOON_OUTLINE_SIZE               */ ) ;	// トゥーンの輪郭線の大きさ
float4              cfDifSpeSource                                             : register( c43 /* DX_VS_CONSTF_DIF_SPE_SOURCE                  */ ) ;	// x:ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )  y:スペキュラカラー( 0.0f:マテリアル  1.0f:頂点 )

float4              cfShadowMap1LightViewProjectionMatrix[ 4 ]                 : register( c44 /* DX_VS_CONSTF_SHADOWMAP1_LIGHT_VIEW_PROJECTION_MAT */ ) ;	// シャドウマップ１用のライトビュー行列とライト射影行列を乗算したもの
float4              cfShadowMap2LightViewProjectionMatrix[ 4 ]                 : register( c48 /* DX_VS_CONSTF_SHADOWMAP2_LIGHT_VIEW_PROJECTION_MAT */ ) ;	// シャドウマップ２用のライトビュー行列とライト射影行列を乗算したもの
float4              cfShadowMap3LightViewProjectionMatrix[ 4 ]                 : register( c52 /* DX_VS_CONSTF_SHADOWMAP3_LIGHT_VIEW_PROJECTION_MAT */ ) ;	// シャドウマップ３用のライトビュー行列とライト射影行列を乗算したもの

float4              cfVerticalFog                                              : register( c56 /* DX_VS_CONSTF_VERTICAL_FOG                    */ ) ;	// 高さフォグ用パラメータを格納するアドレス( x:end/(end - start)  y:-1/(end - start)  z:density  w:density start )
float4              cfVerticalFogMode                                          : register( c57 /* DX_VS_CONSTF_VERTICAL_FOG_MODE               */ ) ;	// 高さフォグ用パラメータを格納するアドレス( x:フォグモード )

float4              cfTextureMatrix[ DX_VS_CONSTF_TEXTURE_MATRIX_NUM ][ 2 ]    : register( c88 /* DX_VS_CONSTF_TEXTURE_MATRIX_START            */ ) ;	// テクスチャ座標操作用行列
float4              cfLocalWorldMatrix[ DX_VS_CONSTF_WORLD_MAT_NUM * 3 ]       : register( c94 /* DX_VS_CONSTF_WORLD_MAT_START                 */ ) ;	// ローカル　→　ワールド行列

bool                cbFogLinear                                                : register( b0  /* DX_VS_CONSTB_FOG_LINEAR                      */ ) ;	// 線形フォグを掛けるかどうか
bool                cbFogExp                                                   : register( b1  /* DX_VS_CONSTB_FOG_EXP                         */ ) ;	// 指数関数フォグを掛けるかどうか
bool                cbFogExp2                                                  : register( b2  /* DX_VS_CONSTB_FOG_EXP2                        */ ) ;	// 指数関数フォグ２を掛けるかどうか
bool                cbFogUse                                                   : register( b3  /* DX_VS_CONSTB_FOG_USE                         */ ) ;	// フォグを掛けるかどうか
VS_CONST_LIGHT_BOOL cbLightBool[ DX_VS_CONSTB_LIGHT_NUM ]                      : register( b4  /* DX_VS_CONSTB_LIGHT_START                     */ ) ;	// ライト処理分岐用パラメータ



#define ZERO	                cfZeroOne.x
#define ONE                     cfZeroOne.y

#define FOG_LINEAR_ADD          cfFog.x
#define FOG_LINEAR_DIV          cfFog.y
#define FOG_EXP_DENSITY         cfFog.z
#define FOG_EXP_E               cfFog.w

#define VERTICAL_FOG_LINEAR_ADD        cfVerticalFog.x
#define VERTICAL_FOG_LINEAR_DIV        cfVerticalFog.y
#define VERTICAL_FOG_EXP_DENSITY       cfVerticalFog.z
#define VERTICAL_FOG_EXP_DENSITY_START cfVerticalFog.w
#define VERTICAL_FOG_EXP_E             cfFog.w

#ifdef SKINMESH
#define LOCAL_WORLD_MAT         lLocalWorldMatrix
#else
#define LOCAL_WORLD_MAT         cfLocalWorldMatrix
#endif
/*
#ifdef DIF_MAT
#define DIF                     cfMaterial.Diffuse
#endif

#ifdef DIF_VTX
#define DIF                     VSInput.Diffuse
#endif

#ifdef SPE_MAT
#define SPE                     cfMaterial.Specular
#endif

#ifdef SPE_VTX
#define SPE                     VSInput.Specular
#endif
*/
#define TEXMTX0_U               cfTextureMatrix[ 0 ][ 0 ]
#define TEXMTX0_V               cfTextureMatrix[ 0 ][ 1 ]

#if SM_3
#define FOG_OUT                 VSOutput.Normal_Fog.w
#else
#define FOG_OUT                 VSOutput.Fog
#endif




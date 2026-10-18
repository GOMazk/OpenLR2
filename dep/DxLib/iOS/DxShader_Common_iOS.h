// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS シェーダー共通定義コード
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_SHADER_COMMON_IOS_H
#define DX_SHADER_COMMON_IOS_H

// マクロ定義 -----------------------------------

#define DX_IOS_COMMON_CONST_LIGHT_NUM			(6)		// 共通パラメータのライトの最大数

// 構造体定義 --------------------------------------------------------------------

// マテリアルパラメータ
struct DX_IOS_CONST_MATERIAL
{
	DX_IOS_SHADER_FLOAT4		Diffuse ;				// ディフューズカラー
	DX_IOS_SHADER_FLOAT4		Specular ;				// スペキュラカラー
	DX_IOS_SHADER_FLOAT4		Ambient_Emissive ;		// マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

	DX_IOS_SHADER_FLOAT		Power ;					// スペキュラの強さ
	DX_IOS_SHADER_FLOAT		TypeParam0 ;			// マテリアルタイプパラメータ0
	DX_IOS_SHADER_FLOAT		TypeParam1 ;			// マテリアルタイプパラメータ1
	DX_IOS_SHADER_FLOAT		TypeParam2 ;			// マテリアルタイプパラメータ2
} ;

// フォグパラメータ
struct DX_IOS_VS_CONST_FOG
{
	DX_IOS_SHADER_FLOAT		LinearAdd ;				// フォグ用パラメータ end / ( end - start )
	DX_IOS_SHADER_FLOAT		LinearDiv ;				// フォグ用パラメータ -1  / ( end - start )
	DX_IOS_SHADER_FLOAT		Density ;				// フォグ用パラメータ density
	DX_IOS_SHADER_FLOAT		E ;						// フォグ用パラメータ 自然対数の低

	DX_IOS_SHADER_FLOAT4		Color ;					// カラー
} ;

// ライトパラメータ
struct DX_IOS_CONST_LIGHT
{
	DX_IOS_SHADER_INT			Type ;					// ライトタイプ( DX_LIGHTTYPE_POINT など )
	DX_IOS_SHADER_INT3			Padding1 ;				// パディング１

	DX_IOS_SHADER_FLOAT3		Position ;				// 座標( ビュー空間 )
	DX_IOS_SHADER_FLOAT		RangePow2 ;				// 有効距離の２乗

	DX_IOS_SHADER_FLOAT3		Direction ;				// 方向( ビュー空間 )
	DX_IOS_SHADER_FLOAT		FallOff ;				// スポットライト用FallOff

	DX_IOS_SHADER_FLOAT3		Diffuse ;				// ディフューズカラー
	DX_IOS_SHADER_FLOAT		SpotParam0 ;			// スポットライト用パラメータ０( cos( Phi / 2.0f ) )

	DX_IOS_SHADER_FLOAT3		Specular ;				// スペキュラカラー
	DX_IOS_SHADER_FLOAT		SpotParam1 ;			// スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	DX_IOS_SHADER_FLOAT4		Ambient ;				// アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

	DX_IOS_SHADER_FLOAT		Attenuation0 ;			// 距離による減衰処理用パラメータ０
	DX_IOS_SHADER_FLOAT		Attenuation1 ;			// 距離による減衰処理用パラメータ１
	DX_IOS_SHADER_FLOAT		Attenuation2 ;			// 距離による減衰処理用パラメータ２
	DX_IOS_SHADER_FLOAT		Padding2 ;				// パディング２
} ;

// フラグメントシェーダー・頂点シェーダー共通パラメータ
struct DX_IOS_CONST_BUFFER_COMMON
{
	DX_IOS_CONST_LIGHT			Light[ DX_IOS_COMMON_CONST_LIGHT_NUM ] ;			// ライトパラメータ
	DX_IOS_CONST_MATERIAL		Material ;				// マテリアルパラメータ
	DX_IOS_VS_CONST_FOG		Fog ;					// フォグパラメータ
} ;

#endif // DX_SHADER_COMMON_IOS_H


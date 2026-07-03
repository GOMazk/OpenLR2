// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		描画処理プログラム( Linux )ヘッダファイル
// 
// 				Ver 3.24f
// 
// -------------------------------------------------------------------------------

#ifndef DX_GRAPHICSLINUX_H
#define DX_GRAPHICSLINUX_H


#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

// インクルード ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxGraphics.h"
#include "../DxArchive_.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "DxShader_DataType_Linux.h"
#include "DxShader_FS_Linux.h"
#include "DxShader_VS_Linux.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define GLINUX										GraphicsHardDataLinux

// Graphics_Linux_DrawPreparation 関数に渡すフラグ
#define DX_LINUX_DRAWPREP_TRANS						(0x00001)
#define DX_LINUX_DRAWPREP_VECTORINT					(0x00002)
#define DX_LINUX_DRAWPREP_GOURAUDSHADE				(0x00008)
#define DX_LINUX_DRAWPREP_PERSPECTIVE					(0x00010)
#define DX_LINUX_DRAWPREP_DIFFUSERGB					(0x00020)
#define DX_LINUX_DRAWPREP_DIFFUSEALPHA				(0x00040)
#define DX_LINUX_DRAWPREP_FOG							(0x00080)
#define DX_LINUX_DRAWPREP_NOBLENDSETTING				(0x00100)
#define DX_LINUX_DRAWPREP_LIGHTING					(0x00200)
#define DX_LINUX_DRAWPREP_SPECULAR					(0x00400)
#define DX_LINUX_DRAWPREP_3D							(0x00800)
#define DX_LINUX_DRAWPREP_TEXADDRESS					(0x01000)
#define DX_LINUX_DRAWPREP_NOTSHADERRESET				(0x02000)
#define DX_LINUX_DRAWPREP_CULLING						(0x04000)
#define DX_LINUX_DRAWPREP_TEXTURE						(0x08000)
#define DX_LINUX_DRAWPREP_TEXALPHACH					(0x10000)
#define DX_LINUX_DRAWPREP_TEXALPHATEST				(0x20000)

// フラグメントシェーダーのアルファテストモード
#define LINUX_FS_ALPHATEST_CMP_GREATER				(0)				// 描画アルファ値がテストアルファ値より大きければ描画
#define LINUX_FS_ALPHATEST_CMP_OTHER					(1)				// GREATER 以外の比較モード
#define LINUX_FS_ALPHATEST_NUM						(2)

// GL Extensions
#define LINUX_GL_EXTENSION_DEPTH24						(0)				// GL_OES_depth24
#define LINUX_GL_EXTENSION_DEPTH32						(1)				// GL_OES_depth32
#define LINUX_GL_EXTENSION_ELEMENT_INDEX_UNIT				(2)				// GL_OES_element_index_uint
#define LINUX_GL_EXTENSION_TEXTURE_FLOAT					(3)				// GL_OES_texture_float
#define LINUX_GL_EXTENSION_TEXTURE_FLOAT_LINEAR			(4)				// GL_OES_texture_float_linear
#define LINUX_GL_EXTENSION_COMPRESSED_PALETTED_TEXTURE	(5)				// GL_OES_compressed_paletted_texture
#define LINUX_GL_EXTENSION_COMPRESSED_ETC1_RGB8_TEXTURE	(6)				// GL_OES_compressed_ETC1_RGB8_texture
#define LINUX_GL_EXTENSION_DEPTH_TEXTURE					(7)				// GL_OES_depth_texture
#define LINUX_GL_EXTENSION_TEXTURE_HALF_FLOAT				(8)				// GL_OES_texture_half_float
#define LINUX_GL_EXTENSION_TEXTURE_HALF_FLOAT_LINEAR		(9)				// GL_OES_texture_half_float_linear
#define LINUX_GL_EXTENSION_VERTEX_HALF_FLOAT				(10)			// GL_OES_vertex_half_float
#define LINUX_GL_EXTENSION_TEXTURE_3D						(11)			// GL_OES_texture_3D
#define LINUX_GL_EXTENSION_TEXTURE_FORMAT_BGRA8888		(12)			// GL_EXT_texture_format_BGRA8888
#define LINUX_GL_EXTENSION_NUM							(13)

// ピクセルフォーマット
#define LINUX_PIXEL_FORMAT_R8G8B8A8					(0)
#define LINUX_PIXEL_FORMAT_R8G8B8						(1)
#define LINUX_PIXEL_FORMAT_A4B4G4R4					(2)
#define LINUX_PIXEL_FORMAT_A1B5G5R5					(3)
#define LINUX_PIXEL_FORMAT_B5G6R5						(4)
#define LINUX_PIXEL_FORMAT_R8G8B8_PAL4				(5)
#define LINUX_PIXEL_FORMAT_R8G8B8_PAL8				(6)
#define LINUX_PIXEL_FORMAT_R8G8B8A8_PAL4				(7)
#define LINUX_PIXEL_FORMAT_R8G8B8A8_PAL8				(8)
#define LINUX_PIXEL_FORMAT_A1B5G5R5_PAL4				(9)
#define LINUX_PIXEL_FORMAT_A1B5G5R5_PAL8				(10)
#define LINUX_PIXEL_FORMAT_R8							(11)
#define LINUX_PIXEL_FORMAT_R16G16B16_FLOAT			(12)
#define LINUX_PIXEL_FORMAT_R32G32B32_FLOAT			(13)
#define LINUX_PIXEL_FORMAT_R16G16B16A16_FLOAT			(14)
#define LINUX_PIXEL_FORMAT_R32G32B32A32_FLOAT			(15)
#define LINUX_PIXEL_FORMAT_NUM						(16)

// 標準描画頂点フォーマット
#define LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D				(0)				// ( VERTEX_NOTEX_2D                      構造体形式 )ライン、ボックス描画用頂点フォーマット
#define LINUX_VERTEX_INPUTLAYOUT_2D					(1)				// ( VERTEX2D or VERTEX_2D                構造体形式 )トランスフォーム済み頂点フォーマット
#define LINUX_VERTEX_INPUTLAYOUT_BLENDTEX_2D			(2)				// ( VERTEX_BLENDTEX_2D or VERTEX2DSHADER 構造体形式 )αブレンドテクスチャ付きトランスフォーム済み頂点フォーマット
#define LINUX_VERTEX_INPUTLAYOUT_NOTEX_3D				(3)				// ( VERTEX_NOTEX_3D                      構造体形式 )ライン、ボックス描画用頂点フォーマット、３Ｄ用( 旧バージョン用 )
#define LINUX_VERTEX_INPUTLAYOUT_3D					(4)				// ( VERTEX_3D                            構造体形式 )グラフィックス描画用頂点フォーマット、３Ｄ用( 旧バージョン用 )
#define LINUX_VERTEX_INPUTLAYOUT_3D_LIGHT				(5)				// ( VERTEX3D                             構造体形式 )グラフィックス描画用頂点フォーマット
#define LINUX_VERTEX_INPUTLAYOUT_SHADER_3D			(6)				// ( VERTEX3DSHADER                       構造体形式 )シェーダー描画用頂点フォーマット
#define LINUX_VERTEX_INPUTLAYOUT_NUM					(7)

// 描画タイプ
#define LINUX_RENDER_TYPE_NORMAL						(0)				// 0:通常描画
#define LINUX_RENDER_TYPE_MUL							(1)				// 1:乗算描画
#define LINUX_RENDER_TYPE_INVERSE						(2)				// 2:RGB反転
#define LINUX_RENDER_TYPE_X4							(3)				// 3:描画輝度4倍
#define LINUX_RENDER_TYPE_PMA_NORMAL					(4)				// 4:乗算済みαブレンドモードの通常描画
#define LINUX_RENDER_TYPE_PMA_INVERSE					(5)				// 5:乗算済みαブレンドモードのRGB反転
#define LINUX_RENDER_TYPE_PMA_X4						(6)				// 6:乗算済みαブレンドモードの描画輝度4倍
#define LINUX_RENDER_TYPE_NUM							(7)

// 頂点データ
#define LINUX_VERTEX_ATTR_POS_F2								(0)			// 座標( x, y )
#define LINUX_VERTEX_ATTR_POS_F3								(1)			// 座標( x, y, z )
#define LINUX_VERTEX_ATTR_POS_F4								(2)			// 座標( x, y, z, w )
#define LINUX_VERTEX_ATTR_NRM									(3)			// 法線
#define LINUX_VERTEX_ATTR_DIF									(4)			// ディフューズカラー
#define LINUX_VERTEX_ATTR_SPC									(5)			// スペキュラカラー
#define LINUX_VERTEX_ATTR_TEXUV0_F2							(6)			// テクスチャ座標０( x, y )
#define LINUX_VERTEX_ATTR_TEXUV1_F2							(7)			// テクスチャ座標１( x, y )
#define LINUX_VERTEX_ATTR_TEXUV2_F2							(8)			// テクスチャ座標２( x, y )
#define LINUX_VERTEX_ATTR_TEXUV3_F2							(9)			// テクスチャ座標３( x, y )
#define LINUX_VERTEX_ATTR_TEXUV4_F2							(10)		// テクスチャ座標４( x, y )
#define LINUX_VERTEX_ATTR_TEXUV5_F2							(11)		// テクスチャ座標５( x, y )
#define LINUX_VERTEX_ATTR_TEXUV6_F2							(12)		// テクスチャ座標６( x, y )
#define LINUX_VERTEX_ATTR_TEXUV7_F2							(13)		// テクスチャ座標７( x, y )
#define LINUX_VERTEX_ATTR_TEXUV0_F4							(14)		// テクスチャ座標０( x, y, z, w )
#define LINUX_VERTEX_ATTR_TEXUV1_F4							(15)		// テクスチャ座標１( x, y, z, w )
#define LINUX_VERTEX_ATTR_TAN									(16)		// 接線( ローカル空間 )
#define LINUX_VERTEX_ATTR_BIN									(17)		// 従法線( ローカル空間 )
#define LINUX_VERTEX_ATTR_BLENDIND0							(18)		// ボーン処理用 Float型定数配列インデックス０
#define LINUX_VERTEX_ATTR_BLENDWEIGHT0						(19)		// ボーン処理用ウエイト値０
#define LINUX_VERTEX_ATTR_BLENDIND1							(20)		// ボーン処理用 Float型定数配列インデックス１
#define LINUX_VERTEX_ATTR_BLENDWEIGHT1						(21)		// ボーン処理用ウエイト値１
#define LINUX_VERTEX_ATTR_NUM									(22)
#define LINUX_VERTEX_ATTR_OTHER								(-1)

// シェーダーの定数情報
#define LINUX_SHADER_UNIFORM_SRC_TEX							(0)			// ソーステクスチャ
#define LINUX_SHADER_UNIFORM_BLEND_TEX						(1)			// ブレンドテクスチャ
#define LINUX_SHADER_UNIFORM_DIF_MAP_TEX						(2)			// ディフューズマップテクスチャ
#define LINUX_SHADER_UNIFORM_NRM_MAP_TEX						(3)			// 法線マップテクスチャ
#define LINUX_SHADER_UNIFORM_SPC_MAP_TEX						(4)			// スペキュラマップテクスチャ
#define LINUX_SHADER_UNIFORM_TOON_DIF_GRAD_TEX				(5)			// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ
#define LINUX_SHADER_UNIFORM_TOON_SPC_GRAD_TEX				(6)			// トゥーンレンダリング用スペキュラカラーグラデーションテクスチャ
#define LINUX_SHADER_UNIFORM_TOON_SPHERE_MAP_TEX				(7)			// トゥーンレンダリング用スフィアマップテクスチャ
#define LINUX_SHADER_UNIFORM_TOON_RGB_TO_VMAXRGBVOL_TEX		(8)			// トゥーンレンダリング用RGB輝度飽和処理用ボリュームテクスチャ
#define LINUX_SHADER_UNIFORM_SUB_TEX							(9)			// サブテクスチャ
#define LINUX_SHADER_UNIFORM_SHADOW_MAP0_TEX					(10)		// シャドウマップ0テクスチャ
#define LINUX_SHADER_UNIFORM_SHADOW_MAP1_TEX					(11)		// シャドウマップ1テクスチャ
#define LINUX_SHADER_UNIFORM_SHADOW_MAP2_TEX					(12)		// シャドウマップ2テクスチャ
#define LINUX_SHADER_UNIFORM_AMB_EMI							(13)		// エミッシブカラー＋アンビエントカラー( エミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー )
#define LINUX_SHADER_UNIFORM_MAT_DIF							(14)		// ディフューズカラー
#define LINUX_SHADER_UNIFORM_MAT_SPC							(15)		// スペキュラカラー
#define LINUX_SHADER_UNIFORM_MAT_POW							(16)		// スペキュラの強さ
#define LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM0					(17)		// マテリアルパラメータ0
#define LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM1					(18)		// マテリアルパラメータ1
#define LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM2					(19)		// マテリアルパラメータ2
#define LINUX_SHADER_UNIFORM_LIGHT_POS						(20)		// 座標( ビュー空間 )
#define LINUX_SHADER_UNIFORM_LIGHT_DIR						(21)		// 方向( ビュー空間 )
#define LINUX_SHADER_UNIFORM_LIGHT_DIF						(22)		// ディフューズカラー
#define LINUX_SHADER_UNIFORM_LIGHT_SPC						(23)		// スペキュラカラー
#define LINUX_SHADER_UNIFORM_LIGHT_AMB						(24)		// アンビエントカラー
#define LINUX_SHADER_UNIFORM_LIGHT_RANGE_FALLOFF_AT0_AT1		(25)		// x:有効距離  y:スポットライト用FllOff  z:距離による減衰処理用パラメータ０  w:距離による減衰処理用パラメータ１
#define LINUX_SHADER_UNIFORM_LIGHT_AT2_SPOTP0_SPOTP1			(26)		// x:距離による減衰処理用パラメータ２  y:スポットライト用パラメータ０( cos( Phi / 2.0 ) )  z:スポットライト用パラメータ１( 1.0 / ( cos( Theta / 2.0 ) - cos( Phi / 2.0 ) ) )
#define LINUX_SHADER_UNIFORM_ANTI_VIEW_PORT_MAT				(27)		// ワールド　→　ビュー行列
#define LINUX_SHADER_UNIFORM_VIEW_MAT							(28)		// ワールド　→　ビュー行列
#define LINUX_SHADER_UNIFORM_PROJ_MAT							(29)		// ビュー　→　プロジェクション行列
#define LINUX_SHADER_UNIFORM_FOG								(30)		// フォグ用パラメータ( x:end/(end - start)  y:-1/(end - start)  z:density  w:自然対数の低 )
#define LINUX_SHADER_UNIFORM_TOON_OUTLINE_SIZE				(31)		// トゥーンの輪郭線の大きさ
#define LINUX_SHADER_UNIFORM_DIFSRC_SPCSRC_MULSPECOL			(32)		// x:ディフューズカラー( 0.0:マテリアル  1.0:頂点 )  y:スペキュラカラー(   0.0:マテリアル  1.0:頂点 )  z:スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
#define LINUX_SHADER_UNIFORM_SM_LVP_MAT						(33)		// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
#define LINUX_SHADER_UNIFORM_TEX_MAT							(34)		// テクスチャ座標操作用行列
#define LINUX_SHADER_UNIFORM_LW_MAT							(35)		// ローカル　→　ワールド行列
#define LINUX_SHADER_UNIFORM_MUL_ALPHA_COLOR					(36)		// カラーにアルファ値を乗算するかどうか( 0.0:乗算しない  1.0:乗算する )
#define LINUX_SHADER_UNIFORM_ALPHA_TEST_REF					(37)		// αテスト値
#define LINUX_SHADER_UNIFORM_ALPHA_TEST_CMPMODE				(38)
#define LINUX_SHADER_UNIFORM_FOG_COLOR						(39)		// フォグカラー
#define LINUX_SHADER_UNIFORM_FACTOR_COLOR						(40)		// ファクターカラー
#define LINUX_SHADER_UNIFORM_IGNORE_TEXTURE_COLOR				(41)		// テクスチャカラー無視処理用カラー
#define LINUX_SHADER_UNIFORM_ADD_COLOR						(42)		// 加算カラー
#define LINUX_SHADER_UNIFORM_SHADOWMAP_LIGHT_ENABLE			(43)		// シャドウマップ０・１・２のライト有効情報( x:シャドウマップ０でライトnが有効か  y:シャドウマップ１でライトnが有効か  z:シャドウマップ２でライトnが有効か )
#define LINUX_SHADER_UNIFORM_SHADOWMAP_DADJ_GRAD				(44)		// シャドウマップの情報( x:閾値深度補正値  y:グラデーション範囲  )
#define LINUX_SHADER_UNIFORM_NUM								(45)
#define LINUX_SHADER_UNIFORM_OTHER							(-1)

// 頂点バッファのサイズ
#define DX_GLES2_VERTBUFFERSIZE							(64 * 1024)		// 64KB

// シェーダー Uniform セットのヘルパーマクロ
#define UNIFORM_SET_MATRIX4( index, _matrix4 )	glUniformMatrix4fv( (index), 1, GL_FALSE, _matrix4 ) ;
#define UNIFORM_SET_FLOAT4(  index, _float4  )	glUniform4f(        (index), _float4[ 0 ], _float4[ 1 ], _float4[ 2 ], _float4[ 3 ] ) ;
#define UNIFORM_SET_FLOAT3(  index, _float3  )	glUniform3f(        (index), _float3[ 0 ], _float3[ 1 ], _float3[ 2 ] ) ;
#define UNIFORM_SET_FLOAT2(  index, _float2  )	glUniform2f(        (index), _float2[ 0 ], _float2[ 1 ] ) ;
#define UNIFORM_SET_FLOAT1(  index, _float1  )	glUniform1f(        (index), _float1 ) ;
#define UNIFORM_SET_INT1(    index, _int1    )	glUniform1i(        (index), _int1   ) ;

// GRAPHICS_LINUX_SHADER_LIST 一つに含まれる GRAPHICS_LINUX_SHADER の数
#define SHADERS_LIST_DATA_NUM							(256)

// 構造体定義 --------------------------------------------------------------------

// Linux用 テクスチャ情報
struct GRAPHICS_LINUX_TEXTURE
{
	GLuint							TextureBuffer ;							// テクスチャバッファ
	int								MipMapCount ;							// ミップマップ数( Level0 しかない場合は 1, Level1 まである場合は 2 )
	int								PixelFormat ;							// テクスチャのピクセルフォーマット( LINUX_PIXEL_FORMAT_R8G8B8A8 など )
	GLuint							Width ;									// テクスチャの幅
	GLuint							Height ;								// テクスチャの高さ
	GLint							MagFilter ;								// 拡大フィルター( GL_TEXTURE_MAG_FILTER のパラメータ、 GL_NEAREST など )
	GLint							MinFilter ;								// 縮小フィルター( GL_TEXTURE_MIN_FILTER のパラメータ、 GL_NEAREST など )
	GLint							WrapS ;									// テクスチャアドレスモードＳ( GL_TEXTURE_WRAP_S のパラメータ、 GL_REPEAT など )
	GLint							WrapT ;									// テクスチャアドレスモードＴ( GL_TEXTURE_WRAP_T のパラメータ、 GL_REPEAT など )
	GLint							WrapR ;									// テクスチャアドレスモードＲ( GL_TEXTURE_WRAP_R のパラメータ、 GL_REPEAT など )
} ;

// Linux用 ハードウエアレンダリング用オリジナル画像テクスチャ情報
struct IMAGEDATA_ORIG_HARD_TEX_PF
{
	GLuint							MSRenderTarget ;						// マルチサンプル用レンダーターゲット

	GLuint							FrameBuffer ;							// フレームバッファ
	GRAPHICS_LINUX_TEXTURE		Texture ;								// テクスチャ
	GLuint							DepthBuffer ;							// 深度バッファ
} ;

// Linux用 シャドウマップ情報
struct SHADOWMAPDATA_PF
{
	GLuint							FrameBuffer ;							// 深度値書き込み用フレームバッファ
	GRAPHICS_LINUX_TEXTURE		Texture ;								// 深度値書き込み用テクスチャ
	GLuint							DepthBuffer ;							// 深度バッファ
} ;

// ピクセルフォーマットの情報
struct PIXELFORMAT_INFO_LINUX
{
	int								IsExtFormat ;							// 拡張フォーマットかどうか
	int								PixelBits ;								// １ピクセルあたりのビット数
	int								AddDataBytes ;							// 追加情報のバイト数( パレット情報など )
	GLint							InternalFormat ;						// GL_RGBA , GL_PALETTE4_RGB8_OES など
	GLenum							Format ;								// GL_RGBA など
	GLenum							Type ;									// GL_UNSIGNED_SHORT_4_4_4_4 など
} ;

// 頂点データ要素一つ当たりの基本情報
struct VERTEXBUFFER_INPUT_ELEMENT_INFO_BASE_LINUX
{
	const GLchar *								AttrName ;					// データ名
	GLint										Size ;						// データ数
	GLenum										DataFormat ;				// データ形式
	GLboolean									Normalized ;				// 正規化するかどうか
} ;

// 頂点データ要素一つ当たりの情報
struct VERTEXBUFFER_INPUT_ELEMENT_INFO_LINUX
{
	int											ElementBase ;				// 頂点基本情報( LINUX_VERTEX_ATTR_POS など )
	VERTEXBUFFER_INPUT_ELEMENT_INFO_BASE_LINUX *	OtherAttr ;					// プリセットにない情報の場合の参照先( Attr が LINUX_VERTEX_ATTR_OTHER の場合に使用する )
	GLuint										Offset ;					// オフセット
} ;

// 頂点シェーダーの入力情報
struct VERTEXBUFFER_INPUT_INFO_LINUX
{
	GLuint										VertexDataSize ;			// 頂点データ一つあたりのバイト数
	int											ElementInfoNum ;			// 入力頂点データの要素数
	VERTEXBUFFER_INPUT_ELEMENT_INFO_LINUX		ElementInfo[ 32 ] ;			// 入力頂点データ情報
} ;

// Linux用 頂点バッファハンドル情報
struct VERTEXBUFFERHANDLEDATA_PF
{
	GLuint							VertexBuffer ;
} ;

// Linux用 インデックスバッファハンドル情報
struct INDEXBUFFERHANDLEDATA_PF
{
	GLuint							IndexBuffer ;
} ;

// Linux のシェーダー一つ辺りの情報を纏めた構造体
struct GRAPHICS_LINUX_SHADER
{
	GLuint							Shader ;
	GLushort						AttributeIndex[ LINUX_VERTEX_ATTR_NUM ] ;		// 各頂点要素が何番目の要素となっているかの情報( 0xffffffff の場合はその要素がないということ )
	GLushort						UniformIndex[ LINUX_SHADER_UNIFORM_NUM ] ;	// 各シェーダー用定数要素が何番目の要素となっているかの情報( 0xffffffff の場合はその要素がないということ )
	int								ApplyUniformUpdateCount ;						// 適用されている定数の変更バージョン
} ;

// Linux用 シェーダーハンドル情報
struct SHADERHANDLEDATA_PF
{
	GLuint							Shader ;
} ;

// Linux用 シェーダー用定数バッファハンドル環境依存情報の構造体
struct SHADERCONSTANTBUFFERHANDLEDATA_PF
{
	int								Dummy ;
} ;

// Linux の標準描画用のシェーダーを纏めた構造体
struct GRAPHICS_HARDWARE_LINUX_SHADER_BASE
{
	// 標準描画用の複雑な処理を行わない頂点シェーダー
	GLuint						BaseSimple_VS[ LINUX_VERTEX_INPUTLAYOUT_NUM ] ;

	// 固定機能パイプライン互換の頂点シェーダー( テクスチャなし )
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint						BaseNoneTex_FS[ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// 固定機能パイプライン互換のシェーダー( テクスチャなし )
	// [ 標準描画頂点フォーマット ]
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*BaseNoneTex_Shader[ LINUX_VERTEX_INPUTLAYOUT_NUM ][ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// 固定機能パイプライン互換のフラグメントシェーダー( テクスチャあり )
	// [ ブレンド画像とのブレンドタイプ( 0:なし  1:DX_BLENDGRAPHTYPE_NORMAL  2:DX_BLENDGRAPHTYPE_WIPE  3:DX_BLENDGRAPHTYPE_ALPHA ) ]
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ テクスチャRGB無視( 0:無視しない  1:無視する ) ]
	// [ テクスチャAlpha無視( 0:無視しない  1:無視する ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint						BaseUseTex_FS[ 4 ][ LINUX_RENDER_TYPE_NUM ][ 2 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ; 

	// 固定機能パイプライン互換のシェーダー( テクスチャあり )
	// [ 標準描画頂点フォーマット ]
	// [ ブレンド画像とのブレンドタイプ( 0:なし  1:DX_BLENDGRAPHTYPE_NORMAL  2:DX_BLENDGRAPHTYPE_WIPE  3:DX_BLENDGRAPHTYPE_ALPHA ) ]
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ テクスチャRGB無視( 0:無視しない  1:無視する ) ]
	// [ テクスチャAlpha無視( 0:無視しない  1:無視する ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*BaseUseTex_Shader[ LINUX_VERTEX_INPUTLAYOUT_NUM ][ 4 ][ LINUX_RENDER_TYPE_NUM ][ 2 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// マスク処理用シェーダー
	GRAPHICS_LINUX_SHADER		MaskEffect_Shader ;
	GRAPHICS_LINUX_SHADER		MaskEffect_ReverseEffect_Shader ;
	GRAPHICS_LINUX_SHADER		MaskEffect_UseGraphHandle_Shader[ 4 ] ;
	GRAPHICS_LINUX_SHADER		MaskEffect_UseGraphHandle_ReverseEffect_Shader[ 4 ] ;

	// 単純転送用シェーダー
	GLuint		StretchRect_VS ;
	GLuint		StretchRectTex2_VS ;
	GLuint		StretchRectTex8_VS ;
	GRAPHICS_LINUX_SHADER		StretchRect_Shader ;

	// レンダーターゲットクリア用シェーダー
	GRAPHICS_LINUX_SHADER		ClearRT_Shader ;
} ;

// Linux の標準３Ｄ描画用のシェーダーを纏めた構造体
struct GRAPHICS_HARDWARE_LINUX_SHADER_BASE3D
{
	// 標準３Ｄ描画用ピクセル単位ライティングタイプの頂点シェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	GLuint		Base3D_PixelLighting_VS[ 2 ][ 4 ] ;

	// 標準３Ｄ描画用ピクセル単位ライティングタイプのフラグメントシェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ ライトインデックス ]
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		Base3D_PixelLighting_Normal_FS[ 2 ][ 84 ][ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// 標準３Ｄ描画用ピクセル単位ライティングタイプのシェーダー( リストは配列の左から )
	// シャドウマップの有無( 0:無し 1:有り )
	// 頂点シェーダー[ フォグタイプ ]
	// フラグメントシェーダー[ ライトインデックス ]
	// フラグメントシェーダー[ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*Base3D_PixelLighting_Normal_Shader[ 2 ][ 4 ][ 84 ][ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;



	// 標準３Ｄ描画用のシャドウマップへのレンダリング用頂点シェーダー
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	GLuint		Base3D_ShadowMap_VS[ 2 ] ;

	// 標準３Ｄ描画用のシャドウマップへのレンダリング用フラグメントシェーダー
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		Base3D_ShadowMap_Normal_FS[ LINUX_FS_ALPHATEST_NUM ] ;

	// 標準３Ｄ描画用のシャドウマップへのレンダリング用シェーダー
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*Base3D_ShadowMap_Normal_Shader[ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;



	// 標準３Ｄ描画用のライティングなし頂点シェーダー
	// [ フォグタイプ ]
	GLuint		Base3D_NoLighting_VS[ 4 ] ;

	// 標準３Ｄ描画用のライティングなしフラグメントシェーダー
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		Base3D_NoLighting_Normal_FS[ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// 標準３Ｄ描画用のライティングなしシェーダー
	// 頂点シェーダー[ フォグタイプ ]
	// フラグメントシェーダー[ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*Base3D_NoLighting_Normal_Shader[ 4 ][ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;



	// 標準３Ｄ描画用の頂点単位ライティング頂点シェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	// [ ライトインデックス ]
	GLuint		Base3D_VertexLighting_VS[ 2 ][ 4 ][ 20 ] ;

	// 標準３Ｄ描画用の頂点単位ライティングフラグメントシェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ ライトインデックス ]
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		Base3D_VertexLighting_Normal_FS[ 2 ][ 10 ][ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// 標準３Ｄ描画用の頂点単位ライティングシェーダー
	// シャドウマップの有無( 0:無し 1:有り )
	// 頂点シェーダー[ フォグタイプ ]
	// 頂点シェーダー[ ライトインデックス ]
	// フラグメントシェーダー[ ライトインデックス ]
	// フラグメントシェーダー[ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*Base3D_VertexLighting_Normal_Shader[ 2 ][ 4 ][ 20 ][ 10 ][ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;
} ;


#ifndef DX_NON_MODEL

// Linux のモデル描画用のシェーダーを纏めた構造体
struct GRAPHICS_HARDWARE_LINUX_SHADER_MODEL
{
	// モデル描画用ピクセル単位ライティングタイプの頂点シェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ 2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	GLuint		MV1_PixelLighting_VS[ 2 ][ 3 ][ 2 ][ 4 ] ;


	// モデル描画用ピクセル単位ライティングタイプのトゥーンタイプ１用フラグメントシェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ トゥーンレンダリングのスペキュラグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ バンプマップ ( 0:なし 1:あり ) ]
	// [ 有効なライトの数 - 1 ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_PixelLighting_ToonType1_FS[ 2 ][ 2 ][ 2 ][ 2 ][ DX_LINUX_COMMON_CONST_LIGHT_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用ピクセル単位ライティングタイプのトゥーンタイプ１用シェーダー( リストは配列の左から )
	// シャドウマップの有無( 0:無し 1:有り )
	// バンプマップの有無( 0:無し 1:有り )
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ 2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ フォグタイプ ]
	// フラグメントシェーダー[ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// フラグメントシェーダー[ トゥーンレンダリングのスペキュラグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// フラグメントシェーダー[ 有効なライトの数 - 1 ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_PixelLighting_ToonType1_Shader[ 2 ][ 2 ][ 3 ][ 4 ][ 2 ][ 2 ][ DX_LINUX_COMMON_CONST_LIGHT_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;



	// モデル描画用ピクセル単位ライティングタイプのトゥーンタイプ２用フラグメントシェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ トゥーンレンダリングのスフィアマップの有無とブレンドタイプ( 0:スフィアマップは無い   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ 有効なライトの数 - 1 ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_PixelLighting_ToonType2_FS[ 2 ][ 3 ][ DX_LINUX_COMMON_CONST_LIGHT_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用ピクセル単位ライティングタイプのトゥーンタイプ２用シェーダー( リストは配列の左から )
	// シャドウマップの有無( 0:無し 1:有り )
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ 2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// 頂点シェーダー[ フォグタイプ ]
	// フラグメントシェーダー[ トゥーンレンダリングのスフィアマップの有無とブレンドタイプ( 0:スフィアマップは無い   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// フラグメントシェーダー[ 有効なライトの数 - 1 ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_PixelLighting_ToonType2_Shader[ 2 ][ 3 ][ 2 ][ 4 ][ 3 ][ DX_LINUX_COMMON_CONST_LIGHT_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;



	// モデル描画用ピクセル単位ライティングタイプのフラグメントシェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]
	// [ バンプマップ ( 0:なし 1:あり ) ]
	// [ 有効なライトの数 - 1 ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_PixelLighting_Normal_FS[ 2 ][ 5 ][ 2 ][ DX_LINUX_COMMON_CONST_LIGHT_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用ピクセル単位ライティングタイプのシェーダー( リストは配列の左から )
	// シャドウマップの有無( 0:無し 1:有り )
	// バンプマップの有無( 0:無し 1:有り )
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ 2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ フォグタイプ ]
	// フラグメントシェーダー[ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]
	// フラグメントシェーダー[ 有効なライトの数 - 1 ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_PixelLighting_Normal_Shader[ 2 ][ 2 ][ 3 ][ 4 ][ 5 ][ DX_LINUX_COMMON_CONST_LIGHT_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;






	// モデル描画用のマテリアル要素描画用フラグメントシェーダー
	// マテリアルタイプ( DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_UNORM など )
	// フラグメントシェーダーのアルファテストモード
	GLuint		MV1_MaterialType_FS[ DX_MATERIAL_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用のマテリアル要素描画用シェーダー
	// MV1_NoLighting_VS + MV1_MaterialType_FS
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// フラグメントシェーダー[ マテリアルタイプ( DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_UNORM など ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_MaterialType_Shader[ 3 ][ 2 ][ DX_MATERIAL_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;




	// モデル描画用のシャドウマップへのトゥーンレンダリング輪郭線描画用頂点シェーダー
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	GLuint		MV1_ToonOutLine_ShadowMap_VS[ 3 ][ 2 ] ;

	// モデル描画用のシャドウマップへのレンダリング用シェーダー
	// MV1_ToonOutLine_ShadowMap_VS + MV1_ShadowMap_Normal_FS
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_ToonOutLine_ShadowMap_Shader[ 3 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;



	// モデル描画用のトゥーンレンダリングの輪郭線描画用頂点シェーダー
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	GLuint		MV1_ToonOutLine_VS[ 3 ][ 2 ][ 4 ] ;

	// モデル描画用のライティングなしシェーダー
	// MV1_ToonOutLine_VS + MV1_NoLighting_Normal_FS[ 0 ]
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// 頂点シェーダー[ フォグタイプ ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_ToonOutLine_Shader[ 3 ][ 2 ][ 4 ][ LINUX_FS_ALPHATEST_NUM ] ;




	// モデル描画用のシャドウマップへのレンダリング用頂点シェーダー
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	GLuint		MV1_ShadowMap_VS[ 3 ][ 2 ] ;


	// モデル描画用のシャドウマップへのレンダリング用フラグメントシェーダー
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_ShadowMap_Normal_FS[ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用のシャドウマップへのレンダリング用シェーダー
	// MV1_ShadowMap_VS + MV1_ShadowMap_Normal_FS
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_ShadowMap_Normal_Shader[ 3 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;



	// モデル描画用のシャドウマップへのトゥーンレンダリング用フラグメントシェーダー
	// [ トゥーンレンダリングタイプ( 0:トゥーンレンダリング type 1   1:トゥーンレンダリング type 2 ) ]
	// [ トゥーンレンダリングのスフィアマップの有無とブレンドタイプ( 0:スフィアマップは無い   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_ShadowMap_Toon_FS[ 2 ][ 3 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用のシャドウマップへのトゥーンレンダリング用シェーダー
	// MV1_ShadowMap_VS + MV1_ShadowMap_Toon_FS
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// フラグメントシェーダー[ トゥーンレンダリングタイプ( 0:トゥーンレンダリング type 1   1:トゥーンレンダリング type 2 ) ]
	// フラグメントシェーダー[ トゥーンレンダリングのスフィアマップの有無とブレンドタイプ( 0:スフィアマップは無い   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// フラグメントシェーダー[ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_ShadowMap_Toon_Shader[ 3 ][ 2 ][ 2 ][ 3 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;



	// モデル描画用のライティングなし頂点シェーダー
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	GLuint		MV1_NoLighting_VS[ 3 ][ 2 ][ 4 ] ;


	// モデル描画用のライティングなしトゥーン用フラグメントシェーダー( リストは配列の左から )
	// [ トゥーンレンダリングタイプ( 0:トゥーンレンダリング type 1   1:トゥーンレンダリング type 2 ) ]
	// [ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT or トゥーンレンダリングではない  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_NoLighting_Toon_FS[ 2 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用のライティングなしトゥーン用シェーダー( リストは配列の左から )
	// MV1_NoLighting_VS + MV1_NoLighting_Toon_FS
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// 頂点シェーダー[ フォグタイプ ]
	// フラグメントシェーダー[ トゥーンレンダリングタイプ( 0:トゥーンレンダリング type 1   1:トゥーンレンダリング type 2 ) ]
	// フラグメントシェーダー[ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT or トゥーンレンダリングではない  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_NoLighting_Toon_Shader[ 3 ][ 2 ][ 4 ][ 2 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;



	// モデル描画用のライティングなしフラグメントシェーダー
	// [ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_NoLighting_Normal_FS[ 5 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用のライティングなしシェーダー
	// MV1_NoLighting_VS + MV1_NoLighting_Normal_FS
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// 頂点シェーダー[ フォグタイプ ]
	// フラグメントシェーダー[ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_NoLighting_Normal_Shader[ 3 ][ 2 ][ 4 ][ 5 ][ LINUX_FS_ALPHATEST_NUM ] ;



	// モデル描画用の頂点単位ライティングあり頂点シェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	// [ ライトインデックス ]
	GLuint		MV1_VertexLighting_VS[ 2 ][ 3 ][ 2 ][ 4 ][ 20 ] ;


	// モデル描画用の頂点単位ライティングありトゥーンタイプ１用フラグメントシェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ トゥーンレンダリングのスペキュラグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ バンプマップ ( 0:なし 1:あり ) ]
	// [ ライトインデックス ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_VertexLighting_ToonType1_FS[ 2 ][ 2 ][ 2 ][ 2 ][ 10 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用の頂点単位ライティングありトゥーンタイプ１用シェーダー
	// シャドウマップの有無( 0:無し 1:有り )
	// バンプマップの有無( 0:無し 1:有り )
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ フォグタイプ ]
	// 頂点シェーダー[ ライトインデックス ]
	// フラグメントシェーダー[ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// フラグメントシェーダー[ トゥーンレンダリングのスペキュラグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// フラグメントシェーダー[ ライトインデックス ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_VertexLighting_ToonType1_Shader[ 2 ][ 2 ][ 3 ][ 4 ][ 20 ][ 2 ][ 2 ][ 10 ][ LINUX_FS_ALPHATEST_NUM ] ;



	// モデル描画用の頂点単位ライティングありトゥーンタイプ２用フラグメントシェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ トゥーンレンダリングのスフィアマップの有無とブレンドタイプ( 0:スフィアマップは無い   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ ライトインデックス ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_VertexLighting_ToonType2_FS[ 2 ][ 3 ][ 10 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用の頂点単位ライティングありトゥーンタイプ２用シェーダー
	// シャドウマップの有無( 0:無し 1:有り )
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ バンプマップの有無( 0:無し 1:有り ) ]
	// 頂点シェーダー[ フォグタイプ ]
	// 頂点シェーダー[ ライトインデックス ]
	// フラグメントシェーダー[ トゥーンレンダリングのスフィアマップの有無とブレンドタイプ( 0:スフィアマップは無い   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// フラグメントシェーダー[ ライトインデックス ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_VertexLighting_ToonType2_Shader[ 2 ][ 3 ][ 2 ][ 4 ][ 20 ][ 3 ][ 10 ][ LINUX_FS_ALPHATEST_NUM ] ;



	// モデル描画用の頂点単位ライティングありフラグメントシェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]
	// [ バンプマップ ( 0:なし 1:あり ) ]
	// [ ライトインデックス ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GLuint		MV1_VertexLighting_Normal_FS[ 2 ][ 5 ][ 2 ][ 10 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用の頂点単位ライティングありシェーダー
	// シャドウマップの有無( 0:無し 1:有り )
	// バンプマップの有無( 0:無し 1:有り )
	// 頂点シェーダー[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// 頂点シェーダー[ フォグタイプ ]
	// 頂点シェーダー[ ライトインデックス ]
	// フラグメントシェーダー[ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]
	// フラグメントシェーダー[ ライトインデックス ]
	// フラグメントシェーダー[ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_LINUX_SHADER		*MV1_VertexLighting_Normal_Shader[ 2 ][ 2 ][ 3 ][ 4 ][ 20 ][ 5 ][ 10 ][ LINUX_FS_ALPHATEST_NUM ] ;
} ;
#endif // DX_NON_MODEL

// Linux の定数情報を纏めた構造体
struct GRAPHICS_HARDWARE_LINUX_CONSTANT
{
	// 頂点シェーダー、フラグメントシェーダー共通
	DX_LINUX_SHADER_FLOAT4			uAmb_Emi ;									// エミッシブカラー＋アンビエントカラー( エミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー )

	DX_LINUX_SHADER_FLOAT4			uMatDif ;									// ディフューズカラー
	DX_LINUX_SHADER_FLOAT4			uMatSpc ;									// スペキュラカラー
	DX_LINUX_SHADER_FLOAT			uMatPow ;									// スペキュラの強さ
	DX_LINUX_SHADER_FLOAT			uMatTypeParam0 ;							// タイプパラメータ0
	DX_LINUX_SHADER_FLOAT			uMatTypeParam1 ;							// タイプパラメータ1
	DX_LINUX_SHADER_FLOAT			uMatTypeParam2 ;							// タイプパラメータ2

	DX_LINUX_SHADER_FLOAT3			uLightPos[ 6 /* 頂点シェーダーで使用するのは 3 */ ] ;					// 座標( ビュー空間 )
	DX_LINUX_SHADER_FLOAT3			uLightDir[ 6 /* 頂点シェーダーで使用するのは 3 */ ] ;					// 方向( ビュー空間 )
	DX_LINUX_SHADER_FLOAT4			uLightDif[ 6 /* 頂点シェーダーで使用するのは 3 */ ] ;					// ディフューズカラー
	DX_LINUX_SHADER_FLOAT4			uLightSpc[ 6 /* 頂点シェーダーで使用するのは 3 */ ] ;					// スペキュラカラー
	DX_LINUX_SHADER_FLOAT4			uLightAmb[ 6 /* 頂点シェーダーで使用するのは 3 */ ] ;					// アンビエントカラー
	DX_LINUX_SHADER_FLOAT4			uLightRange_FallOff_AT0_AT1[ 6 /* 頂点シェーダーで使用するのは 3 */ ] ;	// x:有効距離  y:スポットライト用FllOff  z:距離による減衰処理用パラメータ０  w:距離による減衰処理用パラメータ１
	DX_LINUX_SHADER_FLOAT4			uLightAT2_SpotP0_SpotP1[ 6 /* 頂点シェーダーで使用するのは 3 */ ] ;		// x:距離による減衰処理用パラメータ２  y:スポットライト用パラメータ０( cos( Phi / 2.0 ) )  z:スポットライト用パラメータ１( 1.0 / ( cos( Theta / 2.0 ) - cos( Phi / 2.0 ) ) )


	// 頂点シェーダー用
	DX_LINUX_SHADER_FLOAT4			uAntiVPMat[ 4 ] ;							// ビューポート行列の効果を打ち消す行列
	DX_LINUX_SHADER_FLOAT4			uViewMat[ 3 ] ;								// ワールド　→　ビュー行列
	DX_LINUX_SHADER_FLOAT4			uProjMat[ 4 ] ;								// ビュー　→　プロジェクション行列
	DX_LINUX_SHADER_FLOAT4			uFog ;										// フォグ用パラメータ( x:end/(end - start)  y:-1/(end - start)  z:density  w:自然対数の低 )

	DX_LINUX_SHADER_FLOAT4			uToonOutLineSize ;							// トゥーンの輪郭線の大きさ
	DX_LINUX_SHADER_FLOAT4			uDifSrc_SpeSrc_MulSpeCol ;					// x:ディフューズカラー( 0.0:マテリアル  1.0:頂点 )  y:スペキュラカラー(   0.0:マテリアル  1.0:頂点 )  z:スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )

	DX_LINUX_SHADER_FLOAT4			uSMLVPMat[ 12 /* [ 3 ][ 4 ] */ ] ;	// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの

	DX_LINUX_SHADER_FLOAT4			uTexMat[ 6 /* [ DX_VS_CONSTF_TEXTURE_MATRIX_NUM ][ 2 ] */ ] ;	// テクスチャ座標操作用行列
	DX_LINUX_SHADER_FLOAT4			uLWMat[ 162 /* DX_VS_CONSTF_WORLD_MAT_NUM * 3 */ ] ;		// ローカル　→　ワールド行列


	// フラグメントシェーダー用
	DX_LINUX_SHADER_FLOAT			uMulAlphaColor ;							// カラーにアルファ値を乗算するかどうか( 0.0:乗算しない  1.0:乗算する )

	DX_LINUX_SHADER_FLOAT			uATestRef ;									// αテスト値
	DX_LINUX_SHADER_INT				uATestCmpMode ;

	DX_LINUX_SHADER_FLOAT4			uFogColor ;									// フォグカラー
	DX_LINUX_SHADER_FLOAT4			uFactorColor ;								// ファクターカラー
	DX_LINUX_SHADER_FLOAT4			uIgnoreTextureColor ;						// テクスチャカラー無視処理用カラー
	DX_LINUX_SHADER_FLOAT4			uAddColor ;									// 加算する色

	DX_LINUX_SHADER_FLOAT4			uShadowMapLightEnable[ 6 ] ;				// シャドウマップ０・１・２のライト有効情報( x:シャドウマップ０でライトnが有効か  y:シャドウマップ１でライトnが有効か  z:シャドウマップ２でライトnが有効か )
	DX_LINUX_SHADER_FLOAT2			uShadowMap_DAdj_Grad[ 3 ] ;					// シャドウマップの情報( x:閾値深度補正値  y:グラデーション範囲  )

	int								UpdateCount ;								// 定数の内容が変更された回数

	int								SetNormalMaterial ;							// 定数バッファに標準描画用のマテリアルパラメータが設定されているかどうか( TRUE:設定されている  FALSE:設定されていない )
	int								SetNormalColorSource ;						// 定数バッファに標準描画用の「ディフューズカラー・スペキュラカラーをマテリアルを使用するか、頂点データを使用するか」の設定がされているかどうか( TRUE:設定されている  FALSE:設定されていない )
	int								SetNormalTextureAddressTransformMatrix ;	// 定数バッファに標準描画用のテクスチャ座標用行列が設定されているかどうか( TRUE:設定されている  FALSE:設定されていない )
} ;

// シェーダー情報リスト構造体
struct GRAPHICS_LINUX_SHADER_LIST
{
	int								UseNum ;									// Shaders で使用している配列の数
	GRAPHICS_LINUX_SHADER			Shaders[ SHADERS_LIST_DATA_NUM ] ;			// シェーダー情報
	GRAPHICS_LINUX_SHADER_LIST	*Next ;										// 次のシェーダーリスト
} ;

// Linux のシェーダー情報を纏めた構造体
struct GRAPHICS_HARDWARE_LINUX_SHADER
{
	GRAPHICS_HARDWARE_LINUX_CONSTANT				Constant ;						// 定数情報を纏めた構造体

	GRAPHICS_LINUX_SHADER_LIST					*ShadersListFirst ;				// シェーダー情報リスト(先端)
	GRAPHICS_LINUX_SHADER_LIST					*ShadersListLast ;				// シェーダー情報リスト(末端)

	GRAPHICS_HARDWARE_LINUX_SHADER_BASE			Base ;							// 標準描画処理用のシェーダーを纏めた構造体
	GRAPHICS_HARDWARE_LINUX_SHADER_BASE3D			Base3D ;						// 標準３Ｄ描画処理用のシェーダーを纏めた構造体

#ifndef DX_NON_MODEL
	GRAPHICS_HARDWARE_LINUX_SHADER_MODEL			Model ;							// モデル描画処理用のシェーダーを纏めた構造体
#endif // DX_NON_MODEL
} ;









// LinuxDevice に設定している値を纏めた構造体
struct GRAPHICS_HARDDATA_LINUX_DEVICE_STATE
{
	int								FogEnable ;										// フォグが有効かどうか( TRUE:有効  FALSE:無効 )
	int								FogMode ;										// フォグモード
	DWORD							FogColor ;										// フォグカラー
	float							FogStart ;										// フォグ開始距離
	float							FogEnd ;										// フォグ終了距離
	float							FogDensity ;									// フォグ密度

	COLOR_U8						BackgroundColor ;								// 背景色

	COLOR_F							GlobalAmbientLightColor ;						// グローバルアンビエントライトカラー
	int								LightEnableMaxIndex ;							// 有効なライトの最大インデックス
	int								LightEnableFlag[ DX_LINUX_COMMON_CONST_LIGHT_NUM ] ;// ライトが有効かどうかフラグ( TRUE:有効  FALSE:無効 )
	LIGHTPARAM						LightParam[ DX_LINUX_COMMON_CONST_LIGHT_NUM ] ;	// ライトのパラメータ
	int								Lighting ;										// ライティングを行うかどうかのフラグ( TRUE:ライティングを行う  FALSE:ライティングなし )

	int								UseSpecular ;									// スペキュラを使用するかどうか
	MATERIALPARAM					Material ;										// マテリアル
	float							MaterialTypeParam[ MATERIAL_TYPEPARAM_MAX_NUM ] ;	// マテリアル別タイプパラメータ
	int								MaterialUseVertexDiffuseColor ;					// 頂点のディフューズカラーをマテリアルのディフューズカラーとして使用するかどうか
	int								MaterialUseVertexSpecularColor ;				// 頂点のスペキュラカラーをマテリアルのスペキュラカラーとして使用するかどうか

	int								DrawMode ;										// 描画モード( DX_DRAWMODE_BILINEAR など )、標準のフィルタリングモードではない場合は -1
	int								MaxAnisotropy ;									// 最大異方性、全てのサンプラに対して同じ設定が行われていない場合は -1
	int								MaxAnisotropyDim[ USE_TEXTURESTAGE_NUM ] ;		// 各サンプラの最大異方性
	GLint							TexMagFilter[ USE_TEXTURESTAGE_NUM ] ;			// 拡大フィルター( GL_TEXTURE_MAG_FILTER のパラメータ、 GL_NEAREST など )
	GLint							TexMinFilter[ USE_TEXTURESTAGE_NUM ] ;			// 縮小フィルター( GL_TEXTURE_MIN_FILTER のパラメータ、 GL_NEAREST など )
	GLint							TexAddressModeU[ USE_TEXTURESTAGE_NUM ] ;		// テクスチャアドレスモードＵ( GL_TEXTURE_WRAP_S のパラメータ、 GL_REPEAT など )
	GLint							TexAddressModeV[ USE_TEXTURESTAGE_NUM ] ;		// テクスチャアドレスモードＶ( GL_TEXTURE_WRAP_T のパラメータ、 GL_REPEAT など )
	GLint							TexAddressModeW[ USE_TEXTURESTAGE_NUM ] ;		// テクスチャアドレスモードＷ( GL_TEXTURE_WRAP_R のパラメータ、 GL_REPEAT など )

	RECT							ViewportRect ;									// ビューポートの矩形

	int								CullEnable ;									// カリングを行うかどうか
	GLenum							CullFace ;										// カリング面( GL_BACK など )
	int								DepthBias ;										// 深度値バイアス

	int								DepthEnable ;									// 深度バッファの有効フラグ( 深度バッファ有効、無効は glEnable( GL_DEPTH_TEST ), glDisable( GL_DEPTH_TEST ) で行う )
	int								DepthWriteEnable ;								// 深度バッファへの書き込みが有効かどうか( 書き込みを行うかどうかの設定は glDepthMask で行う )
	GLenum							DepthFunc ;										// 深度値の比較モード( GL_LEQUAL など )

	int								BlendMode ;										// 現在デバイスに設定されているブレンドモード、プリセットのブレンドモード以外の場合は -1 が入る
	int								BlendEnable ;									// ブレンド処理を行うかどうか( DX_BLENDMODE_CUSTOM 用 )
	int								BlendRGBSrc ;									// RGBのソースブレンド( DX_BLENDMODE_CUSTOM 用 )
	int								BlendRGBDest ;									// RGBのデストブレンド( DX_BLENDMODE_CUSTOM 用 )
	int								BlendRGBOp ;									// RGBのブレンド処理( DX_BLENDMODE_CUSTOM 用 )
	int								BlendASrc ;										// Aのソースブレンド( DX_BLENDMODE_CUSTOM 用 )
	int								BlendADest ;									// Aのデストブレンド( DX_BLENDMODE_CUSTOM 用 )
	int								BlendAOp ;										// Aのブレンド処理( DX_BLENDMODE_CUSTOM 用 )
	int								NotWriteAlphaChannelFlag ;						// アルファチャンネルの内容を書き換えないかどうかのフラグ

	GRAPHICS_LINUX_TEXTURE		*SetTexture[ USE_TEXTURESTAGE_NUM ] ;			// 描画時に使用するテクスチャ

	RECT							ScissorRect ;									// シザー矩形
	int								AlphaTestEnable ;								// アルファテストを使用するかどうか
	int								AlphaTestCmpMode ;								// アルファテストで使用する比較モード( DX_CMP_NEVER など )
	int								AlphaTestRef ;									// アルファテストで使用する参照値
	int								AlphaTestModeShaderIndex ;						// アルファテストの設定値に対応するフラグメントシェーダーのアルファテストモード( LINUX_FS_ALPHATEST_CMP_GREATER など )

	int								SetNormalShader ;								// 通常描画用シェーダーがセットされているかどうか
	GRAPHICS_LINUX_SHADER	*		SetShader ;										// 現在セットしてあるシェーダー
	GRAPHICS_LINUX_SHADER	*		SetShader_Force ;								// NULL 以外の場合優先的に使用するシェーダー( 特殊処理用 )

	GLuint							SetVertexBuffer ;								// 現在セットしてある頂点バッファ
	int								VertexAttrEnable[ LINUX_VERTEX_ATTR_NUM ] ;	// 有効になっている頂点データ

	GLuint							TargetFrameBuffer ;								// 描画対象のフレームバッファ
	GLuint							TargetFrameBufferWidth ;						// 描画対象のフレームバッファの幅
	GLuint							TargetFrameBufferHeight ;						// 描画対象のフレームバッファの高さ

	int								TextureAddressTransformMatrixUse ;				// テクスチャ座標変換行列を使用するかどうか( TRUE:使用する  FALSE:使用しない )
	MATRIX							TextureAddressTransformMatrix ;					// テクスチャ座標変換行列
} ;


// LinuxDevice の描画処理に関わる情報の構造体
struct GRAPHICS_HARDDATA_LINUX_DRAWINFO
{
	BYTE *						VertexBufferNextAddr ;						// 次のデータを格納すべき頂点バッファ内のアドレス
	BYTE						VertexBuffer[ DX_GLES2_VERTBUFFERSIZE ] ;	// 頂点バッファ
	int							Use3DVertex ;							// ３Ｄ頂点を使用しているかどうか( 1:使用している  0:使用していない )
	int							VertexNum ; 							// 頂点バッファに格納されている頂点の数
	int							VertexType ;							// 頂点バッファに格納されている頂点データ( VERTEXTYPE_TEX など )
	GLenum						PrimitiveType ;							// 頂点バッファに格納されている頂点データのプリミティブタイプ

	DWORD						DiffuseColor ;							// ディフューズカラー

	int							BeginSceneFlag ;						// Graphics_Linux_BeginScene を実行してあるかどうか
	int							BlendMaxNotDrawFlag ;					// ブレンド値が最大であることにより描画を行わないかどうか、フラグ
} ;

// LinuxDevice の画面関係の情報の構造体
struct GRAPHICS_HARDDATA_LINUX_SCREENINFO
{
	EGLDisplay					Display ;								// 出力先の EGLDisplay
	EGLSurface					Surface ;								// 出力先のディスプレイの EGLSurface
	EGLContext					Context ;								// 出力先のディスプレイの EGLContext
	EGLint						Width ;									// 出力先サーフェスの幅
	EGLint						Height ;								// 出力先サーフェスの高さ

	int							FrontScreenCopyTime ;					// 表画面を描画先としている場合に行う ScreenCopy を行った時間

	int							SubBackBufferTextureSizeX ;				// サブバックバッファテクスチャの横幅
	int							SubBackBufferTextureSizeY ;				// サブバックバッファテクスチャの縦幅
	GLuint						SubBackBufferTextureInternalFormat ;	// サブバックバッファテクスチャの内部フォーマット
	GLenum						SubBackBufferTexturePixelFormat ;		// サブバックバッファテクスチャのピクセルフォーマット
	GLuint						SubBackBufferTexturePixelType ;			// サブバックバッファテクスチャのピクセルタイプ
	void *						SubBackBufferTextureTempBuffer ;		// サブバックバッファテクスチャにイメージを転送する際に使用するバッファ
	GLuint						SubBackBufferFrameBuffer ;				// ScreenCopy や GetDrawScreen を実現するために使用するフレームバッファ
	GLuint						SubBackBufferTexture ;					// ScreenCopy や GetDrawScreen を実現するために使用するテクスチャバッファ
	GLuint						SubBackBufferDepthBuffer ;				// ScreenCopy や GetDrawScreen を実現するために使用する深度バッファ

	void *						DrawScreenLockImage ;					// 描画先のロック時に作成するテンポラリイメージ
	GLuint						DrawScreenLockImagePitch ;				// ロックイメージのピッチ
	// 以下 X11 バックエンド用
	_XDisplay *					XDisplay ;	// Display （実際の型は Display * が適切であるが名前が衝突するため内部名を使用）
	Window						XWindow ;	// Window
	Atom						_atom_WM_PROTOCOLS ;	// 終了判定用Atom
	Atom						_atom_WM_DELETE_WINDOW ;	// 終了判定用Atom
	Atom						_atom_XdndAware ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_XdndEnter ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_XdndLeave ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_XdndPosition ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_XdndStatus ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_XdndFinished ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_XdndDrop ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_XdndTypeList ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_XdndActionCopy ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_XdndSelection ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_text_uri_list ;	// ドラッグアンドドロップ用Atom
	Atom						_atom_CLIPBOARD ;	// クリップボード用Atom
	Atom						_atom_TARGETS ;	// クリップボード用Atom
	Atom						_atom_UTF8_STRING ;	// クリップボード用Atom
	Atom						_atom_INCR ;	// INCR Property（クリップボードなど）用Atom
	bool						_xdnd_cache_itsForMe ;	// ドラッグアンドドロップ用キャッシュ
	Window						_xdnd_cache_sourceWindow ;	// ドラッグアンドドロップ用キャッシュ
	char*						_xdnd_bufPtr ;	// ドラッグアンドドロップ用バッファ:ポインタ
	size_t						_xdnd_bufLen ;	// ドラッグアンドドロップ用バッファ:現在のバッファ長
	size_t						_xdnd_bufPos ;	// ドラッグアンドドロップ用バッファ:処理済みの位置
	size_t						_xdnd_bufNum ;	// ドラッグアンドドロップ用バッファ:残りファイル数
	char*						_clipboard_set_bufPtr ;	// クリップボード設定用バッファ:ポインタ
	size_t						_clipboard_set_bufLen ;	// クリップボード設定用バッファ:現在のバッファ長
	char*						_clipboard_get_bufPtr ;	// クリップボード取得用バッファ:ポインタ
	size_t						_clipboard_get_bufLen ;	// クリップボード取得用バッファ:現在のバッファ長
	EGLConfig					Config ;	// EGLのConfig値、Surface再作成に使用
} ;


// LinuxDevice の描画設定に関わる情報の構造体
struct GRAPHICS_HARDDATA_LINUX_DRAWSETTING
{
	int								DrawPrepParamFlag ;						// 前回 Graphics_Linux_DrawPreparation に入ったときの ParamFlag パラメータ
	int								DrawPrepAlwaysFlag ;					// 必ず Graphics_Linux_DrawPreparation を行うかどうかのフラグ

	int								AlphaChannelValidFlag ;					// αチャンネル有効フラグ
	int								AlphaTestValidFlag ;					// αテスト有効フラグ( Func は必ず D_D3DCMP_GREATEREQUAL )
	int								AlphaTestMode ;							// アルファテストモード
	int								AlphaTestParam ;						// アルファテストパラメータ
	int								BlendMode ;								// ブレンドモード
	int								BlendEnable ;							// ブレンド処理を行うかどうか( DX_BLENDMODE_CUSTOM 用 )
	int								BlendRGBSrc ;							// RGBのソースブレンド( DX_BLENDMODE_CUSTOM 用 )
	int								BlendRGBDest ;							// RGBのデストブレンド( DX_BLENDMODE_CUSTOM 用 )
	int								BlendRGBOp ;							// RGBのブレンド処理( DX_BLENDMODE_CUSTOM 用 )
	int								BlendASrc ;								// Aのソースブレンド( DX_BLENDMODE_CUSTOM 用 )
	int								BlendADest ;							// Aのデストブレンド( DX_BLENDMODE_CUSTOM 用 )
	int								BlendAOp ;								// Aのブレンド処理( DX_BLENDMODE_CUSTOM 用 )
	int								ChangeBlendParamFlag ;					// ブレンド設定に関わる部分の変更があったか、フラグ
	int								ChangeTextureFlag ;						// テクスチャが変更されたか、フラグ
	int								NotWriteAlphaChannelFlag ;				// アルファチャンネルの内容を書き換えないかどうかのフラグ
	int								IgnoreGraphColorFlag ;					// 描画する画像のＲＧＢ成分を無視するかどうかのフラグ
	int								IgnoreGraphAlphaFlag ;					// 描画する画像のＡ成分を無視するかどうかのフラグ
//	int								UseDiffuseRGBColor ;					// ディフューズカラーのＲＧＢ値を使用するか、フラグ
//	int								UseDiffuseAlphaColor ;					// ディフューズカラーのα値を使用するか、フラグ
	int								BlendGraphType ;						// ブレンド画像タイプ
	int								BlendGraphFadeRatio ;					// ブレンド画像のフェードパラメータ
	int								BlendGraphBorderParam ;					// ブレンド画像の境界パラメータ(０(ブレンド画像の影響０)　←　(ブレンド画像の影響少ない)　←　１２８(ブレンド画像の影響１００％)　→　(ブレンド画像の影響を超えて非描画部分が増える)　→２５５(全く描画されない) )
	int								BlendGraphBorderRange ;					// ブレンド画像の境界幅(０～２５５　狭い～広い　しかし４段階)
	float							BlendTextureWidth ;						// ブレンドテクスチャの幅
	float							BlendTextureHeight ;					// ブレンドテクスチャの高さ
	float							InvBlendTextureWidth ;					// ブレンドテクスチャの幅の逆数
	float							InvBlendTextureHeight ;					// ブレンドテクスチャの高さの逆数

	GRAPHICS_LINUX_TEXTURE		*RenderTexture ;						// 描画時に使用するテクスチャー
	GRAPHICS_LINUX_TEXTURE		*BlendTexture ;							// 描画時に描画テクスチャーとブレンドするαチャンネルテクスチャー

	int								CancelSettingEqualCheck ;				// このフラグが立っている場合は現在の設定と変更後の設定が同じかどうかに関わらず、必ず変更処理を実行する
} ;


// LinuxDevice 関係の設定情報の構造体
struct GRAPHICS_HARDDATA_LINUX_DEVICE_SETTING
{
	void					( *DeviceRestoreCallbackFunction)( void *Data ) ;	// デバイスロストから復帰したときに呼ぶ関数
	void					  *DeviceRestoreCallbackData ;						// デバイスロストから復帰したときに呼ぶ関数に渡すポインタ

	void					( *DeviceLostCallbackFunction)( void *Data ) ;		// デバイスロストから復帰する前に呼ぶ関数
	void					  *DeviceLostCallbackData ;							// デバイスロストから復帰する前に呼ぶ関数に渡すポインタ
} ;


// LinuxDevice の能力情報構造体
struct GRAPHICS_HARDDATA_LINUX_DEVICE_CAPS
{
	TCHAR *						Version ;
	TCHAR *						Renderer ;
	TCHAR *						Vendor ;

	unsigned char				Extensions[ LINUX_GL_EXTENSION_NUM ] ;			// Extensions
	int							ExtensionNum ;										// Extension の数
	TCHAR **					ExtensionNames ;									// Extension 名の配列

	TCHAR *						Shading_Language_Version ;

	GLint						Shader_Max_Vertex_Attribs ;							// GL_MAX_VERTEX_ATTRIBS
	GLint						Shader_Max_Vertex_Uniform_Vectors ;					// GL_MAX_VERTEX_UNIFORM_VECTORS
	GLint						Shader_Max_Varying_Vectors ;						// GL_MAX_VARYING_VECTORS
	GLint						Shader_Max_Combined_Texture_Image_Units ;			// GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
	GLint						Shader_Max_Vertex_Texture_Image_Units ;				// GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
	GLint						Shader_Max_Texture_Image_Units ;					// GL_MAX_TEXTURE_IMAGE_UNITS
	GLint						Shader_Max_Fragment_Uniform_Vectors ;				// GL_MAX_FRAGMENT_UNIFORM_VECTORS

	int							ScreenFormat ;										// 画面カラーフォーマット
	int							TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_NUM ] ;	// テクスチャフォーマット
	int							MaskAlphaFormat ;									// マスクアルファチャンネル用フォーマット
	GLenum						DepthBufferFormat[ ZBUFFER_FORMAT_NUM ] ;			// 深度バッファフォーマット
} ;


// Linux を使った描画処理で使用するデータの内の LinuxDevice のリセットで
// 情報を初期化する必要がある、LinuxDevice に密接に関わった情報を纏めた構造体
struct GRAPHICS_HARDDATA_LINUX_DEVICE
{
	GRAPHICS_HARDDATA_LINUX_DEVICE_CAPS			Caps ;					// デバイス能力情報構造体		

	GRAPHICS_HARDDATA_LINUX_DEVICE_SETTING		Setting ;				// デバイス関係の設定情報を纏めた構造体

	GRAPHICS_HARDDATA_LINUX_SCREENINFO			Screen ;				// 画面関係の情報構造体

	GRAPHICS_HARDWARE_LINUX_SHADER				Shader ;				// シェーダーを纏めた構造体

//	GRAPHICS_HARDWARE_LINUX_INPUTLAYOUT			InputLayout ;			// 入力レイアウトを纏めた構造体

	GRAPHICS_HARDDATA_LINUX_DRAWSETTING			DrawSetting ;			// 描画設定に関わる情報を纏めた構造体

	GRAPHICS_HARDDATA_LINUX_DRAWINFO				DrawInfo ;				// 描画処理に関わる情報を纏めた構造体

	GRAPHICS_HARDDATA_LINUX_DEVICE_STATE			State ;					// LinuxDevice に設定している値を纏めた構造体
} ;













// シェーダーコード一つの情報構造体
struct GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO
{
	void					*Binary ;								// シェーダーコードバイナリのアドレス
	int						Size ;									// シェーダーコードバイナリのデータサイズ
} ;


// Linux の 標準描画用のシェーダーコードの元データを纏めた構造体
struct GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE
{
	int						BaseShaderInitializeFlag ;				// シェーダーバイナリのセットアップが完了しているかどうかのフラグ( TRUE:完了している  FALSE:完了していない )

	// ライトインデックスリスト
	short					LightIndexList84[ 4 ][ 4 ][ 4 ][ 4 ][ 4 ][ 4 ] ;
	short					LightIndexList20[ 4 ][ 4 ][ 4 ] ;
	short					LightIndexList10[ 3 ][ 3 ][ 3 ] ;

	void					*Base2DShaderPackageImage ;				// 標準２Ｄ描画用シェーダーパッケージバイナリイメージ

	// 標準描画用の複雑な処理を行わない頂点シェーダー
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO BaseSimple_VS_Code[ LINUX_VERTEX_INPUTLAYOUT_NUM ] ;

	// 固定機能パイプライン互換のフラグメントシェーダー( テクスチャなし )
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO BaseNoneTex_FS_Code[ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// 固定機能パイプライン互換のフラグメントシェーダー( テクスチャあり )
	// [ ブレンド画像とのブレンドタイプ( 0:なし  1:DX_BLENDGRAPHTYPE_NORMAL  2:DX_BLENDGRAPHTYPE_WIPE  3:DX_BLENDGRAPHTYPE_ALPHA ) ]
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ テクスチャRGB無視( 0:無視しない  1:無視する ) ]
	// [ テクスチャAlpha無視( 0:無視しない  1:無視する ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO BaseUseTex_FS_Code[ 4 ][ LINUX_RENDER_TYPE_NUM ][ 2 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// マスク処理用シェーダー
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MaskEffect_VS_Code ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MaskEffect_FS_Code ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MaskEffect_ReverseEffect_FS_Code ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MaskEffect_UseGraphHandle_FS_Code[ 4 ] ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MaskEffect_UseGraphHandle_ReverseEffect_FS_Code[ 4 ] ;

	// 単純転送用シェーダー
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO StretchRect_VS_Code ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO StretchRectTex2_VS_Code ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO StretchRectTex8_VS_Code ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO StretchRect_FS_Code ;

	// クリア処理用シェーダー
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ClearRT_VS_Code ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ClearRT_FS_Code ;

#ifndef DX_NON_FILTER
	DXARC					FilterShaderBinDxa ;					// フィルターシェーダーオブジェクトファイルＤＸＡ構造体
	void					*FilterShaderBinDxaImage ;				// フィルターシェーダーオブジェクトファイルＤＸＡのバイナリイメージ

	void					*RGBAMixS_ShaderPackImage ;				// RGBAMix の S だけの組み合わせ２５６個のシェーダーパッケージバイナリイメージ

	// RGBAMix の S だけの組み合わせ２５６個のシェーダー[ R ][ G ][ B ][ A ][ 0:通常用  1:乗算済みアルファ用 ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO RGBAMixS_FS_Code[ 4 ][ 4 ][ 4 ][ 4 ][ 2 ] ;
#endif // DX_NON_FILTER
} ;

// Linux の 標準３Ｄ描画用のシェーダーコードの元データを纏めた構造体
struct GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE3D
{
	volatile int			Base3DShaderInitializeFlag ;			// シェーダーバイナリのセットアップが完了しているかどうかのフラグ( TRUE:完了している  FALSE:完了していない )

	void					*Base3DShaderPackageImage ;				// 標準３Ｄ描画用シェーダーパッケージバイナリイメージ

	// 標準３Ｄ描画用ピクセル単位ライティングタイプの頂点シェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// /* [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ 2:8ボーン内スキニングメッシュ ) ] */
	// /* [ バンプマップの有無( 0:無し 1:有り ) ] */
	// [ フォグタイプ ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO Base3D_PixelLighting_VS_Code[ 2 ]/*[ 3 ]*//*[ 2 ]*/[ 4 ] ;

	// 標準３Ｄ描画用ピクセル単位ライティングタイプのフラグメントシェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// /*[ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]*/
	// /*[ スペキュラマップ ( 0:無い 1:ある ) ]*/
	// /*[ バンプマップ ( 0:なし 1:あり ) ]*/
	// [ ライトインデックス ]
	// /*[ スペキュラの有無( 0:無し 1:有り ) ]*/
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO Base3D_PixelLighting_Normal_FS_Code[ 2 ]/*[ 5 ]*//*[ 2 ]*//*[ 2 ]*/[ 84 ]/*[ 2 ]*/[ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;



	// 標準３Ｄ描画用のシャドウマップへのレンダリング用頂点シェーダー
	// /*[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]*/
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO Base3D_ShadowMap_VS_Code/*[ 3 ]*/[ 2 ] ;

	// 標準３Ｄ描画用のライティングなし頂点シェーダー
	// /*[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]*/
	// [ フォグタイプ ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO Base3D_NoLighting_VS_Code/*[ 3 ]*/[ 4 ] ;

	// 標準３Ｄ描画用の頂点単位ライティング頂点シェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// /*[ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]*/
	// /*[ バンプマップの有無( 0:無し 1:有り ) ]*/
	// [ フォグタイプ ]
	// [ ライトインデックス ]
	// /*[ スペキュラの有無( 0:無し 1:有り ) ]*/
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO Base3D_VertexLighting_VS_Code[ 2 ]/*[ 3 ]*//*[ 2 ]*/[ 4 ][ 20 ]/*[ 2 ]*/ ;

	// 標準３Ｄ描画用のシャドウマップへのレンダリング用フラグメントシェーダー
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO Base3D_ShadowMap_Normal_FS_Code[ LINUX_FS_ALPHATEST_NUM ] ;

	// 標準３Ｄ描画用のライティングなしフラグメントシェーダー
	// /*[ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]*/
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO Base3D_NoLighting_Normal_FS_Code/*[ 5 ]*/[ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// 標準３Ｄ描画用の頂点単位ライティングフラグメントシェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// /* [ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ] */
	// /* [ スペキュラマップ ( 0:無い 1:ある ) ] */
	// /* [ バンプマップ ( 0:なし 1:あり ) ] */
	// [ ライトインデックス ]
	// /*[ スペキュラの有無( 0:無し 1:有り ) ]*/
	// [ 特殊効果( 0:通常描画  1:乗算描画  2:RGB反転  3:描画輝度４倍  4:乗算済みαブレンドモードの通常描画  5:乗算済みαブレンドモードのRGB反転  6:乗算済みαブレンドモードの描画輝度4倍 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO Base3D_VertexLighting_Normal_FS_Code[ 2 ]/*[ 5 ]*//*[ 2 ]*//*[ 2 ]*/[ 10 ]/*[ 2 ]*/[ LINUX_RENDER_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;
} ;

#ifndef DX_NON_MODEL

// Linux の モデル描画用のシェーダーコードの元データを纏めた構造体
struct GRAPHICS_HARDWARE_LINUX_SHADERCODE_MODEL
{
	volatile int			ModelShaderInitializeFlag ;				// モデル用のシェーダーバイナリのセットアップが完了しているかどうかのフラグ( TRUE:完了している  FALSE:完了していない )

	void					*ModelShaderPackImage ;					// ３Ｄモデル用シェーダーパッケージバイナリイメージ

	// モデル描画用ピクセル単位ライティングタイプの頂点シェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ 2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_PixelLighting_VS_Code[ 2 ][ 3 ][ 2 ][ 4 ] ;

	// モデル描画用ピクセル単位ライティングタイプのトゥーンタイプ１用フラグメントシェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ トゥーンレンダリングのスペキュラグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ バンプマップ ( 0:なし 1:あり ) ]
	// [ 有効なライトの数 - 1 ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_PixelLighting_ToonType1_FS_Code[ 2 ][ 2 ][ 2 ][ 2 ][ DX_LINUX_COMMON_CONST_LIGHT_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用ピクセル単位ライティングタイプのトゥーンタイプ２用フラグメントシェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ トゥーンレンダリングのスフィアマップの有無とブレンドタイプ( 0:スフィアマップは無い   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ 有効なライトの数 - 1 ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_PixelLighting_ToonType2_FS_Code[ 2 ][ 3 ][ DX_LINUX_COMMON_CONST_LIGHT_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用ピクセル単位ライティングタイプのフラグメントシェーダー( リストは配列の左から )
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]
	// [ バンプマップ ( 0:なし 1:あり ) ]
	// [ 有効なライトの数 - 1 ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_PixelLighting_Normal_FS_Code[ 2 ][ 5 ][ 2 ][ DX_LINUX_COMMON_CONST_LIGHT_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;






	// モデル描画用のマテリアル要素描画用フラグメントシェーダー
	// [ マテリアルタイプ( DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_UNORM など ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_MaterialType_FS_Code[ DX_MATERIAL_TYPE_NUM ][ LINUX_FS_ALPHATEST_NUM ] ;






	// モデル描画用のシャドウマップへのトゥーンレンダリング輪郭線描画用頂点シェーダー
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_ToonOutLine_ShadowMap_VS_Code[ 3 ][ 2 ] ;

	// モデル描画用のトゥーンレンダリングの輪郭線描画用頂点シェーダー
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_ToonOutLine_VS_Code[ 3 ][ 2 ][ 4 ] ;

	// モデル描画用のシャドウマップへのレンダリング用頂点シェーダー
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_ShadowMap_VS_Code[ 3 ][ 2 ] ;

	// モデル描画用のライティングなし頂点シェーダー
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_NoLighting_VS_Code[ 3 ][ 2 ][ 4 ] ;

	// モデル描画用の頂点単位ライティングあり頂点シェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ メッシュタイプ( 0:剛体メッシュ 1:4ボーン内スキニングメッシュ  2:8ボーン内スキニングメッシュ ) ]
	// [ バンプマップの有無( 0:無し 1:有り ) ]
	// [ フォグタイプ ]
	// [ ライトインデックス ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_VertexLighting_VS_Code[ 2 ][ 3 ][ 2 ][ 4 ][ 20 ] ;

	// モデル描画用のシャドウマップへのトゥーンレンダリング用フラグメントシェーダー
	// [ トゥーンレンダリングタイプ( 0:トゥーンレンダリング type 1   1:トゥーンレンダリング type 2 ) ]
	// [ トゥーンレンダリングのスフィアマップの有無とブレンドタイプ( 0:スフィアマップは無い   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_ShadowMap_Toon_FS_Code[ 2 ][ 3 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用のシャドウマップへのレンダリング用フラグメントシェーダー
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_ShadowMap_Normal_FS_Code[ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用のライティングなしトゥーン用フラグメントシェーダー( リストは配列の左から )
	// [ トゥーンレンダリングタイプ( 0:トゥーンレンダリング type 1   1:トゥーンレンダリング type 2 ) ]
	// [ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT or トゥーンレンダリングではない  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_NoLighting_Toon_FS_Code[ 2 ][ 2 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用のライティングなしフラグメントシェーダー
	// [ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_NoLighting_Normal_FS_Code[ 5 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用の頂点単位ライティングありトゥーンタイプ１用フラグメントシェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ トゥーンレンダリングのディフューズグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ トゥーンレンダリングのスペキュラグラデーションのブレンドタイプ( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ バンプマップ ( 0:なし 1:あり ) ]
	// [ ライトインデックス ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_VertexLighting_ToonType1_FS_Code[ 2 ][ 2 ][ 2 ][ 2 ][ 10 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用の頂点単位ライティングありトゥーンタイプ２用フラグメントシェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ トゥーンレンダリングのスフィアマップの有無とブレンドタイプ( 0:スフィアマップは無い   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ ライトインデックス ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_VertexLighting_ToonType2_FS_Code[ 2 ][ 3 ][ 10 ][ LINUX_FS_ALPHATEST_NUM ] ;

	// モデル描画用の頂点単位ライティングありフラグメントシェーダー
	// [ シャドウマップの有無( 0:無し 1:有り ) ]
	// [ マルチテクスチャのブレンドモード、MV1_LAYERBLEND_TYPE_TRANSLUCENT などに +1 したもの ( 0:マルチテクスチャ無し  1:αブレンド  2:加算ブレンド  3:乗算ブレンド  4:乗算ブレンド×2 ) ]
	// [ バンプマップ ( 0:なし 1:あり ) ]
	// [ ライトインデックス ]
	// [ フラグメントシェーダーのアルファテストモード ]
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO MV1_VertexLighting_Normal_FS_Code[ 2 ][ 5 ][ 2 ][ 10 ][ LINUX_FS_ALPHATEST_NUM ] ;
} ;

#endif // DX_NON_MODEL


// シェーダーコードの元データを纏めた構造体
struct GRAPHICS_HARDWARE_LINUX_SHADERCODE
{
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE	Base ;				// 標準描画処理用のシェーダーコードを纏めた構造体
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE3D	Base3D ;			// 標準３Ｄ描画処理用のシェーダーコードを纏めた構造体

#ifndef DX_NON_MODEL
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_MODEL	Model ;				// モデル描画処理用のシェーダーコードを纏めた構造体
#endif // DX_NON_MODEL
} ;


// Linux 関係の設定情報の構造体
struct GRAPHICS_HARDDATA_LINUX_SETTING
{
	int								UseMinFeatureLevelLinux ;				// Linux で使用する最低機能レベル
	int								UserDepthBufferSizeSet ;					// 外部から深度バッファサイズの指定があったかどうか
	int								UserDepthBufferBitDepthSet ;				// 外部から深度バッファのビット深度指定があったかどうか

	int								UseOldDrawModiGraphCodeFlag ;				// 以前の DrawModiGraph 関数コードを使用するかどうかのフラグ
} ;



// Linux のテクスチャ処理用情報の構造体
struct GRAPHICS_HARDDATA_LINUX_TEXTURE
{
	void *											CommonBuffer ;					// テクスチャ画像転送用の共有メモリ
	unsigned int									CommonBufferSize ;				// テクスチャ画像転送用の共有メモリのサイズ
} ;

// Linux を使ったグラフィックス処理情報の構造体
struct GRAPHICS_HARDDATA_LINUX
{
	int												UsePixelLightingShader ;	// ピクセル単位でライティングを行うタイプのシェーダーを使用するかどうか( TRUE:使用する  FALSE:使用しない )

	GRAPHICS_HARDDATA_LINUX_SETTING				Setting ;					// Linux 関係の設定情報

	GRAPHICS_HARDDATA_LINUX_DEVICE				Device ;					// LinuxDevice に密接に関連した情報

	GRAPHICS_HARDWARE_LINUX_SHADERCODE			ShaderCode ;				// FragmentShader や VertexShader の元となるシェーダーコードデータを纏めた構造体

	GRAPHICS_HARDDATA_LINUX_TEXTURE				Texture ;					// Linux のテクスチャ処理用情報の構造体
} ;

struct DX_LINUX_RENDER_BLEND_INFO
{
	int									RenderType ;
	int									BlendEnable ;
	GLenum								ColorSourceMul ;
	GLenum								ColorDestMul ;
	GLenum								ColorBlendFunc ;
	GLenum								AlphaSourceMul ;
	GLenum								AlphaDestMul ;
	GLenum								AlphaBlendFunc ;
	int									AlphaZeroNotDrawFlag ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern GLenum g_DXCmpModeToGLES2CompareFunc[ 9 ] ;
extern DX_LINUX_RENDER_BLEND_INFO g_DefaultBlendDescArray[ DX_BLENDMODE_NUM ] ;
extern PIXELFORMAT_INFO_LINUX g_LinuxPixelFormat[ LINUX_PIXEL_FORMAT_NUM ] ;
extern GRAPHICS_HARDDATA_LINUX GraphicsHardDataLinux ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// Linux の初期化関係
extern	int		Graphics_Linux_Initialize( void ) ;								// Linux を使用したグラフィックス処理の初期化を行う( 0:成功 -1:失敗 )
extern	int		Graphics_Linux_Terminate( void ) ;								// Linux を使用したグラフィックス処理の後始末を行う

extern	int		Graphics_Linux_ReleaseObjectAll( void ) ;							// すべての OpenGL ES 系オブジェクトを解放する
extern	int		Graphics_Linux_CreateObjectAll( void ) ;							// すべての OpenGL ES 系オブジェクトを作成する


















// シェーダーコード関係
extern	int		Graphics_Linux_ShaderCode_Base_Initialize( void ) ;				// Linux の標準描画用のシェーダーコードの初期化を行う
extern	int		Graphics_Linux_ShaderCode_Base_Terminate( void ) ;				// Linux の標準描画用のシェーダーコードの後始末を行う

extern	int		Graphics_Linux_ShaderCode_Base3D_Initialize( void ) ;			// Linux の標準３Ｄ描画用のシェーダーコードの初期化を行う
extern	int		Graphics_Linux_ShaderCode_Base3D_Terminate( void ) ;			// Linux の標準３Ｄ描画用のシェーダーコードの後始末を行う

#ifndef DX_NON_MODEL

extern	int		Graphics_Linux_ShaderCode_Model_Initialize( void ) ;			// Linux のモデル描画用のシェーダーコードの初期化を行う
extern	int		Graphics_Linux_ShaderCode_Model_Terminate( void ) ;			// Linux のモデル描画用のシェーダーコードの後始末を行う

#endif // DX_NON_MODEL

extern	int		Graphics_Linux_Shader_Initialize( void ) ;					// Linux のシェーダーの初期化を行う
extern	int		Graphics_Linux_Shader_Terminate( void ) ;						// Linux のシェーダーの後始末をする

extern	GRAPHICS_LINUX_SHADER *Graphics_Linux_GetShaderStruct( void ) ;		// 新しいシェーダー構造体を取得する
extern	int		Graphics_Linux_ShaderList_Terminate( void ) ;					// シェーダー情報リストの後始末を行う

extern	int		Graphics_Linux_VertexShader_Create(			GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO	*Code, GLuint *pDestShader, int Num ) ;		// シェーダーコードから頂点シェーダーを作成する
extern	void	Graphics_Linux_VertexShaderArray_Release(		GLuint										*pObject,					int Num ) ;		// 頂点シェーダー配列を解放する
extern	int		Graphics_Linux_FragmentShader_Create(			GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO	*Code, GLuint *pDestShader, int Num ) ;		// シェーダーコードからフラグメントシェーダーを作成する
extern	void	Graphics_Linux_FragmentShaderArray_Release(	GLuint										*pObject,					int Num ) ;		// フラグメントシェーダー配列を解放する

extern	GLuint	Graphics_Linux_Shader_GetUniformIndex(		GRAPHICS_LINUX_SHADER *ShaderInfo, const GLchar *UniformName ) ;						// シェーダーの Uniform 番号を取得する
extern	int		Graphics_Linux_Shader_Create(					GRAPHICS_LINUX_SHADER *ShaderInfo, GLuint VertexShader, GLuint FragmentShader ) ;		// 頂点シェーダーとフラグメントシェーダーからシェーダー情報をセットアップする
extern	void	Graphics_Linux_ShaderArray_Release(			GRAPHICS_LINUX_SHADER *pShaderInfo,	int Num ) ;										// シェーダー配列を解放する

extern	int		Graphics_Linux_Shader_Normal3DDraw_Setup( void ) ;				// ３Ｄ標準描画の指定の頂点用の描画用シェーダーをセットアップする
extern	int		Graphics_Linux_Shader_GetAlphaTestModeIndex( int AlphaTestEnable, int AlphaTestMode /* DX_CMP_NEVER 等 */ ) ;	// アルファテストの比較モードからフラグメントシェーダーのアルファテストモード( LINUX_FS_ALPHATEST_CMP_GREATER 等 )を取得する










// 頂点バッファ・インデックスバッファ関係










// Linux の画面関係
extern	int		Graphics_Linux_SetupSubBackBuffer( void ) ;															// サブバックバッファを使用する準備を行う( 0:成功  -1:失敗 )
extern	int		Graphics_Linux_TerminateSubBackBuffer( void ) ;														// サブバックバッファの後始末を行う( 0:成功  -1:失敗 )
extern	int		Graphics_Linux_FrontScreenProcess( void ) ;															// 表画面が描画先の際の定期処理















// Linux の画像関係
extern COLORDATA *Graphics_Linux_GetDataFormatColorData( int PixelFormat ) ;									// LINUX_PIXEL_FORMAT_R8G8B8A8 などのフォーマットに合わせたカラーフォーマット情報を取得する

// 描画先用テクスチャと描画用テクスチャが分かれている場合に、描画用テクスチャに描画先用テクスチャの内容を反映する
//extern	int		Graphics_Linux_UpdateDrawTexture( IMAGEDATA_ORIG_HARD_TEX *OrigTex, int TargetSurface, D_DXGI_FORMAT Format ) ;

extern	int		Graphics_Linux_Texture_SetupCommonBuffer( unsigned int Size ) ;		// テクスチャ画像転送用の共有メモリのセットアップを行う
extern	int		Graphics_Linux_Texture_TerminateCommonBuffer( void ) ;				// テクスチャ画像転送用の共有メモリの後始末を行う

// フレームバッファにテクスチャの単純転送を行う
extern	int		Graphics_Linux_StretchRect(
	GLuint					SrcTexture,				uint32_t SrcTextureWidth,       uint32_t SrcTextureHeight,       const RECT *SrcRect,			// NULL で全体
	GLuint					FrameBuffer,			uint32_t FrameBufferWidth,      uint32_t FrameBufferHeight,      const RECT *DestRect,			// NULL で全体
	GLenum					FilterType = GL_NEAREST,
	int						AlphaBlend = FALSE,
	GRAPHICS_LINUX_SHADER	*Shader = NULL,
	GLuint					BlendTexture = 0,		uint32_t BlendTextureWidth = 0, uint32_t BlendTextureHeight = 0, const RECT *BlendRect = NULL,	// NULL で全体
	VERTEX_TEX8_2D *		Texcoord8Vertex = NULL
) ;

// 指定のテクスチャの内容に読み込みアクセスできるようにする
extern	int		Graphics_Linux_Texture_Map(
	GLuint				TargetTexture,
	GLuint				TargetFrameBuffer/* = 0 */,
	GLuint				TargetWidth,
	GLuint				TargetHeight,
	const RECT *		MapRect /* NULL で全体 */,
	BASEIMAGE *			MapBaseImage,
	void **				MapTempImage
) ;

// 指定のテクスチャの内容への読み込みアクセスを終了する
extern	int		Graphics_Linux_Texture_Unmap( void * MapTempImage ) ;

// テクスチャにイメージデータを転送する
extern int Graphics_Linux_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
	const RECT				*SrcRect,
	const RECT				*DestRect,
	      GLuint			UseTex,
	      int				ImageNum,
	      int				TexWidth,
	      int				TexHeight,
	      int				TexUseWidth,
	      int				TexUseHeight,
	const BASEIMAGE			*RgbBaseImage,
	const BASEIMAGE			*AlphaBaseImage,
	      int				DestColorFormat,
	      int				RedIsAlphaFlag,
	      int				UseTransColorConvAlpha,
	      unsigned int		TransCode,
	      int				ASyncThread
) ;




// LinuxDevice の初期化関係
extern	int		Graphics_Linux_Device_Create( void ) ;																// Direct3DDevice11 オブジェクトの作成( 0:成功  -1:失敗 )
extern	int		Graphics_Linux_Device_Delete( void ) ;																// デバイスの破棄に相当する処理を行う

extern	int		Graphics_Linux_Device_Initialize( void ) ;															// Direct3DDevice11 関係の初期化









// LinuxDevice ステータス関係
extern	void	Graphics_Linux_DeviceState_RefreshRenderState( void ) ;												// 描画設定をリフレッシュ
extern	int		Graphics_Linux_DeviceState_SetBaseState( void ) ;														// Direct3DDevice11 の基本設定を行う
extern	int		Graphics_Linux_DeviceState_SetSampleFilterMode( GLenum Filter, int Sampler = -1 ) ;		// サンプラーのテクスチャフィルタリングモードを設定する
extern	int		Graphics_Linux_DeviceState_SetDepthEnable( BOOL DepthEnable )  ;									// 深度バッファの有効無効のセット
extern	int		Graphics_Linux_DeviceState_SetDepthWriteEnable( int Flag ) ;										// 深度バッファの書き込みの有無をセット
extern	int		Graphics_Linux_DeviceState_SetDepthFunc( GLenum DepthFunc ) ;						// 深度値の比較タイプをセット
extern	int		Graphics_Linux_DeviceState_SetDepthBias( int DepthBias ) ;										// 深度値のバイアスをセット
extern	int		Graphics_Linux_DeviceState_SetWorldMatrix( const MATRIX *Matrix ) ;								// ワールド変換用行列をセットする
extern	int		Graphics_Linux_DeviceState_SetViewMatrix( const MATRIX *Matrix ) ;								// ビュー変換用行列をセットする
extern	int		Graphics_Linux_DeviceState_SetProjectionMatrix( const MATRIX *Matrix ) ;							// 投影変換用行列をセットする
extern	int		Graphics_Linux_DeviceState_SetAntiViewportMatrix( const MATRIX *Matrix ) ;							// アンチビューポート行列をセットする
extern	int		Graphics_Linux_DeviceState_SetLightState( int LightNumber, LIGHTPARAM *Light ) ;					// ライトのセット
extern	int		Graphics_Linux_DeviceState_SetLightEnable( int LightNumber, int EnableState ) ;						// ライトの有効、無効を変更
extern	int		Graphics_Linux_DeviceState_SetMaterial( MATERIALPARAM *Material ) ;									// マテリアルのセット
extern	int		Graphics_Linux_DeviceState_SetSpecularEnable( int UseFlag ) ;										// スペキュラを使用するかどうかをセット
extern	int		Graphics_Linux_DeviceState_SetUseVertexDiffuseColor( int UseFlag ) ;								// 頂点のディフューズカラーをマテリアルのディフューズカラーとして使用するかどうかを設定する
extern	int		Graphics_Linux_DeviceState_SetUseVertexSpecularColor( int UseFlag ) ;								// 頂点のスペキュラカラーをマテリアルのスペキュラカラーとして使用するかどうかを設定する
extern	int		Graphics_Linux_DeviceState_SetCullMode( int State ) ;												// カリングの有無をセット 
extern	int		Graphics_Linux_DeviceState_SetAmbient( COLOR_F *Color ) ;											// アンビエントカラーのセット
extern	int		Graphics_Linux_DeviceState_SetTextureAddress( int AddressMode, int Sampler = -1 ) ;				// テクスチャーのアドレッシングモードをセットする
extern	int		Graphics_Linux_DeviceState_SetTextureAddressU( int AddressMode, int Sampler = -1 ) ;				// テクスチャーのアドレッシングモードをセットする
extern	int		Graphics_Linux_DeviceState_SetTextureAddressV( int AddressMode, int Sampler = -1 ) ;				// テクスチャーのアドレッシングモードをセットする
extern	int		Graphics_Linux_DeviceState_SetTextureAddressW( int AddressMode, int Sampler = -1 ) ;				// テクスチャーのアドレッシングモードをセットする
extern	int		Graphics_Linux_DeviceState_SetTextureAddressUVW( int AddressModeU, int AddressModeV, int AddressModeW, int Sampler = -1 ) ;		// テクスチャーのアドレッシングモードをセットする
extern	int		Graphics_Linux_DeviceState_SetTextureAddressTransformMatrix( int Use, MATRIX *Matrix ) ;			// テクスチャ座標変換行列をセットする
extern	int		Graphics_Linux_DeviceState_SetFogEnable( int Flag ) ;												// フォグを有効にするかどうかを設定する( TRUE:有効  FALSE:無効 )
extern	int		Graphics_Linux_DeviceState_SetFogVertexMode( int Mode /* DX_FOGMODE_NONE 等 */ ) ;				// フォグモードを設定する
extern	int		Graphics_Linux_DeviceState_SetFogColor( unsigned int Color ) ;									// フォグカラーを変更する
extern	int		Graphics_Linux_DeviceState_SetFogStartEnd( float Start, float End ) ;								// フォグが始まる距離と終了する距離を設定する( 0.0f ～ 1.0f )
extern	int		Graphics_Linux_DeviceState_SetFogDensity( float Density ) ;										// フォグの密度を設定する( 0.0f ～ 1.0f )
extern	int		Graphics_Linux_DeviceState_SetLighting( int UseFlag ) ;											// ライティングの有無フラグをセットする
extern	int		Graphics_Linux_DeviceState_SetMaxAnisotropy( int MaxAnisotropy, int Sampler = -1 ) ;				// 最大異方性をセットする
extern	int		Graphics_Linux_DeviceState_SetViewport( RECT *Viewport ) ;										// ビューポートをセットする
extern	int		Graphics_Linux_DeviceState_SetViewportEasy( int x1, int y1, int x2, int y2 ) ;					// ビューポートをセットする( 簡易版 )
extern	int		Graphics_Linux_DeviceState_SetDrawMode( int DrawMode ) ;											// 描画モードのセット
extern	int		Graphics_Linux_DeviceState_SetAlphaTestCmpMode( int AlphaTestCmpMode /* DX_CMP_NEVER など */ ) ;	// アルファテストで使用する比較モードを設定する
extern	int		Graphics_Linux_DeviceState_SetAlphaTestRef( int AlphaTestRef ) ;									// アルファテストで使用する参照値を設定する
extern	int		Graphics_Linux_DeviceState_SetMulAlphaColor( int UseMulAlphaColor ) ;								// カラーにアルファ値を乗算するかどうかを設定する
extern	int		Graphics_Linux_DeviceState_SetBackgroundColor( int Red, int Green, int Blue, int Alpha ) ;		// 背景色を設定する
extern	int		Graphics_Linux_DeviceState_SetFactorColor( const DX_LINUX_SHADER_FLOAT4 *FactorColor ) ;			// Factor Color を設定する
extern	int		Graphics_Linux_DeviceState_SetToonOutLineSize( float Size ) ;										// トゥーンレンダリングの輪郭線の太さを設定する
extern	int		Graphics_Linux_DeviceState_SetBlendMode( int BlendMode, int BlendEnable, int BlendRGBSrc, int BlendRGBDest, int BlendRGBOp, int BlendASrc, int BlendADest, int BlendAOp, int NotWriteAlphaChannelFlag ) ;			// 描画ブレンドモードのセット
extern	int		Graphics_Linux_DeviceState_SetRenderTarget( GLuint TargetFrameBuffer, GLuint TargetFrameBufferWidth, GLuint TargetFrameBufferHeight ) ;	// 描画対象の変更
extern	int		Graphics_Linux_DeviceState_SetShader( GRAPHICS_LINUX_SHADER *Shader, int NormalVertexShader = FALSE ) ;			// 使用するシェーダーを変更する
extern	int		Graphics_Linux_DeviceState_ResetShader( int SetNormalShaderCancel = FALSE ) ;						// シェーダーの使用を止める
extern	int		Graphics_Linux_DeviceState_UpdateShaderUniform( GRAPHICS_LINUX_SHADER *Shader, int LightNum, int UseLocalWorldMatrixUniformNum = 3 ) ;	// シェーダーの Uniform を更新する
extern	int		Graphics_Linux_DeviceState_SetupShaderVertexData( GRAPHICS_LINUX_SHADER *Shader, VERTEXBUFFER_INPUT_INFO_LINUX *VertexInputInfo, const void *VertexData, GLuint VertexBuffer = 0 ) ;	// 指定のシェーダーの頂点フォーマットの頂点データの描画準備を行う
extern	int		Graphics_Linux_DeviceState_SetVertexBuffer( GLuint VertexBuffer ) ;								// 使用する頂点バッファを変更する
extern	int		Graphics_Linux_DeviceState_SetTexture( int SlotIndex, GRAPHICS_LINUX_TEXTURE *Texture ) ;		// シェーダーで使用するテクスチャを変更する
extern	int		Graphics_Linux_DeviceState_NormalDrawSetup( void ) ;												// 標準描画の準備を行う
//extern	int		Graphics_Linux_DeviceState_SetupStateAndConstantBuffer( void ) ;								// 現在の設定に基づいて LINUXSampler、LINUXPrimitiveSetup、LINUXDepthStencilControl、LINUXBlendControl のセットアップと、定数バッファのアップデートを行う






// 描画設定関係関数
extern	int		Graphics_Linux_DrawSetting_SetDrawBlendMode( int BlendMode, int BlendEnable, int BlendRGBSrc, int BlendRGBDest, int BlendRGBOp, int BlendASrc, int BlendADest, int BlendAOp, int AlphaTestValidFlag, int AlphaChannelValidFlag ) ;	// 描画ブレンドモードの設定
extern	int		Graphics_Linux_DrawSetting_SetIgnoreDrawGraphColor( int EnableFlag ) ;							// 描画時の画像のＲＧＢを無視するかどうかを設定する
extern	int		Graphics_Linux_DrawSetting_SetIgnoreDrawGraphAlpha( int EnableFlag ) ;							// 描画時の画像のＡを無視するかどうかを設定する
extern	int		Graphics_Linux_DrawSetting_SetWriteAlphaChannelFlag( int NotFlag ) ;								// 描画先のアルファチャンネルの内容を書き換えるかを設定する
extern	int		Graphics_Linux_DrawSetting_SetDrawAlphaTest( int TestMode, int TestParam ) ;						// アルファテストの設定
extern	int		Graphics_Linux_DrawSetting_SetBlendTextureParam( int BlendType, int *Param ) ;					// ブレンドするテクスチャのパラメータをセットする
extern	int		Graphics_Linux_DrawSetting_SetTexture(      GRAPHICS_LINUX_TEXTURE *RenderTexture ) ;			// テクスチャをセットする 
extern	int		Graphics_Linux_DrawSetting_SetBlendTexture( GRAPHICS_LINUX_TEXTURE *BlendTexture ) ;			// ブレンドするテクスチャをセットする 












// Linux の描画処理準備関係
extern	int		Graphics_Linux_RenderVertex( int NextUse3DVertex = -1, int ASyncThread = FALSE ) ;	// 頂点バッファに溜まった頂点データをレンダリングする
extern	void	FASTCALL Graphics_Linux_DrawPreparation( int ParamFlag = 0 ) ;						// 描画準備を行う
extern	void	Graphics_Linux_RenderBegin( void ) ;													// 描画コマンドを開始する
extern	void	Graphics_Linux_RenderEnd( void ) ;													// 描画コマンドを終了する












// Linux を使った描画関係
extern	void	Graphics_Linux_DrawModiTex( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA_HARD_DRAW *DrawTex, bool SimpleDrawFlag ) ;								// テクスチャを変形して描画する
extern	void	Graphics_Linux_DrawPreparationToShader( int ParamFlag, int Is2D ) ;											// シェーダー描画用描画前セットアップ関数










#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_GRAPHICS

#endif // DX_GRAPHICSLINUX_H


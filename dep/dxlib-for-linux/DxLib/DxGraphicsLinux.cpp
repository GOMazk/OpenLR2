//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		描画処理プログラム( Linux )
// 
//  	Ver 3.24f
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

// インクルード ---------------------------------------------------------------
#include "DxGraphicsLinux.h"
#include "DxGraphicsFilterLinux.h"
#include "DxLive2DCubism4Linux.h"
#include "DxMaskLinux.h"
#include "DxMemoryLinux.h"
#include "DxSystemLinux.h"
#include "../DxSystem.h"
#include "../DxLog.h"
#include "../DxModel.h"
#include "../DxMemory.h"
#include "../DxMovie.h"
#include "../DxMask.h"
#include "../DxMath.h"
#include "../DxBaseFunc.h"
#include "../DxGraphics.h"
#include "../DxGraphicsFilter.h"
#include "../DxInput.h"
#include "../DxASyncLoad.h"
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//#include <error.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

#define LINUX_CHECKVALID_HARDWARE									\
		( TRUE )

// Graphics_Linux_DrawPreparation を呼ぶ定型文
#define DX_LINUX_DRAWPREP_NOTEX( FLAG )													\
	if( GLINUX.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_Linux_RenderBegin() ;	\
	if( GLINUX.Device.DrawSetting.RenderTexture != NULL )									\
		Graphics_Linux_DrawSetting_SetTexture( NULL ) ;									\
	if( GSYS.ChangeSettingFlag ||															\
		GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag != FALSE ||								\
		GLINUX.Device.DrawSetting.DrawPrepParamFlag  != FLAG )								\
		Graphics_Linux_DrawPreparation( FLAG ) ;												


#define DX_LINUX_DRAWPREP_TEX( ORIG, TEX, FLAG )											\
	FLAG |= DX_LINUX_DRAWPREP_TEXTURE ;													\
	if( (ORIG)->FormatDesc.AlphaChFlag   ) FLAG |= DX_LINUX_DRAWPREP_TEXALPHACH ;			\
	if( (ORIG)->FormatDesc.AlphaTestFlag ) FLAG |= DX_LINUX_DRAWPREP_TEXALPHATEST ;		\
	if( GLINUX.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_Linux_RenderBegin() ;		\
	if( GLINUX.Device.DrawSetting.RenderTexture != (TEX) )								\
		Graphics_Linux_DrawSetting_SetTexture( (TEX) ) ;									\
	if( GSYS.ChangeSettingFlag ||														\
		GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag != FALSE ||							\
		GLINUX.Device.DrawSetting.DrawPrepParamFlag  != FLAG )							\
		Graphics_Linux_DrawPreparation( FLAG ) ;


#define GETVERTEX_POINT( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 0 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_POINTS ||\
		  GLINUX.Device.DrawInfo.VertexNum + 1 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 0 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_POINTS ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_POINT3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 1 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_POINTS ||\
		  GLINUX.Device.DrawInfo.VertexNum + 1 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 1 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_POINTS ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_LINEBOX( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 0 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_TRIANGLES ||\
		  GLINUX.Device.DrawInfo.VertexNum + 24 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 0 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_TRIANGLES ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_LINETRIANGLE( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 0 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_LINES ||\
		  GLINUX.Device.DrawInfo.VertexNum + 6 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 0 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_LINES ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_LINETRIANGLE3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 1 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_LINES ||\
		  GLINUX.Device.DrawInfo.VertexNum + 6 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 1 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_LINES ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_LINE( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex == 1 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_LINES ||\
		  GLINUX.Device.DrawInfo.VertexNum + 2 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 0 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_LINES ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_LINE3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 1 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_LINES ||\
		  GLINUX.Device.DrawInfo.VertexNum + 2 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 1 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_LINES ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;



#define GETVERTEX_QUAD( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 0 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_TRIANGLES ||\
		  GLINUX.Device.DrawInfo.VertexNum + 6 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 0 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_TRIANGLES ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_TRIANGLE( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 0 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_TRIANGLES ||\
		  GLINUX.Device.DrawInfo.VertexNum + 3 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 0 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_TRIANGLES ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_TRIANGLE3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 1 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_TRIANGLES ||\
		  GLINUX.Device.DrawInfo.VertexNum + 3 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 1 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_TRIANGLES ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_BILLBOARD( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GLINUX.Device.DrawInfo.Use3DVertex != 1 ||\
		  GLINUX.Device.DrawInfo.PrimitiveType != GL_TRIANGLES ||\
		  GLINUX.Device.DrawInfo.VertexNum + 6 >= LINUX_VertexBuffer_MaxVertexNum[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_Linux_RenderVertex( 1 ) ;\
		GLINUX.Device.DrawInfo.PrimitiveType = GL_TRIANGLES ;\
	}\
	*( ( void ** )&p ) = ( void * )GLINUX.Device.DrawInfo.VertexBufferNextAddr ;




#define ADD4VERTEX_POINT		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) ;	\
		GLINUX.Device.DrawInfo.VertexNum ++ ;

#define ADD4VERTEX_POINT3D		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) ;	\
		GLINUX.Device.DrawInfo.VertexNum ++ ;

#define ADD4VERTEX_LINEBOX			\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 24 ;\
		GLINUX.Device.DrawInfo.VertexNum += 24 ;

#define ADD4VERTEX_LINETRIANGLE		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 6 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_LINETRIANGLE3D		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) * 6 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_LINE			\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 2 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 2 ;

#define ADD4VERTEX_LINE3D		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) * 2 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 2 ;

#define ADD4VERTEX_NOTEX		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 6 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_TEX			\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_2D ) * 6 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_BLENDTEX		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_BLENDTEX_2D ) * 6 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 6 ;

#define ADD3VERTEX_NOTEX		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 3 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 3 ;

#define ADD3VERTEX_TEX			\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_2D ) * 3 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 3 ;

#define ADD3VERTEX_BLENDTEX		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_BLENDTEX_2D ) * 3 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 3 ;

#define ADD3VERTEX_NOTEX3D		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) * 3 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 3 ;

#define ADD4VERTEX_BILLBOARD		\
		GLINUX.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_3D ) * 6 ;	\
		GLINUX.Device.DrawInfo.VertexNum += 6 ;

#define SETUPCOLOR( DEST )		\
	if( GSYS.HardInfo.UseVertexColorBGRAFormat )\
	{\
		DEST  = ( ( ( ( ( 	GLINUX.Device.DrawInfo.DiffuseColor & 0x00ff0000 ) >> 16 ) * Red   ) / 255 ) << 16 ) |\
				( ( ( ( ( 	GLINUX.Device.DrawInfo.DiffuseColor & 0x0000ff00 ) >> 8  ) * Green ) / 255 ) << 8  ) |\
				( ( ( ( (	GLINUX.Device.DrawInfo.DiffuseColor & 0x000000ff )       ) * Blue  ) / 255 )       ) | \
				( 			GLINUX.Device.DrawInfo.DiffuseColor & 0xff000000 ) ;\
	}\
	else\
	{\
		DEST  = ( ( ( ( ( 	GLINUX.Device.DrawInfo.DiffuseColor & 0x000000ff )       ) * Red   ) / 255 )       ) |\
				( ( ( ( ( 	GLINUX.Device.DrawInfo.DiffuseColor & 0x0000ff00 ) >> 8  ) * Green ) / 255 ) << 8  ) |\
				( ( ( ( (	GLINUX.Device.DrawInfo.DiffuseColor & 0x00ff0000 ) >> 16 ) * Blue  ) / 255 ) << 16 ) | \
				( 			GLINUX.Device.DrawInfo.DiffuseColor & 0xff000000 ) ;\
	}


// RGB値を輝度最大のRGB値に変換するためのボリュームテクスチャのサイズ
#define RGBTOVMAXRGB_TEX_SIZE		(128)

// 構造体宣言 -----------------------------------------------------------------

// 標準描画用の複雑な処理を行わない頂点シェーダーの情報
typedef struct tagGRAPHICS_LINUX_BASE_SIMPLE_VERTEXSHADER_INFO
{
	const char *					ShaderFileName ;			// 使用する頂点シェーダーファイル名
	VERTEXBUFFER_INPUT_INFO_LINUX	InputInfo ;					// 入力頂点データ情報
} GRAPHICS_LINUX_BASE_SIMPLE_VERTEXSHADER_INFO ;

// データ定義 -----------------------------------------------------------------

DX_LINUX_RENDER_BLEND_INFO g_DefaultBlendDescArray[ DX_BLENDMODE_NUM ] =
{
	{ LINUX_RENDER_TYPE_NORMAL,		FALSE, GL_ONE,					GL_ZERO,				GL_FUNC_ADD,				GL_ONE,					GL_ZERO,				GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_NOBLEND			ノーブレンド
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_SRC_ALPHA,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_SRC_ALPHA,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_ALPHA			αブレンド
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_SRC_ALPHA,			GL_ONE,					GL_FUNC_ADD,				GL_SRC_ALPHA,			GL_ONE,					GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_ADD				加算ブレンド
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ONE_MINUS_DST_COLOR,	GL_ZERO,				GL_FUNC_ADD,				GL_ONE_MINUS_DST_ALPHA,	GL_ZERO,				GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_SUB					減算ブレンド
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ZERO,					GL_SRC_COLOR,			GL_FUNC_ADD,				GL_ZERO,				GL_SRC_ALPHA,			GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_MUL				乗算ブレンド
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_SRC_ALPHA,			GL_ONE,					GL_FUNC_ADD,				GL_SRC_ALPHA,			GL_ONE,					GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_SUB2			内部処理用減算ブレンド１
	{ LINUX_RENDER_TYPE_NORMAL,		FALSE, GL_ONE,					GL_ZERO,				GL_FUNC_ADD,				GL_ONE,					GL_ZERO,				GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_XOR				XORブレンド(非対応)
	{ LINUX_RENDER_TYPE_NORMAL,		FALSE, GL_ONE,					GL_ZERO,				GL_FUNC_ADD,				GL_ONE,					GL_ZERO,				GL_FUNC_ADD,				 TRUE },	// 欠番
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ZERO,					GL_ONE,					GL_FUNC_ADD,				GL_ZERO,				GL_ONE,					GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_DESTCOLOR		カラーは更新されない
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ONE_MINUS_DST_COLOR,	GL_ZERO,				GL_FUNC_ADD,				GL_ONE_MINUS_DST_ALPHA,	GL_ZERO,				GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_INVDESTCOLOR		描画先の色の反転値を掛ける
	{ LINUX_RENDER_TYPE_INVERSE,		TRUE,  GL_SRC_ALPHA,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_SRC_ALPHA,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_INVSRC			描画元の色を反転する
	{ LINUX_RENDER_TYPE_MUL,			TRUE,  GL_ZERO,					GL_SRC_COLOR,			GL_FUNC_ADD,				GL_ZERO,				GL_SRC_ALPHA,			GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_MULA			アルファチャンネル考慮付き乗算ブレンド
	{ LINUX_RENDER_TYPE_X4,			TRUE,  GL_SRC_ALPHA,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_SRC_ALPHA,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_ALPHA_X4		αブレンドの描画側の輝度を最大４倍にできるモード
	{ LINUX_RENDER_TYPE_X4,			TRUE,  GL_SRC_ALPHA,			GL_ONE,					GL_FUNC_ADD,				GL_SRC_ALPHA,			GL_ONE,					GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_ADD_X4			加算ブレンドの描画側の輝度を最大４倍にできるモード
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ONE,					GL_ZERO,				GL_FUNC_ADD,				GL_ONE,					GL_ZERO,				GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_SRCCOLOR		描画元のカラーでそのまま描画される
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_HALF_ADD		半加算ブレンド
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_SRC_ALPHA,			GL_ONE,					GL_FUNC_REVERSE_SUBTRACT,	GL_SRC_ALPHA,			GL_ONE,					GL_FUNC_REVERSE_SUBTRACT,	 TRUE },	// DX_BLENDMODE_SUB1			出力ブレンドが使用可能な場合の減算ブレンド

	{ LINUX_RENDER_TYPE_PMA_NORMAL,	TRUE,  GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_PMA_ALPHA		乗算済みαブレンドモードのαブレンド
	{ LINUX_RENDER_TYPE_PMA_NORMAL,	TRUE,  GL_ONE,					GL_ONE,					GL_FUNC_ADD,				GL_ONE,					GL_ONE,					GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_PMA_ADD			乗算済みαブレンドモードの加算ブレンド
	{ LINUX_RENDER_TYPE_PMA_NORMAL,	TRUE,  GL_ONE,					GL_ONE,					GL_FUNC_REVERSE_SUBTRACT,	GL_ONE,					GL_ONE,					GL_FUNC_REVERSE_SUBTRACT,	 TRUE },	// DX_BLENDMODE_PMA_SUB			乗算済みαブレンドモードの減算ブレンド
	{ LINUX_RENDER_TYPE_PMA_INVERSE,	TRUE,  GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_PMA_INVSRC		乗算済みαブレンドモードの描画元の色を反転する
	{ LINUX_RENDER_TYPE_PMA_X4,		TRUE,  GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_PMA_ALPHA_X4	乗算済みαブレンドモードのαブレンドの描画側の輝度を最大４倍にできるモード
	{ LINUX_RENDER_TYPE_PMA_X4,		TRUE,  GL_ONE,					GL_ONE,					GL_FUNC_ADD,				GL_ONE,					GL_ONE,					GL_FUNC_ADD,				 TRUE },	// DX_BLENDMODE_PMA_ADD_X4		乗算済みαブレンドモードの加算ブレンドの描画側の輝度を最大４倍にできるモード

	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ZERO,					GL_ZERO,				GL_FUNC_ADD,				GL_ZERO,				GL_ZERO,				GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_LIVE2D_ZERO		Live2D のブレンドモード Zero 用
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_LIVE2D_NORMAL	Live2D のブレンドモード Normal 用
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ONE,					GL_ONE,					GL_FUNC_ADD,				GL_ZERO,				GL_ONE,					GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_LIVE2D_ADD		Live2D のブレンドモード Add 用
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_DST_COLOR,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_ZERO,				GL_ONE,					GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_LIVE2D_MULT		Live2D のブレンドモード Mult 用
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ZERO,					GL_ONE_MINUS_SRC_COLOR,	GL_FUNC_ADD,				GL_ZERO,				GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_LIVE2D_MASK		Live2D のブレンドモード Mask 用

	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_SRC_ALPHA,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_ONE,					GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_SPINE_NORMAL	Spine のブレンドモード Normal 用
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_SRC_ALPHA,			GL_ONE,					GL_FUNC_ADD,				GL_ONE,					GL_ONE,					GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_SPINE_ADDITIVE	Spine のブレンドモード Additive 
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_DST_COLOR,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_ONE_MINUS_SRC_ALPHA,	GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_SPINE_MULTIPLY	Spine のブレンドモード Multiply 
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ONE,					GL_ONE_MINUS_SRC_COLOR,	GL_FUNC_ADD,				GL_ONE_MINUS_SRC_COLOR,	GL_ONE_MINUS_SRC_COLOR,	GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_SPINE_SCREEN	Spine のブレンドモード Screen 用

	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_SRC_ALPHA,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				GL_SRC_ALPHA,			GL_ONE_MINUS_SRC_ALPHA,	GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_CUSTOM			カスタムブレンド

	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ZERO,					GL_ONE,					GL_FUNC_ADD,				GL_ONE,					GL_ZERO,				GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_DST_RGB_SRC_A		描画元の A のみを書き込む( 描画先の RGB は変更されない )
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ZERO,					GL_ONE,					GL_FUNC_ADD,				GL_ONE_MINUS_DST_ALPHA,	GL_ZERO,				GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_INVDESTCOLOR_A		描画先の A の反転値を掛ける( 描画先の RGB は変更されない )
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ZERO,					GL_ONE,					GL_FUNC_ADD,				GL_ZERO,				GL_SRC_ALPHA,			GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_MUL_A				A のみの乗算ブレンド( 描画先の RGB は変更されない )
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ZERO,					GL_SRC_ALPHA,			GL_FUNC_ADD,				GL_ONE_MINUS_DST_ALPHA,	GL_ZERO,				GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_PMA_INVDESTCOLOR_A	乗算済みα用の DX_BLENDMODE_INVDESTCOLOR_A
	{ LINUX_RENDER_TYPE_NORMAL,		TRUE,  GL_ZERO,					GL_SRC_ALPHA,			GL_FUNC_ADD,				GL_ZERO,				GL_SRC_ALPHA,			GL_FUNC_ADD,				FALSE },	// DX_BLENDMODE_PMA_MUL_A			乗算済みα用の DX_BLENDMODE_MUL_A
} ;

// ＤＸライブラリのブレンド要素タイプを OpenGL ES の要素タイプに変換するためのテーブル
static const GLenum DxBlendTypeToGLTable[ DX_BLEND_NUM ] =
{
	GL_ZERO,					// DX_BLEND_ZERO
	GL_ONE,						// DX_BLEND_ONE
	GL_SRC_COLOR,				// DX_BLEND_SRC_COLOR
	GL_ONE_MINUS_SRC_COLOR,		// DX_BLEND_INV_SRC_COLOR
	GL_SRC_ALPHA,				// DX_BLEND_SRC_ALPHA
	GL_ONE_MINUS_SRC_ALPHA,		// DX_BLEND_INV_SRC_ALPHA
	GL_DST_COLOR,				// DX_BLEND_DEST_COLOR
	GL_ONE_MINUS_DST_COLOR,		// DX_BLEND_INV_DEST_COLOR
	GL_DST_ALPHA,				// DX_BLEND_DEST_ALPHA
	GL_ONE_MINUS_DST_ALPHA,		// DX_BLEND_INV_DEST_ALPHA
	GL_SRC_ALPHA_SATURATE,		// DX_BLEND_SRC_ALPHA_SAT
} ;

// ＤＸライブラリのブレンド処理タイプを OpenGL ES の処理タイプに変換するためのテーブル
static const GLenum DxBlendOpToGLTable[ DX_BLENDOP_NUM ] =
{
	GL_FUNC_ADD,				// DX_BLENDOP_ADD
	GL_FUNC_SUBTRACT,			// DX_BLENDOP_SUBTRACT
	GL_FUNC_REVERSE_SUBTRACT,	// DX_BLENDOP_REV_SUBTRACT
	GL_FUNC_ADD,				// DX_BLENDOP_MIX
	GL_FUNC_ADD,				// DX_BLENDOP_MAX
} ;

// 頂点バッファに格納できる頂点の最大数のテーブル
static const int LINUX_VertexBuffer_MaxVertexNum[ LINUX_VERTEX_INPUTLAYOUT_NUM ] =
{
	DX_GLES2_VERTBUFFERSIZE / sizeof( VERTEX_NOTEX_2D    ),
	DX_GLES2_VERTBUFFERSIZE / sizeof( VERTEX_2D          ),
	DX_GLES2_VERTBUFFERSIZE / sizeof( VERTEX_BLENDTEX_2D ),
	DX_GLES2_VERTBUFFERSIZE / sizeof( VERTEX_NOTEX_3D    ),
	DX_GLES2_VERTBUFFERSIZE / sizeof( VERTEX_3D          ),
	DX_GLES2_VERTBUFFERSIZE / sizeof( VERTEX3D           ),
	DX_GLES2_VERTBUFFERSIZE / sizeof( VERTEX3DSHADER     ),
} ;

// ３Ｄ頂点かどうかと頂点タイプの組み合わせに対応する頂点データタイプ[ 0:２Ｄ頂点  1:３Ｄ頂点 ][ 頂点タイプ ]
static const int g_VertexTypeToInputLayout[ 2 ][ VERTEXTYPE_NUM ] =
{
	{
		LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D,
		LINUX_VERTEX_INPUTLAYOUT_2D,
		LINUX_VERTEX_INPUTLAYOUT_BLENDTEX_2D,
	},

	{
		LINUX_VERTEX_INPUTLAYOUT_NOTEX_3D,
		LINUX_VERTEX_INPUTLAYOUT_3D,
		-1,
	},
} ;

// 単位行列
static MATRIX LINUX_GlobalIdentMatrix =
{
	{
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	}
} ;

// ＤＸライブラリのプリミティブタイプを SceGxmPrimitiveType に変換するためのテーブル
GLenum g_DXPrimitiveTypeToGLES2PrimitiveType[] =
{
	0,
	GL_POINTS,				// DX_PRIMTYPE_POINTLIST		(1)
	GL_LINES,				// DX_PRIMTYPE_LINELIST			(2)
	GL_LINE_STRIP,			// DX_PRIMTYPE_LINESTRIP		(3)
	GL_TRIANGLES,			// DX_PRIMTYPE_TRIANGLELIST		(4)
	GL_TRIANGLE_STRIP,		// DX_PRIMTYPE_TRIANGLESTRIP	(5)
	GL_TRIANGLE_FAN,		// DX_PRIMTYPE_TRIANGLEFAN		(6)
} ;

// ＤＸライブラリの比較モードを SceGxmDepthFunc に変換するためのテーブル
GLenum g_DXCmpModeToGLES2CompareFunc[ 9 ] =
{
	0,
	GL_NEVER,				// DX_CMP_NEVER			(1)		// FALSE
	GL_LESS,				// DX_CMP_LESS			(2)		// Src <  Dest		DrawAlpha <  TestParam
	GL_EQUAL,				// DX_CMP_EQUAL			(3)		// Src == Dest		DrawAlpha == TestParam
	GL_LEQUAL,				// DX_CMP_LESSEQUAL		(4)		// Src <= Dest		DrawAlpha <= TestParam
	GL_GREATER,				// DX_CMP_GREATER		(5)		// Src >  Dest		DrawAlpha >  TestParam
	GL_NOTEQUAL,			// DX_CMP_NOTEQUAL		(6)		// Src != Dest		DrawAlpha != TestParam
	GL_GEQUAL,				// DX_CMP_GREATEREQUAL	(7)		// Src >= Dest		DrawAlpha >= TestParam
	GL_ALWAYS,				// DX_CMP_ALWAYS		(8)		// TRUE
} ;

#define GLES2WrapModeToDXTexAddrMode( GLMode )	\
	((GLMode) == GL_REPEAT ? DX_TEXADDRESS_WRAP : ( (GLMode) == GL_MIRRORED_REPEAT ? DX_TEXADDRESS_MIRROR : DX_TEXADDRESS_CLAMP ))


// ＤＸライブラリのテクスチャアドレスモードを LINUXWrapMode に変換するためのテーブル
GLenum g_DXTexAddrModeToGLES2WrapMode[ DX_TEXADDRESS_NUM ] =
{
	0,						// ＤＸライブラリでは未使用
	GL_REPEAT,				// DX_TEXADDRESS_WRAP	(1)
	GL_MIRRORED_REPEAT,		// DX_TEXADDRESS_MIRROR	(2)
	GL_CLAMP_TO_EDGE,		// DX_TEXADDRESS_CLAMP	(3)
	GL_CLAMP_TO_EDGE,		// DX_TEXADDRESS_BORDER	(4)
} ;

// 頂点データ要素基本情報
VERTEXBUFFER_INPUT_ELEMENT_INFO_BASE_LINUX g_VertexElementInfoBase[ LINUX_VERTEX_ATTR_NUM ] =
{
	{ "aPosF2",			2, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_POS_F2
	{ "aPos",			3, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_POS_F3
	{ "aPosF4",			4, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_POS_F4
	{ "aNrm",			3, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_NRM
	{ "aDif",			4, GL_UNSIGNED_BYTE,	GL_TRUE  },	// LINUX_VERTEX_ATTR_DIF
	{ "aSpc",			4, GL_UNSIGNED_BYTE,	GL_TRUE  },	// LINUX_VERTEX_ATTR_SPC
	{ "aTexUV0",		2, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV0_F2
	{ "aTexUV1",		2, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV1_F2
	{ "aTexUV2",		2, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV2_F2
	{ "aTexUV3",		2, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV3_F2
	{ "aTexUV4",		2, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV4_F2
	{ "aTexUV5",		2, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV5_F2
	{ "aTexUV6",		2, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV6_F2
	{ "aTexUV7",		2, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV7_F2
	{ "aTexUV0F4",		4, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV0_F4
	{ "aTexUV1F4",		4, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TEXUV1_F4
	{ "aTan",			3, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_TAN
	{ "aBin",			3, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_BIN
	{ "aBlendInd0",		4, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_BLENDIND0
	{ "aBlendWeight0",	4, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_BLENDWEIGHT0
	{ "aBlendInd1",		4, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_BLENDIND1
	{ "aBlendWeight1",	4, GL_FLOAT,			GL_FALSE },	// LINUX_VERTEX_ATTR_BLENDWEIGHT1
} ;

// StretchRect用頂点データの情報
VERTEXBUFFER_INPUT_INFO_LINUX	g_StretchRectVertexInputInfo =
{
	sizeof( float ) * 4,
	2,
	{
		{ LINUX_VERTEX_ATTR_POS_F2,    NULL, 0 },
		{ LINUX_VERTEX_ATTR_TEXUV0_F2, NULL, 8 },
	},
} ;
VERTEXBUFFER_INPUT_INFO_LINUX	g_StretchRectTex2VertexInputInfo =
{
	sizeof( float ) * 6,
	3,
	{
		{ LINUX_VERTEX_ATTR_POS_F2,    NULL,  0 },
		{ LINUX_VERTEX_ATTR_TEXUV0_F2, NULL,  8 },
		{ LINUX_VERTEX_ATTR_TEXUV1_F2, NULL, 16 },
	},
} ;
VERTEXBUFFER_INPUT_INFO_LINUX	g_StretchRectTex8VertexInputInfo =
{
	sizeof( float ) * 18,
	9,
	{
		{ LINUX_VERTEX_ATTR_POS_F2,    NULL,  0 },
		{ LINUX_VERTEX_ATTR_TEXUV0_F2, NULL,  8 },
		{ LINUX_VERTEX_ATTR_TEXUV1_F2, NULL, 16 },
		{ LINUX_VERTEX_ATTR_TEXUV2_F2, NULL, 24 },
		{ LINUX_VERTEX_ATTR_TEXUV3_F2, NULL, 32 },
		{ LINUX_VERTEX_ATTR_TEXUV4_F2, NULL, 40 },
		{ LINUX_VERTEX_ATTR_TEXUV5_F2, NULL, 48 },
		{ LINUX_VERTEX_ATTR_TEXUV6_F2, NULL, 56 },
		{ LINUX_VERTEX_ATTR_TEXUV7_F2, NULL, 64 },
	},
} ;

// マスク用頂点データの情報
VERTEXBUFFER_INPUT_INFO_LINUX	g_MaskVertexInputInfo =
{
	sizeof( float ) * 6,
	3,
	{
		{ LINUX_VERTEX_ATTR_POS_F2,    NULL,  0 },
		{ LINUX_VERTEX_ATTR_TEXUV0_F2, NULL,  8 },
		{ LINUX_VERTEX_ATTR_TEXUV1_F2, NULL, 16 },
	},
} ;

// クリア処理用頂点データの情報
VERTEXBUFFER_INPUT_INFO_LINUX	g_ClearRTVertexInputInfo =
{
	sizeof( float ) * 2,
	1,
	{
		{ LINUX_VERTEX_ATTR_POS_F2, NULL,  0 },
	},
} ;

// 標準描画用の複雑な処理を行わない頂点シェーダーの情報
static GRAPHICS_LINUX_BASE_SIMPLE_VERTEXSHADER_INFO g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NUM ] =
{
	// LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D( VERTEX_NOTEX_2D )
	{
		"Base_VERTEX_NOTEX_2D.vert",
		{
			sizeof( VERTEX_NOTEX_2D ),
			2,
			{
				{ LINUX_VERTEX_ATTR_POS_F4,		NULL,	0	},
				{ LINUX_VERTEX_ATTR_DIF,			NULL,	16	},
			},
		},
	},

	// LINUX_VERTEX_INPUTLAYOUT_2D( VERTEX2D or VERTEX_2D )
	{
		"Base_VERTEX2D.vert",
		{
			sizeof( VERTEX2D ),
			3,
			{
				{ LINUX_VERTEX_ATTR_POS_F4,		NULL,	0	},
				{ LINUX_VERTEX_ATTR_DIF,			NULL,	16	},
				{ LINUX_VERTEX_ATTR_TEXUV0_F2,	NULL,	20	},
			},
		},
	},

	// LINUX_VERTEX_INPUTLAYOUT_BLENDTEX_2D( VERTEX_BLENDTEX_2D or VERTEX2DSHADER )
	{
		"Base_VERTEX_BLENDTEX_2D.vert",
		{
			sizeof( VERTEX_BLENDTEX_2D ),
			4,
			{
				{ LINUX_VERTEX_ATTR_POS_F4,		NULL,	0	},
				{ LINUX_VERTEX_ATTR_DIF,			NULL,	16	},
				{ LINUX_VERTEX_ATTR_TEXUV0_F2,	NULL,	24	},
				{ LINUX_VERTEX_ATTR_TEXUV1_F2,	NULL,	32	},
			},
		},
	},

	// LINUX_VERTEX_INPUTLAYOUT_NOTEX_3D( VERTEX_NOTEX_3D )
	{
		"Base_VERTEX_NOTEX_3D.vert",
		{
			sizeof( VERTEX_NOTEX_3D ),
			2,
			{
				{ LINUX_VERTEX_ATTR_POS_F3,		NULL,	0	},
				{ LINUX_VERTEX_ATTR_DIF,			NULL,	12	},
			},
		},
	},

	// LINUX_VERTEX_INPUTLAYOUT_3D( VERTEX_3D )
	{
		"Base_VERTEX_3D.vert",
		{
			sizeof( VERTEX_3D ),
			3,
			{
				{ LINUX_VERTEX_ATTR_POS_F3,		NULL,	0	},
				{ LINUX_VERTEX_ATTR_DIF,			NULL,	12	},
				{ LINUX_VERTEX_ATTR_TEXUV0_F2,	NULL,	16	},
			},
		},
	},

	// LINUX_VERTEX_INPUTLAYOUT_3D_LIGHT( VERTEX3D )
	{
		"Base_VERTEX3D.vert",
		{
			sizeof( VERTEX3D ),
			6,
			{
				{ LINUX_VERTEX_ATTR_POS_F3,		NULL,	0	},
				{ LINUX_VERTEX_ATTR_NRM,			NULL,	12	},
				{ LINUX_VERTEX_ATTR_DIF,			NULL,	24	},
				{ LINUX_VERTEX_ATTR_SPC,			NULL,	28	},
				{ LINUX_VERTEX_ATTR_TEXUV0_F2,	NULL,	32	},
				{ LINUX_VERTEX_ATTR_TEXUV1_F2,	NULL,	40	},
			},
		},
	},

	// LINUX_VERTEX_INPUTLAYOUT_SHADER_3D( VERTEX3DSHADER )
	{
		"Base_VERTEX3DSHADER.vert",
		{
			sizeof( VERTEX3DSHADER ),
			9,
			{
				{ LINUX_VERTEX_ATTR_POS_F3,		NULL,	0	},
				{ LINUX_VERTEX_ATTR_POS_F4,		NULL,	12	},
				{ LINUX_VERTEX_ATTR_NRM,			NULL,	28	},
				{ LINUX_VERTEX_ATTR_TAN,			NULL,	40	},
				{ LINUX_VERTEX_ATTR_BIN,			NULL,	52	},
				{ LINUX_VERTEX_ATTR_DIF,			NULL,	64	},
				{ LINUX_VERTEX_ATTR_SPC,			NULL,	68	},
				{ LINUX_VERTEX_ATTR_TEXUV0_F2,	NULL,	72	},
				{ LINUX_VERTEX_ATTR_TEXUV1_F2,	NULL,	80	},
			},
		},
	},
} ;

// ピクセルフォーマット
PIXELFORMAT_INFO_LINUX g_LinuxPixelFormat[ LINUX_PIXEL_FORMAT_NUM ] =
{
	{ FALSE,	 32,	      0,	GL_RGBA,					GL_RGBA,		GL_UNSIGNED_BYTE			},	// LINUX_PIXEL_FORMAT_R8G8B8A8
	{ FALSE,	 24,	      0,	GL_RGB,						GL_RGB,			GL_UNSIGNED_BYTE			},	// LINUX_PIXEL_FORMAT_R8G8B8
	{ FALSE,	 16,	      0,	GL_RGBA,					GL_RGBA,		GL_UNSIGNED_SHORT_4_4_4_4	},	// LINUX_PIXEL_FORMAT_R4G4B4A4
	{ FALSE,	 16,	      0,	GL_RGBA,					GL_RGBA,		GL_UNSIGNED_SHORT_5_5_5_1	},	// LINUX_PIXEL_FORMAT_R5G5B5A1
	{ FALSE,	 16,	      0,	GL_RGB,						GL_RGB,			GL_UNSIGNED_SHORT_5_6_5		},	// LINUX_PIXEL_FORMAT_R5G6B5
	{ FALSE,	  8,	      0,	GL_LUMINANCE,				GL_LUMINANCE,	GL_UNSIGNED_BYTE			},	// LINUX_PIXEL_FORMAT_R8
	{ FALSE,	 48,	      0,	GL_RGB,						GL_RGB,			GL_HALF_FLOAT_OES			},	// LINUX_PIXEL_FORMAT_R16G16B16_FLOAT
	{ FALSE,	 96,	      0,	GL_RGB,						GL_RGB,			GL_FLOAT					},	// LINUX_PIXEL_FORMAT_R32G32B32_FLOAT
	{ FALSE,	 64,	      0,	GL_RGBA,					GL_RGBA,		GL_HALF_FLOAT_OES			},	// LINUX_PIXEL_FORMAT_R16G16B16A16_FLOAT
	{ FALSE,	128,	      0,	GL_RGBA,					GL_RGBA,		GL_FLOAT					},	// LINUX_PIXEL_FORMAT_R32G32B32A32_FLOAT
} ;

// Extension の名前
const char *g_LinuxGLExtensionName[ LINUX_GL_EXTENSION_NUM ] =
{
	"GL_OES_depth24",						// LINUX_GL_EXTENSION_DEPTH24						
	"GL_OES_depth32",						// LINUX_GL_EXTENSION_DEPTH32						
	"GL_OES_element_index_uint",			// LINUX_GL_EXTENSION_ELEMENT_INDEX_UNIT			
	"GL_OES_texture_float",					// LINUX_GL_EXTENSION_TEXTURE_FLOAT				
	"GL_OES_texture_float_linear",			// LINUX_GL_EXTENSION_TEXTURE_FLOAT_LINEAR		
	"GL_OES_compressed_paletted_texture",	// LINUX_GL_EXTENSION_COMPRESSED_PALETTED_TEXTURE	
	"GL_OES_compressed_ETC1_RGB8_texture",	// LINUX_GL_EXTENSION_COMPRESSED_ETC1_RGB8_TEXTURE
	"GL_OES_depth_texture",					// LINUX_GL_EXTENSION_DEPTH_TEXTURE				
	"GL_OES_texture_half_float",			// LINUX_GL_EXTENSION_TEXTURE_HALF_FLOAT			
	"GL_OES_texture_half_float_linear",		// LINUX_GL_EXTENSION_TEXTURE_HALF_FLOAT_LINEAR	
	"GL_OES_vertex_half_float",				// LINUX_GL_EXTENSION_VERTEX_HALF_FLOAT
	"GL_OES_texture_3D",					// LINUX_GL_EXTENSION_TEXTURE_3D
	"GL_EXT_texture_format_BGRA8888",		// LINUX_GL_EXTENSION_TEXTURE_FORMAT_BGRA8888
} ;

// Uniform の名前
const GLchar *g_UniformName[ LINUX_SHADER_UNIFORM_NUM ] =
{
	"uSrcTex",						// LINUX_SHADER_UNIFORM_SRC_TEX
	"uBlendTex",					// LINUX_SHADER_UNIFORM_BLEND_TEX
	"uDifMapTex",					// LINUX_SHADER_UNIFORM_DIF_MAP_TEX
	"uNrmMapTex",					// LINUX_SHADER_UNIFORM_NRM_MAP_TEX
	"uSpcMapTex",					// LINUX_SHADER_UNIFORM_SPC_MAP_TEX
	"uToonDifGradTex",				// LINUX_SHADER_UNIFORM_TOON_DIF_GRAD_TEX
	"uToonSpcGradTex",				// LINUX_SHADER_UNIFORM_TOON_SPC_GRAD_TEX
	"uToonSphereMapTex",			// LINUX_SHADER_UNIFORM_TOON_SPHERE_MAP_TEX
	"uToonRGBtoVMaxRGBVolTex",		// LINUX_SHADER_UNIFORM_TOON_RGB_TO_VMAXRGBVOL_TEX
	"uSubTex",						// LINUX_SHADER_UNIFORM_SUB_TEX
	"uShadowMap0Tex",				// LINUX_SHADER_UNIFORM_SHADOW_MAP0_TEX
	"uShadowMap1Tex",				// LINUX_SHADER_UNIFORM_SHADOW_MAP1_TEX
	"uShadowMap2Tex",				// LINUX_SHADER_UNIFORM_SHADOW_MAP2_TEX
	"uAmb_Emi",						// LINUX_SHADER_UNIFORM_AMB_EMI
	"uMatDif",						// LINUX_SHADER_UNIFORM_MAT_DIF
	"uMatSpc",						// LINUX_SHADER_UNIFORM_MAT_SPC
	"uMatPow",						// LINUX_SHADER_UNIFORM_MAT_POW
	"uMatTypeParam0",				// LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM0
	"uMatTypeParam1",				// LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM1
	"uMatTypeParam2",				// LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM2
	"uLightPos",					// LINUX_SHADER_UNIFORM_LIGHT_POS
	"uLightDir",					// LINUX_SHADER_UNIFORM_LIGHT_DIR
	"uLightDif",					// LINUX_SHADER_UNIFORM_LIGHT_DIF
	"uLightSpc",					// LINUX_SHADER_UNIFORM_LIGHT_SPC
	"uLightAmb",					// LINUX_SHADER_UNIFORM_LIGHT_AMB
	"uLightRange_FallOff_AT0_AT1",	// LINUX_SHADER_UNIFORM_LIGHT_RANGE_FALLOFF_AT0_AT1
	"uLightAT2_SpotP0_SpotP1",		// LINUX_SHADER_UNIFORM_LIGHT_AT2_SPOTP0_SPOTP1
	"uAntiVPMat",					// LINUX_SHADER_UNIFORM_ANTI_VIEW_PORT_MAT
	"uViewMat",						// LINUX_SHADER_UNIFORM_VIEW_MAT
	"uProjMat",						// LINUX_SHADER_UNIFORM_PROJ_MAT
	"uFog",							// LINUX_SHADER_UNIFORM_FOG
	"uToonOutLineSize",				// LINUX_SHADER_UNIFORM_TOON_OUTLINE_SIZE
	"uDifSrc_SpeSrc_MulSpeCol",		// LINUX_SHADER_UNIFORM_DIFSRC_SPCSRC_MULSPECOL
	"uSMLVPMat",					// LINUX_SHADER_UNIFORM_SM_LVP_MAT
	"uTexMat",						// LINUX_SHADER_UNIFORM_TEX_MAT
	"uLWMat",						// LINUX_SHADER_UNIFORM_LW_MAT
	"uMulAlphaColor",				// LINUX_SHADER_UNIFORM_MUL_ALPHA_COLOR
	"uATestRef",					// LINUX_SHADER_UNIFORM_ALPHA_TEST_REF
	"uATestCmpMode",				// LINUX_SHADER_UNIFORM_ALPHA_TEST_CMPMODE
	"uFogColor",					// LINUX_SHADER_UNIFORM_FOG_COLOR
	"uFactorColor",					// LINUX_SHADER_UNIFORM_FACTOR_COLOR
	"uIgnoreTextureColor",			// LINUX_SHADER_UNIFORM_IGNORE_TEXTURE_COLOR
	"uAddColor",					// LINUX_SHADER_UNIFORM_ADD_COLOR
	"uShadowMapLightEnable",		// LINUX_SHADER_UNIFORM_SHADOWMAP_LIGHT_ENABLE
	"uShadowMap_DAdj_Grad",			// LINUX_SHADER_UNIFORM_SHADOWMAP_DADJ_GRAD
} ;

GLenum g_TextureEnum[ USE_TEXTURESTAGE_NUM ] =
{
	GL_TEXTURE0,
	GL_TEXTURE1,
	GL_TEXTURE2,
	GL_TEXTURE3,
	GL_TEXTURE4,
	GL_TEXTURE5,
	GL_TEXTURE6,
	GL_TEXTURE7,
	GL_TEXTURE8,
	GL_TEXTURE9,
	GL_TEXTURE10,
	GL_TEXTURE11,
	GL_TEXTURE12,
	GL_TEXTURE13,
	GL_TEXTURE14,
	GL_TEXTURE15,
} ;

// Linux を使ったグラフィックス処理情報の構造体
GRAPHICS_HARDDATA_LINUX GraphicsHardDataLinux ;

// シェーダーバイナリ配列
extern BYTE DxShaderCodeBin_Model_LINUX[] ;
extern BYTE DxShaderCodeBin_Filter_LINUX[] ;
extern BYTE DxShaderCodeBin_Base_LINUX[] ;
extern BYTE DxShaderCodeBin_Base3D_LINUX[] ;
extern BYTE DxShaderCodeBin_RgbaMix_LINUX[] ;

// 関数宣言 -------------------------------------------------------------------

static int Graphics_Linux_DeviceState_SetNormalTextureAddressTransformMatrix_ConstBuffer( void ) ;	// 標準描画用のテクスチャ座標変換行列を定数バッファにセットする
static int Graphics_Linux_DeviceState_UpdateConstantFogParam( void ) ;								// フォグの色以外の定数情報を更新する
static void Graphics_Linux_DeviceState_RefreshAmbientAndEmissiveParam( void ) ;						// アンビエントライトとエミッシブカラーを掛け合わせたパラメータを更新する

// glTexSubImage2D の非同期実行用の関数
static int glTexSubImage2D_ASync(
	GLuint texture,
	GLenum target,
	GLint level,
	GLint xoffset, GLint yoffset,
	GLsizei width, GLsizei height,
	GLenum format, GLenum type,
	const GLvoid* pixels,
	int ASyncThread
) ;


// Linux に設定する射影行列を更新する
static void Graphics_Hardware_LINUX_RefreshProjectionMatrix( void ) ;

static int Graphics_Linux_BltBmpOrBaseImageToGraph3_MipMapBlt(
	      IMAGEDATA_ORIG	*Orig,
	const RECT				*SrcRect,
	const RECT				*DestRect,
	      GLuint			UseTex,
	      int				TexWidth,
	      int				TexHeight,
	const BASEIMAGE			*RgbBaseImage,
	const BASEIMAGE			*AlphaBaseImage,
	      int				DestColorFormat,
	      int				RedIsAlphaFlag,
	      int				UseTransColorConvAlpha,
	      int				ASyncThread
) ;

__inline	static DWORD GetDiffuseColor( void )															// 現在のディフューズカラーを得る
			{
				if( GSYS.HardInfo.UseVertexColorBGRAFormat )
				{
					return  ( DWORD )( ( GSYS.DrawSetting.DrawBright.Red   << 16 ) |
									   ( GSYS.DrawSetting.DrawBright.Green << 8  ) |
									   ( GSYS.DrawSetting.DrawBright.Blue        ) |
									   ( ( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? GSYS.DrawSetting.BlendParam : 255 ) << 24 ) ) ;
				}
				else
				{
					return  ( DWORD )( ( GSYS.DrawSetting.DrawBright.Red         ) |
									   ( GSYS.DrawSetting.DrawBright.Green << 8  ) |
									   ( GSYS.DrawSetting.DrawBright.Blue  << 16 ) |
									   ( ( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? GSYS.DrawSetting.BlendParam : 255 ) << 24 ) ) ;
				}
			}


// プログラム -----------------------------------------------------------------

// Linux を使用したグラフィックス処理の初期化を行う( 0:成功 -1:失敗 )
extern	int		Graphics_Linux_Initialize( void )
{
	// 画面モード変更ではサブバックバッファの作り直しだけ行う
	if( GSYS.Screen.Graphics_Screen_ChangeModeFlag )
	{
		// サブバックバッファの作成
		if( Graphics_Linux_SetupSubBackBuffer() < 0 )
		{
			return -1 ;
		}

		return 0 ;
	}

	// Linux のグラフィックス処理の主な初期化を行う
	if( Graphics_Linux_Device_Create() != 0 )
	{
		goto ERR ;
	}

	// シェーダーの作成
	if( Graphics_Linux_Shader_Initialize() != 0 )
	{
		goto ERR ;
	}

	GSYS.Setting.ValidHardware = GSYS.Setting.NotUseHardware ? FALSE : TRUE ;

	// 終了
	return 0 ;

	// エラー終了
ERR :

	// 後始末を行う
	Graphics_Linux_Terminate() ;

	return -1 ;
}

// Linux を使用したグラフィックス処理の後始末を行う
extern int Graphics_Linux_Terminate( void )
{
	// 画面モード変更ではサブバックバッファの削除だけ行う
	if( GSYS.Screen.Graphics_Screen_ChangeModeFlag )
	{
		Graphics_Linux_TerminateSubBackBuffer() ;

		return 0 ;
	}

	// 描画処理の終了
	Graphics_Linux_RenderEnd() ;

#ifndef DX_NON_MODEL
	// モデルの頂点バッファを解放
	MV1TerminateVertexBufferAll() ;
#endif // DX_NON_MODEL

	// テクスチャへ画像を転送するためのメモリの後始末を行う
	Graphics_Linux_Texture_TerminateCommonBuffer() ;

	// シェーダーの解放
	Graphics_Linux_Shader_Terminate() ;

	// Linux のグラフィック処理の後始末を行う
	Graphics_Linux_Device_Delete() ;

	// 終了
	return 0 ;
}

// すべての OpenGL ES 系オブジェクトを解放する
extern int Graphics_Linux_ReleaseObjectAll( void )
{
	int i ;
	SHADERHANDLEDATA                          *Shader ;
	VERTEXBUFFERHANDLEDATA                    *VertexBuffer ;
	INDEXBUFFERHANDLEDATA                     *IndexBuffer ;
	IMAGEDATA                                 *Image ;
	SHADOWMAPDATA                             *ShadowMap ;

	if( HandleManageArray[ DX_HANDLETYPE_GRAPH ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMax ; i ++ )
		{
			Image = ( IMAGEDATA * )HandleManageArray[ DX_HANDLETYPE_GRAPH ].Handle[ i ] ;
			if( Image == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( Image->HandleInfo.Handle ) ;
#endif

			if( Image->Orig == NULL ) continue ;
			Graphics_Hardware_ReleaseOrigTexture_PF( Image->Orig ) ;

#ifndef DX_NON_MOVIE
			if( Image->Orig->MovieHandle != -1 )
			{
				ReleaseMovieSurface( Image->Orig->MovieHandle ) ;
			}
#endif
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].AreaMax ; i ++ )
		{
			ShadowMap = ( SHADOWMAPDATA * )HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].Handle[ i ] ;
			if( ShadowMap == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( ShadowMap->HandleInfo.Handle ) ;
#endif

			Graphics_Hardware_ShadowMap_ReleaseTexture_PF( ShadowMap  ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMax ; i ++ )
		{
			VertexBuffer = ( VERTEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].Handle[ i ] ;
			if( VertexBuffer == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( VertexBuffer->HandleInfo.Handle ) ;
#endif

			Graphics_Hardware_VertexBuffer_Terminate_PF( VertexBuffer ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMax ; i ++ )
		{
			IndexBuffer = ( INDEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].Handle[ i ] ;
			if( IndexBuffer == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( IndexBuffer->HandleInfo.Handle ) ;
#endif

			Graphics_Hardware_IndexBuffer_Terminate_PF( IndexBuffer ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_SHADER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_SHADER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_SHADER ].AreaMax ; i ++ )
		{
			Shader = ( SHADERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_SHADER ].Handle[ i ] ;
			if( Shader == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( Shader->HandleInfo.Handle ) ;
#endif

			if( Shader->ShaderType == DX_SHADERTYPE_VERTEX )
			{
				Graphics_Linux_VertexShaderArray_Release( &Shader->PF->Shader, 1 ) ;
			}
			else
			{
				Graphics_Linux_FragmentShaderArray_Release( &Shader->PF->Shader, 1 ) ;
			}
		}
	}

	// デバイスセット情報を初期化
	_MEMSET( GLINUX.Device.State.SetTexture, 0, sizeof( GLINUX.Device.State.SetTexture ) ) ;
	GLINUX.Device.State.SetShader            = NULL ;
	GLINUX.Device.State.SetShader_Force		= NULL ;
	_MEMSET( GLINUX.Device.State.VertexAttrEnable, 0, sizeof( GLINUX.Device.State.VertexAttrEnable ) ) ;
	GLINUX.Device.State.SetVertexBuffer      = 0 ;
	GLINUX.Device.State.TargetFrameBuffer    = 0 ;

	// 終了
	return 0 ;
}

// すべての OpenGL ES 系オブジェクトを作成する
extern int Graphics_Linux_CreateObjectAll( void )
{
	int                                       i ;
	SHADERHANDLEDATA                          *Shader ;
	VERTEXBUFFERHANDLEDATA                    *VertexBuffer ;
	INDEXBUFFERHANDLEDATA                     *IndexBuffer ;
	IMAGEDATA                                 *Image ;
	SHADOWMAPDATA                             *ShadowMap ;

	if( HandleManageArray[ DX_HANDLETYPE_GRAPH ].InitializeFlag && HandleManageArray[ DX_HANDLETYPE_GRAPH ].Num > 0 )
	{
		DXST_LOGFILE_ADDUTF16LE( "\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x20\x00\x4f\x00\x70\x00\x65\x00\x6e\x00\x47\x00\x4c\x00\x20\x00\x45\x00\x53\x00\x20\x00\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x8d\x51\x5c\x4f\x10\x62\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"グラフィックハンドルの OpenGL ES オブジェクトの再作成.... " @*/ ) ;

		for( i = HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMax ; i ++ )
		{
			Image = ( IMAGEDATA * )HandleManageArray[ DX_HANDLETYPE_GRAPH ].Handle[ i ] ;

			if( Image == NULL ) continue ;
			if( Image->Orig == NULL ) continue ;
			if( Graphics_Hardware_CreateOrigTexture_PF( Image->Orig ) < 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x20\x00\x25\x00\x30\x00\x38\x00\x78\x00\x20\x00\x67\x30\x31\x59\x57\x65\x00\x30\xb5\x30\xa4\x30\xba\x30\x20\x00\x25\x00\x64\x00\x78\x00\x25\x00\x64\x00\x20\x00\x0a\x00\x00"/*@ L"グラフィックハンドル %08x で失敗　サイズ %dx%d \n" @*/, Image->HandleInfo.Handle, Image->WidthI, Image->HeightI )) ;
			}
		}

		DXST_LOGFILE_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"成功\n" @*/ ) ;
	}

	if( HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].InitializeFlag && HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].Num > 0 )
	{
		DXST_LOGFILE_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x20\x00\x4f\x00\x70\x00\x65\x00\x6e\x00\x47\x00\x4c\x00\x20\x00\x45\x00\x53\x00\x20\x00\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x8d\x51\x5c\x4f\x10\x62\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"シャドウマップハンドルの OpenGL ES オブジェクトの再作成.... " @*/ ) ;

		for( i = HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].AreaMax ; i ++ )
		{
			ShadowMap = ( SHADOWMAPDATA * )HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].Handle[ i ] ;

			if( ShadowMap == NULL ) continue ;
			Graphics_Hardware_ShadowMap_CreateTexture_PF( ShadowMap ) ;
		}

		DXST_LOGFILE_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"成功\n" @*/ ) ;
	}

	if( HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].InitializeFlag && HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].Num > 0 )
	{
		DXST_LOGFILE_ADDUTF16LE( "\x02\x98\xb9\x70\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x20\x00\x4f\x00\x70\x00\x65\x00\x6e\x00\x47\x00\x4c\x00\x20\x00\x45\x00\x53\x00\x20\x00\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x8d\x51\x5c\x4f\x10\x62\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"頂点バッファハンドルの OpenGL ES オブジェクトの再作成.... " @*/ ) ;

		for( i = HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMax ; i ++ )
		{
			VertexBuffer = ( VERTEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].Handle[ i ] ;
			if( VertexBuffer == NULL ) continue ;
			Graphics_Hardware_VertexBuffer_Create_PF( VertexBuffer ) ;
		}

		DXST_LOGFILE_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"成功\n" @*/ ) ;
	}

	if( HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].InitializeFlag && HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].Num > 0 )
	{
		DXST_LOGFILE_ADDUTF16LE( "\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x20\x00\x4f\x00\x70\x00\x65\x00\x6e\x00\x47\x00\x4c\x00\x20\x00\x45\x00\x53\x00\x20\x00\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x8d\x51\x5c\x4f\x10\x62\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"インデックスバッファハンドルの OpenGL ES オブジェクトの再作成.... " @*/ ) ;

		for( i = HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMax ; i ++ )
		{
			IndexBuffer = ( INDEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].Handle[ i ] ;
			if( IndexBuffer == NULL ) continue ;
			Graphics_Hardware_IndexBuffer_Create_PF( IndexBuffer ) ;
		}

		DXST_LOGFILE_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"成功\n" @*/ ) ;
	}

	if( HandleManageArray[ DX_HANDLETYPE_SHADER ].InitializeFlag && HandleManageArray[ DX_HANDLETYPE_SHADER ].Num > 0 )
	{
		DXST_LOGFILE_ADDUTF16LE( "\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x20\x00\x4f\x00\x70\x00\x65\x00\x6e\x00\x47\x00\x4c\x00\x20\x00\x45\x00\x53\x00\x20\x00\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x8d\x51\x5c\x4f\x10\x62\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"シェーダーハンドルの OpenGL ES オブジェクトの再作成.... " @*/ ) ;

		for( i = HandleManageArray[ DX_HANDLETYPE_SHADER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_SHADER ].AreaMax ; i ++ )
		{
			GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ShaderCodeInfo ;

			Shader = ( SHADERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_SHADER ].Handle[ i ] ;
			if( Shader == NULL ) continue ;

			ShaderCodeInfo.Binary = Shader->FunctionCode ;
			ShaderCodeInfo.Size   = Shader->FunctionCodeSize ;
			if( Shader->ShaderType == DX_SHADERTYPE_VERTEX )
			{
				Graphics_Linux_VertexShader_Create( &ShaderCodeInfo, &Shader->PF->Shader, 1 ) ;
			}
			else
			{
				Graphics_Linux_FragmentShader_Create( &ShaderCodeInfo, &Shader->PF->Shader, 1 ) ;
			}
		}

		DXST_LOGFILE_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"成功\n" @*/ ) ;
	}

	// 終了
	return 0 ;
}



































// シェーダーコード関係

// シェーダーコードパッケージからシェーダーコードバイナリのアドレスとサイズを取得する
static void Graphics_Linux_ShaderCodePackage_GetInfo( WORD **pSize, BYTE **pCode, GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO *DestInfo, int Num )
{
	WORD *Size ;
	BYTE *Code ;
	int i ;

	Size = *pSize ;
	Code = *pCode ;
	for( i = 0 ; i < Num ; i ++, Size ++, DestInfo ++ )
	{
		if( *Size != 0 )
		{
			DestInfo->Binary = ( void * )Code ;
			DestInfo->Size   = *Size ;
			Code            += *Size ;
		}
	}

	*pSize = Size ;
	*pCode = Code ;
}

// Linux の標準描画用のシェーダーコードの初期化を行う
extern int Graphics_Linux_ShaderCode_Base_Initialize( void )
{
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE *SCBASE = &GraphicsHardDataLinux.ShaderCode.Base ;
	int   Size = 0 ;
	BYTE  *ShaderAddr = NULL ;
	WORD  *SizeBuf    = NULL ;

	// すでに初期化されていたら何もしない
	if( SCBASE->BaseShaderInitializeFlag == TRUE )
	{
		return TRUE ;
	}

	// ライトインデックスリストの構築
	{
		int l0, l1, l2, l3, l4, l5, ind ;

		ind = 0 ;
		for( l0 = 0 ; l0 < 4 ; l0 ++ )
		{
			for( l1 = 0 ; l1 <= l0 ; l1 ++ )
			{
				for( l2 = 0 ; l2 <= l1 ; l2 ++ )
				{
					for( l3 = 0 ; l3 <= l2 ; l3 ++ )
					{
						for( l4 = 0 ; l4 <= l3 ; l4 ++ )
						{
							for( l5 = 0 ; l5 <= l4 ; l5 ++ )
							{
								SCBASE->LightIndexList84[ l0 ][ l1 ][ l2 ][ l3 ][ l4 ][ l5 ] = ( short )ind ;
								ind ++ ;
							}
						}
					}
				}
			}
		}

		ind = 0 ;
		for( l0 = 0 ; l0 < 4 ; l0 ++ )
		{
			for( l1 = 0 ; l1 <= l0 ; l1 ++ )
			{
				for( l2 = 0 ; l2 <= l1 ; l2 ++ )
				{
					SCBASE->LightIndexList20[ l0 ][ l1 ][ l2 ] = ( short )ind ;
					ind ++ ;
				}
			}
		}

		ind = 0 ;
		for( l0 = 0 ; l0 < 3 ; l0 ++ )
		{
			for( l1 = 0 ; l1 <= l0 ; l1 ++ )
			{
				for( l2 = 0 ; l2 <= l1 ; l2 ++ )
				{
					SCBASE->LightIndexList10[ l0 ][ l1 ][ l2 ] = ( short )ind ;
					ind ++ ;
				}
			}
		}
	}

#ifndef DX_NON_FILTER
	SCBASE->FilterShaderBinDxaImage = NULL ;
	SCBASE->RGBAMixS_ShaderPackImage = NULL ;
#endif // DX_NON_FILTER

	SCBASE->Base2DShaderPackageImage = NULL ;

	// 標準描画用シェーダーオブジェクトファイルＤＸＡを圧縮したデータを解凍する
	{
		Size = DXA_Decode( DxShaderCodeBin_Base_LINUX, NULL ) ;
		SCBASE->Base2DShaderPackageImage = DXALLOC( ( size_t )Size ) ;
		if( SCBASE->Base2DShaderPackageImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Base_LINUX, SCBASE->Base2DShaderPackageImage ) ;
	}

	// アドレスリストのセット
	{
		DWORD HeadSize ;

		HeadSize = sizeof( WORD ) * (
				sizeof( SCBASE->BaseSimple_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->BaseNoneTex_FS_Code              ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->BaseUseTex_FS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->MaskEffect_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->MaskEffect_FS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->MaskEffect_ReverseEffect_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->MaskEffect_UseGraphHandle_FS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->MaskEffect_UseGraphHandle_ReverseEffect_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->StretchRect_VS_Code              ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->StretchRectTex2_VS_Code          ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->StretchRectTex8_VS_Code          ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->StretchRect_FS_Code              ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->ClearRT_VS_Code                  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE->ClearRT_FS_Code                  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		HeadSize = ( HeadSize + 15 ) / 16 * 16 ;

		SizeBuf    = ( WORD * )SCBASE->Base2DShaderPackageImage ;
		ShaderAddr = ( BYTE * )SCBASE->Base2DShaderPackageImage + HeadSize ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE->BaseSimple_VS_Code,                sizeof( SCBASE->BaseSimple_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE->BaseNoneTex_FS_Code,               sizeof( SCBASE->BaseNoneTex_FS_Code              ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE->BaseUseTex_FS_Code,                sizeof( SCBASE->BaseUseTex_FS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&SCBASE->MaskEffect_VS_Code,               sizeof( SCBASE->MaskEffect_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&SCBASE->MaskEffect_FS_Code,               sizeof( SCBASE->MaskEffect_FS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&SCBASE->MaskEffect_ReverseEffect_FS_Code, sizeof( SCBASE->MaskEffect_ReverseEffect_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE->MaskEffect_UseGraphHandle_FS_Code,               sizeof( SCBASE->MaskEffect_UseGraphHandle_FS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE->MaskEffect_UseGraphHandle_ReverseEffect_FS_Code, sizeof( SCBASE->MaskEffect_UseGraphHandle_ReverseEffect_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&SCBASE->StretchRect_VS_Code,              sizeof( SCBASE->StretchRect_VS_Code              ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&SCBASE->StretchRectTex2_VS_Code,          sizeof( SCBASE->StretchRectTex2_VS_Code          ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&SCBASE->StretchRectTex8_VS_Code,          sizeof( SCBASE->StretchRectTex8_VS_Code          ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&SCBASE->StretchRect_FS_Code,              sizeof( SCBASE->StretchRect_FS_Code              ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&SCBASE->ClearRT_VS_Code,                  sizeof( SCBASE->ClearRT_VS_Code                  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&SCBASE->ClearRT_FS_Code,                  sizeof( SCBASE->ClearRT_FS_Code                  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
	}

#ifndef DX_NON_FILTER

	// RGBAMixシェーダーパック圧縮ファイルを展開する
	{
		Size = DXA_Decode( DxShaderCodeBin_RgbaMix_LINUX, NULL ) ;
		SCBASE->RGBAMixS_ShaderPackImage = DXALLOC( ( size_t )Size ) ;
		if( SCBASE->RGBAMixS_ShaderPackImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_RgbaMix_LINUX, SCBASE->RGBAMixS_ShaderPackImage ) ;

		// アドレスリストのセット
		SizeBuf    = ( WORD  * )SCBASE->RGBAMixS_ShaderPackImage ;
		ShaderAddr = ( BYTE  * )SCBASE->RGBAMixS_ShaderPackImage +
			sizeof( WORD ) * (
				sizeof( SCBASE->RGBAMixS_FS_Code )  / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE->RGBAMixS_FS_Code,  sizeof( SCBASE->RGBAMixS_FS_Code )  / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
	}

	// フィルターシェーダーオブジェクトファイルＤＸＡを圧縮したデータを解凍する
	{
		Size = DXA_Decode( DxShaderCodeBin_Filter_LINUX, NULL ) ;
		SCBASE->FilterShaderBinDxaImage = DXALLOC( ( size_t )Size ) ;
		if( SCBASE->FilterShaderBinDxaImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Filter_LINUX, SCBASE->FilterShaderBinDxaImage ) ;

		// ＤＸＡファイルをオープンする
		DXA_Initialize( &SCBASE->FilterShaderBinDxa ) ;
		if( DXA_OpenArchiveFromMem( &SCBASE->FilterShaderBinDxa, SCBASE->FilterShaderBinDxaImage, Size, FALSE, FALSE ) != 0 )
		{
			goto ERR ;
		}
	}

#endif // DX_NON_FILTER

	SCBASE->BaseShaderInitializeFlag = TRUE ;

	// 正常終了
	return TRUE ;

ERR :
	if( SCBASE->Base2DShaderPackageImage != NULL )
	{
		DXFREE( SCBASE->Base2DShaderPackageImage ) ;
		SCBASE->Base2DShaderPackageImage = NULL ;
	}

#ifndef DX_NON_FILTER
	if( SCBASE->FilterShaderBinDxaImage != NULL )
	{
		DXFREE( SCBASE->FilterShaderBinDxaImage ) ;
		SCBASE->FilterShaderBinDxaImage = NULL ;
	}

	if( SCBASE->RGBAMixS_ShaderPackImage != NULL )
	{
		DXFREE( SCBASE->RGBAMixS_ShaderPackImage ) ;
		SCBASE->RGBAMixS_ShaderPackImage = NULL ;
	}
#endif // DX_NON_FILTER

	return FALSE ;
}

// Linux の標準描画用のシェーダーコードの後始末を行う
extern int Graphics_Linux_ShaderCode_Base_Terminate( void )
{
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE *SCBASE = &GraphicsHardDataLinux.ShaderCode.Base ;

	// すでに後始末されていたら何もしない
	if( SCBASE->BaseShaderInitializeFlag == FALSE )
	{
		return TRUE ;
	}

	// 解凍したシェーダーを格納していたメモリの解放
	if( SCBASE->Base2DShaderPackageImage != NULL )
	{
		DXFREE( SCBASE->Base2DShaderPackageImage ) ;
		SCBASE->Base2DShaderPackageImage = NULL ;
	}

#ifndef DX_NON_FILTER

	// フィルターシェーダー用ＤＸＡの後始末
	DXA_Terminate( &SCBASE->FilterShaderBinDxa ) ;

	if( SCBASE->RGBAMixS_ShaderPackImage )
	{
		DXFREE( SCBASE->RGBAMixS_ShaderPackImage ) ;
		SCBASE->RGBAMixS_ShaderPackImage = NULL ;
	}
	if( SCBASE->FilterShaderBinDxaImage )
	{
		DXFREE( SCBASE->FilterShaderBinDxaImage ) ;
		SCBASE->FilterShaderBinDxaImage = NULL ;
	}

#endif // DX_NON_FILTER

	// 初期化フラグを倒す
	SCBASE->BaseShaderInitializeFlag = FALSE ;

	// 正常終了
	return TRUE ;
}

// Linux の標準３Ｄ描画用のシェーダーコードの初期化を行う
extern int Graphics_Linux_ShaderCode_Base3D_Initialize( void )
{
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE3D *SCBASE3D = &GraphicsHardDataLinux.ShaderCode.Base3D ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ DX_HANDLETYPE_GRAPH ] ;
	int   Size = 0 ;
	BYTE  *ShaderAddr = NULL ;
	WORD  *SizeBuf    = NULL ;

	// すでに初期化されていたら何もしない
	if( SCBASE3D->Base3DShaderInitializeFlag == TRUE )
	{
		return TRUE ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// クリティカルセクションを取得した上で改めて初期化確認
	if( SCBASE3D->Base3DShaderInitializeFlag == TRUE )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return TRUE ;
	}

	SCBASE3D->Base3DShaderPackageImage = NULL ;

	// 圧縮データの解凍
	{
		Size = DXA_Decode( DxShaderCodeBin_Base3D_LINUX, NULL ) ;
		SCBASE3D->Base3DShaderPackageImage = DXCALLOC( ( size_t )Size ) ;
		if( SCBASE3D->Base3DShaderPackageImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Base3D_LINUX, SCBASE3D->Base3DShaderPackageImage ) ;
	}

	// アドレスリストのセット
	{
		DWORD HeadSize ;

		HeadSize = sizeof( WORD ) * (
				sizeof( SCBASE3D->Base3D_PixelLighting_VS_Code         ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE3D->Base3D_PixelLighting_Normal_FS_Code  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE3D->Base3D_ShadowMap_VS_Code             ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE3D->Base3D_NoLighting_VS_Code            ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE3D->Base3D_VertexLighting_VS_Code        ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE3D->Base3D_ShadowMap_Normal_FS_Code      ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE3D->Base3D_NoLighting_Normal_FS_Code     ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
				sizeof( SCBASE3D->Base3D_VertexLighting_Normal_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		HeadSize = ( HeadSize + 15 ) / 16 * 16 ;

		SizeBuf    = ( WORD * )SCBASE3D->Base3DShaderPackageImage ;
		ShaderAddr = ( BYTE * )SCBASE3D->Base3DShaderPackageImage + HeadSize ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE3D->Base3D_PixelLighting_VS_Code,         sizeof( SCBASE3D->Base3D_PixelLighting_VS_Code         ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE3D->Base3D_PixelLighting_Normal_FS_Code,  sizeof( SCBASE3D->Base3D_PixelLighting_Normal_FS_Code  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE3D->Base3D_ShadowMap_VS_Code,             sizeof( SCBASE3D->Base3D_ShadowMap_VS_Code             ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE3D->Base3D_NoLighting_VS_Code,            sizeof( SCBASE3D->Base3D_NoLighting_VS_Code            ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE3D->Base3D_VertexLighting_VS_Code,        sizeof( SCBASE3D->Base3D_VertexLighting_VS_Code        ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE3D->Base3D_ShadowMap_Normal_FS_Code,      sizeof( SCBASE3D->Base3D_ShadowMap_Normal_FS_Code      ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE3D->Base3D_NoLighting_Normal_FS_Code,     sizeof( SCBASE3D->Base3D_NoLighting_Normal_FS_Code     ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCBASE3D->Base3D_VertexLighting_Normal_FS_Code, sizeof( SCBASE3D->Base3D_VertexLighting_Normal_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
	}

	SCBASE3D->Base3DShaderInitializeFlag = TRUE ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// 正常終了
	return TRUE ;

ERR :
	if( SCBASE3D->Base3DShaderPackageImage != NULL )
	{
		DXFREE( SCBASE3D->Base3DShaderPackageImage ) ;
		SCBASE3D->Base3DShaderPackageImage = NULL ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return FALSE ;
}


// Linux の標準３Ｄ描画用のシェーダーコードの後始末を行う
extern int Graphics_Linux_ShaderCode_Base3D_Terminate( void )
{
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE3D *SCBASE3D = &GraphicsHardDataLinux.ShaderCode.Base3D ;

	// すでに後始末されていたら何もしない
	if( SCBASE3D->Base3DShaderInitializeFlag == FALSE )
	{
		return TRUE ;
	}

	if( SCBASE3D->Base3DShaderPackageImage )
	{
		DXFREE( SCBASE3D->Base3DShaderPackageImage ) ;
		SCBASE3D->Base3DShaderPackageImage = NULL ;
	}

	// 初期化フラグを倒す
	SCBASE3D->Base3DShaderInitializeFlag = FALSE ;

	// 正常終了
	return TRUE ;
}


#ifndef DX_NON_MODEL

// Linux のモデル描画用のシェーダーコードの初期化を行う
extern	int		Graphics_Linux_ShaderCode_Model_Initialize( void )
{
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_MODEL *SCMODEL = &GraphicsHardDataLinux.ShaderCode.Model ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ DX_HANDLETYPE_GRAPH ] ;
	int   Size = 0 ;
	BYTE  *ShaderAddr = NULL ;
	WORD  *SizeBuf    = NULL ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// すでに初期化されていたら何もしない
	if( SCMODEL->ModelShaderInitializeFlag == TRUE )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return TRUE ;
	}

	SCMODEL->ModelShaderPackImage = NULL ;

	// 圧縮データの解凍
	{
		Size = DXA_Decode( DxShaderCodeBin_Model_LINUX, NULL ) ;
		SCMODEL->ModelShaderPackImage = DXCALLOC( ( size_t )Size ) ;
		if( SCMODEL->ModelShaderPackImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Model_LINUX, SCMODEL->ModelShaderPackImage ) ;
	}

	// アドレスリストのセット
	{
		DWORD HeadSize ;

		HeadSize = sizeof( WORD ) * (
			sizeof( SCMODEL->MV1_PixelLighting_VS_Code            ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_PixelLighting_ToonType1_FS_Code  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_PixelLighting_ToonType2_FS_Code  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_PixelLighting_Normal_FS_Code     ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_MaterialType_FS_Code             ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ToonOutLine_ShadowMap_VS_Code    ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ToonOutLine_VS_Code              ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ShadowMap_VS_Code                ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_NoLighting_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_VertexLighting_VS_Code           ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ShadowMap_Toon_FS_Code           ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ShadowMap_Normal_FS_Code         ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_NoLighting_Toon_FS_Code          ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_NoLighting_Normal_FS_Code        ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_VertexLighting_ToonType1_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_VertexLighting_ToonType2_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_VertexLighting_Normal_FS_Code    ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		HeadSize = ( HeadSize + 15 ) / 16 * 16 ;

		SizeBuf    = ( WORD * )SCMODEL->ModelShaderPackImage ;
		ShaderAddr = ( BYTE * )SCMODEL->ModelShaderPackImage + HeadSize ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_PixelLighting_VS_Code,            sizeof( SCMODEL->MV1_PixelLighting_VS_Code            ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_PixelLighting_ToonType1_FS_Code,  sizeof( SCMODEL->MV1_PixelLighting_ToonType1_FS_Code  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_PixelLighting_ToonType2_FS_Code,  sizeof( SCMODEL->MV1_PixelLighting_ToonType2_FS_Code  ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_PixelLighting_Normal_FS_Code,     sizeof( SCMODEL->MV1_PixelLighting_Normal_FS_Code     ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_MaterialType_FS_Code,             sizeof( SCMODEL->MV1_MaterialType_FS_Code             ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_ToonOutLine_ShadowMap_VS_Code,    sizeof( SCMODEL->MV1_ToonOutLine_ShadowMap_VS_Code    ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_ToonOutLine_VS_Code,              sizeof( SCMODEL->MV1_ToonOutLine_VS_Code              ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_ShadowMap_VS_Code,                sizeof( SCMODEL->MV1_ShadowMap_VS_Code                ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_NoLighting_VS_Code,               sizeof( SCMODEL->MV1_NoLighting_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_VertexLighting_VS_Code,           sizeof( SCMODEL->MV1_VertexLighting_VS_Code           ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_ShadowMap_Toon_FS_Code,           sizeof( SCMODEL->MV1_ShadowMap_Toon_FS_Code           ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_ShadowMap_Normal_FS_Code,         sizeof( SCMODEL->MV1_ShadowMap_Normal_FS_Code         ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_NoLighting_Toon_FS_Code,          sizeof( SCMODEL->MV1_NoLighting_Toon_FS_Code          ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_NoLighting_Normal_FS_Code,        sizeof( SCMODEL->MV1_NoLighting_Normal_FS_Code        ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_VertexLighting_ToonType1_FS_Code, sizeof( SCMODEL->MV1_VertexLighting_ToonType1_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_VertexLighting_ToonType2_FS_Code, sizeof( SCMODEL->MV1_VertexLighting_ToonType2_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		Graphics_Linux_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )SCMODEL->MV1_VertexLighting_Normal_FS_Code,    sizeof( SCMODEL->MV1_VertexLighting_Normal_FS_Code    ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
	}

	SCMODEL->ModelShaderInitializeFlag = TRUE ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// 正常終了
	return TRUE ;

ERR :
	if( SCMODEL->ModelShaderPackImage != NULL )
	{
		DXFREE( SCMODEL->ModelShaderPackImage ) ;
		SCMODEL->ModelShaderPackImage = NULL ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return FALSE ;
}

// Linux のモデル描画用のシェーダーコードの後始末を行う
extern	int		Graphics_Linux_ShaderCode_Model_Terminate( void )
{
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_MODEL *SCMODEL = &GraphicsHardDataLinux.ShaderCode.Model ;

	// すでに後始末されていたら何もしない
	if( SCMODEL->ModelShaderInitializeFlag == FALSE )
	{
		return TRUE ;
	}

	if( SCMODEL->ModelShaderPackImage )
	{
		DXFREE( SCMODEL->ModelShaderPackImage ) ;
		SCMODEL->ModelShaderPackImage = NULL ;
	}

	// 初期化フラグを倒す
	SCMODEL->ModelShaderInitializeFlag = FALSE ;

	// 正常終了
	return TRUE ;
}

#endif // DX_NON_MODEL



// 新しいシェーダー構造体を取得する
extern GRAPHICS_LINUX_SHADER *Graphics_Linux_GetShaderStruct( void )
{
	GRAPHICS_LINUX_SHADER_LIST *UseList ;

	if( GraphicsHardDataLinux.Device.Shader.ShadersListFirst == NULL &&
		GraphicsHardDataLinux.Device.Shader.ShadersListLast  == NULL )
	{
		GraphicsHardDataLinux.Device.Shader.ShadersListFirst = ( GRAPHICS_LINUX_SHADER_LIST * )DXALLOC( sizeof( GRAPHICS_LINUX_SHADER_LIST ) ) ;
		if( GraphicsHardDataLinux.Device.Shader.ShadersListFirst == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xc5\x60\x31\x58\xea\x30\xb9\x30\xc8\x30\x3c\x68\x0d\x7d\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"シェーダー情報リスト格納用のメモリの確保に失敗しました\n" @*/ )) ;
			return NULL ;
		}
		_MEMSET( GraphicsHardDataLinux.Device.Shader.ShadersListFirst, 0, sizeof( GRAPHICS_LINUX_SHADER_LIST ) ) ;
		GraphicsHardDataLinux.Device.Shader.ShadersListLast = GraphicsHardDataLinux.Device.Shader.ShadersListFirst ;

		UseList = GraphicsHardDataLinux.Device.Shader.ShadersListFirst ;
	}
	else
	{
		UseList = GraphicsHardDataLinux.Device.Shader.ShadersListLast ;
	}

	if( UseList->UseNum >= SHADERS_LIST_DATA_NUM )
	{
		GRAPHICS_LINUX_SHADER_LIST *NewList ;

		NewList = ( GRAPHICS_LINUX_SHADER_LIST * )DXALLOC( sizeof( GRAPHICS_LINUX_SHADER_LIST ) ) ;
		if( NewList == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xfd\x8f\xa0\x52\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xc5\x60\x31\x58\xea\x30\xb9\x30\xc8\x30\x3c\x68\x0d\x7d\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"追加のシェーダー情報リスト格納用のメモリの確保に失敗しました\n" @*/ )) ;
			return NULL ;
		}
		_MEMSET( NewList, 0, sizeof( GRAPHICS_LINUX_SHADER_LIST ) ) ;
		UseList->Next = NewList ;
		UseList = NewList ;

		GraphicsHardDataLinux.Device.Shader.ShadersListLast = NewList ;
	}

	UseList->UseNum ++ ;
	return &UseList->Shaders[ UseList->UseNum - 1 ] ;
}

// シェーダー情報リストの後始末を行う
extern int Graphics_Linux_ShaderList_Terminate( void )
{
	GRAPHICS_LINUX_SHADER_LIST *UseList ;

	if( GraphicsHardDataLinux.Device.Shader.ShadersListFirst == NULL &&
		GraphicsHardDataLinux.Device.Shader.ShadersListLast  == NULL )
	{
		return 0 ;
	}

	UseList = GraphicsHardDataLinux.Device.Shader.ShadersListFirst ;
	while( UseList != NULL )
	{
		GRAPHICS_LINUX_SHADER_LIST *Prev ;

		Graphics_Linux_ShaderArray_Release( UseList->Shaders, UseList->UseNum ) ;

		Prev = UseList ;
		UseList = UseList->Next ;
		DXFREE( Prev ) ;
		Prev = NULL ;
	}

	GraphicsHardDataLinux.Device.Shader.ShadersListFirst = NULL ;
	GraphicsHardDataLinux.Device.Shader.ShadersListLast  = NULL ;

	return 0 ;
}

// シェーダーコードから頂点シェーダーを作成する
extern int Graphics_Linux_VertexShader_Create( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO *Code, GLuint *pDestShader, int Num )
{
	int i ;

	for( i = 0; i < Num; i++, Code++, pDestShader++ )
	{
		GLuint shader ;

		shader = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( shader, 1, ( const GLchar ** )&Code->Binary, ( const GLint * )&Code->Size ); 
		glCompileShader( shader );

		GLint compiled;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );

		if( !compiled )
		{
			// コンパイルエラーの文字列の長さを取得
			GLint logLen;
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );

			// コンパイルエラー文字列を取得
			GLchar *log = ( GLchar * )DXALLOC( sizeof( GLchar ) * ( logLen + 1 ) );
			if( log != NULL )
			{
				glGetShaderInfoLog( shader, logLen, 0, log ) ;
				DXST_LOGFILE_ADDA( log ) ;
				DXST_LOGFILE_ADDA( "\n" ) ;
				DXFREE( log ) ;
				log = NULL ;
				return -1 + i ;
			}
		}

		*pDestShader = shader ;
	}

	return 0 ;
}

// シェーダーコードからフラグメントシェーダーを作成する
extern int Graphics_Linux_FragmentShader_Create( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO *Code, GLuint *pDestShader, int Num )
{
	int i ;

	for( i = 0; i < Num; i++, Code++, pDestShader++ )
	{
		GLuint shader;

		shader = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( shader, 1, ( const GLchar ** )&Code->Binary, ( const GLint * )&Code->Size ); 
		glCompileShader( shader );

		GLint compiled;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );

		if( !compiled )
		{
			// コンパイルエラーの文字列の長さを取得
			GLint logLen;
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );

			// コンパイルエラー文字列を取得
			GLchar *log = ( GLchar * )DXALLOC( sizeof( GLchar ) * ( logLen + 1 ) );
			if( log != NULL )
			{
				glGetShaderInfoLog( shader, logLen, 0, log );
				DXST_LOGFILE_ADDA( log ) ;
				DXST_LOGFILE_ADDA( "\n" ) ;
				DXFREE( log ) ;
				log = NULL ;
				return -1 + i ;
			}
		}

		*pDestShader = shader ;
	}

	return 0 ;
}

// シェーダーの Uniform 番号を取得する
extern GLuint Graphics_Linux_Shader_GetUniformIndex( GRAPHICS_LINUX_SHADER *ShaderInfo, const GLchar *UniformName )
{
	GLuint lResult ;

	lResult = glGetUniformLocation( ShaderInfo->Shader, UniformName ) ;
	if( lResult == 0xffffffff )
	{
		char lTempStr[ 128 ] ;

		CL_strcpy_s( DX_CHARCODEFORMAT_ASCII, lTempStr, sizeof( lTempStr ), UniformName ) ;
		CL_strcat_s( DX_CHARCODEFORMAT_ASCII, lTempStr, sizeof( lTempStr ), "[0]" ) ;
		lResult = glGetUniformLocation( ShaderInfo->Shader, lTempStr ) ;
	}

	return lResult ;
}

// 頂点シェーダーとフラグメントシェーダーからシェーダー情報をセットアップする
extern int Graphics_Linux_Shader_Create( GRAPHICS_LINUX_SHADER *ShaderInfo, GLuint VertexShader, GLuint FragmentShader )
{
	int i ;

	ShaderInfo->Shader = glCreateProgram() ;

    glAttachShader( ShaderInfo->Shader, VertexShader );
    glAttachShader( ShaderInfo->Shader, FragmentShader );

    glLinkProgram( ShaderInfo->Shader );

	for( i = 0 ; i < LINUX_VERTEX_ATTR_NUM ; i ++ )
	{
		ShaderInfo->AttributeIndex[ i ] = glGetAttribLocation( ShaderInfo->Shader, g_VertexElementInfoBase[ i ].AttrName ) ;
	}

	for( i = 0 ; i < LINUX_SHADER_UNIFORM_NUM ; i ++ )
	{
		ShaderInfo->UniformIndex[ i ] = Graphics_Linux_Shader_GetUniformIndex( ShaderInfo, g_UniformName[ i ] ) ;
	}

	return 0 ;
}

// 頂点シェーダー配列を解放する
extern void Graphics_Linux_VertexShaderArray_Release( GLuint *pObject, int Num )
{
	int i ;

	for( i = 0 ; i < Num ; i++, pObject++ )
	{
		if( *pObject != 0xffffffff && *pObject != 0 )
		{
			glDeleteShader( *pObject ) ;
			*pObject = 0 ;
		}
	}
}

// フラグメントシェーダー配列を解放する
extern void Graphics_Linux_FragmentShaderArray_Release( GLuint *pObject, int Num )
{
	int i ;

	for( i = 0 ; i < Num ; i++, pObject++ )
	{
		if( *pObject != 0xffffffff && *pObject != 0 )
		{
			glDeleteShader( *pObject ) ;
			*pObject = 0 ;
		}
	}
}

// シェーダー配列を解放する
extern void Graphics_Linux_ShaderArray_Release( GRAPHICS_LINUX_SHADER *pShaderInfo, int Num )
{
	int i ;

	for( i = 0 ; i < Num ; i++, pShaderInfo++ )
	{
		if( pShaderInfo->Shader != 0xffffffff && pShaderInfo->Shader != 0 )
		{
			glDeleteProgram( pShaderInfo->Shader ) ;
			pShaderInfo->Shader = 0 ;
		}
	}
}

// Linux のシェーダーの初期化を行う
extern int Graphics_Linux_Shader_Initialize( void )
{
	GRAPHICS_HARDWARE_LINUX_SHADERCODE      *ShaderCode = &GraphicsHardDataLinux.ShaderCode ;
	GRAPHICS_HARDWARE_LINUX_SHADER          *Shader     = &GraphicsHardDataLinux.Device.Shader ;
	int Result ;
	static GLuint MaskEffect_VS ;
	static GLuint MaskEffect_FS ;
	static GLuint MaskEffect_ReverseEffect_FS ;
	static GLuint MaskEffect_UseGraphHandle_FS[ 4 ] ;
	static GLuint MaskEffect_UseGraphHandle_ReverseEffect_FS[ 4 ] ;
	static GLuint StretchRect_FS ;
	static GLuint ClearRT_VS ;
	static GLuint ClearRT_FS ;
	int i ;
//	int j ;
//	int k ;
//	int l ;
//	int m ;
//	int n ;
	int FunctionResult = -1 ;

	// 既に作成されていたときのために削除処理を行う
	//TerminateLinuxShader() ;
	Graphics_Linux_Shader_Terminate() ;

//	return 0 ;

	DXST_LOGFILE_ADDUTF16LE( "\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\xa2\x95\xc2\x4f\x6e\x30\x1d\x52\x1f\x67\x16\x53\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"シェーダーコード関係の初期化.... " @*/ ) ;

	// Linux の場合はシェーダーは必ず使用できる
	GSYS.HardInfo.UseShader = TRUE ;

	// 標準描画用シェーダーコードが展開されていない場合は展開
	if( Graphics_Linux_ShaderCode_Base_Initialize() == FALSE )
	{
		DXST_LOGFILEFMT_ADDUTF16LE(( "\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\x92\x30\x55\x5c\x8b\x95\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"シェーダーコードを展開するメモリ領域の確保に失敗しました\n" @*/ )) ;
		GSYS.HardInfo.UseShader = FALSE ;
		goto END ;
	}

	// 基本的な描画処理に使用するシェーダーを作成する
	{
//		// 標準描画用の複雑な処理を行わない頂点シェーダーの作成
//		Result = Graphics_Linux_VertexShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )ShaderCode->Base.BaseSimple_VS_Code, Shader->Base.BaseSimple_VS, sizeof( ShaderCode->Base.BaseSimple_VS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
//		if( Result < 0 )
//		{
//			DXST_LOGFILEFMT_ADDUTF16LE(( "\x19\x6a\x96\x6e\xcf\x63\x3b\x75\x28\x75\x6e\x30\x07\x89\xd1\x96\x6a\x30\xe6\x51\x06\x74\x92\x30\x4c\x88\x8f\x30\x6a\x30\x44\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"標準描画用の複雑な処理を行わない頂点シェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
//			GSYS.HardInfo.UseShader = FALSE ;
//			goto END ;
//		}
//
//		// 固定機能パイプライン互換のフラグメントシェーダー( テクスチャなし )の作成
//		Result = Graphics_Linux_FragmentShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )ShaderCode->Base.BaseNoneTex_FS_Code, &Shader->Base.BaseNoneTex_FS[ 0 ][ 0 ], sizeof( ShaderCode->Base.BaseNoneTex_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
//		if( Result < 0 )
//		{
//			DXST_LOGFILEFMT_ADDUTF16LE(( "\xfa\x56\x9a\x5b\x5f\x6a\xfd\x80\xd1\x30\xa4\x30\xd7\x30\xe9\x30\xa4\x30\xf3\x30\x92\x4e\xdb\x63\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x28\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6a\x30\x57\x30\x20\x00\x29\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"固定機能パイプライン互換のフラグメントシェーダーオブジェクト( テクスチャなし ) No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
//			GSYS.HardInfo.UseShader = FALSE ;
//			goto END ;
//		}
//
//		// 固定機能パイプライン互換のシェーダー( テクスチャなし )の作成
//		// ( ここでは使用頻度の高いシェーダーのみ作成 )
//		for( i = 0 ; i < LINUX_VERTEX_INPUTLAYOUT_NUM ; i ++ )
//		{
//			if( i != LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D &&
//				i != LINUX_VERTEX_INPUTLAYOUT_NOTEX_3D )
//			{
//				continue ;
//			}
//
//			for( j = 0 ; j < LINUX_RENDER_TYPE_NUM ; j ++ )
//			{
//				if( j != LINUX_RENDER_TYPE_NORMAL &&
//					j != LINUX_RENDER_TYPE_PMA_NORMAL )
//				{
//					continue ;
//				}
//
//				for( k = 0 ; k < LINUX_FS_ALPHATEST_NUM ; k ++ )
//				{
//					Shader->Base.BaseNoneTex_Shader[ i ][ j ][ k ] = Graphics_Linux_GetShaderStruct() ;
//					if( Shader->Base.BaseNoneTex_Shader[ i ][ j ][ k ] == NULL )
//					{
//						GSYS.HardInfo.UseShader = FALSE ;
//						goto END ;
//					}
//					Result = Graphics_Linux_Shader_Create( Shader->Base.BaseNoneTex_Shader[ i ][ j ][ k ], Shader->Base.BaseSimple_VS[ i ], Shader->Base.BaseNoneTex_FS[ j ][ k ] ) ;
//					if( Result < 0 )
//					{
//						DXST_LOGFILEFMT_ADDUTF16LE(( "\xfa\x56\x9a\x5b\x5f\x6a\xfd\x80\xd1\x30\xa4\x30\xd7\x30\xe9\x30\xa4\x30\xf3\x30\x92\x4e\xdb\x63\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x28\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6a\x30\x57\x30\x20\x00\x29\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"固定機能パイプライン互換のシェーダーオブジェクト( テクスチャなし )の作成に失敗しました\n" @*/ ) ) ;
//						GSYS.HardInfo.UseShader = FALSE ;
//						goto END ;
//					}
//				}
//			}
//		}

		// 固定機能パイプライン互換のシェーダー( テクスチャあり )の作成
		// ( ここでは使用頻度の高いシェーダーのみ作成 )
//		for( i = 0 ; i < LINUX_VERTEX_INPUTLAYOUT_NUM ; i ++ )
//		{
//			if( i != LINUX_VERTEX_INPUTLAYOUT_2D &&
//				i != LINUX_VERTEX_INPUTLAYOUT_3D &&
//				i != LINUX_VERTEX_INPUTLAYOUT_3D_LIGHT )
//			{
//				continue ;
//			}
//
//			for( j = 0 ; j < 4 ; j ++ )
//			{
//				if( j != 0 )
//				{
//					continue ;
//				}
//
//				for( k = 0 ; k < LINUX_RENDER_TYPE_NUM ; k ++ )
//				{
//					if( k != LINUX_RENDER_TYPE_NORMAL &&
//						k != LINUX_RENDER_TYPE_PMA_NORMAL )
//					{
//						continue ;
//					}
//
//					for( l = 0 ; l < 2 ; l ++ )
//					{
//						if( l != 0 )
//						{
//							continue ;
//						}
//
//						for( m = 0 ; m < 2 ; m ++ )
//						{
//							for( n = 0 ; n < LINUX_FS_ALPHATEST_NUM ; n ++ )
//							{
//								Result = Graphics_Linux_FragmentShader_Create( &ShaderCode->Base.BaseUseTex_FS_Code[ j ][ k ][ l ][ m ][ n ], &Shader->Base.BaseUseTex_FS[ j ][ k ][ l ][ m ][ n ], 1 ) ;
//								if( Result < 0 )
//								{
//									DXST_LOGFILEFMT_ADDUTF16LE(( "\xfa\x56\x9a\x5b\x5f\x6a\xfd\x80\xd1\x30\xa4\x30\xd7\x30\xe9\x30\xa4\x30\xf3\x30\x92\x4e\xdb\x63\x6e\x30\xd5\x30\xe9\x30\xb0\x30\xe1\x30\xf3\x30\xc8\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x28\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x42\x30\x8a\x30\x20\x00\x29\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"固定機能パイプライン互換のフラグメントシェーダーオブジェクト( テクスチャあり ) の作成に失敗しました\n" @*/ )) ;
//									GSYS.HardInfo.UseShader = FALSE ;
//									goto END ;
//								}
//
//								Shader->Base.BaseUseTex_Shader[ i ][ j ][ k ][ l ][ m ][ n ] = Graphics_Linux_GetShaderStruct() ;
//								if( Shader->Base.BaseUseTex_Shader[ i ][ j ][ k ][ l ][ m ][ n ] == NULL )
//								{
//									GSYS.HardInfo.UseShader = FALSE ;
//									goto END ;
//								}
//								Result = Graphics_Linux_Shader_Create( Shader->Base.BaseUseTex_Shader[ i ][ j ][ k ][ l ][ m ][ n ], Shader->Base.BaseSimple_VS[ i ], Shader->Base.BaseUseTex_FS[ j ][ k ][ l ][ m ][ n ] ) ;
//								if( Result < 0 )
//								{
//									DXST_LOGFILEFMT_ADDUTF16LE(( "\xfa\x56\x9a\x5b\x5f\x6a\xfd\x80\xd1\x30\xa4\x30\xd7\x30\xe9\x30\xa4\x30\xf3\x30\x92\x4e\xdb\x63\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x28\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x42\x30\x8a\x30\x20\x00\x29\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"固定機能パイプライン互換のシェーダーオブジェクト( テクスチャあり )の作成に失敗しました\n" @*/ ) ) ;
//									GSYS.HardInfo.UseShader = FALSE ;
//									goto END ;
//								}
//							}
//						}
//					}
//				}
//			}
//		}

		// マスク処理用頂点シェーダーの作成
		Result = Graphics_Linux_VertexShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&ShaderCode->Base.MaskEffect_VS_Code, &MaskEffect_VS, sizeof( ShaderCode->Base.MaskEffect_VS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"マスク処理用の頂点シェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}

		// マスク処理用フラグメントシェーダーの作成
		Result = Graphics_Linux_FragmentShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&ShaderCode->Base.MaskEffect_FS_Code, &MaskEffect_FS, sizeof( ShaderCode->Base.MaskEffect_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"マスク処理用のフラグメントシェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
		Result = Graphics_Linux_FragmentShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&ShaderCode->Base.MaskEffect_ReverseEffect_FS_Code, &MaskEffect_ReverseEffect_FS, sizeof( ShaderCode->Base.MaskEffect_ReverseEffect_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xcd\x53\xe2\x8e\xb9\x52\x9c\x67\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"反転効果マスク処理用のフラグメントシェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
		Result = Graphics_Linux_FragmentShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )ShaderCode->Base.MaskEffect_UseGraphHandle_FS_Code, MaskEffect_UseGraphHandle_FS, sizeof( ShaderCode->Base.MaskEffect_UseGraphHandle_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"マスク処理用のフラグメントシェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
		Result = Graphics_Linux_FragmentShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )ShaderCode->Base.MaskEffect_UseGraphHandle_ReverseEffect_FS_Code, MaskEffect_UseGraphHandle_ReverseEffect_FS, sizeof( ShaderCode->Base.MaskEffect_UseGraphHandle_ReverseEffect_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xcd\x53\xe2\x8e\xb9\x52\x9c\x67\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"反転効果マスク処理用のフラグメントシェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}

		// マスク用シェーダーの作成
		Result = Graphics_Linux_Shader_Create( &Shader->Base.MaskEffect_Shader, MaskEffect_VS, MaskEffect_FS ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"マスク処理用のシェーダーオブジェクトの作成に失敗しました\n" @*/ ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
		Result = Graphics_Linux_Shader_Create( &Shader->Base.MaskEffect_ReverseEffect_Shader, MaskEffect_VS, MaskEffect_ReverseEffect_FS ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xcd\x53\xe2\x8e\xb9\x52\x9c\x67\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"反転効果マスク処理用のシェーダーオブジェクトの作成に失敗しました\n" @*/ ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
		for( i = 0 ; i < 4 ; i ++ )
		{
			Result = Graphics_Linux_Shader_Create( &Shader->Base.MaskEffect_UseGraphHandle_Shader[ i ], MaskEffect_VS, MaskEffect_UseGraphHandle_FS[ i ] ) ;
			if( Result < 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"マスク処理用のシェーダーオブジェクトの作成に失敗しました\n" @*/ ) ) ;
				GSYS.HardInfo.UseShader = FALSE ;
				goto END ;
			}
		}
		for( i = 0 ; i < 4 ; i ++ )
		{
			Result = Graphics_Linux_Shader_Create( &Shader->Base.MaskEffect_UseGraphHandle_ReverseEffect_Shader[ i ], MaskEffect_VS, MaskEffect_UseGraphHandle_ReverseEffect_FS[ i ] ) ;
			if( Result < 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\xcd\x53\xe2\x8e\xb9\x52\x9c\x67\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"反転効果マスク処理用のシェーダーオブジェクトの作成に失敗しました\n" @*/ ) ) ;
				GSYS.HardInfo.UseShader = FALSE ;
				goto END ;
			}
		}

		// 単純転送用頂点シェーダー・フラグメントシェーダーの作成
		Result = Graphics_Linux_VertexShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&ShaderCode->Base.StretchRect_VS_Code, &Shader->Base.StretchRect_VS, sizeof( ShaderCode->Base.StretchRect_VS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"単純転送用の頂点シェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
		Result = Graphics_Linux_VertexShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&ShaderCode->Base.StretchRectTex2_VS_Code, &Shader->Base.StretchRectTex2_VS, sizeof( ShaderCode->Base.StretchRectTex2_VS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x32\x00\x0b\x50\x6e\x30\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"テクスチャ座標2個の単純転送用の頂点シェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
		Result = Graphics_Linux_VertexShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&ShaderCode->Base.StretchRectTex8_VS_Code, &Shader->Base.StretchRectTex8_VS, sizeof( ShaderCode->Base.StretchRectTex8_VS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x38\x00\x0b\x50\x6e\x30\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"テクスチャ座標8個の単純転送用の頂点シェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
		Result = Graphics_Linux_FragmentShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&ShaderCode->Base.StretchRect_FS_Code, &StretchRect_FS, sizeof( ShaderCode->Base.StretchRect_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"単純転送用のフラグメントシェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}

		// 単純転送用シェーダーの作成
		Result = Graphics_Linux_Shader_Create( &Shader->Base.StretchRect_Shader, Shader->Base.StretchRect_VS, StretchRect_FS ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"単純転送用のシェーダーオブジェクトの作成に失敗しました\n" @*/ ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}

		// クリア処理用頂点シェーダー・フラグメントシェーダーの作成
		Result = Graphics_Linux_VertexShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&ShaderCode->Base.ClearRT_VS_Code, &ClearRT_VS, sizeof( ShaderCode->Base.ClearRT_VS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xaf\x30\xea\x30\xa2\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"クリア処理用の頂点シェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
		Result = Graphics_Linux_FragmentShader_Create( ( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO * )&ShaderCode->Base.ClearRT_FS_Code, &ClearRT_FS, sizeof( ShaderCode->Base.ClearRT_FS_Code ) / sizeof( GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xaf\x30\xea\x30\xa2\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"クリア処理用のフラグメントシェーダーオブジェクト No.%d の作成に失敗しました\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}

		// クリア処理用シェーダーの作成
		Result = Graphics_Linux_Shader_Create( &Shader->Base.ClearRT_Shader, ClearRT_VS, ClearRT_FS ) ;
		if( Result < 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xaf\x30\xea\x30\xa2\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"クリア処理用のシェーダーオブジェクトの作成に失敗しました\n" @*/ ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto END ;
		}
	}

	DXST_LOGFILE_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"成功\n" @*/ ) ;

	FunctionResult = 0 ;

END :

	// シェーダーの後始末
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )&MaskEffect_VS,								sizeof( MaskEffect_VS )									/ sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )&MaskEffect_FS,								sizeof( MaskEffect_FS )									/ sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )&MaskEffect_ReverseEffect_FS,					sizeof( MaskEffect_ReverseEffect_FS )					/ sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )MaskEffect_UseGraphHandle_FS,					sizeof( MaskEffect_UseGraphHandle_FS )					/ sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )MaskEffect_UseGraphHandle_ReverseEffect_FS,	sizeof( MaskEffect_UseGraphHandle_ReverseEffect_FS )	/ sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )&StretchRect_FS,								sizeof( StretchRect_FS )								/ sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )&ClearRT_VS,									sizeof( ClearRT_VS )									/ sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )&ClearRT_FS,									sizeof( ClearRT_FS )									/ sizeof( GLuint ) ) ;

	// 終了
	return FunctionResult ;
}


// Linux のシェーダーの後始末をする
extern int Graphics_Linux_Shader_Terminate( void )
{
	GRAPHICS_HARDDATA_LINUX_DEVICE *Device = &GraphicsHardDataLinux.Device ;
	GRAPHICS_HARDWARE_LINUX_SHADER *Shader = &Device->Shader ;

	Graphics_Linux_DeviceState_ResetShader() ;

	_MEMSET( Shader->Base.BaseNoneTex_Shader, 0, sizeof( Shader->Base.BaseNoneTex_Shader ) ) ;
	_MEMSET( Shader->Base.BaseUseTex_Shader,  0, sizeof( Shader->Base.BaseUseTex_Shader  ) ) ;
	Graphics_Linux_ShaderArray_Release( ( GRAPHICS_LINUX_SHADER * )&Shader->Base.MaskEffect_Shader,                             sizeof( Shader->Base.MaskEffect_Shader                              ) / sizeof( GRAPHICS_LINUX_SHADER ) ) ;
	Graphics_Linux_ShaderArray_Release( ( GRAPHICS_LINUX_SHADER * )&Shader->Base.MaskEffect_ReverseEffect_Shader,               sizeof( Shader->Base.MaskEffect_ReverseEffect_Shader                ) / sizeof( GRAPHICS_LINUX_SHADER ) ) ;
	Graphics_Linux_ShaderArray_Release( ( GRAPHICS_LINUX_SHADER * )Shader->Base.MaskEffect_UseGraphHandle_Shader,               sizeof( Shader->Base.MaskEffect_UseGraphHandle_Shader               ) / sizeof( GRAPHICS_LINUX_SHADER ) ) ;
	Graphics_Linux_ShaderArray_Release( ( GRAPHICS_LINUX_SHADER * )Shader->Base.MaskEffect_UseGraphHandle_ReverseEffect_Shader, sizeof( Shader->Base.MaskEffect_UseGraphHandle_ReverseEffect_Shader ) / sizeof( GRAPHICS_LINUX_SHADER ) ) ;
	Graphics_Linux_VertexShaderArray_Release( ( GLuint * )&Shader->Base.StretchRect_VS,                                           sizeof( Shader->Base.StretchRect_VS                                 ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release( ( GLuint * )&Shader->Base.StretchRectTex2_VS,                                       sizeof( Shader->Base.StretchRectTex2_VS                             ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release( ( GLuint * )&Shader->Base.StretchRectTex8_VS,                                       sizeof( Shader->Base.StretchRectTex8_VS                             ) / sizeof( GLuint ) ) ;
	Graphics_Linux_ShaderArray_Release( ( GRAPHICS_LINUX_SHADER * )&Shader->Base.StretchRect_Shader,                            sizeof( Shader->Base.StretchRect_Shader                             ) / sizeof( GRAPHICS_LINUX_SHADER ) ) ;
	Graphics_Linux_ShaderArray_Release( ( GRAPHICS_LINUX_SHADER * )&Shader->Base.ClearRT_Shader,                                sizeof( Shader->Base.ClearRT_Shader                                 ) / sizeof( GRAPHICS_LINUX_SHADER ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Base.BaseSimple_VS,											sizeof( Shader->Base.BaseSimple_VS									) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Base.BaseNoneTex_FS,											sizeof( Shader->Base.BaseNoneTex_FS									) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Base.BaseUseTex_FS,											sizeof( Shader->Base.BaseUseTex_FS									) / sizeof( GLuint ) ) ; 

	_MEMSET( Shader->Base3D.Base3D_PixelLighting_Normal_Shader,  0, sizeof( Shader->Base3D.Base3D_PixelLighting_Normal_Shader  ) ) ;
	_MEMSET( Shader->Base3D.Base3D_ShadowMap_Normal_Shader,      0, sizeof( Shader->Base3D.Base3D_ShadowMap_Normal_Shader      ) ) ;
	_MEMSET( Shader->Base3D.Base3D_NoLighting_Normal_Shader,     0, sizeof( Shader->Base3D.Base3D_NoLighting_Normal_Shader     ) ) ;
	_MEMSET( Shader->Base3D.Base3D_VertexLighting_Normal_Shader, 0, sizeof( Shader->Base3D.Base3D_VertexLighting_Normal_Shader ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Base3D.Base3D_PixelLighting_VS,                               sizeof( Shader->Base3D.Base3D_PixelLighting_VS             ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Base3D.Base3D_PixelLighting_Normal_FS,                        sizeof( Shader->Base3D.Base3D_PixelLighting_Normal_FS      ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Base3D.Base3D_ShadowMap_VS,                                   sizeof( Shader->Base3D.Base3D_ShadowMap_VS                 ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Base3D.Base3D_ShadowMap_Normal_FS,                            sizeof( Shader->Base3D.Base3D_ShadowMap_Normal_FS          ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Base3D.Base3D_NoLighting_VS,                                  sizeof( Shader->Base3D.Base3D_NoLighting_VS                ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Base3D.Base3D_NoLighting_Normal_FS,                           sizeof( Shader->Base3D.Base3D_NoLighting_Normal_FS         ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Base3D.Base3D_VertexLighting_VS,                              sizeof( Shader->Base3D.Base3D_VertexLighting_VS            ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Base3D.Base3D_VertexLighting_Normal_FS,                       sizeof( Shader->Base3D.Base3D_VertexLighting_Normal_FS     ) / sizeof( GLuint ) ) ;

#ifndef DX_NON_MODEL

	_MEMSET( Shader->Model.MV1_PixelLighting_ToonType1_Shader,  0, sizeof( Shader->Model.MV1_PixelLighting_ToonType1_Shader  ) ) ;
	_MEMSET( Shader->Model.MV1_PixelLighting_ToonType2_Shader,  0, sizeof( Shader->Model.MV1_PixelLighting_ToonType2_Shader  ) ) ;
	_MEMSET( Shader->Model.MV1_PixelLighting_Normal_Shader,     0, sizeof( Shader->Model.MV1_PixelLighting_Normal_Shader     ) ) ;
	_MEMSET( Shader->Model.MV1_MaterialType_Shader,             0, sizeof( Shader->Model.MV1_MaterialType_Shader             ) ) ;
	_MEMSET( Shader->Model.MV1_ToonOutLine_ShadowMap_Shader,    0, sizeof( Shader->Model.MV1_ToonOutLine_ShadowMap_Shader    ) ) ;
	_MEMSET( Shader->Model.MV1_ToonOutLine_Shader,              0, sizeof( Shader->Model.MV1_ToonOutLine_Shader              ) ) ;
	_MEMSET( Shader->Model.MV1_ShadowMap_Normal_Shader,         0, sizeof( Shader->Model.MV1_ShadowMap_Normal_Shader         ) ) ;
	_MEMSET( Shader->Model.MV1_ShadowMap_Toon_Shader,           0, sizeof( Shader->Model.MV1_ShadowMap_Toon_Shader           ) ) ;
	_MEMSET( Shader->Model.MV1_NoLighting_Toon_Shader,          0, sizeof( Shader->Model.MV1_NoLighting_Toon_Shader          ) ) ;
	_MEMSET( Shader->Model.MV1_NoLighting_Normal_Shader,        0, sizeof( Shader->Model.MV1_NoLighting_Normal_Shader        ) ) ;
	_MEMSET( Shader->Model.MV1_VertexLighting_ToonType1_Shader, 0, sizeof( Shader->Model.MV1_VertexLighting_ToonType1_Shader ) ) ;
	_MEMSET( Shader->Model.MV1_VertexLighting_ToonType2_Shader, 0, sizeof( Shader->Model.MV1_VertexLighting_ToonType2_Shader ) ) ;
	_MEMSET( Shader->Model.MV1_VertexLighting_Normal_Shader,    0, sizeof( Shader->Model.MV1_VertexLighting_Normal_Shader    ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Model.MV1_PixelLighting_VS,                                 sizeof( Shader->Model.MV1_PixelLighting_VS                ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_PixelLighting_ToonType1_FS,                       sizeof( Shader->Model.MV1_PixelLighting_ToonType1_FS      ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_PixelLighting_ToonType2_FS,                       sizeof( Shader->Model.MV1_PixelLighting_ToonType2_FS      ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_PixelLighting_Normal_FS,                          sizeof( Shader->Model.MV1_PixelLighting_Normal_FS         ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_MaterialType_FS,                                  sizeof( Shader->Model.MV1_MaterialType_FS                 ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Model.MV1_ToonOutLine_ShadowMap_VS,                         sizeof( Shader->Model.MV1_ToonOutLine_ShadowMap_VS        ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Model.MV1_ToonOutLine_VS,                                   sizeof( Shader->Model.MV1_ToonOutLine_VS                  ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Model.MV1_ShadowMap_VS,                                     sizeof( Shader->Model.MV1_ShadowMap_VS                    ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_ShadowMap_Normal_FS,                              sizeof( Shader->Model.MV1_ShadowMap_Normal_FS             ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_ShadowMap_Toon_FS,                                sizeof( Shader->Model.MV1_ShadowMap_Toon_FS               ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Model.MV1_NoLighting_VS,                                    sizeof( Shader->Model.MV1_NoLighting_VS                   ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_NoLighting_Toon_FS,                               sizeof( Shader->Model.MV1_NoLighting_Toon_FS              ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_NoLighting_Normal_FS,                             sizeof( Shader->Model.MV1_NoLighting_Normal_FS            ) / sizeof( GLuint ) ) ;
	Graphics_Linux_VertexShaderArray_Release(   ( GLuint * )Shader->Model.MV1_VertexLighting_VS,                                sizeof( Shader->Model.MV1_VertexLighting_VS               ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_VertexLighting_ToonType1_FS,                      sizeof( Shader->Model.MV1_VertexLighting_ToonType1_FS     ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_VertexLighting_ToonType2_FS,                      sizeof( Shader->Model.MV1_VertexLighting_ToonType2_FS     ) / sizeof( GLuint ) ) ;
	Graphics_Linux_FragmentShaderArray_Release( ( GLuint * )Shader->Model.MV1_VertexLighting_Normal_FS,                         sizeof( Shader->Model.MV1_VertexLighting_Normal_FS        ) / sizeof( GLuint ) ) ;

#endif // DX_NON_MODEL

	Graphics_Linux_ShaderList_Terminate() ;

#ifndef DX_NON_FILTER
	GraphFilter_Linux_ReleaseShaderAll() ;
#endif // DX_NON_FILTER

#ifndef DX_NON_LIVE2D_CUBISM4
	Live2D_Cubism4_Linux_ReleaseShaderAll() ;
#endif // DX_NON_LIVE2D_CUBISM4

	// 正常終了
	return 0 ;
}

// ３Ｄ標準描画の指定の頂点用の描画用シェーダーをセットアップする
extern int Graphics_Linux_Shader_Normal3DDraw_Setup( void )
{
	GRAPHICS_HARDWARE_LINUX_SHADER_BASE3D		*SB3D  = &GLINUX.Device.Shader.Base3D ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE3D	*SCB3D = &GLINUX.ShaderCode.Base3D ;
	int											ValidPL ;
	GRAPHICS_LINUX_SHADER						**Shader_PL		= NULL ;
	GLuint										*VS_PL			= NULL ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO	*VSAddress_PL	= NULL ;
	GLuint										*FS_PL			= NULL ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO	*FSAddress_PL	= NULL ;
	GRAPHICS_LINUX_SHADER						**Shader		= NULL ;
	GLuint										*VS				= NULL ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO	*VSAddress		= NULL ;
	GLuint										*FS				= NULL ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO	*FSAddress		= NULL ;
	int											ShadowMap ;
	int											IgnoreTextureAlpha ;
	int											FogType ;
	int											LightIndex84 ;
	int											LightIndex20 ;
	int											LightIndex10 ;
	int											AlphaTestMode ;
	int											BumpMap ;
	int											RenderType ;
	int											NextBlendMode ;

	if( GLINUX.ShaderCode.Base3D.Base3DShaderInitializeFlag == FALSE &&
		Graphics_Linux_ShaderCode_Base3D_Initialize() == FALSE )
	{
		return FALSE ;
	}

	// テクスチャアルファを無視するかどうかをセット
	IgnoreTextureAlpha = 1 ;
	if( GLINUX.Device.DrawSetting.AlphaChannelValidFlag ||
		GLINUX.Device.DrawSetting.AlphaTestValidFlag ||
		GLINUX.Device.State.DepthEnable )
	{
		IgnoreTextureAlpha = 0 ;
	}
	if( IgnoreTextureAlpha != GLINUX.Device.DrawSetting.IgnoreGraphAlphaFlag )
	{
		Graphics_Linux_DrawSetting_SetIgnoreDrawGraphAlpha( IgnoreTextureAlpha ) ;
	}

	// ブレンドモードの決定
	{
		NextBlendMode = GLINUX.Device.DrawSetting.BlendMode ;
		switch( GLINUX.Device.DrawSetting.BlendMode )
		{
		case DX_BLENDMODE_SUB :
			// 減算ブレンドの場合は14番目のブレンドモードを使用する
			NextBlendMode = DX_BLENDMODE_SUB1 ;
			break ;

		case DX_BLENDMODE_NOBLEND :
			// 描画先にα値がある場合は DX_BLENDMODE_NOBLEND でもブレンドモードは DX_BLENDMODE_SRCCOLOR にする
			if( GSYS.DrawSetting.AlphaChDrawMode )
			{
				NextBlendMode = DX_BLENDMODE_SRCCOLOR ;
			}

			// 描画元画像にαチャンネルがある場合やブレンド画像の有無など条件次第で DX_BLENDMODE_ALPHA を使用する
			if( GLINUX.Device.DrawSetting.RenderTexture != NULL )
			{
				if( GLINUX.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
				{
					NextBlendMode = DX_BLENDMODE_ALPHA ;
				}
			}
			break ;
		}
	}

	ShadowMap       = GSYS.DrawSetting.UseShadowMapNum != 0 ? 1 : 0 ;
	FogType         = GLINUX.Device.State.FogEnable ? GLINUX.Device.State.FogMode : DX_FOGMODE_NONE ;
	RenderType = g_DefaultBlendDescArray[ NextBlendMode ].RenderType ;
	AlphaTestMode   = GLINUX.Device.State.AlphaTestModeShaderIndex ;
	BumpMap         = 0 ;

	if( GSYS.DrawSetting.ShadowMapDraw )
	{
		// シャドウマップへの描画

		VS           =  &SB3D->Base3D_ShadowMap_VS     [ BumpMap ] ;
		VSAddress    = &SCB3D->Base3D_ShadowMap_VS_Code[ BumpMap ] ;

		FS           =  &SB3D->Base3D_ShadowMap_Normal_FS     [ AlphaTestMode ] ;
		FSAddress    = &SCB3D->Base3D_ShadowMap_Normal_FS_Code[ AlphaTestMode ] ;

		Shader       = &SB3D->Base3D_ShadowMap_Normal_Shader[ BumpMap ][ AlphaTestMode ] ;
	}
	else
	if( GLINUX.Device.State.Lighting )
	{
		// ライティングあり描画

		int LightMode[ DX_PIXELLIGHTING_LIGHT_NUM ] ;
		int i ;

		for( i = 0 ; i < DX_PIXELLIGHTING_LIGHT_NUM ; i ++ )
		{
			LightMode[ i ] = GLINUX.Device.State.LightEnableFlag[ i ] ? GLINUX.Device.State.LightParam[ i ].LightType : 0 ;
		}

		LightIndex84 = GLINUX.ShaderCode.Base.LightIndexList84[ LightMode[ 0 ] ][ LightMode[ 1 ] ][ LightMode[ 2 ] ][ LightMode[ 3 ] ][ LightMode[ 4 ] ][ LightMode[ 5 ] ] ;
		LightIndex20 = GLINUX.ShaderCode.Base.LightIndexList20[ LightMode[ 0 ] ][ LightMode[ 1 ] ][ LightMode[ 2 ] ] ;
		LightIndex10 = GLINUX.ShaderCode.Base.LightIndexList10
				[ LightMode[ 0 ] == DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ? 2 : ( LightMode[ 0 ] ? 1 : 0 ) ]
				[ LightMode[ 1 ] == DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ? 2 : ( LightMode[ 1 ] ? 1 : 0 ) ]
				[ LightMode[ 2 ] == DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ? 2 : ( LightMode[ 2 ] ? 1 : 0 ) ] ;

		VS_PL        =  &SB3D->Base3D_PixelLighting_VS     [ ShadowMap ][ FogType ] ;
		VSAddress_PL = &SCB3D->Base3D_PixelLighting_VS_Code[ ShadowMap ][ FogType ] ;

		FS_PL        =  &SB3D->Base3D_PixelLighting_Normal_FS     [ ShadowMap ][ LightIndex84 ][ RenderType ][ AlphaTestMode ] ;
		FSAddress_PL = &SCB3D->Base3D_PixelLighting_Normal_FS_Code[ ShadowMap ][ LightIndex84 ][ RenderType ][ AlphaTestMode ] ;

		Shader_PL    =  &SB3D->Base3D_PixelLighting_Normal_Shader[ ShadowMap ][ FogType ][ LightIndex84 ][ RenderType ][ AlphaTestMode ] ;

		VS           =  &SB3D->Base3D_VertexLighting_VS     [ ShadowMap ][ FogType ][ LightIndex20 ] ;
		VSAddress    = &SCB3D->Base3D_VertexLighting_VS_Code[ ShadowMap ][ FogType ][ LightIndex20 ] ;

		FS           =  &SB3D->Base3D_VertexLighting_Normal_FS     [ ShadowMap ][ LightIndex10 ][ RenderType ][ AlphaTestMode ] ;
		FSAddress    = &SCB3D->Base3D_VertexLighting_Normal_FS_Code[ ShadowMap ][ LightIndex10 ][ RenderType ][ AlphaTestMode ] ;

		Shader       =  &SB3D->Base3D_VertexLighting_Normal_Shader[ ShadowMap ][ FogType ][ LightIndex20 ][ LightIndex10 ][ RenderType ][ AlphaTestMode ] ;
	}
	else
	{
		// ライティングなし描画

		VS           =  &SB3D->Base3D_NoLighting_VS     [ FogType ] ;
		VSAddress    = &SCB3D->Base3D_NoLighting_VS_Code[ FogType ] ;

		FS           =  &SB3D->Base3D_NoLighting_Normal_FS     [ RenderType ][ AlphaTestMode ] ;
		FSAddress    = &SCB3D->Base3D_NoLighting_Normal_FS_Code[ RenderType ][ AlphaTestMode ] ;

		Shader       =  &SB3D->Base3D_NoLighting_Normal_Shader[ FogType ][ RenderType ][ AlphaTestMode ] ;
	}

	ValidPL = 
		VSAddress_PL != NULL && VSAddress_PL->Binary != NULL &&
		FSAddress_PL != NULL && FSAddress_PL->Binary != NULL ;

	if( ( GSYS.Light.EnableNum > DX_VERTEXLIGHTING_LIGHT_NUM || GLINUX.UsePixelLightingShader ) && ValidPL )
	{
		// 頂点シェーダーがあるかどうかを調べる
		if( *VS_PL == 0 )
		{
			// 頂点シェーダーの作成を試みる
			if( Graphics_Linux_VertexShader_Create( VSAddress_PL, VS_PL, 1 ) != 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x41\x00\x6e\x00\x64\x00\x72\x00\x6f\x00\x69\x00\x64\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xe9\x30\xa4\x30\xc6\x30\xa3\x30\xf3\x30\xb0\x30\x28\x75\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_Linux_Shader_Normal3DDraw_Setup でピクセルライティング用頂点シェーダーの作成に失敗しました\n" @*/ )) ;
				return FALSE ;
			}
		}

		// フラグメントシェーダーがあるかどうかを調べる
		if( *FS_PL == 0 )
		{
			// フラグメントシェーダーの作成を試みる
			if( Graphics_Linux_FragmentShader_Create( FSAddress_PL, FS_PL, 1 ) != 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x41\x00\x6e\x00\x64\x00\x72\x00\x6f\x00\x69\x00\x64\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xe9\x30\xa4\x30\xc6\x30\xa3\x30\xf3\x30\xb0\x30\x28\x75\xd5\x30\xe9\x30\xb0\x30\xe1\x30\xf3\x30\xc8\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_Linux_Shader_Normal3DDraw_Setup でピクセルライティング用フラグメントシェーダーの作成に失敗しました\n" @*/ )) ;
				return FALSE ;
			}
		}

		// シェーダーがあるかどうかを調べる
		if( *Shader_PL == NULL )
		{
			*Shader_PL = Graphics_Linux_GetShaderStruct() ;
			if( *Shader_PL == NULL )
			{
				return FALSE ;
			}
		}
		if( ( *Shader_PL )->Shader == 0 )
		{
			// シェーダーの作成を試みる
			if( Graphics_Linux_Shader_Create( *Shader_PL, *VS_PL, *FS_PL ) != 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x41\x00\x6e\x00\x64\x00\x72\x00\x6f\x00\x69\x00\x64\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xe9\x30\xa4\x30\xc6\x30\xa3\x30\xf3\x30\xb0\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_Linux_Shader_Normal3DDraw_Setup でピクセルライティング用シェーダーの作成に失敗しました\n" @*/ )) ;
				return FALSE ;
			}
		}

		// シェーダーのセットアップ
		Graphics_Linux_DeviceState_SetShader( *Shader_PL, FALSE ) ;
	}
	else
	{
		// シェーダーがあるかどうかを調べる
		if( *VS == 0 )
		{
			// シェーダーの作成を試みる
			if( Graphics_Linux_VertexShader_Create( VSAddress, VS, 1 ) != 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x41\x00\x6e\x00\x64\x00\x72\x00\x6f\x00\x69\x00\x64\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\x02\x98\xb9\x70\xe9\x30\xa4\x30\xc6\x30\xa3\x30\xf3\x30\xb0\x30\x28\x75\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_Linux_Shader_Normal3DDraw_Setup で頂点ライティング用頂点シェーダーの作成に失敗しました\n" @*/ )) ;
				return FALSE ;
			}
		}


		// シェーダーがあるかどうかを調べる
		if( *FS == 0 )
		{
			// シェーダーの作成を試みる
			if( Graphics_Linux_FragmentShader_Create( FSAddress, FS, 1 ) != 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x41\x00\x6e\x00\x64\x00\x72\x00\x6f\x00\x69\x00\x64\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\x02\x98\xb9\x70\xe9\x30\xa4\x30\xc6\x30\xa3\x30\xf3\x30\xb0\x30\x28\x75\xd5\x30\xe9\x30\xb0\x30\xe1\x30\xf3\x30\xc8\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_Linux_Shader_Normal3DDraw_Setup で頂点ライティング用フラグメントシェーダーの作成に失敗しました\n" @*/ )) ;
				return FALSE ;
			}
		}

		// シェーダーがあるかどうかを調べる
		if( *Shader == NULL )
		{
			*Shader = Graphics_Linux_GetShaderStruct() ;
			if( *Shader == NULL )
			{
				return FALSE ;
			}
		}
		if( ( *Shader )->Shader == 0 )
		{
			// シェーダーの作成を試みる
			if( Graphics_Linux_Shader_Create( *Shader, *VS, *FS ) != 0 )
			{
				DXST_LOGFILEFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x41\x00\x6e\x00\x64\x00\x72\x00\x6f\x00\x69\x00\x64\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\x02\x98\xb9\x70\xe9\x30\xa4\x30\xc6\x30\xa3\x30\xf3\x30\xb0\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_Linux_Shader_Normal3DDraw_Setup で頂点ライティング用シェーダーの作成に失敗しました\n" @*/ )) ;
				return FALSE ;
			}
		}

		// シェーダーのセットアップ
		Graphics_Linux_DeviceState_SetShader( *Shader, FALSE ) ;
	}

	// 終了
	return TRUE ;
}

// アルファテストの比較モードからフラグメントシェーダーのアルファテストモード( LINUX_FS_ALPHATEST_CMP_GREATER 等 )を取得する
extern int Graphics_Linux_Shader_GetAlphaTestModeIndex( int AlphaTestEnable, int AlphaTestMode /* DX_CMP_NEVER 等 */ )
{
	if( AlphaTestEnable == FALSE )
	{
		return LINUX_FS_ALPHATEST_CMP_GREATER ;
	}

	switch( AlphaTestMode )
	{
	default:
		return LINUX_FS_ALPHATEST_CMP_OTHER ;

	case DX_CMP_GREATER :		// DrawAlpha >  TestParam
		return LINUX_FS_ALPHATEST_CMP_GREATER ;
	}
}
































// Linux の画面関係

// ScreenCopy や GetDrawScreen を実現するために使用するテンポラリバッファの作成( 0:成功  -1:失敗 )
extern	int		Graphics_Linux_SetupSubBackBuffer( void )
{
	GLINUX.Device.Screen.SubBackBufferTextureSizeX = GSYS.Screen.MainScreenSizeX ;
	GLINUX.Device.Screen.SubBackBufferTextureSizeY = GSYS.Screen.MainScreenSizeY ;

	// ピクセルタイプの決定
	if( GSYS.Screen.UserScreenImage != NULL )
	{
		DWORD i ;
		DWORD PixelNum = GLINUX.Device.Screen.SubBackBufferTextureSizeX * GLINUX.Device.Screen.SubBackBufferTextureSizeY ;
		BYTE *p ;

		switch( GSYS.Screen.UserScreenImagePixelFormat )
		{
		case DX_USER_SCREEN_PIXEL_FORMAT_R5G6B5:
			GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGB ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_RGB ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelType = GL_UNSIGNED_SHORT_5_6_5 ;
			break ;

		case DX_USER_SCREEN_PIXEL_FORMAT_R5G5B5X1:
			GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGBA ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_RGBA ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelType = GL_UNSIGNED_SHORT_5_5_5_1 ;
			break ;

		case DX_USER_SCREEN_PIXEL_FORMAT_X1R5G5B5:
			GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGBA ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_RGBA ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelType = GL_UNSIGNED_SHORT_5_5_5_1 ;

			GLINUX.Device.Screen.SubBackBufferTextureTempBuffer = DXALLOC( GLINUX.Device.Screen.SubBackBufferTextureSizeX * GLINUX.Device.Screen.SubBackBufferTextureSizeY * 2 ) ;
			if( GLINUX.Device.Screen.SubBackBufferTextureTempBuffer == NULL )
			{
				DXST_LOGFILE_ADDUTF16LE( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x78\x30\x6e\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xe2\x8e\x01\x90\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"サブバックバッファへのテクスチャ転送用のメモリ確保に失敗しました\n" @*/ ) ;
				return -1 ;
			}

			// 転送用バッファの内容を初期化
			p = ( BYTE * )GLINUX.Device.Screen.SubBackBufferTextureTempBuffer ;
			for( i = 0 ; i < PixelNum ; i ++ )
			{
				( ( WORD * )p )[ i ] = 0x0001 ;
			}
			break ;

		case DX_USER_SCREEN_PIXEL_FORMAT_X8B8G8R8:
			GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGBA ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_RGBA ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelType = GL_UNSIGNED_BYTE ;
			break ;

		case DX_USER_SCREEN_PIXEL_FORMAT_X8R8G8B8:
			if( GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_FORMAT_BGRA8888 ] )
			{
				GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGBA ;
				GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_BGRA_EXT ;
			}
			else
			{
				GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGBA ;
				GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_RGBA ;
				GLINUX.Device.Screen.SubBackBufferTextureTempBuffer = DXALLOC( GLINUX.Device.Screen.SubBackBufferTextureSizeX * GLINUX.Device.Screen.SubBackBufferTextureSizeY * 4 ) ;
				if( GLINUX.Device.Screen.SubBackBufferTextureTempBuffer == NULL )
				{
					DXST_LOGFILE_ADDUTF16LE( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x78\x30\x6e\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xe2\x8e\x01\x90\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"サブバックバッファへのテクスチャ転送用のメモリ確保に失敗しました\n" @*/ ) ;
					return -1 ;
				}

				// 転送用バッファの内容を初期化
				p = ( BYTE * )GLINUX.Device.Screen.SubBackBufferTextureTempBuffer ;
				for( i = 0 ; i < PixelNum ; i ++ )
				{
					( ( DWORD * )p )[ i ] = 0xff000000 ;
				}
			}
			GLINUX.Device.Screen.SubBackBufferTexturePixelType = GL_UNSIGNED_BYTE ;
			break ;
		}
	}
	else
	if( GSYS.Setting.NotUseHardware )
	{
		if( GSYS.Screen.MainScreenColorBitDepth == 16 )
		{
			GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGB ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_RGB ;
			GLINUX.Device.Screen.SubBackBufferTexturePixelType = GL_UNSIGNED_SHORT_5_6_5 ;
		}
		else
		{
			if( GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_FORMAT_BGRA8888 ] )
			{
				GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGBA ;
				GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_BGRA_EXT ;
			}
			else
			{
				DWORD i ;
				DWORD PixelNum ;
				DWORD *p ;

				GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGBA ;
				GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_RGBA ;
				GLINUX.Device.Screen.SubBackBufferTextureTempBuffer = DXALLOC( GLINUX.Device.Screen.SubBackBufferTextureSizeX * GLINUX.Device.Screen.SubBackBufferTextureSizeY * 4 ) ;
				if( GLINUX.Device.Screen.SubBackBufferTextureTempBuffer == NULL )
				{
					DXST_LOGFILE_ADDUTF16LE( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x78\x30\x6e\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xe2\x8e\x01\x90\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"サブバックバッファへのテクスチャ転送用のメモリ確保に失敗しました\n" @*/ ) ;
					return -1 ;
				}

				// 転送用バッファの内容を初期化
				PixelNum = GLINUX.Device.Screen.SubBackBufferTextureSizeX * GLINUX.Device.Screen.SubBackBufferTextureSizeY ;
				p = ( DWORD * )GLINUX.Device.Screen.SubBackBufferTextureTempBuffer ;
				for( i = 0 ; i < PixelNum ; i ++ )
				{
					p[ i ] = 0xff000000 ;
				}
			}
			GLINUX.Device.Screen.SubBackBufferTexturePixelType = GL_UNSIGNED_BYTE ;
		}
	}
	else
	{
		GLINUX.Device.Screen.SubBackBufferTextureInternalFormat = GL_RGB ;
		GLINUX.Device.Screen.SubBackBufferTexturePixelFormat = GL_RGB ;
		GLINUX.Device.Screen.SubBackBufferTexturePixelType = GL_UNSIGNED_BYTE ;
	}

	// Offscreen position framebuffer texture target
	glGenTextures( 1, &GLINUX.Device.Screen.SubBackBufferTexture ) ;
	glBindTexture( GL_TEXTURE_2D, GLINUX.Device.Screen.SubBackBufferTexture ) ;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) ;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) ;
	glTexImage2D( GL_TEXTURE_2D, 0, GLINUX.Device.Screen.SubBackBufferTextureInternalFormat, GLINUX.Device.Screen.SubBackBufferTextureSizeX, GLINUX.Device.Screen.SubBackBufferTextureSizeY, 0, GLINUX.Device.Screen.SubBackBufferTexturePixelFormat, GLINUX.Device.Screen.SubBackBufferTexturePixelType, 0 ) ;

	// ソフトウェアレンダリングモードではない場合のみフレームバッファを作成する
	if( GSYS.Setting.NotUseHardware == FALSE )
	{
		glGenFramebuffers( 1, &GLINUX.Device.Screen.SubBackBufferFrameBuffer ) ;
		glBindFramebuffer( GL_FRAMEBUFFER, GLINUX.Device.Screen.SubBackBufferFrameBuffer ) ;

		// レンダーバッファ識別子を生成します。
		glGenRenderbuffers( 1, &GLINUX.Device.Screen.SubBackBufferDepthBuffer ) ;

		// レンダーバッファ識別子に対応したレンダーバッファオブジェクトを生成します。
		glBindRenderbuffer( GL_RENDERBUFFER, GLINUX.Device.Screen.SubBackBufferDepthBuffer ) ;

		// レンダーバッファの幅と高さを指定します。
		glRenderbufferStorage( GL_RENDERBUFFER, GLINUX.Device.Caps.DepthBufferFormat[ ZBUFFER_FORMAT_24BIT ], GLINUX.Device.Screen.SubBackBufferTextureSizeX, GLINUX.Device.Screen.SubBackBufferTextureSizeY ) ;

		// フレームバッファのアタッチメントとしてレンダーバッファをアタッチします。
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GLINUX.Device.Screen.SubBackBufferDepthBuffer ) ;

		// フレームバッファのアタッチメントとして 2D テクスチャをアタッチします。
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GLINUX.Device.Screen.SubBackBufferTexture, 0 ) ;

		// フレームバッファが完全かどうかチェックします。
		GLint status = glCheckFramebufferStatus( GL_FRAMEBUFFER ) ;
		if( status != GL_FRAMEBUFFER_COMPLETE )
		{
			DXST_LOGFILE_ADDUTF16LE( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"サブバックバッファの作成に失敗しました\n" @*/ ) ;
			return -1 ;
		}
		glBindFramebuffer( GL_FRAMEBUFFER, 0 ) ;
	}

	// 終了
	return 0 ;
}

// サブバックバッファの後始末を行う( 0:成功  -1:失敗 )
extern int Graphics_Linux_TerminateSubBackBuffer( void )
{
	if( GLINUX.Device.Screen.SubBackBufferTextureTempBuffer != NULL )
	{
		DXFREE( GLINUX.Device.Screen.SubBackBufferTextureTempBuffer ) ;
		GLINUX.Device.Screen.SubBackBufferTextureTempBuffer = NULL ;
	}

	if( GLINUX.Device.Screen.SubBackBufferFrameBuffer != 0 )
	{
		glDeleteFramebuffers( 1, &GLINUX.Device.Screen.SubBackBufferFrameBuffer ) ;
		GLINUX.Device.Screen.SubBackBufferFrameBuffer = 0 ;
	}

	if( GLINUX.Device.Screen.SubBackBufferTexture != 0 )
	{
		glDeleteTextures( 1, &GLINUX.Device.Screen.SubBackBufferTexture ) ;
		GLINUX.Device.Screen.SubBackBufferTexture = 0 ;
	}

	if( GLINUX.Device.Screen.SubBackBufferDepthBuffer != 0 )
	{
		glDeleteRenderbuffers( 1, &GLINUX.Device.Screen.SubBackBufferDepthBuffer ) ;
		GLINUX.Device.Screen.SubBackBufferDepthBuffer = 0 ;
	}

	// 終了
	return 0 ;
}

// 表画面が描画先の際の定期処理
extern int Graphics_Linux_FrontScreenProcess( void )
{
	int NowTime ;

	// 描画先が表画面ではない場合は何もしない
	if( ( DWORD )GSYS.DrawSetting.TargetScreen[ 0 ] != DX_SCREEN_FRONT )
	{
		return 0 ;
	}

	// 前回 ScreenCopy をしてから 16ms 以上経過していたら ScreenCopy を行う
	NowTime = NS_GetNowCount( FALSE ) ;
	if( NowTime - GLINUX.Device.Screen.FrontScreenCopyTime > 16 )
	{
		Graphics_Screen_ScreenCopyBase( TRUE ) ;

		GLINUX.Device.Screen.FrontScreenCopyTime = NowTime ;
	}

	// 終了
	return 0 ;
}
























// Linux の画像関係

// LINUX_PIXEL_FORMAT_R8G8B8A8 などのフォーマットに合わせたカラーフォーマット情報を取得する
extern COLORDATA *Graphics_Linux_GetDataFormatColorData( int PixelFormat )
{
	static int InitializeFlag = FALSE ;
	static COLORDATA R8G8B8A8_ColorData ;
	static COLORDATA R8G8B8_ColorData ;
	static COLORDATA A4B4G4R4_ColorData ;
	static COLORDATA A1B5G5R5_ColorData ;
	static COLORDATA B5G6R5_ColorData ;
	static COLORDATA R8_ColorData ;
	static COLORDATA R16G16B16F_ColorData ;
	static COLORDATA R32G32B32F_ColorData ;
	static COLORDATA R16G16B16A16F_ColorData ;
	static COLORDATA R32G32B32A32F_ColorData ;

	if( InitializeFlag == FALSE )
	{
		InitializeFlag = TRUE ;
		NS_CreateColorData( &R8G8B8A8_ColorData,       32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000, 0,  0, FALSE ) ;
		NS_CreateColorData( &R8G8B8_ColorData,         24, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000, 0,  0, FALSE ) ;
		NS_CreateColorData( &A4B4G4R4_ColorData,       16, 0x0000f000, 0x00000f00, 0x000000f0, 0x0000000f, 0,  0, FALSE ) ;
		NS_CreateColorData( &A1B5G5R5_ColorData,       16, 0x0000f800, 0x000007c0, 0x0000003e, 0x00000001, 0,  0, FALSE ) ;
		NS_CreateColorData( &B5G6R5_ColorData,         16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000, 0,  0, FALSE ) ;
		NS_CreateColorData( &R8_ColorData,			    8, 0x000000ff, 0x00000000, 0x00000000, 0x00000000, 0,  0, FALSE ) ;
		NS_CreateColorData( &R16G16B16F_ColorData,      0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 3, 16,  TRUE ) ;
		NS_CreateColorData( &R32G32B32F_ColorData,      0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 3, 32,  TRUE ) ;
		NS_CreateColorData( &R16G16B16A16F_ColorData,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 16,  TRUE ) ;
		NS_CreateColorData( &R32G32B32A32F_ColorData,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 32,  TRUE ) ;
	}

	     if( PixelFormat == LINUX_PIXEL_FORMAT_R8G8B8A8			) return &R8G8B8A8_ColorData ;
	else if( PixelFormat == LINUX_PIXEL_FORMAT_R8G8B8				) return &R8G8B8_ColorData ;
	else if( PixelFormat == LINUX_PIXEL_FORMAT_A4B4G4R4			) return &A4B4G4R4_ColorData ;
	else if( PixelFormat == LINUX_PIXEL_FORMAT_A1B5G5R5			) return &A1B5G5R5_ColorData ;
	else if( PixelFormat == LINUX_PIXEL_FORMAT_B5G6R5				) return &B5G6R5_ColorData ;
	else if( PixelFormat == LINUX_PIXEL_FORMAT_R8					) return &R8_ColorData ;
	else if( PixelFormat == LINUX_PIXEL_FORMAT_R16G16B16_FLOAT	) return &R16G16B16F_ColorData ;
	else if( PixelFormat == LINUX_PIXEL_FORMAT_R32G32B32_FLOAT	) return &R32G32B32F_ColorData ;
	else if( PixelFormat == LINUX_PIXEL_FORMAT_R16G16B16A16_FLOAT	) return &R16G16B16A16F_ColorData ;
	else if( PixelFormat == LINUX_PIXEL_FORMAT_R32G32B32A32_FLOAT	) return &R32G32B32A32F_ColorData ;

	return NULL ;
}



// 基本イメージのフォーマットを DX_BASEIMAGE_FORMAT_NORMAL に変換する必要があるかどうかをチェックする
// ( RequiredRgbBaseImageConvFlag と RequiredAlphaBaseImageConvFlag に入る値  TRUE:変換する必要がある  FALSE:変換する必要は無い )
extern	int		Graphics_CheckRequiredNormalImageConv_BaseImageFormat_PF(
	IMAGEDATA_ORIG *Orig,
	int             RgbBaseImageFormat,
	int            *RequiredRgbBaseImageConvFlag,
	int             AlphaBaseImageFormat,
	int            *RequiredAlphaBaseImageConvFlag
)
{
	int RgbConvFlag   = FALSE ;
	int AlphaConvFlag = FALSE ;

	if( RgbBaseImageFormat != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		RgbConvFlag = TRUE ;
	}

	if( AlphaBaseImageFormat >= 0 && AlphaBaseImageFormat != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		AlphaConvFlag = TRUE ;
	}

//	if( Orig->FormatDesc.TextureFlag )
//	{
//		if( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2 || RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4 )
//		{
//			RgbConvFlag = TRUE ;
//		}
//
//		if( AlphaBaseImageFormat >= 0 )
//		{
//			if( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2 || AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4 )
//			{
//				AlphaConvFlag = TRUE ;
//			}
//		}
//	}
//	else
//	{
//		if( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT1 ||
//			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2 ||
//			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT3 ||
//			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4 ||
//			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT5 ||
//			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_BC7_UNORM ||
//			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_BC7_UNORM_SRGB )
//		{
//			RgbConvFlag = TRUE ;
//		}
//
//		if( AlphaBaseImageFormat >= 0 )
//		{
//			if( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT1 ||
//				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2 ||
//				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT3 ||
//				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4 ||
//				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT5 ||
//				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_BC7_UNORM ||
//				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_BC7_UNORM_SRGB )
//			{
//				AlphaConvFlag = TRUE ;
//			}
//		}
//	}

	if( RequiredRgbBaseImageConvFlag != NULL )
	{
		*RequiredRgbBaseImageConvFlag = RgbConvFlag ;
	}

	if( RequiredAlphaBaseImageConvFlag != NULL )
	{
		*RequiredAlphaBaseImageConvFlag = AlphaConvFlag ;
	}

	// 正常終了
	return 0 ;
}

// テクスチャ画像転送用の共有メモリのセットアップを行う
extern int Graphics_Linux_Texture_SetupCommonBuffer( unsigned int Size )
{
	// 既に指定のサイズを確保してあったら何もせずに終了
	if( GLINUX.Texture.CommonBufferSize >= Size )
	{
		return 0 ;
	}

	// 既存のメモリを解放
	DXFREE( GLINUX.Texture.CommonBuffer ) ;
	GLINUX.Texture.CommonBuffer     = NULL ;
	GLINUX.Texture.CommonBufferSize = 0 ;

	// 容量を追加する
	Size += 512 * 1024 ;

	// メモリの確保
	GLINUX.Texture.CommonBuffer = DXALLOC( Size ) ;
	if( GLINUX.Texture.CommonBuffer == NULL )
	{
		DXST_LOGFILEFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6b\x30\x3b\x75\xcf\x50\x92\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5b\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x5f\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x43\x00\x6f\x00\x6d\x00\x6d\x00\x6f\x00\x6e\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x5d\x00\x0a\x00\x00"/*@ L"テクスチャに画像を転送するためのメモリの確保に失敗しました[Graphics_Linux_Texture_SetupCommonBuffer]\n" @*/ )) ;
		return -1 ;
	}

	// 確保したメモリのサイズを保存
	GLINUX.Texture.CommonBufferSize = Size ;

	// 終了
	return 0 ;
}

// テクスチャ画像転送用の共有メモリの後始末を行う
extern int Graphics_Linux_Texture_TerminateCommonBuffer( void )
{
	// メモリを確保していたら解放
	if( GLINUX.Texture.CommonBuffer != NULL )
	{
		DXFREE( GLINUX.Texture.CommonBuffer ) ;
		GLINUX.Texture.CommonBuffer = NULL ;
	}
	GLINUX.Texture.CommonBufferSize = 0 ;

	// 終了
	return 0 ;
}

// レンダーターゲットテクスチャにテクスチャの単純転送を行う
extern int		Graphics_Linux_StretchRect(
	GLuint					SrcTexture,			uint32_t SrcTextureWidth,   uint32_t SrcTextureHeight,   const RECT *SrcRect,	// NULL で全体
	GLuint					FrameBuffer,		uint32_t FrameBufferWidth,  uint32_t FrameBufferHeight,  const RECT *DestRect,	// NULL で全体
	GLenum					FilterType,
	int						AlphaBlend,
	GRAPHICS_LINUX_SHADER	*Shader,
	GLuint					BlendTexture,		uint32_t BlendTextureWidth, uint32_t BlendTextureHeight, const RECT *BlendRect,	// NULL で全体
	VERTEX_TEX8_2D *		Texcoord8Vertex
)
{
	float *VertexDataP ;
	float VertexData[ 4 * 18 ] ;
	uint32_t	SrcTexWidth ;
	uint32_t	SrcTexHeight ;
	uint32_t	DestTexWidth ;
	uint32_t	DestTexHeight ;
	RECTF		SrcRectF ;
	RECTF		DestRectF ;
	RECTF		BlendRectF = { 0.0f } ;
	RECT		SrcRectTemp ;
	RECT		DestRectTemp ;
	RECT		BlendRectTemp ;
	int			BlendMode ;
	int			BlendEnable ;
	int			BlendRGBSrc ;
	int			BlendRGBDest ;
	int			BlendRGBOp ;
	int			BlendASrc ;
	int			BlendADest ;
	int			BlendAOp ;
	int			NotWriteAlphaChannelFlag ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// テクスチャ情報を取得
	SrcTexWidth  = SrcTextureWidth ;
	SrcTexHeight = SrcTextureHeight ;
	DestTexWidth  = FrameBufferWidth ;
	DestTexHeight = FrameBufferHeight ;

	if( SrcRect == NULL )
	{
		SrcRectTemp.left   = 0 ;
		SrcRectTemp.top    = 0 ;
		SrcRectTemp.right  = ( LONG )SrcTexWidth ;
		SrcRectTemp.bottom = ( LONG )SrcTexHeight ;

		SrcRect = &SrcRectTemp ;
	}

	if( DestRect == NULL )
	{
		DestRectTemp.left   = 0 ;
		DestRectTemp.top    = 0 ;
		DestRectTemp.right  = ( LONG )DestTexWidth ;
		DestRectTemp.bottom = ( LONG )DestTexHeight ;

		DestRect = &DestRectTemp ;
	}

	// 頂点書き出し
	DRAWSTOCKINFO

	// 描画終了
	Graphics_Linux_RenderEnd() ;

	// 頂点データを準備
	if( Texcoord8Vertex == NULL )
	{
		DestRectF.left   = ( float )DestRect->left   / DestTexWidth  * 2.0f - 1.0f ;
		DestRectF.top    = ( float )DestRect->top    / DestTexHeight * 2.0f - 1.0f ;
		DestRectF.right  = ( float )DestRect->right  / DestTexWidth  * 2.0f - 1.0f ;
		DestRectF.bottom = ( float )DestRect->bottom / DestTexHeight * 2.0f - 1.0f ;

		SrcRectF.left    = ( float )SrcRect->left    / SrcTexWidth ;
		SrcRectF.top     = ( float )SrcRect->top     / SrcTexHeight ;
		SrcRectF.right   = ( float )SrcRect->right   / SrcTexWidth ;
		SrcRectF.bottom  = ( float )SrcRect->bottom  / SrcTexHeight ;

		if( BlendTexture != 0 )
		{
			if( BlendRect == NULL )
			{
				BlendRectTemp.left   = 0 ;
				BlendRectTemp.top    = 0 ;
				BlendRectTemp.right  = ( LONG )BlendTextureWidth ;
				BlendRectTemp.bottom = ( LONG )BlendTextureHeight ;

				BlendRect = &BlendRectTemp ;
			}

			BlendRectF.left    = ( float )BlendRect->left    / BlendTextureWidth ;
			BlendRectF.top     = ( float )BlendRect->top     / BlendTextureHeight ;
			BlendRectF.right   = ( float )BlendRect->right   / BlendTextureWidth ;
			BlendRectF.bottom  = ( float )BlendRect->bottom  / BlendTextureHeight ;
		}

		// 頂点データをセット
		VertexDataP = VertexData ;

		VertexDataP[ 0 ] = DestRectF.left ;
		VertexDataP[ 1 ] = DestRectF.top ;
		VertexDataP[ 2 ] = SrcRectF.left ;
		VertexDataP[ 3 ] = SrcRectF.top ;
		VertexDataP += 4 ;
		if( BlendTexture != 0 )
		{
			VertexDataP[ 0 ] = BlendRectF.left ;
			VertexDataP[ 1 ] = BlendRectF.top ;
			VertexDataP += 2 ;
		}

		VertexDataP[ 0 ] = DestRectF.right ;
		VertexDataP[ 1 ] = DestRectF.top ;
		VertexDataP[ 2 ] = SrcRectF.right ;
		VertexDataP[ 3 ] = SrcRectF.top ;
		VertexDataP += 4 ;
		if( BlendTexture != 0 )
		{
			VertexDataP[ 0 ] = BlendRectF.right ;
			VertexDataP[ 1 ] = BlendRectF.top ;
			VertexDataP += 2 ;
		}

		VertexDataP[ 0 ] = DestRectF.left ;
		VertexDataP[ 1 ] = DestRectF.bottom ;
		VertexDataP[ 2 ] = SrcRectF.left ;
		VertexDataP[ 3 ] = SrcRectF.bottom ;
		VertexDataP += 4 ;
		if( BlendTexture != 0 )
		{
			VertexDataP[ 0 ] = BlendRectF.left ;
			VertexDataP[ 1 ] = BlendRectF.bottom ;
			VertexDataP += 2 ;
		}

		VertexDataP[ 0 ] = DestRectF.right ;
		VertexDataP[ 1 ] = DestRectF.bottom ;
		VertexDataP[ 2 ] = SrcRectF.right ;
		VertexDataP[ 3 ] = SrcRectF.bottom ;
		VertexDataP += 4 ;
		if( BlendTexture != 0 )
		{
			VertexDataP[ 0 ] = BlendRectF.right ;
			VertexDataP[ 1 ] = BlendRectF.bottom ;
			VertexDataP += 2 ;
		}
	}
	else
	{
		int i ;

		// 頂点データをセット
		VertexDataP = VertexData ;

		for( i = 0 ; i < 4 ; i ++ )
		{
			VertexDataP[  0 ] = Texcoord8Vertex[ i ].pos.x / DestTexWidth  * 2.0f - 1.0f ;
			VertexDataP[  1 ] = Texcoord8Vertex[ i ].pos.y / DestTexHeight * 2.0f - 1.0f ;

			VertexDataP[  2 ] = Texcoord8Vertex[ i ].u0 ;
			VertexDataP[  3 ] = Texcoord8Vertex[ i ].v0 ;
			VertexDataP[  4 ] = Texcoord8Vertex[ i ].u1 ;
			VertexDataP[  5 ] = Texcoord8Vertex[ i ].v1 ;
			VertexDataP[  6 ] = Texcoord8Vertex[ i ].u2 ;
			VertexDataP[  7 ] = Texcoord8Vertex[ i ].v2 ;
			VertexDataP[  8 ] = Texcoord8Vertex[ i ].u3 ;
			VertexDataP[  9 ] = Texcoord8Vertex[ i ].v3 ;
			VertexDataP[ 10 ] = Texcoord8Vertex[ i ].u4 ;
			VertexDataP[ 11 ] = Texcoord8Vertex[ i ].v4 ;
			VertexDataP[ 12 ] = Texcoord8Vertex[ i ].u5 ;
			VertexDataP[ 13 ] = Texcoord8Vertex[ i ].v5 ;
			VertexDataP[ 14 ] = Texcoord8Vertex[ i ].u6 ;
			VertexDataP[ 15 ] = Texcoord8Vertex[ i ].v6 ;
			VertexDataP[ 16 ] = Texcoord8Vertex[ i ].u7 ;
			VertexDataP[ 17 ] = Texcoord8Vertex[ i ].v7 ;

			VertexDataP += 18 ;
		}
	}

	// 描画の開始を行う
	Graphics_Linux_RenderBegin() ;

	// StretchRect用の設定を行う
	{
		// 描画先のフレームバッファを変更( 設定は Graphics_Linux_DeviceState_RefreshRenderState で戻す )
		glBindFramebuffer( GL_FRAMEBUFFER, FrameBuffer ) ;

		// ビューポートをセット( 設定は Graphics_Linux_DeviceState_RefreshRenderState で戻す )
		glViewport( 0, 0, FrameBufferWidth, FrameBufferHeight ) ;

		// Zバッファを無効化
		Graphics_Linux_DeviceState_SetDepthEnable( FALSE ) ;
		Graphics_Linux_DeviceState_SetDepthWriteEnable( FALSE ) ;

		// シェーダーをセット
		if( Shader == NULL )
		{
			Shader = &GLINUX.Device.Shader.Base.StretchRect_Shader ;
		}
		Graphics_Linux_DeviceState_SetShader( Shader, FALSE ) ;

		// Uniform の更新
		Graphics_Linux_DeviceState_UpdateShaderUniform( Shader, 0 ) ;

		// テクスチャをセット( 設定は Graphics_Linux_DeviceState_RefreshRenderState で戻す )
		glActiveTexture( GL_TEXTURE0 ) ;
		glBindTexture( GL_TEXTURE_2D, SrcTexture ) ;

		// テクスチャフィルタリング設定を行う
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterType ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterType ) ;

		// ブレンドテクスチャをセット
		if( BlendTexture != 0 )
		{
			// テクスチャをセット( 設定は Graphics_Linux_DeviceState_RefreshRenderState で戻す )
			glActiveTexture( GL_TEXTURE1 ) ;
			glBindTexture( GL_TEXTURE_2D, BlendTexture ) ;

			// テクスチャフィルタリング設定を行う
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterType ) ;
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterType ) ;
		}

		// ブレンドモードを変更
		BlendMode		= GLINUX.Device.State.BlendMode ;
		BlendEnable		= GLINUX.Device.State.BlendEnable ;
		BlendRGBSrc		= GLINUX.Device.State.BlendRGBSrc ;
		BlendRGBDest	= GLINUX.Device.State.BlendRGBDest ;
		BlendRGBOp		= GLINUX.Device.State.BlendRGBOp ;
		BlendASrc		= GLINUX.Device.State.BlendASrc ;
		BlendADest		= GLINUX.Device.State.BlendADest ;
		BlendAOp		= GLINUX.Device.State.BlendAOp ;
		NotWriteAlphaChannelFlag = GLINUX.Device.State.NotWriteAlphaChannelFlag ;
		Graphics_Linux_DeviceState_SetBlendMode( AlphaBlend ? DX_BLENDMODE_ALPHA : DX_BLENDMODE_NOBLEND, FALSE, DX_BLEND_ONE, DX_BLEND_ZERO, DX_BLENDOP_ADD, DX_BLEND_ONE, DX_BLEND_ZERO, DX_BLENDOP_ADD, FALSE ) ;

		// 頂点データのセットアップ
		Graphics_Linux_DeviceState_SetupShaderVertexData(
			Shader,
			Texcoord8Vertex != NULL ? &g_StretchRectTex8VertexInputInfo : ( BlendTexture != 0 ? &g_StretchRectTex2VertexInputInfo : &g_StretchRectVertexInputInfo ),
			VertexData
		) ;
	}

	// 描画
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) ;
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	// 設定を元に戻す
	{
		// ブレンドモードを変更
		Graphics_Linux_DeviceState_SetBlendMode(
			BlendMode,
			BlendEnable,
			BlendRGBSrc,
			BlendRGBDest,
			BlendRGBOp,
			BlendASrc,
			BlendADest,
			BlendAOp,
			NotWriteAlphaChannelFlag
		) ;

		// 今までの設定を復帰する
		Graphics_Linux_DeviceState_RefreshRenderState() ;
	}

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// 正常終了
	return 0 ;
}

// 指定のテクスチャの内容に読み込みアクセスできるようにする
extern	int		Graphics_Linux_Texture_Map(
	GLuint				TargetTexture,
	GLuint				TargetFrameBuffer,
	GLuint				TargetWidth,
	GLuint				TargetHeight,
	const RECT *		MapRect /* NULL で全体 */,
	BASEIMAGE *			MapBaseImage,
	void **				MapTempImage
)
{
	GLuint Width ;
	GLuint Height ;
	RECT   TempRect ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( MapRect == NULL )
	{
		TempRect.left   = 0 ;
		TempRect.top    = 0 ;
		TempRect.right  = ( LONG )TargetWidth ;
		TempRect.bottom = ( LONG )TargetHeight ;
		MapRect         = &TempRect ;
	}

	// エラーチェック
	if( MapRect->left >= MapRect->right ||
		MapRect->top  >= MapRect->bottom ||
		MapRect->left   < 0 ||
		MapRect->top    < 0 ||
		MapRect->right  > ( int )TargetWidth ||
		MapRect->bottom > ( int )TargetHeight )
	{
		return -1 ;
	}

	Width  = ( GLuint )( MapRect->right  - MapRect->left ) ;
	Height = ( GLuint )( MapRect->bottom - MapRect->top ) ;

	// イメージデータを保存するメモリ領域の確保
	*MapTempImage = DXALLOC( Width * Height * 4 * 2 ) ;
	if( *MapTempImage == NULL )
	{
		return -1 ;
	}

	// テクスチャの場合はフレームバッファに書き出す
	if( TargetFrameBuffer == 0 )
	{
		GLuint TempFrameBuffer = 0 ;
		GLuint TempTexture = 0 ;

		// 作業用テクスチャの作成
		glGenTextures( 1, &TempTexture ) ;
		glBindTexture( GL_TEXTURE_2D, TempTexture ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) ;
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 ) ;

		// 作業用フレームバッファの作成
		glGenFramebuffers( 1, &TempFrameBuffer );
		glBindFramebuffer( GL_FRAMEBUFFER, TempFrameBuffer );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TempTexture, 0 ) ;
		if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )   
		{
			DXFREE( *MapTempImage ) ;
			*MapTempImage = NULL ;
			return -1 ;
		}

		// 作業用フレームバッファにテクスチャの内容を転送
		Graphics_Linux_StretchRect( TargetTexture, TargetWidth, TargetHeight, MapRect, TempFrameBuffer, Width, Height, NULL ) ;

		// ピクセルデータの読み取り
		glBindFramebuffer( GL_FRAMEBUFFER, TempFrameBuffer ) ;
		glReadPixels( 0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, *MapTempImage ) ;
		glBindFramebuffer( GL_FRAMEBUFFER, GLINUX.Device.State.TargetFrameBuffer ) ;

		// フレームバッファとテクスチャの削除
		glBindTexture( GL_TEXTURE_2D, 0 ) ;
		glDeleteTextures( 1, &TempTexture ) ;
		glDeleteFramebuffers( 1, &TempFrameBuffer ) ;
	}
	else
	{
//		GLint ext_format, ext_type ;
//		glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &ext_format);
//		glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &ext_type);

// テストコード
//#ifdef _DEBUG
//		_MEMSET( *MapTempImage, 0xff, Width * Height * 2 * 4 ) ;
//#endif

		// ピクセルデータの読み取り
		glBindFramebuffer( GL_FRAMEBUFFER, TargetFrameBuffer ) ;

		GLint pack_backup, unpack_backup ;
		glGetIntegerv( GL_PACK_ALIGNMENT,   &pack_backup ) ;
		glGetIntegerv( GL_UNPACK_ALIGNMENT, &unpack_backup ) ;
		glPixelStorei( GL_PACK_ALIGNMENT,   1 ) ;
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) ;
		glReadPixels( ( GLuint )MapRect->left, ( GLuint )MapRect->top, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, *MapTempImage ) ;
		glPixelStorei( GL_PACK_ALIGNMENT,   pack_backup ) ;
		glPixelStorei( GL_UNPACK_ALIGNMENT, unpack_backup ) ;

		glBindFramebuffer( GL_FRAMEBUFFER, GLINUX.Device.State.TargetFrameBuffer ) ;

// テストコード
//#ifdef _DEBUG
//		int kk ;
//		for( kk = Width * Height * 2 * 4 - 1 ; kk >= 0; kk -- )
//		{
//			if( ( ( BYTE * )*MapTempImage )[ kk ] != 0xff )
//			{
//				break ;
//			}
//		}
//		if( kk >= Width * Height * 4 )
//		{
//			kk = 0 ;
//		}
//#endif
	}

	// 基本イメージ構造体のデータにセット
	MapBaseImage->ColorData      = *Graphics_Linux_GetDataFormatColorData( LINUX_PIXEL_FORMAT_R8G8B8A8 ) ;
	MapBaseImage->Width          = ( int )Width ;
	MapBaseImage->Height         = ( int )Height ;
	MapBaseImage->Pitch          = Width * 4 ;
	MapBaseImage->GraphData      = ( BYTE * )*MapTempImage ;
	MapBaseImage->MipMapCount    = 0 ;
	MapBaseImage->GraphDataCount = 0 ;

	// 正常終了
	return 0 ;
}

// 指定のテクスチャの内容への読み込みアクセスを終了する
extern	int		Graphics_Linux_Texture_Unmap( void * MapTempImage )
{
	// メモリの解放
	if( MapTempImage != NULL )
	{
		DXFREE( MapTempImage ) ;
		MapTempImage = NULL ;
	}

	// 終了
	return 0 ;
}












































// LinuxDevice の初期化関係

// デバイスの作成に相当する処理を行う( 0:成功  -1:失敗 )
extern	int		Graphics_Linux_Device_Create( void )
{
	// OpenGL ES と EGL の初期化

	// 目的の構成の属性をここで指定します。
	// 以下で、オンスクリーン ウィンドウと
	// 互換性のある、各色最低 8 ビットのコンポーネントの EGLConfig を選択します
	EGLint numConfigs ;
	EGLConfig config ;
	EGLint Major, Minor ;

	const EGLint attribs[] = 
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	} ;
	const EGLint context_attribs[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	} ;

	Display *xdpy = XOpenDisplay(NULL);
	GLINUX.Device.Screen.XDisplay = xdpy ;
	if(!xdpy)
	{
		return -1;
	}
	GLINUX.Device.Screen._atom_WM_PROTOCOLS = XInternAtom(xdpy, "WM_PROTOCOLS", False) ;
	GLINUX.Device.Screen._atom_WM_DELETE_WINDOW = XInternAtom(xdpy, "WM_DELETE_WINDOW", False) ;
	GLINUX.Device.Screen._atom_XdndAware = XInternAtom(xdpy, "XdndAware", False) ;
	GLINUX.Device.Screen._atom_XdndEnter = XInternAtom(xdpy, "XdndEnter", False) ;
	GLINUX.Device.Screen._atom_XdndLeave = XInternAtom(xdpy, "XdndLeave", False) ;
	GLINUX.Device.Screen._atom_XdndPosition = XInternAtom(xdpy, "XdndPosition", False) ;
	GLINUX.Device.Screen._atom_XdndStatus = XInternAtom(xdpy, "XdndStatus", False) ;
	GLINUX.Device.Screen._atom_XdndFinished = XInternAtom(xdpy, "XdndFinished", False) ;
	GLINUX.Device.Screen._atom_XdndDrop = XInternAtom(xdpy, "XdndDrop", False) ;
	GLINUX.Device.Screen._atom_XdndTypeList = XInternAtom(xdpy, "XdndTypeList", False) ;
	GLINUX.Device.Screen._atom_XdndActionCopy = XInternAtom(xdpy, "XdndActionCopy", False) ;
	GLINUX.Device.Screen._atom_XdndSelection = XInternAtom(xdpy, "XdndSelection", False) ;
	GLINUX.Device.Screen._atom_text_uri_list = XInternAtom(xdpy, "text/uri-list", False) ;
	GLINUX.Device.Screen._atom_CLIPBOARD = XInternAtom(xdpy, "CLIPBOARD", False) ;
	GLINUX.Device.Screen._atom_TARGETS = XInternAtom(xdpy, "TARGETS", False) ;
	GLINUX.Device.Screen._atom_UTF8_STRING = XInternAtom(xdpy, "UTF8_STRING", False) ;
	GLINUX.Device.Screen._atom_INCR = XInternAtom(xdpy, "INCR", False) ;
	GLINUX.Device.Screen._xdnd_cache_itsForMe = 0;
	GLINUX.Device.Screen._xdnd_cache_sourceWindow = 0;
	GLINUX.Device.Screen._xdnd_bufPtr = NULL;
	GLINUX.Device.Screen._xdnd_bufLen = 0;
	GLINUX.Device.Screen._xdnd_bufPos = 0;
	GLINUX.Device.Screen._clipboard_set_bufPtr = NULL;
	GLINUX.Device.Screen._clipboard_set_bufLen = 0;
	GLINUX.Device.Screen._clipboard_get_bufPtr = NULL;
	GLINUX.Device.Screen._clipboard_get_bufLen = 0;
	GLINUX.Device.Screen.Display = eglGetDisplay( xdpy ) ;

	// EGL を初期化
	Major = 0 ;
	Minor = 0 ;
	eglInitialize(      GLINUX.Device.Screen.Display, &Major, &Minor ) ;
	eglChooseConfig(    GLINUX.Device.Screen.Display, attribs, &config, 1, &numConfigs ) ;
	GLINUX.Device.Screen.Config = config ;


	Window xwin = XCreateSimpleWindow(xdpy, DefaultRootWindow(xdpy), 0, 0, GSYS.Screen.MainScreenSizeX, GSYS.Screen.MainScreenSizeY, 1, BlackPixel(xdpy, 0), BlackPixel(xdpy, 0));
	GLINUX.Device.Screen.XWindow = xwin;
	XSetWMProtocols(xdpy, xwin, &GLINUX.Device.Screen._atom_WM_DELETE_WINDOW, 1);
	XSelectInput(xdpy, xwin,
        ButtonPressMask |
        ButtonReleaseMask |
        PointerMotionMask |
        KeyPressMask |
        KeyReleaseMask |
        StructureNotifyMask |
        ExposureMask );
	XMapWindow(xdpy, xwin);
	XFlush(xdpy);
	GLINUX.Device.Screen.Surface = eglCreateWindowSurface( GLINUX.Device.Screen.Display, config, xwin, NULL ) ;
	GLINUX.Device.Screen.Context = eglCreateContext(       GLINUX.Device.Screen.Display, config, EGL_NO_CONTEXT, context_attribs ) ;

	if( eglMakeCurrent( GLINUX.Device.Screen.Display, GLINUX.Device.Screen.Surface, GLINUX.Device.Screen.Surface, GLINUX.Device.Screen.Context ) == EGL_FALSE )
	{
		DXST_LOGFILE_ADDUTF16LE( "\x65\x00\x67\x00\x6c\x00\x4d\x00\x61\x00\x6b\x00\x65\x00\x43\x00\x75\x00\x72\x00\x72\x00\x65\x00\x6e\x00\x74\x00\x20\x00\x4c\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"eglMakeCurrent が失敗しました\n" @*/ ) ;
		return -1 ;
	}

	// 画面サイズ取得
	eglQuerySurface( GLINUX.Device.Screen.Display, GLINUX.Device.Screen.Surface, EGL_WIDTH,  &GLINUX.Device.Screen.Width ) ;
	eglQuerySurface( GLINUX.Device.Screen.Display, GLINUX.Device.Screen.Surface, EGL_HEIGHT, &GLINUX.Device.Screen.Height ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( "\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xb5\x30\xa4\x30\xba\x30\x3a\x00\x25\x00\x64\x00\x20\x00\x78\x00\x20\x00\x25\x00\x64\x00\x00"/*@ L"スクリーンサイズ:%d x %d" @*/, GLINUX.Device.Screen.Width, GLINUX.Device.Screen.Height )) ;

	// バージョンやベンダー名を取得
	{
		const GLubyte *Version					= glGetString( GL_VERSION ) ;
		const GLubyte *Renderer					= glGetString( GL_RENDERER ) ;
		const GLubyte *Vendor					= glGetString( GL_VENDOR ) ;
		const GLubyte *Shading_Language_Version	= glGetString( GL_SHADING_LANGUAGE_VERSION ) ;
		char TempStr[ 1024 ] ;
		int Length ;

		if( Version != NULL )
		{
			Length = CL_strlen( DX_CHARCODEFORMAT_ASCII, ( char * )Version ) ;
			GLINUX.Device.Caps.Version = ( TCHAR * )DXALLOC( sizeof( TCHAR ) * ( Length + 1 ) ) ;
			ConvString( ( char * )Version, -1, DX_CHARCODEFORMAT_ASCII, GLINUX.Device.Caps.Version, sizeof( TCHAR ) * ( Length + 1 ), _TCHARCODEFORMAT ) ;
			if( ( DWORD )( Length * 4 + 2 )  < sizeof( TempStr ) )
			{
				ConvString( ( char * )Version, -1, DX_CHARCODEFORMAT_ASCII, TempStr, sizeof( TempStr ), DX_CHARCODEFORMAT_UTF16LE ) ;
				DXST_LOGFILEFMT_ADDUTF16LE(( "\xd0\x30\xfc\x30\xb8\x30\xe7\x30\xf3\x30\x3a\x00\x25\x00\x73\x00\x00"/*@ L"バージョン:%s" @*/, TempStr )) ;
			}
		}

		if( Renderer != NULL )
		{
			Length = CL_strlen( DX_CHARCODEFORMAT_ASCII, ( char * )Renderer ) ;
			GLINUX.Device.Caps.Renderer = ( TCHAR * )DXALLOC( sizeof( TCHAR ) * ( Length + 1 ) ) ;
			ConvString( ( char * )Renderer, -1, DX_CHARCODEFORMAT_ASCII, GLINUX.Device.Caps.Renderer, sizeof( TCHAR ) * ( Length + 1 ), _TCHARCODEFORMAT ) ;
			if( ( DWORD )( Length * 4 + 2 ) < sizeof( TempStr ) )
			{
				ConvString( ( char * )Renderer, -1, DX_CHARCODEFORMAT_ASCII, TempStr, sizeof( TempStr ), DX_CHARCODEFORMAT_UTF16LE ) ;
				DXST_LOGFILEFMT_ADDUTF16LE(( "\xec\x30\xf3\x30\xc0\x30\xe9\x30\xfc\x30\x3a\x00\x25\x00\x73\x00\x00"/*@ L"レンダラー:%s" @*/, TempStr )) ;
			}
		}

		if( Vendor != NULL )
		{
			Length = CL_strlen( DX_CHARCODEFORMAT_ASCII, ( char * )Vendor ) ;
			GLINUX.Device.Caps.Vendor = ( TCHAR * )DXALLOC( sizeof( TCHAR ) * ( Length + 1 ) ) ;
			ConvString( ( char * )Vendor, -1, DX_CHARCODEFORMAT_ASCII, GLINUX.Device.Caps.Vendor, sizeof( TCHAR ) * ( Length + 1 ), _TCHARCODEFORMAT ) ;
			if( ( DWORD )( Length * 4 + 2 ) < sizeof( TempStr ) )
			{
				ConvString( ( char * )Vendor, -1, DX_CHARCODEFORMAT_ASCII, TempStr, sizeof( TempStr ), DX_CHARCODEFORMAT_UTF16LE ) ;
				DXST_LOGFILEFMT_ADDUTF16LE(( "\xd9\x30\xf3\x30\xc0\x30\xfc\x30\x3a\x00\x25\x00\x73\x00\x00"/*@ L"ベンダー:%s" @*/, TempStr )) ;
			}
		}

		if( Shading_Language_Version != NULL )
		{
			Length = CL_strlen( DX_CHARCODEFORMAT_ASCII, ( char * )Shading_Language_Version ) ;
			GLINUX.Device.Caps.Shading_Language_Version = ( TCHAR * )DXALLOC( sizeof( TCHAR ) * ( Length + 1 ) ) ;
			ConvString( ( char * )Shading_Language_Version, -1, DX_CHARCODEFORMAT_ASCII, GLINUX.Device.Caps.Shading_Language_Version, sizeof( TCHAR ) * ( Length + 1 ), _TCHARCODEFORMAT ) ;
			if( ( DWORD )( Length * 4 + 2 ) < sizeof( TempStr ) )
			{
				ConvString( ( char * )Shading_Language_Version, -1, DX_CHARCODEFORMAT_ASCII, TempStr, sizeof( TempStr ), DX_CHARCODEFORMAT_UTF16LE ) ;
				DXST_LOGFILEFMT_ADDUTF16LE(( "\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x00\x8a\x9e\x8a\xd0\x30\xfc\x30\xb8\x30\xe7\x30\xf3\x30\x3a\x00\x25\x00\x73\x00\x00"/*@ L"シェーダー言語バージョン:%s" @*/, TempStr )) ;
			}
		}
	}

	// Extensions 解析
	{
		const GLubyte *lExtensions = glGetString( GL_EXTENSIONS );
		const GLubyte *lExtensionsTemp ;
		int i ;
		int j ;
		int k ;
		int l ;

        if( lExtensions != NULL )
        {
            DXST_LOGFILE_ADDUTF16LE(( "\x4f\x00\x70\x00\x65\x00\x6e\x00\x47\x00\x4c\x00\x20\x00\x45\x00\x53\x00\x20\x00\x45\x00\x78\x00\x74\x00\x65\x00\x6e\x00\x73\x00\x69\x00\x6f\x00\x6e\x00\x73\x00\x20\x00\x00\x4e\xa7\x89\x0a\x00\x00"/*@ L"OpenGL ES Extensions 一覧\n" @*/ )) ;

            _MEMSET( GLINUX.Device.Caps.Extensions, 0, sizeof( GLINUX.Device.Caps.Extensions ) ) ;

            GLINUX.Device.Caps.ExtensionNum = 0 ;
            j = 0 ;
            lExtensionsTemp = lExtensions ;
            for(;;)
            {
                if( lExtensionsTemp[ j ] == '\0' )
                {
                    break ;
                }

                // ログに出力
                {
                    char lTempStr[ 256 ] ;
                    for( i = 0 ; i < 230 && lExtensionsTemp[ j + i ] != '\0' && lExtensionsTemp[ j + i ] != ' '; i++ )
                    {
                        lTempStr[ i + 1 ] = lExtensionsTemp[ j + i ] ;
                    }
                    lTempStr[ 0 ]     = '\t' ;
                    lTempStr[ i + 1 ] = '\n' ;
                    lTempStr[ i + 2 ] = '\0' ;
                    DXST_LOGFILE_ADDA( lTempStr ) ;
                }

                for( i = 0; i < LINUX_GL_EXTENSION_NUM; i++ )
                {
                    k = 0 ;
                    for(;;)
                    {
                        if( lExtensionsTemp[ j + k ] == '\0' || lExtensionsTemp[ j + k ] == ' ' )
                        {
                            break ;
                        }

                        if( g_LinuxGLExtensionName[ i ][ k ] == '\0' )
                        {
                            break ;
                        }

                        if( lExtensionsTemp[ j + k ] != g_LinuxGLExtensionName[ i ][ k ] )
                        {
                            break ;
                        }

                        k ++ ;
                    }
                    if( g_LinuxGLExtensionName[ i ][ k ] == '\0' &&
                        ( lExtensionsTemp[ j + k ] == ' ' || lExtensionsTemp[ j + k ] == '\0' ) )
                    {
                        GLINUX.Device.Caps.Extensions[ i ] = TRUE ;
                    }
                }
                GLINUX.Device.Caps.ExtensionNum ++ ;

                for( ; lExtensionsTemp[ j ] != ' ' && lExtensionsTemp[ j ] != '\0'; j++ ){}
                if( lExtensionsTemp[ j ] == '\0' )
                {
                    break;
                }
                j++ ;
            }

            GLINUX.Device.Caps.ExtensionNames = ( TCHAR ** )DXALLOC( sizeof( TCHAR * ) * ( GLINUX.Device.Caps.ExtensionNum + 1 ) ) ;
            j = 0 ;
            l = 0 ;
            lExtensionsTemp = lExtensions ;
            for(;;)
            {
                GLchar *TempBuffer ;

                if( lExtensionsTemp[ j ] == '\0' )
                {
                    break ;
                }

                for( k = 0; lExtensionsTemp[ j + k ] != ' ' && lExtensionsTemp[ j + k ] != '\0'; k++ ){}
                TempBuffer = ( GLchar * )DXALLOC( sizeof( GLchar ) * ( k + 1 ) ) ;
                _MEMCPY( TempBuffer, &lExtensionsTemp[ j ], sizeof( GLchar ) * k ) ;
                TempBuffer[ k ] = 0 ;
                GLINUX.Device.Caps.ExtensionNames[ l ] = ( TCHAR * )DXALLOC( sizeof( TCHAR ) * ( k + 1 ) ) ;
                ConvString( TempBuffer, -1, DX_CHARCODEFORMAT_ASCII, GLINUX.Device.Caps.ExtensionNames[ l ], sizeof( TCHAR ) * ( k + 1 ), _TCHARCODEFORMAT ) ;
                l ++ ;
                DXFREE( TempBuffer ) ;
                TempBuffer = NULL ;

                j += k ;
                if( lExtensionsTemp[ j ] == '\0' )
                {
                    break;
                }

                j++ ;
            }
            GLINUX.Device.Caps.ExtensionNames[ l ] = NULL ;
        }
	}

	// シェーダー関係の能力取得
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS,									&GLINUX.Device.Caps.Shader_Max_Vertex_Attribs ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( "\x4d\x00\x61\x00\x78\x00\x20\x00\x56\x00\x65\x00\x72\x00\x74\x00\x65\x00\x78\x00\x20\x00\x41\x00\x74\x00\x74\x00\x72\x00\x69\x00\x62\x00\x73\x00\x1a\xff\x25\x00\x64\x00\x00"/*@ L"Max Vertex Attribs：%d" @*/,				 GLINUX.Device.Caps.Shader_Max_Vertex_Attribs )) ;
	glGetIntegerv( GL_MAX_VERTEX_UNIFORM_VECTORS,							&GLINUX.Device.Caps.Shader_Max_Vertex_Uniform_Vectors ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( "\x4d\x00\x61\x00\x78\x00\x20\x00\x56\x00\x65\x00\x72\x00\x74\x00\x65\x00\x78\x00\x20\x00\x55\x00\x6e\x00\x69\x00\x66\x00\x6f\x00\x72\x00\x6d\x00\x20\x00\x56\x00\x65\x00\x63\x00\x74\x00\x6f\x00\x72\x00\x73\x00\x1a\xff\x25\x00\x64\x00\x00"/*@ L"Max Vertex Uniform Vectors：%d" @*/,		 GLINUX.Device.Caps.Shader_Max_Vertex_Uniform_Vectors )) ;
	glGetIntegerv( GL_MAX_VARYING_VECTORS,									&GLINUX.Device.Caps.Shader_Max_Varying_Vectors ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( "\x4d\x00\x61\x00\x78\x00\x20\x00\x56\x00\x61\x00\x72\x00\x79\x00\x69\x00\x6e\x00\x67\x00\x20\x00\x56\x00\x65\x00\x63\x00\x74\x00\x6f\x00\x72\x00\x73\x00\x1a\xff\x25\x00\x64\x00\x00"/*@ L"Max Varying Vectors：%d" @*/,				 GLINUX.Device.Caps.Shader_Max_Varying_Vectors )) ;
	glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,						&GLINUX.Device.Caps.Shader_Max_Combined_Texture_Image_Units ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( "\x4d\x00\x61\x00\x78\x00\x20\x00\x43\x00\x6f\x00\x6d\x00\x62\x00\x69\x00\x6e\x00\x65\x00\x64\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x49\x00\x6d\x00\x61\x00\x67\x00\x65\x00\x20\x00\x55\x00\x6e\x00\x69\x00\x74\x00\x73\x00\x1a\xff\x25\x00\x64\x00\x00"/*@ L"Max Combined Texture Image Units：%d" @*/,	 GLINUX.Device.Caps.Shader_Max_Combined_Texture_Image_Units )) ;
	glGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,						&GLINUX.Device.Caps.Shader_Max_Vertex_Texture_Image_Units ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( "\x4d\x00\x61\x00\x78\x00\x20\x00\x56\x00\x65\x00\x72\x00\x74\x00\x65\x00\x78\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x49\x00\x6d\x00\x61\x00\x67\x00\x65\x00\x20\x00\x55\x00\x6e\x00\x69\x00\x74\x00\x73\x00\x1a\xff\x25\x00\x64\x00\x00"/*@ L"Max Vertex Texture Image Units：%d" @*/,	 GLINUX.Device.Caps.Shader_Max_Vertex_Texture_Image_Units )) ;
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS,								&GLINUX.Device.Caps.Shader_Max_Texture_Image_Units ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( "\x4d\x00\x61\x00\x78\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x49\x00\x6d\x00\x61\x00\x67\x00\x65\x00\x20\x00\x55\x00\x6e\x00\x69\x00\x74\x00\x73\x00\x1a\xff\x25\x00\x64\x00\x00"/*@ L"Max Texture Image Units：%d" @*/,			 GLINUX.Device.Caps.Shader_Max_Texture_Image_Units )) ;
	glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_VECTORS,							&GLINUX.Device.Caps.Shader_Max_Fragment_Uniform_Vectors ) ;	
	DXST_LOGFILEFMT_ADDUTF16LE(( "\x4d\x00\x61\x00\x78\x00\x20\x00\x46\x00\x72\x00\x61\x00\x67\x00\x6d\x00\x65\x00\x6e\x00\x74\x00\x20\x00\x55\x00\x6e\x00\x69\x00\x66\x00\x6f\x00\x72\x00\x6d\x00\x20\x00\x56\x00\x65\x00\x63\x00\x74\x00\x6f\x00\x72\x00\x73\x00\x1a\xff\x25\x00\x64\x00\x00"/*@ L"Max Fragment Uniform Vectors：%d" @*/,		 GLINUX.Device.Caps.Shader_Max_Fragment_Uniform_Vectors )) ;

	// RGBのブレンド設定とは別にアルファチャンネルのブレンド設定が行える
	GSYS.HardInfo.SeparateAlphaBlendEnable = TRUE ;

	// 減算ブレンドが可能かどうかをセットする
	GSYS.HardInfo.ValidSubBlend = TRUE ;

	// 画面のＹ軸が上下反対
	GSYS.HardInfo.ScreenAxisYReverse = TRUE ;

	// DxLib_Init 呼出し後でも SetWaitVSyncFlag の呼び出しに対応
	GSYS.HardInfo.UseOfSetWaitVSyncFlagIsSupportedEvenAfterDxLib_Init = TRUE ;

	// 画面モードの変更ではサブバックバッファの変更のみ行うようにする
	GSYS.HardInfo.ChangeGraphModeOnlyChangeSubBackbuffer = TRUE ;

	// テクスチャが正方形である必要があるかどうかを取得する
	GSYS.HardInfo.TextureSquareOnly = FALSE ;

	// 一度に使用することの出来る最大頂点数を取得する
	GSYS.HardInfo.MaxVertexIndex = 0xffff ;

	// 頂点座標タイプは DX_DRAWFLOATCOORDTYPE_DIRECT3D10
	GSYS.HardInfo.DrawFloatCoordType = DX_DRAWFLOATCOORDTYPE_DIRECT3D10 ;

	// 同時にレンダリングできるターゲットの数を保存
	GSYS.HardInfo.RenderTargetNum = 8 ;

	// テクスチャの最大幅と最大高さを保存する
	{
		GLint MaxTextureSize ;

		glGetIntegerv( GL_MAX_TEXTURE_SIZE, &MaxTextureSize ) ;
		GSYS.HardInfo.MaxTextureWidth  = MaxTextureSize ;
		GSYS.HardInfo.MaxTextureHeight = MaxTextureSize ;
		GSYS.HardInfo.MaxTextureSize = MaxTextureSize ;
	}

	// テクスチャのサイズが２のｎ乗である必要は無い
	GSYS.HardInfo.TextureSizePow2 = FALSE ;

	// 条件付でテクスチャのサイズが２のｎ乗ではなくても大丈夫
	GSYS.HardInfo.TextureSizeNonPow2Conditional = TRUE ;

	// 頂点カラーはRGBAカラータイプを使用する
	GSYS.HardInfo.UseVertexColorBGRAFormat = FALSE ;

	// 一度に描画できる最大プリミティブ数を取得する
	GSYS.HardInfo.MaxPrimitiveCount = 65536 ;

	// Graphics_Linux_DrawPreparation を行うべきフラグを立てる
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 各種フォーマットをセット
	{
		GLINUX.Device.Caps.ScreenFormat = LINUX_PIXEL_FORMAT_R8G8B8A8 ;

		// パレットテクスチャは未対応
		if( FALSE )
//		if( GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_COMPRESSED_PALETTED_TEXTURE ] )
		{
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_PAL4                  ] = LINUX_PIXEL_FORMAT_R8G8B8_PAL4 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_PAL8                  ] = LINUX_PIXEL_FORMAT_R8G8B8_PAL8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL4            ] = LINUX_PIXEL_FORMAT_R8G8B8A8_PAL4 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL8            ] = LINUX_PIXEL_FORMAT_R8G8B8A8_PAL8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL4        ] = LINUX_PIXEL_FORMAT_A1B5G5R5_PAL4 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL8        ] = LINUX_PIXEL_FORMAT_A1B5G5R5_PAL8 ;
		}
		else
		{
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_PAL4                  ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_PAL8                  ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL4            ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL8            ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL4        ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL8        ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		}
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_RGB16                 ] = LINUX_PIXEL_FORMAT_B5G6R5 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_RGB32                 ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16           ] = LINUX_PIXEL_FORMAT_A4B4G4R4 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32           ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16       ] = LINUX_PIXEL_FORMAT_A1B5G5R5 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32       ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DXT1                  ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DXT2                  ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DXT3                  ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DXT4                  ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DXT5                  ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_BC7_UNORM             ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_BC7_UNORM_SRGB        ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM0             ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM1             ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM2             ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM3             ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16              ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;

		if( GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_HALF_FLOAT ] || GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_HALF_FLOAT_LINEAR ] )
		{
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16              ] = LINUX_PIXEL_FORMAT_R16G16B16A16_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16               ] = LINUX_PIXEL_FORMAT_R16G16B16_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16               ] = LINUX_PIXEL_FORMAT_R16G16B16_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16    ] = LINUX_PIXEL_FORMAT_R16G16B16A16_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16     ] = LINUX_PIXEL_FORMAT_R16G16B16_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16     ] = LINUX_PIXEL_FORMAT_R16G16B16_FLOAT ;
		}
		else
		if( GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_FLOAT ] || GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_FLOAT_LINEAR ] )
		{
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16              ] = LINUX_PIXEL_FORMAT_R32G32B32A32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16               ] = LINUX_PIXEL_FORMAT_R32G32B32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16               ] = LINUX_PIXEL_FORMAT_R32G32B32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16    ] = LINUX_PIXEL_FORMAT_R32G32B32A32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16     ] = LINUX_PIXEL_FORMAT_R32G32B32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16     ] = LINUX_PIXEL_FORMAT_R32G32B32_FLOAT ;
		}
		else
		{
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16              ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16               ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16               ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16    ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16     ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16     ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		}

		if( GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_FLOAT ] || GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_FLOAT_LINEAR ] )
		{
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32              ] = LINUX_PIXEL_FORMAT_R32G32B32A32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32               ] = LINUX_PIXEL_FORMAT_R32G32B32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32               ] = LINUX_PIXEL_FORMAT_R32G32B32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32    ] = LINUX_PIXEL_FORMAT_R32G32B32A32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32     ] = LINUX_PIXEL_FORMAT_R32G32B32_FLOAT ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32     ] = LINUX_PIXEL_FORMAT_R32G32B32_FLOAT ;
		}
		else
		{
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32              ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32               ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32               ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32    ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32     ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
			GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32     ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		}

		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8                ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16               ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8                ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16               ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16       ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32       ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16    ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8      ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16     ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8      ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;
		GLINUX.Device.Caps.TextureFormat[      DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16     ] = LINUX_PIXEL_FORMAT_R8G8B8A8 ;

		GLINUX.Device.Caps.MaskAlphaFormat = LINUX_PIXEL_FORMAT_R8 ;
		GLINUX.Device.Caps.DepthBufferFormat[ ZBUFFER_FORMAT_16BIT ] = GL_DEPTH_COMPONENT16 ;
		GLINUX.Device.Caps.DepthBufferFormat[ ZBUFFER_FORMAT_24BIT ] = GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_DEPTH24 ] ? GL_DEPTH_COMPONENT24_OES : GL_DEPTH_COMPONENT16 ;
		GLINUX.Device.Caps.DepthBufferFormat[ ZBUFFER_FORMAT_32BIT ] = GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_DEPTH32 ] ? GL_DEPTH_COMPONENT32_OES : ( GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_DEPTH24 ] ? GL_DEPTH_COMPONENT24_OES : GL_DEPTH_COMPONENT16 ) ;
	}

	// カラーフォーマット情報の初期化を行う
	Graphics_Linux_GetDataFormatColorData( LINUX_PIXEL_FORMAT_R8G8B8A8 ) ;

	// サブバックバッファの作成
	if( Graphics_Linux_SetupSubBackBuffer() < 0 )
	{
		return -1 ;
	}

	// 終了
	return 0 ;
}

// デバイスの破棄に相当する処理を行う
extern	int		Graphics_Linux_Device_Delete( void )
{
	// サブバックバッファ関係の後始末
	Graphics_Linux_TerminateSubBackBuffer() ;

	if( GLINUX.Device.Screen.Display != EGL_NO_DISPLAY ) 
	{
		eglMakeCurrent( GLINUX.Device.Screen.Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT ) ;

		if( GLINUX.Device.Screen.Context != EGL_NO_CONTEXT )
		{
			eglDestroyContext( GLINUX.Device.Screen.Display, GLINUX.Device.Screen.Context ) ;
		}

		if( GLINUX.Device.Screen.Surface != EGL_NO_SURFACE )
		{
			eglDestroySurface( GLINUX.Device.Screen.Display, GLINUX.Device.Screen.Surface ) ;
		}

		eglTerminate( GLINUX.Device.Screen.Display ) ;
	}

	g_LinuxSys.SoftThreadWait = 0 ;
	GLINUX.Device.Screen.Display = EGL_NO_DISPLAY ;
	GLINUX.Device.Screen.Context = EGL_NO_CONTEXT ;
	GLINUX.Device.Screen.Surface = EGL_NO_SURFACE ;

	// メモリの解放
	if( GLINUX.Device.Caps.ExtensionNames != NULL )
	{
		int i ;

		for( i = 0 ; i < GLINUX.Device.Caps.ExtensionNum ; i ++ )
		{
			if( GLINUX.Device.Caps.ExtensionNames[ i ] != NULL )
			{
				DXFREE( GLINUX.Device.Caps.ExtensionNames[ i ] ) ;
				GLINUX.Device.Caps.ExtensionNames[ i ] = NULL ;
			}
		}
		DXFREE( GLINUX.Device.Caps.ExtensionNames ) ;
		GLINUX.Device.Caps.ExtensionNames = NULL ;
	}
	if( GLINUX.Device.Caps.Version != NULL )
	{
		DXFREE( GLINUX.Device.Caps.Version ) ;
		GLINUX.Device.Caps.Version = NULL ;
	}
	if( GLINUX.Device.Caps.Renderer != NULL )
	{
		DXFREE( GLINUX.Device.Caps.Renderer ) ;
		GLINUX.Device.Caps.Renderer = NULL ;
	}
	if( GLINUX.Device.Caps.Vendor != NULL )
	{
		DXFREE( GLINUX.Device.Caps.Vendor ) ;
		GLINUX.Device.Caps.Vendor = NULL ;
	}
	if( GLINUX.Device.Caps.Shading_Language_Version != NULL )
	{
		DXFREE( GLINUX.Device.Caps.Shading_Language_Version ) ;
		GLINUX.Device.Caps.Shading_Language_Version = NULL ;
	}

	// デバイスセット情報を初期化
	_MEMSET( GLINUX.Device.State.SetTexture, 0, sizeof( GLINUX.Device.State.SetTexture ) ) ;
	GLINUX.Device.State.SetNormalShader = FALSE ;
	GLINUX.Device.State.SetShader = NULL ;
	GLINUX.Device.State.SetShader_Force = NULL ;
	GLINUX.Device.State.TargetFrameBuffer = 0 ;
	GLINUX.Device.State.TargetFrameBufferWidth = 0 ;
	GLINUX.Device.State.TargetFrameBufferHeight = 0 ;
	GLINUX.Device.State.SetVertexBuffer = 0 ;

	if( GLINUX.Device.Screen._clipboard_set_bufPtr != NULL )
	{
		DXFREE( GLINUX.Device.Screen._clipboard_set_bufPtr ) ;
		GLINUX.Device.Screen._clipboard_set_bufPtr = NULL ;
		GLINUX.Device.Screen._clipboard_set_bufLen = 0 ;
	}
	if( GLINUX.Device.Screen.XDisplay != NULL )
	{
		XCloseDisplay( GLINUX.Device.Screen.XDisplay ) ;
		GLINUX.Device.Screen.XDisplay = NULL ;
	}

	return 0 ;
}

// Direct3DDevice11 関係の初期化
extern	int		Graphics_Linux_Device_Initialize( void )
{
	int i ;

#ifndef DX_NON_FILTER
	GraphFilter_Initialize() ;
#endif // DX_NON_FILTER

	// 描画ステータスのセット
	{
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

		// 描画情報の初期値をセット
		{
			GLINUX.Device.State.CullEnable		= FALSE ;
			GLINUX.Device.State.CullFace			= GL_BACK ;
			GLINUX.Device.State.DepthBias		= 0 ;

			GLINUX.Device.State.DepthEnable		= FALSE ;
			GLINUX.Device.State.DepthWriteEnable	= FALSE ;
			GLINUX.Device.State.DepthFunc		= GL_LEQUAL ;

			GLINUX.Device.State.BlendMode		= DX_BLENDMODE_NOBLEND ;

			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
			{
				GLINUX.Device.State.TexMagFilter[ i ]		= GL_NEAREST ;
				GLINUX.Device.State.TexMinFilter[ i ]		= GL_NEAREST ;
				GLINUX.Device.State.TexAddressModeU[ i ]		= GL_CLAMP_TO_EDGE ;
				GLINUX.Device.State.TexAddressModeV[ i ]		= GL_CLAMP_TO_EDGE ;
				GLINUX.Device.State.TexAddressModeW[ i ]		= GL_CLAMP_TO_EDGE ;
				GLINUX.Device.State.MaxAnisotropyDim[ i ]	= 0 ;
			}
		}

		// 描画モードをセット
		Graphics_Linux_DeviceState_SetDrawMode( DX_DRAWMODE_NEAREST ) ;

		// 最大異方性をセット
		Graphics_Linux_DeviceState_SetMaxAnisotropy( 16 ) ;

		// テクスチャ座標変換行列を使用するかどうかの設定をセット
		Graphics_Linux_DeviceState_SetTextureAddressTransformMatrix(
			FALSE,
			&LINUX_GlobalIdentMatrix ) ;

		// ライティングＯＮ
		Graphics_Linux_DeviceState_SetLighting( TRUE ) ;

		// マテリアルのパラメータを設定
		{
			MATERIALPARAM Material ;
			Material.Diffuse.r  = 0.8f ;
			Material.Diffuse.g  = 0.8f ;
			Material.Diffuse.b  = 0.8f ;
			Material.Diffuse.a  = 1.0f ;
			Material.Specular.r = 0.8f ;
			Material.Specular.g = 0.8f ;
			Material.Specular.b = 0.8f ;
			Material.Specular.a = 0.0f ;
			Material.Emissive.r = 0.0f ;
			Material.Emissive.g = 0.0f ;
			Material.Emissive.b = 0.0f ;
			Material.Emissive.a = 0.0f ;
			Material.Ambient.r  = 0.0f ;
			Material.Ambient.g  = 0.0f ;
			Material.Ambient.b  = 0.0f ;
			Material.Ambient.a  = 0.0f ;
			Material.Power      = 20.0f ;
			Graphics_Linux_DeviceState_SetMaterial( &Material ) ;
		}

		// 頂点のカラー値をマテリアルのディフューズカラーやスペキュラカラーとして使用するかどうかの設定を初期化
		Graphics_Linux_DeviceState_SetUseVertexDiffuseColor( TRUE ) ;
		Graphics_Linux_DeviceState_SetUseVertexSpecularColor( TRUE ) ;

		// スペキュラ処理を使用するかどうかを初期化
		Graphics_Linux_DeviceState_SetSpecularEnable( TRUE ) ;

		// フォグ関係の設定
		Graphics_Linux_DeviceState_SetFogEnable( FALSE ) ;
		Graphics_Linux_DeviceState_SetFogColor( 0 ) ;
		Graphics_Linux_DeviceState_SetFogVertexMode( DX_FOGMODE_NONE ) ;
		Graphics_Linux_DeviceState_SetFogStartEnd( 0.0f, 1.0f ) ;
		Graphics_Linux_DeviceState_SetFogDensity( 1.0f ) ;

		// グローバルアンビエントカラーセット
		{
			COLOR_F GAmbColor = { 0.0f } ;

			Graphics_Linux_DeviceState_SetAmbient( &GAmbColor ) ;
		}

		// ビューポートを設定
		{
			RECT Viewport ;

			Viewport.left		= 0 ;
			Viewport.top		= 0 ;
			Viewport.right		= ( float )GSYS.Screen.MainScreenSizeX ;
			Viewport.bottom		= ( float )GSYS.Screen.MainScreenSizeY ;
			Graphics_Linux_DeviceState_SetViewport( &Viewport ) ;
		}

		// 描画画像のＲＧＢを無視するかどうかをセット
		Graphics_Linux_DrawSetting_SetIgnoreDrawGraphColor( FALSE ) ;

		// 描画画像のＡを無視するかどうかをセット
		Graphics_Linux_DrawSetting_SetIgnoreDrawGraphAlpha( FALSE ) ;

		// 描画先のアルファチャンネルの内容を書き換えるかを設定する
		Graphics_Linux_DrawSetting_SetWriteAlphaChannelFlag( FALSE ) ;

		// 描画先設定
//		Graphics_Linux_DeviceState_SetRenderTarget(
//			&GLINUX.Device.Screen.DispBuffer[ GLINUX.Device.Screen.BackBufferIndex ].RenderTarget
//		) ;

		// ブレンドモード設定
		Graphics_Linux_DeviceState_SetBlendMode( DX_BLENDMODE_NOBLEND, FALSE, DX_BLEND_ZERO, DX_BLEND_ZERO, DX_BLENDOP_ADD, DX_BLEND_ZERO, DX_BLEND_ZERO, DX_BLENDOP_ADD, FALSE ) ;

		// アルファテスト設定を初期化
		Graphics_Linux_DrawSetting_SetDrawAlphaTest( -1, 0 ) ;

		// ブレンドテクスチャ関係を初期化
		Graphics_Linux_DrawSetting_SetBlendTexture( NULL ) ;
		GLINUX.Device.DrawSetting.BlendGraphBorderParam	= -1 ;
		GLINUX.Device.DrawSetting.BlendGraphType			= 0 ;

		// 描画テクスチャ関係を初期化
		Graphics_Linux_DrawSetting_SetTexture( NULL ) ;
		GLINUX.Device.DrawSetting.AlphaChannelValidFlag	= FALSE ;
		GLINUX.Device.DrawSetting.AlphaTestValidFlag		= FALSE ;

		GLINUX.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;
	}

	// デバイス設定をリフレッシュ
	Graphics_Linux_DeviceState_RefreshRenderState() ;

	// フラグを初期化
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag		= TRUE ;
	GLINUX.Device.DrawSetting.AlphaChannelValidFlag	= FALSE ;
	GLINUX.Device.DrawSetting.AlphaTestValidFlag		= FALSE ;
	GLINUX.Device.DrawSetting.AlphaTestMode			= -1 ;
	GLINUX.Device.DrawSetting.AlphaTestParam			= 0 ;
	GLINUX.Device.DrawSetting.BlendMode				= DX_BLENDMODE_NOBLEND ;
	GLINUX.Device.DrawSetting.ChangeBlendParamFlag	= TRUE ;
	GLINUX.Device.DrawSetting.ChangeTextureFlag		= FALSE ;
	GLINUX.Device.DrawSetting.IgnoreGraphColorFlag	= FALSE ;
	GLINUX.Device.DrawSetting.BlendGraphType			= 0 ;
	GLINUX.Device.DrawSetting.BlendGraphBorderParam	= -1 ;
	GLINUX.Device.DrawSetting.RenderTexture			= NULL ;
	GLINUX.Device.DrawSetting.BlendTexture			= NULL ;

	// 描画情報を初期化
	GLINUX.Device.DrawInfo.DiffuseColor				= 0xffffffff ;
	GLINUX.Device.DrawInfo.BlendMaxNotDrawFlag		= FALSE ;
	GLINUX.Device.DrawInfo.BeginSceneFlag			= FALSE ;
	GLINUX.Device.DrawInfo.VertexNum					= 0 ;
	GLINUX.Device.DrawInfo.Use3DVertex				= 0 ;
	GLINUX.Device.DrawInfo.VertexType				= VERTEXTYPE_TEX ;
	GLINUX.Device.DrawInfo.PrimitiveType				= GL_TRIANGLES ;

	// 頂点バッファのアドレスをセット
	GLINUX.Device.DrawInfo.VertexBufferNextAddr = GLINUX.Device.DrawInfo.VertexBuffer ;

	// 終了
	return 0 ;
}

// 一時的に Direct3DDevice11 を破棄した際に、破棄前の状態を作り出すための関数
extern	int		Graphics_Linux_Device_ReInitialize( void )
{
	int i ;

	// 初期化フラグを立てる
	GLINUX.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

	// 必ず Graphics_D3D9_DrawPreparation を実行するフラグを立てる
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 削除前と同じ環境を作り出す
	{
		// 描画テクスチャ関係を初期化
		Graphics_Linux_DrawSetting_SetTexture( NULL ) ;
		GLINUX.Device.DrawSetting.AlphaChannelValidFlag	= FALSE ;
		GLINUX.Device.DrawSetting.AlphaTestValidFlag	= FALSE ;

		// 描画頂点情報もリセット
		GLINUX.Device.DrawInfo.VertexNum = 0 ;

		// 描画ブレンドモードの設定
		Graphics_Linux_DrawSetting_SetDrawBlendMode(
			GLINUX.Device.DrawSetting.BlendMode,
			GLINUX.Device.DrawSetting.BlendEnable,
			GLINUX.Device.DrawSetting.BlendRGBSrc,
			GLINUX.Device.DrawSetting.BlendRGBDest,
			GLINUX.Device.DrawSetting.BlendRGBOp,
			GLINUX.Device.DrawSetting.BlendASrc,
			GLINUX.Device.DrawSetting.BlendADest,
			GLINUX.Device.DrawSetting.BlendAOp,
			GLINUX.Device.DrawSetting.AlphaTestValidFlag,
			GLINUX.Device.DrawSetting.AlphaChannelValidFlag
		) ;

		// 描画画像のＲＧＢを無視するかどうかをセットする
		Graphics_Linux_DrawSetting_SetIgnoreDrawGraphColor( GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ) ;

		// 描画画像のＡを無視するかどうかをセットする
		Graphics_Linux_DrawSetting_SetIgnoreDrawGraphAlpha( GLINUX.Device.DrawSetting.IgnoreGraphAlphaFlag ) ;

		// 描画先のアルファチャンネルの内容を書き換えるかを設定する
		Graphics_Linux_DrawSetting_SetWriteAlphaChannelFlag( GLINUX.Device.DrawSetting.NotWriteAlphaChannelFlag ) ;

		// 異方性フィルタリングの設定をセット
		Graphics_Linux_DeviceState_SetMaxAnisotropy( GLINUX.Device.State.MaxAnisotropy ) ;

		// UVアドレッシングモードの設定をセット
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			Graphics_Linux_DeviceState_SetTextureAddressUVW(
				GLES2WrapModeToDXTexAddrMode( GLINUX.Device.State.TexAddressModeU[ i ] ),
				GLES2WrapModeToDXTexAddrMode( GLINUX.Device.State.TexAddressModeV[ i ] ),
				GLES2WrapModeToDXTexAddrMode( GLINUX.Device.State.TexAddressModeW[ i ] ),
				i ) ;
		}

		// テクスチャ座標変換行列を使用するかどうかの設定をセット
		Graphics_Linux_DeviceState_SetTextureAddressTransformMatrix(
			GLINUX.Device.State.TextureAddressTransformMatrixUse,
			&GLINUX.Device.State.TextureAddressTransformMatrix ) ;

		// フォグの設定をセット
		Graphics_Linux_DeviceState_SetFogEnable(     GLINUX.Device.State.FogEnable ) ;
		Graphics_Linux_DeviceState_SetFogVertexMode( GLINUX.Device.State.FogMode ) ;
		Graphics_Linux_DeviceState_SetFogColor(      GLINUX.Device.State.FogColor ) ;
		Graphics_Linux_DeviceState_SetFogStartEnd(   GLINUX.Device.State.FogStart, GLINUX.Device.State.FogEnd ) ;
		Graphics_Linux_DeviceState_SetFogDensity(    GLINUX.Device.State.FogDensity ) ;

		// 行列の再設定
		NS_SetTransformToWorldD(                              &GSYS.DrawSetting.WorldMatrix ) ;
		NS_SetTransformToViewD(                               &GSYS.DrawSetting.ViewMatrix ) ;
		Graphics_DrawSetting_SetTransformToProjection_Direct( &GSYS.DrawSetting.ProjectionMatrix ) ;
		NS_SetTransformToViewportD(                           &GSYS.DrawSetting.ViewportMatrix ) ;

		// ライトの再設定
		Graphics_Linux_DeviceState_SetAmbient( &GLINUX.Device.State.GlobalAmbientLightColor ) ;
		Graphics_Linux_DeviceState_SetLighting( GLINUX.Device.State.Lighting ) ;
		for( i = 0 ; i < DX_LINUX_COMMON_CONST_LIGHT_NUM ; i ++ )
		{
			if( GLINUX.Device.State.LightParam[ i ].LightType == 0 )
			{
				continue ;
			}

			Graphics_Linux_DeviceState_SetLightState( i, &GLINUX.Device.State.LightParam[ i ] ) ;
			Graphics_Linux_DeviceState_SetLightEnable( i, GLINUX.Device.State.LightEnableFlag[ i ] ) ;
		}
	}

	// 初期化フラグを倒す
	GLINUX.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// 上記以外の設定をリフレッシュ
	Graphics_Linux_DeviceState_RefreshRenderState() ;

	// 終了
	return 0 ;
}
























// LinuxDevice ステータス関係

// 描画設定をリフレッシュ
extern	void	Graphics_Linux_DeviceState_RefreshRenderState( void )
{
	RECT Viewport = GLINUX.Device.State.ViewportRect ;

	// 初期化フラグを立てる
	GLINUX.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

	// Graphics_Linux_DrawPreparation を行うべきフラグを立てる
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 設定のリフレッシュ
	Graphics_Linux_DeviceState_SetFogEnable             ( GLINUX.Device.State.FogEnable           ) ;
//	Graphics_Linux_DeviceState_SetScissorRect           ( &GLINUX.Device.State.ScissorRect        ) ;
	Graphics_Linux_DrawSetting_SetTexture               ( GLINUX.Device.DrawSetting.RenderTexture ) ;
	Graphics_Linux_DrawSetting_SetBlendTexture          ( GLINUX.Device.DrawSetting.BlendTexture ) ;
	Graphics_Linux_DeviceState_SetBaseState             (                                       ) ;
	Graphics_Linux_DeviceState_SetDrawMode              ( GLINUX.Device.State.DrawMode            ) ;
//	Graphics_Linux_DrawSetting_SetDrawBlendMode         ( GLINUX.Device.DrawSetting.BlendMode, GLINUX.Device.DrawSetting.AlphaChannelValidFlag  ) ;
	Graphics_Linux_DeviceState_NormalDrawSetup          (                                       ) ;
	Graphics_Linux_DeviceState_SetDepthEnable           ( GLINUX.Device.State.DepthEnable         ) ;
	Graphics_Linux_DeviceState_SetDepthWriteEnable      ( GLINUX.Device.State.DepthWriteEnable    ) ;
	Graphics_Linux_DeviceState_SetDepthFunc             ( GLINUX.Device.State.DepthFunc           ) ;
	Graphics_Linux_DeviceState_SetDepthBias             ( GLINUX.Device.State.DepthBias           ) ;
	Graphics_Linux_DeviceState_SetCullMode              ( GLINUX.Device.State.CullEnable == FALSE ? 0 : ( GLINUX.Device.State.CullFace == GL_FRONT ? 1 : 2 ) ) ;
//	Graphics_Linux_DeviceState_SetShadeMode             ( GLINUX.Device.State.ShadeMode           ) ;
	Graphics_Linux_DeviceState_SetMaterial              ( &GLINUX.Device.State.Material           ) ;
	Graphics_Linux_DeviceState_SetSpecularEnable        ( GLINUX.Device.State.UseSpecular         ) ;
	Graphics_Linux_DeviceState_SetUseVertexDiffuseColor ( GLINUX.Device.State.MaterialUseVertexDiffuseColor ) ;
	Graphics_Linux_DeviceState_SetUseVertexSpecularColor( GLINUX.Device.State.MaterialUseVertexSpecularColor ) ;
	Graphics_Linux_DeviceState_SetShader                ( GLINUX.Device.State.SetShader, GLINUX.Device.State.SetNormalShader ) ;
	Graphics_Linux_DeviceState_SetRenderTarget          ( GLINUX.Device.State.TargetFrameBuffer, GLINUX.Device.State.TargetFrameBufferWidth, GLINUX.Device.State.TargetFrameBufferHeight ) ;
	Graphics_Linux_DeviceState_SetViewport              ( &Viewport ) ;
	Graphics_Linux_DeviceState_SetBackgroundColor       ( GLINUX.Device.State.BackgroundColor.r, GLINUX.Device.State.BackgroundColor.g, GLINUX.Device.State.BackgroundColor.b, GLINUX.Device.State.BackgroundColor.a ) ;

	// テクスチャをセット
	{
		int i ;

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			Graphics_Linux_DeviceState_SetTexture( i, GLINUX.Device.State.SetTexture[ i ] ) ;
		}
	}

	// 定数バッファをリフレッシュ
	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 初期化フラグを倒す
	GLINUX.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;
}

// OpenGL の基本設定を行う
extern int Graphics_Linux_DeviceState_SetBaseState( void )
{
	// 書き出し
	DRAWSTOCKINFO

	// 終了
	return 0 ;
}

// サンプラーのテクスチャフィルタリングモードを設定する
extern int Graphics_Linux_DeviceState_SetSampleFilterMode( GLenum Filter, int Sampler )
{
	int i ;
	GLenum MagFilter ;
	GLenum MinFilter ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	MinFilter = Filter ;
	switch( Filter )
	{
	case GL_NEAREST :
	case GL_NEAREST_MIPMAP_NEAREST :
	case GL_NEAREST_MIPMAP_LINEAR :
		MagFilter = GL_NEAREST ;
		break ;

	case GL_LINEAR :
	case GL_LINEAR_MIPMAP_NEAREST :
	case GL_LINEAR_MIPMAP_LINEAR :
		MagFilter = GL_LINEAR ;
		break ;

	default :
		return -1 ;
	}

	if( Sampler < 0 )
	{
		if( Filter == GL_NEAREST ||
			Filter == GL_LINEAR )
		{
			GLINUX.Device.State.DrawMode = -1 ;
		}

		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
			{
				if( ( DWORD )GLINUX.Device.State.TexMagFilter[ i ] != MagFilter ||
					( DWORD )GLINUX.Device.State.TexMinFilter[ i ] != MinFilter )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( ( DWORD )GLINUX.Device.State.TexMagFilter[ i ] == MagFilter &&
				( DWORD )GLINUX.Device.State.TexMinFilter[ i ] == MinFilter &&
				GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GLINUX.Device.State.TexMagFilter[ i ] = MagFilter ;
			GLINUX.Device.State.TexMinFilter[ i ] = MinFilter ;

			if( GLINUX.Device.State.SetTexture[ i ] != NULL )
			{
				glActiveTexture( g_TextureEnum[ i ] ) ;
				glBindTexture( GL_TEXTURE_2D, GLINUX.Device.State.SetTexture[ i ]->TextureBuffer ) ;
				// if( GLINUX.Device.State.SetTexture[ i ]->MagFilter != GLINUX.Device.State.TexMagFilter[ i ] )
				{
					GLINUX.Device.State.SetTexture[ i ]->MagFilter = GLINUX.Device.State.TexMagFilter[ i ] ;
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLINUX.Device.State.SetTexture[ i ]->MagFilter ) ;
				}
				// if( GLINUX.Device.State.SetTexture[ i ]->MinFilter != GLINUX.Device.State.TexMinFilter[ i ] )
				{
					GLenum MinFilterTmp = MinFilter ;
					if( GLINUX.Device.State.SetTexture[ i ]->MipMapCount <= 1 && MinFilter == GL_LINEAR_MIPMAP_LINEAR )
					{
						MinFilterTmp = GL_LINEAR ;
					}
					// if( GLINUX.Device.State.SetTexture[ i ]->MinFilter != MinFilterTmp )
					{
						GLINUX.Device.State.SetTexture[ i ]->MinFilter = MinFilterTmp ;
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLINUX.Device.State.SetTexture[ i ]->MinFilter ) ;
					}
				}
			}
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return 0 ;
		}

		GLINUX.Device.State.DrawMode = -1 ;

		if( ( DWORD )GLINUX.Device.State.TexMagFilter[ Sampler ] == MagFilter &&
			( DWORD )GLINUX.Device.State.TexMinFilter[ Sampler ] == MinFilter &&
			GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GLINUX.Device.State.TexMagFilter[ Sampler ] = MagFilter ;
		GLINUX.Device.State.TexMinFilter[ Sampler ] = MinFilter ;

		if( GLINUX.Device.State.SetTexture[ Sampler ] != NULL )
		{
			glActiveTexture( g_TextureEnum[ Sampler ] ) ;
			glBindTexture( GL_TEXTURE_2D, GLINUX.Device.State.SetTexture[ Sampler ]->TextureBuffer ) ;
			// if( GLINUX.Device.State.SetTexture[ Sampler ]->MagFilter != GLINUX.Device.State.TexMagFilter[ Sampler ] )
			{
				GLINUX.Device.State.SetTexture[ Sampler ]->MagFilter = GLINUX.Device.State.TexMagFilter[ Sampler ] ;
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLINUX.Device.State.SetTexture[ Sampler ]->MagFilter ) ;
			}
			// if( GLINUX.Device.State.SetTexture[ Sampler ]->MinFilter != GLINUX.Device.State.TexMinFilter[ Sampler ] )
			{
				GLenum MinFilterTmp = MinFilter ;
				if( GLINUX.Device.State.SetTexture[ Sampler ]->MipMapCount <= 1 && MinFilter == GL_LINEAR_MIPMAP_LINEAR )
				{
					MinFilterTmp = GL_LINEAR ;
				}
				// if( GLINUX.Device.State.SetTexture[ Sampler ]->MinFilter != MinFilterTmp )
				{
					GLINUX.Device.State.SetTexture[ Sampler ]->MinFilter = MinFilterTmp ;
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLINUX.Device.State.SetTexture[ Sampler ]->MinFilter ) ;
				}
			}
		}
	}

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// 深度バッファの有効無効のセット
extern int Graphics_Linux_DeviceState_SetDepthEnable( BOOL DepthEnable )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// モードが同じだったら何もせず終了
	if( GLINUX.Device.State.DepthEnable == DepthEnable &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GLINUX.Device.State.DepthEnable = DepthEnable ;

	if( DepthEnable )
	{
		glEnable( GL_DEPTH_TEST ) ;
	}
	else
	{
		glDisable( GL_DEPTH_TEST ) ;
	}

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// 深度バッファの書き込みの有無をセット
extern int Graphics_Linux_DeviceState_SetDepthWriteEnable( int Flag )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// モードが同じだったら何もせず終了
	if( GLINUX.Device.State.DepthWriteEnable == Flag &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GLINUX.Device.State.DepthWriteEnable = Flag ;

	glDepthMask( Flag ? GL_TRUE : GL_FALSE ) ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// 深度値の比較タイプをセット
extern int Graphics_Linux_DeviceState_SetDepthFunc( GLenum DepthFunc )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// モードが同じだったら何もせず終了
	if( GLINUX.Device.State.DepthFunc == DepthFunc &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GLINUX.Device.State.DepthFunc = DepthFunc ;

	glDepthFunc( DepthFunc ) ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// 深度値のバイアスをセット
extern int Graphics_Linux_DeviceState_SetDepthBias( int DepthBias )
{
	// モードが同じだったら何もせず終了
	if( GLINUX.Device.State.DepthBias == DepthBias &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GLINUX.Device.State.DepthBias = DepthBias ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// ワールド変換用行列をセットする
extern int Graphics_Linux_DeviceState_SetWorldMatrix( const MATRIX *Matrix )
{
	// 定数データに反映
	GLINUX.Device.Shader.Constant.uLWMat[ 0 ][ 0 ] = Matrix->m[ 0 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 0 ][ 1 ] = Matrix->m[ 1 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 0 ][ 2 ] = Matrix->m[ 2 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 0 ][ 3 ] = Matrix->m[ 3 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 1 ][ 0 ] = Matrix->m[ 0 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 1 ][ 1 ] = Matrix->m[ 1 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 1 ][ 2 ] = Matrix->m[ 2 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 1 ][ 3 ] = Matrix->m[ 3 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 2 ][ 0 ] = Matrix->m[ 0 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 2 ][ 1 ] = Matrix->m[ 1 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 2 ][ 2 ] = Matrix->m[ 2 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uLWMat[ 2 ][ 3 ] = Matrix->m[ 3 ][ 2 ] ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// ビュー変換用行列をセットする
extern int Graphics_Linux_DeviceState_SetViewMatrix( const MATRIX *Matrix )
{
	// ライトの位置・方向の再計算
	{
		int    i ;
		VECTOR Direction ;
		VECTOR Position ;

		for( i = 0 ; i < DX_LINUX_COMMON_CONST_LIGHT_NUM ; i ++ )
		{
			VectorTransformSR( &Direction, &GLINUX.Device.State.LightParam[ i ].Direction, Matrix ) ;
			VectorTransform(   &Position,  &GLINUX.Device.State.LightParam[ i ].Position,  Matrix ) ;
			GLINUX.Device.Shader.Constant.uLightPos[ i ][ 0 ]  = Position.x ;
			GLINUX.Device.Shader.Constant.uLightPos[ i ][ 1 ]  = Position.y ;
			GLINUX.Device.Shader.Constant.uLightPos[ i ][ 2 ]  = Position.z ;
			GLINUX.Device.Shader.Constant.uLightDir[ i ][ 0 ] = Direction.x ;
			GLINUX.Device.Shader.Constant.uLightDir[ i ][ 1 ] = Direction.y ;
			GLINUX.Device.Shader.Constant.uLightDir[ i ][ 2 ] = Direction.z ;
		}
	}

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uViewMat[ 0 ][ 0 ] = Matrix->m[ 0 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 0 ][ 1 ] = Matrix->m[ 1 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 0 ][ 2 ] = Matrix->m[ 2 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 0 ][ 3 ] = Matrix->m[ 3 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 1 ][ 0 ] = Matrix->m[ 0 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 1 ][ 1 ] = Matrix->m[ 1 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 1 ][ 2 ] = Matrix->m[ 2 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 1 ][ 3 ] = Matrix->m[ 3 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 2 ][ 0 ] = Matrix->m[ 0 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 2 ][ 1 ] = Matrix->m[ 1 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 2 ][ 2 ] = Matrix->m[ 2 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uViewMat[ 2 ][ 3 ] = Matrix->m[ 3 ][ 2 ] ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// 投影変換用行列をセットする
extern int Graphics_Linux_DeviceState_SetProjectionMatrix( const MATRIX *Matrix )
{
	// 定数データに反映
	GLINUX.Device.Shader.Constant.uProjMat[ 0 ][ 0 ] = Matrix->m[ 0 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 0 ][ 1 ] = Matrix->m[ 1 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 0 ][ 2 ] = Matrix->m[ 2 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 0 ][ 3 ] = Matrix->m[ 3 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 1 ][ 0 ] = Matrix->m[ 0 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 1 ][ 1 ] = Matrix->m[ 1 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 1 ][ 2 ] = Matrix->m[ 2 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 1 ][ 3 ] = Matrix->m[ 3 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 2 ][ 0 ] = Matrix->m[ 0 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 2 ][ 1 ] = Matrix->m[ 1 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 2 ][ 2 ] = Matrix->m[ 2 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 2 ][ 3 ] = Matrix->m[ 3 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 3 ][ 0 ] = Matrix->m[ 0 ][ 3 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 3 ][ 1 ] = Matrix->m[ 1 ][ 3 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 3 ][ 2 ] = Matrix->m[ 2 ][ 3 ] ;
	GLINUX.Device.Shader.Constant.uProjMat[ 3 ][ 3 ] = Matrix->m[ 3 ][ 3 ] ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// アンチビューポート行列をセットする
extern int Graphics_Linux_DeviceState_SetAntiViewportMatrix( const MATRIX *Matrix )
{
	// 定数データに反映
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 0 ][ 0 ] = Matrix->m[ 0 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 0 ][ 1 ] = Matrix->m[ 1 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 0 ][ 2 ] = Matrix->m[ 2 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 0 ][ 3 ] = Matrix->m[ 3 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 1 ][ 0 ] = Matrix->m[ 0 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 1 ][ 1 ] = Matrix->m[ 1 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 1 ][ 2 ] = Matrix->m[ 2 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 1 ][ 3 ] = Matrix->m[ 3 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 2 ][ 0 ] = Matrix->m[ 0 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 2 ][ 1 ] = Matrix->m[ 1 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 2 ][ 2 ] = Matrix->m[ 2 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 2 ][ 3 ] = Matrix->m[ 3 ][ 2 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 3 ][ 0 ] = Matrix->m[ 0 ][ 3 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 3 ][ 1 ] = Matrix->m[ 1 ][ 3 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 3 ][ 2 ] = Matrix->m[ 2 ][ 3 ] ;
	GLINUX.Device.Shader.Constant.uAntiVPMat[ 3 ][ 3 ] = Matrix->m[ 3 ][ 3 ] ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// アンビエントライトとエミッシブカラーを掛け合わせたパラメータを更新する
extern void Graphics_Linux_DeviceState_RefreshAmbientAndEmissiveParam( void )
{
	int i ;

	// 有効なライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したものをシェーダーにセットする
	for( i = 0 ; i <= GLINUX.Device.State.LightEnableMaxIndex ; i ++ )
	{
		if( GLINUX.Device.State.LightEnableFlag[ i ] == FALSE )
		{
			continue ;
		}

		// 定数バッファに反映する
		GLINUX.Device.Shader.Constant.uLightAmb[ i ][ 0 ] = GLINUX.Device.State.LightParam[ i ].Ambient.r * GLINUX.Device.State.Material.Ambient.r ;
		GLINUX.Device.Shader.Constant.uLightAmb[ i ][ 1 ] = GLINUX.Device.State.LightParam[ i ].Ambient.g * GLINUX.Device.State.Material.Ambient.g ;
		GLINUX.Device.Shader.Constant.uLightAmb[ i ][ 2 ] = GLINUX.Device.State.LightParam[ i ].Ambient.b * GLINUX.Device.State.Material.Ambient.b ;
		GLINUX.Device.Shader.Constant.uLightAmb[ i ][ 3 ] = GLINUX.Device.State.LightParam[ i ].Ambient.a * GLINUX.Device.State.Material.Ambient.a ;
	}

	// グローバルアンビエントライトカラーとマテリアルアンビエントカラーを乗算したものとエミッシブカラーを加算したものを作成する
	GLINUX.Device.Shader.Constant.uAmb_Emi[ 0 ] = GLINUX.Device.State.GlobalAmbientLightColor.r * GLINUX.Device.State.Material.Ambient.r + GLINUX.Device.State.Material.Emissive.r ;
	GLINUX.Device.Shader.Constant.uAmb_Emi[ 1 ] = GLINUX.Device.State.GlobalAmbientLightColor.g * GLINUX.Device.State.Material.Ambient.g + GLINUX.Device.State.Material.Emissive.g ;
	GLINUX.Device.Shader.Constant.uAmb_Emi[ 2 ] = GLINUX.Device.State.GlobalAmbientLightColor.b * GLINUX.Device.State.Material.Ambient.b + GLINUX.Device.State.Material.Emissive.b ;
	GLINUX.Device.Shader.Constant.uAmb_Emi[ 3 ] = GLINUX.Device.State.GlobalAmbientLightColor.a * GLINUX.Device.State.Material.Ambient.a + GLINUX.Device.State.Material.Emissive.a ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;
}



// ライトのセット
extern int Graphics_Linux_DeviceState_SetLightState( int LightNumber, LIGHTPARAM *Light )
{
	VECTOR  Direction ;
	VECTOR  Position ;
	float   ThetaSin ;
	float   ThetaCos ;
	float   PhiSin ;
	float   PhiCos ;

	if( LightNumber < 0 || LightNumber >= DX_LINUX_COMMON_CONST_LIGHT_NUM )
	{
		return -1 ;
	}

	if( _MEMCMP( &GLINUX.Device.State.LightParam[ LightNumber ], Light, sizeof( LIGHTPARAM ) ) == 0 &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// パラメータの保存
	GLINUX.Device.State.LightParam[ LightNumber ] = *Light ;

	// 定数データに反映
	_SINCOS( Light->Theta / 2.0f, &ThetaSin, &ThetaCos ) ;
	_SINCOS( Light->Phi   / 2.0f, &PhiSin,   &PhiCos ) ;

	VectorTransformSR( &Direction, &Light->Direction, &GSYS.DrawSetting.ViewMatrixF ) ;
	VectorTransform(   &Position,  &Light->Position,  &GSYS.DrawSetting.ViewMatrixF ) ;
	GLINUX.Device.Shader.Constant.uLightPos[ LightNumber ][ 0 ]						= Position.x ;
	GLINUX.Device.Shader.Constant.uLightPos[ LightNumber ][ 1 ]						= Position.y ;
	GLINUX.Device.Shader.Constant.uLightPos[ LightNumber ][ 2 ]						= Position.z ;
	GLINUX.Device.Shader.Constant.uLightDir[ LightNumber ][ 0 ]						= Direction.x ;
	GLINUX.Device.Shader.Constant.uLightDir[ LightNumber ][ 1 ]						= Direction.y ;
	GLINUX.Device.Shader.Constant.uLightDir[ LightNumber ][ 2 ]						= Direction.z ;
	GLINUX.Device.Shader.Constant.uLightDif[ LightNumber ][ 0 ]						= Light->Diffuse.r ;
	GLINUX.Device.Shader.Constant.uLightDif[ LightNumber ][ 1 ]						= Light->Diffuse.g ;
	GLINUX.Device.Shader.Constant.uLightDif[ LightNumber ][ 2 ]						= Light->Diffuse.b ;
	GLINUX.Device.Shader.Constant.uLightDif[ LightNumber ][ 3 ]						= Light->Diffuse.a ;
	GLINUX.Device.Shader.Constant.uLightSpc[ LightNumber ][ 0 ]						= Light->Specular.r ;
	GLINUX.Device.Shader.Constant.uLightSpc[ LightNumber ][ 1 ]						= Light->Specular.g ;
	GLINUX.Device.Shader.Constant.uLightSpc[ LightNumber ][ 2 ]						= Light->Specular.b ;
	GLINUX.Device.Shader.Constant.uLightSpc[ LightNumber ][ 3 ]						= Light->Specular.a ;
	GLINUX.Device.Shader.Constant.uLightAmb[ LightNumber ][ 0 ]						= Light->Ambient.r ;
	GLINUX.Device.Shader.Constant.uLightAmb[ LightNumber ][ 1 ]						= Light->Ambient.g ;
	GLINUX.Device.Shader.Constant.uLightAmb[ LightNumber ][ 2 ]						= Light->Ambient.b ;
	GLINUX.Device.Shader.Constant.uLightAmb[ LightNumber ][ 3 ]						= Light->Ambient.a ;
	GLINUX.Device.Shader.Constant.uLightRange_FallOff_AT0_AT1[ LightNumber ][ 0 ]	= Light->Range * Light->Range ;
	GLINUX.Device.Shader.Constant.uLightRange_FallOff_AT0_AT1[ LightNumber ][ 1 ]	= Light->Falloff ;
	GLINUX.Device.Shader.Constant.uLightRange_FallOff_AT0_AT1[ LightNumber ][ 2 ]	= Light->Attenuation0 ;
	GLINUX.Device.Shader.Constant.uLightRange_FallOff_AT0_AT1[ LightNumber ][ 3 ]	= Light->Attenuation1 ;
	GLINUX.Device.Shader.Constant.uLightAT2_SpotP0_SpotP1[ LightNumber ][ 0 ]		= Light->Attenuation2 ;
	GLINUX.Device.Shader.Constant.uLightAT2_SpotP0_SpotP1[ LightNumber ][ 1 ]		= PhiCos ;
	if( _FABS( ThetaCos - PhiCos ) < 0.0000001f )
	{
		GLINUX.Device.Shader.Constant.uLightAT2_SpotP0_SpotP1[ LightNumber ][ 2 ]    = 100000.0f ;
	}
	else
	{
		GLINUX.Device.Shader.Constant.uLightAT2_SpotP0_SpotP1[ LightNumber ][ 2 ]	= 1.0f / ( ThetaCos - PhiCos ) ;
	}
	GLINUX.Device.Shader.Constant.uLightAT2_SpotP0_SpotP1[ LightNumber ][ 3 ]		= 0.0f ;
	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// アンビエントカラーとエミッシブカラーを掛け合わせた情報を更新する
	Graphics_Linux_DeviceState_RefreshAmbientAndEmissiveParam() ;

	// 終了
	return 0 ;
}

// ライトの有効、無効を変更
extern int Graphics_Linux_DeviceState_SetLightEnable( int LightNumber, int EnableState )
{
	if( LightNumber < 0 || LightNumber >= DX_LINUX_COMMON_CONST_LIGHT_NUM )
	{
		return -1 ;
	}

	if( GLINUX.Device.State.LightEnableFlag[ LightNumber ] == EnableState &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GLINUX.Device.State.LightEnableFlag[ LightNumber ] = EnableState ;

	// 有効なライトの最大インデックスを更新
	if( EnableState == TRUE )
	{
		if( GLINUX.Device.State.LightEnableMaxIndex < LightNumber )
			GLINUX.Device.State.LightEnableMaxIndex = LightNumber ;
	}
	else
	{
		if( GLINUX.Device.State.LightEnableMaxIndex == LightNumber )
		{
			int i ;

			for( i = LightNumber ; i > 0 && GLINUX.Device.State.LightEnableFlag[ i ] == FALSE ; i -- ){}
			GLINUX.Device.State.LightEnableMaxIndex = i ;
		}
	}

	// アンビエントカラーとエミッシブカラーを掛け合わせた情報を更新する
	Graphics_Linux_DeviceState_RefreshAmbientAndEmissiveParam() ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// マテリアルのセット
extern int Graphics_Linux_DeviceState_SetMaterial( MATERIALPARAM *Material )
{
	if( _MEMCMP( GSYS.Light.MaterialTypeParam, GLINUX.Device.State.MaterialTypeParam, sizeof( GSYS.Light.MaterialTypeParam) ) == 0 &&
		_MEMCMP( Material, &GLINUX.Device.State.Material, sizeof( MATERIALPARAM ) ) == 0 &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GLINUX.Device.State.Material = *Material ;
	_MEMCPY( GLINUX.Device.State.MaterialTypeParam, GSYS.Light.MaterialTypeParam, sizeof( GSYS.Light.MaterialTypeParam ) ) ;

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uMatDif[ 0 ]   = Material->Diffuse.r ;
	GLINUX.Device.Shader.Constant.uMatDif[ 1 ]   = Material->Diffuse.g ;
	GLINUX.Device.Shader.Constant.uMatDif[ 2 ]   = Material->Diffuse.b ;
	GLINUX.Device.Shader.Constant.uMatDif[ 3 ]   = Material->Diffuse.a ;
	GLINUX.Device.Shader.Constant.uMatSpc[ 0 ]   = Material->Specular.r ;
	GLINUX.Device.Shader.Constant.uMatSpc[ 1 ]   = Material->Specular.g ;
	GLINUX.Device.Shader.Constant.uMatSpc[ 2 ]   = Material->Specular.b ;
	GLINUX.Device.Shader.Constant.uMatSpc[ 3 ]   = Material->Specular.a ;
	GLINUX.Device.Shader.Constant.uMatPow        = Material->Power < 0.00000000001f ? 0.00000000001f : Material->Power ;
	GLINUX.Device.Shader.Constant.uMatTypeParam0 = GLINUX.Device.State.MaterialTypeParam[ 0 ] ;
	GLINUX.Device.Shader.Constant.uMatTypeParam1 = GLINUX.Device.State.MaterialTypeParam[ 1 ] ;
	GLINUX.Device.Shader.Constant.uMatTypeParam2 = GLINUX.Device.State.MaterialTypeParam[ 2 ] ;
	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// アンビエントカラーとエミッシブカラーを掛け合わせた情報を更新する
	Graphics_Linux_DeviceState_RefreshAmbientAndEmissiveParam() ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// スペキュラを使用するかどうかをセット
extern int Graphics_Linux_DeviceState_SetSpecularEnable( int UseFlag )
{
	// 既存のパラメータと同じ場合は何もしない
	if( GLINUX.Device.State.UseSpecular == UseFlag &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uDifSrc_SpeSrc_MulSpeCol[ 2 ] = UseFlag ? 1.0f : 0.0f ;
	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	GLINUX.Device.State.UseSpecular = UseFlag ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// 頂点のディフューズカラーをマテリアルのディフューズカラーとして使用するかどうかを設定する
extern int Graphics_Linux_DeviceState_SetUseVertexDiffuseColor( int UseFlag )
{
	if( UseFlag == GLINUX.Device.State.MaterialUseVertexDiffuseColor &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uDifSrc_SpeSrc_MulSpeCol[ 0 ] = UseFlag ? 1.0f : 0.0f ;
	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	GLINUX.Device.State.MaterialUseVertexDiffuseColor = UseFlag ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// 頂点のスペキュラカラーをマテリアルのスペキュラカラーとして使用するかどうかを設定する
extern int Graphics_Linux_DeviceState_SetUseVertexSpecularColor( int UseFlag )
{
	if( UseFlag == GLINUX.Device.State.MaterialUseVertexSpecularColor &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uDifSrc_SpeSrc_MulSpeCol[ 1 ] = UseFlag ? 1.0f : 0.0f ;
	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	GLINUX.Device.State.MaterialUseVertexSpecularColor = UseFlag ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// カリングの有無をセット 
extern int Graphics_Linux_DeviceState_SetCullMode( int State )
{
	int Enable = FALSE ;
	GLenum CullFace = GL_BACK ;
	GLenum FrontFace = GL_CW ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	switch( State )
	{
	case 0 :
	default :
		Enable = FALSE ;
		break ;

	case 1 :
		Enable = TRUE ;
		CullFace = GL_FRONT ;
		break ;

	case 2 :
		Enable = TRUE ;
		CullFace = GL_BACK ;
		break ;
	}

	// モードが同じだったら何もせず終了
	if( GLINUX.Device.State.CullEnable == Enable &&
		GLINUX.Device.State.CullFace == CullFace &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GLINUX.Device.State.CullEnable = Enable ;
	GLINUX.Device.State.CullFace   = CullFace ;

	if( Enable )
	{
		glEnable( GL_CULL_FACE ) ;
	}
	else
	{
//		glEnable( GL_CULL_FACE ) ;
		glDisable( GL_CULL_FACE ) ;
	}

	glCullFace( CullFace ) ;
	glFrontFace( FrontFace ) ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// アンビエントカラーのセット
extern int Graphics_Linux_DeviceState_SetAmbient( COLOR_F *Color )
{
	if( _MEMCMP( Color, &GLINUX.Device.State.GlobalAmbientLightColor, sizeof( COLOR_F ) ) == 0 &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GLINUX.Device.State.GlobalAmbientLightColor = *Color ;

	// アンビエントカラーとエミッシブカラーを掛け合わせた情報を更新する
	Graphics_Linux_DeviceState_RefreshAmbientAndEmissiveParam() ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// テクスチャーのアドレッシングモードをセットする
extern int  Graphics_Linux_DeviceState_SetTextureAddress( int AddressMode, int Sampler )
{
	int i ;
	GLenum WrapMode ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( AddressMode < 0 || AddressMode >= DX_TEXADDRESS_NUM )
	{
		return -1 ;
	}

	WrapMode = g_DXTexAddrModeToGLES2WrapMode[ AddressMode ] ;

	if( Sampler < 0 )
	{
		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
			{
				if( WrapMode != ( DWORD )GLINUX.Device.State.TexAddressModeU[ i ] ||
					WrapMode != ( DWORD )GLINUX.Device.State.TexAddressModeV[ i ] ||
					WrapMode != ( DWORD )GLINUX.Device.State.TexAddressModeW[ i ] )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeU[ i ] &&
				WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeV[ i ] &&
				WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeW[ i ] &&
				GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GLINUX.Device.State.TexAddressModeU[ i ] = WrapMode ;
			GLINUX.Device.State.TexAddressModeV[ i ] = WrapMode ;
			GLINUX.Device.State.TexAddressModeW[ i ] = WrapMode ;

			if( GLINUX.Device.State.SetTexture[ i ] != NULL )
			{
				glActiveTexture( g_TextureEnum[ i ] ) ;
				glBindTexture( GL_TEXTURE_2D, GLINUX.Device.State.SetTexture[ i ]->TextureBuffer ) ;
				if( GLINUX.Device.State.SetTexture[ i ]->WrapS != GLINUX.Device.State.TexAddressModeU[ i ] )
				{
					GLINUX.Device.State.SetTexture[ i ]->WrapS = GLINUX.Device.State.TexAddressModeU[ i ] ;
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLINUX.Device.State.SetTexture[ i ]->WrapS ) ;
				}
				if( GLINUX.Device.State.SetTexture[ i ]->WrapT != GLINUX.Device.State.TexAddressModeV[ i ] )
				{
					GLINUX.Device.State.SetTexture[ i ]->WrapT = GLINUX.Device.State.TexAddressModeV[ i ] ;
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLINUX.Device.State.SetTexture[ i ]->WrapT ) ;
				}
			}
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		if( WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeU[ Sampler ] &&
			WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeV[ Sampler ] &&
			WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeW[ Sampler ] &&
			GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GLINUX.Device.State.TexAddressModeU[ Sampler ] = WrapMode ;
		GLINUX.Device.State.TexAddressModeV[ Sampler ] = WrapMode ;
		GLINUX.Device.State.TexAddressModeW[ Sampler ] = WrapMode ;

		if( GLINUX.Device.State.SetTexture[ Sampler ] != NULL )
		{
			glActiveTexture( g_TextureEnum[ Sampler ] ) ;
			glBindTexture( GL_TEXTURE_2D, GLINUX.Device.State.SetTexture[ Sampler ]->TextureBuffer ) ;
			if( GLINUX.Device.State.SetTexture[ Sampler ]->WrapS != GLINUX.Device.State.TexAddressModeU[ Sampler ] )
			{
				GLINUX.Device.State.SetTexture[ Sampler ]->WrapS = GLINUX.Device.State.TexAddressModeU[ Sampler ] ;
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLINUX.Device.State.SetTexture[ Sampler ]->WrapS ) ;
			}
			if( GLINUX.Device.State.SetTexture[ Sampler ]->WrapT != GLINUX.Device.State.TexAddressModeV[ Sampler ] )
			{
				GLINUX.Device.State.SetTexture[ Sampler ]->WrapT = GLINUX.Device.State.TexAddressModeV[ Sampler ] ;
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLINUX.Device.State.SetTexture[ Sampler ]->WrapT ) ;
			}
		}
	}

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// テクスチャーのアドレッシングモードをセットする
extern int Graphics_Linux_DeviceState_SetTextureAddressU( int AddressMode, int Sampler )
{
	int i ;
	GLenum WrapMode ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( AddressMode < 0 || AddressMode >= DX_TEXADDRESS_NUM )
	{
		return -1 ;
	}

	WrapMode = g_DXTexAddrModeToGLES2WrapMode[ AddressMode ] ;

	if( Sampler == -1 )
	{
		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
			{
				if( WrapMode != ( DWORD )GLINUX.Device.State.TexAddressModeU[ i ] )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeU[ i ] &&
				GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GLINUX.Device.State.TexAddressModeU[ i ] = WrapMode ;

			if( GLINUX.Device.State.SetTexture[ i ] != NULL )
			{
				glActiveTexture( g_TextureEnum[ i ] ) ;
				glBindTexture( GL_TEXTURE_2D, GLINUX.Device.State.SetTexture[ i ]->TextureBuffer ) ;
				if( GLINUX.Device.State.SetTexture[ i ]->WrapS != GLINUX.Device.State.TexAddressModeU[ i ] )
				{
					GLINUX.Device.State.SetTexture[ i ]->WrapS = GLINUX.Device.State.TexAddressModeU[ i ] ;
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLINUX.Device.State.SetTexture[ i ]->WrapS ) ;
				}
			}
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		if( WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeU[ Sampler ] &&
			GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GLINUX.Device.State.TexAddressModeU[ Sampler ] = WrapMode ;

		if( GLINUX.Device.State.SetTexture[ Sampler ] != NULL )
		{
			glActiveTexture( g_TextureEnum[ Sampler ] ) ;
			glBindTexture( GL_TEXTURE_2D, GLINUX.Device.State.SetTexture[ Sampler ]->TextureBuffer ) ;
			if( GLINUX.Device.State.SetTexture[ Sampler ]->WrapS != GLINUX.Device.State.TexAddressModeU[ Sampler ] )
			{
				GLINUX.Device.State.SetTexture[ Sampler ]->WrapS = GLINUX.Device.State.TexAddressModeU[ Sampler ] ;
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLINUX.Device.State.SetTexture[ Sampler ]->WrapS ) ;
			}
		}
	}

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// テクスチャーのアドレッシングモードをセットする
extern int Graphics_Linux_DeviceState_SetTextureAddressV( int AddressMode, int Sampler )
{
	int i ;
	GLenum WrapMode ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( AddressMode < 0 || AddressMode >= DX_TEXADDRESS_NUM )
	{
		return -1 ;
	}

	WrapMode = g_DXTexAddrModeToGLES2WrapMode[ AddressMode ] ;

	if( Sampler == -1 )
	{
		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
			{
				if( WrapMode != ( DWORD )GLINUX.Device.State.TexAddressModeV[ i ] )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeV[ i ] &&
				GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GLINUX.Device.State.TexAddressModeV[ i ] = WrapMode ;

			if( GLINUX.Device.State.SetTexture[ i ] != NULL )
			{
				glActiveTexture( g_TextureEnum[ i ] ) ;
				glBindTexture( GL_TEXTURE_2D, GLINUX.Device.State.SetTexture[ i ]->TextureBuffer ) ;
				if( GLINUX.Device.State.SetTexture[ i ]->WrapT != GLINUX.Device.State.TexAddressModeV[ i ] )
				{
					GLINUX.Device.State.SetTexture[ i ]->WrapT = GLINUX.Device.State.TexAddressModeV[ i ] ;
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLINUX.Device.State.SetTexture[ i ]->WrapT ) ;
				}
			}
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		if( WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeV[ Sampler ] &&
			GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GLINUX.Device.State.TexAddressModeV[ Sampler ] = WrapMode ;

		if( GLINUX.Device.State.SetTexture[ Sampler ] != NULL )
		{
			glActiveTexture( g_TextureEnum[ Sampler ] ) ;
			glBindTexture( GL_TEXTURE_2D, GLINUX.Device.State.SetTexture[ Sampler ]->TextureBuffer ) ;
			if( GLINUX.Device.State.SetTexture[ Sampler ]->WrapT != GLINUX.Device.State.TexAddressModeV[ Sampler ] )
			{
				GLINUX.Device.State.SetTexture[ Sampler ]->WrapT = GLINUX.Device.State.TexAddressModeV[ Sampler ] ;
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLINUX.Device.State.SetTexture[ Sampler ]->WrapT ) ;
			}
		}
	}

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// テクスチャーのアドレッシングモードをセットする
extern int Graphics_Linux_DeviceState_SetTextureAddressW( int AddressMode, int Sampler )
{
	int i ;
	GLenum WrapMode ;

	if( AddressMode < 0 || AddressMode >= DX_TEXADDRESS_NUM )
	{
		return -1 ;
	}

	WrapMode = g_DXTexAddrModeToGLES2WrapMode[ AddressMode ] ;

	if( Sampler == -1 )
	{
		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
			{
				if( WrapMode != ( DWORD )GLINUX.Device.State.TexAddressModeW[ i ] )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeW[ i ] &&
				GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GLINUX.Device.State.TexAddressModeW[ i ] = WrapMode ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		if( WrapMode == ( DWORD )GLINUX.Device.State.TexAddressModeW[ Sampler ] &&
			GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GLINUX.Device.State.TexAddressModeW[ Sampler ] = WrapMode ;
	}

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// テクスチャーのアドレッシングモードをセットする
extern int Graphics_Linux_DeviceState_SetTextureAddressUVW( int AddressModeU, int AddressModeV, int AddressModeW, int Sampler )
{
	if( Graphics_Linux_DeviceState_SetTextureAddressU( AddressModeU, Sampler ) < 0 )
	{
		return -1 ;
	}

	if( Graphics_Linux_DeviceState_SetTextureAddressV( AddressModeV, Sampler ) < 0 )
	{
		return -1 ;
	}

	if( Graphics_Linux_DeviceState_SetTextureAddressW( AddressModeW, Sampler ) < 0 )
	{
		return -1 ;
	}

	return 0 ;
}

// 標準描画用のテクスチャ座標変換行列を定数バッファにセットする
static int Graphics_Linux_DeviceState_SetNormalTextureAddressTransformMatrix_ConstBuffer( void )
{
	// 定数データに反映
	GLINUX.Device.Shader.Constant.uTexMat[ 0 ][ 0 ] = GLINUX.Device.State.TextureAddressTransformMatrix.m[ 0 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uTexMat[ 0 ][ 1 ] = GLINUX.Device.State.TextureAddressTransformMatrix.m[ 1 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uTexMat[ 0 ][ 2 ] = GLINUX.Device.State.TextureAddressTransformMatrix.m[ 2 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uTexMat[ 0 ][ 3 ] = GLINUX.Device.State.TextureAddressTransformMatrix.m[ 3 ][ 0 ] ;
	GLINUX.Device.Shader.Constant.uTexMat[ 1 ][ 0 ] = GLINUX.Device.State.TextureAddressTransformMatrix.m[ 0 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uTexMat[ 1 ][ 1 ] = GLINUX.Device.State.TextureAddressTransformMatrix.m[ 1 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uTexMat[ 1 ][ 2 ] = GLINUX.Device.State.TextureAddressTransformMatrix.m[ 2 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.uTexMat[ 1 ][ 3 ] = GLINUX.Device.State.TextureAddressTransformMatrix.m[ 3 ][ 1 ] ;
	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 標準描画用のテクスチャパラメータが設定されているかどうかフラグを立てる
	GLINUX.Device.Shader.Constant.SetNormalTextureAddressTransformMatrix = TRUE ;

	// 正常終了
	return 0 ;
}

// テクスチャ座標変換行列をセットする
extern int  Graphics_Linux_DeviceState_SetTextureAddressTransformMatrix( int Use, MATRIX *Matrix )
{
	if( GLINUX.Device.State.TextureAddressTransformMatrixUse == FALSE &&
		Use == FALSE && 
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// テクスチャ座標変換行列を使用するかどうかを保存
	GLINUX.Device.State.TextureAddressTransformMatrixUse = Use ;

	// テクスチャ座標変換行列をセット
	if( Use == TRUE )
	{
		GLINUX.Device.State.TextureAddressTransformMatrix = *Matrix ;
	}
	else
	{
		CreateIdentityMatrix( &GLINUX.Device.State.TextureAddressTransformMatrix ) ;
	}

	// 定数バッファに反映する
	Graphics_Linux_DeviceState_SetNormalTextureAddressTransformMatrix_ConstBuffer() ;

	return 0 ;
}

// フォグを有効にするかどうかを設定する( TRUE:有効  FALSE:無効 )
extern int  Graphics_Linux_DeviceState_SetFogEnable( int Flag )
{
	if( Flag == GLINUX.Device.State.FogEnable &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GLINUX.Device.State.FogEnable = Flag ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// フォグモードを設定する
extern int  Graphics_Linux_DeviceState_SetFogVertexMode( int Mode /* DX_FOGMODE_NONE 等 */ )
{
	if( Mode == GLINUX.Device.State.FogMode &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GLINUX.Device.State.FogMode = Mode ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// フォグカラーを変更する
extern int  Graphics_Linux_DeviceState_SetFogColor( unsigned int Color )
{
	if( Color == GLINUX.Device.State.FogColor &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uFogColor[ 0 ] = ( ( Color >> 16 ) & 0xff ) / 255.0f ;
	GLINUX.Device.Shader.Constant.uFogColor[ 1 ] = ( ( Color >>  8 ) & 0xff ) / 255.0f ;
	GLINUX.Device.Shader.Constant.uFogColor[ 2 ] = ( ( Color >>  0 ) & 0xff ) / 255.0f ;
	GLINUX.Device.Shader.Constant.uFogColor[ 3 ] = 1.0f ;
	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	GLINUX.Device.State.FogColor = Color ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// フォグの色以外の定数情報を更新する
static int Graphics_Linux_DeviceState_UpdateConstantFogParam( void )
{
	// 定数データに反映
	GLINUX.Device.Shader.Constant.uFog[ 0 ] = GLINUX.Device.State.FogEnd / ( GLINUX.Device.State.FogEnd - GLINUX.Device.State.FogStart ) ;
	GLINUX.Device.Shader.Constant.uFog[ 1 ] = -1.0f / ( GLINUX.Device.State.FogEnd - GLINUX.Device.State.FogStart ) ;
	GLINUX.Device.Shader.Constant.uFog[ 2 ] = GLINUX.Device.State.FogDensity ;
	GLINUX.Device.Shader.Constant.uFog[ 3 ] = 2.71828183f ;
	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// フォグが始まる距離と終了する距離を設定する( 0.0f ～ 1.0f )
extern int  Graphics_Linux_DeviceState_SetFogStartEnd( float Start, float End )
{
	int UpdateFlag ;

	DRAWSTOCKINFO

	UpdateFlag = FALSE ;

	if( Start != GLINUX.Device.State.FogStart ||
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == TRUE )
	{
		GLINUX.Device.State.FogStart = Start ;
		UpdateFlag = TRUE ;
	}

	if( End != GLINUX.Device.State.FogEnd ||
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == TRUE )
	{
		GLINUX.Device.State.FogEnd = End ;
		UpdateFlag = TRUE ;
	}

	if( UpdateFlag )
	{
		// 定数情報を更新
		Graphics_Linux_DeviceState_UpdateConstantFogParam() ;
	}

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// フォグの密度を設定する( 0.0f ～ 1.0f )
extern int  Graphics_Linux_DeviceState_SetFogDensity( float Density )
{
	if( Density == GLINUX.Device.State.FogDensity &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GLINUX.Device.State.FogDensity = Density ;

	// 定数情報を更新
	Graphics_Linux_DeviceState_UpdateConstantFogParam() ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// ライティングの有無フラグをセットする
extern int  Graphics_Linux_DeviceState_SetLighting( int UseFlag )
{
	if( UseFlag == GLINUX.Device.State.Lighting &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GLINUX.Device.State.Lighting = UseFlag ;

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// 最大異方性をセットする
extern	int		Graphics_Linux_DeviceState_SetMaxAnisotropy( int MaxAnisotropy, int Sampler )
{
	int i ;
	int SetAnisotropy = MaxAnisotropy ;

	if( Sampler < 0 )
	{
		if( GLINUX.Device.State.MaxAnisotropy == MaxAnisotropy &&
			GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		GLINUX.Device.State.MaxAnisotropy = MaxAnisotropy ;

		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
			{
				if( GLINUX.Device.State.MaxAnisotropyDim[ i ] != SetAnisotropy )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( GLINUX.Device.State.MaxAnisotropyDim[ i ] == SetAnisotropy &&
				GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GLINUX.Device.State.MaxAnisotropyDim[ i ] = SetAnisotropy ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return 0 ;
		}

		GLINUX.Device.State.MaxAnisotropy = -1 ;

		if( GLINUX.Device.State.MaxAnisotropyDim[ Sampler ] == SetAnisotropy &&
			GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GLINUX.Device.State.MaxAnisotropyDim[ Sampler ] = SetAnisotropy ;
	}

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// ビューポートをセットする
extern	int		Graphics_Linux_DeviceState_SetViewport( RECT *Viewport )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( Viewport->left   == GLINUX.Device.State.ViewportRect.left   &&
		Viewport->top    == GLINUX.Device.State.ViewportRect.top    &&
		Viewport->right  == GLINUX.Device.State.ViewportRect.right  &&
		Viewport->bottom == GLINUX.Device.State.ViewportRect.bottom &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// 書き出す
	if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		DRAWSTOCKINFO
	}

	// 描画を終了させておく
	Graphics_Linux_RenderEnd() ;

	// ビューポートの設定を反映
    glViewport(
		Viewport->left,
		Viewport->top,
		Viewport->right  - Viewport->left,
		Viewport->bottom - Viewport->top
	) ;

	// ビューポートの情報の保存
	GLINUX.Device.State.ViewportRect = *Viewport ;

	// 終了
	return 0 ;
}

// ビューポートをセットする( 簡易版 )
extern	int		Graphics_Linux_DeviceState_SetViewportEasy( int x1, int y1, int x2, int y2 )
{
	RECT Viewport ;

	Viewport.left   = x1 ;
	Viewport.top    = y1 ;
	Viewport.right  = x2 ;
	Viewport.bottom = y2 ;

	return Graphics_Linux_DeviceState_SetViewport( &Viewport ) ;
}

// 描画モードのセット
extern	int		Graphics_Linux_DeviceState_SetDrawMode( int DrawMode )
{
	if( GLINUX.Device.State.DrawMode == DrawMode &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		DRAWSTOCKINFO
	}

	// レンダリング設定を変更する
	switch( DrawMode )
	{
	case DX_DRAWMODE_BILINEAR :
		// バイリニア描画
		Graphics_Linux_DeviceState_SetSampleFilterMode( GL_LINEAR_MIPMAP_LINEAR, -1 ) ;
		break ;

	case DX_DRAWMODE_NEAREST :
		// 二アレストネイバー
		Graphics_Linux_DeviceState_SetSampleFilterMode( GL_NEAREST, -1 ) ;
		break ;

	case DX_DRAWMODE_ANISOTROPIC :
		// 異方性フィルタリング
		Graphics_Linux_DeviceState_SetSampleFilterMode( GL_LINEAR_MIPMAP_LINEAR, -1 ) ;
		break ;

	default : return -1 ;
	}

	GLINUX.Device.State.DrawMode                 = DrawMode ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// アルファテストで使用する比較モードを設定する
extern	int		Graphics_Linux_DeviceState_SetAlphaTestCmpMode( int AlphaTestCmpMode /* DX_CMP_NEVER など */ )
{
	if( GLINUX.Device.State.AlphaTestCmpMode == AlphaTestCmpMode &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// 値を保存
	GLINUX.Device.State.AlphaTestCmpMode = AlphaTestCmpMode ;

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uATestCmpMode = AlphaTestCmpMode ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// アルファテストで使用する参照値を設定する
extern	int		Graphics_Linux_DeviceState_SetAlphaTestRef( int AlphaTestRef )
{
	if( GLINUX.Device.State.AlphaTestRef == AlphaTestRef &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// 値を保存
	GLINUX.Device.State.AlphaTestRef = AlphaTestRef ;

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uATestRef = AlphaTestRef / 255.0f ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// カラーにアルファ値を乗算するかどうかを設定する
extern	int		Graphics_Linux_DeviceState_SetMulAlphaColor( int UseMulAlphaColor )
{
	DRAWSTOCKINFO

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uMulAlphaColor = UseMulAlphaColor ? 1.0f : 0.0f ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// 背景色を設定する
extern	int		Graphics_Linux_DeviceState_SetBackgroundColor( int Red, int Green, int Blue, int Alpha )
{
	float RedF ;
	float GreenF ;
	float BlueF ;
	float AlphaF ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( GLINUX.Device.State.BackgroundColor.r == Red &&
		GLINUX.Device.State.BackgroundColor.g == Green &&
		GLINUX.Device.State.BackgroundColor.b == Blue &&
		GLINUX.Device.State.BackgroundColor.a == Alpha &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	RedF   = ( float )Red   / 255.0f ;
	GreenF = ( float )Green / 255.0f ;
	BlueF  = ( float )Blue  / 255.0f ;
	AlphaF = ( float )Alpha / 255.0f ;

	DRAWSTOCKINFO

	glClearColor( RedF, GreenF, BlueF, AlphaF ) ;

	GLINUX.Device.State.BackgroundColor.r = ( BYTE )Red ;
	GLINUX.Device.State.BackgroundColor.g = ( BYTE )Green ;
	GLINUX.Device.State.BackgroundColor.b = ( BYTE )Blue ;
	GLINUX.Device.State.BackgroundColor.a = ( BYTE )Alpha ;
	
	// 終了
	return 0 ;
}

// Factor Color を設定する
extern	int		Graphics_Linux_DeviceState_SetFactorColor( const DX_LINUX_SHADER_FLOAT4 *FactorColor )
{
	if( GLINUX.Device.Shader.Constant.uFactorColor[ 0 ] == (*FactorColor)[ 0 ] &&
		GLINUX.Device.Shader.Constant.uFactorColor[ 1 ] == (*FactorColor)[ 1 ] &&
		GLINUX.Device.Shader.Constant.uFactorColor[ 2 ] == (*FactorColor)[ 2 ] &&
		GLINUX.Device.Shader.Constant.uFactorColor[ 3 ] == (*FactorColor)[ 3 ] &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uFactorColor[ 0 ] = (*FactorColor)[ 0 ] ;
	GLINUX.Device.Shader.Constant.uFactorColor[ 1 ] = (*FactorColor)[ 1 ] ;
	GLINUX.Device.Shader.Constant.uFactorColor[ 2 ] = (*FactorColor)[ 2 ] ;
	GLINUX.Device.Shader.Constant.uFactorColor[ 3 ] = (*FactorColor)[ 3 ] ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// トゥーンレンダリングの輪郭線の太さを設定する
extern int Graphics_Linux_DeviceState_SetToonOutLineSize( float Size )
{
	if( GLINUX.Device.Shader.Constant.uToonOutLineSize[ 0 ] == Size &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// 定数データに反映
	GLINUX.Device.Shader.Constant.uToonOutLineSize[ 0 ] = Size ;
	GLINUX.Device.Shader.Constant.uToonOutLineSize[ 1 ] = Size ;
	GLINUX.Device.Shader.Constant.uToonOutLineSize[ 2 ] = Size ;
	GLINUX.Device.Shader.Constant.uToonOutLineSize[ 3 ] = Size ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 終了
	return 0 ;
}

// 描画ブレンドモードのセット
extern	int		Graphics_Linux_DeviceState_SetBlendMode( int BlendMode, int BlendEnable, int BlendRGBSrc, int BlendRGBDest, int BlendRGBOp, int BlendASrc, int BlendADest, int BlendAOp, int NotWriteAlphaChannelFlag )
{
	DX_LINUX_RENDER_BLEND_INFO *BlendInfo ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( GLINUX.Device.State.BlendMode	== BlendMode &&
		GLINUX.Device.State.BlendEnable	== BlendEnable &&
		GLINUX.Device.State.BlendRGBSrc	== BlendRGBSrc &&
		GLINUX.Device.State.BlendRGBDest	== BlendRGBDest &&
		GLINUX.Device.State.BlendRGBOp	== BlendRGBOp &&
		GLINUX.Device.State.BlendASrc	== BlendASrc &&
		GLINUX.Device.State.BlendADest	== BlendADest &&
		GLINUX.Device.State.BlendAOp		== BlendAOp &&
		GLINUX.Device.State.NotWriteAlphaChannelFlag == NotWriteAlphaChannelFlag &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GLINUX.Device.State.BlendMode	= BlendMode ;
	GLINUX.Device.State.BlendEnable	= BlendEnable ;
	GLINUX.Device.State.BlendRGBSrc	= BlendRGBSrc ;
	GLINUX.Device.State.BlendRGBDest	= BlendRGBDest ;
	GLINUX.Device.State.BlendRGBOp	= BlendRGBOp ;
	GLINUX.Device.State.BlendASrc	= BlendASrc ;
	GLINUX.Device.State.BlendADest	= BlendADest ;
	GLINUX.Device.State.BlendAOp		= BlendAOp ;
	GLINUX.Device.State.NotWriteAlphaChannelFlag = NotWriteAlphaChannelFlag ;

	BlendInfo = &g_DefaultBlendDescArray[ BlendMode ] ;

	if( BlendMode == DX_BLENDMODE_CUSTOM )
	{
		if( BlendEnable )
		{
			glEnable( GL_BLEND ) ;
			if( NotWriteAlphaChannelFlag )
			{
				glBlendEquationSeparate(
					DxBlendOpToGLTable[ BlendRGBOp ],
					GL_FUNC_ADD
				) ;
				glBlendFuncSeparate(
					DxBlendTypeToGLTable[ BlendRGBSrc ], DxBlendTypeToGLTable[ BlendRGBDest ],
					GL_ZERO, GL_ONE
				) ;
			}
			else
			{
				glBlendEquationSeparate(
					DxBlendOpToGLTable[ BlendRGBOp ],
					DxBlendOpToGLTable[ BlendAOp ]
				) ;
				glBlendFuncSeparate(
					DxBlendTypeToGLTable[ BlendRGBSrc ], DxBlendTypeToGLTable[ BlendRGBDest ],
					DxBlendTypeToGLTable[ BlendASrc ],   DxBlendTypeToGLTable[ BlendADest ]
				) ;
			}
		}
		else
		{
			glDisable( GL_BLEND ) ;
		}
	}
	else
	{
		if( BlendInfo->BlendEnable )
		{
			glEnable( GL_BLEND ) ;
			if( GLINUX.Device.State.NotWriteAlphaChannelFlag )
			{
				glBlendEquationSeparate(
					BlendInfo->ColorBlendFunc,
					GL_FUNC_ADD
				) ;
				glBlendFuncSeparate(
					BlendInfo->ColorSourceMul, BlendInfo->ColorDestMul,
					GL_ZERO, GL_ONE
				) ;
			}
			else
			{
				glBlendEquationSeparate(
					BlendInfo->ColorBlendFunc,
					BlendInfo->AlphaBlendFunc
				) ;
				glBlendFuncSeparate(
					BlendInfo->ColorSourceMul, BlendInfo->ColorDestMul,
					BlendInfo->AlphaSourceMul, BlendInfo->AlphaDestMul
				) ;
			}
		}
		else
		{
			glDisable( GL_BLEND ) ;
		}
	}

	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return 0 ;
}

// 描画対象の変更
extern	int		Graphics_Linux_DeviceState_SetRenderTarget( GLuint TargetFrameBuffer, GLuint TargetFrameBufferWidth, GLuint TargetFrameBufferHeight )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( GLINUX.Device.State.TargetFrameBuffer == TargetFrameBuffer &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck  == FALSE )
	{
		return 0 ;
	}

	// 頂点書き出し
	DRAWSTOCKINFO

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// レンダリングターゲットの保存
	GLINUX.Device.State.TargetFrameBuffer = TargetFrameBuffer ;
	GLINUX.Device.State.TargetFrameBufferWidth = TargetFrameBufferWidth ;
	GLINUX.Device.State.TargetFrameBufferHeight = TargetFrameBufferHeight ;

	// レンダリングターゲットの変更
	glBindFramebuffer( GL_FRAMEBUFFER, TargetFrameBuffer ) ;

	// ビューポートの設定値を更新
	if( TargetFrameBuffer != 0 )
	{
		Graphics_Linux_DeviceState_SetViewportEasy( 0, 0, TargetFrameBufferWidth, TargetFrameBufferHeight ) ;
	}

	// 終了
	return  0 ;
}

// 使用するシェーダーを変更する
extern	int	Graphics_Linux_DeviceState_SetShader( GRAPHICS_LINUX_SHADER *Shader, int NormalShader )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( GLINUX.Device.State.SetShader == Shader &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( Shader != NULL )
	{
	    glUseProgram( Shader->Shader ) ;
	}

	GLINUX.Device.State.SetShader          = Shader ;
	GLINUX.Device.State.SetNormalShader    = NormalShader ;
	if( NormalShader == FALSE )
	{
		GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	}

	// 終了
	return 0 ;
}

// シェーダーの使用を止める
extern	int		Graphics_Linux_DeviceState_ResetShader( int SetNormalShaderCancel )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( GLINUX.Device.State.SetNormalShader &&
		SetNormalShaderCancel &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GLINUX.Device.State.SetShader == NULL )
	{
		return 0 ;
	}

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

    glUseProgram( 0 ) ;

	GLINUX.Device.State.SetShader = NULL ;

	GLINUX.Device.State.SetNormalShader    = FALSE ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// シェーダーの Uniform を更新する
extern int Graphics_Linux_DeviceState_UpdateShaderUniform( GRAPHICS_LINUX_SHADER *Shader, int LightNum, int UseLocalWorldMatrixUniformNum )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 現在のシェーダーに適用されている Uniform 更新番号と、
	// グローバルで持っているシェーダーの更新番号が一致している場合は何もせずに終了
	if( Shader == NULL ||
		( Shader->ApplyUniformUpdateCount == GLINUX.Device.Shader.Constant.UpdateCount &&
		  GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) )
	{
		return 0 ;
	}

	// シェーダーを使用状態にセット
	glUseProgram( Shader->Shader ) ;

	// シェーダーにグローバルの Uniform を適用する

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SRC_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SRC_TEX ], 0 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_BLEND_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_BLEND_TEX ], 1 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_DIF_MAP_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_DIF_MAP_TEX ], 0 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_NRM_MAP_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_NRM_MAP_TEX ], 1 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SPC_MAP_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SPC_MAP_TEX ], 2 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TOON_DIF_GRAD_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TOON_DIF_GRAD_TEX ], 3 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TOON_SPC_GRAD_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TOON_SPC_GRAD_TEX ], 4 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TOON_SPHERE_MAP_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TOON_SPHERE_MAP_TEX ], 5 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOW_MAP0_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOW_MAP0_TEX ], 8 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOW_MAP1_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOW_MAP0_TEX ], 9 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOW_MAP2_TEX ] != 0xffff )
	{
		glUniform1i( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOW_MAP0_TEX ], 10 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_AMB_EMI ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_AMB_EMI ], GLINUX.Device.Shader.Constant.uAmb_Emi ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_DIF ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_DIF ], GLINUX.Device.Shader.Constant.uMatDif ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_SPC ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_SPC ], GLINUX.Device.Shader.Constant.uMatSpc ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_POW ] != 0xffff )
	{
		UNIFORM_SET_FLOAT1( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_POW ], GLINUX.Device.Shader.Constant.uMatPow ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM0 ] != 0xffff )
	{
		UNIFORM_SET_FLOAT1( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM0 ], GLINUX.Device.Shader.Constant.uMatTypeParam0 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM1 ] != 0xffff )
	{
		UNIFORM_SET_FLOAT1( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM1 ], GLINUX.Device.Shader.Constant.uMatTypeParam1 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM2 ] != 0xffff )
	{
		UNIFORM_SET_FLOAT1( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MAT_TYPE_PARAM2 ], GLINUX.Device.Shader.Constant.uMatTypeParam2 ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_POS ] != 0xffff )
	{
		glUniform3fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_POS ], LightNum, GLINUX.Device.Shader.Constant.uLightPos[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_DIR ] != 0xffff )
	{
		glUniform3fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_DIR ], LightNum, GLINUX.Device.Shader.Constant.uLightDir[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_DIF ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_DIF ], LightNum, GLINUX.Device.Shader.Constant.uLightDif[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_SPC ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_SPC ], LightNum, GLINUX.Device.Shader.Constant.uLightSpc[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_AMB ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_AMB ], LightNum, GLINUX.Device.Shader.Constant.uLightAmb[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_RANGE_FALLOFF_AT0_AT1 ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_RANGE_FALLOFF_AT0_AT1 ], LightNum, GLINUX.Device.Shader.Constant.uLightRange_FallOff_AT0_AT1[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_AT2_SPOTP0_SPOTP1 ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LIGHT_AT2_SPOTP0_SPOTP1 ], LightNum, GLINUX.Device.Shader.Constant.uLightAT2_SpotP0_SpotP1[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_ANTI_VIEW_PORT_MAT ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_ANTI_VIEW_PORT_MAT ], 4, GLINUX.Device.Shader.Constant.uAntiVPMat[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_VIEW_MAT ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_VIEW_MAT ], 3, GLINUX.Device.Shader.Constant.uViewMat[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_PROJ_MAT ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_PROJ_MAT ], 4, GLINUX.Device.Shader.Constant.uProjMat[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_FOG ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_FOG ], GLINUX.Device.Shader.Constant.uFog ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TOON_OUTLINE_SIZE ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TOON_OUTLINE_SIZE ], GLINUX.Device.Shader.Constant.uToonOutLineSize ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_DIFSRC_SPCSRC_MULSPECOL ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_DIFSRC_SPCSRC_MULSPECOL ], GLINUX.Device.Shader.Constant.uDifSrc_SpeSrc_MulSpeCol ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SM_LVP_MAT ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SM_LVP_MAT ], 12, GLINUX.Device.Shader.Constant.uSMLVPMat[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TEX_MAT ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_TEX_MAT ], 6, GLINUX.Device.Shader.Constant.uTexMat[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LW_MAT ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_LW_MAT ], UseLocalWorldMatrixUniformNum, GLINUX.Device.Shader.Constant.uLWMat[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MUL_ALPHA_COLOR ] != 0xffff )
	{
		UNIFORM_SET_FLOAT1( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_MUL_ALPHA_COLOR ], GLINUX.Device.Shader.Constant.uMulAlphaColor ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_ALPHA_TEST_REF ] != 0xffff )
	{
		UNIFORM_SET_FLOAT1( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_ALPHA_TEST_REF ], GLINUX.Device.Shader.Constant.uATestRef ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_ALPHA_TEST_CMPMODE ] != 0xffff )
	{
		UNIFORM_SET_INT1( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_ALPHA_TEST_CMPMODE ], GLINUX.Device.Shader.Constant.uATestCmpMode ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_FOG_COLOR ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_FOG_COLOR ], GLINUX.Device.Shader.Constant.uFogColor ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_FACTOR_COLOR ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_FACTOR_COLOR ], GLINUX.Device.Shader.Constant.uFactorColor ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_IGNORE_TEXTURE_COLOR ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_IGNORE_TEXTURE_COLOR ], GLINUX.Device.Shader.Constant.uIgnoreTextureColor ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_ADD_COLOR ] != 0xffff )
	{
		UNIFORM_SET_FLOAT4( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_ADD_COLOR ], GLINUX.Device.Shader.Constant.uAddColor ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOWMAP_LIGHT_ENABLE ] != 0xffff )
	{
		glUniform4fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOWMAP_LIGHT_ENABLE ], 3, GLINUX.Device.Shader.Constant.uShadowMapLightEnable[ 0 ] ) ;
	}

	if( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOWMAP_DADJ_GRAD ] != 0xffff )
	{
		glUniform2fv( Shader->UniformIndex[ LINUX_SHADER_UNIFORM_SHADOWMAP_DADJ_GRAD ], 3, GLINUX.Device.Shader.Constant.uShadowMap_DAdj_Grad[ 0 ] ) ;
	}

	// 適用した番号を更新する
	Shader->ApplyUniformUpdateCount = GLINUX.Device.Shader.Constant.UpdateCount ;

	// 終了
	return 0 ;
}

// 指定のシェーダーの頂点フォーマットの頂点データの描画準備を行う
extern int Graphics_Linux_DeviceState_SetupShaderVertexData( GRAPHICS_LINUX_SHADER *Shader, VERTEXBUFFER_INPUT_INFO_LINUX *VertexInputInfo, const void *VertexData, GLuint VertexBuffer )
{
	int i ;
	int UseAttr[ LINUX_VERTEX_ATTR_NUM ] = { 0 } ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 頂点データが無効な場合は頂点バッファを使用する
	if( VertexData == NULL )
	{
		// 頂点バッファをセット
		glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer ) ;
	}
	else
	{
		// 頂点バッファを使用しない場合は頂点バッファとインデックスバッファは無効にする
		glBindBuffer( GL_ARRAY_BUFFER, 0 ) ;
	}

	for( i = 0 ; i < VertexInputInfo->ElementInfoNum ; i ++ )
	{
		VERTEXBUFFER_INPUT_ELEMENT_INFO_LINUX *Element = &VertexInputInfo->ElementInfo[ i ] ;

		if( Element->ElementBase == LINUX_VERTEX_ATTR_OTHER ||
			Shader->AttributeIndex[ Element->ElementBase ] == 0xffff )
		{
			continue ;
		}

		VERTEXBUFFER_INPUT_ELEMENT_INFO_BASE_LINUX *ElementBase = &g_VertexElementInfoBase[ Element->ElementBase ] ;

		UseAttr[ Shader->AttributeIndex[ Element->ElementBase ] ] = TRUE ;
		if( GLINUX.Device.State.VertexAttrEnable[ Shader->AttributeIndex[ Element->ElementBase ] ] == FALSE )
		{
			GLINUX.Device.State.VertexAttrEnable[ Shader->AttributeIndex[ Element->ElementBase ] ] = TRUE ;
			glEnableVertexAttribArray( Shader->AttributeIndex[ Element->ElementBase ] ) ;
		}

		glVertexAttribPointer(
			Shader->AttributeIndex[ Element->ElementBase ],
			ElementBase->Size,
			ElementBase->DataFormat,
			ElementBase->Normalized,
			VertexInputInfo->VertexDataSize,
			( BYTE * )VertexData + Element->Offset
		) ;
	}

	// 使用されていない要素は無効化する
	for( i = 0 ; i < LINUX_VERTEX_ATTR_NUM ; i ++ )
	{
		if( UseAttr[ i ] == FALSE && GLINUX.Device.State.VertexAttrEnable[ i ] == TRUE )
		{
			GLINUX.Device.State.VertexAttrEnable[ i ] = FALSE ;
			glDisableVertexAttribArray( i ) ;
		}
	}

	// 終了
	return 0 ;
}

// 使用する頂点バッファを変更する
extern	int		Graphics_Linux_DeviceState_SetVertexBuffer( GLuint VertexBuffer )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( GLINUX.Device.State.SetVertexBuffer == VertexBuffer &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

    glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer ) ;

	GLINUX.Device.State.SetVertexBuffer = VertexBuffer ;

	// 終了
	return 0 ;
}

// シェーダーで使用するテクスチャを変更する
extern int Graphics_Linux_DeviceState_SetTexture( int SlotIndex, GRAPHICS_LINUX_TEXTURE *Texture )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( SlotIndex < 0 || SlotIndex >= USE_TEXTURESTAGE_NUM )
	{
		return -1 ;
	}

	if( Texture == GLINUX.Device.State.SetTexture[ SlotIndex ] &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	GLINUX.Device.State.SetTexture[ SlotIndex ] = Texture ;

	glActiveTexture( g_TextureEnum[ SlotIndex ] ) ;
	if( Texture == NULL )
	{
		glBindTexture( GL_TEXTURE_2D, 0 ) ;
	}
	else
	{
		glBindTexture( GL_TEXTURE_2D, Texture->TextureBuffer ) ;

		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck )
		{
			Texture->WrapS = GLINUX.Device.State.TexAddressModeU[ SlotIndex ] ;
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Texture->WrapS ) ;

			Texture->WrapT = GLINUX.Device.State.TexAddressModeV[ SlotIndex ] ;
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Texture->WrapT ) ;

			Texture->MagFilter = GLINUX.Device.State.TexMagFilter[ SlotIndex ] ;
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture->MagFilter ) ;

			GLenum MinFilterTmp = GLINUX.Device.State.TexMinFilter[ SlotIndex ] ;
			if( Texture->MipMapCount <= 1 && MinFilterTmp == GL_LINEAR_MIPMAP_LINEAR )
			{
				MinFilterTmp = GL_LINEAR ;
			}
			Texture->MinFilter = MinFilterTmp ;
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture->MinFilter ) ;
		}
		else
		{
			if( Texture->WrapS != GLINUX.Device.State.TexAddressModeU[ SlotIndex ] )
			{
				Texture->WrapS = GLINUX.Device.State.TexAddressModeU[ SlotIndex ] ;
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Texture->WrapS ) ;
			}
			if( Texture->WrapT != GLINUX.Device.State.TexAddressModeV[ SlotIndex ] )
			{
				Texture->WrapT = GLINUX.Device.State.TexAddressModeV[ SlotIndex ] ;
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Texture->WrapT ) ;
			}
			// if( Texture->MagFilter != GLINUX.Device.State.TexMagFilter[ SlotIndex ] )
			{
				Texture->MagFilter = GLINUX.Device.State.TexMagFilter[ SlotIndex ] ;
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture->MagFilter ) ;
			}
			// if( Texture->MinFilter != GLINUX.Device.State.TexMinFilter[ SlotIndex ] )
			{
				GLenum MinFilterTmp = GLINUX.Device.State.TexMinFilter[ SlotIndex ] ;
				if( Texture->MipMapCount <= 1 && MinFilterTmp == GL_LINEAR_MIPMAP_LINEAR )
				{
					MinFilterTmp = GL_LINEAR ;
				}
				// if( Texture->MinFilter != MinFilterTmp )
				{
					Texture->MinFilter = MinFilterTmp ;
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture->MinFilter ) ;
				}
			}
		}
	}

	// 終了
	return 0 ;
}

// 標準描画の準備を行う
extern	int		Graphics_Linux_DeviceState_NormalDrawSetup( void )
{
	int											IgnoreTextureAlpha ;
	int											NextBlendMode ;
	int											NextBlendEnable ;
	int											NextBlendRGBSrc ;
	int											NextBlendRGBDest ;
	int											NextBlendRGBOp ;
	int											NextBlendASrc ;
	int											NextBlendADest ;
	int											NextBlendAOp ;
	int											UseFloatFactorColor = FALSE ;
	DX_LINUX_SHADER_FLOAT4						FloatFactorColor ;
	int											AlphaTestRef = 0 ;
	int											AlphaTestCmpMode = 0 ;
	GRAPHICS_LINUX_TEXTURE					*UseTexture[ USE_TEXTURESTAGE_NUM ] ;
	int											UseTextureNum = 0 ;
	GRAPHICS_HARDWARE_LINUX_SHADER_BASE		*SB  = &GLINUX.Device.Shader.Base ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_BASE	*SCB = &GLINUX.ShaderCode.Base ;
	GLuint										*VS			= NULL ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO	*VSAddress	= NULL ;
	GLuint										*FS			= NULL ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO	*FSAddress	= NULL ;
	GRAPHICS_LINUX_SHADER						**UseShader	= NULL ;

	// テクスチャが変更された、フラグを倒す
	GLINUX.Device.DrawSetting.ChangeTextureFlag = FALSE ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// パラメータが変更された、フラグを倒す
	GLINUX.Device.DrawSetting.ChangeBlendParamFlag = FALSE ;

	// ブレンドモードの決定
	{
		NextBlendMode		= GLINUX.Device.DrawSetting.BlendMode ;
		NextBlendEnable		= GLINUX.Device.DrawSetting.BlendEnable ;
		NextBlendRGBSrc		= GLINUX.Device.DrawSetting.BlendRGBSrc ;
		NextBlendRGBDest	= GLINUX.Device.DrawSetting.BlendRGBDest ;
		NextBlendRGBOp		= GLINUX.Device.DrawSetting.BlendRGBOp ;
		NextBlendASrc		= GLINUX.Device.DrawSetting.BlendASrc ;
		NextBlendADest		= GLINUX.Device.DrawSetting.BlendADest ;
		NextBlendAOp		= GLINUX.Device.DrawSetting.BlendAOp	;
		switch( GLINUX.Device.DrawSetting.BlendMode )
		{
		case DX_BLENDMODE_SUB :
			// 減算ブレンドの場合は14番目のブレンドモードを使用する
			NextBlendMode = DX_BLENDMODE_SUB1 ;
			break ;

		case DX_BLENDMODE_NOBLEND :
			// 描画先にα値がある場合は DX_BLENDMODE_NOBLEND でもブレンドモードは DX_BLENDMODE_SRCCOLOR にする
			if( GSYS.DrawSetting.AlphaChDrawMode )
			{
				NextBlendMode = DX_BLENDMODE_SRCCOLOR ;
			}

			// 描画元画像にαチャンネルがある場合やブレンド画像の有無など条件次第で DX_BLENDMODE_ALPHA を使用する
			if( GLINUX.Device.DrawSetting.RenderTexture != NULL )
			{
				if( GLINUX.Device.DrawSetting.BlendTexture != NULL )
				{
					switch( GLINUX.Device.DrawSetting.BlendGraphType )
					{
					case DX_BLENDGRAPHTYPE_NORMAL :
					case DX_BLENDGRAPHTYPE_ALPHA :
						if( GLINUX.Device.DrawSetting.AlphaTestValidFlag    == TRUE ||
							GLINUX.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
						{
							NextBlendMode = DX_BLENDMODE_ALPHA ;
						}
						break ;

					case DX_BLENDGRAPHTYPE_WIPE :
						NextBlendMode = DX_BLENDMODE_ALPHA ;
						break ;
					}
				}
				else
				{
					if( GLINUX.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
					{
						NextBlendMode = DX_BLENDMODE_ALPHA ;
					}
				}
			}
			break ;
		}
	}
	
	// カレントテクスチャが無い場合とある場合で分岐
	if( GLINUX.Device.DrawSetting.RenderTexture == NULL )
	{
		// カレントテクスチャが無い場合は線や箱の描画と判断、テクスチャステージは無効にする

		// とりあえず描画処理を行う
		GLINUX.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

		// アルファテストパラメータが有効な場合はそれを優先する
		if( GLINUX.Device.DrawSetting.AlphaTestMode != -1 )
		{
			GLINUX.Device.State.AlphaTestEnable = TRUE ;
			AlphaTestCmpMode = GLINUX.Device.DrawSetting.AlphaTestMode ;
			AlphaTestRef     = GLINUX.Device.DrawSetting.AlphaTestParam ;
		}
		else
		{
			GLINUX.Device.State.AlphaTestEnable = FALSE ;
		}

		// 頂点タイプのセット
		GLINUX.Device.DrawInfo.VertexType = VERTEXTYPE_NOTEX ;

		// 使用するシェーダーの取得
		GLINUX.Device.State.AlphaTestModeShaderIndex = Graphics_Linux_Shader_GetAlphaTestModeIndex( GLINUX.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;

		VS        =  &SB->BaseSimple_VS     [ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;
		VSAddress = &SCB->BaseSimple_VS_Code[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;

		FS        = & SB->BaseNoneTex_FS     [ g_DefaultBlendDescArray[ NextBlendMode ].RenderType ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;
		FSAddress = &SCB->BaseNoneTex_FS_Code[ g_DefaultBlendDescArray[ NextBlendMode ].RenderType ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;

		UseShader = &SB->BaseNoneTex_Shader[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ][ g_DefaultBlendDescArray[ NextBlendMode ].RenderType ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;
	}
	else
	{
		// カレントテクスチャがある場合はテクスチャステージブレンドテーブルから値を取得する

		// 使用するテクスチャブレンドステージステータステーブルのインデックスをセット
		IgnoreTextureAlpha = 1 ;
		if( GLINUX.Device.DrawSetting.AlphaChannelValidFlag ||
			GLINUX.Device.DrawSetting.AlphaTestValidFlag    ||
			( GLINUX.Device.State.DepthEnable &&
			  GLINUX.Device.DrawSetting.BlendTexture == NULL ) )
		{
			IgnoreTextureAlpha = 0 ;
		}

		// ブレンドテクスチャーを使用するかどうかで分岐
		if( GLINUX.Device.DrawSetting.BlendTexture != NULL )
		{
			int RenderType ;

			// ブレンドテクスチャーがある場合は AlphaTestValidFlag は無視

			// 使用するテーブルのインデックスを割り出す
			RenderType = g_DefaultBlendDescArray[ NextBlendMode ].RenderType ;

			// 使用するテクスチャーアドレスをセット
			UseTexture[ 0 ] = GLINUX.Device.DrawSetting.RenderTexture ;
			UseTexture[ 1 ] = GLINUX.Device.DrawSetting.BlendTexture ;
			UseTextureNum = 2 ;

			// 頂点タイプのセット
			GLINUX.Device.DrawInfo.VertexType = VERTEXTYPE_BLENDTEX ;

			// ブレンドタイプによって処理を分岐
			switch( GLINUX.Device.DrawSetting.BlendGraphType )
			{
			case DX_BLENDGRAPHTYPE_NORMAL :
				// とりあえず描画処理を行う
				GLINUX.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

				// アルファテストパラメータが有効な場合はそれを優先する
				if( GLINUX.Device.DrawSetting.AlphaTestMode != -1 )
				{
					GLINUX.Device.State.AlphaTestEnable = TRUE ;
					AlphaTestCmpMode = GLINUX.Device.DrawSetting.AlphaTestMode ;
					AlphaTestRef     = GLINUX.Device.DrawSetting.AlphaTestParam ;
				}
				else
				{
					// α処理が有効な場合はα値が０の場合のみ表示されないようにする
					if( GLINUX.Device.DrawSetting.AlphaTestValidFlag == TRUE ||
						g_DefaultBlendDescArray[ NextBlendMode ].BlendEnable == TRUE ||
						GLINUX.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
					{
						GLINUX.Device.State.AlphaTestEnable = TRUE ;
						AlphaTestRef     = 0 ;
						AlphaTestCmpMode = DX_CMP_GREATER ;
					}
					else
					{
						GLINUX.Device.State.AlphaTestEnable = FALSE ;
					}
				}

				// クロスフェード率を設定する
				UseFloatFactorColor = TRUE ;
				FloatFactorColor[ 0 ] = 1.0f ;
				FloatFactorColor[ 1 ] = 1.0f ;
				FloatFactorColor[ 2 ] = 1.0f ;
				FloatFactorColor[ 3 ] = GLINUX.Device.DrawSetting.BlendGraphFadeRatio / 255.0f ;

				// 使用するシェーダーをセット
				GLINUX.Device.State.AlphaTestModeShaderIndex = Graphics_Linux_Shader_GetAlphaTestModeIndex( GLINUX.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;

				VS        =  &SB->BaseSimple_VS     [ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;
				VSAddress = &SCB->BaseSimple_VS_Code[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;

				FS        =  &SB->BaseUseTex_FS     [ 1 ][ RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;
				FSAddress = &SCB->BaseUseTex_FS_Code[ 1 ][ RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;

				UseShader = &SB->BaseUseTex_Shader[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ][ 1 ][ RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;
				break ;

			case DX_BLENDGRAPHTYPE_WIPE :
				// ブレンド境界値が最大の場合は何も描画しない
				GLINUX.Device.DrawInfo.BlendMaxNotDrawFlag = GLINUX.Device.DrawSetting.BlendGraphBorderParam == 255 ? TRUE : FALSE ;

				// float型の FactorColor を使用する
				UseFloatFactorColor = TRUE ;
				FloatFactorColor[ 0 ] = 0.0f ;
				FloatFactorColor[ 1 ] = 0.0f ;
				FloatFactorColor[ 2 ] = 0.0f ;
				FloatFactorColor[ 3 ] = 0.0f ;

				// 境界範囲が１以下かどうかで処理を分岐
				if( GLINUX.Device.DrawSetting.BlendGraphBorderRange <= 1 )
				{
					// ここはもう殆ど例外処理 -------------------------------------------

					// αテストを使用する
					GLINUX.Device.State.AlphaTestEnable = TRUE ;
					AlphaTestRef     = GLINUX.Device.DrawSetting.BlendGraphBorderParam ;
					AlphaTestCmpMode = DX_CMP_GREATEREQUAL ;

					// αブレンド機能ＯＦＦ
					NextBlendMode = DX_BLENDMODE_NOBLEND ;

					// 増分するα値は無し
					FloatFactorColor[ 3 ] = 0.0f ;
					FloatFactorColor[ 2 ] = 1.0f ;

					// もし画像がαチャンネルを使用していて、且つ BlendGraphBorderParam が 0 だったら AlphaRef を０にしない
					if( ( GLINUX.Device.DrawSetting.AlphaChannelValidFlag ||
						  GLINUX.Device.DrawSetting.AlphaTestValidFlag ) &&
						GLINUX.Device.DrawSetting.BlendGraphBorderParam == 0 )
					{
						AlphaTestRef = 1 ;
					}
				}
				else
				{
					int p ;
				
					// αブレンドによるテクスチャブレンド

					// αテストは行わない
					GLINUX.Device.State.AlphaTestEnable = FALSE ;

					// 境界範囲によって４段階
					if( GLINUX.Device.DrawSetting.BlendGraphBorderRange <= 64 )
					{
						// ×４

						p = 1280 - GLINUX.Device.DrawSetting.BlendGraphBorderParam * 1280 / 255 ; 

						// ブレンドテクスチャのブレンド値によって合成パラメータを変更
						if( p < 1024 )
						{
							FloatFactorColor[ 3 ] = - ( float )( 255 - p * 255 / 1024 ) / 255.0f ;
						}
						else
						{
							FloatFactorColor[ 3 ] =   ( float )( 64 * ( p - 1024 ) / 256 ) / 255.0f ;
						}

						FloatFactorColor[ 2 ] = 4.0f ;
					}
					else
					if( GLINUX.Device.DrawSetting.BlendGraphBorderRange <= 128 )
					{
						// ×２

						p = 768 - GLINUX.Device.DrawSetting.BlendGraphBorderParam * 768 / 255 ; 

						// ブレンドテクスチャのブレンド値によって合成パラメータを変更
						if( p < 512 )
						{
							FloatFactorColor[ 3 ] = - ( float )( 255 - p * 255 / 512 ) / 255.0f ;
						}
						else
						{
							FloatFactorColor[ 3 ] =   ( float )( 128 * ( p - 512 ) / 256 ) / 255.0f ;
						}

						FloatFactorColor[ 2 ] = 2.0f ;
					}
					else
					{
						// ×１

						p = 512 - GLINUX.Device.DrawSetting.BlendGraphBorderParam * 512 / 255 ; 

						// ブレンドテクスチャのブレンド値によって合成パラメータを変更
						if( p < 256 )
						{
							FloatFactorColor[ 3 ] = - ( float )( 255 - p * 255 / 256 ) / 255.0f ;
						}
						else
						{
							FloatFactorColor[ 3 ] =   ( float )( 255 * ( p - 256 ) / 256 ) / 255.0f ;
						}

						FloatFactorColor[ 2 ] = 1.0f ;
					}
				}

				// 使用するシェーダーをセット
				GLINUX.Device.State.AlphaTestModeShaderIndex = Graphics_Linux_Shader_GetAlphaTestModeIndex( GLINUX.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;

				VS        =  &SB->BaseSimple_VS     [ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;
				VSAddress = &SCB->BaseSimple_VS_Code[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;

				FS        =  &SB->BaseUseTex_FS     [ 2 ][ RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;
				FSAddress = &SCB->BaseUseTex_FS_Code[ 2 ][ RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;

				UseShader = &SB->BaseUseTex_Shader[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ][ 2 ][ RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;
				break ;

			case DX_BLENDGRAPHTYPE_ALPHA :
				// とりあえず描画処理を行う
				GLINUX.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

				// アルファテストパラメータが有効な場合はそれを優先する
				if( GLINUX.Device.DrawSetting.AlphaTestMode != -1 )
				{
					GLINUX.Device.State.AlphaTestEnable = TRUE ;
					AlphaTestCmpMode = GLINUX.Device.DrawSetting.AlphaTestMode ;
					AlphaTestRef     = GLINUX.Device.DrawSetting.AlphaTestParam ;
				}
				else
				{
					// α処理が有効な場合はα値が０の場合のみ表示されないようにする
					if( g_DefaultBlendDescArray[ NextBlendMode ].BlendEnable == TRUE )
					{
						GLINUX.Device.State.AlphaTestEnable = TRUE ;
						AlphaTestRef     = 0 ;
						AlphaTestCmpMode = DX_CMP_GREATER ;
					}
					else
					{
						GLINUX.Device.State.AlphaTestEnable = FALSE ;
					}
				}

				// 使用するシェーダーをセット
				GLINUX.Device.State.AlphaTestModeShaderIndex = Graphics_Linux_Shader_GetAlphaTestModeIndex( GLINUX.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;

				VS        =  &SB->BaseSimple_VS     [ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;
				VSAddress = &SCB->BaseSimple_VS_Code[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;

				FS        =  &SB->BaseUseTex_FS     [ 3 ][ RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;
				FSAddress = &SCB->BaseUseTex_FS_Code[ 3 ][ RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;

				UseShader = &SB->BaseUseTex_Shader[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ][ 3 ][ RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;
				break ;
			}
		}
		else
		{
			// ブレンドテクスチャーが無い場合

			// とりあえず描画処理を行う
			GLINUX.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

			// 頂点タイプのセット
			GLINUX.Device.DrawInfo.VertexType = VERTEXTYPE_TEX ;

			// アルファテストパラメータが有効な場合はそれを優先する
			if( GLINUX.Device.DrawSetting.AlphaTestMode != -1 )
			{
				GLINUX.Device.State.AlphaTestEnable = TRUE ;
				AlphaTestCmpMode = GLINUX.Device.DrawSetting.AlphaTestMode ;
				AlphaTestRef     = GLINUX.Device.DrawSetting.AlphaTestParam ;
			}
			else
			{
				// αテストを使用するかどうかで処理を分岐
				if( GLINUX.Device.DrawSetting.AlphaTestValidFlag == TRUE )
				{
					// αテストを行う(透過色処理以外にαテストはブレンドテクスチャでも使用している)
					GLINUX.Device.State.AlphaTestEnable = TRUE ;
					AlphaTestRef     = 16 ;
					AlphaTestCmpMode = DX_CMP_GREATER ;
				}
				else
				{
					// α処理が有効な場合はブレンドモードによってはα値が０の場合のみ表示されないようにする
					if( g_DefaultBlendDescArray[ NextBlendMode ].BlendEnable == TRUE &&
						g_DefaultBlendDescArray[ NextBlendMode ].AlphaZeroNotDrawFlag == TRUE )
					{
						GLINUX.Device.State.AlphaTestEnable = TRUE ;
						AlphaTestRef     = 0 ;
						AlphaTestCmpMode = DX_CMP_GREATER ;
					}
					else
					{
						GLINUX.Device.State.AlphaTestEnable = FALSE ;
					}
				}
			}

			// 使用するフラグメントシェーダーをセット
			GLINUX.Device.State.AlphaTestModeShaderIndex = Graphics_Linux_Shader_GetAlphaTestModeIndex( GLINUX.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;

			VS        =  &SB->BaseSimple_VS     [ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;
			VSAddress = &SCB->BaseSimple_VS_Code[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ] ;

			FS        =  &SB->BaseUseTex_FS     [ 0 ][ g_DefaultBlendDescArray[ NextBlendMode ].RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;
			FSAddress = &SCB->BaseUseTex_FS_Code[ 0 ][ g_DefaultBlendDescArray[ NextBlendMode ].RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;

			UseShader = &SB->BaseUseTex_Shader[ g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ][ 0 ][ g_DefaultBlendDescArray[ NextBlendMode ].RenderType ][ GLINUX.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GLINUX.Device.State.AlphaTestModeShaderIndex ] ;

			// 使用するテクスチャーアドレスをセット
			UseTexture[ 0 ] = GLINUX.Device.DrawSetting.RenderTexture ;

			// 使用するテクスチャの数は一つ
			UseTextureNum = 1 ;
		}
	}

	// 頂点シェーダーがあるかどうかを調べる
	if( *VS == 0 )
	{
		// 頂点シェーダーの作成を試みる
		if( Graphics_Linux_VertexShader_Create( VSAddress, VS, 1 ) != 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x41\x00\x6e\x00\x64\x00\x72\x00\x6f\x00\x69\x00\x64\x00\x5f\x00\x44\x00\x65\x00\x76\x00\x69\x00\x63\x00\x65\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x44\x00\x72\x00\x61\x00\x77\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_Linux_DeviceState_NormalDrawSetup で頂点シェーダーの作成に失敗しました\n" @*/ )) ;
			return -1 ;
		}
	}

	// フラグメントシェーダーがあるかどうかを調べる
	if( *FS == 0 )
	{
		// フラグメントシェーダーの作成を試みる
		if( Graphics_Linux_FragmentShader_Create( FSAddress, FS, 1 ) != 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x41\x00\x6e\x00\x64\x00\x72\x00\x6f\x00\x69\x00\x64\x00\x5f\x00\x44\x00\x65\x00\x76\x00\x69\x00\x63\x00\x65\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x44\x00\x72\x00\x61\x00\x77\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\xd5\x30\xe9\x30\xb0\x30\xe1\x30\xf3\x30\xc8\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_Linux_DeviceState_NormalDrawSetup でフラグメントシェーダーの作成に失敗しました\n" @*/ )) ;
			return -1 ;
		}
	}

	// シェーダーが作成されていなかったらシェーダーの作成を試みる
	if( *UseShader == NULL )
	{
		*UseShader = Graphics_Linux_GetShaderStruct() ;
		if( *UseShader == NULL )
		{
			return -1 ;
		}
	}
	if( ( *UseShader )->Shader == 0 )
	{
		if( Graphics_Linux_Shader_Create( *UseShader, *VS, *FS ) != 0 )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x41\x00\x6e\x00\x64\x00\x72\x00\x6f\x00\x69\x00\x64\x00\x5f\x00\x44\x00\x65\x00\x76\x00\x69\x00\x63\x00\x65\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x44\x00\x72\x00\x61\x00\x77\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_Linux_DeviceState_NormalDrawSetup でシェーダーの作成に失敗しました\n" @*/ )) ;
			return -1 ;
		}
	}

	if( UseTextureNum > 0 )
	{
		int i ;

		for( i = 0 ; i < UseTextureNum ; i ++ )
		{
			Graphics_Linux_DeviceState_SetTexture( i, UseTexture[ i ] ) ;
		}
	}

	if( GLINUX.Device.State.AlphaTestEnable )
	{
		Graphics_Linux_DeviceState_SetAlphaTestRef( AlphaTestRef ) ;
		Graphics_Linux_DeviceState_SetAlphaTestCmpMode( AlphaTestCmpMode ) ;
	}
	else
	{
		Graphics_Linux_DeviceState_SetAlphaTestRef( -1 ) ;
		Graphics_Linux_DeviceState_SetAlphaTestCmpMode( DX_CMP_GREATER ) ;
	}

	if( UseFloatFactorColor )
	{
		Graphics_Linux_DeviceState_SetFactorColor( &FloatFactorColor ) ;
	}

	Graphics_Linux_DeviceState_SetBlendMode(
		NextBlendMode,
		NextBlendEnable,
		NextBlendRGBSrc,
		NextBlendRGBDest,
		NextBlendRGBOp,
		NextBlendASrc,
		NextBlendADest,
		NextBlendAOp,
		GLINUX.Device.DrawSetting.NotWriteAlphaChannelFlag
	) ;
	Graphics_Linux_DeviceState_SetShader( *UseShader, TRUE ) ;

	// 終了
	return 0 ;
}


























// 描画設定関係関数

// 描画ブレンドモードの設定
extern int Graphics_Linux_DrawSetting_SetDrawBlendMode( int BlendMode, int BlendEnable, int BlendRGBSrc, int BlendRGBDest, int BlendRGBOp, int BlendASrc, int BlendADest, int BlendAOp, int AlphaTestValidFlag, int AlphaChannelValidFlag )
{
	if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck	== FALSE &&
		GLINUX.Device.DrawSetting.BlendMode					== BlendMode &&
		GLINUX.Device.DrawSetting.BlendEnable				== BlendEnable  &&
		GLINUX.Device.DrawSetting.BlendRGBSrc				== BlendRGBSrc  &&
		GLINUX.Device.DrawSetting.BlendRGBDest				== BlendRGBDest &&
		GLINUX.Device.DrawSetting.BlendRGBOp					== BlendRGBOp   &&
		GLINUX.Device.DrawSetting.BlendASrc					== BlendASrc    &&
		GLINUX.Device.DrawSetting.BlendADest					== BlendADest   &&
		GLINUX.Device.DrawSetting.BlendAOp					== BlendAOp     &&
		GLINUX.Device.DrawSetting.AlphaTestValidFlag			== AlphaTestValidFlag &&
		GLINUX.Device.DrawSetting.AlphaChannelValidFlag		== AlphaChannelValidFlag )
	{
		return 0 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	GLINUX.Device.DrawSetting.BlendMode             = BlendMode ;
	GLINUX.Device.DrawSetting.BlendEnable		   = BlendEnable ;
	GLINUX.Device.DrawSetting.BlendRGBSrc		   = BlendRGBSrc ;
	GLINUX.Device.DrawSetting.BlendRGBDest		   = BlendRGBDest ;
	GLINUX.Device.DrawSetting.BlendRGBOp			   = BlendRGBOp ;
	GLINUX.Device.DrawSetting.BlendASrc			   = BlendASrc ;
	GLINUX.Device.DrawSetting.BlendADest			   = BlendADest ; 
	GLINUX.Device.DrawSetting.BlendAOp			   = BlendAOp ;
	GLINUX.Device.DrawSetting.AlphaChannelValidFlag = AlphaChannelValidFlag ;
	GLINUX.Device.DrawSetting.AlphaTestValidFlag    = AlphaTestValidFlag ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag    = TRUE ;

	// パラメータが変更された、フラグを立てる
	GLINUX.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// 描画時の画像のＲＧＢを無視するかどうかを設定する
extern int Graphics_Linux_DrawSetting_SetIgnoreDrawGraphColor( int EnableFlag )
{
	if( GLINUX.Device.DrawSetting.IgnoreGraphColorFlag == EnableFlag &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 値を保存
	GLINUX.Device.DrawSetting.IgnoreGraphColorFlag = EnableFlag ;

	// 定数データに反映
	if( GLINUX.Device.DrawSetting.IgnoreGraphColorFlag )
	{
		GLINUX.Device.Shader.Constant.uIgnoreTextureColor[ 0 ] = 1.0f ;
		GLINUX.Device.Shader.Constant.uIgnoreTextureColor[ 1 ] = 1.0f ;
		GLINUX.Device.Shader.Constant.uIgnoreTextureColor[ 2 ] = 1.0f ;
	}
	else
	{
		GLINUX.Device.Shader.Constant.uIgnoreTextureColor[ 0 ] = 0.0f ;
		GLINUX.Device.Shader.Constant.uIgnoreTextureColor[ 1 ] = 0.0f ;
		GLINUX.Device.Shader.Constant.uIgnoreTextureColor[ 2 ] = 0.0f ;
	}

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

//	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// パラメータが変更された、フラグを立てる
	GLINUX.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// 描画時の画像のＡを無視するかどうかを設定する
extern int Graphics_Linux_DrawSetting_SetIgnoreDrawGraphAlpha( int EnableFlag )
{
	if( GLINUX.Device.DrawSetting.IgnoreGraphAlphaFlag == EnableFlag &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 値を保存
	GLINUX.Device.DrawSetting.IgnoreGraphAlphaFlag = EnableFlag ;

	// 定数データに反映
	if( GLINUX.Device.DrawSetting.IgnoreGraphAlphaFlag )
	{
		GLINUX.Device.Shader.Constant.uIgnoreTextureColor[ 3 ] = 1.0f ;
	}
	else
	{
		GLINUX.Device.Shader.Constant.uIgnoreTextureColor[ 3 ] = 0.0f ;
	}

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// パラメータが変更された、フラグを立てる
	GLINUX.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// 描画先のアルファチャンネルの内容を書き換えるかを設定する
extern int Graphics_Linux_DrawSetting_SetWriteAlphaChannelFlag( int NotFlag )
{
	if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GLINUX.Device.DrawSetting.NotWriteAlphaChannelFlag == NotFlag )
	{
		return 0 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	GLINUX.Device.DrawSetting.NotWriteAlphaChannelFlag = NotFlag ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag    = TRUE ;

	// パラメータが変更された、フラグを立てる
	GLINUX.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// アルファテストの設定
extern int Graphics_Linux_DrawSetting_SetDrawAlphaTest( int TestMode, int TestParam )
{
	if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GLINUX.Device.DrawSetting.AlphaTestMode  == TestMode &&
		GLINUX.Device.DrawSetting.AlphaTestParam == TestParam )
	{
		return 0 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	GLINUX.Device.DrawSetting.AlphaTestMode      = TestMode ;
	GLINUX.Device.DrawSetting.AlphaTestParam     = TestParam ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// パラメータが変更された、フラグを立てる
	GLINUX.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// ブレンドするテクスチャのパラメータをセットする
extern int Graphics_Linux_DrawSetting_SetBlendTextureParam( int BlendType, int *Param )
{
	// パラメータを保存
	switch( BlendType )
	{
	case DX_BLENDGRAPHTYPE_NORMAL :
		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
			BlendType == GLINUX.Device.DrawSetting.BlendGraphType &&
			GLINUX.Device.DrawSetting.BlendGraphFadeRatio == Param[ 0 ] )
		{
			return 0 ;
		}

		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// 描画待機している描画物を描画
			DRAWSTOCKINFO
		}

		GLINUX.Device.DrawSetting.BlendGraphFadeRatio = Param[ 0 ] ;
		break;

	case DX_BLENDGRAPHTYPE_WIPE:
		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
			BlendType == GLINUX.Device.DrawSetting.BlendGraphType &&
			GLINUX.Device.DrawSetting.BlendGraphBorderParam == Param[ 0 ] &&
			GLINUX.Device.DrawSetting.BlendGraphBorderRange == Param[ 1 ] )
		{
			return 0 ;
		}

		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// 描画待機している描画物を描画
			DRAWSTOCKINFO
		}

		GLINUX.Device.DrawSetting.BlendGraphBorderParam = Param[ 0 ] ;
		GLINUX.Device.DrawSetting.BlendGraphBorderRange = Param[ 1 ] ;
		break;

	case DX_BLENDGRAPHTYPE_ALPHA :
		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
			BlendType == GLINUX.Device.DrawSetting.BlendGraphType )
		{
			return 0 ;
		}

		if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// 描画待機している描画物を描画
			DRAWSTOCKINFO
		}
		break;
	}

	// ブレンドタイプを保存する
	GLINUX.Device.DrawSetting.BlendGraphType = BlendType ;

	// パラメータが変更された、フラグを立てる
	GLINUX.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// テクスチャをセットする 
extern int Graphics_Linux_DrawSetting_SetTexture( GRAPHICS_LINUX_TEXTURE *RenderTexture )
{
	if( RenderTexture == GLINUX.Device.DrawSetting.RenderTexture &&
		GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// 描画待機している描画物を描画
		DRAWSTOCKINFO
	}

	// テクスチャが変更された、フラグを立てる
	GLINUX.Device.DrawSetting.ChangeTextureFlag = TRUE ;

	// テクスチャの有り無しが変更された場合はブレンドパラメータの変更も行うフラグもセットする
	if( ( GLINUX.Device.DrawSetting.RenderTexture == NULL && RenderTexture != NULL ) ||
		( GLINUX.Device.DrawSetting.RenderTexture != NULL && RenderTexture == NULL ) )
	{
		GLINUX.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
	}

	// テクスチャセット
	GLINUX.Device.DrawSetting.RenderTexture      = RenderTexture ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// 終了
	return 0 ;
}

// ブレンドするテクスチャをセットする 
extern int Graphics_Linux_DrawSetting_SetBlendTexture( GRAPHICS_LINUX_TEXTURE *BlendTexture )
{
	if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		BlendTexture == GLINUX.Device.DrawSetting.BlendTexture )
	{
		return 0 ;
	}

	if( GLINUX.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// 描画待機している描画物を描画
		DRAWSTOCKINFO
	}

	// テクスチャが変更された、フラグを立てる
	GLINUX.Device.DrawSetting.ChangeTextureFlag = TRUE ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// ブレンドテクスチャの有り無しが変更された場合はブレンドパラメータの変更も行うフラグもセットする
	if( ( GLINUX.Device.DrawSetting.BlendTexture == NULL && BlendTexture != NULL ) ||
		( GLINUX.Device.DrawSetting.BlendTexture != NULL && BlendTexture == NULL ) )
	{
		GLINUX.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
	}

	// テクスチャセット
	GLINUX.Device.DrawSetting.BlendTexture    = BlendTexture ;

	// テクスチャの幅と高さを保存
	if( BlendTexture != NULL )
	{
		GLINUX.Device.DrawSetting.BlendTextureWidth  = ( float )BlendTexture->Width  ;
		GLINUX.Device.DrawSetting.BlendTextureHeight = ( float )BlendTexture->Height ;
		GLINUX.Device.DrawSetting.InvBlendTextureWidth  = 1.0F / GLINUX.Device.DrawSetting.BlendTextureWidth ;
		GLINUX.Device.DrawSetting.InvBlendTextureHeight = 1.0F / GLINUX.Device.DrawSetting.BlendTextureHeight ;
	}

	// 終了
	return 0 ;
}
































// Linux の描画処理準備関係

#ifndef DX_NON_ASYNCLOAD
static int Graphics_Linux_RenderVertexASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO * Info )
{
	return Graphics_Linux_RenderVertex(
		( int )Info->Data[ 0 ],
		FALSE ) ;
}
#endif // DX_NON_ASYNCLOAD

// 頂点バッファに溜まった頂点データをレンダリングする
extern	int		Graphics_Linux_RenderVertex( int NextUse3DVertex, int ASyncThread )
{
	DWORD InputLayout ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = Graphics_Linux_RenderVertexASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )NextUse3DVertex ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	InputLayout = g_VertexTypeToInputLayout[ GLINUX.Device.DrawInfo.Use3DVertex ][ GLINUX.Device.DrawInfo.VertexType ] ;

	// 頂点が一つも無かったら描画は行わない
	if( GLINUX.Device.DrawInfo.VertexNum != 0 && DxLib_GetEndRequest() == FALSE && GLINUX.Device.State.SetShader != NULL )
	{
		// 非描画フラグが立っていなければレンダリングする
		if( GLINUX.Device.DrawInfo.BlendMaxNotDrawFlag == FALSE )
		{
			// シェーダーをセット
			if( InputLayout == LINUX_VERTEX_INPUTLAYOUT_3D_LIGHT )
			{
				if( Graphics_Linux_Shader_Normal3DDraw_Setup() == FALSE )
				{
					return -1 ;
				}
			}
//			else
//			{
//				Graphics_Linux_DeviceState_SetVertexShader( &GLINUX.Device.Shader.Base.BaseSimple_VS[ InputLayout ], TRUE ) ;
//			}

			// テクスチャのセット
			for( int i = 0; i < USE_TEXTURESTAGE_NUM; i++ )
			{
				if( GLINUX.Device.State.SetTexture[ i ] == NULL )
				{
					continue ;
				}

				glActiveTexture( g_TextureEnum[ i ] ) ;
				glBindTexture( GL_TEXTURE_2D, GLINUX.Device.State.SetTexture[ i ]->TextureBuffer ) ;
			}

			// Uniform の更新
			Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

			// 頂点データのセットアップ
			Graphics_Linux_DeviceState_SetupShaderVertexData(
				GLINUX.Device.State.SetShader,
				&g_BaseSimpleVertexShaderInfo[ InputLayout ].InputInfo,
				GLINUX.Device.DrawInfo.VertexBuffer
			) ;

			// 描画
			glDrawArrays( GLINUX.Device.DrawInfo.PrimitiveType, 0, GLINUX.Device.DrawInfo.VertexNum );
			GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;
		}
	}

	GLINUX.Device.DrawInfo.VertexNum = 0 ;
	GLINUX.Device.DrawInfo.VertexBufferNextAddr = GLINUX.Device.DrawInfo.VertexBuffer ;

	if( NextUse3DVertex >= 0 )
	{
		int PrevUse3DVertex = GLINUX.Device.DrawInfo.Use3DVertex ;

		GLINUX.Device.DrawInfo.Use3DVertex = NextUse3DVertex ;

		if( PrevUse3DVertex != NextUse3DVertex )
		{
			Graphics_Linux_DeviceState_NormalDrawSetup() ;
		}
	}

	// 終了
	return 0 ;
}

// 描画準備を行う
extern	void	FASTCALL Graphics_Linux_DrawPreparation( int ParamFlag )
{
	int AlphaTest ;
	int AlphaChannel ;
	int Specular ;
	int i ;

	// 必ず Graphics_Linux_DrawPreparation を行うべきというフラグを倒す
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = FALSE ;

	// 設定が変更されたかどうかのフラグを倒す
	GSYS.ChangeSettingFlag = FALSE ;

	// ライトを使うかどうかで処理を分岐
	if( ( ParamFlag & DX_LINUX_DRAWPREP_LIGHTING ) != 0 && GSYS.Light.ProcessDisable == FALSE )
	{
		// ライトの設定が変更されていたら変更を適応する
		if( GSYS.Light.HardwareChange )
		{
			Graphics_Light_RefreshState() ;
		}
		else
		{
			// ライトが有効なのに一時的に無効になっていたら有効にする
			if( GLINUX.Device.State.Lighting == 0 )
			{
				Graphics_Linux_DeviceState_SetLighting( TRUE ) ;
			}
		}

		// マテリアルの設定が外部から変更されていたら元に戻す
		if( GSYS.Light.ChangeMaterial )
		{
			GSYS.Light.ChangeMaterial = 0 ;
			Graphics_Linux_DeviceState_SetMaterial( &GSYS.Light.Material ) ;
		}

		// 頂点カラーをマテリアルのカラーとして使用するかどうかをセットする
		if( ( GSYS.Light.MaterialNotUseVertexDiffuseColor ? FALSE : TRUE ) != GLINUX.Device.State.MaterialUseVertexDiffuseColor )
		{
			Graphics_Linux_DeviceState_SetUseVertexDiffuseColor( GSYS.Light.MaterialNotUseVertexDiffuseColor ? FALSE : TRUE ) ;
		}

		if( ( GSYS.Light.MaterialNotUseVertexSpecularColor ? FALSE : TRUE ) != GLINUX.Device.State.MaterialUseVertexSpecularColor )
		{
			Graphics_Linux_DeviceState_SetUseVertexSpecularColor( GSYS.Light.MaterialNotUseVertexSpecularColor ? FALSE : TRUE ) ;
		}
	}
	else
	{
		// ライトが有効になっていたら無効にする
		if( GLINUX.Device.State.Lighting == 1 )
		{
			Graphics_Linux_DeviceState_SetLighting( FALSE ) ;
		}

		// 頂点カラーをマテリアルのカラーとして使用する
		if( GLINUX.Device.State.MaterialUseVertexDiffuseColor == FALSE )
		{
			Graphics_Linux_DeviceState_SetUseVertexDiffuseColor( TRUE ) ;
		}

		if( GLINUX.Device.State.MaterialUseVertexSpecularColor == FALSE )
		{
			Graphics_Linux_DeviceState_SetUseVertexSpecularColor( TRUE ) ;
		}
	}

	// スペキュラを使用するかどうかの設定を行う
	Specular = ( GSYS.DrawSetting.NotUseSpecular == FALSE && ( ParamFlag & DX_LINUX_DRAWPREP_SPECULAR ) != 0 ) ? TRUE : FALSE ;
	if( Specular != GLINUX.Device.State.UseSpecular )
	{
		Graphics_Linux_DeviceState_SetSpecularEnable( Specular ) ;
	}

	// 射影行列の設定を行う
//	Graphics_DrawSetting_SetUse2DProjectionMatrix( ( ParamFlag & DX_LINUX_DRAWPREP_3D ) == 0 ? TRUE : FALSE ) ;

	// Ｚバッファの設定を行う
	{
		BOOL			DepthEnable ;
		int				WriteDepthBuffer ;
		GLenum			DepthFunc ;
		int				DepthBias ;

		if( ParamFlag & DX_LINUX_DRAWPREP_3D )
		{
			DepthEnable      = GSYS.DrawSetting.EnableZBufferFlag3D ;
			WriteDepthBuffer = GSYS.DrawSetting.WriteZBufferFlag3D ;
			DepthFunc        = g_DXCmpModeToGLES2CompareFunc[ GSYS.DrawSetting.ZBufferCmpType3D ] ;
			DepthBias        = GSYS.DrawSetting.ZBias3D ;
		}
		else
		{
			DepthEnable      = GSYS.DrawSetting.EnableZBufferFlag2D ;
			WriteDepthBuffer = GSYS.DrawSetting.WriteZBufferFlag2D ;
			DepthFunc        = g_DXCmpModeToGLES2CompareFunc[ GSYS.DrawSetting.ZBufferCmpType2D ] ;
			DepthBias        = GSYS.DrawSetting.ZBias2D ;
		}

		if( DepthEnable )
		{
			if( GLINUX.Device.State.DepthEnable != DepthEnable )
			{
				Graphics_Linux_DeviceState_SetDepthEnable( DepthEnable ) ;
			}

			if( GLINUX.Device.State.DepthWriteEnable != WriteDepthBuffer  )
			{
				Graphics_Linux_DeviceState_SetDepthWriteEnable( WriteDepthBuffer  ) ;
			}

			if( GLINUX.Device.State.DepthFunc != DepthFunc )
			{
				Graphics_Linux_DeviceState_SetDepthFunc( DepthFunc ) ;
			}

			if( GLINUX.Device.State.DepthBias != DepthBias )
			{
				Graphics_Linux_DeviceState_SetDepthBias( DepthBias ) ;
			}
		}
		else
		{
			if( GLINUX.Device.State.DepthEnable )
			{
				Graphics_Linux_DeviceState_SetDepthEnable( FALSE ) ;
			}

			if( GLINUX.Device.State.DepthWriteEnable != FALSE  )
			{
				Graphics_Linux_DeviceState_SetDepthWriteEnable( FALSE ) ;
			}
		}
	}

	// フォグの設定を行う
	if( ParamFlag & DX_LINUX_DRAWPREP_FOG )
	{
		if( GLINUX.Device.State.FogEnable != GSYS.DrawSetting.FogEnable )
		{
			Graphics_Linux_DeviceState_SetFogEnable( GSYS.DrawSetting.FogEnable ) ;
		}
	}
	else
	{
		if( GLINUX.Device.State.FogEnable )
		{
			Graphics_Linux_DeviceState_SetFogEnable( FALSE ) ;
		}
	}

	// ブレンディング関係のセッティングを行う場合のみ実行する
	if( ( ParamFlag & DX_LINUX_DRAWPREP_NOBLENDSETTING ) == 0 )
	{
		// フラグの初期化
		AlphaTest    = FALSE ;
		AlphaChannel = FALSE ;

		// テクスチャーを使用するかどうかで処理を分岐
		// 使用しない場合は初期値のまま
		if( ParamFlag & DX_LINUX_DRAWPREP_TEXTURE )
		{
			// 透過色処理を行わない場合はカラーキーもαテストもαチャンネルも使用しないので初期値のまま
			// 透過色処理を行う場合のみ処理をする
			if( ParamFlag & DX_LINUX_DRAWPREP_TRANS )
			{
				int TexAlphaTestFlag = ( ParamFlag & DX_LINUX_DRAWPREP_TEXALPHATEST ) != 0 ? 1 : 0 ;
				int TexAlphaChFlag   = ( ParamFlag & DX_LINUX_DRAWPREP_TEXALPHACH   ) != 0 ? 1 : 0 ;

				// テクスチャにαビットがあるかどうかで処理を分岐
				if( TexAlphaTestFlag || TexAlphaChFlag )
				{
					// αテストでも頂点座標のデータ型が浮動小数点型で、且つテクスチャフィルタリングモードが
					// 線形補間であるか、ブレンドモードが DX_BLENDMODE_NOBLEND 以外だったらαチャンネルとして扱う
					if( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendMode != DX_BLENDMODE_DESTCOLOR ) ||
						( ( ParamFlag & DX_LINUX_DRAWPREP_VECTORINT ) == 0   && GSYS.DrawSetting.DrawMode  == DX_DRAWMODE_BILINEAR   ) )
					{
						AlphaChannel = TRUE ;
					}
					else
					{
						// それ以外の場合はテクスチャーのフラグに委ねる
						AlphaTest    = TexAlphaTestFlag ;
						AlphaChannel = TexAlphaChFlag ;
					}
				}
			}
		}

		// 調整されたパラメータをセットする
		if( GLINUX.Device.DrawSetting.IgnoreGraphColorFlag  != GSYS.DrawSetting.IgnoreGraphColorFlag )
		{
			Graphics_Linux_DrawSetting_SetIgnoreDrawGraphColor( GSYS.DrawSetting.IgnoreGraphColorFlag ) ;
		}

		if( GLINUX.Device.DrawSetting.NotWriteAlphaChannelFlag  != GSYS.DrawSetting.NotWriteAlphaChannelFlag )
		{
			Graphics_Linux_DrawSetting_SetWriteAlphaChannelFlag( GSYS.DrawSetting.NotWriteAlphaChannelFlag ) ;
		}

		if( GLINUX.Device.DrawSetting.BlendMode             != GSYS.DrawSetting.BlendMode ||
			GLINUX.Device.DrawSetting.BlendEnable           != GSYS.DrawSetting.BlendEnable  ||
			GLINUX.Device.DrawSetting.BlendRGBSrc           != GSYS.DrawSetting.BlendRGBSrc  ||
			GLINUX.Device.DrawSetting.BlendRGBDest          != GSYS.DrawSetting.BlendRGBDest ||
			GLINUX.Device.DrawSetting.BlendRGBOp            != GSYS.DrawSetting.BlendRGBOp   ||
			GLINUX.Device.DrawSetting.BlendASrc             != GSYS.DrawSetting.BlendASrc    ||
			GLINUX.Device.DrawSetting.BlendADest            != GSYS.DrawSetting.BlendADest   ||
			GLINUX.Device.DrawSetting.BlendAOp              != GSYS.DrawSetting.BlendAOp     ||
			GLINUX.Device.DrawSetting.AlphaTestValidFlag    != AlphaTest ||
			GLINUX.Device.DrawSetting.AlphaChannelValidFlag != AlphaChannel               )
		{
			Graphics_Linux_DrawSetting_SetDrawBlendMode(
				GSYS.DrawSetting.BlendMode,
				GSYS.DrawSetting.BlendEnable,
				GSYS.DrawSetting.BlendRGBSrc,
				GSYS.DrawSetting.BlendRGBDest,
				GSYS.DrawSetting.BlendRGBOp,
				GSYS.DrawSetting.BlendASrc,
				GSYS.DrawSetting.BlendADest,
				GSYS.DrawSetting.BlendAOp,
				AlphaTest,
				AlphaChannel
			) ;
		}

		if( GLINUX.Device.DrawSetting.AlphaTestMode         != GSYS.DrawSetting.AlphaTestMode ||
			GLINUX.Device.DrawSetting.AlphaTestParam        != GSYS.DrawSetting.AlphaTestParam )
		{
			Graphics_Linux_DrawSetting_SetDrawAlphaTest( GSYS.DrawSetting.AlphaTestMode, GSYS.DrawSetting.AlphaTestParam ) ;
		}

		if( GLINUX.Device.DrawSetting.ChangeBlendParamFlag ||
			GLINUX.Device.DrawSetting.ChangeTextureFlag ||
			( GSYS.HardInfo.UseShader &&
			  GLINUX.Device.State.SetNormalShader == FALSE ) )
		{
			Graphics_Linux_DeviceState_NormalDrawSetup() ;
		}
	}

	if( GLINUX.Device.State.DrawMode != GSYS.DrawSetting.DrawMode )
	{
		Graphics_Linux_DeviceState_SetDrawMode( GSYS.DrawSetting.DrawMode ) ;
	}

	if( GLINUX.Device.State.MaxAnisotropy != GSYS.DrawSetting.MaxAnisotropy )
	{
		Graphics_Linux_DeviceState_SetMaxAnisotropy( GSYS.DrawSetting.MaxAnisotropy ) ;
	}

	if( ParamFlag & DX_LINUX_DRAWPREP_CULLING )
	{
		int SetCullMode ;

		if( GLINUX.Device.State.CullEnable == FALSE )
		{
			SetCullMode = 0 ;
		}
		else
		{
			switch( GLINUX.Device.State.CullFace )
			{
			default :
				SetCullMode = 0 ;
				break ;

			case GL_FRONT :
				SetCullMode = 1 ;
				break ;

			case GL_BACK :
				SetCullMode = 2 ;
				break ;
			}
		}

		if( SetCullMode != GSYS.DrawSetting.CullMode )
		{
			Graphics_Linux_DeviceState_SetCullMode( GSYS.DrawSetting.CullMode ) ;
		}
	}
	else
	{
		if( GLINUX.Device.State.CullEnable )
		{
			Graphics_Linux_DeviceState_SetCullMode( DX_CULLING_NONE ) ;
		}
	}

	if( GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix == FALSE )
	{
		Graphics_Linux_DeviceState_SetTextureAddressTransformMatrix( GSYS.DrawSetting.TextureAddressTransformUse, &GSYS.DrawSetting.TextureAddressTransformMatrix ) ;
		GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix = TRUE ;
		GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	}

	if( ParamFlag & DX_LINUX_DRAWPREP_TEXADDRESS )
	{
		for( i = 0; i < USE_TEXTURESTAGE_NUM; i++ )
		{
			if( GSYS.DrawSetting.TexAddressModeU[ i ] == GSYS.DrawSetting.TexAddressModeV[ i ] )
			{
				if( ( DWORD )GLINUX.Device.State.TexAddressModeU[ i ] != g_DXTexAddrModeToGLES2WrapMode[ GSYS.DrawSetting.TexAddressModeU[ i ] ] ||
					( DWORD )GLINUX.Device.State.TexAddressModeV[ i ] != g_DXTexAddrModeToGLES2WrapMode[ GSYS.DrawSetting.TexAddressModeV[ i ] ] )
				{
					Graphics_Linux_DeviceState_SetTextureAddress( GSYS.DrawSetting.TexAddressModeU[ i ], i ) ;
				}
			}
			else
			{
				if( ( DWORD )GLINUX.Device.State.TexAddressModeU[ i ] != g_DXTexAddrModeToGLES2WrapMode[ GSYS.DrawSetting.TexAddressModeU[ i ] ] )
				{
					Graphics_Linux_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ i ], i ) ;
				}

				if( ( DWORD )GLINUX.Device.State.TexAddressModeV[ i ] != g_DXTexAddrModeToGLES2WrapMode[ GSYS.DrawSetting.TexAddressModeV[ i ] ] )
				{
					Graphics_Linux_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ i ], i ) ;
				}
			}
		}
	}
	else
	{
		for( i = 0; i < 4; i++ )
		{
			if( GLINUX.Device.State.TexAddressModeU[ i ] != GL_CLAMP_TO_EDGE ||
				GLINUX.Device.State.TexAddressModeV[ i ] != GL_CLAMP_TO_EDGE )
			{
				Graphics_Linux_DeviceState_SetTextureAddress( DX_TEXADDRESS_CLAMP, i ) ;
			}
		}
	}

	// RenderBegin を実行していなかったら実行する
	if( GLINUX.Device.DrawInfo.BeginSceneFlag == FALSE )
	{
		Graphics_Linux_RenderBegin() ;
	}

	// パラメータを保存
	GLINUX.Device.DrawSetting.DrawPrepParamFlag = ParamFlag ;
}


// 描画コマンドを開始する
extern	void	Graphics_Linux_RenderBegin( void )
{
	if( GLINUX.Device.DrawInfo.BeginSceneFlag == TRUE )
	{
		return ;
	}

	GLINUX.Device.DrawInfo.BeginSceneFlag = TRUE ;
	GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
}

// 描画コマンドを終了する
extern	void	Graphics_Linux_RenderEnd( void )
{
	if( GLINUX.Device.DrawInfo.BeginSceneFlag == FALSE )
	{
		return ;
	}

	GLINUX.Device.DrawInfo.BeginSceneFlag = FALSE ;
}



























// Linux を使った描画関係

// 頂点バッファに溜まった頂点データをレンダリングする
extern	int		Graphics_Hardware_RenderVertex( int ASyncThread )
{
	return Graphics_Linux_RenderVertex( -1, ASyncThread ) ;
}

// ハードウエアアクセラレータ使用版 DrawBillboard3D
extern	int		Graphics_Hardware_DrawBillboard3D_PF( VECTOR Pos, float cx, float cy, float Size, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int ReverseXFlag, int ReverseYFlag, int DrawFlag, RECT *DrawArea )
{
	VERTEX_2D *DrawVert ;
	VERTEX_2D TempVect[ 4 ] ;
	VERTEX_3D *DrawVert3D ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	VERTEX_BLENDTEX_2D TempVert[ 4 ] ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex ;
	IMAGEDATA_ORIG *Orig ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert = NULL ;
	DWORD DiffuseColor ;
	int DrawTexNum ;
	int i ;
	int Flag ;
	int BlendGraphNoIncFlag ;
	float SizeX ;
	float SizeY ;
	float f ;
	VECTOR SrcVec[ 4 ] ;
	VECTOR SrcVec2[ 4 ] ;
	float Sin = 0.0f ;
	float Cos = 1.0f ;
	float ScaleX ;
	float ScaleY ;
	float dleft = 0.0f ;
	float dright = 0.0f ;
	float dtop = 0.0f ;
	float dbottom = 0.0f ;
	float z ;
	float rhw ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	Orig = Image->Orig ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画準備
	if( DrawFlag )
	{
		Flag = TransFlag | DX_LINUX_DRAWPREP_3D | DX_LINUX_DRAWPREP_FOG | DX_LINUX_DRAWPREP_TEXADDRESS ;
		DX_LINUX_DRAWPREP_TEX( Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )
	}
	else
	{
		dleft   = -100000000.0f ;
		dright  =  100000000.0f ;
		dtop    = -100000000.0f ;
		dbottom =  100000000.0f ;
	}

	// 頂点データを取得
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// 描画情報の数をセット
	DrawTexNum = Image->Hard.DrawNum ;

	// 描画情報の数がブレンド画像と異なっていたら０番目のテクスチャだけを使用する
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// 描画情報配列のアドレスをセットしておく
	DrawTex = Image->Hard.Draw ;
	BlendDrawTex = NULL ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// サイズと座標関係の事前計算
	SizeX = Size ;
	SizeY = Size * ( float )Image->HeightF / ( float )Image->WidthF ;

	ScaleX = SizeX / Image->WidthF ;
	ScaleY = SizeY / Image->HeightF ;
	cx *= Image->WidthF ;
	cy *= Image->HeightF ;

	// 回転する場合は回転値を求めておく
	if( Angle != 0.0 )
	{
		_SINCOS( (float)Angle, &Sin, &Cos ) ;
	}

	// テクスチャーの数だけ繰り返す
	for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
	{
		// 描画するテクスチャーのセット
		if( DrawFlag )
		{
			Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;

			if( BlendDrawTex != NULL )
			{
				Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture );
			}

			if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_Linux_DeviceState_NormalDrawSetup() ;
			}
		}

		TexVert = DrawTex->Vertex ;
		if( BlendDrawTex != NULL )
		{
			BlendTexVert = BlendDrawTex->Vertex ;
		}

		// ブレンドグラフィックを使用していなくて、且つ描画する場合は高速な処理を使用する
		if( GLINUX.Device.DrawInfo.VertexType != VERTEXTYPE_BLENDTEX && DrawFlag == TRUE )
		{
			GETVERTEX_BILLBOARD( DrawVert3D ) ;

			// 回転する場合としない場合で処理を分岐
			if( Angle != 0.0 )
			{
				// ローカル座標準備
				SrcVec[2].x = SrcVec[0].x = ( -cx + TexVert[0].x ) * ScaleX ;
				SrcVec[3].x = SrcVec[1].x = ( -cx + TexVert[1].x ) * ScaleX ;

				SrcVec[1].y = SrcVec[0].y = ( -cy + Image->HeightF - TexVert[0].y ) * ScaleY ;
				SrcVec[3].y = SrcVec[2].y = ( -cy + Image->HeightF - TexVert[2].y ) * ScaleY ;

				// 回転計算
				f             = SrcVec[ 0 ].x * Cos - SrcVec[ 0 ].y * Sin ;	
				SrcVec[ 0 ].y = SrcVec[ 0 ].x * Sin + SrcVec[ 0 ].y * Cos ;
				SrcVec[ 0 ].x = f ;

				f             = SrcVec[ 1 ].x * Cos - SrcVec[ 1 ].y * Sin ;	
				SrcVec[ 1 ].y = SrcVec[ 1 ].x * Sin + SrcVec[ 1 ].y * Cos ;
				SrcVec[ 1 ].x = f ;

				f             = SrcVec[ 2 ].x * Cos - SrcVec[ 2 ].y * Sin ;	
				SrcVec[ 2 ].y = SrcVec[ 2 ].x * Sin + SrcVec[ 2 ].y * Cos ;
				SrcVec[ 2 ].x = f ;

				f             = SrcVec[ 3 ].x * Cos - SrcVec[ 3 ].y * Sin ;	
				SrcVec[ 3 ].y = SrcVec[ 3 ].x * Sin + SrcVec[ 3 ].y * Cos ;
				SrcVec[ 3 ].x = f ;

				// ビルボード座標をワールド座標へ変換
				DrawVert3D[ 0 ].pos.x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 0 ].pos.y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 0 ].pos.z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				DrawVert3D[ 1 ].pos.x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 1 ].pos.y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 1 ].pos.z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				DrawVert3D[ 2 ].pos.x = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 2 ].pos.y = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 2 ].pos.z = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				DrawVert3D[ 3 ].pos.x = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 3 ].pos.y = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 3 ].pos.z = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				DrawVert3D[ 4 ].pos.x = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 4 ].pos.y = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 4 ].pos.z = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				DrawVert3D[ 5 ].pos.x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 5 ].pos.y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 5 ].pos.z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;
			}
			else
			{
				VECTOR TempVecX[ 2 ], TempVecY[ 2 ] ;

				// ローカル座標準備
				SrcVec[0].x = ( -cx + TexVert[0].x ) * ScaleX ;
				SrcVec[1].x = ( -cx + TexVert[1].x ) * ScaleX ;

				SrcVec[0].y = ( -cy + Image->HeightF - TexVert[0].y ) * ScaleY ;
				SrcVec[2].y = ( -cy + Image->HeightF - TexVert[2].y ) * ScaleY ;

				// ビルボード座標をワールド座標へ変換
				TempVecX[ 0 ].x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
				TempVecX[ 0 ].y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
				TempVecX[ 0 ].z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

				TempVecX[ 1 ].x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
				TempVecX[ 1 ].y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
				TempVecX[ 1 ].z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

				TempVecY[ 0 ].x = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
				TempVecY[ 0 ].y = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
				TempVecY[ 0 ].z = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

				TempVecY[ 1 ].x = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
				TempVecY[ 1 ].y = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
				TempVecY[ 1 ].z = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

				DrawVert3D[ 0 ].pos.x = TempVecX[ 0 ].x + TempVecY[ 0 ].x ;
				DrawVert3D[ 0 ].pos.y = TempVecX[ 0 ].y + TempVecY[ 0 ].y ;
				DrawVert3D[ 0 ].pos.z = TempVecX[ 0 ].z + TempVecY[ 0 ].z ;

				DrawVert3D[ 1 ].pos.x = TempVecX[ 1 ].x + TempVecY[ 0 ].x ;
				DrawVert3D[ 1 ].pos.y = TempVecX[ 1 ].y + TempVecY[ 0 ].y ;
				DrawVert3D[ 1 ].pos.z = TempVecX[ 1 ].z + TempVecY[ 0 ].z ;

				DrawVert3D[ 2 ].pos.x = TempVecX[ 0 ].x + TempVecY[ 1 ].x ;
				DrawVert3D[ 2 ].pos.y = TempVecX[ 0 ].y + TempVecY[ 1 ].y ;
				DrawVert3D[ 2 ].pos.z = TempVecX[ 0 ].z + TempVecY[ 1 ].z ;

				DrawVert3D[ 3 ].pos.x = TempVecX[ 1 ].x + TempVecY[ 1 ].x ;
				DrawVert3D[ 3 ].pos.y = TempVecX[ 1 ].y + TempVecY[ 1 ].y ;
				DrawVert3D[ 3 ].pos.z = TempVecX[ 1 ].z + TempVecY[ 1 ].z ;

				DrawVert3D[ 4 ].pos.x = TempVecX[ 0 ].x + TempVecY[ 1 ].x ;
				DrawVert3D[ 4 ].pos.y = TempVecX[ 0 ].y + TempVecY[ 1 ].y ;
				DrawVert3D[ 4 ].pos.z = TempVecX[ 0 ].z + TempVecY[ 1 ].z ;

				DrawVert3D[ 5 ].pos.x = TempVecX[ 1 ].x + TempVecY[ 0 ].x ;
				DrawVert3D[ 5 ].pos.y = TempVecX[ 1 ].y + TempVecY[ 0 ].y ;
				DrawVert3D[ 5 ].pos.z = TempVecX[ 1 ].z + TempVecY[ 0 ].z ;
			}

			*( ( DWORD * )&DrawVert3D[0].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[1].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[2].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[3].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[4].b ) = DiffuseColor ; 
			*( ( DWORD * )&DrawVert3D[5].b ) = DiffuseColor ;

			if( ReverseXFlag )
			{
				DrawVert3D[0].u = TexVert[1].u ;
				DrawVert3D[1].u = TexVert[0].u ;
				DrawVert3D[2].u = TexVert[1].u ;
				DrawVert3D[3].u = TexVert[0].u ;
				DrawVert3D[4].u = TexVert[1].u ;
				DrawVert3D[5].u = TexVert[0].u ;
			}
			else
			{
				DrawVert3D[0].u = TexVert[0].u ;
				DrawVert3D[1].u = TexVert[1].u ;
				DrawVert3D[2].u = TexVert[0].u ;
				DrawVert3D[3].u = TexVert[1].u ;
				DrawVert3D[4].u = TexVert[0].u ;
				DrawVert3D[5].u = TexVert[1].u ;
			}
			if( ReverseYFlag )
			{
				DrawVert3D[0].v = TexVert[2].v ;
				DrawVert3D[1].v = TexVert[2].v ;
				DrawVert3D[2].v = TexVert[0].v ;
				DrawVert3D[3].v = TexVert[0].v ;
				DrawVert3D[4].v = TexVert[0].v ;
				DrawVert3D[5].v = TexVert[2].v ;
			}
			else
			{
				DrawVert3D[0].v = TexVert[0].v ;
				DrawVert3D[1].v = TexVert[0].v ;
				DrawVert3D[2].v = TexVert[2].v ;
				DrawVert3D[3].v = TexVert[2].v ;
				DrawVert3D[4].v = TexVert[2].v ;
				DrawVert3D[5].v = TexVert[0].v ;
			}

			// テクスチャーを描画する
			ADD4VERTEX_BILLBOARD
		}
		else
		{
			VECTOR DrawPos[ 4 ] ;

			// ビルボードの４頂点を得る
			{
				// 回転する場合としない場合で処理を分岐
				if( Angle != 0.0 )
				{
					// ローカル座標準備
					SrcVec[2].x = SrcVec[0].x = ( -cx + TexVert[0].x ) * ScaleX ;
					SrcVec[3].x = SrcVec[1].x = ( -cx + TexVert[1].x ) * ScaleX ;

					SrcVec[1].y = SrcVec[0].y = ( -cy + Image->HeightF - TexVert[0].y ) * ScaleY ;
					SrcVec[3].y = SrcVec[2].y = ( -cy + Image->HeightF - TexVert[2].y ) * ScaleY ;

					// 回転計算
					f             = SrcVec[ 0 ].x * Cos - SrcVec[ 0 ].y * Sin ;	
					SrcVec[ 0 ].y = SrcVec[ 0 ].x * Sin + SrcVec[ 0 ].y * Cos ;
					SrcVec[ 0 ].x = f ;

					f             = SrcVec[ 1 ].x * Cos - SrcVec[ 1 ].y * Sin ;	
					SrcVec[ 1 ].y = SrcVec[ 1 ].x * Sin + SrcVec[ 1 ].y * Cos ;
					SrcVec[ 1 ].x = f ;

					f             = SrcVec[ 2 ].x * Cos - SrcVec[ 2 ].y * Sin ;	
					SrcVec[ 2 ].y = SrcVec[ 2 ].x * Sin + SrcVec[ 2 ].y * Cos ;
					SrcVec[ 2 ].x = f ;

					f             = SrcVec[ 3 ].x * Cos - SrcVec[ 3 ].y * Sin ;	
					SrcVec[ 3 ].y = SrcVec[ 3 ].x * Sin + SrcVec[ 3 ].y * Cos ;
					SrcVec[ 3 ].x = f ;

					// ビルボード座標をワールド座標へ変換
					SrcVec2[ 0 ].x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
					SrcVec2[ 0 ].y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
					SrcVec2[ 0 ].z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

					SrcVec2[ 1 ].x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
					SrcVec2[ 1 ].y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
					SrcVec2[ 1 ].z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

					SrcVec2[ 2 ].x = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
					SrcVec2[ 2 ].y = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
					SrcVec2[ 2 ].z = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

					SrcVec2[ 3 ].x = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
					SrcVec2[ 3 ].y = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
					SrcVec2[ 3 ].z = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;
				}
				else
				{
					VECTOR TempVecX[ 2 ], TempVecY[ 2 ] ;

					// ローカル座標準備
					SrcVec[0].x = ( -cx + TexVert[0].x ) * ScaleX ;
					SrcVec[1].x = ( -cx + TexVert[1].x ) * ScaleX ;

					SrcVec[0].y = ( -cy + Image->HeightF - TexVert[0].y ) * ScaleY ;
					SrcVec[2].y = ( -cy + Image->HeightF - TexVert[2].y ) * ScaleY ;

					// ビルボード座標をワールド座標へ変換
					TempVecX[ 0 ].x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
					TempVecX[ 0 ].y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
					TempVecX[ 0 ].z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

					TempVecX[ 1 ].x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
					TempVecX[ 1 ].y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
					TempVecX[ 1 ].z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

					TempVecY[ 0 ].x = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
					TempVecY[ 0 ].y = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
					TempVecY[ 0 ].z = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

					TempVecY[ 1 ].x = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
					TempVecY[ 1 ].y = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
					TempVecY[ 1 ].z = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

					SrcVec2[ 0 ].x = TempVecX[ 0 ].x + TempVecY[ 0 ].x ;
					SrcVec2[ 0 ].y = TempVecX[ 0 ].y + TempVecY[ 0 ].y ;
					SrcVec2[ 0 ].z = TempVecX[ 0 ].z + TempVecY[ 0 ].z ;

					SrcVec2[ 1 ].x = TempVecX[ 1 ].x + TempVecY[ 0 ].x ;
					SrcVec2[ 1 ].y = TempVecX[ 1 ].y + TempVecY[ 0 ].y ;
					SrcVec2[ 1 ].z = TempVecX[ 1 ].z + TempVecY[ 0 ].z ;

					SrcVec2[ 2 ].x = TempVecX[ 0 ].x + TempVecY[ 1 ].x ;
					SrcVec2[ 2 ].y = TempVecX[ 0 ].y + TempVecY[ 1 ].y ;
					SrcVec2[ 2 ].z = TempVecX[ 0 ].z + TempVecY[ 1 ].z ;

					SrcVec2[ 3 ].x = TempVecX[ 1 ].x + TempVecY[ 1 ].x ;
					SrcVec2[ 3 ].y = TempVecX[ 1 ].y + TempVecY[ 1 ].y ;
					SrcVec2[ 3 ].z = TempVecX[ 1 ].z + TempVecY[ 1 ].z ;
				}
			}

			// 頂点タイプによって処理を分岐
			switch( GLINUX.Device.DrawInfo.VertexType )
			{
			case VERTEXTYPE_BLENDTEX :
				// ブレンドテクスチャを使用する

				// 頂点バッファの取得
				if( DrawFlag )
				{
					GETVERTEX_QUAD( DrawVertB )
				}
				else
				{
					DrawVertB = TempVert ;
				}

				DrawVertB[0].color = DiffuseColor ;
				DrawVertB[1].color = DiffuseColor ;
				DrawVertB[2].color = DiffuseColor ;
				DrawVertB[3].color = DiffuseColor ;
				DrawVertB[4].color = DiffuseColor ;
				DrawVertB[5].color = DiffuseColor ;

				// スクリーン座標に変換
				rhw = 1.0f / ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3] ) ;
				z   = rhw  * ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2] ) ;

				DrawPos[ 0 ].x = ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 0 ].y = ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 1 ].x = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 1 ].y = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 2 ].x = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 2 ].y = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 3 ].x = ( SrcVec2[3].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[3].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[3].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 3 ].y = ( SrcVec2[3].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[3].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[3].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;

				DrawVertB[0].pos.x = DrawPos[ 0 ].x ;
				DrawVertB[0].pos.y = DrawPos[ 0 ].y ;
				DrawVertB[1].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[1].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[2].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[2].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[3].pos.x = DrawPos[ 3 ].x ;
				DrawVertB[3].pos.y = DrawPos[ 3 ].y ;
				DrawVertB[4].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[4].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[5].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[5].pos.y = DrawPos[ 1 ].y ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[0].u1 = u[ 0 ] ;
					DrawVertB[0].v1 = v[ 0 ] ;
					DrawVertB[1].u1 = u[ 1 ] ;
					DrawVertB[1].v1 = v[ 0 ] ;
					DrawVertB[2].u1 = u[ 0 ] ;
					DrawVertB[2].v1 = v[ 1 ] ;
					DrawVertB[3].u1 = u[ 1 ] ;
					DrawVertB[3].v1 = v[ 1 ] ;
					DrawVertB[4].u1 = u[ 0 ] ;
					DrawVertB[4].v1 = v[ 1 ] ;
					DrawVertB[5].u1 = u[ 1 ] ;
					DrawVertB[5].v1 = v[ 0 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[0].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				if( ReverseXFlag )
				{
					DrawVertB[0].u2 = TexVert[1].u ;
					DrawVertB[1].u2 = TexVert[0].u ;
					DrawVertB[2].u2 = TexVert[1].u ;
					DrawVertB[3].u2 = TexVert[0].u ;
					DrawVertB[4].u2 = TexVert[1].u ;
					DrawVertB[5].u2 = TexVert[0].u ;
				}
				else
				{
					DrawVertB[0].u2 = TexVert[0].u ;
					DrawVertB[1].u2 = TexVert[1].u ;
					DrawVertB[2].u2 = TexVert[0].u ;
					DrawVertB[3].u2 = TexVert[1].u ;
					DrawVertB[4].u2 = TexVert[0].u ;
					DrawVertB[5].u2 = TexVert[1].u ;
				}
				if( ReverseYFlag )
				{
					DrawVertB[0].v2 = TexVert[2].v ;
					DrawVertB[1].v2 = TexVert[2].v ;
					DrawVertB[2].v2 = TexVert[0].v ;
					DrawVertB[3].v2 = TexVert[0].v ;
					DrawVertB[4].v2 = TexVert[0].v ;
					DrawVertB[5].v2 = TexVert[2].v ;
				}
				else
				{
					DrawVertB[0].v2 = TexVert[0].v ;
					DrawVertB[1].v2 = TexVert[0].v ;
					DrawVertB[2].v2 = TexVert[2].v ;
					DrawVertB[3].v2 = TexVert[2].v ;
					DrawVertB[4].v2 = TexVert[2].v ;
					DrawVertB[5].v2 = TexVert[0].v ;
				}

				DrawVertB[0].pos.z = z ;
				DrawVertB[1].pos.z = z ;
				DrawVertB[2].pos.z = z ;
				DrawVertB[3].pos.z = z ;
				DrawVertB[4].pos.z = z ;
				DrawVertB[5].pos.z = z ;

				DrawVertB[0].rhw = rhw ;
				DrawVertB[1].rhw = rhw ;
				DrawVertB[2].rhw = rhw ;
				DrawVertB[3].rhw = rhw ;
				DrawVertB[4].rhw = rhw ;
				DrawVertB[5].rhw = rhw ;

				// テクスチャーを描画する
				if( DrawFlag )
				{
					ADD4VERTEX_BLENDTEX
				}
				else
				{
					if( dright  > DrawVertB[ 0 ].pos.x ) dright  = DrawVertB[ 0 ].pos.x ;
					if( dright  > DrawVertB[ 1 ].pos.x ) dright  = DrawVertB[ 1 ].pos.x ;
					if( dright  > DrawVertB[ 2 ].pos.x ) dright  = DrawVertB[ 2 ].pos.x ;
					if( dright  > DrawVertB[ 3 ].pos.x ) dright  = DrawVertB[ 3 ].pos.x ;

					if( dleft   < DrawVertB[ 0 ].pos.x ) dleft   = DrawVertB[ 0 ].pos.x ;
					if( dleft   < DrawVertB[ 1 ].pos.x ) dleft   = DrawVertB[ 1 ].pos.x ;
					if( dleft   < DrawVertB[ 2 ].pos.x ) dleft   = DrawVertB[ 2 ].pos.x ;
					if( dleft   < DrawVertB[ 3 ].pos.x ) dleft   = DrawVertB[ 3 ].pos.x ;

					if( dbottom > DrawVertB[ 0 ].pos.y ) dbottom = DrawVertB[ 0 ].pos.y ;
					if( dbottom > DrawVertB[ 1 ].pos.y ) dbottom = DrawVertB[ 1 ].pos.y ;
					if( dbottom > DrawVertB[ 2 ].pos.y ) dbottom = DrawVertB[ 2 ].pos.y ;
					if( dbottom > DrawVertB[ 3 ].pos.y ) dbottom = DrawVertB[ 3 ].pos.y ;

					if( dtop    < DrawVertB[ 0 ].pos.y ) dtop    = DrawVertB[ 0 ].pos.y ;
					if( dtop    < DrawVertB[ 1 ].pos.y ) dtop    = DrawVertB[ 1 ].pos.y ;
					if( dtop    < DrawVertB[ 2 ].pos.y ) dtop    = DrawVertB[ 2 ].pos.y ;
					if( dtop    < DrawVertB[ 3 ].pos.y ) dtop    = DrawVertB[ 3 ].pos.y ;
				}

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
				break ;

			case VERTEXTYPE_TEX :
				// ブレンドテクスチャを使用しない

				// 頂点バッファの取得
				if( DrawFlag )
				{
					GETVERTEX_QUAD( DrawVert )
				}
				else
				{
					DrawVert = TempVect;
				}

				// スクリーン座標に変換
				rhw = 1.0f / ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3] ) ;
				z   = rhw  * ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2] ) ;

				DrawVert[0].pos.x = ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[0].pos.y = ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[1].pos.x = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[1].pos.y = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[2].pos.x = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[2].pos.y = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[3].pos.x = ( SrcVec2[3].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[3].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[3].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[3].pos.y = ( SrcVec2[3].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[3].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[3].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[4].pos.x = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[4].pos.y = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[5].pos.x = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[5].pos.y = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;

				DrawVert[0].pos.z = z ;
				DrawVert[1].pos.z = z ;
				DrawVert[2].pos.z = z ;
				DrawVert[3].pos.z = z ;
				DrawVert[4].pos.z = z ;
				DrawVert[5].pos.z = z ;

				DrawVert[0].rhw = rhw ;
				DrawVert[1].rhw = rhw ;
				DrawVert[2].rhw = rhw ;
				DrawVert[3].rhw = rhw ;
				DrawVert[4].rhw = rhw ;
				DrawVert[5].rhw = rhw ;

				DrawVert[0].color = DiffuseColor ;
				DrawVert[1].color = DiffuseColor ;
				DrawVert[2].color = DiffuseColor ;
				DrawVert[3].color = DiffuseColor ;
				DrawVert[4].color = DiffuseColor ;
				DrawVert[5].color = DiffuseColor ;

				if( ReverseXFlag )
				{
					DrawVert[0].u = TexVert[1].u ;
					DrawVert[1].u = TexVert[0].u ;
					DrawVert[2].u = TexVert[1].u ;
					DrawVert[3].u = TexVert[0].u ;
					DrawVert[4].u = TexVert[1].u ;
					DrawVert[5].u = TexVert[0].u ;
				}
				else
				{
					DrawVert[0].u = TexVert[0].u ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[5].u = TexVert[1].u ;
				}
				if( ReverseYFlag )
				{
					DrawVert[0].v = TexVert[2].v ;
					DrawVert[1].v = TexVert[2].v ;
					DrawVert[2].v = TexVert[0].v ;
					DrawVert[3].v = TexVert[0].v ;
					DrawVert[4].v = TexVert[0].v ;
					DrawVert[5].v = TexVert[2].v ;
				}
				else
				{
					DrawVert[0].v = TexVert[0].v ;
					DrawVert[1].v = TexVert[0].v ;
					DrawVert[2].v = TexVert[2].v ;
					DrawVert[3].v = TexVert[2].v ;
					DrawVert[4].v = TexVert[2].v ;
					DrawVert[5].v = TexVert[0].v ;
				}

				// テクスチャーを描画する
				if( DrawFlag )
				{
					ADD4VERTEX_TEX
				}
				else
				{
					if( dright  > DrawVert[ 0 ].pos.x ) dright  = DrawVert[ 0 ].pos.x ;
					if( dright  > DrawVert[ 1 ].pos.x ) dright  = DrawVert[ 1 ].pos.x ;
					if( dright  > DrawVert[ 2 ].pos.x ) dright  = DrawVert[ 2 ].pos.x ;
					if( dright  > DrawVert[ 3 ].pos.x ) dright  = DrawVert[ 3 ].pos.x ;

					if( dleft   < DrawVert[ 0 ].pos.x ) dleft   = DrawVert[ 0 ].pos.x ;
					if( dleft   < DrawVert[ 1 ].pos.x ) dleft   = DrawVert[ 1 ].pos.x ;
					if( dleft   < DrawVert[ 2 ].pos.x ) dleft   = DrawVert[ 2 ].pos.x ;
					if( dleft   < DrawVert[ 3 ].pos.x ) dleft   = DrawVert[ 3 ].pos.x ;

					if( dbottom > DrawVert[ 0 ].pos.y ) dbottom = DrawVert[ 0 ].pos.y ;
					if( dbottom > DrawVert[ 1 ].pos.y ) dbottom = DrawVert[ 1 ].pos.y ;
					if( dbottom > DrawVert[ 2 ].pos.y ) dbottom = DrawVert[ 2 ].pos.y ;
					if( dbottom > DrawVert[ 3 ].pos.y ) dbottom = DrawVert[ 3 ].pos.y ;

					if( dtop    < DrawVert[ 0 ].pos.y ) dtop    = DrawVert[ 0 ].pos.y ;
					if( dtop    < DrawVert[ 1 ].pos.y ) dtop    = DrawVert[ 1 ].pos.y ;
					if( dtop    < DrawVert[ 2 ].pos.y ) dtop    = DrawVert[ 2 ].pos.y ;
					if( dtop    < DrawVert[ 3 ].pos.y ) dtop    = DrawVert[ 3 ].pos.y ;
				}
				break ;
			}
		}
	}

	if( DrawFlag == FALSE )
	{
		DrawArea->left   = _FTOL( dleft   ) - 1 ;
		DrawArea->right  = _FTOL( dright  ) + 1 ;
		DrawArea->top    = _FTOL( dtop    ) - 1 ;
		DrawArea->bottom = _FTOL( dbottom ) + 1 ;
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawModiBillboard3D
extern	int		Graphics_Hardware_DrawModiBillboard3D_PF( VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int DrawFlag, RECT *DrawArea )
{
	VERTEX_2D *DrawVert ;
	VERTEX_3D *DrawVert3D ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex ;
	IMAGEDATA_ORIG *Orig ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert = NULL ;
	DWORD DiffuseColor ;
	int DrawTexNum ;
	int i, Flag ;
	int BlendGraphNoIncFlag ;
	VECTOR SrcVec2[ 4 ] ;
	float dleft = 0.0f, dright = 0.0f, dtop = 0.0f, dbottom = 0.0f;
	float z ;
	float rhw ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	Orig = Image->Orig ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画準備
	if( DrawFlag )
	{
		Flag = TransFlag | DX_LINUX_DRAWPREP_3D | DX_LINUX_DRAWPREP_FOG | DX_LINUX_DRAWPREP_TEXADDRESS ;
		DX_LINUX_DRAWPREP_TEX( Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )
	}
	else
	{
		dleft   = -100000000.0f ;
		dright  =  100000000.0f ;
		dtop    = -100000000.0f ;
		dbottom =  100000000.0f ;
	}

	// 頂点データを取得
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// 描画情報の数をセット
	DrawTexNum = Image->Hard.DrawNum ;

	// 描画情報の数がブレンド画像と異なっていたら０番目のテクスチャだけを使用する
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// 描画情報配列のアドレスをセットしておく
	DrawTex = Image->Hard.Draw ;
	BlendDrawTex = NULL ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// テクスチャーの数だけ繰り返す
	for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
	{
		// 描画するテクスチャーのセット
		if( DrawFlag )
		{
			Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;

			if( BlendDrawTex != NULL )
			{
				Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture );
			}

			if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_Linux_DeviceState_NormalDrawSetup() ;
			}
		}

		TexVert = DrawTex->Vertex ;
		if( BlendDrawTex != NULL )
		{
			BlendTexVert = BlendDrawTex->Vertex ;
		}

		// ブレンドグラフィックを使用していなくて、且つ描画する場合は高速な処理を使用する
		if( GLINUX.Device.DrawInfo.VertexType != VERTEXTYPE_BLENDTEX && DrawFlag == TRUE )
		{
			GETVERTEX_BILLBOARD( DrawVert3D ) ;

			// ビルボード座標をワールド座標へ変換
			DrawVert3D[ 0 ].pos.x = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 0 ].pos.y = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 0 ].pos.z = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[ 1 ].pos.x = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 1 ].pos.y = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 1 ].pos.z = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[ 2 ].pos.x = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 2 ].pos.y = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 2 ].pos.z = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[ 3 ].pos.x = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 3 ].pos.y = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 3 ].pos.z = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[ 4 ].pos.x = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 4 ].pos.y = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 4 ].pos.z = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[ 5 ].pos.x = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 5 ].pos.y = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 5 ].pos.z = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			*( ( DWORD * )&DrawVert3D[0].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[1].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[2].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[3].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[4].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[5].b ) = DiffuseColor ;

			DrawVert3D[0].u = TexVert[0].u ;
			DrawVert3D[0].v = TexVert[0].v ;
			DrawVert3D[1].u = TexVert[1].u ;
			DrawVert3D[1].v = TexVert[0].v ;
			DrawVert3D[2].u = TexVert[0].u ;
			DrawVert3D[2].v = TexVert[2].v ;
			DrawVert3D[3].u = TexVert[1].u ;
			DrawVert3D[3].v = TexVert[2].v ;
			DrawVert3D[4].u = TexVert[0].u ;
			DrawVert3D[4].v = TexVert[2].v ;
			DrawVert3D[5].u = TexVert[1].u ;
			DrawVert3D[5].v = TexVert[0].v ;

			// テクスチャーを描画する
			ADD4VERTEX_BILLBOARD
		}
		else
		{
			VECTOR DrawPos[ 4 ] ;

			// ビルボードの４頂点を得る
			{
				// ビルボード座標をワールド座標へ変換
				SrcVec2[ 0 ].x = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				SrcVec2[ 0 ].y = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				SrcVec2[ 0 ].z = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				SrcVec2[ 1 ].x = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				SrcVec2[ 1 ].y = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				SrcVec2[ 1 ].z = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				SrcVec2[ 2 ].x = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				SrcVec2[ 2 ].y = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				SrcVec2[ 2 ].z = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				SrcVec2[ 3 ].x = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				SrcVec2[ 3 ].y = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				SrcVec2[ 3 ].z = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;
			}

			// 頂点タイプによって処理を分岐
			switch( GLINUX.Device.DrawInfo.VertexType )
			{
			case VERTEXTYPE_BLENDTEX :
				// ブレンドテクスチャを使用する

				// 頂点バッファの取得
				GETVERTEX_QUAD( DrawVertB )

				DrawVertB[0].color = DiffuseColor ;
				DrawVertB[1].color = DiffuseColor ;
				DrawVertB[2].color = DiffuseColor ;
				DrawVertB[3].color = DiffuseColor ;
				DrawVertB[4].color = DiffuseColor ;
				DrawVertB[5].color = DiffuseColor ;

				// スクリーン座標に変換
				rhw = 1.0f / ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3] ) ;
				z   = rhw  * ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2] ) ;

				DrawPos[ 0 ].x = ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 0 ].y = ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 1 ].x = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 1 ].y = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 2 ].x = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 2 ].y = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 3 ].x = ( SrcVec2[3].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[3].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[3].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 3 ].y = ( SrcVec2[3].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[3].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[3].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;

				DrawVertB[0].pos.x = DrawPos[ 0 ].x ;
				DrawVertB[0].pos.y = DrawPos[ 0 ].y ;
				DrawVertB[1].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[1].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[2].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[2].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[3].pos.x = DrawPos[ 3 ].x ;
				DrawVertB[3].pos.y = DrawPos[ 3 ].y ;
				DrawVertB[4].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[4].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[5].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[5].pos.y = DrawPos[ 1 ].y ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[0].u1 = u[ 0 ] ;
					DrawVertB[0].v1 = v[ 0 ] ;
					DrawVertB[1].u1 = u[ 1 ] ;
					DrawVertB[1].v1 = v[ 0 ] ;
					DrawVertB[2].u1 = u[ 0 ] ;
					DrawVertB[2].v1 = v[ 1 ] ;
					DrawVertB[3].u1 = u[ 1 ] ;
					DrawVertB[3].v1 = v[ 1 ] ;
					DrawVertB[4].u1 = u[ 0 ] ;
					DrawVertB[4].v1 = v[ 1 ] ;
					DrawVertB[5].u1 = u[ 1 ] ;
					DrawVertB[5].v1 = v[ 0 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[0].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				DrawVertB[0].pos.z = z ;
				DrawVertB[1].pos.z = z ;
				DrawVertB[2].pos.z = z ;
				DrawVertB[3].pos.z = z ;
				DrawVertB[4].pos.z = z ;
				DrawVertB[5].pos.z = z ;

				DrawVertB[0].rhw = rhw ;
				DrawVertB[1].rhw = rhw ;
				DrawVertB[2].rhw = rhw ;
				DrawVertB[3].rhw = rhw ;
				DrawVertB[4].rhw = rhw ;
				DrawVertB[5].rhw = rhw ;

				// テクスチャーを描画する
				if( DrawFlag )
				{
					ADD4VERTEX_BLENDTEX
				}
				else
				{
					if( dright  > DrawVertB[ 0 ].pos.x ) dright  = DrawVertB[ 0 ].pos.x ;
					if( dright  > DrawVertB[ 1 ].pos.x ) dright  = DrawVertB[ 1 ].pos.x ;
					if( dright  > DrawVertB[ 2 ].pos.x ) dright  = DrawVertB[ 2 ].pos.x ;
					if( dright  > DrawVertB[ 3 ].pos.x ) dright  = DrawVertB[ 3 ].pos.x ;

					if( dleft   < DrawVertB[ 0 ].pos.x ) dleft   = DrawVertB[ 0 ].pos.x ;
					if( dleft   < DrawVertB[ 1 ].pos.x ) dleft   = DrawVertB[ 1 ].pos.x ;
					if( dleft   < DrawVertB[ 2 ].pos.x ) dleft   = DrawVertB[ 2 ].pos.x ;
					if( dleft   < DrawVertB[ 3 ].pos.x ) dleft   = DrawVertB[ 3 ].pos.x ;

					if( dbottom > DrawVertB[ 0 ].pos.y ) dbottom = DrawVertB[ 0 ].pos.y ;
					if( dbottom > DrawVertB[ 1 ].pos.y ) dbottom = DrawVertB[ 1 ].pos.y ;
					if( dbottom > DrawVertB[ 2 ].pos.y ) dbottom = DrawVertB[ 2 ].pos.y ;
					if( dbottom > DrawVertB[ 3 ].pos.y ) dbottom = DrawVertB[ 3 ].pos.y ;

					if( dtop    < DrawVertB[ 0 ].pos.y ) dtop    = DrawVertB[ 0 ].pos.y ;
					if( dtop    < DrawVertB[ 1 ].pos.y ) dtop    = DrawVertB[ 1 ].pos.y ;
					if( dtop    < DrawVertB[ 2 ].pos.y ) dtop    = DrawVertB[ 2 ].pos.y ;
					if( dtop    < DrawVertB[ 3 ].pos.y ) dtop    = DrawVertB[ 3 ].pos.y ;
				}

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
				break ;

			case VERTEXTYPE_TEX :
				// ブレンドテクスチャを使用しない

				// 頂点バッファの取得
				GETVERTEX_QUAD( DrawVert )

				// スクリーン座標に変換
				rhw = 1.0f / ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3] ) ;
				z   = rhw  * ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2] ) ;

				DrawVert[0].pos.x = ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[0].pos.y = ( SrcVec2[0].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[0].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[0].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[1].pos.x = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[1].pos.y = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[2].pos.x = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[2].pos.y = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[3].pos.x = ( SrcVec2[3].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[3].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[3].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[3].pos.y = ( SrcVec2[3].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[3].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[3].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[4].pos.x = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[4].pos.y = ( SrcVec2[2].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;
				DrawVert[5].pos.x = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0] ) * rhw ;
				DrawVert[5].pos.y = ( SrcVec2[1].x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1] ) * rhw ;

				DrawVert[0].pos.z = z ;
				DrawVert[1].pos.z = z ;
				DrawVert[2].pos.z = z ;
				DrawVert[3].pos.z = z ;
				DrawVert[4].pos.z = z ;
				DrawVert[5].pos.z = z ;

				DrawVert[0].rhw = rhw ;
				DrawVert[1].rhw = rhw ;
				DrawVert[2].rhw = rhw ;
				DrawVert[3].rhw = rhw ;
				DrawVert[4].rhw = rhw ;
				DrawVert[5].rhw = rhw ;

				DrawVert[0].color = DiffuseColor ;
				DrawVert[1].color = DiffuseColor ;
				DrawVert[2].color = DiffuseColor ;
				DrawVert[3].color = DiffuseColor ;
				DrawVert[4].color = DiffuseColor ;
				DrawVert[5].color = DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[5].u = TexVert[1].u ;

				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].v = TexVert[0].v ;

				// テクスチャーを描画する
				if( DrawFlag )
				{
					ADD4VERTEX_TEX
				}
				else
				{
					if( dright  > DrawVert[ 0 ].pos.x ) dright  = DrawVert[ 0 ].pos.x ;
					if( dright  > DrawVert[ 1 ].pos.x ) dright  = DrawVert[ 1 ].pos.x ;
					if( dright  > DrawVert[ 2 ].pos.x ) dright  = DrawVert[ 2 ].pos.x ;
					if( dright  > DrawVert[ 3 ].pos.x ) dright  = DrawVert[ 3 ].pos.x ;

					if( dleft   < DrawVert[ 0 ].pos.x ) dleft   = DrawVert[ 0 ].pos.x ;
					if( dleft   < DrawVert[ 1 ].pos.x ) dleft   = DrawVert[ 1 ].pos.x ;
					if( dleft   < DrawVert[ 2 ].pos.x ) dleft   = DrawVert[ 2 ].pos.x ;
					if( dleft   < DrawVert[ 3 ].pos.x ) dleft   = DrawVert[ 3 ].pos.x ;

					if( dbottom > DrawVert[ 0 ].pos.y ) dbottom = DrawVert[ 0 ].pos.y ;
					if( dbottom > DrawVert[ 1 ].pos.y ) dbottom = DrawVert[ 1 ].pos.y ;
					if( dbottom > DrawVert[ 2 ].pos.y ) dbottom = DrawVert[ 2 ].pos.y ;
					if( dbottom > DrawVert[ 3 ].pos.y ) dbottom = DrawVert[ 3 ].pos.y ;

					if( dtop    < DrawVert[ 0 ].pos.y ) dtop    = DrawVert[ 0 ].pos.y ;
					if( dtop    < DrawVert[ 1 ].pos.y ) dtop    = DrawVert[ 1 ].pos.y ;
					if( dtop    < DrawVert[ 2 ].pos.y ) dtop    = DrawVert[ 2 ].pos.y ;
					if( dtop    < DrawVert[ 3 ].pos.y ) dtop    = DrawVert[ 3 ].pos.y ;
				}
				break ;
			}
		}
	}

	if( DrawFlag == FALSE )
	{
		DrawArea->left   = _FTOL( dleft   ) - 1 ;
		DrawArea->right  = _FTOL( dright  ) + 1 ;
		DrawArea->top    = _FTOL( dtop    ) - 1 ;
		DrawArea->bottom = _FTOL( dbottom ) + 1 ;
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawGraph
extern	int		Graphics_Hardware_DrawGraph_PF( int x, int y, float xf, float yf, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag )
{
	VERTEX_2D			*DrawVert ;
	VERTEX_BLENDTEX_2D	*DrawVertB ;
	IMAGEDATA_HARD_DRAW	*DrawTex ;
	IMAGEDATA_HARD_DRAW	*BlendDrawTex = NULL ;
	IMAGEDATA_ORIG		*Orig ;
	IMAGEDATA_HARD_VERT	*TexVert ;
	IMAGEDATA_HARD_VERT	*BlendTexVert ;
	DWORD				DiffuseColor ;
	int					DrawTexNum ;
	int					i ;
	int					j ;
	int					Flag ;
	int					BlendGraphNoIncFlag ;
	float				left ;
	float				right ;
	float				top ;
	float				bottom ;
	float				Vx ;
	float				Vy ;
	float				VBx ;
	float				VBy ;
	DWORD				drawz ;
	float				u[ 2 ] ;
	float				v[ 2 ] ;

	Orig = Image->Orig ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// float座標補正
//	xf -= 0.5F ;
//	yf -= 0.5F ;

	// 描画準備
	Flag = TransFlag | ( IntFlag << 1 ) ;
	DX_LINUX_DRAWPREP_TEX( Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )

	// 頂点データを取得
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// 描画情報の数をセット
	DrawTexNum = Image->Hard.DrawNum ;

	// 描画情報の数がブレンド画像と異なっていたら０番目のテクスチャだけを使用する
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// 描画情報配列のアドレスをセットしておく
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *( ( DWORD * )&GSYS.DrawSetting.DrawZ ) ;

	// 描画範囲からはみ出ているかどうかで処理を分岐
	if( GSYS.DrawSetting.Valid2DMatrix || IN_DRAWAREA_CHECK( x, y, x + Image->WidthI, y + Image->HeightI ) )
//	if( x + Image->Width  < GSYS.DrawSetting.DrawArea.right  && x > GSYS.DrawSetting.DrawArea.left &&
//		y + Image->Height < GSYS.DrawSetting.DrawArea.bottom && y > GSYS.DrawSetting.DrawArea.right )
	{
		// はみ出ていない場合

		// 頂点タイプによって処理を分岐
		if( GLINUX.Device.DrawInfo.VertexType == VERTEXTYPE_BLENDTEX )
		{
			// ブレンドテクスチャを使用する

			// テクスチャーの数だけ繰り返す
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// 描画するテクスチャーのセット
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

				if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}

				// 頂点バッファの取得
				GETVERTEX_QUAD( DrawVertB )
				
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

				DrawVertB[0].pos.x = xf + TexVert[0].x ;
				DrawVertB[0].pos.y = yf + TexVert[0].y ;
				DrawVertB[1].pos.x = xf + TexVert[1].x ;
				DrawVertB[1].pos.y = yf + TexVert[0].y ;
				DrawVertB[2].pos.x = xf + TexVert[0].x ;
				DrawVertB[2].pos.y = yf + TexVert[2].y ;
				DrawVertB[3].pos.x = xf + TexVert[1].x ;
				DrawVertB[3].pos.y = yf + TexVert[2].y ;
				DrawVertB[4].pos.x = xf + TexVert[0].x ;
				DrawVertB[4].pos.y = yf + TexVert[2].y ;
				DrawVertB[5].pos.x = xf + TexVert[1].x ;
				DrawVertB[5].pos.y = yf + TexVert[0].y ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ;
					u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ;
					v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ;
					v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u[ 0 ] = ( xf + TexVert[0].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ;
					u[ 1 ] = ( xf + TexVert[1].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ;
					v[ 0 ] = ( yf + TexVert[0].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ;
					v[ 1 ] = ( yf + TexVert[2].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ;
					break ;
				}

				u[ 0 ] *= GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] *= GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] *= GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] *= GLINUX.Device.DrawSetting.InvBlendTextureHeight ;

				DrawVertB[0].u1 = u[ 0 ] ;
				DrawVertB[0].v1 = v[ 0 ] ;
				DrawVertB[1].u1 = u[ 1 ] ;
				DrawVertB[1].v1 = v[ 0 ] ;
				DrawVertB[2].u1 = u[ 0 ] ;
				DrawVertB[2].v1 = v[ 1 ] ;
				DrawVertB[3].u1 = u[ 1 ] ;
				DrawVertB[3].v1 = v[ 1 ] ;
				DrawVertB[4].u1 = u[ 0 ] ;
				DrawVertB[4].v1 = v[ 1 ] ;
				DrawVertB[5].u1 = u[ 1 ] ;
				DrawVertB[5].v1 = v[ 0 ] ;

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
		}
		else
		{
			// ブレンドテクスチャを使用しない

			// テクスチャーの数だけ繰り返す
			i = 0 ;
			for(;;)
			{
				// 頂点バッファの取得
				GETVERTEX_QUAD( DrawVert )

				// 頂点データ準備
				TexVert = DrawTex->Vertex ;
				DrawVert[0].color	= DiffuseColor ;
				DrawVert[1].color	= DiffuseColor ;
				DrawVert[2].color	= DiffuseColor ;
				DrawVert[3].color	= DiffuseColor ;
				DrawVert[4].color	= DiffuseColor ;
				DrawVert[5].color	= DiffuseColor ;

				DrawVert[0].pos.x = TexVert[0].x + xf ;
				DrawVert[0].pos.y = TexVert[0].y + yf ;
				DrawVert[1].pos.x = TexVert[1].x + xf ;
				DrawVert[1].pos.y = TexVert[0].y + yf ;
				DrawVert[2].pos.x = TexVert[0].x + xf ;
				DrawVert[2].pos.y = TexVert[2].y + yf ;
				DrawVert[3].pos.x = TexVert[1].x + xf ;
				DrawVert[3].pos.y = TexVert[2].y + yf ;
				DrawVert[4].pos.x = TexVert[0].x + xf ;
				DrawVert[4].pos.y = TexVert[2].y + yf ;
				DrawVert[5].pos.x = TexVert[1].x + xf ;
				DrawVert[5].pos.y = TexVert[0].y + yf ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// 描画するテクスチャーのセット
				DrawTex ++ ;
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DeviceState_NormalDrawSetup() ;
			}
		}
	}
	else
	{
		// はみ出ている場合

		// 完全にはみ出ていたら何もせず終了
		if( OUT_DRAWAREA_CHECK( x, y, x + Image->WidthI, y + Image->HeightI ) ) return 0 ;
//		if( x > GSYS.DrawSetting.DrawArea.right || x + Image->Width < GSYS.DrawSetting.DrawArea.left ||
//			y > GSYS.DrawSetting.DrawArea.bottom || y + Image->Height < GSYS.DrawSetting.DrawArea.top ) return 0 ;

		// 描画矩形の保存
		left	= GSYS.DrawSetting.DrawAreaF.left   ;
		top		= GSYS.DrawSetting.DrawAreaF.top    ;
		right	= GSYS.DrawSetting.DrawAreaF.right  ;
		bottom	= GSYS.DrawSetting.DrawAreaF.bottom ;

		// 頂点タイプによって処理を分岐
		switch( GLINUX.Device.DrawInfo.VertexType )
		{
		case VERTEXTYPE_BLENDTEX :
			// ブレンドテクスチャを使用する

			// テクスチャーの数だけ繰り返す
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				VECTOR DrawPos[ 4 ] ;
				float u1[ 4 ] ;
				float v1[ 4 ] ;
				float u2[ 4 ] ;
				float v2[ 4 ] ;

				// 描画するテクスチャーのセット
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

				if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}

				// 頂点バッファの取得
				GETVERTEX_QUAD( DrawVertB )
				
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				Vx  = 1.0F / ( float )DrawTex->Tex->TexWidth  ;
				Vy  = 1.0F / ( float )DrawTex->Tex->TexHeight ;
				VBx = GLINUX.Device.DrawSetting.InvBlendTextureWidth  ;
				VBy = GLINUX.Device.DrawSetting.InvBlendTextureHeight ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

				DrawPos[0].x = TexVert[0].x + xf ;
				DrawPos[0].y = TexVert[0].y + yf ;
				DrawPos[1].x = TexVert[1].x + xf ;
				DrawPos[1].y = TexVert[0].y + yf ;
				DrawPos[2].x = TexVert[0].x + xf ;
				DrawPos[2].y = TexVert[2].y + yf ;
				DrawPos[3].x = TexVert[1].x + xf ;
				DrawPos[3].y = TexVert[2].y + yf ;

				u2[2] = u2[0] = TexVert[0].u ;
				u2[3] = u2[1] = TexVert[1].u ;
				v2[1] = v2[0] = TexVert[0].v ;
				v2[3] = v2[2] = TexVert[2].v ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u1[2] = u1[0] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u1[2] = u1[0] = ( ( xf + TexVert[0].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( ( xf + TexVert[1].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( ( yf + TexVert[0].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( ( yf + TexVert[2].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				for( j = 0 ; j < 4 ; j ++ )
				{
					// 描画矩形から出ていた場合の補正処理
					if( DrawPos[ j ].x < left )
					{
						u2[ j ] += ( left - DrawPos[ j ].x ) * Vx ;
						u1[ j ] += ( left - DrawPos[ j ].x ) * VBx ;
						DrawPos[ j ].x = left ;
					}
					else
					if( DrawPos[ j ].x > right )
					{
						u2[ j ] -= ( DrawPos[ j ].x - right ) * Vx ;
						u1[ j ] -= ( DrawPos[ j ].x - right ) * VBx ;
						DrawPos[ j ].x = right ;
					}	

					if( DrawPos[ j ].y < top )
					{
						v2[ j ] += ( top - DrawPos[ j ].y ) * Vy ;
						v1[ j ] += ( top - DrawPos[ j ].y ) * VBy ;
						DrawPos[ j ].y = top ;
					}
					else
					if( DrawPos[ j ].y > bottom )
					{
						v2[ j ] -= ( DrawPos[ j ].y - bottom ) * Vy ;
						v1[ j ] -= ( DrawPos[ j ].y - bottom ) * VBy ;
						DrawPos[ j ].y = bottom ;
					}
				}

				DrawVertB[0].pos.x = DrawPos[0].x ;
				DrawVertB[0].pos.y = DrawPos[0].y ;
				DrawVertB[1].pos.x = DrawPos[1].x ;
				DrawVertB[1].pos.y = DrawPos[1].y ;
				DrawVertB[2].pos.x = DrawPos[2].x ;
				DrawVertB[2].pos.y = DrawPos[2].y ;
				DrawVertB[3].pos.x = DrawPos[3].x ;
				DrawVertB[3].pos.y = DrawPos[3].y ;
				DrawVertB[4].pos.x = DrawPos[2].x ;
				DrawVertB[4].pos.y = DrawPos[2].y ;
				DrawVertB[5].pos.x = DrawPos[1].x ;
				DrawVertB[5].pos.y = DrawPos[1].y ;

				DrawVertB[0].u1 = u1[0] ;
				DrawVertB[0].v1 = v1[0] ;
				DrawVertB[1].u1 = u1[1] ;
				DrawVertB[1].v1 = v1[1] ;
				DrawVertB[2].u1 = u1[2] ;
				DrawVertB[2].v1 = v1[2] ;
				DrawVertB[3].u1 = u1[3] ;
				DrawVertB[3].v1 = v1[3] ;
				DrawVertB[4].u1 = u1[2] ;
				DrawVertB[4].v1 = v1[2] ;
				DrawVertB[5].u1 = u1[1] ;
				DrawVertB[5].v1 = v1[1] ;

				DrawVertB[0].u2 = u2[0] ;
				DrawVertB[0].v2 = v2[0] ;
				DrawVertB[1].u2 = u2[1] ;
				DrawVertB[1].v2 = v2[1] ;
				DrawVertB[2].u2 = u2[2] ;
				DrawVertB[2].v2 = v2[2] ;
				DrawVertB[3].u2 = u2[3] ;
				DrawVertB[3].v2 = v2[3] ;
				DrawVertB[4].u2 = u2[2] ;
				DrawVertB[4].v2 = v2[2] ;
				DrawVertB[5].u2 = u2[1] ;
				DrawVertB[5].v2 = v2[1] ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
			break ;
		
		case VERTEXTYPE_TEX :
			// ブレンドテクスチャを使用しない

			// テクスチャーの数だけ繰り返す
			i = 0 ;
			for(;;)
			{
				VECTOR DrawPos[ 4 ] ;
				float u[ 4 ] ;
				float v[ 4 ] ;

				// 頂点バッファの取得
				GETVERTEX_QUAD( DrawVert )
				
				// 頂点データ準備
				TexVert = DrawTex->Vertex ;
				DrawVert[0].color	= DiffuseColor ;
				DrawVert[1].color	= DiffuseColor ;
				DrawVert[2].color	= DiffuseColor ;
				DrawVert[3].color	= DiffuseColor ;
				DrawVert[4].color	= DiffuseColor ;
				DrawVert[5].color	= DiffuseColor ;

				DrawPos[0].x = TexVert[0].x + xf ;
				DrawPos[0].y = TexVert[0].y + yf ;
				DrawPos[1].x = TexVert[1].x + xf ;
				DrawPos[1].y = TexVert[0].y + yf ;
				DrawPos[2].x = TexVert[0].x + xf ;
				DrawPos[2].y = TexVert[2].y + yf ;
				DrawPos[3].x = TexVert[1].x + xf ;
				DrawPos[3].y = TexVert[2].y + yf ;

				u[2] = u[0] = TexVert[0].u ;
				u[3] = u[1] = TexVert[1].u ;
				v[1] = v[0] = TexVert[0].v ;
				v[3] = v[2] = TexVert[2].v ;
				
				// 頂点データを初期化する
				Vx	= 1.0F / ( float )DrawTex->Tex->TexWidth ;
				Vy	= 1.0F / ( float )DrawTex->Tex->TexHeight ;
				for( j = 0 ; j < 4 ; j ++ )
				{
					// 描画矩形から出ていた場合の補正処理
					if( DrawPos[ j ].x < left )
					{
						u[ j ] += ( left - DrawPos[ j ].x ) * Vx ;
						DrawPos[ j ].x = left ;
					}
					else
					if( DrawPos[ j ].x > right )
					{
						u[ j ] -= ( DrawPos[ j ].x - right ) * Vx ;
						DrawPos[ j ].x = right ;
					}	

					if( DrawPos[ j ].y < top )
					{
						v[ j ] += ( top - DrawPos[ j ].y ) * Vy ;
						DrawPos[ j ].y = top ;
					}
					else
					if( DrawPos[ j ].y > bottom )
					{
						v[ j ] -= ( DrawPos[ j ].y - bottom ) * Vy ;
						DrawPos[ j ].y = bottom ;
					}
				}

				DrawVert[0].pos.x = DrawPos[0].x ;
				DrawVert[0].pos.y = DrawPos[0].y ;
				DrawVert[1].pos.x = DrawPos[1].x ;
				DrawVert[1].pos.y = DrawPos[1].y ;
				DrawVert[2].pos.x = DrawPos[2].x ;
				DrawVert[2].pos.y = DrawPos[2].y ;
				DrawVert[3].pos.x = DrawPos[3].x ;
				DrawVert[3].pos.y = DrawPos[3].y ;
				DrawVert[4].pos.x = DrawPos[2].x ;
				DrawVert[4].pos.y = DrawPos[2].y ;
				DrawVert[5].pos.x = DrawPos[1].x ;
				DrawVert[5].pos.y = DrawPos[1].y ;

				DrawVert[0].u = u[0] ;
				DrawVert[0].v = v[0] ;
				DrawVert[1].u = u[1] ;
				DrawVert[1].v = v[1] ;
				DrawVert[2].u = u[2] ;
				DrawVert[2].v = v[2] ;
				DrawVert[3].u = u[3] ;
				DrawVert[3].v = v[3] ;
				DrawVert[4].u = u[2] ;
				DrawVert[4].v = v[2] ;
				DrawVert[5].u = u[1] ;
				DrawVert[5].v = v[1] ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// 描画するテクスチャーのセット
				DrawTex ++ ;
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DeviceState_NormalDrawSetup() ;
			}
			break ;
		}
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawExtendGraph
extern	int		Graphics_Hardware_DrawExtendGraph_PF( int x1, int y1, int x2, int y2, float x1f, float y1f, float x2f, float y2f, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ; 
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	IMAGEDATA_ORIG *Orig ;
	int DrawTexNum ;
	int i, j ;
	int BlendGraphNoIncFlag ;
	float GraphExRateX, GraphExRateY ;
//	int ReverseX = FALSE, ReverseY = FALSE ;
//	int Reverse ;
//	int AdjX = FALSE, AdjY = FALSE ;
	float left, right, top, bottom ;
	float Vx, Vy ;
	float VBx, VBy ;
	DWORD DiffuseColor ;
	DWORD drawz ;
	int Flag ;
	float XBuf[ 2 ] ;
	float YBuf[ 2 ] ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	Orig = Image->Orig ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画判定
//	if( x2 - x1 == Image->Width && y2 - y1 == Image->Height ) return G_D3D_DrawGraph( x1, y1, GraphData, TransFlag ) ; 	

	// 幅がなかった場合はエラー
	if( x1 == x2 || y1 == y2 ) return -1 ;

	// 描画の準備
	Flag = TransFlag | ( IntFlag ? ( GSYS.DrawSetting.DrawMode == DX_DRAWMODE_BILINEAR ? 0 : DX_LINUX_DRAWPREP_VECTORINT ) : 0 ) ;
	DX_LINUX_DRAWPREP_TEX( Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )

	// ディフューズカラーの取得
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// 描画情報の数をセット
	DrawTexNum = Image->Hard.DrawNum ;

	// 描画情報の数がブレンド画像と異なっていたら０番目のテクスチャだけを使用する
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// 描画情報配列のアドレスをセットしておく
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 拡大率を取得しておく
	GraphExRateX = ( x2f - x1f ) / Image->WidthF ;
	GraphExRateY = ( y2f - y1f ) / Image->HeightF ;
//	Reverse = ( GraphExRateX < 0 ) != ( GraphExRateY < 0 ) ;

//	x1f -= 0.5F ;
//	y1f -= 0.5F ;

	if( x1 > x2 ){ i = x2; x2 = x1; x1 = i; }
	if( y1 > y2 ){ i = y2; y2 = y1; y1 = i; }

	// 描画範囲からはみ出ているかどうかで処理を分岐
	if( GSYS.DrawSetting.Valid2DMatrix || IN_DRAWAREA_CHECK( x1, y1, x2, y2 ) )
	{
		// 頂点タイプによって処理を分岐
		switch( GLINUX.Device.DrawInfo.VertexType )
		{
		case VERTEXTYPE_BLENDTEX :	// ブレンドテクスチャを使用する
			// テクスチャーの数だけ繰り返す
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// 描画するテクスチャーのセット
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

				if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}

				// 頂点データを初期化する
				GETVERTEX_QUAD( DrawVertB )
				
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

				XBuf[ 0 ] = TexVert[0].x * GraphExRateX + x1f ;
				XBuf[ 1 ] = TexVert[1].x * GraphExRateX + x1f ;
				YBuf[ 0 ] = TexVert[0].y * GraphExRateY + y1f ;
				YBuf[ 1 ] = TexVert[2].y * GraphExRateY + y1f ;
				DrawVertB[0].pos.x = XBuf[ 0 ] ;
				DrawVertB[0].pos.y = YBuf[ 0 ] ;
				DrawVertB[1].pos.x = XBuf[ 1 ] ;
				DrawVertB[1].pos.y = YBuf[ 0 ] ;
				DrawVertB[2].pos.x = XBuf[ 0 ] ;
				DrawVertB[2].pos.y = YBuf[ 1 ] ;
				DrawVertB[3].pos.x = XBuf[ 1 ] ;
				DrawVertB[3].pos.y = YBuf[ 1 ] ;
				DrawVertB[4].pos.x = XBuf[ 0 ] ;
				DrawVertB[4].pos.y = YBuf[ 1 ] ;
				DrawVertB[5].pos.x = XBuf[ 1 ] ;
				DrawVertB[5].pos.y = YBuf[ 0 ] ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ;
					u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ;
					v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ;
					v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u[ 0 ] = XBuf[ 0 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ;
					u[ 1 ] = XBuf[ 1 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ;
					v[ 0 ] = YBuf[ 0 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ;
					v[ 1 ] = YBuf[ 1 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ;
					break ;
				}

				u[ 0 ] *= GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] *= GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] *= GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] *= GLINUX.Device.DrawSetting.InvBlendTextureHeight ;

				DrawVertB[0].u1 = u[ 0 ] ;
				DrawVertB[0].v1 = v[ 0 ] ;
				DrawVertB[1].u1 = u[ 1 ] ;
				DrawVertB[1].v1 = v[ 0 ] ;
				DrawVertB[2].u1 = u[ 0 ] ;
				DrawVertB[2].v1 = v[ 1 ] ;
				DrawVertB[3].u1 = u[ 1 ] ;
				DrawVertB[3].v1 = v[ 1 ] ;
				DrawVertB[4].u1 = u[ 0 ] ;
				DrawVertB[4].v1 = v[ 1 ] ;
				DrawVertB[5].u1 = u[ 1 ] ;
				DrawVertB[5].v1 = v[ 0 ] ;

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
			break ;

		case VERTEXTYPE_TEX :	// ブレンドテクスチャを使用しない
			// テクスチャーの数だけ繰り返す
			i = 0 ;
			for(;;)
			{
				// 頂点データを初期化する
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;
				
				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

				XBuf[ 0 ] = TexVert[0].x * GraphExRateX + x1f ;
				XBuf[ 1 ] = TexVert[1].x * GraphExRateX + x1f ;
				YBuf[ 0 ] = TexVert[0].y * GraphExRateY + y1f ;
				YBuf[ 1 ] = TexVert[2].y * GraphExRateY + y1f ;
				DrawVert[0].pos.x = XBuf[ 0 ] ;
				DrawVert[0].pos.y = YBuf[ 0 ] ;
				DrawVert[1].pos.x = XBuf[ 1 ] ;
				DrawVert[1].pos.y = YBuf[ 0 ] ;
				DrawVert[2].pos.x = XBuf[ 0 ] ;
				DrawVert[2].pos.y = YBuf[ 1 ] ;
				DrawVert[3].pos.x = XBuf[ 1 ] ;
				DrawVert[3].pos.y = YBuf[ 1 ] ;
				DrawVert[4].pos.x = XBuf[ 0 ] ;
				DrawVert[4].pos.y = YBuf[ 1 ] ;
				DrawVert[5].pos.x = XBuf[ 1 ] ;
				DrawVert[5].pos.y = YBuf[ 0 ] ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// 描画するテクスチャーのセット
				DrawTex ++ ;
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DeviceState_NormalDrawSetup() ;
			}
			break ;
		}
	}
	else
	{
		// 完全にはみ出ていたら何もせず終了
		if( OUT_DRAWAREA_CHECK( x1, y1, x2, y2 ) ) return 0 ;

		// 描画矩形の保存
		left	= GSYS.DrawSetting.DrawAreaF.left   ;
		top		= GSYS.DrawSetting.DrawAreaF.top    ;
		right	= GSYS.DrawSetting.DrawAreaF.right  ;
		bottom	= GSYS.DrawSetting.DrawAreaF.bottom ;

		// 頂点タイプによって処理を分岐
		switch( GLINUX.Device.DrawInfo.VertexType )
		{
		case VERTEXTYPE_BLENDTEX :	// ブレンドテクスチャを使用する
			// テクスチャーの数だけ繰り返す
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				VECTOR DrawPos[ 4 ] ;
				float u1[ 4 ] ;
				float v1[ 4 ] ;
				float u2[ 4 ] ;
				float v2[ 4 ] ;

				// 描画するテクスチャーのセット
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

				if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}

				GETVERTEX_QUAD( DrawVertB )
				
				Vx = 1.0F / ( DrawTex->Tex->TexWidth * GraphExRateX ) ;
				Vy = 1.0F / ( DrawTex->Tex->TexHeight * GraphExRateY ) ;
				VBx = GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				VBy = GLINUX.Device.DrawSetting.InvBlendTextureHeight ;

				// 頂点データを初期化する
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

				XBuf[ 0 ] = TexVert[0].x * GraphExRateX + x1f ;
				XBuf[ 1 ] = TexVert[1].x * GraphExRateX + x1f ;
				YBuf[ 0 ] = TexVert[0].y * GraphExRateY + y1f ;
				YBuf[ 1 ] = TexVert[2].y * GraphExRateY + y1f ;
				DrawPos[0].x = XBuf[ 0 ] ;
				DrawPos[0].y = YBuf[ 0 ] ;
				DrawPos[1].x = XBuf[ 1 ] ;
				DrawPos[1].y = YBuf[ 0 ] ;
				DrawPos[2].x = XBuf[ 0 ] ;
				DrawPos[2].y = YBuf[ 1 ] ;
				DrawPos[3].x = XBuf[ 1 ] ;
				DrawPos[3].y = YBuf[ 1 ] ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u1[2] = u1[0] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u1[2] = u1[0] = ( XBuf[ 0 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( XBuf[ 1 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( YBuf[ 0 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( YBuf[ 1 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				u2[2] = u2[0] = TexVert[0].u ;
				u2[3] = u2[1] = TexVert[1].u ;
				v2[1] = v2[0] = TexVert[0].v ;
				v2[3] = v2[2] = TexVert[2].v ;

				for( j = 0 ; j < 4 ; j ++ )
				{
					// 描画矩形から出ていた場合の補正処理
					if( DrawPos[ j ].x < left )
					{
						u2[ j ] += ( left - DrawPos[ j ].x ) * Vx ;
						u1[ j ] += ( left - DrawPos[ j ].x ) * VBx ;
						DrawPos[ j ].x = left ;
					}
					else
					if( DrawPos[ j ].x > right )
					{
						u2[ j ] -= ( DrawPos[ j ].x - right ) * Vx ;
						u1[ j ] -= ( DrawPos[ j ].x - right ) * VBx ;
						DrawPos[ j ].x = right ;
					}	

					if( DrawPos[ j ].y < top )
					{
						v2[ j ] += ( top - DrawPos[ j ].y ) * Vy ;
						v1[ j ] += ( top - DrawPos[ j ].y ) * VBy ;
						DrawPos[ j ].y = top ;
					}
					else
					if( DrawPos[ j ].y > bottom )
					{
						v2[ j ] -= ( DrawPos[ j ].y - bottom ) * Vy ;
						v1[ j ] -= ( DrawPos[ j ].y - bottom ) * VBy ;
						DrawPos[ j ].y = bottom ;
					}
				}

				DrawVertB[0].pos.x = DrawPos[0].x ;
				DrawVertB[0].pos.y = DrawPos[0].y ;
				DrawVertB[1].pos.x = DrawPos[1].x ;
				DrawVertB[1].pos.y = DrawPos[1].y ;
				DrawVertB[2].pos.x = DrawPos[2].x ;
				DrawVertB[2].pos.y = DrawPos[2].y ;
				DrawVertB[3].pos.x = DrawPos[3].x ;
				DrawVertB[3].pos.y = DrawPos[3].y ;
				DrawVertB[4].pos.x = DrawPos[2].x ;
				DrawVertB[4].pos.y = DrawPos[2].y ;
				DrawVertB[5].pos.x = DrawPos[1].x ;
				DrawVertB[5].pos.y = DrawPos[1].y ;

				DrawVertB[0].u1 = u1[0] ;
				DrawVertB[0].v1 = v1[0] ;
				DrawVertB[1].u1 = u1[1] ;
				DrawVertB[1].v1 = v1[1] ;
				DrawVertB[2].u1 = u1[2] ;
				DrawVertB[2].v1 = v1[2] ;
				DrawVertB[3].u1 = u1[3] ;
				DrawVertB[3].v1 = v1[3] ;
				DrawVertB[4].u1 = u1[2] ;
				DrawVertB[4].v1 = v1[2] ;
				DrawVertB[5].u1 = u1[1] ;
				DrawVertB[5].v1 = v1[1] ;

				DrawVertB[0].u2 = u2[0] ;
				DrawVertB[0].v2 = v2[0] ;
				DrawVertB[1].u2 = u2[1] ;
				DrawVertB[1].v2 = v2[1] ;
				DrawVertB[2].u2 = u2[2] ;
				DrawVertB[2].v2 = v2[2] ;
				DrawVertB[3].u2 = u2[3] ;
				DrawVertB[3].v2 = v2[3] ;
				DrawVertB[4].u2 = u2[2] ;
				DrawVertB[4].v2 = v2[2] ;
				DrawVertB[5].u2 = u2[1] ;
				DrawVertB[5].v2 = v2[1] ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
			break ;

		case VERTEXTYPE_TEX :	// ブレンドテクスチャを使用しない
			// テクスチャーの数だけ繰り返す
			i = 0 ;
			for(;;)
			{
				VECTOR DrawPos[ 4 ] ;
				float u[ 4 ] ;
				float v[ 4 ] ;

				// 頂点データを初期化する
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

				XBuf[ 0 ] = TexVert[0].x * GraphExRateX + x1f ;
				XBuf[ 1 ] = TexVert[1].x * GraphExRateX + x1f ;
				YBuf[ 0 ] = TexVert[0].y * GraphExRateY + y1f ;
				YBuf[ 1 ] = TexVert[2].y * GraphExRateY + y1f ;
				DrawPos[0].x = XBuf[ 0 ] ;
				DrawPos[0].y = YBuf[ 0 ] ;
				DrawPos[1].x = XBuf[ 1 ] ;
				DrawPos[1].y = YBuf[ 0 ] ;
				DrawPos[2].x = XBuf[ 0 ] ;
				DrawPos[2].y = YBuf[ 1 ] ;
				DrawPos[3].x = XBuf[ 1 ] ;
				DrawPos[3].y = YBuf[ 1 ] ;

				u[2] = u[0] = TexVert[0].u ;
				u[3] = u[1] = TexVert[1].u ;
				v[1] = v[0] = TexVert[0].v ;
				v[3] = v[2] = TexVert[2].v ;

				Vx = 1.0F / ( DrawTex->Tex->TexWidth * GraphExRateX ) ;
				Vy = 1.0F / ( DrawTex->Tex->TexHeight * GraphExRateY ) ;
				for( j = 0 ; j < 4 ; j ++ )
				{
					// 描画矩形から出ていた場合の補正処理
					if( DrawPos[ j ].x < left )
					{
						u[ j ] += ( left - DrawPos[ j ].x ) * Vx ;
						DrawPos[ j ].x = left ;
					}
					else
					if( DrawPos[ j ].x > right )
					{
						u[ j ] -= ( DrawPos[ j ].x - right ) * Vx ;
						DrawPos[ j ].x = right ;
					}	

					if( DrawPos[ j ].y < top )
					{
						v[ j ] += ( top - DrawPos[ j ].y ) * Vy ;
						DrawPos[ j ].y = top ;
					}
					else
					if( DrawPos[ j ].y > bottom )
					{
						v[ j ] -= ( DrawPos[ j ].y - bottom ) * Vy ;
						DrawPos[ j ].y = bottom ;
					}
				}

				DrawVert[0].pos.x = DrawPos[0].x ;
				DrawVert[0].pos.y = DrawPos[0].y ;
				DrawVert[1].pos.x = DrawPos[1].x ;
				DrawVert[1].pos.y = DrawPos[1].y ;
				DrawVert[2].pos.x = DrawPos[2].x ;
				DrawVert[2].pos.y = DrawPos[2].y ;
				DrawVert[3].pos.x = DrawPos[3].x ;
				DrawVert[3].pos.y = DrawPos[3].y ;
				DrawVert[4].pos.x = DrawPos[2].x ;
				DrawVert[4].pos.y = DrawPos[2].y ;
				DrawVert[5].pos.x = DrawPos[1].x ;
				DrawVert[5].pos.y = DrawPos[1].y ;

				DrawVert[0].u = u[0] ;
				DrawVert[0].v = v[0] ;
				DrawVert[1].u = u[1] ;
				DrawVert[1].v = v[1] ;
				DrawVert[2].u = u[2] ;
				DrawVert[2].v = v[2] ;
				DrawVert[3].u = u[3] ;
				DrawVert[3].v = v[3] ;
				DrawVert[4].u = u[2] ;
				DrawVert[4].v = v[2] ;
				DrawVert[5].u = u[1] ;
				DrawVert[5].v = v[1] ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// 描画するテクスチャーのセット
				DrawTex ++ ;
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DeviceState_NormalDrawSetup() ;
			}
			break ;
		}
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawRotaGraph
extern	int		Graphics_Hardware_DrawRotaGraph_PF( int x, int y, float xf, float yf, double ExRate, double Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int ReverseXFlag, int ReverseYFlag, int IntFlag )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	VECTOR DrawPos[ 4 ] ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	int DrawTexNum ;
	int i ; 
	float Sin = 0.0f, Cos = 1.0f ;
	int BlendGraphNoIncFlag ;
	float XBuf[ 8 ], YBuf[ 8 ] ;
	float u[ 4 ], v[ 4 ] ;
	float ExtendRate = ( float )ExRate ; 
	float ExtendRateX ;
	float ExtendRateY ;
	DWORD drawz ;
//	BOOL Adj = FALSE ;
	float CenX, CenY ;
	float fx, fy ;
	int hx = 0, hy = 0 ;
	int FastCode, SinCosNone, AngleCheck, AlwaysCheck ;
	int Flag ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 幅がなかった場合はエラー
	if( ExRate == 0.0 ) return -1 ;

	// 描画の準備
	Flag = TransFlag ;
	DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )

	// 回転要素が無いか調べる
	if( Angle == 0.0 )
	{
		SinCosNone = 1;
	}
	else
	{
		SinCosNone = 0;
		_SINCOS( (float)Angle, &Sin, &Cos ) ;
	}
//	if( _FABS( (float)Angle ) < 0.0001 && ExRate < 1.001 && ExRate > 0.999 ) Adj = TRUE ;

	// 座標の補正
	if( IntFlag || GSYS.DrawSetting.DrawMode == DX_DRAWMODE_NEAREST )
	{
		AlwaysCheck = 0 ;
		FastCode    = 0 ;
		hx          = Image->WidthI % 2 ;
		hy          = Image->HeightI % 2 ;
		fx = xf - ( float )x ;
		fy = yf - ( float )y ;
		if( fx < 0.0f ) fx = -fx;
		if( fy < 0.0f ) fy = -fy;
		if( fx < 0.00001f && fy < 0.00001f )
		{
			if( ExRate == 1.0 )
			{
				if( hx == hy )
				{
					if( hx == 0 )
					{
						CenX = ( float )(Image->WidthI >> 1) ;
						CenY = ( float )(Image->HeightI >> 1) ;
						xf   = ( float )x ;
						yf   = ( float )y ;
					}
					else
					{
						CenX = ( float )Image->WidthI * 0.5F ;
						CenY = ( float )Image->HeightI * 0.5F ;
						xf   = ( float )x - 0.5F ;
						yf   = ( float )y - 0.5F ;
						FastCode = 1;
					}
				}
				else
				{
					CenX = ( float )Image->WidthI * 0.5F ;
					CenY = ( float )Image->HeightI * 0.5F ;
					xf   = ( float )x ;
					yf   = ( float )y ;
						
					if( hx != 0 ) CenX -= 0.5F ;
					if( hy != 0 ) CenY -= 0.5F ;
				}
			}
			else
			{
				CenX = ( float )Image->WidthI * 0.5F ;
				CenY = ( float )Image->HeightI * 0.5F ;
				xf   = ( float )x ;
				yf   = ( float )y ;
				FastCode = 1 ;
			}
		}
		else
		{
			CenX = ( float )Image->WidthI * 0.5F ;
			CenY = ( float )Image->HeightI * 0.5F ;
			xf   = ( float )x + 0.5f ;
			yf   = ( float )y + 0.5f ;
			AlwaysCheck = 1 ;
		}
	}
	else
	{
		CenX = ( float )Image->WidthF * 0.5F ;
		CenY = ( float )Image->HeightF * 0.5F ;
//		xf -= 0.5F ;
//		yf -= 0.5F ;
		AngleCheck = 0 ;
		AlwaysCheck = 0 ;
		FastCode = 1 ;
	}

	// 微妙な回転値のチェックを行うかどうかを調べる
	if( IntFlag == TRUE && AlwaysCheck == 0 && SinCosNone == 0 && ( hx == 1 || hy == 1 ) &&
		( ( Angle < DX_PI / 180.0 *         - 4.0   ) ||
		  ( Angle > DX_PI / 180.0 * ( 360.0 + 4.0 ) ) ||
		  ( Angle > DX_PI / 180.0 *         - 4.0   && Angle < DX_PI / 180.0 *           4.0   ) ||
		  ( Angle > DX_PI / 180.0 * (  90.0 - 4.0 ) && Angle < DX_PI / 180.0 * (  90.0 + 4.0 ) ) ||
		  ( Angle > DX_PI / 180.0 * ( 180.0 - 4.0 ) && Angle < DX_PI / 180.0 * ( 180.0 + 4.0 ) ) ||
		  ( Angle > DX_PI / 180.0 * ( 270.0 - 4.0 ) && Angle < DX_PI / 180.0 * ( 270.0 + 4.0 ) ) ||
		  ( Angle > DX_PI / 180.0 * ( 360.0 - 4.0 ) && Angle < DX_PI / 180.0 * ( 360.0 + 4.0 ) ) ) )
	{
		AngleCheck = 1 ;
	}
	else
	{
		AngleCheck = 0 ;
	}

	// 頂点データを取得（今のところ色情報のセットのみ）
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// 描画情報の数をセット
	DrawTexNum = Image->Hard.DrawNum ;

	// 描画情報の数がブレンド画像と異なっていたら０番目のテクスチャだけを使用する
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// 描画情報配列のアドレスをセットしておく
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 反転を考慮した拡大率をセット
	ExtendRateX = ( ( ReverseXFlag == TRUE ) ? -1.0f : 1.0f ) * ExtendRate ;
	ExtendRateY = ( ( ReverseYFlag == TRUE ) ? -1.0f : 1.0f ) * ExtendRate ;

	// 頂点タイプによって処理を分岐
	switch( GLINUX.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// ブレンドテクスチャを使用する
		// テクスチャーの数だけ繰り返す
		if( DrawTexNum == 1 && FastCode == 1 )
		{
			// 描画するテクスチャーのセット
			Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
			Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

			if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_Linux_DeviceState_NormalDrawSetup() ;
			}

			// 頂点データを初期化する
			GETVERTEX_QUAD( DrawVertB )

			TexVert = DrawTex->Vertex ;
			BlendTexVert = BlendDrawTex->Vertex ;

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;
			DrawVertB[4].color = ( DWORD )DiffuseColor ;
			DrawVertB[5].color = ( DWORD )DiffuseColor ;

			XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
			YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY ;
			XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
			YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

			XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
			YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRateY ;
			XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
			YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

			DrawVertB[0].pos.x =  XBuf[1] + xf ;
			DrawVertB[0].pos.y =  YBuf[1] + yf ;
			DrawVertB[1].pos.x =  XBuf[3] + xf ;
			DrawVertB[1].pos.y =  YBuf[3] + yf ;
			DrawVertB[2].pos.x = -XBuf[3] + xf ;
			DrawVertB[2].pos.y = -YBuf[3] + yf ;
			DrawVertB[3].pos.x = -XBuf[1] + xf ;
			DrawVertB[3].pos.y = -YBuf[1] + yf ;
			DrawVertB[4].pos.x = -XBuf[3] + xf ;
			DrawVertB[4].pos.y = -YBuf[3] + yf ;
			DrawVertB[5].pos.x =  XBuf[3] + xf ;
			DrawVertB[5].pos.y =  YBuf[3] + yf ;

			switch( GSYS.DrawSetting.BlendGraphPosMode )
			{
			default :
			case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
				u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[0].u1 = u[ 0 ] ;
				DrawVertB[0].v1 = v[ 0 ] ;
				DrawVertB[1].u1 = u[ 1 ] ;
				DrawVertB[1].v1 = v[ 0 ] ;
				DrawVertB[2].u1 = u[ 0 ] ;
				DrawVertB[2].v1 = v[ 1 ] ;
				DrawVertB[3].u1 = u[ 1 ] ;
				DrawVertB[3].v1 = v[ 1 ] ;
				DrawVertB[4].u1 = u[ 0 ] ;
				DrawVertB[4].v1 = v[ 1 ] ;
				DrawVertB[5].u1 = u[ 1 ] ;
				DrawVertB[5].v1 = v[ 0 ] ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				DrawVertB[0].u1 = ( (  XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[0].v1 = ( (  YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[1].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[1].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[2].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[2].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[3].u1 = ( ( -XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[3].v1 = ( ( -YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[4].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[4].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[5].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[5].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				break ;
			}

			DrawVertB[0].u2 = TexVert[0].u ;
			DrawVertB[0].v2 = TexVert[0].v ;
			DrawVertB[1].u2 = TexVert[1].u ;
			DrawVertB[1].v2 = TexVert[0].v ;
			DrawVertB[2].u2 = TexVert[0].u ;
			DrawVertB[2].v2 = TexVert[2].v ;
			DrawVertB[3].u2 = TexVert[1].u ;
			DrawVertB[3].v2 = TexVert[2].v ;
			DrawVertB[4].u2 = TexVert[0].u ;
			DrawVertB[4].v2 = TexVert[2].v ;
			DrawVertB[5].u2 = TexVert[1].u ;
			DrawVertB[5].v2 = TexVert[0].v ;

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;
			DrawVertB[3].rhw = 1.0f ;
			DrawVertB[4].rhw = 1.0f ;
			DrawVertB[5].rhw = 1.0f ;

			// テクスチャーを描画する
			ADD4VERTEX_BLENDTEX

			if( BlendGraphNoIncFlag == FALSE )
			{
				BlendDrawTex ++ ;
			}
		}
		else
		{
			// テクスチャーの数だけ繰り返す
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// 描画するテクスチャーのセット
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

				if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}

				// 頂点データを初期化する
				GETVERTEX_QUAD( DrawVertB )

				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY ;
				XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
				YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

				XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRateY ;
				XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
				YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

				XBuf[4] = ( TexVert[2].x - CenX ) * ExtendRateX ;
				YBuf[4] = ( TexVert[2].y - CenY ) * ExtendRateY ;
				XBuf[5] = XBuf[4] * Cos - YBuf[4] * Sin ;
				YBuf[5] = XBuf[4] * Sin + YBuf[4] * Cos ;

				XBuf[6] = ( TexVert[3].x - CenX ) * ExtendRateX ;
				YBuf[6] = ( TexVert[3].y - CenY ) * ExtendRateY ;
				XBuf[7] = XBuf[6] * Cos - YBuf[6] * Sin ;
				YBuf[7] = XBuf[6] * Sin + YBuf[6] * Cos ;

				DrawVertB[0].pos.x = XBuf[1] + xf ;
				DrawVertB[0].pos.y = YBuf[1] + yf ;
				DrawVertB[1].pos.x = XBuf[3] + xf ;
				DrawVertB[1].pos.y = YBuf[3] + yf ;
				DrawVertB[2].pos.x = XBuf[5] + xf ;
				DrawVertB[2].pos.y = YBuf[5] + yf ;
				DrawVertB[3].pos.x = XBuf[7] + xf ;
				DrawVertB[3].pos.y = YBuf[7] + yf ;
				DrawVertB[4].pos.x = XBuf[5] + xf ;
				DrawVertB[4].pos.y = YBuf[5] + yf ;
				DrawVertB[5].pos.x = XBuf[3] + xf ;
				DrawVertB[5].pos.y = YBuf[3] + yf ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 2 ] = ( TexVert[2].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 3 ] = ( TexVert[3].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[1].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 2 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 3 ] = ( TexVert[3].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;

					DrawVertB[0].u1 = u[ 0 ] ;
					DrawVertB[0].v1 = v[ 0 ] ;
					DrawVertB[1].u1 = u[ 1 ] ;
					DrawVertB[1].v1 = v[ 1 ] ;
					DrawVertB[2].u1 = u[ 2 ] ;
					DrawVertB[2].v1 = v[ 2 ] ;
					DrawVertB[3].u1 = u[ 3 ] ;
					DrawVertB[3].v1 = v[ 3 ] ;
					DrawVertB[4].u1 = u[ 2 ] ;
					DrawVertB[4].v1 = v[ 2 ] ;
					DrawVertB[5].u1 = u[ 1 ] ;
					DrawVertB[5].v1 = v[ 1 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[0].u1 = ( ( XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( ( YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( ( XBuf[7] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( ( YBuf[7] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
		}
		break ;

	case VERTEXTYPE_TEX :	// ブレンドテクスチャを使用しない
		// テクスチャーの数だけ繰り返す
		if( DrawTexNum == 1 && FastCode == 1 )
		{
			if( SinCosNone == 0 )
			{
				// 頂点データを初期化する
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY ;
				XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
				YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;
				
				XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRateY ;
				XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
				YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

				if( AlwaysCheck )
				{
					DrawPos[0].x =  XBuf[1] + xf ;
					DrawPos[0].y =  YBuf[1] + yf ;
					DrawPos[1].x =  XBuf[3] + xf ;
					DrawPos[1].y =  YBuf[3] + yf ;
					DrawPos[2].x = -XBuf[3] + xf ;
					DrawPos[2].y = -YBuf[3] + yf ;
					DrawPos[3].x = -XBuf[1] + xf ;
					DrawPos[3].y = -YBuf[1] + yf ;

					if( ( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[2].x) ) ||
							( ( DrawPos[0].x > DrawPos[2].x ) && ( DrawPos[0].x < DrawPos[2].x + 0.01f ) ) ||
							( ( DrawPos[2].x > DrawPos[0].x ) && ( DrawPos[2].x < DrawPos[0].x + 0.01f ) ) ) &&
							_FABS( DrawPos[0].x - _FTOL( DrawPos[0].x ) ) < 0.0001f ) ||
						( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[1].x) ) ||
							( ( DrawPos[0].x > DrawPos[1].x ) && ( DrawPos[0].x < DrawPos[1].x + 0.01f ) ) ||
							( ( DrawPos[1].x > DrawPos[0].x ) && ( DrawPos[1].x < DrawPos[0].x + 0.01f ) ) ) &&
							_FABS( DrawPos[0].x - _FTOL( DrawPos[0].x ) ) < 0.0001f ) )
					{
						DrawPos[0].x -= 0.5f;
						DrawPos[1].x -= 0.5f;
						DrawPos[2].x -= 0.5f;
						DrawPos[3].x -= 0.5f;
					}

					if( ( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[2].y) ) ||
							( ( DrawPos[0].y > DrawPos[2].y ) && ( DrawPos[0].y < DrawPos[2].y + 0.01f ) ) ||
							( ( DrawPos[2].y > DrawPos[0].y ) && ( DrawPos[2].y < DrawPos[0].y + 0.01f ) ) ) &&
							_FABS( DrawPos[0].y - _FTOL( DrawPos[0].y ) ) < 0.0001f ) ||
						( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[1].y) ) ||
							( ( DrawPos[0].y > DrawPos[1].y ) && ( DrawPos[0].y < DrawPos[1].y + 0.01f ) ) ||
							( ( DrawPos[1].y > DrawPos[0].y ) && ( DrawPos[1].y < DrawPos[0].y + 0.01f ) ) ) &&
							_FABS( DrawPos[0].y - _FTOL( DrawPos[0].y ) ) < 0.0001f ) )
					{
						DrawPos[0].y -= 0.5f;
						DrawPos[1].y -= 0.5f;
						DrawPos[2].y -= 0.5f;
						DrawPos[3].y -= 0.5f;
					}

					DrawVert[0].pos.x = DrawPos[0].x ;
					DrawVert[0].pos.y = DrawPos[0].y ;
					DrawVert[1].pos.x = DrawPos[1].x ;
					DrawVert[1].pos.y = DrawPos[1].y ;
					DrawVert[2].pos.x = DrawPos[2].x ;
					DrawVert[2].pos.y = DrawPos[2].y ;
					DrawVert[3].pos.x = DrawPos[3].x ;
					DrawVert[3].pos.y = DrawPos[3].y ;
					DrawVert[4].pos.x = DrawPos[2].x ;
					DrawVert[4].pos.y = DrawPos[2].y ;
					DrawVert[5].pos.x = DrawPos[1].x ;
					DrawVert[5].pos.y = DrawPos[1].y ;
				}
				else
				if( AngleCheck )
				{
					DrawPos[0].x =  XBuf[1] + xf ;
					DrawPos[0].y =  YBuf[1] + yf ;
					DrawPos[3].x = -XBuf[1] + xf ;
					DrawPos[3].y = -YBuf[1] + yf ;

					DrawPos[1].x =  XBuf[3] + xf ;
					DrawPos[1].y =  YBuf[3] + yf ;
					DrawPos[2].x = -XBuf[3] + xf ;
					DrawPos[2].y = -YBuf[3] + yf ;

					if( hx &&
						( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[2].x) ) ||
						  ( ( DrawPos[0].x > DrawPos[2].x - 0.01f ) && ( DrawPos[0].x < DrawPos[2].x + 0.01f ) ) ||
						  ( ( DrawPos[2].x > DrawPos[0].x - 0.01f ) && ( DrawPos[2].x < DrawPos[0].x + 0.01f ) ) ||
						  ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[1].x) ) ||
						  ( ( DrawPos[0].x > DrawPos[1].x - 0.01f ) && ( DrawPos[0].x < DrawPos[1].x + 0.01f ) ) ||
						  ( ( DrawPos[1].x > DrawPos[0].x - 0.01f ) && ( DrawPos[1].x < DrawPos[0].x + 0.01f ) ) ) )
					{
						DrawPos[0].x -= 0.5f;
						DrawPos[1].x -= 0.5f;
						DrawPos[2].x -= 0.5f;
						DrawPos[3].x -= 0.5f;
					}

					if( hy &&
						( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[1].y) ) ||
						  ( ( DrawPos[0].y > DrawPos[1].y - 0.01f ) && ( DrawPos[0].y < DrawPos[1].y + 0.01f ) ) ||
						  ( ( DrawPos[1].y > DrawPos[0].y - 0.01f ) && ( DrawPos[1].y < DrawPos[0].y + 0.01f ) ) ||
						  ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[2].y) ) ||
						  ( ( DrawPos[0].y > DrawPos[2].y - 0.01f ) && ( DrawPos[0].y < DrawPos[2].y + 0.01f ) ) ||
						  ( ( DrawPos[2].y > DrawPos[0].y - 0.01f ) && ( DrawPos[2].y < DrawPos[0].y + 0.01f ) ) ) )
					{
						DrawPos[0].y -= 0.5f;
						DrawPos[1].y -= 0.5f;
						DrawPos[2].y -= 0.5f;
						DrawPos[3].y -= 0.5f;
					}

					DrawVert[0].pos.x = DrawPos[0].x ;
					DrawVert[0].pos.y = DrawPos[0].y ;
					DrawVert[1].pos.x = DrawPos[1].x ;
					DrawVert[1].pos.y = DrawPos[1].y ;
					DrawVert[2].pos.x = DrawPos[2].x ;
					DrawVert[2].pos.y = DrawPos[2].y ;
					DrawVert[3].pos.x = DrawPos[3].x ;
					DrawVert[3].pos.y = DrawPos[3].y ;
					DrawVert[4].pos.x = DrawPos[2].x ;
					DrawVert[4].pos.y = DrawPos[2].y ;
					DrawVert[5].pos.x = DrawPos[1].x ;
					DrawVert[5].pos.y = DrawPos[1].y ;
				}
				else
				{
					DrawVert[0].pos.x =  XBuf[1] + xf ;
					DrawVert[0].pos.y =  YBuf[1] + yf ;
					DrawVert[1].pos.x =  XBuf[3] + xf ;
					DrawVert[1].pos.y =  YBuf[3] + yf ;
					DrawVert[2].pos.x = -XBuf[3] + xf ;
					DrawVert[2].pos.y = -YBuf[3] + yf ;
					DrawVert[3].pos.x = -XBuf[1] + xf ;
					DrawVert[3].pos.y = -YBuf[1] + yf ;
					DrawVert[4].pos.x = -XBuf[3] + xf ;
					DrawVert[4].pos.y = -YBuf[3] + yf ;
					DrawVert[5].pos.x =  XBuf[3] + xf ;
					DrawVert[5].pos.y =  YBuf[3] + yf ;
				}

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_TEX
			}
			else
			{
				// 頂点データを初期化する
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY ;

				XBuf[1] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[1] = ( TexVert[1].y - CenY ) * ExtendRateY ;

				DrawVert[0].pos.x =  XBuf[0] + xf ;
				DrawVert[0].pos.y =  YBuf[0] + yf ;
				DrawVert[1].pos.x =  XBuf[1] + xf ;
				DrawVert[1].pos.y =  YBuf[1] + yf ;
				DrawVert[2].pos.x = -XBuf[1] + xf ;
				DrawVert[2].pos.y = -YBuf[1] + yf ;
				DrawVert[3].pos.x = -XBuf[0] + xf ;
				DrawVert[3].pos.y = -YBuf[0] + yf ;
				DrawVert[4].pos.x = -XBuf[1] + xf ;
				DrawVert[4].pos.y = -YBuf[1] + yf ;
				DrawVert[5].pos.x =  XBuf[1] + xf ;
				DrawVert[5].pos.y =  YBuf[1] + yf ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_TEX
			}
		}
		else
		{
			if( SinCosNone == 0 )
			{
				// テクスチャーの数だけ繰り返す
				i = 0 ;
				for(;;)
				{
					// 頂点データを初期化する
					GETVERTEX_QUAD( DrawVert )
					
					TexVert = DrawTex->Vertex ;

					DrawVert[0].color = ( DWORD )DiffuseColor ;
					DrawVert[1].color = ( DWORD )DiffuseColor ;
					DrawVert[2].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;
					DrawVert[4].color = ( DWORD )DiffuseColor ;
					DrawVert[5].color = ( DWORD )DiffuseColor ;

					DrawVert[0].u = TexVert[0].u ;
					DrawVert[0].v = TexVert[0].v ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[1].v = TexVert[0].v ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[2].v = TexVert[2].v ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[3].v = TexVert[2].v ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[4].v = TexVert[2].v ;
					DrawVert[5].u = TexVert[1].u ;
					DrawVert[5].v = TexVert[0].v ;

					XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
					YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY ;
					XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
					YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

					XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
					YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRateY ;
					XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
					YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

					XBuf[4] = ( TexVert[2].x - CenX ) * ExtendRateX ;
					YBuf[4] = ( TexVert[2].y - CenY ) * ExtendRateY ;
					XBuf[5] = XBuf[4] * Cos - YBuf[4] * Sin ;
					YBuf[5] = XBuf[4] * Sin + YBuf[4] * Cos ;

					XBuf[6] = ( TexVert[3].x - CenX ) * ExtendRateX ;
					YBuf[6] = ( TexVert[3].y - CenY ) * ExtendRateY ;
					XBuf[7] = XBuf[6] * Cos - YBuf[6] * Sin ;
					YBuf[7] = XBuf[6] * Sin + YBuf[6] * Cos ;

					if( AlwaysCheck )
					{
						float f;

						DrawPos[0].x = XBuf[1] + xf ;
						DrawPos[0].y = YBuf[1] + yf ;
						DrawPos[1].x = XBuf[3] + xf ;
						DrawPos[1].y = YBuf[3] + yf ;
						DrawPos[2].x = XBuf[5] + xf ;
						DrawPos[2].y = YBuf[5] + yf ;
						DrawPos[3].x = XBuf[7] + xf ;
						DrawPos[3].y = YBuf[7] + yf ;

						f = _FABS( DrawPos[0].x - _FTOL( DrawPos[0].x ) );
						if( ( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[2].x) ) ||
							    ( ( DrawPos[0].x >= DrawPos[2].x - 0.01f ) && ( DrawPos[0].x < DrawPos[2].x + 0.01f ) ) ||
							    ( ( DrawPos[2].x >= DrawPos[0].x - 0.01f ) && ( DrawPos[2].x < DrawPos[0].x + 0.01f ) ) ) &&
							    ( f < 0.001f || f > 0.999f ) ) ||
							( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[1].x) ) ||
							    ( ( DrawPos[0].x >= DrawPos[1].x - 0.01f ) && ( DrawPos[0].x < DrawPos[1].x + 0.01f ) ) ||
							    ( ( DrawPos[1].x >= DrawPos[0].x - 0.01f ) && ( DrawPos[1].x < DrawPos[0].x + 0.01f ) ) ) &&
							    ( f < 0.001f || f > 0.999f ) ) )
						{
							DrawPos[0].x -= 0.5f;
							DrawPos[1].x -= 0.5f;
							DrawPos[2].x -= 0.5f;
							DrawPos[3].x -= 0.5f;
						}

						f = _FABS( DrawPos[0].y - _FTOL( DrawPos[0].y ) );
						if( ( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[2].y) ) ||
							    ( ( DrawPos[0].y >= DrawPos[2].y - 0.01f ) && ( DrawPos[0].y < DrawPos[2].y + 0.01f ) ) ||
							    ( ( DrawPos[2].y >= DrawPos[0].y - 0.01f ) && ( DrawPos[2].y < DrawPos[0].y + 0.01f ) ) ) &&
							    ( f < 0.001f || f > 0.999f ) ) ||
							( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[1].y) ) ||
							    ( ( DrawPos[0].y >= DrawPos[1].y - 0.01f ) && ( DrawPos[0].y < DrawPos[1].y + 0.01f ) ) ||
							    ( ( DrawPos[1].y >= DrawPos[0].y - 0.01f ) && ( DrawPos[1].y < DrawPos[0].y + 0.01f ) ) ) &&
							    ( f < 0.001f || f > 0.999f ) ) )
						{
							DrawPos[0].y -= 0.5f;
							DrawPos[1].y -= 0.5f;
							DrawPos[2].y -= 0.5f;
							DrawPos[3].y -= 0.5f;
						}

						DrawVert[0].pos.x = DrawPos[0].x ;
						DrawVert[0].pos.y = DrawPos[0].y ;
						DrawVert[1].pos.x = DrawPos[1].x ;
						DrawVert[1].pos.y = DrawPos[1].y ;
						DrawVert[2].pos.x = DrawPos[2].x ;
						DrawVert[2].pos.y = DrawPos[2].y ;
						DrawVert[3].pos.x = DrawPos[3].x ;
						DrawVert[3].pos.y = DrawPos[3].y ;
						DrawVert[4].pos.x = DrawPos[2].x ;
						DrawVert[4].pos.y = DrawPos[2].y ;
						DrawVert[5].pos.x = DrawPos[1].x ;
						DrawVert[5].pos.y = DrawPos[1].y ;
					}
					else
					if( AngleCheck )
					{
						float f;

						DrawPos[0].x = XBuf[1] + xf ;
						DrawPos[0].y = YBuf[1] + yf ;
						DrawPos[1].x = XBuf[3] + xf ;
						DrawPos[1].y = YBuf[3] + yf ;
						DrawPos[2].x = XBuf[5] + xf ;
						DrawPos[2].y = YBuf[5] + yf ;
						DrawPos[3].x = XBuf[7] + xf ;
						DrawPos[3].y = YBuf[7] + yf ;

						if( hx )
						{
							f = _FABS( DrawPos[0].x - _FTOL( DrawPos[0].x ) );
							if( ( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[2].x) ) ||
									( ( DrawPos[0].x >= DrawPos[2].x - 0.01f ) && ( DrawPos[0].x < DrawPos[2].x + 0.01f ) ) ||
									( ( DrawPos[2].x >= DrawPos[0].x - 0.01f ) && ( DrawPos[2].x < DrawPos[0].x + 0.01f ) ) ) &&
									( f < 0.001f || f > 0.999f ) ) ||
								( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[1].x) ) ||
									( ( DrawPos[0].x >= DrawPos[1].x - 0.01f ) && ( DrawPos[0].x < DrawPos[1].x + 0.01f ) ) ||
									( ( DrawPos[1].x >= DrawPos[0].x - 0.01f ) && ( DrawPos[1].x < DrawPos[0].x + 0.01f ) ) ) &&
									( f < 0.001f || f > 0.999f ) ) )
							{
								DrawPos[0].x -= 0.5f;
								DrawPos[1].x -= 0.5f;
								DrawPos[2].x -= 0.5f;
								DrawPos[3].x -= 0.5f;
							}
						}

						if( hy )
						{
							f = _FABS( DrawPos[0].y - _FTOL( DrawPos[0].y ) );
							if( ( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[2].y) ) ||
									( ( DrawPos[0].y >= DrawPos[2].y - 0.01f ) && ( DrawPos[0].y < DrawPos[2].y + 0.01f ) ) ||
									( ( DrawPos[2].y >= DrawPos[0].y - 0.01f ) && ( DrawPos[2].y < DrawPos[0].y + 0.01f ) ) ) &&
									( f < 0.001f || f > 0.999f ) ) ||
								( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[1].y) ) ||
									( ( DrawPos[0].y >= DrawPos[1].y - 0.01f ) && ( DrawPos[0].y < DrawPos[1].y + 0.01f ) ) ||
									( ( DrawPos[1].y >= DrawPos[0].y - 0.01f ) && ( DrawPos[1].y < DrawPos[0].y + 0.01f ) ) ) &&
									( f < 0.001f || f > 0.999f ) ) )
							{
								DrawPos[0].y -= 0.5f;
								DrawPos[1].y -= 0.5f;
								DrawPos[2].y -= 0.5f;
								DrawPos[3].y -= 0.5f;
							}
						}

						DrawVert[0].pos.x = DrawPos[0].x ;
						DrawVert[0].pos.y = DrawPos[0].y ;
						DrawVert[1].pos.x = DrawPos[1].x ;
						DrawVert[1].pos.y = DrawPos[1].y ;
						DrawVert[2].pos.x = DrawPos[2].x ;
						DrawVert[2].pos.y = DrawPos[2].y ;
						DrawVert[3].pos.x = DrawPos[3].x ;
						DrawVert[3].pos.y = DrawPos[3].y ;
						DrawVert[4].pos.x = DrawPos[2].x ;
						DrawVert[4].pos.y = DrawPos[2].y ;
						DrawVert[5].pos.x = DrawPos[1].x ;
						DrawVert[5].pos.y = DrawPos[1].y ;
					}
					else
					{
						DrawVert[0].pos.x = XBuf[1] + xf ;
						DrawVert[0].pos.y = YBuf[1] + yf ;
						DrawVert[1].pos.x = XBuf[3] + xf ;
						DrawVert[1].pos.y = YBuf[3] + yf ;
						DrawVert[2].pos.x = XBuf[5] + xf ;
						DrawVert[2].pos.y = YBuf[5] + yf ;
						DrawVert[3].pos.x = XBuf[7] + xf ;
						DrawVert[3].pos.y = YBuf[7] + yf ;
						DrawVert[4].pos.x = XBuf[5] + xf ;
						DrawVert[4].pos.y = YBuf[5] + yf ;
						DrawVert[5].pos.x = XBuf[3] + xf ;
						DrawVert[5].pos.y = YBuf[3] + yf ;
					}

					*((DWORD *)&DrawVert[0].pos.z) = drawz ;
					*((DWORD *)&DrawVert[1].pos.z) = drawz ;
					*((DWORD *)&DrawVert[2].pos.z) = drawz ;
					*((DWORD *)&DrawVert[3].pos.z) = drawz ;
					*((DWORD *)&DrawVert[4].pos.z) = drawz ;
					*((DWORD *)&DrawVert[5].pos.z) = drawz ;

					DrawVert[0].rhw = 1.0f ;
					DrawVert[1].rhw = 1.0f ;
					DrawVert[2].rhw = 1.0f ;
					DrawVert[3].rhw = 1.0f ;
					DrawVert[4].rhw = 1.0f ;
					DrawVert[5].rhw = 1.0f ;

					// テクスチャーを描画する
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// 描画するテクスチャーのセット
					DrawTex ++ ;
					Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}
			}
			else
			{
				// テクスチャーの数だけ繰り返す
				i = 0 ;
				for(;;)
				{
					// 頂点データを初期化する
					GETVERTEX_QUAD( DrawVert )
					
					TexVert = DrawTex->Vertex ;

					DrawVert[0].color = ( DWORD )DiffuseColor ;
					DrawVert[1].color = ( DWORD )DiffuseColor ;
					DrawVert[2].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;
					DrawVert[4].color = ( DWORD )DiffuseColor ;
					DrawVert[5].color = ( DWORD )DiffuseColor ;

					DrawVert[0].u = TexVert[0].u ;
					DrawVert[0].v = TexVert[0].v ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[1].v = TexVert[0].v ;
					DrawVert[2].v = TexVert[2].v ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[3].v = TexVert[2].v ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[4].v = TexVert[2].v ;
					DrawVert[5].u = TexVert[1].u ;
					DrawVert[5].v = TexVert[0].v ;

					XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX + xf ;
					YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY + yf ;
					XBuf[1] = ( TexVert[1].x - CenX ) * ExtendRateX + xf ;
					YBuf[1] = ( TexVert[1].y - CenY ) * ExtendRateY + yf ;
					XBuf[2] = ( TexVert[2].x - CenX ) * ExtendRateX + xf ;
					YBuf[2] = ( TexVert[2].y - CenY ) * ExtendRateY + yf ;
					XBuf[3] = ( TexVert[3].x - CenX ) * ExtendRateX + xf ;
					YBuf[3] = ( TexVert[3].y - CenY ) * ExtendRateY + yf ;

					DrawVert[0].pos.x = XBuf[0] ;
					DrawVert[0].pos.y = YBuf[0] ;
					DrawVert[1].pos.x = XBuf[1] ;
					DrawVert[1].pos.y = YBuf[1] ;
					DrawVert[2].pos.x = XBuf[2] ;
					DrawVert[2].pos.y = YBuf[2] ;
					DrawVert[3].pos.x = XBuf[3] ;
					DrawVert[3].pos.y = YBuf[3] ;
					DrawVert[4].pos.x = XBuf[2] ;
					DrawVert[4].pos.y = YBuf[2] ;
					DrawVert[5].pos.x = XBuf[1] ;
					DrawVert[5].pos.y = YBuf[1] ;

					*((DWORD *)&DrawVert[0].pos.z) = drawz ;
					*((DWORD *)&DrawVert[1].pos.z) = drawz ;
					*((DWORD *)&DrawVert[2].pos.z) = drawz ;
					*((DWORD *)&DrawVert[3].pos.z) = drawz ;
					*((DWORD *)&DrawVert[4].pos.z) = drawz ;
					*((DWORD *)&DrawVert[5].pos.z) = drawz ;

					DrawVert[0].rhw = 1.0f ;
					DrawVert[1].rhw = 1.0f ;
					DrawVert[2].rhw = 1.0f ;
					DrawVert[3].rhw = 1.0f ;
					DrawVert[4].rhw = 1.0f ;
					DrawVert[5].rhw = 1.0f ;

					// テクスチャーを描画する
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// 描画するテクスチャーのセット
					DrawTex ++ ;
					Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}
			}
		}
		break ;
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawRotaGraphFast
extern	int		Graphics_Hardware_DrawRotaGraphFast_PF( int x, int y, float xf, float yf, float ExtendRate, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int ReverseXFlag, int ReverseYFlag, int IntFlag )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	int DrawTexNum ;
	int i ; 
	float Sin = 0.0f, Cos = 1.0f ;
	int BlendGraphNoIncFlag ;
	float XBuf[ 8 ] ;
	float YBuf[ 8 ] ;
	float u[ 4 ] ;
	float v[ 4 ] ;
	float ExtendRateX ;
	float ExtendRateY ;
	DWORD drawz ;
//	BOOL Adj = FALSE ;
	float CenX, CenY ;
	int SinCosNone ;
	int Flag ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 幅がなかった場合はエラー
	if( ExtendRate == 0.0f ) return -1 ;

	// 描画の準備
	Flag = TransFlag ;
	DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )

	// 回転要素が無いか調べる
	if( Angle == 0.0f )
	{
		SinCosNone = 1;
	}
	else
	{
		SinCosNone = 0;
		_TABLE_SINCOS( (float)Angle, &Sin, &Cos ) ;
	}

	// 中心座標の算出
	CenX = ( float )Image->WidthF * 0.5F ;
	CenY = ( float )Image->HeightF * 0.5F ;

	// 頂点データを取得（今のところ色情報のセットのみ）
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// 描画情報の数をセット
	DrawTexNum = Image->Hard.DrawNum ;

	// 描画情報の数がブレンド画像と異なっていたら０番目のテクスチャだけを使用する
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// 描画情報配列のアドレスをセットしておく
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 反転を考慮した拡大率をセット
	ExtendRateX = ( ( ReverseXFlag == TRUE ) ? -1.0f : 1.0f ) * ExtendRate ;
	ExtendRateY = ( ( ReverseYFlag == TRUE ) ? -1.0f : 1.0f ) * ExtendRate ;

	// 頂点タイプによって処理を分岐
	switch( GLINUX.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// ブレンドテクスチャを使用する
		// テクスチャーの数だけ繰り返す
		if( DrawTexNum == 1 )
		{
			// 描画するテクスチャーのセット
			Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
			Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

			if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_Linux_DeviceState_NormalDrawSetup() ;
			}

			// 頂点データを初期化する
			GETVERTEX_QUAD( DrawVertB )

			TexVert = DrawTex->Vertex ;
			BlendTexVert = BlendDrawTex->Vertex ;

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;
			DrawVertB[4].color = ( DWORD )DiffuseColor ;
			DrawVertB[5].color = ( DWORD )DiffuseColor ;

			XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
			YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate ;
			XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
			YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

			XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
			YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRateY ;
			XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
			YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

			DrawVertB[0].pos.x =  XBuf[1] + xf ;
			DrawVertB[0].pos.y =  YBuf[1] + yf ;
			DrawVertB[1].pos.x =  XBuf[3] + xf ;
			DrawVertB[1].pos.y =  YBuf[3] + yf ;
			DrawVertB[2].pos.x = -XBuf[3] + xf ;
			DrawVertB[2].pos.y = -YBuf[3] + yf ;
			DrawVertB[3].pos.x = -XBuf[1] + xf ;
			DrawVertB[3].pos.y = -YBuf[1] + yf ;
			DrawVertB[4].pos.x = -XBuf[3] + xf ;
			DrawVertB[4].pos.y = -YBuf[3] + yf ;
			DrawVertB[5].pos.x =  XBuf[3] + xf ;
			DrawVertB[5].pos.y =  YBuf[3] + yf ;

			switch( GSYS.DrawSetting.BlendGraphPosMode )
			{
			default :
			case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
				u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[0].u1 = u[ 0 ] ;
				DrawVertB[0].v1 = v[ 0 ] ;
				DrawVertB[1].u1 = u[ 1 ] ;
				DrawVertB[1].v1 = v[ 0 ] ;
				DrawVertB[2].u1 = u[ 0 ] ;
				DrawVertB[2].v1 = v[ 1 ] ;
				DrawVertB[3].u1 = u[ 1 ] ;
				DrawVertB[3].v1 = v[ 1 ] ;
				DrawVertB[4].u1 = u[ 0 ] ;
				DrawVertB[4].v1 = v[ 1 ] ;
				DrawVertB[5].u1 = u[ 1 ] ;
				DrawVertB[5].v1 = v[ 0 ] ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				DrawVertB[0].u1 = ( (  XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[0].v1 = ( (  YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[1].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[1].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[2].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[2].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[3].u1 = ( ( -XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[3].v1 = ( ( -YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[4].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[4].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[5].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[5].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				break ;
			}

			DrawVertB[0].u2 = TexVert[0].u ;
			DrawVertB[0].v2 = TexVert[0].v ;
			DrawVertB[1].u2 = TexVert[1].u ;
			DrawVertB[1].v2 = TexVert[0].v ;
			DrawVertB[2].u2 = TexVert[0].u ;
			DrawVertB[2].v2 = TexVert[2].v ;
			DrawVertB[3].u2 = TexVert[1].u ;
			DrawVertB[3].v2 = TexVert[2].v ;
			DrawVertB[4].u2 = TexVert[0].u ;
			DrawVertB[4].v2 = TexVert[2].v ;
			DrawVertB[5].u2 = TexVert[1].u ;
			DrawVertB[5].v2 = TexVert[0].v ;

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;
			DrawVertB[3].rhw = 1.0f ;
			DrawVertB[4].rhw = 1.0f ;
			DrawVertB[5].rhw = 1.0f ;

			// テクスチャーを描画する
			ADD4VERTEX_BLENDTEX

			if( BlendGraphNoIncFlag == FALSE )
			{
				BlendDrawTex ++ ;
			}
		}
		else
		{
			// テクスチャーの数だけ繰り返す
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// 描画するテクスチャーのセット
				Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
				Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

				if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}

				// 頂点データを初期化する
				GETVERTEX_QUAD( DrawVertB )

				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY ;
				XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
				YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

				XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRateY ;
				XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
				YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

				XBuf[4] = ( TexVert[2].x - CenX ) * ExtendRateX ;
				YBuf[4] = ( TexVert[2].y - CenY ) * ExtendRateY ;
				XBuf[5] = XBuf[4] * Cos - YBuf[4] * Sin ;
				YBuf[5] = XBuf[4] * Sin + YBuf[4] * Cos ;

				XBuf[6] = ( TexVert[3].x - CenX ) * ExtendRateX ;
				YBuf[6] = ( TexVert[3].y - CenY ) * ExtendRateY ;
				XBuf[7] = XBuf[6] * Cos - YBuf[6] * Sin ;
				YBuf[7] = XBuf[6] * Sin + YBuf[6] * Cos ;

				DrawVertB[0].pos.x = XBuf[1] + xf ;
				DrawVertB[0].pos.y = YBuf[1] + yf ;
				DrawVertB[1].pos.x = XBuf[3] + xf ;
				DrawVertB[1].pos.y = YBuf[3] + yf ;
				DrawVertB[2].pos.x = XBuf[5] + xf ;
				DrawVertB[2].pos.y = YBuf[5] + yf ;
				DrawVertB[3].pos.x = XBuf[7] + xf ;
				DrawVertB[3].pos.y = YBuf[7] + yf ;
				DrawVertB[4].pos.x = XBuf[5] + xf ;
				DrawVertB[4].pos.y = YBuf[5] + yf ;
				DrawVertB[5].pos.x = XBuf[3] + xf ;
				DrawVertB[5].pos.y = YBuf[3] + yf ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 2 ] = ( TexVert[2].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 3 ] = ( TexVert[3].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[1].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 2 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 3 ] = ( TexVert[3].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;

					DrawVertB[0].u1 = u[ 0 ] ;
					DrawVertB[0].v1 = v[ 0 ] ;
					DrawVertB[1].u1 = u[ 1 ] ;
					DrawVertB[1].v1 = v[ 1 ] ;
					DrawVertB[2].u1 = u[ 2 ] ;
					DrawVertB[2].v1 = v[ 2 ] ;
					DrawVertB[3].u1 = u[ 3 ] ;
					DrawVertB[3].v1 = v[ 3 ] ;
					DrawVertB[4].u1 = u[ 2 ] ;
					DrawVertB[4].v1 = v[ 2 ] ;
					DrawVertB[5].u1 = u[ 1 ] ;
					DrawVertB[5].v1 = v[ 1 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[0].u1 = ( ( XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( ( YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( ( XBuf[7] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( ( YBuf[7] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
		}
		break ;

	case VERTEXTYPE_TEX :	// ブレンドテクスチャを使用しない
		// テクスチャーの数だけ繰り返す
		if( DrawTexNum == 1 )
		{
			if( SinCosNone == 0 )
			{
				// 頂点データを初期化する
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY ;
				XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
				YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;
				
				XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRateY ;
				XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
				YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

				DrawVert[0].pos.x =  XBuf[1] + xf ;
				DrawVert[0].pos.y =  YBuf[1] + yf ;
				DrawVert[1].pos.x =  XBuf[3] + xf ;
				DrawVert[1].pos.y =  YBuf[3] + yf ;
				DrawVert[2].pos.x = -XBuf[3] + xf ;
				DrawVert[2].pos.y = -YBuf[3] + yf ;
				DrawVert[3].pos.x = -XBuf[1] + xf ;
				DrawVert[3].pos.y = -YBuf[1] + yf ;
				DrawVert[4].pos.x = -XBuf[3] + xf ;
				DrawVert[4].pos.y = -YBuf[3] + yf ;
				DrawVert[5].pos.x =  XBuf[3] + xf ;
				DrawVert[5].pos.y =  YBuf[3] + yf ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_TEX
			}
			else
			{
				// 頂点データを初期化する
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY ;

				XBuf[1] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[1] = ( TexVert[1].y - CenY ) * ExtendRateY ;

				DrawVert[0].pos.x =  XBuf[0] + xf ;
				DrawVert[0].pos.y =  YBuf[0] + yf ;
				DrawVert[1].pos.x =  XBuf[1] + xf ;
				DrawVert[1].pos.y =  YBuf[1] + yf ;
				DrawVert[2].pos.x = -XBuf[1] + xf ;
				DrawVert[2].pos.y = -YBuf[1] + yf ;
				DrawVert[3].pos.x = -XBuf[0] + xf ;
				DrawVert[3].pos.y = -YBuf[0] + yf ;
				DrawVert[4].pos.x = -XBuf[1] + xf ;
				DrawVert[4].pos.y = -YBuf[1] + yf ;
				DrawVert[5].pos.x =  XBuf[1] + xf ;
				DrawVert[5].pos.y =  YBuf[1] + yf ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// テクスチャーを描画する
				ADD4VERTEX_TEX
			}
		}
		else
		{
			if( SinCosNone == 0 )
			{
				// テクスチャーの数だけ繰り返す
				i = 0 ;
				for(;;)
				{
					// 頂点データを初期化する
					GETVERTEX_QUAD( DrawVert )
					
					TexVert = DrawTex->Vertex ;

					DrawVert[0].color = ( DWORD )DiffuseColor ;
					DrawVert[1].color = ( DWORD )DiffuseColor ;
					DrawVert[2].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;
					DrawVert[4].color = ( DWORD )DiffuseColor ;
					DrawVert[5].color = ( DWORD )DiffuseColor ;

					DrawVert[0].u = TexVert[0].u ;
					DrawVert[0].v = TexVert[0].v ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[1].v = TexVert[0].v ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[2].v = TexVert[2].v ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[3].v = TexVert[2].v ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[4].v = TexVert[2].v ;
					DrawVert[5].u = TexVert[1].u ;
					DrawVert[5].v = TexVert[0].v ;

					XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
					YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY ;
					XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
					YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

					XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
					YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRateY ;
					XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
					YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

					XBuf[4] = ( TexVert[2].x - CenX ) * ExtendRateX ;
					YBuf[4] = ( TexVert[2].y - CenY ) * ExtendRateY ;
					XBuf[5] = XBuf[4] * Cos - YBuf[4] * Sin ;
					YBuf[5] = XBuf[4] * Sin + YBuf[4] * Cos ;

					XBuf[6] = ( TexVert[3].x - CenX ) * ExtendRateX ;
					YBuf[6] = ( TexVert[3].y - CenY ) * ExtendRateY ;
					XBuf[7] = XBuf[6] * Cos - YBuf[6] * Sin ;
					YBuf[7] = XBuf[6] * Sin + YBuf[6] * Cos ;

					DrawVert[0].pos.x = XBuf[1] + xf ;
					DrawVert[0].pos.y = YBuf[1] + yf ;
					DrawVert[1].pos.x = XBuf[3] + xf ;
					DrawVert[1].pos.y = YBuf[3] + yf ;
					DrawVert[2].pos.x = XBuf[5] + xf ;
					DrawVert[2].pos.y = YBuf[5] + yf ;
					DrawVert[3].pos.x = XBuf[7] + xf ;
					DrawVert[3].pos.y = YBuf[7] + yf ;
					DrawVert[4].pos.x = XBuf[5] + xf ;
					DrawVert[4].pos.y = YBuf[5] + yf ;
					DrawVert[5].pos.x = XBuf[3] + xf ;
					DrawVert[5].pos.y = YBuf[3] + yf ;

					*((DWORD *)&DrawVert[0].pos.z) = drawz ;
					*((DWORD *)&DrawVert[1].pos.z) = drawz ;
					*((DWORD *)&DrawVert[2].pos.z) = drawz ;
					*((DWORD *)&DrawVert[3].pos.z) = drawz ;
					*((DWORD *)&DrawVert[4].pos.z) = drawz ;
					*((DWORD *)&DrawVert[5].pos.z) = drawz ;

					DrawVert[0].rhw = 1.0f ;
					DrawVert[1].rhw = 1.0f ;
					DrawVert[2].rhw = 1.0f ;
					DrawVert[3].rhw = 1.0f ;
					DrawVert[4].rhw = 1.0f ;
					DrawVert[5].rhw = 1.0f ;

					// テクスチャーを描画する
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// 描画するテクスチャーのセット
					DrawTex ++ ;
					Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}
			}
			else
			{
				// テクスチャーの数だけ繰り返す
				i = 0 ;
				for(;;)
				{
					// 頂点データを初期化する
					GETVERTEX_QUAD( DrawVert )
					
					TexVert = DrawTex->Vertex ;

					DrawVert[0].color = ( DWORD )DiffuseColor ;
					DrawVert[1].color = ( DWORD )DiffuseColor ;
					DrawVert[2].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;
					DrawVert[4].color = ( DWORD )DiffuseColor ;
					DrawVert[5].color = ( DWORD )DiffuseColor ;

					DrawVert[0].u = TexVert[0].u ;
					DrawVert[0].v = TexVert[0].v ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[1].v = TexVert[0].v ;
					DrawVert[2].v = TexVert[2].v ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[3].v = TexVert[2].v ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[4].v = TexVert[2].v ;
					DrawVert[5].u = TexVert[1].u ;
					DrawVert[5].v = TexVert[0].v ;

					XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX + xf ;
					YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRateY + yf ;
					XBuf[1] = ( TexVert[1].x - CenX ) * ExtendRateX + xf ;
					YBuf[1] = ( TexVert[1].y - CenY ) * ExtendRateY + yf ;
					XBuf[2] = ( TexVert[2].x - CenX ) * ExtendRateX + xf ;
					YBuf[2] = ( TexVert[2].y - CenY ) * ExtendRateY + yf ;
					XBuf[3] = ( TexVert[3].x - CenX ) * ExtendRateX + xf ;
					YBuf[3] = ( TexVert[3].y - CenY ) * ExtendRateY + yf ;

					DrawVert[0].pos.x = XBuf[0] ;
					DrawVert[0].pos.y = YBuf[0] ;
					DrawVert[1].pos.x = XBuf[1] ;
					DrawVert[1].pos.y = YBuf[1] ;
					DrawVert[2].pos.x = XBuf[2] ;
					DrawVert[2].pos.y = YBuf[2] ;
					DrawVert[3].pos.x = XBuf[3] ;
					DrawVert[3].pos.y = YBuf[3] ;
					DrawVert[4].pos.x = XBuf[2] ;
					DrawVert[4].pos.y = YBuf[2] ;
					DrawVert[5].pos.x = XBuf[1] ;
					DrawVert[5].pos.y = YBuf[1] ;

					*((DWORD *)&DrawVert[0].pos.z) = drawz ;
					*((DWORD *)&DrawVert[1].pos.z) = drawz ;
					*((DWORD *)&DrawVert[2].pos.z) = drawz ;
					*((DWORD *)&DrawVert[3].pos.z) = drawz ;
					*((DWORD *)&DrawVert[4].pos.z) = drawz ;
					*((DWORD *)&DrawVert[5].pos.z) = drawz ;

					DrawVert[0].rhw = 1.0f ;
					DrawVert[1].rhw = 1.0f ;
					DrawVert[2].rhw = 1.0f ;
					DrawVert[3].rhw = 1.0f ;
					DrawVert[4].rhw = 1.0f ;
					DrawVert[5].rhw = 1.0f ;

					// テクスチャーを描画する
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// 描画するテクスチャーのセット
					DrawTex ++ ;
					Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
					Graphics_Linux_DeviceState_NormalDrawSetup() ;
				}
			}
		}
		break ;
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawModiGraph
extern	int		Graphics_Hardware_DrawModiGraph_PF( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag )
{
	if( x1 == x3 && x2 == x4 && y1 == y2 && y3 == y4 ) return Graphics_Hardware_DrawExtendGraph_PF( x1, y1, x4, y4, ( float )x1, ( float )y1, ( float )x4, ( float )y4, Image, BlendImage, TransFlag, TRUE ) ; 

	// もしブレンドテクスチャを使用せず、旧型関数を使用するフラグも立っていなければ
	// 内部分割を行う描画関数を使用する
	if( GLINUX.Setting.UseOldDrawModiGraphCodeFlag == FALSE && GSYS.DrawSetting.BlendGraph <= 0 )
		return Graphics_Hardware_DrawModiGraphF_PF( (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3, (float)x4, (float)y4, Image, BlendImage, TransFlag, SimpleDrawFlag ) ;

	VERTEX_2D           *DrawVert ;
	VERTEX_BLENDTEX_2D	*DrawVertB ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	float GraphSizeX, GraphSizeY ;
	float XBuf, YBuf ;
	int DrawTexNum ;
	unsigned char *VertType ;
	int i, j ; 
	int BlendGraphNoIncFlag ;
	float Reverse0 ;
	float ExRate1X1, ExRate1Y1, ExRate2X1, ExRate2Y1 ;
	float ExRate1X2, ExRate1Y2, ExRate2X2, ExRate2Y2 ;
	DWORD DiffuseColor ;
	VECTOR DrawPos[ 4 ] ;
	float u[ 4 ] ;
	float v[ 4 ] ;
	float  xf1, yf1, xf4, yf4;
	DWORD drawz ;
	int Flag ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Flag = TransFlag | DX_LINUX_DRAWPREP_VECTORINT ;
	DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )

	GraphSizeX = ( float )Image->WidthF ;  GraphSizeY = ( float )Image->HeightF ;
	ExRate1X1 = ( x2 - x1 ) /  GraphSizeX ;  ExRate1X2 = ( x3 - x1 ) /  GraphSizeY ;
	ExRate1Y1 = ( y3 - y1 ) /  GraphSizeY ;  ExRate1Y2 = ( y2 - y1 ) /  GraphSizeX ;
	ExRate2X1 = ( x3 - x4 ) / -GraphSizeX ;  ExRate2X2 = ( x2 - x4 ) / -GraphSizeY ;
	ExRate2Y1 = ( y2 - y4 ) / -GraphSizeY ;  ExRate2Y2 = ( y3 - y4 ) / -GraphSizeX ;
//	xf1 = (float)x1 - 0.5F ; yf1 = (float)y1 - 0.5F ;
//	xf4 = (float)x4 - 0.5F ; yf4 = (float)y4 - 0.5F ; 
	xf1 = (float)x1 ; yf1 = (float)y1 ;
	xf4 = (float)x4 ; yf4 = (float)y4 ; 

	// 頂点データを取得（今のところ色情報のセットのみ）
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 描画情報の数をセット
	DrawTexNum = Image->Hard.DrawNum ;

	// 描画情報の数がブレンド画像と異なっていたら０番目のテクスチャだけを使用する
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// 描画情報配列のアドレスをセットしておく
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// 頂点タイプによって処理を分岐
	switch( GLINUX.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// ブレンドテクスチャを使用する
		// テクスチャーの数だけ繰り返す
		for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
		{
			// 描画するテクスチャーのセット
			Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
			Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

			Graphics_Linux_DeviceState_NormalDrawSetup() ;

			// 頂点バッファの取得
			GETVERTEX_QUAD( DrawVertB )

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;
			DrawVertB[4].color = ( DWORD )DiffuseColor ;
			DrawVertB[5].color = ( DWORD )DiffuseColor ;

			VertType     = DrawTex->VertType ;
			TexVert      = DrawTex->Vertex ;
			BlendTexVert = BlendDrawTex->Vertex ;

			for( j = 0 ; j < 4 ; j ++, TexVert ++ )
			{
				// 頂点座標の算出
				if( *VertType ++ )
				{
					// 三角の上側
					DrawPos[ j ].x = ( TexVert->x * ExRate1X1 ) + ( TexVert->y * ExRate1X2 ) + xf1 ;
					DrawPos[ j ].y = ( TexVert->y * ExRate1Y1 ) + ( TexVert->x * ExRate1Y2 ) + yf1 ;
				} 
				else
				{
					// 三角の下側
					XBuf = TexVert->x - GraphSizeX ;
					YBuf = TexVert->y - GraphSizeY ;
					DrawPos[ j ].x = ( XBuf * ExRate2X1 ) + ( YBuf * ExRate2X2 ) + xf4 ;
					DrawPos[ j ].y = ( YBuf * ExRate2Y1 ) + ( XBuf * ExRate2Y2 ) + yf4 ;
				}

				u[ j ] = ( TexVert->x - BlendTexVert->x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ j ] = ( TexVert->y - BlendTexVert->y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
			}

			// 反転判定処理
			Reverse0 =  ( DrawPos[ 1 ].x - DrawPos[ 0 ].x ) * ( DrawPos[ 2 ].y - DrawPos[ 0 ].y ) -
						( DrawPos[ 2 ].x - DrawPos[ 0 ].x ) * ( DrawPos[ 1 ].y - DrawPos[ 0 ].y ) ;
//			Reverse1 = -( DrawPos[ 1 ].x - DrawPos[ 3 ].x ) * ( DrawPos[ 2 ].y - DrawPos[ 3 ].y ) +
//						( DrawPos[ 1 ].y - DrawPos[ 3 ].y ) * ( DrawPos[ 2 ].x - DrawPos[ 3 ].x ) ;
			TexVert = DrawTex->Vertex ;
			if( Reverse0 <= 0 )
			{
				DrawVertB[ 0 ].pos.x = DrawPos[ 0 ].x ;
				DrawVertB[ 0 ].pos.y = DrawPos[ 0 ].y ;
				DrawVertB[ 1 ].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[ 1 ].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[ 2 ].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[ 2 ].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[ 3 ].pos.x = DrawPos[ 3 ].x ;
				DrawVertB[ 3 ].pos.y = DrawPos[ 3 ].y ;
				DrawVertB[ 4 ].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[ 4 ].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[ 5 ].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[ 5 ].pos.y = DrawPos[ 2 ].y ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					DrawVertB[ 0 ].u1 = u[ 0 ] ;
					DrawVertB[ 0 ].v1 = v[ 0 ] ;
					DrawVertB[ 1 ].u1 = u[ 2 ] ;
					DrawVertB[ 1 ].v1 = v[ 2 ] ;
					DrawVertB[ 2 ].u1 = u[ 1 ] ;
					DrawVertB[ 2 ].v1 = v[ 1 ] ;
					DrawVertB[ 3 ].u1 = u[ 3 ] ;
					DrawVertB[ 3 ].v1 = v[ 3 ] ;
					DrawVertB[ 4 ].u1 = u[ 1 ] ;
					DrawVertB[ 4 ].v1 = v[ 1 ] ;
					DrawVertB[ 5 ].u1 = u[ 2 ] ;
					DrawVertB[ 5 ].v1 = v[ 2 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[ 0 ].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 0 ].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 1 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 1 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 2 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 2 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 3 ].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 3 ].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 4 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 4 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 5 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 5 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[ 0 ].u2 = TexVert[ 0 ].u ;
				DrawVertB[ 0 ].v2 = TexVert[ 0 ].v ;
				DrawVertB[ 1 ].u2 = TexVert[ 2 ].u ;
				DrawVertB[ 1 ].v2 = TexVert[ 2 ].v ;
				DrawVertB[ 2 ].u2 = TexVert[ 1 ].u ;
				DrawVertB[ 2 ].v2 = TexVert[ 1 ].v ;
				DrawVertB[ 3 ].u2 = TexVert[ 3 ].u ;
				DrawVertB[ 3 ].v2 = TexVert[ 3 ].v ;
				DrawVertB[ 4 ].u2 = TexVert[ 1 ].u ;
				DrawVertB[ 4 ].v2 = TexVert[ 1 ].v ;
				DrawVertB[ 5 ].u2 = TexVert[ 2 ].u ;
				DrawVertB[ 5 ].v2 = TexVert[ 2 ].v ;
			}
			else
			{
				DrawVertB[ 0 ].pos.x = DrawPos[ 0 ].x ;
				DrawVertB[ 0 ].pos.y = DrawPos[ 0 ].y ;
				DrawVertB[ 1 ].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[ 1 ].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[ 2 ].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[ 2 ].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[ 3 ].pos.x = DrawPos[ 3 ].x ;
				DrawVertB[ 3 ].pos.y = DrawPos[ 3 ].y ;
				DrawVertB[ 4 ].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[ 4 ].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[ 5 ].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[ 5 ].pos.y = DrawPos[ 1 ].y ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					DrawVertB[ 0 ].u1 = u[ 0 ] ;
					DrawVertB[ 0 ].v1 = v[ 0 ] ;
					DrawVertB[ 1 ].u1 = u[ 1 ] ;
					DrawVertB[ 1 ].v1 = v[ 1 ] ;
					DrawVertB[ 2 ].u1 = u[ 2 ] ;
					DrawVertB[ 2 ].v1 = v[ 2 ] ;
					DrawVertB[ 3 ].u1 = u[ 3 ] ;
					DrawVertB[ 3 ].v1 = v[ 3 ] ;
					DrawVertB[ 4 ].u1 = u[ 2 ] ;
					DrawVertB[ 4 ].v1 = v[ 2 ] ;
					DrawVertB[ 5 ].u1 = u[ 1 ] ;
					DrawVertB[ 5 ].v1 = v[ 1 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[ 0 ].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 0 ].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 1 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 1 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 2 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 2 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 3 ].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 3 ].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 4 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 4 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 5 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 5 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[ 0 ].u2 = TexVert[ 0 ].u ;
				DrawVertB[ 0 ].v2 = TexVert[ 0 ].v ;
				DrawVertB[ 1 ].u2 = TexVert[ 1 ].u ;
				DrawVertB[ 1 ].v2 = TexVert[ 1 ].v ;
				DrawVertB[ 2 ].u2 = TexVert[ 2 ].u ;
				DrawVertB[ 2 ].v2 = TexVert[ 2 ].v ;
				DrawVertB[ 3 ].u2 = TexVert[ 3 ].u ;
				DrawVertB[ 3 ].v2 = TexVert[ 3 ].v ;
				DrawVertB[ 4 ].u2 = TexVert[ 2 ].u ;
				DrawVertB[ 4 ].v2 = TexVert[ 2 ].v ;
				DrawVertB[ 5 ].u2 = TexVert[ 1 ].u ;
				DrawVertB[ 5 ].v2 = TexVert[ 1 ].v ;
			}

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;
			DrawVertB[3].rhw = 1.0f ;
			DrawVertB[4].rhw = 1.0f ;
			DrawVertB[5].rhw = 1.0f ;

			// テクスチャーを描画する
			ADD4VERTEX_BLENDTEX

			if( BlendGraphNoIncFlag == FALSE )
			{
				BlendDrawTex ++ ;
			}
		}
		break ;

	case VERTEXTYPE_TEX :	// ブレンドテクスチャを使用しない
		// テクスチャーの数だけ繰り返す
		i = 0 ;
		for(;;)
		{
			// 頂点バッファの取得
			GETVERTEX_QUAD( DrawVert )

			// 頂点データをセットする
			DrawVert[0].color = ( DWORD )DiffuseColor ;
			DrawVert[1].color = ( DWORD )DiffuseColor ;
			DrawVert[2].color = ( DWORD )DiffuseColor ;
			DrawVert[3].color = ( DWORD )DiffuseColor ;
			DrawVert[4].color = ( DWORD )DiffuseColor ;
			DrawVert[5].color = ( DWORD )DiffuseColor ;

			VertType = DrawTex->VertType ;
			TexVert  = DrawTex->Vertex ;

			for( j = 0 ; j < 4 ; j ++, TexVert ++ )
			{
				// 頂点座標の算出
				if( *VertType ++ )
				{
					// 三角の上側
					DrawPos[ j ].x = ( TexVert->x * ExRate1X1 ) + ( TexVert->y * ExRate1X2 ) + xf1 ;
					DrawPos[ j ].y = ( TexVert->y * ExRate1Y1 ) + ( TexVert->x * ExRate1Y2 ) + yf1 ;
				} 
				else
				{
					// 三角の下側
					XBuf = TexVert->x - GraphSizeX ;
					YBuf = TexVert->y - GraphSizeY ;
					DrawPos[ j ].x = ( XBuf * ExRate2X1 ) + ( YBuf * ExRate2X2 ) + xf4 ;
					DrawPos[ j ].y = ( YBuf * ExRate2Y1 ) + ( XBuf * ExRate2Y2 ) + yf4 ;
				}
			}

			// 反転判定処理
			Reverse0 =  ( DrawPos[ 1 ].x - DrawPos[ 0 ].x ) * ( DrawPos[ 2 ].y - DrawPos[ 0 ].y ) -
						( DrawPos[ 2 ].x - DrawPos[ 0 ].x ) * ( DrawPos[ 1 ].y - DrawPos[ 0 ].y ) ;
			TexVert = DrawTex->Vertex ;
			if( Reverse0 <= 0 )
			{
				DrawVert[ 0 ].pos.x = DrawPos[ 0 ].x ;
				DrawVert[ 0 ].pos.y = DrawPos[ 0 ].y ;
				DrawVert[ 1 ].pos.x = DrawPos[ 2 ].x ;
				DrawVert[ 1 ].pos.y = DrawPos[ 2 ].y ;
				DrawVert[ 2 ].pos.x = DrawPos[ 1 ].x ;
				DrawVert[ 2 ].pos.y = DrawPos[ 1 ].y ;
				DrawVert[ 3 ].pos.x = DrawPos[ 3 ].x ;
				DrawVert[ 3 ].pos.y = DrawPos[ 3 ].y ;
				DrawVert[ 4 ].pos.x = DrawPos[ 1 ].x ;
				DrawVert[ 4 ].pos.y = DrawPos[ 1 ].y ;
				DrawVert[ 5 ].pos.x = DrawPos[ 2 ].x ;
				DrawVert[ 5 ].pos.y = DrawPos[ 2 ].y ;

				DrawVert[ 0 ].u = TexVert[ 0 ].u ;
				DrawVert[ 0 ].v = TexVert[ 0 ].v ;
				DrawVert[ 1 ].u = TexVert[ 2 ].u ;
				DrawVert[ 1 ].v = TexVert[ 2 ].v ;
				DrawVert[ 2 ].u = TexVert[ 1 ].u ;
				DrawVert[ 2 ].v = TexVert[ 1 ].v ;
				DrawVert[ 3 ].u = TexVert[ 3 ].u ;
				DrawVert[ 3 ].v = TexVert[ 3 ].v ;
				DrawVert[ 4 ].u = TexVert[ 1 ].u ;
				DrawVert[ 4 ].v = TexVert[ 1 ].v ;
				DrawVert[ 5 ].u = TexVert[ 2 ].u ;
				DrawVert[ 5 ].v = TexVert[ 2 ].v ;
			}
			else
			{
				DrawVert[ 0 ].pos.x = DrawPos[ 0 ].x ;
				DrawVert[ 0 ].pos.y = DrawPos[ 0 ].y ;
				DrawVert[ 1 ].pos.x = DrawPos[ 1 ].x ;
				DrawVert[ 1 ].pos.y = DrawPos[ 1 ].y ;
				DrawVert[ 2 ].pos.x = DrawPos[ 2 ].x ;
				DrawVert[ 2 ].pos.y = DrawPos[ 2 ].y ;
				DrawVert[ 3 ].pos.x = DrawPos[ 3 ].x ;
				DrawVert[ 3 ].pos.y = DrawPos[ 3 ].y ;
				DrawVert[ 4 ].pos.x = DrawPos[ 2 ].x ;
				DrawVert[ 4 ].pos.y = DrawPos[ 2 ].y ;
				DrawVert[ 5 ].pos.x = DrawPos[ 1 ].x ;
				DrawVert[ 5 ].pos.y = DrawPos[ 1 ].y ;

				DrawVert[ 0 ].u = TexVert[ 0 ].u ;
				DrawVert[ 0 ].v = TexVert[ 0 ].v ;
				DrawVert[ 1 ].u = TexVert[ 1 ].u ;
				DrawVert[ 1 ].v = TexVert[ 1 ].v ;
				DrawVert[ 2 ].u = TexVert[ 2 ].u ;
				DrawVert[ 2 ].v = TexVert[ 2 ].v ;
				DrawVert[ 3 ].u = TexVert[ 3 ].u ;
				DrawVert[ 3 ].v = TexVert[ 3 ].v ;
				DrawVert[ 4 ].u = TexVert[ 2 ].u ;
				DrawVert[ 4 ].v = TexVert[ 2 ].v ;
				DrawVert[ 5 ].u = TexVert[ 1 ].u ;
				DrawVert[ 5 ].v = TexVert[ 1 ].v ;
			}

			*((DWORD *)&DrawVert[0].pos.z) = drawz ; 
			*((DWORD *)&DrawVert[1].pos.z) = drawz ; 
			*((DWORD *)&DrawVert[2].pos.z) = drawz ; 
			*((DWORD *)&DrawVert[3].pos.z) = drawz ;
			*((DWORD *)&DrawVert[4].pos.z) = drawz ;
			*((DWORD *)&DrawVert[5].pos.z) = drawz ;

			DrawVert[0].rhw = 1.0f ;
			DrawVert[1].rhw = 1.0f ;
			DrawVert[2].rhw = 1.0f ;
			DrawVert[3].rhw = 1.0f ;
			DrawVert[4].rhw = 1.0f ;
			DrawVert[5].rhw = 1.0f ;

			ADD4VERTEX_TEX

			i ++ ;
			if( i >= DrawTexNum ) break ;

			// 描画するテクスチャーのセット
			DrawTex ++ ;
			Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
			Graphics_Linux_DeviceState_NormalDrawSetup() ;
		}
		break ;
	}

	// 終了
	return 0 ;
}

// ２次元配列的に配置された頂点データを頂点バッファに追加する
static int LINUX_Graphics_SetPlaneVertexHardware( VERTEX_2D *GraphVert, int xnum, int ynum )
{
	int i, j, k, l ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( xnum < 2 || ynum < 2 ) return -1 ;

	// トライアングルリストモードの場合はバッファは使用しない
	DRAWSTOCKINFO

	// バッファを使用しない設定になっていたらこの場で描画
	{
#define MAX_POSNUM		1000
		WORD list[MAX_POSNUM] ;

		// 頂点インデックスリストを作成する
		k = 0 ;
		l = 0 ;
		for( i = 0 ; i < ynum - 1 ; i ++ )
		{
			// 頂点数が超えそうだったら描画
			if( k + xnum * 2 + 2 > MAX_POSNUM )
			{
				// Uniform の更新
				Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

				// 頂点データのセットアップ
				Graphics_Linux_DeviceState_SetupShaderVertexData(
					GLINUX.Device.State.SetShader,
					&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_2D ].InputInfo,
					GraphVert
				) ;

				// 描画
				glDrawElements( GL_TRIANGLE_STRIP, k, GL_UNSIGNED_SHORT, list );
				GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;
					
				k = 0 ;
			}

			// 前の行からの続きだった場合はストリップの連続用の頂点を出力		
			if( k != 0 )
			{
				list[k] = ( WORD )( l + xnum ) ;
				k ++ ;
			}
			
			for( j = 0 ; j < xnum ; j ++, l ++ )
			{
				list[k] = ( WORD )( l + xnum ) ;
				list[k+1] = ( WORD )l ;
				k += 2 ;
			}
			if( i != ynum - 2 )
			{
				list[k] = list[k-1] ;
				k ++ ;
			}
		}
		if( k != 0 )
		{
			// Uniform の更新
			Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

			// 頂点データのセットアップ
			Graphics_Linux_DeviceState_SetupShaderVertexData(
				GLINUX.Device.State.SetShader,
				&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_2D ].InputInfo,
				GraphVert
			) ;

			// 描画
			glDrawElements( GL_TRIANGLE_STRIP, k, GL_UNSIGNED_SHORT, list );
			GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;
		}
#undef	MAX_POSNUM
	}

	// 終了
	return 0 ;
}

// テクスチャを変形して描画する
extern	void	Graphics_Linux_DrawModiTex( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA_HARD_DRAW *DrawTex, bool SimpleDrawFlag )
{
	IMAGEDATA_HARD_VERT *TexVert ;
	VERTEX_2D *vec ; 
	float xx1, yy1, xx2, yy2, xx3, yy3, xx4, yy4/*, r1, r2, r3, r4*/ ;
//	float xtmp1, ytmp1, xtmp2, ytmp2 ;
	int c/*, f1, f2*/ ;
	DWORD DiffuseColor ;
	float Reverse0, Reverse1;
	DWORD drawz ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return ;
	}

	// 描画するテクスチャーのセット
	Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
	if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
	{
		Graphics_Linux_DeviceState_NormalDrawSetup() ;
	}

	// 頂点データを取得（今のところ色情報のセットのみ）
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	TexVert = DrawTex->Vertex ;

	// 反転判定用処理
	Reverse0 =  ( x2 - x1 ) * ( y3 - y1 ) - ( x3 - x1 ) * ( y2 - y1 ) ;
	Reverse1 = -( x2 - x4 ) * ( y3 - y4 ) + ( y2 - y4 ) * ( x3 - x4 ) ;

	if( SimpleDrawFlag ) goto R1 ;

	// 平行四辺形以上の変形が起きていなかったら普通の処理を実行
//	f1 = f2 = 0 ;
	c = 0 ;
	xx1 = x2 - x1 ; xx2 = x4 - x3 ;
	yy1 = y2 - y1 ; yy2 = y4 - y3 ;

	xx3 = x3 - x1 ; xx4 = x4 - x2 ;
	yy3 = y3 - y1 ; yy4 = y4 - y2 ;

	if( _FABS( xx1 - xx2 ) < 1.0F && _FABS( yy1 - yy2 ) < 1.0F ) c ++/*, f1 = 1*/ ;
	if( _FABS( xx3 - xx4 ) < 1.0F && _FABS( yy3 - yy4 ) < 1.0F ) c ++/*, f2 = 1*/ ;
	if( c == 2 ) goto R1 ;

/*
	// 台形か完全な自由四角形かを判定
	f1 = f2 = c = 0 ;
	r1 = _SQRT( xx1 * xx1 + yy1 * yy1 ) ;	xtmp1 = xx1 / r1 ;	ytmp1 = yy1 / r1 ;
	r2 = _SQRT( xx2 * xx2 + yy2 * yy2 ) ;	xtmp2 = xx2 / r2 ;	ytmp2 = yy2 / r2 ;
	if( _FABS( xtmp1 - xtmp2 ) < 0.001F && _FABS( ytmp1 - ytmp2 ) < 0.001F ) c ++, f1 = 1 ;
	
	r3 = _SQRT( xx3 * xx3 + yy3 * yy3 ) ;	xtmp1 = xx3 / r3 ;	ytmp1 = yy3 / r3 ;
	r4 = _SQRT( xx4 * xx4 + yy4 * yy4 ) ;	xtmp2 = xx4 / r4 ;	ytmp2 = yy4 / r4 ;
	if( _FABS( xtmp1 - xtmp2 ) < 0.001F && _FABS( ytmp1 - ytmp2 ) < 0.001F ) c ++, f2 = 1 ;
*/

	{
#define TDIVNUM		5
#define TDN			(1 + ( 1 << TDIVNUM ))
#define ADJUSTUV	(0.001f)
		int DivNum, tdn ;
		static VERTEX_2D VectData2[TDN * TDN] ;
		VERTEX_2D *v ;
		float xxx1, yyy1, xxx2, yyy2, xxx3, yyy3 ;
		float adx1, ady1, adx2, ady2, adx3, ady3, adtu, adtv, tu, tv ; 
		int i, j ;
		float adjust_u_l, adjust_u_r, adjust_v_t, adjust_v_b ;

		// 分割画像の場合隣の画像のピクセルが入らないための補正ＵＶ値を作成する
		{
			if( TexVert[ 0 ].u < 0.000001f )
			{
				adjust_u_l = 0.0f ;
			}
			else
			{
				adjust_u_l =  ( TexVert[ 3 ].u - TexVert[ 0 ].u ) / ( TexVert[ 3 ].x - TexVert[ 0 ].x ) * ADJUSTUV ;
			}

			if( TexVert[ 3 ].u > 0.99999f )
			{
				adjust_u_r = 0.0f ;
			}
			else
			{
				adjust_u_r = -( TexVert[ 3 ].u - TexVert[ 0 ].u ) / ( TexVert[ 3 ].x - TexVert[ 0 ].x ) * ADJUSTUV ;
			}

			if( TexVert[ 0 ].v < 0.000001f )
			{
				adjust_v_t = 0.0f ;
			}
			else
			{
				adjust_v_t =  ( TexVert[ 3 ].v - TexVert[ 0 ].v ) / ( TexVert[ 3 ].y - TexVert[ 0 ].y ) * ADJUSTUV ;
			}

			if( TexVert[ 3 ].v > 0.99999f )
			{
				adjust_v_b = 0.0f ;
			}
			else
			{
				adjust_v_b = -( TexVert[ 3 ].v - TexVert[ 0 ].v ) / ( TexVert[ 3 ].y - TexVert[ 0 ].y ) * ADJUSTUV ;
			}
		}

		// 分割数の算出
		{
			float v1x, v1y, v2x, v2y ;
			float rate ;
			const float Table[] = { 2.0F, 1.0F, 0.6F, 0.1F, 0.05F } ;

			v1x = x4 - x1 ;		v1y = y4 - y1 ;
			v2x = x3 - x2 ;		v2y = y3 - y2 ;
			rate = (float)( 
				( _SQRT( v1x * v1x + v1y * v1y ) + _SQRT( v2x * v2x + v2y * v2y ) ) /
				( ( _SQRT( (float)( GSYS.DrawSetting.DrawSizeX * GSYS.DrawSetting.DrawSizeX + 
						GSYS.DrawSetting.DrawSizeY * GSYS.DrawSetting.DrawSizeY ) ) * 2 ) ) ) ;
			tdn = 8 ;
			if( tdn < 8 ) tdn = 8 ;
			else if( tdn > TDN ) tdn = TDN ;
			for( DivNum = TDIVNUM ; DivNum > 0 && rate < Table[TDIVNUM - DivNum] ; DivNum -- ){}
		}

		// 自由四角形処理
		adx1 = xx3 / ( tdn - 1 ) ;	ady1 = yy3 / ( tdn - 1 ) ;
		adx2 = xx4 / ( tdn - 1 ) ;	ady2 = yy4 / ( tdn - 1 ) ;
		xxx1 = x1 ;		yyy1 = y1 ;
		xxx2 = x2 ;		yyy2 = y2 ;
		adtu = ( ( TexVert[3].u + adjust_u_r ) - ( TexVert[0].u + adjust_u_l ) ) / ( tdn - 1 ) ;
		adtv = ( ( TexVert[3].v + adjust_v_b ) - ( TexVert[0].v + adjust_v_t ) ) / ( tdn - 1 ) ;
		tv = TexVert[0].v + adjust_v_t ;
		v = VectData2 ;
		for( i = 0 ; i < tdn ; i ++, tv += adtv,
									 xxx1 += adx1, yyy1 += ady1,
									 xxx2 += adx2, yyy2 += ady2 )
		{
			adx3 = ( xxx2 - xxx1 ) / ( tdn - 1 ) ;
			ady3 = ( yyy2 - yyy1 ) / ( tdn - 1 ) ;

//			xxx3 = xxx1 - 0.5F ;	yyy3 = yyy1 - 0.5F ;
			xxx3 = xxx1 ;
			yyy3 = yyy1 ;
			tu = TexVert[0].u + adjust_u_l ;
			for( j = 0 ; j < tdn ; j ++, tu += adtu,
										 xxx3 += adx3, yyy3 += ady3, v ++ )
			{
				v->pos.x = (float)xxx3 ;
				v->pos.y = (float)yyy3 ;
				*((DWORD *)&v->pos.z) = drawz ;
				v->rhw   = 1.0F ;
				v->u     = (float)tu ;
				v->v     = (float)tv ;
				v->color = DiffuseColor ;
			}
		}

		// 頂点追加処理
		LINUX_Graphics_SetPlaneVertexHardware( VectData2, tdn, tdn ) ;

#undef TDIVNUM
#undef TDN
	}

	return ;
		
R1 :
	// 普通に描画
	GETVERTEX_QUAD( vec )

	vec[0].color = ( DWORD )DiffuseColor ;
	vec[1].color = ( DWORD )DiffuseColor ;
	vec[2].color = ( DWORD )DiffuseColor ;
	vec[3].color = ( DWORD )DiffuseColor ;
	vec[4].color = ( DWORD )DiffuseColor ;
	vec[5].color = ( DWORD )DiffuseColor ;

	u[ 0 ] = TexVert[0].u + 0.0625f / DrawTex->Tex->TexWidth ;
	u[ 1 ] = TexVert[1].u - 0.0625f / DrawTex->Tex->TexWidth ;
	v[ 0 ] = TexVert[0].v + 0.0625f / DrawTex->Tex->TexHeight ;
	v[ 1 ] = TexVert[2].v - 0.0625f / DrawTex->Tex->TexHeight ;

	if( Reverse0 <= 0  &&  Reverse1 <= 0 )
	{
		vec[0].pos.x = x1 ;	
		vec[0].pos.y = y1 ;
		vec[1].pos.x = x3 ;
		vec[1].pos.y = y3 ;
		vec[2].pos.x = x2 ;	
		vec[2].pos.y = y2 ;
		vec[3].pos.x = x4 ;
		vec[3].pos.y = y4 ;
		vec[4].pos.x = x2 ;	
		vec[4].pos.y = y2 ;
		vec[5].pos.x = x3 ;
		vec[5].pos.y = y3 ;

		vec[0].u = u[ 0 ] ;
		vec[0].v = v[ 0 ] ;
		vec[1].u = u[ 0 ] ;
		vec[1].v = v[ 1 ] ;
		vec[2].u = u[ 1 ] ;
		vec[2].v = v[ 0 ] ;
		vec[3].u = u[ 1 ] ;
		vec[3].v = v[ 1 ] ;
		vec[4].u = u[ 1 ] ;
		vec[4].v = v[ 0 ] ;
		vec[5].u = u[ 0 ] ;
		vec[5].v = v[ 1 ] ;
	}
	else
	{
		vec[0].pos.x = x1 ;	
		vec[0].pos.y = y1 ;
		vec[1].pos.x = x2 ;	
		vec[1].pos.y = y2 ;
		vec[2].pos.x = x3 ;
		vec[2].pos.y = y3 ;
		vec[3].pos.x = x4 ;
		vec[3].pos.y = y4 ;
		vec[4].pos.x = x3 ;
		vec[4].pos.y = y3 ;
		vec[5].pos.x = x2 ;	
		vec[5].pos.y = y2 ;

		vec[0].u = u[ 0 ] ;
		vec[0].v = v[ 0 ] ;
		vec[1].u = u[ 1 ] ;
		vec[1].v = v[ 0 ] ;
		vec[2].u = u[ 0 ] ;
		vec[2].v = v[ 1 ] ;
		vec[3].u = u[ 1 ] ;
		vec[3].v = v[ 1 ] ;
		vec[4].u = u[ 0 ] ;
		vec[4].v = v[ 1 ] ;
		vec[5].u = u[ 1 ] ;
		vec[5].v = v[ 0 ] ;
	}

	*((DWORD *)&vec[0].pos.z) = drawz ;
	*((DWORD *)&vec[1].pos.z) = drawz ;
	*((DWORD *)&vec[2].pos.z) = drawz ;
	*((DWORD *)&vec[3].pos.z) = drawz ;
	*((DWORD *)&vec[4].pos.z) = drawz ;
	*((DWORD *)&vec[5].pos.z) = drawz ;

	vec[0].rhw = 1.0f ;
	vec[1].rhw = 1.0f ;
	vec[2].rhw = 1.0f ;
	vec[3].rhw = 1.0f ;
	vec[4].rhw = 1.0f ;
	vec[5].rhw = 1.0f ;

	ADD4VERTEX_TEX
}

// ハードウエアアクセラレータ使用版 DrawModiGraphF
extern	int		Graphics_Hardware_DrawModiGraphF_PF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag )
{
	int Flag ;

	// もしブレンドテクスチャ付の場合は内部分割を行わない描画関数を使用する
	if( GSYS.DrawSetting.BlendGraph > 0 ) return Graphics_Hardware_DrawModiGraph_PF( _FTOL( x1 ), _FTOL( y1 ), _FTOL( x2 ), _FTOL( y2 ), _FTOL( x3 ), _FTOL( y3 ), _FTOL( x4 ), _FTOL( y4 ), Image, BlendImage, TransFlag, SimpleDrawFlag ) ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画の準備
	Flag = TransFlag ;
	DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )

	// テクスチャーが１枚のみの場合は処理を分岐
	if( Image->Hard.DrawNum == 1 )
	{
		// そのまま描画
		Graphics_Linux_DrawModiTex( x1, y1, x2, y2, x3, y3, x4, y4, &Image->Hard.Draw[ 0 ], false ) ;
	}
	else
	{
		IMAGEDATA_HARD_DRAW *DrawTex ;
		IMAGEDATA_HARD_VERT *TexVert ;
		int DrawTexNum ;
		int i, j ;
		double /*xx1, yy1, xx2, yy2,*/ xx3, yy3, xx4, yy4, xt1, yt1, xt2, yt2 ;
		double w, h, r ;
		float x[4], y[4], *xp, *yp ;

//		xx1 = x2 - x1 ; xx2 = x4 - x3 ;
//		yy1 = y2 - y1 ; yy2 = y4 - y3 ;

		xx3 = x3 - x1 ; xx4 = x4 - x2 ;
		yy3 = y3 - y1 ; yy4 = y4 - y2 ;
		
//		w = 1.0F / (float)Image->Width ;
//		h = 1.0F / (float)Image->Height ;
		w = 1.0F / (double)Image->WidthF ;
		h = 1.0F / (double)Image->HeightF ;

		// 描画情報の数をセット
		DrawTexNum = Image->Hard.DrawNum ;

		// 描画情報配列のアドレスをセットしておく
		DrawTex = Image->Hard.Draw ;

		// テクスチャーの数だけ繰り返す
		for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
		{
			// テクスチャーのデータアドレスを取得
			TexVert = DrawTex->Vertex ;

			// 座標を算出
			xp = x ;
			yp = y ;
			for( j = 0 ; j < 4 ; j ++, TexVert ++ )
			{
				r = TexVert->y * h ;
				xt1 = x1 + xx3 * r ;	yt1 = y1 + yy3 * r ;
				xt2 = x2 + xx4 * r ;	yt2 = y2 + yy4 * r ;

				r = TexVert->x * w ;
				*xp++ = (float)( ( xt2 - xt1 ) * r + xt1 ) ;
				*yp++ = (float)( ( yt2 - yt1 ) * r + yt1 ) ;
			}

			// 描画
			Graphics_Linux_DrawModiTex( x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3], DrawTex, false ) ;
		}
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawSimpleQuadrangleGraphF
extern	int		Graphics_Hardware_DrawSimpleQuadrangleGraphF_PF( const GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	DWORD drawz ;
	int Flag ;
	float u[ 4 ] ;
	float v[ 4 ] ;
	int i ;
	const GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *ParamV ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// テクスチャの数が１以外の場合はエラー
	if( Image->Hard.DrawNum != 1 )
	{
		return -1 ;
	}

	// 描画の準備
	Flag = Param->TransFlag ;
	DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )

	// 頂点データを取得（今のところ色情報のセットのみ）
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// 描画情報配列のアドレスをセットしておく
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 頂点タイプによって処理を分岐
	switch( GLINUX.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// ブレンドテクスチャを使用する
		// 描画するテクスチャーのセット
		Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
		Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

		if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
		{
			Graphics_Linux_DeviceState_NormalDrawSetup() ;
		}

		TexVert = DrawTex->Vertex ;
		BlendTexVert = BlendDrawTex->Vertex ;

		// 頂点データを初期化する
		ParamV = Param->Vertex ;
		for( i = 0 ; i < Param->QuadrangleNum ; i ++, ParamV += 4 )
		{
			GETVERTEX_QUAD( DrawVertB )

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;
			DrawVertB[4].color = ( DWORD )DiffuseColor ;
			DrawVertB[5].color = ( DWORD )DiffuseColor ;

			DrawVertB[0].pos.x = ParamV[ 0 ].x ;
			DrawVertB[0].pos.y = ParamV[ 0 ].y ;
			DrawVertB[1].pos.x = ParamV[ 1 ].x ;
			DrawVertB[1].pos.y = ParamV[ 1 ].y ;
			DrawVertB[2].pos.x = ParamV[ 2 ].x ;
			DrawVertB[2].pos.y = ParamV[ 2 ].y ;
			DrawVertB[3].pos.x = ParamV[ 3 ].x ;
			DrawVertB[3].pos.y = ParamV[ 3 ].y ;
			DrawVertB[4].pos.x = ParamV[ 2 ].x ;
			DrawVertB[4].pos.y = ParamV[ 2 ].y ;
			DrawVertB[5].pos.x = ParamV[ 1 ].x ;
			DrawVertB[5].pos.y = ParamV[ 1 ].y ;

			switch( GSYS.DrawSetting.BlendGraphPosMode )
			{
			default :
			case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
				u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( TexVert[1].x - BlendTexVert[1].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( TexVert[1].y - BlendTexVert[1].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( TexVert[2].x - BlendTexVert[2].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( TexVert[2].y - BlendTexVert[2].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 3 ] = ( TexVert[3].x - BlendTexVert[3].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 3 ] = ( TexVert[3].y - BlendTexVert[3].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				u[ 0 ] = ( ParamV[0].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( ParamV[0].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( ParamV[1].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( ParamV[1].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( ParamV[2].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( ParamV[2].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 3 ] = ( ParamV[3].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 3 ] = ( ParamV[3].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				break ;
			}

			DrawVertB[0].u1 = u[ 0 ] ;
			DrawVertB[0].v1 = v[ 0 ] ;
			DrawVertB[1].u1 = u[ 1 ] ;
			DrawVertB[1].v1 = v[ 1 ] ;
			DrawVertB[2].u1 = u[ 2 ] ;
			DrawVertB[2].v1 = v[ 2 ] ;
			DrawVertB[3].u1 = u[ 3 ] ;
			DrawVertB[3].v1 = v[ 3 ] ;
			DrawVertB[4].u1 = u[ 2 ] ;
			DrawVertB[4].v1 = v[ 2 ] ;
			DrawVertB[5].u1 = u[ 1 ] ;
			DrawVertB[5].v1 = v[ 1 ] ;

			DrawVertB[0].u2 = ParamV[ 0 ].u ;
			DrawVertB[0].v2 = ParamV[ 0 ].v ;
			DrawVertB[1].u2 = ParamV[ 1 ].u ;
			DrawVertB[1].v2 = ParamV[ 1 ].v ;
			DrawVertB[2].u2 = ParamV[ 2 ].u ;
			DrawVertB[2].v2 = ParamV[ 2 ].v ;
			DrawVertB[3].u2 = ParamV[ 3 ].u ;
			DrawVertB[3].v2 = ParamV[ 3 ].v ;
			DrawVertB[4].u2 = ParamV[ 2 ].u ;
			DrawVertB[4].v2 = ParamV[ 2 ].v ;
			DrawVertB[5].u2 = ParamV[ 1 ].u ;
			DrawVertB[5].v2 = ParamV[ 1 ].v ;

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;
			DrawVertB[3].rhw = 1.0f ;
			DrawVertB[4].rhw = 1.0f ;
			DrawVertB[5].rhw = 1.0f ;

			// テクスチャーを描画する
			ADD4VERTEX_BLENDTEX
		}
		break ;

	case VERTEXTYPE_TEX :	// ブレンドテクスチャを使用しない
		TexVert = DrawTex->Vertex ;

		// 頂点データを初期化する
		ParamV = Param->Vertex ;
		for( i = 0 ; i < Param->QuadrangleNum ; i ++, ParamV += 4 )
		{
			GETVERTEX_QUAD( DrawVert )

			DrawVert[0].color = ( DWORD )DiffuseColor ;
			DrawVert[1].color = ( DWORD )DiffuseColor ;
			DrawVert[2].color = ( DWORD )DiffuseColor ;
			DrawVert[3].color = ( DWORD )DiffuseColor ;
			DrawVert[4].color = ( DWORD )DiffuseColor ;
			DrawVert[5].color = ( DWORD )DiffuseColor ;

			DrawVert[0].u = ParamV[ 0 ].u ;
			DrawVert[0].v = ParamV[ 0 ].v ;
			DrawVert[1].u = ParamV[ 1 ].u ;
			DrawVert[1].v = ParamV[ 1 ].v ;
			DrawVert[2].u = ParamV[ 2 ].u ;
			DrawVert[2].v = ParamV[ 2 ].v ;
			DrawVert[3].u = ParamV[ 3 ].u ;
			DrawVert[3].v = ParamV[ 3 ].v ;
			DrawVert[4].u = ParamV[ 2 ].u ;
			DrawVert[4].v = ParamV[ 2 ].v ;
			DrawVert[5].u = ParamV[ 1 ].u ;
			DrawVert[5].v = ParamV[ 1 ].v ;

			DrawVert[0].pos.x = ParamV[ 0 ].x ;
			DrawVert[0].pos.y = ParamV[ 0 ].y ;
			DrawVert[1].pos.x = ParamV[ 1 ].x ;
			DrawVert[1].pos.y = ParamV[ 1 ].y ;
			DrawVert[2].pos.x = ParamV[ 2 ].x ;
			DrawVert[2].pos.y = ParamV[ 2 ].y ;
			DrawVert[3].pos.x = ParamV[ 3 ].x ;
			DrawVert[3].pos.y = ParamV[ 3 ].y ;
			DrawVert[4].pos.x = ParamV[ 2 ].x ;
			DrawVert[4].pos.y = ParamV[ 2 ].y ;
			DrawVert[5].pos.x = ParamV[ 1 ].x ;
			DrawVert[5].pos.y = ParamV[ 1 ].y ;

			*((DWORD *)&DrawVert[0].pos.z) = drawz ;
			*((DWORD *)&DrawVert[1].pos.z) = drawz ;
			*((DWORD *)&DrawVert[2].pos.z) = drawz ;
			*((DWORD *)&DrawVert[3].pos.z) = drawz ;
			*((DWORD *)&DrawVert[4].pos.z) = drawz ;
			*((DWORD *)&DrawVert[5].pos.z) = drawz ;

			DrawVert[0].rhw = 1.0f ;
			DrawVert[1].rhw = 1.0f ;
			DrawVert[2].rhw = 1.0f ;
			DrawVert[3].rhw = 1.0f ;
			DrawVert[4].rhw = 1.0f ;
			DrawVert[5].rhw = 1.0f ;

			// テクスチャーを描画する
			ADD4VERTEX_TEX
		}
		break ;
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawSimpleTriangleGraphF
extern	int		Graphics_Hardware_DrawSimpleTriangleGraphF_PF( const GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	DWORD drawz ;
	int Flag ;
	float u[ 3 ] ;
	float v[ 3 ] ;
	int i ;
	const GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *ParamV ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// テクスチャの数が１以外の場合はエラー
	if( Image->Hard.DrawNum != 1 )
	{
		return -1 ;
	}

	// 描画の準備
	Flag = Param->TransFlag ;
	DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )

	// 頂点データを取得（今のところ色情報のセットのみ）
	DiffuseColor = GLINUX.Device.DrawInfo.DiffuseColor ;

	// 描画情報配列のアドレスをセットしておく
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 頂点タイプによって処理を分岐
	switch( GLINUX.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// ブレンドテクスチャを使用する
		// 描画するテクスチャーのセット
		Graphics_Linux_DrawSetting_SetTexture( &DrawTex->Tex->PF->Texture ) ;
		Graphics_Linux_DrawSetting_SetBlendTexture( &BlendDrawTex->Tex->PF->Texture ) ;

		if( GLINUX.Device.DrawSetting.ChangeTextureFlag )
		{
			Graphics_Linux_DeviceState_NormalDrawSetup() ;
		}

		TexVert = DrawTex->Vertex ;
		BlendTexVert = BlendDrawTex->Vertex ;

		// 頂点データを初期化する
		ParamV = Param->Vertex ;
		for( i = 0 ; i < Param->TriangleNum ; i ++, ParamV += 3 )
		{
			GETVERTEX_TRIANGLE( DrawVertB )

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;

			DrawVertB[0].pos.x = ParamV[ 0 ].x ;
			DrawVertB[0].pos.y = ParamV[ 0 ].y ;
			DrawVertB[1].pos.x = ParamV[ 1 ].x ;
			DrawVertB[1].pos.y = ParamV[ 1 ].y ;
			DrawVertB[2].pos.x = ParamV[ 2 ].x ;
			DrawVertB[2].pos.y = ParamV[ 2 ].y ;

			switch( GSYS.DrawSetting.BlendGraphPosMode )
			{
			default :
			case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
				u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( TexVert[1].x - BlendTexVert[1].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( TexVert[1].y - BlendTexVert[1].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( TexVert[2].x - BlendTexVert[2].x + GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( TexVert[2].y - BlendTexVert[2].y + GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				u[ 0 ] = ( ParamV[0].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( ParamV[0].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( ParamV[1].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( ParamV[1].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( ParamV[2].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GLINUX.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( ParamV[2].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GLINUX.Device.DrawSetting.InvBlendTextureHeight ;
				break ;
			}

			DrawVertB[0].u1 = u[ 0 ] ;
			DrawVertB[0].v1 = v[ 0 ] ;
			DrawVertB[1].u1 = u[ 1 ] ;
			DrawVertB[1].v1 = v[ 1 ] ;
			DrawVertB[2].u1 = u[ 2 ] ;
			DrawVertB[2].v1 = v[ 2 ] ;

			DrawVertB[0].u2 = ParamV[ 0 ].u ;
			DrawVertB[0].v2 = ParamV[ 0 ].v ;
			DrawVertB[1].u2 = ParamV[ 1 ].u ;
			DrawVertB[1].v2 = ParamV[ 1 ].v ;
			DrawVertB[2].u2 = ParamV[ 2 ].u ;
			DrawVertB[2].v2 = ParamV[ 2 ].v ;

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;

			// テクスチャーを描画する
			ADD3VERTEX_BLENDTEX
		}
		break ;

	case VERTEXTYPE_TEX :	// ブレンドテクスチャを使用しない
		TexVert = DrawTex->Vertex ;

		// 頂点データを初期化する
		ParamV = Param->Vertex ;
		for( i = 0 ; i < Param->TriangleNum ; i ++, ParamV += 3 )
		{
			GETVERTEX_TRIANGLE( DrawVert )

			DrawVert[0].color = ( DWORD )DiffuseColor ;
			DrawVert[1].color = ( DWORD )DiffuseColor ;
			DrawVert[2].color = ( DWORD )DiffuseColor ;

			DrawVert[0].u = ParamV[ 0 ].u ;
			DrawVert[0].v = ParamV[ 0 ].v ;
			DrawVert[1].u = ParamV[ 1 ].u ;
			DrawVert[1].v = ParamV[ 1 ].v ;
			DrawVert[2].u = ParamV[ 2 ].u ;
			DrawVert[2].v = ParamV[ 2 ].v ;

			DrawVert[0].pos.x = ParamV[ 0 ].x ;
			DrawVert[0].pos.y = ParamV[ 0 ].y ;
			DrawVert[1].pos.x = ParamV[ 1 ].x ;
			DrawVert[1].pos.y = ParamV[ 1 ].y ;
			DrawVert[2].pos.x = ParamV[ 2 ].x ;
			DrawVert[2].pos.y = ParamV[ 2 ].y ;

			*((DWORD *)&DrawVert[0].pos.z) = drawz ;
			*((DWORD *)&DrawVert[1].pos.z) = drawz ;
			*((DWORD *)&DrawVert[2].pos.z) = drawz ;

			DrawVert[0].rhw = 1.0f ;
			DrawVert[1].rhw = 1.0f ;
			DrawVert[2].rhw = 1.0f ;

			// テクスチャーを描画する
			ADD3VERTEX_TEX
		}
		break ;
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawFillBox
extern	int		Graphics_Hardware_DrawFillBox_PF( int x1, int y1, int x2, int y2, unsigned int Color )
{
	VERTEX_NOTEX_2D *vec ;
	RECT drect = { 0 } ;
	int Red, Green, Blue, Flag ;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 反転処理
	{
		int b ;

		if( x1 > x2 ){ b = x1 ; x1 = x2 ; x2 = b ; }
		if( y1 > y2 ){ b = y1 ; y1 = y2 ; y2 = b ; }
	}
	
	// クリッピング処理
	if( GSYS.DrawSetting.Valid2DMatrix == FALSE )
	{
		drect.left   = x1 ;
		drect.right  = x2 ;
		drect.top    = y1 ;
		drect.bottom = y2 ;
		RectClipping_Inline( &drect, &GSYS.DrawSetting.DrawArea ) ;
		if( drect.left == drect.right || drect.top == drect.bottom ) return 0 ;
	}

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag ) ;

	// 色その他ステータスのセット
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 頂点データのセット
	GETVERTEX_QUAD( vec )
	
	vec[0].color = Color ;
	vec[1].color = Color ;
	vec[2].color = Color ;
	vec[3].color = Color ;
	vec[4].color = Color ;
	vec[5].color = Color ;
	
	vec[0].pos.x = ( float )drect.left   ;
	vec[0].pos.y = ( float )drect.top    ;
	vec[1].pos.x = ( float )drect.right  ;
	vec[1].pos.y = ( float )drect.top    ;
	vec[2].pos.x = ( float )drect.left   ;
	vec[2].pos.y = ( float )drect.bottom ;
	vec[3].pos.x = ( float )drect.right  ;
	vec[3].pos.y = ( float )drect.bottom ;
	vec[4].pos.x = ( float )drect.left   ;
	vec[4].pos.y = ( float )drect.bottom ;
	vec[5].pos.x = ( float )drect.right  ;
	vec[5].pos.y = ( float )drect.top    ;

	*((DWORD *)&vec[0].pos.z) = drawz ;
	*((DWORD *)&vec[1].pos.z) = drawz ;
	*((DWORD *)&vec[2].pos.z) = drawz ;
	*((DWORD *)&vec[3].pos.z) = drawz ;
	*((DWORD *)&vec[4].pos.z) = drawz ;
	*((DWORD *)&vec[5].pos.z) = drawz ;

	vec[0].rhw = 1.0f ;
	vec[1].rhw = 1.0f ;
	vec[2].rhw = 1.0f ;
	vec[3].rhw = 1.0f ;
	vec[4].rhw = 1.0f ;
	vec[5].rhw = 1.0f ;

	// 頂点データの出力
	ADD4VERTEX_NOTEX

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawLineBox
extern	int		Graphics_Hardware_DrawLineBox_PF( int x1, int y1, int x2, int y2, unsigned int Color, int Thickness )
{
	VERTEX_NOTEX_2D *VertData ;
	int Red, Green, Blue ;
	float fx1, fx2, fy1, fy2 ;
	int Flag;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag ) ;

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 色その他ステータスのセット
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// 反転処理
	{
		int bx, by ;

		if( x1 > x2 || y1 > y2 )
		{
			bx = x1 ; by = y1 ;
			x1 = x2 ; y1 = y2 ;
			x2 = bx ; y2 = by ;
		}
	}

	fx1 = ( float )x1 ;
	fy1 = ( float )y1 ;
	fx2 = ( float )x2 ;
	fy2 = ( float )y2 ;

	GETVERTEX_LINEBOX( VertData ) ;

	VertData[ 0].color = Color ;
	VertData[ 1].color = Color ;
	VertData[ 2].color = Color ;
	VertData[ 3].color = Color ;
	VertData[ 4].color = Color ;
	VertData[ 5].color = Color ;
	VertData[ 6].color = Color ;
	VertData[ 7].color = Color ;
	VertData[ 8].color = Color ;
	VertData[ 9].color = Color ;
	VertData[10].color = Color ;
	VertData[11].color = Color ;
	VertData[12].color = Color ;
	VertData[13].color = Color ;
	VertData[14].color = Color ;
	VertData[15].color = Color ;
	VertData[16].color = Color ;
	VertData[17].color = Color ;
	VertData[18].color = Color ;
	VertData[19].color = Color ;
	VertData[20].color = Color ;
	VertData[21].color = Color ;
	VertData[22].color = Color ;
	VertData[23].color = Color ;

	*((DWORD *)&VertData[ 0].pos.z) = drawz ;
	*((DWORD *)&VertData[ 1].pos.z) = drawz ;
	*((DWORD *)&VertData[ 2].pos.z) = drawz ;
	*((DWORD *)&VertData[ 3].pos.z) = drawz ;
	*((DWORD *)&VertData[ 4].pos.z) = drawz ;
	*((DWORD *)&VertData[ 5].pos.z) = drawz ;
	*((DWORD *)&VertData[ 6].pos.z) = drawz ;
	*((DWORD *)&VertData[ 7].pos.z) = drawz ;
	*((DWORD *)&VertData[ 8].pos.z) = drawz ;
	*((DWORD *)&VertData[ 9].pos.z) = drawz ;
	*((DWORD *)&VertData[10].pos.z) = drawz ;
	*((DWORD *)&VertData[11].pos.z) = drawz ;
	*((DWORD *)&VertData[12].pos.z) = drawz ;
	*((DWORD *)&VertData[13].pos.z) = drawz ;
	*((DWORD *)&VertData[14].pos.z) = drawz ;
	*((DWORD *)&VertData[15].pos.z) = drawz ;
	*((DWORD *)&VertData[16].pos.z) = drawz ;
	*((DWORD *)&VertData[17].pos.z) = drawz ;
	*((DWORD *)&VertData[18].pos.z) = drawz ;
	*((DWORD *)&VertData[19].pos.z) = drawz ;
	*((DWORD *)&VertData[20].pos.z) = drawz ;
	*((DWORD *)&VertData[21].pos.z) = drawz ;
	*((DWORD *)&VertData[22].pos.z) = drawz ;
	*((DWORD *)&VertData[23].pos.z) = drawz ;

	VertData[ 0].rhw = 1.0f ;
	VertData[ 1].rhw = 1.0f ;
	VertData[ 2].rhw = 1.0f ;
	VertData[ 3].rhw = 1.0f ;
	VertData[ 4].rhw = 1.0f ;
	VertData[ 5].rhw = 1.0f ;
	VertData[ 6].rhw = 1.0f ;
	VertData[ 7].rhw = 1.0f ;
	VertData[ 8].rhw = 1.0f ;
	VertData[ 9].rhw = 1.0f ;
	VertData[10].rhw = 1.0f ;
	VertData[11].rhw = 1.0f ;
	VertData[12].rhw = 1.0f ;
	VertData[13].rhw = 1.0f ;
	VertData[14].rhw = 1.0f ;
	VertData[15].rhw = 1.0f ;
	VertData[16].rhw = 1.0f ;
	VertData[17].rhw = 1.0f ;
	VertData[18].rhw = 1.0f ;
	VertData[19].rhw = 1.0f ;
	VertData[20].rhw = 1.0f ;
	VertData[21].rhw = 1.0f ;
	VertData[22].rhw = 1.0f ;
	VertData[23].rhw = 1.0f ;

	if( Thickness <= 1 )
	{
		VertData[0].pos.x = fx1			; VertData[0].pos.y = fy1		 ;
		VertData[1].pos.x = fx2			; VertData[1].pos.y = fy1		 ;
		VertData[2].pos.x = fx1			; VertData[2].pos.y = fy1 + 1.0f ;

		VertData[3].pos.x = fx2			; VertData[3].pos.y = fy1 + 1.0f ;
		VertData[4].pos.x = fx1			; VertData[4].pos.y = fy1 + 1.0f ;
		VertData[5].pos.x = fx2			; VertData[5].pos.y = fy1		 ;

		VertData += 6 ;


		VertData[0].pos.x = fx2 - 1.0f	; VertData[0].pos.y = fy1 + 1.0f ;
		VertData[1].pos.x = fx2			; VertData[1].pos.y = fy1 + 1.0f ;
		VertData[2].pos.x = fx2	- 1.0f	; VertData[2].pos.y = fy2		 ;

		VertData[3].pos.x = fx2			; VertData[3].pos.y = fy2		 ;
		VertData[4].pos.x = fx2 - 1.0f	; VertData[4].pos.y = fy2		 ;
		VertData[5].pos.x = fx2			; VertData[5].pos.y = fy1 + 1.0f ;

		VertData += 6 ;


		VertData[0].pos.x = fx1			; VertData[0].pos.y = fy1 + 1.0f ;
		VertData[1].pos.x = fx1 + 1.0f	; VertData[1].pos.y = fy1 + 1.0f ;
		VertData[2].pos.x = fx1			; VertData[2].pos.y = fy2		 ;

		VertData[3].pos.x = fx1 + 1.0f	; VertData[3].pos.y = fy2		 ;
		VertData[4].pos.x = fx1			; VertData[4].pos.y = fy2		 ;
		VertData[5].pos.x = fx1 + 1.0f	; VertData[5].pos.y = fy1 + 1.0f ;

		VertData += 6 ;


		VertData[0].pos.x = fx1 + 1.0f	; VertData[0].pos.y = fy2 - 1.0f ;
		VertData[1].pos.x = fx2 - 1.0f	; VertData[1].pos.y = fy2 - 1.0f ;
		VertData[2].pos.x = fx1 + 1.0f	; VertData[2].pos.y = fy2		 ;

		VertData[3].pos.x = fx2 - 1.0f	; VertData[3].pos.y = fy2		 ;
		VertData[4].pos.x = fx1 + 1.0f	; VertData[4].pos.y = fy2		 ;
		VertData[5].pos.x = fx2 - 1.0f	; VertData[5].pos.y = fy2 - 1.0f ;

		VertData += 6 ;
	}
	else
	{
		float InT, OutT ;
		if( ( Thickness & 1 ) != 0 )
		{
			OutT = ( float )( Thickness >> 1 ) ;
			InT  = ( float )( ( Thickness >> 1 ) + 1 ) ;
		}
		else
		{
			OutT = ( float )( ( Thickness >> 1 ) - 1 ) ;
			InT  = ( float )( ( Thickness >> 1 ) + 1 ) ;
		}

		VertData[0].pos.x = fx1 - OutT  ; VertData[0].pos.y = fy1 - OutT ;
		VertData[1].pos.x = fx2 + OutT  ; VertData[1].pos.y = fy1 - OutT ;
		VertData[2].pos.x = fx1 - OutT  ; VertData[2].pos.y = fy1 + InT  ;

		VertData[3].pos.x = fx2 + OutT  ; VertData[3].pos.y = fy1 + InT  ;
		VertData[4].pos.x = fx1 - OutT  ; VertData[4].pos.y = fy1 + InT  ;
		VertData[5].pos.x = fx2 + OutT  ; VertData[5].pos.y = fy1 - OutT ;

		VertData += 6 ;


		VertData[0].pos.x = fx2 - InT	; VertData[0].pos.y = fy1 + InT  ;
		VertData[1].pos.x = fx2	+ OutT	; VertData[1].pos.y = fy1 + InT  ;
		VertData[2].pos.x = fx2	- InT	; VertData[2].pos.y = fy2 + OutT ;

		VertData[3].pos.x = fx2 + OutT  ; VertData[3].pos.y = fy2 + OutT ;
		VertData[4].pos.x = fx2	- InT	; VertData[4].pos.y = fy2 + OutT ;
		VertData[5].pos.x = fx2	+ OutT	; VertData[5].pos.y = fy1 + InT  ;

		VertData += 6 ;


		VertData[0].pos.x = fx1	- OutT  ; VertData[0].pos.y = fy1 + InT  ;
		VertData[1].pos.x = fx1 + InT   ; VertData[1].pos.y = fy1 + InT  ;
		VertData[2].pos.x = fx1	- OutT  ; VertData[2].pos.y = fy2 + OutT ;

		VertData[3].pos.x = fx1 + InT   ; VertData[3].pos.y = fy2 + OutT ;
		VertData[4].pos.x = fx1	- OutT  ; VertData[4].pos.y = fy2 + OutT ;
		VertData[5].pos.x = fx1 + InT   ; VertData[5].pos.y = fy1 + InT  ;

		VertData += 6 ;


		VertData[0].pos.x = fx1 + InT   ; VertData[0].pos.y = fy2 - InT  ;
		VertData[1].pos.x = fx2 - InT   ; VertData[1].pos.y = fy2 - InT  ;
		VertData[2].pos.x = fx1 + InT   ; VertData[2].pos.y = fy2 + OutT ;

		VertData[3].pos.x = fx2 - InT   ; VertData[3].pos.y = fy2 + OutT ;
		VertData[4].pos.x = fx1 + InT   ; VertData[4].pos.y = fy2 + OutT ;
		VertData[5].pos.x = fx2 - InT   ; VertData[5].pos.y = fy2 - InT  ;

		VertData += 6 ;
	}

	// 頂点の追加
	ADD4VERTEX_LINEBOX

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawLine
extern	int		Graphics_Hardware_DrawLine_PF( int x1, int y1, int x2, int y2, unsigned int Color )
{
	VERTEX_NOTEX_2D *VectData ;
	int Red, Green, Blue, Flag ;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( x2 - x1 == 0 && y2 - y1 == 0 ) return 0 ; 

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 色をセット
	GETVERTEX_LINE( VectData ) ;
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )
	VectData[ 0 ].color = Color ;
	VectData[ 1 ].color = Color ;
	*((DWORD *)&VectData[ 0 ].pos.z) = drawz ;
	*((DWORD *)&VectData[ 1 ].pos.z) = drawz ;
	VectData[ 0 ].rhw =
	VectData[ 1 ].rhw = 1.0f ;

	// 座標をセット
	VectData[ 0 ].pos.x = ( float )( x1 + 0.5f ) ;
	VectData[ 0 ].pos.y = ( float )( y1 + 0.5f ) ;

	VectData[ 1 ].pos.x = ( float )( x2 + 0.5f ) ;
	VectData[ 1 ].pos.y = ( float )( y2 + 0.5f ) ;

	// 座標の補正
	if( x1 == x2 )
	{
		if( y1 < y2 )
		{
			VectData[ 0 ].pos.y -= 0.1f ;
			VectData[ 1 ].pos.y -= 0.1f ;
		}
		else
		{
			VectData[ 0 ].pos.y += 0.1f ;
			VectData[ 1 ].pos.y += 0.1f ;
		}
	}

	if( y1 == y2 )
	{
		if( x1 < x2 )
		{
			VectData[ 0 ].pos.x -= 0.1f ;
			VectData[ 1 ].pos.x -= 0.1f ;
		}
		else
		{
			VectData[ 0 ].pos.x += 0.1f ;
			VectData[ 1 ].pos.x += 0.1f ;
		}
	}

	// 頂点の追加
	ADD4VERTEX_LINE

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawLine3D
extern	int		Graphics_Hardware_DrawLine3D_PF( VECTOR Pos1, VECTOR Pos2, unsigned int Color, int DrawFlag, RECT *DrawArea )
{
	VERTEX_NOTEX_3D *VectData ;
	int Red, Green, Blue, Flag ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( DrawFlag )
	{
		// 描画の準備
		Flag = DX_LINUX_DRAWPREP_DIFFUSERGB | DX_LINUX_DRAWPREP_3D | DX_LINUX_DRAWPREP_FOG ;
		DX_LINUX_DRAWPREP_NOTEX( Flag )

		GETVERTEX_LINE3D( VectData ) ;

		// 色をセット
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		SETUPCOLOR( Color )
		*( ( DWORD * )&VectData[ 0 ].b ) = Color ;
		*( ( DWORD * )&VectData[ 1 ].b ) = Color ;

		// 座標をセット
		VectData[ 0 ].pos = Pos1 ;
		VectData[ 1 ].pos = Pos2 ;

		// 頂点の追加
		ADD4VERTEX_LINE3D
	}
	else
	{
		VERTEX_2D TempVect[ 2 ] ;
		float dleft, dright, dtop, dbottom ;

		dleft   = -100000000.0f ;
		dright  =  100000000.0f ;
		dtop    = -100000000.0f ;
		dbottom =  100000000.0f ;

		// スクリーン座標に変換
		TempVect[0].pos.x = Pos1.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + Pos1.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + Pos1.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0]  ;
		TempVect[0].pos.y = Pos1.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + Pos1.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + Pos1.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1]  ;
		TempVect[0].pos.z = Pos1.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + Pos1.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + Pos1.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2]  ;
		TempVect[0].rhw   = Pos1.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + Pos1.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + Pos1.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3]  ;

		TempVect[1].pos.x = Pos2.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + Pos2.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + Pos2.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0]  ;
		TempVect[1].pos.y = Pos2.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + Pos2.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + Pos2.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1]  ;
		TempVect[1].pos.z = Pos2.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + Pos2.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + Pos2.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2]  ;
		TempVect[1].rhw   = Pos2.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + Pos2.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + Pos2.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3]  ;

		TempVect[0].rhw = 1.0F / TempVect[0].rhw ;
		TempVect[0].pos.x *= TempVect[0].rhw ;
		TempVect[0].pos.y *= TempVect[0].rhw ;
		TempVect[0].pos.z *= TempVect[0].rhw ;

		TempVect[1].rhw = 1.0F / TempVect[1].rhw ;
		TempVect[1].pos.x *= TempVect[1].rhw ;
		TempVect[1].pos.y *= TempVect[1].rhw ;
		TempVect[1].pos.z *= TempVect[1].rhw ;

		if( dright  > TempVect[ 0 ].pos.x ) dright  = TempVect[ 0 ].pos.x ;
		if( dright  > TempVect[ 1 ].pos.x ) dright  = TempVect[ 1 ].pos.x ;

		if( dleft   < TempVect[ 0 ].pos.x ) dleft   = TempVect[ 0 ].pos.x ;
		if( dleft   < TempVect[ 1 ].pos.x ) dleft   = TempVect[ 1 ].pos.x ;

		if( dbottom > TempVect[ 0 ].pos.y ) dbottom = TempVect[ 0 ].pos.y ;
		if( dbottom > TempVect[ 1 ].pos.y ) dbottom = TempVect[ 1 ].pos.y ;

		if( dtop    < TempVect[ 0 ].pos.y ) dtop    = TempVect[ 0 ].pos.y ;
		if( dtop    < TempVect[ 1 ].pos.y ) dtop    = TempVect[ 1 ].pos.y ;

		DrawArea->left   = _FTOL( dleft   ) - 1 ;
		DrawArea->right  = _FTOL( dright  ) + 1 ;
		DrawArea->top    = _FTOL( dtop    ) - 1 ;
		DrawArea->bottom = _FTOL( dbottom ) + 1 ;
	}

	// 終了
	return 0 ;
}



#define LINUX_CIRCLE_VERTEX_NUM			(2000)

#define DX_LINUX_CIRCLE_THICKNESS											\
	if( x1 < DrawRect.left      ) x1 = DrawRect.left ;						\
	if( x2 > DrawRect.right - 1 ) x2 = DrawRect.right - 1 ;					\
																			\
	VertBuf[ VectNum ].pos.x  = ( float )x1 + 0.1f ;						\
	VertBuf[ VectNum ].pos.y  = y1f + 0.1f ;								\
	VertBuf[ VectNum ].color = Color ;										\
	VertBuf[ VectNum ].rhw   = 1.0f ;										\
	*((DWORD *)&VertBuf[ VectNum ].pos.z) = drawz ;							\
	VectNum ++ ;															\
																			\
	VertBuf[ VectNum ].pos.x =  ( float )( x2 + 1 ) + 0.1f ;				\
	VertBuf[ VectNum ].pos.y =  y1f + 0.1f ;								\
	VertBuf[ VectNum ].color = Color ;										\
	VertBuf[ VectNum ].rhw   = 1.0f ;										\
	*((DWORD *)&VertBuf[ VectNum ].pos.z) = drawz ;							\
	VectNum  ++ ;															\
																			\
	if( VectNum >= LINUX_CIRCLE_VERTEX_NUM )								\
	{																		\
		/* Uniform の更新 */												\
		Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;\
																			\
		/* 頂点データのセットアップ */										\
		Graphics_Linux_DeviceState_SetupShaderVertexData(					\
			GLINUX.Device.State.SetShader,									\
			&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D ].InputInfo,\
			VertBuf															\
		) ;																	\
																			\
		/* 描画 */															\
		glDrawArrays( GL_LINES, 0, VectNum );								\
		GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;						\
																			\
		VectNum = 0 ;														\
	}																		

// ハードウエアアクセラレータ使用版 DrawCircle( 太さ指定あり )
extern	int		Graphics_Hardware_DrawCircle_Thickness_PF( int x, int y, int r, unsigned int Color, int Thickness )
{
	VERTEX_NOTEX_2D *VertBuf ;
	int VectNum ;
	short ( *CirclePos )[ 5 ] ;
	int Red, Green, Blue, Flag ;
	RECT DrawRect ;
	DWORD drawz ;
	int i ;
//	int j ;
	int x1, x2 ;
	float y1f ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 太さが２以下だったら何もせず終了
	if( Thickness < 2 )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画用メモリの確保
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * LINUX_CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 補正
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// 円の座標を代入するバッファを取得
	if( Graphics_Other_AllocCommonBuffer( 1, sizeof( short ) * 5 * DrawRect.bottom ) < 0 )
	{
		return -1 ;
	}
	CirclePos = ( short (*)[ 5 ] )GSYS.Resource.CommonBuffer[ 1 ] ;
	_MEMSET( CirclePos, 0, sizeof( short ) * 5 * DrawRect.bottom ) ;

	// 円の座標リスト作成
	Graphics_Draw_GetCircle_ThicknessDrawPosition( x, y, r, Thickness, CirclePos ) ;

	// 描画処理
	VectNum = 0 ;
	for( i = DrawRect.top ; i < DrawRect.bottom ; i ++ )
	{
		if( CirclePos[ i ][ 4 ] == 0 )
		{
			continue ;
		}

		y1f = ( float )i ;

		if( ( CirclePos[ i ][ 4 ] & ( 4 | 8 ) ) == 0 || CirclePos[ i ][ 2 ] >= CirclePos[ i ][ 3 ] )
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_LINUX_CIRCLE_THICKNESS
		}
		else
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 2 ] ;
			DX_LINUX_CIRCLE_THICKNESS

			x1 = CirclePos[ i ][ 3 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_LINUX_CIRCLE_THICKNESS
		}
	}
	if( VectNum )
	{
		// Uniform の更新
		Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

		// 頂点データのセットアップ
		Graphics_Linux_DeviceState_SetupShaderVertexData(
			GLINUX.Device.State.SetShader,
			&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D ].InputInfo,
			VertBuf
		) ;	

		// 描画
		glDrawArrays( GL_LINES, 0, VectNum ) ;
		GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawOval( 太さ指定あり )
extern	int		Graphics_Hardware_DrawOval_Thickness_PF( int x, int y, int rx, int ry, unsigned int Color, int Thickness )
{
	VERTEX_NOTEX_2D *VertBuf ;
	int VectNum ;
	short ( *CirclePos )[ 5 ] ;
	int Red, Green, Blue, Flag ;
	RECT DrawRect ;
	DWORD drawz ;
	int i ;
//	int j ;
	int x1, x2 ;
	float y1f ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 太さが２以下だったら何もせず終了
	if( Thickness < 2 )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画用メモリの確保
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * LINUX_CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 補正
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// 円の座標を代入するバッファを取得
	if( Graphics_Other_AllocCommonBuffer( 1, sizeof( short ) * 5 * DrawRect.bottom ) < 0 )
	{
		return -1 ;
	}
	CirclePos = ( short (*)[ 5 ] )GSYS.Resource.CommonBuffer[ 1 ] ;
	_MEMSET( CirclePos, 0, sizeof( short ) * 5 * DrawRect.bottom ) ;

	// 楕円の座標リスト作成
	if( Graphics_Draw_GetOval_ThicknessDrawPosition( x, y, rx, ry, Thickness, CirclePos ) < 0 )
	{
		return -1 ;
	}

	// 描画処理
	VectNum = 0 ;
	for( i = DrawRect.top ; i < DrawRect.bottom ; i ++ )
	{
		if( CirclePos[ i ][ 4 ] == 0 )
		{
			continue ;
		}

		y1f = ( float )i ;

		if( ( CirclePos[ i ][ 4 ] & ( 4 | 8 ) ) == 0 || CirclePos[ i ][ 2 ] >= CirclePos[ i ][ 3 ] )
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_LINUX_CIRCLE_THICKNESS
		}
		else
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 2 ] ;
			DX_LINUX_CIRCLE_THICKNESS

			x1 = CirclePos[ i ][ 3 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_LINUX_CIRCLE_THICKNESS
		}
	}
	if( VectNum )
	{
		// Uniform の更新
		Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

		// 頂点データのセットアップ
		Graphics_Linux_DeviceState_SetupShaderVertexData(
			GLINUX.Device.State.SetShader,
			&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D ].InputInfo,
			VertBuf
		) ;	

		// 描画
		glDrawArrays( GL_LINES, 0, VectNum ) ;
		GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;
	}

	// 終了
	return 0 ;
}




#define DX_LINUX_CIRCLE																	\
			if( !( ( x2 < DrawRect.left ) || ( x1 >= DrawRect.right  ) ||				\
			       ( y1 < DrawRect.top  ) || ( y1 >= DrawRect.bottom ) || LineDrawBuf[ y1 ] ) )		\
			{																			\
				if( x1 < DrawRect.left ) x1 = DrawRect.left ;							\
				if( x2 > DrawRect.right - 1 ) x2 = DrawRect.right - 1 ;					\
																						\
				VertBuf[ VectNum ].pos.x  = ( float )x1 + 0.1f ;						\
				VertBuf[ VectNum ].pos.y  = ( float )y1 + 0.2f ;						\
				VertBuf[ VectNum ].color = Color ;										\
				VertBuf[ VectNum ].rhw   = 1.0f ;										\
				*((DWORD *)&VertBuf[ VectNum ].pos.z) = drawz ;							\
				VectNum ++ ;															\
				LineDrawBuf[y1] = TRUE ;												\
																						\
				VertBuf[ VectNum ].pos.x =  ( float )( x2 + 1 ) + 0.1f ;				\
				VertBuf[ VectNum ].pos.y =  ( float )y1 + 0.2f ;						\
				VertBuf[ VectNum ].color = Color ;										\
				VertBuf[ VectNum ].rhw   = 1.0f ;										\
				*((DWORD *)&VertBuf[ VectNum ].pos.z) = drawz ;							\
				VectNum  ++ ;															\
																						\
				if( VectNum >= LINUX_CIRCLE_VERTEX_NUM )								\
				{																		\
					/* Uniform の更新 */												\
					Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;\
																						\
					/* 頂点データのセットアップ */										\
					Graphics_Linux_DeviceState_SetupShaderVertexData(					\
						GLINUX.Device.State.SetShader,									\
						&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D ].InputInfo,\
						VertBuf															\
					) ;																	\
																						\
					/* 描画 */															\
					glDrawArrays( GL_LINES, 0, VectNum );								\
					GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;						\
					VectNum = 0 ;														\
				}																		\
			}

#define DX_LINUX_CIRCLE_PSET															\
			if( !( ( x1 < DrawRect.left ) || ( x1 >= DrawRect.right  ) ||				\
				   ( y1 < DrawRect.top  ) || ( y1 >= DrawRect.bottom ) ) )				\
			{																			\
				VertBuf[ VectNum ].pos.x  = ( float )x1 + 0.1f ;						\
				VertBuf[ VectNum ].pos.y  = ( float )y1 + 0.1f ;						\
				VertBuf[ VectNum ].color = Color ;										\
				VertBuf[ VectNum ].rhw   = 1.0f ;										\
				*((DWORD *)&VertBuf[ VectNum ].pos.z) = drawz ;							\
				VectNum ++ ;															\
																						\
				if( VectNum >= LINUX_CIRCLE_VERTEX_NUM )								\
				{																		\
					/* Uniform の更新 */												\
					Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;\
																						\
					/* 頂点データのセットアップ */										\
					Graphics_Linux_DeviceState_SetupShaderVertexData(					\
						GLINUX.Device.State.SetShader,									\
						&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D ].InputInfo,\
						VertBuf															\
					) ;																	\
																						\
					/* 描画 */															\
					glDrawArrays( GL_POINTS, 0, VectNum );								\
					GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;						\
					VectNum = 0 ;														\
				}																		\
			}

// ハードウエアアクセラレータ使用版 DrawCircle
extern	int		Graphics_Hardware_DrawCircle_PF( int x, int y, int r, unsigned int Color, int FillFlag, int Rx_One_Minus, int Ry_One_Minus )
{
	VERTEX_NOTEX_2D *VertBuf ;
	int             VectNum ;
	BYTE            *LineDrawBuf ;
	int             Red ;
	int             Green ;
	int             Blue ;
	int             Flag ;
	RECT            DrawRect ;
	DWORD           drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	Rx_One_Minus = Rx_One_Minus ? 1 : 0 ;
	Ry_One_Minus = Ry_One_Minus ? 1 : 0 ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画用メモリの確保
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * LINUX_CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 補正
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// 描画処理
	{
		int Dx, Dy, F/*, i, j*/ ;
		int x1, x2, y1 ;

		// 初期値セット
		Dx = r ; Dy = 0 ; F = -2 * r + 3 ;
		VectNum = 0 ;

//		j = 0 ;
		// 描画開始
		if( FillFlag )
		{
			if( Graphics_Other_AllocCommonBuffer( 1, ( DWORD )DrawRect.bottom ) < 0 )
			{
				return DXST_LOGFILE_ADDUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x48\x00\x61\x00\x72\x00\x64\x00\x77\x00\x61\x00\x72\x00\x65\x00\x5f\x00\x44\x00\x72\x00\x61\x00\x77\x00\x43\x00\x69\x00\x72\x00\x63\x00\x6c\x00\x65\x00\x5f\x00\x50\x00\x46\x00\x00"/*@ L"メモリの確保に失敗しました in Graphics_Hardware_DrawCircle_PF" @*/ ) ;
			}
			LineDrawBuf = ( BYTE * )GSYS.Resource.CommonBuffer[ 1 ] ;
			_MEMSET( LineDrawBuf, 0, ( size_t )DrawRect.bottom ) ;

			// 最初のラインを描く
			{
				if( Ry_One_Minus == FALSE )
				{
					x2 = Dx + x - Rx_One_Minus ; x1 = -Dx + x ; y1 = Dy + y ;
					DX_LINUX_CIRCLE ;
				}

				// 座標データを進める
				if( F >= 0 )
				{
					x2 = Dy + x - Rx_One_Minus ; x1 = -Dy + x ; y1 = Dx + y - Ry_One_Minus ;
					DX_LINUX_CIRCLE ;
					x2 = Dy + x - Rx_One_Minus ; x1 = -Dy + x ; y1 = -Dx + y;
					DX_LINUX_CIRCLE ;

					Dx -- ;
					F -= 4 * Dx ;
				}	

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}

			while( Dx >= Dy )
			{
				x2 = Dx + x - Rx_One_Minus ; x1 = -Dx + x ; y1 = Dy + y - Ry_One_Minus ;
				DX_LINUX_CIRCLE ;
				x2 = Dx + x - Rx_One_Minus ; x1 = -Dx + x ; y1 = -Dy + y ;
				DX_LINUX_CIRCLE ;

				// 座標データを進める
				if( F >= 0 )
				{
					x2 = Dy + x - Rx_One_Minus ; x1 = -Dy + x ; y1 = Dx + y - Ry_One_Minus ;
					DX_LINUX_CIRCLE ;
					x2 = Dy + x - Rx_One_Minus ; x1 = -Dy + x ; y1 = -Dx + y ;
					DX_LINUX_CIRCLE ;

					Dx -- ;
					F -= 4 * Dx ;
				}

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}
			if( VectNum )
			{
				// Uniform の更新
				Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

				// 頂点データのセットアップ
				Graphics_Linux_DeviceState_SetupShaderVertexData(
					GLINUX.Device.State.SetShader,
					&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D ].InputInfo,
					VertBuf
				) ;	

				// 描画
				glDrawArrays( GL_LINES, 0, VectNum ) ;
				GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;
			}
		}
		else
		{
			// 最初の点を描く
			{
				if( Ry_One_Minus == FALSE )
				{
					x1 = -Dx + x ; y1 = Dy + y ;
					DX_LINUX_CIRCLE_PSET ;
					x1 = Dx + x - Rx_One_Minus ;  
					DX_LINUX_CIRCLE_PSET ;
				}

				if( Rx_One_Minus == FALSE )
				{
					x1 = Dy + x ; y1 = Dx + y - Ry_One_Minus ;
					DX_LINUX_CIRCLE_PSET ;

					x1 = Dy + x ; y1 = -Dx + y;
					DX_LINUX_CIRCLE_PSET ;
				}

				// 座標データを進める
				if( F >= 0 )
				{
					Dx -- ;
					F -= 4 * Dx ;
				}	

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}

			while( Dx >= Dy )
			{
				x1 = -Dx + x ; y1 = Dy + y - Ry_One_Minus ;
				DX_LINUX_CIRCLE_PSET ;
				x1 = Dx + x - Rx_One_Minus ;  
				DX_LINUX_CIRCLE_PSET ;

				x1 = Dy + x - Rx_One_Minus ; y1 = Dx + y - Ry_One_Minus ;
				DX_LINUX_CIRCLE_PSET ;
				x1 = -Dy + x ; 
				DX_LINUX_CIRCLE_PSET ;
			
				x1 = Dx + x - Rx_One_Minus ; y1 = -Dy + y ;
				DX_LINUX_CIRCLE_PSET ;
				x1 = -Dx + x ; 
				DX_LINUX_CIRCLE_PSET ;

				x1 = Dy + x - Rx_One_Minus ; y1 = -Dx + y ;
				DX_LINUX_CIRCLE_PSET ;
				x1 = -Dy + x ; 
				DX_LINUX_CIRCLE_PSET ;

				// 座標データを進める
				if( F >= 0 )
				{
					Dx -- ;
					F -= 4 * Dx ;
				}

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}
			if( VectNum )
			{
				// Uniform の更新
				Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

				// 頂点データのセットアップ
				Graphics_Linux_DeviceState_SetupShaderVertexData(
					GLINUX.Device.State.SetShader,
					&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D ].InputInfo,
					VertBuf
				) ;	

				// 描画
				glDrawArrays( GL_POINTS, 0, VectNum ) ;
				GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;
			}
		}
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawOval
extern	int		Graphics_Hardware_DrawOval_PF( int x, int y, int rx, int ry, unsigned int Color, int FillFlag, int Rx_One_Minus, int Ry_One_Minus )
{
	VERTEX_NOTEX_2D *VertBuf ;
	int VectNum ;
	BYTE *LineDrawBuf ;
//	VERTEX_NOTEX_2D *VectData ;
//	int i, j, r2 ;
//	float xd, x2, y2 ;
//	float d, yd ;
	int Red, Green, Blue, Flag ;
	RECT DrawRect ;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	Rx_One_Minus = Rx_One_Minus ? 1 : 0 ;
	Ry_One_Minus = Ry_One_Minus ? 1 : 0 ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	if( !rx || !ry ) return -1 ;

	// 描画用メモリの確保
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * LINUX_CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// 補正
	if( rx < 0 ) rx *= -1 ;
	if( ry < 0 ) ry *= -1 ;
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	VectNum = 0 ;

	// 描画処理
	{
		int Dx, Dy, F, H/*, j, i */ ;
		int x1, x2, y1, Df ;
		int yc, xc, rc, r ;

//		j = 0 ;
		// 描画開始
		if( FillFlag )
		{
			if( Graphics_Other_AllocCommonBuffer( 1, ( DWORD )DrawRect.bottom ) < 0 )
			{
				return DXST_LOGFILE_ADDUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x5f\x00\x44\x00\x72\x00\x61\x00\x77\x00\x43\x00\x69\x00\x72\x00\x63\x00\x6c\x00\x65\x00\x00"/*@ L"メモリの確保に失敗しました in G_D3D_DrawCircle" @*/ ) ;
			}
			LineDrawBuf = ( BYTE * )GSYS.Resource.CommonBuffer[ 1 ] ;
			_MEMSET( LineDrawBuf, 0, ( size_t )DrawRect.bottom ) ;

			if( rx >= ry )
			{
				// 初期値をセット
				rc = ( rx * rx ) / ry ;

				xc = 0 ; yc = 0 ;
				r = rx ;
				Df = 1 ;
		
				Dx = r ;
				Dy = 0;
				F = -2 * r + 1 + 2 * 1;
				H = -4 * r + 2 + 1;

				// 最初の線を描く
				{
					if( Ry_One_Minus == FALSE )
					{
						x1 = -Dx + x ; x2 = Dx + x - Rx_One_Minus ; y1 = Dy + y ;
						DX_LINUX_CIRCLE ;
					}

					Df = 0 ;

					// 座標データを進める
					if( F < 0 )
					{
						yc += ry ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						Df = 1 ;

						if( H>=0 )
						{
							Dx--;
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Dx--;	
							yc += ry ;
							if( yc >= rc )
							{
								Dy ++ ;
								yc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				while( Dx >= 0 )
				{
					if( Df )
					{
						x1 = -Dx + x ; x2 = Dx + x - Rx_One_Minus ; y1 = Dy + y - Ry_One_Minus ;
						DX_LINUX_CIRCLE ;

						y1 = -Dy + y ;
						DX_LINUX_CIRCLE ;
					}

					Df = 0 ;

					// 座標データを進める
					if( F < 0 )
					{
						yc += ry ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						if( H>=0 )
						{
							Dx--;
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Dx--;	
							yc += ry ;
							if( yc >= rc )
							{
								Dy ++ ;
								yc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				// 最後の線を描く
				x1 = -Dx + x ; x2 = Dx + x - Rx_One_Minus ; y1 = -Dy + y;
				DX_LINUX_CIRCLE ;
			}
			else
			{
				// 初期値をセット
				rc = ( ry * ry ) / rx ;

				yc = 0 ; xc = 0 ;
				r = ry ;
		
				Dy = r ;
				Dx = 0;
				F = -2 * r + 1 + 2 * 1;
				H = -4 * r + 2 + 1;

				// 最初の線を描く
				{
					// 座標データを進める
					if( F < 0 )
					{
						xc += rx ;
						if( xc >= rc )
						{
							Dx ++ ;
							xc -= rc ;
						}
						F += 4 * Dx + 2 ;
						H += 4 * Dx;
					}
					else
					{
						if( H>=0 )
						{
							Dy--;
							F -= 4 * Dy;
							H -= 4 * Dy - 2 ;
						}
						else
						{
							Dy--;	
							xc += rx ;
							if( xc >= rc )
							{
								Dx ++ ;
								xc -= rc ;
							}
							F += 4 * Dx - 4 * Dy + 2 ;
							H += 4 * Dx - 4 * Dy + 2 ;
						}
					}
				}

				while( Dy >= 0 )
				{
					// 座標データを進める
					if( F < 0 )
					{
						xc += rx ;
						if( xc >= rc )
						{
							Dx ++ ;
							xc -= rc ;
						}
						F += 4 * Dx + 2 ;
						H += 4 * Dx;
					}
					else
					{
						y1 = -Dy + y ; x1 = -Dx + x ; x2 = Dx + x - Rx_One_Minus ;
						DX_LINUX_CIRCLE ;

						y1 = Dy + y - Ry_One_Minus ;
						DX_LINUX_CIRCLE ;

						if( H>=0 )
						{
							Dy--;
							F -= 4 * Dy;
							H -= 4 * Dy - 2 ;
						}
						else
						{
							Dy--;	
							xc += rx ;
							if( xc >= rc )
							{
								Dx ++ ;
								xc -= rc ;
							}
							F += 4 * Dx - 4 * Dy + 2 ;
							H += 4 * Dx - 4 * Dy + 2 ;
						}
					}
				}
			}
			if( VectNum )
			{
				// Uniform の更新
				Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

				// 頂点データのセットアップ
				Graphics_Linux_DeviceState_SetupShaderVertexData(
					GLINUX.Device.State.SetShader,
					&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D ].InputInfo,
					VertBuf
				) ;	

				// 描画
				glDrawArrays( GL_LINES, 0, VectNum ) ;
				GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;
			}
		}
		else
		{
			if( rx >= ry )
			{
				// 初期値をセット
				rc = ( rx * rx ) / ry ;

				xc = 0 ; yc = 0 ;
				r = rx ;
				Df = 2 ;
		
				Dx = r ;
				Dy = 0;
				F = -2 * r + 1 + 2 * 1;
				H = -4 * r + 2 + 1;

				// 最初の点を描く
				{
					if( Ry_One_Minus == FALSE )
					{
						x1 = -Dx + x ; y1 = Dy + y ;
						DX_LINUX_CIRCLE_PSET ;
						x1 = Dx + x - Rx_One_Minus ;  
						DX_LINUX_CIRCLE_PSET ;
					}

					Df = 0 ;

					// 座標データを進める
					if( F < 0 )
					{
						yc += ry ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						Df = 1 ;

						if( H>=0 )
						{
							Dx--;
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Dx--;	
							yc += ry ;
							if( yc >= rc )
							{
								Dy ++ ;
								yc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				while( Dx > 0 )
				{
					if( Df )
					{
						x1 = -Dx + x ; y1 = Dy + y - Ry_One_Minus ;
						DX_LINUX_CIRCLE_PSET ;
						x1 = Dx + x - Rx_One_Minus ;
						DX_LINUX_CIRCLE_PSET ;

						x1 = -Dx + x ; y1 = -Dy + y ;
						DX_LINUX_CIRCLE_PSET ;
						x1 = Dx + x - Rx_One_Minus ;
						DX_LINUX_CIRCLE_PSET ;
					}

					Df = 0 ;

					// 座標データを進める
					if( F < 0 )
					{
						yc += ry ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						Df = 1 ;

						if( H>=0 )
						{
							Dx--;
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Dx--;	
							yc += ry ;
							if( yc >= rc )
							{
								Dy ++ ;
								yc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				// 最後の点を描く
				if( Rx_One_Minus == FALSE )
				{
					x1 = Dx + x ; y1 = -Dy + y ;
					DX_LINUX_CIRCLE_PSET ;

					x1 = -Dx + x ; y1 = Dy + y - Ry_One_Minus ;
					DX_LINUX_CIRCLE_PSET ;
				}
			}
			else
			{
				// 初期値をセット
				rc = ( ry * ry ) / rx ;

				xc = 0 ; yc = 0 ;
				r = ry ;
				Df = 1 ;
		
				Dy = r ;
				Dx = 0;
				F = -2 * r + 1 + 2 * 1;
				H = -4 * r + 2 + 1;

				// 最初の点を描く
				{
					if( Ry_One_Minus == FALSE )
					{
						y1 = -Dy + y ; x1 = Dx + x ;
						DX_LINUX_CIRCLE_PSET ;
						y1 = Dy + y - Ry_One_Minus ;  
						DX_LINUX_CIRCLE_PSET ;
					}

					Df = 0 ;

					// 座標データを進める
					if( F < 0 )
					{
						xc += rx ;
						if( xc >= rc )
						{
							Dx ++ ;
							xc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dx + 2 ;
						H += 4 * Dx;
					}
					else
					{
						Df = 1 ;

						if( H>=0 )
						{
							Dy--;
							F -= 4 * Dy;
							H -= 4 * Dy - 2 ;
						}
						else
						{
							Dy--;	
							xc += rx ;
							if( xc >= rc )
							{
								Dx ++ ;
								xc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dx - 4 * Dy + 2 ;
							H += 4 * Dx - 4 * Dy + 2 ;
						}
					}
				}

				while( Dy > 0 )
				{
					if( Df )
					{
						y1 = -Dy + y ; x1 = Dx + x - Rx_One_Minus ;
						DX_LINUX_CIRCLE_PSET ;
						y1 = Dy + y - Ry_One_Minus ;
						DX_LINUX_CIRCLE_PSET ;

						y1 = -Dy + y ; x1 = -Dx + x ;
						DX_LINUX_CIRCLE_PSET ;
						y1 = Dy + y - Ry_One_Minus ;
						DX_LINUX_CIRCLE_PSET ;
					}

					Df = 0 ;

					// 座標データを進める
					if( F < 0 )
					{
						xc += rx ;
						if( xc >= rc )
						{
							Dx ++ ;
							xc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dx + 2 ;
						H += 4 * Dx;
					}
					else
					{
						Df = 1 ;

						if( H>=0 )
						{
							Dy--;
							F -= 4 * Dy;
							H -= 4 * Dy - 2 ;
						}
						else
						{
							Dy--;	
							xc += rx ;
							if( xc >= rc )
							{
								Dx ++ ;
								xc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dx - 4 * Dy + 2 ;
							H += 4 * Dx - 4 * Dy + 2 ;
						}
					}
				}

				// 最後の点を描く
				if( Rx_One_Minus == FALSE )
				{
					y1 = Dy + y ; x1 = -Dx + x ;
					DX_LINUX_CIRCLE_PSET ;

					y1 = -Dy + y ; x1 = Dx + x - Rx_One_Minus ;
					DX_LINUX_CIRCLE_PSET ;
				}
			}
			if( VectNum )
			{
				// Uniform の更新
				Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

				// 頂点データのセットアップ
				Graphics_Linux_DeviceState_SetupShaderVertexData(
					GLINUX.Device.State.SetShader,
					&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_NOTEX_2D ].InputInfo,
					VertBuf
				) ;	

				// 描画
				glDrawArrays( GL_POINTS, 0, VectNum ) ;
				GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;
			}
		}
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawTriangle
extern	int		Graphics_Hardware_DrawTriangle_PF( int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *vec ; 
	int Red, Green, Blue ;
	int Flag ;
	int sx1, sx2, sy1, sy2 ;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 色その他ステータスのセット
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// ラインかどうかで処理を分岐
	if( FillFlag )
	{
		GETVERTEX_TRIANGLE( vec ) ;

		vec[ 0 ].color =
		vec[ 1 ].color =
		vec[ 2 ].color = Color ;

		vec[ 0 ].pos.x = ( float )x1 ; vec[ 0 ].pos.y = ( float )y1 ;

		// カリングが有効になっている場合はカリングされないようにする
		switch( GSYS.DrawSetting.CullMode )
		{
		case 0 :	// カリングなし
			vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
			vec[ 2 ].pos.x = ( float )x3 ; vec[ 2 ].pos.y = ( float )y3 ;
			break ;

		case 1 :	// 左回りカリング
			sx1 = x2 - x1 ;		sy1 = y2 - y1 ;
			sx2 = x3 - x1 ;		sy2 = y3 - y1 ;
			if( sx1 * sy2 - sy1 * sx2 > 0 )
			{
				vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
				vec[ 2 ].pos.x = ( float )x3 ; vec[ 2 ].pos.y = ( float )y3 ;
			}
			else
			{
				vec[ 1 ].pos.x = ( float )x3 ; vec[ 1 ].pos.y = ( float )y3 ;
				vec[ 2 ].pos.x = ( float )x2 ; vec[ 2 ].pos.y = ( float )y2 ;
			}
			break ;

		case 2 :	// 右回りカリング
			sx1 = x2 - x1 ;		sy1 = y2 - y1 ;
			sx2 = x3 - x1 ;		sy2 = y3 - y1 ;
			if( sx1 * sy2 - sy1 * sx2 < 0 )
			{
				vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
				vec[ 2 ].pos.x = ( float )x3 ; vec[ 2 ].pos.y = ( float )y3 ;
			}
			else
			{
				vec[ 1 ].pos.x = ( float )x3 ; vec[ 1 ].pos.y = ( float )y3 ;
				vec[ 2 ].pos.x = ( float )x2 ; vec[ 2 ].pos.y = ( float )y2 ;
			}
			break ;
		}

		*( ( DWORD * )&vec[ 0 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 1 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 2 ].pos.z ) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw =
		vec[ 2 ].rhw = 1.0f ;

		ADD3VERTEX_NOTEX
	}
	else
	{
		GETVERTEX_LINETRIANGLE( vec )

		vec[ 0 ].color =
		vec[ 1 ].color =
		vec[ 2 ].color = Color ;

		vec[ 0 ].pos.x = ( float )x1 ; vec[ 0 ].pos.y = ( float )y1 ;
		vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
		vec[ 2 ].pos.x = ( float )x3 ; vec[ 2 ].pos.y = ( float )y3 ;
		*( ( DWORD * )&vec[ 0 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 1 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 2 ].pos.z ) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw =
		vec[ 2 ].rhw = 1.0f ;

		ADD4VERTEX_LINETRIANGLE
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawTriangle3D
extern	int		Graphics_Hardware_DrawTriangle3D_PF( VECTOR Pos1, VECTOR Pos2, VECTOR Pos3, unsigned int Color, int FillFlag, int DrawFlag, RECT *DrawArea )
{
	VERTEX_NOTEX_3D *vec ; 
	int Red, Green, Blue ;
	int Flag ;

	if( DrawFlag )
	{
		if( LINUX_CHECKVALID_HARDWARE == FALSE )
		{
			return -1 ;
		}

		// 描画の準備
		Flag = DX_LINUX_DRAWPREP_DIFFUSERGB | DX_LINUX_DRAWPREP_3D | DX_LINUX_DRAWPREP_FOG | DX_LINUX_DRAWPREP_CULLING ;
		DX_LINUX_DRAWPREP_NOTEX( Flag )

		// 色その他ステータスのセット
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		SETUPCOLOR( Color )

		// ラインかどうかで処理を分岐
		if( FillFlag )
		{
			GETVERTEX_TRIANGLE3D( vec ) ;

			*( ( DWORD * )&vec[ 0 ].b ) =
			*( ( DWORD * )&vec[ 1 ].b ) =
			*( ( DWORD * )&vec[ 2 ].b ) = ( DWORD )Color ;
			vec[ 0 ].pos = Pos1 ;
			vec[ 1 ].pos = Pos2 ;
			vec[ 2 ].pos = Pos3 ;

			ADD3VERTEX_NOTEX3D
		}
		else
		{
			GETVERTEX_LINETRIANGLE3D( vec )

			*( ( DWORD * )&vec[ 0 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 1 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 2 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 3 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 4 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 5 ].b ) = ( DWORD )Color ;

			vec[ 0 ].pos = Pos1 ;
			vec[ 1 ].pos = Pos2 ;
			vec[ 2 ].pos = Pos2 ;
			vec[ 3 ].pos = Pos3 ;
			vec[ 4 ].pos = Pos3 ;
			vec[ 5 ].pos = Pos1 ;

			ADD4VERTEX_LINETRIANGLE3D
		}
	}
	else
	{
		VERTEX_2D TempVect[ 3 ] ;
		float dleft, dright, dtop, dbottom ;

		dleft   =  100000000.0f ;
		dright  = -100000000.0f ;
		dtop    =  100000000.0f ;
		dbottom = -100000000.0f ;

		// スクリーン座標に変換
		TempVect[0].pos.x = Pos1.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + Pos1.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + Pos1.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0]  ;
		TempVect[0].pos.y = Pos1.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + Pos1.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + Pos1.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1]  ;
		TempVect[0].pos.z = Pos1.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + Pos1.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + Pos1.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2]  ;
		TempVect[0].rhw   = Pos1.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + Pos1.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + Pos1.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3]  ;

		TempVect[1].pos.x = Pos2.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + Pos2.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + Pos2.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0]  ;
		TempVect[1].pos.y = Pos2.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + Pos2.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + Pos2.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1]  ;
		TempVect[1].pos.z = Pos2.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + Pos2.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + Pos2.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2]  ;
		TempVect[1].rhw   = Pos2.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + Pos2.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + Pos2.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3]  ;

		TempVect[2].pos.x = Pos3.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + Pos3.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + Pos3.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0]  ;
		TempVect[2].pos.y = Pos3.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + Pos3.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + Pos3.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1]  ;
		TempVect[2].pos.z = Pos3.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + Pos3.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + Pos3.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2]  ;
		TempVect[2].rhw   = Pos3.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + Pos3.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + Pos3.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3]  ;

		TempVect[0].rhw = 1.0F / TempVect[0].rhw ;
		TempVect[0].pos.x *= TempVect[0].rhw ;
		TempVect[0].pos.y *= TempVect[0].rhw ;
		TempVect[0].pos.z *= TempVect[0].rhw ;

		TempVect[1].rhw = 1.0F / TempVect[1].rhw ;
		TempVect[1].pos.x *= TempVect[1].rhw ;
		TempVect[1].pos.y *= TempVect[1].rhw ;
		TempVect[1].pos.z *= TempVect[1].rhw ;

		TempVect[2].rhw = 1.0F / TempVect[2].rhw ;
		TempVect[2].pos.x *= TempVect[2].rhw ;
		TempVect[2].pos.y *= TempVect[2].rhw ;
		TempVect[2].pos.z *= TempVect[2].rhw ;

		if( dright  < TempVect[ 0 ].pos.x ) dright  = TempVect[ 0 ].pos.x ;
		if( dright  < TempVect[ 1 ].pos.x ) dright  = TempVect[ 1 ].pos.x ;
		if( dright  < TempVect[ 2 ].pos.x ) dright  = TempVect[ 2 ].pos.x ;

		if( dleft   > TempVect[ 0 ].pos.x ) dleft   = TempVect[ 0 ].pos.x ;
		if( dleft   > TempVect[ 1 ].pos.x ) dleft   = TempVect[ 1 ].pos.x ;
		if( dleft   > TempVect[ 2 ].pos.x ) dleft   = TempVect[ 2 ].pos.x ;

		if( dbottom < TempVect[ 0 ].pos.y ) dbottom = TempVect[ 0 ].pos.y ;
		if( dbottom < TempVect[ 1 ].pos.y ) dbottom = TempVect[ 1 ].pos.y ;
		if( dbottom < TempVect[ 2 ].pos.y ) dbottom = TempVect[ 2 ].pos.y ;

		if( dtop    > TempVect[ 0 ].pos.y ) dtop    = TempVect[ 0 ].pos.y ;
		if( dtop    > TempVect[ 1 ].pos.y ) dtop    = TempVect[ 1 ].pos.y ;
		if( dtop    > TempVect[ 2 ].pos.y ) dtop    = TempVect[ 2 ].pos.y ;

		DrawArea->left   = _FTOL( dleft   ) - 1 ;
		DrawArea->right  = _FTOL( dright  ) + 1 ;
		DrawArea->top    = _FTOL( dtop    ) - 1 ;
		DrawArea->bottom = _FTOL( dbottom ) + 1 ;
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawQuadrangle
extern	int		Graphics_Hardware_DrawQuadrangle_PF( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *vec ;
	int Red, Green, Blue ;
	int Flag ;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 色その他ステータスのセット
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// 塗りつぶしかどうかで処理を分岐
	if( FillFlag )
	{
		// 塗りつぶしの場合

		// 頂点データのセット
		GETVERTEX_QUAD( vec )

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;
		vec[ 2 ].color = Color ;
		vec[ 3 ].color = Color ;
		vec[ 4 ].color = Color ;
		vec[ 5 ].color = Color ;
		
		vec[ 0 ].pos.x = ( float )x1 ; vec[ 0 ].pos.y = ( float )y1 ;
		vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
		vec[ 2 ].pos.x = ( float )x4 ; vec[ 2 ].pos.y = ( float )y4 ;
		vec[ 3 ].pos.x = ( float )x3 ; vec[ 3 ].pos.y = ( float )y3 ;
		vec[ 4 ].pos.x = ( float )x4 ; vec[ 4 ].pos.y = ( float )y4 ;
		vec[ 5 ].pos.x = ( float )x2 ; vec[ 5 ].pos.y = ( float )y2 ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 2 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 3 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 4 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 5 ].pos.z) = drawz ;

		vec[0].rhw = 1.0f ;
		vec[1].rhw = 1.0f ;
		vec[2].rhw = 1.0f ;
		vec[3].rhw = 1.0f ;
		vec[4].rhw = 1.0f ;
		vec[5].rhw = 1.0f ;

		// 頂点データの出力
		ADD4VERTEX_NOTEX
	}
	else
	{
		// 塗りつぶしでは無い場合

		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = ( float )( x1 ) ;
		vec[ 0 ].pos.y = ( float )( y1 ) ;

		vec[ 1 ].pos.x = ( float )( x2 ) ;
		vec[ 1 ].pos.y = ( float )( y2 ) ;

		// 頂点の追加
		ADD4VERTEX_LINE


		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = ( float )( x2 ) ;
		vec[ 0 ].pos.y = ( float )( y2 ) ;

		vec[ 1 ].pos.x = ( float )( x3 ) ;
		vec[ 1 ].pos.y = ( float )( y3 ) ;

		// 頂点の追加
		ADD4VERTEX_LINE


		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = ( float )( x3 ) ;
		vec[ 0 ].pos.y = ( float )( y3 ) ;

		vec[ 1 ].pos.x = ( float )( x4 ) ;
		vec[ 1 ].pos.y = ( float )( y4 ) ;

		// 頂点の追加
		ADD4VERTEX_LINE


		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = ( float )( x4 ) ;
		vec[ 0 ].pos.y = ( float )( y4 ) ;

		vec[ 1 ].pos.x = ( float )( x1 ) ;
		vec[ 1 ].pos.y = ( float )( y1 ) ;

		// 頂点の追加
		ADD4VERTEX_LINE
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawQuadrangle
extern	int		Graphics_Hardware_DrawQuadrangleF_PF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *vec ;
	int Red, Green, Blue ;
	int Flag ;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// 色その他ステータスのセット
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// 塗りつぶしかどうかで処理を分岐
	if( FillFlag )
	{
		// 塗りつぶしの場合

		// 頂点データのセット
		GETVERTEX_QUAD( vec )

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;
		vec[ 2 ].color = Color ;
		vec[ 3 ].color = Color ;
		vec[ 4 ].color = Color ;
		vec[ 5 ].color = Color ;
		
		vec[ 0 ].pos.x = x1 ; vec[ 0 ].pos.y = y1 ;
		vec[ 1 ].pos.x = x2 ; vec[ 1 ].pos.y = y2 ;
		vec[ 2 ].pos.x = x4 ; vec[ 2 ].pos.y = y4 ;
		vec[ 3 ].pos.x = x3 ; vec[ 3 ].pos.y = y3 ;
		vec[ 4 ].pos.x = x4 ; vec[ 4 ].pos.y = y4 ;
		vec[ 5 ].pos.x = x2 ; vec[ 5 ].pos.y = y2 ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 2 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 3 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 4 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 5 ].pos.z) = drawz ;

		vec[0].rhw = 1.0f ;
		vec[1].rhw = 1.0f ;
		vec[2].rhw = 1.0f ;
		vec[3].rhw = 1.0f ;
		vec[4].rhw = 1.0f ;
		vec[5].rhw = 1.0f ;

		// 頂点データの出力
		ADD4VERTEX_NOTEX
	}
	else
	{
		// 塗りつぶしでは無い場合

		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = x1 ;
		vec[ 0 ].pos.y = y1 ;

		vec[ 1 ].pos.x = x2 ;
		vec[ 1 ].pos.y = y2 ;

		// 頂点の追加
		ADD4VERTEX_LINE


		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = x2 ;
		vec[ 0 ].pos.y = y2 ;

		vec[ 1 ].pos.x = x3 ;
		vec[ 1 ].pos.y = y3 ;

		// 頂点の追加
		ADD4VERTEX_LINE


		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = x3 ;
		vec[ 0 ].pos.y = y3 ;

		vec[ 1 ].pos.x = x4 ;
		vec[ 1 ].pos.y = y4 ;

		// 頂点の追加
		ADD4VERTEX_LINE


		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw =
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = x4 ;
		vec[ 0 ].pos.y = y4 ;

		vec[ 1 ].pos.x = x1 ;
		vec[ 1 ].pos.y = y1 ;

		// 頂点の追加
		ADD4VERTEX_LINE
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawPixel
extern	int		Graphics_Hardware_DrawPixel_PF( int x, int y, unsigned int Color )
{
	VERTEX_NOTEX_2D *VectData ;
	int Red, Green, Blue, Flag ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// 色その他ステータスのセット
	GETVERTEX_POINT( VectData ) ;

	if( GLINUX.Device.DrawInfo.DiffuseColor == 0xffffffff )
	{
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		if( GSYS.HardInfo.UseVertexColorBGRAFormat )
		{
			VectData->color = 0xff000000 | ( Red << 16 ) | ( Green << 8 ) | ( Blue ) ;
		}
		else
		{
			VectData->color = 0xff000000 | ( Red ) | ( Green << 8 ) | ( Blue << 16 ) ;
		}
	}
	else
	{
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		SETUPCOLOR( VectData->color )
	}

	// 座標のセット
	VectData->pos.x = ( float )x + 0.1f ;
	VectData->pos.y = ( float )y + 0.1f ;
	VectData->pos.z = GSYS.DrawSetting.DrawZ ;
	VectData->rhw   = 1.0f ;

	// 頂点の追加
	ADD4VERTEX_POINT

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawPixel3D
extern	int		Graphics_Hardware_DrawPixel3D_PF( VECTOR Pos, unsigned int Color, int DrawFlag, RECT *DrawArea )
{
	VERTEX_NOTEX_3D *VectData ; 
	int Red, Green, Blue ;
	int Flag ;

	if( DrawFlag )
	{
		if( LINUX_CHECKVALID_HARDWARE == FALSE )
		{
			return -1 ;
		}

		// 描画の準備
		Flag = DX_LINUX_DRAWPREP_DIFFUSERGB | DX_LINUX_DRAWPREP_3D | DX_LINUX_DRAWPREP_FOG ;
		DX_LINUX_DRAWPREP_NOTEX( Flag )

		// 色その他ステータスのセット
		GETVERTEX_POINT3D( VectData ) ;

		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		if( GLINUX.Device.DrawInfo.DiffuseColor == 0xffffffff )
		{
			if( GSYS.HardInfo.UseVertexColorBGRAFormat )
			{
				*( ( DWORD * )&VectData->b ) = 0xff000000 | ( Red << 16 ) | ( Green << 8 ) | ( Blue ) ;
			}
			else
			{
				*( ( DWORD * )&VectData->b ) = 0xff000000 | ( Red ) | ( Green << 8 ) | ( Blue << 16 ) ;
			}
		}
		else
		{
			SETUPCOLOR( *( ( DWORD * )&VectData->b ) )
		}

		// 座標のセット
		VectData->pos = Pos ;

		// 頂点の追加
		ADD4VERTEX_POINT3D
	}
	else
	{
		VERTEX_2D TempVect ;

		// スクリーン座標に変換
		TempVect.pos.x = Pos.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][0] + Pos.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][0] + Pos.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][0] + GSYS.DrawSetting.Blend3DMatrixF.m[3][0]  ;
		TempVect.pos.y = Pos.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][1] + Pos.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][1] + Pos.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][1] + GSYS.DrawSetting.Blend3DMatrixF.m[3][1]  ;
		TempVect.pos.z = Pos.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][2] + Pos.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][2] + Pos.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][2] + GSYS.DrawSetting.Blend3DMatrixF.m[3][2]  ;
		TempVect.rhw   = Pos.x * GSYS.DrawSetting.Blend3DMatrixF.m[0][3] + Pos.y * GSYS.DrawSetting.Blend3DMatrixF.m[1][3] + Pos.z * GSYS.DrawSetting.Blend3DMatrixF.m[2][3] + GSYS.DrawSetting.Blend3DMatrixF.m[3][3]  ;

		TempVect.rhw = 1.0F / TempVect.rhw ;
		TempVect.pos.x *= TempVect.rhw ;
		TempVect.pos.y *= TempVect.rhw ;
		TempVect.pos.z *= TempVect.rhw ;

		DrawArea->left   = _FTOL( TempVect.pos.x ) - 1 ;
		DrawArea->right  = _FTOL( TempVect.pos.x ) + 1 ;
		DrawArea->top    = _FTOL( TempVect.pos.y ) - 1 ;
		DrawArea->bottom = _FTOL( TempVect.pos.y ) + 1 ;
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawPixelSet
extern	int		Graphics_Hardware_DrawPixelSet_PF( const POINTDATA *PointData, int Num )
{
	VERTEX_NOTEX_2D *VectData ;
	int i ;
	COLORDATA ColorData ;
	int MaxRed, MaxGreen, MaxBlue ; 
	unsigned int Color ;
	int Flag ;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ベクトル基本ステータスを取得
	ColorData = *( ( COLORDATA * )NS_GetDispColorData() ) ;
	
	MaxRed		= ( 1 << ColorData.RedWidth	  ) - 1 ; 
	MaxGreen	= ( 1 << ColorData.GreenWidth ) - 1 ;
	MaxBlue		= ( 1 << ColorData.BlueWidth  ) - 1 ; 

	for( i = 0 ; i < Num ; i ++, PointData ++ )
	{
		GETVERTEX_POINT( VectData ) ;

		Color = PointData->color ;
		VectData->color = ( PointData->pal << 24 ) |
						( ( ( ( ( ( Color & ColorData.RedMask	) >> ColorData.RedLoc	) << 8 ) - 1 ) / MaxRed		)       ) |
						( ( ( ( ( ( Color & ColorData.GreenMask	) >> ColorData.GreenLoc	) << 8 ) - 1 ) / MaxGreen	) << 8  ) |
						( ( ( ( ( ( Color & ColorData.BlueMask	) >> ColorData.BlueLoc	) << 8 ) - 1 ) / MaxBlue	) << 16 ) ; 

		VectData->pos.x = ( float )PointData->x + 0.1f ;
		VectData->pos.y = ( float )PointData->y + 0.1f ;
		*((DWORD *)&VectData->pos.z) = drawz ;
		VectData->rhw    = 1.0f ;

		// 頂点の追加
		ADD4VERTEX_POINT
	}

	// 終了
	return 0;
}

// ハードウエアアクセラレータ使用版 DrawLineSet
extern	int		Graphics_Hardware_DrawLineSet_PF( const LINEDATA *LineData, int Num )
{
	int i ;
	VERTEX_NOTEX_2D *VectData ;
	COLORDATA ColorData ;
	int MaxRed, MaxGreen, MaxBlue ; 
	unsigned int Color ;
	int Flag ;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ベクトル基本ステータスを取得
	ColorData = *( ( COLORDATA * )NS_GetDispColorData() ) ;
	
	MaxRed		= ( 1 << ColorData.RedWidth	) - 1 ; 
	MaxGreen	= ( 1 << ColorData.GreenWidth ) - 1 ;
	MaxBlue		= ( 1 << ColorData.BlueWidth ) - 1 ; 

	for( i = 0 ; i < Num ; i ++, LineData ++ )
	{
		GETVERTEX_LINE( VectData ) ;

		Color = LineData->color ;
		VectData->color = ( LineData->pal << 24 ) |
						( ( ( ( ( ( Color & ColorData.RedMask	) >> ColorData.RedLoc	) << 8 ) - 1 ) / MaxRed		)       ) |
						( ( ( ( ( ( Color & ColorData.GreenMask	) >> ColorData.GreenLoc	) << 8 ) - 1 ) / MaxGreen	) << 8  ) |
						( ( ( ( ( ( Color & ColorData.BlueMask	) >> ColorData.BlueLoc	) << 8 ) - 1 ) / MaxBlue	) << 16 ) ; 
		VectData[1].color = VectData[0].color ;
		*((DWORD *)&VectData[0].pos.z) = drawz ;
		*((DWORD *)&VectData[1].pos.z) = drawz ;
		VectData[0].rhw   =
		VectData[1].rhw   = 1.0f ;
 
		// 座標のセット＆ウインドウ直接描画時補正
		VectData[0].pos.x = ( float )LineData->x1 ;
		VectData[0].pos.y = ( float )LineData->y1 ;
		VectData[1].pos.x = ( float )LineData->x2 ;
		VectData[1].pos.y = ( float )LineData->y2 ;

		// 座標の補正
		if( LineData->x1 == LineData->x2 ) VectData[ 1 ].pos.y += LineData->y2 > LineData->y1 ? -0.1F : 0.1F ;
		if( LineData->y1 == LineData->y2 ) VectData[ 1 ].pos.x += LineData->x2 > LineData->x1 ? -0.1F : 0.1F ;

		// 頂点の追加
		ADD4VERTEX_LINE
	}

	// 終了
	return 0 ;
}

// ハードウエアアクセラレータ使用版 DrawBoxSet
extern	int		Graphics_Hardware_DrawBoxSet_PF( const RECTDATA *RectData, int Num )
{
	int i ;
	VERTEX_NOTEX_2D *VectData ;
	COLORDATA ColorData ;
	int MaxRed, MaxGreen, MaxBlue ; 
	unsigned int Color ;
	int Flag ;
	DWORD drawz ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Flag = DX_LINUX_DRAWPREP_DIFFUSERGB ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	// Ｚバッファに書き込むＺ値をセットする
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ベクトル基本ステータスを取得
	ColorData = *( ( COLORDATA * )NS_GetDispColorData() ) ;
	
	if( ColorData.AlphaMask == 0xff000000 &&
		ColorData.RedMask   == 0x000000ff &&
		ColorData.GreenMask == 0x0000ff00 &&
		ColorData.BlueMask  == 0x00ff0000 )
	{
		for( i = 0 ; i < Num ; i ++, RectData ++ )
		{
			int x1, y1, x2, y2 ;

			if( RectData->x1 > RectData->x2 )
			{
				x1 = RectData->x2 ;
				x2 = RectData->x1 ;
			}
			else
			{
				x1 = RectData->x1 ;
				x2 = RectData->x2 ;
			}

			if( RectData->y1 > RectData->y2 )
			{
				y1 = RectData->y2 ;
				y2 = RectData->y1 ;
			}
			else
			{
				y1 = RectData->y1 ;
				y2 = RectData->y2 ;
			}

			GETVERTEX_QUAD( VectData ) ;

			Color = ( RectData->color & 0x00ffffff ) | ( RectData->pal << 24 ) ;
			VectData[0].color = Color ;
			VectData[1].color = Color ;
			VectData[2].color = Color ;
			VectData[3].color = Color ;
			VectData[4].color = Color ;
			VectData[5].color = Color ;
			VectData[0].pos.x = ( float )RectData->x1 ;
			VectData[0].pos.y = ( float )RectData->y1 ;
			VectData[1].pos.x = ( float )RectData->x2 ;
			VectData[1].pos.y = ( float )RectData->y1 ;
			VectData[2].pos.x = ( float )RectData->x1 ;
			VectData[2].pos.y = ( float )RectData->y2 ;
			VectData[3].pos.x = ( float )RectData->x2 ;
			VectData[3].pos.y = ( float )RectData->y2 ;
			VectData[4].pos.x = ( float )RectData->x1 ;
			VectData[4].pos.y = ( float )RectData->y2 ;
			VectData[5].pos.x = ( float )RectData->x2 ;
			VectData[5].pos.y = ( float )RectData->y1 ;
			*((DWORD *)&VectData[0].pos.z) = drawz ;
			*((DWORD *)&VectData[1].pos.z) = drawz ;
			*((DWORD *)&VectData[2].pos.z) = drawz ;
			*((DWORD *)&VectData[3].pos.z) = drawz ;
			*((DWORD *)&VectData[4].pos.z) = drawz ;
			*((DWORD *)&VectData[5].pos.z) = drawz ;
			VectData[0].rhw   = 1.0f ;
			VectData[1].rhw   = 1.0f ;
			VectData[2].rhw   = 1.0f ;
			VectData[3].rhw   = 1.0f ;
			VectData[4].rhw   = 1.0f ;
			VectData[5].rhw   = 1.0f ;

			// 頂点の追加
			ADD4VERTEX_NOTEX
		}
	}
	else
	{
		MaxRed		= ( 1 << ColorData.RedWidth	  ) - 1 ; 
		MaxGreen	= ( 1 << ColorData.GreenWidth ) - 1 ;
		MaxBlue		= ( 1 << ColorData.BlueWidth  ) - 1 ; 

		for( i = 0 ; i < Num ; i ++, RectData ++ )
		{
			int x1, y1, x2, y2 ;

			if( RectData->x1 > RectData->x2 )
			{
				x1 = RectData->x2 ;
				x2 = RectData->x1 ;
			}
			else
			{
				x1 = RectData->x1 ;
				x2 = RectData->x2 ;
			}

			if( RectData->y1 > RectData->y2 )
			{
				y1 = RectData->y2 ;
				y2 = RectData->y1 ;
			}
			else
			{
				y1 = RectData->y1 ;
				y2 = RectData->y2 ;
			}

			GETVERTEX_QUAD( VectData ) ;

			Color = RectData->color ;
			Color = ( RectData->pal << 24 ) |
							( ( ( ( ( ( Color & ColorData.RedMask	) >> ColorData.RedLoc	) << 8 ) - 1 ) / MaxRed		)       ) |
							( ( ( ( ( ( Color & ColorData.GreenMask	) >> ColorData.GreenLoc	) << 8 ) - 1 ) / MaxGreen	) << 8  ) |
							( ( ( ( ( ( Color & ColorData.BlueMask	) >> ColorData.BlueLoc	) << 8 ) - 1 ) / MaxBlue	) << 16 ) ; 
			VectData[0].color = Color ;
			VectData[1].color = Color ;
			VectData[2].color = Color ;
			VectData[3].color = Color ;
			VectData[4].color = Color ;
			VectData[5].color = Color ;
			VectData[0].pos.x = ( float )RectData->x1 ;
			VectData[0].pos.y = ( float )RectData->y1 ;
			VectData[1].pos.x = ( float )RectData->x2 ;
			VectData[1].pos.y = ( float )RectData->y1 ;
			VectData[2].pos.x = ( float )RectData->x1 ;
			VectData[2].pos.y = ( float )RectData->y2 ;
			VectData[3].pos.x = ( float )RectData->x2 ;
			VectData[3].pos.y = ( float )RectData->y2 ;
			VectData[4].pos.x = ( float )RectData->x1 ;
			VectData[4].pos.y = ( float )RectData->y2 ;
			VectData[5].pos.x = ( float )RectData->x2 ;
			VectData[5].pos.y = ( float )RectData->y1 ;
			*((DWORD *)&VectData[0].pos.z) = drawz ;
			*((DWORD *)&VectData[1].pos.z) = drawz ;
			*((DWORD *)&VectData[2].pos.z) = drawz ;
			*((DWORD *)&VectData[3].pos.z) = drawz ;
			*((DWORD *)&VectData[4].pos.z) = drawz ;
			*((DWORD *)&VectData[5].pos.z) = drawz ;
			VectData[0].rhw   = 1.0f ;
			VectData[1].rhw   = 1.0f ;
			VectData[2].rhw   = 1.0f ;
			VectData[3].rhw   = 1.0f ;
			VectData[4].rhw   = 1.0f ;
			VectData[5].rhw   = 1.0f ;

			// 頂点の追加
			ADD4VERTEX_NOTEX
		}
	}

	// 終了
	return 0 ;
}














// DrawPrimitive3D 系の共通の準備処理を行う関数
__inline int Graphics_Linux_DrawPrimitive3DPreparation( int AddFlag, IMAGEDATA *Image, int TransFlag, int TextureNo = 0 )
{
	int Flag ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Flag = TransFlag | DX_LINUX_DRAWPREP_3D | DX_LINUX_DRAWPREP_FOG | DX_LINUX_DRAWPREP_TEXADDRESS | DX_LINUX_DRAWPREP_CULLING | AddFlag ;
	if( Image )
	{
		DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ TextureNo ].Tex->PF->Texture, Flag )
	}
	else
	{
		int WhiteHandle ;

		WhiteHandle = Graphics_Image_GetWhiteTexHandle() ;
		if( GRAPHCHK( WhiteHandle, Image ) )
		{
			return -1 ;
		}
//		DX_LINUX_DRAWPREP_NOTEX( Flag )
		DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )
	}

	return 0 ;
}

extern	int		Graphics_Hardware_DrawPrimitive_PF( const VERTEX_3D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX_3D *VertP ;
	unsigned char tmp ;
	int BackupUse3DVertex ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	if( Graphics_Linux_DrawPrimitive3DPreparation( 0, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	BackupUse3DVertex = GLINUX.Device.DrawInfo.Use3DVertex ;
	GLINUX.Device.DrawInfo.Use3DVertex = 1 ;
	Graphics_Linux_DeviceState_NormalDrawSetup() ;
	GLINUX.Device.DrawInfo.Use3DVertex = BackupUse3DVertex ;

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_3D ].InputInfo,
		Vertex
	) ;	

	// 描画
	glDrawArrays( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], 0, VertexNum ) ;
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_DrawIndexedPrimitive_PF( const VERTEX_3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX_3D *VertP ;
	unsigned char tmp ;
	int BackupUse3DVertex ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	if( Graphics_Linux_DrawPrimitive3DPreparation( 0, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	BackupUse3DVertex = GLINUX.Device.DrawInfo.Use3DVertex ;
	GLINUX.Device.DrawInfo.Use3DVertex = 1 ;
	Graphics_Linux_DeviceState_NormalDrawSetup() ;
	GLINUX.Device.DrawInfo.Use3DVertex = BackupUse3DVertex ;

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_3D ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_SHORT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_Draw32bitIndexedPrimitive_PF( const VERTEX_3D *Vertex, int VertexNum, const DWORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX_3D *VertP ;
	unsigned char tmp ;
	int BackupUse3DVertex ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	if( Graphics_Linux_DrawPrimitive3DPreparation( 0, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	BackupUse3DVertex = GLINUX.Device.DrawInfo.Use3DVertex ;
	GLINUX.Device.DrawInfo.Use3DVertex = 1 ;
	Graphics_Linux_DeviceState_NormalDrawSetup() ;
	GLINUX.Device.DrawInfo.Use3DVertex = BackupUse3DVertex ;

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_3D ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_INT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_DrawPrimitiveLight_PF( const VERTEX3D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX3D *VertP ;
	unsigned char tmp ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	if( Graphics_Linux_DrawPrimitive3DPreparation( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_SPECULAR, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// シェーダーをセット
	if( Graphics_Linux_Shader_Normal3DDraw_Setup() == FALSE )
	{
		return -1 ;
	}

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_3D_LIGHT ].InputInfo,
		Vertex
	) ;	

	// 描画
	glDrawArrays( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], 0, VertexNum ) ;
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_DrawIndexedPrimitiveLight_PF( const VERTEX3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX3D *VertP ;
	unsigned char tmp ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	if( Graphics_Linux_DrawPrimitive3DPreparation( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_SPECULAR, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// シェーダーをセット
	if( Graphics_Linux_Shader_Normal3DDraw_Setup() == FALSE )
	{
		return -1 ;
	}

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_3D_LIGHT ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_SHORT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_Draw32bitIndexedPrimitiveLight_PF( const VERTEX3D *Vertex, int VertexNum, const DWORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX3D *VertP ;
	unsigned char tmp ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	if( Graphics_Linux_DrawPrimitive3DPreparation( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_SPECULAR, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// シェーダーをセット
	if( Graphics_Linux_Shader_Normal3DDraw_Setup() == FALSE )
	{
		return -1 ;
	}

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_3D_LIGHT ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_INT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_DrawPrimitiveLight_UseVertexBuffer_PF( 
	VERTEXBUFFERHANDLEDATA *VertexBuffer,
	int						PrimitiveType,
	int						StartVertex,
	int						UseVertexNum,
	IMAGEDATA *				Image,
	int						TransFlag 
)
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// VERTEX3D構造体のみ対応
	if( VertexBuffer->Type != DX_VERTEX_TYPE_NORMAL_3D )
	{
		return -1 ;
	}

	if( Graphics_Linux_DrawPrimitive3DPreparation( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_SPECULAR, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	// 頂点シェーダーをセット
	if( Graphics_Linux_Shader_Normal3DDraw_Setup() == FALSE )
	{
		return -1 ;
	}

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_3D_LIGHT ].InputInfo,
		NULL,
		VertexBuffer->PF->VertexBuffer
	) ;

	// 描画
	glDrawArrays( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], StartVertex, UseVertexNum );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_DrawIndexedPrimitiveLight_UseVertexBuffer_PF(
	VERTEXBUFFERHANDLEDATA *	VertexBuffer,
	INDEXBUFFERHANDLEDATA *		IndexBuffer,
	int							PrimitiveType,
	int							BaseVertex,
	int							StartVertex,
	int							UseVertexNum,
	int							StartIndex,
	int							UseIndexNum,
	IMAGEDATA *					Image,
	int							TransFlag
)
{
	GLenum IndexFormat ;
//	int    IndexSize ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 現状では StartIndex が 0 以外の場合は描画できない
	if( StartIndex != 0 )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// VERTEX3D構造体のみ対応
	if( VertexBuffer->Type != DX_VERTEX_TYPE_NORMAL_3D )
	{
		return -1 ;
	}

	switch( IndexBuffer->Type )
	{
	default :
	case DX_INDEX_TYPE_16BIT :
		IndexFormat = GL_UNSIGNED_SHORT ;
		break ;

	case DX_INDEX_TYPE_32BIT :
		IndexFormat = GL_UNSIGNED_INT ;
		break ;
	}
//	IndexSize = IndexFormat == GL_UNSIGNED_SHORT ? 2 : 4 ;

	if( Graphics_Linux_DrawPrimitive3DPreparation( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_SPECULAR, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	// 頂点シェーダーをセット
	if( Graphics_Linux_Shader_Normal3DDraw_Setup() == FALSE )
	{
		return -1 ;
	}


	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_3D_LIGHT ].InputInfo,
		NULL,
		VertexBuffer->PF->VertexBuffer
	) ;

	// 描画
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer->PF->IndexBuffer ) ;

	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], UseIndexNum, IndexFormat, NULL );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) ;

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_DrawPrimitive2D_PF( VERTEX_2D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag, int BillboardFlag, int Is3D, int ReverseXFlag, int ReverseYFlag, int TextureNo, int IsShadowMap )
{
	int Flag ;
	int i ;
	VERTEX_2D *Vert ;
	int SmHandle ;
	SHADOWMAPDATA *ShadowMap ;
	VERTEX_2D *VertP ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	if( BillboardFlag == TRUE )
	{
		Flag = TransFlag | ( Is3D ? DX_LINUX_DRAWPREP_3D | DX_LINUX_DRAWPREP_FOG : 0 ) | DX_LINUX_DRAWPREP_TEXADDRESS ;
	}
	else
	{
		Flag = TransFlag | ( Is3D ? DX_LINUX_DRAWPREP_3D | DX_LINUX_DRAWPREP_FOG : 0 ) | DX_LINUX_DRAWPREP_TEXADDRESS | DX_LINUX_DRAWPREP_CULLING ;
	}

	if( IsShadowMap )
	{
		SmHandle = ( int )( DWORD_PTR )Image ;
		if( !SHADOWMAPCHKFULL( SmHandle, ShadowMap ) )
		{
			Flag |= DX_LINUX_DRAWPREP_TEXTURE ;
			Graphics_Linux_DrawSetting_SetTexture( &ShadowMap->PF->Texture ) ;

			if( GSYS.ChangeSettingFlag ||
				GLINUX.Device.DrawSetting.DrawPrepAlwaysFlag != FALSE ||
				GLINUX.Device.DrawSetting.DrawPrepParamFlag != Flag )
			{
				Graphics_Linux_DrawPreparation( Flag ) ;
			}
		}
	}
	else
	if( Image )
	{
		DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ TextureNo ].Tex->PF->Texture, Flag )
	}
	else
	{
		DX_LINUX_DRAWPREP_NOTEX( Flag )
	}

	// ビルボードの場合はＵＶ値をサーフェスデータから取得する
	if( BillboardFlag == TRUE )
	{
		float tu1, tv1, tu2, tv2 ;
		int Num ;
		
		Vert = Vertex ;
		Num = VertexNum / 6 ;
		tu1 = Image->Hard.Draw[ 0 ].Vertex[ 0 ].u ;
		tv1 = Image->Hard.Draw[ 0 ].Vertex[ 0 ].v ;
		tu2 = Image->Hard.Draw[ 0 ].Vertex[ 3 ].u ;
		tv2 = Image->Hard.Draw[ 0 ].Vertex[ 3 ].v ;
		for( i = 0 ; i < Num ; i ++, Vert += 6 )
		{
			if( ReverseXFlag )
			{
				Vert[ 0 ].u = tu2 ;
				Vert[ 1 ].u = tu1 ;
				Vert[ 2 ].u = tu2 ;
				Vert[ 3 ].u = tu1 ;
			}
			else
			{
				Vert[ 0 ].u = tu1 ;
				Vert[ 1 ].u = tu2 ;
				Vert[ 2 ].u = tu1 ;
				Vert[ 3 ].u = tu2 ;
			}
			if( ReverseYFlag )
			{
				Vert[ 0 ].v = tv2 ;
				Vert[ 1 ].v = tv2 ;
				Vert[ 2 ].v = tv1 ;
				Vert[ 3 ].v = tv1 ;
			}
			else
			{
				Vert[ 0 ].v = tv1 ;
				Vert[ 1 ].v = tv1 ;
				Vert[ 2 ].v = tv2 ;
				Vert[ 3 ].v = tv2 ;
			}

			Vert[ 4 ].u = Vert[ 2 ].u ;
			Vert[ 4 ].v = Vert[ 2 ].v ;
			Vert[ 5 ].u = Vert[ 1 ].u ;
			Vert[ 5 ].v = Vert[ 1 ].v ;
		}
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			VertP->color =	(   VertP->color & 0xff00ff00 ) |
							( ( VertP->color & 0x00ff0000 ) >> 16 ) |
							( ( VertP->color & 0x000000ff ) << 16 ) ;
		}
	}

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_2D ].InputInfo,
		Vertex
	) ;	

	// 描画
	glDrawArrays( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], 0, VertexNum ) ;
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			VertP->color =	(   VertP->color & 0xff00ff00 ) |
							( ( VertP->color & 0x00ff0000 ) >> 16 ) |
							( ( VertP->color & 0x000000ff ) << 16 ) ;
		}
	}

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_DrawPrimitive2DUser_PF( const VERTEX2D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag, int Is3D, int ReverseXFlag, int ReverseYFlag, int TextureNo )
{
	int Flag ;
	int i ;
	VERTEX2D *VertP ;
	unsigned char tmp ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Flag = TransFlag | ( Is3D ? DX_LINUX_DRAWPREP_3D | DX_LINUX_DRAWPREP_FOG : 0 ) | DX_LINUX_DRAWPREP_TEXADDRESS | DX_LINUX_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ TextureNo ].Tex->PF->Texture, Flag )
	}
	else
	{
		DX_LINUX_DRAWPREP_NOTEX( Flag )
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_2D ].InputInfo,
		Vertex
	) ;	

	// 描画
	glDrawArrays( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], 0, VertexNum ) ;
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_DrawIndexedPrimitive2DUser_PF( const VERTEX2D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int Flag ;
	int i ;
	VERTEX2D *VertP ;
	unsigned char tmp ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Flag = TransFlag | DX_LINUX_DRAWPREP_TEXADDRESS | DX_LINUX_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )
	}
	else
	{
		DX_LINUX_DRAWPREP_NOTEX( Flag )
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_2D ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_SHORT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// 終了
	return 0 ;
}

extern	int		Graphics_Hardware_Draw32bitIndexedPrimitive2DUser_PF( const VERTEX2D *Vertex, int VertexNum, const DWORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int Flag ;
	int i ;
	VERTEX2D *VertP ;
	unsigned char tmp ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Flag = TransFlag | DX_LINUX_DRAWPREP_TEXADDRESS | DX_LINUX_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_LINUX_DRAWPREP_TEX( Image->Orig, &Image->Hard.Draw[ 0 ].Tex->PF->Texture, Flag )
	}
	else
	{
		DX_LINUX_DRAWPREP_NOTEX( Flag )
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_2D ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_INT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// 終了
	return 0 ;
}












// シェーダー描画用描画前セットアップ関数
extern void Graphics_Linux_DrawPreparationToShader( int ParamFlag, int Is2D )
{
	int							Flag ;

	// 基本的なセットアップ処理
	Flag = ParamFlag | DX_LINUX_DRAWPREP_SPECULAR | DX_LINUX_DRAWPREP_TEXADDRESS | DX_LINUX_DRAWPREP_NOBLENDSETTING | DX_LINUX_DRAWPREP_CULLING | ( Is2D ? 0 : DX_LINUX_DRAWPREP_3D ) ;
	DX_LINUX_DRAWPREP_NOTEX( Flag )

	Graphics_Linux_DeviceState_SetBlendMode(
		GSYS.DrawSetting.BlendMode,
		GSYS.DrawSetting.BlendEnable,
		GSYS.DrawSetting.BlendRGBSrc,
		GSYS.DrawSetting.BlendRGBDest,
		GSYS.DrawSetting.BlendRGBOp,
		GSYS.DrawSetting.BlendASrc,
		GSYS.DrawSetting.BlendADest,
		GSYS.DrawSetting.BlendAOp,
		GLINUX.Device.DrawSetting.NotWriteAlphaChannelFlag
	) ;
}

// シェーダーを使って２Ｄプリミティブを描画する
extern	int		Graphics_Hardware_DrawPrimitive2DToShader_PF(        const VERTEX2DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */ )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Graphics_Linux_DrawPreparationToShader( 0, TRUE ) ;

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_BLENDTEX_2D ].InputInfo,
		Vertex
	) ;	

	// 描画
	glDrawArrays( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], 0, VertexNum ) ;
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	// 終了
	return 0 ;
}

// シェーダーを使って３Ｄプリミティブを描画する
extern	int		Graphics_Hardware_DrawPrimitive3DToShader_PF(        const VERTEX3DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */ )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Graphics_Linux_DrawPreparationToShader( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_FOG, FALSE ) ;

	// ３Ｄ行列をハードウエアに反映する
	if( GSYS.DrawSetting.MatchHardware3DMatrix == FALSE )
		Graphics_DrawSetting_ApplyLib3DMatrixToHardware() ;


	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_SHADER_3D ].InputInfo,
		Vertex
	) ;	

	// 描画
	glDrawArrays( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], 0, VertexNum ) ;
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	// 終了
	return 0 ;
}

// シェーダーを使って２Ｄプリミティブを描画する( 頂点インデックスを使用する )
extern	int		Graphics_Hardware_DrawPrimitiveIndexed2DToShader_PF( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */ )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Graphics_Linux_DrawPreparationToShader( 0, TRUE ) ;

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_BLENDTEX_2D ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_SHORT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	// 終了
	return 0 ;
}

// シェーダーを使って２Ｄプリミティブを描画する( 頂点インデックスを使用する )
extern	int		Graphics_Hardware_DrawPrimitive32bitIndexed2DToShader_PF( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned int *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */ )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Graphics_Linux_DrawPreparationToShader( 0, TRUE ) ;

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_BLENDTEX_2D ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_INT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	// 終了
	return 0 ;
}

// シェーダーを使って３Ｄプリミティブを描画する( 頂点インデックスを使用する )
extern	int		Graphics_Hardware_DrawPrimitiveIndexed3DToShader_PF( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */ )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Graphics_Linux_DrawPreparationToShader( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_FOG, FALSE ) ;

	// ３Ｄ行列をハードウエアに反映する
	if( GSYS.DrawSetting.MatchHardware3DMatrix == FALSE )
		Graphics_DrawSetting_ApplyLib3DMatrixToHardware() ;

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_SHADER_3D ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_SHORT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	// 終了
	return 0 ;
}

// シェーダーを使って３Ｄプリミティブを描画する( 頂点インデックスを使用する )
extern	int		Graphics_Hardware_DrawPrimitive32bitIndexed3DToShader_PF( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned int *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */ )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Graphics_Linux_DrawPreparationToShader( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_FOG, FALSE ) ;

	// ３Ｄ行列をハードウエアに反映する
	if( GSYS.DrawSetting.MatchHardware3DMatrix == FALSE )
		Graphics_DrawSetting_ApplyLib3DMatrixToHardware() ;

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_SHADER_3D ].InputInfo,
		Vertex
	) ;

	// 描画
	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], IndexNum, GL_UNSIGNED_INT, Indices );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	// 終了
	return 0 ;
}

// シェーダーを使って３Ｄプリミティブを描画する( 頂点バッファ使用版 )
extern	int		Graphics_Hardware_DrawPrimitive3DToShader_UseVertexBuffer2_PF(
	int VertexBufHandle,
	int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */,
	int StartVertex,
	int UseVertexNum
)
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// エラー判定
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
	{
		return -1 ;
	}

	// シェーダー用頂点データではない場合はエラー
	if( VertexBuffer->Type != DX_VERTEX_TYPE_SHADER_3D )
	{
		return -1 ;
	}

	// ３Ｄ行列をハードウエアに反映する
	if( GSYS.DrawSetting.MatchHardware3DMatrix == FALSE )
		Graphics_DrawSetting_ApplyLib3DMatrixToHardware() ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Graphics_Linux_DrawPreparationToShader( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_FOG, FALSE ) ;


	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_SHADER_3D ].InputInfo,
		NULL,
		VertexBuffer->PF->VertexBuffer
	) ;

	// 描画
	glDrawArrays( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], StartVertex, UseVertexNum );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	// 終了
	return 0 ;
}

// シェーダーを使って３Ｄプリミティブを描画する( 頂点バッファとインデックスバッファ使用版 )
extern	int		Graphics_Hardware_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2_PF(
	int VertexBufHandle,
	int IndexBufHandle,
	int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */,
	int BaseVertex,
	int StartVertex,
	int UseVertexNum,
	int StartIndex,
	int UseIndexNum
)
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	INDEXBUFFERHANDLEDATA  *IndexBuffer ;
	GLenum					IndexFormat ;
//	int                     IndexSize ;
	GRAPHICS_LINUX_TEXTURE *UseTexture[ USE_TEXTURESTAGE_NUM ] ;
	int						 UseTextureNum = 0 ;
	int i ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 現状では StartIndex が 0 以外の場合は描画できない
	if( StartIndex != 0 )
	{
		return -1 ;
	}

	if( PrimitiveType < DX_PRIMTYPE_MIN || PrimitiveType > DX_PRIMTYPE_MAX )
	{
		return -1 ;
	}

	// エラー判定
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
	{
		return -1 ;
	}
	if( INDEXBUFFERCHK( IndexBufHandle, IndexBuffer ) )
	{
		return -1 ;
	}

	// シェーダー用頂点データではない場合はエラー
	if( VertexBuffer->Type != DX_VERTEX_TYPE_SHADER_3D )
	{
		return -1 ;
	}

	switch( IndexBuffer->Type )
	{
	default :
	case DX_INDEX_TYPE_16BIT :
		IndexFormat = GL_UNSIGNED_SHORT ;
		break ;

	case DX_INDEX_TYPE_32BIT :
		IndexFormat = GL_UNSIGNED_INT ;
		break ;
	}
//	IndexSize = IndexFormat == GL_UNSIGNED_SHORT ? 2 : 4 ;

	// ３Ｄ行列をハードウエアに反映する
	if( GSYS.DrawSetting.MatchHardware3DMatrix == FALSE )
		Graphics_DrawSetting_ApplyLib3DMatrixToHardware() ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画の準備
	Graphics_Linux_DrawPreparationToShader( DX_LINUX_DRAWPREP_LIGHTING | DX_LINUX_DRAWPREP_FOG, FALSE ) ;

	// 使用するテクスチャを列挙
	UseTextureNum = 0 ;
	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
	{
		IMAGEDATA     *TempImage ;
		SHADOWMAPDATA *TempShadowMap ;

		if( !GRAPHCHKFULL( GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ i ], TempImage ) )
		{
			UseTexture[ i ] = &TempImage->Hard.Draw[ 0 ].Tex->PF->Texture ;
		}
		else
		if( !SHADOWMAPCHKFULL( GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ i ], TempShadowMap ) )
		{
			UseTexture[ i ] = &TempShadowMap->PF->Texture ;
		}
		else
		{
			continue ;
		}

		UseTextureNum = i + 1 ;
	}

	// 使用するテクスチャをセット
	if( UseTextureNum > 0 )
	{
		for( i = 0 ; i < UseTextureNum ; i ++ )
		{
			Graphics_Linux_DeviceState_SetTexture( i, UseTexture[ i ] ) ;
		}
	}

	// Uniform の更新
	Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader_Force != NULL ? GLINUX.Device.State.SetShader_Force : GLINUX.Device.State.SetShader, 3 ) ;

	// 頂点データのセットアップ
	Graphics_Linux_DeviceState_SetupShaderVertexData(
		GLINUX.Device.State.SetShader_Force != NULL ? GLINUX.Device.State.SetShader_Force : GLINUX.Device.State.SetShader,
		&g_BaseSimpleVertexShaderInfo[ LINUX_VERTEX_INPUTLAYOUT_SHADER_3D ].InputInfo,
		NULL,
		VertexBuffer->PF->VertexBuffer
	) ;

	// 描画
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer->PF->IndexBuffer ) ;

	glDrawElements( g_DXPrimitiveTypeToGLES2PrimitiveType[ PrimitiveType ], UseIndexNum, IndexFormat, NULL );
	GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) ;

	// 終了
	return 0 ;
}




























// 環境依存初期化関係

// 描画処理の環境依存部分の初期化を行う関数( 実行箇所区別０ )
extern	int		Graphics_Initialize_Timing0_PF( void )
{
	// g_LinuxSys.NativeWindow に値が代入されるまで待つ
//	while( g_LinuxSys.NativeWindow == NULL )
//	{
//		NS_ProcessMessage() ;
//		Thread_Sleep( 1 ) ;
//	}

	// Linux を使用したグラフィックス処理の初期化
	if( Graphics_Linux_Initialize() == -1 )
	{
		return -1 ;
	}

	// 正常終了
	return 0 ;
}

// 描画処理の環境依存部分の初期化を行う関数( 実行箇所区別２ )
extern	int		Graphics_Initialize_Timing1_PF( void )
{
	// デバイスの設定をリフレッシュ
	Graphics_Linux_DeviceState_RefreshRenderState() ;

	// 画面の初期化
	Graphics_Hardware_ClearDrawScreen_PF( NULL ) ;

	// 正常終了
	return 0 ;
}

// ハードウエアアクセラレータを使用する場合の環境依存の初期化処理を行う
extern	int		Graphics_Hardware_Initialize_PF( void )
{
	return Graphics_Linux_Device_Initialize() ;
}

// 描画処理の環境依存部分の後始末を行う関数
extern	int		Graphics_Terminate_PF( void )
{
	// Linux の描画処理の後始末
	Graphics_Linux_Terminate() ;

	// シェーダーコードの後始末
	Graphics_Linux_ShaderCode_Base_Terminate() ;
	Graphics_Linux_ShaderCode_Base3D_Terminate() ;

#ifndef DX_NON_MODEL
	Graphics_Linux_ShaderCode_Model_Terminate() ;
#endif // DX_NON_MODEL

	// 終了
	return 0 ;
}

// グラフィックスシステムの復帰、又は変更付きの再セットアップを行う
extern	int		Graphics_RestoreOrChangeSetupGraphSystem_PF( int Change, int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int RefreshRate )
{
#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込みの途中の場合は、非同期読み込みが終了するまで待つ
	while( NS_GetASyncLoadNum() > 0 )
	{
		ProcessASyncLoadRequestMainThread() ;
		Thread_Sleep( 0 ) ;
	}
#endif // DX_NON_ASYNCLOAD

//	int i ;

	if( Change == TRUE )
	{
		// 画面モードのセット
		Graphics_Screen_SetMainScreenSize( ScreenSizeX, ScreenSizeY ) ;
		GSYS.Screen.MainScreenColorBitDepth = ColorBitDepth ;
		GSYS.Screen.MainScreenRefreshRate   = RefreshRate ;
//		SetMemImgDefaultColorType( ColorBitDepth == 32 ? 1 : 0 ) ;

		// サブバックバッファの作り直しを行う
		Graphics_Linux_TerminateSubBackBuffer() ;

		// サブバックバッファの作成
		if( Graphics_Linux_SetupSubBackBuffer() < 0 )
		{
			return -1 ;
		}

		if( ( DWORD )GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_BACK ||
			( DWORD )GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_FRONT ) 
		{
			NS_SetDrawScreen( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;
		}

		return 0 ;
	}
	else
	{
		// DirectInput オブジェクトの再セットアップ
#ifndef DX_NON_INPUT
		TerminateInputSystem() ;
		InitializeInputSystem() ;
#endif // DX_NON_INPUT
	}

//	DXST_LOGFILEFMT_ADDUTF16LE(( L"確保メモリ数:%d  確保メモリ総サイズ:%dByte(%dKByte)", NS_DxGetAllocNum(), NS_DxGetAllocSize(), NS_DxGetAllocSize() / 1024 )) ;

	if( DxSysData.NotDrawFlag == FALSE )
	{
		MATRIX_D ViewMatrix, ProjectionMatrix, ViewportMatrix ;

		// OpenGL ES による描画処理の初期化
		GSYS.Screen.FullScreenResolutionModeAct = GSYS.Screen.FullScreenResolutionMode ;
		Graphics_Linux_Initialize() ;

		// グラフィックハンドルが持つ OpenGL ES オブジェクトの再作成
		Graphics_Linux_CreateObjectAll() ;

#ifndef DX_NON_MASK
		// マスクサーフェスの再作成
		Mask_ReCreateSurface() ;
#endif
		// アクティブグラフィックのアドレスを再度設定
		if( Change == FALSE )
		{
			ViewMatrix       = GSYS.DrawSetting.ViewMatrix ;
			ProjectionMatrix = GSYS.DrawSetting.ProjectionMatrix ;
			ViewportMatrix   = GSYS.DrawSetting.ViewportMatrix ;
		}
//		NS_SetRenderTargetToShader( 0, GSYS.DrawSetting.TargetScreen[ 0 ], GSYS.DrawSetting.TargetScreenSurface[ 0 ] ) ;
		NS_SetDrawScreen( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;

		// グラフィック関係の設定を初期化
		Graphics_Linux_Device_ReInitialize() ;

		// 画面の初期化
//		NS_ClearDrawScreen( NULL ) ;
//		NS_ScreenFlip() ;
//		NS_ClearDrawScreen( NULL ) ;
//		NS_ScreenFlip() ;

		// デバイスロスト発生時に削除するフラグが立っているグラフィックを削除する
		Graphics_Image_DeleteDeviceLostDelete() ;

		// グラフィックの復元
		NS_RunRestoreShred() ;

		// グラフィックスデバイスのロストから復帰する前に呼ぶ関数が登録されていたら実行する
		if( GLINUX.Device.Setting.DeviceLostCallbackFunction )
		{
			GLINUX.Device.Setting.DeviceLostCallbackFunction( GLINUX.Device.Setting.DeviceLostCallbackData ) ;
		}

		// グラフィックスデバイスのロストから復帰したときに呼ぶ関数が登録されていたら実行する
		if( GLINUX.Device.Setting.DeviceRestoreCallbackFunction )
		{
			GLINUX.Device.Setting.DeviceRestoreCallbackFunction( GLINUX.Device.Setting.DeviceRestoreCallbackData ) ;
		}

		// ハードウエアの設定をリフレッシュ
		Graphics_Linux_DeviceState_RefreshRenderState() ;

#ifndef DX_NON_MODEL
		// 頂点バッファのセットアップ
		MV1SetupVertexBufferAll() ;
#endif

		// ライトの設定を Direct3D に再反映するようにフラグをセット
		GSYS.Light.HardwareRefresh = TRUE ;

		// その他の再設定
		if( Change == FALSE )
		{
			NS_SetDrawArea( GSYS.DrawSetting.OriginalDrawRect.left, GSYS.DrawSetting.OriginalDrawRect.top, GSYS.DrawSetting.OriginalDrawRect.right, GSYS.DrawSetting.OriginalDrawRect.bottom ) ;
			NS_SetTransformToWorldD( &GSYS.DrawSetting.WorldMatrix );
			NS_SetTransformToViewD( &ViewMatrix );
			NS_SetTransformToProjectionD( &ProjectionMatrix );
			NS_SetTransformToViewportD( &ViewportMatrix );
		}

//		// ０以外のターゲットのセットアップ
//		for( i = 1 ; i < GSYS.HardInfo.RenderTargetNum ; i ++ )
//		{
//			NS_SetRenderTargetToShader( i, GSYS.DrawSetting.TargetScreen[ i ], GSYS.DrawSetting.TargetScreenSurface[ i ] ) ;
//		}
	}

	// 終了
	return 0 ;
}

// 描画用デバイスが有効かどうかを取得する( 戻り値  TRUE:有効  FALSE:無効 )
extern	int		Graphics_Hardware_CheckValid_PF( void )
{
	return LINUX_CHECKVALID_HARDWARE ;
}














































// 環境依存描画設定関係

// シェーダー描画での描画先を設定する
extern	int		Graphics_Hardware_SetRenderTargetToShader_PF( int TargetIndex, int DrawScreen, int SurfaceIndex, int MipLevel )
{
	IMAGEDATA *Image ;
	IMAGEDATA *OldImage ;

	// ターゲットが０以外の場合はシェーダーが使用できない場合は何もしない
	if( GSYS.HardInfo.UseShader == FALSE )
		return -1 ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// セットしていたテクスチャーを外す
	Graphics_Linux_DrawSetting_SetTexture( NULL ) ;

	// 今までの描画先の画像情報の取得
	if( GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ TargetIndex ], OldImage ) )
	{
		OldImage = NULL ;
	}

	// 画像かどうかを判定
	if( GRAPHCHKFULL( DrawScreen, Image ) )
	{
		// 画像ではない場合は描画対象を無効にする
		GSYS.DrawSetting.TargetScreen[ TargetIndex ] = 0 ;
		GSYS.DrawSetting.TargetScreenSurface[ TargetIndex ] = 0 ;
//		Graphics_Linux_DeviceState_SetRenderTarget( 0, 0, 0 ) ;
		return 0 ;
	}

	// 画像だった場合は描画可能では無い場合はエラー
	if( Image->Orig->FormatDesc.DrawValidFlag == FALSE )
	{
		return -1 ;
	}

	// 描画先をセット
	Graphics_Linux_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->FrameBuffer, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;

	// 正常終了
	return 0 ;
}

// メインウインドウの背景色を設定する( Red,Green,Blue:それぞれ ０～２５５ )
extern	int		Graphics_Hardware_SetBackgroundColor_PF( int Red, int Green, int Blue, int Alpha )
{
	Graphics_Linux_DeviceState_SetBackgroundColor( Red, Green, Blue, Alpha ) ;

	return 0 ;
}

// SetDrawBright の引数が一つ版
extern	int		Graphics_Hardware_SetDrawBrightToOneParam_PF( DWORD Bright )
{
	// ディフーズカラーの更新
	GLINUX.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// 正常終了
	return 0 ;
}

// 描画ブレンドモードをセットする
extern	int		Graphics_Hardware_SetDrawBlendMode_PF( int BlendMode, int BlendParam )
{
	// ディフーズカラーの更新
	GLINUX.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// 正常終了
	return 0 ;
}

// カスタムブレンドモードを設定する
extern	int		Graphics_Hardware_SetDrawCustomBlendMode_PF(int BlendEnable, int SrcBlendRGB, int DestBlendRGB, int BlendOpRGB, int SrcBlendA, int DestBlendA, int BlendOpA, int BlendParam)
{
	// ディフーズカラーの更新
	GLINUX.Device.DrawInfo.DiffuseColor = GetDiffuseColor();

	// 正常終了
	return 0;
}

// 描画時のアルファテストの設定を行う( TestMode:DX_CMP_GREATER等( -1:デフォルト動作に戻す )  TestParam:描画アルファ値との比較に使用する値 )
extern	int		Graphics_Hardware_SetDrawAlphaTest_PF( int TestMode, int TestParam )
{
	// ディフーズカラーの更新
	GLINUX.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// 正常終了
	return 0 ;
}

// 描画モードをセットする
extern	int		Graphics_Hardware_SetDrawMode_PF( int DrawMode )
{
	Graphics_Linux_DeviceState_SetDrawMode( DrawMode ) ;

	// 正常終了
	return 0 ;
}

// 描画輝度をセット
extern	int		Graphics_Hardware_SetDrawBright_PF( int RedBright, int GreenBright, int BlueBright )
{
	// ディフーズカラーの更新
	GLINUX.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// 正常終了
	return 0 ;
}

// 描画輝度をセット
extern	int		Graphics_Hardware_SetDrawAddColor_PF( int Red, int Green, int Blue )
{
	GLINUX.Device.Shader.Constant.uAddColor[ 0 ] = GSYS.DrawSetting.DrawAddColorF.r ;
	GLINUX.Device.Shader.Constant.uAddColor[ 1 ] = GSYS.DrawSetting.DrawAddColorF.g ;
	GLINUX.Device.Shader.Constant.uAddColor[ 2 ] = GSYS.DrawSetting.DrawAddColorF.b ;

	GLINUX.Device.Shader.Constant.UpdateCount ++ ;

	// 正常終了
	return 0 ;
}

// SetBlendGraphParam の可変長引数パラメータ付き
extern	int		Graphics_Hardware_SetBlendGraphParamBase_PF( IMAGEDATA *BlendImage, int BlendType, int *Param )
{
	// ブレンド画像が NULL もしくはテクスチャではなかったらブレンド画像処理の解除
	if( BlendImage == NULL || BlendImage->Orig->FormatDesc.TextureFlag == FALSE )
	{
		GSYS.DrawSetting.BlendGraph = -1 ;
		Graphics_Linux_DrawSetting_SetBlendTexture( NULL ) ;
	}
	else
	{
		// ブレンドパラメータをセットする
		Graphics_Linux_DrawSetting_SetBlendTexture( &BlendImage->Hard.Draw[ 0 ].Tex[ 0 ].PF->Texture ) ;
		Graphics_Linux_DrawSetting_SetBlendTextureParam( BlendType, Param ) ;
	}

	// 正常終了
	return 0 ;
}

// 最大異方性の値をセットする
extern	int		Graphics_Hardware_SetMaxAnisotropy_PF( int MaxAnisotropy )
{
	// 正常終了
	return 0 ;
}

// ワールド変換用行列をセットする
extern	int		Graphics_Hardware_SetTransformToWorld_PF( const MATRIX *Matrix )
{
	Graphics_Linux_DeviceState_SetWorldMatrix( Matrix ) ;

	// 正常終了
	return 0 ;
}

// ビュー変換用行列をセットする
extern	int		Graphics_Hardware_SetTransformToView_PF( const MATRIX *Matrix )
{
	Graphics_Linux_DeviceState_SetViewMatrix( Matrix ) ;

	// 正常終了
	return 0 ;
}

// Linux に設定する射影行列を更新する
static void Graphics_Hardware_LINUX_RefreshProjectionMatrix( void )
{
	MATRIX *UseProjectionMatrix ;
	MATRIX *UseViewportMatrix ;
	MATRIX TempMatrix ;

//	if( GSYS.DrawSetting.UseProjectionMatrix2D )
//	{
//		if( AlwaysUpdate == FALSE && Update2DProjection == FALSE )
//		{
//			return ;
//		}
//
//		UseProjectionMatrix = &GSYS.DrawSetting.ProjectionMatrix2D ;
//		UseViewportMatrix   = &GSYS.DrawSetting.ViewportMatrix2D ;
//	}
//	else
//	{
//		if( AlwaysUpdate == FALSE && Update3DProjection == FALSE )
//		{
//			return ;
//		}

		UseProjectionMatrix = &GSYS.DrawSetting.ProjectionMatrixF ;
		UseViewportMatrix   = &GSYS.DrawSetting.ViewportMatrixF ;
//	}
	if( GSYS.DrawSetting.MatchHardware_2DMatrix )
	{
		CreateMultiplyMatrix( &TempMatrix, &GSYS.DrawSetting._2DMatrixF, &GSYS.DrawSetting.Direct3DViewportMatrixAntiF ) ;
	}
	else
	{
		CreateMultiplyMatrix( &TempMatrix, UseProjectionMatrix, UseViewportMatrix                             ) ;
		CreateMultiplyMatrix( &TempMatrix, &TempMatrix,         &GSYS.DrawSetting.Direct3DViewportMatrixAntiF ) ;
	}

	Graphics_Linux_DeviceState_SetProjectionMatrix( &TempMatrix ) ;
}

// 投影変換用行列をセットする
extern	int		Graphics_Hardware_SetTransformToProjection_PF( const MATRIX *Matrix )
{
	Graphics_Hardware_LINUX_RefreshProjectionMatrix() ;

	// 正常終了
	return 0 ;
}

// ビューポート行列をセットする
extern	int		Graphics_Hardware_SetTransformToViewport_PF( const MATRIX * /* Matrix */ )
{
	Graphics_Hardware_LINUX_RefreshProjectionMatrix() ;

	// 正常終了
	return 0 ;
}

// テクスチャアドレスモードを設定する
extern	int		Graphics_Hardware_SetTextureAddressMode_PF( int Mode /* DX_TEXADDRESS_WRAP 等 */, int Stage )
{
	Graphics_Linux_DeviceState_SetTextureAddress( Mode, Stage ) ;

	// 正常終了
	return 0 ;
}

// テクスチャアドレスモードを設定する
extern	int		Graphics_Hardware_SetTextureAddressModeUV_PF( int ModeU, int ModeV, int Stage )
{
	Graphics_Linux_DeviceState_SetTextureAddressU( ModeU, Stage ) ;
	Graphics_Linux_DeviceState_SetTextureAddressV( ModeV, Stage ) ;

	// 正常終了
	return 0 ;
}

// テクスチャ座標変換行列をセットする
extern	int		Graphics_Hardware_SetTextureAddressTransformMatrix_PF( int UseFlag, MATRIX *Matrix, int Sampler )
{
	Graphics_Linux_DeviceState_SetTextureAddressTransformMatrix( UseFlag, Matrix ) ;

	// 正常終了
	return 0 ;
}

// フォグを有効にするかどうかを設定する( TRUE:有効  FALSE:無効 )
extern	int		Graphics_Hardware_SetFogEnable_PF( int Flag )
{
	Graphics_Linux_DeviceState_SetFogEnable( Flag ) ;

	// 正常終了
	return 0 ;
}

// フォグモードを設定する
extern	int		Graphics_Hardware_SetFogMode_PF( int Mode /* DX_FOGMODE_NONE 等 */ )
{
	Graphics_Linux_DeviceState_SetFogVertexMode( Mode ) ;

	// 正常終了
	return 0 ;
}

// フォグカラーを変更する
extern	int		Graphics_Hardware_SetFogColor_PF( DWORD FogColor )
{
	Graphics_Linux_DeviceState_SetFogColor( FogColor ) ;

	// 正常終了
	return 0 ;
}

// フォグが始まる距離と終了する距離を設定する( 0.0f ～ 1.0f )
extern	int		Graphics_Hardware_SetFogStartEnd_PF( float start, float end )
{
	Graphics_Linux_DeviceState_SetFogStartEnd( start, end ) ;

	// 正常終了
	return 0 ;
}

// フォグの密度を設定する( 0.0f ～ 1.0f )
extern	int		Graphics_Hardware_SetFogDensity_PF( float density )
{
	Graphics_Linux_DeviceState_SetFogDensity( density ) ;

	// 正常終了
	return 0 ;
}

// 高さフォグを有効にするかどうかを設定する( TRUE:有効  FALSE:無効 )
extern	int		Graphics_Hardware_SetVerticalFogEnable_PF( int Flag )
{
	// 未実装
	return 0 ;
}

// 高さフォグモードを設定する
extern	int		Graphics_Hardware_SetVerticalFogMode_PF( int Mode /* DX_FOGMODE_NONE 等 */ )
{
	// 未実装
	return 0 ;
}

// 高さフォグカラーを変更する
extern	int		Graphics_Hardware_SetVerticalFogColor_PF( DWORD FogColor )
{
	// 未実装
	return 0 ;
}

// 高さフォグが始まる距離と終了する距離を設定する( 0.0f ～ 1.0f )
extern	int		Graphics_Hardware_SetVerticalFogStartEnd_PF( float start, float end )
{
	// 未実装
	return 0 ;
}

// 高さフォグの密度を設定する( 0.0f ～ 1.0f )
extern	int		Graphics_Hardware_SetVerticalFogDensity_PF( float start, float density )
{
	// 未実装
	return 0 ;
}

// ワールド変換用行列をセットする
extern	int		Graphics_Hardware_DeviceDirect_SetWorldMatrix_PF( const MATRIX *Matrix )
{
	return Graphics_Linux_DeviceState_SetWorldMatrix( Matrix ) ;
}

// ビュー変換用行列をセットする
extern	int		Graphics_Hardware_DeviceDirect_SetViewMatrix_PF( const MATRIX *Matrix )
{
	return Graphics_Linux_DeviceState_SetViewMatrix( Matrix ) ;
}

// 投影変換用行列をセットする
extern	int		Graphics_Hardware_DeviceDirect_SetProjectionMatrix_PF( const MATRIX *Matrix )
{
	return Graphics_Linux_DeviceState_SetProjectionMatrix( Matrix ) ;
}

// 基本データに設定されているフォグ情報をハードウェアに反映する
extern	int		Graphics_Hardware_ApplyLigFogToHardware_PF( void )
{
	if( GLINUX.Device.State.FogEnable != GSYS.DrawSetting.FogEnable )
	{
		Graphics_Linux_DeviceState_SetFogEnable( GSYS.DrawSetting.FogEnable ) ;
	}

	// 正常終了
	return 0 ;
}

// 以前の DrawModiGraph 関数のコードを使用するかどうかのフラグをセットする
extern	int		Graphics_Hardware_SetUseOldDrawModiGraphCodeFlag_PF( int Flag )
{
	GLINUX.Setting.UseOldDrawModiGraphCodeFlag = Flag ;
	
	// 終了
	return 0 ;
}

// 描画先に正しいα値を書き込むかどうかのフラグを更新する
extern	int		Graphics_Hardware_RefreshAlphaChDrawMode_PF( void )
{
	IMAGEDATA *Image ;
	int i ;
	int NextFlag ;

	NextFlag = FALSE ;

	// ０番以外にも描画先が設定されていた場合は通常モード
	for( i = 1 ; i < DX_RENDERTARGET_COUNT ; i ++ )
	{
		if( GSYS.DrawSetting.TargetScreen[ i ] != 0 )
			goto END ;
	}

	// 描画先が通常画面の場合は何もしない
	if( ( DWORD )GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_BACK ||
		( DWORD )GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_FRONT ||
		( DWORD )GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_WORK ||
		( DWORD )GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_TEMPFRONT )
		goto END ;

	// 描画先となっている画像が無効の場合は何もしない
	if( GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) )
		goto END ;

	// 描画先の画像にαチャンネルが無い場合も何もしない
	if( Image->Orig->FormatDesc.AlphaChFlag == FALSE )
		goto END ;
	
	// テクスチャではない場合は何もしない
	if( Image->Orig->FormatDesc.TextureFlag == FALSE )
		goto END ;

	// ここに来たということは正確なαチャンネルの計算を行うということ
	NextFlag = TRUE ;

END :
/*
	// テクスチャの保存
	if( NextFlag == TRUE )
	{
		IMAGEDATA *WorkImage ;
		int WorkTextureHandle ;

		WorkTextureHandle = GetWorkTexture( Image->Orig->FormatDesc.FloatTypeFlag, Image->Width, Image->Height, 0 ) ;
		if( GRAPHCHK( WorkTextureHandle, WorkImage ) )
			return -1 ;

		GRH.RenderTargetTexture          = WorkImage->Orig->Hard.Tex[ 0 ].PF->D3D9.Texture ;
		GRH.RenderTargetSurface          =     Image->Orig->Hard.Tex[ 0 ].PF->D3D9.Surface[ 0 ] ;
		GRH.RenderTargetCopySurface      = WorkImage->Orig->Hard.Tex[ 0 ].PF->D3D9.Surface[ 0 ] ;
		GRH.RenderTargetTextureInvWidth  = 1.0f / WorkImage->Orig->Hard.Tex[ 0 ].TexWidth ;
		GRH.RenderTargetTextureInvHeight = 1.0f / WorkImage->Orig->Hard.Tex[ 0 ].TexHeight ;
	}
	else
	{
		GRH.RenderTargetTexture = FALSE ;
	}
*/
	// 以前とフラグの状態が異なる場合のみブレンドモードの更新も行う
	if( NextFlag != GSYS.DrawSetting.AlphaChDrawMode )
	{
		// フラグの保存
		GSYS.DrawSetting.AlphaChDrawMode = NextFlag ;
		GLINUX.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
		GSYS.ChangeSettingFlag = TRUE ;
	}

	// 終了
	return 0 ;
}

// ミドルウェア等を使用した後に Direct3D や OpenGL のＤＸライブラリ用の設定を再度行うための関数
extern int Graphics_Hardware_RefreshSetting_PF( void )
{
	Graphics_Linux_DeviceState_RefreshRenderState() ;

	// 終了
	return 0 ;
}
























// 環境依存設定関係

// ハードウエアの頂点演算処理機能を使用するかどうかを設定する
extern	int		Graphics_Hardware_SetUseHardwareVertexProcessing_PF( int Flag )
{
	return 0 ;
}

// ピクセル単位でライティングを行うかどうかを設定する、要 ShaderModel 3.0( TRUE:ピクセル単位のライティングを行う  FALSE:頂点単位のライティングを行う( デフォルト ) )
extern	int		Graphics_Hardware_SetUsePixelLighting_PF( int Flag )
{
	GLINUX.UsePixelLightingShader = Flag ;

	// 終了
	return 0 ;
}

// グラフィックスデバイスがロストから復帰した際に呼ばれるコールバック関数を設定する
extern	int		Graphics_Hardware_SetGraphicsDeviceRestoreCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData )
{
	GLINUX.Device.Setting.DeviceRestoreCallbackFunction = Callback ;
	GLINUX.Device.Setting.DeviceRestoreCallbackData = CallbackData ;

	// 終了
	return 0 ;
}

// グラフィックスデバイスがロストから復帰する前に呼ばれるコールバック関数を設定する
extern	int		Graphics_Hardware_SetGraphicsDeviceLostCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData )
{
	GLINUX.Device.Setting.DeviceLostCallbackFunction = Callback ;
	GLINUX.Device.Setting.DeviceLostCallbackData = CallbackData ;

	// 終了
	return 0 ;
}

// 通常描画にプログラマブルシェーダーを使用するかどうかを設定する( TRUE:使用する( デフォルト )  FALSE:使用しない )
extern	int		Graphics_Hardware_SetUseNormalDrawShader_PF( int Flag )
{
	return -1 ;
}

// ビデオメモリの容量を得る
extern	int		Graphics_Hardware_GetVideoMemorySizeEx_PF( ULONGLONG *TotalSize, ULONGLONG *UseSize )
{
	return -1 ;
}

// Vista以降の Windows Aero を無効にするかどうかをセットする、TRUE:無効にする  FALSE:有効にする( DxLib_Init の前に呼ぶ必要があります )
extern	int		Graphics_SetAeroDisableFlag_PF( int Flag )
{
	// 初期化前のみ有効
	if( DxSysData.DxLib_InitializeFlag == TRUE )
	{
		return -1 ;
	}

	// 終了
	return 0 ;
}































// 環境依存画面関係

// 設定に基づいて使用するＺバッファをセットする
extern	int		Graphics_Hardware_SetupUseZBuffer_PF( void )
{
	// 終了
	return 0 ;
}

// 画面のＺバッファの状態を初期化する
extern	int		Graphics_Hardware_ClearDrawScreenZBuffer_PF( const RECT *ClearRect )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// 描画対象サーフェスを変更する
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag && MASKLINUX.MaskScreenFrameBuffer )
	{
		IMAGEDATA           *Image = NULL ;
		SHADOWMAPDATA       *ShadowMap = NULL ;

		// 描画先画像の画像情報を取得
		GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) ;
		SHADOWMAPCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], ShadowMap ) ;

		// シャドウマップが有効な場合はシャドウマップを描画対象にする
		if( ShadowMap )
		{
			Graphics_Linux_DeviceState_SetRenderTarget( ShadowMap->PF->FrameBuffer, ShadowMap->PF->Texture.Width, ShadowMap->PF->Texture.Height ) ;
		}
		else
		// 描画可能画像が有効な場合は描画可能画像を描画対象にする
		if( Image )
		{
			if( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget != 0 )
			{
				Graphics_Linux_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
			}
			else
			{
				Graphics_Linux_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->FrameBuffer, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
			}
		}
		else
		// それ以外の場合はサブバックバッファを描画対象にする
		{
			Graphics_Linux_DeviceState_SetRenderTarget( GLINUX.Device.Screen.SubBackBufferFrameBuffer, GLINUX.Device.Screen.SubBackBufferTextureSizeX, GLINUX.Device.Screen.SubBackBufferTextureSizeY ) ;
		}
	}
#endif

	// ビューポートをサーフェス全体にする
	{
		RECT Viewport ;

		_MEMSET( &Viewport, 0, sizeof( Viewport ) ) ;
		Viewport.left	= 0 ;
		Viewport.top	= 0 ;
		Viewport.right	= GLINUX.Device.State.TargetFrameBufferWidth ;
		Viewport.bottom	= GLINUX.Device.State.TargetFrameBufferHeight ;
		Graphics_Linux_DeviceState_SetViewport( &Viewport ) ;
	}

	glClearColor(
		GSYS.Screen.BackgroundRed   / 255.0f,
		GSYS.Screen.BackgroundGreen / 255.0f,
		GSYS.Screen.BackgroundBlue  / 255.0f,
		GSYS.Screen.BackgroundAlpha / 255.0f
	) ;

	glDepthMask( GL_TRUE ) ;
    glClear( GL_DEPTH_BUFFER_BIT ) ;
	glDepthMask( GLINUX.Device.State.DepthWriteEnable ? GL_TRUE : GL_FALSE ) ;

	// 描画対象サーフェスを元に戻す
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag && MASKLINUX.MaskScreenFrameBuffer )
	{
		Graphics_Linux_DeviceState_SetRenderTarget(
			MASKLINUX.MaskScreenFrameBuffer,
			MASKLINUX.MaskTextureSizeX,
			MASKLINUX.MaskTextureSizeY
		) ;
	}
#endif

	// ビューポートの範囲を元に戻す
	Graphics_Linux_DeviceState_SetViewport( &GSYS.DrawSetting.DrawArea ) ;

	// 終了
	return 0 ;
}

// 画面の状態を初期化する
extern	int		Graphics_Hardware_ClearDrawScreen_PF( const RECT *ClearRect )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// 描画対象サーフェスを変更する
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag && MASKLINUX.MaskScreenFrameBuffer )
	{
		IMAGEDATA           *Image = NULL ;
		SHADOWMAPDATA       *ShadowMap = NULL ;

		// 描画先画像の画像情報を取得
		GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) ;
		SHADOWMAPCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], ShadowMap ) ;

		// シャドウマップが有効な場合はシャドウマップを描画対象にする
		if( ShadowMap )
		{
			Graphics_Linux_DeviceState_SetRenderTarget( ShadowMap->PF->FrameBuffer, ShadowMap->PF->Texture.Width, ShadowMap->PF->Texture.Height ) ;
		}
		else
		// 描画可能画像が有効な場合は描画可能画像を描画対象にする
		if( Image )
		{
			if( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget != 0 )
			{
				Graphics_Linux_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
			}
			else
			{
				Graphics_Linux_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->FrameBuffer, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Width, Image->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
			}
		}
		else
		// それ以外の場合はサブバックバッファを描画対象にする
		{
			Graphics_Linux_DeviceState_SetRenderTarget( GLINUX.Device.Screen.SubBackBufferFrameBuffer, GLINUX.Device.Screen.SubBackBufferTextureSizeX, GLINUX.Device.Screen.SubBackBufferTextureSizeY ) ;
		}
	}
#endif

	// ビューポートをサーフェス全体にする
	{
		RECT Viewport ;

		_MEMSET( &Viewport, 0, sizeof( Viewport ) ) ;
		Viewport.left	= 0 ;
		Viewport.top	= 0 ;
		Viewport.right	= GLINUX.Device.State.TargetFrameBufferWidth ;
		Viewport.bottom	= GLINUX.Device.State.TargetFrameBufferHeight ;
		Graphics_Linux_DeviceState_SetViewport( &Viewport ) ;
	}

	glClearColor(
		GSYS.Screen.BackgroundRed   / 255.0f,
		GSYS.Screen.BackgroundGreen / 255.0f,
		GSYS.Screen.BackgroundBlue  / 255.0f,
		GSYS.Screen.BackgroundAlpha / 255.0f
	) ;

	glDepthMask( GL_TRUE ) ;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
	glDepthMask( GLINUX.Device.State.DepthWriteEnable ? GL_TRUE : GL_FALSE ) ;

	// 描画対象サーフェスを元に戻す
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag && MASKLINUX.MaskScreenFrameBuffer )
	{
		Graphics_Linux_DeviceState_SetRenderTarget(
			MASKLINUX.MaskScreenFrameBuffer,
			MASKLINUX.MaskTextureSizeX,
			MASKLINUX.MaskTextureSizeY
		) ;
	}
#endif

	// ビューポートの範囲を元に戻す
	Graphics_Linux_DeviceState_SetViewport( &GSYS.DrawSetting.DrawArea ) ;

	// 終了
	return 0 ;
}

// 描画先画面のセット
extern	int		Graphics_Hardware_SetDrawScreen_PF( int DrawScreen, int OldScreenSurface, int OldScreenMipLevel, IMAGEDATA *NewTargetImage, IMAGEDATA *OldTargetImage, SHADOWMAPDATA *NewTargetShadowMap, SHADOWMAPDATA *OldTargetShadowMap )
{
	int i ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// セットしていたテクスチャーを外す
	Graphics_Linux_DrawSetting_SetTexture( NULL ) ;
	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
	{
		Graphics_Linux_DeviceState_SetTexture( 0, NULL ) ;
	}

	// 描画先の変更

	// マスクサーフェスが存在していて且つ有効な場合はマスクサーフェスを描画対象にする
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag && MASKLINUX.MaskScreenFrameBuffer )
	{
		Graphics_Linux_DeviceState_SetRenderTarget( MASKLINUX.MaskScreenFrameBuffer, MASKLINUX.MaskTextureSizeX, MASKLINUX.MaskTextureSizeY ) ;
	}
	else
#endif
	// シャドウマップが有効な場合はシャドウマップを描画対象にする
	if( NewTargetShadowMap )
	{
		Graphics_Linux_DeviceState_SetRenderTarget( NewTargetShadowMap->PF->FrameBuffer, NewTargetShadowMap->PF->Texture.Width, NewTargetShadowMap->PF->Texture.Height ) ;
	}
	else
	// 描画可能画像が有効な場合は描画可能画像を描画対象にする
	if( NewTargetImage )
	{
		if( NewTargetImage->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget != 0 )
		{
			Graphics_Linux_DeviceState_SetRenderTarget( NewTargetImage->Hard.Draw[ 0 ].Tex->PF->MSRenderTarget, NewTargetImage->Hard.Draw[ 0 ].Tex->PF->Texture.Width, NewTargetImage->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
		}
		else
		{
			Graphics_Linux_DeviceState_SetRenderTarget( NewTargetImage->Hard.Draw[ 0 ].Tex->PF->FrameBuffer, NewTargetImage->Hard.Draw[ 0 ].Tex->PF->Texture.Width, NewTargetImage->Hard.Draw[ 0 ].Tex->PF->Texture.Height ) ;
		}
	}
	else
	// それ以外の場合はサブバックバッファを描画対象にする
	{
		Graphics_Linux_DeviceState_SetRenderTarget( GLINUX.Device.Screen.SubBackBufferFrameBuffer, GLINUX.Device.Screen.SubBackBufferTextureSizeX, GLINUX.Device.Screen.SubBackBufferTextureSizeY ) ;
	}

	// 使用するＺバッファのセットアップ
	Graphics_Screen_SetupUseZBuffer() ;

	// 正常終了
	return 0 ;
}

// SetDrawScreen の最後で呼ばれる関数
extern	int		Graphics_Hardware_SetDrawScreen_Post_PF( int DrawScreen )
{
	// 正常終了
	return 0 ;
}

// 描画可能領域のセット
extern	int		Graphics_Hardware_SetDrawArea_PF( int x1, int y1, int x2, int y2 )
{
	RECT Viewport ;

	if( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left == 0 ||
		GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  == 0 )
	{
		return -1 ;
	}

	// ビューポートのセット
	Viewport.left	= GSYS.DrawSetting.DrawArea.left ;
	Viewport.top	= GSYS.DrawSetting.DrawArea.top ;
	Viewport.right	= GSYS.DrawSetting.DrawArea.right  ;
	Viewport.bottom	= GSYS.DrawSetting.DrawArea.bottom ;
	Graphics_Linux_DeviceState_SetViewport( &Viewport ) ;

	// 射影行列とビューポート行列の逆行列を乗算したものをセット
	Graphics_Hardware_LINUX_RefreshProjectionMatrix() ;
	GSYS.DrawSetting.MatchHardwareProjectionMatrix = TRUE ;

	// アンチビューポート行列を更新
	Graphics_Linux_DeviceState_SetAntiViewportMatrix( &GSYS.DrawSetting.Direct3DViewportMatrixAntiF ) ;

	// 正常終了
	return 0 ;
}

// 描画先バッファをロックする
extern	int		Graphics_Hardware_LockDrawScreenBuffer_PF( RECT *LockRect, BASEIMAGE *BaseImage, int TargetScreen, IMAGEDATA *TargetImage, int TargetScreenSurface, int TargetScreenMipLevel, int ReadOnly, int TargetScreenTextureNo )
{
	GLuint TargetFrameBuffer ;
	GLuint TargetTexture ;
	GLuint TargetWidth ;
	GLuint TargetHeight ;
//	COLORDATA *TargetColorData ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// 取り込み元となるサーフェスの決定

	// 描画可能画像が対象の場合
	if( TargetImage )
	{
		TargetFrameBuffer = TargetImage->Orig->Hard.Tex[ TargetScreenTextureNo ].PF->FrameBuffer ;
		TargetTexture     = TargetImage->Orig->Hard.Tex[ TargetScreenTextureNo ].PF->Texture.TextureBuffer ;
		TargetWidth       = TargetImage->Orig->Hard.Tex[ TargetScreenTextureNo ].PF->Texture.Width ;
		TargetHeight      = TargetImage->Orig->Hard.Tex[ TargetScreenTextureNo ].PF->Texture.Height ;
	}
	else
	{
		TargetFrameBuffer = GLINUX.Device.Screen.SubBackBufferFrameBuffer ;
		TargetTexture     = GLINUX.Device.Screen.SubBackBufferTexture ;
		TargetWidth       = GLINUX.Device.Screen.SubBackBufferTextureSizeX ;
		TargetHeight      = GLINUX.Device.Screen.SubBackBufferTextureSizeY ;
	}

//	BaseImage->ColorData      = *TargetColorData ;
//	BaseImage->Width          = LockRect->right  - LockRect->left ;
//	BaseImage->Height         = LockRect->bottom - LockRect->top ;
//	BaseImage->Pitch          = TargetTexture->getPitch() * TargetColorData->PixelByte ;
//	BaseImage->GraphData      = ( BYTE * )TargetTexture->getBaseAddress() + LockRect->left * TargetColorData->PixelByte + LockRect->top * BaseImage->Pitch ;
//	BaseImage->MipMapCount    = 0 ;
//	BaseImage->GraphDataCount = 0 ;

	// マップ
	if( Graphics_Linux_Texture_Map( TargetTexture, TargetFrameBuffer, TargetWidth, TargetHeight, LockRect, BaseImage, &GLINUX.Device.Screen.DrawScreenLockImage ) < 0 )
	{
		DXST_LOGFILE_ADDUTF16LE( "\xcf\x63\x3b\x75\xfe\x5b\x61\x8c\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"描画対象バッファのロックに失敗しました\n" @*/ ) ;
		return -1 ;
	}

	// 正常終了
	return 0 ;
}

// 描画先バッファをアンロックする
extern	int		Graphics_Hardware_UnlockDrawScreenBuffer_PF( void )
{
	// マップを解除
	Graphics_Linux_Texture_Unmap( GLINUX.Device.Screen.DrawScreenLockImage ) ;

	// 正常終了
	return 0 ;
}

// 裏画面の内容を表画面に描画する
extern	int		Graphics_Hardware_ScreenCopy_PF( int DrawTargetFrontScreenMode_Copy )
{
	// 正常終了
	return 0 ;
}

// ディスプレイの情報をセットアップする
extern int Graphics_SetupDisplayInfo_PF( void )
{
	return 0 ;
}

// 垂直同期信号を待つ
extern	int		Graphics_Hardware_WaitVSync_PF( int SyncNum )
{
	return 0 ;
}

// ScreenFlip 実行時にＶＳＹＮＣ待ちをするかどうかを設定する
extern	int		Graphics_SetWaitVSyncFlag_PF( int Flag )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return 0 ;
	}

	eglSwapInterval( GLINUX.Device.Screen.Display, Flag ? 1 : 0 ) ;

	return 0 ;
}




// 裏画面と表画面を交換する
extern	int		Graphics_ScreenFlipBase_PF( void )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了する
	Graphics_Linux_RenderEnd() ;

	// サブバックバッファの内容を実バッファに転送する
	{
		RECT DestRect ;
		DWORD DestW ;
		DWORD DestH ;
        GLuint ViewFrameBuffer ;
		int BlendMode ;
		int BlendEnable ;
		int BlendRGBSrc ;
		int BlendRGBDest ;
		int BlendRGBOp ;
		int BlendASrc ;
		int BlendADest ;
		int BlendAOp ;
		int NotWriteAlphaChannelFlag ;
		float VertexData[ 4 ][ 4 ] =
		{
			{ -1.0f,  1.0f,  0.0f, 0.0f },
			{  1.0f,  1.0f,  1.0f, 0.0f },
			{ -1.0f, -1.0f,  0.0f, 1.0f },
			{  1.0f, -1.0f,  1.0f, 1.0f },
		} ;

// TODO 
		if( GSYS.Screen.FullScreenFitScalingFlag )
		{
			DestW = GLINUX.Device.Screen.Width ;
			DestH = GLINUX.Device.Screen.Height ;
		}
		else
		{
			DestW = GLINUX.Device.Screen.Width ;
			DestH = GLINUX.Device.Screen.Width * GLINUX.Device.Screen.SubBackBufferTextureSizeY / GLINUX.Device.Screen.SubBackBufferTextureSizeX ;
			if( DestH > ( DWORD )GLINUX.Device.Screen.Height )
			{
				DestW = GLINUX.Device.Screen.Height * GLINUX.Device.Screen.SubBackBufferTextureSizeX / GLINUX.Device.Screen.SubBackBufferTextureSizeY ;
				DestH = GLINUX.Device.Screen.Height ;
			}
		}

		DestRect.left   = ( GLINUX.Device.Screen.Width  - DestW ) / 2 ;
		DestRect.top    = ( GLINUX.Device.Screen.Height - DestH ) / 2 ;
		DestRect.right  = DestRect.left + DestW ;
		DestRect.bottom = DestRect.top  + DestH ;

		// ブレンドモードをブレンド無しに変更
		BlendMode		= GLINUX.Device.State.BlendMode ;
		BlendEnable		= GLINUX.Device.State.BlendEnable ;
		BlendRGBSrc		= GLINUX.Device.State.BlendRGBSrc ;
		BlendRGBDest	= GLINUX.Device.State.BlendRGBDest ;
		BlendRGBOp		= GLINUX.Device.State.BlendRGBOp ;
		BlendASrc		= GLINUX.Device.State.BlendASrc ;
		BlendADest		= GLINUX.Device.State.BlendADest ;
		BlendAOp		= GLINUX.Device.State.BlendAOp ;
		NotWriteAlphaChannelFlag = GLINUX.Device.State.NotWriteAlphaChannelFlag ;
		Graphics_Linux_DeviceState_SetBlendMode( DX_BLENDMODE_NOBLEND, FALSE, DX_BLEND_ONE, DX_BLEND_ZERO, DX_BLENDOP_ADD, DX_BLEND_ONE, DX_BLEND_ZERO, DX_BLENDOP_ADD, FALSE ) ;

		// 描画先をフレームバッファに変更( 設定は Graphics_Linux_DeviceState_RefreshRenderState で戻す )
		glBindFramebuffer( GL_FRAMEBUFFER, 0 ) ;

		// ビューポートをセット( 設定は Graphics_Linux_DeviceState_RefreshRenderState で戻す )
		// glViewport( DestRect.left, DestRect.top, DestRect.right - DestRect.left, DestRect.bottom - DestRect.top ) ;
		// TODO: ウィンドウリサイズ仮対応
		glViewport( 0, 0, GLINUX.Device.Screen.Width, GLINUX.Device.Screen.Height ) ;

		// Ｚバッファを無効にする
		Graphics_Linux_DeviceState_SetDepthEnable( FALSE ) ;
		Graphics_Linux_DeviceState_SetDepthWriteEnable( FALSE ) ;

		// カリングを無効にする
		Graphics_Linux_DeviceState_SetCullMode( DX_CULLING_NONE ) ;

		// 画面をクリア( 設定は Graphics_Linux_DeviceState_RefreshRenderState で戻す )
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ) ;
		glClear( GL_COLOR_BUFFER_BIT ) ;

		// 単純転送シェーダーをセット
		Graphics_Linux_DeviceState_SetShader( &GLINUX.Device.Shader.Base.StretchRect_Shader, FALSE ) ;

		// Uniform の更新
		Graphics_Linux_DeviceState_UpdateShaderUniform( GLINUX.Device.State.SetShader, 0 ) ;

		// テクスチャをセット( 設定は Graphics_Linux_DeviceState_RefreshRenderState で戻す )
		glActiveTexture( GL_TEXTURE0 ) ;
		glBindTexture( GL_TEXTURE_2D, GLINUX.Device.Screen.SubBackBufferTexture ) ;

		// ユーザー指定のメモリイメージがあるかどうかで処理を分岐
		if( GSYS.Screen.UserScreenImage != NULL )
		{
			BYTE *Src = ( BYTE * )GSYS.Screen.UserScreenImage ;
			BYTE *Dst = ( BYTE * )GLINUX.Device.Screen.SubBackBufferTextureTempBuffer ;
			DWORD i ;
			DWORD Bytes ;

			// ネイティブに対応していない場合は変換してから転送
			if( GSYS.Screen.UserScreenImagePixelFormat == DX_USER_SCREEN_PIXEL_FORMAT_X8R8G8B8 &&
				GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_FORMAT_BGRA8888 ] == FALSE )
			{
				Bytes = GLINUX.Device.Screen.SubBackBufferTextureSizeX * GLINUX.Device.Screen.SubBackBufferTextureSizeY * 4 ;
				for( i = 0 ; i < Bytes ; i += 4 )
				{
					Dst[ i + 2 ] = Src[ i + 0 ] ;
					Dst[ i + 1 ] = Src[ i + 1 ] ;
					Dst[ i + 0 ] = Src[ i + 2 ] ;
				}

				glTexSubImage2D(
					GL_TEXTURE_2D,
					0,
					0,
					0,
					GLINUX.Device.Screen.SubBackBufferTextureSizeX,
					GLINUX.Device.Screen.SubBackBufferTextureSizeY,
					GLINUX.Device.Screen.SubBackBufferTexturePixelFormat,
					GLINUX.Device.Screen.SubBackBufferTexturePixelType,
					GLINUX.Device.Screen.SubBackBufferTextureTempBuffer
				) ;
			}
			else
			if( GSYS.Screen.UserScreenImagePixelFormat == DX_USER_SCREEN_PIXEL_FORMAT_X1R5G5B5 )
			{
				Bytes = GLINUX.Device.Screen.SubBackBufferTextureSizeX * GLINUX.Device.Screen.SubBackBufferTextureSizeY * 2 ;
				for( i = 0 ; i < Bytes ; i += 2 )
				{
					*( ( WORD * )&Dst[ i ] ) = ( WORD )( ( *( ( WORD * )&Src[ i ] ) << 1 ) | 1 ) ;
				}

				glTexSubImage2D(
					GL_TEXTURE_2D,
					0,
					0,
					0,
					GLINUX.Device.Screen.SubBackBufferTextureSizeX,
					GLINUX.Device.Screen.SubBackBufferTextureSizeY,
					GLINUX.Device.Screen.SubBackBufferTexturePixelFormat,
					GLINUX.Device.Screen.SubBackBufferTexturePixelType,
					GLINUX.Device.Screen.SubBackBufferTextureTempBuffer
				) ;
			}
			else
			{
				glTexSubImage2D(
					GL_TEXTURE_2D,
					0,
					0,
					0,
					GLINUX.Device.Screen.SubBackBufferTextureSizeX,
					GLINUX.Device.Screen.SubBackBufferTextureSizeY,
					GLINUX.Device.Screen.SubBackBufferTexturePixelFormat,
					GLINUX.Device.Screen.SubBackBufferTexturePixelType,
					GSYS.Screen.UserScreenImage
				) ;
			}
		}
		else
		// ソフトウェアレンダリングモードの場合はここでテクスチャを更新
		if( GSYS.Setting.NotUseHardware )
		{
			// ネイティブに対応している場合は単純転送
			if( GSYS.Screen.MainScreenColorBitDepth == 16 || GLINUX.Device.Caps.Extensions[ LINUX_GL_EXTENSION_TEXTURE_FORMAT_BGRA8888 ] )
			{
				glTexSubImage2D(
					GL_TEXTURE_2D,
					0,
					0,
					0,
					GLINUX.Device.Screen.SubBackBufferTextureSizeX,
					GLINUX.Device.Screen.SubBackBufferTextureSizeY,
					GLINUX.Device.Screen.SubBackBufferTexturePixelFormat,
					GLINUX.Device.Screen.SubBackBufferTexturePixelType,
					GSYS.SoftRender.MainBufferMemImg.UseImage
				) ;
			}
			else
			{
				BYTE *Src ;
				BYTE *Dst ;
				DWORD i ;
				DWORD Bytes ;

				// 対応していない場合は変換してから転送
				Src = GSYS.SoftRender.MainBufferMemImg.UseImage ;
				Dst = ( BYTE * )GLINUX.Device.Screen.SubBackBufferTextureTempBuffer ;
				Bytes = GLINUX.Device.Screen.SubBackBufferTextureSizeX * GLINUX.Device.Screen.SubBackBufferTextureSizeY * 4 ;
				for( i = 0 ; i < Bytes ; i += 4 )
				{
					Dst[ i + 2 ] = Src[ i + 0 ] ;
					Dst[ i + 1 ] = Src[ i + 1 ] ;
					Dst[ i + 0 ] = Src[ i + 2 ] ;
				}

				glTexSubImage2D(
					GL_TEXTURE_2D,
					0,
					0,
					0,
					GLINUX.Device.Screen.SubBackBufferTextureSizeX,
					GLINUX.Device.Screen.SubBackBufferTextureSizeY,
					GLINUX.Device.Screen.SubBackBufferTexturePixelFormat,
					GLINUX.Device.Screen.SubBackBufferTexturePixelType,
					GLINUX.Device.Screen.SubBackBufferTextureTempBuffer
				) ;
			}
		}

		// 拡大モードをセット
		{
			GLint FilterMode = GL_LINEAR ;
			switch( GSYS.Screen.FullScreenScalingMode )
			{
			case DX_FSSCALINGMODE_BILINEAR :	FilterMode = GL_LINEAR ;	break ;
			case DX_FSSCALINGMODE_NEAREST :		FilterMode = GL_NEAREST ;	break ;
			}
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterMode ) ;
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterMode ) ;
		}

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;

		// 頂点データのセットアップ
		Graphics_Linux_DeviceState_SetupShaderVertexData(
			&GLINUX.Device.Shader.Base.StretchRect_Shader,
			&g_StretchRectVertexInputInfo,
			VertexData
		) ;

		// 描画
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) ;
		GSYS.PerformanceInfo.NowFrameDrawCallCount ++ ;

		// ブレンドモードを元に戻す
		Graphics_Linux_DeviceState_SetBlendMode(
			BlendMode,
			BlendEnable,
			BlendRGBSrc,
			BlendRGBDest,
			BlendRGBOp,
			BlendASrc,
			BlendADest,
			BlendAOp,
			NotWriteAlphaChannelFlag
		) ;
	}

	// スワップする
	eglSwapBuffers( GLINUX.Device.Screen.Display, GLINUX.Device.Screen.Surface );

	// 今までの設定を復帰する
	Graphics_Linux_DeviceState_RefreshRenderState() ;

	return 0 ;
}

// メイン画面のＺバッファの設定を変更する
extern	int		Graphics_Hardware_SetZBufferMode_PF( int ZBufferSizeX, int ZBufferSizeY, int ZBufferBitDepth )
{
	// 未実装
	return 0 ;
}

// 描画先Ｚバッファのセット
extern	int		Graphics_Hardware_SetDrawZBuffer_PF( int DrawScreen, IMAGEDATA *Image )
{
	// 未実装
	return 0 ;
}

// 現在の画面のリフレッシュレートを取得する
extern	int		Graphics_GetRefreshRate_PF( void )
{
	return 60 ;
}
























// 環境依存情報取得関係

// GetColor や GetColor2 で使用するカラーデータを取得する
extern const COLORDATA *Graphics_Hardware_GetMainColorData_PF( void )
{
	return Graphics_Linux_GetDataFormatColorData( LINUX_PIXEL_FORMAT_R8G8B8A8 ) ;
}

// ディスプレーのカラーデータポインタを得る
extern	const COLORDATA *Graphics_Hardware_GetDispColorData_PF( void )
{
	return Graphics_Linux_GetDataFormatColorData( GLINUX.Device.Caps.ScreenFormat ) ;
}

// 指定座標の色を取得する
extern	DWORD Graphics_Hardware_GetPixel_PF( int x, int y )
{
	RECT            SrcRect ;
//	const COLORDATA *DestColorData ;
	BASEIMAGE       BufferImage ;
	DWORD           Ret = 0xffffffff ;
	int	Red, Green, Blue, Alpha ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// 描画先バッファをロック
	SrcRect.left   = x ;
	SrcRect.right  = x + 1 ;
	SrcRect.top    = y ;
	SrcRect.bottom = y + 1 ;
	if( Graphics_Screen_LockDrawScreen( &SrcRect, &BufferImage, -1, -1, -1, TRUE, 0 ) < 0 )
	{
		return Ret ;
	}

	if( BufferImage.ColorData.FloatTypeFlag )
	{
		float RedF = 0.0f, GreenF = 0.0f, BlueF = 0.0f, AlphaF = 0.0f ;

		switch( BufferImage.ColorData.ChannelBitDepth )
		{
		case 16 :
			RedF   =                                        Float16ToFloat32( ( ( WORD * )BufferImage.GraphData )[ 0 ] ) ;
			GreenF = BufferImage.ColorData.ChannelNum > 1 ? Float16ToFloat32( ( ( WORD * )BufferImage.GraphData )[ 1 ] ) : 1.0f ;
			BlueF  = BufferImage.ColorData.ChannelNum > 2 ? Float16ToFloat32( ( ( WORD * )BufferImage.GraphData )[ 2 ] ) : 1.0f ;
			AlphaF = BufferImage.ColorData.ChannelNum > 3 ? Float16ToFloat32( ( ( WORD * )BufferImage.GraphData )[ 3 ] ) : 1.0f ;
			break ;

		case 32 :
			RedF   =                                        ( ( float * )BufferImage.GraphData )[ 0 ] ;
			GreenF = BufferImage.ColorData.ChannelNum > 1 ? ( ( float * )BufferImage.GraphData )[ 1 ] : 1.0f ;
			BlueF  = BufferImage.ColorData.ChannelNum > 2 ? ( ( float * )BufferImage.GraphData )[ 2 ] : 1.0f ;
			AlphaF = BufferImage.ColorData.ChannelNum > 3 ? ( ( float * )BufferImage.GraphData )[ 3 ] : 1.0f ;
			break ;
		}

		Red   = RedF   <= 0.0f ? 0 : ( RedF   >= 1.0f ? 255 : ( int )( RedF   * 255.0f ) ) ;
		Green = GreenF <= 0.0f ? 0 : ( GreenF >= 1.0f ? 255 : ( int )( GreenF * 255.0f ) ) ;
		Blue  = BlueF  <= 0.0f ? 0 : ( BlueF  >= 1.0f ? 255 : ( int )( BlueF  * 255.0f ) ) ;
		Alpha = AlphaF <= 0.0f ? 0 : ( AlphaF >= 1.0f ? 255 : ( int )( AlphaF * 255.0f ) ) ;
	}
	else
	{
		switch( BufferImage.ColorData.ColorBitDepth )
		{
		case 16 :
			NS_GetColor5( &BufferImage.ColorData, *( ( WORD   * )BufferImage.GraphData ), &Red, &Green, &Blue, &Alpha ) ;
			break ;

		case 32 :
			NS_GetColor5( &BufferImage.ColorData, *( ( DWORD  * )BufferImage.GraphData ), &Red, &Green, &Blue, &Alpha ) ;
			break ;
		}
	}

	Ret = 0xff000000 | ( ( ( unsigned int )Red ) << 16 ) | ( ( ( unsigned int )Green ) << 8 ) | ( ( unsigned int )Blue ) ;

	// 描画先バッファをアンロック
	Graphics_Screen_UnlockDrawScreen() ;

	// 色を返す
	return Ret ;
}

// 指定座標の色を取得する( float型 )
extern COLOR_F Graphics_Hardware_GetPixelF_PF( int x, int y )
{
	RECT            SrcRect ;
//	const COLORDATA *DestColorData ;
	BASEIMAGE       BufferImage ;
	COLOR_F			Result ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// 描画先バッファをロック
	SrcRect.left   = x ;
	SrcRect.right  = x + 1 ;
	SrcRect.top    = y ;
	SrcRect.bottom = y + 1 ;
	if( Graphics_Screen_LockDrawScreen( &SrcRect, &BufferImage, -1, -1, -1, TRUE, 0 ) < 0 )
	{
		return NS_GetColorF( -1.0f, -1.0f, -1.0f, -1.0f ) ;
	}

	if( BufferImage.ColorData.FloatTypeFlag )
	{
		switch( BufferImage.ColorData.ChannelBitDepth )
		{
		case 16 :
			Result.r =                                        Float16ToFloat32( ( ( WORD * )BufferImage.GraphData )[ 0 ] ) ;
			Result.g = BufferImage.ColorData.ChannelNum > 1 ? Float16ToFloat32( ( ( WORD * )BufferImage.GraphData )[ 1 ] ) : 1.0f ;
			Result.b = BufferImage.ColorData.ChannelNum > 2 ? Float16ToFloat32( ( ( WORD * )BufferImage.GraphData )[ 2 ] ) : 1.0f ;
			Result.a = BufferImage.ColorData.ChannelNum > 3 ? Float16ToFloat32( ( ( WORD * )BufferImage.GraphData )[ 3 ] ) : 1.0f ;
			break ;

		case 32 :
			Result.r =                                        ( ( float * )BufferImage.GraphData )[ 0 ] ;
			Result.g = BufferImage.ColorData.ChannelNum > 1 ? ( ( float * )BufferImage.GraphData )[ 1 ] : 1.0f ;
			Result.b = BufferImage.ColorData.ChannelNum > 2 ? ( ( float * )BufferImage.GraphData )[ 2 ] : 1.0f ;
			Result.a = BufferImage.ColorData.ChannelNum > 3 ? ( ( float * )BufferImage.GraphData )[ 3 ] : 1.0f ;
			break ;
		}
	}
	else
	{
		int Red, Green, Blue, Alpha ;

		switch( BufferImage.ColorData.ColorBitDepth )
		{
		case 16 :
			NS_GetColor5( &BufferImage.ColorData, *( ( WORD   * )BufferImage.GraphData ), &Red, &Green, &Blue, &Alpha ) ;
			break ;

		case 32 :
			NS_GetColor5( &BufferImage.ColorData, *( ( DWORD  * )BufferImage.GraphData ), &Red, &Green, &Blue, &Alpha ) ;
			break ;
		}

		Result.r = ( float )Red   / 255.0f ;
		Result.g = ( float )Green / 255.0f ;
		Result.b = ( float )Blue  / 255.0f ;
		Result.a = ( float )Alpha / 255.0f ;
	}

	// 描画先バッファをアンロック
	Graphics_Screen_UnlockDrawScreen() ;

	// 色を返す
	return Result ;
}

























// 環境依存画像関係

// YUVサーフェスを使った Theora 動画の内容をグラフィックスハンドルのテクスチャに転送する
extern	int		Graphics_Hardware_UpdateGraphMovie_TheoraYUV_PF( MOVIEGRAPH *Movie, IMAGEDATA *Image )
{
	// 未実装
	return 0 ;
}

// グラフィックメモリ領域のロック
extern	int		Graphics_Hardware_GraphLock_PF( IMAGEDATA *Image, COLORDATA **ColorDataP, int WriteOnly )
{
	COLORDATA    *TargetColorData ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// カラーフォーマットを取得する
	TargetColorData = Graphics_Linux_GetDataFormatColorData( GLINUX.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ] ) ;

	// 標準フォーマット以外ではロックできない
	if( TargetColorData->Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		return -1 ;
	}

	// テンポラリバッファを確保する
	Image->LockImagePitch	= ( DWORD )( TargetColorData->PixelByte * Image->WidthI ) ;
	Image->LockImage		= ( BYTE * )DXALLOC( Image->LockImagePitch * Image->HeightI ) ;
	if( Image->LockImage == NULL )
	{
		DXST_LOGFILE_ADDUTF16LE( "\xed\x30\xc3\x30\xaf\x30\x28\x75\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x3c\x68\x0d\x7d\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ロック用テンポラリイメージ格納用メモリの確保に失敗しました\n" @*/ ) ;
		goto ERR ;
	}

	// 書き込み専用ではない場合はテンポラリバッファにデータを転送する
	if( WriteOnly == FALSE )
	{
		int                  i ;
		BYTE                *Dest ;
		BYTE                *Src ;
//		DWORD                WidthByte ;
		BASEIMAGE            MapBaseImage ;
		void *				 MapTempImage ;
		IMAGEDATA_HARD_DRAW *DrawTex ;
		COLORDATA			SrcColorData ;
		POINT				DestPoint = { 0, 0 } ;
		RECT				SrcRect ;

		NS_CreateARGB8ColorData( &SrcColorData ) ;

		// 描画情報の数だけ繰り返し
		DrawTex = Image->Hard.Draw ;
		for( i = 0 ; i < Image->Hard.DrawNum ; i ++, DrawTex ++ )
		{
			// マップ
			if( Graphics_Linux_Texture_Map(
				DrawTex->Tex->PF->Texture.TextureBuffer,
				DrawTex->Tex->PF->FrameBuffer,
				DrawTex->Tex->PF->Texture.Width,
				DrawTex->Tex->PF->Texture.Height,
				NULL,
				&MapBaseImage,
				&MapTempImage ) < 0 )
			{
				goto ERR ;
			}

			// 転送
			Src  = ( BYTE * )MapBaseImage.GraphData + DrawTex->UsePosXI  * TargetColorData->PixelByte + DrawTex->UsePosYI  * MapBaseImage.Pitch ;
			Dest = Image->LockImage                 + DrawTex->DrawPosXI * TargetColorData->PixelByte + DrawTex->DrawPosYI * Image->LockImagePitch ;
//			WidthByte = ( DWORD )( DrawTex->WidthI * TargetColorData->PixelByte ) ;
			SrcRect.left	= 0 ;
			SrcRect.top		= 0 ;
			SrcRect.right	= MapBaseImage.Width ;
			SrcRect.bottom	= MapBaseImage.Height ;
			NS_GraphColorMatchBltVer2(
				Dest, Image->LockImagePitch, TargetColorData,
				Src,  MapBaseImage.Pitch,    &SrcColorData,
				NULL, 0,                     NULL,
				DestPoint, &SrcRect, FALSE,
				FALSE, 0,
				BASEIM.ImageShavedMode, FALSE,
				FALSE, FALSE,
				FALSE
			) ;

			// マップの解除
			Graphics_Linux_Texture_Unmap( MapTempImage ) ;
		}
	}

	// カラーデータのアドレスを保存
	*ColorDataP = TargetColorData ;

	// 正常終了
	return 0 ;

ERR :

	if( Image->LockImage != NULL )
	{
		DXFREE( Image->LockImage ) ;
		Image->LockImage = NULL ;
	}

	// エラー終了
	return -1 ;
}

// グラフィックメモリ領域のロック解除
extern	int		Graphics_Hardware_GraphUnlock_PF( IMAGEDATA *Image )
{
	COLORDATA           *ColorData ;
	BASEIMAGE            BaseImage ;
	BASEIMAGE            AlphaBaseImage ;
	RECT                 Rect ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// カラーフォーマットを取得する
	ColorData = Graphics_Linux_GetDataFormatColorData( GLINUX.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ] ) ;

	// 基本イメージを構築する
	_MEMSET( &AlphaBaseImage, 0, sizeof( AlphaBaseImage ) ) ;
	BaseImage.ColorData      = *ColorData ;
	BaseImage.Width          = Image->WidthI ;
	BaseImage.Height         = Image->HeightI ;
	BaseImage.Pitch          = ColorData->PixelByte * Image->WidthI ;
	BaseImage.GraphData      = Image->LockImage ;
	BaseImage.MipMapCount    = 0 ;
	BaseImage.GraphDataCount = 0 ;

	// 転送
	Rect.left   = 0 ;
	Rect.top    = 0 ;
	Rect.right  = Image->WidthI ;
	Rect.bottom = Image->HeightI ;
	Graphics_Hardware_BltBmpOrBaseImageToGraph3_PF( &Rect, 0, 0, Image->HandleInfo.Handle, &BaseImage, &AlphaBaseImage, FALSE, FALSE, TRUE, FALSE ) ;

	// ロックイメージの解放
	if( Image->LockImage )
	{
		DXFREE( Image->LockImage ) ;
		Image->LockImage = NULL ;
	}

	// 正常終了
	return 0 ;
}

// グラフィックのＺバッファの状態を別のグラフィックのＺバッファにコピーする( DestGrHandle も SrcGrHandle もＺバッファを持っている描画可能画像で、且つアンチエイリアス画像ではないことが条件 )
extern	int		Graphics_Hardware_CopyGraphZBufferImage_PF( IMAGEDATA *DestImage, IMAGEDATA *SrcImage )
{
	// 未実装
	return 0 ;
}

// 画像データの初期化
extern	int		Graphics_Hardware_InitGraph_PF( void )
{
	// 正常終了
	return 0 ;
}

// グラフィックを特定の色で塗りつぶす
extern	int		Graphics_Hardware_FillGraph_PF( IMAGEDATA *Image, int Red, int Green, int Blue, int Alpha, int ASyncThread )
{
	// 標準処理を行う
	return 2 ;
}

// 描画可能画像やバックバッファから指定領域のグラフィックを取得する
extern	int		Graphics_Hardware_GetDrawScreenGraphBase_PF( IMAGEDATA *Image, IMAGEDATA *TargetImage, int TargetScreen, int TargetScreenSurface, int TargetScreenMipLevel, int TargetScreenWidth, int TargetScreenHeight, int x1, int y1, int x2, int y2, int destX, int destY )
{
	RECT SrcRect ;
	RECT DestRect ;
	RECT Rect ;
	int Width, Height ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO

	// 描画を終了しておく
	Graphics_Linux_RenderEnd() ;

	// 幅と高さを計算
	Width  = x2 - x1 ;
	Height = y2 - y1 ;

	// 転送先となるサーフェスが描画可能テクスチャだった場合は単純に転送
	if( Image->Orig->FormatDesc.DrawValidFlag )
	{
//		GLuint TargetFrameBuffer ;
		GLuint TargetTexture ;
		GLuint TargetWidth ;
		GLuint TargetHeight ;

		// 取り込み元となるテクスチャの決定

		// 描画可能画像が対象の場合
		if( TargetImage )
		{
//			TargetFrameBuffer = TargetImage->Orig->Hard.Tex[ 0 ].PF->FrameBuffer ;
			TargetTexture     = TargetImage->Orig->Hard.Tex[ 0 ].PF->Texture.TextureBuffer ;
			TargetWidth       = TargetImage->Orig->Hard.Tex[ 0 ].PF->Texture.Width ;
			TargetHeight      = TargetImage->Orig->Hard.Tex[ 0 ].PF->Texture.Height ;
		}
		else
		{
//			TargetFrameBuffer = GLINUX.Device.Screen.SubBackBufferFrameBuffer ;
			TargetTexture     = GLINUX.Device.Screen.SubBackBufferTexture ;
			TargetWidth       = GLINUX.Device.Screen.SubBackBufferTextureSizeX ;
			TargetHeight      = GLINUX.Device.Screen.SubBackBufferTextureSizeY ;
		}

		SrcRect.left    = x1 ;
		SrcRect.top     = y1 ;
		SrcRect.right   = x2 ;
		SrcRect.bottom  = y2 ;

		DestRect.left   = destX ;
		DestRect.top    = destY ;
		DestRect.right  = destX + Width ;
		DestRect.bottom = destY + Height ;

		Graphics_Linux_StretchRect(
			TargetTexture, TargetWidth, TargetHeight, &SrcRect,
			Image->Orig->Hard.Tex[ 0 ].PF->FrameBuffer,
			Image->Orig->Hard.Tex[ 0 ].PF->Texture.Width,
			Image->Orig->Hard.Tex[ 0 ].PF->Texture.Height,
			&DestRect
		) ; 

		// 描画を終了しておく
		Graphics_Linux_RenderEnd() ;
	}
	else
	// 通常のテクスチャだった場合は最初にロックを試す
	{
		// 描画先バッファをロック
		BASEIMAGE LockImage ;
		SrcRect.left   = x1 ;
		SrcRect.top    = y1 ;
		SrcRect.right  = x2 ;
		SrcRect.bottom = y2 ;
		if( Graphics_Screen_LockDrawScreen( &SrcRect, &LockImage, TargetScreen, TargetScreenSurface, TargetScreenMipLevel, TRUE, 0 ) < 0 )
		{
			return -1 ;
		}

		// テクスチャに転送
		Rect.left   = 0 ;
		Rect.top    = 0 ;
		Rect.right  = LockImage.Width ;
		Rect.bottom = LockImage.Height ;
		Graphics_Image_BltBmpOrBaseImageToGraph3(
			&Rect,
			destX, destY,
			Image->HandleInfo.Handle,
			&LockImage, NULL,
			FALSE,
			FALSE,
			FALSE
		) ;

		// 描画先バッファのアンロック
		Graphics_Screen_UnlockDrawScreen() ;
	}

	// 正常終了
	return 0 ;
}

static int Graphics_Linux_BltBmpOrBaseImageToGraph3_MipMapBlt(
	      IMAGEDATA_ORIG	*Orig,
	const RECT				*SrcRect,
	const RECT				*DestRect,
	      GLuint			UseTex,
	      int				TexWidth,
	      int				TexHeight,
	const BASEIMAGE			*RgbBaseImage,
	const BASEIMAGE			*AlphaBaseImage,
	      int				DestColorFormat,
	      int				RedIsAlphaFlag,
	      int				UseTransColorConvAlpha,
	      int				ASyncThread
)
{
	int			ImageSize ;
	int			ImageNum ;
	void		*ImageBuffer ;
	int			i ;
	int			j ;
	int			k ;
	DWORD		MipMapCount ;
	const COLORDATA			*DestColor ;
	PIXELFORMAT_INFO_LINUX	*PixelFormat ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// テクスチャのカラー情報を取得する
	DestColor = Graphics_Linux_GetDataFormatColorData( DestColorFormat ) ;
	PixelFormat = &g_LinuxPixelFormat[ DestColorFormat ] ;

	MipMapCount = Orig->Hard.MipMapCount > 16 ? 16 : Orig->Hard.MipMapCount ;

	ImageNum = Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1 ;

	// 転送先がテクスチャ全体で、且つ転送元にミップマップ情報がある場合は分岐
	if( DestRect->left == 0 && DestRect->right  == TexWidth  &&
		DestRect->top  == 0 && DestRect->bottom == TexHeight &&
		RgbBaseImage->MipMapCount >= Orig->Hard.MipMapCount )
	{
		ImageBuffer = RgbBaseImage->GraphData ;
		for( i = 0 ; i < ImageNum ; i ++ )
		{
			int Width ;
			int Height ;

			ImageSize = RgbBaseImage->Width * RgbBaseImage->Height * RgbBaseImage->ColorData.ColorBitDepth / 8 ;
			Width  = RgbBaseImage->Width ;
			Height = RgbBaseImage->Height ;
			for( j = 0 ; ( DWORD )j < MipMapCount ; j ++ )
			{
				// テクスチャへ転送
				glTexSubImage2D_ASync(
					UseTex,
					GL_TEXTURE_2D,
					i,
					0,
					0,
					Width,
					Height,
					PixelFormat->Format,
					PixelFormat->Type,
					ImageBuffer,
					ASyncThread
				) ;

				// 次のイメージのサイズをセット
				Width  >>= 1 ;
				Height >>= 1 ;
				if( Width  == 0 ) Width  = 1 ;
				if( Height == 0 ) Height = 1 ;

				// 転送元アドレスを進める
				ImageBuffer  = ( BYTE * )ImageBuffer + ImageSize ;
				ImageSize   /= 4 ;
				if( ImageSize < 8 )
				{
					ImageSize = 8 ;
				}
			}
		}
	}
	else
	{
		int		ImageW ;
		int		ImageH ;
		int		ImageDW ;
		int		ImageDH ;
		int		ImageSW ;
		int		ImageSH ;
		void	*Image1 ;
		void	*Image2 ;
		void	*ImageD ;
		void	*ImageS ;
		void	*ImageTmp ;
		int		ImagePitch ;
		int		ImagePitchD ;
		int		ImagePitchS ;
		int		ts ;
		POINT	DestPoint ;
		int		RgbImageSize ;
		int		AlphaImageSize ;

		DestPoint.x = 0 ;
		DestPoint.y = 0 ;

		// 一時バッファに格納
		ts = DestRect->right  - DestRect->left ;
		for( ImageW = 1 ; ImageW < ts ; ImageW <<= 1 ){}
		ts = DestRect->bottom - DestRect->top  ;
		for( ImageH = 1 ; ImageH < ts ; ImageH <<= 1 ){}
		ImagePitch  = DestColor->PixelByte * ImageW ;
		ImageSize   = ImagePitch * ImageH ;

		// ピッチは４の倍数にする
		ImagePitch = ( ImagePitch + 3 ) / 4 * 4 ;

		if( ASyncThread )
		{
			ImageBuffer = DXALLOC( ( size_t )( ImageSize * 3 ) ) ;
			if( ImageBuffer == NULL )
			{
				DXST_LOGFILE_ADDUTF16LE( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x78\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x3b\x75\xcf\x50\x92\x30\x00\x4e\x42\x66\x84\x76\x6b\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"テクスチャへ転送する画像を一時的に格納するメモリ領域の確保に失敗しました\n" @*/ ) ;
				return -1 ;
			}
		}
		else
		{
			if( Graphics_Linux_Texture_SetupCommonBuffer( ( unsigned int )( ImageSize * 3 ) ) < 0 )
			{
				return -1 ;
			}
			ImageBuffer = GLINUX.Texture.CommonBuffer ;
		}
		Image1      = ImageBuffer ;
		Image2      = ( BYTE * )Image1 + ImageSize ;
		ImageTmp	= ( BYTE * )Image2 + ImageSize ;

		RgbImageSize   = RgbBaseImage->Pitch   * RgbBaseImage->Height ;
		AlphaImageSize = AlphaBaseImage->Pitch * AlphaBaseImage->Height ;
		for( i = 0 ; i < ImageNum ; i ++ )
		{
			// ミップマップの深さだけ繰り返し
			ImageSW     = ImageW ;
			ImageSH     = ImageH ;
			ImagePitchS = ImagePitch ;
			ImageDW     = ImageW ;
			ImageDH     = ImageH ;
			ImagePitchD = ImagePitch ;
			for( j = 0 ; j < Orig->Hard.MipMapCount ; j ++ )
			{
				// 縮小画像の作成
				if( j == 0 )
				{
					// level0 の時は等倍の出力フォーマット画像を作成する
					NS_GraphColorMatchBltVer2(
						Image1,                                                   ImagePitch,            DestColor,
						( BYTE * )RgbBaseImage->GraphData   + RgbImageSize   * i, RgbBaseImage->Pitch,   &RgbBaseImage->ColorData,
						( BYTE * )AlphaBaseImage->GraphData + AlphaImageSize * i, AlphaBaseImage->Pitch, &AlphaBaseImage->ColorData,
						DestPoint, SrcRect, FALSE,
						UseTransColorConvAlpha && ( DestColor->AlphaWidth != 0 && AlphaBaseImage->GraphData == NULL ), Orig->TransCode,
						BASEIM.ImageShavedMode, FALSE,
						RedIsAlphaFlag, FALSE,
						FALSE ) ;
					ImageD = Image1 ;
					ImagePitchD = ImagePitch ;
				}
				else
				{
					if( j & 1 )
					{
						ImageS = Image1 ;
						ImageD = Image2 ;
					}
					else
					{
						ImageS = Image2 ;
						ImageD = Image1 ;
					}

					GraphHalfScaleBlt( DestColor, ImageD, ImagePitchD, ImageS, ImagePitchS, 0, 0, 0, 0, ImageSW, ImageSH ) ;
				}

				// テクスチャに転送
				{
					RECT TempRect ;
					int ih, iw, ip ;
					BYTE *id, *is ;

					TempRect.left   = DestRect->left   >> j ;
					TempRect.top    = DestRect->top    >> j ;
					TempRect.right  = DestRect->right  >> j ;
					TempRect.bottom = DestRect->bottom >> j ;
					if( TempRect.right == TempRect.left )
					{
						TempRect.right = TempRect.left + 1 ;
					}
					if( TempRect.bottom == TempRect.top )
					{
						TempRect.bottom = TempRect.top + 1 ;
					}

					ih = TempRect.bottom - TempRect.top ;
					iw = TempRect.right - TempRect.left ;
					ip = iw * DestColor->PixelByte ;
					is = ( BYTE * )ImageD ;
					id = ( BYTE * )ImageTmp ;
					for( k = 0 ; k < ih; k++ )
					{
						_MEMCPY( id, is, ip ) ;
						id += ip ;
						is += ImagePitchD ;
					}

					glTexSubImage2D_ASync(
						UseTex,
						GL_TEXTURE_2D,
						j,
						TempRect.left,
						TempRect.top,
						TempRect.right  - TempRect.left,
						TempRect.bottom - TempRect.top,
						PixelFormat->Format,
						PixelFormat->Type,
						ImageTmp,
						ASyncThread
					) ;
				}

				// 次のイメージのサイズをセット
				ImageSW = ImageDW ;
				ImageSH = ImageDH ;
				ImagePitchS = ImagePitchD ;
				ImageDW >>= 1 ;
				ImageDH >>= 1 ;
				if( ImageDW == 0 ) ImageDW = 1 ;
				if( ImageDH == 0 ) ImageDH = 1 ;
				ImagePitchD = ImageDW * DestColor->PixelByte ;

				// ピッチは４の倍数にする
				ImagePitchD = ( ImagePitchD + 3 ) / 4 * 4 ;
			}
		}

		// メモリの解放
		if( ASyncThread )
		{
			DXFREE( ImageBuffer ) ;
		}
	}

	// 正常終了
	return 0 ;
}

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
)
{
	int				i ;
	int				k ;
	POINT			DestPoint ;
	int				H ;
	BYTE			*DestAddr ;
	BYTE			*SrcRgbAddr ;
	BYTE			*SrcAlphaAddr ;
	int				RightLineFillFlag ;
	int				BottomLineFillFlag ;
	DWORD			ImageSize ;
	DWORD			AlphaImageSize ;
	const COLORDATA *DestColor ;
	PIXELFORMAT_INFO_LINUX	*PixelFormat ;
	void			*TempBuffer = NULL ;
	unsigned int	TempBufferWidth ;
	unsigned int	TempBufferHeight ;
	unsigned int	TempBufferSize ;
	int				TempBufferPitch = 0 ;

	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// テクスチャのカラー情報を取得する
	DestColor = Graphics_Linux_GetDataFormatColorData( DestColorFormat ) ;
	PixelFormat = &g_LinuxPixelFormat[ DestColorFormat ] ;

	DestPoint.x = 0 ;
	DestPoint.y = 0 ;

	RightLineFillFlag  = FALSE ;
	BottomLineFillFlag = FALSE ;

	if( TexUseWidth  < TexWidth  && TexUseWidth  == DestRect->right )
	{
		RightLineFillFlag = TRUE ;
	}
	if( TexUseHeight < TexHeight && TexUseHeight == DestRect->bottom )
	{
		BottomLineFillFlag = TRUE ;
	}

	TempBufferWidth  = ( unsigned int )( DestRect->right  - DestRect->left ) ;
	TempBufferHeight = ( unsigned int )( DestRect->bottom - DestRect->top ) ;
	TempBufferPitch  =                   ( TempBufferWidth  + ( RightLineFillFlag  ? 1 : 0 ) ) * DestColor->PixelByte ;
	TempBufferSize   = ( unsigned int )( ( TempBufferHeight + ( BottomLineFillFlag ? 1 : 0 ) ) * TempBufferPitch ) ;

	// ピッチは４の倍数にする
	TempBufferPitch = ( TempBufferPitch + 3 ) / 4 * 4 ;

	if( ASyncThread )
	{
		TempBuffer      = DXALLOC( TempBufferSize ) ;
		if( TempBuffer == NULL )
		{
			DXST_LOGFILEFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6b\x30\x3b\x75\xcf\x50\x92\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5b\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x50\x00\x53\x00\x34\x00\x5f\x00\x42\x00\x6c\x00\x74\x00\x42\x00\x6d\x00\x70\x00\x4f\x00\x72\x00\x42\x00\x61\x00\x73\x00\x65\x00\x49\x00\x6d\x00\x61\x00\x67\x00\x65\x00\x54\x00\x6f\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x33\x00\x5f\x00\x4e\x00\x6f\x00\x4d\x00\x69\x00\x70\x00\x4d\x00\x61\x00\x70\x00\x42\x00\x6c\x00\x74\x00\x5d\x00\x0a\x00\x00"/*@ L"テクスチャに画像を転送するためのメモリの確保に失敗しました[Graphics_Linux_BltBmpOrBaseImageToGraph3_NoMipMapBlt]\n" @*/ )) ;
			return -1 ;
		}
	}
	else
	{
		if( Graphics_Linux_Texture_SetupCommonBuffer( TempBufferSize ) < 0 )
		{
			return -1 ;
		}
		TempBuffer      = GLINUX.Texture.CommonBuffer ;
	}

	for( i = 0 ; i < ImageNum ; i ++ )
	{
		// 転送処理

		ImageSize      = ( DWORD )( RgbBaseImage->Pitch   * RgbBaseImage->Height   ) ;
		AlphaImageSize = ( DWORD )( AlphaBaseImage->Pitch * AlphaBaseImage->Height ) ;

		SrcRgbAddr   = ( BYTE * )RgbBaseImage->GraphData   + i * ImageSize ;
		SrcAlphaAddr = ( BYTE * )AlphaBaseImage->GraphData + i * AlphaImageSize ;

		// テンポラリバッファに画像データを転送する
		{
			NS_GraphColorMatchBltVer2(
				TempBuffer,     TempBufferPitch,       DestColor,
				SrcRgbAddr,     RgbBaseImage->Pitch,   &RgbBaseImage->ColorData,
				SrcAlphaAddr,   AlphaBaseImage->Pitch, &AlphaBaseImage->ColorData,
				DestPoint, SrcRect, FALSE,
				UseTransColorConvAlpha && ( DestColor->AlphaWidth != 0 && AlphaBaseImage->GraphData == NULL ), TransCode,
				BASEIM.ImageShavedMode, FALSE,
				RedIsAlphaFlag, FALSE,
				FALSE
			) ;

			if( BottomLineFillFlag )
			{
				_MEMCPY( ( BYTE * )TempBuffer + TempBufferHeight * TempBufferPitch,
						 ( BYTE * )TempBuffer + ( TempBufferHeight - 1 ) * TempBufferPitch,
						 ( size_t )( DestColor->PixelByte * ( SrcRect->right - SrcRect->left ) ) ) ;
			}
			if( RightLineFillFlag )
			{
				H = SrcRect->bottom - SrcRect->top ;
				DestAddr = ( BYTE * )TempBuffer + TempBufferWidth * DestColor->PixelByte ;
				switch( DestColor->PixelByte )
				{
				case 2 :
					for( k = 0 ; k < H ; k ++, DestAddr += TempBufferPitch )
					{
						*( ( WORD * )DestAddr ) = *( ( WORD * )DestAddr - 1 ) ;
					}
					break ;

				case 4 :
					for( k = 0 ; k < H ; k ++, DestAddr += TempBufferPitch )
					{
						*( ( DWORD * )DestAddr ) = *( ( DWORD * )DestAddr - 1 ) ;
					}
					break ;
				}
			}
			if( BottomLineFillFlag && RightLineFillFlag )
			{
				DestAddr = ( BYTE * )TempBuffer + TempBufferHeight * TempBufferPitch + TempBufferWidth * DestColor->PixelByte ;
				switch( DestColor->PixelByte )
				{
				case 2 : *( ( WORD  * )DestAddr ) = *( ( WORD  * )( DestAddr - TempBufferPitch ) - 1 ) ; break ;
				case 4 : *( ( DWORD * )DestAddr ) = *( ( DWORD * )( DestAddr - TempBufferPitch ) - 1 ) ; break ;
				}
			}

			// テクスチャへ転送
			glTexSubImage2D_ASync(
				UseTex,
				GL_TEXTURE_2D,
				0,
				DestRect->left,
				DestRect->top,
				DestRect->right  - DestRect->left + ( RightLineFillFlag ? 1 : 0 ),
				DestRect->bottom - DestRect->top + ( BottomLineFillFlag ? 1 : 0 ),
				PixelFormat->Format,
				PixelFormat->Type,
				TempBuffer,
				ASyncThread
			) ;
		}
	}


	if( ASyncThread )
	{
		if( TempBuffer != NULL )
		{
			DXFREE( TempBuffer ) ;
			TempBuffer = NULL ;
		}
	}

	// 正常終了
	return 0 ;
}

// Linux のグラフィックハンドルに画像データを転送するための関数
extern	int		Graphics_Hardware_BltBmpOrBaseImageToGraph3_PF(
	const RECT		*SrcRect,
	      int		DestX,
	      int		DestY,
	      int		GrHandle,
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		RedIsAlphaFlag,
	      int		UseTransColorConvAlpha,
	      int		TargetOrig,
	      int		ASyncThread
)
{
	IMAGEDATA					*Image ;
	IMAGEDATA_ORIG				*Orig ;
	IMAGEDATA_ORIG_HARD_TEX		*OrigTex ;
	IMAGEDATA_HARD_DRAW			*DrawTex ;
//	int							ImageNum ;
	int							i ;
//	POINT						DestPoint ;
	RECT						DestRect ;
	RECT						MoveRect ;
	int							IsDXT ;
	GLuint						UseTex ;
	int							SrcWidth ;
	int							SrcHeight ;

	// 描画待機している描画物を描画
	DRAWSTOCKINFO_ASYNC( ASyncThread ) ;

	// DXT圧縮の画像かを調べておく
	IsDXT =
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT1 ||
		  RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT2 ||
		  RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT3 ||
		  RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT4 ||
		  RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT5 ||
		  RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_BC7_UNORM ||
		  RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_BC7_UNORM_SRGB ) ? TRUE : FALSE ;

	// 転送幅を計算しておく
	SrcWidth  = SrcRect->right  - SrcRect->left ;
	SrcHeight = SrcRect->bottom - SrcRect->top ;
	if( SrcWidth <= 0 || SrcHeight <= 0 )
	{
		return 0 ;
	}

	// エラー判定
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) )
		{
			return -1 ;
		}
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
		{
			return -1 ;
		}
	}
	Orig = Image->Orig ;

	// イメージの数をセット
//	ImageNum = RgbBaseImage->GraphDataCount == 0 ? 1 : RgbBaseImage->GraphDataCount ;

	// 転送位置のセット
//	DestPoint.x = 0 ;
//	DestPoint.y = 0 ;

	// 転送対象がオリジナル画像に対してかどうかで処理を分岐
	if( TargetOrig == TRUE )
	{
		// オリジナル画像に対しての場合

		// テクスチャの数だけ繰り返し
		OrigTex = Orig->Hard.Tex ;
		for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
		{
			// 転送矩形の作成
			if( Graphics_Image_BltBmpOrBaseImageToGraph3_Make_OrigTex_MoveRect(
					OrigTex,
					SrcRect,
					SrcWidth,
					SrcHeight,
					DestX,
					DestY,
					&DestRect,
					&MoveRect,
					IsDXT ) < 0 )
			{
				continue ;
			}

			UseTex = OrigTex->PF->Texture.TextureBuffer ;

			// ミップマップかどうかで処理を分岐
			if( Orig->Hard.MipMapCount > 1 )
			{
				if( Graphics_Linux_BltBmpOrBaseImageToGraph3_MipMapBlt(
						Orig,
						&MoveRect,
						&DestRect,
						UseTex,
						OrigTex->TexWidth,
						OrigTex->TexHeight,
						RgbBaseImage,
						AlphaBaseImage,
						GLINUX.Device.Caps.TextureFormat[ Orig->ColorFormat ],
						RedIsAlphaFlag,
						UseTransColorConvAlpha,
						ASyncThread ) < 0 )
					goto ERR ;
			}
			else
			{
				if( Graphics_Linux_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
						&MoveRect,
						&DestRect,
						UseTex,
						Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1,
						OrigTex->TexWidth,
						OrigTex->TexHeight,
						OrigTex->UseWidth,
						OrigTex->UseHeight,
						RgbBaseImage,
						AlphaBaseImage,
						GLINUX.Device.Caps.TextureFormat[ Orig->ColorFormat ],
						RedIsAlphaFlag,
						UseTransColorConvAlpha,
						Orig->TransCode,
						ASyncThread ) < 0 )
					goto ERR ;
			}
		}
	}
	else
	{
		// オリジナル画像に対してではない場合

		// 描画情報の数だけ繰り返し
		DrawTex = Image->Hard.Draw ;
//		DestPoint.x = 0 ;
//		DestPoint.y = 0 ;
		for( i = 0 ; i < Image->Hard.DrawNum ; i ++, DrawTex ++ )
		{
			// 転送矩形の作成
			if( Graphics_Image_BltBmpOrBaseImageToGraph3_Make_DrawTex_MoveRect(
					DrawTex,
					SrcRect,
					SrcWidth,
					SrcHeight,
					DestX,
					DestY,
					&DestRect,
					&MoveRect,
					IsDXT ) < 0 )
			{
				continue ;
			}

			UseTex = DrawTex->Tex->PF->Texture.TextureBuffer ;

			// ミップマップかどうかで処理を分岐
			if( Orig->Hard.MipMapCount > 1 )
			{
				if( Graphics_Linux_BltBmpOrBaseImageToGraph3_MipMapBlt(
						Orig,
						&MoveRect,
						&DestRect,
						UseTex,
						DrawTex->Tex->TexWidth,
						DrawTex->Tex->TexHeight,
						RgbBaseImage,
						AlphaBaseImage,
						GLINUX.Device.Caps.TextureFormat[ Orig->ColorFormat ],
						RedIsAlphaFlag,
						UseTransColorConvAlpha,
						ASyncThread ) < 0 )
					goto ERR ;
			}
			else
			{
				if( Graphics_Linux_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
						&MoveRect,
						&DestRect,
						UseTex,
						Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1,
						DrawTex->Tex->TexWidth,
						DrawTex->Tex->TexHeight,
						DrawTex->Tex->UseWidth,
						DrawTex->Tex->UseHeight,
						RgbBaseImage,
						AlphaBaseImage,
						GLINUX.Device.Caps.TextureFormat[ Orig->ColorFormat ],
						RedIsAlphaFlag,
						UseTransColorConvAlpha,
						Orig->TransCode,
						ASyncThread ) < 0 )
					goto ERR ;
			}
		}
	}

	// 正常終了
	return 0 ;


	// エラー終了
ERR :
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
static int Graphics_Hardware_CreateOrigTexture_ASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	return Graphics_Hardware_CreateOrigTexture_PF( ( IMAGEDATA_ORIG * )Info->Data[ 0 ], FALSE ) ;
}
#endif // DX_NON_ASYNCLOAD

// オリジナル画像情報中のテクスチャを作成する( 0:成功  -1:失敗 )
extern	int		Graphics_Hardware_CreateOrigTexture_PF(  IMAGEDATA_ORIG *Orig, int ASyncThread )
{
	int							i, j ;
	IMAGEDATA_ORIG_HARD_TEX		*OrigTex ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = Graphics_Hardware_CreateOrigTexture_ASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )Orig ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// ハードウエアを使用する画像ではない場合は何もせずに終了
	if( Orig->FormatDesc.TextureFlag == FALSE )
	{
		return 0 ;
	}

	// テクスチャの場合はテクスチャの数だけテクスチャを作成
	OrigTex = Orig->Hard.Tex ;
	for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
	{
		// テクスチャを作成
		if( OrigTex->PF->Texture.TextureBuffer == 0 )
		{
			PIXELFORMAT_INFO_LINUX *PixelFormat ;

			OrigTex->PF->Texture.PixelFormat = GLINUX.Device.Caps.TextureFormat[ Orig->ColorFormat ] ;
			PixelFormat = &g_LinuxPixelFormat[ OrigTex->PF->Texture.PixelFormat ] ;
			glGenTextures( 1, &OrigTex->PF->Texture.TextureBuffer ) ;
			glBindTexture( GL_TEXTURE_2D, OrigTex->PF->Texture.TextureBuffer ) ;

			OrigTex->PF->Texture.Width  = OrigTex->TexWidth ;
			OrigTex->PF->Texture.Height = OrigTex->TexHeight ;

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) ;
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) ;
			OrigTex->PF->Texture.MagFilter = GL_NEAREST ;
			OrigTex->PF->Texture.MinFilter = GL_NEAREST ;

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;
			OrigTex->PF->Texture.WrapS = GL_CLAMP_TO_EDGE ;
			OrigTex->PF->Texture.WrapT = GL_CLAMP_TO_EDGE ;
			OrigTex->PF->Texture.WrapR = GL_CLAMP_TO_EDGE ;

			OrigTex->PF->Texture.MipMapCount = Orig->Hard.MipMapCount <= 1 ? 1 : Orig->Hard.MipMapCount ;
			for( j = 0 ; j < OrigTex->PF->Texture.MipMapCount ; j ++ )
			{
				int TexWidth ;
				int TexHeight ;

				TexWidth  = OrigTex->TexWidth  >> j ;
				if( TexWidth <= 0 )
				{
					TexWidth = 1 ;
				}

				TexHeight = OrigTex->TexHeight >> j ;
				if( TexHeight <= 0 )
				{
					TexHeight = 1 ;
				}

				if( PixelFormat->IsExtFormat )
				{
					glCompressedTexImage2D(
						GL_TEXTURE_2D,
						j,
						PixelFormat->InternalFormat,
						TexWidth,
						TexHeight,
						0,
						0,
						0
					) ;
				}
				else
				{
					glTexImage2D(
						GL_TEXTURE_2D,
						j,
						PixelFormat->InternalFormat,
						TexWidth,
						TexHeight,
						0,
						PixelFormat->Format,
						PixelFormat->Type,
						0
					) ;
				}
			}
		}

		// 描画対象にできる場合はフレームバッファを作成
		if( Orig->FormatDesc.DrawValidFlag )
		{
			// フレームバッファを作成
			if( OrigTex->PF->FrameBuffer == 0 )
			{
				glGenFramebuffers( 1, &OrigTex->PF->FrameBuffer ) ;
				glBindFramebuffer( GL_FRAMEBUFFER, OrigTex->PF->FrameBuffer ) ;

				// 深度バッファを作成
				glGenRenderbuffers( 1, &OrigTex->PF->DepthBuffer ) ;
				glBindRenderbuffer( GL_RENDERBUFFER, OrigTex->PF->DepthBuffer ) ;
				glRenderbufferStorage( GL_RENDERBUFFER, GLINUX.Device.Caps.DepthBufferFormat[ Orig->ZBufferBitDepthIndex ], OrigTex->TexWidth, OrigTex->TexHeight ) ;

				// 深度バッファとテクスチャをフレームバッファにアタッチ
				glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, OrigTex->PF->DepthBuffer ) ;
				glFramebufferTexture2D(    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,   OrigTex->PF->Texture.TextureBuffer, 0 ) ;

				// レンダリングターゲットを元に戻す
				glBindFramebuffer( GL_FRAMEBUFFER, GLINUX.Device.State.TargetFrameBuffer ) ;
			}
		}
	}

	// 終了
	return 0 ;
}

// テクスチャを解放する
static int Graphics_Hardware_Linux_DeleteTexture( GLuint Texture )
{
	int i ;
	int Flag ;

	// シェーダーで使用するテクスチャとして設定されていたら解除する
	Flag = FALSE ;
	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
	{
		if( GLINUX.Device.State.SetTexture[ i ] == NULL ||
			GLINUX.Device.State.SetTexture[ i ]->TextureBuffer != Texture )
		{
			continue ;
		}

		if( Flag == FALSE )
		{
			// 描画待機している描画物を描画
			DRAWSTOCKINFO
			Flag = TRUE ;
		}

		Graphics_Linux_DeviceState_SetTexture( i, NULL ) ;
	}

	// どこかのスロットにセットされていたら描画用テクスチャも解除
	if( Flag )
	{
		Graphics_Linux_DrawSetting_SetTexture( NULL ) ;
	}

	// テクスチャを削除する
	glDeleteTextures( 1, &Texture ) ;

	return 0 ;
}

// オリジナル画像情報中のテクスチャを解放する
extern int Graphics_Hardware_ReleaseOrigTexture_PF( IMAGEDATA_ORIG *Orig )
{
	int i ;
	IMAGEDATA_ORIG_HARD_TEX *OrigTex ;

	// ハードウエアを使用する画像ではない場合は何もせずに終了
	if( Orig->FormatDesc.TextureFlag == FALSE )
	{
		return 0 ;
	}

	OrigTex = Orig->Hard.Tex ;
	for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
	{
		if( OrigTex->PF->FrameBuffer != 0 )
		{
			glDeleteFramebuffers( 1, &OrigTex->PF->FrameBuffer ) ;
			OrigTex->PF->FrameBuffer = 0 ;
		}

		if( OrigTex->PF->Texture.TextureBuffer != 0 )
		{
			Graphics_Hardware_Linux_DeleteTexture( OrigTex->PF->Texture.TextureBuffer ) ;
			OrigTex->PF->Texture.TextureBuffer = 0 ;
		}

		if( OrigTex->PF->DepthBuffer != 0 )
		{
			glDeleteRenderbuffers( 1, &OrigTex->PF->DepthBuffer ) ;
			OrigTex->PF->DepthBuffer = 0 ;
		}
	}

	// 終了
	return 0 ;
}

// 指定のマルチサンプル数で使用できる最大クオリティ値を取得する
extern	int		Graphics_Hardware_GetMultiSampleQuality_PF( int Samples )
{
	return 0 ;
}

// 作成するグラフィックハンドルで使用する環境依存のテクスチャフォーマットを指定する
extern int Graphics_Hardware_SetUsePlatformTextureFormat_PF( int PlatformTextureFormat )
{
	return 0 ;
}




























// 環境依存頂点バッファ・インデックスバッファ関係

// 頂点バッファハンドルの頂点バッファを作成する
extern	int		Graphics_Hardware_VertexBuffer_Create_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	glGenBuffers( 1, &VertexBuffer->PF->VertexBuffer ) ;
	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer->PF->VertexBuffer ) ;
	glBufferData( GL_ARRAY_BUFFER, VertexBuffer->UnitSize * VertexBuffer->Num, NULL, GL_DYNAMIC_DRAW ) ;
	glBindBuffer( GL_ARRAY_BUFFER, 0 ) ;

	return 0 ;
}

// 頂点バッファハンドルの後始末
extern	int		Graphics_Hardware_VertexBuffer_Terminate_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	glDeleteBuffers( 1, &VertexBuffer->PF->VertexBuffer ) ;

	return 0 ;
}

// 頂点バッファに頂点データを転送する
extern	int		Graphics_Hardware_VertexBuffer_SetData_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer, int SetIndex, const void *VertexData, int VertexNum )
{
	glBindBuffer(    GL_ARRAY_BUFFER, VertexBuffer->PF->VertexBuffer ) ;
	glBufferSubData( GL_ARRAY_BUFFER, SetIndex * VertexBuffer->UnitSize, VertexNum * VertexBuffer->UnitSize, VertexData ) ;
	glBindBuffer(    GL_ARRAY_BUFFER, 0 ) ;

	// 正常終了
	return 0 ;
}

// インデックスバッファハンドルのセットアップを行う
extern	int		Graphics_Hardware_IndexBuffer_Create_PF( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	glGenBuffers( 1, &IndexBuffer->PF->IndexBuffer ) ;
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer->PF->IndexBuffer ) ;
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer->UnitSize * IndexBuffer->Num, NULL, GL_DYNAMIC_DRAW ) ;
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) ;

	return 0 ;
}

// インデックスバッファハンドルの後始末
extern	int		Graphics_Hardware_IndexBuffer_Terminate_PF( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	glDeleteBuffers( 1, &IndexBuffer->PF->IndexBuffer ) ;

	return 0 ;
}

// インデックスバッファにインデックスデータを転送する
extern	int		Graphics_Hardware_IndexBuffer_SetData_PF( INDEXBUFFERHANDLEDATA *IndexBuffer, int SetIndex, const void *IndexData, int IndexNum )
{
	glBindBuffer(    GL_ELEMENT_ARRAY_BUFFER, IndexBuffer->PF->IndexBuffer ) ;
	glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, SetIndex * IndexBuffer->UnitSize, IndexNum * IndexBuffer->UnitSize, IndexData ) ;
	glBindBuffer(    GL_ELEMENT_ARRAY_BUFFER, 0 ) ;

	// 正常終了
	return 0 ;
}























// 環境依存ライト関係

// ライティングを行うかどうかを設定する
extern	int		Graphics_Hardware_Light_SetUse_PF( int Flag )
{
	return Graphics_Linux_DeviceState_SetLighting( Flag ) ;
}

// グローバルアンビエントライトカラーを設定する
extern	int		Graphics_Hardware_Light_GlobalAmbient_PF( COLOR_F *Color )
{
	return Graphics_Linux_DeviceState_SetAmbient( Color ) ;
}

// ライトパラメータをセット
extern	int		Graphics_Hardware_Light_SetState_PF( int LightNumber, LIGHTPARAM *LightParam )
{
	return Graphics_Linux_DeviceState_SetLightState( LightNumber, LightParam ) ;
}

// ライトの有効、無効を変更
extern	int		Graphics_Hardware_Light_SetEnable_PF( int LightNumber, int EnableState )
{
	return Graphics_Linux_DeviceState_SetLightEnable( LightNumber, EnableState ) ;
}

// ライトの計算で角度減衰を行わないようにするかどうかを設定する
extern	int		Graphics_Hardware_Light_SetNoAngleAttenuation_PF( int NoAngleAttenuation )
{
	// 未実装
	return 0 ;
}

// ライトの計算でハーフランバートを使用するかどうかを設定する
extern	int		Graphics_Hardware_Light_SetUseHalfLambert_PF( int UseHalfLambert )
{
	// 未実装
	return 0 ;
}


























// 環境依存シャドウマップ関係

// シャドウマップデータに必要なテクスチャを作成する
extern	int		Graphics_Hardware_ShadowMap_CreateTexture_PF( SHADOWMAPDATA *ShadowMap, int ASyncThread )
{
	if( LINUX_CHECKVALID_HARDWARE == FALSE )
	{
		return -1 ;
	}

	// フォーマットの決定
	if( ShadowMap->TexFormat_Float )
	{
		if( ShadowMap->TexFormat_BitDepth == 16 )
		{
			ShadowMap->ColorFormat = DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ;
			ShadowMap->ZBufferFormat = ZBUFFER_FORMAT_16BIT ;
		}
		else
		{
			ShadowMap->ColorFormat = DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ;
			ShadowMap->ZBufferFormat = ZBUFFER_FORMAT_32BIT ;
		}
	}
	else
	{
		if( ShadowMap->TexFormat_BitDepth == 16 )
		{
			ShadowMap->ColorFormat = DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ;
			ShadowMap->ZBufferFormat = ZBUFFER_FORMAT_16BIT ;
		}
		else
		{
			ShadowMap->ColorFormat = DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ;
			ShadowMap->ZBufferFormat = ZBUFFER_FORMAT_32BIT ;
		}
	}

//	// 決定したフォーマットが作成できない場合は失敗
//	if( GLINUX.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ] == D_DXGI_FORMAT_UNKNOWN )
//	{
//		DXST_LOGFILE_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\xc7\x30\xfc\x30\xbf\x30\x28\x75\x6e\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x67\x30\x5c\x4f\x10\x62\x67\x30\x4d\x30\x8b\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"シャドウマップデータ用のフォーマットで作成できるテクスチャフォーマットがありませんでした\n" @*/ ) ;
//		return -1 ;
//	}

	// テクスチャを作成
	{
		PIXELFORMAT_INFO_LINUX *PixelFormat ;

		ShadowMap->PF->Texture.PixelFormat = GLINUX.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ] ;
		PixelFormat = &g_LinuxPixelFormat[ ShadowMap->PF->Texture.PixelFormat ] ;
		if( ShadowMap->PF->Texture.TextureBuffer == 0 )
		{
			glGenTextures( 1, &ShadowMap->PF->Texture.TextureBuffer ) ;
		}
		glBindTexture( GL_TEXTURE_2D, ShadowMap->PF->Texture.TextureBuffer ) ;

		ShadowMap->PF->Texture.Width  = ShadowMap->BaseSizeX ;
		ShadowMap->PF->Texture.Height = ShadowMap->BaseSizeY ;

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) ;
		ShadowMap->PF->Texture.MagFilter = GL_NEAREST ;
		ShadowMap->PF->Texture.MinFilter = GL_NEAREST ;

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;
		ShadowMap->PF->Texture.WrapS = GL_CLAMP_TO_EDGE ;
		ShadowMap->PF->Texture.WrapT = GL_CLAMP_TO_EDGE ;
		ShadowMap->PF->Texture.WrapR = GL_CLAMP_TO_EDGE ;

		if( PixelFormat->IsExtFormat )
		{
			glCompressedTexImage2D(
				GL_TEXTURE_2D,
				0,
				PixelFormat->InternalFormat,
				ShadowMap->BaseSizeX,
				ShadowMap->BaseSizeY,
				0,
				0,
				0
			) ;
		}
		else
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				PixelFormat->InternalFormat,
				ShadowMap->BaseSizeX,
				ShadowMap->BaseSizeY,
				0,
				PixelFormat->Format,
				PixelFormat->Type,
				0
			) ;
		}
	}

	// フレームバッファを作成
	if( ShadowMap->PF->FrameBuffer == 0 )
	{
		glGenFramebuffers( 1, &ShadowMap->PF->FrameBuffer ) ;
	}
	glBindFramebuffer( GL_FRAMEBUFFER, ShadowMap->PF->FrameBuffer ) ;

	// 深度バッファを作成
	if( ShadowMap->PF->DepthBuffer == 0 )
	{
		glGenRenderbuffers( 1, &ShadowMap->PF->DepthBuffer ) ;
	}
	glBindRenderbuffer( GL_RENDERBUFFER, ShadowMap->PF->DepthBuffer ) ;
	glRenderbufferStorage( GL_RENDERBUFFER, GLINUX.Device.Caps.DepthBufferFormat[ ShadowMap->ZBufferFormat ], ShadowMap->BaseSizeX, ShadowMap->BaseSizeY ) ;

	// 深度バッファとテクスチャをフレームバッファにアタッチ
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, ShadowMap->PF->DepthBuffer ) ;
	glFramebufferTexture2D(    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,   ShadowMap->PF->Texture.TextureBuffer, 0 ) ;

	// 終了
	return 0 ;
}

// シャドウマップデータに必要なテクスチャを解放する
extern	int		Graphics_Hardware_ShadowMap_ReleaseTexture_PF( SHADOWMAPDATA *ShadowMap )
{
	if( ShadowMap->PF->FrameBuffer != 0 )
	{
		glDeleteFramebuffers( 1, &ShadowMap->PF->FrameBuffer ) ;
		ShadowMap->PF->FrameBuffer = 0 ;
	}

	if( ShadowMap->PF->Texture.TextureBuffer != 0 )
	{
		Graphics_Hardware_Linux_DeleteTexture( ShadowMap->PF->Texture.TextureBuffer ) ;
		ShadowMap->PF->Texture.TextureBuffer = 0 ;
	}

	if( ShadowMap->PF->DepthBuffer != 0 )
	{
		glDeleteRenderbuffers( 1, &ShadowMap->PF->DepthBuffer ) ;
		ShadowMap->PF->DepthBuffer = 0 ;
	}

	// 終了
	return 0 ;
}

// 頂点シェーダーに設定するシャドウマップの情報を更新する
extern	void	Graphics_Hardware_ShadowMap_RefreshVSParam_PF( void )
{
	SHADOWMAPDATA *ShadowMap ;
	static float TranspMatDef[ 4 ][ 4 ] =
	{
		{ 0.0f, 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 0.0f, -1.0f },
	} ;
	int i ;

	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		// アドレスの取得
		if( SHADOWMAPCHKFULL( GSYS.DrawSetting.ShadowMap[ i ], ShadowMap ) )
		{
			_MEMCPY( GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 ], TranspMatDef, sizeof( TranspMatDef ) ) ;
		}
		else
		{
			// 行列のセット
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 0 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 0 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 0 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 0 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 0 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 0 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 0 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 0 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 1 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 1 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 1 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 1 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 1 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 1 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 1 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 1 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 2 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 2 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 2 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 2 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 2 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 2 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 2 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 2 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 3 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 3 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 3 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 3 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 3 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 3 ] ;
			GLINUX.Device.Shader.Constant.uSMLVPMat[ i * 4 + 3 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 3 ] ;
		}
	}
}

// フラグメントシェーダーに設定するシャドウマップの情報を更新する
extern	void	Graphics_Hardware_ShadowMap_RefreshPSParam_PF( void )
{
	SHADOWMAPDATA *ShadowMap ;
	LIGHT_HANDLE  *pLH ;
	int           CheckNum ;
	int           i ;
	int           j ;

	CheckNum = GSYS.Light.EnableNum > 3 ? 3 : GSYS.Light.EnableNum ;

	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		GLINUX.Device.Shader.Constant.uShadowMapLightEnable[ i ][ 0 ] = 1.0f ;
		GLINUX.Device.Shader.Constant.uShadowMapLightEnable[ i ][ 1 ] = 1.0f ;
		GLINUX.Device.Shader.Constant.uShadowMapLightEnable[ i ][ 2 ] = 1.0f ;

		// アドレスの取得
		if( SHADOWMAPCHKFULL( GSYS.DrawSetting.ShadowMap[ i ], ShadowMap ) )
		{
			GLINUX.Device.Shader.Constant.uShadowMap_DAdj_Grad[ i ][ 0 ] = 1.0f ;
			GLINUX.Device.Shader.Constant.uShadowMap_DAdj_Grad[ i ][ 1 ] = 0.0f ;
		}
		else
		{
			GLINUX.Device.Shader.Constant.uShadowMap_DAdj_Grad[ i ][ 0 ] = ShadowMap->AdjustDepth ;
			GLINUX.Device.Shader.Constant.uShadowMap_DAdj_Grad[ i ][ 1 ] = ShadowMap->GradationParam ;

			for( j = 0 ; j < CheckNum ; j ++ )
			{
				pLH = GSYS.Light.Data[ GSYS.Light.EnableHardwareIndex[ j ] ] ;
				if( pLH->ShadowMapSlotDisableFlag[ i ] == FALSE )
				{
					switch( j )
					{
					case 0 :
						GLINUX.Device.Shader.Constant.uShadowMapLightEnable[ i ][ 0 ] = 0.0f ;
						break ;

					case 1 :
						GLINUX.Device.Shader.Constant.uShadowMapLightEnable[ i ][ 1 ] = 0.0f ;
						break ;

					case 2 :
						GLINUX.Device.Shader.Constant.uShadowMapLightEnable[ i ][ 2 ] = 0.0f ;
						break ;
					}
				}
			}
		}
	}
}

// シャドウマップへの描画の準備を行う
extern	int		Graphics_Hardware_ShadowMap_DrawSetup_PF( SHADOWMAPDATA *ShadowMap )
{
	// 正常終了
	return 0 ;
}

// シャドウマップへの描画を終了する
extern	int		Graphics_Hardware_ShadowMap_DrawEnd_PF( SHADOWMAPDATA *ShadowMap )
{
	// 正常終了
	return 0 ;
}

// 描画で使用するシャドウマップを指定する( スロットは０か１かを指定可能 )
extern	int		Graphics_Hardware_ShadowMap_SetUse_PF( int SlotIndex, SHADOWMAPDATA *ShadowMap )
{
	// 正常終了
	return 0 ;
}































// 環境依存シェーダー関係

// シェーダーハンドルを初期化する
extern	int		Graphics_Hardware_Shader_Create_PF(
	int ShaderHandle,
	int ShaderType,
	void *Image,
	int ImageSize,
	int ImageAfterFree,
	int ASyncThread
)
{
	SHADERHANDLEDATA   *pShader ;
	GLuint FragmentShader = 0 ;
	GLuint VertexShader = 0 ;
	GRAPHICS_HARDWARE_LINUX_SHADERCODE_INFO ShaderCodeInfo ;
	int Result ;

	// エラー判定
	if( ASyncThread )
	{
		if( SHADERCHK_ASYNC( ShaderHandle, pShader ) )
			return -1 ;
	}
	else
	{
		if( SHADERCHK( ShaderHandle, pShader ) )
			return -1 ;
	}

	// シェーダーを作成する
	ShaderCodeInfo.Binary = Image ;
	ShaderCodeInfo.Size   = ImageSize ;
	switch( ShaderType )
	{
	case DX_SHADERTYPE_VERTEX :
		Result = Graphics_Linux_VertexShader_Create( &ShaderCodeInfo, &VertexShader, 1 ) ;
		if( Result < 0 )
		{
			DXST_LOGFILE_ADDUTF16LE( "\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"頂点シェーダーの作成に失敗しました\n" @*/ ) ;
			goto ERR ;
		}
		break ;

	case DX_SHADERTYPE_PIXEL :
		Result = Graphics_Linux_FragmentShader_Create( &ShaderCodeInfo, &FragmentShader, 1 ) ;
		if( Result < 0 )
		{
			DXST_LOGFILE_ADDUTF16LE( "\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"フラグメントシェーダーの作成に失敗しました\n" @*/ ) ;
			goto ERR ;
		}
		break ;

	default :
		DXST_LOGFILE_ADDUTF16LE( "\xfe\x5b\xdc\x5f\x57\x30\x66\x30\x44\x30\x6a\x30\x44\x30\xbf\x30\xa4\x30\xd7\x30\x6e\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x92\x30\x5c\x4f\x10\x62\x57\x30\x88\x30\x46\x30\x68\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"対応していないタイプのシェーダーを作成しようとしました\n" @*/ ) ;
		goto ERR ;
	}

	// バイナリイメージを保存するメモリ領域の確保
	pShader->FunctionCode = DXCALLOC( ( size_t )ImageSize ) ;
	if( pShader->FunctionCode == NULL )
	{
		DXST_LOGFILE_ADDUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"メモリの確保に失敗しました" @*/ ) ;
		goto ERR ;
	}

	// バイナリイメージを保存する
	_MEMCPY( pShader->FunctionCode, Image, ( size_t )ImageSize ) ;

	// バイナリイメージのサイズを保存する
	pShader->FunctionCodeSize = ImageSize ;

	// シェーダータイプを保存する
	pShader->ShaderType = ShaderType ;

	// シェーダーのアドレスを保存
	if( ShaderType == DX_SHADERTYPE_VERTEX )
	{
		pShader->PF->Shader = VertexShader ;
	}
	else
	{
		pShader->PF->Shader = FragmentShader ;
	}

	// 処理後に解放すべしのフラグが立っていたら解放する
	if( ImageAfterFree )
	{
		DXFREE( Image ) ;
	}

	// 正常終了
	return 0 ;

ERR :

	Graphics_Linux_VertexShaderArray_Release( &VertexShader, 1 ) ;
	Graphics_Linux_FragmentShaderArray_Release(  &FragmentShader,  1 ) ;

	// 処理後に解放すべしのフラグが立っていたら解放する
	if( ImageAfterFree )
	{
		DXFREE( Image ) ;
	}

	return -1 ;
}

// シェーダーハンドルの後始末
extern	int		Graphics_Hardware_Shader_TerminateHandle_PF( SHADERHANDLEDATA *Shader )
{
	// シェーダーの解放
	if( Shader->ShaderType == DX_SHADERTYPE_VERTEX )
	{
		Graphics_Linux_VertexShaderArray_Release( &Shader->PF->Shader, 1 ) ;
	}
	else
	{
		Graphics_Linux_FragmentShaderArray_Release(  &Shader->PF->Shader,  1 ) ;
	}

	// 正常終了
	return 0 ;
}

// 使用できるシェーダーのバージョンを取得する( 0=使えない  200=シェーダーモデル２．０が使用可能  300=シェーダーモデル３．０が使用可能 )
extern	int		Graphics_Hardware_Shader_GetValidShaderVersion_PF( void )
{
	return 500 ;
}

// 指定の名前を持つ定数が使用するシェーダー定数の番号を取得する
extern	int		Graphics_Hardware_Shader_GetConstIndex_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	return 0 ;
}

// 指定の名前を持つ定数が使用するシェーダー定数の数を取得する
extern	int		Graphics_Hardware_Shader_GetConstCount_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	return 0 ;
}

// 指定の名前を持つ浮動小数点定数のデフォルトパラメータが格納されているメモリアドレスを取得する
extern	const FLOAT4 *Graphics_Hardware_Shader_GetConstDefaultParamF_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	return 0 ;
}

// シェーダー定数情報を設定する
extern	int		Graphics_Hardware_Shader_SetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, const void *Param, int ParamNum, int UpdateUseArea )
{
	// 正常終了
	return 0 ;
}

// 指定領域のシェーダー定数情報をリセットする
extern	int		Graphics_Hardware_Shader_ResetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, int ParamNum )
{
	// 正常終了
	return 0 ;
}

#ifndef DX_NON_MODEL
// ３Ｄモデル用のシェーダーコードの初期化を行う
extern	int		Graphics_Hardware_Shader_ModelCode_Init_PF( void )
{
	if( Graphics_Linux_ShaderCode_Model_Initialize() == FALSE )
	{
		return -1 ;
	}

	// 正常終了
	return 0 ;
}
#endif // DX_NON_MODEL
































// 環境依存シェーダー用定数バッファ関係

// シェーダー用定数バッファハンドルを初期化する
extern	int		Graphics_Hardware_ShaderConstantBuffer_Create_PF( int ShaderConstantBufferHandle, int BufferSize, int ASyncThread ) 
{
	return -1 ;
}

// シェーダー用定数バッファハンドルの後始末
extern	int		Graphics_Hardware_ShaderConstantBuffer_TerminateHandle_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	// 正常終了
	return 0 ;
}

// シェーダー用定数バッファハンドルの定数バッファのアドレスを取得する
extern	void *	Graphics_Hardware_ShaderConstantBuffer_GetBuffer_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	return 0 ;
}

// シェーダー用定数バッファハンドルの定数バッファへの変更を適用する
extern	int		Graphics_Hardware_ShaderConstantBuffer_Update_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	// 終了
	return 0 ;
}

// シェーダー用定数バッファハンドルの定数バッファを指定のシェーダーの指定のスロットにセットする
extern	int		Graphics_Hardware_ShaderConstantBuffer_Set_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer, int TargetShader /* DX_SHADERTYPE_VERTEX など */, int Slot )
{
	// 終了
	return 0 ;
}





































// 環境依存描画関係

// 指定点から境界色があるところまで塗りつぶす
extern	int		Graphics_Hardware_Paint_PF( int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor )
{
	// デフォルトの処理を行う
	return 2 ;
}











// OpenGL 関係の情報を取得する
extern int GetOpenGLInfo(
	TCHAR **Version,
	TCHAR **Renderer,
	TCHAR **Vendor,
	TCHAR ***ExtensionNames,
	TCHAR **Shading_Language_Version,
	int *Shader_Max_Vertex_Attribs,						// GL_MAX_VERTEX_ATTRIBS
	int *Shader_Max_Vertex_Uniform_Vectors,				// GL_MAX_VERTEX_UNIFORM_VECTORS
	int *Shader_Max_Varying_Vectors,					// GL_MAX_VARYING_VECTORS
	int *Shader_Max_Combined_Texture_Image_Units,		// GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
	int *Shader_Max_Vertex_Texture_Image_Units,			// GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
	int *Shader_Max_Texture_Image_Units,				// GL_MAX_TEXTURE_IMAGE_UNITS
	int *Shader_Max_Fragment_Uniform_Vectors			// GL_MAX_FRAGMENT_UNIFORM_VECTORS
)
{
	if( Version										!= NULL ) *Version									= GLINUX.Device.Caps.Version	;
	if( Renderer									!= NULL ) *Renderer									= GLINUX.Device.Caps.Renderer ;
	if( Vendor										!= NULL ) *Vendor									= GLINUX.Device.Caps.Vendor ;
	if( ExtensionNames								!= NULL ) *ExtensionNames							= GLINUX.Device.Caps.ExtensionNames ;
	if( Shading_Language_Version					!= NULL ) *Shading_Language_Version					= GLINUX.Device.Caps.Shading_Language_Version ;

	if( Shader_Max_Vertex_Attribs					!= NULL ) *Shader_Max_Vertex_Attribs				= GLINUX.Device.Caps.Shader_Max_Vertex_Attribs ;
	if( Shader_Max_Vertex_Uniform_Vectors			!= NULL ) *Shader_Max_Vertex_Uniform_Vectors		= GLINUX.Device.Caps.Shader_Max_Vertex_Uniform_Vectors ;
	if( Shader_Max_Varying_Vectors					!= NULL ) *Shader_Max_Varying_Vectors				= GLINUX.Device.Caps.Shader_Max_Varying_Vectors ;
	if( Shader_Max_Combined_Texture_Image_Units		!= NULL ) *Shader_Max_Combined_Texture_Image_Units	= GLINUX.Device.Caps.Shader_Max_Combined_Texture_Image_Units ;
	if( Shader_Max_Vertex_Texture_Image_Units		!= NULL ) *Shader_Max_Vertex_Texture_Image_Units	= GLINUX.Device.Caps.Shader_Max_Vertex_Texture_Image_Units ;
	if( Shader_Max_Texture_Image_Units				!= NULL ) *Shader_Max_Texture_Image_Units			= GLINUX.Device.Caps.Shader_Max_Texture_Image_Units ;
	if( Shader_Max_Fragment_Uniform_Vectors			!= NULL ) *Shader_Max_Fragment_Uniform_Vectors		= GLINUX.Device.Caps.Shader_Max_Fragment_Uniform_Vectors ;

	// 終了
	return 0 ;
}

// グラフィックハンドルのテクスチャオブジェクトを取得する
extern unsigned int GetGraphOpenGLESTextureObject( int GrHandle )
{
	IMAGEDATA *Image ;
//	int Ret ;

	// 初期化判定
	if( DxSysData.DxLib_InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// ソフトが非アクティブの場合はアクティブになるまで待つ
	CheckActiveState() ;

	// エラー判定
	if( GRAPHCHK( GrHandle, Image ) )
	{
		return ( GLuint )-1 ;
	}

	return Image->Hard.Draw[ 0 ].Tex->PF->Texture.TextureBuffer ;
}

// サブバックバッファテクスチャのテクスチャオブジェクトを取得する
extern unsigned int GetSubBackbufferOpenGLESTextureObject( void )
{
	return GLINUX.Device.Screen.SubBackBufferTexture ;
}

// ＤＸライブラリが行った OpenGL ES の設定を再度行う( 特殊用途 )
extern int RefreshDxLibOpenGLESSetting( void )
{
	// 今までの設定を復帰する
	Graphics_Linux_DeviceState_RefreshRenderState() ;

	return 0 ;
}











#ifndef DX_NON_ASYNCLOAD
static int glTexSubImage2D_ASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	return glTexSubImage2D_ASync(
		( GLuint )Info->Data[ 0 ],
		( GLenum )Info->Data[ 1 ],
		( GLint )Info->Data[ 2 ],
		( GLint )Info->Data[ 3 ], ( GLint )Info->Data[ 4 ],
		( GLsizei )Info->Data[ 5 ], ( GLsizei )Info->Data[ 6 ],
		( GLenum )Info->Data[ 7 ], ( GLenum )Info->Data[ 8 ],
		( const GLvoid* )Info->Data[ 9 ],
		FALSE ) ;
}
#endif // DX_NON_ASYNCLOAD

// glTexSubImage2D の非同期実行用の関数
static int glTexSubImage2D_ASync(
	GLuint texture,
	GLenum target,
	GLint level,
	GLint xoffset, GLint yoffset,
	GLsizei width, GLsizei height,
	GLenum format, GLenum type,
	const GLvoid* pixels,
	int ASyncThread
)
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = glTexSubImage2D_ASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )texture ;
		AInfo.Data[ 1 ] = ( DWORD_PTR )target ;
		AInfo.Data[ 2 ] = ( DWORD_PTR )level ;
		AInfo.Data[ 3 ] = ( DWORD_PTR )xoffset ;
		AInfo.Data[ 4 ] = ( DWORD_PTR )yoffset ;
		AInfo.Data[ 5 ] = ( DWORD_PTR )width ;
		AInfo.Data[ 6 ] = ( DWORD_PTR )height ;
		AInfo.Data[ 7 ] = ( DWORD_PTR )format ;
		AInfo.Data[ 8 ] = ( DWORD_PTR )type ;
		AInfo.Data[ 9 ] = ( DWORD_PTR )pixels ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	glBindTexture( target, texture ) ;
	glTexSubImage2D(
		target,
		level,
		xoffset, yoffset,
		width, height,
		format, type,
		pixels
	) ;
	glBindTexture( target, 0 ) ;

	return 0 ;
}





#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_GRAPHICS



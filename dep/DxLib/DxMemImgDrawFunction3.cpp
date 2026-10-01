// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		メモリイメージ制御用プログラム
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

// インクルード----------------------------------------------------------------
#include "DxMemImg.h"
#include "DxMath.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxBaseFunc.h"
#include "DxMemory.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// データ型定義----------------------------------------------------------------

// ペイント用データ
struct PAINTDATA
{
	short					x, y ;								// 要素点
} ;

// データ宣言------------------------------------------------------------------

// 関数プロトタイプ宣言--------------------------------------------------------

// プログラム------------------------------------------------------------------

#ifdef DX_NON_2DDRAW

extern void DrawFillBoxMemImg( MEMIMG *DestImg, const RECT *FillRect, unsigned int Color )
{
	return ;
}

extern void DrawLineMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	return ;
}

extern void DrawLineBoxMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	return ;
}

extern void DrawPixelSetMemImg( MEMIMG *DestImg, const POINTDATA *PointData, int Num )
{
	return ;
}

extern void DrawLineSetMemImg( MEMIMG *DestImg, const LINEDATA *LineData, int Num )
{
	return ;
}

extern void DrawBoxSetMemImg( MEMIMG *DestImg, const RECTDATA *RectData, int Num )
{
	return ;
}

extern void DrawPixelMemImg( MEMIMG *DestImg, int x, int y, unsigned int Color )
{
	return ;
}

extern void PaintMemImg( MEMIMG *DestImg, int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor )
{
	return ;
}

extern void DrawCircleMemImg( MEMIMG *DestImg, int x, int y, int r, unsigned int Color, int FillFlag, int Rx_One_Minus, int Ry_One_Minus )
{
	return ;
}

extern void DrawOvalMemImg( MEMIMG *DestImg, int x, int y, int rx, int ry, unsigned int Color, int FillFlag, int Rx_One_Minus, int Ry_One_Minus )
{
	return ;
}

#else

#define PAINTMEMIMG_ND( TYPE, ADDNUM )	\
/* 描画ループに掛ける */\
while( PWriteIndex != PReadIndex )\
{\
	/* スタックに詰まれたデータを読み出す */\
	x2 = PData[PReadIndex].x ;\
	y2 = PData[PReadIndex].y ;\
	PReadIndex ++ ;\
	if( PReadIndex == PAINTBUFFERSIZE ) PReadIndex = 0 ;\
\
	/* サーチアドレスを算出 */\
	SrcBPT  = SrcBP  + ( x2 * (ADDNUM) ) + y2 * SrcPitch  ;\
	DestBPT = DestBP + ( x2 * (ADDNUM) ) + y2 * DestPitch ;\
\
	/* もし既にサーチされ終わっていたら次に移る */\
	if( *((TYPE *)SrcBPT) == (DWORD)BoundaryColorD ) continue ;\
\
	/* 左端まで移動 */\
	while( *((TYPE *)SrcBPT) != (DWORD)BoundaryColorD && x2 > MemImgManage.DrawArea.left )\
	{\
		x2 -- ;\
		SrcBPT  -= (ADDNUM) ;\
		DestBPT -= (ADDNUM) ;\
	}\
	if( *((TYPE *)SrcBPT) == (DWORD)BoundaryColorD )\
	{\
		x2 ++ ;\
		SrcBPT  += (ADDNUM) ;\
		DestBPT += (ADDNUM) ;\
	}\
\
	/* フラグ初期化 */\
	UpSarchFlag = DownSarchFlag = FALSE ;\
\
	/* 右に向かって走査開始 */\
	while( MemImgManage.DrawArea.right > x2 && *((TYPE *)SrcBPT) != (DWORD)BoundaryColorD )\
	{\
		/* 現在の座標を塗る */\
		*((TYPE *)SrcBPT)  = (TYPE)BoundaryColorD ;\
		*((TYPE *)DestBPT) = (TYPE)FillColor ;\
\
		/* 上調べフラグが倒れていて、更に上に境界色がない場合は、上のピクセルを */\
		/* 新たなサーチポイントとしてスタックに積む */\
		if( y2 - 1 >= MemImgManage.DrawArea.top )\
		{\
			if( *((TYPE *)( SrcBPT - SrcPitch )) != (DWORD)BoundaryColorD )\
			{\
				if( UpSarchFlag == FALSE )\
				{\
					PData[PWriteIndex].x = ( short )x2 ;\
					PData[PWriteIndex].y = ( short )( y2 - 1 ) ;\
					UpSarchFlag = TRUE ;\
\
					PWriteIndex ++ ;\
					if( PWriteIndex == PAINTBUFFERSIZE ) PWriteIndex = 0 ;\
				}\
			}\
			else\
			{\
				UpSarchFlag = FALSE ;\
			}\
		}\
\
		/* 下調べフラグが倒れていて、更に下に境界色がない場合は、下のピクセルを */\
		/* 新たなサーチポイントとしてスタックに積む */\
		if( y2 + 1 < MemImgManage.DrawArea.bottom )\
		{\
			if( *((TYPE *)( SrcBPT + SrcPitch )) != (DWORD)BoundaryColorD )\
			{\
				if( DownSarchFlag == FALSE )\
				{\
					PData[PWriteIndex].x = ( short )x2 ;\
					PData[PWriteIndex].y = ( short )( y2 + 1 ) ;\
					DownSarchFlag = TRUE ;\
\
					PWriteIndex ++ ;\
					if( PWriteIndex == PAINTBUFFERSIZE ) PWriteIndex = 0 ;\
				}\
			}\
			else\
			{\
				DownSarchFlag = FALSE ;\
			}\
		}\
\
		/* アドレスインクリメント */\
		SrcBPT  += (ADDNUM) ;\
		DestBPT += (ADDNUM) ;\
		x2 ++ ;\
	}\
}


#define PAINTMEMIMG2_ND( TYPE, ADDNUM )	\
/* 指定点の色を取得する */\
BoundaryColorD = (DWORD)*((TYPE *)(DestBP  + ( x * (ADDNUM) ) + y * DestPitch))  ;\
if( FillColor != BoundaryColorD )\
{\
	/* 描画ループに掛ける */\
	while( PWriteIndex != PReadIndex )\
	{\
		/* スタックに詰まれたデータを読み出す */\
		x2 = PData[PReadIndex].x ;\
		y2 = PData[PReadIndex].y ;\
		PReadIndex ++ ;\
		if( PReadIndex == PAINTBUFFERSIZE ) PReadIndex = 0 ;\
	\
		/* サーチアドレスを算出 */\
		DestBPT = DestBP + ( x2 * (ADDNUM) ) + y2 * DestPitch ;\
	\
		/* もし既にサーチされ終わっていたら次に移る */\
		if( *((TYPE *)DestBPT) != (DWORD)BoundaryColorD ) continue ;\
	\
		/* 左端まで移動 */\
		while( *((TYPE *)DestBPT) == (DWORD)BoundaryColorD && x2 > MemImgManage.DrawArea.left )\
		{\
			x2 -- ;\
			DestBPT -= (ADDNUM) ;\
		}\
		if( *((TYPE *)DestBPT) != (DWORD)BoundaryColorD )\
		{\
			x2 ++ ;\
			DestBPT += (ADDNUM) ;\
		}\
	\
		/* フラグ初期化 */\
		UpSarchFlag = DownSarchFlag = FALSE ;\
	\
		/* 右に向かって走査開始 */\
		while( MemImgManage.DrawArea.right > x2 && *((TYPE *)DestBPT) == (DWORD)BoundaryColorD )\
		{\
			/* 現在の座標を塗る */\
			*((TYPE *)DestBPT) = (TYPE)FillColor ;\
	\
			/* 上調べフラグが倒れていて、更に上に境界色がない場合は、上のピクセルを */\
			/* 新たなサーチポイントとしてスタックに積む */\
			if( y2 - 1 >= MemImgManage.DrawArea.top )\
			{\
				if( *((TYPE *)( DestBPT - DestPitch )) == (DWORD)BoundaryColorD )\
				{\
					if( UpSarchFlag == FALSE )\
					{\
						PData[PWriteIndex].x = ( short )x2 ;\
						PData[PWriteIndex].y = ( short )( y2 - 1 ) ;\
						UpSarchFlag = TRUE ;\
	\
						PWriteIndex ++ ;\
						if( PWriteIndex == PAINTBUFFERSIZE ) PWriteIndex = 0 ;\
					}\
				}\
				else\
				{\
					UpSarchFlag = FALSE ;\
				}\
			}\
	\
			/* 下調べフラグが倒れていて、更に下に境界色がない場合は、下のピクセルを */\
			/* 新たなサーチポイントとしてスタックに積む */\
			if( y2 + 1 < MemImgManage.DrawArea.bottom )\
			{\
				if( *((TYPE *)( DestBPT + DestPitch )) == (DWORD)BoundaryColorD )\
				{\
					if( DownSarchFlag == FALSE )\
					{\
						PData[PWriteIndex].x = ( short )x2 ;\
						PData[PWriteIndex].y = ( short )( y2 + 1 ) ;\
						DownSarchFlag = TRUE ;\
	\
						PWriteIndex ++ ;\
						if( PWriteIndex == PAINTBUFFERSIZE ) PWriteIndex = 0 ;\
					}\
				}\
				else\
				{\
					DownSarchFlag = FALSE ;\
				}\
			}\
	\
			/* アドレスインクリメント */\
			DestBPT += (ADDNUM) ;\
			x2 ++ ;\
		}\
	}\
}

// 指定点から境界色があるところまで塗りつぶす
extern void PaintMemImg( MEMIMG *DestImg, int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor )
{
	int x2, y2 ;
	int DestPitch, SrcPitch ;
	PAINTDATA *PData ;
	int PWriteIndex, PReadIndex ;
	int UpSarchFlag, DownSarchFlag ;
	MEMIMG WorkImg ;
	unsigned int BoundaryColorD = ( DWORD )BoundaryColor ;

	union
	{
		BYTE *SrcBPT ;
		WORD *SrcWPT ;
		DWORD *SrcDPT ;
	} ;
	union
	{
		BYTE *DestBPT ;
		WORD *DestWPT ;
		DWORD *DestDPT ;
	} ;
	union
	{
		BYTE *SrcBP ;
		WORD *SrcWP ;
		DWORD *SrcDP ;
	} ;
	union
	{
		BYTE *DestBP ;
		WORD *DestWP ;
		DWORD *DestDP ;
	} ;
	
	// 描画先がαチャンネル付かパレット付きだった場合はエラー
	if( DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// バッファの確保
	PData = ( PAINTDATA * )DXALLOC( sizeof( PAINTDATA ) * PAINTBUFFERSIZE ) ;
	if( PData == NULL ) return;

	// インデックスカウンタ初期化
	PWriteIndex = PReadIndex = 0 ;

	// 最初の走査データを入れる
	PData[0].x = ( short )x ;
	PData[0].y = ( short )y ;
	PWriteIndex ++ ;

	// 描画
	if( BoundaryColor == -1 )
	{
		// １ラインのピッチ長を保存
		DestPitch = ( int )DestImg->Base->Pitch ;

		// 描画先と描画元の基点データアドレスを取得
		DestBP = DestImg->UseImage ;

		switch( DestImg->Base->ColorType )
		{
		case 0 :	// 16bit
			PAINTMEMIMG2_ND( WORD, 2 ) ;
			break ;
			
		case 1 :	// 32bit
			PAINTMEMIMG2_ND( DWORD, 4 ) ;
			break ;
		}
	}
	else
	{
		// 作業用イメージを作成する
		_MEMSET( &WorkImg, 0, sizeof( WorkImg ) ) ;
		if( InitializeMemImg( &WorkImg, ( int )DestImg->Width, ( int )DestImg->Height, -1, 0, DestImg->Base->ColorType, FALSE, FALSE ) == -1 )
			return ;

		// １ラインのピッチ長を保存
		SrcPitch  = ( int )WorkImg.Base->Pitch ;
		DestPitch = ( int )DestImg->Base->Pitch ;

		// 描画先と描画元の基点データアドレスを取得
		SrcBP  = WorkImg.UseImage ;
		DestBP = DestImg->UseImage ;

		// 作業用イメージ領域に描画先の内容をコピーする
		BltMemImg( &WorkImg, DestImg, NULL, 0, 0 ) ;

		switch( DestImg->Base->ColorType )
		{
		case 0 :	// 16bit
			PAINTMEMIMG_ND( WORD, 2 ) ;
			break ;
			
		case 1 :	// 32bit
			PAINTMEMIMG_ND( DWORD, 4 ) ;
			break ;
		}

		// 作業用イメージを削除
		TerminateMemImg( &WorkImg ) ;
	}

	// メモリの解放
	DXFREE( PData ) ;

	// 終了
	return ;
}

// イメージに点を描画する
#define DST16	*DestWP
#define SRC16	Color
#define DSTP16	DestWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DestBP
extern void DrawPixelMemImg( MEMIMG *DestImg, int x, int y, unsigned int Color )
{
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Data, DataD ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;
	
	// 描画先がαチャンネル付かパレット付きだった場合はエラー
	if( DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// はみ出し判定
	if( (DWORD)( ( x - MemImgManage.DrawArea.left ) | ( ( MemImgManage.DrawArea.right  - 1 ) - x ) | 
		         ( y - MemImgManage.DrawArea.top  ) | ( ( MemImgManage.DrawArea.bottom - 1 ) - y ) ) & 0x80000000 ) return ;
	
	// 描画先アドレスをセット
	DestBP = DestImg->UseImage + x * DestImg->Base->ColorDataP->PixelByte + y * DestImg->Base->Pitch ;
	
	// 描画の準備
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// 点を描画する
	switch( DestImg->Base->ColorType )
	{
	case 0 :	// 16bit モード
		// 描画輝度を反映
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
					GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
					BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// ブレンドモードによって処理を分岐
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// ブレンド無し
NOTEX_NOMALDRAW_C16_NOPAL_BNO:
			CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_ALPHA :		// αブレンド
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C16_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_ADD :		// 加算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_SUB :		// 減算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_MUL :		// 乗算ブレンド
			CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_MULA :	// 乗算+αブレンド
			CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
			CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 )
			break ;
		}
		break ;
		
	case 1 :	// 32bit モード
		// 描画輝度を反映
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
			((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
			((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// ブレンドモードによって処理を分岐
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// ブレンド無し
NOTEX_NOMALDRAW_C32_NOPAL_BNO:
			CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_ALPHA :		// αブレンド
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C32_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_ADD :		// 加算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_SUB :		// 減算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_MUL :		// 乗算ブレンド
			CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_MULA :	// 乗算+αブレンド
			CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
			CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 )
			break ;
		}
		break ;
	}

	// 終了
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32



// イメージに中身のあるボックスを描画する
extern void DrawFillBoxMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	RECT rect ;

	rect.left   = x1 ;
	rect.top    = y1 ;
	rect.right  = x2 ;
	rect.bottom = y2 ;
	DrawFillBoxMemImg( DestImg, &rect, Color ) ;
}


#define DRAWFILLBOXMEMIMG_ND( DRAW, DESTADDNUM )	\
do{\
	wtemp = Width ;\
	do{\
		DRAW\
		DestBP += (DESTADDNUM) ;\
	}while( -- wtemp ) ;\
	DestBP += DestAddPitch ;\
}while( -- Height ) ;


// イメージに中身のあるボックスを描画する
#define DST16	*DestWP
#define SRC16	Color
#define DSTP16	DestWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DestBP
extern void DrawFillBoxMemImg( MEMIMG *DestImg, const RECT *FillRect, unsigned int Color )
{
	RECT DrawRect, TempRect ;
	DWORD Width, Height ;
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Data, DataD ;
	DWORD_PTR DestPP ;
	DWORD DestAddPitch, wtemp ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;

	// 描画先がαチャンネル付かパレット付きだった場合はエラー
	if( DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// 矩形が NULL だったら全体
	if( FillRect == NULL )
	{
		FillRect = &TempRect ;
		TempRect.left   = 0 ;
		TempRect.top    = 0 ;
		TempRect.right  = ( LONG )DestImg->Width ;
		TempRect.bottom = ( LONG )DestImg->Height ;
		DrawRect = TempRect ;
	}
	else
	{
		// 描画矩形のセット
		if( FillRect->left > FillRect->right )
		{
			DrawRect.right = FillRect->left ;
			DrawRect.left  = FillRect->right ;
		}
		else
		{
			DrawRect.left  = FillRect->left ;
			DrawRect.right = FillRect->right ;
		}

		if( FillRect->top > FillRect->bottom )
		{
			DrawRect.bottom = FillRect->top ;
			DrawRect.top    = FillRect->bottom ;
		}
		else
		{
			DrawRect.top    = FillRect->top ;
			DrawRect.bottom = FillRect->bottom ;
		}
	}

	// クリッピング
	RectClipping_Inline( &DrawRect , &MemImgManage.DrawArea ) ;
	Width  = ( DWORD )( DrawRect.right  - DrawRect.left ) ;
	Height = ( DWORD )( DrawRect.bottom - DrawRect.top  ) ;
	if( ( Width == 0 ) || ( Height == 0 ) ) return ;

	// 転送先アドレスのセット
	DestBP       = DestImg->UseImage + DrawRect.left * DestImg->Base->ColorDataP->PixelByte + DrawRect.top * DestImg->Base->Pitch ;
	DestAddPitch = DestImg->Base->Pitch - Width * DestImg->Base->ColorDataP->PixelByte ;
	DestPP		 = (DWORD_PTR)DestBP ;

	// 描画の準備
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// カラーモードによって処理を分岐
	switch( DestImg->Base->ColorType )
	{
	case 0 :	// 16bit モード
		// 描画輝度を反映
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
					GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
					BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// ブレンドモードによって処理を分岐
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// ブレンド無し
NOTEX_NOMALDRAW_C16_NOPAL_BNO:
#ifdef DX_NON_INLINE_ASM
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
#else
			_asm
			{
				PUSHF
				CLD
				MOV EDI, DestPP
				MOV EAX, Color
				MOV EDX, EAX
				SHL EDX, 16
				OR EAX, EDX
				MOV EDX, Width
				MOV EBX, Height
			LABEL_NBI_C16_NBR_NAC_BNO_1:
				MOV ECX, EDX
				SHR ECX, 1
				JZ LABEL_NBI_C16_NBR_NAC_BNO_2
				REP STOSD
			LABEL_NBI_C16_NBR_NAC_BNO_2:
				MOV ECX, EDX
				AND ECX, 1
				JZ LABEL_NBI_C16_NBR_NAC_BNO_3
				MOV [EDI], AX
				ADD EDI, 2
			LABEL_NBI_C16_NBR_NAC_BNO_3:
				ADD EDI, DestAddPitch
				DEC EBX
				JNZ LABEL_NBI_C16_NBR_NAC_BNO_1
				POPF
			}
#endif
			break ;

		case DX_BLENDMODE_ALPHA :		// αブレンド
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C16_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_ADD :		// 加算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_SUB :		// 減算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_MUL :		// 乗算ブレンド
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_MULA :	// 乗算+αブレンド
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
			break ;
		}
		break ;
		
	case 1 :	// 32bit モード
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
			((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
			((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// ブレンドモードによって処理を分岐
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// ブレンド無し
NOTEX_NOMALDRAW_C32_NOPAL_BNO:
#ifdef DX_NON_INLINE_ASM
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
#else
			_asm
			{
				PUSHF
				CLD
				MOV EDI, DestPP
				MOV EAX, Color
				MOV EDX, Width
				MOV EBX, Height
			LABEL_NBI_C32_NBR_NAC_BNO_1:
				MOV ECX, EDX
				REP STOSD
				ADD EDI, DestAddPitch
				DEC EBX
				JNZ LABEL_NBI_C32_NBR_NAC_BNO_1
				POPF
			}
#endif
			break ;

		case DX_BLENDMODE_ALPHA :		// αブレンド
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C32_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_ADD :		// 加算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_SUB :		// 減算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_MUL :		// 乗算ブレンド
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_MULA :	// 乗算+αブレンド
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
			break ;
		}
		break ;
	}

	// 終了
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32




#define DRAWLINEMEMIMG_ND( DRAW )	\
{\
	if( DrawSizeX >= DrawSizeY )\
	{\
		j = DrawSizeX / 2 ;\
		i = DrawSizeX ;\
		do{\
			DRAW\
			DestBP += AddX ;\
			j      += DrawSizeY ;\
			if( j >= DrawSizeX ){\
				DestBP += AddY ;\
				j      -= DrawSizeX ;\
			}\
		}while( -- i ) ;\
	}else{\
		j = DrawSizeY / 2 ;\
		i = DrawSizeY ;\
		do{\
			DRAW\
			DestBP += AddY ;\
			j      += DrawSizeX ;\
			if( j >= DrawSizeY ){\
				DestBP += AddX ;\
				j      -= DrawSizeY ;\
			}\
		}while( -- i ) ;\
	}\
}

// イメージに線を描画する
#define DST16	*DestWP
#define SRC16	Color
#define DSTP16	DestWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DestBP
extern void DrawLineMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	RECT DrawArea ;
	int i, j ;
	int DrawSizeX , DrawSizeY ;
	int AddX, AddY ; 
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Data, DataD ;
	DWORD DestPitch ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;

	DrawArea = MemImgManage.DrawArea ;

	// クリッピング処理
	{
		double d1 , d2 ;
		double b1 , b2 ;

		// 描画エリアを外れていた場合は描画しない
		{
			int HiX , LwX , LwY , HiY ;

			if( x2 > x1 ){ HiX = x2 ; LwX = x1 ; }else{ HiX = x1 ; LwX = x2 ; }
			if( y2 > y1 ){ HiY = y2 ; LwY = y1 ; }else{ HiY = y1 ; LwY = y2 ; }

			if( ( ( HiX            - DrawArea.left ) | ( HiY             - DrawArea.top ) |
				  ( DrawArea.right - ( LwX + 1 )   ) | ( DrawArea.bottom - ( LwY + 1 )  ) ) & 0x80000000 ) return ;
//			if( DrawArea.left  >  HiX || DrawArea.top    >  HiY ||
//				DrawArea.right <= LwX || DrawArea.bottom <= LwY ) return ;
		}

		// 線の傾きを取得
		d1 = ( y2 == y1 ) ? 0 : ( x2 - x1 ) / ( double )( y2 - y1 ) ;	// x
		d2 = ( x2 == x1 ) ? 0 : ( y2 - y1 ) / ( double )( x2 - x1 ) ;	// y
		b1 = ( x1 - d1 * y1 ) ;		// x
		b2 = ( y1 - d2 * x1 ) ;		// y

		if( DrawArea.left   >  x1 ){ x1 = DrawArea.left ;       y1 = _DTOL( x1 * d2 + b2 ) ;}
		else
		if( DrawArea.right  <= x1 ){ x1 = DrawArea.right - 1 ;  y1 = _DTOL( x1 * d2 + b2 ) ;}

		if( DrawArea.left   >  x2 ){ x2 = DrawArea.left - 1 ;   y2 = _DTOL( x2 * d2 + b2 ) ;}
		else
		if( DrawArea.right  <= x2 ){ x2 = DrawArea.right  ;     y2 = _DTOL( x2 * d2 + b2 ) ;}
			
		if( DrawArea.top    >  y1 ){ y1 = DrawArea.top ;        x1 = _DTOL( y1 * d1 + b1 ) ;}
		else
		if( DrawArea.bottom <= y1 ){ y1 = DrawArea.bottom - 1 ; x1 = _DTOL( y1 * d1 + b1 ) ;}

		if( DrawArea.top    >  y2 ){ y2 = DrawArea.top - 1 ;    x2 = _DTOL( y2 * d1 + b1 ) ;}
		else
		if( DrawArea.bottom <= y2 ){ y2 = DrawArea.bottom ;     x2 = _DTOL( y2 * d1 + b1 ) ;}

		if( DrawArea.left   >  x1 )	 x1 = DrawArea.left ;
		else
		if( DrawArea.right  <= x1 )	 x1 = DrawArea.right - 1 ; 

		if( DrawArea.left   >  x2 )	 x2 = DrawArea.left - 1 ;
		else
		if( DrawArea.right  <= x2 )	 x2 = DrawArea.right ; 
			
		if( DrawArea.top    >  y1 )	 y1 = DrawArea.top ; 
		else
		if( DrawArea.bottom <= y1 )	 y1 = DrawArea.bottom - 1 ;

		if( DrawArea.top    >  y2 )	 y2 = DrawArea.top  - 1  ;
		else
		if( DrawArea.bottom <= y2 )	 y2 = DrawArea.bottom  ; 
	}

	// 描画サイズを取得
	DrawSizeX = x2 - x1 ;
	DrawSizeY = y2 - y1 ;
	if( DrawSizeX == 0 && DrawSizeY == 0 ) return ; 

	// 進行方向をセット
	if( DrawSizeX < 0 ){ AddX = -1 ; DrawSizeX = -DrawSizeX ; }else{ AddX = 1 ; }
	if( DrawSizeY < 0 ){ AddY = -1 ; DrawSizeY = -DrawSizeY ; }else{ AddY = 1 ; }
	AddX *= DestImg->Base->ColorDataP->PixelByte ;
	AddY *= DestImg->Base->Pitch ;

	// 転送先のアドレスなどをセット
	DestBP    = DestImg->UseImage + x1 * DestImg->Base->ColorDataP->PixelByte + y1 * DestImg->Base->Pitch ;
	DestPitch = DestImg->Base->Pitch ;

	// 描画の準備
	j = 0 ;
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// カラーモードによって処理を分岐
	switch( DestImg->Base->ColorType )
	{
	case 0 :	// 16bit モード
		// 描画輝度を反映
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
					GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
					BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// ブレンドモードによって処理を分岐
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// ブレンド無し
NOTEX_NOMALDRAW_C16_NOPAL_BNO:
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_ALPHA :		// αブレンド
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C16_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_ADD :		// 加算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_SUB :		// 減算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_MUL :		// 乗算ブレンド
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_MULA :	// 乗算+αブレンド
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ) )
			break ;
		}
		break ;
		
	case 1 :	// 32bit モード
		// 描画輝度を反映
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
			((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
			((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// ブレンドモードによって処理を分岐
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// ブレンド無し
NOTEX_NOMALDRAW_C32_NOPAL_BNO:
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_ALPHA :		// αブレンド
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C32_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_ADD :		// 加算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_SUB :		// 減算ブレンド
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_MUL :		// 乗算ブレンド
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_MULA :	// 乗算+αブレンド
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ) )
			break ;
		}
		break ;
	}

	// 終了
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32


// イメージに枠だけのボックスを描画する
extern void DrawLineBoxMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	DrawLineMemImg( DestImg, x1,		y1,		x2,		y1,		Color ) ;
	DrawLineMemImg( DestImg, x2 - 1,	y1 + 1, x2 - 1, y2,		Color ) ;
	DrawLineMemImg( DestImg, x1,		y2 - 1, x2 - 1,	y2 - 1, Color ) ;
	DrawLineMemImg( DestImg, x1,		y1 + 1,	x1,		y2 - 1,	Color ) ;
}

// イメージに点の集合を描画する
extern void DrawPixelSetMemImg( MEMIMG *DestImg, const POINTDATA *PointData, int Num )
{
	int i ;
	DWORD DestPitch ;
	RECT DrawArea ;
	union
	{
		BYTE *DestBP ;
		WORD *DestWP ;
		DWORD *DestDP ;
	} ;
	
	DestBP    = DestImg->UseImage ;
	DestPitch = DestImg->Base->Pitch ;
	DrawArea  = MemImgManage.DrawArea ;

	switch( DestImg->Base->ColorType )
	{
	case 0 :	// 16bit
		for( i = 0 ; i < Num ; i ++, PointData ++ )
		{
			if( ( ( PointData->x - DrawArea.left ) | ( DrawArea.right  - ( PointData->x + 1 ) ) |
				  ( PointData->y - DrawArea.top  ) | ( DrawArea.bottom - ( PointData->y + 1 ) ) ) & 0x80000000 ) continue ;

			*((WORD *)( DestBP + PointData->x * 2 + PointData->y * DestPitch )) = ( WORD )PointData->color ;
		}
		break ;
		
	case 1 :	// 32bit
		for( i = 0 ; i < Num ; i ++, PointData ++ )
		{
			if( ( ( PointData->x - DrawArea.left ) | ( DrawArea.right  - ( PointData->x + 1 ) ) |
				  ( PointData->y - DrawArea.top  ) | ( DrawArea.bottom - ( PointData->y + 1 ) ) ) & 0x80000000 ) continue ;

			*((DWORD *)( DestBP + PointData->x * 4 + PointData->y * DestPitch )) = ( DWORD )PointData->color ;
		}
		break ;
	}
}

// イメージに線の集合を描画する
extern void DrawLineSetMemImg( MEMIMG *DestImg, const LINEDATA *LineData, int Num )
{
	int i ;
	for( i = 0 ; i < Num ; i ++, LineData ++ )
	{
		DrawLineMemImg( DestImg, LineData->x1, LineData->y1, LineData->x2, LineData->y2, LineData->color ) ;
	}
}

// イメージに矩形の集合を描画する
extern void DrawBoxSetMemImg( MEMIMG *DestImg, const RECTDATA *RectData, int Num )
{
	int i ;
	for( i = 0 ; i < Num ; i ++, RectData ++ )
	{
		RECT DrawRect ;
		DrawRect.left   = RectData->x1 ;
		DrawRect.top    = RectData->y1 ;
		DrawRect.right  = RectData->x2 ;
		DrawRect.bottom = RectData->y2 ;
		DrawFillBoxMemImg( DestImg, &DrawRect, RectData->color ) ;
	}
}

#define DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )	\
	if( !( ( x2 < DrawArea.left ) | ( x1 >= DrawArea.right  ) |\
		   ( y1 < DrawArea.top  ) | ( y1 >= DrawArea.bottom ) | LineDrawBuf[ y1 ] ) )\
	{\
		if( x1 < DrawArea.left  ) x1 = DrawArea.left  ;\
		if( x2 > DrawArea.right ) x2 = DrawArea.right ;\
\
		LineDrawBuf[ y1 ] = 1 ;\
\
		C = x2 - x1 + 1 ;\
		DrawBP = DestBP + DestPitch * y1 + x1 * (DESTADDNUM) ;\
		do{\
			DRAW\
			DrawBP += (DESTADDNUM) ;\
		}while( -- C ) ;\
	}


#define DRAWCIRCLEMEMIMG_FILL_ND( DRAW, DESTADDNUM )	\
{\
	if( Ry_One_Minus == FALSE )\
	{\
		x2 = Dx + x - Rx_One_Minus ; x1 = -Dx + x ; y1 = Dy + y ;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
	}\
\
	if( F >= 0 )\
	{\
		x2 = Dy + x - Rx_One_Minus ; x1 = -Dy + x ; y1 = Dx + y - Ry_One_Minus ;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
		x2 = Dy + x - Rx_One_Minus ; x1 = -Dy + x ; y1 = -Dx + y;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
		Dx -- ;\
		F -= 4 * Dx ;\
	}\
\
	Dy ++ ;\
	F += 4 * Dy + 2 ;\
\
	while( Dx >= Dy )\
	{\
		x2 = Dx + x - Rx_One_Minus ; x1 = -Dx + x ; y1 = Dy + y - Ry_One_Minus ;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
		x2 = Dx + x - Rx_One_Minus ; x1 = -Dx + x ; y1 = -Dy + y;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
		if( F >= 0 )\
		{\
			x2 = Dy + x - Rx_One_Minus ; x1 = -Dy + x ; y1 = Dx + y - Ry_One_Minus ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
			x2 = Dy + x - Rx_One_Minus ; x1 = -Dy + x ; y1 = -Dx + y;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
			Dx -- ;\
			F -= 4 * Dx ;\
		}\
\
		Dy ++ ;\
		F += 4 * Dy + 2 ;\
	}\
}



#define DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )	\
	if( ( ( (DWORD)( x1 - DrawArea.left ) <= (DWORD)( DrawArea.right  - DrawArea.left ) ) &&\
		  ( (DWORD)( y1 - DrawArea.top  ) <= (DWORD)( DrawArea.bottom - DrawArea.top  ) ) ) )\
	{\
		DrawBP = DestBP + DestPitch * y1 + x1 * (DESTADDNUM) ;\
		DRAW\
	}

#define DRAWCIRCLEMEMIMG_PSET_ND( DRAW, DESTADDNUM )	\
{\
	if( Ry_One_Minus == FALSE )\
	{\
		x1 = -Dx + x ; y1 = Dy + y ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = Dx + x - Rx_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
	}\
\
	if( Rx_One_Minus == FALSE )\
	{\
		x1 = Dy + x ; y1 = Dx + y - Ry_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = -Dy + x ; y1 = -Dx + y;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
	}\
\
	if( F >= 0 )\
	{\
		Dx -- ;\
		F -= 4 * Dx ;\
	}\
\
	Dy ++ ;\
	F += 4 * Dy + 2 ;\
\
	while( Dx >= Dy )\
	{\
		x1 = -Dx + x ; y1 = Dy + y - Ry_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = Dx + x - Rx_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = -Dx + x ; y1 = -Dy + y;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = Dx + x - Rx_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
\
		x1 = -Dy + x ; y1 = Dx + y - Ry_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = Dy + x - Rx_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = -Dy + x ; y1 = -Dx + y;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = Dy + x - Rx_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		if( F >= 0 )\
		{\
			Dx -- ;\
			F -= 4 * Dx ;\
		}\
\
		Dy ++ ;\
		F += 4 * Dy + 2 ;\
	}\
}



// イメージに円を描画する
#define DST16	*DrawWP
#define SRC16	Color
#define DSTP16	DrawWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DrawBP
extern void DrawCircleMemImg( MEMIMG *DestImg, int x, int y, int r, unsigned int Color, int FillFlag, int Rx_One_Minus, int Ry_One_Minus )
{
	BYTE *LineDrawBuf ;
	RECT DrawArea ;
	DWORD Src1, Data, DataD ;
	DWORD *RateTable1 ;
	DWORD DestPitch ;
	int *RateTable2 ;
	int Dx, Dy, F ;
	int x1, x2, y1, C ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DrawBP ;
		WORD  *DrawWP ;
		DWORD *DrawDP ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;

	Rx_One_Minus = Rx_One_Minus ? 1 : 0 ;
	Ry_One_Minus = Ry_One_Minus ? 1 : 0 ;

	// 描画領域のセット
	DrawArea = MemImgManage.DrawArea ;
	DrawArea.right  -- ;
	DrawArea.bottom -- ;

	// 転送先のアドレスをセット
	DestBP    = DestImg->UseImage ;
	DestPitch = DestImg->Base->Pitch ;

	// 描画の準備
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// 初期値セット
	Dx = r ;
	Dy = 0 ;
	F = -2 * r + 3 ;

	// 塗りつぶすかどうかで処理を分岐
	if( FillFlag )
	{
		if( ( LineDrawBuf = (BYTE *)DXCALLOC( ( size_t )( DrawArea.bottom + 1 ) ) ) == NULL )
			return ;

		// カラーモードによって処理を分岐
		switch( DestImg->Base->ColorType )
		{
		case 0 :	// 16bit モード
			// 描画輝度を反映
			if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
			{
				Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
						GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
						BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
			}
			
			// ブレンドモードによって処理を分岐
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// ブレンド無し
FILL_NOMALDRAW_C16_NOPAL_BNO:
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto FILL_NOMALDRAW_C16_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
				break ;
			}
			break ;
			
		case 1 :	// 32bit モード
			if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
			{
				((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
				((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
				((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
			}
			
			// ブレンドモードによって処理を分岐
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// ブレンド無し
FILL_NOMALDRAW_C32_NOPAL_BNO:
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto FILL_NOMALDRAW_C32_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
				break ;
			}
			break ;
		}

LINEEND:
		DXFREE( LineDrawBuf ) ;
	}
	else
	{
		// カラーモードによって処理を分岐
		switch( DestImg->Base->ColorType )
		{
		case 0 :	// 16bit モード
			// 描画輝度を反映
			if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
			{
				Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
						GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
						BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
			}
			
			// ブレンドモードによって処理を分岐
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// ブレンド無し
PSET_NOMALDRAW_C16_NOPAL_BNO:
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto PSET_NOMALDRAW_C16_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
				break ;
			}
			break ;
			
		case 1 :	// 32bit モード
			if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
			{
				((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
				((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
				((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
			}
			
			// ブレンドモードによって処理を分岐
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// ブレンド無し
PSET_NOMALDRAW_C32_NOPAL_BNO:
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto PSET_NOMALDRAW_C32_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
				break ;
			}
			break ;
		}
	}

	// 終了
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32



#define DRAWOVALMEMIMG_FILL_RX_ND( DRAW, DESTADDNUM )	\
{\
	if( Ry_One_Minus == FALSE )\
	{\
		x1 = -Dx + x ; x2 = Dx + x - Rx_One_Minus ; y1 = Dy + y ;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
	}\
\
	Df = 0 ;\
	if( F < 0 ){\
		yc += ry ;\
		if( yc >= rc ){\
			Dy ++ ;\
			yc -= rc ;\
			Df  = 1  ;\
		}\
		F += 4 * Dy + 2 ;\
		H += 4 * Dy     ;\
	}else{\
		if( H >= 0 ){\
			Dx -- ;\
			F -= 4 * Dx     ;\
			H -= 4 * Dx - 2 ;\
		}else{\
			Dx -- ;\
			yc += ry ;\
			if( yc >= rc ){\
				Dy ++ ;\
				yc -= rc ;\
				Df  = 1  ;\
			}\
			F += 4 * Dy - 4 * Dx + 2 ;\
			H += 4 * Dy - 4 * Dx + 2 ;\
		}\
	}\
\
	while( Dx >= 0 )\
	{\
		if( Df )\
		{\
			x1 = -Dx + x ; x2 = Dx + x - Rx_One_Minus ; y1 = Dy + y - Ry_One_Minus ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
			y1 = -Dy + y ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
		}\
\
		Df = 0 ;\
		if( F < 0 ){\
			yc += ry ;\
			if( yc >= rc ){\
				Dy ++ ;\
				yc -= rc ;\
				Df = 1 ;\
			}\
			F += 4 * Dy + 2 ;\
			H += 4 * Dy ;\
		}else{\
			if( H >= 0 ){\
				Dx -- ;\
				F -= 4 * Dx ;\
				H -= 4 * Dx - 2 ;\
			}else{\
				Dx -- ;\
				yc += ry ;\
				if( yc >= rc ){\
					Dy ++ ;\
					yc -= rc ;\
					Df = 1 ;\
				}\
				F += 4 * Dy - 4 * Dx + 2 ;\
				H += 4 * Dy - 4 * Dx + 2 ;\
			}\
		}\
	}\
\
	x1 = -Dx + x ; x2 = Dx + x - Rx_One_Minus ; y1 = -Dy + y;\
	DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
}


#define DRAWOVALMEMIMG_FILL_RY_ND( DRAW, DESTADDNUM )	\
{\
	if( F < 0 )\
	{\
		xc += rx ;\
		if( xc >= rc )\
		{\
			Dx ++ ;\
			xc -= rc ;\
		}\
		F += 4 * Dx + 2 ;\
		H += 4 * Dx;\
	}\
	else\
	{\
		if( H>=0 )\
		{\
			Dy--;\
			F -= 4 * Dy;\
			H -= 4 * Dy - 2 ;\
		}\
		else\
		{\
			Dy--;\
			xc += rx ;\
			if( xc >= rc )\
			{\
				Dx ++ ;\
				xc -= rc ;\
			}\
			F += 4 * Dx - 4 * Dy + 2 ;\
			H += 4 * Dx - 4 * Dy + 2 ;\
		}\
	}\
	while( Dy >= 0 )\
	{\
		if( F < 0 )\
		{\
			xc += rx ;\
			if( xc >= rc )\
			{\
				Dx ++ ;\
				xc -= rc ;\
			}\
			F += 4 * Dx + 2 ;\
			H += 4 * Dx;\
		}\
		else\
		{\
			y1 = -Dy + y ; x1 = -Dx + x ; x2 = Dx + x - Rx_One_Minus ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
			\
			y1 = Dy + y - Ry_One_Minus ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
			\
			if( H>=0 )\
			{\
				Dy--;\
				F -= 4 * Dy;\
				H -= 4 * Dy - 2 ;\
			}\
			else\
			{\
				Dy--;\
				xc += rx ;\
				if( xc >= rc )\
				{\
					Dx ++ ;\
					xc -= rc ;\
				}\
				F += 4 * Dx - 4 * Dy + 2 ;\
				H += 4 * Dx - 4 * Dy + 2 ;\
			}\
		}\
	}\
}


#define DRAWOVALMEMIMG_PSET_RX_ND( DRAW, DESTADDNUM )	\
{\
	if( Ry_One_Minus == FALSE )\
	{\
		x1 = -Dx + x ; y1 = Dy + y ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
		x1 = Dx + x - Rx_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
	}\
\
	Df = 0 ;\
\
	if( F < 0 ){\
		yc += ry ;\
		if( yc >= rc ){\
			Dy ++ ;\
			yc -= rc ;\
			Df = 1 ;\
		}\
		F += 4 * Dy + 2 ;\
		H += 4 * Dy ;\
	}else{\
		Df = 1 ;\
\
		if( H >= 0 ){\
			Dx -- ;\
			F -= 4 * Dx ;\
			H -= 4 * Dx - 2 ;\
		}else{\
			Dx -- ;\
			yc += ry ;\
			if( yc >= rc ){\
				Dy ++ ;\
				yc -= rc ;\
			}\
			F += 4 * Dy - 4 * Dx + 2 ;\
			H += 4 * Dy - 4 * Dx + 2 ;\
		}\
	}\
\
	while( Dx > 0 )\
	{\
		if( Df )\
		{\
			x1 = -Dx + x ; y1 = Dy + y - Ry_One_Minus ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			x1 = Dx + x - Rx_One_Minus ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
			x1 = -Dx + x ; y1 = -Dy + y ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			x1 = Dx + x - Rx_One_Minus ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
		}\
\
		Df = 0 ;\
		if( F < 0 ){\
			yc += ry ;\
			if( yc >= rc ){\
				Dy ++ ;\
				yc -= rc ;\
				Df = 1 ;\
			}\
			F += 4 * Dy + 2 ;\
			H += 4 * Dy ;\
		}else{\
			Df = 1 ;\
\
			if( H >= 0 ){\
				Dx -- ;\
				F -= 4 * Dx ;\
				H -= 4 * Dx - 2 ;\
			}else{\
				Dx -- ;\
				yc += ry ;\
				if( yc >= rc ){\
					Dy ++ ;\
					yc -= rc ;\
				}\
				F += 4 * Dy - 4 * Dx + 2 ;\
				H += 4 * Dy - 4 * Dx + 2 ;\
			}\
		}\
	}\
\
	if( Rx_One_Minus == FALSE )\
	{\
		x1 = Dx + x ; y1 = -Dy + y ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = -Dx + x ; y1 = Dy + y - Ry_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
	}\
}


#define DRAWOVALMEMIMG_PSET_RY_ND( DRAW, DESTADDNUM )	\
{\
	{\
		if( Ry_One_Minus == FALSE )\
		{\
			y1 = -Dy + y ; x1 = Dx + x ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			y1 = Dy + y - Ry_One_Minus ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
		}\
\
		Df = 0 ;\
\
		if( F < 0 )\
		{\
			xc += rx ;\
			if( xc >= rc )\
			{\
				Dx ++ ;\
				xc -= rc ;\
				Df = 1 ;\
			}\
			F += 4 * Dx + 2 ;\
			H += 4 * Dx;\
		}\
		else\
		{\
			Df = 1 ;\
\
			if( H>=0 )\
			{\
				Dy--;\
				F -= 4 * Dy;\
				H -= 4 * Dy - 2 ;\
			}\
			else\
			{\
				Dy--;\
				xc += rx ;\
				if( xc >= rc )\
				{\
					Dx ++ ;\
					xc -= rc ;\
					Df = 1 ;\
				}\
				F += 4 * Dx - 4 * Dy + 2 ;\
				H += 4 * Dx - 4 * Dy + 2 ;\
			}\
		}\
	}\
\
	while( Dy > 0 )\
	{\
		if( Df )\
		{\
			y1 = -Dy + y ; x1 = Dx + x - Rx_One_Minus ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			y1 = Dy + y - Ry_One_Minus ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
			y1 = -Dy + y ; x1 = -Dx + x ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			y1 = Dy + y - Ry_One_Minus ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
		}\
\
		Df = 0 ;\
\
		if( F < 0 )\
		{\
			xc += rx ;\
			if( xc >= rc )\
			{\
				Dx ++ ;\
				xc -= rc ;\
				Df = 1 ;\
			}\
			F += 4 * Dx + 2 ;\
			H += 4 * Dx;\
		}\
		else\
		{\
			Df = 1 ;\
\
			if( H>=0 )\
			{\
				Dy--;\
				F -= 4 * Dy;\
				H -= 4 * Dy - 2 ;\
			}\
			else\
			{\
				Dy--;\
				xc += rx ;\
				if( xc >= rc )\
				{\
					Dx ++ ;\
					xc -= rc ;\
					Df = 1 ;\
				}\
				F += 4 * Dx - 4 * Dy + 2 ;\
				H += 4 * Dx - 4 * Dy + 2 ;\
			}\
		}\
	}\
\
	if( Rx_One_Minus == FALSE )\
	{\
		y1 = Dy + y ; x1 = -Dx + x ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		y1 = -Dy + y ; x1 = Dx + x - Rx_One_Minus ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
	}\
}




// イメージに楕円を描画する
#define DST16	*DrawWP
#define SRC16	Color
#define DSTP16	DrawWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DrawBP
extern void DrawOvalMemImg( MEMIMG *DestImg, int x, int y, int rx, int ry, unsigned int Color, int FillFlag, int Rx_One_Minus, int Ry_One_Minus )
{
	int DestPitch ;
	DWORD Src1, Data, DataD ;
	BYTE *LineDrawBuf ;
	RECT DrawArea ;
	DWORD *RateTable1 ;
	int *RateTable2 ;
	int Dx, Dy, F , H ;
	int x1 , x2 , y1 , Df ;
	int yc , xc , rc , r ;
	int C ;
	
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DrawBP ;
		WORD  *DrawWP ;
		DWORD *DrawDP ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;

	if( ( rx == 0 ) | ( ry == 0 ) ) return ; 

	// 描画領域のセット
	DrawArea = MemImgManage.DrawArea ;
	DrawArea.right  -- ;
	DrawArea.bottom -- ;
	
	// 補正
	if( rx < 0 ) rx = -rx ;
	if( ry < 0 ) ry = -ry ;

	// 転送先のアドレスをセット
	DestBP    = DestImg->UseImage ;
	DestPitch = ( int )DestImg->Base->Pitch ;

	// 描画の準備
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// 描画開始
	if( FillFlag )
	{
		if( ( LineDrawBuf = ( BYTE * )DXCALLOC( ( size_t )( DrawArea.bottom + 1 ) ) ) == NULL )
			return ;

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
			
			// カラーモードによって処理を分岐
			switch( DestImg->Base->ColorType )
			{
			case 0 :	// 16bit モード
				// 描画輝度を反映
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
							GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
							BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
FILL_RX_NOMALDRAW_C16_NOPAL_BNO:
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto FILL_RX_NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
					break ;
				}
				break ;
				
			case 1 :	// 32bit モード
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
					((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
					((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
FILL_RX_NOMALDRAW_C32_NOPAL_BNO:
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto FILL_RX_NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
					break ;
				}
				break ;
			}
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

			// カラーモードによって処理を分岐
			switch( DestImg->Base->ColorType )
			{
			case 0 :	// 16bit モード
				// 描画輝度を反映
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
							GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
							BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
FILL_RY_NOMALDRAW_C16_NOPAL_BNO:
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto FILL_RY_NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
					break ;
				}
				break ;
				
			case 1 :	// 32bit モード
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
					((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
					((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
FILL_RY_NOMALDRAW_C32_NOPAL_BNO:
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto FILL_RY_NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
					break ;
				}
				break ;
			}
		}

LINEEND:
		DXFREE( LineDrawBuf ) ;
	}
	else
	{
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

			// カラーモードによって処理を分岐
			switch( DestImg->Base->ColorType )
			{
			case 0 :	// 16bit モード
				// 描画輝度を反映
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
							GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
							BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
PSET_RX_NOMALDRAW_C16_NOPAL_BNO:
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto PSET_RX_NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
					break ;
				}
				break ;
				
			case 1 :	// 32bit モード
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
					((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
					((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
PSET_RX_NOMALDRAW_C32_NOPAL_BNO:
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto PSET_RX_NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
					break ;
				}
				break ;
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

			// カラーモードによって処理を分岐
			switch( DestImg->Base->ColorType )
			{
			case 0 :	// 16bit モード
				// 描画輝度を反映
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
							GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
							BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
PSET_RY_NOMALDRAW_C16_NOPAL_BNO:
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto PSET_RY_NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
					break ;
				}
				break ;
				
			case 1 :	// 32bit モード
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
					((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
					((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
PSET_RY_NOMALDRAW_C32_NOPAL_BNO:
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto PSET_RY_NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_INVSRC :	// 描画元の色を反転＋αブレンド
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
					break ;
				}
				break ;
			}
		}
	}

	// 終了
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32



#endif

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

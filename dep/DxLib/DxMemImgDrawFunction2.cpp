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
#include "DxLib.h"
#include "DxStatic.h"
#include "DxBaseFunc.h"
#include "DxGraphics.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// データ型定義----------------------------------------------------------------

// データ宣言------------------------------------------------------------------

// 関数プロトタイプ宣言--------------------------------------------------------

// プログラム------------------------------------------------------------------

#ifndef DX_NON_GRAPHICS

#ifdef DX_NON_2DDRAW

extern void DrawPolygonMemImg( MEMIMG *DestImg, MEMIMG *ZImg, const MEMIMG *SrcImg, const VERTEX_2D *Vertex3, int TransFlag, const MEMIMG *BlendImg, int ZWriteFlag, int PerspectiveEnable, int GouraudShadeMode, int ScissorTest )
{
	return ;
}

#else

#define DRAWPOLYGONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	/*y = StartY ;*/\
	ycon = 0 ;\
	for( ; ycon < NumY ; ycon ++,/*y ++,*/\
		 RHWT += RHWPY,\
		 UT += UPY,\
		 VT += VPY,\
		 ZT += ZPY,\
		 RT += RPY,\
		 GT += GPY,\
		 BT += BPY,\
		 AT += APY,\
		 ZBufPT += ZPitch,\
		 DestPT += DestPitch\
	 )\
	{\
		xcon = StartX_NumX[ycon][1] ;\
		if( xcon == 0 ) continue ;\
		x = StartX_NumX[ycon][0] ;\
		\
		RHW = RHWPX * x + RHWPX / 2 + RHWT ;\
		Z   = ZPX   * x + ZPX   / 2 + ZT ;\
		U   = UPX   * x + UPX   / 2 + UT ;\
		V   = VPX   * x + VPX   / 2 + VT ;\
		R   = RPX   * x + RPX   / 2 + RT ;\
		G   = GPX   * x + GPX   / 2 + GT ;\
		B   = BPX   * x + BPX   / 2 + BT ;\
		A   = APX   * x + APX   / 2 + AT ;\
		ZBPT = x * 2 + ZBufPT ;\
		DestBPT = x * (DESTADDNUM) + DestPT ;\
		if( RHW == 0 ) continue ;\
		\
		for( ; xcon > 0 ; xcon --, /*x ++,*/\
		 RHW += RHWPX,\
		 U += UPX,\
		 V += VPX,\
		 Z += ZPX,\
		 R += RPX,\
		 G += GPX,\
		 B += BPX,\
		 A += APX,\
		 ZBPT += 2,\
		 DestBPT += (DESTADDNUM)\
		  )\
		{\
			if( ZImg )\
			{\
				if( *ZWPT < (WORD)( Z >> 8 ) )\
					continue ;\
				if( ZWriteFlag ) *ZWPT = ( WORD )( Z >> 8 ) ;\
			}\
\
			INV = ( DWORD )( (1 << 29) / RHW ) ;\
			tu = ( int )( ( ( U >> 10 ) * texWidth  * INV ) >> 19 ) ;\
			tv = ( int )( ( ( V >> 10 ) * texHeight * INV ) >> 19 ) ;\
\
			if( (DWORD)( ( tu | ( texWidth - tu - 1 ) ) | ( tv | ( texHeight - tv - 1 ) ) ) & 0x80000000 )\
				continue ;\
\
			SrcBPT  = SrcBP + tu * (SRCADDNUM) + tv * SrcPitch ;\
			BlendBPT = BlendBP + tu + tv * BlendPitch ;\
\
			DRAW\
		}\
	}\
}

#define DRAWPOLYGONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	/*y = StartY ;*/\
	ycon = 0 ;\
	for( ; ycon < NumY ; ycon ++,/* y ++,*/\
		 RHWT += RHWPY,\
		 UT += UPY,\
		 VT += VPY,\
		 ZT += ZPY,\
		 RT += RPY,\
		 GT += GPY,\
		 BT += BPY,\
		 AT += APY,\
		 ZBufPT += ZPitch,\
		 DestPT += DestPitch\
	 )\
	{\
		xcon = StartX_NumX[ycon][1] ;\
		if( xcon == 0 ) continue ;\
		x = StartX_NumX[ycon][0] ;\
		\
		RHW = RHWPX * x + RHWPX / 2 + RHWT ;\
		Z   = ZPX   * x + ZPX   / 2 + ZT ;\
		U   = UPX   * x + UPX   / 2 + UT ;\
		V   = VPX   * x + VPX   / 2 + VT ;\
		R   = RPX   * x + RPX   / 2 + RT ;\
		G   = GPX   * x + GPX   / 2 + GT ;\
		B   = BPX   * x + BPX   / 2 + BT ;\
		A   = APX   * x + APX   / 2 + AT ;\
		ZBPT = x * 2 + ZBufPT ;\
		DestBPT = x * (DESTADDNUM) + DestPT ;\
		if( RHW == 0 ) continue ;\
		\
		for( ; xcon > 0 ; xcon --, /*x ++,*/\
		 RHW += RHWPX,\
		 U += UPX,\
		 V += VPX,\
		 Z += ZPX,\
		 R += RPX,\
		 G += GPX,\
		 B += BPX,\
		 A += APX,\
		 ZBPT += 2,\
		 DestBPT += (DESTADDNUM)\
		  )\
		{\
			if( ZImg )\
			{\
				if( *ZWPT < (WORD)( Z >> 8 ) )\
					continue ;\
				if( ZWriteFlag ) *ZWPT = ( WORD )( Z >> 8 ) ;\
			}\
\
			INV = ( DWORD )( (1 << 29) / RHW ) ;\
			tu = ( int )( ( ( U >> 10 ) * texWidth  * INV ) >> 19 ) ;\
			tv = ( int )( ( ( V >> 10 ) * texHeight * INV ) >> 19 ) ;\
\
			if( (DWORD)( ( tu | ( texWidth - tu - 1 ) ) | ( tv | ( texHeight - tv - 1 ) ) ) & 0x80000000 )\
				continue ;\
\
			SrcBPT  = SrcBP + tu * (SRCADDNUM) + tv * SrcPitch ;\
\
			DRAW\
		}\
	}\
}

/*
#define DRAWPOLYGONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	y = StartY ;\
	for( ycon = 0 ; ycon < NumY ; ycon ++ )\
	{\
		xcon = StartX_NumX[ycon][1] ;\
		x = StartX_NumX[ycon][0] ;\
		if( x < DrawRectLeft )\
		{\
			xcon -= DrawRectLeft - x ;\
			x = DrawRectLeft ;\
		}\
		if( x + xcon > DrawRectRight )\
		{\
			xcon = DrawRectRight - x ;\
		}\
		\
		for( ; xcon > 0 ; xcon --, x ++ )\
		{\
			rhw = rhwPX * x + rhwPY * y + rhw0 ;\
			u = tuPX * x + tuPY * y + tu0 ;\
			v = tvPX * x + tvPY * y + tv0 ;\
			r = rPX * x + rPY * y + r0 ;\
			g = gPX * x + gPY * y + g0 ;\
			b = bPX * x + bPY * y + b0 ;\
			a = aPX * x + aPY * y + a0 ;\
			\
			z = 1.0F / rhw ;\
			if( z > 1.0F || z < 0.0F ) continue ;\
			tu = _FTOL( u * z ) ;\
			tv = _FTOL( v * z ) ;\
			if( ( ( tu | ( texWidth - tu ) ) | ( tv | ( texHeight - tv ) ) ) & 0x80000000 ) continue ;\
			\
			SrcBPT = SrcBP + tu * (SRCADDNUM) + tv * SrcPitch ;\
			BlendBPT = BlendBP + tu + tv * BlendPitch ;\
			DestBPT = DestBP + x * (DESTADDNUM) + y * DestPitch ;\
			MemImgManage.DrawBright.Red   = _FTOL( r ) ;\
			MemImgManage.DrawBright.Green = _FTOL( g ) ;\
			MemImgManage.DrawBright.Blue  = _FTOL( b ) ;\
			MemImgManage.BlendParam = _FTOL( a ) ;\
			DRAW\
		}\
		\
		y ++ ;\
		ycon ++ ;\
	}\
}


#define DRAWPOLYGONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	y = StartY ;\
	for( ycon = 0 ; ycon < NumY ; ycon ++ )\
	{\
		xcon = StartX_NumX[ycon][1] ;\
		x = StartX_NumX[ycon][0] ;\
		if( x < DrawRectLeft )\
		{\
			xcon -= DrawRectLeft - x ;\
			x = DrawRectLeft ;\
		}\
		if( x + xcon > DrawRectRight )\
		{\
			xcon = DrawRectRight - x ;\
		}\
		\
		for( ; xcon > 0 ; xcon --, x ++ )\
		{\
			rhw = rhwPX * x + rhwPY * y + rhw0 ;\
			u = tuPX * x + tuPY * y + tu0 ;\
			v = tvPX * x + tvPY * y + tv0 ;\
			r = rPX * x + rPY * y + r0 ;\
			g = gPX * x + gPY * y + g0 ;\
			b = bPX * x + bPY * y + b0 ;\
			a = aPX * x + aPY * y + a0 ;\
			\
			z = 1.0F / rhw ;\
			if( z > 1.0F || z < 0.0F ) continue ;\
			tu = _FTOL( u * z * texWidth - 0.5F ) ;\
			tv = _FTOL( v * z * texHeight - 0.5F ) ;\
			if( (DWORD)( ( tu | ( texWidth - tu ) ) | ( tv | ( texHeight - tv ) ) ) & 0x80000000 ) continue ;\
			\
			SrcBPT = SrcBP + tu * (SRCADDNUM) + tv * SrcPitch ;\
			DestBPT = DestBP + x * (DESTADDNUM) + y * DestPitch ;\
			MemImgManage.DrawBright.Red   = _FTOL( r ) ;\
			MemImgManage.DrawBright.Green = _FTOL( g ) ;\
			MemImgManage.DrawBright.Blue  = _FTOL( b ) ;\
			MemImgManage.BlendParam = _FTOL( a ) ;\
			DRAW\
		}\
		\
		y ++ ;\
		ycon ++ ;\
	}\
}
*/


// イメージを使用してポリゴンを描画する( ３Ｄ空間に３Ｄポリゴンを描画するのが目的 )
#define PAL16	PaletteWP[ *SrcBPT ]
#define DST16	*DestWPT
#define SRC16	*SrcWPT
#define SRCA16	SrcBPT[2]
#define DSTP16	DestWPT

#define PALP32	(BYTE *)&PaletteDP[ *SrcBPT ]
#define SRCP32	SrcBPT
#define DSTP32	DestBPT

#define AB		(A / COLORQUALI)
#define RB		(R / COLORQUALI)
#define GB		(G / COLORQUALI)
#define BB		(B / COLORQUALI)

#define BLND	*BlendBPT

#define MAXLINE		(4096)
#define RHWQUALI	(1 << 26)
#define UVQUALI		(1 << 26)
#define ZQUALI		(1 << 24)
#define COLORQUALI	(1 << 14)

extern void DrawPolygonMemImg( MEMIMG *DestImg, MEMIMG *ZImg, const MEMIMG *SrcImg, const VERTEX_2D *Vertex3, int TransFlag, const MEMIMG *BlendImg, int ZWriteFlag, int PerspectiveEnable, int GouraudShadeMode, int ScissorTest )
{
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Src2, Data, DataD ;
	DWORD *RedTable , *GreenTable , *BlueTable ;
	DWORD SrcPitch = 0, DestPitch, BlendPitch = 0, ZPitch = 0 ;
	DWORD TransColor ;
	MEMIMG TempMemImg;
	bool UseTempMemImg = false;

	int DrawRectRight,  DrawRectLeft ;
	int DrawRectBottom, DrawRectTop ;

	float tuPX, tuPY, tvPX, tvPY ;		// a, b, c, d の代わり、スクリーン座標からテクスチャ座標に変換する為のもの
	float tu0, tv0 ;					// U00, V00 の代わり、変換後のテクスチャ座標の原点となる座標
	float rhwPX, rhwPY ;				// スクリーン座標から RHW に変換する為のもの
	float rhw0 ;						// 変換後の RHW の原点となる値
	float zPX, zPY ;					// スクリーン座標から Z に変換する為のもの
	float z0 ;							// 変換後の Z の原点となる値
	float rPX, rPY, gPX, gPY, bPX, bPY, aPX, aPY ;	// スクリーン座標から色値に変換する為のもの
	float r0, g0, b0, a0 ;				// 変換後の色値の原点となる値
	float px0, py0 ;					// x0, y0 の代わり、テクスチャ座標に変換する際のスクリーン座標の原点となる座標
	float StartPLX[2], StartPLY[2], StartPRX[2], StartPRY[2] ;	// BS1X, BS2Y 等の代わり、各頂点の描画開始座標
//	float PLX, PLY, PRX, PRY ;			// B1X, B1Y, B2X, B2Y の代わり、処理中の左右の座標を保持する
	float AddPLX[2], AddPRX[2] ;		// BAA1X 等の代わり、１ライン毎に描画開始座標に加算する値
//	float AdPLX, AdPRX ;				// BA1X, BA2X の代わり、現在のフェーズで使用する AddPLX, AddPRX の値を保存する
	static short StartX_NumX[MAXLINE][2] ; // 各ラインの開始Ｘ座標と処理ドット数( 0:開始Ｘ座標  1:処理ドット数 )
	int StartY, NumY ;					// 処理を開始するＹ座標と、処理するライン数
	int tu, tv ;
//	float rhw, u, v, z, r, g, b, a ;
	int x, /*y,*/ xcon, ycon ;
	int texWidth, texHeight ;

	int RHW, RHWPX, RHWPY, RHW0, RHWT ;
	int U, UT, UPX, UPY, U0 ;
	int V, VT, VPX, VPY, V0 ;
	int Z, ZT, ZPX, ZPY, Z0 ;
	int R, RT, RPX, RPY, R0 ;
	int G, GT, GPX, GPY, G0 ;
	int B, BT, BPX, BPY, B0 ;
	int A, AT, APX, APY, A0 ;
	BYTE *DestPT, *ZBufPT ;
	DWORD INV ; 

	const VERTEX_2D *YTable[3], *XTable[3], *ZTable[3] ;

	union
	{
		BYTE *ZBP ;
		WORD *ZWP ;
	} ;
	union
	{
		BYTE *ZBPT ;
		WORD *ZWPT ;
	} ;
	BYTE *BlendBP = NULL ;
	BYTE *BlendBPT ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *SrcBPT ;
		WORD  *SrcWPT ;
		DWORD *SrcDPT ;
	} ;
	union
	{
		BYTE  *PaletteBP ;
		WORD  *PaletteWP ;
		DWORD *PaletteDP ;
	} ;
	union
	{
		BYTE  *SrcBP ;
		WORD  *SrcWP ;
		DWORD *SrcDP ;
	} ;
	union
	{
		BYTE *DestBP ;
		WORD *DestWP ;
		DWORD *DestDP ;
	} ;
	union
	{
		BYTE *DestBPT ;
		WORD *DestWPT ;
		DWORD *DestDPT ;
	} ;

	// 透過色無効の場合は TransFlag を倒す
	if( SrcImg->Base->UseTransColor == 0 )
	{
		TransFlag = FALSE ;
	}

	// シザリングテストの指定がある場合はシザリングの必要があるか調べ
	// 必要がある場合はシザリングを行う
	if( ScissorTest == TRUE )
	{
		int SrcVertNum, DestVertNum, Flag ;
		VERTEX_2D Check[2][7], TempVert[3], *ChkSrc, *ChkDst ;
		float a ;
		int ia, k, j ;
		VERTEX_2D *dvP, *dvN ;
		int cflag[7], all ;

		// 検査
		all  = cflag[0] = Vertex3[0].pos.z < 0.0F ;
		all += cflag[1] = Vertex3[1].pos.z < 0.0F ;
		all += cflag[2] = Vertex3[2].pos.z < 0.0F ;
		all += cflag[3] = Vertex3[0].pos.z > 1.0F ;
		all += cflag[4] = Vertex3[1].pos.z > 1.0F ;
		all += cflag[5] = Vertex3[2].pos.z > 1.0F ;

		// 全ての頂点がクリッピング対象の場合はここで終了
		if( all >= 3 ) return ;
		
		// シザリングの必要がある場合はシザリング
		if( all != 0 )
		{
			// ここに来たと言うことはシザリングが必要ということ
			SrcVertNum = 3 ;
			DestVertNum = 3 ;
			Flag = 1 ;
			Check[0][0] = Vertex3[0] ;
			Check[0][1] = Vertex3[1] ;
			Check[0][2] = Vertex3[2] ;
			Check[0][0].rhw = 1.0F / Check[0][0].rhw ;
			Check[0][0].pos.x *= Check[0][0].rhw ;
			Check[0][0].pos.y *= Check[0][0].rhw ;
			Check[0][0].pos.z *= Check[0][0].rhw ;
			Check[0][1].rhw = 1.0F / Check[0][1].rhw ;
			Check[0][1].pos.x *= Check[0][1].rhw ;
			Check[0][1].pos.y *= Check[0][1].rhw ;
			Check[0][1].pos.z *= Check[0][1].rhw ;
			Check[0][2].rhw = 1.0F / Check[0][2].rhw ;
			Check[0][2].pos.x *= Check[0][2].rhw ;
			Check[0][2].pos.y *= Check[0][2].rhw ;
			Check[0][2].pos.z *= Check[0][2].rhw ;
			for( k = 0 ; k < 2 ; k ++ )
			{
				ChkDst = Check[Flag] ;
				Flag ^= 1 ;
				ChkSrc = Check[Flag] ;
				ChkSrc[SrcVertNum] = ChkSrc[0] ;

				SrcVertNum = DestVertNum ;
				DestVertNum = 0 ;
				switch( k )
				{
				case 0 :	// Ｚ＋クリップ面
					cflag[0] = ChkSrc[0].pos.z > ChkSrc[0].rhw ;
					for( j = 0 ; j < SrcVertNum ; j ++, ChkSrc ++ )
					{
						cflag[j+1] = ChkSrc[1].pos.z > ChkSrc[1].rhw ;
						if( cflag[j] == TRUE  && cflag[j+1] == TRUE ) continue ;
						if( cflag[j] == FALSE && cflag[j+1] == FALSE )
						{
							ChkDst[DestVertNum] = ChkSrc[0] ;
							DestVertNum ++ ;
							continue ;
						}

						if( cflag[j] == FALSE )
						{
							ChkDst[DestVertNum] = ChkSrc[0] ;
							DestVertNum ++ ;

							dvP = &ChkSrc[0] ;
							dvN = &ChkSrc[1] ;
						}
						else
						{
							dvP = &ChkSrc[1] ;
							dvN = &ChkSrc[0] ;
						}

						a = ( 1.0F - dvP->pos.z / dvP->rhw ) / ( dvN->pos.z / dvN->rhw - dvP->pos.z / dvP->rhw ) ;
						ia = (BYTE)_FTOL( a * 255 ) ;
						ChkDst[DestVertNum].pos.x = dvP->pos.x + ( dvN->pos.x - dvP->pos.x ) * a ; 
						ChkDst[DestVertNum].pos.y = dvP->pos.y + ( dvN->pos.y - dvP->pos.y ) * a ; 
						ChkDst[DestVertNum].pos.z = dvP->pos.z + ( dvN->pos.z - dvP->pos.z ) * a ; 
						ChkDst[DestVertNum].u     = dvP->u     + ( dvN->u     - dvP->u     ) * a ; 
						ChkDst[DestVertNum].v     = dvP->v     + ( dvN->v     - dvP->v     ) * a ; 
						ChkDst[DestVertNum].rhw   = dvP->rhw   + ( dvN->rhw   - dvP->rhw   ) * a ; 
						((BYTE *)&ChkDst[DestVertNum].color)[0] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[0] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[0] ) * ia ) ) >> 8 ) ;
						((BYTE *)&ChkDst[DestVertNum].color)[1] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[1] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[1] ) * ia ) ) >> 8 ) ; 
						((BYTE *)&ChkDst[DestVertNum].color)[2] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[2] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[2] ) * ia ) ) >> 8 ) ; 
						((BYTE *)&ChkDst[DestVertNum].color)[3] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[3] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[3] ) * ia ) ) >> 8 ) ; 
						DestVertNum ++ ;
					}
					break ;

				case 1 :	// Ｚ－クリップ面
					cflag[0] = ChkSrc[0].pos.z < 0.0F ;
					for( j = 0 ; j < SrcVertNum ; j ++, ChkSrc ++ )
					{
						cflag[j+1] = ChkSrc[1].pos.z < 0.0F ;
						if( cflag[j] == TRUE  && cflag[j+1] == TRUE ) continue ;
						if( cflag[j] == FALSE && cflag[j+1] == FALSE )
						{
							ChkDst[DestVertNum] = ChkSrc[0] ;
							DestVertNum ++ ;
							continue ;
						}

						if( cflag[j] == FALSE )
						{
							ChkDst[DestVertNum] = ChkSrc[0] ;
							DestVertNum ++ ;

							dvP = &ChkSrc[1] ;
							dvN = &ChkSrc[0] ;
						}
						else
						{
							dvP = &ChkSrc[0] ;
							dvN = &ChkSrc[1] ;
						}

						a = -dvP->pos.z / ( dvN->pos.z - dvP->pos.z ) ;
						ia = (BYTE)_FTOL( a * 255 ) ;
						ChkDst[DestVertNum].pos.x = dvP->pos.x + ( dvN->pos.x - dvP->pos.x ) * a ; 
						ChkDst[DestVertNum].pos.y = dvP->pos.y + ( dvN->pos.y - dvP->pos.y ) * a ; 
						ChkDst[DestVertNum].pos.z = 0.0F /*dvP->pos.z + ( dvN->pos.z - dvP->pos.z ) * a*/ ; 
						ChkDst[DestVertNum].u     = dvP->u     + ( dvN->u     - dvP->u     ) * a ; 
						ChkDst[DestVertNum].v     = dvP->v     + ( dvN->v     - dvP->v     ) * a ; 
						ChkDst[DestVertNum].rhw   = dvP->rhw   + ( dvN->rhw   - dvP->rhw   ) * a ; 
						((BYTE *)&ChkDst[DestVertNum].color)[0] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[0] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[0] ) * ia ) ) >> 8 ) ;
						((BYTE *)&ChkDst[DestVertNum].color)[1] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[1] << 8 ) + ( ( ((BYTE *)&dvN->color)[1] - ((BYTE *)&dvP->color)[1] ) * ia ) ) >> 8 ) ; 
						((BYTE *)&ChkDst[DestVertNum].color)[2] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[2] << 8 ) + ( ( ((BYTE *)&dvN->color)[2] - ((BYTE *)&dvP->color)[2] ) * ia ) ) >> 8 ) ; 
						((BYTE *)&ChkDst[DestVertNum].color)[3] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[3] << 8 ) + ( ( ((BYTE *)&dvN->color)[3] - ((BYTE *)&dvP->color)[3] ) * ia ) ) >> 8 ) ; 
						DestVertNum ++ ;
					}
					break ;
				}
			}

			for( j = 0 ; j < DestVertNum ; j ++ )
			{
				ChkDst[j].rhw = 1.0F / ChkDst[j].rhw ;
				ChkDst[j].pos.x *= ChkDst[j].rhw ;
				ChkDst[j].pos.y *= ChkDst[j].rhw ;
				ChkDst[j].pos.z *= ChkDst[j].rhw ;
			}

			// シザリングした結果を再度この関数に渡す
			DrawPolygonMemImg( DestImg, ZImg, SrcImg, &ChkDst[0], TransFlag, BlendImg, PerspectiveEnable, FALSE ) ;
			for( j = 3 ; j < DestVertNum ; j ++ )
			{
				TempVert[0] = ChkDst[j] ;
				TempVert[1] = ChkDst[0] ;
				TempVert[2] = ChkDst[j-1] ;
				DrawPolygonMemImg( DestImg, ZImg, SrcImg, TempVert, TransFlag, BlendImg, PerspectiveEnable, FALSE ) ;
			}
			
			// シザリングした場合はここで終了
			return ;
		}
	}

	// Ｚバッファが描画先と同じサイズではなかったらエラー
	if( ZImg != NULL && ( ZImg->Width != DestImg->Width || ZImg->Height != DestImg->Height ) ) return ;

	// テクスチャが無い場合は小さいテクスチャをでっち上げる
	if( SrcImg == NULL )
	{
		UseTempMemImg = true;
		_MEMSET( &TempMemImg, 0, sizeof( TempMemImg ) );
		InitializeMemImg( &TempMemImg, 8, 8, -1, 0, DestImg->Base->ColorType, FALSE, FALSE, FALSE );
		DrawFillBoxMemImg( &TempMemImg, 0, 0, 8, 8, 0xffffffff );
		SrcImg = &TempMemImg;
	}

	// カラータイプが違うか、描画先がαチャンネル付かパレット付きだった場合はエラー
	if( SrcImg->Base->ColorType != DestImg->Base->ColorType || DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// 一番上に来ている座標によって描画起点座標等を決定
	{
		const VERTEX_2D *LP, *RP ;
		int DMODEL, DMODER ;

		DMODEL = DMODER = 0 ;

		// XTable に x の小さい順のアドレスを代入する
		if( Vertex3[0].pos.x > Vertex3[1].pos.x ){
			if( Vertex3[1].pos.x > Vertex3[2].pos.x ){
				XTable[0] = &Vertex3[2] ;
				XTable[1] = &Vertex3[1] ;
				XTable[2] = &Vertex3[0] ;
			}else{
				XTable[0] = &Vertex3[1] ;
				if( Vertex3[2].pos.x > Vertex3[0].pos.x ){
					XTable[1] = &Vertex3[0] ;
					XTable[2] = &Vertex3[2] ;
				}else{
					XTable[1] = &Vertex3[2] ;
					XTable[2] = &Vertex3[0] ;
				}
			}
		}else{
			if( Vertex3[0].pos.x > Vertex3[2].pos.x ){
				XTable[0] = &Vertex3[2] ;
				XTable[1] = &Vertex3[0] ;
				XTable[2] = &Vertex3[1] ;
			}else{
				XTable[0] = &Vertex3[0] ;
				if( Vertex3[2].pos.x > Vertex3[1].pos.x ){
					XTable[1] = &Vertex3[1] ;
					XTable[2] = &Vertex3[2] ;
				}else{
					XTable[1] = &Vertex3[2] ;
					XTable[2] = &Vertex3[1] ;
				}
			}
		}

		// YTable に y の小さい順のアドレスを代入する
		if( Vertex3[0].pos.y > Vertex3[1].pos.y ){
			if( Vertex3[1].pos.y > Vertex3[2].pos.y ){
				YTable[0] = &Vertex3[2] ;
				YTable[1] = &Vertex3[1] ;
				YTable[2] = &Vertex3[0] ;
			}else{
				YTable[0] = &Vertex3[1] ;
				if( Vertex3[2].pos.y > Vertex3[0].pos.y ){
					YTable[1] = &Vertex3[0] ;
					YTable[2] = &Vertex3[2] ;
				}else{
					YTable[1] = &Vertex3[2] ;
					YTable[2] = &Vertex3[0] ;
				}
			}
		}else{
			if( Vertex3[0].pos.y > Vertex3[2].pos.y ){
				YTable[0] = &Vertex3[2] ;
				YTable[1] = &Vertex3[0] ;
				YTable[2] = &Vertex3[1] ;
			}else{
				YTable[0] = &Vertex3[0] ;
				if( Vertex3[2].pos.y > Vertex3[1].pos.y ){
					YTable[1] = &Vertex3[1] ;
					YTable[2] = &Vertex3[2] ;
				}else{
					YTable[1] = &Vertex3[2] ;
					YTable[2] = &Vertex3[1] ;
				}
			}
		}

		// ZTable に z の小さい順のアドレスを代入する
		if( Vertex3[0].pos.z > Vertex3[1].pos.z ){
			if( Vertex3[1].pos.z > Vertex3[2].pos.z ){
				ZTable[0] = &Vertex3[2] ;
				ZTable[1] = &Vertex3[1] ;
				ZTable[2] = &Vertex3[0] ;
			}else{
				ZTable[0] = &Vertex3[1] ;
				if( Vertex3[2].pos.z > Vertex3[0].pos.z ){
					ZTable[1] = &Vertex3[0] ;
					ZTable[2] = &Vertex3[2] ;
				}else{
					ZTable[1] = &Vertex3[2] ;
					ZTable[2] = &Vertex3[0] ;
				}
			}
		}else{
			if( Vertex3[0].pos.z > Vertex3[2].pos.z ){
				ZTable[0] = &Vertex3[2] ;
				ZTable[1] = &Vertex3[0] ;
				ZTable[2] = &Vertex3[1] ;
			}else{
				ZTable[0] = &Vertex3[0] ;
				if( Vertex3[2].pos.z > Vertex3[1].pos.z ){
					ZTable[1] = &Vertex3[1] ;
					ZTable[2] = &Vertex3[2] ;
				}else{
					ZTable[1] = &Vertex3[2] ;
					ZTable[2] = &Vertex3[1] ;
				}
			}
		}

		// 完全に画面外に出ていないか調べる
		if( XTable[2]->pos.x < (float)MemImgManage.DrawArea.left || XTable[0]->pos.x >= (float)MemImgManage.DrawArea.right ||
			YTable[2]->pos.y < (float)MemImgManage.DrawArea.top  || YTable[0]->pos.y >= (float)MemImgManage.DrawArea.bottom ||
			ZTable[2]->pos.z < 0.0F                              || ZTable[0]->pos.z >= 1.0F )
			goto END ;

		// サイズが２ドット以下だったら何もせずに終了
		if( ( YTable[2]->pos.y - YTable[0]->pos.y < 1.1F ) ||
			( XTable[2]->pos.x - XTable[0]->pos.x < 1.1F ) ) goto END ;

		// スクリーン座標からUV座標、RHW、RGBA を求める為の計算値を算出する
		// Vx = V1x * p1 + V2x * p2 ;
		// Vy = V1y * p1 + V2y * p2 ;
		// Tu = T1u * p1 + T2u * p2 ;
		// Tv = T1v * p1 + T2v * p2 ;
		// 等の式から Vx と Vy から各値に変換できる２値を算出しています。
		// 全部 px20 * py10 - py20 * px10 ----(1) で割っているのは、片方は
		// 本来なら py10 * px20 - px10 * py20 ----(2) なのですが、どちらも
		// 外積の計算に等しく、丁度プラスマイナスが違うだけなので (2) で
		// 割っている部分の符号を全て反転することで、全ての除算を (1) で済ませています
		{
			float tmp ;
			float px20, py20, px10, py10 ;
			float tu20, tu10, tv20, tv10 ;
			float rhw20, rhw10 ;
			float z20, z10 ;
			float r20, r10, g20, g10, b20, b10, a20, a10 ;
			
			px20 = XTable[2]->pos.x - XTable[0]->pos.x ;
			py20 = XTable[2]->pos.y - XTable[0]->pos.y ;
			px10 = XTable[1]->pos.x - XTable[0]->pos.x ;
			py10 = XTable[1]->pos.y - XTable[0]->pos.y ;
			
			tu20  = ( XTable[2]->u * XTable[2]->rhw ) - ( XTable[0]->u * XTable[0]->rhw ) ;
			tu10  = ( XTable[1]->u * XTable[1]->rhw ) - ( XTable[0]->u * XTable[0]->rhw ) ;
			tv20  = ( XTable[2]->v * XTable[2]->rhw ) - ( XTable[0]->v * XTable[0]->rhw ) ;
			tv10  = ( XTable[1]->v * XTable[1]->rhw ) - ( XTable[0]->v * XTable[0]->rhw ) ;
			rhw20 = XTable[2]->rhw   - XTable[0]->rhw ;
			rhw10 = XTable[1]->rhw   - XTable[0]->rhw ;
			z20   = XTable[2]->pos.z - XTable[0]->pos.z ;
			z10   = XTable[1]->pos.z - XTable[0]->pos.z ;
			r20   = (float)( ((BYTE *)&XTable[2]->color)[2] - ((BYTE *)&XTable[0]->color)[2] ) ;
			r10   = (float)( ((BYTE *)&XTable[1]->color)[2] - ((BYTE *)&XTable[0]->color)[2] ) ;
			g20   = (float)( ((BYTE *)&XTable[2]->color)[1] - ((BYTE *)&XTable[0]->color)[1] ) ;
			g10   = (float)( ((BYTE *)&XTable[1]->color)[1] - ((BYTE *)&XTable[0]->color)[1] ) ;
			b20   = (float)( ((BYTE *)&XTable[2]->color)[0] - ((BYTE *)&XTable[0]->color)[0] ) ;
			b10   = (float)( ((BYTE *)&XTable[1]->color)[0] - ((BYTE *)&XTable[0]->color)[0] ) ;
			a20   = (float)( ((BYTE *)&XTable[2]->color)[3] - ((BYTE *)&XTable[0]->color)[3] ) ;
			a10   = (float)( ((BYTE *)&XTable[1]->color)[3] - ((BYTE *)&XTable[0]->color)[3] ) ;

			tmp = px10 * py20 - px20 * py10;
			if( tmp == 0 ) goto END ;

			tmp   = 1.0F / tmp ;
			tuPX  = (tu10  * py20 - tu20  * py10) * tmp ;	tuPY  = (tu20  * px10 - tu10  * px20) * tmp ;
			tvPX  = (tv10  * py20 - tv20  * py10) * tmp ;	tvPY  = (tv20  * px10 - tv10  * px20) * tmp ;
			rhwPX = (rhw10 * py20 - rhw20 * py10) * tmp ;	rhwPY = (rhw20 * px10 - rhw10 * px20) * tmp ;
			zPX   = (z10   * py20 - z20   * py10) * tmp ;	zPY   = (z20   * px10 - z10   * px20) * tmp ;
			rPX   = (r10   * py20 - r20   * py10) * tmp ;	rPY   = (r20   * px10 - r10   * px20) * tmp ;
			gPX   = (g10   * py20 - g20   * py10) * tmp ;	gPY   = (g20   * px10 - g10   * px20) * tmp ;
			bPX   = (b10   * py20 - b20   * py10) * tmp ;	bPY   = (b20   * px10 - b10   * px20) * tmp ;
			aPX   = (a10   * py20 - a20   * py10) * tmp ;	aPY   = (a20   * px10 - a10   * px20) * tmp ;

			px0  = XTable[0]->pos.x ;
			py0  = XTable[0]->pos.y ;
			tu0  = ( XTable[0]->u * XTable[0]->rhw ) - tuPX * px0 - tuPY * py0 ;
			tv0  = ( XTable[0]->v * XTable[0]->rhw ) - tvPX * px0 - tvPY * py0 ;
			rhw0 = XTable[0]->rhw   - rhwPX * px0 - rhwPY * py0 ;
			z0   = XTable[0]->pos.z - zPX   * px0 - zPY   * py0 ;
			r0   = ((BYTE *)&XTable[0]->color)[2] - rPX * px0 - rPY * py0 ;
			g0   = ((BYTE *)&XTable[0]->color)[1] - gPX * px0 - gPY * py0 ;
			b0   = ((BYTE *)&XTable[0]->color)[0] - bPX * px0 - bPY * py0 ;
			a0   = ((BYTE *)&XTable[0]->color)[3] - aPX * px0 - aPY * py0 ;
		}

		// イレギュラー用処理
		if( _FTOL( YTable[0]->pos.y ) == _FTOL( YTable[1]->pos.y ) )
		{
			// 上部２頂点のＹ成分が同じ場合の処理
			DMODEL = DMODER = 1 ;

			// 上２頂点のどちらが左側でどちらが右側か調べる
			if( YTable[0]->pos.x > YTable[1]->pos.x )
			{
				LP = YTable[1] ;
				RP = YTable[0] ;
			}
			else
			{
				LP = YTable[0] ;
				RP = YTable[1] ;
			}

			// 左右の頂点の開始座標をセット
			StartPLX[1] = LP->pos.x ; StartPLY[1] = LP->pos.y ;
			StartPRX[1] = RP->pos.x ; StartPRY[1] = RP->pos.y ;

			// 左右の頂点の１ラインごとのＸ座標の加算値を算出
			AddPLX[1] = ( YTable[2]->pos.x - StartPLX[1] ) / ( YTable[2]->pos.y - StartPLY[1] ) ;
			AddPRX[1] = ( YTable[2]->pos.x - StartPRX[1] ) / ( YTable[2]->pos.y - StartPRY[1] ) ;
		}
		else
		// 下部２頂点のＹ成分が同じ場合の処理
		if( _FTOL( YTable[1]->pos.y ) == _FTOL( YTable[2]->pos.y ) )
		{
			DMODEL = DMODER = 1 ;

			// 処理開始座標は左右とも同じになる
			StartPLX[1] = StartPRX[1] = YTable[0]->pos.x ;
			StartPLY[1] = StartPRY[1] = YTable[0]->pos.y ;

			// 下側の２頂点がどちらが左側でどちらが右側か調べる
			if( YTable[1]->pos.x > YTable[2]->pos.x )
			{
				LP = YTable[2] ;
				RP = YTable[1] ;
			}
			else
			{
				LP = YTable[1] ;
				RP = YTable[2] ;
			}

			// 左右の頂点の１ラインごとのＸ座標の加算値を算出
			AddPLX[1] = ( LP->pos.x - StartPLX[1] ) / ( LP->pos.y - StartPLY[1] ) ;
			AddPRX[1] = ( RP->pos.x - StartPRX[1] ) / ( RP->pos.y - StartPRY[1] ) ;
		}
		else
		// 特殊なケースではない場合
		{
			// 途中に中継点を通るのは左側か、右側かによって処理を分岐
			if( ( YTable[1]->pos.x - YTable[0]->pos.x ) / ( YTable[1]->pos.y - YTable[0]->pos.y ) <
				( YTable[2]->pos.x - YTable[0]->pos.x ) / ( YTable[2]->pos.y - YTable[0]->pos.y ) ) 
			{
				// 中継点を通るのは左側
				DMODEL = 0 ;
				DMODER = 1 ;

				// 左右の開始点はどちらも一番上の頂点
				StartPLX[0] = StartPRX[1] = YTable[0]->pos.x ;
				StartPLY[0] = StartPRY[1] = YTable[0]->pos.y ;

				// 左側の線の１ラインごとに加算するＸ座標値を算出
				AddPLX[0] = ( YTable[1]->pos.x - StartPLX[0] ) / ( YTable[1]->pos.y - StartPLY[0] ) ;

				// 右側の腺の１ラインごとに加算するＸ座標値を算出
				AddPRX[1] = ( YTable[2]->pos.x - StartPRX[1] ) / ( YTable[2]->pos.y - StartPRY[1] ) ;

				
				// 左側だけ中継点を通るので、中継点から終点に向かう時の開始点をセット
				StartPLX[1] = YTable[1]->pos.x ;
				StartPLY[1] = YTable[1]->pos.y ;

				// 左側の中継点から終点へ向かう時の１ラインごとに加算するＸ座標値を算出
				AddPLX[1] = ( YTable[2]->pos.x - StartPLX[1] ) / ( YTable[2]->pos.y - StartPLY[1] ) ;
			}
			else
			{
				// 中継点を通るのは右側
				DMODEL = 1 ;
				DMODER = 0 ;

				// 左右の開始点はどちらも一番上の頂点
				StartPLX[1] = StartPRX[0] = YTable[0]->pos.x ;
				StartPLY[1] = StartPRY[0] = YTable[0]->pos.y ;

				// 右側の腺の１ラインごとに加算するＸ座標値を算出
				AddPRX[0] = ( YTable[1]->pos.x - StartPRX[0] ) / ( YTable[1]->pos.y - StartPRY[0] ) ;

				// 左側の線の１ラインごとに加算するＸ座標値を算出
				AddPLX[1] = ( YTable[2]->pos.x - StartPLX[1] ) / ( YTable[2]->pos.y - StartPLY[1] ) ;

				
				// 右側だけ中継点を通るので、中継点から終点に向かう時の開始点をセット
				StartPRX[1] = YTable[1]->pos.x + 1 ;
				StartPRY[1] = YTable[1]->pos.y ;

				// 右側の中継点から終点へ向かう時の１ラインごとに加算するＸ座標値を算出
				AddPRX[1] = ( YTable[2]->pos.x - StartPRX[1] ) / ( YTable[2]->pos.y - StartPRY[1] ) ;
			}
		}
		
		// 描画処理を開始するＹ座標と、各ラインでの描画開始Ｘ座標、描画ドット数を算出
		{
			int y, lx, rx, addLX, addRX, endY, cly, cry ;
			
			// 描画開始Ｙ座標を算出
			StartY = ( _FTOL( YTable[0]->pos.y * QUALI ) + QUALI / 2 ) / QUALI ;
			endY   = _FTOL( YTable[2]->pos.y * QUALI ) ;

			// 画面からはみ出していたら補正
			if( StartY < MemImgManage.DrawArea.top ) StartY = MemImgManage.DrawArea.top ;
			if( endY > MemImgManage.DrawArea.bottom * QUALI ) endY = MemImgManage.DrawArea.bottom * QUALI ;

			// 初期Ｙ座標のセット
			y      = StartY * QUALI + QUALI / 2 ;

			// 左右のラインの初期座標と加算座標をセット
			lx     = _FTOL( ( StartPLX[DMODEL] + ( ( (float)StartY + 0.5F ) - StartPLY[DMODEL] ) * AddPLX[DMODEL] ) * QUALI ) ;
			rx     = _FTOL( ( StartPRX[DMODER] + ( ( (float)StartY + 0.5F ) - StartPLY[DMODEL] ) * AddPRX[DMODER] ) * QUALI ) ;
			addLX  = _FTOL( AddPLX[DMODEL]    * QUALI ) ;
			addRX  = _FTOL( AddPRX[DMODER]    * QUALI ) ;

			// 中継点値をセット( 次の中継点は終点の場合は必ず到達しない値(4096 * QUALI)をセットしておく )
			cly = ( DMODEL == 0 ) ? _FTOL( StartPLY[1] * QUALI ) : 4096 * QUALI ;
			cry = ( DMODER == 0 ) ? _FTOL( StartPRY[1] * QUALI ) : 4096 * QUALI ;

			NumY = 0 ;
			while( endY > y )
			{
				// 描画開始Ｘ座標と、描画ドット数を算出
				StartX_NumX[NumY][0] = (short)( ( lx + QUALI / 2 ) / QUALI ) ;
				StartX_NumX[NumY][1] = (short)( ( rx - QUALI / 2 ) / QUALI - StartX_NumX[NumY][0] + 1 ) ;

				// 画面からはみ出していたら補正
				if( StartX_NumX[NumY][0] < MemImgManage.DrawArea.left )
				{
					StartX_NumX[NumY][1] -= (short)( MemImgManage.DrawArea.left - StartX_NumX[NumY][0] ) ;
					StartX_NumX[NumY][0] = (short)( MemImgManage.DrawArea.left ) ;
				}
				if( StartX_NumX[NumY][0] + StartX_NumX[NumY][1] > MemImgManage.DrawArea.right )
				{
					StartX_NumX[NumY][1] = (short)( MemImgManage.DrawArea.right - StartX_NumX[NumY][0] ) ;
				}
				
				// もし描画するドット数が０個だった場合はキャンセル
				if( StartX_NumX[NumY][1] <= 0 ) StartX_NumX[NumY][1] = 0 ;
				NumY ++ ;
				
				rx += addRX ;
				lx += addLX ;

				y += QUALI ;

				// もし左側のラインが中継点を超えていたら走査するラインを変更する
				if( cly <= y )
				{
					// 左右のラインの初期座標と加算座標をセット
					lx     = _FTOL( ( StartPLX[1] + ( (float)y / QUALI - StartPLY[1] ) * AddPLX[1] ) * QUALI ) ;
					addLX  = _FTOL( AddPLX[1] * QUALI ) ;

					// 次の中継点は無いので必ず到達しない値(4096 * QUALI)をセットしておく
					cly = 4096 * QUALI ;
				}
				
				// もし右側のラインが中継点を超えていたら走査するラインを変更する
				if( cry <= y )
				{
					// 左右のラインの初期座標と加算座標をセット
					rx     = _FTOL( ( StartPRX[1] + ( (float)y / QUALI - StartPRY[1] ) * AddPRX[1] ) * QUALI ) ;
					addRX  = _FTOL( AddPRX[1] * QUALI ) ;

					// 次の中継点は無いので必ず到達しない値(4096 * QUALI)をセットしておく
					cry = 4096 * QUALI ;
				}
			}
		}
	}

	// 転送元、転送先のアドレスをセット
	if( SrcImg != NULL )
	{
		SrcBP     = SrcImg->UseImage ;
		SrcPitch  = SrcImg->Base->Pitch ;
	}

	DestBP    = DestImg->UseImage ;
	DestPitch = DestImg->Base->Pitch ;
	
	if( ZImg != NULL )
	{
		ZBP        = ZImg->UseImage ;
		ZPitch     = ZImg->Base->Pitch ;
	}

	if( BlendImg != NULL )
	{
		BlendBP    = BlendImg->UseImage ;
		BlendPitch = BlendImg->Base->Pitch ;
	}

	PaletteBP  = (BYTE *)SrcImg->Base->Palette ;
	TransColor = SrcImg->Base->TransColor ;
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;
	RedTable   = MemImgManage.RateTable[MemImgManage.DrawBright.Red] ;
	GreenTable = MemImgManage.RateTable[MemImgManage.DrawBright.Green] ;
	BlueTable  = MemImgManage.RateTable[MemImgManage.DrawBright.Blue] ;

	// 描画準備
	{
		DrawRectRight  = MemImgManage.DrawArea.right ;
		DrawRectLeft   = MemImgManage.DrawArea.left ;
		DrawRectBottom = MemImgManage.DrawArea.bottom ;
		DrawRectTop    = MemImgManage.DrawArea.top ;

		RHWPX = _DTOL( (double)rhwPX * RHWQUALI ) ;
		RHWPY = _DTOL( (double)rhwPY * RHWQUALI ) ;
		RHW0  = _DTOL( (double)rhw0  * RHWQUALI ) ;

		UPX = _DTOL( (double)tuPX * UVQUALI ) ;
		UPY = _DTOL( (double)tuPY * UVQUALI ) ;
		U0  = _DTOL( (double)tu0  * UVQUALI ) ;

		VPX = _DTOL( (double)tvPX * UVQUALI ) ;
		VPY = _DTOL( (double)tvPY * UVQUALI ) ;
		V0  = _DTOL( (double)tv0  * UVQUALI ) ;
		
		ZPX = _DTOL( (double)zPX * ZQUALI ) ;
		ZPY = _DTOL( (double)zPY * ZQUALI ) ;
		Z0  = _DTOL( (double)z0  * ZQUALI ) ;

		RPX = _DTOL( (double)rPX * COLORQUALI ) ;
		RPY = _DTOL( (double)rPY * COLORQUALI ) ;
		R0  = _DTOL( (double)r0  * COLORQUALI ) ;

		GPX = _DTOL( (double)gPX * COLORQUALI ) ;
		GPY = _DTOL( (double)gPY * COLORQUALI ) ;
		G0  = _DTOL( (double)g0  * COLORQUALI ) ;

		BPX = _DTOL( (double)bPX * COLORQUALI ) ;
		BPY = _DTOL( (double)bPY * COLORQUALI ) ;
		B0  = _DTOL( (double)b0  * COLORQUALI ) ;

		APX = _DTOL( (double)aPX * COLORQUALI ) ;
		APY = _DTOL( (double)aPY * COLORQUALI ) ;
		A0  = _DTOL( (double)a0  * COLORQUALI ) ;

		RHWT = RHW0 + RHWPY * StartY + RHWPY / 2 ;
		UT = U0 + UPY * StartY + UPY / 2 ;
		VT = V0 + VPY * StartY + VPY / 2 ;
		ZT = Z0 + ZPY * StartY + ZPY / 2 ;
		RT = R0 + RPY * StartY + RPY / 2 ;
		GT = G0 + GPY * StartY + GPY / 2 ;
		BT = B0 + BPY * StartY + BPY / 2 ;
		AT = A0 + APY * StartY + APY / 2 ;
		ZBufPT = ZBP    + StartY * ZPitch ;
		DestPT = DestBP + StartY * DestPitch ;

		texWidth  = ( int )SrcImg->Width  ;
		texHeight = ( int )SrcImg->Height ;
	}
	
	// グローシェーディングの場合とそうでない場合で処理を分岐
	if( GouraudShadeMode == FALSE )
	{
		// グローシェーディングではない場合

		// カラーモードによって処理を分岐
		switch( SrcImg->Base->ColorType )
		{
		case 0 :	// 16bit モード
			if( SrcImg->Base->UsePalette == 1 )
			// パレットがある場合
			{
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
	NOMALDRAW_C16_USEPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
				}
			}
			else
			// パレットが無い場合
			{
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
	NOMALDRAW_C16_NOPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2, 2 )
/*								else
								{
									y = StartY ;
									ycon = 0 ;
									for( ; ycon < NumY ; ycon ++, y ++,
										 RHWT += RHWPY,
										 UT += UPY,
										 VT += VPY,
										 ZT += ZPY,
										 RT += RPY,
										 GT += GPY,
										 BT += BPY,
										 AT += APY,
										 ZBufPT += ZPitch,
										 DestPT += DestPitch
									 )
									{
										xcon = StartX_NumX[ycon][1] ;
										if( xcon == 0 ) continue ;
										x = StartX_NumX[ycon][0] ;

										RHW = RHWPX * x + ( RHWPX >> 1 ) + RHWT ;
										Z   = ZPX   * x + ( ZPX   >> 1 ) + ZT ;
										U   = UPX   * x + ( UPX   >> 1 ) + UT ;
										V   = VPX   * x + ( VPX   >> 1 ) + VT ;
										R   = RPX   * x + ( RPX   >> 1 ) + RT ;
										G   = GPX   * x + ( GPX   >> 1 ) + GT ;
										B   = BPX   * x + ( BPX   >> 1 ) + BT ;
										A   = APX   * x + ( APX   >> 1 ) + AT ;
										ZBPT = x * 2 + ZBufPT ;
										DestBPT = x * 2 + DestPT ;
										if( RHW == 0 )
											continue ;

										for( ; xcon ; xcon --,
										 RHW += RHWPX,
										 U += UPX,
										 V += VPX,
										 Z += ZPX,
										 R += RPX,
										 G += GPX,
										 B += BPX,
										 A += APX,
										 ZBPT += 2,
										 DestBPT += 2
										  )
										{
											if( *ZWPT >= (WORD)( Z >> 8 ) )
											{
												*ZWPT = Z >> 8 ;

												INV = (1 << 29) / RHW ;
												tu = ( ( U >> 10 ) * texWidth  * INV ) >> 19 ;
												tv = ( ( V >> 10 ) * texHeight * INV ) >> 19 ;

												if( (DWORD)( ( tu | ( texWidth - tu - 1 ) ) | ( tv | ( texHeight - tv - 1 ) ) ) & 0x80000000 )
													continue ;

												SrcBPT  = SrcBP + tu * 2 + tv * SrcPitch ;

												CODE_NBI_C16_UBR_NAC_BNO_NTBL( SRC16, DST16, DSTP16, (R / COLORQUALI), (G / COLORQUALI), (B / COLORQUALI) )
											}
										}
									}
								}*/
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;
				}
			}
			break ;
			
		case 1 :	// 32bit モード
			if( SrcImg->Base->UsePalette == 1 )
			// パレットがある場合
			{
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
	NOMALDRAW_C32_USEPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
				}
			}
			else
			// パレットが無い場合
			{
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
	NOMALDRAW_C32_NOPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;
				}
			}
			break ;
		}
	}
	else
	{
		// グローシェーディングの場合

		// カラーモードによって処理を分岐
		switch( SrcImg->Base->ColorType )
		{
		case 0 :	// 16bit モード
			if( SrcImg->Base->UsePalette == 1 )
			// パレットがある場合
			{
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BNO_NTBL( PAL16, DST16, DSTP16, BLND, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BNO_NTBL( PAL16, DST16, DSTP16, BLND, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BNO_NTBL( PAL16, DST16, DSTP16, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BNO_NTBL( PAL16, DST16, DSTP16, RB, GB, BB ), 1, 2 )
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAL_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAL_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAL_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAL_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAD_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAD_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAD_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAD_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BSB_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BSB_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BSB_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BSB_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BML_NTBL( PAL16, DST16, DSTP16, BLND, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BML_NTBL( PAL16, DST16, DSTP16, BLND, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BML_NTBL( PAL16, DST16, DSTP16, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BML_NTBL( PAL16, DST16, DSTP16, RB, GB, BB ), 1, 2 )
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BMA_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BMA_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BMA_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BMA_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BIS_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BIS_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BIS_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BIS_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;
				}
			}
			else
			// パレットが無い場合
			{
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_TCK_NTBL( SRC16, DST16, DSTP16, BLND, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_NTBL(     SRC16, DST16, DSTP16, BLND, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_NTBL(     SRC16, SRCA16, DST16, DSTP16, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_TCK_NTBL( SRC16, DST16, DSTP16, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_NTBL(     SRC16, DST16, DSTP16, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_TCK_NTBL( SRC16, DST16, DSTP16, BLND, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_NTBL(     SRC16, DST16, DSTP16, BLND, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_NTBL(     SRC16, SRCA16, DST16, DSTP16, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_TCK_NTBL( SRC16, DST16, DSTP16, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_NTBL(     SRC16, DST16, DSTP16, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;
				}
			}
			break ;
			
		case 1 :	// 32bit モード
			if( SrcImg->Base->UsePalette == 1 )
			// パレットがある場合
			{
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BNO_NTBL( PALP32, DSTP32, BLND, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BNO_NTBL( PALP32, DSTP32, BLND, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BNO_NTBL( PALP32, DSTP32, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BNO_NTBL( PALP32, DSTP32, RB, GB, BB ), 1, 4 )
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAL_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAL_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAL_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAL_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAD_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAD_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAD_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAD_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BSB_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BSB_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BSB_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BSB_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BML_NTBL( PALP32, DSTP32, BLND, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BML_NTBL( PALP32, DSTP32, BLND, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BML_NTBL( PALP32, DSTP32, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BML_NTBL( PALP32, DSTP32, RB, GB, BB ), 1, 4 )
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BMA_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BMA_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BMA_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BMA_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BIS_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BIS_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BIS_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BIS_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;
				}
			}
			else
			// パレットが無い場合
			{
				// ブレンドモードによって処理を分岐
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// ブレンド無し
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_ACK_NTBL( SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_NTBL(     SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_TCK_NTBL( SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_NTBL(     SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_ACK_NTBL( SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_NTBL(     SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_TCK_NTBL( SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_NTBL(     SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_ACK_NTBL( SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_NTBL(     SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_TCK_NTBL( SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_NTBL(     SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_ACK_NTBL( SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_NTBL(     SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_TCK_NTBL( SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_NTBL(     SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;
				}
			}
			break ;
		}
	}

END:
	if( UseTempMemImg )
		TerminateMemImg( &TempMemImg );

/*	else
	// テクスチャが無い場合
	{
		// 描画準備
		if( SrcImg->Base->ColorType == 0 )
		{
			ColorD = RUP16( RT( RBOT16( Color ) ) ) |
				     GUP16( GT( GBOT16( Color ) ) ) |
				   	 BUP16( BT( BBOT16( Color ) ) ) ;
		}
		else
		{
			ColorD = Color ;
			ColorB[0] = (BYTE)RT( ColorB[0] ) ;
			ColorB[1] = (BYTE)GT( ColorB[1] ) ;
			ColorB[2] = (BYTE)BT( ColorB[2] ) ;
		}
		SrcBPT = ColorB ;

		// カラーモードによって処理を分岐
		switch( SrcImg->Base->ColorType )
		{
		case 0 :	// 16bit モード
			// ブレンドモードによって処理を分岐
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// ブレンド無し
NOTEX_NOMALDRAW_C16_NOPAL_BNO:
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C16_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
				break ;
			}
			break ;
			
		case 1 :	// 32bit モード
			// ブレンドモードによって処理を分岐
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// ブレンド無し
NOTEX_NOMALDRAW_C32_NOPAL_BNO:
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C32_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
				break ;
			}
			break ;
		}
	}
*/
	// 終了
	return ;
}
#undef PAL16
#undef DST16
#undef SRC16
#undef SRCA16
#undef DSTP16

#undef PALP32
#undef SRCP32
#undef DSTP32

#undef BLND

#endif

#endif // DX_NON_GRAPHICS

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

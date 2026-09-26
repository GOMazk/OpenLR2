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

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// データ型定義----------------------------------------------------------------

// データ宣言------------------------------------------------------------------

// 関数プロトタイプ宣言--------------------------------------------------------

// プログラム------------------------------------------------------------------

#ifdef DX_NON_2DDRAW

extern void DrawRotationMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, int x, int y, float Angle, float EnlageRateX, float EnlageRateY, int TransFlag, int ReverseXFlag, int ReverseYFlag, const MEMIMG *BlendImg )
{
	return ;
}

extern void DrawTransformMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const POINT *pos4, int TransFlag, const MEMIMG *BlendImg )
{
	return ;
}

extern void DrawBasicPolygonMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const DX_POINTDATA *pos3, int TransFlag, const MEMIMG *BlendImg, unsigned int Color )
{
	return ;
}

#else











/*
#define DRAWROTATIONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	do{\
		GSX = GDX2 ;\
		GSY = GDY2 ;\
		do{\
			if( (DWORD)GDX2 < (DWORD)SizeX && (DWORD)GDY2 < (DWORD)SizeY ){\
				SrcBPT   = SrcBP   + ( GDX2 / QUALI ) * (SRCADDNUM) + ( GDY2 / QUALI ) * SrcPitch   ;\
				BlendBPT = BlendBP + ( GDX2 / QUALI )               + ( GDY2 / QUALI ) * BlendPitch ;\
				DRAW\
			}\
			GDX2 += GAADX ;\
			GDY2 += GAADY ;\
			DestBPT += (DESTADDNUM) ;\
		}while( -- C ) ;\
		C = EndC ;\
		GDX2 = GSX += GLADDX ;\
		GDY2 = GSY += GLADDY ;\
		DestBPT = DestBP += DestPitch ;\
	}while( -- C2 ) ;\
}
*/
#define DRAWROTATIONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	do{\
		do{\
			if( (DWORD)GDX2 < (DWORD)SizeX && (DWORD)GDY2 < (DWORD)SizeY ){\
				SrcBPT   = (BYTE *)SrcLineP[ GDY2 / QUALI ] + ( GDX2 / QUALI ) * (SRCADDNUM) ;\
				BlendBPT = (BYTE *)BlendLineP[ GDY2 / QUALI ] + ( GDX2 / QUALI ) ;\
				DRAW\
			}\
			GDX2 += GAADX ;\
			GDY2 += GAADY ;\
			DestBPT += (DESTADDNUM) ;\
		}while( -- C ) ;\
		C = EndC ;\
		GDX2 += GLADDX ;\
		GDY2 += GLADDY ;\
		DestBPT += DestAddPitch ;\
	}while( -- C2 ) ;\
}

/*
#define DRAWROTATIONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	do{\
		GSX = GDX2 ;\
		GSY = GDY2 ;\
		do{\
			if( (DWORD)GDX2 < (DWORD)SizeX && (DWORD)GDY2 < (DWORD)SizeY ){\
				SrcBPT   = SrcBP   + ( GDX2 / QUALI ) * (SRCADDNUM) + ( GDY2 / QUALI ) * SrcPitch   ;\
				DRAW\
			}\
			GDX2 += GAADX ;\
			GDY2 += GAADY ;\
			DestBPT += (DESTADDNUM) ;\
		}while( -- C ) ;\
		C = EndC ;\
		GDX2 = GSX += GLADDX ;\
		GDY2 = GSY += GLADDY ;\
		DestBPT = DestBP += DestPitch ;\
	}while( -- C2 ) ;\
}
*/
#define DRAWROTATIONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	do{\
		do{\
			if( (DWORD)GDX2 < (DWORD)SizeX && (DWORD)GDY2 < (DWORD)SizeY ){\
				SrcBPT   = (BYTE *)SrcLineP[ GDY2 / QUALI ] + ( GDX2 / QUALI ) * (SRCADDNUM) ;\
				DRAW\
			}\
			GDX2 += GAADX ;\
			GDY2 += GAADY ;\
			DestBPT += (DESTADDNUM) ;\
		}while( -- C ) ;\
		C = EndC ;\
		GDX2 += GLADDX ;\
		GDY2 += GLADDY ;\
		DestBPT += DestAddPitch ;\
	}while( -- C2 ) ;\
}

#define PAL16	PaletteWP[ *SrcBPT ]
#define DST16	*DestWPT
#define SRC16	*SrcWPT
#define SRCA16	SrcBPT[2]
#define DSTP16	DestWPT

#define PALP32	(BYTE *)&PaletteDP[ *SrcBPT ]
#define SRCP32	SrcBPT
#define DSTP32	DestBPT

#define BLND	*BlendBPT
// イメージを回転描画する
extern void DrawRotationMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, int x, int y, float Angle, float EnlageRateX, float EnlageRateY, int TransFlag, int ReverseXFlag, int ReverseYFlag, const MEMIMG *BlendImg )
{
	RECT DrawRect ;
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD_PTR DestPP, PalettePP ;
	DWORD Src1, Src2, Data, DataD ;
	DWORD *RedTable , *GreenTable , *BlueTable ;
	DWORD SrcPitch, DestPitch, BlendPitch = 0, DestAddPitch ;
	DWORD TransColor ;
	int BD1X , BD1Y ;						// 描画中にカウンタとして使うベクトル変数(描画先用) 
	int GDX , GDY ;							// 描画中にカウンタとして使うベクトル変数(描画元用)
	int GLADDX , GLADDY  ;					// 使用中の描画元参照用加算ベクトル
	int GAADX , GAADY ;						// グラフィック内、１描画ごとの加算値
	int MaxY , MaxX ;
	int C, EndC, C2, EndC2 ;
	int GDX2, GDY2 ;
	int SizeX, SizeY ;
	int GLADDX2, GLADDY2 ;
	int GAADY2, GAADX2 ;
	static DWORD PaletteTemp[256] ;
	static DWORD_PTR SrcLineP[4096], BlendLineP[4096] ;

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

	// カラータイプが違うか、描画先がαチャンネル付かパレット付きだった場合はエラー
	if( SrcImg->Base->ColorType != DestImg->Base->ColorType || DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;
	
	// スキャンラインアルゴリズム(もどき)
	{
		int px[4] , py[4] ;
		int Cos , Sin ;
		int SinD , CosD ;
		float SinF, CosF ;

		_SINCOS_PLATFORM( -Angle, &SinF, &CosF ) ;
		SinD = _FTOL( SinF * QUALI ) ;
		CosD = _FTOL( CosF * QUALI ) ;

		// 回転用Ｓｉｎ、Ｃｏｓ値を得る
		_SINCOS_PLATFORM( (float)Angle, &SinF, &CosF ) ;
		Sin = _FTOL( SinF * QUALI ) ;
		Cos = _FTOL( CosF * QUALI ) ;

		// 描画座標を得る
		{
			int xt , yt ;
			float SizeXF, SizeYF ;
			float SizeXFCos, SizeXFSin, SizeYFCos, SizeYFSin ;

			xt = x * QUALI ;
			yt = y * QUALI ;
			SizeXF = SrcImg->Width  * EnlageRateX / 2 ;
			SizeYF = SrcImg->Height * EnlageRateY / 2 ;
			SizeXFCos = SizeXF * Cos ;
			SizeXFSin = SizeXF * Sin ;
			SizeYFCos = SizeYF * Cos ;
			SizeYFSin = SizeYF * Sin ;

			px[0] = _FTOL( -SizeXFCos - (-SizeYFSin) ) + xt ;
			py[0] = _FTOL( -SizeXFSin + (-SizeYFCos) ) + yt ;

			px[1] = _FTOL( -SizeXFCos -   SizeYFSin  ) + xt ;
			py[1] = _FTOL( -SizeXFSin +   SizeYFCos  ) + yt ;

			px[2] = _FTOL(  SizeXFCos -   SizeYFSin  ) + xt ;
			py[2] = _FTOL(  SizeXFSin +   SizeYFCos  ) + yt ;

			px[3] = _FTOL(  SizeXFCos - (-SizeYFSin) ) + xt ;
			py[3] = _FTOL(  SizeXFSin + (-SizeYFCos) ) + yt ;

			// 反転フラグがたっていたら DrawTransformMemImg 関数にまかせる
			if( ReverseXFlag && ReverseYFlag )
			{
				POINT point[4] ;

				point[0].x = px[2] / QUALI ;	point[0].y = py[2] / QUALI ;
				point[1].x = px[1] / QUALI ;	point[1].y = py[1] / QUALI ;
				point[2].x = px[3] / QUALI ;	point[2].y = py[3] / QUALI ;
				point[3].x = px[0] / QUALI ;	point[3].y = py[0] / QUALI ;
				DrawTransformMemImg( DestImg, SrcImg, point, TransFlag, BlendImg ) ;
				return ;
			}
			else
			if( ReverseXFlag )
			{
				POINT point[4] ;

				point[0].x = px[3] / QUALI ;	point[0].y = py[3] / QUALI ;
				point[1].x = px[0] / QUALI ;	point[1].y = py[0] / QUALI ;
				point[2].x = px[2] / QUALI ;	point[2].y = py[2] / QUALI ;
				point[3].x = px[1] / QUALI ;	point[3].y = py[1] / QUALI ;
				DrawTransformMemImg( DestImg, SrcImg, point, TransFlag, BlendImg ) ;
				return ;
			}
			else
			if( ReverseYFlag )
			{
				POINT point[4] ;

				point[0].x = px[1] / QUALI ;	point[0].y = py[1] / QUALI ;
				point[1].x = px[2] / QUALI ;	point[1].y = py[2] / QUALI ;
				point[2].x = px[0] / QUALI ;	point[2].y = py[0] / QUALI ;
				point[3].x = px[3] / QUALI ;	point[3].y = py[3] / QUALI ;
				DrawTransformMemImg( DestImg, SrcImg, point, TransFlag, BlendImg ) ;
				return ;
			}

			GAADX = _FTOL( Cos  / EnlageRateX ) ;
			GAADY = _FTOL( SinD / EnlageRateY ) ;
		}

		// 一番上に来ている座標によって描画起点座標等を決定
		{
			int i , MaxY2 , MinY2 , MaxX2 , MinX2 ;

			MaxY2 = 0 ; MinY2 = 0 ;
			MaxX2 = 0 ; MinX2 = 0 ; 
			for( i = 1 ; i < 4 ; i ++ )
			{
				if( py[MaxY2] < py[i] ) MaxY2 = i ;
				if( py[MinY2] > py[i] ) MinY2 = i ;

				if( px[MaxX2] < px[i] ) MaxX2 = i ;
				if( px[MinX2] > px[i] ) MinX2 = i ;
			}


			BD1X = (int)( px[MinX2] / QUALI ) - 2 ; 
			BD1Y = (int)( py[MinY2] / QUALI ) - 2 ; 

			MaxX = (int)( px[MaxX2] / QUALI ) + 4 ;
			MaxY = (int)( py[MaxY2] / QUALI ) + 4 ;

			{
				DrawRect.left   = BD1X ;
				DrawRect.top    = BD1Y ;
				DrawRect.right  = MaxX ;
				DrawRect.bottom = MaxY ;
				RectClipping_Inline( &DrawRect , &MemImgManage.DrawArea ) ; 
				if( DrawRect.left == DrawRect.right || DrawRect.top == DrawRect.bottom ) return ;

				BD1X = DrawRect.left ;  BD1Y = DrawRect.top ;
				MaxX = DrawRect.right ; MaxY = DrawRect.bottom ;

				py[MaxY2] = MaxY * QUALI ;
				py[MinY2] = BD1Y * QUALI ;
			}

			{
				int xt , yt, h ;
				int GX1 , GX2 , GY1 , GY2 ;

				xt = (int)( BD1X - x ) ;
				yt = (int)( BD1Y - y ) ;

				GX1 = ( int )( _FTOL( ( CosD * xt - SinD * yt ) / EnlageRateX ) + SrcImg->Width  / 2 * QUALI ) ;
				GY1 = ( int )( _FTOL( ( SinD * xt + CosD * yt ) / EnlageRateY ) + SrcImg->Height / 2 * QUALI ) ;

				xt = (int)( BD1X - x ) ;
				yt = (int)( (int)( py[MaxY2] / QUALI ) - y ) ; 

				GX2 = ( int )( _FTOL( ( CosD * xt - SinD * yt ) / EnlageRateX ) + SrcImg->Width  / 2 * QUALI ) ;
				GY2 = ( int )( _FTOL( ( SinD * xt + CosD * yt ) / EnlageRateY ) + SrcImg->Height / 2 * QUALI ) ;

				h = (int)( py[MaxY2] - py[MinY2] ) / QUALI ;
				if( h == 0 ) return ;
				GLADDX = ( GX2 - GX1 ) / h ;
				GLADDY = ( GY2 - GY1 ) / h ;

				GDX = GX1 ;
				GDY = GY1 ;
			}
		}
	}
		
	// 転送元、転送先のアドレスをセット
	SrcBP        = SrcImg->UseImage ;
	SrcPitch     = SrcImg->Base->Pitch ;

	DestBP       = DestImg->UseImage + BD1X * DestImg->Base->ColorDataP->PixelByte + BD1Y * DestImg->Base->Pitch ;
	DestPitch    = DestImg->Base->Pitch ;
	DestPP       = (DWORD_PTR)DestBP ;

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
	PalettePP  = (DWORD_PTR)PaletteBP ;

	// 転送元の各ラインの先頭アドレスを算出
	{
		DWORD_PTR addres, h, i ;

		h  = SrcImg->Height ;
		addres = (DWORD_PTR)SrcBP ;
		for( i = 0 ; i < h ; i ++, addres += SrcPitch )
			SrcLineP[i] = (DWORD_PTR)addres ;

		if( BlendImg != NULL )
		{
			h  = BlendImg->Height ;
			addres = (DWORD_PTR)BlendBP ;
			for( i = 0 ; i < h ; i ++, addres += BlendPitch )
				BlendLineP[i] = (DWORD_PTR)addres ;
		}
	}

	// パレットのコピー
	if( SrcImg->Base->UsePalette == 1 )
	{
		_MEMCPY( PaletteTemp, SrcImg->Base->Palette, ( size_t )( SrcImg->Base->ColorNum * ( SrcImg->Base->ColorType == 0 ? 2 : 4 ) ) ) ;
	}
	
	// 描画準備
	{
		GLADDX2 = GLADDX ;
		GLADDY2 = GLADDY ;
		GAADY2 = GAADY ;
		GAADX2 = GAADX ;

		SizeX = ( int )( SrcImg->Width  * QUALI ) ;
		SizeY = ( int )( SrcImg->Height * QUALI ) ;
			
		EndC = MaxX - BD1X ;
		C = EndC ;

		EndC2 = MaxY - BD1Y ;
		C2 = EndC2 ;

		GDX2 = GDX ;
		GDY2 = GDY ;

		DestBPT = DestBP ;

		GLADDX -= EndC * GAADX ;
		GLADDY -= EndC * GAADY ;
		DestAddPitch = DestPitch - EndC * DestImg->Base->ColorDataP->PixelByte ;
	}

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
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )
						{
#ifdef DX_NON_INLINE_ASM
							DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, GDX2
								MOV EDX, GDY2
							LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_1:
								MOV ECX, EndC
							LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_2:
								CMP ESI, SizeX
								JAE LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3
								CMP EDX, SizeY
								JAE LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3

								MOV EBX, EDX
								MOV EAX, ESI
								SHR EBX, 18
								SHR EAX, 18
								MOV EBX, [SrcLineP + EBX * 4]
								MOVZX EBX, byte ptr [EAX + EBX]
								CMP EBX, TransColor
								JE LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3
								MOV AX, word ptr [PaletteTemp + EBX * 2]
								MOV [EDI], AX

							LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3:
								ADD ESI, GAADX
								ADD EDX, GAADY
								ADD EDI, 2
								DEC ECX
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_2

								ADD ESI, GLADDX
								ADD EDX, GLADDY
								ADD EDI, DestAddPitch
								DEC C2
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_1
							}
#endif
						}
						else
						{
#ifdef DX_NON_INLINE_ASM
							DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, GDX2
								MOV EDX, GDY2
							LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_1:
								MOV ECX, EndC
							LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_2:
								CMP ESI, SizeX
								JAE LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_3
								CMP EDX, SizeY
								JAE LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_3

								MOV EBX, EDX
								MOV EAX, ESI
								SHR EBX, 18
								SHR EAX, 18
								MOV EBX, [SrcLineP + EBX * 4]
								MOVZX EBX, byte ptr [EAX + EBX]
								MOV AX, word ptr [PaletteTemp + EBX * 2]
								MOV [EDI], AX

							LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_3:
								ADD ESI, GAADX
								ADD EDX, GAADY
								ADD EDI, 2
								DEC ECX
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_2

								ADD ESI, GLADDX
								ADD EDX, GLADDY
								ADD EDI, DestAddPitch
								DEC C2
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_1
							}
#endif
						}
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_USEPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ADD :			// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;

			case DX_BLENDMODE_SUB :			// 減算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;
				
			case DX_BLENDMODE_MUL :			// 乗算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;

			case DX_BLENDMODE_MULA :		// 乗算+αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
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
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )
							{
#ifdef DX_NON_INLINE_ASM
								DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, GDX2
									MOV EDX, GDY2
								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_1:
									MOV ECX, EndC
								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_2:
									CMP ESI, SizeX
									JAE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_3
									CMP EDX, SizeY
									JAE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_3

									MOV EBX, EDX
									MOV EAX, ESI
									SHR EBX, 18
									SHR EAX, 17
									MOV EBX, [SrcLineP + EBX * 4]
									AND EAX, 0xfffffffe
									MOVZX EBX, word ptr [EAX + EBX]
									CMP EBX, TransColor
									JE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_3
									MOV [EDI], BX

								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_3:
									ADD ESI, GAADX
									ADD EDX, GAADY
									ADD EDI, 2
									DEC ECX
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_2

									ADD ESI, GLADDX
									ADD EDX, GLADDY
									ADD EDI, DestAddPitch
									DEC C2
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_1
								}
#endif
							}
							else
							{
#ifdef DX_NON_INLINE_ASM
								DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, GDX2
									MOV EDX, GDY2
								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_1:
									MOV ECX, EndC
								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_2:
									CMP ESI, SizeX
									JAE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_3
									CMP EDX, SizeY
									JAE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_3

									MOV EBX, EDX
									MOV EAX, ESI
									SHR EBX, 18
									SHR EAX, 17
									MOV EBX, [SrcLineP + EBX * 4]
									AND EAX, 0xfffffffe
									MOV BX, [EAX + EBX]
									MOV [EDI], BX

								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_3:
									ADD ESI, GAADX
									ADD EDX, GAADY
									ADD EDI, 2
									DEC ECX
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_2

									ADD ESI, GLADDX
									ADD EDX, GLADDY
									ADD EDI, DestAddPitch
									DEC C2
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_1
								}
#endif
							}
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
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
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )
						{
#ifdef DX_NON_INLINE_ASM
							DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, GDX2
								MOV EDX, GDY2
							LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_1:
								MOV ECX, EndC
							LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_2:
								CMP ESI, SizeX
								JAE LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3
								CMP EDX, SizeY
								JAE LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3

								MOV EBX, EDX
								MOV EAX, ESI
								SHR EBX, 18
								SHR EAX, 18
								MOV EBX, [SrcLineP + EBX * 4]
								MOVZX EBX, byte ptr [EAX + EBX]
								CMP EBX, TransColor
								JE LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3
								MOV EAX, dword ptr [PaletteTemp + EBX * 4]
								MOV [EDI], EAX

							LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3:
								ADD ESI, GAADX
								ADD EDX, GAADY
								ADD EDI, 4
								DEC ECX
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_2

								ADD ESI, GLADDX
								ADD EDX, GLADDY
								ADD EDI, DestAddPitch
								DEC C2
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_1
							}
#endif
						}
						else
						{
#ifdef DX_NON_INLINE_ASM
							DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, GDX2
								MOV EDX, GDY2
							LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_1:
								MOV ECX, EndC
							LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_2:
								CMP ESI, SizeX
								JAE LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_3
								CMP EDX, SizeY
								JAE LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_3

								MOV EBX, EDX
								MOV EAX, ESI
								SHR EBX, 18
								SHR EAX, 18
								MOV EBX, [SrcLineP + EBX * 4]
								MOVZX EBX, byte ptr [EAX + EBX]
								MOV EAX, dword ptr [PaletteTemp + EBX * 4]
								MOV [EDI], EAX

							LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_3:
								ADD ESI, GAADX
								ADD EDX, GAADY
								ADD EDI, 4
								DEC ECX
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_2

								ADD ESI, GLADDX
								ADD EDX, GLADDY
								ADD EDI, DestAddPitch
								DEC C2
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_1
							}
#endif
						}
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_USEPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ADD :			// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;

			case DX_BLENDMODE_SUB :			// 減算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;
				
			case DX_BLENDMODE_MUL :			// 乗算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;

			case DX_BLENDMODE_MULA :		// 乗算+αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
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
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )
							{
#ifdef DX_NON_INLINE_ASM
								DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, GDX2
									MOV EDX, GDY2
								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_1:
									MOV ECX, EndC
								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_2:
									CMP ESI, SizeX
									JAE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_3
									CMP EDX, SizeY
									JAE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_3

									MOV EBX, EDX
									MOV EAX, ESI
									SHR EBX, 18
									SHR EAX, 16
									MOV EBX, [SrcLineP + EBX * 4]
									AND EAX, 0xfffffffc
									MOV EBX, [EAX + EBX]
									CMP EBX, TransColor
									JE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_3
									MOV [EDI], EBX

								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_3:
									ADD ESI, GAADX
									ADD EDX, GAADY
									ADD EDI, 4
									DEC ECX
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_2

									ADD ESI, GLADDX
									ADD EDX, GLADDY
									ADD EDI, DestAddPitch
									DEC C2
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_1
								}
#endif
							}
							else
							{
#ifdef DX_NON_INLINE_ASM
								DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, GDX2
									MOV EDX, GDY2
								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_1:
									MOV ECX, EndC
								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_2:
									CMP ESI, SizeX
									JAE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_3
									CMP EDX, SizeY
									JAE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_3

									MOV EBX, EDX
									MOV EAX, ESI
									SHR EBX, 18
									SHR EAX, 16
									MOV EBX, [SrcLineP + EBX * 4]
									AND EAX, 0xfffffffc
									MOV EBX, [EAX + EBX]
									MOV [EDI], EBX

								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_3:
									ADD ESI, GAADX
									ADD EDX, GAADY
									ADD EDI, 4
									DEC ECX
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_2

									ADD ESI, GLADDX
									ADD EDX, GLADDY
									ADD EDI, DestAddPitch
									DEC C2
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_1
								}
#endif
							}
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;
			}
		}
		break ;
	}

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



#define DRAWBASICPOLYGONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	for(;;){\
		BSX1    = B1X ;\
		DestBPT = ( BYTE * )DestBP + xx2 * (DESTADDNUM) + B1Y * DestPitch ;\
		\
		if( xx2 < DrawRectLeft ){\
			C       -= (DWORD)( DrawRectLeft - xx2 ) ;\
			DestBPT += (DWORD)( DrawRectLeft - xx2 ) * (DESTADDNUM) ;\
			xx2     += (DWORD)( DrawRectLeft - xx2 ) ;\
		}\
\
		GX = a * ( xx2 - x0 ) + b * ( B1Y - y0 ) + U00 ;\
		GY = c * ( xx2 - x0 ) + d * ( B1Y - y0 ) + V00 ;\
\
		C ++ ;\
		if( ( B1Y >= DrawRectTop && B1Y < DrawRectBottom ) && ( xx2 < DrawRectRight ) && C > 0 ){\
			do{\
				if( (DWORD)GX < SizeX && (DWORD)GY < SizeY ){\
					SrcBPT   = SrcBP   + (GX / QUALI) * (SRCADDNUM) + (GY / QUALI) * SrcPitch   ;\
					BlendBPT = BlendBP + (GX / QUALI)               + (GY / QUALI) * BlendPitch ;\
					DRAW\
				}\
				GX      += GAADX ;\
				GY      += GAADY ;\
				DestBPT += (DESTADDNUM) ;\
				xx2 ++ ;\
			}while( -- C && xx2 < DrawRectRight ) ;\
		}\
\
		C = ( B2X += BA2X ) / QUALI - ( xx2 = ( ( B1X = BSX1 + BA1X ) / QUALI ) ) ;\
		B1Y ++ ;\
		B2Y ++ ;\
\
		C1 -- ;\
		C2 -- ;\
\
		if( C1 == 0 ){\
			if( DMODE1 == 1 ) break ;\
			DMODE1 = 1 ;\
			BA1X   = BAA1X[1] ;\
			xx2    = ( B1X = BS1X[1] ) / QUALI ;\
			B1Y    = BS1Y[1] / QUALI ;\
			C1     = NextC1 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
\
		if( C2 == 0 ){\
			if( DMODE2 == 1 ) break ;\
			DMODE2 = 1 ;\
			BA2X   = BAA2X[1] ;\
			B2X    = BS2X[1]  ;\
			B2Y    = BS2Y[1] / QUALI ;\
			C2     = NextC2 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
	}\
}


#define DRAWBASICPOLYGONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	for(;;){\
		BSX1    = B1X ;\
		DestBPT = ( BYTE * )DestBP + xx2 * (DESTADDNUM) + B1Y * DestPitch ;\
		\
		if( xx2 < DrawRectLeft ){\
			C       -= (DWORD)( DrawRectLeft - xx2 ) ;\
			DestBPT += (DWORD)( DrawRectLeft - xx2 ) * (DESTADDNUM) ;\
			xx2     += (DWORD)( DrawRectLeft - xx2 ) ;\
		}\
\
		GX = a * ( xx2 - x0 ) + b * ( B1Y - y0 ) + U00 ;\
		GY = c * ( xx2 - x0 ) + d * ( B1Y - y0 ) + V00 ;\
\
		C ++ ;\
		if( ( B1Y >= DrawRectTop && B1Y < DrawRectBottom ) && ( xx2 < DrawRectRight ) && C > 0 ){\
			do{\
				if( (DWORD)GX < SizeX && (DWORD)GY < SizeY ){\
					SrcBPT = SrcBP + (GX / QUALI) * (SRCADDNUM) + (GY / QUALI) * SrcPitch ;\
					DRAW\
				}\
				GX      += GAADX ;\
				GY      += GAADY ;\
				DestBPT += (DESTADDNUM) ;\
				xx2 ++ ;\
			}while( -- C && xx2 < DrawRectRight ) ;\
		}\
\
		C = ( B2X += BA2X ) / QUALI - ( xx2 = ( ( B1X = BSX1 + BA1X ) / QUALI ) ) ;\
		B1Y ++ ;\
		B2Y ++ ;\
\
		C1 -- ;\
		C2 -- ;\
\
		if( C1 == 0 ){\
			if( DMODE1 == 1 ) break ;\
			DMODE1 = 1 ;\
			BA1X   = BAA1X[1] ;\
			xx2    = ( B1X = BS1X[1] ) / QUALI ;\
			B1Y    = BS1Y[1] / QUALI ;\
			C1     = NextC1 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
\
		if( C2 == 0 ){\
			if( DMODE2 == 1 ) break ;\
			DMODE2 = 1 ;\
			BA2X   = BAA2X[1] ;\
			B2X    = BS2X[1]  ;\
			B2Y    = BS2Y[1] / QUALI ;\
			C2     = NextC2 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
	}\
}


#define DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( DRAW, DESTADDNUM )	\
{\
	for(;;){\
		BSX1 = B1X ;\
		DestBPT = ( BYTE * )DestBP + xx2 * (DESTADDNUM) + B1Y * DestPitch ;\
\
		if( xx2 < DrawRectLeft ){\
			C       -= (DWORD)( DrawRectLeft - xx2 ) ;\
			DestBPT += (DWORD)( DrawRectLeft - xx2 ) * (DESTADDNUM) ;\
			xx2     += (DWORD)( DrawRectLeft - xx2 ) ;\
		}\
\
		C ++ ;\
		if( ( B1Y >= DrawRectTop && B1Y < DrawRectBottom ) && ( xx2 < DrawRectRight ) && C > 0 ){\
			do{\
				DRAW\
				DestBPT += (DESTADDNUM) ;\
				xx2 ++ ;\
			}while( -- C && xx2 < DrawRectRight );\
		}\
\
		C = ( B2X += BA2X ) / QUALI - ( xx2 = ( ( B1X = BSX1 + BA1X ) / QUALI ) ) ;\
		B1Y ++ ;\
		B2Y ++ ;\
\
		C1 -- ;\
		C2 -- ;\
\
		if( C1 == 0 ){\
			if( DMODE1 == 1 ) break ;\
			DMODE1 = 1 ;\
			BA1X   = BAA1X[1] ;\
			xx2    = ( B1X = BS1X[1] ) / QUALI  ;\
			B1Y    = BS1Y[1] / QUALI ;\
			C1     = NextC1 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
\
		if( C2 == 0 ){\
			if( DMODE2 == 1 ) break ;\
			DMODE2 = 1 ;\
			BA2X   = BAA2X[1] ;\
			B2X    = BS2X[1]  ;\
			B2Y    = BS2Y[1] / QUALI  ;\
			C2     = NextC2 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
	}\
}




// イメージを変形描画する
#define PAL16	PaletteWP[ *SrcBPT ]
#define DST16	*DestWPT
#define SRC16	*SrcWPT
#define SRCA16	SrcBPT[2]
#define DSTP16	DestWPT

#define PALP32	(BYTE *)&PaletteDP[ *SrcBPT ]
#define SRCP32	SrcBPT
#define DSTP32	DestBPT

#define BLND	*BlendBPT
extern void DrawBasicPolygonMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const DX_POINTDATA *pos3, int TransFlag, const MEMIMG *BlendImg, unsigned int Color )
{
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Src2, Data, DataD ;
	DWORD *RedTable , *GreenTable , *BlueTable ;
	DWORD SrcPitch = 0, DestPitch, BlendPitch = 0 ;
	DWORD TransColor = 0 ;
	int GX, GY, GAADX, GAADY ;
	int BD1X[2],BD1Y[2],BD2X[2],BD2Y[2] ;	// 描画ベクトル１、２(実サイズ単位+固定小数点が1000の位にあり)
											// スクリーン上に見て１は描画開始点から左下方向に伸びるベクトル
											// ２は描画開始点から右下に方向に延びるベクトル
											// ４は２のベクトルから左下方向に延びるベクトル
	int	BS1X[2] ,BS1Y[2] ,BS2X[2] ,BS2Y[2];	// 描画開始座標
	int B1X , B1Y , B2X , B2Y;				// 描画中にカウンタとして使うベクトル変数(描画先用) 
	int BA1X , BA1Y , BA2X , BA2Y ;			// 使用中のスクリーン描画用ベクトル
	int BAA1X[2], BAA1Y[2], BAA2X[2], BAA2Y[2] ;// 使用中のスクリーン描画用ベクトル
	int DMODE1 , DMODE2 ;					// 描画状態(0.一つ目のベクトルに沿って描画中 1.2つ目のベクトルの沿って描画中)
	int a, b, c, d, V00, U00, x0, y0 ;
	int DrawRectRight,  DrawRectLeft ;
	int DrawRectBottom, DrawRectTop ;
	int BSX1, xx2 ;
	int C1, NextC1, C2, NextC2, C ;
	DWORD SizeX , SizeY ;
	
	union
	{
		BYTE  ColorB[4] ;
		DWORD ColorD ;
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
	if( SrcImg != NULL && SrcImg->Base->UseTransColor == 0 )
	{
		TransFlag = FALSE ;
	}
	
	// カラータイプが違うか、描画先がαチャンネル付かパレット付きだった場合はエラー
	if( SrcImg != NULL && SrcImg->Base->ColorType != DestImg->Base->ColorType ) return ;
	if( DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// 描画準備
	{
		int px[4],py[4],grx[4],gry[4] ;		// 描画矩形の各頂点、(左上、左下、右下、右上の順) 

		// 描画座標を得る
		for( int i = 0 ; i < 3 ; i ++ )
		{
			px[i]  = pos3[i].x * QUALI + QUALI / 2 ;
			py[i]  = pos3[i].y * QUALI + QUALI / 2 ;
			grx[i] = pos3[i].u * QUALI ;
			gry[i] = pos3[i].v * QUALI ;
		}

		// 一番上に来ている座標によって描画起点座標等を決定
		{
			double VVx[2] , VVy[2] , Vx , Vy ; 
			int i , j , k ;
			int a1 , a2 , a3 , a4;
			int YTable[3] , XTable[3] ;
			int y1 , y2 , y3 , x1 , x2 , x3 ;

			DMODE1 = DMODE2 = 0 ;

			for( i = 0 ; i < 3 ; i ++ ) YTable[i] = XTable[i] = i ;

			for( i = 0 ; i < 3 ; i ++ )
			{
				for( j = i + 1 ; j < 3 ; j ++ )
				{
					if( px[XTable[i]] > px[XTable[j]] )
					{
						int tmp ;

						tmp = XTable[j] ; 
						XTable[j] = XTable[i] ;
						XTable[i] = tmp ;
					}

					if( py[YTable[i]] > py[YTable[j]] )
					{
						int tmp ;

						tmp = YTable[j] ; 
						YTable[j] = YTable[i] ;
						YTable[i] = tmp ;
					}
				}
			}

			y1 = YTable[0] ; y2 = YTable[1] ; y3 = YTable[2] ;  
			x1 = XTable[0] ; x2 = XTable[1] ; x3 = XTable[2] ;  

			// UV算出用行列 a , b , c, d を算出する
			{
				int dx01 , dx02 , dy01 , dy02 , du01 , du02 , dv01 , dv02 , tmp1 , tmp2  ;

				dx01 = ( px[x3] - px[x1] ) / QUALI ;	du01 = ( grx[x3] - grx[x1] ) / QUALI ;
				dx02 = ( px[x2] - px[x1] ) / QUALI ;	du02 = ( grx[x2] - grx[x1] ) / QUALI ;
				dy01 = ( py[x3] - py[x1] ) / QUALI ;	dv01 = ( gry[x3] - gry[x1] ) / QUALI ;
				dy02 = ( py[x2] - py[x1] ) / QUALI ;	dv02 = ( gry[x2] - gry[x1] ) / QUALI ;

				tmp1  = dx01 * dy02 - dx02 * dy01;
				if( tmp1 == 0 ) return ;
				tmp2  = dx02 * dy01 - dx01 * dy02;

				a = _DTOL( (double)(du01 * dy02 - du02 * dy01) * QUALI / tmp1 );
				b = _DTOL( (double)(du02 * dx01 - du01 * dx02) * QUALI / tmp1 );
				c = _DTOL( (double)(dv01 * dy02 - dv02 * dy01) * QUALI / tmp1 );
				d = _DTOL( (double)(dv02 * dx01 - dv01 * dx02) * QUALI / tmp1 );
				x0 = px[x1] / QUALI ;
				y0 = py[x1] / QUALI ;
				U00 = grx[x1] ;//- a * x0 - b * y0 ;
				V00 = gry[x1] ;//- c * x0 - d * y0 ;
			}

			// イレギュラー用処理
			if( py[y1] == py[y2] )
			{
				// 上部２頂点のＹ成分が同じ場合の処理
				DMODE1 = DMODE2 = 1 ;

				// 参照進み値を得る
				{
					VVx[0] = ( double )( px[y2] - px[y3] ) / QUALI ; VVy[0] = ( double )( py[y2] - py[y3] ) / QUALI ;
					VVx[1] = ( double )( px[y1] - px[y3] ) / QUALI ; VVy[1] = ( double )( py[y1] - py[y3] ) / QUALI ;	

					if( VVy[0] == 0 || VVy[1] == 0 ) return ;

					VVx[0] /= VVy[0] ;
					VVy[0] /= VVy[0] ;

					VVx[1] /= VVy[1] ;
					VVy[1] /= VVy[1] ;

					if( px[y1] > px[y2] )
					{
						a1 = 0 ; a2 = 1 ;
						a3 = y1 ; a4 = y2 ;
					}
					else
					{
						a1 = 1 ; a2 = 0 ;
						a3 = y2 ; a4 = y1 ;
					}

					Vx = VVx[a1] - VVx[a2] ; Vy = VVy[a1] - VVy[a2] ;
				}

				BS1X[1] = px[a4] ;	BS1Y[1] = py[a4] ;

				BS2X[1] = px[a3] ;	BS2Y[1] = py[a3] ;


				BD1X[1] = px[y3] - BS1X[1] ;					
				BD1Y[1] = py[y3] - BS1Y[1] ;
				if( ( k = BD1Y[1] / QUALI ) == 0 ) return ;
				BAA1X[1] = BD1X[1] / k ;		
				BAA1Y[1] = QUALI ;

				BD2X[1] = px[y3] - BS2X[1] ;					
				BD2Y[1] = py[y3] - BS2Y[1] ;
				if( BD2Y[1] / QUALI == 0 ) return ;
				BAA2X[1] = BD2X[1] / ( BD2Y[1] / QUALI );		
				BAA2Y[1] = QUALI ;
			}
			else
			{
				// 下部２頂点のＹ成分が同じ場合の処理
				if( py[y2] == py[y3] )
				{
					DMODE1 = DMODE2 = 1 ;

					// 参照進み値を得る
					{
						VVx[0] = ( double )( px[y2] - px[y1] ) / QUALI ; VVy[0] = ( double )( py[y2] - py[y1] ) / QUALI ;
						VVx[1] = ( double )( px[y3] - px[y1] ) / QUALI ; VVy[1] = ( double )( py[y3] - py[y1] ) / QUALI ;	

						if( VVy[0] == 0 || VVy[1] == 0 ) return ;
						VVx[0] /= VVy[0] ;

						VVx[1] /= VVy[1] ;

						BS1X[1] = BS2X[1] = px[y1] ;
						BS1Y[1] = BS2Y[1] = py[y1] ;

						if( px[y2] > px[y3] )
						{	
							a1 = 0 ; a2 = 1 ;
							a3 = y3 ; a4 = y2 ;
						}
						else
						{
							a1 = 1 ; a2 = 0 ;
							a3 = y2 ; a4 = y3 ;
						}

						Vx = VVx[a1] - VVx[a2] ; Vy = VVy[a1] - VVy[a2] ;

						BD1X[1] = px[a3] - BS1X[1] ;
						BD1Y[1] = py[a3] - BS1Y[1] ;

						BD2X[1] = px[a4] - BS2X[1] ;
						BD2Y[1] = py[a4] - BS2Y[1] ;
					}

					if( ( k = BD1Y[1] / QUALI ) == 0 ) return ;
					BAA1X[1] = BD1X[1] / k ;
					BAA1Y[1] = QUALI ;

					if( BD2Y[1] / QUALI == 0 ) return ;
					BAA2X[1] = BD2X[1] / ( BD2Y[1] / QUALI ) ;
					BAA2Y[1] = QUALI ;
				}
				else
				{
					double ax  , bx ;

					if( ( ( ax = ( ( double )( py[y2] - py[y1] ) / QUALI ) ) == 0.0 ) || 
						( ( bx = ( ( double )( py[y3] - py[y1] ) / QUALI ) ) == 0.0 ) ) return ;

					ax = ( double )( px[y2] - px[y1] ) / ax ;
					bx = ( double )( px[y3] - px[y1] ) / bx ;

					// 通常処理
					VVx[0] = ( double )( px[y2] - px[y1] ) / QUALI ; VVy[0] = ( double )( py[y2] - py[y1] ) / QUALI ;
					VVx[1] = ( double )( px[y3] - px[y1] ) / QUALI ; VVy[1] = ( double )( py[y3] - py[y1] ) / QUALI ;	

					if( VVy[0] == 0 || VVy[1] == 0 ) return ;
					VVx[0] /= VVy[0] ;
					VVy[0] /= VVy[0] ;

					VVx[1] /= VVy[1] ; 
					VVy[1] /= VVy[1] ;


					if( ax < bx ) 
					{
						DMODE1 = 0 ;
						DMODE2 = 1 ;

						// 参照進み値を得る
						Vx = VVx[1] - VVx[0] ; Vy = VVy[1] - VVy[0] ;

						BS1X[0] = BS2X[1] = px[y1] ;
						BS1Y[0] = BS2Y[1] = py[y1] ;

						BD1X[0] = px[y2] - BS1X[0] ; 
						BD1Y[0] = py[y2] - BS1Y[0] ;

						if( ( k = BD1Y[0] / QUALI ) == 0 ) return ;
						BAA1X[0] = ( int )( BD1X[0] / k ) ;
						BAA1Y[0] = QUALI ;

						BS1X[1] = px[y2] ;
						BS1Y[1] = py[y2] ;
						BD1X[1] = px[y3] - BS1X[1] ; 
						BD1Y[1] = py[y3] - BS1Y[1] ;

						if( ( k = BD1Y[1] / QUALI ) == 0 ) return ;
						BAA1X[1] = ( int )( BD1X[1] / k ) ;
						BAA1Y[1] = QUALI ;

						BD2X[1] = px[y3] - BS2X[1] ; 
						BD2Y[1] = py[y3] - BS2Y[1] ;
						if( ( BD2Y[1] / QUALI ) == 0 ) return ;
						BAA2X[1] = ( int )( BD2X[1] / ( BD2Y[1] / QUALI ) ) ;
						BAA2Y[1] = QUALI ;
					}
					else
					{
						DMODE1 = 1 ;
						DMODE2 = 0 ;

						// 参照進み値を得る
						Vx = VVx[0] - VVx[1] ; Vy = VVy[0] - VVy[1] ;

						BS1X[1] = BS2X[0] = px[y1] ;
						BS1Y[1] = BS2Y[0] = py[y1] ;

						BD2X[0] = px[y2] - BS2X[0] ; 
						BD2Y[0] = py[y2] - BS2Y[0] ;
						if( ( BD2Y[0] / QUALI ) == 0 ) return ;
						BAA2X[0] = BD2X[0] / ( BD2Y[0] / QUALI ) ;
						BAA2Y[0] = QUALI ;

						BS2X[1] = px[y2] + 1 ;
						BS2Y[1] = py[y2] ;
						BD2X[1] = px[y3] - BS2X[1] ; 
						BD2Y[1] = py[y3] - BS2Y[1] ;
						if( ( BD2Y[1] / QUALI ) == 0 ) return ;
						BAA2X[1] = BD2X[1] / ( BD2Y[1] / QUALI ) ;
						BAA2Y[1] = QUALI ;

						BD1X[1] = px[y3] - BS1X[1] ; 
						BD1Y[1] = py[y3] - BS1Y[1] ;

						if( ( k = BD1Y[1] / QUALI ) == 0 ) return ;
						BAA1X[1] = BD1X[1] / k ;
						BAA1Y[1] = QUALI ;
					}
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

	if( BlendImg != NULL )
	{
		BlendBP    = BlendImg->UseImage ;
		BlendPitch = BlendImg->Base->Pitch ;
	}

	if( SrcImg != NULL )
	{
		TransColor = SrcImg->Base->TransColor ;
		PaletteBP  = (BYTE *)SrcImg->Base->Palette ;
	}
	else
	{
		PaletteBP  = NULL ;
	}
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;
	RedTable   = MemImgManage.RateTable[MemImgManage.DrawBright.Red] ;
	GreenTable = MemImgManage.RateTable[MemImgManage.DrawBright.Green] ;
	BlueTable  = MemImgManage.RateTable[MemImgManage.DrawBright.Blue] ;
	
	// 描画準備
	{
		B1X = BS1X[DMODE1] ; B1Y = BS1Y[DMODE1] ;
		B2X = BS2X[DMODE2] ; B2Y = BS2Y[DMODE2] ;

		BA1X = BAA1X[DMODE1] ; BA1Y = BAA1Y[DMODE1] ;
		BA2X = BAA2X[DMODE2] ; BA2Y = BAA2Y[DMODE2] ;

		DrawRectRight  = MemImgManage.DrawArea.right ;
		DrawRectLeft   = MemImgManage.DrawArea.left ;
		DrawRectBottom = MemImgManage.DrawArea.bottom ;
		DrawRectTop    = MemImgManage.DrawArea.top ;
		GAADX = a ;
		GAADY = c ;

		NextC1 = ( DMODE1 == 0 ) ? BD1Y[1] / QUALI : 0 ;
		NextC2 = ( DMODE2 == 0 ) ? BD2Y[1] / QUALI : 0 ;
		C1 = BD1Y[DMODE1] / QUALI ;
		C2 = BD2Y[DMODE2] / QUALI ;

		B1Y /= QUALI ;
		C = B2X / QUALI - B1X / QUALI ; 

		xx2 = B1X / QUALI ;
	}

	// 画像がある場合と無い場合で処理を分岐
	if( SrcImg != NULL )
	{
		SizeX = (DWORD)SrcImg->Width  * QUALI ;
		SizeY = (DWORD)SrcImg->Height * QUALI ;
		
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
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
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
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
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
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
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
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
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
	// テクスチャが無い場合
	{
		// 描画準備
		if( DestImg->Base->ColorType == 0 )
		{
			ColorD = RUP16( RTABLE( RBOT16( Color ) ) ) |
				     GUP16( GTABLE( GBOT16( Color ) ) ) |
				   	 BUP16( BTABLE( BBOT16( Color ) ) ) ;
		}
		else
		{
			ColorD = Color ;
			ColorB[0] = (BYTE)BTABLE( ColorB[0] ) ;
			ColorB[1] = (BYTE)GTABLE( ColorB[1] ) ;
			ColorB[2] = (BYTE)RTABLE( ColorB[2] ) ;
		}
		SrcBPT = ColorB ;

		// カラーモードによって処理を分岐
		switch( DestImg->Base->ColorType )
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

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
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

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
				break ;
			}
			break ;
		}
	}

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


// イメージを変形描画する
extern void DrawTransformMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const POINT *pos4, int TransFlag, const MEMIMG *BlendImg )
{
	// 描画範囲を得る
	{
		RECT DrawRect ;
		POINT pos[4] ;
		int i, MaxY, MinY, MaxX, MinX ;

		_MEMCPY( pos, pos4, sizeof( pos ) ) ;

		// 一番上に来ている座標によって描画起点座標等を決定
		{
			MaxY = 0 ; MinY = 0 ;
			MaxX = 0 ; MinX = 0 ; 
			for( i = 1 ; i < 4 ; i ++ )
			{
				if( pos[MaxY].y < pos[i].y ) MaxY = i ;
				if( pos[MinY].y > pos[i].y ) MinY = i ;

				if( pos[MaxX].x < pos[i].x ) MaxX = i ;
				if( pos[MinX].x > pos[i].x ) MinX = i ;
			}

			DrawRect.left   = pos[MinX].x - 2 ; 
			DrawRect.top    = pos[MinY].y - 2 ; 

			DrawRect.right  = pos[MaxX].x + 4;
			DrawRect.bottom = pos[MaxY].y + 4;
		}

		RectClipping_Inline( &DrawRect , &MemImgManage.DrawArea ) ;
		if( DrawRect.left == DrawRect.right || DrawRect.top == DrawRect.bottom ) return ;
	}

	// 描画する
	{
		DX_POINTDATA p[4] ;

		p[0].x = pos4[0].x ;
		p[0].y = pos4[0].y ;
		p[0].u = 0 ;
		p[0].v = 0 ;

		p[1].x = pos4[1].x ;
		p[1].y = pos4[1].y ;
		p[1].u = ( int )SrcImg->Width ;
		p[1].v = 0 ;

		p[2].x = pos4[2].x ;
		p[2].y = pos4[2].y ;
		p[2].u = 0 ;
		p[2].v = ( int )SrcImg->Height ;

		p[3].x = pos4[3].x ;
		p[3].y = pos4[3].y ;
		p[3].u = ( int )SrcImg->Width ;
		p[3].v = ( int )SrcImg->Height ;

		DrawBasicPolygonMemImg( DestImg, SrcImg, &p[0], TransFlag, BlendImg, 0 ) ;
		DrawBasicPolygonMemImg( DestImg, SrcImg, &p[1], TransFlag, BlendImg, 0 ) ;
	}

	// 終了
	return ;
}

#endif

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

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
#ifndef DX_NON_INLINE_ASM
#include "Windows/DxWindow.h"
#endif // DX_NON_INLINE_ASM

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

extern void DrawMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, int DestX, int DestY, int TransFlag, const MEMIMG *BlendImg )
{
	return ;
}

extern void DrawEnlargeMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const RECT *DestRect, int TransFlag, const MEMIMG *BlendImg )
{
	return ;
}

#else

#define DRAWMEMIMG_UBI_UAC_FD( DRAW, ALPHAMAXDRAW, SRCADDNUM, DESTADDNUM )	\
	do{\
		while( ( (*AnaBP) >> 6 ) != 3 ){\
			LoopNum = ( DWORD )( ( *AnaBP & 0x3f ) + 1 ) ;\
			switch( (*AnaBP) >> 6 )\
			{\
			case 0 :\
				BlendBP += LoopNum ;\
				SrcBP += LoopNum * SRCADDNUM ;\
				DestBP += LoopNum * DESTADDNUM ;\
				break ;\
				\
			case 1 :\
				do{\
					ALPHAMAXDRAW\
					BlendBP ++ ;\
					SrcBP += SRCADDNUM ;\
					DestBP += DESTADDNUM ;\
				}while( -- LoopNum ) ;\
				break ;\
				\
			case 2 :\
				do{\
					DRAW\
					BlendBP ++ ;\
					SrcBP += SRCADDNUM ;\
					DestBP += DESTADDNUM ;\
				}while( -- LoopNum ) ;\
				break ;\
			}\
			AnaBP ++ ;\
		}\
		AnaBP ++ ;\
		BlendBP += BlendAddPitch ;\
		SrcBP  += SrcAddPitch ;\
		DestBP += DestAddPitch ;\
	}while( -- Height );


#define DRAWMEMIMG_UBI_NAC_FD( DRAW, SRCADDNUM, DESTADDNUM )	\
	do{\
		while( ( (*AnaBP) >> 6 ) != 3 ){\
			LoopNum = ( DWORD )( ( *AnaBP & 0x3f ) + 1 ) ;\
			if( (*AnaBP) >> 6 )\
			{\
				do{\
					DRAW\
					BlendBP ++ ;\
					SrcBP += SRCADDNUM ;\
					DestBP += DESTADDNUM ;\
				}while( -- LoopNum ) ;\
			}else{\
				SrcBP += LoopNum * SRCADDNUM ;\
				DestBP += LoopNum * DESTADDNUM ;\
				BlendBP += LoopNum ;\
			}\
			AnaBP ++ ;\
		}\
		AnaBP ++ ;\
		BlendBP += BlendAddPitch ;\
		SrcBP  += SrcAddPitch ;\
		DestBP += DestAddPitch ;\
	}while( -- Height );


#define DRAWMEMIMG_NBI_UAC_FD( DRAW, ALPHAMAXDRAW, SRCADDNUM, DESTADDNUM )	\
	do{\
		while( ( (*AnaBP) >> 6 ) != 3 ){\
			LoopNum = ( DWORD )( ( *AnaBP & 0x3f ) + 1 ) ;\
			switch( (*AnaBP) >> 6 )\
			{\
			case 0 :\
				SrcBP += LoopNum * SRCADDNUM ;\
				DestBP += LoopNum * DESTADDNUM ;\
				break ;\
				\
			case 1 :\
				do{\
					ALPHAMAXDRAW\
					SrcBP += SRCADDNUM ;\
					DestBP += DESTADDNUM ;\
				}while( -- LoopNum ) ;\
				break ;\
				\
			case 2 :\
				do{\
					DRAW\
					SrcBP += SRCADDNUM ;\
					DestBP += DESTADDNUM ;\
				}while( -- LoopNum ) ;\
				break ;\
			}\
			AnaBP ++ ;\
		}\
		AnaBP ++ ;\
		SrcBP  += SrcAddPitch ;\
		DestBP += DestAddPitch ;\
	}while( -- Height );

	
#define DRAWMEMIMG_NBI_NAC_FD( DRAW, SRCADDNUM, DESTADDNUM )	\
	do{\
		while( ( (*AnaBP) >> 6 ) != 3 ){\
			LoopNum = ( DWORD )( ( *AnaBP & 0x3f ) + 1 ) ;\
			if( (*AnaBP) >> 6 ){\
				do{\
					DRAW\
					SrcBP += SRCADDNUM ;\
					DestBP += DESTADDNUM ;\
				}while( -- LoopNum ) ;\
			}else{\
				SrcBP += LoopNum * SRCADDNUM ;\
				DestBP += LoopNum * DESTADDNUM ;\
			}\
			AnaBP ++ ;\
		}\
		AnaBP ++ ;\
		SrcBP  += SrcAddPitch ;\
		DestBP += DestAddPitch ;\
	}while( -- Height );


#define DRAWMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
	do{\
		wtemp = Width ;\
		do{\
			DRAW;\
			BlendBP ++ ;\
			SrcBP  += (SRCADDNUM) ;\
			DestBP += (DESTADDNUM) ;\
		}while( -- wtemp );\
		BlendBP += BlendAddPitch ;\
		SrcBP  += SrcAddPitch ;\
		DestBP += DestAddPitch ;\
	}while( -- Height );


#define DRAWMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
	do{\
		wtemp = Width ;\
		do{\
			DRAW\
			SrcBP  += (SRCADDNUM) ;\
			DestBP += (DESTADDNUM) ;\
		}while( -- wtemp );\
		SrcBP  += SrcAddPitch ;\
		DestBP += DestAddPitch ;\
	}while( -- Height );







extern void DrawTurnMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, int XTurnFlag, int YTurnFlag, int DestX, int DestY, int TransFlag, const MEMIMG *BlendImg )
{
	int x1, y1, x2, y2 ;
	
	if( XTurnFlag )
	{
		x1 = ( int )( DestX + SrcImg->Width ) ;
		x2 = DestX ;
	}
	else
	{
		x1 = DestX ;
		x2 = ( int )( DestX + SrcImg->Width ) ;
	}

	if( YTurnFlag )
	{
		y1 = ( int )( DestY + SrcImg->Height ) ;
		y2 = DestY ;
	}
	else
	{
		y1 = DestY ;
		y2 = ( int )( DestY + SrcImg->Height ) ;
	}

	DrawEnlargeMemImg( DestImg, SrcImg, x1, y1, x2, y2, TransFlag, BlendImg ) ;
}


// イメージを描画する
#define PAL16	PaletteWP[ *SrcBP ]
#define DST16	*DestWP
#define SRC16	*SrcWP
#define SRCA16	SrcBP[2]
#define DSTP16	DestWP

#define PALP32	(BYTE *)&PaletteDP[ *SrcBP ]
#define SRCP32	SrcBP
#define DSTP32	DestBP

#define BLND	*BlendBP
extern void DrawMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, int DestX, int DestY, int TransFlag, const MEMIMG *BlendImg )
{
	DWORD TransCode ;
	DWORD Width, Height ;
	DWORD_PTR wtemp, SrcPP, DestPP, PalletPP ;
	DWORD TransColor ;
	DWORD Src1, Src2, Data, DataD ;
	BYTE *RateTable16 ;
	DWORD *RateTable1 ;
	int *RateTable2 ;
	int FastDrawFlag ;
	DWORD *RedTable, *GreenTable, *BlueTable ;
	DWORD LoopNum ;
	DWORD SrcX, SrcY ;
	DWORD SrcAddPitch, DestAddPitch, BlendAddPitch = 0 ;
	BYTE *AnaBP ;
	BYTE *BlendBP = NULL ;
	static DWORD PaletteTemp[256], counter, transcode, width, height, destAddPitch, srcAddPitch ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE *PaletteBP ;
		WORD *PaletteWP ;
		DWORD *PaletteDP ;
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

	// 透過色無効の場合は TransFlag を倒す
	if( SrcImg->Base->UseTransColor == 0 )
	{
		TransFlag = FALSE ;
	}

	// カラータイプが違うか、描画先がαチャンネル付かパレット付きだった場合はエラー
	if( SrcImg->Base->ColorType != DestImg->Base->ColorType || DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// 描画矩形のセット
	{
		RECT DrawRect ;

		DrawRect.left   = DestX ;
		DrawRect.top    = DestY ;
		DrawRect.right  = ( LONG )( DestX + SrcImg->Width  ) ;
		DrawRect.bottom = ( LONG )( DestY + SrcImg->Height ) ;

		RectClipping_Inline( &DrawRect, &MemImgManage.DrawArea ) ;
		Width  = ( DWORD )( DrawRect.right  - DrawRect.left ) ;
		Height = ( DWORD )( DrawRect.bottom - DrawRect.top  ) ;
		SrcX   = ( DWORD )( DrawRect.left   - DestX ) ;
		SrcY   = ( DWORD )( DrawRect.top    - DestY ) ;
		DestX  = DrawRect.left ;
		DestY  = DrawRect.top  ;

		if( Width == 0 || Height == 0 ) return ;
	}

	// 高速描画を行うかどうかをセット
	FastDrawFlag = ( TransFlag == TRUE ) && ( SrcImg->Base->AnalysisDataValid == 1 ) && ( Width == SrcImg->Width ) && ( Height == SrcImg->Height ) ;

	// 転送元、転送先のアドレスをセット
//	SrcBP = FastDrawFlag && SrcImg->AlphaImage != NULL ? SrcImg->AlphaImage : SrcImg->UseImage ;
	SrcBP        = SrcImg->UseImage ;
	SrcBP       += SrcX * SrcImg->Base->ColorDataP->PixelByte + SrcY * SrcImg->Base->Pitch ;
	SrcAddPitch  = SrcImg->Base->Pitch - Width * SrcImg->Base->ColorDataP->PixelByte ;

	DestBP       = DestImg->UseImage + DestX * DestImg->Base->ColorDataP->PixelByte + DestY * DestImg->Base->Pitch ;
	DestAddPitch = DestImg->Base->Pitch - Width * DestImg->Base->ColorDataP->PixelByte ;

	if( BlendImg != NULL )
	{
		BlendBP = BlendImg->UseImage + SrcX + SrcY * BlendImg->Base->Pitch ;
		BlendAddPitch = BlendImg->Base->Pitch - Width ;
	}

	AnaBP      = SrcImg->Base->AnalysisData ;
	PaletteBP  = (BYTE *)SrcImg->Base->Palette ;
	TransColor = SrcImg->Base->TransColor ;
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;
	RedTable   = MemImgManage.RateTable[MemImgManage.DrawBright.Red] ;
	GreenTable = MemImgManage.RateTable[MemImgManage.DrawBright.Green] ;
	BlueTable  = MemImgManage.RateTable[MemImgManage.DrawBright.Blue] ;
	SrcPP      = (DWORD_PTR)SrcBP ;
	DestPP     = (DWORD_PTR)DestBP ;
	PalletPP   = (DWORD_PTR)PaletteBP ;
	RateTable16 = &MemImgManage.RateTable16[MemImgManage.BlendParam / 4][0][0] ;
	
	if( SrcImg->Base->UsePalette == TRUE )
	{
		TransCode =   SrcImg->Base->TransColor         | ( SrcImg->Base->TransColor <<  8 ) |
					( SrcImg->Base->TransColor << 16 ) | ( SrcImg->Base->TransColor << 24 ) ;

		_MEMCPY( PaletteTemp, SrcImg->Base->Palette, ( size_t )( SrcImg->Base->ColorNum * ( SrcImg->Base->ColorType == 0 ? 2 : 4 ) ) ) ;
	}
	else
	{
		if( SrcImg->Base->ColorType == 0 )
		{
			TransCode = SrcImg->Base->TransColor | ( SrcImg->Base->TransColor << 16 ) ;
		}
		else
		{
			TransCode = SrcImg->Base->TransColor ;
		}
	}

	// クリッピングが行われなくて、且つ解析データがある場合と、そうでない場合で処理を分岐
//	if( ( SrcImg->AnalysisDataValid == 0 ) || ( Width != SrcImg->Width ) || ( Height != SrcImg->Height ) )
	if( !FastDrawFlag )
	// 解析情報を使用しない通常の転送処理
	{
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
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )
							{
#ifdef DX_NON_INLINE_ASM
								DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
#else
								_asm
								{
									MOV ESI, SrcPP
									MOV EDI, DestPP
									MOV EDX, PalletPP
									MOV EAX, TransCode
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_1:
									MOV ECX, Width
									SHR ECX, 2
									JZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_7
									
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_2:
									MOV EBX, [ESI]
									CMP EBX, EAX
									JE LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_6

									MOVZX EBX, BYTE PTR [ESI]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_3
									MOV BX, [EDX+EBX*2]
									MOV [EDI], BX
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_3:
									MOVZX EBX, BYTE PTR [ESI+1]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_4
									MOV BX, [EDX+EBX*2]
									MOV [EDI+2], BX
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_4:
									MOVZX EBX, BYTE PTR [ESI+2]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_5
									MOV BX, [EDX+EBX*2]
									MOV [EDI+4], BX
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_5:
									MOVZX EBX, BYTE PTR [ESI+3]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_6
									MOV BX, [EDX+EBX*2]
									MOV [EDI+6], BX
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_6:
									ADD ESI, 4
									ADD EDI, 8
									DEC ECX
									JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_2
									
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_7:
									MOV ECX, Width
									AND ECX, 3
									JZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_10
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_8:
									MOVZX EBX, BYTE PTR [ESI]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_9
									MOV BX, [EDX+EBX*2]
									MOV [EDI], BX
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_9:
									INC ESI
									ADD EDI, 2
									DEC ECX
									JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_8
									
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_10:
									ADD ESI, SrcAddPitch
									ADD EDI, DestAddPitch
									DEC Height
									JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_TR_1
								}
#endif
							}
							else
							{
#ifdef DX_NON_INLINE_ASM
								DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
#else
								_asm
								{
									MOV ESI, SrcPP
									MOV EDI, DestPP
									MOV EDX, Height
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_1:
									MOV ECX, Width
									SHR ECX, 2
									JZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_3

								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_2:
									MOV EBX, [ESI]
									MOVZX EAX, BL
									MOV AX, word ptr [PaletteTemp+EAX*2]
									SHR EBX, 8
									MOV [EDI], AX

									MOVZX EAX, BL
									MOV AX, word ptr [PaletteTemp+EAX*2]
									SHR EBX, 8
									MOV [EDI+2], AX

									MOVZX EAX, BL
									MOV AX, word ptr [PaletteTemp+EAX*2]
									SHR EBX, 8
									MOV [EDI+4], AX

									MOV AX, word ptr [PaletteTemp+EBX*2]
									MOV [EDI+6], AX

									ADD ESI, 4
									ADD EDI, 8
									DEC ECX
									JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_2

								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_3:
									MOV ECX, Width
									AND ECX, 3
									JZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_5
								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_4:
									MOVZX EBX, byte ptr [ESI]
									MOV BX, word ptr [PaletteTemp+EBX*2]
									MOV [EDI], BX
									INC ESI
									ADD EDI, 2
									DEC ECX
									JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_4

								LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_5:
									ADD ESI, SrcAddPitch
									ADD EDI, DestAddPitch
									DEC EDX
									JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_PAL_NTR_1
								}
#endif
							}
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )
							{
#ifdef DX_NON_INLINE_ASM
								DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
#else
								if( MemImgManage.BlendParam == 128 )
								{
									_asm
									{
										MOV ESI, SrcPP
										MOV EDI, DestPP
										MOV EAX, TransCode
									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_1:
										MOV ECX, Width
										SHR ECX, 1
										JZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_7
										
									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_2:
										MOVZX EBX, word ptr [ESI]
										MOV EDX, EBX
										CMP BL, AL
										JE LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_3
										SHR EBX, 8
										AND EDX, 0xff
										CMP BL, AL
										JE LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_4
										MOVZX EDX, word ptr [PaletteTemp + EDX * 2]
										MOVZX EBX, word ptr [PaletteTemp + EBX * 2]
										SHL EBX, 16
										OR EDX, EBX
										MOV EBX, [EDI]
										SHR EDX, 1
										SHR EBX, 1
										AND EDX, MEMIMG_R5G6B5_ALPHAMASK1
										AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
										ADD EDX, EBX
										MOV [EDI], EDX
										JMP LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_6

									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_3:
										SHR EBX, 8
										CMP BL, AL
										JE LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_6
										MOV BX, word ptr [PaletteTemp+EBX*2]
										MOV DX, [EDI+2]
										SHR EDX, 1
										SHR EBX, 1
										AND EDX, MEMIMG_R5G6B5_ALPHAMASK1
										AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
										ADD EDX, EBX
										MOV [EDI+2], DX
										JMP LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_6

									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_4:
										MOV BX, word ptr [PaletteTemp+EDX*2]
										MOV DX, [EDI]
										SHR EBX, 1
										SHR EDX, 1
										AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
										AND EDX, MEMIMG_R5G6B5_ALPHAMASK1
										ADD EDX, EBX
										MOV [EDI], DX

									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_6:
										ADD ESI, 2
										ADD EDI, 4
										DEC ECX
										JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_2
										
									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_7:
										TEST Width, 1
										JZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_10
										MOVZX EBX, BYTE PTR [ESI]
										CMP BL, AL
										JE LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_9
										MOV BX, word ptr [PaletteTemp+EBX*2]
										MOV DX, [EDI]
										SHR EDX, 1
										SHR EBX, 1
										AND EDX, MEMIMG_R5G6B5_ALPHAMASK1
										AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
										ADD EDX, EBX
										MOV [EDI], DX
									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_9:
										INC ESI
										ADD EDI, 2
										
									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_10:
										ADD ESI, SrcAddPitch
										ADD EDI, DestAddPitch
										DEC Height
										JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_TR_1
									}
								}
								else
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
										MOV EDX, RateTable16
										MOV EAX, SrcAddPitch
										MOV EBX, DestAddPitch
										MOV ECX, Width
										MOV srcAddPitch, EAX
										MOV destAddPitch, EBX
										MOV width, ECX
										MOV EAX, Height
										MOV EBX, TransCode
										MOV height, EAX
										MOV transcode, EBX
										PUSH EBP
										MOV EBP, EDX
										MOV height, EAX
									LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_TR_1:
										MOV ECX, width
										MOV counter, ECX
									LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_TR_2:
										MOVZX EAX, byte ptr [ESI]
										CMP AL, byte ptr transcode
										JE LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_TR_3
										MOVZX EDX, word ptr [EDI]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										MOV ECX, EAX
										AND EDX, 0xf800
										AND ECX, 0xf800
										SHR EDX, 11
										SHR ECX, 5
										ADD ECX, EDX
										MOVZX EDX, byte ptr [EBP + ECX]
										SHL EDX, 11

										MOVZX EBX, word ptr [EDI]
										MOV ECX, EAX
										AND EBX, 0x07e0
										AND ECX, 0x07e0
										SHR EBX, 5
										LEA ECX, [ECX * 2 + EBX]
										MOVZX EBX, byte ptr [EBP + ECX]
										SHL EBX, 5
										OR EDX, EBX

										MOVZX EBX, word ptr [EDI]
										AND EAX, 0x001f
										AND EBX, 0x001f
										SHL EAX, 6
										ADD EAX, EBX
										MOVZX EBX, byte ptr [EBP + EAX]
										OR EDX, EBX

										MOV [EDI], DX
									LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_TR_3:
										ADD EDI, 2
										INC ESI
										DEC counter
										JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_TR_2
										ADD ESI, srcAddPitch
										ADD EDI, destAddPitch
										DEC height
										JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_TR_1
										POP EBP
									}
								}
#endif
							}
							else
							{
#ifdef DX_NON_INLINE_ASM
								DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
#else
								if( MemImgManage.BlendParam == 128 )
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
										MOV EDX, Height
									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_NTR_1:
										MOV ECX, Width
										SHR ECX, 1
										JZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_NTR_3
									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_NTR_2:
										MOVZX EAX, byte ptr [ESI]
										MOVZX EBX, byte ptr [ESI+1]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										MOVZX EBX, word ptr [PaletteTemp + EBX * 2]
										SHL EBX, 16
										OR EAX, EBX
										MOV EBX, [EDI]
										SHR EBX, 1
										SHR EAX, 1
										AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
										AND EAX, MEMIMG_R5G6B5_ALPHAMASK1
										ADD EAX, EBX
										MOV [EDI], EAX
										ADD EDI, 4
										ADD ESI, 2
										DEC ECX
										JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_NTR_2

									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_NTR_3:
										TEST Width, 1
										JZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_NTR_4

										MOVZX EAX, byte ptr [ESI]
										MOV BX, [EDI]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										SHR BX, 1
										SHR AX, 1
										AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
										AND EAX, MEMIMG_R5G6B5_ALPHAMASK1
										ADD EAX, EBX
										MOV [EDI], AX
										ADD EDI, 2
										INC ESI

									LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_NTR_4:
										ADD ESI, SrcAddPitch
										ADD EDI, DestAddPitch
										DEC EDX
										JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_PAL_NTR_1
									}
								}
								else
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
										MOV EDX, RateTable16
										MOV EAX, SrcAddPitch
										MOV EBX, DestAddPitch
										MOV ECX, Width
										MOV srcAddPitch, EAX
										MOV destAddPitch, EBX
										MOV width, ECX
										MOV EAX, Height
										PUSH EBP
										MOV EBP, EDX
										MOV height, EAX
									LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_NTR_1:
										MOV ECX, width
										MOV counter, ECX
									LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_NTR_2:
										MOVZX EAX, byte ptr [ESI]
										MOVZX EDX, word ptr [EDI]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										MOV ECX, EAX
										AND EDX, 0xf800
										AND ECX, 0xf800
										SHR EDX, 11
										SHR ECX, 5
										ADD ECX, EDX
										MOVZX EDX, byte ptr [EBP + ECX]
										SHL EDX, 11

										MOVZX EBX, word ptr [EDI]
										MOV ECX, EAX
										AND EBX, 0x07e0
										AND ECX, 0x07e0
										SHR EBX, 5
										LEA ECX, [ECX * 2 + EBX]
										MOVZX EBX, byte ptr [EBP + ECX]
										SHL EBX, 5
										OR EDX, EBX

										MOVZX EBX, word ptr [EDI]
										AND EAX, 0x001f
										AND EBX, 0x001f
										SHL EAX, 6
										ADD EAX, EBX
										MOVZX EBX, byte ptr [EBP + EAX]
										OR EDX, EBX

										MOV [EDI], DX
										ADD EDI, 2
										INC ESI
										DEC counter
										JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_NTR_2
										ADD ESI, srcAddPitch
										ADD EDI, destAddPitch
										DEC height
										JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BAL_PAL_NTR_1
										POP EBP
									}
								}
#endif
							}
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									TransCode &= 0xffff ;
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
									LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_1:
										MOV ECX, Width
										SHR ECX, 1
										JZ LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_3
										MOV wtemp, ECX
									LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_2:
										MOVZX EAX, word ptr [ESI]
										CMP EAX, TransCode
										JE LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_5

										CMP AL, byte ptr TransCode
										JE LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_6
										MOV ECX, EAX
										SHR ECX, 8
										CMP CL, byte ptr TransCode
										JE LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_7
										AND EAX, 0xff
										MOVZX ECX, word ptr [PaletteTemp + ECX * 2]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										SHL ECX, 16
										OR EAX, ECX
										MOV EBX, [EDI]
										XOR EDX, EDX
										AND EAX, MEMIMG_R5G6B5_ADDMASK1
										AND EBX, MEMIMG_R5G6B5_ADDMASK1
										ADD EAX, EBX
										ADC EDX, EDX
										SHL EDX, 27
										MOV EBX, EAX
										AND EBX, MEMIMG_R5G6B5_ADDMASK2
										MOV ECX, EBX
										SHR EBX, MEMIMG_R5G6B5_ADDSHFT
										OR EBX, EDX
										SUB ECX, EBX
										OR ECX, EAX
										MOV [EDI], ECX
										JMP LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_5

									LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_6:
										SHR EAX, 8
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										MOV BX, [EDI+2]
										AND EAX, MEMIMG_R5G6B5_ADDMASK1
										AND EBX, MEMIMG_R5G6B5_ADDMASK1
										ADD EAX, EBX
										MOV EBX, EAX
										AND EBX, MEMIMG_R5G6B5_ADDMASK2
										MOV ECX, EBX
										SHR EBX, MEMIMG_R5G6B5_ADDSHFT
										SUB ECX, EBX
										OR ECX, EAX
										MOV [EDI+2], CX
										JMP LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_5

									LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_7:
										AND EAX, 0xff
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										MOV BX, [EDI]
										AND EAX, MEMIMG_R5G6B5_ADDMASK1
										AND EBX, MEMIMG_R5G6B5_ADDMASK1
										ADD EAX, EBX
										MOV EBX, EAX
										AND EBX, MEMIMG_R5G6B5_ADDMASK2
										MOV ECX, EBX
										SHR EBX, MEMIMG_R5G6B5_ADDSHFT
										SUB ECX, EBX
										OR ECX, EAX
										MOV [EDI], CX

									LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_5:
										ADD EDI, 4
										ADD ESI, 2
										DEC wtemp
										JNZ LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_2

									LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_3:
										TEST Width, 1
										JZ LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_4
										MOVZX EAX, byte ptr [ESI]
										CMP AL, byte ptr TransCode
										JE LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_8
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										MOV BX, [EDI]
										AND EAX, MEMIMG_R5G6B5_ADDMASK1
										AND EBX, MEMIMG_R5G6B5_ADDMASK1
										ADD EAX, EBX
										MOV EBX, EAX
										AND EBX, MEMIMG_R5G6B5_ADDMASK2
										MOV ECX, EBX
										SHR EBX, MEMIMG_R5G6B5_ADDSHFT
										SUB ECX, EBX
										OR ECX, EAX
										MOV [EDI], CX
									LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_8:
										ADD EDI, 2
										INC ESI
										
									LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_4:
										ADD EDI, DestAddPitch
										ADD ESI, SrcAddPitch
										DEC Height
										JNZ LABEL_NBI_C16_NBR_NAC_BAD_PAL_TR_1
									}
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
								}
							}
							else
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
									LABEL_NBI_C16_NBR_NAC_BAD_PAL_NTR_1:
										MOV ECX, Width
										SHR ECX, 1
										JZ LABEL_NBI_C16_NBR_NAC_BAD_PAL_NTR_3
										MOV wtemp, ECX
									LABEL_NBI_C16_NBR_NAC_BAD_PAL_NTR_2:
										MOVZX EAX, byte ptr [ESI]
										MOV EBX, [EDI]
										MOVZX ECX, byte ptr [ESI+1]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										XOR EDX, EDX
										MOVZX ECX, word ptr [PaletteTemp + ECX * 2]
										SHL ECX, 16
										OR EAX, ECX
										AND EAX, MEMIMG_R5G6B5_ADDMASK1
										AND EBX, MEMIMG_R5G6B5_ADDMASK1
										ADD EAX, EBX
										ADC EDX, EDX
										SHL EDX, 27
										MOV EBX, EAX
										AND EBX, MEMIMG_R5G6B5_ADDMASK2
										MOV ECX, EBX
										SHR EBX, MEMIMG_R5G6B5_ADDSHFT
										OR EBX, EDX
										SUB ECX, EBX
										OR ECX, EAX
										MOV [EDI], ECX

										ADD EDI, 4
										ADD ESI, 2
										DEC wtemp
										JNZ LABEL_NBI_C16_NBR_NAC_BAD_PAL_NTR_2

									LABEL_NBI_C16_NBR_NAC_BAD_PAL_NTR_3:
										TEST Width, 1
										JZ LABEL_NBI_C16_NBR_NAC_BAD_PAL_NTR_4
										MOVZX EAX, byte ptr [ESI]
										MOV BX, [EDI]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										AND EAX, MEMIMG_R5G6B5_ADDMASK1
										AND EBX, MEMIMG_R5G6B5_ADDMASK1
										ADD EAX, EBX
										MOV EBX, EAX
										AND EBX, MEMIMG_R5G6B5_ADDMASK2
										MOV ECX, EBX
										SHR EBX, MEMIMG_R5G6B5_ADDSHFT
										SUB ECX, EBX
										OR ECX, EAX
										MOV [EDI], CX

										ADD EDI, 2
										INC ESI

									LABEL_NBI_C16_NBR_NAC_BAD_PAL_NTR_4:
										ADD EDI, DestAddPitch
										ADD ESI, SrcAddPitch
										DEC Height
										JNZ LABEL_NBI_C16_NBR_NAC_BAD_PAL_NTR_1
									}
/*
									do{
										wtemp = Width ;
										do{
//											CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 )
											{
												Src1 = PaletteWP[ *SrcBP ] ;
												Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (*DestWP) & MEMIMG_R5G6B5_ADDMASK1 ) ;
												DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;
*(DestWP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) ) ;
											}
											SrcBP  += 1 ;
											DestBP += 2 ;
										}while( -- wtemp );
										SrcBP  += SrcAddPitch ;
										DestBP += DestAddPitch ;
									}while( -- Height );
*/
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
								}
							}
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									TransCode &= 0xffff ;
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
									LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_1:
										MOV ECX, Width
										SHR ECX, 1
										JZ LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_3
										MOV wtemp, ECX
									LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_2:
										MOVZX EAX, word ptr [ESI]
										CMP EAX, TransCode
										JE LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_5

										CMP AL, byte ptr TransCode
										JE LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_6
										MOV ECX, EAX
										SHR ECX, 8
										CMP CL, byte ptr TransCode
										JE LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_7
										AND EAX, 0xff
										MOVZX ECX, word ptr [PaletteTemp + ECX * 2]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										SHL ECX, 16
										OR EAX, ECX
										MOV EBX, [EDI]
										XOR EDX, EDX
										AND EAX, MEMIMG_R5G6B5_SUBMASK1
										OR  EBX, MEMIMG_R5G6B5_SUBMASK2
										SUB EBX, EAX
										ADC EDX, EDX
										DEC EDX
										MOV EAX, EBX
										AND EAX, MEMIMG_R5G6B5_SUBMASK2
										AND EDX, 0xf8000000
										MOV ECX, EAX
										SHR ECX, MEMIMG_R5G6B5_SUBSHFT
										SUB EAX, ECX
										OR EAX, EDX
										AND EBX, EAX
										MOV [EDI], EBX
										JMP LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_5

									LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_6:
										SHR EAX, 8
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										MOV BX, [EDI+2]
										AND EAX, MEMIMG_R5G6B5_SUBMASK1
										OR  EBX, MEMIMG_R5G6B5_SUBMASK2
										SUB EBX, EAX
										MOV EAX, EBX
										AND EAX, MEMIMG_R5G6B5_SUBMASK2
										MOV ECX, EAX
										SHR ECX, MEMIMG_R5G6B5_SUBSHFT
										SUB EAX, ECX
										AND EBX, EAX
										MOV [EDI+2], BX
										JMP LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_5

									LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_7:
										AND EAX, 0xff
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										MOV BX, [EDI]
										AND EAX, MEMIMG_R5G6B5_SUBMASK1
										OR  EBX, MEMIMG_R5G6B5_SUBMASK2
										SUB EBX, EAX
										MOV EAX, EBX
										AND EAX, MEMIMG_R5G6B5_SUBMASK2
										MOV ECX, EAX
										SHR ECX, MEMIMG_R5G6B5_SUBSHFT
										SUB EAX, ECX
										AND EBX, EAX
										MOV [EDI], BX

									LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_5:
										ADD EDI, 4
										ADD ESI, 2
										DEC wtemp
										JNZ LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_2

									LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_3:
										TEST Width, 1
										JZ LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_4
										MOVZX EAX, byte ptr [ESI]
										CMP AL, byte ptr TransCode
										JE LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_8
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										MOV BX, [EDI]
										AND EAX, MEMIMG_R5G6B5_SUBMASK1
										OR  EBX, MEMIMG_R5G6B5_SUBMASK2
										SUB EBX, EAX
										MOV EAX, EBX
										AND EAX, MEMIMG_R5G6B5_SUBMASK2
										MOV ECX, EAX
										SHR ECX, MEMIMG_R5G6B5_SUBSHFT
										SUB EAX, ECX
										AND EBX, EAX
										MOV [EDI], BX
									LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_8:
										ADD EDI, 2
										INC ESI
										
									LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_4:
										ADD EDI, DestAddPitch
										ADD ESI, SrcAddPitch
										DEC Height
										JNZ LABEL_NBI_C16_NBR_NAC_BSB_PAL_TR_1
									}
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
								}
							}
							else
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
									LABEL_NBI_C16_NBR_NAC_BSB_PAL_NTR_1:
										MOV ECX, Width
										SHR ECX, 1
										JZ LABEL_NBI_C16_NBR_NAC_BSB_PAL_NTR_3
										MOV wtemp, ECX
									LABEL_NBI_C16_NBR_NAC_BSB_PAL_NTR_2:
										MOVZX EAX, byte ptr [ESI]
										MOV EBX, [EDI]
										MOVZX ECX, byte ptr [ESI+1]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										XOR EDX, EDX
										MOVZX ECX, word ptr [PaletteTemp + ECX * 2]
										SHL ECX, 16
										OR EAX, ECX
										AND EAX, MEMIMG_R5G6B5_SUBMASK1
										OR  EBX, MEMIMG_R5G6B5_SUBMASK2
										SUB EBX, EAX
										ADC EDX, EDX
										DEC EDX
										MOV EAX, EBX
										AND EAX, MEMIMG_R5G6B5_SUBMASK2
										AND EDX, 0xf8000000
										MOV ECX, EAX
										SHR ECX, MEMIMG_R5G6B5_SUBSHFT
										SUB EAX, ECX
										OR EAX, EDX
										AND EBX, EAX
										MOV [EDI], EBX

										ADD EDI, 4
										ADD ESI, 2
										DEC wtemp
										JNZ LABEL_NBI_C16_NBR_NAC_BSB_PAL_NTR_2

									LABEL_NBI_C16_NBR_NAC_BSB_PAL_NTR_3:
										TEST Width, 1
										JZ LABEL_NBI_C16_NBR_NAC_BSB_PAL_NTR_4
										MOVZX EAX, byte ptr [ESI]
										MOV BX, [EDI]
										MOVZX EAX, word ptr [PaletteTemp + EAX * 2]
										AND EAX, MEMIMG_R5G6B5_SUBMASK1
										OR  EBX, MEMIMG_R5G6B5_SUBMASK2
										SUB EBX, EAX
										MOV EAX, EBX
										AND EAX, MEMIMG_R5G6B5_SUBMASK2
										MOV ECX, EAX
										SHR ECX, MEMIMG_R5G6B5_SUBSHFT
										SUB EAX, ECX
										AND EBX, EAX
										MOV [EDI], BX

										ADD EDI, 2
										INC ESI

									LABEL_NBI_C16_NBR_NAC_BSB_PAL_NTR_4:
										ADD EDI, DestAddPitch
										ADD ESI, SrcAddPitch
										DEC Height
										JNZ LABEL_NBI_C16_NBR_NAC_BSB_PAL_NTR_1
									}
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
								}
							}
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
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
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )
								{
#ifndef DX_NON_INLINE_ASM
									_asm
									{
										PUSHF
										CLD
										MOV ESI, SrcPP
										MOV EDI, DestPP
										MOV EDX, TransCode
										MOV EBX, Width

									LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_1:
										MOV ECX, EBX
										SHR ECX, 1
										JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_6
									LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_2:
										MOV EAX, [ESI]
										CMP EAX, EDX
										JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3
										
										CMP AX, DX
										JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_4
										MOV [EDI], AX
										SHR EAX, 16
										CMP AX, DX
										JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3
										MOV [EDI+2], AX
										JMP LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3
		
									LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_4:
										SHR EAX, 16
										MOV [EDI+2], AX
										
									LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3:
										ADD ESI, 4
										ADD EDI, 4
										DEC ECX
										JNZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_2
										
									LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_6:
										TEST EBX, 1
										JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_5
										MOV AX, [ESI]
										ADD ESI, 2
										ADD EDI, 2
										CMP AX, DX
										JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_5
										MOV [EDI-2], AX
										
									LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_5:
										ADD EDI, DestAddPitch
										ADD ESI, SrcAddPitch
										DEC Height
										JNZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_1
										
										POPF
									}
#else
									DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
#endif
								}
								else
								{
#ifndef DX_NON_INLINE_ASM
									_asm
									{
										PUSHF
										CLD
										MOV ESI, SrcPP
										MOV EDI, DestPP
										MOV EBX, Height
										MOV EDX, Width
										MOV EAX, DestAddPitch
									LABEL_NFAST_NBI_C16_NBR_NAC_BNO_NTR_1:
										MOV ECX, EDX
										SHR ECX, 1
										JZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_NTR_2
										REP MOVSD
									LABEL_NFAST_NBI_C16_NBR_NAC_BNO_NTR_2:
										TEST EDX, 1
										JZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_NTR_3
										MOVSW
									LABEL_NFAST_NBI_C16_NBR_NAC_BNO_NTR_3:
										ADD EDI, EAX
										ADD ESI, SrcAddPitch
										DEC EBX
										JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BNO_NTR_1
										POPF
									}
#else
									DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
#endif
								}
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
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
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )
								{
#ifdef DX_NON_INLINE_ASM
									DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
#else
									if( MemImgManage.BlendParam == 128 )
									{
										_asm
										{
											MOV ESI, SrcPP
											MOV EDI, DestPP
											MOV EAX, TransCode
										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_1:
											MOV ECX, Width
											SHR ECX, 1
											JZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_7
											
										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_2:
											MOV EBX, [ESI]
											MOV EDX, EBX
											CMP BX, AX
											JE LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_3
											SHR EBX, 16
											CMP BX, AX
											JE LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_4
											MOV EBX, [EDI]
											SHR EDX, 1
											SHR EBX, 1
											AND EDX, MEMIMG_R5G6B5_ALPHAMASK1
											AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
											ADD EDX, EBX
											MOV [EDI], EDX
											JMP LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_6

										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_3:
											SHR EBX, 16
											CMP BX, AX
											JE LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_6
											MOV DX, [EDI+2]
											SHR EDX, 1
											SHR EBX, 1
											AND EDX, MEMIMG_R5G6B5_ALPHAMASK1
											AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
											ADD EDX, EBX
											MOV [EDI+2], DX
											JMP LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_6

										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_4:
											MOV BX, [EDI]
											SHR EDX, 1
											SHR EBX, 1
											AND EDX, MEMIMG_R5G6B5_ALPHAMASK1
											AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
											ADD EDX, EBX
											MOV [EDI], DX

										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_6:
											ADD ESI, 4
											ADD EDI, 4
											DEC ECX
											JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_2
											
										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_7:
											TEST Width, 1
											JZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_10
											MOV BX, [ESI]
											CMP BX, AX
											JE LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_9
											MOV DX, [EDI]
											SHR EDX, 1
											SHR EBX, 1
											AND EDX, MEMIMG_R5G6B5_ALPHAMASK1
											AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
											ADD EDX, EBX
											MOV [EDI], DX
										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_9:
											ADD ESI, 2
											ADD EDI, 2
											
										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_10:
											ADD ESI, SrcAddPitch
											ADD EDI, DestAddPitch
											DEC Height
											JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_TR_1
										}
									}
									else
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
											MOV EDX, RateTable16
											MOV EAX, SrcAddPitch
											MOV EBX, DestAddPitch
											MOV ECX, Width
											MOV srcAddPitch, EAX
											MOV destAddPitch, EBX
											MOV width, ECX
											MOV EAX, Height
											MOV EBX, TransCode
											MOV height, EAX
											MOV transcode, EBX
											PUSH EBP
											MOV EBP, EDX
										LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_TR_1:
											MOV ECX, width
											MOV counter, ECX
										LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_TR_2:
											MOVZX EAX, word ptr [ESI]
											CMP AX, word ptr transcode
											JE LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_TR_3
											MOVZX EDX, word ptr [EDI]
											MOV ECX, EAX
											AND EDX, 0xf800
											AND ECX, 0xf800
											SHR EDX, 11
											SHR ECX, 5
											ADD ECX, EDX
											MOVZX EDX, byte ptr [EBP + ECX]
											SHL EDX, 11

											MOVZX EBX, word ptr [EDI]
											MOV ECX, EAX
											AND EBX, 0x07e0
											AND ECX, 0x07e0
											SHR EBX, 5
											LEA ECX, [ECX * 2 + EBX]
											MOVZX EBX, byte ptr [EBP + ECX]
											SHL EBX, 5
											OR EDX, EBX

											MOVZX EBX, word ptr [EDI]
											AND EAX, 0x001f
											AND EBX, 0x001f
											SHL EAX, 6
											ADD EAX, EBX
											MOVZX EBX, byte ptr [EBP + EAX]
											OR EDX, EBX

											MOV [EDI], DX
										LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_TR_3:
											ADD EDI, 2
											ADD ESI, 2
											DEC counter
											JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_TR_2
											ADD ESI, srcAddPitch
											ADD EDI, destAddPitch
											DEC height
											JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_TR_1
											POP EBP
										}
									}
#endif
								}
								else
								{
#ifdef DX_NON_INLINE_ASM
									DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
#else
									if( MemImgManage.BlendParam == 128 )
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
											MOV EDX, Height
										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_NTR_1:
											MOV ECX, Width
											SHR ECX, 1
											JZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_NTR_3
										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_NTR_2:
											MOV EAX, [ESI]
											MOV EBX, [EDI]
											SHR EAX, 1
											SHR EBX, 1
											AND EAX, MEMIMG_R5G6B5_ALPHAMASK1
											AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
											ADD EAX, EBX
											MOV [EDI], EAX
											ADD EDI, 4
											ADD ESI, 4
											DEC ECX
											JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_NTR_2

										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_NTR_3:
											TEST Width, 1
											JZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_NTR_4

											MOV AX, [ESI]
											MOV BX, [EDI]
											SHR BX, 1
											SHR AX, 1
											AND EBX, MEMIMG_R5G6B5_ALPHAMASK1
											AND EAX, MEMIMG_R5G6B5_ALPHAMASK1
											ADD EAX, EBX
											MOV [EDI], AX
											ADD EDI, 2
											ADD ESI, 2

										LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_NTR_4:
											ADD ESI, SrcAddPitch
											ADD EDI, DestAddPitch
											DEC EDX
											JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BALH_NPAL_NTR_1
										}
									}
									else
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
											MOV EDX, RateTable16
											MOV EAX, SrcAddPitch
											MOV EBX, DestAddPitch
											MOV ECX, Width
											MOV srcAddPitch, EAX
											MOV destAddPitch, EBX
											MOV width, ECX
											MOV EAX, Height
											PUSH EBP
											MOV EBP, EDX
											MOV height, EAX
										LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_NTR_1:
											MOV ECX, width
											MOV counter, ECX
										LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_NTR_2:
											MOVZX EAX, word ptr [ESI]
											MOVZX EDX, word ptr [EDI]
											MOV ECX, EAX
											AND EDX, 0xf800
											AND ECX, 0xf800
											SHR EDX, 11
											SHR ECX, 5
											ADD ECX, EDX
											MOVZX EDX, byte ptr [EBP + ECX]
											SHL EDX, 11

											MOVZX EBX, word ptr [EDI]
											MOV ECX, EAX
											AND EBX, 0x07e0
											AND ECX, 0x07e0
											SHR EBX, 5
											LEA ECX, [ECX * 2 + EBX]
											MOVZX EBX, byte ptr [EBP + ECX]
											SHL EBX, 5
											OR EDX, EBX

											MOVZX EBX, word ptr [EDI]
											AND EAX, 0x001f
											AND EBX, 0x001f
											SHL EAX, 6
											ADD EAX, EBX
											MOVZX EBX, byte ptr [EBP + EAX]
											OR EDX, EBX

											MOV [EDI], DX
											ADD EDI, 2
											ADD ESI, 2
											DEC counter
											JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_NTR_2
											ADD ESI, srcAddPitch
											ADD EDI, destAddPitch
											DEC height
											JNZ LABEL_NFAST_NBI_C16_NBR_NAC_BAL_NPAL_NTR_1
											POP EBP
										}
									}
#endif
								}
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 255 )
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_1:
											MOV ECX, Width
											SHR ECX, 1
											JZ LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_3
											MOV wtemp, ECX
										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_2:
											MOV EAX, [ESI]
											CMP EAX, TransCode
											JE LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_5

											CMP AX, word ptr TransCode
											JE LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_6
											MOV ECX, EAX
											SHR ECX, 16
											CMP CX, word ptr TransCode
											JE LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_7

											MOV EBX, [EDI]
											XOR EDX, EDX
											AND EAX, MEMIMG_R5G6B5_ADDMASK1
											AND EBX, MEMIMG_R5G6B5_ADDMASK1
											ADD EAX, EBX
											ADC EDX, EDX
											SHL EDX, 27
											MOV EBX, EAX
											AND EBX, MEMIMG_R5G6B5_ADDMASK2
											MOV ECX, EBX
											SHR EBX, MEMIMG_R5G6B5_ADDSHFT
											OR EBX, EDX
											SUB ECX, EBX
											OR ECX, EAX
											MOV [EDI], ECX
											JMP LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_5

										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_6:
											SHR EAX, 16
											MOV BX, [EDI+2]
											AND EAX, MEMIMG_R5G6B5_ADDMASK1
											AND EBX, MEMIMG_R5G6B5_ADDMASK1
											ADD EAX, EBX
											MOV EBX, EAX
											AND EBX, MEMIMG_R5G6B5_ADDMASK2
											MOV ECX, EBX
											SHR EBX, MEMIMG_R5G6B5_ADDSHFT
											SUB ECX, EBX
											OR ECX, EAX
											MOV [EDI+2], CX
											JMP LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_5

										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_7:
											MOV BX, [EDI]
											AND EAX, MEMIMG_R5G6B5_ADDMASK1
											AND EBX, MEMIMG_R5G6B5_ADDMASK1
											ADD EAX, EBX
											MOV EBX, EAX
											AND EBX, MEMIMG_R5G6B5_ADDMASK2
											MOV ECX, EBX
											SHR EBX, MEMIMG_R5G6B5_ADDSHFT
											SUB ECX, EBX
											OR ECX, EAX
											MOV [EDI], CX

										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_5:
											ADD EDI, 4
											ADD ESI, 4
											DEC wtemp
											JNZ LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_2

										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_3:
											TEST Width, 1
											JZ LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_4
											MOV AX, [ESI]
											CMP AX, word ptr TransCode
											JE LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_8
											MOV BX, [EDI]
											AND EAX, MEMIMG_R5G6B5_ADDMASK1
											AND EBX, MEMIMG_R5G6B5_ADDMASK1
											ADD EAX, EBX
											MOV EBX, EAX
											AND EBX, MEMIMG_R5G6B5_ADDMASK2
											MOV ECX, EBX
											SHR EBX, MEMIMG_R5G6B5_ADDSHFT
											SUB ECX, EBX
											OR ECX, EAX
											MOV [EDI], CX
										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_8:
											ADD EDI, 2
											ADD ESI, 2
											
										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_4:
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C16_NBR_NAC_BAD_NPAL_TR_1
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
									}
								}
								else
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 255 )
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_NTR_1:
											MOV ECX, Width
											SHR ECX, 1
											JZ LABEL_NBI_C16_NBR_NAC_BAD_NPAL_NTR_3
											MOV wtemp, ECX
										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_NTR_2:
											MOV EAX, [ESI]
											MOV EBX, [EDI]
											XOR EDX, EDX
											AND EAX, MEMIMG_R5G6B5_ADDMASK1
											AND EBX, MEMIMG_R5G6B5_ADDMASK1
											ADD EAX, EBX
											ADC EDX, EDX
											SHL EDX, 27
											MOV EBX, EAX
											AND EBX, MEMIMG_R5G6B5_ADDMASK2
											MOV ECX, EBX
											SHR EBX, MEMIMG_R5G6B5_ADDSHFT
											OR EBX, EDX
											SUB ECX, EBX
											OR ECX, EAX
											MOV [EDI], ECX

											ADD EDI, 4
											ADD ESI, 4
											DEC wtemp
											JNZ LABEL_NBI_C16_NBR_NAC_BAD_NPAL_NTR_2

										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_NTR_3:
											TEST Width, 1
											JZ LABEL_NBI_C16_NBR_NAC_BAD_NPAL_NTR_4
											MOV AX, [ESI]
											MOV BX, [EDI]
											AND EAX, MEMIMG_R5G6B5_ADDMASK1
											AND EBX, MEMIMG_R5G6B5_ADDMASK1
											ADD EAX, EBX
											MOV EBX, EAX
											AND EBX, MEMIMG_R5G6B5_ADDMASK2
											MOV ECX, EBX
											SHR EBX, MEMIMG_R5G6B5_ADDSHFT
											SUB ECX, EBX
											OR ECX, EAX
											MOV [EDI], CX
											ADD EDI, 2
											ADD ESI, 2
											
										LABEL_NBI_C16_NBR_NAC_BAD_NPAL_NTR_4:
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C16_NBR_NAC_BAD_NPAL_NTR_1
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
									}
								}
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 255 )
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_1:
											MOV ECX, Width
											SHR ECX, 1
											JZ LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_3
											MOV wtemp, ECX
										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_2:
											MOV EAX, [ESI]
											CMP EAX, TransCode
											JE LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_5

											CMP AX, word ptr TransCode
											JE LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_6
											MOV ECX, EAX
											SHR ECX, 16
											CMP CX, word ptr TransCode
											JE LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_7
											MOV EBX, [EDI]
											XOR EDX, EDX
											AND EAX, MEMIMG_R5G6B5_SUBMASK1
											OR  EBX, MEMIMG_R5G6B5_SUBMASK2
											SUB EBX, EAX
											ADC EDX, EDX
											DEC EDX
											MOV EAX, EBX
											AND EAX, MEMIMG_R5G6B5_SUBMASK2
											AND EDX, 0xf8000000
											MOV ECX, EAX
											SHR ECX, MEMIMG_R5G6B5_SUBSHFT
											SUB EAX, ECX
											OR EAX, EDX
											AND EBX, EAX
											MOV [EDI], EBX
											JMP LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_5

										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_6:
											SHR EAX, 16
											MOV BX, [EDI+2]
											AND EAX, MEMIMG_R5G6B5_SUBMASK1
											OR  EBX, MEMIMG_R5G6B5_SUBMASK2
											SUB EBX, EAX
											MOV EAX, EBX
											AND EAX, MEMIMG_R5G6B5_SUBMASK2
											MOV ECX, EAX
											SHR ECX, MEMIMG_R5G6B5_SUBSHFT
											SUB EAX, ECX
											AND EBX, EAX
											MOV [EDI+2], BX
											JMP LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_5

										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_7:
											MOV BX, [EDI]
											AND EAX, MEMIMG_R5G6B5_SUBMASK1
											OR  EBX, MEMIMG_R5G6B5_SUBMASK2
											SUB EBX, EAX
											MOV EAX, EBX
											AND EAX, MEMIMG_R5G6B5_SUBMASK2
											MOV ECX, EAX
											SHR ECX, MEMIMG_R5G6B5_SUBSHFT
											SUB EAX, ECX
											AND EBX, EAX
											MOV [EDI], BX

										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_5:
											ADD EDI, 4
											ADD ESI, 4
											DEC wtemp
											JNZ LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_2

										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_3:
											TEST Width, 1
											JZ LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_4
											MOV AX, [ESI]
											CMP AX, word ptr TransCode
											JE LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_8
											MOV BX, [EDI]
											AND EAX, MEMIMG_R5G6B5_SUBMASK1
											OR  EBX, MEMIMG_R5G6B5_SUBMASK2
											SUB EBX, EAX
											MOV EAX, EBX
											AND EAX, MEMIMG_R5G6B5_SUBMASK2
											MOV ECX, EAX
											SHR ECX, MEMIMG_R5G6B5_SUBSHFT
											SUB EAX, ECX
											AND EBX, EAX
											MOV [EDI], BX
										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_8:
											ADD EDI, 2
											ADD ESI, 2
											
										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_4:
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C16_NBR_NAC_BSB_NPAL_TR_1
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
									}
								}
								else
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 255 )
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_NTR_1:
											MOV ECX, Width
											SHR ECX, 1
											JZ LABEL_NBI_C16_NBR_NAC_BSB_NPAL_NTR_3
											MOV wtemp, ECX
										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_NTR_2:
											MOV EAX, [ESI]
											MOV EBX, [EDI]
											XOR EDX, EDX
											AND EAX, MEMIMG_R5G6B5_SUBMASK1
											OR  EBX, MEMIMG_R5G6B5_SUBMASK2
											SUB EBX, EAX
											ADC EDX, EDX
											DEC EDX
											MOV EAX, EBX
											AND EAX, MEMIMG_R5G6B5_SUBMASK2
											AND EDX, 0xf8000000
											MOV ECX, EAX
											SHR ECX, MEMIMG_R5G6B5_SUBSHFT
											SUB EAX, ECX
											OR EAX, EDX
											AND EBX, EAX
											MOV [EDI], EBX

											ADD EDI, 4
											ADD ESI, 4
											DEC wtemp
											JNZ LABEL_NBI_C16_NBR_NAC_BSB_NPAL_NTR_2

										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_NTR_3:
											TEST Width, 1
											JZ LABEL_NBI_C16_NBR_NAC_BSB_NPAL_NTR_4
											MOV AX, [ESI]
											MOV BX, [EDI]
											AND EAX, MEMIMG_R5G6B5_SUBMASK1
											OR  EBX, MEMIMG_R5G6B5_SUBMASK2
											SUB EBX, EAX
											MOV EAX, EBX
											AND EAX, MEMIMG_R5G6B5_SUBMASK2
											MOV ECX, EAX
											SHR ECX, MEMIMG_R5G6B5_SUBSHFT
											SUB EAX, ECX
											AND EBX, EAX
											MOV [EDI], BX

											ADD ESI, 2
											ADD EDI, 2

										LABEL_NBI_C16_NBR_NAC_BSB_NPAL_NTR_4:
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C16_NBR_NAC_BSB_NPAL_NTR_1
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
									}
								}
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
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
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )
							{
#ifdef DX_NON_INLINE_ASM
								DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
#else
								_asm
								{
									MOV ESI, SrcPP
									MOV EDI, DestPP
									MOV EDX, PalletPP
									MOV EAX, TransCode
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_1:
									MOV ECX, Width
									SHR ECX, 2
									JZ LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_7
									
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_2:
									MOV EBX, [ESI]
									CMP EBX, EAX
									JE LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_6

									MOVZX EBX, BYTE PTR [ESI]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_3
									MOV EBX, [EDX+EBX*4]
									MOV [EDI], EBX
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_3:
									MOVZX EBX, BYTE PTR [ESI+1]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_4
									MOV EBX, [EDX+EBX*4]
									MOV [EDI+4], EBX
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_4:
									MOVZX EBX, BYTE PTR [ESI+2]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_5
									MOV EBX, [EDX+EBX*4]
									MOV [EDI+8], EBX
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_5:
									MOVZX EBX, BYTE PTR [ESI+3]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_6
									MOV EBX, [EDX+EBX*4]
									MOV [EDI+12], EBX
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_6:
									ADD ESI, 4
									ADD EDI, 16
									DEC ECX
									JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_2
									
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_7:
									MOV ECX, Width
									AND ECX, 3
									JZ LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_10
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_8:
									MOVZX EBX, BYTE PTR [ESI]
									CMP BL, AL
									JE LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_9
									MOV EBX, [EDX+EBX*4]
									MOV [EDI], EBX
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_9:
									INC ESI
									ADD EDI, 4
									DEC ECX
									JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_8
									
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_10:
									ADD ESI, SrcAddPitch
									ADD EDI, DestAddPitch
									DEC Height
									JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_TR_1
								}
#endif
							}
							else
							{
#ifdef DX_NON_INLINE_ASM
								DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
#else
								_asm
								{
									MOV ESI, SrcPP
									MOV EDI, DestPP
									MOV EDX, PalletPP
									MOV EAX, Height
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_NTR_1:
									MOV ECX, Width
								LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_NTR_2:
									MOVZX EBX, BYTE PTR [ESI]
									MOV EBX, [EDX+EBX*4]
									MOV [EDI], EBX
									INC ESI
									ADD EDI, 4
									DEC ECX
									JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_NTR_2
									ADD ESI, SrcAddPitch
									ADD EDI, DestAddPitch
									DEC EAX
									JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BNO_PAL_NTR_1
								}
#endif
							}
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 128 )
								{
									_asm
									{
										MOV ESI, SrcPP
										MOV EDI, DestPP
										MOV EAX, TransCode
									LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_TR_1:
										MOV ECX, Width
									LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_TR_2:
										MOVZX EBX, byte ptr [ESI]
										CMP BL, AL
										JE LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_TR_3
										MOV EBX, [PaletteTemp + EBX * 4]
										MOV EDX, [EDI]
										SHR EBX, 1
										SHR EDX, 1
										AND EBX, MEMIMG_XRGB8_ALPHAMASK1
										AND EDX, MEMIMG_XRGB8_ALPHAMASK1
										ADD EDX, EBX
										MOV [EDI], EDX
									LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_TR_3:
										INC ESI
										ADD EDI, 4
										DEC ECX
										JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_TR_2
										ADD ESI, SrcAddPitch
										ADD EDI, DestAddPitch
										DEC Height
										JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_TR_1
									}
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
								}
							}
							else
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 128 )
								{
									_asm
									{
										MOV ESI, SrcPP
										MOV EDI, DestPP
										MOV EAX, TransCode
									LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_NTR_1:
										MOV ECX, Width
									LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_NTR_2:
										MOVZX EBX, byte ptr [ESI]
										MOV EBX, [PaletteTemp + EBX * 4]
										MOV EDX, [EDI]
										SHR EBX, 1
										SHR EDX, 1
										AND EBX, MEMIMG_XRGB8_ALPHAMASK1
										AND EDX, MEMIMG_XRGB8_ALPHAMASK1
										ADD EDX, EBX
										MOV [EDI], EDX
										INC ESI
										ADD EDI, 4
										DEC ECX
										JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_NTR_2
										ADD ESI, SrcAddPitch
										ADD EDI, DestAddPitch
										DEC Height
										JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_PAL_NTR_1
									}
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
								}
							}
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									if( WinData.UseMMXFlag == TRUE )
									{
										TransCode &= 0xffff ;
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_1:
											MOV EDX, Width
											SHR EDX, 1
											JZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_3
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_2:
											MOVZX EAX, word ptr [ESI]
											CMP EAX, TransCode
											JE LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_5

											CMP AL, byte ptr TransCode
											JE LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_6
											MOV ECX, EAX
											SHR ECX, 8
											CMP CL, byte ptr TransCode
											JE LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_7
											AND EAX, 0xff
											MOV ECX, [PaletteTemp + ECX * 4]
											MOV EAX, [PaletteTemp + EAX * 4]
											MOVD MM0, ECX
											MOVD MM1, EAX
											PSLLQ MM0, 32
											POR MM0, MM1
											MOVQ MM1, [EDI]
											PADDUSB MM0, MM1
											MOVQ [EDI], MM0
											JMP LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_5

										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_6:
											SHR EAX, 8
											MOVD MM0, [PaletteTemp + EAX * 4]
											MOVD MM1, [EDI+4]
											PADDUSB MM0, MM1
											MOVD [EDI+4], MM0
											JMP LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_5

										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_7:
											AND EAX, 0xff
											MOVD MM0, [PaletteTemp + EAX * 4]
											MOVD MM1, [EDI]
											PADDUSB MM0, MM1
											MOVD [EDI], MM0

										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_5:
											ADD EDI, 8
											ADD ESI, 2
											DEC EDX
											JNZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_2

										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_3:
											TEST Width, 1
											JZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_4
											MOVZX EAX, byte ptr [ESI]
											CMP AL, byte ptr TransCode
											JE LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_8
											MOVD MM0, [PaletteTemp + EAX * 4]
											MOVD MM1, [EDI]
											PADDUSB MM0, MM1
											MOVD [EDI], MM0
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_8:
											ADD EDI, 4
											INC ESI
											
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_4:
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_MMX_1
											EMMS
										}
									}
									else
									{
										TransCode &= 0xff ;
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_1:
											MOV EDX, Width
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_2:
											MOVZX EAX, byte ptr [ESI]
											CMP AL, byte ptr TransCode
											JE LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_3
											MOV EAX, [PaletteTemp + EAX * 4]
											MOV EBX, [EDI]
											AND EAX, MEMIMG_XRGB8_ADDMASK1
											AND EBX, MEMIMG_XRGB8_ADDMASK1
											ADD EAX, EBX
											MOV EBX, EAX
											AND EBX, MEMIMG_XRGB8_ADDMASK2
											MOV ECX, EBX
											SHR EBX, MEMIMG_XRGB8_ADDSHFT
											SUB ECX, EBX
											OR ECX, EAX
											MOV [EDI], ECX
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_3:
											ADD EDI, 4
											INC ESI
											DEC EDX
											JNZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_2
											
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_TR_1
										}
									}
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
								}
							}
							else
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									if( WinData.UseMMXFlag == TRUE )
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
											MOV EDX, Height
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_1:
											MOV ECX, Width
											SHR ECX, 2
											JZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_3
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_2:
											MOVZX EAX, byte ptr [ESI]
											MOVQ MM2, [EDI]
											MOVZX EBX, byte ptr [ESI+1]
											MOVD MM0, [PaletteTemp + EAX * 4]
											MOVD MM1, [PaletteTemp + EBX * 4]
											PSLLQ MM1, 32
											POR MM0, MM1

											MOVZX EAX, byte ptr [ESI+2]
											MOVQ MM5, [EDI+8]
											MOVZX EBX, byte ptr [ESI+3]
											MOVD MM3, [PaletteTemp + EAX * 4]
											MOVD MM4, [PaletteTemp + EBX * 4]
											PSLLQ MM4, 32
											POR MM3, MM4

											PADDUSB MM0, MM2
											PADDUSB MM3, MM5
											MOVQ [EDI],   MM0
											MOVQ [EDI+8], MM3
											ADD EDI, 16
											ADD ESI, 4
											DEC ECX
											JNZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_2

										LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_3:
											MOV ECX, Width
											AND ECX, 3
											JZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_4
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_5:
											MOVZX EAX, byte ptr [ESI]
											MOVD MM1, [EDI]
											MOVD MM0, [PaletteTemp + EAX * 4]
											PADDUSB MM0, MM1
											MOVD [EDI], MM0
											ADD EDI, 4
											INC ESI
											DEC ECX
											JNZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_5

										LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_4:
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC EDX
											JNZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_MMX_1
											EMMS
										}
									}
									else
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_1:
											MOV EDX, Width
										LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_2:
											MOVZX EAX, byte ptr [ESI]
											MOV EBX, [EDI]
											MOV EAX, [PaletteTemp + EAX * 4]
											AND EAX, MEMIMG_XRGB8_ADDMASK1
											AND EBX, MEMIMG_XRGB8_ADDMASK1
											ADD EAX, EBX
											MOV EBX, EAX
											AND EBX, MEMIMG_XRGB8_ADDMASK2
											MOV ECX, EBX
											SHR EBX, MEMIMG_XRGB8_ADDSHFT
											SUB ECX, EBX
											OR ECX, EAX
											MOV [EDI], ECX
											ADD EDI, 4
											INC ESI
											DEC EDX
											JNZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_2

											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C32_NBR_NAC_BAD_PAL_NTR_1
										}
									}
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
								}
							}
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									if( WinData.UseMMXFlag == TRUE )
									{
										TransCode &= 0xffff ;
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_1:
											MOV EDX, Width
											SHR EDX, 1
											JZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_3
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_2:
											MOVZX EAX, word ptr [ESI]
											CMP EAX, TransCode
											JE LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_5

											CMP AL, byte ptr TransCode
											JE LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_6
											MOV ECX, EAX
											SHR ECX, 8
											CMP CL, byte ptr TransCode
											JE LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_7
											AND EAX, 0xff
											MOV ECX, [PaletteTemp + ECX * 4]
											MOV EAX, [PaletteTemp + EAX * 4]
											MOVD MM0, ECX
											MOVD MM1, EAX
											PSLLQ MM0, 32
											POR MM0, MM1
											MOVQ MM1, [EDI]
											PSUBUSB MM1, MM0
											MOVQ [EDI], MM1
											JMP LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_5

										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_6:
											SHR EAX, 8
											MOVD MM0, [PaletteTemp + EAX * 4]
											MOVD MM1, [EDI+4]
											PSUBUSB MM1, MM0
											MOVD [EDI+4], MM1
											JMP LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_5

										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_7:
											AND EAX, 0xff
											MOVD MM0, [PaletteTemp + EAX * 4]
											MOVD MM1, [EDI]
											PSUBUSB MM1, MM0
											MOVD [EDI], MM1

										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_5:
											ADD EDI, 8
											ADD ESI, 2
											DEC EDX
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_2

										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_3:
											TEST Width, 1
											JZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_4
											MOVZX EAX, byte ptr [ESI]
											CMP AL, byte ptr TransCode
											JE LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_8
											MOVD MM0, [PaletteTemp + EAX * 4]
											MOVD MM1, [EDI]
											PSUBUSB MM1, MM0
											MOVD [EDI], MM1
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_8:
											ADD EDI, 4
											INC ESI
											
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_4:
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_MMX_1
											EMMS
										}
									}
									else
									{
										TransCode &= 0xff ;
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_1:
											MOV EDX, Width
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_2:
											MOVZX EAX, byte ptr [ESI]
											CMP AL, byte ptr TransCode
											JE LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_3
											MOV EAX, [PaletteTemp + EAX * 4]
											MOV EBX, [EDI]
											AND EAX, MEMIMG_XRGB8_SUBMASK1
											OR  EBX, MEMIMG_XRGB8_SUBMASK2
											SUB EBX, EAX
											MOV EAX, EBX
											AND EAX, MEMIMG_XRGB8_SUBMASK2
											MOV ECX, EAX
											SHR ECX, MEMIMG_XRGB8_SUBSHFT
											SUB EAX, ECX
											AND EBX, EAX
											MOV [EDI], EBX
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_3:
											ADD EDI, 4
											INC ESI
											DEC EDX
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_2
											
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_TR_1
										}
									}
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
								}
							}
							else
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									if( WinData.UseMMXFlag == TRUE )
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
											MOV EDX, Height
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_1:
											MOV ECX, Width
											SHR ECX, 2
											JZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_3
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_2:
											MOVZX EAX, byte ptr [ESI]
											MOVQ MM2, [EDI]
											MOVZX EBX, byte ptr [ESI+1]
											MOVD MM0, [PaletteTemp + EAX * 4]
											MOVD MM1, [PaletteTemp + EBX * 4]
											PSLLQ MM1, 32
											POR MM0, MM1

											MOVZX EAX, byte ptr [ESI+2]
											MOVQ MM5, [EDI+8]
											MOVZX EBX, byte ptr [ESI+3]
											MOVD MM3, [PaletteTemp + EAX * 4]
											MOVD MM4, [PaletteTemp + EBX * 4]
											PSLLQ MM4, 32
											POR MM3, MM4

											PSUBUSB MM2, MM0
											PSUBUSB MM5, MM3
											MOVQ [EDI],   MM2
											MOVQ [EDI+8], MM5
											ADD EDI, 16
											ADD ESI, 4
											DEC ECX
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_2

										LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_3:
											MOV ECX, Width
											AND ECX, 3
											JZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_4
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_5:
											MOVZX EAX, byte ptr [ESI]
											MOVD MM1, [EDI]
											MOVD MM0, [PaletteTemp + EAX * 4]
											PSUBUSB MM1, MM0
											MOVD [EDI], MM1
											ADD EDI, 4
											INC ESI
											DEC ECX
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_5

										LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_4:
											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC EDX
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_MMX_1
											EMMS
										}
									}
									else
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_1:
											MOV EDX, Width
										LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_2:
											MOVZX EAX, byte ptr [ESI]
											MOV EBX, [EDI]
											MOV EAX, [PaletteTemp + EAX * 4]
											AND EAX, MEMIMG_XRGB8_SUBMASK1
											OR  EBX, MEMIMG_XRGB8_SUBMASK2
											SUB EBX, EAX
											MOV EAX, EBX
											AND EAX, MEMIMG_XRGB8_SUBMASK2
											MOV ECX, EAX
											SHR ECX, MEMIMG_XRGB8_SUBSHFT
											SUB EAX, ECX
											AND EBX, EAX
											MOV [EDI], EBX
											ADD EDI, 4
											INC ESI
											DEC EDX
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_2

											ADD EDI, DestAddPitch
											ADD ESI, SrcAddPitch
											DEC Height
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_PAL_NTR_1
										}
									}
								}
								else
#endif
								{
									DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
								}
							}
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
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
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )
								{
#ifndef DX_NON_INLINE_ASM
									_asm
									{
										MOV ESI, SrcPP
										MOV EDI, DestPP
										MOV EDX, TransCode
										MOV EBX, Width

									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_1:
										MOV ECX, EBX
										SHR ECX, 2
										JZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_7
										
									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_2:
										MOV EAX, [ESI]
										CMP EAX, EDX
										JZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_3
										MOV [EDI], EAX
									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_3:
										MOV EAX, [ESI+4]
										CMP EAX, EDX
										JZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_4
										MOV [EDI+4], EAX
									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_4:
										MOV EAX, [ESI+8]
										CMP EAX, EDX
										JZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_5
										MOV [EDI+8], EAX
									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_5:
										MOV EAX, [ESI+12]
										CMP EAX, EDX
										JZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_6
										MOV [EDI+12], EAX
									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_6:
										ADD ESI, 16
										ADD EDI, 16
										DEC ECX
										JNZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_2


									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_7:
										MOV ECX, EBX
										AND ECX, 3
										JZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_11
									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_9:
										MOV EAX, [ESI]
										CMP EAX, EDX
										JZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_10
										MOV [EDI], EAX
									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_10:
										ADD ESI, 4
										ADD EDI, 4
										DEC ECX
										JNZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_9

									LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_11:
										ADD EDI, DestAddPitch
										ADD ESI, SrcAddPitch
										DEC Height
										JNZ LABEL_FAST_NBI_C32_NBR_NAC_BNO_NTR_1
									}
#else
									DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
#endif
								}
								else
								{
#ifndef DX_NON_INLINE_ASM
									_asm
									{
										PUSHF
										CLD
										MOV ESI, SrcPP
										MOV EDI, DestPP
										MOV EBX, Height
										MOV EDX, Width
										MOV EAX, DestAddPitch
									LABEL_NFAST_NBI_C32_NBR_NAC_BNO_NTR_1:
										MOV ECX, EDX
										REP MOVSD
										ADD EDI, EAX
										ADD ESI, SrcAddPitch
										DEC EBX
										JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BNO_NTR_1
										POPF
									}
#else
									DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
#endif
								}
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
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
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 128 )
									{
										if( WinData.UseMMXFlag == TRUE )
										{
											_asm
											{
												MOV EAX, MEMIMG_XRGB8_ALPHAMASK1
												MOV EDX, MEMIMG_XRGB8_ALPHAMASK1
												MOVD MM2, EAX
												MOVD MM3, EDX
												PSLLQ MM2, 32
												POR MM3, MM2
												MOV EDI, DestPP
												MOV ESI, SrcPP
												MOV EDX, Height
												MOV EBX, TransCode
											LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_1:
												MOV ECX, Width
												SHR ECX, 1
												JZ LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_3
											LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_2:
												MOV EAX, [ESI]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_6
												MOVD MM0, EAX

												MOV EAX, [ESI+4]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_7
												MOVD MM1, EAX
												PSLLQ MM1, 32
												POR MM0, MM1
												MOVQ MM1, [EDI]
												PSRLQ MM0, 1
												PSRLQ MM1, 1
												PAND MM0, MM3
												PAND MM1, MM3
												PADDD MM0, MM1
												MOVQ [EDI], MM0
												JMP LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_5

											LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_6:
												MOV EAX, [ESI+4]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_5
												MOVD MM0, EAX
												MOVD MM1, [EDI+4]
												PSRLQ MM0, 1
												PSRLQ MM1, 1
												PAND MM0, MM3
												PAND MM1, MM3
												PADDD MM0, MM1
												MOVD [EDI+4], MM0
												JMP LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_5

											LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_7:
												MOVD MM1, [EDI]
												PSRLQ MM0, 1
												PSRLQ MM1, 1
												PAND MM0, MM3
												PAND MM1, MM3
												PADDD MM0, MM1
												MOVD [EDI], MM0

											LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_5:
												ADD EDI, 8
												ADD ESI, 8
												DEC ECX
												JNZ LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_2

											LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_3:
												TEST Width, 1
												JZ LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_4
												MOV EAX, [ESI]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_8
												MOVD MM0, EAX
												MOVD MM1, [EDI]
												PSRLQ MM0, 1
												PSRLQ MM1, 1
												PAND MM0, MM3
												PAND MM1, MM3
												PADDD MM0, MM1
												MOVD [EDI], MM0
											LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_8:
												ADD EDI, 4
												ADD ESI, 4

											LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_4:
												ADD EDI, DestAddPitch
												ADD ESI, SrcAddPitch
												DEC EDX
												JNZ LABEL_NBI_C32_NBR_NAC_BAL_NPAL_TR_MMX_1
												EMMS
											}
										}
										else
										{
											_asm
											{
												MOV ESI, SrcPP
												MOV EDI, DestPP
												MOV EAX, TransCode
											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_TR_1:
												MOV ECX, Width
											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_TR_2:
												MOV EBX, [ESI]
												CMP EBX, EAX
												JE LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_TR_3
												MOV EDX, [EDI]
												SHR EBX, 1
												SHR EDX, 1
												AND EBX, MEMIMG_XRGB8_ALPHAMASK1
												AND EDX, MEMIMG_XRGB8_ALPHAMASK1
												ADD EDX, EBX
												MOV [EDI], EDX
											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_TR_3:
												ADD ESI, 4
												ADD EDI, 4
												DEC ECX
												JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_TR_2
												ADD ESI, SrcAddPitch
												ADD EDI, DestAddPitch
												DEC Height
												JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_TR_1
											}
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
									}
								}
								else
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 128 )
									{
										if( WinData.UseMMXFlag == TRUE )
										{
											_asm
											{
												MOV EAX, MEMIMG_XRGB8_ALPHAMASK1
												MOV EDX, MEMIMG_XRGB8_ALPHAMASK1
												MOVD MM2, EAX
												MOVD MM3, EDX
												PSLLQ MM2, 32
												POR MM3, MM2
												MOV EDI, DestPP
												MOV ESI, SrcPP
												MOV EDX, Height
											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_MMX_1:
												MOV ECX, Width
												SHR ECX, 1
												JZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_MMX_3
											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_MMX_2:
												MOVQ MM0, [ESI]
												MOVQ MM1, [EDI]
												PSRLQ MM0, 1
												PSRLQ MM1, 1
												PAND MM0, MM3
												PAND MM1, MM3
												PADDD MM0, MM1
												MOVQ [EDI], MM0
												ADD EDI, 8
												ADD ESI, 8
												DEC ECX
												JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_MMX_2

											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_MMX_3:
												TEST Width, 1
												JZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_MMX_4
												MOVD MM0, [ESI]
												MOVD MM1, [EDI]
												PSRLQ MM0, 1
												PSRLQ MM1, 1
												PAND MM0, MM3
												PAND MM1, MM3
												PADDD MM0, MM1
												MOVD [EDI], MM0
												ADD EDI, 4
												ADD ESI, 4

											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_MMX_4:
												ADD ESI, SrcAddPitch
												ADD EDI, DestAddPitch
												DEC EDX
												JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_MMX_1
												EMMS
											}
										}
										else
										{
											_asm
											{
												MOV EDI, DestPP
												MOV ESI, SrcPP
												MOV EDX, Height
											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_1:
												MOV ECX, Width
												SHR ECX, 1
												JZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_3
											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_2:
												MOV EAX, [ESI]
												MOV EBX, [EDI]
												SHR EAX, 1
												SHR EBX, 1
												AND EAX, MEMIMG_XRGB8_ALPHAMASK1
												AND EBX, MEMIMG_XRGB8_ALPHAMASK1
												ADD EAX, EBX
												MOV [EDI], EAX
												MOV EBX, [ESI+4]
												MOV EAX, [EDI+4]
												SHR EBX, 1
												SHR EAX, 1
												AND EBX, MEMIMG_XRGB8_ALPHAMASK1
												AND EAX, MEMIMG_XRGB8_ALPHAMASK1
												ADD EBX, EAX
												MOV [EDI+4], EBX
												ADD EDI, 8
												ADD ESI, 8
												DEC ECX
												JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_2

											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_3:
												TEST Width, 1
												JZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_4
												MOV EAX, [ESI]
												MOV EBX, [EDI]
												SHR EBX, 1
												SHR EAX, 1
												AND EBX, MEMIMG_XRGB8_ALPHAMASK1
												AND EAX, MEMIMG_XRGB8_ALPHAMASK1
												ADD EAX, EBX
												MOV [EDI], EAX
												ADD EDI, 4
												ADD ESI, 4

											LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_4:
												ADD ESI, SrcAddPitch
												ADD EDI, DestAddPitch
												DEC EDX
												JNZ LABEL_NFAST_NBI_C32_NBR_NAC_BALH_NPAL_NTR_1
											}
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
									}
								}
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 255 )
									{
										if( WinData.UseMMXFlag == TRUE )
										{
											_asm
											{
												MOV EDI, DestPP
												MOV ESI, SrcPP
												MOV EDX, Height
												MOV EBX, TransCode
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_1:
												MOV ECX, Width
												SHR ECX, 1
												JZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_3
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_2:
												MOV EAX, [ESI]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_6
												MOVD MM0, EAX

												MOV EAX, [ESI+4]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_7
												MOVD MM1, EAX

												PSLLQ MM1, 32
												POR MM0, MM1
												MOVQ MM2, [EDI]
												PADDUSB MM0, MM2
												MOVQ [EDI], MM0
												JMP LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_5

											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_6:
												MOV EAX, [ESI+4]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_5
												MOVD MM0, EAX
												MOVD MM1, [EDI+4]
												PADDUSB MM0, MM1
												MOVD [EDI+4], MM0
												JMP LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_5

											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_7:
												MOVD MM1, [EDI]
												PADDUSB MM0, MM1
												MOVD [EDI], MM0

											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_5:
												ADD EDI, 8
												ADD ESI, 8
												DEC ECX
												JNZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_2

											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_3:
												TEST Width, 1
												JZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_4
												MOV EAX, [ESI]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_8
												MOVD MM0, EAX
												MOVD MM1, [EDI]
												PADDUSB MM0, MM1
												MOVD [EDI], MM0
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_8:
												ADD EDI, 4
												ADD ESI, 4

											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_4:
												ADD EDI, DestAddPitch
												ADD ESI, SrcAddPitch
												DEC EDX
												JNZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_MMX_1
												EMMS
											}
										}
										else
										{
											_asm
											{
												MOV EDI, DestPP
												MOV ESI, SrcPP
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_1:
												MOV EDX, Width
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_2:
												MOV EAX, [ESI]
												CMP EAX, TransCode
												JE LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_3
												MOV EBX, [EDI]
												AND EAX, MEMIMG_XRGB8_ADDMASK1
												AND EBX, MEMIMG_XRGB8_ADDMASK1
												ADD EAX, EBX
												MOV EBX, EAX
												AND EBX, MEMIMG_XRGB8_ADDMASK2
												MOV ECX, EBX
												SHR EBX, MEMIMG_XRGB8_ADDSHFT
												SUB ECX, EBX
												OR ECX, EAX
												MOV [EDI], ECX
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_3:
												ADD EDI, 4
												ADD ESI, 4
												DEC EDX
												JNZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_2
												
												ADD EDI, DestAddPitch
												ADD ESI, SrcAddPitch
												DEC Height
												JNZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_TR_1
											}
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
									}
								}
								else
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 255 )
									{
										if( WinData.UseMMXFlag == TRUE )
										{
											_asm
											{
												MOV EDI, DestPP
												MOV ESI, SrcPP
												MOV EDX, Height
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_1:
												MOV ECX, Width
												SHR ECX, 2
												JZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_3
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_2:
												MOVQ MM0, [ESI]
												MOVQ MM1, [ESI+8]
												MOVQ MM2, [EDI]
												MOVQ MM3, [EDI+8]

												PADDUSB MM0, MM2
												PADDUSB MM1, MM3

												MOVQ [EDI], MM0
												MOVQ [EDI+8], MM1

												ADD EDI, 16
												ADD ESI, 16
												DEC ECX
												JNZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_2

											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_3:
												MOV ECX, Width
												AND ECX, 3
												JZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_4
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_5:
												MOVD MM1, [EDI]
												MOVD MM0, [ESI]
												PADDUSB MM0, MM1
												MOVD [EDI], MM0
												ADD EDI, 4
												ADD ESI, 4
												DEC ECX
												JNZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_5

											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_4:
												ADD EDI, DestAddPitch
												ADD ESI, SrcAddPitch
												DEC EDX
												JNZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_MMX_1
												EMMS
											}
										}
										else
										{
											_asm
											{
												MOV EDI, DestPP
												MOV ESI, SrcPP
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_1:
												MOV EDX, Width
											LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_2:
												MOV EAX, [ESI]
												MOV EBX, [EDI]
												AND EAX, MEMIMG_XRGB8_ADDMASK1
												AND EBX, MEMIMG_XRGB8_ADDMASK1
												ADD EAX, EBX
												MOV EBX, EAX
												AND EBX, MEMIMG_XRGB8_ADDMASK2
												MOV ECX, EBX
												SHR EBX, MEMIMG_XRGB8_ADDSHFT
												SUB ECX, EBX
												OR ECX, EAX
												MOV [EDI], ECX
												ADD EDI, 4
												ADD ESI, 4
												DEC EDX
												JNZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_2

												ADD EDI, DestAddPitch
												ADD ESI, SrcAddPitch
												DEC Height
												JNZ LABEL_NBI_C32_NBR_NAC_BAD_NPAL_NTR_1
											}
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
									}
								}
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 255 )
									{
										if( WinData.UseMMXFlag == TRUE )
										{
											_asm
											{
												MOV EDI, DestPP
												MOV ESI, SrcPP
												MOV EDX, Height
												MOV EBX, TransCode
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_1:
												MOV ECX, Width
												SHR ECX, 1
												JZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_3
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_2:
												MOV EAX, [ESI]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_6
												MOVD MM0, EAX

												MOV EAX, [ESI+4]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_7
												MOVD MM1, EAX

												PSLLQ MM1, 32
												POR MM0, MM1
												MOVQ MM2, [EDI]
												PSUBUSB MM2, MM0
												MOVQ [EDI], MM2
												JMP LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_5

											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_6:
												MOV EAX, [ESI+4]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_5
												MOVD MM0, EAX
												MOVD MM1, [EDI+4]
												PSUBUSB MM1, MM0
												MOVD [EDI+4], MM1
												JMP LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_5

											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_7:
												MOVD MM1, [EDI]
												PSUBUSB MM1, MM0
												MOVD [EDI], MM1

											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_5:
												ADD EDI, 8
												ADD ESI, 8
												DEC ECX
												JNZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_2

											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_3:
												TEST Width, 1
												JZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_4
												MOV EAX, [ESI]
												CMP EAX, EBX
												JE LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_8
												MOVD MM0, EAX
												MOVD MM1, [EDI]
												PSUBUSB MM1, MM0
												MOVD [EDI], MM1
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_8:
												ADD EDI, 4
												ADD ESI, 4

											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_4:
												ADD EDI, DestAddPitch
												ADD ESI, SrcAddPitch
												DEC EDX
												JNZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_MMX_1
												EMMS
											}
										}
										else
										{
											_asm
											{
												MOV EDI, DestPP
												MOV ESI, SrcPP
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_1:
												MOV EDX, Width
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_2:
												MOV EAX, [ESI]
												CMP EAX, TransCode
												JE LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_3
												MOV EBX, [EDI]
												AND EAX, MEMIMG_XRGB8_SUBMASK1
												OR  EBX, MEMIMG_XRGB8_SUBMASK2
												SUB EBX, EAX
												MOV EAX, EBX
												AND EAX, MEMIMG_XRGB8_SUBMASK2
												MOV ECX, EAX
												SHR ECX, MEMIMG_XRGB8_SUBSHFT
												SUB EAX, ECX
												AND EBX, EAX
												MOV [EDI], EBX
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_3:
												ADD EDI, 4
												ADD ESI, 4
												DEC EDX
												JNZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_2
												
												ADD EDI, DestAddPitch
												ADD ESI, SrcAddPitch
												DEC Height
												JNZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_TR_1
											}
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
									}
								}
								else
								{
#ifndef DX_NON_INLINE_ASM
									if( MemImgManage.BlendParam == 255 )
									{
										if( WinData.UseMMXFlag == TRUE )
										{
											_asm
											{
												MOV EDI, DestPP
												MOV ESI, SrcPP
												MOV EDX, Height
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_1:
												MOV ECX, Width
												SHR ECX, 2
												JZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_3
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_2:
												MOVQ MM0, [ESI]
												MOVQ MM1, [ESI+8]
												MOVQ MM2, [EDI]
												MOVQ MM3, [EDI+8]

												PSUBUSB MM2, MM0
												PSUBUSB MM3, MM1

												MOVQ [EDI], MM2
												MOVQ [EDI+8], MM3

												ADD EDI, 16
												ADD ESI, 16
												DEC ECX
												JNZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_2

											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_3:
												MOV ECX, Width
												AND ECX, 3
												JZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_4
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_5:
												MOVD MM1, [EDI]
												MOVD MM0, [ESI]
												PSUBUSB MM1, MM0
												MOVD [EDI], MM1
												ADD EDI, 4
												ADD ESI, 4
												DEC ECX
												JNZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_5

											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_4:
												ADD EDI, DestAddPitch
												ADD ESI, SrcAddPitch
												DEC EDX
												JNZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_MMX_1
												EMMS
											}
										}
										else
										{
											_asm
											{
												MOV EDI, DestPP
												MOV ESI, SrcPP
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_1:
												MOV EDX, Width
											LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_2:
												MOV EAX, [ESI]
												MOV EBX, [EDI]
												AND EAX, MEMIMG_XRGB8_SUBMASK1
												OR  EBX, MEMIMG_XRGB8_SUBMASK2
												SUB EBX, EAX
												MOV EAX, EBX
												AND EAX, MEMIMG_XRGB8_SUBMASK2
												MOV ECX, EAX
												SHR ECX, MEMIMG_XRGB8_SUBSHFT
												SUB EAX, ECX
												AND EBX, EAX
												MOV [EDI], EBX
												ADD EDI, 4
												ADD ESI, 4
												DEC EDX
												JNZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_2

												ADD EDI, DestAddPitch
												ADD ESI, SrcAddPitch
												DEC Height
												JNZ LABEL_NBI_C32_NBR_NAC_BSB_NPAL_NTR_1
											}
										}
									}
									else
#endif
									{
										DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
									}
								}
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;
				}
			}
			break ;
		}
	}
	// 解析情報を使用する描画処理
	else
	{
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
FD_C16_USEPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto FD_C16_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
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
FD_C16_NOPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_NBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_UBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_NBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
/*								DWORD TransCode = SrcImg->Base->TransColor | ( SrcImg->Base->TransColor << 16 ) ;
								_asm
								{
									PUSHF
									CLD
									MOV ESI, SrcPP
									MOV EDI, DestPP
									MOV EDX, TransCode
									MOV EBX, Width

								LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_1:
									MOV ECX, EBX
									SHR ECX, 1
									JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_6
								LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_2:
									MOV EAX, [ESI]
									CMP EAX, EDX
									JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3
									
									CMP AX, DX
									JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_4
									MOV [EDI], AX
									SHR EAX, 16
									CMP AX, DX
									JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3
									MOV [EDI+2], AX
									JMP LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3
	
								LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_4:
									SHR EAX, 16
									MOV [EDI+2], AX
									
								LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3:
									ADD ESI, 4
									ADD EDI, 4
									DEC ECX
									JNZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_2
									
								LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_6:
									TEST EBX, 1
									JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_5
									MOV AX, [ESI]
									ADD ESI, 2
									ADD EDI, 2
									CMP AX, DX
									JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_5
									MOV [EDI-2], AX
									
								LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_5:
									ADD EDI, DestAddPitch
									ADD ESI, SrcAddPitch
									DEC Height
									JNZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_1
									
									POPF
								}*/
/*								_asm
								{
									PUSHF
									CLD
									MOV ESI, SrcPP
									MOV EDI, DestPP
									MOV EDX, AnaBP
									MOV EBX, Height
									MOV EAX, DestAddPitch
									SHL EAX, 16
									PUSH EBP
									MOV EBP, SrcAddPitch

								LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_1:
										MOV AL, [EDX]
										INC EDX
										MOV ECX, EAX
										AND AL, 0xc0
										JZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3:
										CMP AL, 0xc0
										JE LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_4:
											AND ECX, 0x3f
											INC ECX
											REP MOVSW
											JMP LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_1
								LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_3:
											AND ECX, 0x3f
											INC ECX
											ADD ECX, ECX
											ADD EDI, ECX
											ADD ESI, ECX
											JMP LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_1

								LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_4:
										SHR EAX, 16
										ADD ESI, EBP
										ADD EDI, EAX
										SHL EAX, 16
										DEC EBX
										JNZ LABEL_FAST_NBI_C16_NBR_NAC_BNO_NTR_1
									POP EBP
									POPF
								}
*/
/*								do{
									while( ( (*AnaBP) >> 6 ) != 3 ){
										LoopNum = ( *AnaBP & 0x3f ) + 1 ;
										if( (*AnaBP) >> 6 ){
											do{
												CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 )
												SrcBP += 2 ;
												DestBP += 2 ;
											}while( -- LoopNum ) ;
										}else{
											SrcBP += LoopNum * 2 ;
											DestBP += LoopNum * 2 ;
										}
										AnaBP ++ ;
									}
									AnaBP ++ ;
									SrcBP  += SrcAddPitch ;
									DestBP += DestAddPitch ;
								}while( -- Height );
*/
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_UBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto FD_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_NBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_UBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_NBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_UBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_NBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_UBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_NBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_UBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_NBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_UBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_NBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_UBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_NBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_UBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_NBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_UBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_NBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_UBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_NBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_UBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_NBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C16_UBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16, BLND ),
														CODE_UBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_NBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C16_UBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16 ),
														CODE_NBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16 ), 4, 2 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2, 2 )
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
FD_C32_USEPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto FD_C32_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// 減算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						}else{
							DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
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
FD_C32_NOPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_NBR_UAC_BNO( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_NBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_UBR_UAC_BNO( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_UBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_NBR_UAC_BNO( SRCP32, DSTP32 ),
														CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_UBR_UAC_BNO( SRCP32, DSTP32 ),
														CODE_NBI_C32_UBR_NAC_BNO( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BNO( SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// αブレンド
					if( MemImgManage.BlendParam == 255 ) goto FD_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_NBR_UAC_BAL( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_NBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_UBR_UAC_BAL( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_UBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_NBR_UAC_BAL( SRCP32, DSTP32 ),
														CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_UBR_UAC_BAL( SRCP32, DSTP32 ),
														CODE_NBI_C32_UBR_NAC_BAL( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BAL( SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// 加算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_NBR_UAC_BAD( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_NBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_UBR_UAC_BAD( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_UBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_NBR_UAC_BAD( SRCP32, DSTP32 ),
														CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_UBR_UAC_BAD( SRCP32, DSTP32 ),
														CODE_NBI_C32_UBR_NAC_BAD( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BAD( SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// 減算ブレンド
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_NBR_UAC_BSB( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_NBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_UBR_UAC_BSB( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_UBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_NBR_UAC_BSB( SRCP32, DSTP32 ),
														CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_UBR_UAC_BSB( SRCP32, DSTP32 ),
														CODE_NBI_C32_UBR_NAC_BSB( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BSB( SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// 乗算ブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_NBR_UAC_BML( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_NBR_NAC_BML( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BML( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_UBR_UAC_BML( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_UBR_NAC_BML( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BML( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_NBR_UAC_BML( SRCP32, DSTP32 ),
														CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_UBR_UAC_BML( SRCP32, DSTP32 ),
														CODE_NBI_C32_UBR_NAC_BML( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BML( SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// 乗算+αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_NBR_UAC_BMA( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_NBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_UBR_UAC_BMA( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_UBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_NBR_UAC_BMA( SRCP32, DSTP32 ),
														CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_UBR_UAC_BMA( SRCP32, DSTP32 ),
														CODE_NBI_C32_UBR_NAC_BMA( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BMA( SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_NBR_UAC_BIS( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_NBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_NBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_UBI_UAC_FD( CODE_UBI_C32_UBR_UAC_BIS( SRCP32, DSTP32, BLND ),
														CODE_UBI_C32_UBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								DRAWMEMIMG_UBI_NAC_FD( CODE_UBI_C32_UBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_NBR_UAC_BIS( SRCP32, DSTP32 ),
														CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								DRAWMEMIMG_NBI_UAC_FD( CODE_NBI_C32_UBR_UAC_BIS( SRCP32, DSTP32 ),
														CODE_NBI_C32_UBR_NAC_BIS( SRCP32, DSTP32 ), 4, 4 )
							}else{
								DRAWMEMIMG_NBI_NAC_FD( CODE_NBI_C32_UBR_NAC_BIS( SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;
				}
			}
			break ;
		}
	}

	// 終了
	return ;
}
#undef PALP32
#undef SRCP32
#undef SRCA16
#undef DSTP32
#undef BLND

#undef PAL16
#undef DST16
#undef SRC16
#undef DSTP16












#define DRAWENLARGEMEMIMG_UBI_ND( DRAW, DESTADDNUM )	\
{\
	DestBPT  = DestBP ;\
	SrcBPT   = SrcBP ;\
	BlendBPT = BlendBP ;\
	do{\
		dwtemp     = DrawWidth ;\
		fastXRtemp = FastExRateX ;\
		for(;;){\
			do{\
				DRAW\
				DestBPT += (DESTADDNUM) ;\
			}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
			if( dwtemp == 0 ) break ;\
			do{\
				SrcBPT     += LBFlag ;\
				BlendBPT   += LBFlagB ;\
				fastXRtemp += ExRateX ;\
			}while( fastXRtemp <= 0 ) ;\
		}\
		DestBP      += DestPitch ;\
		DestBPT      = DestBP ;\
		FastExRateY -= ( 1 << 16 ) ;\
		if( FastExRateY <= 0 ){\
			do{\
				SrcBP       += SrcPitch ;\
				BlendBP     += BlendPitch ;\
				FastExRateY += ExRateY ;\
			}while( FastExRateY <= 0 ) ;\
		}\
		SrcBPT   = SrcBP ;\
		BlendBPT = BlendBP ;\
	}while( -- DrawHeight ) ;\
}


#define DRAWENLARGEMEMIMG_UBI_TCK_ND( DRAW, DESTADDNUM, SRC )	\
{\
	DestBPT  = DestBP ;\
	SrcBPT   = SrcBP ;\
	BlendBPT = BlendBP ;\
	do{\
		dwtemp     = DrawWidth ;\
		fastXRtemp = FastExRateX ;\
		for(;;){\
			if( (SRC) != TransColor ){\
				do{\
					DRAW\
					DestBPT += (DESTADDNUM) ;\
				}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
				if( dwtemp == 0 ) break ;\
			}else{\
				do{\
					DestBPT += (DESTADDNUM) ;\
				}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
				if( dwtemp == 0 ) break ;\
			}\
			do{\
				SrcBPT     += LBFlag ;\
				BlendBPT   += LBFlagB ;\
				fastXRtemp += ExRateX ;\
			}while( fastXRtemp <= 0 ) ;\
		}\
		DestBP      += DestPitch ;\
		DestBPT      = DestBP ;\
		FastExRateY -= ( 1 << 16 ) ;\
		if( FastExRateY <= 0 ){\
			do{\
				SrcBP       += SrcPitch ;\
				BlendBP     += BlendPitch ;\
				FastExRateY += ExRateY ;\
			}while( FastExRateY <= 0 ) ;\
		}\
		SrcBPT   = SrcBP ;\
		BlendBPT = BlendBP ;\
	}while( -- DrawHeight ) ;\
}


#define DRAWENLARGEMEMIMG_UBI_ACK_ND( DRAW, ALPHAMAXDRAW, DESTADDNUM, SRCA )	\
{\
	DestBPT  = DestBP ;\
	SrcBPT   = SrcBP ;\
	BlendBPT = BlendBP ;\
	do{\
		dwtemp     = DrawWidth ;\
		fastXRtemp = FastExRateX ;\
		for(;;){\
			if( (SRCA) != 0 ){\
				if( (SRCA) == 255 ){\
					do{\
						ALPHAMAXDRAW\
						DestBPT += (DESTADDNUM) ;\
					}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
					if( dwtemp == 0 ) break ;\
				}else{\
					do{\
						DRAW\
						DestBPT += (DESTADDNUM) ;\
					}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
					if( dwtemp == 0 ) break ;\
				}\
			}else{\
				do{\
					DestBPT += (DESTADDNUM) ;\
				}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
				if( dwtemp == 0 ) break ;\
			}\
			do{\
				SrcBPT     += LBFlag ;\
				BlendBPT   += LBFlagB ;\
				fastXRtemp += ExRateX ;\
			}while( fastXRtemp <= 0 ) ;\
		}\
		DestBP      += DestPitch ;\
		DestBPT      = DestBP ;\
		FastExRateY -= ( 1 << 16 ) ;\
		if( FastExRateY <= 0 ){\
			do{\
				SrcBP       += SrcPitch ;\
				BlendBP     += BlendPitch ;\
				FastExRateY += ExRateY ;\
			}while( FastExRateY <= 0 ) ;\
		}\
		SrcBPT   = SrcBP ;\
		BlendBPT = BlendBP ;\
	}while( -- DrawHeight ) ;\
}


#define DRAWENLARGEMEMIMG_NBI_ND( DRAW, DESTADDNUM )	\
{\
	DestBPT  = DestBP ;\
	SrcBPT   = SrcBP ;\
	do{\
		dwtemp     = DrawWidth ;\
		fastXRtemp = FastExRateX ;\
		for(;;){\
			do{\
				DRAW\
				DestBPT += (DESTADDNUM) ;\
			}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
			if( dwtemp == 0 ) break ;\
			do{\
				SrcBPT     += LBFlag ;\
				fastXRtemp += ExRateX ;\
			}while( fastXRtemp <= 0 ) ;\
		}\
		DestBP      += DestPitch ;\
		DestBPT      = DestBP ;\
		FastExRateY -= ( 1 << 16 ) ;\
		if( FastExRateY <= 0 ){\
			do{\
				SrcBP       += SrcPitch ;\
				FastExRateY += ExRateY ;\
			}while( FastExRateY <= 0 ) ;\
		}\
		SrcBPT   = SrcBP ;\
	}while( -- DrawHeight ) ;\
}


#define DRAWENLARGEMEMIMG_NBI_TCK_ND( DRAW, DESTADDNUM, SRC )	\
{\
	DestBPT  = DestBP ;\
	SrcBPT   = SrcBP ;\
	do{\
		dwtemp     = DrawWidth ;\
		fastXRtemp = FastExRateX ;\
		for(;;){\
			if( (SRC) != TransColor ){\
				do{\
					DRAW\
					DestBPT += (DESTADDNUM) ;\
				}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
				if( dwtemp == 0 ) break ;\
			}else{\
				do{\
					DestBPT += (DESTADDNUM) ;\
				}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
				if( dwtemp == 0 ) break ;\
			}\
			do{\
				SrcBPT     += LBFlag ;\
				fastXRtemp += ExRateX ;\
			}while( fastXRtemp <= 0 ) ;\
		}\
		DestBP      += DestPitch ;\
		DestBPT      = DestBP ;\
		FastExRateY -= ( 1 << 16 ) ;\
		if( FastExRateY <= 0 ){\
			do{\
				SrcBP       += SrcPitch ;\
				FastExRateY += ExRateY ;\
			}while( FastExRateY <= 0 ) ;\
		}\
		SrcBPT   = SrcBP ;\
	}while( -- DrawHeight ) ;\
}


#define DRAWENLARGEMEMIMG_NBI_ACK_ND( DRAW, ALPHAMAXDRAW, DESTADDNUM, SRCA )	\
{\
	DestBPT  = DestBP ;\
	SrcBPT   = SrcBP ;\
	do{\
		dwtemp     = DrawWidth ;\
		fastXRtemp = FastExRateX ;\
		for(;;){\
			if( (SRCA) != 0 ){\
				if( (SRCA) == 255 ){\
					do{\
						ALPHAMAXDRAW\
						DestBPT += (DESTADDNUM) ;\
					}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
					if( dwtemp == 0 ) break ;\
				}else{\
					do{\
						DRAW\
						DestBPT += (DESTADDNUM) ;\
					}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
					if( dwtemp == 0 ) break ;\
				}\
			}else{\
				do{\
					DestBPT += (DESTADDNUM) ;\
				}while( -- dwtemp != 0 && ( fastXRtemp -= ( 1 << 16 ) ) > 0 ) ;\
				if( dwtemp == 0 ) break ;\
			}\
			do{\
				SrcBPT     += LBFlag ;\
				fastXRtemp += ExRateX ;\
			}while( fastXRtemp <= 0 ) ;\
		}\
		DestBP      += DestPitch ;\
		DestBPT      = DestBP ;\
		FastExRateY -= ( 1 << 16 ) ;\
		if( FastExRateY <= 0 ){\
			do{\
				SrcBP       += SrcPitch ;\
				FastExRateY += ExRateY ;\
			}while( FastExRateY <= 0 ) ;\
		}\
		SrcBPT   = SrcBP ;\
	}while( -- DrawHeight ) ;\
}

// イメージを拡大描画する
extern void DrawEnlargeMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, int x1, int y1, int x2, int y2, int TransFlag, const MEMIMG *BlendImg )
{
	RECT rect ;

	rect.left   = x1 ;
	rect.top    = y1 ;
	rect.right  = x2 ;
	rect.bottom = y2 ;
	DrawEnlargeMemImg( DestImg, SrcImg, &rect, TransFlag, BlendImg ) ;
}

// イメージを拡大描画する
#define SRC8	*SrcBPT
#define PAL16	PaletteWP[ *SrcBPT ]
#define DST16	*DestWPT
#define SRC16	*SrcWPT
#define SRCA16	SrcBPT[2]
#define DSTP16	DestWPT

#define PALP32	(BYTE *)&PaletteDP[ *SrcBPT ]
#define SRCP32	SrcBPT
#define DSTP32	DestBPT
#define SRC32	*SrcDPT
#define SRCA32	SrcBPT[3]

#define BLND	*BlendBPT
extern void DrawEnlargeMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const RECT *DestRect, int TransFlag, const MEMIMG *BlendImg )
{
	DWORD TransCode ;
	int ExRateX , ExRateY ;
	int FastExRateX , FastExRateY ;
	DWORD_PTR LBFlag, UDFlag, LBFlagB ;
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD_PTR SrcPP, DestPP, PalettePP ;
	DWORD *RedTable, *GreenTable, *BlueTable ;
	DWORD Src1, Src2 ;
	DWORD Data, DataD ;
	DWORD DrawWidth, DrawHeight ;
	DWORD DestX, DestY ;
	DWORD SrcX, SrcY ;
	RECT DrawRect ;
	DWORD_PTR SrcPitch, DestPitch, BlendPitch = 0, DestAddPitch ;
	DWORD TransColor ;
	static DWORD PaletteTemp[256] ;

	BYTE *BlendBP = NULL ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE *PaletteBP ;
		WORD *PaletteWP ;
		DWORD *PaletteDP ;
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
	BYTE *BlendBPT ;
	DWORD dwtemp ;
	int fastXRtemp ;

	// 透過色無効の場合は TransFlag を倒す
	if( SrcImg->Base->UseTransColor == 0 )
	{
		TransFlag = FALSE ;
	}

	DrawRect = *DestRect ;
	
	// 座標補正と判定設定
	if( DestRect->left > DestRect->right )
	{
		DrawRect.left  = DestRect->right ;
		DrawRect.right = DestRect->left ;
		LBFlag         = ( DWORD_PTR )-1 ;
	}
	else
	{
		DrawRect.left  = DestRect->left ;
		DrawRect.right = DestRect->right ;
		LBFlag         = 1 ;
	}

	if( DestRect->top > DestRect->bottom )
	{
		DrawRect.top    = DestRect->bottom ;
		DrawRect.bottom = DestRect->top ;
		UDFlag          = ( DWORD_PTR )-1 ;
	}
	else
	{
		DrawRect.top    = DestRect->top ;
		DrawRect.bottom = DestRect->bottom ;
		UDFlag          = 1 ;
	}

	// 拡大倍率を得る
	{
		DWORD w, h ;
		
		w = ( DWORD )( DrawRect.right  - DrawRect.left ) ;
		h = ( DWORD )( DrawRect.bottom - DrawRect.top  ) ;
		if( w == 0 || h == 0 ) return ;
		if( w == SrcImg->Width && h == SrcImg->Height && LBFlag == 1 && UDFlag == 1 )
		{
			DrawMemImg( DestImg, SrcImg, DrawRect.left, DrawRect.top, TransFlag, BlendImg ) ;
			return ;
		}
		ExRateX = ( int )( ( w << 16 ) / SrcImg->Width  ) ;
		ExRateY = ( int )( ( h << 16 ) / SrcImg->Height ) ;
	}

	// 描画矩形を作成
	{
		RECT temp ;
		temp = DrawRect ;
		RectClipping_Inline( &DrawRect, &MemImgManage.DrawArea ) ;
		DrawWidth  = ( DWORD )( DrawRect.right  - DrawRect.left ) ;
		DrawHeight = ( DWORD )( DrawRect.bottom - DrawRect.top  ) ;
		SrcX       = ( DWORD )( ( ( DrawRect.left - temp.left ) << 16 ) / ExRateX ) ;
		SrcY       = ( DWORD )( ( ( DrawRect.top  - temp.top  ) << 16 ) / ExRateY ) ;
		DestX      = ( DWORD )DrawRect.left ;
		DestY      = ( DWORD )DrawRect.top  ;
		FastExRateX = ExRateX - ( ( DrawRect.left - temp.left ) << 16 ) % ExRateX ;
		FastExRateY = ExRateY - ( ( DrawRect.top  - temp.top  ) << 16 ) % ExRateY ;

		if( DrawWidth == 0 || DrawHeight == 0 ) return ;
	}

	// 転送元、転送先のアドレスをセット
	{
		// 反転しているかどうかで分岐
		SrcBP = SrcImg->UseImage ;
		LBFlagB = LBFlag ;
		if( LBFlag != 1 )
		{
			LBFlag = ( DWORD_PTR )( ~( ( DWORD_PTR )SrcImg->Base->ColorDataP->PixelByte ) + 1 ) ;
			SrcBP += ( ( SrcImg->Width - 1 ) - SrcX ) * SrcImg->Base->ColorDataP->PixelByte ;
		}
		else
		{
			LBFlag = (DWORD)SrcImg->Base->ColorDataP->PixelByte ;
			SrcBP += SrcX * SrcImg->Base->ColorDataP->PixelByte ;
		}
		if( UDFlag != 1 )
		{
			SrcBP += ( ( SrcImg->Height - 1 ) - SrcY ) * SrcImg->Base->Pitch ;
			SrcPitch = ~( ( DWORD_PTR )SrcImg->Base->Pitch ) + 1 ;
		}
		else
		{
			SrcBP += SrcY * SrcImg->Base->Pitch ;
			SrcPitch = SrcImg->Base->Pitch ;
		}

		if( BlendImg != NULL )
		{
			BlendBP = BlendImg->UseImage ;
			if( LBFlagB != 1 )
			{
				LBFlagB = ~( ( DWORD_PTR )1 ) + 1 ;
				BlendBP += ( SrcImg->Width - 1 ) - SrcX ;
			}
			else
			{
				LBFlagB = 1 ;
				BlendBP += SrcX ;
			}
			if( UDFlag != 1 )
			{
				BlendBP += ( ( SrcImg->Height - 1 ) - SrcY ) * BlendImg->Base->Pitch ;
				BlendPitch = ~( ( DWORD_PTR )BlendImg->Base->Pitch ) + 1 ;
			}
			else
			{
				BlendBP += SrcY * BlendImg->Base->Pitch ;
				BlendPitch = BlendImg->Base->Pitch ;
			}
		}

		DestBP       = DestImg->UseImage + DestX * DestImg->Base->ColorDataP->PixelByte + DestY * DestImg->Base->Pitch ;
		DestPitch    = DestImg->Base->Pitch ;
		DestAddPitch = DestPitch - DrawWidth * DestImg->Base->ColorDataP->PixelByte ;
	}

	TransCode = SrcImg->Base->TransColor ;
	if( SrcImg->Base->UsePalette == TRUE )
		_MEMCPY( PaletteTemp, SrcImg->Base->Palette, ( size_t )( SrcImg->Base->ColorNum * ( SrcImg->Base->ColorType == 0 ? 2 : 4 ) ) ) ;

	PaletteBP  = (BYTE *)SrcImg->Base->Palette ;
	TransColor = SrcImg->Base->TransColor ;
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;
	RedTable   = MemImgManage.RateTable[MemImgManage.DrawBright.Red] ;
	GreenTable = MemImgManage.RateTable[MemImgManage.DrawBright.Green] ;
	BlueTable  = MemImgManage.RateTable[MemImgManage.DrawBright.Blue] ;

	SrcPP     = (DWORD_PTR)SrcBP ;
	DestPP    = (DWORD_PTR)DestBP ;
	PalettePP = (DWORD_PTR)PaletteBP ;

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
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND(     CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND(     CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )
						{
#ifdef DX_NON_INLINE_ASM
							DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 2, SRC8 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, SrcPP
								MOV EDX, PalettePP
						LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_1:
								MOV ECX, DrawWidth
								MOV EBX, FastExRateX
						LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_2:
								MOVZX EAX, BYTE PTR [ESI]
								CMP EAX, TransCode
								JE LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_8
								MOV AX, [EDX+EAX*2]
						LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_4:
								MOV [EDI], AX
								ADD EDI, 2
								DEC ECX
								JZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_6
								SUB EBX, 0x10000
								JG LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_4
								JMP LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3

						LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_8:
								ADD EDI, 2
								DEC ECX
								JZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_6
								SUB EBX, 0x10000
								JG LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_8

						LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3:
								ADD ESI, LBFlag
								ADD EBX, ExRateX
								JL LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3
								JMP LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_2

						LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_6:
								ADD EDI, DestAddPitch
								MOV EAX, FastExRateY
								SUB EAX, 0x10000
								MOV ESI, SrcPP
								JA LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_7

						LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_5:
								ADD ESI, SrcPitch
								ADD EAX, ExRateY
								JL LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_5

						LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_7:
								MOV FastExRateY, EAX
								MOV SrcPP, ESI
								DEC DrawHeight
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_1
							}
#endif
						}
						else
						{
#ifdef DX_NON_INLINE_ASM
							DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 2 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, SrcPP
								MOV EDX, PalettePP
						LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_1:
								MOV ECX, DrawWidth
								MOV EBX, FastExRateX
						LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_2:
								MOVZX EAX, BYTE PTR [ESI]
								MOV AX, [EDX+EAX*2]
						LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_4:
								MOV [EDI], AX
								ADD EDI, 2
								DEC ECX
								JZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_6
								SUB EBX, 0x10000
								JG LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_4

						LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_3:
								ADD ESI, LBFlag
								ADD EBX, ExRateX
								JL LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_3
								JMP LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_2

						LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_6:
								ADD EDI, DestAddPitch
								MOV EAX, FastExRateY
								SUB EAX, 0x10000
								MOV ESI, SrcPP
								JA LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_7

						LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_5:
								ADD ESI, SrcPitch
								ADD EAX, ExRateY
								JL LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_5

						LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_7:
								MOV FastExRateY, EAX
								MOV SrcPP, ESI
								DEC DrawHeight
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_1
							}
#endif
						}
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 2 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_USEPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 2 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ADD :			// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 2 )
					}
				}
				break ;

			case DX_BLENDMODE_SUB :			// 減算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )
						{
#ifndef DX_NON_INLINE_ASM
							if( MemImgManage.BlendParam == 255 )
							{
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, SrcPP
									MOV EDX, FastExRateY
							LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_1:
									MOV ECX, DrawWidth
									MOV dwtemp, ECX
									MOV EDX, FastExRateX
							LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_2:
									MOVZX EAX, byte ptr [ESI]
									CMP EAX, TransCode
									JE LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_4
									MOV BX, [EDI]
									MOV EAX, [PaletteTemp+EAX*2]
									AND EAX, MEMIMG_R5G6B5_SUBMASK1
									OR  EBX, MEMIMG_R5G6B5_SUBMASK2
									SUB EBX, EAX
									MOV EAX, EBX
									AND EAX, MEMIMG_R5G6B5_SUBMASK2
									MOV ECX, EAX
									SHR ECX, MEMIMG_R5G6B5_SUBSHFT
									SUB EAX, ECX
									AND EBX, EAX
									MOV [EDI], BX
							LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_4:
									ADD EDI, 2
									DEC dwtemp
									JZ LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_6
									SUB EDX, 0x10000
									JG LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_2

							LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_3:
									ADD ESI, LBFlag
									ADD EDX, ExRateX
									JL LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_3
									JMP LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_2

							LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_6:
									ADD EDI, DestAddPitch
									MOV EAX, FastExRateY
									SUB EAX, 0x10000
									MOV ESI, SrcPP
									JA LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_7

							LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_5:
									ADD ESI, SrcPitch
									ADD EAX, ExRateY
									JL LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_5

							LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_7:
									MOV FastExRateY, EAX
									MOV SrcPP, ESI
									DEC DrawHeight
									JNZ LABEL_NBI_C16_NBR_NAC_BSB_TR_PAL_1
								}
							}
							else
#endif
							{
								DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 2, SRC8 )
							}
						}
						else
						{
#ifndef DX_NON_INLINE_ASM
							if( MemImgManage.BlendParam == 255 )
							{
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, SrcPP
									MOV EDX, FastExRateY
							LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_1:
									MOV ECX, DrawWidth
									MOV dwtemp, ECX
									MOV EDX, FastExRateX
							LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_2:
									MOVZX EAX, byte ptr [ESI]
									MOV BX, [EDI]
									MOV EAX, [PaletteTemp+EAX*2]
									AND EAX, MEMIMG_R5G6B5_SUBMASK1
									OR  EBX, MEMIMG_R5G6B5_SUBMASK2
									SUB EBX, EAX
									MOV EAX, EBX
									AND EAX, MEMIMG_R5G6B5_SUBMASK2
									MOV ECX, EAX
									SHR ECX, MEMIMG_R5G6B5_SUBSHFT
									SUB EAX, ECX
									AND EBX, EAX
									MOV [EDI], BX
									ADD EDI, 2
									DEC dwtemp
									JZ LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_6
									SUB EDX, 0x10000
									JG LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_2

							LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_3:
									ADD ESI, LBFlag
									ADD EDX, ExRateX
									JL LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_3
									JMP LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_2

							LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_6:
									ADD EDI, DestAddPitch
									MOV EAX, FastExRateY
									SUB EAX, 0x10000
									MOV ESI, SrcPP
									JA LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_7

							LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_5:
									ADD ESI, SrcPitch
									ADD EAX, ExRateY
									JL LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_5

							LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_7:
									MOV FastExRateY, EAX
									MOV SrcPP, ESI
									DEC DrawHeight
									JNZ LABEL_NBI_C16_NBR_NAC_BSB_NTR_PAL_1
								}
							}
							else
#endif
							{
								DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 2 )
							}
						}
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 2 )
					}
				}
				break ;
				
			case DX_BLENDMODE_MUL :			// 乗算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 2 )
					}
				}
				break ;

			case DX_BLENDMODE_MULA :		// 乗算+αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 2 )
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 2 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 2, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 2 )
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
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_NBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_UBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_NBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )
							{
#ifdef DX_NON_INLINE_ASM
								DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2, SRC16 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, SrcPP
									MOV EDX, TransCode
							LABEL_NBI_C16_NBR_NAC_BNO_TR_1:
									MOV ECX, DrawWidth
									MOV EBX, FastExRateX
							LABEL_NBI_C16_NBR_NAC_BNO_TR_2:
									MOV AX, [ESI]
									CMP AX, DX
									JE LABEL_NBI_C16_NBR_NAC_BNO_TR_8
							LABEL_NBI_C16_NBR_NAC_BNO_TR_4:
									MOV [EDI], AX
									ADD EDI, 2
									DEC ECX
									JZ LABEL_NBI_C16_NBR_NAC_BNO_TR_6
									SUB EBX, 0x10000
									JG LABEL_NBI_C16_NBR_NAC_BNO_TR_4
									JMP LABEL_NBI_C16_NBR_NAC_BNO_TR_3

							LABEL_NBI_C16_NBR_NAC_BNO_TR_8:
									ADD EDI, 2
									DEC ECX
									JZ LABEL_NBI_C16_NBR_NAC_BNO_TR_6
									SUB EBX, 0x10000
									JG LABEL_NBI_C16_NBR_NAC_BNO_TR_8

							LABEL_NBI_C16_NBR_NAC_BNO_TR_3:
									ADD ESI, LBFlag
									ADD EBX, ExRateX
									JL LABEL_NBI_C16_NBR_NAC_BNO_TR_3
									JMP LABEL_NBI_C16_NBR_NAC_BNO_TR_2

							LABEL_NBI_C16_NBR_NAC_BNO_TR_6:
									ADD EDI, DestAddPitch
									MOV EAX, FastExRateY
									SUB EAX, 0x10000
									MOV ESI, SrcPP
									JA LABEL_NBI_C16_NBR_NAC_BNO_TR_7

							LABEL_NBI_C16_NBR_NAC_BNO_TR_5:
									ADD ESI, SrcPitch
									ADD EAX, ExRateY
									JL LABEL_NBI_C16_NBR_NAC_BNO_TR_5

							LABEL_NBI_C16_NBR_NAC_BNO_TR_7:
									MOV FastExRateY, EAX
									MOV SrcPP, ESI
									DEC DrawHeight
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_TR_1
								}
#endif
							}
							else
							{
#ifdef DX_NON_INLINE_ASM
								DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, SrcPP
									MOV EDX, FastExRateY
							LABEL_NBI_C16_NBR_NAC_BNO_NTR_1:
									MOV ECX, DrawWidth
									MOV EBX, FastExRateX
							LABEL_NBI_C16_NBR_NAC_BNO_NTR_2:
									MOV AX, [ESI]
							LABEL_NBI_C16_NBR_NAC_BNO_NTR_4:
									MOV [EDI], AX
									ADD EDI, 2
									DEC ECX
									JZ LABEL_NBI_C16_NBR_NAC_BNO_NTR_6
									SUB EBX, 0x10000
									JG LABEL_NBI_C16_NBR_NAC_BNO_NTR_4

							LABEL_NBI_C16_NBR_NAC_BNO_NTR_3:
									ADD ESI, LBFlag
									ADD EBX, ExRateX
									JL LABEL_NBI_C16_NBR_NAC_BNO_NTR_3
									JMP LABEL_NBI_C16_NBR_NAC_BNO_NTR_2

							LABEL_NBI_C16_NBR_NAC_BNO_NTR_6:
									ADD EDI, DestAddPitch
									SUB EDX, 0x10000
									MOV ESI, SrcPP
									JA LABEL_NBI_C16_NBR_NAC_BNO_NTR_7

							LABEL_NBI_C16_NBR_NAC_BNO_NTR_5:
									ADD ESI, SrcPitch
									ADD EDX, ExRateY
									JL LABEL_NBI_C16_NBR_NAC_BNO_NTR_5

							LABEL_NBI_C16_NBR_NAC_BNO_NTR_7:
									MOV SrcPP, ESI
									DEC DrawHeight
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_NTR_1
								}
#endif
							}
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_UBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_UAC_BNO( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
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
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_NBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_UBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_NBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_UBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_UAC_BAL( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_NBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_UBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_NBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_UBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_UAC_BAD( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_NBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_UBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_NBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
										MOV EDX, FastExRateY
								LABEL_NBI_C16_NBR_NAC_BSB_TR_1:
										MOV ECX, DrawWidth
										MOV dwtemp, ECX
										MOV EDX, FastExRateX
								LABEL_NBI_C16_NBR_NAC_BSB_TR_2:
										MOVZX EAX, word ptr [ESI]
										CMP EAX, TransCode
										JE LABEL_NBI_C16_NBR_NAC_BSB_TR_4
										MOV BX, [EDI]
										AND EAX, MEMIMG_R5G6B5_SUBMASK1
										OR  EBX, MEMIMG_R5G6B5_SUBMASK2
										SUB EBX, EAX
										MOV EAX, EBX
										AND EAX, MEMIMG_R5G6B5_SUBMASK2
										MOV ECX, EAX
										SHR ECX, MEMIMG_R5G6B5_SUBSHFT
										SUB EAX, ECX
										AND EBX, EAX
										MOV [EDI], BX
								LABEL_NBI_C16_NBR_NAC_BSB_TR_4:
										ADD EDI, 2
										DEC dwtemp
										JZ LABEL_NBI_C16_NBR_NAC_BSB_TR_6
										SUB EDX, 0x10000
										JG LABEL_NBI_C16_NBR_NAC_BSB_TR_2

								LABEL_NBI_C16_NBR_NAC_BSB_TR_3:
										ADD ESI, LBFlag
										ADD EDX, ExRateX
										JL LABEL_NBI_C16_NBR_NAC_BSB_TR_3
										JMP LABEL_NBI_C16_NBR_NAC_BSB_TR_2

								LABEL_NBI_C16_NBR_NAC_BSB_TR_6:
										ADD EDI, DestAddPitch
										MOV EAX, FastExRateY
										SUB EAX, 0x10000
										MOV ESI, SrcPP
										JA LABEL_NBI_C16_NBR_NAC_BSB_TR_7

								LABEL_NBI_C16_NBR_NAC_BSB_TR_5:
										ADD ESI, SrcPitch
										ADD EAX, ExRateY
										JL LABEL_NBI_C16_NBR_NAC_BSB_TR_5

								LABEL_NBI_C16_NBR_NAC_BSB_TR_7:
										MOV FastExRateY, EAX
										MOV SrcPP, ESI
										DEC DrawHeight
										JNZ LABEL_NBI_C16_NBR_NAC_BSB_TR_1
									}
								}
								else
#endif
								{
									DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2, SRC16 )
								}
							}
							else
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
										MOV EDX, FastExRateY
								LABEL_NBI_C16_NBR_NAC_BSB_NTR_1:
										MOV ECX, DrawWidth
										MOV dwtemp, ECX
										MOV EDX, FastExRateX
								LABEL_NBI_C16_NBR_NAC_BSB_NTR_2:
										MOV AX, [ESI]
										MOV BX, [EDI]
										AND EAX, MEMIMG_R5G6B5_SUBMASK1
										OR  EBX, MEMIMG_R5G6B5_SUBMASK2
										SUB EBX, EAX
										MOV EAX, EBX
										AND EAX, MEMIMG_R5G6B5_SUBMASK2
										MOV ECX, EAX
										SHR ECX, MEMIMG_R5G6B5_SUBSHFT
										SUB EAX, ECX
										AND EBX, EAX
										MOV [EDI], BX
										ADD EDI, 2
										DEC dwtemp
										JZ LABEL_NBI_C16_NBR_NAC_BSB_NTR_6
										SUB EDX, 0x10000
										JG LABEL_NBI_C16_NBR_NAC_BSB_NTR_2

								LABEL_NBI_C16_NBR_NAC_BSB_NTR_3:
										ADD ESI, LBFlag
										ADD EDX, ExRateX
										JL LABEL_NBI_C16_NBR_NAC_BSB_NTR_3
										JMP LABEL_NBI_C16_NBR_NAC_BSB_NTR_2

								LABEL_NBI_C16_NBR_NAC_BSB_NTR_6:
										ADD EDI, DestAddPitch
										MOV EAX, FastExRateY
										SUB EAX, 0x10000
										MOV ESI, SrcPP
										JA LABEL_NBI_C16_NBR_NAC_BSB_NTR_7

								LABEL_NBI_C16_NBR_NAC_BSB_NTR_5:
										ADD ESI, SrcPitch
										ADD EAX, ExRateY
										JL LABEL_NBI_C16_NBR_NAC_BSB_NTR_5

								LABEL_NBI_C16_NBR_NAC_BSB_NTR_7:
										MOV FastExRateY, EAX
										MOV SrcPP, ESI
										DEC DrawHeight
										JNZ LABEL_NBI_C16_NBR_NAC_BSB_NTR_1
									}
								}
								else
#endif
								{
									DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
								}
							}
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_UBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_UAC_BSB( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_NBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_UBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_NBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_UBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_UAC_BML( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_NBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_UBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_NBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_UBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_UAC_BMA( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_NBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C16_UBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16, BLND ), CODE_UBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16, BLND ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16, BLND ), 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_NBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C16_UBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16 ), CODE_NBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2, SRCA16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_UAC_BIS( SRC16, SRCA16, DST16, DSTP16 ), 2 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2, SRC16 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C16_UBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
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
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )
						{
#ifdef DX_NON_INLINE_ASM
							DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 4, SRC8 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, SrcPP
								MOV EDX, PalettePP
						LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_1:
								MOV ECX, DrawWidth
								MOV EBX, FastExRateX
						LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_2:
								MOVZX EAX, BYTE PTR [ESI]
								CMP EAX, TransCode
								JE LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_8
								MOV EAX, [EDX+EAX*4]
						LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_4:
								MOV [EDI], EAX
								ADD EDI, 4
								DEC ECX
								JZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_6
								SUB EBX, 0x10000
								JG LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_4
								JMP LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3

						LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_8:
								ADD EDI, 4
								DEC ECX
								JZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_6
								SUB EBX, 0x10000
								JG LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_8

						LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3:
								ADD ESI, LBFlag
								ADD EBX, ExRateX
								JL LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3
								JMP LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_2

						LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_6:
								ADD EDI, DestAddPitch
								MOV EAX, FastExRateY
								SUB EAX, 0x10000
								MOV ESI, SrcPP
								JA LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_7

						LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_5:
								ADD ESI, SrcPitch
								ADD EAX, ExRateY
								JL LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_5

						LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_7:
								MOV FastExRateY, EAX
								MOV SrcPP, ESI
								DEC DrawHeight
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_1
							}
#endif
						}
						else
						{
#ifdef DX_NON_INLINE_ASM
							DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 4 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, SrcPP
								MOV EDX, PalettePP
						LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_1:
								MOV ECX, DrawWidth
								MOV EBX, FastExRateX
						LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_2:
								MOVZX EAX, BYTE PTR [ESI]
								MOV EAX, [EDX+EAX*4]
						LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_4:
								MOV [EDI], EAX
								ADD EDI, 4
								DEC ECX
								JZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_6
								SUB EBX, 0x10000
								JG LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_4

						LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_3:
								ADD ESI, LBFlag
								ADD EBX, ExRateX
								JL LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_3
								JMP LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_2

						LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_6:
								ADD EDI, DestAddPitch
								MOV EAX, FastExRateY
								SUB EAX, 0x10000
								MOV ESI, SrcPP
								JA LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_7

						LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_5:
								ADD ESI, SrcPitch
								ADD EAX, ExRateY
								JL LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_5

						LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_7:
								MOV FastExRateY, EAX
								MOV SrcPP, ESI
								DEC DrawHeight
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_1
							}
#endif
						}
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 4 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ALPHA :		// αブレンド
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_USEPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 4 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ADD :			// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 4 )
					}
				}
				break ;

			case DX_BLENDMODE_SUB :			// 減算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )
						{
#ifndef DX_NON_INLINE_ASM
							if( MemImgManage.BlendParam == 255 )
							{
								if( WinData.UseMMXFlag == TRUE )
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
										MOV EDX, FastExRateY
								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_1:
										MOV ECX, DrawWidth
										MOV EBX, FastExRateX
								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_2:
										MOVZX EAX, byte ptr [ESI]
										CMP EAX, TransCode
										JE LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_8
										MOVD MM0, [PaletteTemp + EAX * 4]

								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_4:
										MOVD MM1, [EDI]
										PSUBUSB MM1, MM0
										MOVD [EDI], MM1
										ADD EDI, 4
										DEC ECX
										JZ LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_6
										SUB EBX, 0x10000
										JG LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_4
								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_3:
										ADD ESI, LBFlag
										ADD EBX, ExRateX
										JL LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_3
										JMP LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_2
										
								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_8:
										ADD EDI, 4
										DEC ECX
										JZ LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_6
										SUB EBX, 0x10000
										JG LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_8
								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_9:
										ADD ESI, LBFlag
										ADD EBX, ExRateX
										JL LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_9
										JMP LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_2

								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_6:
										ADD EDI, DestAddPitch
										SUB EDX, 0x10000
										MOV ESI, SrcPP
										JA LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_7

								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_5:
										ADD ESI, SrcPitch
										ADD EDX, ExRateY
										JL LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_5

								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_7:
										MOV SrcPP, ESI
										DEC DrawHeight
										JNZ LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_MMX_1
										EMMS
									}
								}
								else
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
										MOV EDX, FastExRateY
								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_1:
										MOV ECX, DrawWidth
										MOV dwtemp, ECX
										MOV EDX, FastExRateX
								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_2:
										MOVZX EAX, byte ptr [ESI]
										CMP EAX, TransCode
										JE LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_4
										MOV EBX, [EDI]
										MOV EAX, [PaletteTemp + EAX * 4]
										AND EAX, MEMIMG_XRGB8_SUBMASK1
										OR  EBX, MEMIMG_XRGB8_SUBMASK2
										SUB EBX, EAX
										MOV EAX, EBX
										AND EAX, MEMIMG_XRGB8_SUBMASK2
										MOV ECX, EAX
										SHR ECX, MEMIMG_XRGB8_SUBSHFT
										SUB EAX, ECX
										AND EBX, EAX
										MOV [EDI], EBX
								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_4:
										ADD EDI, 4
										DEC dwtemp
										JZ LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_6
										SUB EDX, 0x10000
										JG LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_2

								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_3:
										ADD ESI, LBFlag
										ADD EDX, ExRateX
										JL LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_3
										JMP LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_2

								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_6:
										ADD EDI, DestAddPitch
										MOV EAX, FastExRateY
										SUB EAX, 0x10000
										MOV ESI, SrcPP
										JA LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_7

								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_5:
										ADD ESI, SrcPitch
										ADD EAX, ExRateY
										JL LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_5

								LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_7:
										MOV FastExRateY, EAX
										MOV SrcPP, ESI
										DEC DrawHeight
										JNZ LABEL_NBI_C32_NBR_NAC_BSB_TR_PAL_1
									}
								}
							}
							else
#endif
							{
								DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 4, SRC8 )
							}
						}
						else
						{
#ifndef DX_NON_INLINE_ASM
							if( MemImgManage.BlendParam == 255 )
							{
								if( WinData.UseMMXFlag == TRUE )
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
										MOV EDX, FastExRateY
								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_1:
										MOV ECX, DrawWidth
										MOV EBX, FastExRateX
								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_2:
										MOVZX EAX, byte ptr [ESI]
										MOVD MM0, [PaletteTemp + EAX * 4]
								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_4:
										MOVD MM1, [EDI]
										PSUBUSB MM1, MM0
										MOVD [EDI], MM1
										ADD EDI, 4
										DEC ECX
										JZ LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_6
										SUB EBX, 0x10000
										JG LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_4

								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_3:
										ADD ESI, LBFlag
										ADD EBX, ExRateX
										JL LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_3
										JMP LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_2

								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_6:
										ADD EDI, DestAddPitch
										SUB EDX, 0x10000
										MOV ESI, SrcPP
										JA LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_7

								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_5:
										ADD ESI, SrcPitch
										ADD EDX, ExRateY
										JL LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_5

								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_7:
										MOV SrcPP, ESI
										DEC DrawHeight
										JNZ LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_MMX_1
										EMMS
									}
								}
								else
								{
									_asm
									{
										MOV EDI, DestPP
										MOV ESI, SrcPP
										MOV EDX, FastExRateY
								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_1:
										MOV ECX, DrawWidth
										MOV dwtemp, ECX
										MOV EDX, FastExRateX
								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_2:
										MOVZX EAX, byte ptr [ESI]
										MOV EBX, [EDI]
										MOV EAX, [PaletteTemp + EAX * 4]
										AND EAX, MEMIMG_XRGB8_SUBMASK1
										OR  EBX, MEMIMG_XRGB8_SUBMASK2
										SUB EBX, EAX
										MOV EAX, EBX
										AND EAX, MEMIMG_XRGB8_SUBMASK2
										MOV ECX, EAX
										SHR ECX, MEMIMG_XRGB8_SUBSHFT
										SUB EAX, ECX
										AND EBX, EAX
										MOV [EDI], EBX

										ADD EDI, 4
										DEC dwtemp
										JZ LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_6
										SUB EDX, 0x10000
										JG LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_2

								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_3:
										ADD ESI, LBFlag
										ADD EDX, ExRateX
										JL LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_3
										JMP LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_2

								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_6:
										ADD EDI, DestAddPitch
										MOV EAX, FastExRateY
										SUB EAX, 0x10000
										MOV ESI, SrcPP
										JA LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_7

								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_5:
										ADD ESI, SrcPitch
										ADD EAX, ExRateY
										JL LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_5

								LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_7:
										MOV FastExRateY, EAX
										MOV SrcPP, ESI
										DEC DrawHeight
										JNZ LABEL_NBI_C32_NBR_NAC_BSB_NTR_PAL_1
									}
								}
							}
							else
#endif
							{
								DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 4 )
							}
						}
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 4 )
					}
				}
				break ;
				
			case DX_BLENDMODE_MUL :			// 乗算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 4 )
					}
				}
				break ;

			case DX_BLENDMODE_MULA :		// 乗算+αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 4 )
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 4 )
					}else{
						if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 4, SRC8 )
						else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 4 )
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
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_NBR_UAC_BNO( SRCP32, DSTP32, BLND ), CODE_UBI_C32_NBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_UAC_BNO( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_UBR_UAC_BNO( SRCP32, DSTP32, BLND ), CODE_UBI_C32_UBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_UAC_BNO( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BNO( SRCP32, DSTP32, BLND ), 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_NBR_UAC_BNO( SRCP32, DSTP32 ), CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_UAC_BNO( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )
							{
#ifdef DX_NON_INLINE_ASM
								DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4, SRC32 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, SrcPP
									MOV EDX, TransCode
							LABEL_NBI_C32_NBR_NAC_BNO_TR_1:
									MOV ECX, DrawWidth
									MOV EBX, FastExRateX
							LABEL_NBI_C32_NBR_NAC_BNO_TR_2:
									MOV EAX, [ESI]
									CMP EAX, EDX
									JE LABEL_NBI_C32_NBR_NAC_BNO_TR_8
							LABEL_NBI_C32_NBR_NAC_BNO_TR_4:
									MOV [EDI], EAX
									ADD EDI, 4
									DEC ECX
									JZ LABEL_NBI_C32_NBR_NAC_BNO_TR_6
									SUB EBX, 0x10000
									JG LABEL_NBI_C32_NBR_NAC_BNO_TR_4
									JMP LABEL_NBI_C32_NBR_NAC_BNO_TR_3

							LABEL_NBI_C32_NBR_NAC_BNO_TR_8:
									ADD EDI, 4
									DEC ECX
									JZ LABEL_NBI_C32_NBR_NAC_BNO_TR_6
									SUB EBX, 0x10000
									JG LABEL_NBI_C32_NBR_NAC_BNO_TR_8

							LABEL_NBI_C32_NBR_NAC_BNO_TR_3:
									ADD ESI, LBFlag
									ADD EBX, ExRateX
									JL LABEL_NBI_C32_NBR_NAC_BNO_TR_3
									JMP LABEL_NBI_C32_NBR_NAC_BNO_TR_2

							LABEL_NBI_C32_NBR_NAC_BNO_TR_6:
									ADD EDI, DestAddPitch
									MOV EAX, FastExRateY
									SUB EAX, 0x10000
									MOV ESI, SrcPP
									JA LABEL_NBI_C32_NBR_NAC_BNO_TR_7

							LABEL_NBI_C32_NBR_NAC_BNO_TR_5:
									ADD ESI, SrcPitch
									ADD EAX, ExRateY
									JL LABEL_NBI_C32_NBR_NAC_BNO_TR_5

							LABEL_NBI_C32_NBR_NAC_BNO_TR_7:
									MOV FastExRateY, EAX
									MOV SrcPP, ESI
									DEC DrawHeight
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_TR_1
								}
#endif
							}
							else
							{
#ifdef DX_NON_INLINE_ASM
								DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, SrcPP
									MOV EDX, FastExRateY
							LABEL_NBI_C32_NBR_NAC_BNO_NTR_1:
									MOV ECX, DrawWidth
									MOV EBX, FastExRateX
							LABEL_NBI_C32_NBR_NAC_BNO_NTR_2:
									MOV EAX, [ESI]
							LABEL_NBI_C32_NBR_NAC_BNO_NTR_4:
									MOV [EDI], EAX
									ADD EDI, 4
									DEC ECX
									JZ LABEL_NBI_C32_NBR_NAC_BNO_NTR_6
									SUB EBX, 0x10000
									JG LABEL_NBI_C32_NBR_NAC_BNO_NTR_4

							LABEL_NBI_C32_NBR_NAC_BNO_NTR_3:
									ADD ESI, LBFlag
									ADD EBX, ExRateX
									JL LABEL_NBI_C32_NBR_NAC_BNO_NTR_3
									JMP LABEL_NBI_C32_NBR_NAC_BNO_NTR_2

							LABEL_NBI_C32_NBR_NAC_BNO_NTR_6:
									ADD EDI, DestAddPitch
									SUB EDX, 0x10000
									MOV ESI, SrcPP
									JA LABEL_NBI_C32_NBR_NAC_BNO_NTR_7

							LABEL_NBI_C32_NBR_NAC_BNO_NTR_5:
									ADD ESI, SrcPitch
									ADD EDX, ExRateY
									JL LABEL_NBI_C32_NBR_NAC_BNO_NTR_5

							LABEL_NBI_C32_NBR_NAC_BNO_NTR_7:
									MOV SrcPP, ESI
									DEC DrawHeight
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_NTR_1
								}
#endif
							}
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_UBR_UAC_BNO( SRCP32, DSTP32 ), CODE_NBI_C32_UBR_NAC_BNO( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_UAC_BNO( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BNO( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
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
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_NBR_UAC_BAL( SRCP32, DSTP32, BLND ), CODE_UBI_C32_NBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_UAC_BAL( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_UBR_UAC_BAL( SRCP32, DSTP32, BLND ), CODE_UBI_C32_UBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_UAC_BAL( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BAL( SRCP32, DSTP32, BLND ), 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_NBR_UAC_BAL( SRCP32, DSTP32 ), CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_UAC_BAL( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_UBR_UAC_BAL( SRCP32, DSTP32 ), CODE_NBI_C32_UBR_NAC_BAL( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_UAC_BAL( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BAL( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ADD :		// 加算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_NBR_UAC_BAD( SRCP32, DSTP32, BLND ), CODE_UBI_C32_NBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_UAC_BAD( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_UBR_UAC_BAD( SRCP32, DSTP32, BLND ), CODE_UBI_C32_UBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_UAC_BAD( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BAD( SRCP32, DSTP32, BLND ), 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_NBR_UAC_BAD( SRCP32, DSTP32 ), CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_UAC_BAD( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_UBR_UAC_BAD( SRCP32, DSTP32 ), CODE_NBI_C32_UBR_NAC_BAD( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_UAC_BAD( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BAD( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_SUB :		// 減算ブレンド
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_NBR_UAC_BSB( SRCP32, DSTP32, BLND ), CODE_UBI_C32_NBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_UAC_BSB( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_UBR_UAC_BSB( SRCP32, DSTP32, BLND ), CODE_UBI_C32_UBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_UAC_BSB( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BSB( SRCP32, DSTP32, BLND ), 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_NBR_UAC_BSB( SRCP32, DSTP32 ), CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_UAC_BSB( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									if( WinData.UseMMXFlag == TRUE )
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
											MOV EDX, FastExRateY
									LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_1:
											MOV ECX, DrawWidth
											MOV EBX, FastExRateX
									LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_2:
											MOV EAX, [ESI]
											CMP EAX, TransCode
											JE LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_8
											MOVD MM0, EAX
									LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_4:
											MOVD MM1, [EDI]
											PSUBUSB MM1, MM0
											MOVD [EDI], MM1
											ADD EDI, 4
											DEC ECX
											JZ LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_6
											SUB EBX, 0x10000
											JG LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_4
									LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_10:
											ADD ESI, LBFlag
											ADD EBX, ExRateX
											JL LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_10
											JMP LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_2
											
									LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_8:
											ADD EDI, 4
											DEC ECX
											JZ LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_6
											SUB EBX, 0x10000
											JG LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_8
									LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_3:
											ADD ESI, LBFlag
											ADD EBX, ExRateX
											JL LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_3
											JMP LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_2

									LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_6:
											ADD EDI, DestAddPitch
											SUB EDX, 0x10000
											MOV ESI, SrcPP
											JA LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_7

									LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_5:
											ADD ESI, SrcPitch
											ADD EDX, ExRateY
											JL LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_5

									LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_7:
											MOV SrcPP, ESI
											DEC DrawHeight
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_TR_MMX_1
											EMMS
										}
									}
									else
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
											MOV EDX, FastExRateY
									LABEL_NBI_C32_NBR_NAC_BSB_TR_1:
											MOV ECX, DrawWidth
											MOV dwtemp, ECX
											MOV EDX, FastExRateX
									LABEL_NBI_C32_NBR_NAC_BSB_TR_2:
											MOV EAX, [ESI]
											CMP EAX, TransCode
											JE LABEL_NBI_C32_NBR_NAC_BSB_TR_4
											MOV EBX, [EDI]
											AND EAX, MEMIMG_XRGB8_SUBMASK1
											OR  EBX, MEMIMG_XRGB8_SUBMASK2
											SUB EBX, EAX
											MOV EAX, EBX
											AND EAX, MEMIMG_XRGB8_SUBMASK2
											MOV ECX, EAX
											SHR ECX, MEMIMG_XRGB8_SUBSHFT
											SUB EAX, ECX
											AND EBX, EAX
											MOV [EDI], EBX
									LABEL_NBI_C32_NBR_NAC_BSB_TR_4:
											ADD EDI, 4
											DEC dwtemp
											JZ LABEL_NBI_C32_NBR_NAC_BSB_TR_6
											SUB EDX, 0x10000
											JG LABEL_NBI_C32_NBR_NAC_BSB_TR_2

									LABEL_NBI_C32_NBR_NAC_BSB_TR_3:
											ADD ESI, LBFlag
											ADD EDX, ExRateX
											JL LABEL_NBI_C32_NBR_NAC_BSB_TR_3
											JMP LABEL_NBI_C32_NBR_NAC_BSB_TR_2

									LABEL_NBI_C32_NBR_NAC_BSB_TR_6:
											ADD EDI, DestAddPitch
											MOV EAX, FastExRateY
											SUB EAX, 0x10000
											MOV ESI, SrcPP
											JA LABEL_NBI_C32_NBR_NAC_BSB_TR_7

									LABEL_NBI_C32_NBR_NAC_BSB_TR_5:
											ADD ESI, SrcPitch
											ADD EAX, ExRateY
											JL LABEL_NBI_C32_NBR_NAC_BSB_TR_5

									LABEL_NBI_C32_NBR_NAC_BSB_TR_7:
											MOV FastExRateY, EAX
											MOV SrcPP, ESI
											DEC DrawHeight
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_TR_1
										}
									}
								}
								else
#endif
								{
									DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4, SRC32 )
								}
							}
							else
							{
#ifndef DX_NON_INLINE_ASM
								if( MemImgManage.BlendParam == 255 )
								{
									if( WinData.UseMMXFlag == TRUE )
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
											MOV EDX, FastExRateY
									LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_1:
											MOV ECX, DrawWidth
											MOV EBX, FastExRateX
									LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_2:
											MOVD MM0, [ESI]
									LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_4:
											MOVD MM1, [EDI]
											PSUBUSB MM1, MM0
											MOVD [EDI], MM1
											ADD EDI, 4
											DEC ECX
											JZ LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_6
											SUB EBX, 0x10000
											JG LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_4

									LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_3:
											ADD ESI, LBFlag
											ADD EBX, ExRateX
											JL LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_3
											JMP LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_2

									LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_6:
											ADD EDI, DestAddPitch
											SUB EDX, 0x10000
											MOV ESI, SrcPP
											JA LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_7

									LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_5:
											ADD ESI, SrcPitch
											ADD EDX, ExRateY
											JL LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_5

									LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_7:
											MOV SrcPP, ESI
											DEC DrawHeight
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_NTR_MMX_1
											EMMS
										}
									}
									else
									{
										_asm
										{
											MOV EDI, DestPP
											MOV ESI, SrcPP
											MOV EDX, FastExRateY
									LABEL_NBI_C32_NBR_NAC_BSB_NTR_1:
											MOV ECX, DrawWidth
											MOV dwtemp, ECX
											MOV EDX, FastExRateX
									LABEL_NBI_C32_NBR_NAC_BSB_NTR_2:
											MOV EAX, [ESI]
											MOV EBX, [EDI]
											AND EAX, MEMIMG_XRGB8_SUBMASK1
											OR  EBX, MEMIMG_XRGB8_SUBMASK2
											SUB EBX, EAX
											MOV EAX, EBX
											AND EAX, MEMIMG_XRGB8_SUBMASK2
											MOV ECX, EAX
											SHR ECX, MEMIMG_XRGB8_SUBSHFT
											SUB EAX, ECX
											AND EBX, EAX
											MOV [EDI], EBX

											ADD EDI, 4
											DEC dwtemp
											JZ LABEL_NBI_C32_NBR_NAC_BSB_NTR_6
											SUB EDX, 0x10000
											JG LABEL_NBI_C32_NBR_NAC_BSB_NTR_2

									LABEL_NBI_C32_NBR_NAC_BSB_NTR_3:
											ADD ESI, LBFlag
											ADD EDX, ExRateX
											JL LABEL_NBI_C32_NBR_NAC_BSB_NTR_3
											JMP LABEL_NBI_C32_NBR_NAC_BSB_NTR_2

									LABEL_NBI_C32_NBR_NAC_BSB_NTR_6:
											ADD EDI, DestAddPitch
											MOV EAX, FastExRateY
											SUB EAX, 0x10000
											MOV ESI, SrcPP
											JA LABEL_NBI_C32_NBR_NAC_BSB_NTR_7

									LABEL_NBI_C32_NBR_NAC_BSB_NTR_5:
											ADD ESI, SrcPitch
											ADD EAX, ExRateY
											JL LABEL_NBI_C32_NBR_NAC_BSB_NTR_5

									LABEL_NBI_C32_NBR_NAC_BSB_NTR_7:
											MOV FastExRateY, EAX
											MOV SrcPP, ESI
											DEC DrawHeight
											JNZ LABEL_NBI_C32_NBR_NAC_BSB_NTR_1
										}
									}
								}
								else
#endif
								{
									DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
								}
							}
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_UBR_UAC_BSB( SRCP32, DSTP32 ), CODE_NBI_C32_UBR_NAC_BSB( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_UAC_BSB( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BSB( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MUL :		// 乗算ブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_NBR_UAC_BML( SRCP32, DSTP32, BLND ), CODE_UBI_C32_NBR_NAC_BML( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_UAC_BML( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BML( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BML( SRCP32, DSTP32, BLND ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_UBR_UAC_BML( SRCP32, DSTP32, BLND ), CODE_UBI_C32_UBR_NAC_BML( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_UAC_BML( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BML( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BML( SRCP32, DSTP32, BLND ), 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_NBR_UAC_BML( SRCP32, DSTP32 ), CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_UAC_BML( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_UBR_UAC_BML( SRCP32, DSTP32 ), CODE_NBI_C32_UBR_NAC_BML( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_UAC_BML( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BML( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BML( SRCP32, DSTP32 ), 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MULA :	// 乗算+αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_NBR_UAC_BMA( SRCP32, DSTP32, BLND ), CODE_UBI_C32_NBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_UAC_BMA( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_UBR_UAC_BMA( SRCP32, DSTP32, BLND ), CODE_UBI_C32_UBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_UAC_BMA( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BMA( SRCP32, DSTP32, BLND ), 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_NBR_UAC_BMA( SRCP32, DSTP32 ), CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_UAC_BMA( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_UBR_UAC_BMA( SRCP32, DSTP32 ), CODE_NBI_C32_UBR_NAC_BMA( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_UAC_BMA( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BMA( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// 描画元の色を反転＋αブレンド
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_NBR_UAC_BIS( SRCP32, DSTP32, BLND ), CODE_UBI_C32_NBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_UAC_BIS( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_NBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_NBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_ACK_ND( CODE_UBI_C32_UBR_UAC_BIS( SRCP32, DSTP32, BLND ), CODE_UBI_C32_UBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_UAC_BIS( SRCP32, DSTP32, BLND ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_UBI_TCK_ND( CODE_UBI_C32_UBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_UBI_ND( 	  CODE_UBI_C32_UBR_NAC_BIS( SRCP32, DSTP32, BLND ), 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_NBR_UAC_BIS( SRCP32, DSTP32 ), CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_UAC_BIS( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_ACK_ND( CODE_NBI_C32_UBR_UAC_BIS( SRCP32, DSTP32 ), CODE_NBI_C32_UBR_NAC_BIS( SRCP32, DSTP32 ), 4, SRCA32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_UAC_BIS( SRCP32, DSTP32 ), 4 )
						}else{
							if( TransFlag )	DRAWENLARGEMEMIMG_NBI_TCK_ND( CODE_NBI_C32_UBR_NAC_BIS( SRCP32, DSTP32 ), 4, SRC32 )
							else			DRAWENLARGEMEMIMG_NBI_ND( 	  CODE_NBI_C32_UBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
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



#endif

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


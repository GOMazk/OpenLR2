// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		標準Ｃライブラリ使用コード　Ogg関係
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "DxCompileConfig.h"

#ifndef DX_NON_NAMESPACE
#ifdef DX_NON_USING_NAMESPACE_DXLIB

using namespace DxLib ;

#endif // DX_NON_USING_NAMESPACE_DXLIB
#endif // DX_NON_NAMESPACE

#if !defined( DX_NON_OPUS ) || !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

// インクルード ------------------------------------------------------------------
#include "DxUseCLib.h"
#include "DxUseCLibOgg.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxUseCLib.h"
#include "DxFile.h"
#include "DxLog.h"

#include "DxBaseFunc.h"
#include "DxMemory.h"
#include "DxSystem.h"

#include "DxGraphics.h"
#include "DxASyncLoad.h"

#ifdef WINDOWS_DESKTOP_OS
#include "Windows/DxUseCLibOggWin.h"
#endif // WINDOWS_DESKTOP_OS

#ifdef __ANDROID__
#include "Android/DxUseCLibOggAndroid.h"
#endif // __ANDROID__

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
		#include "iOS/DxUseCLibOggiOS.h"
    #endif // TARGET_OS_IPHONE
#endif // __APPLE__





#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#ifdef __DEBUG__
int ___StockNum ;
int ___StockFrame[ 100 ] ;
int ___StockValidFlag ;
int ___InCurrentFrameNumber ;
int ___Singou1, ___Singou2 ;
volatile ULONGLONG Ogg_ReadBytes_ ;
#endif

// 定数定義 ----------------------------------------------------------------------

// 構造体型宣言 ------------------------------------------------------------------

//namespace DxLib
//{

#ifndef DX_NON_OGGVORBIS

// ＯＧＧデータ変換用構造体
typedef struct tagSOUNDCONV_OGG
{
	int							FileInitializeFlag ;	// File を初期化しているか、フラグ
	OggVorbis_File				File ;					// ＯＧＧファイルを扱うためのOggVorbisFile構造体
	int							Word ;					// ビット数 / 8
	LONGLONG					LoopStart, LoopEnd ;	// ループ情報
} SOUNDCONV_OGG ;

#endif


#ifndef DX_NON_OPUS

// Opusデータ変換用構造体
typedef struct tagSOUNDCONV_OPUS
{
	int							FileInitializeFlag ;	// File を初期化しているか、フラグ
	OggOpusFile					*File ;					// Opusファイルを扱うためのOggOpusFile構造体
	size_t						BlockAlign ;			// １サンプル分のバイト数( チャンネル数 * 2byte )
} SOUNDCONV_OPUS ;

#endif


//}

// 関数プロトタイプ宣言 ----------------------------------------------------------

// extern int      SetupSoundConvert_OGG( SOUNDCONV *SoundConv ) ;										// ＯＧＧファイルのセットアップ処理を行う( [戻] -1:エラー )
// extern int      TerminateSoundConvert_OGG( SOUNDCONV *SoundConv ) ;									// ＯＧＧファイルの後始末処理を行う
// extern int      ConvertProcessSoundConvert_OGG( SOUNDCONV *SoundConv ) ;								// 変換後のバッファにデータを補充する
// extern int      SetSampleTimeSoundConvert_OGG( SOUNDCONV *SoundConv, LONGLONG SampleTime ) ;			// 変換処理の位置を変更する( サンプル単位 )
// extern LONGLONG GetSoundConvertDestSize_Fast_OGG( SOUNDCONV *SoundConv ) ;							// 変換後の大凡のデータサイズを得る

// extern int      SetupSoundConvert_OPUS( SOUNDCONV *SoundConv ) ;										// Opusファイルのセットアップ処理を行う( [戻] -1:エラー )
// extern int      TerminateSoundConvert_OPUS( SOUNDCONV *SoundConv ) ;									// Opusファイルの後始末処理を行う
// extern int      ConvertProcessSoundConvert_OPUS( SOUNDCONV *SoundConv ) ;							// 変換後のバッファにデータを補充する
// extern int      SetSampleTimeSoundConvert_OPUS( SOUNDCONV *SoundConv, LONGLONG SampleTime ) ;		// 変換処理の位置を変更する( サンプル単位 )
// extern LONGLONG GetSoundConvertDestSize_Fast_OPUS( SOUNDCONV *SoundConv ) ;							// 変換後の大凡のデータサイズを得る

//namespace DxLib
//{

#ifndef DX_NON_OGGTHEORA

static int			TheoraDecode_ReadHeader(				DECODE_THEORA *DT ) ;		// ヘッダの読み込み
static void			TheoraDecode_InitializeTheoraDecoder(	DECODE_THEORA *DT ) ;
static void			TheoraDecode_InitializeVorbisDecoder(	DECODE_THEORA *DT ) ;
static int			TheoraDecode_ReadData(					DECODE_THEORA *DT, int Bytes = 4096 ) ;
static bool			TheoraDecode_PlayFile(					DECODE_THEORA *DT, const char *fileName ) ;
static void			TheoraDecode_Rendering(					DECODE_THEORA *DT ) ;
static int			_TheoraDecode_IncToFrame( DWORD_PTR Handle, int AddNum ) ;					// 戻り値  1:キーフレーム  0:キーフレームじゃない  -1:ストリームの終端
static void			TheoraDecode_ReleaseSurface(			DECODE_THEORA *DT, int ASyncThread ) ;

#endif

//}

#ifndef DX_NON_OGGTHEORA
extern "C"
{
	extern float *vorbis_window(         vorbis_dsp_state *v,int W);
}

//namespace DxLib
//{

static long				th__get_data(					OggVorbis_File *vf );
static int				th__seek_helper(				OggVorbis_File *vf, ogg_int64_t offset);
static ogg_int64_t		th__get_next_page(				OggVorbis_File *vf, ogg_page *og, ogg_int64_t boundary );
static ogg_int64_t		th__get_prev_page(				OggVorbis_File *vf, ogg_int64_t begin, ogg_page *og );
static void				th__add_serialno(				ogg_page *og, long **serialno_list, int *n);
static int				th__lookup_serialno(			ogg_page *og, long *serialno_list, int n);
static int				th__get_serialnos(				OggVorbis_File *vf, long **s, int *n);
static int				th__bisect_forward_serialno(	OggVorbis_File *vf, ogg_int64_t begin, ogg_int64_t searched, ogg_int64_t end, long *currentno_list, int	currentnos, long m);
static int				th__fetch_headers(				OggVorbis_File *vf, vorbis_info *vi, vorbis_comment *vc, long *serialno, ogg_page *og_ptr );
static void				th__prefetch_all_headers(		OggVorbis_File *vf, ogg_int64_t dataoffset );
static int				th__make_decode_ready(			OggVorbis_File *vf);
static int				th__open_seekable2(			OggVorbis_File *vf);
static void				th__decode_clear(				OggVorbis_File *vf);
static int				th__fetch_and_process_packet(	OggVorbis_File *vf, ogg_packet *op_in, int readp, int spanp);
static int				th__fseek64_wrap(				FILE *f,ogg_int64_t off,int whence);
static int				th__ov_open1(					void *f, OggVorbis_File *vf, char *initial, long ibytes, ov_callbacks callbacks );
static int				th__ov_open2(					OggVorbis_File *vf);
static int				th_ov_clear(					OggVorbis_File *vf);
static int				th_ov_open_callbacks(			void *f, OggVorbis_File *vf, char *initial, long ibytes, ov_callbacks callbacks );
static int				th_ov_open(					FILE *f,OggVorbis_File *vf,char *initial,long ibytes);
static int				th_ov_fopen(					char *path,OggVorbis_File *vf);
static int				th_ov_halfrate(				OggVorbis_File *vf,int flag);
static int				th_ov_halfrate_p(				OggVorbis_File *vf);
static int				th_ov_test_callbacks(			void *f,OggVorbis_File *vf,char *initial,long ibytes, ov_callbacks callbacks);
static int				th_ov_test(					FILE *f,OggVorbis_File *vf,char *initial,long ibytes);
static int				th_ov_test_open(				OggVorbis_File *vf);
static long				th_ov_streams(					OggVorbis_File *vf);
static long				th_ov_seekable(				OggVorbis_File *vf);
static long				th_ov_bitrate(					OggVorbis_File *vf,int i);
static long				th_ov_bitrate_instant(			OggVorbis_File *vf);
static long				th_ov_serialnumber(			OggVorbis_File *vf,int i);
static ogg_int64_t		th_ov_raw_total(				OggVorbis_File *vf,int i);
static ogg_int64_t		th_ov_pcm_total(				OggVorbis_File *vf,int i);
static double			th_ov_time_total(				OggVorbis_File *vf,int i);
static int				th_ov_raw_seek(				OggVorbis_File *vf,ogg_int64_t pos);
static int				th_ov_pcm_seek_page(			OggVorbis_File *vf, ogg_int64_t pos );
static int				th_ov_pcm_seek(				OggVorbis_File *vf,ogg_int64_t pos);
static int				th_ov_time_seek(				OggVorbis_File *vf,double seconds);
static int				th_ov_time_seek_page(			OggVorbis_File *vf,double seconds);
static ogg_int64_t		th_ov_raw_tell(				OggVorbis_File *vf);
static ogg_int64_t		th_ov_pcm_tell(				OggVorbis_File *vf);
static double			th_ov_time_tell(				OggVorbis_File *vf);
static vorbis_info		*th_ov_info(					OggVorbis_File *vf,int link);
static vorbis_comment	*th_ov_comment(				OggVorbis_File *vf,int link);
static int				th_host_is_big_endian();
static long				th_ov_read(					OggVorbis_File *vf, char *buffer, int length, int bigendianp, int word, int sgned, int *bitstream );
static long				th_ov_read_float(				OggVorbis_File *vf,float ***pcm_channels,int length, int *bitstream);
static void				th__ov_splice(					float **pcm,float **lappcm, int n1, int n2, int ch1, int ch2, float *w1, float *w2);
static int				th__ov_initset(				OggVorbis_File *vf);
static int				th__ov_initprime(				OggVorbis_File *vf);
static void				th__ov_getlap(					OggVorbis_File *vf,vorbis_info *vi,vorbis_dsp_state *vd, float **lappcm,int lapsize);
static int				th_ov_crosslap(				OggVorbis_File *vf1, OggVorbis_File *vf2);
static int				th__ov_64_seek_lap(			OggVorbis_File *vf,ogg_int64_t pos, int (*localseek)(OggVorbis_File *,ogg_int64_t));
static int				th_ov_raw_seek_lap(			OggVorbis_File *vf,ogg_int64_t pos);
static int				th_ov_pcm_seek_lap(			OggVorbis_File *vf,ogg_int64_t pos);
static int				th_ov_pcm_seek_page_lap(		OggVorbis_File *vf,ogg_int64_t pos);
static int				th__ov_d_seek_lap(				OggVorbis_File *vf,double pos,int (*localseek)(OggVorbis_File *,double));
static int				th_ov_time_seek_lap(			OggVorbis_File *vf,double pos);
static int				th_ov_time_seek_page_lap(		OggVorbis_File *vf,double pos);
static void				th_analysis_output_always( char *base,int i,float *v,int n,int bark,int dB,ogg_int64_t off);

//}

#endif

// プログラム --------------------------------------------------------------------

//namespace DxLib
//{

#if defined( DX_NON_OGGVORBIS ) && defined( DX_NON_OGGTHEORA )
#else

// Ｏｇｇ用読み込み関数
static size_t Ogg_StreamRead( void *Buffer, size_t Size, size_t Count, void *DataP )
{
	STREAMDATA *StreamP = ( STREAMDATA * )DataP ;
	if( StreamP == NULL ) return ( size_t )-1 ;

#ifdef __DEBUG__
	Ogg_ReadBytes_ += Size * Count ;
#endif

	return StreamP->ReadShred.Read( Buffer, Size, Count, StreamP->DataPoint ) ;
}

// Ｏｇｇ用６４ビット版シーク関数
static int Ogg_StreamSeek64_wrap( void *DataP, ogg_int64_t Offset, int SeekMode )
{
	STREAMDATA *StreamP = ( STREAMDATA * )DataP ;
	if( StreamP == NULL ) return -1 ;
	return StreamP->ReadShred.Seek( StreamP->DataPoint, Offset, SeekMode );
}

// Ｏｇｇ用クローズ関数
static int Ogg_StreamClose( void * /*StreamP*/ )
{
	// 普通にクローズが成功したとして返す
	return 0 ;
}

// Ｏｇｇ用のデータ位置取得関数
static long Ogg_StreamTell( void *DataP )
{
	STREAMDATA *StreamP = ( STREAMDATA * )DataP ;
	if( StreamP == NULL ) return -1 ;
	return ( long )StreamP->ReadShred.Tell( StreamP->DataPoint );
}

#endif

#ifndef DX_NON_OGGTHEORA
#ifndef DX_NON_OGGVORBIS

/* read a little more data from the file/pipe into the ogg_sync framer
*/
#define CHUNKSIZE 65536
#define READSIZE  2048

static long th__get_data(OggVorbis_File *vf)
{
	errno = 0 ;

	if( !( vf->callbacks.read_func ) ) return ( -1 ) ;
 
	if( vf->datasource )
	{
		char *buffer = ogg_sync_buffer( &vf->oy, READSIZE );

		long bytes = ( long )( ( vf->callbacks.read_func )( buffer, 1, READSIZE, vf->datasource ) ) ;

		if( bytes > 0 ) ogg_sync_wrote( &vf->oy, bytes );
		if( bytes == 0 && errno ) return( -1 );

		return( bytes );
	}else{
		return( 0 );
	}
}

/* save a tiny smidge of verbosity to make the code more readable */
static int th__seek_helper(OggVorbis_File *vf,ogg_int64_t offset)
{
	if(vf->datasource)
	{ 
	    if(vf->offset != offset)
		{
			if( !( vf->callbacks.seek_func ) ||
				( vf->callbacks.seek_func )( vf->datasource, offset, SEEK_SET ) == -1 )
				return OV_EREAD;
			vf->offset = offset;
			ogg_sync_reset( &vf->oy );
		}
	}else{
		/* shouldn't happen unless someone writes a broken callback */
		return OV_EFAULT;
	}
	return 0;
}

/* The read/seek functions track absolute position within the stream */

/* from the head of the stream, get the next page.	boundary specifies
	 if the function is allowed to fetch more data from the stream (and
	 how much) or only use internally buffered data.

	 boundary: -1) unbounded search
				0) read no additional data; use cached only
				n) search for a new page beginning for n bytes

	 return:   <0) did not find a page (OV_FALSE, OV_EOF, OV_EREAD)
				n) found a page at absolute offset n */

static ogg_int64_t th__get_next_page( OggVorbis_File *vf, ogg_page *og, ogg_int64_t boundary )
{
	if( boundary > 0 ) boundary += vf->offset;

	for(;;)
	{
		long more;

		if( boundary > 0 && vf->offset >= boundary )return( OV_FALSE );
		more = ogg_sync_pageseek( &vf->oy, og );
		
		if( more < 0 )
		{
			/* skipped n bytes */
			vf->offset -= more;
		}
		else
		{
			if( more == 0 )
			{
				/* send more paramedics */
				if(!boundary)return(OV_FALSE);
				{
					long ret=th__get_data(vf);
					if(ret==0)return(OV_EOF);
					if(ret<0)return(OV_EREAD);
				}
			}
			else
			{
				/* got a page.	Return the offset at the page beginning,
								 advance the internal offset past the page end */
				ogg_int64_t ret = vf->offset;
				vf->offset += more;
				return(ret);
			}
		}
	}
}

/* find the latest page beginning before the current stream cursor
	 position. Much dirtier than the above as Ogg doesn't have any
	 backward search linkage.	no 'readp' as it will certainly have to
	 read. */
/* returns offset or OV_EREAD, OV_FAULT */
static ogg_int64_t th__get_prev_page( OggVorbis_File *vf, ogg_int64_t begin, ogg_page *og )
{
	ogg_int64_t end=begin;
	ogg_int64_t ret;
	ogg_int64_t offset=-1;

	while( offset == -1 )
	{
		begin -= CHUNKSIZE ;
		if( begin < 0 ) begin = 0;

		ret = th__seek_helper( vf, begin );
		if( ret ) return( ret );

		while( vf->offset < end )
		{
			memset(og,0,sizeof(*og));
			ret = th__get_next_page( vf, og, end-vf->offset );
			if( ret == OV_EREAD ) return( OV_EREAD );
			if( ret < 0 )
			{
				break;
			}
			else
			{
				offset=ret;
			}
		}
	}

	if( og->header_len == 0 )
	{
		/* we have the offset.	Actually snork and hold the page now */
		ret=th__seek_helper(vf,offset);
		if(ret)return(ret);

		ret=th__get_next_page(vf,og,CHUNKSIZE);
		if(ret<0)
		{
			/* this shouldn't be possible */
			return(OV_EFAULT);
		}
	}

	return(offset);
}

static void th__add_serialno(ogg_page *og,long **serialno_list, int *n)
{
	long s = ogg_page_serialno(og);
	(*n)++;

	if(serialno_list){
		*serialno_list = (long *)_ogg_realloc(*serialno_list, sizeof(*serialno_list)*(*n));
	}else{
		*serialno_list = (long *)_ogg_malloc(sizeof(**serialno_list));
	}
	
	(*serialno_list)[(*n)-1] = s;
}

/* returns nonzero if found */
static int th__lookup_serialno(ogg_page *og, long *serialno_list, int n)
{
	long s = ogg_page_serialno(og);

	if(serialno_list)
	{
		while(n--)
		{
			if(*serialno_list == s) return 1;
			serialno_list++;
		}
	}
	return 0;
}

/* start parsing pages at current offset, remembering all serial
	 numbers.	Stop logging at first non-bos page */
static int th__get_serialnos(OggVorbis_File *vf, long **s, int *n)
{
	ogg_page og;

	*s=NULL;
	*n=0;

	for(;;)
	{
		ogg_int64_t llret=th__get_next_page(vf,&og,CHUNKSIZE);
		if(llret == OV_EOF ) return(0);
		if(llret <  0      ) return((int)llret);
		if(!ogg_page_bos(&og)) return 0;

		/* look for duplicate serialnos; add this one if unique */
		if(th__lookup_serialno(&og,*s,*n))
		{
			if(*s)_ogg_free(*s);
			*s=0;
			*n=0;
			return(OV_EBADHEADER);
		}

		th__add_serialno(&og,s,n);
	}
}

/* finds each bitstream link one at a time using a bisection search
	 (has to begin by knowing the offset of the lb's initial page).
	 Recurses for each link so it can alloc the link storage after
	 finding them all, then unroll and fill the cache at the same time */
static int th__bisect_forward_serialno(OggVorbis_File *vf,
						ogg_int64_t begin,
						ogg_int64_t searched,
						ogg_int64_t end,
						long *currentno_list,
						int	currentnos,
						long m){
	ogg_int64_t endsearched=end;
	ogg_int64_t next=end;
	ogg_page og;
	ogg_int64_t ret;
	
	/* the below guards against garbage seperating the last and
		 first pages of two links. */
	while(searched<endsearched)
	{
		ogg_int64_t bisect;
		
		if(endsearched-searched<CHUNKSIZE)
		{
			bisect=searched;
		}else{
			bisect=(searched+endsearched)/2;
		}
		
		ret=th__seek_helper(vf,bisect);
		if(ret)return((int)ret);

		ret=th__get_next_page(vf,&og,-1);
		if(ret==OV_EREAD)return(OV_EREAD);
		if(ret<0 || !th__lookup_serialno(&og,currentno_list,currentnos))
		{
			endsearched=bisect;
			if(ret>=0)next=ret;
		}else{
			searched=ret+og.header_len+og.body_len;
		}
	}

	{
		long *next_serialno_list=NULL;
		int next_serialnos=0;

		ret=th__seek_helper(vf,next);
		if(ret)return((int)ret);
		ret=th__get_serialnos(vf,&next_serialno_list,&next_serialnos);
		if(ret)return((int)ret);
		
		if(searched>=end || next_serialnos==0){
			vf->links=m+1;
			vf->offsets = (ogg_int64_t *)_ogg_malloc((vf->links+1)*sizeof(*vf->offsets));
			vf->offsets[m+1]=searched;
		}else{
			ret=th__bisect_forward_serialno(vf,next,vf->offset,end,next_serialno_list,next_serialnos,m+1);
			if(ret)return((int)ret);
		}
		
		if(next_serialno_list)_ogg_free(next_serialno_list);
	}
	vf->offsets[m]=begin;
	return(0);
}

/* uses the local ogg_stream storage in vf; this is important for
	 non-streaming input sources */
static int th__fetch_headers( OggVorbis_File *vf, vorbis_info *vi, vorbis_comment *vc, long *serialno, ogg_page *og_ptr )
{
	ogg_page og;
	ogg_packet op;
	int i,ret;
	int allbos=0;

	if( !og_ptr )
	{
		ogg_int64_t llret=th__get_next_page(vf,&og,CHUNKSIZE);
		if(llret==OV_EREAD)return(OV_EREAD);
		if(llret<0)return(OV_ENOTVORBIS);
		og_ptr=&og;
	}

	vorbis_info_init(vi);
	vorbis_comment_init(vc);

	/* extract the first set of vorbis headers we see in the headerset */

	for(;;)
	{
	
		/* if we're past the ID headers, we won't be finding a Vorbis
			 stream in this link */
		if(!ogg_page_bos(og_ptr))
		{
			ret = OV_ENOTVORBIS;
			goto bail_header;
		}

		/* prospective stream setup; we need a stream to get packets */
		ogg_stream_reset_serialno( &vf->os, ogg_page_serialno( og_ptr ) );
		ogg_stream_pagein( &vf->os, og_ptr );

		if( ogg_stream_packetout( &vf->os, &op ) > 0 && vorbis_synthesis_idheader( &op ) )
		{

			/* continue Vorbis header load; past this point, any error will
			 render this link useless (we won't continue looking for more
			 Vorbis streams */
			if( serialno ) *serialno = vf->os.serialno;
			vf->ready_state = STREAMSET;

			ret = vorbis_synthesis_headerin( vi, vc, &op );
			if( ret ) goto bail_header;

			i=0;
			while(i<2)
			{ /* get a page loop */
	
				while(i<2)
				{ /* get a packet loop */

					int result = ogg_stream_packetout( &vf->os, &op );
					if( result == 0	) break;
					if( result == -1 )
					{
						ret = OV_EBADHEADER;
						goto bail_header;
					}
		
					if( ( ret = vorbis_synthesis_headerin( vi, vc, &op ) ) != 0 )
					{
						goto bail_header;
					}

					i++;
				}

				while( i < 2 )
				{
					if( th__get_next_page( vf, og_ptr, CHUNKSIZE ) < 0 )
					{
						ret=OV_EBADHEADER;
						goto bail_header;
					}

					/* if this page belongs to the correct stream, go parse it */
					if( vf->os.serialno == ogg_page_serialno( og_ptr ) )
					{
						ogg_stream_pagein( &vf->os, og_ptr );
						break;
					}

					/* if we never see the final vorbis headers before the link
					 ends, abort */
					if( ogg_page_bos( og_ptr ) )
					{
						if( allbos )
						{
							ret = OV_EBADHEADER;
							goto bail_header;
						}
						else
						{
							allbos=1;
						}
					}

					/* otherwise, keep looking */
				}
			}

			return 0; 
		}

		/* this wasn't vorbis, get next page, try again */
		{
			ogg_int64_t llret =th__get_next_page( vf, og_ptr, CHUNKSIZE );
			if( llret == OV_EREAD ) return( OV_EREAD );
			if( llret <	0				) return( OV_ENOTVORBIS );
		} 
	}

 bail_header:
	vorbis_info_clear( vi );
	vorbis_comment_clear( vc );
	vf->ready_state = OPENED;

	return ret;
}

/* last step of the OggVorbis_File initialization; get all the
	 vorbis_info structs and PCM positions.	Only called by the seekable
	 initialization (local stream storage is hacked slightly; pay
	 attention to how that's done) */

/* this is void and does not propogate errors up because we want to be
	 able to open and use damaged bitstreams as well as we can.	Just
	 watch out for missing information for links in the OggVorbis_File
	 struct */
static void th__prefetch_all_headers( OggVorbis_File *vf, ogg_int64_t dataoffset )
{
	ogg_page og;
	int i;
	ogg_int64_t ret;

	vf->vi			= (vorbis_info    *)_ogg_realloc( vf->vi, vf->links *		sizeof( *vf->vi			 ) );
	vf->vc			= (vorbis_comment *)_ogg_realloc( vf->vc, vf->links *		sizeof( *vf->vc			 ) );
	vf->serialnos	= (long           *)_ogg_malloc(			vf->links *		sizeof( *vf->serialnos	 ) );
	vf->dataoffsets = (ogg_int64_t    *)_ogg_malloc(			vf->links *		sizeof( *vf->dataoffsets ) );
	vf->pcmlengths	= (ogg_int64_t    *)_ogg_malloc(			vf->links * 2 * sizeof( *vf->pcmlengths	 ) );

	for( i = 0; i < vf->links; i++ )
	{
		if( i == 0 )
		{
			/* we already grabbed the initial header earlier.	Just set the offset */
			vf->serialnos[i]	 = vf->current_serialno ;
			vf->dataoffsets[i] = dataoffset ;
			ret = th__seek_helper( vf, dataoffset );
			if(ret)
			{
				vf->dataoffsets[i]=-1;
			}
		}
		else
		{
			/* seek to the location of the initial header */

			ret=th__seek_helper(vf,vf->offsets[i]);
			if(ret)
			{
				vf->dataoffsets[i]=-1;
			}
			else
			{
				if(th__fetch_headers(vf,vf->vi+i,vf->vc+i,vf->serialnos+i,NULL)<0)
				{
					vf->dataoffsets[i]=-1;
				}
				else
				{
					vf->dataoffsets[i]=vf->offset;
				}
			}
		}

		/* fetch beginning PCM offset */

		if(vf->dataoffsets[i]!=-1)
		{
			ogg_int64_t accumulated=0;
			long				lastblock=-1;
			int				 result;

			ogg_stream_reset_serialno(&vf->os,vf->serialnos[i]);

			for(;;)
			{
				ogg_packet op;

				ret=th__get_next_page(vf,&og,-1);
				if(ret<0)
				{
					/* this should not be possible unless the file is
					 truncated/mangled */
					break;
				}
			 
				if(ogg_page_bos(&og)) break;

				if(ogg_page_serialno(&og)!=vf->serialnos[i])
					continue;

				/* count blocksizes of all frames in the page */
				ogg_stream_pagein(&vf->os,&og);
				while( ( result = ogg_stream_packetout( &vf->os, &op ) ) != 0 )
				{
					if(result>0)
					{ /* ignore holes */
						long thisblock=vorbis_packet_blocksize(vf->vi+i,&op);
						if(lastblock!=-1)
							accumulated+=(lastblock+thisblock)>>2;
						lastblock=thisblock;
					}
				}

				if(ogg_page_granulepos(&og)!=-1)
				{
					/* pcm offset of last packet on the first audio page */
					accumulated= ogg_page_granulepos(&og)-accumulated;
					break;
				}
			}

			/* less than zero?	This is a stream with samples trimmed off
				 the beginning, a normal occurrence; set the offset to zero */
			if(accumulated<0) accumulated=0;

			vf->pcmlengths[i*2]=accumulated;
		}

		/* get the PCM length of this link. To do this,
			 get the last page of the stream */
		{
			ogg_int64_t end=vf->offsets[i+1];
			ret=th__seek_helper(vf,end);
			if(ret)
			{
				/* this should not be possible */
				vorbis_info_clear(vf->vi+i);
				vorbis_comment_clear(vf->vc+i);
			}
			else
			{
				for(;;)
				{
					ret=th__get_prev_page(vf,vf->offset,&og);
					if(ret<0)
					{
						/* this should not be possible */
						vorbis_info_clear(vf->vi+i);
						vorbis_comment_clear(vf->vc+i);
						break;
					}
					if(ogg_page_serialno(&og)==vf->serialnos[i])
					{
						if(ogg_page_granulepos(&og)!=-1)
						{
							vf->pcmlengths[i*2+1]=ogg_page_granulepos(&og)-vf->pcmlengths[i*2];
							break;
						}
					}
					vf->offset=ret;
				}
			}
		}
	}
}

static int th__make_decode_ready(OggVorbis_File *vf)
{
	if(vf->ready_state>STREAMSET)return 0;
	if(vf->ready_state<STREAMSET)return OV_EFAULT;
	if(vf->seekable){
		if(vorbis_synthesis_init(&vf->vd,vf->vi+vf->current_link))
			return OV_EBADLINK;
	}else{
		if(vorbis_synthesis_init(&vf->vd,vf->vi))
			return OV_EBADLINK;
	}		
	vorbis_block_init(&vf->vd,&vf->vb);
	vf->ready_state=INITSET;
	vf->bittrack=0.f;
	vf->samptrack=0.f;
	return 0;
}

static int th__open_seekable2(OggVorbis_File *vf)
{
	ogg_int64_t dataoffset=vf->offset,end;
	long *serialno_list=NULL;
	int serialnos=0;
	int ret;
	ogg_page og;

	/* we're partially open and have a first link header state in
		 storage in vf */
	/* we can seek, so set out learning all about this file */
	if(vf->callbacks.seek_func && vf->callbacks.tell_func){
		(vf->callbacks.seek_func)(vf->datasource,0,SEEK_END);
		vf->offset=vf->end=(vf->callbacks.tell_func)(vf->datasource);
	}else{
		vf->offset=vf->end=-1;
	}

	/* If seek_func is implemented, tell_func must also be implemented */
	if(vf->end==-1) return(OV_EINVAL);

	/* We get the offset for the last page of the physical bitstream.
		 Most OggVorbis files will contain a single logical bitstream */
	end=th__get_prev_page(vf,vf->offset,&og);
	if(end<0)return((int)end);

	/* back to beginning, learn all serialnos of first link */
	ret=th__seek_helper(vf,0);
	if(ret)return(ret);
	ret=th__get_serialnos(vf,&serialno_list,&serialnos);
	if(ret)return(ret);

	/* now determine bitstream structure recursively */
	if(th__bisect_forward_serialno(vf,0,0,end+1,serialno_list,serialnos,0)<0)return(OV_EREAD);	
	if(serialno_list)_ogg_free(serialno_list);

	/* the initial header memory is referenced by vf after; don't free it */
	th__prefetch_all_headers(vf,dataoffset);
	return(th_ov_raw_seek(vf,0));
}

/* clear out the current logical bitstream decoder */ 
static void th__decode_clear(OggVorbis_File *vf){
	vorbis_dsp_clear(&vf->vd);
	vorbis_block_clear(&vf->vb);
	vf->ready_state=OPENED;
}

/* fetch and process a packet.	Handles the case where we're at a
	 bitstream boundary and dumps the decoding machine.	If the decoding
	 machine is unloaded, it loads it.	It also keeps pcm_offset up to
	 date (seek and read both use this.	seek uses a special hack with
	 readp). 

	 return: <0) error, OV_HOLE (lost packet) or OV_EOF
						0) need more data (only if readp==0)
			1) got a packet 
*/

static int th__fetch_and_process_packet(OggVorbis_File *vf,
						 ogg_packet *op_in,
						 int readp,
						 int spanp){
	ogg_page og;

	/* handle one packet.	Try to fetch it from current stream state */
	/* extract packets from page */
	for(;;){
		
		/* process a packet if we can.	If the machine isn't loaded,
			 neither is a page */
		if(vf->ready_state==INITSET)
		{
			for(;;)
			{
				ogg_packet op;
				ogg_packet *op_ptr=(op_in?op_in:&op);
				int result=ogg_stream_packetout(&vf->os,op_ptr);
				ogg_int64_t granulepos;

				op_in=NULL;
				if(result==-1)return(OV_HOLE); /* hole in the data. */
				if(result>0)
				{
					/* got a packet.	process it */
					granulepos=op_ptr->granulepos;
					if(!vorbis_synthesis(&vf->vb,op_ptr))
					{ /* lazy check for lazy
										header handling.	The
										header packets aren't
										audio, so if/when we
										submit them,
										vorbis_synthesis will
										reject them */

						/* suck in the synthesis data and track bitrate */
						{
							int oldsamples=vorbis_synthesis_pcmout(&vf->vd,NULL);
							/* for proper use of libvorbis within libvorbisfile,
											 oldsamples will always be zero. */
							if(oldsamples)return(OV_EFAULT);
							
							vorbis_synthesis_blockin(&vf->vd,&vf->vb);
							vf->samptrack+=vorbis_synthesis_pcmout(&vf->vd,NULL)-oldsamples;
							vf->bittrack+=op_ptr->bytes*8;
						}
		
						/* update the pcm offset. */
						if(granulepos!=-1 && !op_ptr->e_o_s)
						{
							int link=(vf->seekable?vf->current_link:0);
							int i,samples;
						
							/* this packet has a pcm_offset on it (the last packet
								 completed on a page carries the offset) After processing
								 (above), we know the pcm position of the *last* sample
								 ready to be returned. Find the offset of the *first*

								 As an aside, this trick is inaccurate if we begin
								 reading anew right at the last page; the end-of-stream
								 granulepos declares the last frame in the stream, and the
								 last packet of the last page may be a partial frame.
								 So, we need a previous granulepos from an in-sequence page
								 to have a reference point.	Thus the !op_ptr->e_o_s clause
								 above */

							if(vf->seekable && link>0) granulepos-=vf->pcmlengths[link*2];
							if(granulepos<0)granulepos=0; /* actually, this
								 shouldn't be possible
								 here unless the stream
								 is very broken */

							samples=vorbis_synthesis_pcmout(&vf->vd,NULL);
			
							granulepos-=samples;
							for(i=0;i<link;i++)
								granulepos+=vf->pcmlengths[i*2+1];
							vf->pcm_offset=granulepos;
						}
						return(1);
					}
				}
				else 
				{
					break;
				}
			}
		}

		if(vf->ready_state>=OPENED)
		{
			ogg_int64_t ret;
			
			for(;;)
			{ 
				/* the loop is not strictly necessary, but there's no sense in
				 doing the extra checks of the larger loop for the common
				 case in a multiplexed bistream where the page is simply
				 part of a different logical bitstream; keep reading until
				we get one with the correct serialno */
	
				if(!readp)return(0);
				if((ret=th__get_next_page(vf,&og,-1))<0)
				{
					return(OV_EOF); /* eof. leave unitialized */
				}

				/* bitrate tracking; add the header's bytes here, the body bytes
					 are done by packet above */
				vf->bittrack+=og.header_len*8;
		
				if(vf->ready_state==INITSET)
				{
					if(vf->current_serialno!=ogg_page_serialno(&og))
					{
						
						/* two possibilities: 
							 1) our decoding just traversed a bitstream boundary
							 2) another stream is multiplexed into this logical section? */
						
						if(ogg_page_bos(&og))
						{
							/* boundary case */
							if(!spanp) return(OV_EOF);
							
							th__decode_clear(vf);
							
							if(!vf->seekable)
							{
								vorbis_info_clear(vf->vi);
								vorbis_comment_clear(vf->vc);
							}
							break;

						}
						else
						{
							continue; /* possibility #2 */
						}
					}
				}

				break;
			}
		}

		/* Do we need to load a new machine before submitting the page? */
		/* This is different in the seekable and non-seekable cases.	

			 In the seekable case, we already have all the header
			 information loaded and cached; we just initialize the machine
			 with it and continue on our merry way.

			 In the non-seekable (streaming) case, we'll only be at a
			 boundary if we just left the previous logical bitstream and
			 we're now nominally at the header of the next bitstream
		*/

		if(vf->ready_state!=INITSET)
		{ 
			int link;

			if(vf->ready_state<STREAMSET)
			{
				if(vf->seekable)
				{
					long serialno = ogg_page_serialno(&og);

					/* match the serialno to bitstream section.	We use this rather than
						 offset positions to avoid problems near logical bitstream
						 boundaries */

					for(link=0;link<vf->links;link++)
						if(vf->serialnos[link]==serialno)break;

					if(link==vf->links) continue; /* not the desired Vorbis
									 bitstream section; keep
									 trying */

					vf->current_serialno=serialno;
					vf->current_link=link;
					
					ogg_stream_reset_serialno(&vf->os,vf->current_serialno);
					vf->ready_state=STREAMSET;
					
				}else{
					/* we're streaming */
					/* fetch the three header packets, build the info struct */
					
					int ret=th__fetch_headers(vf,vf->vi,vf->vc,&vf->current_serialno,&og);
					if(ret)return(ret);
					vf->current_link++;
					link=0;
				}
			}
			
			{
				int ret=th__make_decode_ready(vf);
				if(ret<0)return ret;
			}
		}

		/* the buffered page is the data we want, and we're ready for it;
			 add it to the stream state */
		ogg_stream_pagein(&vf->os,&og);

	}
}

/* if, eg, 64 bit stdio is configured by default, this will build with
	 fseek64 */
static int th__fseek64_wrap(FILE *f,ogg_int64_t off,int whence)
{
	if(f==NULL)return(-1);
	return fseek(f,(long)off,whence);
}

static int th__ov_open1( void *f, OggVorbis_File *vf, char *initial, long ibytes, ov_callbacks callbacks )
{
	int offsettest = ( ( f && callbacks.seek_func ) ? callbacks.seek_func( f, 0, SEEK_CUR ) : -1 );
	int ret;
	
	_MEMSET(vf,0,sizeof(*vf));
	vf->datasource=f;
	vf->callbacks = callbacks;

	/* init the framing state */
	ogg_sync_init(&vf->oy);

	/* perhaps some data was previously read into a buffer for testing
		 against other stream types.	Allow initialization from this
		 previously read data (as we may be reading from a non-seekable
		 stream) */
	if(initial){
		char *buffer=ogg_sync_buffer(&vf->oy,ibytes);
		memcpy(buffer,initial,(size_t)ibytes);
		ogg_sync_wrote(&vf->oy,ibytes);
	}

	/* can we seek? Stevens suggests the seek test was portable */
	if(offsettest != -1 ) vf->seekable = 1;

	/* No seeking yet; Set up a 'single' (current) logical bitstream
		 entry for partial open */
	vf->links=1;
	vf->vi = (vorbis_info    *)_ogg_calloc( ( size_t )vf->links, sizeof( *vf->vi ) );
	vf->vc = (vorbis_comment *)_ogg_calloc( ( size_t )vf->links, sizeof( *vf->vc ) );
	ogg_stream_init( &vf->os, -1 ); /* fill in the serialno later */

	/* Try to fetch the headers, maintaining all the storage */
	ret = th__fetch_headers( vf, vf->vi, vf->vc, &vf->current_serialno, NULL );
	if( ret < 0 )
	{
		vf->datasource=NULL;
		th_ov_clear(vf);
	}else 
		vf->ready_state=PARTOPEN;
	return(ret);
}

static int th__ov_open2(OggVorbis_File *vf){
	if(vf->ready_state != PARTOPEN) return OV_EINVAL;
	vf->ready_state=OPENED;
	if(vf->seekable){
		int ret=th__open_seekable2(vf);
		if(ret){
			vf->datasource=NULL;
			th_ov_clear(vf);
		}
		return(ret);
	}else
		vf->ready_state=STREAMSET;

	return 0;
}


/* clear out the OggVorbis_File struct */
int th_ov_clear(OggVorbis_File *vf){
	if(vf){
		vorbis_block_clear(&vf->vb);
		vorbis_dsp_clear(&vf->vd);
		ogg_stream_clear(&vf->os);
		
		if(vf->vi && vf->links){
			int i;
			for(i=0;i<vf->links;i++)
			{
				vorbis_info_clear(vf->vi+i);
				vorbis_comment_clear(vf->vc+i);
			}
			_ogg_free(vf->vi);
			_ogg_free(vf->vc);
		}
		if(vf->dataoffsets)_ogg_free(vf->dataoffsets);
		if(vf->pcmlengths)_ogg_free(vf->pcmlengths);
		if(vf->serialnos)_ogg_free(vf->serialnos);
		if(vf->offsets)_ogg_free(vf->offsets);
		ogg_sync_clear(&vf->oy);
		if(vf->datasource && vf->callbacks.close_func)
			(vf->callbacks.close_func)(vf->datasource);
		_MEMSET(vf,0,sizeof(*vf));
	}
#ifdef DEBUG_LEAKS
	_VDBG_dump();
#endif
	return(0);
}

/* inspects the OggVorbis file and finds/documents all the logical
	 bitstreams contained in it.	Tries to be tolerant of logical
	 bitstream sections that are truncated/woogie. 

	 return: -1) error
						0) OK
*/

int th_ov_open_callbacks( void *f, OggVorbis_File *vf, char *initial, long ibytes, ov_callbacks callbacks )
{
	int ret = th__ov_open1( f, vf, initial, ibytes, callbacks );
	if( ret ) return ret;
	return th__ov_open2( vf );
}

int th_ov_open(FILE *f,OggVorbis_File *vf,char *initial,long ibytes){
	ov_callbacks callbacks = {
		(size_t (*)(void *, size_t, size_t, void *))Ogg_StreamRead,
		(int    (*)(void *, ogg_int64_t, int))Ogg_StreamSeek64_wrap,
		(int    (*)(void *))Ogg_StreamClose,
		(long   (*)(void *))Ogg_StreamTell
	};

	return th_ov_open_callbacks((void *)f, vf, initial, ibytes, callbacks);
}

int th_ov_fopen(char *path,OggVorbis_File *vf){
	int ret;
	FILE *f = fopen(path,"rb");
	if(!f) return -1;

	ret = th_ov_open(f,vf,NULL,0);
	if(ret) fclose(f);
	return ret;
}

 
/* cheap hack for game usage where downsampling is desirable; there's
	 no need for SRC as we can just do it cheaply in libvorbis. */
 
int th_ov_halfrate(OggVorbis_File *vf,int flag){
	int i;
	if(vf->vi==NULL)return OV_EINVAL;
	if(!vf->seekable)return OV_EINVAL;
	if(vf->ready_state>=STREAMSET)
		th__decode_clear(vf); /* clear out stream state; later on libvorbis
													will be able to swap this on the fly, but
													for now dumping the decode machine is needed
													to reinit the MDCT lookups.	1.1 libvorbis
													is planned to be able to switch on the fly */
	
	for(i=0;i<vf->links;i++){
		if(vorbis_synthesis_halfrate(vf->vi+i,flag)){
			th_ov_halfrate(vf,0);
			return OV_EINVAL;
		}
	}
	return 0;
}

int th_ov_halfrate_p(OggVorbis_File *vf){
	if(vf->vi==NULL)return OV_EINVAL;
	return vorbis_synthesis_halfrate_p(vf->vi);
}

/* Only partially open the vorbis file; test for Vorbisness, and load
	 the headers for the first chain.	Do not seek (although test for
	 seekability).	Use th_ov_test_open to finish opening the file, else
	 th_ov_clear to close/free it. Same return codes as open. */

int th_ov_test_callbacks(void *f,OggVorbis_File *vf,char *initial,long ibytes,
		ov_callbacks callbacks)
{
	return th__ov_open1(f,vf,initial,ibytes,callbacks);
}

int th_ov_test(FILE *f,OggVorbis_File *vf,char *initial,long ibytes){
	ov_callbacks callbacks = {
		(size_t (*)(void *, size_t, size_t, void *))Ogg_StreamRead,
		(int (*)(void *, ogg_int64_t, int))Ogg_StreamSeek64_wrap,
		(int (*)(void *))Ogg_StreamClose,
		(long (*)(void *))Ogg_StreamTell
	};

	return th_ov_test_callbacks((void *)f, vf, initial, ibytes, callbacks);
}
	
int th_ov_test_open(OggVorbis_File *vf){
	if(vf->ready_state!=PARTOPEN)return(OV_EINVAL);
	return th__ov_open2(vf);
}

/* How many logical bitstreams in this physical bitstream? */
long th_ov_streams(OggVorbis_File *vf){
	return vf->links;
}

/* Is the FILE * associated with vf seekable? */
long th_ov_seekable(OggVorbis_File *vf){
	return vf->seekable;
}

/* returns the bitrate for a given logical bitstream or the entire
	 physical bitstream.	If the file is open for random access, it will
	 find the *actual* average bitrate.	If the file is streaming, it
	 returns the nominal bitrate (if set) else the average of the
	 upper/lower bounds (if set) else -1 (unset).

	 If you want the actual bitrate field settings, get them from the
	 vorbis_info structs */

long th_ov_bitrate(OggVorbis_File *vf,int i){
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(i>=vf->links)return(OV_EINVAL);
	if(!vf->seekable && i!=0)return(th_ov_bitrate(vf,0));
	if(i<0){
		ogg_int64_t bits=0;
		int i;
		float br;
		for(i=0;i<vf->links;i++)
			bits+=(vf->offsets[i+1]-vf->dataoffsets[i])*8;
		/* This once read: return(rint(bits/th_ov_time_total(vf,-1)));
		 * gcc 3.x on x86 miscompiled this at optimisation level 2 and above,
		 * so this is slightly transformed to make it work.
		 */
		br = (float)(bits/th_ov_time_total(vf,-1));
		return(long)(rint(br));
	}else{
		if(vf->seekable){
			/* return the actual bitrate */
			return(long)(rint((vf->offsets[i+1]-vf->dataoffsets[i])*8/th_ov_time_total(vf,i)));
		}else{
			/* return nominal if set */
			if(vf->vi[i].bitrate_nominal>0)
			{
				return vf->vi[i].bitrate_nominal;
			}
			else
			{
				if(vf->vi[i].bitrate_upper>0)
				{
					if(vf->vi[i].bitrate_lower>0)
					{
						return (vf->vi[i].bitrate_upper+vf->vi[i].bitrate_lower)/2;
					}
					else
					{
						return vf->vi[i].bitrate_upper;
					}
				}
				return(OV_FALSE);
			}
		}
	}
}

/* returns the actual bitrate since last call.	returns -1 if no
	 additional data to offer since last call (or at beginning of stream),
	 EINVAL if stream is only partially open 
*/
long th_ov_bitrate_instant(OggVorbis_File *vf)
{
	int link=(vf->seekable?vf->current_link:0);
	long ret;
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(vf->samptrack==0)return(OV_FALSE);
	ret=(long)(vf->bittrack/vf->samptrack*vf->vi[link].rate+.5);
	vf->bittrack=0.f;
	vf->samptrack=0.f;
	return(ret);
}

/* Guess */
long th_ov_serialnumber(OggVorbis_File *vf,int i)
{
	if(i>=vf->links)return(th_ov_serialnumber(vf,vf->links-1));
	if(!vf->seekable && i>=0)return(th_ov_serialnumber(vf,-1));
	if(i<0){
		return(vf->current_serialno);
	}else{
		return(vf->serialnos[i]);
	}
}

/* returns: total raw (compressed) length of content if i==-1
						raw (compressed) length of that logical bitstream for i==0 to n
			OV_EINVAL if the stream is not seekable (we can't know the length)
			or if stream is only partially open
*/
ogg_int64_t th_ov_raw_total(OggVorbis_File *vf,int i)
{
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable || i>=vf->links)return(OV_EINVAL);
	if(i<0){
		ogg_int64_t acc=0;
		int i;
		for(i=0;i<vf->links;i++)
			acc+=th_ov_raw_total(vf,i);
		return(acc);
	}else{
		return(vf->offsets[i+1]-vf->offsets[i]);
	}
}

/* returns: total PCM length (samples) of content if i==-1 PCM length
			(samples) of that logical bitstream for i==0 to n
			OV_EINVAL if the stream is not seekable (we can't know the
			length) or only partially open 
*/
ogg_int64_t th_ov_pcm_total(OggVorbis_File *vf,int i)
{
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable || i>=vf->links)return(OV_EINVAL);
	if(i<0){
		ogg_int64_t acc=0;
		int i;
		for(i=0;i<vf->links;i++)
			acc+=th_ov_pcm_total(vf,i);
		return(acc);
	}else{
		return(vf->pcmlengths[i*2+1]);
	}
}

/* returns: total seconds of content if i==-1
						seconds in that logical bitstream for i==0 to n
			OV_EINVAL if the stream is not seekable (we can't know the
			length) or only partially open 
*/
double th_ov_time_total(OggVorbis_File *vf,int i)
{
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable || i>=vf->links)return(OV_EINVAL);
	if(i<0){
		double acc=0;
		int i;
		for(i=0;i<vf->links;i++)
			acc+=th_ov_time_total(vf,i);
		return(acc);
	}else{
		return((double)(vf->pcmlengths[i*2+1])/vf->vi[i].rate);
	}
}

/* seek to an offset relative to the *compressed* data. This also
	 scans packets to update the PCM cursor. It will cross a logical
	 bitstream boundary, but only if it can't get any packets out of the
	 tail of the bitstream we seek to (so no surprises).

	 returns zero on success, nonzero on failure */

int th_ov_raw_seek(OggVorbis_File *vf,ogg_int64_t pos)
{
	ogg_stream_state work_os;
	int ret;

	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable)
		return(OV_ENOSEEK); /* don't dump machine if we can't seek */

	if(pos<0 || pos>vf->end)return(OV_EINVAL);

	/* don't yet clear out decoding machine (if it's initialized), in
		 the case we're in the same link.	Restart the decode lapping, and
		 let th__fetch_and_process_packet deal with a potential bitstream
		 boundary */
	vf->pcm_offset=-1;
	ogg_stream_reset_serialno(&vf->os,vf->current_serialno); /* must set serialno */
	vorbis_synthesis_restart(&vf->vd);
		
	ret=th__seek_helper(vf,pos);
	if(ret)goto seek_error;

	/* we need to make sure the pcm_offset is set, but we don't want to
		 advance the raw cursor past good packets just to get to the first
		 with a granulepos.	That's not equivalent behavior to beginning
		 decoding as immediately after the seek position as possible.

		 So, a hack.	We use two stream states; a local scratch state and
		 the shared vf->os stream state.	We use the local state to
		 scan, and the shared state as a buffer for later decode. 

		 Unfortuantely, on the last page we still advance to last packet
		 because the granulepos on the last page is not necessarily on a
		 packet boundary, and we need to make sure the granpos is
		 correct. 
	*/

	{
		ogg_page og;
		ogg_packet op;
		int lastblock=0;
		int accblock=0;
		int thisblock=0;
		int eosflag=0; 

		ogg_stream_init(&work_os,vf->current_serialno); /* get the memory ready */
		ogg_stream_reset(&work_os); /* eliminate the spurious OV_HOLE
																	 return from not necessarily
																	 starting from the beginning */

		for(;;)
		{
			if(vf->ready_state>=STREAMSET)
			{
				/* snarf/scan a packet if we can */
				int result = ogg_stream_packetout(&work_os,&op);
			
				if(result>0)
				{
					if(vf->vi[vf->current_link].codec_setup)
					{
						thisblock=vorbis_packet_blocksize(vf->vi+vf->current_link,&op);
						if(thisblock<0)
						{
							ogg_stream_packetout(&vf->os,NULL);
							thisblock=0;
						}
						else
						{
							if(eosflag)
							{
								ogg_stream_packetout(&vf->os,NULL);
							}
							else
							{
								if(lastblock) accblock+=(lastblock+thisblock)>>2;
							}
						}

						if(op.granulepos!=-1)
						{
							int i,link=vf->current_link;
							ogg_int64_t granulepos=op.granulepos-vf->pcmlengths[link*2];
							if(granulepos<0)granulepos=0;
				
							for(i=0;i<link;i++)
							{
								granulepos+=vf->pcmlengths[i*2+1];
							}
							vf->pcm_offset=granulepos-accblock;
							break;
						}
						lastblock=thisblock;
						continue;
					}
					else
					{
						ogg_stream_packetout(&vf->os,NULL);
					}
				}
			}
			
			if(!lastblock)
			{
				if(th__get_next_page(vf,&og,-1)<0)
				{
					vf->pcm_offset=th_ov_pcm_total(vf,-1);
					break;
				}
			}
			else
			{
				/* huh?	Bogus stream with packets but no granulepos */
				vf->pcm_offset=-1;
				break;
			}
			
			/* has our decoding just traversed a bitstream boundary? */
			if(vf->ready_state>=STREAMSET)
			{
				if(vf->current_serialno!=ogg_page_serialno(&og))
				{
		
					/* two possibilities: 
						 1) our decoding just traversed a bitstream boundary
						 2) another stream is multiplexed into this logical section? */
						
					if(ogg_page_bos(&og))
					{
						/* we traversed */
						th__decode_clear(vf); /* clear out stream state */
						ogg_stream_clear(&work_os);
					} /* else, do nothing; next loop will scoop another page */
				}
			}

			if(vf->ready_state<STREAMSET)
			{
				int link;
				long serialno = ogg_page_serialno(&og);

				for(link=0;link<vf->links;link++)
					if(vf->serialnos[link]==serialno)break;

				if(link==vf->links) continue; /* not the desired Vorbis
												 bitstream section; keep
												 trying */
				vf->current_link=link;
				vf->current_serialno=serialno;
				ogg_stream_reset_serialno(&vf->os,serialno);
				ogg_stream_reset_serialno(&work_os,serialno); 
				vf->ready_state=STREAMSET;
			}
		
			ogg_stream_pagein(&vf->os,&og);
			ogg_stream_pagein(&work_os,&og);
			eosflag=ogg_page_eos(&og);
		}
	}

	ogg_stream_clear(&work_os);
	vf->bittrack=0.f;
	vf->samptrack=0.f;
	return(0);

 seek_error:
	/* dump the machine so we're in a known state */
	vf->pcm_offset=-1;
	ogg_stream_clear(&work_os);
	th__decode_clear(vf);
	return OV_EBADLINK;
}

/* Page granularity seek (faster than sample granularity because we
	 don't do the last bit of decode to find a specific sample).

	 Seek to the last [granule marked] page preceeding the specified pos
	 location, such that decoding past the returned point will quickly
	 arrive at the requested position. */
int th_ov_pcm_seek_page( OggVorbis_File *vf, ogg_int64_t pos )
{
	int link=-1;
	ogg_int64_t result = 0;
	ogg_int64_t total = th_ov_pcm_total( vf, -1 );
	
	if(	vf->ready_state < OPENED ) return( OV_EINVAL );
	if( !vf->seekable			 ) return( OV_ENOSEEK );

	if( pos < 0 || pos > total ) return( OV_EINVAL );
 
	/* which bitstream section does this pcm offset occur in? */
	for( link = vf->links - 1; link >= 0; link -- )
	{
		total -= vf->pcmlengths[ link * 2 + 1 ];
		if( pos >= total )break;
	}

	/* search within the logical bitstream for the page with the highest
		 pcm_pos preceeding (or equal to) pos.	There is a danger here;
		 missing pages or incorrect frame number information in the
		 bitstream could make our task impossible.	Account for that (it
		 would be an error condition) */

	/* new search algorithm by HB (Nicholas Vinen) */
	{
		ogg_int64_t end		  = vf->offsets[ link + 1 ];
		ogg_int64_t begin	  = vf->dataoffsets[ link ];
		ogg_int64_t begintime = vf->pcmlengths[ link * 2 ];
		ogg_int64_t endtime	  = vf->pcmlengths[ link * 2 + 1 ] + begintime;
		ogg_int64_t target	  = pos - total + begintime;
		ogg_int64_t best	  = -1;
	    int         got_page  = 0;
	
		ogg_page og;

		/* if we have only one page, there will be no bisection.  Grab the page here */
		if( begin == end )
		{
			result = th__seek_helper(vf,begin);
			if( result ) goto seek_error;

			result = th__get_next_page(vf,&og,1);
			if( result < 0 ) goto seek_error;

			got_page = 1;
		}

		while( begin < end )
		{
			ogg_int64_t bisect;
			
			if( end - begin < CHUNKSIZE )
			{
				 bisect = begin;
			}
			else
			{
				/* take a (pretty decent) guess. */
				bisect=begin +
				  (ogg_int64_t)((double)(target-begintime)*(end-begin)/(endtime-begintime))
				  - CHUNKSIZE;
				if( bisect < begin + CHUNKSIZE )
					bisect = begin;
			}
			
			result=th__seek_helper(vf,bisect);
			if(result) goto seek_error;
			
			while(begin<end)
			{
				result=th__get_next_page(vf,&og,end-vf->offset);

				if(result==OV_EREAD) goto seek_error;

				if(result<0)
				{
					if(bisect<=begin+1)
					{
						end=begin; /* found it */
					}
					else
					{
						if(bisect==0) goto seek_error;
						bisect-=CHUNKSIZE;
						if(bisect<=begin) bisect=begin+1;

						result=th__seek_helper(vf,bisect);
						if(result) goto seek_error;
					}
				}
				else
				{
					ogg_int64_t granulepos;
					got_page=1;

					if(ogg_page_serialno(&og)!=vf->serialnos[link])
						continue;

					granulepos=ogg_page_granulepos(&og);
					if(granulepos==-1)continue;
			
					if(granulepos<target)
					{
						best=result;	/* raw offset of packet with granulepos */ 
						begin=vf->offset; /* raw offset of next page */
						begintime=granulepos;
				
						if(target-begintime>44100)break;
						bisect=begin; /* *not* begin + 1 */
					}
					else
					{
						if(bisect<=begin+1)
						{
							end=begin;	/* found it */
						}
						else
						{
							if(end==vf->offset)
							{ /* we're pretty close - we'd be stuck in */
								end=result;
								bisect-=CHUNKSIZE; /* an endless loop otherwise. */
								if(bisect<=begin)bisect=begin+1;
								result=th__seek_helper(vf,bisect);
								if(result)
								{
									goto seek_error;
								}
							}
							else
							{
								end=bisect;
								endtime=granulepos;
								break;
							}
						}
					}
				}
			}
		}

		/* Out of bisection: did it 'fail?' */
		if(best == -1)
		{
			/* Check the 'looking for data in first page' special case;
			bisection would 'fail' because our search target was before the
			first PCM granule position fencepost. */

			if(got_page &&
				begin == vf->dataoffsets[link] &&
				ogg_page_serialno(&og)==vf->serialnos[link])
			{
				/* Yes, this is the beginning-of-stream case. We already have
				our page, right at the beginning of PCM data.  Set state
				and return. */

				vf->pcm_offset=total;

				if(link!=vf->current_link)
				{
					/* Different link; dump entire decode machine */
					th__decode_clear(vf);

					vf->current_link=link;
					vf->current_serialno=vf->serialnos[link];
					vf->ready_state=STREAMSET;

				}
				else
				{
					vorbis_synthesis_restart(&vf->vd);
				}

				ogg_stream_reset_serialno(&vf->os,vf->current_serialno);
				ogg_stream_pagein(&vf->os,&og);

			}
			else
			{
				goto seek_error;
			}
		}
		else
		{
			ogg_page og;
			ogg_packet op;
			
			/* seek */
			result=th__seek_helper(vf,best);
			vf->pcm_offset=-1;
			if(result) goto seek_error;
			result=th__get_next_page(vf,&og,-1);
			if(result<0) goto seek_error;
			
			if(link!=vf->current_link)
			{
				/* Different link; dump entire decode machine */
				th__decode_clear(vf);	
				
				vf->current_link=link;
				vf->current_serialno=vf->serialnos[link];
				vf->ready_state=STREAMSET;
	
			}
			else
			{
				vorbis_synthesis_restart(&vf->vd);
			}

			ogg_stream_reset_serialno(&vf->os,vf->current_serialno);
			ogg_stream_pagein(&vf->os,&og);

			/* pull out all but last packet; the one with granulepos */
			for(;;)
			{
				result=ogg_stream_packetpeek(&vf->os,&op);
				if(result==0)
				{
					/* !!! the packet finishing this page originated on a
									 preceeding page. Keep fetching previous pages until we
									 get one with a granulepos or without the 'continued' flag
									 set.	Then just use raw_seek for simplicity. */
			
//					result=th__seek_helper(vf,best);
//					if(result<0) goto seek_error;
//					
//					for(;;)
//					{
//						result=th__get_prev_page(vf,&og);
//						if(result<0) goto seek_error;
//						if(ogg_page_serialno(&og)==vf->current_serialno && (ogg_page_granulepos(&og)>-1 || !ogg_page_continued(&og)))
//						{
//							return th_ov_raw_seek(vf,result);
//						}
//						vf->offset=result;
//					}
					result = best;
					while( result > vf->dataoffsets[ link ] )
					{
						result=th__get_prev_page( vf, result, &og );
						if( result < 0 ) goto seek_error;
						if( ogg_page_serialno( &og ) == vf->current_serialno &&
						   ( ogg_page_granulepos( &og ) > -1 ||
						   !ogg_page_continued( &og ) ) )
						{
							return th_ov_raw_seek( vf, result );
						}
					}
				}
				if(result<0)
				{
					result = OV_EBADPACKET; 
					goto seek_error;
				}
				if(op.granulepos!=-1)
				{
					vf->pcm_offset=op.granulepos-vf->pcmlengths[vf->current_link*2];
					if(vf->pcm_offset<0)vf->pcm_offset=0;
					vf->pcm_offset+=total;
					break;
				}
				else
				{
					result=ogg_stream_packetout(&vf->os,NULL);
				}
			}
		}
	}
	
	/* verify result */
	if(vf->pcm_offset>pos || pos>th_ov_pcm_total(vf,-1))
	{
		result=OV_EFAULT;
		goto seek_error;
	}
	vf->bittrack=0.f;
	vf->samptrack=0.f;
	return(0);
	
 seek_error:
	/* dump machine so we're in a known state */
	vf->pcm_offset=-1;
	th__decode_clear(vf);
	return (int)result;
}

/* seek to a sample offset relative to the decompressed pcm stream 
	 returns zero on success, nonzero on failure */

int th_ov_pcm_seek(OggVorbis_File *vf,ogg_int64_t pos)
{
	int thisblock, lastblock = 0;
	int ret;

	ret = th_ov_pcm_seek_page( vf, pos );
	if( ret < 0 ) return( ret );

	ret = th__make_decode_ready(vf);
	if( ret ) return ret;

	/* discard leading packets we don't need for the lapping of the
		 position we want; don't decode them */

	for(;;)
	{
		ogg_packet op;
		ogg_page og;

		int ret = ogg_stream_packetpeek(&vf->os,&op);
		if(ret>0)
		{
			thisblock=vorbis_packet_blocksize(vf->vi+vf->current_link,&op);
			if(thisblock<0)
			{
				ogg_stream_packetout(&vf->os,NULL);
				continue; /* non audio packet */
			}
			if(lastblock)vf->pcm_offset+=(lastblock+thisblock)>>2;
			
			if(vf->pcm_offset+((thisblock+vorbis_info_blocksize(vf->vi,1))>>2)>=pos)break;
			
			/* remove the packet from packet queue and track its granulepos */
			ogg_stream_packetout(&vf->os,NULL);
			vorbis_synthesis_trackonly(&vf->vb,&op);	/* set up a vb with
																									 only tracking, no
																									 pcm_decode */
			vorbis_synthesis_blockin(&vf->vd,&vf->vb); 
			
			/* end of logical stream case is hard, especially with exact
				 length positioning. */
			
			if(op.granulepos>-1)
			{
				int i;
				/* always believe the stream markers */
				vf->pcm_offset=op.granulepos-vf->pcmlengths[vf->current_link*2];
				if(vf->pcm_offset<0)vf->pcm_offset=0;

				for(i=0;i<vf->current_link;i++)
					vf->pcm_offset+=vf->pcmlengths[i*2+1];
			}
	
			lastblock=thisblock;
			
		}
		else
		{
			if(ret<0 && ret!=OV_HOLE)break;
			
			/* suck in a new page */
			if(th__get_next_page(vf,&og,-1)<0)break;
			if(ogg_page_bos(&og))th__decode_clear(vf);
			
			if(vf->ready_state<STREAMSET)
			{
				long serialno=ogg_page_serialno(&og);
				int link;
				
				for(link=0;link<vf->links;link++)
					if(vf->serialnos[link]==serialno)break;
				if(link==vf->links) continue; 
				vf->current_link=link;
				
				vf->ready_state=STREAMSET;			
				vf->current_serialno=ogg_page_serialno(&og);
				ogg_stream_reset_serialno(&vf->os,serialno); 
				ret=th__make_decode_ready(vf);
				if(ret)return ret;
				lastblock=0;
			}

			ogg_stream_pagein(&vf->os,&og);
		}
	}

	vf->bittrack=0.f;
	vf->samptrack=0.f;
	/* discard samples until we reach the desired position. Crossing a
		 logical bitstream boundary with abandon is OK. */
    int hs=vorbis_synthesis_halfrate_p(vf->vi);
    while(vf->pcm_offset<((pos>>hs)<<hs))
	{
		ogg_int64_t target=(pos-vf->pcm_offset)>>hs;
		long samples=vorbis_synthesis_pcmout(&vf->vd,NULL);

		if(samples>target)samples=(long)target;
		vorbis_synthesis_read(&vf->vd,samples);
		vf->pcm_offset+=samples<<hs;
		
		if(samples<target)
		{
			if(th__fetch_and_process_packet(vf,NULL,1,1)<=0)
			{
				vf->pcm_offset=th_ov_pcm_total(vf,-1); /* eof */
			}
		}
	}
	return 0;
}

/* seek to a playback time relative to the decompressed pcm stream 
	 returns zero on success, nonzero on failure */
int th_ov_time_seek(OggVorbis_File *vf,double seconds)
{
	/* translate time to PCM position and call th_ov_pcm_seek */

	int link=-1;
	ogg_int64_t pcm_total=0;
	double time_total=0.;

	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable)return(OV_ENOSEEK);
	if(seconds<0)return(OV_EINVAL);
	
	/* which bitstream section does this time offset occur in? */
	for(link=0;link<vf->links;link++)
	{
		double addsec = th_ov_time_total(vf,link);
		if(seconds<time_total+addsec)break;
		time_total+=addsec;
		pcm_total+=vf->pcmlengths[link*2+1];
	}

	if(link==vf->links)return(OV_EINVAL);

	/* enough information to convert time offset to pcm offset */
	{
		ogg_int64_t target=(ogg_int64_t)(pcm_total+(seconds-time_total)*vf->vi[link].rate);
		return(th_ov_pcm_seek(vf,target));
	}
}

/* page-granularity version of th_ov_time_seek 
	 returns zero on success, nonzero on failure */
int th_ov_time_seek_page(OggVorbis_File *vf,double seconds)
{
	/* translate time to PCM position and call th_ov_pcm_seek */

	int link=-1;
	ogg_int64_t pcm_total=0;
	double time_total=0.;

	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable)return(OV_ENOSEEK);
	if(seconds<0)return(OV_EINVAL);
	
	/* which bitstream section does this time offset occur in? */
	for(link=0;link<vf->links;link++)
	{
		double addsec = th_ov_time_total(vf,link);
		if(seconds<time_total+addsec)break;
		time_total+=addsec;
		pcm_total+=vf->pcmlengths[link*2+1];
	}

	if(link==vf->links)return(OV_EINVAL);

	/* enough information to convert time offset to pcm offset */
	{
		ogg_int64_t target=(ogg_int64_t)(pcm_total+(seconds-time_total)*vf->vi[link].rate);
		return(th_ov_pcm_seek_page(vf,target));
	}
}

/* tell the current stream offset cursor.	Note that seek followed by
	 tell will likely not give the set offset due to caching */
ogg_int64_t th_ov_raw_tell(OggVorbis_File *vf)
{
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	return(vf->offset);
}

/* return PCM offset (sample) of next PCM sample to be read */
ogg_int64_t th_ov_pcm_tell(OggVorbis_File *vf)
{
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	return(vf->pcm_offset);
}

/* return time offset (seconds) of next PCM sample to be read */
double th_ov_time_tell(OggVorbis_File *vf)
{
	int link=0;
	ogg_int64_t pcm_total=0;
	double time_total=0.f;
	
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(vf->seekable){
		pcm_total=th_ov_pcm_total(vf,-1);
		time_total=th_ov_time_total(vf,-1);
	
		/* which bitstream section does this time offset occur in? */
		for(link=vf->links-1;link>=0;link--){
			pcm_total-=vf->pcmlengths[link*2+1];
			time_total-=th_ov_time_total(vf,link);
			if(vf->pcm_offset>=pcm_total)break;
		}
	}

	return((double)time_total+(double)(vf->pcm_offset-pcm_total)/vf->vi[link].rate);
}

/*	link:	 -1) return the vorbis_info struct for the bitstream section
								currently being decoded
					 0-n) to request information for a specific bitstream section
		
		In the case of a non-seekable bitstream, any call returns the
		current bitstream.	NULL in the case that the machine is not
		initialized */

vorbis_info *th_ov_info(OggVorbis_File *vf,int link)
{
	if(vf->seekable)
	{
		if(link<0)
		{
			if(vf->ready_state>=STREAMSET)
			{
				return vf->vi+vf->current_link;
			}
			else
			{
				return vf->vi;
			}
		}
		else
		{
			if(link>=vf->links)
			{
				return NULL;
			}
			else
			{
				return vf->vi+link;
			}
		}
	}
	else
	{
		return vf->vi;
	}
}

/* grr, strong typing, grr, no templates/inheritence, grr */
vorbis_comment *th_ov_comment(OggVorbis_File *vf,int link){
	if(vf->seekable)
	{
		if(link<0)
		{
			if(vf->ready_state>=STREAMSET) return vf->vc+vf->current_link;
			else                           return vf->vc;
		}
		else
		{
			if(link>=vf->links) return NULL;
			else                return vf->vc+link;
		}
	}
	else
	{
		return vf->vc;
	}
}

static int th_host_is_big_endian()
 {
	ogg_int32_t pattern = 0xfeedface; /* deadbeef */
	unsigned char *bytewise = (unsigned char *)&pattern;
	if (bytewise[0] == 0xfe) return 1;
	return 0;
}

/* up to this point, everything could more or less hide the multiple
	 logical bitstream nature of chaining from the toplevel application
	 if the toplevel application didn't particularly care.	However, at
	 the point that we actually read audio back, the multiple-section
	 nature must surface: Multiple bitstream sections do not necessarily
	 have to have the same number of channels or sampling rate.

	 th_ov_read returns the sequential logical bitstream number currently
	 being decoded along with the PCM data in order that the toplevel
	 application can take action on channel/sample rate changes.	This
	 number will be incremented even for streamed (non-seekable) streams
	 (for seekable streams, it represents the actual logical bitstream
	 index within the physical bitstream.	Note that the accessor
	 functions above are aware of this dichotomy).

	 input values: buffer) a buffer to hold packed PCM data for return
		 length) the byte length requested to be placed into buffer
		 bigendianp) should the data be packed LSB first (0) or
								 MSB first (1)
		 word) word size for output.	currently 1 (byte) or 
					 2 (16 bit short)

	 return values: <0) error/hole in data (OV_HOLE), partial open (OV_EINVAL)
									 0) EOF
			 n) number of bytes of PCM actually returned.	The
			 below works on a packet-by-packet basis, so the
			 return length is not related to the 'length' passed
			 in, just guaranteed to fit.

			*section) set to the logical bitstream number */

long th_ov_read( OggVorbis_File *vf, char *buffer, int length, int bigendianp, int word, int sgned, int *bitstream )
{
	int i,j;
	int host_endian = th_host_is_big_endian();

	float **pcm;
	long samples;

	if( vf->ready_state < OPENED ) return ( OV_EINVAL ) ;

	while( 1 )
	{
		if( vf->ready_state == INITSET )
		{
			samples = vorbis_synthesis_pcmout( &vf->vd, &pcm );
			if( samples )break;
		}

		/* suck in another packet */
		{
			int ret=th__fetch_and_process_packet(vf,NULL,1,1);
			if(ret==OV_EOF) return(0);
			if(ret<=0) return(ret);
		}

	}

	if(samples>0)
	{
		/* yay! proceed to pack data into the byte buffer */
		
		long channels=th_ov_info(vf,-1)->channels;
		long bytespersample=word * channels;
		vorbis_fpu_control fpu;
		if(samples>length/bytespersample)samples=length/bytespersample;

		if(samples <= 0)
			return OV_EINVAL;
		
		/* a tight loop to pack each size */
		{
			int val;
			if(word==1)
			{
				int off=(sgned?0:128);
				vorbis_fpu_setround(&fpu);
				for(j=0;j<samples;j++)
				{
					for(i=0;i<channels;i++)
					{
						val=vorbis_ftoi(pcm[i][j]*128.f);
						if(val>127)val=127;
						else if(val<-128)val=-128;
						*buffer++=(char)(val+off);
					}
				}
				vorbis_fpu_restore(fpu);
			}
			else
			{
				int off=(sgned?0:32768);
	
				if(host_endian==bigendianp)
				{
					if(sgned)
					{
						vorbis_fpu_setround(&fpu);
						for(i=0;i<channels;i++)
						{ /* It's faster in this order */
							float *src=pcm[i];
							short *dest=((short *)buffer)+i;
							for(j=0;j<samples;j++)
							{
								val=vorbis_ftoi(src[j]*32768.f);
								if(val>32767)
								{
									val=32767;
								}
								else
								if(val<-32768)
								{
									val=-32768;
								}
								*dest=(short)val;
								dest+=channels;
							}
						}
						vorbis_fpu_restore(fpu);
					}
					else
					{
						vorbis_fpu_setround(&fpu);
						for(i=0;i<channels;i++)
						{
							float *src=pcm[i];
							short *dest=((short *)buffer)+i;
							for(j=0;j<samples;j++)
							{
								val=vorbis_ftoi(src[j]*32768.f);
								if(val>32767)val=32767;
								else if(val<-32768)val=-32768;
								*dest=(short)(val+off);
								dest+=channels;
							}
						}
						vorbis_fpu_restore(fpu);
			
					}
				}
				else
				if(bigendianp)
				{
					vorbis_fpu_setround(&fpu);
					for(j=0;j<samples;j++)
					{
						for(i=0;i<channels;i++){
							val=vorbis_ftoi(pcm[i][j]*32768.f);
							if(val>32767)val=32767;
							else if(val<-32768)val=-32768;
							val+=off;
							*buffer++=(char)((val>>8));
							*buffer++=(char)(val&0xff);
						}
					}
					vorbis_fpu_restore(fpu);
				}
				else
				{
					int val;
					vorbis_fpu_setround(&fpu);
					for(j=0;j<samples;j++)
					{
						for(i=0;i<channels;i++)
						{
							val=vorbis_ftoi(pcm[i][j]*32768.f);
							if(val>32767)val=32767;
							else if(val<-32768)val=-32768;
							val+=off;
							*buffer++=(char)(val&0xff);
							*buffer++=(char)((val>>8));
						}
					}
					vorbis_fpu_restore(fpu);	
		
				}
			}
		}
		
		vorbis_synthesis_read(&vf->vd,samples);
		vf->pcm_offset+=samples;
		if(bitstream)*bitstream=vf->current_link;
		return(samples*bytespersample);
	}
	else
	{
		return(samples);
	}
}

/* input values: pcm_channels) a float vector per channel of output
		 length) the sample length being read by the app

	 return values: <0) error/hole in data (OV_HOLE), partial open (OV_EINVAL)
									 0) EOF
			 n) number of samples of PCM actually returned.	The
			 below works on a packet-by-packet basis, so the
			 return length is not related to the 'length' passed
			 in, just guaranteed to fit.

			*section) set to the logical bitstream number */



long th_ov_read_float(OggVorbis_File *vf,float ***pcm_channels,int length, int *bitstream)
{

	if(vf->ready_state<OPENED)return(OV_EINVAL);

	for(;;)
	{
		if(vf->ready_state==INITSET)
		{
			float **pcm;
			long samples=vorbis_synthesis_pcmout(&vf->vd,&pcm);
			if(samples)
			{
				if(pcm_channels)*pcm_channels=pcm;
				if(samples>length)samples=length;
				vorbis_synthesis_read(&vf->vd,samples);
				vf->pcm_offset+=samples;
				if(bitstream)*bitstream=vf->current_link;
				return samples;
			}
		}

		/* suck in another packet */
		{
			int ret=th__fetch_and_process_packet(vf,NULL,1,1);
			if(ret==OV_EOF)return(0);
			if(ret<=0)return(ret);
		}

	}
}


static void th__ov_splice(float **pcm,float **lappcm,
					 int n1, int n2,
					 int ch1, int ch2,
					 float *w1, float *w2){
	int i,j;
	float *w=w1;
	int n=n1;

	if(n1>n2){
		n=n2;
		w=w2;
	}

	/* splice */
	for(j=0;j<ch1 && j<ch2;j++){
		float *s=lappcm[j];
		float *d=pcm[j];

		for(i=0;i<n;i++){
			float wd=w[i]*w[i];
			float ws=1.0f-wd;
			d[i]=d[i]*wd + s[i]*ws;
		}
	}
	/* window from zero */
	for(;j<ch2;j++){
		float *d=pcm[j];
		for(i=0;i<n;i++){
			float wd=w[i]*w[i];
			d[i]=d[i]*wd;
		}
	}

}
		
/* make sure vf is INITSET */
static int th__ov_initset(OggVorbis_File *vf){
	for(;;){
		if(vf->ready_state==INITSET)break;
		/* suck in another packet */
		{
			int ret=th__fetch_and_process_packet(vf,NULL,1,0);
			if(ret<0 && ret!=OV_HOLE)return(ret);
		}
	}
	return 0;
}

/* make sure vf is INITSET and that we have a primed buffer; if
	 we're crosslapping at a stream section boundary, this also makes
	 sure we're sanity checking against the right stream information */
static int th__ov_initprime(OggVorbis_File *vf){
	vorbis_dsp_state *vd=&vf->vd;
	for(;;){
		if(vf->ready_state==INITSET)
			if(vorbis_synthesis_pcmout(vd,NULL))break;
		
		/* suck in another packet */
		{
			int ret=th__fetch_and_process_packet(vf,NULL,1,0);
			if(ret<0 && ret!=OV_HOLE)return(ret);
		}
	}	
	return 0;
}

/* grab enough data for lapping from vf; this may be in the form of
	 unreturned, already-decoded pcm, remaining PCM we will need to
	 decode, or synthetic postextrapolation from last packets. */
static void th__ov_getlap(OggVorbis_File *vf,vorbis_info *vi,vorbis_dsp_state *vd,
					 float **lappcm,int lapsize){
	int lapcount=0,i;
	float **pcm;

	/* try first to decode the lapping data */
	while(lapcount<lapsize)
	{
		int samples=vorbis_synthesis_pcmout(vd,&pcm);
		if(samples)
		{
			if(samples>lapsize-lapcount)samples=lapsize-lapcount;
			for(i=0;i<vi->channels;i++)
				memcpy(lappcm[i]+lapcount,pcm[i],sizeof(**pcm)*samples);
			lapcount+=samples;
			vorbis_synthesis_read(vd,samples);
		}
		else
		{
		/* suck in another packet */
			int ret=th__fetch_and_process_packet(vf,NULL,1,0); /* do *not* span */
			if(ret==OV_EOF)break;
		}
	}
	if(lapcount<lapsize){
		/* failed to get lapping data from normal decode; pry it from the
			 postextrapolation buffering, or the second half of the MDCT
			 from the last packet */
		int samples=vorbis_synthesis_lapout(&vf->vd,&pcm);
		if(samples==0)
		{
			for(i=0;i<vi->channels;i++)
				_MEMSET(lappcm[i]+lapcount,0,sizeof(**pcm)*lapsize-lapcount);
			lapcount=lapsize;
		}
		else
		{
			if(samples>lapsize-lapcount)samples=lapsize-lapcount;
			for(i=0;i<vi->channels;i++)
				memcpy(lappcm[i]+lapcount,pcm[i],sizeof(**pcm)*samples);
			lapcount+=samples;
		}
	}
}

/* this sets up crosslapping of a sample by using trailing data from
	 sample 1 and lapping it into the windowing buffer of sample 2 */
int th_ov_crosslap(OggVorbis_File *vf1, OggVorbis_File *vf2)
{
	vorbis_info *vi1,*vi2;
	float **lappcm;
	float **pcm;
	float *w1,*w2;
	int n1,n2,i,ret,hs1,hs2;

	if(vf1==vf2)return(0); /* degenerate case */
	if(vf1->ready_state<OPENED)return(OV_EINVAL);
	if(vf2->ready_state<OPENED)return(OV_EINVAL);

	/* the relevant overlap buffers must be pre-checked and pre-primed
		 before looking at settings in the event that priming would cross
		 a bitstream boundary.	So, do it now */

	ret=th__ov_initset(vf1);
	if(ret)return(ret);
	ret=th__ov_initprime(vf2);
	if(ret)return(ret);

	vi1=th_ov_info(vf1,-1);
	vi2=th_ov_info(vf2,-1);
	hs1=th_ov_halfrate_p(vf1);
	hs2=th_ov_halfrate_p(vf2);

	lappcm = (float **)alloca(sizeof(*lappcm)*vi1->channels);
	n1=vorbis_info_blocksize(vi1,0)>>(1+hs1);
	n2=vorbis_info_blocksize(vi2,0)>>(1+hs2);
	w1=vorbis_window(&vf1->vd,0);
	w2=vorbis_window(&vf2->vd,0);

	for(i=0;i<vi1->channels;i++)
		lappcm[i] = (float *)alloca(sizeof(**lappcm)*n1);

	th__ov_getlap(vf1,vi1,&vf1->vd,lappcm,n1);

	/* have a lapping buffer from vf1; now to splice it into the lapping
		 buffer of vf2 */
	/* consolidate and expose the buffer. */
	vorbis_synthesis_lapout(&vf2->vd,&pcm);
	th_analysis_output_always((char *)"pcmL",0,pcm[0],n1*2,0,0,0);
	th_analysis_output_always((char *)"pcmR",0,pcm[1],n1*2,0,0,0);

	/* splice */
	th__ov_splice(pcm,lappcm,n1,n2,vi1->channels,vi2->channels,w1,w2);
	
	/* done */
	return(0);
}

static int th__ov_64_seek_lap(OggVorbis_File *vf,ogg_int64_t pos,
				 int (*localseek)(OggVorbis_File *,ogg_int64_t))
{
	vorbis_info *vi;
	float **lappcm;
	float **pcm;
	float *w1,*w2;
	int n1,n2,ch1,ch2,hs;
	int i,ret;

	if(vf->ready_state<OPENED)return(OV_EINVAL);
	ret=th__ov_initset(vf);
	if(ret)return(ret);
	vi=th_ov_info(vf,-1);
	hs=th_ov_halfrate_p(vf);
	
	ch1=vi->channels;
	n1=vorbis_info_blocksize(vi,0)>>(1+hs);
	w1=vorbis_window(&vf->vd,0);	/* window arrays from libvorbis are
					 persistent; even if the decode state
					 from this link gets dumped, this
					 window array continues to exist */

	lappcm = (float **)alloca(sizeof(*lappcm)*ch1);
	for(i=0;i<ch1;i++)
		lappcm[i] = (float *)alloca(sizeof(**lappcm)*n1);
	th__ov_getlap(vf,vi,&vf->vd,lappcm,n1);

	/* have lapping data; seek and prime the buffer */
	ret=localseek(vf,pos);
	if(ret)return ret;
	ret=th__ov_initprime(vf);
	if(ret)return(ret);

 /* Guard against cross-link changes; they're perfectly legal */
	vi=th_ov_info(vf,-1);
	ch2=vi->channels;
	n2=vorbis_info_blocksize(vi,0)>>(1+hs);
	w2=vorbis_window(&vf->vd,0);

	/* consolidate and expose the buffer. */
	vorbis_synthesis_lapout(&vf->vd,&pcm);

	/* splice */
	th__ov_splice(pcm,lappcm,n1,n2,ch1,ch2,w1,w2);

	/* done */
	return(0);
}

int th_ov_raw_seek_lap(OggVorbis_File *vf,ogg_int64_t pos){
	return th__ov_64_seek_lap(vf,pos,th_ov_raw_seek);
}

int th_ov_pcm_seek_lap(OggVorbis_File *vf,ogg_int64_t pos){
	return th__ov_64_seek_lap(vf,pos,th_ov_pcm_seek);
}

int th_ov_pcm_seek_page_lap(OggVorbis_File *vf,ogg_int64_t pos){
	return th__ov_64_seek_lap(vf,pos,th_ov_pcm_seek_page);
}

static int th__ov_d_seek_lap(OggVorbis_File *vf,double pos,
				 int (*localseek)(OggVorbis_File *,double))
{
	vorbis_info *vi;
	float **lappcm;
	float **pcm;
	float *w1,*w2;
	int n1,n2,ch1,ch2,hs;
	int i,ret;

	if(vf->ready_state<OPENED)return(OV_EINVAL);
	ret=th__ov_initset(vf);
	if(ret)return(ret);
	vi=th_ov_info(vf,-1);
	hs=th_ov_halfrate_p(vf);

	ch1=vi->channels;
	n1=vorbis_info_blocksize(vi,0)>>(1+hs);
	w1=vorbis_window(&vf->vd,0);	/* window arrays from libvorbis are
					 persistent; even if the decode state
					 from this link gets dumped, this
					 window array continues to exist */

	lappcm = (float **)alloca(sizeof(*lappcm)*ch1);
	for(i=0;i<ch1;i++)
		lappcm[i] = (float *)alloca(sizeof(**lappcm)*n1);
	th__ov_getlap(vf,vi,&vf->vd,lappcm,n1);

	/* have lapping data; seek and prime the buffer */
	ret=localseek(vf,pos);
	if(ret)return ret;
	ret=th__ov_initprime(vf);
	if(ret)return(ret);

 /* Guard against cross-link changes; they're perfectly legal */
	vi=th_ov_info(vf,-1);
	ch2=vi->channels;
	n2=vorbis_info_blocksize(vi,0)>>(1+hs);
	w2=vorbis_window(&vf->vd,0);

	/* consolidate and expose the buffer. */
	vorbis_synthesis_lapout(&vf->vd,&pcm);

	/* splice */
	th__ov_splice(pcm,lappcm,n1,n2,ch1,ch2,w1,w2);

	/* done */
	return(0);
}

int th_ov_time_seek_lap(OggVorbis_File *vf,double pos){
	return th__ov_d_seek_lap(vf,pos,th_ov_time_seek);
}

int th_ov_time_seek_page_lap(OggVorbis_File *vf,double pos){
	return th__ov_d_seek_lap(vf,pos,th_ov_time_seek_page);
}

#define toBARK(n)   (13.1f*atan(.00074f*(n))+2.24f*atan((n)*(n)*1.85e-8f)+1e-4f*(n))
#define todB(x)   (*(x)==0?-400.f:log(*(x)**(x))*4.34294480f)

/* there was no great place to put this.... */
void th_analysis_output_always(char *base,int i,float *v,int n,int bark,int dB,ogg_int64_t off){
  int j;
  FILE *of;
  char buffer[80];

  sprintf(buffer,"%s_%d.m",base,i);
  of=fopen(buffer,"w");

  if(!of)perror("failed to open data dump file");

  for(j=0;j<n;j++){
    if(bark){
      float b=(float)(toBARK((4000.f*j/n)+0.25f));
      fprintf(of,"%f ",b);
    }else
      if(off!=0)
        fprintf(of,"%f ",(double)(j+off)/8000.);
      else
        fprintf(of,"%f ",(double)j);

    if(dB){
      float val;
      if(v[j]==0.)
        val=-140.;
      else
        val=(float)(todB(v+j));
      fprintf(of,"%f\n",val);
    }else{
      fprintf(of,"%f\n",v[j]);
    }
  }
  fclose(of);
}

#endif // DX_NON_OGGTHEORA
#endif // DX_NON_OGGVORBIS





























// Ogg Vorbis 関係のプログラム

#ifndef DX_NON_OGGVORBIS

// 変換後のデータサイズを得る
extern LONGLONG GetSoundConvertDestSize_OGG( SOUNDCONV *SoundConv )
{
//	SOUNDCONV_OGG *oggdata = &SoundConv->OggTypeData ;
	SOUNDCONV_OGG *oggdata = (SOUNDCONV_OGG *)SoundConv->ConvFunctionBuffer ;

#ifndef DX_NON_OGGTHEORA
	if( SoundConv->OggVorbisFromTheoraFile )
	{
		return th_ov_pcm_total( &oggdata->File, -1 ) * SoundConv->OutFormat.nChannels * oggdata->Word ;
	}
	else
#endif
	{
		return ov_pcm_total( &oggdata->File, -1 ) * SoundConv->OutFormat.nChannels * oggdata->Word ;
	}
}

// ＯＧＧファイルのセットアップ処理を行う( [戻] -1:エラー )
extern	int SetupSoundConvert_OGG( SOUNDCONV *SoundConv )
{
	ov_callbacks ogfunc ;
	vorbis_info *info ;
//	SOUNDCONV_OGG *oggdata = &SoundConv->OggTypeData ;
	SOUNDCONV_OGG *oggdata = (SOUNDCONV_OGG *)SoundConv->ConvFunctionBuffer ;
	STREAMDATA *Stream = &SoundConv->Stream ;
	int res, Byte ;
	
	oggdata->FileInitializeFlag = FALSE ;

	// コールバック関数を用意する
	ogfunc.read_func = ( size_t ( * )( void *, size_t, size_t, void * ) )Ogg_StreamRead ;
	ogfunc.seek_func = ( int  ( * )( void *, ogg_int64_t, int ) )Ogg_StreamSeek64_wrap ;
	ogfunc.close_func = ( int ( * )( void * ) )Ogg_StreamClose ;
	ogfunc.tell_func = ( long ( * )( void * ) )Ogg_StreamTell ;

	// Ｏｇｇファイルのセットアップ
#ifndef DX_NON_OGGTHEORA
	if( SoundConv->OggVorbisFromTheoraFile )
	{
		res = th_ov_open_callbacks( Stream, &oggdata->File, NULL, 0, ogfunc ) ;
	}
	else
#endif
	{
		res = ov_open_callbacks( Stream, &oggdata->File, NULL, 0, ogfunc ) ;
	}
	if( res != 0 ) return -1 ;
	oggdata->FileInitializeFlag = TRUE ;

	// ファイルの情報を得る
#ifndef DX_NON_OGGTHEORA
	if( SoundConv->OggVorbisFromTheoraFile )
	{
		info = th_ov_info( &oggdata->File, -1 ) ;
	}
	else
#endif
	{
		info = ov_info( &oggdata->File, -1 ) ;
	}

	// ループ情報がコメント内にあるかチェック
	{
		vorbis_comment *comment ;
		int i ;
		LONGLONG LoopStart  = -1 ;
		LONGLONG LoopLength = -1 ;

#ifndef DX_NON_OGGTHEORA
		if( SoundConv->OggVorbisFromTheoraFile )
		{
			comment = th_ov_comment( &oggdata->File, -1 ) ;
		}
		else
#endif
		{
			comment = ov_comment( &oggdata->File, -1 ) ;
		}

		for( i = 0 ; i < comment->comments ; i ++ )
		{
			if( CL_strncmp( DX_CHARCODEFORMAT_ASCII, comment->user_comments[ i ], "LOOPSTART=", 10 ) == 0 )
			{
				LoopStart = CL_atoi64( DX_CHARCODEFORMAT_ASCII, &comment->user_comments[ i ][ 10 ] ) ;
			}
			else
			if( CL_strncmp( DX_CHARCODEFORMAT_ASCII, comment->user_comments[ i ], "LOOPLENGTH=", 11 ) == 0 )
			{
				LoopLength = CL_atoi64( DX_CHARCODEFORMAT_ASCII, &comment->user_comments[ i ][ 11 ] ) ;
			}
		}

		if( LoopStart >= 0 && LoopLength > 0 )
		{
			oggdata->LoopStart = LoopStart ;
			oggdata->LoopEnd   = LoopStart + LoopLength ;
		}
		else
		if( LoopStart >= 0 )
		{
			oggdata->LoopStart = LoopStart ;
#ifndef DX_NON_OGGTHEORA
			if( SoundConv->OggVorbisFromTheoraFile )
			{
				oggdata->LoopEnd = ( int )th_ov_pcm_total( &oggdata->File, -1 ) ;
			}
			else
#endif
			{
				oggdata->LoopEnd = ( int )ov_pcm_total( &oggdata->File, -1 ) ;
			}
		}
		else
		if( LoopLength > 0 )
		{
			oggdata->LoopStart = 0 ;
			oggdata->LoopEnd   = LoopLength ;
		}
		else
		{
			oggdata->LoopStart = -1 ;
			oggdata->LoopEnd   = -1 ;
		}
	}

	// ＷＡＶＥフォーマットをセットする
	Byte = SoundConv->OggVorbisBitDepth ;
	SoundConv->OutFormat.cbSize          = 0 ;
	SoundConv->OutFormat.wFormatTag      = WAVE_FORMAT_PCM ;
	SoundConv->OutFormat.nChannels       = ( WORD )info->channels ;
	SoundConv->OutFormat.nSamplesPerSec  = ( DWORD )info->rate ;
	SoundConv->OutFormat.wBitsPerSample  = ( WORD )( Byte * 8 ) ;
	SoundConv->OutFormat.nBlockAlign     = ( WORD )( Byte * SoundConv->OutFormat.nChannels ) ;
	SoundConv->OutFormat.nAvgBytesPerSec = SoundConv->OutFormat.nSamplesPerSec * SoundConv->OutFormat.nBlockAlign ;
	oggdata->Word = Byte ;

	// 変換後のＰＣＭデータを一時的に保存するメモリ領域のサイズをセット
	SoundConv->DestDataSize = 4096 ;

	// 変換後のＰＣＭデータのサイズをセット
	SoundConv->DataSize = GetSoundConvertDestSize_OGG( SoundConv );

	// タイプセット
	SoundConv->MethodType = SOUND_METHODTYPE_OGG ;
	
	// 終了
	return 0 ;
}


// ＯＧＧファイルの後始末処理を行う
extern	int TerminateSoundConvert_OGG( SOUNDCONV *SoundConv )
{
//	SOUNDCONV_OGG *oggdata = &SoundConv->OggTypeData ;
	SOUNDCONV_OGG *oggdata = (SOUNDCONV_OGG *)SoundConv->ConvFunctionBuffer ;

	if( oggdata->FileInitializeFlag == TRUE )
	{
#ifndef DX_NON_OGGTHEORA
		if( SoundConv->OggVorbisFromTheoraFile )
		{
			th_ov_clear( &oggdata->File ) ;
		}
		else
#endif
		{
			ov_clear( &oggdata->File ) ;
		}
		oggdata->FileInitializeFlag = FALSE ;
	}
	
	return 0 ;
}


// 変換後のバッファにデータを補充する
extern	int ConvertProcessSoundConvert_OGG( SOUNDCONV *SoundConv )
{
//	SOUNDCONV_OGG *oggdata = &SoundConv->OggTypeData ;
	SOUNDCONV_OGG *oggdata = (SOUNDCONV_OGG *)SoundConv->ConvFunctionBuffer ;
	int res, BitStream = 0 ;

#ifndef DX_NON_OGGTHEORA
	if( SoundConv->OggVorbisFromTheoraFile )
	{
		res = th_ov_read( &oggdata->File,
						(char *)SoundConv->DestData, (int)SoundConv->DestDataSize,
						0, oggdata->Word, oggdata->Word == 1 ? 0 : 1,
						&BitStream ) ;
	}
	else
#endif
	{
		res = ov_read( &oggdata->File,
						(char *)SoundConv->DestData, (int)SoundConv->DestDataSize,
						0, oggdata->Word, oggdata->Word == 1 ? 0 : 1,
						&BitStream ) ;
	}
	if( res == 0 ) return -1 ;
	SoundConv->DestDataValidSize = res ;

	return 0 ;
}


// 変換処理を位置を変更する( サンプル単位 )
extern	int SetSampleTimeSoundConvert_OGG( SOUNDCONV *SoundConv, LONGLONG SampleTime )
{
//	SOUNDCONV_OGG *oggdata = &SoundConv->OggTypeData ;
	SOUNDCONV_OGG *oggdata = (SOUNDCONV_OGG *)SoundConv->ConvFunctionBuffer ;

#ifndef DX_NON_OGGTHEORA
	if( SoundConv->OggVorbisFromTheoraFile )
	{
		th_ov_pcm_seek( &oggdata->File, SampleTime ) ;
	}
	else
#endif
	{
		ov_pcm_seek( &oggdata->File, SampleTime ) ;
	}

	SoundConv->DestDataValidSize = 0 ;
	SoundConv->DestDataCompSize = 0 ;
	SoundConv->DestDataCompSizeAll = SampleTime * SoundConv->OutFormat.nBlockAlign ;

	return 0 ;
}



// 変換後の大凡のデータサイズを得る
extern	LONGLONG GetSoundConvertDestSize_Fast_OGG( SOUNDCONV *SoundConv )
{
//	SOUNDCONV_OGG *oggdata = &SoundConv->OggTypeData ;
	SOUNDCONV_OGG *oggdata = (SOUNDCONV_OGG *)SoundConv->ConvFunctionBuffer ;

#ifndef DX_NON_OGGTHEORA
	if( SoundConv->OggVorbisFromTheoraFile )
	{
		return th_ov_pcm_total( &oggdata->File, -1 ) * SoundConv->OutFormat.nChannels * oggdata->Word ;
	}
	else
#endif
	{
		return ov_pcm_total( &oggdata->File, -1 ) * SoundConv->OutFormat.nChannels * oggdata->Word ;
	}
}

// ループ情報を取得( [戻] -1:エラー )
extern int GetSoundConvertLoopAreaInfo_OGG(  SOUNDCONV *SoundConv, LONGLONG *LoopStartPos, LONGLONG *LoopEndPos )
{
	SOUNDCONV_OGG *oggdata = (SOUNDCONV_OGG *)SoundConv->ConvFunctionBuffer ;

	// ループ情報がない場合はエラー
	if( oggdata->LoopStart < 0 || oggdata->LoopEnd < 0 )
	{
		return -1 ;
	}

	if( LoopStartPos != NULL ) *LoopStartPos = oggdata->LoopStart ;
	if( LoopEndPos   != NULL ) *LoopEndPos   = oggdata->LoopEnd ;

	// 正常終了
	return 0 ;
}

extern int GetOggCommentNumBase( STREAMDATA *Stream )
{
	DWORD_PTR fp = 0 ;
	int Comments ;
	OggVorbis_File File ;
	ov_callbacks ogfunc ;
	vorbis_comment *comment ;

	// コールバック関数を用意する
	ogfunc.read_func = ( size_t ( * )( void *, size_t, size_t, void * ) )Ogg_StreamRead ;
	ogfunc.seek_func = ( int  ( * )( void *, ogg_int64_t, int ) )Ogg_StreamSeek64_wrap ;
	ogfunc.close_func = ( int ( * )( void * ) )Ogg_StreamClose ;
	ogfunc.tell_func = ( long ( * )( void * ) )Ogg_StreamTell ;

	// Ｏｇｇファイルのセットアップ
	if( ov_open_callbacks( Stream, &File, NULL, 0, ogfunc ) != 0 )
	{
		DXST_LOGFILE_ADDA( "GetOggCommentNum Ogg File Setup Error\n" ) ;
		return -1 ;
	}

	// コメントの数を取得
	comment = ov_comment( &File, -1 ) ;
	Comments = comment->comments ;

	// ファイルを閉じる
	ov_clear( &File ) ;

	// コメントの数を返す
	return Comments ;
}

extern int GetOggCommentBase( STREAMDATA *Stream, int CommentIndex, char *CommentNameBuffer, size_t CommentNameBufferBytes, char *CommentBuffer, size_t CommentBufferBytes )
{
	OggVorbis_File File ;
	ov_callbacks ogfunc ;
	vorbis_comment *comment ;
	int SrcBytes, DestBytes ;
	DWORD UTF8CharCode ;
	DWORD TCHARCharCode ;
	int CharBytes ;

	// コールバック関数を用意する
	ogfunc.read_func = ( size_t ( * )( void *, size_t, size_t, void * ) )Ogg_StreamRead ;
	ogfunc.seek_func = ( int  ( * )( void *, ogg_int64_t, int ) )Ogg_StreamSeek64_wrap ;
	ogfunc.close_func = ( int ( * )( void * ) )Ogg_StreamClose ;
	ogfunc.tell_func = ( long ( * )( void * ) )Ogg_StreamTell ;

	// Ｏｇｇファイルのセットアップ
	if( ov_open_callbacks( Stream, &File, NULL, 0, ogfunc ) != 0 )
	{
		DXST_LOGFILE_ADDA( "GetOggComment Ogg File Setup Error\n" ) ;
		return -1 ;
	}

	// コメントの情報を取得
	comment = ov_comment( &File, -1 ) ;

	// コメントインデックスチェック
	if( CommentIndex < 0 || CommentIndex >= comment->comments )
	{
		ov_clear( &File ) ;
		return -1 ;
	}

	// コメント名をコピー
	SrcBytes = 0 ;
	DestBytes = 0 ;
	for(;;)
	{
		if( SrcBytes >= comment->comment_lengths[ CommentIndex ] || comment->user_comments[ CommentIndex ][ SrcBytes ] == 0 )
		{
			DXST_LOGFILE_ADDA( "GetOggComment Comment Format Error\n" ) ;
			ov_clear( &File ) ;
			return -1 ;
		}

		UTF8CharCode = GetCharCode( &comment->user_comments[ CommentIndex ][ SrcBytes ], DX_CHARCODEFORMAT_UTF8, &CharBytes ) ;
		SrcBytes += CharBytes ;
		if( UTF8CharCode == 0x3d /* '=' */ )
		{
			break ;
		}

		if( DestBytes < ( int )CommentNameBufferBytes )
		{
			TCHARCharCode = ConvCharCode( UTF8CharCode, DX_CHARCODEFORMAT_UTF8, _TCHARCODEFORMAT ) ;
			DestBytes += PutCharCode( TCHARCharCode, _TCHARCODEFORMAT, ( char * )&CommentNameBuffer[ DestBytes ], ( size_t )CommentNameBufferBytes - DestBytes ) ;
		}
	}
	if( DestBytes < ( int )CommentNameBufferBytes )
	{
		DestBytes += PutCharCode( 0, _TCHARCODEFORMAT, ( char * )&CommentNameBuffer[ DestBytes ], ( size_t )CommentNameBufferBytes - DestBytes ) ;
	}

	// コメント文字列をコピー
	DestBytes = 0 ;
	for(;;)
	{
		if( SrcBytes >= comment->comment_lengths[ CommentIndex ] || comment->user_comments[ CommentIndex ][ SrcBytes ] == 0 )
		{
			break ;
		}

		UTF8CharCode = GetCharCode( &comment->user_comments[ CommentIndex ][ SrcBytes ], DX_CHARCODEFORMAT_UTF8, &CharBytes ) ;
		SrcBytes += CharBytes ;

		if( DestBytes < ( int )CommentNameBufferBytes )
		{
			TCHARCharCode = ConvCharCode( UTF8CharCode, DX_CHARCODEFORMAT_UTF8, _TCHARCODEFORMAT ) ;
			DestBytes += PutCharCode( TCHARCharCode, _TCHARCODEFORMAT, ( char * )&CommentBuffer[ DestBytes ], ( size_t )CommentBufferBytes - DestBytes ) ;
		}
	}
	if( DestBytes < ( int )CommentBufferBytes )
	{
		DestBytes += PutCharCode( 0, _TCHARCODEFORMAT, ( char * )&CommentBuffer[ DestBytes ], ( size_t )CommentBufferBytes - DestBytes ) ;
	}

	// ファイルを閉じる
	ov_clear( &File ) ;

	// 正常終了
	return 0 ;
}

#endif // DX_NON_OGGVORBIS












// Opus 関係のプログラム

#ifndef DX_NON_OPUS

// Opus用読み込み関数
static int Opus_StreamRead( void *_stream, unsigned char *_ptr, int _nbytes )
{
	STREAMDATA *StreamP = ( STREAMDATA * )_stream ;
	if( StreamP == NULL ) return -1 ;
	return ( int )StreamP->ReadShred.Read( _ptr, 1, _nbytes, StreamP->DataPoint ) ;
}

// Opus用６４ビット版シーク関数
static int Opus_StreamSeek64_wrap( void *_stream, opus_int64 _offset,int _whence )
{
	STREAMDATA *StreamP = ( STREAMDATA * )_stream ;
	if( StreamP == NULL ) return -1 ;
	return StreamP->ReadShred.Seek( StreamP->DataPoint, _offset, _whence );
}

// Opus用クローズ関数
static int Opus_StreamClose( void * /* _stream */ )
{
	// 普通にクローズが成功したとして返す
	return 0 ;
}

// Opus用のデータ位置取得関数
static opus_int64 Opus_StreamTell( void *_stream )
{
	STREAMDATA *StreamP = ( STREAMDATA * )_stream ;
	if( StreamP == NULL ) return -1 ;
	return StreamP->ReadShred.Tell( StreamP->DataPoint );
}

// 変換後のデータサイズを得る
extern	LONGLONG GetSoundConvertDestSize_OPUS(      SOUNDCONV *SoundConv )
{
	SOUNDCONV_OPUS *opusdata = (SOUNDCONV_OPUS *)SoundConv->ConvFunctionBuffer ;

	return op_pcm_total( opusdata->File, -1 )  * SoundConv->OutFormat.nChannels * opusdata->BlockAlign ;
}

// Opusファイルのセットアップ処理を行う( [戻] -1:エラー )
extern	int SetupSoundConvert_OPUS( SOUNDCONV *SoundConv )
{
	OpusFileCallbacks opfunc ;
    const OpusHead *head;
	SOUNDCONV_OPUS *opusdata = (SOUNDCONV_OPUS *)SoundConv->ConvFunctionBuffer ;
	STREAMDATA *Stream = &SoundConv->Stream ;
	int res, Byte ;
	
	opusdata->FileInitializeFlag = FALSE ;

	// コールバック関数を用意する
	opfunc.read = ( int ( * )( void *, unsigned char *, int ) )Opus_StreamRead ;
	opfunc.seek = ( int  ( * )( void *, opus_int64, int ) )Opus_StreamSeek64_wrap ;
	opfunc.tell = ( opus_int64 ( * )( void * ) )Opus_StreamTell ;
	opfunc.close = ( int ( * )( void * ) )Opus_StreamClose ;

	// Opusファイルのセットアップ
	opusdata->File = op_open_callbacks( Stream, &opfunc, NULL, 0, &res ) ;
	if( opusdata->File == NULL )
	{
		return -1 ;
	}
	opusdata->FileInitializeFlag = TRUE ;

	// ファイルの情報を得る
	head = op_head( opusdata->File, -1 ) ;
	if( head == NULL )
	{
		return -1 ;
	}

	// ＷＡＶＥフォーマットをセットする
	Byte = 2 ;
	SoundConv->OutFormat.cbSize          = 0 ;
	SoundConv->OutFormat.wFormatTag      = WAVE_FORMAT_PCM ;
	SoundConv->OutFormat.nChannels       = ( WORD )head->channel_count ;
	SoundConv->OutFormat.nSamplesPerSec  = ( DWORD )48000 ;
	SoundConv->OutFormat.wBitsPerSample  = ( WORD )( Byte * 8 ) ;
	SoundConv->OutFormat.nBlockAlign     = ( WORD )( Byte * SoundConv->OutFormat.nChannels ) ;
	SoundConv->OutFormat.nAvgBytesPerSec = SoundConv->OutFormat.nSamplesPerSec * SoundConv->OutFormat.nBlockAlign ;
	opusdata->BlockAlign = SoundConv->OutFormat.nBlockAlign ;

	// 変換後のＰＣＭデータを一時的に保存するメモリ領域のサイズをセット
	SoundConv->DestDataSize = 4096 ;

	// 変換後のＰＣＭデータのサイズをセット
	SoundConv->DataSize = GetSoundConvertDestSize_OPUS( SoundConv );

	// タイプセット
	SoundConv->MethodType = SOUND_METHODTYPE_OPUS ;
	
	// 終了
	return 0 ;
}


// Opusファイルの後始末処理を行う
extern	int TerminateSoundConvert_OPUS( SOUNDCONV *SoundConv )
{
	SOUNDCONV_OPUS *opusdata = (SOUNDCONV_OPUS *)SoundConv->ConvFunctionBuffer ;

	if( opusdata->FileInitializeFlag == TRUE )
	{
		op_free( opusdata->File ) ;
		opusdata->FileInitializeFlag = FALSE ;
	}
	
	return 0 ;
}


// 変換後のバッファにデータを補充する
extern	int ConvertProcessSoundConvert_OPUS( SOUNDCONV *SoundConv )
{
	SOUNDCONV_OPUS *opusdata = (SOUNDCONV_OPUS *)SoundConv->ConvFunctionBuffer ;
	int res ;

	res = op_read(
		opusdata->File,
		( opus_int16 * )SoundConv->DestData,
		( int )( SoundConv->DestDataSize / opusdata->BlockAlign ),
		NULL
	) ;
	if( res <= 0 )
	{
		return -1 ;
	}
	SoundConv->DestDataValidSize = ( int )( res * opusdata->BlockAlign ) ;

	return 0 ;
}


// 変換処理を位置を変更する( ミリ秒単位 )
extern	int SetSampleTimeSoundConvert_OPUS( SOUNDCONV *SoundConv, LONGLONG SampleTime )
{
	SOUNDCONV_OPUS *opusdata = (SOUNDCONV_OPUS *)SoundConv->ConvFunctionBuffer ;

	op_pcm_seek( opusdata->File, SampleTime ) ;

	SoundConv->DestDataValidSize = 0 ;
	SoundConv->DestDataCompSize = 0 ;
	SoundConv->DestDataCompSizeAll = ( LONGLONG )( SampleTime * SoundConv->OutFormat.nBlockAlign ) ;

	return 0 ;
}



// 変換後の大凡のデータサイズを得る
extern	LONGLONG GetSoundConvertDestSize_Fast_OPUS( SOUNDCONV *SoundConv )
{
	SOUNDCONV_OPUS *opusdata = (SOUNDCONV_OPUS *)SoundConv->ConvFunctionBuffer ;

	return op_pcm_total( opusdata->File, -1 )  * SoundConv->OutFormat.nChannels * opusdata->BlockAlign ;
}

#endif












#ifndef DX_NON_OGGTHEORA

// ソフト的に一度だけ呼ぶべき初期化関数
extern int	TheoraDecode_GrobalInitialize( void )
{
	// 終了
	return 0 ;
}


// Ogg Theora の読み込み処理の準備を行う( 戻り値  0:失敗  1以上:初期化成功 )
extern DWORD_PTR TheoraDecode_InitializeStream(
	STREAMDATASHRED *	StreamShred,
	DWORD_PTR			StreamData,
	int					StockFrameNum,
	int					NotUseYUVFormatSurface,
	int					NotUseYUVGrHandle,
	int					ASyncThread
)
{
	int MaxPageNum ;
	int MaxDataFrameNum ;
	int MaxTimeFrameNum ;
	bool page_renew ;
	DECODE_THEORA *DT = NULL ;
	DWORD NowBytePos, NextBytePos ;
	double FrameTime ;

	// 最初の４バイトを読み込んで OggS じゃ無かったらエラー
	{
		char Test[4];

		StreamShred->Read( Test, 1, 4, StreamData ) ;
		if( Test[ 0 ] != 'O' || Test[ 1 ] != 'g' || Test[ 2 ] != 'g' || Test[ 3 ] != 'S' )
			return 0 ;
		StreamShred->Seek( StreamData, 0L, STREAM_SEEKTYPE_SET ) ;
	}

	// 新しいムービーデコード処理用のメモリ領域の確保
	DT = ( DECODE_THEORA * )DXALLOC( sizeof( DECODE_THEORA ) + sizeof( DECODE_THEORA_PF ) ) ;
	if( DT == NULL )
	{
		DXST_LOGFILE_ADDA( "Theora \x83\x80\x81\x5b\x83\x72\x81\x5b\x83\x66\x83\x52\x81\x5b\x83\x68\x8f\x88\x97\x9d\x97\x70\x82\xcc\x83\x81\x83\x82\x83\x8a\x8a\x6d\x95\xdb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora ムービーデコード処理用のメモリ確保に失敗しました\n" @*/ );
		return 0 ;
	}

	// 構造体の初期化
	_MEMSET( DT, 0, sizeof( DECODE_THEORA ) + sizeof( DECODE_THEORA_PF ) ) ;

	// 環境依存データのアドレスをセット
	DT->PF = ( DECODE_THEORA_PF * )( DT + 1 ) ;

	// クリティカルセクションも初期化する
	CriticalSection_Initialize( &DT->CriticalSection ) ;

	DT->NumOfTheoraHeader = 0 ;
	DT->TheoraCurrentDataFrame = -1 ;
	DT->TheoraCurrentTimeFrame = -1 ;

	// ＹＵＶフォーマットのサーフェスを使用するかどうかをセットする
	DT->NotUseYUVFormatSurface = NotUseYUVFormatSurface ;

	// YUVカラーのグラフィックハンドルを使用しないかどうかをセットする
	DT->NotUseYUVGrHandle = NotUseYUVGrHandle ;

	// Ogg ライブラリで使用する構造体の初期化
	ogg_sync_init(       &DT->OggSyncState ) ;
	theora_info_init(    &DT->TheoraInfo ) ;
	theora_comment_init( &DT->TheoraComment ) ;

	// 使用するストリームの情報を複製
	DT->StreamShred = *StreamShred ;
	DT->StreamData = StreamData ;

	// ヘッダの読み込み
	if( TheoraDecode_ReadHeader( DT ) == -1 )
	{
		goto ERR ;
	}

	// クリティカルセクションのロックを取得
	CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

	// Theora データデコードの準備
	theora_decode_init( &DT->TheoraState, &DT->TheoraInfo ) ;

	// シーク用にすべての Theora ビデオページを洗い出す
	{
		int TimeFrame ;

		FrameTime = 1.0 / ( ( double )DT->TheoraInfo.fps_numerator / DT->TheoraInfo.fps_denominator ) ;

		// 情報をリセット
		ogg_sync_reset( &DT->OggSyncState ) ;
		ogg_stream_reset( &DT->OggTheoraStream ) ;

		// ファイルの先頭に移動
		DT->StreamShred.Seek( DT->StreamData, 0, STREAM_SEEKTYPE_SET ) ;

		// ページ情報格納用のメモリを確保
		MaxPageNum = 1000 ;
		DT->TheoraPageInfo = ( THEORA_PAGEINFO * )DXALLOC( sizeof( THEORA_PAGEINFO ) * MaxPageNum ) ;
		if( DT->TheoraPageInfo == NULL )
		{
			DXST_LOGFILE_ADDA( "Theora \x83\x79\x81\x5b\x83\x57\x8f\xee\x95\xf1\x8a\x69\x94\x5b\x97\x70\x83\x81\x83\x82\x83\x8a\x82\xcc\x8a\x6d\x95\xdb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora ページ情報格納用メモリの確保に失敗しました\n" @*/ );
			goto ERR ;
		}
		DT->TheoraPageInfoNum = 0 ;

		// キーフレーム情報格納用のメモリを確保
		MaxDataFrameNum = 1000 ;
		DT->TheoraKeyFrameInfo = ( unsigned char * )DXALLOC( sizeof( unsigned char ) * MaxDataFrameNum ) ;
		if( DT->TheoraKeyFrameInfo == NULL )
		{
			DXST_LOGFILE_ADDA( "Theora \x83\x4c\x81\x5b\x83\x74\x83\x8c\x81\x5b\x83\x80\x8f\xee\x95\xf1\x8a\x69\x94\x5b\x97\x70\x83\x81\x83\x82\x83\x8a\x82\xcc\x8a\x6d\x95\xdb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora キーフレーム情報格納用メモリの確保に失敗しました\n" @*/ );
			goto ERR ;
		}

		// 再生時間フレームとデータフレームの対応を格納するためのメモリを確保
		MaxTimeFrameNum = 1000 ;
		DT->TheoraTimeFrameToDataFrame = ( int * )DXALLOC( sizeof( int ) * MaxTimeFrameNum ) ;
		if( DT->TheoraTimeFrameToDataFrame == NULL )
		{
			DXST_LOGFILE_ADDA( "Theora \x83\x5e\x83\x43\x83\x80\x83\x74\x83\x8c\x81\x5b\x83\x80\x82\xc6\x83\x66\x81\x5b\x83\x5e\x83\x74\x83\x8c\x81\x5b\x83\x80\x91\xce\x89\x9e\x8f\xee\x95\xf1\x8a\x69\x94\x5b\x97\x70\x83\x81\x83\x82\x83\x8a\x82\xcc\x8a\x6d\x95\xdb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora タイムフレームとデータフレーム対応情報格納用メモリの確保に失敗しました\n" @*/ );
			goto ERR ;
		}

		// 総フレーム数をリセット
		DT->TheoraTotalDataFrame = 0 ;
		DT->TheoraTotalTimeFrame = 0 ;

		/// ページ情報を総洗い
		NowBytePos = 0 ;
		NextBytePos = 0 ;
		TimeFrame = 0 ;
		page_renew = false ;
		for(;;)
		{
			do
			{
				// Theora のパケットが取得できるまでループ
				while( ogg_stream_packetout( &DT->OggTheoraStream, &DT->OggPacket ) != 1 )
				{
					// ページが取得できるかどうかで処理を分岐
					if( ogg_sync_pageout( &DT->OggSyncState, &DT->OggPage ) > 0 )
					{
						// ページが更新された証を残す
						page_renew = true ;

						// 取得できた場合はページが変更された印を残し、バイト単位のアドレスも更新する
						NowBytePos = NextBytePos ;
						NextBytePos += DT->OggPage.body_len + DT->OggPage.header_len ;

						// ストリームにページを代入する
						ogg_stream_pagein( &DT->OggTheoraStream, &DT->OggPage ) ;
					}
					else
					{
						// ページが取得できなかった場合はファイルから新たにデータを読み込む
						if( TheoraDecode_ReadData( DT ) == 0 )
						{
							goto SEARCH_END;
						}
					}
				}
				// Theora のビデオデータではなかった場合はループ
			}while( theora_packet_iskeyframe( &DT->OggPacket ) < 0 ) ;

			// タイムフレームとデータフレームのずれを補正
			if( DT->OggPacket.granulepos >= 0 )
			{
				TimeFrame = _DTOL( theora_granule_time( &DT->TheoraState, DT->OggPacket.granulepos ) / FrameTime ) ;
			}

			// ページが更新されているかどうかで処理を分岐
			if( page_renew )
			{
				// 配列がいっぱいになっていた場合は拡張
				if( DT->TheoraPageInfoNum == MaxPageNum )
				{
					MaxPageNum += 1000 ;
					DT->TheoraPageInfo = ( THEORA_PAGEINFO * )DXREALLOC( DT->TheoraPageInfo, sizeof( THEORA_PAGEINFO ) * MaxPageNum ) ;
				}

				// フレームアドレスとストリームのバイトアドレスをセット
				DT->TheoraPageInfo[ DT->TheoraPageInfoNum ].FrameAddres = ( int )DT->TheoraTotalDataFrame ;
				DT->TheoraPageInfo[ DT->TheoraPageInfoNum ].StreamAddres = ( int )NowBytePos ;

				// キーフレームがあるかどうかとフレーム数はこれからカウントする
				DT->TheoraPageInfo[ DT->TheoraPageInfoNum ].FrameNum = 0 ;
				DT->TheoraPageInfo[ DT->TheoraPageInfoNum ].KeyFrame = 0 ;

				// Theora のビデオデータページの数をインクリメント
				DT->TheoraPageInfoNum ++ ;

				// ページが更新されたかフラグを倒す
				page_renew = false ;
			}

			// キーフレームかどうかの情報を格納するメモリ領域が足りなくなったら拡張
			if( DT->TheoraTotalDataFrame == MaxDataFrameNum )
			{
				MaxDataFrameNum += 1000 ;
				DT->TheoraKeyFrameInfo = ( unsigned char * )DXREALLOC( DT->TheoraKeyFrameInfo, sizeof( unsigned char ) * MaxDataFrameNum ) ;
			}

			// キーフレームだったらフラグを立てる
			if( theora_packet_iskeyframe( &DT->OggPacket ) == 1 )
			{
				DT->TheoraKeyFrameInfo[ DT->TheoraTotalDataFrame ] = 1 ;
			}
			else
			{
				DT->TheoraKeyFrameInfo[ DT->TheoraTotalDataFrame ] = 0 ;
			}

			while( TimeFrame >= DT->TheoraTotalTimeFrame )
			{
				if( DT->TheoraTotalTimeFrame == MaxTimeFrameNum )
				{
					MaxTimeFrameNum += 1000 ;
					DT->TheoraTimeFrameToDataFrame = ( int * )DXREALLOC( DT->TheoraTimeFrameToDataFrame, sizeof( int ) * MaxTimeFrameNum ) ;
				}
				DT->TheoraTimeFrameToDataFrame[ DT->TheoraTotalTimeFrame ] = ( int )DT->TheoraTotalDataFrame ;
				DT->TheoraTotalTimeFrame ++ ;
			}

			// フレーム数をインクリメント
			TimeFrame ++ ;
			DT->TheoraTotalDataFrame ++ ;
			DT->TheoraPageInfo[ DT->TheoraPageInfoNum - 1 ].FrameNum ++ ;
		}
	}
SEARCH_END:

	// 最初のページはストリームの先頭にする
	DT->TheoraPageInfo[ 0 ].StreamAddres = 0 ;

	// クリティカルセクションのロックを解放
	CriticalSection_Unlock( &DT->CriticalSection ) ;

	// DirectDrawObject が有効な場合は YUV フォーマットの一時保存用サーフェスを作成する
	TheoraDecode_CreateSurface( DT, ASyncThread ) ;

	// クリティカルセクションのロックを取得
	CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

	// 通常参照用にシステムメモリに XRGB 形式のバッファを作成する
//	DXST_LOGFILEFMT_ADDA(( "Theora w:%d  h:%d", DT->TheoraInfo.width, DT->TheoraInfo.height ));
	if( CreateXRGB8ColorBaseImage( ( int )DT->TheoraInfo.width, ( int )DT->TheoraInfo.height, ( BASEIMAGE * )&DT->BaseImage ) == -1 )
	{
		DXST_LOGFILE_ADDA( "Theora \x83\x80\x81\x5b\x83\x72\x81\x5b\x83\x66\x83\x52\x81\x5b\x83\x68\x83\x74\x83\x8c\x81\x5b\x83\x80\x83\x58\x83\x67\x83\x62\x83\x4e\x97\x70\x89\xe6\x91\x9c\x83\x6f\x83\x62\x83\x74\x83\x40\x82\xcc\x8a\x6d\x95\xdb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora ムービーデコードフレームストック用画像バッファの確保に失敗しました\n" @*/ );
		goto ERR ;
	}

	// グラフィックハンドルは作成されていない状態にセット
	DT->YGrHandle  = -1 ;
	DT->UVGrHandle = -1 ;

	// ストックフレーム用バッファの作成
	{
		DT->StockFrameMaxNum = StockFrameNum ;
		DT->StockFrame = ( THEORA_STOCKFRAME * )DXALLOC( sizeof( THEORA_STOCKFRAME ) * StockFrameNum ) ;
		if( DT->StockFrame == NULL )
		{
			DXST_LOGFILE_ADDA( "Theora \x83\x80\x81\x5b\x83\x72\x81\x5b\x83\x66\x83\x52\x81\x5b\x83\x68\x83\x74\x83\x8c\x81\x5b\x83\x80\x83\x58\x83\x67\x83\x62\x83\x4e\x97\x70\x82\xcc\x83\x81\x83\x82\x83\x8a\x8a\x6d\x95\xdb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora ムービーデコードフレームストック用のメモリ確保に失敗しました\n" @*/ );
			goto ERR ;
		}

		_MEMSET( ( void * )DT->StockFrame, 0, sizeof( THEORA_STOCKFRAME ) * StockFrameNum ) ;
	}

	// デコード処理スレッドの作成
	if( TheoraDecode_InitializeStream_PF( DT ) < 0 )
	{
		goto ERR ;
	}

	// クリティカルセクションのロックを解放
	CriticalSection_Unlock( &DT->CriticalSection ) ;

	// 先頭をシーク
//	TheoraDecode_SeekToFrame( ( DWORD_PTR )DT, 491 ) ;
//	TheoraDecode_SeekToFrame( ( DWORD_PTR )DT, 82 ) ;
//	TheoraDecode_SeekToFrame( ( DWORD_PTR )DT, 83 ) ;
	TheoraDecode_SeekToFrame( ( DWORD_PTR )DT, 0 ) ;

	// クリティカルセクションのロックを取得
	CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

	// 先読みを許可
	DT->ThreadEnableCacheRead = 1 ;

	// クリティカルセクションのロックを解放
	CriticalSection_Unlock( &DT->CriticalSection ) ;

	// 最初のフレームの画像はセットアップしておく
	TheoraDecode_SetupImage( ( DWORD_PTR )DT, 1, 1, 1, ASyncThread ) ;

	// アドレスを返す
	return ( DWORD_PTR )DT ;

ERR:
	// クリティカルセクションのロックを解放
	CriticalSection_Unlock( &DT->CriticalSection ) ;

	TheoraDecode_Terminate( ( DWORD_PTR )DT ) ;
	return 0 ;
}

// Ogg Theora 読み込み処理の後始末を行う
int	TheoraDecode_Terminate( DWORD_PTR Handle )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;
	volatile THEORA_STOCKFRAME *Stock ;
	int i ;
	bool Result ;

//	DXST_LOGFILEFMT_ADDW(( L"TheoraDecode_Terminate 00 0x%08x", DT )) ;

//	if( DT->DecodeThreadHandle )
	if( Thread_IsValid( &DT->DecodeThreadInfo ) )
	{
		// クリティカルセクションのロックを取得
		CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

		DT->ThreadExitRequest = 1 ;

		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;

		for(;;)
		{
			// クリティカルセクションのロックを取得
			CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

			Result = DT->ThreadState == THEORAT_STATE_EXIT ;

			// クリティカルセクションのロックを解放
			CriticalSection_Unlock( &DT->CriticalSection ) ;

			if( Result )
			{
				break ;
			}

			Thread_Sleep( 1 ) ;
		}

		// スレッドのハンドルを閉じる
//		WinAPIData.Win32Func.CloseHandleFunc( DT->DecodeThreadHandle ) ;
//		DT->DecodeThreadHandle = NULL ;
		Thread_Delete( &DT->DecodeThreadInfo ) ;
	}

	if( DT->StockFrame )
	{
		Stock = DT->StockFrame ;
		for( i = 0 ; i < DT->StockFrameMaxNum ; i ++, Stock ++ )
		{
			if( Stock->YBuffer )
			{
				DXFREE( Stock->YBuffer ) ;
				Stock->YBuffer = NULL ;
			}

			if( Stock->UVBuffer )
			{
				DXFREE( Stock->UVBuffer ) ;
				Stock->UVBuffer = NULL ;
			}
		}
		DXFREE( ( void * )DT->StockFrame ) ;
		DT->StockFrame = NULL ;
	}

	TheoraDecode_ReleaseSurface( DT, FALSE ) ;

	if( DT->TheoraPageInfo )
	{
		DXFREE( DT->TheoraPageInfo ) ;
		DT->TheoraPageInfo = NULL ;
	}

	if( DT->TheoraKeyFrameInfo )
	{
		DXFREE( DT->TheoraKeyFrameInfo ) ;
		DT->TheoraKeyFrameInfo = NULL ;
	}
	if( DT->TheoraTimeFrameToDataFrame )
	{
		DXFREE( DT->TheoraTimeFrameToDataFrame ) ;
		DT->TheoraTimeFrameToDataFrame = NULL ;
	}

	ReleaseBaseImage( ( BASEIMAGE * )&DT->BaseImage ) ;

	if( DT->YGrHandle != -1 )
	{
		SubHandle( DT->YGrHandle, FALSE, FALSE ) ;
		DT->YGrHandle = -1 ;
	}

	if( DT->UVGrHandle != -1 )
	{
		SubHandle( DT->UVGrHandle, FALSE, FALSE ) ;
		DT->UVGrHandle = -1 ;
	}

	ogg_stream_clear(     &DT->OggTheoraStream ) ;
	theora_clear(         &DT->TheoraState ) ;
	theora_comment_clear( &DT->TheoraComment ) ;
	theora_info_clear(    &DT->TheoraInfo ) ;

	ogg_sync_clear(       &DT->OggSyncState ) ;

	CriticalSection_Delete( &DT->CriticalSection ) ;

	DXFREE( DT ) ;

	// 終了
	return 0 ;
}

// Ogg Theora で使用しているサーフェスを解放する
extern int TheoraDecode_SurfaceTerminate( DWORD_PTR Handle )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;

	// サーフェス解放依頼フラグを立てる
	DT->YUVSurfaceReleaseRequest = TRUE ;

	// 終了
	return 0 ;
}

// Ogg にデータを読み込んでセットする
int TheoraDecode_ReadData( DECODE_THEORA *DT, int Bytes )
{
	char *buffer ;
	size_t bytes ;

	buffer = ogg_sync_buffer( &DT->OggSyncState, Bytes ) ;
	bytes  = DT->StreamShred.Read( buffer, 1, ( size_t )Bytes, DT->StreamData ) ;

	ogg_sync_wrote( &DT->OggSyncState, ( long )bytes ) ;

	return ( int )bytes ;
}

// Theora のヘッダを読み込む
int TheoraDecode_ReadHeader( DECODE_THEORA *DT )
{
	ogg_stream_state test;
	bool state = false;
	int ret;

	while( state == false )
	{
		// ファイルからデータの読み込み
		if( TheoraDecode_ReadData( DT ) == 0 )
			break ;

		// ページの取得
		while( ogg_sync_pageout( &DT->OggSyncState, &DT->OggPage ) == 1 )
		{
			// ヘッダーかどうか確認する
			if( ogg_page_bos( &DT->OggPage ) == 0 )
			{
				ogg_stream_pagein( &DT->OggTheoraStream, &DT->OggPage ) ;
				state = true ;
				break ;
			}

			// ストリームの初期化
			ogg_stream_init( &test, ogg_page_serialno( &DT->OggPage ) ) ;
			ogg_stream_pagein( &test, &DT->OggPage ) ;
			ogg_stream_packetout( &test, &DT->OggPacket ) ;

			// Theora のヘッダパケットかどうかを調べる
			if( theora_decode_header( &DT->TheoraInfo, &DT->TheoraComment, &DT->OggPacket ) != 0 )
			{
				// 他のストリームは気にしない
				ogg_stream_clear( &test );
				continue ;
			}

			// Theora発見
			DT->TheoraSerialNo = ogg_page_serialno( &DT->OggPage ) ;
			memcpy( &DT->OggTheoraStream, &test, sizeof( test ) ) ;
			DT->NumOfTheoraHeader = 1 ;

			state = true ;
			break ;
		}
	}

	// まだヘッダーパケットある？
	while( DT->NumOfTheoraHeader < 3 )
	{
		// Theoraのヘッダーを探そう
		while( DT->NumOfTheoraHeader < 3 && ( ret = ogg_stream_packetout( &DT->OggTheoraStream, &DT->OggPacket ) ) != 0 )
		{
			if( ret < 0 )
			{
//				throw "Error parsing Theora stream headers; 不正なストリーム？";
				return -1 ;
			}

			if( theora_decode_header( &DT->TheoraInfo, &DT->TheoraComment, &DT->OggPacket ) )
			{
//				throw "Error parsing Theora stream headers; 不正なストリーム？";
				return -1 ;
			}

			DT->NumOfTheoraHeader++;
			if( DT->NumOfTheoraHeader == 3 )
				break;
		}

		// 他のストリームのヘッダーを読み捨て
		if( ogg_sync_pageout( &DT->OggSyncState, &DT->OggPage ) > 0 )
		{
			ogg_stream_pagein( &DT->OggTheoraStream, &DT->OggPage ) ;
		}
		else
		{
			if( TheoraDecode_ReadData( DT ) == 0 )
			{
				return -1 ;
//				throw "End of file while searching for codec headers.";
			}
		}
	}

	return DT->NumOfTheoraHeader != 0 ? 1 : -1 ;
}

// デコードスレッドがアイドル状態かどうかを調べる( 戻り値  1:アイドル状態  0:稼動中 )
int TheoraDecode_IsIdle( DWORD_PTR Handle )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;
	int Result ;

	// クリティカルセクションのロックを取得
	CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

	Result = DT->ThreadState == THEORAT_STATE_IDLE ? 1 : 0 ;

	// クリティカルセクションのロックを解放
	CriticalSection_Unlock( &DT->CriticalSection ) ;

	return Result ;
}

// 指定のフレームまで移動する
int TheoraDecode_SeekToFrame( DWORD_PTR Handle, int Frame )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;
	bool Result ;
	int DataFrame ;

	// クリティカルセクションのロックを取得
	CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

	// データフレームを取得
	if( Frame < 0 )
	{
		Frame = 0 ;
	}
	if( Frame >= DT->TheoraTotalTimeFrame )
	{
		Frame = DT->TheoraTotalTimeFrame - 1 ;
	}
	DataFrame = DT->TheoraTimeFrameToDataFrame[ Frame ] ;
//	if( DataFrame < 0 ) DataFrame = 0 ;
//	if( DataFrame > DT->TheoraTotalDataFrame ) DataFrame = ( int )DT->TheoraTotalDataFrame ;

	// もしカレントフレームと同じフレームだった場合は何もせず終了
	if( DataFrame == DT->TheoraCurrentDataFrame )
	{
		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;

		return 0 ;
	}

	// イメージがセットアップされている、フラグを倒す
	DT->BaseImageSetup = 0 ;
	DT->YUVImageSetup = 0 ;
	DT->YUVGrHandleSetup = 0 ;

	// スレッドを止める
	DT->ThreadStopRequest = 1 ;

	// クリティカルセクションのロックを解放
	CriticalSection_Unlock( &DT->CriticalSection ) ;

	for(;;)
	{
		// クリティカルセクションのロックを取得
		CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

		Result = DT->ThreadState == THEORAT_STATE_STOP ;

		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;

		if( Result )
		{
			break ;
		}

		Thread_Sleep( 0 ) ;
	}

	// クリティカルセクションのロックを取得
	CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

	// 現在のフレームをセット
	DT->TheoraCurrentDataFrame = DataFrame ;
	DT->TheoraCurrentTimeFrame = Frame ;

	// シーク先をセット
	DT->ThreadSeekFrame = DataFrame ;

	// シークリクエスト
	DT->ThreadSeekRequest = 1 ;

	// スレッドを動かす
	DT->ThreadStopRequest = 0 ;

	// クリティカルセクションのロックを解放
	CriticalSection_Unlock( &DT->CriticalSection ) ;

	// 終了
	return 0 ;
}


// カレントフレームを指定の再生時間に移動する( 単位はマイクロ秒 )
int TheoraDecode_SeekToTime( DWORD_PTR Handle, LONGLONG Time )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;

	return TheoraDecode_SeekToFrame( ( DWORD_PTR )DT, ( int )( Time / ( 1000000.0 / ( ( double )DT->TheoraInfo.fps_numerator / DT->TheoraInfo.fps_denominator ) ) ) ) ;
}

// カレントフレームを指定フレーム分進める
int TheoraDecode_IncToFrame( DWORD_PTR Handle, int AddNum )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;

	if( AddNum <= 0 ) return 0 ;

	// クリティカルセクションのロックを取得
	CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

	// カレントフレームを変更
	DT->TheoraCurrentTimeFrame += AddNum ;
	if( DT->TheoraCurrentTimeFrame >= DT->TheoraTotalTimeFrame )
	{
		DT->TheoraCurrentTimeFrame -= DT->TheoraTotalTimeFrame ;
	}
	DT->TheoraCurrentDataFrame = DT->TheoraTimeFrameToDataFrame[ DT->TheoraCurrentTimeFrame ] ;

#ifdef __DEBUG__
	_MEMMOVE( &___StockFrame[ 12 ], &___StockFrame[ 10 ], sizeof( int ) * 20 ) ;
	___StockFrame[ 10 ] = DT->TheoraCurrentDataFrame ;
	___StockFrame[ 11 ] = AddNum ;
#endif

	// イメージがセットアップされている、フラグを倒す
	DT->BaseImageSetup = 0 ;
	DT->YUVImageSetup = 0 ;
	DT->YUVGrHandleSetup = 0 ;

	// クリティカルセクションのロックを解放
	CriticalSection_Unlock( &DT->CriticalSection ) ;

	// 終了
	return 0 ;
}

#ifdef __DEBUG__
LONGLONG ___time3;
#endif

// カレントフレームを指定フレーム分進める( 戻り値  1:キーフレーム  0:キーフレームじゃない  -1:ストリームの終端 )
int _TheoraDecode_IncToFrame( DWORD_PTR Handle, int AddNum )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;
	int i, LastKeyFrame, LastKeyFrame2 ;
#ifdef __DEBUG__
	LONGLONG time = GetNowHiPerformanceCount( FALSE ) ;
#endif

	// 指定のフレームをインクリメントするにあたり最後に現れるキーフレームを検出する
	LastKeyFrame  = -1 ;
	LastKeyFrame2 = -1 ;
	for( i = 0 ; i < AddNum ; i ++ )
	{
		if( DT->TheoraKeyFrameInfo[ DT->TheoraInCurrentDataFrame + i ] == 1 )
		{
			LastKeyFrame2 = LastKeyFrame ;
			LastKeyFrame  = DT->TheoraInCurrentDataFrame + i ;
		}
	}

	// 指定回数だけ繰り返す
	for( i = 0 ; i < AddNum ; i ++ )
	{
		// 有効な Theora ビデオコードを取得
		do
		{
			// ogg packet が取得できるまでループ
			while( ogg_stream_packetout( &DT->OggTheoraStream, &DT->OggPacket ) != 1 )
			{
				// ogg page を取得
				if( ogg_sync_pageout( &DT->OggSyncState, &DT->OggPage ) > 0 )
				{
					// 取得できたら stream にセット
					ogg_stream_pagein( &DT->OggTheoraStream, &DT->OggPage );
				}
				else
				{
					// 取得できなかったらまずファイルの終端に来ているか調べ、来ていたらここで終了
					if( DT->StreamShred.Eof( DT->StreamData ) != 0 )
						return -1 ;

					// 来ていなかったら新たなデータを ogg sync にセット
					TheoraDecode_ReadData( DT ) ;
				}
			}

			// キーフレーム以外で指定のフレームに辿り着くまでにまだキーフレームが存在する場合はデコードをスキップ
			if( LastKeyFrame2 == -1 || LastKeyFrame2 <= DT->TheoraInCurrentDataFrame )
			{
				if( theora_decode_packetin( &DT->TheoraState, &DT->OggPacket ) != 0 ) continue ;
			}

		}while( theora_packet_iskeyframe( &DT->OggPacket ) < 0 ) ;

		if( DT->OggPacket.granulepos >= 0 )
		{
			theora_control( &DT->TheoraState, TH_DECCTL_SET_GRANPOS, &DT->OggPacket.granulepos, sizeof( DT->OggPacket.granulepos ) ) ;
		}

		// カレントフレームをインクリメント
		DT->TheoraInCurrentDataFrame ++ ;
	}

#ifdef __DEBUG__
	___time3 = GetNowHiPerformanceCount( FALSE ) - time;
#endif

	// キーフレームかどうかを返す
	return theora_packet_iskeyframe( &DT->OggPacket ) == 1 ? 1 : 0 ;
}

#ifdef __DEBUG__
LONGLONG ___time;
#endif

#ifndef DX_NON_ASYNCLOAD

static int TheoraDecode_ReleaseSurfaceASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	TheoraDecode_ReleaseSurface(
				( DECODE_THEORA * )Info->Data[ 0 ],
				FALSE ) ;
	return 0 ;
}

#endif // DX_NON_ASYNCLOAD

static void TheoraDecode_ReleaseSurface( DECODE_THEORA *DT, int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = TheoraDecode_ReleaseSurfaceASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )DT ;
		AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
		return ;
	}
#endif // DX_NON_ASYNCLOAD

	TheoraDecode_ReleaseSurface_PF( DT ) ;
}

#ifndef DX_NON_ASYNCLOAD

static int TheoraDecode_CreateSurfaceASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	TheoraDecode_CreateSurface(
				( DECODE_THEORA * )Info->Data[ 0 ],
				FALSE ) ;
	return 0 ;
}

#endif // DX_NON_ASYNCLOAD

extern void TheoraDecode_CreateSurface( DECODE_THEORA *DT, int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = TheoraDecode_CreateSurfaceASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )DT ;
		AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
		return ;
	}
#endif // DX_NON_ASYNCLOAD

	// 既に確保されていたものは解放
	TheoraDecode_ReleaseSurface( DT, FALSE ) ;

	// 環境依存処理
	TheoraDecode_CreateSurface_PF( DT ) ;
}

// Ogg Theora のデコードスレッドで行うループ処理を行う
extern int TheoraDecode_Thread_LoopProcess( DECODE_THEORA *DT )
{
	int i, j, k ;

	// クリティカルセクションのロックを取得
	CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

	// 終了リクエストがあったらスレッドを終了
	if( DT->ThreadExitRequest == 1 )
	{
		DT->ThreadState = THEORAT_STATE_EXIT ;

		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;
		return 2 ;
	}
	else
	// ストップリクエストがあったらスレッドをストップ
	if( DT->ThreadStopRequest == 1 )
	{
		DT->ThreadState = THEORAT_STATE_STOP ;

		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;

		Thread_Sleep( 1 ) ;
	}
	else
	// シークリクエストがあったらシーク
	if( DT->ThreadSeekRequest == 1 )
	{
		int now_frame ;

		// 状態をシーク状態にする
		DT->ThreadState = THEORAT_STATE_SEEK ;

		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;

		// 指定のフレームが含まれるページを検索
		for( i = 0 ; i < DT->TheoraPageInfoNum && DT->TheoraPageInfo[ i ].FrameAddres + DT->TheoraPageInfo[ i ].FrameNum <= DT->ThreadSeekFrame ; i ++ ){}
		if( i == DT->TheoraPageInfoNum )
		{
			return (DWORD)-1 ;
		}

		// 指定のフレームが含まれるページ以前のキーフレームがあるページまで遡る
		if( i != 0 ) i -- ;
		while( i > 0 && DT->TheoraPageInfo[ i ].KeyFrame == 0 )
			i -- ;

		// パケットが前のページを跨いでたりするとキーフレームを取り逃すことがあるので更に一つ前のページを・・・
		if( i != 0 ) i -- ;

		// 情報をリセット
		ogg_sync_reset( &DT->OggSyncState ) ;
		ogg_stream_reset( &DT->OggTheoraStream ) ;

		// 検出したページの先頭に移動
		DT->StreamShred.Seek( DT->StreamData, DT->TheoraPageInfo[ i ].StreamAddres, STREAM_SEEKTYPE_SET ) ;

		// 指定のフレームまで移動
		{
			now_frame = DT->TheoraPageInfo[ i ].FrameAddres ;

			// １フレーム目はここで処理( ページ跨ぎのパケットを落としても１フレームとしてカウントする必要があるため )
			if( i != 0 )
			{
				do
				{
					TheoraDecode_ReadData( DT ) ;
				}while( ogg_sync_pageout( &DT->OggSyncState, &DT->OggPage ) != 1 ) ;

				// ページ跨ぎのパケットがあったら飛ばされるのでここでその分のフレームをインクリメント
				if( ogg_page_continued( &DT->OggPage ) != 0 )
				{
					now_frame ++ ;
				}

				// ストリームにページをセット
				ogg_stream_pagein( &DT->OggTheoraStream, &DT->OggPage ) ;
			}

			_TheoraDecode_IncToFrame( ( DWORD_PTR )DT, DT->ThreadSeekFrame - now_frame + 1 ) ;
//				while( now_frame <= DT->ThreadSeekFrame )
//				{
//					_TheoraDecode_IncToFrame( ( DWORD_PTR )DT, now_frame != DT->ThreadSeekFrame ? false : true ) ;
//					now_frame ++ ;
//				}
		}

		// クリティカルセクションのロックを取得
		CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

		// フレームスタックリセット
		for( i = 0 ; i < DT->StockFrameMaxNum ; i ++ )
		{
			DT->StockFrame[ i ].UseFlag = 0 ;
		}

		// カレントフレームをセット
		DT->TheoraInCurrentDataFrame = DT->ThreadSeekFrame ;

		// シークリクエスト終了
		DT->ThreadSeekRequest = 0 ;

		// パケットもう無いフラグを倒す
		DT->ThreadPacketEnd = 0 ;

		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;
	}
	else
	// 何も無かった場合はフレームの先読み
	if( DT->ThreadEnableCacheRead )
	{
		volatile THEORA_STOCKFRAME *Stock ;
		ogg_int64_t StockFrame ;

		// 過ぎてしまったフレームのストックは解放
		if( DT->ThreadPacketEnd == 0 )
		{
			Stock = DT->StockFrame ;
			for( i = 0 ; i < DT->StockFrameMaxNum ; i ++, Stock ++ )
			{
				if( Stock->UseFlag == 0 ) continue ;

				// 自分より後のフレームか、ループを考慮した場合の
				// 先頭付近のフレームでなければ破棄
				StockFrame = DT->TheoraCurrentDataFrame + DT->StockFrameMaxNum ;
				if( Stock->FrameNumber < DT->TheoraCurrentDataFrame )
				{
					// ストック可能なフレームの最大がファイルの総フレーム数を超えているかどうかで分岐
					if( StockFrame >= DT->TheoraTotalDataFrame )
					{
						// 超えている場合はファイル先頭のフレームかどうかをしらべ、そうでなければ破棄
						if( StockFrame - DT->TheoraTotalDataFrame < Stock->FrameNumber )
						{
							Stock->UseFlag = 0 ;
						}
					}
					else
					{
						// 超えていない場合は通り過ぎてしまったフレーム
						Stock->UseFlag = 0 ;
					}
				}
				else
				// 内部フレームよりも後の場合はカレントフレームのバッファ分で無い場合は破棄
				if( Stock->FrameNumber > DT->TheoraInCurrentDataFrame &&
					Stock->FrameNumber > DT->TheoraCurrentDataFrame + DT->StockFrameMaxNum )
				{
					Stock->UseFlag = 0 ;
				}
			}
		}

		// 有効なストックの数を調べる
		Stock = DT->StockFrame ;
		j = 0 ;
		for( i = 0 ; i < DT->StockFrameMaxNum; i ++, Stock++ )
		{
			if( Stock->UseFlag )
			{
#ifdef __DEBUG__
				___StockFrame[ j ] = Stock->FrameNumber ;
#endif
				j ++ ;
			}
		}
#ifdef __DEBUG__
		___StockNum = j ;
		while( j < DT->StockFrameMaxNum )
		{
			___StockFrame[ j ] = -1 ;
			j ++ ;
		}
#endif

		// ただ、空きフレームのストックが何も無い場合は何もしない
		Stock = DT->StockFrame ;
		for( i = 0 ; i < DT->StockFrameMaxNum && Stock->UseFlag; i ++, Stock ++ ){}
		if( DT->StockFrameMaxNum == i )
		{
			// 対外的なカレントデータフレームに対応するストックがなくなった場合は一つストックを無効にする
			Stock = DT->StockFrame ;
			for( i = 0 ; i < DT->StockFrameMaxNum && ( Stock->UseFlag == 0 || Stock->FrameNumber != DT->TheoraCurrentDataFrame ) ; i ++, Stock ++ ){}
			if( DT->StockFrameMaxNum == i )
			{
				Stock = DT->StockFrame ;
				for( i = 0 ; i < DT->StockFrameMaxNum; i ++, Stock ++ )
				{
					if( Stock->UseFlag == 0 ) continue ;

					Stock->UseFlag = 0 ;
				}
			}
			else
			// 状態をアイドリングにする
			if( DT->ThreadState != THEORAT_STATE_IDLE )
			{
				DT->ThreadState = THEORAT_STATE_IDLE ;
				DT->ThreadStandbyTime = NS_GetNowCount( FALSE ) ;
			}
		}
		else
		{
			// 状態をデコードにする
			DT->ThreadState = THEORAT_STATE_DECODE ;

			// 現在の内部フレームが既に展開されているかどうかを調べる
			Stock = DT->StockFrame ;
			for( i = 0 ; i < DT->StockFrameMaxNum && ( Stock->UseFlag == 0 || Stock->FrameNumber != DT->TheoraInCurrentDataFrame ) ; i ++, Stock ++ ){}

			// 現在のフレームが既に展開されているかどうかを調べる
			Stock = DT->StockFrame ;
			for( k = 0 ; k < DT->StockFrameMaxNum && ( Stock->UseFlag == 0 || Stock->FrameNumber != DT->TheoraCurrentDataFrame ) ; k ++, Stock ++ ){}

			// 空きフレームを探す
			Stock = DT->StockFrame ;
			for( j = 0 ; j < DT->StockFrameMaxNum && Stock->UseFlag ; j ++, Stock ++ ){}

			// 空きフレームがあり、且つ現在のフレームが既にストックにあるか、
			// もしくはデコードが遅れていたら次のパケットを読む
			if( j != DT->StockFrameMaxNum && 
				( i != DT->StockFrameMaxNum || DT->TheoraCurrentDataFrame > DT->TheoraInCurrentDataFrame ) )
			{
				int AddFrame ;

#ifdef __DEBUG__
				___StockValidFlag = 1 ;
#endif
				// 有効な Theora ビデオコードを取得
				if( k == DT->StockFrameMaxNum && DT->TheoraCurrentDataFrame > DT->TheoraInCurrentDataFrame )
				{
					AddFrame = DT->TheoraCurrentDataFrame - DT->TheoraInCurrentDataFrame ;
					if( AddFrame < 0 ) AddFrame = 1 ;
				}
				else
				{
					AddFrame = 1 ;
				}

				// 終端フレームに達しているかどうかで処理を分岐
				if( DT->TheoraInCurrentDataFrame + AddFrame >= DT->TheoraTotalDataFrame )
				{
					// 終端フレームに達している場合はファイル先頭に移動する

					// クリティカルセクションのロックを解放
					CriticalSection_Unlock( &DT->CriticalSection ) ;

					// 加算フレームを補正
					AddFrame = ( int )( ( DT->TheoraInCurrentDataFrame + AddFrame ) - DT->TheoraTotalDataFrame ) ;

					// 情報をリセット
					ogg_sync_reset( &DT->OggSyncState ) ;
					ogg_stream_reset( &DT->OggTheoraStream ) ;

					// ページの先頭に移動
					DT->StreamShred.Seek( DT->StreamData, DT->TheoraPageInfo[ 0 ].StreamAddres, STREAM_SEEKTYPE_SET ) ;

					// カレントフレームを先頭に変更
					DT->TheoraInCurrentDataFrame = 0 ;

					// 指定のフレームまで移動
					if( AddFrame != 0 )
					{
						_TheoraDecode_IncToFrame( ( DWORD_PTR )DT, AddFrame ) ;
					}

					// クリティカルセクションのロックを取得
					CRITICALSECTION_LOCK( &DT->CriticalSection ) ;
				}
				else
				{
					int Result ;

					// クリティカルセクションのロックを解放
					CriticalSection_Unlock( &DT->CriticalSection ) ;

					// 終端に達していない場合は普通にフレームを移動
					Result = _TheoraDecode_IncToFrame( ( DWORD_PTR )DT, AddFrame ) ;

					// クリティカルセクションのロックを取得
					CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

					if( Result == -1 )
					{
						DT->ThreadPacketEnd = 1 ;

						// クリティカルセクションのロックを解放
						CriticalSection_Unlock( &DT->CriticalSection ) ;

						Thread_Sleep( 1 ) ;
						return 0 ;
					}
				}
			}
			else
			{
#ifdef __DEBUG__
				___StockValidFlag = 0 ;
#endif
			}

#ifdef __DEBUG__
			___InCurrentFrameNumber = DT->TheoraInCurrentDataFrame ;
#endif

			// ストックに空きがある場合は現在のフレームをストックする
			if( j != DT->StockFrameMaxNum )
			{
				yuv_buffer	yuv ;

				// クリティカルセクションのロックを解放
				CriticalSection_Unlock( &DT->CriticalSection ) ;

				// yuv 情報を出力
				theora_decode_YUVout( &DT->TheoraState, &yuv );

//					if( yuv.y_stride == 842150451 )
//					{
//						DXST_LOGFILEFMT_ADDW(( L"TheoraDecode Thread Error 0x%08x", DT )) ;
//						DxErrorCheckAlloc() ;
//						*((DWORD *)0) = 0xffffffff ;
//					}

				// yuv 情報を保存する

				// バッファのサイズが違う場合は一度解放する
				if( Stock->YStride  != yuv.y_stride  || Stock->YWidth  != yuv.y_width  || Stock->YHeight  != yuv.y_height  )
				{
					if( Stock->YBuffer )
					{
						DXFREE( Stock->YBuffer ) ;
						Stock->YBuffer = NULL ;
					}
				}
				if( Stock->UVStride != yuv.uv_stride * 2 || Stock->UVWidth != yuv.uv_width || Stock->UVHeight != yuv.uv_height )
				{
					if( Stock->UVBuffer )
					{
						DXFREE( Stock->UVBuffer ) ;
						Stock->UVBuffer = NULL ;
					}
				}

				// 情報を保存する
				Stock->YStride = yuv.y_stride ;
				Stock->YWidth  = yuv.y_width ;
				Stock->YHeight = yuv.y_height ;

				Stock->UVStride = yuv.uv_stride * 2 ;
				Stock->UVWidth  = yuv.uv_width ;
				Stock->UVHeight = yuv.uv_height ;

				// バッファがまだ確保されていない場合はバッファを確保する
				if( Stock->YBuffer == NULL )
				{
					Stock->YBuffer = DXALLOC( ( size_t )( Stock->YHeight * Stock->YStride   ) ) ;
					if( Stock->YBuffer == NULL )
					{
						*((DWORD *)0) = 0xffffffff ;
					}
				}
				if( Stock->UVBuffer == NULL )
				{
					Stock->UVBuffer = DXALLOC( ( size_t )( Stock->UVHeight * Stock->UVStride ) ) ;
					if( Stock->UVBuffer == NULL )
					{
						*((DWORD *)0) = 0xffffffff ;
					}
				}

				// バッファにデータをコピーする
				_MEMCPY( Stock->YBuffer, yuv.y, ( size_t )( yuv.y_stride  * yuv.y_height  ) ) ;
				{
					DWORD n, m ;
					DWORD bw ;
					DWORD ow ;
					BYTE *dst ;
					BYTE *u, *v ;

					bw = yuv.uv_width / 8 ;
					ow = yuv.uv_width % 8 ;

					dst = ( BYTE * )Stock->UVBuffer ;
					u = ( BYTE * )yuv.u ;
					v = ( BYTE * )yuv.v ;
					for( n = 0 ; n < ( DWORD )yuv.uv_height ; n ++, dst += Stock->UVStride, u += yuv.uv_stride, v += yuv.uv_stride )
					{
						BYTE *u_t, *v_t ;
						BYTE *dst_t ;

						dst_t = dst ;
						u_t = u ;
						v_t = v ;
						for( m = 0 ; m < bw ; m ++, dst_t += 2 * 8, u_t += 8, v_t += 8 )
						{
							dst_t[  0 ] = u_t[ 0 ] ;		dst_t[  1 ] = v_t[ 0 ] ;
							dst_t[  2 ] = u_t[ 1 ] ;		dst_t[  3 ] = v_t[ 1 ] ;
							dst_t[  4 ] = u_t[ 2 ] ;		dst_t[  5 ] = v_t[ 2 ] ;
							dst_t[  6 ] = u_t[ 3 ] ;		dst_t[  7 ] = v_t[ 3 ] ;
							dst_t[  8 ] = u_t[ 4 ] ;		dst_t[  9 ] = v_t[ 4 ] ;
							dst_t[ 10 ] = u_t[ 5 ] ;		dst_t[ 11 ] = v_t[ 5 ] ;
							dst_t[ 12 ] = u_t[ 6 ] ;		dst_t[ 13 ] = v_t[ 6 ] ;
							dst_t[ 14 ] = u_t[ 7 ] ;		dst_t[ 15 ] = v_t[ 7 ] ;
						}
						for( m = 0 ; m < ow ; m ++, dst_t += 2, u_t ++, v_t ++ )
						{
							dst_t[ 0 ] = *u_t ;
							dst_t[ 1 ] = *v_t ;
						}
					}
				}

				// クリティカルセクションのロックを取得
				CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

				// 展開したフレームの情報を残す
				Stock->UseFlag = 1 ;
				Stock->FrameNumber = DT->TheoraInCurrentDataFrame ;
			}
		}

		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;

		// ちょっと待つ
		if( DT->ThreadState == THEORAT_STATE_IDLE /* && NS_GetNowCount( FALSE ) - DT->ThreadStandbyTime > 200 */ )
		{
			Thread_Sleep( 1 ) ;
		}
		else
		{
			Thread_Sleep( 0 ) ;
		}
	}
	else
	// 先読みが許可されていない場合はただ待つ
	{
		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;

		// ちょっと待つ
		if( DT->ThreadState == THEORAT_STATE_IDLE /* && NS_GetNowCount( FALSE ) - DT->ThreadStandbyTime > 200 */ )
		{
			Thread_Sleep( 1 ) ;
		}
		else
		{
			Thread_Sleep( 0 ) ;
		}
	}

	return 0 ;
}

// カレントフレームのRGB画像を作成する( 戻り値  1:作成された  0:されなかった )
int TheoraDecode_SetupImage( DWORD_PTR Handle, int BaseImage, int YUVGrHandle, int YUVImage, int ASyncThread )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;
	volatile THEORA_STOCKFRAME *Stock ;
	unsigned char *d, *ys, *uvs ;
	int i, j, r, g, b, ysadd, dadd, uvadd, w, h, dpitch, uvr, uvg, uvb, y, y2, y3, y4;
#ifdef __DEBUG__
	LONGLONG time ;
#endif

	// 現在のフレームの画像がセットアップされていれば何もせず終了
	if( ( ( YUVImage    && DT->YUVImageSetup    ) || YUVImage    == 0 || DT->ValidYUVSurface   == FALSE ) &&
		( ( YUVGrHandle && DT->YUVGrHandleSetup ) || YUVGrHandle == 0 || DT->NotUseYUVGrHandle == TRUE  ) &&
		( ( BaseImage   && DT->BaseImageSetup   ) || BaseImage   == 0 ) )
		return 0 ;

	// まだセットアップされていない場合はカレントフレームがデコードされるまで待つ
	for(;;)
	{
		// アイドリング中かデコーディング中以外の場合は待つ
		for(;;)
		{
			// クリティカルセクションのロックを取得
			CRITICALSECTION_LOCK( &DT->CriticalSection ) ;

			if( DT->ThreadState == THEORAT_STATE_IDLE   ||
				DT->ThreadState == THEORAT_STATE_DECODE ||
				DT->ThreadState == THEORAT_STATE_STOP   ||
				DT->ThreadState == THEORAT_STATE_EXIT )
			{
				break ;
			}

			// クリティカルセクションのロックを解放
			CriticalSection_Unlock( &DT->CriticalSection ) ;

			Thread_Sleep( 1 ) ;
		}
		if( DT->ThreadState == THEORAT_STATE_EXIT )
		{
			// クリティカルセクションのロックを解放
			CriticalSection_Unlock( &DT->CriticalSection ) ;

			// スレッドが閉じてしまっている場合は作成されなかった、を返す
			return 0 ;
		}

		// カレントフレームのストックを検索
		Stock = DT->StockFrame ;
		for( i = 0 ; i < DT->StockFrameMaxNum && ( Stock->UseFlag == 0 || Stock->FrameNumber != DT->TheoraCurrentDataFrame ) ; i ++, Stock ++ ){}

		// あったらループを抜ける
		if( i != DT->StockFrameMaxNum )
		{
			// クリティカルセクションのロックを解放
			CriticalSection_Unlock( &DT->CriticalSection ) ;

			break ;
		}

		// パケットの終端に来てしまっていたら作成されなかった、を返す
		if( DT->ThreadPacketEnd == 1 )
		{
			// クリティカルセクションのロックを解放
			CriticalSection_Unlock( &DT->CriticalSection ) ;

			return 0 ;
		}

		// クリティカルセクションのロックを解放
		CriticalSection_Unlock( &DT->CriticalSection ) ;

		// 無かったら待つ
		Thread_Sleep( 0 ) ;
	}

#ifdef __DEBUG__
	time = GetNowHiPerformanceCount( FALSE );
#endif

	// ＹＵＶイメージのセットアップ指定があって、まだ作成されていなかったら作成する
	if( YUVImage && DT->YUVImageSetup == 0 && DT->ValidYUVSurface )
	{
		// YUY2 サーフェスの場合
		TheoraDecode_SetupImage_PF( DT, Stock, ASyncThread ) ;
	}

	// シェーダーが使用できる場合はYUVカラーのグラフィックハンドルを使用する
	if( YUVGrHandle && DT->NotUseYUVGrHandle == FALSE && GetValidShaderVersion() >= 200 )
	{
		SETUP_GRAPHHANDLE_GPARAM GParam ;
		BASEIMAGE BaseImage ;
		RECT SrcRect ;

		// グラフィックハンドルがまだ作成されていなかったら作成する
		if( DT->YGrHandle == -1 )
		{
			Graphics_Image_InitSetupGraphHandleGParam( &GParam ) ;

			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam, 32, FALSE ) ;
			GParam.CreateImageChannelNum      = 1 ;
			GParam.CreateImageChannelBitDepth = 8 ;
			DT->YGrHandle = Graphics_Image_MakeGraph_UseGParam( &GParam, Stock->YWidth, Stock->YHeight, FALSE, FALSE, 0, FALSE, ASyncThread ) ;
			NS_SetDeleteHandleFlag( DT->YGrHandle, ( int * )&DT->YGrHandle ) ;
		}
		if( DT->UVGrHandle == -1 )
		{
			Graphics_Image_InitSetupGraphHandleGParam( &GParam ) ;

			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam, 32, FALSE ) ;
			GParam.CreateImageChannelNum      = 2 ;
			GParam.CreateImageChannelBitDepth = 8 ;
			DT->UVGrHandle     = Graphics_Image_MakeGraph_UseGParam( &GParam, Stock->UVWidth, Stock->UVHeight, FALSE, FALSE, 0, FALSE, ASyncThread ) ;
			NS_SetDeleteHandleFlag( DT->UVGrHandle, ( int * )&DT->UVGrHandle ) ;
		}

		_MEMSET( &BaseImage, 0, sizeof( BaseImage ) ) ;
		BaseImage.Width                     = Stock->YWidth ;
		BaseImage.Height                    = Stock->YHeight ;
		BaseImage.Pitch                     = Stock->YStride ;
		BaseImage.GraphData                 = Stock->YBuffer ;
		BaseImage.ColorData.Format          = DX_BASEIMAGE_FORMAT_NORMAL ;
		BaseImage.ColorData.PixelByte       = 1 ;
		BaseImage.ColorData.ChannelNum      = 1 ;
		BaseImage.ColorData.ChannelBitDepth = 8 ;
		SETRECT( SrcRect, 0, 0, BaseImage.Width, BaseImage.Height ) ;
		Graphics_Image_BltBmpOrBaseImageToGraph3(
			&SrcRect,
			0,
			0,
			DT->YGrHandle,
			&BaseImage,
			NULL,
			FALSE,
			FALSE,
			FALSE,
			ASyncThread
		) ;

		BaseImage.Width                     = Stock->UVWidth ;
		BaseImage.Height                    = Stock->UVHeight ;
		BaseImage.Pitch                     = Stock->UVStride ;
		BaseImage.GraphData                 = Stock->UVBuffer ;
		BaseImage.ColorData.Format          = DX_BASEIMAGE_FORMAT_NORMAL ;
		BaseImage.ColorData.PixelByte       = 2 ;
		BaseImage.ColorData.ChannelNum      = 2 ;
		BaseImage.ColorData.ChannelBitDepth = 8 ;
		SETRECT( SrcRect, 0, 0, BaseImage.Width, BaseImage.Height ) ;
		Graphics_Image_BltBmpOrBaseImageToGraph3(
			&SrcRect,
			0,
			0,
			DT->UVGrHandle,
			&BaseImage,
			NULL,
			FALSE,
			FALSE,
			FALSE,
			ASyncThread
		) ;

		// セットアップフラグを立てる
		DT->YUVGrHandleSetup = 1 ;
	}

	// ＲＧＢイメージのセットアップ指定があって、まだ作成されていなかったら作成する
	if( BaseImage && DT->BaseImageSetup == 0 )
	{
		// BASEIMAGE の場合
		d  = ( unsigned char * )DT->BaseImage.GraphData ;
		ys = ( unsigned char * )Stock->YBuffer ;
		uvs = ( unsigned char * )Stock->UVBuffer ;

		// yuv 情報を rgb データに変換
		if( Stock->YWidth  == Stock->UVWidth  * 2 &&
			Stock->YHeight == Stock->UVHeight * 2 )
		{
			ysadd  = Stock->YStride  * 2 - Stock->YWidth ;
			uvadd  = Stock->UVStride     - Stock->UVWidth * 2 ;
			dadd   = DT->BaseImage.Pitch * 2 - Stock->YWidth * 4 ;
			dpitch = DT->BaseImage.Pitch ;
			w = Stock->YWidth  / 2 ;
			h = Stock->YHeight / 2 ;
			for( i = 0; i < h; i++, d += dadd, ys += ysadd, uvs += uvadd )
			{
				for( j = 0; j < w; j ++, d += 8, ys += 2, uvs += 2 )
				{
					uvr =                                  YUVTable[ YUV_RV ][ uvs[ 1 ] ] ;
					uvg = YUVTable[ YUV_GU ][ uvs[ 0 ] ] + YUVTable[ YUV_GV ][ uvs[ 1 ] ] ;
					uvb = YUVTable[ YUV_BU ][ uvs[ 0 ] ]                              ;

					y  = YUVTable[ YUV_Y ][ ys[ 0 ] ] ;
					y2 = YUVTable[ YUV_Y ][ ys[ 1 ] ] ;
					y3 = YUVTable[ YUV_Y ][ ys[ Stock->YStride ] ] ;
					y4 = YUVTable[ YUV_Y ][ ys[ Stock->YStride + 1 ] ] ;

					d[2]              = YUVLimitTable[ ( ( y  + uvr ) >> 14 ) + 512 ] ;
					d[1]              = YUVLimitTable[ ( ( y  + uvg ) >> 14 ) + 512 ] ;
					d[0]              = YUVLimitTable[ ( ( y  + uvb ) >> 14 ) + 512 ] ;
					d[3]              = 255;

					d[2 + 4]          = YUVLimitTable[ ( ( y2 + uvr ) >> 14 ) + 512 ] ;
					d[1 + 4]          = YUVLimitTable[ ( ( y2 + uvg ) >> 14 ) + 512 ] ;
					d[0 + 4]          = YUVLimitTable[ ( ( y2 + uvb ) >> 14 ) + 512 ] ;
					d[3 + 4]          = 255;

					d[2 + dpitch]     = YUVLimitTable[ ( ( y3 + uvr ) >> 14 ) + 512 ] ;
					d[1 + dpitch]     = YUVLimitTable[ ( ( y3 + uvg ) >> 14 ) + 512 ] ;
					d[0 + dpitch]     = YUVLimitTable[ ( ( y3 + uvb ) >> 14 ) + 512 ] ;
					d[3 + dpitch]     = 255;

					d[2 + dpitch + 4] = YUVLimitTable[ ( ( y4 + uvr ) >> 14 ) + 512 ] ;
					d[1 + dpitch + 4] = YUVLimitTable[ ( ( y4 + uvg ) >> 14 ) + 512 ] ;
					d[0 + dpitch + 4] = YUVLimitTable[ ( ( y4 + uvb ) >> 14 ) + 512 ] ;
					d[3 + dpitch + 4] = 255;
				}
			}
		}
		else
		{
			ysadd = Stock->YStride - Stock->YWidth;
			dadd  = DT->BaseImage.Pitch - Stock->YWidth * 4;
			for( i = 0; i < Stock->YHeight; i++, d += dadd, ys += ysadd )
			{
				for( j = 0; j < Stock->YWidth; j ++, d += 4, ys++ )
				{
					uvs = ( BYTE * )Stock->UVBuffer + ( i / 2 ) * Stock->UVStride + j / 2 * 2 ;

					r = (int)(1.164f * (*ys-16)                         + 1.596f * (uvs[1]-128));
					g = (int)(1.164f * (*ys-16) - 0.391f * (uvs[0]-128) - 0.813f * (uvs[1]-128));
					b = (int)(1.164f * (*ys-16) + 2.018f * (uvs[0]-128));
					if( r < 0 ) r = 0; else if( r > 255 ) r = 255;
					if( g < 0 ) g = 0; else if( g > 255 ) g = 255;
					if( b < 0 ) b = 0; else if( b > 255 ) b = 255;
					d[2] = ( unsigned char )r;
					d[1] = ( unsigned char )g;
					d[0] = ( unsigned char )b;
					d[3] = 255;
				}
			}
		}

		// セットアップフラグを立てる
		DT->BaseImageSetup = 1 ;
	}

#ifdef __DEBUG__
	___time = GetNowHiPerformanceCount( FALSE ) - time;
#endif

	// 終了
	return 1 ;
}

// 一時バッファのアドレスを取得する
const BASEIMAGE *TheoraDecode_GetBaseImage( DWORD_PTR Handle )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;

	// BaseImage がセットアップされていなかったらセットアップする
	if( DT->BaseImageSetup == 0 )
	{
		TheoraDecode_SetupImage( ( DWORD_PTR )DT, 1, 0, 0, FALSE ) ;
	}

	// アドレスを返す
	return ( const BASEIMAGE * )&DT->BaseImage ;
}

// 一時バッファの YUV フォーマットのテクスチャを得る
const void * TheoraDecode_GetYUVImage( DWORD_PTR Handle )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;

	return TheoraDecode_GetYUVImage_PF( DT ) ;
}

// YUVカラーの Y成分のグラフィックハンドルを取得する
int TheoraDecode_GetYGrHandle( DWORD_PTR Handle )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;

	return DT->YGrHandle ;
}

// YUVカラーの UV成分のグラフィックハンドルを取得する
int TheoraDecode_GetUVGrHandle( DWORD_PTR Handle )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;

	return DT->UVGrHandle ;
}

// 動画の情報を取得する
int	TheoraDecode_GetInfo( DWORD_PTR Handle, THEORADECODE_INFO *Info )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;

	Info->FrameRate  = ( double )DT->TheoraInfo.fps_numerator / DT->TheoraInfo.fps_denominator ;
	Info->TotalFrame = ( int )DT->TheoraTotalTimeFrame ;
//	Info->Width      = ( int )DT->TheoraInfo.width ;
//	Info->Height     = ( int )DT->TheoraInfo.height ;
	Info->Width      = ( int )DT->TheoraInfo.frame_width ;
	Info->Height     = ( int )DT->TheoraInfo.frame_height ;

	// 終了
	return 0 ;
}

// カレントフレームを取得する
int	TheoraDecode_GetCurrentFrame( DWORD_PTR Handle )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Handle ;

	return DT->TheoraCurrentTimeFrame ;
}

#endif // DX_NON_OGGTHEORA

//}

#endif // !defined( DX_NON_OPUS ) || !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

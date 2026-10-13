// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		アーカイブ制御プログラム
// 
// 				Ver 3.19f
// 
// -------------------------------------------------------------------------------


// ＤＸライブラリ作成時用定義
#define __DX_MAKE

// インクルード-------------------------------------------------------------------
#include "DxArchive_.h"
#include "DxBaseFunc.h"
#include "DxMemory.h"
#include "DxFile.h"
#include "DxFont.h"
#include "DxLog.h"
#include "DxChar.h"
#include "DxThread.h"
#include "DxSystem.h"
#include <stdio.h>


#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

#ifndef DX_NON_DXA

// マクロ定義 --------------------------------------------------------------------

#define DXARCD						DX_ArchiveDirData
//#define CHECKMULTIBYTECHAR(CP)		(( (unsigned char)*(CP) >= 0x81 && (unsigned char)*(CP) <= 0x9F ) || ( (unsigned char)*(CP) >= 0xE0 && (unsigned char)*(CP) <= 0xFC ))	// TRUE:２バイト文字  FALSE:１バイト文字
#define CHARUP(C)					( (C) >= 'a' && (C) <= 'z' ? (C) - 'a' + 'A' : (C))

#define DXARC_ID_AND_VERSION_SIZE	(sizeof( WORD ) * 2)

#define DXARC_HEAD_VER3_SIZE		(24)
#define DXARC_HEAD_VER4_SIZE		(28)
#define DXARC_HEAD_VER6_SIZE		(48)

#define DXARC_FILEHEAD_VER1_SIZE	(40)			// Ver0x0001 の DXARC_FILEHEAD 構造体のサイズ
#define DXARC_FILEHEAD_VER2_SIZE	(44)			// Ver0x0002 の DXARC_FILEHEAD 構造体のサイズ
#define DXARC_FILEHEAD_VER6_SIZE	(64)			// Ver0x0006 の DXARC_FILEHEAD 構造体のサイズ

#ifdef __USE_ULL__
#define NONE_PAL		(0xffffffffffffffffULL)
#else
#define NONE_PAL		(0xffffffffffffffff)
#endif

// 構造体定義 --------------------------------------------------------------------

// DXA_DIR_FindFirst 等の処理で使用する構造体
struct DXA_DIR_FINDDATA
{
	int							UseArchiveFlag;					// アーカイブファイルを使用しているかフラグ
	int							UseArchiveIndex;				// アーカイブを使用している場合、使用しているアーカイブファイルデータのインデックス
	DWORD_PTR					FindHandle;						// ファイル検索用ハンドル
} ;

// DXA_FindFirst 等の処理で使用する構造体
struct DXA_FINDDATA
{
	DXARC						*Container;						// 検索対象のＤＸＡファイル
	BYTE						SearchStr[ FILEPATH_MAX ] ;		// 検索文字列
	union
	{
		DXARC_DIRECTORY			*Directory;						// 検索対象のディレクトリ
		DXARC_DIRECTORY_VER5	*DirectoryV5;					// 検索対象のディレクトリ(Ver5以前用)
	};
	DWORD						ObjectCount;					// 次に渡すディレクトリ内オブジェクトのインデックス
} ;

// 内部大域変数宣言 --------------------------------------------------------------

BYTE Ascii_DotStr[ 2 ]       = { '.',       0 } ;
BYTE Ascii_DoubleDotStr[ 3 ] = { '.',  '.', 0 } ;
BYTE Ascii_EnStr[ 2 ]        = { '\\',      0 } ;
BYTE Ascii_SlashStr[ 2 ]     = { '/',       0 } ;

// アーカイブをディレクトリに見立てる為のデータ
DXARC_DIR DX_ArchiveDirData ;

// 関数プロトタイプ宣言-----------------------------------------------------------

static DXARC_FILEHEAD_VER5 *DXA_GetFileHeaderV5(	DXARC *DXA, const BYTE *FilePath ) ;													// ファイルの情報を得る
static DXARC_FILEHEAD      *DXA_GetFileHeader(		DXARC *DXA, const BYTE *FilePath, DXARC_DIRECTORY **DirectoryP ) ;						// ファイルの情報を得る
static int		DXA_ConvSearchData(					DXARC *DXA, DXARC_SEARCHDATA *Dest, const BYTE *Src, int *Length ) ;					// 文字列を検索用のデータに変換( ヌル文字か \ があったら終了 )
static int		DXA_ChangeCurrentDirectoryFast(		DXARC *DXA, DXARC_SEARCHDATA *SearchData ) ;											// アーカイブ内のディレクトリパスを変更する( 0:成功  -1:失敗 )
static int		DXA_ChangeCurrentDirectoryBase(		DXARC *DXA, const BYTE *DirectoryPath, bool ErrorIsDirectoryReset, DXARC_SEARCHDATA *LastSearchData = NULL ) ;		// アーカイブ内のディレクトリパスを変更する( 0:成功  -1:失敗 )
static size_t	DXA_CreateKeyV2FileString(			DXARC *DXA, DXARC_DIRECTORY *Directory, DXARC_FILEHEAD *FileHead, BYTE *FileString ) ;	// カレントディレクトリにある指定のファイルの鍵バージョン２用の文字列を作成する、戻り値は文字列の長さ( 単位：Byte )( FileString は DXA_KEYV2_STRING_MAXLENGTH の長さが必要 )
//static int	DXA_Decode(							void *Src, void *Dest ) ;																// データを解凍する( 戻り値:解凍後のデータサイズ )
static void		DXA_KeyCreate(						const char *Source, unsigned char *Key ) ;												// 鍵文字列を作成
static void		DXA_KeyV2Create(					const char *Source, unsigned char *Key, size_t KeyBytes = 0 ) ;							// 鍵バージョン２を作成
static void		DXA_KeyConv(						void *Data, LONGLONG  SizeLL, LONGLONG  PositionLL,  unsigned char *Key ) ;				// 鍵文字列を使用して Xor 演算( Key は必ず DXA_KEYSTR_LENGTH の長さがなければならない )
static void		DXA_KeyConvFileRead(				void *Data, ULONGLONG Size,   DWORD_PTR FilePointer, unsigned char *Key, LONGLONG Position = -1 ) ;	// ファイルから読み込んだデータを鍵文字列を使用して Xor 演算する関数( Key は必ず DXA_KEYSTR_LENGTH の長さがなければならない )
static void		DXA_KeyV2Conv(						void *Data, LONGLONG  SizeLL, LONGLONG  PositionLL,  unsigned char *Key ) ;				// 鍵バージョン２文字列を使用して Xor 演算( Key は必ず DXA_KEYV2_LENGTH の長さがなければならない )
static void		DXA_KeyV2ConvFileRead(				void *Data, ULONGLONG Size,   DWORD_PTR FilePointer, unsigned char *Key, LONGLONG Position ) ;		// ファイルから読み込んだデータを鍵文字列を使用して Xor 演算する関数( Key は必ず DXA_KEYV2_LENGTH の長さがなければならない )
static int		DXA_FindProcess(					DXA_FINDDATA *FindData, FILEINFOW *FileInfo );											// 条件に適合するオブジェクトを検索する(検索対象は ObjectCount をインデックスとしたところから)(戻り値 -1:エラー 0:成功)

static int		DXA_DIR_OpenArchive(				const wchar_t *FilePath, void *FileImage = NULL, int FileSize = -1, int FileImageCopyFlag = FALSE, int FileImageReadOnly = FALSE, int ArchiveIndex = -1, int OnMemory = FALSE, int ASyncThread = FALSE ) ;	// アーカイブファイルを開く
static int		DXA_DIR_GetArchive(					const wchar_t *FilePath, void *FileImage = NULL ) ;										// 既に開かれているアーカイブのハンドルを取得する( 戻り値: -1=無かった 0以上:ハンドル )
static int		DXA_DIR_CloseArchive(				int ArchiveHandle ) ;																	// アーカイブファイルを閉じる
static void		DXA_DIR_CloseWaitArchive(			void ) ;																				// 使用されるのを待っているアーカイブファイルを全て閉じる
static int		DXA_DIR_ConvertFullPath(			const wchar_t *Src, wchar_t *Dest, size_t BufferBytes, int CharUp = 1 ) ;				// 全ての英字小文字を大文字にしながら、フルパスに変換する
static int		DXA_DIR_AnalysisFileNameAndDirPath( DXARC *DXA, const BYTE *Src, BYTE *FileName = 0, size_t FileNameBytes = 0, BYTE *DirPath = 0, size_t DirPathBytes = 0 ) ;					// ファイル名も一緒になっていると分かっているパス中からファイル名とディレクトリパスを分割する。フルパスである必要は無い、ファイル名だけでも良い、DirPath の終端に ￥ マークは付かない
static int		DXA_DIR_FileNameCmp(				DXARC *DXA, const BYTE *Src, const BYTE *CmpStr );										// CmpStr の条件に Src が適合するかどうかを調べる( 0:適合する  -1:適合しない )
static int		DXA_DIR_OpenTest(					const wchar_t *FilePath, int *ArchiveIndex, BYTE *ArchiveFilePath, size_t BufferBytes ) ;	// アーカイブファイルをフォルダに見立ててファイルを開く時の情報を得る( -1:アーカイブとしては存在しなかった  0:存在した )

static int		DXA_DirectoryKeyConv(				DXARC *DXA, DXARC_DIRECTORY *Dir, char *KeyV2StringBuffer ) ;							// 指定のディレクトリデータの暗号化を解除する( 丸ごとメモリに読み込んだ場合用 )
static int		DXA_DirectoryKeyConvV5(				DXARC *DXA, DXARC_DIRECTORY_VER5 *Dir ) ;												// 指定のディレクトリデータの暗号化を解除する( 丸ごとメモリに読み込んだ場合用 )



// プログラム --------------------------------------------------------------------

// ファイルの情報を得る
static DXARC_FILEHEAD_VER5 *DXA_GetFileHeaderV5( DXARC *DXA, const BYTE *FilePath )
{
	DXARC_DIRECTORY_VER5 *OldDir ;
	DXARC_FILEHEAD_VER5 *FileH ;
	DWORD FileHeadSize ;
	BYTE *NameData ;
	int i, j, k, Num ;
	DXARC_SEARCHDATA SearchData ;

	// 元のディレクトリを保存しておく
	OldDir = DXA->CurrentDirectoryV5 ;

	// ファイルパスに \ or / が含まれている場合、ディレクトリ変更を行う
	if( CL_strchr( DXA->CharCodeFormat, ( const char * )FilePath, '\\' ) != NULL ||
		CL_strchr( DXA->CharCodeFormat, ( const char * )FilePath, '/'  ) != NULL )
	{
		// カレントディレクトリを目的のファイルがあるディレクトリに変更する
		if( DXA_ChangeCurrentDirectoryBase( DXA, FilePath, false, &SearchData ) >= 0 )
		{
			// エラーが起きなかった場合はファイル名もディレクトリだったことになるのでエラー
			goto ERR ;
		}
	}
	else
	{
		// ファイル名を検索用データに変換する
		DXA_ConvSearchData( DXA, &SearchData, FilePath, NULL ) ;
	}

	// 同名のファイルを探す
	FileHeadSize = ( DWORD )( DXA->HeadV5.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ) ;
	FileH        = ( DXARC_FILEHEAD_VER5 * )( DXA->Table.FileTable + DXA->CurrentDirectoryV5->FileHeadAddress ) ;
	Num          = ( int )DXA->CurrentDirectoryV5->FileHeadNum ;
	for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD_VER5 *)( (BYTE *)FileH + FileHeadSize ) )
	{
		// ディレクトリチェック
		if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
		{
			continue ;
		}

		// 文字列数とパリティチェック
		NameData = DXA->Table.NameTable + FileH->NameAddress ;
		if( SearchData.PackNum != ( ( WORD * )NameData )[ 0 ] ||
			SearchData.Parity  != ( ( WORD * )NameData )[ 1 ] )
		{
			continue ;
		}

		// 文字列チェック
		NameData += 4 ;
		for( j = 0, k = 0 ; j < SearchData.PackNum ; j ++, k += 4 )
		{
			if( *( ( DWORD * )&SearchData.FileName[ k ]) != *( ( DWORD * )&NameData[ k ] ) )
			{
				break ;
			}
		}

		// 適合したファイルがあったらここで終了
		if( SearchData.PackNum == j )
		{
			break ;
		}
	}

	// 無かったらエラー
	if( i == Num )
	{
		goto ERR ;
	}

	// ディレクトリを元に戻す
	DXA->CurrentDirectoryV5 = OldDir ;
	
	// 目的のファイルのアドレスを返す
	return FileH ;
	
ERR :
	// ディレクトリを元に戻す
	DXA->CurrentDirectoryV5 = OldDir ;
	
	// エラー終了
	return NULL ;
}

// ファイルの情報を得る
static DXARC_FILEHEAD *DXA_GetFileHeader( DXARC *DXA, const BYTE *FilePath, DXARC_DIRECTORY **DirectoryP )
{
	DXARC_DIRECTORY *OldDir ;
	DXARC_FILEHEAD *FileH ;
	DWORD FileHeadSize ;
	BYTE *NameData ;
	int i, j, k, Num ;
	DXARC_SEARCHDATA SearchData ;

	// 元のディレクトリを保存しておく
	OldDir = DXA->CurrentDirectory ;

	// ファイルパスに \ or / が含まれている場合、ディレクトリ変更を行う
	if( CL_strchr( DXA->CharCodeFormat, ( const char * )FilePath, '\\' ) != NULL ||
		CL_strchr( DXA->CharCodeFormat, ( const char * )FilePath, '/'  ) != NULL )
	{
		// カレントディレクトリを目的のファイルがあるディレクトリに変更する
		if( DXA_ChangeCurrentDirectoryBase( DXA, FilePath, false, &SearchData ) >= 0 )
		{
			// エラーが起きなかった場合はファイル名もディレクトリだったことになるのでエラー
			goto ERR ;
		}
	}
	else
	{
		// ファイル名を検索用データに変換する
		DXA_ConvSearchData( DXA, &SearchData, FilePath, NULL ) ;
	}

	// 同名のファイルを探す
	FileHeadSize = DXARC_FILEHEAD_VER6_SIZE ;
	FileH        = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + DXA->CurrentDirectory->FileHeadAddress ) ;
	Num          = ( int )DXA->CurrentDirectory->FileHeadNum ;
	for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD *)( (BYTE *)FileH + FileHeadSize ) )
	{
		// ディレクトリチェック
		if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
		{
			continue ;
		}

		// 文字列数とパリティチェック
		NameData = DXA->Table.NameTable + FileH->NameAddress ;
		if( SearchData.PackNum != ( ( WORD * )NameData )[ 0 ] ||
			SearchData.Parity  != ( ( WORD * )NameData )[ 1 ] )
		{
			continue ;
		}

		// 文字列チェック
		NameData += 4 ;
		for( j = 0, k = 0 ; j < SearchData.PackNum ; j ++, k += 4 )
		{
			if( *( ( DWORD * )&SearchData.FileName[ k ] ) != *( ( DWORD * )&NameData[ k ] ) )
			{
				break ;
			}
		}

		// 適合したファイルがあったらここで終了
		if( SearchData.PackNum == j )
		{
			break ;
		}
	}

	// 無かったらエラー
	if( i == Num )
	{
		goto ERR ;
	}

	// ディレクトリを取得する指定があった場合は代入
	if( DirectoryP != NULL )
	{
		*DirectoryP = DXA->CurrentDirectory ;
	}
	
	// ディレクトリを元に戻す
	DXA->CurrentDirectory = OldDir ;
	
	// 目的のファイルのアドレスを返す
	return FileH ;
	
ERR :
	// ディレクトリを元に戻す
	DXA->CurrentDirectory = OldDir ;
	
	// エラー終了
	return NULL ;
}


// 文字列を検索用のデータに変換( ヌル文字か \ があったら終了 )
static int DXA_ConvSearchData( DXARC *DXA, DXARC_SEARCHDATA *Dest, const BYTE *Src, int *Length )
{
	int i, j ;
	int StringLength ;
	WORD  ParityData ;
	DWORD CharCode ;
	int CharBytes ;

	ParityData = 0 ;
	i          = 0 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&Src[ i ], DXA->CharCodeFormat, &CharBytes ) ;
		if( CharCode == '\0' || CharCode == '\\' || CharCode == '/' )
		{
			break ;
		}

		// 小文字の場合は大文字に変換
		if( CharCode >= 'a' && CharCode <= 'z' )
		{
			CharCode = CharCode - 'a' + 'A' ;
			PutCharCode( CharCode, DXA->CharCodeFormat, ( char * )&Dest->FileName[ i ], sizeof( Dest->FileName ) - i ) ;

			switch( CharBytes )
			{
			case 1 :
				ParityData += Dest->FileName[ i ] ;
				break ;

			case 2 :
				ParityData += Dest->FileName[ i ] + Dest->FileName[ i + 1 ] ;
				break ;

			case 4 :
				ParityData += Dest->FileName[ i ] + Dest->FileName[ i + 1 ] + Dest->FileName[ i + 2 ] + Dest->FileName[ i + 3 ] ;
				break ;

			default :
				for( j = 0 ; j < CharBytes ; j ++ )
				{
					ParityData += Dest->FileName[ i + j ] ;
				}
				break ;
			}
		}
		else
		{
			switch( CharBytes )
			{
			case 1 :
				Dest->FileName[ i ] = Src[ i ] ;
				ParityData += Src[ i ] ;
				break ;

			case 2 :
				*( ( WORD * )&Dest->FileName[ i ] ) = *( ( WORD * )&Src[ i ] ) ;
				ParityData += Src[ i ] + Src[ i + 1 ] ;
				break ;

			case 4 :
				*( ( DWORD * )&Dest->FileName[ i ] ) = *( ( DWORD * )&Src[ i ] ) ;
				ParityData += Src[ i ] + Src[ i + 1 ] + Src[ i + 2 ] + Src[ i + 3 ] ;
				break ;

			default :
				for( j = 0 ; j < CharBytes ; j ++ )
				{
					Dest->FileName[ i + j ] = Src[ i + j ] ;
					ParityData += Src[ i + j ] ;
				}
				break ;
			}
		}

		i += CharBytes ;
	}

	// 文字列の長さを保存
	if( Length != NULL )
	{
		*Length = i ;
	}

	// ４の倍数の位置まで０を代入
	StringLength = ( ( i + 1 ) + 3 ) / 4 * 4 ;
	_MEMSET( &Dest->FileName[ i ], 0, ( size_t )( StringLength - i ) ) ;

	// パリティデータの保存
	Dest->Parity = ParityData ;

	// パックデータ数の保存
	Dest->PackNum = ( WORD )( StringLength / 4 ) ;

	// 正常終了
	return 0 ;
}






// 鍵文字列を作成
void DXA_KeyCreate( const char *Source, unsigned char *Key )
{
	size_t Len ;

	if( Source == NULL )
	{
		_MEMSET( Key, 0xaa, DXA_KEYSTR_LENGTH ) ;
	}
	else
	{
		Len = CL_strlen( DX_CHARCODEFORMAT_ASCII, Source ) ;
		if( Len > DXA_KEYSTR_LENGTH )
		{
			_MEMCPY( Key, Source, DXA_KEYSTR_LENGTH ) ;
		}
		else
		{
			// 鍵文字列が DXA_KEYSTR_LENGTH より短かったらループする
			size_t i ;

			for( i = 0 ; i + Len <= DXA_KEYSTR_LENGTH ; i += Len )
			{
				_MEMCPY( Key + i, Source, Len ) ;
			}

			if( i < DXA_KEYSTR_LENGTH )
			{
				_MEMCPY( Key + i, Source, DXA_KEYSTR_LENGTH - i ) ;
			}
		}
	}

	Key[ 0] = ( unsigned char )( ~Key[0] ) ;
	Key[ 1] = ( unsigned char )( ( Key[1] >> 4 ) | ( Key[1] << 4 ) ) ;
	Key[ 2] = ( unsigned char )( Key[2] ^ 0x8a ) ;
	Key[ 3] = ( unsigned char )( ~( ( Key[3] >> 4 ) | ( Key[3] << 4 ) ) ) ;
	Key[ 4] = ( unsigned char )( ~Key[4] ) ;
	Key[ 5] = ( unsigned char )( Key[5] ^ 0xac ) ;
	Key[ 6] = ( unsigned char )( ~Key[6] ) ;
	Key[ 7] = ( unsigned char )( ~( ( Key[7] >> 3 ) | ( Key[7] << 5 ) ) ) ;
	Key[ 8] = ( unsigned char )( ( Key[8] >> 5 ) | ( Key[8] << 3 ) ) ;
	Key[ 9] = ( unsigned char )( Key[9] ^ 0x7f ) ;
	Key[10] = ( unsigned char )( ( ( Key[10] >> 4 ) | ( Key[10] << 4 ) ) ^ 0xd6 ) ;
	Key[11] = ( unsigned char )( Key[11] ^ 0xcc ) ;
}

// 鍵バージョン２を作成
static void DXA_KeyV2Create( const char *Source, unsigned char *Key, size_t KeyBytes )
{
	if( Source == NULL )
	{
		static BYTE DefaultKeyString[ 5 ] = { 0x44, 0x58, 0x41, 0x52, 0x43 }; // "DXARC"
		HashSha256( DefaultKeyString, sizeof( DefaultKeyString ), Key ) ;
	}
	else
	{
		HashSha256( Source, KeyBytes == 0 ? CL_strlen( DX_CHARCODEFORMAT_ASCII, Source ) : KeyBytes, Key ) ;
	}
}

// 鍵文字列を使用して Xor 演算( Key は必ず DXA_KEYSTR_LENGTH の長さがなければならない )
void DXA_KeyConv( void *Data, LONGLONG SizeLL, LONGLONG PositionLL, unsigned char *Key )
{
	int Position ;

	Position = ( int )( PositionLL % DXA_KEYSTR_LENGTH ) ;

	if( SizeLL <= 0xffffffff )
	{
		DWORD SizeT ;
		SizeT = ( DWORD )SizeLL ;

#ifndef DX_NON_INLINE_ASM
		DWORD DataT ;
		DataT = (DWORD)Data ;

		__asm
		{
			MOV EDI, DataT
			MOV ESI, Key

			MOV EAX, SizeT
			CMP EAX, DXA_KEYSTR_LENGTH
			JB LABEL2


			MOV EAX, Position
			CMP EAX, 0
			JE LABEL1


			MOV EDX, SizeT
	LOOP1:
			MOV BL, [ESI+EAX]
			XOR [EDI], BL
			INC EAX
			INC EDI
			DEC EDX
			CMP EAX, DXA_KEYSTR_LENGTH
			JB LOOP1
			XOR ECX, ECX
			MOV Position, ECX

			MOV SizeT, EDX
			CMP EDX, DXA_KEYSTR_LENGTH
			JB LABEL2


	LABEL1:
			MOV EAX, SizeT
			XOR EDX, EDX
			MOV ECX, DXA_KEYSTR_LENGTH
			DIV ECX
			MOV SizeT, EDX
			MOV ECX, EAX

			MOV EAX, [ESI]
			MOV EBX, [ESI+4]
			MOV EDX, [ESI+8]
	LOOP2:
			XOR [EDI],    EAX
			XOR [EDI+4],  EBX
			XOR [EDI+8],  EDX
			ADD EDI, DXA_KEYSTR_LENGTH
			DEC ECX
			JNZ LOOP2


	LABEL2:
			MOV EDX, SizeT
			CMP EDX, 0
			JE LABEL3


			MOV EAX, Position
	LOOP3:
			MOV BL, [ESI+EAX]
			XOR [EDI], BL
			INC EAX
			CMP EAX, DXA_KEYSTR_LENGTH
			JNE LABEL4
			XOR EAX, EAX
	LABEL4:
			INC EDI
			DEC EDX
			JNZ LOOP3
	LABEL3:
		} ;

#else // DX_NON_INLINE_ASM

		BYTE *DataBP ;
		BYTE *KeyBP ;

		DataBP = ( BYTE * )Data ;
		KeyBP = ( BYTE * )Key ;
		if( SizeT >= DXA_KEYSTR_LENGTH )
		{
			if( Position != 0 )
			{
				do
				{
					*DataBP ^= KeyBP[ Position ] ;
					Position ++ ;
					DataBP ++ ;
					SizeT -- ;
				}while( Position < DXA_KEYSTR_LENGTH ) ;
				Position = 0 ;
			}

			if( SizeT >= DXA_KEYSTR_LENGTH )
			{
				DWORD SetNum ;
				DWORD Key1, Key2, Key3 ;

				SetNum = SizeT / DXA_KEYSTR_LENGTH ;
				SizeT -= SetNum * DXA_KEYSTR_LENGTH ;
				Key1 = ( ( DWORD * )KeyBP )[ 0 ] ;
				Key2 = ( ( DWORD * )KeyBP )[ 1 ] ;
				Key3 = ( ( DWORD * )KeyBP )[ 2 ] ;

				do
				{
					( ( DWORD * )DataBP )[ 0 ] ^= Key1 ;
					( ( DWORD * )DataBP )[ 1 ] ^= Key2 ;
					( ( DWORD * )DataBP )[ 2 ] ^= Key3 ;
					DataBP += DXA_KEYSTR_LENGTH ;
					SetNum -- ;
				}while( SetNum > 0 ) ;
			}
		}

		if( SizeT > 0 )
		{
			do
			{
				*DataBP ^= KeyBP[ Position ] ;
				DataBP ++ ;
				Position ++ ;
				if( Position == DXA_KEYSTR_LENGTH )
					Position = 0 ;
				SizeT -- ;
			}while( SizeT > 0 ) ;
		}
#endif // DX_NON_INLINE_ASM
	}
	else
	{
		BYTE *DataBP ;
		BYTE *KeyBP ;

		DataBP = ( BYTE * )Data ;
		KeyBP = ( BYTE * )Key ;
		if( SizeLL >= DXA_KEYSTR_LENGTH )
		{
			if( Position != 0 )
			{
				do
				{
					*DataBP ^= KeyBP[ Position ] ;
					Position ++ ;
					DataBP ++ ;
					SizeLL -- ;
				}while( Position < DXA_KEYSTR_LENGTH ) ;
				Position = 0 ;
			}

			if( SizeLL >= DXA_KEYSTR_LENGTH )
			{
				DWORD SetNum ;
				DWORD Key1, Key2, Key3 ;

				SetNum = ( DWORD )( SizeLL / DXA_KEYSTR_LENGTH ) ;
				SizeLL -= SetNum * DXA_KEYSTR_LENGTH ;
				Key1 = ( ( DWORD * )KeyBP )[ 0 ] ;
				Key2 = ( ( DWORD * )KeyBP )[ 1 ] ;
				Key3 = ( ( DWORD * )KeyBP )[ 2 ] ;

				do
				{
					( ( DWORD * )DataBP )[ 0 ] ^= Key1 ;
					( ( DWORD * )DataBP )[ 1 ] ^= Key2 ;
					( ( DWORD * )DataBP )[ 2 ] ^= Key3 ;
					DataBP += DXA_KEYSTR_LENGTH ;
					SetNum -- ;
				}while( SetNum > 0 ) ;
			}
		}

		if( SizeLL > 0 )
		{
			do
			{
				*DataBP ^= KeyBP[ Position ] ;
				DataBP ++ ;
				Position ++ ;
				if( Position == DXA_KEYSTR_LENGTH )
					Position = 0 ;
				SizeLL -- ;
			}while( SizeLL > 0 ) ;
		}
	}
}

// 鍵バージョン２文字列を使用して Xor 演算( Key は必ず DXA_KEYV2_LENGTH の長さがなければならない )
static void DXA_KeyV2Conv( void *Data, LONGLONG  SizeLL, LONGLONG  PositionLL,  unsigned char *Key )
{
	int Position ;

	Position = ( int )( PositionLL % DXA_KEYV2_LENGTH ) ;

	if( SizeLL <= 0xffffffff )
	{
		DWORD SizeT ;
		SizeT = ( DWORD )SizeLL ;

		BYTE *DataBP ;
		BYTE *KeyBP ;

		DataBP = ( BYTE * )Data ;
		KeyBP = ( BYTE * )Key ;
		if( SizeT >= DXA_KEYV2_LENGTH )
		{
			if( Position != 0 )
			{
				do
				{
					*DataBP ^= KeyBP[ Position ] ;
					Position ++ ;
					DataBP ++ ;
					SizeT -- ;
				}while( Position < DXA_KEYV2_LENGTH ) ;
				Position = 0 ;
			}

			if( SizeT >= DXA_KEYV2_LENGTH )
			{
				DWORD SetNum ;
				DWORD Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8 ;

				SetNum = SizeT / DXA_KEYV2_LENGTH ;
				SizeT -= SetNum * DXA_KEYV2_LENGTH ;
				Key1 = ( ( DWORD * )KeyBP )[ 0 ] ;
				Key2 = ( ( DWORD * )KeyBP )[ 1 ] ;
				Key3 = ( ( DWORD * )KeyBP )[ 2 ] ;
				Key4 = ( ( DWORD * )KeyBP )[ 3 ] ;
				Key5 = ( ( DWORD * )KeyBP )[ 4 ] ;
				Key6 = ( ( DWORD * )KeyBP )[ 5 ] ;
				Key7 = ( ( DWORD * )KeyBP )[ 6 ] ;
				Key8 = ( ( DWORD * )KeyBP )[ 7 ] ;

				do
				{
					( ( DWORD * )DataBP )[ 0 ] ^= Key1 ;
					( ( DWORD * )DataBP )[ 1 ] ^= Key2 ;
					( ( DWORD * )DataBP )[ 2 ] ^= Key3 ;
					( ( DWORD * )DataBP )[ 3 ] ^= Key4 ;
					( ( DWORD * )DataBP )[ 4 ] ^= Key5 ;
					( ( DWORD * )DataBP )[ 5 ] ^= Key6 ;
					( ( DWORD * )DataBP )[ 6 ] ^= Key7 ;
					( ( DWORD * )DataBP )[ 7 ] ^= Key8 ;
					DataBP += DXA_KEYV2_LENGTH ;
					SetNum -- ;
				}while( SetNum > 0 ) ;
			}
		}

		if( SizeT > 0 )
		{
			do
			{
				*DataBP ^= KeyBP[ Position ] ;
				DataBP ++ ;
				Position ++ ;
				if( Position == DXA_KEYV2_LENGTH )
					Position = 0 ;
				SizeT -- ;
			}while( SizeT > 0 ) ;
		}
	}
	else
	{
		BYTE *DataBP ;
		BYTE *KeyBP ;

		DataBP = ( BYTE * )Data ;
		KeyBP = ( BYTE * )Key ;
		if( SizeLL >= DXA_KEYV2_LENGTH )
		{
			if( Position != 0 )
			{
				do
				{
					*DataBP ^= KeyBP[ Position ] ;
					Position ++ ;
					DataBP ++ ;
					SizeLL -- ;
				}while( Position < DXA_KEYV2_LENGTH ) ;
				Position = 0 ;
			}

			if( SizeLL >= DXA_KEYV2_LENGTH )
			{
				DWORD SetNum ;
				DWORD Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8 ;

				SetNum = ( DWORD )( SizeLL / DXA_KEYV2_LENGTH ) ;
				SizeLL -= SetNum * DXA_KEYV2_LENGTH ;
				Key1 = ( ( DWORD * )KeyBP )[ 0 ] ;
				Key2 = ( ( DWORD * )KeyBP )[ 1 ] ;
				Key3 = ( ( DWORD * )KeyBP )[ 2 ] ;
				Key4 = ( ( DWORD * )KeyBP )[ 3 ] ;
				Key5 = ( ( DWORD * )KeyBP )[ 4 ] ;
				Key6 = ( ( DWORD * )KeyBP )[ 5 ] ;
				Key7 = ( ( DWORD * )KeyBP )[ 6 ] ;
				Key8 = ( ( DWORD * )KeyBP )[ 7 ] ;

				do
				{
					( ( DWORD * )DataBP )[ 0 ] ^= Key1 ;
					( ( DWORD * )DataBP )[ 1 ] ^= Key2 ;
					( ( DWORD * )DataBP )[ 2 ] ^= Key3 ;
					( ( DWORD * )DataBP )[ 3 ] ^= Key4 ;
					( ( DWORD * )DataBP )[ 4 ] ^= Key5 ;
					( ( DWORD * )DataBP )[ 5 ] ^= Key6 ;
					( ( DWORD * )DataBP )[ 6 ] ^= Key7 ;
					( ( DWORD * )DataBP )[ 7 ] ^= Key8 ;
					DataBP += DXA_KEYV2_LENGTH ;
					SetNum -- ;
				}while( SetNum > 0 ) ;
			}
		}

		if( SizeLL > 0 )
		{
			do
			{
				*DataBP ^= KeyBP[ Position ] ;
				DataBP ++ ;
				Position ++ ;
				if( Position == DXA_KEYV2_LENGTH )
					Position = 0 ;
				SizeLL -- ;
			}while( SizeLL > 0 ) ;
		}
	}
}

// ファイルから読み込んだデータを鍵文字列を使用して Xor 演算する関数( Key は必ず DXA_KEYSTR_LENGTH の長さがなければならない )
void DXA_KeyConvFileRead( void *Data, ULONGLONG  Size, DWORD_PTR FilePointer, unsigned char *Key, LONGLONG Position )
{
	LONGLONG pos ;

	// ファイルの位置を取得しておく
	if( Position == -1 )
	{
		pos = ReadOnlyFileAccessTell( FilePointer ) ;
	}
	else
	{
		pos = Position ;
	}

	// 読み込む
	ReadOnlyFileAccessRead( Data, ( size_t )Size, 1, FilePointer ) ;
	while( ReadOnlyFileAccessIdleCheck( FilePointer ) == FALSE )
	{
		Thread_Sleep( 1 ) ;
	}

	// データを鍵文字列を使って Xor 演算
	DXA_KeyConv( Data, ( LONGLONG )Size, pos, Key ) ;
}

// ファイルから読み込んだデータを鍵文字列を使用して Xor 演算する関数( Key は必ず DXA_KEYV2_LENGTH の長さがなければならない )
static void DXA_KeyV2ConvFileRead( void *Data, ULONGLONG Size, DWORD_PTR FilePointer, unsigned char *Key, LONGLONG Position )
{
	// 読み込む
	ReadOnlyFileAccessRead( Data, ( size_t )Size, 1, FilePointer ) ;
	while( ReadOnlyFileAccessIdleCheck( FilePointer ) == FALSE )
	{
		Thread_Sleep( 1 ) ;
	}

	// データを鍵文字列を使って Xor 演算
	DXA_KeyV2Conv( Data, ( LONGLONG )Size, Position, Key ) ;
}

// 条件に適合するオブジェクトを検索する(検索対象は ObjectCount をインデックスとしたところから)(戻り値 -1:エラー 0:成功)
static int DXA_FindProcess( DXA_FINDDATA *FindData, FILEINFOW *FileInfo )
{
	BYTE  *nameTable ;
	DXARC *DXA ;
	BYTE  *str ;
	BYTE  *name ;
	DWORD fileHeadSize ;
	BYTE  TempName[ 2048 ] ;
	BYTE  DotStr[ 16 ] ;
	BYTE  DoubleDotStr[ 16 ] ;

	DXA       = FindData->Container;
	nameTable = DXA->Table.NameTable;
	str       = FindData->SearchStr;

	ConvString( ( const char * )Ascii_DotStr,       -1, DX_CHARCODEFORMAT_ASCII, ( char * )DotStr,       sizeof( DotStr       ), DXA->CharCodeFormat ) ;
	ConvString( ( const char * )Ascii_DoubleDotStr, -1, DX_CHARCODEFORMAT_ASCII, ( char * )DoubleDotStr, sizeof( DoubleDotStr ), DXA->CharCodeFormat ) ;

	if( DXA->V5Flag )
	{
		int i, num, addnum ;

		DXARC_DIRECTORY_VER5 *dir ;
		DXARC_FILEHEAD_VER5  *file ;

		dir          = FindData->DirectoryV5 ;
		num          = ( int )dir->FileHeadNum ;
		addnum       = dir->ParentDirectoryAddress == 0xffffffff ? 1 : 2 ;
		fileHeadSize = ( DWORD )( DXA->HeadV5.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ) ;

		if( FindData->ObjectCount == ( DWORD )( num + addnum ) )
		{
			return -1 ;
		}

		file = ( DXARC_FILEHEAD_VER5 * )( DXA->Table.FileTable + dir->FileHeadAddress + fileHeadSize * FindData->ObjectCount ) ;
		for( i = ( int )FindData->ObjectCount; i < num + addnum; i ++ )
		{
			if( i >= num )
			{
				if( i - num == 0 )
				{
					if( DXA_DIR_FileNameCmp( DXA, DotStr, str ) == 0 )
					{
						break ;
					}
				}
				else
				if( i - num == 1 )
				{
					if( DXA_DIR_FileNameCmp( DXA, DoubleDotStr, str ) == 0 )
					{
						break ;
					}
				}
			}
			else
			{
				name = ( BYTE * )( nameTable + file->NameAddress + 4 ) ;
				if( DXA_DIR_FileNameCmp( DXA, name, str ) == 0 )
				{
					break ;
				}

				file = (DXARC_FILEHEAD_VER5 *)( (BYTE *)file + fileHeadSize ) ;
			}
		}
		FindData->ObjectCount = ( DWORD )i ;
		if( i == num + addnum )
		{
			return -1 ;
		}

		if( FileInfo )
		{
			if( i >= num )
			{
				switch( i - num )
				{
				default :
				case 0 :
					CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )DotStr  ) ;
					break ;

				case 1 :
					CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )DoubleDotStr ) ;
					break ;
				}
				FileInfo->DirFlag = 1 ;
				FileInfo->Size    = 0 ;
				_MEMSET( &FileInfo->CreationTime,  0, sizeof( FileInfo->CreationTime  ) ) ;
				_MEMSET( &FileInfo->LastWriteTime, 0, sizeof( FileInfo->LastWriteTime ) ) ;
			}
			else
			{
				name = ( BYTE * )( nameTable + file->NameAddress ) ;
				CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )( name + ( ( WORD * )name )[ 0 ] * 4 + 4 ) ) ;
				FileInfo->DirFlag = ( file->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? TRUE : FALSE ;
				FileInfo->Size    = ( LONGLONG )file->DataSize ;
#ifdef __WINDOWS__
				_FileTimeToLocalDateData( ( FILETIME * )&file->Time.Create,    &FileInfo->CreationTime  ) ;
				_FileTimeToLocalDateData( ( FILETIME * )&file->Time.LastWrite, &FileInfo->LastWriteTime ) ;
#else // __WINDOWS__
				_MEMSET( &FileInfo->CreationTime,  0, sizeof( FileInfo->CreationTime  ) ) ;
				_MEMSET( &FileInfo->LastWriteTime, 0, sizeof( FileInfo->LastWriteTime ) ) ;
#endif // __WINDOWS__
			}

			ConvString( ( const char * )TempName, -1, DXA->CharCodeFormat, ( char * )FileInfo->Name, sizeof( FileInfo->Name ), WCHAR_T_CHARCODEFORMAT ) ;
		}
	}
	else
	{
		ULONGLONG i, num, addnum ;
		DXARC_DIRECTORY *dir ;
		DXARC_FILEHEAD *file ;

		dir          = FindData->Directory ;
		num          = dir->FileHeadNum ;
		addnum       = ( ULONGLONG )( dir->ParentDirectoryAddress == NONE_PAL ? 1 : 2 ) ;
		fileHeadSize = DXARC_FILEHEAD_VER6_SIZE ;

		if( FindData->ObjectCount == ( DWORD )( num + addnum ) )
		{
			return -1 ;
		}

		file = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + dir->FileHeadAddress + fileHeadSize * FindData->ObjectCount ) ;
		for( i = FindData->ObjectCount; i < num + addnum; i ++ )
		{
			if( i >= num )
			{
				if( i - num == 0 )
				{
					if( DXA_DIR_FileNameCmp( DXA, DotStr,       str ) == 0 )
					{
						break ;
					}
				}
				else
				if( i - num == 1 )
				{
					if( DXA_DIR_FileNameCmp( DXA, DoubleDotStr, str ) == 0 )
					{
						break;
					}
				}
			}
			else
			{
				name = ( BYTE * )( nameTable + file->NameAddress + 4 ) ;
				if( DXA_DIR_FileNameCmp( DXA, name, str ) == 0 )
				{
					break ;
				}

				file = (DXARC_FILEHEAD *)( (BYTE *)file + fileHeadSize ) ;
			}
		}
		FindData->ObjectCount = ( DWORD )i ;
		if( i == num + addnum )
		{
			return -1 ;
		}

		if( FileInfo )
		{
			if( i >= num )
			{
				switch( i - num )
				{
				default :
				case 0:
					CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )DotStr       ) ;
					break ;

				case 1 :
					CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )DoubleDotStr ) ;
					break ;
				}
				FileInfo->DirFlag = 1 ;
				FileInfo->Size    = 0 ;
				_MEMSET( &FileInfo->CreationTime,  0, sizeof( FileInfo->CreationTime  ) ) ;
				_MEMSET( &FileInfo->LastWriteTime, 0, sizeof( FileInfo->LastWriteTime ) ) ;
			}
			else
			{
				name = ( BYTE * )( nameTable + file->NameAddress ) ;
				CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )( name + ( ( WORD * )name )[ 0 ] * 4 + 4 ) ) ;
				FileInfo->DirFlag = ( file->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? TRUE : FALSE ;
				FileInfo->Size    = ( LONGLONG)file->DataSize ;
#ifdef __WINDOWS__
				_FileTimeToLocalDateData( ( FILETIME * )&file->Time.Create,    &FileInfo->CreationTime  ) ;
				_FileTimeToLocalDateData( ( FILETIME * )&file->Time.LastWrite, &FileInfo->LastWriteTime ) ;
#else // __WINDOWS__
				_MEMSET( &FileInfo->CreationTime,  0, sizeof( FileInfo->CreationTime  ) ) ;
				_MEMSET( &FileInfo->LastWriteTime, 0, sizeof( FileInfo->LastWriteTime ) ) ;
#endif // __WINDOWS__
			}

			ConvString( ( const char * )TempName, -1, DXA->CharCodeFormat, ( char * )FileInfo->Name, sizeof( FileInfo->Name ), WCHAR_T_CHARCODEFORMAT ) ;
		}
	}

	return 0 ;
}



// アーカイブファイルを扱う為の構造体を初期化する
extern	int	DXA_Initialize( DXARC *DXA )
{
	_MEMSET( DXA, 0, sizeof( DXARC ) ) ;

	// 終了
	return 0 ;
}

// アーカイブファイルを扱う為の構造体の後始末をする
extern int DXA_Terminate( DXARC *DXA )
{
	DXA_CloseArchive( DXA ) ;

	_MEMSET( DXA, 0, sizeof( DXARC ) ) ;

	// 終了
	return 0 ;
}

// 指定のディレクトリデータの暗号化を解除する( 丸ごとメモリに読み込んだ場合用 )
static int DXA_DirectoryKeyConv( DXARC *DXA, DXARC_DIRECTORY *Dir, char *KeyV2StringBuffer )
{
	// メモリイメージではない場合はエラー
	if( DXA->MemoryOpenFlag == FALSE )
		return -1 ;

	// バージョン 0x0006 より前では何もしない
	if( DXA->V5Flag )
		return 0 ;

	// 暗号化解除処理開始
	{
		DWORD i, FileHeadSize ;
		DXARC_FILEHEAD *File ;
		unsigned char KeyV2[DXA_KEYV2_LENGTH] ;
		size_t KeyV2StringBytes ;

		// 格納されているファイルの数だけ繰り返す
		FileHeadSize = DXARC_FILEHEAD_VER6_SIZE ;
		File = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + Dir->FileHeadAddress ) ;
		for( i = 0 ; i < Dir->FileHeadNum ; i ++, File = ( DXARC_FILEHEAD * )( ( BYTE * )File + FileHeadSize ) )
		{
			// ディレクトリかどうかで処理を分岐
			if( File->Attributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// ディレクトリの場合は再帰をかける
				DXA_DirectoryKeyConv( DXA, ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + File->DataAddress ), KeyV2StringBuffer ) ;
			}
			else
			{
				BYTE *DataP ;

				// ファイルの場合は暗号化を解除する
				
				// データがある場合のみ処理
				if( File->DataSize != 0 )
				{
					// データ位置をセットする
					DataP = ( BYTE * )DXA->MemoryImage + DXA->Head.DataStartAddress + File->DataAddress ;

					// データが圧縮されているかどうかで処理を分岐
					if( File->PressDataSize != NONE_PAL )
					{
						// 圧縮されている場合
						if( DXA->Head.Version >= DXA_KEYV2_VER )
						{
							// ファイル個別の鍵を作成
							KeyV2StringBytes = DXA_CreateKeyV2FileString( DXA, Dir, File, ( BYTE * )KeyV2StringBuffer ) ;
							DXA_KeyV2Create( KeyV2StringBuffer, KeyV2 , KeyV2StringBytes ) ;

							DXA_KeyV2Conv( DataP, ( LONGLONG )File->PressDataSize, ( LONGLONG )File->DataSize, KeyV2 ) ;
						}
						else
						{
							DXA_KeyConv( DataP, ( LONGLONG )File->PressDataSize, ( LONGLONG )File->DataSize, DXA->Key ) ;
						}
					}
					else
					{
						// 圧縮されていない場合
						if( DXA->Head.Version >= DXA_KEYV2_VER )
						{
							// ファイル個別の鍵を作成
							KeyV2StringBytes = DXA_CreateKeyV2FileString( DXA, Dir, File, ( BYTE * )KeyV2StringBuffer ) ;
							DXA_KeyV2Create( KeyV2StringBuffer, KeyV2 , KeyV2StringBytes ) ;

							DXA_KeyV2Conv( DataP, ( LONGLONG )File->DataSize, ( LONGLONG )File->DataSize, KeyV2 ) ;
						}
						else
						{
							DXA_KeyConv( DataP, ( LONGLONG )File->DataSize, ( LONGLONG )File->DataSize, DXA->Key ) ;
						}
					}
				}
			}
		}
	}

	// 終了
	return 0 ;
}

// 指定のディレクトリデータの暗号化を解除する( 丸ごとメモリに読み込んだ場合用 )
static int DXA_DirectoryKeyConvV5( DXARC *DXA, DXARC_DIRECTORY_VER5 *Dir )
{
	// メモリイメージではない場合はエラー
	if( DXA->MemoryOpenFlag == FALSE )
	{
		return -1 ;
	}

	// バージョン 0x0005 より前では何もしない
	if( DXA->HeadV5.Version < 0x0005 )
	{
		return 0 ;
	}
	
	// 暗号化解除処理開始
	{
		DWORD i, FileHeadSize ;
		DXARC_FILEHEAD_VER5 *File ;

		// 格納されているファイルの数だけ繰り返す
		FileHeadSize = ( DWORD )( DXA->HeadV5.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ) ;
		File = ( DXARC_FILEHEAD_VER5 * )( DXA->Table.FileTable + Dir->FileHeadAddress ) ;
		for( i = 0 ; i < Dir->FileHeadNum ; i ++, File = ( DXARC_FILEHEAD_VER5 * )( ( BYTE * )File + FileHeadSize ) )
		{
			// ディレクトリかどうかで処理を分岐
			if( File->Attributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// ディレクトリの場合は再帰をかける
				DXA_DirectoryKeyConvV5( DXA, ( DXARC_DIRECTORY_VER5 * )( DXA->Table.DirectoryTable + File->DataAddress ) ) ;
			}
			else
			{
				BYTE *DataP ;

				// ファイルの場合は暗号化を解除する
				
				// データがある場合のみ処理
				if( File->DataSize != 0 )
				{
					// データ位置をセットする
					DataP = ( BYTE * )DXA->MemoryImage + DXA->HeadV5.DataStartAddress + File->DataAddress ;

					// データが圧縮されているかどうかで処理を分岐
					if( DXA->HeadV5.Version >= 0x0002 && File->PressDataSize != 0xffffffff )
					{
						// 圧縮されている場合
						DXA_KeyConv( DataP, File->PressDataSize, File->DataSize, DXA->Key ) ;
					}
					else
					{
						// 圧縮されていない場合
						DXA_KeyConv( DataP, File->DataSize, File->DataSize, DXA->Key ) ;
					}
				}
			}
		}
	}

	// 終了
	return 0 ;
}

// メモリ上にあるアーカイブファイルイメージを開く( 0:成功  -1:失敗 )
extern int DXA_OpenArchiveFromMem( DXARC *DXA, void *ArchiveImage, int ArchiveSize, int ArchiveImageCopyFlag, int ArchiveImageReadOnlyFlag, const char *KeyString, const wchar_t *EmulateArchivePath )
{
	BYTE *datp ;

	// メモリイメージをコピーして使用する場合は読み取り専用にはしない
	if( ArchiveImageCopyFlag )
	{
		ArchiveImageReadOnlyFlag = FALSE ;
	}

	// 既になんらかのアーカイブを開いていた場合はエラー
	if( DXA->WinFilePointer__ != 0 || DXA->MemoryImage != NULL ) return -1 ;

	// 鍵の作成
	DXA_KeyCreate( KeyString, DXA->Key ) ;
	DXA_KeyV2Create( KeyString, DXA->KeyV2 ) ;

	// 鍵文字列の保存
	if( KeyString == NULL )
	{
		DXA->KeyV2String[ 0 ] = '\0' ;
		DXA->KeyV2StringBytes = 0 ;
	}
	else
	{
		DXA->KeyV2StringBytes = CL_strlen( DX_CHARCODEFORMAT_ASCII, KeyString ) ;
		if( DXA->KeyV2StringBytes > DXA_KEYV2STR_LENGTH )
		{
			DXA->KeyV2StringBytes = DXA_KEYV2STR_LENGTH ;
		}
		_MEMCPY( DXA->KeyV2String, KeyString, DXA->KeyV2StringBytes ) ;
		DXA->KeyV2String[ DXA->KeyV2StringBytes ] = '\0' ;
	}

	// ファイルパスを保存
	if( EmulateArchivePath != NULL )
	{
		_WCSCPY_S( DXA->FilePath, sizeof( DXA->FilePath ), EmulateArchivePath ) ;
	}
	else
	{
		_MEMSET( DXA->FilePath, 0, sizeof( DXA->FilePath ) ) ;
	}

	DXA->Table.Top   = NULL ;
	DXA->MemoryImage = NULL ;
	if( ArchiveImageCopyFlag )
	{
		// イメージをコピーするフラグが立っている場合はコピー先のメモリ領域を確保
		DXA->MemoryImage = DXALLOC( ( size_t )ArchiveSize ) ;
		if( DXA->MemoryImage == NULL )
		{
			return -1 ;
		}

		// アーカイブファイルイメージの内容を確保したメモリにコピー
		_MEMCPY( DXA->MemoryImage, ArchiveImage, ( size_t )ArchiveSize ) ;

		// コピーしたメモリイメージを使用する
		DXA->MemoryImageOriginal = ArchiveImage ;
		ArchiveImage             = DXA->MemoryImage ;
	}
	else
	{
		// ポインタを保存
		DXA->MemoryImage         = ArchiveImage ;
		DXA->MemoryImageOriginal = NULL ;
	}

	DXA->V5Flag = FALSE ;

	// 最初にヘッダの部分を反転する
	_MEMCPY( &DXA->Head, ArchiveImage, DXARC_ID_AND_VERSION_SIZE ) ;
	DXA_KeyV2Conv( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, 0, DXA->KeyV2 ) ;

	// ＩＤの検査
	if( DXA->Head.Head != DXAHEAD )
	{
		// バージョン６以前か調べる
		_MEMCPY( &DXA->Head, ArchiveImage, DXARC_ID_AND_VERSION_SIZE ) ;
		DXA_KeyConv( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, 0, DXA->Key ) ;

		// ＩＤの検査
		if( DXA->Head.Head != DXAHEAD )
		{
			// バージョン２以前か調べる
			_MEMSET( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;

			_MEMCPY( &DXA->Head, ArchiveImage, DXARC_ID_AND_VERSION_SIZE ) ;
			DXA_KeyConv( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, 0, DXA->Key ) ;

			// バージョン２以前でもない場合はエラー
			if( DXA->Head.Head != DXAHEAD )
			{
				goto ERR ;
			}
		}
	}

	// バージョン6以降かどうかで処理を分岐
	if( DXA->Head.Version >= 0x0006 )
	{
		DXA->V5Flag = FALSE ;

		DXA->Head.CharCodeFormat = 0 ;

		// ヘッダを解析する
		{
			if( DXA->Head.Version >= DXA_KEYV2_VER )
			{
				_MEMCPY( &DXA->Head, ArchiveImage, DXARC_HEAD_VER6_SIZE ) ;
				DXA_KeyV2Conv( &DXA->Head, DXARC_HEAD_VER6_SIZE, 0, DXA->KeyV2 ) ;
			}
			else
			{
				_MEMCPY( &DXA->Head, ArchiveImage, DXARC_HEAD_VER6_SIZE ) ;
				DXA_KeyConv( &DXA->Head, DXARC_HEAD_VER6_SIZE, 0, DXA->Key ) ;
			}

			// ＩＤの検査
			if( DXA->Head.Head != DXAHEAD )
			{
				goto ERR ;
			}
			
			// バージョン検査
			if( DXA->Head.Version > DXAVER )
			{
				goto ERR ;
			}

			// 文字コード形式をセット
			switch( DXA->Head.CharCodeFormat )
			{
			case DX_CHARCODEFORMAT_UHC :
			case DX_CHARCODEFORMAT_BIG5 :
			case DX_CHARCODEFORMAT_GB2312 :
			case DX_CHARCODEFORMAT_SHIFTJIS :
			case DX_CHARCODEFORMAT_UTF16LE :
			case DX_CHARCODEFORMAT_UTF16BE :
			case DX_CHARCODEFORMAT_WINDOWS_1252 :
			case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
			case DX_CHARCODEFORMAT_UTF8 :
			case DX_CHARCODEFORMAT_UTF32LE :
			case DX_CHARCODEFORMAT_UTF32BE :
				DXA->CharCodeFormat = ( int )DXA->Head.CharCodeFormat ;
				break ;

			default :
				DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
				break ;
			}

			// 読み取り専用の場合は情報テーブルのサイズ分のメモリを確保する
			if( ArchiveImageReadOnlyFlag )
			{
				DXA->Table.Top = ( BYTE * )DXALLOC( DXA->Head.HeadSize ) ;
				if( DXA->Table.Top == NULL )
				{
					goto ERR ;
				}

				_MEMCPY( DXA->Table.Top,  (BYTE *)DXA->MemoryImage + DXA->Head.FileNameTableStartAddress, DXA->Head.HeadSize ) ;
			}
			else
			{
				// 情報テーブルのアドレスをセットする
				DXA->Table.Top          = (BYTE *)DXA->MemoryImage + DXA->Head.FileNameTableStartAddress ;
			}
			if( DXA->Head.Version >= DXA_KEYV2_VER )
			{
				DXA_KeyV2Conv( DXA->Table.Top, DXA->Head.HeadSize, 0, DXA->KeyV2 ) ;
			}
			else
			{
				DXA_KeyConv( DXA->Table.Top, DXA->Head.HeadSize, 0, DXA->Key ) ;
			}

			DXA->Table.NameTable		= DXA->Table.Top ;
			DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
			DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;
		}

		// カレントディレクトリのセット
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;

		DXA->MemoryOpenFlag					= TRUE ;						// メモリイメージから開いているフラグを立てる
		DXA->UserMemoryImageFlag			= TRUE ;						// ユーザーのイメージから開いたフラグを立てる
		DXA->MemoryImageSize				= ArchiveSize ;					// サイズも保存しておく
		DXA->MemoryImageCopyFlag			= ArchiveImageCopyFlag ;		// メモリイメージをコピーして使用しているかどうかのフラグを保存
		DXA->MemoryImageReadOnlyFlag        = ArchiveImageReadOnlyFlag ;	// メモリイメージを読み取り専用にするかどうかのフラグを保存

		// 全てのファイルデータの暗号化を解除する
		if( ArchiveImageReadOnlyFlag == FALSE )
		{
			char KeyV2String[ DXA_KEYV2_STRING_MAXLENGTH ] ;
			DXA_DirectoryKeyConv( DXA, ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable, KeyV2String ) ;
		}
	}
	else
	{
		DXA->V5Flag = TRUE ;

		// バージョン検査
		if( DXA->HeadV5.Version > DXAVER_VER5 )
		{
			goto ERR ;
		}

		// メモリを読み取り専用とするかどうかで処理を分岐
		if( ArchiveImageReadOnlyFlag )
		{
			// バージョンが４以上かどうかで読み込む残りのヘッダサイズを変更
			if( DXA->HeadV5.Version >= 0x0004 )
			{
				_MEMCPY( &DXA->HeadV5, ArchiveImage, DXARC_HEAD_VER4_SIZE ) ;
				DXA_KeyConv( &DXA->HeadV5, DXARC_HEAD_VER4_SIZE, 0, DXA->Key ) ;
				switch( DXA->HeadV5.CharCodeFormat )
				{
				case DX_CHARCODEFORMAT_UHC :
				case DX_CHARCODEFORMAT_BIG5 :
				case DX_CHARCODEFORMAT_GB2312 :
				case DX_CHARCODEFORMAT_SHIFTJIS :
				case DX_CHARCODEFORMAT_UTF16LE :
				case DX_CHARCODEFORMAT_UTF16BE :
				case DX_CHARCODEFORMAT_WINDOWS_1252 :
				case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
				case DX_CHARCODEFORMAT_UTF8 :
				case DX_CHARCODEFORMAT_UTF32LE :
				case DX_CHARCODEFORMAT_UTF32BE :
					DXA->CharCodeFormat = ( int )DXA->HeadV5.CharCodeFormat ;
					break ;

				default :
					DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
					break ;
				}
			}
			else
			{
				_MEMCPY( &DXA->HeadV5, ArchiveImage, DXARC_HEAD_VER3_SIZE ) ;
				DXA_KeyConv( &DXA->HeadV5, DXARC_HEAD_VER3_SIZE, 0, DXA->Key ) ;

				DXA->HeadV5.CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
				DXA->CharCodeFormat        = DX_CHARCODEFORMAT_SHIFTJIS ;
			}

			// 情報テーブルのサイズ分のメモリを確保する
			DXA->Table.Top = ( BYTE * )DXALLOC( DXA->HeadV5.HeadSize ) ;
			if( DXA->Table.Top == NULL )
			{
				goto ERR ;
			}
			
			// 情報テーブルをメモリに読み込む
			_MEMCPY( DXA->Table.Top, ( BYTE * )DXA->MemoryImage + DXA->HeadV5.FileNameTableStartAddress, DXA->HeadV5.HeadSize ) ;
			if( DXA->HeadV5.Version >= 0x0005 )
			{
				DXA_KeyConv( DXA->Table.Top, DXA->HeadV5.HeadSize,                                     0, DXA->Key ) ;
			}
			else
			{
				DXA_KeyConv( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->HeadV5.FileNameTableStartAddress, DXA->Key ) ;
			}
		}
		else
		{
			// すべてのデータを反転する
			if( DXA->HeadV5.Version < 0x0005 )
			{
				DXA_KeyConv( ArchiveImage, ArchiveSize, 0, DXA->Key ) ;
			}

			datp = (BYTE *)ArchiveImage ;

			// ヘッダを解析する
			{
				if( DXA->HeadV5.Version >= 0x0005 )
				{
					_MEMCPY( &DXA->HeadV5, datp, DXARC_HEAD_VER3_SIZE ) ;
					DXA_KeyConv( &DXA->HeadV5, DXARC_HEAD_VER3_SIZE, 0, DXA->Key ) ;
				}
				else
				{
					_MEMCPY( &DXA->HeadV5, datp, DXARC_HEAD_VER3_SIZE ) ;
				}
				datp += DXARC_HEAD_VER3_SIZE ;

				// ＩＤの検査
				if( DXA->HeadV5.Head != DXAHEAD )
				{
					goto ERR ;
				}
				
				// バージョン検査
				if( DXA->HeadV5.Version > DXAVER_VER5 )
				{
					goto ERR ;
				}

				// バージョンが 4以上だったら文字コード形式を読み込む
				if( DXA->HeadV5.Version >= 0x0004 )
				{
					DXA->HeadV5.CharCodeFormat = *( ( DWORD * )datp ) ;
					if( DXA->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConv( &DXA->HeadV5.CharCodeFormat, 4, DXARC_HEAD_VER3_SIZE, DXA->Key ) ;
					}
					switch( DXA->HeadV5.CharCodeFormat )
					{
					case DX_CHARCODEFORMAT_UHC :
					case DX_CHARCODEFORMAT_BIG5 :
					case DX_CHARCODEFORMAT_GB2312 :
					case DX_CHARCODEFORMAT_SHIFTJIS :
					case DX_CHARCODEFORMAT_UTF16LE :
					case DX_CHARCODEFORMAT_UTF16BE :
					case DX_CHARCODEFORMAT_WINDOWS_1252 :
					case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
					case DX_CHARCODEFORMAT_UTF8 :
					case DX_CHARCODEFORMAT_UTF32LE :
					case DX_CHARCODEFORMAT_UTF32BE :
						DXA->CharCodeFormat = ( int )DXA->HeadV5.CharCodeFormat ;
						break ;

					default :
						DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
						break ;
					}
				}
				else
				{
					DXA->HeadV5.CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
					DXA->CharCodeFormat        = DX_CHARCODEFORMAT_SHIFTJIS ;
				}

				// 情報テーブルのアドレスをセットする
				DXA->Table.Top = (BYTE *)DXA->MemoryImage + DXA->HeadV5.FileNameTableStartAddress ;
				if( DXA->HeadV5.Version >= 0x0005 )
				{
					DXA_KeyConv( DXA->Table.Top, DXA->HeadV5.HeadSize, 0, DXA->Key ) ;
				}
			}
		}

		// 情報テーブルのアドレスをセットする
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->HeadV5.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->HeadV5.DirectoryTableStartAddress ;

		// カレントディレクトリのセット
		DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ;

		DXA->MemoryOpenFlag					= TRUE ;						// メモリイメージから開いているフラグを立てる
		DXA->UserMemoryImageFlag			= TRUE ;						// ユーザーのイメージから開いたフラグを立てる
		DXA->MemoryImageSize				= ArchiveSize ;					// サイズも保存しておく
		DXA->MemoryImageCopyFlag			= ArchiveImageCopyFlag ;		// メモリイメージをコピーして使用しているかどうかのフラグを保存
		DXA->MemoryImageReadOnlyFlag        = ArchiveImageReadOnlyFlag ;	// メモリイメージを読み取り専用にするかどうかのフラグを保存

		if( ArchiveImageReadOnlyFlag == FALSE )
		{
			// 全てのファイルデータの暗号化を解除する
			if( DXA->HeadV5.Version >= 0x0005 )
			{
				DXA_DirectoryKeyConvV5( DXA, ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ) ;
			}
		}
	}

	// 終了
	return 0 ;

ERR :
	if( ArchiveImageCopyFlag )
	{
		if( DXA->MemoryImage != NULL )
		{
			DXFREE( DXA->MemoryImage ) ;
			DXA->MemoryImage = NULL ;
		}
	}
	else
	if( ArchiveImageReadOnlyFlag )
	{
		if( DXA->Table.Top != NULL )
		{
			DXFREE( DXA->Table.Top ) ;
			DXA->Table.Top = NULL ;
		}
	}
	else
	{
		if( DXA->V5Flag )
		{
			// 反転したデータを元に戻す
			if( DXA->HeadV5.Version < 0x0005 )
			{
				DXA_KeyConv( ArchiveImage, ArchiveSize, 0, DXA->Key ) ;
			}
		}
	}
	
	// 終了
	return -1 ;
}

// アーカイブファイルを扱う準備が整ったかを得る( TRUE:整っている  FALSE:整っていない )
extern int DXA_CheckIdle( DXARC *DXA )
{
	// 非同期オープン中ではなければ特にやることはない
	if( DXA->ASyncOpenFlag == FALSE )
	{
		return TRUE ;
	}

	// ファイル読み込みが完了しているか調べる
	if( ReadOnlyFileAccessIdleCheck( DXA->ASyncOpenFilePointer ) == FALSE )
	{
		return FALSE ;
	}

	// ファイルを閉じる
	ReadOnlyFileAccessClose( DXA->ASyncOpenFilePointer ) ;
	DXA->ASyncOpenFilePointer = 0;

	// すべてのデータを反転する
	if( DXA->V5Flag == FALSE )
	{
		char KeyV2String[ DXA_KEYV2_STRING_MAXLENGTH ] ;
		DXA_DirectoryKeyConv( DXA, ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable, KeyV2String ) ;
	}
	else
	{
		if( DXA->HeadV5.Version >= 0x0005 )
		{
			DXA_DirectoryKeyConvV5( DXA, ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ) ;
		}
		else
		{
			DXA_KeyConv( DXA->MemoryImage, DXA->MemoryImageSize, 0, DXA->Key ) ;
		}
	}

	// 非同期オープン中フラグを倒す
	DXA->ASyncOpenFlag = FALSE ;

	// 終了
	return TRUE ;
}

// アーカイブファイルを開き最初にすべてメモリ上に読み込んでから処理する( 0:成功  -1:失敗 )
extern int DXA_OpenArchiveFromFileUseMem( DXARC *DXA, const wchar_t *ArchivePath, const char *KeyString , int ASyncThread )
{
	// 既になんらかのアーカイブを開いていた場合はエラー
	if( DXA->WinFilePointer__ != 0 || DXA->MemoryImage )
	{
		return -1 ;
	}

	// 鍵の作成
	DXA_KeyCreate( KeyString, DXA->Key ) ;
	DXA_KeyV2Create( KeyString, DXA->KeyV2 ) ;

	// 鍵文字列の保存
	if( KeyString == NULL )
	{
		DXA->KeyV2String[ 0 ] = '\0' ;
		DXA->KeyV2StringBytes = 0 ;
	}
	else
	{
		DXA->KeyV2StringBytes = CL_strlen( DX_CHARCODEFORMAT_ASCII, KeyString ) ;
		if( DXA->KeyV2StringBytes > DXA_KEYV2STR_LENGTH )
		{
			DXA->KeyV2StringBytes = DXA_KEYV2STR_LENGTH ;
		}
		_MEMCPY( DXA->KeyV2String, KeyString, DXA->KeyV2StringBytes ) ;
		DXA->KeyV2String[ DXA->KeyV2StringBytes ] = '\0' ;
	}

	// ヘッダ部分だけ先に読み込む
	DXA->ASyncOpenFilePointer = 0 ;
	DXA->MemoryImage          = NULL ;
	DXA->ASyncOpenFilePointer = ReadOnlyFileAccessOpen( ArchivePath, FALSE, TRUE, FALSE ) ;
	if( DXA->ASyncOpenFilePointer == 0 )
	{
		return -1 ;
	}

	// ファイルパスを保存
	_WCSCPY_S( DXA->FilePath, sizeof( DXA->FilePath ), ArchivePath ) ;

	// ファイルのサイズを取得する
	ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_END ) ;
	DXA->MemoryImageSize = ReadOnlyFileAccessTell( DXA->ASyncOpenFilePointer ) ;
	ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;

	// ファイルの内容を全てメモリに読み込む為のメモリ領域の確保
	DXA->MemoryImage = DXALLOC( ( size_t )DXA->MemoryImageSize ) ;

	// ＩＤとバージョン番号部分だけを読み込み
	DXA_KeyV2ConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->KeyV2, 0 ) ;

	// ＩＤを検査
	if( DXA->Head.Head != DXAHEAD )
	{
		// バージョン６以前か調べる
		ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;
		DXA_KeyConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, 0 ) ;

		// ＩＤの検査
		if( DXA->Head.Head != DXAHEAD )
		{
			// バージョン２以前か調べる
			_MEMSET( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;

			ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;
			DXA_KeyConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, 0 ) ;

			// バージョン２以前でもない場合はエラー
			if( DXA->Head.Head != DXAHEAD )
			{
				goto ERR ;
			}
		}
	}

	// バージョン6以降かどうかで処理を分岐
	if( DXA->Head.Version >= 0x0006 )
	{
		DXA->V5Flag = FALSE ;

		// バージョン検査
		if( DXA->Head.Version > DXAVER )
		{
			goto ERR ;
		}

		// バージョンが４以上かどうかで読み込む残りのヘッダサイズを変更
		if( DXA->Head.Version >= DXA_KEYV2_VER )
		{
			DXA_KeyV2ConvFileRead( ( BYTE * )&DXA->Head + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER6_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->KeyV2, DXARC_ID_AND_VERSION_SIZE ) ;
		}
		else
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->Head + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER6_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
		}
		switch( DXA->Head.CharCodeFormat )
		{
		case DX_CHARCODEFORMAT_UHC :
		case DX_CHARCODEFORMAT_BIG5 :
		case DX_CHARCODEFORMAT_GB2312 :
		case DX_CHARCODEFORMAT_SHIFTJIS :
		case DX_CHARCODEFORMAT_UTF16LE :
		case DX_CHARCODEFORMAT_UTF16BE :
		case DX_CHARCODEFORMAT_WINDOWS_1252 :
		case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
		case DX_CHARCODEFORMAT_UTF8 :
		case DX_CHARCODEFORMAT_UTF32LE :
		case DX_CHARCODEFORMAT_UTF32BE :
			DXA->CharCodeFormat = ( int )DXA->Head.CharCodeFormat ;
			break ;

		default :
			DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
			break ;
		}
		
		// 情報テーブルのサイズ分のメモリを確保する
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->Head.HeadSize ) ;
		if( DXA->Table.Top == NULL )
		{
			goto ERR ;
		}
		
		// 情報テーブルをメモリに読み込む
		ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, ( LONGLONG )DXA->Head.FileNameTableStartAddress, SEEK_SET ) ;
		if( DXA->Head.Version >= DXA_KEYV2_VER )
		{
			DXA_KeyV2ConvFileRead( DXA->Table.Top, DXA->Head.HeadSize, DXA->ASyncOpenFilePointer, DXA->KeyV2, 0 ) ;
		}
		else
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->Head.HeadSize, DXA->ASyncOpenFilePointer, DXA->Key, 0 ) ;
		}

		// 情報テーブルのアドレスをセットする
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;

		// カレントディレクトリのセット
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;
	}
	else
	{
		DXA->V5Flag = TRUE ;

		// バージョン検査
		if( DXA->HeadV5.Version > DXAVER_VER5 )
		{
			goto ERR ;
		}

		// バージョンが４以上かどうかで読み込む残りのヘッダサイズを変更
		if( DXA->HeadV5.Version >= 0x0004 )
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->HeadV5 + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER4_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
			switch( DXA->HeadV5.CharCodeFormat )
			{
			case DX_CHARCODEFORMAT_UHC :
			case DX_CHARCODEFORMAT_BIG5 :
			case DX_CHARCODEFORMAT_GB2312 :
			case DX_CHARCODEFORMAT_SHIFTJIS :
			case DX_CHARCODEFORMAT_UTF16LE :
			case DX_CHARCODEFORMAT_UTF16BE :
			case DX_CHARCODEFORMAT_WINDOWS_1252 :
			case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
			case DX_CHARCODEFORMAT_UTF8 :
			case DX_CHARCODEFORMAT_UTF32LE :
			case DX_CHARCODEFORMAT_UTF32BE :
				DXA->CharCodeFormat = ( int )DXA->HeadV5.CharCodeFormat ;
				break ;

			default :
				DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
				break ;
			}
		}
		else
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->HeadV5 + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER3_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
			DXA->HeadV5.CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
			DXA->CharCodeFormat        = DX_CHARCODEFORMAT_SHIFTJIS ;
		}
		
		// 情報テーブルのサイズ分のメモリを確保する
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->HeadV5.HeadSize ) ;
		if( DXA->Table.Top == NULL )
		{
			goto ERR ;
		}
		
		// 情報テーブルをメモリに読み込む
		ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, DXA->HeadV5.FileNameTableStartAddress, SEEK_SET ) ;
		if( DXA->HeadV5.Version >= 0x0005 )
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->ASyncOpenFilePointer, DXA->Key, 0 ) ;
		}
		else
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->ASyncOpenFilePointer, DXA->Key ) ;
		}

		// 情報テーブルのアドレスをセットする
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->HeadV5.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->HeadV5.DirectoryTableStartAddress ;

		// カレントディレクトリのセット
		DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ;
	}

	// 改めてファイルを丸ごと読み込む
	ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;
	ReadOnlyFileAccessRead( DXA->MemoryImage, ( size_t )DXA->MemoryImageSize, 1, DXA->ASyncOpenFilePointer );

	// ファイル非同期オープン中だということをセットしておく
	DXA->ASyncOpenFlag = TRUE ;

	DXA->MemoryOpenFlag					= TRUE ;			// メモリイメージから開いているフラグを立てる
	DXA->UserMemoryImageFlag			= FALSE ;			// ユーザーのイメージから開いたわけではないのでフラグを倒す
	DXA->MemoryImageCopyFlag			= FALSE ;			// メモリイメージをコピーして使用しているフラグを倒す
	DXA->MemoryImageReadOnlyFlag        = FALSE ;			// メモリイメージを読み取り専用にするかどうかのフラグを倒す

	// 同期オープンの場合はここで開き終わるのを待つ
	if( ASyncThread == FALSE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// 終了
	return 0 ;

ERR :
	if( DXA->ASyncOpenFilePointer )
	{
		ReadOnlyFileAccessClose( DXA->ASyncOpenFilePointer );
		DXA->ASyncOpenFilePointer = 0;
	}

	if( DXA->MemoryImage )
	{
		DXFREE( DXA->MemoryImage );
		DXA->MemoryImage = 0;
	}

	DXA->ASyncOpenFlag = FALSE ;

	// 終了
	return -1 ;
}

// アーカイブファイルを開く( 0:成功  -1:失敗 )
extern int DXA_OpenArchiveFromFile( DXARC *DXA, const wchar_t *ArchivePath, const char *KeyString )
{
	// 既になんらかのアーカイブを開いていた場合はエラー
	if( DXA->WinFilePointer__ != 0 || DXA->MemoryImage )
	{
		return -1 ;
	}

	// ヘッダの初期化
	_MEMSET( &DXA->Head, 0, sizeof( DXA->Head ) ) ;

	// アーカイブファイルを開こうと試みる
	DXA->WinFilePointer__ = ReadOnlyFileAccessOpen( ArchivePath, FALSE, TRUE, FALSE ) ;
	if( DXA->WinFilePointer__ == 0 )
	{
		return -1 ;
	}

	// ファイルパスを保存
	_WCSCPY_S( DXA->FilePath, sizeof( DXA->FilePath ), ArchivePath ) ;

	// 鍵の作成
	DXA_KeyCreate( KeyString, DXA->Key ) ;
	DXA_KeyV2Create( KeyString, DXA->KeyV2 ) ;

	// 鍵文字列の保存
	if( KeyString == NULL )
	{
		DXA->KeyV2String[ 0 ] = '\0' ;
		DXA->KeyV2StringBytes = 0 ;
	}
	else
	{
		DXA->KeyV2StringBytes = CL_strlen( DX_CHARCODEFORMAT_ASCII, KeyString ) ;
		if( DXA->KeyV2StringBytes > DXA_KEYV2STR_LENGTH )
		{
			DXA->KeyV2StringBytes = DXA_KEYV2STR_LENGTH ;
		}
		_MEMCPY( DXA->KeyV2String, KeyString, DXA->KeyV2StringBytes ) ;
		DXA->KeyV2String[ DXA->KeyV2StringBytes ] = '\0' ;
	}

	// ＩＤとバージョン番号部分だけを読み込み
	DXA_KeyV2ConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->KeyV2, 0 ) ;

	// ＩＤの検査
	if( DXA->Head.Head != DXAHEAD )
	{
		// バージョン６以前か調べる
		ReadOnlyFileAccessSeek( DXA->WinFilePointer__, 0L, SEEK_SET ) ;
		DXA_KeyConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, 0 ) ;

		// ＩＤの検査
		if( DXA->Head.Head != DXAHEAD )
		{
			// バージョン２以前か調べる
			_MEMSET( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;

			ReadOnlyFileAccessSeek( DXA->WinFilePointer__, 0L, SEEK_SET ) ;
			DXA_KeyConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, 0 ) ;

			// バージョン２以前でもない場合はエラー
			if( DXA->Head.Head != DXAHEAD )
			{
				goto ERR ;
			}
		}
	}

	// バージョン6以降かどうかで処理を分岐
	if( DXA->Head.Version >= 0x0006 )
	{
		DXA->V5Flag = FALSE ;

		// バージョン検査
		if( DXA->Head.Version > DXAVER )
		{
			goto ERR ;
		}

		if( DXA->Head.Version >= DXA_KEYV2_VER )
		{
			DXA_KeyV2ConvFileRead( ( BYTE * )&DXA->Head + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER6_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->KeyV2, DXARC_ID_AND_VERSION_SIZE ) ;
		}
		else
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->Head + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER6_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
		}
		switch( DXA->Head.CharCodeFormat )
		{
		case DX_CHARCODEFORMAT_UHC :
		case DX_CHARCODEFORMAT_BIG5 :
		case DX_CHARCODEFORMAT_GB2312 :
		case DX_CHARCODEFORMAT_SHIFTJIS :
		case DX_CHARCODEFORMAT_UTF16LE :
		case DX_CHARCODEFORMAT_UTF16BE :
		case DX_CHARCODEFORMAT_WINDOWS_1252 :
		case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
		case DX_CHARCODEFORMAT_UTF8 :
		case DX_CHARCODEFORMAT_UTF32LE :
		case DX_CHARCODEFORMAT_UTF32BE :
			DXA->CharCodeFormat = ( int )DXA->Head.CharCodeFormat ;
			break ;

		default :
			DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
			break ;
		}

		// 情報テーブルのサイズ分のメモリを確保する
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->Head.HeadSize ) ;
		if( DXA->Table.Top == NULL )
		{
			goto ERR ;
		}
		
		// 情報テーブルをメモリに読み込む
		ReadOnlyFileAccessSeek( DXA->WinFilePointer__, ( LONGLONG )DXA->Head.FileNameTableStartAddress, SEEK_SET ) ;
		if( DXA->Head.Version >= DXA_KEYV2_VER )
		{
			DXA_KeyV2ConvFileRead( DXA->Table.Top, DXA->Head.HeadSize, DXA->WinFilePointer__, DXA->KeyV2, 0 ) ;
		}
		else
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->Head.HeadSize, DXA->WinFilePointer__, DXA->Key, 0 ) ;
		}

		// 情報テーブルのアドレスをセットする
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;

		// カレントディレクトリのセット
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;
	}
	else
	{
		DXA->V5Flag = TRUE ;

		// バージョン検査
		if( DXA->HeadV5.Version > DXAVER_VER5 )
		{
			goto ERR ;
		}

		// バージョンが４以上かどうかで読み込む残りのヘッダサイズを変更
		if( DXA->HeadV5.Version >= 0x0004 )
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->HeadV5 + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER4_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
			switch( DXA->HeadV5.CharCodeFormat )
			{
			case DX_CHARCODEFORMAT_UHC :
			case DX_CHARCODEFORMAT_BIG5 :
			case DX_CHARCODEFORMAT_GB2312 :
			case DX_CHARCODEFORMAT_SHIFTJIS :
			case DX_CHARCODEFORMAT_UTF16LE :
			case DX_CHARCODEFORMAT_UTF16BE :
			case DX_CHARCODEFORMAT_WINDOWS_1252 :
			case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
			case DX_CHARCODEFORMAT_UTF8 :
			case DX_CHARCODEFORMAT_UTF32LE :
			case DX_CHARCODEFORMAT_UTF32BE :
				DXA->CharCodeFormat = ( int )DXA->HeadV5.CharCodeFormat ;
				break ;

			default :
				DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
				break ;
			}
		}
		else
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->HeadV5 + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER3_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
			DXA->HeadV5.CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
			DXA->CharCodeFormat        = DX_CHARCODEFORMAT_SHIFTJIS ;
		}

		// 情報テーブルのサイズ分のメモリを確保する
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->HeadV5.HeadSize ) ;
		if( DXA->Table.Top == NULL )
		{
			goto ERR ;
		}
		
		// 情報テーブルをメモリに読み込む
		ReadOnlyFileAccessSeek( DXA->WinFilePointer__, DXA->HeadV5.FileNameTableStartAddress, SEEK_SET ) ;
		if( DXA->HeadV5.Version >= 0x0005 )
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->WinFilePointer__, DXA->Key, 0 ) ;
		}
		else
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->WinFilePointer__, DXA->Key ) ;
		}

		// 情報テーブルのアドレスをセットする
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->HeadV5.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->HeadV5.DirectoryTableStartAddress ;

		// カレントディレクトリのセット
		DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ;
	}

	DXA->MemoryOpenFlag					= FALSE ;			// メモリイメージから開いているフラグを倒す
	DXA->UserMemoryImageFlag			= FALSE ;			// ユーザーのイメージから開いたわけではないのでフラグを倒す
	DXA->MemoryImageCopyFlag			= FALSE ;			// メモリイメージをコピーして使用しているフラグを倒す
	DXA->MemoryImageReadOnlyFlag        = FALSE ;			// メモリイメージを読み取り専用にするかどうかのフラグを倒す

	// 終了
	return 0 ;

ERR :
	if( DXA->WinFilePointer__ != 0 )
	{
		ReadOnlyFileAccessClose( DXA->WinFilePointer__ ) ;
		DXA->WinFilePointer__ = 0 ;
	}

	if( DXA->Table.Top != NULL )
	{
		DXFREE( DXA->Table.Top ) ;
		DXA->Table.Top = NULL ;
	}
	
	// 終了
	return -1 ;
}

// アーカイブファイルを閉じる
extern int DXA_CloseArchive( DXARC *DXA )
{
	// 既に閉じていたら何もせず終了
	if( DXA->WinFilePointer__ == 0 && DXA->MemoryImage == NULL )
	{
		return 0 ;
	}

	// 非同期同期オープン中の場合はここで開き終わるのを待つ
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// メモリから開いているかどうかで処理を分岐
	if( DXA->MemoryOpenFlag == TRUE )
	{
		// アーカイブプログラムがメモリに読み込んだ場合とそうでない場合で処理を分岐
		if( DXA->UserMemoryImageFlag == TRUE )
		{
			// ユーザーから渡されたデータだった場合

			// アーカイブイメージをコピーして使用していた場合は単純に解放する
			if( DXA->MemoryImageCopyFlag )
			{
				if( DXA->MemoryImage != NULL )
				{
					DXFREE( DXA->MemoryImage ) ;
					DXA->MemoryImage = NULL ;
				}
			}
			else
			{
				if( DXA->MemoryImageReadOnlyFlag )
				{
					// 読み取り専用の場合は情報テーブル用に確保していたメモリを解放
					if( DXA->Table.Top != NULL )
					{
						DXFREE( DXA->Table.Top ) ;
						DXA->Table.Top = NULL ;
					}
				}
				else
				{
					// 渡されたメモリアドレス先の内容を直接使用していた場合は
					// 反転したデータを元に戻す
					if( DXA->V5Flag == FALSE )
					{
						char KeyV2String[ DXA_KEYV2_STRING_MAXLENGTH ] ;
						DXA_DirectoryKeyConv( DXA, ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable, KeyV2String ) ;

						if( DXA->Head.Version >= DXA_KEYV2_VER )
						{
							DXA_KeyV2Conv( DXA->Table.Top, DXA->Head.HeadSize, 0, DXA->KeyV2 ) ;
						}
						else
						{
							DXA_KeyConv( DXA->Table.Top, DXA->Head.HeadSize, 0, DXA->Key ) ;
						}
					}
					else
					{
						if( DXA->HeadV5.Version >= 0x0005 )
						{
							DXA_DirectoryKeyConvV5( DXA, ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ) ;
							DXA_KeyConv( DXA->Table.Top, DXA->HeadV5.HeadSize, 0, DXA->Key ) ;
						}
						else
						{
							DXA_KeyConv( DXA->MemoryImage, DXA->MemoryImageSize, 0, DXA->Key ) ;
						}
					}
				}
			}
		}
		else
		{
			// アーカイブプログラムがメモリに読み込んだ場合

			// 確保していたメモリを開放する
			DXFREE( DXA->Table.Top ) ;
			DXFREE( DXA->MemoryImage ) ;
		}
	}
	else
	{
		// アーカイブファイルを閉じる
		ReadOnlyFileAccessClose( DXA->WinFilePointer__ ) ;

		// 情報テーブルを格納していたメモリ領域も解放
		DXFREE( DXA->Table.Top ) ;
	}

	// 初期化
	_MEMSET( DXA, 0, sizeof( DXARC ) ) ;

	// 終了
	return 0 ;
}

// アーカイブ内のディレクトリパスを変更する( 0:成功  -1:失敗 )
static int DXA_ChangeCurrentDirectoryFast( DXARC *DXA, DXARC_SEARCHDATA *SearchData )
{
	int i, j, k, Num ;
	BYTE *NameData, *PathData ;
	WORD PackNum, Parity ;
	DWORD FileHeadSize ;

	// 非同期同期オープン中の場合はここで開き終わるのを待つ
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	PackNum  = SearchData->PackNum ;
	Parity   = SearchData->Parity ;
	PathData = SearchData->FileName ;

	// カレントディレクトリから同名のディレクトリを探す
	if( DXA->V5Flag )
	{
		DXARC_FILEHEAD_VER5 *FileH ;

		FileH        = ( DXARC_FILEHEAD_VER5 * )( DXA->Table.FileTable + DXA->CurrentDirectoryV5->FileHeadAddress ) ;
		Num          = ( int )DXA->CurrentDirectoryV5->FileHeadNum ;
		FileHeadSize = ( DWORD )( DXA->HeadV5.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ) ;
		for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD_VER5 *)( (BYTE *)FileH + FileHeadSize ) )
		{
			// ディレクトリチェック
			if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
			{
				continue ;
			}

			// 文字列数とパリティチェック
			NameData = DXA->Table.NameTable + FileH->NameAddress ;
			if( PackNum != ( ( WORD * )NameData )[ 0 ] ||
				Parity  != ( ( WORD * )NameData )[ 1 ] )
			{
				continue ;
			}

			// 文字列チェック
			NameData += 4 ;
			for( j = 0, k = 0 ; j < PackNum ; j ++, k += 4 )
			{
				if( *( ( DWORD * )&PathData[ k ] ) != *( ( DWORD * )&NameData[ k ] ) )
				{
					break ;
				}
			}

			// 適合したディレクトリがあったらここで終了
			if( PackNum == j )
			{
				break ;
			}
		}

		// 無かったらエラー
		if( i == Num )
		{
			return -1 ;
		}

		// 在ったらカレントディレクトリを変更
		DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )( DXA->Table.DirectoryTable + FileH->DataAddress ) ;
	}
	else
	{
		DXARC_FILEHEAD *FileH ;

		FileH        = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + DXA->CurrentDirectory->FileHeadAddress ) ;
		Num          = ( int )DXA->CurrentDirectory->FileHeadNum ;
		FileHeadSize = DXARC_FILEHEAD_VER6_SIZE ;
		for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD *)( (BYTE *)FileH + FileHeadSize ) )
		{
			// ディレクトリチェック
			if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
			{
				continue ;
			}

			// 文字列数とパリティチェック
			NameData = DXA->Table.NameTable + FileH->NameAddress ;
			if( PackNum != ( ( WORD * )NameData )[ 0 ] ||
				Parity  != ( ( WORD * )NameData )[ 1 ] )
			{
				continue ;
			}

			// 文字列チェック
			NameData += 4 ;
			for( j = 0, k = 0 ; j < PackNum ; j ++, k += 4 )
			{
				if( *( ( DWORD * )&PathData[ k ] ) != *( ( DWORD * )&NameData[ k ] ) )
				{
					break ;
				}
			}

			// 適合したディレクトリがあったらここで終了
			if( PackNum == j )
			{
				break ;
			}
		}

		// 無かったらエラー
		if( i == Num )
		{
			return -1 ;
		}

		// 在ったらカレントディレクトリを変更
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + FileH->DataAddress ) ;
	}

	// 正常終了
	return 0 ;
}

// アーカイブ内のディレクトリパスを変更する( 0:成功  -1:失敗 )
extern int DXA_ChangeCurrentDir( DXARC *DXA, int CharCodeFormat, const char *DirPath )
{
	BYTE TempBuffer[ 4096 ] ;
	const BYTE *DirPathB ;

	// 文字コード形式が異なる場合は変換する
	if( CharCodeFormat != DXA->CharCodeFormat )
	{
		ConvString( DirPath, -1, CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), DXA->CharCodeFormat ) ;
		DirPathB = TempBuffer ;
	}
	else
	{
		DirPathB = ( const BYTE * )DirPath ;
	}

	return DXA_ChangeCurrentDirectoryBase( DXA, DirPathB, true ) ;
}

// アーカイブ内のディレクトリパスを変更する( 0:成功  -1:失敗 )
static int DXA_ChangeCurrentDirectoryBase( DXARC *DXA, const BYTE *DirectoryPath, bool ErrorIsDirectoryReset, DXARC_SEARCHDATA *LastSearchData )
{
	DXARC_DIRECTORY *OldDir ;
	DXARC_SEARCHDATA SearchData ;

	// 非同期同期オープン中の場合はここで開き終わるのを待つ
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// ここに留まるパスだったら無視
	if( CL_strcmp_str2_ascii( DXA->CharCodeFormat, ( const char * )DirectoryPath, ( const char * )Ascii_DotStr ) == 0 )
	{
		return 0 ;
	}

	// 『\ or /』だけの場合はルートディレクトリに戻る
	if( CL_strcmp_str2_ascii( DXA->CharCodeFormat, ( const char * )DirectoryPath, ( const char * )Ascii_EnStr    ) == 0 ||
		CL_strcmp_str2_ascii( DXA->CharCodeFormat, ( const char * )DirectoryPath, ( const char * )Ascii_SlashStr ) == 0 )
	{
		if( DXA->V5Flag )
		{
			DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ;
		}
		else
		{
			DXA->CurrentDirectory   = ( DXARC_DIRECTORY      * )DXA->Table.DirectoryTable ;
		}
		return 0 ;
	}

	// 下に一つ下がるパスだったら処理を分岐
	if( CL_strcmp_str2_ascii( DXA->CharCodeFormat, ( const char * )DirectoryPath, ( const char * )Ascii_DoubleDotStr ) == 0 )
	{
		if( DXA->V5Flag )
		{
			// ルートディレクトリに居たらエラー
			if( DXA->CurrentDirectoryV5->ParentDirectoryAddress == 0xffffffff ) return -1 ;
			
			// 親ディレクトリがあったらそちらに移る
			DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )( DXA->Table.DirectoryTable + DXA->CurrentDirectoryV5->ParentDirectoryAddress ) ;
		}
		else
		{
			// ルートディレクトリに居たらエラー
			if( DXA->CurrentDirectory->ParentDirectoryAddress == NONE_PAL ) return -1 ;
			
			// 親ディレクトリがあったらそちらに移る
			DXA->CurrentDirectory   = ( DXARC_DIRECTORY      * )( DXA->Table.DirectoryTable + DXA->CurrentDirectory->ParentDirectoryAddress ) ;
		}
		return 0 ;
	}

	// それ以外の場合は指定の名前のディレクトリを探す
	
	// 変更以前のディレクトリを保存しておく
	OldDir = DXA->CurrentDirectory ;

	// パス中に『\』があるかどうかで処理を分岐
	if( CL_strchr( DXA->CharCodeFormat, ( const char * )DirectoryPath, '\\' ) == NULL &&
		CL_strchr( DXA->CharCodeFormat, ( const char * )DirectoryPath, '/'  ) == NULL )
	{
		// ファイル名を検索専用の形式に変換する
		DXA_ConvSearchData( DXA, &SearchData, DirectoryPath, NULL ) ;

		// ディレクトリを変更
		if( DXA_ChangeCurrentDirectoryFast( DXA, &SearchData ) < 0 ) goto ERR ;
	}
	else
	{
		// \ or / がある場合は繋がったディレクトリを一つづつ変更してゆく
		int i ;
		int StrLength ;
		int CharBytes ;
		int CharBytes2 ;
		DWORD CharCode ;
		DWORD CharCode2 ;

		i = 0 ;

		// ループ
		for(;;)
		{
			// 文字列を取得する
			DXA_ConvSearchData( DXA, &SearchData, &DirectoryPath[ i ], &StrLength ) ;
			i += StrLength ;

			// もし初っ端が \ or / だった場合はルートディレクトリに落とす
			CharCode = GetCharCode( ( const char * )&DirectoryPath[ i ], DXA->CharCodeFormat, &CharBytes ) ;
			if( StrLength == 0 && ( CharCode == '\\' || CharCode == '/' ) )
			{
				BYTE EnStr[ 16 ] ;

				ConvString( ( const char * )Ascii_EnStr, -1, DX_CHARCODEFORMAT_ASCII, ( char * )EnStr, sizeof( EnStr ), DXA->CharCodeFormat ) ;
				DXA_ChangeCurrentDirectoryBase( DXA, EnStr, false ) ;
			}
			else
			{
				// それ以外の場合は普通にディレクトリ変更
				if( DXA_ChangeCurrentDirectoryFast( DXA, &SearchData ) < 0 )
				{
					// エラーが起きて、更にエラーが起きた時に元のディレクトリに戻せの
					// フラグが立っている場合は元のディレクトリに戻す
					if( ErrorIsDirectoryReset == true )
					{
						DXA->CurrentDirectory = OldDir ;
					}

					// エラー終了
					goto ERR ;
				}
			}

			// もし終端文字で終了した場合はループから抜ける
			// 又はあと \ or / しかない場合もループから抜ける
			if( CharCode == '\0' )
			{
				break ;
			}
			else
			{
				CharCode2 = GetCharCode( ( const char * )&DirectoryPath[ i + CharBytes ], DXA->CharCodeFormat, &CharBytes2 ) ;
				if( ( CharCode == '\\' && CharCode2 == '\0' ) ||
					( CharCode == '/'  && CharCode2 == '\0' ) )
				{
					break ;
				}
			}
			i += CharBytes ;
		}
	}

	if( LastSearchData != NULL )
	{
		_MEMCPY( LastSearchData->FileName, SearchData.FileName, ( size_t )( SearchData.PackNum * 4 ) ) ;
		LastSearchData->Parity  = SearchData.Parity ;
		LastSearchData->PackNum = SearchData.PackNum ;
	}

	// 正常終了
	return 0 ;

ERR:
	if( LastSearchData != NULL )
	{
		_MEMCPY( LastSearchData->FileName, SearchData.FileName, ( size_t )( SearchData.PackNum * 4 ) ) ;
		LastSearchData->Parity  = SearchData.Parity ;
		LastSearchData->PackNum = SearchData.PackNum ;
	}

	// エラー終了
	return -1 ;
}

// カレントディレクトリにある指定のファイルの鍵バージョン２用の文字列を作成する、戻り値は文字列の長さ( 単位：Byte )( FileString は DXA_KEYV2_STRING_MAXLENGTH の長さが必要 )
static size_t DXA_CreateKeyV2FileString( DXARC *DXA, DXARC_DIRECTORY *Directory, DXARC_FILEHEAD *FileHead, BYTE *FileString )
{
	size_t StartAddr ;

	// 最初にパスワードの文字列をセット
	if( DXA->KeyV2StringBytes != 0 )
	{
		_MEMCPY( FileString, DXA->KeyV2String, DXA->KeyV2StringBytes ) ;
		FileString[ DXA->KeyV2StringBytes ] = '\0' ;
		StartAddr = DXA->KeyV2StringBytes ;
	}
	else
	{
		FileString[ 0 ] = '\0' ;
		StartAddr = 0 ;
	}

	// 次にファイル名の文字列をセット
	CL_strcat_s( DXA->CharCodeFormat, ( char * )&FileString[ StartAddr ], DXA_KEYV2_STRING_MAXLENGTH - StartAddr, ( char * )( DXA->Table.NameTable + FileHead->NameAddress + 4 ) ) ;

	// その後にディレクトリの文字列をセット
	if( Directory->ParentDirectoryAddress != NONE_PAL )
	{
		do
		{
			CL_strcat_s( DXA->CharCodeFormat, ( char * )&FileString[ StartAddr ], DXA_KEYV2_STRING_MAXLENGTH - StartAddr, ( char * )( DXA->Table.NameTable + ( ( DXARC_FILEHEAD * )( DXA->Table.FileTable + Directory->DirectoryAddress ) )->NameAddress + 4 ) ) ;
			Directory = ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + Directory->ParentDirectoryAddress ) ;
		}while( Directory->ParentDirectoryAddress != NONE_PAL ) ;
	}

	return StartAddr + CL_strlen( DXA->CharCodeFormat, ( char * )&FileString[ StartAddr ] ) * GetCharCodeFormatUnitSize( DXA->CharCodeFormat ) ;
}

// アーカイブ内のカレントディレクトリパスを取得する
//extern int DXA_GetCurrentDir( DXARC *DXA, char *DirPathBuffer, int BufferSize )
//{
//	char DirPath[FILEPATH_MAX] ;
//	int Depth, i ;
//
//	// 非同期同期オープン中の場合はここで開き終わるのを待つ
//	if( DXA->ASyncOpenFlag == TRUE )
//	{
//		while( DXA_CheckIdle( DXA ) == FALSE )
//		{
//			Thread_Sleep( 0 ) ;
//		}
//	}
//
//	if( DXA->V5Flag )
//	{
//		DXARC_DIRECTORY_VER5 *Dir[200], *DirTempP ;
//
//		// ルートディレクトリに着くまで検索する
//		Depth = 0 ;
//		DirTempP = DXA->CurrentDirectoryV5 ;
//		while( DirTempP->DirectoryAddress != 0xffffffff && DirTempP->DirectoryAddress != 0 )
//		{
//			Dir[Depth] = DirTempP ;
//			DirTempP = ( DXARC_DIRECTORY_VER5 * )( DXA->Table.DirectoryTable + DirTempP->ParentDirectoryAddress ) ;
//			Depth ++ ;
//		}
//		
//		// パス名を連結する
//		DirPath[0] = '\0' ;
//		for( i = Depth - 1 ; i >= 0 ; i -- )
//		{
//			_STRCAT( DirPath, "\\" ) ;
//			_STRCAT( DirPath, (char *)DXA->Table.NameTable + ((DXARC_FILEHEAD_VER5 *)( DXA->Table.FileTable + Dir[i]->DirectoryAddress ))->NameAddress ) ;
//		}
//	}
//	else
//	{
//		DXARC_DIRECTORY *Dir[200], *DirTempP ;
//
//		// ルートディレクトリに着くまで検索する
//		Depth = 0 ;
//		DirTempP = DXA->CurrentDirectory ;
//		while( DirTempP->DirectoryAddress != 0xffffffff && DirTempP->DirectoryAddress != 0 )
//		{
//			Dir[Depth] = DirTempP ;
//			DirTempP = ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + DirTempP->ParentDirectoryAddress ) ;
//			Depth ++ ;
//		}
//		
//		// パス名を連結する
//		DirPath[0] = '\0' ;
//		for( i = Depth - 1 ; i >= 0 ; i -- )
//		{
//			_STRCAT( DirPath, "\\" ) ;
//			_STRCAT( DirPath, (char *)DXA->Table.NameTable + ((DXARC_FILEHEAD *)( DXA->Table.FileTable + Dir[i]->DirectoryAddress ))->NameAddress ) ;
//		}
//	}
//
//	// バッファの長さが０か、長さが足りないときはディレクトリ名の長さを返す
//	if( BufferSize == 0 || BufferSize < (int)_STRLEN( DirPath ) )
//	{
//		return _STRLEN( DirPath ) + 1 ;
//	}
//	else
//	{
//		// ディレクトリ名をバッファに転送する
//		_STRCPY( DirPathBuffer, DirPath ) ;
//	}
//
//	// 終了
//	return 0 ;
//}

// 戻り値: -1=エラー  0=ＤＸアーカイブファイル内のファイルではない  1=ＤＸアーカイブファイル内のファイル
extern int DXA_DIR_IsDXA(DWORD_PTR Handle)
{
	DXARC_DIR_FILE* file = DXARCD.File[Handle & 0x0FFFFFFF];
	if (file == NULL)
	{
		return -1;
	}

	return file->UseArchiveFlag;
}

// アーカイブ内のオブジェクトを検索する( -1:エラー -1以外:DXA検索ハンドル )
extern DWORD_PTR DXA_FindFirst( DXARC *DXA, const BYTE *FilePath, FILEINFOW *Buffer )
{
	DXA_FINDDATA *find ;
	BYTE Dir[  FILEPATH_MAX ] ;
	BYTE Name[ FILEPATH_MAX ] ;
	int CharBytes ;

	// 非同期同期オープン中の場合はここで開き終わるのを待つ
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// メモリの確保
	find = ( DXA_FINDDATA * )DXALLOC( sizeof( *find ) ) ;
	if( find == NULL )
	{
		return ( DWORD_PTR )-1 ;
	}

	find->Container = DXA;
	DXA_DIR_AnalysisFileNameAndDirPath( DXA, FilePath, Name, sizeof( Name ), Dir, sizeof( Dir ) );

	// 全て大文字にする
	CL_strupr( DXA->CharCodeFormat, ( char * )Dir  ) ;
	CL_strupr( DXA->CharCodeFormat, ( char * )Name ) ;

	// 検索対象のディレクトリを取得
	if( GetCharCode( ( const char * )Dir, DXA->CharCodeFormat, &CharBytes ) == '\0' )
	{
		find->Directory = DXA->CurrentDirectory ;
	}
	else
	{
		DXARC_DIRECTORY *OldDir;

		OldDir = DXA->CurrentDirectory;

		// 指定のディレクトリが無い場合はエラー
		if( DXA_ChangeCurrentDirectoryBase( DXA, Dir, false ) == -1 )
		{
			DXFREE( find ) ;
			DXA->CurrentDirectory = OldDir ;
			return ( DWORD_PTR )-1 ;
		}

		find->Directory       = DXA->CurrentDirectory ;
		DXA->CurrentDirectory = OldDir ;
	}

	find->ObjectCount = 0;
	CL_strcpy_s( DXA->CharCodeFormat, ( char * )find->SearchStr, sizeof( find->SearchStr ), ( const char * )Name ) ;

	// 適合する最初のファイルを検索する
	if( DXA_FindProcess( find, Buffer ) == -1 )
	{
		DXFREE( find );
		return ( DWORD_PTR )-1 ;
	}
	find->ObjectCount ++ ;

	// ハンドルを返す
	return ( DWORD_PTR )find ;
}

// アーカイブ内のオブジェクトを検索する( -1:エラー 0:成功 )
extern int DXA_FindNext( DWORD_PTR DxaFindHandle, FILEINFOW *Buffer )
{
	DXA_FINDDATA *find;

	find = ( DXA_FINDDATA * )DxaFindHandle ;
	if( DXA_FindProcess( find, Buffer ) == -1 )
	{
		return -1 ;
	}
	find->ObjectCount ++ ;

	return 0;
}

// アーカイブ内のオブジェクト検索を終了する
extern int DXA_FindClose( DWORD_PTR DxaFindHandle )
{
	DXA_FINDDATA *find ;

	find = ( DXA_FINDDATA * )DxaFindHandle ;
	DXFREE( find ) ;

	return 0 ;
}

// アーカイブファイル中の指定のファイルをメモリに読み込む( -1:エラー 0以上:ファイルサイズ )
//extern int DXA_LoadFile( DXARC *DXA, const char *FilePath, void *Buffer, ULONGLONG BufferSize )
//{
//	// 非同期同期オープン中の場合はここで開き終わるのを待つ
//	if( DXA->ASyncOpenFlag == TRUE )
//	{
//		while( DXA_CheckIdle( DXA ) == FALSE )
//		{
//			Thread_Sleep( 0 ) ;
//		}
//	}
//
//	if( DXA->V5Flag )
//	{
//		DXARC_FILEHEAD_VER5 *FileH ;
//
//		// 指定のファイルの情報を得る
//		FileH = DXA_GetFileHeaderV5( DXA, FilePath ) ;
//		if( FileH == NULL ) return -1 ;
//
//		// ファイルサイズが足りているか調べる、足りていないか、バッファ、又はサイズが０だったらサイズを返す
//		if( BufferSize < FileH->DataSize || BufferSize == 0 || Buffer == NULL )
//		{
//			return ( int )FileH->DataSize ;
//		}
//		
//		// 足りている場合はバッファーに読み込む
//
//		// ファイルが圧縮されているかどうかで処理を分岐
//		if( DXA->HeadV5.Version >= 0x0002 && FileH->PressDataSize != 0xffffffff )
//		{
//			// 圧縮されている場合
//
//			// メモリ上に読み込んでいるかどうかで処理を分岐
//			if( DXA->MemoryOpenFlag == TRUE )
//			{
//				if( DXA->MemoryImageReadOnlyFlag )
//				{
//					void *temp ;
//
//					// 圧縮データをメモリに読み込んでから解凍する
//
//					// 圧縮データが収まるメモリ領域の確保
//					temp = DXALLOC( FileH->PressDataSize ) ;
//
//					// 圧縮データの転送
//					_MEMCPY( temp, (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, FileH->DataSize ) ;
//					if( DXA->HeadV5.Version >= 0x0005 )
//					{
//						DXA_KeyConv( temp, FileH->PressDataSize,                                   FileH->DataSize, DXA->Key ) ;
//					}
//					else
//					{
//						DXA_KeyConv( temp, FileH->PressDataSize, DXA->HeadV5.DataStartAddress + FileH->DataAddress, DXA->Key ) ;
//					}
//					
//					// 解凍
//					DXA_Decode( temp, Buffer ) ;
//					
//					// メモリの解放
//					DXFREE( temp ) ;
//				}
//				else
//				{
//					// メモリ上の圧縮データを解凍する
//					DXA_Decode( (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, Buffer ) ;
//				}
//			}
//			else
//			{
//				void *temp ;
//
//				// 圧縮データをメモリに読み込んでから解凍する
//
//				// 圧縮データが収まるメモリ領域の確保
//				temp = DXALLOC( FileH->PressDataSize ) ;
//
//				// 圧縮データの読み込み
//				ReadOnlyFileAccessSeek( DXA->WinFilePointer__, DXA->HeadV5.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;
//				if( DXA->HeadV5.Version >= 0x0005 )
//				{
//					DXA_KeyConvFileRead( temp, FileH->PressDataSize, DXA->WinFilePointer__, DXA->Key, FileH->DataSize ) ;
//				}
//				else
//				{
//					DXA_KeyConvFileRead( temp, FileH->PressDataSize, DXA->WinFilePointer__, DXA->Key ) ;
//				}
//				
//				// 解凍
//				DXA_Decode( temp, Buffer ) ;
//				
//				// メモリの解放
//				DXFREE( temp ) ;
//			}
//		}
//		else
//		{
//			if( DXA->MemoryOpenFlag == TRUE )
//			{
//				if( DXA->MemoryImageReadOnlyFlag )
//				{
//					// ファイルポインタを移動
//					_MEMCPY( Buffer, (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, FileH->DataSize ) ;
//
//					// 読み込み
//					if( DXA->HeadV5.Version >= 0x0005 )
//					{
//						DXA_KeyConv( Buffer, FileH->DataSize,                                   FileH->DataSize, DXA->Key ) ;
//					}
//					else
//					{
//						DXA_KeyConv( Buffer, FileH->DataSize, DXA->HeadV5.DataStartAddress + FileH->DataAddress, DXA->Key ) ;
//					}
//				}
//				else
//				{
//					// コピー
//					_MEMCPY( Buffer, (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, FileH->DataSize ) ;
//				}
//			}
//			else
//			{
//				// ファイルポインタを移動
//				ReadOnlyFileAccessSeek( DXA->WinFilePointer__, DXA->HeadV5.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;
//
//				// 読み込み
//				if( DXA->HeadV5.Version >= 0x0005 )
//				{
//					DXA_KeyConvFileRead( Buffer, FileH->DataSize, DXA->WinFilePointer__, DXA->Key, FileH->DataSize ) ;
//				}
//				else
//				{
//					DXA_KeyConvFileRead( Buffer, FileH->DataSize, DXA->WinFilePointer__, DXA->Key ) ;
//				}
//			}
//		}
//	}
//	else
//	{
//		DXARC_FILEHEAD *FileH ;
//
//		// 指定のファイルの情報を得る
//		FileH = DXA_GetFileHeader( DXA, FilePath ) ;
//		if( FileH == NULL ) return -1 ;
//
//		// ファイルサイズが足りているか調べる、足りていないか、バッファ、又はサイズが０だったらサイズを返す
//		if( BufferSize < FileH->DataSize || BufferSize == 0 || Buffer == NULL )
//		{
//			return ( int )FileH->DataSize ;
//		}
//		
//		// 足りている場合はバッファーに読み込む
//
//		// ファイルが圧縮されているかどうかで処理を分岐
//		if( FileH->PressDataSize != NONE_PAL )
//		{
//			// 圧縮されている場合
//
//			// メモリ上に読み込んでいるかどうかで処理を分岐
//			if( DXA->MemoryOpenFlag == TRUE )
//			{
//				if( DXA->MemoryImageReadOnlyFlag )
//				{
//					void *temp ;
//
//					// 圧縮データをメモリに読み込んでから解凍する
//
//					// 圧縮データが収まるメモリ領域の確保
//					temp = DXALLOC( ( size_t )FileH->PressDataSize ) ;
//
//					// 圧縮データの読み込み
//					_MEMCPY( temp, (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, ( size_t )FileH->DataSize ) ;
//					DXA_KeyConv( temp, ( LONGLONG )FileH->DataSize, ( LONGLONG )FileH->DataSize, DXA->Key ) ;
//					
//					// 解凍
//					DXA_Decode( temp, Buffer ) ;
//					
//					// メモリの解放
//					DXFREE( temp ) ;
//				}
//				else
//				{
//					// メモリ上の圧縮データを解凍する
//					DXA_Decode( (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, Buffer ) ;
//				}
//			}
//			else
//			{
//				void *temp ;
//
//				// 圧縮データをメモリに読み込んでから解凍する
//
//				// 圧縮データが収まるメモリ領域の確保
//				temp = DXALLOC( ( size_t )FileH->PressDataSize ) ;
//
//				// 圧縮データの読み込み
//				ReadOnlyFileAccessSeek( DXA->WinFilePointer__, ( LONGLONG )( DXA->Head.DataStartAddress + FileH->DataAddress ), SEEK_SET ) ;
//				DXA_KeyConvFileRead( temp, FileH->PressDataSize, DXA->WinFilePointer__, DXA->Key, ( LONGLONG )FileH->DataSize ) ;
//				
//				// 解凍
//				DXA_Decode( temp, Buffer ) ;
//				
//				// メモリの解放
//				DXFREE( temp ) ;
//			}
//		}
//		else
//		{
//			if( DXA->MemoryOpenFlag == TRUE )
//			{
//				if( DXA->MemoryImageReadOnlyFlag )
//				{
//					// コピー
//					_MEMCPY( Buffer, (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, ( size_t )FileH->DataSize ) ;
//
//					DXA_KeyConv( Buffer, ( LONGLONG )FileH->DataSize, ( LONGLONG )FileH->DataSize, DXA->Key ) ;
//				}
//				else
//				{
//					// コピー
//					_MEMCPY( Buffer, (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, ( size_t )FileH->DataSize ) ;
//				}
//			}
//			else
//			{
//				// ファイルポインタを移動
//				ReadOnlyFileAccessSeek( DXA->WinFilePointer__, ( LONGLONG )( DXA->Head.DataStartAddress + FileH->DataAddress ), SEEK_SET ) ;
//
//				// 読み込み
//				DXA_KeyConvFileRead( Buffer, FileH->DataSize, DXA->WinFilePointer__, DXA->Key, ( LONGLONG )FileH->DataSize ) ;
//			}
//		}
//	}
//	
//	// 終了
//	return 0 ;
//}

// アーカイブファイルをメモリに読み込んだ場合のファイルイメージが格納されている先頭アドレスを取得する( DXA_OpenArchiveFromFileUseMem 若しくは DXA_OpenArchiveFromMem で開いた場合に有効 )
extern void *DXA_GetFileImage( DXARC *DXA )
{
	// 非同期同期オープン中の場合はここで開き終わるのを待つ
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// メモリイメージから開いていなかったらエラー
	if( DXA->MemoryOpenFlag == FALSE )
	{
		return NULL ;
	}

	// 先頭アドレスを返す
	return DXA->MemoryImage ;
}

// アーカイブファイル中の指定のファイルのファイル内の位置とファイルの大きさを得る( -1:エラー )
extern int DXA_GetFileInfo( DXARC *DXA, int CharCodeFormat, const char *FilePath, int *Position, int *Size )
{
	BYTE TempBuffer[ 4096 ] ;
	const BYTE *FilePathB ;

	// 文字コード形式が異なる場合は変換する
	if( CharCodeFormat != DXA->CharCodeFormat )
	{
		ConvString( FilePath, -1, CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), DXA->CharCodeFormat ) ;
		FilePathB = TempBuffer ;
	}
	else
	{
		FilePathB = ( const BYTE * )FilePath ;
	}

	// 非同期同期オープン中の場合はここで開き終わるのを待つ
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	if( DXA->V5Flag )
	{
		DXARC_FILEHEAD_VER5 *FileH ;

		// 指定のファイルの情報を得る
		FileH = DXA_GetFileHeaderV5( DXA, FilePathB ) ;
		if( FileH == NULL )
		{
			return -1 ;
		}

		// ファイルのデータがある位置とファイルサイズを保存する
		if( Position != NULL )
		{
			*Position = ( int )( DXA->HeadV5.DataStartAddress + FileH->DataAddress ) ;
		}

		if( Size     != NULL )
		{
			*Size     = ( int )FileH->DataSize ;
		}
	}
	else
	{
		DXARC_FILEHEAD *FileH ;
		DXARC_DIRECTORY *Directory ;

		// 指定のファイルの情報を得る
		FileH = DXA_GetFileHeader( DXA, FilePathB, &Directory ) ;
		if( FileH == NULL )
		{
			return -1 ;
		}

		// ファイルのデータがある位置とファイルサイズを保存する
		if( Position != NULL )
		{
			*Position = ( int )( DXA->Head.DataStartAddress + FileH->DataAddress ) ;
		}

		if( Size     != NULL )
		{
			*Size     = ( int )( FileH->DataSize ) ;
		}
	}

	// 成功終了
	return 0 ;
}



// アーカイブファイル内のファイルを開く(ファイル閉じる作業は必要なし)
extern int DXA_STREAM_Initialize( DXARC_STREAM *DXAStream, DXARC *DXA, const BYTE *FilePath, int UseASyncReadFlag )
{
	// 非同期同期オープン中の場合はここで開き終わるのを待つ
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// データのセット
	DXAStream->Archive          = DXA ;
	DXAStream->EOFFlag          = FALSE ;
	DXAStream->FilePoint        = 0 ;
	DXAStream->DecodeDataBuffer = NULL ;
	DXAStream->DecodeTempBuffer = NULL ;
	DXAStream->UseASyncReadFlag = UseASyncReadFlag ;
	DXAStream->ASyncState       = DXARC_STREAM_ASYNCSTATE_IDLE ;

	// ファイルから開いている場合はアーカイブファイルのファイルポインタを作成
	if( DXA->MemoryOpenFlag == FALSE )
	{
		DXAStream->WinFilePointer = ReadOnlyFileAccessOpen( DXA->FilePath, FALSE, TRUE, FALSE ) ;
		if( DXAStream->WinFilePointer == 0 )
		{
			return -1 ;
		}
	}

	if( DXA->V5Flag )
	{
		DXARC_FILEHEAD_VER5 *FileH ;

		// 指定のファイルの情報を得る
		FileH = DXA_GetFileHeaderV5( DXA, FilePath ) ;
		if( FileH == NULL )
		{
			if( DXA->MemoryOpenFlag == FALSE )
			{
				ReadOnlyFileAccessClose( DXAStream->WinFilePointer ) ;
				DXAStream->WinFilePointer = 0 ;
			}
			return -1 ;
		}

		// ファイル情報をセット
		DXAStream->FileHeadV5 = FileH ;

		// ファイルが圧縮されている場合はここで読み込んで解凍してしまう
		if( DXA->HeadV5.Version >= 0x0002 && FileH->PressDataSize != 0xffffffff )
		{
			// 解凍データが収まるメモリ領域の確保
			DXAStream->DecodeDataBuffer = DXALLOC( FileH->DataSize ) ;

			// メモリ上に読み込まれているかどうかで処理を分岐
			if( DXA->MemoryOpenFlag == TRUE )
			{
				if( DXA->MemoryImageReadOnlyFlag )
				{
					// 圧縮データが収まるメモリ領域の確保
					DXAStream->DecodeTempBuffer = DXALLOC( FileH->PressDataSize ) ;

					// 圧縮データの読み込み
					_MEMCPY( DXAStream->DecodeTempBuffer, (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, FileH->PressDataSize ) ;
					if( DXA->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConv( DXAStream->DecodeTempBuffer, FileH->PressDataSize,                                   FileH->DataSize, DXA->Key ) ;
					}
					else
					{
						DXA_KeyConv( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXA->HeadV5.DataStartAddress + FileH->DataAddress, DXA->Key ) ;
					}

					// 解凍
					DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
				
					// メモリの解放
					DXFREE( DXAStream->DecodeTempBuffer ) ;
					DXAStream->DecodeTempBuffer = NULL ;
				}
				else
				{
					// 解凍
					DXA_Decode( (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, DXAStream->DecodeDataBuffer ) ;
				}
			}
			else
			{
				// 圧縮データが収まるメモリ領域の確保
				DXAStream->DecodeTempBuffer = DXALLOC( FileH->PressDataSize ) ;

				// 圧縮データの読み込み
				DXAStream->ASyncReadFileAddress = DXA->HeadV5.DataStartAddress + FileH->DataAddress ;
				ReadOnlyFileAccessSeek( DXAStream->WinFilePointer, ( LONGLONG )DXAStream->ASyncReadFileAddress, SEEK_SET ) ;

				// 非同期の場合は読み込みと鍵解除を別々に行う
				if( DXAStream->UseASyncReadFlag == TRUE )
				{
					// ファイルから読み込み
					ReadOnlyFileAccessRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, 1, DXAStream->WinFilePointer ) ;
					DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_PRESSREAD ;
				}
				else
				{
					if( DXA->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConvFileRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXAStream->WinFilePointer, DXA->Key, FileH->DataSize ) ;
					}
					else
					{
						DXA_KeyConvFileRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXAStream->WinFilePointer, DXA->Key ) ;
					}

					// 解凍
					DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
				
					// メモリの解放
					DXFREE( DXAStream->DecodeTempBuffer ) ;
					DXAStream->DecodeTempBuffer = NULL ;
				}
			}
		}
	}
	else
	{
		DXARC_FILEHEAD *FileH ;
		DXARC_DIRECTORY *Directory ;

		// 指定のファイルの情報を得る
		FileH = DXA_GetFileHeader( DXA, FilePath, &Directory ) ;
		if( FileH == NULL )
		{
			if( DXA->MemoryOpenFlag == FALSE )
			{
				ReadOnlyFileAccessClose( DXAStream->WinFilePointer ) ;
				DXAStream->WinFilePointer = 0 ;
			}
			return -1 ;
		}

		// ファイル情報をセット
		DXAStream->FileHead = FileH ;

		// 鍵バージョン２を使うバージョンの場合は鍵バージョン２を作成する
		if( DXA->Head.Version >= DXA_KEYV2_VER )
		{
			char KeyV2String[ DXA_KEYV2_STRING_MAXLENGTH ] ;
			size_t KeyV2StringBytes ;
			KeyV2StringBytes = DXA_CreateKeyV2FileString( DXA, Directory, FileH, ( BYTE * )KeyV2String ) ;
			DXA_KeyV2Create( KeyV2String, DXAStream->KeyV2 , KeyV2StringBytes ) ;
		}

		// ファイルが圧縮されている場合はここで読み込んで解凍してしまう
		if( FileH->PressDataSize != NONE_PAL )
		{
			// 解凍データが収まるメモリ領域の確保
			DXAStream->DecodeDataBuffer = DXALLOC( ( size_t )FileH->DataSize ) ;

			// メモリ上に読み込まれているかどうかで処理を分岐
			if( DXA->MemoryOpenFlag == TRUE )
			{
				if( DXA->MemoryImageReadOnlyFlag )
				{
					// 圧縮データが収まるメモリ領域の確保
					DXAStream->DecodeTempBuffer = DXALLOC( ( size_t )FileH->PressDataSize ) ;

					// 圧縮データの読み込み
					_MEMCPY( DXAStream->DecodeTempBuffer, (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, ( size_t )FileH->PressDataSize ) ;
					if( DXA->Head.Version >= DXA_KEYV2_VER )
					{
						DXA_KeyV2Conv( DXAStream->DecodeTempBuffer, ( LONGLONG )FileH->PressDataSize, ( LONGLONG )FileH->DataSize, DXAStream->KeyV2 ) ;
					}
					else
					{
						DXA_KeyConv( DXAStream->DecodeTempBuffer, ( LONGLONG )FileH->PressDataSize, ( LONGLONG )FileH->DataSize, DXA->Key ) ;
					}

					// 解凍
					DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
				
					// メモリの解放
					DXFREE( DXAStream->DecodeTempBuffer ) ;
					DXAStream->DecodeTempBuffer = NULL ;
				}
				else
				{
					// 解凍
					DXA_Decode( (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, DXAStream->DecodeDataBuffer ) ;
				}
			}
			else
			{
				// 圧縮データが収まるメモリ領域の確保
				DXAStream->DecodeTempBuffer = DXALLOC( ( size_t )FileH->PressDataSize ) ;

				// 圧縮データの読み込み
				DXAStream->ASyncReadFileAddress = DXA->Head.DataStartAddress + FileH->DataAddress;
				ReadOnlyFileAccessSeek( DXAStream->WinFilePointer, ( LONGLONG )DXAStream->ASyncReadFileAddress, SEEK_SET ) ;

				// 非同期の場合は読み込みと鍵解除を別々に行う
				if( DXAStream->UseASyncReadFlag == TRUE )
				{
					// ファイルから読み込み
					ReadOnlyFileAccessRead( DXAStream->DecodeTempBuffer, ( size_t )FileH->PressDataSize, 1, DXAStream->WinFilePointer ) ;
					DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_PRESSREAD ;
				}
				else
				{
					if( DXA->Head.Version >= DXA_KEYV2_VER )
					{
						DXA_KeyV2ConvFileRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXAStream->WinFilePointer, DXAStream->KeyV2, ( LONGLONG )FileH->DataSize ) ;
					}
					else
					{
						DXA_KeyConvFileRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXAStream->WinFilePointer, DXA->Key, ( LONGLONG )FileH->DataSize ) ;
					}

					// 解凍
					DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
				
					// メモリの解放
					DXFREE( DXAStream->DecodeTempBuffer ) ;
					DXAStream->DecodeTempBuffer = NULL ;
				}
			}
		}
	}

	// 終了
	return 0 ;
}

// アーカイブファイル内のファイルを閉じる
extern int DXA_STREAM_Terminate( DXARC_STREAM *DXAStream )
{
	// 非同期読み込みで状態がまだ待機状態ではなかったら待機状態になるまで待つ
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	// メモリの解放
	if( DXAStream->DecodeDataBuffer != NULL )
	{
		DXFREE( DXAStream->DecodeDataBuffer ) ;
		DXAStream->DecodeDataBuffer = NULL ;
	}

	if( DXAStream->DecodeTempBuffer != NULL )
	{
		DXFREE( DXAStream->DecodeTempBuffer ) ;
		DXAStream->DecodeTempBuffer = NULL ;
	}

	// ファイルを閉じる
	if( DXAStream->Archive->MemoryOpenFlag == FALSE )
	{
		ReadOnlyFileAccessClose( DXAStream->WinFilePointer ) ;
		DXAStream->WinFilePointer = 0 ;
	}

	// ゼロ初期化
	_MEMSET( DXAStream, 0, sizeof( DXARC_STREAM ) ) ;

	// 終了
	return 0 ;
}

// ファイルの内容を読み込む
extern int DXA_STREAM_Read( DXARC_STREAM *DXAStream, void *Buffer, size_t ReadLength )
{
	size_t ReadSize ;
	ULONGLONG DataSize ;
	ULONGLONG DataStartAddress ;
	ULONGLONG DataAddress ;

	// 非同期読み込みで状態がまだ待機状態ではなかったら待機状態になるまで待つ
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	if( DXAStream->Archive->V5Flag )
	{
		DataSize = DXAStream->FileHeadV5->DataSize ;
		DataAddress = DXAStream->FileHeadV5->DataAddress ;
		DataStartAddress = DXAStream->Archive->HeadV5.DataStartAddress ;
	}
	else
	{
		DataSize = DXAStream->FileHead->DataSize ;
		DataAddress = DXAStream->FileHead->DataAddress ;
		DataStartAddress = DXAStream->Archive->Head.DataStartAddress ;
	}

	// EOF フラグが立っていたら０を返す
	if( DXAStream->EOFFlag == TRUE )
	{
		return 0 ;
	}

	// EOF 検出
	if( DataSize == DXAStream->FilePoint )
	{
		DXAStream->EOFFlag = TRUE ;
		return 0 ;
	}

	// データを読み込む量を設定する
	ReadSize = ReadLength < DataSize - DXAStream->FilePoint ? ReadLength : ( size_t )( DataSize - DXAStream->FilePoint ) ;

	// データが圧縮されていたかどうかで処理を分岐
	if( DXAStream->DecodeDataBuffer != NULL )
	{
		// データをコピーする
		_MEMCPY( Buffer, (BYTE *)DXAStream->DecodeDataBuffer + DXAStream->FilePoint, ReadSize ) ;
	}
	else
	{
		// メモリ上にデータがあるかどうかで処理を分岐
		if( DXAStream->Archive->MemoryOpenFlag == TRUE )
		{
			// メモリ上にある場合

			// データをコピーする
			_MEMCPY( Buffer, (BYTE *)DXAStream->Archive->MemoryImage + DataStartAddress + DataAddress + DXAStream->FilePoint, ReadSize ) ;

			if( DXAStream->Archive->MemoryImageReadOnlyFlag )
			{
				if( DXAStream->Archive->V5Flag )
				{
					if( DXAStream->Archive->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConv( Buffer, ( LONGLONG )ReadSize,                       ( LONGLONG )( DataSize + DXAStream->FilePoint ), DXAStream->Archive->Key ) ;
					}
					else
					{
						DXA_KeyConv( Buffer, ( LONGLONG )ReadSize, ( LONGLONG )( DataStartAddress + DataAddress + DXAStream->FilePoint ), DXAStream->Archive->Key ) ;
					}
				}
				else
				{
					if( DXAStream->Archive->Head.Version >= DXA_KEYV2_VER )
					{
						DXA_KeyV2Conv( Buffer, ( LONGLONG )ReadSize,                       ( LONGLONG )( DataSize + DXAStream->FilePoint ), DXAStream->KeyV2 ) ;
					}
					else
					{
						DXA_KeyConv(   Buffer, ( LONGLONG )ReadSize,                       ( LONGLONG )( DataSize + DXAStream->FilePoint ), DXAStream->Archive->Key ) ;
					}
				}
			}
		}
		else
		{
			// ファイルから読み込んでいる場合

			// アーカイブファイルポインタと、仮想ファイルポインタが一致しているか調べる
			// 一致していなかったらアーカイブファイルポインタを移動する
			DXAStream->ASyncReadFileAddress = DataAddress + DataStartAddress + DXAStream->FilePoint ;
			if( ( ULONGLONG )ReadOnlyFileAccessTell( DXAStream->WinFilePointer ) != DXAStream->ASyncReadFileAddress )
			{
				ReadOnlyFileAccessSeek( DXAStream->WinFilePointer, ( LONGLONG )DXAStream->ASyncReadFileAddress, SEEK_SET ) ;
			}

			// 非同期読み込みの場合と同期読み込みの場合で処理を分岐
			if( DXAStream->UseASyncReadFlag )
			{
				// ファイルから読み込み
				ReadOnlyFileAccessRead( Buffer, ReadSize, 1, DXAStream->WinFilePointer ) ;
				DXAStream->ReadBuffer = Buffer;
				DXAStream->ReadSize = ( int )ReadSize;
				DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_READ ;
			}
			else
			{
				// データを読み込む
				if( DXAStream->Archive->V5Flag )
				{
					if( DXAStream->Archive->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConvFileRead( Buffer, ReadSize, DXAStream->WinFilePointer, DXAStream->Archive->Key, ( LONGLONG )( DataSize + DXAStream->FilePoint ) ) ;
					}
					else
					{
						DXA_KeyConvFileRead( Buffer, ReadSize, DXAStream->WinFilePointer, DXAStream->Archive->Key ) ;
					}
				}
				else
				{
					if( DXAStream->Archive->Head.Version >= DXA_KEYV2_VER )
					{
						DXA_KeyV2ConvFileRead( Buffer, ReadSize, DXAStream->WinFilePointer, DXAStream->KeyV2,        ( LONGLONG )( DataSize + DXAStream->FilePoint ) ) ;
					}
					else
					{
						DXA_KeyConvFileRead(   Buffer, ReadSize, DXAStream->WinFilePointer, DXAStream->Archive->Key, ( LONGLONG )( DataSize + DXAStream->FilePoint ) ) ;
					}
				}
			}
		}
	}
	
	// EOF フラグを倒す
	DXAStream->EOFFlag = FALSE ;

	// 読み込んだ分だけファイルポインタを移動する
	DXAStream->FilePoint += ( int )ReadSize ;
	
	// 読み込んだ容量を返す
	return ( int )ReadSize ;
}
	
// ファイルポインタを変更する
extern	int DXA_STREAM_Seek( DXARC_STREAM *DXAStream, LONGLONG SeekPoint, int SeekMode )
{
	ULONGLONG DataSize ;

	// 非同期読み込みで状態がまだ待機状態ではなかったら待機状態になるまで待つ
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	if( DXAStream->Archive->V5Flag )
	{
		DataSize = DXAStream->FileHeadV5->DataSize ;
	}
	else
	{
		DataSize = DXAStream->FileHead->DataSize ;
	}

	// シークタイプによって処理を分岐
	switch( SeekMode )
	{
	case SEEK_SET : break ;		
	case SEEK_CUR : SeekPoint += ( LONGLONG )( DXAStream->FilePoint ) ; break ;
	case SEEK_END :	SeekPoint  = ( LONGLONG )( DataSize + SeekPoint ) ; break ;
	}
	
	// 補正
	if( SeekPoint > ( LONGLONG )DataSize ) SeekPoint = ( LONGLONG )DataSize ;
	if( SeekPoint < 0 ) SeekPoint = 0 ;
	
	// セット
	DXAStream->FilePoint = ( ULONGLONG )SeekPoint ;
	
	// EOFフラグを倒す
	DXAStream->EOFFlag = FALSE ;
	
	// 終了
	return 0 ;
}

// 現在のファイルポインタを得る
extern	LONGLONG DXA_STREAM_Tell( DXARC_STREAM *DXAStream )
{
	// 非同期読み込みで状態がまだ待機状態ではなかったら待機状態になるまで待つ
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	return ( LONGLONG )DXAStream->FilePoint ;
}

// ファイルの終端に来ているか、のフラグを得る
extern	int DXA_STREAM_Eof( DXARC_STREAM *DXAStream )
{
	// 非同期読み込みで状態がまだ待機状態ではなかったら待機状態になるまで待つ
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	return DXAStream->EOFFlag ? EOF : 0 ;
}

// 読み込み処理が完了しているかどうかを調べる
extern	int	DXA_STREAM_IdleCheck( DXARC_STREAM *DXAStream )
{
	// 非同期読み込みではない場合は何もせず TRUE を返す
	if( DXAStream->UseASyncReadFlag == FALSE )
	{
		return TRUE ;
	}

	// 状態によって処理を分岐
	switch( DXAStream->ASyncState )
	{
	case DXARC_STREAM_ASYNCSTATE_IDLE:			// 待機状態
		return TRUE;

	case DXARC_STREAM_ASYNCSTATE_PRESSREAD:		// 圧縮データ読み込み待ち

		// 読み込み終了待ち
		if( ReadOnlyFileAccessIdleCheck( DXAStream->WinFilePointer ) == FALSE )
		{
			return FALSE;
		}

		// 読み込み終わったらまず鍵を外す
		if( DXAStream->Archive->V5Flag )
		{
			if( DXAStream->Archive->HeadV5.Version >= 0x0005 )
			{
				DXA_KeyConv( DXAStream->DecodeTempBuffer, ( LONGLONG )DXAStream->FileHeadV5->PressDataSize, ( LONGLONG )DXAStream->FileHeadV5->DataSize, DXAStream->Archive->Key ) ;
			}
			else
			{
				DXA_KeyConv( DXAStream->DecodeTempBuffer, ( LONGLONG )DXAStream->FileHeadV5->PressDataSize, ( LONGLONG )DXAStream->ASyncReadFileAddress, DXAStream->Archive->Key ) ;
			}
		}
		else
		{
			if( DXAStream->Archive->Head.Version >= DXA_KEYV2_VER )
			{
				DXA_KeyV2Conv( DXAStream->DecodeTempBuffer, ( LONGLONG )DXAStream->FileHead->PressDataSize, ( LONGLONG )DXAStream->FileHead->DataSize, DXAStream->KeyV2 ) ;
			}
			else
			{
				DXA_KeyConv( DXAStream->DecodeTempBuffer, ( LONGLONG )DXAStream->FileHead->PressDataSize, ( LONGLONG )DXAStream->FileHead->DataSize, DXAStream->Archive->Key ) ;
			}
		}

		// 解凍
		DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
	
		// メモリの解放
		DXFREE( DXAStream->DecodeTempBuffer ) ;
		DXAStream->DecodeTempBuffer = NULL ;

		// 状態を待機状態にする
		DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_IDLE;
		return TRUE;

	case DXARC_STREAM_ASYNCSTATE_READ:			// 読み込み待ち

		// 読み込み終了待ち
		if( ReadOnlyFileAccessIdleCheck( DXAStream->WinFilePointer ) == FALSE )
		{
			return FALSE;
		}

		// 読み込み終わったら鍵を外す
		if( DXAStream->Archive->V5Flag )
		{
			if( DXAStream->Archive->HeadV5.Version >= 0x0005 )
			{
				DXA_KeyConv( DXAStream->ReadBuffer, ( LONGLONG )DXAStream->ReadSize, ( LONGLONG )( DXAStream->FileHeadV5->DataSize + ( DXAStream->ASyncReadFileAddress - ( DXAStream->FileHeadV5->DataAddress + DXAStream->Archive->HeadV5.DataStartAddress ) ) ), DXAStream->Archive->Key ) ;
			}
			else
			{
				DXA_KeyConv( DXAStream->ReadBuffer, ( LONGLONG )DXAStream->ReadSize, ( LONGLONG )DXAStream->ASyncReadFileAddress, DXAStream->Archive->Key ) ;
			}
		}
		else
		{
			if( DXAStream->Archive->Head.Version >= DXA_KEYV2_VER )
			{
				DXA_KeyV2Conv( DXAStream->ReadBuffer, ( LONGLONG )DXAStream->ReadSize, ( LONGLONG )( DXAStream->FileHead->DataSize + ( DXAStream->ASyncReadFileAddress - ( DXAStream->FileHead->DataAddress + DXAStream->Archive->Head.DataStartAddress ) ) ), DXAStream->KeyV2 ) ;
			}
			else
			{
				DXA_KeyConv( DXAStream->ReadBuffer, ( LONGLONG )DXAStream->ReadSize, ( LONGLONG )( DXAStream->FileHead->DataSize + ( DXAStream->ASyncReadFileAddress - ( DXAStream->FileHead->DataAddress + DXAStream->Archive->Head.DataStartAddress ) ) ), DXAStream->Archive->Key ) ;
			}
		}

		// 状態を待機状態にする
		DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_IDLE;
		return TRUE;
	}

	return TRUE ;
}

// ファイルのサイズを取得する
extern	LONGLONG DXA_STREAM_Size( DXARC_STREAM *DXAStream )
{
	if( DXAStream->Archive->V5Flag )
	{
		return DXAStream->FileHeadV5->DataSize ;
	}
	else
	{
		return ( LONGLONG )DXAStream->FileHead->DataSize ;
	}
}







// フルパスではないパス文字列をフルパスに変換する
static int DXA_DIR_ConvertFullPath( const wchar_t *Src, wchar_t *Dest, size_t BufferBytes, int CharUp )
{
	int Result ;

	Result = ConvertFullPathW_( Src, Dest, BufferBytes, NULL ) ;
	if( CharUp )
	{
		CL_strupr( WCHAR_T_CHARCODEFORMAT, ( char * )Dest ) ;
	}

	return Result ;

#if 0
	int     i, k ;
	wchar_t iden[ FILEPATH_MAX ] ;
	wchar_t CurrentDir[ FILEPATH_MAX ] ;
	DWORD   CharCode1 ;
	int     CharBytes1 ;
	DWORD   CharCode2 ;
	int     CharBytes2 ;
	DWORD   LastCharCode ;
	int     LastCharBytes ;
	char *  LastCharAddress ;
	int     CharNum ;
	size_t	DestBytes ;

	// カレントディレクトリを得る
	_WGETCWD( CurrentDir, sizeof( CurrentDir ) ) ;
	_WCSUPR( CurrentDir ) ;
	if( Src == NULL )
	{
		_WCSCPY_S( Dest, BufferBytes, CurrentDir ) ;
		goto END ;
	}

	i = 0 ;
	k = 0 ;

	DestBytes = 0 ;

	// １文字目と２文字目を取得
	CharCode1 = GetCharCode( ( const char * )&( ( BYTE * )Src )[ 0 ], WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
	CharCode2 = 0 ;
	if( CharCode1 != 0 )
	{
		CharCode2 = GetCharCode( ( const char * )&( ( BYTE * )Src )[ CharBytes1 ], WCHAR_T_CHARCODEFORMAT, &CharBytes2 ) ;
	}

	// 最初に『\』又は『/』が２回連続で続いている場合はネットワークを介していると判断

	if( ( CharCode1 == '\\' && CharCode2 == '\\' ) ||
		( CharCode1 == '/'  && CharCode2 == '/'  ) )
	{
		DestBytes += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;

		i += CharBytes1 + CharBytes2 ;
	}
	else
	// 最初が『\』又は『/』の場合はカレントドライブのルートディレクトリまで落ちる
	if( CharCode1 == '\\' || CharCode1 == '/' )
	{
		DWORD CurCharCode1 ;
		DWORD CurCharCode2 ;
		int   CurCharBytes1 ;
		int   CurCharBytes2 ;

		CurCharCode1 = GetCharCode( ( char * )&( ( BYTE * )CurrentDir )[ 0             ], WCHAR_T_CHARCODEFORMAT, &CurCharBytes1 ) ;
		CurCharCode2 = GetCharCode( ( char * )&( ( BYTE * )CurrentDir )[ CurCharBytes1 ], WCHAR_T_CHARCODEFORMAT, &CurCharBytes2 ) ;
		DestBytes += PutCharCode( CurCharCode1, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		DestBytes += PutCharCode( CurCharCode2, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;

		i += CharBytes1 ;
	}
	else
	// ドライブ名が書かれていたらそのドライブへ
	if( CharCode2 == ':' )
	{
		DestBytes += PutCharCode( CharUp ? CHARUP( CharCode1 ) : CharCode1, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		DestBytes += PutCharCode(                  CharCode2              , WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;

		i += CharBytes1 + CharBytes2 ;

		// : の後の \ マークは飛ばす
		CharCode1 = GetCharCode( ( const char * )&( ( BYTE * )Src )[ i ], WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
		if( CharCode1 == '\\' )
		{
			i += CharBytes1 ;
		}
	}
	else
	// それ以外の場合はカレントディレクトリ
	{
		_WCSCPY_S( Dest, BufferBytes, CurrentDir ) ;
		DestBytes += _WCSLEN( Dest ) * sizeof( wchar_t ) ;

		CharNum = GetStringCharNum(  ( const char * )CurrentDir, WCHAR_T_CHARCODEFORMAT ) ;
		if( CharNum != 0 )
		{
			LastCharAddress = ( char * )GetStringCharAddress( ( const char * )CurrentDir, WCHAR_T_CHARCODEFORMAT, CharNum - 1 ) ;
			LastCharCode    = GetCharCode( LastCharAddress, WCHAR_T_CHARCODEFORMAT, &LastCharBytes ) ;
			if( LastCharCode == '\\' || LastCharCode == '/' )
			{
				PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, LastCharAddress, BUFFERBYTES_CANCEL ) ;
				DestBytes -= LastCharBytes ;
			}
		}
	}

	for(;;)
	{
		CharCode1 = GetCharCode( ( const char * )&( ( BYTE * )Src )[ i ], WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
		switch( CharCode1 )
		{
		case '\0' :
			if( k != 0 )
			{
				CharCode2 = GetCharCode( ( const char * )&( ( BYTE * )Dest )[ 0 ], WCHAR_T_CHARCODEFORMAT, &CharBytes2 ) ;
				if( CharCode2 != '\0' )
				{
					DestBytes += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
				}
				_WCSCPY_S( ( wchar_t * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes, iden ) ;
				DestBytes = _WCSLEN( Dest ) * sizeof( wchar_t ) ;
				k  = 0 ;
			}
			goto END ;

		case '\\' :
		case '/' :
			// 文字列が無かったらスキップ
			if( k == 0 )
			{
				i += CharBytes1 ;
				break ;
			}
			if( _WCSCMP( iden, L"." ) == 0 )
			{
				// なにもしない
			}
			else
			if( _WCSCMP( iden, L".." ) == 0 )
			{
				// 一つ浅いディレクトリへ
				PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
				CharNum         = GetStringCharNum(  ( const char * )Dest, WCHAR_T_CHARCODEFORMAT ) ;
				LastCharAddress = ( char * )GetStringCharAddress( ( const char * )Dest, WCHAR_T_CHARCODEFORMAT, CharNum - 1 ) ;
				LastCharCode    = GetCharCode( LastCharAddress, WCHAR_T_CHARCODEFORMAT, &LastCharBytes ) ;
				DestBytes -= LastCharBytes ;
				for(;;)
				{
					if( LastCharCode == '\\' || LastCharCode == '/' || LastCharCode == ':' )
					{
						break ;
					}

					PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
					CharNum -- ;
					LastCharAddress = ( char * )GetStringCharAddress( ( const char * )Dest, WCHAR_T_CHARCODEFORMAT, CharNum - 1 ) ;
					LastCharCode    = GetCharCode( LastCharAddress, WCHAR_T_CHARCODEFORMAT, &LastCharBytes ) ;
					DestBytes -= LastCharBytes ;
				}

				if( LastCharCode != ':' )
				{
					PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
				}
				else
				{
					if( BufferBytes - DestBytes > ( size_t )LastCharBytes )
					{
						DestBytes += LastCharBytes ;
					}
				}
			}
			else
			{
				CharCode2 = GetCharCode( ( const char * )&( ( BYTE * )Dest )[ 0 ], WCHAR_T_CHARCODEFORMAT, &CharBytes2 ) ;
				if( CharCode2 != '\0' )
				{
					DestBytes += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
				}
				_WCSCPY_S( ( wchar_t * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes, iden ) ;
				DestBytes = _WCSLEN( Dest ) * sizeof( wchar_t ) ;
			}

			k = 0 ;
			i += CharBytes1 ;
			break ;
		
		default :
			k += PutCharCode( CharUp ? CHARUP( CharCode1 ) : CharCode1, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )iden )[ k ], sizeof( iden ) - k ) ;
			     PutCharCode( '\0',                                     WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )iden )[ k ], sizeof( iden ) - k ) ;

			i += CharBytes1 ;
			break ;
		}
	}
	
END :
	// 正常終了
	return 0 ;
#endif
}

// ファイル名も一緒になっていると分かっているパス中からファイル名とディレクトリパスを分割する
// フルパスである必要は無い、ファイル名だけでも良い
// DirPath の終端に ￥ マークは付かない
static int DXA_DIR_AnalysisFileNameAndDirPath( DXARC *DXA, const BYTE *Src, BYTE *FileName, size_t FileNameBytes, BYTE *DirPath, size_t DirPathBytes )
{
	int   i ;
	int   Last ;
	int   LastCharBytes ;
	DWORD CharCode ;
	int   CharBytes ;
	
	// ファイル名を抜き出す
	i    = 0 ;
	Last = -1 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&Src[ i ], DXA->CharCodeFormat, &CharBytes ) ;
		if( CharCode == '\0' )
		{
			break ;
		}

		if( CharCode == '\\' || CharCode == '/' || CharCode == '\0' || CharCode == ':' )
		{
			Last          = i ;
			LastCharBytes = CharBytes ;
		}

		i += CharBytes ;
	}

	if( FileName != NULL )
	{
		if( Last != -1 )
		{
			if( FileNameBytes > ( size_t )( Last + LastCharBytes ) )
			{
				CL_strncpy_s( DXA->CharCodeFormat, ( char * )FileName, FileNameBytes, ( const char * )&Src[ Last + LastCharBytes ], ( int )( ( FileNameBytes - ( Last + LastCharBytes ) ) / GetCharCodeFormatUnitSize( DXA->CharCodeFormat ) ) ) ;
			}
		}
		else
		{
			CL_strncpy_s( DXA->CharCodeFormat, ( char * )FileName, FileNameBytes, ( const char * )Src, ( int )( FileNameBytes / GetCharCodeFormatUnitSize( DXA->CharCodeFormat ) ) ) ;
		}
	}
	
	// ディレクトリパスを抜き出す
	if( DirPath != NULL )
	{
		if( Last != -1 )
		{
			if( DirPathBytes > ( size_t )Last + 1 )
			{
				_MEMCPY( DirPath, Src, ( size_t )Last ) ;
				PutCharCode( '\0', DXA->CharCodeFormat, ( char * )&DirPath[ Last ], BUFFERBYTES_CANCEL ) ;
			}
		}
		else
		{
			PutCharCode( '\0', DXA->CharCodeFormat, ( char * )&DirPath[ 0 ], DirPathBytes ) ;
		}
	}
	
	// 終了
	return 0 ;
}

// CmpStr の条件に Src が適合するかどうかを調べる( 0:適合する  -1:適合しない )
static int DXA_DIR_FileNameCmp( DXARC *DXA, const BYTE *Src, const BYTE *CmpStr )
{
	const BYTE *s, *c ;
	DWORD SrcCharCode ;
	int   SrcCharBytes ;
	DWORD CmpCharCode ;
	int   CmpCharBytes ;

	s = Src ;
	c = CmpStr ;
	for(;;)
	{
		SrcCharCode = GetCharCode( ( const char * )s, DXA->CharCodeFormat, &SrcCharBytes ) ;
		CmpCharCode = GetCharCode( ( const char * )c, DXA->CharCodeFormat, &CmpCharBytes ) ;

		if( SrcCharCode != '\0' && CmpCharCode == '\0' )
		{
			return -1 ;
		}

		if( SrcCharCode == '\0' && CmpCharCode != '\0' )
		{
			if( CmpCharCode != '*' )
			{
				return -1 ;
			}

			while( CmpCharCode == '*' )
			{
				c += CmpCharBytes ;
				CmpCharCode = GetCharCode( ( const char * )c, DXA->CharCodeFormat, &CmpCharBytes ) ;
			}
			if( CmpCharCode != '\0' )
			{
				return -1 ;
			}

			break ;
		}

		if( SrcCharCode == '\0' && CmpCharCode == '\0' )
		{
			break ;
		}

		switch( CmpCharCode )
		{
		case '?':
			c += CmpCharBytes ;
			s += SrcCharBytes ;
			break;

		case '*':
			while( CmpCharCode == '*' )
			{
				c += CmpCharBytes ;
				CmpCharCode = GetCharCode( ( const char * )c, DXA->CharCodeFormat, &CmpCharBytes ) ;
			}
			if( CmpCharCode == '\0' )
			{
				return 0 ;
			}

			while( SrcCharCode != '\0' && SrcCharCode != CmpCharCode )
			{
				s += SrcCharBytes ;
				SrcCharCode = GetCharCode( ( const char * )s, DXA->CharCodeFormat, &SrcCharBytes ) ;
			}
			if( SrcCharCode == '\0' )
			{
				return -1 ;
			}

			c += CmpCharBytes ;
			s += SrcCharBytes ;
			break;

		default:
			if( CmpCharCode != SrcCharCode )
			{
				return -1 ;
			}

			c += CmpCharBytes ;
			s += SrcCharBytes ;
			break;
		}

		if( ( CmpCharCode == '\0' && SrcCharCode != '\0' ) ||
			( CmpCharCode != '\0' && SrcCharCode == '\0' ) )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// アーカイブファイルをフォルダに見立ててファイルを開く時の情報を得る( -1:アーカイブとしては存在しなかった  0:存在した )
static int DXA_DIR_OpenTest( const wchar_t *FilePath, int *ArchiveIndex, BYTE *ArchiveFilePath, size_t BufferBytes )
{
	int   i ;
	int   len ;
	int   arcindex ;
	wchar_t fullpath[ FILEPATH_MAX ] ;
	wchar_t fullpath_up[ FILEPATH_MAX ] ;
	wchar_t path[ FILEPATH_MAX ] ;
	wchar_t temp[ FILEPATH_MAX ] ;
	wchar_t dir[ FILEPATH_MAX ] ;
	wchar_t *p ;
	int   BackUseDirectoryPathCharValid ;
	DWORD BackUseDirectoryPathCharCode ;
	int   BackUseDirectoryPathCharBytes ;

	// フルパスを得る
	if( DXARCD.NotArchivePathCharUp )
	{
		DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ), 0 ) ;
		CL_strcpy( WCHAR_T_CHARCODEFORMAT, ( char * )fullpath_up, ( char * )fullpath ) ;
		CL_strupr( WCHAR_T_CHARCODEFORMAT, ( char * )fullpath_up ) ;
	}
	else
	{
		DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ), 1 ) ;
	}

	// 前回と使用するアーカイブのパスが同じ場合は同じアーカイブを使用する
	BackUseDirectoryPathCharValid = FALSE ;
	if( DXARCD.BackUseDirectoryPathLength != 0 && _MEMCMP( fullpath, DXARCD.BackUseDirectory, DXARCD.BackUseDirectoryPathLength ) == 0 )
	{
		BackUseDirectoryPathCharValid = TRUE ;
		BackUseDirectoryPathCharCode  = GetCharCode( ( const char * )&( ( BYTE * )fullpath )[ DXARCD.BackUseDirectoryPathLength ], WCHAR_T_CHARCODEFORMAT, &BackUseDirectoryPathCharBytes ) ;
	}
	if( BackUseDirectoryPathCharValid &&
		( BackUseDirectoryPathCharCode == '\\' || BackUseDirectoryPathCharCode == '/' ) )
	{
		// 前回使用したＤＸＡファイルを開く
		arcindex = DXA_DIR_OpenArchive( DXARCD.BackUseDirectory, NULL, -1, FALSE, FALSE, DXARCD.BackUseArchiveIndex ) ;
		if( arcindex == -1 )
		{
			return -1 ;
		}

		// ＤＸＡファイルがある場所以降のパスを作成する
		if( DXARCD.NotArchivePathCharUp )
		{
			p = ( wchar_t * )&( ( BYTE * )fullpath_up )[ DXARCD.BackUseDirectoryPathLength + BackUseDirectoryPathCharBytes ] ;
		}
		else
		{
			p = ( wchar_t * )&( ( BYTE * )fullpath    )[ DXARCD.BackUseDirectoryPathLength + BackUseDirectoryPathCharBytes ] ;
		}
	}
	else
	{
		DWORD CharCode1 ;
		int   CharBytes1 ;
		DWORD CharCode2 ;
		int   CharBytes2 ;

		// 前回とは違うパスの場合は一から調べる

		// ディレクトリを一つ一つ追って行く
		p   = fullpath ;
		len = 0 ;
		for(;;)
		{
			// ネットワークを介していた場合の専用処理
			if( p - fullpath == 0 )
			{
				// fullpath の１文字目と２文字目を取得
				CharCode1 = GetCharCode( ( const char * )&( ( BYTE * )fullpath )[ 0 ], WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
				CharCode2 = 0 ;
				if( CharCode1 != 0 )
				{
					CharCode2 = GetCharCode( ( const char * )&( ( BYTE * )fullpath )[ CharBytes1 ], WCHAR_T_CHARCODEFORMAT, &CharBytes2 ) ;
				}

				if( CharCode1 == '\\' && CharCode2 == '\\' )
				{
					len += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
					len += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
					       PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;

					p = ( wchar_t * )( ( BYTE * )p + CharBytes1 + CharBytes2 ) ;
				}
			}

			// ディレクトリを一つ取る
			i = 0 ;
			for(;;)
			{
				CharCode1 = GetCharCode( ( const char * )p, WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
				p = ( wchar_t * )( ( BYTE * )p + CharBytes1 ) ;
				if( ( wchar_t * )( ( BYTE * )p - CharBytes1 ) != fullpath && ( CharCode1 == '\0' || CharCode1 == '/' || CharCode1 == '\\' ) )
				{
					break ;
				}

				len += PutCharCode( CharCode1, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
				i   += PutCharCode( CharCode1, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )dir  )[ i   ], sizeof( dir  ) - i ) ;

			}

			if( CharCode1 == '\0' || i == 0 )
			{
				return -1 ;
			}

			PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
			PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )dir  )[ i   ], sizeof( dir  ) - i   ) ;

			// フォルダ名をDXアーカイブファイル名にする
			{
				int TempLen ;

				_MEMCPY( temp, path, len ) ;
				TempLen = len ;

				TempLen += PutCharCode( '.', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
				if( DXARCD.ArchiveExtensionLength == 0 )
				{
					if( DXARCD.NotArchivePathCharUp )
					{
						TempLen += PutCharCode( 'd',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
						TempLen += PutCharCode( 'x',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
						TempLen += PutCharCode( 'a',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
					}
					else
					{
						TempLen += PutCharCode( 'D',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
						TempLen += PutCharCode( 'X',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
						TempLen += PutCharCode( 'A',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
					}
					TempLen += PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
				}
				else
				{
					_WCSCPY_S( ( wchar_t * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen, DXARCD.ArchiveExtension ) ;
				}
			}

			// ＤＸＡファイルとして開いてみる
			arcindex = DXA_DIR_OpenArchive( temp ) ;
			if( arcindex != -1 )
			{
				break ;
			}

			// 開けなかったら次の階層へ
			len += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
		}

		// 開けたら今回の情報を保存する
		_WCSCPY_S( DXARCD.BackUseDirectory, sizeof( DXARCD.BackUseDirectory ), temp ) ;
//		if( DXARCD.ArchiveExtensionLength == 0 )
//		{
//			_MEMCPY( DXARCD.BackUseDirectory, temp, len + ( 1 
// 
// . */ + 3 /* DXA */ + 1 /* '\0' */ ) * sizeof( wchar_t ) ) ;
//		}
//		else
//		{
//			_MEMCPY( DXARCD.BackUseDirectory, temp, len + ( 1 /* . */ + DXARCD.ArchiveExtensionLength + 1 /* '\0' */ ) * sizeof( wchar_t ) ) ;
//		}
		DXARCD.BackUseDirectoryPathLength = len ;
		DXARCD.BackUseArchiveIndex        = arcindex ;

		if( DXARCD.NotArchivePathCharUp )
		{
			p = fullpath_up + ( p - fullpath ) ;
		}
	}

	// 情報をセットする
	*ArchiveIndex = arcindex;

	if( ArchiveFilePath )
	{
		int DestCharCodeFormat ;

		DestCharCodeFormat = DXARCD.Archive[ arcindex ]->Archive.CharCodeFormat ;
		ConvString( ( const char * )p, -1, WCHAR_T_CHARCODEFORMAT, ( char * )ArchiveFilePath, BufferBytes, DestCharCodeFormat ) ;
	}

	// 終了
	return 0;
}

// アーカイブファイルを開く
static int DXA_DIR_OpenArchive( const wchar_t *FilePath, void *FileImage, int FileSize, int FileImageCopyFlag, int FileImageReadOnly, int ArchiveIndex, int OnMemory, int ASyncThread )
{
	int					i ;
	int					index ;
	int					newindex ;
	DXARC_DIR_ARCHIVE *	arc ;
	DXARC_DIR_ARCHIVE *	tarc ;
	DXARC				temparc ;

	// アーカイブの指定がある場合はそのまま使用する
	if( ArchiveIndex != -1 )
	{
		tarc = DXARCD.Archive[ ArchiveIndex ] ;
		if( tarc != NULL )
		{
			if(	_WCSCMP( FilePath, tarc->Path ) == 0 )
			{
				DXARCD.Archive[ ArchiveIndex ]->UseCounter ++ ;
				return ArchiveIndex ;
			}
		}
	}

	// 既に開かれているか調べる
	newindex = -1 ;
	index    = 0 ;
	for( i = 0 ; i < DXARCD.ArchiveNum ; index ++ )
	{
		arc = DXARCD.Archive[ index ] ;
		if( arc == NULL )
		{
			newindex = index ;
			continue ;
		}
		
		i ++ ;

		if( _WCSCMP( arc->Path, FilePath ) == 0 )
		{
			// 既に開かれていた場合はそのインデックスを返す
			arc->UseCounter ++ ;
			return index ;
		}
	}
	
	// なかった場合は新規にデータを追加する

	// ハンドルが既に一杯の場合は使用されていないアーカイブハンドルを解放する
	if( DXARCD.ArchiveNum == DXA_DIR_MAXARCHIVENUM )
	{
		// 未使用のハンドルを解放
		DXA_DIR_CloseWaitArchive() ;
		
		// それでも一杯である場合はエラー
		if( DXARCD.ArchiveNum == DXA_DIR_MAXARCHIVENUM )
		{
			return -1 ;
		}
	} 
	if( newindex == -1 )
	{
		for( newindex = 0 ; DXARCD.Archive[ newindex ] != NULL ; newindex ++ ){}
	}

	// アーカイブファイルが存在しているか確認がてら初期化する
	DXA_Initialize( &temparc ) ;
	if( FileImage != NULL )
	{
		// メモリ上に展開されたファイルイメージを使用する場合
		if( DXA_OpenArchiveFromMem( &temparc, FileImage, FileSize, FileImageCopyFlag, FileImageReadOnly, DXARCD.ValidKeyString == TRUE ? DXARCD.KeyString : NULL, FilePath ) < 0 )
			return -1 ;
	}
	else
	if( OnMemory == TRUE )
	{
		// メモリに読み込む場合
		if( DXA_OpenArchiveFromFileUseMem( &temparc, FilePath, DXARCD.ValidKeyString == TRUE ? DXARCD.KeyString : NULL, ASyncThread ) < 0 )
			return -1 ;
	}
	else
	{
		// ファイルから読み込む場合
		if( DXA_OpenArchiveFromFile( &temparc, FilePath, DXARCD.ValidKeyString == TRUE ? DXARCD.KeyString : NULL ) < 0 )
			return -1 ;
	}

	// 新しいアーカイブデータ用のメモリを確保する
	arc = DXARCD.Archive[ newindex ] = ( DXARC_DIR_ARCHIVE * )DXALLOC( sizeof( DXARC_DIR_ARCHIVE ) ) ;
	if( DXARCD.Archive[ newindex ] == NULL )
	{
		DXA_CloseArchive( &temparc ) ;
		DXA_Terminate( &temparc ) ;
		return -1 ;
	}

	// 情報セット
	_MEMCPY( &arc->Archive, &temparc, sizeof( DXARC ) ) ;
	arc->UseCounter = 1 ;
	_WCSCPY_S( arc->Path, sizeof( arc->Path ), FilePath ) ;

	// 使用中のアーカイブの数を増やす
	DXARCD.ArchiveNum ++ ;

	// インデックスを返す
	return newindex ;
}

// 既に開かれているアーカイブのハンドルを取得する( 戻り値: -1=無かった 0以上:ハンドル )
static int DXA_DIR_GetArchive( const wchar_t *FilePath, void *FileImage )
{
	int i, index ;
	DXARC_DIR_ARCHIVE *arc ;

	index = 0 ;
	for( i = 0 ; i < DXARCD.ArchiveNum ; index ++ )
	{
		arc = DXARCD.Archive[index] ;
		if( arc == NULL )
		{
			continue ;
		}

		i ++ ;

		if( FilePath )
		{
			if( _WCSCMP( arc->Path, FilePath ) == 0 )
			{
				return index ;
			}
		}
		else
		{
			if( arc->Archive.MemoryImageCopyFlag )
			{
				if( arc->Archive.MemoryImageOriginal == FileImage )
				{
					return index ;
				}
			}
			else
			{
				if( arc->Archive.MemoryImage == FileImage )
				{
					return index ;
				}
			}
		}
	}

	return -1 ;
}

// アーカイブファイルを閉じる
static int DXA_DIR_CloseArchive( int ArchiveHandle )
{
	DXARC_DIR_ARCHIVE *arc ;

	// 使用されていなかったら何もせず終了
	arc = DXARCD.Archive[ArchiveHandle] ;
	if( arc == NULL || arc->UseCounter == 0 )
	{
		return -1 ;
	}

	// 参照カウンタを減らす
	arc->UseCounter -- ;

	// 終了
	return 0 ;
}

// 使用されるのを待っているアーカイブファイルを全て閉じる
static void DXA_DIR_CloseWaitArchive( void )
{
	int i, Num, index ;
	DXARC_DIR_ARCHIVE *arc ;
	
	Num = DXARCD.ArchiveNum ;
	for( i = 0, index = 0 ; i < Num ; index ++ )
	{
		if( DXARCD.Archive[index] == NULL )
		{
			continue ;
		}
		i ++ ;

		arc = DXARCD.Archive[index] ;

		// 使われていたら解放しない
		if( arc->UseCounter > 0 )
		{
			continue ;
		}

		// 後始末
		DXA_CloseArchive( &arc->Archive ) ;
		DXA_Terminate( &arc->Archive ) ;
		DXFREE( arc ) ;
		DXARCD.Archive[index] = NULL ;
		
		// アーカイブの数を減らす
		DXARCD.ArchiveNum -- ;
	}
}

// アーカイブをディレクトリに見立てる処理の初期化
extern	int DXA_DIR_Initialize( void )
{
	// 既に初期化済みの場合は何もしない
	if( DXARCD.InitializeFlag )
	{
		return -1 ;
	}

	// クリティカルセクションの初期化
	CriticalSection_Initialize( &DXARCD.CriticalSection ) ;

//	_MEMSET( &DXARCD, 0, sizeof( DXARC_DIR ) ) ;
//	DXA_DIR_Terminate() ;

	// 使用されていないアーカイブファイルを解放する
	DXA_DIR_CloseWaitArchive() ;

	// 初期化完了フラグを立てる
	DXARCD.InitializeFlag = TRUE ;

	// 終了
	return 0 ;
}


// アーカイブをディレクトリに見立てる処理の後始末
extern int DXA_DIR_Terminate( void )
{
	// 既に後始末済みの場合は何もしない
	if( DXARCD.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// 使用されていないアーカイブファイルを解放する
	DXA_DIR_CloseWaitArchive() ;

	// クリティカルセクションの後始末
	CriticalSection_Delete( &DXARCD.CriticalSection ) ;

	// 初期化完了フラグを倒す
	DXARCD.InitializeFlag = FALSE ;

	// 終了
	return 0 ;
}

// アーカイブファイルのパスを大文字にしないかどうかのフラグをセットする
extern int DXA_DIR_SetNotArchivePathCharUp( int NotArchivePathCharUpFlag )
{
	DXARCD.NotArchivePathCharUp = NotArchivePathCharUpFlag ;

	// 終了
	return 0 ;
}

// アーカイブファイルの拡張子を設定する
extern int DXA_DIR_SetArchiveExtension( const wchar_t *Extension )
{
	int Length ;
	
	Length = ( int )_WCSLEN( Extension ) ;

	if( Length >= 64 || Extension == NULL || Extension[0] == _T( '\0' ) )
	{
		DXARCD.ArchiveExtension[ 0 ]  = 0 ;
		DXARCD.ArchiveExtensionLength = 0 ;
	}
	else
	{
		DXARCD.ArchiveExtensionLength = Length ;
		_WCSCPY_S( DXARCD.ArchiveExtension, sizeof( DXARCD.ArchiveExtension ), Extension ) ;
	}

	// 終了
	return 0 ;
}

// アーカイブファイルと通常のフォルダのどちらも存在した場合、どちらを優先させるかを設定する( 1:フォルダを優先 0:ＤＸアーカイブファイルを優先(デフォルト) )
extern int DXA_DIR_SetDXArchivePriority( int Priority )
{
	DXARCD.DXAPriority = Priority ;

	// 終了
	return 0 ;
}

// アーカイブファイルの鍵文字列を設定する
extern int DXA_DIR_SetKeyString( const char *KeyString )
{
	if( KeyString == NULL )
	{
		DXARCD.ValidKeyString = FALSE ;
	}
	else
	{
		DXARCD.ValidKeyString = TRUE ;
		if( _STRLEN( KeyString ) > DXA_KEYV2STR_LENGTH )
		{
			_MEMCPY( DXARCD.KeyString, KeyString, DXA_KEYV2STR_LENGTH ) ;
			DXARCD.KeyString[ DXA_KEYV2STR_LENGTH ] = '\0' ;
		}
		else
		{
			_STRCPY( DXARCD.KeyString, KeyString ) ;
		}
	}

	// 終了
	return 0 ;
}

// ファイルを丸ごと読み込む関数
extern LONGLONG DXA_DIR_LoadFile( const wchar_t *FilePath, void *Buffer, int BufferSize )
{
	LONGLONG siz ;
	DWORD_PTR handle ;

	handle = DXA_DIR_Open( FilePath ) ;
	if( handle == 0 )
	{
		return false ;
	}

	DXA_DIR_Seek( handle, 0L, SEEK_END ) ;
	siz = DXA_DIR_Tell( handle ) ;
	DXA_DIR_Seek( handle, 0L, SEEK_SET ) ;

	if( siz <= BufferSize )
	{
		DXA_DIR_Read( Buffer, ( size_t )siz, 1, handle ) ;
	}

	DXA_DIR_Close( handle ) ;

	// 終了
	return siz ;
}

// DXA_DIR_Open の基本関数
extern DWORD_PTR DXA_DIR_Open( const wchar_t *FilePath, int UseCacheFlag, int BlockReadFlag, int UseASyncReadFlag )
{
	int index ;
	DXARC_DIR_FILE *file ;
//	char DXAErrorStr[ FILEPATH_MAX ] ;
	BYTE DxaInFilePath[ FILEPATH_MAX ] ;

	// 初期化されていなかったら初期化する
	if( DXARCD.InitializeFlag == FALSE )
	{
		DXA_DIR_Initialize() ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &DXARCD.CriticalSection ) ;

//	UseCacheFlag  = UseCacheFlag ;
//	BlockReadFlag = BlockReadFlag ;
//	DXAErrorStr[ 0 ] = 0 ;

	// 空きデータを探す
	if( DXARCD.FileNum == DXA_DIR_MAXFILENUM )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

		DXST_LOGFILE_ADDUTF16LE( "\x0c\x54\x42\x66\x6b\x30\x8b\x95\x51\x30\x8b\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\x70\x65\x4c\x30\x50\x96\x4c\x75\x92\x30\x85\x8d\x48\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"同時に開けるファイルの数が限界を超えました\n" @*/ ) ;
		return 0 ;
	}
	for( index = 0 ; DXARCD.File[index] != NULL ; index ++ ){}

	// メモリの確保
	DXARCD.File[ index ] = (DXARC_DIR_FILE *)DXALLOC( sizeof( DXARC_DIR_FILE ) ) ;
	if( DXARCD.File[ index ] == NULL )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

		DXST_LOGFILE_ADDUTF16LE( "\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ファイルの情報を格納するメモリの確保に失敗しました\n" @*/ ) ;
		return 0 ;
	}
	file = DXARCD.File[index] ;

	// アーカイブファイルとフォルダのどちらを優先するかで処理を分岐
	if( DXARCD.DXAPriority == 0 )
	{
		int FileOpen = FALSE ;

		// アーカイブを優先する場合

		// アーカイブファイルが無いか調べる
		if( DXA_DIR_OpenTest( FilePath, ( int * )&file->UseArchiveIndex, DxaInFilePath, sizeof( DxaInFilePath ) ) == 0 )
		{
			// ディレクトリ名と同名のＤＸＡファイルを開けたらその中から指定のファイルを読み込もうとする
			if( DXA_STREAM_Initialize( &file->DXAStream, &DXARCD.Archive[ file->UseArchiveIndex ]->Archive, DxaInFilePath, UseASyncReadFlag ) < 0 )
			{
//				_STRCPY( DXAErrorStr, DXSTRING( "ＤＸＡファイルの中に指定のファイルは見つかりませんでした\n" ) ) ;
				DXA_DIR_CloseArchive( ( int )file->UseArchiveIndex ) ;
			}
			else
			{
				// アーカイブを使用しているフラグを立てる
				file->UseArchiveFlag = 1 ;

				FileOpen = TRUE ;
			}
		}

		if( FileOpen == FALSE )
		{
			// アーカイブファイルが無かったら普通のファイルから読み込む設定を行う
			file->UseArchiveFlag = 0 ;

			// 普通のファイルが無いか調べる
			file->WinFilePointer_ = ReadOnlyFileAccessOpen( FilePath, UseCacheFlag, TRUE, UseASyncReadFlag ) ;
			if( file->WinFilePointer_ == 0 )
			{
				goto ERR ;
			}
			else
			{
				FileOpen = TRUE ;
			}
		}
	}
	else
	{
		// 普通のファイルを優先する場合

		// 普通のファイルが無いか調べる
		if( ( file->WinFilePointer_ = ReadOnlyFileAccessOpen( FilePath, UseCacheFlag, TRUE, UseASyncReadFlag ) ) != 0 )
		{
			// 開いたら普通のファイルから読み込む設定を行う
			file->UseArchiveFlag = 0 ;
		}
		else
		{
			// アーカイブファイルが無いか調べる
			if( DXA_DIR_OpenTest( FilePath, (int *)&file->UseArchiveIndex, DxaInFilePath, sizeof( DxaInFilePath ) ) == 0 )
			{
				// ディレクトリ名と同名のＤＸＡファイルを開けたらその中から指定のファイルを読み込もうとする
				if( DXA_STREAM_Initialize( &file->DXAStream, &DXARCD.Archive[ file->UseArchiveIndex ]->Archive, DxaInFilePath, UseASyncReadFlag ) < 0 )
				{
//					_STRCPY( DXAErrorStr, DXSTRING( "ＤＸＡファイルの中に指定のファイルは見つかりませんでした\n" ) ) ;
					DXA_DIR_CloseArchive( ( int )file->UseArchiveIndex ) ;
					goto ERR ;
				}
				else
				{
					// アーカイブを使用しているフラグを立てる
					file->UseArchiveFlag = 1 ;
				}
			}
			else
			{
				// 無かったらエラー
				goto ERR;
			}
		}
	}

	// ハンドルの数を増やす
	DXARCD.FileNum ++ ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

	// インデックスを返す
	return index | 0xF0000000 ;

ERR:
	// メモリの解放
	if( DXARCD.File[index] != NULL ) DXFREE( DXARCD.File[index] ) ;
	DXARCD.File[index] = NULL ;
	
	// エラー文字列出力
//	DXST_LOGFILEFMT_ADDW(( L"ファイル %s のオープンに失敗しました\n", FilePath )) ;
//	if( DXAErrorStr[0] != '\0' ) DXST_LOGFILEFMT_ADDW(( "ＤＸＡエラー：%s", DXAErrorStr )) ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

	// エラー終了
	return 0 ;
}

// ファイルを閉じる
extern int DXA_DIR_Close( DWORD_PTR Handle )
{
	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &DXARCD.CriticalSection ) ;

	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;

	// 使用されていなかったら何もせず終了
	if( file == NULL )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

		return -1 ;
	}
	
	// アーカイブを使用しているかどうかで分岐
	if( file->UseArchiveFlag == FALSE )
	{
		// 使用していない場合は標準入出力のファイルポインタを解放する
		ReadOnlyFileAccessClose( file->WinFilePointer_ ) ;
		file->WinFilePointer_ = 0 ;
	}
	else
	{
		// アーカイブを使用していた場合はアーカイブの参照数を減らす
		
		// アーカイブファイルの参照数を減らす
		DXA_DIR_CloseArchive( ( int )file->UseArchiveIndex ) ;

		// アーカイブファイルの後始末
		DXA_STREAM_Terminate( &file->DXAStream ) ;
	}

	// メモリを解放する
	DXFREE( file ) ;
	DXARCD.File[Handle & 0x0FFFFFFF] = NULL ;
	
	// 数を減らす
	DXARCD.FileNum -- ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

	// 終了
	return 0 ;
}

// ファイルポインタの位置を取得する
extern	LONGLONG DXA_DIR_Tell( DWORD_PTR Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return -1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessTell( file->WinFilePointer_ ) ;
	}
	else
	{
		return DXA_STREAM_Tell( &file->DXAStream ) ;
	}
}

// ファイルポインタの位置を変更する
extern int DXA_DIR_Seek( DWORD_PTR Handle, LONGLONG SeekPoint, int SeekType )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return -1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessSeek( file->WinFilePointer_, SeekPoint, SeekType ) ;
	}
	else
	{
		return DXA_STREAM_Seek( &file->DXAStream, SeekPoint, SeekType ) ;
	}
}

// ファイルからデータを読み込む
extern size_t DXA_DIR_Read( void *Buffer, size_t BlockSize, size_t BlockNum, DWORD_PTR Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return ( size_t )-1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessRead( Buffer, BlockSize, BlockNum, file->WinFilePointer_ ) ;
	}
	else
	{
		return DXA_STREAM_Read( &file->DXAStream, Buffer, BlockSize * BlockNum ) / BlockSize ;
	}
}

// ファイルの終端を調べる
extern int DXA_DIR_Eof( DWORD_PTR Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return -1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessEof( file->WinFilePointer_ ) ;
	}
	else
	{
		return DXA_STREAM_Eof( &file->DXAStream ) ;
	}
}

extern	int DXA_DIR_ChDir( const wchar_t *Path )
{
	_WCHDIR( Path ) ;

	return 0 ;
}

extern	int DXA_DIR_GetDir( wchar_t *Buffer )
{
	_WGETCWD( Buffer, BUFFERBYTES_CANCEL ) ;

	return 0 ;
}

extern	int DXA_DIR_GetDirS( wchar_t *Buffer, size_t BufferBytes )
{
	_WGETCWD( Buffer, BufferBytes ) ;

	return 0 ;
}

extern	int DXA_DIR_IdleCheck( DWORD_PTR Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return -1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessIdleCheck( file->WinFilePointer_ ) ;
	}
	else
	{
		return DXA_STREAM_IdleCheck( &file->DXAStream ) ;
	}
}

// 戻り値: -1=エラー  -1以外=FindHandle
extern DWORD_PTR DXA_DIR_FindFirst( const wchar_t *FilePath, FILEINFOW *Buffer )
{
	DXA_DIR_FINDDATA *find ;
	BYTE nPath[ FILEPATH_MAX ] ;

	// メモリの確保
	find = ( DXA_DIR_FINDDATA * )DXALLOC( sizeof( DXA_DIR_FINDDATA ) ) ;
	if( find == NULL )
	{
		return ( DWORD_PTR )-1 ;
	}
	_MEMSET( find, 0, sizeof( *find ) ) ;

	// 指定のオブジェクトがアーカイブファイル内か調べる
	if( DXA_DIR_OpenTest( FilePath, &find->UseArchiveIndex, nPath, sizeof( nPath ) ) == -1 )
	{
		// アーカイブファイル内ではなかった場合はファイルから検索する
		find->UseArchiveFlag = 0 ;
		find->FindHandle = ReadOnlyFileAccessFindFirst( FilePath, Buffer ) ;
	}
	else
	{
		// アーカイブファイル内の場合はアーカイブファイル内から検索する
		find->UseArchiveFlag = 1 ;
		find->FindHandle = DXA_FindFirst( &DXARCD.Archive[ find->UseArchiveIndex ]->Archive, nPath, Buffer ) ;
	}

	// 検索ハンドルが取得できなかった場合はエラー
	if( find->FindHandle == ( DWORD_PTR )-1 )
	{
		// アーカイブファイル内の場合はアーカイブファイルの使用カウントを減らす
		if( find->UseArchiveFlag != 0 )
		{
			DXA_DIR_CloseArchive( find->UseArchiveIndex ) ;
		}

		DXFREE( find );
		return ( DWORD_PTR )-1 ;
	}

	// ハンドルを返す
	return (DWORD_PTR)find ;
}

// 戻り値: -1=エラー  0=成功
extern int DXA_DIR_FindNext( DWORD_PTR FindHandle, FILEINFOW *Buffer )
{
	DXA_DIR_FINDDATA *find;

	find = (DXA_DIR_FINDDATA *)FindHandle;
	if( find->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessFindNext( find->FindHandle, Buffer ) ;
	}
	else
	{
		return DXA_FindNext( find->FindHandle, Buffer ) ;
	}
}

// 戻り値: -1=エラー  0=成功
extern int DXA_DIR_FindClose( DWORD_PTR FindHandle )
{
	DXA_DIR_FINDDATA *find;

	find = (DXA_DIR_FINDDATA *)FindHandle;
	if( find->UseArchiveFlag == 0 )
	{
		ReadOnlyFileAccessFindClose( find->FindHandle ) ;
	}
	else
	{
		DXA_FindClose( find->FindHandle );
		DXA_DIR_CloseArchive( find->UseArchiveIndex ) ;
	}

	DXFREE( find );

	return 0;
}




// 指定のＤＸＡファイルを丸ごとメモリに読み込む( 戻り値: -1=エラー  0=成功 )
extern int NS_DXArchivePreLoad( const TCHAR *FilePath , int ASyncThread )
{
	wchar_t fullpath[ FILEPATH_MAX ] ;

	// フルパスを得る(ついでに全ての文字を大文字にする)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )FilePath, -1, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#endif // UNICODE

	return DXA_DIR_OpenArchive( fullpath, NULL, -1, FALSE, FALSE, -1, TRUE, ASyncThread ) == -1 ? -1 : 0 ;
}

// 指定のＤＸＡファイルを丸ごとメモリに読み込む( 戻り値  -1:エラー  0:成功 )
extern int NS_DXArchivePreLoadWithStrLen( const TCHAR *FilePath, size_t FilePathLength, int ASyncThread )
{
	int Result ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_ONE_BEGIN( FilePath, FilePathLength, return -1 )
	Result = NS_DXArchivePreLoad( UseFilePathBuffer , ASyncThread ) ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( FilePath )
	return Result ;
}

// 指定のＤＸＡファイルの事前読み込みが完了したかどうかを取得する( 戻り値： TRUE=完了した FALSE=まだ )
extern int NS_DXArchiveCheckIdle( const TCHAR *FilePath )
{
	int handle;
	wchar_t fullpath[FILEPATH_MAX];

	// フルパスを得る(ついでに全ての文字を大文字にする)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )FilePath, -1, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#endif // UNICODE

	// ファイルパスからハンドルを取得する
	handle = DXA_DIR_GetArchive( fullpath ) ;
	if( handle == -1 )
	{
		return 0 ;
	}

	// 準備が完了したかどうかを得る
	return DXA_CheckIdle( &DXARCD.Archive[handle]->Archive ) ;
}

// 指定のＤＸＡファイルの事前読み込みが完了したかどうかを取得する( 戻り値  TRUE:完了した FALSE:まだ )
extern int NS_DXArchiveCheckIdleWithStrLen( const TCHAR *FilePath, size_t FilePathLength )
{
	int Result ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_ONE_BEGIN( FilePath, FilePathLength, return -1 )
	Result = NS_DXArchiveCheckIdle( UseFilePathBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( FilePath )
	return Result ;
}

// 指定のＤＸＡファイルをメモリから解放する
extern int NS_DXArchiveRelease( const TCHAR *FilePath )
{
	int handle;
	wchar_t fullpath[ FILEPATH_MAX ] ;

	// フルパスを得る(ついでに全ての文字を大文字にする)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )FilePath, -1, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#endif // UNICODE

	// ファイルパスからハンドルを取得する
	handle = DXA_DIR_GetArchive( fullpath ) ;
	if( handle == -1 )
	{
		return 0 ;
	}

	// 閉じる
	DXA_DIR_CloseArchive( handle ) ;
	DXA_DIR_CloseWaitArchive() ;

	// 終了
	return 0 ;
}

// 指定のＤＸＡファイルをメモリから解放する
extern int NS_DXArchiveReleaseWithStrLen( const TCHAR *FilePath, size_t FilePathLength )
{
	int Result ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_ONE_BEGIN( FilePath, FilePathLength, return -1 )
	Result = NS_DXArchiveRelease( UseFilePathBuffer ) ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( FilePath )
	return Result ;
}

// ＤＸＡファイルの中に指定のファイルが存在するかどうかを調べる、TargetFilePath はＤＸＡファイルをカレントフォルダとした場合のパス( 戻り値:  -1=エラー  0:無い  1:ある )
extern int NS_DXArchiveCheckFile( const TCHAR *FilePath, const TCHAR *TargetFilePath )
{
	int index, ret ;
	DXARC_DIR_ARCHIVE *Archive ;
	wchar_t fullpath[ FILEPATH_MAX ] ;

	// フルパスを得る(ついでに全ての文字を大文字にする)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )FilePath, -1, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#endif // UNICODE

	// アーカイブファイルがあるかどうか調べる
	index = DXA_DIR_OpenArchive( fullpath ) ;
	if( index == -1 )
	{
		return -1 ;
	}

	// アーカイブの中に指定のファイルがあるかどうかを調べる
	Archive = DXARCD.Archive[ index ] ;
	ret = DXA_GetFileInfo( &Archive->Archive, _TCHARCODEFORMAT, ( const char * )TargetFilePath, NULL, NULL ) ;

	DXA_DIR_CloseArchive( index ) ;

	// 結果を返す
	return ret == -1 ? 0 : 1 ;
}

// ＤＸＡファイルの中に指定のファイルが存在するかどうかを調べる、TargetFilePath はＤＸＡファイルをカレントフォルダとした場合のパス( 戻り値:  -1=エラー  0:無い  1:ある )
extern int NS_DXArchiveCheckFileWithStrLen( const TCHAR *FilePath, size_t FilePathLength, const TCHAR *TargetFilePath, size_t TargetFilePathLength )
{
	int Result = -1 ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( FilePath )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_BEGIN( TargetFilePath )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( FilePath,       FilePathLength,       goto ERR )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_SETUP( TargetFilePath, TargetFilePathLength, goto ERR )

	Result = NS_DXArchiveCheckFile( UseFilePathBuffer, UseTargetFilePathBuffer ) ;

ERR :

	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( FilePath )
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( TargetFilePath )
	return Result ;
}

// メモリ上に展開されたＤＸＡファイルを指定のファイルパスにあることにする
extern int NS_DXArchiveSetMemImage(		void *ArchiveImage, int ArchiveImageSize, const TCHAR *EmulateFilePath, int ArchiveImageCopyFlag, int ArchiveImageReadOnly )
{
	wchar_t fullpath[ FILEPATH_MAX ] ;

	// フルパスを得る(ついでに全ての文字を大文字にする)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( EmulateFilePath, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )EmulateFilePath, -1, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ), DXARCD.NotArchivePathCharUp == FALSE ? 1 : 0 ) ;
#endif // UNICODE

	return DXA_DIR_OpenArchive( fullpath, ArchiveImage, ArchiveImageSize, ArchiveImageCopyFlag, ArchiveImageReadOnly, -1, FALSE, FALSE ) == -1 ? -1 : 0;
}

// メモリ上に展開されたＤＸＡファイルを指定のファイルパスにあることにする( EmulateFilePath は見立てる dxa ファイルのパス、例えばＤＸＡファイルイメージを Image.dxa というファイル名で c:\Temp にあることにしたい場合は EmulateFilePath に "c:\\Temp\\Image.dxa" を渡す、SetDXArchiveExtension で拡張子を変更している場合は EmulateFilePath に渡すファイルパスの拡張子もそれに合わせる必要あり )
extern int NS_DXArchiveSetMemImageWithStrLen( void *ArchiveImage, int ArchiveImageSize, const TCHAR *EmulateFilePath, size_t EmulateFilePathLength, int ArchiveImageCopyFlag, int ArchiveImageReadOnly )
{
	int Result ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_ONE_BEGIN( EmulateFilePath, EmulateFilePathLength, return -1 )
	Result = NS_DXArchiveSetMemImage( ArchiveImage, ArchiveImageSize, UseEmulateFilePathBuffer, ArchiveImageCopyFlag, ArchiveImageReadOnly ) ;
	TCHAR_STRING_WITH_STRLEN_TO_TCHAR_STRING_END( EmulateFilePath )
	return Result ;
}

// DXArchiveSetMemImage の設定を解除する
extern int NS_DXArchiveReleaseMemImage(	void *ArchiveImage )
{
	int handle;

	// ファイルパスからハンドルを取得する
	handle = DXA_DIR_GetArchive( NULL, ArchiveImage ) ;
	if( handle == -1 )
	{
		return 0 ;
	}

	// 閉じる
	DXA_DIR_CloseArchive( handle ) ;
	DXA_DIR_CloseWaitArchive() ;

	// 終了
	return 0 ;
}

#endif


// データを解凍する( 戻り値:解凍後のデータサイズ )
#define MIN_COMPRESS		(4)						// 最低圧縮バイト数
#define MAX_SEARCHLISTNUM	(64)					// 最大一致長を探す為のリストを辿る最大数
#define MAX_SUBLISTNUM		(65536)					// 圧縮時間短縮のためのサブリストの最大数
#define MAX_COPYSIZE 		(0x1fff + MIN_COMPRESS)	// 参照アドレスからコピー出切る最大サイズ( 圧縮コードが表現できるコピーサイズの最大値 + 最低圧縮バイト数 )
#define MAX_ADDRESSLISTNUM	(1024 * 1024 * 1)		// スライド辞書の最大サイズ
#define MAX_POSITION		(1 << 24)				// 参照可能な最大相対アドレス( 16MB )

// 圧縮時間短縮用リスト
typedef struct LZ_LIST
{
	LZ_LIST *next, *prev ;
	DWORD address ;
} LZ_LIST ;

// データを圧縮する( 戻り値:圧縮後のデータサイズ )
extern	int	DXA_Encode( void *Src, DWORD SrcSize, void *Dest )
{
	int dstsize ;
	int    bonus,    conbo,    conbosize,    address,    addresssize ;
	int maxbonus, maxconbo, maxconbosize, maxaddress, maxaddresssize ;
	BYTE keycode, *srcp, *destp, *dp, *sp, *sp2, *sp1 ;
	DWORD srcaddress, code ;
	int j ;
	DWORD i, m ;
	DWORD maxlistnum, maxlistnummask, listaddp ;
	DWORD sublistnum, sublistmaxnum ;
	LZ_LIST *listbuf, *listtemp, *list, *newlist ;
	BYTE *listfirsttable, *usesublistflagtable, *sublistbuf ;
	
	// サブリストのサイズを決める
	{
			 if( SrcSize < 100 * 1024 )			sublistmaxnum = 1 ;
		else if( SrcSize < 3 * 1024 * 1024 )	sublistmaxnum = MAX_SUBLISTNUM / 3 ;
		else									sublistmaxnum = MAX_SUBLISTNUM ;
	}

	// リストのサイズを決める
	{
		maxlistnum = MAX_ADDRESSLISTNUM ;
		if( maxlistnum > SrcSize )
		{
			while( ( maxlistnum >> 1 ) > 0x100 && ( maxlistnum >> 1 ) > SrcSize )
				maxlistnum >>= 1 ;
		}
		maxlistnummask = maxlistnum - 1 ;
	}

	// メモリの確保
	usesublistflagtable   = (BYTE *)DXALLOC(
		sizeof( DWORD_PTR )	* 65536 +					// メインリストの先頭オブジェクト用領域
		sizeof( LZ_LIST   )	* maxlistnum +				// メインリスト用領域
		sizeof( BYTE      )	* 65536 +					// サブリストを使用しているかフラグ用領域
		sizeof( DWORD_PTR )	* 256 * sublistmaxnum ) ;	// サブリスト用領域
		
	// アドレスのセット
	listfirsttable =     usesublistflagtable + sizeof( BYTE      ) * 65536 ;
	sublistbuf     =          listfirsttable + sizeof( DWORD_PTR ) * 65536 ;
	listbuf        = (LZ_LIST *)( sublistbuf + sizeof( DWORD_PTR ) * 256 * sublistmaxnum ) ;
	
	// 初期化
	_MEMSET( usesublistflagtable, 0, sizeof( BYTE      ) * 65536               ) ;
	_MEMSET(          sublistbuf, 0, sizeof( DWORD_PTR ) * 256 * sublistmaxnum ) ;
	_MEMSET(      listfirsttable, 0, sizeof( DWORD_PTR ) * 65536               ) ;
	list = listbuf ;
	for( i = maxlistnum / 8 ; i ; i --, list += 8 )
	{
		list[0].address =
		list[1].address =
		list[2].address =
		list[3].address =
		list[4].address =
		list[5].address =
		list[6].address =
		list[7].address = 0xffffffff ;
	}

	srcp  = (BYTE *)Src ;
	destp = (BYTE *)Dest ;

	// 圧縮元データの中で一番出現頻度が低いバイトコードを検索する
	{
		DWORD qnum, table[256], mincode ;

		for( i = 0 ; i < 256 ; i ++ )
			table[i] = 0 ;
		
		sp   = srcp ;
		qnum = SrcSize / 8 ;
		i    = qnum * 8 ;
		for( ; qnum ; qnum --, sp += 8 )
		{
			table[sp[0]] ++ ;
			table[sp[1]] ++ ;
			table[sp[2]] ++ ;
			table[sp[3]] ++ ;
			table[sp[4]] ++ ;
			table[sp[5]] ++ ;
			table[sp[6]] ++ ;
			table[sp[7]] ++ ;
		}
		for( ; i < SrcSize ; i ++, sp ++ )
			table[*sp] ++ ;
			
		keycode = 0 ;
		mincode = table[0] ;
		for( i = 1 ; i < 256 ; i ++ )
		{
			if( mincode < table[i] ) continue ;
			mincode = table[i] ;
			keycode = (BYTE)i ;
		}
	}

	// 圧縮元のサイズをセット
	((DWORD *)destp)[0] = SrcSize ;

	// キーコードをセット
	destp[8] = keycode ;

	// 圧縮処理
	dp               = destp + 9 ;
	sp               = srcp ;
	srcaddress       = 0 ;
	dstsize          = 0 ;
	listaddp         = 0 ;
	sublistnum       = 0 ;
	while( srcaddress < SrcSize )
	{
		// 残りサイズが最低圧縮サイズ以下の場合は圧縮処理をしない
		if( srcaddress + MIN_COMPRESS >= SrcSize ) goto NOENCODE ;

		// リストを取得
		code = *((WORD *)sp) ;
		list = (LZ_LIST *)( listfirsttable + code * sizeof( DWORD_PTR ) ) ;
		if( usesublistflagtable[code] == 1 )
		{
			list = (LZ_LIST *)( (DWORD_PTR *)list->next + sp[2] ) ;
		}
		else
		{
			if( sublistnum < sublistmaxnum )
			{
				list->next = (LZ_LIST *)( sublistbuf + sizeof( DWORD_PTR ) * 256 * sublistnum ) ;
				list       = (LZ_LIST *)( (DWORD_PTR *)list->next + sp[2] ) ;
			
				usesublistflagtable[code] = 1 ;
				sublistnum ++ ;
			}
		}

		// 一番一致長の長いコードを探す
		maxconbo   = -1 ;
		maxaddress = -1 ;
		maxbonus   = -1 ;
		for( m = 0, listtemp = list->next ; /*m < MAX_SEARCHLISTNUM &&*/ listtemp != NULL ; listtemp = listtemp->next, m ++ )
		{
			address = ( int )( srcaddress - listtemp->address ) ;
			if( address >= MAX_POSITION )
			{
				if( listtemp->prev ) listtemp->prev->next = listtemp->next ;
				if( listtemp->next ) listtemp->next->prev = listtemp->prev ;
				listtemp->address = 0xffffffff ;
				continue ;
			}
			
			sp2 = &sp[-address] ;
			sp1 = sp ;
			if( srcaddress + MAX_COPYSIZE < SrcSize )
			{
				conbo = MAX_COPYSIZE / 4 ;
				while( conbo && *((DWORD *)sp2) == *((DWORD *)sp1) )
				{
					sp2 += 4 ;
					sp1 += 4 ;
					conbo -- ;
				}
				conbo = MAX_COPYSIZE - ( MAX_COPYSIZE / 4 - conbo ) * 4 ;

				while( conbo && *sp2 == *sp1 )
				{
					sp2 ++ ;
					sp1 ++ ;
					conbo -- ;
				}
				conbo = MAX_COPYSIZE - conbo ;
			}
			else
			{
				for( conbo = 0 ;
						conbo < MAX_COPYSIZE &&
						conbo + srcaddress < SrcSize &&
						sp[conbo - address] == sp[conbo] ;
							conbo ++ ){}
			}

			if( conbo >= 4 )
			{
				conbosize   = ( conbo - MIN_COMPRESS ) < 0x20 ? 0 : 1 ;
				addresssize = address < 0x100 ? 0 : ( address < 0x10000 ? 1 : 2 ) ;
				bonus       = conbo - ( 3 + conbosize + addresssize ) ;

				if( bonus > maxbonus )
				{
					maxconbo       = conbo ;
					maxaddress     = address ;
					maxaddresssize = addresssize ;
					maxconbosize   = conbosize ;
					maxbonus       = bonus ;
				}
			}
		}

		// リストに登録
		newlist = &listbuf[listaddp] ;
		if( newlist->address != 0xffffffff )
		{
			if( newlist->prev ) newlist->prev->next = newlist->next ;
			if( newlist->next ) newlist->next->prev = newlist->prev ;
			newlist->address = 0xffffffff ;
		}
		newlist->address = srcaddress ;
		newlist->prev    = list ;
		newlist->next    = list->next ;
		if( list->next != NULL ) list->next->prev = newlist ;
		list->next       = newlist ;
		listaddp         = ( listaddp + 1 ) & maxlistnummask ;

		// 一致コードが見つからなかったら非圧縮コードとして出力
		if( maxconbo == -1 )
		{
NOENCODE:
			// キーコードだった場合は２回連続で出力する
			if( *sp == keycode )
			{
				if( destp != NULL )
				{
					dp[0]  =
					dp[1]  = keycode ;
					dp += 2 ;
				}
				dstsize += 2 ;
			}
			else
			{
				if( destp != NULL )
				{
					*dp = *sp ;
					dp ++ ;
				}
				dstsize ++ ;
			}
			sp ++ ;
			srcaddress ++ ;
		}
		else
		{
			// 見つかった場合は見つけた位置と長さを出力する
			
			// キーコードと見つけた位置と長さを出力
			if( destp != NULL )
			{
				// キーコードの出力
				*dp++ = keycode ;

				// 出力する連続長は最低 MIN_COMPRESS あることが前提なので - MIN_COMPRESS したものを出力する
				maxconbo -= MIN_COMPRESS ;

				// 連続長０～４ビットと連続長、相対アドレスのビット長を出力
				*dp = (BYTE)( ( ( maxconbo & 0x1f ) << 3 ) | ( maxconbosize << 2 ) | maxaddresssize ) ;

				// キーコードの連続はキーコードと値の等しい非圧縮コードと
				// 判断するため、キーコードの値以上の場合は値を＋１する
				if( *dp >= keycode ) dp[0] += 1 ;
				dp ++ ;

				// 連続長５～１２ビットを出力
				if( maxconbosize == 1 )
					*dp++ = (BYTE)( ( maxconbo >> 5 ) & 0xff ) ;

				// maxconbo はまだ使うため - MIN_COMPRESS した分を戻す
				maxconbo += MIN_COMPRESS ;

				// 出力する相対アドレスは０が( 現在のアドレス－１ )を挿すので、－１したものを出力する
				maxaddress -- ;

				// 相対アドレスを出力
				*dp++ = (BYTE)( maxaddress ) ;
				if( maxaddresssize > 0 )
				{
					*dp++ = (BYTE)( maxaddress >> 8 ) ;
					if( maxaddresssize == 2 )
						*dp++ = (BYTE)( maxaddress >> 16 ) ;
				}
			}
			
			// 出力サイズを加算
			dstsize += 3 + maxaddresssize + maxconbosize ;
			
			// リストに情報を追加
			if( srcaddress + maxconbo < SrcSize )
			{
				sp2 = &sp[1] ;
				for( j = 1 ; j < maxconbo && (DWORD_PTR)&sp2[2] - (DWORD_PTR)srcp < SrcSize ; j ++, sp2 ++ )
				{
					code = *((WORD *)sp2) ;
					list = (LZ_LIST *)( listfirsttable + code * sizeof( DWORD_PTR ) ) ;
					if( usesublistflagtable[code] == 1 )
					{
						list = (LZ_LIST *)( (DWORD_PTR *)list->next + sp2[2] ) ;
					}
					else
					{
						if( sublistnum < sublistmaxnum )
						{
							list->next = (LZ_LIST *)( sublistbuf + sizeof( DWORD_PTR ) * 256 * sublistnum ) ;
							list       = (LZ_LIST *)( (DWORD_PTR *)list->next + sp2[2] ) ;
						
							usesublistflagtable[code] = 1 ;
							sublistnum ++ ;
						}
					}

					newlist = &listbuf[listaddp] ;
					if( newlist->address != 0xffffffff )
					{
						if( newlist->prev ) newlist->prev->next = newlist->next ;
						if( newlist->next ) newlist->next->prev = newlist->prev ;
						newlist->address = 0xffffffff ;
					}
					newlist->address = srcaddress + j ;
					newlist->prev = list ;
					newlist->next = list->next ;
					if( list->next != NULL ) list->next->prev = newlist ;
					list->next = newlist ;
					listaddp = ( listaddp + 1 ) & maxlistnummask ;
				}
			}
			
			sp         += maxconbo ;
			srcaddress += maxconbo ;
		}
	}

	// 圧縮後のデータサイズを保存する
	*((DWORD *)&destp[4]) = ( DWORD )( dstsize + 9 ) ;

	// 確保したメモリの解放
	DXFREE( usesublistflagtable ) ;

	// データのサイズを返す
	return dstsize + 9 ;
}

extern int DXA_Decode( void *Src, void *Dest )
{
	DWORD srcsize, destsize, code, indexsize, keycode, conbo, index = 0 ;
	BYTE *srcp, *destp, *dp, *sp ;

	destp = (BYTE *)Dest ;
	srcp  = (BYTE *)Src ;
	
	// 解凍後のデータサイズを得る
	destsize = *((DWORD *)&srcp[0]) ;

	// 圧縮データのサイズを得る
	srcsize = *((DWORD *)&srcp[4]) - 9 ;

	// キーコード
	keycode = srcp[8] ;
	
	// 出力先がない場合はサイズだけ返す
	if( Dest == NULL )
	{
		return ( int )destsize ;
	}
	
	// 展開開始
	sp  = srcp + 9 ;
	dp  = destp ;
	while( srcsize )
	{
		// キーコードか同かで処理を分岐
		if( sp[0] != keycode )
		{
			// 非圧縮コードの場合はそのまま出力
			*dp = *sp ;
			dp      ++ ;
			sp      ++ ;
			srcsize -- ;
			continue ;
		}
	
		// キーコードが連続していた場合はキーコード自体を出力
		if( sp[1] == keycode )
		{
			*dp = (BYTE)keycode ;
			dp      ++ ;
			sp      += 2 ;
			srcsize -= 2 ;
			
			continue ;
		}

		// 第一バイトを得る
		code = sp[1] ;

		// もしキーコードよりも大きな値だった場合はキーコード
		// とのバッティング防止の為に＋１しているので－１する
		if( code > keycode ) code -- ;

		sp      += 2 ;
		srcsize -= 2 ;

		// 連続長を取得する
		conbo = code >> 3 ;
		if( code & ( 0x1 << 2 ) )
		{
			conbo |= *sp << 5 ;
			sp      ++ ;
			srcsize -- ;
		}
		conbo += MIN_COMPRESS ;	// 保存時に減算した最小圧縮バイト数を足す

		// 参照相対アドレスを取得する
		indexsize = code & 0x3 ;
		switch( indexsize )
		{
		case 0 :
			index = *sp ;
			sp      ++ ;
			srcsize -- ;
			break ;
			
		case 1 :
			index = *((WORD *)sp) ;
			sp      += 2 ;
			srcsize -= 2 ;
			break ;
			
		case 2 :
			index = ( DWORD )( *((WORD *)sp) | ( sp[2] << 16 ) ) ;
			sp      += 3 ;
			srcsize -= 3 ;
			break ;
		}
		index ++ ;		// 保存時に－１しているので＋１する

		// 展開
		if( index < conbo )
		{
			DWORD num ;

			num  = index ;
			while( conbo > num )
			{
				_MEMCPY( dp, dp - num, num ) ;
				dp    += num ;
				conbo -= num ;
				num   += num ;
			}
			if( conbo != 0 )
			{
				_MEMCPY( dp, dp - num, conbo ) ;
				dp += conbo ;
			}
		}
		else
		{
			_MEMCPY( dp, dp - index, conbo ) ;
			dp += conbo ;
		}
	}

	// 解凍後のサイズを返す
	return (int)destsize ;
}

// バイナリデータを半角文字列に変換する( 戻り値:変換後のデータサイズ )
static unsigned char BinToChar128Table[ 128 ] =
{
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40,
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0xA1, 0xA2,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
	0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2,
	0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2,
} ;
static unsigned char Char128ToBinTable[ 256 ] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e,
	0x5f, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e,
	0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
	0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
	0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x4e, 0x4f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c,
	0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c,
	0x7d, 0x7e, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;
extern DWORD BinToChar128( void *Src, DWORD SrcSize, void *Dest )
{
	unsigned int DestSize ;

	DestSize  = SrcSize + ( SrcSize + 6 ) / 7 + 5 ;

	if( Dest != NULL )
	{
		unsigned int PackNum ;
		unsigned int ModNum ;
		unsigned char *DestP ;
		unsigned char *SrcP ;
		unsigned int i ;

		DestP = ( unsigned char * )Dest ;
		SrcP  = ( unsigned char * )&SrcSize ;

		DestP[ 0 ] = BinToChar128Table[                                 ( SrcP[ 0 ] >> 1 ) ] ;
		DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) | ( SrcP[ 1 ] >> 2 ) ] ;
		DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) | ( SrcP[ 2 ] >> 3 ) ] ;
		DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) | ( SrcP[ 3 ] >> 4 ) ] ;
		DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 )                      ] ;

		DestP += 5 ;

		PackNum = SrcSize / 7 ;
		ModNum  = SrcSize - PackNum * 7 ;
		SrcP  = ( unsigned char * )Src ;
		for( i = 0 ; i < PackNum ; i ++ )
		{
			DestP[ 0 ] = BinToChar128Table[                                 ( SrcP[ 0 ] >> 1 ) ] ;
			DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) | ( SrcP[ 1 ] >> 2 ) ] ;
			DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) | ( SrcP[ 2 ] >> 3 ) ] ;
			DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) | ( SrcP[ 3 ] >> 4 ) ] ;
			DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 ) | ( SrcP[ 4 ] >> 5 ) ] ;
			DestP[ 5 ] = BinToChar128Table[ ( ( SrcP[ 4 ] & 0x1f ) << 2 ) | ( SrcP[ 5 ] >> 6 ) ] ;
			DestP[ 6 ] = BinToChar128Table[ ( ( SrcP[ 5 ] & 0x3f ) << 1 ) | ( SrcP[ 6 ] >> 7 ) ] ;
			DestP[ 7 ] = BinToChar128Table[ ( ( SrcP[ 6 ] & 0x7f )      )                      ] ;

			DestP += 8 ;
			SrcP  += 7 ;
		}

		if( ModNum != 0 )
		{
			DestP[ 0 ] = BinToChar128Table[                                 ( SrcP[ 0 ] >> 1 ) ] ;
			if( ModNum == 1 )
			{
				DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) ] ;
			}
			else
			{
				DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) | ( SrcP[ 1 ] >> 2 ) ] ;
				if( ModNum == 2 )
				{
					DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) ] ;
				}
				else
				{
					DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) | ( SrcP[ 2 ] >> 3 ) ] ;
					if( ModNum == 3 )
					{
						DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) ] ;
					}
					else
					{
						DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) | ( SrcP[ 3 ] >> 4 ) ] ;
						if( ModNum == 4 )
						{
							DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 ) ] ;
						}
						else
						{
							DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 ) | ( SrcP[ 4 ] >> 5 ) ] ;
							if( ModNum == 5 )
							{
								DestP[ 5 ] = BinToChar128Table[ ( ( SrcP[ 4 ] & 0x1f ) << 2 ) ] ;
							}
							else
							{
								DestP[ 5 ] = BinToChar128Table[ ( ( SrcP[ 4 ] & 0x1f ) << 2 ) | ( SrcP[ 5 ] >> 6 ) ] ;
								if( ModNum == 6 )
								{
									DestP[ 6 ] = BinToChar128Table[ ( ( SrcP[ 5 ] & 0x3f ) << 1 ) ] ;
								}
								else
								{
									DestP[ 6 ] = BinToChar128Table[ ( ( SrcP[ 5 ] & 0x3f ) << 1 ) | ( SrcP[ 6 ] >> 7 ) ] ;
									DestP[ 7 ] = BinToChar128Table[ ( ( SrcP[ 6 ] & 0x7f )      )                      ] ;
								}
							}
						}
					}
				}
			}
		}
	}

	return DestSize ;
}

// 半角文字列をバイナリデータに変換する( 戻り値:変換後のデータサイズ )
extern DWORD Char128ToBin( void *Src, void *Dest )
{
	unsigned int DestSize ;
	unsigned char *SrcP ;
	unsigned char *DestP ;

	SrcP    = ( unsigned char * )Src ;
	DestP   = ( unsigned char * )&DestSize ;

	DestP[ 0 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 0 ] ] << 1 ) | ( Char128ToBinTable[ SrcP[ 1 ] ] >> 6 ) ) ;
	DestP[ 1 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 1 ] ] << 2 ) | ( Char128ToBinTable[ SrcP[ 2 ] ] >> 5 ) ) ;
	DestP[ 2 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 2 ] ] << 3 ) | ( Char128ToBinTable[ SrcP[ 3 ] ] >> 4 ) ) ;
	DestP[ 3 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 3 ] ] << 4 ) | ( Char128ToBinTable[ SrcP[ 4 ] ] >> 3 ) ) ;

	SrcP += 5 ;

	if( Dest != NULL )
	{
		unsigned int PackNum ;
		unsigned int ModNum ;
		unsigned int i ;

		PackNum = DestSize / 7 ;
		ModNum  = DestSize - PackNum * 7 ;
		DestP = ( unsigned char * )Dest ;
		for( i = 0 ; i < PackNum ; i ++ )
		{
			DestP[ 0 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 0 ] ] << 1 ) | ( Char128ToBinTable[ SrcP[ 1 ] ] >> 6 ) ) ;
			DestP[ 1 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 1 ] ] << 2 ) | ( Char128ToBinTable[ SrcP[ 2 ] ] >> 5 ) ) ;
			DestP[ 2 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 2 ] ] << 3 ) | ( Char128ToBinTable[ SrcP[ 3 ] ] >> 4 ) ) ;
			DestP[ 3 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 3 ] ] << 4 ) | ( Char128ToBinTable[ SrcP[ 4 ] ] >> 3 ) ) ;
			DestP[ 4 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 4 ] ] << 5 ) | ( Char128ToBinTable[ SrcP[ 5 ] ] >> 2 ) ) ;
			DestP[ 5 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 5 ] ] << 6 ) | ( Char128ToBinTable[ SrcP[ 6 ] ] >> 1 ) ) ;
			DestP[ 6 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 6 ] ] << 7 ) | ( Char128ToBinTable[ SrcP[ 7 ] ]      ) ) ;

			DestP += 7 ;
			SrcP  += 8 ;
		}

		if( ModNum != 0 )
		{
			DestP[ 0 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 0 ] ] << 1 ) | ( Char128ToBinTable[ SrcP[ 1 ] ] >> 6 ) ) ;
			if( ModNum > 1 )
			{
				DestP[ 1 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 1 ] ] << 2 ) | ( Char128ToBinTable[ SrcP[ 2 ] ] >> 5 ) ) ;
				if( ModNum > 2 )
				{
					DestP[ 2 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 2 ] ] << 3 ) | ( Char128ToBinTable[ SrcP[ 3 ] ] >> 4 ) ) ;
					if( ModNum > 3 )
					{
						DestP[ 3 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 3 ] ] << 4 ) | ( Char128ToBinTable[ SrcP[ 4 ] ] >> 3 ) ) ;
						if( ModNum > 4 )
						{
							DestP[ 4 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 4 ] ] << 5 ) | ( Char128ToBinTable[ SrcP[ 5 ] ] >> 2 ) ) ;
							if( ModNum > 5 )
							{
								DestP[ 5 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 5 ] ] << 6 ) | ( Char128ToBinTable[ SrcP[ 6 ] ] >> 1 ) ) ;
								if( ModNum > 6 )
								{
									DestP[ 6 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 6 ] ] << 7 ) | ( Char128ToBinTable[ SrcP[ 7 ] ]      ) ) ;
								}
							}
						}
					}
				}
			}
		}
	}

	return DestSize ;
}

// バイナリデータを元に SHA-256 のハッシュ値を計算する( DestBuffer の示すアドレスを先頭に 32byte ハッシュ値が書き込まれます )
#define RROT( a, n )		( DWORD )( ( ( a ) >> ( n ) ) | ( DWORD )( ( a ) << ( 32 - ( n ) ) ) )
#define S0( x )				( RROT( ( x ),  2 ) ^ RROT( ( x ), 13 ) ^ RROT( ( x ),22 ) )
#define S1( x )				( RROT( ( x ),  6 ) ^ RROT( ( x ), 11 ) ^ RROT( ( x ),25 ) )
#define s0( x )				( RROT( ( x ),  7 ) ^ RROT( ( x ), 18 ) ^ ( ( x ) >>  3 ) )
#define s1( x )				( RROT( ( x ), 17 ) ^ RROT( ( x ), 19 ) ^ ( ( x ) >> 10 ) )
#define CH( x, y, z )		( ( ( x ) & ( y ) ) ^ ( ( ~( x ) ) & ( z ) ) )
#define MAJ( x, y, z )		( ( ( x ) & ( y ) ) ^ ( ( x ) & ( z ) ) ^ ( ( y ) & ( z ) ) )

#define LIT_TO_BIG( sp, b )	( ( sp[ 0 + b ] << 24 ) | ( sp[ 1 + b ] << 16 ) | ( sp[ 2 + b ] << 8 ) | sp[ 3 + b ] )
#define W_CALC( i )			( W[ i - 16 ] + s0( W[ i - 15 ] ) + W[ i - 7 ] + s1( W[ i - 2 ] ) )

static DWORD K[ 64 ] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
} ;

__inline void HashSha256_Calc( const BYTE *Src, DWORD *H )
{
	DWORD i ;
	DWORD X[ 8 ] ;
	DWORD W[ 64 ] ;

	W[  0 ] = LIT_TO_BIG( Src, 4 *  0 ) ;		W[  1 ] = LIT_TO_BIG( Src, 4 *  1 ) ;
	W[  2 ] = LIT_TO_BIG( Src, 4 *  2 ) ;		W[  3 ] = LIT_TO_BIG( Src, 4 *  3 ) ;
	W[  4 ] = LIT_TO_BIG( Src, 4 *  4 ) ;		W[  5 ] = LIT_TO_BIG( Src, 4 *  5 ) ;
	W[  6 ] = LIT_TO_BIG( Src, 4 *  6 ) ;		W[  7 ] = LIT_TO_BIG( Src, 4 *  7 ) ;
	W[  8 ] = LIT_TO_BIG( Src, 4 *  8 ) ;		W[  9 ] = LIT_TO_BIG( Src, 4 *  9 ) ;
	W[ 10 ] = LIT_TO_BIG( Src, 4 * 10 ) ;		W[ 11 ] = LIT_TO_BIG( Src, 4 * 11 ) ;
	W[ 12 ] = LIT_TO_BIG( Src, 4 * 12 ) ;		W[ 13 ] = LIT_TO_BIG( Src, 4 * 13 ) ;
	W[ 14 ] = LIT_TO_BIG( Src, 4 * 14 ) ;		W[ 15 ] = LIT_TO_BIG( Src, 4 * 15 ) ;

	W[ 16 ] = W_CALC( 16 ) ;	W[ 17 ] = W_CALC( 17 ) ;	W[ 18 ] = W_CALC( 18 ) ;	W[ 19 ] = W_CALC( 19 ) ;
	W[ 20 ] = W_CALC( 20 ) ;	W[ 21 ] = W_CALC( 21 ) ;	W[ 22 ] = W_CALC( 22 ) ;	W[ 23 ] = W_CALC( 23 ) ;
	W[ 24 ] = W_CALC( 24 ) ;	W[ 25 ] = W_CALC( 25 ) ;	W[ 26 ] = W_CALC( 26 ) ;	W[ 27 ] = W_CALC( 27 ) ;
	W[ 28 ] = W_CALC( 28 ) ;	W[ 29 ] = W_CALC( 29 ) ;	W[ 30 ] = W_CALC( 30 ) ;	W[ 31 ] = W_CALC( 31 ) ;
	W[ 32 ] = W_CALC( 32 ) ;	W[ 33 ] = W_CALC( 33 ) ;	W[ 34 ] = W_CALC( 34 ) ;	W[ 35 ] = W_CALC( 35 ) ;
	W[ 36 ] = W_CALC( 36 ) ;	W[ 37 ] = W_CALC( 37 ) ;	W[ 38 ] = W_CALC( 38 ) ;	W[ 39 ] = W_CALC( 39 ) ;
	W[ 40 ] = W_CALC( 40 ) ;	W[ 41 ] = W_CALC( 41 ) ;	W[ 42 ] = W_CALC( 42 ) ;	W[ 43 ] = W_CALC( 43 ) ;
	W[ 44 ] = W_CALC( 44 ) ;	W[ 45 ] = W_CALC( 45 ) ;	W[ 46 ] = W_CALC( 46 ) ;	W[ 47 ] = W_CALC( 47 ) ;
	W[ 48 ] = W_CALC( 48 ) ;	W[ 49 ] = W_CALC( 49 ) ;	W[ 50 ] = W_CALC( 50 ) ;	W[ 51 ] = W_CALC( 51 ) ;
	W[ 52 ] = W_CALC( 52 ) ;	W[ 53 ] = W_CALC( 53 ) ;	W[ 54 ] = W_CALC( 54 ) ;	W[ 55 ] = W_CALC( 55 ) ;
	W[ 56 ] = W_CALC( 56 ) ;	W[ 57 ] = W_CALC( 57 ) ;	W[ 58 ] = W_CALC( 58 ) ;	W[ 59 ] = W_CALC( 59 ) ;
	W[ 60 ] = W_CALC( 60 ) ;	W[ 61 ] = W_CALC( 61 ) ;	W[ 62 ] = W_CALC( 62 ) ;	W[ 63 ] = W_CALC( 63 ) ;

	X[ 0 ] = H[ 0 ] ;
	X[ 1 ] = H[ 1 ] ;
	X[ 2 ] = H[ 2 ] ;
	X[ 3 ] = H[ 3 ] ;
	X[ 4 ] = H[ 4 ] ;
	X[ 5 ] = H[ 5 ] ;
	X[ 6 ] = H[ 6 ] ;
	X[ 7 ] = H[ 7 ] ;
#if 0
	for( i = 0 ; i < 64 ; i ++ )
	{
		DWORD temp1 = X[ 7 ] + S1( X[ 4 ] ) + CH( X[ 4 ], X[ 5 ], X[ 6 ] ) + K[ i ] + W[ i ] ;
		DWORD temp2 = S0( X[ 0 ] ) + MAJ( X[ 0 ], X[ 1 ], X[ 2 ] ) ;
		X[ 7 ] = X[ 6 ] ;
		X[ 6 ] = X[ 5 ] ;
		X[ 5 ] = X[ 4 ] ;
		X[ 4 ] = X[ 3 ] + temp1 ;
		X[ 3 ] = X[ 2 ] ;
		X[ 2 ] = X[ 1 ] ;
		X[ 1 ] = X[ 0 ] ;
		X[ 0 ] = temp1 + temp2 ;
	}
#else
	for( i = 0 ; i < 64 ; i += 8 )
	{
		DWORD temp1 = X[ 7 ] + S1( X[ 4 ] ) + CH( X[ 4 ], X[ 5 ], X[ 6 ] ) + K[ i ] + W[ i ] ;
		DWORD temp2 = S0( X[ 0 ] ) + MAJ( X[ 0 ], X[ 1 ], X[ 2 ] ) ;
//		X[ 7 ] = X[ 6 ] ;
//		X[ 6 ] = X[ 5 ] ;
//		X[ 5 ] = X[ 4 ] ;
//		X[ 4 ] = X[ 3 ] + temp1 ;
//		X[ 3 ] = X[ 2 ] ;
//		X[ 2 ] = X[ 1 ] ;
//		X[ 1 ] = X[ 0 ] ;
//		X[ 0 ] = temp1 + temp2 ;
		DWORD temp3 = X[ 6 ] + S1( X[ 3 ] + temp1 ) + CH( X[ 3 ] + temp1, X[ 4 ], X[ 5 ] ) + K[ i + 1 ] + W[ i + 1 ] ;
		DWORD temp4 = S0( temp1 + temp2 ) + MAJ( temp1 + temp2, X[ 0 ], X[ 1 ] ) ;
//		X[ 7 ] = X[ 5 ] ;
//		X[ 6 ] = X[ 4 ] ;
//		X[ 5 ] = X[ 3 ] + temp1 ;
//		X[ 4 ] = X[ 2 ] + temp3 ;
//		X[ 3 ] = X[ 1 ] ;
//		X[ 2 ] = X[ 0 ] ;
//		X[ 1 ] = temp1 + temp2 ;
//		X[ 0 ] = temp3 + temp4 ;
		DWORD temp5 = X[ 5 ] + S1( X[ 2 ] + temp3 ) + CH( X[ 2 ] + temp3, X[ 3 ] + temp1, X[ 4 ] ) + K[ i + 2 ] + W[ i + 2 ] ;
		DWORD temp6 = S0( temp3 + temp4 ) + MAJ( temp3 + temp4, temp1 + temp2, X[ 0 ] ) ;
//		X[ 7 ] = X[ 4 ] ;
//		X[ 6 ] = X[ 3 ] + temp1 ;
//		X[ 5 ] = X[ 2 ] + temp3 ;
//		X[ 4 ] = X[ 1 ] + temp5 ;
//		X[ 3 ] = X[ 0 ] ;
//		X[ 2 ] = temp1 + temp2 ;
//		X[ 1 ] = temp3 + temp4 ;
//		X[ 0 ] = temp5 + temp6 ;
		DWORD temp7 = X[ 4 ] + S1( X[ 1 ] + temp5 ) + CH( X[ 1 ] + temp5, X[ 2 ] + temp3, X[ 3 ] + temp1 ) + K[ i + 3 ] + W[ i + 3 ] ;
		DWORD temp8 = S0( temp5 + temp6 ) + MAJ( temp5 + temp6, temp3 + temp4, temp1 + temp2 ) ;
//		X[ 7 ] = X[ 3 ] + temp1 ;
//		X[ 6 ] = X[ 2 ] + temp3 ;
//		X[ 5 ] = X[ 1 ] + temp5 ;
//		X[ 4 ] = X[ 0 ] + temp7 ;
//		X[ 3 ] = temp1 + temp2 ;
//		X[ 2 ] = temp3 + temp4 ;
//		X[ 1 ] = temp5 + temp6 ;
//		X[ 0 ] = temp7 + temp8 ;
		DWORD temp9 = X[ 3 ] + temp1 + S1( X[ 0 ] + temp7 ) + CH( X[ 0 ] + temp7, X[ 1 ] + temp5, X[ 2 ] + temp3 ) + K[ i + 4 ] + W[ i + 4 ] ;
		DWORD temp10 = S0( temp7 + temp8 ) + MAJ( temp7 + temp8, temp5 + temp6, temp3 + temp4 ) ;
//		X[ 7 ] = X[ 2 ] + temp3 ;
//		X[ 6 ] = X[ 1 ] + temp5 ;
//		X[ 5 ] = X[ 0 ] + temp7 ;
//		X[ 4 ] = temp1 + temp2 + temp9 ;
//		X[ 3 ] = temp3 + temp4 ;
//		X[ 2 ] = temp5 + temp6 ;
//		X[ 1 ] = temp7 + temp8 ;
//		X[ 0 ] = temp9 + temp10 ;
		DWORD temp11 = X[ 2 ] + temp3 + S1( temp1 + temp2 + temp9 ) + CH( temp1 + temp2 + temp9, X[ 0 ] + temp7, X[ 1 ] + temp5 ) + K[ i + 5 ] + W[ i + 5 ] ;
		DWORD temp12 = S0( temp9 + temp10 ) + MAJ( temp9 + temp10, temp7 + temp8, temp5 + temp6 ) ;
//		X[ 7 ] = X[ 1 ] + temp5 ;
//		X[ 6 ] = X[ 0 ] + temp7 ;
//		X[ 5 ] = temp1 + temp2 + temp9 ;
//		X[ 4 ] = temp3 + temp4 + temp11 ;
//		X[ 3 ] = temp5 + temp6 ;
//		X[ 2 ] = temp7 + temp8 ;
//		X[ 1 ] = temp9 + temp10 ;
//		X[ 0 ] = temp11 + temp12 ;
		DWORD temp13 = X[ 1 ] + temp5 + S1( temp3 + temp4 + temp11 ) + CH( temp3 + temp4 + temp11, temp1 + temp2 + temp9, X[ 0 ] + temp7 ) + K[ i + 6 ] + W[ i + 6 ] ;
		DWORD temp14 = S0( temp11 + temp12 ) + MAJ( temp11 + temp12, temp9 + temp10, temp7 + temp8 ) ;
//		X[ 7 ] = X[ 0 ] + temp7 ;
//		X[ 6 ] = temp1 + temp2 + temp9 ;
//		X[ 5 ] = temp3 + temp4 + temp11 ;
//		X[ 4 ] = temp5 + temp6 + temp13 ;
//		X[ 3 ] = temp7 + temp8 ;
//		X[ 2 ] = temp9 + temp10 ;
//		X[ 1 ] = temp11 + temp12 ;
//		X[ 0 ] = temp13 + temp14 ;
		DWORD temp15 = X[ 0 ] + temp7 + S1( temp5 + temp6 + temp13 ) + CH( temp5 + temp6 + temp13, temp3 + temp4 + temp11, temp1 + temp2 + temp9 ) + K[ i + 7 ] + W[ i + 7 ] ;
		DWORD temp16 = S0( temp13 + temp14 ) + MAJ( temp13 + temp14, temp11 + temp12, temp9 + temp10 ) ;
		X[ 7 ] = temp1 + temp2 + temp9  ;
		X[ 6 ] = temp3 + temp4 + temp11 ;
		X[ 5 ] = temp5 + temp6 + temp13 ;
		X[ 4 ] = temp7 + temp8 + temp15 ;
		X[ 3 ] = temp9 + temp10  ;
		X[ 2 ] = temp11 + temp12 ;
		X[ 1 ] = temp13 + temp14 ;
		X[ 0 ] = temp15 + temp16 ;
	}
#endif
	H[ 0 ] += X[ 0 ] ;
	H[ 1 ] += X[ 1 ] ;
	H[ 2 ] += X[ 2 ] ;
	H[ 3 ] += X[ 3 ] ;
	H[ 4 ] += X[ 4 ] ;
	H[ 5 ] += X[ 5 ] ;
	H[ 6 ] += X[ 6 ] ;
	H[ 7 ] += X[ 7 ] ;
}

extern void HashSha256( const void *SrcData, size_t SrcDataSize, void *DestBuffer )
{
	DWORD H[ 8 ] ;
	BYTE Buffer[ 128 ] ;
	const BYTE *sp = ( const BYTE * )SrcData ;
	size_t i ;
	size_t FullBlockNum = SrcDataSize / 64 ;
	size_t DataSize ;
	size_t FillSize ;
	ULONGLONG BitLength = SrcDataSize * 8 ;

	H[ 0 ] = 0x6a09e667 ;
	H[ 1 ] = 0xbb67ae85 ;
	H[ 2 ] = 0x3c6ef372 ;
	H[ 3 ] = 0xa54ff53a ;
	H[ 4 ] = 0x510e527f ;
	H[ 5 ] = 0x9b05688c ;
	H[ 6 ] = 0x1f83d9ab ;
	H[ 7 ] = 0x5be0cd19 ;

	for( i = 0 ; i < FullBlockNum ; i ++ )
	{
		HashSha256_Calc( sp, H ) ;
		sp += 64 ;
	}

	DataSize = SrcDataSize - FullBlockNum * 64 ;
	for( i = 0 ; i < DataSize ; i ++ )
	{
		Buffer[ i ] = sp[ i ] ;
	}
	Buffer[ DataSize ] = 0x80 ;

	if( DataSize + 1 + 8 > 64 )
	{
		FillSize = ( 128 - 8 ) - ( DataSize + 1 ) ;
		for( i = 0 ; i < FillSize ; i ++ )
		{
			Buffer[ DataSize + 1 + i ] = 0 ;
		}

		Buffer[ 127 ] = ( ( BYTE * )&BitLength )[ 0 ] ;
		Buffer[ 126 ] = ( ( BYTE * )&BitLength )[ 1 ] ;
		Buffer[ 125 ] = ( ( BYTE * )&BitLength )[ 2 ] ;
		Buffer[ 124 ] = ( ( BYTE * )&BitLength )[ 3 ] ;
		Buffer[ 123 ] = ( ( BYTE * )&BitLength )[ 4 ] ;
		Buffer[ 122 ] = ( ( BYTE * )&BitLength )[ 5 ] ;
		Buffer[ 121 ] = ( ( BYTE * )&BitLength )[ 6 ] ;
		Buffer[ 120 ] = ( ( BYTE * )&BitLength )[ 7 ] ;

		HashSha256_Calc( &Buffer[  0 ], H ) ;
		HashSha256_Calc( &Buffer[ 64 ], H ) ;
	}
	else
	{
		FillSize = ( 64 - 8 ) - ( DataSize + 1 ) ;
		for( i = 0 ; i < FillSize ; i ++ )
		{
			Buffer[ DataSize + 1 + i ] = 0 ;
		}

		Buffer[ 63 ] = ( ( BYTE * )&BitLength )[ 0 ] ;
		Buffer[ 62 ] = ( ( BYTE * )&BitLength )[ 1 ] ;
		Buffer[ 61 ] = ( ( BYTE * )&BitLength )[ 2 ] ;
		Buffer[ 60 ] = ( ( BYTE * )&BitLength )[ 3 ] ;
		Buffer[ 59 ] = ( ( BYTE * )&BitLength )[ 4 ] ;
		Buffer[ 58 ] = ( ( BYTE * )&BitLength )[ 5 ] ;
		Buffer[ 57 ] = ( ( BYTE * )&BitLength )[ 6 ] ;
		Buffer[ 56 ] = ( ( BYTE * )&BitLength )[ 7 ] ;

		HashSha256_Calc( &Buffer[  0 ], H ) ;
	}

	( ( BYTE * )DestBuffer )[  0 ] = ( ( BYTE * )H )[ 3 ] ;
	( ( BYTE * )DestBuffer )[  1 ] = ( ( BYTE * )H )[ 2 ] ;
	( ( BYTE * )DestBuffer )[  2 ] = ( ( BYTE * )H )[ 1 ] ;
	( ( BYTE * )DestBuffer )[  3 ] = ( ( BYTE * )H )[ 0 ] ;
	( ( BYTE * )DestBuffer )[  4 ] = ( ( BYTE * )H )[ 7 ] ;
	( ( BYTE * )DestBuffer )[  5 ] = ( ( BYTE * )H )[ 6 ] ;
	( ( BYTE * )DestBuffer )[  6 ] = ( ( BYTE * )H )[ 5 ] ;
	( ( BYTE * )DestBuffer )[  7 ] = ( ( BYTE * )H )[ 4 ] ;
	( ( BYTE * )DestBuffer )[  8 ] = ( ( BYTE * )H )[ 11 ] ;
	( ( BYTE * )DestBuffer )[  9 ] = ( ( BYTE * )H )[ 10 ] ;
	( ( BYTE * )DestBuffer )[ 10 ] = ( ( BYTE * )H )[ 9 ] ;
	( ( BYTE * )DestBuffer )[ 11 ] = ( ( BYTE * )H )[ 8 ] ;
	( ( BYTE * )DestBuffer )[ 12 ] = ( ( BYTE * )H )[ 15 ] ;
	( ( BYTE * )DestBuffer )[ 13 ] = ( ( BYTE * )H )[ 14 ] ;
	( ( BYTE * )DestBuffer )[ 14 ] = ( ( BYTE * )H )[ 13 ] ;
	( ( BYTE * )DestBuffer )[ 15 ] = ( ( BYTE * )H )[ 12 ] ;
	( ( BYTE * )DestBuffer )[ 16 ] = ( ( BYTE * )H )[ 19 ] ;
	( ( BYTE * )DestBuffer )[ 17 ] = ( ( BYTE * )H )[ 18 ] ;
	( ( BYTE * )DestBuffer )[ 18 ] = ( ( BYTE * )H )[ 17 ] ;
	( ( BYTE * )DestBuffer )[ 19 ] = ( ( BYTE * )H )[ 16 ] ;
	( ( BYTE * )DestBuffer )[ 20 ] = ( ( BYTE * )H )[ 23 ] ;
	( ( BYTE * )DestBuffer )[ 21 ] = ( ( BYTE * )H )[ 22 ] ;
	( ( BYTE * )DestBuffer )[ 22 ] = ( ( BYTE * )H )[ 21 ] ;
	( ( BYTE * )DestBuffer )[ 23 ] = ( ( BYTE * )H )[ 20 ] ;
	( ( BYTE * )DestBuffer )[ 24 ] = ( ( BYTE * )H )[ 27 ] ;
	( ( BYTE * )DestBuffer )[ 25 ] = ( ( BYTE * )H )[ 26 ] ;
	( ( BYTE * )DestBuffer )[ 26 ] = ( ( BYTE * )H )[ 25 ] ;
	( ( BYTE * )DestBuffer )[ 27 ] = ( ( BYTE * )H )[ 24 ] ;
	( ( BYTE * )DestBuffer )[ 28 ] = ( ( BYTE * )H )[ 31 ] ;
	( ( BYTE * )DestBuffer )[ 29 ] = ( ( BYTE * )H )[ 30 ] ;
	( ( BYTE * )DestBuffer )[ 30 ] = ( ( BYTE * )H )[ 29 ] ;
	( ( BYTE * )DestBuffer )[ 31 ] = ( ( BYTE * )H )[ 28 ] ;
}


// バイナリデータをBase64文字列に変換する( 戻り値:変換後のデータサイズ )
static unsigned char BinToBase64Table[64] =
{
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,	0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
	0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,	0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
	0x77, 0x78, 0x79, 0x7A, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2B, 0x2F
};
static unsigned char Base64ToBinTable[256] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
	0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
extern DWORD BinToBase64(void* Src, unsigned int SrcSize, void* Dest)
{
	unsigned int DestSize;
	unsigned int ModNum;
	unsigned int PackNum;

	PackNum = SrcSize / 3;
	ModNum = SrcSize - PackNum * 3;
	DestSize = PackNum * 4 + (ModNum > 0 ? ModNum + 1 : 0) + 6;

	if (Dest != NULL)
	{
		unsigned char* DestP;
		unsigned char* SrcP;
		unsigned int i;

		DestP = (unsigned char*)Dest;
		SrcP = (unsigned char*)&SrcSize;

		DestP[0] = BinToBase64Table[(SrcP[0] >> 2)];
		DestP[1] = BinToBase64Table[((SrcP[0] & 0x03) << 4) | (SrcP[1] >> 4)];
		DestP[2] = BinToBase64Table[((SrcP[1] & 0x0f) << 2) | (SrcP[2] >> 6)];
		DestP[3] = BinToBase64Table[((SrcP[2] & 0x3f))];
		DestP[4] = BinToBase64Table[(SrcP[3] >> 2)];
		DestP[5] = BinToBase64Table[((SrcP[3] & 0x03) << 4)];

		DestP += 6;

		SrcP = (unsigned char*)Src;
		for (i = 0; i < PackNum; i++)
		{
			DestP[0] = BinToBase64Table[(SrcP[0] >> 2)];
			DestP[1] = BinToBase64Table[((SrcP[0] & 0x03) << 4) | (SrcP[1] >> 4)];
			DestP[2] = BinToBase64Table[((SrcP[1] & 0x0f) << 2) | (SrcP[2] >> 6)];
			DestP[3] = BinToBase64Table[((SrcP[2] & 0x3f))];

			DestP += 4;
			SrcP += 3;
		}

		if (ModNum != 0)
		{
			DestP[0] = BinToBase64Table[(SrcP[0] >> 2)];
			if (ModNum == 1)
			{
				DestP[1] = BinToBase64Table[((SrcP[0] & 0x03) << 4)];
				DestP += 2;
			}
			else
			{
				DestP[1] = BinToBase64Table[((SrcP[0] & 0x03) << 4) | (SrcP[1] >> 4)];
				DestP[2] = BinToBase64Table[((SrcP[1] & 0x0f) << 2)];
				DestP += 3;
			}
		}
	}

	return DestSize;
}

// Base64文字列をバイナリデータに変換する( 戻り値:変換後のデータサイズ )
extern DWORD Base64ToBin(void* Src, void* Dest)
{
	unsigned int DestSize;
	unsigned char* SrcP;
	unsigned char* DestP;

	SrcP = (unsigned char*)Src;
	DestP = (unsigned char*)&DestSize;

	DestP[0] = (Base64ToBinTable[SrcP[0]] << 2) | (Base64ToBinTable[SrcP[1]] >> 4);
	DestP[1] = (Base64ToBinTable[SrcP[1]] << 4) | (Base64ToBinTable[SrcP[2]] >> 2);
	DestP[2] = (Base64ToBinTable[SrcP[2]] << 6) | (Base64ToBinTable[SrcP[3]]);
	DestP[3] = (Base64ToBinTable[SrcP[4]] << 2) | (Base64ToBinTable[SrcP[5]] >> 4);

	SrcP += 6;

	if (Dest != NULL)
	{
		unsigned int PackNum;
		unsigned int ModNum;
		unsigned int i;

		PackNum = DestSize / 3;
		ModNum = DestSize - PackNum * 3;
		DestP = (unsigned char*)Dest;
		for (i = 0; i < PackNum; i++)
		{
			DestP[0] = (Base64ToBinTable[SrcP[0]] << 2) | (Base64ToBinTable[SrcP[1]] >> 4);
			DestP[1] = (Base64ToBinTable[SrcP[1]] << 4) | (Base64ToBinTable[SrcP[2]] >> 2);
			DestP[2] = (Base64ToBinTable[SrcP[2]] << 6) | (Base64ToBinTable[SrcP[3]]);

			DestP += 3;
			SrcP += 4;
		}

		if (ModNum != 0)
		{
			DestP[0] = (Base64ToBinTable[SrcP[0]] << 2) | (Base64ToBinTable[SrcP[1]] >> 4);
			if (ModNum > 1)
			{
				DestP[1] = (Base64ToBinTable[SrcP[1]] << 4) | (Base64ToBinTable[SrcP[2]] >> 2);
				if (ModNum > 2)
				{
					DestP[2] = (Base64ToBinTable[SrcP[2]] << 6) | (Base64ToBinTable[SrcP[3]]);
				}
			}
		}
	}

	return DestSize;
}

static DWORD CRC32Table[ 256 ] ;
static int CRC32TableInit = 0 ;

// バイナリデータを元に CRC32 のハッシュ値を計算する
extern DWORD HashCRC32( const void *SrcData, size_t SrcDataSize )
{
	DWORD CRC = 0xffffffff ;
	BYTE *SrcByte = ( BYTE * )SrcData ;
	DWORD i ;

	// テーブルが初期化されていなかったら初期化する
	if( CRC32TableInit == 0 )
	{
		DWORD Magic = 0xedb88320 ;	// 0x4c11db7 をビットレベルで順番を逆にしたものが 0xedb88320
		DWORD j ;

		for( i = 0; i < 256; i++ )
		{
			DWORD Data = i ;
			for( j = 0; j < 8; j++ )
			{
				int b = ( Data & 1 ) ;
				Data >>= 1 ;
				if( b != 0 )
				{
					Data ^= Magic ;
				}
			}
			CRC32Table[ i ] = Data ;
		}

		// テーブルを初期化したフラグを立てる
		CRC32TableInit = 1 ;
	}

	for( i = 0 ; i < SrcDataSize ; i ++ )
	{
		CRC = CRC32Table[ ( BYTE )( CRC ^ SrcByte[ i ] ) ] ^ ( CRC >> 8 ) ;
	}

	return CRC ^ 0xffffffff ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


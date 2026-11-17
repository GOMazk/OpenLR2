//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Android用ファイル関係プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

// インクルード ---------------------------------------------------------------
#include "../DxFile.h"
#include "../DxLog.h"
#include "../DxChar.h"
#include "../DxBaseFunc.h"
#include "../DxMemory.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

#define MAX_PATH_LENGTH		1024
#define NUM_OPS				1024
#define MAX_FH_NUM			1024
#define MAX_DH_NUM			256
#define CHUNK_SIZE			2048

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

// パスの始まりが / や \\ だった場合、ルートディレクトリを指定したことにするか( 1:ルートディレクトリを指定したことにする  0:ルートディレクトリを指定したことにしない )
int g_EnableRootDirName = 0 ;

// 追加のドライブ名の数
int g_AddDriveNameNum = 3 ;

// 追加のドライブ名
const char * g_AddDriveName[ 16 ] =
{
	"/data/",
	"/mnt/",
	"/storage/",
} ;

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

extern void SetAssetManager( AAssetManager *AManager )
{
	GFileData.PF.AssetManagerP = AManager ;
}

extern void SetInternalAndExternalDataPath( const char *InternalDataPath, const char *ExternalDataPath )
{
	GFileData.PF.InternalDataPath = InternalDataPath ;
	GFileData.PF.InternalDataPathLength = CL_strlen( DX_CHARCODEFORMAT_UTF8, InternalDataPath ) ;
	GFileData.PF.ExternalDataPath = ExternalDataPath ;
	GFileData.PF.ExternalDataPathLength = CL_strlen( DX_CHARCODEFORMAT_UTF8, ExternalDataPath ) ;
}

// ファイルアクセス処理の初期化・終了関数

// ファイルアクセス処理の初期化関数の環境依存の処理を行う関数
extern int InitializeFile_PF( void )
{
	return 0 ;
}

// ファイルアクセス処理の後始末関数の環境依存の処理を行う関数
extern int TerminateFile_PF( void )
{
	return 0 ;
}

// ファイルアクセス関数
static int ANDR_wchar_t_to_utf8_Path( const wchar_t *Path, char *utf8_PathBuffer, size_t utf8_PathBufferBytes )
{
	wchar_t DirPath[ 512 ], FullPath[ 512 ] ;

	// カレントディレクトリの取得
	_WGETCWD( DirPath, sizeof( DirPath ) ) ;

	// フルパスを作成
	ConvertFullPathW_( Path, FullPath, sizeof( FullPath ), DirPath ) ;

	// パス中の \ を / に置換する
	ChangeEnMarkToSlashW_( FullPath ) ;

//	// ドライブ名が大文字になっていたら小文字にする
//	{
//		int i ;
//
//		for( i = 0 ; FullPath[ i ] != '\0' && FullPath[ i ] != L':' ; i ++ )
//		{
//			if( FullPath[ i ] >= L'A' && FullPath[ i ] <= L'Z' )
//			{
//				FullPath[ i ] = FullPath[ i ] - L'A' + L'a' ;
//			}
//		}
//	}

	// wchar_t から UTF-8 に変換
	ConvString( ( const char * )FullPath, -1, WCHAR_T_CHARCODEFORMAT, utf8_PathBuffer, utf8_PathBufferBytes, DX_CHARCODEFORMAT_UTF8 ) ;

	return 0 ;
}

extern int ReadOnlyFileAccessOpen_PF( FILEACCESS *FileAccess, const wchar_t *Path, int UseCacheFlag, int BlockReadFlag )
{
	char FullPathUTF8[ 1024 ] ;

	// wchar_t から UTF-8 に変換
	ANDR_wchar_t_to_utf8_Path( Path, FullPathUTF8, sizeof( FullPathUTF8 ) ) ;

	FileAccess->PF.Asset = NULL ;
	FileAccess->PF.FilePointer = NULL ;

//	// InternalDataPath か ExternalDataPath のディレクトリが指定されている場合は標準入出力を使用する
//	if( ( GFileData.PF.InternalDataPathLength != 0 && CL_strncmp( DX_CHARCODEFORMAT_UTF8, FullPathUTF8, GFileData.PF.InternalDataPath, GFileData.PF.InternalDataPathLength ) == 0 ) ||
//		( GFileData.PF.ExternalDataPathLength != 0 && CL_strncmp( DX_CHARCODEFORMAT_UTF8, FullPathUTF8, GFileData.PF.ExternalDataPath, GFileData.PF.ExternalDataPathLength ) == 0 ) )
	// １文字目が / か \\ だったら標準入出力を使用する
	if( FullPathUTF8[ 0 ] == '/' || FullPathUTF8[ 0 ] == '\\' )
	{
		FileAccess->PF.FilePointer = fopen( FullPathUTF8, "rb" ) ;
		if( FileAccess->PF.FilePointer == NULL )
		{
			return -1 ;
		}

		// ファイルサイズを取得
		fseek( FileAccess->PF.FilePointer, 0, SEEK_END ) ;
		FileAccess->Size = ftell( FileAccess->PF.FilePointer ) ;
		fseek( FileAccess->PF.FilePointer, 0, SEEK_SET ) ;
	}
	else
	{
		// それ以外はアセットマネージャーからファイルを開く
		FileAccess->PF.Asset = AAssetManager_open( GFileData.PF.AssetManagerP, FullPathUTF8, AASSET_MODE_UNKNOWN ) ;
		if( FileAccess->PF.Asset == NULL )
		{
			return -1 ;
		}

		// ファイルサイズを取得
		FileAccess->Size = AAsset_getLength64( FileAccess->PF.Asset ) ;
	}

	// 正常終了
	return 0 ;
}

extern int ReadOnlyFileAccessClose_PF( FILEACCESS *FileAccess )
{
	// ファイルを閉じる
	if( FileAccess->PF.FilePointer != NULL )
	{
		fclose( FileAccess->PF.FilePointer ) ;
		FileAccess->PF.FilePointer = NULL ;
	}
	if( FileAccess->PF.Asset != NULL )
	{
		AAsset_close( FileAccess->PF.Asset ) ;
		FileAccess->PF.Asset = NULL ;
	}

	return 0 ;
}

extern int ReadOnlyFileAccessSeek_PF( FILEACCESS *FileAccess, LONGLONG SeekPoint )
{
	// ファイルアクセス位置を変更する
	if( FileAccess->PF.FilePointer != NULL )
	{
		fseek( FileAccess->PF.FilePointer, SeekPoint, SEEK_SET ) ;
	}
	else
	{
		AAsset_seek64( FileAccess->PF.Asset, SeekPoint, SEEK_SET ) ;
	}

	// 正常終了
	return 0 ;
}

extern size_t ReadOnlyFileAccessRead_PF( void *Buffer, size_t BlockSize, size_t DataNum, FILEACCESS *FileAccess )
{
	int BytesRead ;

	if( BlockSize == 0 || DataNum == 0 )
	{
		return 0 ;
	}

	if( FileAccess->PF.FilePointer != NULL )
	{
		BytesRead = fread( Buffer, BlockSize, DataNum, FileAccess->PF.FilePointer ) * BlockSize ;
	}
	else
	{
		BytesRead = AAsset_read( FileAccess->PF.Asset, Buffer, BlockSize * DataNum ) ;
	}

	return BytesRead ;
}

extern int ReadOnlyFileAccessIdleCheck_PF(	FILEACCESS *FileAccess )
{
	return TRUE ;
}

extern int ReadOnlyFileAccessChDir_PF( const wchar_t *Path )
{
	return _WCHDIR( Path ) ;
}

extern int ReadOnlyFileAccessGetDirS_PF( wchar_t *Buffer, size_t BufferBytes )
{
	_WGETCWD( Buffer, BufferBytes ) ;

	return 0 ;
}

extern int ReadOnlyFileAccessFindFirst_PF(	FINDINFO *FindInfo, const wchar_t *FilePath, FILEINFOW *Buffer )
{
	wchar_t DirPath[ 1024 ] ;
	char DirPathUTF8[ 1024 ] ;

	// ディレクトリパスとファイル名を分ける
	AnalysisFileNameAndDirPathW_( FilePath, FindInfo->PF.SearchFileName, sizeof( FindInfo->PF.SearchFileName ), DirPath, sizeof( DirPath ) ) ;

	// ディレクトリパスを wchar_t から UTF-8 に変換
	ANDR_wchar_t_to_utf8_Path( DirPath, DirPathUTF8, sizeof( DirPathUTF8 ) ) ;

	FindInfo->PF.AssetDir = NULL ;
	FindInfo->PF.Dir = NULL ;

//	// InternalDataPath か ExternalDataPath のディレクトリが指定されている場合は標準入出力を使用する
//	if( CL_strncmp( DX_CHARCODEFORMAT_UTF8, DirPathUTF8, GFileData.PF.InternalDataPath, GFileData.PF.InternalDataPathLength ) == 0 ||
//		CL_strncmp( DX_CHARCODEFORMAT_UTF8, DirPathUTF8, GFileData.PF.ExternalDataPath, GFileData.PF.ExternalDataPathLength ) == 0 )
	// １文字目が / か \\ だったら標準入出力を使用する
	if( DirPathUTF8[ 0 ] == '/' || DirPathUTF8[ 0 ] == '\\' )
	{
		FindInfo->PF.Dir = opendir( DirPathUTF8 ) ;
		if( FindInfo->PF.Dir == NULL )
		{
			return -1 ;
		}
	}
	else
	{
		// アセットマネージャーを使用してディレクトリを開く
		FindInfo->PF.AssetDir = AAssetManager_openDir( GFileData.PF.AssetManagerP, DirPathUTF8 ) ;
		if( FindInfo->PF.AssetDir == NULL )
		{
			return -1 ;
		}
	}

	// 最初のファイルを検索する
	if( ReadOnlyFileAccessFindNext_PF( FindInfo, Buffer ) < 0 )
	{
		if( FindInfo->PF.Dir != NULL )
		{
			closedir( FindInfo->PF.Dir ) ;
			FindInfo->PF.Dir = NULL ;
		}

		if( FindInfo->PF.AssetDir != NULL )
		{
			AAssetDir_close( FindInfo->PF.AssetDir ) ;
			FindInfo->PF.AssetDir = NULL ;
		}

		return -1 ;
	}

	return 0 ;
}

extern int ReadOnlyFileAccessFindNext_PF(	FINDINFO *FindInfo, FILEINFOW *Buffer )
{
	const char *FileName ;
	wchar_t FileNameW[ 1024 ] ;

	if( FindInfo->PF.AssetDir == NULL && FindInfo->PF.Dir == NULL )
	{
		return -1 ;
	}

	if( FindInfo->PF.Dir != NULL )
	{
		for(;;)
		{
			dirent *ret = readdir( FindInfo->PF.Dir ) ;
			if( ret == NULL )
			{
				return -1 ;
			}

			ConvString( ret->d_name, -1, DX_CHARCODEFORMAT_UTF8, ( char * )FileNameW, sizeof( FileNameW ), WCHAR_T_CHARCODEFORMAT ) ;

			if( Strcmp_Str2_WildcardW_( FileNameW, FindInfo->PF.SearchFileName ) == 0 )
			{
				if( Buffer )
				{
					CL_strcpy( WCHAR_T_CHARCODEFORMAT, ( char * )Buffer->Name, ( char * )FileNameW ) ;
					Buffer->DirFlag = ret->d_type == DT_DIR ? TRUE : FALSE ;
					Buffer->Size = 0 ;
					_MEMSET( &Buffer->CreationTime, 0, sizeof( Buffer->CreationTime ) ) ;
					_MEMSET( &Buffer->LastWriteTime, 0, sizeof( Buffer->LastWriteTime ) ) ;
				}

				return 0 ;
			}
		}
	}
	else
	{
		for(;;)
		{
			FileName = AAssetDir_getNextFileName( FindInfo->PF.AssetDir ) ;
			if( FileName == NULL )
			{
				return -1 ;
			}

			ConvString( FileName, -1, DX_CHARCODEFORMAT_UTF8, ( char * )FileNameW, sizeof( FileNameW ), WCHAR_T_CHARCODEFORMAT ) ;

			if( Strcmp_Str2_WildcardW_( FileNameW, FindInfo->PF.SearchFileName ) == 0 )
			{
				if( Buffer )
				{
					CL_strcpy( WCHAR_T_CHARCODEFORMAT, ( char * )Buffer->Name, ( char * )FileNameW ) ;
					Buffer->DirFlag = 0 ;
					Buffer->Size = 0 ;
					_MEMSET( &Buffer->CreationTime, 0, sizeof( Buffer->CreationTime ) ) ;
					_MEMSET( &Buffer->LastWriteTime, 0, sizeof( Buffer->LastWriteTime ) ) ;
				}

				return 0 ;
			}
		}
	}

	return -1 ;
}

extern int ReadOnlyFileAccessFindClose_PF( FINDINFO *FindInfo )
{
	if( FindInfo->PF.Dir != NULL )
	{
		closedir( FindInfo->PF.Dir ) ;
		FindInfo->PF.Dir = NULL ;
	}

	if( FindInfo->PF.AssetDir != NULL )
	{
		AAssetDir_close( FindInfo->PF.AssetDir ) ;
		FindInfo->PF.AssetDir = NULL ;
	}

	return 0 ;
}













// 書き込み専用ファイルアクセス関数
extern	int			WriteOnlyFileAccessDelete_PF( const wchar_t *Path )
{
	char FullPathUTF8[ 1024 ] ;

	// wchar_t から UTF-8 に変換
	ANDR_wchar_t_to_utf8_Path( Path, FullPathUTF8, sizeof( FullPathUTF8 ) ) ;

	remove( FullPathUTF8 ) ;

	return 0 ;
}

extern	DWORD_PTR	WriteOnlyFileAccessOpen_PF(  const wchar_t *Path )
{
	char FullPathUTF8[ 1024 ] ;
	FILE *fp ;

	// wchar_t から UTF-8 に変換
	ANDR_wchar_t_to_utf8_Path( Path, FullPathUTF8, sizeof( FullPathUTF8 ) ) ;

	// ファイルを開く
	fp = fopen( FullPathUTF8, "wb" ) ;
	if( fp == NULL )
	{
		return 0 ;
	}

	return ( DWORD_PTR )fp ;
}

extern	int			WriteOnlyFileAccessClose_PF( DWORD_PTR Handle )
{
	FILE *fp = ( FILE * )Handle ;

	fclose( fp ) ;

	return 0 ;
}

extern	int			WriteOnlyFileAccessSeek_PF(  DWORD_PTR Handle, LONGLONG SeekPoint, int SeekType )
{
	FILE *fp = ( FILE * )Handle ;

	fseek( fp, SeekPoint, SeekType ) ;

	return 0 ;
}

extern	int			WriteOnlyFileAccessWrite_PF( DWORD_PTR Handle, void *Buffer, size_t WriteSize, size_t *GetWriteSize )
{
	FILE *fp = ( FILE * )Handle ;
	size_t lWriteSize ;

	lWriteSize = fwrite( Buffer, 1, WriteSize, fp ) ;

	if( GetWriteSize )
	{
		*GetWriteSize = lWriteSize ;
	}

	return 0 ;
}







#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


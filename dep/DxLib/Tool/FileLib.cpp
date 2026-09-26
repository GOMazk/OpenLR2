// ============================================================================
//
//		ファイル関係のライブラリ
//
//		Creator			: 山田　巧
//		Creation Data	: 09/17/2004
//
// ============================================================================

// include --------------------------------------
#include <string.h>
#include <stdio.h>
#include <mbstring.h>
#include <windows.h>
#include "FileLib.h"

// define ---------------------------------------

#define PATH_LENGTH		(256)			// パスバッファの長さ
#define TEXTCHECKSIZE	(0x2000)		// テキストデータか調べるサイズ

// data type ------------------------------------

// data -----------------------------------------

// function proto type --------------------------

// ファイルオブジェクト列挙用関数( -1:エラー  0以上:ファイルの数 )
// flist は NULL でも良い
static int __EnumObject( char *Path, char *CurrentPath, FILE_INFOLIST *FileList,
							int OmitDirectory, int SubDirectory,
							char **OmitName, char **OmitExName, char **ValidExName ) ;

// function code --------------------------------

// ファイルオブジェクト列挙用関数( -1:エラー  0以上:ファイルの数 )
// flist は NULL でも良い
static int __EnumObject( char *Path, char *CurrentPath, FILE_INFOLIST *FileList,
							int OmitDirectory, int SubDirectory,
							char **OmitName, char **OmitExName, char **ValidExName )
{
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle = INVALID_HANDLE_VALUE ;
	int FileNum, IsDirectory ;
	char RelDir[PATH_LENGTH] ;
	char *AbsDir ;
	int RelDirLen, AbsDirLen, StartNum ;

	// ディレクトリパスの作成
	AbsDir = Path ;
	AbsDirLen = strlen( AbsDir ) ;
	strcpy( RelDir, AbsDir + strlen( CurrentPath ) ) ;
	RelDirLen = strlen( RelDir ) ;

	// ファイルの列挙開始
	{
		char temp[PATH_LENGTH] ;

		strcpy( temp, AbsDir ) ;
		strcat( temp, "*" ) ;
		FindHandle = FindFirstFile( temp, &FindData ) ;
		if( FindHandle == INVALID_HANDLE_VALUE )
			return -1 ;
	}

	// ファイルの情報を列挙する
	FileNum = 0 ;
	if( FileList != NULL ) StartNum = FileList->Num ;
	do
	{
		// 上のフォルダに戻ったりするためのパスは無視する
		if( strcmp( FindData.cFileName, "." ) == 0 || strcmp( FindData.cFileName, ".." ) == 0 ) continue ;

		// ディレクトリかどうかを得る
		IsDirectory = ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? 1 : 0 ;

		// 有効拡張子指定があり、有効拡張子ではない場合は無視する
		if( ValidExName != NULL && IsDirectory == 0 )
		{
			int i ;
			char *name ;

			name = strchr( FindData.cFileName, '.' ) ;
			if( name == NULL ) name = "" ;
			else name ++ ;

			for( i = 0 ; ValidExName[i] != NULL && stricmp( name, ValidExName[i] ) != 0 ; i ++ ){}
			if( ValidExName[i] == NULL ) continue ;
		}

		// 除外指定がある文字列は無視する
		if( OmitName != NULL )
		{
			int i ;

			for( i = 0 ; OmitName[i] != NULL && strcmp( FindData.cFileName, OmitName[i] ) != 0 ; i ++ ){}
			if( OmitName[i] != NULL ) continue ;
		}

		// 除外指定がある拡張子を持つファイルは無視する
		if( OmitExName != NULL && IsDirectory == 0 )
		{
			int i ;
			char *name ;

			name = strchr( FindData.cFileName, '.' ) ;
			if( name == NULL ) name = "" ;
			else name ++ ;

			for( i = 0 ; OmitExName[i] != NULL && strcmp( name, OmitExName[i] ) != 0 ; i ++ ){}
			if( OmitExName[i] != NULL ) continue ;
		}

		// ディレクトリの場合でサブディレクトリも調べる指定がある場合はサブディレクトリ内も列挙する
		if( IsDirectory == 1 && SubDirectory == 1 )
		{
			int res ;
			char tempAbs[PATH_LENGTH], tempRel[PATH_LENGTH] ;

			// 絶対パスの作成
			strcpy( tempAbs, AbsDir ) ;
			strcat( tempAbs, FindData.cFileName ) ;
			strcat( tempAbs, "\\" ) ;

			// 相対パスの作成
			strcpy( tempRel, RelDir ) ;
			strcat( tempRel, FindData.cFileName ) ;
			strcat( tempRel, "\\" ) ;
			
			// 列挙
			res = __EnumObject( tempAbs, CurrentPath, FileList, OmitDirectory, SubDirectory, OmitName, OmitExName, ValidExName ) ;
			if( res < 0 )
				goto ERR ;
			FileNum += res ;
		}

		// データを格納することが出来る場合はデータを格納する
		if( FileList != NULL &&
			( IsDirectory == 0 || ( IsDirectory == 1 && OmitDirectory == 0 ) ) )
		{
			FILE_INFO *info ;
			int FileNameLen ;

			info = &FileList->List[FileList->Num] ;
			memset( info, 0, sizeof( FILE_INFO ) ) ;

			// 時刻を保存
			info->Date.Create		= ( ( ( LONGLONG )FindData.ftCreationTime	.dwHighDateTime ) << 32 ) + FindData.ftCreationTime		.dwLowDateTime ;
			info->Date.LastAccess	= ( ( ( LONGLONG )FindData.ftLastAccessTime	.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime	.dwLowDateTime ;
			info->Date.LastWrite	= ( ( ( LONGLONG )FindData.ftLastWriteTime	.dwHighDateTime ) << 32 ) + FindData.ftLastWriteTime	.dwLowDateTime ;

			info->Size			= FindData.nFileSizeLow ;		// サイズを保存
			info->Attributes	= FindData.dwFileAttributes ;	// 属性を保存
			info->IsDirectory	= (u8)IsDirectory ;

			// パス系の保存
			{
				// パス系を保存するメモリ領域の確保
				FileNameLen = strlen( FindData.cFileName ) ;
				info->FileName = ( char * )malloc( ( FileNameLen + 1 ) + ( AbsDirLen + 1 ) + ( RelDirLen + 1 ) ) ;
				if( info->FileName == NULL )
					goto ERR ;
				info->RelDirectoryPath = info->FileName + FileNameLen + 1 ;
				info->AbsDirectoryPath = info->RelDirectoryPath + RelDirLen + 1 ;

				// コピー
				strcpy( info->FileName, FindData.cFileName ) ;
				strcpy( info->RelDirectoryPath, RelDir ) ;
				strcpy( info->AbsDirectoryPath, AbsDir ) ;
			}

			// ファイルの数を増やす
			FileList->Num ++ ;
		}

		// ファイルの数を増やす
		FileNum ++ ;
	}
	while( FindNextFile( FindHandle, &FindData ) != 0 ) ;

	// 列挙終了
	FindClose( FindHandle ) ;
	FindHandle = INVALID_HANDLE_VALUE ;

	// 終了
	return FileNum ;

	// エラー処理
ERR :
	if( FindHandle != INVALID_HANDLE_VALUE ) FindClose( FindHandle ) ;

	// 既に確保してしまったメモリの解放処理
	if( FileList != NULL )
	{
		int i ;

		for( i = StartNum ; i < FileList->Num ; i ++ )
			if( FileList->List[i].FileName != NULL ) free( FileList->List[i].FileName ) ;
	}

	// エラー終了
	return -1 ;
}

// データを簡易暗号化する関数
extern void EasyEncode( void *Data, unsigned int Size )
{
	if( Size == 0 ) return ;

#ifdef _WIN64
	unsigned int i ;
	BYTE *p = ( BYTE * )Data ;
	for( i = 0 ; i < Size ; i ++ )
	{
		p[ i ] = ( ~p[ i ] << 4 ) | ( ~p[ i ] >> 4 ) ;
	}
#else
	unsigned int tempSize, tempData ;

	tempData = ( unsigned int )Data ;
	tempSize = ( unsigned int )Size ;
	__asm
	{
		// 準備
		mov edi, tempData
		mov ecx, tempSize

		// 値を反転、回転4ビットして書き戻す
LOOP1:
		mov al, [edi]
		not al
		rol al, 4
		mov [edi], al
		inc edi

		// ループ処理
		dec ecx
		jnz LOOP1
	} ;
#endif
}

// データを簡易暗号化してファイルに書き出す関数
extern void EasyEncodeFileWrite( void *Data, int Size, FILE *fp )
{
	EasyEncode( Data, Size ) ;
	fwrite( Data, Size, 1, fp ) ;
	EasyEncode( Data, Size ) ;
}

// データを簡易暗号化してファイルから読み込む関数
extern void EasyEncodeFileRead( void *Data, int Size, FILE *fp )
{
	fread( Data, Size, 1, fp ) ;
	EasyEncode( Data, Size ) ;
}

// ファイルの内容をメモリに読み込む( 0:成功  -1:失敗 )
extern int LoadFileMem( const char *Path, void **DataBuf, int *Size )
{
	FILE *fp = NULL ;
	void *buf = NULL ;
	int size ;

	fp = fopen( Path, "rb" ) ;
	if( fp == NULL ) goto ERR ;

	// サイズを得る
	fseek( fp, 0L, SEEK_END ) ;
	size = ftell( fp ) ;
	fseek( fp, 0L, SEEK_SET ) ;

	// メモリの確保
	buf = malloc( size ) ;
	if( buf == NULL ) goto ERR ;

	// 読み込み
	fread( buf, size, 1, fp ) ;
	fclose( fp ) ;

	// セット
	if( DataBuf != NULL ) *DataBuf = buf ;
	if( Size != NULL ) *Size = size ;

	// 終了
	return 0 ;

ERR :
	if( buf != NULL ) free( buf ) ;
	if( fp != NULL ) fclose( fp ) ;

	return -1 ;
}

// ファイルの内容をメモリに読み込む( 0:成功  -1:失敗 )
extern int LoadFileMem( const char *Path, void *DataBuf, int *Size )
{
	FILE *fp = NULL ;
	int size ;

	fp = fopen( Path, "rb" ) ;
	if( fp == NULL ) return -1;

	// サイズを得る
	fseek( fp, 0L, SEEK_END ) ;
	size = ftell( fp ) ;
	fseek( fp, 0L, SEEK_SET ) ;

	// 読み込み
	if( DataBuf )
		fread( DataBuf, size, 1, fp ) ;
	fclose( fp ) ;

	// セット
	if( Size != NULL ) *Size = size ;

	// 終了
	return 0 ;
}

// メモリの内容をファイルに書き出す 
extern int SaveFileMem( const char *Path, void *Data, int Size )
{
	FILE *fp ;

	fp = fopen( Path, "wb" ) ;
	if( fp == NULL ) return -1 ;
	fwrite( Data, Size, 1, fp ) ;
	fclose( fp ) ;

	return 0 ;
}


// 指定のディレクトリを作成する、中間のディレクトリも存在しない場合は作成する
// 最後尾に '\' があっても無視する
// ドライブ名の後に '\' がない場合は正常に動作しない
extern int __CreateDirectory( const char *Path )
{
	char dir[MAX_PATH] ;

	// フルパスを得る
	ConvertFullPath__( Path, dir, NULL ) ;
	SetEnMark( dir ) ;

	// 指定のディレクトリが存在したら何もせず終了
	{
		WIN32_FIND_DATA FindData ;
		HANDLE FindHandle ;

		FindHandle = FindFirstFile( dir, &FindData ) ;
		if( FindHandle != INVALID_HANDLE_VALUE )
		{
			FindClose( FindHandle ) ;
			return 0 ;
		}
	}

	// ディレクトリを作成するループ
	{
		char *p ;
		p = strchr( dir, '\\' ) ;
		while( p != NULL )
		{
			*p = '\0' ;
			CreateDirectory( dir, NULL ) ;
			*p = '\\' ;

			p = strchr( p + 1, '\\' ) ;
		}
	}

	// 終了
	return 0 ;
}


// 指定のパスが示しているものがディレクトリかどうかを得る
extern int IsDirectory( const char *Path )
{
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle ;
	
	// ファイルの情報を得る
	FindHandle = FindFirstFile( Path, &FindData ) ;
	if( FindHandle == INVALID_HANDLE_VALUE ) return -1 ;
	FindClose( FindHandle ) ;
	
	// ディレクトリかどうかを返す
	return ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? 1 : 0 ;
}

// 指定のパスの情報を得る
extern int CreateFileInfo( const char *Path, FILE_INFO *FileInfoBuffer )
{
	FILE_INFO *info ;
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle = INVALID_HANDLE_VALUE ;
	int AbsDirLen, RelDirLen ;
	char RelDir[1] ;
	char AbsDir[PATH_LENGTH] ;
	
	info = FileInfoBuffer ;
	
	// ディレクトリパスの作成
	{
		ConvertFullPath__( Path, AbsDir ) ;
		DelChr( AbsDir, '\\' ) ;
		strrchr( AbsDir, '\\' )[1] = '\0' ;
		AbsDirLen = strlen( AbsDir ) ;
		RelDir[0] = '\0' ;
		RelDirLen = 0 ;
	}

	// ファイルの情報を得る
	FindHandle = FindFirstFile( Path, &FindData ) ;
	if( FindHandle == INVALID_HANDLE_VALUE ) return -1 ;
	FindClose( FindHandle ) ;
	
	// とりあえず零初期化
	memset( info, 0, sizeof( FILE_INFO ) ) ;

	// 時刻を保存
	info->Date.Create		= ( ( ( LONGLONG )FindData.ftCreationTime	.dwHighDateTime ) << 32 ) + FindData.ftCreationTime		.dwLowDateTime ;
	info->Date.LastAccess	= ( ( ( LONGLONG )FindData.ftLastAccessTime	.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime	.dwLowDateTime ;
	info->Date.LastWrite	= ( ( ( LONGLONG )FindData.ftLastWriteTime	.dwHighDateTime ) << 32 ) + FindData.ftLastWriteTime	.dwLowDateTime ;

	info->Size			= FindData.nFileSizeLow ;		// サイズを保存
	info->Attributes	= FindData.dwFileAttributes ;	// 属性を保存
	info->IsDirectory	= (u8)(( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? 1 : 0) ;	// ディレクトリかどうかを保存

	// パス系の保存
	{
		int FileNameLen ;
	
		// パス系を保存するメモリ領域の確保
		FileNameLen = strlen( FindData.cFileName ) ;
		info->FileName = ( char * )malloc( ( FileNameLen + 1 ) + ( AbsDirLen + 1 ) + ( RelDirLen + 1 ) ) ;
		if( info->FileName == NULL ) return -1 ;
		info->RelDirectoryPath = info->FileName + FileNameLen + 1 ;
		info->AbsDirectoryPath = info->RelDirectoryPath + RelDirLen + 1 ;

		// コピー
		strcpy( info->FileName, FindData.cFileName ) ;
		strcpy( info->RelDirectoryPath, RelDir ) ;
		strcpy( info->AbsDirectoryPath, AbsDir ) ;
	}
	
	// 終了
	return 0 ;
}

// パス情報の後始末を行う
extern int ReleaseFileInfo( FILE_INFO *FileInfo )
{
	if( FileInfo->FileName != NULL ) free( FileInfo->FileName ) ;
	FileInfo->FileName = NULL ;
	
	// 終了
	return 0 ;
}

// 指定のパスのファイルのタイムスタンプを FileInfo に格納されているタイムスタンプにする
extern int SetFileTimeStamp( const char *Path, FILE_INFO *FileInfo )
{
	HANDLE HFile ;
	FILETIME CreateTime, LastAccessTime, LastWriteTime ;

	HFile = CreateFile( Path,
						GENERIC_WRITE, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if( HFile == INVALID_HANDLE_VALUE )
	{
		HFile = HFile ;
	}

	CreateTime.dwHighDateTime     = ( u32 )( FileInfo->Date.Create     >> 32        ) ;
	CreateTime.dwLowDateTime      = ( u32 )( FileInfo->Date.Create     & 0xffffffff ) ;
	LastAccessTime.dwHighDateTime = ( u32 )( FileInfo->Date.LastAccess >> 32        ) ;
	LastAccessTime.dwLowDateTime  = ( u32 )( FileInfo->Date.LastAccess & 0xffffffff ) ;
	LastWriteTime.dwHighDateTime  = ( u32 )( FileInfo->Date.LastWrite  >> 32        ) ;
	LastWriteTime.dwLowDateTime   = ( u32 )( FileInfo->Date.LastWrite  & 0xffffffff ) ;
	SetFileTime( HFile, &CreateTime, &LastAccessTime, &LastWriteTime ) ;
	CloseHandle( HFile ) ;

	// 終了
	return 0;
}

// 二つのファイルのタイムスタンプが一致しているかどうかを得る(0:一致している -1:一致していない)
extern int CmpFileTimeStamp( FILE_INFO *FileInfo1, FILE_INFO *FileInfo2, bool Create, bool LastAccess, bool LastWrite )
{
	if( Create     && FileInfo1->Date.Create     != FileInfo2->Date.Create     ) return -1;
	if( LastAccess && FileInfo1->Date.LastAccess != FileInfo2->Date.LastAccess ) return -1;
	if( LastWrite  && FileInfo1->Date.LastWrite  != FileInfo2->Date.LastWrite  ) return -1;
	return 0;
}

// 指定のディレクトリのファイルリストを取得する
// パス文字列の最後に『\』が無くても常に最後の文字列をディレクトリと判断する
// フルパスではない場合は現在のカレントディレクトリからの相対パスとなる
// FileListInfo が 0 ではない場合使用後 ReleaseFileObjectList を呼ぶ必要がある
// SubDirectory を 1 にするとサブディレクトリ内のファイルも全て列挙する
// NULL 、もしくは文字が一つもない文字列を渡した場合カレントディレクトリのファイルが列挙される
// OmitDirectory を 1 にするとディレクトリは列挙しない
// OmitName に除外したいファイル名を渡すとその文字列を除外してくれる( ';'で複数の文字列を連結可能 )
// OmitExName に除外したい拡張子を渡すとその拡張子を持つファイルを除外してくれる( ';'で複数の文字列を連結可能 )
// ValidExName に有効にしたい拡張子を渡すとその拡張子を持つファイルのみ列挙してくれる( Omit系よりも優先度が高い, ';'で複数の文字列を連結可能 )
// 戻り値 : -1 = エラー  0以上 = 列挙したファイルの数
extern int CreateFileList( const char *DirectoryPath, FILE_INFOLIST *FileListInfo,
							int OmitDirectory, int SubDirectory,
							const char *OmitName, const char *OmitExName, const char *ValidExName )
{
	char DirPath[PATH_LENGTH] ;
	FILE_INFOLIST FileList ;
	char *OmitStrBuf = NULL, *OmitExStrBuf = NULL, *ValidExStrBuf = NULL ;
	char *OmitStr[50], *OmitExStr[50], *ValidExStr[100] ;
	
	// フルパスを得る
	ConvertFullPath__( DirectoryPath, DirPath, NULL ) ;
	
	// ディレクトリではなくファイルだったら此処で処理してしまう
	{
		int res ;
		
		DelChr( DirPath, '\\' ) ;
		
		res = IsDirectory( DirPath ) ;
		if( res == -1 ) return -1 ;
		
		if( IsDirectory( DirPath ) == 0 )
		{
			FileListInfo->Num = 1 ;
			
			FileListInfo->List = ( FILE_INFO * )malloc( sizeof( FILE_INFO ) ) ;
			CreateFileInfo( DirectoryPath, FileListInfo->List ) ;
			
			return 1 ;
		}

		SetEnMark( DirPath ) ;
	}

	// 初期化
	FileList.Num = 0 ;
	FileList.List = NULL ;
	OmitStrBuf = ( char * )malloc( 1024 ) ;
	if( OmitStrBuf == NULL ) goto ERR ;
	OmitExStrBuf = ( char * )malloc( 1024 ) ;
	if( OmitExStrBuf == NULL ) goto ERR ;
	ValidExStrBuf = ( char * )malloc( 1024 ) ;
	if( ValidExStrBuf == NULL ) goto ERR ;

	// 除外文字列リストの作成
	if( OmitName != NULL )
	{
		int i ;
		char *p ;

		strcpy( OmitStrBuf, OmitName ) ;
		i = 0 ;
		p = OmitStrBuf ;
		OmitStr[i] = p ;
		i ++ ;
		while( ( p = strchr( p, ';' ) ) != NULL )
		{
			*p = '\0' ;
			p ++ ;
			OmitStr[i] = p ;
			i ++ ;
		}
		OmitStr[i] = NULL ;
	}
	else OmitStr[0] = NULL ;

	// 除外拡張子リストの作成
	if( OmitExName != NULL )
	{
		int i ;
		char *p ;

		strcpy( OmitExStrBuf, OmitExName ) ;
		i = 0 ;
		p = OmitExStrBuf ;
		OmitExStr[i] = p ;
		i ++ ;
		while( ( p = strchr( p, ';' ) ) != NULL )
		{
			*p = '\0' ;
			p ++ ;
			OmitExStr[i] = p ;
			i ++ ;
		}
		OmitExStr[i] = NULL ;
	}
	else OmitExStr[0] = NULL ;

	// 有効拡張子リストの作成
	if( ValidExName != NULL )
	{
		int i ;
		char *p ;

		strcpy( ValidExStrBuf, ValidExName ) ;
		i = 0 ;
		p = ValidExStrBuf ;
		ValidExStr[i] = p ;
		i ++ ;
		while( ( p = strchr( p, ';' ) ) != NULL )
		{
			*p = '\0' ;
			p ++ ;
			ValidExStr[i] = p ;
			i ++ ;
		}
		ValidExStr[i] = NULL ;
	}
	else ValidExStr[0] = NULL ;

	// 列挙用関数に投げてファイルの数を得る
	FileList.Num = __EnumObject( DirPath, DirPath, NULL, OmitDirectory, SubDirectory, OmitStr, OmitExStr, ValidExStr[0] != NULL ? ValidExStr : NULL ) ;
	if( FileList.Num < 0 )
		goto ERR ;

	// ファイルリストのデータが格納できるメモリ領域を確保
	FileList.List = ( FILE_INFO * )malloc( FileList.Num * sizeof( FILE_INFO ) ) ;
	if( FileList.List == NULL )
		goto ERR ;
	memset( FileList.List, 0, FileList.Num * sizeof( FILE_INFO ) ) ;

	// ファイルリストにデータを格納する
	FileList.Num = 0 ;
	if( __EnumObject( DirPath, DirPath, &FileList, OmitDirectory, SubDirectory, OmitStr, OmitExStr, ValidExStr[0] != NULL ? ValidExStr : NULL ) < 0 )
		goto ERR ;

	// データを格納
	if( FileListInfo != NULL ) *FileListInfo = FileList ;

	// 除外文字列バッファ解放
	free( OmitStrBuf ) ;

	// 除外拡張子バッファ解放
	free( OmitExStrBuf ) ;

	// 有効拡張子バッファ解放
	free( ValidExStrBuf ) ;

	// 終了
	return FileList.Num ;

	// エラー処理
ERR :
	ReleaseFileList( &FileList ) ;
	if( OmitStrBuf != NULL ) free( OmitStrBuf ) ;
	if( OmitExStrBuf != NULL ) free( OmitExStrBuf ) ;
	if( ValidExStrBuf != NULL ) free( ValidExStrBuf ) ;
	
	return -1 ;
}

extern int ReleaseFileList( FILE_INFOLIST *DirectoryInfo )
{
	// メモリの解放
	if( DirectoryInfo->List != NULL )
	{
		int i ;
		FILE_INFO *finfo ;

		finfo = DirectoryInfo->List ;
		for( i = 0 ; i < DirectoryInfo->Num ; i ++, finfo ++ )
			if( finfo->FileName != NULL ) free( finfo->FileName ) ;

		free( DirectoryInfo->List ) ;
		DirectoryInfo->List = NULL ;
	}

	// ゼロ初期化
	memset( DirectoryInfo, 0, sizeof( FILE_INFOLIST ) ) ;

	// 終了
	return 0 ;
}

// 特定のパス文字列から色々な情報を取得する
// ( CurrentDir はフルパスである必要がある(語尾に『\』があっても無くても良い) )
// ( CurrentDir が 0 の場合は実際のカレントディレクトリを使用する )
extern int AnalyseFilePath(
	const char *Src,
	char *FullPath, char *DirPath, char *FileName, char *Name, char *ExeName, const char *CurrentDir )
{
	char full[256], dir[256], fname[256], name[256], exename[256];

	ConvertFullPath__( Src, full, CurrentDir );
	AnalysisFileNameAndDirPath( full, fname, dir );
	AnalysisFileNameAndExeName( fname, name, exename );
	if( FullPath ) strcpy( FullPath, full    );
	if( DirPath  ) strcpy( DirPath,  dir     );
	if( FileName ) strcpy( FileName, fname   );
	if( Name     ) strcpy( Name,     name    );
	if( ExeName  ) strcpy( ExeName,  exename );

	return 0;
}

// フルパスではないパス文字列をフルパスに変換する
// ( CurrentDir はフルパスである必要がある(語尾に『\』があっても無くても良い) )
// ( CurrentDir が NULL の場合は実際のカレントディレクトリを使用する )
extern int ConvertFullPath__( const char *Src, char *Dest, const char *CurrentDir )
{
	int i, j, k ;
	char iden[256], cur[MAX_PATH] ;

	if( CurrentDir == NULL )
	{
		GetCurrentDirectory( MAX_PATH, cur ) ;
		CurrentDir = cur ;
	}

	if( Src == NULL )
	{
		strcpy( Dest, CurrentDir ) ;
		goto END ;
	}

	i = 0 ;
	j = 0 ;
	k = 0 ;
	
	// 最初に『\』又は『/』が２回連続で続いている場合はネットワークを介していると判断
	if( ( Src[0] == '\\' && Src[1] == '\\' ) ||
		( Src[0] == '/'  && Src[1] == '/'  ) )
	{
		Dest[0] = '\\';
		Dest[1] = '\0';

		i += 2;
		j ++ ;
	}
	else
	// 最初が『\』又は『/』の場合はカレントドライブのルートディレクトリまで落ちる
	if( Src[0] == '\\' )
	{
		Dest[0] = CurrentDir[0] ;
		Dest[1] = CurrentDir[1] ;
		Dest[2] = '\0' ;

		i ++ ;
		j = 2 ;
	}
	else
	// ドライブ名が書かれていたらそのドライブへ
	if( Src[1] == ':' )
	{
		Dest[0] = Src[0] ;
		Dest[1] = Src[1] ;
		Dest[2] = '\0' ;

		i = 2 ;
		j = 2 ;

		if( Src[i] == '\\' ) i ++ ;
	}
	else
	// それ以外の場合はカレントディレクトリ
	{
		strcpy( Dest, CurrentDir ) ;
		j = strlen( Dest ) ;
		if( Dest[j-1] == '\\' || Dest[j-1] == '/' )
		{
			Dest[j-1] = '\0' ;
			j -- ;
		}
	}

	for(;;)
	{
		switch( Src[i] )
		{
		case '\0' :
			if( k != 0 )
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
				k = 0 ;
			}
			goto END ;

		case '\\' :
		case '/' :
			// 文字列が無かったらスキップ
			if( k == 0 )
			{
				i ++ ;
				break;
			}
			if( strcmp( iden, "." ) == 0 )
			{
				// なにもしない
			}
			else
			if( strcmp( iden, ".." ) == 0 )
			{
				// 一つ下のディレクトリへ
				j -- ;
				while( Dest[j] != '\\' && Dest[j] != '/' && Dest[j] != ':' ) j -- ;
				if( Dest[j] != ':' ) Dest[j] = '\0' ;
				else j ++ ;
			}
			else
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
			}

			k = 0 ;
			i ++ ;
			break ;
		
		default :
			if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
			{
				iden[k] = Src[i] ;
				iden[k+1] = 0 ; 
				k ++ ;
				i ++ ;
			}
			else
			{
				*(( unsigned short * )&iden[k] ) = *(( unsigned short * )&Src[i] ) ;
				iden[k+2] = '\0' ;
				k += 2 ;
				i += 2 ;
			}
			break ;
		}
	}
	
END :
	// 正常終了
	return 0 ;
}

// ファイル名も一緒になっていると分かっているパス中からファイル名とディレクトリパスを分割する
// フルパスである必要は無い、ファイル名だけでも良い
extern int AnalysisFileNameAndDirPath( const char *Src, char *FileName, char *DirPath )
{
	int i, Last ;
	
	// ファイル名を抜き出す
	i = 0 ;
	Last = -1 ;
	while( Src[i] != '\0' )
	{
		if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
		{
			if( Src[i] == '\\' || Src[i] == '/' || Src[i] == '\0' || Src[i] == ':' ) Last = i ;
			i ++ ;
		}
		else
		{
			i += 2 ;
		}
	}
	if( FileName != NULL )
	{
		if( Last != -1 ) strcpy( FileName, &Src[Last+1] ) ;
		else strcpy( FileName, Src ) ;
	}
	
	// ディレクトリパスを抜き出す
	if( DirPath != NULL )
	{
		if( Last != -1 )
		{
			strncpy( DirPath, Src, Last ) ;
			DirPath[Last] = '\0' ;
		}
		else
		{
			DirPath[0] = '\0' ;
		}
	}
	
	// 終了
	return 0 ;
}

// ファイルパスからファイル名と拡張子を取得する
extern int AnalysisFileNameAndExeName( const char *Src, char *Name, char *ExeName )
{
	char FileName[256], *p, ename[128], name[128] ;

	// ファイル名のみを取得
	AnalysisFileNameAndDirPath( Src, FileName, 0 ) ;

	// 『.』があるかどうかで処理を分岐
	if( ( p = strrchr( FileName, '.' ) ) == NULL )
	{
		strcpy( name, FileName ) ;
		ename[0] = '\0' ;
	}
	else
	{
		strncpy( name, FileName, p - FileName ) ;
		name[p - FileName] = '\0' ;
		strcpy( ename, p + 1 ) ;
	}

	if( Name != NULL ) strcpy( Name, name ) ;
	if( ExeName != NULL ) strcpy( ExeName, ename ) ;

	// 終了
	return 0 ;
}

// ファイルパスの拡張子を変えた文字列を得る
extern int GetChangeExeNamePath( const char *Src, char *Dest, const char *ExeName )
{
	char DirPath[256], FileName[128] ;

	AnalysisFileNameAndDirPath( Src, NULL, DirPath ) ;
	AnalysisFileNameAndExeName( Src, FileName, 0 ) ;
	SetEnMark( DirPath ) ;
	sprintf( Dest, "%s%s.%s", DirPath, FileName, ExeName ) ;
	
	// 終了
	return 0 ;
}


// 語尾に『\』がついていない場合は付ける
extern void SetEnMark( char *PathBuf )
{
	int Len = ( int )strlen( PathBuf ) ;
	
	if( PathBuf[Len-1] != '\\' )
	{
		PathBuf[Len] = '\\' ;
		PathBuf[Len+1] = '\0' ;
	}
}

// 語尾に指定の文字がない場合はつける
extern void SetChr( char *PathBuf, char chr )
{
	int Len = ( int )strlen( PathBuf ) ;
	
	if( PathBuf[Len-1] != chr )
	{
		PathBuf[Len] = chr ;
		PathBuf[Len+1] = '\0' ;
	}
}

// 語尾に指定の文字がある場合は削除する
extern void DelChr( char *PathBuf, char chr )
{
	int Len = ( int )strlen( PathBuf ) ;

	if( PathBuf[Len-1] == chr )
	{
		PathBuf[Len-1] = '\0' ;
	}
}


// 拡張子を得る
extern int GetExName( const char *Path, char *ExNameBuf )
{
	char *p ;
	
	// 一番最後に '.' が出る部分を取得する
	p = strrchr( ( char * )Path, '.' ) ;
	if( p == NULL ) ExNameBuf[0] = '\0' ;
	else strcpy( ExNameBuf, p + 1 ) ;

	// 終了
	return 0 ;
}

// 拡張子を変更する
extern int SetExName( const char *Path, char *ExName, char *DestBuf )
{
	char *p ;
	char tempstr[256] ;
	
	strcpy( tempstr, Path ) ;

	// 一番最後に '.' が出る部分を取得する
	p = strrchr( tempstr, '.' ) ;
	if( p == NULL )
	{
		sprintf( DestBuf, "%s.%s", tempstr, ExName ) ;
	}
	else
	{
		strncpy( DestBuf, tempstr, p - tempstr + 1 ) ;
		strcpy( DestBuf + ( p - tempstr + 1 ), ExName ) ;
	}

	// 終了
	return 0 ;
}


// テキストデータかどうかを判定する( 1:テキストデータ  0:バイナリデータ )
extern int CheckTextData( void *buffer, int size )
{
	unsigned char *p ;
	int len, saerch ;
	
	// 先頭８ＫＢのみ見る
	saerch = size > TEXTCHECKSIZE ? TEXTCHECKSIZE : size ;

	// 終端文字が２回以上出てきたらバイナリデータ
	{
		int con ;
	
		p = ( unsigned char * )buffer ;
		con = 0 ;
		for( len = saerch ; len > 0 ; len --, p ++ )
		{
			if( *p == '\0' )
			{
				con ++ ;
				if( con >= 2 ) return 0 ;
			}
		}
	}
	
	// 改行、タブ以外のコントロール文字が出てきたらバイナリデータ
	{
		int con ;
		
		p = ( unsigned char * )buffer ;
		con = 0 ;
		for( len = saerch ; len > 0 ; len --, p ++ )
		{
			if( *p < 0x20 && *p != '\r' && *p != '\n' && *p != '\t' )
			{
				con ++ ;
				if( con >= 1 ) return 0 ;
			}
		}
	}
	
	// 文字ではないコードが出てきたらバイナリデータ
	{
		p = ( unsigned char * )buffer ;
		for( len = saerch ; len > 0 ; len --, p ++ )
		{
			if( ( *p >= 0x81 && *p <= 0x9f ) || ( *p >= 0xe0 && *p <= 0xfc ) )
			{
				len -- ;
				p ++ ;
				if( len == 0 && size <= saerch ) return 0 ;
				
				if( !( ( *p >= 0x40 && *p <= 0x7e ) || ( *p >= 0x80 && *p <= 0xfc ) ) )
					return 0 ;
				
				if( len == 0 ) return 1 ;
			}
		}
	}
	
	// 此処に来たらテキストデータ
	return 1 ;
}

// テキストファイルかどうかを判定する( 1:テキストデータ  0:バイナリデータ )
extern int CheckTextFile( const char *Path )
{
	void *buf ;
	int size, res ;
	FILE *fp ;

	fp = fopen( Path, "rb" ) ;
	if( fp == NULL ) return -1 ;
	
	fseek( fp, 0L, SEEK_END ) ;
	size = ftell( fp ) ;
	fseek( fp, 0L, SEEK_SET ) ;
	
	size = size > TEXTCHECKSIZE ? TEXTCHECKSIZE : size ;
	
	buf = malloc( size ) ;
	if( buf == NULL )
	{
		fclose( fp ) ;
		return -1 ;
	}
	
	fread( buf, size, 1, fp ) ;
	fclose( fp ) ;
	
	res = CheckTextData( buf, size ) ;
	
	free( buf ) ;

	return res ;
}

// ２バイト文字か調べる( TRUE:２バイト文字 FALSE:１バイト文字 )
extern int CheckMultiByteChar( char *Buf )
{
	return  ( (unsigned char)*Buf >= 0x81 && (unsigned char)*Buf <= 0x9F ) || ( (unsigned char)*Buf >= 0xE0 && (unsigned char)*Buf <= 0xFC ) ;
}








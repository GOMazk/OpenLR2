#include "FileLib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

int btNameNum ;
char btNameList[ 10000 ][ 128 ] ;
char DestBuffer[ 1024 * 1024 ] ;

void Rename( FILE_INFOLIST *FInfoList, bool FileRename )
{
	FILE_INFO *FInfo ;
	int i ;
	int j ;
	int k ;
	int l ;
	unsigned char *FileImage ;
	int FileSize ;
	unsigned char *data ;
	unsigned char *dest ;
	char FileName[ 1024 ] ;
	char Name[ 4096 ] ;

	FInfo = FInfoList->List ;
	for( i = 0 ; i < FInfoList->Num ; i ++, FInfo ++ )
	{
		sprintf( FileName, "%s%s", FInfo->AbsDirectoryPath, FInfo->FileName ) ;
		LoadFileMem( FileName, ( void ** )&FileImage, &FileSize ) ;

		memset( DestBuffer, 0, sizeof( DestBuffer ) ) ;

		dest = ( unsigned char * )DestBuffer ;
		data = FileImage ;
		j = 0 ;
		for(;;)
		{
			if( ( data[ 0 ] >= 'a' && data[ 0 ] <= 'z' ) ||
				( data[ 0 ] >= 'A' && data[ 0 ] <= 'Z' ) ||
				data[ 0 ] == '_' )
			{
				for( k = 0 ; ; )
				{
					if( ( data[ 0 ] < 'a' || data[ 0 ] > 'z' ) &&
						( data[ 0 ] < 'A' || data[ 0 ] > 'Z' ) &&
						( data[ 0 ] < '0' || data[ 0 ] > '9' ) &&
						data[ 0 ] != '_' )
					{
						break ;
					}
					else
					{
						Name[ k ] = data[ 0 ] ;
						k ++ ;

						j ++ ;
						data ++ ;
						if( j == FileSize )
						{
							break ;
						}
					}
				}

				Name[ k ] = '\0' ;
				if( j + 3 > FileSize || data[ 0 ] != '.' || data[ 1 ] != 'h' || data[ 2 ] != '\"' )
				{
					for( l = 0 ; l < btNameNum && strcmp( Name, btNameList[ l ] ) != 0 ; l ++ ){}
					if( l != btNameNum )
					{
						dest[ 0 ] = 'D' ;
						dest[ 1 ] = '_' ;
						dest += 2 ;
					}
				}
				strcpy( ( char * )dest, Name ) ;
				dest += k ;

				if( j == FileSize )
				{
					break ;
				}
			}
			else
			{
				dest[ 0 ] = data[ 0 ] ;
				dest ++ ;

				j ++ ;
				data ++ ;
				if( j == FileSize )
				{
					break ;
				}
			}
		}

		if( FileRename )
		{
			sprintf( FileName, "%sD_%s", FInfo->AbsDirectoryPath, FInfo->FileName ) ;
		}
		SaveFileMem( FileName, DestBuffer, ( char * )dest - DestBuffer ) ;

		free( FileImage ) ;
	}
}

void RenameMain( const char *Path )
{
	FILE_INFOLIST FInfoList ;
	FILE_INFO *FInfo ;
	int i ;
	int j ;
	int k ;
	unsigned char *FileImage ;
	int FileSize ;
	unsigned char *data ;
	unsigned char *start ;
	char FileName[ 1024 ] ;
	char Name[ 4096 ] ;
	bool EnumFlag ;
	bool TypedefFlag ;
	bool TypedefEndFlag ;
	unsigned char LastChar ;

	CreateFileList( Path, &FInfoList, 1, 1, 0, 0, "h;cpp" ) ;

	FInfo = FInfoList.List ;
	for( i = 0 ; i < FInfoList.Num ; i ++, FInfo ++ )
	{
		sprintf( FileName, "%s%s", FInfo->AbsDirectoryPath, FInfo->FileName ) ;
		LoadFileMem( FileName, ( void ** )&FileImage, &FileSize ) ;

		EnumFlag = false ;
		TypedefFlag = false ;

		data = FileImage ;
		j = 0 ;
		for(;;)
		{
			if( data[ 0 ] == '/' && data[ 1 ] == '/' )
			{
				data += 2 ;
				j += 2 ;
				while( j < FileSize && data[ 0 ] != '\n' )
				{
					data ++ ;
					j ++ ;
				}
				data ++ ;
				j ++ ;
				if( j >= FileSize )
				{
					break ;
				}
			}
			else
			if( ( data[ 0 ] >= 'a' && data[ 0 ] <= 'z' ) ||
				( data[ 0 ] >= 'A' && data[ 0 ] <= 'Z' ) ||
				data[ 0 ] == '_' )
			{
				start = data ;
				for( k = 0 ; ; )
				{
					if( ( data[ 0 ] < 'a' || data[ 0 ] > 'z' ) &&
						( data[ 0 ] < 'A' || data[ 0 ] > 'Z' ) &&
						( data[ 0 ] < '0' || data[ 0 ] > '9' ) &&
						data[ 0 ] != '_' )
					{
						break ;
					}
					else
					{
						Name[ k ] = data[ 0 ] ;
						k ++ ;

						j ++ ;
						data ++ ;
						if( j == FileSize )
						{
							break ;
						}
					}
				}

				Name[ k ] = '\0' ;
				if( ( Name[ 0 ] != 'i' || Name[ 1 ] != 'n' || Name[ 2 ] != 't' || Name[ 3 ] != '\0' ) &&
					strncmp( "DWORD", Name, strlen( "DWORD" ) ) != 0 &&
					strncmp( "for", Name, strlen( "for" ) ) != 0 &&
					strncmp( "printf", Name, strlen( "printf" ) ) != 0 &&
					strncmp( "using", Name, strlen( "using" ) ) != 0 &&
					(
					  ( TypedefEndFlag && data[ 0 ] == ';' && start[ -3 ] == '\n' && start[ -2 ] == '}' && start[ -1 ] == ' ' ) || 
					  ( TypedefFlag && data[ 0 ] == ';' ) ||
					  ( TypedefFlag && ( data[ 0 ] == '[' && data[ 1 ] >= '0' && data[ 1 ] <= '9' && data[ 2 ] == ']' && data[ 3 ] == ';' ) ) ||
					  ( TypedefFlag && ( data[ 0 ] == '[' && data[ 1 ] >= '0' && data[ 1 ] <= '9' && data[ 2 ] >= '0' && data[ 2 ] <= '9' && data[ 3 ] == ']' && data[ 4 ] == ';' ) ) ||
					  ( TypedefFlag && ( data[ 0 ] == '[' && data[ 1 ] >= '0' && data[ 1 ] <= '9' && data[ 2 ] == ']' && data[ 3 ] == '[' && data[ 4 ] >= '0' && data[ 4 ] <= '9' && data[ 5 ] == ']' && data[ 6 ] == ';' ) ) ||
					  ( EnumFlag && ( LastChar == '{' || LastChar == ',' ) ) ||
					  (
					    ( j >= 8  && strncmp( "#define ",  ( char * )&start[ -8 ], 8 ) == 0 && data[ 1 ] != '\r' && data[ 0 ] != '\r' ) ||
					    ( j >= 6  && strncmp( "class ",    ( char * )&start[ -6 ], 6 ) == 0 ) ||
					    ( j >= 6  && strncmp( "class\t",   ( char * )&start[ -6 ], 6 ) == 0 ) ||
					    ( j >= 5  && strncmp( "enum\t",    ( char * )&start[ -5 ], 5 ) == 0 ) ||
					    ( j >= 5  && strncmp( "enum ",     ( char * )&start[ -5 ], 5 ) == 0 ) ||
					    ( j >= 7  && strncmp( "struct ",   ( char * )&start[ -7 ], 7 ) == 0 ) ||
					    ( j >= 9  && strncmp( "(struct) ", ( char * )&start[ -9 ], 9 ) == 0 ) ||
					    ( j >= 1  && strncmp( "typedef", Name, strlen( "typedef" ) ) == 0 && start[ -1 ] == '\n' ) ||
					    ( j >= 2  && strncmp( "typedef", Name, strlen( "typedef" ) ) == 0 && start[ -2 ] == '\r' && start[ -1 ] == '\n' ) ||
					    ( j >= 3  && strncmp( "typedef", Name, strlen( "typedef" ) ) == 0 && start[ -3 ] == '\r' && start[ -2 ] == '\n' && start[ -1 ] == '\t' ) ||
					    ( j >= 6  && strncmp( "typedef", Name, strlen( "typedef" ) ) == 0 && start[ -6 ] == '\r' && start[ -5 ] == '\n' && start[ -4 ] == ' ' && start[ -3 ] == ' ' && start[ -2 ] == ' ' && start[ -1 ] == ' ' ) ||
					    ( Name[ 0 ] == 'b' && Name[ 1 ] == 't' && Name[ 2 ] >= 'A' && Name[ 2 ] <= 'Z' ) ||
					    ( Name[ 0 ] == 'g' &&                     Name[ 1 ] >= 'A' && Name[ 1 ] <= 'Z' ) ||
					    ( Name[ 0 ] == 'd' &&                     Name[ 1 ] >= 'A' && Name[ 1 ] <= 'Z' ) ||
					    ( Name[ 0 ] == 't' &&                     Name[ 1 ] >= 'A' && Name[ 1 ] <= 'Z' ) ||
					    ( Name[ 0 ] == 's' &&                     Name[ 1 ] >= 'A' && Name[ 1 ] <= 'Z' ) ||
					    ( Name[ 0 ] == 'p' && Name[ 1 ] == 'l' && Name[ 2 ] >= 'A' && Name[ 2 ] <= 'Z' ) ||
					    ( Name[ 0 ] == 'g' && Name[ 1 ] == 'i' && Name[ 2 ] == 'm' && Name[ 3 ] == '_' ) ||
					    ( Name[ 0 ] == 'G' && Name[ 1 ] == 'I' && Name[ 2 ] == 'M' && Name[ 3 ] == '_' ) ||
					    ( Name[ 0 ] == 'G' && Name[ 1 ] == 'E' && Name[ 2 ] == 'N' && Name[ 3 ] == '_' ) ||
					    ( Name[ 0 ] == 'B' && Name[ 1 ] == 'T' && Name[ 2 ] == '_'                     ) ||
					    strncmp( "SphereTriangleDetector", Name, strlen( "SphereTriangleDetector" ) ) == 0 ||
					    strncmp( "SequentialThreadFunc", Name, strlen( "SequentialThreadFunc" ) ) == 0 ||
					    strncmp( "SequentiallsMemorySetupFunc", Name, strlen( "SequentiallsMemorySetupFunc" ) ) == 0 ||
					    strncmp( "Win32ThreadFunc", Name, strlen( "Win32ThreadFunc" ) ) == 0 ||
					    strncmp( "Win32lsMemorySetupFunc", Name, strlen( "Win32lsMemorySetupFunc" ) ) == 0 ||
					    strncmp( "PosixThreadFunc", Name, strlen( "PosixThreadFunc" ) ) == 0 ||
					    strncmp( "PosixlsMemorySetupFunc", Name, strlen( "PosixlsMemorySetupFunc" ) ) == 0 ||
					    strncmp( "disableCcd", Name, strlen( "disableCcd" ) ) == 0 ||
					    strncmp( "cullPoints2", Name, strlen( "cullPoints2" ) ) == 0 ||
					    strncmp( "addr64", Name, strlen( "addr64" ) ) == 0 ||
					    strncmp( "ContactAddedCallback", Name, strlen( "ContactAddedCallback" ) ) == 0 ||
					    strncmp( "ContactDestroyedCallback", Name, strlen( "ContactDestroyedCallback" ) ) == 0 ||
					    strncmp( "ContactProcessedCallback", Name, strlen( "ContactProcessedCallback" ) ) == 0 ||
					    strncmp( "gjkepa2_impl", Name, strlen( "gjkepa2_impl" ) ) == 0 ||
					    strncmp( "Vectormath", Name, strlen( "Vectormath" ) ) == 0 ||
					    strncmp( "Aos", Name, strlen( "Aos" ) ) == 0 ||
					    strncmp( "enum\r\n", ( char * )start, strlen( "enum\r\n" ) ) == 0 ||
					    strncmp( "maxIterations", Name, strlen( "maxIterations" ) ) == 0 
					  )
					)
				  )
				{
					if( ( j >= 5 && ( strncmp( "enum ", ( char * )&start[ -5 ], 5 ) == 0 || strncmp( "enum\t", ( char * )&start[ -5 ], 5 ) == 0 ) ) ||
						strncmp( "enum\r\n", ( char * )start, strlen( "enum\r\n" ) ) == 0 )
					{
						EnumFlag = true ;
					}

					if( strncmp( "typedef", Name, strlen( "typedef" ) ) == 0 )
					{
						TypedefFlag = true ;
					}

					if( ( Name[ 0 ] != 'D' || Name[ 1 ] != '_' ) &&
						strncmp( "enum\r\n", ( char * )start, strlen( "enum\r\n" ) ) != 0 &&
						strncmp( "typedef", Name, strlen( "typedef" ) ) != 0 )
					{
						for( k = 0 ; k < btNameNum && strcmp( Name, btNameList[ k ] ) != 0 ; k ++ ){}
						if( k == btNameNum )
						{
							strcpy( btNameList[ btNameNum ], Name ) ;
							btNameNum ++ ;
						}
					}
				}

				if( j == FileSize )
				{
					break ;
				}
			}
			else
			{
				if( TypedefFlag && ( data[ 0 ] == ';' || data[ 0 ] == '\n' ) )
				{
					TypedefFlag = false ;
					if( data[ 0 ] == '\n' )
					{
						TypedefEndFlag = true ;
					}
				}
				if( EnumFlag && data[ 0 ] == '}' )
				{
					EnumFlag = false ;
				}
				if( data[ 0 ] != ' ' && data[ 0 ] != '\t' && data[ 0 ] != '\r' && data[ 0 ] != '\n' )
				{
					LastChar = data[ 0 ] ;
				}

				j ++ ;
				data ++ ;
				if( j == FileSize )
				{
					break ;
				}
			}
		}

		free( FileImage ) ;
	}

	Rename( &FInfoList, false ) ;
}

int main( int argc, char **argv )
{
	printf( "Bullet Physics Version 2.75 Rename\n" ) ;
	if( argc != 2 )
	{
		printf( "bullet_rename.exe BulletPhysicsSrcFolderPath\n" ) ;
		return -1 ;
	}

	RenameMain( argv[ 1 ] ) ;

	return 0 ;
}

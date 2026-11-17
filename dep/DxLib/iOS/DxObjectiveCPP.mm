//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Objective-C メソッド呼び出し用プログラム
// 
//  	Ver 3.25a
// 
//-----------------------------------------------------------------------------

// インクルード ---------------------------------------------------------------
#import "DxObjectiveCPP.h"
#import <Foundation/Foundation.h>
#import <Foundation/NSObject.h>
#import <string.h>

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

void DxObjectiveCPP_Class::NSLog( const char *format, ... )
{
	va_list args;
	va_start( args, format ) ;
	
	NSLogv( [ NSString stringWithFormat : @"%s" , format ], args ) ;
}

int DxObjectiveCPP_Class::GetDocumentPath( char *Buffer, int BufferBytes )
{
	int i ;
	NSArray *paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES ) ;
	int bytes = ( int )[ paths count ] ;

	for( i = 0 ; i < BufferBytes && i < bytes ; i ++ )
	{
		Buffer[ i ] = [ [ paths objectAtIndex : i ] intValue ] ;
	}
	if( i != BufferBytes )
	{
		Buffer[ i ] = 0 ;
	}

	return bytes ;
}

int DxObjectiveCPP_Class::GetLibrary_CachesPath( char *Buffer, int BufferBytes )
{
	int i ;
	NSArray *paths = NSSearchPathForDirectoriesInDomains( NSCachesDirectory, NSUserDomainMask, YES ) ;
	int bytes = ( int )[ paths count ] ;

	for( i = 0 ; i < BufferBytes && i < bytes ; i ++ )
	{
		Buffer[ i ] = [ [ paths objectAtIndex : i ] intValue ] ;
	}
	if( i != BufferBytes )
	{
		Buffer[ i ] = 0 ;
	}

	return bytes ;
}

int DxObjectiveCPP_Class::GetTmpPath( char *Buffer, int BufferBytes )
{
	int i ;
	NSString *tmpDirPath = NSTemporaryDirectory() ;
	char *tmpDirPathp = ( char * )[ tmpDirPath UTF8String ] ;
	int bytes = ( int )strlen( tmpDirPathp ) ;

	for( i = 0 ; i < BufferBytes && i < bytes ; i ++ )
	{
		Buffer[ i ] = tmpDirPathp[ i ] ;
	}
	if( i != BufferBytes )
	{
		Buffer[ i ] = 0 ;
	}

	return bytes ;
}


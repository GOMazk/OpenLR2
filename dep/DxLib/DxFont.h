// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		フォント処理用プログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_FONT_H
#define DX_FONT_H

#include "DxCompileConfig.h"

#ifndef DX_NON_FONT

// インクルード ------------------------------------------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxMemImg.h"
#include "DxHandle.h"
#include <stdarg.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define FSYS FontSystem

/* バージョン情報

0      初期バージョン
1      カーニングペア情報追加

*/

/* データマップ

FONTDATAFILEHEADER

圧縮されて格納
	FONTDATAFILEPRESSHEADER
	FONTDATAFILECHARADATA × 文字の数
	FONTDATAFILEKERNINGPAIRDATA × カーニングペアの数	

*/

// バージョン
#define DX_FONTDATAFILE_VERSION					(1)

// バージョン 0 の各構造体のサイズ
#define DX_FONTBASEINFO_SIZE_VER0				(20)
#define DX_FONTDATAFILEPRESSHEADER_SIZE_VER0	(344)
#define DX_FONTDATAFILEHEADER_SIZE_VER0			(368)

// フォントの元イメージのデータタイプ
#define DX_FONT_SRCIMAGETYPE_1BIT				(0)					// 1ピクセル1ビット
#define DX_FONT_SRCIMAGETYPE_1BIT_SCALE4		(1)					// 1ピクセル1ビット、画像解像度4倍
#define DX_FONT_SRCIMAGETYPE_1BIT_SCALE8		(2)					// 1ピクセル1ビット、画像解像度8倍
#define DX_FONT_SRCIMAGETYPE_1BIT_SCALE16		(3)					// 1ピクセル1ビット、画像解像度16倍
#define DX_FONT_SRCIMAGETYPE_4BIT_MAX15			(4)					// 1ピクセル4ビット、値の範囲は0～15
#define DX_FONT_SRCIMAGETYPE_8BIT_ON_OFF		(5)					// 1ピクセル8ビット、値の範囲は0又は0以外
#define DX_FONT_SRCIMAGETYPE_8BIT_MAX16			(6)					// 1ピクセル8ビット、値の範囲は0～16
#define DX_FONT_SRCIMAGETYPE_8BIT_MAX64			(7)					// 1ピクセル8ビット、値の範囲は0～64
#define DX_FONT_SRCIMAGETYPE_8BIT_MAX255		(8)					// 1ピクセル8ビット、値の範囲は0～255

#define FONT_CACHE_MAXNUM						(2024)				// フォントキャッシュに格納できる最大文字数
#define FONT_CACHE_EX_NUM						(1024)				// 0xffff を超えるコードの文字データアドレスを保持する数
#define FONT_CACHE_MEMORYSIZE					(0x50000)			// フォントキャッシュの最大容量
#define FONT_CACHE_MAX_YLENGTH					(0x4000)			// フォントキャッシュサーフェスの最大縦幅
#define FONT_GRAPHICHANDLE_IMAGE_MAXNUM			(256)				// 特定の文字コードをグラフィックハンドルの画像に置き換えられる最大数
#define FONT_SUBSTITUTION_MAXNUM				(16)				// 登録できる代替フォントの最大数

// フォントハンドルの有効性チェック
#define FONTHCHK( HAND, HPOINT )		HANDLECHK(       DX_HANDLETYPE_FONT, HAND, *( ( HANDLEINFO ** )&HPOINT ) )
#define FONTHCHK_ASYNC( HAND, HPOINT )	HANDLECHK_ASYNC( DX_HANDLETYPE_FONT, HAND, *( ( HANDLEINFO ** )&HPOINT ) )

// デフォルトフォントハンドル定型処理
#define DEFAULT_FONT_HANDLE_SETUP		\
	if( FontHandle == DX_DEFAULT_FONT_HANDLE )\
	{\
		if( FSYS.UserDefaultFontHandle > 0 )\
		{\
			FontHandle = FSYS.UserDefaultFontHandle ;\
		}\
		else\
		{\
			RefreshDefaultFont() ;\
			FontHandle = FSYS.DefaultFontHandle ;\
		}\
	}

// 構造体定義 --------------------------------------------------------------------

// フォント列挙時用データ構造体
struct ENUMFONTDATA
{
	wchar_t *				FontBuffer ;
	int						FontNum ;
	int						BufferNum ;
	int						JapanOnlyFlag ;
	int						Valid ;
	const wchar_t *			EnumFontName ;
} ;

// CreateFontToHandle で使用されるグローバル変数を纏めたもの
struct CREATEFONTTOHANDLE_GPARAM
{
	int						NotTextureFlag ;					// 作成するフォントキャッシュをテクスチャにするか、フラグ
	int						TextureCacheColorBitDepth16Flag ;	// 作成するフォントキャッシュテクスチャのカラービット数を16ビットにするかどうかのフラグ
	int						CacheCharNum ;						// フォントキャッシュでキャッシュできる文字の数
	int						UsePremulAlphaFlag ;				// 乗算済みαを使用するかどうかのフラグ( TRUE:使用する  FALSE:使用しない )
	int						DisableAdjustFontSize ;				// フォントサイズの補正を行わないかどうかのフラグ( TRUE:補正を行わない  FALSE:補正を行う )
} ;

// フォントの基本情報
struct FONTBASEINFO
{
	WORD					FontThickness ;						// フォントの太さ
	WORD					FontSize ;							// 描画するフォントのサイズ
	WORD					FontHeight ;						// 描画するフォントの最大縦幅
	WORD					FontAddHeight ;						// サイズ調整の為に足した高さ
	BYTE					Italic ;							// イタリックフォントかどうか(TRUE:イタリック FALSE:非イタリック)
	BYTE					Padding ;
	WORD					MaxWidth ;							// フォントの最大幅
	WORD					CharSet ;							// キャラクタセット
	WORD					CharCodeFormat ;					// 文字コード形式( 0xffff の場合は特に指定なし )
	WORD					Ascent ;							// 文字のベースラインから一番上までの高さ
	WORD					Padding2 ;
	DWORD					KerningPairNum ;					// カーニングペアの数
	DWORD					Padding3[ 10 ] ;
} ;

// フォントファイルのヘッダ情報の圧縮する部分
struct FONTDATAFILEPRESSHEADER
{
	WORD					FontName[ 128 ] ;	// フォントの名前( UTF-16LE )
	FONTBASEINFO			BaseInfo ;			// 基本情報
	BYTE					ImageBitDepth ;		// 画像のビット深度( DX_FONTIMAGE_BIT_1 等 )
	BYTE					Padding[ 3 ] ;
	DWORD					Padding2[ 16 ] ;
} ;

// フォントファイルのヘッダ情報
struct FONTDATAFILEHEADER
{
	BYTE					Magic[ 4 ] ;		// FNTF
	WORD					Version ;			// バージョン
	WORD					Padding ;
	DWORD					ImageAddress ;		// 画像データが保存されている先頭アドレス
	DWORD					MaxImageBytes ;		// 画像データの最大サイズ
	DWORD					CharaNum ;			// 文字情報の数
	DWORD					CharaExNum ;		// 文字情報の内、文字コードが 0x10000 以上の文字の数
	FONTDATAFILEPRESSHEADER	Press ;				// ヘッダファイル内の圧縮対象の部分
} ;

// カーニングペア情報
struct FONTDATAFILEKERNINGPAIRDATA
{
	DWORD					First ;				// ペアの１文字目
	DWORD					Second ;			// ペアの２文字目
	int						KernAmount ;		// ずらすドット数
	DWORD					Padding ;
} ;

// ファイル保存用のフォント１文字の情報
struct FONTDATAFILECHARADATA
{
	DWORD					CodeUnicode ;		// Unicode の文字コード
	BYTE					Press ;				// 圧縮しているかどうか( 1:圧縮している  0:圧縮していない )
	BYTE					Padding ;
	short					DrawX ;				// 文字画像を描画すべきＸ座標
	short					DrawY ;				// 文字画像を描画すべきＹ座標
	short					AddX ;				// 次の文字を描画すべき相対Ｘ座標
	WORD					SizeX ;				// 文字のＸサイズ
	WORD					SizeY ;				// 文字のＹサイズ
	DWORD					ImageAddress ;		// 画像データが保存されているアドレス( FONTDATAFILEHEADER.ImageAddress の示すアドレスを 0 とする )
	DWORD					ImagePitch ;		// 画像データの１ラインあたりのバイト数
	DWORD					Padding2[ 2 ] ;
} ;

// フォント１文字のキャッシュ情報
struct FONTCHARDATA
{
	DWORD					CodeUnicode ;		// Unicode の文字コード
	DWORD					IVSCode ;			// 異体字セレクタ( 使用しない場合は 0 )
	BYTE					ValidFlag ;			// 有効フラグ
	BYTE					GraphHandleFlag ;	// GraphIndex がグラフィックハンドルかどうかのフラグ( TRUE:グラフィックハンドル  FALSE:文字データのインデックス )
	short					DrawX ;				// 文字画像を描画すべきＸ座標
	short					DrawY ;				// 文字画像を描画すべきＹ座標
	short					AddX ;				// 次の文字を描画すべき相対Ｘ座標
	WORD					SizeX ;				// 文字画像の幅
	WORD					SizeY ;				// 文字画像の高さ
	int						GraphIndex ;		// 文字データのインデックス( 若しくはグラフィックハンドル )
	int						GraphIndexX ;		// 文字データのＸマス位置
	int						GraphIndexY ;		// 文字データのＹマス位置
	FONTDATAFILEKERNINGPAIRDATA *KerningPair ;	// この文字に対応するカーニングペア情報のアドレス
	DWORD					KerningPairNum ;	// この文字に対応するカーニングペア情報の数
	struct FONTCODEDATA *	CodeData ;			// このフォントを管理しているデータのポインタ
} ;

// フォントキャッシュ存在フラグ構造体
struct FONTCODEDATA
{
	int						ExistFlag ;			// キャッシュの存在フラグ
	FONTCHARDATA *			CharData ;			// キャッシュデータの存在位置
} ;

// フォントデータファイル運用時に使用する変数を纏めた構造体
struct FONTDATAFILEUSEINFO
{
	BYTE *					FileBuffer ;						// フォントデータファイルの中身を格納したメモリ領域の先頭アドレス
	int						FontFileSize ;						// フォントデータファイルのサイズ
	FONTDATAFILEHEADER *	Header ;							// フォントデータファイルヘッダ
	FONTDATAFILECHARADATA *	Chara ;								// フォントデータファイル内の各文字の情報
	BYTE *					Image ;								// フォントデータファイルの画像データのアドレス
	int						ImageType ;							// フォントデータファイルのイメージデータタイプ( DX_FONT_SRCIMAGETYPE_1BIT 等 )
	FONTDATAFILECHARADATA **CharaTable ;						// フォントデータファイル内の各文字の情報へのアドレスのテーブル( コード 0x0000～0xffff の範囲 )
	FONTDATAFILECHARADATA **CharaExArray ;						// 0xffff を超える文字コードの文字情報へのアドレス
	void *					PressImageDecodeBuffer ;			// 解凍した文字イメージを格納するバッファ
} ;

// 代替フォント情報
struct FONTSUBSTITUTIONINFO
{
	int						SubstitutionFontHandle ;			// 代替フォントハンドル
	int						DrawX ;								// 描画X座標
	int						DrawY ;								// 描画Y座標
} ;

// フォントキャッシュの管理データ
struct FONTMANAGE
{
	HANDLEINFO				HandleInfo ;						// ハンドル共通データ

	struct FONTMANAGE_PF *	PF ;								// 環境依存データ

	int *					LostFlag ;							// 解放時に TRUE にするフラグへのポインタ

	int						UseFontDataFile ;					// フォントデータファイルを使用しているかどうか( TRUE:使用している  FALSE:使用していない )
	FONTDATAFILEUSEINFO		FontDataFile ;						// フォントデータファイルの情報

	FONTCHARDATA			CharData[ FONT_CACHE_MAXNUM + 1 ] ;	// キャッシュデータ
	FONTCODEDATA			CodeData[ 0x10000 ] ;				// 存在データを含むデータ配列
	int						CodeDataExNum ;						// 0xffff を超える文字コードの文字データの数
	FONTCHARDATA *			CodeDataEx[ FONT_CACHE_EX_NUM ] ;	// 0xffff を超える文字コードの文字データへのポインタ
	unsigned int			Index ;								// 次データ追加時の配列インデックス
	int						MaxCacheCharNum ;					// キャッシュできる最大数
	unsigned char *			CacheMem ;							// テキストキャッシュメモリ
	int						CachePitch ;						// テキストキャッシュメモリのピッチ	
	int						CacheDataBitNum ;					// テキストキャッシュ上の１ピクセルのビット数

	FONTDATAFILEKERNINGPAIRDATA * KerningPairData ;				// カーニングペア情報

	int						GraphHandleFontImageNum ;			// 特定の文字コードをグラフィックハンドルの画像に置き換えている数
	FONTCHARDATA			GraphHandleFontImage[ FONT_GRAPHICHANDLE_IMAGE_MAXNUM ] ;	// 特定の文字コードをグラフィックハンドルの画像に置き換える処理用のデータ

	int						SubstitutionInfoNum ;				// 代替フォントの数
	FONTSUBSTITUTIONINFO	SubstitutionInfo[ FONT_SUBSTITUTION_MAXNUM ] ;	// 代替フォント情報

	TCHAR					FontNameT[ 256 ] ;					// フォント名( TCHAR 版 )
	wchar_t					FontName[ 256 ] ;					// フォント名
	FONTBASEINFO			BaseInfo ;							// 基本情報
	int						UseCharCodeFormat ;					// 引数で渡す文字コード形式
	int						Space ;								// 次の文字を表示する座標に加算ドット数
	int						LineSpaceValidFlag ;				// LineSpace が有効かどうかのフラグ
	int						LineSpace ;							// 改行の際に座標に加算するドット数
	int						FontType ;							// フォントのタイプ
	int						EdgeSize ;							// エッジの太さ
	BASEIMAGE				EdgeCircleImage ;					// エッジの円画像

	SIZE					CacheImageSize ;					// キャッシュ画像ののサイズ 
	int						LengthCharNum ;						// キャッシュ画像の縦に並べられる文字の数

	BYTE *					TempBuffer ;						// フォントの最大サイズ分の１ピクセル１バイトの作業用バッファ
	DWORD					TempBufferSize ;					// 作業用バッファのサイズ

	int						ModiDrawScreen[ 2 ] ;				// DrawModiString 用の作業用スクリーン
	int						ModiDrawScreenV[ 2 ] ;				// DrawModiString 用の作業用スクリーン( 縦書き用 )

	int						TextureCache ;						// テクスチャにキャッシュする場合のテクスチャグラフィックハンドル
	int						TextureCacheSub ;					// テクスチャにキャッシュする場合のテクスチャグラフィックハンドル(縁用)
	int						TextureCacheLostFlag ;				// TextureCache が無効になったときに TRUE になる変数
	BASEIMAGE				TextureCacheBaseImage ;				// TextureCache に転送したものと同じものを格納した基本イメージ
	int						TextureCacheColorBitDepth ;			// テクスチャキャッシュのカラービット数
	int						TextureCacheUsePremulAlpha ;		// テクスチャキャッシュのαチャンネルを乗算済みαにするかどうか
	int						TextureCacheSizeX ;					// テクスチャキャッシュの幅
	int						TextureCacheSizeY ;					// テクスチャキャッシュの高さ
	float					TextureCacheInvSizeX ;				// テクスチャキャッシュの幅の逆数
	float					TextureCacheInvSizeY ;				// テクスチャキャッシュの高さの逆数

	int						TextureCacheUpdateRectValid ;		// テクスチャキャッシュ更新矩形情報が有効かどうか
	RECT					TextureCacheUpdateRect ;			// テクスチャキャッシュ更新矩形情報

	int						TextureCacheFlag ;					// テクスチャキャッシュをしているか、フラグ
} ;

// フォントシステム用構造体
struct FONTSYSTEM
{
	int						InitializeFlag ;					// 初期化フラグ

	int						NotTextureFlag ;					// 作成するフォントキャッシュをテクスチャにするか、フラグ
	int						TextureCacheColorBitDepth16Flag ;	// 作成するフォントキャッシュテクスチャのカラービット数を16ビットにするかどうかのフラグ
	int						AntialiasingFontOnlyFlag ;			// アンチエイリアスフォントのみ使用できるか、フラグ
	int						CacheCharNum ;						// フォントキャッシュでキャッシュできる文字の数
	int						UsePremulAlphaFlag ;				// 乗算済みαを使用するかどうかのフラグ( TRUE:使用する  FALSE:使用しない )
	int						DisableAdjustFontSize ;				// フォントサイズの補正を行わないかどうかのフラグ( TRUE:補正を行わない  FALSE:補正を行う )
	int						OnlyDrawType ;						// フォントの描画で縁のみ、又は本体のみ描画を行うための設定( OnlyType  0:通常描画 1:本体のみ描画 2:縁のみ描画 )
	int						IgnoreLFFlag ;						// \n を無視するかどうかのフラグ( TRUE:無視する  FALSE:無視しない )
	BYTE					BitCountTable[ 256 ] ;				// ビットカウントテーブル
	BYTE					MAX15ToMAX16[ 16 ] ;				// 0～15  の値を 0～16 に変換するためのテーブル
	BYTE					MAX255ToMAX16[ 256 ] ;				// 0～255 の値を 0～16 に変換するためのテーブル
	BYTE					MAX15ToMAX64[ 16 ] ;				// 0～15  の値を 0～64 に変換するためのテーブル
	BYTE					MAX255ToMAX64[ 256 ] ;				// 0～255 の値を 0～64 に変換するためのテーブル

	wchar_t					DoubleByteSpaceCharCode ;			// 全角スペースの wchar_t コード

	int						UseDefaultFontImage ;				// デフォルトフォントイメージを使用しているかどうか( TRUE:使用している  FALSE:使用していない )
	void *					DefaultFontImage ;					// デフォルトフォントイメージを格納しているアドレスを保存するポインタ
	int						DefaultFontImageGraphHandle[ 8 ][ 16 ] ;	// デフォルトフォントイメージのグラフィックハンドル

	int						DefaultFontHandle ;					// デフォルトで使用するフォントのハンドル
	int						UserDefaultFontHandle ;				// ユーザー指定のデフォルトのフォントハンドル

	int						EnableInitDefaultFontName ;			// デフォルトで使用するフォントの設定
	wchar_t					DefaultFontName[ 256 ] ;

	int						EnableInitDefaultFontSize ;
	int						DefaultFontSize ;

	int						EnableInitDefaultFontThick ;
	int						DefaultFontThick ;

	int						EnableInitDefaultFontType ;
	int						DefaultFontType ;

	int						EnableInitDefaultFontCharSet ;
	int						DefaultFontCharSet ;
	int						DefaultFontCharSet_Change ;

	int						EnableInitDefaultFontEdgeSize ;
	int						DefaultFontEdgeSize ;

	int						EnableInitDefaultFontItalic ;
	int						DefaultFontItalic ;

	int						EnableInitDefaultFontSpace ;
	int						DefaultFontSpace ;

	int						EnableInitDefaultFontLineSpace ;
	int						DefaultFontLineSpaceValid ;
	int						DefaultFontLineSpace ;
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

extern FONTSYSTEM FontSystem ;

// 関数プロトタイプ宣言-----------------------------------------------------------

extern	int			InitFontManage( void ) ;																					// フォントシステムの初期化
extern	int			TermFontManage( void ) ;																					// フォント制御の終了
extern	int			InitCacheFontToHandle( void ) ;																				// フォントのキャッシュ情報を初期化する
extern	int			InitFontCacheToHandle( FONTMANAGE *ManageData, int ASyncThread = FALSE ) ;											// 特定のフォントのキャッシュ情報を初期化する

extern	int			InitializeFontHandle( HANDLEINFO *HandleInfo ) ;															// フォントハンドルを初期化する
extern	int			TerminateFontHandle( HANDLEINFO *HandleInfo ) ;																// フォントハンドルの後始末をする
extern	int			DumpInfoFontHandle( HANDLEINFO *HandleInfo ) ;																// フォントハンドルの情報出力

extern	int			RefreshFontDrawResourceToHandle( FONTMANAGE *ManageData, int ASyncThread = FALSE ) ;						// フォントハンドルが使用する描画バッファやテクスチャキャッシュを再初期化する
extern	int			SetAntialiasingFontOnlyFlag( int AntialiasingFontOnlyFlag ) ;												// アンチエイリアスフォントのみ使用できるか、フラグをセットする

extern	int			FontCacheStringDrawToHandleST(
							int DrawFlag,
							int   xi, int   yi,
							float xf, float yf, int PosIntFlag,
							int ExRateValidFlag,
							double ExRateX, double ExRateY,
							int RotateValidFlag,
							float RotCenterX, float RotCenterY, double RotAngle, 
							const wchar_t *StrData,
							unsigned int Color, MEMIMG *DestMemImg, const RECT *ClipRect,
							int TransFlag, FONTMANAGE *ManageData, unsigned int EdgeColor,
							int StrLen, int VerticalFlag, SIZE *DrawSize,
							int *LineCount, 
							DRAWCHARINFO *CharInfos, size_t CharInfoBufferSize, int *CharInfoNum, int CharInfoType /* 0:簡易情報  1:詳細情報 */,
							int OnlyType /* 0:通常描画 1:本体のみ 2:縁のみ */ ) ;
extern	int			RefreshDefaultFont( void ) ;																				// デフォルトフォントを再作成する
extern	int			InitFontToHandleBase( int Terminate = FALSE ) ;																// InitFontToHandle の内部関数

extern	FONTMANAGE *GetFontManageDataToHandle( int FontHandle ) ;																// フォント管理データの取得

extern	void		InitCreateFontToHandleGParam( CREATEFONTTOHANDLE_GPARAM *GParam ) ;											// CREATEFONTTOHANDLE_GPARAM のデータをセットする

extern	int			CreateFontToHandle_UseGParam(          CREATEFONTTOHANDLE_GPARAM *GParam, const wchar_t *FontName, int Size, int Thick, int FontType, int CharSet, int EdgeSize, int Italic, int Handle, int ASyncLoadFlag = FALSE ) ;	// CreateFontToHandle のグローバル変数にアクセスしないバージョン
extern	int			LoadFontDataFromMemToHandle_UseGParam( CREATEFONTTOHANDLE_GPARAM *GParam, const void *FontDataImage, int FontDataImageSize, int EdgeSize = -1, int Handle = -1, int ASyncLoadFlag = FALSE ) ;				// LoadFontDataFromMemToHandle のグローバル変数にアクセスしないバージョン
extern	int			LoadFontDataToHandle_UseGParam(        CREATEFONTTOHANDLE_GPARAM *GParam, const wchar_t FileName, int EdgeSize, int ASyncLoadFlag = FALSE ) ;																// LoadFontDataToHandle のグローバル変数にアクセスしないバージョン

extern	int			SetupFontCache( CREATEFONTTOHANDLE_GPARAM *GParam, FONTMANAGE *ManageData, int ASyncThread ) ;											// 文字キャッシュのセットアップを行う
extern	FONTCHARDATA *	FontCacheCharAddToHandle( int AddNum, const DWORD *CharCodeList, const DWORD *IVSCodeList, FONTMANAGE *ManageData, int TextureCacheUpdate = TRUE ) ;	// 文字キャッシュに新しい文字を加える
extern	int				FontCacheCharImageBltToHandle( FONTMANAGE *ManageData, FONTCHARDATA *CharData, DWORD CharCode, DWORD IVSCode, int Space, int ImageType /* DX_FONT_SRCIMAGETYPE_1BIT 等 */, void *ImageBuffer, DWORD ImageSizeX, DWORD ImageSizeY, DWORD ImagePitch, int ImageDrawX, int ImageDrawY, int ImageAddX, int TextureCacheUpdate ) ;		// 指定のフォントデータに画像を転送する
extern	int			FontTextureCacheAddUpdateRect( FONTMANAGE *ManageData, RECT *Rect ) ;	// フォントのテクスチャキャッシュの更新矩形を追加する
extern	int			FontTextureCacheUpdateRectApply( FONTMANAGE *ManageData ) ;				// フォントのテクスチャキャッシュの更新矩形を実際のテクスチャに適用する

extern	int			GetFontHandleCharCodeFormat( int FontHandle ) ;		// フォントハンドルに設定されている文字コード形式を取得する( 戻り値  -1:エラー  -1以外:文字コード形式 )

// 指定の文字コードのフォントキャッシュデータを取得する、キャッシュ内に無い場合はキャッシュへの追加を試みて、失敗したら NULL を返す
extern	FONTCHARDATA *GetFontCacheChar( FONTMANAGE *ManageData, DWORD CharCode, DWORD IVSCode, FONTMANAGE **UseManageData, int *DrawOffsetX, int *DrawOffsetY, int EnableGraphHandleFontImage, int ErrorMessage ) ;


// wchar_t版関数
extern	int			EnumFontName_WCHAR_T(							wchar_t *NameBuffer, int NameBufferNum, int JapanOnlyFlag = TRUE ) ;													// 使用可能なフォントの名前を列挙する( NameBuffer に 64バイト区切りで名前が格納されます )
extern	int			EnumFontNameEx_WCHAR_T(							wchar_t *NameBuffer, int NameBufferNum,                              int CharSet = -1 /* DX_CHARSET_DEFAULT 等 */ ) ;	// 使用可能なフォントの名前を列挙する( NameBuffer に 64バイト区切りで名前が格納されます )( 文字セット指定版 )
extern	int			EnumFontNameEx2_WCHAR_T(						wchar_t *NameBuffer, int NameBufferNum, const wchar_t *EnumFontName, int CharSet = -1 /* DX_CHARSET_DEFAULT 等 */ ) ;	// 指定のフォント名のフォントを列挙する
extern	int			CheckFontName_WCHAR_T(							const wchar_t *FontName, int CharSet = -1 /* DX_CHARSET_DEFAULT 等 */ ) ;												// 指定のフォント名のフォントが存在するかどうかをチェックする( 戻り値  TRUE:存在する  FALSE:存在しない )

extern	int			CreateFontToHandle_WCHAR_T(                     const wchar_t *FontName, int Size, int Thick, int FontType = -1 , int CharSet = -1 , int EdgeSize = -1 , int Italic = FALSE , int Handle = -1 ) ;		// フォントハンドルを作成する
extern	int			LoadFontDataToHandle_WCHAR_T(					const wchar_t *FileName,                      int EdgeSize = -1 ) ;			// フォントデータファイルからフォントハンドルを作成する
extern	int			ChangeFont_WCHAR_T(                             const wchar_t *FontName, int CharSet = -1 /* DX_CHARSET_SHFTJIS 等 */ ) ;							// デフォルトフォントハンドルで使用するフォントを変更
extern	int			SetDefaultFontState_WCHAR_T(                    const wchar_t *FontName, int Size, int Thick, int FontType = -1 , int CharSet = -1 , int EdgeSize = -1 , int Italic = FALSE ) ;	// デフォルトフォントハンドルの設定を変更する
extern	int			AddFontImageToHandle_WCHAR_T(					int FontHandle, const wchar_t *Char, int GrHandle, int DrawX, int DrawY, int AddX ) ;				// 指定の文字の代わりに描画するグラフィックハンドルを登録する
extern	int			SubFontImageToHandle_WCHAR_T(					int FontHandle, const wchar_t *Char ) ;															// 指定の文字の代わりに描画するグラフィックハンドルの登録を解除する

extern	int			GetDrawStringWidth_WCHAR_T(                     const wchar_t *String, int StrLen, int VerticalFlag ) ;									// デフォルトフォントハンドルを使用した文字列の描画幅を取得する
extern	int			GetDrawFormatStringWidth_WCHAR_T(               const wchar_t *FormatString, ... ) ;																// デフォルトフォントハンドルを使用した書式付き文字列の描画幅を取得する
extern	int			GetDrawExtendStringWidth_WCHAR_T(               double ExRateX, const wchar_t *String, int StrLen, int VerticalFlag ) ;					// デフォルトフォントハンドルを使用した文字列の描画幅を取得する( 拡大率付き )
extern	int			GetDrawExtendFormatStringWidth_WCHAR_T(         double ExRateX, const wchar_t *FormatString, ... ) ;												// デフォルトフォントハンドルを使用した書式付き文字列の描画幅を取得する( 拡大率付き )

extern	int			GetFontCharInfo_WCHAR_T(                        int FontHandle, const wchar_t *Char, int *DrawX, int *DrawY, int *NextCharX, int *SizeX, int *SizeY ) ;	// フォントハンドルの指定の文字の描画情報を取得する
extern	int			GetDrawStringWidthToHandle_WCHAR_T(             const wchar_t   *String, size_t StringLength, int StrLen, int FontHandle, int VerticalFlag ) ;		// フォントハンドルを使用した文字列の描画幅を取得する
extern	int			GetDrawFormatStringWidthToHandle_WCHAR_T(       int FontHandle, const wchar_t *FormatString, ... ) ;												// フォントハンドルを使用した書式付き文字列の描画幅を取得する
extern	int			GetDrawExtendStringWidthToHandle_WCHAR_T(       double ExRateX, const wchar_t *String, size_t StringLength, int StrLen, int FontHandle, int VerticalFlag ) ;	// フォントハンドルを使用した文字列の描画幅を取得する
extern	int			GetDrawExtendFormatStringWidthToHandle_WCHAR_T( double ExRateX, int FontHandle, const wchar_t *FormatString, ... ) ;								// フォントハンドルを使用した書式付き文字列の描画幅を取得する
extern	int			GetDrawStringSizeToHandle_WCHAR_T(              int *SizeX, int *SizeY, int *LineCount, const wchar_t   *String, size_t StringLength, int StrLen, int FontHandle, int VerticalFlag ) ;					// フォントハンドルを使用した文字列の描画幅・高さ・行数を取得する
extern	int			GetDrawExtendStringSizeToHandle_WCHAR_T(        int *SizeX, int *SizeY, int *LineCount, double ExRateX, double ExRateY, const wchar_t *String, size_t StringLength, int StrLen, int FontHandle, int VerticalFlag ) ;	// フォントハンドルを使用した文字列の描画幅・高さ・行数を取得する
extern	int			GetDrawStringCharInfoToHandle_WCHAR_T(          DRAWCHARINFO *InfoBuffer, size_t InfoBufferSize, const wchar_t *String, size_t StringLength, int StrLen, int FontHandle, int VerticalFlag ) ;									// フォントハンドルを使用した文字列の１文字毎の情報を取得する
extern	int			GetDrawExtendStringCharInfoToHandle_WCHAR_T(    DRAWCHARINFO *InfoBuffer, size_t InfoBufferSize, double ExRateX, double ExRateY, const wchar_t *String, size_t StringLength, int StrLen, int FontHandle, int VerticalFlag ) ;	// フォントハンドルを使用した文字列の１文字毎の情報を取得する
extern	int			GetDrawStringKerningPairInfoToHandle_WCHAR_T(   const wchar_t *PairChar, int *KernAmount, int FontHandle ) ;		// フォントハンドルを使用した二つの文字のペアのカーニング情報を取得する( PairChar:カーニング情報を調べるペアとなる2文字の文字列( 2文字以上あっても先頭の2文字だけ使用されます )  KernAmount:2文字目の文字を基本の位置からずらすドット数を代入するint型変数のアドレス )
extern	int			GetFontStateToHandle_WCHAR_T(                   wchar_t   *FontName, int *Size, int *Thick, int FontHandle, int *FontType = NULL , int *CharSet = NULL , int *EdgeSize = NULL , int *Italic = NULL ) ;		// フォントハンドルの情報を取得する

extern	int			FontCacheStringDrawToHandle_WCHAR_T(            int x, int y, const wchar_t *StrData, int StrLen, unsigned int Color, unsigned int EdgeColor, BASEIMAGE *DestImage, const RECT *ClipRect /* NULL 可 */ , int FontHandle, int VerticalFlag , SIZE *DrawSizeP = NULL ) ;
extern	int			FontBaseImageBlt_WCHAR_T(                       int x, int y, const wchar_t *StrData, int StrLen, BASEIMAGE *DestImage, BASEIMAGE *DestEdgeImage,                 int VerticalFlag ) ;	// 基本イメージに文字列を描画する( デフォルトフォントハンドルを使用する )
extern	int			FontBaseImageBltToHandle_WCHAR_T(               int x, int y, const wchar_t *StrData, int StrLen, BASEIMAGE *DestImage, BASEIMAGE *DestEdgeImage, int FontHandle, int VerticalFlag ) ;	// 基本イメージに文字列を描画する

extern	int			DrawString_WCHAR_T(                             int x, int y,                                              const wchar_t *String, size_t StringLength, unsigned int Color, unsigned int EdgeColor ) ;							// デフォルトフォントハンドルを使用して文字列を描画する
extern	int			DrawVString_WCHAR_T(                            int x, int y,                                              const wchar_t *String, size_t StringLength, unsigned int Color, unsigned int EdgeColor ) ;							// デフォルトフォントハンドルを使用して文字列を描画する( 縦書き )
extern	int			DrawFormatString_WCHAR_T(                       int x, int y,                                 unsigned int Color, const wchar_t *FormatString, ... ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を描画する
extern	int			DrawFormatVString_WCHAR_T(                      int x, int y,                                 unsigned int Color, const wchar_t *FormatString, ... ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を描画する( 縦書き )
extern	int			DrawExtendString_WCHAR_T(                       int x, int y, double ExRateX, double ExRateY,              const wchar_t *String, size_t StringLength, unsigned int Color, unsigned int EdgeColor ) ;							// デフォルトフォントハンドルを使用して文字列の拡大描画
extern	int			DrawExtendVString_WCHAR_T(                      int x, int y, double ExRateX, double ExRateY,              const wchar_t *String, size_t StringLength, unsigned int Color, unsigned int EdgeColor ) ;							// デフォルトフォントハンドルを使用して文字列の拡大描画( 縦書き )
extern	int			DrawExtendFormatString_WCHAR_T(                 int x, int y, double ExRateX, double ExRateY, unsigned int Color, const wchar_t *FormatString, ... ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVString_WCHAR_T(                int x, int y, double ExRateX, double ExRateY, unsigned int Color, const wchar_t *FormatString, ... ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )
extern	int			DrawRotaString_WCHAR_T(							int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor, int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// デフォルトフォントハンドルを使用して文字列を回転描画する
extern	int			DrawRotaFormatString_WCHAR_T(					int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor, int VerticalFlag , const wchar_t *FormatString , ... ) ;		// デフォルトフォントハンドルを使用して書式指定文字列を回転描画する
extern	int			DrawModiString_WCHAR_T(							int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, unsigned int EdgeColor , int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// デフォルトフォントハンドルを使用して文字列を変形描画する
extern	int			DrawModiFormatString_WCHAR_T(					int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, unsigned int EdgeColor , int VerticalFlag , const wchar_t *FormatString , ... ) ;		// デフォルトフォントハンドルを使用して書式指定文字列を変形描画する

extern	int			DrawStringF_WCHAR_T(                            float x, float y,                                              const wchar_t *String, size_t StringLength, unsigned int Color, unsigned int EdgeColor ) ;						// デフォルトフォントハンドルを使用して文字列を描画する( 座標指定が float 版 )
extern	int			DrawVStringF_WCHAR_T(                           float x, float y,                                              const wchar_t *String, size_t StringLength, unsigned int Color, unsigned int EdgeColor ) ;						// デフォルトフォントハンドルを使用して文字列を描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawFormatStringF_WCHAR_T(                      float x, float y,                                 unsigned int Color, const wchar_t *FormatString, ... ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を描画する( 座標指定が float 版 )
extern	int			DrawFormatVStringF_WCHAR_T(                     float x, float y,                                 unsigned int Color, const wchar_t *FormatString, ... ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawExtendStringF_WCHAR_T(                      float x, float y, double ExRateX, double ExRateY,              const wchar_t *String, size_t StringLength, unsigned int Color, unsigned int EdgeColor ) ;						// デフォルトフォントハンドルを使用して文字列の拡大描画( 座標指定が float 版 )
extern	int			DrawExtendVStringF_WCHAR_T(                     float x, float y, double ExRateX, double ExRateY,              const wchar_t *String, size_t StringLength, unsigned int Color, unsigned int EdgeColor ) ;						// デフォルトフォントハンドルを使用して文字列の拡大描画( 縦書き )( 座標指定が float 版 )
extern	int			DrawExtendFormatStringF_WCHAR_T(                float x, float y, double ExRateX, double ExRateY, unsigned int Color, const wchar_t *FormatString, ... ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する( 座標指定が float 版 )
extern	int			DrawExtendFormatVStringF_WCHAR_T(               float x, float y, double ExRateX, double ExRateY, unsigned int Color, const wchar_t *FormatString, ... ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawRotaStringF_WCHAR_T(						float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor , int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// デフォルトフォントハンドルを使用して文字列を回転描画する( 座標指定が float 版 )
extern	int			DrawRotaFormatStringF_WCHAR_T(					float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor , int VerticalFlag , const wchar_t *FormatString , ... ) ;		// デフォルトフォントハンドルを使用して書式指定文字列を回転描画する( 座標指定が float 版 )
extern	int			DrawModiStringF_WCHAR_T(						float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, unsigned int EdgeColor , int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// デフォルトフォントハンドルを使用して文字列を変形描画する( 座標指定が float 版 )
extern	int			DrawModiFormatStringF_WCHAR_T(					float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, unsigned int EdgeColor , int VerticalFlag , const wchar_t *FormatString , ... ) ;		// デフォルトフォントハンドルを使用して書式指定文字列を変形描画する( 座標指定が float 版 )

extern	int			DrawNumberPlusToI_WCHAR_T(                      int x, int y, const wchar_t *NoteString, int    Num, int RisesNum, unsigned int Color, unsigned int EdgeColor ) ;											// デフォルトフォントハンドルを使用して整数型の数値とその説明の文字列を一度に描画する
extern 	int			DrawNumberPlusToF_WCHAR_T(                      int x, int y, const wchar_t *NoteString, double Num, int Length,   unsigned int Color, unsigned int EdgeColor ) ;											// デフォルトフォントハンドルを使用して浮動小数点型の数値とその説明の文字列を一度に描画する

extern	int			DrawStringToZBuffer_WCHAR_T(                    int x, int y, const wchar_t *String, size_t StringLength,                                                                 int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;									// デフォルトフォントハンドルを使用してＺバッファに対して文字列を描画する
extern	int			DrawVStringToZBuffer_WCHAR_T(                   int x, int y, const wchar_t *String, size_t StringLength,                                                                 int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;									// デフォルトフォントハンドルを使用してＺバッファに対して文字列を描画する( 縦書き )
extern	int			DrawFormatStringToZBuffer_WCHAR_T(              int x, int y,                                                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */, const wchar_t *FormatString, ... ) ;	// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を描画する
extern	int			DrawFormatVStringToZBuffer_WCHAR_T(             int x, int y,                                                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */, const wchar_t *FormatString, ... ) ;	// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を描画する( 縦書き )
extern	int			DrawExtendStringToZBuffer_WCHAR_T(              int x, int y, double ExRateX, double ExRateY, const wchar_t *String, size_t StringLength,                                 int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;									// デフォルトフォントハンドルを使用してＺバッファに対して文字列を拡大描画する
extern	int			DrawExtendVStringToZBuffer_WCHAR_T(             int x, int y, double ExRateX, double ExRateY, const wchar_t *String, size_t StringLength,                                 int WriteZMode /* DX_ZWRITE_MASK 等 */ ) ;									// デフォルトフォントハンドルを使用してＺバッファに対して文字列を拡大描画する( 縦書き )
extern	int			DrawExtendFormatStringToZBuffer_WCHAR_T(        int x, int y, double ExRateX, double ExRateY,                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */, const wchar_t *FormatString, ... ) ;	// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVStringToZBuffer_WCHAR_T(       int x, int y, double ExRateX, double ExRateY,                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */, const wchar_t *FormatString, ... ) ;	// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を拡大描画する( 縦書き )
extern	int			DrawRotaStringToZBuffer_WCHAR_T(				int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, int WriteZMode /* DX_ZWRITE_MASK 等 */ , int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// デフォルトフォントハンドルを使用して文字列を回転描画する
extern	int			DrawRotaFormatStringToZBuffer_WCHAR_T(			int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, int WriteZMode /* DX_ZWRITE_MASK 等 */ , int VerticalFlag , const wchar_t *FormatString , ... ) ;		// デフォルトフォントハンドルを使用して書式指定文字列を回転描画する
extern	int			DrawModiStringToZBuffer_WCHAR_T(				int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4,                                      int WriteZMode /* DX_ZWRITE_MASK 等 */ , int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// デフォルトフォントハンドルを使用して文字列を変形描画する
extern	int			DrawModiFormatStringToZBuffer_WCHAR_T(			int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4,                                      int WriteZMode /* DX_ZWRITE_MASK 等 */ , int VerticalFlag , const wchar_t *FormatString , ... ) ;		// デフォルトフォントハンドルを使用して書式指定文字列を変形描画する


extern	int			DrawStringToHandle_WCHAR_T(                     int x, int y, const wchar_t *String, size_t StringLength, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag ) ;											// フォントハンドルを使用して文字列を描画する
extern	int			DrawVStringToHandle_WCHAR_T(                    int x, int y, const wchar_t *String, size_t StringLength, unsigned int Color, int FontHandle, unsigned int EdgeColor ) ;																		// フォントハンドルを使用して文字列を描画する( 縦書き )
extern	int			DrawFormatStringToHandle_WCHAR_T(               int x, int y, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;																						// フォントハンドルを使用して書式指定文字列を描画する
extern	int			DrawFormatVStringToHandle_WCHAR_T(              int x, int y, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;																						// フォントハンドルを使用して書式指定文字列を描画する( 縦書き )
extern	int			DrawExtendStringToHandle_WCHAR_T(               int x, int y, double ExRateX, double ExRateY, const wchar_t *String, size_t StringLength, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag ) ;			// フォントハンドルを使用して文字列を拡大描画する
extern	int			DrawExtendVStringToHandle_WCHAR_T(              int x, int y, double ExRateX, double ExRateY, const wchar_t *String, size_t StringLength, unsigned int Color, int FontHandle, unsigned int EdgeColor ) ;										// フォントハンドルを使用して文字列を拡大描画する( 縦書き )
extern	int			DrawExtendFormatStringToHandle_WCHAR_T(         int x, int y, double ExRateX, double ExRateY, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;														// フォントハンドルを使用して書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVStringToHandle_WCHAR_T(        int x, int y, double ExRateX, double ExRateY, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;														// フォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )
extern	int			DrawRotaStringToHandle_WCHAR_T(					int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// フォントハンドルを使用して文字列を回転描画する
extern	int			DrawRotaFormatStringToHandle_WCHAR_T(			int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag , const wchar_t *FormatString , ... ) ;		// フォントハンドルを使用して書式指定文字列を回転描画する
extern	int			DrawModiStringToHandle_WCHAR_T(					int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// フォントハンドルを使用して文字列を変形描画する
extern	int			DrawModiFormatStringToHandle_WCHAR_T(			int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag , const wchar_t *FormatString , ... ) ;		// フォントハンドルを使用して書式指定文字列を変形描画する

extern	int			DrawStringFToHandle_WCHAR_T(                    float x, float y, const wchar_t *String, size_t StringLength, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag ) ;										// フォントハンドルを使用して文字列を描画する( 座標指定が float 版 )
extern	int			DrawVStringFToHandle_WCHAR_T(                   float x, float y, const wchar_t *String, size_t StringLength, unsigned int Color, int FontHandle, unsigned int EdgeColor ) ;																	// フォントハンドルを使用して文字列を描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawFormatStringFToHandle_WCHAR_T(              float x, float y, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;																					// フォントハンドルを使用して書式指定文字列を描画する( 座標指定が float 版 )
extern	int			DrawFormatVStringFToHandle_WCHAR_T(             float x, float y, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;																					// フォントハンドルを使用して書式指定文字列を描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawExtendStringFToHandle_WCHAR_T(              float x, float y, double ExRateX, double ExRateY, const wchar_t *String, size_t StringLength, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag ) ;		// フォントハンドルを使用して文字列を拡大描画する( 座標指定が float 版 )
extern	int			DrawExtendVStringFToHandle_WCHAR_T(             float x, float y, double ExRateX, double ExRateY, const wchar_t *String, size_t StringLength, unsigned int Color, int FontHandle, unsigned int EdgeColor ) ;									// フォントハンドルを使用して文字列を拡大描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawExtendFormatStringFToHandle_WCHAR_T(        float x, float y, double ExRateX, double ExRateY, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;													// フォントハンドルを使用して書式指定文字列を拡大描画する( 座標指定が float 版 )
extern	int			DrawExtendFormatVStringFToHandle_WCHAR_T(       float x, float y, double ExRateX, double ExRateY, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;													// フォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawRotaStringFToHandle_WCHAR_T(				float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// フォントハンドルを使用して文字列を回転描画する( 座標指定が float 版 )
extern	int			DrawRotaFormatStringFToHandle_WCHAR_T(			float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag , const wchar_t *FormatString , ... ) ;		// フォントハンドルを使用して書式指定文字列を回転描画する( 座標指定が float 版 )
extern	int			DrawModiStringFToHandle_WCHAR_T(				float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag , const wchar_t *String, size_t StringLength              ) ;		// フォントハンドルを使用して文字列を変形描画する( 座標指定が float 版 )
extern	int			DrawModiFormatStringFToHandle_WCHAR_T(			float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, int FontHandle, unsigned int EdgeColor , int VerticalFlag , const wchar_t *FormatString , ... ) ;		// フォントハンドルを使用して書式指定文字列を変形描画する( 座標指定が float 版 )

extern	int			DrawNumberPlusToIToHandle_WCHAR_T(              int x, int y, const wchar_t *NoteString, int    Num, int RisesNum, unsigned int Color, int FontHandle, unsigned int EdgeColor ) ;											// フォントハンドルを使用して整数型の数値とその説明の文字列を一度に描画する
extern	int			DrawNumberPlusToFToHandle_WCHAR_T(              int x, int y, const wchar_t *NoteString, double Num, int Length,   unsigned int Color, int FontHandle, unsigned int EdgeColor ) ;											// フォントハンドルを使用して浮動小数点型の数値とその説明の文字列を一度に描画する


// va_list 関数
extern	int			GetDrawFormatStringWidth_VaList(				const TCHAR *FormatString, va_list VaList ) ;																																				// デフォルトフォントハンドルを使用した書式付き文字列の描画幅を取得する
extern	int			GetDrawExtendFormatStringWidth_VaList(			double ExRateX, const TCHAR *FormatString, va_list VaList ) ;																																// デフォルトフォントハンドルを使用した書式付き文字列の描画幅を取得する( 拡大率付き )
extern	int			GetDrawFormatStringSize_VaList(					int *SizeX, int *SizeY, int *LineCount, const TCHAR *FormatString, va_list VaList ) ;																										// デフォルトフォントハンドルを使用した書式付き文字列の描画幅・高さ・行数を取得する
extern	int			GetDrawExtendFormatStringSize_VaList(			int *SizeX, int *SizeY, int *LineCount, double ExRateX, double ExRateY, const TCHAR *FormatString, va_list VaList ) ;																		// デフォルトフォントハンドルを使用した書式付き文字列の描画幅・高さ・行数を取得する( 拡大率付き )
extern	int			GetDrawFormatStringCharInfo_VaList(				DRAWCHARINFO *InfoBuffer, size_t InfoBufferSize, const TCHAR *FormatString, va_list VaList ) ;																								// デフォルトフォントハンドルを使用した書式付き文字列の１文字毎の情報を取得する
extern	int			GetDrawExtendFormatStringCharInfo_VaList(		DRAWCHARINFO *InfoBuffer, size_t InfoBufferSize, double ExRateX, double ExRateY, const TCHAR *FormatString, va_list VaList ) ;																// デフォルトフォントハンドルを使用した書式付き文字列の１文字毎の情報を取得する
extern	int			GetDrawFormatStringWidthToHandle_VaList(		int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																																// フォントハンドルを使用した書式付き文字列の描画幅を取得する
extern	int			GetDrawExtendFormatStringWidthToHandle_VaList(	double ExRateX, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																												// フォントハンドルを使用した書式付き文字列の描画幅を取得する
extern	int			GetDrawFormatStringSizeToHandle_VaList(			int *SizeX, int *SizeY, int *LineCount, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																						// フォントハンドルを使用した書式付き文字列の描画幅・高さ・行数を取得する
extern	int			GetDrawExtendFormatStringSizeToHandle_VaList(	int *SizeX, int *SizeY, int *LineCount, double ExRateX, double ExRateY, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;														// フォントハンドルを使用した書式付き文字列の描画幅・高さ・行数を取得する
extern	int			GetDrawFormatStringCharInfoToHandle_VaList(		DRAWCHARINFO *InfoBuffer, size_t InfoBufferSize, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																				// フォントハンドルを使用した書式付き文字列の１文字毎の情報を取得する
extern	int			GetDrawExtendFormatStringCharInfoToHandle_VaList( DRAWCHARINFO *InfoBuffer, size_t InfoBufferSize, double ExRateX, double ExRateY, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;											// フォントハンドルを使用した書式付き文字列の１文字毎の情報を取得する

extern	int			DrawFormatString_VaList(						int x, int y,                                 unsigned int Color,                         const TCHAR *FormatString, va_list VaList ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を描画する
extern	int			DrawFormatVString_VaList(						int x, int y,                                 unsigned int Color,                         const TCHAR *FormatString, va_list VaList ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を描画する( 縦書き )
extern	int			DrawFormatString2_VaList(						int x, int y,                                 unsigned int Color, unsigned int EdgeColor, const TCHAR *FormatString, va_list VaList ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を描画する
extern	int			DrawFormatVString2_VaList(						int x, int y,                                 unsigned int Color, unsigned int EdgeColor, const TCHAR *FormatString, va_list VaList ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を描画する( 縦書き )
extern	int			DrawExtendFormatString_VaList(					int x, int y, double ExRateX, double ExRateY, unsigned int Color,                         const TCHAR *FormatString, va_list VaList ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVString_VaList(					int x, int y, double ExRateX, double ExRateY, unsigned int Color,                         const TCHAR *FormatString, va_list VaList ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )
extern	int			DrawExtendFormatString2_VaList(					int x, int y, double ExRateX, double ExRateY, unsigned int Color, unsigned int EdgeColor, const TCHAR *FormatString, va_list VaList ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVString2_VaList(				int x, int y, double ExRateX, double ExRateY, unsigned int Color, unsigned int EdgeColor, const TCHAR *FormatString, va_list VaList ) ;														// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )
extern	int			DrawRotaFormatString_VaList(					int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor, int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;	// デフォルトフォントハンドルを使用して書式指定文字列を回転描画する
extern	int			DrawModiFormatString_VaList(					int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, unsigned int EdgeColor, int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;					// デフォルトフォントハンドルを使用して書式指定文字列を変形描画する
extern	int			DrawFormatStringF_VaList(						float x, float y,                                 unsigned int Color,                         const TCHAR *FormatString, va_list VaList ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を描画する( 座標指定が float 版 )
extern	int			DrawFormatVStringF_VaList(						float x, float y,                                 unsigned int Color,                         const TCHAR *FormatString, va_list VaList ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawFormatString2F_VaList(						float x, float y,                                 unsigned int Color, unsigned int EdgeColor, const TCHAR *FormatString, va_list VaList ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を描画する( 座標指定が float 版 )
extern	int			DrawFormatVString2F_VaList(						float x, float y,                                 unsigned int Color, unsigned int EdgeColor, const TCHAR *FormatString, va_list VaList ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawExtendFormatStringF_VaList(					float x, float y, double ExRateX, double ExRateY, unsigned int Color,                         const TCHAR *FormatString, va_list VaList ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する( 座標指定が float 版 )
extern	int			DrawExtendFormatVStringF_VaList(				float x, float y, double ExRateX, double ExRateY, unsigned int Color,                         const TCHAR *FormatString, va_list VaList ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawExtendFormatString2F_VaList(				float x, float y, double ExRateX, double ExRateY, unsigned int Color, unsigned int EdgeColor, const TCHAR *FormatString, va_list VaList ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する( 座標指定が float 版 )
extern	int			DrawExtendFormatVString2F_VaList(				float x, float y, double ExRateX, double ExRateY, unsigned int Color, unsigned int EdgeColor, const TCHAR *FormatString, va_list VaList ) ;													// デフォルトフォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawRotaFormatStringF_VaList(					float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor, int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;		// デフォルトフォントハンドルを使用して書式指定文字列を回転描画する( 座標指定が float 版 )
extern	int			DrawModiFormatStringF_VaList(					float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, unsigned int EdgeColor, int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;									// デフォルトフォントハンドルを使用して書式指定文字列を変形描画する( 座標指定が float 版 )
extern	int			DrawFormatStringToZBuffer_VaList(				int x, int y,                                                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */ , const TCHAR *FormatString, va_list VaList ) ;									// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を描画する
extern	int			DrawFormatVStringToZBuffer_VaList(				int x, int y,                                                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */ , const TCHAR *FormatString, va_list VaList ) ;									// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を描画する( 縦書き )
extern	int			DrawExtendFormatStringToZBuffer_VaList(			int x, int y, double ExRateX, double ExRateY,                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */ , const TCHAR *FormatString, va_list VaList ) ;									// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVStringToZBuffer_VaList(		int x, int y, double ExRateX, double ExRateY,                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */ , const TCHAR *FormatString, va_list VaList ) ;									// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を拡大描画する( 縦書き )
extern	int			DrawRotaFormatStringToZBuffer_VaList(			int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, int WriteZMode /* DX_ZWRITE_MASK 等 */ , int VerticalFlag , const TCHAR *FormatString , va_list VaList ) ;				// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を回転描画する
extern	int			DrawModiFormatStringToZBuffer_VaList(			int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4,                                      int WriteZMode /* DX_ZWRITE_MASK 等 */ , int VerticalFlag , const TCHAR *FormatString , va_list VaList ) ;				// デフォルトフォントハンドルを使用してＺバッファに対して書式指定文字列を変形描画する

extern	int			DrawFormatStringToHandle_VaList(				int x, int y,                                 unsigned int Color,                         int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																				// フォントハンドルを使用して書式指定文字列を描画する
extern	int			DrawFormatVStringToHandle_VaList(				int x, int y,                                 unsigned int Color,                         int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																				// フォントハンドルを使用して書式指定文字列を描画する( 縦書き )
extern	int			DrawFormatString2ToHandle_VaList(				int x, int y,                                 unsigned int Color, unsigned int EdgeColor, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																				// フォントハンドルを使用して書式指定文字列を描画する
extern	int			DrawFormatVString2ToHandle_VaList(				int x, int y,                                 unsigned int Color, unsigned int EdgeColor, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																				// フォントハンドルを使用して書式指定文字列を描画する( 縦書き )
extern	int			DrawExtendFormatStringToHandle_VaList(			int x, int y, double ExRateX, double ExRateY, unsigned int Color,                         int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																				// フォントハンドルを使用して書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVStringToHandle_VaList(			int x, int y, double ExRateX, double ExRateY, unsigned int Color,                         int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																				// フォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )
extern	int			DrawExtendFormatString2ToHandle_VaList(			int x, int y, double ExRateX, double ExRateY, unsigned int Color, unsigned int EdgeColor, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																				// フォントハンドルを使用して書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVString2ToHandle_VaList(		int x, int y, double ExRateX, double ExRateY, unsigned int Color, unsigned int EdgeColor, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																				// フォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )
extern	int			DrawRotaFormatStringToHandle_VaList(			int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor, int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;	// フォントハンドルを使用して書式指定文字列を回転描画する
extern	int			DrawModiFormatStringToHandle_VaList(			int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FontHandle, unsigned int EdgeColor, int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;											// フォントハンドルを使用して書式指定文字列を変形描画する
extern	int			DrawFormatStringFToHandle_VaList(				float x, float y,                                 unsigned int Color,                         int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																			// フォントハンドルを使用して書式指定文字列を描画する( 座標指定が float 版 )
extern	int			DrawFormatVStringFToHandle_VaList(				float x, float y,                                 unsigned int Color,                         int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																			// フォントハンドルを使用して書式指定文字列を描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawFormatString2FToHandle_VaList(				float x, float y,                                 unsigned int Color, unsigned int EdgeColor, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																			// フォントハンドルを使用して書式指定文字列を描画する( 座標指定が float 版 )
extern	int			DrawFormatVString2FToHandle_VaList(				float x, float y,                                 unsigned int Color, unsigned int EdgeColor, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																			// フォントハンドルを使用して書式指定文字列を描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawExtendFormatStringFToHandle_VaList(			float x, float y, double ExRateX, double ExRateY, unsigned int Color,                         int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																			// フォントハンドルを使用して書式指定文字列を拡大描画する( 座標指定が float 版 )
extern	int			DrawExtendFormatVStringFToHandle_VaList(		float x, float y, double ExRateX, double ExRateY, unsigned int Color,                         int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																			// フォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawExtendFormatString2FToHandle_VaList(		float x, float y, double ExRateX, double ExRateY, unsigned int Color, unsigned int EdgeColor, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																			// フォントハンドルを使用して書式指定文字列を拡大描画する( 座標指定が float 版 )
extern	int			DrawExtendFormatVString2FToHandle_VaList(		float x, float y, double ExRateX, double ExRateY, unsigned int Color, unsigned int EdgeColor, int FontHandle, const TCHAR *FormatString, va_list VaList ) ;																			// フォントハンドルを使用して書式指定文字列を拡大描画する( 縦書き )( 座標指定が float 版 )
extern	int			DrawRotaFormatStringFToHandle_VaList(			float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor, int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;// フォントハンドルを使用して書式指定文字列を回転描画する( 座標指定が float 版 )
extern	int			DrawModiFormatStringFToHandle_VaList(			float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, int FontHandle, unsigned int EdgeColor, int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;							// フォントハンドルを使用して書式指定文字列を変形描画する( 座標指定が float 版 )
extern	int			DrawFormatStringToHandleToZBuffer_VaList(		int x, int y, int FontHandle,                                                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */ , const TCHAR *FormatString, va_list VaList ) ;							// フォントハンドルを使用してＺバッファに対して書式指定文字列を描画する
extern	int			DrawFormatVStringToHandleToZBuffer_VaList(		int x, int y, int FontHandle,                                                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */ , const TCHAR *FormatString, va_list VaList ) ;							// フォントハンドルを使用してＺバッファに対して書式指定文字列を描画する( 縦書き )
extern	int			DrawExtendFormatStringToHandleToZBuffer_VaList(	int x, int y, double ExRateX, double ExRateY, int FontHandle,                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */ , const TCHAR *FormatString, va_list VaList ) ;							// フォントハンドルを使用してＺバッファに対して書式指定文字列を拡大描画する
extern	int			DrawExtendFormatVStringToHandleToZBuffer_VaList(int x, int y, double ExRateX, double ExRateY, int FontHandle,                                                        int WriteZMode /* DX_ZWRITE_MASK 等 */ , const TCHAR *FormatString, va_list VaList ) ;							// フォントハンドルを使用してＺバッファに対して書式指定文字列を拡大描画する( 縦書き )
extern	int			DrawRotaFormatStringToHandleToZBuffer_VaList(	int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */ , int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;		// フォントハンドルを使用してＺバッファに対して書式指定文字列を回転描画する
extern	int			DrawModiFormatStringToHandleToZBuffer_VaList(	int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */ , int VerticalFlag, const TCHAR *FormatString, va_list VaList ) ;											// フォントハンドルを使用してＺバッファに対して書式指定文字列を変形描画する


// 環境依存関係
extern	int			InitFontManage_PF( void ) ;																																// InitFontManage の環境依存処理を行う関数
extern	int			TermFontManage_PF( void ) ;																																// TermFontManage の環境依存処理を行う関数

extern	int			CreateFontToHandle_PF( CREATEFONTTOHANDLE_GPARAM *GParam, FONTMANAGE *	ManageData, int DefaultCharSet ) ;												// CreateFontToHandle の環境依存処理を行う関数
extern	int			CreateFontToHandle_Error_PF( FONTMANAGE * ManageData ) ;																								// CreateFontToHandle の環境依存エラー処理を行う関数
extern	int			TerminateFontHandle_PF( FONTMANAGE *ManageData ) ;																										// TerminateFontHandle の環境依存処理を行う関数

extern	int			SetupFontCache_PF( CREATEFONTTOHANDLE_GPARAM *GParam, FONTMANAGE *ManageData, int ASyncThread ) ;														// SetupFontCache の環境依存処理を行う関数

extern	int			FontCacheCharAddToHandle_Timing0_PF( FONTMANAGE *ManageData ) ;																							// FontCacheCharaAddToHandleの環境依存処理を行う関数( 実行箇所区別 0 )
extern	int			FontCacheCharAddToHandle_Timing1_PF( FONTMANAGE *ManageData, FONTCHARDATA *CharData, DWORD CharCode, DWORD IVSCode, int TextureCacheUpdate ) ;			// FontCacheCharaAddToHandleの環境依存処理を行う関数( 実行箇所区別 1 )
extern	int			FontCacheCharAddToHandle_Timing2_PF( FONTMANAGE *ManageData ) ;																							// FontCacheCharaAddToHandleの環境依存処理を行う関数( 実行箇所区別 2 )

extern	int			EnumFontName_PF( ENUMFONTDATA *EnumFontData, int IsEx = FALSE, int CharSet = -1 ) ;																		// EnumFontName の環境依存処理を行う関数




#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FONT

#endif // DX_FONT_H

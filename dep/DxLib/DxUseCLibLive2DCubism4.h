// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		標準Ｃライブラリ使用コード　Live2D Cubism4 関係ヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

/*

	DxUseCLibLive2DCubism4.cpp は Live2D Cubism 4 SDK for Native の Cubism Native Framework 及び
	Cubism Native Samples がベースとなっていて、Live2D Open Software License が適用されます。
　　　- Live2D Open Software License 
　　　[日本語](http://www.live2d.com/eula/live2d-open-software-license-agreement_jp.html)
　　　[English](http://www.live2d.com/eula/live2d-open-software-license-agreement_en.html)

*/

#ifndef DX_USECLIB_LIVE2DCUBISM4_H
#define DX_USECLIB_LIVE2DCUBISM4_H

// インクルード ------------------------------------------------------------------
#include "DxCompileConfig.h"

#ifndef DX_NON_LIVE2D_CUBISM4
#include "DxLib.h"
#include "DxHandle.h"
#include "DxThread.h"
#include "DxGraphics.h"
#include "DxBaseFunc.h"
#include "DxStatic.h"

// マクロ定義 --------------------------------------------------------------------

// クラス・構造体・列挙型定義 ----------------------------------------------------

class D_CubismMotionQueueManager ;
class D_csmString ;

typedef void ( * csmLogFunctionP )( const char* message ) ;

typedef void( *D_CubismMotionEventFunction )( const D_CubismMotionQueueManager* caller, const D_csmString& eventValue, void* customData ) ;
typedef void* D_CubismMotionQueueEntryHandle ;
extern const D_CubismMotionQueueEntryHandle InvalidMotionQueueEntryHandleValue ;	// 無効なモーションの識別番号の定義

// モーションの優先度定数
const int D_CubismMotion_PriorityNone = 0;
const int D_CubismMotion_PriorityIdle = 1;
const int D_CubismMotion_PriorityNormal = 2;
const int D_CubismMotion_PriorityForce = 3;

// カラーブレンディングのモード
enum D_CubismBlendMode
{
	D_CubismBlendMode_Normal					= 0,		// 通常
	D_CubismBlendMode_Additive					= 1,		// 加算
	D_CubismBlendMode_Multiplicative			= 2,		// 乗算
} ;

// モーションカーブの種類
enum D_CubismMotionCurveTarget
{
	D_CubismMotionCurveTarget_Model,						// モデルに対して
	D_CubismMotionCurveTarget_Parameter,					// パラメータに対して
	D_CubismMotionCurveTarget_PartOpacity					// パーツの不透明度に対して
} ;

// モーションカーブのセグメントの種類
enum D_CubismMotionSegmentType
{
	D_CubismMotionSegmentType_Linear			= 0,		// リニア
	D_CubismMotionSegmentType_Bezier			= 1,		// ベジェ曲線
	D_CubismMotionSegmentType_Stepped			= 2,		// ステップ
	D_CubismMotionSegmentType_InverseStepped	= 3			// インバースステップ
} ;

// Alignment constraints.
enum
{
	D_csmAlignofMoc								= 64,		// Necessary alignment for mocs (in bytes).
	D_csmAlignofModel							= 16		// Necessary alignment for models (in bytes).
} ;

// Bit masks for non-dynamic drawable flags.
enum
{
	D_csmBlendAdditive							= 1 << 0,	// Additive blend mode mask.
	D_csmBlendMultiplicative					= 1 << 1,	// Multiplicative blend mode mask.
	D_csmIsDoubleSided							= 1 << 2,	// Double-sidedness mask.
	D_csmIsInvertedMask							= 1 << 3	// Clipping mask inversion mode mask.
} ;


// Bit masks for dynamic drawable flags.
enum
{
	D_csmIsVisible								= 1 << 0,	// Flag set when visible.
	D_csmVisibilityDidChange					= 1 << 1,	// Flag set when visibility did change.
	D_csmOpacityDidChange						= 1 << 2,	// Flag set when opacity did change.
	D_csmDrawOrderDidChange						= 1 << 3,	// Flag set when draw order did change.
	D_csmRenderOrderDidChange					= 1 << 4,	// Flag set when render order did change.
	D_csmVertexPositionsDidChange				= 1 << 5,	// Flag set when vertex positions did change.
	D_csmBlendColorDidChange					= 1 << 6	// Flag set when blend color did change.
} ;

// moc3 file format version.
enum
{
	D_csmMocVersion_Unknown						= 0,		// unknown
	D_csmMocVersion_30							= 1,		// moc3 file version 3.0.00 - 3.2.07
	D_csmMocVersion_33							= 2,		// moc3 file version 3.3.00 -
	D_csmMocVersion_40							= 3,		// moc3 file version 4.0.00 -
	D_csmMocVersion_42							= 4,		// moc3 file version 4.2.00 -
} ;

enum
{
	D_csmParameterType_Normal					= 0,		// Normal parameter
	D_csmParameterType_BlendShape				= 1			// Parameter for blend shape
} ;

// ベジェカーブの解釈方法のフラグタイプ
enum D_EvaluationOptionFlag
{
	D_EvaluationOptionFlag_AreBeziersRistricted	= 0,		// ベジェハンドルの規制状態
} ;

typedef int D_csmParameterType ;

#define CSM_STRING_SMALL_LENGTH					64
#define CSM_MAP_DEFAULT_SIZE					10
#define CSM_IDMANAGER_MAX_ID_NUM				65536


// 矩形形状(座標・長さはfloat値)を定義するクラス
class D_csmRectF
{
public:
	D_csmRectF() ;																			// コンストラクタ
	D_csmRectF( float x, float y, float w, float h ) ;										// 引数付きコンストラクタ
	virtual ~D_csmRectF() ;																	// デストラクタ
	float						GetCenterX() const { return X + 0.5f * Width; }				// 矩形中央のX座標を取得する
	float						GetCenterY() const { return Y + 0.5f * Height; }			// 矩形中央のY座標を取得する
	float						GetRight() const { return X + Width; }						// 右端のX座標を取得する
	float						GetBottom() const { return Y + Height; }					// 下端のY座標を取得する
	void						SetRect( D_csmRectF* r ) ;									// 矩形に値をセットする
	void						Expand( float w, float h ) ;								// 矩形中央を軸にして縦横を拡縮する

	float						X ;							// 左端X座標
	float						Y ;							// 上端Y座標
	float						Width ;						// 幅
	float						Height ;					// 高さ
} ;

// 2次元ベクトル型
struct D_CubismVector2
{
	float X ;          // X軸の値
	float Y ;          // Y軸の値

	D_CubismVector2() : X( 0.0f ), Y( 0.0f ){}														// コンストラクタ
	D_CubismVector2( float x, float y ) : X( x ), Y( y ){}											// デストラクタ
	friend D_CubismVector2		operator+( const D_CubismVector2& a, const D_CubismVector2& b ) ;	// ベクトルの加算
	friend D_CubismVector2		operator-( const D_CubismVector2& a, const D_CubismVector2& b ) ;	// ベクトルの減算
	friend D_CubismVector2		operator*( const D_CubismVector2& vector, const float scalar ) ;	// ベクトルの乗算（ベクトル値とスカラー値）
	friend D_CubismVector2		operator*( const float scalar, const D_CubismVector2& vector ) ;	// ベクトルの乗算（スカラー値とベクトル値）
	friend D_CubismVector2		operator/( const D_CubismVector2& vector, const float scalar ) ;	// ベクトルの除算（ベクトル値とスカラー値）
	const D_CubismVector2&		operator+=( const D_CubismVector2& rhs ) ;							// 加算
	const D_CubismVector2&		operator-=( const D_CubismVector2& rhs ) ;							// 減算
	const D_CubismVector2&		operator*=( const D_CubismVector2& rhs ) ;							// 乗算（ベクトル値）
	const D_CubismVector2&		operator/=( const D_CubismVector2& rhs ) ;							// 除算（ベクトル値）
	const D_CubismVector2&		operator*=( const float scalar ) ;									// 乗算（スカラー値）
	const D_CubismVector2&		operator/=( const float scalar ) ;									// 除算（スカラー値）
	bool						operator==( const D_CubismVector2& rhs ) const ;					// 等しさの確認（等しいか？）
	bool						operator!=( const D_CubismVector2& rhs ) const ;					// 等しさの確認（等しくないか？）
	void						Normalize() ;														// 正規化の適用
	float						GetLength() const ;													// ベクトルの長さの取得
	float						GetDistanceWith( D_CubismVector2 a ) const ;						// ベクトルの距離の取得
	float						Dot( const D_CubismVector2& a ) const ;								// ドット積の計算
} ;

// 4次元ベクトル型
struct D_CubismVector4
{
	float X ;          // X軸の値
	float Y ;          // Y軸の値
	float Z ;          // Z軸の値
	float W ;          // W軸の値
} ;

// Utility functions for csmVector2.
D_CubismVector2					operator+( const D_CubismVector2& a, const D_CubismVector2& b ) ;
D_CubismVector2					operator-( const D_CubismVector2& a, const D_CubismVector2& b ) ;
D_CubismVector2					operator*( const D_CubismVector2& vector, const float scalar ) ;
D_CubismVector2					operator*( const float scalar, const D_CubismVector2& vector ) ;
D_CubismVector2					operator/( const D_CubismVector2& vector, const float scalar ) ;

// 数値計算などに使用するユーティリティクラス
class D_CubismMath
{
public:
	static const float Pi;
	static float RangeF( float value, float min, float max ) ;								// 第一引数の値を最小値と最大値の範囲に収めた値を返す
#ifndef DX_NON_NAMESPACE
	static float SinF( float x ){ float Sin, Cos ; DxLib::_SINCOS( x, &Sin, &Cos ) ; return Sin ; }// サイン関数の値を求める
	static float CosF( float x ){ float Sin, Cos ; DxLib::_SINCOS( x, &Sin, &Cos ) ; return Cos ; }// コサイン関数の値を求める
	static float SqrtF( float x ){ return DxLib::_SQRT( x ) ; }									// 平方根(ルート)を求める
#else // DX_NON_NAMESPACE
	static float SinF( float x ){ float Sin, Cos ; _SINCOS( x, &Sin, &Cos ) ; return Sin ; }// サイン関数の値を求める
	static float CosF( float x ){ float Sin, Cos ; _SINCOS( x, &Sin, &Cos ) ; return Cos ; }// コサイン関数の値を求める
	static float SqrtF( float x ){ return _SQRT( x ) ; }									// 平方根(ルート)を求める
#endif // DX_NON_NAMESPACE
	static float AbsF( float x ){ return x < 0.0f ? -x : x ; }								// 絶対値の値を求める
	static float GetEasingSine( float value ) ;												// イージング処理されたサインを求める	
	static float Max( float l, float r ){ return ( l > r ) ? l : r ; }						// 大きい方の値を返す
	static float Min( float l, float r ){ return ( l > r ) ? r : l ; }						// 小さい方の値を返す。
	static float DegreesToRadian( float degrees ) ;											// 角度値をラジアン値に変換します。
	static float RadianToDegrees( float radian ) ;											// ラジアン値を角度値に変換します。
	static float DirectionToRadian( D_CubismVector2 from, D_CubismVector2 to ) ;			// 2つのベクトルからラジアン値を求めます。
	static float DirectionToDegrees( D_CubismVector2 from, D_CubismVector2 to ) ;			// 2つのベクトルから角度値を求めます。
	static D_CubismVector2 RadianToDirection( float totalAngle ) ;							// ラジアン値を方向ベクトルに変換します。

private:
	// privateコンストラクタ
	D_CubismMath() ;
} ;

// 文字列クラス
class D_csmString
{
public:
	D_csmString() ;																			// コンストラクタ
	D_csmString( const D_csmString& s ) ;													// 引数付きコンストラクタ
	D_csmString( const BYTE/*wchar_t*/ * c ) ;												// 引数付きコンストラクタ
	D_csmString( const char * c ) ;															// 引数付きコンストラクタ
	D_csmString( const char * c, int length ) ;												// 引数付きコンストラクタ
	D_csmString( const char * c, int length, bool usePtr ) ;								// 引数付きコンストラクタ
	virtual ~D_csmString() ;																// デストラクタ
	D_csmString&				operator=( const char* c ) ;								// =演算子のオーバーロード( D_csmString型 )
	bool						operator==( const D_csmString& s ) const ;					// ==演算子のオーバーロード( D_csmString型 )
	bool						operator==( const char* c ) const ;							// ==演算子のオーバーロード( char型 )
	D_csmString					operator+( const D_csmString& s ) const ;					// +演算子のオーバーロード(D_csmString型)
	D_csmString					operator+( const char* c ) const ;							// +演算子のオーバーロード(char型)
	D_csmString&				operator+=( const D_csmString& s ) ;						// +=演算子のオーバーロード(D_csmString型)
	D_csmString&				operator+=( const char* c ) ;								// +=演算子のオーバーロード(char型)
	D_csmString&				Append( const char* c, int length ) ;						// 文字列を後方に追加する
	D_csmString&				Append( int length, const char v ) ;						// 文字サイズを拡張して文字を埋める
	const char *				GetRawString() const ;										// C言語文字列としてのポインタを取得する
	const char *				GetRawStringA() ;											// C言語文字列としてのポインタを取得する( ＤＸライブラリの戻り値用 )
	const BYTE/*wchar_t*/ *		GetRawStringW() ;											// C言語文字列としてのポインタを取得する( wchar_t用 )

protected:
	void						Initialize( const char* c, int length, bool usePtr ) ;		// D_csmStringインスタンスの初期化関数。文字列のセットとハッシュコードの算出を行う。
	int							Copy( const BYTE/*wchar_t*/ * c, int length ) ;				// 文字列をコピーする. 終端に\0を追加する
	int							Copy( const char* c, int length ) ;							// 文字列をコピーする. 終端に\0を追加する
	int							CalcHashcode( const char* c, int length ) ;					// 文字列からハッシュ値を生成して返す
	void						Clear() ;													// ポインタを解放する

private:
	bool						IsEmpty() const ;											// 文字列が空かどうか？
	void						SetEmpty() ;												// 文字列を空に設定
	char *						WritePointer() ;											// C言語文字列としてのポインタを取得する

	char *						_ptr ;						// 文字型配列のポインタ
	int							_length ;					// 半角文字数（メモリ確保は最後に0が入るため_length+1）
	int							_hashcode ;					// インスタンスに当てられたハッシュ値
	char						_small[ 64 ] ;				// 文字列の長さが64-1未満の場合はこちらを使用

	bool						_enableA ;					// ＤＸライブラリの戻り値用のデータが有効かどうか
	char *						_ptrA ;						// 文字型配列のポインタ( ＤＸライブラリの戻り値用 )
	int							_lengthA ;					// 半角文字数（メモリ確保は最後に0が入るため_length+1）( ＤＸライブラリの戻り値用 )
	char						_smallA[ 64 ] ;				// 文字列の長さが64-1未満の場合はこちらを使用( ＤＸライブラリの戻り値用 )

	bool						_enableW ;					// wchar_t 用のデータが有効かどうか
	BYTE/*wchar_t*/ *			_ptrW ;						// 文字型配列のポインタ( wchar_t用 )
	int							_lengthW ;					// 半角文字数（メモリ確保は最後に0が入るため_length+1）( wchar_t用 )
	DWORD/*wchar_t*/			_smallW[ 64 ] ;				// 文字列の長さが64-1未満の場合はこちらを使用( wchar_t用 )
} ;

// 文字列クラス( wchar_t版 )
class D_csmStringW
{
public:
	D_csmStringW() ;																		// コンストラクタ
	D_csmStringW( const BYTE/*wchar_t*/ * c ) ;												// 引数付きコンストラクタ
	D_csmStringW( const char* c ) ;															// 引数付きコンストラクタ
	D_csmStringW( const BYTE/*wchar_t*/ * c, int length ) ;									// 引数付きコンストラクタ
	D_csmStringW( const D_csmStringW& s ) ;													// 引数付きコンストラクタ
	D_csmStringW( const BYTE/*wchar_t*/ * c, int length, bool usePtr ) ;						// 引数付きコンストラクタ
	virtual ~D_csmStringW() ;																// デストラクタ
	D_csmStringW&				operator=( const D_csmStringW& s ) ;						// =演算子のオーバーロード(D_csmStringW型)
	D_csmStringW&				operator=( const BYTE/*wchar_t*/ * c ) ;					// =演算子のオーバーロード(wchar_t型)
	bool						operator==( const D_csmStringW& s ) const ;					// ==演算子のオーバーロード(D_csmStringW型)
	bool						operator==( const BYTE/*wchar_t*/ * c ) const ;				// ==演算子のオーバーロード(wchar_t型)
	bool						operator<( const D_csmStringW& s ) const ;					// <演算子のオーバーロード(D_csmStringW型)
	bool						operator<( const BYTE/*wchar_t*/ * c ) const ;				// <演算子のオーバーロード(wchar_t型)
	bool						operator>( const D_csmStringW& s ) const ;					// >演算子のオーバーロード(D_csmStringW型)
	bool						operator>( const BYTE/*wchar_t*/ * c ) const ;				// >演算子のオーバーロード(wchar_t型)
	D_csmStringW				operator+( const D_csmStringW& s ) const ;					// +演算子のオーバーロード(D_csmStringW型)
	D_csmStringW				operator+( const BYTE/*wchar_t*/ * c ) const ;				// +演算子のオーバーロード(wchar_t型)
	D_csmStringW&				operator+=( const D_csmStringW& s ) ;						// +=演算子のオーバーロード(D_csmStringW型)
	D_csmStringW&				operator+=( const BYTE/*wchar_t*/ * c ) ;					// +=演算子のオーバーロード(wchar_t型)
	D_csmStringW&				Append( const BYTE/*wchar_t*/ * c, int length ) ;			//  文字列を後方に追加する
	D_csmStringW&				Append( int length, const DWORD/*wchar_t*/ v ) ;			// 文字サイズを拡張して文字を埋める
	int							GetLength() const { return _length; }						// 文字列の長さを返す
	const BYTE/*wchar_t*/ *		GetRawString() const ;										// C言語文字列としてのポインタを取得する
	void						Clear() ;													// ポインタを解放する
	int							GetHashcode() ;												// ハッシュコードを取得する

protected:
	void						Copy( const BYTE/*wchar_t*/ * c, int length ) ;				// 文字列をコピーする. 終端に\0を追加する
	void						Copy( const char* c, int length ) ;							// 文字列をコピーする. 終端に\0を追加する
	void						Initialize( const BYTE/*wchar_t*/ * c, int length, bool usePtr ) ;	// D_csmStringWインスタンスの初期化関数。文字列のセットとハッシュコードの算出を行う。
	int							CalcHashcode( const BYTE/*wchar_t*/ * c, int length ) ;				// 文字列からハッシュ値を生成して返す

private:
	static int					s_totalInstanceNo ;			// 通算のインスタンス番号
	BYTE/*wchar_t*/ *			_ptr ;						// 文字型配列のポインタ
	int							_length ;					// 半角文字数（メモリ確保は最後に0が入るため_length+1）
	int							_hashcode ;					// インスタンスに当てられたハッシュ値
	int							_instanceNo ;				// インスタンスに割り当てられた番号
	DWORD/*wchar_t*/ 			_small[ CSM_STRING_SMALL_LENGTH ] ;	// 文字列の長さがCSM_STRING_SMALL_LENGTH-1未満の場合はこちらを使用 

	bool						IsEmpty() const ;											// 文字列が空かどうか？
	void						SetEmpty() ;												// 文字列を空に設定
	BYTE/*wchar_t*/ *			WritePointer() ;											// C言語文字列としてのポインタを取得する
} ;

class D_CubismIdManager ;

// パラメータ名・パーツ名・Drawable名を保持
struct D_CubismId
{
	friend class D_CubismIdManager ;

	D_csmString&				GetString() { return _id ; }								// ID名を取得

private:
	D_CubismId(){}																			// コンストラクタ
	D_CubismId( const char* id ){ _id = id ; }												// コンストラクタ
	D_CubismId( const D_CubismId& c ) : _id( c._id ) {}
	~D_CubismId(){}																			// デストラクタ
	D_CubismId&					operator=( const D_CubismId& c ){ if( this != &c ){ _id = c._id ; } return *this ; }
	bool						operator==( const D_CubismId& c ) const { return ( _id == c._id ) ; }
	bool						operator!=( const D_CubismId& c ) const { return !( _id == c._id ) ; }

	D_csmString					_id ;														// ID名
} ;

typedef D_CubismId * D_CubismIdHandle ;

// ベクター型（可変配列型）
template< class T >
class D_csmVector
{
public :
	D_csmVector() ;																			// コンストラクタ
	D_csmVector( int initialCapacity, bool zeroClear = false ) ;							// 引数付きコンストラクタ
	D_csmVector( const D_csmVector& c ){ Copy( c ) ; }										// コピーコンストラクタ
	virtual ~D_csmVector() ;																	// デストラクタ

	T*							GetPtr(){ return _ptr ; }									// コンテナの先頭アドレスを返す
	T&							operator[]( int index ){ return _ptr[ index ]; }			// []演算子のオーバーロード
	const T&					operator[]( int index ) const { return _ptr[ index ]; }		// []演算子のオーバーロード( const)
	T&							At( int index ){ return _ptr[ index ] ;	}					// インデックスで指定した要素を返す
	void						PushBack( const T& value, bool callPlacementNew = true ) ;	// PushBack処理.コンテナに新たな要素を追加する。
	void						Clear() ;													// コンテナの全要素を解放する
	unsigned int				GetSize() const { return _size ; }							// コンテナの要素数を返す
	void						Resize( int size, T value = T() ){ UpdateSize( size, value, true ) ; }	// vector#resize()に相当するサイズ変更
	void						UpdateSize( int size, T value = T(), bool callPlacementNew = true ) ;	// vector#resize()に相当するサイズ変更
	void						PrepareCapacity( int newSize ) ;							// コンテナのキャパシティを確保する

	class iterator ;			// D_csmVector<T>のイテレータの前方宣言

	void						Insert( iterator position, iterator begin, iterator end, bool callPlacementNew = true ) ;		// コンテナにコンテナ要素を挿入する
	void						Assign( int newSize, T value = T(), bool callPlacementNew = true ) ;							// コンテナの全要素に対して代入処理を行う。
	bool						Remove( int index ) ;										// コンテナからインデックスで指定した要素を削除する

	// D_csmVector<T>のイテレータ
	class iterator
	{
		// D_csmVector<T>をフレンドクラスとする
		friend class D_csmVector ;

	public :
		iterator() : _index( 0 ) , _vector( NULL ){}										// コンストラクタ
		iterator( D_csmVector< T > *v ) : _index( 0 ), _vector( v ){}						// 引数付きコンストラクタ
		iterator( D_csmVector< T > *v, int idx ) : _index( idx ), _vector( v ){}			// 引数付きコンストラクタ
		iterator&				operator=( const iterator& ite ){ _index = ite._index ; _vector = ite._vector ; return *this ; }	// =演算子のオーバーロード
		iterator&				operator++(){ ++_index ; return *this ; }					// 前置++演算子のオーバーロード
		iterator&				operator--(){ --_index ; return *this ; }					// 前置--演算子のオーバーロード
		iterator				operator++( int ){ iterator iteold( _vector, _index++ ) ; return iteold ; }	// 後置++演算子のオーバーロード( intは後置用のダミー引数)
		iterator				operator--( int ){ iterator iteold( _vector, _index-- ) ; return iteold ; }	// 後置--演算子のオーバーロード( intは後置用のダミー引数)
		T&						operator*() const{ return _vector->_ptr[ _index ] ; }		// *演算子のオーバーロード
		bool					operator!=( const iterator& ite ) const { return ( _index != ite._index ) || ( _vector != ite._vector ) ; }	// !=演算子のオーバーロード

	public :
		int						_index ;					// コンテナのインデックス値
		D_csmVector< T >		*_vector ;					// コンテナの参照
	} ;

	//D_csmVector<T>のイテレータ（const）
	class const_iterator
	{
		// D_csmVector<T>をフレンドクラスとする
		friend class D_csmVector ;

	public :
		const_iterator() : _index( 0 ), _vector( NULL ){}									// コンストラクタ
		const_iterator( const D_csmVector< T >* v ) : _index( 0 ), _vector( v ){}			// 引数付きコンストラクタ
		const_iterator( const D_csmVector< T >* v, int idx ) : _index( idx ), _vector( v ){}	// 引数付きコンストラクタ
		const_iterator&			operator=( const const_iterator& ite ){ _index = ite._index ; _vector = ite._vector ; return *this ; }	// =演算子のオーバーロード
		const_iterator&			operator++(){ ++_index ; return *this ; }					// 前置++演算子のオーバーロード
		const_iterator&			operator--(){ --_index ; return *this ;	}					// 前置--演算子のオーバーロード
		const_iterator			operator++( int ){ const_iterator iteold( _vector, _index++ ) ; return iteold ; }	// 後置++演算子のオーバーロード( intは後置用のダミー引数)
		const_iterator			operator--( int ){ const_iterator iteold( _vector, _index-- ) ; return iteold ; }	// 後置--演算子のオーバーロード( intは後置用のダミー引数)
		T&						operator*() const { return _vector->_ptr[ _index] ; }		// *演算子のオーバーロード
		bool					operator!=( const const_iterator& ite ) const { return ( _index != ite._index ) || ( _vector != ite._vector ) ; }	// !=演算子のオーバーロード

	public:
		int						_index ;					// コンテナのインデックス値
		const D_csmVector< T >	*_vector ;					// コンテナのポインタ
	} ;

	const iterator				Begin(){ iterator ite( this, 0 ) ; return ite ; }			// コンテナの先頭要素を返す
	const iterator				End(){ iterator ite( this, _size ) ; return ite ; }			// コンテナの終端要素を返す
	const const_iterator		Begin() const { const_iterator ite( this, 0 ) ; return ite ; }	// コンテナの先頭要素を返す
	const const_iterator		End() const { const_iterator ite( this, _size ) ; return ite ; }	// コンテナの終端要素を返す
	const iterator				Erase( const iterator& ite )								// コンテナから要素を削除して他の要素をシフトする
	{
		int index = ite._index ;
		if( index < 0 || _size <= index ) return ite ;
		if( index < _size - 1 ) _MEMMOVE( &( _ptr[ index ] ), &( _ptr[ index + 1 ] ), sizeof( T ) * ( _size - index - 1 ) ) ;
		--_size ;
		iterator ite2( this, index ) ;
		return ite2 ;
	}
	const const_iterator		Erase( const const_iterator& ite )							// コンテナから要素を削除して他の要素をシフトする
	{
		int index = ite._index ;
		if( index < 0 || _size <= index ) return ite ;
		if( index < _size - 1 ) _MEMMOVE( &( _ptr[ index ] ), &( _ptr[ index + 1 ] ), sizeof( T ) * ( _size - index - 1 ) ) ;
		--_size ;
		const_iterator ite2( this, index ) ;
		return ite2 ;
	}

	D_csmVector&				operator=( const D_csmVector& c ){ if( this != &c ){ Clear() ; Copy( c ) ; } return *this ; }	// コピーコンストラクタ

public:
	void						Copy( const D_csmVector& c ) ;	// D_csmVector<T>のコピー関数

	T*							_ptr ;						// コンテナの先頭アドレス（ポインタ）
	int							_size ;						// コンテナの要素数（サイズ）
	int							_capacity ;					// コンテナのキャパシティ
} ;

// Key-D_JsonValueのペアを定義するクラス
template< class _KeyT, class _ValT >
class D_csmPair
{
public:
	D_csmPair() : First(), Second() {}														// コンストラクタ
	D_csmPair( const _KeyT& key ) : First( key ), Second() {}								// 引数付きコンストラクタ
	D_csmPair( const _KeyT& key, const _ValT& value ) : First( key ), Second( value ) {}	// 引数付きコンストラクタ
	virtual ~D_csmPair() {}																	// デストラクタ

	_KeyT						First ;						// Keyとして用いる変数
	_ValT						Second ;					// D_JsonValueとして用いる変数
} ;

// マップ型
template< class _KeyT, class _ValT >
class D_csmMap
{
public:
	D_csmMap() ;																			// コンストラクタ
	D_csmMap( int size ) ;																	// 引数付きコンストラクタ
	virtual ~D_csmMap() ;																	// デストラクタ
	void						AppendKey( _KeyT& key ) ;									// キーを追加する
	_ValT&						operator[]( _KeyT key ) ;									// 添字演算子[key]のオーバーロード
	const _ValT&				operator[]( _KeyT key ) const ;								// 添字演算子[key]のオーバーロード( const)
	bool						IsExist( _KeyT key ) ;										// 引数で渡したKeyを持つ要素が存在するか
	void						Clear() ;													// Key-D_JsonValueのポインタを全て解放する
	int							GetSize() const { return _size ; }							// コンテナのサイズを取得する
	void						PrepareCapacity( int newSize, bool fitToSize ) ;			// コンテナのキャパシティを確保する

	// D_csmMap<T>のイテレータ
	class iterator
	{
		// D_csmMap<T>をフレンドクラスとする
		friend class D_csmMap;

	public:
		iterator() : _index( 0 ), _map( NULL ){}											// コンストラクタ
		iterator( D_csmMap< _KeyT, _ValT >* v ) : _index( 0 ), _map( v ){}					// 引数付きコンストラクタ
		iterator( D_csmMap< _KeyT, _ValT >* v, int idx ) : _index( idx ), _map( v ) {}		// 引数付きコンストラクタ
		iterator&				operator=( const iterator& ite ){ _index = ite._index ; _map = ite._map ; return *this ; }	// =演算子のオーバーロード
		iterator&				operator++(){ _index++ ; return *this ; }					// 前置++演算子のオーバーロード
		iterator&				operator--(){ _index-- ; return *this ; }					// 前置--演算子のオーバーロード
		iterator				operator++( int ){ iterator iteold( _map, _index++ ) ; return iteold ; }	// 後置++演算子のオーバーロード( intは後置用のダミー引数)
		iterator				operator--( int ){ iterator iteold( _map, _index-- ) ; return iteold ; }	// 後置--演算子のオーバーロード( intは後置用のダミー引数)
		D_csmPair< _KeyT, _ValT >& operator*() const { return _map->_keyValues[ _index ] ; }	// *演算子のオーバーロード
		bool					operator!=( const iterator& ite ) const { return ( _index != ite._index ) || ( _map != ite._map ) ;	}	// !=演算子のオーバーロード

	private:
		int						_index ;					// コンテナのインデックス値
		D_csmMap< _KeyT, _ValT >* _map ;					// コンテナのポインタ
	} ;

	// D_csmMap<T>のイテレータ( const)
	class const_iterator
	{
		// D_csmMap<T>をフレンドクラスとする
		friend class D_csmMap;

	public:
		const_iterator() : _index( 0 ), _map( NULL ) {}										// コンストラクタ
		const_iterator( const D_csmMap< _KeyT, _ValT >* v ) : _index( 0 ), _map( v ) {}		// 引数付きコンストラクタ
		const_iterator( const D_csmMap< _KeyT, _ValT >* v, int idx ) : _index( idx ), _map( v ) {}	// 引数付きコンストラクタ
		const_iterator&			operator=( const const_iterator& ite ){ _index = ite._index ; _map = ite._map ; return *this ; }	// =演算子のオーバーロード
		const_iterator&			operator++(){ ++_index ; return *this ; }					// 前置++演算子のオーバーロード
		const_iterator&			operator--(){ --_index ; return *this ; }					// 前置--演算子のオーバーロード
		const_iterator			operator++( int ){ const_iterator iteold( _map, _index++ ) ; return iteold ; }	// 後置++演算子のオーバーロード( intは後置用のダミー引数)
		const_iterator			operator--( int ){ const_iterator iteold( _map, _index-- ) ; return iteold ; }	// 後置--演算子のオーバーロード( intは後置用のダミー引数)
		D_csmPair< _KeyT, _ValT >* operator->() const { return &_map->_keyValues[ _index ] ; }	// ->演算子のオーバーロード
		D_csmPair< _KeyT, _ValT >& operator*() const { return _map->_keyValues[ _index ] ; }	// *演算子のオーバーロード
		bool					operator!=( const const_iterator& ite ) const { return ( _index != ite._index ) || ( _map != ite._map ) ; }	// !=演算子のオーバーロード

	private:
		int						_index ;					// コンテナのインデックス値
		const D_csmMap< _KeyT, _ValT >* _map ;				// コンテナのポインタ( const)
	} ;

	const const_iterator		Begin() const { const_iterator ite( this, 0 ) ; return ite ; }	// コンテナの先頭要素を返す
	const const_iterator		End() const { const_iterator ite( this, _size ) ; return ite ; }	// コンテナの終端要素を返す
	const iterator				Erase( const iterator& ite )								// コンテナから要素を削除する
	{
		int index = ite._index ;
		if( index < 0 || _size <= index ) return ite ;
		if( index < _size - 1 )
			_MEMMOVE( &( _keyValues[ index ] ), &( _keyValues[ index + 1 ] ), sizeof( D_csmPair< _KeyT, _ValT > ) * ( _size - index - 1 ) ) ;
		--_size ;
		iterator ite2( this, index ) ;
		return ite2 ;
	}
	const const_iterator		Erase( const const_iterator& ite )							// コンテナから要素を削除する
	{
		int index = ite._index ;
		if( index < 0 || _size <= index ) return ite ;
		if( index < _size - 1 )
			_MEMMOVE( &( _keyValues[ index ] ), &( _keyValues[ index + 1 ] ), sizeof( D_csmPair< _KeyT, _ValT > ) * ( _size - index - 1 ) ) ;
		--_size ;
		const_iterator ite2( this, index ) ;
		return ite2 ;
	}

public:
	D_csmPair< _KeyT, _ValT > *	_keyValues ;				// Key-D_JsonValueペアの配列
	_ValT *						_dummyValuePtr ;			// 空の値を返すためのダミー(staticのtemplteを回避するためメンバとする）
	int							_size ;						// コンテナの要素数（サイズ）
	int							_capacity ;					// コンテナのキャパシティ
} ;

// ID名の管理
class D_CubismIdManager
{
	friend struct D_CubismId ;

public:
	D_CubismIdManager() ;																	// コンストラクタ
	~D_CubismIdManager() ;																	// デストラクタ
	void						RegisterIds( const char** ids, int count ) ;				// ID名をリストから登録
	void						RegisterIds( const D_csmVector< D_csmString >& ids ) ;		// ID名をリストから登録
	D_CubismId *				RegisterId( const char* id ) ;								// ID名を登録
	D_CubismId *				RegisterId( const D_csmString& id ) ;						// ID名を登録
	D_CubismId *				GetId( const D_csmString& id ) ;							// ID名からIDを取得する
	D_CubismId *				GetId( const char* id ) ;									// ID名からIDを取得する
	D_CubismId *				FindId( const char* id ) const ;							// ID名からIDを検索
	bool						IsExist( const D_csmString& id ) const ;					// ID名からIDの確認
	bool						IsExist( const char* id ) const ;							// ID名からIDの確認

private:
	D_CubismIdManager( const D_CubismIdManager& ) ;
	D_CubismIdManager& operator=( const D_CubismIdManager& ) ;

	D_CubismId *				_ids[ CSM_IDMANAGER_MAX_ID_NUM ] ;							// 登録されているIDのリスト
	volatile int				_idNum ;													// 登録されているIDの数
#ifndef DX_NON_NAMESPACE
	DxLib::DX_CRITICAL_SECTION	_criticalSection ;											// クリティカルセクション
#else // DX_NON_NAMESPACE
	DX_CRITICAL_SECTION			_criticalSection ;											// クリティカルセクション
#endif // DX_NON_NAMESPACE
} ;

class D_JsonValue ;
class D_JsonError ;
class D_JsonNullValue ;

#define D_CSM_JSON_ERROR_TYPE_MISMATCH            "D_JsonError:type mismatch"
#define D_CSM_JSON_ERROR_INDEX_OUT_OF_BOUNDS      "D_JsonError:index out of bounds"

// パースしたJSONエレメントの要素の基底クラス
class D_JsonValue
{
	friend class D_JsonArray ;

public:
	static D_JsonValue*			ErrorValue ;				// 一時的な返り値として返すエラー。CubismFramework::Dispose()するまではCSM_DELETEしない。
	static D_JsonValue*			NullValue ;					// 一時的な返り値として返すNULL。CubismFramework::Dispose()するまではCSM_DELETEしない。
	D_JsonValue() {}																		// コンストラクタ
	virtual ~D_JsonValue() {}																// デストラクタ
	virtual D_csmString&		GetString( const D_csmString& defaultValue = "", const D_csmString& indent = "" ) = 0 ;// 要素を文字列で返す(D_csmString型)
	virtual const char*			GetRawString( const D_csmString& defaultValue = "", const D_csmString& indent = "" ) ;// 要素を文字列で返す(char*)
	virtual const char*			GetRawStringA( const D_csmString& defaultValue = "", const D_csmString& indent = "" ) ;// 要素を文字列で返す(ＤＸライブラリ用)
	virtual const BYTE/*wchar_t*/ *	GetRawStringW( const D_csmString& defaultValue = "", const D_csmString& indent = "" ) ;// 要素を文字列で返す(wchar_t*)
	virtual int					ToInt( int defaultValue = 0 ) { return defaultValue ; }			// 要素を数値型で返す( int)
	virtual float				ToFloat( float defaultValue = 0.0f ) { return defaultValue ; }	// 要素を数値型で返す(float)
	virtual bool				ToBoolean( bool defaultValue = false ) { return defaultValue ; }	// 要素を真偽値で返す( bool)
	virtual int					GetSize() { return 0; }											// 要素を真偽値で返す( bool)
	virtual D_csmVector<D_JsonValue*>*				GetVector( D_csmVector<D_JsonValue*>* defaultValue = NULL ) { return defaultValue ; }		// 要素をコンテナで返す(D_csmVector<D_JsonValue*>)
	virtual D_csmMap<D_csmString, D_JsonValue*>*	GetMap( D_csmMap<D_csmString, D_JsonValue*>* defaultValue = NULL ) { return defaultValue ; }	// 要素をマップで返す(D_csmMap<D_csmString, D_JsonValue*>)
	virtual D_csmVector<D_csmString>&				GetKeys(){ return *s_dummyKeys ; }															// マップのキー一覧をコンテナで返す
	virtual D_JsonValue&		operator[]( int index )	;									// 添字演算子[int]
	virtual D_JsonValue&		operator[]( const D_csmString& string )	;					// 添字演算子[D_csmString]
	virtual D_JsonValue&		operator[]( const char* s )	;								// 添字演算子[char*]
	virtual bool				IsError() { return false ; }								// D_JsonValueの種類がエラー値ならtrue。
	virtual bool				IsNull() { return false ; }									// D_JsonValueの種類がNULL値ならtrue。
	virtual bool				IsBool() { return false ; }									// D_JsonValueの種類が真偽値ならtrue。
	virtual bool				IsFloat() { return false ; }								// D_JsonValueの種類が数値型ならtrue。
	virtual bool				IsString() { return false ; }								// D_JsonValueの種類が文字列ならtrue。
	virtual bool				IsArray() { return false ; }								// D_JsonValueの種類が配列ならtrue。
	virtual bool				IsMap() { return false ; }									// D_JsonValueの種類がマップ型ならtrue。
	virtual bool				Equals( const D_csmString& /*value*/ ) { return false ; }	// 引数の値と等しければtrue。
	virtual bool				Equals( const char* /*value*/ ) { return false ; }			// 引数の値と等しければtrue。
	virtual bool				Equals( int /*value*/ ) { return false ; }					// 引数の値と等しければtrue。
	virtual bool				Equals( float /*value*/ ) { return false ; }				// 引数の値と等しければtrue。
	virtual bool				Equals( bool /*value*/ ) { return false ; }					// 引数の値と等しければtrue。
	virtual bool				IsStatic() { return false ; }								// D_JsonValueの値が静的ならtrue. 静的なら解放しない
	virtual D_JsonValue*		SetErrorNotForClientCall( const char* /*errorStr*/ ) { return ErrorValue ; }// D_JsonValueにエラー値をセットする

	static void					StaticInitializeNotForClientCall() ;							//  初期化用メソッド
	static void					StaticReleaseNotForClientCall() ;							// リリース用メソッド
protected:
	D_csmString					_stringBuffer ;				// 文字列バッファ

private:
	static D_csmVector<D_csmString>* s_dummyKeys ;			// ダミーキー
} ;

// Ascii文字のみ対応した最小限の軽量JSONパーサ。
class D_CubismJson
{
public:
	static D_CubismJson*		Create( const BYTE* buffer, size_t size ) ;					// バイトデータから直接ロードしてパースする
	static void					Delete( D_CubismJson* instance ) ;							// パースしたJSONオブジェクトの解放処理
	D_JsonValue&				GetRoot() const ;											// パースしたJSONのルート要素のポインタを返す
	const char*					GetParseError() const { return _error ; }					// パース時のエラー値を返す
	bool						CheckEndOfFile() const { return ( *_root )[ 1 ].Equals( "EOF" ) ; }// ルート要素の次の要素がファイルの終端だったらtrueを返す

protected:
	bool ParseBytes( const BYTE* buffer, int size ) ;// JSONのパースを実行する
	D_csmString ParseString( const char* string, int length, int begin, int* outEndPos ) ;	// 次の「"」までの文字列をパースする。文字列は外部で解放する必要がある。
	D_JsonValue* ParseObject( const char* buffer, int length, int begin, int* outEndPos ) ;	// JSONのオブジェクトエレメントをパースしてD_JsonValueオブジェクトを返す
	D_JsonValue* ParseArray( const char* buffer, int length, int begin, int* outEndPos ) ;	// JSONの配列エレメントをパースしてD_JsonValueオブジェクトを返す
	D_JsonValue* ParseValue( const char* buffer, int length, int begin, int* outEndPos ) ;	// JSONエレメントからD_JsonValue(float,D_JsonString,D_JsonValue*,D_JsonArray,null,true,false)をパースするエレメントの書式に応じて内部でParseString(), ParseObject(), ParseArray()を呼ぶ

private:
	D_CubismJson() ;																			// コンストラクタ
	D_CubismJson( const BYTE* buffer, int length ) ;											// 引数付きコンストラクタ
	virtual ~D_CubismJson() ;																// デストラクタ

	const char*					_error ;						// パース時のエラー
	int							_lineCount ;					// エラー報告に用いる行数カウント
	D_JsonValue*				_root ;						// パースされたルート要素
} ;

// パースしたJSONの要素をDouble値として扱う
class D_JsonFloat : public D_JsonValue
{
public:
	D_JsonFloat( float v ) : D_JsonValue() { this->_value = v; }							// コンストラクタ
	virtual ~D_JsonFloat() {}																// デストラクタ
	virtual bool				IsFloat() { return true ; }									// D_JsonValueの種類が数値型ならtrue。
	virtual D_csmString&		GetString( const D_csmString& defaultValue = "", const D_csmString& indent = "" ) ;	// 要素を文字列で返す(D_csmString型)
	virtual int					ToInt( int /*defaultValue*/ = 0 ) { return static_cast< int >( this->_value ) ; }	// 要素を数値型で返す( int)
	virtual float				ToFloat( float /*defaultValue*/ = 0 ) { return this->_value ; }						// 要素を数値型で返す(float)
	virtual bool				Equals( float v ) { return v == this->_value ; }			// 引数の値と等しければtrue
	virtual bool				Equals( const D_csmString& /*v*/ ) { return false ; }		// 引数の値と等しければtrue
	virtual bool				Equals( const char* /*v*/ ) { return false ; }				// 引数の値と等しければtrue
	virtual bool				Equals( int /*v*/ ) { return false ; }						// 引数の値と等しければtrue
	virtual bool				Equals( bool /*v*/ ) { return false ; }						// 引数の値と等しければtrue

private:
	float						_value ;						// JSON要素の値
} ;

// パースしたJSONの要素を真偽値として扱う
class D_JsonBoolean : public D_JsonValue
{
	friend class D_JsonValue ;

public:
	static D_JsonBoolean*		TrueValue ; // true
	static D_JsonBoolean*		FalseValue ;// false
	virtual ~D_JsonBoolean() {}																// デストラクタ
	virtual bool				IsBool() { return true ; }									// D_JsonValueの種類が真偽値ならtrue。
	virtual bool				ToBoolean( bool /*defaultValue = false*/ ) { return _boolValue ; }// 要素を真偽値で返す( bool)
	virtual D_csmString&		GetString( const D_csmString& defaultValue = "", const D_csmString& indent = "" ) ;// 要素を文字列で返す(D_csmString型)
	virtual bool				Equals( bool v ) { return v == _boolValue ; }				// 引数の値と等しければtrue。
	virtual bool				Equals( const D_csmString& /*v*/ ) { return false ; }		// 引数の値と等しければtrue。
	virtual bool				Equals( const char* /*v*/ ) { return false ; }				// 引数の値と等しければtrue。
	virtual bool				Equals( int /*v*/ ) { return false ; }						// 引数の値と等しければtrue。
	virtual bool				Equals( float /*v*/ ) { return false ; }					// 引数の値と等しければtrue。
	virtual bool				IsStatic() { return true ; }								// D_JsonValueの値が静的ならtrue. 静的なら解放しない

private:
	D_JsonBoolean( bool v ) : D_JsonValue() { this->_boolValue = v; }						// 引数付きコンストラクタ
	bool						_boolValue ;					// JSON要素の値
} ;

// パースしたJSONの要素を文字列として扱う
class D_JsonString : public D_JsonValue
{
public:
	D_JsonString( const D_csmString& s ) : D_JsonValue() { this->_stringBuffer = s ; }		// 引数付きコンストラクタ
	D_JsonString( const char* s ) : D_JsonValue() { this->_stringBuffer = s ; }				// 引数付きコンストラクタ
	virtual ~D_JsonString() {}																// デストラクタ
	virtual bool				IsString() { return true ; }									// D_JsonValueの種類が文字列ならtrue。
	virtual D_csmString&		GetString( const D_csmString& defaultValue = "", const D_csmString& indent = "" ) ;// 要素を文字列で返す(D_csmString型)
	virtual bool				Equals( const D_csmString& v ) { return ( _stringBuffer == v ) ; }// 引数の値と等しければtrue。
	virtual bool				Equals( const char* v ) { return ( _stringBuffer == v ) ; }	// 引数の値と等しければtrue。
	virtual bool				Equals( int /*v*/ ) { return false ; }						// 引数の値と等しければtrue。
	virtual bool				Equals( float /*v*/ ) { return false ; }					// 引数の値と等しければtrue。
	virtual bool				Equals( bool /*v*/ ) { return false ; }						// 引数の値と等しければtrue。
} ;

// JSONパース時のエラー結果。文字列型のように振る舞う
class D_JsonError : public D_JsonString
{
	friend class D_JsonValue ; //
	friend class D_JsonArray ; //
	friend class D_CubismJson ; //

public:
	virtual bool				IsStatic() { return _isStatic ; }							// D_JsonValueの値が静的ならtrue. 静的なら解放しない
	virtual D_JsonValue*		SetErrorNotForClientCall( const char* s ){ _stringBuffer = s ; return this ; }	//  エラー情報をセットする

protected:
	D_JsonError( const D_csmString& s, bool isStatic ) : D_JsonString( s ), _isStatic( isStatic ) {}// 引数付きコンストラクタ
	virtual ~D_JsonError() {}																// デストラクタ
	virtual bool				IsError() { return true ; }									// D_JsonValueの種類がエラー値ならtrue。

	bool						_isStatic ;					// 静的なD_JsonValueかどうか
} ;

// パースしたJSONの要素をNull値として持つ
class D_JsonNullValue : public D_JsonValue
{
	friend class D_JsonValue ; //
	friend class D_CubismJson ; //

public:
	virtual ~D_JsonNullValue() {}															// デストラクタ
	virtual bool IsNull() { return true ; }													// D_JsonValueの種類がNULL値ならtrue。
	virtual D_csmString& GetString( const D_csmString& /*defaultValue = ""*/, const D_csmString& /*indent = ""*/ ){ return _stringBuffer ; }	// 要素を文字列で返す(D_csmString型)
	virtual bool IsStatic() { return true ; }												// D_JsonValueの値が静的ならtrue. 静的なら解放しない

private:
	D_JsonNullValue() : D_JsonValue() { _stringBuffer = "D_JsonNullValue"; }				//  コンストラクタ
} ;

// パースしたJSONの要素を配列として持つ
class D_JsonArray : public D_JsonValue
{
public:
	D_JsonArray() : D_JsonValue(), _array() {}												//  コンストラクタ
	virtual ~D_JsonArray() ;																// デストラクタ
	virtual bool				IsArray() { return true ; }									// D_JsonValueの種類が配列ならtrue。
	virtual D_JsonValue&		operator[]( int index ) ;									// 添字演算子[int]
	virtual D_JsonValue&		operator[]( const D_csmString& string ) ;					// 添字演算子[D_csmString]
	virtual D_JsonValue&		operator[]( const char* s ) ;								// 添字演算子[char*]
	virtual D_csmString&		GetString( const D_csmString& defaultValue = "", const D_csmString& indent = "" ) ;// 要素を文字列で返す(D_csmString型)
	void						Add( D_JsonValue* v ) { _array.PushBack( v, false ) ; }		// 配列要素を追加する
	virtual D_csmVector<D_JsonValue*>* GetVector( D_csmVector<D_JsonValue*>* /*defaultValue = NULL*/ ) { return &_array ; }// 要素をコンテナで返す(D_csmVector<D_JsonValue*>)
	virtual int					GetSize() { return static_cast< int >( _array.GetSize() ) ; }// 要素の数を返す

private:
	D_csmVector<D_JsonValue*>	_array ;						// JSON要素の値
} ;

// パースしたJSONの要素をマップとして持つ
class D_JsonMap : public D_JsonValue
{
public:
	D_JsonMap() : D_JsonValue(), _keys( NULL ) {}											//  コンストラクタ
	virtual ~D_JsonMap() ;																	//  デストラクタ
	virtual bool				IsMap() { return true ; }									//  D_JsonValueの値がD_JsonMap型ならtrue
	virtual D_JsonValue&		operator[]( const D_csmString& s ) ;						// 添字演算子[D_csmString]
	virtual D_JsonValue&		operator[]( const char* s ) ;								// 添字演算子[char*]
	virtual D_JsonValue&		operator[]( int index ) ;									// 添字演算子[int]
	virtual D_csmString&		GetString( const D_csmString& defaultValue = "", const D_csmString& indent = "" ) ;
	void						Put( D_csmString& key, D_JsonValue* v ) ;					// D_JsonMapに要素を追加する
	virtual int					GetSize() { return static_cast< int >( _keys->GetSize() ) ; }// D_JsonMapの要素数を取得する
	virtual D_csmMap<D_csmString, D_JsonValue*>* GetMap( D_csmMap<D_csmString, D_JsonValue*>* defaultValue = NULL ) ;//  要素をD_JsonMap型で返す
	virtual D_csmVector<D_csmString>& GetKeys() ;											//  D_JsonMapからキーのリストを取得する

private:
	D_csmMap<D_csmString, D_JsonValue*>	_map;		// JSON要素の値
	D_csmVector<D_csmString>*			_keys ;		// JSON要素の値
} ;

// motion3.jsonのコンテナ。
class D_CubismMotionJson
{
public:
	D_CubismMotionJson( const BYTE* buffer, size_t size ) ;									// コンストラクタ
	virtual ~D_CubismMotionJson() ;															// デストラクタ
	float						GetMotionDuration() const ;									// モーションの長さの取得
	bool						IsMotionLoop() const ;										// モーションのループ情報の取得
	bool						GetEvaluationOptionFlag( int flagType ) const ;				// モーションのベジェカーブの解釈方式のフラグ取得
	int							GetMotionCurveCount() const ;								// モーションカーブの個数の取得
	float						GetMotionFps() const ;										// モーションのフレームレートの取得
	int							GetMotionTotalSegmentCount() const ;						// モーションのセグメントの総合計の取得
	int							GetMotionTotalPointCount() const ;							// モーションのカーブの制御点の総合計の取得
	bool						IsExistMotionFadeInTime() const ;							// モーションのフェードイン時間の存在
	bool						IsExistMotionFadeOutTime() const ;							// モーションのフェードアウト時間の存在
	float						GetMotionFadeInTime() const ;								// モーションのフェードイン時間の取得
	float						GetMotionFadeOutTime() const ;								// モーションのフェードアウト時間の取得
	const char*					GetMotionCurveTarget( int curveIndex ) const ;				// モーションのカーブの種類の取得
	D_CubismIdHandle			GetMotionCurveId( int curveIndex ) ;						// モーションのカーブのIDの取得
	bool						IsExistMotionCurveFadeInTime( int curveIndex ) const ;		// モーションのカーブのフェードイン時間の存在
	bool						IsExistMotionCurveFadeOutTime( int curveIndex ) const ;		// モーションのカーブのフェードアウト時間の存在
	float						GetMotionCurveFadeInTime( int curveIndex ) const ;			// モーションのカーブのフェードイン時間の取得
	float						GetMotionCurveFadeOutTime( int curveIndex ) const ;			// モーションのカーブのフェードアウト時間の取得
	int							GetMotionCurveSegmentCount( int curveIndex ) const ;		// モーションのカーブのセグメントの個数の取得
	float						GetMotionCurveSegment( int curveIndex, int segmentIndex ) const ;// モーションのカーブのセグメントの値の取得
	int							GetEventCount() const ;										// イベントの個数の取得
	int							GetTotalEventValueSize() const ;							// イベントの総文字数の取得
	float						GetEventTime( int userDataIndex ) const ;					// イベントの時間の取得
	const char*					GetEventValue( int userDataIndex ) const ;					// イベントの取得

private:
	static const char*			Meta ;
	static const char*			Duration ;
	static const char*			Loop ;
	static const char*			AreBeziersRestricted ;
	static const char*			CurveCount ;
	static const char*			Fps ;
	static const char*			TotalSegmentCount ;
	static const char*			TotalPointCount ;
	static const char*			Curves ;
	static const char*			Target ;
	static const char*			Id ;
	static const char*			FadeInTime ;
	static const char*			FadeOutTime ;
	static const char*			Segments ;
	static const char*			UserData ;
	static const char*			UserDataCount ;
	static const char*			TotalUserDataSize ;
	static const char*			Time ;
	static const char*			Value ;
	D_CubismJson*				_json ;      // motion3.jsonデータ
} ;

// モーションカーブの制御点
struct D_CubismMotionPoint
{
	float						Time ;						// 時間[秒]
	float						Value ;						// 値
} ;

// モーションカーブのセグメント
struct D_CubismMotionSegment
{
    float						( *Evaluate )( const D_CubismMotionPoint *points, float time ) ;	// 使用する評価関数
	int							BasePointIndex ;			// 最初のセグメントへのインデックス
	int							SegmentType ;				// セグメントの種類
} ;

// モーションカーブ。
struct D_CubismMotionCurve
{
	D_CubismMotionCurveTarget	Type ;						// カーブの種類
	D_CubismIdHandle			Id ;						// カーブのID
	int							SegmentCount ;				// セグメントの個数
	int							BaseSegmentIndex ;			// 最初のセグメントのインデックス
	float						FadeInTime ;				// フェードインにかかる時間[秒]
	float						FadeOutTime ;				// フェードアウトにかかる時間[秒]
} ;

// イベント。
struct D_CubismMotionEvent
{
	float						FireTime ;
	D_csmString					Value ;
} ;

// モーションデータ。
struct D_CubismMotionData
{
	float						Duration ;					// モーションの長さ[秒]
	short						Loop ;						// ループするかどうか
	short						CurveCount ;				// カーブの個数
	int							EventCount ;				// UserDataの個数
	float						Fps ;						// フレームレート

	D_csmVector< D_CubismMotionCurve >		Curves ;		// カーブのリスト
	D_csmVector< D_CubismMotionSegment >	Segments ;		// セグメントのリスト
	D_csmVector< D_CubismMotionPoint >		Points ;		// ポイントのリスト
	D_csmVector< D_CubismMotionEvent >		Events ;		// イベントのリスト
} ;

class D_CubismModel ;

// Mocデータの管理
class D_CubismMoc
{
	friend class D_CubismModel ;
public:
	static D_CubismMoc*			Create( const BYTE* mocBytes, size_t size ) ;				// バッファからMocデータの作成
	static void					Delete( D_CubismMoc* moc ) ;								// Mocデータを削除
	D_CubismModel*				CreateModel() ;												// モデルを作成
	void						DeleteModel( D_CubismModel* model ) ;						// モデルを削除

private:
	D_CubismMoc( void* moc ) ;																// コンストラクタ
	virtual ~D_CubismMoc() ;																// デストラクタ

	void *						_moc ;						// Mocデータ
	int							_modelCount ;				// Mocデータから作られたモデルの個数
} ;

//  前方宣言
class D_CubismModel ;

// 4x4の行列
class D_CubismMatrix44
{
public:
	D_CubismMatrix44() ;																	// コンストラクタ
	virtual ~D_CubismMatrix44() ;															// デストラクタ
	static void					Multiply( float* a, float* b, float* dst ) ;				// 乗算
	void						LoadIdentity() ;											// 単位行列に初期化
	float*						GetArray() ;												// 行列を浮動小数点数の配列で取得
	void						SetMatrix( float* tr ) ;									// 行列を設定
	float						GetScaleX() const ;											// X軸の拡大率を取得
	float						GetScaleY() const ;											// Y軸の拡大率を取得
	float						GetTranslateX() const ;										// X軸の移動量を取得
	float						GetTranslateY() const ;										// Y軸の移動量を取得
	float						TransformX( float src ) ;									// X軸の値を現在の行列で計算
	float						TransformY( float src ) ;									// Y軸の値を現在の行列で計算
	float						InvertTransformX( float src ) ;								// X軸の値を現在の行列で逆計算
	float						InvertTransformY( float src ) ;								// Y軸の値を現在の行列で逆計算
	void						TranslateRelative( float x, float y ) ;						// 現在の行列の位置を起点にして移動
	void						Translate( float x, float y ) ;								// 現在の行列の位置を移動
	void						TranslateX( float x ) ;										// 現在の行列のX軸の位置を移動
	void						TranslateY( float y ) ;										// 現在の行列のY軸の位置を移動
	void						ScaleRelative( float x, float y ) ;							// 現在の行列の拡大率を相対的に設定
	void						Scale( float x, float y ) ;									// 現在の行列の拡大率を設定
	void						RotateRelative( float angle ) ;								// 現在の行列の回転( ラジアン )を相対的に設定
	void						MultiplyByMatrix( D_CubismMatrix44* m ) ;					// 現在の行列に行列を乗算

protected:
	float						_tr[ 16 ] ;					// 4x4行列データ
} ;

// モデル描画を処理するレンダラ
class D_CubismRenderer
{
public:
	// テクスチャの色をRGBAで扱うための構造体
	struct CubismTextureColor
	{
		CubismTextureColor() : R( 1.0f ), G( 1.0f ), B( 1.0f ), A( 1.0f ) {} ;				// コンストラクタ
		CubismTextureColor( float r, float g, float b, float a )							// コンストラクタ
		: R( r )
		, G( g )
		, B( b )
		, A( a ) {};

		virtual ~CubismTextureColor() {} ;													// デストラクタ

		float					R ;							// 赤チャンネル
		float					G ;							// 緑チャンネル
		float					B ;							// 青チャンネル
		float					A ;							// αチャンネル
	} ; // CubismTextureColor
	static D_CubismRenderer*	Create() ;													// レンダラのインスタンスを生成して取得する
	static void					Delete( D_CubismRenderer* renderer ) ;						// レンダラのインスタンスを解放する
	static void					StaticRelease() ;											// レンダラが保持する静的なリソースを解放する
	virtual void				Initialize( D_CubismModel* model, int ASyncThread ) ;		// レンダラの初期化処理を実行する
	void						DrawModel() ;												// モデルを描画する
	void						SetMvpMatrix( D_CubismMatrix44* matrix4x4 ) ;				// Model-View-Projection 行列をセットする
	D_CubismMatrix44			GetMvpMatrix() const ;										// Model-View-Projection 行列を取得する
	void						SetModelColor( float red, float green, float blue, float alpha ) ;	// モデルの色をセットする。
	CubismTextureColor			GetModelColor() const ;										// モデルの色を取得する。
	void						IsPremultipliedAlpha( bool enable ) ;						//  乗算済みαの有効・無効をセットする。
	bool						IsPremultipliedAlpha() const ;								//  乗算済みαの有効・無効を取得する。
	void						IsCulling( bool culling ) ;									//  カリング（片面描画）の有効・無効をセットする。
	bool						IsCulling() const ;											//  カリング（片面描画）の有効・無効を取得する。
	void						SetAnisotropy( float anisotropy ) ;							// テクスチャの異方性フィルタリングのパラメータをセットする
	float						GetAnisotropy() const ;										// テクスチャの異方性フィルタリングのパラメータをセットする
	D_CubismModel*				GetModel() const ;											// レンダリングするモデルを取得する。
	void						UseHighPrecisionMask( bool high ) ;							// マスク描画の方式を変更する。
	bool						IsUsingHighPrecisionMask() ;								// マスク描画の方式を取得する。

protected:
	D_CubismRenderer() ;																	// コンストラクタ
	virtual ~D_CubismRenderer() ;															// デストラクタ
	virtual void				DoDrawModel() = 0 ;											// モデル描画の実装
	virtual void				DrawMesh( int textureNo, int indexCount, int vertexCount, WORD* indexArray, float* vertexArray, float* uvArray, float opacity, D_CubismBlendMode colorBlendMode, bool invertedMask ) = 0 ;	// 描画オブジェクト（アートメッシュ）を描画する。
	virtual void				SaveProfile() = 0 ;											// モデル描画直前のレンダラのステートを保持する
	virtual void				RestoreProfile() = 0 ;										// モデル描画直前のレンダラのステートを復帰させる

private:
	// コピーコンストラクタを隠す
	D_CubismRenderer( const D_CubismRenderer& ) ;
	D_CubismRenderer&			operator=( const D_CubismRenderer& ) ;

	D_CubismMatrix44			_mvpMatrix4x4 ;				// Model-View-Projection 行列
	CubismTextureColor			_modelColor ;				// モデル自体のカラー(RGBA)
	bool						_isCulling ;				// カリングが有効ならtrue
	bool						_isPremultipliedAlpha ;		// 乗算済みαならtrue
	float						_anisotropy ;				// テクスチャの異方性フィルタリングのパラメータ
	D_CubismModel*				_model ;					// レンダリング対象のモデル
	bool						_useHighPrecisionMask ;		// falseの場合、マスクを纏めて描画する trueの場合、マスクはパーツ描画ごとに書き直す 
} ;

// モデル
class D_CubismModel
{
	friend class D_CubismMoc ;
public:

	// テクスチャの色をRGBAで扱うための構造体
	struct DrawableColorData
	{
		DrawableColorData() : IsOverwritten( false ) , Color() {};		// コンストラクタ
		DrawableColorData( bool isOverwritten, D_CubismRenderer::CubismTextureColor color ) : IsOverwritten( isOverwritten ), Color( color ) {};	// コンストラクタ
		virtual ~DrawableColorData() {};								// デストラクタ

		bool IsOverwritten ;
		D_CubismRenderer::CubismTextureColor Color ;
	} ;

	// テクスチャのカリング設定を管理するための構造体
	struct DrawableCullingData
	{
		DrawableCullingData() : IsOverwritten( false ), IsCulling( 0 ) {} ;	// コンストラクタ
		DrawableCullingData( bool isOverwritten, int isCulling ) : IsOverwritten( isOverwritten ), IsCulling( isCulling ) {} ;	// コンストラクタ
		virtual ~DrawableCullingData() {} ;								// デストラクタ

		bool IsOverwritten ;
		int IsCulling ;
	} ;

	// テクスチャの色をRGBAで扱うための構造体
	struct PartColorData
	{
		PartColorData()	: IsOverwritten( false ) , Color() {} ;			// コンストラクタ
		PartColorData( bool isOverwritten, D_CubismRenderer::CubismTextureColor color )	: IsOverwritten( isOverwritten ) , Color( color ) {} ;	// コンストラクタ
		virtual ~PartColorData() {} ;									// デストラクタ

		bool IsOverwritten ;
		D_CubismRenderer::CubismTextureColor Color ;
	} ;

	void						Update() const ;											// モデルのパラメータの更新
	float						GetCanvasWidthPixel() const ;								// キャンバスの幅を取得
	float						GetCanvasHeightPixel() const ;								// キャンバスの高さを取得
	float						GetPixelsPerUnit() const ;									// PixelsPerUnitを取得
	float						GetCanvasWidth() const ;									// キャンバスの幅の取得
	float						GetCanvasHeight() const ;									// キャンバスの高さの取得
	void						GetCanvasInfo( D_CubismVector2 *SizeInPixels, D_CubismVector2 *OriginInPixels, float *PixelsPerUnit ) ;		// キャンバスの情報を取得する
	int							GetPartIndex( D_CubismIdHandle partId ) ;					// パーツのインデックスの取得
	D_CubismIdHandle			GetPartId( int partIndex ) ;								// パーツのIDを取得
	int							GetPartCount() const ;										// パーツの個数の取得
	void						SetPartOpacity( D_CubismIdHandle partId, float opacity ) ;	// パーツの不透明度の設定
	void						SetPartOpacity( int partIndex, float opacity ) ;			// パーツの不透明度の設定
	float						GetPartOpacity( D_CubismIdHandle partId ) ;					// パーツの不透明度の取得
	float						GetPartOpacity( int partIndex ) ;							// パーツの不透明度の取得
	int							GetParameterIndex( D_CubismIdHandle parameterId ) ;			// パラメータのインデックスの取得
	int							GetParameterCount() const ;									// パラメータの個数の取得
	D_CubismIdHandle			GetParameterId( int parameterIndex ) ;						// パラメータのIDの取得
	D_csmParameterType			GetParameterType( DWORD parameterIndex ) const ;				// パラメータの種類の取得
	float						GetParameterMaximumValue( DWORD parameterIndex ) const ;	// パラメータの最大値の取得
	float						GetParameterMinimumValue( DWORD parameterIndex ) const ;	// パラメータの最小値の取得
	float						GetParameterDefaultValue( DWORD parameterIndex ) const ;	// パラメータのデフォルト値の取得
	float						GetParameterValue( D_CubismIdHandle parameterId ) ;			// パラメータの値の取得
	float						GetParameterValue( int parameterIndex ) ;					// パラメータの値の取得
	void						SetParameterValue( D_CubismIdHandle parameterId, float value, float weight = 1.0f, bool force = false ) ;	// パラメータの値の設定
	void						SetParameterValue( int parameterIndex, float value, float weight = 1.0f, bool force = false ) ;				// パラメータの値の設定
	void						SetDisableChangeParameter( D_CubismIdHandle parameterId, bool isDisable ) ;	// パラメータの値の変更を無効化するかどうかを設定する
	void						SetDisableChangeParameter( int parameterIndex, bool isDisable ) ;			// パラメータの値の変更を無効化するかどうかを設定する
	void						AddParameterValue( D_CubismIdHandle parameterId, float value, float weight = 1.0f ) ;	// パラメータの値の加算
	void						AddParameterValue( int parameterIndex, float value, float weight = 1.0f ) ;				// パラメータの値の加算
	void						MultiplyParameterValue( D_CubismIdHandle parameterId, float value, float weight = 1.0f ) ;	// パラメータの値の乗算
	void						MultiplyParameterValue( int parameterIndex, float value, float weight = 1.0f ) ;		// パラメータの値の乗算
	int							GetDrawableIndex( D_CubismIdHandle drawableId ) const ;		// Drawableのインデックスの取得
	int							GetDrawableCount() const ;									// Drawableの個数の取得
	D_CubismIdHandle			GetDrawableId( int drawableIndex ) const ;					// DrawableのIDの取得
	const int *					GetDrawableRenderOrders() const ;							// Drawableの描画順リストの取得
	int							GetDrawableTextureIndex( int drawableIndex ) const ;		// Drawableのテクスチャインデックスの取得
	int							GetDrawableVertexIndexCount( int drawableIndex ) const ;	// Drawableの頂点インデックスの個数の取得
	int							GetDrawableVertexCount( int drawableIndex ) const ;			// Drawableの頂点の個数の取得
	const float *				GetDrawableVertices( int drawableIndex ) const ;			// Drawableの頂点リストの取得
	const WORD *				GetDrawableVertexIndices( int drawableIndex ) const ;		// Drawableの頂点インデックスリストの取得
	const D_CubismVector2 *		GetDrawableVertexPositions( int drawableIndex ) const ;		// Drawableの頂点リストの取得
	const D_CubismVector2 *		GetDrawableVertexUvs( int drawableIndex ) const ;			// Drawableの頂点のUVリストの取得
	float						GetDrawableOpacity( int drawableIndex ) const ;				// Drawableの不透明度の取得
	D_CubismVector4				GetDrawableMultiplyColor( int drawableIndex ) const ;		// Drawableの乗算色の取得
	D_CubismVector4				GetDrawableScreenColor( int drawableIndex ) const ;			// Drawableのスクリーン色の取得
	int							GetDrawableParentPartIndex( DWORD drawableIndex ) const ;	// Drawableの親パーツのインデックスの取得
	D_CubismBlendMode			GetDrawableBlendMode( int drawableIndex ) const ;			// Drawableのブレンドモードの取得
	bool						GetDrawableInvertedMask( int drawableIndex ) const ;		// Drawableのマスクの反転使用の取得
	bool						GetDrawableDynamicFlagIsVisible( int drawableIndex ) const ;	// Drawableの表示情報の取得
	bool						GetDrawableDynamicFlagVisibilityDidChange( int drawableIndex ) const ;	// Drawableの表示状態の変化の取得
	bool						GetDrawableDynamicFlagOpacityDidChange( int drawableIndex ) const ;	// Drawableの不透明度の変化情報の取得
	bool						GetDrawableDynamicFlagDrawOrderDidChange( int drawableIndex ) const ;	// DrawableのDrawOrderの変化情報の取得
	bool						GetDrawableDynamicFlagRenderOrderDidChange( int drawableIndex ) const ;	// Drawableの描画順序の変化情報の取得
	bool						GetDrawableDynamicFlagVertexPositionsDidChange( int drawableIndex ) const ;	// DrawableのVertexPositionsの変化情報の取得
	bool						GetDrawableDynamicFlagBlendColorDidChange( int drawableIndex ) const ;	// Drawableの乗算色・スクリーン色の変化情報の取得
	const int**					GetDrawableMasks() const ;									// Drawableのクリッピングマスクリストの取得
	const int*					GetDrawableMaskCounts() const ;								// Drawableのクリッピングマスクの個数リストの取得
	bool						IsUsingMasking() const ;									// クリッピングマスクの使用状態
	void						LoadParameters() ;											// 保存されたパラメータの読み込み
	void						SaveParameters() ;											// パラメータの保存
	D_CubismRenderer::CubismTextureColor GetMultiplyColor( int drawableIndex ) const ;		// drawableの乗算色を取得する
	D_CubismRenderer::CubismTextureColor GetScreenColor( int drawableIndex ) const ;		// drawableのスクリーン色を取得する
	void						SetMultiplyColor( int drawableIndex, const D_CubismRenderer::CubismTextureColor& color ) ;	// drawableの乗算色を設定する
	void						SetMultiplyColor( int drawableIndex, float r, float g, float b, float a = 1.0f ) ;			// drawableの乗算色を設定する
	void						SetScreenColor( int drawableIndex, const D_CubismRenderer::CubismTextureColor& color ) ;	// drawableのスクリーン色を設定する
	void						SetScreenColor( int drawableIndex, float r, float g, float b, float a = 1.0f ) ;			// drawableのスクリーン色を設定する
	D_CubismRenderer::CubismTextureColor GetPartMultiplyColor( int partIndex ) const ;		// partの乗算色を取得する
	D_CubismRenderer::CubismTextureColor GetPartScreenColor( int partIndex ) const ;		// partの乗算色を取得する
	void						SetPartMultiplyColor( int partIndex, const D_CubismRenderer::CubismTextureColor& color ) ;	// partのスクリーン色を設定する
	void						SetPartMultiplyColor( int partIndex, float r, float g, float b, float a = 1.0f ) ;			// partの乗算色を設定する
	void						SetPartScreenColor( int partIndex, const D_CubismRenderer::CubismTextureColor& color ) ;	// partのスクリーン色を設定する
	void						SetPartScreenColor( int partIndex, float r, float g, float b, float a = 1.0f ) ;			// partのスクリーン色を設定する
	bool						GetOverwriteFlagForModelMultiplyColors() const ;								// SDKからモデル全体の乗算色を上書きするか。
	bool						GetOverwriteFlagForModelScreenColors() const ;									// SDKからモデル全体のスクリーン色を上書きするか。
	void						SetOverwriteFlagForModelMultiplyColors( bool value ) ;							// SDKからモデル全体の乗算色を上書きするかをセットする
	void						SetOverwriteFlagForModelScreenColors( bool value ) ;							// SDKからモデル全体のスクリーン色を上書きするかをセットする  SDK上の色情報を使うならtrue、モデルの色情報を使うならfalse
	bool						GetOverwriteFlagForDrawableMultiplyColors( int drawableIndex ) const ;			// SDKからdrawableの乗算色を上書きするか。
	bool						GetOverwriteFlagForDrawableScreenColors( int drawableIndex ) const ;			// SDKからdrawableのスクリーン色を上書きするか。
	void						SetOverwriteFlagForDrawableMultiplyColors( DWORD drawableIndex, bool value ) ;	// SDKからdrawableの乗算色を上書きするかをセットする
	void						SetOverwriteFlagForDrawableScreenColors( DWORD drawableIndex, bool value ) ;	// SDKからdrawableのスクリーン色を上書きするかをセットする
	bool						GetOverwriteColorForPartMultiplyColors( int partIndex ) const ;					// SDKからpartの乗算色を上書きするか。
	bool						GetOverwriteColorForPartScreenColors( int partIndex ) const ;					// SDKからpartのスクリーン色を上書きするか。
	void						SetOverwriteColorForPartMultiplyColors( DWORD partIndex, bool value ) ;			// SDKからpartの乗算色を上書きするかをセットする     SDK上の色情報を使うならtrue、モデルの色情報を使うならfalse
	void						SetOverwriteColorForPartScreenColors( DWORD partIndex, bool value ) ;			// SDKからpartのスクリーン色を上書きするかをセットする      SDK上の色情報を使うならtrue、モデルの色情報を使うならfalse
	int							GetDrawableCulling( int drawableIndex ) const ;									// Drawableのカリング情報の取得
	void						SetDrawableCulling( int drawableIndex, int isCulling ) ;						// Drawableのカリング情報を設定する
	bool						GetOverwriteFlagForModelCullings() const ;										// SDKからモデル全体のカリング設定を上書きするか。
	void						SetOverwriteFlagForModelCullings( bool value ) ;								// SDKからモデル全体のカリング設定を上書きするかをセットする  SDK上のカリング設定を使うならtrue、モデルのカリング設定を使うならfalse
	bool						GetOverwriteFlagForDrawableCullings( int drawableIndex ) const ;				// SDKからdrawableのカリング設定を上書きするか。
	void						SetOverwriteFlagForDrawableCullings( DWORD drawableIndex, bool value ) ;		// SDKからdrawableのカリング設定を上書きするかをセットする  SDK上のカリング設定を使うならtrue、モデルのカリング設定を使うならfalse
	float						GetModelOpacity() ;																// モデルの不透明度を取得する
	void						SetModelOpacity( float value ) ;												// モデルの不透明度を設定する
	void*						GetModel() const ;

private:
	D_CubismModel( void* model ) ;															// コンストラクタ
	virtual ~D_CubismModel() ;																// デストラクタ
	D_CubismModel( const D_CubismModel& ) ;

	D_CubismModel&				operator=( const D_CubismModel& ) ;
	void						Initialize() ;												// 初期化

	// partのOverwriteColor Set関数
	void SetPartColor(
		DWORD partIndex,
		float r, float g, float b, float a,
		D_csmVector< PartColorData >& partColors,
		D_csmVector< DrawableColorData >& drawableColors ) ;

	// partのOverwriteFlag Set関数
	void SetOverwriteColorForPartColors(
		DWORD partIndex,
		bool value,
		D_csmVector< PartColorData >& partColors,
		D_csmVector< DrawableColorData >& drawableColors ) ;

	D_csmMap< int, float >				_notExistPartOpacities ;	// 存在していないパーツの不透明度のリスト
	D_csmMap< D_CubismIdHandle, int >	_notExistPartId ;			// 存在していないパーツIDのリスト
	D_csmMap< int, float >				_notExistParameterValues ;	// 存在していないパラメータの値のリスト
	D_csmMap< D_CubismIdHandle, int >	_notExistParameterId ;		// 存在していないパラメータIDのリスト
	D_csmVector< int >					_disableChangeParameterIndices ;	// 通常の変更は拒否されるパラメータ番号
	D_csmVector< float >				_savedParameters ;			// 保存されたパラメータ
	void *								_model ;					// モデル
	float *								_parameterValues ;			// パラメータの値のリスト
	const float *						_parameterMaximumValues ;	// パラメータの最大値のリスト
	const float *						_parameterMinimumValues ;	// パラメータの最小値のリスト
	float *								_partOpacities ;			// パーツの不透明度のリスト
    float								_modelOpacity ;				// モデルの不透明度
	D_csmVector< D_CubismIdHandle >		_parameterIds ;
	D_csmVector< D_CubismIdHandle >		_partIds ;
	D_csmVector< D_CubismIdHandle >		_drawableIds ;
	D_csmVector< DrawableColorData >	_userScreenColors;			// Drawable 乗算色の配列
	D_csmVector< DrawableColorData >	_userMultiplyColors;		// Drawable スクリーン色の配列
	D_csmVector< DrawableCullingData >	_userCullings;				// カリング設定の配列
	D_csmVector< PartColorData >		_userPartScreenColors;		// Part 乗算色の配列
	D_csmVector< PartColorData >		_userPartMultiplyColors;	// Part スクリーン色の配列
	D_csmVector< D_csmVector< DWORD > >	_partChildDrawables;		// Partの子DrawableIndexの配列
	bool								_isOverwrittenModelMultiplyColors; // 乗算色を全て上書きするか？
	bool								_isOverwrittenModelScreenColors; // スクリーン色を全て上書きするか？
	bool								_isOverwrittenCullings;		// モデルのカリング設定をすべて上書きするか？
} ;

// D_CubismMotionQueueManagerで再生している各モーションの管理
class D_CubismMotionQueueEntry
{
	friend class D_CubismMotionQueueManager ;
	friend class D_ACubismMotion ;
	friend class D_CubismMotion ;

public:
	D_CubismMotionQueueEntry() ;															// コンストラクタ
	virtual ~D_CubismMotionQueueEntry() ;// デストラクタ
	void						StartFadeout( float fadeOutSeconds, float userTimeSeconds ) ;	// フェードアウトの開始
	bool						IsFinished() const ;										// モーションの終了の確認
	bool						IsStarted() const ;											// モーションの開始の確認
	float						GetStartTime() const ;										// モーションの開始時刻の取得
	float						GetFadeInStartTime() const ;								// フェードインの開始時刻の取得
	float						GetEndTime() const ;										// フェードインの終了時刻の取得
	void						SetStartTime( float startTime ) ;							// モーションの開始時刻の設定
	void						SetFadeInStartTime( float startTime ) ;						// フェードインの開始時刻の設定
	void						SetEndTime( float endTime )	;								// フェードインの終了時刻の設定
	void						IsFinished( bool f ) ;										// モーションの終了の設定
	void						IsStarted( bool f ) ;										// モーションの開始の設定
	bool						IsAvailable() const ;										// モーションの有効性の確認
	void						IsAvailable( bool v ) ;										// モーションの有効性の設定
	void						SetState( float timeSeconds, float weight ) ;				// モーションの状態の設定
	float						GetStateTime() const ;										// モーションの現在時刻の取得
	float						GetStateWeight() const ;									// モーションの重みの取得
	float						GetLastCheckEventTime() const ;								// 最後にイベントの発火をチェックした時間を取得
	void						SetLastCheckEventTime( float checkTime ) ;					// 最後にイベントをチェックした時間を設定

private:
	bool						_autoDelete ;					// 自動削除
	class D_ACubismMotion*		_motion ;						// モーション
	bool						_available ;					// 有効化フラグ
	bool						_finished ;						// 終了フラグ
	bool						_started ;						// 開始フラグ（0.9.00以降）
	float						_startTimeSeconds ;				//  モーション再生開始時刻[秒]
	float						_fadeInStartTimeSeconds ;		//  フェードイン開始時刻（ループの時は初回のみ）[秒]
	float						_endTimeSeconds ;				// 終了予定時刻[秒]
	float						_stateTimeSeconds ;				//  時刻の状態[秒]
	float						_stateWeight ;					//  重みの状態
	float						_lastEventCheckSeconds ;		//   最終のMotion側のチェックした時間
	D_CubismMotionQueueEntryHandle  _motionQueueEntryHandle	;	// インスタンスごとに一意の値を持つ識別番号
} ;

// モーション再生の管理
class D_CubismMotionQueueManager
{
public:
	D_CubismMotionQueueManager() ;															// コンストラクタ
	virtual ~D_CubismMotionQueueManager() ;													// デストラクタ
	D_CubismMotionQueueEntryHandle StartMotion( D_ACubismMotion* motion, bool autoDelete, float userTimeSeconds ) ;		// 指定したモーションの開始
	bool						IsFinished() ;																			// すべてのモーションの終了の確認
	bool						IsFinished( D_CubismMotionQueueEntryHandle motionQueueEntryNumber ) ;					// 指定したモーションの終了の確認
	void						StopAllMotions() ;																		// すべてのモーションの停止
	D_CubismMotionQueueEntry *	GetCubismMotionQueueEntry( D_CubismMotionQueueEntryHandle motionQueueEntryNumber ) ;	// 指定したD_CubismMotionQueueEntryの取得
	void						SetEventCallback( D_CubismMotionEventFunction callback, void* customData = NULL ) ;		// イベントを受け取るCallbackの登録
	float						GetMotionPlayTime(){ return _motionPlayTime; }

protected:
	virtual bool				DoUpdateMotion( D_CubismModel* model, float userTimeSeconds ) ;							// モーションの更新

	float										_userTimeSeconds ;	// デルタ時間の積算値[秒]
	float										_motionPlayTime ;	// モーション再生時間

private:
	D_csmVector< D_CubismMotionQueueEntry * >   _motions ;			// モーション
	D_CubismMotionEventFunction					_eventCallback ;	// コールバック関数ポインタ
	void*										_eventCustomData ;	// コールバックに戻されるデータ
} ;

// モーションの抽象基底クラス
class D_ACubismMotion
{
public:
	static void					Delete( D_ACubismMotion* motion ) ;							// インスタンスの破棄
	D_ACubismMotion() ;																		// コンストラクタ
	void						UpdateParameters( D_CubismModel* model, D_CubismMotionQueueEntry* motionQueueEntry, float userTimeSeconds ) ;	// モデルのパラメータ更新
	void						SetFadeInTime( float fadeInSeconds ) ;						// フェードイン
	void						SetFadeOutTime( float fadeOutSeconds ) ;					// フェードアウト
	float						GetFadeOutTime() const ;									// フェードアウトにかかる時間の取得
	float						GetFadeInTime() const ;										// フェードインにかかる時間の取得
	void						SetWeight( float weight ) ;									// モーション適用の重みの設定
	float						GetWeight() const ;											// モーション適用の重みの取得
	virtual float				GetDuration() ;												// モーションの長さの取得
	virtual float				GetLoopDuration() ;											// モーションのループ1回分の長さの取得
	void						SetOffsetTime( float offsetSeconds ) ;						// モーション再生の開始時刻の設定
	virtual const D_csmVector< const D_csmString * >& GetFiredEvent( float beforeCheckTimeSeconds, float motionTimeSeconds ) ;	// モデルのパラメータ更新

private:
	// Prevention of copy Constructor
	D_ACubismMotion( const D_ACubismMotion& ) ;
	D_ACubismMotion& operator=( const D_ACubismMotion& ) ;

protected:
	virtual ~D_ACubismMotion() ;															// デストラクタ
	virtual void				DoUpdateParameters( D_CubismModel* model, float userTimeSeconds, float weight, D_CubismMotionQueueEntry* motionQueueEntry ) = 0 ;// モデルのパラメータの更新の実行

	float						_fadeInSeconds ;			// フェードインにかかる時間[秒]
	float						_fadeOutSeconds ;			// フェードアウトにかかる時間[秒]
	float						_weight ;					// モーションの重み
	float						_offsetSeconds ;			// モーション再生の開始時刻[秒]
	D_csmVector< const D_csmString * >	_firedEventValues ;
} ;

// 表情のモーション
class D_CubismExpressionMotion : public D_ACubismMotion
{
private:
	// 表情パラメータ値の計算方式
	enum ExpressionBlendType
	{
		ExpressionBlendType_Add					= 0,		// 加算
		ExpressionBlendType_Multiply			= 1,		// 乗算
		ExpressionBlendType_Overwrite			= 2			// 上書き
	} ;

public:
	// 表情のパラメータ情報
	struct D_ExpressionParameter
	{
		D_CubismIdHandle		ParameterId ;	// パラメータID
		ExpressionBlendType		BlendType ;		// パラメータの演算種類
		float					Value ;			// 値
	} ;
	static D_CubismExpressionMotion*	Create( const BYTE* buf, size_t size ) ;				// インスタンスの作成
	virtual void						DoUpdateParameters( D_CubismModel* model, float userTimeSeconds, float weight, D_CubismMotionQueueEntry* motionQueueEntry ) ;	// モデルのパラメータの更新の実行

private:
	D_CubismExpressionMotion() ;
	virtual ~D_CubismExpressionMotion() ;

	D_csmVector<D_ExpressionParameter> _parameters ;		// 表情のパラメータ情報リスト
} ;

// モーションクラス
class D_CubismMotion : public D_ACubismMotion
{
public:
	static D_CubismMotion*		Create( const BYTE* buffer, size_t size ) ;									// インスタンスの生成
	virtual void				DoUpdateParameters( D_CubismModel* model, float userTimeSeconds, float fadeWeight, D_CubismMotionQueueEntry* motionQueueEntry ) ;	// モデルのパラメータの更新の実行
	void						IsLoop( bool loop ) ;														// ループ情報の設定
	bool						IsLoop() const ;															// ループ情報の取得
	void						IsLoopFadeIn( bool loopFadeIn ) ;											// ループ時のフェードイン情報の設定
	bool						IsLoopFadeIn() const ;														// ループ時のフェードイン情報の取得
	virtual float				GetDuration() ;																// モーションの長さの取得
	virtual float				GetLoopDuration() ;															// モーションのループ時の長さの取得
	void						SetParameterFadeInTime( D_CubismIdHandle parameterId, float value ) ;		// パラメータに対するフェードインの時間の設定
	void						SetParameterFadeOutTime( D_CubismIdHandle parameterId, float value ) ;		// パラメータに対するフェードアウトの時間の設定
	float						GetParameterFadeInTime( D_CubismIdHandle parameterId ) const ;				// パラメータに対するフェードインの時間の取得
	float						GetParameterFadeOutTime( D_CubismIdHandle parameterId ) const ;				// パラメータに対するフェードアウトの時間の取得
	void						SetEffectIds( const D_csmVector<D_CubismIdHandle>& eyeBlinkParameterIds, const D_csmVector<D_CubismIdHandle>& lipSyncParameterIds ) ;	// 自動エフェクトがかかっているパラメータIDリストの設定
	virtual const D_csmVector< const D_csmString * >& GetFiredEvent( float beforeCheckTimeSeconds, float motionTimeSeconds ) ;	// モデルのパラメータ更新

private:
	D_CubismMotion() ;																		// コンストラクタ
	virtual ~D_CubismMotion() ;																// デストラクタ
	D_CubismMotion( const D_CubismMotion& ) ;												// Prevention of copy Constructor
	D_CubismMotion& operator=( const D_CubismMotion& ) ;
	void Parse( const BYTE* motionJson, const size_t size ) ;								// motion3.jsonのパース

	float						_sourceFrameRate ;			// ロードしたファイルのFPS。記述が無ければデフォルト値15fpsとなる
	float						_loopDurationSeconds ;		// mtnファイルで定義される一連のモーションの長さ
	bool						_isLoop ;					// ループするか?
	bool						_isLoopFadeIn ;				// ループ時にフェードインが有効かどうかのフラグ。初期値では有効。
	float						_lastWeight ;				// 最後に設定された重み
	D_CubismMotionData*			_motionData ;				// 実際のモーションデータ本体
	D_csmVector< D_CubismIdHandle >	_eyeBlinkParameterIds ;	// 自動まばたきを適用するパラメータIDハンドルのリスト。  モデル（モデルセッティング）とパラメータを対応付ける。
	D_csmVector< D_CubismIdHandle >	_lipSyncParameterIds ;	// リップシンクを適用するパラメータIDハンドルのリスト。  モデル（モデルセッティング）とパラメータを対応付ける。
	D_CubismIdHandle			_modelCurveIdEyeBlink ;		// モデルが持つ自動まばたき用パラメータIDのハンドル。  モデルとモーションを対応付ける。
	D_CubismIdHandle			_modelCurveIdLipSync ;		// モデルが持つリップシンク用パラメータIDのハンドル。  モデルとモーションを対応付ける。
} ;

// モデル座標設定用の4x4行列
class D_CubismModelMatrix : public D_CubismMatrix44
{
public:
	D_CubismModelMatrix() ;																	// コンストラクタ
	D_CubismModelMatrix( float w, float h ) ;												// コンストラクタ
	virtual ~D_CubismModelMatrix() ;														// デストラクタ
	void						SetWidth( float w ) ;										// 横幅を設定
	void						SetHeight( float h ) ;										// 縦幅を設定
	void						SetPosition( float x, float y ) ;							// 位置を設定
	void						SetCenterPosition( float x, float y ) ;						// 中心位置を設定
	void						Top( float y ) ;											// 上辺の位置を設定
	void						Bottom( float y ) ;											// 下辺の位置を設定
	void						Left( float x ) ;											// 左辺の位置を設定
	void						Right( float x ) ;											// 右辺の位置を設定
	void						CenterX( float x ) ;										// X軸の中心位置を設定
	void						SetX( float x ) ;											// X軸の位置を設定
	void						CenterY( float y ) ;										// Y軸の中心位置を設定
	void						SetY( float y ) ;											// Y軸の位置を設定
	void						SetupFromLayout( D_csmMap<D_csmString, float>& layout ) ;	// レイアウト情報から位置を設定

private:
	float						_width ;					// 横幅
	float						_height ;					// 縦幅
} ;

// モーションの管理
class D_CubismMotionManager : public D_CubismMotionQueueManager
{
public:
	D_CubismMotionManager() ;																// コンストラクタ
	virtual ~D_CubismMotionManager() ;														// デストラクタ
	int							GetCurrentPriority() const ;								// 再生中のモーションの優先度の取得
	int							GetReservePriority() const ;								// 予約中のモーションの優先度の取得
	void						SetReservePriority( int val ) ;								// 予約中のモーションの優先度の設定
	D_CubismMotionQueueEntryHandle StartMotionPriority( D_ACubismMotion* motion, bool autoDelete, int priority ) ;// 優先度を設定してモーションの開始
	bool						UpdateMotion( D_CubismModel* model, float deltaTimeSeconds ) ;	// モーションの更新
	bool						ReserveMotion( int priority ) ;								// モーションの予約

private:
	int							_currentPriority ;                 // 現在再生中のモーションの優先度
	int							_reservePriority ;                 // 再生予定のモーションの優先度。再生中は0になる。モーションファイルを別スレッドで読み込むときの処理
} ;

// モデル設定情報を取り扱う関数を宣言した純粋仮想クラス。
class D_ICubismModelSetting
{
public:
	virtual ~D_ICubismModelSetting() {}																// デストラクタ
	virtual const char*					GetModelFileName() = 0 ;											// Mocファイルの名前を取得する
	virtual const char*					GetModelFileNameA() = 0 ;											// Mocファイルの名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetModelFileNameW() = 0 ;											// Mocファイルの名前を取得する
	virtual int							GetTextureCount() = 0 ;												// モデルが使用するテクスチャの数を取得する
	virtual const char*					GetTextureDirectory() = 0 ;											// テクスチャが配置されたディレクトリの名前を取得する
	virtual const char*					GetTextureDirectoryA() = 0 ;										// テクスチャが配置されたディレクトリの名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetTextureDirectoryW() = 0 ;										// テクスチャが配置されたディレクトリの名前を取得する
	virtual const char*					GetTextureFileName( int index ) = 0 ;								// モデルが使用するテクスチャの名前を取得する
	virtual const char*					GetTextureFileNameA( int index ) = 0 ;								// モデルが使用するテクスチャの名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetTextureFileNameW( int index ) = 0 ;								// モデルが使用するテクスチャの名前を取得する
	virtual int							GetHitAreasCount() = 0 ;											// モデルに設定された当たり判定の数を取得する
	virtual D_CubismIdHandle			GetHitAreaId( int index ) = 0 ;										// 当たり判定に設定されたIDを取得する
	virtual const char*					GetHitAreaName( int index ) = 0 ;									// 当たり判定に設定された名前を取得する
	virtual const char*					GetHitAreaNameA( int index ) = 0 ;									// 当たり判定に設定された名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetHitAreaNameW( int index ) = 0 ;									// 当たり判定に設定された名前を取得する
	virtual const char*					GetPhysicsFileName() = 0 ;											// 物理演算設定ファイルの名前を取得する
	virtual const char*					GetPhysicsFileNameA() = 0 ;											// 物理演算設定ファイルの名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetPhysicsFileNameW() = 0 ;											// 物理演算設定ファイルの名前を取得する
	virtual const char*					GetPoseFileName() = 0 ;												// パーツ切り替え設定ファイルの名前を取得する
	virtual const char*					GetPoseFileNameA() = 0 ;											// パーツ切り替え設定ファイルの名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetPoseFileNameW() = 0 ;											// パーツ切り替え設定ファイルの名前を取得する
	virtual int							GetExpressionCount() = 0 ;											// 表情設定ファイルの数を取得する
	virtual const char*					GetExpressionName( int index ) = 0 ;								// 表情設定ファイルを識別する名前（別名）を取得する
	virtual const char*					GetExpressionNameA( int index ) = 0 ;								// 表情設定ファイルを識別する名前（別名）を取得する
	virtual const BYTE/*wchar_t*/ *		GetExpressionNameW( int index ) = 0 ;								// 表情設定ファイルを識別する名前（別名）を取得する
	virtual const char*					GetExpressionFileName( int index ) = 0 ;							// 表情設定ファイルの名前を取得する
	virtual const char*					GetExpressionFileNameA( int index ) = 0 ;							// 表情設定ファイルの名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetExpressionFileNameW( int index ) = 0 ;							// 表情設定ファイルの名前を取得する
	virtual int							GetMotionGroupCount() = 0 ;											// モーショングループの数を取得する
	virtual const char*					GetMotionGroupName( int index ) = 0 ;								// モーショングループの名前を取得する
	virtual const char*					GetMotionGroupNameA( int index ) = 0 ;								// モーショングループの名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetMotionGroupNameW( int index ) = 0 ;								// モーショングループの名前を取得する
	virtual int							GetMotionCount( const char* groupName ) = 0 ;						// モーショングループに含まれるモーションの数を取得する
	virtual const char*					GetMotionFileName( const char* groupName, int index ) = 0 ;			// グループ名とインデックス値からモーションファイルの名前を取得する
	virtual const char*					GetMotionFileNameA( const char* groupName, int index ) = 0 ;		// グループ名とインデックス値からモーションファイルの名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetMotionFileNameW( const char* groupName, int index ) = 0 ;		// グループ名とインデックス値からモーションファイルの名前を取得する
	virtual const char*					GetMotionSoundFileName( const char* groupName, int index ) = 0 ;	// モーションに対応するサウンドファイルの名前を取得する
	virtual const char*					GetMotionSoundFileNameA( const char* groupName, int index ) = 0 ;	// モーションに対応するサウンドファイルの名前を取得する
	virtual const BYTE/*wchar_t*/ *		GetMotionSoundFileNameW( const char* groupName, int index ) = 0 ;	// モーションに対応するサウンドファイルの名前を取得する
	virtual float						GetMotionFadeInTimeValue( const char* groupName, int index ) = 0 ;	// モーション開始時のフェードイン処理時間を取得する
	virtual float						GetMotionFadeOutTimeValue( const char* groupName, int index ) = 0 ;	// モーション終了時のフェードアウト処理時間を取得する
	virtual const char*					GetUserDataFile() = 0 ;												// ユーザデータのファイル名を取得する
	virtual const char*					GetUserDataFileA() = 0 ;											// ユーザデータのファイル名を取得する
	virtual const BYTE/*wchar_t*/ *		GetUserDataFileW() = 0 ;											// ユーザデータのファイル名を取得する
	virtual bool						GetLayoutMap( D_csmMap<D_csmString, float>& outLayoutMap ) = 0 ;	// レイアウト情報を取得する
	virtual int							GetEyeBlinkParameterCount() = 0 ;									// 目パチに関連付けられたパラメータの数を取得する
	virtual D_CubismIdHandle			GetEyeBlinkParameterId( int index ) = 0 ;							// 目パチに関連付けられたパラメータのIDを取得する
	virtual int							GetLipSyncParameterCount() = 0 ;									// リップシンクに関連付けられたパラメータの数を取得する
	virtual D_CubismIdHandle			GetLipSyncParameterId( int index ) = 0 ;							// リップシンクに関連付けられたパラメータのIDを取得する
} ;

// Model3Jsonパーサー.
class D_CubismModelSettingJson : public D_ICubismModelSetting
{
public:
	D_CubismModelSettingJson( const BYTE* buffer, size_t size ) ;								// 引数付きコンストラクタ
	virtual ~D_CubismModelSettingJson() ;													// デストラクタ
	D_CubismJson*				GetJsonPointer() const ;									// CubismJsonオブジェクトのポインタを取得する
	const char*					GetModelFileName() ;
	const char*					GetModelFileNameA() ;
	const BYTE/*wchar_t*/ *		GetModelFileNameW() ;
	int							GetTextureCount() ;
	const char*					GetTextureDirectory() ;
	const char*					GetTextureDirectoryA() ;
	const BYTE/*wchar_t*/ *		GetTextureDirectoryW() ;
	const char*					GetTextureFileName( int index ) ;
	const char*					GetTextureFileNameA( int index ) ;
	const BYTE/*wchar_t*/ *		GetTextureFileNameW( int index ) ;
	int							GetHitAreasCount() ;
	D_CubismIdHandle			GetHitAreaId( int index ) ;
	const char*					GetHitAreaName( int index ) ;
	const char*					GetHitAreaNameA( int index ) ;
	const BYTE/*wchar_t*/ *		GetHitAreaNameW( int index ) ;
	const char*					GetPhysicsFileName() ;
	const char*					GetPhysicsFileNameA() ;
	const BYTE/*wchar_t*/ *		GetPhysicsFileNameW() ;
	const char*					GetPoseFileName() ;
	const char*					GetPoseFileNameA() ;
	const BYTE/*wchar_t*/ *		GetPoseFileNameW() ;
	int							GetExpressionCount() ;
	const char*					GetExpressionName( int index ) ;
	const char*					GetExpressionNameA( int index ) ;
	const BYTE/*wchar_t*/ *		GetExpressionNameW( int index ) ;
	const char*					GetExpressionFileName( int index ) ;
	const char*					GetExpressionFileNameA( int index ) ;
	const BYTE/*wchar_t*/ *		GetExpressionFileNameW( int index ) ;
	int							GetMotionGroupCount() ;
	const char*					GetMotionGroupName( int index ) ;
	const char*					GetMotionGroupNameA( int index ) ;
	const BYTE/*wchar_t*/ *		GetMotionGroupNameW( int index ) ;
	int							GetMotionCount( const char* groupName ) ;
	const char*					GetMotionFileName( const char* groupName, int index ) ;
	const char*					GetMotionFileNameA( const char* groupName, int index ) ;
	const BYTE/*wchar_t*/ *		GetMotionFileNameW( const char* groupName, int index ) ;
	const char*					GetMotionSoundFileName( const char* groupName, int index ) ;
	const char*					GetMotionSoundFileNameA( const char* groupName, int index ) ;
	const BYTE/*wchar_t*/ *		GetMotionSoundFileNameW( const char* groupName, int index ) ;
	float						GetMotionFadeInTimeValue( const char* groupName, int index ) ;
	float						GetMotionFadeOutTimeValue( const char* groupName, int index ) ;
	const char*					GetUserDataFile() ;
	const char*					GetUserDataFileA() ;
	const BYTE/*wchar_t*/ *		GetUserDataFileW() ;
	bool						GetLayoutMap( D_csmMap<D_csmString, float>& outLayoutMap ) ;
	int							GetEyeBlinkParameterCount() ;
	D_CubismIdHandle			GetEyeBlinkParameterId( int index ) ;
	int							GetLipSyncParameterCount() ;
	D_CubismIdHandle			GetLipSyncParameterId( int index ) ;

private:
	enum FrequentNode
	{
		FrequentNode_Groups,		// GetRoot()[Groups] 
		FrequentNode_Moc,			// GetRoot()[FileReferences][Moc] 
		FrequentNode_Motions,		// GetRoot()[FileReferences][Motions] 
		FrequentNode_Expressions,	// GetRoot()[FileReferences][Expressions] 
		FrequentNode_Textures,		// GetRoot()[FileReferences][Textures] 
		FrequentNode_Physics,		// GetRoot()[FileReferences][Physics] 
		FrequentNode_Pose,			// GetRoot()[FileReferences][Pose] 
		FrequentNode_HitAreas,		// GetRoot()[HitAreas] 
	} ;

	bool						IsExistModelFile() const ;											// モデルファイルのキーが存在するかどうかを確認する
	bool						IsExistTextureFiles() const ;										// テクスチャファイルのキーが存在するかどうかを確認する
	bool						IsExistHitAreas() const ;											// 当たり判定のキーが存在するかどうかを確認する
	bool						IsExistPhysicsFile() const ;										// 物理演算ファイルのキーが存在するかどうかを確認する
	bool						IsExistPoseFile() const ;											// ポーズ設定ファイルのキーが存在するかどうかを確認する
	bool						IsExistExpressionFile() const ;										// 表情設定ファイルのキーが存在するかどうかを確認する
	bool						IsExistMotionGroups() const ;										// モーショングループのキーが存在するかどうかを確認する
	bool						IsExistMotionGroupName( const char* groupName ) const ;				// 引数で指定したモーショングループのキーが存在するかどうかを確認する
	bool						IsExistMotionSoundFile( const char* groupName, int index ) const ;	// 引数で指定したモーションに対応するサウンドファイルのキーが存在するかどうかを確認する
	bool						IsExistMotionFadeIn( const char* groupName, int index ) const ;		// 引数で指定したモーションに対応するフェードイン時間のキーが存在するかどうかを確認する
	bool						IsExistMotionFadeOut( const char* groupName, int index ) const ;	// 引数で指定したモーションに対応するフェードアウト時間のキーが存在するかどうかを確認する
	bool						IsExistUserDataFile() const ;										// UserDataのファイル名が存在するか確認
	bool						IsExistEyeBlinkParameters() const ;									// 目パチに対応付けられたパラメータが存在するかどうかを確認する
	bool						IsExistLipSyncParameters() const ;									// リップシンクに対応付けられたパラメータが存在するかどうかを確認する

	D_CubismJson*				_json ;      // モデルデータjson 
	D_csmVector<D_JsonValue*>	_jsonValue ; // 上jsonの頻出ノード 
} ;

// 自動まばたき処理
class D_CubismEyeBlink
{
public:
	// まばたきの状態
	enum EyeState
	{
		EyeState_First							= 0,		// 初期状態
		EyeState_Interval						= 1,		// まばたきしていない状態
		EyeState_Closing						= 2,		// まぶたが閉じていく途中の状態
		EyeState_Closed							= 3,		// まぶたが閉じている状態
		EyeState_Opening						= 4			// まぶたが開いていく途中の状態
	} ;

	static D_CubismEyeBlink*	Create( D_ICubismModelSetting* modelSetting = NULL ) ;				// インスタンスの作成
	static void					Delete( D_CubismEyeBlink* eyeBlink ) ;								// インスタンスの破棄
	void						SetBlinkingInterval( float blinkigInterval ) ;						// まばたきの間隔の設定
	void						SetBlinkingSettings( float closing, float closed, float opening ) ;	// またばきのモーションの詳細設定
	void						SetParameterIds( const D_csmVector<D_CubismIdHandle>& parameterIds ) ;// まばたきさせるパラメータIDのリストの設定
	const D_csmVector<D_CubismIdHandle>&     GetParameterIds() const ;								// まばたきさせるパラメータIDのリストの取得
	void						UpdateParameters( D_CubismModel* model, float deltaTimeSeconds ) ;	// モデルのパラメータの更新

public:
	D_CubismEyeBlink( D_ICubismModelSetting* modelSetting ) ;								// コンストラクタ
	virtual ~D_CubismEyeBlink() ;															// デストラクタ
	float						DeterminNextBlinkingTiming() const ;						// 次のまばたきのタイミングの決定

	int							_blinkingState ;			// 現在の状態
	D_csmVector<D_CubismIdHandle>   _parameterIds ;			// 操作対象のパラメータのIDのリスト
	float						_nextBlinkingTime ;			// 次のまばたきの時刻[秒]
	float						_stateStartTimeSeconds ;	// 現在の状態が開始した時刻[秒]
	float						_blinkingIntervalSeconds ;	// まばたきの間隔[秒]
	float						_closingSeconds ;			// まぶたを閉じる動作の所要時間[秒]
	float						_closedSeconds ;			// まぶたを閉じている動作の所要時間[秒]
	float						_openingSeconds ;			// まぶたを開く動作の所要時間[秒]
	float						_userTimeSeconds ;			// デルタ時間の積算値[秒]

} ;

// 呼吸処理
class D_CubismBreath
{
public:
	// 呼吸のパラメータ情報
	struct BreathParameterData
	{
		BreathParameterData() : ParameterId( NULL ), Offset( 0.0f ), Peak( 0.0f ), Cycle( 0.0f ), Weight( 0.0f ) { }	// コンストラクタ
		BreathParameterData( D_CubismIdHandle parameterId, float offset, float peak, float cycle, float weight )		// コンストラクタ
			: ParameterId( parameterId )
			, Offset( offset )
			, Peak( peak )
			, Cycle( cycle )
			, Weight( weight )
		{ }
		D_CubismIdHandle		ParameterId ;				// 呼吸をひもづけるパラメータID
		float					Offset ;					// 呼吸を正弦波としたときの、波のオフセット
		float					Peak ;						// 呼吸を正弦波としたときの、波の高さ
		float					Cycle ;						// 呼吸を正弦波としたときの、波の周期
		float					Weight ;					// パラメータへの重み
	} ;
	static D_CubismBreath*		Create() ;																	// インスタンスの作成
	static void					Delete( D_CubismBreath* instance ) ;										// インスタンスの破棄
	void						SetParameters( D_csmVector<BreathParameterData>& breathParameters ) ;		// 呼吸のパラメータのひもづけ
	D_csmVector<BreathParameterData>& GetParameters() ;														// 呼吸にひもづいているパラメータの取得
	void						UpdateParameters( D_CubismModel* model, float deltaTimeSeconds ) ;			// モデルのパラメータの更新

private:
	D_CubismBreath() ;																		// コンストラクタ
	virtual ~D_CubismBreath() ;																// デストラクタ

	D_csmVector<BreathParameterData> _breathParameters ;	// 呼吸にひもづいているパラメータのリスト
	float						_currentTime ;				// 積算時間[秒]
} ;

// パーツの不透明度の設定
class D_CubismPose
{
public:
	// パーツにまつわるデータを管理
	struct PartData
	{
		PartData() ;																		// コンストラクタ
		PartData( const PartData& v ) ;														// コピーコンストラクタ
		virtual ~PartData() ;																// デストラクタ
		PartData&				operator=( const PartData& v ) ;							// 代入のオーバーロード
		void					Initialize( D_CubismModel* model ) ;						// 初期化

		D_CubismIdHandle		PartId ;					// パーツID
		int						ParameterIndex ;			// パラメータのインデックス
		int						PartIndex ;					// パーツのインデックス
		D_csmVector<PartData>	Link ;						// 連動するパラメータ
	} ;
	static D_CubismPose*		Create( const BYTE* pose3json, size_t size ) ;// インスタンスの作成
	static void					Delete( D_CubismPose* pose ) ;// インスタンスの破棄
	void						UpdateParameters( D_CubismModel* model, float deltaTimeSeconds ) ;// モデルのパラメータの更新

private:
	D_CubismPose() ;																		// コンストラクタ
	virtual ~D_CubismPose() ;																// デストラクタ
	void						Reset( D_CubismModel* model ) ;								// 表示を初期化
	void						CopyPartOpacities( D_CubismModel* model ) ;					// パーツの不透明度をコピー
	void						DoFade( D_CubismModel* model, float deltaTimeSeconds, int beginIndex, int partGroupCount ) ;// パーツのフェード操作を実行

	D_csmVector<PartData>		_partGroups ;				// パーツグループ
	D_csmVector<int>			_partGroupCounts ;			// それぞれのパーツグループの個数
	float						_fadeTimeSeconds ;			// フェード時間[秒]
	D_CubismModel*				_lastModel ;				// 前回操作したモデル
} ;

// 顔の向きの制御処理
class D_CubismTargetPoint
{
public:
	D_CubismTargetPoint() ;																	// コンストラクタ
	virtual ~D_CubismTargetPoint() ;														// デストラクタ
	void						Update( float deltaTimeSeconds ) ;							// 更新処理
	float						GetX() const ;												// X軸の顔の向きの値を取得
	float						GetY() const ;												// Y軸の顔の向きの値を取得
	void						Set( float x, float y ) ;									// 顔の向きの目標値を設定

private:
	float						_faceTargetX ;				// 顔の向きのX目標値(この値に近づいていく)
	float						_faceTargetY ;				// 顔の向きのY目標値(この値に近づいていく)
	float						_faceX ;					// 顔の向きX(-1.0 - 1.0)
	float						_faceY ;					// 顔の向きY(-1.0 - 1.0)
	float						_faceVX ;					// 顔の向きの変化速度X
	float						_faceVY ;					// 顔の向きの変化速度Y
	float						_lastTimeSeconds ;			// 最後の実行時間[秒]
	float						_userTimeSeconds ;			// デルタ時間の積算値[秒]
} ;

// 物理演算の適用先の種類
enum D_CubismPhysicsTargetType
{
    D_CubismPhysicsTargetType_Parameter			= 0,		// パラメータに対して適用
} ;

// 物理演算の入力の種類
enum D_CubismPhysicsSource
{
    D_CubismPhysicsSource_X						= 0,		// X軸の位置から
    D_CubismPhysicsSource_Y						= 1,		// Y軸の位置から
    D_CubismPhysicsSource_Angle					= 2,		// 角度から
} ;

// 物理演算で使用する外部の力
struct D_PhysicsJsonEffectiveForces
{
    D_CubismVector2				Gravity ;					// 重力
    D_CubismVector2				Wind ;						// 風
} ;

// 物理演算のパラメータ情報
struct D_CubismPhysicsParameter
{
    D_CubismIdHandle			Id ;						// パラメータID
    D_CubismPhysicsTargetType	TargetType ;				// 適用先の種類
} ;

// 物理演算の正規化情報
struct D_CubismPhysicsNormalization
{
    float						Minimum ;					// 最大値
    float						Maximum ;					// 最小値
    float						Default ;					// デフォルト値
} ;

// 物理演算の演算に使用する物理点の情報
struct D_CubismPhysicsParticle
{
    D_CubismVector2				InitialPosition ;			// 初期位置
    float						Mobility ;					// 動きやすさ
    float						Delay ;						// 遅れ
    float						Acceleration ;				// 加速度
    float						Radius ;					// 距離
    D_CubismVector2				Position ;					// 現在の位置
    D_CubismVector2				LastPosition ;				// 最後の位置
    D_CubismVector2				LastGravity ;				// 最後の重力
    D_CubismVector2				Force ;						// 現在かかっている力
    D_CubismVector2				Velocity ;					// 現在の速度
} ;

// 物理演算の物理点の管理
struct D_CubismPhysicsSubRig
{
    int							InputCount ;				// 入力の個数
    int							OutputCount ;				// 出力の個数
    int							ParticleCount ;				// 物理点の個数
    int							BaseInputIndex ;			// 入力の最初のインデックス
    int							BaseOutputIndex ;			// 出力の最初のインデックス
    int							BaseParticleIndex ;			// 物理点の最初のインデックス
    D_CubismPhysicsNormalization NormalizationPosition ;	// 正規化された位置
    D_CubismPhysicsNormalization NormalizationAngle ;		// 正規化された角度
} ;

// 正規化されたパラメータの取得関数の宣言
typedef void ( *D_NormalizedPhysicsParameterValueGetter )(
    D_CubismVector2*			targetTranslation,
    float*						targetAngle,
    float						value,
    float						parameterMinimumValue,
    float						parameterMaximumValue,
    float						parameterDefaultValue,
    D_CubismPhysicsNormalization* normalizationPosition,
    D_CubismPhysicsNormalization* normalizationAngle,
    int							isInverted,
    float						weight
) ;

// 物理演算の値の取得関数の宣言
typedef float ( *D_PhysicsValueGetter )(
    D_CubismVector2				translation,
    D_CubismPhysicsParticle*	particles,
    int							particleIndex,
    int							isInverted,
    D_CubismVector2				parentGravity
) ;

// 物理演算のスケールの取得関数の宣言
typedef float ( *D_PhysicsScaleGetter )( D_CubismVector2 translationScale, float angleScale ) ;

// 物理演算の入力情報
struct D_CubismPhysicsInput
{
    D_CubismPhysicsParameter	Source ;					// 入力元のパラメータ
    int							SourceParameterIndex ;		// 入力元のパラメータのインデックス
    float						Weight ;					// 重み
    short						Type ;						// 入力の種類
    short						Reflect ;					// 値が反転されているかどうか
    D_NormalizedPhysicsParameterValueGetter GetNormalizedParameterValue ;	// 正規化されたパラメータ値の取得関数
} ;

// 物理演算の出力情報
struct D_CubismPhysicsOutput
{
    D_CubismPhysicsParameter	Destination ;				// 出力先のパラメータ
    int							DestinationParameterIndex ;	// 出力先のパラメータのインデックス
    int							VertexIndex ;				// 振り子のインデックス
    D_CubismVector2				TranslationScale ;			// 移動値のスケール
    float						AngleScale ;				// 角度のスケール
    float						Weight ;					// 重み
    D_CubismPhysicsSource		Type ;						// 出力の種類
    short						Reflect ;					// 値が反転されているかどうか
    float						ValueBelowMinimum ;			// 最小値を下回った時の値
    float						ValueExceededMaximum ;		// 最大値をこえた時の値
    D_PhysicsValueGetter		GetValue ;					// 物理演算の値の取得関数
    D_PhysicsScaleGetter		GetScale ;					// 物理演算のスケール値の取得関数
} ;

// 物理演算のデータ
struct D_CubismPhysicsRig
{
    int							SubRigCount ;				// 物理演算の物理点の個数
    D_csmVector<D_CubismPhysicsSubRig> Settings ;			// 物理演算の物理点の管理のリスト
    D_csmVector<D_CubismPhysicsInput> Inputs ;				// 物理演算の入力のリスト
    D_csmVector<D_CubismPhysicsOutput> Outputs ;			// 物理演算の出力のリスト
    D_csmVector<D_CubismPhysicsParticle> Particles ;		// 物理演算の物理点のリスト
    D_CubismVector2				Gravity ;					// 重力
    D_CubismVector2				Wind ;						// 風
} ;

// physics3.jsonのコンテナ
class D_CubismPhysicsJson
{
public:
	D_CubismPhysicsJson( const BYTE* buffer, size_t size ) ;													// コンストラクタ
	virtual ~D_CubismPhysicsJson() ;																			// デストラクタ
	D_CubismVector2				GetGravity() const ;														// 重力の取得
	D_CubismVector2				GetWind() const ;															// 風の取得
	int							GetSubRigCount() const ;													// 物理点の管理の個数の取得
	int							GetTotalInputCount() const ;												// 入力の総合計の取得
	int							GetTotalOutputCount() const ;												// 出力の総合計の取得
	int							GetVertexCount() const ;													// 物理点の個数の取得
	float						GetNormalizationPositionMinimumValue( int physicsSettingIndex ) const ;		// 正規化された位置の最小値の取得
	float						GetNormalizationPositionMaximumValue( int physicsSettingIndex ) const ;		// 正規化された位置の最大値の取得
	float						GetNormalizationPositionDefaultValue( int physicsSettingIndex ) const ;		// 正規化された位置のデフォルト値の取得
	float						GetNormalizationAngleMinimumValue( int physicsSettingIndex ) const ;		// 正規化された角度の最小値の取得
	float						GetNormalizationAngleMaximumValue( int physicsSettingIndex ) const ;		// 正規化された角度の最大値の取得
	float						GetNormalizationAngleDefaultValue( int physicsSettingIndex ) const ;		// 正規化された角度のデフォルト値の取得
	int							GetInputCount( int physicsSettingIndex ) const ;							// 入力の個数の取得
	float						GetInputWeight( int physicsSettingIndex, int inputIndex ) const ;			// 入力の重みの取得
	bool						GetInputReflect( int physicsSettingIndex, int inputIndex ) const ;			// 入力の反転の取得
	const char*					GetInputType( int physicsSettingIndex, int inputIndex ) const ;				// 入力の種類の取得
	D_CubismIdHandle			GetInputSourceId( int physicsSettingIndex, int inputIndex ) const ;			// 入力元のIDの取得
	int							GetOutputCount( int physicsSettingIndex ) const ;							// 出力の個数の取得
	int							GetOutputVertexIndex( int physicsSettingIndex, int outputIndex ) const ;	// 出力の物理点のインデックスの取得
	float						GetOutputAngleScale( int physicsSettingIndex, int outputIndex ) const ;		// 出力の角度のスケールの取得
	float						GetOutputWeight( int physicsSettingIndex, int outputIndex ) const ;			// 出力の重みの取得
	D_CubismIdHandle			GetOutputsDestinationId( int physicsSettingIndex, int outputIndex ) const ;	// 出力先のIDの取得
	const char*					GetOutputType( int physicsSettingIndex, int outputIndex ) const ;			// 出力の種類の取得
	bool						GetOutputReflect( int physicsSettingIndex, int outputIndex ) const ;		// 出力の反転の取得
	int							GetParticleCount( int physicsSettingIndex ) const ;							// 物理点の個数の取得
	float						GetParticleMobility( int physicsSettingIndex, int vertexIndex ) const ;		// 物理点の動きやすさの取得
	float						GetParticleDelay( int physicsSettingIndex, int vertexIndex ) const ;		// 物理点の遅れの取得
	float						GetParticleAcceleration( int physicsSettingIndex, int vertexIndex ) const ;	// 物理点の加速度の取得
	float						GetParticleRadius( int physicsSettingIndex, int vertexIndex ) const ;		// 物理点の距離の取得
	D_CubismVector2				GetParticlePosition( int physicsSettingIndex, int vertexIndex ) const ;		// 物理点の位置の取得

private:
	D_CubismJson* _json ;          // physics3.jsonデータ
} ;

// 物理演算クラス
class D_CubismPhysics
{
public:
	// オプション
	struct Options
	{
		D_CubismVector2			Gravity ;					// 重力方向
		D_CubismVector2			Wind ;						// 風の方向
	} ;
	static D_CubismPhysics*		Create( const BYTE* buffer, size_t size ) ;					// インスタンスの作成
	static void					Delete( D_CubismPhysics* physics ) ;						// インスタンスの破棄
	void						Evaluate( D_CubismModel* model, float deltaTimeSeconds ) ;	// 物理演算の評価
	void						SetOptions( const Options& options ) ;						// オプションの設定
	const Options&				GetOptions() const ;										// オプションの取得

private:
	D_CubismPhysics() ;																		// コンストラクタ
	virtual ~D_CubismPhysics() ;															// デストラクタ
	D_CubismPhysics( const D_CubismPhysics& ) ;
	D_CubismPhysics&			operator=( const D_CubismPhysics& ) ;
	void						Parse( const BYTE* physicsJson, size_t size ) ;				// physics3.jsonのパース
	void						Initialize() ;												// 初期化

	D_CubismPhysicsRig*			_physicsRig ;				// 物理演算のデータ
	Options						_options ;					// オプション
} ;

class D_CubismModelUserDataJson
{
public:
	D_CubismModelUserDataJson( const BYTE* buffer, size_t size ) ;							// コンストラクタ
	virtual ~D_CubismModelUserDataJson() ;													// デストラクタ
	int							GetUserDataCount() const ;									// ユーザデータ個数の取得
	int							GetTotalUserDataSize() const ;								// ユーザデータ総文字列数の取得
	D_csmString					GetUserDataTargetType( int i ) const ;						// ユーザデータのタイプの取得
	D_CubismIdHandle			GetUserDataId( int i ) const ;								// ユーザデータのターゲットIDの取得
	const char*					GetUserDataValue( int i ) const ;							// ユーザデータの文字列の取得

private:
	D_CubismJson*				_json ;
} ;

// ユーザデータの管理クラス
class D_CubismModelUserData
{
public:
	// ユーザデータ構造体
	struct CubismModelUserDataNode
	{
		D_CubismIdHandle		TargetType ;				// ユーザデータターゲットタイプ
		D_CubismIdHandle		TargetId ;					// ユーザデータターゲットのID
		D_csmString				Value ;						// ユーザデータ
	} ;
	static D_CubismModelUserData* Create( const BYTE* buffer, size_t size ) ;				// インスタンスの作成
	static void					Delete( D_CubismModelUserData* modelUserData ) ;			// インスタンスの破棄
	virtual ~D_CubismModelUserData() ;														// デストラクタ
	const D_csmVector<const CubismModelUserDataNode*>& GetArtMeshUserDatas() const ;		// ArtMeshのユーザデータのリストの取得

private:
	void ParseUserData( const BYTE* buffer, size_t size ) ;									// userdata3.jsonのパース

	D_csmVector<const CubismModelUserDataNode*> _userDataNodes ;		// ユーザデータ構造体配列
	D_csmVector<const CubismModelUserDataNode*> _artMeshUserDataNodes ;	// 閲覧リスト保持
} ;

//  前方宣言
class D_CubismRenderer_DxLib ;
class D_CubismShader_DxLib ;
class D_CubismClippingContext ;

//  オフスクリーン描画用構造体
class D_CubismOffscreenFrame_DxLib
{
public:
	D_CubismOffscreenFrame_DxLib() ;
	void						BeginDraw() ;												// 指定の描画ターゲットに向けて描画開始
	void						EndDraw() ;													// 描画終了
	void						Clear( float r, float g, float b, float a ) ;				// レンダリングターゲットのクリア
	bool						CreateOffscreenFrame( DWORD displayBufferWidth, DWORD displayBufferHeight ) ;// CubismOffscreenFrame作成
	void						DestroyOffscreenFrame() ;									// CubismOffscreenFrameの削除
	void						SetClearColor( float r, float g, float b, float a ) ;		// クリアカラーの上書き
	int							GetTextureView() const ;									// テクスチャビューへのアクセッサ
	DWORD						GetBufferWidth() const ;									// バッファ幅取得
	DWORD						GetBufferHeight() const ;									// バッファ高さ取得
	bool						IsValid() const ;											// 現在有効かどうか

private:
	int							_GraphHandle ;				// 生成テクスチャ 
	int							_BackupDrawScreen ;
	DWORD						_bufferWidth ;				// Create時に指定されたサイズ 
	DWORD						_bufferHeight ;				// Create時に指定されたサイズ 

} ;

// DirectX::XMMATRIXに変換
#ifndef DX_NON_NAMESPACE
DxLib::MATRIX D_ConvertToD3DX( D_CubismMatrix44& mtx ) ;
#else // DX_NON_NAMESPACE
MATRIX D_ConvertToD3DX( D_CubismMatrix44& mtx ) ;
#endif // DX_NON_NAMESPACE

//  クリッピングマスクの処理を実行するクラス
class D_CubismClippingManager_DxLib
{
	friend class D_CubismShader_DxLib ;
	friend class D_CubismRenderer_DxLib ;

private:
	D_CubismClippingManager_DxLib() ;														//  コンストラクタ
	virtual ~D_CubismClippingManager_DxLib() ;												//  デストラクタ
	D_CubismRenderer::CubismTextureColor* GetChannelFlagAsColor( int channelNo ) ;			// カラーチャンネル(RGBA)のフラグを取得する
	void						CalcClippedDrawTotalBounds( D_CubismModel& model, D_CubismClippingContext* clippingContext ) ;// マスクされる描画オブジェクト群全体を囲む矩形(モデル座標系)を計算する
	void						Initialize( D_CubismModel& model, int drawableCount, const int** drawableMasks, const int* drawableMaskCounts ) ;//  マネージャの初期化処理
	void						SetupClippingContext( D_CubismModel& model, D_CubismRenderer_DxLib* renderer, D_CubismOffscreenFrame_DxLib& useTarget ) ;// クリッピングコンテキストを作成する。モデル描画時に実行する。
	D_CubismClippingContext*	FindSameClip( const int* drawableMasks, int drawableMaskCounts ) const ;// 既にマスクを作っているかを確認。
	void						SetupLayoutBounds( int usingClipCount ) const ;				// クリッピングコンテキストを配置するレイアウト。
	D_CubismOffscreenFrame_DxLib* GetColorBuffer() const ;									// カラーバッファのアドレスを取得する
	D_csmVector<D_CubismClippingContext*>* GetClippingContextListForDraw() ;				// 画面描画に使用するクリッピングマスクのリストを取得する
	void						SetClippingMaskBufferSize( int size ) ;						// クリッピングマスクバッファのサイズを設定する
	int							GetClippingMaskBufferSize() const ;							// クリッピングマスクバッファのサイズを取得する

	D_CubismOffscreenFrame_DxLib*						_colorBuffer ;						// マスク用カラーバッファーのアドレス
	int													_currentFrameNo ;					// マスクテクスチャに与えるフレーム番号
	D_csmVector<D_CubismRenderer::CubismTextureColor*>	_channelColors ;
	D_csmVector<D_CubismClippingContext*>				_clippingContextListForMask ;		// マスク用クリッピングコンテキストのリスト
	D_csmVector<D_CubismClippingContext*>				_clippingContextListForDraw ;		// 描画用クリッピングコンテキストのリスト
	int													_clippingMaskBufferSize ;			// クリッピングマスクのバッファサイズ（初期値:256）
	D_CubismMatrix44									_tmpMatrix ;						// マスク計算用の行列
	D_CubismMatrix44									_tmpMatrixForMask ;					// マスク計算用の行列
	D_CubismMatrix44									_tmpMatrixForDraw ;					// マスク計算用の行列
	D_csmRectF											_tmpBoundsOnModel ;					// マスク配置計算用の矩形
} ;

// クリッピングマスクのコンテキスト
class D_CubismClippingContext
{
	friend class D_CubismClippingManager_DxLib;
	friend class D_CubismShader_DxLib;
	friend class D_CubismRenderer_DxLib;

public:
	D_CubismClippingContext( D_CubismClippingManager_DxLib* manager, const int* clippingDrawableIndices, int clipCount ) ;// 引数付きコンストラクタ
	virtual ~D_CubismClippingContext() ;													// デストラクタ
	void						AddClippedDrawable( int drawableIndex ) ;					// このマスクにクリップされる描画オブジェクトを追加する
	D_CubismClippingManager_DxLib*	GetClippingManager() ;									// このマスクを管理するマネージャのインスタンスを取得する。

	bool						_isUsing ;					// 現在の描画状態でマスクの準備が必要ならtrue
	const int*					_clippingIdList ;			// クリッピングマスクのIDリスト
	int							_clippingIdCount ;			// クリッピングマスクの数
	int							_layoutChannelNo ;			// RGBAのいずれのチャンネルにこのクリップを配置するか(0:R , 1:G , 2:B , 3:A)
	D_csmRectF*					_layoutBounds ;				// マスク用チャンネルのどの領域にマスクを入れるか(View座標-1..1, UVは0..1に直す)
	D_csmRectF*					_allClippedDrawRect ;		// このクリッピングで、クリッピングされる全ての描画オブジェクトの囲み矩形（毎回更新）
	D_CubismMatrix44			_matrixForMask ;			// マスクの位置計算結果を保持する行列
	D_CubismMatrix44			_matrixForDraw ;			// 描画オブジェクトの位置計算結果を保持する行列
	D_csmVector<int>*			_clippedDrawableIndexList ;	// このマスクにクリップされる描画オブジェクトのリスト
	D_CubismClippingManager_DxLib* _owner;					// このマスクを管理しているマネージャのインスタンス
} ;

class D_CubismRenderer_DxLib ;

//  CubismDX11内部で設定するステートのうち、途中で変更する可能性のあるものを管理。D_CubismRenderer_DxLibがシングルトンとして管理。
class D_CubismRenderState_DxLib
{
	friend class D_CubismRenderer_DxLib;
public:
	enum
	{
		State_None								= 0,
		State_Blend								= 1,		// ブレンドモード 
		State_Viewport							= 2,		// ビューポート 
		State_ZEnable							= 3,		// Z有効無効 
		State_CullMode							= 4,		// カリングモード 
		State_Sampler							= 5,		// テクスチャフィルター 
		State_Max								= 6,
	} ;
	// ブレンドステート D3D11ではオブジェクト単位での管理 
	enum Blend
	{
		Blend_Origin							= 0,
		Blend_Zero								= 1,
		Blend_Normal							= 2,
		Blend_Add								= 3,
		Blend_Mult								= 4,
		Blend_Mask								= 5,
		Blend_Max								= 6,
	} ;
	// カリング D3D11ではオブジェクト単位での管理 
	enum Cull
	{
		Cull_Origin								= 0,		// 元々の設定 
		Cull_None								= 1,		// カリング無し 
		Cull_Ccw								= 2,		// CCW表示 
		Cull_Max								= 3,
	} ;
	// Z D3D11ではオブジェクト単位での管理 
	enum Depth
	{
		Depth_Origin							= 0,		// 元々の設定 
		Depth_Disable							= 1,		// Zoff 
		Depth_Enable							= 2,		// Zon 
		Depth_Max								= 3,
	} ;
	// サンプラーステート D3D11ではオブジェクト単位での管理 
	enum Sampler
	{
		Sampler_Origin							= 0,		// 元々の設定 
		Sampler_Normal							= 1,		// 使用ステート 
		Sampler_Max								= 3,
	} ;

	// デフォルトの=でコピーします
	struct Stored
	{
		Stored()
		{
			_blendState = Blend_Zero;
			_blendFactor_r = 0.0f;
			_blendFactor_g = 0.0f;
			_blendFactor_b = 0.0f;
			_blendFactor_a = 0.0f;
			_blendMask = 0xffffffff;
			_cullMode = Cull_None;
			_depthEnable = Depth_Disable;
			_depthRef = 0 ;
			_viewportX = 0 ;
			_viewportY = 0 ;
			_viewportWidth = 0 ;
			_viewportHeight = 0 ;
			_viewportMinZ = 0.0f;
			_viewportMaxZ = 0.0f;
			_sampler = Sampler_Normal;
		// DirectX::XMMATRIXに変換
		#ifndef DX_NON_NAMESPACE
			DxLib::_MEMSET( _valid, 0, sizeof( _valid ) ) ;
		#else // DX_NON_NAMESPACE
			_MEMSET( _valid, 0, sizeof( _valid ) ) ;
		#endif // DX_NON_NAMESPACE
		}
		// State_Blend 
		Blend					_blendState ;
		float					_blendFactor_r ;
		float					_blendFactor_g ;
		float					_blendFactor_b ;
		float					_blendFactor_a ;
		DWORD					_blendMask ;
		// State_CullMode 
		Cull					_cullMode ;
		// State_Viewport 
		float					_viewportX ;
		float					_viewportY ;
		float					_viewportWidth ;
		float					_viewportHeight ;
		float					_viewportMinZ ;
		float					_viewportMaxZ ;
		// State_ZEnable 
		Depth					_depthEnable ;
		DWORD					_depthRef ;
		// State_Sampler 
		Sampler					_sampler ;
		bool					_valid[ State_Max ] ;   // 設定したかどうか。現在はStartFrameで一通りは呼んでいる 
	} ;

	void						StartFrame() ;												// フレーム先頭で呼び出す処理
	void						Save() ;													// 管理中のステートをPush
	void						Restore() ;													// Push下ステートをPop、Saveの際にD_CubismRenderState_DxLibで設定していなかった項目は戻せないことに注意
	void						SetBlend( Blend blendState, float blendFactor_r, float blendFactor_g, float blendFactor_b, float blendFactor_a, DWORD mask, bool force=false ) ;// ブレンドモードセット
	void						SetCullMode( Cull cullFace, bool force = false ) ;			// カリングモードセット
	void						SetViewport( float left, float top, float width, float height, float zMin, float zMax, bool force = false ) ;// ビューポートセット
	void						SetZEnable( Depth enable, DWORD stelcilRef, bool force = false ) ;// Z有効無効セット
	void						SetSampler( Sampler sample, bool force = false ) ;			//  サンプラーステートセット

private:
	D_CubismRenderState_DxLib() ;
	~D_CubismRenderState_DxLib() ;
	void						Create() ;													// 各種オブジェクトを作成する
	void						SaveCurrentNativeState() ;									// D3DDeviceから、Cubismに関係する値を取得したうえでD_CubismRenderState_DxLibに反映しSaveを呼び出し、_pushedは破棄、Cubismフレーム処理の最初、StartFrameの後で呼んでいる
	void						RestoreNativeState() ;										// 最初にPushしたステートまで戻す

	Stored						_stored ;					// ストアされた各種設定 
	D_csmVector<Stored>			_pushed ;
	int							backupBlendMode ;
	int							backupBlendParam ;
	int							backupCullMode ;
	int							backupUseZBuffer ;
	int							backupMaxAnisotropy ;
	int							backupDrawMode ;
	RECT						backupDrawArea ;
	int							backupDrawAlphaTestMode ;
	int							backupDrawAlphaTestParam ;
	int							backupDrawBright_r ;
	int							backupDrawBright_g ;
	int							backupDrawBright_b ;
} ;

enum D_ShaderNames
{
	// SetupMask
	D_ShaderNames_SetupMask									= 0,

	//Normal
	D_ShaderNames_Normal									= 1,
	D_ShaderNames_NormalMasked								= 2,
	D_ShaderNames_NormalMaskedInverted						= 3,
	D_ShaderNames_NormalPremultipliedAlpha					= 4,
	D_ShaderNames_NormalMaskedPremultipliedAlpha			= 5,
	D_ShaderNames_NormalMaskedInvertedPremultipliedAlpha	= 6,

	//Add
	D_ShaderNames_Add										= 7,
	D_ShaderNames_AddMasked									= 8,
	D_ShaderNames_AddMaskedInverted							= 9,
	D_ShaderNames_AddPremultipliedAlpha						= 10,
	D_ShaderNames_AddMaskedPremultipliedAlpha				= 11,
	D_ShaderNames_AddMaskedInvertedPremultipliedAlpha		= 12,

	//Mult
	D_ShaderNames_Mult										= 13,
	D_ShaderNames_MultMasked								= 14,
	D_ShaderNames_MultMaskedInverted						= 15,
	D_ShaderNames_MultPremultipliedAlpha					= 16,
	D_ShaderNames_MultMaskedPremultipliedAlpha				= 17,
	D_ShaderNames_MultMaskedInvertedPremultipliedAlpha		= 18,

	D_ShaderNames_Max										= 19,
} ;

//  前方宣言
class D_CubismRenderer_DxLib ;
class D_CubismClippingContext ;

// Cubismで使用するシェーダ管理クラス、D_CubismRenderer_DxLibのstatic変数として一つだけ実体化される
class D_CubismShader_DxLib
{
	friend class D_CubismRenderer_DxLib;

public:
	D_CubismShader_DxLib() ;																// privateなコンストラクタ
	virtual ~D_CubismShader_DxLib() ;														// privateなデストラクタ
	void						ReleaseShaderProgram() ;									// シェーダプログラムを解放する
	int							GetVertexShader( DWORD assign ) ;							// 頂点シェーダの取得
	int							GetPixelShader( DWORD assign ) ;							// ピクセルシェーダの取得
	void						SetupShader() ;												// 頂点宣言のデバイスへの設定、シェーダがまだ未設定ならロード

private:
	void						GenerateShaders() ;											// シェーダプログラムを初期化する
	bool						LoadShaderProgram( bool isPs, int assign, const char* entryPoint ) ;// シェーダプログラムをロード

	int							_shaderSetsVS[ D_ShaderNames_Max ] ;	// ロードしたシェーダプログラムを保持する変数(VS) 
	int							_shaderSetsPS[ D_ShaderNames_Max ] ;	// ロードしたシェーダプログラムを保持する変数(PS) 
} ;

// シェーダーコンスタントバッファ
struct D_CubismConstantBufferDxLib
{
#ifndef DX_NON_NAMESPACE
	DxLib::MATRIX				projectMatrix ;
	DxLib::MATRIX				clipMatrix ;
	DxLib::COLOR_F				baseColor ;
	DxLib::COLOR_F				multiplyColor ;
	DxLib::COLOR_F				screenColor ;
	DxLib::COLOR_F				channelFlag ;
#else // DX_NON_NAMESPACE
	MATRIX						projectMatrix ;
	MATRIX						clipMatrix ;
	COLOR_F						baseColor ;
	COLOR_F						multiplyColor ;
	COLOR_F						screenColor ;
	COLOR_F						channelFlag ;
#endif // DX_NON_NAMESPACE
} ;

// DirectX11用の描画命令を実装したクラス
class D_CubismRenderer_DxLib : public D_CubismRenderer
{
	friend class D_CubismRenderer;
	friend class D_CubismClippingManager_DxLib;
	friend class D_CubismShader_DxLib;

public:
	static void					InitializeConstantSettings( DWORD bufferSetNum ) ;			// レンダラを作成するための各種設定
	static void					SetDefaultRenderState() ;									// CubismRenderStateにデフォルトの設定をセットする
	static void					StartFrame( DWORD viewportWidth, DWORD viewportHeight ) ;	// Cubism描画関連の先頭で行う処理。各フレームでのCubism処理前にこれを呼んでもらう
	static void					EndFrame() ;												// Cubism描画関連の終了時行う処理。各フレームでのCubism処理前にこれを呼んでもらう
	static D_CubismRenderState_DxLib* GetRenderStateManager() ;								// D_CubismRenderer_DxLibで使用するレンダーステート管理マネージャ取得
	static void					DeleteRenderStateManager() ;								// レンダーステート管理マネージャ削除
	static D_CubismShader_DxLib* GetShaderManager() ;										// シェーダ管理機構の取得
	static void					DeleteShaderManager() ;										// シェーダ管理機構の削除
	static void					OnDeviceLost() ;											// デバイスロスト・デバイス再作成時コールする
	static void					GenerateShader() ;											// 使用シェーダー作成
	virtual void				Initialize( D_CubismModel* model, int ASyncThread ) ;		// レンダラの初期化処理を実行する、引数に渡したモデルからレンダラの初期化処理に必要な情報を取り出すことができる
	void						BindTexture( DWORD modelTextureAssign, int textureView ) ;	// OpenGLテクスチャのバインド処理、D_CubismRendererにテクスチャを設定し、D_CubismRenderer中でその画像を参照するためのIndex値を戻り値とする
	const D_csmMap<int, int>&	GetBindedTextures() const ;									// OpenGLにバインドされたテクスチャのリストを取得する
	void						SetClippingMaskBufferSize( int size ) ;						//  クリッピングマスクバッファのサイズを設定する、マスク用のFrameBufferを破棄・再作成するため処理コストは高い。
	int							GetClippingMaskBufferSize() const ;							//  クリッピングマスクバッファのサイズを取得する
	void						ExecuteDraw( int vertexBuffer, int indexBuffer, int constantBuffer, const int indexCount, const int textureNo, CubismTextureColor& modelColorRGBA, const CubismTextureColor& multiplyColor, const CubismTextureColor& screenColor, D_CubismBlendMode colorBlendMode, bool invertedMask ) ;	//  使用するシェーダの設定・コンスタントバッファの設定などを行い、描画を実行

protected:
	D_CubismRenderer_DxLib() ;																// コンストラクタ
	virtual ~D_CubismRenderer_DxLib() ;														// デストラクタ
	virtual void				DoDrawModel() ;												// モデルを描画する実際の処理
	void						DrawMesh( int textureNo, int indexCount, int vertexCount, WORD* indexArray, float* vertexArray, float* uvArray, float opacity, D_CubismBlendMode colorBlendMode, bool invertedMask ) ;
	void						DrawMeshDX11( int drawableIndex, int textureNo, int indexCount, int vertexCount, WORD* indexArray, float* vertexArray, float* uvArray, const CubismTextureColor& multiplyColor, const CubismTextureColor& screenColor, float opacity, D_CubismBlendMode colorBlendMode, bool invertedMask ) ;// 描画オブジェクト（アートメッシュ）を描画する。ポリゴンメッシュとテクスチャ番号をセットで渡す。

private:
	static void					DoStaticRelease() ;											// レンダラが保持する静的なリソースを解放する
	static void					ReleaseShader() ;											// 使用シェーダーと頂点定義の削除
	D_CubismRenderer_DxLib( const D_CubismRenderer_DxLib& ) ;								// Prevention of copy Constructor
	D_CubismRenderer_DxLib&		operator=( const D_CubismRenderer_DxLib& ) ;
	void						PreDraw() ;													// 描画開始時の追加処理。モデルを描画する前にクリッピングマスクに必要な処理を実装している。
	void						PostDraw() ;												// 描画完了後の追加処理。
	virtual void				SaveProfile() ;												// モデル描画直前のステートを保持する
	virtual void				RestoreProfile() ;											// モデル描画直前のステートを保持する
	void						SetClippingContextBufferForMask( D_CubismClippingContext* clip ) ;// マスクテクスチャに描画するクリッピングコンテキストをセットする。
	D_CubismClippingContext*	GetClippingContextBufferForMask() const ;					// マスクテクスチャに描画するクリッピングコンテキストを取得する。
	void						SetClippingContextBufferForDraw( D_CubismClippingContext* clip ) ;// 画面上に描画するクリッピングコンテキストをセットする。
	D_CubismClippingContext*	GetClippingContextBufferForDraw() const ;					// 画面上に描画するクリッピングコンテキストを取得する。
	void						CopyToBuffer( int drawAssign, const int vcount, const float* varray, const float* uvarray ) ;// GetDrawableVertices,GetDrawableVertexUvsの内容をバッファへコピー

	int**						_vertexBuffers ;			// 頂点のバッファ 
	int**						_indexBuffers ;				// インデックスのバッファ 
	int**						_constantBuffers ;			// 定数のバッファ
	DWORD						_drawableNum ;				// _vertexBuffers, _indexBuffersの確保数 
	int							_commandBufferNum ;
	int							_commandBufferCurrent ;
	D_csmVector<int>			_sortedDrawableIndexList ;	// 描画オブジェクトのインデックスを描画順に並べたリスト
	D_csmMap<int, int>			_textures ;					// モデルが参照するテクスチャとレンダラでバインドしているテクスチャとのマップ
	D_csmVector<D_CubismOffscreenFrame_DxLib>	_offscreenFrameBuffer ;			// マスク描画用のフレームバッファ 
	D_CubismClippingManager_DxLib*				_clippingManager ;				// クリッピングマスク管理オブジェクト
	D_CubismClippingContext*					_clippingContextBufferForMask ;	// マスクテクスチャに描画するためのクリッピングコンテキスト
	D_CubismClippingContext*					_clippingContextBufferForDraw ;	// 画面上描画するためのクリッピングコンテキスト
} ;

// ユーザーが実際に使用するモデル
class D_CubismUserModel
{
public:
	D_CubismUserModel() ;																	// コンストラクタ
	virtual ~D_CubismUserModel() ;															// デストラクタ
	virtual bool				IsInitialized() ;											// 初期化状態の取得
	virtual void				IsInitialized( bool v ) ;									// 初期化状態の設定
	virtual bool				IsUpdating() ;												// 更新状態の取得
	virtual void				IsUpdating( bool v ) ;										// 更新状態の設定
	virtual void				SetDragging( float x, float y )	;							// マウスドラッグ情報の設定
	virtual void				SetAcceleration( float x, float y, float z ) ;				// 加速度情報の設定
	D_CubismModelMatrix*		GetModelMatrix() const ;									// モデル行列の取得
	virtual void				SetOpacity( float a ) ;										// 不透明度の設定
	virtual float				GetOpacity() ;												// 不透明度の取得
	virtual void				LoadModel( const BYTE* buffer, size_t size ) ;				// モデルデータの読み込み
	virtual D_ACubismMotion*	LoadMotion( const BYTE* buffer, size_t size, const BYTE/*wchar_t*/ * name ) ;// モーションデータの読み込み
	virtual D_ACubismMotion*	LoadExpression( const BYTE* buffer, size_t size, const BYTE/*wchar_t*/ * name ) ;// 表情データの読み込み
	virtual void				LoadPose( const BYTE* buffer, size_t size ) ;				// ポーズデータの読み込み
	virtual void				LoadPhysics( const BYTE* buffer, size_t size ) ;			// 物理演算データの読み込み
	virtual void				LoadUserData( const BYTE* buffer, size_t size ) ;			// モデルに付属するユーザーデータを読み込む
	virtual bool				IsHit( D_CubismIdHandle drawableId, float pointX, float pointY ) ;// あたり判定の取得
	D_CubismModel*				GetModel() const ;											// モデルの取得
	D_CubismRenderer*			GetRenderer() { return _renderer ; }						// レンダラの取得
	void						CreateRenderer( int ASyncThread ) ;							// レンダラの生成
	void						DeleteRenderer() ;											// レンダラの解放
	virtual void				MotionEventFired( const D_csmString& eventValue ) ;			//  イベント発火時の標準処理
	static void					CubismDefaultMotionEventCallback( const D_CubismMotionQueueManager* caller, const D_csmString& eventValue, void* customData ) ;//  イベント用のCallback

public:
	D_CubismMoc*				_moc ;						// Mocデータ
	D_CubismModel*				_model ;					// Modelインスタンス
	D_CubismMotionManager*		_motionManager ;			// モーション管理
	D_CubismMotionManager*		_expressionManager ;		// 表情管理
	D_CubismEyeBlink*			_eyeBlink ;					// 自動まばたき
	D_CubismBreath*				_breath ;					// 呼吸
	D_CubismModelMatrix*		_modelMatrix ;				// モデル行列
	D_CubismPose*				_pose ;						// ポーズ管理
	D_CubismTargetPoint*		_dragManager ;				// マウスドラッグ
	D_CubismPhysics*			_physics ;					// 物理演算
	D_CubismModelUserData*		_modelUserData ;			// ユーザデータ
	bool						_initialized ;				// 初期化されたかどうか
	bool						_updating ;					// 更新されたかどうか
	float						_opacity ;					// 不透明度
	bool						_lipSync ;					// リップシンクするかどうか
	float						_lastLipSyncValue ;			// 最後のリップシンクの制御値
	float						_dragX ;					// マウスドラッグのX位置
	float						_dragY ;					// マウスドラッグのY位置
	float						_accelerationX ;			// X軸方向の加速度
	float						_accelerationY ;			// Y軸方向の加速度
	float						_accelerationZ ;			// Z軸方向の加速度
	bool						_debugMode ;				// デバッグモードかどうか

private:
	D_CubismRenderer*			_renderer ;      // レンダラ
} ;

class D_CubismString
{
public:
	static D_csmString			GetFormatedString( const char* format, ... ) ;						// 標準出力の書式を適用した文字列を取得する。
	static bool					IsStartsWith( const char* text, const char* startWord ) ;			// textがstartWordで始まっているかどうかを返す
	static float				StringToFloat( const char* string, int length, int position, int* outEndPos ) ;// position位置の文字から数字を解析する。

private:
	// コンストラクタ・デストラクタ呼び出し不可な静的クラスにする
	D_CubismString() ;
} ;

struct D_LAppModelParameter
{
	int							parameterIndex ;
	float						parameterValue ;
} ;

// ユーザーが実際に使用するモデルの実装クラスモデル生成、処理コンポーネント生成、更新処理とレンダリングの呼び出しを行う。
class D_LAppModel : public D_CubismUserModel
{
public:
	D_LAppModel() ;																			// コンストラクタ
	virtual ~D_LAppModel() ;																// デストラクタ
	bool						LoadAssets( const BYTE/*wchar_t*/ * dir, const BYTE/*wchar_t*/ * fileName, int ASyncThread ) ;	// model3.jsonが置かれたディレクトリとファイルパスからモデルを生成する
	void						ReloadRenderer( int ASyncThread ) ;							// レンダラを再構築する
	void						Update( float deltaTimeSeconds ) ;							// モデルの更新処理。モデルのパラメータから描画状態を決定する。
	void						Draw( D_CubismMatrix44& matrix, bool isMultModelMatrix ) ;	// モデルを描画する処理。モデルを描画する空間のView-Projection行列を渡す。
	D_CubismMotionQueueEntryHandle StartMotion( const char* group, int no, int priority, float fadeInSeconds, float fadeOutSeconds, bool isLoopFadeIn = true, bool isLoop = false ) ;	// 引数で指定したモーションの再生を開始する。
	D_CubismMotionQueueEntryHandle StartRandomMotion( const char* group, int priority ) ;	// ランダムに選ばれたモーションの再生を開始する。
	void						SetExpression( const BYTE/*wchar_t*/ * expressionID ) ;		// 引数で指定した表情モーションをセットする
	void						SetRandomExpression() ;										// ランダムに選ばれた表情モーションをセットする
	virtual void				MotionEventFired( const D_csmString& eventValue ) ;			// イベントの発火を受け取る
	virtual bool				HitTest( const char* hitAreaName, float x, float y ) ;		//  当たり判定テスト。
	void						DeleteMark() { _deleteModel = true ; }						// モデルに削除マークを付ける
	D_CubismOffscreenFrame_DxLib& GetRenderBuffer() ;										// 別ターゲットに描画する際に使用するバッファの取得
	void						SetUserParameter( int parameterIndex, float parameterValue ) ;	// ユーザー設定のパラメータを設定する

protected:
	void						DoDraw() ;													// モデルを描画する処理。モデルを描画する空間のView-Projection行列を渡す。

public:
	bool						SetupModel( D_ICubismModelSetting* setting ) ;				// model3.jsonからモデルを生成する。model3.jsonの記述に従ってモデル生成、モーション、物理演算などのコンポーネント生成を行う。
	void						SetupTextures( int ASyncThread ) ;							// テクスチャをロードする
	void						PreloadMotionGroup( const char* group ) ;					// モーションデータをグループ名から一括でロードする。モーションデータの名前は内部でModelSettingから取得する。
	void						ReleaseMotionGroup( const char* group ) const ;				// モーションデータをグループ名から一括で解放する。モーションデータの名前は内部でModelSettingから取得する。
	void						ReleaseMotions() ;											// すべてのモーションデータの解放
	void						ReleaseExpressions() ;										// すべての表情データの解放

	D_ICubismModelSetting*		_modelSetting ;				// モデルセッティング情報
	D_csmStringW				_modelHomeDir ;				// モデルセッティングが置かれたディレクトリ
	float						_userTimeSeconds ;			// デルタ時間の積算値[秒]
	D_csmVector<D_CubismIdHandle> _eyeBlinkIds ;			// モデルに設定されたまばたき処理用パラメータID
	D_csmVector<D_CubismIdHandle> _lipSyncIds ;				// モデルに設定されたリップシンク処理用パラメータID
	D_csmMap<D_csmStringW, D_ACubismMotion*> _motions ;		// 読み込まれているモーションのリスト
	D_csmMap<D_csmStringW, D_ACubismMotion*> _expressions ;	// 読み込まれている表情のリスト
	D_csmVector<D_csmRectF>		_hitArea ;
	D_csmVector<D_csmRectF>		_userArea ;
	D_CubismId*					_idParamAngleX ;			// パラメータID: ParamAngleX
	D_CubismId*					_idParamAngleY ;			// パラメータID: ParamAngleX
	D_CubismId*					_idParamAngleZ ;			// パラメータID: ParamAngleX
	D_CubismId*					_idParamBodyAngleX ;		// パラメータID: ParamBodyAngleX
	D_CubismId*					_idParamEyeBallX ;			// パラメータID: ParamEyeBallX
	D_CubismId*					_idParamEyeBallY ;			// パラメータID: ParamEyeBallXY
	D_csmVector<int>			_bindTextureId ;			// テクスチャID 
	D_csmVector<D_LAppModelParameter>	_userParameters ;	// ユーザー設定のパラメータ
	bool						_deleteModel ;				// 実体消滅予定フラグ Drawを呼ばない 
	D_CubismOffscreenFrame_DxLib _renderBuffer ;			// フレームバッファ以外の描画先 
} ;

// テーブル-----------------------------------------------------------------------

extern int Live2D_VertexShaderToDxLibShader_Table[ 7 ] ;
extern int Live2D_PixelShaderToDxLibShader_Table[ 7 ] ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

extern D_CubismIdManager *	new_D_CubismIdManager( void ) ;
extern void					delete_D_CubismIdManager( D_CubismIdManager *obj ) ;
extern D_LAppModel *		new_D_LAppModel( void ) ;
extern void					delete_D_LAppModel( D_LAppModel *obj ) ;
extern D_CubismMatrix44 *	new_D_CubismMatrix44( void ) ;
extern void					delete_D_CubismMatrix44( D_CubismMatrix44 *obj ) ;

#endif // DX_NON_LIVE2D_CUBISM4

#endif // DX_USECLIB_LIVE2DCUBISM4_H

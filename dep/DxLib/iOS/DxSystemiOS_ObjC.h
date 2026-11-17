// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用システムプログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_SYSTEMIOS_OBJC_H
#define DX_SYSTEMIOS_OBJC_H

// インクルード ------------------------------------------------------------------

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

extern	int CheckGraphicsInitialize( void ) ;

// ビューフレームバッファの情報を取得する
extern	int GetGraphicsViewFramebufferInfo( unsigned int *ViewFrameBuffer, int *Width, int *Height ) ;

// カレントコンテキストをセットする
extern	void GraphicsSetCurrentContext( void ) ;

// ビューにレンダーバッファをアウトプットする
extern	void GraphicsViewRenderBufferPresent( void ) ;

// アプリケーションディレクトリを取得する
extern	const char *GetApplicationDirectory( void ) ;

// Homeディレクトリを取得する
extern	const char *GetHomeDirectory( void ) ;

// Tmpディレクトリを取得する
extern	const char *GetTmpDirectory( void ) ;

// フォント関係関数
extern	void * CreateDxiOSFontData( const char *FontName, int FontSize ) ;			// フォントデータの作成
extern	int DeleteDxiOSFontData( void *FontData ) ;									// フォントデータの削除
extern	void GetDxiOSFontBaseInfo( void *FontData, int *ImageWidth, int *ImageHeight, int *ImagePitch, float *FontAscent, float *FontDescent, float *FontLeading, float *FontUnderlinePosition, float *FontUnderlineThickness, float *FontCapHeight, float *FontXHeight ) ;	// フォントデータの基本情報を取得
extern	int SetupDxiOSFontImage( void *FontData, const char *Chara ) ;				// フォントデータを下に文字画像のセットアップ
extern	void * GetDxiOSFontImage( void *FontData, int *ImageDrawX, int *ImageDrawY, int *ImageSizeX, int *ImageSizeY, int *ImagePitch, int *ImageAddX ) ;			// 文字画像の情報を取得

// ゲームコントローラー関係関数
extern  void UpdateControllerInfo( int PadNo ) ;

// マイク許可処理完了確認
extern	int CheckRecordPermissionProcessEnd( void ) ;

// メールアプリ起動
extern	int	MailApp_Send_iOS( const char *MailAddr, const char *MailCCAddr, const char *MailBCCAddr, const char *Subject, const char *Text ) ;

// 自前ミキシング関係
extern int SelfMixingPlayer_Setup( void ) ;														// 自前ミキシング用のサウンド再生の準備を行う
extern int SelfMixingPlayer_Terminate( void ) ;													// 自前ミキシング用のサウンド再生の後始末を行う

// ジェスチャー無効化
extern int DisableGesture_iOS( void ) ;

#endif // DX_SYSTEMIOS_OBJC_H

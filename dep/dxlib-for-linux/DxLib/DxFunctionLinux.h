
#ifndef DX_FUNCTIONLINUX_H
#define DX_FUNCTIONLINUX_H

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

#define DX_FUNCTION_START


// DxWindow.cpp関数プロトタイプ宣言

//// ウインドウ関係情報取得関数
extern  int         GetWindowSize(                  int *Width, int *Height ) ;                             // メインウインドウのクライアント領域のサイズを取得する

//// 設定系関数
extern  int         SetMainWindowText(                      const TCHAR *WindowText                          ) ;                                // SetWindowText の別名関数

// ドラッグ＆ドロップされたファイル関係
extern  int         SetDragFileValidFlag(       int Flag ) ;                                                                    // ファイルのメインウインドウへのドラッグ＆ドロップ機能を有効にするかどうかのフラグをセットする
extern  int         DragFileInfoClear(          void ) ;                                                                        // メインウインドウへドラッグ＆ドロップされたファイルの情報をリセットする
extern  int         GetDragFilePath(            TCHAR *FilePathBuffer, int FilePathBufferBytes DEFAULTPARAM( = -1 ) ) ;         // メインウインドウへドラッグ＆ドロップされたファイル名を取得する( FilePathBuffer:ファイル名を格納するバッファの先頭アドレス　　戻り値　-1:取得できなかった  0:取得できた )
extern  int         GetDragFileNum(             void ) ;                                                                        // メインウインドウへドラッグ＆ドロップされたファイルの数を取得する

#undef DX_FUNCTION_START
#define DX_FUNCTION_END
#undef DX_FUNCTION_END


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_FUNCTIONLINUX_H


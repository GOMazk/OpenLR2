#define DX_MAKE

#include "DxWindowLinux.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE


extern int NS_GetWindowSize(int *Width, int *Height){
    *Width = GLINUX.Device.Screen.Width;
    *Height = GLINUX.Device.Screen.Height;
    return 0;
}

extern int NS_SetMainWindowText(const TCHAR *WindowText){
    Display* d = GLINUX.Device.Screen.XDisplay;
    Window w = GLINUX.Device.Screen.XWindow;
    // [本家非互換]ウィンドウが存在していない場合は反映されない
    if((!d)||(!w)){
        return -1;
    }
    Xutf8SetWMProperties(d, w, WindowText, NULL, NULL, 0, NULL, NULL, NULL);
    return 0;
}

extern int NS_SetDragFileValidFlag(int Flag){
    Display* d = GLINUX.Device.Screen.XDisplay;
    Window w = GLINUX.Device.Screen.XWindow;
    if((!d)||(!w)){
        return -1;
    }
    if(Flag){
        Atom xdndVer = 5;
        XChangeProperty(d, w, GLINUX.Device.Screen._atom_XdndAware, XA_ATOM, 32, PropModeReplace, (unsigned char*)&xdndVer, 1);
    }else{
        XDeleteProperty(d, w, GLINUX.Device.Screen._atom_XdndAware);
    }
    return 0;
}

extern int NS_DragFileInfoClear(){
    if(GLINUX.Device.Screen._xdnd_bufPtr){
        DXFREE(GLINUX.Device.Screen._xdnd_bufPtr);
        GLINUX.Device.Screen._xdnd_bufPtr = NULL;
    }
    GLINUX.Device.Screen._xdnd_bufLen = 0;
    GLINUX.Device.Screen._xdnd_bufPos = 0;
    GLINUX.Device.Screen._xdnd_bufNum = 0;
    return 0;
}

/*
内部挙動メモ

R: \r
N: \n
Z: \0

(2ファイル落とす)
_buf: [file://fooRNfile://hogeRN]
_len: 23

(1回目 - 読んだ所までposを移動)
pos:  [^                        ]0
buf:  [file://fooZ]
ret:  11

(2回目 - 末端に到達するとバッファをクリア)
pos:  [            ^            ]12
buf:  [file://hogeZ]
ret:  12
_buf: (null)
pos:  []0

(3回目 - もうファイルはない)
ret: -1

(バッファが短い場合を含む各挙動)
buf=abcdefghRN
GetDragFilePath(NULL)、バッファ空   -> ret: -1
GetDragFilePath(NULL)、バッファあり -> ret: 直後のパス長(含む\0: 10)
GetDragFilePath(path)、バッファ空   -> ret: -1
GetDragFilePath(path)、バッファあり -> ret: 0, path=abcdefghZ
GetDragFilePath(path, 5) -> ret: 0, path=abcdZ
*/
extern int NS_GetDragFilePath(TCHAR *FilePathBuffer, int FilePathBufferBytes){
    if(!GLINUX.Device.Screen._xdnd_bufPtr){
        return -1;
    }

    int nextPathLen = 0;
    for(int i=GLINUX.Device.Screen._xdnd_bufPos; i<GLINUX.Device.Screen._xdnd_bufLen; i++){
        if(GLINUX.Device.Screen._xdnd_bufPtr[i] == '\r'){ // to conclude i+1 is \n
            nextPathLen = i-GLINUX.Device.Screen._xdnd_bufPos; // without \0
            break;
        }
    }

    if(!FilePathBuffer){ // FilePathBuffer is null, return size (with \0) instead
        return nextPathLen + 1;
    }else{
        int nextRetPathLen = nextPathLen;
        if(nextPathLen >= FilePathBufferBytes){
            nextRetPathLen = FilePathBufferBytes-1;
        }
        _MEMCPY(FilePathBuffer, GLINUX.Device.Screen._xdnd_bufPtr+GLINUX.Device.Screen._xdnd_bufPos, nextRetPathLen);
        FilePathBuffer[nextPathLen] = '\0';
        if(GLINUX.Device.Screen._xdnd_bufLen == GLINUX.Device.Screen._xdnd_bufPos+nextPathLen+2){ // if last path of buf, free buf
            DXFREE(GLINUX.Device.Screen._xdnd_bufPtr);
            GLINUX.Device.Screen._xdnd_bufPtr = NULL;
            GLINUX.Device.Screen._xdnd_bufLen = 0;
            GLINUX.Device.Screen._xdnd_bufPos = 0;
        }else{
            GLINUX.Device.Screen._xdnd_bufPos += nextPathLen+2;
        }
        GLINUX.Device.Screen._xdnd_bufNum--;
    }
    return 0;
}

extern int NS_GetDragFileNum(){
    return GLINUX.Device.Screen._xdnd_bufNum;
}





#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


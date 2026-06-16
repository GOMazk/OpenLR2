#ifndef DX_WINDOW_LINUX_H
#define DX_WINDOW_LINUX_H

#include "DxGraphicsLinux.h"
#include "../DxBaseFunc.h"
#include "../DxMemory.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE


#define NS_GetWindowSize                        GetWindowSize
#define NS_SetMainWindowText                    SetMainWindowText
#define NS_SetDragFileValidFlag                 SetDragFileValidFlag
#define NS_DragFileInfoClear                    DragFileInfoClear
#define NS_GetDragFilePath                      GetDragFilePath
#define NS_GetDragFileNum                       GetDragFileNum


extern int GetWindowSize(int *Width, int *Height);
extern int SetMainWindowText(const TCHAR *WindowText);

extern int SetDragFileValidFlag(int Flag);
extern int DragFileInfoClear();
extern int GetDragFilePath(TCHAR *FilePathBuffer, int FilePathBufferBytes);
extern int GetDragFileNum();



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_WINDOW_LINUX_H

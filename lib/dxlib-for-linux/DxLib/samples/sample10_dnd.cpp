#include "DxLib.h"
#include <stdlib.h>

// for debugging
#ifdef __linux__
#include "Linux/DxGraphicsLinux.h"
#endif


void clearLog(){
    clsDx();
    printfDx("\
keys:\n\
  C: clear  |  E: enable DnD  |  D: disable DnD\n\
  R: enable DragFileInfoClear before process\n\
  F: disable DragFileInfoClear before process\n\
  T: enable skipping GetDragFilePath process\n\
  G: disable skipping GetDragFilePath process\n\
  Y: enable proceed a file in a second\n\
  H: disable proceed a file in a second\n\
  U: enable out buffer alloc upto 20 bytes\n\
  J: disable out buffer alloc upto 20 bytes\n\
drag and drop here...\n\
");
}

int main(){
#ifdef WINDOWS_DESKTOP_OS
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
    ChangeWindowMode(1);
    SetAlwaysRunFlag(1);
#endif
    //SetGraphMode(800, 600, 0, 0);
    if(DxLib_Init() == -1){
        return -1;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    auto c = GetColor(0xab, 0xcd, 0xef);
    int size = 0;
    bool isInfoClear = 0;
    bool isValid = 1;
    bool isSkip = 0;
    bool isSlow = 0;
    bool isShortBuf = 0;

    TCHAR* path = NULL;
    clearLog();
    SetDragFileValidFlag(1);
    for(int tick=0; ProcessMessage() != -1; tick++){
        ClearDrawScreen();

        switch(CheckHitKeyAll()){
        case KEY_INPUT_C: clearLog(); break;
        case KEY_INPUT_E: isValid = 1; SetDragFileValidFlag(1); break;
        case KEY_INPUT_D: isValid = 0; SetDragFileValidFlag(0); break;
        case KEY_INPUT_R: isInfoClear = 1; break;
        case KEY_INPUT_F: isInfoClear = 0; break;
        case KEY_INPUT_T: isSkip = 1; break;
        case KEY_INPUT_G: isSkip = 0; break;
        case KEY_INPUT_Y: isSlow = 1; break;
        case KEY_INPUT_H: isSlow = 0; break;
        case KEY_INPUT_U: isShortBuf = 1; break;
        case KEY_INPUT_J: isShortBuf = 0; break;
        default: break;
        }

        if(isInfoClear){
            DragFileInfoClear();
        }

        if(!isSkip && ((!isSlow) || (!(tick%60)))){
            for(int num = GetDragFileNum(); num;){
                printfDx("tick: %d, num: %d\n", tick, num);
                break;
            }

            while((size = GetDragFilePath(NULL)) != -1){
                printfDx("  size: %d\n", size);
                size = (isShortBuf&&size>20)?20:size;
                if(!(path = (TCHAR*)malloc(size))){ return -1; }
                printfDx("   ret: %d\n", GetDragFilePath(path, size));
                printfDx("  path: %s\n", path);
                free(path);
                if(isSlow){
                    break;
                }
            }
        }
        {
            // for debugging
            int l=0;
            #ifdef __linux__
            DrawFormatString(400, 20*l++, 0xabcdef, "ptr:%p", GLINUX.Device.Screen._xdnd_bufPtr);
            DrawFormatString(400, 20*l++, 0xabcdef, "len:%d", GLINUX.Device.Screen._xdnd_bufLen);
            DrawFormatString(400, 20*l++, 0xabcdef, "pos:%d", GLINUX.Device.Screen._xdnd_bufPos);
            DrawFormatString(400, 20*l++, 0xabcdef, "num:%d", GLINUX.Device.Screen._xdnd_bufNum);
            #endif
            DrawFormatString(400, 20*l++, 0xabcdef, "[E/D]valid:%d", isValid);
            DrawFormatString(400, 20*l++, 0xabcdef, "[R/F]clear:%d", isInfoClear);
            DrawFormatString(400, 20*l++, 0xabcdef, "[T/G]skip:%d", isSkip);
            DrawFormatString(400, 20*l++, 0xabcdef, "[Y/H]slow:%d", isSlow);
            DrawFormatString(400, 20*l++, 0xabcdef, "[U/J]short:%d", isShortBuf);
        }
        ScreenFlip();
    }
    DxLib_End();
    return 0;
}

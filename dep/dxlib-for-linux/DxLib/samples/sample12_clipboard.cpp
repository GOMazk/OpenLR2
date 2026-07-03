#include "DxLib.h"

// for debugging
#ifdef __linux__
#include "Linux/DxGraphicsLinux.h"
#endif

void clearLog(){
    clsDx();
    printfDx(_T("\
keys:\n\
  C: clear\n\
  1: SetClipboardText(\"Hello world!\")\n\
  2: SetClipboardText(\"あいうえお\")  // aiueo of hiragana\n\
  3: SetClipboardTextWithStrLen(\"Hello world!\", 5)\n\
  4: SetClipboardText(\"\")  // blank\n\
  5: GetClipboardText(NULL)\n\
  6: DxRealloc(str, len)\n\
  7: GetClipboardText(str, len)\n\
  8: [str(s)]\n\
  9: str(02x)\n\
  0: str(02x)(forced unsigned char)\n\
"));
}

int main(){
#ifdef WINDOWS_DESKTOP_OS
    //SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
    SetWindowSizeChangeEnableFlag(1, 0);
    ChangeWindowMode(1);
    SetAlwaysRunFlag(1);
    SetWindowMaximizeButtonBehavior(1);
#endif
    SetGraphMode(800, 600, 32);
    if(DxLib_Init() == -1){
        return -1;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    auto c = GetColor(0xab, 0xcd, 0xef);
    int currKey = 0;
    int prevKey = 0;
    clearLog();
    int len = 1;
    TCHAR* str = (TCHAR*)DxAlloc(len);
    unsigned char* str0 = (unsigned char*)str;
    while(ProcessMessage() != -1){
        ClearDrawScreen();

        currKey = CheckHitKeyAll();
        switch(currKey & (~prevKey)){
        case KEY_INPUT_C: clearLog(); break;
        case KEY_INPUT_1: printfDx(_T("1: %d\n"), SetClipboardText(_T("Hello world!"))); break;
        case KEY_INPUT_2: printfDx(_T("2: %d\n"), SetClipboardText(_T("あいうえお"))); break;
        case KEY_INPUT_3: printfDx(_T("3: %d\n"), SetClipboardTextWithStrLen(_T("Hello world!"), 5)); break;
        case KEY_INPUT_4: printfDx(_T("4: %d\n"), SetClipboardText(_T(""))); break;
        case KEY_INPUT_5: printfDx(_T("5: %d\n"), len = GetClipboardText(NULL, 0)); break;
        case KEY_INPUT_6: printfDx(_T("6: %p\n"), str = (TCHAR*)DxRealloc(str, len)); str0=(unsigned char*)str; break;
        case KEY_INPUT_7: printfDx(_T("7: %d\n"), GetClipboardText(str, len)); break;
        case KEY_INPUT_8: printfDx(_T("8: [%s]\n"), str); break;
        case KEY_INPUT_9: printfDx(_T("9: ")); for(int i=0; i<len/(sizeof(TCHAR)); i++){ printfDx(_T("%x "), str[i]); } printfDx(_T("\n"), str); break;
        case KEY_INPUT_0: printfDx(_T("0: ")); for(int i=0; i<len; i++){ printfDx(_T("%02x "), str0[i]); } printfDx(_T("\n"), str); break;
        default: break;
        }
        prevKey = currKey;
        {
            // for debugging
            int l=0;
            #ifdef __linux__
            DrawFormatString(400, 20*l++, 0xabcdef, _T("set ptr:%p"), GLINUX.Device.Screen._clipboard_set_bufPtr);
            DrawFormatString(400, 20*l++, 0xabcdef, _T("set len:%d"), GLINUX.Device.Screen._clipboard_set_bufLen);
            DrawFormatString(400, 20*l++, 0xabcdef, _T("get ptr:%p"), GLINUX.Device.Screen._clipboard_get_bufPtr);
            DrawFormatString(400, 20*l++, 0xabcdef, _T("get len:%d"), GLINUX.Device.Screen._clipboard_get_bufLen);
            #endif
        }
        ScreenFlip();
    }
    DxLib_End();
    return 0;
}

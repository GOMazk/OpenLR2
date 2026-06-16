#include "DxLib.h"


int main(){
#ifdef WINDOWS_DESKTOP_OS
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
    SetWindowSizeChangeEnableFlag(1, 0);
    ChangeWindowMode(1);
    SetAlwaysRunFlag(1);
    SetWindowMaximizeButtonBehavior(1);
#endif
    SetGraphMode(800, 600, 0, 0);
    if(DxLib_Init() == -1){
        return -1;
    }
    SetMainWindowText("DxLibあいう");
    SetDrawScreen(DX_SCREEN_BACK);
    int w=0, h=0;

    auto c = GetColor(0xab, 0xcd, 0xef);
    while(ProcessMessage() != -1){
        GetWindowSize(&w, &h);
        ClearDrawScreen();
        DrawFormatString(0, 20, c, "(%d, %d)", w, h);
        DrawCircle(0, 0, 20, c, 0);
        DrawCircle(w, 0, 20, c, 0);
        DrawCircle(0, h, 20, c, 0);
        DrawCircle(w, h, 20, c, 0);
        ScreenFlip();
    }
    DxLib_End();
    return 0;
}

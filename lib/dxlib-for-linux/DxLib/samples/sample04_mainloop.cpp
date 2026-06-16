#include "DxLib.h"
#include <math.h>

int main(){
#ifdef WINDOWS_DESKTOP_OS
    ChangeWindowMode(1);
#endif
    if(DxLib_Init() == -1){
        return -1;
    }
    SetDrawScreen(DX_SCREEN_BACK);
    int t=0;
    while(ProcessMessage() != -1){
        t++;
        ClearDrawScreen();
        DrawFormatString(0, 0, 0xcccccc, "t: %d", t);
        DrawCircle(120, 120, 100, 0xabcdef, 0);
        DrawLine(120, 120, 120+100*sin(0.01*t), 120+100*cos(0.01*t), 0xabcdef);
        ScreenFlip();
    }
    DxLib_End();
    return 0;
}

#include "DxLib.h"

#define F(s) y+=20; DrawFormatString(x+100, y, c, "%d = " #s, s);

int main(){
#ifdef WINDOWS_DESKTOP_OS
    ChangeWindowMode(1);
#endif
    if(DxLib_Init() == -1){
        return -1;
    }
    SetDrawScreen(DX_SCREEN_BACK);
    int x=0, y=0;
    int mx=0, my=0;
    auto c = GetColor(0xab, 0xcd, 0xef);
    while(ProcessMessage() != -1){
        x=0; y=0;
        ClearDrawScreen();
        F(DrawLine(x+20, y+20, x+80, y, c));
        F(DrawLineAA(x+20, y+20, x+80, y, c));
        F(DrawBox(x+20, y+2, x+80, y+18, c, 0));
        F(DrawBox(x+20, y+2, x+80, y+18, c, 1));
        F(DrawBoxAA(x+20.3, y+2.3, x+80.3, y+18.3, c, 0));
        F(DrawBoxAA(x+20.3, y+2.3, x+80.3, y+18.3, c, 1));
        F(DrawCircle(x+30, y+10, 16, c, 0));
        F(DrawCircle(x+70, y+10, 16, c, 1));
        F(DrawCircleAA(x+30, y+10, 16, 32, c, 0));
        F(DrawCircleAA(x+70, y+10, 16, 32, c, 1));
        F(DrawOval(x+50, y+10, 30, 8, c, 0));
        F(DrawOval(x+50, y+10, 30, 8, c, 1));
        F(DrawOvalAA(x+50, y+10, 30, 8, 32, c, 0));
        F(DrawOvalAA(x+50, y+10, 30, 8, 32, c, 1));
        F(DrawTriangle(x+20, y+20, x+40, y, x+80, y+10, c, 0)); // probably not work
        F(DrawTriangle(x+20, y+20, x+80, y+10, x+40, y, c, 0)); // probably not work
        F(DrawTriangle(x+20, y+20, x+40, y, x+80, y+10, c, 1));
        F(DrawTriangleAA(x+20, y+20, x+40, y, x+80, y+10, c, 0));
        F(DrawTriangleAA(x+20, y+20, x+40, y, x+80, y+10, c, 1));
        F(DrawPixel(x+50, y+10, c));
        //y+=20; DrawFormatString(20, y, c, "%08x = GetPixel(x+50, y-10)", GetPixel(x+50, y-10));
        GetMousePoint(&mx, &my);
        y+=20; DrawFormatString(20, y, c, "%08x = GetPixel(%d, %d)", GetPixel(mx, my), mx, my);
        ScreenFlip();
    }
    DxLib_End();
    return 0;
}

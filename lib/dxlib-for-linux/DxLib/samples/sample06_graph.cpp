#include "DxLib.h"

#define F(s) s; DrawString(x, y, #s, c); y+=40;

int main(){
#ifdef WINDOWS_DESKTOP_OS
    ChangeWindowMode(1);
#endif
    if(DxLib_Init() == -1){
        return -1;
    }
    SetDrawScreen(DX_SCREEN_BACK);
    auto p1 = "assets/bbball.png";
    auto p2 = "assets/bbball_tiled.png";
    const auto g2n = 20;
    auto g1 = LoadGraph(p1);
    int g2s[g2n];
    LoadDivGraph(p2, 20, 5, 4, 100, 100, g2s);
    auto g3 = DerivationGraph(30, 30, 40, 40, g1);
    int t=0;
    int x=0, y=0;
    auto c = GetColor(0xab, 0xcd, 0xef);
    while(ProcessMessage() != -1){
        t++;
        x=50; y=20;
        ClearDrawScreen();
        DrawBox(40, 0, 200, 480, 0x003300, 1);
        DrawFormatString(0, 0, c, "t: %d", t);
        F(DrawGraph(x, y, g3, 1));
        F(DrawTurnGraph(x, y, g3, 0));
        F(DrawExtendGraph(x, y, x+200, y+20, g3, 1));
        F(DrawRotaGraph(x, y, 1.2, t*0.01, g3, 1, 0));
        F(DrawRotaGraph2(x, y, 15, 15, 1.2, t*0.01, g3, 1, 1));
        F(DrawRotaGraph3(x, y, 15, 15, 1.4, 0.8, t*0.01, g3, 0, 0));
        F(DrawModiGraph(x, y, x+30, y, x+60, y+30, x+30, y+30, g3, 1));
        F(DrawRectGraph(x, y, 40, 40, 20, 20, g1, 1, 0));
        F(DrawGraph(x, y, g2s[(t/4)%g2n], 1));
        ScreenFlip();
    }
    DxLib_End();
    return 0;
}

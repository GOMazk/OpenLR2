#include "DxLib.h"

int main(){
#ifdef WINDOWS_DESKTOP_OS
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
    ChangeWindowMode(1);
    SetAlwaysRunFlag(1);
#endif
    if(DxLib_Init() == -1){
        return -1;
    }
    DrawString(0, 0, "Default: abcあいう漢字", 0x00ff00);
    int fh = LoadFontDataToHandle("assets/es1.dft", 0);
    // es1.dftは「漢」のグリフを持たないので描画されない
    DrawStringToHandle(0, 20, "Loaded: abcあいう漢字", 0x00ff00, fh);
    while(ProcessMessage() != -1){}
    DxLib_End();
    return 0;
}

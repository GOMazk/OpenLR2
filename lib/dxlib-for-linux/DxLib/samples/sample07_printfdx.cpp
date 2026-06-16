#include "DxLib.h"

int main(){
#ifdef WINDOWS_DESKTOP_OS
    ChangeWindowMode(1);
#endif
    if(DxLib_Init() == -1){
        return -1;
    }
    printfDx("should not show it");
    clsDx();
    printfDx("abcあいうdef012");
    WaitKey();
    DxLib_End();
    return 0;
}


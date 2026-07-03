#include "DxLib.h"

int main(){
#ifdef WINDOWS_DESKTOP_OS
    ChangeWindowMode(1);
#endif
    if(DxLib_Init() == -1){
        return -1;
    }
    SetUseZBuffer3D(1);
    SetWriteZBuffer3D(1);
    DrawSphere3D(VGet(320, 200, 0), 80, 32, 0xfedcba, 0xffffff, 1);
    WaitKey();
    DxLib_End();
    return 0;
}

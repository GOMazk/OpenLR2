#include "DxLib.h"

int main(){
#ifdef WINDOWS_DESKTOP_OS
    ChangeWindowMode(1);
#endif
    if(DxLib_Init() == -1){
        return -1;
    }
#ifdef DX_NON_SOUND
    DrawString(0, 0, "DX_NON_SOUND is defined, do nothing", 0xffffff);
#else
    DrawString(0, 0, "Playing sound...", 0xffffff);
    PlaySoundFile("assets/noise.ogg", DX_PLAYTYPE_LOOP);
#endif
    WaitKey();
    DxLib_End();
    return 0;
}

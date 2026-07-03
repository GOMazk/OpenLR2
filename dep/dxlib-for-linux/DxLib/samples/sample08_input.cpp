#include "DxLib.h"
#include <math.h>

const char* ReverseKeyMap(int KeyCode){
    switch( KeyCode )
    {
    case 0x0E : return "KEY_INPUT_BACK" ;
    case 0x0F : return "KEY_INPUT_TAB" ;
    case 0x1C : return "KEY_INPUT_RETURN" ;
    case 0x2A : return "KEY_INPUT_LSHIFT" ;
    case 0x36 : return "KEY_INPUT_RSHIFT" ;
    case 0x1D : return "KEY_INPUT_LCONTROL" ;
    case 0x9D : return "KEY_INPUT_RCONTROL" ;
    case 0x01 : return "KEY_INPUT_ESCAPE" ;
    case 0x39 : return "KEY_INPUT_SPACE" ;
    case 0xC9 : return "KEY_INPUT_PGUP" ;
    case 0xD1 : return "KEY_INPUT_PGDN" ;
    case 0xCF : return "KEY_INPUT_END" ;
    case 0xC7 : return "KEY_INPUT_HOME" ;
    case 0xCB : return "KEY_INPUT_LEFT" ;
    case 0xC8 : return "KEY_INPUT_UP" ;
    case 0xCD : return "KEY_INPUT_RIGHT" ;
    case 0xD0 : return "KEY_INPUT_DOWN" ;
    case 0xD2 : return "KEY_INPUT_INSERT" ;
    case 0xD3 : return "KEY_INPUT_DELETE" ;
    case 0x0C : return "KEY_INPUT_MINUS" ;
    case 0x7D : return "KEY_INPUT_YEN" ;
    case 0x90 : return "KEY_INPUT_PREVTRACK" ;
    case 0x34 : return "KEY_INPUT_PERIOD" ;
    case 0x35 : return "KEY_INPUT_SLASH" ;
    case 0x38 : return "KEY_INPUT_LALT" ;
    case 0xB8 : return "KEY_INPUT_RALT" ;
    case 0x46 : return "KEY_INPUT_SCROLL" ;
    case 0x27 : return "KEY_INPUT_SEMICOLON" ;
    case 0x92 : return "KEY_INPUT_COLON" ;
    case 0x1A : return "KEY_INPUT_LBRACKET" ;
    case 0x1B : return "KEY_INPUT_RBRACKET" ;
    case 0x91 : return "KEY_INPUT_AT" ;
    case 0x2B : return "KEY_INPUT_BACKSLASH" ;
    case 0x33 : return "KEY_INPUT_COMMA" ;
    case 0x94 : return "KEY_INPUT_KANJI" ;
    case 0x79 : return "KEY_INPUT_CONVERT" ;
    case 0x7B : return "KEY_INPUT_NOCONVERT" ;
    case 0x70 : return "KEY_INPUT_KANA" ;
    case 0xDD : return "KEY_INPUT_APPS" ;
    case 0x3A : return "KEY_INPUT_CAPSLOCK" ;
    case 0xB7 : return "KEY_INPUT_SYSRQ" ;
    case 0xC5 : return "KEY_INPUT_PAUSE" ;
    case 0xDB : return "KEY_INPUT_LWIN" ;
    case 0xDC : return "KEY_INPUT_RWIN" ;
    case 0x45 : return "KEY_INPUT_NUMLOCK" ;
    case 0x52 : return "KEY_INPUT_NUMPAD0" ;
    case 0x4F : return "KEY_INPUT_NUMPAD1" ;
    case 0x50 : return "KEY_INPUT_NUMPAD2" ;
    case 0x51 : return "KEY_INPUT_NUMPAD3" ;
    case 0x4B : return "KEY_INPUT_NUMPAD4" ;
    case 0x4C : return "KEY_INPUT_NUMPAD5" ;
    case 0x4D : return "KEY_INPUT_NUMPAD6" ;
    case 0x47 : return "KEY_INPUT_NUMPAD7" ;
    case 0x48 : return "KEY_INPUT_NUMPAD8" ;
    case 0x49 : return "KEY_INPUT_NUMPAD9" ;
    case 0x37 : return "KEY_INPUT_MULTIPLY" ;
    case 0x4E : return "KEY_INPUT_ADD" ;
    case 0x4A : return "KEY_INPUT_SUBTRACT" ;
    case 0x53 : return "KEY_INPUT_DECIMAL" ;
    case 0xB5 : return "KEY_INPUT_DIVIDE" ;
    case 0x9C : return "KEY_INPUT_NUMPADENTER" ;
    case 0x3B : return "KEY_INPUT_F1" ;
    case 0x3C : return "KEY_INPUT_F2" ;
    case 0x3D : return "KEY_INPUT_F3" ;
    case 0x3E : return "KEY_INPUT_F4" ;
    case 0x3F : return "KEY_INPUT_F5" ;
    case 0x40 : return "KEY_INPUT_F6" ;
    case 0x41 : return "KEY_INPUT_F7" ;
    case 0x42 : return "KEY_INPUT_F8" ;
    case 0x43 : return "KEY_INPUT_F9" ;
    case 0x44 : return "KEY_INPUT_F10" ;
    case 0x57 : return "KEY_INPUT_F11" ;
    case 0x58 : return "KEY_INPUT_F12" ;
    case 0x1E : return "KEY_INPUT_A" ;
    case 0x30 : return "KEY_INPUT_B" ;
    case 0x2E : return "KEY_INPUT_C" ;
    case 0x20 : return "KEY_INPUT_D" ;
    case 0x12 : return "KEY_INPUT_E" ;
    case 0x21 : return "KEY_INPUT_F" ;
    case 0x22 : return "KEY_INPUT_G" ;
    case 0x23 : return "KEY_INPUT_H" ;
    case 0x17 : return "KEY_INPUT_I" ;
    case 0x24 : return "KEY_INPUT_J" ;
    case 0x25 : return "KEY_INPUT_K" ;
    case 0x26 : return "KEY_INPUT_L" ;
    case 0x32 : return "KEY_INPUT_M" ;
    case 0x31 : return "KEY_INPUT_N" ;
    case 0x18 : return "KEY_INPUT_O" ;
    case 0x19 : return "KEY_INPUT_P" ;
    case 0x10 : return "KEY_INPUT_Q" ;
    case 0x13 : return "KEY_INPUT_R" ;
    case 0x1F : return "KEY_INPUT_S" ;
    case 0x14 : return "KEY_INPUT_T" ;
    case 0x16 : return "KEY_INPUT_U" ;
    case 0x2F : return "KEY_INPUT_V" ;
    case 0x11 : return "KEY_INPUT_W" ;
    case 0x2D : return "KEY_INPUT_X" ;
    case 0x15 : return "KEY_INPUT_Y" ;
    case 0x2C : return "KEY_INPUT_Z" ;
    case 0x0B : return "KEY_INPUT_0" ;
    case 0x02 : return "KEY_INPUT_1" ;
    case 0x03 : return "KEY_INPUT_2" ;
    case 0x04 : return "KEY_INPUT_3" ;
    case 0x05 : return "KEY_INPUT_4" ;
    case 0x06 : return "KEY_INPUT_5" ;
    case 0x07 : return "KEY_INPUT_6" ;
    case 0x08 : return "KEY_INPUT_7" ;
    case 0x09 : return "KEY_INPUT_8" ;
    case 0x0A : return "KEY_INPUT_9" ;
    default : return "default" ;
    }
}


int main(){
#ifdef WINDOWS_DESKTOP_OS
    ChangeWindowMode(1);
#endif
    //SetGraphMode(800, 600, 32);
    if(DxLib_Init() == -1){
        return -1;
    }
    SetDrawScreen(DX_SCREEN_BACK);
    int t=0;
    int mx=0, my=0;
    while(ProcessMessage() != -1){
        mx=0; my=0;
        ClearDrawScreen();
        DrawFormatString(mx, 20*(my++), 0xcccccc, "t: %d", t);
        DrawFormatString(mx, 20*(my++), 0xcccccc, "*** Joypad ***");
        DrawFormatString(mx, 20*(my++), 0x333333, "wai     GetJoypadNum()");
        DrawFormatString(mx, 20*(my++), 0x333333, "wai     GetJoypadInputState()");
        DrawFormatString(mx, 20*(my++), 0x333333, "wai     GetJoypadAnalogInput()");
        DrawFormatString(mx, 20*(my++), 0x333333, "wai     GetJoypadDirectInputState()");
        DrawFormatString(mx, 20*(my++), 0x333333, "w       GetJoypadXInputState()");
        DrawFormatString(mx, 20*(my++), 0x333333, "wai     GetJoypadDeadZone()");
        DrawFormatString(mx, 20*(my++), 0x333333, "w       StartJoypadVibration()");
        DrawFormatString(mx, 20*(my++), 0x333333, "w       StopJoypadVibration()");
        DrawFormatString(mx, 20*(my++), 0xcccccc, "*** Mouse ***");
        DrawFormatString(mx, 20*(my++), 0x333333, "w       SetMouseDispFlag()");
        int gmpr, gmpx, gmpy;
        gmpr = GetMousePoint(&gmpx, &gmpy);
        DrawFormatString(mx, 20*(my++), 0xcccccc, "wa  %d = GetMousePoint(%d, %d)", gmpr, gmpx, gmpy);
        DrawFormatString(mx, 20*(my++), 0x333333, "w       SetMousePoint()");
        int gmir = GetMouseInput();
        DrawFormatString(mx, 20*(my++), 0xcccccc, "wa  %d = GetMouseInput()", gmir);
        DrawFormatString(mx, 20*(my++), 0x333333, "wa      GetMouseInputLog2()");
        int gmwrvr = GetMouseWheelRotVol();
        DrawFormatString(mx, 20*(my++), 0xcccccc, "wa  %d = GetMouseWheelRotVol()", gmwrvr);
        int gmhwrvr = GetMouseHWheelRotVol();
        DrawFormatString(mx, 20*(my++), 0xcccccc, "    %d = GetMouseHWheelRotVol()", gmhwrvr);
        DrawFormatString(mx, 20*(my++), 0xcccccc, "*** Touch ***");
        DrawFormatString(mx, 20*(my++), 0x333333, "wai     GetTouchInputNum()");
        DrawFormatString(mx, 20*(my++), 0x333333, "wai     GetTouchInput()");
        mx=300; my=0;
        DrawFormatString(mx, 20*(my++), 0xcccccc, "*** Keyboard ***");
        int chkakr = CheckHitKeyAll(DX_CHECKINPUT_KEY);
        int chkapr = CheckHitKeyAll(DX_CHECKINPUT_PAD);
        int chkamr = CheckHitKeyAll(DX_CHECKINPUT_MOUSE);
        int chkaar = CheckHitKeyAll(DX_CHECKINPUT_ALL);
        DrawFormatString(mx, 20*(my++), 0xcccccc, "wa  %d = CheckHitKeyAll(DX_CHECKINPUT_KEY)", chkakr);
        DrawFormatString(mx, 20*(my++), 0xcccccc, "wa  %d = CheckHitKeyAll(DX_CHECKINPUT_PAD)", chkapr);
        DrawFormatString(mx, 20*(my++), 0xcccccc, "wa  %d = CheckHitKeyAll(DX_CHECKINPUT_MOUSE)", chkamr);
        DrawFormatString(mx, 20*(my++), 0xcccccc, "wa  %d = CheckHitKeyAll(DX_CHECKINPUT_ALL)", chkaar);
        int chkr = CheckHitKey(KEY_INPUT_A);
        DrawFormatString(mx, 20*(my++), 0xcccccc, "wa  %d = CheckHitKey(KEY_INPUT_A)", chkr);
        char ghksab[256];
        int ghksar = GetHitKeyStateAll(ghksab);
        DrawFormatString(mx, 20*(my++), 0xcccccc, "wa  %d = GetHitKeyStateAll(...)", ghksar);
        for(int ghksal = 0; ghksal < 256; ghksal++){
            DrawPixel((ghksal%32)*2+mx, (ghksal/32)*2+20*my, ghksab[ghksal] ? 0xffffff : 0x111111);
        }
        my++;
        DrawFormatString(mx, 20*(my++), 0xcccccc, "*** Char ***");
        DrawFormatString(mx, 20*(my++), 0x333333, "w       GetInputChar()");
        DrawFormatString(mx, 20*(my++), 0x333333, "w       GetInputCharWait()");
        DrawFormatString(mx, 20*(my++), 0x333333, "w       ClearInputCharBuf()");
        my++;
        DrawFormatString(mx, 20*(my++), 0x333333, "pressing keys:");
        for(int j=0; j<256; j++){
            if(ghksab[j]){
                DrawFormatString(mx, 20*(my++), 0xcccccc, "[%s]", ReverseKeyMap(j));
            }
        }
        ScreenFlip();
        t++;
    }
    DxLib_End();
    return 0;
}

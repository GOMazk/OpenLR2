#include "DxLib.h"

int main(){
    if(DxLib_Init() == -1){
        return -1;
    }
    DxLib_End();
    return 0;
}

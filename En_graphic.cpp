#include "En_graphic.h"
#include "DxLib/DxLib.h"

//49e780
int ReloadImage(CSTR filename, int *grHandle) {
	return ReloadGraph(filename, *grHandle, 0);
}

//49ed30
int ScreenCapture(uint iGrHandle){
	GetDrawScreenGraph(0, 0, 640, 480, iGrHandle, 1);
	return 0;
}

int hBackImage;
//4a9ff0
int SetBackground(int hImage) {
	hBackImage = hImage;
	return 1;
}


#include "En_graphic.h"
#include <DxLib.h>

#include "En_dxlibstub.h"

int ScreenCapture(uint iGrHandle, int x, int y){
	GetDrawScreenGraph(0, 0, x, y, iGrHandle, 1);
	return 0;
}

int hBackImage;
int SetBackground(int hImage) {
	hBackImage = hImage;
	return 1;
}

int screenSizeX, screenSizeY;
int skinSizeX, skinSizeY;

// Maps the global resolution counter (config.system.resolution) to pixels.
void GetConfigResolution(int counter, int* outX, int* outY) {
	switch (counter) {
		case 1:  *outX = 1280; *outY = 720;  break;  // HD
		case 2:  *outX = 1920; *outY = 1080; break;  // UHD (experimental)
		default: *outX = 640;  *outY = 480;  break;  // SD
	}
}

int Resize(game* g, double skinX, double skinY, bool bit16) {
	int oldXpos = 320, oldYpos = 240;

	GetWindowPosition(&oldXpos, &oldYpos);
	SetGraphMode(skinX, skinY, bit16? 16 : 32, GetRefreshRate());
	SetWindowSizeExtendRate((double)g->config.system.windowsize_x / skinX, (double)g->config.system.windowsize_y / skinY);
	SetDrawScreen(DX_SCREEN_BACK);
	SetWindowPosition(oldXpos, oldYpos);

	skinSizeX = skinX;
	skinSizeY = skinY;
	return 0;
}

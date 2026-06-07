#include "LR2_skindraw.h"
#include "Engine.h"
#include "LR2_skinobject.h"
#include "LR2_skinload.h"

#ifndef _WIN32
#include "En_dxlibstub.h"
#endif // _WIN32

int InitDrawingBuffer(DrawingBuf *drb){
	for (int i = 0; i < drb->count; i++) {
		InitDSTdraw(&drb->dstd[i]);
	}
	drb->count = 0;
	for (int i = 0; i < 20; i++) {
		drb->top[i] = -1;
		drb->bottom[i] = -1;
		drb->isHidSud[i] = -1;
	}
	return 1;
}

int AllocDrawingBuffer(DrawingBuf *drb){
	drb->max = 2000;
	drb->count = 0;
	for (int i = 0; i < 20; i++) {
		drb->isHidSud[i] = -1;
		drb->top[i] = -1;
		drb->bottom[i] = -1;
	}
	drb->dstd = (DSTdraw *)malloc(drb->max * sizeof(DSTdraw));
	if (drb->dstd == (DSTdraw *)0x0) {
		ErrorLogAdd("描画用バッファのメモリ取得に失敗しました。\n");
		return -1;
	}
	return 1;
}

int ReallocDrawingBuffer(DrawingBuf *drb){
	drb->max = drb->max + 1000;
	drb->dstd = (DSTdraw *)realloc(drb->dstd, drb->max * sizeof(DSTdraw));
	assert(drb->dstd != nullptr);
	if (drb->dstd == (DSTdraw *)0x0) {
		ErrorLogAdd("描画用バッファのメモリ再取得に失敗しました。\n");
		return -1;
	}
	return 1;
}

int InitDSTdraw(DSTdraw *dstd){
	dstd->x = 0;
	dstd->y = 0;
	dstd->w = 0;
	dstd->sortID = 0;
	dstd->h = 0;
	dstd->a = 0;
	dstd->angle = 0.0;
	dstd->r = 0;
	dstd->g = 0;
	dstd->b = 0;
	dstd->acc = 0;
	dstd->blend = 0;
	dstd->filter = 0;
	dstd->time = -1;
	dstd->center = 0;
	dstd->grHandle = -1;
	dstd->fontHandle = -1;
	dstd->subHandle = -1;
	dstd->align = 0;
	dstd->isDrawBackbox = '\0';
	return 1;
}

DSTdraw DSTDbyTime(DSTdraw *dstd1, DSTdraw *dstd2, double t1, double t2, double tO) {
	DSTdraw ret;
	
	ret.x = ByTime(dstd1->x, dstd2->x, t1, t2, tO);
	ret.y = ByTime(dstd1->y, dstd2->y, t1, t2, tO);
	ret.w = ByTime(dstd1->w, dstd2->w, t1, t2, tO);
	ret.h = ByTime(dstd1->h, dstd2->h, t1, t2, tO);
	ret.sortID = ByTime(dstd1->sortID, dstd2->sortID, t1, t2, tO);
	ret.a = ByTime(dstd1->a, dstd2->a, t1, t2, tO);
	ret.r = ByTime(dstd1->r, dstd2->r, t1, t2, tO);
	ret.g = ByTime(dstd1->g, dstd2->g, t1, t2, tO);
	ret.b = ByTime(dstd1->b, dstd2->b, t1, t2, tO);
	ret.angle = ByTime(dstd1->angle, dstd2->angle, t1, t2, tO);

	ret.blend = dstd1->blend;
	ret.filter = dstd1->filter;
	ret.time = 0;
	ret.center = dstd1->center;
	ret.isDrawBackbox = dstd1->isDrawBackbox; //DEBUG: not in original code
	return ret;
}

DSTdraw SetDSTdrawByTime(DSTstruct dst, double time) {
	int tStart, tEnd;
	int t = time, t2;
	DSTdraw oBuf;

	DSTdraw *rDstd;
	int select;

	InitDSTdraw(&oBuf);
	if (dst.dstCount > 0 && dst.dataSize > 0) {
		tStart = dst.draw[0].time;
		tEnd = dst.draw[dst.dstCount -1].time;

		t2 = tEnd;
		if (tStart <= tEnd && tStart <= t && (0 <= dst.loop || t <= tEnd)) {
			if (tStart == tEnd || dst.loop == tEnd) {
				if (t < t2) {
					t2 = t;
				}
			}
			else if (dst.loop < tEnd) {
				t2 = t;
				if (tEnd < t) {
					t2 = (t - dst.loop) % (tEnd - dst.loop) + dst.loop;
				}
			}
			else {
				t2 = 0;
			}

			select = 0;
			for (int i = 0; i < dst.dstCount; i++) {
				if (dst.draw[i].time <= t2) {
					select = i;
				}
			}
			rDstd = &dst.draw[select];

			if (t2 != rDstd->time && select != dst.dstCount - 1 ) {
				oBuf.x = ChangeValueByTime((double)rDstd->x, (double)rDstd[1].x, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.y = ChangeValueByTime((double)rDstd->y, (double)rDstd[1].y, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.w = ChangeValueByTime((double)rDstd->w, (double)rDstd[1].w, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.h = ChangeValueByTime((double)rDstd->h, (double)rDstd[1].h, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.sortID = ChangeValueByTime((double)rDstd->sortID, (double)rDstd[1].sortID, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.a = ChangeValueByTime((double)rDstd->a, (double)rDstd[1].a, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.r = ChangeValueByTime((double)rDstd->r, (double)rDstd[1].r, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.g = ChangeValueByTime((double)rDstd->g, (double)rDstd[1].g, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.b = ChangeValueByTime((double)rDstd->b, (double)rDstd[1].b, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.angle = ChangeValueByTime((double)rDstd->angle, (double)rDstd[1].angle, (double)rDstd->time, (double)rDstd[1].time, (double)t2, rDstd->acc);
				oBuf.blend = rDstd->blend;
				oBuf.center = rDstd->center;
				oBuf.filter = rDstd->filter;
				oBuf.time = 0;
				return oBuf;
			}
			oBuf.x = rDstd->x;
			oBuf.y = rDstd->y;
			oBuf.w = rDstd->w;
			oBuf.h = rDstd->h;
			oBuf.a = rDstd->a;
			oBuf.r = rDstd->r;
			oBuf.g = rDstd->g;
			oBuf.b = rDstd->b;
			oBuf.sortID = rDstd->sortID;
			oBuf.angle = rDstd->angle;
			oBuf.blend = rDstd->blend;
			oBuf.filter = rDstd->filter;
			oBuf.center = rDstd->center;
			oBuf.time = 0;
			return oBuf;
		}
	}
	oBuf.time = -1;
	return oBuf;
}


int GetSRCcycleNow(SRCstruct src, double time){
	int ret;

	if (time < 0.0) return 0;
	if (src.graphcount <= 0) return 0;
	
	if (src.cycle < 0) ret = 0;
	else if (src.cycle == 0) ret = 0;
	else ret = ((int)time % src.cycle) * src.graphcount / src.cycle;

	if (src.graphcount <= ret || ret < 0) ret = 0;

	return ret;
}

int AddDrawingBufferT(DrawingBuf *drb, int grHandle, DSTdraw *dstd){
	if ((drb->isDisabled == '\0') && (grHandle != -1)) {
		dstd->fontHandle = grHandle;
		dstd->grHandle = -1;
		if (drb->count < 2000) {
			memcpy(&drb->dstd[drb->count], dstd, sizeof(DSTdraw));
			drb->count = drb->count + 1;
			return 1;
		}
		return 0;
	}
	return 0;
}

int AddDrawingBuffer_Text(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T) {
	DSTdraw tDstd;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.fontHandle = src->fontHandle;
	if (tDstd.fontHandle == -1) return 0;
	tDstd.angle = src->cycle;
	tDstd.subHandle = src->st;
	tDstd.align = src->align;
	if (tDstd.time != -1) AddDrawingBufferT(drb, tDstd.fontHandle, &tDstd);
	return 1;
}

int AddDrawingBuffer_TextXY(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int handle, int x, int y) {
	DSTdraw tDstd;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.fontHandle = src->fontHandle;
	if (tDstd.fontHandle == -1) return 0;
	tDstd.angle = src->cycle;
	tDstd.subHandle = handle;
	tDstd.align = src->align;
	tDstd.x += x;
	tDstd.y += y;
	if (tDstd.time != -1) AddDrawingBufferT(drb, tDstd.fontHandle, &tDstd);
	return 1;
}


int AddDrawingBuffer(DrawingBuf *drb, int grHandle, DSTdraw *dstd){
	if ((drb->isDisabled == '\0') && (grHandle != -1)) {
		dstd->grHandle = grHandle;
		dstd->fontHandle = -1;
		if (drb->count == drb->max) {
			ReallocDrawingBuffer(drb);
		}
		memcpy(&drb->dstd[drb->count], dstd, sizeof(DSTdraw));
		drb->count = drb->count + 1;
		return 1;
	}
	return 0;
}

int AddDrawingBuffer_BarGraph(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, double min, double max, double val) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0 || src->graphcount <= 0) return 0;

	grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (min == max || val <= min) return -1;
	if (val >= max) val = max;
	val = val / (max - min);
	if (src->op2 == 0) tDstd.w *= val;
	else if (src->op2 == 1) tDstd.h *= val;
	if (tDstd.time != -1 && grh != -1) {
		AddDrawingBuffer(drb, grh, &tDstd);
	}
	return 0;
}

int AddDrawingBuffer_Image(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T) {
	DSTdraw tDstd;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (tDstd.time != -1 && src->inArray < src->graphcount && src->inArray > -1) {
		AddDrawingBuffer(drb, src->grHandles[src->inArray], &tDstd);
	}

	return 1;
}

int AddDrawingBuffer_OnMouse(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, inputStructure *input, int panel) {
	DSTdraw tDstd;
	int grh;
	
	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	grh = GetSRCcycleNow(*src, GetTimeLapse(src->timer, T));
	tDstd.x += src->op2;
	tDstd.y += src->op3;
	tDstd.w = src->op4;
	tDstd.h = src->op5;

	if (MouseOnDSTD(&tDstd, &input->mouse_oldX, &input->mouse_oldY) && (src->op1 == 0 || src->op1 == panel)) {
		tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
		if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, src->grHandles[grh], &tDstd);
	}
	return 0;
}

int AddDrawingBuffer_LN(DrawingBuf *drb, SRCstruct *srcLs, SRCstruct *srcLe, SRCstruct *srcLb, DSTstruct *dst, Timer *T, float shiftX, float shiftY, float longY, float sizeX, float sizeY, int active) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0 || srcLs->graphcount <= 0 || srcLe->graphcount <= 0 || srcLb->graphcount <= 0) return 0;

	int alpha = active != -1 ? 255 : 128;

	//body
	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.w += sizeX;
	tDstd.h += sizeY;
	tDstd.x -= sizeX * 0.5;
	tDstd.y -= sizeY * 0.5;
	if (active == 0) {
		grh = srcLb->grHandles[0];
	}
	else if (srcLb->timer == dst->timer) {
		grh = srcLb->grHandles[GetSRCcycleNow(*srcLb, GetTimeLapse(srcLb->timer, T) - dst->draw->time)];
	}
	else {
		grh = srcLb->grHandles[GetSRCcycleNow(*srcLb, GetTimeLapse(srcLb->timer, T))];
	}
	tDstd.x += shiftX;
	tDstd.subHandle = dst->n;
	tDstd.blend = 1;
	tDstd.a = alpha;
	tDstd.y += longY + tDstd.h;
	tDstd.h = shiftY - longY - tDstd.h;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	//start
	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.w += sizeX;
	tDstd.h += sizeY;
	tDstd.x -= sizeX * 0.5;
	tDstd.y -= sizeY * 0.5;
	tDstd.sortID += 2;
	if (active == 0) {
		grh = srcLs->grHandles[0];
	}
	else if (srcLs->timer == dst->timer) {
		grh = srcLs->grHandles[GetSRCcycleNow(*srcLs, GetTimeLapse(srcLs->timer, T) - dst->draw->time)];
	}
	else {
		grh = srcLs->grHandles[GetSRCcycleNow(*srcLs, GetTimeLapse(srcLs->timer, T))];
	}
	tDstd.x += shiftX;
	tDstd.subHandle = dst->n;
	tDstd.blend = 1;
	tDstd.a = alpha;
	tDstd.y += shiftY;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	//end
	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.w += sizeX;
	tDstd.h += sizeY;
	tDstd.x -= sizeX * 0.5;
	tDstd.y -= sizeY * 0.5;
	tDstd.sortID += 1;
	if (active == 0) {
		grh = srcLe->grHandles[0];
	}
	else if (srcLe->timer == dst->timer) {
		grh = srcLe->grHandles[GetSRCcycleNow(*srcLe, GetTimeLapse(srcLe->timer, T) - dst->draw->time)];
	}
	else {
		grh = srcLe->grHandles[GetSRCcycleNow(*srcLe, GetTimeLapse(srcLe->timer, T))];
	}
	tDstd.x += shiftX;
	tDstd.subHandle = dst->n;
	tDstd.blend = 1;
	tDstd.a = alpha;
	tDstd.y += longY;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

int AddDrawingBuffer_PlayArea(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, float shiftX, float shiftY, int alpha, float sizeX, float sizeY, char flag) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.w += sizeX;
	tDstd.h += sizeY;
	tDstd.x -= sizeX * 0.5;
	tDstd.y -= sizeY * 0.5;
	if (src->graphcount <= 0) return 0;
	
	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	tDstd.x += shiftX;
	tDstd.y += shiftY;
	tDstd.filter = 0;
	tDstd.blend = 1;
	if (alpha >= 0) tDstd.a = alpha;
	tDstd.subHandle = (flag == 0) ? -1 : dst->n;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

int AddDrawingBuffer_Gauge(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int hp, char isSurvival) {
	DSTdraw tDstd;
	int grh,tremble,cyc=0;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;
	
	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (src->graphcount > 4 && src->cycle > 0) {
		cyc = (src->graphcount/4) * ((int)GetTimeLapse(src->timer, T) % src->cycle) / src->cycle * 4;
	}

	tremble = hp - GetRand(2);
	for (int i = 1; i <= 50; i++) {
		if (isSurvival == 0) {
			if (1 <= i && i < 40) {
				if ((i < tremble || hp <= i || i == 1) && i <= hp)
					grh = src->grHandles[cyc + 1];
				else
					grh = src->grHandles[cyc + 3];
			}
			else {
				if (tremble <= i && i < hp && i != 1)
					grh = src->grHandles[cyc + 2];
				else if(hp < i)
					grh = src->grHandles[cyc + 2];
				else
					grh = src->grHandles[cyc];
			}
		}
		else if (i < tremble || hp <= i || i == 1) {
			if(hp < i)
				grh = src->grHandles[cyc + 2];
			else
				grh = src->grHandles[cyc];
		}
		else grh = src->grHandles[cyc + 2];

		if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);
		tDstd.x += src->op1;
		tDstd.y += src->op2;
	}
	return 1;
}

int AddDrawingBuffer_BGA(DrawingBuf *drb, SRCstruct */*src*/, DSTstruct *dst, Timer *T, int grHandle, char flag) {
	DSTdraw tDstd;
	int x, y;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	
	GetGraphSize(grHandle, &x, &y);
	if (x < 256){
		tDstd.x += tDstd.w / 256.0 * (256.0 - x) * 0.5; // *0.00390625 /256
		tDstd.w *= x / 256.0;
	}
	if (y < 256) {
		tDstd.h *= y / 256.0;
	}
	if (flag == 0) tDstd.sortID++;
	
	tDstd.isDrawBackbox = flag;

	if (tDstd.time != -1 && grHandle != -1) AddDrawingBuffer(drb, grHandle, &tDstd);

	return 1;
}

int AddDrawingBuffer_Scratch(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int angle) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (src->graphcount <= 0) return 0;

	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	tDstd.angle = angle;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

int AddDrawingBuffer_Image2(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (src->graphcount <= 0) return 0;

	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

int AddDrawingBuffer_Object(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int x, int y) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	tDstd.x += x;
	tDstd.y += y;

	if (src->graphcount <= 0) return 0;

	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

int AddDrawingBuffer_ObjectAlpha(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int x, int y, int a) {
	DSTdraw tDstd;
	int grh;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	if (src->graphcount <= 0) return 0;

	if (src->timer == dst->timer) {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T) - dst->draw->time)];
	}
	else {
		grh = src->grHandles[GetSRCcycleNow(*src, GetTimeLapse(src->timer, T))];
	}
	tDstd.x += x;
	tDstd.y += y;
	tDstd.blend = 1;
	tDstd.a = a;
	if (tDstd.time != -1 && grh != -1) AddDrawingBuffer(drb, grh, &tDstd);

	return 1;
}

int AddDrawingBuffer_EventLoading(DrawingBuf *drb, int grHandle, DSTstruct *dst, Timer *T, int x, int y) {
	DSTdraw tDstd;

	if (dst->dstCount <= 0 || dst->dataSize <= 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));

	tDstd.x += x;
	tDstd.y += y;
	if (tDstd.time != -1 && grHandle != -1) AddDrawingBuffer(drb, grHandle, &tDstd);

	return 1;
}

int AddDrawingBuffer_Lunaris(DrawingBuf *drb, SRCstruct *src, DSTdraw *dstd, Timer *T) {
	int grh;

	if (src->graphcount <= 0) return 0;

	grh = GetSRCcycleNow(*src, GetTimeLapse(src->timer, T));
	if (dstd->time != -1 && src->grHandles[grh] != -1) AddDrawingBuffer(drb, src->grHandles[grh], dstd);

	return 1;
}

int AddDrawingBuffer_Numbers(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T, int number, int x, int y) {
	DSTdraw tDstd;
	int grh,cyc,grct;
	CSTR str;

	if (dst->dstCount <= 0 || dst->dataSize <= 0 || src->op3 <= 0 || src->op2 <= -1 || src->op2 >= 3) return 0;

	cyc = 0;
	grct = 0;
	if (src->graphcount % 10 == 0){
		grct = 1;
		if(src->cycle > 0) cyc = (src->graphcount / 10) * ((int)GetTimeLapse(src->timer, T) % src->cycle) / src->cycle * 10;
	}
	if (src->graphcount % 11 == 0) {
		grct = 2;
		if (src->cycle > 0) cyc = (src->graphcount / 11) * ((int)GetTimeLapse(src->timer, T) % src->cycle) / src->cycle * 11;
	}
	if (src->graphcount % 24 == 0) {
		grct = 3;
		if (src->cycle > 0) cyc = (src->graphcount / 24) * ((int)GetTimeLapse(src->timer, T) % src->cycle) / src->cycle * 24;
	}
	if (grct == 0) return 0;
	if (grct != 3 && number < 0) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	if (tDstd.time == -1) return 0;

	tDstd.x += x;
	tDstd.y += y;
	//op2 : align
	if (src->op2 == 0) {
		cstrSprintf(&str, "%10d", abs(number));
		str.lastCut(src->op3);
	}
	else if (src->op2 == 1) {
		cstrSprintf(&str, "%d          ", abs(number));
		str.nullAtPos(src->op3);
	}
	else if (src->op2 == 2) {
		int blanks = src->op3 - CountDigit(number);
		if (blanks < 0)	blanks = 0;
		tDstd.x += blanks * tDstd.w * 0.5;

		cstrSprintf(&str, "%d          ", abs(number));
		str.nullAtPos(src->op3);
	}

	if (grct == 3) {
		if (number < 0) grh = src->grHandles[cyc + 23];
		else grh = src->grHandles[cyc + 11];
		AddDrawingBuffer(drb, grh, &tDstd);
		tDstd.x += tDstd.w;
	}
	if (number < 0 && src->graphcount == 24) cyc += 12;

	for (int i = 0; i < str.length(); i++) {
		switch (*str.atPos(i)) {
			case ' ':
				if (grct != 1) {
					grh = src->grHandles[cyc + 10];
					AddDrawingBuffer(drb, grh, &tDstd);
				}
				break;
			case '0':
				grh = src->grHandles[cyc + 0];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '1':
				grh = src->grHandles[cyc + 1];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '2':
				grh = src->grHandles[cyc + 2];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '3':
				grh = src->grHandles[cyc + 3];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '4':
				grh = src->grHandles[cyc + 4];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '5':
				grh = src->grHandles[cyc + 5];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '6':
				grh = src->grHandles[cyc + 6];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '7':
				grh = src->grHandles[cyc + 7];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '8':
				grh = src->grHandles[cyc + 8];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			case '9':
				grh = src->grHandles[cyc + 9];
				AddDrawingBuffer(drb, grh, &tDstd);
				break;
			default:
				break;
		}
		tDstd.x += tDstd.w;
	}
	return 1;
}

int AddDrawingBuffer_Slider(DrawingBuf *drb, SRCstruct *src, DSTstruct *dst, Timer *T) {
	DSTdraw tDstd;

	if (src->op2 < 1) return 0;

	tDstd = SetDSTdrawByTime(*dst, GetTimeLapse(dst->timer, T));
	AddDrawingBuffer_Object(drb, src, dst, T, src->sx, src->sy);

	return 1;
}

int AddDrawingBuffer_JudgeCombo(DrawingBuf *drb, SRCstruct *jSrc, DSTstruct *jDst, SRCstruct *cSrc, DSTstruct *cDst, Timer *T, int combo, int optX, int optY) {
	DSTdraw tDstd;
	int digit;
	float pos, posn;

	if (jDst->dstCount <= 0 || jDst->dataSize <= 0) return 0;

	digit = 0;
	if (combo > 0) {
		digit = 1;
		for (int i = combo; i > 9; i /= 10) {
			digit++;
		}
	}

	//op1 : noshift
	pos = 0.0;
	posn = 0.0;
	if (jSrc->op1 == 0 && combo > 0) {
		tDstd = SetDSTdrawByTime(*cDst, GetTimeLapse(jDst->timer, T));
		pos = -(digit * tDstd.w * 0.5);
	}
	else {
		tDstd = SetDSTdrawByTime(*cDst, GetTimeLapse(jDst->timer, T));
		posn = -( (digit-1) * tDstd.w * 0.5);
	}

	AddDrawingBuffer_Object(drb, jSrc, jDst, T, optX + pos, optY);

	if (combo > 0) {
		tDstd = SetDSTdrawByTime(*jDst, GetTimeLapse(jDst->timer, T));
		//tDstd2 = SetDSTdrawByTime(*jDst, GetTimeLapse(jDst->timer, T));
		AddDrawingBuffer_Numbers(drb, cSrc, cDst, T, combo, optX + pos + posn + tDstd.x, optY + tDstd.y);
	}
	return 1;
}


float skinsizeX, skinsizeY;

int LRDrawImg(int *grHandle, DSTdraw *dstD) {
	float x1, y1, x2, y2;
	float rx, ry, rad;
	VECTOR vec1, vec2, vec3, vec4;
	int xs, ys;

	if ((dstD->a < 2) && (dstD->blend != 0)) {
		return 0;
	}
	if ((dstD->blend == 1) && (0xfe < dstD->a)) {
		dstD->blend = 0;
	}
	SetDrawMode(dstD->filter);
	SetDrawBlendMode(dstD->blend, dstD->a);
	SetDrawBright(dstD->r, dstD->g, dstD->b);
	if (dstD->h < 0.0) {
		dstD->h = -dstD->h;
		dstD->y = dstD->y - dstD->h;
	}
	if (dstD->w < 0.0) {
		dstD->w = -dstD->w;
		dstD->x = dstD->x - dstD->w;
	}
	x1 = dstD->x;
	y1 = dstD->y;
	x2 = dstD->w + dstD->x;
	y2 = dstD->y + dstD->h;

	if (dstD->angle != 0.0) {
		rad = (dstD->angle / 360.0) * 6.283185;
		switch (dstD->center) {
		case 1:
			rx = x1;
			ry = y2;
			break;
		case 2:
			rx = (x1 + x2) * 0.5;
			ry = y2;
			break;
		case 3:
			rx = x2; 
			ry = y2;
			break;
		case 4:
			rx = x1;
			ry = (y1 + y2) * 0.5;
			break;
		default:
			rx = (x1 + x2) * 0.5;
			ry = (y1 + y2) * 0.5;
			break;
		case 6:
			rx = x2;
			ry = (y1 + y2) * 0.5;
			break;
		case 7:
			rx = x1;
			ry = y1;
			break;
		case 8:
			rx = (x1 + x2) * 0.5;
			ry = y1;
			break;
		case 9:
			rx = x2;
			ry = y1;
			break;
		}
		vec1.x = x1 - rx;
		vec1.y = y1 - ry;
		VectorRotationZ(&vec1, &vec1, rad);
		vec2.x = x2 - rx;
		vec2.y = y1 - ry;
		VectorRotationZ(&vec2, &vec2, rad);
		vec3.x = x2 - rx;
		vec3.y = y2 - ry;
		VectorRotationZ(&vec3, &vec3, rad);
		vec4.x = x1 - rx;
		vec4.y = y2 - ry;
		VectorRotationZ(&vec4, &vec4, rad);
		if (dstD->h != 0.0 && dstD->w != 0.0 && dstD->time >= 0) {
			DrawModiGraph((vec1.x + rx) * skinsizeX + 0.5f, (vec1.y + ry) * skinsizeY + 0.5f, (vec2.x + rx) * skinsizeX + 0.5f, (vec2.y + ry) * skinsizeY + 0.5f,
				(vec3.x + rx) * skinsizeX + 0.5f, (vec3.y + ry) * skinsizeY + 0.5f, (vec4.x + rx) * skinsizeX + 0.5f, (vec4.y + ry) * skinsizeY + 0.5f, *grHandle, 1);
		}
	}
	else {
		GetGraphSize(*grHandle, &xs, &ys);
		GetTimeWrap();
		if (dstD->isDrawBackbox) {
			DrawBox(x1*skinsizeX + 0.5f, y1*skinsizeY + 0.5f, x2*skinsizeX + 0.5f, y2*skinsizeY + 0.5f, GetColor(0, 0, 0), true);
		}
		DrawExtendGraph(x1*skinsizeX + 0.5f, y1*skinsizeY + 0.5f, x2*skinsizeX + 0.5f, y2*skinsizeY + 0.5f, *grHandle, 1);
	}
	return 1;
}

int GetTextGraphLength(CSTR *str, ImageFont *imF) {
	int totalX = 0;
	if (str->length() == 0) return 0;
	std::u32string u32Str = utf8_to_utf32(str->body);
	for (auto& ch : u32Str) {
		int x = 0, y = 0;
		auto& chTex = imF->chars[ch];
		if (chTex.grHandle == -1) LoadFontCharGraph(imF, ch);
		GetGraphSize(chTex.grHandle != -1 ? chTex.grHandle : imF->chars[U'?'].grHandle, &x, &y);
		totalX += imF->kerning + x;
	}
	return totalX - imF->kerning;
}

void LRDrawText(int* grHandle, DSTdraw *dstd, CSTR *str, ImageFont *imF) {
	if (imF->size <= 0 || dstd->h == 0.0 || dstd->w == 0.0 || str->length() < 1) {		
		if (dstd->h != 0.0 && dstd->w != 0.0 && str->length() > 0 && *grHandle != -1) {
			int size = 0, iDum = 0;
			SetDrawMode(dstd->filter);
			SetDrawBlendMode(dstd->blend, dstd->a);
			SetDrawBright(dstd->r, dstd->g, dstd->b);
			GetFontStateToHandle(0, &size, &iDum, *grHandle);
			double hl = dstd->h / (float)size;
			float width = GetDrawStringWidthToHandle(str->outstr(), str->length(), *grHandle, 0);
			if (width != 0.0) {
				float wl = width > dstd->w ? wl = dstd->w / width : 1.f;
				wl = wl * hl;
				if (dstd->align == 1) {
					dstd->x = dstd->x - (int)(width*wl*0.5);
				}
				else if (dstd->align == 2) {
					dstd->x = dstd->x - (int)(width*wl);
				}
				GetTimeWrap();
				DrawExtendStringToHandle(dstd->x, dstd->y, wl, hl, str->outstr(), GetColor(dstd->r, dstd->g, dstd->b), *grHandle, 0, 0);
			}
		}
	}
	else {
		SetDrawMode(dstd->filter);
		SetDrawBlendMode(dstd->blend, dstd->a);
		SetDrawBright(dstd->r, dstd->g, dstd->b);
		double hl = dstd->h / (float)imF->size;
		float width = GetTextGraphLength(str, imF);
		if (width != 0.0) {
			float wl = width > dstd->w ? wl = dstd->w / width : 1.f;		
			wl = wl * hl;
			if (dstd->align == 1) {
				dstd->x = dstd->x - (int)(width*wl*0.5);
			}
			else if (dstd->align == 2) {
				dstd->x = dstd->x - (int)(width*wl);
			}
			float wSum = 0.f;
			std::u32string u32Str = utf8_to_utf32(str->body);
			for (auto ch : u32Str) {
				int x = 0, y = 0;
				float xf = 0.f, yf = 0.f;
				auto chTex = imF->chars[ch];
				GetGraphSize(chTex.grHandle, &x, &y);
				xf = x;
				yf = y;
				if (xf <= 0.0) {
					ch = U'?';
					chTex = imF->chars[ch];
					GetGraphSize(chTex.grHandle, &x, &y);
					xf = x;
					yf = y;
				}
				xf *= wl;
				if (ch != U' ' && ch != U'\n' && chTex.grHandle != -1) {
					DrawExtendGraph(dstd->x + wSum + 0.5f, dstd->y + 0.5f, dstd->x + wSum + xf + 0.5f, dstd->y + hl * yf + 0.5f, chTex.grHandle, 1);
				}
				wSum += imF->kerning * wl + xf;
			}
			return;
		}
	}
}

void LRDrawTextInput(int* hFont, DSTdraw *dstd, int* hInput, ImageFont *imgfont) {
	IMEINPUTDATA* pIME;
	CSTR buf(0x401);
	int grLen;
	int len1, len2;
	if (*hInput != -1) {
		GetKeyInputString(buf, *hInput);
		if (imgfont->size < 1) {
			if (dstd->h != 0.0 || dstd->w != 0.0 || *hFont != -1) {
				SetDrawMode(dstd->filter);
				SetDrawBlendMode(0, 0xff);
				SetDrawBright(dstd->r, dstd->g, dstd->b);
				SetKeyInputStringFont(*hFont);
				GetTimeWrap();
				DrawKeyInputString(dstd->x, dstd->y, *hInput);
			}
		}
		else if (dstd->h != 0.0 || dstd->w != 0.0) {
			pIME = (IMEINPUTDATA*)GetIMEInputData();
			dstd->w = 640.0; //TODO_RESOULUTION
			LRDrawText(hFont, dstd, &buf, imgfont);
			grLen = 0;
			if (GetKeyInputCursorPosition(*hInput)) {
				CSTR tCstr(buf.left(GetKeyInputCursorPosition(*hInput)));
				grLen = GetTextGraphLength(&tCstr, imgfont);
			}
			if (pIME == NULL)	buf.fillzero();
			else				cstrSprintf(&buf, "%s", *pIME);
			dstd->x += grLen;
			if (buf.length() < 1) {
				DrawBox(dstd->x, dstd->y, dstd->x + 1.0, dstd->y + dstd->h, GetColor(0xff, 0xff, 0xff), 1);
			}
			else {
				for (int i = 0; pIME != 0 && i < pIME->ClauseNum; i++) {
					int pos1 = pIME->ClauseData[grLen].Position;
					int pos2 = pIME->ClauseData[grLen].Length + pos1;
					
					if (pos2 <= pos1) break;
					if (pos1) {
						CSTR tCstr(buf.left(pos1));
						pos1 = GetTextGraphLength(&tCstr, imgfont);
					}
					len1 = pos1;
					CSTR tCstr(buf.left(pos2));
					len2 = GetTextGraphLength(&tCstr, imgfont);
					DrawBox(dstd->x + len1 + 1.0, dstd->y, dstd->x + len2 - 1.0, dstd->h + dstd->y, (grLen == pIME->SelectClause) ? GetColor(255, 0, 0) : GetColor(64, 64, 64),1);
				}
				LRDrawText(hFont, dstd, &buf, imgfont);
			}
			GetIMEInputModeStr(buf);
			dstd->y = 480.0 - dstd->h;
			dstd->x = 640.0; //TODO_RESOULUTION
			dstd->align = 2;
			LRDrawText(hFont, dstd, &buf, imgfont);
		}
	}
}

int sortDST(const void *a, const void *b) {
	DSTdraw &aa = *(DSTdraw*)a;
	DSTdraw &bb = *(DSTdraw*)b;
	return aa.sortID - bb.sortID;
}

int LRDraw(DrawingBuf *drBuf, TextStruct *txt, SONGSELECT *sSel, skstruct *sks, int targetBufNum, int x, int y) {
	DSTdraw &draw = drBuf->dstd[targetBufNum];

	if (drBuf->isDisabled) return 0;
	if (targetBufNum == 0) qsort(drBuf->dstd, drBuf->count, sizeof(DSTdraw), sortDST);
	if (drBuf->count <= targetBufNum) return -1;
	
	skinsizeX = sks->adjust.rate_x / 100.0;
	skinsizeY = sks->adjust.rate_y / 100.0;

	draw.x += sks->adjust.shift_x + x;
	draw.y += sks->adjust.shift_y + y;
	if (draw.fontHandle == -1) {
		if (draw.grHandle != -1) {
			if (0 <= draw.subHandle && draw.subHandle < 20 && drBuf->isHidSud[draw.subHandle] > 0) {
				SetDrawArea(0, drBuf->top[draw.subHandle], skinSizeX, drBuf->bottom[draw.subHandle]); //TODO_RESOULUTION
			}
			LRDrawImg(&draw.grHandle, &draw);
			if (0 <= draw.subHandle && draw.subHandle < 20 && drBuf->isHidSud[draw.subHandle] > 0) {
				SetDrawArea(0, 0, skinSizeX, skinSizeY);
			}
			return 1;
		}
	}
	else if (draw.grHandle != -1) return 1;
	//if (draw.subHandle == -1) return 1; //DEBUG : this line is not in original
	if (draw.fontHandle != -1) {
		if (draw.subHandle >= 10000) {
			if (sSel->filter.disablesubtitle == 1) {
				LRDrawText(&draw.fontHandle, &draw, &sSel->bmsList[draw.subHandle - 10000].title, &sks->ImageFonts[(int)draw.angle]);
			}
			else {
				LRDrawText(&draw.fontHandle, &draw, &sSel->bmsList[draw.subHandle - 10000].fulltitle, &sks->ImageFonts[(int)draw.angle]);
			}
		}
		else if (draw.subHandle >= 1000) {
			LRDrawText(&draw.fontHandle, &draw, &txt->readme.body[draw.subHandle - 1000], &sks->ImageFonts[(int)draw.angle]);
		}
		else if (txt->st_text_num == draw.subHandle) {
			LRDrawTextInput(&draw.fontHandle, &draw, &txt->hKeyInput, &sks->ImageFonts[(int)draw.angle]);
		}
		else {
			CSTR a = GetStringFromArray(draw.subHandle, txt->objectStr);
			LRDrawText(&draw.fontHandle, &draw, &a, &sks->ImageFonts[(int)draw.angle]);
		}
	}

	return 1;
}

int DrawBGA(int grHandle){
	int TransFlag;
	int grW;
	int grH;
	float tgX;
	float tgW;
	float tgH;

	GetGraphSize(grHandle, &grW, &grH);
	tgX = 0.0;
	tgW = 256.0;
	tgH = 256.0;
	if (grW < 256) {
		tgX = (256.0 - (float)grW) * 0.5;;
		tgW = (float)grW * 256.0 / 256.0;
	}
	if (grH < 256) {
		tgH = (float)grH * 256.0 / 256.0;
	}
	TransFlag = 1;
	DrawExtendGraph(tgX, 0, tgX + tgW, tgH + 0.0, grHandle, TransFlag);
	return 1;
}

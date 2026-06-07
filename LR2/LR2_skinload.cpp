#include "LR2_skinload.h"
#include "LR2_skindraw.h"
#include "LR2_configsave.h"
#include "En_fileutil.h"
#include "filesystem.h"
#include "filesystem.h"
bool IsMultibyte(byte ch){
	/*if (0x80 < ch) {
		if (ch < 0xa0) return true;
		if (0xdf < ch) return ch < 0xfe;
	}
	return false;*/
	return (0x81 <= ch && ch < 0xA0) || (0xE0 <= ch && ch < 0xFE);
}

////LR2graphic_load
int InitSRC(SRCstruct *src){
	for(int i = 0; i < src->graphcount; i++){
		DeleteGraph(src->grHandles[i]);
	}
	if (src->count == 0) {
		src->count = 1;
		src->grHandles = (int*)malloc(sizeof(int));
	}
	memset(src->grHandles, '\0', src->count * sizeof(int));
	src->cycle = 0;
	src->graphcount = 0;
	src->n = 0;
	src->timer = 0;
	src->align = 0;
	src->st = 0;
	src->op1 = 0;
	src->op2 = 0;
	src->op3 = 0;
	src->op4 = 0;
	src->op5 = 0;
	src->sx = 0;
	src->sy = 0;
	src->inArray = 0;
	src->fontHandle = -1;
	return 1;
}

//maybe deleted by compiler, restored it for convenience
int InitDST(DSTstruct *dst) {
	dst->n = 0;
	dst->loop = 0;
	dst->opt1 = 0;
	dst->opt2 = 0;
	dst->opt3 = 0;
	dst->opt4 = 0;
	dst->opt5 = 0;
	dst->dstCount = 0;
	dst->dataSize = 0;
	return 1;
}

int ReadDST(DSTstruct *dst, CSVbuf *csv, int order){

	//parameters only for first line
	if (dst->dstCount == 0) {
		dst->n = csv->val[1];
		dst->loop = csv->val[16];
		dst->timer = csv->val[17];
		dst->opt1 = csv->val[18];
		dst->opt2 = csv->val[19];
		dst->opt3 = csv->val[20];
		dst->opt4 = csv->val[21];
		dst->opt5 = csv->val[22];
	}

	//memory allocation if needed
	if (dst->dataSize == dst->dstCount) {
		dst->dataSize = dst->dataSize + 10;
		dst->draw = (DSTdraw*)realloc(dst->draw, dst->dataSize * sizeof(DSTdraw));
		assert(dst->draw != nullptr);
		for (int i = dst->dstCount; i < dst->dataSize; i++) {
			InitDSTdraw(&dst->draw[i]);
		}
	}

	//read dst
	dst->draw[dst->dstCount].time = csv->val[2];
	dst->draw[dst->dstCount].x = (int)(float)csv->val[3];
	dst->draw[dst->dstCount].y = (int)(float)csv->val[4];
	dst->draw[dst->dstCount].w = (int)(float)csv->val[5];
	dst->draw[dst->dstCount].h = (int)(float)csv->val[6];
	dst->draw[dst->dstCount].acc = csv->val[7];
	dst->draw[dst->dstCount].a = csv->val[8];
	dst->draw[dst->dstCount].r = csv->val[9];
	dst->draw[dst->dstCount].g = csv->val[10];
	dst->draw[dst->dstCount].b = csv->val[11];
	dst->draw[dst->dstCount].blend = csv->val[12];
	dst->draw[dst->dstCount].filter = csv->val[13];
	dst->draw[dst->dstCount].angle = (float)csv->val[14];
	dst->draw[dst->dstCount].center = csv->val[15];
	dst->draw[dst->dstCount].sortID = order;
	dst->draw[dst->dstCount].subHandle = -1;
	dst->dstCount++;
	return 1;
}

int ReadSRC(SRCstruct *src, CSVbuf *csv, skstruct *sk){
	src->n = csv->val[1];
	src->cycle = csv->val[9];
	src->timer = csv->val[10];
	src->op1 = csv->val[11];
	src->op2 = csv->val[12];
	src->op3 = csv->val[13];
	src->op4 = csv->val[14];
	src->op5 = csv->val[15];
	if (csv->val[5] < 0) GetGraphSize(sk->GrHandle[csv->val[2]], &csv->val[5], NULL); //w
	if (csv->val[6] < 0) GetGraphSize(sk->GrHandle[csv->val[2]], NULL, &csv->val[6]); //h
	if (csv->val[3] < 1) csv->val[3] = 0; //x
	if (csv->val[4] < 1) csv->val[4] = 0; //y
	if (csv->val[7] < 1) csv->val[7] = 1; //div_x
	if (csv->val[8] < 1) csv->val[8] = 1; //div_y
	src->graphcount = csv->val[8] * csv->val[7];
	if (src->graphcount < 1) src->graphcount = 1;
	if (src->count < src->graphcount) {
		src->count = src->graphcount;
		src->grHandles = (int*)realloc(src->grHandles, src->graphcount * sizeof(int));
	}
	assert(src->grHandles != nullptr);
	for (int i = 0; i < src->count; i++) {
		src->grHandles[i] = -1;
	}

	for (int i = 0; i < csv->val[7]; i++) {
		for (int j = 0; j < csv->val[8]; j++) {
			if (sk->grIsMovie[csv->val[2]] == 0) {
				int blockY = csv->val[6] / csv->val[8];
				int blockX = csv->val[5] / csv->val[7];
				src->grHandles[i + csv->val[7]*j] = DerivationGraph(blockX*i + csv->val[3], blockY*j + csv->val[4], blockX, blockY, sk->GrHandle[csv->val[2]]);
			}
			else {
				src->grHandles[i + csv->val[7]*j] =	sk->GrHandle[csv->val[2]];
			}
		}
	}
	return 1;
}

bool CheckIndexRange(int index, int min, int max, int line, char *str){
	if ((min <= index) && (index <= max)) {
		return true;
	}
	ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nインデックス範囲外です(%d-%d)。\n", line, str, min, max);
	return false;
}

int ReadSRC_BAR_TITLE(SRCstruct *src, CSVbuf *csv, skstruct *sk){
	src->n = csv->val[1];
	src->fontHandle = sk->fontHandle[csv->val[2]];
	src->cycle = csv->val[2];
	src->st = csv->val[3];
	src->align = csv->val[4];
	src->op1 = csv->val[5];
	src->op2 = csv->val[6];
	src->op3 = csv->val[7];
	src->op4 = csv->val[8];
	src->op5 = csv->val[9];
	return 1;
}


// InitSkin
int InitSkin(skstruct *sk, int /*unused*/, char font) {
	SetTransColor(0, 255, 0);
	sk->startinput_start = 0;
	sk->startinput_rank = 0;
	sk->startinput_update = 0;
	sk->scenetime = 0;
	sk->loadstart = 0;
	sk->loadend = 0;
	sk->playstart = 2000;
	sk->fadeout = 0;
	sk->close = 0;
	sk->horizontal = 0;

	for (int i = 0; i < 1000; i++) {
		InitDSTdraw(&sk->drBuf.dstd[i]);
	}
	(sk->drBuf).count = 0;

	(sk->image).srcSize = 0;
	if ((sk->image).dstSize == 0) {
		(sk->image).dstSize = 200;
		(sk->image).src = (SRCstruct *)malloc(200*sizeof(SRCstruct));
		memset(sk->image.src, '\0', (sk->image).dstSize * sizeof(SRCstruct));
		(sk->image).dst = (DSTstruct *)malloc((sk->image).dstSize * sizeof(DSTstruct));
		memset(sk->image.dst, '\0', (sk->image).dstSize * sizeof(DSTstruct));
	}
	for (int i = 0; i < sk->image.dstSize; i++) {
		InitSRC(&sk->image.src[i]);
	}
	for (int i = 0; i < sk->image.dstSize; i++) {
		InitDST(&sk->image.dst[i]);
	}

	for (auto& obj : sk->otherObject) {
		(obj).srcSize = 0;
		if ((obj).dstSize == 0) {
			(obj).dstSize = 20;
			(obj).src = (SRCstruct *)malloc(20 * sizeof(SRCstruct));
			memset(obj.src, '\0', (obj).dstSize * sizeof(SRCstruct));
			(obj).dst = (DSTstruct *)malloc((obj).dstSize * sizeof(DSTstruct));
			memset(obj.dst, '\0', (obj).dstSize * sizeof(DSTstruct));
		}
		for (int i = 0; i < obj.dstSize; i++) {
			InitSRC(&obj.src[i]);
		}
		for (int i = 0; i < obj.dstSize; i++) {
			InitDST(&obj.dst[i]);
		}
	}

	InitSRC(&sk->src_MOUSECURSOR);
	sk->dst_MOUSECURSOR.n = 0;
	sk->dst_MOUSECURSOR.loop = 0;
	sk->dst_MOUSECURSOR.opt1 = 0;
	sk->dst_MOUSECURSOR.opt2 = 0;
	sk->dst_MOUSECURSOR.opt3 = 0;
	sk->dst_MOUSECURSOR.opt4 = 0;
	sk->dst_MOUSECURSOR.opt5 = 0;
	sk->dst_MOUSECURSOR.dstCount = 0;
	sk->dst_MOUSECURSOR.dataSize = 0;

	sk->BAR_CENTER = 0;
	for (int i = 0; i < 10; i++) {
		InitSRC(&sk->src_BAR_BODY[i]);
	}
	for (int i = 0; i < 30; i++) {
		InitDST(&sk->dst_BAR_BODY_OFF[i]);
	}
	for (int i = 0; i < 30; i++) {
		InitDST(&sk->dst_BAR_BODY_ON[i]);
	}

	for (int i = 0; i < 5; i++) {
		InitSRC(&sk->src_BAR_TITLE[i]);
	}
	for (int i = 0; i < 5; i++) {
		InitDST(&sk->dst_BAR_TITLE[i]);
	}

	sk->bar_availabe_from = 0;
	sk->bar_availabe_to = 0;
	InitSRC(&sk->src_BAR_FLASH);
	InitDST(&sk->dst_BAR_FLASH);
	InitDST(&sk->dst_BAR_STAGEFILE);

	for (int i = 0; i < 10; i++) {
		InitSRC(&sk->src_BAR_LEVEL[i]);
	}
	for (int i = 0; i < 10; i++) {
		InitDST(&sk->dst_BAR_LEVEL[i]);
	}

	for (int i = 0; i < 20; i++) {
		InitSRC(&sk->src_NOTE[i]);
		InitSRC(&sk->src_MINE[i]);
		InitSRC(&sk->src_LN_START[i]);
		InitSRC(&sk->src_LN_END[i]);
		InitSRC(&sk->src_LN_BODY[i]);
		InitSRC(&sk->src_AUTO_NOTE[i]);
		InitSRC(&sk->src_AUTO_MINE[i]);
		InitSRC(&sk->src_AUTO_LN_START[i]);
		InitSRC(&sk->src_AUTO_LN_END[i]);
		InitSRC(&sk->src_AUTO_LN_BODY[i]);
		InitDST(&sk->dst_NOTE[i]);
	}

	for (int i = 0; i < 2; i++) {
		InitSRC(&sk->src_LINE[i]);
		InitDST(&sk->dst_LINE[i]);
	}

	for (int i = 0; i < 6; i++) {
		InitSRC(&sk->src_NOWJUDGE_1P[i]);
		InitSRC(&sk->src_NOWJUDGE_2P[i]);
	}
	for (int i = 0; i < 6; i++) {
		InitDST(&sk->dst_NOWJUDGE_1P[i]);
		InitDST(&sk->dst_NOWJUDGE_2P[i]);
	}

	for (int i = 0; i < 6; i++) {
		InitSRC(&sk->src_NOWCOMBO_1P[i]);
		InitSRC(&sk->src_NOWCOMBO_2P[i]);
	}
	for (int i = 0; i < 6; i++) {
		InitDST(&sk->dst_NOWCOMBO_1P[i]);
		InitDST(&sk->dst_NOWCOMBO_2P[i]);
	}

	for (int i = 0; i < 6; i++) {
		InitSRC(&sk->src_BAR_LAMP[i]);
		InitDST(&sk->dst_BAR_LAMP[i]);
		InitSRC(&sk->src_BAR_MY_LAMP[i]);
		InitDST(&sk->dst_BAR_MY_LAMP[i]);
		InitSRC(&sk->src_BAR_RIVAL_LAMP[i]);
		InitDST(&sk->dst_BAR_RIVAL_LAMP[i]);
	}

	for (int i = 0; i < 2; i++) {
		InitSRC(&sk->src_GROOVEGAUGE[i]);
		InitDST(&sk->dst_GROOVEGAUGE[i]);
	}

	for (int i = 0; i < 2; i++) {
		InitSRC(&sk->src_GAUGECHART_1P[i]);
		InitDST(&sk->dst_GAUGECHART_1P[i]);
		InitSRC(&sk->src_GAUGECHART_2P[i]);
		InitDST(&sk->dst_GAUGECHART_2P[i]);
	}

	for (int i = 0; i < 3; i++) {
		InitSRC(&sk->src_SCORECHART[i]);
		InitDST(&sk->dst_SCORECHART[i]);
	}

	for (int i = 0; i < 2; i++) {
		InitSRC(&sk->src_JUDGELINE[i]);
		InitDST(&sk->dst_JUDGELINE[i]);
	}
	sk->scratchAngle_1 = 0.0;
	sk->scratchAngle_2 = 0.0;
	InitSRC(&sk->src_THUMBNAIL);
	InitDST(&sk->dst_THUMBNAIL);
	InitSRC(&sk->src_README[0]);
	InitDST(&sk->dst_README[0]);
	InitSRC(&sk->src_README[1]);
	InitDST(&sk->dst_README[1]);
	for (int i = 0; i < 10; i++) {
		sk->ImageFonts[i].images.resize(1000);
	}
	sk->count = 0;
	sk->num_of_struct = 0;
	sk->num_of_ImageFont = 0;
	if (font == '\0') {
		InitFontToHandle();
	}
	for (int i = 0; i < 10; i++) {
		sk->helpfilePath[i].fillzero();
	}
	sk->textmergin_1 = 10;
	sk->textmergin_2 = 10;
	sk->helpfileCount = 0;
	sk->flag_BGA = 1;
	(sk->adjust).rate_x = 100;
	(sk->adjust).rate_y = 100;
	(sk->adjust).shift_x = 0;
	(sk->adjust).shift_y = 0;
	(sk->adjust).judge_x = 0;
	(sk->adjust).judge_y = 0;
	(sk->adjust).size_x = 0;
	(sk->adjust).size_y = 0;
	(sk->adjust).dark_type = 0;
	(sk->adjust).note_1p_x = 0;
	(sk->adjust).note_1p_y = 0;
	(sk->adjust).note_2p_x = 0;
	(sk->adjust).note_2p_y = 0;
	sk->scratchside_1 = 0;
	sk->scratchside_2 = 1;
	for (int i = 0; i < 20; i++) {
		sk->customfileRANDOM[i].fillzero();
		sk->customfile[i].fillzero();
	}
	sk->customfile_count = 0;
	
	DeleteGraph(sk->GrHandle[GrH_Preview]);
	sk->GrHandle[GrH_Preview] = MakeGraph(skinSizeX, skinSizeY); //TODO_RESOULUTION	
	DeleteGraph(sk->GrHandle[104]);
	sk->GrHandle[104] = MakeGraph(256, 256);
	if (sk->GrHandle[GrH_Stage] == -1) sk->GrHandle[GrH_Stage] = MakeGraph(640, 480);
	if (sk->GrHandle[GrH_BackBMP] == -1) sk->GrHandle[GrH_BackBMP] = MakeGraph(640, 480);
	if (sk->GrHandle[GrH_Banner] == -1) sk->GrHandle[GrH_Banner] = MakeGraph(300, 80);
	DeleteGraph(sk->GrHandle[GRHTYPE_BLACK]);
	sk->GrHandle[GRHTYPE_BLACK] = LoadGraph(fs::make_preferred("LR2files/Config/black.bmp").data());
	DeleteGraph(sk->GrHandle[GRHTYPE_WHITE]);
	sk->GrHandle[GRHTYPE_WHITE] = LoadGraph(fs::make_preferred("LR2files/Config/white.bmp").data());
	sk->reloadbanner = 0;
	for (int i = 0; i < 10; i++) {
		InitSRC(&sk->src_BAR_RANK[i]);
		InitDST(&sk->dst_BAR_RANK[i]);
		InitSRC(&sk->src_BAR_RIVAL[i]);
		InitDST(&sk->dst_BAR_RIVAL[i]);
	}

	InitSRC(&sk->src_EVENT_MODE_CURSOR);
	for (int i = 0; i < 10; i++) {
		InitDST(&sk->dst_EVENT_MODE_CURSOR_ON[i]);
		InitDST(&sk->dst_EVENT_MODE_CURSOR_OFF[i]);
		sk->event_STARTINPUT[i]=0;
		sk->event_FADEOUT[i] = 0;
	}

	for (int i = 0; i < 5; i++) {
		InitSRC(&sk->src_BAR_STAR[i]);
		InitDST(&sk->dst_BAR_STAR[i]);
	}

	for (int i = 0; i < 5; i++) {
		InitDST(&sk->dst_EVENT_LOADINGBG[i]);
	}
	return 1;
}

//imagefont section
int InitImageFont(ImageFont *imgfont) {
	if (imgfont->size == 0) return 0;
	imgfont->size = 0;
	imgfont->kerning = 0;
	imgfont->filepath[0] = '\0';
	for (auto& [ch, chTex] : imgfont->chars) {
		DeleteGraph(chTex.grHandle);
		chTex.grHandle = -1;
	}
	for (int i = 0; i < 1000; i++) {
		DeleteGraph(imgfont->images[i].grHandle);
		imgfont->images[i].grHandle = -1;
		imgfont->images[i].filename[0] = '\0';
	}
	return 1;
}

int ReadImageFont(CSTR filename, ImageFont *imgfont) {
	CSTR str1;
	
	str1 = filename.getDirectory();

	if (strcmp(str1, imgfont->filepath)) {
		imgfont->size = 0;
		imgfont->kerning = 0;
		imgfont->filepath[0] = '\0';
		for (auto& [idx, chTex] : imgfont->chars) {
			DeleteGraph(chTex.grHandle);
			chTex.grHandle = -1;
		}
		for (int i = 0; i < 1000; i++) {
			DeleteGraph(imgfont->images[i].grHandle);
			imgfont->images[i].grHandle = -1;
			imgfont->images[i].filename[0] = '\0';
		}

		int f = FileRead_open(filename);
		if (f == 0) {
			ErrorLogFmtAdd("画像フォントファイル%sの読み込みに失敗しました\n",filename.body);
			return -1;
		}
		
		CSTR str2(256);
		CSVbuf csvBuf;
		strcpy(imgfont->filepath, str1);
		
		while (FileRead_gets(str2, 250, f) != -1) {
			if (*str2.atPos(0) == '#') {
				str2.trimWhiteSpace();
				DealWhiteSpace(&str2);
				SplitCSV(str2, &csvBuf, ",");
				if (*str2.atPos(1) == 'R') {
					char s_sjis[3]{ 0 };
					int chInt = csvBuf.val[1];
					if (chInt >= 0 && chInt <= 255)
					{
						// ID = ASCII
						s_sjis[0] = chInt & 0xFF;
					}
					else if (chInt >= 256 && chInt <= 8126)
					{
						// ID = Shift-JIS文字コードを10進数に変換して32832を引いた値
						int i = chInt + 32832;
						s_sjis[0] = (i >> 8) & 0xFF;
						s_sjis[1] = i & 0xFF;
					}
					else if (chInt >= 8127 && chInt <= 15306)
					{
						// ID = Shift-JIS文字コードを10進数に変換して49281を引いてた値
						int i = chInt + 49281;
						s_sjis[0] = (i >> 8) & 0xFF;
						s_sjis[1] = i & 0xFF;
					}
					std::u32string u32Ch = utf8_to_utf32(ansi2utf(s_sjis, 932));
					char32_t chIdx = u32Ch.front();
					auto& chTex = imgfont->chars[chIdx];
					chTex.srcX = csvBuf.val[3];
					chTex.srcY = csvBuf.val[4];
					chTex.width = csvBuf.val[5];
					chTex.height = csvBuf.val[6];
					chTex.ImageNum = csvBuf.val[2];
				}
				else if (*str2.atPos(1) == 'M') {
					imgfont->kerning = csvBuf.val[1];
				}
				else if (*str2.atPos(1) == 'S') {
					imgfont->size = csvBuf.val[1];
				}
				else if (*str2.atPos(1) == 'T') {
					strcpy(imgfont->images[csvBuf.val[1]].filename, csvBuf.str[2]);
				}
			}
			*str2.atPos(0) = '\0';
		}

		FileRead_close(f);

		LoadFontCharGraph(imgfont, U'?');
	}

	return 1;
}

int LoadFontGraph(ImageFont *imgfont, int fontNum) {
	if ( 0 <= fontNum && fontNum < 1000 && imgfont->images[fontNum].grHandle == -1 ) {
		CSTR str(imgfont->filepath, 0);
		str.add(imgfont->images[fontNum].filename);
		DeleteGraph(imgfont->images[fontNum].grHandle);
		imgfont->images[fontNum].grHandle = LoadGraph(str, 0);
		return 1;
	}
	return 0;
}

int LoadFontCharGraph(ImageFont *imgfont, char32_t vChar) {
	auto& fc = imgfont->chars[vChar];
	int fNum = fc.ImageNum;
	auto& fb = imgfont->images[fNum];
	if (0 <= fNum && fNum < 1000 && fc.grHandle == -1 && 0 < fc.height && 0 < fc.width) {
		if (fb.grHandle == -1) {
			LoadFontGraph(imgfont, fc.ImageNum);
			/* load failure check */
			if (fb.grHandle == -1) {
				return 0;
			}
		}
		fc.grHandle = DerivationGraph(fc.srcX, fc.srcY, fc.width, fc.height, fb.grHandle);
		return 1;
	}
	return 0;
}

int LoadFontForText(ImageFont *imgfont, CSTR *str){
	if (str->length() <= 0 /*|| imgfont->size == 0*/) return 0;
	std::u32string u32str = utf8_to_utf32(str->body);
	for (auto& ch : u32str) {
		auto& chTex = imgfont->chars[ch];
		if (chTex.grHandle == -1) {
			LoadFontCharGraph(imgfont, ch);
		}
	}
	return 1;
}

//2p graphic section
int FlipSide_Timer(int *n){
	int t = *n;
	switch (t) {
	case 0x2a:
		*n = 0x2b;
		return 1;
	case 0x2b:
		*n = 0x2a;
		return 1;
	case 0x2c:
		*n = 0x2d;
		return 1;
	case 0x2d:
		*n = 0x2c;
		return 1;
	case 0x2e:
		*n = 0x2f;
		return 1;
	case 0x2f:
		*n = 0x2e;
		return 1;
	case 0x30:
		*n = 0x31;
		return 1;
	case 0x31:
		*n = 0x30;
		return 1;
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x39:
	case 0x3a:
	case 0x3b:
	case 0x46:
	case 0x47:
	case 0x48:
	case 0x49:
	case 0x4a:
	case 0x4b:
	case 0x4c:
	case 0x4d:
	case 0x4e:
	case 0x4f:
	case 100:
	case 0x65:
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
	case 0x6a:
	case 0x6b:
	case 0x6c:
	case 0x6d:
	case 0x78:
	case 0x79:
	case 0x7a:
	case 0x7b:
	case 0x7c:
	case 0x7d:
	case 0x7e:
	case 0x7f:
	case 0x80:
	case 0x81:
		*n = t + 10;
		return 1;
	case 0x3c:
	case 0x3d:
	case 0x3e:
	case 0x3f:
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x50:
	case 0x51:
	case 0x52:
	case 0x53:
	case 0x54:
	case 0x55:
	case 0x56:
	case 0x57:
	case 0x58:
	case 0x59:
	case 0x6e:
	case 0x6f:
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
	case 0x74:
	case 0x75:
	case 0x76:
	case 0x77:
	case 0x82:
	case 0x83:
	case 0x84:
	case 0x85:
	case 0x86:
	case 0x87:
	case 0x88:
	case 0x89:
	case 0x8a:
	case 0x8b:
		*n = t + -10;
		return 1;
	case 0x8f:
		*n = 0x90;
		return 1;
	case 0x90:
		*n = 0x8f;
	}
	return 1;
}

int ApplyFlipside(skstruct *sk){
	int iVar2;
	SRCstruct srcTemp;
	DSTstruct dstTemp;

	memcpy(&srcTemp, &sk->src_GROOVEGAUGE[0], sizeof(SRCstruct));
	memcpy(&sk->src_GROOVEGAUGE[0], &sk->src_GROOVEGAUGE[1], sizeof(SRCstruct));
	memcpy(&sk->src_GROOVEGAUGE[1], &srcTemp, sizeof(SRCstruct));

	memcpy(&dstTemp, &sk->dst_GROOVEGAUGE[0], sizeof(DSTstruct));
	memcpy(&sk->dst_GROOVEGAUGE[0], &sk->dst_GROOVEGAUGE[1], sizeof(DSTstruct));
	memcpy(&sk->dst_GROOVEGAUGE[1], &dstTemp, sizeof(DSTstruct));
	
	for (int i = 0; i < 6; i++) {
		memcpy(&srcTemp, &sk->src_NOWJUDGE_1P[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOWJUDGE_1P[i], &sk->src_NOWJUDGE_2P[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOWJUDGE_2P[i], &srcTemp, sizeof(SRCstruct));

		memcpy(&dstTemp, &sk->dst_NOWJUDGE_1P[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOWJUDGE_1P[i], &sk->dst_NOWJUDGE_2P[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOWJUDGE_2P[i], &dstTemp, sizeof(DSTstruct));

		memcpy(&srcTemp, &sk->src_NOWCOMBO_1P[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOWCOMBO_1P[i], &sk->src_NOWCOMBO_2P[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOWCOMBO_2P[i], &srcTemp, sizeof(SRCstruct));

		memcpy(&dstTemp, &sk->dst_NOWCOMBO_1P[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOWCOMBO_1P[i], &sk->dst_NOWCOMBO_2P[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOWCOMBO_2P[i], &dstTemp, sizeof(DSTstruct));
	}

	for (int i = 0; i < 10; i++) {
		memcpy(&srcTemp, &sk->src_NOTE[i], sizeof(SRCstruct));
		memcpy(&sk->src_NOTE[i], &sk->src_NOTE[10+i], sizeof(SRCstruct));
		memcpy(&sk->src_NOTE[10+i], &srcTemp, sizeof(SRCstruct));

		memcpy(&dstTemp, &sk->dst_NOTE[i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOTE[i], &sk->dst_NOTE[10 + i], sizeof(DSTstruct));
		memcpy(&sk->dst_NOTE[10 + i], &dstTemp, sizeof(DSTstruct));

		memcpy(&srcTemp, &sk->src_LN_BODY[i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_BODY[i], &sk->src_LN_BODY[10 + i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_BODY[10 + i], &srcTemp, sizeof(SRCstruct));

		memcpy(&srcTemp, &sk->src_LN_START[i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_START[i], &sk->src_LN_START[10 + i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_START[10 + i], &srcTemp, sizeof(SRCstruct));

		memcpy(&srcTemp, &sk->src_LN_END[i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_END[i], &sk->src_LN_END[10 + i], sizeof(SRCstruct));
		memcpy(&sk->src_LN_END[10 + i], &srcTemp, sizeof(SRCstruct));
		//AdditionlTODO: need to add feature MINE, AUTO?
	}

	memcpy(&srcTemp, &sk->src_LINE[0], sizeof(SRCstruct));
	memcpy(&sk->src_LINE[0], &sk->src_LINE[1], sizeof(SRCstruct));
	memcpy(&sk->src_LINE[1], &srcTemp, sizeof(SRCstruct));

	memcpy(&dstTemp, &sk->dst_LINE[0], sizeof(DSTstruct));
	memcpy(&sk->dst_LINE[0], &sk->dst_LINE[1], sizeof(DSTstruct));
	memcpy(&sk->dst_LINE[1], &dstTemp, sizeof(DSTstruct));


	for (int i = 0; i < (sk->image).srcSize; i++) {
		FlipSide_Timer(&sk->image.src[i].timer);
		FlipSide_Timer(&sk->image.dst[i].timer);
		if (sk->image.dst[i].opt5 == 1) sk->image.dst[i].opt5 = 2;
		else if (sk->image.dst[i].opt5 == 2) sk->image.dst[i].opt5 = 1;
	}

	for (int i = 0; i < sk->otherObject[6].srcSize; i++) {
		//logic arragned
		int &op = sk->otherObject[6].src[i].op1;
		if (op < 100) {
			if (op == 10) op = 12;
			else if (op == 12) op = 10;
		}
		else if (op < 120) op += 20;
		else if (op > 119) op -= 20;
	}

	for (int i = 0; i < sk->otherObject[5].srcSize; i++) {
		int &op = sk->otherObject[5].src[i].op1;
		if (op == 10) op = 14;
		else if (op == 14) op = 10;
	}

	for (int i = 0; i < sk->otherObject[0].srcSize; i++) {
		int &op = sk->otherObject[0].src[i].st;
		if (op == 1) op = 2;
		else if (op == 2) op = 1;
	}

	iVar2 = sk->scratchside_1;
	sk->scratchside_1 = sk->scratchside_2;
	sk->scratchside_2 = iVar2;

	for (int i = 0; i < sk->otherObject[2].srcSize; i++) {
		int &op = sk->otherObject[2].src[i].op3;
		if (op == 4)op = 5;
		else if (op == 5) op = 4;
	}

	return 1;
}

int ClearSkinGraph(skstruct *sk){
	InitSkin(sk, 0, '\0');
	for (int i = 0; i < 200; i++) {
		sk->caption[i].fillzero();
		sk->GrHandle[i] = -1;
		sk->count = 0; //???
	}

	for (int i = 0; i < 10; i++) {
		InitImageFont(&sk->ImageFonts[i]);
	}
	return 1;
}

//maybe deleted by compiler, restored it for convenience
int ExpandSkinObjectMax(SkinObject *so, int add) {
	if (so->dstSize - 1 == so->srcSize) {
		so->src = (SRCstruct*)realloc(so->src, (so->dstSize + add) * sizeof(SRCstruct));
		assert(so->src != nullptr);
		so->dst = (DSTstruct*)realloc(so->dst, (so->dstSize + add) * sizeof(DSTstruct));
		assert(so->dst != nullptr);
		for (int i = so->dstSize; i < so->dstSize + add; i++) {
			memset(&so->src[i], 0, sizeof(SRCstruct));
			InitSRC(&so->src[i]);
			memset(&so->dst[i], 0, sizeof(DSTstruct));
			InitDST(&so->dst[i]);
		}
		so->dstSize += add;
	}
	return 1;
}

static void adjust_input_filepath(CSTR& path)
{
#ifndef _WIN32
	path.replace("\\", "/");
#endif // _WIN32
}

// ReadSkin // maybe unsatble
int ReadSkin(skstruct *sk,CSTR FilePath, int unused, int skin_num, SkinUser* sku, char flag_skipFont) {
	FILE *pFile;
	CSTR fBuf(1024);
	char* pFbuf;
	CSVbuf csv;
	int tSkin_num = 0;
	int line;
	int IFCOUNT, IFSWITCH[100];

	bool flipside = false;

	tSkin_num = skin_num;
	for (int i = 0; i < 100; i++) IFSWITCH[i] = 0;
	IFCOUNT = 0;
	sk->op[0] = 1;
	sk->op[999] = 0;
	ErrorLogFmtAdd("スキンの読み込みを開始します。 %s\n", FilePath.body);
	ErrorLogTabAdd();

	pFile = fopen(FilePath.body, "r");
	CSTR dir(FilePath.getDirectory());
	line = 0;

	if (!pFile) {
		ErrorLogFmtAdd("スキンの読み込みに失敗しました。 %s\n", FilePath.body);
		ErrorLogTabSub();
		return 0;
	}
	pFbuf = fBuf.outstr();
	for (pFbuf = fgets(pFbuf, 1023, pFile); pFbuf; pFbuf = fgets(pFbuf, 1023, pFile)) {
		fBuf = ansi2utf(pFbuf, 932).c_str();
		ExpandSkinObjectMax(&sk->image, 50);
		for (auto& obj : sk->otherObject) {
			ExpandSkinObjectMax(&obj, 20);
		}

		line++;

		if (fBuf.length() > 6) {
			if (*fBuf.atPos(0) == '#') {
				fBuf.nullAtPos(fBuf.length() - 1);
				fBuf.trimWhiteSpace();
				DealWhiteSpace(&fBuf);
				if (!fBuf.left(1).isDiff("#")) {
					if (fBuf.left(3).isSame("#IF")) {
						if (IFCOUNT != 99) {
							IFSWITCH[IFCOUNT + 1] = 1;
							SplitCSV(fBuf, &csv, ",");
							for (int i = 1; i < 10; i++) {
								if (csv.val[i] < 0 || csv.val[i]>999 || sk->op[csv.val[i]] == 0) {
									i = 10;
									IFSWITCH[IFCOUNT + 1] = 2;
								}
							}
							IFCOUNT++;
						}
						else {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nネスト可能な#IFの上限に達しました。\n", line, fBuf.body);
							if (IFSWITCH[IFCOUNT] > 1) {
								*fBuf.atPos(0) = '\0';
								continue;
							}
						}
					}
					else if (fBuf.left(7).isSame("#ELSEIF") && IFSWITCH[IFCOUNT] != 3) {
						if (IFCOUNT) {
							if (IFSWITCH[IFCOUNT] == 1) IFSWITCH[IFCOUNT] = 3;
							else {
								IFSWITCH[IFCOUNT] = 1;
								SplitCSV(fBuf, &csv, ",");
								for (int i = 1; i < 10; i++) {
									if (csv.val[i] < 0 || csv.val[i]>999 || sk->op[csv.val[i]] == 0) {
										i = 10;
										IFSWITCH[IFCOUNT] = 2;
									}
								}
							}
						}
						else ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n対応する#IFが見つかりません。\n", line, fBuf.body);
					}
					else if (fBuf.left(5).isSame("#ELSE") && IFSWITCH[IFCOUNT] != 3) {
						if (IFCOUNT == 0) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n対応する#IFが見つかりません。\n", line, fBuf.body);
						}
						else {
							IFSWITCH[IFCOUNT] = (IFSWITCH[IFCOUNT] == 1) ? 3 : 1;
						}
					}
					else if (fBuf.left(6).isSame("#ENDIF")) {
						if (IFCOUNT == 0) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n対応する#IFが見つかりません。\n", line, fBuf.body);
						}
						else {
							IFSWITCH[IFCOUNT] = 0;
							IFCOUNT--;
						}
					}
				}

				if(IFCOUNT > 0){
					if (IFSWITCH[IFCOUNT] > 1) {
						*fBuf.atPos(0) = '\0';
						continue;
					}
				}


				if (!fBuf.left(1).isDiff("#")) {
					if (fBuf.left(6).isSame("#IMAGE")) {
						if (sk->count == 100) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nこれ以上#IMAGEを登録できません。\n", line, fBuf.body);
						}
						else {
							SplitCSV(fBuf, &csv, ",");
							adjust_input_filepath(csv.str[1]);
							if (csv.str[1].isSame("CONTINUE")) {
								sk->caption[sk->count].assign("CONTINUE");
								sk->count++;
							}
							else {
								if (sk->caption[sk->count].isDiff(&csv.str[1]) || sk->grIsMovie[sk->count] == 1 || sk->caption[sk->count].findStrPos("*") != -1) {
									DeleteGraph(sk->GrHandle[sk->count]);
									sk->caption[sk->count].assign(&csv.str[1]);
									if (csv.str[1].right(3).isSame("mpg") || csv.str[1].right(3).isSame("avi") || csv.str[1].right(3).isSame("wma") || csv.str[1].right(3).isSame("ogv")) {
										sk->grIsMovie[sk->count] = 1;
									}
									else {
										sk->grIsMovie[sk->count] = 0;
									}
									for (int i = 0; i < sk->customfile_count; i++) {
										if (sk->customfileRANDOM[i].isSame(csv.str[1].left(sk->customfileRANDOM[i].length())) && sk->customfile[i].isDiff("RANDOM") != 0 && sk->customfile[i].isDiff("ERROR") && sk->customfile[i].length() > 0 ) {
											csv.str[1].replace("*", sk->customfile[i]);
											break;
										}
									}
									CSTR temp(GetRandomFileNoError(csv.str[1], dir), 0);
									sk->GrHandle[sk->count] = LoadGraph(temp);
									sk->caption[sk->count].assign(&temp);
								}
								sk->count++;
							}
						}
					}
					else if (fBuf.left(5).isSame("#FONT") && !flag_skipFont) {
						if (sk->num_of_struct == 10) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nこれ以上#FONTを登録できません。\n", line, fBuf.body);
						}
						else {
							SplitCSV(fBuf, &csv, ",");
							//sk->fontHandle[sk->num_of_struct] = CreateFontToHandle(sk->fontname, csv.val[1], csv.val[2], csv.val[3], 0, -1, 0, -1, -1); //DxLib3.02
							sk->fontHandle[sk->num_of_struct] = CreateFontToHandle(sk->fontname.body, csv.val[1], csv.val[2], csv.val[3], DX_CHARSET_UTF8, -1, 0, -1); //DxLib3.24f
							if (sk->fontHandle[sk->num_of_struct] == -1) {
								sk->fontHandle[sk->num_of_struct] = 0;
							}
							sk->num_of_struct = sk->num_of_struct + 1;
						}
					}
					else if (fBuf.left(10).isSame("#SRC_IMAGE")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->image.src[sk->image.srcSize], &csv, sk);
						if (sk->image.src[sk->image.srcSize].graphcount < 1 || sk->image.src[sk->image.srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf.body);
						}
						if (sk->image.srcSize > 0 && (sk->image.dst[sk->image.srcSize - 1].dstCount < 1 || sk->image.dst[sk->image.srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_IMAGEに対応した#DST_IMAGEが存在しないか、登録に失敗したようです\n", line, fBuf.body);
						}
						sk->image.srcSize++;
					}
					else if (fBuf.left(10).isSame("#DST_IMAGE") && sk->image.srcSize > 0) {
						int oldDstCount = sk->image.dst[sk->image.srcSize - 1].dstCount;
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->image.dst[sk->image.srcSize - 1], &csv, tSkin_num);
						if (sk->image.dst[sk->image.srcSize - 1].dstCount < 1 || sk->image.dst[sk->image.srcSize - 1].dataSize < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nDSTの登録に失敗しました。DSTの一番最初がエラーを起こしている可能性があります。\n", line, fBuf.body);
						}
						else if (sk->image.dst[sk->image.srcSize - 1].dstCount == oldDstCount){
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nDSTの登録に失敗しました。この行の登録のみ失敗しました。\n", line, fBuf.body);
						}
					}
					else if (fBuf.left(9).isSame("#SRC_TEXT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->otherObject[0].src[sk->otherObject[0].srcSize].n = csv.val[1];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].fontHandle = sk->fontHandle[csv.val[2]];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].cycle = csv.val[2];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].st = csv.val[3];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].align = csv.val[4];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op1 = csv.val[5];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op2 = csv.val[6];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op3 = csv.val[7];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op4 = csv.val[8];
						sk->otherObject[0].src[sk->otherObject[0].srcSize].op5 = csv.val[9];
						sk->otherObject[0].srcSize++;
					}
					else if (fBuf.left(9).isSame("#DST_TEXT") && sk->otherObject[0].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[0].dst[sk->otherObject[0].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(11).isSame("#SRC_SLIDER")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[2].src[sk->otherObject[2].srcSize], &csv, sk);
						if (sk->otherObject[2].src[sk->otherObject[2].srcSize].graphcount < 1 || sk->otherObject[2].src[sk->otherObject[2].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf.body);
						}
						if (sk->otherObject[2].srcSize > 0 && (sk->otherObject[2].dst[sk->otherObject[2].srcSize - 1].dstCount < 1 || sk->otherObject[2].dst[sk->otherObject[2].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_SLIDERに対応した#DST_SLIDERが存在しないか、登録に失敗したようです\n", line, fBuf.body);
						}
						sk->otherObject[2].srcSize++;
					}
					else if (fBuf.left(11).isSame("#DST_SLIDER") && sk->otherObject[2].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[2].dst[sk->otherObject[2].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(11).isSame("#SRC_BUTTON")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[1].src[sk->otherObject[1].srcSize], &csv, sk);
						if (sk->otherObject[1].src[sk->otherObject[1].srcSize].graphcount < 1 || sk->otherObject[1].src[sk->otherObject[1].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf.body);
						}
						if (sk->otherObject[1].srcSize > 0 && (sk->otherObject[1].dst[sk->otherObject[1].srcSize - 1].dstCount < 1 || sk->otherObject[1].dst[sk->otherObject[1].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_BUTTONに対応した#DST_BUTTONが存在しないか、登録に失敗したようです\n", line, fBuf.body);
						}
						sk->otherObject[1].srcSize++;
					}
					else if (fBuf.left(11).isSame("#DST_BUTTON") && sk->otherObject[1].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[1].dst[sk->otherObject[1].srcSize - 1], &csv, tSkin_num);

					}
					else if (fBuf.left(12).isSame("#SRC_ONMOUSE")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[3].src[sk->otherObject[3].srcSize], &csv, sk);
						if (sk->otherObject[3].src[sk->otherObject[3].srcSize].graphcount < 1 || sk->otherObject[3].src[sk->otherObject[3].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf.body);
						}
						if (sk->otherObject[3].srcSize > 0 && (sk->otherObject[3].dst[sk->otherObject[3].srcSize - 1].dstCount < 1 || sk->otherObject[3].dst[sk->otherObject[3].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_BGAに対応した#DST_BGAが存在しないか、登録に失敗したようです\n", line, fBuf.body);
						}
						sk->otherObject[3].srcSize++;
					}
					else if (fBuf.left(12).isSame("#DST_ONMOUSE") && sk->otherObject[3].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[3].dst[sk->otherObject[3].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(8).isSame("#SRC_BGA")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[4].src[sk->otherObject[4].srcSize], &csv, sk);
						if (sk->otherObject[4].src[sk->otherObject[4].srcSize].graphcount < 1 || sk->otherObject[4].src[sk->otherObject[4].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf.body);
						}
						if (sk->otherObject[4].srcSize > 0 && (sk->otherObject[4].dst[sk->otherObject[4].srcSize - 1].dstCount < 1 || sk->otherObject[4].dst[sk->otherObject[4].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではありません)ひとつ前の#SRC_BGAに対応した#DST_BGAが存在しないか、登録に失敗したようです\n", line, fBuf.body);
						}
						sk->otherObject[4].srcSize++;
					}
					else if (fBuf.left(8).isSame("#DST_BGA") && sk->otherObject[4].srcSize > 0) {
						SplitCSV(fBuf, &csv, ","); 
						ReadDST(&sk->otherObject[4].dst[sk->otherObject[4].srcSize - 1], &csv, tSkin_num);
						tSkin_num++;
					}
					else if (fBuf.left(11).isSame("#SRC_NUMBER")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[6].src[sk->otherObject[6].srcSize], &csv, sk);
						if (sk->otherObject[6].src[sk->otherObject[6].srcSize].graphcount < 1 || sk->otherObject[6].src[sk->otherObject[6].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しました。\n", line, fBuf.body);
						}
						if (sk->otherObject[6].srcSize > 0 && (sk->otherObject[6].dst[sk->otherObject[6].srcSize - 1].dstCount < 1 || sk->otherObject[6].dst[sk->otherObject[6].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではあ りません)ひとつ前の#SRC_NUMBERに対応した#DST_NUMBERが 存在しないか、登録に失敗したようです\n", line, fBuf.body);
						}
						sk->otherObject[6].srcSize++;
					}
					else if (fBuf.left(11).isSame("#DST_NUMBER") && sk->otherObject[6].srcSize > 0) {
						SplitCSV(fBuf, &csv, ","); 
						ReadDST(&sk->otherObject[6].dst[sk->otherObject[6].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(9).isSame("#SRC_MASK")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[7].src[sk->otherObject[7].srcSize], &csv, sk);
						sk->otherObject[7].srcSize++;
					}
					else if (fBuf.left(9).isSame("#DST_MASK") && sk->otherObject[7].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[7].dst[sk->otherObject[7].srcSize - 1], &csv, tSkin_num);
					}
					else if (fBuf.left(13).isSame("#SRC_BARGRAPH")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->otherObject[5].src[sk->otherObject[5].srcSize], &csv, sk);
						if (sk->otherObject[5].src[sk->otherObject[5].srcSize].graphcount < 1 || sk->otherObject[5].src[sk->otherObject[5].srcSize].count < 1) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n画像の登録に失敗しまし た。\n", line, fBuf.body);
						}
						if (sk->otherObject[5].srcSize > 0 && (sk->otherObject[5].dst[sk->otherObject[5].srcSize - 1].dstCount < 1 || sk->otherObject[5].dst[sk->otherObject[5].srcSize - 1].dataSize < 1)) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n(この行のエラーではあ りません)ひとつ前の#SRC_BARGRAPHに対応した#DST_BARGRAP Hが存在しないか、登録に失敗したようです\n", line, fBuf.body);
						}
						sk->otherObject[5].srcSize++;
					}
					else if (fBuf.left(13).isSame("#DST_BARGRAPH") && sk->otherObject[5].srcSize > 0) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->otherObject[5].dst[sk->otherObject[5].srcSize - 1], &csv, tSkin_num);
						tSkin_num++;
					}
					else if (fBuf.left(13).isSame("#SRC_BAR_BODY")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 29, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_BODY[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(17).isSame("#DST_BAR_BODY_OFF")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 29, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_BODY_OFF[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(16).isSame("#DST_BAR_BODY_ON")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 29, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_BODY_ON[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(11).isSame("#BAR_CENTER")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 29, line, pFbuf)) {
							SplitCSV(fBuf, &csv, ",");
							sk->BAR_CENTER = csv.val[1];
						}
					}
					else if (fBuf.left(14).isSame("#SRC_BAR_TITLE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 4, line, pFbuf)) {
							ReadSRC_BAR_TITLE(&sk->src_BAR_TITLE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#DST_BAR_TITLE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 4, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_TITLE[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(11).isSame("#TRANSCOLOR")) {
						SplitCSV(fBuf, &csv, ",");
						SetTransColor(csv.val[1], csv.val[2], csv.val[3]);
					}
					else if (fBuf.left(11).isSame("#TRANSCLOLR")) {
						SplitCSV(fBuf, &csv, ",");
						SetTransColor(csv.val[1], csv.val[2], csv.val[3]);
					}
					else if (fBuf.left(12).isSame("#TRANSCLOLOR")) {
						SplitCSV(fBuf, &csv, ",");
						SetTransColor(csv.val[1], csv.val[2],csv.val[3]);
					}
					else if (fBuf.left(11).isSame("#STARTINPUT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->startinput_start = csv.val[1];
						sk->startinput_rank = csv.val[2];
						sk->startinput_update = csv.val[3];
					}
					else if (fBuf.left(10).isSame("#SCENETIME")) {
						SplitCSV(fBuf, &csv, ",");
						sk->scenetime = csv.val[1];
					}
					else if (fBuf.left(8).isSame("#FADEOUT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->fadeout = csv.val[1];
					}
					else if (fBuf.left(6).isSame("#CLOSE")) {
						SplitCSV(fBuf, &csv, ",");
						sk->close = csv.val[1];
					}
					else if (fBuf.left(5).isSame("#SKIP")) {
						SplitCSV(fBuf, &csv, ",");
						sk->close = csv.val[1];
					}
					else if (fBuf.left(10).isSame("#PLAYSTART")) {
						SplitCSV(fBuf, &csv, ",");
						sk->playstart = csv.val[1];
					}
					else if (fBuf.left(10).isSame("#LOADSTART")) {
						SplitCSV(fBuf, &csv, ",");
						sk->loadstart = csv.val[1];
					}
					else if (fBuf.left(8).isSame("#LOADEND")) {
						SplitCSV(fBuf, &csv, ",");
						sk->loadend = csv.val[1];
					}
					else if (fBuf.left(14).isSame("#BAR_AVAILABLE")) {
						SplitCSV(fBuf, &csv, ",");
						sk->bar_availabe_from = csv.val[1];
						sk->bar_availabe_to = csv.val[2];
					}
					else if (fBuf.left(14).isSame("#SRC_BAR_FLASH")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_BAR_FLASH, &csv, sk);
					}
					else if (fBuf.left(14).isSame("#DST_BAR_FLASH")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_BAR_FLASH, &csv, tSkin_num);
					}
					else if (fBuf.left(18).isSame("#DST_BAR_STAGEFILE")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_BAR_STAGEFILE, &csv, tSkin_num);
					}
					else if (fBuf.left(16).isSame("#SRC_MOUSECURSOR")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_MOUSECURSOR, &csv, sk);
					}
					else if (fBuf.left(16).isSame("#DST_MOUSECURSOR")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_MOUSECURSOR, &csv, tSkin_num);
					}
					else if (fBuf.left(14).isSame("#SRC_BAR_LEVEL")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 10, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_LEVEL[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#DST_BAR_LEVEL")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 10, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_LEVEL[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(9).isSame("#SRC_NOTE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_NOTE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(9).isSame("#SRC_MINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_MINE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(13).isSame("#SRC_LN_START")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_LN_START[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(11).isSame("#SRC_LN_END")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_LN_END[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(12).isSame("#SRC_LN_BODY")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_LN_BODY[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_AUTO_NOTE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_NOTE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_AUTO_MINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_MINE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(18).isSame("#SRC_AUTO_LN_START")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_LN_START[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(16).isSame("#SRC_AUTO_LN_END")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_LN_END[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(17).isSame("#SRC_AUTO_LN_BODY")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadSRC(&sk->src_AUTO_LN_BODY[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(9).isSame("#DST_NOTE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 19, line, pFbuf)) {
							ReadDST(&sk->dst_NOTE[csv.val[1]], &csv, tSkin_num);
							tSkin_num += 2;
						}
					}
					else if (fBuf.left(16).isSame("#SRC_NOWJUDGE_1P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadSRC(&sk->src_NOWJUDGE_1P[csv.val[1]], &csv, sk);
							sk->src_NOWJUDGE_1P[csv.val[1]].timer = 46;
						}
					}
					else if (fBuf.left(16).isSame("#DST_NOWJUDGE_1P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadDST(&sk->dst_NOWJUDGE_1P[csv.val[1]], &csv, tSkin_num);
							sk->dst_NOWJUDGE_1P[csv.val[1]].timer = 46;
						}
					}
					else if (fBuf.left(16).isSame("#SRC_NOWCOMBO_1P")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_NOWCOMBO_1P[csv.val[1]], &csv, sk);
						sk->src_NOWCOMBO_1P[csv.val[1]].timer = 46;
					}
					else if (fBuf.left(16).isSame("#DST_NOWCOMBO_1P")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_NOWCOMBO_1P[csv.val[1]], &csv, tSkin_num);
						sk->dst_NOWJUDGE_1P[csv.val[1]].timer = 46; //???mistake?
					}
					else if (fBuf.left(16).isSame("#SRC_NOWJUDGE_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadSRC(&sk->src_NOWJUDGE_2P[csv.val[1]], &csv, sk);
							sk->src_NOWJUDGE_2P[csv.val[1]].timer = 47;
						}
					}
					else if (fBuf.left(16).isSame("#DST_NOWJUDGE_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadDST(&sk->dst_NOWJUDGE_2P[csv.val[1]], &csv, tSkin_num);
							sk->dst_NOWJUDGE_2P[csv.val[1]].timer = 47;
						}
					}
					else if (fBuf.left(16).isSame("#SRC_NOWCOMBO_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadSRC(&sk->src_NOWCOMBO_2P[csv.val[1]], &csv, sk);
							sk->src_NOWCOMBO_2P[csv.val[1]].timer = 47;
						}
					}
					else if (fBuf.left(16).isSame("#DST_NOWCOMBO_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadDST(&sk->dst_NOWCOMBO_2P[csv.val[1]], &csv, tSkin_num);
							sk->dst_NOWJUDGE_2P[csv.val[1]].timer = 47; //???mistake?
						}
					}
					else if (fBuf.left(16).isSame("#SRC_GROOVEGAUGE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_GROOVEGAUGE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(16).isSame("#DST_GROOVEGAUGE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_GROOVEGAUGE[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(18).isSame("#SRC_GAUGECHART_1P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_GAUGECHART_1P[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(18).isSame("#DST_GAUGECHART_1P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_GAUGECHART_1P[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(18).isSame("#SRC_GAUGECHART_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_GAUGECHART_2P[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(18).isSame("#DST_GAUGECHART_2P")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_GAUGECHART_2P[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(15).isSame("#SRC_SCORECHART")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 2, line, pFbuf)) {
							ReadSRC(&sk->src_SCORECHART[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(15).isSame("#DST_SCORECHART")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 2, line, pFbuf)) {
							ReadDST(&sk->dst_SCORECHART[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(9).isSame("#SRC_LINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_LINE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(9).isSame("#DST_LINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_LINE[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_JUDGELINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC(&sk->src_JUDGELINE[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#DST_JUDGELINE")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_JUDGELINE[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(13).isSame("#SRC_BAR_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_LAMP[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(13).isSame("#DST_BAR_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_LAMP[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(16).isSame("#SRC_BAR_MY_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_MY_LAMP[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(16).isSame("#DST_BAR_MY_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_MY_LAMP[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(19).isSame("#SRC_BAR_RIVAL_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_RIVAL_LAMP[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(19).isSame("#DST_BAR_RIVAL_LAMP")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_RIVAL_LAMP[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(13).isSame("#SRC_BAR_STAR")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_STAR[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(13).isSame("#DST_BAR_STAR")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 5, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_STAR[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_THUMBNAIL")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_THUMBNAIL, &csv, sk);
					}
					else if (fBuf.left(14).isSame("#DST_THUMBNAIL")) {
						SplitCSV(fBuf, &csv, ",");
						ReadDST(&sk->dst_THUMBNAIL, &csv, tSkin_num);
					}
					else if (fBuf.left(11).isSame("#SRC_README")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadSRC_BAR_TITLE(&sk->src_README[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(11).isSame("#DST_README")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 1, line, pFbuf)) {
							ReadDST(&sk->dst_README[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(20).isSame("#DST_EVENT_LOADINGBG")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 4, line, pFbuf)) {
							ReadDST(&sk->dst_EVENT_LOADINGBG[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(22).isSame("#SRC_EVENT_MODE_CURSOR")) {
						SplitCSV(fBuf, &csv, ",");
						ReadSRC(&sk->src_EVENT_MODE_CURSOR, &csv, sk);
					}
					else if (fBuf.left(25).isSame("#DST_EVENT_MODE_CURSOR_ON")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 10, line, pFbuf)) {
							ReadDST(&sk->dst_EVENT_MODE_CURSOR_ON[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(26).isSame("#DST_EVENT_MODE_CURSOR_OFF")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 10, line, pFbuf)) {
							ReadDST(&sk->dst_EVENT_MODE_CURSOR_OFF[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(17).isSame("#EVENT_STARTINPUT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->event_STARTINPUT[0] = csv.val[1];
						sk->event_STARTINPUT[1] = csv.val[2];
						sk->event_STARTINPUT[2] = csv.val[3];
						sk->event_STARTINPUT[3] = csv.val[4];
						sk->event_STARTINPUT[4] = csv.val[5];
						sk->event_STARTINPUT[5] = csv.val[6];
						sk->event_STARTINPUT[6] = csv.val[7];
						sk->event_STARTINPUT[7] = csv.val[8];
						sk->event_STARTINPUT[8] = csv.val[9];
						sk->event_STARTINPUT[9] = csv.val[10];
					}
					else if (fBuf.left(14).isSame("#EVENT_FADEOUT")) {
						SplitCSV(fBuf, &csv, ",");
						sk->event_FADEOUT[0] = csv.val[1];
						sk->event_FADEOUT[1] = csv.val[2];
						sk->event_FADEOUT[2] = csv.val[3];
						sk->event_FADEOUT[3] = csv.val[4];
						sk->event_FADEOUT[4] = csv.val[5];
						sk->event_FADEOUT[5] = csv.val[6];
						sk->event_FADEOUT[6] = csv.val[7];
						sk->event_FADEOUT[7] = csv.val[8];
						sk->event_FADEOUT[8] = csv.val[9];
						sk->event_FADEOUT[9] = csv.val[10];
					}
					else if (fBuf.left(8).isSame("#LR2FONT") && !flag_skipFont) {
						SplitCSV(fBuf, &csv, ",");
						adjust_input_filepath(csv.str[1]);
						if (sk->num_of_ImageFont == 10) {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\nこれ以上の登録はできません。\n", line, fBuf.body);
						}
						else if (csv.val[2] == 1 || sk->disableimagefont == 0) {
							if (csv.str[1].isDiff("CONTINUE")) {
								for (int i = 0; i < sk->customfile_count; i++) {
									if(sk->customfileRANDOM[i].isSame(csv.str[1].left(sk->customfileRANDOM[i].length())) && sk->customfile[i].isDiff("RANDOM") && sk->customfile[i].isDiff("ERROR") && sk->customfile[i].length() > 0){
										csv.str[1].replace("*", sk->customfile[i]);
										//line?
										break;
									}
								}
								ReadImageFont(GetRandomFileNoError(csv.str[1], dir), &sk->ImageFonts[sk->num_of_ImageFont]);
							}
							sk->num_of_ImageFont++;
						}
						else {
							InitImageFont(&sk->ImageFonts[sk->num_of_ImageFont]);
							sk->num_of_ImageFont++;
						}
					}
					else if (fBuf.left(9).isSame("#HELPFILE")) {
						adjust_input_filepath(csv.str[1]);
						SplitCSV(fBuf, &csv, ",");
						if (sk->helpfileCount < 10) {
							sk->helpfilePath[sk->helpfileCount].assign(&csv.str[1]);
							sk->helpfileCount = sk->helpfileCount + 1;
						}
					}
					else if (fBuf.left(6).isSame("#NOBGA")) {
						sk->flag_BGA = 0;
					}
					else if (fBuf.left(11).isSame("#FLIPRESULT")) {
						sk->flag_flip = true;
						sk->op[350] = false;
						sk->op[351] = true;
					}
					else if (fBuf.left(9).isSame("#FLIPSIDE")) {
						flipside = true;
					}
					else if (fBuf.left(12).isSame("#DISABLEFLIP")) {
						sk->flag_flip = false;
						sk->op[350] = true;
						sk->op[351] = false;
					}
					else if (fBuf.left(11).isSame("#TEXTMERGIN")) {
						SplitCSV(fBuf, &csv, ",");
						sk->textmergin_1 = csv.val[1];
						sk->textmergin_2 = csv.val[2];
					}
					else if (fBuf.left(12).isSame("#SCRATCHSIDE")) {
						SplitCSV(fBuf, &csv, ",");
						sk->scratchside_1 = csv.val[1];
						sk->scratchside_2 = csv.val[2];
					}
					else if (fBuf.left(8).isSame("#INCLUDE")) {
						SplitCSV(fBuf, &csv, ",");
						adjust_input_filepath(csv.str[1]);
						for (int i = 0; i < sk->customfile_count; i++) {
							if (sk->customfileRANDOM[i].isSame(csv.str[1].left(sk->customfileRANDOM[i].length()))
								&& sk->customfile[i].isDiff("RANDOM") && sk->customfile[i].isDiff("ERROR")
								&& (sk->customfile[i].length() > 0)) {
								
								csv.str[1].replace("*", sk->customfile[i]);
								break;
							}
						}
						if (tSkin_num == 0) tSkin_num = 1;
						tSkin_num += ReadSkin(sk, GetRandomFileNoError(csv.str[1], dir), unused, tSkin_num, sku, flag_skipFont);
					}
					else if (fBuf.left(13).isSame("#CUSTOMOPTION")) {
						sk->customfile_count++;
					}
					else if (fBuf.left(11).isSame("#CUSTOMFILE")) {
						SplitCSV(fBuf, &csv, ",");
						adjust_input_filepath(csv.str[2]);
						sk->customfileRANDOM[sk->customfile_count].assign(&csv.str[2]);
						sk->customfile[sk->customfile_count].assign(&sku->customize_filename[sk->customfile_count]);
						if (sk->customfile[sk->customfile_count].isSame("RANDOM")) {
							sk->customfile[sk->customfile_count].assign( GetRandomFile(sk->customfileRANDOM[sk->customfile_count], 1) );
						}
						sk->customfile_count++;
					}
					else if (fBuf.left(13).isSame("#CUSTOMFOLDER")) {
						SplitCSV(fBuf, &csv, ",");
						adjust_input_filepath(csv.str[2]);
						sk->customfileRANDOM[sk->customfile_count].assign(&csv.str[2]);
						sk->customfile[sk->customfile_count].assign(&sku->customize_filename[sk->customfile_count]);
						sk->customfile_count++;
					}
					else if (fBuf.left(13).isSame("#RELOADBANNER")) {
						sk->reloadbanner = 1;
					}
					else if (fBuf.left(10).isSame("#SETOPTION")) {
						SplitCSV(fBuf, &csv, ",");
						if (csv.val[1] < 1000) {
							sk->op[csv.val[1]] = (csv.val[2] != 0);
						}
						else {
							ErrorLogFmtAdd("スキン読み込みエラー %d行目\n%s\n#SETOPTIONの第一引数(オプション値)は900～999の範囲内にして下さい。\n", line, fBuf.body);
						}
					}
					else if (fBuf.left(13).isSame("#SRC_BAR_RANK")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_RANK[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(13).isSame("#DST_BAR_RANK")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_RANK[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(14).isSame("#SRC_BAR_RIVAL")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadSRC(&sk->src_BAR_RIVAL[csv.val[1]], &csv, sk);
						}
					}
					else if (fBuf.left(14).isSame("#DST_BAR_RIVAL")) {
						SplitCSV(fBuf, &csv, ",");
						if (CheckIndexRange(csv.val[1], 0, 9, line, pFbuf)) {
							ReadDST(&sk->dst_BAR_RIVAL[csv.val[1]], &csv, tSkin_num);
						}
					}
					else if (fBuf.left(11).isSame("#HORIZONTAL")) {
						sk->horizontal = 1;
					}
				}
				tSkin_num++;
			}
		}
		*fBuf.atPos(0) = '\0';
	}
	fclose(pFile);
	if (skin_num == 0) {
		for (int i = sk->count; i < 100; i++) {
			DeleteGraph(sk->GrHandle[i]);
			sk->GrHandle[i] = -1;
			sk->caption[i].fillzero();
		}
		ProcessMessage();
		for (int i = 0; i < sk->count; i++) {
			if (sk->grIsMovie[i]) {
				SeekMovieToGraph(sk->GrHandle[i], 0);
				PlayMovieToGraph(sk->GrHandle[i], 3, 0);
				SetMovieVolumeToGraph(0, sk->GrHandle[i]);
			}
		}
		ErrorLogFmtAdd("スキンの読み込みに成功しました。 %s\n", FilePath.body);
		ErrorLogTabSub();
		SetTransColor(0, 0xff, 0);
		if (flipside != '\0') {
			ApplyFlipside(sk);
		}
		return 1;
	}
	ErrorLogFmtAdd("子スキンの読み込みに成功しました。 %s\n", FilePath.body);
	ErrorLogTabSub();
	return tSkin_num;
}

// LoadScene
int LoadScene(skstruct* sk, CSTR skinfile, int p5, char font) {
	SkinUser tsku;
	CSTR tStr;
	InitSkin(sk, p5, font);
	sk->skinMD5.assign(MD5str(skinfile));
	cstrSprintf(&tStr, fs::make_preferred("LR2files/SkinCustomize/%s.xml").data(),sk->skinMD5.body);
	ReadSkinCustomize(&tsku, tStr);
	(sk->adjust).shift_x = tsku.adjust.shift_x;
	(sk->adjust).shift_y = tsku.adjust.shift_y;
	(sk->adjust).rate_x = tsku.adjust.rate_x;
	(sk->adjust).rate_y = tsku.adjust.rate_y;
	(sk->adjust).judge_x = tsku.adjust.judge_x;
	(sk->adjust).judge_y = tsku.adjust.judge_y;
	(sk->adjust).size_x = tsku.adjust.size_x;
	(sk->adjust).size_y = tsku.adjust.size_y;
	(sk->adjust).dark_type = tsku.adjust.dark_type;
	(sk->adjust).note_1p_x = tsku.adjust.note_1p_x;
	(sk->adjust).note_1p_y = tsku.adjust.note_1p_y;
	(sk->adjust).note_2p_x = tsku.adjust.note_2p_x;
	(sk->adjust).note_2p_y = tsku.adjust.note_2p_y;
	for (int i = 0; i < 40; i++) {
		int t = tsku.customize_value[i];
		if (900 <= t && t < 1000) sk->op[t] = 1;
	}
	return ReadSkin(sk, skinfile, p5, 0, &tsku, font);
}
int LoadSceneG(game* g, skstruct* sk, int skinNum, int font) {
	CSTR skinfile(g->config.skin.skinFilePath[skinNum]);

	Resize(g, g->skinData.Data[g->skinData.skinID[skinNum]].targetX, g->skinData.Data[g->skinData.skinID[skinNum]].targetY, 0);
	
	LoadScene(sk, skinfile, g->skinData.Data[g->skinData.skinID[skinNum]].informationP5, font);
	return 0;
}

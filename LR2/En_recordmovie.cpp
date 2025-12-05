#include "En_recordmovie.h"
#include "filesystem.h"
#include <cstring>

#include "structure.h"
#include "filesystem.h"
#include "En_fileutil.h"

#include <DxLib/DxLib.h>

#ifdef _WIN32
#pragma comment(lib,"vfw32.lib")
 #include <windows.h>
 #include <vfw.h>
#endif

RECORDING::RECORDING() {
	memset(this, 0, sizeof(RECORDING)); // FIXME: bad memset
}

bool RECORDING::RefreshCurFrame() {
	this->curFrame = this->writeSamplePos;
	return true;
}

int RECORDING::GetCurTime() {
	return this->curFrame * 1000 / this->framerate;
}

int RECORDING::CpyScreenToAVI() {
#ifdef _WIN32
	this->srcHDC = GetDC(this->hwnd);
	BitBlt(this->dstHDC, 0, 0, this->bmiHeader.biWidth, this->bmiHeader.biHeight, this->srcHDC, 0, 0, SRCCOPY);
	ReleaseDC(this->hwnd, this->srcHDC);
	return AVIStreamWrite(this->pAVIstream, this->writeSamplePos++, 1, this->buf, this->bmiHeader.biSizeImage, AVIIF_KEYFRAME, 0, 0); //watch out that ++
#else
	return {};
#endif // _WIN32
}

bool RECORDING::Release() {
#ifdef _WIN32
	if (this->compvars.hic) {
		ICCompressorFree(&this->compvars);
		this->compvars.hic = NULL;
	}

	if (this->pAVIstream) {
		AVIStreamRelease(this->pAVIstream);
		this->pAVIstream = NULL;
	}

	if (this->pAVIFILE) {
		AVIFileRelease(this->pAVIFILE);
		this->pAVIFILE = NULL;
	}
	return true;
#else
	return {};
#endif // _WIN32
}

#ifdef _WIN32
static int REC_CpyAVIStreamToFile(PAVIFILE pfile, PAVISTREAM pavi, int /*unused*/) {

	unsigned int lpos;
	AVISTREAMINFOA si;
	PAVISTREAM pNewavi;
	long sz;
	long samples;
	LPVOID lpFormat;
	int totalLength;

	lpos = AVIStreamStart(pavi);
	totalLength = lpos + AVIStreamLength(pavi) - 1;

	if (AVIStreamInfoA(pavi, &si, sizeof(AVISTREAMINFOA)) != 0) return 0;
	if (AVIFileCreateStreamA(pfile, &pNewavi, &si) != 0) return 0;
	if (AVIStreamReadFormat(pavi, lpos, NULL, &sz) != 0) {
		AVIStreamRelease(pNewavi);
		return 0;
	}

	lpFormat = malloc(sz);
	if (lpFormat == NULL) {
		AVIStreamRelease(pNewavi);
		return 0;
	}

	if (AVIStreamReadFormat(pavi, lpos, lpFormat, &sz) == 0 && AVIStreamSetFormat(pNewavi, lpos, lpFormat, sz) == 0) {
		for (; lpos <= totalLength; lpos += samples) {
			if (!(AVIStreamRead(pavi, lpos, -1, NULL, 0, &sz, &samples) == 0 && sz > 0 && samples > 0)) break;
			if (AVIStreamRead(pavi, lpos, 4096, NULL, 0, &sz, &samples) != 0) break;
			auto* format = realloc(lpFormat, sz);
			if (format == nullptr) break;
			lpFormat = format;
			if (AVIStreamRead(pavi, lpos, samples, lpFormat, sz, &sz, &samples) != 0) break;
			if (AVIStreamWrite(pNewavi, lpos, samples, lpFormat, sz, 0, NULL, NULL) != 0) break;
		}
	}

	AVIStreamRelease(pNewavi);
	free(lpFormat);
	return 0;
}

static int CreateStream(CSTR filename, int framerate, COMPVARS *compvars, BITMAPINFOHEADER* lpbmi, PAVIFILE* pAVIFILE, PAVISTREAM* pAVIstream) {

	IAVIFile* pFile;
	AVISTREAMINFOA si;
	PAVISTREAM pAviCompressed;
	PAVISTREAM pavi;
	AVICOMPRESSOPTIONS options;

	if (AVIFileOpenA(&pFile, filename, OF_CREATE | OF_WRITE, 0) != 0) {
		MessageBoxA(NULL, "ファイルの作成またはオープンに失敗しました。", NULL, MB_ICONEXCLAMATION);
		return 0;
	}
	memset(&si, 0, sizeof(AVISTREAMINFOA));
	si.fccHandler = compvars->fccHandler;
	si.dwRate = framerate;
	si.fccType = 0x73646976; //'vids'
	si.dwScale = 1;
	SetRect(&si.rcFrame, 0, 0, lpbmi->biWidth, lpbmi->biHeight);

	if (AVIFileCreateStreamA(pFile, &pavi, &si) != 0) {
		MessageBoxA(NULL, "ストリームの作成に失敗しました。", NULL, MB_ICONEXCLAMATION);
		AVIFileRelease(pFile);
		return 0;
	}

	options.dwKeyFrameEvery = compvars->lKey;
	options.fccHandler = compvars->fccHandler;
	options.dwQuality = compvars->lQ;
	options.dwBytesPerSecond = compvars->lDataRate;
	options.fccType = 0x73646976; //'vids'
	options.dwFlags = AVICOMPRESSF_KEYFRAMES | AVICOMPRESSF_DATARATE;
	options.lpFormat = NULL;
	options.cbFormat = 0;
	options.lpParms = NULL;
	options.cbParms = 0;
	options.dwInterleaveEvery = 0;

	if (AVIMakeCompressedStream(&pAviCompressed, pavi, &options, NULL) != 0) {
		AVIStreamRelease(pavi);
		AVIFileRelease(pFile);
		return 0;
	}

	AVIStreamSetFormat(pAviCompressed, 0, lpbmi, sizeof(BITMAPINFOHEADER));
	*pAVIFILE = pFile;
	*pAVIstream = pAviCompressed;
	AVIStreamRelease(pavi);
	return 1;
}
#endif // _WIN32

bool RECORDING::PrepareAVIRecord(double framerate, int bit, CSTR filename, uint frameLen, HWND hwnd) {
#ifdef _WIN32
	tagRECT cRect;

	this->framerate = framerate;
	this->bitdepth = bit;
	this->filename = filename;
	this->framelen = frameLen;
	this->writeSamplePos = 0;
	this->curFrame = 0;

	remove(filename);
	CSTR dat(fs::make_preferred("LR2files/Config/compvars.dat").data());

	if (dat.canOpenFile()) {
		FILE* pFile = fopen(dat, "rb");
		fread(&this->compvars, sizeof(COMPVARS), 1, pFile);
		fclose(pFile);
	}
	else {
		SetMouseDispFlag(1);
		ProcessMessage();
		memset(&this->compvars, 0, sizeof(COMPVARS));
		this->compvars.cbSize = 64;
		if (ICCompressorChoose(NULL,0,NULL,NULL,&this->compvars,NULL) == 0) return false;
		FILE *pFile = fopen(fs::make_preferred("LR2files/Config/compvars.dat").data(), "wb");
		fwrite(&this->compvars, sizeof(COMPVARS), 1, pFile);
		fclose(pFile);
		MessageBoxA(NULL, "圧縮設定が保存されました。\n設定の変更はJUKEBOXタブ→詳細設定→動画圧縮設定で行えます。", "報告", 0);
		SetMouseDispFlag(0);
	}

	if (hwnd == NULL) hwnd = GetDesktopWindow();
	this->hwnd = hwnd;
	GetClientRect(this->hwnd, &cRect);
	this->bmiHeader.biWidth = 0;
	this->bmiHeader.biHeight = 0;
	this->bmiHeader.biPlanes = 0;
	this->bmiHeader.biBitCount = 0;
	this->bmiHeader.biCompression = 0;
	this->bmiHeader.biSizeImage = 0;
	this->bmiHeader.biXPelsPerMeter = 0;
	this->bmiHeader.biYPelsPerMeter = 0;
	this->bmiHeader.biClrUsed = 0;
	this->bmiHeader.biClrImportant = 0;
	
	this->bmiHeader.biWidth = cRect.right;
	this->bmiHeader.biPlanes = 1;
	this->bmiHeader.biBitCount = 0x18;
	this->bmiHeader.biSizeImage = (cRect.right + 1) * 3 * cRect.bottom * 4;
	this->bmiHeader.biHeight = cRect.bottom;
	this->bmiHeader.biSize = 40;
	this->bmiHeader.biCompression = 0;
	CreateStream(filename, framerate, &this->compvars, &this->bmiHeader, &this->pAVIFILE, &this->pAVIstream);
	this->dstHDC = CreateCompatibleDC(NULL);
	this->hBIT = CreateDIBSection(NULL, (BITMAPINFO*)&this->bmiHeader, 0, &this->buf, NULL, 0); //it needs bitmapInfo(44bytes), not bitmapInfoHeader(40bytes)
	this->hGDI = SelectObject(this->dstHDC, this->hBIT);
	return 1;
#else
	return {};
#endif // _WIN32
}

int RECORDING::InsertAudioToMovie(CSTR pathAudio, bool deleteFlag) {
#ifdef _WIN32
	CSTR path(pathAudio);
	bool isMp3 = path.lower().right(3).isSame("mp3");

	if (isMp3) {
		path = path.left(path.length() - 3);
		path.add("wav");
		Mp3toWavP(pathAudio, path);
		pathAudio = path;
	}

	if (this->compvars.hic) {
		ICCompressorFree(&this->compvars);
		this->compvars.hic = NULL;
	}
	if (this->pAVIstream) {
		AVIStreamRelease(this->pAVIstream);
		this->pAVIstream = NULL;
	}

	if (AVIStreamOpenFromFileA(&this->pAVIstream, pathAudio, 0x73647561, 0, OF_SHARE_DENY_NONE, NULL) != 0) { //'auds'
		MessageBoxA(NULL, "オーディオファイルの読み込みに失敗しました", "エラー", 0);
	}
	else {
		if (REC_CpyAVIStreamToFile(this->pAVIFILE, this->pAVIstream, this->curFrame) == 0) {
			MessageBoxA(NULL, "ストリーム追加に失敗しました", "エラー", 0);
		}
		AVIStreamRelease(this->pAVIstream);
		this->pAVIstream = NULL;
	}

	if (isMp3) remove(path);
	if (deleteFlag) remove(pathAudio);

	return 1;
#else
	return {}; // TODO(linux): stub
#endif // _WIN32
}

static int REC_COPYFILE(FILE *oFile, FILE *iFile, uint size){
	void *buf;
	uint bufSize;
	uint _Count;

	bufSize = size;
	if (size > 0x100000) {
		do {
			bufSize >>= 1;
		} while (bufSize > 0x100000);
	}

	buf = malloc(bufSize);
	while (buf == NULL) {
		bufSize >>= 1;
		if (bufSize < 0x400) {
			ErrorLogAdd("コピー用バッファが確保できません。\n");
			return 0;
		}
		buf = malloc(bufSize);
	}

	while (size != 0) {
		_Count = bufSize;
		if (size <= bufSize) {
			_Count = size;
		}
		if (fread(buf, 1, _Count, iFile) != _Count) {
			ErrorLogAdd("ファイルが読めません。\n");
			return 0;
		}
		if (fwrite(buf, 1, _Count, oFile) != _Count) {
			ErrorLogAdd("ファイルに書けません。\n");
			return 0;
		}
		size -= _Count;
	}

	if (fflush(oFile) == 0) {
		free(buf);
		return 1;
	}

	ErrorLogAdd("ファイルに書けません。\n");
	return 0;
}

char WAVhead[70] = {0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20, 0x1e, 0x00, 0x00, 0x00, 0x55, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x71, 0x05, 0x66, 0x61, 0x63, 0x74, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00 };
int WAVstereo[4] = { 2,2,2,1 }; 
char MP3VersionString[2][8] = { "mpeg1","mpeg2" }; //TODO : need different declare for exact same with original binary
char MP3StereoString[4][12] = {"stereo","j-stereo","dual","mono" };
int MP3BitArray[2][4] = { {22050, 24000, 16000, 99999}, {44100, 48000, 32000, 99999} };
int MP3FrequencyArray[2][16] = { {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 99999}, {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 99999} };
int Mp3toWavF(FILE *iFile, FILE *oFile) { //TODO : need test

	byte buf[1024];
	
	fseek(iFile, 0, 2);
	int iSize = ftell(iFile);
	bool odd = (iSize % 2 != 0);
	if (odd) iSize++;

	rewind(iFile);
	int read = fread(buf, 1, 0x400, iFile);
	if (read != 0x400) {
		ErrorLogAdd("入力ファイルが読めません。\n");
		return 0;
	}

	if( !(buf[0] == 0xff && (buf[1] & 0xf0) == 0xf0) ){ //file signature check
		ErrorLogAdd("入力ファイルはMP3データではないようです。\n");
		return 0;
	}

	byte MP3Version = (byte)buf[1] >> 3 & 1;
	int MP3Frequency = MP3FrequencyArray[MP3Version][(byte)buf[2] >> 4];
	int MP3Bitrate = MP3BitArray[MP3Version][(byte)buf[2] >> 2 & 3];
	byte MP3Stereo = (byte)buf[3] >> 6;
	ErrorLogFmtAdd("%s, %ldkbit/s, %ldHz, %s", MP3VersionString[MP3Version], MP3Frequency, MP3Bitrate, MP3StereoString[MP3Stereo]);

	int i;
	for (i = 2; i < 0x400; i++) {
		byte MP3VersionNew = (byte)buf[i + 1] >> 3 & 1;
		if ((buf[i] == (byte)0xff && (buf[i + 1] & 0xf0) == 0xf0) && MP3VersionNew == MP3Version && MP3FrequencyArray[MP3VersionNew][(byte)buf[i+2] >> 4] == MP3Frequency && MP3BitArray[MP3VersionNew][(byte)buf[i + 2] >> 2 & 3] == MP3Bitrate && (byte)buf[i+3] >> 6 == MP3Stereo) 
			break;
	}
	int AAUpos = i;
	ErrorLogFmtAdd("$%08lX\n", i & 0xffff);

	*(DWORD*)&WAVhead[4] = iSize + 0x3e; //62
	*(WORD*)&WAVhead[22] = WAVstereo[MP3Stereo];
	*(DWORD*)&WAVhead[24] = MP3Bitrate;
	*(DWORD*)&WAVhead[28] = MP3Frequency * 125;
	*(WORD*)&WAVhead[44] = AAUpos;
	*(DWORD*)&WAVhead[58] = (((MP3Version!=0) + 1) * 0x240) * (iSize / (ulonglong)(AAUpos & 0xffff));
	*(DWORD*)&WAVhead[66] = iSize - odd;

	if (fwrite(WAVhead,1,70,oFile) != 70) {
		ErrorLogAdd("ファイルに書けません。\n");
		return 0;
	}
	rewind(iFile);
	if (REC_COPYFILE(oFile, iFile, iSize - odd) == 0)
		return 0; 

	if (fwrite(WAVhead, 1, odd, oFile) != odd) {
		ErrorLogAdd("ファイルに書けません。\n");
		return 0;
	}
	
	if (fflush(oFile) != 0) {
		ErrorLogAdd("ファイルに書けません。\n");
		return 0; 
	}

	// FIXME: missing fclose(iFile); fclose(oFile);

	return 1;
}

bool Mp3toWavP(char *iPath, char *oPath) {
	FILE *iFile, *oFile;

	iFile = fopen(iPath, "rb");
	if (iFile == NULL) {
		ErrorLogFmtAdd("入力ファイルが開けません(%s)。\n", iPath);
		_exit(1);
	}

	oFile = fopen(iPath, "wb");
	if (oFile == NULL) {
		ErrorLogFmtAdd("出力ファイルが開けません(%s)。\n", oPath);
		_exit(1);
	}

	return Mp3toWavF(iFile, oFile) == 0;
}

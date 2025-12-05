#if _WIN64
#pragma comment(lib,"bin64/fmod_vc.lib")
#else
#if _WIN32
#pragma comment(lib,"bin86/fmod_vc.lib")
#endif
#endif
#include "En_audio.h"
#include "strclass.h"
#include "FMOD/fmod.h"
#include "DxLib/DxLib.h"
#include <algorithm>
#include <cmath>
#include <array>
#include <cstdint>
#include "filesystem.h"
using std::uintptr_t;

#ifdef _WIN32

#include <codecvt>

#else

#include <chrono>

#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l) ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))

enum { CP_ACP };

static DWORD timeGetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
		.count();
}

#endif // _WIN32

#include "Engine.h"

//we got the fmodex 4.13.04. so we have no version problem now.
//still there are writings for migration, for future

// 140611 1.04.00 FMOD_HARDWARE and FMOD_SOFTWARE flags have been removed. All voices are software mixed in FMOD Studio.
// 190314 2.00.00 The minimum supported version of Windows has been lifted from Windows XP to Windows 7. Coupled with this change we have removed the legacy DirectSound and WinMM output modes.
//https://www.fmod.com/docs/2.02/api/white-papers-transitioning-from-fmodex.html#system_adddsp-removed-from-the-system-api
//https://www.fmod.com/docs/2.02/api/welcome-revision-history.html
//http://upstream.rosalinux.ru/changelogs/fmod/44418/changelog.html

const char* GetFMODerror(int errCode){
	switch (errCode) {
	case 0:
		return "No errors.";
	case 1:
		return "Tried to call lock a second time before unlock was called. ";
	case 2:
		return "Tried to call a function on a data type that does not allow this type of functionality (ie call ing Sound::lock on a streaming sound). "			;
	case 3:
		return "Neither NTSCSI nor ASPI could be initialised. ";
	case 4:
		return "An error occurred while initialising the CDDA subsystem. ";
	case 5:
		return "Couldn\'t find the specified device. ";
	case 6:
		return "No audio tracks on the specified disc. ";
	case 7:
		return "No CD/DVD devices were found. ";
	case 8:
		return "No disc present in the specified drive. ";
	case 9:
		return "A CDDA read error occurred. ";
	case 10:
		return "Error trying to allocate a channel. ";
	case 0xb:
		return "The specified channel has been reused to play another sound. ";
	case 0xc:
		return "A Win32 COM related error occured. COM failed to initialize or a QueryInterface failed meaning  a Windows codec or driver was not installed properly. ";
	case 0xd:
		return "DMA Failure.  See debug output for more information. ";
	case 0xe:
		return "DSP connection error.  Connection possibly caused a cyclic dependancy.  Or tried to connect a t ree too many units deep (more than 128). ";
	case 0xf:
		return "DSP Format error.  A DSP unit may have attempted to connect to this network with the wrong form at. ";
	case 0x10:
		return "DSP connection error.  Couldn\'t find the DSP unit specified. ";
	case 0x11:
		return "DSP error.  Cannot perform this operation while the network is in the middle of running.  This  will most likely happen if a connection or disconnection is attempted in a DSP callback. ";
	case 0x12:
		return "DSP connection error.  The unit being connected to or disconnected should only have 1 input or  output. ";
	case 0x13:
		return "Error loading file. ";
	case 0x14:
		return "Couldn\'t perform seek operation.  This is a limitation of the medium (ie netstreams) or the fi le format. ";
	case 0x15:
		return "Media was ejected while reading. ";
	case 0x16:
		return "End of file unexpectedly reached while trying to read essential data (truncated data?). ";
	case 0x17:
		return "File not found. ";
	case 0x18:
		return "Unwanted file access occured. ";
	case 0x19:
		return "Unsupported file or audio format. ";
	case 0x1a:
		return "A HTTP error occurred. This is a catch-all for HTTP errors not listed elsewhere. ";
	case 0x1b:
		return "The specified resource requires authentication or is forbidden. ";
	case 0x1c:
		return "Proxy authentication is required to access the specified resource. ";
	case 0x1d:
		return "A HTTP server error occurred. ";
	case 0x1e:
		return "The HTTP request timed out. ";
	case 0x1f:
		return "FMOD was not initialized correctly to support this function. ";
	case 0x20:
		return "Cannot call this command after System::init. ";
	case 0x21:
		return "An error occured that wasn\'t supposed to.  Contact support. ";
	case 0x22:
		return "On Xbox 360, this memory address passed to FMOD must be physical, (ie allocated with XPhysicalA lloc.) ";
	case 0x23:
		return "Value passed in was a NaN, Inf or denormalized float. ";
	case 0x24:
		return "An invalid object handle was used. ";
	case 0x25:
		return "An invalid parameter was passed to this function. ";
	case 0x26:
		return "An invalid speaker was passed to this function based on the current speaker mode. ";
	case 0x27:
		return "The syncpoint did not come from this sound handle. ";
	case 0x28:
		return "The vectors passed in are not unit length, or perpendicular. ";
	case 0x29:
		return "PS2 only.  fmodex.irx failed to initialize.  This is most likely because you forgot to load it.  ";
	case 0x2a:
		return "Reached maximum audible playback count for this sound\'s soundgroup. ";
	case 0x2b:
		return "Not enough memory or resources. ";
	case 0x2c:
		return "Can\'t use FMOD_OPENMEMORY_POINT on non PCM source data, or non mp3/xma/adpcm data if FMOD_CREA TECOMPRESSEDSAMPLE was used. ";
	case 0x2d:
		return "PS2 only.  Not enough memory or resources on PlayStation 2 IOP ram. ";
	case 0x2e:
		return "Not enough memory or resources on console sound ram. ";
	case 0x2f:
		return "Tried to call a command on a 3d sound when the command was meant for 2d sound. ";
	case 0x30:
		return "Tried to call a command on a 2d sound when the command was meant for 3d sound. ";
	case 0x31:
		return "Tried to use a feature that requires hardware support.  (ie trying to play a VAG compressed sou nd in software on PS2). ";
	case 0x32:
		return "Tried to use a feature that requires the software engine.  Software engine has either been turn ed off, or command was executed on a hardware channel which does not support this feature. ";
	case 0x33:
		return "Couldn\'t connect to the specified host. ";
	case 0x34:
		return "A socket error occurred.  This is a catch-all for socket-related errors not listed elsewhere. ";
	case 0x35:
		return "The specified URL couldn\'t be resolved. ";
	case 0x36:
		return "Operation on a non-blocking socket could not complete immediately. ";
	case 0x37:
		return "Operation could not be performed because specified sound is not ready. ";
	case 0x38:
		return "Error initializing output device, but more specifically, the output device is already in use an d cannot be reused. ";
	case 0x39:
		return "Error creating hardware sound buffer. ";
	case 0x3a:
		return "A call to a standard soundcard driver failed, which could possibly mean a bug in the driver or  resources were missing or exhausted. ";
	case 0x3b:
		return "Error enumerating the available driver list. List may be inconsistent due to a recent device ad dition or removal. ";
	case 0x3c:
		return "Soundcard does not support the minimum features needed for this soundsystem (16bit stereo outpu t). ";
	case 0x3d:
		return "Error initializing output device. ";
	case 0x3e:
		return "FMOD_HARDWARE was specified but the sound card does not have the resources necessary to play it . ";
	case 0x3f:
		return "Attempted to create a software sound but no software channels were specified in System::init. ";
	case 0x40:
		return "Panning only works with mono or stereo sound sources. ";
	case 0x41:
		return "An unspecified error has been returned from a 3rd party plugin. ";
	case 0x42:
		return "The number of allowed instances of a plugin has been exceeded. ";
	case 0x43:
		return "A requested output, dsp unit type or codec was not available. ";
	case 0x44:
		return "A resource that the plugin requires cannot be found. (ie the DLS file for MIDI playback) ";
	case 0x45:
		return "An error occured trying to initialize the recording device. ";
	case 0x46:
		return "Specified Instance in FMOD_REVERB_PROPERTIES couldn\'t be set. Most likely because it is an inv alid instance number, or another application has locked the EAX4 FX slot. ";
	case 0x47:
		return "This subsound is already being used by another sound, you cannot have more than one parent to a  sound.  Null out the other parent\'s entry first. ";
	case 0x48:
		return "Shared subsounds cannot be replaced or moved from their parent stream, such as when the parent  stream is an FSB file. ";
	case 0x49:
		return "The subsound\'s mode bits do not match with the parent sound\'s mode bits.  See documentation f or function that it was called with. ";
	case 0x4a:
		return "The error occured because the sound referenced contains subsounds.  The operation cannot be per formed on a parent sound, or a parent sound was played without setting up a sentence first. ";
	case 0x4b:
		return "The specified tag could not be found or there are no tags. ";
	case 0x4c:
		return "The sound created exceeds the allowable input channel count.  This can be increased using the m axinputchannels parameter in System::setSoftwareFormat. ";
	case 0x4d:
		return "Something in FMOD hasn\'t been implemented when it should be! contact support! ";
	case 0x4e:
		return "This command failed because System::init or System::setDriver was not called. ";
	case 0x4f:
		return "A command issued was not supported by this object.  Possibly a plugin without certain callbacks  specified. ";
	case 0x50:
		return "An error caused by System::update occured. ";
	case 0x51:
		return "The version number of this file format is not supported. ";
	case 0x52:
		return "An Event failed to be retrieved, most likely due to \'just fail\' being specified as the max pl aybacks behavior. ";
	case 0x53:
		return "Can\'t execute this command on an EVENT_INFOONLY event. ";
	case 0x54:
		return "An error occured that wasn\'t supposed to.  See debug log for reason. ";
	case 0x55:
		return "Event failed because \'Max streams\' was hit when FMOD_INIT_FAIL_ON_MAXSTREAMS was specified. ";
	case 0x56:
		return "FSB mismatches the FEV it was compiled with, the stream/sample mode it was meant to be created  with was different, or the FEV was built for a different platform. ";
	case 0x57:
		return "A category with the same name already exists. ";
	case 0x58:
		return "The requested event, event group, event category or event property could not be found. ";
	case 0x59:
		return "Tried to call a function on a complex event that\'s only supported by simple events. ";
	case 0x5a:
		return "An event with the same GUID already exists. ";
	case 0x5b:
		return "Music system is not initialized probably because no music data is loaded. ";
	default:
		return "Unknown error.";
	}
}

int IsAltSoundExist(CSTR *filepath){
	// TODO: .flac, like in FindAltSound
	const char *str;
	CSTR tStr(*filepath); //somewhat starnge

	filepath->lower();
	if (tStr.findStrPos("wav")) {
		filepath->nullAtPos(filepath->length() - 3);
		str = "ogg";
	}
	else if (tStr.findStrPos("ogg")) {
		filepath->nullAtPos(filepath->length() - 3);
		str = "wav";
	}
	else return -1;
	
	filepath->add(str);

	return IsFileExist(*filepath) ? 1 : -1;
}


int ReleaseSound(AUDIO *aud, SOUNDDATA *sound){
	CSTR &filepath = sound->filename;

	if (sound->load == '\0') {
		return -1;
	}
	if (filepath.length() <= 1) {
		filepath.fillzero();
		sound->length = 0;
		sound->load = '\0';
		return -1;
	}
	if (aud->is_fmod_disabled == 1) {
		StopSoundMem(sound->soundHandle);
		DeleteSoundMem(sound->soundHandle);//DeleteSoundMem(sound->soundHandle, 0);
	}
	else {
		FMOD_Sound_Release(sound->fmod_sound);
	}
	filepath.fillzero();
	sound->length = 0;
	sound->load = '\0';
	return 0;
}

// COPY

int StopSound(AUDIO *aud, SOUNDDATA *sound){
	FMOD_SOUND *tmpSnd;

	if (sound->load == '\0') {
		return -1;
	}
	if (aud->is_fmod_disabled == 1) {
		StopSoundMem(sound->soundHandle);
	}
	FMOD_Channel_GetCurrentSound(sound->fmod_channel, &tmpSnd);
	if (tmpSnd == sound->fmod_sound) {
		FMOD_Channel_Stop(sound->fmod_channel);
	}
	return 0;
}

int SetSoundVolume(AUDIO *aud, SOUNDDATA *sound, float volume){
	FMOD_SOUND *tmpSnd;

	if (sound->load == '\0') {
		return -1;
	}
	if (aud->is_fmod_disabled == 1) {
		ChangeVolumeSoundMem(volume * 255.0, sound->soundHandle);
	}
	FMOD_Channel_GetCurrentSound(sound->fmod_channel, &tmpSnd);
	if (tmpSnd == sound->fmod_sound) {
		FMOD_Channel_SetVolume(sound->fmod_channel, volume);
	}
	return 0;
}

int SoundGetCurrentTime(AUDIO *aud, SOUNDDATA *sound){
	FMOD_SOUND *tmpSnd;
	uint pos;

	if (sound->load == '\0') {
		return -1;
	}
	if (aud->is_fmod_disabled == 1) {
		return GetSoundCurrentTime(sound->soundHandle);
	}
	FMOD_Channel_GetCurrentSound(sound->fmod_channel, &tmpSnd);
	if (tmpSnd == sound->fmod_sound) {
		FMOD_Channel_GetPosition(sound->fmod_channel, &pos, 1);
		return pos;
	}
	return -1;
}

// endsound
int EndSound(AUDIO *aud){

	if (aud->is_fmod_disabled != 1) {
		for (int i = 0; i < 7; i++) {
			FMOD_DSP_Release(aud->param.DSP_eq[i]);
		}
		for (int i = 0; i < 3; i++) {
			FMOD_DSP_Release(aud->param.DSP_fx[i]);
		}
		FMOD_DSP_Release(aud->param.DSP_pitch);
		FMOD_ChannelGroup_Release(aud->chnBgm);
		FMOD_ChannelGroup_Release(aud->chnKey);
		FMOD_ChannelGroup_Release(aud->chnMaster);
		FMOD_System_Close(aud->fmodSys);
		FMOD_System_Release(aud->fmodSys);
	}
	return 0;
}

int SOUND_dxlibFx(SOUNDDATA sound, int v_master, int /*v_chn*/, int pitch, double freq) {

	double dMul;
	
	if (sound.load) {
		ChangeVolumeSoundMem(v_master, sound.soundHandle);
		dMul = 1.0;
		if (pitch > 0) {
			for (; pitch > 0; pitch--) {
				dMul *= 1.0594631433486938;
			}
		}
		else if (pitch < 0) {
			for (pitch = -pitch; pitch > 0; pitch--) {
				dMul /= 1.0594631433486938;
			}
		}
		SetFrequencySoundMem((double)GetFrequencySoundMem(sound.soundHandle) * dMul * freq, sound.soundHandle);
	}
	return 0;
}

int SetFadeOut(AUDIO *aud, int fadetime){

	if (fadetime <= 0) {
		return -1;
	}
	aud->param.fadeout_volume = 1.0;
	aud->param.time_fadeout_start = timeGetTime();
	aud->param.time_fadeout_end = timeGetTime() + fadetime;
	return 1;
}

int SetFadePreview(AUDIO *aud, int fadetime, char quiet){

	if (fadetime <= 0) {
		return -1;
	}
	aud->param.time_fadePreview_start = timeGetTime();
	aud->param.fadePreviewStartVolume = aud->param.fadePreviewCurrentVolume;
	aud->param.time_fadePreview_end = timeGetTime() + fadetime;
	aud->param.fadePreviewIsQuiet = quiet;
	aud->param.fadePreviewTargetVolume = quiet? 0.025 : 1.0;
	return 1;
}

char sndBufUnk;
int GetSoundBuffer(AUDIO *aud, double runtime, int volume) {

	int len;
	aud->bitRate = 0;
	len = (runtime / 1000.0 + 1.0) * 44100.0 * 2;
	aud->size = ((len / 4096) + 1) * 4096; //to avoid size 0?
	aud->volume = volume / 100.0;
	if (aud->volume >= 1.0) {
		aud->volume = 1.0;
	}
	if (aud->volume < 0.0) {
		aud->volume = 0.0;
	}
	ErrorLogFmtAdd("音量設定 %d", aud->volume * 100.0);
	ErrorLogFmtAdd("バッファを取得します。サイズ%d\n", aud->size);

	aud->buffer = (short*)malloc(aud->size*2); // min(aud->size*2,0xFFFFFFFF) is compiler code?
	if(aud->buffer == NULL) {
		ErrorLogFmtAdd("バッファ取得に失敗。サイズ%d\n", aud->size);
		sndBufUnk = 0;
		return 1;
	}

	ErrorLogFmtAdd("バッファ取得が完了しました。サイズ%d\n", aud->size);
	memset(aud->buffer, '\0', aud->size * 2);
	ErrorLogFmtAdd("0で埋めました\n");
	sndBufUnk = 0;
	return 1;
}

// debug test needed
void WriteSoundFile(AUDIO *aud, CSTR filename, uint size) {
	
	FILE* _File;
	char header1[12], header2[24], header3[8];

	_File = fopen(filename.body, "wb");
	if ((size != 0) && (size <= aud->size)) {
		aud->size = size;
	}
	if (_File) {
		fseek(_File, 0, 0);
		header1[0] = 'R';
		header1[1] = 'I';
		header1[2] = 'F';
		header1[3] = 'F';
		*(unsigned int*)(&header1[4]) = aud->size * 2 + 0x20;
		header1[8] = 'W';
		header1[9] = 'A';
		header1[10] = 'V';
		header1[11] = 'E';

		header2[0] = 'f';
		header2[1] = 'm';
		header2[2] = 't';
		header2[3] = ' ';
		*(unsigned int*)(&header2[4]) = 16;
		*(unsigned short*)(&header2[8]) = 1;
		*(unsigned short*)(&header2[10]) = 2;
		*(unsigned int*)(&header2[12]) = 44100;
		*(unsigned int*)(&header2[16]) = 176400;
		*(unsigned short*)(&header2[20]) = 4;
		*(unsigned short*)(&header2[22]) = 16;

		header3[0] = 'd';
		header3[1] = 'a';
		header3[2] = 't';
		header3[3] = 'a';
		*(unsigned int*)(&header3[4]) = aud->size * 2;

		fwrite(header1, 0xc, 1, _File);
		fwrite(header2, 0x18, 1, _File);
		fwrite(header3, 8, 1, _File);
		fwrite(aud->buffer, aud->size * 2, 1, _File);
		fclose(_File);
	}
	if (aud->buffer) {
		free(aud->buffer);
	}
	ErrorLogFmtAdd("データを保存しました。\n");
}

int SOUND_normalize(AUDIO */*aud*/, SOUNDDATA *sound){
	uint len;
	int bitdepth;
	int channels;
	float samples;

	len = 0;
	FMOD_Sound_GetLength(sound->fmod_sound, &len, 4);
	samples = 44100.0;
	channels = 2;
	bitdepth = 16;
	FMOD_Sound_GetFormat(sound->fmod_sound, NULL, NULL, &channels, &bitdepth);
	FMOD_Sound_GetDefaults(sound->fmod_sound, &samples, NULL);

	void* ptr1 = 0;
	void* ptr2 = 0;
	uint len1 = 0;
	uint len2 = 0;

	FMOD_Sound_Lock(sound->fmod_sound, 0, len, &ptr1, &ptr2, &len1, &len2);

	sound->raw.bits = bitdepth;
	sound->raw.channels = channels;
	sound->raw.length = len;
	sound->raw.dataSize = len;
	sound->raw.samples = samples;
	sound->raw.data = (byte *)malloc(len);
	assert(sound->raw.data != nullptr);
	
	memcpy(sound->raw.data, sound, len);

	if (sound->raw.bits == 8 && sound->flag2c) { //TODO : flag2c rename
		for (uint i = 0; i < len; i++) {
			sound->raw.data[i] += 0x80;
		}
	}

	FMOD_Sound_Unlock(sound->fmod_sound, ptr1, ptr2, len1, len2);
	
	sound->raw.MakeBitDepth16();
	sound->raw.MakeStereo();
	sound->raw.MakeSampleRate44100();

	return 1;
}

// TODO: need test
int RecordSound(AUDIO *aud, SOUNDDATA *sound, double time, double len) {

	if (sound->load && len >= 0.0) {

		int paramlen = time * 44100.0 / 1000.0 * 2.0;
		int soundlen = sound->raw.length / 2;
		if (len > 0.0) {
			soundlen = len * 44100.0 / 1000.0 * 2.0;
			if (soundlen > (sound->raw.length / 2)) soundlen = (sound->raw.length / 2);
		}

		for (uint i = 0; i < soundlen; i++) {
			double fade = 1.0;
			if (i < 300) {
				fade = (i + 1) / 300.0;
			}
			if (sound->raw.length - i < 300) {
				fade = (sound->raw.length - i) / 300.0;
			}
			
			if (aud->size <= paramlen + i) {
				//exapnd aud buffer size (*2)
				short* newbuffer = (short*)malloc(aud->size * 2 * 2);
				assert(newbuffer != nullptr);
				memset(newbuffer, 0, aud->size * 4);
				memcpy(newbuffer, aud->buffer, aud->size * 2);
				if (aud->buffer != NULL) {
					free(aud->buffer);
					aud->buffer = NULL;
				}
				aud->buffer = newbuffer;
				aud->size = aud->size * 2;
			}

			int newval = *(short*)((uintptr_t)aud->buffer + (paramlen + i) * 2) + (float)(*(short*)(sound->raw.data + i * 2)) * aud->volume * fade;

			if (newval >= 0x7fff) *(short*)((uintptr_t)aud->buffer + (paramlen + i) * 2) = 0x7fff;
			else if (newval < -0x7fff) *(short*)((uintptr_t)aud->buffer + (paramlen + i) * 2) = -0x8000;
			else *(short*)((uintptr_t)aud->buffer + (paramlen + i) * 2) = (short)newval;
		}
		return 1;
	}
	return 0;
}

int RecordFadeout(AUDIO *aud, double from, double length) {

	if (length <= 0.0) return 0;

	int start = from * 44100.0 / 1000.0 * 2.0;
	int len = length * 44100.0 / 1000.0 * 2.0;
	for (uint i = start, c = 0; i < aud->size; i++, c++) {

		if (i < start + len) {
			aud->buffer[i] = aud->buffer[i] * ((double)1 - c / (double)len);
		}
		else {
			aud->buffer[i] = 0;
		}
	}
	return 1;
}

int LoadSound(AUDIO *aud, SOUNDDATA *sound, CSTR filepath, int loop, int /*disableDSP*/, int previewFlag) {

	CSTR path;
	path.assign(&filepath);

	if (IsFileExist(filepath) == false && IsAltSoundExist(&filepath) == -1) {
		ErrorLogFmtAdd("音声ファイルが見つかりません。%s...\n",path.body);
		if (sound->load == '\0') {
			sound->filename.fillzero();
			sound->load = '\0';
			return -2;
		}
		return 0;
	}

	if (sound->load == 1) {
		if (filepath.isSame(&sound->filename) && previewFlag == sound->streaming) return 1;
		ReleaseSound(aud, sound);
	}

	sound->streaming = previewFlag;

	if (aud->is_fmod_disabled != 1) {
		FMOD_RESULT result;
		if (aud->cmd_mediaOut) {
			FMOD_MODE mode = FMOD_ACCURATETIME | FMOD_LOOP_OFF;
			result = FMOD_System_CreateSound(aud->fmodSys, filepath.body, mode, nullptr, &sound->fmod_sound); 
			SOUND_normalize(aud, sound);
		}
		else {
			FMOD_MODE mode = loop == 0 ? FMOD_LOOP_OFF : FMOD_LOOP_NORMAL;
			if (previewFlag != 0) {
				mode |= FMOD_CREATESTREAM;
			}
			result = FMOD_System_CreateSound(aud->fmodSys, filepath.body, mode, nullptr, &sound->fmod_sound);
		}

		sound->flag2c = 0;
		sound->loop = (loop != 0);
		if (result == FMOD_OK) {
			sound->filename.assign(&filepath);
			sound->load = 1;
			FMOD_Sound_GetLength(sound->fmod_sound, &sound->length, 1);
			if (sound->length >= 240000) {
				sound->length = 240000;
			}
		}
		else {
			sound->filename.fillzero();
			sound->load = 0;
			sound->length = -1;
		}
		return 1;
	}

	if (previewFlag) SetCreateSoundDataType(3);
	sound->soundHandle = LoadSoundMem(filepath.body, 3, -1);
	if (previewFlag) SetCreateSoundDataType(0);
	if (sound->soundHandle == -1) {
		sound->filename.fillzero();
		sound->load = 0;
		return -1;
	}
	sound->filename.assign(&filepath);
	sound->length = GetSoundTotalTime(sound->soundHandle);
	sound->load = 1;
	sound->loop = (loop != 0);
	sound->unused0C = 0;
	return 1;
}

int PlaySound(AUDIO *aud, SOUNDDATA *sound, FMOD_CHANNELGROUP *channelgroup, int stage) {

	if (aud->cmd_mediaOut == false) {
		if (sound->load == 0) return -1;
		if (aud->is_fmod_disabled == 1) {
			int v_master;
			int v_chn;
			int pitch;
			double freq = 1;

			StopSoundMem(sound->soundHandle);
			PlaySoundMem(sound->soundHandle, (sound->loop == 0)? 1 : 3, 1);

			pitch = (aud->param.pitch_on == 0) ? 0 : aud->param.pitch_amount;
			if (stage < 5) freq = aud->param.stagePitch[stage];

			if (aud->param.fx_volume_on == 0) {
				v_master = aud->param.fadeout_volume * 100.0; // fadeout_volume
				v_chn = 100;
			}
			else {
				v_master = aud->param.volume_master;
				v_chn = aud->param.volume_key;
			}

			SOUND_dxlibFx(*sound, v_master, v_chn, pitch, freq);
			return 0;
		}

		FMOD_SOUND* tempSound;
		
		FMOD_Channel_GetCurrentSound(sound->fmod_channel, &tempSound);
		if (tempSound == sound->fmod_sound) FMOD_Channel_Stop(sound->fmod_channel);
		FMOD_System_PlaySound(aud->fmodSys, sound->fmod_sound, channelgroup, false, &sound->fmod_channel);
		FMOD_Channel_SetChannelGroup(sound->fmod_channel, channelgroup);
	}
	else if(aud->replay2avi){
		RecordSound(aud, sound, aud->aviTimer, 0);
		return 0;
	}
	return 0;
}


int SOUND_FmodToDxlib(AUDIO *aud) {
	
	int pitch, v_master, _volume_BGM, _volume_key;

	if (aud->param.pitch_on == 0) {
		pitch = 0;
	}
	else {
		pitch = aud->param.pitch_amount;
	}

	if (aud->param.fx_volume_on == 0) {
		v_master = 100;
		_volume_BGM = 100;
		_volume_key = 100;
	}
	else {
		_volume_key = aud->param.volume_key;
		_volume_BGM = aud->param.volume_BGM;
		v_master = aud->param.volume_master;
	}

	SOUND_dxlibFx(aud->sysSound.select, v_master, _volume_BGM, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.folder_open, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.folder_close, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.panel_open, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.option_change, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.decide, v_master, _volume_BGM, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.clear, v_master, _volume_BGM, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.fail, v_master, _volume_BGM, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.stop, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.mine, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.scratch, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.courseclear, v_master, _volume_key, pitch, 1.0);
	SOUND_dxlibFx(aud->sysSound.coursefail, v_master, _volume_key, pitch, 1.0);
	return 0;
}

int ApplySoundFX(AUDIO *aud, int /*flag*/, char /*disable*/) {

	if(aud->cmd_mediaOut) return 0;
	if (aud->is_fmod_disabled == 1) {
		aud->param.eq_on = 0;
		aud->param.fx_on[0] = 0;
		aud->param.fx_on[1] = 0;
		aud->param.fx_on[2] = 0;
		aud->param.pitch_type = 0;
		SOUND_FmodToDxlib(aud);
		return 0;
	}

	// volume
	if (aud->param.fx_volume_on) {
		FMOD_ChannelGroup_SetVolume(aud->chnBgm, (aud->param.volume_BGM / 100.0) * (aud->param.volume_master / 100.0) * aud->param.fadeout_volume);
		FMOD_ChannelGroup_SetVolume(aud->chnKey, (aud->param.volume_master / 100.0) * (aud->param.volume_key / 100.0) * aud->param.fadeout_volume);
	}
	else {
		FMOD_ChannelGroup_SetVolume(aud->chnBgm, aud->param.fadeout_volume);
		FMOD_ChannelGroup_SetVolume(aud->chnKey, aud->param.fadeout_volume);
	}

	for (int i = 0; i < 5; i++) {
		FMOD_ChannelGroup_SetPitch(aud->chnStageBgm[i], aud->param.stagePitch[i]);
		FMOD_ChannelGroup_SetPitch(aud->chnStageKey[i], aud->param.stagePitch[i]);
		FMOD_ChannelGroup_SetVolume(aud->chnStageKey[i], aud->param.stageKeyVolume[i]* aud->param.stageKeyVolume[i]);
		FMOD_ChannelGroup_SetVolume(aud->chnStageBgm[i], aud->param.stageBgmVolume[i]* aud->param.stageBgmVolume[i]);
	}
	
	// pitch/freq
	if (aud->param.DSP_pitch == NULL) {
		if ((aud->param.pitch_type == 1 || aud->param.pitch_type == 2) && (aud->param.pitch_amount != 0) && (aud->param.pitch_on == 1)) {
			FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_PITCHSHIFT, &aud->param.DSP_pitch);
			FMOD_DSP_SetParameterFloat(aud->param.DSP_pitch, FMOD_DSP_PITCHSHIFT_FFTSIZE, 512.f);
			FMOD_ChannelGroup_AddDSP(aud->chnMaster, 0, aud->param.DSP_pitch);
			FMOD_DSP_SetActive(aud->param.DSP_pitch, FALSE);
		}
	}
	else if (aud->param.pitch_type == 0 || aud->param.pitch_amount == 0 || aud->param.pitch_on == 0) {
		FMOD_ChannelGroup_RemoveDSP(aud->chnMaster, aud->param.DSP_pitch);
		FMOD_DSP_Release(aud->param.DSP_pitch);
		aud->param.DSP_pitch = NULL;
	}

	if (aud->param.pitch_on == 0 || (aud->param.pitch_on == 1 && aud->param.pitch_type == 1)) {
		FMOD_ChannelGroup_SetPitch(aud->chnBgm, 1.0);
		FMOD_ChannelGroup_SetPitch(aud->chnKey, 1.0);
	}
	else if ((aud->param.pitch_type == 0) || (aud->param.pitch_type == 2)) {
		int pitch = aud->param.pitch_amount;
		double dMul;
		dMul = 1.0;
		if (pitch > 0) {
			for (; pitch > 0; pitch--) {
				dMul *= 1.0594631433486938; //check is done with VS08. it's 0x3ff0f38fa0000000 same with original
			} //TOFIX? : sound freq/speed doesn't match game note speed (on +-12, 2.00000111 and 2.0 or 0.4999997226 and 0.5)
		}
		else if (pitch < 0) {
			for (pitch = -pitch; pitch > 0; pitch--) {
				dMul /= 1.0594631433486938;
			}
		}
		FMOD_ChannelGroup_SetPitch(aud->chnBgm, dMul);
		FMOD_ChannelGroup_SetPitch(aud->chnKey, dMul);
	}

	if (aud->param.pitch_on) {
		if (aud->param.pitch_type == 1 && aud->param.DSP_pitch) {
			int pitch = aud->param.pitch_amount;
			double dMul;
			dMul = 1.0;
			if (pitch > 0) {
				for (; pitch > 0; pitch--) {
					dMul *= 1.059463094359; //same with ParseBmsFile()
				}
			}
			else if (pitch < 0) {
				for (pitch = -pitch; pitch > 0; pitch--) {
					dMul /= 1.059463094359;
				}
			}
			FMOD_DSP_SetParameterFloat(aud->param.DSP_pitch, FMOD_DSP_PITCHSHIFT_PITCH, dMul);
			FMOD_DSP_SetActive(aud->param.DSP_pitch, 1);
		}
		else if (aud->param.pitch_type == 2 && aud->param.DSP_pitch) {
			int pitch = aud->param.pitch_amount;
			double dMul;
			dMul = 1.0;
			if (pitch > 0) {
				for (; pitch > 0; pitch--) {
					dMul /= 1.059463094359;
				}
			}
			else if (pitch < 0) {
				for (pitch = -pitch; pitch > 0; pitch--) {
					dMul *= 1.059463094359;
				}
			}
			FMOD_DSP_SetParameterFloat(aud->param.DSP_pitch, FMOD_DSP_PITCHSHIFT_PITCH, dMul);
			FMOD_DSP_SetActive(aud->param.DSP_pitch, 1);
		}
	}

	// EQ
	if (aud->param.DSP_eq[0] == NULL) {
		FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_MULTIBAND_EQ, &aud->param.DSP_eq[0]);
		FMOD_ChannelGroup_AddDSP(aud->chnMaster, 1, aud->param.DSP_eq[0]);

		FMOD_DSP_SetParameterInt(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_A_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_PEAKING);
		FMOD_DSP_SetParameterInt(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_B_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_PEAKING);
		FMOD_DSP_SetParameterInt(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_C_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_PEAKING);
		FMOD_DSP_SetParameterInt(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_D_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_PEAKING);
		FMOD_DSP_SetParameterInt(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_E_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_PEAKING);

		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_A_FREQUENCY, 63.f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_B_FREQUENCY, 160.f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_C_FREQUENCY, 400.f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_D_FREQUENCY, 1000.f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_E_FREQUENCY, 2500.f);

		// bandwidth
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_A_Q, 1.5f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_B_Q, 1.5f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_C_Q, 1.5f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_D_Q, 1.5f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_E_Q, 1.5f);

		FMOD_DSP_SetActive(aud->param.DSP_eq[0], TRUE);
	}
	if (aud->param.DSP_eq[1] == NULL) {
		FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_MULTIBAND_EQ, &aud->param.DSP_eq[1]);
		FMOD_ChannelGroup_AddDSP(aud->chnMaster, 2, aud->param.DSP_eq[1]);

		FMOD_DSP_SetParameterInt(aud->param.DSP_eq[1], FMOD_DSP_MULTIBAND_EQ_A_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_PEAKING);
		FMOD_DSP_SetParameterInt(aud->param.DSP_eq[1], FMOD_DSP_MULTIBAND_EQ_B_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_PEAKING);

		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[1], FMOD_DSP_MULTIBAND_EQ_A_FREQUENCY, 6300.f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[1], FMOD_DSP_MULTIBAND_EQ_B_FREQUENCY, 16000.f);

		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[1], FMOD_DSP_MULTIBAND_EQ_A_Q, 1.5f);
		FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[1], FMOD_DSP_MULTIBAND_EQ_B_Q, 1.5f);

		FMOD_DSP_SetActive(aud->param.DSP_eq[1], TRUE);
	}

	for (int i = 0; i < 7; i++) {
		float dMul = static_cast<float>(aud->param.eq_gain[i]);
		switch (i) {
		case 0: FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_A_GAIN, dMul); break;
		case 1: FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_B_GAIN, dMul); break;
		case 2: FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_C_GAIN, dMul); break;
		case 3: FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_D_GAIN, dMul); break;
		case 4: FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[0], FMOD_DSP_MULTIBAND_EQ_E_GAIN, dMul); break;
		case 5: FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[1], FMOD_DSP_MULTIBAND_EQ_A_GAIN, dMul); break;
		case 6: FMOD_DSP_SetParameterFloat(aud->param.DSP_eq[1], FMOD_DSP_MULTIBAND_EQ_B_GAIN, dMul); break;
		}
	}

	FMOD_DSP_SetBypass(aud->param.DSP_eq[0], aud->param.eq_on == 1 ? FALSE : TRUE);
	FMOD_DSP_SetBypass(aud->param.DSP_eq[1], aud->param.eq_on == 1 ? FALSE : TRUE);

	// FX
	static std::array<int, 3> fxLastChannel{ -1, -1, -1 };
	for (int i = 0; i < 3; i++) {
		FMOD_DSP_TYPE typeNow;
		FMOD_DSP_TYPE typeLast;
		if (aud->param.DSP_fx[i] == 0)
			typeNow = FMOD_DSP_TYPE_UNKNOWN;
		else
			FMOD_DSP_GetType(aud->param.DSP_fx[i], &typeNow);

		switch (aud->param.fxType[i]) {
		case 1: typeLast = FMOD_DSP_TYPE_SFXREVERB; break;
		case 2: typeLast = FMOD_DSP_TYPE_ECHO; break;
		case 3: typeLast = FMOD_DSP_TYPE_LOWPASS; break;
		case 4: typeLast = FMOD_DSP_TYPE_HIGHPASS; break;
		case 5: typeLast = FMOD_DSP_TYPE_FLANGE; break;
		case 6: typeLast = FMOD_DSP_TYPE_CHORUS; break;
		case 7: typeLast = FMOD_DSP_TYPE_DISTORTION; break;
		default: typeLast = FMOD_DSP_TYPE_UNKNOWN;
		}
		if (typeNow != typeLast) {
			if (aud->param.DSP_fx[i]) {
				switch (aud->param.fxChannel[i]) {
				case 0: FMOD_ChannelGroup_RemoveDSP(aud->chnMaster, aud->param.DSP_fx[i]); break;
				case 1: FMOD_ChannelGroup_RemoveDSP(aud->chnKey, aud->param.DSP_fx[i]); break;
				case 2: FMOD_ChannelGroup_RemoveDSP(aud->chnBgm, aud->param.DSP_fx[i]); break;
				}
				FMOD_DSP_Release(aud->param.DSP_fx[i]);
				aud->param.DSP_fx[i] = NULL;
			}

			switch (aud->param.fxType[i]) {
			case 1: FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_SFXREVERB, &aud->param.DSP_fx[i]); break;
			case 2: FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_ECHO, &aud->param.DSP_fx[i]); break;
			case 3: FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_LOWPASS, &aud->param.DSP_fx[i]); break;
			case 4: FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_HIGHPASS, &aud->param.DSP_fx[i]); break;
			case 5: FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_FLANGE, &aud->param.DSP_fx[i]); break;
			case 6: FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_CHORUS, &aud->param.DSP_fx[i]); break;
			case 7: FMOD_System_CreateDSPByType(aud->fmodSys, FMOD_DSP_TYPE_DISTORTION, &aud->param.DSP_fx[i]); break;
			default: aud->param.DSP_fx[i] = NULL; continue;
			}

			switch (aud->param.fxChannel[i]) {
			case 0: FMOD_ChannelGroup_AddDSP(aud->chnMaster, 3, aud->param.DSP_fx[i]); break;
			case 1: FMOD_ChannelGroup_AddDSP(aud->chnKey, 0, aud->param.DSP_fx[i]); break;
			case 2: FMOD_ChannelGroup_AddDSP(aud->chnBgm, 0, aud->param.DSP_fx[i]); break;
			}
			fxLastChannel[i] = aud->param.fxChannel[i];

			FMOD_DSP_SetActive(aud->param.DSP_fx[i], TRUE);
		}
		
		if (aud->param.DSP_fx[i]) {
			FMOD_DSP_SetBypass(aud->param.DSP_fx[i], aud->param.fx_on[i] == 1 ? FALSE : TRUE);

			if (aud->param.fxChannel[i] != fxLastChannel[i]) {
				switch (fxLastChannel[i]) {
				case 0: FMOD_ChannelGroup_RemoveDSP(aud->chnMaster, aud->param.DSP_fx[i]); break;
				case 1: FMOD_ChannelGroup_RemoveDSP(aud->chnKey, aud->param.DSP_fx[i]); break;
				case 2: FMOD_ChannelGroup_RemoveDSP(aud->chnBgm, aud->param.DSP_fx[i]); break;
				}

				switch (aud->param.fxChannel[i]) {
				case 0: FMOD_ChannelGroup_AddDSP(aud->chnMaster, 3, aud->param.DSP_fx[i]); break;
				case 1: FMOD_ChannelGroup_AddDSP(aud->chnKey, 0, aud->param.DSP_fx[i]); break;
				case 2: FMOD_ChannelGroup_AddDSP(aud->chnBgm, 0, aud->param.DSP_fx[i]); break;
				}
				fxLastChannel[i] = aud->param.fxChannel[i];
			}

			float fTmp;
			auto percentageToDecibel = [](float p) { return 10.0 * std::log10(static_cast<float>(p) / 100.f); };
			int& p1 = aud->param.fxParam[i][0];
			int& p2 = aud->param.fxParam[i][1];
			switch (aud->param.fxType[i]) {
			case 1:
				// FMOD_DSP_REVERB_ROOMSIZE 0.0-1.0
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_SFXREVERB_DECAYTIME, std::clamp(static_cast<float>(p1 * 200.0), 100.f, 20000.f));
				// FMOD_DSP_REVERB_WETMIX 0.0-1.0
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_SFXREVERB_WETLEVEL, std::clamp(static_cast<float>(percentageToDecibel(p2)), -80.0f, 20.0f));
				break;
			case 2:
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_ECHO_DELAY, std::clamp(static_cast<float>(p1 * 10.0), 1.0f, 5000.0f));
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_ECHO_WETLEVEL, std::clamp(static_cast<float>(percentageToDecibel(p2)), -80.0f, 10.0f));
				break;
			case 3:
				fTmp = 22000.0 / std::pow(1.06, 100 - p1);
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_LOWPASS_CUTOFF, std::clamp(fTmp, 1.f, 22000.f));
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_LOWPASS_RESONANCE, std::clamp(static_cast<float>(p2 * 9.0 / 100.0 + 1.0), 0.f, 10.f));
				break;
			case 4:
				fTmp = 22.0 * std::pow(1.06, p1);
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_HIGHPASS_CUTOFF, std::clamp(fTmp, 1.f, 22000.f));
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_HIGHPASS_RESONANCE, std::clamp(static_cast<float>(p2 * 9.0 / 100.0 + 1.0), 0.f, 10.f));
				break;
			case 5:
				fTmp = 0.02 * std::pow(1.06, p2);
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_FLANGE_DEPTH, std::clamp(static_cast<float>(p1) / 100.f, 0.01f, 1.f));
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_FLANGE_RATE, std::clamp(fTmp, 0.f, 20.f));
				break;
			case 6:
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_CHORUS_RATE, std::clamp(static_cast<float>(p1) / 5.f, 0.f, 20.f));
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_CHORUS_DEPTH, std::clamp(static_cast<float>(p2), 0.f, 100.f));
				break;
			case 7:
				FMOD_DSP_SetParameterFloat(aud->param.DSP_fx[i], FMOD_DSP_DISTORTION_LEVEL, std::clamp(static_cast<float>(p1) / 100.f, 0.f, 1.f));
				break;
			}
		}
	}
	FMOD_System_Update(aud->fmodSys);
	return 1;
}

int InitFade(AUDIO *aud){
	aud->param.fadeout_volume = 1.0;
	aud->param.fadePreviewCurrentVolume = 1.0;
	aud->param.time_fadeout_start = -1;
	aud->param.time_fadeout_end = -1;
	aud->param.time_fadePreview_start = -1;
	aud->param.time_fadePreview_end = -1;
	ApplySoundFX(aud, 0, '\0');
	return 1;
}

int SetVolumeByFade(AUDIO *aud){
	float fVar1;
	DWORD timeNow;

	timeNow = timeGetTime();
	if (aud->param.time_fadeout_start != -1) {
		if (timeNow < aud->param.time_fadeout_end) {
			fVar1 = (float)(aud->param.time_fadeout_end - timeNow) / (float)(aud->param.time_fadeout_end - aud->param.time_fadeout_start);
		}
		else {
			fVar1 = 0.0;
		}
		aud->param.fadeout_volume = fVar1;
		ApplySoundFX(aud, 0, '\0');
	}
	if (aud->param.time_fadePreview_start != -1) {
		if (timeNow < aud->param.time_fadePreview_end) {
			fVar1 = (float)(timeNow - aud->param.time_fadePreview_start) / (float)(aud->param.time_fadePreview_end - aud->param.time_fadePreview_start);
			fVar1 = (1.0 - fVar1) * aud->param.fadePreviewStartVolume + aud->param.fadePreviewTargetVolume * fVar1;
		}
		else {
			fVar1 = aud->param.fadePreviewTargetVolume;
		}
		aud->param.fadePreviewCurrentVolume = fVar1;
		if (aud->param.fadePreviewCurrentVolume < 0.05) {
			aud->param.fadePreviewCurrentVolume = 0.025;
		}
		if (1.0 < aud->param.fadePreviewCurrentVolume) {
			aud->param.fadePreviewCurrentVolume = 1.0;
		}
		SetSoundVolume(aud, &aud->sysSound.select, aud->param.fadePreviewCurrentVolume);
		SetSoundVolume(aud, &aud->sysSound.exselect, aud->param.fadePreviewCurrentVolume);
	}
	return 1;
}

int InitSound(AUDIO *aud, uint bufferLength, int numBuffer, char fDisable, int outputType, int driver){

	int numDrivers;
	char driverName[256];

	if (aud->cmd_mediaOut) {
		FMOD_System_Create(&aud->fmodSys, FMOD_VERSION);
		FMOD_System_Init(aud->fmodSys, 1, 0, NULL);
		return 1;
	}
	if (!aud->is_fmod_disabled) {
		ErrorLogAdd("サウンドシステムの初期化を行います。\n");
		FMOD_System_Create(&aud->fmodSys, FMOD_VERSION);
		FMOD_System_SetDSPBufferSize(aud->fmodSys, bufferLength, numBuffer);
		ErrorLogAdd("\n");
		FMOD_System_SetSoftwareChannels(aud->fmodSys, 0x100);
		if (FMOD_System_Init(aud->fmodSys, 0x100, FMOD_INIT_NORMAL, NULL) != FMOD_OK) {
			ErrorLogAdd("FMOD_System_Init failed!\n");
			EndSound(aud);
			return 1;
		}

#ifdef _WIN32
		if ([&] {
			switch (outputType) {
			case 0:
			default:
				ErrorLogAdd("OUTPUT TYPE:WASAPI (LR2 DIRECTSOUND config ignored)\n");
				return FMOD_System_SetOutput(aud->fmodSys, FMOD_OUTPUTTYPE_WASAPI);
			case 1:
				ErrorLogAdd("OUTPUT TYPE:WASAPI\n");
				return FMOD_System_SetOutput(aud->fmodSys, FMOD_OUTPUTTYPE_WASAPI);
			case 2:
				ErrorLogAdd("OUTPUT TYPE:ASIO\n");
				return FMOD_System_SetOutput(aud->fmodSys, FMOD_OUTPUTTYPE_ASIO);
			}
			}() != FMOD_OK) {
			ErrorLogAdd("FMOD_System_SetOutput failed! Using autodetect\n");
			if (FMOD_System_SetOutput(aud->fmodSys, FMOD_OUTPUTTYPE_AUTODETECT) != FMOD_OK)
			{
				// FMOD_SYSTEM* is unusable after FMOD_System_SetOutput failed.
				ErrorLogAdd("FMOD_System_SetOutput failed hard!\n");
				EndSound(aud);
				return 0;
			}
		}
#endif // _WIN32

		FMOD_System_GetNumDrivers(aud->fmodSys, &numDrivers);
		if (driver + 1 > numDrivers) {
			driver = 0;
		}
		if (FMOD_System_GetDriverInfo(aud->fmodSys, driver, driverName, sizeof(driverName), nullptr, nullptr, nullptr, nullptr) == FMOD_OK) {
			ErrorLogFmtAdd("PLAYBACK DRIVER:%s\n", driverName);
		} else {
			ErrorLogFmtAdd("FMOD_System_GetDriverInfo failed\n");
		}
		FMOD_System_SetDriver(aud->fmodSys, driver);
		ErrorLogAdd("バッファサイズの設定を行います...\n");
		FMOD_System_CreateChannelGroup(aud->fmodSys, "bgm", &aud->chnBgm);
		FMOD_System_CreateChannelGroup(aud->fmodSys, "key", &aud->chnKey);
		FMOD_System_GetMasterChannelGroup(aud->fmodSys, &aud->chnMaster);
		
		CSTR buf;
		for (int i = 0; i < 5; i++) {
			cstrSprintf(&buf, "stage%d_bgm", i);
			FMOD_System_CreateChannelGroup(aud->fmodSys, buf, &aud->chnStageBgm[i]);
			FMOD_ChannelGroup_AddGroup(aud->chnBgm, aud->chnStageBgm[i], true, nullptr);
			cstrSprintf(&buf, "stage%d_key", i);
			FMOD_System_CreateChannelGroup(aud->fmodSys, buf, &aud->chnStageKey[i]);
			FMOD_ChannelGroup_AddGroup(aud->chnKey, aud->chnStageKey[i], true, nullptr);
		}
		aud->param.DSP_eq[0] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[1] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[2] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[3] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[4] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[5] = (FMOD_DSP *)0x0;
		aud->param.DSP_eq[6] = (FMOD_DSP *)0x0;
		aud->param.DSP_fx[0] = (FMOD_DSP *)0x0;
		aud->param.DSP_fx[1] = (FMOD_DSP *)0x0;
		aud->param.DSP_fx[2] = (FMOD_DSP *)0x0;
		aud->param.DSP_pitch = (FMOD_DSP *)0x0;
		aud->param.stagePitch[0] = 1.0;
		aud->param.stagePitch[1] = 1.0;
		aud->param.stagePitch[2] = 1.0;
		aud->param.stagePitch[3] = 1.0;
		aud->param.stagePitch[4] = 1.0;
		ApplySoundFX(aud, 1, fDisable);
		ErrorLogFmtAdd("サウンドの初期化が成功しました。\n");
		aud->param.fadeout_volume = 1.0;
		aud->param.fadePreviewCurrentVolume = 1.0;
		aud->param.time_fadeout_start = -1;
		aud->param.time_fadeout_end = -1;
		aud->param.time_fadePreview_start = -1;
		aud->param.time_fadePreview_end = -1;
		ApplySoundFX(aud, 0, '\0');
		return 0;
	}
	return 1;
}

RAWSOUND::RAWSOUND() {
	channels = 0;
	samples = 0;
	bits = 0;
	length = 0;
	dataSize = 0;
	data = NULL;
}

void RAWSOUND::ExpandBuffer(int minSize) { 

	size_t newSize = this->dataSize;
	byte* buf;
	if (newSize < minSize) {
		do {
			newSize = newSize * 2;
		} while (newSize < minSize);
		buf = (byte*)malloc(newSize);
		memcpy(buf, this->data, this->dataSize);
		memset(&buf[this->dataSize], 0, newSize - this->dataSize);
		free(this->data);
		this->data = buf;
		this->dataSize = newSize;
	}
	return;
}

void RAWSOUND::MakeBitDepth16(void){

	if (this->bits == 8) {
		ExpandBuffer(this->length * 2);
		for (int i = this->length - 1; i >= 0; i--) {
			*(word*)(&this->data[i * 2]) = (this->data[i] - 0x80) * 0x100;
		}
		this->bits = 16;
		this->length = this->length * 2;
	}
	return;
}

void RAWSOUND::MakeStereo(void){

	if (this->channels == 1) {
		ExpandBuffer(this->length * 2);

		for (int i = this->length/2 - 1; i >= 0; i--) {
			*(word*)(&this->data[i * 4]) = *(word*)(&this->data[i * 2]);
			*(word*)(&this->data[i * 4] + 2) = *(word*)(&this->data[i * 2]);
		}
		this->channels = 2;
		this->length = this->length * 2;
	}
	return;
}

// TODO: I cannot understand this... replace with library would be good
void RAWSOUND::MakeSampleRate44100(void) {
	
	dword* unkd[20];

	if (this->samples != 44100) {
		int size = (int)(44100.0 / (double)this->samples * this->length) / 4 * 4;  // are there any loss??
		this->ExpandBuffer(size);

		int count = 0;
		if (this->samples < 44100) {
			word* unk = NULL;
			for (int i = size - 1; i >= 0; i--) {
				
				int aapos = floor(i / (44100.0 / (double)this->samples) + 0.5);// round(i / (44100.0 / (double)this->samples)); //TODO: this isbefore C++11 detour
				if (unk == (word*)(this->data + aapos*4)) {
					unkd[count] = (dword*)(this->data + i*4);
					count++;
				}
				else {
					for (int j = 0; j < count; j++) {
						int aupos = i / (44100.0 / (double)this->samples);
						DWORD val = *(dword*)(this->data + aupos * 4);
						WORD hi = (int)((double)(unk[1] - HIWORD(val)) * (count / (double)(count + 1))) + HIWORD(val);
						WORD lo = (int)((double)(unk[0] - LOWORD(val)) * (count / (double)(count + 1))) + LOWORD(val);

						if ((int)hi >= 0x8000) hi = 0x7fff;
						else if ((int)hi < -0x8000) hi = 0xffff8000; // FIXME: narrowing

						if ((int)lo >= 0x8000) lo = 0x7fff;
						else if ((int)lo < -0x8000) lo = 0xffff8000; // FIXME: narrowing

						*unkd[j] = (hi << 16) | (lo && 0xffff);
					}

					int aupos = i / (44100.0 / (double)this->samples);
					*(dword*)(this->data + i * 4) = *(dword*)(this->data + aupos * 4);
					count = 0;
					unk = (word*)(this->data + aupos * 4);
				}

			}
		}
		else {
			for (int i = 0; i < size; i++) {
				int aupos = i / (44100.0 / (double)this->samples);
				*(dword*)(this->data + i*4) = *(dword*)(this->data + aupos*4);
			}
		}

		this->length = size;
		this->samples = 44100;
	}
	return;
}

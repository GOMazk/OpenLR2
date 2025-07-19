#include "structure.h"
#include "FMODex/fmod.h"

const char * GetFMODerror(int errCode);

//fmod init / end
int InitSound(AUDIO *aud, uint bufferLength, int numBuffer, char fDisable, int outputType, int driver);
int EndSound(AUDIO * aud); 

//load file
int IsAltSoundExist(CSTR * filepath);
int SOUND_normalize(AUDIO * aud, SOUNDDATA * sound);
int LoadSound(AUDIO * aud, SOUNDDATA * sound, CSTR filepath, int loop, int disableDSP, int previewFlag);

//unload
int ReleaseSound(AUDIO * aud, SOUNDDATA * sound);

//apply param
int SetSoundVolume(AUDIO * aud, SOUNDDATA * sound, float volume);

int InitFade(AUDIO * aud);
int SetFadeOut(AUDIO * aud, int fadetime);
int SetFadePreview(AUDIO *aud, int fadetime, char quiet);
int SetVolumeByFade(AUDIO * aud);

int SOUND_FmodToDxlib(AUDIO * aud);
int SOUND_dxlibFx(SOUNDDATA sound, int v_master, int v_chn, int pitch, double freq);
int ApplySoundFX(AUDIO * aud, int flag, char disable);

//play
int PlaySound(AUDIO * aud, SOUNDDATA * sound, FMOD_CHANNELGROUP *channelgroup, int stage);
int StopSound(AUDIO * aud, SOUNDDATA * sound);

int SoundGetCurrentTime(AUDIO * aud, SOUNDDATA * sound);

//write file
int GetSoundBuffer(AUDIO * aud, double runtime, int volume);
void WriteSoundFile(AUDIO * aud, CSTR filename, uint size);
int RecordSound(AUDIO * aud, SOUNDDATA * sound, double time, double len);
int RecordFadeout(AUDIO * aud, double from, double length);
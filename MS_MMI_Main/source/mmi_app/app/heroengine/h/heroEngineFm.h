#ifndef __HERO_ENGINE_FM_H__
#define __HERO_ENGINE_FM_H__

int heroFmAudioPause();

int heroFmAudioResume();

int heroFmAudioOpen(char* input, int input_len, char** output, int* output_len);

int heroFmAudioClose();

int heroFmAudioAppendData(char* input, int input_len, char** output, int* output_len);

int heroFmAudioClear();

int heroFmAudioGetCacheSize(char** output, int *output_len);

int heroFmAudioSetVolume(char* input, int input_len);

int heroFmAudioGetVolume(char* input, int input_len);

int heroFmAudioGetMaxCacheSize();

#endif

#ifndef __HERO_TTS_APP_H__
#define __HERO_TTS_APP_H__

enum{
	HERO_TTS_MOUDLE_ID_SIGNAL_BEG = 0,
	HERO_TTS_MOUDLE_ID_START_PCM,
	HERO_TTS_MOUDLE_ID_STOP_PCM,
	HERO_TTS_MOUDLE_ID_CALLBACK,
	HERO_TTS_MOUDLE_ID_SIGNAL_END,
	HERO_TTS_MOUDLE_ID_SIGNAL_MAX,
};
int hero_tts_convert_moudle_id(int id);

typedef void(*hero_tts_play_callback)(int ret, void *userdata);

int hero_tts_start_play(char *string, int stringsize, hero_tts_play_callback callback, void *userdata, int ttsisCall);

int hero_tts_stop_play(void);

int hero_tts_is_play_ring(void);

char *hero_tts_get_string_buf(int *bufsize);

void hero_tts_volume_set(int volume);

#endif

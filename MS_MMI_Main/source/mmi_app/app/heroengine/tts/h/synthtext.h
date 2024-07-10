#ifndef __SYNTH_TEXT_H__
#define __SYNTH_TEXT_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define LOG_DEBUG
//#define LOG_DEBUG		tiho_tts_trace

void tiho_tts_audio_send_data(void *data, int dataSize);

int tiho_tts_start(unsigned short* menu_item_text, unsigned int text_length);

void tiho_tts_stop(void);

void tiho_tts_trace(const char *format, ...);

void *hero_tts_malloc(int size);

void hero_tts_free(void *ptr);

void hero_tts_get_res_dir(char *outBuf);

int hero_tts_check_res(void);

int hero_tts_open_res(void);

void hero_tts_close_res(int handle);

int hero_tts_res_read(int handle, int pos, void *p, unsigned int l);

int tiho_check_res(void);

#define TIHO_TTS_SPEED_MIN					-32768		/* slowest voice speed */
#define TIHO_TTS_SPEED_NORMAL				0			/* normal voice speed (default) */
#define TIHO_TTS_SPEED_MAX					+32767		/* fastest voice speed */
int hero_tts_get_speed(void);

#define TIHO_TTS_READDIGIT_AUTO			0			/* decide automatically (default) */
#define TIHO_TTS_READDIGIT_AS_NUMBER		1			/* say digit as number */
#define TIHO_TTS_READDIGIT_AS_VALUE		2			/* say digit as value */
void tiho_tts_set_read_digit_type(int type);

/*语音合成输出音量增大*/
#define TIHO_TTS_VOLUME_MIN				-32768		/* minimized volume */
#define TIHO_TTS_VOLUME_NORMAL				0			/* normal volume */
#define TIHO_TTS_VOLUME_MAX				+32767		/* maximized volume (default) */
int tiho_get_tts_out_volume();

/*语音合成输出音量增强，可能会导致破音*/
#define TIHO_TTS_VOLUME_INCREASE_MIN				0		/* minimized volume (default) */
#define TIHO_TTS_VOLUME_INCREASE_MAX				10		/* maximized volume */
int tiho_get_tts_out_volume_increase();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

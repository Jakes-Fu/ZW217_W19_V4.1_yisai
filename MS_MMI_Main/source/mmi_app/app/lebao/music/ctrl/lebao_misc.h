#ifndef __LEBAO_MISC_H
#define __LEBAO_MISC_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "port_cfg.h"
#include "lebao_api.h"
#include "lebao_event.h"
#include "lebao_srv.h"
#include "lebao_config.h"
#include "helper_mp3.h"
#include "helper_file.h"

#include "mmiset_export.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
	sds id;
	sds buffer;
} lebao_mp3_buffer_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void 	lebao_port_init(void);
void 	lebao_port_destroy(void);

// lebao_audio.c

// void	lebao_audio_init(void);
// void	lebao_audio_destroy(void);
int		lebao_set_ringtone(const char* filename);
int		lebao_play_file(const char* filename);
int		lebao_play_buffer(void * data, const int size);
int		lebao_play_uri(const char* uri);
int		lebao_pause_music(void);
int		lebao_resume_music(void);
int		lebao_stop_music(void);
int		lebao_get_play_progress(void);
int		lebao_start_record(const char* filename);
int		lebao_stop_record(void);
int		lebao_start_record_stream(link_list_t* stream, int maxDurationMs);

int		lebao_set_volume(const int vol);
int		lebao_get_volume(void);
void	lebao_set_max_volume(const int vol);
int		lebao_get_max_volume(void);
int		lebao_get_volume_step(void);
int		lebao_get_volume_percent(const int vol);
int		lebao_get_playType(void);
int		lebao_play_stream(http_mp3_data_t * data);

int		lebao_get_duration(void);
int		lebao_get_elapsed_seconds(void);
int 	lebao_query_ringtone(MMISET_CALL_MORE_RING_T* filePath, MMISET_CALL_MORE_RING_T* songName);
int 	lebao_set_default_ringtone(void);
int 	lebao_del_default_ringtone(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_MISC_H*/

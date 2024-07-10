/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "agora_rtc_api.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <os_api.h>

#define BWE_MIN_BPS   (150 * 1000)
#define BWE_MAX_BPS   (700 * 1000)
#define BWE_START_BPS (700 * 1000)

#define AVC_TASK_PRIORITY    (67)

#define LOGI(TAG, fmt, ...)          SCI_TRACE_LOW("I/ [AVC] %s" fmt " at %s:%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOGT(TAG, fmt, ...)          SCI_TRACE_LOW("T/ [AVC] %s" fmt " at %s:%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOGD(TAG, fmt, ...)          SCI_TRACE_LOW("D/ [AVC] %s" fmt " at %s:%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOGE(TAG, fmt, ...)          SCI_TRACE_LOW("E/ [AVC] %s" fmt " at %s:%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOGW(TAG, fmt, ...)          SCI_TRACE_LOW("W/ [AVC] %s" fmt " at %s:%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)

typedef enum {
    AVC_LCD_OSD_ID_0 = 0,
    AVC_LCD_OSD_ID_1,
    AVC_LCD_OSD_ID_2,
} avc_lcd_osd_id_e;

typedef enum {
    AVC_DATA_AUDIO = 0,
    AVC_DATA_VIDEO,
    AVC_DATA_TYPE_CNT
} avc_media_type_e;

typedef struct {
    audio_codec_e codec;
    uint32_t      sent_ts;
} avc_audio_info_t;

typedef struct {
    video_codec_e codec;
    bool          is_key_frame;
    uint32_t      fps;
    uint32_t      sent_ts;
} avc_video_info_t;

typedef struct {
    avc_media_type_e     data_type;
    union media_info {
        avc_audio_info_t audio_info;
        avc_video_info_t video_info;
    } u;
} avc_media_info_t;

typedef enum {
    AVC_CODEC_ENCODE = 0,
    AVC_CODEC_DECODE,
} avc_codec_type_e;

typedef void (*hal_media_data_cb)(uint8_t *data, size_t len, avc_media_info_t *info);

/** media APIs */
int32_t hal_media_create(avc_media_type_e media_type, hal_media_data_cb on_media_data);
int32_t hal_video_enable(bool enable);
int32_t hal_media_start(avc_media_type_e media_type);
int32_t hal_media_stop(avc_media_type_e media_type);
void    hal_media_destroy(avc_media_type_e media_type);

/** codec APIs */
int32_t hal_codec_create(hal_media_data_cb on_codec_data, avc_codec_type_e type);
int32_t hal_codec_process(uint8_t *data, size_t len, avc_media_info_t *info, avc_codec_type_e type);
int32_t hal_codec_stop(avc_codec_type_e type);
int32_t hal_codec_keyframe_request(void);
int32_t hal_codec_target_bps_change(uint32_t target_bitrate);
void    hal_codec_destroy(avc_codec_type_e type);

/** render APIs */
int32_t hal_render_enable(avc_media_type_e media_type, bool enable);
int32_t hal_render_create(avc_media_type_e media_type);
int32_t hal_render_process(uint8_t *data, size_t len, avc_media_info_t *info);
int32_t hal_render_stop(avc_media_type_e media_type);
void    hal_render_destroy(avc_media_type_e media_type);
int32_t hal_osd_blend(avc_lcd_osd_id_e osd_id, /* osd id, max osd count = 3 */
                      bool enable,             /* TRUE display osd on video, FALSE remove osd on video */
                      uint8_t *rgba8888,       /* osd format must be RGBA8888, the RGBA8888 buffer pointer */
                      uint16_t left,           /* display location on LCD */
                      uint16_t top,            /* display location on LCD */
                      uint16_t right,          /* display location on LCD */
                      uint16_t bottom          /* display location on LCD */);
int32_t hangup_button_show(bool enable,   /* TRUE display hangup button*/
                      uint16_t left,            /* display location on LCD */
                      uint16_t top,             /* display location on LCD */
                      uint16_t right,           /* display location on LCD */
                      uint16_t bottom           /* display location on LCD */);

/** System APIs */
uint32_t hal_sys_get_time_msec(void);

#ifdef __cplusplus
}
#endif
#endif /* __PLATFORM_H__ */

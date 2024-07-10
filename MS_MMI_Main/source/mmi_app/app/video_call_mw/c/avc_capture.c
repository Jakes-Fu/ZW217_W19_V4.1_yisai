/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include "avc_capture.h"

#define TAG "[capture]"

typedef struct {
    avc_pipeline_node_t pipeline;
    avc_msg_notify_cb   user_message_callback;
} avc_capture_t;

static avc_capture_t* __get_capture_instance(void)
{
    static avc_capture_t *capture = NULL;

    if (NULL == capture) {
        capture = (avc_capture_t *)malloc(sizeof(avc_capture_t));
        assert(capture);
        memset(capture, 0, sizeof(avc_capture_t));
    }

    return capture;
}

static void __register_message_router(avc_capture_t *p_capture, avc_msg_notify_cb message_callback)
{
    p_capture->user_message_callback = message_callback;
}

static int __pipeline_on_ctrl(avc_pipeline_node_t *pipeline, avc_pipeline_event_t *event)
{
    LOGD(TAG, "recv cmd=%d", event->type);

    switch (event->type) {
        case AVC_PIPELINE_START:
            /* step1. start next node first */
            avc_pipeline_push_cmd(pipeline, event);

            /* step2. start audio & video capture */
            hal_media_start(AVC_DATA_AUDIO);

#ifndef CONFIG_AUDIO_ONLY
            hal_media_start(AVC_DATA_VIDEO);
#endif
            break;
        case AVC_PIPELINE_STOP:
            /* step1. stop audio & video capture */
            hal_media_stop(AVC_DATA_AUDIO);

#ifndef CONFIG_AUDIO_ONLY
            hal_media_stop(AVC_DATA_VIDEO);
#endif

            /* step2. stop next node */
            avc_pipeline_push_cmd(pipeline, event);
            break;
        default:
            LOGW(TAG, "unsupport cmd[%d]", event->type);
            break;
    }

    return 0;
}

static void __on_media_audio_data(uint8_t* data, size_t len, avc_media_info_t *media_info)
{
    avc_capture_t* p_capture = __get_capture_instance();
    LOGD(TAG, "capture audio source callback. data=%p, len=%d", data, (int)len);
    avc_pipeline_push_data(&p_capture->pipeline, data, len, media_info);
}

#ifndef CONFIG_AUDIO_ONLY
static void __on_media_video_data(uint8_t* data, size_t len, avc_media_info_t *media_info)
{
    avc_capture_t* p_capture = __get_capture_instance();
    LOGD(TAG, "capture video source callback. data=%p, len=%d", data, (int)len);
    avc_pipeline_push_data(&p_capture->pipeline, data, len, media_info);
}
#endif

avc_pipeline_node_t* avc_capture_create(avc_config_t *p_config, avc_msg_notify_cb message_callback)
{
    int rval;
    avc_capture_t* p_capture = __get_capture_instance();

    __register_message_router(p_capture, message_callback);

    avc_pipeline_node_init(&p_capture->pipeline, __pipeline_on_ctrl, NULL, TAG);

    rval = hal_media_create(AVC_DATA_AUDIO, __on_media_audio_data);
    assert(rval == 0);

#ifndef CONFIG_AUDIO_ONLY
    rval = hal_media_create(AVC_DATA_VIDEO, __on_media_video_data);
    assert(rval == 0);
#endif

    LOGT(TAG, "capture create successfully.");
    return &p_capture->pipeline;
}

void avc_capture_destroy(void)
{
    avc_capture_t* p_capture = __get_capture_instance();

    hal_media_destroy(AVC_DATA_AUDIO);

#ifndef CONFIG_AUDIO_ONLY
    hal_media_destroy(AVC_DATA_VIDEO);
#endif

    memset(p_capture, 0, sizeof(avc_capture_t));
}

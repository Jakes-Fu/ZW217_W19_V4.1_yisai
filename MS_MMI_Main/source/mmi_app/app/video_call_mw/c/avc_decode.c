/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include "avc_decode.h"

#define TAG "[decode]"

typedef struct {
    avc_pipeline_node_t pipeline;
    avc_msg_notify_cb   user_message_callback;

#ifdef DECODE_COST_DEBUG
    uint32_t            decode_total_cost_ms[AVC_DATA_TYPE_CNT];
    uint32_t            decode_times[AVC_DATA_TYPE_CNT];
    uint32_t            decode_start_time[AVC_DATA_TYPE_CNT];
#endif
} avc_decode_t;

static avc_decode_t* __get_decode_instance(void)
{
    static avc_decode_t* decode = NULL;

    if (NULL == decode) {
        decode = (avc_decode_t *)malloc(sizeof(avc_decode_t));
        assert(decode);
        memset(decode, 0, sizeof(avc_decode_t));
    }

    return decode;
}

static void __register_message_router(avc_decode_t *p_decode, avc_msg_notify_cb message_callback)
{
    p_decode->user_message_callback = message_callback;
}

static int __pipeline_on_ctrl(avc_pipeline_node_t *pipeline, avc_pipeline_event_t *event)
{
    LOGD(TAG, "recv cmd=%d", event->type);

    switch (event->type) {
        case AVC_PIPELINE_START:
            /* step1. start next node first */
            avc_pipeline_push_cmd(pipeline, event);

            /* step2. start decode */
            //do nothing now
            break;
        case AVC_PIPELINE_STOP:
            /* step1. stop decode */
            hal_codec_stop(AVC_CODEC_DECODE);

            /* step2. stop next node */
            avc_pipeline_push_cmd(pipeline, event);
            break;
        default:
            break;
    }

    return 0;
}

static int __pipeline_on_data(avc_pipeline_node_t *pipeline,
                              uint8_t *data, size_t len, avc_media_info_t *media_info)
{
    avc_decode_t *p_decode = __get_decode_instance();
    LOGD(TAG, "recv data, len=%d, type=%d", (int)len, media_info->data_type);

#ifdef DECODE_COST_DEBUG
    p_decode->decode_start_time[media_info->data_type] = (media_info->data_type == AVC_DATA_AUDIO ?
                                                          media_info->u.audio_info.sent_ts :
                                                          media_info->u.video_info.sent_ts);
#endif

    return hal_codec_process(data, len, media_info, AVC_CODEC_DECODE);
}

static void _on_decoded_media_data(uint8_t* data, size_t len, avc_media_info_t *media_info)
{
    avc_decode_t *p_decode = __get_decode_instance();
    uint32_t offset;

    LOGD(TAG, "recv decoded data, push to next node");

#ifdef DECODE_COST_DEBUG
    offset = (media_info->data_type == AVC_DATA_AUDIO ?
              media_info->u.audio_info.sent_ts - p_decode->decode_start_time[media_info->data_type] :
              media_info->u.video_info.sent_ts - p_decode->decode_start_time[media_info->data_type]);
    p_decode->decode_total_cost_ms[media_info->data_type] += offset;
    p_decode->decode_times[media_info->data_type]++;
    LOGE(TAG, "[profile.] [%s] times=%u, cost=%ums, avg=%ums, latest=%ums", media_info->data_type == AVC_DATA_AUDIO ? "audio_de" : "video_de",
         p_decode->decode_times[media_info->data_type], p_decode->decode_total_cost_ms[media_info->data_type],
         p_decode->decode_total_cost_ms[media_info->data_type] / p_decode->decode_times[media_info->data_type],
         offset);
#endif

    avc_pipeline_push_data(&p_decode->pipeline, data, len, media_info);
}

avc_pipeline_node_t* avc_decode_create(avc_msg_notify_cb message_callback)
{
    int rval;
    avc_decode_t *p_decode = __get_decode_instance();

    __register_message_router(p_decode, message_callback);

    avc_pipeline_node_init(&p_decode->pipeline, __pipeline_on_ctrl, __pipeline_on_data, TAG);

    rval = hal_codec_create(_on_decoded_media_data, AVC_CODEC_DECODE);
    assert(rval == 0);

    LOGT(TAG, "decode create successfully.");
    return &p_decode->pipeline;
}

void avc_decode_destroy(void)
{
    avc_decode_t *p_decode = __get_decode_instance();

    hal_codec_destroy(AVC_CODEC_DECODE);

    memset(p_decode, 0, sizeof(avc_decode_t));
}

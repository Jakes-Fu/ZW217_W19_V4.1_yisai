/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include "avc_encode.h"

#define TAG "[encode]"

typedef struct {
    avc_pipeline_node_t pipeline;
    avc_msg_notify_cb   user_message_callback;

#ifdef ENCODE_COST_DEBUG
    uint32_t            encode_total_cost_ms[AVC_DATA_TYPE_CNT];
    uint32_t            encode_times[AVC_DATA_TYPE_CNT];
    uint32_t            encode_start_time[AVC_DATA_TYPE_CNT];
#endif
} avc_encode_t;

static avc_encode_t* __get_encode_instance(void)
{
    static avc_encode_t *encode = NULL;

    if (NULL == encode) {
        encode = (avc_encode_t *)malloc(sizeof(avc_encode_t));
        assert(encode);
        memset(encode, 0, sizeof(avc_encode_t));
    }

    return encode;
}

static void __register_message_router(avc_encode_t *p_encode, avc_msg_notify_cb message_callback)
{
    p_encode->user_message_callback = message_callback;
}

static int __pipeline_on_ctrl(avc_pipeline_node_t *pipeline, avc_pipeline_event_t *event)
{
    LOGD(TAG, "recv cmd=%d", event->type);

    switch (event->type) {
        case AVC_PIPELINE_START:
            /* step1. start next node first */
            avc_pipeline_push_cmd(pipeline, event);

            /* step2. start encode */
            //do nothing now
            break;
        case AVC_PIPELINE_STOP:
            /* step1. stop encode */
            hal_codec_stop(AVC_CODEC_ENCODE);

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
    avc_encode_t *p_encode = __get_encode_instance();

    LOGD(TAG, "recv data. type=%d, codec=%d", media_info->data_type,
         media_info->data_type == AVC_DATA_AUDIO ? media_info->u.audio_info.codec : media_info->u.video_info.codec);

#ifdef ENCODE_COST_DEBUG
    p_encode->encode_start_time[media_info->data_type] = (media_info->data_type == AVC_DATA_AUDIO ?
                                                          media_info->u.audio_info.sent_ts :
                                                          media_info->u.video_info.sent_ts);
#endif

    hal_codec_process(data, len, media_info, AVC_CODEC_ENCODE);
    return 0;
}

static void _on_encoded_media_data(uint8_t* data, size_t len, avc_media_info_t *media_info)
{
    avc_encode_t *p_encode = __get_encode_instance();
    uint32_t offset;
    LOGD(TAG, "recv encoded data, push to next node");

#ifdef ENCODE_COST_DEBUG

    offset = (media_info->data_type == AVC_DATA_AUDIO ?
              media_info->u.audio_info.sent_ts - p_encode->encode_start_time[media_info->data_type] :
              media_info->u.video_info.sent_ts - p_encode->encode_start_time[media_info->data_type]);
    p_encode->encode_total_cost_ms[media_info->data_type] += offset;
    p_encode->encode_times[media_info->data_type]++;
    LOGE(TAG, "[profile.] [%s] times=%u, cost=%ums, avg=%ums, latest=%d", media_info->data_type == AVC_DATA_AUDIO ? "audio_en" : "video_en",
         p_encode->encode_times[media_info->data_type], p_encode->encode_total_cost_ms[media_info->data_type],
         p_encode->encode_total_cost_ms[media_info->data_type] / p_encode->encode_times[media_info->data_type],
         offset);
#endif

    avc_pipeline_push_data(&p_encode->pipeline, data, len, media_info);
}

avc_pipeline_node_t* avc_encode_create(avc_msg_notify_cb message_callback)
{
    int rval;
    avc_encode_t *p_encode = __get_encode_instance();

    __register_message_router(p_encode, message_callback);

    avc_pipeline_node_init(&p_encode->pipeline, __pipeline_on_ctrl, __pipeline_on_data, TAG);

    rval = hal_codec_create(_on_encoded_media_data, AVC_CODEC_ENCODE);
    assert(rval == 0);

    LOGT(TAG, "encode create successfully.");
    return &p_encode->pipeline;
}

void avc_encode_destroy(void)
{
    avc_encode_t *p_encode = __get_encode_instance();

    hal_codec_destroy(AVC_CODEC_ENCODE);

    memset(p_encode, 0, sizeof(avc_encode_t));
}

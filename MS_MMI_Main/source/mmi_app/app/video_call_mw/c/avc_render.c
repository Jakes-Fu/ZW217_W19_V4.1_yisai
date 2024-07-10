/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include "avc_render.h"

#define TAG "[render]"

typedef struct {
    avc_pipeline_node_t pipeline;
    avc_msg_notify_cb   user_message_callback;

#ifdef RENDER_COST_DEBUG
    uint32_t            render_total_cost_ms[AVC_DATA_TYPE_CNT];
    uint32_t            render_times[AVC_DATA_TYPE_CNT];
    uint32_t            render_start_time[AVC_DATA_TYPE_CNT];
#endif
} avc_render_t;

static avc_render_t* __get_render_instance(void)
{
    static avc_render_t *render = NULL;

    if (NULL == render) {
        render = (avc_render_t *)malloc(sizeof(avc_render_t));
        assert(render);
        memset(render, 0, sizeof(avc_render_t));
    }

    return render;
}

static void __register_message_router(avc_render_t *p_render, avc_msg_notify_cb message_callback)
{
    p_render->user_message_callback = message_callback;
}

static int __pipeline_on_ctrl(avc_pipeline_node_t *pipeline, avc_pipeline_event_t *event)
{
    avc_render_t *p_render = __get_render_instance();
    LOGD(TAG, "recv cmd=%d", event->type);

    switch (event->type) {
        case AVC_PIPELINE_START:
            /* step1. start next node first */
            avc_pipeline_push_cmd(pipeline, event);//do nothing actually, last node

            break;
        case AVC_PIPELINE_STOP:
             /* step1. stop render */
            hal_render_stop(AVC_DATA_AUDIO);

#ifndef CONFIG_AUDIO_ONLY
            hal_render_stop(AVC_DATA_VIDEO);
#endif

            /* step2. stop next node */
            avc_pipeline_push_cmd(pipeline, event);//do nothing actually, last node
            break;
        default:
            LOGW(TAG, "unsupport cmd=%d", event->type);
            break;
    }

    return 0;
}

static int __pipeline_on_data(avc_pipeline_node_t *pipeline, uint8_t *data, size_t len, avc_media_info_t *media_info)
{
    avc_render_t *p_render = __get_render_instance();

#ifdef RENDER_COST_DEBUG
    uint32_t now;
    p_render->render_start_time[media_info->data_type] = (AVC_DATA_AUDIO == media_info->data_type ?
                                                          media_info->u.audio_info.sent_ts :
                                                          media_info->u.video_info.sent_ts);
#endif

    hal_render_process(data, len, media_info);

#ifdef RENDER_COST_DEBUG
    now = hal_sys_get_time_msec();
    p_render->render_total_cost_ms[media_info->data_type] += (now - p_render->render_start_time[media_info->data_type]);
    p_render->render_times[media_info->data_type]++;

    LOGE(TAG, "[profile.] [%s] times=%u, cost=%ums, avg=%ums", AVC_DATA_AUDIO == media_info->data_type ? "audio_render" : "video_render",
         p_render->render_times[media_info->data_type], p_render->render_total_cost_ms[media_info->data_type],
         p_render->render_total_cost_ms[media_info->data_type] / p_render->render_times[media_info->data_type]);
#endif

    return 0;
}

avc_pipeline_node_t* avc_render_create(avc_msg_notify_cb message_callback)
{
    int rval;
    avc_render_t *p_render = __get_render_instance();

    __register_message_router(p_render, message_callback);

    avc_pipeline_node_init(&p_render->pipeline, __pipeline_on_ctrl, __pipeline_on_data, TAG);

    rval = hal_render_create(AVC_DATA_AUDIO);
    assert(rval == 0);

#ifndef CONFIG_AUDIO_ONLY
    rval = hal_render_create(AVC_DATA_VIDEO);
    assert(rval == 0);
#endif

    LOGT(TAG, "render create successfully.");
    return &p_render->pipeline;
}

void avc_render_destroy(void)
{
    avc_render_t *p_render = __get_render_instance();

    hal_render_destroy(AVC_DATA_AUDIO);

#ifndef CONFIG_AUDIO_ONLY
    hal_render_destroy(AVC_DATA_VIDEO);
#endif

    memset(p_render, 0, sizeof(avc_render_t));
}

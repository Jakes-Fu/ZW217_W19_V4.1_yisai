/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include "avc_rtc.h"
#include "avc_license.h"

#define TAG "[rtc]"

typedef struct {
    avc_pipeline_node_t       pipeline;
    bool                      b_pipeline_started;
    char                      *channel_name;
    avc_msg_notify_cb         user_message_callback;
    bool                      b_user_joined;
    bool                      b_channel_joined;
    int                       video_rotation;

#ifndef CONFIG_AUDIO_ONLY
    bool                      b_first_keyframe_received;
    bool                      b_first_keyframe_sent;
    bool                      b_first_video_frame_reported;
#endif

    agora_rtc_event_handler_t agora_rtc_event_handler;
} rtc_t;

static rtc_t *__get_rtc_instance(void)
{
    static rtc_t *rtc = NULL;

    if (NULL == rtc) {
        rtc = (rtc_t *)malloc(sizeof(rtc_t));
        assert(rtc);
        memset(rtc, 0, sizeof(rtc_t));
    }

    return rtc;
}

static void __send_message_2_user(rtc_t *rtc, avc_msg_t *msg)
{
    if (rtc->user_message_callback) {
        rtc->user_message_callback(msg);
    }
}

static void __pipeline_started(rtc_t *rtc)
{
    rtc->b_pipeline_started = true;
}

static bool __is_pipeline_started(rtc_t *rtc)
{
    return rtc->b_pipeline_started;
}

static void __pipeline_stopped(rtc_t *rtc)
{
    rtc->b_pipeline_started = false;
}

static int __pipeline_on_ctrl(avc_pipeline_node_t *pipeline, avc_pipeline_event_t *event)
{
    rtc_t *rtc = __get_rtc_instance();
    int rval;
    LOGD(TAG, "recv cmd=%d", event->type);

    switch (event->type) {
        case AVC_PIPELINE_START:
            /* step1. start next node first */
            avc_pipeline_push_cmd(pipeline, event);

            /* step2. pipeline flag set */
            __pipeline_started(rtc);
            break;
        case AVC_PIPELINE_STOP:
            /* step1. pipeline flag set */
            __pipeline_stopped(rtc);

            /* step2. leave channel. maybe lost connect. but API has covered this case */
            agora_rtc_leave_channel(rtc->channel_name);

            /* step3. stop next node */
            avc_pipeline_push_cmd(pipeline, event);
            break;
        default:
            LOGW(TAG, "unsupport cmd[%d]", event->type);
            break;
    }

    return 0;
}

#ifndef CONFIG_AUDIO_ONLY
static void __agora_rtc_video_data_send(rtc_t *rtc, uint8_t *data_ptr, size_t data_len, avc_media_info_t *data_info)
{
    int rval;
    video_frame_info_t info;
    uint8_t stream_id   = 0;
    info.codec          = data_info->u.video_info.codec;
    info.frames_per_sec = data_info->u.video_info.fps;
    info.type           = data_info->u.video_info.is_key_frame ? VIDEO_FRAME_KEY : VIDEO_FRAME_DELTA;
    info.rotation       = (video_orientation_e)rtc->video_rotation;
    LOGE(TAG, "[FPS]. fps=%u", data_info->u.video_info.fps);
    rval = agora_rtc_send_video_data(rtc->channel_name, stream_id, data_ptr, data_len, &info);
    if (rval < 0) {
        LOGE(TAG, "send data failed, rval=%d codec=%d len=%d is_key_frame=%d", rval, info.codec, (int)data_len, info.type);
    } else {
        LOGD(TAG, "send data successfully. len=%d", (int)data_len);
    }
}
#endif

static void __agora_rtc_audio_data_send(rtc_t *rtc, uint8_t *data_ptr, size_t data_len, avc_media_info_t *data_info)
{
    int rval;
    audio_frame_info_t info;
    info.codec = data_info->u.audio_info.codec;
    rval = agora_rtc_send_audio_data(rtc->channel_name, data_ptr, data_len, &info);
    if (rval < 0) {
        LOGE(TAG, "send audio data failed, codec=%d len=%d", info.codec, (int)data_len);
    } else {
        LOGD(TAG, "push audio data successfully. len=%d", (int)data_len);
    }
}

#ifndef CONFIG_AUDIO_ONLY
static bool __is_first_keyframe_sent(rtc_t *rtc, avc_media_info_t *data_info)
{
    return rtc->b_first_keyframe_sent || data_info->u.video_info.is_key_frame;
}

static void __set_first_keyframe_sent_flag(rtc_t *rtc)
{
    rtc->b_first_keyframe_sent = true;
}

static void __unset_first_keyframe_sent_flag(rtc_t *rtc)
{
    rtc->b_first_keyframe_sent = false;
}

static void __key_frame_req_msg(rtc_t *rtc)
{
    avc_msg_t msg;
    msg.code = AVC_MSG_KEY_FRAME_REQUEST;
    __send_message_2_user(rtc, &msg);
}

static void __change_encode_bitrate_req_msg(rtc_t *rtc, uint32_t target_bps)
{
    avc_msg_t msg;
    msg.code                    = AVC_MSG_BWE_TARGET_BITRATE_UPDATE;
    msg.data.bwe.target_bitrate = target_bps;
    __send_message_2_user(rtc, &msg);
}
#endif

static int __pipeline_on_data(avc_pipeline_node_t *pipeline, uint8_t *data_ptr, size_t data_len, avc_media_info_t *media_info)
{
    rtc_t *rtc = __get_rtc_instance();
    LOGD(TAG, "recv data. type=%d, codec=%d, len=%d", media_info->data_type,
         media_info->data_type == AVC_DATA_AUDIO ? media_info->u.audio_info.codec : media_info->u.video_info.codec,
         (int)data_len);

    if (!__is_pipeline_started(rtc) || !rtc->b_channel_joined || !rtc->b_user_joined) {
        LOGD(TAG, "pipeline started=%d, joined=%d, user_joined=%d", __is_pipeline_started(rtc), rtc->b_channel_joined, rtc->b_user_joined);
        return 0;
    }

    switch (media_info->data_type) {
        case AVC_DATA_AUDIO:
            __agora_rtc_audio_data_send(rtc, data_ptr, data_len, media_info);
            break;
        case AVC_DATA_VIDEO:

#ifndef CONFIG_AUDIO_ONLY
            if (__is_first_keyframe_sent(rtc, media_info)) {
                __set_first_keyframe_sent_flag(rtc);
                __agora_rtc_video_data_send(rtc, data_ptr, data_len, media_info);
            } else {
                LOGD(TAG, "first frame must be keyframe. now keyframe request first.");
                __key_frame_req_msg(rtc);
            }
#endif
            break;
        default:
            break;
    }

    return 0;
}

static void __register_message_router(rtc_t *rtc, avc_msg_notify_cb message_callback)
{
    rtc->user_message_callback = message_callback;
}

#ifndef CONFIG_AUDIO_ONLY
static bool __first_keyframe_not_receive_now(rtc_t *rtc, int is_key_frame)
{
    return (!rtc->b_first_keyframe_received && !is_key_frame);
}

static void __set_first_keyframe_flag_received(rtc_t *rtc)
{
    rtc->b_first_keyframe_received = true;
}

static void __set_first_keyframe_flag_unreceive(rtc_t *rtc)
{
    rtc->b_first_keyframe_received = false;
}
#endif

static void __on_join_channel_success(const char *channel, int elapsed_ms)
{
    rtc_t *rtc = __get_rtc_instance();
    avc_msg_t msg;

    LOGD(TAG, "ch=%s elapsed_ms=%d", channel, elapsed_ms);

    rtc->b_channel_joined = true;

    msg.code = AVC_MSG_JOIN_CHANNEL_SUCCESS;
    __send_message_2_user(rtc, &msg);
}

static void __error_msg(rtc_t *rtc, avc_msg_e err)
{
    avc_msg_t msg;
    msg.code = err;
    __send_message_2_user(rtc, &msg);
}

static void __on_error(const char *channel, int code, const char *msg)
{
    rtc_t *rtc = __get_rtc_instance();

    switch (code) {
        case ERR_INVALID_APP_ID:
            LOGE(TAG, "Invalid App ID. Please double check. Err message %s", msg);
            __error_msg(rtc, AVC_MSG_INVALID_APP_ID);
            break;
        case ERR_INVALID_CHANNEL_NAME:
            LOGE(TAG, "Invalid channel name \"%s\". Please double check. Err message %s", channel, msg);
            __error_msg(rtc, AVC_MSG_INVALID_CHANNEL_NAME);
            break;
        case ERR_INVALID_TOKEN:
            LOGE(TAG, "Invalid token. Please double check. Err message %s", msg);
            __error_msg(rtc, AVC_MSG_INVALID_TOKEN);
            break;
        case ERR_TOKEN_EXPIRED:
            LOGE(TAG, "Invalid token. Please double check. Err message %s", msg);
            __error_msg(rtc, AVC_MSG_TOKEN_EXPIRED);
            break;
        default:
            LOGE(TAG, "recv errno, code=%u msg=%s", code, msg);
            break;
    }
}

static void __on_user_joined(const char *channel, uint32_t uid, int elapsed_ms)
{
    rtc_t *rtc = __get_rtc_instance();
    avc_msg_t msg;

    LOGD(TAG, "ch=%s uid=%u elapsed_ms=%d.", channel, uid, elapsed_ms);

    rtc->b_user_joined = true;

    msg.code = AVC_MSG_USER_JOINED;
    __send_message_2_user(rtc, &msg);
}

static void __on_user_offline(const char *channel, uint32_t uid, int reason)
{
    rtc_t *rtc = __get_rtc_instance();
    avc_msg_t msg;

    LOGD(TAG, "ch=%s uid=%u reason=%d.", channel, uid, reason);

    rtc->b_user_joined = false;

    msg.code = AVC_MSG_USER_OFFLINE;
    __send_message_2_user(rtc, &msg);
}

#ifndef CONFIG_AUDIO_ONLY
static void __on_key_frame_gen_req(const char *channel, uint32_t uid, uint8_t stream_id)
{
    rtc_t *rtc = __get_rtc_instance();
    LOGD(TAG, "ch=%s uid=%u stream_id=%d", channel, uid, stream_id);

    __unset_first_keyframe_sent_flag(rtc);
    __key_frame_req_msg(rtc);
}
#endif

static void __on_audio_data(const char *channel, uint32_t uid, uint16_t sent_ts, audio_codec_e codec,
                            const void *data_ptr, size_t data_len)
{
    rtc_t *rtc = __get_rtc_instance();
    avc_media_info_t media_info = {0};

    LOGD(TAG, "[%p] ch=%s uid=%u sent_ts=%u codec=%u, data_ptr=%p, data_len=%d", rtc,
         channel, uid, sent_ts, codec, data_ptr, (int)data_len);

    if (NULL == rtc || !__is_pipeline_started(rtc) || !rtc->b_channel_joined || !rtc->b_user_joined) {
        return;
    }

    media_info.data_type            = AVC_DATA_AUDIO;
    media_info.u.audio_info.codec   = codec;
    media_info.u.audio_info.sent_ts = hal_sys_get_time_msec();

    avc_pipeline_push_data(&rtc->pipeline, (uint8_t *)data_ptr, data_len, &media_info);
}

#ifndef CONFIG_AUDIO_ONLY
static void __first_frame_recv_report_msg(rtc_t *rtc)
{
    avc_msg_t msg;
    msg.code = AVC_MSG_FIRST_VIDEO_FRAME_RECV;
    __send_message_2_user(rtc, &msg);
}

static void __report_first_video_frame_2_user()
{
    rtc_t *rtc = __get_rtc_instance();

    if (!rtc->b_first_video_frame_reported) {
        __first_frame_recv_report_msg(rtc);
        rtc->b_first_video_frame_reported = true;
        LOGD(TAG, "first video frame report message sent");
    }
}

static void __on_video_data(const char *channel, uint32_t uid, uint16_t sent_ts, video_codec_e codec,
                            uint8_t stream_id, int is_key_frame, const void *data_ptr, size_t data_len)
{
    rtc_t *rtc = __get_rtc_instance();
    avc_media_info_t media_info = {0};

    LOGD(TAG, "[%p] ch=%s uid=%u sent_ts=%u codec=%u stream_id=%u key_frame=%d data_ptr=%p data_len=%d", rtc,
         channel, uid, sent_ts, codec, stream_id, is_key_frame, data_ptr, (int)data_len);

    if (NULL == rtc || !__is_pipeline_started(rtc) || !rtc->b_channel_joined || !rtc->b_user_joined) {
        LOGD(TAG, "pipeline started=%d, b_channel_joined=%d, b_user_joined=%d",
             __is_pipeline_started(rtc), rtc->b_channel_joined, rtc->b_user_joined);
        return;
    }

    if (__first_keyframe_not_receive_now(rtc, is_key_frame)) {
        LOGD(TAG, "first frame not keyframe. request keyframe first.");
        agora_rtc_request_video_key_frame(channel, uid, stream_id);
        return;
    }

    __set_first_keyframe_flag_received(rtc);

    media_info.data_type                 = AVC_DATA_VIDEO;
    media_info.u.video_info.codec        = codec;
    media_info.u.video_info.is_key_frame = is_key_frame;
    media_info.u.video_info.sent_ts      = hal_sys_get_time_msec();

    avc_pipeline_push_data(&rtc->pipeline, (uint8_t *)data_ptr, data_len, &media_info);

    __report_first_video_frame_2_user();
}

static void __on_target_bitrate_changed(const char *channel, uint32_t target_bps)
{
    rtc_t *rtc = __get_rtc_instance();
    LOGD(TAG, "ch=%s bitrate changed target_bps=%u", channel, target_bps);
    if (!__is_pipeline_started(rtc)) {
        LOGW(TAG, "pipeline donot start, skip bitrate changed process");
        return;
    }

    __change_encode_bitrate_req_msg(rtc, target_bps);
}
#endif

static void __on_connection_lost(const char *channel)
{
    rtc_t *rtc = __get_rtc_instance();
    avc_msg_t msg;

    LOGD(TAG, "ch=%s connetct lost", channel);

    rtc->b_channel_joined = false;

#ifndef CONFIG_AUDIO_ONLY
    __set_first_keyframe_flag_unreceive(rtc);
    __unset_first_keyframe_sent_flag(rtc);
#endif

    msg.code = AVC_MSG_CONNECTION_LOST;
    __send_message_2_user(rtc, &msg);
}

static void __on_rejoin_channel_success(const char *channel, int elapsed_ms)
{
    rtc_t *rtc = __get_rtc_instance();
    LOGD(TAG, "ch=%s elapsed_ms=%d, rejoin channel success", channel, elapsed_ms);

    rtc->b_channel_joined = true;

#ifndef CONFIG_AUDIO_ONLY
    __set_first_keyframe_flag_unreceive(rtc);
    __unset_first_keyframe_sent_flag(rtc);
#endif
}

static void __register_agora_rtc_event_handler(rtc_t *rtc)
{
    rtc->agora_rtc_event_handler.on_join_channel_success   = __on_join_channel_success;
    rtc->agora_rtc_event_handler.on_error                  = __on_error;
    rtc->agora_rtc_event_handler.on_user_joined            = __on_user_joined;
    rtc->agora_rtc_event_handler.on_user_offline           = __on_user_offline;
    rtc->agora_rtc_event_handler.on_audio_data             = __on_audio_data;
#ifndef CONFIG_AUDIO_ONLY
    rtc->agora_rtc_event_handler.on_key_frame_gen_req      = __on_key_frame_gen_req;
    rtc->agora_rtc_event_handler.on_video_data             = __on_video_data;
    rtc->agora_rtc_event_handler.on_target_bitrate_changed = __on_target_bitrate_changed;
#endif
    rtc->agora_rtc_event_handler.on_connection_lost        = __on_connection_lost;
    rtc->agora_rtc_event_handler.on_rejoin_channel_success = __on_rejoin_channel_success;
}

static void __deep_copy_items_destroy(rtc_t *rtc)
{
    if (rtc->channel_name != NULL) {
        free(rtc->channel_name);
        rtc->channel_name = NULL;
    }
}

static int32_t __deep_copy_necessary_items_2_rtc_handle(avc_config_t *p_config, rtc_t *rtc)
{
    rtc->channel_name = (char *)malloc(strlen(p_config->p_channel_name) + 1);
    if (NULL == rtc->channel_name) {
        LOGE(TAG, "alloc channel name failed.");
        goto L_ERROR;
    }
    strcpy(rtc->channel_name, p_config->p_channel_name);

    return 0;

L_ERROR:
    __deep_copy_items_destroy(rtc);
    return -1;
}

#define __MIN(a, b) ((a) > (b) ? (b) : (a))
static int32_t __agora_init(avc_config_t *p_config, rtc_t *rtc)
{
    int rval;
#ifndef CONFIG_AUDIO_ONLY
    int bwe_min_bps = BWE_MIN_BPS, bwe_max_bps = BWE_MAX_BPS, bwe_start_bps = BWE_START_BPS;
#endif
    int token_len = p_config->p_token ? strlen(p_config->p_token) : 0;
    char tmp[64];
    rtc_service_option_t option = {0};

    //step1. agora sdk version.
    LOGE(TAG, "welcome to RTC SDK v%s", agora_rtc_get_version());

    //step2. verify license
#ifdef CONFIG_LICENSE
    rval = agora_rtc_license_verify(p_config->str_certificate, strlen(p_config->str_certificate), DEFAULT_CREDENTIAL, strlen(DEFAULT_CREDENTIAL));
    if (rval < 0) {
        LOGE(TAG, "verify license failed, pls confirm, rval=%d error=%s", rval, agora_rtc_err_2_str(rval));
        return -1;
    }
#endif

    //step3. deep copy.
    rval = __deep_copy_necessary_items_2_rtc_handle(p_config, rtc);
    if (rval != 0) {
        return -1;
    }

    //step4. register event handler
    __register_agora_rtc_event_handler(rtc);

    //step5. set network net_id
    if (p_config->enable_net_param) {
        snprintf(tmp, sizeof(tmp), "{\"rtc.network\": {\"type\": %d, \"id\": %d} }", p_config->net_param_type, p_config->net_param_id);
        agora_rtc_set_params(tmp);
    }

    //step6. agora rtc init.
    option.area_code = AREA_CODE_GLOB;
    if (NULL == p_config->device_id) {
        option.device_id[0] = '\0';
    } else {
        snprintf(option.device_id, sizeof(option.device_id), "%s", p_config->device_id);
    }
    rval = agora_rtc_init(p_config->p_appid, p_config->uid, &rtc->agora_rtc_event_handler, p_config->p_log_dir, &option);
    if (rval < 0) {
        LOGE(TAG, "agora rtc init failed, rval=%d error=%s", rval, agora_rtc_err_2_str(rval));
        __deep_copy_items_destroy(rtc);
        return -1;
    }

#ifndef CONFIG_AUDIO_ONLY
    //step7. set bwe
    if (p_config->enable_bwe_param) {
        bwe_max_bps   = p_config->bwe_param_max_bps;
        bwe_start_bps = __MIN(bwe_start_bps, bwe_max_bps);
    }

    LOGD(TAG, "BWE[%u,%u,%u]", bwe_min_bps, bwe_max_bps, bwe_start_bps);
    rval = agora_rtc_set_bwe_param(bwe_min_bps, bwe_max_bps, bwe_start_bps);
    if (rval < 0) {
        LOGW(TAG, "set BWE failed");
    }
#endif

    //step8. set video rotation
    rtc->video_rotation = p_config->rotation;

    //step9. join channel
    rval = agora_rtc_join_channel(p_config->p_channel_name, p_config->p_token, token_len, NULL);
    if (rval < 0) {
        LOGE(TAG, "join channel %s failed, rval=%d error=%s", rtc->channel_name, rval, agora_rtc_err_2_str(rval));
        __deep_copy_items_destroy(rtc);
        agora_rtc_fini();
        return -1;
    }

    return 0;
}

avc_pipeline_node_t* avc_rtc_create(avc_config_t *p_config, avc_msg_notify_cb message_callback)
{
    rtc_t *rtc = __get_rtc_instance();

    __register_message_router(rtc, message_callback);

    avc_pipeline_node_init(&rtc->pipeline, __pipeline_on_ctrl, __pipeline_on_data, TAG);

    if (0 != __agora_init(p_config, rtc)) {
        return NULL;
    }

    LOGD(TAG, "rtc create successfully.");
    return &rtc->pipeline;
}

void avc_rtc_destroy(void)
{
    rtc_t *rtc = __get_rtc_instance();

    agora_rtc_fini();
    __deep_copy_items_destroy(rtc);

    memset(rtc, 0, sizeof(rtc_t));
    LOGD(TAG, "rtc destroy successfully.");
}

int32_t avc_network_change(int net_type, uint32_t net_id)
{
    char tmp[128];
    int rval = 0;

    snprintf(tmp, sizeof(tmp), "{\"rtc.network\": {\"type\": %d, \"id\": %d, \"update\": true } }", net_type, net_id);
    rval = agora_rtc_set_params(tmp);
    return rval;
}

void avc_mute_local_audio(bool mute)
{
    rtc_t *rtc = __get_rtc_instance();

    if (rtc->b_channel_joined) {
        agora_rtc_mute_local_audio(rtc->channel_name, mute);
    }
}

void avc_mute_remote_video(bool mute)
{
#ifndef CONFIG_AUDIO_ONLY
    hal_render_enable(AVC_DATA_VIDEO, !mute);
#endif
}

void avc_mute_local_video(bool mute)
{
#ifndef CONFIG_AUDIO_ONLY
    hal_video_enable(!mute);
#endif
}

/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include "avc.h"
#include "avc_capture.h"
#include "avc_encode.h"
#include "avc_rtc.h"
#include "avc_decode.h"
#include "avc_render.h"
#include "avc_license.h"

#include "socket_types.h"
#include "IN_Message.h"
#include "socket_api.h"

#define TAG                 "[amf]"
#define AVC_VERSION         "1.5.0"
#define AVC_MAGIC_NUMBER1   (0xbeefdead)
#define AVC_MAGIC_NUMBER2   (0xdeadbeef)
#define AVC_MQ_EXIT         (-1)

typedef enum {
    AVC_ERR_OK = 0,
    AVC_ERR_FAILED,
    AVC_ERR_NOW_RUNNING,
    AVC_ERR_NOW_STOPPED,
    AVC_ERR_NOT_INITIAL,
    AVC_ERR_MAGIC_NUMBER_INVALID,
} avc_error_e;

typedef enum {
    AVC_STOP = 0,
    AVC_START,
} avc_status_e;

typedef struct {
    xSignalHeaderRec header;
    avc_msg_t        msg;
} mq_signal_msg_t;

typedef struct {
    avc_pipeline_node_t root;
    avc_pipeline_node_t *capture;
    avc_pipeline_node_t *encode;
    avc_pipeline_node_t *rtc;
    avc_pipeline_node_t *decode;
    avc_pipeline_node_t *render;
    avc_status_e        status;
    avc_msg_notify_cb   user_message_callback;
    uint32_t            magic_num1;
    uint32_t            magic_num2;
} avc_t;

static bool                           __mq_exist = false;
static uint32_t                       __mq_id    = -1;
static SCI_MUTEX_PTR                  __mutex    = NULL;
static uint32_t                       __dns_id   = -1;
static ASYNC_GETHOSTBYNAME_CNF_SIG_T* __hostname = NULL;

static avc_t* __get_avc_instance(void)
{
    static avc_t *avc = NULL;

    if (NULL == avc) {
        avc = (avc_t *)malloc(sizeof(avc_t));
        assert(avc);
        memset(avc, 0, sizeof(avc_t));
    }

    return avc;
}

static bool __is_avc_status_equal(avc_t *avc, avc_status_e status)
{
    return avc->status == status;
}

static void __register_message_router(avc_t *avc, avc_msg_notify_cb message_callback)
{
    avc->user_message_callback = message_callback;
}

#ifndef CONFIG_AUDIO_ONLY
static bool __is_keyframe_request_msg_process(avc_msg_t *msg, avc_t *avc)
{
    if (AVC_MSG_KEY_FRAME_REQUEST == msg->code) {
        LOGD(TAG, "key frame request.");
        hal_codec_keyframe_request();
        return true;
    }

    return false;
}

static bool __is_encode_target_bitrate_msg_process(avc_msg_t *msg, avc_t *avc)
{
    if (AVC_MSG_BWE_TARGET_BITRATE_UPDATE == msg->code) {
        LOGD(TAG, "target bitrate changed request.");
        hal_codec_target_bps_change(msg->data.bwe.target_bitrate);
        return true;
    }

    return false;
}
#endif

static void __mq(uint32_t argc, void *argv)
{
    bool *inited = (bool *)argv;
    avc_t *avc = __get_avc_instance();
    mq_signal_msg_t *msg;
    char thread_name[SCI_MAX_NAME_SIZE];
    char queue_name[SCI_MAX_NAME_SIZE];
    uint32 state;
    uint32 priority;
    uint32 preempt;

    __mq_exist = true;
    *inited = true;

    while (true) {
        msg = (mq_signal_msg_t *)SCI_GetSignal(SCI_IdentifyThread());
        LOGT(TAG, "recv mq[%u]. code=%d. [sender=%u, code=%d, size=%d]", __mq_id, msg->msg.code,
             msg->header.Sender, msg->header.SignalCode, msg->header.SignalSize);

        SCI_GetThreadInfo(msg->header.Sender, thread_name, queue_name, &state, &priority, &preempt);
        LOGT(TAG, "threadname=%s, q_name=%s, state=%u, priority=%u, preempt=%u", thread_name, queue_name, state, priority, preempt);

        if (msg->msg.code == AVC_MQ_EXIT) {
            LOGT(TAG, "recv mq stop msg");
            free(msg);
            break;
        }

        if (avc->user_message_callback) {
            avc->user_message_callback(&msg->msg);
        }

        free(msg);
    }

    LOGT(TAG, "mq exit");

    __mq_exist = false;
    SCI_ThreadExit();
}

static void __make_sure_mq_not_exist_now(void)
{
    LOGT(TAG, "check mq status");
    while (__mq_exist) SCI_Sleep(2);
    LOGT(TAG, "mq not started now, create new mq");
}

static void __mq_create()
{
    bool inited = false;
    __make_sure_mq_not_exist_now();
    __mq_id = SCI_CreateThread("avc_mq", "q_avc", __mq, 1, &inited, 1024 * 8, 200, 67, SCI_PREEMPT, SCI_AUTO_START);
    while (!inited) SCI_Sleep(2);

    LOGT(TAG, "avc mq create successfully. mq_id=%u", __mq_id);
}

static void __mq_exit_async(void)
{
    mq_signal_msg_t *m;
    m = malloc(sizeof(mq_signal_msg_t));
    assert(m);

    m->header.Sender     = SCI_IdentifyThread();
    m->header.SignalCode = -1;
    m->header.SignalSize = sizeof(mq_signal_msg_t);
    m->msg.code          = AVC_MQ_EXIT;

    LOGT(TAG, "send exit signal. code=%d", m->msg.code);

    SCI_SendSignal((xSignalHeader)m, __mq_id);
}

static void __avc_user_message_router(avc_msg_t *msg)
{
    avc_t *avc = __get_avc_instance();
    mq_signal_msg_t *m;

#ifndef CONFIG_AUDIO_ONLY
    if (__is_keyframe_request_msg_process(msg, avc)) return;

    if (__is_encode_target_bitrate_msg_process(msg, avc)) return;
#endif

    if (__mq_id == -1) {
        LOGW(TAG, "mq not running now.");
        return;
    }

    m = malloc(sizeof(mq_signal_msg_t));
    assert(m);

    m->header.Sender     = SCI_IdentifyThread();
    m->header.SignalCode = -1;
    m->header.SignalSize = sizeof(mq_signal_msg_t);
    memcpy(&m->msg, msg, sizeof(avc_msg_t));

    LOGT(TAG, "msg router msg=%d pid=%u", m->msg.code, __mq_id);

    SCI_SendSignal((xSignalHeader)m, __mq_id);
}

#define LINKING(pre, rear) avc_pipeline_connect(avc->pre, avc->rear)
static void __module_linking(avc_t *avc)
{
    avc_pipeline_connect(&avc->root, avc->capture);
    LINKING(capture, encode);
    LINKING(encode,  rtc);
    LINKING(rtc,     decode);
    LINKING(decode,  render);
}

static void __magic_number_set(avc_t *avc)
{
    avc->magic_num1 = AVC_MAGIC_NUMBER1;
    avc->magic_num2 = AVC_MAGIC_NUMBER2;
}

static void __linking_module_launch(avc_t *avc)
{
    avc_pipeline_node_t *root = &avc->root;
    avc_pipeline_event_t event;
    event.type    = AVC_PIPELINE_START;
    event.content = NULL;
    avc_pipeline_push_cmd(root, &event);
}

static int32_t __is_magic_number_valid(avc_t *avc)
{
    return (avc->magic_num1 == AVC_MAGIC_NUMBER1 &&
            avc->magic_num2 == AVC_MAGIC_NUMBER2);
}

static void __linking_module_descend(avc_t *avc)
{
    avc_pipeline_node_t *root = &avc->root;
    avc_pipeline_event_t event;
    event.type    = AVC_PIPELINE_STOP;
    event.content = NULL;
    avc_pipeline_push_cmd(root, &event);
}

#define HANDLE_DESTROY(node)  if (NULL != avc->node) {    \
                                  avc_##node##_destroy(); \
                                  avc->node = NULL;       \
                              }
static void __avc_pipeline_handle_destroy(avc_t *avc)
{
    HANDLE_DESTROY(capture);
    HANDLE_DESTROY(encode);
    HANDLE_DESTROY(rtc);
    HANDLE_DESTROY(decode);
    HANDLE_DESTROY(render);
}

static void __magic_number_clear(avc_t *avc)
{
    avc->magic_num1 = 0;
    avc->magic_num2 = 0;
}

static void __avc_status_set(avc_t *avc, avc_status_e status)
{
    avc->status = status;
}

int avc_start(avc_config_t *p_config)
{
    avc_t *avc = __get_avc_instance();

    if (NULL == __mutex) {
        LOGE(TAG, "avc not initial, mutex null.");
        return -AVC_ERR_NOT_INITIAL;
    }

    SCI_GetMutex(__mutex, SCI_WAIT_FOREVER);

    //step1. check avc status, must be AVC_STOP
    if (__is_avc_status_equal(avc, AVC_START)) {
        LOGE(TAG, "avc started, status invalid.");
        SCI_PutMutex(__mutex);
        return -AVC_ERR_NOW_RUNNING;
    }

    //step2. register message router to userspace
    __register_message_router(avc, p_config->msg_notify_cb);

    //step3. create pipeline nodes
    avc->capture = avc_capture_create(p_config, __avc_user_message_router);
    avc->encode  = avc_encode_create(__avc_user_message_router);
    avc->rtc     = avc_rtc_create(p_config, __avc_user_message_router);
    avc->decode  = avc_decode_create(__avc_user_message_router);
    avc->render  = avc_render_create(__avc_user_message_router);

    //step4. pipeline node validate
    if (NULL == avc->capture ||
        NULL == avc->encode ||
        NULL == avc->rtc ||
        NULL == avc->decode ||
        NULL == avc->render) {

        LOGE(TAG, "create avc pipeline failed. capture=%0x, encode=%0x, rtc=%0x, decode=%0x, render=%0x",
            avc->capture, avc->encode, avc->rtc, avc->decode, avc->render);

        __avc_pipeline_handle_destroy(avc);

        SCI_PutMutex(__mutex);
        return -AVC_ERR_FAILED;
    }

    //step5. root pipeline init
    avc_pipeline_node_init(&avc->root, NULL, NULL, TAG);

    //step6. pipeline linking
    __module_linking(avc);

    //step7. magic number
    __magic_number_set(avc);

    //step8. create async message queue
    __mq_create();

    //step9. launch pipeline
    __linking_module_launch(avc);

    //step10. set avc status
    __avc_status_set(avc, AVC_START);

    SCI_PutMutex(__mutex);

    LOGD(TAG, "avc start.");
    return AVC_ERR_OK;
}

int avc_stop(void)
{
    avc_t *avc = __get_avc_instance();

    if (NULL == __mutex) {
        LOGE(TAG, "avc stopped, mutex null.");
        return -AVC_ERR_NOW_STOPPED;
    }

    //step1. sync process. lock
    SCI_GetMutex(__mutex, SCI_WAIT_FOREVER);

    //step2. check avc status, must be AVC_START
    if (__is_avc_status_equal(avc, AVC_STOP)) {
        LOGE(TAG, "avc stopped, return.");

        SCI_PutMutex(__mutex);
        return -AVC_ERR_NOW_STOPPED;
    }

    //step3. magic number validate
    if (!__is_magic_number_valid(avc)) {
        LOGE(TAG, "fatal magic number error, memory must be invalid.");

        SCI_PutMutex(__mutex);
        return -AVC_ERR_MAGIC_NUMBER_INVALID;
    }

    //step4. pipeline module descend
    __linking_module_descend(avc);

    //step5. pipeline destroy
    __avc_pipeline_handle_destroy(avc);

    //step6. clear magic number
    __magic_number_clear(avc);

    //step7. set avc status
    __avc_status_set(avc, AVC_STOP);

    //step8. mq exit
    __mq_exit_async();

    //step9. sync process. unlock
    SCI_PutMutex(__mutex);

    LOGD(TAG, "avc stop.");
    return AVC_ERR_OK;
}

const char* avc_get_version(void)
{
    return AVC_VERSION;
}

const char* avc_err_2_str(int err)
{
    if (err < 0) {
        err = 0 - err;
    }

#define VIDEO_CALL_ERR_2_STR_CASE(e)  \
    case e:                           \
        return #e;

    switch (err) {
        VIDEO_CALL_ERR_2_STR_CASE(AVC_ERR_OK);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_ERR_FAILED);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_ERR_NOW_RUNNING);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_ERR_NOW_STOPPED);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_ERR_MAGIC_NUMBER_INVALID);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_JOIN_CHANNEL_SUCCESS);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_USER_JOINED);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_USER_OFFLINE);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_CONNECTION_LOST);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_INVALID_APP_ID);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_INVALID_CHANNEL_NAME);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_INVALID_TOKEN);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_TOKEN_EXPIRED);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_LICENSE_PARAM_INVALID);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_LICENSE_DNS_PARSE_FAILED);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_LICENSE_NET_FAILED);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_LICENSE_ACTIVATE_FAILED);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_KEY_FRAME_REQUEST);
        VIDEO_CALL_ERR_2_STR_CASE(AVC_MSG_BWE_TARGET_BITRATE_UPDATE);
        default:
            break;
    }

    return "N/A";
}

static bool __license_parameter_invalidate(avc_license_t *p_license)
{
    return (p_license &&
            p_license->str_appid &&
            p_license->str_customer_key &&
            p_license->str_customer_secret &&
            p_license->str_license_key &&
            p_license->str_certificate &&
            p_license->str_certificate_len &&
            *p_license->str_certificate_len >= AGORA_CERTIFICATE_MAX_LEN);
}

int avc_osd_blend(avc_osd_id_e osd_id,     /* osd id, max osd count = 3 */
                  bool enable,             /* TRUE display osd on video, FALSE remove osd on video */
                  uint8_t *rgba8888,       /* osd format must be RGBA8888, the RGBA8888 buffer pointer */
                  uint16_t left,           /* display location on LCD */
                  uint16_t top,            /* display location on LCD */
                  uint16_t right,          /* display location on LCD */
                  uint16_t bottom          /* display location on LCD */)
{
    #ifndef CONFIG_AUDIO_ONLY
    avc_lcd_osd_id_e id;

    switch (osd_id) {
        case AVC_OSD_LAYER_ID_0:
            id = AVC_LCD_OSD_ID_0;
            break;
        case AVC_OSD_LAYER_ID_1:
            id = AVC_LCD_OSD_ID_1;
            break;
        case AVC_OSD_LAYER_ID_2:
            id = AVC_LCD_OSD_ID_2;
            break;
        default:
            LOGE(TAG, "unsupport OSD layer id=%d", osd_id);
            return -1;
    }

    return hal_osd_blend(id, enable, rgba8888, left, top, right, bottom);
#else
    return 0;
#endif
}

int avc_hangup_button(bool enable,
                  uint16_t left,
                  uint16_t top,
                  uint16_t right,
                  uint16_t bottom)
{
    hangup_button_show(enable, left, top, right, bottom);
}

/**
 * @brief this API can be called in async mode
 *        multi-thread safety
 */
struct sci_hostent* avc_dns_parse(char *name_ptr, uint32_t netid)
{
    int try_cnt = 2000;
    TCPIP_IPADDR_T hostip = 0;
    ASYNC_GETHOSTBYNAME_CNF_SIG_T* hostname = NULL;

    SCI_GetMutex(__mutex, SCI_WAIT_FOREVER);
    LOGT(TAG, "[DNS][1] start dns parse. url=%s. PID=%u. netid=%d", name_ptr, SCI_IdentifyThread(), netid);

    __hostname = NULL;
    sci_async_gethostbyname(name_ptr, __dns_id, 5 * 1000, netid);

    while (true) {
        if (NULL != __hostname) {
            if (__hostname->error_code != 0) {
                LOGW(TAG, "[DNS][4] dns response err=%d", __hostname->error_code);
                SCI_FREE(__hostname);
                SCI_PutMutex(__mutex);
                return NULL;
            }

            memcpy(&hostip, __hostname->hostent.h_addr_list[0], __hostname->hostent. h_length);
            LOGT(TAG, "[DNS][5] get dns response[%0x]. ip=%s", __hostname, inet_ntoa(hostip));
            hostname = __hostname;
            SCI_PutMutex(__mutex);
            return &hostname->hostent;
        }

        if (try_cnt-- == 0) {
            LOGE(TAG, "[DNS][6] async dns parse has not response, fatal error");
            break;
        }

        SCI_Sleep(5);
    }

    SCI_PutMutex(__mutex);
    return NULL;
}

#define STRUCT_OFFSET(item, type)     (unsigned long)&(((type *)((void *)0))->item)
void avc_dns_parse_free_hostent(struct sci_hostent* hostent)
{
    ASYNC_GETHOSTBYNAME_CNF_SIG_T *dns_sig_ptr = (ASYNC_GETHOSTBYNAME_CNF_SIG_T *)((unsigned char *)hostent - STRUCT_OFFSET(hostent, ASYNC_GETHOSTBYNAME_CNF_SIG_T));
    LOGT(TAG, "[DNS] free ptr=%0x", dns_sig_ptr);

    if (NULL != hostent) {
        SCI_FREE(dns_sig_ptr);
    }
}

/**
 * @brief async dns parse process
 * never exit forever, to make sure dns fastest.
 */
static void __dns_async(uint32_t argc, void *argv)
{
    ASYNC_GETHOSTBYNAME_CNF_SIG_T* dns_sig_ptr;

    while (true) {
        dns_sig_ptr = (ASYNC_GETHOSTBYNAME_CNF_SIG_T *)SCI_GetSignal(SCI_IdentifyThread());
        LOGT(TAG, "[DNS][2] get new signal. code=%d", dns_sig_ptr->SignalCode);

        if (dns_sig_ptr->SignalCode == SOCKET_ASYNC_GETHOSTBYNAME_CNF) {
            LOGT(TAG, "[DNS][3] get async result. err=%d", dns_sig_ptr->error_code);
            __hostname = dns_sig_ptr;
            continue;
        }

        SCI_FREE(dns_sig_ptr);
    }

    SCI_ThreadExit();
}

static void __dns_async_task_create(void) {
    if (__dns_id == -1) {
        __dns_id = SCI_CreateAppThread("dns", "dns_q", __dns_async, 0, NULL, 1024 * 8, 200, 67, SCI_PREEMPT, SCI_AUTO_START);
        LOGT(TAG, "create async dns task.");
    } else {
        LOGT(TAG, "async dns task already exist.");
    }
}

int avc_init(avc_init_t *p_init)
{
    int rval;

    __dns_async_task_create();

    if (NULL == p_init) {
        LOGE(TAG, "p_init null");
        return -1;
    }

    if (NULL == __mutex) {
        __mutex = SCI_CreateMutex("avc", SCI_INHERIT);
        assert(__mutex);
    }

    if (p_init->enable_license_activate) {
        if (!__license_parameter_invalidate(&p_init->license)) {
            LOGE(TAG, "license parameter invalid");
            return -AVC_MSG_LICENSE_PARAM_INVALID;
        }

        rval = avc_license_activate(p_init->license.str_appid,
                                    p_init->license.str_customer_key,
                                    p_init->license.str_customer_secret,
                                    p_init->license.str_license_key,
                                    p_init->license.str_certificate,
                                    p_init->license.str_certificate_len,
                                    p_init->net_id);
        if (rval != 0) {
            LOGE(TAG, "license activate failed.");
            return rval;
        }
    }

    return 0;
}

void avc_fini()
{
    LOGT(TAG, "avc fini done");
}

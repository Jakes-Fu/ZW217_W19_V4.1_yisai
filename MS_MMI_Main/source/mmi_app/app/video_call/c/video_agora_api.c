/*************************************************************
 * File  :  video_call_demo.c
 * Module:  agora_video_call API demo
 *
 * This is a part of the RTSA call Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include <string.h>
#include <os_api.h>
#include <stdint.h>

#include "mmipdp_export.h"
#include "video_call.h"
#include "zdt_net.h"

#ifdef WIN32
int avc_init(avc_init_t *p_init){return 0;}
void avc_fini(void){}
void avc_mute_remote_video(bool mute){}
void avc_mute_local_video(bool mute){}
int avc_start(avc_config_t *p_config){return 0;}
int avc_stop(void){}
const char* avc_err_2_str(int err){return "";}
#endif

#define TAG     "[video_call]"

#define LOGS(TAG, fmt, ...)          SCI_TRACE_LOW("" fmt "\n", ##__VA_ARGS__)
#define LOGI(TAG, fmt, ...)          SCI_TRACE_LOW("I/ %s" fmt " at %s@Line%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOGT(TAG, fmt, ...)          SCI_TRACE_LOW("T/ %s" fmt " at %s@Line%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOGD(TAG, fmt, ...)          SCI_TRACE_LOW("D/ %s" fmt " at %s@Line%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOGE(TAG, fmt, ...)          SCI_TRACE_LOW("E/ %s" fmt " at %s@Line%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOGW(TAG, fmt, ...)          SCI_TRACE_LOW("W/ %s" fmt " at %s@Line%u\n", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__)

typedef enum {
    CMD_START,
    CMD_STOP,
} cmd_e;

typedef struct {
    xSignalHeaderRec header;
    cmd_e            cmd;
    char appId[65];
    char channel_name[65];
    VIDEO_CALL_STATUS_CALLBACK video_call_status;
} video_call_cmd_t;

static uint32_t gs_worker_task_id = -1;

static void avc_msg_notify_callback(avc_msg_t *p_msg)
{
    LOGD(TAG, "recv msg. code=%d", p_msg->code);
    switch (p_msg->code) {
        case AVC_MSG_JOIN_CHANNEL_SUCCESS: /* local本端成功加入channel消息 */
            break;
        case AVC_MSG_USER_JOINED: /* remote远端成功加入channel消息 */
            break;
        case AVC_MSG_USER_OFFLINE: /* remote远端已下线 */
            break;
        case AVC_MSG_CONNECTION_LOST: /* local本端因网络原因掉线 */
            break;
        case AVC_MSG_INVALID_APP_ID: /* appId不合法 */
            break;
        case AVC_MSG_INVALID_CHANNEL_NAME: /* channel name不合法 */
            break;
        case AVC_MSG_INVALID_TOKEN: /* token不合法 */
            break;
        case AVC_MSG_TOKEN_EXPIRED: /* token已过期 */
            break;
        case AVC_MSG_FIRST_VIDEO_FRAME_RECV: /* 加入channel后接收到remote远端第一帧视频数据 */
            break;
        default:
            LOGW(TAG, "no msg handler, code %d", p_msg->code);
            break;
    }
}

static void __action_cmd_start(char *appid, char *channel_name,VIDEO_CALL_STATUS_CALLBACK status_callback)
{
    avc_init_t   init = {0};
    #define      CERTIFICATE_MAX_LEN  (1024)
    static char  str_certificate[CERTIFICATE_MAX_LEN] = {0};
    uint32_t     str_certificate_len = CERTIFICATE_MAX_LEN;
    avc_config_t config = {0};
    int          rval;
    uint32       net_id;

    LOGT(TAG, "enter");

    /**
     * @brief 获取展锐net_id
     * step1. get netid
     * 第一步. 获取展锐网络参数net_id
     */
    net_id = MMIZDT_Net_GetNetID();
    LOGT(TAG, "net_id=%d", net_id);
    if (0 == net_id) {
        LOGE(TAG, "invalid net_id. net_id=%d", net_id);
    #ifndef WIN32
        return;
    #endif
    }

    /**
     * @brief 视频通话SDK全局初始化
     * step2. call avc_init, global initialize of agora video call sdk
     * 第二步. 调用avc_init，进行视频通话SDK全局初始化
     */
    init.net_id                      = net_id;
    init.enable_license_activate     = (str_certificate[0] == '\0');//如果证书已经激活，设置为false即可，init.license元素可以不赋值
    init.license.str_appid           = appid;//"<your appId>";//https://console.agora.io/projects?isTrusted=true 登陆您的账号后，在该路径创建项目。创建后请联系声网商务开通license
    init.license.str_customer_key    = "873ab5598e484d2bb59a52881473857b";//"<your customer key>";//https://console.agora.io/restfulApi 登陆您的账号后，在该路径获取
    init.license.str_customer_secret = "b7e9624420ad4237b8915af2e8a97d52";//"<your customer secret>";//https://console.agora.io/restfulApi 登陆您的账号后，在该路径获取
    init.license.str_license_key     = g_zdt_phone_imei_1;//"<your device IMEI>";//每个设备消耗一个license，该值指代设备，在该项目中必须全局唯一，类似IMEI
    init.license.str_certificate     = str_certificate;//license激活输出结果，作为avc_start启动参数，激活成功后，需将str_certificate字符串存放于flash中，之后需要时从flash加载即可
    init.license.str_certificate_len = &str_certificate_len;//str_certificate_len即是入参，也是出参数，出参指激活后str_certificate的字符串长度，证书长度不超过1KB，长度不固定

    if (0 != (rval = avc_init(&init))) {
        LOGE(TAG, "avc start failed, reason:%s", avc_err_2_str(rval));
        avc_fini();
        return;
    }

    /**
     * @brief 开启视频通话
     * step3. call avc_start, start agora video call
     * 第三步. 调用avc_start，开启视频通话
     */
    config.p_appid = appid;
    config.p_channel_name = channel_name;
    LOGD(TAG, "appid:%s channel_name=%s", config.p_appid, config.p_channel_name);
    //config.p_appid           = "<your appId>";//您创建的项目appId，该值与init.license.str_appid一致
    config.p_token           = NULL;//您创建项目时，若选择 安全模式：APP ID + Token需通过自建token服务器，并从服务器获取您的token https://docs.agora.io/cn/Interactive%20Broadcast/token_server?platform=Android
    config.p_log_dir         = NULL;//日志文件目录，展锐系统直接赋值NULL即可
    //config.p_channel_name    = "<your channel name>";//该值为频道号，视频聊天中有且仅有appid与channel name都相同是才可进行视频互通【即Andorid/IOS端需使用与8910端相同的appid与channel name才可实现视频通话】
    config.msg_notify_cb     = status_callback;//avc_msg_notify_callback;//异步消息回调
    config.enable_net_param  = true;//启用网路，默认设置为true即可
    config.net_param_type    = 0;//当前网路类型
    config.net_param_id      = net_id;//当前网路netid
    config.enable_bwe_param  = true;//启用带宽设置，默认设置为true即可
    config.uid               = 0;//如果您启用了token，uid必须和您生成token时使用的UID一致，如果您不启用token设置为0即可
    config.device_id         = "spreadtrum-w217";//当前设备平台，默认"spreadtrum-w217"即可
    config.bwe_param_max_bps = 600 * 1000;//设备上行最大使用带宽限制，默认600000即可
    config.str_certificate   = str_certificate;//您为该设备激活的证书
    #if defined(W217_W18_VIDEO_CALL_AGORA_DIRECTION)// W18 240X284
    config.rotation          = CAPTURE_ROT_DEFAULT;//CAPTURE_ROT_DEFAULT;//采集视频旋转 // wuxx ZW201_CAMERA_ROTATION   CAPTURE_ROT_DEFAULT--CAPTURE_ROT_CLOCKWISE_90// ZW201_W18 --CAPTURE_ROT_DEFAULT
    #else
    config.rotation          = CAPTURE_ROT_CLOCKWISE_90;//CAPTURE_ROT_DEFAULT;//采集视频旋转 // wuxx ZW201_CAMERA_ROTATION   CAPTURE_ROT_DEFAULT--CAPTURE_ROT_CLOCKWISE_90
    #endif
    if (0 != (rval = avc_start(&config))) {
        LOGE(TAG, "avc start failed, reason:%s", avc_err_2_str(rval));
        avc_fini();
        return;
    }
    /**
     * @brief 开启远端音视频
     * step4. umute remote video and audio
     * 第四步. 开启远端音视频，进入视频聊天
     * 音视频通话请设置为 avc_mute_remote_video(false); avc_mute_local_video(false);
     * 纯音频通话请设置为 avc_mute_remote_video(true); avc_mute_local_video(true);
     */
    avc_mute_remote_video(false);
    avc_mute_local_video(false);

    LOGT(TAG, "exit");
#ifdef WIN32
    {
        avc_msg_t avc_msg ={0};
        avc_msg.code = AVC_MSG_FIRST_VIDEO_FRAME_RECV;
        config.msg_notify_cb(&avc_msg);
    }
    
#endif
}

static void __action_cmd_stop()
{
    LOGT(TAG, "enter");
    avc_stop();
    avc_fini();
    LOGD(TAG, "gs_worker_task_id:%d", gs_worker_task_id);
    LOGT(TAG, "exit");
}

static void __worker_task_entry(uint32 argc, void* argv)
{
    video_call_cmd_t *sig_ptr = NULL;
    bool             *p_b_thread_created = (bool *)argv;

    *p_b_thread_created = true;

    while (true) {
        sig_ptr = (video_call_cmd_t *)SCI_GetSignal(SCI_IdentifyThread());
        assert(sig_ptr);

        LOGT(TAG, "recv signal. cmd=%d", sig_ptr->cmd);

        switch (sig_ptr->cmd) {
            case CMD_START:
                __action_cmd_start(&sig_ptr->appId, &sig_ptr->channel_name, sig_ptr->video_call_status);
                break;
            case CMD_STOP:
                __action_cmd_stop();
                break;
            default:
                break;
        }

        SCI_FREE(sig_ptr);
    }

    SCI_ThreadExit();
    LOGD(TAG, "gs_worker_task_id:%d", gs_worker_task_id);
    gs_worker_task_id = -1;   
}

#define CALL_TASK_THREAD_NAME       "VIDEO_CALL_WORKER_TASK"
#define CALL_TASK_QUEUE_NAME        "Q_VIDEO_CALL_WORKER_TASK"
#define CALL_TASK_STACK_SIZE        (16 * 1024)
#define CALL_TASK_QUEUE_NUM         (100)
#define CALL_TASK_TASK_PRIV         (67)
static void _create_worker_task(void)
{
    static bool b_thread_created = false;
    if (b_thread_created) {
        LOGD(TAG, "thread has created gs_worker_task_id:%d", gs_worker_task_id);
        //SCI_ThreadExit();
        return;
    }

    LOGD(TAG, "enter");

    gs_worker_task_id = SCI_CreateAppThread(CALL_TASK_THREAD_NAME,  // Name string of the thread
                                            CALL_TASK_QUEUE_NAME,   // Queue name string of the thread
                                            __worker_task_entry,    // Entry function of the thread
                                            1,                      // First parameter for entry function,
                                            &b_thread_created,      // Second parameter for entry function,
                                            CALL_TASK_STACK_SIZE,   // Size of the thread stack in bytes
                                            CALL_TASK_QUEUE_NUM,    // Number of messages which can be enqueued
                                            CALL_TASK_TASK_PRIV,    // Priority of the thread.
                                            SCI_PREEMPT,            // Indicates if the thread is preemptable.
                                            SCI_AUTO_START);

    while (!b_thread_created) SCI_Sleep(2);
    LOGD(TAG, "gs_worker_task_id:%d", gs_worker_task_id);

    LOGD(TAG, "exit");
}

PUBLIC int video_call_start(char *appid, char *channel_name, VIDEO_CALL_STATUS_CALLBACK status_callback)
{
    video_call_cmd_t *cmd_ptr = NULL;

    LOGD(TAG, "enter");

    _create_worker_task();

    cmd_ptr = (video_call_cmd_t *)SCI_ALLOCA(sizeof(video_call_cmd_t));
    cmd_ptr->header.Sender     = SCI_IdentifyThread();
    cmd_ptr->header.SignalSize = sizeof(video_call_cmd_t);
    cmd_ptr->header.SignalCode = -1;
    cmd_ptr->cmd               = CMD_START;
    strcpy(cmd_ptr->appId, appid);
    strcpy(cmd_ptr->channel_name, channel_name);
    cmd_ptr->video_call_status = status_callback;

    SCI_SendSignal((xSignalHeader)cmd_ptr, gs_worker_task_id);
    LOGD(TAG, "exit");
    return 0;
}

PUBLIC int video_call_stop(void)
{
    video_call_cmd_t *cmd_ptr = NULL;
    if (-1 == gs_worker_task_id) {
        LOGW(TAG, "video call not start now.");
        return 0;
    }

    LOGD(TAG, "enter");
    cmd_ptr = (video_call_cmd_t *)SCI_ALLOCA(sizeof(video_call_cmd_t));
    cmd_ptr->header.Sender     = SCI_IdentifyThread();
    cmd_ptr->header.SignalSize = sizeof(video_call_cmd_t);
    cmd_ptr->header.SignalCode = -1;
    cmd_ptr->cmd               = CMD_STOP;

    SCI_SendSignal((xSignalHeader)cmd_ptr, gs_worker_task_id);
    LOGD(TAG, "exit");
    return 0;
}

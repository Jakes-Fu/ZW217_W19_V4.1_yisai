/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AGORA_VIDEO_CALL_H__
#define __AGORA_VIDEO_CALL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#ifndef WIN32
#include <stdbool.h>
#else
typedef int bool;
#define false 0
#define true 1
#endif

typedef enum {
    AVC_OSD_LAYER_ID_0 = 0,
    AVC_OSD_LAYER_ID_1,
    AVC_OSD_LAYER_ID_2,
} avc_osd_id_e;

typedef enum {
    AVC_MSG_JOIN_CHANNEL_SUCCESS = 0x1000,
    AVC_MSG_USER_JOINED,
    AVC_MSG_USER_OFFLINE,
    AVC_MSG_CONNECTION_LOST,
    AVC_MSG_INVALID_APP_ID,
    AVC_MSG_INVALID_CHANNEL_NAME,
    AVC_MSG_INVALID_TOKEN,
    AVC_MSG_TOKEN_EXPIRED,
    AVC_MSG_FIRST_VIDEO_FRAME_RECV,

    AVC_MSG_LICENSE_PARAM_INVALID,
    AVC_MSG_LICENSE_DNS_PARSE_FAILED,
    AVC_MSG_LICENSE_NET_FAILED,
    AVC_MSG_LICENSE_ACTIVATE_FAILED,

    AVC_MSG_KEY_FRAME_REQUEST,
    AVC_MSG_BWE_TARGET_BITRATE_UPDATE,
} avc_msg_e;

typedef enum {
    CAPTURE_ROT_DEFAULT = 0,  //默认isp采集模式，不旋转
    CAPTURE_ROT_CLOCKWISE_90, //采集数据顺时针旋转90度
    CAPTURE_ROT_CLOCKWISE_180,//采集数据顺时针旋转180度
    CAPTURE_ROT_CLOCKWISE_270,//采集数据顺时针旋转270度
} avc_rotation_e;

typedef struct {
    uint32_t target_bitrate;
} avc_bwe_t;

typedef struct {
    avc_msg_e     code;

    union {
        avc_bwe_t bwe;
    } data;
} avc_msg_t;

typedef void (*avc_msg_notify_cb)(avc_msg_t *p_msg);

typedef struct {
    const char        *p_appid;         //您向agora注册申请的appid
    uint32_t          uid;              //通话中的终端唯一标识码，置零则系统随机分配
    const char        *device_id;       //设备类型，字符串长度不得大于63
    const char        *p_token;         //token
    const char        *p_channel_name;  //视频通话频道号，不能大于64字节
    const char        *p_log_dir;       //日志输出路径
    avc_msg_notify_cb msg_notify_cb;    //异步消息回调函数
    bool              enable_net_param; //启用网路配置参数
    uint32_t          net_param_id;     //展锐net_id
    uint32_t          net_param_type;   //展锐网路类型0:mobile(3G/4G);1:LAN;
    bool              enable_bwe_param; //启用上行带宽码率控制
    uint32_t          bwe_param_max_bps;//上行带宽最大码率值
    const char        *str_certificate; //证书
    avc_rotation_e    rotation;         //video旋转参数
} avc_config_t;

typedef struct {
    const char *str_appid;          //您向agora注册申请的appid
    const char *str_customer_key;   //客户ID
    const char *str_customer_secret;//客户秘钥
    const char *str_license_key;    //激活license的设备唯一标识码
    char       *str_certificate;    //license激活后输出的证书字符串
    uint32_t   *str_certificate_len;//证书字符串长度
} avc_license_t;

typedef struct {
    bool          enable_license_activate;//true启用激活程序，false不启用激活程序
    avc_license_t license;
    uint32_t      net_id;
} avc_init_t;

/**
 * 视频通话全局初始化函数
 *
 * @note 该接口支持license激活功能，如果您需要激活license请将enable_license_activate设置为true
 *       成功获取到证书后，您可以将证书存储于flash中，无需重复激活
 *       如果您的证书丢失，支持重复激活证书
 *       该函数调用后可以重复【开启、关闭视频通话】
 *
 * @param[in,out] p_init    初始化参数
 *
 * @return
 * - = 0: 成功
 * - < 0: 失败
 */
int avc_init(avc_init_t *p_init);

/**
 * 视频通话全局反初始化函数，用于回收全局资源
 *
 * @note avc_init返回失败也需要调用该接口，以防资源泄漏
 *
 * @param 无
 *
 * @return 无
 */
void avc_fini(void);

/**
 * 启动视频通话
 *
 * @note avc_init成功后，方可开启视频通话功能，在视频通话过程中，不支持重复调用该接口
 *       avc_start与avc_stop必须成队调用，否则接口调用无效
 *
 * @param[in] p_config
 *
 * @return
 * - = 0: 成功
 * - < 0: 失败
 */
int avc_start(avc_config_t *p_config);

/**
 * 结束视频通话
 *
 * @note avc_start与avc_stop必须成对调用，否则接口调用无效
 *
 * @param 无
 *
 * @return
 * - = 0: 成功
 * - < 0: 失败
 */
int avc_stop(void);

/**
 * OSD叠加
 *
 * @note 视频通话过程中，支持最多3个OSD叠加，有且仅支持RGBA格式
 *       8910硬件支持的RGBA格式各分量顺序为BGRA
 *       特别申明，该接口调用一次耗时约35ms，对于静态OSD调用一次即可，不需要频繁调用该接口
 *       该接口调用频率，每秒不能大于1次，否则会对系统整体性能带来负面影响，造成系统不稳定
 *
 * @param[in] osd_id        OSD图层ID号
 * @param[in] enable        true渲染OSD，false移除OSD
 * @param[in] rgba8888      OSD数据源首地址，各分量存储的顺序必须是B、G、R、A
 * @param[in] left          OSD在LCD屏幕上left坐标【坐标不支持拉伸缩放，必须与rgba8888实际大小完全一致】
 * @param[in] top           OSD在LCD屏幕上top坐标【坐标不支持拉伸缩放，必须与rgba8888实际大小完全一致】
 * @param[in] right         OSD在LCD屏幕上right坐标【坐标不支持拉伸缩放，必须与rgba8888实际大小完全一致】
 * @param[in] bottom        OSD在LCD屏幕上bottom坐标【坐标不支持拉伸缩放，必须与rgba8888实际大小完全一致】
 *
 * @return
 * - = 0: 成功
 * - < 0: 失败
 */
int avc_osd_blend(avc_osd_id_e osd_id,
                  bool enable,
                  uint8_t *rgba8888,
                  uint16_t left,
                  uint16_t top,
                  uint16_t right,
                  uint16_t bottom);

/**
 * 切换网络
 *
 * @note 设备网络切换后，必须立即调用该接口，否则将对视频通话服务造成影响
 *
 * @param[in] net_type      0: mobile(3G/4G);  1: LAN;
 * @param[in] net_id        展锐网络net_id
 *
 * @return
 * - = 0: 成功
 * - < 0: 失败
 */
int avc_network_change(int net_type, uint32_t net_id);

/**
 * 本地音频控制
 *
 * @note 默认情况下，本地音频开启，非静音状态
 *
 * @param[in] mute      true：静音本地音频，false：恢复本地音频;
 *
 * @return 无
 */
void avc_mute_local_audio(bool mute);

/**
 * 远端视频控制
 *
 * @note 默认情况下禁用远端视频，目的是为了客户可以控制远端视频渲染到LCD的时机
 *       用户需要avc_mute_remote_video(false)；才可以开启远端视频在LCD渲染功能
 *
 * @param[in] mute      true：禁用远端视频，false：恢复远端视频;
 *
 * @return 无
 */
void avc_mute_remote_video(bool mute);

/**
 * 本地视频控制
 *
 * @note 默认情况下禁用本地视频上行
 *       avc_mute_local_video(false)；才可以开启本地视频上行
 *
 * @param[in] mute      true：禁用本地视频上行，false：恢复本地视频上行;
 *
 * @return 无
 */
void avc_mute_local_video(bool mute);

int avc_hangup_button(bool enable,
                  uint16_t left,
                  uint16_t top,
                  uint16_t right,
                  uint16_t bottom);

/**
 * 获取版本号
 *
 * @note 获取到的版本为字符串常量，切勿调用free释放返回值
 *
 * @param 无
 *
 * @return
 * 版本号
 */
const char* avc_get_version(void);

/**
 * 获取错误码字符串
 *
 * @note 获取到的错误码为字符串常量，切勿调用free释放返回值
 *
 * @param[in] err       error code
 *
 * @return
 * 版本号
 */
const char* avc_err_2_str(int err);

#ifdef __cplusplus
}
#endif
#endif /* __AGORA_VIDEO_CALL_H__ */
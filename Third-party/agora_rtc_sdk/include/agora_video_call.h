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
    CAPTURE_ROT_DEFAULT = 0,  //Ĭ��isp�ɼ�ģʽ������ת
    CAPTURE_ROT_CLOCKWISE_90, //�ɼ�����˳ʱ����ת90��
    CAPTURE_ROT_CLOCKWISE_180,//�ɼ�����˳ʱ����ת180��
    CAPTURE_ROT_CLOCKWISE_270,//�ɼ�����˳ʱ����ת270��
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
    const char        *p_appid;         //����agoraע�������appid
    uint32_t          uid;              //ͨ���е��ն�Ψһ��ʶ�룬������ϵͳ�������
    const char        *device_id;       //�豸���ͣ��ַ������Ȳ��ô���63
    const char        *p_token;         //token
    const char        *p_channel_name;  //��Ƶͨ��Ƶ���ţ����ܴ���64�ֽ�
    const char        *p_log_dir;       //��־���·��
    avc_msg_notify_cb msg_notify_cb;    //�첽��Ϣ�ص�����
    bool              enable_net_param; //������·���ò���
    uint32_t          net_param_id;     //չ��net_id
    uint32_t          net_param_type;   //չ����·����0:mobile(3G/4G);1:LAN;
    bool              enable_bwe_param; //�������д������ʿ���
    uint32_t          bwe_param_max_bps;//���д����������ֵ
    const char        *str_certificate; //֤��
    avc_rotation_e    rotation;         //video��ת����
} avc_config_t;

typedef struct {
    const char *str_appid;          //����agoraע�������appid
    const char *str_customer_key;   //�ͻ�ID
    const char *str_customer_secret;//�ͻ���Կ
    const char *str_license_key;    //����license���豸Ψһ��ʶ��
    char       *str_certificate;    //license����������֤���ַ���
    uint32_t   *str_certificate_len;//֤���ַ�������
} avc_license_t;

typedef struct {
    bool          enable_license_activate;//true���ü������false�����ü������
    avc_license_t license;
    uint32_t      net_id;
} avc_init_t;

/**
 * ��Ƶͨ��ȫ�ֳ�ʼ������
 *
 * @note �ýӿ�֧��license����ܣ��������Ҫ����license�뽫enable_license_activate����Ϊtrue
 *       �ɹ���ȡ��֤��������Խ�֤��洢��flash�У������ظ�����
 *       �������֤�鶪ʧ��֧���ظ�����֤��
 *       �ú������ú�����ظ����������ر���Ƶͨ����
 *
 * @param[in,out] p_init    ��ʼ������
 *
 * @return
 * - = 0: �ɹ�
 * - < 0: ʧ��
 */
int avc_init(avc_init_t *p_init);

/**
 * ��Ƶͨ��ȫ�ַ���ʼ�����������ڻ���ȫ����Դ
 *
 * @note avc_init����ʧ��Ҳ��Ҫ���øýӿڣ��Է���Դй©
 *
 * @param ��
 *
 * @return ��
 */
void avc_fini(void);

/**
 * ������Ƶͨ��
 *
 * @note avc_init�ɹ��󣬷��ɿ�����Ƶͨ�����ܣ�����Ƶͨ�������У���֧���ظ����øýӿ�
 *       avc_start��avc_stop����ɶӵ��ã�����ӿڵ�����Ч
 *
 * @param[in] p_config
 *
 * @return
 * - = 0: �ɹ�
 * - < 0: ʧ��
 */
int avc_start(avc_config_t *p_config);

/**
 * ������Ƶͨ��
 *
 * @note avc_start��avc_stop����ɶԵ��ã�����ӿڵ�����Ч
 *
 * @param ��
 *
 * @return
 * - = 0: �ɹ�
 * - < 0: ʧ��
 */
int avc_stop(void);

/**
 * OSD����
 *
 * @note ��Ƶͨ�������У�֧�����3��OSD���ӣ����ҽ�֧��RGBA��ʽ
 *       8910Ӳ��֧�ֵ�RGBA��ʽ������˳��ΪBGRA
 *       �ر��������ýӿڵ���һ�κ�ʱԼ35ms�����ھ�̬OSD����һ�μ��ɣ�����ҪƵ�����øýӿ�
 *       �ýӿڵ���Ƶ�ʣ�ÿ�벻�ܴ���1�Σ�������ϵͳ�������ܴ�������Ӱ�죬���ϵͳ���ȶ�
 *
 * @param[in] osd_id        OSDͼ��ID��
 * @param[in] enable        true��ȾOSD��false�Ƴ�OSD
 * @param[in] rgba8888      OSD����Դ�׵�ַ���������洢��˳�������B��G��R��A
 * @param[in] left          OSD��LCD��Ļ��left���꡾���겻֧���������ţ�������rgba8888ʵ�ʴ�С��ȫһ�¡�
 * @param[in] top           OSD��LCD��Ļ��top���꡾���겻֧���������ţ�������rgba8888ʵ�ʴ�С��ȫһ�¡�
 * @param[in] right         OSD��LCD��Ļ��right���꡾���겻֧���������ţ�������rgba8888ʵ�ʴ�С��ȫһ�¡�
 * @param[in] bottom        OSD��LCD��Ļ��bottom���꡾���겻֧���������ţ�������rgba8888ʵ�ʴ�С��ȫһ�¡�
 *
 * @return
 * - = 0: �ɹ�
 * - < 0: ʧ��
 */
int avc_osd_blend(avc_osd_id_e osd_id,
                  bool enable,
                  uint8_t *rgba8888,
                  uint16_t left,
                  uint16_t top,
                  uint16_t right,
                  uint16_t bottom);

/**
 * �л�����
 *
 * @note �豸�����л��󣬱����������øýӿڣ����򽫶���Ƶͨ���������Ӱ��
 *
 * @param[in] net_type      0: mobile(3G/4G);  1: LAN;
 * @param[in] net_id        չ������net_id
 *
 * @return
 * - = 0: �ɹ�
 * - < 0: ʧ��
 */
int avc_network_change(int net_type, uint32_t net_id);

/**
 * ������Ƶ����
 *
 * @note Ĭ������£�������Ƶ�������Ǿ���״̬
 *
 * @param[in] mute      true������������Ƶ��false���ָ�������Ƶ;
 *
 * @return ��
 */
void avc_mute_local_audio(bool mute);

/**
 * Զ����Ƶ����
 *
 * @note Ĭ������½���Զ����Ƶ��Ŀ����Ϊ�˿ͻ����Կ���Զ����Ƶ��Ⱦ��LCD��ʱ��
 *       �û���Ҫavc_mute_remote_video(false)���ſ��Կ���Զ����Ƶ��LCD��Ⱦ����
 *
 * @param[in] mute      true������Զ����Ƶ��false���ָ�Զ����Ƶ;
 *
 * @return ��
 */
void avc_mute_remote_video(bool mute);

/**
 * ������Ƶ����
 *
 * @note Ĭ������½��ñ�����Ƶ����
 *       avc_mute_local_video(false)���ſ��Կ���������Ƶ����
 *
 * @param[in] mute      true�����ñ�����Ƶ���У�false���ָ�������Ƶ����;
 *
 * @return ��
 */
void avc_mute_local_video(bool mute);

int avc_hangup_button(bool enable,
                  uint16_t left,
                  uint16_t top,
                  uint16_t right,
                  uint16_t bottom);

/**
 * ��ȡ�汾��
 *
 * @note ��ȡ���İ汾Ϊ�ַ����������������free�ͷŷ���ֵ
 *
 * @param ��
 *
 * @return
 * �汾��
 */
const char* avc_get_version(void);

/**
 * ��ȡ�������ַ���
 *
 * @note ��ȡ���Ĵ�����Ϊ�ַ����������������free�ͷŷ���ֵ
 *
 * @param[in] err       error code
 *
 * @return
 * �汾��
 */
const char* avc_err_2_str(int err);

#ifdef __cplusplus
}
#endif
#endif /* __AGORA_VIDEO_CALL_H__ */
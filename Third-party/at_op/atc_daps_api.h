#ifndef _ATC_DAPS_API_H
#define _ATC_DAPS_API_H
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
//#include "atc_common.h"


#ifdef MQTT_SUPPORT
#include "MQTTAsync.h"
#endif
#define IPERF_PWD_MAX_LEN       20

#if 1
typedef struct
{
    uint8        param_len;
    uint8        param[IPERF_PWD_MAX_LEN];
}IPERF_PWD_PARAM_T;
#endif 

#ifdef MQTT_SUPPORT
typedef struct AT_MQTT_MESSAGE_SIG_
{  
    _SIGNAL_VARS
    uint32 id;     ///< event identifier
    uint32 param1; ///< 1st parameter
}AT_MQTT_MESSAGE_SIG;

typedef enum
{
    DISCONNECTED = 0,
    DISCONNECTING = 1,
    CONNECTED = 2,
    CONNECTING = 3
} EMqttConnectState;

struct MqttOpts
{
    /* MQTT client state */
    EMqttConnectState m_eState;

    /* MQTT options */
    int m_eMQTTVersion;
    char *m_pcTopic;
    char *m_pcClientid;
    int m_iQos;
    char *m_pcUsername;
    char *m_pcPassword;
    char *m_pcHost;
    char *m_pcPort;
    int m_iCleanSession;
    char *m_pcConnection;
    int m_iKeepalive;
    int m_iRetained;

    /* message options */
    char *m_pcMessage;

    /* TLS options */
    int m_iInsecure;
    char *m_pcCapath;
    char *m_pcCert;
    char *m_pcCafile;
    char *m_pcKey;
    char *m_pcKeypass;
    char *m_pcCiphers;
};
#endif
#ifdef MQTT_SUPPORT

#define MQTT_DEBUG_HEAD "PahoMqtt "

#define MQTT_HOSTNAME_LEN_MAX (256)
#define MQTT_PORT_DEF (1883)
#define MQTT_PORT_MIN (1)
#define MQTT_PORT_MAX (65535)
#define MQTT_CLIENTID_LEN_MAX (256)
#define MQTT_KEEPLIVE_DEF (60)
#define MQTT_KEEPLIVE_MIN (10)
#define MQTT_KEEPLIVE_MAX (3600)
#define MQTT_CLEANSESSION_DEF (1)
#define MQTT_CLEANSESSION_MIN (0)
#define MQTT_CLEANSESSION_MAX (1)
#define MQTT_USERNAME_LEN_MAX (256)
#define MQTT_PASSWORD_LEN_MAX (256)
#define MQTT_TOPIC_LEN_MAX (64)
#define MQTT_MESSAGE_LEN_MAX (256)
#define MQTT_QOS_DEF (0)
#define MQTT_QOS_MIN (0)
#define MQTT_QOS_MAX (2)
#define MQTT_DUP_DEF (0)
#define MQTT_DUP_MIN (0)
#define MQTT_DUP_MAX (1)
#define MQTT_RETAIN_DEF (0)
#define MQTT_RETAIN_MIN (0)
#define MQTT_RETAIN_MAX (1)
#define MQTT_SUB_FLAG_DEF (0)
#define MQTT_UNSUB_FLAG (0)
#define MQTT_SUB_FLAG (1)

#define MQTT_EVENT_BASE (0xAC)
#define MQTT_CONN_RSP_SUCC (MQTT_EVENT_BASE + 0)
#define MQTT_CONN_RSP_FAIL (MQTT_EVENT_BASE + 1)
#define MQTT_CONN_RSP_LOST (MQTT_EVENT_BASE + 2)
#define MQTT_PUB_RSP_SUCC (MQTT_EVENT_BASE + 3)
#define MQTT_PUB_RSP_FAIL (MQTT_EVENT_BASE + 4)
#define MQTT_SUB_RSP_SUCC (MQTT_EVENT_BASE + 5)
#define MQTT_SUB_RSP_FAIL (MQTT_EVENT_BASE + 6)
#define MQTT_UNSUB_RSP_SUCC (MQTT_EVENT_BASE + 7)
#define MQTT_UNSUB_RSP_FAIL (MQTT_EVENT_BASE + 8)
#define MQTT_DISCONN_RSP_SUCC (MQTT_EVENT_BASE + 9)
#define MQTT_DISCONN_RSP_FAIL (MQTT_EVENT_BASE + 10)
#endif


#ifdef MQTT_SUPPORT
void MqttOptsInit();
void MqttOptsUninit();
void mqtt_onConnect(void *context, MQTTAsync_successData *response);
void mqtt_onConnectFailure(void *context, MQTTAsync_failureData *response);
void mqtt_ConnectionLostCb(void *context, char *cause);
int mqtt_MessageArrivedCb(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
int MqttConnect(MQTTAsync client);
void onPublish(void *context, MQTTAsync_successData *response);
void onPublishFailure(void *context, MQTTAsync_failureData *response);
int MqttPublish(MQTTAsync client, int datalen, char *data);
void onUnsubscribe(void *context, MQTTAsync_successData *response);
void onUnsubscribeFailure(void *context, MQTTAsync_failureData *response);
int MqttUnsubscribe(MQTTAsync client);
void onSubscribe(void *context, MQTTAsync_successData *response);
void onSubscribeFailure(void *context, MQTTAsync_failureData *response);
int MqttSubscribe(MQTTAsync client);
void onDisconnect(void *context, MQTTAsync_successData *response);
void onDisconnectFailure(void *context, MQTTAsync_failureData *response);
int MqttDisconnect(MQTTAsync client);
#endif




/*****************************************************************************
**                    function define
******************************************************************************/
PUBLIC int iperf_func(void* parameter, uint32 netid, uint8 S_link_id);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                                                                       *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif

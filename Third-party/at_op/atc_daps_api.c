/****************************************************************************
** File Name:      atc_daps_api.c                                           *
** Author:                                                          *
** Date:           2022/10/14                                              *
** Copyright:      2022 uinsoc, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2022/10/14  xirui.zhang.          create
** 
****************************************************************************/
#ifdef PLATFORM_UWS6121E
/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "daps_iperf_api.h"
//#include "atc_common.h"
#include "atc_daps_api.h"





#ifdef MQTT_SUPPORT
struct MqttOpts g_stMqttOpts = {0};
MQTTAsync g_stMqttClient = NULL;
#endif



/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/
PUBLIC int iperf_func(void* parameter, uint32 netid, uint8 S_link_id)
{ 
    IPERF_CONFIG_PARAM_T param_t = {0};
    int res=1;
    uint8 * p_param;
    IPERF_PWD_PARAM_T * param = (IPERF_PWD_PARAM_T *)parameter;

    SCI_TRACE_LOW("param[4]=%s, param[5]=%s, param[6]=%s", param[4].param, param[5].param, param[6].param);

    p_param = param[0].param;
    
    if(*param[0].param=='=')
    {
        p_param++;
    }
    
    if(strcasecmp(p_param, "c") == 0)  
    {
        if(strcasecmp(param[1].param, "tcp") == 0)      //client+tcp===>    -ip  -port  -time -parallel -interval
        {
            if( strcasecmp( param[2].param, "close" ) == 0 )
            {
                res = daps_iperf_stop( IPERF_ROLE_TCP_CLIENT );
            }
            else
            {
                SCI_MEMCPY(param_t.ip.param, param[2].param, param[2].param_len);
                SCI_MEMCPY(param_t.port.param, param[3].param, param[3].param_len);
                SCI_MEMCPY(param_t.time.param, param[4].param, param[4].param_len);
                SCI_MEMCPY(param_t.parallel.param, param[5].param, param[5].param_len);
                SCI_MEMCPY(param_t.interval.param, param[6].param, param[6].param_len);
                SCI_TRACE_LOW("ip=%s port=%s time=%s", param_t.ip.param, param_t.port.param, param_t.time.param);
                res = iperf_tcp_client(&param_t, netid, S_link_id);       
            }
        }
        else if(strcasecmp(param[1].param, "udp") == 0)     //client+udp===>    -ip  -port  -time  -bw -parallel -interval
        {
            if( strcasecmp( param[2].param, "close" ) == 0 )
            {
                res = daps_iperf_stop( IPERF_ROLE_UDP_CLIENT );
            }
            else
            {
                SCI_MEMCPY(param_t.ip.param, param[2].param, param[2].param_len);
                SCI_MEMCPY(param_t.port.param, param[3].param, param[3].param_len);
                SCI_MEMCPY(param_t.time.param, param[4].param, param[4].param_len);
                SCI_MEMCPY(param_t.bw.param, param[5].param, param[5].param_len);
                SCI_MEMCPY(param_t.parallel.param, param[6].param, param[6].param_len);
                SCI_MEMCPY(param_t.interval.param, param[7].param, param[7].param_len);
                res = iperf_udp_client(&param_t, netid, S_link_id); 
            }
        } 
    }
    else if(strcasecmp(p_param, "s") == 0) 
    {
        if(strcasecmp(param[1].param, "tcp") == 0)      //server+tcp===>    -port -parallel -interval
        {
            if( strcasecmp( param[2].param, "close" ) == 0 )
            {
                res = daps_iperf_stop( IPERF_ROLE_TCP_SERVER );
            }
            else
            {
                SCI_MEMCPY(param_t.port.param, param[2].param, param[2].param_len);
                SCI_MEMCPY(param_t.parallel.param, param[3].param, param[3].param_len);
                SCI_MEMCPY(param_t.interval.param, param[4].param, param[4].param_len);
                res = iperf_tcp_server(&param_t, netid, S_link_id);
            }
        }
        else if(strcasecmp(param[1].param, "udp") == 0)     //server+udp===>    -port -interval
        {
            if( strcasecmp( param[2].param, "close" ) == 0 )
            {
                res = daps_iperf_stop( IPERF_ROLE_UDP_SERVER );
            }
            else
            {
                SCI_MEMCPY(param_t.port.param, param[2].param, param[2].param_len);
                SCI_MEMCPY(param_t.interval.param, param[3].param, param[3].param_len);
                res = iperf_udp_server(&param_t, netid, S_link_id);
            }
        }
    }  
    return res;
}

#ifdef MQTT_SUPPORT
uint32 mqtt_netid = 0;
BLOCK_ID g_AT_MQTTMESSAGE_TASK = SCI_INVALID_BLOCK_ID;
unsigned char g_mqtt_current_link_id;

LOCAL void AtMqttPostEvent(uint32 nEventId, uint32 response)
{
    AT_MQTT_MESSAGE_SIG* sig_ptr = NULL;

    if (g_AT_MQTTMESSAGE_TASK != SCI_INVALID_BLOCK_ID)
    {
        SCI_TRACE_LOW(" [MQTT]  [AT+MQTT send signal nEventId id 0x%x]   %s , %d\n",nEventId,__FUNCTION__,__LINE__);
        sig_ptr = (AT_MQTT_MESSAGE_SIG*)malloc(sizeof(AT_MQTT_MESSAGE_SIG));
        if (sig_ptr)
        {
            sig_ptr->id = nEventId;
            sig_ptr->param1 = (uint32)response;
            SCI_SendSignal((xSignalHeader)(sig_ptr), g_AT_MQTTMESSAGE_TASK);
        }
    }
}

//Mqtt Options init & uninit
void MqttOptsInit()
{
    g_stMqttOpts.m_eState = DISCONNECTED;
    g_stMqttOpts.m_eMQTTVersion = MQTTVERSION_DEFAULT;
    g_stMqttOpts.m_pcTopic = NULL;
    g_stMqttOpts.m_pcClientid = NULL;
    g_stMqttOpts.m_iQos = MQTT_QOS_DEF;
    g_stMqttOpts.m_pcUsername = NULL;
    g_stMqttOpts.m_pcPassword = NULL;
    g_stMqttOpts.m_pcHost = NULL;
    g_stMqttOpts.m_pcPort = NULL;
    g_stMqttOpts.m_iCleanSession = MQTT_CLEANSESSION_DEF;
    g_stMqttOpts.m_pcConnection = 0;
    g_stMqttOpts.m_iKeepalive = MQTT_KEEPLIVE_DEF;
    g_stMqttOpts.m_iRetained = MQTT_RETAIN_DEF;
    g_stMqttOpts.m_pcMessage = NULL;
    g_stMqttOpts.m_iInsecure = 0;
    g_stMqttOpts.m_pcCapath = NULL;
    g_stMqttOpts.m_pcCert = NULL;
    g_stMqttOpts.m_pcCafile = NULL;
    g_stMqttOpts.m_pcKey = NULL;
    g_stMqttOpts.m_pcKeypass = NULL;
    g_stMqttOpts.m_pcCiphers = NULL;


    return;
}

void MqttOptsUninit()
{
    g_stMqttOpts.m_eState = DISCONNECTED;
    g_stMqttOpts.m_eMQTTVersion = MQTTVERSION_DEFAULT;
    g_stMqttOpts.m_pcTopic = NULL;
    g_stMqttOpts.m_pcClientid = NULL;
    g_stMqttOpts.m_iQos = MQTT_QOS_DEF;
    g_stMqttOpts.m_pcUsername = NULL;
    g_stMqttOpts.m_pcPassword = NULL;
    g_stMqttOpts.m_pcHost = NULL;
    g_stMqttOpts.m_pcPort = NULL;
    g_stMqttOpts.m_iCleanSession = MQTT_CLEANSESSION_DEF;
    g_stMqttOpts.m_pcConnection = 0;
    g_stMqttOpts.m_iKeepalive = MQTT_KEEPLIVE_DEF;
    g_stMqttOpts.m_iRetained = MQTT_RETAIN_DEF;
    g_stMqttOpts.m_pcMessage = NULL;
    g_stMqttOpts.m_iInsecure = 0;
    g_stMqttOpts.m_pcCapath = NULL;
    g_stMqttOpts.m_pcCert = NULL;
    g_stMqttOpts.m_pcCafile = NULL;
    g_stMqttOpts.m_pcKey = NULL;
    g_stMqttOpts.m_pcKeypass = NULL;
    g_stMqttOpts.m_pcCiphers = NULL;


    return;
}

//Connect
void mqtt_onConnect(void *context, MQTTAsync_successData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Connection success! [%s , %d]\n",__FUNCTION__,__LINE__);
    AtMqttPostEvent(MQTT_CONN_RSP_SUCC, 0x0);

    return;
}

void mqtt_onConnectFailure(void *context, MQTTAsync_failureData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Connection fail! [%s , %d]\n",__FUNCTION__,__LINE__);
    AtMqttPostEvent(MQTT_CONN_RSP_FAIL, 0x0);

    return;
}

static int onSSLError(const char *str, size_t len, void *context)
{
    char cOutstr[64] = {0};
    //MQTTAsync client = (MQTTAsync)context;
    //sys_arch_printf(0, "SSL error: %s\n", str);
    cOutstr[0] = '\0';
    sprintf(cOutstr, "SSL error, %s.",  str);
    SCI_TRACE_LOW(" SSL cOutstr : %s ",cOutstr,__FUNCTION__,__LINE__);

    return 0;
}
int MqttConnect(MQTTAsync client)
{
    MQTTAsync_connectOptions ConnOpts = MQTTAsync_connectOptions_initializer;
    MQTTAsync_SSLOptions ssl_opts = MQTTAsync_SSLOptions_initializer;
    int rc = 0;

    ConnOpts.onSuccess = mqtt_onConnect;
    ConnOpts.onFailure = mqtt_onConnectFailure;
    ConnOpts.cleansession = g_stMqttOpts.m_iCleanSession;

    ConnOpts.keepAliveInterval = g_stMqttOpts.m_iKeepalive;
    ConnOpts.username = g_stMqttOpts.m_pcUsername;
    ConnOpts.password = g_stMqttOpts.m_pcPassword;
    ConnOpts.MQTTVersion = g_stMqttOpts.m_eMQTTVersion;
    ConnOpts.context = client;
    ConnOpts.automaticReconnect = 0;

    SCI_TRACE_LOW(" MQTT  m_pcConnection %s  [%s %d]",g_stMqttOpts.m_pcConnection,__FUNCTION__,__LINE__);
    if (g_stMqttOpts.m_pcConnection && (strncmp(g_stMqttOpts.m_pcConnection, "ssl://", 6) == 0 ||
                            strncmp(g_stMqttOpts.m_pcConnection, "wss://", 6) == 0))
    {
        ssl_opts.verify = 0;
        ssl_opts.CApath = g_stMqttOpts.m_pcCapath;
        //ssl_opts.keyStore = client_crt;
        //ssl_opts.trustStore = CA_CRT;
        //ssl_opts.privateKey = client_key;
        
        ssl_opts.privateKeyPassword = g_stMqttOpts.m_pcKeypass;
        ssl_opts.enabledCipherSuites = g_stMqttOpts.m_pcCiphers;
        ssl_opts.ssl_error_cb = onSSLError;
        ssl_opts.ssl_error_context = client;
        ssl_opts.enableServerCertAuth = 1;
        ssl_opts.sslVersion = MQTT_SSL_VERSION_TLS_1_0;
        ConnOpts.ssl = &ssl_opts;
    }

    rc = MQTTAsync_connect(client, &ConnOpts);

    return rc;
}
#if 1
static void mqtt_ReportInfo(uint8 current_link_id,char *data)
{
    char temp = "\r\n";
    if (data)
    {
        SCI_TRACE_LOW( data);
       // ATC_MUX_Write(current_link_id,"\r\n");
#ifdef _MUX_ENABLE_
        if(ATC_DEBUG_LINK == current_link_id )
        {            
            SIO_ATC_WriteCmdRes(data, strlen((char*)data));
            SIO_ATC_WriteCmdRes(temp, strlen((char*)temp));
        }
        else
        {
            MUX_Write(data, strlen((char*)data),current_link_id);
            MUX_Write(temp, strlen((char*)temp),current_link_id);
        }
#else
#ifdef PPP_USB
        SIO_ATC_WriteCmdRes(data, strlen((char*)data),ATC_GetSioPort());
        SIO_ATC_WriteCmdRes(temp, strlen((char*)temp),ATC_GetSioPort());
#else
        SIO_ATC_WriteCmdRes(data, strlen((char*)data));
        SIO_ATC_WriteCmdRes(temp, strlen((char*)temp));
#endif
#endif
        //ATC_MUX_Write(current_link_id,data);
    }
}
#endif


//Connection Lost Callback
void mqtt_ConnectionLostCb(void *context, char *cause)
{
    SCI_TRACE_LOW(" [ MQTT ]  +MQTTCONN: Connection lost!  [%s %d]  \n", __FUNCTION__,__LINE__);
    AtMqttPostEvent(MQTT_CONN_RSP_LOST, 0x0);

    return;
}

//Message Callback
int mqtt_MessageArrivedCb(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    char *tmpmess = NULL;
    char *outstr = NULL;
    int length = 0;

    if (g_AT_MQTTMESSAGE_TASK != SCI_INVALID_BLOCK_ID)
    {
        tmpmess = (char *)malloc(message->payloadlen + 1);
        if(!tmpmess )
        {
            return 1;
        }
        tmpmess[0] = '\0';
        strncpy(tmpmess, (char *)message->payload, message->payloadlen);
        tmpmess[message->payloadlen] = '\0';
        
        length = topicLen + message->payloadlen + 48;
        outstr = (char *)malloc(length);
        if(!outstr)
        {
            if(tmpmess)
            {
                free(tmpmess);
                tmpmess=NULL;
            }
            return 1;
        }
        outstr[0] = '\0';
        sprintf(outstr, "%s:%s,%d,%s", "+MQTTPUBLISH", topicName, message->payloadlen, tmpmess);
        outstr[((strlen(outstr) < (length - 1)) ? strlen(outstr) : (length - 1))] = '\0';
        mqtt_ReportInfo(g_mqtt_current_link_id, outstr);
        
    }

    if (NULL != message)
    {
        MQTTAsync_freeMessage(&message);
    }

    if (NULL != topicName)
    {
        MQTTAsync_free(topicName);
    }

    if (NULL != tmpmess)
    {
        free(tmpmess);
        tmpmess = NULL;
    }
    if (NULL != outstr)
    {
        free(outstr);
        outstr = NULL;
    }

    return 1;
}

void MQTT_MessageHandle_entry( uint32 argc, void* argv)
{
    uint16 EVENT;
    int thread_top = 0;
    char *outstr = NULL;
    char cOutstr[64] = {0};
    xSignalHeaderRec *sig_ptr = NULL;
    AT_MQTT_MESSAGE_SIG* message = NULL;
    
    SCI_TRACE_LOW("[MQTT]  %s  %d\n",__FUNCTION__,__LINE__);
    g_AT_MQTTMESSAGE_TASK = SCI_IdentifyThread();
    while (1)
    {
        SCI_TRACE_LOW("[MQTT]  AT+MQTT waite signal  [%s ,%d]\n",__FUNCTION__,__LINE__);
        sig_ptr = SCI_GetSignal(g_AT_MQTTMESSAGE_TASK);
        if (NULL != sig_ptr)
        {
            message = (AT_MQTT_MESSAGE_SIG*) sig_ptr;
            EVENT = message->id;
        }
        else
        {
            continue;
        }
        SCI_TRACE_LOW("[MQTT]  EVENT is 0x%x [%s ,%d]\n",EVENT,__FUNCTION__,__LINE__);
        switch (EVENT)
        {
            case MQTT_CONN_RSP_SUCC:
            {
                outstr = "+MQTTCONN: MQTT Connect Success!";
                mqtt_ReportInfo(g_mqtt_current_link_id, outstr);
                g_stMqttOpts.m_eState = CONNECTED;
                break;
            }
            case MQTT_CONN_RSP_FAIL:
            {
                MQTTAsync_destroy(&g_stMqttClient);
                MQTTAsync_uninit();
                outstr = "+MQTTCONN: MQTT Connect Fail!";
                mqtt_ReportInfo(g_mqtt_current_link_id, outstr);
                MqttOptsUninit();
                thread_top = 1;
                break;
            }
            case MQTT_CONN_RSP_LOST:
            {
                SCI_TRACE_LOW(" [ MQTT ]  Connection lost!  [%s %d]  \n", __FUNCTION__,__LINE__);
                cOutstr[0] = '\0';
                sprintf(cOutstr, "%s", "+MQTTDISCONN: Connection Lost!");
                cOutstr[strlen(cOutstr)] = '\0';
                mqtt_ReportInfo(g_mqtt_current_link_id, cOutstr);
                MQTTAsync_destroy(&g_stMqttClient);
                MQTTAsync_uninit();
                MqttOptsUninit();
                thread_top = 1;
                break;
            }
            case MQTT_DISCONN_RSP_SUCC:
            {
                MQTTAsync_destroy(&g_stMqttClient);
                MQTTAsync_uninit();
                outstr = "+MQTTDISCONN: MQTT Disconnect  Success";
                mqtt_ReportInfo(g_mqtt_current_link_id, outstr);
                MqttOptsUninit();
                thread_top = 1;
                break;
            }
            case MQTT_DISCONN_RSP_FAIL:
            {
                cOutstr[0] = '\0';
                sprintf(cOutstr, "%s", "+MQTTDISCONN: MQTT Disconnect Fail.");
                cOutstr[strlen(cOutstr)] = '\0';
                mqtt_ReportInfo(g_mqtt_current_link_id, cOutstr);
                break;
            }
            default:
                break;
        }
        if (sig_ptr)
        {
            free(sig_ptr);
            sig_ptr=NULL;
        }
        if(thread_top)
        {
            mqtt_netid = 0;
            socket_SetNetId(0);
            SCI_TRACE_LOW("[AT+MQTT thread exit]%s     %d\n",__FUNCTION__,__LINE__);
            g_AT_MQTTMESSAGE_TASK = SCI_INVALID_BLOCK_ID;
            SCI_ThreadExit();
        }
    }
}

int AT_MQTT_MessageHandleTask(unsigned char  link_id)
{
    if (g_AT_MQTTMESSAGE_TASK != SCI_INVALID_BLOCK_ID)
    {
        SCI_TRACE_LOW(" [MQTT]  AT+MQTT error  [%s %d]\n",__FUNCTION__,__LINE__);
        return -1;
    }

    g_mqtt_current_link_id = link_id;
    SCI_CreateThread("ATC_MQTT_MESSAGE",
                                  "ATC_MQTT_MESSAGE",
                                   MQTT_MessageHandle_entry,
                                     0, NULL,
                                     1024 * 4,
                                     200,
                                     220,            
                                     1, 
                                     1);

    return 0;
}

//Publish
void onPublish(void *context, MQTTAsync_successData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Publish success! [%s , %d]\n",__FUNCTION__,__LINE__);
    return;
}

void onPublishFailure(void *context, MQTTAsync_failureData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Publish fail! [%s , %d]\n",__FUNCTION__,__LINE__);
    return;
}

int MqttPublish(MQTTAsync client, int datalen, char *data)
{
    MQTTAsync_responseOptions PubRespOpts = MQTTAsync_responseOptions_initializer;
    int rc;

    PubRespOpts.onSuccess = onPublish;
    PubRespOpts.onFailure = onPublishFailure;
    PubRespOpts.context = client;
    rc = MQTTAsync_send(client, g_stMqttOpts.m_pcTopic, datalen, data, g_stMqttOpts.m_iQos, g_stMqttOpts.m_iRetained, &PubRespOpts);

    return rc;
}

//Subscribe & Unsubscribe
void onUnsubscribe(void *context, MQTTAsync_successData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Unsubscribe success! [%s , %d]\n",__FUNCTION__,__LINE__);
    return;
}

void onUnsubscribeFailure(void *context, MQTTAsync_failureData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Unsubscribe fail! [%s , %d]\n",__FUNCTION__,__LINE__);
    return;
}

int MqttUnsubscribe(MQTTAsync client)
{
    MQTTAsync_responseOptions UnsubRespOpts = MQTTAsync_responseOptions_initializer;
    int rc;

    UnsubRespOpts.onSuccess = onUnsubscribe;
    UnsubRespOpts.onFailure = onUnsubscribeFailure;
    UnsubRespOpts.context = client;
    rc = MQTTAsync_unsubscribe(client, g_stMqttOpts.m_pcTopic, &UnsubRespOpts);

    return rc;
}

void onSubscribe(void *context, MQTTAsync_successData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Subscribe success! [%s , %d]\n",__FUNCTION__,__LINE__);
    return;
}

void onSubscribeFailure(void *context, MQTTAsync_failureData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Subscribe fail! [%s , %d]\n",__FUNCTION__,__LINE__);
    return;
}

int MqttSubscribe(MQTTAsync client)
{
    MQTTAsync_responseOptions SubRespOpts = MQTTAsync_responseOptions_initializer;
    int rc;

    SubRespOpts.onSuccess = onSubscribe;
    SubRespOpts.onFailure = onSubscribeFailure;
    SubRespOpts.context = client;
    rc = MQTTAsync_subscribe(client, g_stMqttOpts.m_pcTopic, g_stMqttOpts.m_iQos, &SubRespOpts);

    return rc;
}

//Disconnect
void onDisconnect(void *context, MQTTAsync_successData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Disconnection success! [%s , %d]\n",__FUNCTION__,__LINE__);
    AtMqttPostEvent(MQTT_DISCONN_RSP_SUCC, 0x0);

    return;
}

void onDisconnectFailure(void *context, MQTTAsync_failureData *response)
{
    SCI_TRACE_LOW(" [MQTT]  Disconnection fail! [%s , %d]\n",__FUNCTION__,__LINE__);
    AtMqttPostEvent(MQTT_DISCONN_RSP_FAIL, 0x0);

    return;
}

int MqttDisconnect(MQTTAsync client)
{
    MQTTAsync_disconnectOptions DisconncOpts = MQTTAsync_disconnectOptions_initializer;
    int rc;

    DisconncOpts.onSuccess = onDisconnect;
    DisconncOpts.onFailure = onDisconnectFailure;
    DisconncOpts.context = client;
    rc = MQTTAsync_disconnect(client, &DisconncOpts);

    return rc;
}
#if 0
extern ERR_MNGPRS_CODE_E MNGPRS_ReadPdpContextStateEx( //RETURN: whether success or failure
    MN_DUAL_SYS_E dual_sys,                     //IN:
    MN_GPRS_CON_STATE_T **ret_state_ptr,        //OUT: the memory save the result
    uint32 *pdp_context_num                     //OUT:
);

extern uint8 MNGPRS_GetNsapiByCidEx(MN_DUAL_SYS_E dual_sys, uint8 cid);

uint8 MQTT_MNGPRS_GetPdpContextNsapiEx()
{
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T * ret_state_ptr = 0;
    uint32 pdp_context_num = 0;
    uint8 nsapi = 5;
    int8 i=0;
    
    if(ERR_MNGPRS_NO_ERR == MNGPRS_ReadPdpContextStateEx(dual_sys,  &ret_state_ptr, &pdp_context_num))
    {
        if(ret_state_ptr != 0)
        {
            for(i=0;i<pdp_context_num;i++)
            {
                if (ret_state_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                {
                    nsapi = MNGPRS_GetNsapiByCidEx(dual_sys,ret_state_ptr[i].pdp_id);    
                    SCI_TRACE_LOW(" MQTT  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                    break;
                }
            }
        }
    }
        
    if (nsapi != 0)
    {
        SCI_TRACE_LOW(" MQTT  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
        return nsapi;
    }
    else
    {
        SCI_TRACE_LOW(" MQTT    error  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
        return 0;
    }
}
#endif

#endif



#endif

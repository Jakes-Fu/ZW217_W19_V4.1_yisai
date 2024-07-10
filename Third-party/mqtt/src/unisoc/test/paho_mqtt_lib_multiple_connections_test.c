#include "Thread.h"
#include "../../paho.mqtt.c-master/src/MQTTAsync.h"

#define LOG_HEAD "PahoMqtt "

struct mqtt_opts
{
    /* MQTT options */
    int MQTTVersion;
    char *topic;
    char *clientid;
    int qos;
    char *username;
    char *password;
    char *host;
    char *port;
    char *connection;
    int keepalive;
    int retained;

    /* message options */
    char *message;

    /* TLS options */
    int insecure;
    char *capath;
    char *cert;
    char *cafile;
    char *key;
    char *keypass;
    char *ciphers;
};

static struct mqtt_opts stOptions1 = {0};
static struct mqtt_opts stOptions2 = {0};
static MQTTAsync pstClient1 = NULL;
static MQTTAsync pstClient2 = NULL;

static void onDisconnect(void *context, MQTTAsync_successData *response)
{
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Disconnection success(token is %d).", LOG_HEAD, __func__, __LINE__, response->token);
    OSI_LOGI(0, cOutstr);

    return;
}
static void onDisconnectFailure(void *context, MQTTAsync_failureData *response)
{
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Disconnection fail(token is %d).", LOG_HEAD, __func__, __LINE__, response->token);
    OSI_LOGI(0, cOutstr);

    return;
}

static void onConnect(void *context, MQTTAsync_successData *response)
{
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Connection success(token is %d).", LOG_HEAD, __func__, __LINE__, response->token);
    OSI_LOGI(0, cOutstr);

    return;
}
static void onConnectFailure(void *context, MQTTAsync_failureData *response)
{
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Connection fail(token is %d).", LOG_HEAD, __func__, __LINE__, response->token);
    OSI_LOGI(0, cOutstr);

    return;
}

static void onPublish(void *context, MQTTAsync_successData *response)
{
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Publish success(token is %d).", LOG_HEAD, __func__, __LINE__, response->token);
    OSI_LOGI(0, cOutstr);

    return;
}
static void onPublishFailure(void *context, MQTTAsync_failureData *response)
{
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Publish fail(token is %d).", LOG_HEAD, __func__, __LINE__, response->token);
    OSI_LOGI(0, cOutstr);

    return;
}

static void onSubscribe(void *context, MQTTAsync_successData *response)
{
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Subscribe success(token is %d).", LOG_HEAD, __func__, __LINE__, response->token);
    OSI_LOGI(0, cOutstr);

    return;
}
static void onSubscribeFailure(void *context, MQTTAsync_failureData *response)
{
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Subscribe fail(token is %d).", LOG_HEAD, __func__, __LINE__, response->token);
    OSI_LOGI(0, cOutstr);

    return;
}

//Init lock resouce
static void UnisocMqttInit(void)
{
    MQTTAsync_init();

    return;
}

//Uninit lock resouce
static void UnisocMqttUninit(void)
{
    MQTTAsync_uninit();

    return;
}

static int messageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    char *tmpmess = NULL;
    char *outstr = NULL;
    int length = 0;

    if ((NULL != message) && (0 != message->payloadlen))
    {
        tmpmess = (char *)malloc(message->payloadlen + 1);
    }

    if (NULL != tmpmess)
    {
        tmpmess[0] = '\0';
        strncpy(tmpmess, (char *)message->payload, message->payloadlen);
        tmpmess[message->payloadlen] = '\0';
    }

    length = topicLen + message->payloadlen + 48;

    outstr = (char *)malloc(length);
    if (NULL != outstr)
    {
        outstr[0] = '\0';
        sprintf(outstr, "%s:%s,%d,%s", LOG_HEAD, topicName, message->payloadlen, tmpmess);
        outstr[((strlen(outstr) < (length - 1)) ? strlen(outstr) : (length - 1))] = '\0';
        OSI_LOGI(0, outstr);
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

static int UnisocMqttCreate()
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};
    char cUrl1[128] = {
        0,
    };
    char cUrl2[128] = {
        0,
    };
    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test create of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    stOptions1.MQTTVersion = MQTTVERSION_3_1_1;
    stOptions1.host = "211.95.108.52"; //"test.mosquitto.org";
    stOptions1.port = "1883";
    stOptions1.username = "zymqtt"; //NULL;
    stOptions1.password = "zymqtt"; //NULL;
    stOptions1.clientid = "UnisocClient1";
    stOptions1.keepalive = 30;
    stOptions1.retained = 0;

    sprintf(cUrl1, "%s:%s", stOptions1.host, stOptions1.port);
    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Url1 \"%s\"", LOG_HEAD, __func__, __LINE__, cUrl1);
    OSI_LOGI(0, cOutstr);

    rc = MQTTAsync_create(&pstClient1, cUrl1, stOptions1.clientid, 1, NULL);
    if (rc != MQTTASYNC_SUCCESS)
    {
        rc = -1;
        goto finish;
    }

    MQTTAsync_setCallbacks(pstClient1, pstClient1, NULL, messageArrived, NULL);

    stOptions2.MQTTVersion = MQTTVERSION_3_1_1;
    stOptions2.host = "211.95.108.52"; //"test.mosquitto.org";
    stOptions2.port = "1883";
    stOptions2.username = "zymqtt"; //NULL;
    stOptions2.password = "zymqtt"; //NULL;
    stOptions2.clientid = "UnisocClient2";
    stOptions2.keepalive = 30;
    stOptions2.retained = 0;

    sprintf(cUrl2, "%s:%s", stOptions2.host, stOptions2.port);
    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Url2 \"%s\"", LOG_HEAD, __func__, __LINE__, cUrl2);
    OSI_LOGI(0, cOutstr);

    rc = MQTTAsync_create(&pstClient2, cUrl2, stOptions2.clientid, 1, NULL);
    if (rc != MQTTASYNC_SUCCESS)
    {
        rc = -1;
        goto finish;
    }

    MQTTAsync_setCallbacks(pstClient2, pstClient2, NULL, messageArrived, NULL);

finish:
    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test create finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);
    return rc;
}

static int UnisocMqttConnection(int unmber, MQTTAsync client)
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};
    MQTTAsync_connectOptions ConnOpts = MQTTAsync_connectOptions_initializer;

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test connection of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    ConnOpts.onSuccess = onConnect;
    ConnOpts.onFailure = onConnectFailure;
    ConnOpts.cleansession = 1;
    ConnOpts.context = client;
    ConnOpts.automaticReconnect = 1;
    if (1 == unmber)
    {
        ConnOpts.keepAliveInterval = stOptions1.keepalive;
        ConnOpts.username = stOptions1.username;
        ConnOpts.password = stOptions1.password;
        ConnOpts.MQTTVersion = stOptions1.MQTTVersion;
    }
    else if (2 == unmber)
    {
        ConnOpts.keepAliveInterval = stOptions2.keepalive;
        ConnOpts.username = stOptions2.username;
        ConnOpts.password = stOptions2.password;
        ConnOpts.MQTTVersion = stOptions2.MQTTVersion;
    }

    rc = MQTTAsync_connect(client, &ConnOpts);

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test connection finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);
    return rc;
}

static int UnisocMqttSubscribe(int unmber, MQTTAsync client)
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};
    char *pcTmpTopic = NULL;
    int TmpQos = -1;
    MQTTAsync_responseOptions SubRespOpts = MQTTAsync_responseOptions_initializer;

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test subscribe of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    SubRespOpts.onSuccess = onSubscribe;
    SubRespOpts.onFailure = onSubscribeFailure;
    SubRespOpts.context = client;
    if (1 == unmber)
    {
        pcTmpTopic = stOptions1.topic = "/unisoc/topic/test_1";
        TmpQos = stOptions1.qos = 0;
    }
    else if (2 == unmber)
    {
        pcTmpTopic = stOptions2.topic = "/unisoc/topic/test_2";
        TmpQos = stOptions2.qos = 0;
    }

    rc = MQTTAsync_subscribe(client, pcTmpTopic, TmpQos, &SubRespOpts);

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test subscribe finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);
    return rc;
}

static int UnisocMqttPublish(int unmber, MQTTAsync client)
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};
    char *pcTmpTopic = NULL;
    char *pcTmpMessage = NULL;
    int TmpQos = -1;
    int TmpRetained = -1;
    MQTTAsync_responseOptions PubRespOpts = MQTTAsync_responseOptions_initializer;

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test publish of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    PubRespOpts.onSuccess = onPublish;
    PubRespOpts.onFailure = onPublishFailure;
    PubRespOpts.context = client;
    if (1 == unmber)
    {
        pcTmpTopic = stOptions1.topic = "/unisoc/topic/test_1";
        pcTmpMessage = stOptions1.message = "message.test_1";
        TmpQos = stOptions1.qos = 0;
        TmpRetained = stOptions1.retained;
    }
    else if (2 == unmber)
    {
        pcTmpTopic = stOptions2.topic = "/unisoc/topic/test_2";
        pcTmpMessage = stOptions2.message = "message.test_2";
        TmpQos = stOptions2.qos = 0;
        TmpRetained = stOptions2.retained;
    }
    rc = MQTTAsync_send(client, pcTmpTopic, strlen(pcTmpMessage), pcTmpMessage, TmpQos, TmpRetained, &PubRespOpts);

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test publish finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);
    return rc;
}

static int UnisocMqttDisconnect(MQTTAsync client)
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};
    MQTTAsync_disconnectOptions DisconncOpts = MQTTAsync_disconnectOptions_initializer;

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test disconnection of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    DisconncOpts.onSuccess = onDisconnect;
    DisconncOpts.onFailure = onDisconnectFailure;
    DisconncOpts.context = client;
    rc = MQTTAsync_disconnect(client, &DisconncOpts);

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test disconnection finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);
    return rc;
}

static int UnisocMqttDestroy(MQTTAsync client)
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test destroy of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    MQTTAsync_destroy(&client);

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test destroy finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);
    return rc;
}

void paho_mqtt_lib_multiple_connections_test(void)
{
    UnisocMqttInit();
    osiThreadSleep(100);

    UnisocMqttCreate();
    osiThreadSleep(500);
    UnisocMqttConnection(1, pstClient1);
    UnisocMqttConnection(2, pstClient2);
    osiThreadSleep(1000);
    UnisocMqttSubscribe(1, pstClient1);
    UnisocMqttSubscribe(2, pstClient2);
    osiThreadSleep(1000);
    UnisocMqttPublish(1, pstClient1);
    UnisocMqttPublish(2, pstClient2);
    osiThreadSleep(1000);
    UnisocMqttDisconnect(pstClient1);
    UnisocMqttDisconnect(pstClient2);
    osiThreadSleep(1000);
    UnisocMqttDestroy(pstClient1);
    UnisocMqttDestroy(pstClient2);
    osiThreadSleep(500);

    UnisocMqttUninit();
    return;
}

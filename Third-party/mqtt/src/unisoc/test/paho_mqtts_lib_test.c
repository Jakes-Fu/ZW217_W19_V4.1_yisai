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

static struct mqtt_opts opts = {0};
static MQTTAsync pstClient = NULL;

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

static int disconnected = 0;
static void onDisconnect(void *context, MQTTAsync_successData *response)
{
    char cOutstr[64] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Disconnection success.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    disconnected = 1;
}
static void onDisconnectFailure(void *context, MQTTAsync_failureData *response)
{
    char cOutstr[64] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Disconnection fail.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    disconnected = -1;
}

static int connected = 0;
static void onConnect(void *context, MQTTAsync_successData *response)
{
    char cOutstr[64] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Connection success.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    connected = 1;
}
static void onConnectFailure(void *context, MQTTAsync_failureData *response)
{
    char cOutstr[128] = {0};

    if (NULL != response->message)
    {
        cOutstr[0] = '\0';
        sprintf(cOutstr, "%s%s(%d):Connection faile. errno(%d). msg:%s.", LOG_HEAD, __func__, __LINE__, response->code, response->message);
        OSI_LOGI(0, cOutstr);
    }
    else
    {
        cOutstr[0] = '\0';
        sprintf(cOutstr, "%s%s(%d):Connection fail. errno(%d).", LOG_HEAD, __func__, __LINE__, response->code);
        OSI_LOGI(0, cOutstr);
    }
    connected = -1;
}

static int onSSLError(const char *str, size_t len, void *context)
{
    char cOutstr[64] = {0};
    //MQTTAsync client = (MQTTAsync)context;
    //sys_arch_printf(0, "SSL error: %s\n", str);
    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):SSL error, %s.", LOG_HEAD, __func__, __LINE__, str);
    OSI_LOGI(0, cOutstr);

    return 0;
}

static int published = 0;
static void onPublish(void *context, MQTTAsync_successData *response)
{
    char cOutstr[64] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Publish success.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    published = 1;
}
static void onPublishFailure(void *context, MQTTAsync_failureData *response)
{
    char cOutstr[64] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Publish fail.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    published = -1;
}

static int subscribed = 0;
static void onSubscribe(void *context, MQTTAsync_successData *response)
{
    char cOutstr[64] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Subscribe success.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    subscribed = 1;
}
static void onSubscribeFailure(void *context, MQTTAsync_failureData *response)
{
    char cOutstr[64] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Subscribe fail.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    subscribed = -1;
}

static void MqttInit(void)
{
    MQTTAsync_init();

    return;
}

static const char *CA_CRT = "-----BEGIN CERTIFICATE-----\n\
MIICvzCCAiigAwIBAgIJAOEH30RV037pMA0GCSqGSIb3DQEBCwUAMHYxCzAJBgNV\n\
BAYTAkNOMRAwDgYDVQQIDAd0aWFuamluMQswCQYDVQQHDAJUSjEMMAoGA1UECgwD\n\
UkRBMQwwCgYDVQQLDANSREExDDAKBgNVBAMMA1JEQTEeMBwGCSqGSIb3DQEJARYP\n\
Y2FAcmRhbWljcm8uY29tMCAXDTE4MDkxOTA5MDYxM1oYDzIxMTgwODI2MDkwNjEz\n\
WjB2MQswCQYDVQQGEwJDTjEQMA4GA1UECAwHdGlhbmppbjELMAkGA1UEBwwCVEox\n\
DDAKBgNVBAoMA1JEQTEMMAoGA1UECwwDUkRBMQwwCgYDVQQDDANSREExHjAcBgkq\n\
hkiG9w0BCQEWD2NhQHJkYW1pY3JvLmNvbTCBnzANBgkqhkiG9w0BAQEFAAOBjQAw\n\
gYkCgYEAviKxja+DM/W7JCwf3QK8mYmlF3a4YABgMEwAlW4S/ulZ50HGBBzEMHj7\n\
Q0GANnTGSJFaSpbOkjA8TJ5fly/mLffz1GYcgkU2lCiiDckh1WXJOwZsivAogoML\n\
0LmLTN7xHXzV2lN10U+fDM+v3AD1rEJlDVoZ5z4d7SwsSj7qW8kCAwEAAaNTMFEw\n\
HQYDVR0OBBYEFG3UUGWA/6v3YQTZB+CDey0JJ4nXMB8GA1UdIwQYMBaAFG3UUGWA\n\
/6v3YQTZB+CDey0JJ4nXMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQAD\n\
gYEAGyeuyAqK8RgCSwoDJGj358cEURpC0FT/KuH+43XrkUHCJCMhH2zcaeOr6eBG\n\
ws0bYutiSRLnxMb5LP1tha+xRBiruykIuNCWoK3z751rr1xQsjzkl0EV41UjIGoF\n\
pPH8E14NwjLr9vPA68ZkfQmBI8H08yTRsqUJOLYLP/mLwIY=\n\
-----END CERTIFICATE-----";

static const char *client_crt = "Certificate:\n\
    Data:\n\
        Version: 3 (0x2)\n\
        Serial Number: 36 (0x24)\n\
    Signature Algorithm: sha256WithRSAEncryption\n\
        Issuer: C=CN, ST=tianjin, L=TJ, O=RDA, OU=RDA, CN=RDA/emailAddress=ca@rdamicro.com\n\
        Validity\n\
            Not Before: Sep 19 00:00:00 2018 GMT\n\
            Not After : Sep 19 00:00:00 2027 GMT\n\
        Subject: C=CN, ST=tianjin, O=RDA, OU=RDA, CN=RDA/emailAddress=client@rdamicro.com\n\
        Subject Public Key Info:\n\
            Public Key Algorithm: rsaEncryption\n\
                Public-Key: (1024 bit)\n\
                Modulus:\n\
                    00:d6:ba:00:f0:aa:5b:22:ff:42:0f:9c:86:53:be:\n\
                    4f:5c:86:c4:53:15:f3:f0:22:36:a2:1c:0f:97:4b:\n\
                    5c:35:28:cc:0b:e7:08:c8:87:7a:3d:f9:a5:5a:12:\n\
                    44:7f:c4:8b:11:ce:94:fb:78:a7:8c:48:d7:75:0e:\n\
                    8a:98:53:cf:87:1f:9b:d9:b5:f4:1e:22:e7:64:43:\n\
                    da:04:86:2d:62:25:93:75:bf:cc:2a:89:07:db:69:\n\
                    1c:6b:7f:e4:e1:42:44:63:48:c9:ac:ab:f5:f2:8c:\n\
                    f1:c4:03:3c:b0:2e:c1:61:a7:44:12:76:98:3e:d9:\n\
                    3d:97:9c:ee:46:bf:8a:99:4f\n\
                Exponent: 65537 (0x10001)\n\
        X509v3 extensions:\n\
            X509v3 Basic Constraints:\n\
                CA:FALSE\n\
            Netscape Comment:\n\
                OpenSSL Generated Certificate\n\
            X509v3 Subject Key Identifier:\n\
                A4:DF:A9:EC:82:49:AF:00:1C:98:E6:57:21:AF:CE:D5:24:35:0F:26\n\
            X509v3 Authority Key Identifier:\n\
                keyid:6D:D4:50:65:80:FF:AB:F7:61:04:D9:07:E0:83:7B:2D:09:27:89:D7\n\
    \n\
    Signature Algorithm: sha256WithRSAEncryption\n\
         7e:ea:bb:db:93:f2:5e:5d:0d:d8:7e:16:ed:78:32:11:56:61:\n\
         65:ab:f2:b8:63:bb:19:25:a8:64:27:33:49:a3:90:5c:59:75:\n\
         e5:0e:1e:ae:65:57:40:ac:5b:97:98:8a:5e:2f:0a:a8:81:63:\n\
         20:0b:eb:a1:79:20:f0:aa:09:20:c4:0c:b6:0e:a3:84:ec:29:\n\
         3b:1d:8e:97:26:53:e5:a3:b8:83:3e:0c:11:34:ed:13:13:3e:\n\
         24:a1:25:54:bd:3e:5d:3a:ff:85:7e:62:da:a8:14:5b:d5:16:\n\
         f7:60:aa:49:9f:77:97:8d:d4:54:59:74:50:c4:0b:bc:ce:9f:\n\
         ec:69\n\
-----BEGIN CERTIFICATE-----\n\
MIIC1DCCAj2gAwIBAgIBJDANBgkqhkiG9w0BAQsFADB2MQswCQYDVQQGEwJDTjEQ\n\
MA4GA1UECAwHdGlhbmppbjELMAkGA1UEBwwCVEoxDDAKBgNVBAoMA1JEQTEMMAoG\n\
A1UECwwDUkRBMQwwCgYDVQQDDANSREExHjAcBgkqhkiG9w0BCQEWD2NhQHJkYW1p\n\
Y3JvLmNvbTAeFw0xODA5MTkwMDAwMDBaFw0yNzA5MTkwMDAwMDBaMG0xCzAJBgNV\n\
BAYTAkNOMRAwDgYDVQQIDAd0aWFuamluMQwwCgYDVQQKDANSREExDDAKBgNVBAsM\n\
A1JEQTEMMAoGA1UEAwwDUkRBMSIwIAYJKoZIhvcNAQkBFhNjbGllbnRAcmRhbWlj\n\
cm8uY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDWugDwqlsi/0IPnIZT\n\
vk9chsRTFfPwIjaiHA+XS1w1KMwL5wjIh3o9+aVaEkR/xIsRzpT7eKeMSNd1DoqY\n\
U8+HH5vZtfQeIudkQ9oEhi1iJZN1v8wqiQfbaRxrf+ThQkRjSMmsq/XyjPHEAzyw\n\
LsFhp0QSdpg+2T2XnO5Gv4qZTwIDAQABo3sweTAJBgNVHRMEAjAAMCwGCWCGSAGG\n\
+EIBDQQfFh1PcGVuU1NMIEdlbmVyYXRlZCBDZXJ0aWZpY2F0ZTAdBgNVHQ4EFgQU\n\
pN+p7IJJrwAcmOZXIa/O1SQ1DyYwHwYDVR0jBBgwFoAUbdRQZYD/q/dhBNkH4IN7\n\
LQknidcwDQYJKoZIhvcNAQELBQADgYEAfuq725PyXl0N2H4W7XgyEVZhZavyuGO7\n\
GSWoZCczSaOQXFl15Q4ermVXQKxbl5iKXi8KqIFjIAvroXkg8KoJIMQMtg6jhOwp\n\
Ox2OlyZT5aO4gz4METTtExM+JKElVL0+XTr/hX5i2qgUW9UW92CqSZ93l43UVFl0\n\
UMQLvM6f7Gk=\n\
-----END CERTIFICATE-----";

static const char *client_key = "-----BEGIN RSA PRIVATE KEY-----\n\
MIICXgIBAAKBgQDWugDwqlsi/0IPnIZTvk9chsRTFfPwIjaiHA+XS1w1KMwL5wjI\n\
h3o9+aVaEkR/xIsRzpT7eKeMSNd1DoqYU8+HH5vZtfQeIudkQ9oEhi1iJZN1v8wq\n\
iQfbaRxrf+ThQkRjSMmsq/XyjPHEAzywLsFhp0QSdpg+2T2XnO5Gv4qZTwIDAQAB\n\
AoGBAIFmGgbuQnm2pdLOmsyAlUbHGCyRwC1oENBkZKjiCzEl4sERe2OM8QfEF/dN\n\
puXwRXZ3raRVs3KAuwaZur/NUfOa3kwGljnb859z5HcsWxb85ts+4TFP8AT2izjj\n\
XGYHBZZhVhD7Fr4rXOaKbzBprjXTgSgtJ6JZaAtgrWhD7kUxAkEA+bNwBKaTXgBK\n\
egN7NRfrL2bTFWKxY+Jse4NTflDaFDtUVrEqjpZlUCyGO5gPLkY4Zb7llgu28+E+\n\
FZQMdh5VRQJBANwks93pDiUmmiyL4f+DP4Bvtc1fgK7NYEMey42/46LlNC9s7YBA\n\
XXozLTdXtUJLliRy1VRK763OtocZ3kkfC4MCQHdNFCgkriQrX2oMX8FuPB/ZsOB2\n\
1uoyNEKO7EVdu9QOxKzm2L5nfOBhZYDzlc02H5v9KRQXZMIAy/jjU1DcDUECQQCQ\n\
0Bf95zwl1iHfIdl1wnm4XEPkqGk3E+S54n0Wbt8oncvZUrddAXo8U4Pv/uM+jAhl\n\
S6DnhS/rTqqlbq/Zu/FPAkEAjl3zMwClAVUEcqwDzLV6Pw4vaOxDtO+cx+XL24K8\n\
zIN9d8BuUWotkY5D/cnuDenQPMERtFJwRjEtiJKqqE3evg==\n\
-----END RSA PRIVATE KEY-----";

static int MqttConnect(MQTTAsync client)
{
    MQTTAsync_connectOptions ConnOpts = MQTTAsync_connectOptions_initializer;
    MQTTAsync_SSLOptions ssl_opts = MQTTAsync_SSLOptions_initializer;
    int rc = 0;

    connected = 0;
    ConnOpts.onSuccess = onConnect;
    ConnOpts.onFailure = onConnectFailure;
    ConnOpts.cleansession = 1;

    ConnOpts.keepAliveInterval = opts.keepalive;
    ConnOpts.username = opts.username;
    ConnOpts.password = opts.password;
    ConnOpts.MQTTVersion = opts.MQTTVersion;
    ConnOpts.context = client;
    ConnOpts.automaticReconnect = 1;
    ConnOpts.connectTimeout = 30;

    if (opts.connection && (strncmp(opts.connection, "ssl://", 6) == 0 ||
                            strncmp(opts.connection, "wss://", 6) == 0))
    {
        ssl_opts.verify = 0;
        ssl_opts.CApath = opts.capath;
        ssl_opts.keyStore = client_crt;
        ssl_opts.trustStore = CA_CRT;
        ssl_opts.privateKey = client_key;
        ssl_opts.privateKeyPassword = opts.keypass;
        ssl_opts.enabledCipherSuites = opts.ciphers;
        ssl_opts.ssl_error_cb = onSSLError;
        ssl_opts.ssl_error_context = client;
        ssl_opts.enableServerCertAuth = 1;
        ssl_opts.sslVersion = MQTT_SSL_VERSION_TLS_1_0;
        ConnOpts.ssl = &ssl_opts;
    }

    rc = MQTTAsync_connect(client, &ConnOpts);

    return rc;
}

static int MqttPublish(MQTTAsync client, int datalen, char *data)
{
    MQTTAsync_responseOptions PubRespOpts = MQTTAsync_responseOptions_initializer;
    int rc;

    published = 0;
    PubRespOpts.onSuccess = onPublish;
    PubRespOpts.onFailure = onPublishFailure;
    PubRespOpts.context = client;
    rc = MQTTAsync_send(client, opts.topic, datalen, data, opts.qos, opts.retained, &PubRespOpts);

    return rc;
}

static int MqttSubscribe(MQTTAsync client)
{
    MQTTAsync_responseOptions SubRespOpts = MQTTAsync_responseOptions_initializer;
    int rc;

    subscribed = 0;
    SubRespOpts.onSuccess = onSubscribe;
    SubRespOpts.onFailure = onSubscribeFailure;
    SubRespOpts.context = client;
    rc = MQTTAsync_subscribe(client, opts.topic, opts.qos, &SubRespOpts);

    return rc;
}

static int MqttDisconnect(MQTTAsync client)
{
    MQTTAsync_disconnectOptions DisconncOpts = MQTTAsync_disconnectOptions_initializer;
    int rc;

    disconnected = 0;
    DisconncOpts.onSuccess = onDisconnect;
    DisconncOpts.onFailure = onDisconnectFailure;
    DisconncOpts.context = client;
    rc = MQTTAsync_disconnect(client, &DisconncOpts);

    return rc;
}

static int test1()
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};
    char *pcUrl = NULL;

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test connection of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    opts.MQTTVersion = MQTTVERSION_3_1_1;
    opts.host = "211.95.108.52";
    opts.port = "1885";
    opts.username = "admin";
    opts.password = "123456";
    opts.clientid = "UnisocClient";
    opts.keepalive = 60;
    opts.retained = 0;
    opts.connection = "ssl://211.95.108.52:1885";

    pcUrl = malloc(128);
    if (NULL == pcUrl)
    {
        cOutstr[0] = '\0';
        sprintf(cOutstr, "%s%s(%d):malloc falied.", LOG_HEAD, __func__, __LINE__);
        OSI_LOGI(0, cOutstr);
        rc = -1;
        goto finish;
    }
    else
    {
        sprintf(pcUrl, "ssl://%s:%s", opts.host, opts.port);
    }

    MqttInit();
    rc = MQTTAsync_create(&pstClient, pcUrl, opts.clientid, 1, NULL);
    if (rc != MQTTASYNC_SUCCESS)
    {
        if (NULL != pcUrl)
        {
            free(pcUrl);
            pcUrl = NULL;
        }
        rc = -1;
        goto finish;
    }

    rc = MQTTAsync_setCallbacks(pstClient, pstClient, NULL, messageArrived, NULL);
    if (rc != MQTTASYNC_SUCCESS)
    {
        if (NULL != pcUrl)
        {
            free(pcUrl);
            pcUrl = NULL;
        }

        MQTTAsync_destroy(&pstClient);
        MQTTAsync_uninit();
        rc = -1;
        goto finish;
    }

    MqttConnect(pstClient);
    while (0 == connected)
    {
        osiThreadSleep(100);
    }

    if (1 == connected)
    {
        rc = 0;
    }
    else
    {
        MQTTAsync_destroy(&pstClient);
        MQTTAsync_uninit();
        rc = -1;
    }

finish:
    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test connection finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);

    if (NULL != pcUrl)
    {
        free(pcUrl);
        pcUrl = NULL;
    }

    return rc;
}

static int test2()
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test subscribe of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    opts.topic = "/unisoc/topic/test";
    opts.qos = 0;

    MqttSubscribe(pstClient);
    while (0 == subscribed)
    {
        osiThreadSleep(100);
    }
    if (1 == subscribed)
    {
        rc = 0;
    }
    else
    {
        rc = -1;
    }

    //finish:
    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test subscribe finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);
    return rc;
}

static int test3()
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test publish of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    opts.topic = "/unisoc/topic/test";
    opts.message = "message.test.tls";
    opts.qos = 0;

    MqttPublish(pstClient, strlen(opts.message), opts.message);
    while (0 == published)
    {
        osiThreadSleep(100);
    }
    if (1 == published)
    {
        rc = 0;
    }
    else
    {
        rc = -1;
    }

    //finish:
    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test publish finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);
    return rc;
}

static int test4()
{
    int rc = 0; //0:success -1:fail
    char cOutstr[128] = {0};

    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test disconnection of paho mqtt lib.", LOG_HEAD, __func__, __LINE__);
    OSI_LOGI(0, cOutstr);

    MqttDisconnect(pstClient);
    while (0 == disconnected)
    {
        osiThreadSleep(100);
    }
    if (1 == disconnected)
    {
        rc = 0;
    }
    else
    {
        rc = -1;
    }

    MQTTAsync_destroy(&pstClient);
    MQTTAsync_uninit();

    //finish:
    cOutstr[0] = '\0';
    sprintf(cOutstr, "%s%s(%d):Test disconnection finish of paho mqtt lib. rc=%d", LOG_HEAD, __func__, __LINE__, rc);
    OSI_LOGI(0, cOutstr);
    return rc;
}

int paho_mqtts_lib_test(void)
{
    int rc = 0;

    rc = test1();
    if (0 == rc)
    {
        rc += test2();
        rc += test3();
        osiThreadSleep(1000);
        rc += test4();
    }

    return rc;
}

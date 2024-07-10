#include "coap_async_api.h"
//#include "async_worker.h"
//#include "lwipopts.h"
//#include "osi_log.h"
#include "os_api.h"


uint32 coap_netid = 0;


COAP_DATA_T g_coap_data;
coap_client_t * g_coap_client;

static bool g_bCoapDoing = false;
//static SCI_MUTEX_PTR g_coapMutex = NULL;

enum COAPAsync_threadStates
{
	STOPPED, STARTING, RUNNING, STOPPING
};

enum COAPAsync_threadStates coapThread_state = STOPPED;

static BLOCK_ID COAP_Thread_id = 0;


PUBLIC void coapProcessReturnData(coap_client_t *client, const unsigned char *data, size_t len, int num)
{
    if (NULL != g_coap_data.gBuff[num])
        return;
    g_coap_data.gLength = g_coap_data.gLength + len;
    g_coap_data.gBuff[num] = malloc(len + 1);
    memset(g_coap_data.gBuff[num], 0, len + 1);
    memcpy(g_coap_data.gBuff[num], data, len);
    g_coap_data.gNum = num;
    g_coap_data.gLen[num] = len;
    SCI_TRACE_LOW(" COAP response   num : %d  ,len :  %d    [ %s   %d ]  \n",num,g_coap_data.gLen[num],__FUNCTION__,__LINE__);
    SCI_TRACE_LOW(" COAP response    payload :  %s    [ %s   %d ]  \n",g_coap_data.gBuff[num],__FUNCTION__,__LINE__);
    return;
}

static void COAPAsync_lock_mutex(SCI_MUTEX_PTR amutex)
{
	Thread_lock_mutex(amutex);
}


static void COAPAsync_unlock_mutex(SCI_MUTEX_PTR amutex)
{
	Thread_unlock_mutex(amutex);
}
coap_client_t *coap_client_new(void )
{
    coap_client_t *client = malloc(sizeof(coap_client_t));

    if (client != NULL)
    {
        memset(client, 0, sizeof(coap_client_t));
        client->status = COAP_STATUS_INIT;
        //client->user_task = user_task;
        //client->user_cb = user_cb;
        //client->user_data = user_data;
    }

    //g_coapMutex =Thread_create_mutex("g_coapMutex");
    return client;
}

coap_client_t *coap_getCoapClient(void)
{
    if (NULL == g_coap_client)
    {
        g_coap_client = coap_client_new();
        return g_coap_client;
    }
    else if ((NULL != g_coap_client) )
    {
        SCI_TRACE_LOW(" COAP at_coap_client  != NULL .  wait to end   !\n ");        
        return NULL;
    }    
}

bool coap_client_free(coap_client_t *client)
{
    if ((NULL != client) && (client->status != COAP_STATUS_READY))
    {
        free(client->cmdline);
        free(client->payload);
        client->cmdline = NULL;
        client->payload = NULL;
        client->status = COAP_STATUS_NONE;
        free(client);
        //Thread_destroy_mutex(g_coapMutex);
        return true;
    }
    
    return false;
}

 void coap_data_free()
{
    int i ;    
    for (i = 0; i <= g_coap_data.gNum; i++)
    {
        free(g_coap_data.gBuff[i]);
        g_coap_data.gBuff[i] = NULL;
        g_coap_data.gLen[i] = 0;
    }
    g_coap_data.gLength = 0;
    g_coap_data.gNum = 0;

    return;
}



coap_status_t coap_client_getStatus(coap_client_t *client)
{
    if (NULL != client)
    {
        return client->status;
    }

    return COAP_STATUS_NONE;
}
#if 0
void *coap_client_getUserData(coap_client_t *client)
{
    if (NULL != client)
    {
        return client->user_data;
    }

    return NULL;
}
bool coap_client_cleanParams(coap_client_t *client)
{
    if ((NULL != client) && (client->status != COAP_STATUS_READY))
    {
        free(client->cmdline);
        free(client->payload);
        client->cmdline = NULL;
        client->payload = NULL;
        client->status = COAP_STATUS_INIT;
        return true;
    }
    return false;
}
#endif

bool coap_client_setParams(coap_client_t *client, uint8_t *cmdline_param)
{
    if ((NULL != client) && (client->status == COAP_STATUS_INIT))
    {
        uint8_t *cmdline = NULL;
        unsigned int cmdline_length;
        if (cmdline_param == NULL)
            return true;
        cmdline_length = strlen((const char *)cmdline_param);
        if (client->cmdline != NULL)
        {
            cmdline_length += strlen((const char *)client->cmdline);
        }
        cmdline = malloc(cmdline_length + 2);
        memset(cmdline, 0, cmdline_length + 2);
        if (client->cmdline != NULL)
            sprintf((char *)cmdline, "%s %s", client->cmdline, cmdline_param);
        else
            strcpy((char *)cmdline, (const char *)cmdline_param);
        if (client->cmdline)
            free(client->cmdline);
        client->cmdline = cmdline;
        //OSI_LOGXI(OSI_LOGPAR_SISS, 0x100075c6, "func:%s line:%d setParams param:%s,cmdline:%s", __func__, __LINE__, cmdline_param, client->cmdline);
        return true;
    }
    return false;
}

bool coap_client_setIndataCbk(coap_client_t *client, coap_incoming_data_cb_t data_cb)
{
    if ((NULL != client) && (client->status == COAP_STATUS_INIT))
    {
        client->data_cb = data_cb;
        return true;
    }
    return false;
}

bool coap_client_setPayload(coap_client_t *client, uint8_t *data, uint32_t data_len)
{
    if ((NULL != client) && (client->status == COAP_STATUS_INIT || client->status == COAP_STATUS_READY))
    {
        
        if (NULL != client->payload)
        {
            free(client->payload);
        }
        client->payload = malloc(data_len + 1);
        client->payload_len = data_len;
        memcpy(client->payload, data, data_len);
        client->payload[data_len] = 0;
        //OSI_LOGXI(OSI_LOGPAR_S, 0x100075c7, "coap_client_setPayload setPayload=%s", client->payload);

        return true;
    }
    return false;
}

char *coap_client_getPayload(coap_client_t *client)
{
    if ((NULL != client) && (NULL != client->payload))
    {
        return (char *)client->payload;
    }

    return NULL;
}

bool COAP_Thread_entry(void)
{
    
    bool rc = true;
    int nResult = -1;
    int  i = 0;
    //OSI_LOGXI(OSI_LOGPAR_SII, 0, "func:%s line:%d event:%d", __func__, __LINE__, (int)req->event);
    //coap_client_t *client = (coap_client_t *)req->param;
    char **argv = NULL;
    uint8_t argc = 0;
    bool has_payload = false;
    char delim[] = " ";
    char *token;
    static char *lasts;
    coap_client_t *client;
    const uint8_t *cmdline;

    client = g_coap_client;
    
    cmdline = client->cmdline;
    coapThread_state = RUNNING;
    argv = malloc(CMDLINE_NUM * 4);
    if(argv == NULL)
    {
        SCI_TRACE_LOW(" COAP  malloc failed !     [  %s  %d]\n",__FUNCTION__,__LINE__);
        rc = false;
        goto error;
    }
    
    memset(argv, 0, CMDLINE_NUM * 4);
    argv[argc++] = "coap_client";

    for (token = strtok_r((char *)cmdline, delim, &lasts); token != NULL; token = strtok_r(NULL, delim, &lasts))
    {
        argv[argc] = malloc(strlen(token) + 1);
        strcpy(argv[argc], token);
        argc++;

        if (argc >= CMDLINE_NUM)
        {
            rc = false;
            goto error;
        }
    }
    #if 0
    has_payload = aworker_param_getu32(req, 0, &paramok);
    if (has_payload && (client->payload == NULL))
    {
        rc = false;
        goto error;
    }
    #endif
    
    if (client->payload)
    {
        coap_client_set_payload(NULL, client->payload_len, 1);
        coap_client_set_payload(client->payload, client->payload_len, 1);
    }
    
    
    if (client->data_cb)
        coap_client_set_datahander(client->data_cb, client);

    client->status = COAP_STATUS_PROCEING;
    nResult = coap_client_main(argc, argv);
error:
    for ( i = 1; i < argc; i++)
    {
        if (NULL != argv[i])
        {
            free(argv[i]);
            argv[i] = NULL;
        }
    }
    if (NULL != argv)
    {
        free(argv);
        argv = NULL;
    }

    if (nResult != -1)
    {
        rc = true;
    }
    SCI_TRACE_LOW(" COAP  THREAD EXIT !     [  %s  %d]\n",__FUNCTION__,__LINE__);
    coapThread_state = STOPPED;
    coap_data_free();
    coap_client_free(client);
    g_coap_client = NULL;
    Thread_stop();
    return rc;
}



bool coap_async_get(coap_client_t *client, uint8_t *url)
{
    if ((NULL != client) && (client->status == COAP_STATUS_INIT))
    {
        bool rc = true;
        uint8_t getCmd[] = "-m get";
        //coap_client_setParams(client, cmdline);
        coap_client_setParams(client, getCmd);
        coap_client_setParams(client, url);
        client->status = COAP_STATUS_READY;

        if(coapThread_state !=STOPPED )
        {
            SCI_TRACE_LOW(" COAP  THREAD is runing !    coapThread_state :%d    [  %s  %d]\n",coapThread_state,__FUNCTION__,__LINE__);
            return false;
        }
        //COAPAsync_lock_mutex(g_coapMutex);        
        coapThread_state = STARTING;
        //COAPAsync_unlock_mutex(g_coapMutex);      
        
        
        COAP_Thread_id =Thread_start(COAP_Thread_entry, NULL,1);

        
         
        return rc;
    }
    return false;
}

bool coap_async_put(coap_client_t *client, uint8_t *url, uint8_t *payload)
{
    if ((NULL != client) && (client->status == COAP_STATUS_INIT))
    {
        bool rc = true;
        uint8_t getCmd[] = "-m put";
        uint32_t data_len = 0;

        
        //coap_client_setParams(client, cmdline);
        coap_client_setParams(client, getCmd);
        coap_client_setParams(client, url);
        client->status = COAP_STATUS_READY;

        if(coapThread_state !=STOPPED )
        {
            SCI_TRACE_LOW(" COAP  THREAD IS runing !    coapThread_state :%d    [  %s  %d]\n",coapThread_state,__FUNCTION__,__LINE__);
            return false;
        }
        //COAPAsync_lock_mutex(g_coapMutex);        
        coapThread_state = STARTING;
        //COAPAsync_unlock_mutex(g_coapMutex); 

        //set payload  
        data_len = strlen(payload);
        coap_client_setPayload(client,payload,data_len);
        //
        
        
        COAP_Thread_id =Thread_start(COAP_Thread_entry, NULL,1);

        
         
        return rc;
    }
    return false;
}

bool coap_async_post(coap_client_t *client, uint8_t *url, uint8_t *payload)
{
    if ((NULL != client) && (client->status == COAP_STATUS_INIT))
    {
        bool rc = true;
        uint8_t getCmd[] = "-m post";
        uint32_t data_len = 0;

        
        //coap_client_setParams(client, cmdline);
        coap_client_setParams(client, getCmd);
        coap_client_setParams(client, url);
        client->status = COAP_STATUS_READY;

        if(coapThread_state !=STOPPED )
        {
            SCI_TRACE_LOW(" COAP  THREAD IS runing !    coapThread_state :%d    [  %s  %d]\n",coapThread_state,__FUNCTION__,__LINE__);
            return false;
        }
        //COAPAsync_lock_mutex(g_coapMutex);        
        coapThread_state = STARTING;
        //COAPAsync_unlock_mutex(g_coapMutex); 

        //set payload  
        data_len = strlen(payload);
        coap_client_setPayload(client,payload,data_len);
        //
        
        
        COAP_Thread_id =Thread_start(COAP_Thread_entry, NULL,1);

        
         
        return rc;
    }
    return false;
}


bool coap_async_delete(coap_client_t *client, uint8_t *url)
{
    if ((NULL != client) && (client->status == COAP_STATUS_INIT))
    {
        bool rc = true;
        uint8_t getCmd[] = "-m delete";
        //coap_client_setParams(client, cmdline);
        coap_client_setParams(client, getCmd);
        coap_client_setParams(client, url);
        client->status = COAP_STATUS_READY;

        if(coapThread_state !=STOPPED )
        {
            SCI_TRACE_LOW(" COAP  THREAD IS runing !    coapThread_state :%d    [  %s  %d]\n",coapThread_state,__FUNCTION__,__LINE__);
            return false;
        }
        //COAPAsync_lock_mutex(g_coapMutex);        
        coapThread_state = STARTING;
        //COAPAsync_unlock_mutex(g_coapMutex);      
        
        
        COAP_Thread_id =Thread_start(COAP_Thread_entry, NULL,1);

        
         
        return rc;
    }
    return false;
}


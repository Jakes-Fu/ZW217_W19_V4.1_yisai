#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include "lwm2m_api.h"
#include "lwm2mclient.h"
#include "liblwm2m.h"
#include "commandline.h"
//#include "expat.h"    //modify by xiameng.yang
#include "internals.h"
//#include "osi_log.h"  //modify by xiameng.yang



static void lwm2m_clientd(uint32 argc, void *argv);

static bool isRefValid(uint8_t ref)
{
    return ref < MAX_REF_NUM && lwm2m_configs[ref] != NULL;
}

static bool isRefRegistered(uint8_t ref)
{
    return isRefValid(ref) && lwm2m_configs[ref]->isregisted;
}

#define EVID_CASE(nEvId) \
    case nEvId:          \
        return ((uint8_t *)#nEvId)

uint8_t *lwm2m_eventName(uint8_t nEvId)
{
    switch (nEvId)
    {
        EVID_CASE(LWM2M_EVENT_BOOTSTRAP_START);
        EVID_CASE(LWM2M_EVENT_BOOTSTRAP_SUCCESS);
        EVID_CASE(LWM2M_EVENT_BOOTSTRAP_FAILED);
        EVID_CASE(LWM2M_EVENT_CONNECT_SUCCESS);
        EVID_CASE(LWM2M_EVENT_CONNECT_FAILED);
        EVID_CASE(LWM2M_EVENT_REG_SUCCESS);
        EVID_CASE(LWM2M_EVENT_REG_FAILED);
        EVID_CASE(LWM2M_EVENT_REG_TIMEOUT);
        EVID_CASE(LWM2M_EVENT_LIFETIME_TIMEOUT);
        EVID_CASE(LWM2M_EVENT_STATUS_HALT);
        EVID_CASE(LWM2M_EVENT_UPDATE_SUCCESS);
        EVID_CASE(LWM2M_EVENT_UPDATE_FAILED);
        EVID_CASE(LWM2M_EVENT_UPDATE_TIMEOUT);
        EVID_CASE(LWM2M_EVENT_RESPONSE_FAILED);
        EVID_CASE(LWM2M_EVENT_NOTIFY_FAILED);
#ifdef CONFIG_LWM2M_CTWING_SUPPORT
        EVID_CASE(LWM2M_EVENT_NOTIFY_SEND_SUCCESS);
        EVID_CASE(LWM2M_EVENT_NOTIFY_SENT_SUCCESS);
        EVID_CASE(LWM2M_EVENT_NOTIFY_SENT_TIMEOUT);
        EVID_CASE(LWM2M_EVENT_UPDATE_BINDING_SEND);
        EVID_CASE(LWM2M_EVENT_UPDATE_BINDING_SUCCESS);
        EVID_CASE(LWM2M_EVENT_UPDATE_BINDING_FAILED);
        EVID_CASE(LWM2M_EVENT_UPDATE_BINDING_TIMEOUT);
        EVID_CASE(LWM2M_EVENT_DEREG_SUCCESS);
        EVID_CASE(LWM2M_EVENT_DEREG_FAILED);
        EVID_CASE(LWM2M_EVENT_DEREG_TIMEOUT);
#endif
    default:
        LOG_ARG("Unknown Event name:%d", nEvId);
        return (uint8_t *)"Unknown Event name";
    }
}

int lwm2m_is_pending_regcmd(uint8_t ref)
{
    if (!isRefValid(ref))
        return LWM2M_RET_ERROR;
    return lwm2m_configs[ref]->pending_regcmd;
}

BLOCK_ID g_liblwm2m_MESSAGE_TASK = SCI_INVALID_BLOCK_ID;

int liblwm2m_MessageHandleTask(osiCallback_t callback)
{
    if (g_liblwm2m_MESSAGE_TASK != SCI_INVALID_BLOCK_ID)
    {
        SCI_TRACE_LOW(" [ LWM2MDEBUG]   error  [%s %d]\n",__FUNCTION__,__LINE__);
        return -1;
    }
    
    //g_mqtt_current_link_id = link_id;
    SCI_CreateThread("t_liblwm2m_MESSAGE",
                                  "q_liblwm2m_MESSAGE",
                                   callback,
                                     1, NULL,
                                     LWM2MTASK_STACK_SIZE,
                                     LWM2MTASK_EVENT_QUEUE_SIZE,
                                     LWM2MTASK_PRIORITY,            
                                     SCI_PREEMPT, 
                                     SCI_AUTO_START);     
 
     
    return 0;
}

void lwm2mPostEvent(uint8_t ref, uint32_t nEventId, uint32_t nResult, uint32_t nParam1, uint32_t nParam2)
{
    lwm2mEventSIG_t *ev = malloc(sizeof(lwm2mEventSIG_t));
    lwm2mEvent_t *lwm2m_ev = malloc(sizeof(lwm2mEvent_t));
    int ret ;
    lwm2m_ev->param1 = nResult;
    lwm2m_ev->param2 = nParam1;
    lwm2m_ev->param3 = nParam2;

    ev->id = nEventId;
    ev->param1 = (uint32_t)lwm2m_configs[ref]->userparam;
    ev->param2 = ref;
    ev->param3 = (uint32_t)lwm2m_ev;

     if (g_liblwm2m_MESSAGE_TASK != SCI_INVALID_BLOCK_ID)
    {
        SCI_TRACE_LOW(" [ LWM2MDEBUG]  [nEventId id 0x%x]   [%s , %d]\n",nEventId,__FUNCTION__,__LINE__);        
        if (ev)
        {            
            SCI_SendSignal((xSignalHeader)(ev), g_liblwm2m_MESSAGE_TASK);
        }
    }
        
    //osiThreadCallback((osiThread_t *)lwm2m_configs[ref]->event_handle, (osiCallback_t)lwm2m_configs[ref]->callback, ev);

    
    ret = -1;
    if (nEventId == EVETN_IND && lwm2m_configs[ref]->pending_regcmd)
    {
        if (nResult == LWM2M_EVENT_BOOTSTRAP_FAILED || nResult == LWM2M_EVENT_REG_FAILED || nResult == LWM2M_EVENT_STATUS_HALT || nResult == LWM2M_EVENT_REG_TIMEOUT)
            ret = 0;
        else if (nResult == LWM2M_EVENT_REG_SUCCESS)
            ret = 1;
    }
    if (ret != -1)
    {
        lwm2m_configs[ref]->pending_regcmd = 0;
        lwm2mPostEvent(ref, SERVER_REGCMD_RESULT_IND, ret, 0, 0);
    }
}

typedef struct
{
    uint8_t item2;
    uint8_t ishostkey;
    uint8_t isuserdatakey;
    uint8_t host[50];
    uint8_t cmdline[100];
} att_value_t;

static void elementStart(void *data, const char *el, const char **attr)
{
    att_value_t *value = (att_value_t *)data;
    OSI_LOGI(0x10007bc2, "el =%s,%s,%s", el, attr[0], attr[1]);
    if (el != NULL && attr[0] != NULL && attr[1] != NULL)
    {
        if (strcmp(el, "item") == 0 && strcmp(attr[0], "id") == 0 && strcmp(attr[1], "2") == 0)
            value->item2 = 2;
        else if (value->item2 && strcmp(el, "data") == 0 && strcmp(attr[0], "name") == 0)
        {
            if (strcmp(attr[1], "Host") == 0)
                value->ishostkey = 1;
            else if (strcmp(attr[1], "Userdata") == 0)
                value->isuserdatakey = 1;
        }
    }
}
#if 0
static void attValues(void *data, const XML_Char *s, int len)
{
    att_value_t *value = (att_value_t *)data;
    if (value->ishostkey && len < 50)
    {
        memcpy(value->host, s, len);
        value->ishostkey = 0;
        value->item2--;
        OSI_LOGI(0x10007bc3, "host =%s", value->host);
    }
    else if (value->isuserdatakey && len < 100)
    {
        memcpy(value->cmdline, s, len);
        value->isuserdatakey = 0;
        value->item2--;
        OSI_LOGI(0x10007bc4, "cmdline =%s", value->cmdline);
    }
}

int8_t lwm2m_new_config_xml(void *file_config, uint16_t size)
{
    if (netif_default == NULL)
        return -1;
    XML_Parser parser = XML_ParserCreate(NULL);
    if (parser == NULL)
    {
        return -1;
    }
    char *port = NULL;
    uint8_t cmdline[256] = {0};
    att_value_t value = {0};
    XML_SetUserData(parser, &value);
    /* Tell expat to use functions start() and end() each times it encounters
     * the start or end of an element. */
    XML_SetStartElementHandler(parser, elementStart);
    XML_SetCharacterDataHandler(parser, attValues);
    if (XML_Parse(parser, file_config, size, XML_TRUE) == XML_STATUS_ERROR)
    {
        return -1;
    }
    XML_ParserFree(parser);
    if (strlen((char *)value.host) != 0)
    {
        port = strstr((char *)value.host, ":");
        if (port != NULL)
        {
            value.host[port - (char *)value.host] = 0;
            port++;
        }
        OSI_LOGI(0x10007bc5, "host =%s,port=%s", value.host, port);
    }
    if (strlen((char *)value.cmdline) != 0)
    {
        OSI_LOGI(0x10007bc4, "cmdline =%s", value.cmdline);
    }
    if (port)
        snprintf((char *)cmdline, 255, "%s -h %s -p %s", value.cmdline, value.host, port);
    else
        snprintf((char *)cmdline, 255, "%s -h %s", value.cmdline, value.host);
    return lwm2m_new_config(cmdline);
}
#endif
int8_t lwm2m_new_config(const uint8_t *cmdline)
{
    uint8_t ref = 0xff;
    int i ;
    lwm2m_config_t *lwm2m_config;
    for (i = 0; i < MAX_REF_NUM; i++)
    {
        lwm2m_config_t *lwm2m_config = lwm2m_configs[i];
        if (lwm2m_config != NULL)
        {
            if (strcmp((const char *)cmdline, (const char *)lwm2m_config->cmdline) == 0)
            {
                if (lwm2m_config->isregisted)
                    return -1;
                else
                    ref = i;
                break;
            }
        }
        else if (ref == 0xff)
            ref = i;
    }
    if (ref == 0xff)
        return -1;

    lwm2m_config = lwm2m_configs[ref];
    if (lwm2m_config == NULL)
    {
        uint8_t **argv = lwm2m_malloc(40 * 4);
        uint8_t argc = 1;
        char delim[] = " ";
        char *token;
        static char *lasts;

        lwm2m_config = lwm2m_malloc(sizeof(lwm2m_config_t));
        memset(lwm2m_config, 0, sizeof(lwm2m_config_t));
        strcpy((char *)lwm2m_config->cmdline, (const char *)cmdline);
        for (token = strtok_r((char *)cmdline, delim, &lasts); token != NULL; token = strtok_r(NULL, delim, &lasts))
        {
            argv[argc] = lwm2m_malloc(strlen(token) + 1);
            strcpy((char *)argv[argc], token);
            argc++;
        }
        lwm2m_config->observer = false;
        lwm2m_config->argv = argv;
        lwm2m_config->argc = argc;
        lwm2m_config->isregisted = 0;
        lwm2m_config->ipc_socket = -1;
        lwm2m_config->ipc_data_socket = -1;
    }
    else
    {
        if (lwm2m_config->isregisted)
            return -1;
    }
    lwm2m_configs[ref] = lwm2m_config;
    return ref;
}

lwm2m_ret_t lwm2m_get_observer(uint8_t ref)
{
    if (!isRefValid(ref))
        return LWM2M_RET_ERROR;

    return lwm2m_configs[ref]->observer == true ? LWM2M_RET_OK : LWM2M_RET_ERROR;
}

lwm2m_ret_t lwm2m_set_observer(uint8_t ref, bool observer)
{
    if (!isRefValid(ref))
        return LWM2M_RET_ERROR;

    lwm2m_configs[ref]->observer = observer;
    return LWM2M_RET_OK;
}

lwm2m_ret_t lwm2m_free_config(uint8_t ref)
{
    if (!isRefValid(ref) || isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    if (lwm2m_configs[ref])
    {
        obj_arry_t *objects = lwm2m_configs[ref]->objects;
        int i = 0;
        for (; i < MAX_INIT_OBJECTS; i++)
        {
            if (objects[i].objId != 0)
            {
                lwm2m_free(objects[i].bitmap);
            }
        }

        for (i = 1; i < lwm2m_configs[ref]->argc; i++)
        {
            // Null pointer protection
            if (lwm2m_configs[ref]->argv[i] != NULL)
            {
                lwm2m_free(lwm2m_configs[ref]->argv[i]);
                lwm2m_configs[ref]->argv[i] = NULL;
            }
        }
        lwm2m_free(lwm2m_configs[ref]->argv);
        memset(lwm2m_configs[ref]->cmdline, 0, 256);
        lwm2m_free(lwm2m_configs[ref]);
        lwm2m_configs[ref] = NULL;
    }
    return LWM2M_RET_OK;
}
#if 0
lwm2m_ret_t lwm2mcfg_set_handler(uint8_t ref, void *taskHandle, osiCallback_t callback, void *userparam)
{
    if (!isRefValid(ref) || isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    lwm2m_config_t *lwm2m_config = lwm2m_configs[ref];
    lwm2m_config->event_handle = (uint32_t)taskHandle;
    lwm2m_config->callback = callback;
    lwm2m_config->userparam = userparam;

    return LWM2M_RET_OK;
}

lwm2m_ret_t lwm2mcfg_set_handler_ext(uint8_t ref, void *taskHandle, osiCallback_t callback, void *userparam, uint8_t nDLCI)
{
    if (!isRefValid(ref) || isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    lwm2m_config_t *lwm2m_config = lwm2m_configs[ref];
    if (lwm2m_config == NULL)
    {
        return LWM2M_RET_ERROR;
    }

    lwm2m_config->event_handle = (uint32_t)taskHandle;
    lwm2m_config->callback = callback;
    lwm2m_config->userparam = userparam;
    lwm2m_config->nDLCI = nDLCI;

    return LWM2M_RET_OK;
}
#endif
lwm2m_config_t *lwm2m_get_config(uint8_t configIndex)
{
    return lwm2m_configs[configIndex];
}

uint8_t lwm2m_get_ipcSocket(uint8_t configIndex)
{
    return lwm2m_configs[configIndex]->ipc_socket;
}

uint8_t lwm2m_get_ipcDataSocket(uint8_t configIndex)
{
    return lwm2m_configs[configIndex]->ipc_data_socket;
}

void lwm2m_parse_buffer(char *buffer, void *value, uint8_t count, void *delim)
{
    char *token;
    int i = 0;
    static char *lasts;
    OSI_LOGXI(OSI_LOGPAR_SI, 0x10007bc6, "lwm2m_parse_buffer buffer:%s, count=%d", buffer, count);
    if (delim == NULL)
        delim = " ";
    for (token = strtok_r((char *)buffer, delim, &lasts); token != NULL && i < count; token = strtok_r(NULL, delim, &lasts))
    {
        ((int *)value)[i++] = (int)token;
    }
}

lwm2m_ret_t lwm2m_excute_cmd(uint8_t *data, uint32_t data_len, uint8_t ref)
{
    lwm2m_config_t *lwm2m_config;
    struct sockaddr_in to4 = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    lwm2m_config = lwm2m_configs[ref];
    if (NULL == lwm2m_config)
    {
        return LWM2M_RET_ERROR;
    }
    SCI_TRACE_LOW( "  LWM2MDEBUG  lwm2m_excute_cmd  data_len:%d   [%s  %d]\n",data_len,__FUNCTION__,__LINE__);
    //struct sockaddr_in to4 = {0};
    //to4.sin_len = sizeof(struct sockaddr_in);  //xiameng.yang
    to4.sin_family = AF_INET;
    to4.sin_port = 1234 + ref;
    //to4.sin_addr.s_addr = PP_HTONL(INADDR_LOOPBACK);
    to4.sin_addr.s_addr = inet_addr("127.0.0.1");
    SCI_TRACE_LOW( "  LWM2MDEBUG    IPC_SOCKET:  0x%x to4.sin_port :%d  [%s  %d]\n",lwm2m_config->ipc_socket,to4.sin_port,__FUNCTION__,__LINE__);
    sendto(lwm2m_config->ipc_socket, data, data_len, 0, (const struct sockaddr *)&to4, sizeof(to4));
    return LWM2M_RET_OK;
}

lwm2m_ret_t lwm2m_excute_data_cmd(uint8_t *data, uint32_t data_len, uint8_t ref)
{
    lwm2m_config_t *lwm2m_config;
    struct sockaddr_in to4 = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    lwm2m_config = lwm2m_configs[ref];
    if (NULL == lwm2m_config ) // || netif_default == NULL)  xiameng.yang
    {
        return LWM2M_RET_ERROR;
    }

    //struct sockaddr_in to4 = {0};
    //to4.sin_len = sizeof(struct sockaddr_in);  //xiameng.yang
    to4.sin_family = AF_INET;
    to4.sin_port = 2235 + ref;
    //to4.sin_addr.s_addr = PP_HTONL(INADDR_LOOPBACK);
    to4.sin_addr.s_addr = inet_addr("127.0.0.1");
    sendto(lwm2m_config->ipc_data_socket, data, data_len, 0, (const struct sockaddr *)&to4, sizeof(to4));
    return LWM2M_RET_OK;
}

lwm2m_ret_t lwm2m_is_registed(uint8_t ref)
{
    return isRefRegistered(ref);
}

obj_arry_t *lwm2m_get_init_objs(uint8_t ref)
{
    lwm2m_config_t *lwm2m_config;
    if (!isRefValid(ref))
        return NULL;
    lwm2m_config = lwm2m_configs[ref];
    return lwm2m_config->objects;
}

lwm2m_ret_t lwm2m_add_obj(uint16_t objId, uint32_t instCount, uint8_t *bitmap, uint16_t resCount, uint8_t ref)
{
    //if (!isRefValid(ref))
      //  return LWM2M_RET_ERROR;

    lwm2m_config_t *lwm2m_config = lwm2m_configs[ref];
    obj_arry_t *objects = lwm2m_config->objects;
    int newIndex = -1;
    int i = 0;
    
    if (!isRefValid(ref))
        return LWM2M_RET_ERROR;
    
    if ((objId >= LWM2M_SECURITY_OBJECT_ID &&
         objId <= LWM2M_CONN_STATS_OBJECT_ID)
#ifdef CONFIG_LWM2M_ONENET_DM_SUPPORT
        || objId == LWM2M_DM_OBJECT_ID
#endif
    )
        return LWM2M_RET_ERROR;

    for (; i < MAX_INIT_OBJECTS; i++)
    {
        if (objects[i].objId == objId)
        {
            return LWM2M_RET_ERROR;
        }
        else if (objects[i].objId == 0 && newIndex == -1)
        {
            newIndex = i;
        }
    }

    if (newIndex != -1 && newIndex < MAX_INIT_OBJECTS)
    {
        objects[newIndex].objId = objId;
        objects[newIndex].bitmap = (uint8_t *)(lwm2m_strdup((char *)bitmap));
        objects[newIndex].instCount = instCount;
        objects[newIndex].resCount = resCount;
    }
    else
    {
        return LWM2M_RET_ERROR;
    }

    if (isRefRegistered(ref))
    {
        uint8_t cmd[550] = {0};
        if (instCount > 500)
            return LWM2M_RET_ERROR;
        sprintf((char *)cmd, "add %d %d %s %d", objId, (unsigned int)instCount, bitmap, resCount);
        if (lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref) == LWM2M_RET_OK)
            return LWM2M_RET_PENDING;
        else
            return LWM2M_RET_ERROR;
    }
    return LWM2M_RET_OK;
}

lwm2m_ret_t lwm2m_del_obj(uint16_t objId, uint8_t ref)
{
    //if (!isRefValid(ref))
       // return LWM2M_RET_ERROR;

    lwm2m_config_t *lwm2m_config = lwm2m_configs[ref];
    obj_arry_t *objects = lwm2m_config->objects;
    int i = 0;
    bool isObjExist = false;
    ipso_obj_t *obj_type_table;
    int j = 0;
    
    if (!isRefValid(ref))
        return LWM2M_RET_ERROR;
    
    for (; i < MAX_INIT_OBJECTS; i++)
    {
        if (objects[i].objId == objId)
        {
            objects[i].objId = 0;
            objects[i].resCount = 0;
            lwm2m_free(objects[i].bitmap);
            objects[i].bitmap = NULL;
            objects[i].instCount = 0;
            isObjExist = true;

            obj_type_table = lwm2m_config->obj_type_table;
            
            for (; j < MAX_DYNAMIC_OBJECTS; j++)
            {
                if (obj_type_table[j].objId == objId)
                {
                    memset(&obj_type_table[j], 0, sizeof(ipso_obj_t));
                    break;
                }
            }
        }
    }

    if (!isObjExist)
    {
        return LWM2M_RET_ERROR;
    }

    if (isRefRegistered(ref))
    {
        uint8_t cmd[10] = {0};
        sprintf((char *)cmd, "rm %d", objId);
        if (lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref) == LWM2M_RET_OK)
            return LWM2M_RET_PENDING;
        else
            return LWM2M_RET_ERROR;
    }
    return LWM2M_RET_OK;
}

obj_arry_t *lwm2m_find_obj(uint16_t objId, uint8_t ref)
{
     lwm2m_config_t *lwm2m_config ;
    obj_arry_t *objects;

    int i;
    if (!isRefValid(ref))
        return NULL;

    lwm2m_config = lwm2m_configs[ref];
    objects = lwm2m_config->objects;
    for ( i = 0; i < MAX_INIT_OBJECTS; i++)
    {
        if (objects[i].objId == objId)
        {
            return &(objects[i]);
        }
    }

    return NULL;
}

lwm2m_ret_t lwm2m_read_rsp(uint16_t objId, uint16_t instId, uint16_t resId, lwm2m_tpye_t type,
                           const uint8_t *value, uint32_t value_length, bool isEnd, uint8_t result, uint8_t ref)
{
    uint8_t cmd[50] = {0};
    lwm2m_ret_t ret;
    uint8_t *data ;
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[50] = {0};
    //lwm2m_ret_t ret;
    snprintf((char *)cmd, 50, "read %d %d %d %d %d %d %d", objId, instId, resId, isEnd, result, type, (unsigned int)value_length);
    data = lwm2m_malloc(strlen((char *)cmd) + 1 + value_length + 1);
    memset(data, 0, strlen((char *)cmd) + 1 + value_length + 1);
    memcpy(data, cmd, strlen((char *)cmd));
    memcpy(data + strlen((char *)cmd) + 1, value, value_length);
    ret = lwm2m_excute_cmd(data, strlen((char *)cmd) + 1 + value_length + 1, ref);
    lwm2m_free(data);
    return ret;
}

lwm2m_ret_t lwm2m_write_rsp(bool result, uint8_t ref)
{
    uint8_t cmd[20] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[20] = {0};
    sprintf((char *)cmd, "write %d", result);
    return lwm2m_excute_data_cmd(cmd, strlen((char *)cmd), ref);
}

lwm2m_ret_t lwm2m_exec_rsp(bool result, uint8_t ref)
{
    uint8_t cmd[20] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[20] = {0};
    sprintf((char *)cmd, "execute %d", result);
    return lwm2m_excute_data_cmd(cmd, strlen((char *)cmd), ref);
}

lwm2m_ret_t lwm2m_discover_rsp(uint8_t result, const uint8_t *value, uint8_t ref)
{
    uint8_t cmd[256] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;
    if (strlen((char *)value) > 200)
        return LWM2M_RET_ERROR;

    //uint8_t cmd[256] = {0};
    snprintf((char *)cmd, 255, "discover %d %s", result, value);
    return lwm2m_excute_data_cmd(cmd, strlen((char *)cmd), ref);
}

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
lwm2m_ret_t lwm2m_observe_rsp_onenet(uint16_t mid, uint8_t result, bool isAutoAck, uint8_t ref)
{
    uint8_t cmd[256] = {0};
    SCI_TRACE_LOW( "  LWM2MDEBUG  observe_rsp_onenet  mid : %d [%s  %d]\n",mid,__FUNCTION__,__LINE__);
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    if (result < ONENET_COAP_204_CHANGED || result > ONENET_COAP_503_SERVICE_UNAVAILABLE)
    {
        return LWM2M_RET_ERROR;
    }

    //uint8_t cmd[256] = {0};
    snprintf((char *)cmd, 255, "OnenetObserve %d %d %d", mid, result, isAutoAck);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}

lwm2m_ret_t lwm2m_discover_rsp_onenet(uint16_t mid, uint8_t result, bool isAutoAck, const uint8_t *value, uint8_t ref)
{
    uint8_t cmd[256] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    if (strlen((char *)value) > 200)
        return LWM2M_RET_ERROR;

    //uint8_t cmd[256] = {0};
    snprintf((char *)cmd, 255, "OnenetDiscover %d %d %d %s", mid, result, isAutoAck, value);
    SCI_TRACE_LOW(" LWM2MDEBUG CMD LEN :%d  [ %s  %d]\n",strlen((char *)cmd),__FUNCTION__,__LINE__);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}

lwm2m_ret_t lwm2m_notify_onenet(uint16_t mid, uint16_t objId, uint16_t instId, uint16_t resId, lwm2m_tpye_t type,
                                const uint8_t *value, uint32_t value_length, uint8_t result, uint8_t coapType, uint8_t ref)
{
   // if (!isRefValid(ref) || !isRefRegistered(ref))
    //    return LWM2M_RET_ERROR;

    uint8_t cmd[300] = {0};
    lwm2m_ret_t ret;
    uint8_t *data;
    
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;
    
    snprintf((char *)cmd, 100, "OnenetNotify %d /%d/%d/%d %d %d %d %d", mid, objId, instId, resId, result, type, (unsigned int)value_length, coapType);
    data = lwm2m_malloc(strlen((char *)cmd) + 1 + value_length + 1);
    memset(data, 0, strlen((char *)cmd) + 1 + value_length + 1);
    memcpy(data, cmd, strlen((char *)cmd));
    if (value != NULL)
        memcpy(data + strlen((char *)cmd) + 1, value, value_length);
    ret = lwm2m_excute_cmd(data, strlen((char *)cmd) + 1 + value_length + 1, ref);
    lwm2m_free(data);
    return ret;
}

lwm2m_ret_t lwm2m_read_rsp_onenet(uint16_t mid, uint16_t objId, uint16_t instId, uint16_t resId, lwm2m_tpye_t type,
                                  const uint8_t *value, uint32_t value_length, uint8_t result, uint8_t ref)
{
    //if (!isRefValid(ref) || !isRefRegistered(ref))
       // return LWM2M_RET_ERROR;

    uint8_t cmd[256] = {0};
    lwm2m_ret_t ret;
     uint8_t *data;
    if(!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;
    SCI_TRACE_LOW( "LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
    snprintf((char *)cmd, 50, "OnenetRead %d /%d/%d/%d %d %d %d", mid, objId, instId, resId, result, type, (unsigned int)value_length);
    data = lwm2m_malloc(strlen((char *)cmd) + 1 + value_length + 1);
    memset(data, 0, strlen((char *)cmd) + 1 + value_length + 1);
    memcpy(data, cmd, strlen((char *)cmd));
    if (value != NULL)
        memcpy(data + strlen((char *)cmd) + 1, value, value_length);
    ret = lwm2m_excute_cmd(data, strlen((char *)cmd) + 1 + value_length + 1, ref);
    lwm2m_free(data);
    return ret;
}

lwm2m_ret_t lwm2m_write_rsp_onenet(uint16_t mid, uint8_t result, uint8_t ref)
{
    uint8_t cmd[64] = {0};;
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[64] = {0};
    sprintf((char *)cmd, "OnenetWrite %d %d", mid, result);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}

lwm2m_ret_t lwm2m_execute_rsp_onenet(uint16_t mid, uint8_t result, uint8_t ref)
{
    uint8_t cmd[64] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[64] = {0};
    sprintf((char *)cmd, "OnenetExec %d %d", mid, result);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}

lwm2m_ret_t lwm2m_parameter_rsp_onenet(uint16_t mid, uint8_t result, uint8_t ref)
{
    uint8_t cmd[64] = {0};;
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[64] = {0};
    sprintf((char *)cmd, "OnenetParam %d %d", mid, result);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}

lwm2m_ret_t lwm2m_clearFwState_onenet(bool fwFlag, uint8_t ref)
{
    if (!isRefValid(ref))
        return LWM2M_RET_ERROR;

    lwm2m_configs[ref]->clearstate = fwFlag;
    return 0;
}

#ifdef CONFIG_LWM2M_ONENET_SOTA
lwm2m_ret_t lwm2m_set_sota_info_onenet(char *version, uint32_t upgrade_space, uint8_t ref)
{
    if (!isRefValid(ref))
        return LWM2M_RET_ERROR;
    if (version == NULL)
        return LWM2M_RET_ERROR;
    if (strlen(version) > 16)
        return LWM2M_RET_ERROR;
    if (!cissys_setSotaVersion(version))
        return LWM2M_RET_ERROR;
    cissys_setSotaMemory(upgrade_space);
    return LWM2M_RET_OK;
}

lwm2m_ret_t lwm2m_notify_sota_result_onenet(cissys_event_t eventid, uint8_t ref)
{
    uint8_t cmd[64] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[64] = {0};
    sprintf((char *)cmd, "OnenetSotaResult %d", eventid);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}
#endif

lwm2m_ret_t lwm2m_SetResMode_onenet(uint8_t Resmode, uint8_t ref)
{
    if (!isRefValid(ref))
        return LWM2M_RET_ERROR;

    lwm2m_configs[ref]->ResMode = Resmode;
    return 0;
}

#endif

lwm2m_ret_t lwm2m_start_fota_download(const uint8_t *uri, uint8_t ref)
{
    uint8_t cmd[256] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;
    if (strlen((char *)uri) > 200)
        return LWM2M_RET_ERROR;

    //uint8_t cmd[256] = {0};
    snprintf((char *)cmd, 255, "download_fota %s", uri);
    LOG_ARG("lwm2m_start_fota_download %s", uri);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}

lwm2m_ret_t lwm2m_notify(uint16_t objId, uint16_t instId, uint16_t resId, const uint8_t *value,
                         uint32_t value_length, lwm2m_tpye_t type, uint8_t ref)
{
    return lwm2m_notify_with_typeex(objId, instId, resId, value, value_length, type, ref, NOTIFY_NON, 0, 0);
}

lwm2m_ret_t lwm2m_notify_with_type(uint16_t objId, uint16_t instId, uint16_t resId, const uint8_t *value,
                                   uint32_t value_length, lwm2m_tpye_t type, uint8_t ref, uint8_t notifyType)
{
    return lwm2m_notify_with_typeex(objId, instId, resId, value, value_length, type, ref, notifyType, 0, 0);
}

lwm2m_ret_t lwm2m_notify_with_typeex(uint16_t objId, uint16_t instId, uint16_t resId, const uint8_t *value,
                                     uint32_t value_length, lwm2m_tpye_t type, uint8_t ref, uint8_t notifyType, uint8_t seqNo, uint8_t raiMode)
{
    //if (!isRefValid(ref) || !isRefRegistered(ref))
       // return LWM2M_RET_ERROR;

    uint8_t cmd[50] = {0};
    lwm2m_ret_t ret;
    uint8_t *data;

    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    if (value == NULL)
        value_length = 0;

    snprintf((char *)cmd, 50, "change /%d/%d/%d %d %d %d %d %d", objId, instId, resId, type, (unsigned int)value_length,
             notifyType, seqNo, raiMode);
    OSI_LOGXI(OSI_LOGPAR_S, 0x1000984f, "notify type %s ", cmd);
    data = lwm2m_malloc(strlen((char *)cmd) + 1 + value_length + 1);
    if(data == NULL)
    {
        return LWM2M_RET_ERROR;
    }
    memset(data, 0, strlen((char *)cmd) + 1 + value_length + 1);
    memcpy(data, cmd, strlen((char *)cmd));
    memcpy(data + strlen((char *)cmd) + 1, value, value_length);
    ret = lwm2m_excute_cmd(data, strlen((char *)cmd) + 1 + value_length + 1, ref);
    lwm2m_free(data);
    return ret;
}

lwm2m_ret_t lwm2m_notify_fota_state(uint32_t state, uint32_t resulte, uint8_t ref)
{
    uint8_t cmd[50] = {0};
    lwm2m_ret_t ret;
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[50] = {0};
    //lwm2m_ret_t ret;
    snprintf((char *)cmd, 50, "notify_fota %ld %ld", state, resulte);
    ret = lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
    return ret;
}

lwm2m_ret_t lwm2m_update(uint32_t lifetime, bool withObject, uint8_t ref)
{
    uint8_t cmd[50] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[50] = {0};
    sprintf((char *)cmd, "update %d %d", (unsigned int)lifetime, withObject);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}

#ifdef CONFIG_LWM2M_CTWING_SUPPORT

int get_err_code(uint32_t result)
{
    int err_code = 0;
    switch (result)
    {
    case COAP_400_BAD_REQUEST:
        err_code = 10;
        break;
    case COAP_403_BAD_FORBIDDEN:
        err_code = 13;
        break;
    case COAP_404_NOT_FOUND:
        err_code = 14;
        break;
    case 0xff: // send failed
        err_code = 2;
        break;
    case COAP_412_PRECONDITION_FAILED:
        err_code = 22;
        break;
    default:
        break;
    }
    return err_code;
}
lwm2m_ret_t lwm2m_update_binding(uint32_t binding, uint8_t ref)
{
    uint8_t cmd[50] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;

    //uint8_t cmd[50] = {0};
    sprintf((char *)cmd, "update_binding %d", (unsigned int)binding);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}
#endif

lwm2m_ret_t lwm2m_set_parameter(uint16_t objId, uint16_t instId, uint16_t resId,
                                uint8_t *parameter, uint8_t ref)
{
    uint8_t cmd[256] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;
    if (strlen((char *)parameter) > 200)
        return LWM2M_RET_ERROR;

    //uint8_t cmd[256] = {0};
    snprintf((char *)cmd, 255, "param %d %d %d %s", objId, instId, resId, parameter);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}

lwm2m_ret_t lwm2m_register(int ref, uint32_t lifetime, uint32_t timeout)
{
     lwm2m_config_t *lwm2m_config;
    if (!isRefValid(ref) || isRefRegistered(ref))
        return LWM2M_RET_ERROR;
    SCI_TRACE_LOW( " LWM2MDEBUG enter ------------> :[%s  %d]\n",__FUNCTION__,__LINE__);     
    SCI_TRACE_LOW( " LWM2MDEBUG ref :%d  lifetime : %d   timeout: %d:[%s  %d]\n",ref,lifetime,timeout,__FUNCTION__,__LINE__);   

    lwm2m_config = lwm2m_configs[ref];
    lwm2m_config->pending_regcmd = 1;
    lwm2m_config->isquit = 0;
    lwm2m_config->lifetime = lifetime;
    lwm2m_config->timeout = timeout;

   
    //lwm2m_config->this = (uint32_t)osiThreadCreate("lwm2m_clientd", lwm2m_clientd, (void *)ref, OSI_PRIORITY_NORMAL, 3072 * 4, 32);
    lwm2m_config->this = (uint32_t)lwm2m_Thread_start("lwm2m_clientd",lwm2m_clientd,(void *)ref);
    
    SCI_TRACE_LOW( " LWM2MDEBUG end <------------ :[%s  %d]\n",__FUNCTION__,__LINE__);     
    if (lwm2m_config->this == 0)
    {
        OSI_LOGI(0x10007bc7, "lwm2m_register create task failed.");
        return LWM2M_RET_ERROR;
    }
    else
    {
        return LWM2M_RET_OK;
    }
}
lwm2m_ret_t QuitHandshake();

lwm2m_ret_t lwm2m_unregister(int ref)
{
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;
    
    OSI_LOGI(0x10007bc7, " enter lwm2m_unregister .");
    lwm2m_configs[ref]->isquit = 1;
    QuitHandshake();
    
    return lwm2m_excute_cmd((uint8_t *)"quit", 4, ref);
}

lwm2m_ret_t lwm2m_set_dynamic_ipso(int ref)
{
    lwm2m_config_t *lwm2m_config;
    if (!isRefValid(ref) || isRefRegistered(ref))
        return LWM2M_RET_ERROR;
    lwm2m_config = lwm2m_configs[ref];
    lwm2m_config->use_dynamic_ipso = 1;
    return LWM2M_RET_OK;
}

lwm2m_ret_t lwm2m_set_send_flag(int ref, int flag)
{
    uint8_t cmd[15] = {0};
    if (!isRefValid(ref) || !isRefRegistered(ref))
        return LWM2M_RET_ERROR;
    //uint8_t cmd[15] = {0};
    sprintf((char *)cmd, "setflag %d", flag);
    return lwm2m_excute_cmd(cmd, strlen((char *)cmd), ref);
}

bool lwm2m_is_dynamic_ipso(int ref)
{
    lwm2m_config_t *lwm2m_config;
    if (!isRefValid(ref))
        return false;
    lwm2m_config = lwm2m_configs[ref];
    return lwm2m_config->use_dynamic_ipso;
}

#define MALLOC_BLK 16
static void lwm2m_clientd(uint32 argc, void *argv)
{
    uint8_t ref = (uint8_t)(uint32_t)argv;
    lwm2m_config_t *lwm2m_config = lwm2m_configs[ref];
    int ret = -1;
    struct sockaddr_in to4 = {0};
    int begin;
    int i;

    OSI_LOGI(0x10007bc8, "lwm2m_clientd task is running.  ref : %d",ref);
    

    if (lwm2m_config == NULL )//|| netif_default == NULL)  xiameng.yang
    {
        OSI_LOGI(0x10007bc8, "lwm2m_clientd task go exit !.  ");
        goto EXIT;
    }

    // create & bind ipc socket
    OSI_LOGI(0x10007bc8, "lwm2m_clientd create ipc socket !.  ");
    lwm2m_config->ipc_socket = TO_SOCKET_FD(sci_sock_socket(PF_INET, SOCK_DGRAM, 0,sci_getLoopBackNetid()));
    if (lwm2m_config->ipc_socket < 0)
        {
        OSI_LOGI(0x10007bc8, "lwm2m_clientd task go exit !.  ");
        goto EXIT;
        }
    //struct sockaddr_in to4 = {0};
    //to4.sin_len = sizeof(struct sockaddr_in);  //xiameng.yang
    to4.sin_family = AF_INET; 
    to4.sin_port = 1234 + ref;
    //to4.sin_addr.s_addr = PP_HTONL(INADDR_LOOPBACK);
    to4.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(lwm2m_config->ipc_socket, (const struct sockaddr *)&to4, sizeof(to4)) < 0)
        {
        OSI_LOGI(0x10007bc8, "lwm2m_clientd task go exit !.  ");
        goto EXIT;
        }
    SCI_TRACE_LOW( "  LWM2MDEBUG    ipc_socket:  0x%x  to4.sin_port :%d [%s  %d]\n",lwm2m_config->ipc_socket,to4.sin_port, __FUNCTION__,__LINE__);
    // create & bind ipc data socket
    lwm2m_config->ipc_data_socket = TO_SOCKET_FD(sci_sock_socket(PF_INET, SOCK_DGRAM, 0,sci_getLoopBackNetid()));
    if (lwm2m_config->ipc_data_socket < 0)
        {
        OSI_LOGI(0x10007bc8, "lwm2m_clientd task go exit !.  ");
        goto EXIT;
        }
    //to4.sin_len = sizeof(struct sockaddr_in);  //xiameng.yang
    to4.sin_family = AF_INET;
    to4.sin_port = 2235 + ref;
    //to4.sin_addr.s_addr = PP_HTONL(INADDR_LOOPBACK);
    to4.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(lwm2m_config->ipc_data_socket, (const struct sockaddr *)&to4, sizeof(to4)) < 0)
        {
        OSI_LOGI(0x10007bc8, "lwm2m_clientd task go exit !.  ");
        goto EXIT;
        }
        
    SCI_TRACE_LOW( "  LWM2MDEBUG    ipc_data_socket:  0x%x   [%s  %d]\n",lwm2m_config->ipc_data_socket,__FUNCTION__,__LINE__);

    begin = lwm2m_config->argc;

    lwm2m_config->argv[0] = ((uint8_t *)"lwm2mclient_main");
    lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
    strcpy((char *)lwm2m_config->argv[lwm2m_config->argc++], "-f");
    lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
    snprintf(((char *)(lwm2m_config->argv[lwm2m_config->argc++])), MALLOC_BLK, "%d", ref);
    lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
    strcpy((char *)lwm2m_config->argv[lwm2m_config->argc++], "-k");
    lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
    snprintf((char *)lwm2m_config->argv[lwm2m_config->argc++], MALLOC_BLK, "%d", (unsigned int)lwm2m_config->ipc_socket);

    if (lwm2m_config->lifetime > 0)
    {
        lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
        strcpy((char *)lwm2m_config->argv[lwm2m_config->argc++], "-t");
        lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
        snprintf((char *)lwm2m_config->argv[lwm2m_config->argc++], MALLOC_BLK, "%d", (unsigned int)lwm2m_config->lifetime);
    }

    if (lwm2m_config->timeout > 0)
    {
        lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
        strcpy((char *)lwm2m_config->argv[lwm2m_config->argc++], "-o");
        lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
        snprintf((char *)lwm2m_config->argv[lwm2m_config->argc++], MALLOC_BLK, "%d", (unsigned int)lwm2m_config->timeout);
    }

    if (lwm2m_config->clearstate != 0)
    {
        lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
        strcpy((char *)lwm2m_config->argv[lwm2m_config->argc++], "-m");
        lwm2m_config->argv[lwm2m_config->argc] = lwm2m_malloc(MALLOC_BLK);
        snprintf((char *)lwm2m_config->argv[lwm2m_config->argc++], MALLOC_BLK, "%d", (unsigned int)lwm2m_config->clearstate);
    }
    lwm2m_config->isregisted = 1;
    ret = lwm2mclient_main(lwm2m_config->argc, (char **)lwm2m_config->argv);
    lwm2m_config->isregisted = 0;

    for ( i = begin; i < lwm2m_config->argc; i++)
    {
        lwm2m_free(lwm2m_config->argv[i]);
        lwm2m_config->argv[i] = NULL;
    }
    lwm2m_config->argc = begin;

    if (lwm2m_is_dynamic_ipso(ref))
    {
        ipso_obj_t *obj_type_table = lwm2m_config->obj_type_table;
        memset(obj_type_table, 0, sizeof(ipso_obj_t) * MAX_DYNAMIC_OBJECTS);
    }

EXIT:
    OSI_LOGI(0x10007bc9, "lwm2m_clientd task is quit.");

    if (lwm2m_config != NULL)
    {
        if (lwm2m_config->ipc_socket >= 0)
        {
            close(lwm2m_config->ipc_socket);
            lwm2m_config->ipc_socket = -1;
        }
        if (lwm2m_config->ipc_data_socket >= 0)
        {
            close(lwm2m_config->ipc_data_socket);
            lwm2m_config->ipc_data_socket = -1;
        }
        lwm2m_config->this = 0;
    }
    SCI_TRACE_LOW(" LWM2MDEBUG     EXIT   ALL!!! [%s  %d]\n",__FUNCTION__,__LINE__);
    lwm2mPostEvent(ref, SERVER_QUIT_IND, ret, 0, 0);
    
    lwm2mPostEvent(ref, MESSAGE_TASK_EXIT_IND, ret, 0, 0);  //EXIT MESSAGE TASK;
    lwm2m_Thread_stop();
}

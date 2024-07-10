/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Julien Vermillard, Sierra Wireless
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "internals.h"
#include "liblwm2m.h"
#include "lwm2m_api.h"
#include "commandline.h"
#include "objtable.h"
//#include "osi_log.h"   //modify by xiameng.yang

typedef struct _skel_res_
{
    uint16_t resId;
    lwm2m_data_type_t type;
    uint8_t *data;
    double floatValue;
    uint32_t len;
    bool hasNotify;
    bool hasRead;
#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
    uint8_t notifyType;
    uint8_t seqNum;
    uint8_t raiMode;
#endif
} skel_res_t;

typedef struct _skel_instance_
{
    struct _skel_instance_ *next; // matches lwm2m_list_t::next
    uint16_t instanceId;
    skel_res_t *dataArray;
    int arraySize; //Add to avoid mem leak when free dataArray
    bool isResReady;
} skel_instance_t;

#define _PRV_STR_LENGTH 32

static int skel_valuedecode(lwm2m_data_t *dataP,
                            uint8_t **bufferP, int type)
{
    size_t res;

    switch (type)
    {
    case LWM2M_TYPE_OPAQUE:
    case LWM2M_TYPE_STRING:
        *bufferP = (uint8_t *)lwm2m_malloc(dataP->value.asBuffer.length + 1);
        if (*bufferP == NULL)
            return -1;
        memset(*bufferP, 0, dataP->value.asBuffer.length + 1);
        memcpy(*bufferP, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
        (*bufferP)[dataP->value.asBuffer.length] = 0;
        return (int)dataP->value.asBuffer.length;
    case LWM2M_TYPE_INTEGER:
    {
        uint8_t intString[_PRV_STR_LENGTH];
        int64_t v = 0;
        res = lwm2m_data_decode_int(dataP, &v);
        if (res != 0)
            res = utils_intToText(v, intString, _PRV_STR_LENGTH);
        if (res == 0)
            return -1;

        *bufferP = (uint8_t *)lwm2m_malloc(res + 1);
        if (NULL == *bufferP)
            return -1;
        memset(*bufferP, 0, res + 1);
        memcpy(*bufferP, intString, res);
        (*bufferP)[res] = 0;
        return (int)res;
    }
    case LWM2M_TYPE_FLOAT:
    {
        uint8_t floatString[_PRV_STR_LENGTH * 2];
        double v = 0;
        res = lwm2m_data_decode_float(dataP, &v);
        if (res != 0)
            res = utils_floatToText(v, floatString, _PRV_STR_LENGTH * 2);
        if (res == 0)
            return -1;

        *bufferP = (uint8_t *)lwm2m_malloc(res + 1);
        if (NULL == *bufferP)
            return -1;
        memset(*bufferP, 0, res + 1);
        memcpy(*bufferP, floatString, res);
        (*bufferP)[res] = 0;
        return (int)res;
    }
    case LWM2M_TYPE_BOOLEAN:
    {
        bool v = 0;
        res = lwm2m_data_decode_bool(dataP, &v);
        if (res == 1)
        {
            char *sv = v ? "1" : "0";
            *bufferP = (uint8_t *)lwm2m_malloc(strlen(sv) + 1);
            if (NULL == *bufferP)
                return -1;
            memset(*bufferP, 0, strlen(sv) + 1);
            strcpy((char *)(*bufferP), sv);
            return strlen(sv);
        }
        else
            return -1;
    }
    case LWM2M_TYPE_UNDEFINED:
    default:
        return -1;
    }
}

static int wait_and_process_rsp(lwm2m_context_t *context, command_desc_t *skel_commands)
{
    struct timeval tv = {0};
    fd_set readfds;
    int result = -1;

    uint8_t buffer[256];
    int numBytes = 0;
    uint8_t ipc_socket = lwm2m_get_ipcDataSocket(context->ref);
    bool check_update = false;
    time_t timeout = 0;
retry:
    tv.tv_sec = 5 - timeout;
    check_update = 0;
    timeout = 0;
    registration_step(context, lwm2m_gettime(), &timeout);
    if (timeout != 0 && timeout <= tv.tv_sec)
    {
        tv.tv_sec = timeout;
        check_update = true;
        OSI_LOGI(0x10007bd5, "wait_and_process_rsp need check update timeout=%ld", (long)timeout);
    }
    FD_ZERO(&readfds);
    FD_SET(ipc_socket, &readfds);
    result = select(ipc_socket + 1, &readfds, NULL, NULL, &tv);
    if (result == 0)
    {
        if (check_update)
            goto retry;
        else
            result = -1;
    }
    else if (result > 0)
    {
        if (FD_ISSET(ipc_socket, &readfds))
        {
            numBytes = read(ipc_socket, buffer, 256 - 1);
            if (numBytes > 1)
            {
                buffer[numBytes] = 0;
                result = handle_command(skel_commands, (char *)buffer);
            }
        }
    }
    return result;
}

//AT+MIPLREAD=<ref>, <msgid>, <objectid>, <instanceid>, <resourceid>, <valuetype>,<len>, <value>,<flag><CR>
static int prv_readrsp(char *buffer,
                       void *user_data)
{
    uint16_t objid;
    uint16_t instid;
    uint16_t resid;
    bool isEnd;
    uint8_t result ;
    lwm2m_tpye_t type;
    uint32_t value_length;
    uint8_t *data;
    lwm2m_object_t *skelObj;
    skel_instance_t *targetP;
    ipso_res_t *ipso_res ;
    ipso_obj_t *ipso_obj;
    int nbRes;
    int i ;
    
    lwm2m_context_t *lwm2mH = (lwm2m_context_t *)user_data;
    int value[7];
    lwm2m_parse_buffer(buffer, value, 7, NULL);
    objid = atoi((const char *)value[0]);
    buffer += strlen((char *)(value[0])) + 1;
    instid = atoi((const char *)value[1]);
    buffer += strlen((char *)(value[1])) + 1;
     resid = atoi((const char *)value[2]);
    buffer += strlen((char *)(value[2])) + 1;
    isEnd = atoi((const char *)value[3]);
    buffer += strlen((char *)(value[3])) + 1;
    result = atoi((const char *)value[4]);
    buffer += strlen((char *)(value[4])) + 1;
    type = (lwm2m_tpye_t)atoi((const char *)value[5]);
    buffer += strlen((char *)(value[5])) + 1;
    value_length = (uint32_t)atoi((const char *)value[6]);
    buffer += strlen((char *)(value[6])) + 1;
    data = (uint8_t *)buffer;

    OSI_LOGI(0x10007bd6, "prv_readrsp objid=%d,instid=%d,resid=%d,isEnd=%d", objid, instid, resid, isEnd);
    OSI_LOGXI(OSI_LOGPAR_IIS, 0x10007bd7, "prv_readrsp result=%d, type=%d, data=%s", result, type, data);

    if (result == 1)
    {
        result = COAP_205_CONTENT;
    }
    else if (result == 11)
    {
        result = COAP_400_BAD_REQUEST;
    }
    else if (result == 12)
    {
        result = COAP_401_UNAUTHORIZED;
    }
    else if (result == 13)
    {
        result = COAP_404_NOT_FOUND;
    }
    else if (result == 14)
    {
        result = COAP_405_METHOD_NOT_ALLOWED;
    }
    else if (result == 15)
    {
        result = COAP_406_NOT_ACCEPTABLE;
    }
    else
    {
        result = COAP_500_INTERNAL_SERVER_ERROR;
    }

    if (result != COAP_205_CONTENT)
    {
        return result;
    }

    skelObj = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mH->objectList, objid);
    if (skelObj == NULL)
        return COAP_404_NOT_FOUND;
    targetP = (skel_instance_t *)lwm2m_list_find(skelObj->instanceList, instid);

    if (NULL == targetP || !lwm2m_isResReady(objid, lwm2mH->ref))
        return COAP_404_NOT_FOUND;
    ipso_res = NULL;
    ipso_obj = lwm2m_get_ipso_obj(objid, resid, &ipso_res, lwm2mH->ref);
    if (ipso_obj == NULL || ipso_res == NULL || ipso_obj->resCount == 0)
        return COAP_404_NOT_FOUND;

    nbRes = ipso_obj->resCount;
    if (ipso_res->value_type == 0)
        ipso_res->value_type = type;
    else if (type == 0)
        type = ipso_res->value_type;
    else if (ipso_res->value_type != type)
        return COAP_400_BAD_REQUEST;

    for ( i = 0; i < nbRes; i++)
    {
        if (targetP->dataArray[i].resId == resid)
        {
            if (type + 3 == LWM2M_TYPE_FLOAT)
            {
                double vd;
                utils_textToFloat(data, value_length, &vd);
                targetP->dataArray[i].floatValue = vd;
            }

            targetP->dataArray[i].data = lwm2m_malloc(value_length + 1);
            memcpy(targetP->dataArray[i].data, data, value_length);
            targetP->dataArray[i].len = value_length;
            targetP->dataArray[i].type = type;
            targetP->dataArray[i].hasRead = true;
            break;
        }
    }
    return isEnd ? 1 : result;
}

//+MIPLREAD: <ref>, <msgid>, <objectid>, <instanceid>, <resourceid>, [<count>]
static int lwm2m_skel_readvalue(lwm2m_context_t *context, read_param_t *param)
{
    int result = -1;
    //lwm2mPostEvent(context->ref, OBJ_READ_IND, coap_get_mid(), (uint32_t)param, 0);
    
    command_desc_t skel_commands[] =
        {
            {"read", "read rsp.", NULL, prv_readrsp, NULL},
            COMMAND_END_LIST};
    lwm2mPostEvent(context->ref, OBJ_READ_IND, coap_get_mid(), (uint32_t)param, 0);
    skel_commands[0].userData = context;
    while (1)
    {
        result = wait_and_process_rsp(context, skel_commands);
        if (result == COAP_205_CONTENT || result == 1)
        {
            lwm2mPostEvent(context->ref, OBJ_OPERATE_RSP_IND, 1, OBJ_READ_IND, 0);
        }
        else
        {
            if (result != -1)
            {
                lwm2mPostEvent(context->ref, OBJ_OPERATE_RSP_IND, -1, OBJ_READ_IND, 0);
            }
            break;
        }

        if (result == 1)
        {
            result = COAP_205_CONTENT;
            break;
        }
    }
    lwm2m_free(param);
    return result;
}

static uint8_t prv_get_value(lwm2m_data_t *dataP,
                             skel_instance_t *targetP, int nbRs)
{
    int i;
    for (i = 0; i < nbRs; i++)
    {
        if (targetP->dataArray[i].resId == dataP->id)
        {
            uint8_t *data = targetP->dataArray[i].data;
            uint32_t len = targetP->dataArray[i].len;
            if (data == NULL)
                return COAP_406_NOT_ACCEPTABLE;
#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
            dataP->notify_type = targetP->dataArray[i].notifyType;
            dataP->seq_num = targetP->dataArray[i].seqNum;
            dataP->rai_mode = targetP->dataArray[i].raiMode;
#endif
            switch (targetP->dataArray[i].type + 3)
            {
            case LWM2M_TYPE_STRING:
                lwm2m_data_encode_string((char *)data, dataP);
                break;
            case LWM2M_TYPE_OPAQUE:
                lwm2m_data_encode_opaque(data, len, dataP);
                break;
            case LWM2M_TYPE_INTEGER:
                lwm2m_data_encode_int(atoi((const char *)data), dataP);
                break;
            case LWM2M_TYPE_FLOAT:
                lwm2m_data_encode_float(targetP->dataArray[i].floatValue, dataP);
                break;
            case LWM2M_TYPE_BOOLEAN:
                lwm2m_data_encode_bool(atoi((const char *)data), dataP);
                break;
            default:
                return COAP_406_NOT_ACCEPTABLE;
            }
            lwm2m_free(data);
            targetP->dataArray[i].data = NULL;
            return COAP_205_CONTENT;
        }
    }
    return COAP_404_NOT_FOUND;
}

static uint8_t prv_skel_read(uint16_t instanceId,
                             int *numDataP,
                             lwm2m_data_t **dataArrayP,
                             lwm2m_object_t *objectP)
{
    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;
    skel_instance_t *targetP;
    uint8_t result = COAP_205_CONTENT;
    int i;
    ipso_obj_t *ipso_obj;
    int nbRes ;
    ipso_res_t *ipso_res_array ;
    read_param_t *readparam;
    bool isHasNofity ;
    //int readCount = 0;
    int index ;

    OSI_LOGI(0x10007bd8, "prv_skel_read instanceId:%d, numData=%d", instanceId, *numDataP);

    targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);

    if (NULL == targetP || !lwm2m_isResReady(objectP->objID, contextP->ref))
        return COAP_404_NOT_FOUND;

    ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
        return COAP_404_NOT_FOUND;

    nbRes = ipso_obj->resCount;
    ipso_res_array = ipso_obj->resArray;

    readparam = (read_param_t *)lwm2m_malloc(sizeof(read_param_t));
    readparam->objid = objectP->objID;
    readparam->insid = instanceId;
    readparam->resid = 0;
    readparam->count = 0;

    // is the server asking for the full instance ?
    isHasNofity = false;
    //int readCount = 0;
    index = 0;
    if (*numDataP == 0)
    {
        for (i = 0; i < nbRes; i++)
        {
            if (targetP->dataArray[i].hasNotify)
            {
                lwm2m_data_t **dataP;
                int *numData;
                lwm2m_free(readparam);
                targetP->dataArray[i].hasNotify = false;

                *dataArrayP = lwm2m_data_new(1);
                if (*dataArrayP == NULL)
                {
                    return COAP_500_INTERNAL_SERVER_ERROR;
                }
                *numDataP = 1;

                (*dataArrayP)[0].type = LWM2M_TYPE_OBJECT_INSTANCE;
                (*dataArrayP)[0].id = instanceId;

                dataP = &((*dataArrayP)[0].value.asChildren.array);
                *dataP = lwm2m_data_new(1);
                if (*dataP == NULL)
                {
                    return COAP_500_INTERNAL_SERVER_ERROR;
                }

                numData = (int *)&((*dataArrayP)[0].value.asChildren.count);
                *numData = 1;
                (*dataP)[0].id = targetP->dataArray[i].resId;

                i = 0;
                do
                {
                    result = prv_get_value((*dataP) + i, targetP, nbRes);
                    i++;
                } while (i < *numData && result == COAP_205_CONTENT);
                return result;
            }
        }

        if (!isHasNofity)
        {
            readparam->resid = -1;
        }
    }
    else if (*numDataP == 1)
    {
        readparam->resid = (*dataArrayP)[0].id;
        for (i = 0; i < nbRes; i++)
        {
            if (readparam->resid == targetP->dataArray[i].resId && targetP->dataArray[i].hasNotify)
            {
                isHasNofity = true;
                targetP->dataArray[i].hasNotify = false;
                break;
            }
        }
    }
    readparam->count = *numDataP;

    if (!isHasNofity)
    {
        int readCount = 0;
        result = lwm2m_skel_readvalue(contextP, readparam);
        //int readCount = 0;
        for (i = 0; i < nbRes; i++)
        {
            if (targetP->dataArray[i].hasRead)
            {
                if (*numDataP == 1 && targetP->dataArray[i].resId != (*dataArrayP)[0].id)
                {
                    readCount = 0;
                    result = COAP_404_NOT_FOUND;
                    break;
                }
                else if (*numDataP == 1)
                {
                    readCount = 1;
                    break;
                }
                else
                {
                    readCount++;
                }
            }
        }
        OSI_LOGI(0x10007bd9, "prv_skel_read read value result: %u.%2u", (result & 0xFF) >> 5, (result & 0x1F));
        OSI_LOGI(0x10007bda, "prv_skel_read readCount = %d", readCount);
        if (readCount == 0)
        {
            OSI_LOGI(0x10007bdb, "prv_skel_read no resources are read");
            return result;
        }
        else
        {
            OSI_LOGI(0x10007bdc, "prv_skel_read some or all resources are read");
        }

        if (*numDataP == 0)
        {
            *dataArrayP = lwm2m_data_new(readCount);
        }
        *numDataP = readCount;
        if (*dataArrayP == NULL)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
        for (index = 0, i = 0; i < nbRes && index < readCount; i++)
        {
            if (targetP->dataArray[i].hasRead)
            {
                (*dataArrayP)[index++].id = ipso_res_array[i].resId;
                targetP->dataArray[i].hasRead = false;
            }
        }
    }
    else
    {
        lwm2m_free(readparam);
    }

    i = 0;
    do
    {
        result = prv_get_value((*dataArrayP) + i, targetP, nbRes);
        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);
    OSI_LOGI(0x10007bdd, "prv_skel_read final result: %u.%2u", (result & 0xFF) >> 5, (result & 0x1F));
    return result;
}

static uint8_t prv_skel_read_object(lwm2m_object_t *objectP, lwm2m_data_t **dataArrayP)
{
    //OSI_LOGI(0x10007bde, "prv_skel_read_object objectP->objID: %d", objectP->objID);

    int i;
    int j;
    int index;
    int nbRes;
    int readCount = 0;
    uint8_t result = COAP_205_CONTENT;

    ipso_obj_t *ipso_obj;
    lwm2m_list_t *instanceP;
    skel_instance_t *targetP;
    ipso_res_t *ipso_res_array;
    read_param_t *readparam;
    
    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;
    OSI_LOGI(0x10007bde, "prv_skel_read_object objectP->objID: %d", objectP->objID);

    instanceP = objectP->instanceList;
    if (instanceP == NULL)
    {
        OSI_LOGI(0x10007bdf, "prv_skel_read_object objectP->instanceList is NULL");
        return COAP_404_NOT_FOUND;
    }

    i = 0;
    while (instanceP != NULL)
    {
        targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceP->id);
        if (NULL == targetP || !lwm2m_isResReady(objectP->objID, contextP->ref))
        {
            return COAP_404_NOT_FOUND;
        }

        ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
        if (ipso_obj == NULL || ipso_obj->resCount == 0)
        {
            return COAP_404_NOT_FOUND;
        }

        nbRes = ipso_obj->resCount;
        ipso_res_array = ipso_obj->resArray;
        for (j = 0; j < nbRes; j++)
        {
            if (targetP->dataArray[j].hasNotify)
            {
                int *numDataP;
                lwm2m_data_t **dataP = &((*dataArrayP)[i].value.asChildren.array);
                *dataP = lwm2m_data_new(1);
                if (*dataP == NULL)
                {
                    return COAP_500_INTERNAL_SERVER_ERROR;
                }

                numDataP = (int *)&((*dataArrayP)[i].value.asChildren.count);
                *numDataP = 1;

                targetP->dataArray[j].hasNotify = false;
                (*dataP)[0].id = ipso_res_array[j].resId;

                result = COAP_205_CONTENT;
                index = 0;
                do
                {
                    result = prv_get_value((*dataP) + index, targetP, nbRes);
                    index++;
                } while (index < *numDataP && result == COAP_205_CONTENT);
                return result;
            }
        }
        i++;
        instanceP = instanceP->next;
    }

    readparam = (read_param_t *)lwm2m_malloc(sizeof(read_param_t));
    readparam->objid = objectP->objID;
    readparam->insid = -1;
    readparam->resid = -1;

    result = lwm2m_skel_readvalue(contextP, readparam);
    instanceP = objectP->instanceList;
    while (instanceP != NULL)
    {
        targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceP->id);
        for (j = 0; j < nbRes; j++)
        {
            if (targetP->dataArray[j].hasRead)
            {
                readCount++;
                break;
            }
        }
        instanceP = instanceP->next;
    }

    if (readCount == 0)
    {
        OSI_LOGI(0x10007be0, "prv_skel_read_object no resources are read");
        return COAP_404_NOT_FOUND;
    }
    else
    {
        OSI_LOGI(0x10007be1, "prv_skel_read_object some or all resources are read");
        result = COAP_205_CONTENT;
    }

    i = 0;
    instanceP = objectP->instanceList;
    while (instanceP != NULL && result == COAP_205_CONTENT)
    {
        lwm2m_data_t **dataP;
        int *numDataP;
        targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceP->id);
        readCount = 0;
        for (j = 0; j < nbRes; j++)
        {
            if (targetP->dataArray[j].hasRead)
            {
                readCount++;
            }
        }

        if (readCount == 0)
        {
            instanceP = instanceP->next;
            i++;
            continue;
        }

        dataP = &((*dataArrayP)[i].value.asChildren.array);
        *dataP = lwm2m_data_new(readCount);
        if (*dataP == NULL)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }

        numDataP = (int *)&((*dataArrayP)[i].value.asChildren.count);
        *numDataP = readCount;

        ipso_res_array = ipso_obj->resArray;
        for (index = 0, j = 0; j < nbRes && index < readCount; j++)
        {
            if (targetP->dataArray[j].hasRead)
            {
                (*dataP)[index++].id = ipso_res_array[j].resId;
                targetP->dataArray[j].hasRead = false;
            }
        }

        index = 0;
        do
        {
            result = prv_get_value((*dataP) + index, targetP, nbRes);
            index++;
        } while (index < *numDataP && result == COAP_205_CONTENT);

        instanceP = instanceP->next;
        i++;
    }
    OSI_LOGI(0x10007be2, "prv_skel_read_object final result: %u.%2u", (result & 0xFF) >> 5, (result & 0x1F));
    return result;
}

//AT+MIPLDISCOVERRSP=<ref>,<msgid>,<result>,<length>,<valuestring><CR>
static int prv_discoversp(char *buffer,
                          void *user_data)
{
    uint16_t objId = (uint16_t)(uint32_t)user_data;
    uint8_t ref = (uint8_t)((uint32_t)user_data >> 16);
    uint8_t result ;
    uint8_t *vaulestring ;
    int value[2];
    lwm2m_parse_buffer(buffer, value, 2, NULL);
    result = (uint8_t)atoi((const char *)value[0]);
    vaulestring = (uint8_t *)value[1];
    OSI_LOGXI(OSI_LOGPAR_IIS, 0x10007be3, "prv_discoversp result=%d,objId=%d,value=%s", result, objId, vaulestring);
    if (result == 1)
    {
        result = lwm2m_registNewRes(objId, vaulestring, ref);
    }
    return result;
}

//+MIPLDISCOVER:<ref>,<msgid>,<objectid><CR>
static int lwm2m_skel_discover(lwm2m_context_t *context, uint16_t objId)
{
    int result ;
    command_desc_t skel_commands[] =
        {
            {"discover", "discover rsp.", NULL, prv_discoversp, NULL},
            COMMAND_END_LIST};
    lwm2mPostEvent(context->ref, OBJ_DISCOVER_IND, coap_get_mid(), objId, 0);
    result = -1;
    //command_desc_t skel_commands[] =
       // {
         //   {"discover", "discover rsp.", NULL, prv_discoversp, NULL},
          //  COMMAND_END_LIST};
    skel_commands[0].userData = (void *)(objId | context->ref << 16);
    result = wait_and_process_rsp(context, skel_commands);
    if (result == 1)
    {
        lwm2mPostEvent(context->ref, OBJ_OPERATE_RSP_IND, result, OBJ_DISCOVER_IND, 0);
    }
    else
    {
        lwm2mPostEvent(context->ref, OBJ_OPERATE_RSP_IND, -1, OBJ_DISCOVER_IND, 0);
    }
    return result;
}

static uint8_t prv_skel_discover(uint16_t instanceId,
                                 int *numDataP,
                                 lwm2m_data_t **dataArrayP,
                                 lwm2m_object_t *objectP)
{
    uint8_t result;
    ipso_obj_t *ipso_obj;
    int nbRes;
    ipso_res_t *ipso_res_array;
    skel_instance_t *targetP;
    int i;

    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;
    OSI_LOGI(0x10007be4, "prv_skel_discover instanceId:%d, numData=%d", instanceId, *numDataP);

    result = COAP_205_CONTENT;

    if (!lwm2m_isObjSupport(objectP->objID, contextP->ref))
        return COAP_404_NOT_FOUND;
    ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
        return COAP_404_NOT_FOUND;
     nbRes = ipso_obj->resCount;
    ipso_res_array = ipso_obj->resArray;

    targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (!lwm2m_isResDiscover(objectP->objID, contextP->ref))
    {
        result = lwm2m_skel_discover(contextP, objectP->objID);
        if (result == 1)
        {
            result = COAP_205_CONTENT;
        }
        else if (result == 11)
        {
            result = COAP_400_BAD_REQUEST;
        }
        else if (result == 12)
        {
            result = COAP_401_UNAUTHORIZED;
        }
        else if (result == 13)
        {
            result = COAP_404_NOT_FOUND;
        }
        else if (result == 14)
        {
            result = COAP_405_METHOD_NOT_ALLOWED;
        }
        else if (result == 15)
        {
            result = COAP_406_NOT_ACCEPTABLE;
        }
        else
        {
            result = COAP_500_INTERNAL_SERVER_ERROR;
        }

        if (result != COAP_205_CONTENT)
        {
            return result;
        }
    }

    if (targetP != NULL && !targetP->isResReady)
    {
        for ( i = 0; i < nbRes; i++)
        {
            targetP->dataArray[i].resId = ipso_res_array[i].resId;
            targetP->dataArray[i].type = ipso_res_array[i].value_type;
        }
        targetP->isResReady = true;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL)
            return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;
        for (i = 0; i < nbRes; i++)
        {
            (*dataArrayP)[i].id = ipso_res_array[i].resId;
        }
    }
    else
    {
        for ( i = 0; i < *numDataP && result == COAP_205_CONTENT; i++)
        {
            if ((*dataArrayP)[i].id != ipso_res_array[i].resId)
                result = COAP_404_NOT_FOUND;
        }
    }
    return result;
}

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
static uint8_t prv_skel_discover_onenet(uint16_t instanceId,
                                        int *numDataP,
                                        lwm2m_data_t **dataArrayP,
                                        lwm2m_object_t *objectP)
{
    ipso_obj_t *ipso_obj = NULL;

    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;

    OSI_LOGI(0x10007be5, "prv_skel_discover_onenet instanceId:%d, numData=%d", instanceId, *numDataP);

    if (!lwm2m_isObjSupport(objectP->objID, contextP->ref))
        return COAP_404_NOT_FOUND;

    ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
        return COAP_404_NOT_FOUND;

    lwm2mPostEvent(contextP->ref, OBJ_DISCOVER_IND, coap_get_mid(), objectP->objID, 0);

    return COAP_205_CONTENT;
}

static uint8_t prv_skel_read_onenet(uint16_t instanceId,
                                    int *numDataP,
                                    lwm2m_data_t **dataArrayP,
                                    lwm2m_object_t *objectP)
{
    ipso_obj_t *ipso_obj = NULL;

    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;

    OSI_LOGI(0x10007be6, "prv_skel_read_onenet instanceId:%d, numData=%d", instanceId, *numDataP);

    if (!lwm2m_isObjSupport(objectP->objID, contextP->ref))
        return COAP_404_NOT_FOUND;

    ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
        return COAP_404_NOT_FOUND;

    return COAP_205_CONTENT;
}

static uint8_t prv_skel_write_onenet(uint16_t instanceId,
                                     int numData,
                                     lwm2m_data_t *dataArray,
                                     lwm2m_object_t *objectP)
{
    skel_instance_t *targetP;
    ipso_obj_t *ipso_obj;
     int nbRes ;
    write_param_t *writehead ;
    write_param_t *writelist ;
    uint32_t count ;
    int i;
    int j;
    

    
    uint8_t result = COAP_404_NOT_FOUND;
    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;

    targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);

    OSI_LOGI(0x10007be7, "prv_skel_write_onenet instanceId:%d, numData=%d", instanceId, numData);

    if (targetP == NULL || !lwm2m_isResReady(objectP->objID, contextP->ref))
        return COAP_404_NOT_FOUND;

    ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
        return COAP_404_NOT_FOUND;

    nbRes = ipso_obj->resCount;
    writehead = NULL;
    writelist = NULL;
    count = 0;
    
    for (i = numData - 1; i >= 0; i--)
    {
        for (j = 0; j < nbRes; j++)
        {
            if (dataArray[i].id == targetP->dataArray[j].resId)
            {
                uint8_t *data;
                write_param_t *writeparam;
                OSI_LOGI(0x10007be8, "prv_skel_write_onenet dataArray[%d].id=%d, type=%d", i, dataArray[i].id, dataArray[i].type);

                switch (dataArray[i].type)
                {
                case LWM2M_TYPE_OPAQUE:
                case LWM2M_TYPE_STRING:
                case LWM2M_TYPE_INTEGER:
                case LWM2M_TYPE_FLOAT:
                case LWM2M_TYPE_BOOLEAN:
                {
                    
                    int len = skel_valuedecode(&dataArray[i], &data, dataArray[i].type);
                    if (len == -1)
                        break;
                    writeparam = (write_param_t *)lwm2m_malloc(sizeof(write_param_t));
                    writeparam->objid = objectP->objID;
                    writeparam->insid = instanceId;
                    writeparam->resid = dataArray[i].id;
                    writeparam->type = (dataArray[i].type - 3);
                    writeparam->data = data;
                    writeparam->len = len;
                    writeparam->next = NULL;
                    if (writelist != NULL)
                        writelist->next = writeparam;
                    writelist = writeparam;
                    if (writehead == NULL)
                        writehead = writelist;

                    OSI_LOGXI(OSI_LOGPAR_IS, 0x10007be9, "prv_skel_write_onenet len=%d, data=%s", len, data);

                    count++;
                }
                    result = COAP_204_CHANGED;
                    break;
                default:
                    result = COAP_404_NOT_FOUND;
                    break;
                }
            }
        }
    }
    if (writehead != NULL)
    {
        lwm2mPostEvent(contextP->ref, OBJ_WRITE_IND, coap_get_mid(), (uint32_t)writehead, count);
        result = COAP_204_CHANGED;
    }
    return result;
}

static uint8_t prv_skel_execute_onenet(uint16_t instanceId,
                                       uint16_t resourceId,
                                       uint8_t *buffer,
                                       int length,
                                       lwm2m_object_t *objectP)

{
    skel_instance_t *targetP;
     exec_param_t *exec_param;
    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;
    targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
        return COAP_404_NOT_FOUND;

    OSI_LOGI(0x10007bea, "prv_skel_execute_onenet instanceId:%d,resourceId=%d", instanceId, resourceId);

    exec_param = lwm2m_malloc(sizeof(exec_param_t));
    if (exec_param == NULL)
    {
        return COAP_500_INTERNAL_SERVER_ERROR;
    }
    exec_param->objid = objectP->objID;
    exec_param->insid = instanceId;
    exec_param->resid = resourceId;
    exec_param->length = length;
    exec_param->buffer = lwm2m_malloc(length + 1);
    if (exec_param->buffer == NULL)
    {
        free(exec_param);
        return COAP_500_INTERNAL_SERVER_ERROR;
    }
    memcpy(exec_param->buffer, buffer, length);
    lwm2mPostEvent(contextP->ref, OBJ_EXE_IND, coap_get_mid(), (uint32_t)exec_param, 0);
    return COAP_204_CHANGED;
}

int skel_discover_update_resource(uint16_t instanceId, lwm2m_object_t *objectP)
{
    int i = 0;
    ipso_obj_t *ipso_obj;
    int nbRes ;
    ipso_res_t *ipso_res_array;
    skel_instance_t *targetP;
    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;

    if (!lwm2m_isResReady(objectP->objID, contextP->ref))
        return -1;

    ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
    if (ipso_obj == NULL)
        return -1;

    nbRes = ipso_obj->resCount;
    ipso_res_array = ipso_obj->resArray;

    targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (targetP == NULL)
        return -1;

    if (!targetP->isResReady)
    {
        for (i = 0; i < nbRes; i++)
        {
            targetP->dataArray[i].resId = ipso_res_array[i].resId;
            targetP->dataArray[i].type = ipso_res_array[i].value_type;
        }
        targetP->isResReady = true;
    }

    LOG_ARG("update %d resource success", nbRes);

    return 0;
}

int skel_read_update_resource(lwm2m_context_t *contextP, lwm2m_uri_t *uriP, const uint8_t *value, uint32_t valuelen, lwm2m_tpye_t valuetype)
{   
    skel_instance_t *targetP ;
    ipso_res_t *ipso_res ;
    ipso_obj_t *ipso_obj ;
    int i ;
    lwm2m_object_t *skelObj = (lwm2m_object_t *)LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (skelObj == NULL)
        return -1;

    if (!lwm2m_isResReady(skelObj->objID, contextP->ref))
        return -1;

    targetP = (skel_instance_t *)lwm2m_list_find(skelObj->instanceList, uriP->instanceId);
    if (NULL == targetP)
        return -1;

    ipso_res = NULL;
    ipso_obj = lwm2m_get_ipso_obj(skelObj->objID, uriP->resourceId, &ipso_res, contextP->ref);
    if (ipso_obj == NULL || ipso_res == NULL || ipso_obj->resCount == 0)
        return -1;

    ipso_res->value_type = valuetype;

    for ( i = 0; i < ipso_obj->resCount; i++)
    {
        if (targetP->dataArray[i].resId == uriP->resourceId)
        {
            if (valuetype + 3 == LWM2M_TYPE_FLOAT)
                memcpy(&(targetP->dataArray[i].floatValue), value, sizeof(double));

            targetP->dataArray[i].data = lwm2m_malloc(valuelen + 1);
            memcpy(targetP->dataArray[i].data, value, valuelen);
            targetP->dataArray[i].len = valuelen;
            targetP->dataArray[i].type = valuetype;

            LOG_ARG("update %d resource success", uriP->resourceId);
            return 0;
        }
    }

    return -1;
}

#endif

//AT+MIPLWRITE=<ref>, <msgid>, <result><CR>
static int prv_writersp(char *buffer,
                        void *user_data)
{
    int value[1];
    bool result;
    lwm2m_parse_buffer(buffer, value, 1, NULL);
    result = (bool)atoi((const char *)value[0]);
    OSI_LOGI(0x10007beb, "prv_writersp result=%d", result);
    return result;
}

//+MIPLWRITE: <ref>, <msgid>, <objectid>, <instanceid>, <resourceid>, <valuetype>, <len>, <value>, <index><CR>
static int lwm2m_skel_writevalue(lwm2m_context_t *context, write_param_t *param, uint32_t count)
{
    int result = -1;
    command_desc_t skel_commands[] =
        {
            {"write", "write rsp.", NULL, prv_writersp, NULL},
            COMMAND_END_LIST};
    lwm2mPostEvent(context->ref, OBJ_WRITE_IND, coap_get_mid(), (uint32_t)param, count);
    
    
    result = wait_and_process_rsp(context, skel_commands);
    if (result != -1)
        lwm2mPostEvent(context->ref, OBJ_OPERATE_RSP_IND, result, OBJ_WRITE_IND, 0);

    while (param != NULL)
    {
        write_param_t *tmp = param;
        param = param->next;
        lwm2m_free(tmp->data);
        lwm2m_free(tmp);
    }
    return result;
}

static uint8_t prv_skel_write(uint16_t instanceId,
                              int numData,
                              lwm2m_data_t *dataArray,
                              lwm2m_object_t *objectP)
{
    skel_instance_t *targetP;
    uint8_t result = COAP_404_NOT_FOUND;
    ipso_obj_t *ipso_obj;
    int nbRes ;
    write_param_t *writehead ;
    write_param_t *writelist ;
    uint32_t count ;
    int i;
    int j;
    
    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;

    targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    OSI_LOGI(0x10007bec, "prv_skel_write instanceId:%d, numData=%d", instanceId, numData);

    if (targetP == NULL || !lwm2m_isResReady(objectP->objID, contextP->ref))
        return COAP_404_NOT_FOUND;

    ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
        return COAP_404_NOT_FOUND;

    nbRes = ipso_obj->resCount;
    writehead = NULL;
    writelist = NULL;
    count = 0;
    for (i = numData - 1; i >= 0; i--)
    {
        for ( j = 0; j < nbRes; j++)
        {
            if (dataArray[i].id == targetP->dataArray[j].resId)
            {
                int type;
                uint8_t *data;
                int len;
                write_param_t *writeparam ;
                if (lwm2m_is_dynamic_ipso(contextP->ref))
                {
                    type = dataArray[i].type;
                }
                else
                {
                    type = targetP->dataArray[j].type + 3;
                }
                OSI_LOGI(0x10007bed, "prv_skel_write dataArray[%d].id=%d, type=%d", i, dataArray[i].id, type);
                switch (type)
                {
                case LWM2M_TYPE_OPAQUE:
                case LWM2M_TYPE_STRING:
                case LWM2M_TYPE_INTEGER:
                case LWM2M_TYPE_FLOAT:
                case LWM2M_TYPE_BOOLEAN:
                {
                    //uint8_t *data;
                    len = skel_valuedecode(&dataArray[i], &data, type);
                    if (len == -1)
                        break;
                    writeparam = (write_param_t *)lwm2m_malloc(sizeof(write_param_t));
                    writeparam->objid = objectP->objID;
                    writeparam->insid = instanceId;
                    writeparam->resid = dataArray[i].id;
                    writeparam->type = 2;
                    writeparam->data = data;
                    writeparam->len = len;
                    writeparam->next = NULL;
                    if (writelist != NULL)
                        writelist->next = writeparam;
                    writelist = writeparam;
                    if (writehead == NULL)
                        writehead = writelist;
                    OSI_LOGXI(OSI_LOGPAR_IS, 0x10007bee, "prv_skel_write len=%d, data=%s", len, data);
                    count++;
                }
                    result = COAP_204_CHANGED;
                    break;
                default:
                    result = COAP_404_NOT_FOUND;
                }
                break;
            }
        }
    }
    if (writehead != NULL)
    {
        int ret = 0;
        ret = lwm2m_skel_writevalue(contextP, writehead, count);
        if (ret != 1)
            result = COAP_500_INTERNAL_SERVER_ERROR;
        else
            result = COAP_204_CHANGED;
    }
    return result;
}

static uint8_t prv_skel_notify(uint16_t instanceId,
                               int numData,
                               lwm2m_data_t *dataArray,
                               lwm2m_object_t *objectP)
{
    skel_instance_t *targetP;
    ipso_obj_t *ipso_obj;
    int nbRes ;
     uint32_t count;
     int i;
     int j;
    
    uint8_t result = COAP_404_NOT_FOUND;
    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;

    targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    OSI_LOGI(0x10007bef, "prv_skel_notify instanceId:%d,numData=%d,targetP=%x", instanceId, numData, (unsigned int)targetP);

    if (targetP == NULL || !lwm2m_isResReady(objectP->objID, contextP->ref))
        return COAP_404_NOT_FOUND;

    ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
        return COAP_404_NOT_FOUND;

    nbRes = ipso_obj->resCount;
     count = 0;
    if (numData == 1)
    {
        for (j = 0; j < nbRes; j++)
        {
            i = 0;
            if (dataArray[i].id == targetP->dataArray[j].resId)
            {
                uint8_t *data;
                int len;
                if (lwm2m_is_dynamic_ipso(contextP->ref))
                {
                    targetP->dataArray[j].type = dataArray[i].type - 3;
                }
                OSI_LOGI(0x10007bf0, "prv_skel_notify dataArray[%d].id=%d,type=%d", i, dataArray[i].id, targetP->dataArray[j].type);
                switch (targetP->dataArray[j].type + 3)
                {
                case LWM2M_TYPE_OPAQUE:
                case LWM2M_TYPE_STRING:
                case LWM2M_TYPE_INTEGER:
                case LWM2M_TYPE_FLOAT:
                case LWM2M_TYPE_BOOLEAN:
                {
                    //uint8_t *data;
                    len = skel_valuedecode(&dataArray[i], &data, targetP->dataArray[j].type + 3);
                    if (len == -1)
                        break;
                    if (targetP->dataArray[j].type + 3 == LWM2M_TYPE_FLOAT)
                    {
                        targetP->dataArray[j].floatValue = dataArray[i].value.asFloat;
                    }
                    if (targetP->dataArray[j].data != NULL)
                    {
                        lwm2m_free(targetP->dataArray[j].data);
                        targetP->dataArray[j].data = NULL;
                    }
                    targetP->dataArray[j].data = data;
                    targetP->dataArray[j].len = len;
                    targetP->dataArray[j].hasNotify = true;
#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                    targetP->dataArray[j].notifyType = dataArray[i].notify_type;
                    targetP->dataArray[j].seqNum = dataArray[i].seq_num;
                    targetP->dataArray[j].raiMode = dataArray[i].rai_mode;
#endif
                    OSI_LOGXI(OSI_LOGPAR_IS, 0x10007bf1, "prv_skel_notify len=%d, data=%s", len, data);
                    count++;
                }
                    result = COAP_204_CHANGED;
                    break;
                default:
                    result = COAP_404_NOT_FOUND;
                }
                break;
            }
        }
    }
    else
        result = COAP_404_NOT_FOUND;

    return result;
}

//AT+MIPLEXECUTE=<ref>, <msgid>, <result><CR>
static int prv_execrsp(char *buffer,
                       void *user_data)
{
    int value[1];
    bool result;
    lwm2m_parse_buffer(buffer, value, 1, NULL);
    result = (bool)atoi((const char *)value[0]);
    OSI_LOGI(0x10007bf2, "prv_execrsp result=%d", result);
    return result;
}

//+MIPLEXCUTE: <ref>, <msgid>, <objectid>, <instanceid>, <resourceid><CR>
static int lwm2m_skel_exec(lwm2m_context_t *context, exec_param_t *param)
{
    //lwm2mPostEvent(context->ref, OBJ_EXE_IND, coap_get_mid(), (uint32_t)param, 0);
    int result = -1;
    command_desc_t skel_commands[] =
        {
            {"execute", "execute rsp.", NULL, prv_execrsp, NULL},
            COMMAND_END_LIST};
            
    lwm2mPostEvent(context->ref, OBJ_EXE_IND, coap_get_mid(), (uint32_t)param, 0);
    result = wait_and_process_rsp(context, skel_commands);
    if (result != -1)
        lwm2mPostEvent(context->ref, OBJ_OPERATE_RSP_IND, result, OBJ_EXE_IND, 0);
    lwm2m_free(param->buffer);
    lwm2m_free(param);

    if (result >= 0)
    {
        return COAP_204_CHANGED;
    }
    else
    {
        return COAP_405_METHOD_NOT_ALLOWED;
    }
}

static uint8_t prv_skel_execute(uint16_t instanceId,
                                uint16_t resourceId,
                                uint8_t *buffer,
                                int length,
                                lwm2m_object_t *objectP)

{
    skel_instance_t *targetP;
    exec_param_t *exec_param;
    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;
    targetP = (skel_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
        return COAP_404_NOT_FOUND;

    OSI_LOGI(0x10007bf3, "prv_skel_execute instanceId:%d,resourceId=%d", instanceId, resourceId);

    exec_param = lwm2m_malloc(sizeof(exec_param_t));
    exec_param->objid = objectP->objID;
    exec_param->insid = instanceId;
    exec_param->resid = resourceId;
    exec_param->length = length;
    exec_param->buffer = lwm2m_malloc(length + 1);
    memcpy(exec_param->buffer, buffer, length);
    return lwm2m_skel_exec(contextP, exec_param);
}

static uint8_t prv_skel_delete(uint16_t id,
                               lwm2m_object_t *objectP)
{
    skel_instance_t *skelInstance;

    objectP->instanceList = lwm2m_list_remove(objectP->instanceList, id, (lwm2m_list_t **)&skelInstance);
    if (NULL == skelInstance)
        return COAP_404_NOT_FOUND;
    OSI_LOGI(0x10007bf4, "prv_skel_delete id:%d", id);
    if (skelInstance->dataArray != NULL)
        lwm2m_free(skelInstance->dataArray);
    lwm2m_free(skelInstance);

    return COAP_202_DELETED;
}

static uint8_t prv_skel_create(uint16_t instanceId,
                               int numData,
                               lwm2m_data_t *dataArray,
                               lwm2m_object_t *objectP)
{
    skel_instance_t *skelInstance;
    ipso_obj_t *ipso_obj;
    int nbRes;
    ipso_res_t *ipso_res_array;
    uint8_t result = COAP_204_CHANGED;
    lwm2m_context_t *contextP = (lwm2m_context_t *)objectP->userData;

    skelInstance = (skel_instance_t *)lwm2m_malloc(sizeof(skel_instance_t));
    if (NULL == skelInstance)
        return COAP_500_INTERNAL_SERVER_ERROR;
    memset(skelInstance, 0, sizeof(skel_instance_t));

    skelInstance->instanceId = instanceId;
    objectP->instanceList = LWM2M_LIST_ADD(objectP->instanceList, skelInstance);

    if (!lwm2m_isObjSupport(objectP->objID, contextP->ref))
        return COAP_404_NOT_FOUND;

    ipso_obj = lwm2m_get_ipso_obj(objectP->objID, -1, NULL, contextP->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
        return COAP_404_NOT_FOUND;

    nbRes = ipso_obj->resCount;
    ipso_res_array = ipso_obj->resArray;
    
    if (numData != 0 && numData != nbRes)
    {
        OSI_LOGI(0x10007bf5, "prv_skel_create numData(%d) != nbRes(%d)", numData, nbRes);
    }
    skelInstance->dataArray = (skel_res_t *)lwm2m_malloc(nbRes * sizeof(skel_res_t));
    skelInstance->arraySize = nbRes;
    if (lwm2m_isResReady(objectP->objID, contextP->ref))
    {
        int i;
        for ( i = 0; i < nbRes; i++)
        {
            skelInstance->dataArray[i].resId = ipso_res_array[i].resId;
            skelInstance->dataArray[i].type = ipso_res_array[i].value_type;
            skelInstance->dataArray[i].hasNotify = false;
            skelInstance->dataArray[i].hasRead = false;
        }
    }
    if (dataArray != NULL)
    {
        result = prv_skel_write(instanceId, numData, dataArray, objectP);
    }

    if (result != COAP_204_CHANGED)
    {
        (void)prv_skel_delete(instanceId, objectP);
    }
    else
    {
        result = COAP_201_CREATED;
    }
    return result;
}

static void clean_skel_object(lwm2m_object_t *object)
{
    lwm2m_context_t *contextP = (lwm2m_context_t *)object->userData;
    if (contextP != NULL && !lwm2m_is_dynamic_ipso(contextP->ref))
    {
        lwm2m_resetResDiscoverState(object->objID, 0);
    }

    while (object->instanceList != NULL)
    {
        skel_instance_t *skelInstance = (skel_instance_t *)object->instanceList;
        object->instanceList = object->instanceList->next;

        if (skelInstance->dataArray != NULL)
        {
            int size = skelInstance->arraySize;
            int i;
            for ( i = 0; skelInstance->dataArray[i].data != NULL && i < size; i++)
            {
                lwm2m_free(skelInstance->dataArray[i].data);
            }
            lwm2m_free(skelInstance->dataArray);
        }
        lwm2m_free(skelInstance);
    }
    lwm2m_free(object);
}

lwm2m_object_t *get_skel_object(lwm2m_context_t *contextP, uint16_t objid, uint16_t instid, bool *isNewObj)
{
    lwm2m_object_t *skelObj;

    skelObj = (lwm2m_object_t *)LWM2M_LIST_FIND(contextP->objectList, objid);
    if (skelObj == NULL)
    {
        skelObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
        if (NULL != skelObj)
        {
            memset(skelObj, 0, sizeof(lwm2m_object_t));
            skelObj->objID = objid;

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
            if (contextP->serverType == LWM2M_SERVER_ONENET)
            {
                skelObj->readFunc = prv_skel_read_onenet;
                skelObj->writeFunc = prv_skel_write_onenet;
                skelObj->executeFunc = prv_skel_execute_onenet;
                skelObj->discoverFunc = prv_skel_discover_onenet;
            }
            else
            {
#endif
                skelObj->readFunc = prv_skel_read;
                skelObj->writeFunc = prv_skel_write;
                skelObj->executeFunc = prv_skel_execute;
                skelObj->discoverFunc = prv_skel_discover;
#ifdef CONFIG_LWM2M_ONENET_SUPPORT
            }
#endif
            skelObj->createFunc = prv_skel_create;
            skelObj->deleteFunc = prv_skel_delete;
            skelObj->cleanFunc = clean_skel_object;
            skelObj->notifyFunc = prv_skel_notify;
            skelObj->readObjectFunc = prv_skel_read_object;
            skelObj->userData = contextP;

            *isNewObj = true;
        }
    }
    return skelObj;
}

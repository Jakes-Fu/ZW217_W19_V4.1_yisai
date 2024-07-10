/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/

#include <string.h>
#include <stddef.h>
#include "internals.h"
#include "dm_endpoint.h"

#define RES_DM_DEV_INFO                  6601
#define RES_DM_APP_INFO                  6602
#define RES_DM_MAC                       6603
#define RES_DM_ROM                       6604
#define RES_DM_RAM                       6605
#define RES_DM_CPU                       6606
#define RES_DM_SYS_VERSION               6607
#define RES_DM_FIRM_VERSION              6608
#define RES_DM_FIRM_NAME                 6609
#define RES_DM_VOLTE                     6610

#define RES_DM_NET_TYPE                  6611
#define RES_DM_NET_ACCT                  6612
#define RES_DM_PHONE                     6613
#define RES_DM_LOCATION                  6614


#define RES_VALUE_BUFF_LEN  512

typedef struct _dm_device_data
{
    struct _dm_device_data * next;   // matches st_list_t::next
    uint16_t             instanceId;        // matches st_list_t::id

    uint32_t state;
    uint32_t result;

    char dev_info[RES_VALUE_BUFF_LEN];
    char app_info[RES_VALUE_BUFF_LEN];
    char mac[RES_VALUE_BUFF_LEN];
    char rom[RES_VALUE_BUFF_LEN];
    char ram[RES_VALUE_BUFF_LEN];
    char cpu[RES_VALUE_BUFF_LEN];
    char sys_ver[RES_VALUE_BUFF_LEN];
    char firm_ver[RES_VALUE_BUFF_LEN];
    char firm_name[RES_VALUE_BUFF_LEN];
    char volte[RES_VALUE_BUFF_LEN];
    char net_type[RES_VALUE_BUFF_LEN];
    char net_acct[RES_VALUE_BUFF_LEN];
    char phone[RES_VALUE_BUFF_LEN];
    char location[RES_VALUE_BUFF_LEN];
} dm_device_data_t;

static uint8_t read_dm_device_info(int resId, char **outbuff)
{
    char buff[256] = { 0 };
    int  buflen = sizeof(buff);

    *outbuff = NULL;

    switch (resId)
    {
        case RES_DM_DEV_INFO://devinfo
            snprintf(buff,buflen,"%s", "");
            break;
        case RES_DM_APP_INFO://appinfo
            snprintf(buff,buflen,"app_name1|pak_name1|1234000|3\r\napp_name2|pak_name2|2234000|5\r\n");
            break;
        case RES_DM_MAC://mac
            snprintf(buff,buflen,"00:50:56:C0:00:01");
            break;
        case RES_DM_ROM://rom
            snprintf(buff,buflen,"4GB");
            break;
        case RES_DM_RAM://ram
            snprintf(buff,buflen,"2GB");
            break;
        case RES_DM_CPU://CPU
            snprintf(buff,buflen,"ARM");
            break;
        case RES_DM_SYS_VERSION://SYS VERSION
            snprintf(buff,buflen,"LINUX 2.6.18");
            break;
        case RES_DM_FIRM_VERSION://FIRMWARE VERSION
            snprintf(buff,buflen,"1.20.1");
            break;
        case RES_DM_FIRM_NAME://FIRMWARE NAME
            snprintf(buff,buflen,"X-A10");
            break;
        case RES_DM_VOLTE://Volte
            snprintf(buff,buflen,"0");
            break;
        case RES_DM_NET_TYPE://NetType
            snprintf(buff,buflen,"4G");
            break;
        case RES_DM_NET_ACCT://Net Account
            snprintf(buff,buflen,"Account");
            break;
        case 6613://PhoneNumber
            snprintf(buff,buflen,"13812345678");
            break;
        case RES_DM_LOCATION://Location
            snprintf(buff,buflen,"25.77701556036132,123.52958679200002,GPS");
            break;
        default:
            return COAP_404_NOT_FOUND;
    }

    *outbuff = (char*)lwm2m_malloc(strlen(buff)+1);
    memset(*outbuff, 0x00, strlen(buff)+1);
    memcpy(*outbuff, buff, strlen(buff));

    return COAP_205_CONTENT;
}

static uint8_t get_dm_encode_info(uint16_t resId,char **outbuff, char *pwd)
{
    uint8_t result = COAP_205_CONTENT;
    char *szvalbuf = NULL;
    int outlen = 0;
    *outbuff = NULL;

    do
    {
        result = read_dm_device_info(resId, &szvalbuf);
        if (result != COAP_205_CONTENT || szvalbuf == NULL)
        {
            return COAP_404_NOT_FOUND;
        }

        if (strlen((char *)szvalbuf) == 0)
        {
            result = COAP_IGNORE;
            break;
        }

        //data encode
        outlen = dm_read_info_encode(szvalbuf, outbuff, pwd);
        if (outlen < 0)
        {
            result = COAP_500_INTERNAL_SERVER_ERROR;
        }
    }while(0);

    if (szvalbuf)
    {
        lwm2m_free(szvalbuf);
        szvalbuf = NULL;
    }

    LOG("get_dm_encode_info get device info successfully");
    return result;
}

static uint8_t prv_dm_get_value(lwm2m_context_t * contextP,
                                    lwm2m_data_t * dataArrayP,
                                    int number,
                                    dm_device_data_t * devDataP)
{
    uint8_t result = 0;
    uint16_t resId;
    lwm2m_data_t *dataP;
    int i = 0;

    for (i = 0; i < number; i++)
    {
        if (number == 1)
        {
            resId = dataArrayP->id;
            dataP = dataArrayP;
        }
        else
        {
            resId = dataArrayP[i].id;
            dataP = dataArrayP+i;
        }

        switch (resId)
        {
            case RES_DM_DEV_INFO:
            case RES_DM_APP_INFO:
            case RES_DM_MAC:
            case RES_DM_ROM:
            case RES_DM_RAM:
            case RES_DM_CPU:
            case RES_DM_SYS_VERSION:
            case RES_DM_FIRM_VERSION:
            case RES_DM_FIRM_NAME:
            case RES_DM_VOLTE:
            case RES_DM_NET_TYPE:
            case RES_DM_NET_ACCT:
            case RES_DM_PHONE:
            case RES_DM_LOCATION:
            {
                char *outbuff = NULL;
                result = get_dm_encode_info(resId, &outbuff, contextP->dm_pwd);
                if (COAP_205_CONTENT != result && COAP_IGNORE != result)
                {
                    return result;
                }

                if (result == COAP_205_CONTENT)
                {
                    lwm2m_data_encode_string(outbuff, dataP);
                }

                if (outbuff)
                {
                    lwm2m_free(outbuff);
                }

                result = COAP_205_CONTENT;
                break;
            }

            default:
                return COAP_404_NOT_FOUND;
        }
    }
    return result;
}

static uint8_t prv_dm_read(uint16_t instanceId,
                                 int * numDataP,
                                 lwm2m_data_t ** dataArrayP,
                                 lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    dm_device_data_t * data = (dm_device_data_t *)(objectP->userData);;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    // return by dm customized module async
    if(((lwm2m_context_t *)objectP->lwm2mH)->dm_CustomizedRead)
    {
        return COAP_205_CONTENT;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        int nbRes = 0;
        uint16_t resList[] = {
                RES_DM_DEV_INFO,
                RES_DM_APP_INFO,
                RES_DM_MAC,
                RES_DM_ROM,
                RES_DM_RAM,
                RES_DM_CPU,
                RES_DM_SYS_VERSION,
                RES_DM_FIRM_VERSION,
                RES_DM_FIRM_NAME,
                RES_DM_VOLTE,
                RES_DM_NET_TYPE,
                RES_DM_NET_ACCT,
                RES_DM_PHONE,
                RES_DM_LOCATION,
        };

        nbRes = sizeof(resList)/sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL) 
            return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;

        for(i = 0; i < nbRes; i++)
        {
            (*dataArrayP)[i].id = resList[i];
        }

        if (prv_dm_get_value((lwm2m_context_t *)objectP->lwm2mH, (*dataArrayP), nbRes, data) != COAP_205_CONTENT)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
        result = COAP_205_CONTENT;
    }
    else
    {
        result = prv_dm_get_value((lwm2m_context_t *)objectP->lwm2mH, (*dataArrayP), 1, data);
    }

    return result;

}

uint8_t prv_dm_discover(uint16_t instanceId,
                         int * numDataP,
                         lwm2m_data_t ** dataArrayP,
                         lwm2m_object_t * objectP)
{
    uint8_t result;
    int i;

    result = COAP_205_CONTENT;

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        int nbRes = 0;
        uint16_t resList[] = {
            RES_DM_DEV_INFO,
            RES_DM_APP_INFO,
            RES_DM_MAC,
            RES_DM_ROM,
            RES_DM_RAM,
            RES_DM_CPU,
            RES_DM_SYS_VERSION,
            RES_DM_FIRM_VERSION,
            RES_DM_FIRM_NAME,
            RES_DM_VOLTE,
            RES_DM_NET_TYPE,
            RES_DM_NET_ACCT,
            RES_DM_PHONE,
            RES_DM_LOCATION
        };
        nbRes = sizeof(resList) / sizeof(uint16_t);

        if (*dataArrayP == NULL) 
            return COAP_500_INTERNAL_SERVER_ERROR;
        (*dataArrayP)->id = 0;
        (*dataArrayP)->type = LWM2M_TYPE_OBJECT_INSTANCE;
        (*dataArrayP)->value.asChildren.count = nbRes;
        (*dataArrayP)->value.asChildren.array =  lwm2m_data_new(nbRes);
        memset((*dataArrayP)->value.asChildren.array, 0, (nbRes)*sizeof(lwm2m_data_t));
        *numDataP = nbRes;
        for (i = 0; i < nbRes; i++)
        {
            (*dataArrayP)->value.asChildren.array[i].id = resList[i];
            (*dataArrayP)->value.asChildren.array[i].type = LWM2M_TYPE_OBJECT_LINK;
            (*dataArrayP)->value.asChildren.array[i].value.asObjLink.objectId = LWM2M_DEVICE_OBJECT_ID;
            (*dataArrayP)->value.asChildren.array[i].value.asObjLink.objectInstanceId = 0;
        }
    }
    else
    {
        for (i = 0; i < *numDataP && result == COAP_205_CONTENT; i++)
        {
            switch ((*dataArrayP)[i].id)
            {
                case RES_DM_DEV_INFO:
                case RES_DM_APP_INFO:
                case RES_DM_MAC:
                case RES_DM_ROM:
                case RES_DM_RAM:
                case RES_DM_CPU:
                case RES_DM_SYS_VERSION:
                case RES_DM_FIRM_VERSION:
                case RES_DM_FIRM_NAME:
                case RES_DM_VOLTE:
                case RES_DM_NET_TYPE:
                case RES_DM_NET_ACCT:
                case RES_DM_PHONE:
                case RES_DM_LOCATION:
                    break;
                default:
                    result = COAP_404_NOT_FOUND;
            }
        }
    }

    return result;
}

static uint8_t prv_dm_write(uint16_t instanceId,
                            int numData,
                            lwm2m_data_t * dataArray,
                            lwm2m_object_t * objectP)
{
    return COAP_400_BAD_REQUEST;
}

static uint8_t prv_dm_execute(uint16_t instanceId,
                        uint16_t resourceId,
                        uint8_t * buffer,
                        int length,
                        lwm2m_object_t * objectP)
{
    return COAP_400_BAD_REQUEST;
}

static void prv_dm_clean(lwm2m_object_t * objectP)
{
    if (NULL != objectP->userData)
    {
        lwm2m_free(objectP->userData);
        objectP->userData = NULL;
    }
    if (NULL != objectP->instanceList)
    {
        lwm2m_free(objectP->instanceList);
        objectP->instanceList = NULL;
    }
}

lwm2m_object_t * get_object_DM_Onenet(void * lwm2mH)
{
    /*
     * The get_object_firmware function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * OnenetDMObj;

    OnenetDMObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != OnenetDMObj)
    {
        memset(OnenetDMObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns its unique ID
         * The 5 is the standard ID for the optional object "Object firmware".
         */
        OnenetDMObj->objID = LWM2M_DM_OBJECT_ID;
        OnenetDMObj->lwm2mH = lwm2mH;

        /*
         * and its unique instance
         *
         */
        OnenetDMObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != OnenetDMObj->instanceList)
        {
            memset(OnenetDMObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(OnenetDMObj);
            return NULL;
        }

        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        OnenetDMObj->readFunc    = prv_dm_read;
        OnenetDMObj->writeFunc   = prv_dm_write;
        OnenetDMObj->executeFunc = prv_dm_execute;
        OnenetDMObj->cleanFunc = prv_dm_clean;
        OnenetDMObj->userData    = lwm2m_malloc(sizeof(dm_device_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != OnenetDMObj->userData)
        {
            strcpy(((dm_device_data_t *)(OnenetDMObj->userData))->dev_info, "");
            strcpy(((dm_device_data_t *)(OnenetDMObj->userData))->app_info, "2.0");
        }
        else
        {
            lwm2m_free(OnenetDMObj->instanceList);
            lwm2m_free(OnenetDMObj);
            OnenetDMObj = NULL;
        }
    }

    return OnenetDMObj;
}



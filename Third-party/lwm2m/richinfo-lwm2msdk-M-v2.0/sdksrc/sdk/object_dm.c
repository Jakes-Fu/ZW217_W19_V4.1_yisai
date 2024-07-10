/*******************************************************************************
/*
 * This object is single instance only, and provide dm info functionality.
 * Object ID is 666.
 * Object instance ID is 0.
 */

#include "liblwm2m.h"
#include "internals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Resource Id's:
#define LWM2M_DM_UE_OBJECT               666

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

typedef struct
{
    uint8_t state;
    uint8_t result;
    
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
    
} dmread_data_t;

extern int lwm2m_sdk_dm_get_devinfo(uint16_t resId,char **outbuff);



static uint8_t prv_dm_read(uint16_t instanceId,
                                 int * numDataP,
                                 lwm2m_data_t ** dataArrayP,
                                 lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    dmread_data_t * data = (dmread_data_t*)(objectP->userData);
    int total_count;
    int iret;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

     total_count = 14;

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        *dataArrayP = lwm2m_data_new(total_count);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = total_count;

        for(i=0;i<total_count;i++)
        (*dataArrayP)[i].id = 6600+i+1;
    }

    i = 0;
    iret = 0;
    do
    {
    
        if((*dataArrayP)[i].id >=RES_DM_DEV_INFO && (*dataArrayP)[i].id <= RES_DM_LOCATION )
        {
            //char sztmp[512];
            char *outbuff = NULL;
            iret = lwm2m_sdk_dm_get_devinfo((*dataArrayP)[i].id,&outbuff);
            if(iret!=0)
            {
                LOG_ARG("resId=%u,lwm2m_sdk_dm_get_devinfo,iret=%d",(*dataArrayP)[i].id ,iret);
                result = COAP_404_NOT_FOUND;
            }
            else
            {            
                lwm2m_data_encode_string(outbuff, *dataArrayP + i); 
                result = COAP_205_CONTENT;
            }
            if(outbuff) free(outbuff);
        }
        else
            result = COAP_404_NOT_FOUND;

        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

    return result;
}



lwm2m_object_t * get_object_dm(void)
{
    lwm2m_object_t * firmwareObj;

    firmwareObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != firmwareObj)
    {
        memset(firmwareObj, 0, sizeof(lwm2m_object_t));
        firmwareObj->objID = LWM2M_DM_UE_OBJECT;

        /*
         * and its unique instance
         *
         */
        firmwareObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != firmwareObj->instanceList)
        {
            memset(firmwareObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(firmwareObj);
            return NULL;
        }

       
        firmwareObj->readFunc    = prv_dm_read;
        firmwareObj->writeFunc   = 0;
        firmwareObj->executeFunc = 0;
        firmwareObj->userData    = lwm2m_malloc(sizeof(dmread_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != firmwareObj->userData)
        {
            dmread_data_t *data = (dmread_data_t*)(firmwareObj->userData);

            data->state = 1;
            data->result = 0;
            strcpy(data->dev_info, "");
            strcpy(data->app_info, "1.0");

            
        }
        else
        {
            lwm2m_free(firmwareObj);
            firmwareObj = NULL;
        }
    }

    return firmwareObj;
}

void free_object_dm(lwm2m_object_t * objectP)
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
    lwm2m_free(objectP);
}


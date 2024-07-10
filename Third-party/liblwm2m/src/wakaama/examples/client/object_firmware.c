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
 *    Julien Vermillard - initial implementation
 *    Fabien Fleutot - Please refer to git log
 *    David Navarro, Intel Corporation - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    
 *******************************************************************************/

/*
 * This object is single instance only, and provide firmware upgrade functionality.
 * Object ID is 5.
 */

/*
 * resources:
 * 0 package                   write
 * 1 package url               write
 * 2 update                    exec
 * 3 state                     read
 * 4 update supported objects  read/write
 * 5 update result             read
 * 8 - 26600                   for onenet
 */

#include "liblwm2m.h"
#include "lwm2mclient.h"
#include "internals.h"
#include "lwm2m_api.h"
#ifdef CONFIG_LWM2M_FOTA_SUPPORT
#include "fupdate.h"
#include "fupdate_security.h"
#include "vfs.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ---- private object "Firmware" specific defines ----
// Resource Id's:
#define RES_M_PACKAGE                   0
#define RES_M_PACKAGE_URI               1
#define RES_M_UPDATE                    2
#define RES_M_STATE                     3
#define RES_O_UPDATE_SUPPORTED_OBJECTS  4
#define RES_M_UPDATE_RESULT             5
#define RES_O_PKG_NAME                  6
#define RES_O_PKG_VERSION               7

#ifdef CONFIG_LWM2M_ONENET_UPDATE
typedef enum
{
    FOTA_RESULT_INIT,                //0 init
    FOTA_RESULT_SUCCESS,             //1 success
    FOTA_RESULT_NOFREE,              //2 no space
    FOTA_RESULT_OVERFLOW,            //3 downloading overflow
    FOTA_RESULT_DISCONNECT,          //4 downloading disconnect
    FOTA_RESULT_CHECKFAIL,           //5 validate fail
    FOTA_RESULT_NOSUPPORT,           //6 unsupport package
    FOTA_RESULT_URIINVALID,          //7 invalid uri
    FOTA_RESULT_UPDATEFAIL,          //8 update fail
    FOTA_RESULT_PROTOCOLFAIL        //9 unsupport protocol
}cissys_fw_result_type_t;

#define PRV_FIRMWARE_URI  "COAP://NULL"
#define PRV_FIRMWARE_URI_LEN 32
extern uint32_t writeFotaUpgradeData(uint32_t block1_num, uint8_t  block1_more, uint8_t * data, uint16_t datalen);

#endif
lwm2m_fota_state_t g_fota_state;
static lwm2m_fota_result_t s_fota_resulte;
uint8_t *g_download_URI = NULL;
#ifdef CONFIG_LWM2M_ONENET_UPDATE

int LENGTH = 0;
int RESULT = 0;
int STATE = 0;

int32_t reset_fotaIDIL(lwm2m_context_t* context, uint32_t msgid);
bool cissys_readContext(firmware_data_t * context);
bool cissys_resetFwContext();
bool cissys_setFwState(firmware_data_t * context, uint8_t state);
bool cissys_setFwUpdateResult(firmware_data_t * context, uint8_t result);
int cissys_setFwSavedBytes(firmware_data_t * context, uint32_t length);

#ifdef CONFIG_LWM2M_ONENET_SOTA
bool cissys_setSwState(bool ismcu);
#endif
bool cissys_eraseFwFlash (lwm2m_context_t * lwm2mH,firmware_data_t* data);
void fw_validate(lwm2m_context_t * lwm2mH,firmware_data_t* data,int32_t is_validate_success);
bool cissys_checkFwValidation(lwm2m_context_t * lwm2mH,firmware_data_t* data);
uint32_t cissys_writeFwBytes(lwm2m_context_t * lwm2mH, firmware_data_t* data,uint32_t size, uint8_t* buffer);
char * utils_strdup(const char * str);
bool executeFotaUpgrade();
bool cissys_updateFirmware (lwm2m_context_t * lwm2mH, firmware_data_t* data);
#endif

void execute_fota_upgrade();


static uint8_t Is_Coap_URI(uint8_t* uri)
{
    uint8_t ret = 0;

    if ( ( 0 == strncmp((char*)uri, "coaps://", strlen("coaps://"))) || 
         ( 0 == strncmp((char*)uri, "coap://",  strlen("coap://"))) )
    {
        ret = 1;
    }

    return ret;
}

#if 0
static uint8_t * prv_get_uri_path(uint8_t* uri)
{
    //"coap://192.168.188.200:8763/test/text1.file"
    uint8_t * path = (uint8_t *)strchr((const char *)uri, '/');
    if(path == NULL) return path;
    path = path + 2;
    path = (uint8_t *)strchr((const char *)path, '/');
    return path;
}
#endif

static uint8_t prv_firmware_read(uint16_t instanceId,
                                 int * numDataP,
                                 lwm2m_data_t ** dataArrayP,
                                 lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        *dataArrayP = lwm2m_data_new(4);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = 4;
        (*dataArrayP)[0].id = 3;
        (*dataArrayP)[1].id = 4;
        (*dataArrayP)[2].id = 5;
        (*dataArrayP)[3].id = 26500;
    }

    i = 0;
    do
    {
        switch ((*dataArrayP)[i].id)
        {
        case RES_M_PACKAGE:
        case RES_M_PACKAGE_URI:
        case RES_M_UPDATE:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;

        case RES_M_STATE:
            // firmware update state (int)
            #ifdef CONFIG_LWM2M_ONENET_UPDATE
            if(((lwm2m_context_t*)(objectP->lwm2mH))->serverType ==  LWM2M_SERVER_ONENET){
            lwm2m_data_encode_int(STATE, *dataArrayP + i);
            }else
            #endif
                {
            lwm2m_data_encode_int(g_fota_state, *dataArrayP + i);}
            result = COAP_205_CONTENT;
            break;

        case RES_O_UPDATE_SUPPORTED_OBJECTS:
            lwm2m_data_encode_bool(data->supported, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        case RES_M_UPDATE_RESULT:
            #ifdef CONFIG_LWM2M_ONENET_UPDATE
            if(((lwm2m_context_t*)(objectP->lwm2mH))->serverType ==  LWM2M_SERVER_ONENET){
            lwm2m_data_encode_int(RESULT, *dataArrayP + i);
            }else
            #endif
                {
            lwm2m_data_encode_int(s_fota_resulte, *dataArrayP + i);}
            result = COAP_205_CONTENT;
            break;
#ifdef CONFIG_LWM2M_ONENET_UPDATE
        case RES_M_TRANSGERRED_BYTES:
            lwm2m_data_encode_int(cissys_getFwSavedBytes(), *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;
#endif
        default:
            result = COAP_404_NOT_FOUND;
        }

        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

    return result;
}

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
static uint8_t prv_firmware_discover(uint16_t instanceId,
                                 int * numDataP,
                                 lwm2m_data_t ** dataArrayP,
                                 lwm2m_object_t * objectP)
{
  uint8_t result;
  int i;

  // this is a single instance object
  if (instanceId != 0)
  {
      return COAP_404_NOT_FOUND;
  }

  result = COAP_205_CONTENT;

  // is the server asking for the full object ?
  if (*numDataP == 0)
  {
      uint16_t resList[] = {
            RES_M_PACKAGE,
            RES_M_PACKAGE_URI,
            RES_M_UPDATE,
            RES_M_STATE,
            RES_M_UPDATE_RESULT,
            RES_M_SUPPORT_PORTOCOL,
            RES_M_DELIVER_METHOD,
            RES_M_TRANSGERRED_BYTES,
            RES_M_SWITCH_TO_DOWNLOAD,
            RES_M_SWITCH_MCU_UPDATE,
            RES_M_FOTA_TRIGGER
      };
      int nbRes = sizeof(resList) / sizeof(uint16_t);

      *dataArrayP = lwm2m_data_new(nbRes);
      if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
      *numDataP = nbRes;
      for (i = 0; i < nbRes; i++)
      {
          (*dataArrayP)[i].id = resList[i];
      }
  }
  else
  {
      for (i = 0; i < *numDataP && result == COAP_205_CONTENT; i++)
      {
          switch ((*dataArrayP)[i].id)
          {
          case RES_M_PACKAGE:
          case RES_M_PACKAGE_URI:
          case RES_M_UPDATE:
          case RES_M_STATE:
          case RES_M_UPDATE_RESULT:
          case RES_M_SUPPORT_PORTOCOL:
          case RES_M_DELIVER_METHOD:
          case RES_M_TRANSGERRED_BYTES:
          case RES_M_SWITCH_TO_DOWNLOAD:
          case RES_M_SWITCH_MCU_UPDATE:
          case RES_M_FOTA_TRIGGER:
              break;
          default:
              result = COAP_404_NOT_FOUND;
          }
      }
  }

  return result;
}
#endif

#ifdef CONFIG_LWM2M_ONENET_UPDATE

int32_t reset_fotaIDIL(lwm2m_context_t* context, uint32_t msgid)
{
#ifdef CONFIG_LWM2M_ONENET_SOTA
    if(context->isupdate_mcu == false)
#endif
    {
        lwm2mPostEvent(context->ref,EVETN_IND,msgid, 0, 0);
    }
    return 0;
}

bool cissys_readContext(firmware_data_t * context)
{
    #if 0   //xiameng.yang
    int len = vfs_file_read("/cis_fw_context", context, sizeof(firmware_data_t));
    if (len == -1)
    {
        context->result = RESULT;
        context->savebytes = LENGTH;
        context->state = STATE;
        vfs_file_write("/cis_fw_context", context, sizeof(firmware_data_t));
        //printf("cissys_readContext create file cis_fw_context success");
        return true;
    }

    LOG_ARG("cissys_readContext result:%d , state:%d, savebytes:%d, ", context->result,context->state,context->savebytes);

    RESULT = context->result;
    LENGTH = context->savebytes;
    STATE = context->state;
    #endif
    return true;
}

bool cissys_resetFwContext()
{
    #if 0   //xiameng.yang
    int ret;
    LOG("cissys_resetFwContext");
    RESULT = 0;
    LENGTH = 0;
    STATE = 0;
    ret = vfs_unlink("/cis_fw_context");
    if (ret == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
    #endif
}

int cissys_getFwSavedBytes()
{
    return LENGTH;
}

bool cissys_setFwState(firmware_data_t * context, uint8_t state)
{
    #if 0  //xiameng.yang
    STATE=state;
    context->state = STATE;
    vfs_file_write("/cis_fw_context", context, sizeof(firmware_data_t));
    #endif
    return true;
}

bool cissys_setFwUpdateResult(firmware_data_t * context, uint8_t result)
{
    #if 0  //xiameng.yang
    RESULT=result;
    context->result = RESULT;
    vfs_file_write("/cis_fw_context", context, sizeof(firmware_data_t));
    #endif
    return true;
}

int cissys_setFwSavedBytes(firmware_data_t * context, uint32_t length)
{
    #if 0 //xiameng.yang
    LOG_ARG("cissys_setFwSavedBytes,len =%d",length);
    LENGTH=length;
    context->savebytes = LENGTH;
    vfs_file_write("/cis_fw_context", context, sizeof(firmware_data_t));
    #endif
    return 1;
}

#ifdef CONFIG_LWM2M_ONENET_SOTA
bool isupdatemcu = false;
char SOTA_VERSION[17]="0.0.0";
uint32_t MCU_SotaSize=100;
bool cissys_setSwState(bool ismcu)
{
    isupdatemcu = ismcu;
    return true;
}
bool cissys_getSwState(void)
{
    return isupdatemcu;
}

void cissys_setSotaMemory(uint32_t size)
{
    MCU_SotaSize=size;
}

uint32_t cissys_getSotaMemory(void)
{
    return MCU_SotaSize;
}

uint32_t cissys_getSotaVersion(char** version)
{
    int length = strlen(SOTA_VERSION);
    strncpy((char *)*version,SOTA_VERSION,length);
    return length;
}

bool cissys_setSotaVersion(char* version)
{
    int length  = strlen(version);
    if(length > 16)
    {
        return false;
    }
    memset(SOTA_VERSION,0,sizeof(SOTA_VERSION));
    strncpy(SOTA_VERSION,version,length);
    return true;
}

uint32_t cissys_writeSwBytes(firmware_data_t* data,uint32_t size)
{
    #if 0
    LENGTH+=size;
    data->savebytes = LENGTH;
    LOG_ARG("cissys_writeFwBytes,length=%d",LENGTH);
    vfs_file_write("/cis_fw_context", data, sizeof(firmware_data_t));
    #endif
    return 0;
}
#endif

int cissys_EventHandler(cissys_event_t id, lwm2m_context_t * lwm2mH, firmware_data_t* data)
{
#ifdef CONFIG_LWM2M_ONENET_SOTA
    if(lwm2mH->isupdate_mcu==true)
    {
        LOG_ARG("need_async_ack=%d",lwm2mH->fw_request.need_async_ack);
        /*whether need async response*/
        if(lwm2mH->fw_request.need_async_ack)
        {
            lwm2mH->fw_request.need_async_ack = false;
            lwm2mPostEvent(lwm2mH->ref, SOTA_RSP_IND, 1, 0, 0);
        }
        else
        {
            lwm2mPostEvent(lwm2mH->ref, SOTA_RSP_IND, 0, 0, 0);
            return 0;
        }
    }
#endif 
    switch (id)
    {
        case cissys_event_write_success:
        {
            lwm2mH->fw_request.write_state = PACKAGE_WRITE_IDIL;
            lwm2mH->fw_request.code = COAP_204_CHANGED;
            //single write,wait for the validate result
            LOG_ARG("single_packet_write=%d",lwm2mH->fw_request.single_packet_write);
            if (lwm2mH->fw_request.single_packet_write == true)
            {
#ifdef CONFIG_LWM2M_ONENET_SOTA
                if(lwm2mH->isupdate_mcu==true)
                {
                    cissys_writeSwBytes(data, lwm2mH->fw_request.last_packet_size);
                    lwm2mH->fw_request.code = COAP_204_CHANGED;
                    lwm2mH->fw_request.single_packet_write = false;
                    object_asynAckNodata(lwm2mH,lwm2mH->fw_request.response_ack,COAP_204_CHANGED);
                    cissys_setFwState(data, FOTA_STATE_DOWNLOADED);
                    lwm2mPostEvent(lwm2mH->ref,EVETN_IND,LWM2M_EVENT_SOTA_DOWNLOAED, 0, 0);
                }
                else 
#endif
                {
                    if (!cissys_checkFwValidation(lwm2mH,data))
                    {
                        //set validate result
                        cissys_setFwUpdateResult(data, FOTA_RESULT_CHECKFAIL);
                        lwm2mH->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
                        object_asynAckNodata(lwm2mH,lwm2mH->fw_request.response_ack,COAP_500_INTERNAL_SERVER_ERROR);
                        reset_fotaIDIL(lwm2mH,LWM2M_EVENT_FIRMWARE_DOWNLOAD_FAILED);
                    }
                    else
                    {
                        cissys_setFwState(data, FOTA_STATE_DOWNLOADED);
                    }
                }
                //wait for validation result to send ack
                lwm2mH->fw_request.code = COAP_204_CHANGED;
                return 0;
            }
            LOG_ARG("block1_more=%d",lwm2mH->fw_request.block1_more);
            //block write result
            if (lwm2mH->fw_request.block1_more == 0) //the last block for write
            {
                cissys_setFwState(data, FOTA_STATE_DOWNLOADED);
#ifdef CONFIG_LWM2M_ONENET_SOTA
                if(lwm2mH->isupdate_mcu==true)
                {
                    cissys_writeSwBytes(data, lwm2mH->fw_request.last_packet_size);
                    lwm2mH->fw_request.code = COAP_204_CHANGED;
                    object_asynAckBlockWrite(lwm2mH,lwm2mH->fw_request.response_ack,COAP_204_CHANGED,lwm2mH->fw_request.ack_type);
                    cissys_setFwState(data, FOTA_STATE_DOWNLOADED);
                    lwm2mPostEvent(lwm2mH->ref,EVETN_IND,LWM2M_EVENT_SOTA_DOWNLOAED, 0, 0);
                }
                else 
#endif
                {
                    if (!cissys_checkFwValidation(lwm2mH,data))
                    {
                        cissys_setFwUpdateResult(data, FOTA_RESULT_CHECKFAIL);
                        lwm2mH->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
                        object_asynAckBlockWrite(lwm2mH,lwm2mH->fw_request.response_ack,COAP_500_INTERNAL_SERVER_ERROR,lwm2mH->fw_request.ack_type);
                        reset_fotaIDIL(lwm2mH,LWM2M_EVENT_FIRMWARE_DOWNLOAD_FAILED);
                    }
                }
            }
            else//not the last write
            {
#ifdef CONFIG_LWM2M_ONENET_SOTA
                if(lwm2mH->isupdate_mcu == true)
                    cissys_writeSwBytes(data, lwm2mH->fw_request.last_packet_size);
#endif
                lwm2mH->fw_request.code = COAP_231_CONTINUE;
                object_asynAckBlockWrite(lwm2mH, lwm2mH->fw_request.response_ack, COAP_231_CONTINUE, lwm2mH->fw_request.ack_type);
            }
        }
        break;
        case cissys_event_write_fail:
        {
            if (lwm2mH->fw_request.single_packet_write == true)   //single write
            {
                lwm2mH->fw_request.single_packet_write = false;
                lwm2mH->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
                object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_500_INTERNAL_SERVER_ERROR);
            }
            else
            {
                lwm2mH->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
                object_asynAckBlockWrite(lwm2mH, lwm2mH->fw_request.response_ack, COAP_500_INTERNAL_SERVER_ERROR, lwm2mH->fw_request.ack_type);
            }

#ifdef CONFIG_LWM2M_ONENET_SOTA
            if(lwm2mH->isupdate_mcu==true)
            { 
                lwm2mH->fw_request.write_state = PACKAGE_WRITE_IDIL;
                cissys_setFwState(data, FOTA_STATE_IDIL);
                cissys_setFwSavedBytes(data, 0);
                cissys_setFwUpdateResult(data, FOTA_RESULT_INIT );
            }
            else
#endif
            {
                data->result = FOTA_RESULT_CHECKFAIL;
                cissys_setFwUpdateResult(data, data->result);
                lwm2mH->fw_request.write_state = PACKAGE_WRITE_IDIL;
                reset_fotaIDIL(lwm2mH, LWM2M_EVENT_FIRMWARE_DOWNLOAD_FAILED);
            }
        }
        break;
        case cissys_event_fw_erase_success:
        {
            lwm2mH->fw_request.write_state = PACKAGE_WRITE_IDIL;
            cissys_setFwState(data, FOTA_STATE_IDIL);
            cissys_setFwSavedBytes(data, 0);
            cissys_setFwUpdateResult(data,FOTA_RESULT_INIT );
            object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_204_CHANGED);
            reset_fotaIDIL(lwm2mH, LWM2M_EVENT_FIRMWARE_ERASE_SUCCESS);
#ifdef CONFIG_LWM2M_ONENET_SOTA
            if(lwm2mH->isupdate_mcu == true)
            {
                lwm2mH->isupdate_mcu=false;
                cissys_setSwState(lwm2mH->isupdate_mcu);	
            }
#endif
        }
        break;
        case cissys_event_fw_erase_fail:
        {
#ifdef CONFIG_LWM2M_ONENET_SOTA
            if(true==lwm2mH->isupdate_mcu)
            {
                lwm2mH->fw_request.write_state = PACKAGE_WRITE_IDIL;
                cissys_setFwState(data, FOTA_STATE_IDIL);
                cissys_setFwSavedBytes(data, 0);
                cissys_setFwUpdateResult(data,FOTA_RESULT_INIT );
            }
            else
#endif
            {
                lwm2mH->fw_request.write_state = PACKAGE_WRITE_FAIL;
                data->result = FOTA_RESULT_UPDATEFAIL;
                cissys_setFwUpdateResult(data, data->result);
                lwm2mH->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
            }
            object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_500_INTERNAL_SERVER_ERROR);
            reset_fotaIDIL(lwm2mH, LWM2M_EVENT_FIRMWARE_ERASE_FAIL);
        }
        break;
        case cissys_event_fw_validate_success:
        {
            LOG("cissys_event_fw_validate_success");
            cissys_setFwState(data, FOTA_STATE_DOWNLOADED);
            lwm2mPostEvent(lwm2mH->ref,EVETN_IND,LWM2M_EVENT_FIRMWARE_DOWNLOADED, 0, 0);
            lwm2mH->fw_request.code = COAP_204_CHANGED;
            if (lwm2mH->fw_request.single_packet_write == true)   //single write validate
            {
                lwm2mH->fw_request.single_packet_write = false;
                object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_204_CHANGED);
            }
            else
            {
                object_asynAckBlockWrite(lwm2mH, lwm2mH->fw_request.response_ack,COAP_204_CHANGED, lwm2mH->fw_request.ack_type);
            }
        }
        break;
        case cissys_event_fw_validate_fail:
        {
            cissys_setFwUpdateResult(data, FOTA_RESULT_CHECKFAIL);
            lwm2mH->fw_request.write_state = PACKAGE_WRITE_FAIL;
            lwm2mH->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
            if (lwm2mH->fw_request.single_packet_write == true)   //single write validate
            {
                lwm2mH->fw_request.single_packet_write = false;
                object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_500_INTERNAL_SERVER_ERROR);
            }
            else
            {
                object_asynAckBlockWrite(lwm2mH, lwm2mH->fw_request.response_ack, COAP_500_INTERNAL_SERVER_ERROR, lwm2mH->fw_request.ack_type);
            }
            reset_fotaIDIL(lwm2mH, LWM2M_EVENT_FIRMWARE_DOWNLOAD_FAILED);
        }
        break;
        case cissys_event_fw_update_fail:
        {
#ifdef CONFIG_LWM2M_ONENET_SOTA
            if(lwm2mH->isupdate_mcu == true)
            {
                lwm2mH->fw_request.write_state = PACKAGE_WRITE_IDIL;
                cissys_setFwState(data, FOTA_STATE_IDIL);
                cissys_setFwSavedBytes(data, 0);
                cissys_setFwUpdateResult(data,FOTA_RESULT_INIT );
                object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_400_BAD_REQUEST);
            }
            else
#endif
            {
                data->result = FOTA_RESULT_UPDATEFAIL;
                cissys_setFwUpdateResult(data, data->result);
            }
                reset_fotaIDIL(lwm2mH, LWM2M_EVENT_FIRMWARE_UPDATE_FAILED);
        }
        break;
        default:
            break;
    }
    return 0;
}

bool cissys_eraseFwFlash (lwm2m_context_t * lwm2mH,firmware_data_t* data)
{
    #if 0
    //eraseCallback = 1;
    LOG("cissys_eraseFwFlash");
    fupdateInvalidate(true);
    if(0 != cissys_EventHandler(cissys_event_fw_erase_success, lwm2mH, data))
        return false;
    #endif
    return true;
}

void fw_validate(lwm2m_context_t * lwm2mH,firmware_data_t* data,int32_t is_validate_success)
{
    if(is_validate_success == 1)
    {
        cissys_EventHandler(cissys_event_fw_validate_success,lwm2mH, data);
    }
    else
    {
        cissys_EventHandler(cissys_event_fw_validate_fail,lwm2mH, data);
    }
}

bool cissys_checkFwValidation(lwm2m_context_t * lwm2mH,firmware_data_t* data)
{
    #if 0
    if (fupdateCheckSignedPack() && fupdateIsTmpSpaceEnough() && fupdateSetReady(NULL)
#ifdef CONFIG_FUPDATE_SUPPORT_VERSION_CHECK
    &&fupdateIsVersionUpgrade()
#endif
    )
    {
        fw_validate(lwm2mH,data,1);
    }
    else
    {
        LOG("fupdateSetReady failed");
        fw_validate(lwm2mH,data,0);
    }
    #endif
    return true;
}

uint32_t cissys_writeFwBytes(lwm2m_context_t * lwm2mH, firmware_data_t* data,uint32_t size, uint8_t* buffer)
{
    #if 0  //xiameg.yang
    LENGTH+=size;
    data->savebytes = LENGTH;
    LOG_ARG("cissys_writeFwBytes,length=%d",LENGTH);
    vfs_file_write("/cis_fw_context", data, sizeof(firmware_data_t));
    osiThreadSleep(100);
    LOG("writed package ok");
    if(0 != cissys_EventHandler(cissys_event_write_success, lwm2mH, data))
        return -1;
    #endif
    return 0;
}

char * utils_strdup(const char * str)
{
    size_t length = strlen(str);
    char* result = (char *)lwm2m_malloc( length +1);
    if( result == NULL)
    {
        return NULL;
    }
    memcpy(result, str, length);
    result[length] = 0;
    return result;
}

uint8_t *utils_bufferdup(const uint8_t * src,size_t len)
{
    uint8_t* buffer = NULL;
    buffer = (uint8_t*)lwm2m_malloc(len);
    if(buffer == NULL)
    {
        LOG("utils_bufferdup,buffer == null");
        return NULL;
    }
    memcpy(buffer, src, len);
    return buffer;
}

bool executeFotaUpgrade()
{
    #if 0
    LOG("cis_sdk executeFotaUpgrade");
    if (fupdateCheckSignedPack() && fupdateIsTmpSpaceEnough() && fupdateSetReady(NULL)
#ifdef CONFIG_FUPDATE_SUPPORT_VERSION_CHECK
    &&fupdateIsVersionUpgrade()
#endif
    )
    {
        osiShutdown(OSI_SHUTDOWN_RESET);
        return true;
    }
    else
    {
        LOG("executeFotaUpgrade failed");
        fupdateInvalidate(true);
        return false;
    }
    #endif
}

bool cissys_updateFirmware (lwm2m_context_t * lwm2mH, firmware_data_t* data)
{
    //osiThreadSleep(3000);
    SCI_Sleep(3000);
    cissys_setFwUpdateResult(data, FOTA_RESULT_SUCCESS);
    if (!executeFotaUpgrade())
    {
        lwm2mPostEvent(lwm2mH->ref,EVETN_IND,LWM2M_EVENT_FIRMWARE_UPDATE_FAILED, 0, 0);//core_callbackEvent(context,CIS_EVENT_FIRMWARE_UPDATE_FAILED,NULL);
        cissys_setFwUpdateResult(data, FOTA_RESULT_UPDATEFAIL);
        lwm2mH->fw_request.code = COAP_204_CHANGED;
        object_asynAckBlockWrite(lwm2mH,lwm2mH->fw_request.response_ack,COAP_204_CHANGED,lwm2mH->fw_request.ack_type);
        reset_fotaIDIL(lwm2mH,LWM2M_EVENT_FIRMWARE_UPDATE_FAILED);
    }
    return true;
}

#endif
static uint8_t prv_firmware_write(uint16_t instanceId,
                                  int numData,
                                  lwm2m_data_t * dataArray,
                                  lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);
    lwm2m_context_t * lwm2mH = (lwm2m_context_t*)(objectP->lwm2mH);
    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    i = 0;

    do
    {
        lwm2m_data_t * st_data = NULL;
#ifdef CONFIG_LWM2M_ONENET_UPDATE
        if(lwm2mH->serverType == LWM2M_SERVER_ONENET)
        {
            st_data = &dataArray->value.asChildren.array[i];
        }
        else
#endif
        {
            st_data = &dataArray[i];
        }
        LOG_ARG("st_data.id: %d", st_data->id);
        switch (st_data->id)
        {
        case RES_M_PACKAGE:
#ifdef CONFIG_LWM2M_ONENET_UPDATE

        if(lwm2mH->serverType == LWM2M_SERVER_ONENET){
#ifdef CONFIG_LWM2M_ONENET_SOTA
            if (lwm2mH->isupdate_mcu == true) //sota
            {
                if (st_data->value.asBuffer.length == 0)
                {
                    lwm2mH->fw_request.single_packet_write = false;
                    lwm2mH->fw_request.need_async_ack = true;
                    lwm2mPostEvent(lwm2mH->ref,EVETN_IND,LWM2M_EVENT_SOTA_FLASHERASE, 0, 0);
                    return COAP_206_CONFORM;
                }
                else
                {
                    if (data->state == FOTA_STATE_DOWNLOADING)
                    {
                        lwm2mH->fw_request.single_packet_write = true;
                        lwm2mH->fw_request.need_async_ack = true;
                        lwm2mH->fw_request.write_state = PACKAGE_WRITING;
                        lwm2mH->fw_request.last_packet_size = st_data->value.asBuffer.length;

                        write_param_t *writeparam = (write_param_t *)lwm2m_malloc(sizeof(write_param_t));
                        if (writeparam == NULL)
                        {
                            return COAP_500_INTERNAL_SERVER_ERROR;
                        }
                        writeparam->objid = LWM2M_FIRMWARE_UPDATE_OBJECT_ID;
                        writeparam->insid = 0;
                        writeparam->resid = RES_M_PACKAGE;
                        writeparam->type = (lwm2m_tpye_t)(st_data->type -3);
                        writeparam->data = utils_bufferdup(st_data->value.asBuffer.buffer, st_data->value.asBuffer.length);
                        writeparam->len = st_data->value.asBuffer.length;
                        writeparam->next = NULL;
                        lwm2mPostEvent(lwm2mH->ref, OBJ_WRITE_IND, coap_get_mid(), (uint32_t)writeparam, 1);
                        return COAP_206_CONFORM;
                    }
                    else
                    {
                        return COAP_500_INTERNAL_SERVER_ERROR;
                    }
                }
                result = COAP_204_CHANGED;
            }
            else // fota
#endif            
            {
                LOG_ARG("prv_firmware_write length =%d",st_data->value.asBuffer.length);
                if (st_data->value.asBuffer.length == 0)//restart a new update task
                {
                    lwm2mH->fw_request.single_packet_write = false;
                    lwm2mH->fw_request.need_async_ack = true;
                    lwm2mH->fw_request.write_state = PACKAGE_WRITING;
                    if (!cissys_eraseFwFlash(lwm2mH,data))
                    {
                        object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_500_INTERNAL_SERVER_ERROR);
                        reset_fotaIDIL(lwm2mH, COAP_500_INTERNAL_SERVER_ERROR);
                        return COAP_500_INTERNAL_SERVER_ERROR;
                    }
                }
                else
                {
                    if (data->state == FOTA_STATE_DOWNLOADING)
                    {
                        uint32_t result;
                        lwm2mH->fw_request.single_packet_write = true;
                        lwm2mH->fw_request.need_async_ack = true;
                        lwm2mH->fw_request.write_state = PACKAGE_WRITING;
                        lwm2mH->fw_request.last_packet_size = st_data->value.asBuffer.length;

                        result = writeFotaUpgradeData(0, 0, st_data->value.asBuffer.buffer, st_data->value.asBuffer.length);
                        LOG_ARG("writeFotaUpgradeData return :%d",result);
                        if (COAP_NO_ERROR != result)
                        {
                            object_asynAckBlockWrite(lwm2mH,lwm2mH->fw_request.response_ack,COAP_408_REQ_ENTITY_INCOMPLETE,lwm2mH->fw_request.ack_type);
                            reset_fotaIDIL(lwm2mH,COAP_408_REQ_ENTITY_INCOMPLETE);
                            return COAP_408_REQ_ENTITY_INCOMPLETE;
                        }
                        if (0 != cissys_writeFwBytes(lwm2mH,data,st_data->value.asBuffer.length, st_data->value.asBuffer.buffer))
                        {
                            object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_500_INTERNAL_SERVER_ERROR);
                            reset_fotaIDIL(lwm2mH, COAP_500_INTERNAL_SERVER_ERROR);
                            return COAP_500_INTERNAL_SERVER_ERROR;
                        }
                    }
                    else
                    {
                        return COAP_500_INTERNAL_SERVER_ERROR;
                    }
                }
                result = COAP_204_CHANGED;
            }
    }else
#endif//CONFIG_LWM2M_ONENET_UPDATE
    {
        // inline firmware binary
        result = COAP_204_CHANGED;
    }
            break;
        case RES_M_PACKAGE_URI:
        {
            // URL for download the firmware
            lwm2m_data_t * data = &dataArray[i];
            int length = data->value.asBuffer.length;
            g_download_URI = malloc(length+1);
            if(NULL == g_download_URI)
            {
                 notify_fota_state(LWM2M_FOTA_STATE_IDLE, LWM2M_FOTA_RESULT_OUT_OF_RAM, lwm2mH->ref);
                 result = COAP_204_CHANGED;
            }
            else
            {
                memcpy(g_download_URI, data->value.asBuffer.buffer, length);
                g_download_URI[length] = 0;
                LOG_ARG("get RES_M_PACKAGE_URI %d downloadURI %s",length,g_download_URI);
                //uint8_t * path = prv_get_uri_path(downloadURI);
                /* Only Support Coap URI.*/
                if(Is_Coap_URI(g_download_URI))
                {
                    lwm2mH->fota_context.block2Num = 0;
                    lwm2mH->fota_context.block2bufferSize = 0;
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
                    lwm2mPostEvent(lwm2mH->ref, FOTA_DOWNLOAD_REQ_IND, 0, 0, 0);
                    if(lwm2mH->fota_context.fotaMode == LWM2M_AUTOMATIC_FOTA)
#endif
                    {
                        notify_fota_state(LWM2M_FOTA_STATE_DOWNLOADING, LWM2M_FOTA_RESULT_INIT, lwm2mH->ref);
                        lwm2m_start_fota_download(g_download_URI, lwm2mH->ref);
                        if(NULL != g_download_URI)
                        {
                            free(g_download_URI);
                            g_download_URI = NULL;
                        }
                    }
                }
                else
                {
                    notify_fota_state(LWM2M_FOTA_STATE_IDLE, LWM2M_FOTA_RESULT_INVALID_URI, lwm2mH->ref);
                    if(NULL != g_download_URI)
                    {
                        free(g_download_URI);
                        g_download_URI = NULL;
                    }
                }
                result = COAP_204_CHANGED;
            }
        }
            break;

        case RES_O_UPDATE_SUPPORTED_OBJECTS:
            if (lwm2m_data_decode_bool(&dataArray[i], &data->supported) == 1)
            {
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;

        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
        }

        i++;
    } while (i < numData && result == COAP_204_CHANGED);

    return result;
}

static uint8_t prv_firmware_execute(uint16_t instanceId,
                                    uint16_t resourceId,
                                    uint8_t * buffer,
                                    int length,
                                    lwm2m_object_t * objectP)
{
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);
    lwm2m_context_t * lwm2mH = (lwm2m_context_t*)(objectP->lwm2mH);
    LOG_ARG("std_firmware_execute instanceId=%u,resourceId:%d,length=%d", instanceId,resourceId,length);
    LOG_ARG("fota state:%d,fota result=%d, serverType=%d", data->state,data->result,lwm2mH->serverType);
    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }
#ifdef CONFIG_LWM2M_ONENET_SOTA
    else if (resourceId == RES_M_SWITCH_MCU_UPDATE)
    {
        OSI_LOGI(0x10009862, "Execute MCU_UPDATE\r\n");
        lwm2mH->isupdate_mcu = true;
        cissys_setSwState(lwm2mH->isupdate_mcu);
        return COAP_204_CHANGED;
    }
#endif

    if (length != 0) return COAP_400_BAD_REQUEST;
    // for execute callback, resId is always set.
    switch (resourceId)
    {
    case RES_M_UPDATE:
#ifdef CONFIG_LWM2M_ONENET_UPDATE
        if(lwm2mH->serverType == LWM2M_SERVER_ONENET)
        {
            if (data->state == FOTA_STATE_DOWNLOADED)
            {
                if (data->result == FOTA_RESULT_INIT
                || data->result == FOTA_RESULT_OVERFLOW
                || data->result == FOTA_RESULT_DISCONNECT
                || data->result == FOTA_RESULT_UPDATEFAIL)//bad request
                {
                    LOG("Execute updating");
                    // trigger your firmware download and update logic
                    cissys_setFwState(data, FOTA_STATE_UPDATING);
                    object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_204_CHANGED);
#ifdef CONFIG_LWM2M_ONENET_SOTA
                    if (lwm2mH->isupdate_mcu == true) //sota
                    {
                        lwm2mPostEvent(lwm2mH->ref,EVETN_IND, LWM2M_EVENT_SOTA_UPDATING, 0, 0);
                    }
                    else //fota
#endif                    
                    {
                        lwm2mPostEvent(lwm2mH->ref,EVETN_IND, LWM2M_EVENT_FIRMWARE_UPDATING, 0, 0);
                        if (!cissys_updateFirmware(lwm2mH,data))
                            return COAP_500_INTERNAL_SERVER_ERROR;
                    }
                    return COAP_IGNORE;
                }
                else
                {
                    return COAP_400_BAD_REQUEST;
                }
            }

            else if (data->state == FOTA_STATE_UPDATING)
            {

                if (data->result == FOTA_RESULT_INIT
                || data->result == FOTA_RESULT_SUCCESS
                || data->result == FOTA_RESULT_NOFREE
                || data->result == FOTA_RESULT_OVERFLOW
                || data->result == FOTA_RESULT_DISCONNECT
                || data->result == FOTA_RESULT_UPDATEFAIL)//bad request
                {
                    LOG("Execute updating again");
                    object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_204_CHANGED);
#ifdef CONFIG_LWM2M_ONENET_SOTA
                    if (lwm2mH->isupdate_mcu == true) // sota
                    {
                        lwm2mPostEvent(lwm2mH->ref, EVETN_IND, LWM2M_EVENT_FIRMWARE_UPDATING, 0, 0);                    
                    }
                    else // fota
#endif                   
                    {
                        lwm2mPostEvent(lwm2mH->ref, EVETN_IND, LWM2M_EVENT_FIRMWARE_UPDATING, 0, 0);//core_callbackEvent(lwm2mH, CIS_EVENT_FIRMWARE_UPDATING, NULL);
                        if (!cissys_updateFirmware(lwm2mH,data))
                            return COAP_500_INTERNAL_SERVER_ERROR;
                    }
                    return COAP_IGNORE;
                }
                else
                {
                    return COAP_400_BAD_REQUEST;
                }
            }
            else
            {
                // firmware update already running
                return COAP_400_BAD_REQUEST;
            }
        }else
#endif//CONFIG_LWM2M_ONENET_UPDATE
        {
            if (data->state == 1)
            {
                fprintf(stdout, "\n\t FIRMWARE UPDATE\r\n\n");
                // trigger your firmware download and update logic
                LOG("FIRMWARE UPDATE \r\n");
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
                lwm2mPostEvent(lwm2mH->ref, FOTA_UPGRADE_REQ_IND, 0, 0, 0);
                if(lwm2mH->fota_context.fotaMode == LWM2M_AUTOMATIC_FOTA)
#endif
                {
                    notify_fota_state(LWM2M_FOTA_STATE_UPDATING, LWM2M_FOTA_RESULT_SUCCESS, lwm2mH->ref);
                    execute_fota_upgrade();
                }
                data->state = 2;
                return COAP_204_CHANGED;
            }
            else
            {
                // firmware update already running
                return COAP_400_BAD_REQUEST;
            }
        }
#ifdef CONFIG_LWM2M_ONENET_UPDATE
        case RES_M_SWITCH_TO_DOWNLOAD:
            if(lwm2mH->serverType == LWM2M_SERVER_ONENET){
                if (data->state == FOTA_STATE_IDIL)//last state is idil
                {
                    cissys_setFwState(data, FOTA_STATE_DOWNLOADING);
                    object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_204_CHANGED);
#ifdef CONFIG_LWM2M_ONENET_SOTA
                    if (lwm2mH->isupdate_mcu == true) // sota
                    {
                        lwm2mPostEvent(lwm2mH->ref, EVETN_IND, LWM2M_EVENT_SOTA_DOWNLOADING, 0, 0);
                    }
                    else // fota
#endif
                    {
                        lwm2mPostEvent(lwm2mH->ref, EVETN_IND, LWM2M_EVENT_FIRMWARE_DOWNLOADING, 0, 0);
                    }
                    return COAP_IGNORE;//YYL 2018/12/25
                }
                else if (data->state == FOTA_STATE_DOWNLOADING)//last state is downloading
                {
                    if (data->result == FOTA_RESULT_INIT
                    || data->result == FOTA_RESULT_DISCONNECT
                    || data->result == FOTA_RESULT_OVERFLOW)  //resume downloading
                    {
                        object_asynAckNodata(lwm2mH, lwm2mH->fw_request.response_ack, COAP_204_CHANGED);//send ack to peer and do nothing
                        return COAP_IGNORE;
                    }
                    else
                    {
                        return COAP_400_BAD_REQUEST;//send 400
                    }
                }
                else
                {
                    return COAP_400_BAD_REQUEST;
                }
            }
            return COAP_IGNORE;;
#endif
    default:
        LOG("std_firmware_execute COAP_405_METHOD_NOT_ALLOWED");
        return COAP_405_METHOD_NOT_ALLOWED;
    }
    LOG("std_firmware_execute,end");
}

void display_firmware_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    firmware_data_t * data = (firmware_data_t *)object->userData;
    fprintf(stdout, "  /%u: Firmware object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "    state: %u, supported: %s, result: %u\r\n",
                data->state, data->supported?"true":"false", data->result);
    }
#endif
}

static void free_object_firmware(lwm2m_object_t * objectP)
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
    //lwm2m_free(objectP);
}

lwm2m_object_t * get_object_firmware(void * lwm2mH)
{
    /*
     * The get_object_firmware function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * firmwareObj;

    firmwareObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != firmwareObj)
    {
        memset(firmwareObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns its unique ID
         * The 5 is the standard ID for the optional object "Object firmware".
         */
        firmwareObj->objID = LWM2M_FIRMWARE_UPDATE_OBJECT_ID;
        firmwareObj->lwm2mH = lwm2mH;

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

        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        firmwareObj->readFunc    = prv_firmware_read;
        firmwareObj->writeFunc   = prv_firmware_write;
#ifdef CONFIG_LWM2M_ONENET_SUPPORT
        if(((lwm2m_context_t *)lwm2mH)->serverType == LWM2M_SERVER_ONENET)
            firmwareObj->discoverFunc = prv_firmware_discover;
#endif
        firmwareObj->executeFunc = prv_firmware_execute;
        firmwareObj->cleanFunc = free_object_firmware;
        firmwareObj->userData    = lwm2m_malloc(sizeof(firmware_data_t));
        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != firmwareObj->userData)
        {
#ifdef CONFIG_LWM2M_ONENET_UPDATE
            if(((lwm2m_context_t *)lwm2mH)->serverType == LWM2M_SERVER_ONENET){
                LOG_ARG("fw_flag: %d",((lwm2m_context_t *)lwm2mH)->fw_flag);
                if (((lwm2m_context_t *)lwm2mH)->fw_flag)
                {
                    LOG("goto cissys_resetFwContext");
                    if (cissys_resetFwContext())
                    {
                        LOG("cissys_resetFwContext success");
                    }
                    else
                    {
                        LOG("cissys_resetFwContext fail");
                    }
                }
                cissys_readContext(firmwareObj->userData);
            }else
#endif
            {
                ((firmware_data_t*)firmwareObj->userData)->state = 1;
                ((firmware_data_t*)firmwareObj->userData)->supported = false;
                ((firmware_data_t*)firmwareObj->userData)->result = 0;
            }
        }
        else
        {
            lwm2m_free(firmwareObj->instanceList);
            lwm2m_free(firmwareObj);
            firmwareObj = NULL;
        }
    }

    return firmwareObj;
}

void set_fota_state_result(lwm2m_fota_state_t state, lwm2m_fota_result_t result)
{
    g_fota_state = state;
    s_fota_resulte = result;
}

void notify_fota_state(lwm2m_fota_state_t state, lwm2m_fota_result_t resulte, uint8_t ref)
{
    g_fota_state = state;
    s_fota_resulte = resulte;
    LOG_ARG("notify_fota_state state %d resulte %d\r\n", state, resulte);
    if(state == LWM2M_FOTA_STATE_IDLE && (resulte == LWM2M_FOTA_RESULT_SUCCESS || resulte == LWM2M_FOTA_RESULT_INIT))
    {
        LOG_ARG( "get fota err state %d resulte %d\r\n", state, resulte);
#if 0
#ifdef LTE_NBIOT_SUPPORT
        if(s_psm_state != 0)
        {
            nbiot_nvSetPsmEnable(s_psm_state);
            CFW_NvParasChangedNotification(1);
        }
#endif
#endif
    }
    lwm2m_notify_fota_state(state, resulte, ref);

}

uint32_t write_fota_upgrade_data(uint32_t block_num, uint8_t  block_more, uint8_t * data, uint16_t datalen)
{
#ifdef CONFIG_LWM2M_FOTA_SUPPORT
    static int fd = 0;
    if (0==block_num)
    {
        fupdateInvalidate(true);
        vfs_mkdir(CONFIG_FS_FOTA_DATA_DIR, 0);
        fd  = vfs_open(FUPDATE_PACK_FILE_NAME, O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0);
        if (fd < 0)
            return COAP_IGNORE;
    }

    if (fd <= 0)
        return COAP_IGNORE;

    ssize_t write_len = vfs_write(fd, data, datalen);
    if(write_len != datalen)
    {
      vfs_close(fd);
      fd = 0;
      LOG("write_fota_upgrade_data vfs_write failed\r\n");
      return COAP_IGNORE;
    }
    if(!block_more)
    {
        vfs_close(fd);
        fd = 0;
    }
#endif
    return COAP_NO_ERROR;
}

bool check_fota_file_sanity()
{
 #if 0
    LOG("check_fota_file_sanity \r\n");
 #ifdef CONFIG_LWM2M_FOTA_SUPPORT
    return (fupdateCheckSignedPack() && fupdateIsPackValid(NULL));
 #else
    return true;
 #endif
 #endif
}

void execute_fota_upgrade()
{
    #if 0
 #ifdef CONFIG_LWM2M_FOTA_SUPPORT
    bool result = (fupdateCheckSignedPack() && fupdateIsTmpSpaceEnough() && fupdateSetReady(NULL)
#ifdef CONFIG_FUPDATE_SUPPORT_VERSION_CHECK
    &&fupdateIsVersionUpgrade()
#endif
 );

    if (result)
    {
        LOG("execute_fota_upgrade swithoff \r\n");
        g_reboot = 1;
    }
    else
    {
         fupdateInvalidate(true);
    }
#endif
#endif
}

#ifdef  CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
bool lwm2m_send_critical_data_ok()
{
    if(g_fota_state == LWM2M_FOTA_STATE_IDLE)
    {
        return false;
    }
    return true;
}

bool lwm2m_start_controlled_fota_download(uint8_t ref)
{
    if((g_fota_state != LWM2M_FOTA_STATE_IDLE ))
    {
        return false;
    }
    if(g_download_URI == NULL)
    {
        return false;
    }

    if(Is_Coap_URI(g_download_URI))
    {
        notify_fota_state(LWM2M_FOTA_STATE_DOWNLOADING, LWM2M_FOTA_RESULT_INIT, ref);
        lwm2m_start_fota_download(g_download_URI, ref);
    }
    
    if(g_download_URI != NULL)
    {
        free(g_download_URI);
        g_download_URI = NULL;
    }
    return true;
}

bool lwm2m_cancel_controlled_fota_download(uint8_t ref)
{
    if((g_fota_state != LWM2M_FOTA_STATE_IDLE ))
    {
        return false;
    }
    notify_fota_state(LWM2M_FOTA_STATE_IDLE, LWM2M_FOTA_RESULT_FIRMWARE_UPDATE_FAILED, ref);
    if(g_download_URI != NULL)
    {
        free(g_download_URI);
        g_download_URI = NULL;
    }
    return true;
}

bool lwm2m_start_controlled_fota_update(uint8_t ref)
{
    if((g_fota_state != LWM2M_FOTA_STATE_DOWNLOADED ))
    {
        return false;
    }
    notify_fota_state(LWM2M_FOTA_STATE_UPDATING, LWM2M_FOTA_RESULT_SUCCESS, ref);
    execute_fota_upgrade();
    return true;
}

bool lwm2m_cancel_controlled_fota_update(uint8_t ref)
{
    if((g_fota_state != LWM2M_FOTA_STATE_DOWNLOADED ))
    {
        return false;
    }
    notify_fota_state(LWM2M_FOTA_STATE_IDLE, LWM2M_FOTA_RESULT_FIRMWARE_UPDATE_FAILED, ref);
    return true;
}
#endif


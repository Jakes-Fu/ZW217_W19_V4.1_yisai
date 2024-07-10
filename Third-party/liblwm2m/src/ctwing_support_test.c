/******************************************************************************
 ** File Name:      atc_daps_api.c                                           *
 ** Author:                                                                   *
 ** DATE:                                                                     *
 ** Copyright:                                                                *
 ** Description:    This file contains the functions that handle the api
 **                 used to sopport onenet platform  like  LWM2M_XXX()  and provide test examples  like 
                    LWM2M_TEST_XXX()
 ******************************************************************************/

#include "sci_types.h"
#include "sci_api.h"
#include "mn_type.h"



#ifdef LIBLWM2M_SUPPORT
#include "lwm2m_api.h"
#endif

#ifdef CONFIG_LWM2M_CTWING_SUPPORT

typedef struct
{
    char ipaddress[128];
    uint32_t port;
    uint32_t lifetime;
    uint32_t securityMode;
    char pskid[128];
    char psk[128];
    uint32_t bindingMode;
    uint8_t regType; // 0 is manual registration; 1 is self registration
    uint8_t authType;
} aepConfig_t;

typedef enum AEP_CMD_STATUS_E
{
    AEP_CMD_STATUS_LOGOUT,
    AEP_CMD_STATUS_LOGONING,
    AEP_CMD_STATUS_LOGON_WITHOUT_OBSERVER,
    AEP_CMD_STATUS_LOGON_WITH_OBSERVER,
    AEP_CMD_STATUS_LOGOUTING,
    AEP_CMD_STATUS_REQUIRING
} AEP_CMD_STATUS;

static int8_t gAepRef = -1;
static aepConfig_t gAepConfig;
AEP_CMD_STATUS gAepStatus;
uint8_t g_imei_test[16] = {8,6,7,4,0,0,0,2,1,4,3,5,2,0,5};
 extern BLOCK_ID g_liblwm2m_MESSAGE_TASK;
void aep_server_ind_process(uint32 argc, void* argv)
{
    int i = 0;    
    xSignalHeaderRec *sig_ptr = NULL;
    lwm2mEventSIG_t* pEvent = NULL;
    uint8_t ref;
    lwm2mEvent_t *lwm2m_ev;
    uint32_t result;
    uint32_t result_param1;
    uint32_t result_param2;
    
    
    SCI_TRACE_LOW("[ LWM2MDEBUG]  %s  %d\n",__FUNCTION__,__LINE__);
    g_liblwm2m_MESSAGE_TASK = SCI_IdentifyThread();


    while (1)
    {
        SCI_TRACE_LOW("[ LWM2MDEBUG]   wait signal  [%s ,%d]\n",__FUNCTION__,__LINE__);
        sig_ptr = SCI_GetSignal(g_liblwm2m_MESSAGE_TASK);
        if (NULL != sig_ptr)
        {
            pEvent = (lwm2mEventSIG_t*) sig_ptr;            
        }
    
    
        //atCmdEngine_t *engine = (atCmdEngine_t *)pEvent->param1;
        ref = (uint8_t)pEvent->param2;
        lwm2m_ev = (lwm2mEvent_t *)pEvent->param3;

        result = lwm2m_ev->param1;
        result_param1 = lwm2m_ev->param2;
        result_param2 = lwm2m_ev->param3;

        SCI_TRACE_LOW("[ LWM2MDEBUG] CTM2M# aep_server_ind_process nEventId=%d", pEvent->id);
        SCI_TRACE_LOW("[ LWM2MDEBUG] CTM2M# aep_server_ind_process result=%d", result);
        SCI_TRACE_LOW("[ LWM2MDEBUG] CTM2M# aep_server_ind_process result_param1=%d", result_param1);
        SCI_TRACE_LOW("[ LWM2MDEBUG] CTM2M# aep_server_ind_process result_param2=%d", result_param2);
        
        switch (pEvent->id)
        {
            case EVETN_IND:
            {
#if DEBUG_CDP_EVENT
                char refstr[50] = {0};
                sprintf(refstr, "+LWM2MEVENT:%d,%s", ref, lwm2m_eventName(result));
                SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                //aepRespUrcText(refstr);
#endif
                if (result == LWM2M_EVENT_UPDATE_SUCCESS)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:ping,%d", 0);
                    //aepRespUrcText(refstr);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    
                }
                if (result == LWM2M_EVENT_UPDATE_FAILED)
                {
                    char refstr[100] = {0};
                    int err_code = 0;
                    err_code = get_err_code(result_param2);
                    sprintf(refstr, "+CTM2M:ping,%d", err_code);
                    //aepRespUrcText(refstr);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    
                }
                if (result == LWM2M_EVENT_UPDATE_TIMEOUT)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:ping,%d", 1);
                    //aepRespUrcText(refstr);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                }
                if (result == LWM2M_EVENT_UPDATE_BINDING_SEND)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2MUPDATE:%ld", result_param1);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    // This case must have AT engine and need to finish AT engine.
                    if (0)//gAepAtEngine != NULL && atCmdEngineIsValid(gAepAtEngine))
                    {
                        //atCmdRespInfoText(gAepAtEngine, refstr);
                        //atCmdRespOK(gAepAtEngine);
                    }
                }
                if (result == LWM2M_EVENT_UPDATE_BINDING_SUCCESS)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:update,%d,%ld", 0, result_param1);
                    //aepRespUrcText(refstr);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    
                }
                if (result == LWM2M_EVENT_UPDATE_BINDING_FAILED)
                {
                    char refstr[100] = {0};
                    int err_code = 0;
                    err_code = get_err_code(result_param2);
                    sprintf(refstr, "+CTM2M:update,%d,%ld", err_code, result_param1);
                    //aepRespUrcText(refstr);
                }
                if (result == LWM2M_EVENT_UPDATE_BINDING_TIMEOUT)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:update,%d,%ld", 1, result_param1);
                     SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                }
                if (result == LWM2M_EVENT_DEREG_SUCCESS)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:dereg,%d", 0);
                     SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                    gAepStatus = AEP_CMD_STATUS_LOGOUT;
                }
                if (result == LWM2M_EVENT_DEREG_FAILED)
                {
                    char refstr[100] = {0};
                    int err_code = 0;
                    err_code = get_err_code(result_param2);
                    sprintf(refstr, "+CTM2M:dereg,%d", err_code);
                    //aepRespUrcText(refstr);
                     SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    gAepStatus = AEP_CMD_STATUS_LOGOUT;
                }
                if (result == LWM2M_EVENT_DEREG_TIMEOUT)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:dereg,%d", 1);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                    gAepStatus = AEP_CMD_STATUS_LOGOUT;
                }
                if (result == LWM2M_EVENT_REG_SUCCESS)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:reg,%d", 0);
                    //aepRespUrcText(refstr);
                    gAepStatus = AEP_CMD_STATUS_LOGON_WITHOUT_OBSERVER;
                     SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                }
                if (result == LWM2M_EVENT_REG_FAILED)
                {
                    char refstr[100] = {0};
                    int err_code = 0;
                    err_code = get_err_code(result_param2);
                    sprintf(refstr, "+CTM2M:reg,%d", err_code);
                     SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                    gAepStatus = AEP_CMD_STATUS_LOGOUT;
                }
                if (result == LWM2M_EVENT_REG_TIMEOUT)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:reg,%d", 1);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                    gAepStatus = AEP_CMD_STATUS_LOGOUT;
                }
            }
            break;
            case SERVER_QUIT_IND:
            {
                if (gAepStatus == AEP_CMD_STATUS_LOGOUTING)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:dereg,%d", 0);
                    //aepRespUrcText(refstr);
                     SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                }
                if (gAepStatus == AEP_CMD_STATUS_LOGONING)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:reg,%d", 2);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                }
#ifdef CONFIG_CTIOTSM_SUPPORT
                if (0)//gAepStatus == AEP_CMD_STATUS_REQUIRING)
                {
                    if (result == 1)
                    {
                        // This case must have AT engine and need to finish AT engine.
                        if (gAepAtEngine != NULL && atCmdEngineIsValid(gAepAtEngine))
                            atCmdRespOK(gAepAtEngine);
                    }
                    else
                    {
                        if (gAepAtEngine != NULL && atCmdEngineIsValid(gAepAtEngine))
                            atCmdRespCmeError(gAepAtEngine, ERR_AT_CME_EXE_FAIL);
                    }
                }
#endif
                gAepStatus = AEP_CMD_STATUS_LOGOUT;
                gAepRef = -1;
                //gAepAtEngine = NULL;
                lwm2m_free_config(ref);
            }
            break;
            case OBJ_READ_IND:
            {
                read_param_t *param = (read_param_t *)result_param1;
#if DEBUG_CDP_EVENT
                uint32_t msgid = result;
#endif
                if (param->objid == 19 && param->insid == 0 && param->resid == 0)
                {
                    lwm2m_read_rsp(19, 0, 0, lwm2m_opaque, NULL, 0, true, true, ref);
                }
                else
                    lwm2m_read_rsp(19, 0, 0, lwm2m_opaque, NULL, 0, true, false, ref);
#if DEBUG_CDP_EVENT
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+MIPLREAD:%d,%d,%d,%d,%d,%d", ref, msgid, param->objid, (short)param->insid, (short)param->resid, param->count);
                     SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                }
#endif
            }
            break;
            case OBJ_WRITE_IND:
            {
                write_param_t *param = (write_param_t *)result_param1;
                uint32_t count = result_param2;
                int i ;
                if (param->objid == 19 && param->insid == 1 && param->resid == 0 && count == 1)
                {
                    int dataLen = param->len * 2 + 20;
                    char *refstr = (char *)malloc(dataLen);
                    if (refstr == NULL)
                    {
                        SCI_TRACE_LOW("[ LWM2MDEBUG]  +CTM2MRECV: not enough memory");
                        break;
                    }
                    memset(refstr, 0, dataLen);
                    sprintf(refstr, "+CTM2MRECV:");
                    for ( i = 0; i < param->len; i++)
                    {
                        char tmp[5] = {0};
                        sprintf(tmp, "%02x", param->data[i]);
                        strcat(refstr, tmp);
                        SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    }
                    #if 0
                    int i = 0;
                    for (; i < (strlen(refstr) / 512); i++)
                    {
                        if (gAepAtEngine != NULL && atCmdEngineIsValid(gAepAtEngine))
                            atCmdRespInfoNText(gAepAtEngine, refstr + (i * 512), 512);
                        else
                            atCmdRespDefUrcNText(refstr + (i * 512), 512);
                    }
                    if (gAepAtEngine != NULL && atCmdEngineIsValid(gAepAtEngine))
                        atCmdRespInfoNText(gAepAtEngine, refstr + (i * 512), strlen(refstr + (i * 512)));
                    else
                        atCmdRespDefUrcNText(refstr + (i * 512), strlen(refstr + (i * 512)));
                    #endif
                    free(refstr);
                    lwm2m_write_rsp(1, ref);
                }
                else
                {
                    SCI_TRACE_LOW("[ LWM2MDEBUG]  OBJ_WRITE_IND Failed! objid=%d, insid=%d, resid=%d, count=%d", param->objid, param->insid, param->resid, count);
                }
            }
            break;
            case OBJ_OBSERVE_IND:
            {
                uint16_t objid = result_param1 >> 16;
                int16_t insid = result_param1;
                int16_t resid = result_param2 >> 16;
                uint8_t isAddObserve = result_param2;
                char refstr[100] = {0};
                if (objid == 19 && insid == 0 && resid == 0)
                {
                    if (isAddObserve == 0)
                    {
                        if (gAepStatus == AEP_CMD_STATUS_LOGON_WITH_OBSERVER)
                        {
                            sprintf(refstr, "+CTM2M:obsrv,%d", 2);
                            SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                            //aepRespUrcText(refstr);
                        }
                        gAepStatus = AEP_CMD_STATUS_LOGON_WITHOUT_OBSERVER;
                    }
                    else
                    {
                        if (gAepStatus == AEP_CMD_STATUS_LOGON_WITHOUT_OBSERVER)
                        {
                            sprintf(refstr, "+CTM2M:obsrv,%d", 0);
                            //aepRespUrcText(refstr);
                            SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                        }
                        gAepStatus = AEP_CMD_STATUS_LOGON_WITH_OBSERVER;
                    }
                }
            }
            break;
            case DATA_SEND_IND:
            {
                if (result == LWM2M_EVENT_NOTIFY_SEND_SUCCESS)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2MSEND:%ld", result_param1);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    // This case must have AT engine and need to finish AT engine.
                    if (0)//gAepAtEngine != NULL && atCmdEngineIsValid(gAepAtEngine))
                    {
                        //atCmdRespInfoText(gAepAtEngine, refstr);
                        //atCmdRespOK(gAepAtEngine);
                    }
                }
                if (result == LWM2M_EVENT_NOTIFY_SENT_SUCCESS)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:send,0,%ld", result_param1); //result_param1
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                }
                if (result == LWM2M_EVENT_NOTIFY_SENT_TIMEOUT)
                {
                    char refstr[100] = {0};
                    sprintf(refstr, "+CTM2M:send,1,%ld", result_param1);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                }
                if (result == LWM2M_EVENT_NOTIFY_FAILED)
                {
                    char refstr[100] = {0};
                    int err_code = 11;
                    if (result_param2 == 3) //COAP_TYPE_RST
                        err_code = 9;
                    sprintf(refstr, "+CTM2M:send,%d,%ld", err_code, result_param1);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                    //aepRespUrcText(refstr);
                }
            }
            break;
            case FOTA_DOWNLOADING_IND:
            {
                char refstr[100] = {0};
                int download_seze = result_param1;
                if (download_seze == 0)
                {
                    sprintf(refstr, "FOTA download start");
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                }
                else
                {
                    sprintf(refstr, "FOTA downloading ... %d B", download_seze);
                    SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                }
                //aepRespUrcText(refstr);
            }
            break;
            case FOTA_DOWNLOAD_FAILED_IND:
            {
                char refstr[100] = {0};
                sprintf(refstr, "FOTA download failed");
                SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                //aepRespUrcText(refstr);
            }
            break;
            case FOTA_DOWNLOAD_SUCCESS_IND:
            {
                char refstr[100] = {0};
                sprintf(refstr, "FOTA download success");
                SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                //aepRespUrcText(refstr);
            }
            break;
            case FOTA_PACKAGE_CHECK_IND:
            {
                char refstr[100] = {0};
                sprintf(refstr, "FOTA package check %s", result_param1 == 1 ? "success" : "failed");
                //aepRespUrcText(refstr);
                SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                if (result_param1 != 1)
                {
                   // aepRespUrcText("FOTA update failed");
                   SCI_TRACE_LOW("[ LWM2MDEBUG]  FOTA update failed  [%s ,%d]\n",__FUNCTION__,__LINE__); 
                }
            }
            break;
            case FOTA_UPGRADING_IND:
            {
                char refstr[100] = {0};
                sprintf(refstr, "FOTA updating. Ready to reboot");
                SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                //aepRespUrcText(refstr);
            }
            break;
            case FOTA_UPGRADE_OK_IND:
            {
                char refstr[100] = {0};
                sprintf(refstr, "FOTA update success");
                SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                //aepRespUrcText(refstr);
            }
            break;
            case MESSAGE_TASK_EXIT_IND:
                if(lwm2m_ev !=NULL)
                {
                    free(lwm2m_ev);
                    lwm2m_ev = NULL;
                }
                if(pEvent !=NULL)
                {
                    free(pEvent);                    
                }    
                SCI_TRACE_LOW(" LWM2MDEBUG     EXIT  MESSAGE TASK!!! [%s  %d]\n",__FUNCTION__,__LINE__);
                g_liblwm2m_MESSAGE_TASK = SCI_INVALID_BLOCK_ID ;
                lwm2m_Thread_stop();
                break;                
            default:
            {
#if DEBUG_CDP_EVENT
                char refstr[50] = {0};
                sprintf(refstr, "+UNPROCESS EVENT:%d", pEvent->id);
                SCI_TRACE_LOW("[ LWM2MDEBUG]   refstr: %s  [%s ,%d]\n",refstr,__FUNCTION__,__LINE__);
                //aepRespUrcText(refstr);
#endif
            }
            break;

        }           
    SCI_TRACE_LOW("  LWM2MDEBUG aep_server_ind_process exit EventId=%d,gAepRef=%d", pEvent->id, gAepRef);
    free(lwm2m_ev);
    lwm2m_ev = NULL;
    free(pEvent);
    }
}


int LWM2M_CTM2MINIT(const char *ipaddress ,uint32_t uPort,uint32_t lifetime,uint32_t binding_mode,uint32_t security_mode,const char *pskid,const char *psk )
{
       if (security_mode == 1 || security_mode == 2)
        { 
            if ( pskid == NULL)
            {
                SCI_TRACE_LOW("  LWM2MDEBUG cann't get pskid  [%s  %d]\n",__FUNCTION__,__LINE__);                
                return -1;
            }
            if (strlen(pskid) > 127)
            {
                SCI_TRACE_LOW("  LWM2MDEBUG  cann't get pskid [%s  %d]\n",__FUNCTION__,__LINE__);                  
                return -1;
            }
           
            if ( psk == NULL)
            {
                SCI_TRACE_LOW("  LWM2MDEBUG cann't get psk [%s  %d]\n",__FUNCTION__,__LINE__);
                return  -1;
            }
            if (strlen(psk) > 127)
            {
                SCI_TRACE_LOW("  LWM2MDEBUG cann't get psk [%s  %d]\n",__FUNCTION__,__LINE__);                
                return -1;
            }
        }

       memset(gAepConfig.ipaddress, 0, sizeof(gAepConfig.ipaddress));
        strncpy(gAepConfig.ipaddress, ipaddress, sizeof(gAepConfig.ipaddress) - 1);
        gAepConfig.port = uPort;
        gAepConfig.lifetime = lifetime;
        gAepConfig.bindingMode = binding_mode;
        gAepConfig.securityMode = security_mode;
        memset(gAepConfig.pskid, 0, sizeof(gAepConfig.pskid));
        memset(gAepConfig.psk, 0, sizeof(gAepConfig.psk));
        if (security_mode == 1 || security_mode == 2)
        {
            if (pskid != NULL)
                strncpy(gAepConfig.pskid, pskid, sizeof(gAepConfig.pskid) - 1);
            if (psk != NULL)
                strncpy(gAepConfig.psk, psk, sizeof(gAepConfig.psk) - 1);
        }
        SCI_TRACE_LOW("  LWM2MDEBUG gAepConfig.ipaddress :%s ,gAepConfig.port %d  [%s  %d]\n",gAepConfig.ipaddress,gAepConfig.port,__FUNCTION__,__LINE__); 

        
        return 0;

}
extern bool getSimImei(uint8_t simId, uint8_t *imei, uint8_t *len);

int  LWM2M_CTM2MREG(uint16_t lifetime)
{
     uint8_t imei[16] = {0};
     uint8_t imeisize = 16;
    uint8_t cmdline[500] = {0};
    int8_t ref = -1;

    int ret = 0;
    if (gAepRef != -1)
    {
        SCI_TRACE_LOW("  LWM2MDEBUG  gAepRef=%d   [%s  %d]\n",gAepRef,__FUNCTION__,__LINE__);         
        //atCmdRespErrorText(pParam->engine, "+CTM2M ERROR:33");
        return -1;
    }

    #ifdef CONFIG_CTIOTSM_SUPPORT  //not use
    if (1 == gAepConfig.authType)
    {
        SCI_TRACE_LOW("  LWM2MDEBUG  1 == gAepConfig.authType,simid mode not support");
        //atCmdRespErrorText(pParam->engine, "+CTM2M ERROR:33");
        return -1;
    }
    if ((2 == gAepConfig.authType) && (1 != gKeyIndex))
    {
        SCI_TRACE_LOW("  LWM2MDEBUG 2 == gAepConfig.authType) && (0 == g_isSelectKey),invalid");
        //atCmdRespErrorText(pParam->engine, "+CTM2M ERROR:33");
        return -1;
    }
#endif


    gAepConfig.lifetime = lifetime;
#if 1
    if(false == getSimImei(MN_DUAL_SYS_1,imei,&imeisize))
    {
        SCI_TRACE_LOW("  LWM2MDEBUG get imei error [ %s  %d]\n",__FUNCTION__,__LINE__);
        //atCmdRespErrorText(pParam->engine, "+CTM2M ERROR:33");
        //return -1;
        //imei = g_imei_test;
        
    }
    #endif
    SCI_TRACE_LOW("  LWM2MDEBUG get imei :%s [ %s  %d]\n",imei,__FUNCTION__,__LINE__);
    
      
#ifdef CONFIG_CTIOTSM_SUPPORT
    if (2 == gAepConfig.authType)
    {
        sprintf((char *)cmdline, "-n %s -h %s -p %ld -4 -t %ld -e %d -M %d -B %ld", imei, gAepConfig.ipaddress, gAepConfig.port, gAepConfig.lifetime,LWM2M_SERVER_AEP,gAepConfig.authType, gAepConfig.bindingMode);
    }
    else
    {
#endif
        if (gAepConfig.securityMode == 1 || gAepConfig.securityMode == 2)
        {
            SCI_TRACE_LOW("  LWM2MDEBUG  [ %s  %d]\n",__FUNCTION__,__LINE__);
            if (strlen(gAepConfig.pskid) == 0 || strlen(gAepConfig.psk) == 0)
            {
                SCI_TRACE_LOW("  LWM2MDEBUG  pskid or psk is NULL [ %s  %d]\n",__FUNCTION__,__LINE__);
                //AT_CMD_RETURN(atCmdRespErrorText(pParam->engine, "+CTM2M ERROR:3"));
            }
            else
            {
                sprintf((char *)cmdline, "-n %s -h %s -p %ld -4 -t %ld -i %s -s %s  -e %d -B %ld", imei, gAepConfig.ipaddress, gAepConfig.port, gAepConfig.lifetime, gAepConfig.pskid, gAepConfig.psk,LWM2M_SERVER_AEP, gAepConfig.bindingMode);
            }
        }
        else
            sprintf((char *)cmdline, "-n %s -h %s -p %ld -4 -t %ld  -e %d -B %ld", imei, gAepConfig.ipaddress, gAepConfig.port, gAepConfig.lifetime,LWM2M_SERVER_AEP, gAepConfig.bindingMode);
#ifdef CONFIG_CTIOTSM_SUPPORT
        }
#endif
    SCI_TRACE_LOW("  LWM2MDEBUG cmdline:%s   [ %s  %d]\n",cmdline,__FUNCTION__,__LINE__);

    
    ref = lwm2m_new_config(cmdline);
    if (ref >= 0)
    {
        lwm2m_ret_t ret;        
        SCI_TRACE_LOW("  LWM2MDEBUG wm2m_new_config success ref:%d  [ %s  %d]\n",ref,__FUNCTION__,__LINE__);
        
        
        if(liblwm2m_MessageHandleTask((osiCallback_t)aep_server_ind_process) != 0)
        {
            SCI_TRACE_LOW( " LWM2MDEBUG MessageHandleTask error:[%s  %d]\n",__FUNCTION__,__LINE__);     
            return -1;
        }
        ret = lwm2m_add_obj(19, 2, (uint8_t *)"11", 6, ref);
        if (ret != LWM2M_RET_OK || lwm2m_register(ref, lifetime, 60) != LWM2M_RET_OK)
        {
            SCI_TRACE_LOW( " LWM2MDEBUG lwm2m_register error [ %s  %d]\n",cmdline,__FUNCTION__,__LINE__);
            lwm2m_free_config(ref);
            gAepRef = -1;
            return -1;
            //AT_CMD_RETURN(atCmdRespErrorText(pParam->engine, "+CTM2M ERROR:34"));
        }
        else
        {
            gAepRef = ref;
            //gAepAtEngine = pParam->engine;
            //gAepStatus = AEP_CMD_STATUS_LOGONING;
            return 0;
            //RETURN_OK(pParam->engine);
        }
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG lwm2m_new_config error [ %s  %d]\n",cmdline,__FUNCTION__,__LINE__);        
    }
}

int  LWM2M_CTM2MDEREG(void)
{
    int ret;
    if (gAepRef == -1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  err gAepRef:%d [%s  %d]\n",gAepRef,__FUNCTION__,__LINE__);           
        return -1;    
    }
    
    if (lwm2m_unregister(gAepRef) == LWM2M_RET_OK)    
    {
        SCI_TRACE_LOW(  " LWM2MDEBUG  lwm2m unregister success");          
        ret =0;
    }
    else
    {
        SCI_TRACE_LOW(  " LWM2MDEBUG  lwm2m unregister failed");
        ret = -1;  
    }

}

int LWM2M_TEST_CTM2MINIT(int is_dtls)
{
    int ret ;
    if(is_dtls !=1)
    {
        const char *ipaddress ="221.229.214.202" ;
        uint32_t uPort = 5683;
        uint32_t lifetime =86400 ;
        uint32_t binding_mode = 0;
        uint32_t security_mode = 0;
        if((ret =LWM2M_CTM2MINIT(ipaddress , uPort, lifetime, binding_mode, security_mode,NULL,NULL)) == -1)
        {
            SCI_TRACE_LOW( " LWM2MDEBUG init error:[%s  %d]\n",__FUNCTION__,__LINE__);     
            return -1;
        }
        else
        {
            SCI_TRACE_LOW( " LWM2MDEBUG init is ok:[%s  %d]\n",__FUNCTION__,__LINE__);     
        }
        return 0;
    }
    else
    {
         const char *ipaddress ="221.229.214.202" ;
        uint32_t uPort = 5684;
        uint32_t lifetime =86400 ;
        uint32_t binding_mode = 0;
        uint32_t security_mode = 1;
        const char *pskid ="352273017386343";
        const char *psk = "111222333444";
        if((ret =LWM2M_CTM2MINIT(ipaddress , uPort, lifetime, binding_mode, security_mode,pskid,psk)) == -1)
        {
            SCI_TRACE_LOW( " LWM2MDEBUG init error:[%s  %d]\n",__FUNCTION__,__LINE__);     
            return -1;
        }
        else
        {
            SCI_TRACE_LOW( " LWM2MDEBUG init is ok:[%s  %d]\n",__FUNCTION__,__LINE__);     
        }
        return 0;
    }
}

int LWM2M_TEST_CTM2MREG(void)
{
    int ret;
    int lifetime = 160;
    
    if((ret = LWM2M_CTM2MREG(lifetime)) == -1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG register error:[%s  %d]\n",__FUNCTION__,__LINE__);     
        return -1;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG register is ok:[%s  %d]\n",__FUNCTION__,__LINE__);     
    }
    return 0;
}


int LWM2M_TEST_CTM2MDEREG(void)
{
    int ret;
    if((ret = LWM2M_CTM2MDEREG()) == -1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG unregister error:[%s  %d]\n",__FUNCTION__,__LINE__);     
        return -1;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG unregister is ok:[%s  %d]\n",__FUNCTION__,__LINE__);     
    }
    return 0;
}
#endif




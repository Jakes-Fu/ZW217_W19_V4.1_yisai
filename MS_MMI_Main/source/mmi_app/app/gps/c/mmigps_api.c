/*****************************************************************************
** File Name:        mmigps_api.c                                            *
** Author:           xiuyun.wang                                             *
** Date:             02/25/2020                                              *
** Copyright:                                                                *
** Description:    This file is used to define gps api                       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 02/25/2020           xiuyun.wang           Create                         *
******************************************************************************/

#include "sci_types.h"
#include "os_api.h"
#include "in_message.h"
#include "mmi_signal_ext.h"
#include "mn_gps_api.h"
#include "mmisms_export.h"
#include "mmigps_api.h"
#include "mmicom_trace.h"
#include "mmipdp_export.h"
#include "mmiwifi_export.h"
#include "mmiset_nv.h"
#include "mmi_nv.h"
#ifdef ADULT_WATCH_SUPPORT
#include "mmiconnection_export.h"
#endif

typedef struct 
{
    MMIGPS_STATUS_EUM cur_gps_state;
    BOOLEAN is_need_start_gps;//此变量表示用户start gps时，若 gps已经开启，则需要stop gps 然后再start gps
    BOOLEAN is_need_stop_gps;//此变量表示用户start gps时，若 gps已经开启，则需要stop gps 然后再start gps
    BOOLEAN last_gps_power_on_status;
    MN_DUAL_SYS_E dul_sys;  
    MMIGPS_START_GPS_PARAM_T start_param;
    GPS_REQUEST_CALLBACK gps_request_callback;
}MMIGPS_DATA_T;

MMI_APPLICATION_T g_mmigps_app = {0}; 
LOCAL MMIGPS_DATA_T g_gps_data = {0};
LOCAL MN_GPS_GNSS_MODE_E g_gps_gnss_mode = MNGPS_START_GPS_GLONASS;
#define TIMEOUT_WAIT_FOR_CORRECT_STATUS      (1*2*1000)
LOCAL uint8  g_wait_for_correct_status_timer_id  = 0;

LOCAL void Gps_HandleForCorrectGpsStatusTimer(uint8 timer_id,uint32 param)
{
    
    if (g_wait_for_correct_status_timer_id == timer_id)
    {
        MMK_StopTimer(g_wait_for_correct_status_timer_id);
        g_wait_for_correct_status_timer_id = 0;
    }
}

LOCAL void Gps_StartForCorrectGpsStatustimer(MN_DUAL_SYS_E dual_sys)
{
    if (NULL != g_wait_for_correct_status_timer_id)
    {
        MMK_StopTimer(g_wait_for_correct_status_timer_id);
        g_wait_for_correct_status_timer_id = 0;
    }
    g_wait_for_correct_status_timer_id = MMK_CreateTimerCallback(TIMEOUT_WAIT_FOR_CORRECT_STATUS, Gps_HandleForCorrectGpsStatusTimer, 0, TRUE);
}

/*****************************************************************************/
//  Description : get current GPS sim sys
//  Global resource dependence :
//  Author: weipengwang.rda
//  RETRUN: 
//  Note: 
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GPS_GetSimSys(void)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MN_DUAL_SYS_E temp_dual_sys = MN_DUAL_SYS_1;
    uint16 sim_sys = MN_SYS_SIM_1;
    uint32 cur_sim_num = 0;

    cur_sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (cur_sim_num == 0)
    {
        temp_dual_sys = MN_SYS_SIM_1;
        TRACE_APP_GPS("[GPS]: Sim not exist\n");
    }
    else
    {
        dual_sys = MMIAPISET_GetActiveSim();
        if (MMIAPIPHONE_IsSimAvailable(dual_sys))
        {
            temp_dual_sys = dual_sys;
        }
        else
        {
            temp_dual_sys = (MN_DUAL_SYS_E)sim_sys;
        }
    }
    TRACE_APP_GPS("[GPS]: temp_dual_sys:%d\n",temp_dual_sys);
    return temp_dual_sys;
}

/*****************************************************************************/
//  Description :handle gps message from L4
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGpsMsg(PWND app_ptr, uint16 msg_id, DPARAM  param)
{
    if (PNULL == app_ptr)
    {
        TRACE_APP_GPS("[GPS]: error app_ptr = PNULL\n");
        return MMI_RESULT_FALSE;
    }
    if(NULL == param)
    {
        TRACE_APP_GPS("[GPS]: error param = PNULL\n");
        return MMI_RESULT_FALSE;
    }
    switch ( msg_id )
    {
        TRACE_APP_GPS("[GPS]: msg_id:%d\n",msg_id);
        case APP_MN_GPS_DOWNLOAD_CNF:
        {
            APP_MN_GPS_DOWNLOAD_CNF_T *pDownloadcnf = (APP_MN_GPS_DOWNLOAD_CNF_T *)param;
            if(PNULL == pDownloadcnf)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_DOWNLOAD_CNF param is null\n");
                break;
            }
            if(TRUE == pDownloadcnf->is_ok)
            {
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_DOWNLOAD_CNF is ok\n");
                g_gps_data.cur_gps_state = GPS_DOWNLOAD_STATE;

                //if gps nv is open state, init
                if( TRUE == MMIGPS_OnOffState_Get())
                {
                    TRACE_APP_GPS("[GPS]: gps nv is open so init gps\n");
                    MMIGPS_Init();
                }
                else
                {
                    TRACE_APP_GPS("[GPS]: gps nv is close\n");
                }
            }else
            {
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_DOWNLOAD_CNF is NOT ok\n");
            }  
            break;
        }
        case APP_MN_GPS_INIT_CNF:
        {
            APP_MN_GPS_INIT_CNF_T *pInitcnf = (APP_MN_GPS_INIT_CNF_T *)param;
            BOOLEAN gps_on_off_state = TRUE;
            if(PNULL == pInitcnf)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_INIT_CNF param is null\n");
                break;
            }
            if(TRUE == pInitcnf->is_ok)
            {
                MMINV_WRITE(MMINV_SET_GPS_ON_OFF,&gps_on_off_state);
                g_gps_data.cur_gps_state = GPS_INIT_READY_STATE;
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_INIT_CNF is ok\n");
            }
            else
            {
                //MNGPS_InitReq(pInitcnf->dual_sys);
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_INIT_CNF is NOT ok\n");
            }
            if(NULL != g_gps_data.gps_request_callback)
            {
                g_gps_data.gps_request_callback((APP_MN_GPS_SIGNAL_E)msg_id,(void*)param);
            }
            break;
        }
        case APP_MN_GPS_START_CNF:
        {
            APP_MN_GPS_START_CNF_T *pStartcnf = (APP_MN_GPS_START_CNF_T *)param;
            if(PNULL == pStartcnf)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_START_CNF param is null\n");
                break;
            }
            if(TRUE == pStartcnf->is_ok)
            {
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_START_CNF is ok\n");
                g_gps_data.cur_gps_state = GPS_STARTED_STATE;
                
                if(TRUE == g_gps_data.is_need_stop_gps)
                {
                    g_gps_data.is_need_stop_gps = FALSE;
                    MNGPS_StopGpsReq(g_gps_data.dul_sys);

                }
                if(NULL != g_gps_data.gps_request_callback)
                {
                    g_gps_data.gps_request_callback((APP_MN_GPS_SIGNAL_E)msg_id,(void*)param);
                }
                MAIN_SetIdleGpsState(TRUE);
            }
            else
            {
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_START_CNF is not ok\n");

            }
            break;
        }
        case APP_MN_GPS_STOP_CNF:
        {
            APP_MN_GPS_STOP_CNF_T *pStopcnf = (APP_MN_GPS_STOP_CNF_T *)param;
            if(PNULL == pStopcnf)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_STOP_CNF param is null\n");
                break;
            }
            if(TRUE == pStopcnf->is_ok)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_STOP_CNF is ok\n");
                g_gps_data.cur_gps_state = GPS_STOPED_STATE;
                TRACE_APP_GPS("[GPS]: g_gps_data.gps_request_callback:%d\n",g_gps_data.gps_request_callback);
                if(NULL != g_gps_data.gps_request_callback)
                {
                    g_gps_data.gps_request_callback((APP_MN_GPS_SIGNAL_E)msg_id,(void*)param);
                }

                if(TRUE == g_gps_data.is_need_start_gps)
                {
                    TRACE_APP_GPS("[GPS]:APP_MN_GPS_STOP_CNF is ok,Need start gps\n");
                    if(MMIGPS_START_TYPE_NONE == g_gps_data.start_param.start_type)
                    {
                        MNGPS_StartGpsReq(pStopcnf->dual_sys);
                    }else
                    {
                        MNGPS_StartGpsReqEx(pStopcnf->dual_sys,g_gps_data.start_param.start_type,g_gps_data.start_param.start_mode);
                    }
                    g_gps_data.is_need_start_gps = FALSE;
                }else
                {

                    if(FALSE == g_gps_data.last_gps_power_on_status)//GPS之前的状态，若为FALSE,则之前没有power on GPS,现在需要Power off。
                    {
                    
                        TRACE_APP_GPS("[GPS]: APP_MN_GPS_STOP_CNF not power on\n");
                    }

                }
                MAIN_SetIdleGpsState(FALSE);
            }else
            {
                MNGPS_StopGpsReq(pStopcnf->dual_sys);
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_STOP_CNF is NOT ok\n");
            }
            break;
        }
        case APP_MN_GPS_POWEROFF_CNF:
        {
            BOOLEAN gps_on_off_state = FALSE;
            APP_MN_GPS_POWEROFF_CNF_T *pPoweroffcnf = (APP_MN_GPS_POWEROFF_CNF_T *)param;
            if(PNULL == pPoweroffcnf)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_POWEROFF_CNF param is null\n");
                break;
            }
            if(TRUE == pPoweroffcnf->is_ok)
            {
                MMINV_WRITE(MMINV_SET_GPS_ON_OFF,&gps_on_off_state);
                g_gps_data.cur_gps_state = GPS_DOWNLOAD_STATE;
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_POWEROFF_CNF is ok\n");
            }else
            {
                //MNGPS_PowerOffReq(pPoweroffcnf->dual_sys);
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_POWEROFF_CNF is NOT ok\n");
            }
            break;
        }
        case APP_MN_GPS_SET_GNSS_MODE_CNF:
        {
            APP_MN_GPS_SETGNSSMODE_CNF_T *pSetGnssModecnf = (APP_MN_GPS_SETGNSSMODE_CNF_T *)param;
            if(PNULL == pSetGnssModecnf)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_SET_GNSS_MODE_CNF param is null\n");
                break;
            }
            if(TRUE == pSetGnssModecnf->is_ok)
            {
                g_gps_data.cur_gps_state = GPS_IDLE_STATE;
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_SET_GNSS_MODE_CNF is ok\n");
                MMIPUB_OpenAlertWarningWin(TXT_GPS_GNSSMODE_SET_SUC);
                MMIAPIPHONE_PowerReset();
            }else
            {
                MMIPUB_OpenAlertFailWin(TXT_GPS_GNSSMODE_SET_FAIL);
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_SET_GNSS_MODE_CNF is NOT ok\n");
            }
            break;
        }
        case APP_MN_GPS_DEL_AID_DATA_CNF:
        {
            APP_MN_GPS_DEL_AID_DATA_CNF_T *pDelAidDataCnf = (APP_MN_GPS_DEL_AID_DATA_CNF_T *)param;
            if(PNULL == pDelAidDataCnf)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_DEL_AID_DATA_CNF param is null\n");
                break;
            }
            if(TRUE == pDelAidDataCnf->is_ok)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_DEL_AID_DATA_CNF is ok\n");
            }
            else
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_DEL_AID_DATA_CNF is NOT ok\n");
            }
            if(NULL != g_gps_data.gps_request_callback)
            {
                g_gps_data.gps_request_callback((APP_MN_GPS_SIGNAL_E)msg_id,(void*)param);
            }
            break;
        }
        default:
        {
            if(NULL != g_gps_data.gps_request_callback)
            {
                return g_gps_data.gps_request_callback((APP_MN_GPS_SIGNAL_E)msg_id,(void*)param);
            }else
            {
                return MMI_RESULT_FALSE;
            }
            break;
        }
    }
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : download gps module
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGPS_Download(void)
{
    TRACE_APP_GPS("enter function\n");
    SCI_MEMSET(&g_gps_data, 0x00, sizeof(MMIGPS_DATA_T));
    g_mmigps_app.ProcessMsg = HandleGpsMsg; 
    MNGPS_DownloadReq(MN_SYS_SIM_1);
}
/*****************************************************************************/
//  Description : init gps module
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIGPS_RES_E MMIGPS_Init(void)
{
    TRACE_APP_GPS("enter function\n");
    if(GPS_DOWNLOAD_STATE == g_gps_data.cur_gps_state)
    {
        MNGPS_InitReq(MN_SYS_SIM_1);
        return MMIGPS_RES_SUCC;
    }
    else
    {
        TRACE_APP_GPS("current gps state is not DOWMLOAD state, current state is :%d\n", g_gps_data.cur_gps_state);
        return MMIGPS_RES_STATE_WRONG;
    }
}
/*****************************************************************************/
//  Description : request for starting gps
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIGPS_RES_E MMIGPS_Start(MMIGPS_START_GPS_PARAM_T * start_param,GPS_REQUEST_CALLBACK call_back)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    TRACE_APP_GPS("[GPS]: request_type:%d\n",start_param->request_type);
    //入参有效性判断 
    if((MMIGPS_REQUEST_TYPE_MAX  <= start_param->request_type) || (start_param->request_type< 0))
    {
        TRACE_APP_GPS("[GPS]:Input Param request_type(%d)is wrong!\n",start_param->request_type);
        return MMIGPS_RES_PARAM_WRONG;
    }
    if( NULL == call_back)
    {
        TRACE_APP_GPS("[GPS]:Input Param call_back is NULL!\n");
        return MMIGPS_RES_PARAM_WRONG;
    }
    if((MNGPS_FACTORY_MODE < start_param->start_mode)||(start_param->start_mode < 0))
    {
        TRACE_APP_GPS("[GPS]:Input Param start mode(%d)is wrong\n",start_param->start_mode);
        return MMIGPS_RES_PARAM_WRONG;
    }
    if((MMIGPS_START_TYPE_NONE != start_param->start_type)&&(MMIGPS_START_TYPE_GPS != start_param->start_type)&&(MMIGPS_START_TYPE_BDS != start_param->start_type)&&
        (MMIGPS_START_TYPE_GPS_BDS != start_param->start_type)&&(MMIGPS_START_TYPE_GPS_GLONASS != start_param->start_type))
    {
        TRACE_APP_GPS("[GPS]: Input Param start_type(%d)is wrong\n",start_param->start_type);
        return MMIGPS_RES_PARAM_WRONG;
    }

    //全局变量赋值
    // 保存入参
    dual_sys = GPS_GetSimSys(); 
    g_gps_data.gps_request_callback = call_back;
    g_gps_data.dul_sys = dual_sys;
    //保存request_type 及启动模式、启动类型
    g_gps_data.start_param.request_type = start_param->request_type;
    g_gps_data.start_param.start_mode = start_param->start_mode;
    g_gps_data.start_param.start_type = start_param->start_type;
    TRACE_APP_GPS("gps current state:%d, start_type:%d,start_mode:%d\n",g_gps_data.cur_gps_state,g_gps_data.start_param.start_type,g_gps_data.start_param.start_mode);
    switch(g_gps_data.cur_gps_state)
    {
        case GPS_INIT_READY_STATE:
            if(MMIGPS_START_TYPE_NONE == g_gps_data.start_param.start_type)
            {
                g_gps_data.cur_gps_state = GPS_STARTING_STATE;
                MNGPS_StartGpsReq(dual_sys);
            }else
            {
                g_gps_data.cur_gps_state =GPS_STARTING_STATE;
                MNGPS_StartGpsReqEx(dual_sys,g_gps_data.start_param.start_type,g_gps_data.start_param.start_mode);
            }
            break;
        case GPS_STARTING_STATE:
            g_gps_data.is_need_stop_gps = TRUE; //fix Bug 1341686
            g_gps_data.is_need_start_gps = TRUE;
            break;
        case GPS_STARTED_STATE:
        {
            MMIGPS_Stop();//restart gps
            g_gps_data.is_need_start_gps = TRUE;
            break;
        }
        case GPS_STOPED_STATE:
        {
            if(MMIGPS_START_TYPE_NONE == g_gps_data.start_param.start_type)
            {
                MNGPS_StartGpsReq(dual_sys);
            }else
            {
                MNGPS_StartGpsReqEx(dual_sys,g_gps_data.start_param.start_type,g_gps_data.start_param.start_mode);
            }
            break;
        }
        default:
        {
            TRACE_APP_GPS("gps current state is :%d, can not start", g_gps_data.cur_gps_state);
            break;
        }
    }
   return MMIGPS_RES_SUCC;
}

/*****************************************************************************/
//  Description : request for stop gps or powser off gps
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIGPS_RES_E MMIGPS_Stop(void)
{
    ERR_MNPHONE_CODE_E result_status = ERR_MNPHONE_NO_ERR;
    MNGPS_STATUS_INFO_T cur_status = {0};
    TRACE_APP_GPS("g_gps_data.cur_gps_state:%d",g_gps_data.cur_gps_state);
    switch(g_gps_data.cur_gps_state)
    {
        case GPS_IDLE_STATE:
        case GPS_STOPED_STATE:
        {
            return MMIGPS_RES_STATE_WRONG;
        }
        case GPS_STARTING_STATE:
        case GPS_INIT_READY_STATE:
        {
            g_gps_data.is_need_stop_gps = TRUE;
            break;
        }
        case GPS_STARTED_STATE:
        {
            MNGPS_StopGpsReq(g_gps_data.dul_sys);
            break;
        }
        default:
        {
            return MMIGPS_RES_STATE_WRONG;
        }
    }
    return MMIGPS_RES_SUCC;
}

PUBLIC MMIGPS_STATUS_EUM MMIGPS_CurrentStatus_Get(void)
{
    return g_gps_data.cur_gps_state;
}
/*****************************************************************************/
//  Description : power off gps
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGPS_PowerOff(void)
{
    ERR_MNPHONE_CODE_E result_status = ERR_MNPHONE_NO_ERR;
    MNGPS_STATUS_INFO_T cur_status = {0};
    TRACE_APP_GPS("g_gps_data.cur_gps_state:%d",g_gps_data.cur_gps_state);
    switch(g_gps_data.cur_gps_state)
    {
        case GPS_STOPED_STATE:
        case GPS_INIT_READY_STATE:
            MNGPS_PowerOffReq(MN_SYS_SIM_1);
            break;
        case GPS_STARTING_STATE:
        case GPS_DOWNLOAD_STATE:
        case GPS_STARTED_STATE:
        default:
            break;
    }
}
/*****************************************************************************/
//  Description : set gnss mode for gps
//  Global resource dependence :
//  [IN]: gnss_mode
//  [OUT]: none
//  [RETRUN]: none
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGPS_GnssMode_Set(MN_GPS_GNSS_MODE_E app_gnss_mode)
{
    g_gps_gnss_mode = app_gnss_mode;

    MNGPS_SetGnssModeReq(app_gnss_mode);
}
/*****************************************************************************/
//  Description : release pdp
//  Global resource dependence :
//  [IN]: none
//  [OUT]: none
//  [RETRUN]: none
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_ReleasePdp(void)
{
    MMIAPIPDP_Deactive(MMI_MODULE_GPS);
}
LOCAL void HandlePdpCallback(MMIPDP_CNF_INFO_T *msg_ptr)
{
    int simindex = 0;
    if(PNULL == msg_ptr)
    {
        return;
    }

    TRACE_APP_GPS("msg_ptr->msg_id = %d.", msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:
        {
            TRACE_APP_GPS("MMIPDP_ACTIVE_CNF.msg_ptr->result = %d,pdp_id:%d,nsapi:%d", msg_ptr->result,msg_ptr->pdp_id, msg_ptr->nsapi);

            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                MNGPS_SendPdpInfoReq(g_gps_data.dul_sys, msg_ptr->pdp_id, msg_ptr->nsapi);
            }
            else
            {
                MMIAPIPDP_Deactive(MMI_MODULE_GPS);
            }
        }
            break;
        case MMIPDP_DEACTIVE_CNF:
            TRACE_APP_GPS("MMIPDP_DEACTIVE_CNF");
            break;

        case MMIPDP_DEACTIVE_IND:
            TRACE_APP_GPS("MMIPDP_DEACTIVE_IND");
            MMIAPIPDP_Deactive(MMI_MODULE_GPS);
            break;
        default:
            break;
        }
}
/*****************************************************************************/
//  Description : set up pdp with different pdp type
//  Global resource dependence :
//  [IN]: pdp type:gprs or wifi
//  [IN]: pdp_callback:set up pdp callback
//  [OUT]: none
//  [RETRUN]: none
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_SetupPdp(MMIPDP_INTERFACE_E pdp_type, MMIPDP_MSG_CALLBACK pdp_callback)
{
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    BOOLEAN return_val = FALSE;
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    uint8 linkNum = 0;
    int i = 0;
    int kk = 0;

    linkNum = MMIAPICONNECTION_GetLinkSettingNum(g_gps_data.dul_sys );
    TRACE_APP_GPS("linkNum = %d.", linkNum);
    for (i = 0; i < linkNum; i++)
    {
        setting_item_ptr = PNULL;
        setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(g_gps_data.dul_sys , i);
        TRACE_APP_GPS("linksetting = %x", setting_item_ptr);
        if(PNULL != setting_item_ptr && 0 != setting_item_ptr->apn_len)
        {
            TRACE_APP_GPS("%d", setting_item_ptr->apn_len);
            for (kk = 0; kk < setting_item_ptr->apn_len; kk++)
            {
                TRACE_APP_GPS("[%d]--%x", kk, setting_item_ptr->apn[kk]);
            }
            TRACE_APP_GPS("hplmn = %d, [%d, %d, %d]", setting_item_ptr->hplmn, setting_item_ptr->plmn.mcc, setting_item_ptr->plmn.mnc, setting_item_ptr->plmn.mnc_digit_num);
            if (PNULL != setting_item_ptr->apn)
            {
                break;
            }
        }
    }
    if(setting_item_ptr != PNULL)
    {
        app_info.app_handler = MMI_MODULE_GPS;
        app_info.apn_ptr = (char*)setting_item_ptr->apn;
        app_info.user_name_ptr = (char*)setting_item_ptr->username;
        app_info.psw_ptr = (char*)setting_item_ptr->password;
        app_info.dual_sys = g_gps_data.dul_sys;
        app_info.priority = 3;
        app_info.ps_service_rat = MN_UNSPECIFIED;
        app_info.handle_msg_callback = pdp_callback;
        app_info.ps_service_type = BROWSER_E;
        app_info.storage = MN_GPRS_STORAGE_ALL;
        app_info.ps_interface = pdp_type;
        return_val = MMIAPIPDP_Active(&app_info);
        TRACE_APP_GPS("set up socket");
    }
}
void GPS_pingCallback(TCPIP_PING_RESULT_E res, uint32 time_delay, TCPIP_PING_HANDLE ping_handle,
	uint8  ttl, char* ipaddr)
{
	if (res == PINGRES_SUCCESS)
	{
        TRACE_APP_GPS("ping sucess, time=%d, ttl=%d.\n", time_delay, ttl);
        TRACE_APP_GPS("set up wifi socket");
        MMIGPS_SetupPdp(MMIPDP_INTERFACE_WIFI,HandlePdpCallback);
	}
    else
    {
        TRACE_APP_GPS("ping failed, time=%d, ttl=%d.\n", time_delay, ttl);
        MMIGPS_SetupPdp(MMIPDP_INTERFACE_GPRS,HandlePdpCallback);
	}
}
LOCAL void GPS_PingExternalNet(void)
{
    char addr[]="www.baidu.com";
    TRACE_APP_GPS("GPS_PingExternalNet,ping www.baidu.com");
    sci_ping_request(addr, sizeof(addr), 1000, GPS_pingCallback, NULL);
}
/*****************************************************************************/
//  Description : handle data socket
//  Global resource dependence :
//  [IN]: signal parm
//  [OUT]: none
//  [RETRUN]: none
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_DataSocketHandle(void *param)
{
    APP_MN_GPS_DATA_SOCKET_IND_T *param_ptr = (APP_MN_GPS_DATA_SOCKET_IND_T *)param;

    if(NULL == param_ptr)
    {
        TRACE_APP_GPS("param_ptr is null !!!");
        return ;
    }

    TRACE_APP_GPS("socket_cmd:%d",param_ptr->dataSocket_type);
    switch(param_ptr->dataSocket_type)
    {
        case MN_GPS_SETUP_DATA_SOCKET:
            //setup datasocket  
            if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
            {
                TRACE_APP_GPS("check wifi socket");
                GPS_PingExternalNet();
            }
            else
            {
                TRACE_APP_GPS("set up gprs socket");
                MMIGPS_SetupPdp(MMIPDP_INTERFACE_GPRS,HandlePdpCallback);
            }
            break;
        case MN_GPS_RELEASE_DATA_SOCKET:
            TRACE_APP_GPS("deactive socket");
            // release data socket
            MMIAPIPDP_Deactive(MMI_MODULE_GPS);
            break;
        default:
            break;
    }

}
/*****************************************************************************/
//  Description : delete aid data for gps test
//  Global resource dependence :
//  [IN]: none
//  [OUT]: none
//  [RETRUN]: none
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_AidData_Delete(void)
{
    MNGPS_DeleteAidDataReq();
}
/*****************************************************************************/
//  Description : registe callback for gps test
//  Global resource dependence :
//  [IN]: call_back: call back funtion for receive msg
//  [OUT]: none
//  [RETRUN]: none
//  Note:
/*****************************************************************************/
PUBLIC void MMIGPS_RegisterCallBack(GPS_REQUEST_CALLBACK call_back)
{
    if( NULL == call_back)
    {
        TRACE_APP_GPS("[GPS]:Input Param call_back is NULL!\n");
        return;
    }
    g_gps_data.gps_request_callback = call_back;
}
/*****************************************************************************/
//  Description : open gps
//  Global resource dependence :
//  RETRUN: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIGPS_RES_E MMIGPS_Open(void)
{
    MMIGPS_RES_E init_res = MMIGPS_RES_STATE_WRONG;
    BOOLEAN gps_on_off_state = TRUE;

    TRACE_APP_GPS("[GPS]:MMIGPS_Open,\n");
    if(GPS_DOWNLOAD_STATE == MMIGPS_CurrentStatus_Get())
    {
        TRACE_APP_GPS("[GPS]:open gps, that is init gps\n");
        //MMINV_WRITE(MMINV_SET_GPS_ON_OFF,&gps_on_off_state);
        init_res = MMIGPS_Init();
        TRACE_APP_GPS("[GPS]:init gps res:%d\n", init_res);
    }
}
/*****************************************************************************/
//  Description : close gps
//  Global resource dependence :
//  RETRUN:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGPS_Close(void)
{
    BOOLEAN gps_on_off_state = FALSE;
    MMIGPS_STATUS_EUM gps_state = MMIGPS_CurrentStatus_Get();

    TRACE_APP_GPS("[GPS]:close gps, that is poweroff gps\n");

    //MMINV_WRITE(MMINV_SET_GPS_ON_OFF,&gps_on_off_state);
    if(GPS_STOPED_STATE == gps_state ||GPS_INIT_READY_STATE == gps_state)
    {
        MMIGPS_PowerOff();
    }
}
/*****************************************************************************/
//  Description : get gps on/off state
//  Global resource dependence :
//  RETRUN:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIGPS_OnOffState_Get(void)
{
    BOOLEAN gps_on_off_state = FALSE;
    MMI_RESULT_E   eResult = MMI_RESULT_TRUE;

    MMINV_READ(MMINV_SET_GPS_ON_OFF, &gps_on_off_state, eResult);
    if (MN_RETURN_SUCCESS != eResult)
    {
        TRACE_APP_GPS("[GPS]:gps_on_off_state read nv fail, rewrite it\n",gps_on_off_state);
        gps_on_off_state = FALSE;
        MMINV_WRITE(MMINV_SET_GPS_ON_OFF, &gps_on_off_state);
    }

    TRACE_APP_GPS("[GPS]:gps_on_off_state in nv :%d\n",gps_on_off_state);
    return gps_on_off_state;
}

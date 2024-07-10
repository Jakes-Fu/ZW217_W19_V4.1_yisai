/****************************************************************************
** File Name:       mmidm_task.C                                               *
** Author:                                                                 *
** Date:           25/12/2007                                            *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the dm task           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 25/12/2007     yi.su              this file is main task of dm  
****************************************************************************/
#define _MMIDM_TASK_C_
//#include "std_header.h"

#include "mmi_app_dm_trc.h"
#ifdef DM_SUPPORT
#include "mmi_common.h"
#include "mmi_nv.h"
#include "sci_service.h"
//#include "mn_events.h"
//#include "sig_code.h"
#include "mn_type.h"
#include "mmidm_config.h"
#include "os_api.h"
#include "mn_api.h"
#include "mmidm_nv.h"
#include "mmk_timer.h"
#include "mmidm_app.h"
#include "mmidm_internal.h"
#include "in_message.h"
#include "app_tcp_if.h"
#include "mmipdp_export.h"
#ifdef DM_SUPPORT_DFRESH
#include "mmidm_parsexml.h"
#endif
#include "priority_system.h"
#include "mmi_nv.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define     DM_STACK_SIZE                 (1024*30)           //dm task stack size
#define     DM_STACK_QUEUE_NUM            (120)               //dm task stack queue number
#define     DM_TASK_PRIORITY            PRI_DM_TASK  // (30)               //dm task priority
#define     DM_PDP_WAITING_TIME           1000             //dm waiting pdp time
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL  uint32           s_dm_task_id = 0;                     //dm task id
LOCAL  MMIDM_PDP_STATUS_E   s_pdp_status = MMIDM_PDP_STATUS_NONE; //0:none;1:start;2:connect;3:abort
LOCAL  MMIDM_PDP_CONNECT_TYPE s_connect_type = CONNECT_CMWAP;
LOCAL  uint32            s_pdp_net_id = 0;
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle mms pdp msg 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleDMPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle pdp active cnf
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void HandlePdpActiveCnf(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : get pdp status
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL void SetPdpActiveStatus(MMIDM_PDP_STATUS_E   pdp_status);

/*****************************************************************************/
//  Description : set pdp connect type
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetPdpConnectType(char *apn);

/*****************************************************************************/
//  Description : handle mms pdp msg 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleDMPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == msg_ptr)
    {
        //SCI_TRACE_LOW:"MMIDM==> HandleDMPDPMsg PNULL == msg_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_82_112_2_18_2_14_25_824,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMIDM==> HandleDMPDPMsg msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_85_112_2_18_2_14_25_825,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
        HandlePdpActiveCnf(msg_ptr);
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        SetPdpActiveStatus(MMIDM_PDP_STATUS_NONE);
        break;
        
    case MMIPDP_DEACTIVE_IND:
        SetPdpActiveStatus(MMIDM_PDP_STATUS_NONE);
        break;
        
    default:
        break;
    }
    //SCI_FREE(msg_ptr);
}
/*****************************************************************************/
//  Description : set pdp connect type
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIDM_PDP_CONNECT_TYPE MMIDM_GetPdpConnectType(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetPdpConnectType %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_111_112_2_18_2_14_25_826,(uint8*)"d",s_connect_type);
    return s_connect_type;
}
/*****************************************************************************/
//  Description : set pdp connect type
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetPdpConnectType(char *apn)
{

    if (PNULL == apn)
    {
        s_connect_type = CONNECT_UNKNOW;
        //SCI_TRACE_LOW:"MMIDM==> SetPdpConnectType CONNECT_UNKNOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_124_112_2_18_2_14_25_827,(uint8*)"");
        return;
    }
    if(!memcmp(apn,DM_APN_CMWAP_INFO,strlen(DM_APN_CMWAP_INFO)))
    {
        s_connect_type = CONNECT_CMWAP;
    }
    else if(!memcmp(apn,DM_APN_CMNET_INFO,strlen(DM_APN_CMNET_INFO)))
    {
        s_connect_type = CONNECT_CMNET;
    }
    else
    {
        s_connect_type = CONNECT_UNKNOW;
    }
    //SCI_TRACE_LOW:"MMIDM==> SetPdpConnectType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_139_112_2_18_2_14_25_828,(uint8*)"d",s_connect_type);
}
/*****************************************************************************/
//  Description : active pdp context
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ActivePdpContext(void)
{
#ifndef WIN32   
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MN_DUAL_SYS_E dual_sys = MMIDM_GetMainSim();
    char               dm_apn[MMIDM_APN_MAX_LEN] = {0};
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    //SCI_MEMCPY(dm_apn,"cmwap",5);
    MMINV_READ(MMINV_DM_APN, &dm_apn, return_value);
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_ActivePdpContext dm_apn=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_154_112_2_18_2_14_25_829,(uint8*)"s",dm_apn);
    
    SetPdpConnectType(&dm_apn);/*lint !e64 */

    app_info.app_handler = MMI_MODULE_DM;
    //app_info.auth_type = MN_PAP;
    app_info.apn_ptr = dm_apn;
    app_info.user_name_ptr = "";
    app_info.psw_ptr = "";
    app_info.dual_sys = dual_sys;
    app_info.priority = 3;
    app_info.ps_service_type = BROWSER_E;
    app_info.ps_service_rat = MN_UNSPECIFIED;
    app_info.handle_msg_callback = HandleDMPDPMsg;
    app_info.storage = MN_GPRS_STORAGE_ALL;

    SetPdpActiveStatus(MMIDM_PDP_STATUS_START);

    if(!MMIAPIPDP_Active(&app_info))
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ActivePdpContext fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_174_112_2_18_2_14_25_830,(uint8*)"");
    }
#else
    char               dm_apn[MMIDM_APN_MAX_LEN] = {0};
    SCI_MEMCPY(dm_apn,"cmwap",5);
    SetPdpActiveStatus(MMIDM_PDP_STATUS_CONNECT);
#endif
}

/*****************************************************************************/
//  Description : active pdp context
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_DeactivePdpContext(void)
{
    if (!MMIAPIPDP_Deactive(MMI_MODULE_DM))
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIAPIPDP_Deactive fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_191_112_2_18_2_14_26_831,(uint8*)"");
    }
    SetPdpActiveStatus(MMIDM_PDP_STATUS_NONE);
}

/*****************************************************************************/
//  Description : active pdp connect
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ActivePdpConnect(void)
{
    BOOLEAN result = FALSE;
    int32   time     = 0;
    
    if(MMIDM_PDP_STATUS_CONNECT !=MMIDM_GetPdpActiveStatus())//未连接状态下，激活PDP进行连接
    {    
        MMIDM_ActivePdpContext();
        while(TRUE)/*lint !e716 */
        {
            if (MMIDM_PDP_STATUS_CONNECT == MMIDM_GetPdpActiveStatus())
            {
                result = TRUE;
                break;
            }
            else if(MMIDM_PDP_STATUS_ABORT == MMIDM_GetPdpActiveStatus()||
                MMIDM_PDP_STATUS_NONE == MMIDM_GetPdpActiveStatus())
            {
                result = FALSE;
                break;
            }
            else
            {
                //其他状态不处理，等待状态变化
            }
            SCI_Sleep(DM_PDP_WAITING_TIME);
            
            time++;
            if(time >= 30)
            {
                //SCI_TRACE_LOW:"MMIDM==> MMIDM_ActivePdpConnect TIME OUT"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_230_112_2_18_2_14_26_832,(uint8*)"");
                return FALSE;
            }
        }
    }
    else
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : handle pdp active cnf
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void HandlePdpActiveCnf(MMIPDP_CNF_INFO_T *msg_ptr)
{   
    //SCI_TRACE_LOW:"MMIDM==> pdp active result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_248_112_2_18_2_14_26_833,(uint8*)"d",msg_ptr->result);

    s_pdp_net_id = msg_ptr->nsapi;
    //get pdp active cnf 
    if (MMIPDP_RESULT_SUCC == msg_ptr->result)
    {
        //pdp active success
        SetPdpActiveStatus(MMIDM_PDP_STATUS_CONNECT);
    }
    else
    {
        SetPdpActiveStatus(MMIDM_PDP_STATUS_ABORT);
    }

}

/*****************************************************************************/
//  Description : handle dm task
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void DM_TaskHandle(uint32 argc, void * argv)
{
    DM_SIGNAL_T                 *signal_ptr = PNULL;
   // BLOCK_ID                    task_id = s_dm_task_id;
    //SOCKET_CONNECT_EVENT_IND_SIG_T*       connect_ind_sig_ptr = PNULL;
    uint32                      sock_id = 0;
//    uint32                      sock_err = 0;
#ifdef DM_SUPPORT_REDBENCH
    DM_START_RESULT_E           result = DM_START_NONE;
#endif    
    static   BOOLEAN            is_on_free= FALSE;;
    //SCI_TRACE_LOW:"DM:task start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_279_112_2_18_2_14_26_834,(uint8*)"");
 
    while(TRUE)/*lint !e716 */
    {
        signal_ptr = (DM_SIGNAL_T*)SCI_GetSignal(SCI_IdentifyThread());
        if (PNULL != signal_ptr)
        {
            //SCI_TRACE_LOW:"MMIDM==> task cycle, signal code = 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_286_112_2_18_2_14_26_835,(uint8*)"d",signal_ptr->SignalCode );

            switch(signal_ptr ->SignalCode) 
            {
            case DM_TASK_START_MESSAGE:
                 //SCI_TRACE_LOW:"MMIDM==> task DM_TASK_START_MESSAGE "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_291_112_2_18_2_14_26_836,(uint8*)"");
#ifdef DM_SUPPORT_DFRESH                
                 MMIDM_BeginDmTansport();
#elif defined(DM_SUPPORT_REDBENCH)
                 result = MMIDM_StartVDM(signal_ptr->content.type,signal_ptr->content.msg_body,signal_ptr->content.msg_size);
                 //SCI_TRACE_LOW:"MMIDM==> task DM_TASK_START_MESSAGE result=%d"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_296_112_2_18_2_14_26_837,(uint8*)"d",result);
                 switch(result)
                 {
                    case DM_START_FAIL:
                        MMIDM_SetDmState(DM_CANCEL);
                        MMIDM_CloseWaitingWin();
                        MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
             	        break;
                    case DM_START_DONE:
                        MMIDM_CloseWaitingWin();
                        MMIDM_SendSigToDmTask(DM_TASK_EXIT_MESSAGE,MMIDM_GetDmTaskID(),PNULL);
                        break;
                    default:
                        break;
                 }
                 if (PNULL!=signal_ptr->content.msg_body)
                 {
                    SCI_FREE(signal_ptr->content.msg_body);
                 }                 
#endif

                 break;
            case DM_TASK_RUN_MESSAGE:
                 //SCI_TRACE_LOW:"MMIDM==> task DM_TASK_RUN_MESSAGE "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_319_112_2_18_2_14_26_838,(uint8*)"");
#ifdef DM_SUPPORT_DFRESH               
                 MMIDM_SetDmState(DM_RUN);
                 MMIDM_SendDmData();
#elif defined(DM_SUPPORT_REDBENCH)
                 if (DM_NONE != MMIDM_GetDmState() && !is_on_free)
                 {
                     MMIDM_Run();
                 }                 
#endif                 
                 break;
            case DM_TASK_DM_CANCEL:
                //SCI_TRACE_LOW:"MMIDM==> task DM_TASK_DM_CANCEL "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_331_112_2_18_2_14_26_839,(uint8*)"");
                MMIDM_SetDmState(DM_CANCEL);
                break;
            case DM_TASK_DM_CLOSE://client to abort
                //SCI_TRACE_LOW:"MMIDM==> task DM_TASK_DM_CLOSE "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_335_112_2_18_2_14_26_840,(uint8*)"");
#ifdef DM_SUPPORT_DFRESH
               MMIDM_setSessionStep(STEP_ABORT);
#elif defined(DM_SUPPORT_REDBENCH)
                if(DM_CANCEL == MMIDM_GetDmState())
                {
                    MMIDM_ExitDM();
                }
#endif
                break;
#ifdef DM_SUPPORT_REDBENCH
            case DM_TASK_DL_CANCEL:
                 MMIDM_SetDlState(DM_CANCEL);
                 break;
            case DM_TASK_DL_CLOSE:
                if(DM_CANCEL == MMIDM_GetDlState())
                {   
                    MMIDM_ExitDM();
                    MMIDM_SetDmState(DM_CANCEL);
                }
                break;
#endif                

            case DM_TASK_DM_EXIT:
#ifdef DM_SUPPORT_DFRESH
                //SCI_TRACE_LOW:"MMIDM==> task DM_TASK_DM_EXIT "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_360_112_2_18_2_14_26_841,(uint8*)"");
                MMIDM_SetDmState(DM_CANCEL);//server need to close
               // MMIDM_DeactivePdpContext();
                MMIDM_EndDm();
                MMIDM_ExitDM();
                if (MMIDM_IsNeedReset())
                {
                    MMIAPIPHONE_PowerReset();
                }
                MMIDM_SendSignalToMMITask(APP_DM_MESSAGE_IND,MSG_DM_SESSION_EXIT,PNULL);
#elif defined(DM_SUPPORT_REDBENCH)
                MMIDM_ExitDM();

#endif
                break;
            case DM_TASK_DM_BROKEN:
                MMIDM_BrokenDMSession();
                break;
            case DM_TASK_EXIT_MESSAGE:
                 SCI_Sleep(100);
                 //SCI_TRACE_LOW:"MMIDM==> task DM_TASK_EXIT_MESSAGE "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_380_112_2_18_2_14_26_842,(uint8*)"");
                 if (DM_NONE != MMIDM_GetDmState()&&!is_on_free)
                 {
                    is_on_free = TRUE;
#ifdef DM_SUPPORT_DFRESH                  
                    MMIDM_EndDm();
#endif                    
                    MMIDM_FreeAllMOInstance();
                    MMIDM_Destroy();
                    MMIDM_DeactivePdpContext();
                    MMIDM_DeleteAllResumeFile();
                    MMIDM_SetDmState(DM_NONE);
                    is_on_free = FALSE;
                    if (MMIDM_IsNeedReset())
                    {
                        MMIAPIPHONE_PowerReset();
                    }
                    MMIDM_SendSignalToMMITask(APP_DM_MESSAGE_IND,MSG_DM_SESSION_EXIT,PNULL);
                 }
                 break;

            case SOCKET_READ_EVENT_IND:
                 sock_id = ((SOCKET_READ_EVENT_IND_SIG_T*)signal_ptr)->socket_id;
                 //SCI_TRACE_LOW:"MMIDM==> task SOCKET_READ_EVENT_IND "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_403_112_2_18_2_14_26_843,(uint8*)"");
                 MMIDM_HandleSocketMessage(SOCKET_STATE_READ,sock_id);
                break;
            case SOCKET_WRITE_EVENT_IND:
                 sock_id = ((SOCKET_WRITE_EVENT_IND_SIG_T*)signal_ptr)->socket_id;
                 //SCI_TRACE_LOW:"MMIDM==> task SOCKET_WRITE_EVENT_IND "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_408_112_2_18_2_14_26_844,(uint8*)"");
                 MMIDM_HandleSocketMessage(SOCKET_STATE_WRITE,sock_id); 
                 break;
            case SOCKET_CONNECT_EVENT_IND:
                 sock_id = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)signal_ptr)->socket_id;
                 //SCI_TRACE_LOW:"MMIDM==> task SOCKET_CONNECT_EVENT_IND "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_413_112_2_18_2_14_26_845,(uint8*)"");
                 if (0==((SOCKET_CONNECT_EVENT_IND_SIG_T*)signal_ptr)->error_code)
                 {
                     MMIDM_HandleSocketMessage(SOCKET_STATE_CONNECT,sock_id);
                 }
                 else
                 {
                     MMIDM_HandleSocketMessage(SOCKET_STATE_CLOSE,sock_id);
                 }
                 break;
            case SOCKET_CONNECTION_CLOSE_EVENT_IND:
                 sock_id = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)signal_ptr)->socket_id;
                 MMIDM_HandleSocketMessage(SOCKET_STATE_CLOSE,sock_id);
                 break;
            default:
                 break;
            }

            SCI_FREE(signal_ptr); 
        }

        if (!s_dm_task_id)
        {
            SCI_ThreadExit();
        }
    }

}

/*****************************************************************************/
//  Description : create dm task
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDM_CreateTask(void)
{
    if (0 != s_dm_task_id)
    {
        //之前创建的task还没有关闭!
        //SCI_TRACE_LOW:"MMIDM==> DM_CreateTask, task hasn't been closed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_451_112_2_18_2_14_26_846,(uint8*)"");
        return 0;
    }
    
    s_dm_task_id = SCI_CreateThread( 
                        "T_P_DM", 
                        "Q_P_DM", 
                        DM_TaskHandle, 
                        0, 
                        0, 
                        DM_STACK_SIZE, 
                        DM_STACK_QUEUE_NUM, 
                        DM_TASK_PRIORITY, 
                        SCI_PREEMPT, 
                        SCI_AUTO_START);
    
    //SCI_TRACE_LOW:"MMIDM==> DM_CreateTask, task id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_467_112_2_18_2_14_26_847,(uint8*)"d",s_dm_task_id);
    return s_dm_task_id;
}

/*****************************************************************************/
//  Description : get Dm task id             
//  Global resource dependence : 
//  Author: jian.ma
//  Note:   
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetDmTaskID(void)
{
    return s_dm_task_id;
}


/*****************************************************************************/
//  Description : get pdp status
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIDM_PDP_STATUS_E  MMIDM_GetPdpActiveStatus(void)
{
    //SCI_TRACE_LOW:"MMIDM==> get pdp status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_487_112_2_18_2_14_26_848,(uint8*)"d",s_pdp_status);
    return(s_pdp_status);
}
/*****************************************************************************/
//  Description : set pdp status
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void  SetPdpActiveStatus(MMIDM_PDP_STATUS_E   pdp_status)
{
    //SCI_TRACE_LOW:"MMIDM==> set pdp status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_TASK_496_112_2_18_2_14_26_849,(uint8*)"d",pdp_status);
    s_pdp_status = pdp_status;
}
/*****************************************************************************/
//  Description : get pdp net id
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetPdpNetID(void)
{
    return s_pdp_net_id;
}
#endif

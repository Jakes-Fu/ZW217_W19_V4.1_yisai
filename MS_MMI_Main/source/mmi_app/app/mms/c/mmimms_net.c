/****************************************************************************
** File Name:      mmimms_edit.c                                           *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
#include "mmimms_net.h"
#include "mmimms_edit.h"
#include "mmimms_file.h"
#include "mmimms_id.h"
#include "mmimms_push.h"
#include "mmi_appmsg.h"
#include "mmimms_setting.h"
#include "mmiconnection_export.h"
#include "mmi_module.h"
#include "mmimms_control_main.h"
#include "mmimms_internal.h"
#include "mmipdp_export.h"
#include "sysMemFile.h"
#include "mmi_signal_ext.h"
#include "mmicc_export.h"
#include "mmiphone_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmieng.h"
#include "mmieng_export.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMS_NET_TIME_OUT           10000
#define MMS_RECV_RETRY_MAX_TIMES   5
#define MMIMMS_MCOMPANY_USER_AGENGT  "MAUI WAP Browser"
#define MMIMMS_404_4_USER_AGENGT  "MicromaxQ50/Q03C MAUI-browser/MIDP-2.0Configuration/CLDC-1.1"
#define MMIMMS_404_4_USER_AGENGT_PROFILE "MicromaxQ50"

#define MMS_SEND_RETRY_MAX_TIMES   3
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
typedef struct 
{
    uint16 mcc;
    uint16 mnc;
}UNINOR_MCC_MNC_T;

static const UNINOR_MCC_MNC_T s_uninor_mcc_mnc_table[] = 
{
    {405,813},
    {405,814},
    {405,815},
    {405,816},
    {405,817},
    {405,818},
    {405,819},
    {405,820},
    {405,821},
    {405,822},
    {405,844},
    {405,875},
    {405,876},
    {405,877},
    {405,878},
    {405,879},
    {405,880},
    {405,925},
    {405,926},
    {405,927},
    {405,928},
    {405,929},
}; 

static const UNINOR_MCC_MNC_T s_mcompany_ua_mcc_mnc_table[] = 
{
    {250,20}    //俄罗斯Tele2网络彩信需要修改ua
    ,{404,4}    // cr147299 需要修改ua
};

static const char* s_mcompany_ua_table[] = 
{
    MMIMMS_404_4_USER_AGENGT
    ,MMIMMS_404_4_USER_AGENGT  // cr147299
};

static const char* s_mcompany_ua_profile_table[] = 
{
    MMIMMS_404_4_USER_AGENGT_PROFILE
    ,MMIMMS_404_4_USER_AGENGT_PROFILE  // cr147299
};

LOCAL MMIMMS_PROGRESS_T  s_activemms_info = {-1, 0, 0, 0, 0, 0, 1};/*lint !e64*/
uint8  g_mms_recv_timer = 0;
static uint8 s_recv_retry_time = 0;     //彩信重复次数
static uint8 s_send_retry_pdp_time = 0;
BOOLEAN s_is_send_retry_timer = FALSE;
BOOLEAN g_is_pdp_active_timer = FALSE;        //timer时间到是否增加timer
LOCAL BOOLEAN s_switch_simcard_pdp_active = FALSE;
//LOCAL MMIMMS_STATUS_E s_net_op_status = MMIMMS_STATUS_NONE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 彩信网络回调函数 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void MMSNetCallback(
                          int32 status,
                          void *prompt,
                          int32 status_value
                          );

/*****************************************************************************/
//  Description : send old mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void SendOldMMS(
                      MMIMMS_PROGRESS_T *activemms_info_ptr
                      );

/*****************************************************************************/
//  Description : receive mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void ReceiveMMS(
                      MMIMMS_PROGRESS_T *activemms_info_ptr
                      );

/*****************************************************************************/
//  Description : active pdp for mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_PDPActive(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : deactive pdp for mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_PDPDeactive(void);

/*****************************************************************************/
//  Description : active pdp for mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPActiveCnf(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle pdp deactive ind
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPDeactiveInd(void);

/*****************************************************************************/
//  Description : ResetActiveMMSInfo 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void ResetActiveMMSInfo(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 彩信网络回调函数 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void MMSNetCallback(
                          int32 status,
                          void *prompt,
                          int32 status_value
                          )
{
    uint8 fname[MMIMMS_FILE_NAME_LEN] = {0};
    static MMIMMS_MSG_SEND_END_T    send_end_info = {0};
    static MMIMMS_MSG_RECV_END_T    recv_end_info = {0};
    MMISMS_ORDER_INFO_T* item_ptr = PNULL;
    BOOLEAN is_mms_exist = FALSE;
    BOOLEAN is_recv_sik_ok = TRUE;
    //SCI_TRACE_LOW:"[MMIMMS]:MMSNetCallback status=%d, status_value=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_181_112_2_18_2_38_30_219,(uint8*)"dd",status, status_value);

    switch(status)
    {
    case MMS_STATUS_NORMAL:
        //可以在此处在状态栏上显示当前状况，如正在连接网关，正在发送数据，正在接收数据等
        //是正常处理过程中的状态显示。
        {
            switch(status_value)
            {
            case MMS_NETLINKING:            
                break;
                
            case MMS_NETLINK_SUCCESS:   //pdp激活成功
                s_activemms_info.status = MMIMMS_STATUS_NETLINK_SUCCESS;
                MMIMMS_ControlToUIMsg(MSG_MMS_NETLINK_SUCCESS,&s_activemms_info);
                break;

            case MMS_SOCKET_CONNECTING:
                break;
                
            case MMS_CONNECT_SUCCESS:
                s_activemms_info.status = MMIMMS_STATUS_NETCONNET_SUCCESS;
                MMIMMS_ControlToUIMsg(MSG_MMS_NETCONNECT_SUCCESS,&s_activemms_info);
                break;
                
            case  MMS_RECEIVING:
                if(MMIMMS_SENDRECV_RECVING != s_activemms_info.type)
                {
                    //SCI_TRACE_LOW:"MMSNetCallback MMS_RECEIVING fetal error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_210_112_2_18_2_38_30_220,(uint8*)"");
                }
                s_activemms_info.status = MMIMMS_STATUS_DATA_TRANSFERING;
                MMIMMS_ControlToUIMsg(MSG_MMS_NETCONNECT_SUCCESS,&s_activemms_info);
                break;
                
            case  MMS_SENDING:
                if(MMIMMS_SENDRECV_SENDING != s_activemms_info.type)
                {
                    //SCI_TRACE_LOW:"MMSNetCallback MMS_SENDING fetal error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_219_112_2_18_2_38_30_221,(uint8*)"");
                }
                s_activemms_info.status = MMIMMS_STATUS_DATA_TRANSFERING;
                MMIMMS_ControlToUIMsg(MSG_MMS_NETCONNECT_SUCCESS,&s_activemms_info);
                break;
                
            default:
                break;
            }
        }
        break;
        
    case  MMS_STATUS_SEND_RETURN:               
            //SCI_TRACE_LOW:"MMSNetCallback fetal error MMS_STATUS_SEND_RETURN MMIMMS_SENDRECV_SENDING != s_activemms_info.type"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_232_112_2_18_2_38_30_222,(uint8*)"");
            send_end_info.record_id = s_activemms_info.record_id;

            if (MMIMMS_SENDRECV_SENDRPT == s_activemms_info.type
                || MMIMMS_SENDRECV_SENDRESPONSE == s_activemms_info.type)
            {
                send_end_info.is_saved = TRUE;
            }
            else
            {
                send_end_info.is_saved = MMIMMS_IsRecordIDExist(s_activemms_info.record_id);
            }
            
            switch(status_value)
            {
            case MMS_RESPONSE_STATUS_VALUE_OK:
                if(send_end_info.is_saved)
                {
                    MMIMMS_ChangeRecordType(send_end_info.record_id,MMISMS_MO_SEND_SUCC);
                }
                send_end_info.send_error = MMIMMS_NO_ERROR;
                break;       
            case MMS_RESPONSE_STATUS_VALUE_E_UNSPECIFIED:
                send_end_info.send_error = MMIMMS_ERROR_FAIL;   //操作失败(未知错误)
                break;
            case MMS_RESPONSE_STATUS_VALUE_E_SERVICE_DENIED:
                send_end_info.send_error = MMIMMS_ERROR_SERVICE_DENIED;
                break;
            case MMS_RESPONSE_STATUS_VALUE_E_FORMAT_CORRUPT:
                send_end_info.send_error = MMIMMS_ERROR_FORMAT_CORRUPT;
                break;   
            case MMS_RESPONSE_STATUS_VALUE_E_ADDRESS_UNRESOLVED:
                send_end_info.send_error = MMIMMS_ERROR_ADDRESS_UNRESOLVED;
                break;
            case MMS_RESPONSE_STATUS_VALUE_E_MESSAGE_NOT_FOUND:
                send_end_info.send_error = MMIMMS_ERROR_MESSAGE_NOT_FOUND;
                break;
            case MMS_RESPONSE_STATUS_VALUE_E_NETWORK_PROBLEM:
                send_end_info.send_error = MMIMMS_ERROR_SERVER_BUSY;
                break;
            case MMS_RESPONSE_STATUS_VALUE_E_NOT_ACCEPTED:
                send_end_info.send_error = MMIMMS_ERROR_NOT_ACCEPTED;
                break;
            case MMS_RESPONSE_STATUS_VALUE_E_UNSUPORTED_MESSAGE:
                send_end_info.send_error = MMIMMS_ERROR_UNSUPORTED_MESSAGE;
                break;
            case MMS_RESPONSE_STATUS_VALUE_E_UNKNOW_ERROR:
                send_end_info.send_error = MMIMMS_ERROR_FAIL;   //操作失败(未知错误)
                break;
                
            case MMS_NETLINK_FAILED:         //网络拨号失败
                send_end_info.send_error = MMIMMS_ERROR_NETLINK_FAILED;
                _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",send_end_info.record_id);
                sysVFremove((char *)fname);
                break;
                
#ifdef FDN_SUPPORT
            case MMS_NETLINK_FDN_ONLY:         //网络拨号失败,prompt "FDN only"
                send_end_info.send_error = MMIMMS_ERROR_NETLINK_FDN_ONLY;
                _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",send_end_info.record_id);
                sysVFremove((char *)fname);
                break;
#endif

        #ifdef DATA_ROAMING_SUPPORT
            case  MMS_NETLINK_DATA_ROAMING_DISABLE:
            {
				send_end_info.send_error = MMIMMS_ERROR_DATA_ROAMING_DISABLE;
                _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",send_end_info.record_id);
                sysVFremove((char *)fname);				
                break;
            }
        #endif
            case MMS_OPR_CANCEL:     //用户取消||网络突然中断
                if(MMIMMS_STATUS_PDP_DEACTIVE_IND == s_activemms_info.status)
                {
                    send_end_info.send_error = MMIMMS_ERROR_NETLINK_DISCONNECT;
                }
                else
                {
                    send_end_info.send_error = MMIMMS_ERROR_CANCEL;
                }
                break;

            default:
                break;
            }            
            MMS_SendRecvClose();
            //SCI_TRACE_LOW:"MMSNetCallback, mms Send!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_302_112_2_18_2_38_30_223,(uint8*)"");
            //SCI_TRACE_LOW:"MMSNetCallback, mms recv failed s_send_retry_pdp_time = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_303_112_2_18_2_38_30_224,(uint8*)"d",s_send_retry_pdp_time);
            if (   MMIMMS_SENDRECV_SENDING == s_activemms_info.type
                && (MMS_NETLINK_FAILED == status_value ||
                   (MMS_OPR_CANCEL == status_value && MMIMMS_STATUS_PDP_DEACTIVE_IND == s_activemms_info.status))
            #ifdef MMI_GPRS_SWITCH_SUPPORT
                && !MMIAPICONNECTION_isGPRSSwitchedOff()
            #endif
                )
            {
                if (s_send_retry_pdp_time < MMS_SEND_RETRY_MAX_TIMES)
                {
                    s_send_retry_pdp_time ++;
                    s_activemms_info.status = MMIMMS_STATUS_NONE;
                    s_activemms_info.current_size = 0;
                    MMIMMS_ControlToUIMsg(MSG_MMS_SEND_RETRYING,&s_activemms_info);
                    if (0 != g_mms_recv_timer)
                    {
                        MMK_StopTimer(g_mms_recv_timer);
                        g_mms_recv_timer = 0;
                    }
                    g_mms_recv_timer = MMK_CreateTimerCallback(MMS_NET_TIME_OUT, MMIAPIMMS_HandleNetTimer, NULL, FALSE);
                    g_is_pdp_active_timer = FALSE;
                    s_is_send_retry_timer = TRUE;
                }
                else
                {
                    MMIMMS_ControlToUIMsg(MSG_MMS_SEND_END,&send_end_info); 
                    s_send_retry_pdp_time = 0;
                    s_is_send_retry_timer = FALSE;
                    ResetActiveMMSInfo();
                    MMIMMS_DeleteManualListItem(send_end_info.record_id);
                    MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0); //后台收发时要进行触发
#ifdef MMI_SMS_CHAT_SUPPORT                    
                    MMIAPISMS_UpdateResendMMSLabel(send_end_info.record_id);
#endif
                }                
            }
            else
            {
                if (   MMIMMS_SENDRECV_SENDING == s_activemms_info.type
                    || MMIMMS_SENDRECV_SENDRPT == s_activemms_info.type)//除了阅读报告外的状态彩信不上报
                {
                    MMIMMS_ControlToUIMsg(MSG_MMS_SEND_END,&send_end_info);
                } 
                s_send_retry_pdp_time = 0;
                s_is_send_retry_timer = FALSE;
                ResetActiveMMSInfo();
                MMIMMS_DeleteManualListItem(send_end_info.record_id);
                MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0); //后台收发时要进行触发
#ifdef MMI_SMS_CHAT_SUPPORT                    
                MMIAPISMS_UpdateResendMMSLabel(send_end_info.record_id);
#endif
            }
            break;
        
    case  MMS_STATUS_RECV_RETURN:
            if(MMIMMS_SENDRECV_RECVING != s_activemms_info.type)
            {
                //SCI_TRACE_LOW:"MMSNetCallback fetal error MMS_STATUS_SEND_RETURN MMIMMS_SENDRECV_SENDING != s_activemms_info.type"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_351_112_2_18_2_38_30_225,(uint8*)"");
            }
            recv_end_info.record_id = s_activemms_info.record_id;
            
            item_ptr = MMIMMS_GetListItemByID(recv_end_info.record_id);            
            
            if(PNULL != item_ptr &&  MMISMS_MT_NOT_NOTIFY == item_ptr->mo_mt_type)
            {
                recv_end_info.recv_type = MMIMMS_RECV_NEWMMS;
            }
            else
            {
                recv_end_info.recv_type = MMIMMS_RECV_OLDMMS;
            }
            switch(status_value)
            {
            case MMS_RECV_SUCCESS:
                //保存LIST ，解码保存MMS，删除PUSH
                if(MMIMMS_UpdateRecvItem(recv_end_info.record_id))
                {
                    recv_end_info.recv_error = MMIMMS_NO_ERROR;
                }
                else
                {
                    //SCI_TRACE_LOW:"[MMIMMS]:MMSNetCallback MMIMMS_UpdateRecvItem fail"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_375_112_2_18_2_38_30_226,(uint8*)"");
                    recv_end_info.recv_error = MMIMMS_ERROR_NO_SPACE;
                }
                //MMIMMS_ChangeRecordType(recv_end_info.record_id,MMISMS_MT_TO_BE_READ);
                break;
                
            case MMS_RECV_FAILED:
                recv_end_info.recv_error = MMIMMS_ERROR_FAIL;
                break;
                
            case MMS_OPR_CANCEL:
                if(MMIMMS_STATUS_PDP_DEACTIVE_IND == s_activemms_info.status)
                {
                    recv_end_info.recv_error = MMIMMS_ERROR_NETLINK_DISCONNECT;
                    if (!MMIAPIPHONE_IsSimAvailable(s_activemms_info.dual_sys))
                    {
                        is_recv_sik_ok = FALSE;
                    }
                }
                else
                {
                    recv_end_info.recv_error = MMIMMS_ERROR_CANCEL;
                }
                break;
                
            case MMS_NETLINK_FAILED:
                recv_end_info.recv_error = MMIMMS_ERROR_NETLINK_FAILED;
                if (!MMIAPIPHONE_IsSimAvailable(s_activemms_info.dual_sys))
                {
                    is_recv_sik_ok = FALSE;
                }
                break;
#ifdef FDN_SUPPORT
            case MMS_NETLINK_FDN_ONLY:
                recv_end_info.recv_error = MMIMMS_ERROR_NETLINK_FDN_ONLY;
                if (!MMIAPIPHONE_IsSimAvailable(s_activemms_info.dual_sys))
                {
                    is_recv_sik_ok = FALSE;
                }
                break;
#endif                
        //////////////////////////////////////////////////////////////////////////
        // added by feng.xiao
        #ifdef DATA_ROAMING_SUPPORT
            case  MMS_NETLINK_DATA_ROAMING_DISABLE:
            {
                recv_end_info.recv_error = MMIMMS_ERROR_DATA_ROAMING_DISABLE;
                break;
            }
        #endif

            case MMS_RECV_FAILED_BY_NOT_FOUND:
                recv_end_info.recv_error = MMIMMS_ERROR_MESSAGE_NOT_FOUND;
                break;

            case MMS_RECV_FAILED_NO_SPACE:
                recv_end_info.recv_error = MMIMMS_ERROR_NO_SPACE;
                break;

            case MMS_RECV_FAILED_INVALIDPERIOD:
                recv_end_info.recv_error = MMIMMS_ERROR_INVALIDPERIOD;
                break;

            case MMS_RECV_FAILED_INVALID_URL:
                recv_end_info.recv_error = MMIMMS_ERROR_INVALID_URL;
                break;

            case MMS_RECV_FAILED_NEED_AUTH:
                recv_end_info.recv_error = MMIMMS_ERROR_RECV_NEED_AUTH;
                break;

            case MMS_RECV_FAILED_ERROR_DATA:
                recv_end_info.recv_error = MMIMMS_ERROR_RECV_ERROR_DATA;
                break;
            default:
                break;
            }
            
            MMS_SendRecvClose();//不管什么类型的彩信失败都必须先关闭

            //SCI_TRACE_LOW:"MMSNetCallback, mms recv!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_455_112_2_18_2_38_30_227,(uint8*)"");
            if (   !is_recv_sik_ok
                || MMS_RECV_FAILED_BY_NOT_FOUND == status_value
                || MMS_RECV_FAILED_NO_SPACE == status_value
                || MMS_RECV_FAILED_INVALIDPERIOD == status_value
                || MMS_RECV_FAILED_INVALID_URL == status_value
                || MMS_RECV_FAILED_NEED_AUTH == status_value
                || MMS_RECV_FAILED_ERROR_DATA == status_value
                || MMS_RECV_SUCCESS == status_value  //下载成功或者异常失败，或者手动取消，该条彩信流程结束，转为彩信通知
                || (MMS_OPR_CANCEL == status_value && MMIMMS_STATUS_PDP_DEACTIVE_IND != s_activemms_info.status)
            #ifdef DATA_ROAMING_SUPPORT
                || status_value == MMS_NETLINK_DATA_ROAMING_DISABLE //added by feng.xiao
            #endif
            #ifdef FDN_SUPPORT
                || status_value == MMS_NETLINK_FDN_ONLY
            #endif
            #ifdef MMI_GPRS_SWITCH_SUPPORT
                || MMIAPICONNECTION_isGPRSSwitchedOff()
            #endif
               )
            {
                //SCI_TRACE_LOW:"MMSNetCallback, mms recv failed or succ"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_473_112_2_18_2_38_30_228,(uint8*)"");
                ResetActiveMMSInfo();
                if(PNULL != item_ptr && MMISMS_MT_NOT_NOTIFY == item_ptr->mo_mt_type 
                    && ( MMS_RECV_FAILED_BY_NOT_FOUND == status_value
                         || MMS_RECV_FAILED_NO_SPACE == status_value
                         || MMS_RECV_FAILED_NEED_AUTH == status_value
                         || MMS_RECV_FAILED_ERROR_DATA == status_value
                         || MMS_RECV_FAILED_INVALIDPERIOD == status_value
                         || MMS_RECV_FAILED_INVALID_URL == status_value
                    #ifdef DATA_ROAMING_SUPPORT
                         || status_value == MMS_NETLINK_DATA_ROAMING_DISABLE //added by feng.xiao
                    #endif
                    #ifdef FDN_SUPPORT
                        || status_value == MMS_NETLINK_FDN_ONLY
                    #endif
                    #ifdef MMI_GPRS_SWITCH_SUPPORT
                        || MMIAPICONNECTION_isGPRSSwitchedOff()
                    #endif
                        )
                   )
                {
                    item_ptr->mo_mt_type = MMISMS_MT_NEED_NOT_DOWNLOAD;
                    MMIMMS_SaveMMSListFile();
                }
                MMIMMS_ControlToUIMsg(MSG_MMS_RECV_END,&recv_end_info);
                MMIMMS_DeleteManualListItem(recv_end_info.record_id);
                MMIMMS_DeleteAutoListItem(recv_end_info.record_id);
                s_recv_retry_time = 0;
                MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0); 
            } 
            else
            {
                //SCI_TRACE_LOW:"MMSNetCallback, mms recv failed s_recv_retry_time = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_502_112_2_18_2_38_31_229,(uint8*)"d",s_recv_retry_time);
                if (s_recv_retry_time < MMS_RECV_RETRY_MAX_TIMES)//重试的次数小于5次则开启timer重复下载当前彩信
                {                    
                    s_activemms_info.status = MMIMMS_STATUS_NONE;
                    s_activemms_info.current_size = 0;
                    MMIMMS_ControlToUIMsg(MSG_MMS_RECV_RETRYING,&s_activemms_info);
                    if (0 != g_mms_recv_timer)
                    {
                        MMK_StopTimer(g_mms_recv_timer);
                        g_mms_recv_timer = 0;
                    }
                    g_mms_recv_timer = MMK_CreateTimerCallback(MMS_NET_TIME_OUT, MMIAPIMMS_HandleNetTimer, NULL, FALSE);
                    s_recv_retry_time ++;
                    g_is_pdp_active_timer = FALSE;
                    s_is_send_retry_timer = TRUE;
                }
                else
                {
                    //重复五次后写为彩信通知
                    //SCI_TRACE_LOW:"MMSNetCallback, mms recv failed retry download mms"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_520_112_2_18_2_38_31_230,(uint8*)"");
                    ResetActiveMMSInfo();
                    if(PNULL != item_ptr && MMISMS_MT_NOT_NOTIFY == item_ptr->mo_mt_type)
                    {
                        item_ptr->mo_mt_type = MMISMS_MT_NOT_DOWNLOAD;
                        MMIMMS_SaveMMSListFile();
                    }                    
                    MMIMMS_DeleteManualListItem(recv_end_info.record_id);
                    MMIMMS_DeleteAutoListItem(recv_end_info.record_id);
                    MMIMMS_ControlToUIMsg(MSG_MMS_RECV_END,&recv_end_info);
                    s_recv_retry_time = 0;
                    MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
                }
            }
        break;
        
    case MMS_STATUS_SENDED_BYTES:
        if (0 != *(uint32*)prompt)
        {
            s_activemms_info.total_size = *(uint32*)prompt;
        }
        if (0 == status_value)
        {
            //SCI_TRACE_LOW:"MMSNetCallback,  current = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_543_112_2_18_2_38_31_231,(uint8*)"d", s_activemms_info.current_size);
            s_activemms_info.current_size = 0;
        }
        s_activemms_info.current_size += (uint32)status_value;
        //SCI_TRACE_LOW:"MMSNetCallback, total = %d, current = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_547_112_2_18_2_38_31_232,(uint8*)"dd",s_activemms_info.total_size,s_activemms_info.current_size);
        if (s_activemms_info.current_size > s_activemms_info.total_size)
        {
            s_activemms_info.current_size = s_activemms_info.total_size;
        }
        if (MMIMMS_SENDRECV_SENDING == s_activemms_info.type)
        {
            MMIMMS_ControlToUIMsg(MSG_MMS_DATA_SENT,&s_activemms_info);
        }
        break;
        
    case MMS_STATUS_RECVED_BYTES:
        if (0 != *(uint32*)prompt)
        {
            s_activemms_info.total_size = *(uint32*)prompt;           
        }
        //SCI_TRACE_LOW:"MMSNetCallback, prompt = %d, total = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_563_112_2_18_2_38_31_233,(uint8*)"dd",*(uint32*)prompt,s_activemms_info.total_size);
        s_activemms_info.current_size += (uint32)status_value;
        if (0 == *(uint32*)prompt)
        {
            MMIMMS_PUSH_FILE_T *push_info_ptr  = PNULL;

            if (MMIMMS_IsRecordIDExist(s_activemms_info.record_id))
            {
                push_info_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
                if (PNULL != push_info_ptr)
                {
                    //SCI_MEMSET((uint8 *)push_info_ptr, 0 , sizeof(MMIMMS_PUSH_FILE_T));
                    if (MMIMMS_ReadPush(push_info_ptr, s_activemms_info.record_id))
                    {
                        s_activemms_info.total_size = push_info_ptr->size;
                    }
                    SCI_FREE(push_info_ptr);
                    push_info_ptr = PNULL;
                }                
                //SCI_TRACE_LOW:"MMSNetCallback, prompt = %d, total = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_582_112_2_18_2_38_31_234,(uint8*)"dd",*(uint32*)prompt,s_activemms_info.total_size);
            }            
        }
        MMIMMS_ControlToUIMsg(MSG_MMS_DATA_RECEIVED,&s_activemms_info);
        break;

    case MMS_STATUS_SEND_RETRYING:
        s_activemms_info.status = MMIMMS_STATUS_DATA_TRANSFERING;
        s_activemms_info.current_size = 0;
        is_mms_exist = MMIMMS_IsRecordIDExist(s_activemms_info.record_id);
        if (is_mms_exist)
        {
            MMIMMS_ControlToUIMsg(MSG_MMS_SEND_RETRYING,&s_activemms_info);
        }
        break;

    case MMS_STATUS_RECV_RETRYING:
        s_activemms_info.status = MMIMMS_STATUS_DATA_TRANSFERING;
        s_activemms_info.current_size = 0;
        MMIMMS_ControlToUIMsg(MSG_MMS_RECV_RETRYING,&s_activemms_info);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : Reset_ActiveMMSInfo 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void ResetActiveMMSInfo(void)
{
    MN_DUAL_SYS_E pre_sim = MN_DUAL_SYS_MAX;

    pre_sim = s_activemms_info.dual_sys;
    SCI_MEMSET(&s_activemms_info,0,sizeof(MMIMMS_PROGRESS_T));
    s_activemms_info.record_id = -1;
    s_activemms_info.dual_sys = pre_sim;
}

/*****************************************************************************/
//  Description : send old mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void SendOldMMS(
                      MMIMMS_PROGRESS_T *activemms_info_ptr
                      )
{
    uint32   file_size = 0;

    if (PNULL == activemms_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:SendOldMMS PNULL == activemms_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_635_112_2_18_2_38_31_235,(uint8*)"");
        return;
    }
    file_size = MMIMMS_LoadMMSToVfile(activemms_info_ptr->record_id);
    SCI_TRACE_LOW("Mmimms_net.c:SendOldMMS, file_size=%d", file_size);
    if(file_size)
    {
        activemms_info_ptr->total_size = file_size;
        activemms_info_ptr->status = MMIMMS_STATUS_NETLINKING;
        if(MMIMMS_PDPActive(activemms_info_ptr->dual_sys))
        {
            //SCI_TRACE_LOW:"[MMIMMS]:SendOldMMS call pdp active function return true"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_645_112_2_18_2_38_31_236,(uint8*)"");
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMMS]:SendOldMMS PDP active fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_649_112_2_18_2_38_31_237,(uint8*)"");
            MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_NETLINK_FAILED);
        }
    }
    else
    {
        MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_RESPONSE_STATUS_VALUE_E_MESSAGE_NOT_FOUND);
    }
}

/*****************************************************************************/
//  Description : receive mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void ReceiveMMS(
                      MMIMMS_PROGRESS_T *activemms_info_ptr
                      )
{
    MMIMMS_PUSH_FILE_T *push_info_ptr = PNULL;
//  uint8 setting_index = 0;
    MMISMS_ORDER_INFO_T* item_ptr = PNULL;


    item_ptr = MMIMMS_GetListItemByID(activemms_info_ptr->record_id);
    push_info_ptr = (MMIMMS_PUSH_FILE_T *)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
    if (PNULL == push_info_ptr)
    {
        MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RECV_FAILED_NO_SPACE);
        return;
    }
    else
    {
        SCI_MEMSET((uint8*)push_info_ptr, 0, sizeof(MMIMMS_PUSH_FILE_T));
        MMIMMS_ReadPush(push_info_ptr, activemms_info_ptr->record_id);
    }
    
    if(!MMIMMS_IsEnoughSpace(push_info_ptr->size + MMIMMS_RESERVE_LIST_SIZE))
    {
        MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RECV_FAILED_NO_SPACE);
        return;
    }
        
#ifdef WIN32   //for test
    SCI_MEMCPY(push_info_ptr->url,"http://172.16.14.173:8082/mms/newvideo.mms",SCI_STRLEN("http://172.16.14.173:8082/mms/newvideo.mms"));
#endif
    if(!item_ptr || MMIMMS_IsExpire(item_ptr->time,(uint32)push_info_ptr->expiry_time))  //检查是否过期了
    {
        MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RECV_FAILED_INVALIDPERIOD);
    } 
    else if(push_info_ptr->url[0])
    {
        if (!MMIMMS_IsPdpPermitUsing(activemms_info_ptr->dual_sys))//不允许pdp激活情况
        {
            activemms_info_ptr->status = MMIMMS_STATUS_NONE;
            g_is_pdp_active_timer = TRUE;
            if (0 != g_mms_recv_timer)
            {
                MMK_StopTimer(g_mms_recv_timer);
                g_mms_recv_timer = 0;
            }
            g_mms_recv_timer = MMK_CreateTimerCallback(MMS_NET_TIME_OUT, MMIAPIMMS_HandleNetTimer, NULL, FALSE);
        }
        else
        {
            activemms_info_ptr->status = MMIMMS_STATUS_NETLINKING;
            if(MMIMMS_PDPActive(activemms_info_ptr->dual_sys))
            {
                //SCI_TRACE_LOW:"[MMIMMS]:ReceiveMMS call pdp active function return true"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_716_112_2_18_2_38_31_238,(uint8*)"");
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMMS]:ReceiveMMS PDP active fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_720_112_2_18_2_38_31_239,(uint8*)"");
                MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_NETLINK_FAILED);
            }
        }        
    }  
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:ReceiveMMS url is wrong!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_727_112_2_18_2_38_31_240,(uint8*)"");
        MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RECV_FAILED_INVALID_URL);
    }
    SCI_FREE(push_info_ptr);
    push_info_ptr = PNULL;
}

/*****************************************************************************/
//  Description : active receive and send module 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ActiveRecvSendModule(void)
{
    //无后台收发时，作为自动下载的触发
    //后台收发时，作为收发队列的触发
    MMIMMS_AUTOLIST_T *auto_list = PNULL;
    MMIMMS_AUTOLIST_T *manual_list = PNULL;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ActiveRecvSendModule status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_745_112_2_18_2_38_31_241,(uint8*)"d",s_activemms_info.status);
    if(MMIMMS_STATUS_NONE == s_activemms_info.status && !s_is_send_retry_timer)//当彩信发送pdp激活失败进入timer的时候不允许其他先操作
    {
        ResetActiveMMSInfo();
        manual_list = MMIMMS_GetManualListFirstItem();
        auto_list = MMIMMS_GetAutoListFirstItem();
        if(manual_list)
        {
            if (s_activemms_info.dual_sys != manual_list->dual_sys)
            {
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ActiveRecvSendModule pdp deactive different sim card"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_755_112_2_18_2_38_31_242,(uint8*)"");
                if(MMIMMS_PDPDeactive())
                {
                    s_switch_simcard_pdp_active = TRUE;
                    s_activemms_info.dual_sys = manual_list->dual_sys;
                    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ActiveRecvSendModule manual_list MMIMMS_PDPDeactive"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_760_112_2_18_2_38_31_243,(uint8*)"");
                    return;
                }
            }

            s_activemms_info.dual_sys = manual_list->dual_sys;
            s_activemms_info.record_id = manual_list->record_id;
            s_activemms_info.immediate_recv = 0; //非立即接收
            s_activemms_info.type = manual_list->send_type;
            
            if(    MMIMMS_SENDRECV_SENDING == s_activemms_info.type
                || MMIMMS_SENDRECV_SENDRPT == s_activemms_info.type
                || MMIMMS_SENDRECV_SENDRESPONSE == s_activemms_info.type)
            {
                SendOldMMS(&s_activemms_info);
            }
            else if(MMIMMS_SENDRECV_RECVING == s_activemms_info.type)
            {
                ReceiveMMS(&s_activemms_info);
            }
        }
        else if(auto_list)
        {
            if (s_activemms_info.dual_sys != auto_list->dual_sys)
            {
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ActiveRecvSendModule pdp deactive different sim card"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_785_112_2_18_2_38_31_244,(uint8*)"");
                if(MMIMMS_PDPDeactive())
                {
                    s_switch_simcard_pdp_active = TRUE;
                    s_activemms_info.dual_sys = auto_list->dual_sys;
                    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ActiveRecvSendModule auto_list MMIMMS_PDPDeactive"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_790_112_2_18_2_38_31_245,(uint8*)"");
                    return;
                }
            }

            s_activemms_info.dual_sys = auto_list->dual_sys;
            s_activemms_info.record_id = auto_list->record_id;
            s_activemms_info.immediate_recv = 1;// 立即接收
            s_activemms_info.type = auto_list->send_type;
            ReceiveMMS(&s_activemms_info);
        }
        else
        {
            SCI_TRACE_LOW("[MMIMMS]:MMIMMS_ActiveRecvSendModule pdp deactive");  //队列中没有MMS了，去活
            s_switch_simcard_pdp_active = FALSE;
            MMIMMS_PDPDeactive();
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ActiveRecvSendModule ACTIVING"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_809_112_2_18_2_38_31_246,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : send new mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SendNewMMS(
                                        MN_DUAL_SYS_E dual_sys, 
                                        MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                        )
{
    MMIMMS_ERROR_E      return_val = MMIMMS_NO_ERROR;
    uint16              count = 0;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return MMIMMS_ERROR_FATAL;
    }
    
    if (!MMIMMS_IsAllowSendMMSByFDN(dual_sys, editdoc_ptr->edit_to))
    {
        return MMIMMS_ERROR_FDN_NUMBER;
    }

    if (!MMIMMS_IsMMSNumberValid(editdoc_ptr->edit_to))
    {
        return MMIMMS_ERROR_INVALID_ADDRESS;
    }

    count = MMIMMS_GetAllContentCount(editdoc_ptr);
    if (PNULL == editdoc_ptr->editbody_ptr->slide_cur || 0 == count)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SendNewMMS MMIMMS_ERROR_NO_CONTENT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_838_112_2_18_2_38_31_247,(uint8*)"");
        return_val = MMIMMS_ERROR_NO_CONTENT;
    }
    else
    {
        return_val = MMIMMS_SaveMMS(dual_sys, MMISMS_MO_SEND_FAIL, editdoc_ptr);
        if(MMIMMS_NO_ERROR == return_val || MMIMMS_ERROR_NO_SPACE == return_val)
        {
            if(MMIMMS_AddManualList(dual_sys,editdoc_ptr->record_id,MMIMMS_SENDRECV_SENDING))
            {
                MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_SendNewMMS add manual list fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_852_112_2_18_2_38_32_248,(uint8*)"");
                return_val = MMIMMS_ERROR_FAIL;
            }
        }
    }  

    return return_val;
}

/*****************************************************************************/
//  Description : send old mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SendOldMMS(
                                        MN_DUAL_SYS_E     dual_sys, 
                                        int32				record_id
                                        )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;
    if(MMIMMS_AddManualList(dual_sys,record_id,MMIMMS_SENDRECV_SENDING))
    {
        MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_SendOldMMS add manual list fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_877_112_2_18_2_38_32_249,(uint8*)"");
        return_val = MMIMMS_ERROR_FAIL;
    }
    
    return return_val;
}

/*****************************************************************************/
//  Description : send read report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SendReadReport(
                                            MN_DUAL_SYS_E     dual_sys, 
                                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                            )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;
    MMIMMS_EDIT_DOCUMENT_T * reportdoc_ptr = PNULL;
    
    if (!MMIMMS_IsAllowSendMMSByFDN(dual_sys, editdoc_ptr->edit_to))
    {
        return MMIMMS_ERROR_FDN_NUMBER;
    }

    if (!MMIMMS_IsMMSNumberValid(editdoc_ptr->edit_to))
    {
        return MMIMMS_ERROR_INVALID_ADDRESS;
    }
    
    reportdoc_ptr = EditMMSReadReport(editdoc_ptr);
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SendReadReport reportdoc_ptr=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_902_112_2_18_2_38_32_250,(uint8*)"d",reportdoc_ptr);
    if(PNULL != reportdoc_ptr)
    {
        MMIMMS_SetDefaultHeadValue(reportdoc_ptr);
        reportdoc_ptr->message_class = MMS_MESSAGE_CLASS_TYPE_PERSONAL;
        reportdoc_ptr->delivery_report = MMS_DELIVERY_REPORT_NO;
        reportdoc_ptr->read_report = MMS_READ_REPORT_NO;
        
        if(MMIMMS_IntegrateMMS(reportdoc_ptr))
        {
            if(MMIMMS_AddManualList(dual_sys,reportdoc_ptr->record_id,MMIMMS_SENDRECV_SENDRPT))
            {
                MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_SendReadReport add manual list fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_918_112_2_18_2_38_32_251,(uint8*)"");
                return_val = MMIMMS_ERROR_FAIL;
            }
        }
        else
        {
            return_val = MMIMMS_ERROR_UNSUPORTED_MESSAGE;
        }
        MMIMMS_DestroyEditDocument(reportdoc_ptr);
    }
    else
    {
        return_val = MMIMMS_ERROR_ADDRESS_UNRESOLVED;
    }
    
    return return_val;
    //发送阅读报告--彩信
}

/*****************************************************************************/
//  Description : reject mms 彩信存储空间满时回应服务器 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_ResponceMMS(
                                            MN_DUAL_SYS_E     dual_sys, 
                                            MMS_NOTIFICATION_IND_MSG_T *msg_ptr,
                                            int respValue
                                            )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;
//  MMIMMS_EDIT_DOCUMENT_T * reportdoc_ptr = PNULL;
    MMS_DOCUMENT_HANDLE  mmsdoc_ptr = NULL;
    uint8 fname [MMIMMS_FILE_NAME_LEN] = {0};
    uint16 file_len = 0;
    int32 record_id = 0;
    
    //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_ResponceMMS respValue=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_946_112_2_18_2_38_32_252,(uint8*)"d",respValue);
    mmsdoc_ptr = MMS_CreateNewMms();
    if(mmsdoc_ptr)
    {
        MMS_SetHeadByType(mmsdoc_ptr, MMS_MESSAGE_TYPE_NOTIFYRESP_IND);
        /*	2. set value to it */
        MMS_SetHeadField(mmsdoc_ptr, MMS_TRANSACTION_ID, (long)0, (char *)msg_ptr->tid);
        
        MMS_SetHeadField(mmsdoc_ptr, MMS_STATUS, respValue, (int8 *)0);//to be MMS_STATUS_VALUE_RETRIVED
        
        if (MMIMMS_Setting_GetRecvDeliveryReport())     //送达报告
        {
            MMS_SetHeadField(mmsdoc_ptr, MMS_REPORT_ALLOWED, (long)MMS_REPORT_ALLOWED_YES, (char *)0);
        }
        else
        {
            MMS_SetHeadField(mmsdoc_ptr, MMS_REPORT_ALLOWED, (long)MMS_REPORT_ALLOWED_NO, (char *)0);
        }
        
        /* create a reply mms document */  //to be
        file_len = MMIMMS_GenUniName(fname,(MMIMMS_FILE_NAME_LEN-MMIMMS_SUFFIX_LEN));
        if(file_len > 0)
        {
            record_id = atoi((char *)fname);
        }
        else
        {
            MMS_DeleteMms(mmsdoc_ptr);
            return MMIMMS_ERROR_FAIL;
        }
        _snprintf((char* )(fname+file_len),(MMIMMS_FILE_NAME_LEN-file_len-1),"%s",".mms");
        file_len += SCI_STRLEN(".mms");
        
        MMS_SetMmsFile(mmsdoc_ptr, (char*)fname);
        /*3. encode it */
        MMS_EncodeMms(mmsdoc_ptr);
        /*	4: delete document */
        MMS_DeleteMms(mmsdoc_ptr);
        if(MMIMMS_AddManualList(dual_sys,record_id,MMIMMS_SENDRECV_SENDRESPONSE))
        {
            MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_ResponceMMS add manual list fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_990_112_2_18_2_38_32_253,(uint8*)"");
            return_val = MMIMMS_ERROR_FAIL;
        }
    }
    
    return return_val;
}

/*****************************************************************************/
//  Description : receive mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_ReceiveMMS(
                              MN_DUAL_SYS_E     dual_sys,
                              int32				record_id
                              )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;
   // MMISMS_ORDER_INFO_T *cursor_ptr = PNULL;

    //cursor_ptr = MMIMMS_GetListItemByID(record_id);
//    if (PNULL != cursor_ptr)
//    {
//        if (cursor_ptr->mo_mt_type == MMISMS_MT_NOT_DOWNLOAD)
//        {
//            cursor_ptr->mo_mt_type = MMISMS_MT_NOT_NOTIFY;
//        }
//        MMIMMS_SaveMMSListFile();
//    }

    if(MMIMMS_AddManualList(dual_sys,record_id,MMIMMS_SENDRECV_RECVING))
    {
        MMIMMS_DeleteAutoListItem(record_id);
        MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_ResponceMMS add manual list fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1027_112_2_18_2_38_32_254,(uint8*)"");
        return_val = MMIMMS_ERROR_FAIL;
    }
        
    return return_val;
}
/*****************************************************************************/
//  Description : send new mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_PROGRESS_T MMIMMS_GetActiveMMSProgress(void)
{
    return s_activemms_info;
}

/*****************************************************************************/
//  Description : whether mms is downloading/sending or not 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsInProcess(int32 record_id)
{
    return (s_activemms_info.record_id == record_id);
}


/*****************************************************************************/
//  Description : 取消当前彩信的收发
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_CancelSendRecvById(int32 record_id)
{
    if(record_id ==s_activemms_info.record_id)  //正在收发
    {
        MMIMMS_CancelSendRecv(TRUE);
    }
    else    //清除队列中的信息
    {
        MMIMMS_DeleteManualListItem(record_id);
        MMIMMS_DeleteAutoListItem(record_id);   
    }
}



/*****************************************************************************/
//  Description : 取消当前彩信的收发
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: is_manual: false PDP自动断开;  true 手动取消 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_CancelSendRecv(BOOLEAN is_manual)
{
    BOOLEAN return_val = TRUE;
    uint8 fname[MMIMMS_FILE_NAME_LEN] = {0};

    switch(s_activemms_info.status) 
    {
    case MMIMMS_STATUS_NETLINKING:
        if(!is_manual)
        {
            s_activemms_info.status = MMIMMS_STATUS_PDP_DEACTIVE_IND;
        }
        //SCI_TRACE_LOW:"mmitest remove fname"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1087_112_2_18_2_38_32_255,(uint8*)"");
        _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",s_activemms_info.record_id);
        sysVFremove((char *)fname);
        if(MMIMMS_SENDRECV_SENDING == s_activemms_info.type)
        {
            MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_OPR_CANCEL);
        }
        else
        {
            MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_OPR_CANCEL);
        }
        break;
    case MMIMMS_STATUS_NETLINK_SUCCESS:
    case MMIMMS_STATUS_NETCONNET_SUCCESS:    	
    case MMIMMS_STATUS_DATA_TRANSFERING:
        if(!is_manual)
        {
            s_activemms_info.status = MMIMMS_STATUS_PDP_DEACTIVE_IND;
        }
        MMS_CancelSendRecv();
    	break;

    default:
        return_val = FALSE;
        break;
    }

    return return_val;
}

/*****************************************************************************/
//  Description : 停止彩信收发模块,清空收发队列
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ClearRecvSendModule(void)
{ 
    MMIMMS_FreeSendRecvList();
    MMIMMS_CancelSendRecv(TRUE);
}

/*****************************************************************************/
//  Description : MMS is sending or receiving 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsSendingOrRecving(void)
{
    if(MMIMMS_STATUS_NONE == s_activemms_info.status)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : active pdp for mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_PDPActive(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN return_val = FALSE;
    uint8 setting_index = 0;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;

    setting_index = MMIMMS_Setting_GetNetSettingIndex(dual_sys);    
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, setting_index);
    if(setting_item_ptr)
    {
        app_info.app_handler = MMI_MODULE_MMS;
        app_info.auth_type = setting_item_ptr->auth_type;
        app_info.apn_ptr = (char*)setting_item_ptr->apn;
        app_info.user_name_ptr = (char*)setting_item_ptr->username;
        app_info.psw_ptr = (char*)setting_item_ptr->password;
        app_info.dual_sys = dual_sys;
        app_info.priority = 3;
        app_info.ps_service_rat = MN_UNSPECIFIED;
        app_info.handle_msg_callback = HandlePDPMsg;
        app_info.ps_service_type = MMS_E;//BROWSER_E
        app_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
        app_info.ip_type = setting_item_ptr->ip_type;
#endif

        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_PDPActive dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1168_112_2_18_2_38_32_256,(uint8*)"d",dual_sys);
        return_val = MMIAPIPDP_Active(&app_info);
    }

    return return_val;
}

/*****************************************************************************/
//  Description : deactive pdp for mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_PDPDeactive(void)
{
    BOOLEAN return_val = FALSE;

    return_val = MMIAPIPDP_Deactive(MMI_MODULE_MMS);
    MMIMMS_ControlToUIMsg(MSG_MMS_NETDISCONNECT,PNULL);
    return return_val;
}

/*****************************************************************************/
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPMsg msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1198_112_2_18_2_38_32_257,(uint8*)"d",msg_ptr->msg_id);
    
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
        HandlePDPActiveCnf(msg_ptr);
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        if (s_switch_simcard_pdp_active)
        {
            MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
            s_switch_simcard_pdp_active = FALSE;
        }
        break;
        
    case MMIPDP_DEACTIVE_IND:
        //SCI_TRACE_LOW:"HandlePDPMsg: MMIPDP_DEACTIVE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1215_112_2_18_2_38_32_258,(uint8*)"");
        HandlePDPDeactiveInd();
        MMIMMS_ControlToUIMsg(MSG_MMS_NETDISCONNECT,PNULL);
        break;
        
    default:
        break;
    }
    //SCI_FREE(msg_ptr);
}

/*****************************************************************************/
//  Description : handle play ring timer
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_HandleNetTimer(uint8 timer_id, uint32 param)
{
    if (g_mms_recv_timer == timer_id)
    {
        //SCI_TRACE_LOW:"MMIMMS_HandleNetTimer:recv timer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1234_112_2_18_2_38_33_259,(uint8*)"");
        if (0 != g_mms_recv_timer)
        {
            MMK_StopTimer(g_mms_recv_timer);
            g_mms_recv_timer = 0;
        }
        if (g_is_pdp_active_timer)//不允许pdp激活的timer
        {   
            //SCI_TRACE_LOW:"MMIMMS_HandleNetTimer:recv timer pdp permit not!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1242_112_2_18_2_38_33_260,(uint8*)"");
            s_is_send_retry_timer = FALSE;
            if (MMIMMS_IsPdpPermitUsing(s_activemms_info.dual_sys))
            {
                g_is_pdp_active_timer = FALSE;
                MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
            }
            else
            {
                g_mms_recv_timer = MMK_CreateTimerCallback(MMS_NET_TIME_OUT, MMIAPIMMS_HandleNetTimer, NULL, FALSE);
                g_is_pdp_active_timer = TRUE;
                s_is_send_retry_timer = TRUE;
            }
        }
        else//下载失败的timer
        {
            //SCI_TRACE_LOW:"MMIMMS_HandleNetTimer:retry recv timer! detach pdp"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1257_112_2_18_2_38_33_261,(uint8*)"");
            if (s_is_send_retry_timer)
            {
                s_is_send_retry_timer = FALSE;//发送timer到期允许下载操作了
            }
            if (MMIMMS_PDPDeactive())
            {
                s_switch_simcard_pdp_active = TRUE;
            }
            else
            {
                MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
            }            
            
        }       
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : is pdp permit using,G ner and in phone can not use pdp
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsPdpPermitUsing(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_permit_used = TRUE;
    MN_PHONE_PLMN_STATUS_E net_status = PLMN_NO_SERVICE;
    net_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_IsPdpPermitUsing net_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1286_112_2_18_2_38_33_262,(uint8*)"d",net_status);
    if (PLMN_NORMAL_GPRS_ONLY == net_status        
        || PLMN_EMERGENCY_GPRS_ONLY == net_status)
    {
        is_permit_used = TRUE;
    }
    if (PLMN_NORMAL_GSM_GPRS_BOTH == net_status || PLMN_NORMAL_GSM_ONLY == net_status)
    {
        is_permit_used = TRUE;
        if (MMI_GMMREG_RAT_GPRS == MMIAPIPHONE_GetTDOrGsm(dual_sys) && MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            is_permit_used = FALSE;
        }
    }

    return is_permit_used;
}

/*****************************************************************************/
//  Description : handle pdp active cnf
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPActiveCnf(MMIPDP_CNF_INFO_T *msg_ptr)
{
    MMS_CONFIG_SETTING_T cfgset = {0};
    MMS_SEND_SETTING_T sendset = {0};
    MMS_RECV_SETTING_T recvset = {0};
    uint8 *fname = PNULL;
    uint8 setting_index = 0;
    MMIMMS_PUSH_FILE_T *push_info_ptr = PNULL;
    uint16      file_len = 0;
    wchar       *w_fname = PNULL;
    wchar       *mms_file_path = PNULL;
    uint16      mms_file_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_item_ptr = PNULL;
    MMIENG_UA_T *ua_ptr = PNULL;
    MN_PLMN_T select_plmn = {0, 0, 2};
    uint32 total_num = 0;
    BOOLEAN is_uninor = FALSE;
    uint32 i = 0;
    uint8 *mms_center_ptr = PNULL;
    uint16 mms_center_len = 0;
    MMISMS_ORDER_INFO_T *mms_send_order_ptr = PNULL;

    BOOLEAN is_need_mua = FALSE;
    uint32 ua_network_num = 0;

    //cr 147299
    BOOLEAN is_need_mua_uaProfile = FALSE;
    uint16 uUa_Rec_Num = 0;
    char* cUa_ptr = PNULL;
    char* cUa_profile_ptr = PNULL;

    if(MMIMMS_STATUS_NETLINKING != s_activemms_info.status)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPActiveCnf"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1335_112_2_18_2_38_33_263,(uint8*)"");
        return;
    }
    if(MMIPDP_RESULT_SUCC != msg_ptr->result)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPActiveCnf msg_ptr->result=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1340_112_2_18_2_38_33_264,(uint8*)"d",msg_ptr->result);
        if(    MMIMMS_SENDRECV_SENDING == s_activemms_info.type
            || MMIMMS_SENDRECV_SENDRPT == s_activemms_info.type
            || MMIMMS_SENDRECV_SENDRESPONSE == s_activemms_info.type)
        {
#ifdef DATA_ROAMING_SUPPORT
            if (msg_ptr->result == MMIPDP_RESULT_NOT_PERMIT_ROAMING)
            {
                MMSNetCallback(MMS_STATUS_SEND_RETURN, PNULL, MMS_NETLINK_DATA_ROAMING_DISABLE);
            }
            else
#endif
#ifdef FDN_SUPPORT
            if(MMIPDP_RESULT_FDN_NOT_PERMIT == msg_ptr->result)
            {
                MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_NETLINK_FDN_ONLY);
            }
            else
#endif
            {
                MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_NETLINK_FAILED);
            }
        }
        else if(MMIMMS_SENDRECV_RECVING == s_activemms_info.type)
        {
            //////////////////////////////////////////////////////////////////////////
            // modified by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
            if (msg_ptr->result == MMIPDP_RESULT_NOT_PERMIT_ROAMING)
            {
                MMSNetCallback(MMS_STATUS_RECV_RETURN, PNULL, MMS_NETLINK_DATA_ROAMING_DISABLE);
            }
            else
#endif
#ifdef FDN_SUPPORT
            if(MMIPDP_RESULT_FDN_NOT_PERMIT == msg_ptr->result)
            {
                MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_NETLINK_FDN_ONLY);
            }
            else
#endif
            {
                MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_NETLINK_FAILED);
            }
        }
        return;
    }

    setting_index = MMIMMS_Setting_GetNetSettingIndex(s_activemms_info.dual_sys);    
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(s_activemms_info.dual_sys, setting_index);
    if(PNULL != setting_item_ptr)
    {
        cfgset.wap_gateway = (char *)setting_item_ptr->gateway;    
        cfgset.wap_port = setting_item_ptr->port;
        cfgset.app_module_id = MMI_MODULE_MMS;
        cfgset.is_http = (setting_item_ptr->access_type == 0) ? FALSE:TRUE;
        cfgset.user_name_ptr = (char *)setting_item_ptr->username;
        cfgset.password_ptr = (char *)setting_item_ptr->password;
    }
    cfgset.net_id = (uint32)msg_ptr->nsapi;

    MMIAPIPHONE_GetCurPLMN(&select_plmn, s_activemms_info.dual_sys);
    total_num = sizeof(s_uninor_mcc_mnc_table) / sizeof(UNINOR_MCC_MNC_T);
    for (i = 0; i < total_num; i++)
    {
        if (select_plmn.mcc == s_uninor_mcc_mnc_table[i].mcc && select_plmn.mnc == s_uninor_mcc_mnc_table[i].mnc)
        {
            is_uninor = TRUE;
            break;
        }
    }
    if (is_uninor)
    {
        if(PNULL != setting_item_ptr)
        {
            mms_center_len = strlen((char *)setting_item_ptr->homepage);
        }
        else
        {
            mms_center_len = 0;
        }
        mms_center_ptr = (uint8 *)SCI_ALLOCAZ(mms_center_len + 2);
        if (PNULL == mms_center_ptr)
        {
            //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPActiveCnf alloc mms_center_ptr failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1414_112_2_18_2_38_33_265,(uint8*)"");
            if(    MMIMMS_SENDRECV_SENDING == s_activemms_info.type
                || MMIMMS_SENDRECV_SENDRPT == s_activemms_info.type
                || MMIMMS_SENDRECV_SENDRESPONSE == s_activemms_info.type)
            {
                MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_RESPONSE_STATUS_VALUE_E_UNKNOW_ERROR);
            }
            else if(MMIMMS_SENDRECV_RECVING == s_activemms_info.type)
            {
                MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RESPONSE_STATUS_VALUE_E_UNKNOW_ERROR);
            }            
            return;
        }
        SCI_MEMSET(mms_center_ptr, 0, (mms_center_len + 2));
        if(PNULL != setting_item_ptr)
        {
            SCI_MEMCPY(mms_center_ptr, (char *)setting_item_ptr->homepage, (mms_center_len + 2));
        }
        if ('/' != *(mms_center_ptr + mms_center_len - 1))
        {
            *(mms_center_ptr + mms_center_len) = '/';
        }
        cfgset.mms_centery = (char *)mms_center_ptr;
    }
    else if(PNULL != setting_item_ptr)
    {
        cfgset.mms_centery = (char *)setting_item_ptr->homepage;
    }

    ua_ptr = (MMIENG_UA_T *)SCI_ALLOCAZ(sizeof(MMIENG_UA_T));
    if (PNULL == ua_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPActiveCnf alloc mms_center_ptr failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1443_112_2_18_2_38_33_266,(uint8*)"");
        if(    MMIMMS_SENDRECV_SENDING == s_activemms_info.type
            || MMIMMS_SENDRECV_SENDRPT == s_activemms_info.type
            || MMIMMS_SENDRECV_SENDRESPONSE == s_activemms_info.type)
        {
            MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_RESPONSE_STATUS_VALUE_E_UNKNOW_ERROR);
        }
        else if(MMIMMS_SENDRECV_RECVING == s_activemms_info.type)
        {
            MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RESPONSE_STATUS_VALUE_E_UNKNOW_ERROR);
        }
        if (is_uninor)
        {
            SCI_FREE(mms_center_ptr);
            mms_center_ptr = PNULL;
        }
        return;
    }
    //SCI_MEMSET((char *)ua_ptr, 0, sizeof(MMIENG_UA_T));
    MMIAPIENG_GetUa(ua_ptr);

    ua_network_num = sizeof(s_mcompany_ua_mcc_mnc_table) / sizeof(UNINOR_MCC_MNC_T);
    for (i = 0; i < ua_network_num; i++)
    {
        if (select_plmn.mcc == s_mcompany_ua_mcc_mnc_table[i].mcc && select_plmn.mnc == s_mcompany_ua_mcc_mnc_table[i].mnc)
        {
            is_need_mua= TRUE;
            is_need_mua_uaProfile = TRUE;
            uUa_Rec_Num = i;
            break;
        }
    }
    
    SCI_TRACE_LOW("[MMIMMS]:HandlePDPActiveCnf is_need_mua=%d, is_need_mua_uaProfile=%d, uUa_Rec_Num=%d, mcc=%d, mnc=%d", 
        is_need_mua, is_need_mua_uaProfile, uUa_Rec_Num, select_plmn.mcc, select_plmn.mnc);
        
    if(is_need_mua)
    {
        // cr147299
        cfgset.user_agent_ptr = ( (uUa_Rec_Num<(sizeof(s_mcompany_ua_table) / sizeof(char*)))
            ?(char *)s_mcompany_ua_table[uUa_Rec_Num]:(char*)MMIMMS_MCOMPANY_USER_AGENGT);
    }
    else
    {
        if (0 != SCI_STRLEN((char *)ua_ptr->user_agent))
        {
            cfgset.user_agent_ptr = (char *)ua_ptr->user_agent;
        }
        else
        {
            cfgset.user_agent_ptr = (char *)MMIAPICOM_GetUserAgent();
        }
    }

    // cr147299
    if(is_need_mua_uaProfile && uUa_Rec_Num)  //uUa_Rec_Num=0 which russia no need update from table
    {
        cfgset.ua_profile_ptr = ( (uUa_Rec_Num<(sizeof(s_mcompany_ua_profile_table) / sizeof(char*)))
            ?(char *)s_mcompany_ua_profile_table[uUa_Rec_Num]:(char*)MMIMMS_404_4_USER_AGENGT_PROFILE);
    }
    else
    {    
        if (0 != SCI_STRLEN((char *)ua_ptr->uaprofile))
        {
            cfgset.ua_profile_ptr = (char *)ua_ptr->uaprofile;
        }
        else
        {
            cfgset.ua_profile_ptr = (char *)MMIAPICOM_GetUserAgentProfile();
        }
    }
        
    //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPActiveCnf s_activemms_info.type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1498_112_2_18_2_38_33_267,(uint8*)"d",s_activemms_info.type);
    if(    MMIMMS_SENDRECV_SENDING == s_activemms_info.type
        || MMIMMS_SENDRECV_SENDRPT == s_activemms_info.type
        || MMIMMS_SENDRECV_SENDRESPONSE == s_activemms_info.type)
    {
        fname = (uint8 *)SCI_ALLOCAZ(MMIMMS_FILE_NAME_LEN + 1);
        if (PNULL == fname)
        {
            //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPActiveCnf alloc fname failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1506_112_2_18_2_38_33_268,(uint8*)"");
            MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_RESPONSE_STATUS_VALUE_E_UNKNOW_ERROR);

            if (is_uninor)
            {
                SCI_FREE(mms_center_ptr);
                mms_center_ptr = PNULL;
            }
            SCI_FREE(ua_ptr);
            ua_ptr = PNULL;
            return;
        }
        //SCI_MEMSET(fname, 0, (MMIMMS_FILE_NAME_LEN + 1));

        if(MMS_SendRecvInit(&cfgset))
        {
            _snprintf((char* )fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",s_activemms_info.record_id);
            sendset.max_times = MMIMMS_MAX_RETRY_TIMES;
            sendset.send_vfilename = (char*)fname;  
            sendset.conf_vfilename = (char*)"confirm.mms";

            mms_send_order_ptr = MMIMMS_GetListItemByID(s_activemms_info.record_id);//根据是否在order里面存在判断是否是发送的是状态消息，如果是状态消息，不用写入信箱
            if (PNULL == mms_send_order_ptr)
            {
                sendset.max_times = MMIMMS_MAX_RSP_RETRY_TIMES;
            }

            if(MMS_Send(&sendset,MMSNetCallback))
            {
                MMSNetCallback(MMS_STATUS_NORMAL,PNULL,MMS_NETLINK_SUCCESS);
            }
            else
            {
                //SCI_TRACE_LOW:"MMIMMS]:MMIMMS_SendNewMMS mms send failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1539_112_2_18_2_38_33_269,(uint8*)"");
                MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_RESPONSE_STATUS_VALUE_E_UNKNOW_ERROR);
                MMS_SendRecvClose();
            } 
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SendNewMMS MMS_SetConfig(&cfgset) failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1546_112_2_18_2_38_33_270,(uint8*)"");
            MMSNetCallback(MMS_STATUS_SEND_RETURN,PNULL,MMS_RESPONSE_STATUS_VALUE_E_UNKNOW_ERROR);
        }
        SCI_FREE(fname);
        fname = PNULL;
    }
    else if(MMIMMS_SENDRECV_RECVING == s_activemms_info.type)
    {
        fname = (uint8 *)SCI_ALLOCAZ(MMIMMS_FILE_NAME_LEN + 1);
        w_fname = (wchar *)SCI_ALLOCAZ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar));
        mms_file_path = (wchar *)SCI_ALLOCAZ((MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar));
        if (PNULL == fname || PNULL == w_fname || PNULL == mms_file_path)
        {
            //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPActiveCnf alloc fname failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1559_112_2_18_2_38_33_271,(uint8*)"");
            MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RECV_FAILED);
            if (PNULL != fname)
            {
                SCI_FREE(fname);
                fname = PNULL;
            }
            if (PNULL != w_fname)
            {
                SCI_FREE(w_fname);
                w_fname = PNULL;
            }
            if (PNULL != mms_file_path)
            {
                SCI_FREE(mms_file_path);
                mms_file_path = PNULL;
            }

            if (is_uninor)
            {
                SCI_FREE(mms_center_ptr);
                mms_center_ptr = PNULL;
            }
            SCI_FREE(ua_ptr);
            ua_ptr = PNULL;
            return;
        }
        //SCI_MEMSET(fname, 0, (MMIMMS_FILE_NAME_LEN + 1));
        //SCI_MEMSET(w_fname, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
        //SCI_MEMSET(mms_file_path, 0, ((MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar))); 
        
        _snprintf((char* )fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",s_activemms_info.record_id);
        file_len = SCI_STRLEN((char* )fname);
        MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,file_len);
        push_info_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
        if (PNULL != push_info_ptr)
        {
            //SCI_MEMSET((uint8 *)push_info_ptr, 0 , sizeof(MMIMMS_PUSH_FILE_T));
            MMIMMS_ReadPush(push_info_ptr, s_activemms_info.record_id);
            if(MMIMMS_GetMMSFilePathForWrite(w_fname,file_len,mms_file_path,&mms_file_path_len,push_info_ptr->size + MMIMMS_RESERVE_LIST_SIZE))
            {
                if(MMS_SendRecvInit(&cfgset))
                {
                    recvset.immediate_recv = s_activemms_info.immediate_recv;
                    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReceiveMMS immediate_recv=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1603_112_2_18_2_38_33_272,(uint8*)"d",recvset.immediate_recv);
#ifdef WIN32   //for test
                    //SCI_MEMCPY(push_info.url,"http://172.16.14.173:8082/mms/newvideo.mms",SCI_STRLEN("http://172.16.14.173:8082/mms/newvideo.mms"));
                    SCI_MEMCPY(push_info_ptr->url,"http://172.16.14.173:8082/mms/recv.mms",SCI_STRLEN("http://172.16.14.173:8082/mms/recv.mms"));
#endif
                    recvset.max_times = MMIMMS_MAX_RETRY_TIMES;
                    recvset.get_url=(char*)push_info_ptr->url;
                    MMI_WSTRNCPY(recvset.recv_file_name,MMS_MAX_FILE_NAME_LEN,mms_file_path,mms_file_path_len,mms_file_path_len);
                    recvset.recv_msgsize = push_info_ptr->size;
                    recvset.is_permit_send_report = MMIMMS_Setting_GetRecvDeliveryReport(); //发送送达报告
                    s_activemms_info.total_size = push_info_ptr->size;
                    if(MMS_Receive(&recvset,MMSNetCallback))
                    {
                        MMSNetCallback(MMS_STATUS_NORMAL,PNULL,MMS_NETLINK_SUCCESS);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIMMS]:MMIMMS_ReceiveMMS mms recv failed"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1620_112_2_18_2_38_33_273,(uint8*)"");
                        MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RECV_FAILED);
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReceiveMMS MMS_SetConfig(&cfgset) failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1626_112_2_18_2_38_33_274,(uint8*)"");
                    MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RECV_FAILED);
                } 
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPActiveCnf recv no space"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1632_112_2_18_2_38_33_275,(uint8*)"");
                MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RECV_FAILED_NO_SPACE);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReceiveMMS alloc pushfile failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1638_112_2_18_2_38_33_276,(uint8*)"");
            MMSNetCallback(MMS_STATUS_RECV_RETURN,PNULL,MMS_RECV_FAILED);
        }

        SCI_FREE(push_info_ptr);
        push_info_ptr = PNULL;
        SCI_FREE(fname);
        fname = PNULL;
        SCI_FREE(w_fname);
        w_fname = PNULL;
        SCI_FREE(mms_file_path);
        mms_file_path = PNULL;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:HandlePDPActiveCnf s_activemms_info.type= %d  WRONG"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_NET_1653_112_2_18_2_38_33_277,(uint8*)"d",s_activemms_info.type);
    }

    if (is_uninor)
    {
        SCI_FREE(mms_center_ptr);
        mms_center_ptr = PNULL;
    }
    SCI_FREE(ua_ptr);
    ua_ptr = PNULL;
}

/*****************************************************************************/
//  Description : handle pdp deactive ind
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPDeactiveInd(void)
{
    if(MMIMMS_STATUS_NONE != s_activemms_info.status)
    {
        MMIMMS_CancelSendRecv(FALSE);
    }   
}


/****************************************************************************
** File Name:      mmivt_kernel_process.c                                  *
** Author:                                                                 *
** Date:           12/18/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2009        samboo.shen         Create
****************************************************************************/
//#ifndef _MMIVT_KERNEL_PROCESS_C_
#define _MMIVT_KERNEL_PROCESS_C_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_vt_trc.h"
#ifdef __cplusplus
extern   "C"
{
#endif
    
    //#include "mmivt_export.h"
#ifdef VT_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmivt_ui.h"
#include "mmivt_kernel.h"
    
#include "mmivt_kernel_324Adapter.h"
#include "mmivt_kernel_refAdapter.h"

#include "mmicl_export.h"
#include "mmicc_export.h"
#include "mmibt_export.h"
#ifdef STREAMING_HS_SUPPORT
#include "mmismp_export.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define VT_CALL_ID 0    //VT电话只有一通，默认的call id为0
#define MMI_CALL_TYPE_DATA MN_CALL_TYPE_DATA

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
    VT_324_STATE_IDLE,	//324未启动
    VT_324_STATE_ACTIVE,	//324激活
    VT_324_STATE_DISCONNECTING	//324断开中
    //VT_324_STATE_MAX
}VT_324_STATE_E; // 324的当前状态;

typedef enum
{
    VT_PS_STATE_IDLE,	//PS未启动
    VT_PS_STATE_ACTIVE,	//PS激活
    VT_PS_STATE_DISCONNECTING	//PS断开中
    //VT_PS_STATE_MAX
}VT_PS_STATE_E; // PS的当前状态

typedef struct
{
    uint32 start_time;
    uint32 stop_time;
}VT_CALL_TIME_T;//VT电话的开始时间和结束时间

typedef struct 
{
    MMIVT_KERNEL_STATE_E cur_state;       //记录vt的当前状态
    MMIVT_KERNEL_STATE_E last_state;      //记录vt的上一个状态
    MMIVT_KERNEL_MSG_E last_action;    //记录vt的上一个动作
    MMVT_KERNEL_SUBSTATE_E sub_state;  //子状态
}VT_STATE_INFO_T;

typedef struct
{
    MN_DUAL_SYS_E dual_sys; //sim卡 ID
    uint8 call_id; //和CC匹配
    //uint8 call_type; 和CC匹配
    VT_STATE_INFO_T state_info;   //记录vt的状态
    MMIVT_CALL_TYPE_E call_type; //mo还是mt
    VT_324_STATE_E h324_state;	//324协议栈的状态
    VT_PS_STATE_E ps_state;	//ps的状态
    MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause; //挂断原因
    MMIVT_ADDRESS_INFO_T call_adress; //记录来电号码的相关信息
    VT_CALL_TIME_T call_time_info; //记录电话的起始和结束时间
}VT_KERNEL_INFO_T;


LOCAL VT_KERNEL_INFO_T s_vt_kernel_info ={0};   //VT状态

LOCAL BOOLEAN s_is_mt_received = FALSE; //记录本方是否处理来电，保存入call log时决定存入类型

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Handle_user_dial
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理电话拨号操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUserDial(DPARAM param);



/*****************************************************************************/
//  Description : HandleSetupComplete
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理对方已经接听的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleSetupComplete(void);

/*****************************************************************************/
//  Description : HandleStartMedia
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理324已经建立，数据流开始交换的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleStartMedia(void);

/*****************************************************************************/
//  Description : HandleUserHangup
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理用户挂断操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUserHangup(DPARAM param);

/*****************************************************************************/
//  Description : HandleUserHangup
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理324开始断开操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E Handle324ClosedInd(void);

/*****************************************************************************/
//  Description : Handle324CloseCnf
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理324断开完成的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E Handle324CloseCnf(void);

#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : HandleDisconnectedInd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理PS断开完成的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleDisconnectedInd(DPARAM param);
#endif

/*****************************************************************************/
//  Description : HandleIncomingCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理来电操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleIncomingCall(DPARAM param);

/*****************************************************************************/
//  Description : HandleAlertingInd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理本方振铃请求的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleAlertingInd(void);

/*****************************************************************************/
//  Description : HandleConnectCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理接听电话的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleConnectCall(void);

/*****************************************************************************/
//  Description : HandleConnectInd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理接听完成的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleConnectInd(void);

/*****************************************************************************/
//  Description : TranslateState
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:vt的状态切换函数
/*****************************************************************************/
LOCAL void TranslateState(MMIVT_KERNEL_STATE_E cur_state, MMIVT_KERNEL_MSG_E action);

/*****************************************************************************/
//  Description : MakeVtCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:拨出VT电话
/*****************************************************************************/
LOCAL MMIVT_RESULT_E MakeVtCall(MN_DUAL_SYS_E dual_sys, uint8* tele_num, uint8 tele_len);
/*****************************************************************************/
//  Description : SendMsgToUI
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:向UI发送消息
/*****************************************************************************/
LOCAL MMIVT_RESULT_E SendMsgToUI(MMIVT_UI_MSG_E action, DPARAM param);

/*****************************************************************************/
//  Description : SetAdressPBInfo
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void SetAdressPBInfo( void);

/*****************************************************************************/
//  Description : SetAddressInfoByBCDNum
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:according to normal num set VT address info (mt calling)
/*****************************************************************************/
LOCAL void SetAddressInfoByBCDNum( MN_CALLING_NUMBER_T* bcd_num);

/*****************************************************************************/
//  Description : RecordStartTime
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void RecordStartTime( void);

/*****************************************************************************/
//  Description : RecordStopTime
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void RecordStopTime(void);

/*****************************************************************************/
//  Description : 判断是否是远端的回落请求
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsRemoteFallBackReq( void);
#ifdef MMIPB_MOST_USED_SUPPORT
//+CR251638
/*****************************************************************************/
//  Description : 存入常用联系人
//  Global resource dependence : 
//  Author:Buddy.
//  Note: 
/*****************************************************************************/
LOCAL void SaveToMostUsedList(void);
//-CR251638
#endif
#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : 存入通话记录
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SaveToCallLog(void);
#endif

/*****************************************************************************/
//  Description : 处理userinput消息.....
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void HandleUserInput(DPARAM param);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtCalling
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:是否处于VT过程
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtCalling (void)
{
    BOOLEAN result = FALSE;
    
    switch(MMIVT_GetCallState()) 
    {
    case MMIVT_KERNEL_STATE_PS_CONNECTING:
    case MMIVT_KERNEL_STATE_H324_CONNECTING:
    case MMIVT_KERNEL_STATE_CALL_CONNECTED:
    case MMIVT_KERNEL_STATE_ALERTING:
    case MMIVT_KERNEL_STATE_CONNECT_PENDING:
    case MMIVT_KERNEL_STATE_DISCONNECTING:
        result = TRUE;
    	break;
    
    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtConnecting
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:是否已经处于VT通话状态
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtConnecting (void)
{
    BOOLEAN result = FALSE;
    
    switch(MMIVT_GetCallState()) 
    {
    case MMIVT_KERNEL_STATE_H324_CONNECTING:
    case MMIVT_KERNEL_STATE_CALL_CONNECTED:
    case MMIVT_KERNEL_STATE_CONNECT_PENDING:
    case MMIVT_KERNEL_STATE_DISCONNECTING:
        result = TRUE;
    	break;
    
    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtMoCalling
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:是否处于VT呼出中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtMoCalling (void)
{
    BOOLEAN result = FALSE;
    
    switch(MMIVT_GetCallState()) 
    {
    case MMIVT_KERNEL_STATE_PS_CONNECTING:
        if (MMIVT_CALL_TYPE_MO_CALL == MMIVT_GetCallType())
        {
            result = TRUE;
        }       
    	break;
    
    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtMtIncoming
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:是否处于VT来电中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtMtIncoming (void)
{
    BOOLEAN result = FALSE;
    
    switch(MMIVT_GetCallState()) 
    {
    case MMIVT_KERNEL_STATE_ALERTING:
        result = TRUE;
    	break;
    
    default:
        result = FALSE;
        break;
    }
    return result;
}

//*****************************************************************************/
//  Description : MMIAPIVT_IsVTIncomingMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:是否是VT call的setup消息
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVTIncomingMsg (uint16 msg_id, DPARAM param)
{
    BOOLEAN result = FALSE;
    //param 不为NULL
    if (PNULL == param)
    {
        //SCI_TRACE_LOW:"mmivt null param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_348_112_2_18_3_6_4_14,(uint8*)"");
        return FALSE;
    }
    if (APP_MN_SETUP_IND == msg_id)
    {
        APP_MN_SETUP_IND_T* info = (APP_MN_SETUP_IND_T*)param;
        if (MMI_CALL_TYPE_DATA == info->call_type)
        {
            result = TRUE;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : MMIVT_HandleExternalMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:接收来自外部的消息（PS & h324）。蓝牙处理流程依照CC的处理方式和顺序
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVT_HandleExternalMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    

    //IF(处于VT状态 | | 来电消息是VT类型)
    if (!(MMIAPIVT_IsVTIncomingMsg(msg_id, param)||MMIAPIVT_IsVtCalling()))
    {
        return MMI_RESULT_FALSE;
    }
    
    //SCI_TRACE_LOW:"mmivt:enter MMIVT_HandleExtMsg(), msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_376_112_2_18_3_6_4_15,(uint8*)"d", msg_id);

    switch(msg_id) 
    {
    case APP_MN_CALL_START_IND:
        s_vt_kernel_info.call_id = ((APP_MN_CALL_START_IND_T*)param)->call_id;
        //蓝牙
        MMIAPIBT_OutgoingCall();
        break;
        
    case APP_MN_SYNC_IND:
        
        if (MMIVT_CALL_TYPE_MO_CALL == MMIVT_GetCallType())
        {
            //蓝牙
            MMIAPIBT_RemoteAlerting();
        }
        break;

    case APP_MN_SETUP_COMPLETE_CNF:
        //蓝牙
        {
            BT_ADDRESS address = {0};
            MMIAPIBT_CallConnect(&address);
        }
        //对方已经接听
        MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_SETUP_COMPLETE_IND, PNULL);
        break;
        
    case APPSIG_MPLH324M_MSG_STARTMEDIA:
        MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_START_MEDIA_IND, PNULL);
        break;
        
    case APPSIG_MPLH324M_MSG_CLOSE_IND:
        MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_324_CLOSED_IND, PNULL);
        break;
        
    case APPSIG_MPLH324M_MSG_CLOSE_CNF:
        MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_324_CLOSED_CNF, PNULL);
        break;
        
    case APP_MN_CALL_DISCONNECTED_IND:
    case APP_MN_CALL_ERR_IND:
        //蓝牙
        {
            BT_ADDRESS address = {0};
            if (MMIAPIBT_GetActivatedHS(&address))
            {
                switch(MMIVT_GetCallState()) 
                {
                case MMIVT_KERNEL_STATE_PS_CONNECTING:
                    if (MMIVT_CALL_TYPE_MO_CALL == MMIVT_GetCallType())
                    {
                        MMIAPIBT_StopAlert();
                    }
                	break;
                case MMIVT_KERNEL_STATE_ALERTING:
                case MMIVT_KERNEL_STATE_CONNECT_PENDING:
                case MMIVT_KERNEL_STATE_DISCONNECTING:
                    if(MMIVT_CALL_TYPE_MT_CALL == s_vt_kernel_info.call_type)
                    {
                        MMIAPIBT_StopRing();
                    }
                    break;                    
                case MMIVT_KERNEL_STATE_H324_CONNECTING:
                case MMIVT_KERNEL_STATE_CALL_CONNECTED:
                    
                    break;
                default:
                    //do nothing
                    break;
                }
                MMIAPIBT_CallDisconnect();
            }
        }
        
        if(s_vt_kernel_info.call_id == ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->call_id)
        {
            MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_DISCONNECTED_IND, param);
        }
        break;
        
    case APP_MN_SETUP_IND:
        if(!MMIAPICC_HandleMTCallDisc(param))//处理黑名单
        {
            MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_INCOMING_CALL_IND, param);
        }
        break;
        
    case APP_MN_ALERTING_IND:
        {
            APP_MN_ALERTING_IND_T *pSig = (APP_MN_ALERTING_IND_T*)param;

            if (MN_CALL_LOCAL_USER_ALERTING == pSig->alerting_type )
            {
                MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_ALERTING_IND, PNULL);
            }
            else if (MN_CALL_REMOTE_USER_ALERTING == pSig->alerting_type)
            {
                //子状态处理，不改变基本状态
                s_vt_kernel_info.state_info.sub_state = MMVT_KERNEL_SUBSTATE_REMOTE_ALERTING;
                SendMsgToUI(MMIVT_UI_MSG_REMOTE_ALERTING, PNULL);
            }
            //蓝牙
            {
                MMI_STRING_T prompt_str = {0};
                wchar temp_wchar[MMIVT_PHONE_NUM_MAX_LEN + 3] = {0};
                uint16 copy_len = 0;
            
                if(s_vt_kernel_info.call_adress.tele_len > 0)
                {
                    prompt_str.wstr_ptr = temp_wchar;
                    copy_len = MIN(s_vt_kernel_info.call_adress.tele_len, MMIVT_PHONE_NUM_MAX_LEN);
                    prompt_str.wstr_len = copy_len;
                    MMI_STRNTOWSTR(prompt_str.wstr_ptr,
                        prompt_str.wstr_len,
                        (uint8 *)s_vt_kernel_info.call_adress.tele_num,
                        s_vt_kernel_info.call_adress.tele_len,
                        copy_len);
                
                }
                if (MN_CALL_LOCAL_USER_ALERTING == pSig->alerting_type )
                {
                    MMIAPIBT_IncomingCall(prompt_str);
                }
            }
        }
        break;
        
    case APP_MN_SETUP_COMPLETE_IND:
        //蓝牙
        {
            BT_ADDRESS address = {0};
            MMIAPIBT_CallConnect(&address);
        }
        MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_CONNECT_IND, PNULL);
        break;
        
    case APPSIG_MPLH324M_MSG_IND_USER_INPUT_ALPHANUMERIC:
        HandleUserInput(param);
        break;

	case APPSIG_MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_START://视频通话时，对方无视频数据
		SendMsgToUI(MMIVT_UI_MSG_REMOTE_NO_SIGNAL_START, PNULL);
		break;

	case APPSIG_MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_STOP://对方恢复视频数据
        SendMsgToUI(MMIVT_UI_MSG_REMOTE_NO_SIGNAL_STOP, PNULL);
		break;

    case APPSIG_MPLH324M_MSG_MEDIA_INACTIVE_IND: //对方关闭摄像头
        SendMsgToUI(MMIVT_UI_MSG_REMOTE_CAMERA_OFF, PNULL);
        break;

    case APPSIG_MPLH324M_MSG_MEDIA_ACTIVE_IND: //对方打开摄像头
        SendMsgToUI(MMIVT_UI_MSG_REMOTE_CAMERA_ON, PNULL);
        break;
        
    case MSG_VT_REF_ERROR_IND:
        SendMsgToUI(MMIVT_UI_MSG_REF_ERROR_IND, param);
        break;
        
    default:
        //donothing;
        //SCI_TRACE_LOW:"mmivt: ext msg: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_540_112_2_18_3_6_4_16,(uint8*)"d", msg_id);
        break;
    }
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : MMIVT_HandleKernelMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理来自ps和ui的操作，进行相应的状态跃迁。
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIVT_HandleKernelMsg(
    MMIVT_KERNEL_MSG_E msg,	//需要处理的action
    DPARAM param	//action相关信息
    )
{
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    //SCI_TRACE_LOW:"mmivt cur_state:%d, 324 state:%d, ps state:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_559_112_2_18_3_6_4_17,(uint8*)"ddd", MMIVT_GetCallState(),s_vt_kernel_info.h324_state,s_vt_kernel_info.ps_state);
    switch(msg)
    {
    case MMIVT_KERNEL_MSG_USER_DIAL:
        result = HandleUserDial(param);
        break;
        
    case MMIVT_KERNEL_MSG_SETUP_COMPLETE_IND:
        result = HandleSetupComplete();
        break;
        
    case MMIVT_KERNEL_MSG_START_MEDIA_IND:
        result = HandleStartMedia();
        break;
        
    case MMIVT_KERNEL_MSG_USER_HANGUP:
        result = HandleUserHangup(param);
        break;
        
    case MMIVT_KERNEL_MSG_324_CLOSED_IND:
        result = Handle324ClosedInd();
        break;
        
    case MMIVT_KERNEL_MSG_324_CLOSED_CNF:
        result = Handle324CloseCnf();
        break;
        
    case MMIVT_KERNEL_MSG_DISCONNECTED_IND:
        result = HandleDisconnectedInd(param);
        break;
        
    case MMIVT_KERNEL_MSG_INCOMING_CALL_IND:
        result = HandleIncomingCall(param);
        break;
        
    case MMIVT_KERNEL_MSG_ALERTING_IND:
        result = HandleAlertingInd();
        break;
        
    case MMIVT_KERNEL_MSG_CONNECT_CALL:
        result = HandleConnectCall();
        break;
        
    case MMIVT_KERNEL_MSG_CONNECT_IND:
        result = HandleConnectInd();
        break;
        
    default:
        //SCI_TRACE_LOW:"mmivt wrong msg: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_607_112_2_18_3_6_4_18,(uint8*)"d", msg);
        result = MMIVT_RESULT_INVALID_ACTION;/*lint !e527*/
        break;
    }

    return result;
    
}

/*****************************************************************************/
//  Description : Handle_user_dial
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理电话拨号操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUserDial(DPARAM param)
{
    
    MMIVT_RESULT_E  vt_result = MMIVT_RESULT_SUCCESS;
    MMIVT_KERNEL_INFO_USER_DIAL_T *user_dial_info = (MMIVT_KERNEL_INFO_USER_DIAL_T*) param;// + sizeof(xSignalHeader);
    MMIVT_REF_RET_E ref_open_result = MMIVT_REF_RET_SUCCESS;
    //SCI_TRACE_LOW:"mmivt HandleUserDial"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_626_112_2_18_3_6_4_19,(uint8*)"");
    //uint8 copy_len = 0;
    //IF(当前状态不是IDLE)
    if (MMIVT_KERNEL_STATE_IDLE != MMIVT_GetCallState())
    {
        //异常处理并return
        //SCI_TRACE_LOW:"mmivt err state: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_632_112_2_18_3_6_4_20,(uint8*)"d", MMIVT_GetCallState());
        return MMIVT_RESULT_ERROR_STATE;/*lint !e527*/
    }
    
    //电话呼出（PS状态变为ACTIVE）
    vt_result = MakeVtCall(user_dial_info->dual_sys, user_dial_info->tele_num, user_dial_info->tele_len);
    if (MMIVT_RESULT_SUCCESS != vt_result)
    {
        //异常处理
        SendMsgToUI(MMIVT_UI_MSG_QUIT, PNULL);//关闭VT
        return vt_result;
    }
    
    s_vt_kernel_info.ps_state = VT_PS_STATE_ACTIVE;
    //状态变为PS_CONNECTING;
    TranslateState(MMIVT_KERNEL_STATE_PS_CONNECTING, MMIVT_KERNEL_MSG_USER_DIAL);
    s_vt_kernel_info.state_info.sub_state = MMVT_KERNEL_SUBSTATE_NORMAL;
    
    //记录call_type、call_address、dual_sys的相关信息；
    s_vt_kernel_info.call_type = MMIVT_CALL_TYPE_MO_CALL;
    s_vt_kernel_info.call_adress.tele_len = MIN(MMIVT_PHONE_NUM_MAX_LEN, user_dial_info->tele_len);
    SCI_MEMSET((void*)s_vt_kernel_info.call_adress.tele_num, 0x00, sizeof(s_vt_kernel_info.call_adress.tele_num));
    MMI_MEMCPY(
        (void*)s_vt_kernel_info.call_adress.tele_num,
        sizeof(s_vt_kernel_info.call_adress.tele_num),
        (void*)user_dial_info->tele_num,
        user_dial_info->tele_len,
        s_vt_kernel_info.call_adress.tele_len);

    s_vt_kernel_info.disc_cause = MN_CAUSE_UNASSIGNED_NO;
    s_vt_kernel_info.dual_sys = user_dial_info->dual_sys;

    MMICC_ConstructVoiceCodec(TRUE);
    MMICC_EnableVoiceCodec(TRUE);
    //启动VT和H324模块（VT_Open、H324_Open）
    ref_open_result = MMIVT_REF_OpenVT();
    if (MMIVT_REF_RET_SUCCESS != ref_open_result || !MMIVT_324_Open(TRUE))
    {
        //启动失败，挂断VT
        SendMsgToUI(MMIVT_UI_MSG_HANGUP, PNULL);
        MMIVT_ErrorNotify( ref_open_result);
        return MMIVT_RESULT_FAILED;
    }
    
    //配置显示区域参数；
    MMIVT_REF_SetDispArea(VT_IMAGE_STATUS_ROTATE_0_DEGREE, MMIVT_DISP_MODE_PREVIEW);
    
    //开始进入Preview模式（Preview_Start）
    MMIVT_REF_StartPreview();

    //record call start time
    RecordStartTime();

    return vt_result;
    
}


/*****************************************************************************/
//  Description : HandleSetupComplete
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理对方已经接听的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleSetupComplete(void)
{
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    //SCI_TRACE_LOW:"mmivt HandleSetupComplete"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_695_112_2_18_3_6_4_21,(uint8*)"");
    //IF(当前状态不是PS_CONNECTING || 不是MO Call)
    if ((MMIVT_KERNEL_STATE_PS_CONNECTING != MMIVT_GetCallState())
        ||(MMIVT_CALL_TYPE_MO_CALL != MMIVT_GetCallType())
        )
    {
        //异常处理并return
        //SCI_TRACE_LOW:"mmivt err state:%d type:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_702_112_2_18_3_6_4_22,(uint8*)"dd", MMIVT_GetCallState(), MMIVT_GetCallType());
        return MMIVT_RESULT_ERROR_STATE;/*lint !e527*/
    }
    
    //状态变为H324_CONNECTING;
    TranslateState(MMIVT_KERNEL_STATE_H324_CONNECTING, MMIVT_KERNEL_MSG_SETUP_COMPLETE_IND);
    
    //324开始发起连接（H324_MakeCall 324状态变为ACTIVE）;
    if(!MMIVT_324_MakeCall())
    {
        SendMsgToUI(MMIVT_UI_MSG_HANGUP, PNULL);
        return MMIVT_RESULT_FAILED;        
    }
    
    s_vt_kernel_info.h324_state = VT_324_STATE_ACTIVE;
    
    return result;
}

/*****************************************************************************/
//  Description : HandleStartMedia
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理324已经建立，数据流开始交换的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleStartMedia(void)
{
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    MMIVT_REF_RET_E ref_recode = MMIVT_REF_RET_SUCCESS;

    //SCI_TRACE_LOW:"mmivt HandleStartMedia"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_730_112_2_18_3_6_4_23,(uint8*)"");
    
    //IF(当前状态是MMIVT_KERNEL_STATE_CALL_CONNECTED(324会发送3次start media,只有第一次做处理))
    if (MMIVT_KERNEL_STATE_CALL_CONNECTED == MMIVT_GetCallState())
    {
        //SCI_TRACE_LOW:"mmivt HandleStartMedia no exe"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_735_112_2_18_3_6_4_24,(uint8*)"");
        return MMIVT_RESULT_FAILED;
    }
    
    //IF(当前状态不是H324_CONNECTING)
    if (MMIVT_KERNEL_STATE_H324_CONNECTING != MMIVT_GetCallState())
    {
        //异常处理并return
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_743_112_2_18_3_6_5_25,(uint8*)"d", MMIVT_GetCallState());
        return MMIVT_RESULT_ERROR_STATE;/*lint !e527*/
    }

    //启动声音设备
    //MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_INIT, MMICC_AUDIO_DEV_MAX);
  //  if(MMIVT_CALL_TYPE_MT_CALL == MMIVT_GetCallType())
    {
        MMICC_ConstructVoiceCodec(TRUE);
        MMICC_EnableVoiceCodec(TRUE);
    }
    
    
    //状态变为CALL_CONNECTED;
    TranslateState(MMIVT_KERNEL_STATE_CALL_CONNECTED, MMIVT_KERNEL_MSG_START_MEDIA_IND);
    
    if (MMIVT_CALL_TYPE_MO_CALL == MMIVT_GetCallType())
    {
        //通知UI执行REMOTE_CONNECT;
        result = SendMsgToUI(MMIVT_UI_MSG_REMOTE_CONNECT, PNULL);
        
        //退出Preview模式（Preview_Stop）
        MMIVT_REF_StopPreview(); 
    }
    else if (MMIVT_CALL_TYPE_MT_CALL == MMIVT_GetCallType())
    {
        //通知UI执行REMOTE_CONNECT;
        result = SendMsgToUI(MMIVT_UI_MSG_CONNECT_END, PNULL);
    }
    else
    {
        //异常处理
        //SCI_TRACE_LOW:"mmivt err type:%"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_775_112_2_18_3_6_5_26,(uint8*)"", MMIVT_GetCallType());
        return MMIVT_RESULT_ERROR_STATE;
    }
    
    //配置显示区域参数；
    MMIVT_REF_SetDispArea(VT_IMAGE_STATUS_ROTATE_0_DEGREE, MMIVT_DISP_MODE_REMOTE_LOCAL);

    //配置sensor参数；
    MMIVT_REF_InitSensorParam();


    //进入通话状态，显示双方视频（VT_DialStart）
    ref_recode = MMIVT_REF_StartTalk();

    //设置电话开始时间
    RecordStartTime();

    //handle exception
    if(MMIVT_REF_RET_SUCCESS != ref_recode)
    {
        SendMsgToUI(MMIVT_UI_MSG_HANGUP, PNULL);
        //SCI_TRACE_LOW:"mmivt HandleStartMedia err result: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_796_112_2_18_3_6_5_27,(uint8*)"d", result);
    }

    return result;
}

/*****************************************************************************/
//  Description : HandleUserHangup
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理用户挂断操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUserHangup(DPARAM param)
{
    
    //disc cause要根据parm传递的值来决定，目前暂定#16
    MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause = MN_CAUSE_NORMAL_CLEARING;
    
    //SCI_TRACE_LOW:"mmivt HandleUserHangup"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_812_112_2_18_3_6_5_28,(uint8*)"");

    //停止录音
    MMIVT_StopRecord();

    //退出dial模式和preview模式
    MMIVT_REF_StopTalk();
    MMIVT_REF_StopPreview();
    
	MMICC_EnableVoiceCodec(FALSE);
	MMICC_DestroyVoiceCodec();
    //IF(324状态是ACTIVE)
    if (VT_324_STATE_ACTIVE == s_vt_kernel_info.h324_state)
    {
        //发起324断开请求（H324_Disc）
        MMIVT_324_HangUp();
        s_vt_kernel_info.h324_state = VT_324_STATE_DISCONNECTING;
    }
    //ELSE IF(PS状态是ACTIVE)
    else if(VT_PS_STATE_ACTIVE == s_vt_kernel_info.ps_state
        || VT_PS_STATE_DISCONNECTING == s_vt_kernel_info.ps_state)
    {
        //incoming call的时候 cause值为MN_CAUSE_USER_BUSY
        if (MMIVT_KERNEL_STATE_ALERTING == MMIVT_GetCallState())
        {
            
            if ((PNULL != param) &&(*(BOOLEAN*)param))//回落请求
            {
                disc_cause = MN_CAUSE_INCOMPAT_DEST;
            }
            else
            {
                disc_cause = MN_CAUSE_USER_BUSY;
            }
            
        }
        
        //发起PS断开请求（MNCALL_DiscCallEx）
        if(MN_RETURN_SUCCESS != MNCALL_DisconnectCallEx(s_vt_kernel_info.dual_sys, s_vt_kernel_info.call_id, disc_cause, PNULL))
        {
            //异常处理
            return MMIVT_RESULT_FAILED;
        }
        s_vt_kernel_info.ps_state = VT_PS_STATE_DISCONNECTING;
    }
    else
    {
        //异常处理
        //SCI_TRACE_LOW:"mmivt err ps state:%d, 324 state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_858_112_2_18_3_6_5_29,(uint8*)"dd", s_vt_kernel_info.ps_state, s_vt_kernel_info.h324_state);
        return MMIVT_RESULT_ERROR_STATE;
    }
    
    switch(MMIVT_GetCallState())
    {
    case MMIVT_KERNEL_STATE_PS_CONNECTING:
    case MMIVT_KERNEL_STATE_CONNECT_PENDING:
    case MMIVT_KERNEL_STATE_H324_CONNECTING:
    case MMIVT_KERNEL_STATE_ALERTING:

        if ( MMIVT_CALL_TYPE_MT_CALL == MMIVT_GetCallType() )
        {
            s_is_mt_received = TRUE;
        }
        //状态变为DISCONNECTING
        TranslateState(MMIVT_KERNEL_STATE_DISCONNECTING, MMIVT_KERNEL_MSG_USER_HANGUP);

        break;
        
    case MMIVT_KERNEL_STATE_CALL_CONNECTED:
        //状态变为DISCONNECTING
        TranslateState(MMIVT_KERNEL_STATE_DISCONNECTING, MMIVT_KERNEL_MSG_USER_HANGUP);
        break;
        
    default:
        //异常处理；
        //SCI_TRACE_LOW:"mmivt HandleUserHangup err state: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_885_112_2_18_3_6_5_30,(uint8*)"d", MMIVT_GetCallState());
        return MMIVT_RESULT_ERROR_STATE;
    }

    
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : Handle324ClosedInd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理324开始断开操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E Handle324ClosedInd(void)
{
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;

    //SCI_TRACE_LOW:"mmivt Handle324ClosedInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_901_112_2_18_3_6_5_31,(uint8*)"");
    //IF（324状态是IDLE）
    if (VT_324_STATE_IDLE == s_vt_kernel_info.h324_state)
    {
        //异常处理并return
        //SCI_TRACE_LOW:"mmivt err 324 state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_906_112_2_18_3_6_5_32,(uint8*)"");
        return MMIVT_RESULT_ERROR_STATE;
    }

    s_vt_kernel_info.h324_state = VT_324_STATE_DISCONNECTING;

    //停止录音
    MMIVT_StopRecord();

    //退出dial模式和preview模式
    MMIVT_REF_StopTalk();
    MMIVT_REF_StopPreview();
    //record stop time
    RecordStopTime();


    
    switch(MMIVT_GetCallState())
    {		
    case MMIVT_KERNEL_STATE_H324_CONNECTING:
    case MMIVT_KERNEL_STATE_CALL_CONNECTED:
        //状态变为DISCONNECTING;
        TranslateState(MMIVT_KERNEL_STATE_DISCONNECTING, MMIVT_KERNEL_MSG_324_CLOSED_IND);
        //通知UI执行DISCONNECT
        result = SendMsgToUI(MMIVT_UI_MSG_DISCONNECT, PNULL);
        break;

    case MMIVT_KERNEL_STATE_DISCONNECTING:
        //状态变为DISCONNECTING;
        TranslateState(MMIVT_KERNEL_STATE_DISCONNECTING, MMIVT_KERNEL_MSG_324_CLOSED_IND);
        result = SendMsgToUI(MMIVT_UI_MSG_DISCONNECT, PNULL);
        break;
        
    case MMIVT_KERNEL_STATE_CONNECT_PENDING:
        //DO NOTHING
        break;
        
    default:
        //异常处理；
        //SCI_TRACE_LOW:"mmivt Handle324ClosedInd err state: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_945_112_2_18_3_6_5_33,(uint8*)"d", MMIVT_GetCallState());
        return MMIVT_RESULT_ERROR_STATE;/*lint !e527*/
    }
    return result;
    
}

/*****************************************************************************/
//  Description : Handle324CloseCnf
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理324断开完成的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E Handle324CloseCnf(void)
{	
    MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause = MN_CAUSE_NORMAL_CLEARING;
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    //SCI_TRACE_LOW:"mmivt Handle324CloseCnf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_960_112_2_18_3_6_5_34,(uint8*)"");
    //IF（状态不是DISCONNECTING）
    if (MMIVT_KERNEL_STATE_DISCONNECTING != MMIVT_GetCallState())
    {
        //异常处理并return
        //SCI_TRACE_LOW:"mmivt err state: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_965_112_2_18_3_6_5_35,(uint8*)"d", MMIVT_GetCallState());
        return MMIVT_RESULT_ERROR_STATE;
    }
    //IF(324状态是DISCONNECTING)
    if(VT_324_STATE_DISCONNECTING == s_vt_kernel_info.h324_state)
    {
        //324状态是IDLE
        s_vt_kernel_info.h324_state = VT_324_STATE_IDLE;
    }
    else
    {
        //异常处理并return 
        //SCI_TRACE_LOW:"mmivt err 324 state: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_977_112_2_18_3_6_5_36,(uint8*)"d", s_vt_kernel_info.h324_state);
        return MMIVT_RESULT_ERROR_STATE;
    }

    switch(s_vt_kernel_info.ps_state) 
    {
    case VT_PS_STATE_ACTIVE:
        //发起PS断开请求（MNCALL_DiscCallEx）
        if(MN_RETURN_SUCCESS != MNCALL_DisconnectCallEx(s_vt_kernel_info.dual_sys, s_vt_kernel_info.call_id, disc_cause, PNULL))
        {
            //异常处理
            return MMIVT_RESULT_FAILED;
        }
        //PS状态变为DISCONNECTING
        s_vt_kernel_info.ps_state = VT_PS_STATE_DISCONNECTING;
    	break;

    case VT_PS_STATE_DISCONNECTING:
        //DONOTHING
    	break;

    case VT_PS_STATE_IDLE:
        //状态变为IDLE
        TranslateState(MMIVT_KERNEL_STATE_IDLE, MMIVT_KERNEL_MSG_324_CLOSED_CNF);

        //关闭324和ref
        MMIVT_324_Close();
        MMIVT_REF_CloseVT();

        //audio deinit
        //MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_CLOSE, MMICC_AUDIO_DEV_MAX);
        MMICC_EnableVoiceCodec(FALSE);
        MMICC_DestroyVoiceCodec();

        //IF(VT回落电话)
        if (IsRemoteFallBackReq())
        {
            //通知UI执行REMOTE_FB_REQ;
            result = SendMsgToUI(MMIVT_UI_MSG_REMOTE_FB_REQ, PNULL);
        }
        //ELSE
        else
        {
            //通知UI执行DISC_END;
            result = SendMsgToUI(MMIVT_UI_MSG_DISC_END, PNULL);
        }   
    	break;

    default:
        //异常处理并return
        //SCI_TRACE_LOW:"mmivt err ps state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1027_112_2_18_3_6_5_37,(uint8*)"d", s_vt_kernel_info.ps_state);
        return MMIVT_RESULT_ERROR_STATE;/*lint !e527*/

    }
    
    return result;
    
}


#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : HandleDisconnectedInd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理PS断开完成的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleDisconnectedInd(DPARAM param)
{
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    MMIVT_KERNEL_INFO_DISCONNECTED_IND_T* dis_ind_info = (MMIVT_KERNEL_INFO_DISCONNECTED_IND_T*)param;

    s_vt_kernel_info.disc_cause = dis_ind_info->disconnected_cause;
    
    SaveToCallLog();
#ifdef MMIPB_MOST_USED_SUPPORT
    //+CR251638
    //Save to the Most Used list in Phonebook.
    SaveToMostUsedList();
#endif
    //-CR251638

    //SCI_TRACE_LOW:"mmivt HandleDisconnectedInd cause %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1056_112_2_18_3_6_5_38,(uint8*)"d", MMIVT_GetDiscCause());
    //IF（PS状态为ACTIVE || DISCONNECTING）	
    if((VT_PS_STATE_ACTIVE == s_vt_kernel_info.ps_state)
        ||(VT_PS_STATE_DISCONNECTING == s_vt_kernel_info.ps_state))
    {
        //PS状态变为IDLE；
        s_vt_kernel_info.ps_state = VT_PS_STATE_IDLE;
    }
    else
    {
        //异常处理并return 
        //SCI_TRACE_LOW:"mmivt err ps state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1067_112_2_18_3_6_5_39,(uint8*)"d",s_vt_kernel_info.ps_state);
        return MMIVT_RESULT_ERROR_STATE;
    }
    //停止录音
    MMIVT_StopRecord();

    //退出dial模式和preview模式
    MMIVT_REF_StopTalk();
    MMIVT_REF_StopPreview();


    
    //IF（324状态是ACTIVE）
    if(VT_324_STATE_ACTIVE == s_vt_kernel_info.h324_state)
    {
        //发起324断开请求（H324_Disc）
        MMIVT_324_HangUp();
        //324状态变为DISCONNECTING
        s_vt_kernel_info.h324_state = VT_324_STATE_DISCONNECTING;
        //record stop time
        RecordStopTime();
    }
    switch(MMIVT_GetCallState())
    {
    case MMIVT_KERNEL_STATE_DISCONNECTING:
        //IF（324状态是IDLE）
        if(VT_324_STATE_IDLE == s_vt_kernel_info.h324_state)
        {
            //状态变为IDLE;
            TranslateState(MMIVT_KERNEL_STATE_IDLE, MMIVT_KERNEL_MSG_DISCONNECTED_IND);
            
            //关闭324和ref
            MMIVT_324_Close();
            MMIVT_REF_CloseVT();

            //audio deinit
            //MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_CLOSE, MMICC_AUDIO_DEV_MAX);
            MMICC_EnableVoiceCodec(FALSE);
            MMICC_DestroyVoiceCodec();

            //IF(VT回落电话)
            if (IsRemoteFallBackReq())
            {
                //通知UI执行REMOTE_FB_REQ;
                result = SendMsgToUI(MMIVT_UI_MSG_REMOTE_FB_REQ, PNULL);
            }
            //ELSE
            else
            {
                //通知UI执行DISC_END;
                result = SendMsgToUI(MMIVT_UI_MSG_DISC_END, PNULL);
            }   
            
        }
        //ELSE IF(324状态是DISCONNECTING
        else if(VT_324_STATE_DISCONNECTING == s_vt_kernel_info.h324_state)
        {	
            //DO NOTHING}
        }
        else
        {
            //异常处理并return 
            //SCI_TRACE_LOW:"mmivt err 324 state:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1129_112_2_18_3_6_5_40,(uint8*)"d", s_vt_kernel_info.h324_state);
            return MMIVT_RESULT_ERROR_STATE;
        }
        break;
        
    case MMIVT_KERNEL_STATE_PS_CONNECTING:
        //状态变为IDLE；
        TranslateState(MMIVT_KERNEL_STATE_IDLE, MMIVT_KERNEL_MSG_DISCONNECTED_IND);
        
        //关闭324和ref
        MMIVT_324_Close();
        MMIVT_REF_CloseVT();

        //IF(VT回落电话)
        if (IsRemoteFallBackReq())
        {
            //通知UI执行REMOTE_FB_REQ;
            result = SendMsgToUI(MMIVT_UI_MSG_REMOTE_FB_REQ, PNULL);
        }
        //ELSE
        else
        {
            //通知UI执行DISC_END;
            result = SendMsgToUI(MMIVT_UI_MSG_DISC_END, PNULL);
        }   

        break;
        
    case MMIVT_KERNEL_STATE_CONNECT_PENDING:
    case MMIVT_KERNEL_STATE_ALERTING:
        //状态变为IDLE；
        TranslateState(MMIVT_KERNEL_STATE_IDLE, MMIVT_KERNEL_MSG_DISCONNECTED_IND);
        //通知UI执行DISC_END;

        //关闭324和ref
        MMIVT_324_Close();
        MMIVT_REF_CloseVT();

        result = SendMsgToUI(MMIVT_UI_MSG_DISC_END, PNULL);

        break;

        
    case MMIVT_KERNEL_STATE_H324_CONNECTING:
    case MMIVT_KERNEL_STATE_CALL_CONNECTED:
        //状态变为DISCONNECTING
        TranslateState(MMIVT_KERNEL_STATE_DISCONNECTING, MMIVT_KERNEL_MSG_DISCONNECTED_IND);

        //通知UI执行DISCONNECT
        result = SendMsgToUI(MMIVT_UI_MSG_DISCONNECT, PNULL);
        
        break;
        
    default:
        //异常处理；
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1184_112_2_18_3_6_6_41,(uint8*)"d", MMIVT_GetCallState());
        return MMIVT_RESULT_ERROR_STATE;
        
    }



    return result;
}
#endif

/*****************************************************************************/
//  Description : HandleIncomingCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理来电操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleIncomingCall(DPARAM param)
{ 
    MMIVT_KERNEL_INFO_INCOMING_CALL_T* info_incoming_call_info = (MMIVT_KERNEL_INFO_INCOMING_CALL_T*)param;
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    //SCI_TRACE_LOW:"mmivt HandleIncomingCall"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1203_112_2_18_3_6_6_42,(uint8*)"");

    if (PNULL == info_incoming_call_info)
    {
        //SCI_TRACE_LOW:"mmivt param null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1207_112_2_18_3_6_6_43,(uint8*)"");
        return MMIVT_RESULT_ERROR_PARM;
    }

    //如果已经处于通话中，自动挂断（有且只能有一通视频通话）
    if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {
        MN_RETURN_RESULT_E dis_result = MN_RETURN_SUCCESS;

        dis_result = MNCALL_DisconnectCallEx(info_incoming_call_info->dual_sys, info_incoming_call_info->call_id, MN_CAUSE_USER_BUSY, PNULL);
        //SCI_TRACE_LOW:"mmivt disc incoming vt res:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1217_112_2_18_3_6_6_44,(uint8*)"d", dis_result);

        return MMIVT_RESULT_FAILED;
    }

#if 0 //def STREAMING_HS_SUPPORT // chenxiang 20100310 
    MMIAPISMP_CloseSMP(TRUE);
#endif // STREAMING_HS_SUPPORT

    //IF（当前状态不是IDLE）
    if(MMIVT_KERNEL_STATE_IDLE != MMIVT_GetCallState())
    {
        //异常处理并return 
        //SCI_TRACE_LOW:"mmivt err state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1230_112_2_18_3_6_6_45,(uint8*)"");
        return MMIVT_RESULT_ERROR_STATE;
    }
    

    
    //状态变为PS_CONECTING
    TranslateState(MMIVT_KERNEL_STATE_PS_CONNECTING, MMIVT_KERNEL_MSG_INCOMING_CALL_IND);
    s_vt_kernel_info.state_info.sub_state = MMVT_KERNEL_SUBSTATE_NORMAL;
    
    //PS状态变为ACTIVE
    s_vt_kernel_info.ps_state = VT_PS_STATE_ACTIVE;

    s_is_mt_received = FALSE;

    SCI_MEMSET(&(s_vt_kernel_info.call_adress), 0x00, sizeof((s_vt_kernel_info.call_adress)));
    //记录call_type、call_address、dual_sys的相关信息；
    s_vt_kernel_info.call_type = MMIVT_CALL_TYPE_MT_CALL;
    s_vt_kernel_info.dual_sys = info_incoming_call_info->dual_sys;
    s_vt_kernel_info.call_adress.is_number_present = info_incoming_call_info->calling_num_present;
    s_vt_kernel_info.disc_cause = MN_CAUSE_UNASSIGNED_NO;
    s_vt_kernel_info.call_id = info_incoming_call_info->call_id;
    if (s_vt_kernel_info.call_adress.is_number_present)
    {
        SetAddressInfoByBCDNum(&(info_incoming_call_info->calling_num));
    }

    

    
    return result;
    
}

/*****************************************************************************/
//  Description : HandleAlertingInd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理本方振铃请求的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleAlertingInd(void)
{
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    MMIVT_REF_RET_E ref_open_result = MMIVT_REF_RET_SUCCESS;
    //SCI_TRACE_LOW:"mmivt HandleAlertingInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1272_112_2_18_3_6_6_46,(uint8*)"");
    //IF（当前状态不是PS_CONECTING || 不是MT Call）
    if ((MMIVT_KERNEL_STATE_PS_CONNECTING != MMIVT_GetCallState())
        ||(MMIVT_CALL_TYPE_MT_CALL != MMIVT_GetCallType()))
    {
        //异常处理并return
        //SCI_TRACE_LOW:"mmivt err state:%d, type:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1278_112_2_18_3_6_6_47,(uint8*)"dd",MMIVT_GetCallState(), MMIVT_GetCallType());
        return MMIVT_RESULT_ERROR_STATE;
    }
    
    //状态变为ALERTING
    TranslateState(MMIVT_KERNEL_STATE_ALERTING, MMIVT_KERNEL_MSG_ALERTING_IND);
    
    //向PS发送消息（MNCALL_IndicateAlertEx）
    MNCALL_IndicateAlertEx(s_vt_kernel_info.dual_sys, s_vt_kernel_info.call_id, PNULL);

    //通知UI执行REMOTE_INCOMING;
    {
        result = SendMsgToUI(MMIVT_UI_MSG_REMOTE_INCOMING, PNULL);
    }

    //启动VT和H324模块（VT_Open、H324_Open）
    ref_open_result = MMIVT_REF_OpenVT();
    if (MMIVT_REF_RET_SUCCESS != ref_open_result || !MMIVT_324_Open(FALSE))
    {
        //启动失败，挂断VT
        MMIVT_ErrorNotify(ref_open_result);
        SendMsgToUI(MMIVT_UI_MSG_HANGUP, PNULL);
        return MMIVT_RESULT_FAILED;
    }

    //record start time
    RecordStartTime();
    return result;
    
}

/*****************************************************************************/
//  Description : HandleConnectCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理接听电话的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleConnectCall(void)
{
    //SCI_TRACE_LOW:"mmivt HandleConnectCall"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1315_112_2_18_3_6_6_48,(uint8*)"");
    //IF(当前状态不是ALERTING || 不是MT CALL)
    if((MMIVT_KERNEL_STATE_ALERTING != MMIVT_GetCallState())
        ||(MMIVT_CALL_TYPE_MT_CALL != MMIVT_GetCallType())
        )
    {
        //异常处理并return 
        //SCI_TRACE_LOW:"mmivt err state:%d, type:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1322_112_2_18_3_6_6_49,(uint8*)"dd",MMIVT_GetCallState(), MMIVT_GetCallType());
        return MMIVT_RESULT_ERROR_STATE;
    }
    
    //接听电话（MNCALL_ConnectCallEx）
    if(MN_RETURN_SUCCESS != MNCALL_ConnectCallEx(
        s_vt_kernel_info.dual_sys,
        s_vt_kernel_info.call_id, 
        PNULL
        )
        )
    {
        //异常处理
        SendMsgToUI(MMIVT_UI_MSG_HANGUP, PNULL);
        return MMIVT_RESULT_FAILED;
    }
    
    //状态变为CONNECT_PENDING
    TranslateState(MMIVT_KERNEL_STATE_CONNECT_PENDING, MMIVT_KERNEL_MSG_CONNECT_CALL);
    
    s_is_mt_received = TRUE;

    //?通知UI执行CONNECT
    
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : HandleConnectInd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理接听完成的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleConnectInd(void)
{
    //SCI_TRACE_LOW:"mmivt HandleConnectInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1355_112_2_18_3_6_6_50,(uint8*)"");
    //IF(当前状态不是CONNECT_PENDING || 不是MT CALL)
    if((MMIVT_KERNEL_STATE_CONNECT_PENDING != MMIVT_GetCallState())
        ||(MMIVT_CALL_TYPE_MT_CALL != MMIVT_GetCallType())
        )
    {
        //异常处理并return
        //SCI_TRACE_LOW:"mmivt err state:%d, type:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1362_112_2_18_3_6_6_51,(uint8*)"dd",MMIVT_GetCallState(), MMIVT_GetCallType());
        return MMIVT_RESULT_ERROR_STATE;
    }
    
    //状态变为H324_CONNECTING
    TranslateState(MMIVT_KERNEL_STATE_H324_CONNECTING, MMIVT_KERNEL_MSG_CONNECT_IND);
    //324开始发起连接（H324_ListenFor 324的状态变为ACTIVE）
    if(!MMIVT_324_ConnectReq())
    {
        //324 接听不成功异常处理
        SendMsgToUI(MMIVT_UI_MSG_HANGUP, PNULL);  
        return MMIVT_RESULT_FAILED;
    }
    
    s_vt_kernel_info.h324_state = VT_324_STATE_ACTIVE;
    
    //for test
    //MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_START_MEDIA_IND, PNULL);

    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : TranslateState
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:vt的状态切换函数
/*****************************************************************************/
LOCAL void TranslateState(MMIVT_KERNEL_STATE_E cur_state, MMIVT_KERNEL_MSG_E action)
{
    s_vt_kernel_info.state_info.last_state = s_vt_kernel_info.state_info.cur_state;
    s_vt_kernel_info.state_info.cur_state = cur_state;
    s_vt_kernel_info.state_info.last_action = action;
}

/*****************************************************************************/
//  Description : MakeVtCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:呼出视频电话
/*****************************************************************************/
LOCAL MMIVT_RESULT_E MakeVtCall(MN_DUAL_SYS_E dual_sys, uint8* tele_num, uint8 tele_len)
{
    MN_CALLED_NUMBER_T call_number = {0};
    
    //SCI_TRACE_LOW:"mmivt MakeVtCall"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1403_112_2_18_3_6_6_52,(uint8*)"");
    MMIVT_GetCallBCDNumber(&call_number);
    
    if (MN_RETURN_SUCCESS != MNCALL_StartCallEx(dual_sys, MN_CALL_TYPE_DATA, 
        &(call_number), 
        PNULL, 
        MMIAPISET_GetClirType(dual_sys), 
        PNULL ) )
    {
        //异常处理
        return MMIVT_RESULT_START_PS_FAILED; 
    }
    
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : SendMsgToUI
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:发送消息给UI
/*****************************************************************************/
LOCAL MMIVT_RESULT_E SendMsgToUI(MMIVT_UI_MSG_E msg, DPARAM param)
{
    return MMIVT_HandleUIMsg(msg, param);
}

/*****************************************************************************/
//  Description : MMIVT_SetAddressInfoByNormalNum
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:according to normal num set VT address info (MO calling)
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIVT_SetAddressInfoByNormalNum( uint8* tele_num, uint8 tele_len)
{
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    if( TRUE != MMIAPICOM_GenPartyNumber(tele_num, tele_len, &party_num)	)
    {
        //SCI_TRACE_LOW:"mmivt genPartyNum error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1437_112_2_18_3_6_6_53,(uint8*)"");
        return MMIVT_RESULT_ERROR_PARM;
    }
    SCI_MEMSET(&(s_vt_kernel_info.call_adress), 0x00, sizeof((s_vt_kernel_info.call_adress)));
    s_vt_kernel_info.call_adress.tele_len = MIN(MMIVT_PHONE_NUM_MAX_LEN, tele_len);
    MMI_MEMCPY(
        (void*)s_vt_kernel_info.call_adress.tele_num,
        s_vt_kernel_info.call_adress.tele_len,
        (void*)tele_num,
        tele_len,
        s_vt_kernel_info.call_adress.tele_len
        );
    s_vt_kernel_info.call_adress.tele_len = tele_len;
    s_vt_kernel_info.call_adress.is_number_present = TRUE;

    //record bcd num
    s_vt_kernel_info.call_adress.bcd_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
    s_vt_kernel_info.call_adress.bcd_num.number_type = party_num.number_type;
    {
        uint8 copy_len = ( party_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: party_num.num_len;
        MMI_MEMCPY(
            (void*)s_vt_kernel_info.call_adress.bcd_num.party_num,
            sizeof(s_vt_kernel_info.call_adress.bcd_num.party_num),
            (void*)party_num.bcd_num,
            sizeof(party_num.bcd_num),
            copy_len
            );
        s_vt_kernel_info.call_adress.bcd_num.num_len = copy_len ;
    }

    //record pb info
    {
        SetAdressPBInfo();
    }
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : SetAddressInfoByBCDNum
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:according to normal num set VT address info (mt calling)
/*****************************************************************************/
LOCAL void SetAddressInfoByBCDNum( MN_CALLING_NUMBER_T* bcd_num)
{
    uint8 tel_num[MMIVT_PHONE_NUM_MAX_LEN + 2] = {0};    
    uint8 tel_num_len = 0;
    uint8 copy_len = 0;
    
    if(PNULL == bcd_num)
    {
        //SCI_TRACE_LOW:"SetAddressInfoByBCDNum bcd_num is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1486_112_2_18_3_6_6_54,(uint8*)"");
        return;
    }
    //SCI_MEMSET(&(s_vt_kernel_info.call_adress), 0x00, sizeof((s_vt_kernel_info.call_adress)));

    //record bcd num
    s_vt_kernel_info.call_adress.bcd_num.number_plan = bcd_num->number_plan;
    s_vt_kernel_info.call_adress.bcd_num.number_type = bcd_num->number_type;

    copy_len = ( bcd_num->num_len > MMIPB_BCD_NUMBER_MAX_LEN ) ? MMIPB_BCD_NUMBER_MAX_LEN : bcd_num->num_len;
    MMI_MEMCPY(
        s_vt_kernel_info.call_adress.bcd_num.party_num,
        sizeof(s_vt_kernel_info.call_adress.bcd_num.party_num),
        bcd_num->party_num,
        sizeof(bcd_num->party_num),
        copy_len
        );
    s_vt_kernel_info.call_adress.bcd_num.num_len = copy_len;
    
    
    tel_num_len = MMIAPICOM_GenNetDispNumber(
        bcd_num->number_type, 
        (uint8)bcd_num->num_len,
        (uint8*)bcd_num->party_num,
        tel_num,
        (uint8)sizeof(tel_num)
        );

    
    //get normal number
    s_vt_kernel_info.call_adress.tele_len = tel_num_len;
    if (tel_num_len >0)
    { 
        s_vt_kernel_info.call_adress.tele_len = MIN(MMIVT_PHONE_NUM_MAX_LEN, tel_num_len);
        MMI_MEMCPY(
            s_vt_kernel_info.call_adress.tele_num,
            sizeof(s_vt_kernel_info.call_adress.tele_num),
            tel_num,
            tel_num_len,
            s_vt_kernel_info.call_adress.tele_len
            );
    }
    
    //record pb info
    if (bcd_num->num_len >0)
    {
        SetAdressPBInfo();
    }

}

/*****************************************************************************/
//  Description : SetAdressPBInfo
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void SetAdressPBInfo( void)
{
    //MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    MMIPB_BCD_NUMBER_T	pb_num = {0};
    MMI_STRING_T temp_name = {0};
    wchar temp_name_str[MMIVT_MAX_PBNAME_LEN + 1] = {0};
    uint8 copy_len = 0;
    BOOLEAN result = FALSE;

    temp_name.wstr_ptr = temp_name_str;
    
    pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(s_vt_kernel_info.call_adress.bcd_num.number_type,s_vt_kernel_info.call_adress.bcd_num.number_plan);
    copy_len = ( s_vt_kernel_info.call_adress.bcd_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: s_vt_kernel_info.call_adress.bcd_num.num_len;
    MMI_MEMCPY(
        (void*)pb_num.number,
        sizeof(pb_num.number),
        (void*)s_vt_kernel_info.call_adress.bcd_num.party_num,
        sizeof(s_vt_kernel_info.call_adress.bcd_num.party_num),
        copy_len
        );
    pb_num.number_len = copy_len;
    result = MMIAPIPB_GetNameByNumber(&pb_num, &temp_name, CC_MAX_PBNAME_LEN, TRUE) ;/*lint !e506 */
    
    s_vt_kernel_info.call_adress.is_exit_in_pb = result;
    
    if (result && (temp_name.wstr_len > 0))
    {
        s_vt_kernel_info.call_adress.name_len = temp_name.wstr_len;
        MMI_WSTRNCPY(
            s_vt_kernel_info.call_adress.name,
            s_vt_kernel_info.call_adress.name_len,
            temp_name.wstr_ptr,
            temp_name.wstr_len,
            temp_name.wstr_len
            );
    }
    else
    {
        //PB中没有该号码，以号码作为名字
        uint16 copy_str_len = s_vt_kernel_info.call_adress.tele_len;//( s_vt_kernel_info.call_adress.tele_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : s_vt_kernel_info.call_adress.tele_len;/*lint !e506 */
        s_vt_kernel_info.call_adress.name_len = copy_str_len;
        MMI_STRNTOWSTR(
            (void*)s_vt_kernel_info.call_adress.name,
            s_vt_kernel_info.call_adress.name_len,
            (void*)s_vt_kernel_info.call_adress.tele_num,
            copy_str_len,
            copy_str_len
            );
    }    
}

/*****************************************************************************/
//  Description : MMIAPIVT_GetCallName
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIAPIVT_GetCallName( MMI_STRING_T* out_name_str_ptr)
{
    if(PNULL == out_name_str_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIVT_GetCallName out_name_str_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1600_112_2_18_3_6_6_55,(uint8*)"");
        return MMIVT_RESULT_FAILED;
    }

    out_name_str_ptr->wstr_ptr = s_vt_kernel_info.call_adress.name;
    out_name_str_ptr->wstr_len = s_vt_kernel_info.call_adress.name_len;

    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : MMIVT_GetCallState
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_KERNEL_STATE_E MMIVT_GetCallState( void)
{
    return s_vt_kernel_info.state_info.cur_state;
}

/*****************************************************************************/
//  Description : MMIVT_GetSubState
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMVT_KERNEL_SUBSTATE_E MMIVT_GetSubState( void)
{
    return s_vt_kernel_info.state_info.sub_state;
}

/*****************************************************************************/
//  Description : MMIVT_GetDualSys
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIVT_GetDualSys( void)
{
    return s_vt_kernel_info.dual_sys;
}

/*****************************************************************************/
//  Description : MMIVT_SetDualSys
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIVT_SetDualSys(MN_DUAL_SYS_E dual_sys)
{
    s_vt_kernel_info.dual_sys = dual_sys;
}


/*****************************************************************************/
//  Description : MMIVT_GetCallType
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_CALL_TYPE_E MMIVT_GetCallType( void)
{
    return s_vt_kernel_info.call_type;
}

/*****************************************************************************/
//  Description : MMIVT_GetCallBCDNumber
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_GetCallBCDNumber( MN_CALLED_NUMBER_T *bcd_number_ptr)
{
    if(PNULL == bcd_number_ptr)
    {
        //SCI_TRACE_LOW:"MMIVT_GetCallBCDNumber bcd_number_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1654_112_2_18_3_6_6_56,(uint8*)"");
        return;
    }
    bcd_number_ptr->num_len = MIN(MN_MAX_ADDR_BCD_LEN, s_vt_kernel_info.call_adress.bcd_num.num_len);
    bcd_number_ptr->number_plan = s_vt_kernel_info.call_adress.bcd_num.number_plan;
    bcd_number_ptr->number_type = s_vt_kernel_info.call_adress.bcd_num.number_type;
    MMI_MEMCPY(
        bcd_number_ptr->party_num,
        bcd_number_ptr->num_len,
        s_vt_kernel_info.call_adress.bcd_num.party_num,
        s_vt_kernel_info.call_adress.bcd_num.num_len,
        bcd_number_ptr->num_len
        );
}

/*****************************************************************************/
//  Description : MMIVT_GetCallBCDNumber
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: out parameter
/*****************************************************************************/
PUBLIC void MMIVT_GetCallNormalNumber( uint8* out_num_ptr, uint8* out_num_len_ptr)
{
    if(PNULL == out_num_ptr ||PNULL == out_num_len_ptr)
    {
        //SCI_TRACE_LOW:"MMIVT_GetCallNormalNumber bcd_number_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1677_112_2_18_3_6_7_57,(uint8*)"");
        return;
    }

    *out_num_len_ptr = s_vt_kernel_info.call_adress.tele_len;
    MMI_MEMCPY(
        out_num_ptr,
        *out_num_len_ptr,
        s_vt_kernel_info.call_adress.tele_num,
        s_vt_kernel_info.call_adress.tele_len,
        *out_num_len_ptr
        );
    *out_num_len_ptr = s_vt_kernel_info.call_adress.tele_len;
}

/*****************************************************************************/
//  Description : MMIAPIVT_GetCallPbBCDNumber
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_GetCallPbBCDNumber( MMIPB_BCD_NUMBER_T *pb_number_ptr)
{
    MN_CALLED_NUMBER_T bcd_number = {0};
    MMIVT_GetCallBCDNumber(&bcd_number);

    pb_number_ptr->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(bcd_number.number_type, MN_NUM_PLAN_UNKNOW);
    pb_number_ptr->number_len = MIN(MN_MAX_ADDR_BCD_LEN, bcd_number.num_len);
    MMI_MEMCPY(
        pb_number_ptr->number,
        sizeof(pb_number_ptr->number),
        (void*)bcd_number.party_num,
        sizeof(bcd_number.party_num),
        bcd_number.num_len
        );
}

/*****************************************************************************/
//  Description : MMIVT_GetCallStartTime
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIVT_GetCallStartTime( void)
{
    return s_vt_kernel_info.call_time_info.start_time;
}

/*****************************************************************************/
//  Description : MMIVT_GetDiscCause
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MN_CALL_DISCONNECT_CALL_CAUSE_E MMIVT_GetDiscCause( void)
{
    return s_vt_kernel_info.disc_cause;
}

/*****************************************************************************/
//  Description : MMIVT_GetCallDuration
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIVT_GetCallDuration( void)
{
    return MMIAPICL_GetCallDurationTime(s_vt_kernel_info.call_time_info.start_time, s_vt_kernel_info.call_time_info.stop_time);
}

/*****************************************************************************/
//  Description : RecordStartTime
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void RecordStartTime( void)
{
    s_vt_kernel_info.call_time_info.start_time = MMIAPICOM_GetCurTime();
    s_vt_kernel_info.call_time_info.stop_time = s_vt_kernel_info.call_time_info.start_time;
}

/*****************************************************************************/
//  Description : RecordStopTime
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void RecordStopTime()
{
    s_vt_kernel_info.call_time_info.stop_time = MMIAPICOM_GetCurTime();
}

/*****************************************************************************/
//  Description : 判断是否是远端的回落请求
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsRemoteFallBackReq( void)
{
    BOOLEAN result = FALSE;
    MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause = MMIVT_GetDiscCause();
    if (MMIVT_CALL_TYPE_MO_CALL == MMIVT_GetCallType())
    {
        if (MN_CAUSE_RESOURCES_UNAV == disc_cause
            || MN_CAUSE_BEAR_CAP_NOT_AUTH == disc_cause
            || MN_CAUSE_BEAR_CAP_UNAV == disc_cause
            || MN_CAUSE_INCOMPAT_DEST == disc_cause)
        {
            result = TRUE;
        }
    }
    //SCI_TRACE_LOW:"mmivt is fall back req:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1776_112_2_18_3_6_7_58,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : 接受回落请求，发起语音呼叫
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_MakeFBCall( void)
{
    MMIAPICC_MakeCall(s_vt_kernel_info.dual_sys, s_vt_kernel_info.call_adress.tele_num, s_vt_kernel_info.call_adress.tele_len, 
        PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
}
#ifdef MMIPB_MOST_USED_SUPPORT
//+CR251638
/*****************************************************************************/
//  Description : 存入常用联系人
//  Global resource dependence : 
//  Author:Buddy.
//  Note: 
/*****************************************************************************/
LOCAL void SaveToMostUsedList(void)
{
    uint8        tele_num[CC_MAX_TELE_NUM_LEN + 2]= {0};
    uint8	  tel_num_len = 0;
    BOOLEAN is_valid_number = FALSE;
    
    if( s_vt_kernel_info.call_adress.is_number_present)
    {        
        tel_num_len =  MMIAPICOM_GenNetDispNumber( s_vt_kernel_info.call_adress.bcd_num.number_type,
                                    (uint8)MIN(MN_MAX_ADDR_BCD_LEN, s_vt_kernel_info.call_adress.bcd_num.num_len),
                                    (uint8*)s_vt_kernel_info.call_adress.bcd_num.party_num,
                                    tele_num,
                                    (CC_MAX_TELE_NUM_LEN + 2));

        if (MMIAPICOM_IsValidNumberString((char *)tele_num, tel_num_len))
        {
            is_valid_number = TRUE;
        }
    }

    //SCI_TRACE_LOW:"SaveToMostUsedList::is_valid_number = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1815_112_2_18_3_6_7_59,(uint8*)"d", is_valid_number);
    if (is_valid_number)
    {
        MMIAPIPB_SetNumberUsed(tele_num, tel_num_len);
    }
}
//-CR251638  
#endif
#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : 存入通话记录
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SaveToCallLog(void)
{
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_DIALED;

    MMICL_CALL_INFO_T	call_info = {0};
    call_info.call_time.start_time = s_vt_kernel_info.call_time_info.start_time;
    call_info.call_time.stop_time = s_vt_kernel_info.call_time_info.stop_time;
    call_info.call_type = CC_CALL_VIDEO;
    call_info.dual_sys = s_vt_kernel_info.dual_sys;

    call_info.call_num.is_num_present = s_vt_kernel_info.call_adress.is_number_present;
    call_info.call_num.number.num_len = MIN(MN_MAX_ADDR_BCD_LEN, s_vt_kernel_info.call_adress.bcd_num.num_len);
    call_info.call_num.number.number_plan = s_vt_kernel_info.call_adress.bcd_num.number_plan;
    call_info.call_num.number.number_type = s_vt_kernel_info.call_adress.bcd_num.number_type;

    MMI_MEMCPY(
        call_info.call_num.number.party_num,
        MN_MAX_ADDR_BCD_LEN,
        s_vt_kernel_info.call_adress.bcd_num.party_num,
        MN_MAX_ADDR_BCD_LEN,
        s_vt_kernel_info.call_adress.bcd_num.num_len
        );

/*
    //+CR113258
    #if defined(CL_SAVE_NAME_SUPPORT)
    if (s_vt_kernel_info.call_adress.is_exit_in_pb)
    {
        call_info.name_len = s_vt_kernel_info.call_adress.name_len;
        MMI_WSTRNCPY(
            call_info.name,
            CC_MAX_PBNAME_LEN,
            s_vt_kernel_info.call_adress.name,
            MMIVT_MAX_PBNAME_LEN,
            call_info.name_len
            );
    }
    #endif
    //-CR113258
*/

    if (MMIVT_CALL_TYPE_MT_CALL == MMIVT_GetCallType())
    {
        if (s_is_mt_received)
        {
            call_type = MMICL_CALL_RECEIVED;
        }
        else
        {
            call_type = MMICL_CALL_MISSED;
        }
    }

    if (MMICL_CALL_MISSED == call_type)
    {
        MMIAPICC_HandleMissedCall();
    }

    MMIAPICL_RecordCallInfo(s_vt_kernel_info.dual_sys, call_type, call_info);
}
#endif

/*****************************************************************************/
//  Description : 处理userinput消息.....
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void HandleUserInput(DPARAM param)
{
    wchar* user_str_ptr =  &(((APP_MN_WCHAR_DATA_T*)param)->data);

    const wchar mmr_stop_str[] = {0x4443, 0};//{'C', 'D', 0}; //"CD"
    const wchar mmr_start_str[] = {0x4343, 0};//{'C', 'C', 0}; //"CC"

    //SCI_TRACE_LOW:"mmivt: userinput is %s %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1900_112_2_18_3_6_7_60,(uint8*)"sd", user_str_ptr,*user_str_ptr);

    if (MMIVT_KERNEL_STATE_CALL_CONNECTED != MMIVT_GetCallState())
    {
        //SCI_TRACE_LOW:"mmivt HandleUserInput err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1904_112_2_18_3_6_7_61,(uint8*)"d", MMIVT_GetCallState());
        return ;
    }

    if(MMVT_KERNEL_SUBSTATE_MMR == MMIVT_GetSubState() &&( 0 == MMIAPICOM_Wstrncmp(user_str_ptr, mmr_stop_str, 1)))	//如果收到"CD"的user input,确定是mmr收费信号
    {
    	//SCI_TRACE_LOW:"mmivt quit mmr"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1910_112_2_18_3_6_7_62,(uint8*)"");
    	//通知UI退出多媒体彩铃
        SendMsgToUI(MMIVT_UI_MSG_QUIT_MMR, PNULL);
        s_vt_kernel_info.state_info.sub_state = MMVT_KERNEL_SUBSTATE_NORMAL;

        //重新开始计时
        RecordStartTime();
    }

	if ( 0 == MMIAPICOM_Wstrncmp(user_str_ptr, mmr_start_str, 1))
	{
		//SCI_TRACE_LOW:"mmivt enter mmr"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_PROCESS_1921_112_2_18_3_6_7_63,(uint8*)"");
		s_vt_kernel_info.state_info.sub_state = MMVT_KERNEL_SUBSTATE_MMR;
        //通知UI进入多媒体彩铃
        SendMsgToUI(MMIVT_UI_MSG_ENTER_MMR, PNULL);
	}
    
}
#endif //VT_SUPPORT
    
    
#ifdef   __cplusplus
    }
#endif
    
//#endif //_MMIVT_KERNEL_PROCESS_C_

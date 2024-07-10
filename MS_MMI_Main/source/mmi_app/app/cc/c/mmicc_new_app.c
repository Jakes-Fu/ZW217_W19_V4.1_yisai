/*****************************************************************************
** Copyright 2022 <Name of Copyright Owner: mingwei.jia>                     *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/
/*****************************************************************************
** File Name:      mmicc_new_app.c                                           *
** Author:         mingwei.jia                                               *
** Date:           29/9/2022                                                 *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2022      mingwei.jia           Create                                 *
******************************************************************************/

/*---------------------------------------------------------------------------*
**                              INCLUDE FILES                                *
**---------------------------------------------------------------------------*/
#include "mmi_app_cc_trc.h"
#include "mmicc_export.h"
#include "mmicc_internal.h"
#include "mmicc_id.h"
#include "mmicc_nv.h"
#include "mmicc_text.h"

#include "mmi_id.h"
#include "mmiidle_statusbar.h"

#include "ual_tele_common.h"
#include "ual_common.h"
#include "ual_tele_call.h"
#include "ual_tele_sim.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmicc_internal.h"
#include "mmitv_out.h"
#include "mmiidle_export.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmidm_export.h"
#include "mmiussd_export.h"
#include "mmipub.h"
#include "mmicc_audio.h"
#include "watch_cc_view.h"
#include "mmieng_export.h"
#include "mmidc_export.h"
#include "mmipicview_export.h"
#include "watch_gallery_export.h"

/*---------------------------------------------------------------------------*
**                            MACRO DEFINITION                               *
**---------------------------------------------------------------------------*/
#ifndef WIN32
#define TRACE_CCAPP(_format,...) \
        do \
        { \
            SCI_TraceLow("{%s}[%s]<%d> "_format, "CC_APP", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        }while(0)

#else
#define TRACE_CCAPP(_format,...) \
        do \
        { \
            printf("{%s}<%d>"_format, "CC_APP",  __LINE__, ##__VA_ARGS__);\
        }while(0)
#endif

#define CALL_DIRECTION_MO   (0)     //通话方向为去电
#define CALL_DIRECTION_MT   (1)     //通话方向为来电

/*---------------------------------------------------------------------------*
**                            TYPE AND CONSTANT                              *
**---------------------------------------------------------------------------*/
typedef struct
{
    uint8 call_id;          //本通CALL的ID
}CALL_INFO_T; //每通CALL的信息

typedef struct
{
    uint32 sim_id;
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 1];
    uint8 tele_num_len;
}WANT_MO_CALL_INFO_T; //want mo call的信息

typedef struct
{
    uint32     sim_id;
    MMICL_CALL_TYPE_E  calllog_type;
    MMICL_CALL_INFO_T  cl_info;//call log信息
}MMICC_CALL_LOG_INFO_T;

/*---------------------------------------------------------------------------*
**                             LOCAL VARIABLES                               *
**---------------------------------------------------------------------------*/
LOCAL CALL_INFO_T s_call_info[CC_MAX_CALL_NUM] = {0};//每通CALL的信息
LOCAL WANT_MO_CALL_INFO_T s_want_mo_call_info = {0};
LOCAL MMICC_MO_CALL_ERROR_TYPE_E  s_cc_mo_error_type = CC_MO_ERROR_NONE;
LOCAL uint32 s_missed_call_num = 0;
LOCAL ual_tele_call_dtmf_info_t s_dtmf_info = {0};
#ifdef MMI_RECORD_SUPPORT
LOCAL RECORD_SRV_HANDLE  s_cc_watch_record_handle = PNULL;
#endif
LOCAL MMICL_CALL_TYPE_E s_calllog_type = MMICL_CALL_DIALED;
LOCAL MMICC_CALL_LOG_INFO_T s_calllog_info = {0};
LOCAL uint8 s_record_calllog_timer = 0;
extern BOOLEAN s_cc_reject_call_by_unkown_call;

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                             LOCAL FUNCTION                                *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : make a call from pb or not from pb
//  Global resource dependence : s_call_context
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL CC_RESULT_E MakeCallReqExt(
    MN_DUAL_SYS_E dual_sys,
    uint8 *src_tele_num,
    uint8 src_tele_len
    );

/*****************************************************************************/
// Description : the process message function of the CC application
// Global resource dependence :
// Author:figo.feng
// Note:2003.12.01
/*****************************************************************************/
LOCAL BOOLEAN CC_HandleMsgCallback(ual_cms_msg_t param);

/*****************************************************************************/
//  Description : 记录call log信息
//  Global resource dependence :
//  Author: wei.chen1
//  Note:
/*****************************************************************************/
LOCAL void CC_RecordCallLogInfo(uint8 timer_id, uint32 param)
{
    uint8 sim_id = 0;
    MMICL_CALL_INFO_T  cl_info = {0};
    MMICL_CALL_TYPE_E  calllog_type = MMICL_CALL_MISSED;

    sim_id = s_calllog_info.sim_id;
    calllog_type = s_calllog_info.calllog_type;
    SCI_MEMCPY(&cl_info, &s_calllog_info.cl_info, sizeof(MMICL_CALL_INFO_T));

    if (s_record_calllog_timer == timer_id)
    {
        MMIAPICL_RecordCallInfo(sim_id, calllog_type, cl_info);
        SCI_TRACE_LOW("CC_RecordCallLogInfo out is %d",SCI_GetTickCount());
        if ((TRUE == MMK_IsOpenWin(WATCHCL_CALLLOG_WIN_ID)) //Bug 2143039
            && (0 == ual_tele_call_get_call_count())) //Bug 2177161
        {
            MMK_SendMsg(WATCHCL_CALLLOG_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
    }
    MMK_StopTimer(s_record_calllog_timer);
}

/*****************************************************************************/
//  Description : process when received signal MSG_UAL_TELE_CALL_MO_START_IND
//  Global resource dependence : s_call_context
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void CC_MoStartInd(ual_tele_call_mo_start_ind_t *p_mo_start_ind)
{
    uint8 index = 0;

    //check the param
    if(PNULL == p_mo_start_ind)
    {
        TRACE_CCAPP("pSig == PNULL");
        return;
    }
    TRACE_CCAPP("call_id = %d", p_mo_start_ind->call_id);

    if( p_mo_start_ind->call_id >= CC_MAX_CALL_NUM )
    {
        return;
    }

#ifdef BLUETOOTH_SUPPORT
    //#ifdef MMI_BLUETOOTH
    MMIAPIBT_OutgoingCall();
    //#endif
#endif
    // TODO:使用DPHONE_SUPPORT控制，调用MMIDEFAULT_GetHookStates判断
    //保存call_index信息
    for(index = 0; index < CC_MAX_CALL_NUM; index++)
    {
        if(CC_INVALID_ID == s_call_info[index].call_id)
        {
            s_call_info[index].call_id = p_mo_start_ind->call_id;
            TRACE_CCAPP("s_call_info[%d].call_id = %d", index, s_call_info[index].call_id);
            break;
        }
    }
#ifdef MMI_VOLTE_SUPPORT
    TRACE_CCAPP("is_ps_call = %d", p_mo_start_ind->is_ps_call);
    if(p_mo_start_ind->is_ps_call)
    {
        MMICC_UpdateCallStatusDisplay(MMICC_VOLTE_CALL_IND);
    }
#endif
}

/*****************************************************************************/
// Description : process when received signal MSG_UAL_TELE_CALL_MO_ALERT_IND
// Global resource dependence :
// Author:miao.liu2
// Note:
/*****************************************************************************/
LOCAL void CC_MoAlertingInd(ual_tele_call_mo_alert_ind_t *p_mo_alert_ind)
{
    ual_tele_call_result_e res = UAL_TELE_CALL_RES_MAX;

    //check the param
    if(PNULL== p_mo_alert_ind)
    {
        TRACE_CCAPP("p_mo_alert_ind == PNULL");
        return;
    }
    TRACE_CCAPP("ccall_id = %d, alerting_type = %d", p_mo_alert_ind->call_id, p_mo_alert_ind->alert_type);

    if(p_mo_alert_ind->call_id >= CC_MAX_CALL_NUM )
    {
        TRACE_CCAPP("call_id = %d", p_mo_alert_ind->call_id);
        return;
    }
    // TODO:使用MET_MEX_SUPPORT控制，调用MMIMex_Phs_HookResult判断

    //get amr type,if result is wb,display hd icon.
    //todo:后续通过AMR上报消息显示

    switch(p_mo_alert_ind->alert_type )
    {
        //MO,收到对端发送来的ALERT消息
        case UAL_TELE_CALL_ALERT_TYPE_REMOTE:
        {
#ifdef MMI_VOLTE_SUPPORT
            if(p_mo_alert_ind->is_ps_call)
            {
                res = ual_tele_call_stop_local_netring();
            }
            else
#endif
            {
                res = ual_tele_call_play_local_netring(MMI_1SECONDS, MMI_3SECONDS, 0, 0, 0);
            }
            TRACE_CCAPP("res: %d ", res);
            break;
        }
        //MT,indicate alerting
        case UAL_TELE_CALL_ALERT_TYPE_LOCAL:
        {
            //modify for MT Firewall
#ifdef MMI_VOLTE_SUPPORT
            if(TRUE == p_mo_alert_ind->is_ps_call)
            {
                TRACE_CCAPP("local ring type: %d ", p_mo_alert_ind->alert_type);
                res = ual_tele_call_play_local_netring(MMI_1SECONDS, MMI_3SECONDS, 0, 0, 0);
                TRACE_CCAPP("res: %d ", res);
            }
#endif
        }
        break;

    default:
        TRACE_CCAPP("alerting type: %d ", p_mo_alert_ind->alert_type);
        break;
    }
}

/*****************************************************************************/
// Description : get the index of call according to the call id
// Global resource dependence :
// Author:figo.feng
// Note:2003.12.01
/*****************************************************************************/
LOCAL uint8 CC_GetIndexByCallId(MN_DUAL_SYS_E dual_sys, uint8 call_id)
{
    uint8 i = CC_INVALID_INDEX;
    //check the param
    if( call_id >= CC_MAX_CALL_NUM )
    {
        TRACE_CCAPP("call_id: %d ", call_id);
        return CC_INVALID_INDEX;
    }
    TRACE_CCAPP("sim_id: %d, call_id: %d ", dual_sys, call_id);
    //循环查找匹配的call id
    for(i = 0; i < CC_MAX_CALL_NUM; i++)
    {
        if(call_id == s_call_info[i].call_id)
        {
            return i;
        }
    }
    TRACE_CCAPP("i: %d", i);
    return CC_INVALID_INDEX;
}

/*****************************************************************************/
// Description :
// Global resource dependence :
// Author:samboo.shen
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_IsCallIndexValid(uint8 call_index)
{
    return (call_index < CC_MAX_CALL_NUM);
}

/*****************************************************************************/
// Description : process when received signal MSG_UAL_TELE_CALL_MO_CONNECTED_CNF
// Global resource dependence :
// Author:miao.liu2
// Note:
/*****************************************************************************/
LOCAL void CC_MoConnectedCnf(ual_tele_call_mo_connected_cnf_t *p_mo_connected_ind)
{
    uint8 call_index = CC_INVALID_ID;
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS      address = {0};
#endif
    ual_tele_call_result_e res = UAL_TELE_CALL_RES_MAX;

    //check the param
    if(PNULL == p_mo_connected_ind || p_mo_connected_ind->call_id >= CC_MAX_CALL_NUM)
    {
        TRACE_CCAPP("p_mo_connected_ind = NULL || p_mo_connected_ind->call_id > 7");
        return;
    }
    TRACE_CCAPP("call_id = %d", p_mo_connected_ind->call_id);

    //get the call index according to its call id
    call_index = CC_GetIndexByCallId(p_mo_connected_ind->sim_id, p_mo_connected_ind->call_id);

    if(!CC_IsCallIndexValid(call_index))
    {
        TRACE_CCAPP("call_index %d", call_index);
        return;
    }

    //vibrate for call connect prompt
// TODO:后续封装新的震动接口
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
    MMICC_VibrateForConnectPrompt();
#endif

    //停止播放回铃音
    res = ual_tele_call_stop_local_netring();
#ifdef BLUETOOTH_SUPPORT
    //判断是否有激活的蓝牙耳机
    MMIAPIBT_CallConnect(&address);
#ifdef BT_BQB_SUPPORT
    MMIAPIBT_SetCallHeldStatus();
#endif
#endif
    // TODO:使用DPHONE_SUPPORT控制，调用MMIDEFAULT_GetHookStates判断

    MMICC_UpdateCallStatusDisplay(MMICC_MO_CONNECTED_IND);
    // TODO:使用DPHONE_SUPPORT控制，调用ResetCallHandfreeState
}

/*****************************************************************************/
// Description : record call log
// Global resource dependence :
// Author:miao.liu2
// Note:
/*****************************************************************************/
LOCAL void CC_DisconnectedCallLog(uint32 msg_id,
                                             uint8 sim_id,
                                             char tele_num[UAL_TELE_CALL_TELE_NUM_MAX + 1],
                                             uint8 tele_num_len,
                                             uint32 start_call_time, //通话起始的时间
                                             uint32 end_call_time,   //通话结束的时间
                                             uint8 call_direction   //MO or MT
                                             )
{
    MMICL_CALL_INFO_T       cl_info = {0};/*lint !e64*/
    BOOLEAN                 is_stk_call = FALSE;
    CC_CALL_TIME_T          call_time = {0};
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS      address={0};
#endif
    uint32                  call_duration = 0;

    // TODO:蓝牙相关状态更新, BLUETOOTH_SUPPORT
    //记录call log

    //停止录音
    if (!MMIAPIENG_GetIQModeStatus())
    {
#ifdef MMI_RECORD_SUPPORT
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
        MMIAPICC_StopCCRecord();//挂断的是后停止录音
#else
        MMICC_StopRecordInCall();
#endif
#endif
    }

    call_time.dual_sys = sim_id;
    call_duration = end_call_time - start_call_time;
    call_time.start_time = MMIAPICOM_GetCurTime() - call_duration;
    call_time.stop_time = MMIAPICOM_GetCurTime();
    TRACE_CCAPP("sim_id = %d, start_time = %d, stop_time = %d", call_time.dual_sys, call_time.start_time, call_time.stop_time);
    SCI_MEMSET(&cl_info, 0, sizeof(cl_info));
    MMI_MEMCPY(
        &cl_info.call_time,
        sizeof(cl_info.call_time),
        &call_time,
        sizeof(call_time),
        sizeof(CC_CALL_TIME_T)
        );

    if( tele_num_len > 0)
    {
        MMI_PARTY_NUMBER_T party_num = {0};

        cl_info.call_num.is_num_present = TRUE;
        //convert str to BCD
        if(FALSE == MMIAPICOM_GenPartyNumber(tele_num, tele_num_len, &party_num))
        {
            TRACE_CCAPP("Gen Party Number Fail.");
            return;
        }
        //save to calllog
        cl_info.call_num.number.number_type = party_num.number_type ;
        cl_info.call_num.number.number_plan = MN_NUM_PLAN_ISDN_TELE;
        SCI_MEMSET((void*)cl_info.call_num.number.party_num, 0xFF, sizeof(uint8) * MN_MAX_ADDR_BCD_LEN);//cid611198
        cl_info.call_num.number.num_len = (party_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN : party_num.num_len;
        MMI_MEMCPY(
            (void*)cl_info.call_num.number.party_num,
            sizeof(MMI_PARTY_NUMBER_T),
            (void*)party_num.bcd_num,
            sizeof(party_num.bcd_num),
            cl_info.call_num.number.num_len
            );
    }
    else
    {
        cl_info.call_num.is_num_present = FALSE;
    }
    // TODO:使用CL_SAVE_NAME_SUPPORT控制，保存name
    //是否是紧急呼叫
    if (CALL_DIRECTION_MO == call_direction)
    {
        //bug 186648 modify is_only_for_display(the last parameter of MMIAPICC_IsEmergencyNum) true to false,is_only_for_display is setted true is to some requirement,
        //but in different countries in normal status the phone make NORMAL call by 000, 08, 110, 999, 118 and 119 num(no sim emergency) ,
        //it is not normal display emergency call in window
        cl_info.is_emergency_call = MMIAPICC_IsEmergencyNum(tele_num, tele_num_len, sim_id, FALSE);
    }
    TRACE_CCAPP("call_direction = %d", call_direction);
    switch(call_direction)
    {
        case CALL_DIRECTION_MO:
            // 已拨电话//@samboo 建议将电话纪录单独提取出来，在disconnect_ind收到后处理，而不是分布在各个地方进行处理
            //call the interface of calllog
            s_calllog_type = MMICL_CALL_DIALED;
#ifdef MMI_STK_SUPPORT
            //is_stk_call = s_call_context.call_info[call_index].is_stk_call;
#endif
            break;

        case CALL_DIRECTION_MT:
            TRACE_CCAPP("msg_id = %d, start_call_time = %d", msg_id, start_call_time);
            if ((MSG_UAL_TELE_CALL_DISCONNECTED_IND == msg_id) &&
                (0 == start_call_time))
            {
                //未接来电
                s_calllog_type = MMICL_CALL_MISSED;

                s_missed_call_num ++;
                MAIN_SetMissedCallState(TRUE);
#ifdef PDA_UI_DROPDOWN_WIN
                MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_MISS_CALL);
#endif
                MMIAPICC_SaveMissedCallNumToNv();
            }
            else
            {
                //已接电话
                s_calllog_type = MMICL_CALL_RECEIVED;
                // TODO:使用ASP_SUPPORT控制
            }
            break;

        default:
            break;
    }

    //更新UI
    MMICC_UpdateCallStatusDisplay(MMICC_DISCONNECTED_IND);

    if (!is_stk_call && !s_cc_reject_call_by_unkown_call)//stk call need not save to call log
    {
        //MMIAPICL_RecordCallInfo(sim_id, s_calllog_type, cl_info );
        s_calllog_info.sim_id = sim_id;
        s_calllog_info.calllog_type = s_calllog_type;
        SCI_MEMSET(&s_calllog_info.cl_info, 0x00, sizeof(MMICL_CALL_INFO_T));
        SCI_MEMCPY(&s_calllog_info.cl_info, &cl_info, sizeof(MMICL_CALL_INFO_T));

        s_record_calllog_timer = MMK_CreateTimerCallback(MMI_500MSECONDS, CC_RecordCallLogInfo, NULL, FALSE);
    }
    // TODO:使用MMI_STATUSBAR_SCROLLMSG_SUPPORT控制
    // TODO:使用MMIPB_MOST_USED_SUPPORT控制
    /* Bug fix: cr00198061. when a call disconnected, allow key auto lock */
    MMIDEFAULT_AllowTurnOffBackLight(TRUE);

    if (0 != MMICC_GetCallNum())
    {
        MAIN_SetCallingState(TRUE);
    }
}

/*****************************************************************************/
// Description : process when received signal MSG_UAL_TELE_CALL_DISCONNECTED_IND/MSG_UAL_TELE_CALL_DISCONNECTED_CNF
// Global resource dependence :
// Author:miao.liu2
// Note:
/*****************************************************************************/
LOCAL void CC_DisconnectedCall(uint32 msg_id, void *p_disconnected_param )
{
    uint8                             call_id = CC_INVALID_ID;
    uint8                             call_index = CC_INVALID_INDEX;
    uint8                             i = 0;
    ual_tele_sim_id_e                 sim_id = UAL_SIM_ID_MAX;
    char                              tele_num[UAL_TELE_CALL_TELE_NUM_MAX + 1] = {0};
    uint8                             tele_num_len = 0;
    uint32                            start_call_time = 0;
    uint32                            end_call_time = 0;
    uint8                             call_direction = 0;
#ifdef MMI_STK_SUPPORT
    ual_tele_call_disconnect_cause_e  cause = UAL_TELE_CALL_DISC_CAUSE_ERROR_NONE;
#endif
    ual_tele_call_disconnected_cnf_t *p_disconnected_cnf = PNULL;
    ual_tele_call_disconnected_ind_t *p_disconnected_ind = PNULL;
    ual_tele_call_result_e            res = UAL_TELE_CALL_RES_MAX;

    TRACE_CCAPP("enter, msg_id = %d", msg_id);
    //check the param
    if(PNULL == p_disconnected_param)
    {
        TRACE_CCAPP("p_disconnected_param == PNULL");
        return;
    }
    if(MSG_UAL_TELE_CALL_DISCONNECTED_CNF == msg_id)
    {
        p_disconnected_cnf = (ual_tele_call_disconnected_cnf_t*)p_disconnected_param;
        sim_id = p_disconnected_cnf->sim_id;
        call_id = p_disconnected_cnf->call_id;
        start_call_time = p_disconnected_cnf->start_call_time;
        end_call_time = p_disconnected_cnf->end_call_time;
        call_direction = p_disconnected_cnf->call_direction;
        if(PNULL != p_disconnected_cnf->tele_num || 0 != p_disconnected_cnf->tele_num_len)
        {
            SCI_MEMCPY(tele_num, p_disconnected_cnf->tele_num, MIN(sizeof(tele_num), sizeof(p_disconnected_cnf->tele_num)));
            tele_num_len = p_disconnected_cnf->tele_num_len;
        }
        ZDT_SOS_Call_Fail();
    }
    else if(MSG_UAL_TELE_CALL_DISCONNECTED_IND == msg_id)
    {
        p_disconnected_ind = (ual_tele_call_disconnected_ind_t*)p_disconnected_param;
        sim_id = p_disconnected_ind->sim_id;
        call_id = p_disconnected_ind->call_id;
        start_call_time = p_disconnected_ind->start_call_time;
        end_call_time = p_disconnected_ind->end_call_time;
        call_direction = p_disconnected_ind->call_direction;
        if(PNULL != p_disconnected_ind->tele_num || 0 != p_disconnected_ind->tele_num_len)
        {
            SCI_MEMCPY(tele_num, p_disconnected_ind->tele_num, MIN(sizeof(tele_num), sizeof(p_disconnected_ind->tele_num)));
            tele_num_len = p_disconnected_ind->tele_num_len;
        }
    }
    else
    {
        TRACE_CCAPP("msg_id is error");
        return;
    }
    TRACE_CCAPP("sim_id = %d, call id = %d, tele_num_len = %d, start_call_time = %d, end_call_time = %d, call_direction = %d",
                sim_id, call_id, tele_num_len, start_call_time, end_call_time, call_direction);

    res = ual_tele_call_stop_local_netring();
#ifdef MMI_STK_SUPPORT
    if(call_id >= CC_MAX_CALL_NUM)
    {
        if ((CC_INVALID_ID == call_id)
            && (MN_CAUSE_SIM_STK_NOT_ALLOWED == cause)
            && MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
            )//call control fail
        {
            MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

            MMK_CloseWin(MMICC_ANIMATION_WIN_ID);
#if !defined(MMI_GUI_STYLE_TYPICAL)
            MMK_CloseWin(MMICC_STATUS_WIN_ID);
#endif
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_CALL_CONTROL_NOT_ALLOW,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);

            MMIAPISTK_SetupCall(dual_sys, FALSE, FALSE, cause);
        }

        return;
    }
#endif
    //get the call index according to its call id
    call_index = CC_GetIndexByCallId(sim_id, call_id);
    //update call info
    for(i = call_index; i < CC_MAX_CALL_NUM - 1; i++)//cid611209
    {
        s_call_info[i].call_id = s_call_info[i + 1].call_id;
        TRACE_CCAPP("s_call_info[%d].call_id = %d", i, s_call_info[i].call_id);
    }

    //if()//不存在incoming call 或者是存在incoming call 且挂断的不是incoming call
    {
        MMIAPICC_StopRingOrVibrate();
    }

    MMIDEFAULT_TurnOnBackLight();

    CC_DisconnectedCallLog(msg_id, sim_id, tele_num, tele_num_len, start_call_time, end_call_time, call_direction);

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetCallHeldStatus();
#endif
#ifdef MMI_STK_SUPPORT
    if (MSG_UAL_TELE_CALL_DISCONNECTED_IND == msg_id)
    {
        MMIAPISTK_SetupCall(dual_sys, TRUE, FALSE, cause);
    }
    else
    {
        MMIAPISTK_SetupCall(dual_sys, FALSE, TRUE, cause);
    }
#endif
    // TODO:使用DPHONE_SUPPORT控制，调用MMIDEFAULT_GetHandFreeStates和MMIAPICC_IsInState判断
}

/*****************************************************************************/
// Description : 是否要正常对待当前的这通来电...
// Global resource dependence :
// Author:jun.hu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAcceptCurCallAsNormal(void)
{
    BOOLEAN  ret_result = TRUE;
#ifdef ASP_SUPPORT
    // 看看用户有没有接听电话....
    ret_result = s_accept_call;

    s_accept_call = FALSE;
#endif

    return ret_result;
}

/*****************************************************************************/
// Description : process when received signal MSG_UAL_TELE_CALL_MT_CONNECTED_CNF
// Global resource dependence :
// Author:figo.feng
// Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_MtConnectedCnf(ual_tele_call_mt_connected_cnf_t *p_mt_connected_ind)
{
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS      address = {0};
#endif

    if(PNULL== p_mt_connected_ind)
    {
        TRACE_CCAPP("p_mt_connected_ind == PNULL");
        return;
    }
    //check the param
    TRACE_CCAPP("call_id = %d", p_mt_connected_ind->call_id);

    if(p_mt_connected_ind->call_id >= CC_MAX_CALL_NUM )
    {
        TRACE_CCAPP("p_mt_connected_ind->call_id >= 7");
        return;
    }

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetCallHeldStatus();
#endif

    // 处理当前的来电为正常来电，而非做答录处理时，才建立起通话的画面
    if(IsAcceptCurCallAsNormal())
    {
        TRACE_CCAPP("current is normal call");
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_CallConnect(&address);
#endif
        MMICC_UpdateCallStatusDisplay(MMICC_MT_CONNECTED_IND);
    }
    // TODO:使用ASP_SUPPORT控制

#ifdef DPHONE_SUPPORT
    ResetCallHandfreeState();  //Modified by yanqi.2010-Apr-13 for 未接通前回铃音等状态不对
#endif
}

/*****************************************************************************/
// Description : handle MTFW Disconnected msg
// Global resource dependence :
// Author:jibin
// Note:2004.10.12
/*****************************************************************************/
LOCAL BOOLEAN HandleMTFWDisconnected(uint32 msg_id, void *p_disconnected_param )
{
    uint8 call_id = CC_INVALID_ID;
    uint8 call_index = CC_INVALID_INDEX;
    ual_tele_sim_id_e sim_id = UAL_SIM_ID_MAX;
    ual_tele_call_disconnect_cause_e cause = UAL_TELE_CALL_DISC_CAUSE_ERROR_NONE;
    ual_tele_call_disconnected_cnf_t *p_disconnected_cnf = PNULL;
    ual_tele_call_disconnected_ind_t *p_disconnected_ind = PNULL;

    TRACE_CCAPP("enter, msg_id = %d", msg_id);
    //check the param
    if(PNULL == p_disconnected_param)
    {
        TRACE_CCAPP("p_disconnected_param == PNULL");
        return FALSE; //cid604694
    }
    if(MSG_UAL_TELE_CALL_DISCONNECTED_CNF == msg_id)
    {
        p_disconnected_cnf = (ual_tele_call_disconnected_cnf_t*)p_disconnected_param;
        sim_id = p_disconnected_cnf->sim_id;
        call_id = p_disconnected_cnf->call_id;
        cause = p_disconnected_cnf->cause;
    }
    else if(MSG_UAL_TELE_CALL_DISCONNECTED_IND == msg_id)
    {
        p_disconnected_ind = (ual_tele_call_disconnected_ind_t*)p_disconnected_param;
        sim_id = p_disconnected_ind->sim_id;
        call_id = p_disconnected_ind->call_id;
        cause = p_disconnected_ind->cause;
    }
    else
    {
        TRACE_CCAPP("msg_id is error");
        return FALSE;//cid604694
    }
    TRACE_CCAPP("call_id = %d, cause = %d", call_id, cause);
    call_index = CC_GetIndexByCallId(sim_id, call_id);

    // TODO:判断是否是MTFW
    return FALSE;
}

/*****************************************************************************/
// Description : process when received signal MSG_UAL_TELE_CALL_MT_START_IND
// Global resource dependence :
// Author:miao.liu2
// Note:
/*****************************************************************************/
LOCAL void CC_MtStartInd(ual_tele_call_mt_start_ind_t *p_mt_start_ind)
{
    uint8               index = 0;

    if(PNULL == p_mt_start_ind)
    {
        TRACE_CCAPP("p_mt_start_ind == PNULL");
        return;
    }

    TRACE_CCAPP("call_id = %d", p_mt_start_ind->call_id);

    if( p_mt_start_ind->call_id >= CC_MAX_CALL_NUM )
    {
        return;
    }

    for(index = 0; index < CC_MAX_CALL_NUM; index++)
    {
        if(CC_INVALID_ID == s_call_info[index].call_id)
        {
            s_call_info[index].call_id = p_mt_start_ind->call_id;
            TRACE_CCAPP("s_call_info[%d].call_id = %d", index, s_call_info[index].call_id);
            break;
        }
    }

    MMIAPIFMM_ExitFileOption();

    MMIAPIDC_Interrupt();

#ifdef MMS_SUPPORT
    MMIAPIMMS_Interupt();
#endif

    MMIAPIPICVIEW_StopDeleteFiles();
    //not necessary here
#ifdef VIDEO_PLAYER_SUPPORT
    WatchGallery_Vp_PauseForCC();//1341508,1346393
#endif

    // TODO:MOBILE_VIDEO_SUPPORT宏控制
    // TODO:VT_SUPPORT宏控制

    MMICC_UpdateCallStatusDisplay(MMICC_MT_START_IND);
#ifdef MMI_STK_SUPPORT
    //check close stk display text win
    MMIAPISTK_CheckCloseDisplayTextIRWin();
#endif
}

/*****************************************************************************/
// Description : process to the signal of the hold confirm
// Global resource dependence :
// Author:louis.wei
// Note:
/*****************************************************************************/
LOCAL void CC_HoldCnf(ual_tele_call_hold_cnf_t *p_hold_cnf)
{
    uint8             call_index = CC_INVALID_INDEX;

    if(PNULL == p_hold_cnf)
    {
        TRACE_CCAPP(" p_hold_cnf == NULL");
        return;
    }
    TRACE_CCAPP("call_id = %d, req_is_accepted = %d", p_hold_cnf->call_id, p_hold_cnf->is_accepted);

    call_index = CC_GetIndexByCallId(p_hold_cnf->sim_id, p_hold_cnf->call_id );
    if(TRUE == p_hold_cnf->is_accepted)
    {
        if( 0 != MMIAPICC_GetHoldCallCount())	//既有active又有hold
        {
            //do nothing
        }
        else    //只有active
        {
            if(call_index >= CC_MAX_CALL_NUM)
            {
                TRACE_CCAPP("call index: %d", call_index);
                return ;
            }

            //the operation of hold active call is successful
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
            MMIAPIBT_SetCallHeldStatus();
#endif

            //MMICC_EnableMute(MMICC_IsCurrentCallMute());  TODO
            //there just have/has hold call
            MMICC_UpdateCallStatusDisplay(MMICC_HOLD_SUC_IND);
#ifdef MMI_STK_SUPPORT
            MMIAPISTK_SetupCall(p_hold_cnf->sim_id, TRUE, FALSE, 0);
#endif
        }
    }
    else
    {
        SCI_MEMSET(&s_want_mo_call_info, 0x00, sizeof(WANT_MO_CALL_INFO_T));
        if(call_index >= CC_MAX_CALL_NUM)
        {
            TRACE_CCAPP("call index: %d", call_index);
            return ;
        }

        MMICC_UpdateCallStatusDisplay(MMICC_HOLD_FAIL_IND);
#ifdef MMI_STK_SUPPORT
        MMIAPISTK_SetupCall(p_hold_cnf->sim_id, FALSE, FALSE, MN_CAUSE_FACILITY_REJECTED);
#endif
    }
}

/*****************************************************************************/
// Description : process to signal of retrieve confirm
// Global resource dependence :
// Author:louis.wei
// Note:
/*****************************************************************************/
LOCAL void CC_UnholdCnf(ual_tele_call_unhold_cnf_t *p_unhold_cnf)
{
    uint8                  call_index   = CC_INVALID_INDEX;

    if(PNULL == p_unhold_cnf)
    {
        TRACE_CCAPP("param == pNULL");
        return;
    }

    //get the index of call id
    call_index = CC_GetIndexByCallId(p_unhold_cnf->sim_id, p_unhold_cnf->call_id );
    if(TRUE == p_unhold_cnf->is_accepted)
    {
        //MMICC_EnableMute(MMICC_IsCurrentCallMute());  todo

        MMICC_UpdateCallStatusDisplay(MMICC_RETRIEVE_SUC_IND);

        //the operation of hold active call is successful
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
        MMIAPIBT_SetCallHeldStatus();
#endif
    }
    else
    {
        if(call_index >= CC_MAX_CALL_NUM)
        {
            TRACE_CCAPP("call index: %d", call_index);
            return ;
        }

        MMICC_UpdateCallStatusDisplay(MMICC_RETRIEVE_FAIL_IND);
    }
}

/*---------------------------------------------------------------------------*
**                             PUBLIC FUNCTION                               *
**---------------------------------------------------------------------------*/

#ifdef MMI_VOLTE_SUPPORT
/*****************************************************************************/
//  Description : check is current call path is 4G
//  Global resource dependence :
//   Author:yonghua.zhang
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsVolteNetworkStatus(void)
{
    ual_tele_call_data_info_t call_info = {0};
    if (UAL_TELE_CALL_RES_SUCCESS == ual_tele_call_get_current_call_info(&call_info))
    {
        TRACE_CCAPP("sim_id = %d,ip_ps_call = %D",call_info.sim_id,call_info.is_ps_call);
        return call_info.is_ps_call;
    }
    TRACE_CCAPP("ip_ps_call = FALSE");
    return FALSE;
}
#endif

/*****************************************************************************/
//  Description : init the cc global
//  Global resource dependence :
//  Author:figo.feng
//  Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMICC_Init_Global(void)
{
    uint8 i = 0;
    uint32 cc_handle = 0;

    TRACE_CCAPP("init s_call_info");
    //initial global variable: s_call_info
    SCI_MEMSET(&s_call_info, 0, sizeof(s_call_info));

    //initial the special member of s_call_info
    for(i = 0; i < CC_MAX_CALL_NUM; i++)
    {
        s_call_info[i].call_id = CC_INVALID_ID;
    }
    ual_tele_call_register(CC_HandleMsgCallback, &cc_handle);
}

/*****************************************************************************/
//  Description : process phone number
//  Global resource dependence :
//  Author:bin.ji
//  Note:2004.9.25
/*****************************************************************************/
LOCAL CC_RESULT_E ProcessPhoneNumExt(
                                     MN_DUAL_SYS_E dual_sys,
                                     uint8 *tele_num,
                                     uint8 tele_len
                                     )
{
    //check the param
    if(PNULL== tele_num)
    {
        TRACE_CCAPP("tele_num == PNULL");
        return CC_RESULT_ERROR;
    }
    SCI_MEMSET((&s_want_mo_call_info), 0, sizeof(s_want_mo_call_info));
    TRACE_CCAPP("dual_sys = %d, tele_num = %s, tele_len =%d",dual_sys, tele_num, tele_len);

    //ready to start call
    if(CC_RESULT_SUCCESS != MakeCallReqExt(dual_sys, tele_num, tele_len))
    {
        TRACE_CCAPP("MMIAPICC_MakeCallReq error");
        SCI_MEMSET(((char*)&s_want_mo_call_info), 0, sizeof(s_want_mo_call_info));
        return CC_RESULT_ERROR;
    }
    else
    {
        TRACE_CCAPP("call succ");
        //call succ, so exit TV-out
        MMIAPITVOUT_Close();
    }

#if !defined(MMI_GUI_STYLE_TYPICAL) &&defined(CALL_TIMES_SUPPORT)
    /*从idle进call logs必须在打电话时删除显示*/
    if(MMK_IsOpenWin(MMIIDLE_DIAL_WIN_ID))
    {
        TRACE_CCAPP("CloseWin MMICL_CALL_TIMES_WIN_ID");
        MMK_CloseWin(MMICL_CALL_TIMES_WIN_ID);
    }
#endif
#if defined(MMI_PDA_SUPPORT)
    MMIAPIIDLE_ResetDialNum();
#else
    MMIAPIIDLE_CloseDialWin();
#endif
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : make a call from pb or not from pb
//  Global resource dependence : s_call_context
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL CC_RESULT_E MakeCallReqExt(
                                 MN_DUAL_SYS_E dual_sys,
                                 uint8 *src_tele_num,
                                 uint8 src_tele_len
                                 )
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_MAX;
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
    uint8 tele_len = 0;
#endif
    //check the param
    if(PNULL== src_tele_num)
    {
        TRACE_CCAPP("tele_num == PNULL");
        return CC_RESULT_ERROR;
    }

    //copy tele_num and tele_len
    MMI_MEMCPY(
        s_want_mo_call_info.tele_num,
        (CC_MAX_TELE_NUM_LEN + 1),
        src_tele_num,
        (CC_MAX_TELE_NUM_LEN + 1),
        src_tele_len
        );
    s_want_mo_call_info.tele_num_len = src_tele_len;
    s_want_mo_call_info.sim_id = dual_sys;

#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
    MMIMEX_AQIPPhNum(party_num, &s_call_context,copy_len,tele_num, &tele_len);
#endif
    ret = ual_tele_call_make_call(dual_sys,s_want_mo_call_info.tele_num);
    if(UAL_TELE_CALL_RES_SUCCESS != ret)
    {
        switch(ret)
        {
            case UAL_TELE_CALL_RES_ERROR:
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERROR");
                s_cc_mo_error_type = CC_MO_DIAL_FAILED;//拨号失败
                break;

            case UAL_TELE_CALL_RES_ERR_NUM_NULL://号码为空
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_NUM_NULL");
                s_cc_mo_error_type = CC_MO_EMPTY_NUM;//号码为空
                break;

            case UAL_TELE_CALL_RES_ERR_NUM_INVALID://号码格式错误
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_NUM_INVALID");
                s_cc_mo_error_type = CC_MO_INVALID_NUM;//无效号码
                break;

            case UAL_TELE_CALL_RES_ERR_SIM_ID://SIM ID error
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_SIM_ID");
                s_cc_mo_error_type = CC_MO_DIAL_FAILED;//拨号失败
                break;

            case UAL_TELE_CALL_RES_ERR_CRSS://CRSS解析错误
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_CRSS");
                s_cc_mo_error_type = CC_MO_CMD_NO_SUPPORT;//指令不支持
                break;

            case UAL_TELE_CALL_RES_ERR_BUSY://MO/MT/呼叫中
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_BUSY");
                s_cc_mo_error_type = CC_MO_DIAL_FAILED;//拨号失败
                break;

            case UAL_TELE_CALL_RES_ERR_FLY_MODE://飞行模式开启
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_FLY_MODE");
                s_cc_mo_error_type = CC_MO_IN_FLY_MODE;//先关闭飞行模式
                break;

            case UAL_TELE_CALL_RES_ERR_ECC_IN_FLY_MODE://紧急呼叫且飞行模式开启
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_ECC_IN_FLY_MODE");
                s_cc_mo_error_type = CC_MO_IN_FLY_MODE;//先关闭飞行模式
                break;

            case UAL_TELE_CALL_RES_ERR_NETWORK_LIMIT://网络状态为无服务或注册中
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_NETWORK_LIMIT");
                s_cc_mo_error_type = CC_MO_NET_UNAVAIL;//网络不可用
                break;

            case UAL_TELE_CALL_RES_ERR_ECC_ONLY://仅限紧急呼叫
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_ECC_ONLY");
                s_cc_mo_error_type = CC_MO_NET_UNAVAIL;//网络不可用
                break;

            case UAL_TELE_CALL_RES_ERR_SIM_NOT_READY://SIM状态异常
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_SIM_NOT_READY");
                s_cc_mo_error_type = CC_MO_NO_SIM;//无SIM卡
                break;

            case UAL_TELE_CALL_RES_ERR_CALL_FULL://呼叫数目达到上限
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_CALL_FULL");
                s_cc_mo_error_type = CC_MO_DIAL_FAILED;//拨号失败
                break;

            case UAL_TELE_CALL_RES_ERR_IMS_NOT_READY://IMS没有成功注册
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_IMS_NOT_READY");
                s_cc_mo_error_type = CC_MO_NO_VOLTE_CALL;//不是volte call
                break;

            case UAL_TELE_CALL_RES_ERR_ECC_NOT_SUPPORT://不支持紧急呼叫
                TRACE_CCAPP("error is UAL_TELE_CALL_RES_ERR_ECC_NOT_SUPPORT");
                s_cc_mo_error_type = CC_MO_ECC_CALL;//不支持紧急呼叫
                break;

            default:
                TRACE_CCAPP("error is default val");
                s_cc_mo_error_type = CC_MO_ERROR_TYPE_MAX;
                break;
        }
        return CC_RESULT_ERROR;
    }
    s_cc_mo_error_type = CC_MO_ERROR_NONE;
    TRACE_CCAPP("make call success");
    //MMIAUDIO_HandleNewMsgKey();

#ifdef FM_SUPPORT
    MMIAPIFM_StopRecord();
    MMIAPIFM_StopAutoSearch();
#endif
    //suspend Mp3 or FM background play
    //MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_CC);

    MMIAPIFMM_ExitFileOption();

    MMIAPIDC_Interrupt();
// MMIAPICC_StopCCRecord();
#ifdef MMI_RECORD_SUPPORT
    MMIAPIRECORD_StopRecordOrPlay(); //打出电话的时候停止录音
#endif
    MMIAPIPICVIEW_StopDeleteFiles();
    //not necessary here
#if defined(VIDEO_PLAYER_SUPPORT) && !defined(MMI_VIDEOPLAYER_MINI_FUNCTION)
    MMIAPIVP_NotifyVPStop();
#endif

#ifdef MOBILE_VIDEO_SUPPORT
    if(MMIAPIMV_IsLiving())
    {
        MMIAPIMV_Exit();
    }
#endif
    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_CC, FALSE);

    //@liqing.peng, cr72005
//    MMICC_EnableMute(FALSE);//cr228324
    //MMICC_EnableRemoteMute(FALSE); //bug 184484

//============== MET MEX start ===================
#ifdef MET_MEX_SUPPORT
    if(MMIMEX_IsOutCalling())
    {
        //mex application registered to hook this message
        return CC_RESULT_SUCCESS;
    }
#endif
//============== MET MEX end ===================
    //修改CALL LOG resolve in the future
    MMICC_UpdateCallStatusDisplay(MMICC_MO_START_IND);
    MMICC_EnableRemoteMute(FALSE); //bug 184484
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : If ip number is included in telephone number, remove it
//  Global resource dependence : s_call_context
//  Author:lin.lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_RemoveIpNumber(
                                              uint8*  tele_num_ptr,
                                              uint8   tele_len,
                                              uint8*  origin_number_ptr,
                                              uint8   origin_number_len
                                              )
{
#ifdef MMI_IP_CALL_SUPPORT
    uint8 ip_number[MMISET_IPNUMBER_MAX_LEN+1] = {0};
    uint8 ip_number_len = 0;

    if (PNULL == tele_num_ptr || 0 == tele_len)
    {
        return FALSE;
    }
    TRACE_CCAPP("tele_num_ptr = %s, tele_len = %d",tele_num_ptr, tele_len);

    MMIAPISET_GetIPNumber(s_call_context.dual_sys, tele_num_ptr, ip_number, &ip_number_len);

    tele_num_ptr[tele_len] = 0;
    if ( (0 != ip_number_len ) && (tele_len > ip_number_len) && (0 == strncmp((char*)tele_num_ptr, (char*)ip_number, ip_number_len)))
    {
        if(origin_number_len >= tele_len - ip_number_len + 1)
        {
            strcpy((char*)origin_number_ptr, (char*)tele_num_ptr + ip_number_len);
            return TRUE;
        }
        else
        {
            TRACE_CCAPP("origin_number_len = %d, tele_len = %d, ip_number_len = %d",origin_number_len, tele_len, ip_number_len);
            return FALSE;
        }
    }
    return FALSE;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : get current exited call count
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetExitedCallCount(void)
{
    uint8 call_count = 0;
    uint8 active_call_count  = 0;
    uint8 hold_call_count  = 0;
#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        call_count = 1;
    }
    else
#endif
    {
        active_call_count = MMIAPICC_GetActiveCallCount();
        hold_call_count = MMIAPICC_GetHoldCallCount();
        call_count = active_call_count + hold_call_count;
    }
    TRACE_CCAPP("call_count = %d",call_count);
    return (call_count);
}

/*****************************************************************************/
//  Discription: the blue tooth headset  accept the incoming call
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPPICC_BtAcceptCall(void)
{
#ifdef BLUETOOTH_SUPPORT
    CC_CALL_STATE_E call_status = CC_INIT_STATE;

    call_status = MMICC_GetCurrentCallStatus();

    //只有在Incoming Call状态才处理这个消息，别的通话状态均不处理
    if (CC_INCOMING_CALL_STATE == call_status)
    {
        MMK_SendMsg(WATCHCC_MTCALLING_WIN_ID,MSG_BT_ACCEPT_INCOMING_CALL,PNULL);
    }
    else if (CC_WAITING_STATE == call_status)
    {
        CC_HoldAndAnswerCW();
        TRACE_CCAPP("hold and answer call!");
    }
#ifdef VT_SUPPORT
    else if (MMIAPIVT_IsVtMtIncoming())
    {
        MMIAPIVT_ConnectCall();
    }
#endif
    TRACE_CCAPP("the call status is %d", call_status);
#endif
}

/*****************************************************************************/
//  Discription: the blue tooth headset cancel the call including(incoming and outgoing call)
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPICC_BtCancelCall(void)
{
#ifdef BLUETOOTH_SUPPORT
    CC_CALL_STATE_E call_status = CC_INIT_STATE;

    call_status = MMICC_GetCurrentCallStatus();
    TRACE_CCAPP("the call status is %d", call_status);

    //Incoming call时，拒接来电
    if (CC_INCOMING_CALL_STATE == call_status)
    {
        MMK_SendMsg(WATCHCC_MTCALLING_WIN_ID,MSG_BT_REJECT_INCOMING_CALL,PNULL);
    }
    else if( CC_CALLING_STATE == call_status)
    {
        MMK_SendMsg(WATCHCC_MOCALLING_WIN_ID,MSG_BT_CANCEL_OUTGOING_CALL,PNULL);
#if !defined(MMI_GUI_STYLE_TYPICAL)
        MMK_SendMsg(WATCHCC_CONNECTED_WIN_ID,MSG_BT_CANCEL_OUTGOING_CALL,PNULL);
#endif
    }
    else if ( CC_CALL_STATE == call_status || CC_HOLD_STATE == call_status || CC_WAITING_STATE == call_status)//正在通话中，将电话挂断
    {
        if (MMK_IsOpenWin(WATCHCC_CONNECTED_WIN_ID))
        {
            MMK_SendMsg(WATCHCC_CONNECTED_WIN_ID,MSG_BT_CANCEL_CALL,PNULL);
        }
        else if (CC_IsCallIDValid(s_call_context.current_call))
        {
            MMIAPICC_ReleaseCallByRedkey();
        }
    }
#ifdef VT_SUPPORT
    else if (MMIAPIVT_IsVtMtIncoming() || MMIAPIVT_IsVtConnecting())
    {
        MMIAPIVT_ReleaseVTCallByRedkey();
    }
#endif
#endif
}

/*****************************************************************************/
//  Description : process when user want to receive a MT call
//  Global resource dependence :
//  Author:figo.feng
//  Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ConnectCallReq(void)
{
    TRACE_CCAPP("enter function");

    //call function of MN to accept call
#ifndef MMI_MULTI_SIM_SYS_NONE
    if(UAL_TELE_CALL_RES_SUCCESS != ual_tele_call_answer_call())
    {
        TRACE_CCAPP("call ual_tele_call_answer_call failure");
        return CC_RESULT_ERROR;
    }
#endif

#ifndef UAL_TELE_SUPPORT
    s_call_context.exist_call.exist_incoming_call = FALSE;
    //stop ring
    MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);
    MMIAPISET_StopRing(MMISET_RING_TYPE_CALL_IN_CALL);
#endif

#ifdef MMI_ETWS_SUPPORT
    if( MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
    {
        MMIAPISMSCB_SetIsNeedRedisplay(TRUE);
    }

    if( TRUE == MMIAPISMSCB_IsETWSAlert() )
    {
        MMIAPISET_StopSOSVibrate();
    }
#endif
#ifndef UAL_TELE_SUPPORT
    //修改CALL LOG
    //更新显示
    s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_CONNECT_OPER;

    //cr133748
    s_mt_need_open_codec_flag = s_mt_need_open_codec_flag|CC_MT_CALL_CONNECTING;
    CC_OpenAudioTunnel(TRUE);
#endif
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : process when user hang up the call
//  Global resource dependence :
//  Author:figo.feng
//  Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseCallReq(uint8 call_index)
{

    //check the param
    TRACE_CCAPP("call_index = %d",call_index);

    //call function of MN to disconnect call
    if(UAL_TELE_CALL_RES_SUCCESS != ual_tele_call_release_call(s_call_info[call_index].call_id))
    {
        TRACE_CCAPP("ual_tele_call_release_call return failure");
        return CC_RESULT_ERROR;
    }

    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : process when user hang up the call
//  Global resource dependence :
//  Author:figo.feng
//  Note:2003.12.01
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseCallReq(MN_DUAL_SYS_E dual_sys, uint8 call_index,MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    return CC_ReleaseCallReq(call_index);
}

/*****************************************************************************/
//  Description : clear the stored DTMF after sending them
//  Global resource dependence :
//  Author:figo.feng
//  Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMICC_ClearStoredDtmf(void)
{
#ifndef UAL_TELE_SUPPORT
    CC_ClearStoredDtmf();
#endif
}

/*****************************************************************************/
//  Description : clear the stored DTMF after sending them
//  Global resource dependence :
//  Author:figo.feng
//  Note:2003.12.01
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SendSingleDtmf(uint32 msg_id)
{
    uint8 dtmf_ch = 0;
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_ERROR;
    ual_tele_call_data_info_t call_info = {0};

    //CC界面，全键盘工程，默认需对共用的数字按键进行消息转换。
    if ((MSG_KEYDOWN_Q <= msg_id && msg_id <= MSG_KEYDOWN_EXCLAMATION)
        || (MSG_KEYUP_Q <= msg_id && msg_id <= MSG_KEYUP_EXCLAMATION))
    {
        msg_id = MMIAPIIDLE_GetNumMsgId(msg_id);
    }

    if ((MSG_KEYUP_1 <= msg_id && msg_id <= MSG_KEYUP_9)
        || MSG_KEYUP_0 == msg_id
        || MSG_KEYUP_STAR == msg_id
        || MSG_KEYUP_HASH == msg_id
        )
    {
        TRACE_CCAPP("msg_id error");
        return CC_RESULT_ERROR;
    }
    //get the ascii from msg id
    dtmf_ch = MMIAPICC_MsgidToAscii(msg_id);
    //if the pressed key is valid

    ret = ual_tele_call_get_current_call_info(&call_info);
    if(UAL_TELE_CALL_RES_ERROR == ret)
    {
        TRACE_CCAPP("get current call info fail");
        return CC_RESULT_ERROR;
    }

    ret = ual_tele_call_start_single_dtmf(call_info.sim_id,dtmf_ch,TRUE);
    if(UAL_TELE_CALL_RES_ERROR == ret)
    {
        TRACE_CCAPP("start single dtmf fail");
        return CC_RESULT_ERROR;
    }
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : get the ascii from msg id
//  Global resource dependence :
//  Author:figo.feng
//  Note:2003.12.01
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_MsgidToAscii(
                                   MMI_MESSAGE_ID_E msg_id
                                   )
{
    uint8 result = CC_INVALID_DTMF_VALUE;

    switch(msg_id)
    {
        //press key 1-9
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
        result = msg_id - (MSG_KEYDOWN_1) + '1';
        break;

        //release key 1-9
//    case MSG_KEYUP_1:
//    case MSG_KEYUP_2:
//    case MSG_KEYUP_3:
//    case MSG_KEYUP_4:
//    case MSG_KEYUP_5:
//    case MSG_KEYUP_6:
//    case MSG_KEYUP_7:
//    case MSG_KEYUP_8:
//    case MSG_KEYUP_9:
//        result = msg_id - (MSG_KEYUP_1) + '1';
//        break;

        //press key *
    case MSG_KEYDOWN_STAR:
//    case MSG_KEYUP_STAR:
        result = '*';
        break;

        //press key 0
    case MSG_KEYDOWN_0:
//    case MSG_KEYUP_0:
        result = '0';
        break;

        //press key #
    case MSG_KEYDOWN_HASH:
//    case MSG_KEYUP_HASH:
        result = '#';
        break;

    default:
        result = CC_INVALID_DTMF_VALUE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : if GSM network support
//  Global resource dependence :
//  Author:figo.feng
//  Note:2003.12.01
/*****************************************************************************/
LOCAL BOOLEAN CC_IsEmergencyOnly(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_sim_exist = FALSE;

    if(UAL_TELE_SIM_STATUS_READY == ual_tele_sim_get_sim_status(dual_sys))
    {
        is_sim_exist = TRUE;
    }
    TRACE_CCAPP("is_sim_exist = %d",is_sim_exist);
    return (!is_sim_exist || MMIAPIDM_GetHandsetLock() || UAL_TELE_SIM_STATUS_NOT_INITED == ual_tele_sim_get_sim_status(dual_sys));
}

/*****************************************************************************/
//  Description :
// 1) 112和911任何情况下都是紧急呼叫
// 2) sim/usim卡上的ecc文件中的紧急呼叫号码呼出都是紧急呼叫
// 3) 无卡时，000, 08, 110, 999, 118 and 119 呼出都是紧急呼叫
//  Global resource dependence :
//  Author:samboo.shen
//  Note:is_only_for_display means in normal status the phone make NORMAL call by 000, 08, 110, 999, 118 and 119 num
// but display ENMERGENCY CALL in window
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsEmergencyNum(
                                 uint8 *tele_num,
                                 uint8 tele_len,
                                 MN_DUAL_SYS_E dual_sys,
                                 BOOLEAN is_only_for_display
                                 )
{
    uint8 temp_tele_num[CC_MAX_TELE_NUM_LEN + 1] = {0};
    uint8 temp_tele_len = 0;
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    BOOLEAN is_local_exisit = FALSE;
    BOOLEAN is_emergency_call_in_local = FALSE;
#endif

    //check the param
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        TRACE_CCAPP("error dual_sys = %d",dual_sys);
        return FALSE;
    }

    if(PNULL == tele_num)
    {
        TRACE_CCAPP("tele_num == PNULL");
        return FALSE;
    }

    if(0 == tele_len)
    {
        TRACE_CCAPP("tele_len == 0");
        return FALSE;
    }

    if ('+' == tele_num[0])
    {
        temp_tele_len = MIN(tele_len - 1, CC_MAX_TELE_NUM_LEN);

        if(0 == temp_tele_len)
        {
            TRACE_CCAPP("temp_tele_len == 0");
            return FALSE;
        }

        SCI_MEMCPY(temp_tele_num, tele_num + 1, temp_tele_len);
    }
    else
    {
        temp_tele_len = MIN(tele_len, CC_MAX_TELE_NUM_LEN);
        TRACE_CCAPP("temp_tele_len == %d",temp_tele_len);
        SCI_MEMCPY(temp_tele_num, tele_num, temp_tele_len);
    }

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
   is_emergency_call_in_local = CC_IsEmergencyInLocalEmergencyList(tele_num, tele_len, &is_local_exisit, dual_sys);

    if (is_emergency_call_in_local)
    {
        return TRUE;
    }
#endif

    if(ual_tele_call_is_emergency_num(dual_sys,temp_tele_num,temp_tele_len))
    {
        return TRUE;
    }

    return FALSE;

}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsEmergencyCall(void)
{
    return ual_tele_call_is_emergency_num(s_want_mo_call_info.sim_id,s_want_mo_call_info.tele_num,s_want_mo_call_info.tele_num_len);
}

/*****************************************************************************/
//  Description : Release all active call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseActiveCall(void)
{
    uint16  i = 0;
    CC_RESULT_E  result = CC_RESULT_SUCCESS;
    uint8 call_count = 0;
    ual_tele_call_state_e call_state = UAL_TELE_CALL_STATE_MAX;

#ifndef MMI_MULTI_SIM_SYS_NONE
    call_count = ual_tele_call_get_call_count();
    for( i = 0; i < call_count; i++ )
    {
        call_state = ual_tele_call_get_call_state(s_call_info[i].call_id);
        if( UAL_TELE_CALL_STATE_ACTIVE == call_state )
        {
            //disconnect the call which state is active
            if(UAL_TELE_CALL_RES_SUCCESS != ual_tele_call_release_call(s_call_info[i].call_id))
            {
                TRACE_CCAPP("ual_tele_call_release_call  fail");
                result = CC_RESULT_ERROR;
            }

#ifndef UAL_TELE_SUPPORT
            //whether the call is in mpty
            if( ! s_call_context.call_info[i].is_in_mpty )
                break;
#endif
        }
    }
#else
    index = FindStateCall(CC_CALL_STATE);
    if(index < s_call_context.call_number)
    {
        s_bt_current_call_index = index;
        s_call_context.call_info[ s_bt_current_call_index].operated_by_user = CC_RELEASE_OPER;
        bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_RELEASE_ACTIVE_CALLS);
        if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
        {
            TRACE_CCAPP("ccapp.c: CC_HoldAndAnswerCW  MMIAPIBT_HfuCallHold  fail");
        }
    }
#endif

    return result;
}

/*****************************************************************************/
//  Description : Release all hold call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseHoldCall(void)
{
    uint16  i = 0;
    CC_RESULT_E  result = CC_RESULT_SUCCESS;
    uint8 call_count = 0;
    ual_tele_call_state_e call_state = UAL_TELE_CALL_STATE_MAX;

    TRACE_CCAPP("enter function");

    call_count = ual_tele_call_get_call_count();
    for( i = 0; i < call_count; i++ )
    {
        call_state = ual_tele_call_get_call_state(s_call_info[i].call_id);
        if( UAL_TELE_CALL_STATE_HOLD == call_state )
        {
            if(UAL_TELE_CALL_RES_SUCCESS != ual_tele_call_release_call(s_call_info[i].call_id))
            {
                TRACE_CCAPP("ual_tele_call_release_call  fail");
                result = CC_RESULT_ERROR;
            }
#ifndef UAL_TELE_SUPPORT
            //whether the call is in mpty
            if( ! s_call_context.call_info[i].is_in_mpty )
                break;
#endif
        }
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : s_call_context
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseActiveCall(void)
{
    return CC_ReleaseActiveCall();
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseHoldCall(void)
{
    return CC_ReleaseHoldCall();
}

/*****************************************************************************/
//  Description : Release all call ext
//  Global resource dependence : s_call_context
//  Author:jibin
//  Note:2004.10.14
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_ReleaseAllCallExt(
                                              CC_RELEASE_ALL_TYPE_E release_all_type
                                              )
{
    TRACE_CCAPP("enter function");

    if(MN_RETURN_SUCCESS != ual_tele_call_release_all())
    {
        TRACE_CCAPP("ual_tele_call_release_all fail");
        return CC_RESULT_ERROR;
    }

    //提示正在释放
    if (CC_RELEASE_ALL_NOPROMPT != release_all_type)
    {
//        MMK_CloseWin( MMICC_ANIMATION_WIN_ID  );
    }
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : Hold the active call for preparing  for waiting call
//  Global resource dependence :
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_HoldAndAnswerCW(void)
{
    // TODO:
    //return CC_HoldAndAnswerCW();
    return CC_RESULT_ERROR;//cid608709
}

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//  Description : Handle SIM Plug Notify CC
//  Global resource dependence :
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleSIMPlugNotifyCCFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    ual_tele_call_data_info_t call_info = {0};
    TRACE_CCAPP("dual_sys=%d, notify_event=%d",dual_sys, notify_event);
    ual_tele_call_get_current_call_info(&call_info);

    switch (notify_event)
    {
        case MMIPHONE_NOTIFY_SIM_PLUG_IN:
        {
            if (MMIAPICC_IsInState(CC_IN_CALL_STATE) && (dual_sys == call_info.sim_id))
            {
                MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_PROMPT);
            }
            MMK_CloseWin(MMICL_CALL_COSTS_WIN_ID);
#ifdef MMI_LDN_SUPPORT
            MMIAPICL_SetIsCallInfoReady(dual_sys, FALSE);
#endif
        }
            break;

        case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
        {
            if (MMIAPICC_IsInState(CC_IN_CALL_STATE) && (dual_sys == call_info.sim_id))
            {
                MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_PROMPT);
            }

            if (dual_sys == MMIAPIUSSD_GetDualSys())
            {
                MMIAPIUSSD_CloseSendWin();
                MMIAPIUSSD_CloseInputWin();
                MMIAPIUSSD_CloseShowDataWin();
            }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMK_CloseWin(MMICC_COM_SELECT_SIM_WIN_ID);
            MMK_CloseWin(MMICL_COM_SELECT_SIM_WIN_ID);
            MMK_CloseWin(MAIN_SIM_SELECT_WIN_ID);
#endif
            MMK_CloseWin(MMICL_CALL_COSTS_WIN_ID);
            MMK_CloseWin(MMICL_CALL_TIMER_WIN_ID);
#ifdef MMI_GPRS_SUPPORT
            MMK_CloseWin(MMICL_GPRS_FLOWRATE_WIN_ID);
#endif

#ifdef MMI_LDN_SUPPORT
            MMIAPICL_SetIsCallInfoReady(dual_sys, TRUE);
            MMIAPICL_ResetCallInfoRecord(dual_sys);
#endif
        }
            break;

        default:
            {
                // do nothing now!!
            }
            break;
    }
}
#endif

/*****************************************************************************/
//  Description : Hold the active call for preparing  for waiting call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_HoldAndAnswerCW(void)
{
    // TODO:
    //return CC_HoldAndAnswerCW();
    return CC_RESULT_ERROR;//cid604865
}

/*****************************************************************************/
//  Description : Release the active call for preparing for waiting call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseAndAnswerCW(void)
{

    TRACE_CCAPP("enter function");

    if( UAL_TELE_CALL_RES_SUCCESS != ual_tele_call_release_and_answer())
    {
        TRACE_CCAPP("ual_tele_call_release_and_answer fail");
        return CC_RESULT_ERROR;
    }

#ifdef ASP_SUPPORT
    s_accept_call = TRUE;
#endif
    MMK_CloseWin(MMICC_MENU_WIN_ID) ;
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : Release the active call for preparing for waiting call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseAndAnswerCW(void)
{
    return CC_ReleaseAndAnswerCW();
}

/*****************************************************************************/
//  Description : process hold the call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_HoldCall(
                              void
                              )
{
#ifndef UAL_TELE_SUPPORT
    uint8   active_index = CC_INVALID_INDEX;
    uint8   hold_index = CC_INVALID_INDEX;
#endif
    TRACE_CCAPP("enter function");

    //whether the call module has not call
    if( 0 != ual_tele_call_get_call_count() )
    {
        if( ( 0 != MMIAPICC_GetActiveCallCount() ) &&
            ( 0 != MMIAPICC_GetHoldCallCount() ) )
        {
            // TODO:
#ifndef UAL_TELE_SUPPORT
            //shuttle the hold call and active call
            active_index = FindStateCall( CC_CALL_STATE );
            if (active_index >= s_call_context.call_number)
            {
                TRACE_CCAPP("invalid active idx: %d",active_index);
                return CC_RESULT_ERROR;
            }
            s_call_context.call_info[active_index].operated_by_user = CC_SHUTTLE_OPER;
            s_call_context.current_call = active_index;
            //TRACE_CCAPP("the active call is %s");
            //get the hold call
            hold_index =FindStateCall( CC_HOLD_STATE );
            if (hold_index >= s_call_context.call_number)
            {
                TRACE_CCAPP("invalid hold idx: %d",hold_index);
                return CC_RESULT_ERROR;
            }
            s_call_context.call_info[hold_index].operated_by_user = CC_SHUTTLE_OPER;
            //TRACE_CCAPP("the hold call is %s");

            //the operation is to shuttle the active call and the hold call
            if( MN_RETURN_SUCCESS != MNCALL_SwapCallsEx(
                s_call_context.dual_sys,
                s_call_context.call_info[active_index].call_id,
                s_call_context.call_info[hold_index].call_id) )
            {
                TRACE_CCAPP("CC_HoldCall: MNCALL_SwapCalls is fault");
                return CC_RESULT_ERROR;
            }
#endif
        }
        else if( 0 != MMIAPICC_GetActiveCallCount() )
        {
            //hold the active call
            if( UAL_TELE_CALL_RES_SUCCESS != ual_tele_call_hold_call())
            {
                TRACE_CCAPP("CC_HoldCall: MNCALL_HoldCall is fault");
                return CC_RESULT_ERROR;
            }
        }
        else
        {
            if( 0 != MMIAPICC_GetHoldCallCount())
            {
            //retrieve the hold call
                if( UAL_TELE_CALL_RES_SUCCESS != ual_tele_call_unhold_call())
                {
                    TRACE_CCAPP("CC_HoldCall: MNCALL_RetrieveCall is fault");
                    return CC_RESULT_ERROR;
                }
            }
        }
    }
    else
    {
        //use the msgbox to prompt the operation is fail
        MMIPUB_OpenAlertWarningWin(TXT_CC_OPER_FAIL);
#if !defined(MMI_GUI_STYLE_TYPICAL)
        MMIPUB_SetWinTitleTextId( MMIPUB_ALERT_WIN_ID, TXT_CC_HOLD_PDA, FALSE );
#endif
        return CC_RESULT_ERROR;
    }

    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : process hold the call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_HoldCall(
                              void
                              )
{
    return CC_HoldCall();
}

/*****************************************************************************/
//  Description : process to build MPTY call for accepting the incoming call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_BuildMPTYCall(
                                          void
                                          )
{
#ifndef UAL_TELE_SUPPORT
    return CC_BuildMPTYCall();
#else
    return CC_RESULT_ERROR;
#endif
}

/*****************************************************************************/
//  Description : process to build MPTY call for accepting the incoming call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_BuildMPTYAndAnswerCW(
                                          void
                                          )
{
    CC_RESULT_E     result = CC_RESULT_ERROR;
#ifndef UAL_TELE_SUPPORT
    TRACE_CCAPP("enter function");

    //build mpty call in the first
    MMK_CloseWin( MMICC_ANIMATION_WIN_ID  );
    if( CC_RESULT_SUCCESS == CC_BuildMPTYCall( ) )
    {
        s_call_context.operate_waiting_call = BUILD_MPTY_AND_ACCEPT;
        result = CC_RESULT_SUCCESS;
    }
    else
    {
        result = CC_RESULT_ERROR;
    }
#endif
    return result;
}

/*****************************************************************************/
//  Description : process to handle the confirm for build MPTY call
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void CC_HandleBuildMPTYCnf(
                                 DPARAM param
                                 )
{
#ifndef UAL_TELE_SUPPORT
    APP_MN_BUILD_MPTY_CNF_T *sig_ptr = (APP_MN_BUILD_MPTY_CNF_T*)param;
    uint8               i = 0;
    uint8               call_index = CC_INVALID_INDEX;

    if(PNULL == sig_ptr)
    {
        TRACE_CCAPP("param == NULL");
        return;
    }
    TRACE_CCAPP("enter function");

    //whether the call id is vaild
    if( ( sig_ptr->call_id >= CC_MAX_CALL_NUM ) )
    {
        TRACE_CCAPP("call_id = %d",sig_ptr->call_id);
        return;
    }

    call_index = CC_GetIndexByCallId(sig_ptr->dual_sys, sig_ptr->call_id );

    if(!CC_IsCallIDValid(call_index) )
    {
        //TRACE_CCAPP("invald call index %d");
        return;
    }

    s_call_context.current_call = call_index;
    s_call_context.call_info[call_index].operated_by_user = CC_NO_OPER;/*lint !e661 !e662*/

    TranslateOperState(  CC_BUILDMPTY_OPER );

    if( sig_ptr->req_is_accepted )
    {
        for( i = 0; i < s_call_context.call_number ; i++ )
        {
            if( CC_WAITING_STATE != s_call_context.call_info[ i ].call_state )
            {
                s_call_context.call_info[ i ].is_in_mpty = TRUE;

                //modify the call state in the s_call_context
                if( CC_HOLD_STATE == s_call_context.call_info[i].call_state)
                {
                    s_call_context.call_info[i].call_state = CC_CALL_STATE;
                    s_call_context.exist_call.exist_active_call_num++;
                    s_call_context.exist_call.exist_hold_call_num--;
                }
            }
        }

        if( ( BUILD_MPTY_AND_ACCEPT == s_call_context.operate_waiting_call ) &&
            ( s_call_context.exist_call.exist_incoming_call ) )
        {

            //hold the MPTY for accepting incoming call
            //            if( MN_RETURN_SUCCESS != MNCALL_HoldCall( sig_ptr->call_id ) )
            if( MN_RETURN_SUCCESS != MNCALL_HoldCallEx(s_call_context.dual_sys, sig_ptr->call_id ) )
            {
                TRACE_CCAPP("MNCALL_HoldCall is fault");
                return;
            }

            s_call_context.operate_waiting_call = HOLD_ALL_ACTIVE_AND_ACCEPT;
            s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_HOLD_OPER;
        }
        else
        {
            //the MPTY is in active state
            s_call_context.call_info[ s_call_context.current_call  ].operated_by_user = CC_NO_OPER;
        }
        MMICC_EnableMute(MMICC_IsCurrentCallMute());
        //update the prompt in the cc window
        MMICC_UpdateCallStatusDisplay(MMICC_BUILD_MPTY_SUC_IND);
    }
    else
    {
        s_call_context.current_call = FindCallInModule();
        MMICC_UpdateCallStatusDisplay(MMICC_BUILD_MPTY_FAIL_IND);

    }
//合并成为多方通话后需要通知蓝牙更新状态
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetCallHeldStatus();
#endif
#endif
}

/*****************************************************************************/
//  Description : check is exist outgoing call or not
//  Global resource dependence : s_call_context
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistOutgoingCall(void)
{
    uint8 all_call_count = 0;
    uint8 i = 0;

    all_call_count = ual_tele_call_get_call_count();
    for(i = 0;i < all_call_count;i++)
    {
        if(UAL_TELE_CALL_STATE_OUTGOING == ual_tele_call_get_call_state(s_call_info[i].call_id))
        {
            TRACE_CCAPP("is exist");
            return TRUE;
        }
    }
    TRACE_CCAPP("not exist");
    return FALSE;
}

/*****************************************************************************/
//  Description : check is exist incomming call or not
//  Global resource dependence : s_call_context
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistIncommingCall(void)
{
    uint8 all_call_count = 0;
    uint8 i = 0;

    all_call_count = ual_tele_call_get_call_count();
    for(i = 0;i < all_call_count;i++)
    {
        if(UAL_TELE_CALL_STATE_INCOMING == ual_tele_call_get_call_state(s_call_info[i].call_id))
        {
            TRACE_CCAPP("is exist");
            return TRUE;
        }
    }
    TRACE_CCAPP("not exist");
    return FALSE;
}

/*****************************************************************************/
//  Description : check is exist MPTY or not
//  Global resource dependence : s_call_context
//  Author:jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistActiveMPTY(void)
{
#ifndef UAL_TELE_SUPPORT
    TRACE_CCAPP("enter function ");

    if (1 < s_call_context.exist_call.exist_active_call_num)
    {
        return TRUE;
    }
    else
#endif
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : check is exist MPTY or not
//  Global resource dependence : s_call_context
//  Author:jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistHoldMPTY(void)
{
#ifndef UAL_TELE_SUPPORT
    TRACE_CCAPP("enter function ");

    if (1 < s_call_context.exist_call.exist_hold_call_num)
    {
        return TRUE;
    }
    else
#endif
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : check is exist MPTY or not
//  Global resource dependence : s_call_context
//  Author:jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistActiveCall(void)
{
    if (0 < MMIAPICC_GetActiveCallCount())
    {
#if defined(MMI_MULTI_SIM_SYS_NONE)  //no more active bt call !! ,
        if((BT_CALL_STATUS_INDICATOR == s_bt_ciev_info.ind_type) &&
            (BT_HFP_IND_CALL_NO_CALL_ACTIVE ==s_bt_ciev_info.ind_value))
            return FALSE;
#endif
        TRACE_CCAPP("exist");
        return TRUE;
    }
    else
    {
        TRACE_CCAPP("not exist");
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : check is exist MPTY or not
//  Global resource dependence : s_call_context
//  Author:jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistHoldCall(void)
{
    if (0 < MMIAPICC_GetHoldCallCount())
    {
        TRACE_CCAPP("exist");
        return TRUE;
    }
    else
    {
        TRACE_CCAPP("not exist");
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : check is exist MPTY or not
//  Global resource dependence : s_call_context
//  Author:jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistActiveAndHoldCalls(void)
{
    if (0 < MMIAPICC_GetActiveCallCount()
        && 0 < MMIAPICC_GetHoldCallCount())
    {
        TRACE_CCAPP("exist");
        return TRUE;
    }
    else
    {
        TRACE_CCAPP("not exist");
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : check is exist MPTY or not
//  Global resource dependence : s_call_context
//  Author:jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistMPTY(void)
{
#ifndef UAL_TELE_SUPPORT
    if (MMICC_IsExistActiveMPTY()
        || MMICC_IsExistHoldMPTY())
    {
        return TRUE;
    }
    else
#endif
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : process to spilt call from MPTY
//  Global resource dependence : s_call_context
//  Author:louis.wei
//  Note: must is exist mpty! must only 1 state!
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SplitSpecCall(
                                   uint8 call_index
                                   )
{
#ifndef UAL_TELE_SUPPORT
    uint8       i = 0;

    TRACE_CCAPP("enter function,call_index = %d",call_index);

    //check the call id is vaild
    if (call_index >= CC_MAX_CALL_NUM || s_call_context.call_info[call_index].call_id >= CC_MAX_CALL_NUM || !MMICC_IsExistMPTY())
    {
        //TRACE_CCAPP("");
        return CC_RESULT_ERROR;
    }
    //must only 1 state
    if (0 != s_call_context.exist_call.exist_active_call_num
        && 0 != s_call_context.exist_call.exist_hold_call_num)
    {
        //TRACE_CCAPP("");
        return CC_RESULT_ERROR;
    }

        //split the prointed call
    if( MN_RETURN_SUCCESS != MNCALL_SplitMPTYEx(s_call_context.dual_sys, s_call_context.call_info[call_index].call_id ) )
    {
        //TRACE_CCAPP("");
        return CC_RESULT_ERROR;
    }
    s_call_context.call_info[i].operated_by_user = CC_SPLIT_OPER;
    s_call_context.current_call = i;
#endif
    return CC_RESULT_SUCCESS;

}

/*****************************************************************************/
//  escription : 是否存在正在建立mpty，分离mpty，断开等call
//  lobal resource dependence : s_call_context
//  Author:bin.ji
//  note:2004.11.26
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistOperatedCall(void)
{
#ifndef UAL_TELE_SUPPORT
    return CC_IsExistOperatedCall();
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  escription : search the call which state is pointed by param
//  lobal resource dependence :
//  Author:louis.wei
//  note:
/*****************************************************************************/
PUBLIC uint8   MMICC_FindStateCall(
                            CC_CALL_STATE_E  state
                            )
{
    uint8 call_count = 0;
    uint8 i = 0;

    call_count = ual_tele_call_get_call_count();
    for(i = 0;i < call_count;i++)
    {
        if(state == MMICC_GetCallStatusByIndex(i))
        {
            break;
        }
    }
    TRACE_CCAPP("call_count = %d",call_count);
    return i;
}

/*****************************************************************************/
//  escription : decide whether send dtmf or stop dtmf
//  lobal resource dependence :
//  Author: louis.wei
//  note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_SendDtmfByChar(MN_DUAL_SYS_E dual_sys, uint8 dtmf_ch)
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_MAX;

    ret = ual_tele_call_start_single_dtmf(dual_sys,dtmf_ch,TRUE);
    if(UAL_TELE_CALL_RES_ERROR == ret)
    {
        TRACE_CCAPP(" stop dtmf fail");
        return CC_RESULT_ERROR;
    }
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  escription : stop dtmf
//  lobal resource dependence :
//  Author:
//  note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_StopDtmf(MN_DUAL_SYS_E dual_sys)
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_MAX;

    ret = ual_tele_call_stop_single_dtmf(dual_sys);
    if(UAL_TELE_CALL_RES_ERROR == ret)
    {
        TRACE_CCAPP(" stop dtmf fail");
        return CC_RESULT_ERROR;
    }
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : check is in call state, include incoming and outgoing,connected
//  Global resource dependence : none
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsInState(CC_CALL_STATE_FOR_USING_E call_state)
{
    BOOLEAN result = FALSE;
    // TODO:黑名单相关判断
    if (0 == ual_tele_call_get_call_count())
    {
        TRACE_CCAPP("0 = call_count");
        return FALSE;
    }

    switch(call_state)
    {
    case CC_IN_CALL_STATE:
        TRACE_CCAPP("CC_IN_CALL_STATE");
#ifdef VT_SUPPORT
        if (MMIAPIVT_IsVtCalling())
#endif
        {
            result = TRUE;
        }
        break;

    case CC_MO_CONNECTING_STATE:
        TRACE_CCAPP("CC_MO_CONNECTING_STATE");
        if ((0 != MMICC_IsExistOutgoingCall())
#ifdef VT_SUPPORT
            ||MMIAPIVT_IsVtMoCalling()
#endif
            )
        {
            return TRUE;
        }
        break;
    case CC_MT_CONNECTING_STATE:
        TRACE_CCAPP("CC_MT_CONNECTING_STATE");
        if ((0 != MMICC_IsExistIncommingCall())
#ifdef VT_SUPPORT
            ||MMIAPIVT_IsVtMtIncoming()
#endif
            )
        {
            return TRUE;
        }
        break;
    case CC_CALL_CONNECTED_STATE:
        TRACE_CCAPP("CC_CALL_CONNECTED_STATE");
        if ((0 != MMIAPICC_GetActiveCallCount()) ||
            (0 != MMIAPICC_GetHoldCallCount())
#ifdef VT_SUPPORT
            ||MMIAPIVT_IsVtConnecting()
#endif
            )
        {
            return TRUE;
        }
        break;
    case CC_CALL_DISCONNECTED_STATE:
        TRACE_CCAPP("CC_CALL_DISCONNECTED_STATE");
        if(MMK_IsOpenWin(MMICC_DISCONNECT_WIN_ID))
        {
            return TRUE;
        }
        break;

    default:
        break;
    }
    TRACE_CCAPP("call_state = %d result = %d", call_state, result);
    return result;
}

//@zhaohui add
/*****************************************************************************/
//  Description : to get the name of call
//  Global resource dependence :
//  Author:zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_GetCallName(
                                 // uint8 call_id,//暂时屏蔽，不考虑vt多方通话
                                 MMI_STRING_T *name_str_ptr//OUT
                                 )
{
#ifndef UAL_TELE_SUPPORT
    if( s_call_context.call_info[s_call_context.current_call].name_len > 0)
    {
        name_str_ptr->wstr_len =  s_call_context.call_info[s_call_context.current_call].name_len;
        name_str_ptr->wstr_ptr = s_call_context.call_info[s_call_context.current_call].name;
    }
    else
    {
        MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  name_str_ptr);
    }
#endif
}

/*****************************************************************************/
//  Description : get current call number
//  Global resource dependence : none
//  Author: baokun
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_GetCurrentCallNumber(MMIPB_BCD_NUMBER_T *bcd_number_ptr, BOOLEAN is_remove_ip_head, BOOLEAN is_want_mo_call)
{
#ifndef UAL_TELE_SUPPORT
    GetCurrentCallNumber(bcd_number_ptr, is_remove_ip_head, is_want_mo_call);
#endif
}

/*****************************************************************************/
//  Description : clear missed call number
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ClearMissedCallNum(void)
{
    // TODO:
    s_missed_call_num = 0;
    MAIN_SetMissedCallState(FALSE);
    MMIAPICC_SaveMissedCallNumToNv();
    if(MMK_IsOpenWin(MMICC_MISSCALL_WIN_ID))
    {
        MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : handle MT Call disconnect
//  Global resource dependence :
//  Author:figo.feng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_HandleMTCallDisc(DPARAM param)
{
#ifndef UAL_TELE_SUPPORT
    return HandleMTCallDisc(param);
#else
    return FALSE; //cid603791
#endif
}

/*****************************************************************************/
//  Description :ShowCallDiscCause
//  Global resource dependence : none
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC CC_MN_CALL_DISC_CAUSE_E MMICC_ShowCallDiscCause(
                                                MN_CALL_DISCONNECT_CALL_CAUSE_E cause
                                                )
{
    CC_MN_CALL_DISC_CAUSE_E cause_type = CC_DISC_CAUSE_NORMAL;
#ifndef UAL_TELE_SUPPORT
    //normal cause
    if (cause <= MN_CAUSE_NORMAL_UNSPECIFIED)
    {
        switch(cause)
        {
            //cause by normal disconnect
        case MN_CAUSE_NORMAL_CLEARING:
        case MN_CAUSE_NORMAL_UNSPECIFIED:
            cause_type = CC_DISC_CAUSE_NORMAL;
            break;

            //cause by user busy
        case MN_CAUSE_USER_BUSY:
            cause_type = CC_DISC_CAUSE_USER_BUSY;
            break;

            //cause by user not responding
        case MN_CAUSE_NO_USER_RESPONDING:
        case MN_CAUSE_ALERTING_NO_ANSWER:
            cause_type = CC_DISC_CAUSE_USER_NOT_RESPOND;
            break;

            //cause by error number
        case MN_CAUSE_UNASSIGNED_NO:
        case MN_CAUSE_NUMBER_CHANGED:
        case MN_CAUSE_INVALID_NO_FORMAT:
            cause_type = CC_DISC_CAUSE_ERROR_NUMBER;
            break;

            //cause by cannot connect to user
        case MN_CAUSE_NO_ROUTE_TO_DEST:
        case MN_CAUSE_CHAN_UNACCEPTABLE:
        case MN_CAUSE_OPER_DETERM_BARRING:
        case MN_CAUSE_NONSEL_USER_CLRING:
        case MN_CAUSE_DEST_OUT_OF_ORDER:
        case MN_CAUSE_FACILITY_REJECTED:
        case MN_CAUSE_RSP_TO_STATUS_ENQ:
        case MN_CAUSE_CALL_REJECTED:
            cause_type = CC_DISC_CAUSE_CALL_REJECTED;
            break;

        default:
            break;
        }
    }
    // Resource Unavailable
    else if ((MN_CAUSE_NORMAL_UNSPECIFIED < cause) && (cause <= MN_CAUSE_RESOURCES_UNAV))
    {
        cause_type = CC_DISC_CAUSE_RES_UNAV;
    }
    else if (MN_CAUSE_REQ_FAC_NOT_SUBSCR == cause && MN_CALL_CLIR_INVOCATION == MMIAPISET_GetClirType(s_call_context.dual_sys))
    {
        cause_type = CC_DISC_CAUSE_HIDE_ID;
    }
    // Service or Option Not Available
    else if ((MN_CAUSE_RESOURCES_UNAV < cause) && (cause <= MN_CAUSE_SERV_OPT_UNAV))
    {
        cause_type = CC_DISC_CAUSE_SERVICE_UNAV;
    }
    // Call beings exceed limit
    else if (MN_CAUSE_ACM_EQ_OR_GT_ACMMAX == cause)
    {
        cause_type = CC_DISC_CAUSE_BEING_EXCEED_LIMIT;
    }
    // Service or Option Not Implement
    else if ((MN_CAUSE_SERV_OPT_UNAV < cause) && (cause <= MN_CAUSE_SVC_OPT_NOT_IMPL))
    {
        cause_type = CC_DISC_CAUSE_SERVICE_NOT_IMP;
    }
    // Invalid Message
    else if ((MN_CAUSE_SVC_OPT_NOT_IMPL < cause) && (cause <= MN_CAUSE_INVALID_MSG_SEMANTIC))
    {
        cause_type = CC_DISC_CAUSE_INVALID_MES;
    }
    // Protocol Error
    else if ((MN_CAUSE_INVALID_MSG_SEMANTIC < cause) && (cause <= MN_CAUSE_PROTOCOL_ERROR))
    {
        cause_type = CC_DISC_CAUSE_PROTOCOL_ERROR;
    }
    // Network Error
    else if ((MN_CAUSE_PROTOCOL_ERROR < cause) && (cause <= MN_CAUSE_INTERWORKING))
    {
        cause_type = CC_DISC_CAUSE_NETWORK_ERROR;
    }
    else
    {
        cause_type = CC_DISC_CAUSE_NORMAL;
    }
    TRACE_CCAPP("cause type = %d",cause_type);
#endif
    return cause_type;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsVideoCall(void)
{
#ifdef VT_SUPPORT
    return MMIAPIVT_IsVtCalling();
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : 获取当前通话时间信息
//  Global resource dependence :
//  Author:xiongxiaoyan
//  Note:获取当前通话时间信息
/*****************************************************************************/
PUBLIC uint32 MMICC_GetCurrentCallTime(void)
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_ERROR;
    ual_tele_call_data_info_t call_info = {0};
    uint32 cur_time_count = 0;
    uint32 cur_time = 0;
    ret = ual_tele_call_get_current_call_info(&call_info);
    if(UAL_TELE_CALL_RES_SUCCESS != ret)
    {
        TRACE_CCAPP("ual_tele_call_get_current_call_info fail");
        return 0;
    }
    cur_time = SCI_GetTickCount() / 1000;
    cur_time_count = (call_info.start_call_time == 0) ? 0: (cur_time - call_info.start_call_time);
    TRACE_CCAPP("cur_time_count = %d",cur_time_count);
    return cur_time_count;
}

/*****************************************************************************/
//  Description : 获取当前通话时间信息
//  Global resource dependence :
//  Author:xiongxiaoyan
//  Note:获取当前通话时间信息
/*****************************************************************************/
PUBLIC void MMIAPICC_GetCallTimeStr(MMI_STRING_T *prompt_str)
{
    uint8 time_str[CC_DISPLAY_TIME_LEN]={0};
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_ERROR;
    ual_tele_call_data_info_t call_info = {0};
    uint32 cur_time_count = 0;
    uint32 cur_time = 0;

    //通话中时间的计算:通过总时间减去该通电话
    // 的起始时间来获得
    ret = ual_tele_call_get_current_call_info(&call_info);
    if(UAL_TELE_CALL_RES_SUCCESS != ret)
    {
        TRACE_CCAPP("ual_tele_call_get_current_call_info fail");
        return;
    }
    cur_time = SCI_GetTickCount() / 1000;
    cur_time_count = cur_time - call_info.start_call_time;

    sprintf((char*)time_str,
        "%02d:%02d:%02d",
        (int)(cur_time_count / CC_HOUR_TO_SECOND),
        (int)((cur_time_count % CC_HOUR_TO_SECOND) / CC_MINUTE_TO_SECOND),
        (int)(cur_time_count % CC_MINUTE_TO_SECOND)
        );

    prompt_str->wstr_len = strlen((char*)time_str);
    //SCI_MEMCPY(prompt_str->wstr_ptr, time_str, strlen((char*)time_str));
    MMI_STRNTOWSTR(prompt_str->wstr_ptr, prompt_str->wstr_len, (uint8 *)time_str, CC_DISPLAY_TIME_LEN, prompt_str->wstr_len);
    return;
}

/*****************************************************************************/
//  Description : close MMICC_DISCONNECT_WIN_ID
//  Global resource dependence :
//  Author:samboo.shen
//  Note:2007.10.19
/*****************************************************************************/
PUBLIC void MMIAPICC_CloseDisconnectWin(void)
{
    MMK_CloseWin(MMICC_DISCONNECT_WIN_ID);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_ConnectCallReq(void)
{
    CC_ConnectCallReq();
}

/*****************************************************************************/
//  Description : 挂断当前电话
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseCurrentCallReq(MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    uint8 call_index = 0;
    call_index = MMICC_GetCurrentCallIndex();
    return CC_ReleaseCallReq(call_index);
}

/*****************************************************************************/
//  Description : making redial call , video call or normal call
//  Global resource dependence : s_call_text
//  Author:samboo.shen
//  Note:2007.7.27
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICC_MakeRedialCall( MN_DUAL_SYS_E dual_sys, uint8 *tele_num, uint8 tele_len)
{
#ifndef UAL_TELE_SUPPORT
    MMI_STRING_T name_str = {0};
    MMI_STRING_T *name_ptr = PNULL;
    wchar               str_buf[CC_MAX_PBNAME_LEN + 1] = {0};

    if (0 != s_call_context.want_mo_call.name_len)//this call is from a record
    {
        name_str.wstr_ptr = str_buf;
        if (s_call_context.want_mo_call.is_name_exist)
        {
            name_str.wstr_len = s_call_context.want_mo_call.name_len;
            MMI_WSTRNCPY(name_str.wstr_ptr,
                                        CC_MAX_PBNAME_LEN,
                                        s_call_context.want_mo_call.name,
                                        sizeof(s_call_context.want_mo_call.name),
                                        name_str.wstr_len);
        }
        name_ptr = &name_str;
    }
    else
    {
        name_ptr = PNULL;
    }

    TRACE_CCAPP("uto dial normal call...");
    if( CC_RESULT_SUCCESS != MMIAPICC_MakeCall(
        dual_sys,
        tele_num,
        tele_len,
        PNULL,
        name_ptr,
        MMIAPICC_GetCallSimType(dual_sys),
        CC_CALL_NORMAL_CALL,
        PNULL
        ))
    {
        return MMI_RESULT_FALSE;
    }
#endif
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : if the disc cause is "unobtainable destination-permanent/long term"
//  Global resource dependence :
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是号码错误,参见GSM 02.07
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsValideDest(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause)
{
#ifndef UAL_TELE_SUPPORT
    return CC_IsValideDest(disc_cause);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : if the disc cause is "busy destination" or "tempory"
//  Global resource dependence :
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是否时用户忙或者暂时无法接通,参见GSM 02.07
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsUserBusy(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause)
{
#ifndef UAL_TELE_SUPPORT
    return CC_IsUserBusy(disc_cause);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : get sim status when get call ready status msg
//  Global resource dependence :
//  Author:bown.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_GetSimCallReady(MN_DUAL_SYS_E dual_sys)
{
#ifndef UAL_TELE_SUPPORT
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        TRACE_CCAPP("dual_sys >= MMI_DUAL_SYS_MAX");
        return FALSE;
    }
    //TRACE_CCAPP("");

    return s_call_ready[dual_sys];
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : set sim status when switch card
//  Global resource dependence :
//  Author:bown.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SetSimCallReady(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ready)
{
#ifndef UAL_TELE_SUPPORT
    TRACE_CCAPP("is_ready = %d, dual_sys = %d",is_ready,dual_sys);
    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        s_call_ready[dual_sys] = is_ready;
    }
    else
    {
        TRACE_CCAPP("error dual_sys = %d",dual_sys);
    }
#endif
}

/*****************************************************************************/
//  Description : emergency call win msg
//  Global resource dependence :
//  Author:bownzhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsInputEmergencyCall(MMI_STRING_T *str_info_ptr, uint16 *emergency_call_len_ptr, MN_DUAL_SYS_E dual_sys)
{
    uint32 i = 0;
    uint32 j = 0;
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    wchar temp_emergency_call[CC_EMERGENCY_CALL_NUM_MAX_LEN + 1] = {0};
    uint16 temp_emergency_call_len = 0;
#endif
    BOOLEAN is_input_emergency_call = FALSE;
    uint8 *emergency_call_ptr = PNULL;
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    BOOLEAN is_local_exisit = FALSE;
    //BOOLEAN is_emergency_call_in_local = FALSE;
    const MMIPLMN_EMERGENCY_CALL_T *local_emergency_list_ptr = PNULL;
#endif

    if (PNULL == str_info_ptr || PNULL == emergency_call_len_ptr)
    {
        return FALSE;
    }

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    for (j = 0; j < MMI_DUAL_SYS_MAX; j++)
    {
        local_emergency_list_ptr = MMIAPIPLMN_GetLocalEmergency(i);//@@@

        if (PNULL != local_emergency_list_ptr && !is_local_exisit)
        {
            is_local_exisit = TRUE;
        }
        else
        {
            is_local_exisit = FALSE;
        }

        //local emergency call check
        if (PNULL != local_emergency_list_ptr)
        {
            for (i = 0; ; i++)
            {
                if (PNULL != local_emergency_list_ptr[i].emergency_call_ptr)
                {
                    SCI_MEMSET(temp_emergency_call, 0x00, sizeof(temp_emergency_call));
                    temp_emergency_call_len = strlen(local_emergency_list_ptr[i].emergency_call_ptr);
                    temp_emergency_call_len = MIN(temp_emergency_call_len, str_info_ptr->wstr_len);
                    MMI_STRNTOWSTR(temp_emergency_call,
                        CC_CALL_NUM_MAX_LEN_EX,
                        (uint8 *)local_emergency_list_ptr[i].emergency_call_ptr,
                        strlen(local_emergency_list_ptr[i].emergency_call_ptr),
                        temp_emergency_call_len);

                    if (0 != MMIAPICOM_CompareTwoWstr(str_info_ptr->wstr_ptr, str_info_ptr->wstr_len, temp_emergency_call, temp_emergency_call_len))
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else if (strlen(local_emergency_list_ptr[i].emergency_call_ptr) < str_info_ptr->wstr_len)
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else
                    {
                        *emergency_call_len_ptr = strlen(local_emergency_list_ptr[i].emergency_call_ptr);
                        is_input_emergency_call = TRUE;

                        return is_input_emergency_call;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
#endif

    for (j = 0; j < MMI_DUAL_SYS_MAX; j++)
    {
        if (UAL_TELE_SIM_STATUS_NO_SIM != ual_tele_sim_get_sim_status(j))
        {
            uint8 temp_tele_num[CC_MAX_TELE_NUM_LEN + 1] = {0};
            uint8 temp_tele_len = 0;
            MMI_WSTRNTOSTR(temp_tele_num,
                CC_MAX_TELE_NUM_LEN,
                str_info_ptr->wstr_ptr,
                str_info_ptr->wstr_len,
                str_info_ptr->wstr_len);
            temp_tele_len = strlen(temp_tele_num);
            *emergency_call_len_ptr = temp_tele_len;
            SCI_TRACE_LOW("[Editpad] HandleEditpadWinMsg ecc_strlen is %d",*emergency_call_len_ptr);
            if(ual_tele_call_is_emergency_num(j,temp_tele_num,temp_tele_len))
            {
                return TRUE;
            }
        }
    }

    return is_input_emergency_call;
}

/*****************************************************************************/
//  Description : is dtmf
//  Global resource dependence :
//  Author:bown.zhang
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_IsDTMF(uint8* dtmf_str, uint8 dtmf_len)
{
#ifndef UAL_TELE_SUPPORT
    uint32 i = 0;

    for (i = 0; i < dtmf_len; i++)
    {
        //DTMF 0-9,A,B,C,D,*,#
        if ((dtmf_str[i] >= '0' && dtmf_str[i] <= '9') ||
            (dtmf_str[i] >= 'A' && dtmf_str[i] <= 'D') ||
            dtmf_str[i] == '#' ||
            dtmf_str[i] =='*')
        {
            break;
        }
    }

    if (i >= s_call_context.dtmf_state.dtmf_len)
    {
        s_call_context.dtmf_state.is_dtmf= FALSE;

        MMK_CloseWin(WATCHCC_SEND_DTMF_NOTE_WIN_ID);
        MMK_CloseWin(WATCHCC_SEND_DTMF_QUERY_WIN_ID);
        CC_ClearStoredDtmf();

        return CC_RESULT_ERROR;
    }
#endif
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : send valid dtmf
//  Global resource dependence :
//  Author:bown.zhang
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SendDTMFStr(uint8* dtmf_str, uint8 dtmf_len)
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_MAX;
    ual_tele_call_data_info_t call_info = {0};
    ual_tele_call_dtmf_info_t dtmf_info = {0};

    ret = ual_tele_call_get_current_call_info(&call_info);
    if(UAL_TELE_CALL_RES_SUCCESS != ret)
    {
        TRACE_CCAPP("ual_tele_call_get_current_call_info fail");
        return CC_RESULT_ERROR;
    }

    dtmf_info.sim_id = call_info.sim_id;
    dtmf_info.dtmf_len = dtmf_len;
    SCI_MEMCPY(dtmf_info.dtmf_str,dtmf_str,dtmf_len)

    ret = ual_tele_call_send_string_dtmf(dtmf_info);
    if(UAL_TELE_CALL_RES_SUCCESS != ret)
    {
        TRACE_CCAPP("ual_tele_call_send_string_dtmf fail");
        return CC_RESULT_ERROR;
    }
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : get call error type
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMICC_MO_CALL_ERROR_TYPE_E MMICC_GetMoCallErrorType(void)
{
    return s_cc_mo_error_type;
}

/*****************************************************************************/
//  Description : Make Call Interface
//  Global resource dependence :
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_MakeCall(
   MN_DUAL_SYS_E dual_sys,//current sim
   uint8 *tele_num,//phone number
   uint8 tele_len,//phone number wstr_len
   MMI_STRING_T* disp_ptr,//disp ptr
   MMI_STRING_T* name_ptr,//name ptr
   CC_CALL_SIM_TYPE_E call_sim_type,//call  sim type
   uint32 call_type, //call type
   MN_SUBADDR_T* sub_addr //STK中拨打分机号
   )
{
    MN_DUAL_SYS_E call_dual_sys = MN_DUAL_SYS_MAX;
    CC_MAKE_CALL_INFO make_call_info = {0};/*lint !e64*/
    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

    if(PNULL == tele_num)
    {
        TRACE_CCAPP("tele_num is NULL");
        return CC_RESULT_ERROR;
    }

    //先解析是否跟SIM卡操作有关
    if (!(CC_CALL_STK_CALL & call_type))
    {
        if(MMIAPIPARSE_ParseString(tele_num, tele_len))
        {
            TRACE_CCAPP("Parsed in ParseString");
            return cc_result;
        }
    }

    if (MMIAPICC_IsVideoCall())
    {
        TRACE_CCAPP("MakeCall req is reject when vt");
        //MMIPUB_OpenAlertWarningWin(TXT_CC_FORBID_MAKECALL_WHEN_VT);
        s_cc_mo_error_type = CC_MO_FORBID_IN_VT;
        return CC_RESULT_ERROR;
    }

    TRACE_CCAPP("dual_sys=%d,call_sim_type=%d,call_type=%d", dual_sys, call_sim_type, call_type);

    //store make call info
    SCI_MEMSET(&make_call_info, 0x00, sizeof(CC_MAKE_CALL_INFO));
    make_call_info.dual_sys = dual_sys;
    make_call_info.tele_len = MIN(tele_len, CC_MAX_TELE_NUM_LEN);
    SCI_MEMCPY(make_call_info.tele_num, tele_num, make_call_info.tele_len);
    make_call_info.disp_ptr= disp_ptr;
    make_call_info.name_ptr = name_ptr;
    make_call_info.call_sim_type = call_sim_type;
    make_call_info.call_type = call_type;
    make_call_info.sub_addr= sub_addr;

    if((CC_CALL_STK_CALL & call_type) && (MMI_DUAL_SYS_MAX > dual_sys))
    {
        call_dual_sys = dual_sys;
    }
#ifdef VT_SUPPORT//Bug 336791
    else if (call_type & CC_CALL_VIDEO_CALL) //video Call
    {
        call_dual_sys = MN_DUAL_SYS_1;
    }
#endif
    else
    {
        //call_dual_sys = GetMoCallSimIdEx(dual_sys, tele_num, tele_len, call_sim_type);
        call_dual_sys = MN_DUAL_SYS_1;
    }

    TRACE_CCAPP("call_dual_sys=%d", call_dual_sys);
    // TODO: make ecc in fly mode

    cc_result = MMICC_ProcessMakeCall(call_dual_sys, &make_call_info);
    return cc_result;
}

/*****************************************************************************/
//  Description : Process Make Call
//  Global resource dependence :
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ProcessMakeCall(
   MN_DUAL_SYS_E dual_sys,
   CC_MAKE_CALL_INFO *make_call_info_ptr
   )
{
   BOOLEAN is_num_valid = FALSE;
#ifdef MMI_IP_CALL_SUPPORT
   BOOLEAN is_ip_call = FALSE;
#endif
   BOOLEAN is_call_video_call = FALSE;
   BOOLEAN is_stk_call = FALSE;
   CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
#ifndef UAL_TELE_SUPPORT
   MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
   uint8 copy_len = 0;
   MMI_STRING_T *new_call_name_ptr = {0};
#endif
   uint8 tele_number[CC_MAX_TELE_NUM_LEN + 1] = {0};
   uint8 tele_len = 0;
   MMI_PARSE_TYPE_E parse_result = MMI_PARSE_NORMAL_CALL_TYPE_E;
#ifdef MV_SUPPORT
   BOOLEAN is_need_open_mv = FALSE;
#endif

    if(PNULL == make_call_info_ptr)
    {
        TRACE_CCAPP("make_call_info_ptr is NULL");
        return CC_RESULT_ERROR;
    }

   TRACE_CCAPP("dual_sys=%d",dual_sys);

// TODO:自动测试中,避免呼出一些报警电话等
#ifndef UAL_TELE_SUPPORT
    //MS00186848 用于自动测试中,避免呼出一些报警电话等
    if (MMIAPIENG_GetCallBarring() && IsIncludeBarringNumbers(make_call_info_ptr->tele_num, make_call_info_ptr->tele_len))
    {
        TRACE_CCAPP("MO call is barring: tele_num = %s***",make_call_info_ptr->tele_num);
        //MMIPUB_OpenAlertWarningWin(TXT_CC_CANNOT_CALL);
        s_cc_mo_error_type = CC_MO_IS_BARRING_NUM;
        return CC_RESULT_ERROR;
    }
#endif

   //parse call type
#ifdef MMI_IP_CALL_SUPPORT
    if (make_call_info_ptr->call_type & CC_CALL_IP_CALL) //IP Call
    {
       is_ip_call = TRUE;
    }
#endif
#ifdef VT_SUPPORT
    if (make_call_info_ptr->call_type & CC_CALL_VIDEO_CALL) //video Call
    {
       is_call_video_call = TRUE;
    }
#endif
    if (make_call_info_ptr->call_type & CC_CALL_STK_CALL) //stk Call
    {
       is_stk_call = TRUE;
    }
#ifdef MV_SUPPORT
    is_need_open_mv = IsCanUseMagicVoice(TRUE)
                   && !MMK_IsOpenWin(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID)
                   && (0 == s_call_context.call_number)
                   && !MMIAPICC_IsEmergencyNum(make_call_info_ptr->tele_num, make_call_info_ptr->tele_len, dual_sys, FALSE)
                   && !MMK_IsOpenWin(MMICC_AUTO_DIAL_WIN_ID);
#endif
    if ((MN_DUAL_SYS_MAX > dual_sys)
#ifdef MV_SUPPORT
    && !is_need_open_mv
#endif
    )
    {
#ifdef MMI_IP_CALL_SUPPORT
       if (is_ip_call) //IP Call
       {
           uint8 ip_number[MMISET_IPNUMBER_MAX_LEN + 1] = {0};
           BOOLEAN is_ip_active = FALSE;
           is_ip_active = MMIAPISET_GetActiveIPNumber(dual_sys, ip_number, (MMISET_IPNUMBER_MAX_LEN + 1));

           if (is_ip_active)
           {
               SCI_MEMSET(tele_number, 0x00, sizeof(tele_number));

               is_num_valid = CC_GetIPCallNum(dual_sys, make_call_info_ptr->tele_num, make_call_info_ptr->tele_len, tele_number, sizeof(tele_number));
               if (!is_num_valid)
               {
                   //MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
                   s_cc_mo_error_type = CC_MO_INVALID_NUM;
                   return CC_RESULT_ERROR;
               }
           }
           else
           {
               //MMIPUB_OpenAlertWarningWin(TXT_CC_NO_IP_NUM);//MS00189367
               s_cc_mo_error_type = CC_MO_NO_IP_NUM;
               return CC_RESULT_ERROR;
           }
       }
       else
#endif
       {
           MMI_MEMCPY(
               tele_number,
               sizeof(tele_number),
               make_call_info_ptr->tele_num,
               make_call_info_ptr->tele_len,
               MIN(sizeof(tele_number), make_call_info_ptr->tele_len)
               );
       }

       tele_len = strlen((char*)tele_number);

       if (!is_stk_call)
       {
            parse_result = MMIAPIPARSE_ParseTeleNum(dual_sys, tele_number, tele_len, is_call_video_call);
       }

       TRACE_CCAPP("parse_result=%d", parse_result);

       if (MMI_PARSE_CALL_CLIR_SUPPRESSION_E == parse_result
           || MMI_PARSE_CALL_CLIR_INVOCATION_E == parse_result
           || MMI_PARSE_NORMAL_CALL_TYPE_E == parse_result)
       {

            if (ual_tele_call_get_call_count()> 0)//cr128072
            {
                ual_tele_call_data_info_t call_info = {0};
                ual_tele_call_get_current_call_info(&call_info);
                if(call_info.sim_id != dual_sys)
                {
                    //准备呼出的sim卡和正在通话中的sim卡不相同，则不能呼出
                    TRACE_CCAPP("sim corrupt!");
                    //MMIPUB_OpenAlertWarningWin(TXT_SIM_CORRUPT);
                    s_cc_mo_error_type = CC_MO_SIM_CORRUPT;
                    return CC_RESULT_ERROR;
                }
            }
#ifndef UAL_TELE_SUPPORT
           if (0 != s_call_context.exist_call.exist_active_call_num)//已有通话
           {
               if (0 != s_call_context.exist_call.exist_hold_call_num) //已有保持通话
               {
                   //提示用户需先建立MPTY
#if !defined(MMI_GUI_STYLE_TYPICAL)
                    //MMIPUB_OpenAlertWarningWin(TXT_CC_MERGE_CALLS_FIRST);
                   s_cc_mo_error_type = CC_MO_MERGR_CALL_FIRST;
                   MMIPUB_SetWinTitleTextId( MMIPUB_ALERT_WIN_ID, TXT_CC_COMMUN, FALSE );
#else
                   //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_EST_MPTY_FIRST,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKEXIT,PNULL);
                   s_cc_mo_error_type = CC_MO_MERGR_CALL_FIRST;
#endif
                   return CC_RESULT_ERROR;
               }
               else
               {
                    SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, (sizeof(s_call_context.want_mo_call)));

                    //set is stk call
                    s_call_context.want_mo_call.call_type = make_call_info_ptr->call_type;

                   //set the information for s_call_context
                   s_call_context.want_mo_call.is_want_mo_call = TRUE;

                   MMIAPICOM_GenPartyNumber(tele_number, tele_len, &party_num );

                   s_call_context.want_mo_call.address.number_type = party_num.number_type ;

                   s_call_context.want_mo_call.address.number_plan = MN_NUM_PLAN_ISDN_TELE;

                   SCI_MEMSET((void*)s_call_context.want_mo_call.address.party_num, 0xFF, sizeof(s_call_context.want_mo_call.address.party_num));

                   copy_len = ( party_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: party_num.num_len;
                   s_call_context.want_mo_call.address.num_len = copy_len ;
                   MMI_MEMCPY(
                       (void*)s_call_context.want_mo_call.address.party_num,
                       sizeof(s_call_context.want_mo_call.address.party_num),
                       (void*)party_num.bcd_num,
                       sizeof(party_num.bcd_num),
                       copy_len
                       );

                    if (PNULL != make_call_info_ptr->name_ptr)
                    {
                        new_call_name_ptr = make_call_info_ptr->name_ptr;
                        if (0 != new_call_name_ptr->wstr_len && PNULL != new_call_name_ptr->wstr_ptr)
                        {
                            s_call_context.want_mo_call.is_name_exist = TRUE;//传入的姓名不为空,这是从ADN/FDN/SDN发起的通话. this is a call from ADN/FDN/SDN list

                            s_call_context.want_mo_call.name_len = new_call_name_ptr->wstr_len;
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
                            copy_len = s_call_context.want_mo_call.name_len;
#else
                            copy_len = ( s_call_context.want_mo_call.name_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : s_call_context.want_mo_call.name_len;/*lint !e506 */
#endif
                            MMI_WSTRNCPY(
                                s_call_context.want_mo_call.name,
                                sizeof(s_call_context.want_mo_call.name),
                                new_call_name_ptr->wstr_ptr,
                                new_call_name_ptr->wstr_len,
                                copy_len
                                );
                        }
                        else
                        {
                            s_call_context.want_mo_call.is_name_exist = FALSE;

                            s_call_context.want_mo_call.name_len = tele_len;
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
                            copy_len = tele_len;
#else
                            copy_len = ( tele_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : tele_len;
#endif
                            MMI_STRNTOWSTR(
                                s_call_context.want_mo_call.name,
                                s_call_context.want_mo_call.name_len,
                                tele_number,
                                tele_len,
                                copy_len
                                );
                            s_call_context.want_mo_call.name_len = copy_len;
                        }
                    }

                   //get the active call
                   if ( CC_CALL_STATE != s_call_context.call_info[ s_call_context.current_call ].call_state)
                   {
                       s_call_context.current_call = FindStateCall( CC_CALL_STATE );
                       if (s_call_context.current_call >= s_call_context.call_number)
                       {
                           //TRACE_CCAPP(" ");
                           return CC_RESULT_ERROR;
                       }
                   }

                   //hold the active for preparing to send mo call
                   if ( MN_RETURN_SUCCESS != MNCALL_HoldCallEx(s_call_context.dual_sys, s_call_context.call_info[ s_call_context.current_call ].call_id ) )
                   {
                       ClearWantCallInfo();
                       //TRACE_CCAPP(" ");

                       return CC_RESULT_ERROR;
                   }
                   else
                   {
                       s_is_need_call_after_hold = TRUE;
                   }

                   //更新显示提示信息
                   s_call_context.call_info[s_call_context.current_call].operated_by_user = CC_HOLD_OPER;
               }
           }
           else
#endif //#ifndef UAL_TELE_SUPPORT
           {
               if ((MMI_PARSE_CALL_CLIR_SUPPRESSION_E == parse_result) && (4 < tele_len))
               {
                   cc_result = ProcessPhoneNumExt(dual_sys, tele_number + 4, tele_len - 4);
               }
               else if ((MMI_PARSE_CALL_CLIR_INVOCATION_E == parse_result) && (4 < tele_len))
               {
                   cc_result = ProcessPhoneNumExt(dual_sys, tele_number + 4, tele_len - 4);
               }
               else// if (MMI_PARSE_NORMAL_CALL_TYPE_E == parse_result)
               {
                   cc_result = ProcessPhoneNumExt(dual_sys, tele_number, tele_len);
               }
           }
       }
       else
       {
           TRACE_CCAPP(" has handled in parse function");

#ifdef MMI_NOT_SUPPORT_USSD_SS_INCC
            if((MMI_PARSE_SS_TYPE_E == parse_result) || (MMI_PARSE_USSD_TYPE_E == parse_result))
            {
                TRACE_CCAPP("MMICC_ProcessMakeCall:parse_result:%d\n",parse_result);
                s_cc_mo_error_type = CC_MO_CMD_NO_SUPPORT;
                cc_result = CC_RESULT_ERROR;
            }
#endif
           return cc_result;
       }
    }
    else//第一通去电时,才可以主动打开魔音选择窗口
    {
        CC_MAKE_CALL_INFO * call_info_ptr = PNULL;

        if (MMIAPISET_GetFlyMode())
        {
            //MMIAPIPHONE_AlertSimNorOKStatus();
            s_cc_mo_error_type = CC_MO_IN_FLY_MODE;
            return CC_RESULT_ERROR;
        }

        call_info_ptr = (CC_MAKE_CALL_INFO *)SCI_ALLOCA(sizeof(CC_MAKE_CALL_INFO));
        if (PNULL != call_info_ptr)
        {
            SCI_MEMSET(call_info_ptr, 0x00, sizeof(CC_MAKE_CALL_INFO));
            SCI_MEMCPY(call_info_ptr, make_call_info_ptr, sizeof(CC_MAKE_CALL_INFO));

            if (PNULL != make_call_info_ptr->disp_ptr)
            {
                call_info_ptr->disp_ptr = (MMI_STRING_T *)SCI_ALLOCA(sizeof(MMI_STRING_T));
                SCI_MEMSET(call_info_ptr->disp_ptr, 0x00, sizeof(MMI_STRING_T));
                SCI_MEMCPY(call_info_ptr->disp_ptr, make_call_info_ptr->disp_ptr, sizeof(MMI_STRING_T));

                if (PNULL != make_call_info_ptr->disp_ptr->wstr_ptr)
                {
                    call_info_ptr->disp_ptr->wstr_ptr = SCI_ALLOCA((make_call_info_ptr->disp_ptr->wstr_len + 1)*sizeof(wchar));
                    SCI_MEMSET(call_info_ptr->disp_ptr->wstr_ptr, 0x00, (make_call_info_ptr->disp_ptr->wstr_len*sizeof(wchar)));
                    MMI_WSTRNCPY(call_info_ptr->disp_ptr->wstr_ptr,
                        make_call_info_ptr->disp_ptr->wstr_len,
                        make_call_info_ptr->disp_ptr->wstr_ptr,
                        make_call_info_ptr->disp_ptr->wstr_len,
                        make_call_info_ptr->disp_ptr->wstr_len);
                }
            }

            if (PNULL != make_call_info_ptr->name_ptr)
            {
                call_info_ptr->name_ptr = (MMI_STRING_T *)SCI_ALLOCA(sizeof(MMI_STRING_T));
                SCI_MEMSET(call_info_ptr->name_ptr, 0x00, sizeof(MMI_STRING_T));
                SCI_MEMCPY(call_info_ptr->name_ptr, make_call_info_ptr->name_ptr, sizeof(MMI_STRING_T));

                if (PNULL != make_call_info_ptr->name_ptr->wstr_ptr)
                {
                    call_info_ptr->name_ptr->wstr_ptr = SCI_ALLOCA((make_call_info_ptr->name_ptr->wstr_len + 1)*sizeof(wchar));
                    SCI_MEMSET(call_info_ptr->name_ptr->wstr_ptr, 0x00, (make_call_info_ptr->name_ptr->wstr_len*sizeof(wchar)));
                    MMI_WSTRNCPY(call_info_ptr->name_ptr->wstr_ptr,
                        make_call_info_ptr->name_ptr->wstr_len,
                        make_call_info_ptr->name_ptr->wstr_ptr,
                        make_call_info_ptr->name_ptr->wstr_len,
                        make_call_info_ptr->name_ptr->wstr_len);
                }
            }

#ifdef MV_SUPPORT
            if (MN_DUAL_SYS_MAX == dual_sys)//先进行双卡的选择
#endif
            {
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
                MMICC_OpenSelectSimWin((ADD_DATA) call_info_ptr);
#endif
            }
#ifdef MV_SUPPORT
            else
            {
                if (!is_stk_call)
                {
                     parse_result = MMIAPIPARSE_ParseTeleNum(dual_sys, make_call_info_ptr->tele_num, make_call_info_ptr->tele_len, is_call_video_call);
                }

                if ((MMI_PARSE_CALL_CLIR_SUPPRESSION_E == parse_result)
                    || (MMI_PARSE_CALL_CLIR_INVOCATION_E == parse_result)
                    || (MMI_PARSE_NORMAL_CALL_TYPE_E == parse_result))
                {
                    call_info_ptr->dual_sys = dual_sys;
                    MMICC_OpenMVBeforeCallConnected((ADD_DATA)call_info_ptr);//OpenMagicVoiceSelectWindow((ADD_DATA) call_info_ptr);
                }
                else
                {
                    if (PNULL != call_info_ptr->disp_ptr)
                    {
                        if (PNULL != call_info_ptr->disp_ptr->wstr_ptr)
                        {
                            SCI_FREE(call_info_ptr->disp_ptr->wstr_ptr);
                            call_info_ptr->disp_ptr->wstr_ptr = PNULL;
                        }

                        SCI_FREE(call_info_ptr->disp_ptr);
                        call_info_ptr->disp_ptr = PNULL;
                    }

                    if (PNULL != call_info_ptr->name_ptr)
                    {
                        if (PNULL != call_info_ptr->name_ptr->wstr_ptr)
                        {
                            SCI_FREE(call_info_ptr->name_ptr->wstr_ptr);
                            call_info_ptr->name_ptr->wstr_ptr = PNULL;
                        }

                        SCI_FREE(call_info_ptr->name_ptr);
                        call_info_ptr->name_ptr = PNULL;
                    }

                    SCI_FREE(call_info_ptr);
                    call_info_ptr = PNULL;

#ifdef MMI_NOT_SUPPORT_USSD_SS_INCC
                    if((MMI_PARSE_SS_TYPE_E == parse_result) || (MMI_PARSE_USSD_TYPE_E == parse_result))
                    {
                        TRACE_CCAPP("MMICC_ProcessMakeCall:mv_support parse_result:%d\n",parse_result);
                        s_cc_mo_error_type = CC_MO_CMD_NO_SUPPORT;
                        return CC_RESULT_ERROR;
                    }
#endif
                }
            }
#endif
        }
        else
        {
            TRACE_CCAPP(" Alloc memory Fail");

            return CC_RESULT_ERROR;
        }
    }

    return cc_result;
}

#ifndef MMI_MULTI_GREEN_KEY_SINGLE
/*****************************************************************************/
//  Description : get call sim type according to call key
//  Global resource dependence :
//  Author: Michael.shi
//  Note: especially for mulstim.
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_CheckDualSysType(MMI_MESSAGE_ID_E msg_id, MN_DUAL_SYS_E dual_sys)
{
    MN_DUAL_SYS_E new_dual_sys;

    new_dual_sys = dual_sys;
    switch (msg_id)
    {
        case MSG_KEYUP_CALL1: // MSG_KEYUP_GREEN
        case MSG_KEYDOWN_CALL1: //MSG_KEYDOWN_GREEN
            new_dual_sys = MN_DUAL_SYS_1;
            break;
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL2:
        case MSG_KEYDOWN_CALL2:
            new_dual_sys = MN_DUAL_SYS_2;
            break;
#endif
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL3:
        case MSG_KEYDOWN_CALL3:
            new_dual_sys = MN_DUAL_SYS_3;
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL4:
        case MSG_KEYDOWN_CALL4:
            new_dual_sys = MN_DUAL_SYS_4;
            break;
#endif

        default:
            break;
    }

    return new_dual_sys;
}

/*****************************************************************************/
//  Description : get call sim type according to call key
//  Global resource dependence :
//  Author: Michael.shi
//  Note: especially for mulstim.
/*****************************************************************************/
PUBLIC CC_CALL_SIM_TYPE_E MMIAPICC_CheckCallSimType(MMI_MESSAGE_ID_E msg_id, CC_CALL_SIM_TYPE_E sim_type)
{
    CC_CALL_SIM_TYPE_E new_call_sim_type;

    new_call_sim_type = sim_type;
    switch (msg_id)
    {
        case MSG_KEYUP_CALL1:            //MSG_KEYUP_GREEN
        case MSG_KEYDOWN_CALL1:    //MSG_KEYDOWN_GREEN
            new_call_sim_type = CC_SIM1_CALL;
            break;

        case MSG_KEYUP_CALL2:
        case MSG_KEYDOWN_CALL2:
            new_call_sim_type = CC_SIM2_CALL;
            break;

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL3:
        case MSG_KEYDOWN_CALL3:
            new_call_sim_type = CC_SIM3_CALL;
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL4:
        case MSG_KEYDOWN_CALL4:
            new_call_sim_type = CC_SIM4_CALL;
            break;
#endif

        default:
            break;
    }

    return new_call_sim_type;
}
#endif /* MMI_MULTI_GREEN_KEY_SINGLE */

/*****************************************************************************/
//  Description : get call sim type
//  Global resource dependence :
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC CC_CALL_SIM_TYPE_E MMIAPICC_GetCallSimType(MN_DUAL_SYS_E dual_sys)
{
    CC_CALL_SIM_TYPE_E call_sim_type = CC_CALL_SIM_MAX;

    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        call_sim_type = (CC_CALL_SIM_TYPE_E)(CC_SIM1_CALL + ((CC_CALL_SIM_TYPE_E)dual_sys - MN_DUAL_SYS_1));
    }
    else if(dual_sys == MMI_DUAL_SYS_MAX)
    {
        call_sim_type = CC_CALL_SIM_MAX;
    }
    else
    {
        // do nothing now!!!
    }
    TRACE_CCAPP("dual_sys=%d,call_sim_type=%d", dual_sys, call_sim_type);

    return call_sim_type;
}

/*****************************************************************************/
//  Description :judge whether the mpty can be setup
//  Global resource dependence : none
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsMptySetupAvailable(void)
{
    BOOLEAN is_mpty_setup_available = FALSE;
#ifndef UAL_TELE_SUPPORT
    uint8 call_index = 0;

    //电话数目有2个或以上，并且有电话不在mpty（多方通话）里面，则判定可以建立多方通话
    if (s_call_context.call_number > 1)
    {
        for( call_index = 0; call_index < s_call_context.call_number ; call_index++ )
        {
            if (!s_call_context.call_info[ call_index ].is_in_mpty)
            {
                is_mpty_setup_available = TRUE;
                break;
            }
        }
    }
#endif
    TRACE_CCAPP("CC_IsMptySetupAvailable %d",is_mpty_setup_available);
    return is_mpty_setup_available;
}

/*****************************************************************************/
//  Description :judge whether the mpty can be split
//  Global resource dependence : none
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsMptySplitAvailable(void)
{
    BOOLEAN is_mpty_split_available = FALSE;
#ifndef UAL_TELE_SUPPORT
    //存在mpty（多方通话），并且无法建立mpty，则分离mpty是可行的,如果一个mpty，而且是held，也不能分离
    if (MMICC_IsExistMPTY() && !MMICC_IsMptySetupAvailable() && MMICC_IsExistActiveCall())
    {
        is_mpty_split_available = TRUE;
    }
#endif
    TRACE_CCAPP("CC_IsMptySplitAvailable %d",is_mpty_split_available);
    return is_mpty_split_available;
}

/*****************************************************************************/
//  Description : Release call by red key: release incoming and outgoing call in high priority;
//                active call normal priority; hold call low priority
//  Global resource dependence : s_call_context
//  Author:jibin
//  Note:2004.10.14
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_ReleaseCallByRedkey(void)
{
    //如果有来电去电，优先挂断
    TRACE_CCAPP("enter func");
    if ((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()) ||
        (CC_WAITING_STATE == MMICC_GetCurrentCallStatus()))
    {
        uint8 call_index = 0;
        call_index = MMICC_GetCurrentCallIndex();
        return CC_ReleaseCallReq(call_index);
    }

    if (CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
    {
        uint8 call_index = 0;
        call_index = MMICC_GetCurrentCallIndex();
        return CC_ReleaseCallReq(call_index);
    }

    //激活电话存在，挂断
    if (MMICC_IsExistActiveCall())
    {
        return CC_ReleaseActiveCall();
    }

    //最后挂断保留电话
    if (MMICC_IsExistHoldCall())
    {
        return CC_ReleaseHoldCall();
    }

    if( MMK_IsOpenWin(MMICC_AUTO_DIAL_WIN_ID) )
    {
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
        MMICC_ClearAutoRedialInfo();
#endif
        MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );
    }

    return CC_RESULT_ERROR;
}

/*****************************************************************************/
//  Description : 获取当前呼叫的类型
//  Global resource dependence :
//  Author:xiongxiaoyan
//  Note:20060404
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_GetCallDualSys(void)
{
    ual_tele_call_data_info_t call_info = {0};
    if (UAL_TELE_CALL_RES_SUCCESS == call_data_get_current_call_info(&call_info))
    {
        TRACE_CCAPP("call_info.sim_id = %d",call_info.sim_id);
        return call_info.sim_id;
    }
    return MMI_DUAL_SYS_MAX;
}

/*****************************************************************************/
//  Description : get missed call number
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICC_GetMissedCallNumber(void)
{
    TRACE_CCAPP("s_missed_call_num = %d",s_missed_call_num);
    return s_missed_call_num;
}

/*****************************************************************************/
//  Description : dec missed call number
//  Global resource dependence :
//  Author:tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_DecMissedCallNumber(uint32 call_times)
{
    s_missed_call_num -= call_times;
    if (s_missed_call_num == 0)
    {
        //关闭idle状态下提示未接电话的窗口以及状态栏图标
        MMIAPICC_ClearMissedCallNum();
    }
    MMIAPICC_SaveMissedCallNumToNv();
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
    MMIAPICC_ShowMCHint();
#endif
}

/*****************************************************************************/
//  Description : get missed call number
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SaveMissedCallNumToNv(void)
{
    MMINV_WRITE(MMI_CC_MISSED_CALL_NUM, &s_missed_call_num);
}

/*****************************************************************************/
//  Description : get missed call number
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIAPICC_LoadMissedCallNumFromNv(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    uint32 missed_call_num = 0;

    MMINV_READ(MMI_CC_MISSED_CALL_NUM, &missed_call_num, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMI_CC_MISSED_CALL_NUM, &missed_call_num);
    }
    s_missed_call_num = missed_call_num;
    TRACE_CCAPP("s_missed_call_num = %d",s_missed_call_num);

    if (missed_call_num > 0)
    {
        MAIN_SetMissedCallState(TRUE);
    }
    else //missed_call_num == 0
    {
        MAIN_SetMissedCallState(FALSE);
    }

    return return_value;
}

BOOLEAN FilterMsgFromATCmdCallEX(MMI_MESSAGE_ID_E    msg_id)
{
#ifndef UAL_TELE_SUPPORT
    return FilterMsgFromATCmdCall(msg_id,PNULL);
#else
    return FALSE;
#endif

}

/*****************************************************************************/
//  Description : get missed call number
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsCallByMMI(void)
{
#ifndef UAL_TELE_SUPPORT
    return ((s_dial_method != CC_DIAL_METHOD_OTHER) && (s_dial_method != CC_DIAL_METHOD_MAX));
#else
    return TRUE;
#endif
}

/*****************************************************************************/
//  Discription: get the message ID_STATUS_HFAG_HFU_HOLD
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICC_HandleBtHfuHoldInd(BT_MSG_T* msg_body_ptr )
{
#ifdef BLUETOOTH_SUPPORT

    BT_DIAL_ACTION_T hold_info = *((BT_DIAL_ACTION_T* )msg_body_ptr->body_ptr);
    TRACE_CCAPP("the hf hold action is %d",hold_info.hold_action);

    //call cc function according hold action
    if (hold_info.hold_action == BT_HOLD_RELEASE_HELD_CALLS)
    {
        //CC_ReleaseHoldCall();
        // hangup the incoming call
        TRACE_CCAPP("zyx,receive at+chld=0");
        if((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()) ||
            (CC_WAITING_STATE == MMICC_GetCurrentCallStatus()))
        {
            CC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(), MN_CAUSE_USER_BUSY);
            TRACE_CCAPP("zyx,update call setup status");
#ifdef BT_BQB_SUPPORT
            MMIAPIBT_CIEVCommandIndicator(BT_CALL_SETUP_STATUS_INDICATOR,0);
#endif
        }
        else
        {
            TRACE_CCAPP("zyx,CC_ReleaseHoldCall");
            CC_ReleaseHoldCall();
        }
        TRACE_CCAPP("release incoming call");
    }
    else if (hold_info.hold_action == BT_HOLD_RELEASE_ACTIVE_CALLS)
    {
        //此处暂时调用CRSS相关方法来完成，需要关注该宏MMI_CRSS_SUPPORT的开关!
#ifdef MMI_CRSS_SUPPORT
        CC_CRSSReleaseAndAnswer();
#else
        TRACE_CCAPP("CRSS no longer supported! We should rewrite the func for BT_HOLD_RELEASE_ACTIVE_CALLS.");
#endif
    }
    else if (hold_info.hold_action == BT_HOLD_HOLD_ACTIVE_CALLS)
    {
        //Places all active calls (if any exist) on hold and accepts the other (held or waiting) call.
        if (s_call_context.exist_call.exist_incoming_call)
    {
            CC_HoldAndAnswerCW();
            TRACE_CCAPP("CC_HoldAndAnswerCW");
    }
        else if(s_call_context.exist_call.exist_active_call_num > 0
            || s_call_context.exist_call.exist_hold_call_num > 0
            && s_call_context.exist_call.exist_outgoing_call == 0)
    {
            CC_HoldCall();
            TRACE_CCAPP("CC_HoldCall");
        }
        else
        {
            TRACE_CCAPP("the hold action is not support!");
        }
    }

    else if (hold_info.hold_action == BT_HOLD_ADD_HELD_CALL)
    {
        if(MMICC_IsExistActiveAndHoldCalls())
        {
            CC_BuildMPTYCall();
            TRACE_CCAPP("CC_HandleBtHfuHoldInd: BT_HOLD_ADD_HELD_CALL");
            }
            else
        {
            TRACE_CCAPP("CC_HandleBtHfuHoldInd: no active call or hold call");
        }
    }
    else
    {
        TRACE_CCAPP("the hold action is not support!");
    }
#endif
}

/*****************************************************************************/
//  Description : to get the number of call
//  Global resource dependence :
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetCallNumber(
                                   uint8* number_ptr, //out
                                   uint8 number_max_len
                                   )
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_ERROR;
    ual_tele_call_data_info_t call_info = {0};
    uint8 tel_num_len = 0;

    if((PNULL == number_ptr))
    {
        TRACE_CCAPP(" number_ptr is PNULL");
        return tel_num_len;
    }

    ret = ual_tele_call_get_current_call_info(&call_info);
    if(UAL_TELE_CALL_RES_SUCCESS != ret)
    {
        TRACE_CCAPP(" get current call info fail");
        return tel_num_len;
    }

    tel_num_len = MIN(call_info.tele_num_len,CC_MAX_TELE_NUM_LEN);
    SCI_MEMCPY(number_ptr,call_info.tele_num,tel_num_len)
    TRACE_CCAPP(" number_ptr = %s, tel_num_len = %d",number_ptr,tel_num_len);

    return tel_num_len;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_AnswerCall(void)
{
#ifdef ASP_SUPPORT
    s_accept_call = TRUE;
#endif
    //MMICC_StopRingOrVibrateBeforeCCing(); //modified by jun.hu for 此处对TTS采用异步停止的方式，以解决接听要2S的问题

    if( !MMICC_IsCurrentCallIDValid())
    {
        //can not do other handle in this state
        return FALSE;
    }

    //根据当前CALL的状态进行相应的处理
    switch(MMICC_GetCurrentCallStatus())
    {
    case CC_CALLING_STATE:
        //避免重拨的过程中，按下OK键，导致不能准确的重拨
        //                CLEAAR_AUTOREDIAL_FLAG();
#ifdef CMCC_UI_STYLE
        {
            uint8   tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
            uint16  tel_num_len = 0;
            BT_ADDRESS   address = {0};

            tel_num_len = MMIAPICOM_GenDispNumber(s_call_context.want_mo_call.address.number_type, (uint8)s_call_context.want_mo_call.address.num_len, s_call_context.want_mo_call.address.party_num,tel_num,(uint8)sizeof(tel_num));

            if (MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, s_call_context.want_mo_call.dual_sys, TRUE)
                && s_call_context.exist_call.exist_outgoing_call
                && !MMIAPIBT_GetActivatedHS(&address))
            {
                MMICC_OpenEmergency_OptionMenu();
            }
        }
#endif
        break;

    case CC_WAITING_STATE:
        if(1 != MMICC_GetCallNum())
        {
            CC_ReleaseAndAnswerCW();
        }
        break;

    case CC_INCOMING_CALL_STATE:
    {
#ifdef MV_SUPPORT
        //只有第一通电话时才主动打开魔音选择窗口
        if (IsCanUseMagicVoice(TRUE) && !MMK_IsFocusWin(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID)&& (1 == MMICC_GetCallNum()))
         {
            MMICC_OpenMVBeforeCallConnected(NULL);//OpenMagicVoiceSelectWindow(NULL);
#ifdef ASP_SUPPORT
            //stop asp timer to avoid answering call when setting mv
            MMIAPIASP_StopWaitTimer();
#endif
            return FALSE;
        }
#endif
        //accept call
        if(CC_RESULT_ERROR == CC_ConnectCallReq())
        {
            //显示失败
        }
    }
        break;

    default:
        TRACE_CCAPP("call_state = %d",MMICC_GetCurrentCallStatus());
        break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : response call control
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ResponseCallCtl(MN_DUAL_SYS_E dual_sys,
                                                    MMI_STRING_T *alert_str_ptr,
                                                    MMI_STRING_T *display_str_ptr,
                                                    MN_CALLED_NUMBER_T *cc_address,
                                                    SIMAT_SIM_CONTROL_RESULT_E result,
                                                    SIMAT_SIMC_DATA_TYPE_E data_type,
                                                    BOOLEAN is_need_modified)
{
    TRACE_CCAPP("dual_sys=%d, is_need_modified=%d, result=%d", dual_sys, is_need_modified, result);
#ifndef UAL_TELE_SUPPORT
    if (SIMAT_SIMC_ADDRESS == data_type && PNULL != cc_address && cc_address->num_len > 0 && is_need_modified)
    {
        wchar temp_wchar[MMICC_PHONE_NUM_MAX_LEN + 3] = {0};
        uint8 tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
        uint8 tel_num_len = 0;
        uint8 copy_len = 0;
        BOOLEAN b_search_result = FALSE;
        MMI_STRING_T name = {0};
        uint8 pb_group_id = 0;
        uint16 pb_entry_id = 0;
        MMI_STRING_T prompt_str= {0};
        BOOLEAN is_emergency_call = FALSE;

        if (SIMAT_SIMC_ALLOWED_AND_MODIFIED == result)
        {
            MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;

            fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys);

            if (fdn_context_ptr->is_fdn_enable)//fdn enable&modified call not in FDN list
            {
                uint32 i = 0;
                PHONEBOOK_ENTRY_T *ldn_entry_ptr = PNULL;

                ldn_entry_ptr = SCI_ALLOCA(sizeof(PHONEBOOK_ENTRY_T));

                if (PNULL == ldn_entry_ptr)
                {
                    TRACE_CCAPP("MMIAPICC_ResponseCallCtl No Memory");
                    return FALSE;
                }

                for (i = 1; i <= fdn_context_ptr->fdn_info.max_record_num; i++)
                {
                    if (fdn_context_ptr->entry_state[i - 1])
                    {
                        SCI_MEMSET(ldn_entry_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));

                        ldn_entry_ptr->entry_id = i;

                        if (ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadPhonebookEntryEx(dual_sys, PHONEBOOK_FDN_STORAGE, ldn_entry_ptr))
                        {
                            if (ldn_entry_ptr->number_len == cc_address->num_len
                                && ldn_entry_ptr->npi == cc_address->number_plan
                                && ldn_entry_ptr->ton == cc_address->number_type
                                && (0 == memcmp(ldn_entry_ptr->number, cc_address->party_num, ldn_entry_ptr->number_len)))
                            {
                                s_is_call_modified_store = TRUE;

                                break;
                            }
                        }
                    }
                }

                SCI_FREE(ldn_entry_ptr);
            }
        }

        if (s_is_call_modified_store)
        {
            SCI_MEMSET(&s_call_modified_address, 0x00, sizeof(s_call_modified_address));
            SCI_MEMCPY(&s_call_modified_address, &s_call_context.want_mo_call.address, sizeof(s_call_modified_address));
        }

        tel_num_len = MMIAPICOM_GenNetDispNumber(
            cc_address->number_type,
            (uint8)cc_address->num_len,
            cc_address->party_num,
            tel_num,
            sizeof(tel_num)
            );

        s_call_context.want_mo_call.address.number_type = cc_address->number_type ;
        s_call_context.want_mo_call.address.number_plan = cc_address->number_plan;

        SCI_MEMSET((void*)s_call_context.want_mo_call.address.party_num, 0xFF, sizeof(s_call_context.want_mo_call.address.party_num));

        copy_len = ( cc_address->num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: cc_address->num_len;
        MMI_MEMCPY(
            (void*)s_call_context.want_mo_call.address.party_num,
            sizeof(s_call_context.want_mo_call.address.party_num),
            (void*)cc_address->party_num,
            sizeof(cc_address->party_num),
            copy_len
            );
        s_call_context.want_mo_call.address.num_len = copy_len ;

        //调用pb接口获得相应的电话信息
        SCI_MEMSET(s_call_context.want_mo_call.name, 0x00, sizeof(s_call_context.want_mo_call.name));
        name.wstr_ptr = s_call_context.want_mo_call.name;

        is_emergency_call = MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, dual_sys, FALSE);

        if(is_emergency_call)
        {
            MMI_STRING_T clear_str = {0};

            MMI_GetLabelTextByLang(
                TXT_KL_PSW_EMERGENCY_CALL,
                &prompt_str
                );

#ifndef MMI_GUI_STYLE_TYPICAL
            GUILABEL_SetText(MMICC_CONNECTED_STATE_LABEL_CTRL_ID, &clear_str, TRUE);
            GUILABEL_SetText(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, &clear_str, TRUE);
            GUILABEL_SetText(MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, &prompt_str, TRUE);
            GUILABEL_SetText(MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID, &clear_str, TRUE);
#else
            GUILABEL_SetText(MMICC_CALLING_STATE_LABEL_CTRL_ID, &clear_str, TRUE);
            GUILABEL_SetText(MMICC_CALLING_NAME_LABEL_CTRL_ID, &clear_str, TRUE);
            GUILABEL_SetText(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, &prompt_str, TRUE);
            GUILABEL_SetText(MMICC_CALLING_LOCAT_LABEL_CTRL_ID, &clear_str, TRUE);
#endif
        }
        else
        {
            //查找pb
            b_search_result = GetNameFromPb(&s_call_context.want_mo_call, tel_num, tel_num_len, &name, &pb_entry_id, &pb_group_id); 

            //PB中没有该号码
            if( !b_search_result ||
                ( 0 == name.wstr_len) )
            {
                s_call_context.want_mo_call.is_name_exist = FALSE;

                s_call_context.want_mo_call.name_len = tel_num_len;
#ifdef MMI_DUALMODE_ENABLE
                copy_len = tel_num_len;
#else
                copy_len = ( tel_num_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : tel_num_len;
#endif
                MMI_STRNTOWSTR(
                    s_call_context.want_mo_call.name,
                    s_call_context.want_mo_call.name_len,
                    tel_num,
                    tel_num_len,
                    copy_len
                    );

                s_call_context.want_mo_call.name_len = copy_len;
            }
            else
            {
                s_call_context.want_mo_call.is_name_exist = TRUE;

#ifdef MMI_DUALMODE_ENABLE
                copy_len = name.wstr_len;
#else
                copy_len = ( name.wstr_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : name.wstr_len;
#endif

                s_call_context.want_mo_call.name_len = copy_len;
            }

            if (s_call_context.want_mo_call.is_name_exist)
            {
                prompt_str.wstr_ptr = (wchar *)s_call_context.want_mo_call.name;
                prompt_str.wstr_len = s_call_context.want_mo_call.name_len;
            }
            else
            {
                prompt_str.wstr_ptr = PNULL;
                prompt_str.wstr_len = 0;
            }

#ifndef MMI_GUI_STYLE_TYPICAL
            GUILABEL_SetText(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, &prompt_str, TRUE);
#else
            GUILABEL_SetText(MMICC_CALLING_NAME_LABEL_CTRL_ID, &prompt_str, TRUE);
#endif
            SCI_MEMSET(&prompt_str, 0x00, sizeof(prompt_str));

            if (tel_num_len >0)
            {
                prompt_str.wstr_ptr = (wchar *)temp_wchar;
                prompt_str.wstr_len = tel_num_len;

                MMI_STRNTOWSTR(prompt_str.wstr_ptr,
                    prompt_str.wstr_len,
                    (uint8 *)tel_num,
                    tel_num_len,
                    tel_num_len);
            }
            else
            {
                //the number wstr_len is 0.display unknown number info
                MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  &prompt_str);
            }

#ifndef MMI_GUI_STYLE_TYPICAL
            if(s_call_context.want_mo_call.is_name_exist)
            {
                GUILABEL_SetText(MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, &prompt_str, TRUE);
            }
            else
            {
                GUILABEL_SetText(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, &prompt_str, TRUE);
            }
#else
            GUILABEL_SetText(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, &prompt_str, TRUE);
#endif
        }
    }

    if (PNULL != display_str_ptr && display_str_ptr->wstr_len > 0)
    {
        if (SIMAT_SIMC_ADDRESS == data_type)
        {
#ifndef MMI_GUI_STYLE_TYPICAL
            GUILABEL_SetText(MMICC_CONNECTED_STATE_LABEL_CTRL_ID, display_str_ptr, TRUE);
#else
            GUILABEL_SetText(MMICC_CALLING_STATE_LABEL_CTRL_ID, display_str_ptr, TRUE);
#endif
        }
        else if (SIMAT_SIMC_USSD_STRING == data_type && MMK_IsOpenWin(MMIUSSD_SEND_WIN_ID))
        {
            MMIPUB_SetWaitWinTextEx(TRUE, 0, display_str_ptr, MMIUSSD_SEND_WIN_ID);
        }
        else if (SIMAT_SIMC_SS_STRING == data_type && MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID))
        {
            MMIPUB_SetWaitWinTextEx(TRUE, 0, display_str_ptr, MMISET_SET_CONNECT_NETWORK_WIN_ID);
        }
    }

    if (PNULL != alert_str_ptr && alert_str_ptr->wstr_len > 0)
    {
        MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

        MMIPUB_CloseAlertWin();
        MMIPUB_OpenAlertWinByTextPtr(PNULL, alert_str_ptr, PNULL, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);  
    }
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsCurrentCallIDValid(void)
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_ERROR;
    ual_tele_call_data_info_t call_info = {0};

    ret = ual_tele_call_get_current_call_info(&call_info);

    if(UAL_TELE_CALL_RES_SUCCESS != ret)
    {
        TRACE_CCAPP(" get current call info fail");
        return FALSE;
    }
    if(CC_MAX_CALL_NUM > call_info.call_id)
    {
        TRACE_CCAPP(" Current Call ID Valid");
        return TRUE;
    }
    TRACE_CCAPP(" Current Call ID not Valid");
    return FALSE;
}

/*****************************************************************************/
//  Description : Get Calling SIM(Not In call return MMI_DUAL_SYS_MAX)
//  Global resource dependence : none
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_GetCallingSIM(void)
{
    ual_tele_call_data_info_t call_info = {0};

    if (UAL_TELE_CALL_RES_SUCCESS == ual_tele_call_get_current_call_info(&call_info))
    {
#ifdef VT_SUPPORT
//bug339497
        if(MMIAPIVT_IsVtCalling())
        {
            TRACE_CCAPP("MMIAPICC_GetCallingSIM vt calling retrun MN_DUAL_SYS_1");
            return MN_DUAL_SYS_1;
        }
#endif
        return call_info.sim_id;
    }

    return MMI_DUAL_SYS_MAX;
}

/*****************************************************************************/
//  Description : 909 problem, call disconnect msg is coming, but the voice  is being
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleDownlinkChanel(void)
{
#ifndef UAL_TELE_SUPPORT
    if(!MMICC_IsRemoteMute())
    {
        if(MMICC_IsExistHoldCall() && !MMICC_IsExistActiveCall())
        {
            MMISRVAUD_SetMuteNohandle(TRUE);
        }
        else
        {
            MMISRVAUD_SetMuteNohandle(FALSE);
        }
    }
#endif
}

/*****************************************************************************/
//  Description : get current existed  hod call count
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetHoldCallCount(void)
{
    uint8 all_call_count = 0;
    uint8 hold_call_count = 0;
    uint8 i = 0;

    all_call_count = ual_tele_call_get_call_count();
    for(i = 0;i < all_call_count;i++)
    {
        if(UAL_TELE_CALL_STATE_HOLD == ual_tele_call_get_call_state(s_call_info[i].call_id))
        {
            hold_call_count++;
        }
    }
    TRACE_CCAPP("hold_call_count = %d",hold_call_count);
    return hold_call_count;
}

/*****************************************************************************/
//  Description : get current existed  active call count
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetActiveCallCount(void)
{
    uint8 all_call_count = 0;
    uint8 active_call_count = 0;
    uint8 i = 0;

    all_call_count = ual_tele_call_get_call_count();
    for(i = 0;i < all_call_count;i++)
    {
        if(UAL_TELE_CALL_STATE_ACTIVE == ual_tele_call_get_call_state(s_call_info[i].call_id))
        {
            active_call_count++;
        }
    }
    TRACE_CCAPP("active_call_count = %d",active_call_count);
    return active_call_count;
}

/*****************************************************************************/
//  Description : get current existed  mpty call count
//  Global resource dependence : none
//  Author: taoxue
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetMPTYCallCount(void)
{
#ifndef UAL_TELE_SUPPORT
    if (1 < MMIAPICC_GetActiveCallCount())
    {
        return s_call_context.exist_call.exist_active_call_num;
    }
    else if (1 < MMIAPICC_GetHoldCallCount())
    {
        return s_call_context.exist_call.exist_hold_call_num;
    }
    else
#endif
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : get current call string
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_GetCurrentCallString(
                                          MMI_STRING_T *str_ptr
                                          )
{
#ifndef UAL_TELE_SUPPORT
    if (PNULL != str_ptr)
    {
        if(MMICC_IsCallIndexInMPTY(s_call_context.current_call))
        {
            MMI_GetLabelTextByLang(TXT_CC_MULTICALL, str_ptr);
        }
        else
        {
            str_ptr->wstr_ptr = s_call_context.call_info[s_call_context.current_call].name;
            str_ptr->wstr_len = s_call_context.call_info[s_call_context.current_call].name_len;
        }
    }
#endif
}

/*****************************************************************************/
//  Description : judge whether allowed to send dtmf when in outgoing or incoming call
//  Global resource dependence :
//  Author : samboo.shen
//  Note: not for connected state
/*****************************************************************************/
PUBLIC void MMICC_ClearWantCallInfo(void)
{
#ifndef UAL_TELE_SUPPORT
    ClearWantCallInfo();
#endif
}

/*****************************************************************************/
//  Description : judge whether allowed to send dtmf when in outgoing or incoming call
//  Global resource dependence :
//  Author : samboo.shen
//  Note: not for connected state
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsAllowSendDtmf(void)
{
    BOOLEAN is_allowed = FALSE;
#ifndef UAL_TELE_SUPPORT
    if(CC_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state)
    {
        is_allowed = TRUE;
    }
    else if((CC_CALLING_STATE == s_call_context.call_info[s_call_context.current_call].call_state)
        ||(CC_INCOMING_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state)
        ||(CC_WAITING_STATE == s_call_context.call_info[s_call_context.current_call].call_state))
    {
        is_allowed = ((DTMF_MN_SYNC_IND_BIT | DTMF_MN_PROGRESS_IND_BIT) == s_call_context.dtmf_state.allow_send_flag);
    }
    else
    {
        is_allowed = FALSE;
    }
#endif
    return is_allowed;
}

/*****************************************************************************/
//  Description : judge whether allowed to add new call
//  Global resource dependence :
//  Author : tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsAllowAddNewCall(void)
{
#ifndef UAL_TELE_SUPPORT
    BOOLEAN is_allowed = TRUE;
    if ((MMICC_IsExistOutgoingCall()
        ||CC_CALLING_STATE == s_call_context.call_info[s_call_context.current_call].call_state
        ||CC_INCOMING_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state
        ||CC_WAITING_STATE == s_call_context.call_info[s_call_context.current_call].call_state
        ||MMICC_IsExistActiveAndHoldCalls())
        ||MMIAPIDM_GetHandsetLock())
    {
        is_allowed = FALSE;
    }
    return is_allowed;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : judge whether allowed to send dtmf when in outgoing or incoming call
//  Global resource dependence :
//  Author : samboo.shen
//  Note: not for connected state
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsSendingStoredDtmf(void)
{
#ifndef UAL_TELE_SUPPORT
    return s_call_context.dtmf_state.is_dtmf;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : get call num
//  Global resource dependence :
//  Author : samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMICC_GetCallNum(void)
{
    return ual_tele_call_get_call_count();
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMICC_GetCurrentCallIndex(void)
{
    uint8 i = 0;
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_ERROR;
    ual_tele_call_data_info_t call_info = {0};

    ret = ual_tele_call_get_current_call_info(&call_info);

    if(UAL_TELE_CALL_RES_SUCCESS == ret)
    {
        for(i = 0;i < CC_MAX_CALL_NUM;i++)
        {
            if(call_info.call_id == s_call_info[i].call_id)
            {
                TRACE_CCAPP("i = %d",i);
                return i;
            }
        }
    }
    TRACE_CCAPP("ual_tele_call_get_current_call_info fail");
    return 0;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note: num_str_ptr must be allocated or be fixed length array,
//   and the size must be more than MMICC_PHONE_NUM_MAX_LEN + 2
//   num_str_ptr & num_len_ptr [OUT]
/*****************************************************************************/
PUBLIC void MMICC_GetWantCallNumStr(uint8* num_str_ptr, uint8* num_len_ptr)
{
    if(PNULL != num_str_ptr && PNULL != num_len_ptr)
    {
    SCI_MEMCPY(num_str_ptr,s_want_mo_call_info.tele_num,MIN(s_want_mo_call_info.tele_num_len,CC_MAX_TELE_NUM_LEN));
    *num_len_ptr = s_want_mo_call_info.tele_num_len;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note: return value means if the call specified str exists
//   spec_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetWantCallSpecifiedStr(MMI_STRING_T* spec_str_ptr)
{
    BOOLEAN result = FALSE;
#ifndef UAL_TELE_SUPPORT
    if (s_call_context.want_mo_call.is_exist_specified_str && s_call_context.want_mo_call.specified_str_len > 0) //显示特定字符串
    {
        spec_str_ptr->wstr_len = s_call_context.want_mo_call.specified_str_len;
        spec_str_ptr->wstr_ptr = s_call_context.want_mo_call.specified_str;
        result = TRUE;
    }
#endif
    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note: return value means if the call name exists
//   name_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetWantCallNameStr(MMI_STRING_T* name_str_ptr)
{
    BOOLEAN is_name_exist = FALSE;
#ifndef UAL_TELE_SUPPORT
    if(s_call_context.want_mo_call.is_name_exist && s_call_context.want_mo_call.name_len > 0)
    {
        is_name_exist = TRUE;
        if(PNULL != name_str_ptr)
        {
            name_str_ptr->wstr_ptr = s_call_context.want_mo_call.name;
            name_str_ptr->wstr_len = s_call_context.want_mo_call.name_len;
        }
    }
    else
    {
        is_name_exist = FALSE;
    }
#endif
    return is_name_exist;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsWantCallExist(void)
{
#ifndef UAL_TELE_SUPPORT
    return s_call_context.want_mo_call.is_want_mo_call;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMICC_GetWantCallDualSys(void)
{
#ifndef UAL_TELE_SUPPORT
    return s_call_context.want_mo_call.dual_sys;
#else
    return MN_DUAL_SYS_1;
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note: num_str_ptr must be allocated or be fixed length array,
//   and the size must be more than MMICC_PHONE_NUM_MAX_LEN + 2
//   num_str_ptr & num_len_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetCallNumStrByIndex(uint8* num_str_ptr, uint8* num_len_ptr, uint8 call_index)
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_ERROR;
    ual_tele_call_data_info_t call_info = {0};

    if((PNULL == num_str_ptr) || (PNULL == num_len_ptr))
    {
        TRACE_CCAPP(" num_str_ptr or num_len_ptr is PNULL");
        return FALSE;
    }

    ret = ual_tele_call_get_call_info(s_call_info[call_index].call_id,&call_info);
    if(UAL_TELE_CALL_RES_SUCCESS != ret)
    {
        TRACE_CCAPP(" get call info fail");
        return FALSE;
    }
    *num_len_ptr = MIN(call_info.tele_num_len,CC_MAX_TELE_NUM_LEN);
    SCI_MEMCPY(num_str_ptr,call_info.tele_num,MIN(call_info.tele_num_len,CC_MAX_TELE_NUM_LEN))
    TRACE_CCAPP(" num_str_ptr = %s, num_len_ptr = %d",num_str_ptr,num_len_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_BCD_NUMBER_T MMICC_GetPbNumByIndex(uint8 call_index)
{
    MMIPB_BCD_NUMBER_T pb_num = {0};
#ifndef UAL_TELE_SUPPORT
    uint16 copy_len = 0;

    pb_num.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(s_call_context.call_info[call_index].address.number_type, s_call_context.call_info[call_index].address.number_plan );
    copy_len = ( s_call_context.call_info[call_index].address.num_len > MMIPB_BCD_NUMBER_MAX_LEN ) ? MMIPB_BCD_NUMBER_MAX_LEN : s_call_context.call_info[call_index].address.num_len;
    pb_num.number_len = copy_len;
    MMI_MEMCPY(
        pb_num.number,
        sizeof(pb_num.number),
        s_call_context.call_info[call_index].address.party_num,
        sizeof(s_call_context.call_info[call_index].address.party_num),
        copy_len
        );
#endif
    return pb_num;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:samboo.shen
//  Note: return value means if the call name exists
//   name_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetCallNameStrByIndex(MMI_STRING_T* name_str_ptr, uint8 call_index)
{
    BOOLEAN is_name_exist = FALSE;
#ifndef UAL_TELE_SUPPORT
    if(!CC_IsCallIDValid(call_index))
    {
        return FALSE;
    }

    if(s_call_context.call_info[call_index].is_name_exist && s_call_context.call_info[call_index].name_len > 0)
    {
        is_name_exist = TRUE;
        if(PNULL != name_str_ptr)
        {

            name_str_ptr->wstr_ptr = s_call_context.call_info[call_index].name;
            name_str_ptr->wstr_len = s_call_context.call_info[call_index].name_len;
    }
    }
    else
    {
        is_name_exist = FALSE;
    }
#endif
    return is_name_exist;
}

/*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_CALL_STATE_E MMICC_GetCallStatusByIndex(uint8 call_index)
{
    if(CC_MAX_CALL_NUM <= call_index)
    {
        return CC_NULL_STATE;
    }
    else
    {
        ual_tele_call_state_e ual_call_state = UAL_TELE_CALL_STATE_MAX;
        CC_CALL_STATE_E ret_call_state = CC_CALL_STATE_MAX;

        ual_call_state = ual_tele_call_get_call_state(s_call_info[call_index].call_id);
        switch(ual_call_state)
        {
            case UAL_TELE_CALL_STATE_NULL:
                TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_NULL");
                ret_call_state = CC_NULL_STATE;
                break;

            case UAL_TELE_CALL_STATE_OUTGOING:
                TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_OUTGOING");
                ret_call_state = CC_CALLING_STATE;
                break;

            case UAL_TELE_CALL_STATE_INCOMING:
            {
                uint8 count = 0;
                TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_INCOMING");
                count = ual_tele_call_get_call_count();
                if(1 == count)
                {
                    TRACE_CCAPP("count is 1");
                    ret_call_state = CC_INCOMING_CALL_STATE;
                }
                else
                {
                    ret_call_state = CC_WAITING_STATE;
                }
                break;
            }
            case UAL_TELE_CALL_STATE_ACTIVE:
                TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_ACTIVE");
                ret_call_state = CC_CALL_STATE;
                break;

            case UAL_TELE_CALL_STATE_HOLD:
                TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_HOLD");
                ret_call_state = CC_HOLD_STATE;
                break;

            default:
                TRACE_CCAPP("call status is default");
                ret_call_state = CC_CALL_STATE_MAX;
                break;
        }
        return ret_call_state;
    }
}

/*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_CALL_STATE_E MMICC_GetCurrentCallStatus(void)
{
    ual_tele_call_state_e ual_call_state = UAL_TELE_CALL_STATE_MAX;
    CC_CALL_STATE_E ret_call_state = CC_CALL_STATE_MAX;

    ual_call_state = ual_tele_call_get_current_call_state();
    switch(ual_call_state)
    {
        case UAL_TELE_CALL_STATE_NULL:
            TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_NULL");
            ret_call_state = CC_NULL_STATE;
            break;

        case UAL_TELE_CALL_STATE_OUTGOING:
            TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_OUTGOING");
            ret_call_state = CC_CALLING_STATE;
            break;

        case UAL_TELE_CALL_STATE_INCOMING:
        {
            uint8 count = 0;
            TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_INCOMING");
            count = ual_tele_call_get_call_count();
            if(1 == count)
            {
                TRACE_CCAPP("count is 1");
                ret_call_state = CC_INCOMING_CALL_STATE;
            }
            else
            {
                ret_call_state = CC_WAITING_STATE;
            }
            break;
        }
        case UAL_TELE_CALL_STATE_ACTIVE:
            TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_ACTIVE");
            ret_call_state = CC_CALL_STATE;
            break;

        case UAL_TELE_CALL_STATE_HOLD:
            TRACE_CCAPP("call status is UAL_TELE_CALL_STATE_HOLD");
            ret_call_state = CC_HOLD_STATE;
            break;

        default:
            TRACE_CCAPP("call status is default");
            ret_call_state = CC_CALL_STATE_MAX;
            break;
    }
    return ret_call_state;
}

/*****************************************************************************/
//  Description : get call operation
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_OPERATED_BY_USER_E MMICC_GetCallOperatedByIndex(uint8 call_index)
{
#ifndef UAL_TELE_SUPPORT
    if(CC_IsCallIDValid(call_index))
    {
        return s_call_context.call_info[call_index].operated_by_user;
    }
    else
    {
        return CC_NO_OPER;
    }
#else
        return CC_NO_OPER;
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsCallIndexInMPTY(uint8 call_index)
{
#ifndef UAL_TELE_SUPPORT
    if(CC_IsCallIDValid(call_index))
    {
        return s_call_context.call_info[call_index].is_in_mpty;
    }
    else
    {
        return FALSE;
    }
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMICC_GetCallDirection(uint8 call_index)
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_ERROR;
    ual_tele_call_data_info_t call_info = {0};
    ret = ual_tele_call_get_call_info(s_call_info[call_index].call_id,&call_info);
    if(UAL_TELE_CALL_RES_SUCCESS != ret)
    {
        TRACE_CCAPP(" get call info fail");
        return 0;
    }
    TRACE_CCAPP(" call_info.call_direction = %d",call_info.call_direction);
    return call_info.call_direction;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_DTMF_STATE_T* MMICC_GetStoredDtmfInfo(void)
{
#ifndef UAL_TELE_SUPPORT
    return &s_call_context.dtmf_state;
#else
    return PNULL;
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMICC_GetCallTimeCount(void)
{
#ifndef UAL_TELE_SUPPORT
    return s_call_context.call_time_count;
#else
    return 0;
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_SetCallTimeCount(uint32 count)
{
#ifndef UAL_TELE_SUPPORT
     s_call_context.call_time_count = count;
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetIsNewMissedCall(void)
{
    // TODO:
#ifndef UAL_TELE_SUPPORT
    //return s_is_new_missed_call;
#endif
    return FALSE;//cid607300
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_SetIsNewMissedCall(BOOLEAN is_new_missed_call)
{
    // TODO:
#ifndef UAL_TELE_SUPPORT
     s_is_new_missed_call = is_new_missed_call;
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : s_call_context
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsCurrentCallMute(void)
{
    BOOLEAN is_mute = FALSE;

    is_mute = ual_tele_call_get_mic_mute();
    TRACE_CCAPP(" IsCurrentCallMute: %d",is_mute);
    return is_mute;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : s_call_context
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_SetCurrentCallMute(BOOLEAN is_mute)
{
    BOOLEAN result = FALSE;
    TRACE_CCAPP(" Is Mute: %d",is_mute);

    result = ual_tele_call_set_mic_mute(is_mute);

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : s_calllog_type
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMICL_CALL_TYPE_E MMICC_GetCallLogType(void)
{
    TRACE_CCAPP(" s_calllog_type = %d", s_calllog_type);
    return s_calllog_type;
}

/*****************************************************************************/
//  Description : is existing unciphering call
//  Global resource dependence :
//  Author:tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsExistUncipheringCall(void)
{
    BOOLEAN is_exist_unciphering_call = FALSE;
// TODO:获取通话是否为未加密的
    return is_exist_unciphering_call;
}

#ifndef SIM_PLUG_IN_SUPPORT //bug 184713
/*****************************************************************************/
//  Description : if sim removed,exit mo call
//  Global resource dependence : none
//  Author:chunjie liu
//  Note:cucc support conctrol,befor mo call request,make a request to sim
/*****************************************************************************/
PUBLIC void MMIAPICC_ExitMoCall(MN_DUAL_SYS_E dual_sys)
{
    MN_DUAL_SYS_E calling_sys = MMIAPICC_GetCallingSIM();

   if(MMI_DUAL_SYS_MAX != calling_sys
        && calling_sys == dual_sys)
   {
#ifndef UAL_TELE_SUPPORT
        if(MMK_IsOpenWin( MMICC_STATUS_WIN_ID ))
        {
            MMK_CloseWin(MMICC_STATUS_WIN_ID);
        }
        s_dial_method = CC_DIAL_METHOD_NULL;
        s_call_context.exist_call.exist_outgoing_call = FALSE;
        CC_ClearStoredDtmf();
        //clear s_call_context.want_mo_call
        SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, sizeof(s_call_context.want_mo_call));
        s_call_context.want_mo_call.is_want_mo_call = FALSE;
        if(PNULL != MMICC_GetAppletInstance() )
        {
            if(MMICC_GetAppletInstance()->cc_instance.is_original_phone_lock)
            {
                MMICC_GetAppletInstance()->cc_instance.is_original_phone_lock = FALSE;
            }

            //stop cc applet
            MMICC_DestroyAudioHandle();
            MMK_PostMsg( MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
        }
#endif
        //状态栏显示通话状态图标
        MAIN_SetCallingState(FALSE);
   }
}
#endif

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description : CC_GetCurrentDev
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E CC_GetCurrentDev(void)
{
    MMIFILE_DEVICE_E    fileDev = MMI_DEVICE_NUM;
    wchar*              filePathPtr = MMIRECORDSRV_GetRecordFilePath(s_cc_watch_record_handle);

    if(PNULL != filePathPtr)
    {
        fileDev = MMIAPIFMM_GetDeviceTypeByPath(filePathPtr, MMIFILE_DEVICE_LEN);
    }

    if(MMI_DEVICE_NUM <= fileDev)
    {
        fileDev= MMIAPIRECORD_GetRecordStorage();
    }

    TRACE_CCAPP("[CC_GetCurrentDev] file_dev = %d",fileDev);
    return fileDev;
}

/*****************************************************************************/
//  Description : EventNotifyCb_RecordInCall
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void EventNotifyCb_RecordInCall(RECORD_SRV_HANDLE record_srv_handle,
                                                MMIRECORD_SRV_SUPPORT_TYPE_E support_type,
                                                MMIRECORD_SRV_RESULT_E result )
{
    TRACE_CCAPP("[EventNotifyCb_RecordInCall] record_srv_handle = %d, s_cc_watch_record_handle = %d, result = %d, support_type = %d",
                                                     record_srv_handle, s_cc_watch_record_handle, result, support_type);

    if (record_srv_handle == s_cc_watch_record_handle
    && MMIRECORD_SRV_SUPPORT_RECORD == support_type)
    {
        if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED ,CC_GetCurrentDev(), MMIRECORD_SRV_MIN_NEED_SPACE, PNULL))
        {
            result = MMIRECORD_SRV_RESULT_NO_SPACE;
        }
        switch (result)
        {
            case MMIRECORD_SRV_RESULT_SUCCESS:
                break;
            case MMIRECORD_SRV_RESULT_NO_SPACE:
                break;
            case MMIRECORD_SRV_RESULT_FOLDER_FULL:
                break;
            default:
            {
                MMIRECORDSRV_DeleteRecordFile(s_cc_watch_record_handle,TRUE);
                break;
            }
        }
        if(MMIRECORD_SRV_RESULT_SUCCESS != result)
        {
            MMICC_StopRecordInCall();
        }
    }
}

/*****************************************************************************/
//  Description : start record in call
//  Parameter: [In] None
//             [Out] None
//             [Return] Result
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMICC_StartRecordInCall(void)
{
    MMIRECORD_SRV_RECORD_PARAM_T    recordParam = {0};
    MMIFILE_DEVICE_E                recordDev = MMIAPIRECORD_GetRecordStorage();
    MMI_STRING_T                    nameStr = {0};

    wchar         telname[CC_MAX_PBNAME_LEN + 1]   = {0};
    wchar         name[CC_MAX_PBNAME_LEN + 1]   = {0};
    uint8         cur_call_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8         tel_num_len = CC_MAX_TELE_NUM_LEN + 2;

    if (PNULL != s_cc_watch_record_handle)
    {
        return MMIRECORD_SRV_RESULT_ERROR;
    }

    s_cc_watch_record_handle = MMIRECORDSRV_RequestRecordHandle(EventNotifyCb_RecordInCall);
    if (PNULL == s_cc_watch_record_handle)
    {
        return MMIRECORD_SRV_RESULT_ERROR;
    }

    MMIAPICC_GetCallName(&nameStr);
    tel_num_len = MMIAPICC_GetCallNumber(cur_call_num, tel_num_len);
    if(MMIAPIPB_IsUseDefaultName(nameStr.wstr_ptr)||!MMIAPICOM_FileNameIsValid(&nameStr))
    {
        MMIAPICOM_StrToWstr(cur_call_num, telname);
        if(MMIAPICOM_GetValidFileName(FALSE, telname, tel_num_len, name, CC_MAX_PBNAME_LEN))
        {
            nameStr.wstr_ptr = name;
            nameStr.wstr_len = MMIAPICOM_Wstrlen(name);
        }
    }
    recordParam.fmt = MMISRVAUD_RECORD_FMT_ADPCM;//MMIAPIRECORD_GetRecordFmt(); //bug1529219
    recordParam.is_create_file_name = TRUE;
    recordParam.prefix_ptr = PNULL;
    recordParam.record_dev_ptr = &recordDev;
    recordParam.record_file_id_ptr = PNULL;
    recordParam.record_name_ptr = &nameStr;
    recordParam.source = MMISRVAUD_RECORD_SOURCE_CALL;
    recordParam.record_param_ptr = PNULL;
    recordParam.volume = MMIAPISET_GetMultimVolume();
    return MMIRECORDSRV_StartRecord(s_cc_watch_record_handle,recordParam);
}

/*****************************************************************************/
//  Description : stop record in call
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_StopRecordInCall(void)
{
    if (PNULL != s_cc_watch_record_handle)
    {
        MMIRECORDSRV_StopRecord(s_cc_watch_record_handle);
        MMIRECORDSRV_FreeRecordHandle(s_cc_watch_record_handle);
        s_cc_watch_record_handle = 0;
    }
}

/*****************************************************************************/
//  Description : if is in recording
//  Parameter: [In] None
//             [Out] None
//             [Return] true or false
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsRecording(void)
{
    if (PNULL != s_cc_watch_record_handle)
    {
        return TRUE;
    }
    return FALSE;
}

#endif

PUBLIC MMI_RESULT_E MMICC_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    if (0 == adap_handle_call_ps_msg(msg_id, param))
    {
        return MMI_RESULT_TRUE;
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}

/*****************************************************************************/
//  Description : not send dtmf
//  Global resource dependence :
//  Author:mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_CancelStringDtmf(void)
{
    ual_tele_call_result_e ret = UAL_TELE_CALL_RES_ERROR;
    ret = ual_tele_call_cancel_string_dtmf(s_dtmf_info.sim_id);
    SCI_MEMSET(&s_dtmf_info,0X00,sizeof(ual_tele_call_dtmf_info_t));
    return CC_RESULT_SUCCESS;//cid609705
}
/*****************************************************************************/
//  Description : send  dtmf
//  Global resource dependence :
//  Author:mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SendStringDtmf(void)
{
    CC_RESULT_E ret = CC_RESULT_SUCCESS;
    ret = MMICC_SendDTMFStr(s_dtmf_info.dtmf_str, s_dtmf_info.dtmf_len);
    SCI_MEMSET(&s_dtmf_info,0X00,sizeof(ual_tele_call_dtmf_info_t));
    return (ret == CC_RESULT_SUCCESS ? TRUE : FALSE);//cid601830
}
/*****************************************************************************/
// Description : the process message function of the CC application
// Global resource dependence :
// Author:figo.feng
// Note:2003.12.01
/*****************************************************************************/
LOCAL BOOLEAN CC_HandleMsgCallback(ual_cms_msg_t param)
{
#ifdef VT_SUPPORT
    if ((MMIAPIVT_IsVTIncomingMsg(msg_id, param)||MMIAPIVT_IsVtCalling()))
    {
        return FALSE;
    }
#else//不支持VT的时候，来VT电话直接挂断
#if 0
    if (HandleIncomingMsgWhenVTNotSupport(msg_id, param))
    {
        return MMI_RESULT_TRUE;
    }
#endif
#endif

    //check the param
    TRACE_CCAPP("msg_id = %d", param.msg_id);

    if(PNULL== param.p_body)
    {
        TRACE_CCAPP("param == PNULL");
        return FALSE;
    }
    // TODO:FilterMsgFromATCmdCall, 过滤AT指令

    switch( param.msg_id )
    {
        case MSG_UAL_TELE_CALL_MO_START_IND:
        {
            ual_tele_call_mo_start_ind_t *p_mo_start_ind = (ual_tele_call_mo_start_ind_t*)param.p_body;
            TRACE_CCAPP("MMICC_HandlePsMsg,receive MSG_UAL_TELE_CALL_MO_START_IND");
            //收到协议栈已经受理呼叫发起请求的消息
            CC_MoStartInd(p_mo_start_ind);
            break;
        }
        case MSG_UAL_TELE_CALL_MO_ALERT_IND:
        {
            ual_tele_call_mo_alert_ind_t *p_mo_alert_ind = (ual_tele_call_mo_alert_ind_t*)param.p_body;
            TRACE_CCAPP("receive MSG_UAL_TELE_CALL_MO_ALERT_IND");
            //收到振铃提示事件的处理
            CC_MoAlertingInd(p_mo_alert_ind);
            break;
        }
        case MSG_UAL_TELE_CALL_MO_CONNECTED_CNF:
        {
            ual_tele_call_mo_connected_cnf_t *p_mo_connected_ind = (ual_tele_call_mo_connected_cnf_t*)param.p_body;
            TRACE_CCAPP("receive MSG_UAL_TELE_CALL_MO_CONNECTED_CNF");
            //收到建立连接完成事件（MO）的处理
            CC_MoConnectedCnf(p_mo_connected_ind);
#ifdef MMI_STK_SUPPORT
            MMIAPISTK_SetupCall(p_mo_connected_ind->sim_id, TRUE, TRUE, 0);
#endif
            break;
        }
        case MSG_UAL_TELE_CALL_MT_CONNECTED_CNF:
        {
            ual_tele_call_mt_connected_cnf_t *p_mt_connected_ind = (ual_tele_call_mt_connected_cnf_t*)param.p_body;
            TRACE_CCAPP("receive MSG_UAL_TELE_CALL_MT_CONNECTED_CNF");
            CC_MtConnectedCnf(p_mt_connected_ind);
            // TODO:使用MET_MEX_SUPPORT控制，调用MMIMEX_PhsOutCallConnectedCB
            break;
        }
        case MSG_UAL_TELE_CALL_DISCONNECTED_CNF:
        case MSG_UAL_TELE_CALL_DISCONNECTED_IND:
        {

            if(!HandleMTFWDisconnected(param.msg_id, param.p_body))        //收到CALL中断事件的处理
            {
                //不是MTFW disconnected
                CC_DisconnectedCall(param.msg_id, param.p_body);            }
            break;
        }
        case MSG_UAL_TELE_CALL_MT_START_IND:
        {
            ual_tele_call_mt_start_ind_t *p_mt_start_ind = (ual_tele_call_mt_start_ind_t*)param.p_body;
            TRACE_CCAPP("receive MSG_UAL_TELE_CALL_MT_START_IND");
#ifdef AVOID_MTC_DC_DISTURB_SUPPORT
            MMIPUB_OpenAlertWarningWin(TXT_CC_SYSTEM_RUNNING); //专门处理来电dc绿条纹问题
#endif
            //MT call come in, so exit TV-out
            MMIAPITVOUT_Close();
            CC_MtStartInd(p_mt_start_ind);
#ifdef MMI_VOLTE_SUPPORT
            if(p_mt_start_ind->is_ps_call)
            {
                //update call UI
                MMICC_UpdateCallStatusDisplay(MMICC_VOLTE_CALL_IND);            }
#endif
            break;
        }
        case MSG_UAL_TELE_CALL_HOLD_CNF:
        {
            ual_tele_call_hold_cnf_t *p_hold_cnf = (ual_tele_call_hold_cnf_t*)param.p_body;
            TRACE_CCAPP("receive MSG_UAL_TELE_CALL_HOLD_CNF");
            //receive the cofirm about hold call
            CC_HoldCnf(p_hold_cnf);
            break;
        }
        case MSG_UAL_TELE_CALL_UNHOLD_CNF:
        {
            ual_tele_call_unhold_cnf_t *p_unhold_cnf = (ual_tele_call_unhold_cnf_t*)param.p_body;
            TRACE_CCAPP("receive MSG_UAL_TELE_CALL_MT_START_IND");
            CC_UnholdCnf(p_unhold_cnf);
            break;
        }
        case MSG_UAL_TELE_CALL_QUERY_DTMF_IND:
        {
            ual_tele_call_query_dtmf_ind_t *p_query_dtmf_ind = (ual_tele_call_query_dtmf_ind_t*)param.p_body;
            TRACE_CCAPP("receive MSG_UAL_TELE_CALL_QUERY_DTMF_IND");
            SCI_MEMSET(&s_dtmf_info,0X00,sizeof(ual_tele_call_dtmf_info_t));
            SCI_MEMCPY(&s_dtmf_info,&(p_query_dtmf_ind->dtmf_info),sizeof(ual_tele_call_dtmf_info_t));
            WatchCC_SendDtmf_QueryWin_Enter(s_dtmf_info.dtmf_str,s_dtmf_info.dtmf_len);
            break;
        }
        case MSG_UAL_TELE_CALL_SEND_DTMF_CNF:
        {
            TRACE_CCAPP("receive MSG_UAL_TELE_CALL_SEND_DTMF_CNF");
            break;
        }
        case MSG_UAL_TELE_CALL_START_DTMF_IND:
        {
            ual_tele_call_start_dtmf_ind_t *p_start_dtmf_ind = (ual_tele_call_start_dtmf_ind_t*)param.p_body;
            TRACE_CCAPP("receive MSG_UAL_TELE_CALL_START_DTMF_IND");
            WatchCC_SendDtmf_NoteWin_Enter(p_start_dtmf_ind->dtmf_info.dtmf_str,p_start_dtmf_ind->dtmf_info.dtmf_len);
            break;
        }
        default:
            break;
    }
    return TRUE;//cid609704
}

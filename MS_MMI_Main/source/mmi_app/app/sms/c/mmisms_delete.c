/****************************************************************************
** File Name:      mmisms_delete.c                                          *
** Author:                                                                  *
** Date:           8/23/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 08/2007        liming.deng      Create                                   *
**                                                                          *
****************************************************************************/
/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_delete.h"
#include "mmisms_app.h"
#include "mmisms_text.h"
#include "mmisms_id.h"
#include "mmi_text.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmiudisk_export.h"
#include "mmiota_export.h"
#include "mmimms_export.h"
#include "mmiidle_export.h"
#include "mmialarm_export.h"
#include "mmiota_id.h"
#include "mmisms_save.h"
#include "mmisms_send.h"
#include "mmiidle_statusbar.h"
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#include "mail_fldr.h"
#include "mail_export.h"
#include "mmisms_order.h"
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmisms_appcontrol.h"
#include "mmismsapp_interface.h"
#ifdef DROPDOWN_NOTIFY_SHORTCUT
#include "watch_sms_notifyconnect.h"
#endif
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;      // SMS的全局变量
#ifdef MMI_PDA_SUPPORT
extern MMISMS_DETAIL_MSGLOG_DATA_T *g_sms_match_data_ptr;
extern uint16 g_match_sms_delete_num;
#endif
/**-------------------------------------------------------------------------*
**                         FUNCTION DECLARE                                 *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : to handle the signal APP_MN_UPDATE_SMS_STATUS_CNF
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: handle the normal delete operation
/*****************************************************************************/
LOCAL void HandleDeleteCnf( 
                           MN_SMS_CAUSE_E    cause,    //IN:
                           MN_SMS_RECORD_ID_T record_id,   // the SMS position 
                           MN_SMS_STORAGE_E   storage,     // where the short message is saved
                           MN_DUAL_SYS_E      dual_sys                           
                           );

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//     Description : to handle the signal APP_MN_UPDATE_SMS_STATUS_CNF
//    Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//    Note: handle the normal move operation
/*****************************************************************************/
LOCAL void HandleMoveDeleteCnf( 
                           MN_SMS_CAUSE_E    cause    //IN:
                           );
#endif


/**-------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : to handle the signal APP_MN_UPDATE_SMS_STATUS_CNF
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: handle the normal delete operation
/*****************************************************************************/
LOCAL void HandleDeleteCnf( 
                           MN_SMS_CAUSE_E    cause,    //IN:
                           MN_SMS_RECORD_ID_T record_id,   // the SMS position 
                           MN_SMS_STORAGE_E   storage,     // where the short message is saved
                           MN_DUAL_SYS_E      dual_sys                           
                           )
{
    //MN_SMS_RECORD_ID_T record_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    BOOLEAN ret = FALSE;

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMI_STRING_T prompt_str = {0};
    char handle_number[HANDLE_LEN] = {0};
    uint8 is_extra_msg = 0;
#endif

    //SCI_TRACE_LOW:"MMISMS: HandleDeleteCnf cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_85_112_2_18_2_52_34_21,(uint8*)"d", cause);
//============== MET MEX  start ===================
#ifdef MET_MEX_SUPPORT
    if(MMIMEX_DelSmsRsp(0,cause))
    {
        //process started by mex application, should not go on.            
        return;       
    }
#endif // MET_MEX_SUPPORT
//============== MET MEX  end ===================
    
    if (MN_SMS_OPERATE_SUCCESS == cause)
    {
        cur_order_id = MMISMS_GetCurOperationOrderId();
#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_DeleteTimerMsg(cur_order_id);
#endif
        // delete the message from order list
        record_id = g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index]->record_id;
        SCI_TRACE_LOW("[MMISMS] folder_type = %d, record_id = %d", g_mmisms_global.folder_type, record_id);

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)    
        if(g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index]->flag.folder_type == MMISMS_FOLDER_BT)
        {
            
            MMISMS_GetHandleInfoByRecordId(record_id, handle_number, &is_extra_msg);
            SCI_TRACE_LOW("[MMISMS] HandleDeleteCnf record_id = %d, handle_number = %s", record_id, handle_number);
            SCI_TRACE_LOW("[MMISMS] HandleDeleteCnf MMIAPIBT_IsConnectDevice() = %d, MMIAPIBT_MapIsConnected() = %d", MMIAPIBT_IsConnectDevice(), MMIAPIBT_MapIsConnected());

#ifndef WIN32
            if(TRUE == MMIAPIBT_IsConnectDevice() && MMIAPIBT_MapIsConnected()
                       && (PNULL != handle_number[0]))
            {
                if (((TRUE == MMIAPISMS_GetIsUpdateInbox()) && ( MMISMS_BT_BOX_MT == MMISMS_GetCurBoxType()) )
					|| ((TRUE == MMIAPISMS_GetIsUpdateSendbox())  && ( MMISMS_BT_BOX_SENDSUCC == MMISMS_GetCurBoxType()) )
					|| ((TRUE == MMIAPISMS_GetIsUpdateOutbox())  && ( MMISMS_BT_BOX_SENDFAIL == MMISMS_GetCurBoxType()) )
					|| ((TRUE == MMIAPISMS_GetIsUpdateDraft())   && ( MMISMS_BT_BOX_NOSEND == MMISMS_GetCurBoxType()) ) )
				{
					MMIAPISMS_HandleMsgDeleteStatus();
					return;
				}
				else
				{
                    SCI_TRACE_LOW("[MMISMS] HandleDeleteCnf BT_MapSetMessageStatus BT_MAP_SET_MSG_STATUS_DELETED");
					g_mmisms_global.folder_type = MMISMS_FOLDER_BT;
					MMIAPISMS_CreateMapSyncTimer();
					BT_MapSetMessageStatus(handle_number, BT_MAP_SET_MSG_STATUS_DELETED, TRUE);
					return;
				}
            }
            else
            {
                MMIAPISMS_HandleMsgDeleteStatus();
                return;
            }
            
#else
            MMIAPISMS_HandleMsgDeleteStatus();
            return;
#endif
        }
        else
#endif
{
#ifdef DROPDOWN_NOTIFY_SHORTCUT

        //notify notification App to delete this item
        uint32 notify_handle = 0;
        MMIEVENT_INFO_T event_info = {0};

        notify_handle = WatchSMS_GetNotifyHandleByOrderID(cur_order_id);
        event_info.event_type = SMS_DELETESMS_EVENT;
        event_info.param_ptr = &notify_handle;
        SCI_TRACE_LOW("SMSNotify: delete sms Trigger notify_handle=%d",notify_handle);
        MMIFW_TriggerEvent(&event_info);
#endif
        MMISMS_DelSmsInfoInOrder(cur_order_id);            
}

        if (MMISMS_FOLDER_NORMAL != g_mmisms_global.nv_param.folder_type[record_id]
            && MN_SMS_STORAGE_ME == storage)
        {
            MMISMS_SetNVFolderTypeFlag(record_id,MMISMS_FOLDER_NORMAL);
        }               

        ret = MMISMSAPP_HandleDeleteCnf(record_id,storage);
        if(!ret)
        {
            SCI_TRACE_LOW("mmisms enter delete sms not called by sms ui");
            if((g_mmisms_global.update_info.current_num < MMISMS_SPLIT_MAX_NUM)
                && (g_mmisms_global.update_info.order_id_arr[g_mmisms_global.update_info.current_num] != PNULL )
                )
            {
                MMISMS_ORDER_ID_T update_order_id = PNULL;
                update_order_id = g_mmisms_global.update_info.order_id_arr[g_mmisms_global.update_info.current_num];
                MNSMS_UpdateSmsStatusEx(update_order_id->flag.dual_sys, update_order_id->flag.storage, update_order_id->record_id, MN_SMS_FREE_SPACE);                
                MMISMS_SetCurOrderId(0, update_order_id);   
                g_mmisms_global.update_info.current_num++;
            }
            else
            {
                MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE); 
            }
        }
        else
        {        
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE); 
        }
    }
    else
    {
        MMISMS_ClearOperInfo();
        // 提示用户删除失败
        if(MN_SMS_SIM_SAVE_STATUS_FREE == cause)
        {
            MMIPUB_OpenAlertFailWin(TXT_SMS_READ_ERR);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
        
        //全部删除完毕，将未处理链表中的sms插入到相应的链表中
        MMISMS_InsertUnhandledSmsToOrder();
        MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
        MMISMS_SetDelAllFlag(FALSE);

//lzk mark	
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)  
        MMISMS_HandleOrderChangeCallBack();
#endif

        MMIPUB_CloseWaitWin(MMISMS_DELETE_ANIM_WIN_ID);
        MMIAPIALM_CheckAndStartDelayedAlarm();
    }
       
}

/*****************************************************************************/
//     Description : to delete request to MN
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteReqToMN(
                            MMISMS_BOX_TYPE_E        type,        //IN:
                            MMISMS_ORDER_ID_T       order_id,    //IN:
                            MMISMS_OPER_STATUS_E    oper_status    //IN:
                            )
{
    BOOLEAN                    ret_val              = FALSE;
    MN_SMS_STORAGE_E        storage              = MN_SMS_NO_STORED;
    ERR_MNSMS_CODE_E        result            = ERR_MNSMS_NONE;
    MMISMS_STATE_T          sms_order_info = {0};
    MN_DUAL_SYS_E           dual_sys          = MN_DUAL_SYS_1;
    //SCI_TRACE_LOW:"MMISMS: DeleteReqToMN(%d, %d, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_161_112_2_18_2_52_34_22,(uint8*)"ddd", type, order_id, oper_status);
    
    // get the selected SMS's state
    MMISMS_GetSmsInfoFromOrder(order_id,&sms_order_info);
    
    storage = (MN_SMS_STORAGE_E)sms_order_info.flag.storage;
    dual_sys = (MN_DUAL_SYS_E)sms_order_info.flag.dual_sys;
    // to delete the SMS
    //SCI_TRACE_LOW:"MMISMS: DeleteReqToMN record_id = %d, storage = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_170_112_2_18_2_52_34_23,(uint8*)"dd", sms_order_info.record_id, storage);
    if(storage == MN_SMS_NO_STORED)
    {
       return ret_val;
    }
    result = MNSMS_UpdateSmsStatusEx(dual_sys, storage, sms_order_info.record_id, MN_SMS_FREE_SPACE);
    
    //SCI_TRACE_LOW:"MMISMS: DeleteReqToMN MNSMS_UpdateSmsStatusEx result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_173_112_2_18_2_52_34_24,(uint8*)"d", result);
    
    if (ERR_MNSMS_NONE == result)
    {
        //SCI_TRACE_LOW:"MMISMS: DeleteReqToMN oper_status = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_177_112_2_18_2_52_34_25,(uint8*)"d",oper_status);
        MMISMS_SetOperStatus(oper_status);
        MMISMS_SetCurOrderId(g_mmisms_global.operation.cur_order_index, order_id);    
        g_mmisms_global.operation.cur_type = type;
        //SCI_TRACE_LOW:"sms: DeleteReqToMN, g_mmisms_global.operation.cur_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_181_112_2_18_2_52_34_26,(uint8*)"d", g_mmisms_global.operation.cur_type);
        ret_val = TRUE;
    }
    else
    {
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        if((type ==  MMISMS_BT_BOX_MT || type ==  MMISMS_BT_BOX_SENDFAIL ||
            type ==  MMISMS_BT_BOX_NOSEND || type ==  MMISMS_BT_BOX_SENDSUCC) && 0 == storage)
        {
            MMISMS_DelBtSmsInfoInOrder(order_id);
        }
#endif
        ret_val = FALSE;
    }
    
    return (ret_val);        
}

/*****************************************************************************/
//     Description : to delete the pointed SMS
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeletePointedSMSById( 
                                           MMISMS_ORDER_ID_T    order_pos
                                           )
{
    BOOLEAN ret_val = FALSE;
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T record_id = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    MMISMS_GetPointedSMSMemInfo(order_pos, &dual_sys, &storage, &record_id);

    if(storage == MN_SMS_NO_STORED)
    {
       return ret_val;
    }
    // to delete the SMS
    if (ERR_MNSMS_NONE == MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, MN_SMS_FREE_SPACE))
    {
        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }
    
    return (ret_val);
}

/*****************************************************************************/
//     Description : to handle the signal APP_MN_UPDATE_SMS_STATUS_CNF
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: in the operation of delete and update, the system will return the same signal.
/*****************************************************************************/
PUBLIC void MMISMS_HandleUpdateSMSCnf(
                                      DPARAM  param
                                      )
{
    APP_MN_UPDATE_SMS_STATUS_CNF_T *sig_ptr = (APP_MN_UPDATE_SMS_STATUS_CNF_T *)param;
    MMISMS_OPER_STATUS_E oper_status = MMISMS_GetCurrentOperStatus();
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T record_id = 0;
    BOOLEAN result = TRUE;
    BOOLEAN is_in_order = TRUE;    // used for checking record in the order
    MMISMS_ORDER_ID_T cur_order_id= PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#if 0
// no use
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_SPACE;
#endif    
    uint8 total_num = 1;
    MMISMS_ORDER_ID_T next_longsms_ptr = PNULL;    
    uint8 fail_id[ MMISMS_DEST_ADDR_MAX_NUM ]= { 0 };
    uint16 send_result = 0;
    uint16 dest_count = 0;

    MMISMS_SEND_T *send_message_ptr = PNULL;
    MMISMS_UPDATE_INFO_T *update_info_ptr = PNULL;
    BOOLEAN is_find_backsend = FALSE;
    BOOLEAN is_mosend = FALSE;
    
    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf sig_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_251_112_2_18_2_52_34_27,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf record_id = %d, storage = %d,oper_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_255_112_2_18_2_52_34_28,(uint8*)"ddd", sig_ptr->record_id, sig_ptr->storage,oper_status);

    send_message_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
    if (PNULL != send_message_ptr)
    {
        if (send_message_ptr->backsend_update_data.is_backsend_update)
        {
            if (MN_SMS_NO_STORED != send_message_ptr->backsend_update_data.backsend_updata_storage 
                && sig_ptr->storage == send_message_ptr->backsend_update_data.backsend_updata_storage
                && sig_ptr->record_id == send_message_ptr->backsend_update_data.backsend_record_id)
            {
                SCI_TRACE_LOW("MMISMS: MMISMS_HandleUpdateSMSCnf backsend_update_data record_id = %d, storage = %d",send_message_ptr->backsend_update_data.backsend_record_id,send_message_ptr->backsend_update_data.backsend_updata_storage);
                oper_status = send_message_ptr->backsend_update_data.backsend_send_oper;
                is_find_backsend = TRUE;
            }
        }
        if (!is_find_backsend && send_message_ptr->mosend_update_data.is_mosend_update)
        {
            if (MN_SMS_NO_STORED != send_message_ptr->mosend_update_data.mosend_updata_storage 
                && sig_ptr->storage == send_message_ptr->mosend_update_data.mosend_updata_storage
                && sig_ptr->record_id == send_message_ptr->mosend_update_data.mosend_record_id)
            {
                SCI_TRACE_LOW("MMISMS: MMISMS_HandleUpdateSMSCnf mosend_update_data record_id = %d, storage = %d",send_message_ptr->backsend_update_data.backsend_record_id,send_message_ptr->backsend_update_data.backsend_updata_storage);
                oper_status = send_message_ptr->mosend_update_data.mosend_send_oper;
                is_mosend = TRUE;
            }
        }        
    }
    
    SCI_TRACE_LOW("MMISMS: MMISMS_HandleUpdateSMSCnf oper_status = %d",oper_status);

    // according to the operate status
    switch (oper_status) 
    {
#ifdef MMI_PDA_SUPPORT
    case MMISMS_MATCH_DATA_DELETE:
        {
            if (PNULL == g_sms_match_data_ptr)
            {
                MMISMS_EndDeleteMatchMsg(g_sms_match_data_ptr);
            }
            else
            {
                cur_order_id = g_sms_match_data_ptr->order_id_list[g_match_sms_delete_num];
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
                if(MMISMS_FOLDER_BT == cur_order_id->flag.folder_type)
                {
                    MMISMS_DelBtSmsInfoInOrder(cur_order_id);
                }
                else
                {
                    MMISMS_DelSmsInfoInOrder(cur_order_id);
                }
#else				
                MMISMS_DelSmsInfoInOrder(cur_order_id);
#endif
                MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);    
                g_match_sms_delete_num = MMISMS_GetNextSelectedOrderID();
                if (g_match_sms_delete_num >= g_sms_match_data_ptr->matched_count)
                {
                    MMISMS_EndDeleteMatchMsg(g_sms_match_data_ptr);
                }
                else
                {
                    MMISMS_DeleteNextMatchMsgById(g_sms_match_data_ptr->order_id_list[g_match_sms_delete_num]);
                }                
            }
        }
        break;
#endif
    case MMISMS_DELETE_MSG:                // delete SMS operation
    case MMISMS_MOVE_SMS_TO_SECURITY:
    case MMISMS_MOVE_SMS_FROM_SECURITY:
    case MMISMS_MOVE_SMS_TO_ME:
    case MMISMS_MOVE_SMS_TO_SIM:        
        // get the storage and record_id
        is_in_order = MMISMS_GetPointedSMSMemInfo(
            MMISMS_GetCurOperationOrderId(), 
            &dual_sys,
            &storage, 
            &record_id
            );
        if ((is_in_order)
            && (sig_ptr->record_id == record_id) 
            && (sig_ptr->storage == storage)
            && (sig_ptr->dual_sys == dual_sys))
        {
            // the same storage and record_id.
            result = TRUE;
        }
        else
        {
            // search in order list.
            // ignore the different message type
            //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf get record_id = %d, storage = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_334_112_2_18_2_52_35_29,(uint8*)"dd", record_id, storage);
            result = MMISMS_GetOrderIdByStorage(
                &cur_order_id,
                MMISMS_TYPE_SMS,
                sig_ptr->dual_sys,
                sig_ptr->storage,
                sig_ptr->record_id
                );
        }
        
        if (result)
        {
            if(MMISMS_DELETE_MSG == oper_status)
            {
                HandleDeleteCnf(sig_ptr->cause,sig_ptr->record_id,sig_ptr->storage,sig_ptr->dual_sys);
            }
#ifdef MMI_SMS_MOVE_SUPPORT            
            else if(MMISMS_IsMoveOperation(oper_status))
            {
                HandleMoveDeleteCnf(sig_ptr->cause);
            }
#endif            
            else
            {
                MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);
                if (MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID))
                {
                    //MMK_CloseWin(MMK_GetPrevWinId(MMISMS_DELETE_ANIM_WIN_ID));
                    MMIPUB_CloseWaitWin(MMISMS_DELETE_ANIM_WIN_ID);
                }
            }
        }
        else
        {
            // there's error in the deleting procedure.
            //MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);
            if (MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID))
            {
                //MMK_CloseWin(MMK_GetPrevWinId(MMISMS_DELETE_ANIM_WIN_ID));
                MMIPUB_CloseWaitWin(MMISMS_DELETE_ANIM_WIN_ID);
            }
        }
        break;
        
    case MMISMS_UPDATE_SENDFAIL_SMS:
    case MMISMS_UPDATE_NOSEND_SMS:    // update the no send SMS   
    case MMISMS_DELETE_SENDFAIL_SMS:
    case MMISMS_UPDATE_BACKSEND_SMS:
        if(g_mmisms_global.save_info.is_change_save_pos)
        {
            SCI_TRACE_LOW("mmisms is_change_save_pos TRUE");
            cur_order_id = MMISMS_GetLongsmsOrder();
            if(PNULL == cur_order_id)
            {
                g_mmisms_global.save_info.is_change_save_pos = FALSE;
                MMISMS_SetsmsOrder(PNULL);
                MMISMS_ClearSaveInfo();
                MMISMS_DisplayUpdatePrompt(TRUE);
                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
//lzk mark			
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)  
                MMISMS_HandleOrderChangeCallBack();
#endif
                MMISMS_SetDeliverResendFlag(FALSE);
                //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_422_112_2_18_2_52_35_30,(uint8*)"d", __LINE__);
                MMISMS_ClearOperInfo();
                MMISMS_DeleteSendList();
                return;
            }
            next_longsms_ptr = cur_order_id->next_long_sms_ptr;
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        if(MMISMS_FOLDER_BT == cur_order_id->flag.folder_type)
        {
            MMISMS_DelBtSmsInfoInOrder(cur_order_id);
        }
        else
        {
            MMISMS_DelSmsInfoInOrder(cur_order_id);
        }
#else            			
            MMISMS_DelSmsInfoInOrder(cur_order_id);
#endif
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);    
            if( PNULL != next_longsms_ptr)
            {
#if 0
// no use para
                err_code = MNSMS_UpdateSmsStatusEx(
                                            next_longsms_ptr->flag.dual_sys, 
                                            next_longsms_ptr->flag.storage, 
                                            next_longsms_ptr->record_id, 
                                            MN_SMS_FREE_SPACE);
#else
                            MNSMS_UpdateSmsStatusEx(
                                            next_longsms_ptr->flag.dual_sys, 
                                            next_longsms_ptr->flag.storage, 
                                            next_longsms_ptr->record_id, 
                                            MN_SMS_FREE_SPACE);
#endif
                
                MMISMS_SetsmsOrder(next_longsms_ptr);
            }
            else if(MMISMS_UPDATE_BACKSEND_SMS == oper_status)
            {
                g_mmisms_global.save_info.is_change_save_pos = FALSE;
                MMISMS_SetsmsOrder(PNULL);
                MMISMS_ClearSaveInfo();
                MMISMS_DisplayUpdatePrompt(TRUE);
                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
				
//lzk mark
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)  
                MMISMS_HandleOrderChangeCallBack();
#endif
                MMISMS_SetDeliverResendFlag(FALSE);
                //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_422_112_2_18_2_52_35_30,(uint8*)"d", __LINE__);
                MMISMS_ClearOperInfo();
                MMISMS_DeleteSendList();
            }
            else
            {
                if(MMISMS_UPDATE_NOSEND_SMS == oper_status)
                {
                    MMISMS_SMSEDIT_CONTENT_T* edit_message_ptr = MMISMS_GetEditContentFromGlobal();
                    if(PNULL != edit_message_ptr)
                    {
                        MMISMS_SetsmsOrder(PNULL);
                        total_num = MMISMS_SplitLongMsg(&edit_message_ptr->send_message, FALSE);
                        MMISMS_SaveNextMessage(
                                                    dual_sys,
                                                    edit_message_ptr->dest_info.update_dest_addr,
                                                    total_num,
                                                    1,
                                                    PNULL);
                    }    

                }
                else if(MMISMS_DELETE_SENDFAIL_SMS != oper_status)
                {
                    send_message_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
                    if(PNULL != send_message_ptr)
                    {
                        MMISMS_SetsmsOrder(PNULL);
                        total_num = MMISMS_SplitLongMsg(&send_message_ptr->send_content.send_message, send_message_ptr->is_head_has_port);
                        MMISMS_SaveNextMessage(
                                                    dual_sys,
                                                    send_message_ptr->send_content.dest_info.update_dest_addr,
                                                    total_num,
                                                    1,
                                                    send_message_ptr);
                    }                    
                }
                else
                {
                    // 提示保存成功
                    g_mmisms_global.save_info.is_change_save_pos = FALSE;
                    MMISMS_SetsmsOrder(PNULL);
                    MMISMS_ClearSaveInfo();
                    MMISMS_DisplayUpdatePrompt(TRUE);
                    MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
					
//lzk mark
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)  
                    MMISMS_HandleOrderChangeCallBack();
#endif
                    MMISMS_SetDeliverResendFlag(FALSE);
                    //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_422_112_2_18_2_52_35_30,(uint8*)"d", __LINE__);
                    MMISMS_ClearOperInfo();
                }                
            }
            return;
        }
        if (PNULL != send_message_ptr)
        {
            if (MMISMS_UPDATE_BACKSEND_SMS == oper_status)
            {
                update_info_ptr = &send_message_ptr->backsend_update_data.backsend_update_info;
            }
            else if (is_mosend)
            {
                update_info_ptr = &send_message_ptr->mosend_update_data.mosend_update_info;
            }
        }
        if (PNULL == update_info_ptr)
        {
            update_info_ptr = &g_mmisms_global.update_info;
        }
        
        // get the storage and record_id
        MMISMS_GetPointedSMSMemInfo(
            update_info_ptr->order_id_arr[update_info_ptr->current_num],
            &dual_sys,
            &storage, 
            &record_id
            );
        SCI_TRACE_LOW("mmisms update sms meminfo: dual_sys = %d,storage = %d,record_id = %d",dual_sys,storage,record_id);
        if ((sig_ptr->record_id == record_id) && (sig_ptr->storage == storage))
        {
            // update the SMS
            if (MN_SMS_OPERATE_SUCCESS == sig_ptr->cause)
            {
#ifdef MMI_SMS_CHAT_SUPPORT            
                BOOLEAN is_need_update_chatwin = FALSE;
#endif
#ifdef MMI_TIMERMSG_SUPPORT
                MMISMS_TIMERMSG_DeleteTimerMsg(update_info_ptr->order_id_arr[update_info_ptr->current_num]);
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
                if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)) //当前条目删除完整了
                {
                    uint32 index = 0;

                    cur_order_id = update_info_ptr->order_id_arr[update_info_ptr->current_num];
                    if(MMISMS_FindChatWinIndex(&index, cur_order_id))
                    {
                        if((update_info_ptr->current_num + 1) < update_info_ptr->update_num)
                        {
                            if (cur_order_id->flag.is_concatenate_sms)
                            {
                                next_longsms_ptr = cur_order_id->next_long_sms_ptr;
                                g_mmisms_global.chat_item[index].cur_order_id = next_longsms_ptr;
                            }
                        }
                        else
                        {
                            is_need_update_chatwin = TRUE;
                            g_mmisms_global.del_index = index;
                        }                        
                    }                    
                    
                }
#endif           

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
                if(MMISMS_FOLDER_BT == update_info_ptr->order_id_arr[update_info_ptr->current_num]->flag.folder_type)
                {
                    MMISMS_DelBtSmsInfoInOrder(update_info_ptr->order_id_arr[update_info_ptr->current_num]);
                }
                else
                {
                    MMISMS_DelSmsInfoInOrder(update_info_ptr->order_id_arr[update_info_ptr->current_num]);
                }
#else                
                MMISMS_DelSmsInfoInOrder(update_info_ptr->order_id_arr[update_info_ptr->current_num]);  //根据order_id_arr找到删除的form中的index
#endif
                MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);    
#ifdef MMI_SMS_CHAT_SUPPORT                
                if(is_need_update_chatwin && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
                {
                     MMK_SendMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, 
                        MMISMS_MSG_UPDATELIST, 
                        PNULL);    //通知相应的窗口，更新list内容    
                }
#endif                
               update_info_ptr->current_num++;
                
                if (update_info_ptr->current_num < update_info_ptr->total_num || 
                    (MMISMS_UPDATE_NOSEND_SMS == oper_status && ( update_info_ptr->current_num < MMISMS_SPLIT_MAX_NUM )&& update_info_ptr->order_id_arr[update_info_ptr->current_num] != PNULL))
                {
                    //未删完,要设置storage和record id
                    MMISMS_GetPointedSMSMemInfo(
                        update_info_ptr->order_id_arr[update_info_ptr->current_num],
                        &dual_sys, &storage, &record_id);
                    if (MMISMS_UPDATE_BACKSEND_SMS == oper_status)
                    {
                        if(send_message_ptr != PNULL)
                        {
                            send_message_ptr->backsend_update_data.backsend_record_id = record_id;
                            send_message_ptr->backsend_update_data.backsend_updata_storage = storage;
                        }
                    }
                    else if (PNULL != send_message_ptr && send_message_ptr->mosend_update_data.is_mosend_update)
                    {
                            send_message_ptr->mosend_update_data.mosend_record_id = record_id;
                            send_message_ptr->mosend_update_data.mosend_updata_storage = storage;
                    }                    
                    
                    if (!MMISMS_DeletePointedSMSById(update_info_ptr->order_id_arr[update_info_ptr->current_num]))
                    {
                        MMISMS_DisplayUpdatePrompt(TRUE);
                    }
                }
                else if(MMISMS_UPDATE_NOSEND_SMS == oper_status)
                {
                    if (PNULL != send_message_ptr && send_message_ptr->mosend_update_data.is_mosend_update)
                    {
                        if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
                        {
                            MMISMS_InitSendSaveResult(send_message_ptr);
                            MMISMS_AppSendSms(sig_ptr->dual_sys, send_message_ptr->send_content.is_need_save, send_message_ptr);
                            //MMISMS_UpdateSmsSendindNumber(send_message_ptr);
                            //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);
                        }
                        else
                        {
                            // 提示发送失败/保存成功
                            if(send_message_ptr->mosend_update_data.mosend_update_info.total_num >= send_message_ptr->backsend_update_data.backsend_update_info.total_num )
                            {
                                MMISMS_ClearSaveInfo();
                                send_result = MMISMS_GetResultofSending(fail_id, &dest_count);
                                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                                MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                                MMIPUB_CloseWaitWin(MMISMS_READSMS_WAIT_WIN_ID);
                                MMISMS_SetDeliverResendFlag(FALSE);
                                MMISMS_ShowSendResult((uint8)send_result, fail_id);
                                MMISMS_DeleteSendList();
                                //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf %d"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_533_112_2_18_2_52_35_31,(uint8*)"d", __LINE__);
                            }
                        }
                    }
                    else
                    {
                        MMISMS_DisplayUpdatePrompt(TRUE);
                        MMISMSAPP_HandleReplaceCnf(record_id,
                            storage,
                            TRUE);
                    }                    
                }
                else if(MMISMS_DELETE_SENDFAIL_SMS == oper_status || MMISMS_UPDATE_BACKSEND_SMS == oper_status)
                {
                    if (MMISMS_UPDATE_BACKSEND_SMS == oper_status 
                        && PNULL != send_message_ptr 
                        && send_message_ptr->mosend_update_data.is_mosend_update
                        && send_message_ptr->mosend_update_data.mosend_update_info.current_num < send_message_ptr->mosend_update_data.mosend_update_info.total_num)
                    {
                        result = TRUE;
                        break;
                    }
                    //if(MMK_IsOpenWin(MMISMS_SEND_ANIM_WIN_ID))
                    {                        
                        // to send the message to next destination
                        if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId() && PNULL != send_message_ptr)
                        {
                            MMISMS_InitSendSaveResult(send_message_ptr);
                            MMISMS_AppSendSms(sig_ptr->dual_sys, send_message_ptr->send_content.is_need_save, send_message_ptr);
                            //MMISMS_UpdateSmsSendindNumber(send_message_ptr);
                            //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);
                        }
                        else
                        {
                            #ifdef MMI_TIMERMSG_SUPPORT          
							BOOLEAN is_timer_mode = MMISMS_GetIsTimerMode();
                            #endif
                            // 提示发送失败/保存成功
                            MMISMS_ClearSaveInfo();
                            send_result = MMISMS_GetResultofSending(fail_id, &dest_count);
							
                            if(MMK_IsOpenWin(MMISMS_SAVEMSG_ANIM_WIN_ID)
                            #ifdef MMI_TIMERMSG_SUPPORT							
                            || is_timer_mode
                            #endif	
                            )
                            {
                                MMISMS_CloseEditWin(); 
                            }
                            MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                            MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                            MMIPUB_CloseWaitWin(MMISMS_READSMS_WAIT_WIN_ID); 
                            
                            MMISMS_SetDeliverResendFlag(FALSE);
                            MMISMS_ShowSendResult((uint8)send_result, fail_id);
                            MMISMS_DeleteSendList();
                            //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_583_112_2_18_2_52_35_32,(uint8*)"d", __LINE__);
                        }  
                    }
//                     else
//                     {
//                         // 提示保存成功
//                         MMISMS_ClearSaveInfo();
//                         MMISMS_DisplayUpdatePrompt(TRUE);
//                         MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
//                         MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
//                         MMISMS_SetDeliverResendFlag(FALSE);
//                         SCI_TRACE_LOW("MMISMS: MMISMS_HandleUpdateSMSCnf %d", __LINE__);
//                         MMISMS_ClearOperInfo();
//                         MMISMS_DeleteSendList();
//                     }
                }
                else
                {
#ifdef MMI_TIMERMSG_SUPPORT
                    BOOLEAN is_timer_msg = MMISMS_GetIsTimerMsg();
#endif
                    //删完
                    g_mmisms_global.is_wait_update = FALSE;

                    send_result = MMISMS_GetResultofSending(fail_id, &dest_count);
                    MMISMS_ClearSaveInfo();
                    MMISMS_ShowSendResult((uint8)send_result, fail_id);
                    MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
                    MMISMS_DeleteSendList();
                    MMISMS_SetDeliverResendFlag(FALSE);

#ifdef MMI_TIMERMSG_SUPPORT
                    if (!is_timer_msg)
#endif
                    {
                        MMIPUB_CloseWaitWin(MMISMS_READSMS_WAIT_WIN_ID);
#ifdef MMIMMS_SMS_IN_1_SUPPORT 
                        //MMIAPIMMS_CloseEditWin();
#else
                        //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
#endif
                        MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);                    
                    }
                }
            }
            else
            {

                MMISMS_ORDER_ID_T order_id = PNULL;
                MMISMS_GetOrderIdByStorage(&order_id,MMISMS_TYPE_SMS,sig_ptr->dual_sys,sig_ptr->storage,sig_ptr->record_id);  
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
                if(MMISMS_FOLDER_BT == order_id->flag.folder_type)
                {
                    MMISMS_DelBtSmsInfoInOrder(order_id);
                }
                else
                {
                    MMISMS_DelSmsInfoInOrder(order_id);
                }
#else
                MMISMS_DelSmsInfoInOrder(order_id);
#endif
                MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);    
                SCI_TRACE_LOW("MMISMS: MMISMS_HandleUpdateSMSCnf delete error");
   
                if(send_message_ptr != PNULL && (
                   (send_message_ptr->mosend_update_data.is_mosend_update && MMISMS_UPDATE_NOSEND_SMS == oper_status)||
                   (send_message_ptr->backsend_update_data.is_backsend_update && MMISMS_UPDATE_BACKSEND_SMS == oper_status))
                )
                {
                    MMISMS_ClearSaveInfo();
                    send_result = MMISMS_GetResultofSending(fail_id, &dest_count);
                    MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                    MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                    MMIPUB_CloseWaitWin(MMISMS_READSMS_WAIT_WIN_ID);
                    MMISMS_SetDeliverResendFlag(FALSE);
                    MMISMS_ShowSendResult((uint8)send_result, fail_id);
                    MMISMS_DeleteSendList();
                    //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_533_112_2_18_2_52_35_31,(uint8*)"d", __LINE__);
                }
            }
            
            result = TRUE;
        }
        else
        {
#ifdef MMI_TIMERMSG_SUPPORT
            MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif  
            {
                MMISMS_ORDER_ID_T order_id = PNULL;
                MMISMS_GetOrderIdByStorage(&order_id,MMISMS_TYPE_SMS,sig_ptr->dual_sys,sig_ptr->storage,sig_ptr->record_id);  
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
                if(MMISMS_FOLDER_BT == order_id->flag.folder_type)
                {
                    MMISMS_DelBtSmsInfoInOrder(order_id);
                }
                else
                {
                    MMISMS_DelSmsInfoInOrder(order_id);
                }
#else				
                MMISMS_DelSmsInfoInOrder(order_id);
#endif
                MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);    
                SCI_TRACE_LOW("MMISMS: MMISMS_HandleUpdateSMSCnf delete error");
            }
            if(send_message_ptr != PNULL && (
                    (send_message_ptr->mosend_update_data.is_mosend_update && MMISMS_UPDATE_NOSEND_SMS == oper_status)||
                    (send_message_ptr->backsend_update_data.is_backsend_update && MMISMS_UPDATE_BACKSEND_SMS == oper_status))
              )
            {
                MMISMS_ClearSaveInfo();
                send_result = MMISMS_GetResultofSending(fail_id, &dest_count);
                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                MMIPUB_CloseWaitWin(MMISMS_READSMS_WAIT_WIN_ID);
                MMISMS_SetDeliverResendFlag(FALSE);
                MMISMS_ShowSendResult((uint8)send_result, fail_id);
                MMISMS_DeleteSendList();
                //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_533_112_2_18_2_52_35_31,(uint8*)"d", __LINE__);    
            }
            result = FALSE;
        }
        break;
        
    case MMISMS_READ_SMS:    // update SMS status from unread to read
        result = FALSE;
        break;  

#ifdef MMI_SMS_CHAT_SUPPORT           
    case MMISMS_UPDATE_EXCEED_CHAT_SMS:
        {            
            if(g_mmisms_global.update_oper_order_id != PNULL)
            {
                MMISMS_STATE_T order_info = {0};            
                MMISMS_GetSmsInfoFromOrder (
                                            g_mmisms_global.update_oper_order_id, 
                                            &order_info 
                                            );        
                // to modify the status in the order list and order the mt sms again.
                order_info.flag.mo_mt_type = MMISMS_MT_HAVE_READ;        
                order_info.flag.is_receive_all = FALSE;        
                MMISMS_UpdateSmsInfoInOrder(g_mmisms_global.update_oper_order_id, &order_info);
                MMISMS_SetExceedUnreadNeedUpdate();
            }
            else
            {
                MMISMS_ClearOperInfo();
                MMK_CloseWin(MMISMS_WAITING_WIN_ID);
            }
        }
        break;
#endif

    default:
        result = FALSE;
        break;
    }
    
    if (!result)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUpdateSMSCnf sig_ptr->record_id = %d, sig_ptr->storage = %d, oper_status = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_652_112_2_18_2_52_35_33,(uint8*)"ddd", sig_ptr->record_id, sig_ptr->storage, oper_status);
        if (g_mmisms_global.update_status_num > 0)
        {
            // from unread to read
            g_mmisms_global.update_status_num--;
        }
    }

    if (MMK_IsOpenWin(MMISMS_MEMFULL_ALERT_USER_WIN_ID))
    {
        if (!MMIAPISMS_IsSMSFull())
        {
            MMK_CloseWin(MMISMS_MEMFULL_ALERT_USER_WIN_ID);
        }
    }

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    if(MMISMS_FOLDER_BT ==	g_mmisms_global.folder_type) 
    {
        MAIN_SetBtMsgState(TRUE);
    }
    else
#endif
{
    MAIN_SetMsgState(TRUE);
}

#ifdef PDA_UI_DROPDOWN_WIN
    if (!MMIAPISMS_IsSMSFull())
    {
        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_NOTIFY_MESSAGE_FULL);
    }    
#endif
}

/*****************************************************************************/
//     Description : to delete the SMS box called by window
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppDeleteMsgBox(
                                                MMISMS_BOX_TYPE_E    type,    //IN:
                                                uint16* msg_type,  //out
                                                MMISMS_FOLDER_TYPE_E folder_type
                                                )
{
    MMISMS_OPER_ERR_E ret_val = MMISMS_EMPTY_BOX;
 
    
    return (ret_val);
}
#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//     Description : to handle the signal APP_MN_UPDATE_SMS_STATUS_CNF
//    Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//    Note: handle the normal move operation
/*****************************************************************************/
LOCAL void HandleMoveDeleteCnf( 
                           MN_SMS_CAUSE_E    cause    //IN:
                           )
{
#if 0
//no use
    int32 record_id = 0;
#endif
    MMISMS_ORDER_ID_T order_id = MMISMS_GetCurOperationOrderId();
    MMISMS_OPER_ERR_E oper_error = MMISMS_NO_ERR;
#if 0
//no use
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
#endif
    uint8 index = g_mmisms_global.operation.cur_order_index;
    
    //SMSHandleMoveCnf(record_id,record_id,cause);
    //SCI_TRACE_LOW:"MMISMS: HandleMoveCnf cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_948_112_2_18_2_52_36_38,(uint8*)"d", cause);

    if (MN_SMS_OPERATE_SUCCESS == cause)
    {
        //SCI_TRACE_LOW:"MMISMS: HandleMoveCnf move_all_flag = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_DELETE_952_112_2_18_2_52_36_39,(uint8*)"d", MMISMS_GetMoveAllFlag());
        // 删除SIM短信按取消，需将已经删除的短信重新保存
        if (MMISMS_GetMoveSingleFlag())
        {
            //storage = (MN_SMS_STORAGE_E)order_id->flag.storage;
            //MMISMS_MOVESaveSms(order_id);
#ifdef MMI_TIMERMSG_SUPPORT
            MMISMS_TIMERMSG_DeleteTimerMsg(g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index]);
#endif

            MMISMS_SetCurMsgLocked(FALSE);
#if 0
//no use
            record_id = g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index]->record_id;
#endif

            //order_id->flag.storage = storage; 
            MMISMS_DelSmsInfoInOrder(order_id);
            //order_id->flag.storage = MN_SMS_STORAGE_ME;

            index++;
            if(MMISMS_SPLIT_MAX_NUM <= index)
            {
                order_id = PNULL;
            }
            else
            {
                g_mmisms_global.operation.cur_order_index = index;
                order_id = MMISMS_GetCurOperationOrderId();
            }
           
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE); 
            if(PNULL != order_id)
            {
#if 0
//no use
                err_code = MNSMS_UpdateSmsStatusEx(
                                                    order_id->flag.dual_sys, 
                                                    order_id->flag.storage, 
                                                    order_id->record_id, 
                                                    MN_SMS_FREE_SPACE);                
#else
                            MNSMS_UpdateSmsStatusEx(
                                                    order_id->flag.dual_sys, 
                                                    order_id->flag.storage, 
                                                    order_id->record_id, 
                                                    MN_SMS_FREE_SPACE);   
#endif
            }
            else
            {

                MMISMSAPP_HandleMoveCnf(MMISMS_NO_ERR);
            }
        }
        else
        {       
            MMISMSAPP_HandleMoveCnf(MMISMS_FINISH_OPER_ERR);
        }        
    }
    else
    {        
        if(MMISMS_GetMoveSingleFlag())
        {
            MMISMS_SetMoveSingleFlag(FALSE);
            MMISMSAPP_HandleMoveCnf(MMISMS_NO_ERR);
        }
        else
        {
            if(MN_SMS_SIM_SAVE_STATUS_FREE == cause)
            {
                oper_error = MMISMS_SMS_READ_FAIL;
            }
        
            MMISMSAPP_HandleMoveCnf(oper_error);
        }
    }

}
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : get del index in chat read
//    Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//    Note: handle the normal move operation
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetDelIndex(void)
{
    return g_mmisms_global.del_index;
}
/*****************************************************************************/
//     Description : get del index in chat read
//    Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//    Note: handle the normal move operation
/*****************************************************************************/
PUBLIC void MMISMS_SetDelIndex(uint16 index)
{
    g_mmisms_global.del_index = index;
}

/*****************************************************************************/
//  Description :get chat win index
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMISMS_FindChatWinIndex(   //已有短信找index
                                          uint32 *index_ptr,
                                          MMISMS_ORDER_ID_T order_id
                                         )
{
    uint32 max = MMISMS_CHAT_MAX_ITEM;
    uint32 i = 0;
    BOOLEAN result = FALSE;
    MMISMS_ORDER_ID_T cur_longsms_order_id = PNULL;
    if(order_id == PNULL)
    {
        return FALSE; 
    }
    for(i=0;i<max;i++)
    {
        if(!order_id->flag.is_concatenate_sms)
        {
            if((g_mmisms_global.chat_item[i].cur_order_id != PNULL) && MMISMS_IsSameSMS(order_id, g_mmisms_global.chat_item[i].cur_order_id))
            {
                *index_ptr = i;
                return TRUE;
            }
        }
        else
        {
            cur_longsms_order_id = g_mmisms_global.chat_item[i].cur_order_id;
            while(PNULL != cur_longsms_order_id)
            {
                 if(MMISMS_HaveSamePart(cur_longsms_order_id,order_id))
                 {
                    *index_ptr= i;
                    return TRUE;
                 }
                 cur_longsms_order_id = cur_longsms_order_id->next_long_sms_ptr;
            }
        }
    }

    return result;
}

#endif

#ifdef MMISMS_REPLACE_CONCATENATED_SMS_SUPPORT
/*****************************************************************************/
//  Description : delet replace sms
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_DeleteReplaceSms(DPARAM param)
{
    APP_MN_DELETE_SMS_IND_T *sig_ptr = (APP_MN_DELETE_SMS_IND_T *)param;
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_MAX;
    MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id = PNULL;    
    uint16 i = 0;
    
    if(PNULL == sig_ptr)
    {
        SCI_TRACE_LOW("mmisms delete replace sig_ptr PNULL");
        return;
    }
    storage = sig_ptr->storage;
    dual_sys = sig_ptr->dual_sys;
    SCI_TRACE_LOW("mmisms delete replace sig_ptr valid_num %d",sig_ptr->valid_num);
    for(i = 0; i < sig_ptr->valid_num; i++)
    {
        SCI_TRACE_LOW("mmisms delete replace sig_ptr record_id = %d",sig_ptr->record_id[i]);
        cur_unit_id = MMISMS_GetUsedOrderId();
        while (cur_unit_id != PNULL)
        {
            if(cur_unit_id->sms_state.flag.storage == storage
                && cur_unit_id->sms_state.flag.dual_sys == dual_sys
                && cur_unit_id->sms_state.record_id == sig_ptr->record_id[i]
                )
            {
                SCI_TRACE_LOW("mmisms delete replace sig_ptr refnum = %d,seqnum = %d",cur_unit_id->sms_state.head_ref,cur_unit_id->sms_state.longsms_seq_num);
                MMISMS_DelSmsInfoInOrder(&cur_unit_id->sms_state);
                break;
            }
            else
            {
                cur_unit_id = cur_unit_id->next_ptr;
            }
        }                   
    }
    MMISMS_HandleOrderChangeCallBack();
}
#endif


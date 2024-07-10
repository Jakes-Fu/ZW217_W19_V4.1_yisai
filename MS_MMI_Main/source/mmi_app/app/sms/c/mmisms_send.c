/****************************************************************************
** File Name:      mmisms_send.c                                            *
** Author:         liming.deng                                              *
** Date:           8/23/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe sending SMS.               *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2007        liming.deng      Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_send.h"
#include "mmisms_order.h"
#include "mmisms_order.h"
#include "mmisms_export.h"
#include "mmi_signal_ext.h"
#include "mmisms_app.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmisms_save.h"
#include "mmisms_read.h"
#include "mmipub.h"
#ifdef MMI_TIMERMSG_SUPPORT
#include "mmisms_timermsg.h"
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#include "mmicc_export.h"
#endif
#include "mmistk_export.h"
#include "mmisms_set.h"
#include "mmipb_export.h"
#include "mmisms_edit.h"
#include "mmivcard_export.h"
#include "mmi_appmsg.h"
//#include "mmimms_text.h"
#include "mmk_timer.h"
#include "gui_ucs2b_converter.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef PUSH_EMAIL_SUPPORT
#include "mmimail_export.h"
#endif
//#include "mmieng.h"
#include "mmieng_export.h"
#ifdef MMI_VCALENDAR_SUPPORT
#include "Mmivcalendar.h"
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT 
#include "mmimms_id.h"
#include "mmimms_export.h"
#endif
#include "mmiidle_statusbar.h"
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#include "mn_error.h"
#include "mmisms_commonui.h"
#include "mmisms_chatreadwin.h"
#ifdef MMI_AUTOTEST_SUPPORT
#include "mmi_autotest.h"
#endif /* MMI_AUTOTEST_SUPPORT */

#include "mmisms_appcontrol.h"

#ifdef UAL_RF_AM_SUPPORT
#include "ual_rf_am.h"
#endif
#include "mmismsapp_interface.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define SMS_SEND_ANIM_TIMER_OUT				5000	//SENDANIM TIMER周期
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 
uint8  g_sms_retry_timer = 0;
MMISMS_SENDINFO_T  s_mmisms_sendlist = {0};
#ifdef MMI_MSG_RESEND
LOCAL uint8   s_resend_timer_id = 0;
LOCAL uint32 s_temp_cnt = 0;
#endif
LOCAL uint8				s_sms_waiting_time_id = 0;
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS的全局变量


/*****************************************************************************/
// Description : to set the user data head for Vcard
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void InitUserDataHeadForVcard(
                                    int32                     srcPort,
                                    MN_SMS_USER_DATA_HEAD_T  *data_head_ptr
                                    );

/*****************************************************************************/
//     Description : handle send SMS confirm, the cause is not success and need not resend
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleSMSSendCnfErrorNoResend(
                                         MN_DUAL_SYS_E   dual_sys,
                                         MN_SMS_CAUSE_E    cause,            //IN:
                                         BOOLEAN        is_need_send    //IN:
                                         );


/*****************************************************************************/
//     Description : handle send SMS confirm, the cause is success 
//    Global resource dependence : none
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleSMSSendSuccessCnf(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : to handle the signal of APP_MN_SEND_SMS_CNF
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void HandleSMSSendCnf(
                            MN_DUAL_SYS_E   dual_sys,
                            MN_SMS_CAUSE_E    cause,    //IN:
                            MN_SMS_MO_FAIL_CAUSE_E rl_cause
                            );

/*****************************************************************************/
//     Description : handle send SMS confirm, the cause is not success 
//    Global resource dependence : none
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleSMSSendErrorCnf(
                                 MN_DUAL_SYS_E   dual_sys,
                                 MN_SMS_CAUSE_E    cause,        //IN:
                                 MN_SMS_MO_FAIL_CAUSE_E rl_cause
                                 );

/*****************************************************************************/
//     Description : to send the pointed SMS to MN
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SendMsgReqToMN( 
                                       MN_DUAL_SYS_E    dual_sys,
                                       MN_CALLED_NUMBER_T        *dest_number_ptr,    //IN:
                                       APP_SMS_USER_DATA_T         *user_data_ptr,        //IN: the user data
                                       MN_SMS_STORAGE_E            storage,            //IN: the storage (SIM or NV)
                                       MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN: the dcs
                                       BOOLEAN                    is_more_msg,            //IN: the more message to send 
                                       BOOLEAN            is_resend,
                                       BOOLEAN is_send_rpt
                                       );

/*****************************************************************************/
//     Description : to close the other win after sent
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void CloseSendWinByResult(
                                uint8 fail_count
                                );

/*****************************************************************************/
//     Description : to set the count of fail by send group
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void SendFailCount( void );

#ifdef MMI_MSG_RESEND
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
LOCAL void MMISMS_HandleResendTimer(uint8 timer_id, uint32 param);
#endif

#ifdef JAVA_SUPPORT_SUN
#ifndef _MSC_VER
/*****************************************************************************/
//  Description : sun java send cnf callback
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISMS_SUNJavaSendCnfCallback(MN_SMS_CAUSE_E cause);  
#endif
#endif

/*****************************************************************************/
//  Description : vcard or vacalender send cnf callback
//  Global resource dependence : 
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISMS_VcardOrVcalSendCnfCallback(BOOLEAN is_success, DPARAM parm);

/*****************************************************************************/
//     Description : to set the user data head for other modules who has the 
//                   dst port and src port
//	Global resource dependence : none
//    Author:minghu.mao
//	Note: 
/*****************************************************************************/
LOCAL void InitUserDataHeadForOtherModules(
								   int32             srcPort,
				                   int32             dstPort,
                                   MN_SMS_USER_DATA_HEAD_T  *data_head_ptr,
								   BOOLEAN           is_longmsg
                                   );

/*****************************************************************************/
//  Description : clear expired sms
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void ClearExpiredSmsInSendList( void );

/*****************************************************************************/
//  Description : get is need resend sms
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_IsNeedResend(MN_SMS_MO_FAIL_CAUSE_E cause);

/*****************************************************************************/
//     Description : handle send SMS confirm, the cause is success 
//    Global resource dependence : none
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleSMSSendSuccessCnf(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_finished = TRUE;
    BOOLEAN is_send_success = TRUE;
    MMISMS_OPER_STATUS_E oper_status = MMISMS_NO_OPER;
    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleSMSSendSuccessCnf send_message = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_204_112_2_18_2_53_17_333,(uint8*)"");
        return;
    }
    if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content) || MMISMS_CheckNextDestAddr())
    {
        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
    }
    // if the resend function is valid, we must judge the element
    // and the operation status will not be send and save.    
    if (send_info_ptr->send_content.is_need_save) 
    {
        if ( MN_SMS_NO_STORED !=  send_info_ptr->send_save_result.save_storage )//如果收到了write_cnf
        {
            is_finished = TRUE;         
            
            MMISMS_SaveInfoToOrderByResult(
                send_info_ptr->send_save_result.dual_sys,
                send_info_ptr->send_save_result.save_storage, 
                send_info_ptr->send_save_result.save_record_id,
                MN_SMS_MO_SR_NOT_REQUEST, TRUE, MMISMS_FOLDER_NORMAL,  oper_status, send_info_ptr, MMISMS_SENDSTATUS_SENDSUCC
                );
            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDSUCC;
            // clear resend count
            send_info_ptr->send_save_result.send_result = MMISMS_WAIT_RESULT;
            send_info_ptr->send_save_result.save_storage = MN_SMS_NO_STORED;
            if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content))   //发送长短信的下一条
            {
                // send the next message to the same destination
                MMISMS_SendMsgBySeqNum(dual_sys, GET_HEAD_MAX_NUM(&send_info_ptr->send_content), GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1, FALSE, send_info_ptr);
            }
            else  //普通短信
            {
                if (0 == strlen((char *)send_info_ptr->send_content.dest_info.update_dest_addr))
                {
                    MMISMS_SaveUpdateAddr(FALSE);
                }
                
                // clear the destination address which send successfully
                MMISMS_ClearDestAddrById();
                
                if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
                {
                    // to send message to next destination address
                    MMISMS_InitSendSaveResult(send_info_ptr);
                    if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))  //发送到下一个号码
                    {
                        OperAfterSendCnf(dual_sys, is_send_success, is_finished);
                    }
                    else
                    {
                        //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                        //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);
                    }
                }
                else
                {
                    OperAfterSendCnf(dual_sys, is_send_success, is_finished);
                }
            }
            
        }
        else
        {
            send_info_ptr->send_save_result.send_status = MMISMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT;
            // do nothing            
        }        
        
    }
    else
    {
        // if there has any SMS to be sent, the variable is invalid.
        // if there has no SMS to be sent, the variable is indicated the end of the procedure.
        is_finished = TRUE;
        
        // clear resend count
        
        if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content)) //发送长短信的下一条
        {
            // send the next message to the same destination
            MMISMS_SendMsgBySeqNum(dual_sys, GET_HEAD_MAX_NUM(&send_info_ptr->send_content), GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1, FALSE, send_info_ptr);
        }
        else  //普通短信
        {
            if (0 == strlen((char *)send_info_ptr->send_content.dest_info.update_dest_addr))
            {
                MMISMS_SaveUpdateAddr(FALSE);
            }
            
            // clear the destination address which send successfully
            MMISMS_ClearDestAddrById();
            
            if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
            {
                // to send message to next destination address
                MMISMS_InitSendSaveResult(send_info_ptr);
                if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))  //发送到下一个号码
                {
                    OperAfterSendCnf(dual_sys, is_send_success, is_finished);
                }
                else
                {
                    //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                    //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);                        
                }
            }
            else
            {
                OperAfterSendCnf(dual_sys, is_send_success, is_finished);
            }
        }
    }    
}

/*****************************************************************************/
//     Description : handle send SMS confirm, the cause is not success 
//    Global resource dependence : none
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleSMSSendErrorCnf(
                                 MN_DUAL_SYS_E   dual_sys,
                                 MN_SMS_CAUSE_E    cause,        //IN:
                                 MN_SMS_MO_FAIL_CAUSE_E rl_cause
                                 )
{
    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleSMSSendErrorCnf PNULL ==  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_332_112_2_18_2_53_17_334,(uint8*)"");
        return;
    }
    
    if (MN_SMS_OPERATE_SUCCESS == cause)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleSMSSendErrorCnf cause ==  MN_SMS_OPERATE_SUCCESS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_338_112_2_18_2_53_17_335,(uint8*)"");
        return;
    }
    
    switch (cause)
    {
    case MN_SMS_SIM_NOT_READY:
    case MN_SMS_MN_BUSY:
        // for the current two cause, the SMS can not send.
        // just save and prompt error message.
        send_info_ptr->send_save_result.send_result = MMISMS_SEND_FAIL;    
        HandleSMSSendCnfErrorNoResend(dual_sys, cause, FALSE);
        break;

#ifndef MMI_DUALMODE_ENABLE 
    case MN_SMS_CANCEL_SUCCESS:
        //SCI_TRACE_LOW:"HandleSMSSendErrorCnf:cancel send sms is success"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_354_112_2_18_2_53_17_336,(uint8*)"");
        MMISMS_SetDeliverCancel(TRUE);
        send_info_ptr->send_save_result.send_result = MMISMS_SEND_CANCEL;  
        HandleSMSSendCnfErrorNoResend(dual_sys, cause, FALSE);
        break;
#endif
        
    default:
        SCI_TRACE_LOW("mmisms rl_cause = %d",rl_cause);
        //SCI_TRACE_LOW:"HandleSMSSendErrorCnf:resend_count is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_364_112_2_18_2_53_17_337,(uint8*)"d",send_info_ptr->resend_count);

        if (send_info_ptr->resend_count > 0 && MMISMS_IsNeedResend(rl_cause))
                {
                    
                    send_info_ptr->resend_count--;
                    send_info_ptr->send_save_result.send_result = MMISMS_SEND_FAIL;
                    send_info_ptr->send_save_result.send_status = MMISMS_WAIT_FOR_WRITE_CNF_TO_RESEND;
#ifdef MMI_SMS_MINI_SUPPORT          
                    if( g_sms_retry_timer != 0)
                    {
                        MMK_StopTimer(g_sms_retry_timer);
                        g_sms_retry_timer = 0;
                    }
                    g_sms_retry_timer = MMK_CreateWinTimer(MMISMS_SEND_ANIM_WIN_ID,MMISMS_RETRY_WAIT_TIMER,FALSE);
#else
                    MMISMS_ResendSms();
#endif
                }    
                else
                {
                    send_info_ptr->send_save_result.send_result = MMISMS_SEND_FAIL;
                    HandleSMSSendCnfErrorNoResend(dual_sys, cause, TRUE);
        }
        
        break;
    }
}

/*****************************************************************************/
//     Description : handle send SMS confirm, the cause is not success and need not resend
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleSMSSendCnfErrorNoResend(
                                         MN_DUAL_SYS_E  dual_sys,
                                         MN_SMS_CAUSE_E    cause,            //IN:
                                         BOOLEAN        is_need_send    //IN:
                                         )
{
    BOOLEAN is_need_save    = FALSE;
    BOOLEAN is_finish = FALSE;
    BOOLEAN is_need_init = TRUE;
    BOOLEAN is_send_success = FALSE;
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_ERR;
    MMISMS_OPER_STATUS_E oper_status = MMISMS_NO_OPER;    
    uint8 fail_id[MMISMS_DEST_ADDR_MAX_NUM] = {0};
    uint16 dest_count  = 0;
    uint16 send_result = 0;

    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleSMSSendCnfErrorNoResend PNULL ==  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_411_112_2_18_2_53_17_338,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"HandleSMSSendCnfErrorNoResend send type"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_415_112_2_18_2_53_17_339,(uint8*)"");
    MMISMS_SaveUpdateAddr(FALSE);
        SendFailCount();
    is_need_save    = send_info_ptr->send_content.is_need_save;
        
    if (send_info_ptr->send_content.tpdu_head.length > 0
        && (MMISMS_HEAD_8BIT_IEI == send_info_ptr->send_content.tpdu_head.user_header_arr[MMISMS_HEAD_IEI_POS]))  //是长短消息
        {
            if (is_need_save) 
            {
                if(MN_SMS_NO_STORED != send_info_ptr->send_save_result.save_storage) //本条收到了WRITE_CNF
                {
                    is_finish=TRUE;
                
                    MMISMS_SaveInfoToOrderByResult(
                    send_info_ptr->send_save_result.dual_sys,
                    send_info_ptr->send_save_result.save_storage, 
                    send_info_ptr->send_save_result.save_record_id,
                    MN_SMS_MO_TO_BE_SEND, TRUE, MMISMS_FOLDER_NORMAL,  oper_status, send_info_ptr, MMISMS_SENDSTATUS_SENDFAIL
                    );
                send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDFAIL;
                if (MMISMS_CheckNextDestAddr())
                    {
                    send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
                }
                
                if (!MMISMS_IsExistNextSMS(&send_info_ptr->send_content)) //是长短信的最后一条
                {
                        //发送下一条
                        if ( is_need_send )
                        {
                            if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
                            {
                                // send to the next destination address
                            MMISMS_InitSendSaveResult(send_info_ptr);
                            if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(dual_sys, send_info_ptr->send_content.is_need_save,send_info_ptr))
                                {
                                    OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                                }
                                else
                                {
                                //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                                //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);
                                }
                                //SCI_TRACE_LOW:"MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_459_112_2_18_2_53_17_340,(uint8*)"d", is_finish);
                            }
                            else
                            {
                                OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                            }
                        }
                        else
                        {
                            //do nothing
                        send_info_ptr->send_save_result.send_status = MMISMS_LONG_SMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT;
                        }
                    }
                    else
                    {
                        //OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                        //置保存、控制位为纯保存，保存下一条为草稿
                        MMISMS_SetOperStatus(MMISMS_SAVE_SENDFAIL_SMS);
                    send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SAVE_SENDFAIL_SMS;
                        save_result = MMISMS_SaveNextMessage(
                                    dual_sys,
                                send_info_ptr->send_content.dest_info.update_dest_addr,
                                GET_HEAD_MAX_NUM(&send_info_ptr->send_content),
                                (uint8)(GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1),
                                send_info_ptr);
                        if(MMISMS_NO_ERR != save_result)
                        {
                            if(MMISMS_NO_SPACE == save_result)
                            {
                                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, MMISMS_HandleSaveNextMessageAlertWin);
                                OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                            }
                            else
                            {
                                MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
                                CloseSendWinByResult(0);
                            }
                        }
                    }
                } 
                else//本条没有收到write_cnf
                {
                    // do nothing
                send_info_ptr->send_save_result.send_status = MMISMS_LONG_SMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXTPART;
                    is_need_init = FALSE;
#ifdef MMI_MSG_RESEND
                    MMISMS_ResendDelayed();
#endif
                }
            }
#ifndef MMI_DUALMODE_ENABLE
            else  if (MN_SMS_CANCEL_SUCCESS != cause)//不需要保存并且没有取消发送
#else
            else  if (!MMISMS_GetDeliverCancel())//不需要保存并且没有取消发送
#endif
            {
            if (MMISMS_BOX_SENDFAIL != send_info_ptr->cur_box_type)//发件箱中发送的短消息，发送失败后不保存
                {
                if(send_info_ptr->send_content.is_resend)
                    {
                        if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
                        {
                        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
                            // send to the next destination address
                        MMISMS_InitSendSaveResult(send_info_ptr);
                        if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))
                            {
                                OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                            }
                            else
                            {
                            //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                            //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);
                            }
                            //SCI_TRACE_LOW:"MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_530_112_2_18_2_53_17_341,(uint8*)"d", is_finish);
                        }
                        else
                        {
                            CloseSendWinByResult(0);
                            send_result = MMISMS_GetResultofSending(fail_id, &dest_count); 
                            MMISMS_SetDeliverResendFlag(FALSE);
                            MMISMS_ShowSendResult(send_result, fail_id);  
                        }
                    }
                    else
                    {
                        //保存长短信的第一条消息            
                        MMISMS_SetOperStatus(MMISMS_SAVE_SENDFAIL_SMS);
                    send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SAVE_SENDFAIL_SMS;
                        save_result = MMISMS_SaveNextMessage(
                                    dual_sys,
                                send_info_ptr->send_content.dest_info.update_dest_addr,
                                GET_HEAD_MAX_NUM(&send_info_ptr->send_content),
                                1,
                                send_info_ptr);
                        if(MMISMS_NO_ERR != save_result)
                        {
                            if(MMISMS_NO_SPACE == save_result)
                            {
                                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, MMISMS_HandleSaveNextMessageAlertWin);
                                OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                            }
                            else
                            {
                                MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
                                CloseSendWinByResult(0);
                            }
                       }                    
                    }
                }
                else
                {
                    //MMISMS_AppUpdateMoSms(dual_sys, MMISMS_UPDATE_SENDFAIL_SMS, MN_SMS_MO_TO_BE_SEND);
                    CloseSendWinByResult(0);
                    send_result = MMISMS_GetResultofSending(fail_id, &dest_count); 
                    MMISMS_ShowSendResult(send_result, fail_id); 
                    MMISMS_DeleteSendList();
                }
            }
#ifndef MMI_DUALMODE_ENABLE
            else if(MMISMS_GetDeliverCancel())
            {
            //MMISMS_InitSendSaveResult(FALSE);
                MMIPUB_OpenAlertSuccessWin(TXT_SMS_CANCEL_SENDING_SUCCESS);
                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);   
#ifdef MMIMMS_SMS_IN_1_SUPPORT 
            //MMIAPIMMS_CloseEditWin();
#else
            //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
#endif
                MMISMS_ClearOperInfo();
                MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
            MMISMS_DeleteSendList();
            }
#endif            
        }
        else//是普通短消息
        {
            if (is_need_save)
            {
            if(MN_SMS_NO_STORED != send_info_ptr->send_save_result.save_storage) //收到了WRITE_CNF
                {
                    is_finish =TRUE; 
                    MMISMS_SaveInfoToOrderByResult(
                    send_info_ptr->send_save_result.dual_sys,
                    send_info_ptr->send_save_result.save_storage, 
                    send_info_ptr->send_save_result.save_record_id,
                    MN_SMS_MO_TO_BE_SEND, TRUE, MMISMS_FOLDER_NORMAL, oper_status, send_info_ptr, MMISMS_SENDSTATUS_SENDFAIL
                    );
                send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDFAIL;
                
                    //发送下一条
                    if (is_need_send)
                    {
                        if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
                        {
                        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
                            // send to the next destination address
                        MMISMS_InitSendSaveResult(send_info_ptr);
                        if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))
                            {
                                OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                            }
                            else
                            {
                            //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                            //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);
                            }
                            //SCI_TRACE_LOW:"MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_625_112_2_18_2_53_18_342,(uint8*)"d", is_finish);
                        }
                        else
                        {
                            OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                        }
                    }
                    else
                    {
                        OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                    }            
                    
                }
                else
                {
                send_info_ptr->send_save_result.send_status = MMISMS_NO_RESEND_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT;
                    //DO NOTHING
                    is_need_init = FALSE;
#ifdef MMI_MSG_RESEND
                    MMISMS_ResendDelayed();
#endif
                }
            }
#ifndef MMI_DUALMODE_ENABLE
            else  if (MN_SMS_CANCEL_SUCCESS != cause)//不需要保存并且没有取消发送
#else
            else  if (!MMISMS_GetDeliverCancel())//不需要保存并且没有取消发送
#endif
            {
                //保存发送失败的普通短信
            if (MMISMS_BOX_SENDFAIL != send_info_ptr->cur_box_type)//发件箱中发送的短消息，发送失败后不保存
                {
                    if(send_info_ptr->send_content.is_resend)
                    {
                        if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
                        {
                        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
                            // send to the next destination address
                        MMISMS_InitSendSaveResult(send_info_ptr);
                        if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))
                            {
                                OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                            }
                            else
                            {
                            //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                            //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);
                            }
                            //SCI_TRACE_LOW:"MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_670_112_2_18_2_53_18_343,(uint8*)"d", is_finish);
                        }
                        else
                        {
                            CloseSendWinByResult(0);
                            send_result = MMISMS_GetResultofSending(fail_id, &dest_count); 
                            MMISMS_SetDeliverResendFlag(FALSE);
                            MMISMS_ShowSendResult(send_result, fail_id);
                            MMISMS_DeleteSendList();
                        }
                    }
                    else
                    {
                        MMISMS_SetOperStatus(MMISMS_SAVE_SENDFAIL_SMS);
                        send_info_ptr ->cur_send_data.cur_send_oper = MMISMS_SAVE_SENDFAIL_SMS;
                        save_result = MMISMS_SaveNextMessage(dual_sys, send_info_ptr->send_content.dest_info.update_dest_addr,1, 1, send_info_ptr);
                        SCI_TRACE_LOW("MMISMS: HandleSMSSendCnfErrorNoResend is not resend");
						if(MMISMS_NO_ERR != save_result)
                        {
                            OperAfterSendCnf(dual_sys, is_send_success, is_finish);
                        }
                    }
                }
                else
            {                
                //MMISMS_AppUpdateMoSms(dual_sys, MMISMS_UPDATE_SENDFAIL_SMS, MN_SMS_MO_TO_BE_SEND, send_info_ptr);
                    CloseSendWinByResult(0);
                    send_result = MMISMS_GetResultofSending(fail_id, &dest_count); 
                MMISMS_ShowSendResult(send_result, fail_id);
                MMISMS_DeleteSendList();
                }
            }
#ifndef MMI_DUALMODE_ENABLE
            else if(MMISMS_GetDeliverCancel())
            {
            //MMISMS_InitSendSaveResult(FALSE);
                MMIPUB_OpenAlertSuccessWin(TXT_SMS_CANCEL_SENDING_SUCCESS);
                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);   
#ifdef MMIMMS_SMS_IN_1_SUPPORT 
            //MMIIAPIMMS_CloseEditWin();
#else
            //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
#endif
                MMISMS_ClearOperInfo();
                MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
            MMISMS_DeleteSendList();
            }
#endif 
        }
        if (is_need_init)
        {
       // MMISMS_InitSendSaveResult(FALSE);
    }    
}

/*****************************************************************************/
// Description : to set the user data head for Vcard
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void InitUserDataHeadForVcard(int32                     srcPort,
                                    MN_SMS_USER_DATA_HEAD_T  *data_head_ptr)
{
    if (PNULL == data_head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:InitUserDataHeadForJAVA data_head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_734_112_2_18_2_53_18_344,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"mmivcard:InitUserDataHeadForVcard srcPort=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_737_112_2_18_2_53_18_345,(uint8*)"d", srcPort);
    data_head_ptr->length = MMISMS_HEAD_LENGTH_FOR_PORT;
    data_head_ptr->user_header_arr[ MMISMS_HEAD_IEI_POS ] = MMISMS_HEAD_IEI_FOR_PORT;
    data_head_ptr->user_header_arr[ MMISMS_HEAD_IEL_POS ] = MMISMS_HEAD_IEL_FOR_PORT;
    data_head_ptr->user_header_arr[ MMISMS_HEAD_SRC_PORTH_POS ] = (srcPort>>8)&(0x00ff);
    data_head_ptr->user_header_arr[ MMISMS_HEAD_SRC_PORTL_POS ] = srcPort&(0x00ff);
    data_head_ptr->user_header_arr[ MMISMS_HEAD_DST_PORTH_POS ] = 0;
    data_head_ptr->user_header_arr[ MMISMS_HEAD_DST_PORTL_POS ] = 0;
    return;
}

/*****************************************************************************/
//     Description : to handle the signal of APP_MN_SEND_SMS_CNF
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void HandleSMSSendCnf(
                            MN_DUAL_SYS_E   dual_sys,
                            MN_SMS_CAUSE_E    cause,    //IN:
                            MN_SMS_MO_FAIL_CAUSE_E rl_cause 
                            )
{
    MMISMS_SEND_T *send_info_ptr = PNULL;
    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleSMSSendCnf PNULL ==  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_763_112_2_18_2_53_18_346,(uint8*)"");
        return;
    }    
    //SCI_TRACE_LOW:"MMISMS: HandleSMSSendCnf cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_766_112_2_18_2_53_18_347,(uint8*)"d", cause);
    if (MN_SMS_OPERATE_SUCCESS == cause)
    {
        send_info_ptr->send_save_result.send_result = MMISMS_SEND_SUCC;
        HandleSMSSendSuccessCnf(dual_sys);       
    }
    else
    {
        HandleSMSSendErrorCnf(dual_sys, cause, rl_cause);        
    }

#ifdef MMI_AUTOTEST_SUPPORT
    if (MN_SMS_OPERATE_SUCCESS == cause)
    {
        MMI_TraceAutoTestOfMessage(TXT_COMM_SEND_SUCCEED);
    }
    else
    {
        MMI_TraceAutoTestOfMessage(TXT_COMM_SEND_FAILED);
    }
#endif /* MMI_AUTOTEST_SUPPORT */
}

/*****************************************************************************/
//     Description : to handle after received the send confirm,提示用户发送结果。
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void OperAfterSendCnf(
                            MN_DUAL_SYS_E dual_sys,
                            BOOLEAN is_send_success,
                            BOOLEAN    is_finish    //IN:
                            )
{
    // finish the process of send
    uint8 fail_id[MMISMS_DEST_ADDR_MAX_NUM] = {0};
    uint16 dest_count  = 0;
    uint16 send_result = 0;
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_ERR;
#ifdef MMI_TIMERMSG_SUPPORT
    MMIACC_SMART_EVENT_FAST_TABLE_T *cur_smart_even_info_ptr = MMISMS_TIMERMSG_GetCurMsgNode();
    BOOLEAN is_need_delete_msg = FALSE;
#endif
    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
        {
        //SCI_TRACE_LOW:"MMISMS:OperAfterSendCnf PNULL ==  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_803_112_2_18_2_53_18_348,(uint8*)"");
            return;
        }

#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg()
            && PNULL != cur_smart_even_info_ptr
            && (ALM_MODE_ONCE == cur_smart_even_info_ptr->fre_mode))
        {
            is_need_delete_msg = TRUE;
        }
#endif
    
    send_result = MMISMS_GetResultofSending(fail_id, &dest_count);
    
    if (MMISMS_SENDSAVE_SAVESUCC == send_info_ptr->backsend_update_data.save_send_result
        || MMISMS_SENDSAVE_SAVEFAIL == send_info_ptr->backsend_update_data.save_send_result)
    {//后台发送删除发件箱短信
        MN_SMS_RECORD_ID_T update_record_id = send_info_ptr->backsend_update_data.save_send_record_id[0];
        MMISMS_ORDER_ID_T      order_id = PNULL;
        BOOLEAN                ret_value = FALSE;
        
        ret_value = MMISMS_GetOrderIdByStorage(&order_id, MMISMS_TYPE_SMS, send_info_ptr->send_content.dual_sys, send_info_ptr->backsend_update_data.save_send_storage, update_record_id);

        if (ret_value && PNULL != order_id)
        {
            send_info_ptr->backsend_update_data.is_backsend_update = TRUE;
            send_info_ptr->backsend_update_data.backsend_send_oper = MMISMS_UPDATE_BACKSEND_SMS;
            send_info_ptr->backsend_update_data.backsend_update_info.current_num = 0;
            MMISMS_SetBackSendUpdateInfo(order_id, send_info_ptr);                        
            ret_val = MMISMS_AppUpdateMoSms(dual_sys, MMISMS_UPDATE_BACKSEND_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
            is_finish =  FALSE;
            g_mmisms_global.is_wait_update = TRUE;
        }        
    }
    else
    {
        if(MMISMS_BOX_SENDFAIL != send_info_ptr->cur_box_type &&  MMISMS_BOX_NOSEND != send_info_ptr->cur_box_type)
        {        
            MMISMS_ShowSendResult(send_result, fail_id);
            MMISMS_DeleteSendList();
            return;
        }
    }


#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg()
            && PNULL != cur_smart_even_info_ptr
            && (ALM_MODE_ONCE != cur_smart_even_info_ptr->fre_mode)
        )
    {
        MMISMS_ORDER_ID_T cur_order_id = PNULL;
        cur_order_id = MMISMS_GetCurOperationOrderId();   
        if(PNULL != cur_order_id)
        {
          cur_order_id->backsend_status = MMISMS_SENDSTATUS_NONE;
        }
    }
#endif

    if ((MMISMS_BOX_SENDFAIL == send_info_ptr->cur_box_type  
#ifdef MMI_TIMERMSG_SUPPORT        
        && !MMISMS_GetIsTimerMsg()
#endif
        )
#ifdef MMI_TIMERMSG_SUPPORT
        ||(MMISMS_GetIsTimerMsg())&&((MMISMS_BOX_SENDFAIL == send_info_ptr->cur_box_type))&&is_need_delete_msg
#endif
        ) //重发后删除发件箱中的信息
    {
        send_info_ptr->mosend_update_data.is_mosend_update = TRUE;
        send_info_ptr->mosend_update_data.mosend_send_oper = MMISMS_UPDATE_SENDFAIL_SMS;
        send_info_ptr->mosend_update_data.mosend_update_info.current_num = 0;
        ret_val = MMISMS_AppUpdateMoSms(dual_sys, MMISMS_UPDATE_SENDFAIL_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
        is_finish =  FALSE;
        g_mmisms_global.is_wait_update = TRUE;
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_NO_ERR != ret_val)
        {
            is_need_delete_msg = FALSE;
        }
#endif
    }
    
    if (MMISMS_BOX_NOSEND == send_info_ptr->cur_box_type && is_send_success) //发送成功，删除草稿箱中的信息
    {
        send_info_ptr->mosend_update_data.is_mosend_update = TRUE;
        send_info_ptr->mosend_update_data.mosend_send_oper = MMISMS_UPDATE_NOSEND_SMS;
        send_info_ptr->mosend_update_data.mosend_update_info.current_num = 0;
        ret_val = MMISMS_AppUpdateMoSms(dual_sys, MMISMS_UPDATE_NOSEND_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
        is_finish = FALSE;
        g_mmisms_global.is_wait_update = TRUE;
    }
    

    
    if (0 == send_result)
    {
        //处理发送成功 
        if((MMISMS_UPDATE_SENDFAIL_SMS != send_info_ptr->mosend_update_data.mosend_send_oper)
            && (MMISMS_UPDATE_NOSEND_SMS != send_info_ptr->mosend_update_data.mosend_send_oper)
            && (MMISMS_UPDATE_BACKSEND_SMS != send_info_ptr->backsend_update_data.backsend_send_oper))
        {
            MMISMS_ShowSendResult(send_result, fail_id);
            MMISMS_DeleteSendList();
        }

#ifdef MMI_TIMERMSG_SUPPORT
        if (!MMISMS_GetIsTimerMsg())
#endif
        {
            CloseSendWinByResult(send_result);
        }


        MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
#ifdef MMI_TIMERMSG_SUPPORT
        if (!is_need_delete_msg)
        {
            MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
        }
#ifdef MMI_PDA_SUPPORT
        if (MMK_IsOpenWin(MMISMS_MARK_OPT_WIN_ID))
        {
            MMK_CloseWin(MMISMS_MARK_OPT_WIN_ID);
        }
#endif
#endif
        if (is_finish)
        {
            MMISMS_ClearOperInfo();
        }
        return;
    }
    else
    {
        //处理发送失败的结果
        MMISMS_DEST_ADDR_INFO_T *dest_info_ptr = &(send_info_ptr->send_content.dest_info);
        
        if ( (1 == dest_count)                    // these is not send more destination
            && (!send_info_ptr->send_content.is_need_save)// not need to save
            )                
        {
            // to save these messages which have sent fail //delete by liming.deng //later
            MMISMS_SetOperStatus(MMISMS_SAVE_SENDFAIL_SMS);
            send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SAVE_SENDFAIL_SMS;
            ret_val = MMISMS_SaveNextMessage(dual_sys, dest_info_ptr->update_dest_addr,GET_HEAD_MAX_NUM(&send_info_ptr->send_content), 1, send_info_ptr);
            if (ret_val != MMISMS_NO_ERR)
            {
                MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
                CloseSendWinByResult(0);
#ifdef MMI_TIMERMSG_SUPPORT
                if (!is_need_delete_msg)
                {
                    MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
                }
#endif
            }
        }
        else
        {
            if((MMISMS_UPDATE_SENDFAIL_SMS != send_info_ptr->mosend_update_data.mosend_send_oper)
                && (MMISMS_UPDATE_NOSEND_SMS != send_info_ptr->mosend_update_data.mosend_send_oper)
                && MMISMS_UPDATE_BACKSEND_SMS != send_info_ptr->backsend_update_data.backsend_send_oper)
            {
                MMISMS_ShowSendResult(send_result, fail_id);
                MMISMS_DeleteSendList();
            }

#ifdef MMI_TIMERMSG_SUPPORT
            if (!MMISMS_GetIsTimerMsg())
#endif
            {
                CloseSendWinByResult(send_result);
            }
            MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
#ifdef MMI_TIMERMSG_SUPPORT
            if (!is_need_delete_msg)
            {
                MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
            }
#endif
            if (is_finish)
            {
                MMISMS_ClearOperInfo();
            }
            return;
        }
    }
    
    //accord the ret_val to display the prompt
    if (MMISMS_NO_ERR != ret_val)
    {
        if((MMISMS_UPDATE_SENDFAIL_SMS != send_info_ptr->mosend_update_data.mosend_send_oper)
            && (MMISMS_UPDATE_NOSEND_SMS != send_info_ptr->mosend_update_data.mosend_send_oper)
            && MMISMS_UPDATE_BACKSEND_SMS != send_info_ptr->backsend_update_data.backsend_send_oper)
        {
            MMISMS_ShowSendResult(send_result, fail_id);
            MMISMS_DeleteSendList();
        }

    }
    MMISMS_DisplayPrompByErr(ret_val, TXT_SENDING);
    
#ifdef MMI_TIMERMSG_SUPPORT
    if (!MMISMS_GetIsTimerMsg())
#endif
    {
        CloseSendWinByResult(send_result);
    }
#ifdef MMI_TIMERMSG_SUPPORT
    if (!is_need_delete_msg)
    {
        MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
    }
#endif
}

/*****************************************************************************/
//     Description : to send the pointed SMS to MN
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SendMsgReqToMN( 
                            MN_DUAL_SYS_E dual_sys,
                            MN_CALLED_NUMBER_T *dest_number_ptr,    //IN:
                            APP_SMS_USER_DATA_T *user_data_ptr,        //IN: the user data
                            MN_SMS_STORAGE_E storage,            //IN: the storage (SIM or NV)
                            MN_SMS_ALPHABET_TYPE_E alphabet_type,        //IN: the dcs
                            BOOLEAN is_more_msg,            //IN: the more message to send 
                            BOOLEAN is_resend,
                            BOOLEAN is_send_rpt
                            )
{
    MMISMS_OPER_ERR_E  send_result = MMISMS_NO_ERR;
    ERR_MNSMS_CODE_E mn_err_code = ERR_MNSMS_NONE;
    MN_SMS_MO_SMS_T mo_sms = {0};
    SCI_TIME_T time = {0};
    SCI_DATE_T date = {0};
    MN_SMS_PATH_E send_path = MN_SMS_GSM_PATH;
    MMISMS_INJECTION_FUNC p_injection_func = PNULL;
    BOOLEAN    ret = TRUE;
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    BOOLEAN reply_path = FALSE;
#endif
    if (PNULL == dest_number_ptr || PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:SendMsgReqToMN dest_number_ptr user_data_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1015_112_2_18_2_53_18_349,(uint8*)"");
        return send_result;
    }    
    //SCI_TRACE_LOW:"SMS: SendMsgReqToMN storage = %d, dual_sys = %d,is_resend=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1018_112_2_18_2_53_18_350,(uint8*)"ddd", storage, dual_sys, is_resend);
    
    SCI_MEMSET(&mo_sms, 0, sizeof(MN_SMS_MO_SMS_T));
    
    // the destination address
    mo_sms.dest_addr_t = *dest_number_ptr;
    mo_sms.dest_addr_t.number_plan = MN_NUM_PLAN_ISDN_TELE;
    mo_sms.dest_addr_present = TRUE;   
    // set the param of mo_sms
    mo_sms.dcs.alphabet_type = alphabet_type;
    mo_sms.dcs.class_is_present = FALSE;
    mo_sms.time_format_e = MN_SMS_TP_VPF_RELATIVE_FORMAT;
#ifndef MMI_SMS_MULTI_PARAM_SUPPORT
    mo_sms.time_valid_period.time_second = MMISMS_GetValidPeriod();
#endif
    mo_sms.status_report_is_request = is_send_rpt;

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    //reply path
    reply_path = MMISMS_GetReplyPathFlag();

    SCI_TRACE_LOW("SMS:SendMsgReqToMN reply_path=%d, reply_path_is_exist=%d, is_answer_sms=%d, global sc_addr_t_len=%d,global sc_addr_t=%s", 
        reply_path,  g_mmisms_global.read_msg.reply_path_is_exist, g_mmisms_global.is_answer_sms, g_mmisms_global.read_msg.sc_addr_t.num_len,g_mmisms_global.read_msg.sc_addr_t.party_num);

    SCI_TRACE_BUF("SMS:SendMsgReqToMN reply sca:", g_mmisms_global.read_msg.sc_addr_t.party_num, sizeof(g_mmisms_global.read_msg.sc_addr_t.party_num));

    if(reply_path
       && g_mmisms_global.read_msg.reply_path_is_exist
       && g_mmisms_global.is_answer_sms
      )
    {
        mo_sms.sc_addr_present = TRUE;
        mo_sms.sc_addr_t = g_mmisms_global.read_msg.sc_addr_t;
        mo_sms.reply_path_is_request = TRUE;
        MMISMS_SetIsAnswerSms(FALSE);
    }
    else
    {
        mo_sms.reply_path_is_request = FALSE;
    }
	SCI_TRACE_BUF("SMS:SendMsgReqToMN reply sca:", mo_sms.sc_addr_t.party_num, sizeof(mo_sms.sc_addr_t.party_num));
#endif

#ifndef MMI_SMS_MULTI_PARAM_SUPPORT
    mo_sms.pid_present = TRUE;
    mo_sms.pid_e = MN_SMS_PID_DEFAULT_TYPE;
#endif

    TM_GetSysTime(&time);
    TM_GetSysDate(&date);
    mo_sms.time_stamp_t.day = date.mday;
    mo_sms.time_stamp_t.hour = time.hour;
    mo_sms.time_stamp_t.minute = time.min;
    mo_sms.time_stamp_t.month = date.mon;
    mo_sms.time_stamp_t.second = time.sec;
    mo_sms.time_stamp_t.timezone = 0;    //待完善
    mo_sms.time_stamp_t.year = date.year - MMISMS_OFFSERT_YEAR;    //@cr116674  
    
    // user data
    MMISMS_EncodeMoSmsData(
        dual_sys,
        &mo_sms,
        alphabet_type,
        user_data_ptr
        );

    //设置GPRS优先，发送失败，重传用GSM
    if (is_resend && (MMISMS_PREFERRED_PATH_GPRS_PREFERRED == MMISMS_GetPreferredConnection()))
    {
        send_path = MN_SMS_GSM_PATH;
    }
    else
    {
        send_path = MMISMS_GetSendPath();
    }

    //SCI_TRACE_LOW:"MMISMS: SendMsgReqToMN is_more_msg=%d,send_path=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1087_112_2_18_2_53_18_351,(uint8*)"dd", is_more_msg, send_path);
    //inject function
    p_injection_func = MMISMSAPP_Get_Injection_Func();
    if(PNULL != p_injection_func)
    {
#ifdef UAL_RF_AM_SUPPORT
        ual_rf_am_sms_send_info_t sms_info = {0};
        sms_info.dual_sys = dual_sys;
        sms_info.storage = storage;
        sms_info.send_path = send_path;
        sms_info.is_more_msg = is_more_msg;
        SCI_MEMCPY(&(sms_info.mo_sms), &mo_sms, sizeof(MN_SMS_MO_SMS_T));
        ual_rf_am_set_mode(UAL_RF_AM_COMMUNICATION_MODE);
        SCI_TRACE_LOW("SendMsgReqToMN execute injection func");
        ret = (p_injection_func)(0, (void*)&sms_info, sizeof(ual_rf_am_sms_send_info_t));
        if(TRUE == ret)
        {
            SCI_TRACE_LOW("SendMsgReqToMN execute injection func finish");
            return MMISMS_NO_ERR;
        }
#endif
    }
    // call the MN API to send request
    mn_err_code = MNSMS_AppSendSmsEx(
        dual_sys,
        &mo_sms,
        storage,
        send_path,
        is_more_msg    // for cr66039(34.2.9.1_1900 fta fail)
        );

    //SCI_TRACE_LOW:"MMISMS: SendMsgReqToMN mn_err_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1098_112_2_18_2_53_18_352,(uint8*)"d", mn_err_code);
    if (ERR_MNSMS_NONE == mn_err_code)
    {
        send_result  = MMISMS_NO_ERR;
    }
    else if (ERR_MNSMS_FDN_SCA_FAILED == mn_err_code)
    {
        send_result = MMISMS_FDN_SCA_FAILED;
    }
    else if(ERR_MNSMS_FDN_DEST_NUM_FAILED == mn_err_code)
    {
        send_result = MMISMS_FDN_DEST_NUM_FAILED;
    }
    else
    {
        send_result = MMISMS_NO_SC_ADDRESS;
    }
    
    return (send_result);
}

#ifdef SXH_APPLE_SUPPORT
MMISMS_OPER_ERR_E APPLE_SendMsg( 
													 MN_DUAL_SYS_E            dual_sys,
													 MN_CALLED_NUMBER_T        *dest_number_ptr,    //IN:
													 APP_SMS_USER_DATA_T         *user_data_ptr,        //IN: the user data
													 MN_SMS_STORAGE_E            storage,            //IN: the storage (SIM or NV)
													 MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN: the dcs
													 BOOLEAN                    is_more_msg            //IN: the more message to send 
    )
{
	return SendMsgReqToMN(dual_sys,dest_number_ptr,user_data_ptr,storage,alphabet_type,is_more_msg,FALSE,FALSE);
}


#endif

#ifdef TTS_SUPPORT_YOUNGTONE
//send 销量msg
MMISMS_OPER_ERR_E yte_SendMsg( 
													 MN_DUAL_SYS_E            dual_sys,
													 MN_CALLED_NUMBER_T        *dest_number_ptr,    //IN:
													 APP_SMS_USER_DATA_T         *user_data_ptr,        //IN: the user data
													 MN_SMS_STORAGE_E            storage,            //IN: the storage (SIM or NV)
													 MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN: the dcs
													 BOOLEAN                    is_more_msg            //IN: the more message to send 
    )
{
	return SendMsgReqToMN(dual_sys,dest_number_ptr,user_data_ptr,storage,alphabet_type,is_more_msg,FALSE,FALSE);
}
#endif

/*****************************************************************************/
//     Description : to send next message or re_send the current message
//    Global resource dependence : g_mmisms_global
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_SendMsgBySeqNum(
                                                                                        MN_DUAL_SYS_E   dual_sys,
                                                                                        uint8  total_num,
                                                                                        uint8 seq_num,
                                                BOOLEAN is_resend,
                                                MMISMS_SEND_T *send_info_ptr
                                                                                        )
{
    MMISMS_OPER_ERR_E send_result = MMISMS_NO_ERR;
    MN_SMS_STORAGE_E  storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T record_id = 0;
    uint8 head_len = 0;
    MN_CALLED_NUMBER_T dest_num = {MN_NUM_TYPE_UNKNOW, MN_NUM_PLAN_UNKNOW, 0, 0};    

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SendMsgBySeqNum send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1153_112_2_18_2_53_19_353,(uint8*)"");
        return MMISMS_SENDSMS_FAIL;
    }
    send_info_ptr->cur_send_data.total_num = total_num;
    send_info_ptr->cur_send_data.seq_num = seq_num;
    send_info_ptr->send_content.is_resend = is_resend;

    head_len = send_info_ptr->send_content.tpdu_head.length;
    //SCI_TRACE_LOW:"MMISMS: MMISMS_SendMsgBySeqNum seq_num = %d, dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1161_112_2_18_2_53_19_354,(uint8*)"dd", seq_num, dual_sys);
    
    if ((0 < head_len) && (MMISMS_R8_LANGUAGE_NONE == send_info_ptr->send_content.send_message.language))
    {
        if (seq_num > GET_HEAD_MAX_NUM(&send_info_ptr->send_content))
    {
            //SCI_TRACE_LOW:"MMISMS:MMISMS_SendMsgBySeqNum seq_num > GET_HEAD_MAX_NUM()"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1167_112_2_18_2_53_19_355,(uint8*)"");
            return MMISMS_SENDSMS_FAIL;
        }
    }
    
    if (MMISMS_GetDestNumber(&dest_num, &send_info_ptr->send_content.dest_info))
    {
        MMISMS_SetSMSSaveInfo(dual_sys, &dest_num);

        send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SEND_SMS;
        
        // send the next message
        // get the content and head of the next SMS.
        if(send_info_ptr->send_content.is_need_save)
        {
            MMISMS_GetStorageAndRecordidForSendSms(send_info_ptr, &storage, &record_id);
        }
        send_info_ptr->cur_send_data.cur_send_storage = storage;
        send_info_ptr->cur_send_data.cur_send_record_id = record_id;
        if(send_info_ptr->is_sendfail_retry)  
        {
            send_info_ptr->resend_count = MMISMS_RESEND_MAX_COUNT;//长短信重新记数
        }
        
        if (send_info_ptr->send_content.is_need_save && is_resend)
        {
            //MMISMS_SetOperStatus(MMISMS_SEND_SAVE_SMS);
            send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SEND_SAVE_SMS;
        }
        if (send_info_ptr->send_content.is_need_save && !is_resend)
        {
            if (send_info_ptr->send_save_result.is_finished)
            {                
                if (MN_SMS_NO_STORED != storage)
                {
                    if (((0 < head_len) && !MMISMS_IsR8Language(send_info_ptr->send_content.send_message.language))
                        || ((MMISMS_R8_MAX_NORMAL_HEAD_LEN < head_len) && MMISMS_IsR8Language(send_info_ptr->send_content.send_message.language)))
                    {
                        g_mmisms_global.save_info.max_num = GET_HEAD_MAX_NUM(&send_info_ptr->send_content);
                    }
                    else
                    {
                        g_mmisms_global.save_info.max_num = 1;
                    }
                    g_mmisms_global.save_info.ref_num = GET_CURRENT_REF_NUM(&send_info_ptr->send_content);
                    
                    // set operation status
                    //MMISMS_SetOperStatus(MMISMS_SEND_SAVE_SMS);
                    send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SEND_SAVE_SMS;
                }
                else
                {
                    // if the storage space is not enough to save the SMS, send it only.
                    send_info_ptr->send_content.is_need_save = FALSE;
                }
            }
            else
            {
                if (MN_SMS_NO_STORED == storage)
                {
                    send_info_ptr->send_content.is_need_save = FALSE;
                }
                //MMISMS_SetOperStatus(MMISMS_SEND_SAVE_SMS);
                send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SEND_SAVE_SMS;
            }
        }

        if (!(1 < total_num && 1 < seq_num))//如果是长短信第二条，则不需要再次加入链表
        {
            send_result = MMISMS_AddSendList(send_info_ptr);
        }
        
        if(!send_result 
            && MMISMS_SENDSTATUS_SAVING != send_info_ptr->backsend_status
            && MMISMS_SENDSTATUS_WAITING != send_info_ptr->backsend_status )
        {
            MMISMS_ActiveSendList();
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SendMsgBySeqNum !MMISMS_GetDestNumber()"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1242_112_2_18_2_53_19_356,(uint8*)"");
        send_result = MMISMS_NO_DEST_ADDRESS;
    }
    
    return (send_result);
}

/*****************************************************************************/
//     Description : to close the other win after sent
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void CloseSendWinByResult(
                                uint8 fail_count
                                )
{
    //SCI_TRACE_LOW:"MMISMS: CloseSendWinByResult(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1257_112_2_18_2_53_19_357,(uint8*)"d", fail_count);
    
    //MMK_CloseWin(MMISMS_EDIT_WIN_ID);
#ifdef MMIMMS_SMS_IN_1_SUPPORT 
    //MMIAPIMMS_CloseEditWin();
#else
    //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
#endif
    MMK_CloseWin(MMISMS_SENDNUMOPTION_WIN_ID);
    MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
    MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
    MMK_CloseWin(MMISMS_EDITOPTION_WIN_ID);
    MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
}

/*****************************************************************************/
//     Description : to clear the dest addr in 
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ClearDestAddrById( void )
{
    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr = PNULL;    // the destation address structure
    MMISMS_SEND_T *send_info_ptr = PNULL;
    uint8 id = 0;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_ClearDestAddrById PNULL ==  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1286_112_2_18_2_53_19_358,(uint8*)"");
        return;
    }
    dest_info_ptr = &(send_info_ptr->send_content.dest_info);
    id = dest_info_ptr->dest_list.dest_addr_id;
    SCI_MEMSET( 
        dest_info_ptr->dest_list.dest_addr[id],
        0,
        (MMISMS_NUMBER_MAX_LEN + 1)
        );
}

/*****************************************************************************/
//     Description : to save the address for update
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SaveUpdateAddr( BOOLEAN is_edit)
{
    // the destation address structure
    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr = PNULL;           
    uint8 id  = 0;
    uint8 len = 0;
    uint8 copy_len = 0;

    MMISMS_SEND_T *send_info_ptr = PNULL;

    if(!is_edit)
    {
        send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
        if (PNULL != send_info_ptr)
        {
            dest_info_ptr = &send_info_ptr->send_content.dest_info;
        }
    }
    else
    {
        dest_info_ptr = &g_mmisms_global.edit_content.dest_info;
    }

    if (PNULL != dest_info_ptr)
    {
        SCI_MEMSET(dest_info_ptr->update_dest_addr, 0, sizeof (dest_info_ptr->update_dest_addr));    
    id = dest_info_ptr->dest_list.dest_addr_id;
    len = strlen((char *)dest_info_ptr->dest_list.dest_addr[id]);
        copy_len = MIN(len, MMISMS_NUMBER_MAX_LEN);    
        SCI_MEMCPY(dest_info_ptr->update_dest_addr, dest_info_ptr->dest_list.dest_addr[id], copy_len);
    }    
}

/*****************************************************************************/
//     Description : to set the count of fail by send group
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void SendFailCount( void )
{
    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:SendFailCount PNULL ==  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1347_112_2_18_2_53_19_359,(uint8*)"");
        return;
    }
    send_info_ptr->send_content.dest_info.failed_count++;
}

/*****************************************************************************/
//     Description : to get the dest addr 
//    Global resource dependence : , 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDestNumber(
                                    MN_CALLED_NUMBER_T    *dest_number_ptr,    //OUT:
                                    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr
                                    )
{
    BOOLEAN   ret_val = FALSE;
    uint8 addr_id = 0;
    uint8 *dest_ptr = PNULL;
    uint8 dest_len = 0;
    MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};   

    if (PNULL == dest_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetDestNumber dest_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1370_112_2_18_2_53_19_360,(uint8*)"");
        return FALSE;
    }
    
    if (PNULL == dest_number_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetDestNumber dest_number_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1376_112_2_18_2_53_19_361,(uint8*)"");
        return FALSE;
    }
        
    // init the destination number
    dest_number_ptr->num_len     = 0;
    dest_number_ptr->number_plan = MN_NUM_PLAN_UNKNOW;
    dest_number_ptr->number_type = MN_NUM_TYPE_UNKNOW;    
    
    addr_id = dest_info_ptr->dest_list.dest_addr_id;
    dest_ptr = dest_info_ptr->dest_list.dest_addr[addr_id];
    dest_len = strlen((char *)dest_ptr);
    
    if (dest_len > 0)
    {
        SCI_TRACE_LOW("MMISMS_GetDestNumber = %s", dest_ptr);
        MMISMS_GetMnCalledNum(dest_ptr, dest_len, &party_number, dest_number_ptr);
        SCI_TRACE_LOW("MMISMS_dest_number_ptr = %s", dest_number_ptr);
        ret_val = TRUE;
    }
    
    return (ret_val);
}

/*****************************************************************************/
//     Description : to split the long SMS 
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_SplitLongMsg(                                    //RETURN: the max number of message
                                 MMISMS_CONTENT_T    *sms_content_ptr,//IN: the content of need send
                                 BOOLEAN             is_head_has_port
                                 )
{
    uint8 limit_len = 0;
    uint8 split_len = 0;
    uint8 total_num = 0;
    
    if (PNULL == sms_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SplitLongMsg sms_content_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1413_112_2_18_2_53_19_362,(uint8*)"");
        return 0;
    }
    // get the limit condition according to the alphabet
    if(MMISMS_IsR8Language(sms_content_ptr->language))       
    {
        g_mmisms_global.is_r8 = TRUE;
        g_mmisms_global.is_single_shift = sms_content_ptr->is_single_shift;
    }
    else
    {
        g_mmisms_global.is_r8 = FALSE;
        g_mmisms_global.is_single_shift = FALSE;
    }
    
    MMISMS_GetLenByAlpha(sms_content_ptr->alphabet, &split_len, &limit_len, is_head_has_port);
    if (0 == split_len)
    {
        SCI_TRACE_LOW("MMISMS:MMISMS_SplitLongMsg 0 == split_len");
        return 0;
    }

    g_mmisms_global.is_r8 = FALSE;
    if (limit_len < sms_content_ptr->length)
    {
        if ((MN_SMS_DEFAULT_ALPHABET == sms_content_ptr->alphabet) ||
            (MN_SMS_8_BIT_ALPHBET == sms_content_ptr->alphabet))
        {
            uint32 start_pos = 0;
            uint32 split_index = 0;
            uint8 sms_length = 0;
            
            for (split_index = 0; split_index < MMISMS_SPLIT_MAX_NUM; split_index++)
            {
                sms_length = (uint8)MIN(split_len, (sms_content_ptr->length - start_pos));
                
                if (start_pos + sms_length < sms_content_ptr->length)
                {
                    if (DEFAULT_EXT_MARK == sms_content_ptr->data[start_pos + sms_length - 1])
                    {
                        start_pos = start_pos + sms_length - 1;
                    }
                    else
                    {
                        start_pos = start_pos + sms_length;
                    }
                }
                else
                {
                    split_index++;
                    break;
                }
            }
            
            total_num = split_index;
        }
        else
        {
            total_num = MMIAPICOM_CeilValue(sms_content_ptr->length, split_len);
        }
    }
    else
    {
        total_num = 1;            
    }
    
    // return the length of the max SMS
    return (total_num);
}

/*****************************************************************************/
//     Description : to check whether it has the next dest addr
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CheckNextDestAddr( void )
{
    BOOLEAN ret_val = FALSE;
    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr = PNULL;    // the destation address structure

    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_CheckNextDestAddr send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1491_112_2_18_2_53_19_363,(uint8*)"");
        return FALSE;
    }
    dest_info_ptr = &(send_info_ptr->send_content.dest_info);
    if (dest_info_ptr->dest_list.number > (dest_info_ptr->dest_list.dest_addr_id + 1))
    {
        ret_val = TRUE;
    }      
    //SCI_TRACE_LOW:"MMISMS: MMISMS_CheckNextDestAddr: the number is = %d, the addr id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1499_112_2_18_2_53_19_364,(uint8*)"dd",dest_info_ptr->dest_list.number,dest_info_ptr->dest_list.dest_addr_id);

    return ( ret_val );
}

/*****************************************************************************/
//     Description : to check whether it has the next SMS to send according to the 
//                head of user data
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsExistNextSMS( MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr )
{
    BOOLEAN is_next_sms = FALSE;
    MN_SMS_USER_DATA_HEAD_T *head_ptr = PNULL;
    uint8 i = 0;
    uint8 max_num = 0, seq_num = 0;
    uint8 iei_num = 0;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;

    if (PNULL == edit_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_IsExistNextSMS send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1521_112_2_18_2_53_19_365,(uint8*)"");
        return FALSE;
    }
    head_ptr = &(edit_content_ptr->tpdu_head);
    MNSMS_DecodeUserDataHeadEx(dual_sys, head_ptr, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            max_num = decode_head.iei_arr[i].iei_data_t[1];
            seq_num = decode_head.iei_arr[i].iei_data_t[2];
            //SCI_TRACE_LOW:"MMISMS_IsExistNextSMS:max_num is %d,seq_num is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1533_112_2_18_2_53_19_366,(uint8*)"dd",max_num,seq_num);
            if (max_num > seq_num)
            {
                is_next_sms = TRUE;
                break;
            }
        }
    }
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);
    //SCI_TRACE_LOW:"SMS: IsExistNextSMS %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1542_112_2_18_2_53_19_367,(uint8*)"d", is_next_sms);
    
    return (is_next_sms);
}

/*****************************************************************************/
//     Description : to set the user data head
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_InitUserDataHead(
                                    uint8 max_num,        //IN:
                                    uint8 seq_num,        //IN: the seq number (1 ~ 8)
                                    MN_SMS_USER_DATA_HEAD_T  *data_head_ptr,  //OUT: the user data head
                                    MMISMS_SEND_T *send_info_ptr
                                    )
{
    MMINV_SMS_REFERENCE_NUM_T ref_num = 0;
#ifdef MMI_R8_SUPPORT    
    MMISMS_CONTENT_T *sms_content_ptr = PNULL;
    uint16 content_len_r8 = 0;
#if 0
// no use
    MMISMS_R8_LANGUAGE_E language = MMISMS_R8_LANGUAGE_NONE;
#endif
    BOOLEAN is_single_shift = FALSE;
    wchar *content_r8_ptr = PNULL;
#endif   

    if (PNULL == data_head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InitUserDataHead data_head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1570_112_2_18_2_53_19_368,(uint8*)"");
        return;
    }
    if (0 == max_num || max_num > MMISMS_SPLIT_MAX_NUM)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InitUserDataHead 0 >= max_num || max_num > MMISMS_SPLIT_MAX_NUM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1575_112_2_18_2_53_19_369,(uint8*)"");
        return;
    }
    if (0 == seq_num || seq_num > max_num)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InitUserDataHead 0 >= seq_num || seq_num > max_num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1580_112_2_18_2_53_19_370,(uint8*)"");
        return;
    }    
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_InitUserDataHead(%d, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1584_112_2_18_2_53_19_371,(uint8*)"dd", max_num, seq_num);
    
    if( PNULL == send_info_ptr || (!send_info_ptr->is_head_has_port)) 
    {
        SCI_MEMSET( data_head_ptr, 0, sizeof(MN_SMS_USER_DATA_HEAD_T) );
    }
    
    if ( max_num > 1 )
    {
        if (1 == seq_num)
        {
            // get the refer number
            ref_num = g_mmisms_global.nv_param.ref_num ;
            MMISMS_SetRefNumToNV( (MMINV_SMS_REFERENCE_NUM_T)( ref_num + 1 ) );
        }
        
        // set the head of user data according to spec
        SET_HEAD_VAR( data_head_ptr, ref_num,  max_num, seq_num );               
    }

#ifdef MMI_R8_SUPPORT  
    content_r8_ptr = SCI_ALLOCAZ( ( MMISMS_R8_MESSAGE_LEN + 1 )*sizeof(wchar) );
    if(PNULL == content_r8_ptr)
    {
        return;
    }    
    //SCI_MEMSET(content_r8_ptr,0x0,(sizeof(wchar) * ( MMISMS_R8_MESSAGE_LEN + 1 )));
    if (PNULL != send_info_ptr)
    {
        sms_content_ptr = &send_info_ptr->send_content.send_message;
    }
    else
    {
        if(g_mmisms_global.is_save_draft)  //保存草稿
        {
            SCI_TRACE_LOW("MMISMS:MMISMS_InitUserDataHead save draft");
            g_mmisms_global.is_save_draft = FALSE;
            sms_content_ptr = &g_mmisms_global.edit_content.send_message;
        }
        else  //复制，移动
        {
            sms_content_ptr = &g_mmisms_global.read_msg.read_content;
        }        
    }
    if(MN_SMS_DEFAULT_ALPHABET == sms_content_ptr->alphabet)
    {
        if((MMISMS_R8_LANGUAGE_TURKISH == sms_content_ptr->language) ||
            (MMISMS_R8_LANGUAGE_SPANISH == sms_content_ptr->language) ||
            (MMISMS_R8_LANGUAGE_PORTUGUESE == sms_content_ptr->language))    
        {
     	     BOOLEAN is_shift_lock = TRUE;
     	     if (sms_content_ptr->language == MMISMS_R8_LANGUAGE_SPANISH)
     	     {
     	         is_shift_lock = FALSE;
     	     }
        
            content_len_r8 = MMISMS_Default2NationalR8(
                                        sms_content_ptr->data,
                                        content_r8_ptr,
                                        sms_content_ptr->length,
                                        sms_content_ptr->language,
                                        is_shift_lock,
                                        sms_content_ptr->is_single_shift
                                        );

#if 0
// no use
            language = MMISMS_R8_DecodeContentForLanguage(content_r8_ptr, content_len_r8, &is_single_shift);
#else
            MMISMS_R8_DecodeContentForLanguage(content_r8_ptr, content_len_r8, &is_single_shift);
#endif
            SET_HEAD_R8_VAR( data_head_ptr, sms_content_ptr->language, is_single_shift );     
            
        }
    }
    else 
    {
        //SCI_TRACE_LOW:"mmisms send_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1641_112_2_18_2_53_20_372,(uint8*)"");
        //to do
    }
    SCI_FREE(content_r8_ptr);
    content_r8_ptr = PNULL;
#endif    
}

/*****************************************************************************/
//     Description : to move the id to next addr
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MoveDestAddrId( void )
{
    BOOLEAN ret_val = FALSE;
    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr    = PNULL;    // the destation address structure

    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_MoveDestAddrId send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1663_112_2_18_2_53_20_373,(uint8*)"");
        return FALSE;
    }
    dest_info_ptr    = &(send_info_ptr->send_content.dest_info);   
        dest_info_ptr->dest_list.dest_addr_id++;
        ret_val = TRUE;
    
    return ( ret_val );
}

/*****************************************************************************/
//     Description : to get the destinations which are fail to sent
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetResultofSending(                        //RETURN: the number failed to send
                                        uint8   *status_id_ptr, //OUT: the id of fail to send 
                                        uint16    *dest_count_ptr    //OUT: 
                                        )
{
    int16 i = 0;
    uint16 num = 0;    
    MMISMS_DEST_ADDR_INFO_T dest_info = {0};

    MMISMS_SEND_T *send_info_ptr = PNULL;    
    
    if (PNULL == status_id_ptr || PNULL == dest_count_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetResultofSending status_id_ptr dest_count_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1690_112_2_18_2_53_20_374,(uint8*)"");
        return 0;
    }  

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetResultofSending send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1697_112_2_18_2_53_20_375,(uint8*)"");
        return 0;
    }
    
    dest_info = send_info_ptr->send_content.dest_info;
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetResultofSending dest_addr_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1702_112_2_18_2_53_20_376,(uint8*)"d", dest_info.dest_list.dest_addr_id);
    for (i=0; i<dest_info.dest_list.number; i++)
    {
        if (0 < strlen((char *)dest_info.dest_list.dest_addr[i]))
        {
            status_id_ptr[num] = i;
            num++;
        }
    }
    
    *dest_count_ptr = dest_info.dest_list.number;
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetResultofSending: list_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1714_112_2_18_2_53_20_377,(uint8*)"d", num);
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetResultofSending: dest_count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1715_112_2_18_2_53_20_378,(uint8*)"d", (*dest_count_ptr));
    
    return (num);
}

/*****************************************************************************/
//     Description : clear resend count
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ClearResendCount(MMISMS_SEND_T *send_info_ptr)
{
    MMISMS_PREFERRED_PATH_E preferred_path = MMISMS_PREFERRED_PATH_GSM_PREFERRED;

    if (PNULL == send_info_ptr)
    {
        return;
    }

    preferred_path = MMISMS_GetPreferredConnection();

    switch(preferred_path)
    {
    case MMISMS_PREFERRED_PATH_GPRS_PREFERRED://设置GPRS优先重传次数为1
        send_info_ptr->resend_count = MMISMS_SEND_PATH_RESEND_COUNT;
        break;

    case MMISMS_PREFERRED_PATH_GSM_PREFERRED:
    case MMISMS_PREFERRED_PATH_GSM_ONLY:
    case MMISMS_PREFERRED_PATH_GPRS_ONLY:
        if(!send_info_ptr->send_content.is_resend)
        {
            send_info_ptr->resend_count = 0;
        }
        else
        {
            send_info_ptr->resend_count = MMISMS_RESEND_MAX_COUNT;
        }
        break;

    default:
        if(!send_info_ptr->send_content.is_resend)
        {
            send_info_ptr->resend_count = 0;
        }
        else
        {
            send_info_ptr->resend_count = MMISMS_RESEND_MAX_COUNT;
        }
        break;
    }
}

/*****************************************************************************/
//     Description : to get the result of send
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowSendResult(
                                  uint8        fail_count,
                                  uint8        *fail_id_ptr
                                  )
{
    //int16 i = 0;
    //int16 j = 0;
    //MMI_STRING_T prompt_str1 = {0};
    //MMI_STRING_T prompt_str2 = {0};
    //uint8 disp_str_len = 0;
    //uint8 disp_num_len = 0;
    //uint8 fail_num[6] = {0};
    //wchar disp_num[10]= {0};
    //MMI_STRING_T prompt_str = {0};
    //MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    
    if (PNULL == fail_id_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_ShowSendResult fail_id_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1789_112_2_18_2_53_20_379,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMISMS: MMISMS_ShowSendResult  fail_count = %d, id_ptr = %p "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1792_112_2_18_2_53_20_380,(uint8*)"dd", fail_count, fail_id_ptr);

    MMIDEFAULT_TurnOnBackLight();

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Add for CR 103433
    MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim(MMISMS_GetCurSendDualSys()));        
#endif

    if (MMISMS_GetDeliverCancel())//取消发送不提示结果
    {
        return;
    }

#ifdef MET_MEX_SUPPORT    
    if(MMIMEX_SendSmsCB((uint16)fail_count))
    {
        return;
    }
#endif    
#ifndef ADULT_WATCH_SUPPORT
    if (0 == fail_count)
    {
        // success
        if(MMISMS_PRESETFROM_CALL != MMISMS_GetPresetFrom())
        {
            MMISMS_OpenNoteWin( MMISMS_NOTE_WIN_ID,TXT_COMM_SEND_SUCCEED,0 );
        }
    }
    else
    {
        if (MMK_IsOpenWin(MMISMS_SENDNUMOPTION_WIN_ID))    // update the listbox in this window
        {
            MMK_SendMsg(MMISMS_SENDNUMOPTION_WIN_ID,MMISMS_MSG_UPDATE_NUMBER, PNULL);
        }
        MMISMS_OpenNoteWin( MMISMS_NOTE_WIN_ID,TXT_COMM_SEND_FAILED,0 );
        //SCI_FREE(prompt_str.wstr_ptr);
        //prompt_str.wstr_ptr = PNULL;
    }
#endif
#ifdef MMI_SMS_MINI_SUPPORT
        CloseSendWinByResult(0);
        MMISMS_ClearEditContent();
#endif
}

/*****************************************************************************/
//     Description : to handle the signal of APP_MN_SEND_SMS_CNF
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSendCnf(
                                 DPARAM param
                                 )
{
    APP_MN_SEND_SMS_CNF_T *sig_ptr = (APP_MN_SEND_SMS_CNF_T *)param;
    //MMISMS_OPER_STATUS_E oper_status = MMISMS_GetCurrentOperStatus();  
    MMISMS_OPER_STATUS_E oper_status = MMISMS_NO_OPER;
    MMISMS_SEND_T           *send_info_ptr = PNULL;
    BOOLEAN is_send_end = FALSE;        //使用给其他的模块发送短消息用，如果认为结束可以使用回调结束流程，如果没有结束可以继续发送
    BOOLEAN is_succss = FALSE;
    
    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSendCnf sig_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1890_112_2_18_2_53_20_381,(uint8*)"");
        return;
    }

#ifdef JAVA_SUPPORT_SUN
#ifndef _MSC_VER
    MMISMS_SUNJavaSendCnfCallback(sig_ptr->cause);    
#endif
#endif

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
    //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSendCnf sig_ptr->cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1901_112_2_18_2_53_20_382,(uint8*)"d", sig_ptr->cause);

    if (MN_SMS_OPERATE_SUCCESS == sig_ptr->cause)
    {
        is_succss = TRUE;
    }
    else
    {
        is_succss = FALSE;
    }
    //Add for cc reject call by sms callback
    if(MMISMS_PRESETFROM_CALL == MMISMS_GetPresetFrom())
    {
        MMISMSAPP_HandleSendCnf(is_succss);
    }

    if (PNULL != send_info_ptr && (MN_SMS_OPERATE_SUCCESS != sig_ptr->cause   //发送失败认为结束
        || 1 == GET_HEAD_MAX_NUM(&send_info_ptr->send_content)     // 如果是单条，不管成功失败认为结束了
        || 0 == GET_HEAD_MAX_NUM(&send_info_ptr->send_content)
        || (GET_HEAD_MAX_NUM(&send_info_ptr->send_content) == GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content))//多条的时候，如果最后一条认为结束
        ))
    {
        is_send_end = TRUE;
    }
    if (is_send_end && PNULL != send_info_ptr && PNULL != send_info_ptr->send_callback)
    {//to do discuss
        send_info_ptr->send_callback(is_succss, PNULL);
        MMISMS_DeleteSendList();//处理下一条发送流程
#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(FALSE);
#endif
        return;
    }
    if (PNULL != send_info_ptr)
    {
        oper_status = send_info_ptr->cur_send_data.cur_send_oper;
    }    
    //短消息模块和其他模块没有发送完的时候走统一的流程.其他模块失败就停止了，即使多条也停止,后面的设置也就没有必要了
        switch (oper_status) 
        {
        case MMISMS_SEND_SMS:
        case MMISMS_SEND_OTHER:
        case MMISMS_SEND_SAVE_SMS:
            HandleSMSSendCnf(sig_ptr->dual_sys, sig_ptr->cause,sig_ptr->rl_cause);
            break;
            
        default:
            //调用STK的发送confirm的处理
        MMIAPISTK_SendSMSPDUCnf(sig_ptr->dual_sys, sig_ptr->cause);           
            break;
        }
}

/*****************************************************************************/
//     Description : to send the SMS ,called by window
//    Global resource dependence : , 
//                               g_mmisms_global,
//  Author:louis.wei
//    Note: use the g_mmisms_global.read_msg to save the content need to send
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppSendSms(
                                           MN_DUAL_SYS_E    dual_sys,
                                           BOOLEAN        is_need_save,    //IN:
                                           MMISMS_SEND_T *send_info_ptr
                                           )
{    
    uint8 total_num    = 1;
    MMISMS_OPER_ERR_E send_result = MMISMS_NO_DEST_ADDRESS;
    
    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_AppSendSms send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1964_112_2_18_2_53_20_383,(uint8*)"");
        return MMISMS_SENDSMS_FAIL;
    }
    
    send_info_ptr->is_deliver_cancel = FALSE;
    
    // split the long message and get the total number of SMS   
   if (PNULL == send_info_ptr->send_callback)//normal sms    
   {
        SCI_MEMSET( &send_info_ptr->send_content.tpdu_head, 0, sizeof( MN_SMS_USER_DATA_HEAD_T ) );
   } 
    
    SCI_MEMSET( &g_mmisms_global.save_info , 0, sizeof( MMISMS_SAVE_INFO_T ) );
    
    // save the status of operation
    send_info_ptr->send_content.is_need_save = ( send_info_ptr->send_content.is_need_save || is_need_save );
    //SCI_TRACE_LOW:"MMISMS: MMISMS_AppSendSms is_need_save = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1980_112_2_18_2_53_20_384,(uint8*)"d", send_info_ptr->send_content.is_need_save);
    
    total_num = MMISMS_SplitLongMsg( &send_info_ptr->send_content.send_message, send_info_ptr->is_head_has_port);
    //SCI_TRACE_LOW:"MMISMS: MMISMS_AppSendSms total_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1983_112_2_18_2_53_20_385,(uint8*)"d", total_num);

    // set the user data head
    MMISMS_InitUserDataHead(total_num, 1, &send_info_ptr->send_content.tpdu_head, send_info_ptr);

    send_result = MMISMS_SendMsgBySeqNum(dual_sys,total_num,1,FALSE, send_info_ptr);        
    
    MMISMS_CLEAR_OPER(send_result);
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_AppSendSms send_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1992_112_2_18_2_53_20_386,(uint8*)"d", send_result);
    
    return (send_result);
}

/*****************************************************************************/
//     Description : hanlde when receive write_cnf after send_cnf
//    Global resource dependence : , 
//                               g_mmisms_global,
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
void MMISMS_HandleSendCnfWhenWriteCnf( MMISMS_SEND_STATUS_E send_status)
{
    BOOLEAN is_send_success = TRUE;
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_ERR;    
    MMISMS_SEND_T           *send_info_ptr = PNULL;

    //SCI_TRACE_LOW:" MMISMS_HandleSendCnfWhenWriteCnf: send_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2008_112_2_18_2_53_20_387,(uint8*)"d",send_status);
    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSendCnfWhenWriteCnf send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2013_112_2_18_2_53_20_388,(uint8*)"");
        return;
    }
    
    switch(send_status)
    {
    case MMISMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT:
        send_info_ptr->send_save_result.is_finished = TRUE;       
        send_info_ptr->send_save_result.save_storage = MN_SMS_NO_STORED;
        if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content))   //发送长短信的下一条
        {//发送长短信可以把状态设置成none，发送下一条
            // send the next message to the same destination
            MMISMS_SendMsgBySeqNum(send_info_ptr->send_save_result.dual_sys,GET_HEAD_MAX_NUM(&send_info_ptr->send_content), GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1, FALSE, send_info_ptr);
        }
        else  //普通短信
        {//处理发送完成
            if (0 == strlen((char *)send_info_ptr->send_content.dest_info.update_dest_addr))
        {
                MMISMS_SaveUpdateAddr(FALSE);
            }
            
            // clear the destination address which send successfully
            MMISMS_ClearDestAddrById();
            
            if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
            {//如果有多个地址，发送下一个地址
                // to send message to next destination address
                MMISMS_InitSendSaveResult(send_info_ptr);
                if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(send_info_ptr->send_save_result.dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))  //发送到下一个号码
                {
                    OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
                }
                else
                {
                    //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                    //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);                      
                }
            }
            else
            {//结束发送六层
                OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
            }
        }
        break;
        
    case MMISMS_LONG_SMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXTPART:
        //长短信发送失败，不管该条长短信是否发送完都不发送了 
        if (!MMISMS_IsExistNextSMS(&send_info_ptr->send_content)) //是长短信的最后一条
        { 
            if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
            {            
                // send to the next destination address
                MMISMS_InitSendSaveResult(send_info_ptr);
                if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(send_info_ptr->send_save_result.dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))
                {
                    OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
                }
                else
                {
                    //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                    //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);                       
                }
            }
            else
            {
                OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
            }
            
        }
        else
       {//长短信如果第一条发送失败后面没有必要发送了，但是要保存，不然已发信箱没有后面的了
 //           OperAfterSendCnf(send_info_ptr->dual_sys, is_send_success, TRUE);
            //置保存、控制位为纯保存，保存下一条为草稿
            MMISMS_SetOperStatus(MMISMS_SAVE_SENDFAIL_SMS);
            send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SAVE_SENDFAIL_SMS;
            save_result = MMISMS_SaveNextMessage(
                        send_info_ptr->send_save_result.dual_sys,
                        send_info_ptr->send_content.dest_info.update_dest_addr,
                        GET_HEAD_MAX_NUM(&send_info_ptr->send_content),
                        (uint8)(GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1),
                        send_info_ptr);
            if (MMISMS_NO_ERR != save_result)
            {
                if(MMISMS_NO_SPACE == save_result)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, MMISMS_HandleSaveNextMessageAlertWin);
                }
                else
                {
                    MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
                    CloseSendWinByResult(0);     
                }
            }
        }
        break;
        
    case MMISMS_LONG_SMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT:
        send_info_ptr->send_save_result.is_finished = TRUE;
        send_info_ptr->send_save_result.save_storage = MN_SMS_NO_STORED;
        if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
        {            
            // send to the next destination address
            MMISMS_InitSendSaveResult(send_info_ptr);
            if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(send_info_ptr->send_save_result.dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))
            {
                OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
            }
            else
            {
                //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);                        
            }
        }
        else
        {
            OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
        }        
        break;
        
    case MMISMS_NO_RESEND_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT:
        send_info_ptr->send_save_result.is_finished = TRUE;
        //发送下一条
        send_info_ptr->send_save_result.save_storage = MN_SMS_NO_STORED;
        if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
        {            
            // send to the next destination address
            MMISMS_InitSendSaveResult(send_info_ptr);
            if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(send_info_ptr->send_save_result.dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))
            {
                OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
            }
            else
            {
                //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);                      
            }
        }
        else
        {
            OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
        }        
        break;
        
    default:
        
        break;
    }
    send_info_ptr->send_save_result.send_status = MMISMS_NO_SEND_STATUS;   //发送后清除状态
}

/*****************************************************************************/
//     Description : to init the global
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitSendSaveResult(MMISMS_SEND_T *send_info_ptr)
{
    MMISMS_SEND_SAVE_RESULT_T *send_save_result_ptr =PNULL; 

    if (PNULL == send_info_ptr)
    {
        return;
    }
    send_save_result_ptr = &(send_info_ptr->send_save_result);
    
    send_save_result_ptr->is_finished = TRUE;
    send_save_result_ptr->send_result = MMISMS_WAIT_RESULT;
    send_save_result_ptr->save_storage = MN_SMS_NO_STORED;
    send_save_result_ptr->save_record_id = 0;
}

/*****************************************************************************/
//     Description : to clear the id of destinations in g_mmisms_deliver
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ClearDestId(void)
{
    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr = PNULL;

    dest_info_ptr = &(g_mmisms_global.edit_content.dest_info);
    dest_info_ptr->dest_list.dest_addr_id = 0;
}

/*****************************************************************************/
//     Description : to clear the all destinations in g_mmisms_deliver
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void    MMISMS_ClearDestNum( void )
{    
    SCI_MEMSET( &g_mmisms_global.edit_content.dest_info, 0, sizeof( MMISMS_DEST_ADDR_INFO_T ) );
}

//#ifdef MMI_SMS_MINI_SUPPORT
/*****************************************************************************/
//     Description : to clear the all sms content edit
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void    MMISMS_ClearEditContent( void )
{    
    SCI_MEMSET( &g_mmisms_global.edit_content.send_message, 0, sizeof( MMISMS_CONTENT_T ) );
}
//#endif

/*****************************************************************************/
//     Description : to add the phone number to the global variable
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetAddressToMessage(
                              const uint8        *num_ptr,    //IN:
                                       uint8            num_len,        //IN:
                                       MMISMS_NUMBER_LIST_T *dest_list_ptr
                              )
{
    uint8 count = 0;
    BOOLEAN is_exit_same_num = FALSE;
    int32 i = 0;
    uint16 addr_len = 0;
    
    if (PNULL == num_ptr || PNULL == dest_list_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SetAddressToMessage num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2221_112_2_18_2_53_21_389,(uint8*)"");
        return;
    }
    count = dest_list_ptr->number;
    
    //SCI_TRACE_LOW:"MMISMS:MMISMS_SetAddressToMessage count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2226_112_2_18_2_53_21_390,(uint8*)"d", count);
    for(i = 0; i< count; i++)
    {
        uint16 len = 0;
        addr_len = SCI_STRLEN((char *)dest_list_ptr->dest_addr[i]);

        if (num_len == addr_len)
        {
            len = MIN(num_len,  MMISMS_NUMBER_MAX_LEN);

            if ( 0 == memcmp((char *)dest_list_ptr->dest_addr[i], (char *)num_ptr, len))/*lint !e670*/
            {
                is_exit_same_num = TRUE;
                break;
            }
        }
    }
    
    if ((count < MMISMS_DEST_ADDR_MAX_NUM) && (0 < num_len) && (!is_exit_same_num))
    {
        uint16 len = MIN(num_len, MMISMS_NUMBER_MAX_LEN);
        SCI_MEMCPY(      
            dest_list_ptr->dest_addr[count], 
            num_ptr,
            len
            );  
        
        dest_list_ptr->number = count + 1;
    }
}

/*****************************************************************************/
//     Description : to get the amount of the destination number
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint8   MMISMS_GetDestCount( void )
{
    uint8 count = 0;
    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr = PNULL;    // the destation address structure

    dest_info_ptr = &(g_mmisms_global.edit_content.dest_info);

    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetDestCount count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2268_112_2_18_2_53_21_391,(uint8*)"d",dest_info_ptr->dest_list.number);
    count = dest_info_ptr->dest_list.number ;    
    return (count);
}

/*****************************************************************************/
//     Description : to get the destation number according the pointed position
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetPointedDestNum(                            //RETURN: 
                                        uint16     index,            //IN: start from 0
                                        uint8        *num_len_ptr,    //OUT:
                                        uint8        *num_ptr        //OUT:
                                        )
{
    BOOLEAN result = FALSE;
    uint8 str_len = 0;
    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr = PNULL;    // the destation address structure
    
    if (PNULL == num_len_ptr || PNULL == num_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetPointedDestNum num_len_ptr num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2289_112_2_18_2_53_21_392,(uint8*)"");
        return FALSE;
    }

    dest_info_ptr = &g_mmisms_global.edit_content.dest_info;
    if (index < dest_info_ptr->dest_list.number)
    {
        str_len         = SCI_STRLEN((char *)dest_info_ptr->dest_list.dest_addr[index]);
        *num_len_ptr = MIN(str_len, MMISMS_NUMBER_MAX_LEN);
        SCI_MEMCPY( 
            num_ptr, 
            dest_info_ptr->dest_list.dest_addr[index],
            *num_len_ptr 
            );
        result = TRUE;
    }
    else
    {
        *num_len_ptr = 0;
        result         = FALSE;
    }
    
    return (result);
}

/*****************************************************************************/
//     Description : to delete the destination number according the pointed position
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteDestNum(
                                    uint16    pos // start from 0
                                    )
{
    BOOLEAN result = FALSE;
    uint8 num = 0;
    uint16 i = 0;
    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr = PNULL;    // the destation address structure

    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_DeleteDestNum send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2333_112_2_18_2_53_21_393,(uint8*)"");
        return FALSE;
    }
    dest_info_ptr = &(send_info_ptr->send_content.dest_info);
    num = dest_info_ptr->dest_list.number;
    
    if ((pos < num)&&(num > 0))
    {
        for (i=pos; i<num-1; i++)
        {
            SCI_MEMCPY( 
                dest_info_ptr->dest_list.dest_addr[i],
                dest_info_ptr->dest_list.dest_addr[i + 1],
                MMISMS_NUMBER_MAX_LEN+1
                );
        }
        
        // clear the last destination address
        SCI_MEMSET(
            dest_info_ptr->dest_list.dest_addr[num-1],
            0,
            (MMISMS_NUMBER_MAX_LEN+1)
            );
        
        // decrease the total number.
        send_info_ptr->send_content.dest_info.dest_list.number--;
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//     Description : to set the save flag
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSaveFlag( 
                               BOOLEAN     is_save
                               )
{
    g_mmisms_global.edit_content.is_need_save = is_save;
}

/*****************************************************************************/
//     Description : to get the save flag
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetSaveFlag(void)
{
    return (g_mmisms_global.edit_content.is_need_save);
}

/*****************************************************************************/
//  Description : to get the resend flag
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDeliverResendFlag(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetDeliverResendFlag: g_mmisms_global.edit_content.is_resend = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2392_112_2_18_2_53_21_394,(uint8*)"d",g_mmisms_global.edit_content.is_resend);
    return (g_mmisms_global.edit_content.is_resend);
}

/*****************************************************************************/
//  Description : to set the resend flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetDeliverResendFlag( 
                               BOOLEAN     is_resend
                               )
{
    //SCI_TRACE_LOW:"MMISMS_SetDeliverResendFlag: is_resend = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2404_112_2_18_2_53_21_395,(uint8*)"d",is_resend);
    g_mmisms_global.edit_content.is_resend = is_resend;
}
/*****************************************************************************/
//     Description : to get the content of message
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void  MMISMS_GetSMSContent( 
                                    MMI_STRING_T  *string_ptr
                                    )
{
    MMISMS_CONTENT_T *msg_content_ptr = PNULL;
//    uint16 content_len = 0;
//    uint32 i = 0;
//    uint16 data_offset = 0;


    msg_content_ptr = &(g_mmisms_global.read_msg.read_content);

    MMISMS_GetSMSStringFromContent( msg_content_ptr,string_ptr);


}

/*****************************************************************************/
//     Description : to get the content of message
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetSMSStringFromContent( MMISMS_CONTENT_T        *msg_content_ptr,
                                        MMI_STRING_T  *string_ptr
                                        )
{
    uint16 content_len = 0;
    uint32 i = 0;
    uint16 data_offset = 0;
    
    if (PNULL == string_ptr||msg_content_ptr ==PNULL)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSMSContent string_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2423_112_2_18_2_53_21_396,(uint8*)"");
        return FALSE;
    }
 
    string_ptr->wstr_ptr = SCI_ALLOCAZ((msg_content_ptr->length + 1)*sizeof(wchar));
    if(PNULL == string_ptr->wstr_ptr)
    {
        return FALSE;
    }
    for (i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
    {
        if (0 != msg_content_ptr->split_sms_length[i])
        {
            content_len = 0;
            MMISMS_ConvertValidDataToDispStr(
                                       string_ptr->wstr_ptr + string_ptr->wstr_len,
                                       &content_len,
                                       msg_content_ptr->data + data_offset,
                                       MIN(MMISMS_R8_MESSAGE_LEN,msg_content_ptr->split_sms_length[i]),
                                       msg_content_ptr->alphabet_ori[i],
                                       msg_content_ptr->language,
                                       TRUE,
                                       msg_content_ptr->is_single_shift
                                    );
            string_ptr->wstr_len += content_len;
            data_offset += msg_content_ptr->split_sms_length[i];
        }
    }    
    //SCI_MEMSET(string_ptr->wstr_ptr, 0x00, ((msg_content_ptr->length + 1)*sizeof(wchar)));
    return TRUE;
}

/*****************************************************************************/
//     Description : to save the message to global var
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetReadMsgTOSend( MMISMS_SEND_T *send_info_ptr )
{
    wchar *string = PNULL;

    if (PNULL == send_info_ptr)
    {
        return;
    }
    
    SCI_MEMCPY(&send_info_ptr->send_content.send_message, &(g_mmisms_global.read_msg.read_content), sizeof(MMISMS_CONTENT_T));
    send_info_ptr->send_content.send_message.language = g_mmisms_global.read_msg.language;

    if(MN_SMS_UCS2_ALPHABET == send_info_ptr->send_content.send_message.alphabet)
    {
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
		//do nothing
#else
        string = SCI_ALLOCAZ( ( send_info_ptr->send_content.send_message.length + 1 )*sizeof(wchar) );
        if(PNULL == string)
        {
            return;
        }
	//	SCI_MEMSET(string,0,( ( send_info_ptr->send_content.send_message.length + 1 )*sizeof(wchar)));
        GUI_UCS2L2UCS2B((uint8 *)string,
                        send_info_ptr->send_content.send_message.length * sizeof(wchar),
                        send_info_ptr->send_content.send_message.data,
                        send_info_ptr->send_content.send_message.length * sizeof(wchar));
        SCI_MEMCPY(
                    send_info_ptr->send_content.send_message.data,
                    (uint8 *)string,
                    send_info_ptr->send_content.send_message.length
                  );  
    	
    	SCI_FREE(string);
		string = PNULL;
#endif
    }
}

#ifdef MMI_TIMERMSG_SUPPORT


/*****************************************************************************/
//     Description : 窗口模块调用该函数，用于定时短消息的发送
//    Global resource dependence : 
//  Author:Liming deng
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_WintabSendTimerSMS( 
                                              BOOLEAN is_re_send
                                              )
{
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_ERR;
    BOOLEAN is_need_save = FALSE;
    MMISMS_SEND_T *send_info_ptr = PNULL;
    MMIACC_SMART_EVENT_FAST_TABLE_T *cur_smart_even_info_ptr = MMISMS_TIMERMSG_GetCurMsgNode();
    BOOLEAN is_need_delete_msg = FALSE;
#ifdef MMI_SMS_CHAT_SUPPORT        
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
#endif
    MN_DUAL_SYS_E send_sys = MN_DUAL_SYS_1;

    if(g_mmisms_global.edit_content.is_resend)
    { 
        send_sys = g_mmisms_global.edit_content.dual_sys;
    }
    else if(!is_re_send)
    {
        send_sys = g_mmisms_global.edit_content.dual_sys;
    }
    else
    {
        send_sys = g_mmisms_global.cur_send_dual_sys; // resend根据最新选卡发送
    }    
    if (send_sys >= MMI_DUAL_SYS_MAX)
    {
        send_sys = g_mmisms_global.cur_send_dual_sys;
    }

    if (send_sys >= MMI_DUAL_SYS_MAX)
    {
        send_sys = MN_DUAL_SYS_1;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(send_sys))
    {
        return MMISMS_CALLING_BUSY;
    }
#endif

    send_info_ptr = MMISMS_CreateSendSMS();

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:" MMISMS_WintabSendSMS (PNULL == send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2578_112_2_18_2_53_21_399,(uint8*)"");
        return MMISMS_SENDSMS_FAIL;
    }

    //初始化内容
    send_info_ptr->send_content.is_need_save = g_mmisms_global.edit_content.is_need_save; 
    send_info_ptr->send_content.is_enough_space_backsend = g_mmisms_global.edit_content.is_enough_space_backsend;
    SCI_MEMCPY(&send_info_ptr->send_content.dest_info, &g_mmisms_global.edit_content.dest_info, sizeof(MMISMS_DEST_ADDR_INFO_T));
    
    is_need_save = send_info_ptr->send_content.is_need_save;
#ifdef MMI_SMS_CHAT_SUPPORT       
    if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
    {        
        cur_order_id = MMISMS_GetCurOperationOrderId();
        if (cur_order_id != PNULL && MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type && is_re_send)
        {
            SCI_TRACE_LOW(" MMISMS_WintabSendSMS MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type");
            send_info_ptr->cur_box_type = MMISMS_BOX_SENDFAIL;
        }
        else
        {
            send_info_ptr->cur_box_type = g_mmisms_global.operation.cur_type;
        }
    }
    else
    {
        send_info_ptr->cur_box_type = g_mmisms_global.operation.cur_type;
    }
#else
    send_info_ptr->cur_box_type = g_mmisms_global.operation.cur_type;
#endif
    send_info_ptr->send_content.is_resend = is_re_send;    

    if (MMISMS_GetIsTimerMsg()
        && PNULL != cur_smart_even_info_ptr
        && (ALM_MODE_ONCE == cur_smart_even_info_ptr->fre_mode))
    {
        is_need_delete_msg = TRUE;
    }
    if (is_need_delete_msg)
    {//定时短信放入发件箱，所以需要设置
        send_info_ptr->cur_box_type = MMISMS_BOX_SENDFAIL;
    }
    
    MMISMS_ClearResendCount(send_info_ptr);
    
    if(g_mmisms_global.edit_content.is_resend)
    { 
        g_mmisms_global.update_info.current_num = 0;
        is_need_save = FALSE;

        SCI_MEMCPY(&send_info_ptr->send_content.send_message, &g_mmisms_global.edit_content.send_message, sizeof(MMISMS_CONTENT_T));
    }
    else if(!is_re_send)
    {
        if (!MMISMS_GetIsTimerMsg())
        {
//            MMISMS_SetIsNeedLoadSaved(FALSE);
        }

        SCI_MEMCPY(&send_info_ptr->send_content.send_message, &g_mmisms_global.edit_content.send_message, sizeof(MMISMS_CONTENT_T));
    }
    else
    {
#ifdef MMI_SMS_CHAT_SUPPORT  	
        if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {
            MMI_STRING_T resend_content = {0};
            MMISMS_ChatWinGetItemContent(&resend_content);
            MMISMS_SetMessageContent(resend_content.wstr_len, resend_content.wstr_ptr, TRUE, &send_info_ptr->send_content);
        }
        else
#endif		
        {
            MMISMS_SetReadMsgTOSend(send_info_ptr);
        }
    }    

    send_info_ptr->send_content.dual_sys = send_sys;
    
    MMISMS_InitSendSaveResult(send_info_ptr);
    if (MMISMS_BOX_SENDFAIL == send_info_ptr->cur_box_type || MMISMS_BOX_NOSEND == send_info_ptr->cur_box_type)
    {   
        //备份mo sms的updateinfo
#ifdef MMI_SMS_CHAT_SUPPORT        
        if(is_re_send && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {            
            send_info_ptr->mosend_update_data.is_mosend_update = TRUE;
            send_info_ptr->mosend_update_data.mosend_send_oper = MMISMS_UPDATE_SENDFAIL_SMS;
			MMISMS_SetMoSendUpdateInfo(cur_order_id, send_info_ptr);
        }
        else
#endif            
        {
            MMISMS_SetMoSendUpdateInfo(g_mmisms_global.update_info.order_id_arr[0], send_info_ptr);
        }
    }
#ifndef MMI_SMS_MINI_SUPPORT   	
    //非后台发送时只走else分支，不然再次开机后会有2条重负保存
    if (send_info_ptr->send_content.is_need_save && MMISMS_BOX_SENDFAIL != send_info_ptr->cur_box_type && !send_info_ptr->send_content.is_enough_space_backsend)
    {//如果需要保存设置该条信息为保存
        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SAVING;       
    }
    else
#endif            
    {
        if (MMISMS_IsSendListEmpty())
        {
            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
        }
        else
        {
            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_WAITING;
        }        
    }
    send_info_ptr->start_send_tickcount = SCI_GetTickCount();
    send_info_ptr->is_need_rpt = TRUE;
    err_code = MMISMS_AppSendSms(send_info_ptr->send_content.dual_sys,is_need_save, send_info_ptr);

    if (MMISMS_SENDSTATUS_SAVING == send_info_ptr->backsend_status)
    {
         MMISMS_AppSaveSms(send_info_ptr->send_content.dual_sys, TRUE);
        send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SAVE_SEND_SMS;
    }

    return err_code;
}
#endif

/*****************************************************************************/
//     Description : 窗口模块调用该函数，用于短消息的发送
//    Global resource dependence : 
//  Author:Liming deng
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_WintabSendSMS( 
                                              BOOLEAN is_re_send
                                              )
{
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_ERR;
    BOOLEAN is_need_save = FALSE;
    MMISMS_SEND_T *send_info_ptr = PNULL;
#ifdef MMI_SMS_CHAT_SUPPORT        
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
#endif
    MN_DUAL_SYS_E send_sys = MN_DUAL_SYS_1;

    if(g_mmisms_global.edit_content.is_resend)
    { 
        send_sys = g_mmisms_global.edit_content.dual_sys;
    }
    else if(!is_re_send)
    {
        send_sys = g_mmisms_global.edit_content.dual_sys;
    }
    else
    {
        send_sys = g_mmisms_global.cur_send_dual_sys; // resend根据最新选卡发送
    }

    if (send_sys >= MMI_DUAL_SYS_MAX)
    {
        send_sys = g_mmisms_global.cur_send_dual_sys;
    }

    if (send_sys >= MMI_DUAL_SYS_MAX)
    {
        send_sys = MN_DUAL_SYS_1;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(send_sys))
    {
        return MMISMS_CALLING_BUSY;
    }
#endif

    send_info_ptr = MMISMS_CreateSendSMS();

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:" MMISMS_WintabSendSMS (PNULL == send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2578_112_2_18_2_53_21_399,(uint8*)"");
        return MMISMS_SENDSMS_FAIL;
    }

    //初始化内容
    send_info_ptr->send_content.is_need_save = g_mmisms_global.edit_content.is_need_save; 
    send_info_ptr->send_content.is_enough_space_backsend = g_mmisms_global.edit_content.is_enough_space_backsend;
    SCI_MEMCPY(&send_info_ptr->send_content.dest_info, &g_mmisms_global.edit_content.dest_info, sizeof(MMISMS_DEST_ADDR_INFO_T));
    
    is_need_save = send_info_ptr->send_content.is_need_save;
#ifdef MMI_SMS_CHAT_SUPPORT       
    if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
    {        
        cur_order_id = MMISMS_GetCurOperationOrderId();
        if (cur_order_id != PNULL && MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type && is_re_send)
        {
            SCI_TRACE_LOW(" MMISMS_WintabSendSMS MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type");
            send_info_ptr->cur_box_type = MMISMS_BOX_SENDFAIL;
        }
        else
        {
            send_info_ptr->cur_box_type = g_mmisms_global.operation.cur_type;
        }
    }
    else
    {
        send_info_ptr->cur_box_type = g_mmisms_global.operation.cur_type;
    }
#else
    send_info_ptr->cur_box_type = g_mmisms_global.operation.cur_type;
#endif
    send_info_ptr->send_content.is_resend = is_re_send;    
    send_info_ptr->is_sendfail_retry = TRUE;
    MMISMS_ClearResendCount(send_info_ptr);   
    if(g_mmisms_global.edit_content.is_resend)
    { 
        g_mmisms_global.update_info.current_num = 0;
        is_need_save = FALSE;

        SCI_MEMCPY(&send_info_ptr->send_content.send_message, &g_mmisms_global.edit_content.send_message, sizeof(MMISMS_CONTENT_T));
    }
    else if(!is_re_send)
    {
        SCI_MEMCPY(&send_info_ptr->send_content.send_message, &g_mmisms_global.edit_content.send_message, sizeof(MMISMS_CONTENT_T));
    }
    else
    {
#ifdef MMI_SMS_CHAT_SUPPORT  	
        if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {
            MMI_STRING_T resend_content = {0};
            MMISMS_ChatWinGetItemContent(&resend_content);
            MMISMS_SetMessageContent(resend_content.wstr_len, resend_content.wstr_ptr, TRUE, &send_info_ptr->send_content);
        }
        else
#endif
        {
            MMISMS_SetReadMsgTOSend(send_info_ptr);
        }
    }    

    send_info_ptr->send_content.dual_sys = send_sys;

    MMISMS_InitSendSaveResult(send_info_ptr);
    if (MMISMS_BOX_SENDFAIL == send_info_ptr->cur_box_type || MMISMS_BOX_NOSEND == send_info_ptr->cur_box_type)
    {   
        //备份mo sms的updateinfo
#ifdef MMI_SMS_CHAT_SUPPORT        
        if(is_re_send && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {            
            send_info_ptr->mosend_update_data.is_mosend_update = TRUE;
            send_info_ptr->mosend_update_data.mosend_send_oper = MMISMS_UPDATE_SENDFAIL_SMS;
            MMISMS_SetMoSendUpdateInfo(cur_order_id, send_info_ptr);
        }
        else
#endif            
        {
            MMISMS_SetMoSendUpdateInfo(g_mmisms_global.update_info.order_id_arr[0], send_info_ptr);
        }
    }
    SCI_TRACE_LOW("MMISMS: is_need_save = %d",send_info_ptr->send_content.is_need_save);
    SCI_TRACE_LOW("MMISMS: cur_box_type = %d",send_info_ptr->cur_box_type);
    SCI_TRACE_LOW("MMISMS: is_enough_space_backsend = %d",send_info_ptr->send_content.is_enough_space_backsend);
#ifndef MMI_SMS_MINI_SUPPORT   	
    //非后台发送时只走else分支，不然再次开机后会有2条重负保存
    if (send_info_ptr->send_content.is_need_save && MMISMS_BOX_SENDFAIL != send_info_ptr->cur_box_type && !send_info_ptr->send_content.is_enough_space_backsend)
    {//如果需要保存设置该条信息为保存
        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SAVING;       
    }
    else
#endif            
    {
        if (MMISMS_IsSendListEmpty())
        {
            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
        }
        else
        {
            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_WAITING;
        }        
    }
	SCI_TRACE_LOW("MMISMS:MMISMS_WintabSendSMS send_info_ptr backsend_status = %d!",send_info_ptr->backsend_status);
    send_info_ptr->start_send_tickcount = SCI_GetTickCount();
    send_info_ptr->is_need_rpt = TRUE;
    err_code = MMISMS_AppSendSms(send_info_ptr->send_content.dual_sys,is_need_save, send_info_ptr);

    if (MMISMS_SENDSTATUS_SAVING == send_info_ptr->backsend_status)
    {
        //MMISMS_AppSaveSms(send_info_ptr->send_content.dual_sys, TRUE);
        //解决第一次发短信后底层Write cnf 消息还没上来，又发起发短信，导致send_info_ptr取的不对的问题
        MMISMS_AppSaveSmsEx(send_info_ptr,TRUE);
        send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SAVE_SEND_SMS;
    }

    return err_code;
}

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description :发送Vcard短信
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_WintabSendVcard(MN_DUAL_SYS_E dual_sys)
{
    MMI_STRING_T build_data = {0};
    uint8 *p_temp = PNULL;
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_ERR;
    uint16 wstr_len=0;

#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        //MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return MMISMS_SYSTEM_BUSY;
    }
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        return MMISMS_CALLING_BUSY;
    }
#endif

    MMISMS_ClearDestId();
    g_mmisms_global.cur_send_dual_sys = dual_sys;

    p_temp = MMIAPIVCARD_GetBuildData();

    if (PNULL == p_temp)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_WintabSendVcard p_temp = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2696_112_2_18_2_53_22_400,(uint8*)"");
        return err_code;
    }
    wstr_len=strlen((char *)p_temp)/sizeof(wchar);
    if(0!=(strlen((char *)p_temp)%sizeof(wchar)))
    {
        wstr_len+=1;
    }
    build_data.wstr_len =wstr_len;
    build_data.wstr_ptr = (wchar *)p_temp;
    err_code = MMISMS_AppSendSmsForVcardAndVcal(g_mmisms_global.cur_send_dual_sys, &build_data, TRUE);
    MMIAPIVCARD_BuildFree();
    //SCI_TRACE_LOW:"MMISMS_WintabSendVcard, err_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2708_112_2_18_2_53_22_401,(uint8*)"d", err_code);
    return err_code;
}
#endif

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description :发送Vcalendar短信
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_WintabSendVcalendar(MN_DUAL_SYS_E dual_sys)
{
    MMI_STRING_T build_data = {0};
    uint8 *p_temp = PNULL;
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_ERR;
    uint16 wstr_len=0;

#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        //MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return MMISMS_SYSTEM_BUSY;
    }
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        return MMISMS_CALLING_BUSY;
    }
#endif

    MMISMS_ClearDestId();
    g_mmisms_global.cur_send_dual_sys = dual_sys;
    
    p_temp = MMIVCAL_GetBuildData();
    if (PNULL == p_temp)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_WintabSendVcalendar p_temp = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2737_112_2_18_2_53_22_402,(uint8*)"");
        return err_code;
    }
    wstr_len=strlen((char *)p_temp)/sizeof(wchar);
    if(0!=(strlen((char *)p_temp)%sizeof(wchar)))
    {
        wstr_len+=1;
    }
    build_data.wstr_len = wstr_len;
    build_data.wstr_ptr = (wchar *)p_temp;
    err_code = MMISMS_AppSendSmsForVcardAndVcal(g_mmisms_global.cur_send_dual_sys,&build_data, FALSE);
    MMIVCAL_VcalBuildExit();
    //SCI_TRACE_LOW:"MMISMS_WintabSendVcalendar, err_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2749_112_2_18_2_53_22_403,(uint8*)"d", err_code);
    return err_code;
}
#endif
/*****************************************************************************/
// Description : to send the SMS for Vcard
// Global resource dependence :
// Author:Bill.Ji
// Note: vcard和vcanlendar的创建进入号码界面创建，这个时候已经创建好了，因为号码已经写入全局变量了 
/*****************************************************************************/
MMISMS_OPER_ERR_E MMISMS_AppSendSmsForVcardAndVcal(
                                                   MN_DUAL_SYS_E   dual_sys,
                                                   MMI_STRING_T    *sms_init_ptr,
                                                   BOOLEAN is_vcard)
{       
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_DEST_ADDRESS;
    MMISMS_SEND_T *send_info_ptr = PNULL;    
    
    if (PNULL == sms_init_ptr || PNULL == sms_init_ptr->wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_AppSendSmsForVcardAndVcal sms_init_ptr sms_init_ptr->wstr_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2767_112_2_18_2_53_22_404,(uint8*)"");
        return err_code;
    }
    
    //SCI_TRACE_LOW:"mmivcard:MMISMS_AppSendSmsForVcardAndVcal"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2771_112_2_18_2_53_22_405,(uint8*)"");
    if (0 != sms_init_ptr->wstr_len)
    {
        send_info_ptr = MMISMS_CreateSendSMS();
        if(PNULL == send_info_ptr)
        {
            //SCI_TRACE_LOW:"MMISMS MMISMS_AppSendSmsForVcardAndVcal PNULL == send_message"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2777_112_2_18_2_53_22_406,(uint8*)"");
            return MMISMS_SENDSMS_FAIL;
        }
        SCI_MEMCPY(&send_info_ptr->send_content.dest_info, &g_mmisms_global.edit_content.dest_info, sizeof(MMISMS_DEST_ADDR_INFO_T));
        send_info_ptr->send_content.is_need_save = FALSE;
        send_info_ptr->send_content.dual_sys = dual_sys;
        send_info_ptr->send_content.is_vacard_vcal = TRUE;
        send_info_ptr->cur_box_type = g_mmisms_global.operation.cur_type;
        if (is_vcard)
        {
            send_info_ptr->is_head_has_port = TRUE;
            InitUserDataHeadForVcard(MMISMS_VCARD_PORT_NUM, &send_info_ptr->send_content.tpdu_head);
        }
        else
        {
            send_info_ptr->is_head_has_port = TRUE;
            InitUserDataHeadForVcard(MMISMS_VCAL_PORT_NUM, &send_info_ptr->send_content.tpdu_head);
        }
        
        MMISMS_ClearResendCount(send_info_ptr);
        MMISMS_SetMessageContent(sms_init_ptr->wstr_len,sms_init_ptr->wstr_ptr, FALSE, &send_info_ptr->send_content);
        send_info_ptr->send_content.send_message.alphabet = MN_SMS_8_BIT_ALPHBET;
        MMISMS_InitSendSaveResult(send_info_ptr);
        send_info_ptr->send_callback = MMISMS_VcardOrVcalSendCnfCallback;
        if (MMISMS_IsSendListEmpty())
        {
            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
        }
        else
        {
            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_WAITING;
        }
        send_info_ptr->start_send_tickcount = SCI_GetTickCount();
        send_info_ptr->is_need_rpt = TRUE;
        err_code = MMISMS_AppSendSms(dual_sys, FALSE, send_info_ptr);
    }
    return err_code;
}

/*****************************************************************************/
//     Description : set dest dual sys
//    Global resource dependence : 
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
void MMISMS_SetDestDualSys( MN_DUAL_SYS_E dual_sys )
{    
    g_mmisms_global.edit_content.dual_sys = dual_sys;
}

/*****************************************************************************/
//     Description : get dest dual sys
//    Global resource dependence : 
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
MN_DUAL_SYS_E    MMISMS_GetDestDualSys( void )
{
    return g_mmisms_global.edit_content.dual_sys;
}
/*****************************************************************************/
//     Description : hanlde write cnf error 
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
void MMISMS_HandleWriteError( void )
{
    BOOLEAN is_send_success = FALSE;
    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleWriteError send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2844_112_2_18_2_53_22_407,(uint8*)"");
        return;
    }

    MMISMS_SetOperStatus(MMISMS_SEND_SMS);
    send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SEND_SMS;
    
    if ( MMISMS_IsExistNextSMS(&send_info_ptr->send_content)
        && (MMISMS_SEND_SUCC == send_info_ptr->send_save_result.send_result )
        ) //长短消息的前一条发送成功，且不是长短信的最后一条
    {
        //后续发送的SMS都不需要保存 
        send_info_ptr->cur_send_data.cur_send_storage = MN_SMS_NO_STORED;
        // send the next message to the same destination
        MMISMS_SendMsgBySeqNum(send_info_ptr->send_save_result.dual_sys,GET_HEAD_MAX_NUM(&send_info_ptr->send_content), GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1, FALSE, send_info_ptr);
    }
    else //继续发送下一条
    {
        send_info_ptr->send_save_result.save_storage = MN_SMS_NO_STORED;        

        // clear the destination address which send successfully
        MMISMS_ClearDestAddrById();
                
        if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
        {
            // to send message to next destination address
            MMISMS_InitSendSaveResult(send_info_ptr);
            //MMISMS_DeleteSendList();
            if (MMISMS_NO_DEST_ADDRESS == MMISMS_AppSendSms(send_info_ptr->send_save_result.dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr))  //发送到下一个号码
            {
                OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
            }
            else
            {
                //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);                      
            }
        }
        else
        {
            OperAfterSendCnf(send_info_ptr->send_save_result.dual_sys, is_send_success, TRUE);
        }            
    }
    
}

/*****************************************************************************/
// Description : Set long sms head data
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void Set_Longsms_Head_Var(MN_SMS_USER_DATA_HEAD_T   *head_ptr,
                                                                     uint8 ref_num,
                                                                     uint8 max_num,
                                                                     uint8 seq_num)
{
    uint8 pos = 0;
    
    if (PNULL == head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:Set_Longsms_Head_Var head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2903_112_2_18_2_53_22_408,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"SMS: Set_Longsms_Head_Var ref_num=%d,max_num=%d,seq_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2907_112_2_18_2_53_22_409,(uint8*)"ddd",ref_num, max_num, seq_num);
    if (head_ptr->length > 0 &&
        head_ptr->user_header_arr[MMISMS_HEAD_IEL_POS] > 0)
    {
        pos = head_ptr->length;
    }

    head_ptr->length += MMISMS_HEAD_8BIT_LENGTH;
    head_ptr->user_header_arr[pos+MMISMS_HEAD_IEI_POS] = MMISMS_HEAD_8BIT_IEI;
    head_ptr->user_header_arr[pos+MMISMS_HEAD_IEL_POS] = MMISMS_HEAD_8BIT_IEL;
    head_ptr->user_header_arr[pos+MMISMS_HEAD_REF_NUM_POS] = ref_num;
    head_ptr->user_header_arr[pos+MMISMS_HEAD_8BIT_MAX_NUM_POS] = max_num;
    head_ptr->user_header_arr[pos+MMISMS_HEAD_8BIT_SEQ_NUM_POS] = seq_num;

    return;
}

/*****************************************************************************/
// Description : Set R8 sms head data
// Global resource dependence:
// Author:fengming.huang
// Note:
/*****************************************************************************/
PUBLIC void Set_SmsR8_Head_Var(MN_SMS_USER_DATA_HEAD_T   *head_ptr,
                                 MMISMS_R8_LANGUAGE_E   language,
                                 BOOLEAN   single_shift)
{
    uint8    pos = 0;

    if (PNULL == head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:Set_SmsR8_Head_Var head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2936_112_2_18_2_53_22_410,(uint8*)"");
        return;
    }
    if (head_ptr->length > 0 &&
        head_ptr->user_header_arr[MMISMS_HEAD_IEL_POS] > 0)
    {
        pos = head_ptr->length;
    }
    if (language == MMISMS_R8_LANGUAGE_SPANISH)
    {
        head_ptr->length                                  += MMISMS_R8_HEAD_LEN;
        head_ptr->user_header_arr[pos+MMISMS_HEAD_IEI_POS] = MN_SMS_NATIONAL_LANG_SINGLE_SHIFT;
        head_ptr->user_header_arr[pos+MMISMS_HEAD_IEL_POS] = MMISMS_R8_HEAD_IEL;
        head_ptr->user_header_arr[pos+MMISMS_HEAD_REF_NUM_POS] = (uint8)language;
    }
    else
    {
        head_ptr->length                                  += MMISMS_R8_HEAD_LEN;
        head_ptr->user_header_arr[pos+MMISMS_HEAD_IEI_POS] = MN_SMS_NATIONAL_LANG_LOCK_SHIFT;
        head_ptr->user_header_arr[pos+MMISMS_HEAD_IEL_POS] = MMISMS_R8_HEAD_IEL;
        head_ptr->user_header_arr[pos+MMISMS_HEAD_REF_NUM_POS] = (uint8)language;
    
        if(single_shift)
        {
            head_ptr->length                                  += MMISMS_R8_HEAD_LEN;
            head_ptr->user_header_arr[pos+MMISMS_R8_HEAD_LEN+MMISMS_HEAD_IEI_POS] = MN_SMS_NATIONAL_LANG_SINGLE_SHIFT;
            head_ptr->user_header_arr[pos+MMISMS_R8_HEAD_LEN+MMISMS_HEAD_IEL_POS] = MMISMS_R8_HEAD_IEL;
            head_ptr->user_header_arr[pos+MMISMS_R8_HEAD_LEN+MMISMS_HEAD_REF_NUM_POS] = (uint8)language;
        }
    }
    
    return;
}

/*****************************************************************************/
// Description : Get current ref number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Current_Ref_Num(MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr)
{
    uint8 i = 0;
    uint8 ref_num = 0;
    uint8 iei_num = 0;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;

    if (PNULL == edit_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:Get_Current_Ref_Num edit_content_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2975_112_2_18_2_53_22_411,(uint8*)"");
        return 0;
    }
    
    MNSMS_DecodeUserDataHeadEx(dual_sys, &edit_content_ptr->tpdu_head, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            ref_num = decode_head.iei_arr[i].iei_data_t[0];
            break;
        }
    }
    //SCI_TRACE_LOW:"SMS: Get_Current_Ref_Num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2989_112_2_18_2_53_22_412,(uint8*)"d", ref_num);
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);

    return ref_num;
}

/*****************************************************************************/
// Description : Get current max number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Head_Max_Num(MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr)
{
    uint8  i = 0;
    uint8 max_num = 0;
    uint8 iei_num = 0;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;

    if (PNULL == edit_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:Get_Head_Max_Num edit_content_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3009_112_2_18_2_53_22_413,(uint8*)"");
        return 1;
    }
    
    if (((edit_content_ptr->tpdu_head.length > 0) && !MMISMS_IsR8Language(edit_content_ptr->send_message.language))
        || ((edit_content_ptr->tpdu_head.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) && MMISMS_IsR8Language(edit_content_ptr->send_message.language)))
    {
        max_num = 1;
    }
    MNSMS_DecodeUserDataHeadEx(dual_sys, &edit_content_ptr->tpdu_head, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            max_num = decode_head.iei_arr[i].iei_data_t[1];
            break;
        }
    }
    //SCI_TRACE_LOW:"SMS: Get_Current_Max_Num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3028_112_2_18_2_53_22_414,(uint8*)"d", max_num);
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);

    return max_num;
}

/*****************************************************************************/
// Description : Get current seq number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Current_Seq_Num(MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr)
{
    uint8 i = 0;
    uint8 seq_num = 1;
    uint8 iei_num = 0;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;

    if (PNULL == edit_content_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:Get_Current_Seq_Num edit_content_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3048_112_2_18_2_53_22_415,(uint8*)"");
        return 1;
    }
    
    MNSMS_DecodeUserDataHeadEx(dual_sys, &edit_content_ptr->tpdu_head, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            seq_num = decode_head.iei_arr[i].iei_data_t[2];
            break;
        }
    }
    //SCI_TRACE_LOW:"SMS: Get_Current_Seq_Num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3062_112_2_18_2_53_22_416,(uint8*)"d", seq_num);
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);

    return seq_num;
}

/*****************************************************************************/
// Description : Set current seq number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void Set_Current_Seq_Num(MN_SMS_USER_DATA_HEAD_T   *head_ptr,
                                                                uint8 seq_num)
{
    uint8 pos = 0, i = 0;
    uint8 iei_num = 0;
    int index = -1;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;
    
    if (PNULL == head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:Set_Current_Seq_Num head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3083_112_2_18_2_53_22_417,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"SMS: Set_Current_Seq_Num seq_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3086_112_2_18_2_53_22_418,(uint8*)"d", seq_num);
    MNSMS_DecodeUserDataHeadEx(dual_sys, head_ptr, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            index = i;
            break;
        }
        else
        {
            pos += decode_head.iei_arr[i].length + 1;
        }
    }
    if (-1 != index )
    {
        head_ptr->user_header_arr[pos+i+MMISMS_HEAD_8BIT_SEQ_NUM_POS] = seq_num;
    }
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);

    return;
}

/*****************************************************************************/
//     Description : to get the dest addr 
//    Global resource dependence : 
//  Author:wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetMnCalledNum(
                                                                        uint8 *dest_ptr,
                                                                        uint8 dest_len,
                                                                        MMI_PARTY_NUMBER_T *party_number_ptr,    //OUT:
                                                                        MN_CALLED_NUMBER_T *dest_number_ptr     //OUT:
                                                                        )
{
    uint8 max_dest_num[MMISMS_RECIEVER_NUM_MAX] = {0};

    if (PNULL == dest_ptr || PNULL == party_number_ptr || PNULL == dest_number_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetMnCalledNum dest_ptr party_number_ptr dest_number_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3125_112_2_18_2_53_23_419,(uint8*)"");
        return;
    }    
    // init the destination number
    dest_number_ptr->num_len     = 0;
    dest_number_ptr->number_plan = MN_NUM_PLAN_UNKNOW;
    dest_number_ptr->number_type = MN_NUM_TYPE_UNKNOW;   
    
    if(dest_len > MMISMS_RECIEVER_NUM_MAX)
    {
        dest_len = MMISMS_RECIEVER_NUM_MAX;
    }

    SCI_MEMSET(max_dest_num, 0x00, sizeof(max_dest_num));
    SCI_MEMCPY(max_dest_num, dest_ptr, dest_len);
    
    if (dest_len > 0)
    {
        MMIAPICOM_GenPartyNumber((uint8*)max_dest_num, dest_len, party_number_ptr);
        MMIPARTYNUM_2_MNCALLEDNUM(party_number_ptr, dest_number_ptr);
    }

    //temporay solution : CR148886 special case which 38 is national number but no '+' before
    if((12 == dest_len)  && ((0x33 == dest_ptr[0]) && (0x38==dest_ptr[1])) ) // 12 mean lenth of 380919570696 (38 is national number)
    {
        dest_number_ptr->number_type = MN_NUM_TYPE_INTERNATIONAL;
    }
    
}

/*****************************************************************************/
// Description : Set Deliver Cancel
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetDeliverCancel(BOOLEAN is_deliver_cancel)
{
    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SetDeliverCancel send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3160_112_2_18_2_53_23_420,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMISMS: MMISMS_SetDeliverCancel=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3163_112_2_18_2_53_23_421,(uint8*)"d", is_deliver_cancel);

    send_info_ptr->is_deliver_cancel = is_deliver_cancel;
}

/*****************************************************************************/
// Description : Get Is Deliver Cancel
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDeliverCancel(void)
{
    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetDeliverCancel send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3180_112_2_18_2_53_23_422,(uint8*)"");
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetDeliverCancel=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3184_112_2_18_2_53_23_423,(uint8*)"d", send_info_ptr->is_deliver_cancel);

    return send_info_ptr->is_deliver_cancel;
}
/*****************************************************************************/
//Description : 等待窗口处理
//Global resource dependence : s_sms_waiting_time_id
//Author:chunjie.liu
//Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleTimeWaitWin(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E        recode    = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
    	s_sms_waiting_time_id = MMK_CreateWinTimer(MMISMS_SEND_ANIM_WIN_ID, SMS_SEND_ANIM_TIMER_OUT, FALSE);
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        MMIPUB_CloseWaitWin(win_id);
        if (0 != s_sms_waiting_time_id)
				{
					MMK_StopTimer(s_sms_waiting_time_id);
					s_sms_waiting_time_id=0;
				}
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    case MSG_TIMER:
    	  if(s_sms_waiting_time_id == *((uint8 *)param))
    	  {
    	  	MMIPUB_CloseWaitWin(win_id);
    	  	if (0 != s_sms_waiting_time_id)
					{
						MMK_StopTimer(s_sms_waiting_time_id);
						s_sms_waiting_time_id=0;
					}
    	  }
    	  recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    case MSG_CLOSE_WINDOW:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return recode;
}
/*****************************************************************************/
//  Description : response sms control
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ResponseSmsCtl(MN_DUAL_SYS_E dual_sys,
                                                                                MMI_STRING_T *alert_str_ptr,
                                                                                MMI_STRING_T *display_str_ptr,
                                                                                MN_CALLED_NUMBER_T *dest_me,
                                                                                MN_CALLED_NUMBER_T *dest_sc,
                                                                                BOOLEAN is_need_modified)
{
    //SCI_TRACE_LOW:"MMISMS: MMISMS_ResponseSmsCtl dual_sys=%d, is_need_modified=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3200_112_2_18_2_53_23_424,(uint8*)"dd", dual_sys, is_need_modified);

    if (MMK_IsOpenWin(MMISMS_SEND_ANIM_WIN_ID))
    {
        MMI_STRING_T name = {0};
        wchar name_arr[MMISMS_PBNAME_MAX_LEN + 2] = {0};

        name.wstr_ptr = name_arr;

        if (PNULL != dest_me && dest_me->num_len > 0 && is_need_modified)
        {
            uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 2]= {0};
            uint16 tele_num_len = 0;
            uint8 addr_id = 0;
            MMISMS_SEND_T   *send_message = PNULL;

            send_message = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);

            if(PNULL == send_message)
            {
                //SCI_TRACE_LOW:"MMISMS MMISMS_ResponseSmsCtl PNULL == send_message"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3220_112_2_18_2_53_23_425,(uint8*)"");

                return FALSE;
            }

            tele_num_len = MMIAPICOM_GenNetDispNumber(dest_me->number_type,
                                                                                        dest_me->num_len,
                                                                                        (uint8 *)dest_me->party_num,
                                                                                        (uint8 *)tele_num,
                                                                                        (uint8)sizeof(tele_num)
                                                                                        );

            addr_id = send_message->send_content.dest_info.dest_list.dest_addr_id;

            if (send_message->send_content.dest_info.dest_list.number > addr_id)
            {
                tele_num_len = MIN(tele_num_len, sizeof(send_message->send_content.dest_info.dest_list.dest_addr[addr_id]));

                SCI_MEMSET(send_message->send_content.dest_info.dest_list.dest_addr[addr_id], 0x00, sizeof(send_message->send_content.dest_info.dest_list.dest_addr[addr_id]));
                SCI_MEMCPY(send_message->send_content.dest_info.dest_list.dest_addr[addr_id], tele_num, tele_num_len);

                if (!MMISMS_GetNameByNumberFromPB((uint8 *)tele_num, &name, MMISMS_PBNAME_MAX_LEN))
                {
                    //not find
                    name.wstr_len = MIN(MMISMS_PBNAME_MAX_LEN, tele_num_len);

                    MMI_STRNTOWSTR(name.wstr_ptr,
                                    MMISMS_PBNAME_MAX_LEN,
                                    (uint8 *)tele_num,
                                    tele_num_len,
                                    name.wstr_len);
                }

//#ifndef MMI_MULTI_SIM_SYS_SINGLE
//                MMIPUB_SetWaitWinTextEx(TRUE, 2, &name, MMISMS_SEND_ANIM_WIN_ID);
//#else
//                MMIPUB_SetWaitWinTextEx(TRUE, 1, &name, MMISMS_SEND_ANIM_WIN_ID);
//#endif
            }
        }

		/*Bug 80505 - [Orange][6530][32x32_BAR_QW][STK]DUT failed to display alpha ID when executing MO SM control*/
        if (PNULL != display_str_ptr && display_str_ptr->wstr_len > 0)
        {
            MMIPUB_SetWaitWinTextEx(TRUE, 0, display_str_ptr, MMISMS_SEND_ANIM_WIN_ID);
        }

        //SCI_TRACE_LOW:"[MMISMS] MMISMS_ResponseSmsCtl Success!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3266_112_2_18_2_53_23_426,(uint8*)"");
    }
    else if (PNULL != display_str_ptr && display_str_ptr->wstr_ptr && display_str_ptr->wstr_len > 0)
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_ResponseSmsCtl else Success!");
        //cr 109284 it isn't normal if msd cancel input password when power on
        //bug 145853
        MMIPUB_OpenWaitWin(1, display_str_ptr, PNULL, PNULL, MMISMS_SEND_ANIM_WIN_ID, IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, MMISMS_HandleTimeWaitWin);
    }

    if (PNULL != alert_str_ptr && alert_str_ptr->wstr_len > 0)
    {
        MMIPUB_CloseAlertWin();
        MMIPUB_OpenAlertWinByTextPtr(PNULL, alert_str_ptr, PNULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);  
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : create a send message
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SEND_T *MMISMS_CreateSendSMS( void )
{
    MMISMS_SEND_T *send_info_ptr = PNULL;

    send_info_ptr = (MMISMS_SEND_T *)SCI_ALLOCAZ(sizeof(MMISMS_SEND_T));
    if (PNULL != send_info_ptr)
    {
        //SCI_MEMSET(send_info_ptr, 0, sizeof(MMISMS_SEND_T));
    }
    else
    {
        //SCI_TRACE_LOW:"[MMISMS] alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3293_112_2_18_2_53_23_427,(uint8*)"");
        return PNULL;
    }
    send_info_ptr->send_content.dual_sys = MN_DUAL_SYS_MAX;
#if 0    
	if(MMIAPIENG_GetCMCCTestFlag())
	{
		send_info_ptr->send_content.is_resend = FALSE;
	}
	else
	{
		send_info_ptr->send_content.is_resend = TRUE;
	}
#endif	
    //SCI_TRACE_LOW:"[MMISMS] MMISMS_CreateSendSMS Success!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3305_112_2_18_2_53_23_428,(uint8*)"");
    return send_info_ptr;
}

/*****************************************************************************/
//  Description : get the edit content from global
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:该结构在sms编辑时候用
/*****************************************************************************/
PUBLIC MMISMS_SMSEDIT_CONTENT_T * MMISMS_GetEditContentFromGlobal( void )
{    
    return &g_mmisms_global.edit_content;
}

/*****************************************************************************/
//  Description : get the send info from global
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:该结构在sms加入发送链表后使用
/*****************************************************************************/
PUBLIC MMISMS_SEND_T * MMISMS_GetSendInfoFromSendLIst( MMISMS_BACKSENDSTATUS_E status )
{
    MMISMS_SEND_T * send_info_ptr = PNULL;
    BOOLEAN is_find = FALSE;

    if (0 == s_mmisms_sendlist.send_num && PNULL == s_mmisms_sendlist.sendlist_head_ptr)
    {
        return PNULL;
    }
    else
    {
        if (MMISMS_SENDSTATUS_SAVING != status)
        {
            return s_mmisms_sendlist.sendlist_head_ptr;
        }
        else
        {
            send_info_ptr = s_mmisms_sendlist.sendlist_head_ptr;
            while (PNULL != send_info_ptr)
            {
                if (status == send_info_ptr->backsend_status)
                {
                    is_find = TRUE;
                    break;
                }
                send_info_ptr = send_info_ptr->next;
            }
            if (is_find)
            {
                return send_info_ptr;
            }
            else
            {
                return PNULL;
            }
        }        
    }
}

/*****************************************************************************/
//  Description : Add send_info_ptr to send list
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AddSendList( MMISMS_SEND_T *send_info_ptr )
{
    MMISMS_SEND_T *temp_sendinfo = PNULL;
    MMISMS_OPER_ERR_E send_result = MMISMS_NO_ERR;

    //应该做保护，如果状态为不对，就把list里面清理掉
    {//如果第一条消息不是sending状态或者saving就是不对的，发送中的消息应该是sending
        if (PNULL != s_mmisms_sendlist.sendlist_head_ptr && 
            MMISMS_SENDSTATUS_SENDING != s_mmisms_sendlist.sendlist_head_ptr->backsend_status
            && MMISMS_SENDSTATUS_SAVING != s_mmisms_sendlist.sendlist_head_ptr->backsend_status)
		{
            //SCI_TRACE_LOW:"[MMISMS] MMISMS_AddSendList status error  !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3376_112_2_18_2_53_23_429,(uint8*)"");
            MMISMS_DeleteSendList();
        }        
		}

    if (s_mmisms_sendlist.send_num >= MMISMS_SENDLIST_MAX_NUM)
    {
        return MMISMS_SENDLIST_FULL;
    }

    //SCI_TRACE_LOW:"[MMISMS] MMISMS_AddSendList s_mmisms_sendlist.send_num = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3386_112_2_18_2_53_23_430,(uint8*)"d", s_mmisms_sendlist.send_num);
    if (0 == s_mmisms_sendlist.send_num && PNULL == s_mmisms_sendlist.sendlist_head_ptr)
    {        
        s_mmisms_sendlist.sendlist_head_ptr = send_info_ptr;
        if(send_info_ptr->backsend_status == MMISMS_SENDSTATUS_WAITING)
        {
            if(send_info_ptr->send_content.is_need_save)
            {
                send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SAVING;
            }
            else
            {
                send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
            }
        }
    }
	else
    {        
        temp_sendinfo = s_mmisms_sendlist.sendlist_head_ptr;
        while (PNULL != temp_sendinfo)
		{
            if (send_info_ptr == temp_sendinfo)
            {//如果加入一样的就不要再加入
                //SCI_TRACE_LOW:"[MMISMS] MMISMS_AddSendList the same one!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3398_112_2_18_2_53_23_431,(uint8*)"");
                return send_result;
            }
            temp_sendinfo = temp_sendinfo->next;
        }
        ClearExpiredSmsInSendList();
        temp_sendinfo = s_mmisms_sendlist.sendlist_head_ptr;
        if(PNULL == temp_sendinfo)
        {
            s_mmisms_sendlist.sendlist_head_ptr = send_info_ptr;
        }
        else
        {
            while (PNULL != temp_sendinfo->next)
            {
                temp_sendinfo = temp_sendinfo->next;
            }
            temp_sendinfo->next = send_info_ptr;
        }
    }
    s_mmisms_sendlist.send_num++; 

    return send_result;
}

/*****************************************************************************/
//  Description : clear expired sms
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void ClearExpiredSmsInSendList( void )
{
    MMISMS_SEND_T *temp_sendinfo = PNULL;
    MMISMS_SEND_T *last_sendinfo = PNULL;
    MMISMS_SEND_T *cur_sendinfo = s_mmisms_sendlist.sendlist_head_ptr;
    uint32 cur_time = 0;
    uint32 spend_time = 0;     //应该花的时间

    if (PNULL == cur_sendinfo)
        {
        return;
        }
    last_sendinfo = cur_sendinfo;
    while (PNULL != cur_sendinfo)
    {
        spend_time = cur_sendinfo->cur_send_data.total_num * cur_sendinfo->send_content.dest_info.dest_list.number * MMISMS_SENDSMS_SINGLE_TIME;
        cur_time = SCI_GetTickCount();
        if (cur_time > (cur_sendinfo->start_send_tickcount + spend_time))
        {//expired 删除该条信息同时更新发件箱状态
            MMISMS_SetBackSendStatus(MMISMS_SENDSTATUS_SENDFAIL, cur_sendinfo);
            if (s_mmisms_sendlist.sendlist_head_ptr == cur_sendinfo)
            {
                s_mmisms_sendlist.sendlist_head_ptr = cur_sendinfo->next; 
                temp_sendinfo = cur_sendinfo;
                cur_sendinfo = s_mmisms_sendlist.sendlist_head_ptr;
            }
        else
        {
                last_sendinfo->next = cur_sendinfo->next;
                temp_sendinfo = cur_sendinfo; 
                cur_sendinfo = last_sendinfo->next;
        }
            if (0 < s_mmisms_sendlist.send_num)
            {
                s_mmisms_sendlist.send_num --;
            }    
            SCI_FREE(temp_sendinfo);
            temp_sendinfo = PNULL;            
	}
        else
        {
            last_sendinfo = cur_sendinfo;
            cur_sendinfo = cur_sendinfo->next;
        }        
    }
}

/*****************************************************************************/
//  Description : Delete all message in send list
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_DeleteAllMsgInList( void )
{
    MMISMS_SEND_T *temp_sendinfo = PNULL;
    MMISMS_SEND_T *cur_sendinfo = PNULL;

    if (PNULL == s_mmisms_sendlist.sendlist_head_ptr)
    {
        s_mmisms_sendlist.send_num = 0;
        return;
    }
    cur_sendinfo = s_mmisms_sendlist.sendlist_head_ptr;
    while (PNULL != cur_sendinfo)
    {
        temp_sendinfo = cur_sendinfo;
        {
            SCI_FREE(temp_sendinfo);
            temp_sendinfo = PNULL;
        }
        cur_sendinfo = cur_sendinfo->next;
    }
    s_mmisms_sendlist.send_num = 0;
	s_mmisms_sendlist.sendlist_head_ptr = PNULL;
}

/*****************************************************************************/
//  Description : is send list empty
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsSendListEmpty( void )
{
    if (PNULL == s_mmisms_sendlist.sendlist_head_ptr || s_mmisms_sendlist.send_num == 0 || 
        (1 == s_mmisms_sendlist.send_num 
        && PNULL != s_mmisms_sendlist.sendlist_head_ptr
        && MMISMS_SENDSTATUS_SAVING == s_mmisms_sendlist.sendlist_head_ptr->backsend_status)
                                   )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :判断是否 sendlist num大于1且链表中短信状态都为saving状态
//  Parameter: [In] NULL
//             [out] NULL
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsAllSendListSaving( void )
{
    MMISMS_SEND_T * cursend_info_ptr = PNULL;
    BOOLEAN IsSaving = FALSE;

    if (1 < s_mmisms_sendlist.send_num)
    {
        cursend_info_ptr = s_mmisms_sendlist.sendlist_head_ptr;
        while (PNULL != cursend_info_ptr)
        {
            if(MMISMS_SENDSTATUS_SAVING != cursend_info_ptr->backsend_status)
            {
                IsSaving = FALSE;
                SCI_TRACE_LOW("MMISMS:MMISMS_IsAllSendListSaving backsend_status not SAVING!");
                break;
            }
            else
            {
                cursend_info_ptr = cursend_info_ptr->next;
            }

            IsSaving = TRUE;
        }
    }
    else
    {
        IsSaving = FALSE;
    }
    SCI_TRACE_LOW("MMISMS:MMISMS_IsAllSendListSaving IsSaving =%d",IsSaving);
    return IsSaving;
}


/*****************************************************************************/
//  Description : Delete send_info_ptr to send list
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_DeleteSendList(void)
{
    MMISMS_SEND_T *cur_sendinfo = PNULL;

    if (0 == s_mmisms_sendlist.send_num && PNULL == s_mmisms_sendlist.sendlist_head_ptr)
    {
        //SCI_TRACE_LOW:"[MMISMS] MMISMS_DeleteSendList no sms delete!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3531_112_2_18_2_53_23_432,(uint8*)"");
        return;
    }
    cur_sendinfo = s_mmisms_sendlist.sendlist_head_ptr;

#ifndef MMI_SMS_MINI_SUPPORT   	
    // 发多条的时候，如果快速的按取消后，不能再发任何短信。
    if ( cur_sendinfo->send_content.is_need_save && MMISMS_SENDSTATUS_SENDING == cur_sendinfo->backsend_status)
    {//发送多个号码不删除，直接返回，添加的时候启动发送        
        return;        
    }
#endif            
                
    if (PNULL == cur_sendinfo->next)
	{
        s_mmisms_sendlist.sendlist_head_ptr = PNULL;
    }
    else
    {
        s_mmisms_sendlist.sendlist_head_ptr = cur_sendinfo->next;
    }
    s_mmisms_sendlist.send_num--;
    //释放空间
    SCI_FREE(cur_sendinfo);
    cur_sendinfo = PNULL;
    
    if (0 < s_mmisms_sendlist.send_num && PNULL != s_mmisms_sendlist.sendlist_head_ptr)
    {
        s_mmisms_sendlist.sendlist_head_ptr->cur_send_data.cur_send_oper = MMISMS_SEND_SAVE_SMS;
        MMISMS_ActiveSendList();
	}
	else
    {//设置状态
        if (s_mmisms_sendlist.is_sending_sms)
        {
            s_mmisms_sendlist.is_sending_sms = FALSE;
            MAIN_SetMsgState(TRUE);
        }	
    }

    //SCI_TRACE_LOW:"[MMISMS] MMISMS_DeleteSendList s_mmisms_sendlist.send_num = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3568_112_2_18_2_53_23_433,(uint8*)"d", s_mmisms_sendlist.send_num);
}

/*****************************************************************************/
//  Description : Get is sending sms
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsSendingSms( void )
{
    return s_mmisms_sendlist.is_sending_sms;
}

/*****************************************************************************/
//  Description : delete Send Info in the Send list by record id
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:for sms waiting for send
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteInfoInSendListById( uint32 record_id, MN_SMS_STORAGE_E storage)
{
    MMISMS_SEND_T * send_info_ptr = PNULL;

    send_info_ptr = MMISMS_GetSendInfoFromSendListById(record_id, storage);
    if (PNULL != send_info_ptr)
    {
        MMISMS_DeleteSendList();
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : Get Send Info in the Send list by record id
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SEND_T * MMISMS_GetSendInfoFromSendListById( uint32 record_id, MN_SMS_STORAGE_E storage)
{
    MMISMS_SEND_T * send_info_ptr = PNULL;
    MMISMS_SEND_T * cursend_info_ptr = PNULL;
    uint32 send_list_num = 0;

    cursend_info_ptr = s_mmisms_sendlist.sendlist_head_ptr;
    while (PNULL != cursend_info_ptr)
    {
        if (send_list_num >= MMISMS_SENDLIST_MAX_NUM)
        {//保护，大于最大值就不要再找了，找不到处理
            //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSendInfoFromSendListByRecordId send_list_num >=  MMISMS_SENDLIST_MAX_NUM"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3611_112_2_18_2_53_24_434,(uint8*)"");
            break;
    }
        send_list_num++;
        if (storage == cursend_info_ptr->backsend_update_data.save_send_storage && record_id == cursend_info_ptr->backsend_update_data.save_send_record_id[0])
        {
            send_info_ptr = cursend_info_ptr;
            break;
        }
        cursend_info_ptr = cursend_info_ptr->next;
    }

    //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSendInfoFromSendListByRecordId send_list_num ==  %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3623_112_2_18_2_53_24_435,(uint8*)"d", send_list_num);
    return send_info_ptr;
}

/*****************************************************************************/
//  Description : Set the Back send status by record id order info
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetBackSendStatus( MMISMS_BACKSENDSTATUS_E send_status, MMISMS_SEND_T * send_info_ptr)
{
    MMISMS_ORDER_ID_T      order_id = PNULL;
    BOOLEAN                ret_value = FALSE;
    uint32                 longsms_index = 0;

    if (!send_info_ptr->send_content.is_need_save)
    {//不保存的就不需要这个东西了
        return FALSE;
    }
    send_info_ptr->backsend_status = send_status;
    ret_value = MMISMS_GetOrderIdByStorage(&order_id, MMISMS_TYPE_SMS, send_info_ptr->send_content.dual_sys, send_info_ptr->backsend_update_data.save_send_storage, send_info_ptr->backsend_update_data.save_send_record_id[0]);
    if (ret_value && PNULL != order_id)
    {
        order_id->backsend_status = send_status;
        if (order_id->flag.is_concatenate_sms)
        {
            while (PNULL != order_id)
            {
                if (longsms_index >=  MMISMS_SPLIT_MAX_NUM)
                {
                    //SCI_TRACE_LOW:"MMISMS:MMISMS_SetBackSendStatus longsms_index >=  MMISMS_SPLIT_MAX_NUM"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3652_112_2_18_2_53_24_436,(uint8*)"");
                    break;
                }
                order_id->backsend_status = send_status;
                longsms_index++;
                order_id = order_id->next_long_sms_ptr;
            }
        }
        else
        {
            order_id->backsend_status = send_status;
        }
    }
    if (MMISMS_BOX_SENDFAIL == send_info_ptr->cur_box_type || MMISMS_BOX_NOSEND == send_info_ptr->cur_box_type)
    {
        order_id = g_mmisms_global.update_info.order_id_arr[0];
        longsms_index = 0;
        while (PNULL != order_id)
        {
            if (longsms_index >=  MMISMS_SPLIT_MAX_NUM)
            {
                //SCI_TRACE_LOW:"MMISMS:MMISMS_SetBackSendStatus longsms_index >=  MMISMS_SPLIT_MAX_NUM"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3673_112_2_18_2_53_24_437,(uint8*)"");
                break;
            }
            order_id->backsend_status = send_status;
            longsms_index++;
            order_id = order_id->next_long_sms_ptr;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : Active send list change the send_status
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ActiveSendList( void )
{
    MMISMS_OPER_ERR_E send_result = MMISMS_NO_ERR;
    MMISMS_SEND_T * send_info_ptr = s_mmisms_sendlist.sendlist_head_ptr;
    MN_SMS_STORAGE_E  storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T record_id = 0;
    
    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMISMS] MMISMS_ActiveSendList PNULL == send_info_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3697_112_2_18_2_53_24_438,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"[MMISMS] MMISMS_ActiveSendList send_info_ptr->backsend_status = %d !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3700_112_2_18_2_53_24_439,(uint8*)"d", send_info_ptr->backsend_status);

    //预设置record id
    if (send_info_ptr->send_content.is_need_save && MMISMS_SENDSAVE_SAVESUCC == send_info_ptr->backsend_update_data.save_send_result)
    {  
        MMISMS_GetStorageAndRecordidForSendSms(send_info_ptr, &storage, &record_id);
        if (MN_SMS_NO_STORED != storage)
        {
            send_info_ptr->cur_send_data.cur_send_record_id = record_id;
            send_info_ptr->cur_send_data.cur_send_storage = storage;            
        }
        else
        {
            send_info_ptr->send_content.is_need_save = FALSE;
        }
        send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SEND_SAVE_SMS;
    }

    send_result = MMISMS_AppSendSmsToMn(send_info_ptr);
    
    if (MMISMS_NO_ERR == send_result)
    {
        //s_mmisms_sendlist.send_status = MMISMS_SENDLIST_SENDING;
        if (send_info_ptr->send_content.is_need_save)
        {//允许保存的sms才有状态更新
            MMISMS_SetBackSendStatus(MMISMS_SENDSTATUS_SENDING, send_info_ptr);
        }     
        if(PNULL == send_info_ptr->send_callback || send_info_ptr->send_content.is_vacard_vcal)  //other发送的情况不显示
        {
            s_mmisms_sendlist.is_sending_sms = TRUE;
            MAIN_SetMsgState(TRUE);
        }
    }
    else//该条发送失败，取消掉，释放空间，发送下一条
    {
        do 
        {
             if(!MMISMS_CheckNextDestAddr())//发送多个联系人的情况
             {
                 send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDFAIL;
                 if (send_info_ptr->send_content.is_need_save)
                 {
                     MMISMS_SetBackSendStatus(MMISMS_SENDSTATUS_SENDFAIL, send_info_ptr);
                 }        
                 MMISMS_DisplaySendErr(send_info_ptr->send_content.dual_sys, send_result);
                 MMISMS_DeleteSendList();
                 MMISMS_ActiveSendList();
                 break;
             }
             else
             {
                 MMISMS_MoveDestAddrId();
                 MMISMS_InitSendSaveResult(send_info_ptr);
                 send_result = MMISMS_AppSendSmsToMn(send_info_ptr);
                 SCI_TRACE_LOW("mmisms send result %d",send_result);
                 if(MMISMS_NO_ERR == send_result )
                 {
                     break;
                 }
             }        
        }while(1);   //lint !e506
    }
}

/*****************************************************************************/
//  Description : App send a sms message to mn
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppSendSmsToMn( MMISMS_SEND_T *send_info_ptr )
{
    MMISMS_OPER_ERR_E send_result = MMISMS_NO_ERR;
    BOOLEAN is_more_msg = FALSE;
        APP_SMS_USER_DATA_T user_data = {0};
        MN_CALLED_NUMBER_T dest_num = {MN_NUM_TYPE_UNKNOW, MN_NUM_PLAN_UNKNOW, 0, 0};
    BOOLEAN status_report_is_request = MMISMS_GetStatusReportFlag();

    if (PNULL == send_info_ptr)
        {
        return MMISMS_SENDSMS_FAIL;
        }

    if (MMISMS_GetDestNumber(&dest_num, &send_info_ptr->send_content.dest_info))
    {
        MMISMS_GetUserData(send_info_ptr->cur_send_data.seq_num,  &(send_info_ptr->send_content.send_message), &(send_info_ptr->send_content.tpdu_head), TRUE, &user_data, send_info_ptr);
        is_more_msg = (MMISMS_IsExistNextSMS(&send_info_ptr->send_content) || MMISMS_CheckNextDestAddr());
        if((send_info_ptr->cur_send_data.seq_num < send_info_ptr->cur_send_data.total_num) && (send_info_ptr->cur_send_data.total_num > 1))
        {
            status_report_is_request = FALSE;
        }
        if(send_info_ptr->is_force_rpt)
        {
            send_result = SendMsgReqToMN(send_info_ptr->send_content.dual_sys, &dest_num, &user_data, 
                 send_info_ptr->cur_send_data.cur_send_storage, send_info_ptr->send_content.send_message.alphabet, is_more_msg, send_info_ptr->send_content.is_resend, TRUE);
        }
        else
        {
             if (!send_info_ptr->is_need_rpt)//from other modules
             {
                 send_result = SendMsgReqToMN(send_info_ptr->send_content.dual_sys, &dest_num, &user_data, 
                 send_info_ptr->cur_send_data.cur_send_storage, send_info_ptr->send_content.send_message.alphabet, is_more_msg, send_info_ptr->send_content.is_resend, FALSE);
             }
             else
             {
                 send_result = SendMsgReqToMN(send_info_ptr->send_content.dual_sys, &dest_num, &user_data, 
                 send_info_ptr->cur_send_data.cur_send_storage, send_info_ptr->send_content.send_message.alphabet, is_more_msg, send_info_ptr->send_content.is_resend, status_report_is_request);
             }
        }        
    }
    else
    {
        //SCI_TRACE_LOW:"[MMISMS] MMISMS_AppSendSmsToMn sms no dest address!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3781_112_2_18_2_53_24_440,(uint8*)"");
        send_result = MMISMS_NO_DEST_ADDRESS;
    }
    //SCI_TRACE_LOW:"[MMISMS] MMISMS_AppSendSmsToMn send_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3784_112_2_18_2_53_24_441,(uint8*)"d", send_result);
    return send_result;    
}

#ifdef JAVA_SUPPORT_SUN
#ifndef _MSC_VER
/*****************************************************************************/
//  Description : sun java send cnf callback
//  Global resource dependence : 
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISMS_SUNJavaSendCnfCallback(MN_SMS_CAUSE_E cause)
{
    APPSMS_SendResult_CallBack(cause,0);
    return FALSE;
}    
#endif
#endif

/*****************************************************************************/
//  Description : vcard or vacalender send cnf callback
//  Global resource dependence : 
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISMS_VcardOrVcalSendCnfCallback(BOOLEAN is_success, DPARAM parm)
{
    if (is_success)
    {
        MMISMS_OpenMsgBox(TXT_COMM_SEND_SUCCEED, IMAGE_PUBWIN_SUCCESS);
    }
    else
    {
        MMISMS_OpenMsgBox(TXT_COMM_SEND_FAILED, IMAGE_PUBWIN_WARNING);
    }
    CloseSendWinByResult(0);
    MMISMS_ClearOperInfo();
    return TRUE;    
}

/*****************************************************************************/
//  Description : send the SMS for other modules, such as java,vacard,dm,msd,cmcc etc..
//  Global resource dependence :  
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SendSmsForOther(MMISMS_SEND_DATA_T *send_data_ptr)
{
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_ERR;
    MMISMS_SEND_T     *send_info_ptr = PNULL;
    uint8             total_num = 0;
    BOOLEAN           is_head_has_port = FALSE;
    uint16 sim_sys                        = MN_DUAL_SYS_1;
    uint32 sim_num                        = 0;
    MN_DUAL_SYS_E send_sys = MN_DUAL_SYS_1;

    if (PNULL == send_data_ptr || (!send_data_ptr->binary_sms_data.is_binary && PNULL == send_data_ptr->sms_content_ptr))
    {
        //SCI_TRACE_LOW:"MMISMS MMISMS_SendSmsForOther PNULL == send_data_ptr || PNULL == sms_content_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3838_112_2_18_2_53_24_442,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMISMS_SendSmsForOther dual_sys = %d src_port = %d dst_port = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3843_112_2_18_2_53_24_443,(uint8*)"ddd", send_data_ptr->dual_sys, send_data_ptr->src_port, send_data_ptr->dst_port);

    if (PNULL == send_data_ptr->dest_addr_ptr 
        || (!send_data_ptr->binary_sms_data.is_binary && (PNULL == send_data_ptr->sms_content_ptr->wstr_ptr 
        || 0 == send_data_ptr->sms_content_ptr->wstr_len)))
    {
        //SCI_TRACE_LOW:"MMISMS MMISMS_SendSmsForOther PNULL == dest_addr_ptr || PNULL == sms_content_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3849_112_2_18_2_53_24_444,(uint8*)"");
        return FALSE;
    }

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        //SCI_TRACE_LOW:" MMISMS_SendSmsForOther sim is not ok,can't send sms!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3856_112_2_18_2_53_24_445,(uint8*)"");
        return FALSE;
    }

    if (MMIAPIPHONE_IsSimAvailable(send_data_ptr->dual_sys))
    {//如果指定的sim可以用则使用，否则要指定一张可以使用的sim卡
        send_sys = send_data_ptr->dual_sys;
    }
    else
    {
        send_sys = (MN_DUAL_SYS_E)sim_sys;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(send_sys))
    {
        SCI_TRACE_LOW("MMISMS_SendSmsForOther send_sys=%d. Another SIM calling now.", send_sys);

        return FALSE;
    }
#endif

    send_info_ptr = MMISMS_CreateSendSMS();

    if (PNULL == send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS MMISMS_SendSmsForOther PNULL == send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3864_112_2_18_2_53_24_446,(uint8*)"");
        return FALSE;
    }

    send_info_ptr->send_content.dual_sys = send_sys;

    MMISMS_ClearResendCount(send_info_ptr);

    if(!send_data_ptr->binary_sms_data.is_binary)
    {
        MMISMS_SetMessageContent(send_data_ptr->sms_content_ptr->wstr_len, send_data_ptr->sms_content_ptr->wstr_ptr, send_data_ptr->is_need_packet, &send_info_ptr->send_content);
    
        if (MN_SMS_UCS2_ALPHABET != send_info_ptr->send_content.send_message.alphabet && !send_data_ptr->is_need_packet)
        {//如果不需要压缩，则使用8bit编码方式
            send_info_ptr->send_content.send_message.alphabet = MN_SMS_8_BIT_ALPHBET;
        }
    }
    else
    {        
        send_info_ptr->send_content.send_message.length   = MIN(send_data_ptr->binary_sms_data.binary_length , MMISMS_MAX_MESSAGE_LEN);
        SCI_MEMCPY(
            send_info_ptr->send_content.send_message.data,
            send_data_ptr->binary_sms_data.binary_data,
            send_info_ptr->send_content.send_message.length 
            );  
        if(send_data_ptr->is_need_packet)
        {
            send_info_ptr->send_content.send_message.alphabet = MN_SMS_DEFAULT_ALPHABET;
        }
        else
        {
            send_info_ptr->send_content.send_message.alphabet = MN_SMS_8_BIT_ALPHBET;
        }
    }
    MMISMS_SetAddressToMessage(send_data_ptr->dest_addr_ptr, send_data_ptr->dest_addr_len, &send_info_ptr->send_content.dest_info.dest_list);
    if (0 < send_data_ptr->src_port && 0 < send_data_ptr->dst_port)
    {
        is_head_has_port = TRUE;
        send_info_ptr->is_head_has_port = TRUE;
    }

    if (is_head_has_port)
    {
        total_num = MMISMS_SplitLongMsg(&send_info_ptr->send_content.send_message, send_info_ptr->is_head_has_port);
        if (1 < total_num)
        {
            InitUserDataHeadForOtherModules(send_data_ptr->src_port, send_data_ptr->dst_port, &send_info_ptr->send_content.tpdu_head, TRUE);
        }
        else
        {
            InitUserDataHeadForOtherModules(send_data_ptr->src_port, send_data_ptr->dst_port, &send_info_ptr->send_content.tpdu_head, FALSE);
        }
    }

    MMISMS_InitSendSaveResult(send_info_ptr);
    send_info_ptr->send_callback = send_data_ptr->send_callback;
    if (MMISMS_IsSendListEmpty())
    {
        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
    }
    else
    {
        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_WAITING;
    }

    send_info_ptr->start_send_tickcount = SCI_GetTickCount();
    send_info_ptr->is_need_rpt = FALSE;
    send_info_ptr->is_force_rpt = send_data_ptr->is_force_rpt;
    err_code = MMISMS_AppSendSms(send_info_ptr->send_content.dual_sys, FALSE, send_info_ptr);

    if (MMISMS_NO_ERR != err_code)
    {
        //SCI_TRACE_LOW:"MMISMS_SendSmsForOther err_code == %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3922_112_2_18_2_53_24_447,(uint8*)"d", err_code);
        MMISMS_DisplaySendErr(send_info_ptr->send_content.dual_sys, err_code);
        SCI_FREE(send_info_ptr);
        return FALSE;
    }
    return TRUE;
}

#ifdef MMI_INDIAN_SOS_SUPPORT
/*****************************************************************************/
//  Description : send one or more sms messages..
//  Global resource dependence :  
//  Author:Yongli Tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MultiSendSms(MMISMS_MULTI_SEND_DATA_T *send_data_ptr)
{
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_ERR;
    MMISMS_SEND_T *send_info_ptr = PNULL;
    uint8 total_num = 0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    uint8 index = 0;
    uint8 addr_len = 0;

    if (PNULL == send_data_ptr || PNULL == send_data_ptr->sms_content_ptr)
    {
        SCI_TRACE_LOW("MMISMS MMISMS_MultiSendSms PNULL == send_data_ptr || PNULL == sms_content_ptr");
        return FALSE;
    }

    if (0 == send_data_ptr->dest_num_len)
    {
        SCI_TRACE_LOW("MMISMS MMISMS_MultiSendSms no dest info.");
        return FALSE;
    }

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        SCI_TRACE_LOW("MMISMS  MMISMS_MultiSendSms sim is not ok,can't send sms!");
        return FALSE;
    }

    send_info_ptr = MMISMS_CreateSendSMS();

    if (PNULL == send_info_ptr)
    {
        SCI_TRACE_LOW("MMISMS MMISMS_MultiSendSms PNULL == send_info_ptr");
        return FALSE;
    }

    if (MMIAPIPHONE_IsSimAvailable(send_data_ptr->dual_sys))
    {//?????sim??????,????????????sim?
        send_info_ptr->send_content.dual_sys = send_data_ptr->dual_sys;
    }
    else
    {
        send_info_ptr->send_content.dual_sys = (MN_DUAL_SYS_E)sim_sys;
    }

    MMISMS_ClearResendCount(send_info_ptr);

    //set sms content
    MMISMS_SetMessageContent(send_data_ptr->sms_content_ptr->wstr_len, send_data_ptr->sms_content_ptr->wstr_ptr, send_data_ptr->is_need_packet, &send_info_ptr->send_content);
    //set sms dest info
    MMISMS_ClearDestId();
    MMISMS_ClearDestNum();
    for(index = 0; index < send_data_ptr->dest_num_len; index++)
    {
        addr_len = SCI_STRLEN((char *)send_data_ptr->dest_num[index]);
        MMISMS_SetAddressToMessage((uint8 *)send_data_ptr->dest_num[index], addr_len, &g_mmisms_global.edit_content.dest_info.dest_list);
    }
    SCI_MEMCPY(&send_info_ptr->send_content.dest_info, &g_mmisms_global.edit_content.dest_info, sizeof(MMISMS_DEST_ADDR_INFO_T));
    send_info_ptr->send_content.dest_info.dest_list.dest_addr_id = 0;
    MMISMS_InitSendSaveResult(send_info_ptr);
    send_info_ptr->send_callback = send_data_ptr->send_callback;
    if (MMISMS_IsSendListEmpty())
    {
        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
    }
    else
    {
        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_WAITING;
    }

    send_info_ptr->start_send_tickcount = SCI_GetTickCount();
    send_info_ptr->is_need_rpt = TRUE;
    err_code = MMISMS_AppSendSms(send_info_ptr->send_content.dual_sys, TRUE, send_info_ptr);

    if (MMISMS_NO_ERR != err_code)
    {
        SCI_TRACE_LOW("MMISMS_SendSmsForOther err_code == %d",err_code);
        MMISMS_DisplaySendErr(send_info_ptr->send_content.dual_sys, err_code);
        SCI_FREE(send_info_ptr);
        return FALSE;
    }
    return TRUE;
}
#endif

/*****************************************************************************/
//     Description : to set the user data head for other modules who has the 
//                   dst port and src port
//    Global resource dependence : none
//    Author:minghu.mao
//    Note: 
/*****************************************************************************/
LOCAL void InitUserDataHeadForOtherModules(
                                           int32             srcPort,
                                           int32             dstPort,
                                           MN_SMS_USER_DATA_HEAD_T  *data_head_ptr,
                                           BOOLEAN           is_longmsg
                                           )
{
    if (PNULL == data_head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:InitUserDataHeadForOtherModules data_head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3944_112_2_18_2_53_24_448,(uint8*)"");
        return;
        }
        
    //SCI_TRACE_LOW:"MMISMS: InitUserDataHeadForOtherModules (srcPort:%d, dstPort:%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3948_112_2_18_2_53_24_449,(uint8*)"dd", srcPort ,dstPort);
        
    if (!is_longmsg)        //is not a long sms
    {
        data_head_ptr->length                                 = MMISMS_HEAD_LENGTH_FOR_PORT;    
        data_head_ptr->user_header_arr[ MMISMS_HEAD_IEI_POS ] = MMISMS_HEAD_IEI_FOR_PORT;    
        data_head_ptr->user_header_arr[ MMISMS_HEAD_IEL_POS ] = MMISMS_HEAD_IEL_FOR_PORT;    
        
        data_head_ptr->user_header_arr[ MMISMS_HEAD_SRC_PORTH_POS ] = (dstPort>>8)&(0x00ff);
        data_head_ptr->user_header_arr[ MMISMS_HEAD_SRC_PORTL_POS ] =     dstPort&0x00ff;
        data_head_ptr->user_header_arr[ MMISMS_HEAD_DST_PORTH_POS ] = (srcPort>>8)&(0x00ff);        
        data_head_ptr->user_header_arr[ MMISMS_HEAD_DST_PORTL_POS ] = srcPort&(0x00ff);    
    }
    else
    {
        data_head_ptr->length                               += MMISMS_HEAD_LENGTH_FOR_PORT;    
        data_head_ptr->user_header_arr[ MMISMS_HEAD_IEI_POS+MMISMS_HEAD_8BIT_LENGTH ] = MMISMS_HEAD_IEI_FOR_PORT;    
        data_head_ptr->user_header_arr[ MMISMS_HEAD_IEL_POS+MMISMS_HEAD_8BIT_LENGTH ] = MMISMS_HEAD_IEL_FOR_PORT;    
        
        data_head_ptr->user_header_arr[ MMISMS_HEAD_SRC_PORTH_POS+MMISMS_HEAD_8BIT_LENGTH ] = (dstPort>>8)&(0x00ff);    
        data_head_ptr->user_header_arr[ MMISMS_HEAD_SRC_PORTL_POS+MMISMS_HEAD_8BIT_LENGTH ] = dstPort&0x00ff;            
        data_head_ptr->user_header_arr[ MMISMS_HEAD_DST_PORTH_POS+MMISMS_HEAD_8BIT_LENGTH ] = (srcPort>>8)&(0x00ff);    
        data_head_ptr->user_header_arr[ MMISMS_HEAD_DST_PORTL_POS+MMISMS_HEAD_8BIT_LENGTH ] = srcPort&(0x00ff);        
    }
}

/*****************************************************************************/
//     Description : update the sending number info in send fail box
//  Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_UpdateSmsSendindNumber( MMISMS_SEND_T *send_info_ptr )
{
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
    MN_SMS_RECORD_ID_T update_record_id = 0;
    MMISMS_ORDER_ID_T  order_id = PNULL;
    BOOLEAN            ret_value = FALSE;
    uint8 addr_id = 0;
    MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};
    MN_CALLED_NUMBER_T called_number = {0};

    if (PNULL == send_info_ptr)
        {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_UpdateSmsSendindNumber send_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3990_112_2_18_2_53_24_450,(uint8*)"");
        return FALSE;
    }
    if (MMISMS_SENDSTATUS_SENDING != send_info_ptr->backsend_status)
    {//不是发送中的短信不用更新号码
        //SCI_TRACE_LOW:"MMISMS:MMISMS_UpdateSmsSendindNumber backsend_status != MMISMS_SENDSTATUS_SENDING"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_3995_112_2_18_2_53_24_451,(uint8*)"");
        return FALSE;
    }

    if (2 > send_info_ptr->send_content.dest_info.dest_list.number)
    {//单个号码不用更新
        return FALSE;
    }

    update_record_id = send_info_ptr->backsend_update_data.save_send_record_id[0];
    addr_id = send_info_ptr->send_content.dest_info.dest_list.dest_addr_id;
    //SCI_TRACE_LOW:"MMISMS:MMISMS_UpdateSmsSendindNumber dest_addr_id = %d, update_record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_4006_112_2_18_2_53_24_452,(uint8*)"dd", addr_id, update_record_id);
    ret_value = MMISMS_GetOrderIdByStorage(&order_id, MMISMS_TYPE_SMS, send_info_ptr->send_content.dual_sys, send_info_ptr->backsend_update_data.save_send_storage, update_record_id);
    if (ret_value && PNULL != order_id)
    { 
        MMISMS_GetMnCalledNum(send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id], strlen((char*)send_info_ptr->send_content.dest_info.dest_list.dest_addr[addr_id]), 
            &party_number, &called_number);
        while (PNULL != order_id)
        {            
            SCI_MEMCPY(&order_id->number.bcd_number.number, party_number.bcd_num, party_number.num_len);
            order_id->number.bcd_number.number_len = party_number.num_len;
            order_id = order_id->next_long_sms_ptr;
        }

        MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);          
    }
#endif
    return TRUE;
}

/*****************************************************************************/
//    Description : update record id
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_UpdateCurSendRecordId( 
                                         MMISMS_SEND_T * send_info_ptr,           
                                         MN_SMS_STORAGE_E        storage,    
                                         MN_SMS_RECORD_ID_T    record_id,
                                         BOOLEAN is_id_fixed
                                         )
{
    MN_SMS_STORAGE_E        sending_storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T        sending_record_id = 0;
    MN_SMS_RECORD_ID_T        back_mo_send_id = 0;
    BOOLEAN is_find_backsend = FALSE;   
    MMISMS_GetStorageAndRecordidForSendSms(send_info_ptr, &sending_storage, &sending_record_id);
    //SCI_TRACE_LOW:"MMISMS: MMISMS_UpdateCurSendRecordId 11 cur_send_info_ptr->cur_send_data.cur_send_record_id = %d, sending_record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_4041_112_2_18_2_53_25_453,(uint8*)"dd",send_info_ptr->cur_send_data.cur_send_record_id, sending_record_id);
    if (MN_SMS_NO_STORED != send_info_ptr->cur_send_data.cur_send_storage 
        && storage == send_info_ptr->cur_send_data.cur_send_storage
        && record_id == send_info_ptr->cur_send_data.cur_send_record_id)
    {             
        if (MN_SMS_NO_STORED != sending_storage)
        {
//             if (is_id_fixed)
//             {
//                 if (MN_DUAL_SYS_1 == send_info_ptr->send_content.dual_sys || MN_SMS_STORAGE_SIM == send_info_ptr->cur_send_data.cur_send_storage)
//                 {
//                     send_info_ptr->cur_send_data.cur_send_record_id = sending_record_id - 1;
//                 }
//                 else
//                 {
//                     send_info_ptr->cur_send_data.cur_send_record_id = sending_record_id + 1;
//                 }                
//             }
//             else
            {
                send_info_ptr->cur_send_data.cur_send_record_id = sending_record_id;
            }
            
            send_info_ptr->cur_send_data.cur_send_storage = sending_storage;
        }
    }
    if (send_info_ptr->backsend_update_data.is_backsend_update)
    {
        if (MN_SMS_NO_STORED != send_info_ptr->backsend_update_data.backsend_updata_storage 
            && storage == send_info_ptr->backsend_update_data.backsend_updata_storage
            && record_id == send_info_ptr->backsend_update_data.backsend_record_id)
        {
            is_find_backsend = TRUE;
            if (MN_SMS_NO_STORED != sending_storage)
            {
                if (is_id_fixed)
                {
                    SCI_TRACE_LOW("MMISMS: MMISMS_UpdateCurSendRecordId find record id back_mo_send_id = %d, sending_record_id = %d",back_mo_send_id,sending_record_id);
                    MMISMS_GetStorageAndRecordidForSendSmsEx(send_info_ptr, &sending_storage, &back_mo_send_id,&sending_record_id);
                    send_info_ptr->cur_send_data.cur_send_record_id = sending_record_id;
                    send_info_ptr->backsend_update_data.backsend_record_id = back_mo_send_id;              
                }
                else
                {
                    send_info_ptr->backsend_update_data.backsend_record_id = sending_record_id;
                }
                send_info_ptr->backsend_update_data.backsend_updata_storage = sending_storage;
            }
        }
    }
    if (!is_find_backsend && send_info_ptr->mosend_update_data.is_mosend_update)
    {
        if (MN_SMS_NO_STORED != send_info_ptr->mosend_update_data.mosend_updata_storage 
            && storage == send_info_ptr->mosend_update_data.mosend_updata_storage
            && record_id == send_info_ptr->mosend_update_data.mosend_record_id)
        {
            if (MN_SMS_NO_STORED != sending_storage)
            {
                 if (is_id_fixed)
                {
                    SCI_TRACE_LOW("MMISMS: MMISMS_UpdateCurSendRecordId 1 find record id back_mo_send_id = %d, sending_record_id = %d",back_mo_send_id,sending_record_id);
                    MMISMS_GetStorageAndRecordidForSendSmsEx(send_info_ptr, &sending_storage, &back_mo_send_id,&sending_record_id);
                    send_info_ptr->cur_send_data.cur_send_record_id = sending_record_id;
                    send_info_ptr->mosend_update_data.mosend_record_id = back_mo_send_id;
                }
                else
                {
                    send_info_ptr->mosend_update_data.mosend_record_id = sending_record_id;
                }
                send_info_ptr->mosend_update_data.mosend_updata_storage = sending_storage;
            }
        }
    }
}


/*****************************************************************************/
//    Description : to get the record id and storage for sending or saving
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetStorageAndRecordidForSendSmsEx( 
                                                   MMISMS_SEND_T * send_info_ptr,           //IN
                                                   MN_SMS_STORAGE_E        *storage_ptr,    //OUT:
                                                   MN_SMS_RECORD_ID_T    *record_id1_ptr,     //OUT:
                                                   MN_SMS_RECORD_ID_T    *record_id2_ptr     //OUT:
                                                   )
{
    MN_SMS_STORAGE_E  storage = MN_SMS_NO_STORED;
    BOOLEAN is_find_free_record = FALSE;
    MN_SMS_RECORD_ID_T record_id1 = 0;
    MN_SMS_RECORD_ID_T record_id2 = 0;
    if (PNULL == send_info_ptr || PNULL == storage_ptr || PNULL == record_id1_ptr || PNULL == record_id2_ptr)
    {
        return;
    }
    // check whether the storage space is enough
    if (send_info_ptr->cur_send_data.total_num >1)
    {
        if (1 == send_info_ptr->cur_send_data.seq_num ) //长短信的第一条，查找位置，以保证所有的sms保存在同一个位置。
        {
            MMISMS_GetStoreStorageForSendLongSms(send_info_ptr->send_content.dual_sys,send_info_ptr->cur_send_data.total_num, send_info_ptr->cur_send_data.seq_num, &storage);
            send_info_ptr->cur_send_data.cur_send_storage = storage;
            g_mmisms_global.save_info.storage = storage;
        }
        else
        {
            storage = send_info_ptr->cur_send_data.cur_send_storage;
        }                
        is_find_free_record = MMISMS_FindFreeRecordIdEx(send_info_ptr->send_content.dual_sys,storage, &record_id1,&record_id2);
        if(!is_find_free_record)
        {
            storage = MN_SMS_NO_STORED;
        }
    }
    else
    {
        MMISMS_GetStoreStorageEx(send_info_ptr->send_content.dual_sys,&storage, &record_id1,&record_id2);
    }
    if (MN_SMS_NO_STORED != storage)
    {
        *storage_ptr = storage;
        *record_id1_ptr = record_id1;
        *record_id2_ptr = record_id2;
    }
}

/*****************************************************************************/
//    Description : to get the record id and storage for sending or saving
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetStorageAndRecordidForSendSms( 
                                                   MMISMS_SEND_T * send_info_ptr,           //IN
                                                   MN_SMS_STORAGE_E        *storage_ptr,    //OUT:
                                                   MN_SMS_RECORD_ID_T    *record_id_ptr     //OUT:
                                                   )
{
    MN_SMS_STORAGE_E  storage = MN_SMS_NO_STORED;
    BOOLEAN is_find_free_record = FALSE;
    MN_SMS_RECORD_ID_T record_id = 0;

    if (PNULL == send_info_ptr || PNULL == storage_ptr || PNULL == record_id_ptr)
    {
        return;
    }
    // check whether the storage space is enough
    if (send_info_ptr->cur_send_data.total_num >1)
    {
        if (1 == send_info_ptr->cur_send_data.seq_num ) //长短信的第一条，查找位置，以保证所有的sms保存在同一个位置。
        {
            MMISMS_GetStoreStorageForSendLongSms(send_info_ptr->send_content.dual_sys,send_info_ptr->cur_send_data.total_num, send_info_ptr->cur_send_data.seq_num, &storage);
            send_info_ptr->cur_send_data.cur_send_storage = storage;
            g_mmisms_global.save_info.storage = storage;
        }
        else
        {
            storage = send_info_ptr->cur_send_data.cur_send_storage;
        }                
        is_find_free_record = MMISMS_FindFreeRecordId(send_info_ptr->send_content.dual_sys,storage, &record_id);
        if(!is_find_free_record)
        {
            storage = MN_SMS_NO_STORED;
        }
    }
    else
    {
        MMISMS_GetStoreStorage(send_info_ptr->send_content.dual_sys,&storage, &record_id);
    }
    if (MN_SMS_NO_STORED != storage)
    {
        *storage_ptr = storage;
        *record_id_ptr = record_id;
    }
}

/*****************************************************************************/
//    Description : to set the back send update info
//    Global resource dependence : g_mmisms_global
//    Author:
//    Note:used for delete sms after send
/*****************************************************************************/
PUBLIC void MMISMS_SetBackSendUpdateInfo( 
                                         MMISMS_ORDER_ID_T    order_id,
                                         MMISMS_SEND_T * send_info_ptr
                                         )
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    int32 i = 0;
    
    SCI_MEMSET(((void*)&send_info_ptr->backsend_update_data.backsend_update_info),
        0,
        sizeof(MMISMS_UPDATE_INFO_T));
    cur_order_id = order_id;
    if (order_id->flag.is_concatenate_sms) //is long sms
                {
        while (cur_order_id != PNULL)
        {
            if (i >=  MMISMS_SPLIT_MAX_NUM)
            {
                //SCI_TRACE_LOW:"MMISMS:MMISMS_SetSMSUpdateInfo i >=  MMISMS_SPLIT_MAX_NUM"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_4194_112_2_18_2_53_25_454,(uint8*)"");
                break;
                }
            send_info_ptr->backsend_update_data.backsend_update_info.order_id_arr[i] = cur_order_id;
            i ++;
            cur_order_id = cur_order_id->next_long_sms_ptr;
            }
        }
    else
    {
        send_info_ptr->backsend_update_data.backsend_update_info.order_id_arr[0] = order_id;
    }    
}

/*****************************************************************************/
//    Description : to set the mo send update info
//    Global resource dependence : g_mmisms_global
//    Author:
//    Note:used for delete sms after send
/*****************************************************************************/
PUBLIC void MMISMS_SetMoSendUpdateInfo( 
                                         MMISMS_ORDER_ID_T    order_id,
                                         MMISMS_SEND_T * send_info_ptr
                                         )
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    int32 i = 0;
    if(PNULL == order_id)
    {
        return;
    }
    SCI_MEMSET(((void*)&send_info_ptr->mosend_update_data.mosend_update_info),
        0,
        sizeof(MMISMS_UPDATE_INFO_T));
    cur_order_id = order_id;
    if (order_id->flag.is_concatenate_sms) //is long sms
        {
        while (cur_order_id != PNULL)
        {
            if (i >=  MMISMS_SPLIT_MAX_NUM)
            {
                //SCI_TRACE_LOW:"MMISMS:MMISMS_SetSMSUpdateInfo i >=  MMISMS_SPLIT_MAX_NUM"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_4230_112_2_18_2_53_25_455,(uint8*)"");
                break;
        }
            send_info_ptr->mosend_update_data.mosend_update_info.order_id_arr[i] = cur_order_id;
            i ++;
            cur_order_id = cur_order_id->next_long_sms_ptr;
        }
    }
    else
    {
        send_info_ptr->mosend_update_data.mosend_update_info.order_id_arr[0] = order_id;
    }    
}
#ifdef MMI_SMS_MINI_SUPPORT
/*****************************************************************************/
//     Description : to get the content of message
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void    MMISMS_GetSMSEditContent( 
                                    MMI_STRING_T  *string_ptr
                                    )
{
    MMISMS_CONTENT_T *msg_content_ptr = PNULL;
    uint16 content_len = 0;
    wchar *content_r8_ptr = PNULL;
#if 0
// no use
    uint32 i = 0;
#endif
    
    if (PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSMSContent string_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2423_112_2_18_2_53_21_396,(uint8*)"");
        return;
    }
    content_r8_ptr = SCI_ALLOCAZ( ( MMISMS_R8_MESSAGE_LEN + 1 )*sizeof(wchar) );
    if(PNULL == content_r8_ptr)
    {
        return;
    }    
    //SCI_MEMSET(content_r8_ptr,0x0,(sizeof(wchar) * ( MMISMS_R8_MESSAGE_LEN + 1 )));
    msg_content_ptr = &g_mmisms_global.edit_content.send_message;
    
    string_ptr->wstr_ptr = SCI_ALLOCAZ((msg_content_ptr->length + 1)*sizeof(wchar));
    if(PNULL == string_ptr->wstr_ptr)
    {
        SCI_FREE(content_r8_ptr);
        content_r8_ptr = PNULL;
        return;
    }
    //SCI_MEMSET(string_ptr->wstr_ptr, 0x00, ((msg_content_ptr->length + 1)*sizeof(wchar)));
    
    if (MN_SMS_UCS2_ALPHABET == msg_content_ptr->alphabet)
    {
       //defined(__BIG_ENDIAN) || defined(__BigEndian)
        MMI_MEMCPY((uint8 *)string_ptr->wstr_ptr,
            msg_content_ptr->length,
            msg_content_ptr->data,
            msg_content_ptr->length,
            msg_content_ptr->length
            );
#if 0
        GUI_UCS2L2UCS2B((uint8 *)(string_ptr->wstr_ptr),
            msg_content_ptr->length,
            (uint8 *)msg_content_ptr->data,
            msg_content_ptr->length);
#endif
        
        string_ptr->wstr_len  += msg_content_ptr->length/sizeof(wchar);
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS_R8:MMISMS_GetSMSContent language is %d,is_lock_shift is %d,is_single_shift is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2472_112_2_18_2_53_21_397,(uint8*)"ddd",g_mmisms_global.read_msg.language,g_mmisms_global.read_msg.is_lock_shift,g_mmisms_global.read_msg.is_single_shift);
        
        if(MMISMS_IsR8Language(msg_content_ptr->language))
        {
            content_len = MMISMS_Default2NationalR8(
                msg_content_ptr->data,
                content_r8_ptr,
                msg_content_ptr->length,
                g_mmisms_global.read_msg.language,
                g_mmisms_global.read_msg.is_lock_shift,
                g_mmisms_global.read_msg.is_single_shift
                );
            
            //SCI_TRACE_LOW:"MMISMS_R8:MMISMS_GetSMSContent content_len is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_2485_112_2_18_2_53_21_398,(uint8*)"d",content_len);
            
            MMI_WSTRNCPY(
                string_ptr->wstr_ptr,
                MMISMS_MAX_MESSAGE_LEN,
                (const wchar *)content_r8_ptr,
                content_len,
                content_len);
            
            string_ptr->wstr_len += content_len;
        }
        else
        {
            uint16 default_len = 0;
            
            default_len = MMIAPICOM_Default2Wchar(
                msg_content_ptr->data,
                string_ptr->wstr_ptr,
                msg_content_ptr->length
                );
            
            string_ptr->wstr_len += default_len;
        }
    }
    
    SCI_FREE(content_r8_ptr);
    content_r8_ptr = PNULL;
}
#endif

#ifdef MMI_MSG_RESEND
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISMS_ResendDelayed(void)
{
    SCI_TRACE_LOW("MMISMS_ResendDelayed");

    if (s_resend_timer_id != 0)
    {
        MMK_StopTimer(s_resend_timer_id);  
        s_resend_timer_id = 0;
    }

    if (!MMISMS_GetResendFlag())
    {
        return FALSE;
    }
       
    s_resend_timer_id = MMK_CreateTimerCallback(30000, MMISMS_HandleResendTimer, PNULL, FALSE);

    s_temp_cnt = 0;
    return TRUE;
}

/****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsProcessing(void)
{
	BOOLEAN ret = FALSE;

	if (MMISMS_GetIsSendingSms()
		|| !MMISMS_IsOrderOk()
		|| MMK_IsOpenWin(MMISMS_READSMS_WAIT_WIN_ID)
		|| MMK_IsOpenWin(MMISMS_SAVEMSG_ANIM_WIN_ID)
		|| MMK_IsOpenWin(MMISMS_SEND_ANIM_WIN_ID)
		|| MMK_IsOpenWin(MMISMS_TIMERMSG_WAIT_WIN_ID)
		|| MMK_IsOpenWin(MMISMS_COPY_ANIM_WIN_ID)
		|| MMK_IsOpenWin(MMISMS_MOVE_ANIM_WIN_ID)
		|| MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID)
		)
	{
		ret = TRUE;
	}

	SCI_TRACE_LOW("MMISMS_IsProcessing ret = %d", ret);

	return ret;
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void MMISMS_HandleResendTimer(uint8 timer_id, uint32 param)
{
//	BOOLEAN				is_ps_ready = FALSE;
//	BOOLEAN				is_operating = FALSE;
	BOOLEAN				is_resend = TRUE;
//	MN_DUAL_SYS_E		dual_sys = MMI_DUAL_SYS_MAX;
	MMISMS_ORDER_ID_T	order_id = PNULL;
	MMISMS_ORDER_ID_T	cur_order_id = PNULL;
	int					i = 0;

	s_temp_cnt++;
	SCI_TRACE_LOW("MMISMS_HandleResendTimer s_temp_cnt = %d", s_temp_cnt);

	order_id = MMISMS_GetLatestSendFailMsg();

	if (PNULL != order_id)
	{
		is_resend = MMIAPIPHONE_GetIsPsReady(order_id->flag.dual_sys);
		SCI_TRACE_LOW("MMISMS_HandleResendTimer is_resend = %d", is_resend);
		is_resend = is_resend && !MMISMS_IsProcessing();

		if (is_resend)
		{
			MMISMS_ClearOperInfo();
			MMISMS_ReadyReadSms();
			MMISMS_SetOperStatus(MMISMS_READ_RESEND_SMS); 
			g_mmisms_global.operation.cur_order_index = 0;
			SCI_MEMSET(g_mmisms_global.operation.cur_order_id_arr, 0 , sizeof(g_mmisms_global.operation.cur_order_id_arr));                         
                     
			if (order_id->flag.is_concatenate_sms) //is long sms
			{
				cur_order_id = order_id;
				while (cur_order_id != PNULL)
				{
					MMISMS_SetCurOrderId(i, cur_order_id); //设置当前order_id到全局变量数组
					cur_order_id  = cur_order_id->next_long_sms_ptr;
					i++;
				}
			}
			else //is normal sms
			{
				MMISMS_SetCurOrderId(0, order_id); //设置当前order_id到全局变量数组 
			}
            
            order_id->flag.is_resend = TRUE;
            
			MNSMS_ReadSmsEx((MN_DUAL_SYS_E)order_id->flag.dual_sys, (MN_SMS_STORAGE_E)order_id->flag.storage, order_id->record_id);
		}
		else
		{
			MMISMS_ResendDelayed();
		}
	}

    return;
}
#endif

/*****************************************************************************/
//  Description : resend sms list change the send_status
//  Global resource dependence : g_mmisms_global
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ResendSms(void)
{
    MMISMS_OPER_ERR_E send_result = MMISMS_NO_ERR;
    MMISMS_SEND_T * send_info_ptr = s_mmisms_sendlist.sendlist_head_ptr;
    
    if (PNULL == send_info_ptr)
    {
        return;
    }
    SCI_TRACE_LOW("mmisms resend sms");

    if(MMISMS_SENDSTATUS_SENDING == send_info_ptr->backsend_status)
    {
        send_result = MMISMS_AppSendSmsToMn(send_info_ptr);
    }
                                   
    if (MMISMS_NO_ERR != send_result)
    {
        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDFAIL;
        if (send_info_ptr->send_content.is_need_save)
        {
            MMISMS_SetBackSendStatus(MMISMS_SENDSTATUS_SENDFAIL, send_info_ptr);
        }        
        MMISMS_DisplaySendErr(send_info_ptr->send_content.dual_sys, send_result);
        MMISMS_DeleteSendList();
        MMISMS_ActiveSendList();
    }
}

/*****************************************************************************/
//  Description : get is need resend sms
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_IsNeedResend(MN_SMS_MO_FAIL_CAUSE_E cause)
{
    BOOLEAN ret_val = FALSE;

    if (MN_SMS_CAUSE_NO_VALUE == cause)
    {
        return ret_val;
    }

    switch(cause)
    {
    case MN_SMS_CAUSE_SMS_TRANSFER_REJECTED:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_DEST_OUT_OF_ORDER:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_NET_OUT_OF_ORDER:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_TEMP_FAILURE:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_CONGESTION:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_RESOURCES_UNAV:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_INVALID_SMS_TRANS_REF_VAL:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_INVALID_MSG_UNSPECIFIED:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_MAND_IE_ERROR:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_MSG_NONEXISTENT:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_MSG_GEN_ERROR:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_IE_NONEXISTENT:
         ret_val = TRUE;
         break;

    case MN_SMS_CAUSE_PROTOCOL_ERROR:
         ret_val = TRUE;
         break;

    case 17:                           //mn_type.h中枚举结构体MN_SMS_MO_FAIL_CAUSE_E里没有定义，暂时使用立即数
         ret_val = TRUE;
         break;

    case 22:
         ret_val = TRUE;
         break;

    case 11:
         ret_val = TRUE;
         break;

    case 36:
         ret_val = TRUE;
         break;

    case 61:
         ret_val = TRUE;
         break;

    case 255:
         ret_val = TRUE;
         break;

    default:
               
         ret_val = FALSE;
         break;
    }

    return ret_val;
}

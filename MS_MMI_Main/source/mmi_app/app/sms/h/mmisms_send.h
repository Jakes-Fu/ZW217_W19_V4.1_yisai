/*************************************************************************
 ** File Name:      mmisms_send.h                                        *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about sending sms     *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/8/23      liming.deng      Create.                              *
*************************************************************************/

#ifndef _MMISMS_SEND_H_
#define _MMISMS_SEND_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmisms_app.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/*****************************************************************************/
//     Description : clear resend count
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ClearResendCount(MMISMS_SEND_T *send_info_ptr);

/*****************************************************************************/
//     Description : to send the SMS for MSD
//    Global resource dependence :  
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SendInit(void);

/*****************************************************************************/
//     Description : to handle the signal of APP_MN_SEND_SMS_CNF
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSendCnf(DPARAM param );

/*****************************************************************************/
//     Description : to send the SMS ,called by window
//    Global resource dependence : , 
//                               g_mmisms_global,
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E  MMISMS_AppSendSms (
                                             MN_DUAL_SYS_E    dual_sys,
                                             BOOLEAN is_need_save,
                                             MMISMS_SEND_T *send_info_ptr
                                             );
/*****************************************************************************/
//     Description : to get the save flag
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowSendResult(
                          uint8        fail_count,
                          uint8        *fail_id_ptr
                          );

/*****************************************************************************/
//     Description : to get the destinations which are fail to sent
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetResultofSending(                        //RETURN: the number failed to send
                                uint8   *status_id_ptr, //OUT: the id of fail to send 
                                uint16    *dest_count_ptr    //OUT: 
                                );
/*****************************************************************************/
//     Description : to split the long SMS 
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_SplitLongMsg(                                    //RETURN: the max number of message
                         MMISMS_CONTENT_T    *sms_content_ptr,//IN: the content of need send
                         BOOLEAN             is_head_has_port
                         );

/*****************************************************************************/
//     Description : to check whether it has the next dest addr
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISMS_CheckNextDestAddr( void );


/*****************************************************************************/
//     Description : to move the id to next addr
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC    BOOLEAN MMISMS_MoveDestAddrId( void );

/*****************************************************************************/
//     Description : to check whether it has the next SMS to send according to the 
//                head of user data
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsExistNextSMS ( MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr );

/*****************************************************************************/
//     Description : to set the user data head
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_InitUserDataHead(
                           uint8                    max_num,        //IN:
                           uint8                    seq_num,        //IN: the seq number (1 ~ 5)
                           MN_SMS_USER_DATA_HEAD_T  *data_head_ptr,  //OUT: the user data head
                           MMISMS_SEND_T *send_info_ptr
                           );

/*****************************************************************************/
//     Description : hanlde when receive write_cnf after send_cnf
//    Global resource dependence : , 
//                               g_mmisms_global,
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_HandleSendCnfWhenWriteCnf( MMISMS_SEND_STATUS_E send_status);

/*****************************************************************************/
//     Description : to clear the id of destinations in 
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ClearDestId( void );

/*****************************************************************************/
//     Description : to add the phonenumber to the global variable
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetAddressToMessage(
                                       const uint8        *num_ptr,    //IN:
                                       uint8            num_len,        //IN:
                                       MMISMS_NUMBER_LIST_T *dest_list_ptr
                                       );

/*****************************************************************************/
//     Description : to get the dest addr 
//    Global resource dependence : , 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDestNumber(
                                    MN_CALLED_NUMBER_T    *dest_number_ptr,    //OUT:
                                    MMISMS_DEST_ADDR_INFO_T *dest_info_ptr
                                    );

/*****************************************************************************/
//     Description : to save the address for update
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SaveUpdateAddr( BOOLEAN is_edit);

/*****************************************************************************/
//     Description : to get the amount of the destination number
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetDestCount( void );

/*****************************************************************************/
//     Description : to get the origin number form MT SMS
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetOriginNum(
                          MN_DUAL_SYS_E *dual_sys_ptr,
                          uint8    *num_ptr,        //OUT:
                          uint8    max_num_len        //IN:
                          );
/*****************************************************************************/
//     Description : to get the origin number from order info
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetOriginNumByPos(                    //RETURN: the length of num_ptr
                                      MN_DUAL_SYS_E     *dual_sys_ptr,
                                      uint8                *num_ptr,        //OUT:
                                      uint8                 max_num_len);
/*****************************************************************************/
//     Description : to get the destation number according the pointed position
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetPointedDestNum(                            //RETURN: 
                                        uint16         index,            //IN: start from 0
                                        uint8        *num_len_ptr,    //OUT:
                                        uint8        *num_ptr        //OUT:
                                        );

/*****************************************************************************/
//     Description : to delete the destination number according the pointed position
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteDestNum(
                             uint16    pos // start from 0
                             );

/*****************************************************************************/
//     Description : to set the save flag
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSaveFlag( 
                        BOOLEAN     is_save
                        );

/*****************************************************************************/
//     Description : to get the save flag
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetSaveFlag(void);

/*****************************************************************************/
//     Description : to get the content of message
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void  MMISMS_GetSMSContent( 
                             MMI_STRING_T  *string_ptr
                             );

/*****************************************************************************/
//     Description : to get the content of message
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetSMSStringFromContent( 
                            MMISMS_CONTENT_T    *msg_content_ptr,
                            MMI_STRING_T  *string_ptr
                            );


#ifdef MMI_SMS_MINI_SUPPORT
/*****************************************************************************/
//     Description : to get the content of message
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void  MMISMS_GetSMSEditContent( 
                             MMI_STRING_T  *string_ptr
                             );
#endif

#ifdef MMI_MSG_RESEND
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMISMS_ResendDelayed(void);
#endif

/*****************************************************************************/
//     Description : to save the message to global var
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void    MMISMS_SetReadMsgTOSend( MMISMS_SEND_T *send_info_ptr );

/*****************************************************************************/
//     Description : 窗口模块调用该函数进行短消息的发送
//    Global resource dependence : 
//  Author:liming deng
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_WintabSendSMS(BOOLEAN is_re_send);

#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
//     Description : 窗口模块调用该函数进行定时短消息的发送
//    Global resource dependence : 
//  Author:liming deng
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_WintabSendTimerSMS(BOOLEAN is_re_send);
#endif


/*****************************************************************************/
//     Description : hanlde write cnf error 
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
void MMISMS_HandleWriteError( void );

/*****************************************************************************/
//     Description : set dest dual sys
//    Global resource dependence : 
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
void    MMISMS_SetDestDualSys( MN_DUAL_SYS_E dual_sys );

/*****************************************************************************/
//     Description : get dest dual sys
//    Global resource dependence : 
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
MN_DUAL_SYS_E    MMISMS_GetDestDualSys( void );

#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description :发送Vcard短信
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_WintabSendVcard(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
// Description : Get current ref number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Current_Ref_Num(MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr);

/*****************************************************************************/
// Description : Get current max number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Head_Max_Num(MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr);

/*****************************************************************************/
// Description : Get current seq number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Current_Seq_Num(MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr);

/*****************************************************************************/
// Description : Set current seq number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void Set_Current_Seq_Num(MN_SMS_USER_DATA_HEAD_T   *head_ptr,
                                uint8                      seq_num);

/*****************************************************************************/
// Description : Set long sms head data
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void Set_Longsms_Head_Var(MN_SMS_USER_DATA_HEAD_T   *head_ptr,
                                 uint8                      ref_num,
                                 uint8                      max_num,
                                 uint8                      seq_num);

/*****************************************************************************/
// Description : Set R8 sms head data
// Global resource dependence:
// Author:fengming.huang
// Note: 
/*****************************************************************************/
PUBLIC void Set_SmsR8_Head_Var(MN_SMS_USER_DATA_HEAD_T   *head_ptr,
                                 MMISMS_R8_LANGUAGE_E   language,
                                 BOOLEAN   single_shift);



#ifdef JAVA_SUPPORT_IA
/*****************************************************************************/
//    Description : MMISMS_AppSendCommonSmsForJAVA,no has sms user header
//    Global resource dependence :
//    Author:pan.yang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_AppSendCommonSmsForJAVA(
                                        uint8	        *sms_data_ptr,	//IN:
                                        int32          sms_data_len,
                                        uint8            *dest_addr_ptr,    //IN:
                                        uint8            dest_addr_len,    //IN:
                                        int32          type
                                        );
#endif
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
                                                                        MN_CALLED_NUMBER_T *dest_number_ptr    //OUT:
                                                                        );

/*****************************************************************************/
// Description : Set Deliver Cancel
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetDeliverCancel(BOOLEAN is_deliver_cancel);

/*****************************************************************************/
// Description : Get Is Deliver Cancel
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDeliverCancel(void);

/*****************************************************************************/
//     Description : to send next message or re_send the current message
//    Global resource dependence : g_mmisms_global
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_SendMsgBySeqNum(
                                        MN_DUAL_SYS_E   dual_sys,
                                        uint8   total_num,
                                        uint8    seq_num,
                                        BOOLEAN    is_resend,
                                        MMISMS_SEND_T *send_info_ptr
                                        );

/*****************************************************************************/
// Description : to get the origin number form unit info
// Global resource dependence : s_mmisms_linked_order
// Author:liming.deng
// Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetOriginNumBySearchResult(
                                      MN_DUAL_SYS_E     *dual_sys_ptr,
                                      uint8             *num_ptr,       //OUT:
                                      uint8             max_num_len    //IN:                                          
                                      );

/*****************************************************************************/
// Description : to get the subject form unit info
// Global resource dependence : 
// Author:plum.peng
// Note:
/*****************************************************************************/
PUBLIC MMISMS_SMSDISPLAY_CONTENT_T* MMISMS_GetSubjectBySearchResult(void);

/*****************************************************************************/
//     Description : to clear the dest addr in 
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ClearDestAddrById( void );

/*****************************************************************************/
//  Description : to get the resend flag
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDeliverResendFlag(void);

/*****************************************************************************/
//  Description : to set the resend flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetDeliverResendFlag( 
                                   BOOLEAN     is_resend
                                   );

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
                                                                                BOOLEAN is_need_modified);

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description :发送Vcalendar短信
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_WintabSendVcalendar(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//  Description : create a send message
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SEND_T *MMISMS_CreateSendSMS( void );

/*****************************************************************************/
//  Description : get the edit content from global
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:该结构在sms编辑时候用
/*****************************************************************************/
PUBLIC MMISMS_SMSEDIT_CONTENT_T * MMISMS_GetEditContentFromGlobal( void );

/*****************************************************************************/
//  Description : Add sendinfo to send list
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AddSendList( MMISMS_SEND_T *sendinfo );
/*****************************************************************************/
//  Description : Delete sendinfo to send list
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_DeleteSendList(void);

/*****************************************************************************/
//  Description : Active send list change the send_status
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ActiveSendList( void );

/*****************************************************************************/
//  Description : Set the Back send status by record id order info
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetBackSendStatus( MMISMS_BACKSENDSTATUS_E send_status, MMISMS_SEND_T * send_info);

/*****************************************************************************/
//  Description : Get Send Info in the Send list by record id
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SEND_T * MMISMS_GetSendInfoFromSendListById( uint32 record_id, MN_SMS_STORAGE_E storage);

/*****************************************************************************/
//  Description : App send a sms message to mn
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppSendSmsToMn( MMISMS_SEND_T *sendinfo );

/*****************************************************************************/
//  Description : get the send info from global
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SEND_T * MMISMS_GetSendInfoFromSendLIst( MMISMS_BACKSENDSTATUS_E status );

#ifdef MMI_INDIAN_SOS_SUPPORT
/*****************************************************************************/
//  Description : send one or more sms messages..
//  Global resource dependence :  
//  Author:Yongli Tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MultiSendSms(MMISMS_MULTI_SEND_DATA_T *send_data_ptr);
#endif

/*****************************************************************************/
//  Description : send the SMS for other modules
//  Global resource dependence :  
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SendSmsForOther(MMISMS_SEND_DATA_T *send_data_ptr);
/*****************************************************************************/
//  Description : Delete all message in send list
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_DeleteAllMsgInList( void );

/*****************************************************************************/
//  Description : delete Send Info in the Send list by record id
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:for sms waiting for send
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteInfoInSendListById( uint32 record_id, MN_SMS_STORAGE_E storage);

/*****************************************************************************/
//  Description : is send list empty
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsSendListEmpty( void );

/*****************************************************************************/
//  Description :判断是否 sendlist num大于1且链表中短信状态都为saving状态
//  Parameter: [In] NULL
//             [out] NULL
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsAllSendListSaving( void );

/*****************************************************************************/
//     Description : update the sending number info in sengfail box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_UpdateSmsSendindNumber( MMISMS_SEND_T *send_info_ptr );

/*****************************************************************************/
//  Description : Get is sending sms
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsSendingSms( void );

/*****************************************************************************/
//    Description : to set the back send update info
//    Global resource dependence : g_mmisms_global
//    Author:
//    Note:used for delete sms after send
/*****************************************************************************/
PUBLIC void MMISMS_SetBackSendUpdateInfo( 
                                         MMISMS_ORDER_ID_T    order_id,
                                         MMISMS_SEND_T * send_info_ptr
                                         );

/*****************************************************************************/
//    Description : to set the mo send update info
//    Global resource dependence : g_mmisms_global
//    Author:
//    Note:used for delete sms after send
/*****************************************************************************/
PUBLIC void MMISMS_SetMoSendUpdateInfo( 
                                         MMISMS_ORDER_ID_T    order_id,
                                         MMISMS_SEND_T * send_info_ptr
                                         );
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
                                                   );

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
                                                   );

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
                                         );

/*****************************************************************************/
//  Description : resend sms list 
//  Global resource dependence : s_mmisms_sendlist
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ResendSms(void);

/*****************************************************************************/
//     Description : to handle after received the send cnf
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void OperAfterSendCnf( 
                             MN_DUAL_SYS_E dual_sys,
                             BOOLEAN is_send_success,
                              BOOLEAN is_finish);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_SEND_H_

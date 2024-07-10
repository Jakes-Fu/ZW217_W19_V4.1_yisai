/*************************************************************************
 ** File Name:      mmisms_save.h                                      *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about saving sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/8/23      liming.deng      Create.                              *
*************************************************************************/

#ifndef _MMISMS_SAVE_H_
#define _MMISMS_SAVE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmisms_order.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*****************************************************************************/
//     Description : to handle ps signal of APP_MN_WRITE_SMS_CNF
//    Global resource dependence : g_mmisms_global                               
//    Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSaveSMSCnf (
                                    DPARAM param
                                    );

/*****************************************************************************/
//     Description : to save the SMS , called by window
//    Global resource dependence : g_mmisms_global
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppSaveSms( MN_DUAL_SYS_E dual_sys , BOOLEAN is_send_save);

/*****************************************************************************/
//  Description :判断是否 sendlist num大于1且链表中短信状态都为saving状态
//  Parameter: [In] NULL
//             [out] NULL
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppSaveSmsEx(MMISMS_SEND_T  *send_info_ptr, BOOLEAN is_send_save);

/*****************************************************************************/
//     Description : to save the info to order list by the result of save
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC     void MMISMS_SaveInfoToOrderByResult(
                                    MN_DUAL_SYS_E            dual_sys, 
                                    MN_SMS_STORAGE_E        save_storage,
                                    MN_SMS_RECORD_ID_T        save_record_id,
                                    MN_SMS_STATUS_E            status,
                                    BOOLEAN                    is_exist_addr,
                                    MMISMS_FOLDER_TYPE_E       folder_type,
                                    MMISMS_OPER_STATUS_E oper_status,
                                    MMISMS_SEND_T *send_message_ptr,
                                    MMISMS_BACKSENDSTATUS_E send_status
                                    );

/*****************************************************************************/
//     Description : to save the SMS , called by window
//    Global resource dependence : g_mmisms_global
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppUpdateMoSms( 
                                                MN_DUAL_SYS_E        dual_sys,
                                                MMISMS_OPER_STATUS_E	update_oper,	//IN:
                                                MN_SMS_STATUS_E update_status,
                                                MMISMS_SEND_T *send_message_ptr
                                               );

/*****************************************************************************/
//     Description : to save the next SMS 
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_SaveNextMessage(
                                                MN_DUAL_SYS_E dual_sys,
                                                uint8    *dest_addr_ptr,    //IN:
                                                uint8   total_num,        //IN
                                                uint8   seq_num ,           //IN: the seq number ( 1 ~ 8)
                                                MMISMS_SEND_T *send_message_ptr
                                                );

/*****************************************************************************/
//     Description : to save the Class0 SMS 
//    Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_SaveClass0Msg( void );

/*****************************************************************************/
//     Description : to check whether there have enough spaces to save
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_HaveEnoughSpace(
                               MN_DUAL_SYS_E dual_sys,
                               BOOLEAN    is_new_message,    //IN:
                               uint8    org_num            //IN:
                               );



/*****************************************************************************/
//     Description : encode MOSMS data
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EncodeMoSmsData(
                           MN_DUAL_SYS_E    dual_sys,
                           MN_SMS_MO_SMS_T            *mo_sms_ptr,        //OUT:
                           MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN:
                           APP_SMS_USER_DATA_T         *user_data_ptr        //IN:
                           );

/*****************************************************************************/
//     Description : encode MT SMS data
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EncodeMtSmsData(
                                   MN_DUAL_SYS_E    dual_sys,
                                   MN_SMS_MT_SMS_T            *mt_sms_ptr,        //OUT:
                                   MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN:
                                   APP_SMS_USER_DATA_T         *user_data_ptr        //IN:
                                   );

/*****************************************************************************/
//     Description : to get the user data which would send from sms_content_ptr
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetUserData(
                       uint8                    sms_index,        //IN: the index of SMS (1 ~ MMISMS_SPLIT_MAX_NUM) 
                       MMISMS_CONTENT_T            *sms_content_ptr, //IN: the content of need send
                       MN_SMS_USER_DATA_HEAD_T  *tpdu_head_ptr,   //IN/OUT: the head of user data
                       BOOLEAN                  is_need_convert,
                       APP_SMS_USER_DATA_T      *user_data_ptr,    //OUT: the user data
                       MMISMS_SEND_T *send_info_ptr
                       );


/*****************************************************************************/
//     Description : to get the length according the alpha of message 
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetLenByAlpha(
                         MN_SMS_ALPHABET_TYPE_E    alpha,            //IN:
                         uint8                     *split_len_ptr,    //OUT: the len to split message
                         uint8                     *limit_len_ptr,    //OUT: the limit len
                         BOOLEAN                   is_head_has_port                        
                         );

/*****************************************************************************/
//     Description : MMISMS_SetSMSSaveInfo
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSMSSaveInfo(
                                                MN_DUAL_SYS_E dual_sys,
                                                MN_CALLED_NUMBER_T *dest_num_ptr
                         );

/*****************************************************************************/
//     Description : to send saved request to MN
//    Global resource dependence : none
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_UpdateStatusReportMsgReq(                                            //RETURN:
                                            MN_DUAL_SYS_E            dual_sys,
                                            MN_SMS_STATUS_E        status,                //IN:
                                            MN_SMS_STORAGE_E        storage,            //IN:
                                            MN_SMS_RECORD_ID_T        record_id            //IN:
                                            );

/*****************************************************************************/
//     Description : HandleSaveNextMessageAlertWin
//    Global resource dependence : 
//  Author:wancan.you
//    Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleSaveNextMessageAlertWin(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 );

/*****************************************************************************/
//     Description : to saved request to MN
//    Global resource dependence : none
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_MOVESaveSms(                                            //RETURN:
                                            MMISMS_ORDER_ID_T order_id
                                            );

/*****************************************************************************/
//     Description : set next long sms order
//    Global resource dependence : g_mmisms_global
 //  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetsmsOrder(
                                    MMISMS_ORDER_ID_T order_id
                                    );

/*****************************************************************************/
//     Description : get next long sms order
//    Global resource dependence : g_mmisms_global
 //  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetLongsmsOrder(void);

/*****************************************************************************/
//  Description : to saved request to MN
//  Global resource dependence : none
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_CopySaveSms(                                            //RETURN:
                                            MMISMS_ORDER_ID_T order_id
                                            );

/*****************************************************************************/
//  Description : to copy the next SMS 
//  Global resource dependence : g_mmisms_global
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_CopyNextMessage(
                                                uint8   total_num,        //IN
                                                uint8   seq_num            //IN: the seq number ( 1 ~ 8)
                                                );

/*****************************************************************************/
//  Description : to get the user data which would send from sms_content_ptr
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetReadUserData(
                                    uint8                    sms_index,        //IN: the index of SMS (1 ~ MMISMS_SPLIT_MAX_NUM) 
                                    MMISMS_CONTENT_T            *sms_content_ptr, //IN: the content of need send
                                    MN_SMS_USER_DATA_HEAD_T  *tpdu_head_ptr,   //IN/OUT: the head of user data 
                                    BOOLEAN                  is_need_convert,
                                    APP_SMS_USER_DATA_T      *user_data_ptr    //OUT: the user data
                                    );

/*****************************************************************************/
//  Description : SMS数量获取
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC uint16 MMISMS_GetSmsNumByStorage(MN_DUAL_SYS_E        dual_sys,      //IN
                                        MN_SMS_STORAGE_E    storage        //IN
                                        );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_SAVE_H_

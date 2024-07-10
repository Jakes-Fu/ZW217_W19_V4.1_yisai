/*************************************************************************
 ** File Name:      mmisms_save.h                                      *
 ** Author:         liming.deng                                          *
 ** Date:           2012/08/01                                           *
 ** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about saving sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2012/8/23      rong.gu      Create.                              *
*************************************************************************/
#ifndef _MMISMS_APPCONTROL_H_
#define _MMISMS_APPCONTROL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_default.h"
#include "guimenu.h"
#include "guilistbox.h"
#include "guitext.h"
#include "mmi_text.h"
#include "mmisms_order.h"
#include "mmisms_text.h"
#include "mmisms_id.h"
#include "mmisms_image.h"
#include "mmisms_menutable.h"
#include "mms_image.h"
#include "guiform.h"
#include "mmipub.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*****************************************************************************/
//  Description : for cr00116469 @tzw
//  Global resource dependence : 
//  Author: tzw
//  Note: 打开sms 主界面窗口
/*****************************************************************************/
PUBLIC void MMISMS_OpenMainMenuWindowExt(void);

/*****************************************************************************/
// Description : open sms finish read
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSmsFinishRead(MMISMS_ORDER_ID_T cur_order_id,MMISMS_BOX_TYPE_E box_type,MMI_CTRL_ID_T ctrl_id,uint32 *marked_sms_num);

/*****************************************************************************/
//     Description : mark
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void MMISMS_MarkItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,uint32* mark_num);

/*****************************************************************************/
//     Description : cancel mark
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void MMISMS_CancelMarkItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,uint32* mark_num);

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_MarkAllItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num,BOOLEAN is_push_mark);

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_CancelMarkAllItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num,BOOLEAN is_push_mark);

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_MarkSameNumItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num);

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_CancelMarkSameNumItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num);

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_MarkSimItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,MN_DUAL_SYS_E mark_sim_id,uint32* mark_num);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_MarkOpt(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num);
#endif



/*****************************************************************************/
//  Description :set sort menu current focus item
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetSortMenuFocusItem(MMI_CTRL_ID_T menu_ctrl_id,MMI_MENU_GROUP_ID_T group_id);

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_AnswerMMS(MMISMS_ORDER_ID_T cur_order_id,MMISMS_BOX_TYPE_E box_type,MMI_CTRL_ID_T cur_ctrl_id,uint32 *marked_sms_num);

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_AnswerSms(MMISMS_ORDER_ID_T cur_order_id,MMISMS_BOX_TYPE_E box_type,MMI_CTRL_ID_T cur_ctrl_id,uint32 *marked_sms_num);
 
/*****************************************************************************/
//     Description : to operate the delete
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OperateDelorDelAll(MMISMS_ORDER_ID_T cur_order_id,uint32 marked_sms_num,MMIPUB_HANDLE_FUNC handle_func);

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReplyCall(MMISMS_ORDER_ID_T cur_order_id,uint32 call_type);

#if 0
/*****************************************************************************/
//  Description : to operate the move
//  Global resource dependence : 
//  Author:feingming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OperateMove(MMISMS_ORDER_ID_T cur_order_id,uint32 marked_sms_num,MMIPUB_HANDLE_FUNC handle_func);
#endif

#if 0
/*****************************************************************************/
//  Description : to operate the move
//  Global resource dependence : 
//  Author:feingming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OperateCopy(MMISMS_ORDER_ID_T cur_order_id,uint32 marked_sms_num,MMIPUB_HANDLE_FUNC handle_func);
#endif

/*****************************************************************************/
//     Description : to get the origin number form order info
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetCurNumByOrder(                        //RETURN: the length of num_ptr
                                      MMISMS_ORDER_ID_T  cur_order_id,
                                      MN_DUAL_SYS_E     *dual_sys_ptr,
                                      uint8                *num_ptr,        //OUT:
                                      uint8                 max_num_len    //IN:                                          
                                      );
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_AddToPbNew(MMISMS_ORDER_ID_T order_id,uint8 *number_arr);

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT   
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_AddToPbExist(MMISMS_ORDER_ID_T order_id,uint8 *number_arr);
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add sender to blacklist
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_AddSenderToBlackList(MMIPB_BCD_NUMBER_T pb_bcd);
#endif
#if 0
#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : to copy all mared message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CopyAllMarkedMsg( 
                                    MMISMS_BOX_TYPE_E    box_type
                                    );
#endif                                

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : to move all mared message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_MoveAllMarkedMsg( 
                      MMISMS_BOX_TYPE_E    box_type
                      );
#endif
#endif
/*****************************************************************************/
//     Description : to open the msg box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenMsgBox( 
                       MMI_TEXT_ID_T    text_id,    //IN:
                       MMI_IMAGE_ID_T    image_id    //IN:
                       );

/*****************************************************************************/
//     Description : get delete all flag
//                  TRUE:        do delete all sms operation  
//                  FALSE:    without do delete all sms operation
//    Global resource dependence : 
//  Author:jian.ma
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDelAllFlag(void);

/*****************************************************************************/
//     Description : set delete all flag
//                  TRUE:        do delete all sms operation  
//                  FALSE:    without do delete all sms operation
//    Global resource dependence : 
//  Author:jian.ma
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetDelAllFlag(BOOLEAN is_del_all);


/*****************************************************************************/
//     Description : get move all flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move all SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetMoveAllFlag(void);

/*****************************************************************************/
//     Description : set move all flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move all SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMoveAllFlag(BOOLEAN is_move_all);

/*****************************************************************************/
//     Description : set move single flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move single SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMoveSingleFlag(BOOLEAN is_move_single);

/*****************************************************************************/
//     Description : get move single flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move single SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetMoveSingleFlag(void);


/*****************************************************************************/
//     Description : Get the focus child win id.
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMISMS_GetCurActiveListCtrlId(void);

/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_HandleSendOperation(
                               MN_DUAL_SYS_E    dual_sys,
                               MMI_WIN_ID_T    win_id,    //IN:
                               BOOLEAN        is_re_send 
                               );


/*****************************************************************************/
//  Description :sms edit text property
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetEditTextProperty(MMI_CTRL_ID_T			ctrl_id);

#ifdef MMI_SMS_CHAT_SUPPORT

/*****************************************************************************/
//  Description : read new sms
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ReadChatMsg(MMISMS_ORDER_ID_T cur_order_id);
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E  MultiSendSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

#endif

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description : set win font size
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMISMS_GetWinFont(void);

/*****************************************************************************/
//  Description : reset win with different font size
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ResetWinFont(MMISMS_BOX_TYPE_E box_type);
#endif

/*****************************************************************************/
//     Description : check me have enough space for save sms
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MEHaveEnoughSpace(void);

/*****************************************************************************/
//     Description : get the sms is in the senfail box
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MsgIsInSendFailBox(void);

/*****************************************************************************/
//     Description : get the sms is in the draft box
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MsgIsInDraftBox(void);

/*****************************************************************************/
// 	Description : to show the mem full prompt
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowMemFullPrompt(
                                            MN_DUAL_SYS_E dual_sys,
                                            MN_SMS_MEM_FULL_STATUS_E mem_status);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : get phone number from calllog
//    Global resource dependence : 
//  Author: jixin.xu
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetNumberFromCalllog(
                                   MMI_MESSAGE_ID_E    msg_id    //IN:
                                   );
#endif

/*****************************************************************************/
//     Description : to open write SMS window    
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenWriteWin(void);

/*****************************************************************************/
//  Description : 打开sms 隐私保护密码输入成功后的写界面窗口
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenWriteWindowExt(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : 打开sms 隐私保护密码输入成功后的写界面窗口
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSendGroupWinExt(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//     Description : save the message to the draft box
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SaveAsDraft(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :Get is save draft
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsSaveDraft( void);

/*****************************************************************************/
//  Description :Set is save draft
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetIsSaveDraft( BOOLEAN is_save );

#ifdef MMI_SMS_MINI_SUPPORT
/*****************************************************************************/
//  Description :Load msg string from stored buffer
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_LoadStoreddMsgBuffer(MMI_STRING_T *sms_string);

/*****************************************************************************/
//  Description :save msg string to stored buffer
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SaveStoredMsgdBuffer(MMI_STRING_T sms_string);
#endif
/*****************************************************************************/
//  Description : to handle curr sms busy
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsSmsBusy(MMISMS_ORDER_ID_T cur_order_id);

/*****************************************************************************/
//  Description : to resend sms
//  Global resource dependence :
//  Author:louis.wei
//  Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_ResendSMSbyOrderID(MMISMS_ORDER_ID_T order_id,MN_DUAL_SYS_E dual_sys);

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//  Description : to enter the window for edit SMS
//  Global resource dependence :
//  Author:louis.wei
//  Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_BtResendSMSbyOrderID(MMISMS_ORDER_ID_T order_id,MN_DUAL_SYS_E dual_sys);
#endif
/*****************************************************************************/
//     Description : Get the focus child win id.
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMISMS_GetSmsFocusWinId(MMISMS_BOX_TYPE_E box_type);

/*****************************************************************************/
//  Description :根据setting设置的显示格式,以及cur_order_id获取时间
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetDateTimeStringByOrder(MMISMS_ORDER_ID_T cur_order_id,  //in
                                 BOOLEAN is_disp_date,//in
                                 BOOLEAN is_disp_year,
                                 BOOLEAN is_disp_time,//in
                                 wchar *date_time_ptr  //out
                                );

/*****************************************************************************/
//  Description :根据setting设置的显示格式,以及相对秒数获取时间
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetDateTimeStringBySecond(uint32 second,  //in
                                 BOOLEAN is_disp_date,//in
                                 BOOLEAN is_disp_year,                                 
                                 BOOLEAN is_disp_time,//in
                                 wchar *date_time_ptr  //out
                                );

/*****************************************************************************/
//  Description :根据setting设置的显示格式,以及MN_SMS_TIME_STAMP_T获取时间
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetDateTimeStringByStamp(MMI_TM_T *sc_time_ptr,  //in
                                 BOOLEAN is_disp_date,//in
                                 BOOLEAN is_disp_year, 
                                 BOOLEAN is_disp_time,//in
                                 wchar *date_time_ptr  //out
                                );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_APPCONTROL_H_

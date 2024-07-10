/****************************************************************************
** File Name:      mmisms_securitywin.c                                    *
** Author:         liming deng                                             *
** Date:           1/4/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2012        minghu.mao       Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#include "mmisms_securitywin.h"
#include "mmisms_app.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_menutable.h"
#include "mmisms_order.h"
#include "mmisms_commonui.h"
#include "mmicc_export.h"
#include "guictrl_api.h"
#include "mmisms_appcontrol.h"
#include "mmimms_export.h"
#include "mmiidle_statusbar.h"
#include "mmialarm_export.h"
#include "mmi_nv.h"

#ifdef MMI_OTA_SUPPORT
#include "mmiota_id.h"
#endif
#ifdef MMS_SUPPORT
#include "mmimms_text.h"
#endif
#include "mmisms_send.h"
#include "mmiudisk_export.h"

#ifdef MMI_OTA_SUPPORT
#include "Mmiota_export.h"
#endif
#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
#include "Mmibrowser_export.h"
#endif
#endif			
#ifdef MET_MEX_SUPPORT
#include "Met_mex_export.h"
#endif
#include "mmismsapp_interface.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//END

#define MMISMS_SECURITY_LEN_IS_IN_RANGE(_LEN)            \
    ((MMISMS_SECURITY_PWD_MIN_LEN <= (_LEN))    \
    && (MMISMS_SECURITY_PWD_MAX_LEN >= (_LEN))) \
    
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    MMI_WIN_ID_T            win_handle;
    uint32                  menu_id;
    uint32                  group_id;
    uint32            marked_sms_num;
    MMI_CTRL_ID_T     cur_list_ctrl_id;
    MMISMS_ORDER_ID_T cur_focus_order_id;
    MMISMS_BOX_TYPE_E  security_box_type;
    MMISMS_VAL_SER_DATA_T s_sms_validate_secdata;
#ifdef MMI_PDA_SUPPORT
    BOOLEAN is_mark_all;
#endif
}MMISMS_SECURITYBOX_WINDATA_T;
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//MMISMS_DELMSG_GLOBLE_T g_sms_delmsg_all_info = {0};


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUCTION DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 关闭安全信箱验证窗口
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_CloseValidateSecruityBox(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : to handle security setting input old power on password
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInputPwdWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenBoxMenuOpt(MMIPUB_MENU_NOTIFY_T *param );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenMenuKeyInit(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenListMarkInit(MMIPUB_MENU_NOTIFY_T *param);
#endif

/*****************************************************************************/
// 	Description : 安全信箱密码是否正确
//	Global resource dependence : 
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCorrectSecurityboxPwd(
                                          const wchar *pwd_ptr,
                                          uint16     pwd_len
                                          );

/*****************************************************************************/
//     Description : to Enter the list of box
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void MMISMS_EnterSecurityBox(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSecurityBoxChildWinMsg(
                                            MMI_WIN_ID_T        win_id,        //IN:
                                            MMI_MESSAGE_ID_E msg_id,        //IN:
                                            DPARAM            param        //IN:
                                            );

/*****************************************************************************/
//  Description : 处理输入新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPwdWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           );
/*****************************************************************************/
//  Description : 处理确认新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPwdCnfWindow(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM                param
                                              );

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenOpt(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenInit(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecurityWinDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinDelAll(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box lock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinLockItem(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinUnLockItem(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

#ifdef MMI_SMS_COPY_SUPPORT 
/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinCopyToMe(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinCopyToSIM(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecurityWinCopyQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  SecuritWinCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecurityCopyCallBack(void *user_date,MMISMS_COPY_RESULT_INFO *data_info_ptr);
#endif


#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  SecuritWinMoveSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinMoveToSIM(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinMoveToMe(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecurityMoveCallBack(void *user_date,MMISMS_MOVE_RESULT_INFO *data_info_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecuritWinMoveQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );


#endif

/*****************************************************************************/
//  Description :安全信箱验证, 调用回调函数
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ValidateSecruityBoxCallBack(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr,uint32 val_result);

/*****************************************************************************/
//  Description :安全信箱验证是否需要调用回调函数
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsValidateSecruityBoxCallBack(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL BOOLEAN SecurityWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data);

/*****************************************************************************/
// Description : open sms finish read
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void HandleSecurityBoxFinishReadSMS(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

WINDOW_TABLE( MMISMS_SECURITYBOX_CHILD_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSecurityBoxChildWinMsg),    
    WIN_ID( MMISMS_SECURITYBOX_CHILD_WIN_ID ),
    WIN_TITLE(TXT_SMS_SECURITY),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_SECURITYBOX_FORM_CTRL_ID),    
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL,TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_SECURITYBOX_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISMS_SECURITYBOX_LISTBOX_CTRL_ID, MMISMS_SECURITYBOX_FORM_CTRL_ID),
    #else
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SECURITYBOX_LISTBOX_CTRL_ID),
    #endif
    END_WIN
};

WINDOW_TABLE( MMISMS_SET_SECURITY_NEW_PWD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInputNewPwdWindow ),    
    WIN_ID( MMISMS_SET_SECURITY_NEW_PWD_WIN_ID ),
    WIN_TITLE( TXT_SMS_INPUT_NEW_SECURITY_PWD ),
    CREATE_EDIT_PASSWORD_CTRL(MMISMS_SECURITY_PWD_MAX_LEN,MMISMS_SET_SECURITY_NEW_PWD_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMISMS_SET_SECURITY_NEW_PWD_CNF_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInputNewPwdCnfWindow ),    
    WIN_ID( MMISMS_SET_SECURITY_NEW_PWD_CNF_WIN_ID ),
    WIN_TITLE( TXT_SMS_INPUT_NEW_PWD_CNF ),
    CREATE_EDIT_PASSWORD_CTRL(MMISMS_SECURITY_PWD_MAX_LEN,MMISMS_SET_SECURITY_NEW_PWD_CNF_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMISMS_SECURITYBOX_PWD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInputPwdWindow ),    
    WIN_ID( MMISMS_SECURITYBOX_PWD_WIN_ID ),
    WIN_TITLE( MMISMS_INPUT_SECURITY_PASSWORD ),
    CREATE_EDIT_PASSWORD_CTRL(MMISMS_SECURITY_PWD_MAX_LEN,MMISMS_SET_SECURITY_PWD_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
 
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL BOOLEAN SecurityWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    if (callback_data == PNULL)
    {
        return TRUE;
    }
    
    MMK_SendMsg(
        (MMI_HANDLE_T)callback_data->user_data,
        MSG_SMS_FINISH_READ,
        PNULL);
    
    return TRUE;
}

/*****************************************************************************/
// Description : open sms finish read
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void HandleSecurityBoxFinishReadSMS(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint8    num_len = 0;     
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};  
    MMISMS_BOX_TYPE_E state_box_type = MMISMS_BOX_NONE;
    
    if(PNULL == win_data_ptr || PNULL == win_data_ptr->cur_focus_order_id)
    {
        MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
        SCI_TRACE_LOW("mmisms cur_order_id pnull");    
        return;
    }
    state_box_type = MMISMS_GetSmsBoxtype(win_data_ptr->cur_focus_order_id);            
    if( MMISMS_BOX_MT == state_box_type || MMISMS_BOX_SENDSUCC == state_box_type)
    {
        MMISMS_READ_MSG_T read_msg = {0};
        MMISMS_ClearOperInfo();
        //MMISMS_SetReadedSms2Window(state_box_type);        
        read_msg = MMISMS_GetReadResult();
        MMISMS_EnterShowSmsWin(read_msg);
    }
    else if( MMISMS_BOX_SENDFAIL == state_box_type || MMISMS_BOX_NOSEND == state_box_type)
    {
        MMI_STRING_T msg_content = {0};
        MMISMS_ORDER_ID_T order_id = PNULL;
        // set the init SMS content
        MMISMS_GetSMSContent(&msg_content);
        
        num_len = MMISMS_GetCurNumByOrder(win_data_ptr->cur_focus_order_id,&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
        
        // open edit window
        MMISMS_WriteNewMessage(
            MN_DUAL_SYS_MAX,
            &msg_content,
            FALSE,
            number,
            num_len
            );
        
        #ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_BOX_SENDFAIL == state_box_type)
        {
            MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;
        
            MMISMS_SetCurOrderArrIndex(0);
            order_id = MMISMS_GetCurOperationOrderId();
        
            event_fast_info_ptr = MMISMS_TIMERMSG_GetEventByOrder(order_id);
        
            if (PNULL != event_fast_info_ptr)
            {
                MMISMS_SetIsTimerMode(TRUE);
        
                MMISMS_TIMERMSG_SetCurEvent(event_fast_info_ptr);
            }
        }
        #endif
        
        if (PNULL != msg_content.wstr_ptr)
        {
            SCI_FREE(msg_content.wstr_ptr);
            msg_content.wstr_ptr = PNULL;
        }

    }
    MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);   
}


/*****************************************************************************/
//     Description : open read message 
//    Global resource dependence :
//  Author:rong.gu
//    Note:读取内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenReadMessage(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_BOX_TYPE_E state_box_type = MMISMS_BOX_NONE;
    MMI_STRING_T   prompt_str = {0};
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif
    cur_order_id = win_data_ptr->cur_focus_order_id;
    if(PNULL == cur_order_id)
    {
        return;
    }

    state_box_type = MMISMS_GetSmsBoxtype(cur_order_id); 
    MMISMS_SetSMSUpdateInfo(cur_order_id);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);    
    switch(cur_order_id->flag.msg_type)
    {
        case MMISMS_TYPE_SMS:
#ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_GetIsTimerMsg())
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
                break;
            }
#endif
            if (MMISMS_IsSmsBusy(cur_order_id))
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
                break;
            }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            if(MMISMS_BOX_NOSEND == state_box_type)
            {
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS);
            }
#endif
            MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
            MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);
            
            MMISMSAPP_ReadOneSms(
                cur_order_id,
                SecurityWinReadSmsCallback,
                PNULL,
                (void*)win_data_ptr->win_handle);
    
            break;
            
        case MMISMS_TYPE_MMS:
			//解决cr MS00150274,U盘使用过程中不能看MMS
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                break;
            }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
            if (MMIAPIBROWSER_IsRunning())
            {
                MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                break;
            }
#endif
#endif			
#ifdef MET_MEX_SUPPORT
            if (MMIMEX_ConflictPromt())
            {
                // mex kernel is running and may result in mem conflict
                break;
            }     
#endif			
            MMISMS_ClearOperInfo();                     
            switch(state_box_type) 
            {                
            case MMISMS_BOX_MT:
            case MMISMS_BOX_SENDSUCC:
            case MMISMS_BOX_SENDFAIL:
                MMIAPIMMS_ReadMMS((uint32)cur_order_id->record_id);
                break;
            case MMISMS_BOX_NOSEND:
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT);
                MMIAPIMMS_DraftEditMMS((uint32)cur_order_id->record_id);                   
                break;
            default:
                break;
            }

            //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
            }
#ifdef MMI_OTA_SUPPORT
            if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
            {
                MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
            }

            if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
            }
#endif
            break;
            
        case MMISMS_TYPE_WAP_PUSH:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {

                MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT                        
                MMIAPIOTA_WapPushEnter((uint16)cur_order_id->record_id);
#endif
                //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                }
#ifdef MMI_OTA_SUPPORT
                if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                {
                    MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                }
#endif
            }
            break;
            
        case MMISMS_TYPE_WAP_OTA:
        case MMISMS_TYPE_MMS_OTA:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {               
                MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT
                MMIAPIOTA_OTAEnter((uint16)cur_order_id->record_id);
#endif
                //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                }
#ifdef MMI_OTA_SUPPORT				
                if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                {
                    MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                }
#endif
            }
            break;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.19
        case MMISMS_TYPE_MAIL:
            //读取邮件
            mail_ReadMailByRecordId((uint16)cur_order_id->record_id);
            break;
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
        default: 
            break;
    }         
}


/*****************************************************************************/
//  Description :安全信箱验证, 调用回调函数
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ValidateSecruityBoxCallBack(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr,uint32 val_result)
{
    //Call back with result
    if (PNULL != win_data_ptr->s_sms_validate_secdata.user_callback)
    {
        win_data_ptr->s_sms_validate_secdata.user_callback(val_result);
        //SCI_TRACE_LOW:"MMIAPISMS_OpenValidateSecruityBox. user_callback = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_31958_112_2_18_2_55_26_880,(uint8*)"");
    }
    //Set flag FALSE
    return;
}

/*****************************************************************************/
//  Description :安全信箱验证是否需要调用回调函数
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsValidateSecruityBoxCallBack(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    return win_data_ptr->s_sms_validate_secdata.isValidate;
}

/*****************************************************************************/
//  Description : 打开安全信箱验证窗口
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenValidateSecruityBox(SMSAPI_VALIDATESECURITY_CALLBACK user_callback)
{

    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;    
    
    if (MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))   //不可重入
    {
        MMK_CloseWin(MMISMS_SECURITYBOX_CHILD_WIN_ID);
        return;
    }
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_SECURITYBOX_WINDATA_T));
    
    if (user_callback == PNULL)
    {
        win_data_ptr->s_sms_validate_secdata.isValidate = FALSE;
        win_data_ptr->s_sms_validate_secdata.user_callback = PNULL;
        //SCI_TRACE_LOW:"MMIAPISMS_OpenValidateSecruityBox. user_callback = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_31983_112_2_18_2_55_26_881,(uint8*)"");
        MMK_CreatePubEditWin((uint32*)MMISMS_SECURITYBOX_PWD_WIN_TAB,(ADD_DATA)win_data_ptr);  
        return;
    }
     
    win_data_ptr->s_sms_validate_secdata.isValidate = TRUE;
    win_data_ptr->s_sms_validate_secdata.user_callback = user_callback;
    MMK_CreatePubEditWin((uint32*)MMISMS_SECURITYBOX_PWD_WIN_TAB,(ADD_DATA)win_data_ptr);  
}

/*****************************************************************************/
//  Description : 关闭安全信箱验证窗口
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_CloseValidateSecruityBox(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    win_data_ptr->s_sms_validate_secdata.isValidate = FALSE;
    win_data_ptr->s_sms_validate_secdata.user_callback = PNULL;
    MMK_CloseWin(MMISMS_SECURITYBOX_PWD_WIN_ID);  
}

/*****************************************************************************/
//     Description : to Enter the list of box
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void MMISMS_EnterSecurityBox(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    BOOLEAN is_not_inited = FALSE;
    MMI_HANDLE_T win_handle = 0;
    is_not_inited = MMISMS_IsSimNotInited();
    MMISMS_SetCurType(MMISMS_BOX_SECURITY);

    if(MMISMS_IsOrderOk() && MMISMS_IsMMSOrderReady()) // 增加判断mms是否排序完毕
    {
        win_handle = MMK_CreateWin((uint32 *)MMISMS_SECURITYBOX_CHILD_WIN_TAB,(ADD_DATA)win_data_ptr);     
        win_data_ptr->win_handle = win_handle;
        if(win_handle != PNULL)
        {
            MMK_SetWinUserData(win_handle, win_data_ptr);
        }
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        // 需要开启等待窗口
        MMISMS_OpenInitWaitingWindow(MMISMS_OPEN_BOX);
    }    
}

/*****************************************************************************/
//  Description : to handle security setting input old power on password
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInputPwdWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_STRING_T       old_pwd_value    =   {0};  
    MMI_RESULT_E        result          =   MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T*)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr PNULL");
        return result;        
    }
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
		 MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(MMISMS_SET_SECURITY_PWD_CTRL_ID, &edit_both_rect);
        //set left softkey
        GUIEDIT_SetSoftkey(MMISMS_SET_SECURITY_PWD_CTRL_ID,0,MMISMS_SECURITY_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);

        MMK_SetAtvCtrl(win_id, MMISMS_SET_SECURITY_PWD_CTRL_ID);

        break;
        
    case MSG_FULL_PAINT:
        #ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id,IMAGE_NUMBERINPUT_PIN);
        #endif
        break;

    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about PIN
        GUIEDIT_GetString(MMISMS_SET_SECURITY_PWD_CTRL_ID,&old_pwd_value);
        
        if (MMISMS_SECURITY_LEN_IS_IN_RANGE(old_pwd_value.wstr_len))
        {
            //密码是否正确
            if (IsCorrectSecurityboxPwd( old_pwd_value.wstr_ptr, old_pwd_value.wstr_len))
            {
                if (MMISMS_IsValidateSecruityBoxCallBack(win_data_ptr))		//need callback and close current Win
                {
                    MMISMS_ValidateSecruityBoxCallBack(win_data_ptr,TRUE);
                    MMIAPISMS_CloseValidateSecruityBox(win_data_ptr);
                    break;
                }
                if(MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
                {
                    MMK_CreatePubEditWin((uint32*)MMISMS_SET_SECURITY_NEW_PWD_WIN_TAB,PNULL);  
                }
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                else
                {
                    MMISMS_EnterSecurityBox(win_data_ptr);
                }
#endif
                MMK_CloseWin(win_id);
            }
            else
            {
                //提示密码错误!
                MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                
                //清空password
                GUIEDIT_ClearAllStr(MMISMS_SET_SECURITY_PWD_CTRL_ID);
            }
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        if(!MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
        {
            SCI_FREE(win_data_ptr);
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : handle menu key opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityBoxOpenMenuKeyOpt(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr,BOOLEAN is_enable_mark)
{
     MMIPUB_MENU_PARAM_T menuparam = {0};
     if(is_enable_mark)
     {
         if(0 == win_data_ptr->marked_sms_num)
         {
             if(win_data_ptr->cur_focus_order_id != PNULL && win_data_ptr->cur_focus_order_id->flag.is_locked)
             {
                 menuparam.group_id = MENU_SMS_MARK_UNLOCK_MENU_OPT;
             }
             else
             {
                 menuparam.group_id = MENU_SMS_MARK_MENU_OPT;
             }
         }
         else
         {
             MMISMS_ClearOperInfo();
             if(PNULL == MMISMS_FindValidSecuritySMSForDelAll())
             {
                 menuparam.group_id = MENU_SMS_MARK_UNLOCK_MENU_OPT;
             }
             else
             {
                 menuparam.group_id = MENU_SMS_MARK_MENU_OPT;
             }
         }
         win_data_ptr->group_id = menuparam.group_id; 
         menuparam.win_id =   MMISMS_MARK_OPT_WIN_ID;
         menuparam.ctrl_id =  MMISMS_MARK_MENU_OPT_MENU_CTRL_ID;    
         menuparam.dst_handle = win_data_ptr->win_handle;
         menuparam.func.OnMenuOk = HandleSecurityWinOpenBoxMenuOpt;  
         menuparam.func.OnMenuInit = HandleSecurityWinOpenListMarkInit;       
         MMIPUB_OpenPubMenuWin( &menuparam );
     }
     else
     {          
         menuparam.group_id = MENU_SMS_SECURITYLIST_MENUKEY_OPT;             
         menuparam.win_id =   MMISMS_SECURITYLIST_MENUKEY_WIN_ID;
         menuparam.ctrl_id =  MMISMS_SECURITYLIST_MENUKEY_OPT_MENU_CTRL_ID;    
         menuparam.dst_handle = win_data_ptr->win_handle;
         menuparam.func.OnMenuOk = HandleSecurityWinOpenBoxMenuOpt;  
         menuparam.func.OnMenuInit = HandleSecurityWinOpenMenuKeyInit;       
         MMIPUB_OpenPubMenuWin( &menuparam );         
     }
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenMenuKeyInit(MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;    
    MMI_STRING_T box_str = {0};
    uint16 count = 0;  
    
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if(PNULL == win_data_ptr)
    {
        return;
    }
    MMISMS_SetSortMenuFocusItem(MMISMS_SECURITYLIST_MENUKEY_OPT_MENU_CTRL_ID,MENU_SMS_MTLIST_SORT_OPT);

    count = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_SECURITY);
    if(0 == count)
    {
        MMIAPICOM_EnableGrayed(MMISMS_SECURITYLIST_MENUKEY_WIN_ID, MENU_SMS_SECURITYLIST_MENUKEY_OPT,MMISMS_MENU_SORT_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(MMISMS_SECURITYLIST_MENUKEY_WIN_ID, MENU_SMS_SECURITYLIST_MENUKEY_OPT,MMISMS_MENU_MARK_ITEM_ID,TRUE);
    }
            
    MMI_GetLabelTextByLang(TXT_COMMON_SORT , &box_str);
    GUIMENU_SetMenuTitle(&box_str, MMISMS_SECURITYLIST_MENUKEY_OPT_MENU_CTRL_ID);    
    menu_enable_t.group_id = MENU_SMS_SECURITYLIST_MENUKEY_OPT;
    menu_enable_t.win_id = MMISMS_SECURITYLIST_MENUKEY_WIN_ID;
    MMISMS_HandleSortMenuEnableGray(&menu_enable_t);    
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenListMarkInit(MMIPUB_MENU_NOTIFY_T *param)
{       
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;    
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }  
    menu_enable_t.cur_order_id = win_data_ptr->cur_focus_order_id;
    menu_enable_t.box_type = MMISMS_BOX_SECURITY;
    menu_enable_t.marked_num = win_data_ptr->marked_sms_num;    
    menu_enable_t.win_id = MMISMS_MARK_OPT_WIN_ID;
    menu_enable_t.group_id = win_data_ptr->group_id;
    
    MMISMS_HandleMarkMenuEnableGray(&menu_enable_t);

#ifdef MMI_SMS_MOVE_SUPPORT     
    menu_enable_t.group_id = MENU_SMS_SECURITY_BOX_OPT;
    MMISMS_HandleMoveMenuEnableGray(&menu_enable_t);
#endif

#ifdef MMI_SMS_COPY_SUPPORT 
    menu_enable_t.group_id = MENU_SMS_COPY_OPT;
    MMISMS_HandleCopyMenuEnableGray(&menu_enable_t);
#endif
    
    if(0 == win_data_ptr->marked_sms_num)
    {
    #ifdef MMI_SMS_MOVE_SUPPORT         
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, menu_enable_t.group_id,MMISMS_MENU_MOVE_ITEM_ID,TRUE);
    #endif
    #ifdef MMI_SMS_COPY_SUPPORT
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, menu_enable_t.group_id,MMISMS_MENU_COPY_ITEM_ID,TRUE);
    #endif
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, menu_enable_t.group_id,MMISMS_MENU_LOCK_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, menu_enable_t.group_id,MMISMS_MENU_UNLOCK_ITEM_ID,TRUE);
    }              
#ifdef MMI_SMS_COPY_SUPPORT
    MMISMS_SetCopyMenuTitle();
#endif
    
}


#endif

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityBoxDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T        win_id = MMISMS_SECURITYBOX_CHILD_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms chatbox event ptr PNULL");
       return;
    }
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {  
        MMK_SetWinDirty(win_id, TRUE);
        if(MMK_IsFocusWin(win_id))
        {
           MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }    
}

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSecurityBoxChildWinMsg(
                                            MMI_WIN_ID_T        win_id,        //IN:
                                            MMI_MESSAGE_ID_E msg_id,        //IN:
                                            DPARAM            param        //IN:
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SECURITYBOX_LISTBOX_CTRL_ID;
    uint16 selected_index = 0;
    LOCAL BOOLEAN s_have_items = FALSE;
    LOCAL BOOLEAN s_need_update = FALSE;
    GUILIST_MARK_ITEM_T *list_param_ptr = NULL;
    uint16 i = 0;
    uint16 index = 0;
    #ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    #endif

    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(win_id);
    
    //SCI_TRACE_LOW:"MMISMS: HandleSecurityBoxChildWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_23687_112_2_18_2_55_9_831,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);

        // load items to list box 
        MMISMS_SetFoldType(MMISMS_FOLDER_SECURITY);
        //win_data_ptr->win_id = win_id;
        win_data_ptr->cur_list_ctrl_id = MMISMS_SECURITYBOX_LISTBOX_CTRL_ID;
        
        s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_SECURITY, ctrl_id );
        if (s_have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }
#ifdef MMI_PDA_SUPPORT                  
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(MMISMS_SECURITYBOX_FORM_CTRL_ID,ctrl_id, &child_height);
        GUIFORM_IsSlide(MMISMS_SECURITYBOX_FORM_CTRL_ID, FALSE);
        GUIFORM_PermitChildBg(MMISMS_SECURITYBOX_FORM_CTRL_ID,FALSE);
        GUILIST_PermitBorder(ctrl_id,FALSE);
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        GUIFORM_SetChildDisplay(MMISMS_SECURITYBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUILIST_SetDefaultIcon(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);
        win_data_ptr->is_mark_all = FALSE;
#endif
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleSecurityBoxDateChangeCallBack, TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#ifndef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif
        s_need_update = FALSE;
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:    
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:HandleSecurityBoxChildWinMsg need_item_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_23731_112_2_18_2_55_9_832,(uint8*)"");
                break;
            }
            index = need_item_data_ptr->item_index;
            MMISMS_SetListItemData(MMISMS_BOX_SECURITY, ctrl_id, index);
        }
        break;

#ifndef MMI_GUI_STYLE_TYPICAL        
   case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                MMISMS_DrawListBoxItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id,MMISMS_BOX_SECURITY);
            }
            break;
        }          
#endif

    case MSG_CTL_LIST_MARK_ITEM:   
        //to get the info of current selected item
        list_param_ptr = (GUILIST_MARK_ITEM_T*)param;

        if(PNULL == list_param_ptr)
        {
            recode = MMI_RESULT_FALSE;
            break;
        }

        MMISMS_SetCurOperationOrderId(MMISMS_BOX_SECURITY, list_param_ptr->item_idx);

        #ifdef MMI_PDA_SUPPORT
        MMISMS_SetSoftkeyAfterMarkOpera(win_id,ctrl_id,&win_data_ptr->is_mark_all);            
        #endif
        
        if (ctrl_id == list_param_ptr->ctrl_id)
        {
            if (list_param_ptr->had_maked)
            {
                MMISMS_SetCurMsgMarked(TRUE);
                win_data_ptr->marked_sms_num++;
            }
            else
            {
                MMISMS_SetCurMsgMarked(FALSE);
                if(win_data_ptr->marked_sms_num != 0)
                {
                    win_data_ptr->marked_sms_num --;
                }
                // 如果标记数为零，关闭标记状态
                #ifndef MMI_PDA_SUPPORT
                if (0 == win_data_ptr->marked_sms_num)
                {
                    GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                }
                #endif
            }
        }
        break;
        
    case MMISMS_MSG_NEEDUPDATELIST:
      //  MMISMS_SetCurType(MMISMS_BOX_SENDSUCC);
        s_need_update = TRUE;
        break;

    case MSG_GET_FOCUS:            
        if (MMISMS_GetCurBoxType() != MMISMS_BOX_SECURITY) //从其他信箱切换回来
        {
            //清除掉原来的标记
            win_data_ptr->marked_sms_num = 0;
            for (i = 0; i< GUILIST_GetTotalItemNum(ctrl_id); i++)
            {
                GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
            }
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            MMISMS_SetAllMsgMarked(MMISMS_BOX_SECURITY, FALSE);
#ifdef MMI_PDA_SUPPORT
            MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_SECURITY, GUIFORM_CHILD_DISP_HIDE);
#endif

            //重新load信息list
            MMISMS_SetCurType(MMISMS_BOX_SECURITY);
            if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
            {
                GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, (uint16)MMISMS_MAIN_MENU_NODE_SENTBOX - MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG);
                // GUIMENU_SetFirstAndSelectedItem(0, (uint16)MMISMS_MAIN_MENU_NODE_SENTBOX - MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG,MMISMS_MENU_CTRL_ID);
            }
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_SECURITY, ctrl_id );
            if (s_have_items)
            {            
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }
            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
            s_need_update = FALSE;
#ifdef MMI_PDA_SUPPORT
            win_data_ptr->is_mark_all = FALSE;
#endif
        }
        else
        {
            if (s_need_update|| MMISMS_IsNeedSort())
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_SECURITY, ctrl_id );
                if (s_have_items)
                {            
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                }
                else
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                s_need_update = FALSE;
            }
        }
        GUILIST_ValidCurItemPos(ctrl_id);
        break;

        case MSG_FULL_PAINT:
        if(MMK_GetWinDirty(win_id))
        {
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            s_have_items = MMISMS_LoadSmsListFromOrder(MMISMS_BOX_SECURITY, ctrl_id);
            if (s_have_items)
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }
            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
            }
            MMK_SetWinDirty(win_id,FALSE);
        }
        break;


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT        
                if(GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
                {            
                    uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;               
                    switch(src_id)
                    {
                      case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                         {  
                             win_data_ptr->is_mark_all = MMISMS_HandleMarkAll(win_data_ptr->is_mark_all, win_id, ctrl_id, MMISMS_BOX_SECURITY,&win_data_ptr->marked_sms_num);
                         }
                         break;
                   
                      case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                          if(win_data_ptr->marked_sms_num > 0 )
                          {
                              if (MMISMS_IsUpdateOk())
                              {                                                                  
                                  //OpenDelteQueryWin(src_id);     
                                  MMISMS_OpenDelteQueryWin(MMISMS_DELETE_PART,FALSE,win_data_ptr->cur_focus_order_id,MMISMS_BOX_SECURITY,HandleSecurityWinDelQueryWinMsg);
                              }
                          }
                          else
                          {
                               MMISMS_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);    
                          }
                          break;

                      case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                          {
                              win_data_ptr->marked_sms_num = 0;            
                              win_data_ptr->is_mark_all = FALSE;
                              GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                              MMISMS_SetAllMsgMarked(MMISMS_BOX_SECURITY, FALSE);
                              
                              GUIFORM_SetChildDisplay(MMISMS_SECURITYBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);    
                              if (MMK_IsFocusWin(win_id))
                              {
                                  MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
                              }
                          }
                          break;

                     default:
                        break;
                    }
                    break;
                }      
#endif                  
                if (s_have_items && (win_data_ptr->marked_sms_num == 0))
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SECURITY, selected_index);
					HandleSecurityWinOpenReadMessage(win_data_ptr);
                    //MMISMS_OpenMsgInSmsList(win_data_ptr->cur_focus_order_id,MMISMS_BOX_SECURITY);
                }	 
                break;    
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef MMI_PDA_SUPPORT
        case MSG_APP_MENU:
            {
                 BOOLEAN  is_enable_mark = FALSE;
                 is_enable_mark = GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK);
                 if(!is_enable_mark)
                 {
                    win_data_ptr->is_mark_all = FALSE; 
                    GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE,STXT_CANCEL,TRUE);  
                 }
                 selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                 win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SECURITY, selected_index);
                 HandleSecurityBoxOpenMenuKeyOpt(win_data_ptr,is_enable_mark); 
            }
                 break;
        case MSG_CTL_LIST_LONGOK:
#endif            
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                BOOLEAN  is_enable_mark = FALSE;
                is_enable_mark = GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK);                
                if(is_enable_mark && msg_id == MSG_CTL_LIST_LONGOK)  //mark 情况下不处理longok
                {                    
                    break;
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SECURITY, selected_index);
                HandleSecurityWinOpenOpt(win_data_ptr);  
            }
            break;

    case MSG_SMS_FINISH_READ:
        {   
            HandleSecurityBoxFinishReadSMS(win_data_ptr);
        }
        break;
    
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MMISMS_MSG_READ_MSG:
        if (s_have_items && (win_data_ptr->marked_sms_num == 0) )
        {  
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SECURITY, selected_index);
            HandleSecurityWinOpenReadMessage(win_data_ptr);
            //MMISMS_OpenMsgInSmsList(win_data_ptr->cur_focus_order_id,MMISMS_BOX_SECURITY);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:

        MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);        
        MMK_CloseWin(win_id);  
        break;

    case MSG_CLOSE_WINDOW:
        MMISMS_SetFoldType(MMISMS_FOLDER_NORMAL);
        win_data_ptr->marked_sms_num =0;
        MMISMS_SetAllMsgMarked(MMISMS_BOX_SECURITY, FALSE);
        if(win_data_ptr)
        {
            SCI_FREE(win_data_ptr);
        }
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleSecurityBoxDateChangeCallBack, FALSE);
        break;
        
  default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


/*****************************************************************************/
//  Description : 处理输入新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPwdWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           )
{   
    MMI_STRING_T        pwd_value       =   {0};  
    MMI_STRING_T        pwd_value_cnf   =   {0};  
    MMI_RESULT_E        result          =   MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T edit_both_rect = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
		 MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(MMISMS_SET_SECURITY_NEW_PWD_CTRL_ID, &edit_both_rect);
        //set left softkey
        GUIEDIT_SetSoftkey(MMISMS_SET_SECURITY_NEW_PWD_CTRL_ID,0,MMISMS_SECURITY_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,MMISMS_SET_SECURITY_NEW_PWD_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        #ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id,IMAGE_NUMBERINPUT_PIN);
        #endif
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about PIN
        GUIEDIT_GetString(MMISMS_SET_SECURITY_NEW_PWD_CTRL_ID,&pwd_value);

        if (MMISMS_SECURITY_LEN_IS_IN_RANGE(pwd_value.wstr_len))
        {
            MMK_CreatePubEditWin((uint32*)MMISMS_SET_SECURITY_NEW_PWD_CNF_WIN_TAB,PNULL);
        }
    break;

    case MSG_SET_NEW_PWD_AGAIN:
        // get the value about new pwd again
        GUIEDIT_GetString(MMISMS_SET_SECURITY_NEW_PWD_CNF_CTRL_ID,&pwd_value_cnf);

        // get the value about new pwd
        GUIEDIT_GetString(MMISMS_SET_SECURITY_NEW_PWD_CTRL_ID,&pwd_value);
        
        //比较两个pwd是否相同
        if( MMIAPICOM_StrCompare( &pwd_value, &pwd_value_cnf ) )
        //if (MMIAPICOM_CompareTwoTeleNum((uint8*)pwd_value_cnf.str_ptr,(uint8)pwd_value_cnf.str_len,(uint8*)pwd_value.str_ptr,(uint8)pwd_value.str_len))
        {
            //set power on password
            MMISMS_NV_SetSecurityBoxPwd( pwd_value.wstr_ptr, pwd_value.wstr_len);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            
            //close input new pwd and input new pwd again window
            MMK_CloseWin(MMISMS_SET_SECURITY_NEW_PWD_WIN_ID);
            MMK_CloseWin(MMISMS_SET_SECURITY_NEW_PWD_CNF_WIN_ID);
        }
        else
        {
            //提示错误窗口,两次输入的pwd不一致
            MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PWD_CONFLICT);

            //清空password
            GUIEDIT_ClearAllStr(MMISMS_SET_SECURITY_NEW_PWD_CTRL_ID);

            //close input new pwd again window
            MMK_CloseWin(MMISMS_SET_SECURITY_NEW_PWD_CNF_WIN_ID);

            MMK_SetAtvCtrl(win_id,MMISMS_SET_SECURITY_NEW_PWD_CTRL_ID);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 处理确认新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPwdCnfWindow(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM                param
                                              )
{   
    MMI_STRING_T       pwd_cnf_value    =   {0};  
    MMI_RESULT_E        result          =   MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T edit_both_rect = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
		 MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif    
        GUIAPICTRL_SetBothRect(MMISMS_SET_SECURITY_NEW_PWD_CNF_CTRL_ID, &edit_both_rect);
        //set left softkey
        GUIEDIT_SetSoftkey(MMISMS_SET_SECURITY_NEW_PWD_CNF_CTRL_ID,0,MMISMS_SECURITY_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,MMISMS_SET_SECURITY_NEW_PWD_CNF_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        #ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id,IMAGE_NUMBERINPUT_PIN);
        #endif
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about PIN
        GUIEDIT_GetString(MMISMS_SET_SECURITY_NEW_PWD_CNF_CTRL_ID,&pwd_cnf_value);

        if (MMISMS_SECURITY_LEN_IS_IN_RANGE(pwd_cnf_value.wstr_len))
        {
            //send the new pin again to input new pin window
            MMK_SendMsg(MMISMS_SET_SECURITY_NEW_PWD_WIN_ID,MSG_SET_NEW_PWD_AGAIN,PNULL);
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}


/*****************************************************************************/
// 	Description : 安全信箱密码是否正确
//	Global resource dependence : 
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCorrectSecurityboxPwd(
                                          const wchar *pwd_ptr,
                                          uint16     pwd_len
                                          )
{
    BOOLEAN                 result = FALSE;
    MMISMS_SECURITYBOX_PWD_T   security_pwd = {0};
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMISMS_SECURITYBOX_PWD_T security_default_pwd ={{0},MMISMS_SECURITY_PWD_DEFAULT_LEN};
    
    if (PNULL == pwd_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:IsCorrectSecurityboxPwd pwd_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_24813_112_2_18_2_55_12_834,(uint8*)"");
        return FALSE;
    }    
    //read power on password from
    MMINV_READ(MMINV_SMS_SECURITY_PWD,&security_pwd,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMCPY(&security_pwd,&security_default_pwd,sizeof(MMISMS_SECURITYBOX_PWD_T));
        MMINV_WRITE(MMINV_SMS_SECURITY_PWD,&security_pwd);
    }
    
    if( pwd_len == security_pwd.pwd_len
        && 0 == MMIAPICOM_WstrStrncmp( pwd_ptr, (const uint8*)security_pwd.pwd, pwd_len ) )
    {
        result = TRUE;
    }

    //result = MMIAPICOM_CompareTwoTeleNum((uint8*)pwd_ptr,(uint8)pwd_len,(uint8*)phone_code.pwd,(uint8)phone_code.pwd_len);
    
    return (result);
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenBoxMenuOpt(MMIPUB_MENU_NOTIFY_T *param )
{       
    uint32 group_id, menu_id;
   
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;    
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    
    group_id = param->group_id;
    menu_id = param->menu_id;
    
    win_data_ptr->menu_id = menu_id;
    win_data_ptr->group_id = group_id;
    switch(menu_id)
    {
        case MMISMS_MENU_ANSWER_SMS_ITEM_ID:
            MMISMS_AnswerSms(win_data_ptr->cur_focus_order_id,MMISMS_BOX_SECURITY,MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,&win_data_ptr->marked_sms_num);
            break;

        case MMISMS_MENU_ANSWER_MMS_ITEM_ID:
            MMISMS_AnswerMMS(win_data_ptr->cur_focus_order_id,MMISMS_BOX_SECURITY,MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,&win_data_ptr->marked_sms_num);
            break;

        case MMISMS_MENU_WRITE_ID_ITEM_ID:
            MMISMS_EnterNewSmsNewWin();
            break;
            
        case MMISMS_MENU_NEW_SMS_ITEM_ID:
            MMISMS_SetSecurityBoxType(MMISMS_BOX_NONE);
            MMISMS_OpenWriteWin();
            break;
            
        case MMISMS_MENU_NEW_MMS_ITEM_ID:
            MMIAPIMMS_EditNewMMS();
            break;

#ifdef MMI_PDA_SUPPORT
        case MMISMS_MENU_MARK_ITEM_ID:
            MMISMS_MarkOpt(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,MMISMS_BOX_SECURITY,&win_data_ptr->marked_sms_num);
            break;
#endif

        case MMISMS_MENU_TIME_SORT_ITEM_ID:
            MMISMS_SetOrderType( MMISMS_ORDER_BY_TIME);
            break;
            
        case MMISMS_MENU_NAME_SORT_ITEM_ID:
            MMISMS_SetOrderType( MMISMS_ORDER_BY_NAME);
            break;
            
        case MMISMS_MENU_SUBJECT_SORT_ITEM_ID:
            MMISMS_SetOrderType( MMISMS_ORDER_BY_CONTENT);
            break;
            
        case MMISMS_MENU_TYPE_SORT_ITEM_ID:
            MMISMS_SetOrderType( MMISMS_ORDER_BY_TYPE);
            break;
            
        case MMISMS_MENU_MODIFY_KEY_ITEM_ID:
            MMK_CreatePubEditWin((uint32*)MMISMS_SECURITYBOX_PWD_WIN_TAB,(ADD_DATA)win_data_ptr); 
            break;
        case MMISMS_MENU_OPT_MARK_ITEM_ID:
            MMISMS_MarkItem(win_data_ptr->cur_focus_order_id,MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,&win_data_ptr->marked_sms_num);
            break;
        case MMISMS_MENU_CANCEL_MARK_ITEM_ID:           
            MMISMS_CancelMarkItem(win_data_ptr->cur_focus_order_id,MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,&win_data_ptr->marked_sms_num);
            break;
            
        case MMISMS_MENU_MARKALL_ITEM_ID: 
            MMISMS_MarkAllItem(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,MMISMS_BOX_SECURITY,&win_data_ptr->marked_sms_num,FALSE);
            break;

        case MMISMS_MENU_CANCLE_MARKALL_ITEM_ID:
            MMISMS_CancelMarkAllItem(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,MMISMS_BOX_SECURITY,&win_data_ptr->marked_sms_num,FALSE);
            break;
            
        case MMISMS_MENU_MARK_NUM_ITEM_ID:
            MMISMS_MarkSameNumItem(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,MMISMS_BOX_SECURITY,&win_data_ptr->marked_sms_num);
            break;

        case MMISMS_MENU_CANCEL_MARK_NUM_ITEM_ID:
            MMISMS_CancelMarkSameNumItem(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,MMISMS_BOX_SECURITY,&win_data_ptr->marked_sms_num);
            break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMISMS_MENU_MARK_SIM1_SMS_ITEM_ID:
        case MMISMS_MENU_MARK_SIM2_SMS_ITEM_ID:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_MENU_MARK_SIM3_SMS_ITEM_ID:
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_MENU_MARK_SIM4_SMS_ITEM_ID:
#endif
#else
        case MMISMS_MENU_MARK_SIM_SMS_ITEM_ID:
#endif 
        {
            MN_DUAL_SYS_E mark_sim_id = MN_DUAL_SYS_1;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            mark_sim_id = MN_DUAL_SYS_1+(menu_id-MMISMS_MENU_MARK_SIM1_SMS_ITEM_ID);            
#else
            mark_sim_id = MN_DUAL_SYS_1;
#endif
            MMISMS_MarkSimItem(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,MMISMS_BOX_SECURITY,mark_sim_id,&win_data_ptr->marked_sms_num);
        }
        break;
        
        case MMISMS_MENU_DEL_ITEM_ID:
        case MMISMS_MENU_DELBOX_ALL_ITEM_ID:
            HandleSecurityWinDelAll(win_data_ptr);            
            break;
        
        case MMISMS_MENU_DIAL_ITEM_ID:
            MMISMS_ReplyCall(win_data_ptr->cur_focus_order_id,CC_CALL_NORMAL_CALL);
            break;
#ifdef VT_SUPPORT            
        case MMISMS_MENU_VIDEOCALL_ITEM_ID:
            MMISMS_ReplyCall(win_data_ptr->cur_focus_order_id,CC_CALL_VIDEO_CALL);
            break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case MMISMS_MENU_IPDIAL_ITEM_ID:
#endif      
            MMISMS_ReplyCall(win_data_ptr->cur_focus_order_id,CC_CALL_IP_CALL);
            break;
        case MMISMS_MENU_LOCK_ITEM_ID:
            HandleSecurityWinLockItem(win_data_ptr);
            break;
            
        case MMISMS_MENU_UNLOCK_ITEM_ID:
            HandleSecurityWinUnLockItem(win_data_ptr);
            break;
            
#ifdef MMI_SMS_MOVE_SUPPORT 
        case MMISMS_MENU_BOX_SECURITY_ITEM_ID:
        case MMISMS_MENU_BOX_PREVIEW_ITEM_ID:
            HandleSecurityWinMoveToMe(win_data_ptr);  
            break;
            
        case MMISMS_MENU_BOX_MOVETOME_ITEM_ID:
            HandleSecurityWinMoveToMe(win_data_ptr);         
            break;
            
        case MMISMS_MENU_BOX_MOVETOSIM_ITEM_ID:
            HandleSecurityWinMoveToSIM(win_data_ptr);         
            break;    
#endif

#ifdef MMI_SMS_COPY_SUPPORT 
        case MMISMS_MENU_BOX_COPYTOME_ITEM_ID:
            HandleSecurityWinCopyToMe(win_data_ptr);         
            break; 

        case MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID:
            HandleSecurityWinCopyToSIM(win_data_ptr);         
            break;             
#endif        
        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_MAX;
                uint8 number_arr[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
                MMISMS_GetCurNumByOrder(win_data_ptr->cur_focus_order_id, &dual_sys, (uint8*)number_arr, MMISMS_PBNUM_MAX_LEN + 2);
                MMISMS_AddToPbNew(win_data_ptr->cur_focus_order_id,(uint8*)number_arr);
            }
            break;
    
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT    
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_MAX;
                uint8 number_arr[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
                MMISMS_GetCurNumByOrder(win_data_ptr->cur_focus_order_id, &dual_sys, (uint8*)number_arr, MMISMS_PBNUM_MAX_LEN + 2);
                MMISMS_AddToPbExist(win_data_ptr->cur_focus_order_id,(uint8*)number_arr);
            }
            break;
#endif    

        default:
            break;
    }
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenOpt(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};
    MMISMS_ORDER_ID_T cur_order_id = win_data_ptr->cur_focus_order_id;

    if ((win_data_ptr->marked_sms_num == 0 && cur_order_id != PNULL && cur_order_id->flag.is_locked)
    ||(win_data_ptr->marked_sms_num > 0 && PNULL == MMISMS_FindValidSecuritySMSForDelAll()))
    {
        menuparam.group_id = MENU_SMS_SECURITY_LIST_UNLOCK_OPT;
    }
    else
    {
        menuparam.group_id = MENU_SMS_SECURITY_LIST_OPT;
    }
       
    win_data_ptr->group_id = menuparam.group_id;
    menuparam.win_id =   MMISMS_SECURITYBOX_WIN_ID;
    menuparam.ctrl_id =  MMISMS_SECURITYBOX_OPT_MENU_CTRL_ID;    
    menuparam.dst_handle = win_data_ptr->win_handle;
    menuparam.func.OnMenuOk = HandleSecurityWinOpenBoxMenuOpt;  
    menuparam.func.OnMenuInit = HandleSecurityWinOpenInit;       
    MMIPUB_OpenPubMenuWin( &menuparam ); 
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinOpenInit(MMIPUB_MENU_NOTIFY_T *param)
{
    uint32 common_group_id = 0;
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;    
    uint16 count = 0;
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }      
    menu_enable_t.cur_order_id = win_data_ptr->cur_focus_order_id;
    menu_enable_t.box_type = MMISMS_BOX_SECURITY;
    menu_enable_t.marked_num = win_data_ptr->marked_sms_num;    
    menu_enable_t.win_id = MMISMS_SECURITYBOX_WIN_ID;

    if ((win_data_ptr->marked_sms_num == 0 && win_data_ptr->cur_focus_order_id != PNULL && win_data_ptr->cur_focus_order_id->flag.is_locked)
    ||(win_data_ptr->marked_sms_num > 0 && PNULL == MMISMS_FindValidSecuritySMSForDelAll()))
    {
        common_group_id = MENU_SMS_SECURITY_LIST_UNLOCK_OPT;
    }
    else
    {
       common_group_id = MENU_SMS_SECURITY_LIST_OPT;
    }
    
#ifdef MMI_PDA_SUPPORT
    menu_enable_t.group_id = common_group_id;
#else
    #if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    menu_enable_t.group_id = MENU_SMS_MTLIST_CALL_OPT;
    #endif
#endif

    MMISMS_HandleDialMenuEnableGray(&menu_enable_t);
#ifndef MMI_PDA_SUPPORT
#ifdef MMS_SUPPORT
#ifndef MMIMMS_SMS_IN_1_SUPPORT
    menu_enable_t.group_id = MENU_SMS_MTLIST_ANSWER_OPT;
#endif   
#endif    
#endif   
    if(menu_enable_t.group_id != MENU_SMS_MTLIST_ANSWER_OPT)
    {
        menu_enable_t.group_id = common_group_id;
    }
    
    MMISMS_HandleAnswerMsgMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleCommMenuEnableGray(&menu_enable_t);
    
    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);

#ifdef MMI_SMS_MOVE_SUPPORT     
    menu_enable_t.group_id = MENU_SMS_SECURITY_BOX_OPT;
    MMISMS_HandleMoveMenuEnableGray(&menu_enable_t);
#endif

#ifdef MMI_SMS_COPY_SUPPORT 
    menu_enable_t.group_id = MENU_SMS_COPY_OPT;
    MMISMS_HandleCopyMenuEnableGray(&menu_enable_t);
#endif

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleSortMenuEnableGray(&menu_enable_t);

    MMISMS_SetSortMenuFocusItem(MMISMS_SECURITYBOX_OPT_MENU_CTRL_ID,MENU_SMS_MTLIST_SORT_OPT);

    menu_enable_t.group_id = MENU_SMS_MTLIST_MARK_OPT;
    MMISMS_HandleMarkMenuEnableGray(&menu_enable_t);
    
    if(0 == menu_enable_t.marked_num && (PNULL != win_data_ptr->cur_focus_order_id) && (win_data_ptr->cur_focus_order_id->flag.msg_type  != MMISMS_TYPE_SMS))
    {
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, common_group_id, MMISMS_MENU_COPY_ITEM_ID, TRUE);        
    }    
    
    count = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_SECURITY);
    if( 0 == count)
    {
        MMIAPICOM_EnableGrayed(MMISMS_SECURITYBOX_WIN_ID, MENU_SMS_SECURITY_LIST_OPT, MMISMS_MENU_DEL_ITEM_ID, TRUE);   
        MMIAPICOM_EnableGrayed(MMISMS_SECURITYBOX_WIN_ID, MENU_SMS_SECURITY_LIST_OPT, MMISMS_MENU_LOCK_ITEM_ID, TRUE);          
        MMIAPICOM_EnableGrayed(MMISMS_SECURITYBOX_WIN_ID, MENU_SMS_SECURITY_LIST_OPT, MMISMS_MENU_MOVE_ITEM_ID, TRUE);     
        MMIAPICOM_EnableGrayed(MMISMS_SECURITYBOX_WIN_ID, MENU_SMS_SECURITY_LIST_OPT, MMISMS_MENU_MARK_ITEM_ID, TRUE);
        MMIAPICOM_EnableGrayed(MMISMS_SECURITYBOX_WIN_ID, MENU_SMS_SECURITY_LIST_OPT, MMISMS_MENU_SORT_ITEM_ID, TRUE);        
    }
}


/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecurityWinDeleteCallBack(void *user_date,MMISMS_DELETE_DATE_INFO *data_info_ptr)
{
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T*)user_date;
    if(PNULL == win_data_ptr)
    {
        return FALSE;
    }
    win_data_ptr->marked_sms_num = 0;
#ifdef MMI_PDA_SUPPORT
    MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_SECURITY, GUIFORM_CHILD_DISP_HIDE);
#endif
    GUILIST_SetTextListMarkable(win_data_ptr->cur_list_ctrl_id, FALSE);
    MMISMS_SetAllMsgMarked(MMISMS_BOX_SECURITY, FALSE);

    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
    MMK_CloseWin(MMISMS_MTBOX_WIN_ID);
    MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
    MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
    MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);

    MMISMS_SetDelAllFlag(FALSE);  
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value ) //begin deleting sms
    {
        //finish the del
        if(data_info_ptr->is_mms_del)
        {
            MMIAPIMMS_SaveListInfo();
        }
    }
    else if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMISMS_ClearOperInfo();       
    }
    MAIN_SetMsgState(TRUE);
    return TRUE;
}

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecurityWinDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T cur_ctrl_id = MMISMS_SECURITYBOX_LISTBOX_CTRL_ID;
    MMISMS_OPERATE_DATA_INFO_T data_info = {0};
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_SECURITYBOX_CHILD_WIN_ID);

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:   
        //MMISMS_OperateDelorDelAll(win_data_ptr->cur_focus_order_id,win_data_ptr->marked_sms_num,SecurityWinHandleDelWaitWinMsg);
        #ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
    
            return recode;
        }
#endif
        MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
        data_info.box_type = MMISMS_BOX_SECURITY; 
        MMISMS_EnterDelWaitWin(&data_info,HandleSecurityWinDeleteCallBack,(void*)win_data_ptr);
        
        if(win_data_ptr->marked_sms_num > 0)
        {
            GUILIST_SetCurItemIndex(cur_ctrl_id, 0);
            GUILIST_SetTopItemIndex(cur_ctrl_id, 0);
        }

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMISMS_CancelMarkAllItem(cur_ctrl_id,MMISMS_BOX_SECURITY, &win_data_ptr->marked_sms_num, FALSE);
#ifdef MMI_PDA_SUPPORT     
        GUISOFTKEY_SetTextId(win_data_ptr->win_handle, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE,STXT_CANCEL,FALSE);         
        win_data_ptr->is_mark_all = FALSE; 
#endif        
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinDelAll(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{    
    MMISMS_DELETE_TYPE_E delete_type = MMISMS_DELETE_MAX;

    if (MMISMS_IsUpdateOk())
    {
        delete_type = (0 == win_data_ptr->marked_sms_num)?MMISMS_DELETE_SINGLE:MMISMS_DELETE_PART;
        MMISMS_OpenDelteQueryWin(delete_type,FALSE,win_data_ptr->cur_focus_order_id,MMISMS_BOX_SECURITY,HandleSecurityWinDelQueryWinMsg);
    }    
}

/*****************************************************************************/
//     Description : handle msg box lock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinLockItem(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_SetGlobalOperationOrderId(win_data_ptr->cur_focus_order_id);
    if (0 == win_data_ptr->marked_sms_num )
    {
        MMISMS_SetCurMsgLocked(TRUE);//锁定当前信息
    }
    else
    {
        MMISMS_SetMarkedMsgLock(MMISMS_BOX_SECURITY,TRUE);//锁定标记信息
        win_data_ptr->marked_sms_num = 0;//锁定后去掉标记
        GUILIST_SetTextListMarkable(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID, FALSE);
    #ifdef MMI_PDA_SUPPORT
        MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_SECURITY, GUIFORM_CHILD_DISP_HIDE);
    #endif
        MMISMS_SetAllMsgMarked(MMISMS_BOX_SECURITY, FALSE);
    }
    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);       
}

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinUnLockItem(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_SetGlobalOperationOrderId(win_data_ptr->cur_focus_order_id);
    if (0 == win_data_ptr->marked_sms_num)
    {
        MMISMS_SetCurMsgLocked(FALSE);//解锁当前信息
    }
    else
    {
        MMISMS_SetMarkedMsgLock(MMISMS_BOX_SECURITY,FALSE);//解锁标记信息
        win_data_ptr->marked_sms_num = 0;
        GUILIST_SetTextListMarkable(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID, FALSE);
#ifdef MMI_PDA_SUPPORT
        MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_SECURITY, GUIFORM_CHILD_DISP_HIDE);
#endif
        MMISMS_SetAllMsgMarked(MMISMS_BOX_SECURITY, FALSE);
    }
    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);    
}
#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinCopyToMe(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_ME);
        MMISMS_OpenCopyQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_SECURITY,win_data_ptr->marked_sms_num,win_data_ptr->cur_focus_order_id,HandleSecurityWinCopyQueryWinMsg);  
    }
}
/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecurityCopyCallBack(void *user_date,MMISMS_COPY_RESULT_INFO *data_info_ptr)
{
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T*)user_date;
    
    if(PNULL  == win_data_ptr || PNULL == data_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    win_data_ptr->marked_sms_num = 0;

    GUILIST_SetTextListMarkable(win_data_ptr->cur_list_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
    MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_SECURITY, GUIFORM_CHILD_DISP_HIDE);
#endif
    MMISMS_SetAllMsgMarked(MMISMS_BOX_SECURITY, FALSE);

    if(!MMISMS_GetMoveSingleFlag())
    {
        MMISMS_ClearOperInfo();
    }
    MMISMS_SetMoveAllFlag(FALSE);
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MAIN_SetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    MMIPUB_CloseWaitWin(MMISMS_COPY_ANIM_WIN_ID);  
    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value ||MMISMS_NO_ERR == data_info_ptr->ret_value ) //begin deleting sms
    {
        MMISMS_WriteFolderType();            
        MMISMS_ClearOperInfo();
    }
    else if(MMISMS_NO_ENOUGH_SPACE == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(TXT_NO_SPACE, IMAGE_PUBWIN_WARNING);     
        MMISMS_ClearOperInfo();        
    }
    else if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMISMS_ClearOperInfo();       
    }
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecurityWinCopyQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID; 
//    MMI_STRING_T prompt_str = {0};
    MMISMS_COPY_DATA_INFO_T copy_data = {0};
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_SECURITYBOX_CHILD_WIN_ID);

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
    #ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
    
            return recode;
        }
    #endif

        MMISMS_SetMoveAllFlag(TRUE);
    
        if (0 == win_data_ptr->marked_sms_num)
        {
            MMISMS_SetCurMsgMarked(TRUE);
        }

        copy_data.box_type = MMISMS_BOX_SECURITY;            
        if(MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID == win_data_ptr->menu_id)
        {
            copy_data.oper_status = MMISMS_COPY_SMS_TO_SIM; 
        }
        else if(MMISMS_MENU_BOX_COPYTOME_ITEM_ID == win_data_ptr->menu_id)
        {
            copy_data.oper_status = MMISMS_COPY_SMS_TO_ME; 
        }    
        MMISMS_EnterCopyWaitWin(&copy_data,HandleSecurityCopyCallBack,win_data_ptr);            
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if(0 == win_data_ptr->marked_sms_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
        }
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}



/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinCopyToSIM(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        uint16 sim_num = 0;
        uint16 sim_ok = 0;
        MN_DUAL_SYS_E copy_dual_sys = MN_DUAL_SYS_1;
        uint32 copy_sim_num = 0;
        uint32 copy_mark_num = 0;
    
        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
    
        if(0 == win_data_ptr->marked_sms_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
        }
    
        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_SIM);
    
        if (0 == sim_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);            
        }
        else if (1 == sim_num)
        {
            copy_dual_sys = (MN_DUAL_SYS_E)sim_ok;
            copy_sim_num = MMISMS_GetMarkedStorageMsgNumByDualsys(MMISMS_BOX_SECURITY,MN_SMS_STORAGE_SIM,copy_dual_sys);
            copy_mark_num = MMISMS_GetMarkedStorageMsgNum(MMISMS_BOX_SECURITY,MN_SMS_STORAGE_ALL);
    
            if(copy_sim_num == copy_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            }
            else
            {
                MMISMS_SetCurSaveDualSys(copy_dual_sys);
                //MMISMS_OperateCopy();
                //MsgBoxOpenCopyQueryWinMsg(win_data_ptr);
                MMISMS_OpenCopyQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_SECURITY, win_data_ptr->marked_sms_num, win_data_ptr->cur_focus_order_id, HandleSecurityWinCopyQueryWinMsg);
            }
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(SecuritWinCopySelectSimCallback,PNULL,FALSE);
        }
#endif
    }    
}

/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  SecuritWinCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 copy_sim_num = 0;
    uint32 copy_mark_num = 0;
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;
    
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_SECURITYBOX_CHILD_WIN_ID);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }
    
    if (isReturnOK)
    {
        if(!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)dual_sys))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
            if(0 == win_data_ptr->marked_sms_num)
            {
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            }
        }
        else
        {  
            copy_sim_num = MMISMS_GetMarkedStorageMsgNumByDualsys(MMISMS_BOX_SECURITY,MN_SMS_STORAGE_SIM,(MN_DUAL_SYS_E)dual_sys);
            copy_mark_num = MMISMS_GetMarkedStorageMsgNum(MMISMS_BOX_SECURITY,MN_SMS_STORAGE_ALL);
            
            if(copy_sim_num == copy_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
                if(0 == win_data_ptr->marked_sms_num)
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
                }
            }
            else
            {
                MMISMS_SetCurSaveDualSys((MN_DUAL_SYS_E)dual_sys);
                //MMISMS_OperateCopy();
                //MsgBoxOpenCopyQueryWinMsg(win_data_ptr);
                MMISMS_OpenCopyQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_SECURITY, win_data_ptr->marked_sms_num, win_data_ptr->cur_focus_order_id, HandleSecurityWinCopyQueryWinMsg);
            }
        }
    }    
    return recode;    
}
#endif

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  SecuritWinMoveSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 move_sim_num = 0;
    uint32 move_mark_num = 0;
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;
    
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_SECURITYBOX_CHILD_WIN_ID);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }
    
    if(isReturnOK)
    {
        move_sim_num = MMISMS_GetMarkedStorageMsgNumByDualsys(MMISMS_BOX_SECURITY,MN_SMS_STORAGE_SIM,(MN_DUAL_SYS_E)dual_sys);
        move_mark_num = MMISMS_GetMarkedStorageMsgNum(MMISMS_BOX_SECURITY,MN_SMS_STORAGE_ALL);

        if(!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)dual_sys))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
            if(0 == win_data_ptr->marked_sms_num)
            {
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            }
        }
        else
        {
            if(move_sim_num == move_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_MOVE);
                if(0 == win_data_ptr->marked_sms_num)
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
                }
            }
            else
            {
                MMISMS_SetCurSaveDualSys((MN_DUAL_SYS_E)dual_sys);
                MMISMS_OpenMoveQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_SECURITY,win_data_ptr->marked_sms_num,win_data_ptr->cur_focus_order_id,HandleSecuritWinMoveQueryWinMsg);
            }
        }
    }
    
    return recode;
}


/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinMoveToSIM(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        uint16 sim_num = 0;
        uint16 sim_ok = 0;
        MN_DUAL_SYS_E move_dual_sys = MN_DUAL_SYS_1;
        uint32 move_sim_num = 0;
        uint32 move_mark_num = 0;
    
        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
    
        if(win_data_ptr->marked_sms_num == 0)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
        }
    
        MMISMS_SetMoveOperStatus(MMISMS_MOVE_SMS_TO_SIM);
    
        if (0 == sim_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);            
        }
        else if (1 == sim_num)
        {
            move_dual_sys = (MN_DUAL_SYS_E)sim_ok;
    
            move_sim_num = MMISMS_GetMarkedStorageMsgNumByDualsys(MMISMS_BOX_SECURITY,MN_SMS_STORAGE_SIM,move_dual_sys);
            move_mark_num = MMISMS_GetMarkedStorageMsgNum(MMISMS_BOX_SECURITY,MN_SMS_STORAGE_ALL);
    
            if(move_sim_num == move_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_MOVE);
                MMISMS_SetCurMsgMarked(FALSE);
            }
            else
            {
                MMISMS_SetCurSaveDualSys(move_dual_sys);
                //MMISMS_OperateMove();
                MMISMS_OpenMoveQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_SECURITY,win_data_ptr->marked_sms_num,win_data_ptr->cur_focus_order_id,HandleSecuritWinMoveQueryWinMsg);
            }
        }
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(SecuritWinMoveSelectSimCallback,(ADD_DATA)win_data_ptr,FALSE);
            //MMK_CreateWin((uint32 *)MMISMS_MOVE_SELECT_SIM_WIN_TAB, PNULL);
        }
    #endif
    }
}

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSecurityWinMoveToMe(MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        MMISMS_OpenMoveQueryWinMsg(win_data_ptr->menu_id,MMISMS_BOX_SECURITY,win_data_ptr->marked_sms_num,win_data_ptr->cur_focus_order_id,HandleSecuritWinMoveQueryWinMsg);       
    }
}

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecurityMoveCallBack(void *user_date,MMISMS_MOVE_RESULT_INFO *data_info_ptr)
{
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T*)user_date;
    MMI_RESULT_E ret = FALSE;
    if(PNULL == win_data_ptr)
    {
        return ret;
    }
    win_data_ptr->marked_sms_num = 0;
    
    GUILIST_SetTextListMarkable(win_data_ptr->cur_list_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
    MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_SECURITY, GUIFORM_CHILD_DISP_HIDE);
#endif
    MMISMS_SetAllMsgMarked(MMISMS_BOX_SECURITY, FALSE);

    if(!MMISMS_GetMoveSingleFlag())
    {
        MMISMS_ClearOperInfo();
    }
    MMISMS_SetMoveAllFlag(FALSE);
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MAIN_SetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif

    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value) //begin deleting sms
    {
        if(data_info_ptr->is_mms_move)
        {
            MMIAPIMMS_SaveListInfo();
        }

        MMISMS_WriteFolderType();
        MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);
        MMISMS_ClearOperInfo();
        ret = TRUE;
    }
    else if(MMISMS_NO_ENOUGH_SPACE == data_info_ptr->ret_value)
    {        
        if(data_info_ptr->is_mms_move)
        {
            MMIAPIMMS_SaveListInfo();
        }

        MMISMS_OpenMsgBox(TXT_NO_SPACE, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);
        MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);        
        MMISMS_ClearOperInfo();   
        ret = FALSE;
    }     
    else if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {   
        if(data_info_ptr->is_mms_move)
        {
            MMIAPIMMS_SaveListInfo();
        }
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);     
        MMISMS_ClearOperInfo();     
        ret = FALSE;
    }
    return ret;
}

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecuritWinMoveQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;

    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID; 
    MMISMS_SECURITYBOX_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_MOVE_DATA_INFO_T move_data_info = {0};
    win_data_ptr = (MMISMS_SECURITYBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_SECURITYBOX_CHILD_WIN_ID);
    
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:           
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
    
            return recode;
        }
#endif

        MMISMS_SetMoveAllFlag(TRUE);
    
        move_data_info.box_type = MMISMS_BOX_SECURITY;
        move_data_info.oper_status = MMISMS_GetCurrentMoveOperStatus();
        MMISMS_EnterMoveWaitWin(&move_data_info, HandleSecurityMoveCallBack, win_data_ptr);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if(0 == win_data_ptr->marked_sms_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
        }
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}


#endif
#endif

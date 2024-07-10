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
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_default.h"
#include "mmisms_id.h"
#include "mmisms_menutable.h"
#include "mmisms_text.h"
#include "mmisms_send.h"
#include "mmisms_commonui.h"
#include "mmipub.h"
#include "mmisms_save.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//END

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUCTION DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : to handle the send prompt option window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendPromptOptMenuWinMsg(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM                param
                                                 );
                                                 
WINDOW_TABLE( MMISMS_SEND_PROMPT_OPT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSendPromptOptMenuWinMsg),    
    WIN_ID( MMISMS_SEND_PROMPT_OPT_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_SMS_SEND_PROMPT_OPT, MMISMS_SEND_PROMPT_OPT_CTRL_ID),
    END_WIN
};

 
/*****************************************************************************/
//     Description : to handle the send prompt option window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendPromptOptMenuWinMsg(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM                param
                                                 )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SEND_PROMPT_OPT_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMISMS_OPER_ERR_E send_err_code = MMISMS_NO_ERR;
    MMI_STRING_T title_string = {0};
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    MMI_WIN_ID_T  query_win_id = MMISMS_QUERY_WIN_ID;

    //SCI_TRACE_LOW:"MMISMS: HandleSendPromptOptMenuWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_17940_112_2_18_2_54_58_795,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 
        MMI_GetLabelTextByLang(TXT_COMM_SEND_OPTION, &title_string);
        GUIMENU_SetMenuTitle(&title_string, ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMISMS_MENU_SIM1_SEND_ITEM_ID:
            MMISMS_SetSaveFlag(FALSE);
            send_err_code = MMISMS_WintabSendSMS(FALSE);
            MMISMS_DisplaySendErr( dual_sys, send_err_code );

            MMISMS_CloseEditWin();

            // close current window
            MMK_CloseWin(win_id);
            break;

        case MMISMS_MENU_SAVE_SEND_ITEM_ID:
            MMISMS_SetSaveFlag(TRUE);
            if (MMISMS_HaveEnoughSpace(dual_sys, TRUE, 0))
            {
                // if there have enough space, send and save the SMS immediately.
                send_err_code = MMISMS_WintabSendSMS(FALSE);
                MMISMS_DisplaySendErr( dual_sys, send_err_code );				
                MMISMS_CloseEditWin();
                // close current window
                MMK_CloseWin(win_id);
            }
            else
            {
                //提示空间不足是否继续？
                MMIPUB_OpenQueryWinByTextId(TXT_SMS_QUERY_CONTINUE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            }
            break;

        default:
            // close current window
            MMK_CloseWin(win_id);
            break;
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(&query_win_id);
        MMISMS_SetSaveFlag(FALSE);
        send_err_code = MMISMS_WintabSendSMS(FALSE);
        MMISMS_DisplaySendErr( dual_sys , send_err_code );
        MMISMS_CloseEditWin();
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return (recode);
}

/*****************************************************************************/
//     Description : to enter send prompt win
//    Global resource dependence : 
//  Author: rong gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterSendPromptWin(ADD_DATA data_ptr)
{
    MMK_CreateWin((uint32 *)MMISMS_SEND_PROMPT_OPT_WIN_TAB, (ADD_DATA)data_ptr);
}

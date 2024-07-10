#include "mmi_app_common_trc.h"
#ifdef WIN32
/*****************************************************************************
** File Name:      mmi_softkeytest.c                                          *
** Author:                                                                   *
** Date:           06/17/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2011      bin.ji           Creat                                  *
******************************************************************************/
#define _MMI_SOFTKEYTEST_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmibt_text.h"
#include "mmibt_app.h"
#include "mmipub.h"
#include "mmi_id.h"
#include "window_parse.h"
#include "mmicc_id.h"
#include "mmiset_export.h"

#define     MMICC_SELECT_SIM_MAX_NUM      2

/*****************************************************************************/
//  Description : handle test window message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id,
                                    DPARAM            param
                                    );

//显示使用SIM卡拨出的选项                                                
LOCAL WINDOW_TABLE( MMICC_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleTestWinMsg),    
    WIN_ID(MMICC_COM_SELECT_SIM_WIN_ID),//Sam.hua@Dec20. this Win id is removed
    WIN_TITLE(TXT_SIM_SEL),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
//    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
//    WIN_SOFTKEY(STXT_RETURN, TXT_DELETE, STXT_OK),
//    WIN_SOFTKEY(STXT_RETURN, TXT_NULL, TXT_NULL),
    WIN_SOFTKEY(TXT_NULL, STXT_RETURN, TXT_NULL),
//    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICC_COM_SELECT_SIM_CTRL_ID),//Sam.hua@Dec20. this ctrl id is removed
    END_WIN
};

/*****************************************************************************/
//  Description : handle test window message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id,
                                    DPARAM            param
                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;    
    MMI_TEXT_ID_T   label = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMI_CTRL_ID_T       ctrl_id = MMICC_COM_SELECT_SIM_CTRL_ID;
    MMI_CONTROL_CREATE_T        softkey_ctrl = {0};
    GUISOFTKEY_INIT_DATA_T      softkey_init = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_SetMaxItem(ctrl_id, MMICC_SELECT_SIM_MAX_NUM, FALSE );//max item 2
            MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, MMICC_SELECT_SIM_MAX_NUM, MMISET_APPEND_PS_READY);
//            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, TXT_NULL, STXT_RETURN, FALSE);
    //        MMK_SetAtvCtrl(win_id, ctrl_id);
            //init softkey
            /*
            softkey_init.leftsoft_id  = STXT_OPTION;
            softkey_init.midsoft_id   = TXT_NULL;
            softkey_init.rightsoft_id = TXT_DELETE;

            //create softkey control
            softkey_ctrl.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
            softkey_ctrl.guid              = SPRD_GUI_SOFTKEY_ID;
            softkey_ctrl.init_data_ptr     = &softkey_init;
            softkey_ctrl.parent_win_handle = win_id;
            MMK_CreateControl(&softkey_ctrl);
            MMK_SetWinSoftkeyCtrlId(win_id,softkey_ctrl.ctrl_id);
            */
        }
        break;

    case MSG_APP_LEFT: 
        //button0变灰
        GUIWIN_SetSoftkeyBtnState(win_id, 0, TRUE, FALSE);
        GUIWIN_UpdateSoftkey(win_id);
        break;
        
    case MSG_APP_RIGHT: 
        //set button text
        /*
        {
            GUI_RECT_T rect = {0};
            
            GUIAPICTRL_GetRect(MMITHEME_GetSoftkeyCtrlId(), &rect);
            rect.top -= 30;
            rect.bottom -= 30;
            rect.left += 30;
            rect.right -= 30;            
            GUIAPICTRL_SetRect(MMITHEME_GetSoftkeyCtrlId(), &rect);
            GUISOFTKEY_Update(win_id, MMITHEME_GetSoftkeyCtrlId());
        }
        */
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, STXT_RETURN, TXT_DELETE, TRUE);
//        GUIWIN_SeSoftkeytButtonTextId( win_id,  TXT_DELETE, 0, TRUE);
        break;

    case MSG_APP_UP:
        //set button test ptr
        {
            MMI_STRING_T kstring = {0};
            wchar test_string[] = L"hello world---------ABC";

            kstring.wstr_ptr = test_string;
            kstring.wstr_len = MMIAPICOM_Wstrlen(test_string);
            //button1变成"hello world---------ABC"
            GUIWIN_SetSoftkeyButtonTextPtr( win_id, kstring, 1, TRUE);
        }
        break;
        
    case MSG_APP_DOWN:
        //get button text id, not support
        {
            MMI_TEXT_ID_T text_id[3] = {0};
            BOOLEAN	result = FALSE; 
            
            //TXT_NULL, STXT_OK, STXT_RETURN
//            result = GUIWIN_GetSoftkeyTextId( win_id, &text_id[0], &text_id[1], &text_id[2] );
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_RETURN, TXT_COMMON_OK, TRUE);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        if ((PNULL != param) && (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
        {
            //left softkey处理
            //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOFTKEYTEST_153_112_2_18_1_59_3_75,(uint8*)"");
        }
        else if ((PNULL != param) && (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
        {
            //middle softkey处理
            //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOFTKEYTEST_158_112_2_18_1_59_3_76,(uint8*)"");
        }
        else if ((PNULL != param) && (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
        {
            //right softkey处理
            //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOFTKEYTEST_163_112_2_18_1_59_3_77,(uint8*)"");
        }
        else
#endif
        {
            //其他消息处理
            param = param;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        /*
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        */

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Discription: This fucntion is used to test softkey 
//  
//  Global resource dependence: None
//  Author: bin.ji
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMISOFTKEY_Test( void )
{
    MMK_CreateWin((uint32*)MMICC_SELECT_SIM_WIN_TAB, PNULL);  
}

#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527

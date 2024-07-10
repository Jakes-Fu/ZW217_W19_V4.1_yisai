/****************************************************************************
** File Name:      mmisms_chatreadwin.c                                    *
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
#include "guilistbox.h"
#include "guimenu.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_menutable.h"
#include "mmisms_order.h"
#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif
#include "mmimms_export.h"
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#include "mail_export.h"
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
#include "mmisms_mainmenuwin.h"
#include "mmisms_messagecountwin.h"
#include "mmisms_messageboxwin.h"
//#include "mmisms_simboxwin.h"
#ifdef MMI_SMS_SECURITYBOX_SUPPORT
#include "mmisms_securitywin.h"
#endif
//#include "mmismsapp_wintab.h"
#include "mmiudisk_export.h"

#ifdef MMISEARCH_SUPPORT_ONLYMMS
#include "mmisearch_export.h"
#endif
#include "mmisms_appcontrol.h"

#ifdef MMI_SMS_SIMBOX
#include "mmisms_simboxwin.h"
#endif
#include "mmisms_showsmswin.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISMS_MENUWIN_TIME                  1000
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


/*****************************************************************************/
//     Description : to handle the SMS menu window message
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsMainMenuWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
);

/*****************************************************************************/
//     Description : to handle the SMS menu window message
//    Global resource dependence : 
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsNewMsgMenuWinMsg(
                                                   MMI_WIN_ID_T        win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM            param
                                                  );

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void MainMenuWinCloseCtrlFunc (MMISMS_MAINMENU_WINDATA_T *win_data_ptr);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
#ifndef MMI_GUI_STYLE_TYPICAL
// the window for main menu
WINDOW_TABLE( MMISMS_MENUWIN_TAB ) =
{
    WIN_FUNC((uint32) HandleSmsMainMenuWinMsg),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
    WIN_TITLE(TXT_COMMUNICATION_MESSAGE),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMISMS_MENU_CTRL_ID),
    WIN_ID( MMIMSG_MAIN_MENU_WIN_ID ),
    END_WIN
};
#else
// the window for main menu
WINDOW_TABLE( MMISMS_MENUWIN_TAB ) =
{
    WIN_FUNC((uint32) HandleSmsMainMenuWinMsg),
#ifdef DPHONE_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_STATUSBAR,
    WIN_TITLE(TXT_COMMUNICATION_MESSAGE),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
    WIN_ID( MMIMSG_MAIN_MENU_WIN_ID ),
    END_WIN
};
#endif

//add by liming.deng  begin
WINDOW_TABLE( MMISMS_NEW_MSG_MENUWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSmsNewMsgMenuWinMsg),    
    WIN_ID( MMISMS_NEW_MSG_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_NEW_MSG_OPT, MMISMS_NEW_MSG_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};
//add by liming.deng end


/*****************************************************************************/
//     Description : according to the menu id, open the corresponding window
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void EnterSmsCorrespondingMenu(MMISMS_MAIN_MENU_NODE_E node_id);
/*****************************************************************************/
//     Description : handle chat
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsMainMenuOptOK(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//  Description : Start Check if need update sms menu timer
//  Global resource dependence :
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
LOCAL void StartMenuWinTimer(
                                  MMISMS_MAINMENU_WINDATA_T *win_data_ptr
                                  );

/*****************************************************************************/
//  Description : stop Check if need update sms menu timer
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
LOCAL void StopMenuWinTimer(MMISMS_MAINMENU_WINDATA_T *win_data_ptr);

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : update sms main menu
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL void UpdateSmsMainMenu(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  ctrl_id                             
                             );
/*****************************************************************************/
//     Description : update sms main menu
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL void CreateSmsMainMenu(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  ctrl_id                             
                             );

/*****************************************************************************/
//     Description : set sms number state to string 
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL void SetNumState2String(
                              MMI_STRING_T        *string_ptr,    //OUT:
                              MMISMS_BOX_TYPE_E box_type        //IN:
                              );
#endif




#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void AppendPdaSmsMainListItem2Line(
    MMI_CTRL_ID_T  ctrl_id,    //控件ID
    MMI_TEXT_ID_T  text_1,     //主文本
    MMI_STRING_T  text_str,     //副文本
    MMI_IMAGE_ID_T image_id,   //图标ID
    uint32         user_data,   //用户数据
    BOOLEAN is_replace,
    uint16 replace_index,
    GUIITEM_STYLE_E item_style
)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = image_id;

    //text 1
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_1;

    //text 2
    if (text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = text_str.wstr_ptr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = text_str.wstr_len;
    }
    if (is_replace)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, replace_index);
    }
    else
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }
}

/*****************************************************************************/
//  Description :Init pda sms main list
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void  InitPdaSmsMainListCtrl(BOOLEAN is_replace)
{
    MMI_CTRL_ID_T       ctrl_id = MMISMS_MENU_CTRL_ID;
    MMI_STRING_T  text_str = {0};
    uint8 num_string[10 + 1] = {0};
    uint16 num_string_len = 0;
    wchar num_wstring[10 + 1] = {0};
    uint16 total_num = 0;
    uint16 unread_num = 0;
    uint16 item_index = 0;

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
	uint16 bt_msg_num = 0;
#endif

    if (!is_replace)
    {
        GUILIST_SetMaxItem(ctrl_id, MMISMS_MAIN_MENU_NODE_MAX, FALSE );
    }

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    
    if(FALSE == MMIAPIBT_IsConnectDevice() || MMIAPISMS_GetCurOperIsDisconnect())
    {
        SCI_TRACE_LOW("[MMISMS] InitPdaSmsMainListCtrl MMIAPIBT_IsConnectDevice is FALSE");
        MMIAPISMS_DeleteAllMapSyncInfo();
    }
    else
    {
        if(MMIAPISMS_GetCurOperIsBoot() || MMIAPISMS_GetCurOperIsDisconnect())
        {
            MMIAPISMS_DeleteAllMapSyncInfo();
        }
    }
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
    total_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_CHAT);
    unread_num = MMISMS_GetChatNodeTotalUnreadNum();


    num_string_len = sprintf((char *)num_string, "%d/%d", unread_num, total_num);
    MMI_STRNTOWSTR(num_wstring, 10, (const uint8 *)num_string, 10, num_string_len);
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
    //SCI_TRACE_LOW:"SetNumState2String:chat total_num = %d,unread_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_4445_112_2_18_2_54_29_724, (uint8*)"dd", total_num, unread_num);
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_SMS_CHAT_MODE, text_str, IMAGE_SECMENU_ICON_MESSAGE_CHATBOX, MMISMS_MAIN_MENU_NODE_CHAT, is_replace, item_index,GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR);
    item_index++;
#endif

    SCI_MEMSET(&text_str, 0x00, sizeof(MMI_STRING_T));
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_COMMON_WRITE_MESSAGE, text_str, IMAGE_SECMENU_ICON_MESSAGE_WRITE, MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG, is_replace, item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR);
    item_index++;

    SCI_MEMSET(&num_string, 0x00, sizeof(num_string));
    SCI_MEMSET(&num_wstring, 0x00, sizeof(num_wstring));
    total_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_MT);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    unread_num = MMISMS_GetTobeReadMessageCount(MN_DUAL_SYS_MAX) ;
#else
    unread_num = MMISMS_GetTobeReadMessageCount(MN_DUAL_SYS_1);
#endif
    unread_num += MMISMS_GetUndownloadMMSNumInOrder();
    num_string_len = sprintf((char *)num_string, "%d/%d", unread_num, total_num);
    MMI_STRNTOWSTR(num_wstring, 10, (const uint8 *)num_string, 10, num_string_len);
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
    //SCI_TRACE_LOW:"SetNumState2String:chat total_num = %d,unread_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_4465_112_2_18_2_54_29_725, (uint8*)"dd", total_num, unread_num);
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_INBOX, text_str, IMAGE_SECMENU_ICON_MESSAGE_INBOX, MMISMS_MAIN_MENU_NODE_INBOX, is_replace, item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT);
    item_index++;

    SCI_MEMSET(&num_string, 0x00, sizeof(num_string));
    SCI_MEMSET(&num_wstring, 0x00, sizeof(num_wstring));
    total_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_SENDFAIL);
    num_string_len = sprintf((char *)num_string, "%d", total_num);
    MMI_STRNTOWSTR(num_wstring, 10, (const uint8 *)num_string, 10, num_string_len);
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
    //SCI_TRACE_LOW:"SetNumState2String:chat total_num = %d,unread_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_4475_112_2_18_2_54_29_726, (uint8*)"dd", total_num, unread_num);
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_COMM_OUTBOX, text_str, IMAGE_SECMENU_ICON_MESSAGE_OUTBOX, MMISMS_MAIN_MENU_NODE_OUTBOX, is_replace, item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT);
    item_index++;

    SCI_MEMSET(&num_string, 0x00, sizeof(num_string));
    SCI_MEMSET(&num_wstring, 0x00, sizeof(num_wstring));
    total_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_NOSEND);
    num_string_len = sprintf((char *)num_string, "%d", total_num);
    MMI_STRNTOWSTR(num_wstring, 10, (const uint8 *)num_string, 10, num_string_len);
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
    //SCI_TRACE_LOW:"SetNumState2String:chat total_num = %d,unread_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_4485_112_2_18_2_54_29_727, (uint8*)"dd", total_num, unread_num);
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_COMM_DRAFT, text_str, IMAGE_SECMENU_ICON_MESSAGE_DRAFTBOX, MMISMS_MAIN_MENU_NODE_DRAFTBOX, is_replace, item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT);
    item_index++;

    SCI_MEMSET(&num_string, 0x00, sizeof(num_string));
    SCI_MEMSET(&num_wstring, 0x00, sizeof(num_wstring));
    total_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_SENDSUCC);
    num_string_len = sprintf((char *)num_string, "%d", total_num);
    MMI_STRNTOWSTR(num_wstring, 10, (const uint8 *)num_string, 10, num_string_len);
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
    //SCI_TRACE_LOW:"SetNumState2String:chat total_num = %d,unread_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_4495_112_2_18_2_54_29_728, (uint8*)"dd", total_num, unread_num);
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_SMS_SENTBOX, text_str, IMAGE_SECMENU_ICON_MESSAGE_SENT, MMISMS_MAIN_MENU_NODE_SENTBOX, is_replace, item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT);
    item_index++;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    SCI_MEMSET(&text_str, 0x00, sizeof(MMI_STRING_T));
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_SMS_SECURITY, text_str, IMAGE_SECMENU_ICON_MESSAGE_SECURITY, MMISMS_MAIN_MENU_NODE_SECURITYBOX, is_replace, item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR);
    item_index++;
#endif
    SCI_MEMSET(&text_str, 0x00, sizeof(MMI_STRING_T));
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_SMS_FREQWORD, text_str, IMAGE_SECMENU_ICON_WAP_BOOKMARKS, MMISMS_MAIN_MENU_NODE_FREQWORD, is_replace, item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR);
    item_index++;

#if defined(MMI_SMSCB_SUPPORT)
    SCI_MEMSET(&text_str, 0x00, sizeof(MMI_STRING_T));
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_SMS_CB, text_str, IMAGE_SECMENU_ICON_WAP_HOMEPAGE, MMISMS_MAIN_MENU_NODE_BROADCAST, is_replace, item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR);
    item_index++;
#endif

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
    SCI_MEMSET(&text_str, 0x00, sizeof(MMI_STRING_T));
    AppendPdaSmsMainListItem2Line(ctrl_id, TXT_VOICEMAIL, text_str, IMAGE_SECMENU_ICON_TOOL_RECORD, MMISMS_MAIN_MENU_NODE_VOICEMAIL, is_replace, item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR);
    item_index++;
#endif

#ifdef MMI_SMS_SIMBOX
    SCI_MEMSET(&text_str,0x00,sizeof(MMI_STRING_T));
    AppendPdaSmsMainListItem2Line(ctrl_id,TXT_SMS_SIM_BOX,text_str,IMAGE_SECMENU_ICON_MESSAGE_SIM_INBOX,MMISMS_MAIN_MENU_NODE_SIMBOX,is_replace,item_index, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR);    
    item_index++;    
#endif   
//end
}


/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMainMenuChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T        win_id = MMIMSG_MAIN_MENU_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms MTBox event ptr PNULL");
       return;
    }
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {          
        MMK_PostMsg(win_id, MMISMS_MSG_UPDATEMENU, PNULL, 0);
    }    
}

/*****************************************************************************/
//     Description : to handle the SMS menu option window message
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsMainMenuWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_MENU_CTRL_ID;
    uint32 node_id = 0;
    LOCAL BOOLEAN s_need_update = FALSE;
    uint16 index = 0;
    MMISMS_MAINMENU_WINDATA_T *win_data_ptr = (MMISMS_MAINMENU_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms mainmenu win_data_ptr param PNULL");
        return MMI_RESULT_TRUE;    
    }
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
            win_data_ptr->win_id = win_id;
            InitPdaSmsMainListCtrl(FALSE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMainMenuChangeCallBack, TRUE); 
            break;

        case MSG_LCD_SWITCH:
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_MENU:
#ifdef DPHONE_SUPPORT
#else
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                menuparam.group_id = MENU_SMS_POPUP_OPT;
                menuparam.win_id =   MMISMS_MAINMENU_OPT_MENU_WIN_ID;
                menuparam.ctrl_id =  MMISMS_MAINMENU_OPT_MENU_CTRL_ID;    
                menuparam.dst_handle = win_id;
                menuparam.func.OnMenuOk = HandleSmsMainMenuOptOK;                    
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            index = GUILIST_GetCurItemIndex(ctrl_id);
//        node_id =  index + 1;
            GUILIST_GetItemData(ctrl_id, index, &node_id);
            EnterSmsCorrespondingMenu((MMISMS_MAIN_MENU_NODE_E)node_id);
            break;

        case MMISMS_MSG_NEEDUPDATEMENU:
            s_need_update = TRUE;
            break;

        case MSG_GET_FOCUS:
            if (s_need_update)
            {
                InitPdaSmsMainListCtrl(TRUE);
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口              
                s_need_update = FALSE;
            }

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        SCI_TRACE_LOW("[MMISMS] HandleSmsMainMenuWinMsg MMIAPIBT_IsConnectDevice = %d,  MMIAPISMS_GetCurOperIsDisconnect = %d", MMIAPIBT_IsConnectDevice(), MMIAPISMS_GetCurOperIsDisconnect());
        if(FALSE == MMIAPIBT_IsConnectDevice() || MMIAPISMS_GetCurOperIsDisconnect())
        {
            MMIAPISMS_DeleteAllMapSyncInfo();
        }
#endif

            break;

        case MMISMS_MSG_UPDATEMENU:
            if(MMK_IsFocusWin(win_id))
            {
                InitPdaSmsMainListCtrl(TRUE);
                StartMenuWinTimer(win_data_ptr);
            }
            s_need_update = TRUE;
            break;

        case MSG_LOSE_FOCUS:
            StopMenuWinTimer(win_data_ptr);
            break;
            
        case MSG_TIMER:
            if (s_need_update)
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL); //刷新窗口
                s_need_update = FALSE;
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
			StopMenuWinTimer(win_data_ptr);                        
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.19
            MMIAPIMAIL_Exit();
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
            MainMenuWinCloseCtrlFunc(win_data_ptr);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMainMenuChangeCallBack, FALSE); 
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

#else
/*****************************************************************************/
//     Description : to handle the SMS menu window message
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsMainMenuWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_MENU_CTRL_ID;
    uint32 node_id = 0;
    LOCAL BOOLEAN s_need_update = FALSE;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMISMS_MAINMENU_WINDATA_T *win_data_ptr = (MMISMS_MAINMENU_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms mainmenu win_data_ptr param PNULL");
        return MMI_RESULT_TRUE;    
    }
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
#if 0
            if (MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
            {
                MMIAPISET_ValidatePrivacyPwd(win_id, MMISET_PROTECT_SMS_TYPE);
            }
#endif
            win_data_ptr->win_id = win_id;
            CreateSmsMainMenu(win_id, ctrl_id);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

        case MSG_LCD_SWITCH:
            if (MMITHEME_IsMainScreenLandscape())
            {
                GUIMENU_SetRect(ctrl_id, &both_rect.h_rect);
            }
            else
            {
                GUIMENU_SetRect(ctrl_id, &both_rect.v_rect);
            }
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_MENU:
#ifdef DPHONE_SUPPORT
#else
            //MMK_CreateWin((uint32 *)MMISMS_MAINMENU_OPT_MENU_WIN_TAB, PNULL);
            break;
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            EnterSmsCorrespondingMenu((MMISMS_MAIN_MENU_NODE_E)node_id);
            break;

        case MMISMS_MSG_NEEDUPDATEMENU:
            s_need_update = TRUE;
            break;

        case MSG_GET_FOCUS:
            if (s_need_update)
            {
                UpdateSmsMainMenu(win_id, ctrl_id);
                s_need_update = FALSE; //更新菜单后将静态变量置为初始值
            }
            break;

        case MMISMS_MSG_UPDATEMENU:
            UpdateSmsMainMenu(win_id, ctrl_id);
            StartMenuWinTimer(win_data_ptr);
            s_need_update = TRUE;
            break;

        case MSG_LOSE_FOCUS:
            StopMenuWinTimer(win_data_ptr);
            break;
            
        case MSG_TIMER:
            if (s_need_update)
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL); //刷新窗口
                s_need_update = FALSE;
            }
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:            
            StopMenuWinTimer(win_data_ptr);
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.19
            MMIAPIMAIL_Exit();
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
            MainMenuWinCloseCtrlFunc(win_data_ptr);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#endif

/*****************************************************************************/
//     Description : according to the menu id, open the corresponding window
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void EnterSmsCorrespondingMenu(MMISMS_MAIN_MENU_NODE_E node_id)
{
    switch (node_id)
    {
        case MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG:
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMISMS_OpenWriteWin();
#else
#ifdef MMS_SUPPORT
            //MMK_CreateWin((uint32 *)MMISMS_NEW_MSG_MENUWIN_TAB, PNULL);  //TODO
            MMISMS_EnterNewSmsNewWin();
#else
            MMISMS_OpenWriteWin();
#endif
#endif
            break;

        case MMISMS_MAIN_MENU_NODE_INBOX:
            MMISMS_EnterSmsBox( MMISMS_BOX_MT );
            break;

        case MMISMS_MAIN_MENU_NODE_OUTBOX:
            MMISMS_EnterSmsBox( MMISMS_BOX_SENDFAIL );
            break;

        case  MMISMS_MAIN_MENU_NODE_SENTBOX:
            MMISMS_EnterSmsBox( MMISMS_BOX_SENDSUCC );
            break;

        case MMISMS_MAIN_MENU_NODE_DRAFTBOX:
            MMISMS_EnterSmsBox( MMISMS_BOX_NOSEND );
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_MAIN_MENU_NODE_SECURITYBOX:
            //MMK_CreatePubEditWin((uint32*)MMISMS_SECURITYBOX_PWD_WIN_TAB, PNULL);
            //MMISMS_EnterSmsBox( MMISMS_BOX_SECURITY);
            MMIAPISMS_OpenValidateSecruityBox(PNULL);
            break;
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
        case MMISMS_MAIN_MENU_NODE_CHAT:
            MMISMS_EnterSmsBox( MMISMS_BOX_CHAT );
            break;
#endif

#if defined(DPHONE_SUPPORT ) || !defined(MMI_GUI_STYLE_TYPICAL)
        case MMISMS_MAIN_MENU_NODE_FREQWORD:
            //MMK_CreateWin((uint32 *)MMISMS_WORDLIST_WIN_TAB, PNULL);
            MMISMS_EnterWordListWin();
            break;
            
#ifdef MMISEARCH_SUPPORT_ONLYMMS
        case MMISMS_MAIN_MENU_NODE_MMS_SEARCH:
            MMIAPISEARCHMMS_Entry();
            break;
#endif            

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
        case MMISMS_MAIN_MENU_NODE_VOICEMAIL:
            MMIAPISMS_OpenVoicemailWin();
            break;
#endif

#ifdef MMI_SMS_SIMBOX
    case MMISMS_MAIN_MENU_NODE_SIMBOX:
        //
        //Select SIM and Enter
        MMISMS_OpenSIMBOX();
        break;
#endif

#if defined(MMI_SMSCB_SUPPORT)
        case MMISMS_MAIN_MENU_NODE_BROADCAST:
#ifndef MMI_GUI_STYLE_TYPICAL
            MMIAPISMSCB_OpenCBList();
#else
            MMIAPISMSCB_OpenMainMenu();
#endif
            break;
#endif

        case MMISMS_MAIN_MENU_NODE_SETTING://短信设置
            MMISMS_CreateSmsSettingWin();
            break;

        case MMISMS_MAIN_MENU_NODE_MEMSTATE:
            if (MMISMS_IsOrderOk())
            {
                MMISMS_EnterMessageCountWin();
            }
            else
            {
                MMISMS_OpenInitWaitingWindow(MMISMS_VIEW_MEMORY);
            }
            break;
#endif
        default:
            SCI_TRACE_LOW("MMISMS: EnterSmsCorrespondingMenu node_id = %d", node_id);
            break;
    }
}

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterSMSMainMenuWin(void)
{
    MMI_HANDLE_T win_handle = 0;
    MMISMS_MAINMENU_WINDATA_T *win_data_ptr = PNULL;

    if(MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
    {
        SCI_TRACE_LOW("mmisms main menu is open");
        return;
    }
    
    win_data_ptr = (MMISMS_MAINMENU_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_MAINMENU_WINDATA_T));

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms malloc error");
        return;
    }
    
    win_handle  = MMK_CreateWin((uint32*)MMISMS_MENUWIN_TAB, PNULL);

    if(win_handle == PNULL)
    {
        SCI_TRACE_LOW("mmisms MMISMS_EnterSMSMainMenuWin create error");
        SCI_FREE(win_data_ptr);
        return;
    }
    SCI_TRACE_LOW("mmisms MMISMS_EnterSMSMainMenuWin create success");
    MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);
    
}

/*****************************************************************************/
//     Description : handle chat
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsMainMenuOptOK(MMIPUB_MENU_NOTIFY_T *param)
{
    uint32 menu_id = 0;       
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    switch (menu_id)
    {
        case MMISMS_MENU_FREQWORD_ID_ITEM_ID:
            //MMK_CreateWin((uint32 *)MMISMS_WORDLIST_WIN_TAB, PNULL);
			MMISMS_EnterWordListWin();
            break;
            
    #ifdef MMI_SMS_VOICE_MAIL_SUPPORT
        case MMISMS_MENU_VOICEMAIL:
            MMIAPISMS_OpenVoicemailWin();
            break;
    #endif
    
    #if defined(MMI_SMSCB_SUPPORT)
        case MMISMS_MENU_BROADCAST_ID:
            MMIAPISMSCB_OpenCBList();
            break;
    #endif
    
    #ifdef DPHONE_SUPPORT
        case MMISMS_MENU_SETTING_ID_ITEM_ID://短信设置
    #endif
        case MMISMS_MENU_SMS_SETTING_ID_ITEM_ID:
            MMISMS_CreateSmsSettingWin();
            break;
    
        case MMISMS_MENU_MMS_SETTING_ID_ITEM_ID:
            MMIAPIMMS_OpenMMSSettingWin();
            break;


#ifdef MMISEARCH_SUPPORT_ONLYMMS
        case MMISMS_MENU_MMSSEARCH_ID_ITEM_ID:
            MMIAPISEARCHMMS_Entry();
            break;
#endif            
        case MMISMS_MENU_BROWSER_SETTING_ID_ITEM_ID:
            MMISMS_CreateBrowserSettingWin();
            break;
    
        case MMISMS_MENU_MEMSTATE_ID_ITEM_ID:
            if (MMISMS_IsOrderOk())
            {
				MMISMS_EnterMessageCountWin();
            }
            else
            {
                //g_oper_type = MMISMS_VIEW_MEMORY;
                MMISMS_OpenInitWaitingWindow(MMISMS_VIEW_MEMORY);
            }
            break;
    #ifdef MMI_SMS_DELALL_SUPPORT
        case MMISMS_MENU_DELALL_MSG_ITEM_ID:
            if (MMISMS_IsOrderOk())
            {
                MMISMS_EnterDelAllWin();
            }
            else
            {
                //g_oper_type = MMISMS_VIEW_MEMORY;
                MMISMS_OpenInitWaitingWindow(MMISMS_VIEW_MEMORY);
            }
            break;
    #endif
    
    #if defined(MMI_SMSCB_SUPPORT)
        case MMISMS_MENU_BROADCAST_SETTING_ITEM_ID:
            MMIAPISMSCB_OpenSettingWin();
            break;
    #endif
    
    #ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
        case MMISMS_MENU_MAIL_SETTING_ITEM_ID:
    
            mail_EnterMailSettings();
            break;
    #endif//PUSH_EMAIL_3_IN_1_SUPPORT
    
    #ifndef MMI_GUI_STYLE_TYPICAL
        case MMISMS_MENU_SETTING_ID_ITEM_ID:
            MMISMS_OpenSettingTabWin();   // to do
            break;
    #endif
    
        default:
            //SCI_TRACE_LOW:"HandleSmsMainMenuOptWinMsg menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_4848_112_2_18_2_54_30_729, (uint8*)"d", menu_id);
            break;
    }
}



/*****************************************************************************/
//  Description : Start Check if need update sms menu timer
//  Global resource dependence :
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
LOCAL void StartMenuWinTimer(
                                  MMISMS_MAINMENU_WINDATA_T *win_data_ptr
                                  )
{
    

    if (0 == win_data_ptr->menu_win_timer_id)
    {
        win_data_ptr->menu_win_timer_id = MMK_CreateWinTimer(win_data_ptr->win_id, MMISMS_MENUWIN_TIME, TRUE);
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS StartMenuWinTimer: the key timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_22917_112_2_18_2_55_8_823,(uint8*)"");
    }
}

/*****************************1************************************************/
//  Description : stop Check if need update sms menu timer
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
LOCAL void StopMenuWinTimer(MMISMS_MAINMENU_WINDATA_T *win_data_ptr)
{
    if (0 < win_data_ptr->menu_win_timer_id)
    {
        MMK_StopTimer(win_data_ptr->menu_win_timer_id);
        win_data_ptr->menu_win_timer_id = 0;
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS StopMenuWinTimer: the key timer has stop!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_22934_112_2_18_2_55_8_824,(uint8*)"");
    }
}
/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void MainMenuWinCloseCtrlFunc (MMISMS_MAINMENU_WINDATA_T *win_data_ptr)
{
    if (win_data_ptr != PNULL)
    {
        SCI_FREE(win_data_ptr);
    }
}


#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : create sms main menu
//    Global resource dependence : none
//  Author: liming.deng
//    Note:    
/*****************************************************************************/
LOCAL void CreateSmsMainMenu(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  ctrl_id
                             )
{
    MMI_STRING_T tstring = {0};
    MMI_STRING_T nstring = {0};
    MMI_STRING_T num_status = {0};
    uint16 i = 0;
    wchar number_string[MMISMS_MEMSTATE_LABEL_MAX_LEN + 1] = {0};
    MMI_STRING_T menu_string = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    
    num_status.wstr_ptr = number_string;
    menu_string.wstr_ptr = SCI_ALLOC_APP( ( MMISMS_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar) );
    menu_string.wstr_len = MMISMS_MEMSTATE_LABEL_MAX_LEN;
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMISMS_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar)));
    
    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect,win_id,ctrl_id,GUIMENU_STYLE_THIRD);

    MMI_GetLabelTextByLang(TXT_COMMUNICATION_MESSAGE, &tstring);
#ifdef DPHONE_SUPPORT 
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#else
    GUIMENU_SetMenuTitle(&tstring,ctrl_id);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OPTION, TXT_NULL, STXT_RETURN);
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
    //chat menu
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMISMS_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar)));
    MMI_GetLabelTextByLang(TXT_SMS_CHAT_MODE, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_CHAT);
    MMIAPICOM_MergeTwoString(
                        &nstring,
                        &num_status,
                        &menu_string,
                        MMISMS_MEMSTATE_LABEL_MAX_LEN
                        );
    node_item.item_text_ptr = &menu_string;
#ifdef DPHONE_SUPPORT 
    node_item.select_icon_id = 0;
#else    
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_CHATBOX;
#endif //zss modify end 
    GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_CHAT, 0, &node_item, ctrl_id);
#endif

    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    MMI_GetLabelTextByLang(TXT_COMMON_WRITE_MESSAGE, &nstring);
    node_item.item_text_ptr = &nstring;
#ifdef DPHONE_SUPPORT 
    node_item.select_icon_id = 0;//IMAGE_NULL
#else    
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_WRITE;
#endif //zss modify end    
    GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG, 0, &node_item, ctrl_id);

    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMISMS_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar)));
    MMI_GetLabelTextByLang(TXT_INBOX, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_MT);
    MMIAPICOM_MergeTwoString(
                        &nstring,
                        &num_status,
                        &menu_string,
                        MMISMS_MEMSTATE_LABEL_MAX_LEN
                        );
    node_item.item_text_ptr = &menu_string;
#ifdef DPHONE_SUPPORT 
    node_item.select_icon_id = 0;
#else    
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_INBOX;
#endif //zss modify end 
    GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_INBOX, 0, &node_item, ctrl_id);

    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMISMS_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar)));
    MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_SENDFAIL);
    MMIAPICOM_MergeTwoString(
                        &nstring,
                        &num_status,
                        &menu_string,
                        MMISMS_MEMSTATE_LABEL_MAX_LEN
                        );
    node_item.item_text_ptr = &menu_string;
#ifdef DPHONE_SUPPORT 
    node_item.select_icon_id = 0;
#else    
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_OUTBOX;
#endif //zss modify end     
    GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_OUTBOX, 0, &node_item, ctrl_id);
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMISMS_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar)));
    MMI_GetLabelTextByLang(TXT_COMM_DRAFT, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_NOSEND);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMISMS_MEMSTATE_LABEL_MAX_LEN
        );
    node_item.item_text_ptr = &menu_string;
#ifdef DPHONE_SUPPORT 
    node_item.select_icon_id = 0;
#else    
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_DRAFTBOX;
#endif //zss modify end 
    GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_DRAFTBOX, 0, &node_item, ctrl_id);
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMISMS_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar)));
    MMI_GetLabelTextByLang(TXT_SMS_SENTBOX, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_SENDSUCC);
    MMIAPICOM_MergeTwoString(
                        &nstring,
                        &num_status,
                        &menu_string,
                        MMISMS_MEMSTATE_LABEL_MAX_LEN
                      );
    node_item.item_text_ptr = &menu_string;
#ifdef DPHONE_SUPPORT 
    node_item.select_icon_id = 0;
#else    
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_SENT;
#endif //zss modify end     
    GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_SENTBOX, 0, &node_item, ctrl_id);

#ifdef DPHONE_SUPPORT 
	SCI_MEMSET(&nstring, 0, sizeof(nstring));
	MMI_GetLabelTextByLang(TXT_SMS_FREQWORD, &nstring);
	node_item.item_text_ptr = &nstring;
	node_item.select_icon_id = 0;
	GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_FREQWORD, 0, &node_item, ctrl_id);

	SCI_MEMSET(&nstring, 0, sizeof(nstring));
	MMI_GetLabelTextByLang(STXT_MAIN_SETTING, &nstring);
	node_item.item_text_ptr = &nstring;
	node_item.select_icon_id = 0;
	GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_SETTING, 0, &node_item, ctrl_id);

	SCI_MEMSET(&nstring, 0, sizeof(nstring));
	MMI_GetLabelTextByLang(TXT_SMS_MESSAGESPACE, &nstring);
	node_item.item_text_ptr = &nstring;
	node_item.select_icon_id = 0;
	GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_MEMSTATE, 0, &node_item, ctrl_id);
    
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
	SCI_MEMSET(&nstring, 0, sizeof(nstring));
	MMI_GetLabelTextByLang(TXT_VOICEMAIL, &nstring);
	node_item.item_text_ptr = &nstring;
	node_item.select_icon_id = 0;
	GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_VOICEMAIL, 0, &node_item, ctrl_id);
#endif    
#else
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    MMI_GetLabelTextByLang(TXT_SMS_SECURITY, &nstring);

    node_item.item_text_ptr = &nstring;
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_SECURITY;
    GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_SECURITYBOX, 0, &node_item, ctrl_id);
#endif
    #ifdef MMI_PDA_SUPPORT

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT   
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
	MMI_GetLabelTextByLang(TXT_VOICEMAIL, &nstring);
	node_item.item_text_ptr = &nstring;
	node_item.select_icon_id = 0;
	GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_VOICEMAIL, 0, &node_item, ctrl_id);
#endif

#if defined(MMI_SMSCB_SUPPORT)
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
	MMI_GetLabelTextByLang(TXT_SMS_CB, &nstring);
	node_item.item_text_ptr = &nstring;
	node_item.select_icon_id = 0;
	GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_BROADCAST, 0, &node_item, ctrl_id);
#endif

	SCI_MEMSET(&nstring, 0, sizeof(nstring));
	MMI_GetLabelTextByLang(TXT_SMS_FREQWORD, &nstring);
	node_item.item_text_ptr = &nstring;
	node_item.select_icon_id = 0;
	GUIMENU_InsertNode(i++, MMISMS_MAIN_MENU_NODE_FREQWORD, 0, &node_item, ctrl_id);
    #endif
#endif
    if (PNULL != menu_string.wstr_ptr)
    {
        SCI_FREE(menu_string.wstr_ptr);
        menu_string.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//     Description : update sms main menu
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL void UpdateSmsMainMenu(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  ctrl_id                             
                             )
{
    MMI_STRING_T nstring = {0};
    MMI_STRING_T num_status = {0};
    uint16 i = 0;
    wchar number_string[MMISMS_MEMSTATE_LABEL_MAX_LEN + 1] = {0};
    MMI_STRING_T menu_string = {0};
    uint16 menu_wstr_len = (MMISMS_MEMSTATE_LABEL_MAX_LEN+1)*sizeof(wchar);
    
    num_status.wstr_ptr = number_string;
    menu_string.wstr_ptr = SCI_ALLOC_APP(menu_wstr_len);
    menu_string.wstr_len = MMISMS_MEMSTATE_LABEL_MAX_LEN;

#ifdef MMI_SMS_CHAT_SUPPORT
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_SMS_CHAT_MODE, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_CHAT);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMISMS_MEMSTATE_LABEL_MAX_LEN
        );
    GUIMENU_ReplaceNode(i++, MMISMS_MAIN_MENU_NODE_CHAT, 0, &menu_string, ctrl_id);
#endif

    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);    
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    MMI_GetLabelTextByLang(TXT_COMMON_WRITE_MESSAGE, &nstring);
    GUIMENU_ReplaceNode( i++, MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG, 0, &nstring, ctrl_id );
    
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_INBOX, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_MT);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMISMS_MEMSTATE_LABEL_MAX_LEN
        );
    GUIMENU_ReplaceNode(i++, MMISMS_MAIN_MENU_NODE_INBOX, 0, &menu_string, ctrl_id);
    
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_SENDFAIL);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMISMS_MEMSTATE_LABEL_MAX_LEN
        );
    GUIMENU_ReplaceNode(i++, MMISMS_MAIN_MENU_NODE_OUTBOX, 0, &menu_string, ctrl_id);
    
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_COMM_DRAFT, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_NOSEND);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMISMS_MEMSTATE_LABEL_MAX_LEN
        );
    GUIMENU_ReplaceNode(i++, MMISMS_MAIN_MENU_NODE_DRAFTBOX, 0, &menu_string, ctrl_id);
    
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_SMS_SENTBOX, &nstring);
    SetNumState2String(&num_status, MMISMS_BOX_SENDSUCC);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMISMS_MEMSTATE_LABEL_MAX_LEN
        );
    GUIMENU_ReplaceNode(i++, MMISMS_MAIN_MENU_NODE_SENTBOX, 0, &menu_string, ctrl_id);
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    MMI_GetLabelTextByLang(TXT_SMS_SECURITY, &nstring);

    GUIMENU_ReplaceNode(i++, MMISMS_MAIN_MENU_NODE_SECURITYBOX, 0, &nstring, ctrl_id);
#endif    
    if (PNULL != menu_string.wstr_ptr)
    {
        SCI_FREE(menu_string.wstr_ptr);
        menu_string.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//     Description : set sms number state to string 
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL void SetNumState2String(
                              MMI_STRING_T        *string_ptr,    //OUT:
                              MMISMS_BOX_TYPE_E box_type        //IN:
                              )
{
    int16 total_num = 0;
    int16 unread_num = 0;
    uint8 num_state[10] = {0};

    if ( MMISMS_BOX_MT == box_type)
    {
        total_num = MMISMS_GetBoxTotalNumberFromOrder(box_type);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        unread_num = MMISMS_GetTobeReadMessageCount(MN_DUAL_SYS_MAX) ;
#else
        unread_num = MMISMS_GetTobeReadMessageCount(MN_DUAL_SYS_1);
#endif
        unread_num += MMISMS_GetUndownloadMMSNumInOrder();

        string_ptr->wstr_len = sprintf((char *)num_state, "(%d/%d)", unread_num, total_num);
    }
#ifdef MMI_SMS_CHAT_SUPPORT    
    else if(MMISMS_BOX_CHAT == box_type)//会话模式
    {
        total_num = MMISMS_GetBoxTotalNumberFromOrder(box_type);

        unread_num = MMISMS_GetChatNodeTotalUnreadNum();

        string_ptr->wstr_len = sprintf((char *)num_state, "(%d/%d)", unread_num, total_num); 

        //SCI_TRACE_LOW:"SetNumState2String:chat total_num = %d,unread_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_22152_112_2_18_2_55_6_815,(uint8*)"dd",total_num,unread_num);
    }
#endif
    else
    {
        total_num = MMISMS_GetBoxTotalNumberFromOrder(box_type);
        if (box_type == MMISMS_BOX_SENDSUCC)
        {
            string_ptr->wstr_len = sprintf((char *)num_state, "(%d)", total_num);
        }
        else
        {
            string_ptr->wstr_len = sprintf((char *)num_state, "(%d)", total_num);
        }
    }

    MMI_STRNTOWSTR(string_ptr->wstr_ptr, string_ptr->wstr_len, (const uint8 *)num_state, string_ptr->wstr_len, string_ptr->wstr_len);
}
#endif

PUBLIC void MMISMS_EnterNewSmsNewWin(void)
{
    MMK_CreateWin((uint32 *)MMISMS_NEW_MSG_MENUWIN_TAB, PNULL); 
}

/*****************************************************************************/
//     Description : to handle the SMS menu window message
//    Global resource dependence : 
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsNewMsgMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM            param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_NEW_MSG_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T menu_id = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        if(MMIAPIUDISK_UdiskIsRun())
        {
            //grayed mms menu
            MMIAPICOM_EnableGrayed(win_id,MENU_NEW_MSG_OPT,MMISMS_MENU_NEW_MMS_ITEM_ID,TRUE);
        }

        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_GET_FOCUS:

        if(MMIAPIUDISK_UdiskIsRun())
        {
            //grayed mms menu
            MMIAPICOM_EnableGrayed(win_id,MENU_NEW_MSG_OPT,MMISMS_MENU_NEW_MMS_ITEM_ID,TRUE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_NEW_MSG_OPT,MMISMS_MENU_NEW_MMS_ITEM_ID,FALSE);
        }
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
        case MMISMS_MENU_NEW_SMS_ITEM_ID:
            MMISMS_OpenWriteWin();
            break;
            
        case MMISMS_MENU_NEW_MMS_ITEM_ID:
            MMIAPIMMS_EditNewMMS();
            break;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        case MMISMS_MENU_NEW_MESSAGE_ITEM_ID:
            MMISMS_OpenWriteWin(); 
            break;
#endif
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support

        case MMISMS_MENU_NEW_MAIL_ITEM_ID:

            mail_EditNewMail();
            break;
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
            
        default:
            //SCI_TRACE_LOW:"MMISMS: MMISMS HandleSmsNewMsgMenuWinMsg: menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_4354_112_2_18_2_54_29_723,(uint8*)"d",menu_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


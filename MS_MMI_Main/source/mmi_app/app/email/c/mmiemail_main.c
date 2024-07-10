/***************************************************************************************
** File Name:      mmiemail_main.c                                                     *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:                                                                        *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create mmiemail_main.c                              *
**                                                                                     *
***************************************************************************************/

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_pos.h"
#include "mmk_type.h"

#include "email_text.h"
#include "mmiemail_id.h"
#include "mmiemail_main.h"
#include "mmicom_string.h"
#include "mmiemail_menutable.h"
#include "mmiemail_setting.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_edit.h"
#include "mmiemail_box.h"
#include "mmiemail_def.h"
#include "mmipub.h"
//#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
//#endif
/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
#define MMIEMAIL_MEMSTATE_LABEL_MAX_LEN  30
HEmlSPMgr g_email_sp_handle = PNULL;
/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                            *
**------------------------------------------------------------------------------------*/
extern MMIEMAIL_TRANS_TYPE_E  g_email_trans_type; 
extern MMIEMAIL_EDIT_T *s_email_edit_p;
extern BOOLEAN g_is_email_by_pb;
extern EMAOPT_T *g_email_opt_ptr;
/**------------------------------------------------------------------------------------*
**                         STATIC DEFINITION                                           *
**------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                                     *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : create main menu window from setting window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void CreateMainmenuWinFromSetting(void);
/**************************************************************************************/
// Description : get all email number in boxs.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN BoxGetEmailNum(uint32 box_id, uint32 email_num);
/**************************************************************************************/
// Description : to handle email main menu window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleEmailMainmenuWinMsg(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             );
/**************************************************************************************/
// Description : to handle email main menu option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E  HandleEmailMainMenuOptWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 );
/**************************************************************************************/
// Description : create main menu.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void CreateMainMenuCtrl(
                              MMI_WIN_ID_T win_id,
                              MMI_CTRL_ID_T ctrl_id
                              );
/**************************************************************************************/
// Description : update main menu.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void UpdateMainMenuCtrl(
                              MMI_WIN_ID_T win_id, 
                              MMI_CTRL_ID_T ctrl_id
                              );
/**************************************************************************************/
// Description : update main menu's title.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void UpdateMainMenuCtrlTitle(
                                   MMI_WIN_ID_T win_id, 
                                   MMI_CTRL_ID_T ctrl_id
                                   );
/**************************************************************************************/
// Description : to set email number and status for boxs in main.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SetNumState2String(
                              MMI_STRING_T *string_ptr,    //OUT:
                              BOX_ID box_id
                              //MMIEMAIL_BOX_TYPE_E box_type  //IN:
                              );

/**************************************************************************************/
// Description : get create main menu window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_MAIN_MENU_WIN_TAB) = 
{
    //WIN_STATUSBAR,
        WIN_TITLE(TXT_EMAIL_TITLE),
        WIN_FUNC((uint32)HandleEmailMainmenuWinMsg),
        WIN_ID(EMAIL_MAIN_MENU_WIN_ID),
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
        END_WIN
};
/**************************************************************************************/
// Description : get create main menu options
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(MMIEMAIL_MAINMENU_OPT_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleEmailMainMenuOptWinMsg),
        WIN_ID(EMAIL_MAINMENU_OPT_WIN_ID),
        WIN_STYLE(WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EMAIL_MAIN_MENU_OPT, EMAIL_MAIN_MENU_OPT_CTRL_ID),
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
        END_WIN
};

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : create main menu window form setting.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void CreateMainmenuWinFromSetting(void)
{
    MMK_CreateWin((uint32 *)EMAIL_MAIN_MENU_WIN_TAB, PNULL);
}

/**************************************************************************************/
// Description : get create main menu window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_CreateMainmenuWin(void)
{
    MMK_CreateWin((uint32 *)EMAIL_MAIN_MENU_WIN_TAB, PNULL);
}
/**************************************************************************************/
// Description : get unread email number.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/

/**************************************************************************************/
// Description : to handle email main menu window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleEmailMainmenuWinMsg(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = EMAIL_MAIN_MENU_CTRL_ID;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    uint32 node_id = 0;
    uint32 netid = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateMainMenuCtrl(win_id, ctrl_id);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_LCD_SWITCH:
        if(MMITHEME_IsMainScreenLandscape())
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
#if defined(MMI_PDA_SUPPORT) 
    case MSG_APP_MENU:
#endif
        MMK_CreateWin((uint32 *)MMIEMAIL_MAINMENU_OPT_WIN_TAB, PNULL);
        break;
        
    case EVT_EMA_RECEIVE:
        EMA_UTILS_LOG_INFO(("HandleEmailMainMenuOptWinMsg EVT_EMA_INNER_INBOX_RECEIVE"));
        switch (g_email_trans_type)
        {
        case MMIEMAIL_TRANS_MSGHEAD:
            if (param)
            {
                netid= *((uint32*)param);
            }
            EMA_UTILS_TRACE("MMIEMAIL_SP_TRS_RetrieveActEmailHead, the net id is %d", netid);
            if (EMAIL_SUCCESS != MMIEMAIL_SP_TRS_RetrieveActEmailHead(EMA_GETTRANS_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, (char *)"Inbox", netid))
            {
                EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_RetrieveHead, MMIEMAIL_SP_TRS_RetrieveActEmailHead failed."));
                return recode;
            }
            break;
            
        case MMIEMAIL_TRANS_MSG:
            if (param)
            {
                netid= *((uint32*)param);
            }
            EMA_UTILS_TRACE("MMIEMAIL_SP_TRS_RetrieveActEmail, the net id is %d", netid);
            if (EMAIL_SUCCESS != MMIEMAIL_SP_TRS_RetrieveActEmail(EMA_GETTRANS_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, (char *)"Inbox", netid))
            {
                EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_RetrieveHeadBody, MMIEMAIL_SP_TRS_RetrieveActEmail failed."));
                return FALSE;
            }
            break;
        default:
            break;
        }
        //MMK_PostMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_INNER_INBOX_RECEIVE, 0, 0);
        break;
    
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT)
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch (node_id)
        {
        case MMIEMAIL_MAIN_MENU_NODE_WRITE_NEW_MSG:
            if(g_is_email_by_pb)
            {
                MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, (void*)g_email_opt_ptr);
            }
            else
            {
                MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, PNULL);
            }
            break;
        case MMIEMAIL_MAIN_MENU_NODE_INBOX:
            MMIEMAIL_CreateBoxWin(MMIEMAIL_BOX_FROM_INBOX, PNULL);
            break;
        case MMIEMAIL_MAIN_MENU_NODE_OUTBOX:
            MMIEMAIL_CreateBoxWin(MMIEMAIL_BOX_FROM_OUTBOX, PNULL);
            break;
        case MMIEMAIL_MAIN_MENU_NODE_DRAFTBOX:
            MMIEMAIL_CreateBoxWin(MMIEMAIL_BOX_FROM_DRAFT, PNULL);
            break;
        case MMIEMAIL_MAIN_MENU_NODE_SENTBOX:
            MMIEMAIL_CreateBoxWin(MMIEMAIL_BOX_FROM_SENTBOX, PNULL);
            break;
        default:
            break;
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
        if(MMIAPIEMAIL_GetIsPbSendEdit())
        {
            MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_edit_doc ={0};
            if(PNULL != s_email_edit_p)
            {   //fix 189723
                EMAIL_ATTACH_T  *add_file = PNULL;
                if (PNULL != s_email_edit_p->edit_doc_ptr)
                {
                    if(PNULL != s_email_edit_p->edit_doc_ptr->edit_to.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_to.wstr_ptr);
                    }
                    if(PNULL != s_email_edit_p->edit_doc_ptr->edit_cc.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_cc.wstr_ptr);
                    }
                    if(PNULL != s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_ptr);
                    }
                    if(PNULL != s_email_edit_p->edit_doc_ptr->edit_subject.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_subject.wstr_ptr);
                    }
                    if(PNULL != s_email_edit_p->edit_doc_ptr->edit_text.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_text.wstr_ptr);
                    }
                    
                    add_file = s_email_edit_p->edit_doc_ptr->attach_list_ptr;
                    while(PNULL != add_file)
                    {
                        s_email_edit_p->edit_doc_ptr->attach_list_ptr = add_file->next_ptr;
                        EMA_UTILS_FREEIF(PNULL, add_file->file_name_ptr);
                        EMA_UTILS_FREE(PNULL, add_file);
                        add_file = s_email_edit_p->edit_doc_ptr->attach_list_ptr;
                    }
                    EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr);
                    
                    s_email_edit_p->edit_doc_ptr = PNULL;
                }
                
                if (PNULL != s_email_edit_p->edit_back_ptr)
                {
                    if(PNULL != s_email_edit_p->edit_back_ptr->edit_to.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_to.wstr_ptr);
                    }
                    if(PNULL != s_email_edit_p->edit_back_ptr->edit_cc.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_cc.wstr_ptr);
                    }
                    if(PNULL != s_email_edit_p->edit_back_ptr->edit_bcc.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_bcc.wstr_ptr);
                    }
                    if(PNULL != s_email_edit_p->edit_back_ptr->edit_subject.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_subject.wstr_ptr);
                    }
                    if(PNULL != s_email_edit_p->edit_back_ptr->edit_text.wstr_ptr)
                    {
                        EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_text.wstr_ptr);
                    }
                    
                    add_file = s_email_edit_p->edit_back_ptr->attach_list_ptr;
                    while(PNULL != add_file)
                    {
                        s_email_edit_p->edit_back_ptr->attach_list_ptr = add_file->next_ptr;
                        EMA_UTILS_FREEIF(PNULL, add_file->file_name_ptr);
                        EMA_UTILS_FREE(PNULL, add_file);
                        add_file = s_email_edit_p->edit_back_ptr->attach_list_ptr;
                    }
                    EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr);
                    
                    s_email_edit_p->edit_back_ptr = PNULL;
                }
                
                MMIEMAIL_ClearEmailCache(&s_email_edit_p->data_cache);
                EMA_UTILS_FREEIF(PNULL, s_email_edit_p);
            }
            MMIAPIEMAIL_SetSendVcardDocument(&pb_edit_doc);
            MMIAPIEMAIL_SetIsPbSendEdit(FALSE);
            if(MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
            {
                MMK_CloseWin(EMAIL_EDIT_WIN_ID);
            }
            if(MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
            {
                MMK_CloseWin(EMAIL_MAIN_MENU_WIN_ID);
            }
            if(MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
            {
                MMK_CloseWin(EMAIL_SETTING_WIN_ID);
            }
        }
#endif
        break;
        
    case EVT_EMA_UPDATE_MAIN_NUM:
        UpdateMainMenuCtrl(win_id, ctrl_id);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_UPDATE_MAIN_TITLE:
        UpdateMainMenuCtrlTitle(win_id, ctrl_id);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/**************************************************************************************/
// Description : to handle email main menu window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleEmailMainMenuOptWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                                )
{
    MMI_RESULT_E        recode   = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id  = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, EMAIL_MAIN_MENU_OPT_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(EMAIL_MAIN_MENU_OPT_CTRL_ID, &group_id, &menu_id);
        switch (menu_id)
        {
        case EMAIL_MAINMENU_OPT_RECEIVE_SEND_ITEM_ID:
            MMIEMAIL_PublicProc_Retrieve();
            break;
        case EMAIL_MAINMENU_OPT_SEARCH_MAIL_ITEM_ID:
            break;
        case EMAIL_MAINMENU_OPT_NEWFOLER_ITEM_ID:
            break;
        case EMAIL_MAINMENU_OPT_MAIL_SYNC_ITEM_ID:
            break;
        case EMAIL_MAINMENU_OPT_ACCOUNT_SETTING_ITEM_ID:
            MMIEMAIL_EntryAccountSetting();
            break;
        case EMAIL_MAINMENU_OPT_SYS_SETTING_ITEM_ID:
            MMIEMAIL_EntrySystemSetting();
            break;
        case EMAIL_MAINMENU_OPT_HELP_ITEM_ID:
            break;
        default:
            break;
        }
        MMK_CloseWin(win_id);
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

/**************************************************************************************/
// Description : to set email number and status for boxs in main.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SetNumState2String(
                              MMI_STRING_T *string_ptr,    //OUT:
                              BOX_ID box_id
                              //MMIEMAIL_BOX_TYPE_E box_type        //IN:
                              )
{
   uint32 unread_num = 0;
   uint32 email_num = 0;
   uint8  num_state[10] = {0};
   
   if (TRUE != BoxGetEmailNum((uint16)box_id, (uint32)&email_num))
   {
      EMA_UTILS_LOG_ERROR(("SetNumState2String, BoxGetEmailNum failed!"));
   }
   
   if (1 == box_id)
   {
      if (0 != email_num)
      {
         if(EMAIL_SUCCESS != MMIEMAIL_InboxGetUnreadNum(&unread_num))
         {
            EMA_UTILS_LOG_ERROR(("SetNumState2String, MMIEMAIL_InboxGetUnreadNum failed!"));
         }
         
         string_ptr->wstr_len = EMA_UTILS_SPRINTF((char *)num_state, "(%d/%d)", unread_num, email_num);
      } 
      else
      {
         string_ptr->wstr_len = EMA_UTILS_SPRINTF((char *)num_state, "(0)");
      }
   } 
   else
   {
      if (0 != email_num)
      {
         string_ptr->wstr_len = EMA_UTILS_SPRINTF((char *)num_state, "(%d)", email_num);
      }
      else
      {
         string_ptr->wstr_len = EMA_UTILS_SPRINTF((char *)num_state, "(0)");
      }
   }
   
   MMI_STRNTOWSTR(string_ptr->wstr_ptr, string_ptr->wstr_len, (const uint8 *)num_state, string_ptr->wstr_len, string_ptr->wstr_len);
}

/**************************************************************************************/
// Description : create main menu.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void CreateMainMenuCtrl(
                              MMI_WIN_ID_T win_id, 
                              MMI_CTRL_ID_T ctrl_id
                              )
{
    MMI_STRING_T tstring = {0};
    MMI_STRING_T nstring = {0};
    MMI_STRING_T num_status = {0};
    uint16 index = 0;
    wchar number_string[MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1] = {0};
    MMI_STRING_T menu_string = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    HStoreMgr      store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
    EMASP_PRFITEM_VAL val = {0};
    int32          ret = 0;
    //wchar title_string[EMA_MAX_ADDR_LEN] = {0};
    
    do 
    {
        num_status.wstr_ptr = number_string;
        menu_string.wstr_ptr = EMA_UTILS_MALLOC(PNULL, ( MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar) );
        EMA_UTILS_BREAKNULL(menu_string.wstr_ptr, ret, "out of memory");
        menu_string.wstr_len = MMIEMAIL_MEMSTATE_LABEL_MAX_LEN;
        EMA_UTILS_MEMSET( menu_string.wstr_ptr, 0, ((MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1) * sizeof(wchar)));
        
        GUIMENU_CreatDynamic(&both_rect, win_id, ctrl_id, GUIMENU_STYLE_THIRD);
        
        EMA_UTILS_BREAKNULL(store_mgr, ret, "out of memory");
        if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr, EMA_SP_ACTIVE_ACCOUNT, EMASP_PRFITEM_ACCOUNT_NAME, &val))
        {
            EMA_UTILS_LOG_ERROR(("CreateMainMenuCtrl, get EMASP_PRFITEM_ACCOUNT_NAME failed."));
            return;
        }
        tstring.wstr_ptr = (wchar*)val.pData;
        tstring.wstr_len = EMA_UTILS_WSTRLEN(tstring.wstr_ptr);
        //MMI_GetLabelTextByLang(TXT_EMAIL_TITLE, &tstring);
        GUIMENU_SetMenuTitle(&tstring, ctrl_id);
        GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OPTION, TXT_NULL, STXT_RETURN);
        
        EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
        MMI_GetLabelTextByLang(TXT_WRITE_MAIL, &nstring);
        node_item.item_text_ptr = &nstring;
        node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_WRITE;
        GUIMENU_InsertNode(index++, MMIEMAIL_MAIN_MENU_NODE_WRITE_NEW_MSG, 0, &node_item, ctrl_id);
        
        EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
        EMA_UTILS_MEMSET(menu_string.wstr_ptr, 0, ((MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1) * sizeof(wchar)));
        MMI_GetLabelTextByLang(TXT_INBOX, &nstring);
        SetNumState2String(&num_status, index);
        MMIAPICOM_MergeTwoString(
            &nstring,
            &num_status,
            &menu_string,
            MMIEMAIL_MEMSTATE_LABEL_MAX_LEN
            );
        node_item.item_text_ptr = &menu_string;
        node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_INBOX;
        GUIMENU_InsertNode(index++, MMIEMAIL_MAIN_MENU_NODE_INBOX, 0, &node_item, ctrl_id);
        
        EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
        EMA_UTILS_MEMSET( menu_string.wstr_ptr, 0, ((MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar)));
        MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &nstring);
        SetNumState2String(&num_status, index);
        MMIAPICOM_MergeTwoString(
            &nstring,
            &num_status,
            &menu_string,
            MMIEMAIL_MEMSTATE_LABEL_MAX_LEN
            );
        node_item.item_text_ptr = &menu_string;
        node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_OUTBOX;
        GUIMENU_InsertNode(index++, MMIEMAIL_MAIN_MENU_NODE_OUTBOX, 0, &node_item, ctrl_id);
        
        EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
        EMA_UTILS_MEMSET( menu_string.wstr_ptr, 0, ((MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar)));
        MMI_GetLabelTextByLang(TXT_SENT, &nstring);
        SetNumState2String(&num_status, index);
        MMIAPICOM_MergeTwoString(
            &nstring,
            &num_status,
            &menu_string,
            MMIEMAIL_MEMSTATE_LABEL_MAX_LEN
            );
        node_item.item_text_ptr = &menu_string;
        node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_SENT;
        GUIMENU_InsertNode(index++, MMIEMAIL_MAIN_MENU_NODE_SENTBOX, 0, &node_item, ctrl_id);
        
        EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
        EMA_UTILS_MEMSET( menu_string.wstr_ptr, 0, ((MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar)));
        MMI_GetLabelTextByLang(TXT_COMM_DRAFT, &nstring);
        SetNumState2String(&num_status, index);
        MMIAPICOM_MergeTwoString(
            &nstring,
            &num_status,
            &menu_string,
            MMIEMAIL_MEMSTATE_LABEL_MAX_LEN
            );
        node_item.item_text_ptr = &menu_string;
        node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_DRAFTBOX;
        GUIMENU_InsertNode(index++, MMIEMAIL_MAIN_MENU_NODE_DRAFTBOX, 0, &node_item, ctrl_id);
        
        EMA_UTILS_FREE(PNULL, menu_string.wstr_ptr);
    } while (0);
}

/**************************************************************************************/
// Description : update main menu.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void UpdateMainMenuCtrl(
                              MMI_WIN_ID_T win_id, 
                              MMI_CTRL_ID_T ctrl_id
                              )
{
    MMI_STRING_T nstring = {0};
    MMI_STRING_T num_status = {0};
    uint16 index = 0;
    wchar number_string[MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1] = {0};
    MMI_STRING_T menu_string = {0};
    uint16 menu_wstr_len = (MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1)*sizeof(wchar);
    
    num_status.wstr_ptr = number_string;
    menu_string.wstr_ptr = EMA_UTILS_MALLOC(PNULL, menu_wstr_len);
    menu_string.wstr_len = MMIEMAIL_MEMSTATE_LABEL_MAX_LEN;
    EMA_UTILS_MEMSET( menu_string.wstr_ptr, 0, menu_wstr_len);
    
    EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
    MMI_GetLabelTextByLang(TXT_WRITE_MAIL, &nstring);
    GUIMENU_ReplaceNode(index++, MMIEMAIL_MAIN_MENU_NODE_WRITE_NEW_MSG, 0, &nstring, ctrl_id);
    
    EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
    EMA_UTILS_MEMSET(menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_INBOX, &nstring);
    SetNumState2String(&num_status, index);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMIEMAIL_MEMSTATE_LABEL_MAX_LEN
        );
    GUIMENU_ReplaceNode(index++, MMIEMAIL_MAIN_MENU_NODE_INBOX, 0, &menu_string, ctrl_id);
    
    EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
    EMA_UTILS_MEMSET( menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &nstring);
    SetNumState2String(&num_status, index);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMIEMAIL_MEMSTATE_LABEL_MAX_LEN
        );
    GUIMENU_ReplaceNode(index++, MMIEMAIL_MAIN_MENU_NODE_OUTBOX, 0, &menu_string, ctrl_id);
    
    EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
    EMA_UTILS_MEMSET( menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_SENT, &nstring);
    SetNumState2String(&num_status, index);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMIEMAIL_MEMSTATE_LABEL_MAX_LEN
        );
    GUIMENU_ReplaceNode(index++, MMIEMAIL_MAIN_MENU_NODE_SENTBOX, 0, &menu_string, ctrl_id);
    
    EMA_UTILS_MEMSET(&nstring, 0, sizeof(nstring));
    EMA_UTILS_MEMSET( menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_COMM_DRAFT, &nstring);
    SetNumState2String(&num_status, index);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMIEMAIL_MEMSTATE_LABEL_MAX_LEN
        );
    GUIMENU_ReplaceNode(index++, MMIEMAIL_MAIN_MENU_NODE_DRAFTBOX, 0, &menu_string, ctrl_id);
    
    EMA_UTILS_FREEIF(PNULL, menu_string.wstr_ptr);
}

/**************************************************************************************/
// Description : update main menu's title.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void UpdateMainMenuCtrlTitle(
                                   MMI_WIN_ID_T win_id, 
                                   MMI_CTRL_ID_T ctrl_id
                                   )
{
    MMI_STRING_T tstring = {0};
    MMI_STRING_T menu_string = {0};
    //GUIMENU_DYNA_ITEM_T node_item = {0};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    HStoreMgr      store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
    EMASP_PRFITEM_VAL val = {0};
    int32       ret = 0;
    
    do
    {
        menu_string.wstr_ptr = EMA_UTILS_MALLOC(PNULL, ( MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1 ) * sizeof(wchar));
        EMA_UTILS_BREAKNULL(menu_string.wstr_ptr, ret, "out of memory");
        menu_string.wstr_len = MMIEMAIL_MEMSTATE_LABEL_MAX_LEN;
        EMA_UTILS_MEMSET( menu_string.wstr_ptr, 0, ((MMIEMAIL_MEMSTATE_LABEL_MAX_LEN + 1) * sizeof(wchar)));
        
        GUIMENU_CreatDynamic(&both_rect, win_id, ctrl_id, GUIMENU_STYLE_THIRD);
        
        EMA_UTILS_BREAKNULL(store_mgr, ret, "out of memory");
        if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr, EMA_SP_ACTIVE_ACCOUNT, EMASP_PRFITEM_ACCOUNT_NAME, &val))
        {
            EMA_UTILS_LOG_ERROR(("UpdateMainMenuCtrlTitle, get EMASP_PRFITEM_ACCOUNT_NAME failed."));
            return;
        }
        tstring.wstr_ptr = (wchar*)val.pData;
        tstring.wstr_len = EMA_UTILS_WSTRLEN(tstring.wstr_ptr);
        GUIMENU_SetMenuTitle(&tstring, ctrl_id);
        GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OPTION, TXT_NULL, STXT_RETURN);
        
        EMA_UTILS_FREE(PNULL, menu_string.wstr_ptr);
    } while (0);
}
/**************************************************************************************/
// Description : get unread email number.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_InboxGetUnreadNum(uint32 *num)
{
    int result = EMAIL_ERROR;
    uint32 vector_num = 0;
    uint32 index = 0;
    EMAIL_VECTOR_T *vector_ptr = PNULL;
    EMAIL_SUBBOX_INFO_T *subbox_info_ptr = PNULL;
    
    do 
    {
        result = MMIEMAIL_VectorNew(&vector_ptr);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_InboxGetUnreadNum, create vector model failed.");
        
        result = MMIEMAIL_SP_Store_GetSpecBoxInfo(EMA_GETSTORE_HANDLE(g_email_sp_handle), 
            EMA_SP_ACTIVE_ACCOUNT, 
            EMASP_BOX_STATE_LOCAL,
            vector_ptr);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_InboxGetUnreadNum, MMIEMAIL_SP_Store_GetSpecBoxInfo");
        EMA_UTILS_BREAKNULL(vector_ptr, result, "MMIEMAIL_InboxGetUnreadNum, vector_ptr is PNULL.");
        vector_num = MMIEMAIL_VectorGetVectorNumber(vector_ptr);
        
        for (index = 0; index < vector_num; index++)
        {
            MMIEMAIL_VectorGetAt(vector_ptr, index, (void **)&subbox_info_ptr);
            if (PNULL != subbox_info_ptr)
            {
                if (EMA_UTILS_WSTRLEN(subbox_info_ptr->wsz_folder) > 0)
                {
                    MMIAPICOM_Wstrupper(subbox_info_ptr->wsz_folder);
                    if (0 == EMA_UTILS_WSTRCMP((wchar*)L"INBOX", subbox_info_ptr->wsz_folder)
                        && EMA_INBOX == subbox_info_ptr->box_id)
                    {
                        *num = subbox_info_ptr->num;
                        break;
                    }
                }
            }
        }
    } while(0);
    
    MMIEMAIL_VectorDelete(vector_ptr);
    
    if (EMAIL_SUCCESS != result)
    {
        return FALSE;
    }
    return TRUE;
}

/**************************************************************************************/
// Description : get all email number in boxs.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN BoxGetEmailNum(uint32 box_id, uint32 email_num)
{
   EMAIL_VECTOR_T *vector_ptr = PNULL;
   uint32         vector_num = 0; 
   
   if (EMAIL_SUCCESS != MMIEMAIL_SP_Store_GetBoxInfo(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, box_id, &vector_ptr))
   {
      EMA_UTILS_LOG_ERROR(("BoxGetEmailNum, MMIEMAIL_SP_Store_GetBoxInfo failed."));
      return FALSE;
   }
   if (PNULL != vector_ptr)
   {
      vector_num = MMIEMAIL_VectorGetVectorNumber(vector_ptr);
      if (vector_num > 0)
      {
         *(uint32*)email_num = vector_num;
      }
   }
   MMIEMAIL_VectorDelete(vector_ptr);
   return TRUE;
}



/*Edit by script, ignore 2 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527

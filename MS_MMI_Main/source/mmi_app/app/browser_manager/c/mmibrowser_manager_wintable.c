/*****************************************************************************
** File Name:      mmibrowser_manager_wintable.c                             *
** Author:          fen.xie                                                  *
** Date:           01/11/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_browser_manager_trc.h"
#include "window_parse.h"
//#include "sci_types.h" 
#include "mmi_module.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "mmibrowser_manager_wintable.h"
#include "mmibrowser_manager_id.h"
#include "browser_manager_image.h"
#include "browser_manager_text.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "guirichtext.h"
#include "guilistbox.h"
#include "mmibrowser_export.h"
#include "mmimms_text.h"
#include "mmibrowser_manager_menutable.h"
#include "mmicc_export.h"
#include "mmi_image.h"
#include "mmibt_text.h"
#include "mmi_text.h"
#ifdef  CMCC_UI_STYLE
#include "mmisms_text.h"
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL const MMI_TEXT_ID_T s_browser_type_text_id[MMIBROWSER_TYPE_MAX + 1] = 
{
#ifdef BROWSER_SUPPORT_DORADO    
    TXT_COMMON_BROWSER_DORADO,
#endif    
#ifdef OPERA_MINI_SUPPORT
    TXT_COMMON_BROWSER_OPERAMINI,
#endif    
#ifdef BROWSER_SUPPORT_NETFRONT
    TXT_COMMON_BROWSER_NF,
#endif    
#ifdef BROWSER_SUPPORT_WEBKIT
    TXT_COMMON_BROWSER_WK,
#endif    
    TXT_NULL,
};
#if 0//useless
LOCAL const MMI_IMAGE_ID_T s_browser_type_image_id[MMIBROWSER_TYPE_MAX + 1] = 
{
#ifdef OPERA_MINI_SUPPORT
    IMAGE_BM_OPERAMINI_ICON,
#endif    
#ifdef BROWSER_SUPPORT_NETFRONT
    IMAGE_BM_NF_ICON,
#endif    
#ifdef BROWSER_SUPPORT_DORADO    
    IMAGE_BM_DORADO_ICON,
#endif    
    IMAGE_NULL,
};
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern MMIBROWSER_MANAGER_INFO_T   g_browser_info;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#if 0
#if defined(WIFI_SUPPORT) || !defined(MMI_MULTI_SIM_SYS_SINGLE) 
/*****************************************************************************/
//  Description : handle message of select network window 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectNetworkWinMsg(
                MMI_WIN_ID_T	win_id, 
                MMI_MESSAGE_ID_E	msg_id, 
                DPARAM param
                );
#endif

/*****************************************************************************/
//  Description :  Handle open browser waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenBrowserWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );
#endif

#ifdef  CMCC_UI_STYLE
/*****************************************************************************/
//     Description : to handle the show message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenUrlQueryWinMsg(
                                       MMI_WIN_ID_T        win_id,        //IN:
                                       MMI_MESSAGE_ID_E msg_id,        //IN:
                                       DPARAM            param        //IN:
                                       );
#endif
/*****************************************************************************/
//  Description : handle message of select network window 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectBrowserWinMsg(
                MMI_WIN_ID_T	win_id, 
                MMI_MESSAGE_ID_E	msg_id, 
                DPARAM param
                );

#if defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT)
/*****************************************************************************/
//  Description : handle browser cc window message
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCCSelectWinMsg(
                                   MMI_WIN_ID_T win_id, 
                                   MMI_MESSAGE_ID_E msg_id,
                                   DPARAM param
                                   );
#endif
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBrowserWindow(
                MMI_WIN_ID_T    win_id, 
                MMI_MESSAGE_ID_E   msg_id, 
                DPARAM             param
                );

/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/
#if 0
//#if defined(WIFI_SUPPORT) || !defined (MMI_MULTI_SIM_SYS_SINGLE)
//the select network window
WINDOW_TABLE( MMIBM_SELECT_NETWORK_WIN_TAB ) = 
{
    WIN_TITLE(TXT_NETWORK_SELECT),
    WIN_FUNC((uint32)HandleSelectNetworkWinMsg),    
    WIN_ID(MMIBM_SELECT_NETWORK_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIBM_SELECT_NERWORK_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

WINDOW_TABLE( MMIBM_SELECT_BROWSER_WIN_TAB ) = 
{
    WIN_TITLE(TXT_BM_SELECT_BROWSER),
    WIN_FUNC((uint32)HandleSelectBrowserWinMsg),    
    WIN_ID(MMIBM_SELECT_BROWSER_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIBM_SELECT_BROWSER_FORM_CTRL_ID),
        CHILD_LIST_CTRL(TRUE, GUILIST_RADIOLIST_E, MMIBM_SELECT_BROWSER_LIST1_CTRL_ID, MMIBM_SELECT_BROWSER_FORM_CTRL_ID),
        CHILD_LIST_CTRL(TRUE, GUILIST_CHECKLIST_E, MMIBM_SELECT_BROWSER_LIST2_CTRL_ID, MMIBM_SELECT_BROWSER_FORM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMIBM_SET_BROWSER_WIN_TAB ) = 
{
    WIN_TITLE(TXT_BM_SET_SELECT_BROWSER),
    WIN_FUNC((uint32)HandleSetBrowserWindow),    
    WIN_ID(MMIBM_SET_BROWSER_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIBM_SET_BROWSER_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

#if defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT)
WINDOW_TABLE(MMIBM_CC_SELECTION_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleCCSelectWinMsg),
    WIN_TITLE(TXT_NULL),
    WIN_ID(MMIBM_CC_SELECTION_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void AppendSetBrowserListItem(MMI_CTRL_ID_T ctrl_id)
{
    uint16      cur_selection   =   0;
    MMIBROWSER_BROWSER_TYPE_E   default_browser_type = MMIBROWSER_TYPE_MAX;
    uint32  i = 0;
    MMI_STRING_T    str = {0};

    //add items to listbox
    MMIAPISET_AppendListItemByTextIdExt(TXT_COMMON_ASK_EVERYTIME, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        MMI_GetLabelTextByLang(s_browser_type_text_id[i], &str);
        MMIAPISET_AppendListItemByTextBuffer(str.wstr_ptr, str.wstr_len, i, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    }
    
    default_browser_type = MMIBM_GetDefaultBrowser();
    if (MMIBROWSER_TYPE_MAX <= default_browser_type)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = default_browser_type + 1;
    }
    
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);//set selected item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);//set current item
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void SetSelectBrowserWin(void)
{
    uint32  i = 0;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/

    GUIFORM_SetType(MMIBM_SELECT_BROWSER_FORM_CTRL_ID, GUIFORM_TYPE_TP);
    GUIFORM_PermitChildBorder(MMIBM_SELECT_BROWSER_FORM_CTRL_ID, FALSE);    
    GUILIST_PermitBorder(MMIBM_SELECT_BROWSER_LIST1_CTRL_ID, FALSE);
    GUILIST_PermitBorder(MMIBM_SELECT_BROWSER_LIST2_CTRL_ID, FALSE);

    GUILIST_SetMaxItem(MMIBM_SELECT_BROWSER_LIST1_CTRL_ID, MMIBROWSER_TYPE_MAX, FALSE);
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_BROWSER_MINI_SUPPORT
        item_data.item_content[0].item_data.image_id = IMAGE_SECMENU_ICON_BROWSER;//s_browser_type_image_id[i];
#else
        item_data.item_content[0].item_data.image_id = IMAGE_NULL;
#endif
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[1].item_data.text_id = s_browser_type_text_id[i];
                
        item_data.softkey_id[0] = TXT_NULL;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
        item_t.user_data = i;
        
        GUILIST_AppendItem( MMIBM_SELECT_BROWSER_LIST1_CTRL_ID, &item_t );      
    }
    GUILIST_SetCurItemIndex(MMIBM_SELECT_BROWSER_LIST1_CTRL_ID,0);
    GUILIST_SetSelectedItem(MMIBM_SELECT_BROWSER_LIST1_CTRL_ID,0, TRUE);

    GUILIST_SetMaxItem(MMIBM_SELECT_BROWSER_LIST2_CTRL_ID, 1, FALSE);
    GUILIST_SetMaxSelectedItem(MMIBM_SELECT_BROWSER_LIST2_CTRL_ID,1);
    MMIAPISET_AppendListItemByTextIdExt(TXT_CMSBRW_SETDEFAULT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,MMIBM_SELECT_BROWSER_LIST2_CTRL_ID,GUIITEM_STYLE_ONE_LINE_CHECK_EXT);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void EntrySelectBrowser(MMI_WIN_ID_T	win_id)
{
    uint32  cur_selection = 0;
    MMIBROWSER_BROWSER_TYPE_E   browser_type = MMIBROWSER_TYPE_MAX;
    MMIBROWSER_ENTRY_PARAM  *entry_param_ptr = PNULL;
    
    cur_selection = GUILIST_GetCurItemIndex(MMIBM_SELECT_BROWSER_LIST1_CTRL_ID);
    GUILIST_GetItemData(MMIBM_SELECT_BROWSER_LIST1_CTRL_ID, cur_selection, &browser_type);
        
    if (GUILIST_GetSelectedItemIndex(MMIBM_SELECT_BROWSER_LIST2_CTRL_ID,PNULL,1) > 0)
    {
        MMIBM_SetDefaultBrowser(browser_type);
    }

    if (browser_type < MMIBROWSER_TYPE_MAX) 
    {
        if ((g_browser_info.process_func[browser_type] != PNULL) 
            && (g_browser_info.process_func[browser_type]->Entry != PNULL))
        {
            entry_param_ptr = MMIBM_GetEntryParam();
            entry_param_ptr->browser_type = browser_type;
            g_browser_info.cur_browser_type = browser_type;
            g_browser_info.process_func[g_browser_info.cur_browser_type]->Entry(entry_param_ptr);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
    }
    else        
    {
        //ERROR
    }
    
    //MMK_CloseWin( win_id ); //bug136061
#ifdef BROWSER_SUPPORT_DORADO    
	if(MMIBROWSER_TYPE_DORADO != browser_type)
	{
		MMK_CloseWin( win_id );
	}
#endif
}

/*****************************************************************************/
//  Description : handle message of select network window 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectBrowserWinMsg(
                MMI_WIN_ID_T	win_id, 
                MMI_MESSAGE_ID_E	msg_id, 
                DPARAM param
                )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetSelectBrowserWin();
        MMK_SetAtvCtrl(win_id,MMIBM_SELECT_BROWSER_LIST1_CTRL_ID);
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
    {
        MMI_CTRL_ID_T ctrl_id = 0;

        if (PNULL != param)
        {
            ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

            if (ctrl_id == MMIBM_SELECT_BROWSER_LIST1_CTRL_ID)
            {
                EntrySelectBrowser(win_id);
            }
            else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
            }
            else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
            }
            else
            {
            }
        }
    }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif
            
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (MMIBM_SELECT_BROWSER_LIST1_CTRL_ID == MMK_GetActiveCtrlId(win_id))//entry browser
        {
            EntrySelectBrowser(win_id);
        }
        else
        {
            //do nothing
        }
        break;
            
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIBM_DestoryEntryParam();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBrowserWindow(
                MMI_WIN_ID_T    win_id, 
                MMI_MESSAGE_ID_E   msg_id, 
                DPARAM             param
                )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIBM_SET_BROWSER_LIST_CTRL_ID, (MMIBROWSER_TYPE_MAX + 1), FALSE );
        AppendSetBrowserListItem(MMIBM_SET_BROWSER_LIST_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMIBM_SET_BROWSER_LIST_CTRL_ID);
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_NOTIFY_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            uint32  cur_selection = 0;
            MMIBROWSER_BROWSER_TYPE_E   browser_type = MMIBROWSER_TYPE_MAX;
            
            cur_selection = GUILIST_GetCurItemIndex(MMIBM_SET_BROWSER_LIST_CTRL_ID);

            if (0 == cur_selection)//ask every time
            {
                MMIBM_SetDefaultBrowser(MMIBROWSER_TYPE_MAX);
            }
            else//choose one browser
            {
                GUILIST_GetItemData(MMIBM_SET_BROWSER_LIST_CTRL_ID, cur_selection, &browser_type);
                if (browser_type < MMIBROWSER_TYPE_MAX) 
                {
                    MMIBM_SetDefaultBrowser(browser_type);
                }
                else        
                {
                    //ERROR
                }
            
            }

            MMK_CloseWin( win_id );
        }        
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : open the browser type select window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_OpenBrowserSelectWin(void)
{
    MMK_CreateWin((uint32*)MMIBM_SELECT_BROWSER_WIN_TAB, PNULL);
}
/*****************************************************************************/
//  Description : close the browser select window
//  Global resource dependence : 
//  Author: jinju.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_CloseBrowserSelectWin(void)
{
	if(MMK_IsOpenWin(MMIBM_SELECT_BROWSER_WIN_ID))
	{
		MMK_CloseWin((uint32*)MMIBM_SELECT_BROWSER_WIN_ID);
	}
}
/*****************************************************************************/
//  Description : open the browser type setting window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_OpenSetBrowserWin(void)
{
    MMK_CreateWin((uint32*)MMIBM_SET_BROWSER_WIN_TAB, PNULL);
}
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
/*****************************************************************************/
//  Description : Create CC select win
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBM_OpenCCSelWin(MMIBM_CC_INFO_T *in_cc_info_ptr)
{
    MMIBM_CC_INFO_T *cc_info_ptr = NULL;
    uint16 num_len = 0;
    BOOLEAN result = FALSE;

    do
    {
        if (PNULL == in_cc_info_ptr || PNULL == in_cc_info_ptr->phone_num_ptr)
        {
            //SCI_TRACE_LOW:"[BM]MMIBM_OpenCCSelWin error1"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_WINTABLE_484_112_2_18_2_7_40_23,(uint8*)"");
            break;
        }
        
        if (!MMIAPICOM_IsValidNumberString((char *)in_cc_info_ptr->phone_num_ptr, strlen((char *)in_cc_info_ptr->phone_num_ptr)))//MS00223055
        {
            //SCI_TRACE_LOW:"[BM]MMIBM_OpenCCSelWin invalid number"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_WINTABLE_490_112_2_18_2_7_40_24,(uint8*)"");
    		//alert for ivalid phone nummber
             MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
            return FALSE;
        }
        
        cc_info_ptr = SCI_ALLOCA(sizeof(MMIBM_CC_INFO_T));
        if (PNULL == cc_info_ptr)
        {
            break;
        }
        SCI_MEMSET(cc_info_ptr, 0x00, sizeof(MMIBM_CC_INFO_T));
        cc_info_ptr->cc_option = in_cc_info_ptr->cc_option;

        num_len = strlen((char *)in_cc_info_ptr->phone_num_ptr);
        cc_info_ptr->phone_num_ptr = SCI_ALLOCA(num_len + 1);
        if (PNULL == cc_info_ptr->phone_num_ptr)
        {
            SCI_FREE(cc_info_ptr);
            break;
        }

        SCI_MEMSET(cc_info_ptr->phone_num_ptr, 0x00, num_len + 1);
        SCI_MEMCPY(cc_info_ptr->phone_num_ptr, in_cc_info_ptr->phone_num_ptr, num_len);

        //SCI_TRACE_LOW:"[BM]mmi cc enter selectwin  %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_WINTABLE_515_112_2_18_2_7_40_25,(uint8*)"s",(char*)cc_info_ptr->phone_num_ptr);
        MMK_CreateWin((uint32*)MMIBM_CC_SELECTION_WIN_TAB, (ADD_DATA)cc_info_ptr);
        result = TRUE;
    }
    while(0);
    
    if (!result)//MS00223055
    {
    	//alert for ivalid phone nummber
         MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle browser cc window message
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCCSelectWinMsg(
                                   MMI_WIN_ID_T win_id, 
                                   MMI_MESSAGE_ID_E msg_id,
                                   DPARAM param
                                   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIBM_CC_INFO_T *cc_info_ptr = PNULL;

    cc_info_ptr = (MMIBM_CC_INFO_T *)MMK_GetWinAddDataPtr(win_id);
    if (PNULL == cc_info_ptr)
    {
        MMK_CloseWin(win_id);
        return recode;
    }

    switch( msg_id )
    {       
    case MSG_OPEN_WINDOW:
    {
        MMI_HANDLE_T ctrl_handle = PNULL;
        GUIMENU_DYNA_ITEM_T node_item = {0};
        uint16 node_index = 0;
        MMI_STRING_T string = {0};
        GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

        GUIMENU_CreatDynamic(&both_rect, win_id, MMIBM_CC_SELECTION_MENU_CTRL_ID, GUIMENU_STYLE_THIRD);
        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIBM_CC_SELECTION_MENU_CTRL_ID);

        SCI_MEMSET(&string, 0x00, sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang(TXT_CALL, &string);
        GUIMENU_SetMenuTitle(&string, ctrl_handle);

        if ((cc_info_ptr->cc_option & MMIBM_CC_OPTION_VOICE) != 0)
        {

            SCI_MEMSET(&string, 0x00, sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_CALL_AUDIO, &string);
            node_item.item_text_ptr = &string;
            node_item.select_icon_id = 0;
            GUIMENU_InsertNode(node_index++, ID_BM_CC_VOICE, 0, &node_item, ctrl_handle);
        }

#ifdef VT_SUPPORT
        if ((cc_info_ptr->cc_option & MMIBM_CC_OPTION_VIDEO) != 0)
        {
            SCI_MEMSET(&string, 0x00, sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_CALL_VIDEO, &string);
            node_item.item_text_ptr = &string;
            node_item.select_icon_id = 0;
            GUIMENU_InsertNode(node_index++, ID_BM_CC_VIDEO, 0, &node_item, ctrl_handle);
        }
#endif

        if ((cc_info_ptr->cc_option & MMIBM_CC_OPTION_IP) != 0)
        {
            SCI_MEMSET(&string, 0x00, sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_CALL_IPCALL, &string);
            node_item.item_text_ptr = &string;
            node_item.select_icon_id = 0;
            GUIMENU_InsertNode(node_index++, ID_BM_CC_IP, 0, &node_item, ctrl_handle);
        }

        MMK_SetActiveCtrl(ctrl_handle, FALSE);
    }
        break;

    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    {
        uint32 call_type = CC_CALL_NORMAL_CALL;
        uint32 menu_id = 0;
        MMI_HANDLE_T ctrl_handle = PNULL;
        uint8  phone_len = 0;

        ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIBM_CC_SELECTION_MENU_CTRL_ID);
        menu_id = GUIMENU_GetCurNodeId(ctrl_handle);
        switch(menu_id) 
        {
        case ID_BM_CC_IP:
            call_type = CC_CALL_IP_CALL;
            break;
            
        case ID_BM_CC_VIDEO:
            call_type = CC_CALL_VIDEO_CALL;
            break;

        //case ID_BM_CC_VOICE:
        default:
            call_type = CC_CALL_NORMAL_CALL;
            break;
        }
        
        if (PNULL != cc_info_ptr->phone_num_ptr)
        {
            phone_len = strlen((char *)cc_info_ptr->phone_num_ptr);
        }

        if (MMIAPICOM_IsValidNumberString((char *)cc_info_ptr->phone_num_ptr, phone_len))//MS00223055
        {
            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX, cc_info_ptr->phone_num_ptr, phone_len, PNULL, PNULL, CC_CALL_SIM_MAX, call_type, PNULL);
        }
        else
        {
            //alert for invalid phone nummber
             MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        }

        MMK_CloseWin(win_id);
    }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != cc_info_ptr->phone_num_ptr)
        {
            SCI_FREE(cc_info_ptr->phone_num_ptr);
        }
        SCI_FREE(cc_info_ptr);
       break;

    default:
        break;
    }

    return recode;
}
#endif
#if 0
#if defined(WIFI_SUPPORT) || !defined(MMI_MULTI_SIM_SYS_SINGLE) 
/*****************************************************************************/
//  Description : append item to listbox 
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendSelectListItem(MMI_CTRL_ID_T ctrl_id)
{
#if defined(WIFI_SUPPORT)
    MMI_STRING_T sim_str = {0};
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
#endif    

#ifdef WIFI_SUPPORT
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX + 1, FALSE );
#else
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE );
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE 
    MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, MMI_DUAL_SYS_MAX, MMISET_APPEND_SYS_OK);
#else//only defined WIFI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MN_DUAL_SYS_1;

    MMI_GetLabelTextByLang(TXT_GPRS,&sim_str);
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = sim_str.wstr_len;
    
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;

    //set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
#endif

#ifdef WIFI_SUPPORT
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MMI_DUAL_SYS_MAX;//WIFI
    #ifdef WLAN_SUPPORT
        MMI_GetLabelTextByLang(TXT_WIFI,&sim_str);
    #else
        MMI_GetLabelTextByLang(TXT_WIFI,&sim_str);
    #endif
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = MAIN_GetIdleWifiIcon();
                                 

    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = sim_str.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;

    //set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
#endif

    GUILIST_SetOwnSofterKey(ctrl_id, TRUE);

    return TRUE;
}

/*****************************************************************************/
//  Description : handle message of select network window 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectNetworkWinMsg(
                MMI_WIN_ID_T	win_id, 
                MMI_MESSAGE_ID_E	msg_id, 
                DPARAM param
                )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint16          list_index = 0;
    uint32          item_data = 0;
    LOCAL BOOLEAN   s_is_selected = TRUE;
   
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef WIFI_SUPPORT
        GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_SIM_SEL,FALSE);
#else
        GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_NETWORK_SELECT,FALSE);
#endif
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
#endif
        MMK_SetAtvCtrl(MMIBM_SELECT_NETWORK_WIN_ID, MMIBM_SELECT_NERWORK_CTRL_ID);
        s_is_selected = FALSE;
        break;

    case MSG_FULL_PAINT:
        {
            uint16 cur_selection = 0;
            cur_selection = GUILIST_GetCurItemIndex(MMIBM_SELECT_NERWORK_CTRL_ID);
            AppendSelectListItem(MMIBM_SELECT_NERWORK_CTRL_ID);
            GUILIST_SetCurItemIndex(MMIBM_SELECT_NERWORK_CTRL_ID, cur_selection);
        }
        break;

#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    case MSG_GET_FOCUS:
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
        break;
#endif          
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        list_index = GUILIST_GetCurItemIndex(MMIBM_SELECT_NERWORK_CTRL_ID);

        if(GUILIST_IsCurrItemInvalid(MMIBM_SELECT_NERWORK_CTRL_ID))
        {
            break;
        }
        
        GUILIST_GetItemData(MMIBM_SELECT_NERWORK_CTRL_ID, list_index, &item_data);
        
        if( item_data < MMI_DUAL_SYS_MAX )
        {
            MMIBROWSER_SetIsUseWifi(FALSE);
            MMIBROWSER_SetSim((MN_DUAL_SYS_E)item_data);
        }    
        else//WIFI
        {
#ifdef WIFI_SUPPORT        
            MMIBROWSER_SetIsUseWifi(TRUE);
#else
            //SCI_TRACE_LOW:"[BM]HandleSelectNetworkWinMsg sys_state[%d] = MMI_DUAL_SYS_MAX"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_WINTABLE_813_112_2_18_2_7_41_26,(uint8*)"d", list_index);
#endif
        }
        
        //MMIBROWSER_Enter();
        s_is_selected = TRUE;
        MMK_CloseWin(win_id);//返回上一级
        
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:        
        MMK_CloseWin(win_id);//返回上一级
        break;

    case MSG_CLOSE_WINDOW:
        //select fail, need to destory something when MMIAPIBROWSER_Entry()
        if (!s_is_selected)
        {
            MMIBM_DestoryEntryParam();
        }
        break;
        
    default:        
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
#endif

/*****************************************************************************/
//  Description :  Handle open browser waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenBrowserWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
     
    //SCI_TRACE_LOW:"HandleOpenBrowserWaitingWin, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_WINTABLE_857_112_2_18_2_7_41_27,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {    
    case MSG_MMI_BM_OPEN_BROWSER_BEGIN:
        if ((g_browser_info.cur_browser_type >= MMIBROWSER_TYPE_MAX) 
            || (PNULL == g_browser_info.process_func[i]))
        {
            //ERROR
        }
        else        
        {
            g_browser_info.process_func[g_browser_info.cur_browser_type]->Entry(MMIBM_GetEntryParam());
        }

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIBM_DestoryEntryParam();
        break;

    case MSG_APP_RED:
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    //SCI_TRACE_LOW:"HandleOpenBrowserWaitingWin, msg_id = 0x%x end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_WINTABLE_887_112_2_18_2_7_41_28,(uint8*)"d", msg_id);

    return result;
}

/*****************************************************************************/
//  Description : open the MMIBM_WAITING_OPER_BROWSER_WIN
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_OpenBrowserWaitingWin(void)
{
    MMI_WIN_ID_T waiting_win_id = MMIBM_WAITING_OPER_BROWSER_WIN_ID;
    uint32       waiting_time_out = 0;
    
    MMIPUB_OpenAlertWinByTextId(&waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
        IMAGE_PUBWIN_WAIT, &waiting_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleOpenBrowserWaitingWin);

    MMK_PostMsg(waiting_win_id, MSG_MMI_BM_OPEN_BROWSER_BEGIN, PNULL, NULL);
}

/*****************************************************************************/
//  Description : open the network(sim or wlan) select window of browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBM_OpenNetworkSelect(void)
{  
    BOOLEAN result = TRUE;
    uint16  sim_ok = 0;
    uint32  sim_num = 0;
    MMIBROWSER_ENTRY_PARAM  *entry_param_ptr = PNULL;

    entry_param_ptr = MMIBM_GetEntryParam(void);

    if (MMIAPISET_GetFlyMode())//NEWMS00111359
    {
        MMIAPISET_AlertFlyMode();
        return FALSE;
    }
    
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);

#ifdef WIFI_SUPPORT
    if ((0 == sim_num) || is_use_wifi)
    {
        entry_param_ptr->is_use_wifi = TRUE;
        MMIBM_OpenBrowserWaitingWin();
    }
    else
    {
        MMK_CreateWin((uint32*)MMIBM_SELECT_NETWORK_WIN_TAB, PNULL);
    }
#else
    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        result = FALSE;
    }
    else if (1 == sim_num)
    {
        entry_param_ptr->dual_sys = (MN_DUAL_SYS_E)sim_ok;
        MMIBM_OpenBrowserWaitingWin();
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
        MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

        major_sim_type = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_WAP);

        if ((major_sim_type < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
            && MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1))/*lint !e656*/ 
            )
        {
            entry_param_ptr->dual_sys = (MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1));/*lint !e656*/ 
            MMIBM_OpenBrowserWaitingWin();
        }
        else
        {
            MMK_CreateWin((uint32*)MMIBM_SELECT_NETWORK_WIN_TAB, PNULL);
        }
    }
#endif
#endif

    return result;
}
#endif//useless

#ifdef  CMCC_UI_STYLE
/*****************************************************************************/
//     Description : to handle the show message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenUrlQueryWinMsg(
                                       MMI_WIN_ID_T        win_id,        //IN:
                                       MMI_MESSAGE_ID_E msg_id,        //IN:
                                       DPARAM            param        //IN:
                                       )
{
    MMI_RESULT_E is_ret = MMI_RESULT_FALSE;

    SCI_TRACE_LOW("HandleOpenUrlQueryWinMsg msg_id =%x", msg_id);

    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMIPUB_INFO_T  *win_info_ptr = PNULL;
            char           *url_ptr = PNULL;
            MMIBROWSER_ENTRY_PARAM entry_param = {0};

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                url_ptr  = (char *)win_info_ptr->user_data;
                if (PNULL != url_ptr)
                {
                    entry_param.type = MMIBROWSER_ACCESS_URL;
                    entry_param.dual_sys = MN_DUAL_SYS_MAX;
                    entry_param.url_ptr = url_ptr;
                    entry_param.is_alert = FALSE;
                    MMIAPIBROWSER_Entry(&entry_param);
                }
            }
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            char * url_ptr = PNULL;            
            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

            if (PNULL != win_info_ptr)
            {
                url_ptr  = (char *)win_info_ptr->user_data;
                if (PNULL != url_ptr)
                {
                    SCI_FREE(url_ptr);
                }
            }

            is_ret = MMI_RESULT_TRUE;
        }
        break;

    default:
        is_ret = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (is_ret);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBM_QueryOpenUrlWin(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr)
{
    uint16  temp_len = 0;
    char    *url_ptr = PNULL;

    if (entry_param_ptr != PNULL && (entry_param_ptr->url_ptr != PNULL) 
        && ((temp_len = SCI_STRLEN(entry_param_ptr->url_ptr)) > 0))
    {
        url_ptr = SCI_ALLOCA(temp_len + 1);
        if (url_ptr != PNULL)
        {
            SCI_MEMCPY(url_ptr, entry_param_ptr->url_ptr, (temp_len + 1));
        }
        else
        {
            return MMIBROWSER_FAIL;
        }

        MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_SMS_OPEN_LINK, IMAGE_PUBWIN_QUERY, PNULL, HandleOpenUrlQueryWinMsg, (uint32)url_ptr);

        return MMIBROWSER_SUCC;
    }

    return MMIBROWSER_FAIL;
}
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


/*Edit by script, ignore 5 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527

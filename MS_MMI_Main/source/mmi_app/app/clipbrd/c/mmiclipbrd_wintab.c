/*****************************************************************************
** File Name:      mmiclipbrd_wintab.c                                       *
** Author:         hua.fang                                                  *
** Date:           06/05/2010                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_app_clipbrd_trc.h"
#include "window_parse.h"
#include "guictrl_api.h"
#include "mmitheme_pos.h"
#include "mmiclipbrd_id.h"
#include "mmiclipbrd_menutable.h"
#include "mmiclipbrd_export.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : clipboard menu msg handler
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleClipboardMenuMsg(
                                          MMI_WIN_ID_T        win_id,   // [in]
                                          MMI_MESSAGE_ID_E    msg_id,   // [in]
                                          DPARAM              param     // [in]
                                          );

/*****************************************************************************/
//  Description : send clipboard menu msg to ctrl
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SendMenuMsgToCtrl(
                             MMI_HANDLE_T dst_ctrl_handle,  // [in]
                             MMI_MENU_ID_T menu_id          // [in]
                             );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
// clipboard menu window
WINDOW_TABLE( MMICLIPBRD_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleClipboardMenuMsg),
    WIN_ID(MMICLIPBRD_MENU_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CLIPBOARD, MMICLIPBRD_MENU_CTRL_ID),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : show clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T _MMICLIPBRD_ShowMenu(                                    // [ret]  return clipboard window handle
                                         const CLIPBRDMENU_DATA_T *data_ptr  // [in] data
                                         )
{
    MMI_HANDLE_T win_handle = 0;
    CLIPBRDMENU_DATA_T *clipboard_data_ptr = PNULL;

    if (PNULL == data_ptr)
    {
        return win_handle;
    }

    // alloc memory to store data which will be used when show menu.
    clipboard_data_ptr = (CLIPBRDMENU_DATA_T*)SCI_ALLOC_APP(sizeof(CLIPBRDMENU_DATA_T));
    *clipboard_data_ptr = *data_ptr;

    // the data is stored in window
    win_handle = MMK_CreateWin((uint32*)MMICLIPBRD_WIN_TAB, (ADD_DATA)clipboard_data_ptr);
    if (0 == win_handle)
    {
        SCI_FREE(clipboard_data_ptr);
    }

    return win_handle;
}

/*****************************************************************************/
//  Description : close clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN _MMICLIPBRD_CloseMenu(                       // [ret]
                                     MMI_HANDLE_T handle    // [in] clipboard window handle
                                     )
{
    return MMK_CloseWin(handle);
}

/*****************************************************************************/
//  Description : clipboard menu msg handler
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleClipboardMenuMsg(
                                          MMI_WIN_ID_T        win_id,   // [in]
                                          MMI_MESSAGE_ID_E    msg_id,   // [in]
                                          DPARAM              param     // [in]
                                          )
{
    CLIPBRDMENU_DATA_T  *clipboard_data_ptr = PNULL;
    MMI_RESULT_E        result_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    GUI_BOTH_RECT_T     both_rect = {0};
    MMI_HANDLE_T        dst_ctrl_handle = 0;
    IGUICTRL_T          *ctrl_ptr = PNULL;
    BOOLEAN             is_parent_handled = FALSE;
    GUIMENU_BUTTON_INFO_T   menu_button_info = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMICLIPBRD_MENU_CTRL_ID);

        menu_button_info.is_static = TRUE;
        menu_button_info.group_id = MENU_CLIPBOARD;
        menu_button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;

        GUIMENU_SetButtonStyle(MMICLIPBRD_MENU_CTRL_ID, &menu_button_info);

        clipboard_data_ptr = (CLIPBRDMENU_DATA_T*)MMK_GetWinAddDataPtr(win_id);
        if (PNULL == clipboard_data_ptr)     // data which is set at creating win
        {
            break;
        }

        if (MMITHEME_IsMainScreenLandscape())
        {
            both_rect.h_rect.left   = clipboard_data_ptr->start_point.x;
            both_rect.h_rect.top    = clipboard_data_ptr->start_point.y;
            both_rect.h_rect.right  = both_rect.h_rect.left + 1;
            both_rect.h_rect.bottom = both_rect.h_rect.top + 1;

            both_rect.v_rect.left   = clipboard_data_ptr->start_point.y;
            both_rect.v_rect.top    = clipboard_data_ptr->start_point.x;
            both_rect.v_rect.right  = both_rect.v_rect.left + 1;
            both_rect.v_rect.bottom = both_rect.v_rect.top + 1;
        }
        else
        {
            both_rect.v_rect.left   = clipboard_data_ptr->start_point.x;
            both_rect.v_rect.top    = clipboard_data_ptr->start_point.y;
            both_rect.v_rect.right  = both_rect.v_rect.left + 1;
            both_rect.v_rect.bottom = both_rect.v_rect.top + 1;

            both_rect.h_rect.left   = clipboard_data_ptr->start_point.y;
            both_rect.h_rect.top    = clipboard_data_ptr->start_point.x;
            both_rect.h_rect.right  = both_rect.h_rect.left + 1;
            both_rect.h_rect.bottom = both_rect.h_rect.top + 1;
        }
        GUIAPICTRL_SetBothRect(MMICLIPBRD_MENU_CTRL_ID,&both_rect);

        // disable cut item
        if (clipboard_data_ptr->is_cut_disabled)
        {
            GUIMENU_SetItemGrayed(
                    MMICLIPBRD_MENU_CTRL_ID,
                    MENU_CLIPBOARD,
                    CLIPBRD_MENU_CUT_ITEM_ID,
                    TRUE);
        }

        // disable copy item
        if (clipboard_data_ptr->is_copy_disabled)
        {
            GUIMENU_SetItemGrayed(
                    MMICLIPBRD_MENU_CTRL_ID,
                    MENU_CLIPBOARD,
                    CLIPBRD_MENU_COPY_ITEM_ID,
                    TRUE);
        }

        // disable paste item
        if (clipboard_data_ptr->is_paste_disabled)
        {
            GUIMENU_SetItemGrayed(
                    MMICLIPBRD_MENU_CTRL_ID,
                    MENU_CLIPBOARD,
                    CLIPBRD_MENU_PASTE_ITEM_ID,
                    TRUE);
        }

        // disable select all item
        if (clipboard_data_ptr->is_select_all_disabled)
        {
            GUIMENU_SetItemGrayed(
                    MMICLIPBRD_MENU_CTRL_ID,
                    MENU_CLIPBOARD,
                    CLIPBRD_MENU_SELECTALL_ITEM_ID,
                    TRUE);
        }

        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:     // when select one item in menu
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        clipboard_data_ptr = (CLIPBRDMENU_DATA_T*)MMK_GetWinAddDataPtr(win_id);
        if (PNULL != clipboard_data_ptr)     // data which is set at creating win
        {
            GUIMENU_GetId(MMICLIPBRD_MENU_CTRL_ID, &group_id, &menu_id);

            // send msg to dst ctrl
            SendMenuMsgToCtrl(clipboard_data_ptr->dst_ctrl_handle, menu_id);
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_NOTIFY_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;

    // @fix cr210977
    case MSG_APP_RED:
        result_code = MMI_RESULT_FALSE;             // 默认让MMK_HandlePublicKey处理

        clipboard_data_ptr = (CLIPBRDMENU_DATA_T*)MMK_GetWinAddDataPtr(win_id);
        if (PNULL != clipboard_data_ptr)    // data which is set at creating win
        {
            // 先保存一下dst_ctrl，因为MMK_CloseWin后会把clipboard_data清掉
            dst_ctrl_handle = clipboard_data_ptr->dst_ctrl_handle;

            // 如果是editbox，则需特殊处理一下，有些应用按RED键后不是直接回IDLE
            ctrl_ptr = MMK_GetCtrlPtr(dst_ctrl_handle);
            if (PNULL != ctrl_ptr
                && SPRD_GUI_EDITBOX_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr))
            {
                MMK_CloseWin(win_id);

                // 发给控件，如果控件不处理，会发给应用的
                is_parent_handled = MMK_SendMsg(dst_ctrl_handle, msg_id, param);
                if (is_parent_handled)
                {
                    result_code = MMI_RESULT_TRUE;
                }
            }
        }

        break;

	case MSG_CLOSE_WINDOW:
        // free the memory
        clipboard_data_ptr = (CLIPBRDMENU_DATA_T*)MMK_GetWinAddDataPtr(win_id);
        if (PNULL != clipboard_data_ptr)
        {
            // send message to tell clipboard menu is closed
            // 改成POST消息，因为目标控件要在MSG_NOTIFY_CLIPBRD_MENU_CLOSED消息
            // 里判断窗口是否有焦点，同步的话，调用IsFocusWin有问题
            MMK_PostMsg(clipboard_data_ptr->dst_ctrl_handle, MSG_NOTIFY_CLIPBRD_MENU_CLOSED, 0, 0);

            SCI_FREE(clipboard_data_ptr);
        }
        break;

    default:
        result_code = MMI_RESULT_FALSE;
        break;
    }

    return result_code;
}

/*****************************************************************************/
//  Description : send clipboard menu msg to ctrl
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SendMenuMsgToCtrl(
                             MMI_HANDLE_T dst_ctrl_handle,  // [in]
                             MMI_MENU_ID_T menu_id          // [in]
                             )
{
    MMI_MESSAGE_ID_E msg_id = 0;

    switch (menu_id)
    {
    case CLIPBRD_MENU_CUT_ITEM_ID:
        msg_id = MSG_NOTIFY_CLIPBRD_CUT;
        break;

    case CLIPBRD_MENU_COPY_ITEM_ID:
        msg_id = MSG_NOTIFY_CLIPBRD_COPY;
        break;

    case CLIPBRD_MENU_PASTE_ITEM_ID:
        msg_id = MSG_NOTIFY_CLIPBRD_PASTE;
        break;

    case CLIPBRD_MENU_SELECTALL_ITEM_ID:
        msg_id = MSG_NOTIFY_CLIPBRD_SELECTALL;
        break;

    default:
        //SCI_TRACE_LOW:"== SendMenuMsgToCtrl == msg id %d is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICLIPBRD_WINTAB_314_112_2_18_2_12_4_2,(uint8*)"d", menu_id);
        break;
    }

    if (0 != msg_id)
    {
        MMK_PostMsg(dst_ctrl_handle, msg_id, 0, 0);
    }
}
                         



/*Edit by script, ignore 1 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527

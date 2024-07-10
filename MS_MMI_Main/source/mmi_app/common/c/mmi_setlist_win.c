/*****************************************************************************
** File Name:      mmi_setlist_win.c                                         *
** Author:         xiaoqing.lu                                               *
** Date:           19/11/2010                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2010        xiaoqing.lu      Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_app_common_trc.h"
#include "window_parse.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmi_setlist_win.h"
#include "guisetlist.h"
#include "mmitheme_pos.h"
#include "guibutton.h"
#include "guires.h"
#include "mmidisplay_data.h"

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
//  Description : set title info
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SetPopWinTitle(
    MMI_WIN_ID_T win_id,
    MMI_OPEN_SETLIST_WIN_T *setlist_data_ptr
);

/*****************************************************************************/
//  Description : handle win msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetlistPopWinMsg(
                                          MMI_WIN_ID_T        win_id,   // [in]
                                          MMI_MESSAGE_ID_E    msg_id,   // [in]
                                          DPARAM              param     // [in]
                                          );

/*****************************************************************************/
//  Description : set dynamic List
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetPopWinListBox(
                            MMI_OPEN_SETLIST_WIN_T *data_ptr,
                            IGUICTRL_T *list_ctrl,
                            MMI_HANDLE_T ctrl_handle
                            );

/*****************************************************************************/
//  Description : 设置setlist选中条目
//  Global resource dependence :
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SetSetListSelectedItem(
                                  MMI_OPEN_SETLIST_WIN_T *setlist_data_ptr,
                                  uint16 sel_count,
                                  uint16 *item_array_ptr
                                  );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
// clipboard menu window
WINDOW_TABLE( MMISETLIST_CHILD_REDIO_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleSetlistPopWinMsg),
    WIN_ID(MMISETLIST_CHILD_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISETLIST_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

// clipboard menu window
WINDOW_TABLE( MMISETLIST_CHILD_CHECK_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleSetlistPopWinMsg),
    WIN_ID(MMISETLIST_CHILD_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISETLIST_LIST_CTRL_ID),
#if defined MMI_PDA_SUPPORT    
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : set title info
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SetPopWinTitle(
    MMI_WIN_ID_T win_id,
    MMI_OPEN_SETLIST_WIN_T *setlist_data_ptr
)
{
    MMI_STRING_T str_info = {0};
    uint32 str_len = 0;
    
    if(PNULL == setlist_data_ptr)
    {
        //SCI_TRACE_LOW:"SetPopWinTitle error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SETLIST_WIN_113_112_2_18_1_58_55_71,(uint8*)"");
        return;
    }

    // 获得title字符串长度
    GUISETLIST_CallBack(
        setlist_data_ptr->parant_ctrl_handle,
        PNULL,
        &str_len,
        MMI_SETLIST_GET_TITLE_STRLEN);
    
    // 获得title字符串的信息，并设置title控件
    if (str_len > 0)
    {
        str_info.wstr_len = str_len;
        
        str_info.wstr_ptr = SCI_ALLOC_APP(sizeof(wchar)*str_info.wstr_len);
        SCI_MEMSET(str_info.wstr_ptr, 0, sizeof(wchar)*str_info.wstr_len);
        
        GUISETLIST_CallBack(
            setlist_data_ptr->parant_ctrl_handle,
            PNULL,
            &str_info,
            MMI_SETLIST_GET_TITLE_STRDATA);
#ifdef MMI_PDA_SUPPORT
        MMIPUB_SetWinTitleText(win_id, &str_info, FALSE);
#else
		GUIWIN_SetTitleText(win_id,str_info.wstr_ptr,str_info.wstr_len,FALSE);
#endif
        
        SCI_FREE(str_info.wstr_ptr);
    }
    
    return;
}

/*****************************************************************************/
//  Description : 设置setlist选中条目
//  Global resource dependence :
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SetSetListSelectedItem(
    MMI_OPEN_SETLIST_WIN_T *setlist_data_ptr,
    uint16 sel_count,
    uint16 *item_array_ptr
)
{
    uint32 item_idx = 0;
    
    if (PNULL == setlist_data_ptr|| PNULL == item_array_ptr)
    {
        return;
    }

    GUISETLIST_ResetAllItemState(setlist_data_ptr->parant_ctrl_handle);
    
    for (item_idx = 0; item_idx < sel_count; item_idx++)
    {
        GUISETLIST_SetItemState(setlist_data_ptr->parant_ctrl_handle, item_array_ptr[item_idx], TRUE);
    }

    return;
}

/*****************************************************************************/
//  Description : handle win msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetlistPopWinMsg(
                                          MMI_WIN_ID_T        win_id,   // [in]
                                          MMI_MESSAGE_ID_E    msg_id,   // [in]
                                          DPARAM              param     // [in]
                                          )
{
    MMI_HANDLE_T            ctrl_handle       = 0;
    MMI_RESULT_E            ret_code          = MMI_RESULT_TRUE;
    MMI_OPEN_SETLIST_WIN_T  *setlist_data_ptr = PNULL;
    IGUICTRL_T              *list_ctrl        = PNULL;

    setlist_data_ptr = (MMI_OPEN_SETLIST_WIN_T*)MMK_GetWinAddDataPtr(win_id);

    list_ctrl = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(win_id, MMISETLIST_LIST_CTRL_ID);

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMISETLIST_LIST_CTRL_ID);

    if (PNULL == setlist_data_ptr)
    {
        //SCI_TRACE_LOW:"HandleSetlistPopWinMsg error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SETLIST_WIN_199_112_2_18_1_58_55_72,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL != list_ctrl && 0 != ctrl_handle)
        {
            MMK_SetAtvCtrl(win_id, ctrl_handle);
            SetPopWinTitle(win_id, setlist_data_ptr);

            SetPopWinListBox(setlist_data_ptr, list_ctrl, ctrl_handle);
        }
        
        break;

    case MSG_FULL_PAINT:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (GUILIST_RADIOLIST_E == setlist_data_ptr->list_type)
        {
            uint32 selected_item = 0;
           
            selected_item = GUILIST_GetCurItemIndex(ctrl_handle);
            MMK_SendMsg(setlist_data_ptr->parant_ctrl_handle, MSG_CTL_SETLIST_SET, (DPARAM)&selected_item);
            MMK_CloseWin(win_id);
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if (PNULL == list_ctrl) 
        {
            break;
        }

        if (GUILIST_CHECKLIST_E == setlist_data_ptr->list_type)
        {
            uint16 *item_array_ptr = PNULL;
            uint16 sel_count = 0;
            uint32 item_cnt = 0;
                        
            item_cnt = GUILIST_GetSelectedItemIndex(MMISETLIST_LIST_CTRL_ID, PNULL, 0);

            item_array_ptr = SCI_ALLOC_APP(sizeof(uint16) * item_cnt);

            SCI_MEMSET(item_array_ptr, 0, (sizeof(uint16) * item_cnt));
            
            sel_count = GUILIST_GetSelectedItemIndex(MMISETLIST_LIST_CTRL_ID,
                            item_array_ptr, 
                            item_cnt);

            SetSetListSelectedItem(setlist_data_ptr, sel_count, item_array_ptr);

            SCI_FREE(item_array_ptr);

            MMK_SendMsg(setlist_data_ptr->parant_ctrl_handle, MSG_CTL_SETLIST_SET, PNULL);
        }
        else if (GUILIST_RADIOLIST_E == setlist_data_ptr->list_type)
        {
            uint32 selected_item = 0;
           
            selected_item = GUILIST_GetCurItemIndex(ctrl_handle);

            MMK_SendMsg(setlist_data_ptr->parant_ctrl_handle, MSG_CTL_SETLIST_SET, (DPARAM)&selected_item);
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //if (PNULL != setlist_data_ptr)
        {
            SCI_FREE(setlist_data_ptr);
            setlist_data_ptr = PNULL;
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_NOTIFY_CANCEL:
    // case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;

    default:
        ret_code = MMI_RESULT_FALSE;
        break;
    }

    return ret_code;
}

/*****************************************************************************/
//  Description : set dynamic List
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SetPopWinListBox(
                            MMI_OPEN_SETLIST_WIN_T *data_ptr,
                            IGUICTRL_T *list_ctrl,
                            MMI_HANDLE_T ctrl_handle
                            )
{   
    if(PNULL == data_ptr || PNULL == list_ctrl)
    {
        //SCI_TRACE_LOW:"SetPopWinListBox input param is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SETLIST_WIN_308_112_2_18_1_58_55_73,(uint8*)"");
        return;
    }
    
#ifdef MMI_PDA_SUPPORT
    GUISETLIST_SetCtrlState(data_ptr->parant_ctrl_handle, GUISETLIST_STATE_OPEN_FULLWIN, FALSE);
#else
    GUISETLIST_SetCtrlState(data_ptr->parant_ctrl_handle, GUISETLIST_STATE_OPEN_FULLWIN, TRUE);
#endif    

    // 为list申请内存，设置list最大条目
    GUISETLIST_CallBack(
        data_ptr->parant_ctrl_handle,
        list_ctrl,
        PNULL,
        MMI_SETLIST_ALLOC);

    if (GUILIST_CHECKLIST_E == data_ptr->list_type)
    {
        GUILIST_SetMaxSelectedItem(ctrl_handle, data_ptr->check_allow_max_num);
    }
    // append list item
    GUISETLIST_CallBack(
        data_ptr->parant_ctrl_handle,
        list_ctrl,
        PNULL,
        MMI_SETLIST_APPEND_ITEM);

    if (GUILIST_RADIOLIST_E == data_ptr->list_type)
    {
        // 设置list当前和选中item
        GUILIST_SetSelectedItem(ctrl_handle, data_ptr->cur_item_index, TRUE);
        GUILIST_SetCurItemIndex(ctrl_handle, data_ptr->cur_item_index);
    }

    return;
}
        
/*****************************************************************************/
//  Description : 打开setlist窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMISETLIST_CreateWin(
                                 MMI_OPEN_SETLIST_WIN_T *data_ptr
                                 )
{
    MMI_OPEN_SETLIST_WIN_T *setlist_data_ptr = PNULL;

    // 首先关闭
    MMK_CloseWin(MMISETLIST_CHILD_WIN_ID);

    if (PNULL != data_ptr)
    {
        setlist_data_ptr = (MMI_OPEN_SETLIST_WIN_T*)SCI_ALLOC_APP(sizeof(MMI_OPEN_SETLIST_WIN_T));
        *setlist_data_ptr = *data_ptr;
        
        setlist_data_ptr->list_font  = MMI_DEFAULT_TEXT_FONT;
        setlist_data_ptr->font_color = MMI_BLACK_COLOR;

        if (GUILIST_CHECKLIST_E == setlist_data_ptr->list_type)
        {
            MMK_CreatePubListWin((uint32*)MMISETLIST_CHILD_CHECK_WIN_TAB, (ADD_DATA)setlist_data_ptr);
        }
        else if (GUILIST_RADIOLIST_E == setlist_data_ptr->list_type)
        {
            MMK_CreatePubListWin((uint32*)MMISETLIST_CHILD_REDIO_WIN_TAB, (ADD_DATA)setlist_data_ptr);
        }
    }
    
    return;
}




/*Edit by script, ignore 1 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527

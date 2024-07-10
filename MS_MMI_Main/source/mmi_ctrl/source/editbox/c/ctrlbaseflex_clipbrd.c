/*****************************************************************************
** File Name:      ctrlbaseflex_clipbrd.c                                    *
** Author:         hua.fang                                                  *
** Date:           09/10/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012        hua.fang          Create                                   *
******************************************************************************/

#ifdef GUIF_CLIPBOARD

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "ctrlbaseflex.h"
#include "mmi_gui_trc.h"

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
//  Description : create slider
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CreateEditSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : start slider timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartSliderTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*******************************************************************************************/
//  Description : whether support clipbrd
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN BASEFLEX_IsSupportClipbrd (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
)
{
    BOOLEAN         is_support_clipbrd = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;

    if (baseflex_ctrl_ptr->is_clipbrd_support && base_ctrl_ptr->is_active)
    {
        is_support_clipbrd = TRUE;
    }

    return is_support_clipbrd;
}

/*****************************************************************************/
//  Description : display clip board menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_DisplayClipboardMenu (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUI_POINT_T             *point_ptr      //in:
)
{
    CLIPBRDMENU_DATA_T  clip_brd_data = {0};
    GUI_POINT_T         clipbrd_pt = {0};
    GUI_RECT_T          cursor_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (0 == baseflex_ctrl_ptr->clipbrd_win_handle
            && BASEFLEX_IsSupportClipbrd (baseflex_ctrl_ptr))
    {
        if (MMITHEME_DRAGMODE_DEFAULT != baseflex_ctrl_ptr->flex_theme.drag_mode)
        {
            // 重新显示光标
            if (!baseedit_ctrl_ptr->cursor.is_display)
            {
                //set display cursor
                baseedit_ctrl_ptr->cursor.is_display    = TRUE;
                baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

                //display cursor
                BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);
            }
        }

        if (PNULL == point_ptr)
        {
            BASEEDIT_GetCursorRect (baseedit_ctrl_ptr, PNULL, &cursor_rect);

            clipbrd_pt.x = cursor_rect.left;
            clipbrd_pt.y = cursor_rect.top;
        }
        else
        {
            clipbrd_pt = *point_ptr;
        }

        if (MMITHEME_DRAGMODE_DEFAULT != baseflex_ctrl_ptr->flex_theme.drag_mode)
        {
            clipbrd_pt.y = (int16) (clipbrd_pt.y - MMITHEME_GetClipbrdHeight());
        }

        //set data
        clip_brd_data.dst_ctrl_handle   = base_ctrl_ptr->handle;
        clip_brd_data.start_point       = clipbrd_pt;

        // 拖动模式下，不显示拷贝、剪切
        if (GUIEDIT_DRAGSTATE_DRAG == baseflex_ctrl_ptr->drag_state)
        {
            clip_brd_data.is_cut_disabled  = TRUE;
            clip_brd_data.is_copy_disabled = TRUE;

            if (0 == baseedit_ctrl_ptr->str_len)
            {
                clip_brd_data.is_select_disabled = TRUE;
                clip_brd_data.is_select_all_disabled = TRUE;
            }
        }
        else
        {
            //no highlight
            if ((baseflex_ctrl_ptr->is_disable_cutcopy) ||
                (baseedit_ctrl_ptr->highlight.start_pos == baseedit_ctrl_ptr->highlight.end_pos))
            {
                clip_brd_data.is_cut_disabled  = TRUE;
                clip_brd_data.is_copy_disabled = TRUE;
            }

            clip_brd_data.is_select_disabled = TRUE;

            // 在选字模式下，也不需要全选功能
            if (0 == baseedit_ctrl_ptr->str_len ||
                GUIEDIT_DRAGSTATE_SELECT == baseflex_ctrl_ptr->drag_state)
            {
                clip_brd_data.is_select_all_disabled = TRUE;
            }
        }

        clip_brd_data.is_paste_disabled = (BOOLEAN) (!MMITHEME_IsClipbrdAvailable (CLIPBRD_FORMAT_TEXT));

        //display clip board menu
        baseflex_ctrl_ptr->is_show_clipbrd = TRUE;
        baseflex_ctrl_ptr->clipbrd_win_handle = MMITHEME_ShowClipbrdMenu (&clip_brd_data);
    }
}

/*****************************************************************************/
//  Description : close clip board menu
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_CloseClipboardMenu (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
)
{
    if (0 != baseflex_ctrl_ptr->clipbrd_win_handle)
    {
        MMITHEME_CloseClipbrdMenu (baseflex_ctrl_ptr->clipbrd_win_handle);
    }
}

/*****************************************************************************/
//  Description : handle edit copy msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditCopyMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    uint16      highlight_min_pos = 0;
    uint16      highlight_len = 0;
    wchar       *data_ptr = PNULL;
    uint32      data_size = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get highlight
    if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
    {
        //get highlight string
        highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
        data_ptr = baseedit_ctrl_ptr->str_ptr + highlight_min_pos;

        //get highlight string len
        highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

        //set data size
        data_size = (uint32) (highlight_len * sizeof (wchar));

        //copy data
        MMITHEME_SetClipbrdData (CLIPBRD_FORMAT_TEXT,
                                 data_ptr,
                                 data_size);
    }

    // 恢复到NORMAL状态
    BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL_RESET);
}

/*****************************************************************************/
//  Description : handle edit cut msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditCutMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    uint16              highlight_min_pos = 0;
    uint16              highlight_len = 0;
    wchar               *data_ptr = PNULL;
    uint32              data_size = 0;
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get highlight
    if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
    {
        //get highlight string
        highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
        data_ptr = baseedit_ctrl_ptr->str_ptr + highlight_min_pos;

        //get highlight string len
        highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

        //set data size
        data_size = (uint32) (highlight_len * sizeof (wchar));

        //cut data
        if (CLIPBRD_RESULT_NOERROR == MMITHEME_SetClipbrdData (CLIPBRD_FORMAT_TEXT, data_ptr, data_size))
        {
            BASEFLEX_DelString (baseflex_ctrl_ptr, &update_info);
        }
    }

    // 恢复到NORMAL状态
    BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
}

/*****************************************************************************/
//  Description : handle edit paste msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditPasetMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    wchar               *data_ptr = PNULL;
    uint16              data_len = 0;
    uint32              data_size = 0;
    CLIPBRD_RESULT_E    result = CLIPBRD_RESULT_NOERROR;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;

    //get paste string
    result = MMITHEME_GetClipbrdData (CLIPBRD_FORMAT_TEXT, &data_ptr, &data_size);

    if ((CLIPBRD_RESULT_NOERROR == result) && (0 < data_size))
    {
        data_len = (uint16) (data_size / sizeof (wchar));

        //insert string
        CTRLBASEFLEX_InsertString (
                base_ctrl_ptr->handle,
                data_ptr,
                data_len);
    }

    // 恢复到NORMAL状态
    BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
}

/*****************************************************************************/
//  Description : handle edit select all msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditSelectAllMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    GUI_POINT_T     clipbrd_pt = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //set highlight
    baseedit_ctrl_ptr->highlight.is_underline = FALSE;
    VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, baseedit_ctrl_ptr->str_len, SETHIGHLIGHT_BOTH);

    //set cursor
    VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, baseedit_ctrl_ptr->str_len);
    baseedit_ctrl_ptr->cursor.x_coordinate  = GUIEDIT_CURSOR_INVALID_COORDINATE;

    //update display
    VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, TRUE);

    if (MMITHEME_DRAGMODE_DEFAULT != baseflex_ctrl_ptr->flex_theme.drag_mode)
    {
        BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_SELECT);

        clipbrd_pt.x = base_ctrl_ptr->display_rect.left;
        clipbrd_pt.y = base_ctrl_ptr->display_rect.top;

        BASEFLEX_DisplayClipboardMenu (baseflex_ctrl_ptr, &clipbrd_pt);
    }
}

/*****************************************************************************/
//  Description : 处理选择消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleSelectClipbrdMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
)
{
    uint16 cursor_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (baseedit_ctrl_ptr->str_len > 0)
    {
        if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
        {
            cursor_pos = baseedit_ctrl_ptr->highlight.start_pos;
        }
        else if (baseedit_ctrl_ptr->cursor.cur_pos == baseedit_ctrl_ptr->str_len)
        {
            cursor_pos = (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 1);
        }
        else
        {
            cursor_pos = baseedit_ctrl_ptr->cursor.cur_pos;
        }

        //reset date cursor not display
        BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);

        baseedit_ctrl_ptr->cursor.x_coordinate  = GUIEDIT_CURSOR_INVALID_COORDINATE;

        //send move cursor msg
        BASEEDIT_NotifyCursorMsg (baseedit_ctrl_ptr);

        //reset highlight not display
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

        VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr,
                                  cursor_pos,
                                  (uint16) (cursor_pos + 1),
                                  SETHIGHLIGHT_BOTH);

        // cursor will be between min_pos and max_pos while moving upword,
        // so check cursor and make it at the beginning of the highlight.
        // must call after GUIEDIT_SetHighlight, otherwise it will be error.
        BASEFLEX_CheckCursor (baseflex_ctrl_ptr);

        //display highlight
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

        ////set display cursor
        //edit_ctrl_ptr->cursor.is_display    = TRUE;
        //edit_ctrl_ptr->cursor.is_rect_valid = FALSE;

        ////display cursor
        //GUIEDIT_DisplayCursor(edit_ctrl_ptr);

        BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_SELECT);

        // 显示剪贴板
        BASEFLEX_DisplayClipboardMenu (baseflex_ctrl_ptr, PNULL);
    }
}

/*****************************************************************************/
//  Description : destroy slider
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_DestroyEditSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
)
{
    int32 i = 0;

    // 清除滑块
    for (i = 0; i < MMITHEME_SLIDER_TYPE_MAX; i++)
    {
        if (0 != baseflex_ctrl_ptr->slider[i])
        {
            MMITHEME_DestroySlider (baseflex_ctrl_ptr->slider[i]);
        }
    }
}

/*****************************************************************************/
//  Description : 改变滑块状态
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_ChangeDragState (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUIEDIT_DRAGSTATE_E     drag_state
)
{
    GUIEDIT_DRAGSTATE_E old_drag_state = GUIEDIT_DRAGSTATE_NORMAL;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (MMITHEME_DRAGMODE_DEFAULT != baseflex_ctrl_ptr->flex_theme.drag_mode &&
        BASEFLEX_IsSupportClipbrd (baseflex_ctrl_ptr))
    {
        if (GUIEDIT_DRAGSTATE_NORMAL == drag_state ||
            GUIEDIT_DRAGSTATE_NORMAL_RESET == drag_state)
        {
            old_drag_state = baseflex_ctrl_ptr->drag_state;

            baseflex_ctrl_ptr->drag_state = GUIEDIT_DRAGSTATE_NORMAL;
            baseedit_ctrl_ptr->cursor.is_hide = FALSE;

            // 需要强制设置光标，去掉高亮的情况
            if (GUIEDIT_DRAGSTATE_NORMAL_RESET == drag_state)
            {
                if (MMK_IsFocusWin (base_ctrl_ptr->win_handle))
                {
                    //reset date cursor not display
                    BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

                    baseedit_ctrl_ptr->cursor.x_coordinate  = GUIEDIT_CURSOR_INVALID_COORDINATE;

                    //reset highlight not display
                    BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);
                }

                //set no highlight
                BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

                BASEFLEX_CheckCursor (baseflex_ctrl_ptr);
            }

            // 这里，如果从选择状态过来的，需要显示下光标
            if ((GUIEDIT_DRAGSTATE_SELECT == old_drag_state
                    || GUIEDIT_DRAGSTATE_NORMAL_RESET == drag_state)
                && MMK_IsFocusWin (base_ctrl_ptr->win_handle))
            {
                //set display cursor
                baseedit_ctrl_ptr->cursor.is_display    = TRUE;
                baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

                //display cursor
                BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);
            }
        }
        else if (GUIEDIT_DRAGSTATE_DRAG == drag_state)
        {
            baseflex_ctrl_ptr->drag_state = GUIEDIT_DRAGSTATE_DRAG;
            baseedit_ctrl_ptr->cursor.is_hide = FALSE;
        }
        else if (GUIEDIT_DRAGSTATE_SELECT == drag_state)
        {
            baseflex_ctrl_ptr->drag_state = GUIEDIT_DRAGSTATE_SELECT;
            baseedit_ctrl_ptr->cursor.is_hide = TRUE;
        }

        BASEFLEX_StopSliderTimer (baseflex_ctrl_ptr);

        BASEFLEX_ShowSlider (baseflex_ctrl_ptr);

        if (GUIEDIT_DRAGSTATE_DRAG == baseflex_ctrl_ptr->drag_state ||
            GUIEDIT_DRAGSTATE_SELECT == baseflex_ctrl_ptr->drag_state)
        {
            StartSliderTimer (baseflex_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : 显示滑块
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_ShowSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    if (MMITHEME_DRAGMODE_DEFAULT != baseflex_ctrl_ptr->flex_theme.drag_mode &&
        BASEFLEX_IsSupportClipbrd (baseflex_ctrl_ptr))
    {
        // 如果没创建，创建之
        if (0 == baseflex_ctrl_ptr->slider[0])
        {
            CreateEditSlider (baseflex_ctrl_ptr);
        }

        if (GUIEDIT_DRAGSTATE_DRAG == baseflex_ctrl_ptr->drag_state)
        {
            MMITHEME_ShowSlider (baseflex_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_BLOCK], 0, 0);
            MMITHEME_HideSlider (baseflex_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_LEFT_SELECTOR]);
            MMITHEME_HideSlider (baseflex_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR]);
        }
        else if (GUIEDIT_DRAGSTATE_SELECT == baseflex_ctrl_ptr->drag_state)
        {
            MMITHEME_HideSlider (baseflex_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_BLOCK]);
            MMITHEME_ShowSlider (baseflex_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_LEFT_SELECTOR], 0, 0);
            MMITHEME_ShowSlider (baseflex_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR], 0, 0);
        }
        else
        {
            MMITHEME_HideSlider (baseflex_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_BLOCK]);
            MMITHEME_HideSlider (baseflex_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_LEFT_SELECTOR]);
            MMITHEME_HideSlider (baseflex_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR]);
        }
    }
}

/*****************************************************************************/
//  Description : 开始滑块拖动
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleBeginSlider (
    CTRLBASEFLEX_OBJ_T *baseflex_ctrl_ptr,
    DPARAM             param
)
{
    BOOLEAN result = TRUE;
    int32  slider_type = (int32) param;

    BASEFLEX_StopSliderTimer (baseflex_ctrl_ptr);

    switch (slider_type)
    {
    case MMITHEME_SLIDER_TYPE_BLOCK:
        baseflex_ctrl_ptr->tpdown_slider = GUIEDIT_SLIDER_BLOCK;
        break;

    case MMITHEME_SLIDER_TYPE_LEFT_SELECTOR:
        baseflex_ctrl_ptr->tpdown_slider = GUIEDIT_SLIDER_LEFT_SELECTOR;
        break;

    case MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR:
        baseflex_ctrl_ptr->tpdown_slider = GUIEDIT_SLIDER_RIGHT_SELECTOR;
        break;

    default:
        result = FALSE;
        baseflex_ctrl_ptr->tpdown_slider = GUIEDIT_SLIDER_NONE;
        break;
    }

    if (result)
    {
        // 默认拖动时不清除选中区
        baseflex_ctrl_ptr->tp_region = GUIEDIT_TP_REGION_NONE;

        baseflex_ctrl_ptr->tp_state = GUIEDIT_TP_STATE_DRAG;
    }
}

/*****************************************************************************/
//  Description : 拖动滑块
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleMoveSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
)
{
    MMITHEME_SLIDER_MSG_PARAM_T *slider_param_ptr = PNULL;
    MMI_MULTI_KEY_TP_MSG_PARAM_T    tp_msg = {0};

    if (GUIEDIT_SLIDER_NONE == baseflex_ctrl_ptr->tpdown_slider)
    {
        return;
    }

    slider_param_ptr = (MMITHEME_SLIDER_MSG_PARAM_T*) param;
    tp_msg.cur_tp_point = slider_param_ptr->tp_point;

    BASEFLEX_HandleTpMove (baseflex_ctrl_ptr, &tp_msg);
}

/*****************************************************************************/
//  Description : 滑块拖动结束
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEndSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
)
{
#ifdef TOUCH_PANEL_SUPPORT
    //stop tp scroll timer
    BASEFLEX_StopTpScrollTimer (baseflex_ctrl_ptr);
#endif
    baseflex_ctrl_ptr->tpdown_slider = GUIEDIT_SLIDER_NONE;

    StartSliderTimer (baseflex_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
    BASEFLEX_DisplayClipboardMenu (baseflex_ctrl_ptr, PNULL);
#endif
}

/*****************************************************************************/
//  Description : stop slider timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StopSliderTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
)
{
    if (0 < baseflex_ctrl_ptr->slider_dis_timer_id)
    {
        MMK_StopTimer (baseflex_ctrl_ptr->slider_dis_timer_id);
        baseflex_ctrl_ptr->slider_dis_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle slider timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleSliderTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
)
{
    BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);

#ifdef GUIF_CLIPBOARD
    BASEFLEX_CloseClipboardMenu (baseflex_ctrl_ptr);
#endif
}

/*****************************************************************************/
//  Description : create slider
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CreateEditSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
)
{
    int32 i = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;

    if (MMITHEME_DRAGMODE_DEFAULT != baseflex_ctrl_ptr->flex_theme.drag_mode)
    {
        for (i = 0; i < MMITHEME_SLIDER_TYPE_MAX; i++)
        {
            baseflex_ctrl_ptr->slider[i] =
                MMITHEME_CreateSlider (i, base_ctrl_ptr->win_handle, base_ctrl_ptr->handle);
        }
    }
}

/*****************************************************************************/
//  Description : start slider timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartSliderTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
)
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;

    //include progress bar
    if ((0 == baseflex_ctrl_ptr->slider_dis_timer_id) &&
        (MMK_IsFocusWin (base_ctrl_ptr->win_handle)))
    {
        baseflex_ctrl_ptr->slider_dis_timer_id = MMK_CreateWinTimer (
                base_ctrl_ptr->handle,
                baseflex_ctrl_ptr->flex_theme.slider_display_period,
                FALSE);
    }
}

#endif      // GUIF_CLIPBOARD



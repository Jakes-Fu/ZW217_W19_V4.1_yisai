/*****************************************************************************
** File Name:      ctrltext_clipbrd.c                                        *
** Author:         hua.fang                                                  *
** Date:           09/16/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2011        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmitheme_clipbrd.h"

#ifdef GUIF_CLIPBOARD

#include "ctrltext_export.h"
#include "ctrltext.h"
#include "mmk_timer.h"

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
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CreateTextSlider(
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                            );

/*****************************************************************************/
//  Description : start slider timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartSliderTimer(
                            CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                            );

/*****************************************************************************/
//  Description : stop slider timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopSliderTimer(
                            CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
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
PUBLIC BOOLEAN GUITEXT_IsSupportClipbrd(
                                        CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                        )
{
    BOOLEAN     is_support_clipbrd = FALSE;

    if (text_ctrl_ptr->is_clipbrd_support && text_ctrl_ptr->is_active)
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
PUBLIC void GUITEXT_DisplayClipboardMenu(
                                        GUI_POINT_T     *point_ptr,     //in:
                                        CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                        )
{
    CLIPBRDMENU_DATA_T  clip_brd_data = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    if (GUITEXT_IsSupportClipbrd(text_ctrl_ptr)
        && 0 == text_ctrl_ptr->clipbrd_win_handle
        && PNULL != point_ptr)
    {
        if (MMITHEME_DRAGMODE_DEFAULT != text_ctrl_ptr->theme.drag_mode)
        {
            point_ptr->y -= MMITHEME_GetClipbrdHeight();
        }

        //set data
        clip_brd_data.dst_ctrl_handle   = base_ctrl_ptr->handle;
        clip_brd_data.start_point       = *point_ptr;
        clip_brd_data.is_cut_disabled   = TRUE;
        clip_brd_data.is_paste_disabled = TRUE;
        clip_brd_data.is_select_disabled = TRUE;

        //no highlight
        if (text_ctrl_ptr->highlight.start_pos == text_ctrl_ptr->highlight.end_pos)
        {
            clip_brd_data.is_copy_disabled = TRUE;
        }

        if (0 == text_ctrl_ptr->highlight.start_pos
            && text_ctrl_ptr->highlight.end_pos == text_ctrl_ptr->str_len)
        {
            clip_brd_data.is_select_all_disabled = TRUE;
        }

        //display clip board menu
        text_ctrl_ptr->clipbrd_win_handle = MMITHEME_ShowClipbrdMenu(&clip_brd_data);
    }
}

/*****************************************************************************/
//  Description : close clip board menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_CloseClipboardMenu(
                                      CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                      )
{
    if (0 != text_ctrl_ptr->clipbrd_win_handle)
    {
        MMITHEME_CloseClipbrdMenu(text_ctrl_ptr->clipbrd_win_handle);
    }
}

/*****************************************************************************/
//  Description : handle text copy msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleTextCopyMsg(
                                     CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                     )
{
    wchar       *data_ptr = PNULL;
    uint16      data_len = 0;
    uint32      data_size = 0;

    //get highlight
    if (GUITEXT_GetHighlightStr(&data_ptr,&data_len,text_ctrl_ptr))
    {
        //set data size
        data_size = (uint32)(data_len * sizeof(wchar));
        
        //copy data
        MMITHEME_SetClipbrdData(CLIPBRD_FORMAT_TEXT,
            data_ptr,
            data_size);
    }

    GUITEXT_ChangeDragState(text_ctrl_ptr, GUITEXT_DRAGSTATE_NORMAL_RESET);
}

/*****************************************************************************/
//  Description : handle text select all msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleTextSelectAllMsg(
                                          CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                          )
{
    GUI_POINT_T     clipbrd_pt = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //set highlight
    text_ctrl_ptr->highlight.start_pos = 0;
    text_ctrl_ptr->highlight.end_pos   = text_ctrl_ptr->str_len;

    //update display
    GUITEXT_DisplayText(TRUE,text_ctrl_ptr);

    if (MMITHEME_DRAGMODE_DEFAULT != text_ctrl_ptr->theme.drag_mode)
    {
        GUITEXT_ChangeDragState(text_ctrl_ptr, GUITEXT_DRAGSTATE_SELECT);

        //// 潜规则
        //// 后面会打开窗口，在LOSE FOCUS消息里拷LCD内容，这里主层还没画到LCD上，
        //// 不会显示到出来，这里主动更新一下
        //GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );

        clipbrd_pt.x = base_ctrl_ptr->display_rect.left;
        clipbrd_pt.y = base_ctrl_ptr->display_rect.top;

        GUITEXT_DisplayClipboardMenu(&clipbrd_pt, text_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : handle text select msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleTextSelectMsg(
                                       CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                       )
{
    if (text_ctrl_ptr->str_len > 0
        && text_ctrl_ptr->highlight.start_pos == text_ctrl_ptr->highlight.end_pos)
    {
        if (text_ctrl_ptr->highlight.start_pos == text_ctrl_ptr->str_len)
        {
            text_ctrl_ptr->highlight.start_pos--;
        }
        else
        {
            text_ctrl_ptr->highlight.end_pos++;
        }

        //reset highlight not display
        GUITEXT_DisplayHighlight(TRUE,text_ctrl_ptr);

        GUITEXT_ChangeDragState(text_ctrl_ptr, GUITEXT_DRAGSTATE_SELECT);
    }
}

/*****************************************************************************/
//  Description : create slider
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CreateTextSlider(
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                            )
{
    int32 i = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    if (MMITHEME_DRAGMODE_DEFAULT != text_ctrl_ptr->theme.drag_mode)
    {
        for (i = 0; i < MMITHEME_SLIDER_TYPE_MAX; i++)
        {
            text_ctrl_ptr->slider[i] =
                MMITHEME_CreateSlider(i, text_ctrl_ptr->win_handle, base_ctrl_ptr->handle);
        }
    }
}

/*****************************************************************************/
//  Description : destroy slider
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_DestroyTextSlider(
                                     CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                     )
{
    int32 i = 0;

    StopSliderTimer(text_ctrl_ptr);

    // 清除滑块
    for (i = 0; i < MMITHEME_SLIDER_TYPE_MAX; i++)
    {
        if (0 != text_ctrl_ptr->slider[i])
        {
            MMITHEME_DestroySlider(text_ctrl_ptr->slider[i]);
        }
    }
}

/*****************************************************************************/
//  Description : 显示滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_ShowSlider(
                               CTRLTEXT_OBJ_T  *text_ctrl_ptr
                               )
{
    if (MMITHEME_DRAGMODE_DEFAULT != text_ctrl_ptr->theme.drag_mode
        && GUITEXT_IsSupportClipbrd(text_ctrl_ptr))
    {
        if (0 == text_ctrl_ptr->slider[0])
        {
            CreateTextSlider(text_ctrl_ptr);
        }

        if (GUITEXT_DRAGSTATE_SELECT == text_ctrl_ptr->drag_state)
        {
            MMITHEME_HideSlider(text_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_BLOCK]);
            MMITHEME_ShowSlider(text_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_LEFT_SELECTOR], 0, 0);
            MMITHEME_ShowSlider(text_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR], 0, 0);
        }
        else
        {
            MMITHEME_HideSlider(text_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_BLOCK]);
            MMITHEME_HideSlider(text_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_LEFT_SELECTOR]);
            MMITHEME_HideSlider(text_ctrl_ptr->slider[MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR]);
        }
    }
}

/*****************************************************************************/
//  Description : 改变滑块状态
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_ChangeDragState(
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr,
                                    GUITEXT_DRAGSTATE_E drag_state
                                    )
{
    if (MMITHEME_DRAGMODE_DEFAULT != text_ctrl_ptr->theme.drag_mode
        && GUITEXT_IsSupportClipbrd(text_ctrl_ptr))
    {
        if (GUITEXT_DRAGSTATE_NORMAL == drag_state
            || GUITEXT_DRAGSTATE_NORMAL_RESET == drag_state)
        {
            text_ctrl_ptr->drag_state = GUITEXT_DRAGSTATE_NORMAL;

            // 需要强制设置光标，去掉高亮的情况
            if (GUITEXT_DRAGSTATE_NORMAL_RESET == drag_state)
            {
                if (MMK_IsFocusWin(text_ctrl_ptr->win_handle))
                {
                    //reset highlight not display
                    GUITEXT_DisplayHighlight(FALSE,text_ctrl_ptr);
                }

                //set highlight
                text_ctrl_ptr->highlight.end_pos   = text_ctrl_ptr->highlight.start_pos;
            }
        }
        else if (GUITEXT_DRAGSTATE_SELECT == drag_state)
        {
            text_ctrl_ptr->drag_state = GUITEXT_DRAGSTATE_SELECT;
        }

        StopSliderTimer(text_ctrl_ptr);

        if (GUITEXT_DRAGSTATE_SELECT == text_ctrl_ptr->drag_state)
        {
            StartSliderTimer(text_ctrl_ptr);
        }

        GUITEXT_ShowSlider(text_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : start slider timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartSliderTimer(
                            CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                            )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //include progress bar
    if ((0 == text_ctrl_ptr->slider_dis_timer_id) &&
        (MMK_IsFocusWin(text_ctrl_ptr->win_handle)))
    {
        text_ctrl_ptr->slider_dis_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,
                                                text_ctrl_ptr->theme.slider_display_period,
                                                FALSE);
    }
}

/*****************************************************************************/
//  Description : stop slider timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopSliderTimer(
                           CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                           )
{
    if (0 < text_ctrl_ptr->slider_dis_timer_id)
    {
        MMK_StopTimer(text_ctrl_ptr->slider_dis_timer_id);
        text_ctrl_ptr->slider_dis_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle slider timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_HandleSliderTimer(
                                         CTRLTEXT_OBJ_T     *text_ctrl_ptr,  //in:
                                         uint8              time_id
                                         )
{
    BOOLEAN is_handled = FALSE;

    if (text_ctrl_ptr->slider_dis_timer_id == time_id)
    {
        GUITEXT_ChangeDragState(text_ctrl_ptr, GUITEXT_DRAGSTATE_NORMAL);

        GUITEXT_CloseClipboardMenu(text_ctrl_ptr);

        is_handled = TRUE;
    }

    return is_handled;
}

/*****************************************************************************/
//  Description : 开始滑块拖动
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleBeginSlider(
                                     CTRLTEXT_OBJ_T     *text_ctrl_ptr,
                                     DPARAM             param
                                     )
{
    int32 slider_type = (int32)param;

    StopSliderTimer(text_ctrl_ptr);

    switch(slider_type)
    {
    case MMITHEME_SLIDER_TYPE_LEFT_SELECTOR:
        text_ctrl_ptr->tpdown_slider = GUITEXT_SLIDER_LEFT_SELECTOR;
        text_ctrl_ptr->tp_state = GUITEXT_TP_STATE_DRAG;
        text_ctrl_ptr->tp_region = GUITEXT_TP_REGION_NONE;
        break;

    case MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR:
        text_ctrl_ptr->tpdown_slider = GUITEXT_SLIDER_RIGHT_SELECTOR;
        text_ctrl_ptr->tp_state = GUITEXT_TP_STATE_DRAG;
        text_ctrl_ptr->tp_region = GUITEXT_TP_REGION_NONE;
        break;

    default:
        text_ctrl_ptr->tpdown_slider = GUITEXT_SLIDER_NONE;
        break;
    }
}

/*****************************************************************************/
//  Description : 拖动滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleMoveSlider(
                                    CTRLTEXT_OBJ_T     *text_ctrl_ptr,
                                    DPARAM             param
                                    )
{
    MMITHEME_SLIDER_MSG_PARAM_T *slider_param_ptr = PNULL;
    MMI_MULTI_KEY_TP_MSG_PARAM_T    tp_msg = {0};

    if (GUITEXT_SLIDER_NONE == text_ctrl_ptr->tpdown_slider)
    {
        return;
    }

    slider_param_ptr = (MMITHEME_SLIDER_MSG_PARAM_T*)param;
    tp_msg.cur_tp_point = slider_param_ptr->tp_point;
#ifdef TOUCH_PANEL_SUPPORT
    GUITEXT_HandleTextTpMove(text_ctrl_ptr, &tp_msg);
#endif
}

/*****************************************************************************/
//  Description : 滑块拖动结束
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleEndSlider(
                                   CTRLTEXT_OBJ_T     *text_ctrl_ptr,
                                   DPARAM             param
                                   )
{
    GUI_POINT_T     tp_point = {0};
    GUI_RECT_T      cursor_rect = {0};
    uint16          pos = 0;
#ifdef TOUCH_PANEL_SUPPORT
    //stop tp scroll timer
    GUITEXT_StopTpScrollTimer(text_ctrl_ptr);
#endif
    text_ctrl_ptr->tpdown_slider = GUITEXT_SLIDER_NONE;

    StartSliderTimer(text_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
    if ((int32)MMITHEME_SLIDER_TYPE_LEFT_SELECTOR == (int32)param)
    {
        pos = GUITEXT_GetHighlightMinPos(text_ctrl_ptr);
    }
    else
    {
        pos = GUITEXT_GetHighlightMaxPos(text_ctrl_ptr);
    }

    GUITEXT_GetPosRect(text_ctrl_ptr, pos, &cursor_rect);

    tp_point.x = cursor_rect.left;
    tp_point.y = cursor_rect.top;

    GUITEXT_DisplayClipboardMenu(&tp_point, text_ctrl_ptr);
#endif
}

#endif      // GUIF_CLIPBOARD


/*****************************************************************************
** File Name:      ctrledit_tp.c                                             *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2009      Jassmine              Creat
******************************************************************************/


#ifdef TOUCH_PANEL_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "spml_api.h"
#include "ctrlbaseflex.h"
#include "mmi_theme.h"
#include "mmitheme_clipbrd.h"
#include "chng_freq.h"
#include "mmk_tp.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get tp line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTpLineIndex (//is tp in edit rect
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_POINT_T         tp_point,
    uint16              *line_index_ptr     //in/out:
);

/*****************************************************************************/
//  Description : get tp char offset
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTpCharOffset (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              line_index,     //in:
    GUI_POINT_T         tp_point,       //in:
    BOOLEAN             *is_tp_str_ptr  //in/out:may PNULL
);

/*****************************************************************************/
//  Description : is tp on the highlight string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpHighLight (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_tp_str,      //in:is tp on string
    uint16              char_offset,    //in:tp char offset
    uint16              line_index      //in:tp line index
);

/*****************************************************************************/
//  Description : single line edit handle tp msg for slide
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleSingleTpSlide (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    DPARAM              param
);

/*****************************************************************************/
//  Description : handle edit tp move msg, highlight string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTpMoveDrag (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    DPARAM              param
);

/*****************************************************************************/
//  Description : is tp in edit rect,除去显示不完整的line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpInEditLine (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_POINT_T    tp_point,           //in:
    GUI_RECT_T     *top_rect_ptr,      //in/out:完整显示的top行区域
    GUI_RECT_T     *bottom_rect_ptr    //in/out:完整显示的bottom行区域
);

/*****************************************************************************/
//  Description : edit first line is display all
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayFirstLine (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : edit last line is display all
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayLastLine (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : set edit cursor position for tp
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 SetEditCursorPos (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    GUI_POINT_T         *tp_point_ptr,          //in:
    BOOLEAN             *is_cursor_set_ptr      //out:
);

/*****************************************************************************/
//  Description : start tp scroll timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpScrollTimer (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_scroll_up    //in:
);

/*****************************************************************************/
//  Description : start tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpSlideTimer(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : reset text slide display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetTpSlide(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_update
);

/*****************************************************************************/
//  Description : handle text slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTpSlide(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_POINT_T         tp_point
);

/*****************************************************************************/
//  Description : set top index by relative display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetTopIndex(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    int32               relative_top
);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : stop tp scroll timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StopTpScrollTimer (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
)
{
    if (0 < baseflex_ctrl_ptr->tp_scroll_timer_id)
    {
        MMK_StopTimer (baseflex_ctrl_ptr->tp_scroll_timer_id);
        baseflex_ctrl_ptr->tp_scroll_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle tp edit scroll timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleTpScrollTimer (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
)
{
    BOOLEAN     is_cursor_set = FALSE;
    BOOLEAN     result = TRUE;
    uint16      cursor_pos = 0;
    uint16      line_height = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //stop tp timer
    BASEFLEX_StopTpScrollTimer (baseflex_ctrl_ptr);

    if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
    {
        if (baseflex_ctrl_ptr->is_tp_scroll_up)
        {
            //the first page
            if (0 == baseedit_ctrl_ptr->cursor.cur_pos)
            {
                result = FALSE;
            }
        }
        else
        {
            //the last page
            if (baseedit_ctrl_ptr->cursor.cur_pos == baseedit_ctrl_ptr->str_len)
            {
                result = FALSE;
            }
        }

        if (result)
        {
            cursor_pos = BASEFLEX_SetCursorPosForLeftRight (baseflex_ctrl_ptr, baseflex_ctrl_ptr->is_tp_scroll_up, &is_cursor_set);
        }
    }
    else
    {
        line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

        if (baseflex_ctrl_ptr->is_tp_scroll_up)
        {
            //the first page
            if (IsDisplayFirstLine (baseflex_ctrl_ptr))
            {
                result = FALSE;
            }
            else if (!baseedit_ctrl_ptr->is_rect_adaptive)
            {
                if (0 == (baseedit_ctrl_ptr->top_line_offset % line_height))
                {
                    baseedit_ctrl_ptr->top_line_offset -= line_height;
                }
                else
                {
                    baseedit_ctrl_ptr->top_line_offset = (baseedit_ctrl_ptr->top_line_offset / line_height) * line_height;
                }
            }
        }
        else
        {
            //the last page
            if (IsDisplayLastLine (baseflex_ctrl_ptr))
            {
                result = FALSE;
            }
            else if (!baseedit_ctrl_ptr->is_rect_adaptive)
            {
                if (0 == (baseedit_ctrl_ptr->top_line_offset % line_height))
                {
                    baseedit_ctrl_ptr->top_line_offset += line_height;
                }
                else
                {
                    baseedit_ctrl_ptr->top_line_offset = (1 + (baseedit_ctrl_ptr->top_line_offset / line_height)) * line_height;
                }
            }
        }

        if (result)
        {
            //get cursor pos for tp
            cursor_pos = SetEditCursorPos (baseflex_ctrl_ptr, &baseflex_ctrl_ptr->tp_move_point, &is_cursor_set);
        }
    }

    if (is_cursor_set)
    {
#ifdef GUIF_CLIPBOARD
        if (GUIEDIT_SLIDER_BLOCK == baseflex_ctrl_ptr->tpdown_slider)
        {
            BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, cursor_pos);
        }
        else if (GUIEDIT_SLIDER_LEFT_SELECTOR == baseflex_ctrl_ptr->tpdown_slider)
        {
            VTLBASEEDIT_SetHighlight (
                    baseedit_ctrl_ptr,
                    cursor_pos, 0,
                    SETHIGHLIGHT_START);
        }
        else if (GUIEDIT_SLIDER_RIGHT_SELECTOR == baseflex_ctrl_ptr->tpdown_slider
                 || MMITHEME_DRAGMODE_DEFAULT == baseflex_ctrl_ptr->flex_theme.drag_mode)
#endif
        {
            VTLBASEEDIT_SetHighlight (
                    baseedit_ctrl_ptr,
                    0, cursor_pos,
                    SETHIGHLIGHT_END);
        }

        // cursor will be between min_pos and max_pos while moving upword,
        // so check cursor and make it at the beginning of the highlight.
        // must call after GUIEDIT_SetHighlight, otherwise it will be error.
        BASEFLEX_CheckCursor (baseflex_ctrl_ptr);

        //update
        VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, TRUE);

        //send move cursor msg
        BASEEDIT_NotifyCursorMsg (baseedit_ctrl_ptr);
    }

    //start tp timer
    StartTpScrollTimer (baseflex_ctrl_ptr, baseflex_ctrl_ptr->is_tp_scroll_up);
}

/*****************************************************************************/
//  Description : handle edit tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditTpDown (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    DPARAM              param
)
{
    BOOLEAN         is_tp_line = FALSE;
    BOOLEAN         is_tp_char = FALSE;
    BOOLEAN         is_tp_str = FALSE;
    BOOLEAN         is_set_cursor = FALSE;
    uint16          tp_line_index = 0;
    uint16          char_offset = 0;
    uint16          highlight_len = 0;
    uint16          cursor_pos = 0;
    GUI_POINT_T     tp_point = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //is tp down
    baseflex_ctrl_ptr->is_tp_down = TRUE;

    //reset tp state
    baseflex_ctrl_ptr->tp_state = GUIEDIT_TP_STATE_NONE;

    //reset tp region
    baseflex_ctrl_ptr->tp_region = GUIEDIT_TP_REGION_NONE;

#ifdef GUIF_CLIPBOARD
    //set not display clip board menu
    BASEFLEX_CloseClipboardMenu (baseflex_ctrl_ptr);
#endif

    baseflex_ctrl_ptr->is_tp_moved = FALSE;

    if (0 < baseedit_ctrl_ptr->str_len)
    {
        //set tp press point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

        //set previous point
        baseflex_ctrl_ptr->prev_point = tp_point;

        //reset cursor not display
        BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

        //get tp line index
        is_tp_line = GetTpLineIndex (baseflex_ctrl_ptr, tp_point, &tp_line_index);

        //get tp x offset
        char_offset = GetTpCharOffset (baseflex_ctrl_ptr, tp_line_index, tp_point, &is_tp_char);

        //get highlight len
        highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

        //is tp string
        if ( (is_tp_line) && (is_tp_char))
        {
            is_tp_str = TRUE;
        }

        //no highlight
        if (0 == highlight_len)
        {
            is_set_cursor = TRUE;
        }
        else
        {
            //is tp highlight
            if (!IsTpHighLight (baseflex_ctrl_ptr, is_tp_str, char_offset, tp_line_index))
            {
                //reset highlight not display
                BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

                is_set_cursor = TRUE;
            }
        }

        if (is_set_cursor)
        {
            //set cursor position
            cursor_pos = (uint16) (baseedit_ctrl_ptr->line_info_ptr[tp_line_index].start_pos + char_offset);

            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);

            baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;

            //set no highlight
            BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, cursor_pos);

            // 如果点在object上，需要高亮
            IsTpHighLight (baseflex_ctrl_ptr, is_tp_str, char_offset, tp_line_index);

            //set display cursor
            baseedit_ctrl_ptr->cursor.is_display    = TRUE;
            baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

            //display cursor
            BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
            BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_DRAG);
#endif
        }

        //for TP down active edit,discard tp move/up msg
        baseflex_ctrl_ptr->is_tp_rect_change = FALSE;

        //is glide
        if (baseflex_ctrl_ptr->flex_theme.is_support_slide
            && !baseedit_ctrl_ptr->is_rect_adaptive)
        {
            //set start and previous point
            baseflex_ctrl_ptr->start_point = tp_point;
            baseflex_ctrl_ptr->prev_point  = tp_point;

            //set tp down slide state
            baseflex_ctrl_ptr->tpdown_slide_state = baseflex_ctrl_ptr->slide_state;

            //add velocity item
            MMK_AddVelocityItem(tp_point.x,tp_point.y);

            //if (MMK_TP_SLIDE_FLING != baseflex_ctrl_ptr->slide_state)
            //{
            //    //start tp slide timer
            //    StartTpSlideTimer(baseflex_ctrl_ptr);
            //}
        }
    }
#ifdef GUIF_CLIPBOARD
    else
    {
        BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_DRAG);
    }
#endif
}

/*****************************************************************************/
//  Description : handle edit tp short msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E BASEFLEX_HandleTpShort (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    if ((0 < baseedit_ctrl_ptr->str_len) &&
        (!baseflex_ctrl_ptr->is_tp_rect_change))
    {
        if (MMITHEME_DRAGMODE_DEFAULT == baseflex_ctrl_ptr->flex_theme.drag_mode)
        {
            //set text tp state
            baseflex_ctrl_ptr->tp_state = GUIEDIT_TP_STATE_DRAG;
        }

        //stop slide timer
        BASEFLEX_StopTpSlideTimer(baseflex_ctrl_ptr);

        result = MMI_RESULT_TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle edit tp move msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleTpMove (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //for TP down active edit,discard tp move/up msg
    if ((0 < baseedit_ctrl_ptr->str_len) &&
        (!baseflex_ctrl_ptr->is_tp_rect_change))
    {
        baseflex_ctrl_ptr->is_tp_moved = TRUE;

        if (GUIEDIT_TP_STATE_DRAG == baseflex_ctrl_ptr->tp_state)
        {
            HandleTpMoveDrag (baseflex_ctrl_ptr, param);
        }
        else if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
        {
            HandleSingleTpSlide (baseflex_ctrl_ptr, param);
        }
        else if (baseflex_ctrl_ptr->flex_theme.is_support_slide
            && !baseedit_ctrl_ptr->is_rect_adaptive)
        {
            GUI_POINT_T     tp_point = {0};

            //set tp press point
            tp_point.x = MMK_GET_TP_X (param);
            tp_point.y = MMK_GET_TP_Y (param);

            if (IS_TP_MOVE_Y(baseflex_ctrl_ptr->start_point.y, tp_point.y))
            {
                //set slide state
                if ((MMK_TP_SLIDE_NONE == baseflex_ctrl_ptr->tpdown_slide_state) ||
                    (MMK_IsWayChanged(baseflex_ctrl_ptr->fling_velocity,&baseflex_ctrl_ptr->start_point,&tp_point,FALSE)))
                {
                    baseflex_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;

                    if (0 == baseflex_ctrl_ptr->slide_timer_id)
                    {
                        //start tp slide timer
                        StartTpSlideTimer(baseflex_ctrl_ptr);
                    }
                }
            }

            //add velocity item
            MMK_AddVelocityItem(tp_point.x,tp_point.y);
        }
    }
}

/*****************************************************************************/
//  Description : handle edit tp long msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleTpLong (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
)
{
#ifdef GUIF_CLIPBOARD
    GUI_POINT_T         tp_point = {0};

    // show clipboard only when not moved.
    if ((MMK_TP_SLIDE_NONE == baseflex_ctrl_ptr->slide_state) && 
        (!baseflex_ctrl_ptr->is_tp_moved)
        && BASEFLEX_IsSupportClipbrd (baseflex_ctrl_ptr))
    {
        if (MMITHEME_DRAGMODE_DEFAULT != baseflex_ctrl_ptr->flex_theme.drag_mode)
        {
            //display clip board menu
            BASEFLEX_DisplayClipboardMenu (baseflex_ctrl_ptr, PNULL);
        }
        else
        {
            //set tp press point
            tp_point.x = MMK_GET_TP_X (param);
            tp_point.y = MMK_GET_TP_Y (param);

            //display clip board menu
            BASEFLEX_DisplayClipboardMenu (baseflex_ctrl_ptr, &tp_point);
        }
    }
#endif
}

/*****************************************************************************/
//  Description : handle edit tp up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditTpUp (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
)
{
    uint16          highlight_len = 0;
    uint16          cursor_pos = 0;
    GUI_POINT_T     tp_point = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //is tp down
    baseflex_ctrl_ptr->is_tp_down = FALSE;

    //stop tp scroll timer
    BASEFLEX_StopTpScrollTimer (baseflex_ctrl_ptr);

    //stop slide timer
    BASEFLEX_StopTpSlideTimer(baseflex_ctrl_ptr);

    //for TP down active edit,discard tp move/up msg
    if ((!MMK_GET_TP_SLIDE (param)) &&
        (0 < baseedit_ctrl_ptr->str_len) &&
        (!baseflex_ctrl_ptr->is_tp_rect_change))
    {
        //set tp press point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

		if (MMK_TP_SLIDE_NONE != baseflex_ctrl_ptr->slide_state)
		{
			//text is slide
            ((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->is_slide = TRUE;
		}

        if ((baseflex_ctrl_ptr->flex_theme.is_support_slide) &&
            (!baseedit_ctrl_ptr->is_rect_adaptive) &&
            ((MMK_TP_SLIDE_NONE != baseflex_ctrl_ptr->slide_state) ||
                ((IS_TP_MOVE_Y(baseflex_ctrl_ptr->prev_point.y, tp_point.y)) &&
		            (GUIEDIT_TP_STATE_DRAG != baseflex_ctrl_ptr->tp_state))))
        {
            //add velocity item
            MMK_AddVelocityItem(tp_point.x,tp_point.y);
            
            //calculate current velocity
            MMK_ComputeCurrentVelocity(
                    PNULL,
                    &baseflex_ctrl_ptr->fling_velocity,
                    FLING_MAX_VELOCITY,
                    FLING_MAX_VELOCITY );

            //fling velocity
            if (FLING_MIN_VELOCITY > abs((int32)baseflex_ctrl_ptr->fling_velocity))
            {
                //reset velocity item
                MMK_ResetVelocityItem();

                baseflex_ctrl_ptr->fling_velocity = 0;

                //reset slide state
                baseflex_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

                //reset text slide
                ResetTpSlide(baseflex_ctrl_ptr, TRUE);
            }
            else
            {
                //enter fling state
                baseflex_ctrl_ptr->slide_state = MMK_TP_SLIDE_FLING;

                //start tp slide timer
                StartTpSlideTimer(baseflex_ctrl_ptr);
            }
        }
        else
        {
            //get highlight len
            highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

            if (0 < highlight_len)
            {
                //get cursor pos for tp
                cursor_pos = SetEditCursorPos (baseflex_ctrl_ptr, &tp_point, PNULL);

                //tp on highlight
                // fix cr: 点在最后选中文本对象，后快速拖到普通数字处，此时处于
                // GUIEDIT_TP_REGION_NONE状态，也非GUIEDIT_TP_STATE_DRAG，但由于光标
                // 已改，但highlight没动，出错
                if (GUIEDIT_TP_REGION_HIGHLIGHT == baseflex_ctrl_ptr->tp_region ||
                    GUIEDIT_TP_STATE_DRAG != baseflex_ctrl_ptr->tp_state)
                {
                    //set no highlight
                    BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, cursor_pos);

                    BASEFLEX_CheckCursor (baseflex_ctrl_ptr);
                }
                else if (GUIEDIT_TP_STATE_DRAG == baseflex_ctrl_ptr->tp_state)
                {
                    VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr,
                                              0,
                                              cursor_pos,
                                              SETHIGHLIGHT_END);

                    // cursor will be between min_pos and max_pos while moving upword,
                    // so check cursor and make it at the beginning of the highlight.
                    // must call after GUIEDIT_SetHighlight, otherwise it will be error.
                    BASEFLEX_CheckCursor (baseflex_ctrl_ptr);

                    //display highlight
                    BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

#ifdef GUIF_CLIPBOARD
                    //display clip board menu
                    if (baseflex_ctrl_ptr->is_tp_moved)
                    {
                        BASEFLEX_DisplayClipboardMenu (baseflex_ctrl_ptr, &tp_point);
                    }
#endif
                }

#ifdef GUIF_CLIPBOARD
                BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_DRAG);
#endif

                //set display cursor
                baseedit_ctrl_ptr->cursor.is_display    = TRUE;
                baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

                //display cursor
                BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);
            }
            else if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
            {
#ifdef GUIF_CLIPBOARD
                BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_DRAG);
#endif

                HandleSingleTpSlide (baseflex_ctrl_ptr, param);
            }

            //config im
            VTLBASEEDIT_ConfigImKey (baseedit_ctrl_ptr);

            //send move cursor msg
            BASEEDIT_NotifyCursorMsg (baseedit_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : get tp line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTpLineIndex (//is tp in edit rect
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_POINT_T         tp_point,
    uint16              *line_index_ptr     //in/out:
)
{
    BOOLEAN     result = FALSE;
    uint16      i = 0;
    uint16      top_line_index = 0;
    uint16      bottom_line_index = 0;
    GUI_RECT_T  line_rect = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //get top line rect
    top_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
    line_rect = BASEEDIT_GetLineRect (
            baseedit_ctrl_ptr,
            top_line_index);

    if (tp_point.y < line_rect.top)
    {
        //set line index is the top line
        *line_index_ptr = top_line_index;
    }
    else
    {
        //get bottom line index
        bottom_line_index = BASEEDIT_GetBottomLineIndex (baseedit_ctrl_ptr);

        //get bottom line rect
        line_rect = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, bottom_line_index);

        if (tp_point.y > line_rect.bottom)
        {
            //set line index is the bottom line
            *line_index_ptr = bottom_line_index;
        }
        else
        {
            //get tp line index
            for (i = top_line_index; i <= bottom_line_index; i++)
            {
                //get line rect,include dividing line
                line_rect = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, i);
                line_rect.bottom = (int16) (line_rect.bottom + baseedit_ctrl_ptr->common_theme.dividing_line_width);

                if ((tp_point.y >= line_rect.top) &&
                    (tp_point.y <= line_rect.bottom))
                {
                    *line_index_ptr = i;
                    result = TRUE;
                    break;
                }
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get tp char offset
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTpCharOffset (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              line_index,     //in:
    GUI_POINT_T         tp_point,       //in:
    BOOLEAN             *is_tp_str_ptr  //in/out:may PNULL
)
{
    wchar       *str_ptr = PNULL;
    uint16      str_len = 0;
    uint16      char_offset = 0;
    uint16      text_width = 0;
    int32		adjust_offset = 0;
    GUI_RECT_T  text_rect = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //get text rect
    text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, line_index);

    //get display string width
    if (tp_point.x <= text_rect.left)
    {
        if (TEXT_DIR_RTL == baseedit_ctrl_ptr->line_info_ptr[line_index].direction)
        {
            char_offset = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, line_index);
        }
        else
        {
            char_offset = 0;
        }

        //set is tp on the string
        if (PNULL != is_tp_str_ptr)
        {
            *is_tp_str_ptr = FALSE;
        }
    }
    else
    {
        text_width = (uint16) (tp_point.x - text_rect.left + 1);

        //get display string
        str_len = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, line_index);

        str_ptr = VTLBASEEDIT_GetDispStr (
                      baseedit_ctrl_ptr,
                      baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos,
                      str_len);

        char_offset = GUI_GetCursorPos (
                          BASEEDIT_GetCursorAlign (baseedit_ctrl_ptr),
                          &text_rect,
                          baseedit_ctrl_ptr->line_info_ptr[line_index].direction,
                          str_ptr,
                          str_len,
                          0,
                          tp_point.x,
                          baseedit_ctrl_ptr->common_theme.font.font,
                          baseedit_ctrl_ptr->common_theme.char_space,
                          is_tp_str_ptr);

        //adjust char offset
        adjust_offset = char_offset;
        GUI_GetCursorInfo (
            BASEEDIT_GetCursorAlign (baseedit_ctrl_ptr),
            &text_rect,
            baseedit_ctrl_ptr->line_info_ptr[line_index].direction,
            str_ptr,
            str_len,
            FALSE,
            baseedit_ctrl_ptr->common_theme.font.font,
            baseedit_ctrl_ptr->common_theme.char_space,
            &adjust_offset);
        char_offset = (uint16) (adjust_offset);

        //last char is LF
        if ((char_offset == str_len) &&
            (BASEEDIT_IsNewLineChar (baseedit_ctrl_ptr, baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos + str_len)))
        {
            //adjust char offset
            if (char_offset > 0)
            {
                char_offset--;
            }

            if (PNULL != is_tp_str_ptr)
            {
                *is_tp_str_ptr = TRUE;
            }
        }
    }

    return (char_offset);
}

/*****************************************************************************/
//  Description : is tp on the highlight string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpHighLight (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_tp_str,      //in:is tp on string
    uint16              char_offset,    //in:tp char offset
    uint16              line_index      //in:tp line index
)
{
    BOOLEAN     result = FALSE;
    uint16      highlight_start = 0;
    uint16      highlight_end = 0;
    uint16      highlight_len = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //get highlight start pos and length
    highlight_len = BASEEDIT_GetHighlightStartPosAndLen (
                        baseedit_ctrl_ptr,
                        line_index,
                        &highlight_start);

    if ((is_tp_str) && (0 < highlight_len))
    {
        //get highlight end pos
        highlight_end = (uint16) (highlight_start + highlight_len);

        if ((char_offset >= highlight_start) && (char_offset <= highlight_end))
        {
            //set tp region
            baseflex_ctrl_ptr->tp_region = GUIEDIT_TP_REGION_HIGHLIGHT;

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : single line edit handle tp msg for slide
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleSingleTpSlide (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    DPARAM              param
)
{
    wchar               *remain_str_ptr = PNULL;
    wchar               *tmp_str_ptr = PNULL;
    uint16              remain_str_len = 0;
    uint16              text_width = 0;
    uint16              char_num = 0;
    uint16              string_pixel = 0;
    uint16              cursor_pos = 0;
    uint16              i = 0;
    uint16              prev_start_pos = 0;
    GUI_POINT_T         tp_point = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //set tp press point
    tp_point.x = MMK_GET_TP_X (param);
    tp_point.y = MMK_GET_TP_Y (param);

    //set previous start pos
    prev_start_pos = baseedit_ctrl_ptr->line_info_ptr[0].start_pos;

    //slide string to left
    if ((tp_point.x < baseflex_ctrl_ptr->prev_point.x)
        && (baseedit_ctrl_ptr->str_len > baseedit_ctrl_ptr->line_info_ptr[1].start_pos))
    {
        //set text width
        text_width = (uint16) (baseflex_ctrl_ptr->prev_point.x - tp_point.x);

        //get remain string
        //remain_str_ptr = baseedit_ctrl_ptr->str_ptr + baseedit_ctrl_ptr->line_info_ptr[1].start_pos;
        remain_str_len = (uint16) (baseedit_ctrl_ptr->str_len - baseedit_ctrl_ptr->line_info_ptr[1].start_pos);
        remain_str_ptr = VTLBASEEDIT_GetDispStr(baseedit_ctrl_ptr, baseedit_ctrl_ptr->line_info_ptr[1].start_pos, remain_str_len);

        GUI_GetStringInfoInLineWithDir (
                baseedit_ctrl_ptr->line_info_ptr[0].direction,
                remain_str_ptr,
                remain_str_len,
                baseedit_ctrl_ptr->common_theme.font.font,
                (uint8) baseedit_ctrl_ptr->common_theme.char_space,
                text_width,
                &char_num,
                &string_pixel,
                FALSE);

        //set cursor position
        cursor_pos = baseedit_ctrl_ptr->cursor.cur_pos;
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->line_info_ptr[1].start_pos + char_num));

        //get first char index
        BASEFLEX_GetFirstCharIndex (baseflex_ctrl_ptr);

        //reset cursor position
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);
    }
    else if ( (tp_point.x > baseflex_ctrl_ptr->prev_point.x) &&
              (0 < baseedit_ctrl_ptr->line_info_ptr[0].start_pos))
    {
        //set text width
        text_width = (uint16) (tp_point.x - baseflex_ctrl_ptr->prev_point.x);

        //get remain string len
        remain_str_len = baseedit_ctrl_ptr->line_info_ptr[0].start_pos;

        //alloc
        remain_str_ptr = SCI_ALLOC_APP ( (remain_str_len + 1) * sizeof (wchar));
        SCI_MEMSET (remain_str_ptr, 0, ( (remain_str_len + 1) *sizeof (wchar)));

        //get reverse string
        tmp_str_ptr = VTLBASEEDIT_GetDispStr(baseedit_ctrl_ptr, 0, remain_str_len);
        for (i = 0; i < remain_str_len; i++)
        {
            remain_str_ptr[i] = tmp_str_ptr[remain_str_len-1-i];
        }

        GUI_GetStringInfoInLineWithDir (
                baseedit_ctrl_ptr->line_info_ptr[0].direction,
                remain_str_ptr,
                remain_str_len,
                baseedit_ctrl_ptr->common_theme.font.font,
                (uint8) baseedit_ctrl_ptr->common_theme.char_space,
                text_width,
                &char_num,
                &string_pixel,
                FALSE);

        if (0 != char_num &&
            baseedit_ctrl_ptr->line_info_ptr[0].start_pos >= char_num)
        {
            //set cursor position
            cursor_pos = baseedit_ctrl_ptr->cursor.cur_pos;
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->line_info_ptr[0].start_pos - char_num));

            //get first char index
            baseedit_ctrl_ptr->line_info_ptr[0].start_pos = baseedit_ctrl_ptr->cursor.cur_pos;
            BASEFLEX_GetFirstCharIndex (baseflex_ctrl_ptr);

            //reset cursor position
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);
        }

        //free reverse string
        SCI_FREE (remain_str_ptr);
    }

    if (prev_start_pos != baseedit_ctrl_ptr->line_info_ptr[0].start_pos)
    {
        //set previous point
        baseflex_ctrl_ptr->prev_point = tp_point;

        //update display
        VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, TRUE);
    }
}

/*****************************************************************************/
//  Description : handle edit tp move msg, highlight string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTpMoveDrag (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    DPARAM              param
)
{
    BOOLEAN         is_cursor_set = FALSE;
    BOOLEAN         is_handle = FALSE;
    uint16          cursor_pos = 0;
    GUI_POINT_T     tp_point = {0};
    GUI_RECT_T      top_line_rect = {0};
    GUI_RECT_T      bottom_line_rect = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //set tp press point
    tp_point.x = MMK_GET_TP_X (param);
    tp_point.y = MMK_GET_TP_Y (param);

    //is in text rect
    if (IsTpInEditLine (baseflex_ctrl_ptr, tp_point, &top_line_rect, &bottom_line_rect))
    {
        is_handle =  TRUE;
    }
    else
    {
        //set tp move point
        baseflex_ctrl_ptr->tp_move_point = tp_point;

        if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
        {
            if (tp_point.x < top_line_rect.left)
            {
                //the first
                if (0 == baseedit_ctrl_ptr->cursor.cur_pos)
                {
                    is_handle = TRUE;
                }
                else
                {
                    //start tp scroll timer
                    StartTpScrollTimer (baseflex_ctrl_ptr, TRUE);
                }
            }
            else if (tp_point.x > top_line_rect.right)
            {
                //the last
                if (baseedit_ctrl_ptr->cursor.cur_pos == baseedit_ctrl_ptr->str_len)
                {
                    is_handle = TRUE;
                }
                else
                {
                    //start tp scroll timer
                    StartTpScrollTimer (baseflex_ctrl_ptr, FALSE);
                }
            }
        }
        else
        {
            if (tp_point.y < top_line_rect.top)
            {
                //the first page
                if (IsDisplayFirstLine (baseflex_ctrl_ptr))
                {
                    is_handle = TRUE;
                }
                else
                {
                    //start tp scroll timer
                    StartTpScrollTimer (baseflex_ctrl_ptr, TRUE);
                }
            }
            else if (tp_point.y > bottom_line_rect.bottom)
            {
                //the last page
                if (IsDisplayLastLine (baseflex_ctrl_ptr))
                {
                    is_handle = TRUE;
                }
                else
                {
                    //start tp scroll timer
                    StartTpScrollTimer (baseflex_ctrl_ptr, FALSE);
                }
            }
        }
    }

    if (is_handle)
    {
        //stop tp scroll timer
        BASEFLEX_StopTpScrollTimer (baseflex_ctrl_ptr);

        //get cursor pos for tp
        cursor_pos = SetEditCursorPos (baseflex_ctrl_ptr, &tp_point, &is_cursor_set);

        if (is_cursor_set)
        {
            //move on highlight
            if (GUIEDIT_TP_REGION_HIGHLIGHT == baseflex_ctrl_ptr->tp_region)
            {
                //reset tp region
                baseflex_ctrl_ptr->tp_region = GUIEDIT_TP_REGION_NONE;

                VTLBASEEDIT_SetHighlight (
                        baseedit_ctrl_ptr,
                        cursor_pos,
                        0,
                        SETHIGHLIGHT_START);
            }

#ifdef GUIF_CLIPBOARD
            if (GUIEDIT_SLIDER_BLOCK == baseflex_ctrl_ptr->tpdown_slider)
            {
                BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, cursor_pos);
            }
            else if (GUIEDIT_SLIDER_LEFT_SELECTOR == baseflex_ctrl_ptr->tpdown_slider)
            {
                VTLBASEEDIT_SetHighlight (
                        baseedit_ctrl_ptr,
                        cursor_pos, 0,
                        SETHIGHLIGHT_START);
            }
            else if (GUIEDIT_SLIDER_RIGHT_SELECTOR == baseflex_ctrl_ptr->tpdown_slider
                     || MMITHEME_DRAGMODE_DEFAULT == baseflex_ctrl_ptr->flex_theme.drag_mode)
#endif
            {
                VTLBASEEDIT_SetHighlight (
                        baseedit_ctrl_ptr,
                        0, cursor_pos,
                        SETHIGHLIGHT_END);
            }

            // cursor will be between min_pos and max_pos while moving upword,
            // so check cursor and make it at the beginning of the highlight.
            BASEFLEX_CheckCursor (baseflex_ctrl_ptr);

            //display highlight
            BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

            //send move cursor msg
            BASEEDIT_NotifyCursorMsg (baseedit_ctrl_ptr);
        }

        //set display cursor
        baseedit_ctrl_ptr->cursor.is_display    = TRUE;
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

        //display cursor
        BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : is tp in edit rect,除去显示不完整的line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpInEditLine (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_POINT_T    tp_point,           //in:
    GUI_RECT_T     *top_rect_ptr,      //in/out:完整显示的top行区域
    GUI_RECT_T     *bottom_rect_ptr    //in/out:完整显示的bottom行区域
)
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_get_top = FALSE;
    uint16          i = 0;
    uint16          top_line_index = 0;
    uint16          bottom_line_index = 0;
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    top_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
    bottom_line_index = BASEEDIT_GetBottomLineIndex(baseedit_ctrl_ptr);

    for (i = top_line_index; i <= bottom_line_index; i++)
    {
        //get line rect
        line_rect = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, i);

        //get crossed rect
        if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, line_rect))
        {
            //获得完整显示的top行区域
            if ((!is_get_top) &&
                (line_rect.top == cross_rect.top) &&
                (line_rect.bottom == cross_rect.bottom))
            {
                *top_rect_ptr = cross_rect;
                is_get_top = TRUE;
            }

            //获得完整显示的bottom行区域
            if ((line_rect.top == cross_rect.top) &&
                (line_rect.bottom == cross_rect.bottom))
            {
                *bottom_rect_ptr = cross_rect;
            }

            //else
            //{
            //    break;
            //}
        }
        else
        {
            if (is_get_top)
            {
                break;
            }
        }
    }

    if (is_get_top)
    {
        //get edit all line rect
        cross_rect        = *top_rect_ptr;
        cross_rect.bottom = bottom_rect_ptr->bottom;

        if (GUI_PointIsInRect (tp_point, cross_rect))
        {
            result =  TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : edit first line is display all
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayFirstLine (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //the first page
    if (0 == baseedit_ctrl_ptr->top_line_offset)
    {
        //first line is display all
        if (baseedit_ctrl_ptr->is_rect_adaptive)
        {
            if (base_ctrl_ptr->display_rect.top <= base_ctrl_ptr->rect.top)
            {
                result = TRUE;
            }
        }
        else
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : edit last line is display all
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayLastLine (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    BOOLEAN             result = FALSE;
    int32               min_top_line_offset = 0;
    int32               max_top_line_offset = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //the last page
    BASEEDIT_GetTopLineOffsetRange(baseedit_ctrl_ptr, &min_top_line_offset, &max_top_line_offset);

    if (min_top_line_offset != max_top_line_offset)
    {
        if (baseedit_ctrl_ptr->top_line_offset == max_top_line_offset)
        {
            //last line is display all
            result = TRUE;
        }
    }
    else
    {
        if (baseedit_ctrl_ptr->is_rect_adaptive)
        {
            if (base_ctrl_ptr->display_rect.bottom >= base_ctrl_ptr->rect.bottom)
            {
                result = TRUE;
            }
        }
        else
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit cursor position for tp
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 SetEditCursorPos (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    GUI_POINT_T         *tp_point_ptr,          //in:
    BOOLEAN             *is_cursor_set_ptr      //out:
)
{
    BOOLEAN         is_set_cursor = TRUE;
    uint16          tp_line_index = 0;
    uint16          char_offset = 0;
    uint16          cursor_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //reset date cursor not display
    BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

    //get tp line index
    GetTpLineIndex (baseflex_ctrl_ptr, *tp_point_ptr, &tp_line_index);

    //get tp x offset
    char_offset = GetTpCharOffset (baseflex_ctrl_ptr, tp_line_index, *tp_point_ptr, PNULL);

    //set cursor position
    cursor_pos = (uint16) (baseedit_ctrl_ptr->line_info_ptr[tp_line_index].start_pos + char_offset);

#ifdef GUIF_CLIPBOARD
    if (GUIEDIT_SLIDER_LEFT_SELECTOR == baseflex_ctrl_ptr->tpdown_slider)
    {
        if (cursor_pos >= BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr))
        {
            is_set_cursor = FALSE;
        }
    }
    else if (GUIEDIT_SLIDER_RIGHT_SELECTOR == baseflex_ctrl_ptr->tpdown_slider)
    {
        if (cursor_pos <= BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr))
        {
            is_set_cursor = FALSE;
        }
    }
#endif

    if (is_set_cursor)
    {
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);

        baseedit_ctrl_ptr->cursor.x_coordinate  = GUIEDIT_CURSOR_INVALID_COORDINATE;

        //reset highlight not display
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);
    }

    if (PNULL != is_cursor_set_ptr)
    {
        *is_cursor_set_ptr = is_set_cursor;
    }

    return cursor_pos;
}

/*****************************************************************************/
//  Description : start tp scroll timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpScrollTimer (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_scroll_up    //in:
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;

    //include progress bar
    if ((0 == baseflex_ctrl_ptr->tp_scroll_timer_id) &&
        (MMK_IsFocusWin (base_ctrl_ptr->win_handle)))
    {
        baseflex_ctrl_ptr->tp_scroll_timer_id = MMK_CreateWinTimer (
                base_ctrl_ptr->handle,
                baseflex_ctrl_ptr->flex_theme.tp_scroll_period,
                FALSE);

        //set tp scroll direction
        baseflex_ctrl_ptr->is_tp_scroll_up = is_scroll_up;
    }
}

/*****************************************************************************/
//  Description : start tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpSlideTimer(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)baseflex_ctrl_ptr;

    //include progress bar
    if (0 == baseflex_ctrl_ptr->slide_timer_id)
    {
        baseflex_ctrl_ptr->slide_timer_id = MMK_CreateWinTimer(
                base_ctrl_ptr->handle,
                baseflex_ctrl_ptr->flex_theme.slide_period,
                FALSE);
    }
}

/*****************************************************************************/
//  Description : stop tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StopTpSlideTimer(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    if (0 < baseflex_ctrl_ptr->slide_timer_id)
    {
        MMK_StopTimer(baseflex_ctrl_ptr->slide_timer_id);
        baseflex_ctrl_ptr->slide_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : reset text slide display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetTpSlide(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_update
)
{
    uint16      i = 0;
    uint16      reset_slide_frame = 0;
    int32       relative_top = 0;
    int32       min_top_line_offset = 0;
    int32       max_top_line_offset = 0;
    int32       move_y_offset = 0;
    int32       scroll_step = 0;
    BOOLEAN     is_draw = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //init display top
    relative_top = baseedit_ctrl_ptr->top_line_offset;

    //get display max top
    BASEEDIT_GetTopLineOffsetRange(
        baseedit_ctrl_ptr,
        &min_top_line_offset,
        &max_top_line_offset);

    //is need reset
    if ((min_top_line_offset > relative_top) ||
        (max_top_line_offset < relative_top))
    {
#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMITHEME_GetFreqHandler(), FREQ_INDEX_MP3);
#endif

        if (min_top_line_offset > relative_top)
        {
            move_y_offset = min_top_line_offset - relative_top;
        }
        else
        {
            move_y_offset = max_top_line_offset - relative_top;
        }

        if (is_update)
        {
            reset_slide_frame = baseflex_ctrl_ptr->flex_theme.reset_slide_frame;
        }
        else
        {
            reset_slide_frame = baseflex_ctrl_ptr->flex_theme.reset_slide_start;
        }

        for (i=baseflex_ctrl_ptr->flex_theme.reset_slide_start; i<=reset_slide_frame; i++)
        {
            //calculate scroll step
            scroll_step = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,(int16)move_y_offset,reset_slide_frame,i);

            //set display top
            if (SetTopIndex(baseflex_ctrl_ptr, relative_top + scroll_step))
            {
                //update display
                VTLBASEEDIT_DisplayAll(baseedit_ctrl_ptr, TRUE);

                is_draw = TRUE;

                if (i != reset_slide_frame)
                {
                    //update
                    MMITHEME_UpdateRect();
                }
            }
        }

#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMITHEME_GetFreqHandler());
#endif
    }

    //for direct draw
    if (is_update && !is_draw)
    {
        //get top and bottom index
        if (min_top_line_offset > relative_top)
        {
            SetTopIndex(baseflex_ctrl_ptr, min_top_line_offset);
        }
        else if (relative_top > max_top_line_offset)
        {
            SetTopIndex(baseflex_ctrl_ptr, max_top_line_offset);
        }

        //update display
        VTLBASEEDIT_DisplayAll(baseedit_ctrl_ptr, TRUE);
    }
}

/*****************************************************************************/
//  Description : handle text tp slide timer msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleTpSlideTimer(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    GUI_POINT_T         tp_point = {0};
    MMI_TP_STATUS_E     tp_status = MMI_TP_NONE;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)baseflex_ctrl_ptr;

    //stop slide timer
    BASEFLEX_StopTpSlideTimer(baseflex_ctrl_ptr);

    if (MMK_IsFocusWin(base_ctrl_ptr->win_handle))
    {
        //start tp slide timer
        StartTpSlideTimer(baseflex_ctrl_ptr);
        
        if (MMK_TP_SLIDE_NONE != baseflex_ctrl_ptr->slide_state)
        {
            MMK_GetLogicTPMsg(&tp_status, &tp_point);

            //handle slide
            if ((!baseflex_ctrl_ptr->is_tp_down) ||
                IS_TP_MOVE_Y(baseflex_ctrl_ptr->prev_point.y, tp_point.y))
            {
                HandleTpSlide(baseflex_ctrl_ptr, tp_point);

                //set previous point
                baseflex_ctrl_ptr->prev_point = tp_point;
            }
        }
    }
}

/*****************************************************************************/
//  Description : handle text slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTpSlide(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_POINT_T         tp_point
)
{
    BOOLEAN     is_reset = FALSE;
    uint16      display_height = 0;
    int32       scroll_step = 0;
    int32       relative_top = 0;
    int32       max_relative_top = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //init display top
    relative_top = baseedit_ctrl_ptr->top_line_offset;

    switch (baseflex_ctrl_ptr->slide_state)
    {
    case MMK_TP_SLIDE_TP_SCROLL:
        scroll_step = tp_point.y - baseflex_ctrl_ptr->prev_point.y;

        relative_top = relative_top - scroll_step;
        break;

    case MMK_TP_SLIDE_FLING:
        //get fling offset
        scroll_step = MMK_GetFlingOffset(
                0,baseflex_ctrl_ptr->fling_velocity,
                (float)FLING_TIME,&baseflex_ctrl_ptr->fling_velocity);

        //get display height
        display_height = BASEEDIT_GetDispHeight(baseedit_ctrl_ptr);

        if (0 == baseflex_ctrl_ptr->fling_velocity)
        {
            //stop slide timer
            BASEFLEX_StopTpSlideTimer(baseflex_ctrl_ptr);

            //reset slide state
            baseflex_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

            is_reset = TRUE;
        }
        else if (0 > baseflex_ctrl_ptr->fling_velocity)//up
        {
            //set display top
            relative_top = relative_top - scroll_step;

            //get display max top
            BASEEDIT_GetTopLineOffsetRange(
                baseedit_ctrl_ptr,
                PNULL, &max_relative_top);
            //max_relative_top = GetMaxRelativeTop(baseflex_ctrl_ptr);

            //last item display 1/2 in display rect
            if (relative_top - max_relative_top > (display_height>>1))
            {
                //stop slide timer
                BASEFLEX_StopTpSlideTimer(baseflex_ctrl_ptr);

                //reset slide state
                baseflex_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

                relative_top = max_relative_top + (display_height>>1);

                is_reset = TRUE;
            }
        }
        else//down
        {
            //set display top
            relative_top = relative_top - scroll_step;

            //last item display 1/2 in display rect
            if ((0 > relative_top) && ((-relative_top) > (display_height>>1)))
            {
                //stop slide timer
                BASEFLEX_StopTpSlideTimer(baseflex_ctrl_ptr);

                //reset slide state
                baseflex_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

                relative_top = -(display_height>>1);

                is_reset = TRUE;
            }
        }
        break;

    default:
        break;
    }

    if (SetTopIndex(baseflex_ctrl_ptr, relative_top))
    {
        //update display
        VTLBASEEDIT_DisplayAll(baseedit_ctrl_ptr, TRUE);
    }

    //resest
    if (is_reset)
    {
        ResetTpSlide(baseflex_ctrl_ptr, TRUE);
    }
    else
    {
        UILAYER_SetDirectDraw(TRUE);
    }
}

/*****************************************************************************/
//  Description : set top index by relative display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetTopIndex(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    int32               relative_top
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (relative_top != baseedit_ctrl_ptr->top_line_offset)
    {
        baseedit_ctrl_ptr->top_line_offset = relative_top;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif



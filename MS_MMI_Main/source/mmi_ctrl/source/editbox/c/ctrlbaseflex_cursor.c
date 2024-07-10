/*****************************************************************************
** File Name:      ctrlbaseflex_cursor.c                                     *
** Author:         hua.fang                                                  *
** Date:           09/13/2012                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "ctrlbaseflex.h"
#include "mmi_gui_trc.h"
#include "spml_api.h"
#include "mmi_theme.h"

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
//  Description : upward or downward moving the cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveCursorUpDown (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_up
);

/*****************************************************************************/
//  Description : get new cursor position for up down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetNewCursorPosForUpDown (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_up
);

/*****************************************************************************/
//  Description : left or right moving the cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveCursorLeftRight (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_left
);

/*****************************************************************************/
//  Description : get new cursor position for left right msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNewCursorPosForLeftRight (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_left,
    BOOLEAN             *is_cursor_set_ptr
);

/*****************************************************************************/
//  Description : 是否需要设置光标
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetCursor (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,    //in
    uint16              cursor_pos
);

/*****************************************************************************/
//  Description : copy cursor background buffer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CopyCursorBg (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_RECT_T          cursor_rect
);

/*****************************************************************************/
//  Description : display cursor background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayCursorBg (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_RECT_T          cursor_rect
);

/*****************************************************************************/
//  Description : fill cursor color
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FillCursorColor (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              cursor_line_index,
    GUI_RECT_T          cursor_rect
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : start cursor timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StartCursorTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if ((PNULL != baseflex_ctrl_ptr) &&
        (MMK_IsFocusWin (base_ctrl_ptr->win_handle)) &&
        (0 == baseedit_ctrl_ptr->cursor.timer_id))
    {
        baseedit_ctrl_ptr->cursor.timer_id = MMK_CreateWinTimer (
                base_ctrl_ptr->handle,
                baseflex_ctrl_ptr->flex_theme.cursor_period,
                FALSE);
    }
}

/*****************************************************************************/
//  Description : stop cursor timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StopCursorTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseedit_ctrl_ptr &&
        0 < baseedit_ctrl_ptr->cursor.timer_id)
    {
        MMK_StopTimer (baseedit_ctrl_ptr->cursor.timer_id);
        baseedit_ctrl_ptr->cursor.timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle cursor timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleCursorTimer (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //stop cursor timer
    BASEFLEX_StopCursorTimer (baseflex_ctrl_ptr);

    //set cursor
    baseedit_ctrl_ptr->cursor.is_display = (BOOLEAN) (!baseedit_ctrl_ptr->cursor.is_display);

    //display cursor
    BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);
}

/*****************************************************************************/
//  Description : display edit cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_DisplayCursor (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    BOOLEAN             result = FALSE;
    uint16              cursor_line_index = 0;
    GUI_RECT_T          cursor_rect = {0};
    GUI_RECT_T          cross_rect 	= {0};
    GUI_RECT_T          client_rect = {0};
    GUI_RECT_T          edit_client_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if ((base_ctrl_ptr->is_active) &&
        (!baseedit_ctrl_ptr->cursor.is_hide))
    {
        //get cursor rect
        if (BASEEDIT_GetCursorRect (baseedit_ctrl_ptr, &cursor_line_index, &cursor_rect))
        {
            client_rect = MMITHEME_GetClientRectEx (base_ctrl_ptr->win_handle);
            edit_client_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);

            //get crossed rect
            if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, cursor_rect) &&
                GUI_IntersectRect (&cross_rect, client_rect, cross_rect) &&
                GUI_IntersectRect(&cross_rect, edit_client_rect, cross_rect))
            {
                if (VTLBASE_GetVisible ( (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr))
                {
                    //display cursor
                    if (baseedit_ctrl_ptr->cursor.is_display)
                    {
                        //copy bg
                        CopyCursorBg (baseflex_ctrl_ptr, cross_rect);

                        //fill cursor color
                        FillCursorColor (baseflex_ctrl_ptr, cursor_line_index, cross_rect);
                    }
                    else
                    {
                        //display bg
                        DisplayCursorBg (baseflex_ctrl_ptr, cross_rect);
                    }
                }

                //start cursor timer
                BASEFLEX_StartCursorTimer (baseflex_ctrl_ptr);

                result = TRUE;
            }
        }
    }

    if (!result)
    {
        baseedit_ctrl_ptr->cursor.is_display = FALSE;
    }
}

/*****************************************************************************/
//  Description : reset don't display cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_ResetCursor (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (!baseedit_ctrl_ptr->cursor.is_hide)
    {
        if (baseedit_ctrl_ptr->cursor.is_display)
        {
            baseedit_ctrl_ptr->cursor.is_display = FALSE;
            baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
            BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);
        }

        //stop cursor timer
        BASEFLEX_StopCursorTimer (baseflex_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : get new cursor position for left right msg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEFLEX_SetCursorPosForLeftRight (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    BOOLEAN                 is_left,
    BOOLEAN                 *is_cursor_set_ptr
)
{
    return GetNewCursorPosForLeftRight (baseflex_ctrl_ptr, is_left, is_cursor_set_ptr);
}

/*****************************************************************************/
//  Description : adjust cursor position
//  Global resource dependence :
//  Author: jassmine
//  Note: move cursor,delete char for multi language
/*****************************************************************************/
PUBLIC void BASEFLEX_AdjustCursorPos (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,	//in:
    BOOLEAN			        is_left,		    //in:
    uint16			        *cursor_pos_ptr	    //out:
)
{
    wchar			    *str_ptr = PNULL;
    uint16			    start_pos = 0;
    uint16			    str_len = 0;
    uint16			    line_index = 0;
    int32			    cursor_offset = 0;
    GUI_RECT_T		    text_rect = {0};
    SPML_TEXT_DIR_T	    text_direction = TEXT_DIR_LTR;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get text rect
    text_rect = BASEEDIT_GetTextRect (
            baseedit_ctrl_ptr,
            BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr));

    //get cursor line index
    line_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, *cursor_pos_ptr);

    if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
    {
        //set line start pos and cursor offset
        if ( (0 < *cursor_pos_ptr) && (is_left))
        {
            start_pos	  = (uint16) (*cursor_pos_ptr - 1);
            cursor_offset = 1;
        }
        else
        {
            start_pos	  = *cursor_pos_ptr;
            cursor_offset = 0;
        }

        //get display string
        str_len = (uint16) (baseedit_ctrl_ptr->str_len - start_pos);
    }
    else
    {
        //set line start pos
        start_pos = baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos;

        //set cursor offset
        cursor_offset = *cursor_pos_ptr - start_pos;

        //get display string
        str_len = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, line_index);
    }

    str_ptr = VTLBASEEDIT_GetDispStr (
                  baseedit_ctrl_ptr,
                  start_pos,
                  str_len);

    //set line text direction
    if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
    {
        text_direction = SPMLAPI_GetTextDirection (str_ptr, str_len);
    }
    else
    {
        text_direction = baseedit_ctrl_ptr->line_info_ptr[line_index].direction;
    }

    //adjust cursor position
    GUI_GetCursorInfo (
            BASEEDIT_GetCursorAlign (baseedit_ctrl_ptr),
            &text_rect,
            text_direction,
            str_ptr,
            str_len,
            is_left,
            baseedit_ctrl_ptr->common_theme.font.font,
            baseedit_ctrl_ptr->common_theme.char_space,
            &cursor_offset);

    *cursor_pos_ptr = (uint16) (start_pos + cursor_offset);
}

/*****************************************************************************/
//  Description : move cursor and update cursor,string etc
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_MoveCursor (//is change
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,
    GUIEDIT_CURSOR_MOVE_E       move_direction
)
{
    BOOLEAN     result = FALSE;

    switch (move_direction)
    {
    case GUIEDIT_CURSOR_MOVE_UP:
        result = MoveCursorUpDown (baseflex_ctrl_ptr, TRUE);
        break;

    case GUIEDIT_CURSOR_MOVE_DOWN:
        result = MoveCursorUpDown (baseflex_ctrl_ptr, FALSE);
        break;

    case GUIEDIT_CURSOR_MOVE_LEFT:
        result = MoveCursorLeftRight (baseflex_ctrl_ptr, TRUE);
        break;

    case GUIEDIT_CURSOR_MOVE_RIGHT:
        result = MoveCursorLeftRight (baseflex_ctrl_ptr, FALSE);
        break;

    default:
        //SCI_TRACE_LOW:"GUIEDIT_MoveCursor:move_direction %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_CURSOR_575_112_2_18_3_15_58_128, (uint8*) "d", move_direction);
        break;
    }

#ifdef GUIF_CLIPBOARD
    if (result)
    {
        BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
    }

#endif

    return (result);
}

/*****************************************************************************/
//  Description : check cursor position
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_CheckCursor (
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (baseedit_ctrl_ptr->cursor.cur_pos != baseedit_ctrl_ptr->highlight.start_pos &&
        baseedit_ctrl_ptr->cursor.cur_pos != baseedit_ctrl_ptr->highlight.end_pos)
    {
        baseedit_ctrl_ptr->cursor.cur_pos = baseedit_ctrl_ptr->highlight.end_pos;

        if (baseedit_ctrl_ptr->cursor.cur_pos > baseedit_ctrl_ptr->str_len)
        {
            baseedit_ctrl_ptr->cursor.cur_pos = baseedit_ctrl_ptr->str_len;
        }
    }

#endif
}

/*****************************************************************************/
//  Description : set edit top line index,for add,delete and move cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_MakeCursorVisible (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    uint16              line_height = 0;
    uint16              display_height = 0;
    uint16              cursor_line_index = 0;
    int32               cursor_offset = 0;
    int32               min_top_line_offset = 0;
    int32               max_top_line_offset = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //is auto adjust edit rect,yes top line index is 0
    if (!baseedit_ctrl_ptr->is_rect_adaptive)
    {
        BASEEDIT_GetTopLineOffsetRange(
                baseedit_ctrl_ptr,
                &min_top_line_offset,
                &max_top_line_offset);

        line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);
        display_height = BASEEDIT_GetDispHeight(baseedit_ctrl_ptr);

        //get current cursor line index
        cursor_line_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

        cursor_offset = cursor_line_index * line_height;

        if (min_top_line_offset + cursor_offset < baseedit_ctrl_ptr->top_line_offset)
        {
            baseedit_ctrl_ptr->top_line_offset = min_top_line_offset + cursor_offset;
        }
        else if (min_top_line_offset + cursor_offset + line_height >= baseedit_ctrl_ptr->top_line_offset + display_height)
        {
            baseedit_ctrl_ptr->top_line_offset = min_top_line_offset + cursor_offset + line_height - display_height;
        }

        if (baseedit_ctrl_ptr->top_line_offset < min_top_line_offset)
        {
            baseedit_ctrl_ptr->top_line_offset = min_top_line_offset;
        }
        else if (baseedit_ctrl_ptr->top_line_offset > max_top_line_offset)
        {
            baseedit_ctrl_ptr->top_line_offset = max_top_line_offset;
        }
    }
}

/*****************************************************************************/
//  Description : upward or downward moving the cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveCursorUpDown (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_up
)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_need_handle = TRUE;
    BOOLEAN     is_update_str = FALSE;
    uint16		prev_cursor_pos = 0;
    uint16      cursor_line_index = 0;
    uint16      prev_top_line_offset = 0;
    uint16      min_highlight_pos = 0;
    uint16      max_highlight_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get previous cursor position
    prev_cursor_pos = baseedit_ctrl_ptr->cursor.cur_pos;

    //reset date cursor not display
    BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

    //have highlight
    if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
    {
        //reset highlight not display
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

        if (is_up)
        {
            min_highlight_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);

            BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, min_highlight_pos);
        }
        else
        {
            max_highlight_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

            BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, max_highlight_pos);
        }

        result = TRUE;
    }

    if (1 < baseedit_ctrl_ptr->total_line_num)
    {
        //get cursor line index
        cursor_line_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

        if (is_up)
        {
            //is need handle
            if ((!baseedit_ctrl_ptr->common_theme.is_cir_handle_ud) &&
                (0 == cursor_line_index))
            {
                is_need_handle = FALSE;
            }
        }
        else
        {
            if ((!baseedit_ctrl_ptr->common_theme.is_cir_handle_ud) &&
                (cursor_line_index == (baseedit_ctrl_ptr->total_line_num - 1)))
            {
                is_need_handle = FALSE;
            }
        }

        if (is_need_handle)
        {
            //get previous top line index
            prev_top_line_offset = baseedit_ctrl_ptr->top_line_offset;

            //get new cursor position
            GetNewCursorPosForUpDown (baseflex_ctrl_ptr, is_up);

            //set top line index
            // start_pos与end_pos不等，只有在文本对象的时候出现
            if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
            {
                // 这里先临时将cur_pos置为start_pos，保证start_pos也出现在可视区之内
                baseedit_ctrl_ptr->cursor.cur_pos = baseedit_ctrl_ptr->highlight.start_pos;
                BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);

                baseedit_ctrl_ptr->cursor.cur_pos = baseedit_ctrl_ptr->highlight.end_pos;
            }

            BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);

            if (prev_top_line_offset != baseedit_ctrl_ptr->top_line_offset)
            {
                //need update edit all string
                is_update_str = TRUE;
            }

            result = TRUE;
        }
    }

    //update text display for move cursor
    BASEFLEX_UpdateForMoveCursor (baseflex_ctrl_ptr, is_update_str, prev_cursor_pos);

    return (result);
}

/*****************************************************************************/
//  Description : get new cursor position for up down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetNewCursorPosForUpDown (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_up
)
{
    wchar       *str_ptr = PNULL;
    uint16      str_len = 0;
    uint16      start_pos = 0;
    uint16      char_num = 0;
    uint16      cursor_line_index = 0;
    uint16      prev_cursor_line_index = 0;
    uint16      cursor_pos = 0;
    GUI_RECT_T  text_rect = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get text rect
    text_rect = BASEEDIT_GetTextRect (
            baseedit_ctrl_ptr,
            BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr));

    //get cursor line index
    cursor_line_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);
    prev_cursor_line_index = cursor_line_index;

    //get current cursor x coordinate
    if (GUIEDIT_CURSOR_INVALID_COORDINATE == baseedit_ctrl_ptr->cursor.x_coordinate)
    {
        //get cursor left
        baseedit_ctrl_ptr->cursor.x_coordinate = BASEEDIT_GetLeftCoordinate (
                baseedit_ctrl_ptr,
                cursor_line_index,
                baseedit_ctrl_ptr->cursor.cur_pos,
                text_rect);
    }

    //get new cursor line index
    if (is_up)
    {
        //move cursor up
        if (0 < cursor_line_index)
        {
            cursor_line_index--;
        }
        else
        {
            cursor_line_index = (uint16) (baseedit_ctrl_ptr->total_line_num - 1);
        }
    }
    else
    {
        //move cursor down
        if (cursor_line_index < (baseedit_ctrl_ptr->total_line_num - 1))
        {
            cursor_line_index++;
        }
        else
        {
            cursor_line_index = 0;
        }
    }

    //get new cursor line string
    start_pos = baseedit_ctrl_ptr->line_info_ptr[cursor_line_index].start_pos;

    //get string len in the new cursor line
    str_len = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, cursor_line_index);

    str_ptr = VTLBASEEDIT_GetDispStr (
                  baseedit_ctrl_ptr,
                  baseedit_ctrl_ptr->line_info_ptr[cursor_line_index].start_pos,
                  str_len);

    char_num = GUI_GetCursorPos (
                   BASEEDIT_GetCursorAlign (baseedit_ctrl_ptr),
                   &text_rect,
                   baseedit_ctrl_ptr->line_info_ptr[cursor_line_index].direction,
                   str_ptr,
                   str_len,
                   (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - baseedit_ctrl_ptr->line_info_ptr[prev_cursor_line_index].start_pos),
                   baseedit_ctrl_ptr->cursor.x_coordinate,
                   baseedit_ctrl_ptr->common_theme.font.font,
                   baseedit_ctrl_ptr->common_theme.char_space,
                   PNULL);

    SCI_ASSERT (char_num <= str_len); /*assert verified*/

    //the char before new cursor is LR or CR
    if ((0 == char_num) &&
        (0 < start_pos) &&
        (!BASEEDIT_IsNewLineChar (baseedit_ctrl_ptr, start_pos)))
    {
        char_num = 1;
    }
    else if ((0 < char_num) &&
             (BASEEDIT_IsNewLineChar (baseedit_ctrl_ptr, start_pos + char_num)))
    {
        char_num--;
    }

    //set current cursor position
    cursor_pos = (uint16) (baseedit_ctrl_ptr->line_info_ptr[cursor_line_index].start_pos + char_num);

    VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);

    //set no highlight
    BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, cursor_pos);
}

/*****************************************************************************/
//  Description : left or right moving the cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveCursorLeftRight (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_left
)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_need_handle = TRUE;
    BOOLEAN     is_update_str = FALSE;
    uint16		prev_cursor_pos = 0;
    uint16      prev_top_line_offset = 0;
    uint16      prev_first_char_index = 0;
    uint16      cursor_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (0 < baseedit_ctrl_ptr->str_len)
    {
        //get previous cursor position
        prev_cursor_pos = baseedit_ctrl_ptr->cursor.cur_pos;

        //reset date cursor not display
        BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

        //have highlight
        if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
        {
            //reset highlight not display
            BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

            //set cursor position
            if (is_left)
            {
                cursor_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
            }
            else
            {
                cursor_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);
            }

            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);

            result = TRUE;
        }
        else
        {
            //is need handle
            if (is_left)
            {
                if ((!baseedit_ctrl_ptr->common_theme.is_cir_handle_lf) &&
                    (0 == baseedit_ctrl_ptr->cursor.cur_pos))
                {
                    is_need_handle = FALSE;
                }
            }
            else
            {
                if ((!baseedit_ctrl_ptr->common_theme.is_cir_handle_lf) &&
                    (baseedit_ctrl_ptr->cursor.cur_pos == baseedit_ctrl_ptr->str_len))
                {
                    is_need_handle = FALSE;
                }
            }

            if (is_need_handle)
            {
                //get previous top line index and first char index
                prev_top_line_offset = baseedit_ctrl_ptr->top_line_offset;
                prev_first_char_index = baseedit_ctrl_ptr->line_info_ptr[0].start_pos;

                //get new cursor position
                cursor_pos = GetNewCursorPosForLeftRight (baseflex_ctrl_ptr, is_left, PNULL);

                if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
                {
                    if (prev_first_char_index != baseedit_ctrl_ptr->line_info_ptr[0].start_pos)
                    {
                        //need update edit all string
                        is_update_str = TRUE;
                    }
                }
                else
                {
                    //set top line index
                    BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);

                    if (prev_top_line_offset != baseedit_ctrl_ptr->top_line_offset)
                    {
                        //need update edit all string
                        is_update_str = TRUE;
                    }
                }

                result = TRUE;
            }
        }

        //set no highlight
        if (is_need_handle)
        {
            BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, cursor_pos);

            // start_pos与end_pos不等，只有在文本对象的时候出现
            if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
            {
                // 这里先临时将cur_pos置为start_pos，保证start_pos也出现在可视区之内
                baseedit_ctrl_ptr->cursor.cur_pos = baseedit_ctrl_ptr->highlight.start_pos;
                BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);

                baseedit_ctrl_ptr->cursor.cur_pos = baseedit_ctrl_ptr->highlight.end_pos;
                BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);

                if (prev_top_line_offset != baseedit_ctrl_ptr->top_line_offset)
                {
                    //need update edit all string
                    is_update_str = TRUE;
                }
            }
        }

        //update text display for move cursor
        BASEFLEX_UpdateForMoveCursor (baseflex_ctrl_ptr, is_update_str, prev_cursor_pos);

        //reset cursor x coordinate
        baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get new cursor position for left right msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNewCursorPosForLeftRight (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_left,
    BOOLEAN             *is_cursor_set_ptr
)
{
    BOOLEAN is_set_curosr = FALSE;
    uint16 cursor_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (is_left)
    {
        //move cursor left
        if (0 < baseedit_ctrl_ptr->cursor.cur_pos)
        {
            cursor_pos = (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 1);

            //adjust cursor position
            BASEFLEX_AdjustCursorPos (baseflex_ctrl_ptr, TRUE, &cursor_pos);

            is_set_curosr = IsNeedSetCursor (baseflex_ctrl_ptr, cursor_pos);

            if (is_set_curosr)
            {
                //set cursor position
                VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);

                //set first char index
                if ((GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style) &&
                    (baseedit_ctrl_ptr->line_info_ptr[0].start_pos > baseedit_ctrl_ptr->cursor.cur_pos))
                {
                    baseedit_ctrl_ptr->line_info_ptr[0].start_pos = baseedit_ctrl_ptr->cursor.cur_pos;
                    BASEFLEX_GetFirstCharIndex (baseflex_ctrl_ptr);
                }
            }
        }
        else
        {
            cursor_pos = baseedit_ctrl_ptr->str_len;

            is_set_curosr = IsNeedSetCursor (baseflex_ctrl_ptr, cursor_pos);

            if (is_set_curosr)
            {
                VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);

                //set first char index
                if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
                {
                    BASEFLEX_GetFirstCharIndex (baseflex_ctrl_ptr);
                }
            }
        }
    }
    else
    {
        //move cursor right
        if (baseedit_ctrl_ptr->cursor.cur_pos < baseedit_ctrl_ptr->str_len)
        {
            cursor_pos = (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1);

            //adjust cursor position
            BASEFLEX_AdjustCursorPos (baseflex_ctrl_ptr, FALSE, &cursor_pos);

            is_set_curosr = IsNeedSetCursor (baseflex_ctrl_ptr, cursor_pos);

            if (is_set_curosr)
            {
                //set cursor position
                VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);

                //set first char index
                if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
                {
                    BASEFLEX_GetFirstCharIndex (baseflex_ctrl_ptr);
                }
            }
        }
        else
        {
            cursor_pos = 0;

            is_set_curosr = IsNeedSetCursor (baseflex_ctrl_ptr, cursor_pos);

            if (is_set_curosr)
            {
                VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cursor_pos);

                //set first char index
                if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
                {
                    baseedit_ctrl_ptr->line_info_ptr[0].start_pos = 0;
                    BASEFLEX_GetFirstCharIndex (baseflex_ctrl_ptr);
                }
            }
        }
    }

    if (PNULL != is_cursor_set_ptr)
    {
        *is_cursor_set_ptr = is_set_curosr;
    }

    return (cursor_pos);
}

/*****************************************************************************/
//  Description : 是否需要设置光标
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetCursor (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,    //in
    uint16              cursor_pos
)
{
    BOOLEAN is_set_cursor = TRUE;

#ifdef GUIF_CLIPBOARD
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

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

    return is_set_cursor;
}

/*****************************************************************************/
//  Description : copy cursor background buffer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CopyCursorBg (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_RECT_T          cursor_rect
)
{
    uint16          cursor_width = 0;
    uint16          cursor_height = 0;
    uint32          cursor_size = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get cursor width,height and size
    cursor_width  = (uint16) (cursor_rect.right - cursor_rect.left + 1);
    cursor_height = (uint16) (cursor_rect.bottom - cursor_rect.top + 1);
    cursor_size   = cursor_width * cursor_height;
    //SCI_ASSERT(GUIEDIT_CURSOR_DEFAULT_WIDTH == cursor_width); /*assert verified*/ // maybe smaller when not in display

    //buffer size is same
    if ( (cursor_size != baseedit_ctrl_ptr->cursor.bg_buf_size) &&
            (PNULL != baseedit_ctrl_ptr->cursor.bg_buf_ptr))
    {
        //free bg buffer
        SCI_FREE (baseedit_ctrl_ptr->cursor.bg_buf_ptr);
        baseedit_ctrl_ptr->cursor.bg_buf_size = 0;
    }

    if (PNULL == baseedit_ctrl_ptr->cursor.bg_buf_ptr)
    {
        //alloc bg buffer
        baseedit_ctrl_ptr->cursor.bg_buf_ptr  = SCI_ALLOC_APP (cursor_size * sizeof (GUI_COLOR_T));
        SCI_MEMSET (baseedit_ctrl_ptr->cursor.bg_buf_ptr, 0, (cursor_size*sizeof (GUI_COLOR_T)));
        baseedit_ctrl_ptr->cursor.bg_buf_size = cursor_size;
    }

    //copy buffer
    LCD_CopyLayerBufToMem (
        &base_ctrl_ptr->lcd_dev_info,
        baseedit_ctrl_ptr->cursor.bg_buf_ptr,
        cursor_width,
        cursor_height,
        cursor_rect.left,
        cursor_rect.top);
}

/*****************************************************************************/
//  Description : display cursor background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayCursorBg (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUI_RECT_T          cursor_rect
)
{
    uint16          cursor_width = 0;
    uint16          cursor_height = 0;
    uint32          cursor_size = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get cursor width,height and size
    cursor_width  = (uint16) (cursor_rect.right - cursor_rect.left + 1);
    cursor_height = (uint16) (cursor_rect.bottom - cursor_rect.top + 1);
    cursor_size   = cursor_width * cursor_height;

    if ( (PNULL != baseedit_ctrl_ptr->cursor.bg_buf_ptr) &&
            //(GUIEDIT_CURSOR_DEFAULT_WIDTH == cursor_width) &&     // maybe smaller when not in display
            (baseedit_ctrl_ptr->cursor.bg_buf_size == cursor_size))
    {
        //display bg
        LCD_CopyMemToLayerBuf (
            &base_ctrl_ptr->lcd_dev_info,
            baseedit_ctrl_ptr->cursor.bg_buf_ptr,
            cursor_width,
            cursor_height,
            cursor_rect.left,
            cursor_rect.top);

        //store rect
        MMITHEME_StoreUpdateRect ( (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info, cursor_rect);
    }
}

/*****************************************************************************/
//  Description : fill cursor color
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FillCursorColor (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              cursor_line_index,
    GUI_RECT_T          cursor_rect
)
{
    uint16          highlight_len = 0;
    GUI_RECT_T      cursor_rect1 = {0};
    GUI_RECT_T      cursor_rect2 = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get highlight len
    highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    if ( (baseedit_ctrl_ptr->highlight.is_underline) ||
            (0 == highlight_len) ||
            (baseedit_ctrl_ptr->cursor.cur_pos == baseedit_ctrl_ptr->line_info_ptr[cursor_line_index].start_pos))
    {
        GUI_FillRect (
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
            cursor_rect,
            baseedit_ctrl_ptr->common_theme.font.color);
    }
    else
    {
        cursor_rect1 = cursor_rect;
        cursor_rect2 = cursor_rect;

        if (BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr) == baseedit_ctrl_ptr->cursor.cur_pos)
        {
            //set cursor rect1,cursor color
            cursor_rect1.right--;

            //set cursor rect2,~highlight color
            cursor_rect2.left++;
        }
        else
        {
            //set cursor rect1,cursor color
            cursor_rect1.left++;

            //set cursor rect2,~highlight color
            cursor_rect2.right--;
        }

        //fill cursor color
        GUI_FillRect (
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
            cursor_rect1,
            baseedit_ctrl_ptr->common_theme.font.color);

        //fill !font color
        GUI_FillRect (
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
            cursor_rect2,
            (GUI_COLOR_T) (~baseedit_ctrl_ptr->common_theme.font.color));
    }
}



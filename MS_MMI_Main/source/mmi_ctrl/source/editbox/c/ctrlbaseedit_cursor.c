/*****************************************************************************
** File Name:      ctrledit_cursor.c                                         *
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



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "spml_api.h"
#include "ctrlbaseedit.h"
#include "guifont.h"
#include "mmi_theme.h"

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

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_GetCursorRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     //in
    uint16              *cursor_line_ptr,       //in/out:may PNULL
    GUI_RECT_T          *cursor_rect_ptr        //in/out:
)
{
    BOOLEAN         result = FALSE;
    uint16          cursor_line_index = 0;
    uint16          top_line_index = 0;
    uint16          bottom_line_index = 0;
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      cursor_rect = {0};

    //get current cursor line index
    cursor_line_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);
    top_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
    bottom_line_index = BASEEDIT_GetBottomLineIndex(baseedit_ctrl_ptr);

    //judge cursor is in current page
    if ((cursor_line_index >= top_line_index) &&
        (cursor_line_index <= bottom_line_index))
    {
        if (baseedit_ctrl_ptr->cursor.is_rect_valid)
        {
            cursor_rect = baseedit_ctrl_ptr->cursor.rect;
        }
        else
        {
            //get current cursor line text rect
            text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, cursor_line_index);

            //set cursor rect
            cursor_rect = text_rect;

            // 用对齐方式，调用GUI_GetCusorCoordinate时自动算，此处不用处理
            ////Adjust cursor rect left position by align
            //BASEEDIT_AdjustLeftByAlign(edit_ctrl_ptr,cursor_line_index,&cursor_rect);

            //get cursor rect left
            cursor_rect.left = BASEEDIT_GetLeftCoordinate (
                    baseedit_ctrl_ptr,
                    cursor_line_index,
                    baseedit_ctrl_ptr->cursor.cur_pos,
                    cursor_rect);

            //set cursor right
            cursor_rect.right = (int16) (cursor_rect.left + GUIEDIT_CURSOR_DEFAULT_WIDTH - 1);

            //set cursor rect
            baseedit_ctrl_ptr->cursor.is_rect_valid = TRUE;
            baseedit_ctrl_ptr->cursor.rect          = cursor_rect;
        }

        result = TRUE;
    }

    //set cursor rect
    if (PNULL != cursor_rect_ptr)
    {
        *cursor_rect_ptr = cursor_rect;
    }

    if (PNULL != cursor_line_ptr)
    {
        *cursor_line_ptr = cursor_line_index;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_GetPosRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     //in
    uint16              pos,                    //in
    GUI_RECT_T          *rect_ptr               //out
)
{
    BOOLEAN         result = FALSE;
    uint16          line_index = 0;
    uint16          top_line_index = 0;
    uint16          bottom_line_index = 0;
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      rect = {0};

    //get current cursor line index
    line_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, pos);
    top_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
    bottom_line_index = BASEEDIT_GetBottomLineIndex(baseedit_ctrl_ptr);

    //judge cursor is in current page
    if ((line_index >= top_line_index) &&
        (line_index <= bottom_line_index))
    {
        //get current cursor line text rect
        text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, line_index);

        //set cursor rect
        rect = text_rect;

        // 用对齐方式，调用GUI_GetCusorCoordinate时自动算，此处不用处理
        ////Adjust cursor rect left position by align
        //BASEEDIT_AdjustLeftByAlign(edit_ctrl_ptr,line_index,&rect);

        //get cursor rect left
        rect.left = BASEEDIT_GetLeftCoordinate (baseedit_ctrl_ptr, line_index, pos, rect);

        //set cursor right
        rect.right = (int16) (rect.left + GUIEDIT_CURSOR_DEFAULT_WIDTH - 1);

        result = TRUE;
    }

    //set cursor rect
    if (PNULL != rect_ptr)
    {
        *rect_ptr = rect;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get current cursor line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:除第一行,光标前一个字符是换行符之外，光标不能显示在行首位置
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetCursorLineIndex (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              cursor_pos
)
{
    BOOLEAN     is_new_line = FALSE;
    BOOLEAN		is_highlight_end = FALSE;
    uint16      highlight_len = 0;
    uint16		max_pos = 0;
    uint16      i = 0;
    uint16      line_index = 0;

    SCI_ASSERT (0 < baseedit_ctrl_ptr->total_line_num); /*assert verified*/

    //the char before cursor is new line char
    is_new_line = BASEEDIT_IsNewLineChar (baseedit_ctrl_ptr, cursor_pos);

    //get highlight len
    highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    //set highlight max position
    max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

    if ((0 < highlight_len) && (cursor_pos == max_pos))
    {
        is_highlight_end = TRUE;
    }

    if ((is_new_line) || ((0 < highlight_len) && (!is_highlight_end)))
    {
        if (cursor_pos >= baseedit_ctrl_ptr->line_info_ptr[baseedit_ctrl_ptr->total_line_num - 1].start_pos)
        {
            line_index = (uint16) (baseedit_ctrl_ptr->total_line_num - 1);
        }
        else
        {
            //get line index
            for (i = 0; i < baseedit_ctrl_ptr->total_line_num; i++)
            {
                if ((cursor_pos >= baseedit_ctrl_ptr->line_info_ptr[i].start_pos) &&
                    (cursor_pos < baseedit_ctrl_ptr->line_info_ptr[i+1].start_pos))
                {
                    line_index = i;
                    break;
                }
            }
        }
    }
    else
    {
        if (0 == cursor_pos)
        {
            line_index = 0;
        }
        else if (cursor_pos > baseedit_ctrl_ptr->line_info_ptr[baseedit_ctrl_ptr->total_line_num - 1].start_pos)
        {
            //is last line
            line_index = (uint16) (baseedit_ctrl_ptr->total_line_num - 1);
        }
        else
        {
            //get line index
            for (i = 0; i < baseedit_ctrl_ptr->total_line_num; i++)
            {
                if ((cursor_pos > baseedit_ctrl_ptr->line_info_ptr[i].start_pos) &&
                    (cursor_pos <= baseedit_ctrl_ptr->line_info_ptr[i+1].start_pos))
                {
                    line_index = i;
                    break;
                }
            }
        }
    }

    return (line_index);
}

/*****************************************************************************/
//  Description : 光标的对齐方式
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_ALIGN_E BASEEDIT_GetCursorAlign (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    GUI_ALIGN_E align = ALIGN_LEFT;
    GUI_ALIGN_E text_align = VTLBASEEDIT_GetAlign (baseedit_ctrl_ptr);

    switch (text_align)
    {
    case ALIGN_HMIDDLE:
    case ALIGN_HVMIDDLE:
        align = ALIGN_HMIDDLE;
        break;

    case ALIGN_RIGHT:
    case ALIGN_RVMIDDLE:
        align = ALIGN_RIGHT;
        break;

    default:
        align = ALIGN_LEFT;
        break;
    }

    return align;
}

/*****************************************************************************/
//  Description : get cursor left coordinate
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int16 BASEEDIT_GetLeftCoordinate (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in
    uint16              line_index,         //in
    uint16              pos,                //in
    GUI_RECT_T          text_rect           //in
)
{
    int16           left_coordinate = text_rect.left;
    uint16          line_str_len = 0;
    wchar           *str_ptr = PNULL;

    //get line string length
    line_str_len = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, line_index);

    if (pos < baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos ||
        pos > baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos + line_str_len)
    {
        // 如果当前位置不可见，将位置置于显示区域之外
        left_coordinate = -GUIEDIT_CURSOR_DEFAULT_WIDTH;
    }
    else
    {
        str_ptr = VTLBASEEDIT_GetDispStr (
                      baseedit_ctrl_ptr,
                      baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos,
                      line_str_len);

        //set cursor left by text rect
        left_coordinate = GUI_GetCusorCoordinate (
                              BASEEDIT_GetCursorAlign (baseedit_ctrl_ptr),
                              &text_rect,
                              baseedit_ctrl_ptr->line_info_ptr[line_index].direction,
                              str_ptr,
                              line_str_len,
                              (uint16) (pos - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos),
                              baseedit_ctrl_ptr->common_theme.font.font,
                              baseedit_ctrl_ptr->common_theme.char_space);

        //adjust cursor rect left:need adjust left coordinate both the leftmost and rightmost postion
        if (pos == baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos)
        {
            //调整行首光标显示在页边的空白
            if (left_coordinate >= text_rect.right)
            {
                left_coordinate = text_rect.right;
            }
            else if (left_coordinate < text_rect.left + GUIEDIT_CURSOR_DEFAULT_WIDTH)
            {
                left_coordinate = text_rect.left;
            }
            else
            {
                left_coordinate = (int16) (left_coordinate - GUIEDIT_CURSOR_DEFAULT_WIDTH);
            }
        }
		else if (pos == baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos + line_str_len)
		{
			if (left_coordinate >= text_rect.right )
			{
				left_coordinate = text_rect.right;
			}
		}
    }

    return (left_coordinate);
}

/*****************************************************************************/
//  Description : set cursor position
//  Global resource dependence :
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BaseEditCtrlSetCursorPos (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,	//in:
    uint16			    cur_pos			    //in:from 0
)
{
    if (cur_pos > baseedit_ctrl_ptr->str_len)
    {
        cur_pos = baseedit_ctrl_ptr->str_len;
    }

    baseedit_ctrl_ptr->cursor.cur_pos = cur_pos;
}


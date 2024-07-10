/*****************************************************************************
** File Name:      ctrltext_internal.c                                       *
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
#include "ctrltext_export.h"
#include "ctrltext.h"
#include "guires.h"
#include "guistring.h"
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
/*****************************************************************************/
//  Description : get top offset, adjust top by align
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int32 GetTextTopOffset(
                             CTRLTEXT_OBJ_T *text_ctrl_ptr  //in:
                             );

/*****************************************************************************/
//  Description : get current cursor line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:除第一行,光标前一个字符是换行符之外，光标不能显示在行首位置
/*****************************************************************************/
PUBLIC uint16 GetCursorLineIndex(
                                 uint16         cursor_pos,
                                 CTRLTEXT_OBJ_T *text_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : get cursor left coordinate
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetLeftCoordinate(
                              uint16          line_index,  //in:
                              uint16             pos,
                              GUI_RECT_T      text_rect,          //in:
                              CTRLTEXT_OBJ_T  *text_ctrl_ptr      //in
                              );

/*****************************************************************************/
//  Description : adjust cursor/highlight rect left position by align
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustLeftByAlign(
                             uint16            line_index,     //in:
                             GUI_RECT_T        *rect_ptr,       //in/out:
                             CTRLTEXT_OBJ_T    *text_ctrl_ptr   //in:
                             );

/*****************************************************************************/
//  Description : get line string pixel
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTextLineStrPixel(
                                 uint16         line_index,     //in:
                                 uint16         rect_width,     //in:
                                 CTRLTEXT_OBJ_T *text_ctrl_ptr  //in:
                                 );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : display text background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_DisplayBg(
                              GUI_RECT_T        bg_rect,
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr
                              )
{
    GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //set image rect
    img_rect.left   = (int16)(bg_rect.left - base_ctrl_ptr->rect.left);
    img_rect.top    = (int16)(bg_rect.top - base_ctrl_ptr->rect.top);
    img_rect.right  = (int16)(img_rect.left + bg_rect.right - bg_rect.left);
    img_rect.bottom = (int16)(img_rect.top + bg_rect.bottom - bg_rect.top);

    //set bg display info
    bg_display.is_transparent = FALSE;
    bg_display.rect           = base_ctrl_ptr->rect;
    bg_display.display_rect   = bg_rect;
    bg_display.img_rect       = img_rect;
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = text_ctrl_ptr->win_handle;

    //display bg
    GUI_DisplayBg(&text_ctrl_ptr->theme.bg,
        &bg_display,
        &base_ctrl_ptr->lcd_dev_info);
}

/*****************************************************************************/
//  Description : get text rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T GUITEXT_GetRect(
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                  )
{
    BOOLEAN         is_border = TRUE;
    GUI_RECT_T      text_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //text is exist border
    if (GUI_BORDER_NONE == text_ctrl_ptr->theme.border.type)
    {
        is_border = FALSE;
    }

    if ((is_border) &&
        (!GUI_IsRectEmpty(base_ctrl_ptr->rect)))
    {
        text_rect.left   = (int16)(base_ctrl_ptr->rect.left + text_ctrl_ptr->theme.border.width);
        text_rect.right  = (int16)(base_ctrl_ptr->rect.right - text_ctrl_ptr->theme.border.width);
        text_rect.top    = (int16)(base_ctrl_ptr->rect.top + text_ctrl_ptr->theme.border.width);
        text_rect.bottom = (int16)(base_ctrl_ptr->rect.bottom - text_ctrl_ptr->theme.border.width);
    }
    else
    {
        text_rect = base_ctrl_ptr->rect;
    }

    return (text_rect);
}

/*****************************************************************************/
//  Description : get text line rect,except dividing line,progress bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T GUITEXT_GetLineRect(
                                      uint16            line_index,     //in:from 0
                                      CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                      )
{
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      text_rect = {0};
    GUI_BIG_RECT_T  relative_rect = {0};

    //get line relative rect
    relative_rect = GUITEXT_GetLineRelativeRect(line_index,text_ctrl_ptr);

    //get text rect
    text_rect = GUITEXT_GetRect(text_ctrl_ptr);

    //convert relative rect to line rect
    line_rect.left   = (int16)(relative_rect.left + text_rect.left - text_ctrl_ptr->relative_disp_rect.left);
    line_rect.top    = (int16)(relative_rect.top + text_rect.top - text_ctrl_ptr->relative_disp_rect.top);
    line_rect.right  = (int16)(relative_rect.right + text_rect.left - text_ctrl_ptr->relative_disp_rect.left);
    line_rect.bottom = (int16)(relative_rect.bottom + text_rect.top - text_ctrl_ptr->relative_disp_rect.top);

    return (line_rect);
}

/*****************************************************************************/
//  Description : get line relative rect,except dividing line,progress bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T GUITEXT_GetLineRelativeRect(
                                                  uint16            line_index,     //in:from 0
                                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                                  )
{
    uint16              line_height = 0;
    uint16              prg_width = 0;
    int32               top_offset = 0;
    GUI_BIG_RECT_T      relative_rect = {0};
    MMITHEME_TEXT_T     *theme_ptr = &(text_ctrl_ptr->theme);

    //get line height
    line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

    //need scroll bar
    if (PNULL != text_ctrl_ptr->prgbox_ctrl_ptr)
    {
        prg_width = GUITEXT_GetScrollBarWidth(text_ctrl_ptr);
    }

    //get line relative rect
    relative_rect = text_ctrl_ptr->relative_disp_rect;
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        relative_rect.left = relative_rect.left + prg_width;
    }
    if (MMI_DIRECTION_RIGHT != MMITHEME_GetDisDirection())
    {
        relative_rect.right = relative_rect.right - prg_width;
    }

    //get text top offset
    top_offset = GetTextTopOffset(text_ctrl_ptr);

    relative_rect.top    = top_offset + theme_ptr->margin_top_space + line_index*line_height;
    relative_rect.bottom = relative_rect.top + line_height - text_ctrl_ptr->theme.line_space - 1;

    return (relative_rect);
}

/*****************************************************************************/
//  Description : get top offset, adjust top by align
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int32 GetTextTopOffset(
                             CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                             )
{
    uint16          line_height = 0;
    uint16          text_height = 0;
    int32           top_offset = 0;
    GUI_RECT_T      text_rect = {0};

    //no scroll bar
    if (PNULL == text_ctrl_ptr->prgbox_ctrl_ptr)
    {
        text_rect = GUITEXT_GetRect(text_ctrl_ptr);

        //get line height
        line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

        //get text height
        text_height = (uint16)(text_ctrl_ptr->theme.margin_top_space + text_ctrl_ptr->theme.margin_bottom_space);
        if (0 < text_ctrl_ptr->total_line_num)
        {
            text_height = (uint16)(text_height + line_height*text_ctrl_ptr->total_line_num - text_ctrl_ptr->theme.line_space);
        }

        //get text rect
        text_rect = GUITEXT_GetRect(text_ctrl_ptr);

        switch (text_ctrl_ptr->theme.align)
        {
        case ALIGN_LEFT:
        case ALIGN_RIGHT:
        case ALIGN_HMIDDLE:
        case ALIGN_TOP:
            //top,do nothing
            break;

        case ALIGN_BOTTOM:
            //bottom
            if ((0 < text_height) && 
                (text_rect.bottom - text_rect.top + 1 > text_height))
            {
                top_offset = (int16)(text_rect.bottom - text_rect.top + 1 - text_height);
            }
            break;

        case ALIGN_VMIDDLE:
        case ALIGN_LVMIDDLE:
        case ALIGN_RVMIDDLE:
        case ALIGN_HVMIDDLE:
            //middle
            if ((0 < text_height) &&  
                (text_rect.bottom - text_rect.top + 1 > text_height))
            {
                top_offset = (int16)((text_rect.bottom - text_rect.top + 1 - text_height) >> 1);
            }
            break;

        default:
            break;
        }
    }

    return (top_offset);
}

/*****************************************************************************/
//  Description : get one line text string rect,except margin
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T GUITEXT_GetLineStrRect(
                                         uint16             line_index,     //in:from 0
                                         CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                         )
{
    GUI_RECT_T  text_rect = {0};

    //get line rect
    text_rect = GUITEXT_GetLineRect(line_index,text_ctrl_ptr);

    //except margin
    text_rect.left  = (uint16)(text_rect.left + text_ctrl_ptr->theme.margin_left_space);
    text_rect.right = (uint16)(text_rect.right - text_ctrl_ptr->theme.margin_right_space);

    return (text_rect);
}

/*****************************************************************************/
//  Description : set text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_SetDisplayRect(
                                      int32             display_top,    //in:
                                      CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                                      )
{
    BOOLEAN     result = FALSE;
    uint16      display_height = 0;

    //adjust display top
    if (display_top != text_ctrl_ptr->relative_disp_rect.top)
    {
        //get display height
        display_height = GUITEXT_GetDispHeight(text_ctrl_ptr);

        //set display top and bottom
        text_ctrl_ptr->relative_disp_rect.top    = display_top;
        text_ctrl_ptr->relative_disp_rect.bottom = display_top + display_height - 1;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get text display rect height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetDispHeight(
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                    )
{
    uint16      display_height = 0;
    GUI_RECT_T  text_rect = {0};

    //get text rect
    text_rect = GUITEXT_GetRect(text_ctrl_ptr);

    //get display height
    display_height = (uint16)(text_rect.bottom - text_rect.top + 1);

    return (display_height);
}

/*****************************************************************************/
//  Description : get text display rect max top
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC int32 GUITEXT_GetDispMaxTop(
                                   CTRLTEXT_OBJ_T   *text_ctrl_ptr
                                   )
{
    uint16      display_height = 0;
    int32       max_top = 0;
    uint32      text_height = 0;

    //get display height
    display_height = GUITEXT_GetDispHeight(text_ctrl_ptr);

    //get text relative height
    text_height = GUITEXT_GetRelativeHeight(text_ctrl_ptr);
    
    //get max top
    if (text_height > display_height)
    {
        max_top = text_ctrl_ptr->relative_rect.bottom - display_height + 1;
    }

    return (max_top);
}

/*****************************************************************************/
//  Description : get text relative height,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 GUITEXT_GetRelativeHeight(
                                        CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                        )
{
    uint32      relative_height = 0;

    //get relative height
    relative_height = text_ctrl_ptr->relative_rect.bottom - text_ctrl_ptr->relative_rect.top + 1;

    return (relative_height);
}

/*****************************************************************************/
//  Description : get text line height,include line space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetLineHeight(
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                    )
{
    uint16      font_height = 0;
    uint16      line_height = 0;

    //get font height
    font_height = GUI_GetFontHeight(text_ctrl_ptr->theme.font.font,UNICODE_HANZI);

    //get line height
    line_height = (uint16)(font_height + text_ctrl_ptr->theme.line_space);

    return (line_height);
}

/*****************************************************************************/
//  Description : get scroll bar width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetScrollBarWidth(
                                        CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                        )
{
    uint16      bar_width = 0;

    if (text_ctrl_ptr->theme.is_support_slide)
    {
        bar_width = MMITHEME_GetSlideScrollBarWidth();
    }
    else
    {
        bar_width = MMITHEME_GetScrollBarWidth();
    }

    return (bar_width);
}

/*****************************************************************************/
//  Description : get line string length
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetLineStrLen(
                                    uint16          line_index,     //in:from 0
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                    )
{
    uint16      str_len = 0;

    if (line_index < text_ctrl_ptr->total_line_num)
    {
        //is last line
        if ((line_index+1) == text_ctrl_ptr->total_line_num)
        {
            str_len = (uint16)(text_ctrl_ptr->str_len - text_ctrl_ptr->line_info_ptr[line_index].start_pos);
        }
        else
        {
            str_len = (uint16)(text_ctrl_ptr->line_info_ptr[line_index+1].start_pos - text_ctrl_ptr->line_info_ptr[line_index].start_pos);
        }
    }

    return (str_len);
}

/*****************************************************************************/
//  Description : get line string region style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetLineStrRegion(//region number
                                       BOOLEAN                  is_highlight,       //in:
                                       uint16                   line_index,         //in:from 0
                                       uint16                   region_max_num,     //in:
                                       CTRLTEXT_OBJ_T           *text_ctrl_ptr,     //in:
                                       GUIFONT_REGION_STYLE_T   *region_style_ptr   //in/out:
                                       )
{
    uint16          i = 0;
    uint16          region_num = 0;
    uint16          line_str_len = 0;
    uint16          line_start_pos = 0;
    uint16          line_end_pos = 0;
    uint16          region_start_pos = 0;
    uint16          region_end_pos = 0;
    GUITEXT_TAG_T   *tag_ptr = PNULL;
    GUITEXT_LINE_T  *line_info_ptr = PNULL;

    //set line info
    line_info_ptr = &text_ctrl_ptr->line_info_ptr[line_index];

    if ((PNULL != region_style_ptr) && 
        (0 < line_info_ptr->tag_num))
    {
        //set region num
        region_num = line_info_ptr->tag_num;
        SCI_ASSERT(region_num <= region_max_num);   /*assert verified*/

        //get line string length
        line_str_len = GUITEXT_GetLineStrLen(line_index,text_ctrl_ptr);

        //set line string start and end pos
        line_start_pos = line_info_ptr->start_pos;
        line_end_pos   = (uint16)(line_start_pos + line_str_len);

        //set start tag
        tag_ptr = line_info_ptr->start_tag_ptr;
        
        //get all effect
        for (i=0; i<region_num; i++)
        {
            //get effect start and end pos
            region_start_pos = tag_ptr->start_pos;
            region_end_pos   = (uint16)(region_start_pos + tag_ptr->len);
            region_start_pos = (uint16)MAX(line_start_pos,region_start_pos);
            region_end_pos   = (uint16)MIN(line_end_pos,region_end_pos);

            if (region_end_pos > region_start_pos)
            {
                //set effect
                region_style_ptr[i].start      = (uint16)(region_start_pos - line_start_pos);
                region_style_ptr[i].length     = (uint16)(region_end_pos - region_start_pos);
                region_style_ptr[i].state     |= GUISTR_STATE_UNDERLINE;
                region_style_ptr[i].font_color = text_ctrl_ptr->theme.font.color;
            }

            tag_ptr = tag_ptr->next_ptr;
        }
    }

    if ((PNULL != region_style_ptr) && 
        (is_highlight))
    {
        //set region num
        region_num++;
        SCI_ASSERT(region_num <= region_max_num);   /*assert verified*/

        //set highlight effect
        region_style_ptr[i].state     |= GUISTR_STATE_INVERSE;
        region_style_ptr[i].font_color = text_ctrl_ptr->theme.font.color;

        //get highlight start pos and length 
        region_style_ptr[i].length = GUITEXT_GetHighlightStartPosAndLen(&region_style_ptr[i].start,
                                    line_index,
                                    text_ctrl_ptr);
    }

    return (region_num);
}

/*****************************************************************************/
//  Description : get current cursor line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:除第一行,光标前一个字符是换行符之外，光标不能显示在行首位置
/*****************************************************************************/
PUBLIC uint16 GetCursorLineIndex(
                                 uint16         cursor_pos,
                                 CTRLTEXT_OBJ_T *text_ctrl_ptr
                                 )
{
    BOOLEAN     is_new_line = FALSE;
	BOOLEAN		is_highlight_end = FALSE;
    uint16      highlight_len = 0;
	uint16		max_pos = 0;
    uint16      i = 0;
    uint16      line_index = 0;

    SCI_ASSERT(0 < text_ctrl_ptr->total_line_num); /*assert verified*/

    //the char before cursor is new line char
    if ((0 < cursor_pos) &&
        (GUI_IsNewlineChar(text_ctrl_ptr->str_ptr[cursor_pos - 1])))
    {
        //carriage return or line feed
        is_new_line = TRUE;
    }
    
    //get highlight len
    highlight_len = GUITEXT_GetHighlightLen(text_ctrl_ptr);

	//set highlight max position
	max_pos = GUITEXT_GetHighlightMaxPos(text_ctrl_ptr);
	if ((0 <highlight_len) &&
		(cursor_pos == max_pos))
	{
		is_highlight_end = TRUE;
	}

    if ((is_new_line) ||
        ((0 < highlight_len) && (!is_highlight_end)))
    {
        if (cursor_pos >= text_ctrl_ptr->line_info_ptr[text_ctrl_ptr->total_line_num - 1].start_pos)
        {
            line_index = (uint16)(text_ctrl_ptr->total_line_num - 1);
        }
        else
        {
            //get line index
            for (i=0; i<text_ctrl_ptr->total_line_num; i++)
            {
                if ((cursor_pos >= text_ctrl_ptr->line_info_ptr[i].start_pos) &&
                    (cursor_pos < text_ctrl_ptr->line_info_ptr[i+1].start_pos))
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
        else if (cursor_pos > text_ctrl_ptr->line_info_ptr[text_ctrl_ptr->total_line_num - 1].start_pos)
        {
            //is last line
            line_index = (uint16)(text_ctrl_ptr->total_line_num - 1);
        }
        else
        {
            //get line index
            for (i=0; i<text_ctrl_ptr->total_line_num; i++)
            {
                if ((cursor_pos > text_ctrl_ptr->line_info_ptr[i].start_pos) &&
                    (cursor_pos <= text_ctrl_ptr->line_info_ptr[i+1].start_pos))
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
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_GetPosRect(
                                  CTRLTEXT_OBJ_T     *text_ctrl_ptr,      //in
                                  uint16             pos,
                                  GUI_RECT_T         *rect_ptr   //in/out:
                                  )
{
    BOOLEAN         result = FALSE;
    uint16          line_index = 0;
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      rect = {0};

    //get current cursor line index
    line_index = GetCursorLineIndex(pos,text_ctrl_ptr);

    //judge cursor is in current page
    if ((line_index >= text_ctrl_ptr->top_line_index) &&
        (line_index < (text_ctrl_ptr->top_line_index + text_ctrl_ptr->page_line_num)
            || (line_index == (text_ctrl_ptr->top_line_index + text_ctrl_ptr->page_line_num)    // fix NEWMS00187760
                && 0 != text_ctrl_ptr->relative_disp_rect.top%GUITEXT_GetLineHeight(text_ctrl_ptr))))
    {
        //get current cursor line text rect
        text_rect = GUITEXT_GetLineStrRect(line_index,text_ctrl_ptr);

        //set cursor rect
        rect = text_rect;

        //Adjust cursor rect left position by align
        AdjustLeftByAlign(line_index,&rect,text_ctrl_ptr);

        //get cursor rect left
        rect.left = GetLeftCoordinate(line_index,pos,rect,text_ctrl_ptr);

        //set cursor right
        rect.right = (int16)rect.left;

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
//  Description : get cursor left coordinate
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetLeftCoordinate(
                              uint16          line_index,  //in:
                              uint16             pos,
                              GUI_RECT_T      text_rect,          //in:
                              CTRLTEXT_OBJ_T  *text_ctrl_ptr      //in
                              )
{
    int16           left_coordinate = text_rect.left;
    uint16          line_str_len = 0;

    //get line string length
    line_str_len = GUITEXT_GetLineStrLen(line_index,text_ctrl_ptr);

    //set cursor left by text rect
    left_coordinate = GUI_GetCusorCoordinate(
        text_ctrl_ptr->theme.align,
        &text_rect,
        text_ctrl_ptr->line_info_ptr[line_index].direction,
        text_ctrl_ptr->str_ptr+text_ctrl_ptr->line_info_ptr[line_index].start_pos,
        line_str_len,
        (uint16)(pos - text_ctrl_ptr->line_info_ptr[line_index].start_pos),
        text_ctrl_ptr->theme.font.font,
        text_ctrl_ptr->theme.char_space);

    //adjust cursor rect left
    if (pos == text_ctrl_ptr->line_info_ptr[line_index].start_pos)
    {
        //调整行首光标显示在页边的空白
        if (left_coordinate >= text_rect.right)
        {
            left_coordinate = (int16)(text_rect.right + 1);
        }
        else
        {
            left_coordinate = (int16)(left_coordinate);
        }
    }

    return (left_coordinate);
}

/*****************************************************************************/
//  Description : adjust cursor/highlight rect left position by align
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustLeftByAlign(
                             uint16            line_index,     //in:
                             GUI_RECT_T        *rect_ptr,       //in/out:
                             CTRLTEXT_OBJ_T    *text_ctrl_ptr   //in:
                             )
{
    uint16          string_pixel = 0;

    //get string pixel
    string_pixel = GetTextLineStrPixel(line_index,(uint16)(rect_ptr->right-rect_ptr->left+1),text_ctrl_ptr);

    //get sting start position
    switch (text_ctrl_ptr->theme.align)
    {
    case ALIGN_RIGHT:
    case ALIGN_RVMIDDLE:
        if (rect_ptr->right > string_pixel)
        {
            //set left start
            if (0 < string_pixel)
            {
                rect_ptr->left = (int16)(rect_ptr->right - string_pixel + 1);
            }
            else
            {
                rect_ptr->left = rect_ptr->right;
            }
        }
        break;

    case ALIGN_HMIDDLE:
    case ALIGN_HVMIDDLE:
        if (rect_ptr->left + rect_ptr->right > string_pixel)
        {
            //set left start
            rect_ptr->left = (int16)((rect_ptr->left + rect_ptr->right - string_pixel + 1) >> 1);
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : get line string pixel
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTextLineStrPixel(
                                 uint16         line_index,     //in:
                                 uint16         rect_width,     //in:
                                 CTRLTEXT_OBJ_T *text_ctrl_ptr  //in:
                                 )
{
    wchar       *str_ptr = PNULL;
    uint16      str_len = 0;
    uint16      char_num = 0;
    uint16      string_pixel = 0;

    //get string
    str_ptr = text_ctrl_ptr->str_ptr + text_ctrl_ptr->line_info_ptr[line_index].start_pos;
    str_len = GUITEXT_GetLineStrLen(line_index,text_ctrl_ptr);

    //get string pixel
    GUI_GetStringInfoInLine(str_ptr,
            str_len,
            text_ctrl_ptr->theme.font.font,
            (uint8)(text_ctrl_ptr->theme.char_space),
            rect_width,
            &char_num,
            &string_pixel,
            FALSE);

    return (string_pixel);
}


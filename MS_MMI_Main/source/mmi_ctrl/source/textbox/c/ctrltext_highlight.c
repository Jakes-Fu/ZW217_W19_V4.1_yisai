/*****************************************************************************
** File Name:      ctrltext_highlight.c                                      *
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
#include "guistring.h"

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
//  Description : update text string bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdateTextBg(
                        uint16          start_line_index,   //in:
                        uint16          end_line_index,     //in:
                        CTRLTEXT_OBJ_T  *text_ctrl_ptr      //in:
                        );

/*****************************************************************************/
//  Description : display highlight and string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightAndStr(
                                  BOOLEAN           is_highlight,   //in:
                                  uint16            line_index,     //in:
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                                  );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : display text highlight string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_DisplayHighlight(
                                     BOOLEAN            is_highlight,   //in:FASLE:only display string
                                     CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                     )
{
    uint16          min_pos = 0;
    uint16          max_pos = 0;
    uint16          min_line = 0;
    uint16          max_line = 0;
    uint16          i = 0;

    if (text_ctrl_ptr->highlight.start_pos != text_ctrl_ptr->highlight.end_pos)
    {
        //active,display highlight
        if (!text_ctrl_ptr->is_active)
        {
            is_highlight = FALSE;
        }

        //set highlight min and max position
        min_pos = GUITEXT_GetHighlightMinPos(text_ctrl_ptr);
        max_pos = GUITEXT_GetHighlightMaxPos(text_ctrl_ptr);

        //get highlight min line index
        min_line = GUITEXT_GetHighlightLineIndex(FALSE,min_pos,text_ctrl_ptr);

        //get highlight end line index
        max_line = GUITEXT_GetHighlightLineIndex(TRUE,max_pos,text_ctrl_ptr);

        //get current page min,max line index
        min_line = (uint16)MAX(min_line,text_ctrl_ptr->top_line_index);
        max_line = (uint16)MIN(max_line,text_ctrl_ptr->bottom_line_index);

        if (min_line <= max_line)
        {
            //update bg
            UpdateTextBg(min_line,max_line,text_ctrl_ptr);

            for (i=min_line; i<=max_line; i++)
            {
                //display highlight and string
                DisplayHighlightAndStr(is_highlight,i,text_ctrl_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description : get the highlight minor position
//  Global resource dependence : 
//  Author: Jassmine
//  Note: must 
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightMinPos(
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                )
{
    uint16  min_pos = 0;
    
    min_pos = (uint16)MIN(text_ctrl_ptr->highlight.start_pos,text_ctrl_ptr->highlight.end_pos);

    return (min_pos);
}

/*****************************************************************************/
//  Description : get the highlight max position
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightMaxPos(
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                )
{
    uint16  max_pos = 0;

    max_pos = (uint16)MAX(text_ctrl_ptr->highlight.start_pos,text_ctrl_ptr->highlight.end_pos);

    return (max_pos);
}

/*****************************************************************************/
//  Description : get current highlight line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightLineIndex(
                                            BOOLEAN         is_end,
                                            uint16          highlight_pos,
                                            CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                            )
{
    BOOLEAN     is_new_line = FALSE;
    uint16      highlight_len = 0;
    uint16      i = 0;
    uint16      line_index = 0;

    SCI_ASSERT(0 < text_ctrl_ptr->total_line_num); /*assert verified*/

    //the char before cursor is new line char
    if ((0 < highlight_pos) &&
        (GUI_IsNewlineChar(text_ctrl_ptr->str_ptr[highlight_pos - 1])))
    {
        //carriage return or line feed
        is_new_line = TRUE;
    }
    
    //get highlight len
    highlight_len = GUITEXT_GetHighlightLen(text_ctrl_ptr);

    if ((!is_end) || (is_new_line) || (0 < highlight_len))
    {
        if (highlight_pos >= text_ctrl_ptr->line_info_ptr[text_ctrl_ptr->total_line_num - 1].start_pos)
        {
            line_index = (uint16)(text_ctrl_ptr->total_line_num - 1);
        }
        else
        {
            //get line index
            for (i=0; i<text_ctrl_ptr->total_line_num; i++)
            {
                if ((highlight_pos >= text_ctrl_ptr->line_info_ptr[i].start_pos) &&
                    (highlight_pos < text_ctrl_ptr->line_info_ptr[i+1].start_pos))
                {
                    line_index = i;
                    break;
                }
            }
        }
    }
    else
    {
        if (0 == highlight_pos)
        {
            line_index = 0;
        }
        else if (highlight_pos > text_ctrl_ptr->line_info_ptr[text_ctrl_ptr->total_line_num - 1].start_pos)
        {
            //is last line
            line_index = (uint16)(text_ctrl_ptr->total_line_num - 1);
        }
        else
        {
            //get line index
            for (i=0; i<text_ctrl_ptr->total_line_num; i++)
            {
                if ((highlight_pos > text_ctrl_ptr->line_info_ptr[i].start_pos) &&
                    (highlight_pos <= text_ctrl_ptr->line_info_ptr[i+1].start_pos))
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
//  Description : get text highlight length
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightLen(
                                      CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                      )
{
    uint16      highlight_len = 0;

    if (text_ctrl_ptr->highlight.end_pos > text_ctrl_ptr->highlight.start_pos)
    {
        highlight_len = (uint16)(text_ctrl_ptr->highlight.end_pos - text_ctrl_ptr->highlight.start_pos);
    }
    else
    {
        highlight_len = (uint16)(text_ctrl_ptr->highlight.start_pos - text_ctrl_ptr->highlight.end_pos);
    }

    return (highlight_len);
}

/*****************************************************************************/
//  Description : update text string bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdateTextBg(
                        uint16          start_line_index,   //in:
                        uint16          end_line_index,     //in:
                        CTRLTEXT_OBJ_T  *text_ctrl_ptr      //in:
                        )
{
    uint16          i = 0;
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      bg_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //get bg rect
    bg_rect = GUITEXT_GetLineRect(start_line_index,text_ctrl_ptr);
    if (end_line_index > start_line_index)
    {
        line_rect = GUITEXT_GetLineRect(end_line_index,text_ctrl_ptr);
        bg_rect.bottom = line_rect.bottom;
    }

    //get crossed rect
    if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,bg_rect))
    {
        //display bg
        GUITEXT_DisplayBg(cross_rect,text_ctrl_ptr);
    }

    //draw dividing line
    if ((0 < text_ctrl_ptr->theme.line_width) && 
        (end_line_index > start_line_index))
    {
        for (i=start_line_index; i<end_line_index; i++)
        {
            //get line rect
            line_rect        = GUITEXT_GetLineRect(i,text_ctrl_ptr);
            line_rect.top    = (int16)(line_rect.bottom + 1);
            line_rect.bottom = (int16)(line_rect.top + text_ctrl_ptr->theme.line_width - 1);

            //get crossed rect
            if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,line_rect))
            {
                //draw line
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    cross_rect,
                    text_ctrl_ptr->theme.line_color);
            }
        }
    }
}

/*****************************************************************************/
//  Description : display highlight and string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightAndStr(
                                  BOOLEAN           is_highlight,   //in:
                                  uint16            line_index,     //in:
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                                  )
{
    uint16                  region_num = 0;
    GUI_RECT_T              text_rect = {0};
    GUI_RECT_T              cross_rect = {0};
    GUI_ALIGN_E             align = ALIGN_LVMIDDLE;
    MMI_STRING_T            text_str = {0};
    GUISTR_STYLE_T          str_style = {0}; /*lint !e64*/
    GUIFONT_REGION_STYLE_T  *region_style_ptr = PNULL;
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //get line text rect
    text_rect = GUITEXT_GetLineStrRect(line_index,text_ctrl_ptr);

    //get crossed rect
    if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,text_rect))
    {
        //set string
        text_str.wstr_ptr = text_ctrl_ptr->str_ptr + text_ctrl_ptr->line_info_ptr[line_index].start_pos;
        text_str.wstr_len = GUITEXT_GetLineStrLen(line_index,text_ctrl_ptr);

        //alloc region memory
        region_num = text_ctrl_ptr->line_info_ptr[line_index].tag_num;
        if (is_highlight)
        {
            region_num++;
        }
        if (0 < region_num)
        {
            region_style_ptr = SCI_ALLOC_APP(region_num*sizeof(GUIFONT_REGION_STYLE_T));
            SCI_MEMSET(region_style_ptr,0,(region_num*sizeof(GUIFONT_REGION_STYLE_T)));
        }

        //get line string region style
        region_num = GUITEXT_GetLineStrRegion(is_highlight,line_index,region_num,text_ctrl_ptr,region_style_ptr);

        //set string style
        str_style.font       = text_ctrl_ptr->theme.font.font;
        str_style.font_color = text_ctrl_ptr->theme.font.color;
        str_style.align      = align;
        str_style.bg_color   = (GUI_COLOR_T )(~text_ctrl_ptr->theme.font.color);
        str_style.line_color = text_ctrl_ptr->theme.underline_color;
        str_style.char_space = text_ctrl_ptr->theme.char_space;
        str_style.region_ptr = region_style_ptr;
        str_style.region_num = region_num;

        //display string
        GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
            (const GUI_RECT_T*)&text_rect,
            (const GUI_RECT_T*)&cross_rect,
            &text_str,
            &str_style,
            GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR,
            text_ctrl_ptr->line_info_ptr[line_index].direction);

        //free region
        if (PNULL != region_style_ptr)
        {
            SCI_FREE(region_style_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : get highlight start pos len in the line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightStartPosAndLen(
                                                 uint16             *start_pos_ptr, //in/out:相对于行首
                                                 uint16             line_index,     //in:
                                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                                                 )
{
    uint16      min_pos = 0;
    uint16      max_pos = 0;
    uint16      min_line = 0;
    uint16      max_line = 0;
    uint16      highlight_len = 0;

    //get min and max position
    min_pos = GUITEXT_GetHighlightMinPos(text_ctrl_ptr);
    max_pos = GUITEXT_GetHighlightMaxPos(text_ctrl_ptr);

    //get highlight min line index
    min_line = GUITEXT_GetHighlightLineIndex(FALSE,min_pos,text_ctrl_ptr);

    //get highlight end line index
    max_line = GUITEXT_GetHighlightLineIndex(TRUE,max_pos,text_ctrl_ptr);

    if (line_index == min_line)
    {
        //first highlight line
        *start_pos_ptr = (uint16)(min_pos - text_ctrl_ptr->line_info_ptr[line_index].start_pos);
        
        //is last line
        if ((line_index+1) == text_ctrl_ptr->total_line_num)
        {
            highlight_len = (uint16)(text_ctrl_ptr->str_len - min_pos);
        }
        else
        {
            highlight_len = (uint16)(text_ctrl_ptr->line_info_ptr[line_index+1].start_pos - min_pos);
        }
    }
    else if (line_index == max_line)
    {
        //last highlight line
        *start_pos_ptr = 0;
        highlight_len = (uint16)(max_pos - text_ctrl_ptr->line_info_ptr[line_index].start_pos);
    }
    else if ((line_index > min_line) && (line_index < max_line))
    {
        *start_pos_ptr = 0;
        highlight_len = GUITEXT_GetLineStrLen(line_index,text_ctrl_ptr);
    }
    else
    {
        *start_pos_ptr = 0;
        highlight_len = 0;
    }
    
    //adjust highlight length
    if (highlight_len > (max_pos-min_pos))
    {
        highlight_len = (uint16)(max_pos - min_pos);
    }

    return (highlight_len);
}

/*****************************************************************************/
//  Description : get highlight string and length
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_GetHighlightStr(
                                       uint16           **highlight_str_pptr,   //in/out:
                                       uint16           *highlight_len_ptr,     //in/out:
                                       CTRLTEXT_OBJ_T   *text_ctrl_ptr          //in:
                                       )
{
    BOOLEAN     result = FALSE;
    uint16      min_pos = 0;

    //get highlight
    if (text_ctrl_ptr->highlight.start_pos != text_ctrl_ptr->highlight.end_pos)
    {
        //get highlight min position
        min_pos = GUITEXT_GetHighlightMinPos(text_ctrl_ptr);

        //get highlight string and len
        *highlight_str_pptr = text_ctrl_ptr->str_ptr + min_pos;
        *highlight_len_ptr  = GUITEXT_GetHighlightLen(text_ctrl_ptr);

        result = TRUE;
    }

    return (result);
}



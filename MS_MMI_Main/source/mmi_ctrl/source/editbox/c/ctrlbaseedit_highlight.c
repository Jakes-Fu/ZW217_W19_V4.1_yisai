/*****************************************************************************
** File Name:      ctrlbaseedit_highlight.c                                  *
** Author:         hua.fang                                                  *
** Date:           09/11/2012                                                *
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
#include "ctrlbaseedit.h"
#include "guistring.h"

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
//  Description : display highlight and string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightAndStr (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_highlight,       //in:
    uint16              line_index          //in:
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : display edit highlight
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplayHighlight (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr,
    BOOLEAN                is_highlight         //in:FASLE:only display string
)
{
    uint16          min_pos = 0;
    uint16          max_pos = 0;
    uint16          min_line = 0;
    uint16          max_line = 0;
    uint16          line_index = 0;
    uint16          i = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if ((baseedit_ctrl_ptr->common_theme.is_disp_highlight
            || !baseedit_ctrl_ptr->highlight.is_underline)
        && (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos))
    {
        //active,display highlight
        if (!base_ctrl_ptr->is_active)
        {
            is_highlight = FALSE;
        }

        //get min line index
        min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
        min_line = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, min_pos);
        line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
        min_line = (uint16) MAX (min_line, line_index);

        //get end line index
        max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);
        max_line = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, max_pos);
        line_index = BASEEDIT_GetBottomLineIndex(baseedit_ctrl_ptr);
        max_line = (uint16) MIN (max_line, line_index);

        if (min_line <= max_line)
        {
            //update bg
            BASEEDIT_UpdateTextBg (baseedit_ctrl_ptr, min_line, max_line);

            for (i = min_line; i <= max_line; i++)
            {
                //display highlight and string
                DisplayHighlightAndStr (baseedit_ctrl_ptr, is_highlight, i);
            }
        }
    }
}

/*****************************************************************************/
//  Description : get highlight start pos len in the line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetHighlightStartPosAndLen (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    uint16              line_index,         //in:
    uint16              *start_pos_ptr      //in/out:相对于行首
)
{
    uint16      min_pos = 0;
    uint16      max_pos = 0;
    uint16      min_line = 0;
    uint16      max_line = 0;
    uint16      highlight_len = 0;

    //get min and max position
    min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
    max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

    //get highlight min and max line index
    min_line = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, min_pos);
    max_line = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, max_pos);

    if (line_index == min_line)
    {
        //first highlight line
        if (min_pos > baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos)
        {
            *start_pos_ptr = (uint16) (min_pos - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
        }
        else
        {
            *start_pos_ptr = 0;
        }

        //is last line
        if ( (line_index + 1) == baseedit_ctrl_ptr->total_line_num)
        {
            if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
            {
                // TODO:
                // 取高亮长度时有问题，暂时以风格来区分
                if (BASEEDIT_IsFixedFormat (baseedit_ctrl_ptr))
                {
                    highlight_len = (uint16) (max_pos - min_pos);
                }
                else if (max_pos > baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos &&
                         min_pos < baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos)
                {
                    uint16 visible_min_pos = 0;
                    uint16 visible_max_pos = 0;

                    if (min_pos < baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos)
                    {
                        visible_min_pos = baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos;
                    }
                    else
                    {
                        visible_min_pos = min_pos;
                    }

                    if (max_pos > baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos)
                    {
                        visible_max_pos = baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos;
                    }
                    else
                    {
                        visible_max_pos = max_pos;
                    }

                    highlight_len = (uint16) (visible_max_pos - visible_min_pos);
                }
            }
            else
            {
                highlight_len = (uint16) (baseedit_ctrl_ptr->str_len - min_pos);
            }
        }
        else
        {
            highlight_len = (uint16) (baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos - min_pos);
        }
    }
    else if (line_index == max_line)
    {
        //last highlight line
        *start_pos_ptr = 0;
        highlight_len = (uint16) (max_pos - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
    }
    else if ( (line_index > min_line) && (line_index < max_line))
    {
        *start_pos_ptr = 0;
        highlight_len = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, line_index);
    }
    else
    {
        *start_pos_ptr = 0;
        highlight_len  = 0;
    }

    //adjust highlight length
    if (highlight_len > (max_pos - min_pos))
    {
        highlight_len = (uint16) (max_pos - min_pos);
    }

	if(*start_pos_ptr > (uint16)(baseedit_ctrl_ptr->str_len -1))
	{
		*start_pos_ptr = 0;
		highlight_len  = 0;
	}
    return (highlight_len);
}

/*****************************************************************************/
//  Description : get edit highlight length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetHighlightLen (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16      highlight_len = 0;

    if (baseedit_ctrl_ptr->highlight.end_pos > baseedit_ctrl_ptr->highlight.start_pos)
    {
        highlight_len = (uint16) (baseedit_ctrl_ptr->highlight.end_pos - baseedit_ctrl_ptr->highlight.start_pos);
    }
    else
    {
        highlight_len = (uint16) (baseedit_ctrl_ptr->highlight.start_pos - baseedit_ctrl_ptr->highlight.end_pos);
    }

    return (highlight_len);
}

/*****************************************************************************/
//  Description : get the highlight minor position
//  Global resource dependence :
//  Author: Jassmine
//  Note: must
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetHighlightMinPos (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16  min_pos = 0;

    min_pos = (uint16) MIN (baseedit_ctrl_ptr->highlight.start_pos, baseedit_ctrl_ptr->highlight.end_pos);

    return (min_pos);
}

/*****************************************************************************/
//  Description : get the highlight max position
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetHighlightMaxPos (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16  max_pos = 0;

    max_pos = (uint16) MAX (baseedit_ctrl_ptr->highlight.start_pos, baseedit_ctrl_ptr->highlight.end_pos);

    return (max_pos);
}

/*****************************************************************************/
//  Description : set edit no highlight
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_SetNoHighlight (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              pos
)
{
    VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, pos, pos, SETHIGHLIGHT_BOTH);
    baseedit_ctrl_ptr->highlight.is_underline = FALSE;

    if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
    {
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);
    }
}

/*****************************************************************************/
//  Description : set highlight position
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BaseEditCtrlSetHighlight (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //[in]
    uint16              start_pos,          //[in]
    uint16              end_pos,            //[in]
    SETHIGHLIGHT_E      highlight_type      //[in]
)
{
    if (PNULL == baseedit_ctrl_ptr)
    {
        return;
    }

    // 做点保护，防止设置的值超过总数
    if (!BASEEDIT_IsFixedFormat (baseedit_ctrl_ptr))
    {
        if (start_pos > baseedit_ctrl_ptr->str_len)
        {
            start_pos = baseedit_ctrl_ptr->str_len;
        }

        if (end_pos > baseedit_ctrl_ptr->str_len)
        {
            end_pos = baseedit_ctrl_ptr->str_len;
        }
    }

    if (SETHIGHLIGHT_START & highlight_type)    /*lint !e655*/
    {
        baseedit_ctrl_ptr->highlight.start_pos = start_pos;
    }

    if (SETHIGHLIGHT_END & highlight_type)      /*lint !e655*/
    {
        baseedit_ctrl_ptr->highlight.end_pos = end_pos;
    }

    if ((baseedit_ctrl_ptr->highlight.is_underline) &&
        (baseedit_ctrl_ptr->highlight.start_pos == baseedit_ctrl_ptr->highlight.end_pos) &&
        (0 != baseedit_ctrl_ptr->im_ctrl_handle))
    {
        //clear im
        GUIIM_ClearState (baseedit_ctrl_ptr->im_ctrl_handle);
    }
}

/*****************************************************************************/
//  Description : display highlight and string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightAndStr (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_highlight,       //in:
    uint16              line_index          //in:
)
{
    uint16                  region_num = 0;
    GUI_RECT_T              text_rect = {0};
    GUI_RECT_T              cross_rect = {0};
    GUI_RECT_T              edit_client_rect = {0};
    GUI_ALIGN_E             align = ALIGN_LEFT;
    MMI_STRING_T            text_str = {0};
    GUISTR_STYLE_T          str_style = {0}; /*lint !e64*/
    GUISTR_STATE_T          str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    GUIFONT_REGION_STYLE_T  region_style = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //get line text rect
    text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, line_index);

    // get edit client rect
    edit_client_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);

    //get crossed rect
    if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, text_rect)
        && GUI_IntersectRect(&cross_rect, edit_client_rect, cross_rect))
    {
        //set string
        text_str.wstr_len = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, line_index);

        text_str.wstr_ptr = VTLBASEEDIT_GetDispStr (
                                baseedit_ctrl_ptr,
                                baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos,
                                text_str.wstr_len);

        if (is_highlight)
        {
            //set highlight effect
            region_num = 1;
            region_style.state |= GUISTR_STATE_INVERSE;
            region_style.font_color = (GUI_COLOR_T) (~baseedit_ctrl_ptr->common_theme.highlight_str_color);

            //get highlight start pos  and length
            region_style.length = BASEEDIT_GetHighlightStartPosAndLen (
                    baseedit_ctrl_ptr,
                    line_index,
                    &region_style.start);

            //set bg color
            str_style.bg_color = baseedit_ctrl_ptr->common_theme.highlight_bg_color;
        }

        //get align
        align = VTLBASEEDIT_GetAlign (baseedit_ctrl_ptr);

        //set string style
        str_style.font       = baseedit_ctrl_ptr->common_theme.font.font;
        str_style.font_color = baseedit_ctrl_ptr->common_theme.font.color;
        str_style.align      = align;
        str_style.char_space = baseedit_ctrl_ptr->common_theme.char_space;
        str_style.region_ptr = &region_style;
        str_style.region_num = region_num;

        if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
        {
            str_state |= GUISTR_STATE_NOT_HALF_CHAR;
        }

        if (GUIEDIT_DISPLAY_DIR_LT == baseedit_ctrl_ptr->display_dir)
        {
            str_state |= GUISTR_STATE_ALIGN_BY_DIR;
        }

        //display string
        GUISTR_DrawTextToLCDInRect (
                (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
                (const GUI_RECT_T*) &text_rect,
                (const GUI_RECT_T*) &cross_rect,
                &text_str,
                &str_style,
                str_state,
                baseedit_ctrl_ptr->line_info_ptr[line_index].direction);
    }
}




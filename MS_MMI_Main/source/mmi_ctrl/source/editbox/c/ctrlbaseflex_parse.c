/*****************************************************************************
** File Name:      ctrlbaseflex_parse.c                                      *
** Author:         hua.fang                                                  *
** Date:           09/14/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
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
#include "guiprgbox.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// for single line parse
typedef enum
{
    GUIEDIT_SINGLE_PARSE_BEGIN,             // parse start
    GUIEDIT_SINGLE_PARSE_MOVE_LEFT,         // when cursor invisible on rightside, we move first char left
    GUIEDIT_SINGLE_PARSE_MOVE_RIGHT,        // when cursor already visible, but the last char is not rightmost, we move first char right
    GUIEDIT_SINGLE_PARSE_END,               // move end

    GUIEDIT_SINGLE_PARSE_MAX
} GUIEDIT_SINGLE_PARSE_E;

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
//  Description : parse single line edit text,include start position
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParseSingleLineEditText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : parse multi line edit text,include start position,total line number and top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note: parse from front to behind
/*****************************************************************************/
LOCAL void ParseMultiLineEditText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              start_line_index    //in:
);

/*****************************************************************************/
//  Description : parse multi line edit text,include start position,total line number and top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note: parse from behind to from
/*****************************************************************************/
LOCAL void ParseMultiLineEditTextExt (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr      //in:
);

/*****************************************************************************/
//  Description : handle parse error string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleParseErrorStr (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              str_len         //in:
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get first char index for single line edit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_GetFirstCharIndex (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
)
{
    wchar               *remain_str_ptr = PNULL;
    uint16              first_char_index = 0;
    uint16              remain_str_len = 0;
    uint16              display_str_len = 0;
    uint16              text_width = 0;
    uint16              char_num = 0;
    uint16              string_pixel = 0;
    GUI_RECT_T          text_rect = {0};
    GUIEDIT_SINGLE_PARSE_E  parse_state = GUIEDIT_SINGLE_PARSE_BEGIN;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    SCI_ASSERT (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style); /*assert verified*/

    if (baseedit_ctrl_ptr->str_len > 0)
    {
        //get text rect
        text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, 0);

        //get text width
        text_width = (uint16) (text_rect.right - text_rect.left + 1);

        //set first char index
        first_char_index = baseedit_ctrl_ptr->line_info_ptr[0].start_pos;

        // when current cursor is smaller than the old first char index, use cursor
        // position as first char index
        // here, we use "<=" because we ensure cursor not at the first visible char
        if (baseedit_ctrl_ptr->cursor.cur_pos <= first_char_index)
        {
            if (0 == baseedit_ctrl_ptr->cursor.cur_pos)
            {
                first_char_index = baseedit_ctrl_ptr->cursor.cur_pos;
            }
            else
            {
                // make cursor behind the first visible char
                first_char_index = baseedit_ctrl_ptr->cursor.cur_pos - 1;
            }
        }

        do
        {
            //set remain string
            //remain_str_ptr = baseedit_ctrl_ptr->str_ptr + first_char_index;
            remain_str_len = (uint16) (baseedit_ctrl_ptr->str_len - first_char_index);
            remain_str_ptr = VTLBASEEDIT_GetDispStr(baseedit_ctrl_ptr, first_char_index, remain_str_len);

            //need display string len
            display_str_len = (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - first_char_index);

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

            if ((GUIEDIT_SINGLE_PARSE_BEGIN == parse_state || GUIEDIT_SINGLE_PARSE_MOVE_LEFT == parse_state) &&
                char_num < display_str_len)  // when cursor invisible on rightside, move string left
            {
                parse_state = GUIEDIT_SINGLE_PARSE_MOVE_LEFT;

                first_char_index++;
            }
            else if ( (GUIEDIT_SINGLE_PARSE_BEGIN == parse_state || GUIEDIT_SINGLE_PARSE_MOVE_RIGHT == parse_state)
                      && (char_num == remain_str_len)
                      && (0 < first_char_index)
                      && (string_pixel < text_width)) // when cursor already visible, but last char not rightmost, then move right
            {
                parse_state = GUIEDIT_SINGLE_PARSE_MOVE_RIGHT;

                first_char_index--;
            }
            else if (GUIEDIT_SINGLE_PARSE_MOVE_RIGHT == parse_state
                     && char_num < remain_str_len)   // when last char invisible after moving right, then just move left one char
            {
                parse_state = GUIEDIT_SINGLE_PARSE_END;

                first_char_index++;
            }
            else
            {
                break;
            }
        }
        while (first_char_index < baseedit_ctrl_ptr->str_len);
    }

    //set first char index
    baseedit_ctrl_ptr->line_info_ptr[0].start_pos = first_char_index;
    baseedit_ctrl_ptr->line_info_ptr[1].start_pos = (uint16) (first_char_index + char_num);
}

/*****************************************************************************/
//  Description : set line info,include start position,total line number and top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BaseFlexCtrlParseText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    uint16              start_line_index        //in:
)
{
    BOOLEAN             result = FALSE;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //edit rect is valid
    if (!GUI_IsInvalidRect (base_ctrl_ptr->rect))
    {
        if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
        {
            //parse single line edit text
            ParseSingleLineEditText (baseflex_ctrl_ptr);
        }
        else
        {
            if (GUIEDIT_DISPLAY_DIR_LT == baseedit_ctrl_ptr->display_dir)
            {
                //parse multi line edit text from front to behind
                ParseMultiLineEditText (baseflex_ctrl_ptr, start_line_index);
            }
            else
            {
                //parse multi line edit text from behind to front
                ParseMultiLineEditTextExt (baseflex_ctrl_ptr);
            }
        }

        if (PNULL != baseedit_ctrl_ptr->prgbox_ctrl_ptr)
        {
            //set progress param
            GUIPRGBOX_SetParamByPtr (
                    baseedit_ctrl_ptr->prgbox_ctrl_ptr,
                    BASEEDIT_GetTotalHeight(baseedit_ctrl_ptr),
                    BASEEDIT_GetDispHeight(baseedit_ctrl_ptr));
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : parse single line edit text,include start position
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParseSingleLineEditText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    SCI_ASSERT (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style); /*assert verified*/
    SCI_ASSERT (PNULL != baseedit_ctrl_ptr->line_info_ptr); /*assert verified*/

    baseedit_ctrl_ptr->line_info_ptr[0].direction = SPMLAPI_GetTextDirection (
            baseedit_ctrl_ptr->str_ptr,
            baseedit_ctrl_ptr->str_len);

    //get first char index
    BASEFLEX_GetFirstCharIndex (baseflex_ctrl_ptr);

    //reset top line index and total line number
    baseedit_ctrl_ptr->total_line_num = 1;
    BASEEDIT_GetTopLineOffsetRange(baseedit_ctrl_ptr, &baseedit_ctrl_ptr->top_line_offset, PNULL);
}

/*****************************************************************************/
//  Description : parse multi line edit text,include start position,total line number and top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note: parse from front to behind
/*****************************************************************************/
LOCAL void ParseMultiLineEditText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              start_line_index    //in:
)
{
    wchar       *remain_str_ptr = PNULL;
    uint16      remain_str_len = 0;
    uint16      line_index = 0;
    uint16      start_pos = 0;
    uint16      text_width = 0;
    uint16      char_num = 0;
    uint16      string_pixel = 0;
    GUI_RECT_T  text_rect = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    SCI_ASSERT (PNULL != baseedit_ctrl_ptr->line_info_ptr); /*assert verified*/

    //get text rect
    text_rect = BASEEDIT_GetTextRect (
            baseedit_ctrl_ptr,
            BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr));

    //get text width
    text_width = (uint16) (text_rect.right - text_rect.left + 1);

    //get line info
    line_index = start_line_index;
    start_pos  = baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos;

    while (start_pos < baseedit_ctrl_ptr->str_len)
    {
        //reset param
        char_num     = 0;
        string_pixel = 0;

        //set line start position and text direction
        baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos = start_pos;

        //get remain string
        //remain_str_ptr = baseedit_ctrl_ptr->str_ptr + start_pos;
        remain_str_len = (uint16) (baseedit_ctrl_ptr->str_len - start_pos);
        remain_str_ptr = VTLBASEEDIT_GetDispStr(baseedit_ctrl_ptr, start_pos, remain_str_len);

        //set line text direction
        if ((0 == start_pos) ||
            (BASEEDIT_IsNewLineChar (baseedit_ctrl_ptr, start_pos)))
        {
            baseedit_ctrl_ptr->line_info_ptr[line_index].direction = SPMLAPI_GetTextDirection (remain_str_ptr, remain_str_len);
        }
        else
        {
            SCI_ASSERT (0 < line_index); /*assert verified*/
            baseedit_ctrl_ptr->line_info_ptr[line_index].direction = baseedit_ctrl_ptr->line_info_ptr[line_index - 1].direction;
        }

        GUI_GetStringInfoInLineWithDir (
                baseedit_ctrl_ptr->line_info_ptr[line_index].direction,
                remain_str_ptr,
                remain_str_len,
                baseedit_ctrl_ptr->common_theme.font.font,
                (uint8) baseedit_ctrl_ptr->common_theme.char_space,
                text_width,
                &char_num,
                &string_pixel,
                baseflex_ctrl_ptr->is_byword);

        //parse error string
        if ( (0 == char_num) && (0 == string_pixel))
        {
            HandleParseErrorStr (baseflex_ctrl_ptr, start_pos);
            break;
        }

        start_pos = (uint16) (start_pos + char_num);
        line_index++;
    }

    //judge the last char
    if (BASEEDIT_IsNewLineChar (baseedit_ctrl_ptr, baseedit_ctrl_ptr->str_len))
    {
        baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos = baseedit_ctrl_ptr->str_len;
        line_index++;
    }

    //reset other line info,[CR 3 char/4 line]
    SCI_MEMSET (
            &baseedit_ctrl_ptr->line_info_ptr[line_index],
            0,
            ( (baseedit_ctrl_ptr->str_max_len + 1 - line_index) *sizeof (GUIEDIT_LINE_T)));

    //total line number
    baseedit_ctrl_ptr->total_line_num = line_index;

    if (0 == baseedit_ctrl_ptr->total_line_num)
    {
        baseedit_ctrl_ptr->total_line_num = 1;
    }

    //set top line index
    BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);
}

/*****************************************************************************/
//  Description : parse multi line edit text,include start position,total line number and top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note: parse from behind to from
/*****************************************************************************/
LOCAL void ParseMultiLineEditTextExt (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr      //in:
)
{
    wchar       *str_ptr = PNULL;
    uint16      str_len = 0;
    uint16      text_width = 0;
    uint16      total_line_num = 0;
    uint16      line_index = 0;
    uint16      start_pos = 0;
    uint16      char_num = 0;
    uint16      string_pixel = 0;
    wchar       *remain_str_ptr = PNULL;
    uint16      remain_str_len = 0;
    GUI_RECT_T  text_rect = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    SCI_ASSERT (GUIEDIT_TYPE_PHONENUM == baseedit_ctrl_ptr->type); /*assert verified*/

    //get reverse string
    BASEFLEX_GetReverseString (baseflex_ctrl_ptr, &str_ptr, &str_len);

    SCI_ASSERT (PNULL != baseedit_ctrl_ptr->line_info_ptr); /*assert verified*/

    //get text rect
    text_rect = BASEEDIT_GetTextRect (
            baseedit_ctrl_ptr,
            BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr));

    //get text width
    text_width = (uint16) (text_rect.right - text_rect.left + 1);

    //get total line number
    total_line_num = VTLBASEEDIT_GetLineNum (baseedit_ctrl_ptr, text_width);

    //get line info
    while (start_pos < str_len)
    {
        //reset param
        char_num     = 0;
        string_pixel = 0;

        //get remain string
        remain_str_ptr = str_ptr + start_pos;
        remain_str_len = (uint16) (str_len - start_pos);

        GUI_GetStringInfoInLineWithDir (
                TEXT_DIR_LTR,
                remain_str_ptr,
                remain_str_len,
                baseedit_ctrl_ptr->common_theme.font.font,
                (uint8) baseedit_ctrl_ptr->common_theme.char_space,
                text_width,
                &char_num,
                &string_pixel,
                FALSE);

        //parse error string
        if ( (0 == char_num) && (0 == string_pixel))
        {
            SCI_ASSERT ("ParseMultiLineEditTextExt:parse error!"); /*assert verified*/
        }

        start_pos = (uint16) (start_pos + char_num);
        line_index++;

        //set line start position and text direction
        baseedit_ctrl_ptr->line_info_ptr[total_line_num - line_index].start_pos = (uint16) (str_len - start_pos);

        //set direction
        baseedit_ctrl_ptr->line_info_ptr[total_line_num - line_index].direction = TEXT_DIR_LTR;
    }

    //reset other line info,[CR 3 char/4 line]
    SCI_MEMSET (
            &baseedit_ctrl_ptr->line_info_ptr[line_index],
            0,
            ( (baseedit_ctrl_ptr->str_max_len + 1 - line_index) *sizeof (GUIEDIT_LINE_T)));

    //total line number
    baseedit_ctrl_ptr->total_line_num = line_index;

    if (0 == baseedit_ctrl_ptr->total_line_num)
    {
        baseedit_ctrl_ptr->total_line_num = 1;
    }

    //set top line index
    BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);

    //free reverse string
    BASEFLEX_FreeReverseString (&str_ptr);
}

/*****************************************************************************/
//  Description : handle parse error string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleParseErrorStr (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              str_len         //in:
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //SCI_TRACE_LOW:"HandleParseErrorStr: %d position string is error!"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_3778_112_2_18_3_15_41_121, (uint8*) "d", str_len);

    //modify string len
    baseedit_ctrl_ptr->str_len = str_len;

    //modify current cursor position
    if (baseedit_ctrl_ptr->cursor.cur_pos >= str_len)
    {
        if (0 < str_len)
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (str_len - 1));
        }
        else
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, 0);
        }
    }

    //modify current highlight position
    if (baseedit_ctrl_ptr->highlight.start_pos >= str_len)
    {
        if (0 < str_len)
        {
            VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, (uint16) (str_len - 1), 0, SETHIGHLIGHT_START);
        }
        else
        {
            VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 0, SETHIGHLIGHT_START);
        }
    }

    if (baseedit_ctrl_ptr->highlight.end_pos >= str_len)
    {
        if (0 < str_len)
        {
            VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, (uint16) (str_len - 1), SETHIGHLIGHT_END);
        }
        else
        {
            VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 0, SETHIGHLIGHT_END);
        }
    }

    if (baseedit_ctrl_ptr->highlight.start_pos == baseedit_ctrl_ptr->highlight.end_pos)
    {
        baseedit_ctrl_ptr->highlight.is_underline = FALSE;
    }
}


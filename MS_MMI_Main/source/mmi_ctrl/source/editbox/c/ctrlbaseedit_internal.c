/*****************************************************************************
** File Name:      ctrlbaseedit_internal.c                                   *
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

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "ctrlbaseedit.h"
#include "mmi_gui_trc.h"
#include "mmi_theme.h"
#include "ctrlform_export.h"

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
//  Description : adjust rect top position by align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustRectTopByAlign (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    GUI_RECT_T          *rect_ptr           //in/out:
);

/*****************************************************************************/
//  Description : get line string pixel
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetEditLineStrPixel (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              line_index,     //in:
    uint16              rect_width      //in:
);

/*****************************************************************************/
//  Description : notify parent edit change msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SendChangeMsg (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    BOOLEAN             is_line_num_change //in:
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_Init(
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr
)
{
    // 以下这些设置，因为需要各个子类的配置信息，而这些信息是在构造时才初始化，
    // 这样时序上就有问题，所以放在这个消息里做，让子类调用

    //alloc string memory
    SCI_ASSERT (PNULL == baseedit_ctrl_ptr->str_ptr); /*assert verified*/
    baseedit_ctrl_ptr->str_ptr = BASEEDIT_AllocStrMemory (baseedit_ctrl_ptr->str_max_len);

    //alloc line info memory
    SCI_ASSERT (PNULL == baseedit_ctrl_ptr->line_info_ptr); /*assert verified*/
    baseedit_ctrl_ptr->line_info_ptr = BASEEDIT_AllocLineMemory (baseedit_ctrl_ptr->str_max_len);
}

/*****************************************************************************/
//  Description : alloc edit string memory
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC wchar* BASEEDIT_AllocStrMemory (
    uint16     max_len
)
{
    wchar   *edit_str_ptr = PNULL;

    if (0 < max_len)
    {
        edit_str_ptr = SCI_ALLOC_APP ( (max_len + 1) * sizeof (wchar));

        SCI_MEMSET (edit_str_ptr, 0, ( (max_len + 1) *sizeof (wchar)));
    }

    return (edit_str_ptr);
}

/*****************************************************************************/
//  Description : alloc edit line info memory
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_LINE_T* BASEEDIT_AllocLineMemory (
    uint16   max_len
)
{
    GUIEDIT_LINE_T  *line_info_ptr = PNULL;

    if (0 < max_len)
    {
        line_info_ptr = SCI_ALLOC_APP ( (max_len + 1) * sizeof (GUIEDIT_LINE_T));

        SCI_MEMSET (line_info_ptr, 0, ( (max_len + 1) *sizeof (GUIEDIT_LINE_T)));
    }

    return (line_info_ptr);
}

/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E BASEEDIT_NotifyParentMsg (
    MMI_HANDLE_T        ctrl_handle,
    MMI_MESSAGE_ID_E    notify_msg
)
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;

    switch (notify_msg)
    {
    case MSG_NOTIFY_OK:
    case MSG_NOTIFY_MIDSK:
    case MSG_NOTIFY_CANCEL:
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
    case MSG_NOTIFY_EDITBOX_MAXTEXT:
        //notify parent control
        result = CTRLMSG_PostNotify (ctrl_handle, notify_msg);
        break;

    default:
        //SCI_TRACE_LOW:"GUIEDIT_NotifyParentMsg:notify_msg = %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_INTERNAL_467_112_2_18_3_16_5_131, (uint8*) "d", notify_msg);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : notify parent total line number has change
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_NotifyLineMsg (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              prev_total_num  //in:
)
{
    BOOLEAN     result = FALSE;

    //post parent control
    if (prev_total_num == baseedit_ctrl_ptr->total_line_num)
    {
        //set cursor rect is invalid
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

        result = BASEEDIT_NotifyCursorMsg (baseedit_ctrl_ptr);
    }
    else
    {
        result = SendChangeMsg (baseedit_ctrl_ptr, TRUE);
    }

    return (result);
}

/*****************************************************************************/
//  Description : notify parent edit move cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_NotifyCursorMsg (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    BOOLEAN         result = FALSE;
    GUI_RECT_T      cursor_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if (baseedit_ctrl_ptr->is_rect_adaptive)
    {
        //get cursor rect
        if (BASEEDIT_GetCursorRect (baseedit_ctrl_ptr, PNULL, &cursor_rect))
        {
            //current cursor is not display all
            if ((cursor_rect.top < base_ctrl_ptr->display_rect.top) ||
                (cursor_rect.bottom > base_ctrl_ptr->display_rect.bottom))
            {
                result = SendChangeMsg (baseedit_ctrl_ptr, FALSE);
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : display edit background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplayBg (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    GUI_RECT_T          bg_rect
)
{
    GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //set image rect
    img_rect.left   = (int16) (bg_rect.left - base_ctrl_ptr->rect.left);
    img_rect.top    = (int16) (bg_rect.top - base_ctrl_ptr->rect.top);
    img_rect.right  = (int16) (img_rect.left + bg_rect.right - bg_rect.left);
    img_rect.bottom = (int16) (img_rect.top + bg_rect.bottom - bg_rect.top);

    //set bg display info
    bg_display.is_transparent = baseedit_ctrl_ptr->is_bg_transparent;
    bg_display.rect           = base_ctrl_ptr->rect;
    bg_display.display_rect   = bg_rect;
    bg_display.img_rect       = img_rect;
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = base_ctrl_ptr->win_handle;

    //display bg
    GUI_DisplayBg (
            &baseedit_ctrl_ptr->common_theme.bg,
            &bg_display,
            &base_ctrl_ptr->lcd_dev_info);
}

/*****************************************************************************/
//  Description : update text string bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_UpdateTextBg (
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr,     //in:
    uint16                  start_line_index,       //in:
    uint16                  end_line_index          //in:
)
{
    uint16          i = 0;
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      bg_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_RECT_T      edit_client_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    // get edit client rect
    edit_client_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);

    //get bg rect
    bg_rect = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, start_line_index);

    if (end_line_index > start_line_index)
    {
        line_rect = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, end_line_index);
        bg_rect.bottom = line_rect.bottom;
    }

    //get crossed rect
    if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, bg_rect)
        && GUI_IntersectRect(&cross_rect, edit_client_rect, cross_rect))
    {
        //display bg
        BASEEDIT_DisplayBg (baseedit_ctrl_ptr, cross_rect);

        //draw dividing line
        if ((0 < baseedit_ctrl_ptr->common_theme.dividing_line_width) &&
            (end_line_index > start_line_index))
        {
            for (i = start_line_index; i < end_line_index; i++)
            {
                //get line rect
                line_rect        = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, i);
                line_rect.top    = (int16) (line_rect.bottom + 1);
                line_rect.bottom = (int16) (line_rect.top + baseedit_ctrl_ptr->common_theme.dividing_line_width - 1);

                //get crossed rect
                if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, line_rect)
                    && GUI_IntersectRect(&cross_rect, edit_client_rect, cross_rect))
                {
                    //draw line
                    GUI_FillRect (
                            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
                            cross_rect,
                            baseedit_ctrl_ptr->common_theme.dividing_line_color);
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : get edit display rect height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetDispHeight(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16      display_height = 0;
    GUI_RECT_T  edit_rect = {0};

    //get text rect
    edit_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);

    //get display height
    display_height = (uint16)(edit_rect.bottom - edit_rect.top + 1);

    return (display_height);
}

/*****************************************************************************/
//  Description : get edit rect,except border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
// 这里拿edit的客户区，该区域要扣除边框和margin区域
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEEDIT_GetClientRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    BOOLEAN         is_border = TRUE;
    GUI_RECT_T      edit_rect = {0};
    GUI_RECT_T      zero_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //edit is exist border
    if ((!baseedit_ctrl_ptr->is_permit_border) ||
        (GUI_BORDER_NONE == baseedit_ctrl_ptr->common_theme.border.type))
    {
        is_border = FALSE;
    }

    if ((is_border) && (!GUI_IsRectEmpty (base_ctrl_ptr->rect)))
    {
        edit_rect.left   = (int16) (base_ctrl_ptr->rect.left + baseedit_ctrl_ptr->common_theme.border.width);
        edit_rect.right  = (int16) (base_ctrl_ptr->rect.right - baseedit_ctrl_ptr->common_theme.border.width);
        edit_rect.top    = (int16) (base_ctrl_ptr->rect.top + baseedit_ctrl_ptr->common_theme.border.width);
        edit_rect.bottom = (int16) (base_ctrl_ptr->rect.bottom - baseedit_ctrl_ptr->common_theme.border.width);
    }
    else
    {
        edit_rect = base_ctrl_ptr->rect;
    }

    edit_rect.top += baseedit_ctrl_ptr->common_theme.margin_tb_space;
    edit_rect.bottom -= baseedit_ctrl_ptr->common_theme.margin_tb_space;
    edit_rect.left += baseedit_ctrl_ptr->common_theme.margin_lr_space;
    edit_rect.right -= baseedit_ctrl_ptr->common_theme.margin_lr_space;

    if (GUI_IsInvalidRectEx(edit_rect))
    {
        edit_rect = zero_rect;
    }

    SCI_ASSERT (edit_rect.top <= edit_rect.bottom); /*assert verified*/

    return (edit_rect);
}

/*****************************************************************************/
//  Description : get edit line height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetLineHeight(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16          font_height = 0;
    uint16          line_height = 0;

    //get font height
    font_height = BASEEDIT_GetFontHeight (baseedit_ctrl_ptr);

    //get line height
    line_height = (uint16) (font_height + baseedit_ctrl_ptr->common_theme.line_space);

    return line_height;
}

/*****************************************************************************/
//  Description : get edit line rect,except dividing line,progress bar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEEDIT_GetLineRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              line_index          //in:from 0
)
{
    uint16          line_height = 0;
    uint16          dividing_line_width = 0;
    uint16          top_line_index = 0;
    GUI_RECT_T      edit_rect = {0};
    GUI_RECT_T      line_rect = {0};

    //adjust line index
    top_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
    //SCI_ASSERT (line_index >= top_line_index); /*assert verified*/
    //line_index = (uint16) (line_index - top_line_index);

    //get line height
    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    //get edit rect
    edit_rect = BASEEDIT_GetClientRect (baseedit_ctrl_ptr);

    if (GUI_IsRectEmpty (edit_rect))
    {
        return line_rect;
    }

    //get dividing line width
    dividing_line_width = baseedit_ctrl_ptr->common_theme.dividing_line_width;

    //get line rect
    line_rect = edit_rect;

    //adjust line rect top
    if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
    {
        AdjustRectTopByAlign (baseedit_ctrl_ptr, &line_rect);
    }

    //set line rect
    line_rect.top    = (int16) (line_rect.top + line_height * line_index - baseedit_ctrl_ptr->top_line_offset);
    line_rect.bottom = (int16) (line_rect.top + line_height - dividing_line_width - 1);
    //SCI_ASSERT (line_rect.top < edit_rect.bottom); /*assert verified*/

    VTLBASEEDIT_CheckLineRect(baseedit_ctrl_ptr, &line_rect);

    //adjust rect
    if (line_rect.bottom > edit_rect.bottom)
    {
        line_rect.bottom = edit_rect.bottom;
    }

    return (line_rect);
}

/*****************************************************************************/
//  Description : get one line text rect,except margin
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEEDIT_GetTextRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              line_index      //in:from 0
)
{
    GUI_RECT_T      text_rect = {0};

    //get line rect
    text_rect = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, line_index);

    ////except margin
    //text_rect.left  = (uint16) (text_rect.left + baseedit_ctrl_ptr->common_theme.margin_lr_space);
    //text_rect.right = (uint16) (text_rect.right - baseedit_ctrl_ptr->common_theme.margin_lr_space);

    return (text_rect);
}

/*****************************************************************************/
//  Description : get line string length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetLineStrLen (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    uint16              line_index          //in:from 0
)
{
    uint16      str_len = 0;

    if (line_index < baseedit_ctrl_ptr->total_line_num)
    {
        //is last line
        if ( (line_index + 1) == baseedit_ctrl_ptr->total_line_num)
        {
            if (BASEEDIT_IsFixedFormat (baseedit_ctrl_ptr))
            {
                str_len = (uint16) (baseedit_ctrl_ptr->str_len - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
            }
            else
            {
                if ((GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style) &&
                    (0 < baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos))
                {
                    str_len = (uint16) (baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
                }
                else
                {
                    str_len = (uint16) (baseedit_ctrl_ptr->str_len - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
                }
            }
        }
        else
        {
            str_len = (uint16) (baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
        }
    }

    return (str_len);
}

/*****************************************************************************/
//  Description : adjust cursor/highlight rect left position by align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_AdjustLeftByAlign (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    uint16              line_index,         //in:
    GUI_RECT_T          *rect_ptr           //in/out:
)
{
    uint16          string_pixel = 0;
    GUI_ALIGN_E     align = ALIGN_LEFT;

    //get string pixel
    string_pixel = GetEditLineStrPixel (
            baseedit_ctrl_ptr,
            line_index,
            (uint16) (rect_ptr->right - rect_ptr->left + 1));

    //get align
    align = VTLBASEEDIT_GetAlign (baseedit_ctrl_ptr);

    //get sting start position
    switch (align)
    {
    case ALIGN_RIGHT:
    case ALIGN_RVMIDDLE:
        if (rect_ptr->right > string_pixel)
        {
            //set left start
            if (0 < string_pixel)
            {
                rect_ptr->left = (int16) (rect_ptr->right - string_pixel + 1);
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
            rect_ptr->left = (int16) ( (rect_ptr->left + rect_ptr->right - string_pixel + 1) >> 1);
        }

        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : get font height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetFontHeight (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16          font_height = 0;

    switch (baseedit_ctrl_ptr->type)
    {
    case GUIEDIT_TYPE_TEXT:
        font_height = GUI_GetFontHeight (baseedit_ctrl_ptr->common_theme.font.font, UNICODE_HANZI);
        break;

    case GUIEDIT_TYPE_PHONENUM:
    case GUIEDIT_TYPE_DIGITAL:
    case GUIEDIT_TYPE_PASSWORD:
    case GUIEDIT_TYPE_LIST:
    case GUIEDIT_TYPE_DATE:
    case GUIEDIT_TYPE_TIME:
    case GUIEDIT_TYPE_IP:
    default:
        font_height = GUI_GetFontHeight (baseedit_ctrl_ptr->common_theme.font.font, UNICODE_ASCII);
        break;
    }

    return (font_height);
}

/*****************************************************************************/
//  Description : get edit separator char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC char BASEEDIT_GetSeparatorChar (
    GUIEDIT_SEPARATOR_E    separator_style
)
{
    char        separator_char = 0;

    switch (separator_style)
    {
    case GUIEDIT_SEPARATOR_DOT:
        separator_char = '.';
        break;

    case GUIEDIT_SEPARATOR_LINE:
        separator_char = '-';
        break;

    case GUIEDIT_SEPARATOR_COMMA:
        separator_char = ',';
        break;

    case GUIEDIT_SEPARATOR_BLANK:
        separator_char = ' ';
        break;

    case GUIEDIT_SEPARATOR_COLON:
        separator_char = ':';
        break;

    case GUIEDIT_SEPARATOR_SOLIDUS:
        separator_char = '/';
        break;

    case GUIEDIT_SEPARATOR_SEMICOLON:
        separator_char = ';';
        break;

    default:
        //SCI_TRACE_LOW:"GetSeparatorChar:separator style %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_INTERNAL_843_112_2_18_3_16_6_133, (uint8*) "d", separator_style);
        break;
    }

    return (separator_char);
}

/*****************************************************************************/
//  Description : edit is fixed format,for example time,date,IP
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_IsFixedFormat (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    BOOLEAN     result = FALSE;

    switch (baseedit_ctrl_ptr->type)
    {
    case GUIEDIT_TYPE_TEXT:
    case GUIEDIT_TYPE_PHONENUM:
    case GUIEDIT_TYPE_DIGITAL:
    case GUIEDIT_TYPE_PASSWORD:
    case GUIEDIT_TYPE_LIST:
        result = FALSE;
        break;

    case GUIEDIT_TYPE_DATE:
    case GUIEDIT_TYPE_TIME:
    case GUIEDIT_TYPE_IP:
        result = TRUE;
        break;

    default:
        //SCI_TRACE_LOW:"GUIEDIT_IsFixedFormat:edit type %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_INTERNAL_1147_112_2_18_3_16_7_134, (uint8*) "d", baseedit_ctrl_ptr->type);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : convert line index,when display from right and bottom,need convert
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC uint16 BASEEDIT_ConvertLineIndex (
//    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
//    uint16              index
//)
//{
//    uint16  line_index = index;

//    if ( (baseedit_ctrl_ptr->total_line_num < baseedit_ctrl_ptr->page_line_num) &&
//            (GUIEDIT_DISPLAY_DIR_RB == baseedit_ctrl_ptr->display_dir))
//    {
//        line_index = (uint16) (baseedit_ctrl_ptr->page_line_num + line_index - baseedit_ctrl_ptr->total_line_num);
//    }

//    return (line_index);
//}

/*****************************************************************************/
//  Description : 用来判断字符是否为换行符
//  Global resource dependence :
//  Author: Jassmine
//  Note:
// 由于密码风格不需要换行，所以永远也不可能是换行符
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_IsNewLineChar (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              char_index
)
{
    BOOLEAN is_new_line = FALSE;

    if ((GUIEDIT_TYPE_PASSWORD != baseedit_ctrl_ptr->type) &&
        (0 < char_index) && (char_index <= baseedit_ctrl_ptr->str_len) &&
        (GUI_IsNewlineChar (baseedit_ctrl_ptr->str_ptr[char_index - 1])))
    {
        //carriage return or line feed
        is_new_line = TRUE;
    }

    return is_new_line;
}

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetPageLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16          font_height = 0;
    uint16          line_height = 0;
    uint16          page_height = 0;
    uint16          line_num = 0;
    GUI_RECT_T      edit_rect = {0};

    //get font height
    font_height = BASEEDIT_GetFontHeight (baseedit_ctrl_ptr);

    //get line height
    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    //get edit rect
    edit_rect = BASEEDIT_GetClientRect (baseedit_ctrl_ptr);

    if (!GUI_IsRectEmpty (edit_rect))
    {
        //get page height
        page_height = (uint16) (edit_rect.bottom - edit_rect.top + 1);

        line_num = (uint16) (page_height / line_height);

        if ( (page_height % line_height) >= font_height)
        {
            line_num++;
        }

        //modify line number for single edit
        if ((GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style) && (0 < line_num))
        {
            line_num = 1;
        }

        if (line_num == 0)
        {
            line_num = 1;
        }
    }

    return (line_num);
}

/*****************************************************************************/
//  Description : get total content height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 BASEEDIT_GetTotalHeight(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16  line_height = 0;
    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    return line_height * baseedit_ctrl_ptr->total_line_num;
}

/*****************************************************************************/
//  Description : get display top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetTopLineIndex (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16  line_height = 0;
    int32   top_index = 0;

    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    top_index = baseedit_ctrl_ptr->top_line_offset / line_height;
    if (top_index < 0)
    {
        top_index = 0;
    }
    else if (top_index >= (int32)baseedit_ctrl_ptr->total_line_num)
    {
        if (baseedit_ctrl_ptr->total_line_num > 0)
        {
            top_index = baseedit_ctrl_ptr->total_line_num - 1;
        }
        else
        {
            top_index = 0;
        }
    }

    return (uint16)top_index;
}

/*****************************************************************************/
//  Description : get display bottom line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetBottomLineIndex (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    uint16  line_height = 0;
    int32   bottom_index = 0;
    int32   bottom_offset = 0;

    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);
    bottom_offset = baseedit_ctrl_ptr->top_line_offset + BASEEDIT_GetDispHeight(baseedit_ctrl_ptr);

    bottom_index = bottom_offset / line_height;

    if (bottom_index < 0)
    {
        bottom_index = 0;
    }
    else if (bottom_index >= (int32)baseedit_ctrl_ptr->total_line_num)
    {
        if (baseedit_ctrl_ptr->total_line_num > 0)
        {
            bottom_index = baseedit_ctrl_ptr->total_line_num - 1;
        }
        else
        {
            bottom_index = 0;
        }
    }

    return (uint16)bottom_index;
}

/*****************************************************************************/
//  Description : get top line offset range
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_GetTopLineOffsetRange(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    int32               *min_top_line_offset_ptr,
    int32               *max_top_line_offset_ptr
)
{
    uint16  display_height = BASEEDIT_GetDispHeight(baseedit_ctrl_ptr);
    uint32  total_height = BASEEDIT_GetTotalHeight(baseedit_ctrl_ptr);
    int32   min_top_line_offset = 0;
    int32   max_top_line_offset = 0;

    if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)       // 单行
    {
        min_top_line_offset = 0;
        max_top_line_offset = 0;
    }
    else if ((total_height < display_height) &&
             (GUIEDIT_DISPLAY_DIR_RB == baseedit_ctrl_ptr->display_dir))  // 多行，从下往上显示
    {
        min_top_line_offset = (total_height - display_height);
        max_top_line_offset = min_top_line_offset;
    }
    else
    {
        min_top_line_offset = 0;

        if (total_height > display_height)
        {
            max_top_line_offset = total_height - display_height;
        }
        else
        {
            max_top_line_offset = 0;
        }
    }

    if (PNULL != min_top_line_offset_ptr)
    {
        *min_top_line_offset_ptr = min_top_line_offset;
    }

    if (PNULL != max_top_line_offset_ptr)
    {
        *max_top_line_offset_ptr = max_top_line_offset;
    }
}

/*****************************************************************************/
//  Description : adjust rect top position by align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustRectTopByAlign (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    GUI_RECT_T          *rect_ptr           //in/out:
)
{
    uint16          font_height = 0;
    GUI_ALIGN_E     align = ALIGN_LEFT;

    //get font height
    font_height = BASEEDIT_GetFontHeight (baseedit_ctrl_ptr);

    //get align
    align = VTLBASEEDIT_GetAlign (baseedit_ctrl_ptr);

    //get sting start position
    switch (align)
    {
    case ALIGN_BOTTOM:
        //set cursor top
        rect_ptr->top = (int16) (rect_ptr->bottom - font_height + 1);
        break;

    case ALIGN_VMIDDLE:
    case ALIGN_LVMIDDLE:
    case ALIGN_RVMIDDLE:
    case ALIGN_HVMIDDLE:
        //set cursor top
        rect_ptr->top = (int16) ( (rect_ptr->top + rect_ptr->bottom - font_height + 1) >> 1);
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
LOCAL uint16 GetEditLineStrPixel (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              line_index,     //in:
    uint16              rect_width      //in:
)
{
    wchar       *str_ptr = PNULL;
    uint16      str_len = 0;
    uint16      char_num = 0;
    uint16      string_pixel = 0;

    //get string
    str_ptr = baseedit_ctrl_ptr->str_ptr + baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos;
    str_len = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, line_index);

    //get string pixel
    GUI_GetStringInfoInLine (
            str_ptr, str_len,
            baseedit_ctrl_ptr->common_theme.font.font,
            (uint8) (baseedit_ctrl_ptr->common_theme.char_space),
            rect_width,
            &char_num,
            &string_pixel,
            FALSE);

    return (string_pixel);
}

/*****************************************************************************/
//  Description : notify parent edit change msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SendChangeMsg (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    BOOLEAN             is_line_num_change //in:
)
{
    BOOLEAN                 result = FALSE;
    GUIFORM_NOTIFY_LINE_T   notify_line = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //post parent control
    if (0 != MMK_GetParentCtrlHandle (base_ctrl_ptr->handle))
    {
        //set notify param
        notify_line.is_line_num_change = is_line_num_change;
        notify_line.ctrl_handle = base_ctrl_ptr->handle;
        CTRLMSG_SendNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_LINE_CHANGE, &notify_line);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : edit's parent is control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_IsParentCtrl(
                                     CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
                                     )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T    parent_handle = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;
    
    if (PNULL != base_ctrl_ptr)
    {
        //get parent control
        parent_handle = MMK_GetParentCtrlHandle(base_ctrl_ptr->handle);

        if (0 != parent_handle)
        {
            result = TRUE;
        }
    }

    return (result);
}


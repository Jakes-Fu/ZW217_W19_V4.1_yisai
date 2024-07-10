/*****************************************************************************
** File Name:      ctrlstatusbar_scrollmsg.c                                       *
** Author:                                                                   *
** Date:              09/2012                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012     xiyuan.ma      Create                                   *
******************************************************************************/

#ifdef GUIF_STATUSBAR_SCROLL
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmk_timer.h"
#include "mmk_msg.h"
#include "mmi_gui_trc.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guistring.h"

#include "ctrlstatusbar_scrollmsg.h"
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL STRUCT DEFINITION                          */
/*---------------------------------------------------------------------------*/

//需要滚动显示的数据
LOCAL GUISTBSCROLLMSG_SCROLL_MESSAGE_T s_statusbar_scrollmsg = {0};

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get text style
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetScrollmsgTextStyle (
    GUISTR_STYLE_T *style_ptr
);

/*****************************************************************************/
//  Description : 显示完一个item
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void ConsumeOneItem (
    void
);

/*****************************************************************************/
//  Description : 显示完一行
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void ConsumeOneLine (
    void
);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 得到当前（第几）帧
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLSTBSCROLLMSG_GetCurFrame (
    void
)
{
    return s_statusbar_scrollmsg.cur_frame;
}



/*****************************************************************************/
//  Description : 得到当前滚动状态
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC GUISTBSCROLLMSG_SCROLL_STATE_E CTRLSTBSCROLLMSG_GetCurScrollState (
    void
)
{
    GUISTBSCROLLMSG_SCROLL_STATE_E scroll_state = GUISTBSCROLLMSG_SCROLL_NONE;

    if (0 != s_statusbar_scrollmsg.cur_frame)
    {
        scroll_state = s_statusbar_scrollmsg.scroll_state;
    }
    else
    {
        switch (s_statusbar_scrollmsg.scroll_state)
        {
        case GUISTBSCROLLMSG_SCROLL_NONE:

            if (CTRLSTBSCROLLMSG_GetCurLine (PNULL))
            {
                scroll_state = GUISTBSCROLLMSG_SCROLL_START;
            }
            else
            {
                scroll_state = GUISTBSCROLLMSG_SCROLL_NONE;
            }

            break;

        case GUISTBSCROLLMSG_SCROLL_START:

            if (CTRLSTBSCROLLMSG_GetNextLine (PNULL))
            {
                scroll_state = GUISTBSCROLLMSG_SCROLL_RUNNING;
            }
            else
            {
                scroll_state = GUISTBSCROLLMSG_SCROLL_END;
            }

            break;

        case GUISTBSCROLLMSG_SCROLL_RUNNING:

            if (CTRLSTBSCROLLMSG_GetNextLine (PNULL))
            {
                scroll_state = GUISTBSCROLLMSG_SCROLL_RUNNING;
            }
            else
            {
                scroll_state = GUISTBSCROLLMSG_SCROLL_END;
            }

            break;

        case GUISTBSCROLLMSG_SCROLL_END:

            if (CTRLSTBSCROLLMSG_GetNextLine (PNULL))
            {
                scroll_state = GUISTBSCROLLMSG_SCROLL_RUNNING;
            }
            else
            {
                scroll_state = GUISTBSCROLLMSG_SCROLL_NONE;
            }

            break;

        default:
            break;
        }
    }

    s_statusbar_scrollmsg.scroll_state = scroll_state;

    return scroll_state;
}



/*****************************************************************************/
//  Description : 显示完一行
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTBSCROLLMSG_ConsumeOneFrame (
    void
)
{
    if ( (GUISTBSCROLLMSG_SCROLL_FRAME_NUM - 1) == s_statusbar_scrollmsg.cur_frame)
    {
        if (GUISTBSCROLLMSG_SCROLL_START != s_statusbar_scrollmsg.scroll_state)
        {
            //避免重复计算，start不要consume最后一帧
            ConsumeOneLine();
        }

        s_statusbar_scrollmsg.cur_frame = 0;
    }
    else
    {
        s_statusbar_scrollmsg.cur_frame++;
    }
}



/*****************************************************************************/
//  Description : get text style
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetScrollmsgTextStyle (
    GUISTR_STYLE_T *style_ptr
)
{
    if (PNULL != style_ptr)
    {
        style_ptr->angle = ANGLE_0;
        style_ptr->align = ALIGN_HVMIDDLE;
        style_ptr->font = DP_FONT_16;
        style_ptr->font_color = 0xffff;//MMI_WHITE_COLOR;
    }
}



/*****************************************************************************/
//  Description : 显示完一行
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void ConsumeOneLine (
    void
)
{
    MMI_STRING_T kstring = {0};
    GUISTR_STYLE_T      str_style = {0};
    uint16 line_length = 0;
    uint16 pixel_num_line = 0;
    uint16 width = GUISTBSCROLLMSG_TEXT_WIDTH;

    GetScrollmsgTextStyle (&str_style);
    kstring = s_statusbar_scrollmsg.item[s_statusbar_scrollmsg.cur_item_index].string_info;

    if (s_statusbar_scrollmsg.cur_str_pos < kstring.wstr_len)
    {
        if (GUI_GetStringInfoInLine (
                    kstring.wstr_ptr + s_statusbar_scrollmsg.cur_str_pos,
                    kstring.wstr_len - s_statusbar_scrollmsg.cur_str_pos,
                    str_style.font,
                    str_style.char_space,
                    width,  //字符串所占的宽度
                    &line_length,
                    &pixel_num_line,
                    TRUE
                ))
        {
            //成功
            s_statusbar_scrollmsg.cur_str_pos += line_length;

            if (s_statusbar_scrollmsg.cur_str_pos >= kstring.wstr_len)
            {
                ConsumeOneItem();
            }
        }
        else
        {
            //失败
            ConsumeOneItem();
        }
    }
    else
    {
        ConsumeOneItem();
    }
}



/*****************************************************************************/
//  Description : 找到下一个item
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindNextItem (
    uint32 *item_index_ptr
)
{
    uint32 i = 0;
    BOOLEAN is_find = FALSE;

    for (i = s_statusbar_scrollmsg.cur_item_index + 1; i < GUISTBSCROLLMSG_MAX_SCROLL_ITEM_NUM; i++)
    {
        if (s_statusbar_scrollmsg.item[i].is_valid)
        {
            is_find = TRUE;
            break;
        }
    }

    if (!is_find)
    {
        for (i = 0; i < s_statusbar_scrollmsg.cur_item_index; i++)
        {
            if (s_statusbar_scrollmsg.item[i].is_valid)
            {
                is_find = TRUE;
                break;
            }
        }
    }

    if (is_find)
    {
        *item_index_ptr = i;
    }

    return is_find;
}



/*****************************************************************************/
//  Description : 显示完一个item
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void ConsumeOneItem (
    void
)
{
    uint32 index = 0;
    BOOLEAN is_find = FALSE;

    s_statusbar_scrollmsg.item[s_statusbar_scrollmsg.cur_item_index].is_valid = FALSE;
    is_find = FindNextItem (&index);

    if (is_find)
    {
        s_statusbar_scrollmsg.cur_item_index = index;
    }
    else
    {
        //找不到下一个，停止
        s_statusbar_scrollmsg.cur_item_index = 0;
    }

    s_statusbar_scrollmsg.cur_str_pos = 0;
}



/*****************************************************************************/
//  Description : 添加一条滚动信息
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBSCROLLMSG_AppendScrollItem (
    GUISTBSCROLLMSG_SCROLL_ITEM_T *item_ptr
)
{
    uint32 i = 0;
    BOOLEAN is_find = FALSE;
    BOOLEAN result = FALSE;
    uint32 index = 0;

    SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_SCROLLMSG_305_112_2_18_3_21_40_345, (uint8*) "d", item_ptr);

    if (PNULL == item_ptr)
    {
        return FALSE;
    }

    for (i = 0; i < GUISTBSCROLLMSG_MAX_SCROLL_ITEM_NUM; i++)
    {
        if (item_ptr->type == s_statusbar_scrollmsg.item[i].type)
        {
            is_find = TRUE;
            break;
        }
    }

    if (!is_find)
    {
        for (i = 0; i < GUISTBSCROLLMSG_MAX_SCROLL_ITEM_NUM; i++)
        {
            if (!s_statusbar_scrollmsg.item[i].is_valid)
            {
                is_find = TRUE;
                break;
            }
        }
    }

    if (is_find && (i < GUISTBSCROLLMSG_MAX_SCROLL_ITEM_NUM))
    {
        if (PNULL != s_statusbar_scrollmsg.item[i].string_info.wstr_ptr)
        {
            SCI_FREE (s_statusbar_scrollmsg.item[i].string_info.wstr_ptr);
        }

        SCI_MEMSET (& (s_statusbar_scrollmsg.item[i]), 0x00, sizeof (GUISTBSCROLLMSG_SCROLL_ITEM_T));
        s_statusbar_scrollmsg.item[i].is_valid = TRUE;
        s_statusbar_scrollmsg.item[i].icon_id = item_ptr->icon_id;
        s_statusbar_scrollmsg.item[i].type = item_ptr->type;
        s_statusbar_scrollmsg.item[i].string_info.wstr_ptr = SCI_ALLOCA (item_ptr->string_info.wstr_len * sizeof (wchar));

        if (PNULL != s_statusbar_scrollmsg.item[i].string_info.wstr_ptr)
        {
            s_statusbar_scrollmsg.item[i].string_info.wstr_len = item_ptr->string_info.wstr_len;
            SCI_MEMCPY (s_statusbar_scrollmsg.item[i].string_info.wstr_ptr, item_ptr->string_info.wstr_ptr,
                        item_ptr->string_info.wstr_len*sizeof (wchar));

            //调整cur_item_index
            if (GUISTBSCROLLMSG_SCROLL_NONE == s_statusbar_scrollmsg.scroll_state)
            {
                if (FindNextItem (&index))
                {
                    s_statusbar_scrollmsg.cur_item_index = index;
                }
            }
            else if (i == s_statusbar_scrollmsg.cur_item_index)
            {
                s_statusbar_scrollmsg.cur_str_pos = 0;
                s_statusbar_scrollmsg.cur_frame = 0;
            }

            result = TRUE;
        }
    }

    return result;
}



/*****************************************************************************/
//  Description : 是否由应用停止
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTBSCROLLMSG_SetIsValid (
    GUISTBSCROLLMSG_SCROLL_ITEM_E type,
    BOOLEAN is_valid
)
{
    GUISTBSCROLLMSG_SCROLL_ITEM_T *cur_item_ptr = PNULL;

    cur_item_ptr = & (s_statusbar_scrollmsg.item[s_statusbar_scrollmsg.cur_item_index]);

    if (type == cur_item_ptr->type)
    {
        cur_item_ptr->is_valid = is_valid;
    }
}



/*****************************************************************************/
//  Description : 得到当前行显示数据
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBSCROLLMSG_GetCurLine (
    GUISTBSCROLLMSG_SCROLL_ITEM_T *item_ptr
)
{
    GUISTBSCROLLMSG_SCROLL_ITEM_T *cur_item_ptr = PNULL;
    uint16 line_length = 0;
    uint16 pixel_num_line = 0;
    BOOLEAN result = FALSE;
    uint16 width = GUISTBSCROLLMSG_TEXT_WIDTH;
    GUISTR_STYLE_T      str_style = {0};
    uint32 cur_str_pos = 0;

    GetScrollmsgTextStyle (&str_style);
    cur_str_pos = s_statusbar_scrollmsg.cur_str_pos;
    cur_item_ptr = & (s_statusbar_scrollmsg.item[s_statusbar_scrollmsg.cur_item_index]);

    if (cur_item_ptr->is_valid && (cur_item_ptr->string_info.wstr_len > s_statusbar_scrollmsg.cur_str_pos)
            && (PNULL != cur_item_ptr->string_info.wstr_ptr))
    {
        if (GUI_GetStringInfoInLine (
                    cur_item_ptr->string_info.wstr_ptr + cur_str_pos,
                    cur_item_ptr->string_info.wstr_len - cur_str_pos,
                    str_style.font,
                    str_style.char_space,
                    width,  //字符串所占的宽度
                    &line_length,
                    &pixel_num_line,
                    TRUE
                ))
        {
            if (PNULL != item_ptr)
            {
                *item_ptr = *cur_item_ptr;
                item_ptr->string_info.wstr_ptr += s_statusbar_scrollmsg.cur_str_pos;
                item_ptr->string_info.wstr_len = line_length;
            }

            result = TRUE;
        }
    }

    return result;
}



/*****************************************************************************/
//  Description : 得到下一行显示数据
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBSCROLLMSG_GetNextLine (
    GUISTBSCROLLMSG_SCROLL_ITEM_T *item_ptr
)
{
    GUISTBSCROLLMSG_SCROLL_ITEM_T *cur_item_ptr = PNULL;
    uint16 line_length = 0;
    uint16 pixel_num_line = 0;
    uint16 width = GUISTBSCROLLMSG_TEXT_WIDTH;
    uint32 cur_str_pos = 0;
    BOOLEAN result = FALSE;
    uint32 index = 0;
    GUISTR_STYLE_T      str_style = {0};

    GetScrollmsgTextStyle (&str_style);
    cur_str_pos = s_statusbar_scrollmsg.cur_str_pos;
    cur_item_ptr = & (s_statusbar_scrollmsg.item[s_statusbar_scrollmsg.cur_item_index]);

    if (cur_item_ptr->is_valid && (cur_str_pos < cur_item_ptr->string_info.wstr_len)
            && (PNULL != cur_item_ptr->string_info.wstr_ptr))
    {
        if (GUI_GetStringInfoInLine (
                    cur_item_ptr->string_info.wstr_ptr + cur_str_pos,
                    cur_item_ptr->string_info.wstr_len - cur_str_pos,
                    str_style.font,
                    str_style.char_space,
                    width,  //字符串所占的宽度
                    &line_length,
                    &pixel_num_line,
                    TRUE
                ))
        {
            cur_str_pos += line_length;

            if (cur_str_pos < cur_item_ptr->string_info.wstr_len)
            {
                if (GUI_GetStringInfoInLine (
                            cur_item_ptr->string_info.wstr_ptr + cur_str_pos,
                            cur_item_ptr->string_info.wstr_len - cur_str_pos,
                            str_style.font,
                            str_style.char_space,
                            width,  //字符串所占的宽度
                            &line_length,
                            &pixel_num_line,
                            TRUE
                        ))
                {
                    //当前item下一行
                    if (PNULL != item_ptr)
                    {
                        *item_ptr = *cur_item_ptr;
                        item_ptr->string_info.wstr_ptr += cur_str_pos;
                        //item_ptr->string_info.wstr_len -= cur_str_pos;
                        item_ptr->string_info.wstr_len = line_length;
                    }

                    result = TRUE;
                }
            }
        }
    }

    if (!result)
    {
        //下一个item
        if (FindNextItem (&index))
        {
            if (PNULL != item_ptr)
            {
                *item_ptr = s_statusbar_scrollmsg.item[index];
            }

            result = TRUE;
        }
    }

    return result;
}


#endif //end of GUIF_STATUSBAR_SCROLL
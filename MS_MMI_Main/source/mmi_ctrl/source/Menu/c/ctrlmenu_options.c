/*****************************************************************************
** File Name:      ctrlmenu_options.c                                        *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "ctrlmenu.h"
#include "ctrlmenu_export.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmi_gui_trc.h"

#ifdef MMI_OPTIONMENU_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         TYPE AND CONSTANT                                 *
 *---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         LOCAL FUNCTION DECLARE                            *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 分配布局数据的内存
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AllocLayoutInfo(
                           CTRLMENU_OBJ_T   *menu_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : 重置布局数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ResetLayoutInfo(
                           CTRLMENU_OBJ_T   *menu_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : 计算一行里能容纳下多少个此宽度的item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//  存储数据的顺序与显示的顺序正好相反
/*****************************************************************************/
LOCAL void GetAllItemNumInLine(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                               uint16           *item_len_array_ptr,
                               uint16           item_count,
                               BOOLEAN          is_more_item,
                               uint16           min_width,
                               uint16           max_width
                               );

/*****************************************************************************/
//  Description : 计算一行里能容纳下多少个此宽度的item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetItemNumInLine(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              MMI_STRING_T      *text_ptr,
                              uint16            min_width,
                              uint16            max_width
                              );

/*****************************************************************************/
//  Description : 计算一行里容纳的item数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 返回下一个开始的序号
// 返回item_count表明start_index之后的所有item都能容纳下
/*****************************************************************************/
LOCAL uint16 CalculateLineNum(
                              uint16    *item_len_array_ptr,
                              uint16    item_count,
                              uint16    start_item_index,
                              uint16    max_num_in_line,
                              uint16    *num_in_line_ptr
                              );

/*****************************************************************************/
//  Description : 显示背景
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DisplayBackground(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                             GUI_RECT_T         *bg_rect_ptr
                             );

/*****************************************************************************/
//  Description : 显示item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DisplayItem(
                       CTRLMENU_OBJ_T       *menu_ctrl_ptr,     // [in]
                       uint16               item_index,         // [in]
                       GUI_RECT_T           clip_rect,          // [in]
                       GUI_LCD_DEV_INFO     *lcd_dev_info_ptr   // [in]
                       );

/*****************************************************************************/
//  Description : 显示边框
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DisplayBorder(
                         CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                         GUI_LCD_DEV_INFO   *lcd_dev_info_ptr
                         );

/*****************************************************************************/
//  Description : 更新焦点项
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void RefreshActiveItem(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                             uint16             cur_item_index
                             );
#endif

/*****************************************************************************/
//  Description : 临时隐藏前几项
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetItemVisibleByNum(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr, // [in]
                                  uint16            num,            // [in]
                                  BOOLEAN           visible         // [in]
                                  );

#ifdef MMI_OPTIONMENU_SUPPORT
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : optionsmenu 初始化
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_InitOptions(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                 )
{
    AllocLayoutInfo(menu_ctrl_ptr);

    ResetLayoutInfo(menu_ctrl_ptr);
}

/*****************************************************************************/
//  Description : 分配布局数据的内存
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AllocLayoutInfo(
                           CTRLMENU_OBJ_T   *menu_ctrl_ptr
                           )
{
    uint32  item_info_size = sizeof(CTRLMENU_OPTIONS_ITEM_T)*menu_ctrl_ptr->options_theme.max_item_num;

    if (PNULL == menu_ctrl_ptr->options_item_info_ptr)
    {
        menu_ctrl_ptr->options_item_info_ptr = SCI_ALLOC_APP(item_info_size);
    }

    if (PNULL == menu_ctrl_ptr->item_len_array_ptr)
    {
        menu_ctrl_ptr->item_len_array_ptr = SCI_ALLOC_APP(item_info_size);
    }

    if (PNULL == menu_ctrl_ptr->num_in_line_ptr)
    {
        menu_ctrl_ptr->num_in_line_ptr = SCI_ALLOC_APP(item_info_size);
    }
}

/*****************************************************************************/
//  Description : 重置布局数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ResetLayoutInfo(
                           CTRLMENU_OBJ_T   *menu_ctrl_ptr
                           )
{
    uint32  item_info_size = sizeof(CTRLMENU_OPTIONS_ITEM_T)*menu_ctrl_ptr->options_theme.max_item_num;

    SCI_MEMSET(menu_ctrl_ptr->options_item_info_ptr, 0, item_info_size);
    SCI_MEMSET(menu_ctrl_ptr->item_len_array_ptr, 0, item_info_size);
    SCI_MEMSET(menu_ctrl_ptr->num_in_line_ptr, 0, item_info_size);
}

/*****************************************************************************/
//  Description : handle msg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_OptionsSpecial(
                                            CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                            MMI_MESSAGE_ID_E    msg_id
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    switch (msg_id)
    {
    case MSG_APP_MENU:
        CTRLMENU_NotifyMsg(base_ctrl_ptr->handle, MSG_APP_CANCEL);
        break;

    case MSG_CTL_LOSE_FOCUS:
        menu_ctrl_ptr->is_item_pressed = FALSE;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    CTRLMENU_HandleAnimMsg(menu_ctrl_ptr,msg_id);
#endif

    return result;
}

/*****************************************************************************/
//  Description : 更新布局数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 现在区域包括:
//  1、顶上的阴影区
//  2、周围的边线
//  3、中间的item
/*****************************************************************************/
PUBLIC void CTRLMENU_OptionsLayout(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   )
{
    BOOLEAN is_more_item = FALSE;                       // 是否需要more项
    uint16  item_count = menu_ctrl_ptr->item_total_num; // item个数
    uint16  *item_len_array_ptr = menu_ctrl_ptr->item_len_array_ptr;    // 每个item的长度(这里记的是一行能容纳多少个此长度的item)
    uint16  *num_in_line_ptr = menu_ctrl_ptr->num_in_line_ptr;          // 一行里有多少个item(第0行是最底下的，序号越大，显示的位置越靠上)
    uint16  min_width = 0;
    uint16  max_width = 0;
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    uint16  next_item_index = 0;
    uint16  max_item_num = 0;
    uint16  line_count = 0;
    uint16  line_index = 0;
    uint16  item_index = 0;
    uint16  i = 0;
    uint16  item_width = 0;
    GUI_RECT_T      rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    // check whether needs more item
    if (item_count > menu_ctrl_ptr->options_theme.max_item_num)
    {
        is_more_item = TRUE;
        item_count = menu_ctrl_ptr->options_theme.max_item_num;
    }

    if (0 == item_count)
    {
        return;
    }

    menu_ctrl_ptr->dis_num_in_options_menu = item_count;

    // reset old item info
    ResetLayoutInfo(menu_ctrl_ptr);

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    max_width = (uint16)(lcd_width - (menu_ctrl_ptr->options_theme.border_width * 2));

    if (MMK_IsWindowLandscape(menu_ctrl_ptr->win_handle))
    {
        min_width = menu_ctrl_ptr->options_theme.h_min_item_width;
    }
    else
    {
        min_width = menu_ctrl_ptr->options_theme.v_min_item_width;
    }

    // store how many items in a line while this length
    GetAllItemNumInLine(
            menu_ctrl_ptr, item_len_array_ptr, item_count,
            is_more_item, min_width, max_width);

    // store how many items in each line
    line_count = 0;
    next_item_index = 0;
    max_item_num = item_count;

    do
    {
        next_item_index = CalculateLineNum(
                item_len_array_ptr,
                item_count,
                next_item_index,
                max_item_num,
                &num_in_line_ptr[line_count]);

        max_item_num = num_in_line_ptr[line_count];

        line_count++;
        SCI_ASSERT(line_count <= item_count);   /*assert verified*/
    } while (next_item_index != item_count);

    // 当最前面两行相差超过2个时，拉均匀点
    if (line_count > 1)
    {
        while (num_in_line_ptr[line_count - 2] - num_in_line_ptr[line_count - 1] >= 2)
        {
            next_item_index = CalculateLineNum(
                    item_len_array_ptr,
                    item_count,
                    (uint16)(item_count - num_in_line_ptr[line_count - 1] - 1),   // 把第二行的第一个拉上去
                    (uint16)(num_in_line_ptr[line_count - 2] - 1),                // 第二行的个数减1
                    PNULL);

            // 拉上去一个后，不能完全容纳
            if (next_item_index < item_count)
            {
                break;
            }

            num_in_line_ptr[line_count - 1]++;
            num_in_line_ptr[line_count - 2]--;
            SCI_ASSERT(num_in_line_ptr[line_count - 1] <= num_in_line_ptr[line_count - 2]); /*assert verified*/
        }
    }

    // calculate position of all items
    menu_ctrl_ptr->line_count = line_count;
    item_index = item_count;
    for (line_index = 0; line_index < line_count; line_index++)
    {
        item_width = (uint16)(max_width / num_in_line_ptr[line_index]);

        rect.bottom = (int16)(lcd_height - line_index * menu_ctrl_ptr->options_theme.item_height);
#if !defined(GUIF_SOFTKEY_PDASTYLE)
        rect.bottom -= (int16)MMITHEME_GetWinSoftkeyHeight(menu_ctrl_ptr->win_handle);
#endif
        rect.top = (int16)(rect.bottom  - menu_ctrl_ptr->options_theme.item_height + 1);

        rect.right = (int16)(lcd_width - 1 - menu_ctrl_ptr->options_theme.border_width);
        rect.left = (int16)(rect.right - item_width + 1);

        for (i = 0; i < num_in_line_ptr[line_index]; i++)
        {
            SCI_ASSERT(0 != item_index);    /*assert verified*/
            item_index--;

            if (0 != i)
            {
                rect.left = (int16)(rect.left - item_width);
                rect.right = (int16)(rect.right - item_width);
                SCI_ASSERT(rect.left >= menu_ctrl_ptr->options_theme.border_width);    /*assert verified*/

                if (num_in_line_ptr[line_index] - 1 == i)
                {
                    rect.left = menu_ctrl_ptr->options_theme.border_width;
                }
            }

            menu_ctrl_ptr->options_item_info_ptr[item_index].rect = rect;
            menu_ctrl_ptr->options_item_info_ptr[item_index].line_index = line_index;
            menu_ctrl_ptr->options_item_info_ptr[item_index].column_index = i;

            if (is_more_item && 0 == line_index && 0 == i)
            {
                menu_ctrl_ptr->options_item_info_ptr[item_index].is_more_item = TRUE;
            }
            else
            {
                menu_ctrl_ptr->options_item_info_ptr[item_index].is_more_item = FALSE;
            }
        }
    }

    // set menu rect
    base_ctrl_ptr->rect.left = 0;
    base_ctrl_ptr->rect.right = (int16)(base_ctrl_ptr->rect.left + lcd_width - 1);
    base_ctrl_ptr->rect.top = (int16)(menu_ctrl_ptr->options_item_info_ptr[0].rect.top - menu_ctrl_ptr->options_theme.border_width - MMITHEME_OPTIONSMENU_SHADOW_HEIGHT);
    base_ctrl_ptr->rect.bottom = menu_ctrl_ptr->options_item_info_ptr[item_count - 1].rect.bottom;
}

/*****************************************************************************/
//  Description : 计算一行里能容纳下多少个此宽度的item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//  存储数据的顺序与显示的顺序正好相反
/*****************************************************************************/
LOCAL void GetAllItemNumInLine(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                               uint16           *item_len_array_ptr,
                               uint16           item_count,
                               BOOLEAN          is_more_item,
                               uint16           min_width,
                               uint16           max_width
                               )
{
    uint16 i = 0;
    MMI_STRING_T    text_str = {0};

    // when has 'more' item, get it num
    if (is_more_item)
    {
        MMITHEME_GetLabelTextByLang(menu_ctrl_ptr->options_theme.more_item_text_id, &text_str);
        item_len_array_ptr[i] = GetItemNumInLine(menu_ctrl_ptr, &text_str, min_width, max_width);
        i++;
    }

    // get normal item's num
    for (; i < item_count; i++)
    {
        CTRLMENU_GetItem(menu_ctrl_ptr, (uint16)(item_count - i - 1), &text_str, PNULL, PNULL);
        item_len_array_ptr[i] = GetItemNumInLine(menu_ctrl_ptr, &text_str, min_width, max_width);
    }
}

/*****************************************************************************/
//  Description : 计算一行里能容纳下多少个此宽度的item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetItemNumInLine(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              MMI_STRING_T      *text_ptr,
                              uint16            min_width,
                              uint16            max_width
                              )
{
    uint16  item_width = 0;

    item_width = GUI_CalculateStringPiexlNum(
            text_ptr->wstr_ptr,
            text_ptr->wstr_len,
            menu_ctrl_ptr->options_theme.item_font,
            0);

    if (item_width > max_width)
    {
        item_width = max_width;
    }
    else if (item_width < min_width)
    {
        item_width = min_width;
    }

    return (uint16)(max_width / item_width);
}

/*****************************************************************************/
//  Description : 计算一行里容纳的item数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 返回下一个开始的序号
// 返回item_count表明start_index之后的所有item都能容纳下
/*****************************************************************************/
LOCAL uint16 CalculateLineNum(
                              uint16    *item_len_array_ptr,
                              uint16    item_count,
                              uint16    start_item_index,
                              uint16    max_num_in_line,
                              uint16    *num_in_line_ptr
                              )
{
    uint16  item_index = 0;
    uint16  cur_item_num = 0;
    uint16  min_item_num_in_line = max_num_in_line;     // 以最大长度的item来算，能最大容纳的item数量

    SCI_ASSERT(start_item_index < item_count);  /*assert verified*/

    SCI_ASSERT(0 != max_num_in_line);   /*assert verified*/
    SCI_ASSERT(max_num_in_line <= item_count);  /*assert verified*/

    for (item_index = start_item_index; item_index < item_count; item_index++)
    {
        if (item_len_array_ptr[item_index] < min_item_num_in_line)
        {
            min_item_num_in_line = item_len_array_ptr[item_index];
        }

        if (cur_item_num >= min_item_num_in_line)
        {
            break;
        }

        cur_item_num++;
    }

    SCI_ASSERT(0 != cur_item_num);  /*assert verified*/
    SCI_ASSERT(item_index - start_item_index == cur_item_num);  /*assert verified*/
    if (PNULL != num_in_line_ptr)
    {
        *num_in_line_ptr = cur_item_num;
    }

    return item_index;
}

/*****************************************************************************/
//  Description : optionsmenu 显示
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_DrawOptions(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                 )
{
    BOOLEAN             is_common_lcd = FALSE;
    uint16              i = 0;
    GUI_RECT_T          shadow_rect = {0};
    GUI_RECT_T          bg_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, &is_common_lcd);
    if (!is_common_lcd)
    {
        // 清MENU内容
        UILAYER_Clear(lcd_dev_info_ptr);
    }

    // draw background
    // 只要在阴影下部画背景
    bg_rect = base_ctrl_ptr->rect;
    bg_rect.top = (int16)(bg_rect.top + MMITHEME_OPTIONSMENU_SHADOW_HEIGHT);

    DisplayBackground(menu_ctrl_ptr, &bg_rect);

    // draw item
    for (i = 0; i < menu_ctrl_ptr->dis_num_in_options_menu; i++)
    {
        DisplayItem(menu_ctrl_ptr, i, base_ctrl_ptr->rect, lcd_dev_info_ptr);
    }

    // draw border
    DisplayBorder(menu_ctrl_ptr, lcd_dev_info_ptr);

    //display shadow
    IMG_EnableTransparentColor(TRUE);

    shadow_rect = base_ctrl_ptr->rect;
    shadow_rect.bottom = (int16)(shadow_rect.top + MMITHEME_OPTIONSMENU_SHADOW_HEIGHT - 1);
#if defined GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
    GUIRES_DisplayImg(
        PNULL, &shadow_rect, PNULL,
        menu_ctrl_ptr->win_handle,
        menu_ctrl_ptr->options_theme.shadow_img,
        (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr);
#endif
    IMG_EnableTransparentColor(FALSE);

    // anim
    if (menu_ctrl_ptr->is_first_disp && !is_common_lcd)
    {
        menu_ctrl_ptr->is_first_disp = FALSE;
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        CTRLMENU_MoveDown(menu_ctrl_ptr, FALSE);
#endif
    }
}

/*****************************************************************************/
//  Description : 显示背景
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DisplayBackground(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                             GUI_RECT_T         *bg_rect_ptr
                             )
{
    BOOLEAN             is_common_lcd = FALSE;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, &is_common_lcd);

    // 如果是menu自己的层，那就层系统做，可以用透明色
    if (is_common_lcd)
    {
        LCD_FillRect(
            lcd_dev_info_ptr, *bg_rect_ptr,
            menu_ctrl_ptr->options_theme.bg_color);
    }
    else
    {
        LCD_FillArgbRect(
            (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
            *bg_rect_ptr,
            menu_ctrl_ptr->options_theme.bg_color,
            menu_ctrl_ptr->options_theme.bg_alpha);
    }
}

/*****************************************************************************/
//  Description : 显示item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DisplayItem(
                       CTRLMENU_OBJ_T       *menu_ctrl_ptr,     // [in]
                       uint16               item_index,         // [in]
                       GUI_RECT_T           clip_rect,          // [in]
                       GUI_LCD_DEV_INFO     *lcd_dev_info_ptr   // [in]
                       )
{
    BOOLEAN         is_grayed = FALSE;
    BOOLEAN         is_active_item = FALSE;
    GUISTR_STYLE_T  str_style = {0};
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR;
    GUI_COLOR_T     text_color = 0;
    MMI_STRING_T    text_str = {0};
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      image_rect = {0};
    GUI_RECT_T      text_rect = {0};
    MMI_IMAGE_ID_T  image_id = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (item_index >= menu_ctrl_ptr->dis_num_in_options_menu
        || item_index >= menu_ctrl_ptr->options_theme.max_item_num
        || GUI_IsRectEmpty(menu_ctrl_ptr->options_item_info_ptr[item_index].rect))
    {
        return;
    }

    // get item information
    if (menu_ctrl_ptr->options_item_info_ptr[item_index].is_more_item)
    {
        is_grayed = FALSE;
        image_id = menu_ctrl_ptr->options_theme.more_item_image_id;
        MMITHEME_GetLabelTextByLang(menu_ctrl_ptr->options_theme.more_item_text_id, &text_str);
    }
    else
    {
        CTRLMENU_GetItem(menu_ctrl_ptr, item_index, &text_str, &image_id, &is_grayed);
    }

    // check whether need to draw active
    if (item_index == menu_ctrl_ptr->cur_item_index &&
        ((!is_grayed && menu_ctrl_ptr->is_item_pressed) || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())))
    {
        is_active_item = TRUE;
    }

    // active bar
    item_rect = menu_ctrl_ptr->options_item_info_ptr[item_index].rect;
    if (is_active_item)
    {
        IMG_EnableTransparentColor(TRUE);

        //display selected bar
        GUIRES_DisplayImg(
            PNULL, &item_rect, PNULL,
            menu_ctrl_ptr->win_handle,
            menu_ctrl_ptr->common_theme.selected_img,
            (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr);

        IMG_EnableTransparentColor(FALSE);
    }
    else
    {
        DisplayBackground(menu_ctrl_ptr, &item_rect);
    }

    // 画ICON
    if (MMITHEME_CheckImageID(image_id))
    {
        image_rect = item_rect;
        image_rect.bottom = (int16)(image_rect.top + menu_ctrl_ptr->options_theme.image_height - 1);
        image_rect = GUI_GetCenterRectExt(
                            image_rect,
                            menu_ctrl_ptr->options_theme.image_height,
                            menu_ctrl_ptr->options_theme.image_height);

        GUIRES_DisplayImg(
            PNULL, &image_rect, PNULL,
            menu_ctrl_ptr->win_handle, image_id,
            (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr);
    }

    // 画文字
    if (0 != text_str.wstr_len)
    {
        text_rect = item_rect;

        // 如果有图，显示在图下
        if (!GUI_IsRectEmpty(image_rect))
        {
            text_rect.top = (int16)(image_rect.bottom + 1);
        }

        if (is_active_item)
        {
            str_style.font = menu_ctrl_ptr->options_theme.cur_item_font;
        }
        else
        {
            str_style.font = menu_ctrl_ptr->options_theme.item_font;
        }

        if (is_grayed)
        {
            text_color = MMITHEME_GetGrayColor();
        }
        else
        {
            text_color = CTRLMENU_GetFontColor(menu_ctrl_ptr);
        }

        str_style.font_color = text_color;
        str_style.align = ALIGN_HVMIDDLE;

        // display text
        GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
            (const GUI_RECT_T*)&text_rect,
            (const GUI_RECT_T*)&clip_rect,
            (const MMI_STRING_T*)&text_str,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);
    }

    // draw separator
    if (item_rect.top != base_ctrl_ptr->rect.top + menu_ctrl_ptr->options_theme.border_width + MMITHEME_OPTIONSMENU_SHADOW_HEIGHT)
    {
        LCD_DrawHLine(
                lcd_dev_info_ptr, item_rect.left, item_rect.top, item_rect.right,
                menu_ctrl_ptr->common_theme.split_line_color);
    }

    if (item_rect.left != base_ctrl_ptr->rect.left + menu_ctrl_ptr->options_theme.border_width)
    {
        LCD_DrawVLine(
                lcd_dev_info_ptr, item_rect.left, item_rect.top, item_rect.bottom,
                menu_ctrl_ptr->common_theme.split_line_color);
    }
}

/*****************************************************************************/
//  Description : 显示边框
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DisplayBorder(
                         CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                         GUI_LCD_DEV_INFO   *lcd_dev_info_ptr
                         )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    // 在阴影下部画边线
    GUI_RECT_T      rect = base_ctrl_ptr->rect;

    rect.top    = (int16)(rect.top + MMITHEME_OPTIONSMENU_SHADOW_HEIGHT);
    rect.left   = base_ctrl_ptr->rect.left;
    rect.right  = (int16)(rect.left + menu_ctrl_ptr->options_theme.border_width - 1);
    rect.bottom = base_ctrl_ptr->rect.bottom;

    LCD_FillArgbRect(
        (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
        rect,
        menu_ctrl_ptr->options_theme.border_color,
        menu_ctrl_ptr->options_theme.bg_alpha);

    // right
    rect.right  = base_ctrl_ptr->rect.right;
    rect.left   = (int16)(rect.right - menu_ctrl_ptr->options_theme.border_width + 1);
    rect.bottom = base_ctrl_ptr->rect.bottom;

    LCD_FillArgbRect(
        (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
        rect,
        menu_ctrl_ptr->options_theme.border_color,
        menu_ctrl_ptr->options_theme.bg_alpha);

    // top
    rect.left   = base_ctrl_ptr->rect.left;
    rect.right  = base_ctrl_ptr->rect.right;
    rect.bottom = (int16)(rect.top + menu_ctrl_ptr->options_theme.border_width - 1);

    LCD_FillArgbRect(
        (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
        rect,
        menu_ctrl_ptr->options_theme.border_color,
        menu_ctrl_ptr->options_theme.bg_alpha);
}

/*****************************************************************************/
//  Description : handle OK key
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    when in PDA project:
//    |--------------------------------------|
//    |key             |    Options          |
//    |--------------------------------------|
//    |OK(menu key)    |    in client: OK    |
//    |                |    in more: Popup   |
//    |                |    not : close      |
//    |--------------------------------------|

//    |Web             |    in client: OK    |
//    |                |    in more: Popup   |
//    |                |    not : close      |
//    |--------------------------------------|
//    |back            |    close            |
//    |--------------------------------------|
//    |tp              |    in client: OK    |
//    |                |    in more: Popup   |
//    |                |    not : close      |
//    |--------------------------------------|
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleOk(
                                        CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                        MMI_MESSAGE_ID_E    msg_id
                                        )
{
    BOOLEAN is_handled = FALSE;

    switch (msg_id)
    {
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_UP:
#endif
#ifdef MMI_PDA_SUPPORT
        if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
            && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
        {
            CTRLMENU_DrawOptions(menu_ctrl_ptr);
            is_handled = TRUE;
            break;
        }
#endif

        if (menu_ctrl_ptr->cur_item_index < menu_ctrl_ptr->dis_num_in_options_menu)
        {
            // 点在MORE上，显示MORE菜单
            if (menu_ctrl_ptr->options_item_info_ptr[menu_ctrl_ptr->cur_item_index].is_more_item)
            {
                CTRLMENU_SwitchOptions(menu_ctrl_ptr, FALSE);
                is_handled = TRUE;
            }
        }
        break;

    default:
        is_handled = TRUE;
        break;
    }

    return is_handled;
}

/*****************************************************************************/
//  Description : 处理上下键消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_OptionsHandleUpDown(
                                                 CTRLMENU_OBJ_T *menu_ctrl_ptr,
                                                 BOOLEAN        is_up
                                                 )
{
    uint16          i = 0;
    int16           cur_left_pos = 0;
    uint16          next_line_index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLMENU_OPTIONS_ITEM_T *cur_item_info = PNULL;
    CTRLMENU_OPTIONS_ITEM_T *item_info_ptr = PNULL;

    if (menu_ctrl_ptr->cur_item_index >= menu_ctrl_ptr->dis_num_in_options_menu)
    {
        return (result);
    }

#ifdef MMI_PDA_SUPPORT
    if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
        && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
    {
        CTRLMENU_DrawOptions(menu_ctrl_ptr);
        return (result);
    }
#endif

    cur_item_info = &menu_ctrl_ptr->options_item_info_ptr[menu_ctrl_ptr->cur_item_index];

    // 行索引是从下往上的，所以，往上，索引加，往下，索引减
    if (is_up)
    {
        if (cur_item_info->line_index + 1 >= menu_ctrl_ptr->line_count)
        {
            next_line_index = 0;
        }
        else
        {
            next_line_index = (uint16)(cur_item_info->line_index + 1);
        }
    }
    else
    {
        if (0 == cur_item_info->line_index)
        {
            next_line_index = (uint16)(menu_ctrl_ptr->line_count - 1);
        }
        else
        {
            next_line_index = (uint16)(cur_item_info->line_index - 1);
        }
    }

    cur_left_pos = cur_item_info->rect.left;

    for (i = 0; i < menu_ctrl_ptr->dis_num_in_options_menu; i++)
    {
        item_info_ptr = &menu_ctrl_ptr->options_item_info_ptr[i];

        if (item_info_ptr->line_index == next_line_index)
        {
            // 当前的左坐标落下区域内，找到所要的索引
            if (cur_left_pos >= item_info_ptr->rect.left && cur_left_pos <= item_info_ptr->rect.right)
            {
                RefreshActiveItem(menu_ctrl_ptr, i);
                break;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : 更新焦点项
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void RefreshActiveItem(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                             uint16             cur_item_index
                             )
{
    uint16              pre_item_index = menu_ctrl_ptr->cur_item_index;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

    menu_ctrl_ptr->cur_item_index = cur_item_index;

    // 清除前个active项
    DisplayBackground(menu_ctrl_ptr, &menu_ctrl_ptr->options_item_info_ptr[pre_item_index].rect);
    DisplayItem(menu_ctrl_ptr, pre_item_index, base_ctrl_ptr->rect, lcd_dev_info_ptr);

    // 显示active项
    DisplayBackground(menu_ctrl_ptr, &menu_ctrl_ptr->options_item_info_ptr[cur_item_index].rect);
    DisplayItem(menu_ctrl_ptr, cur_item_index, base_ctrl_ptr->rect, lcd_dev_info_ptr);
}

/*****************************************************************************/
//  Description : handle key left
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleLeft(
                                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                          )
{
    uint16  next_active_item_index = 0;

    if (menu_ctrl_ptr->cur_item_index >= menu_ctrl_ptr->dis_num_in_options_menu)
    {
        return FALSE;
    }

#ifdef MMI_PDA_SUPPORT
    if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
        && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
    {
        CTRLMENU_DrawOptions(menu_ctrl_ptr);
        return FALSE;
    }
#endif

    if (0 == menu_ctrl_ptr->cur_item_index)
    {
        next_active_item_index = (uint16)(menu_ctrl_ptr->dis_num_in_options_menu - 1);
    }
    else
    {
        next_active_item_index = (uint16)(menu_ctrl_ptr->cur_item_index - 1);
    }

    RefreshActiveItem(menu_ctrl_ptr, next_active_item_index);

    return FALSE;
}

/*****************************************************************************/
//  Description : handle key right
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleRight(
                                           CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                           )
{
    uint16  next_active_item_index = 0;

    if (menu_ctrl_ptr->cur_item_index >= menu_ctrl_ptr->dis_num_in_options_menu)
    {
        return FALSE;
    }

#ifdef MMI_PDA_SUPPORT
    if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
        && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
    {
        CTRLMENU_DrawOptions(menu_ctrl_ptr);
        return FALSE;
    }
#endif

    if (menu_ctrl_ptr->cur_item_index + 1 >= menu_ctrl_ptr->dis_num_in_options_menu)
    {
        next_active_item_index = 0;
    }
    else
    {
        next_active_item_index = (uint16)(menu_ctrl_ptr->cur_item_index + 1);
    }

    RefreshActiveItem(menu_ctrl_ptr, next_active_item_index);

    return FALSE;
}

/*****************************************************************************/
//  Description : handle number key
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleNum(
                                         uint16         num_index,
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         )
{
    BOOLEAN result = FALSE;

    if (num_index < menu_ctrl_ptr->dis_num_in_options_menu)
    {
        if (num_index != menu_ctrl_ptr->cur_item_index)
        {
            RefreshActiveItem(menu_ctrl_ptr, num_index);
        }

        result = TRUE;
    }

    return result;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 处理tp down消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_OptionsHandleTpDown(
                                         GUI_POINT_T    *tp_point_ptr,
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         )
{
    uint16              i = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    menu_ctrl_ptr->is_item_pressed = FALSE;

    menu_ctrl_ptr->slide_info.start_point.x = tp_point_ptr->x;
    menu_ctrl_ptr->slide_info.start_point.y = tp_point_ptr->y;

    for (i = 0; i < menu_ctrl_ptr->dis_num_in_options_menu; i++)
    {
        if (GUI_PointIsInRect(*tp_point_ptr, menu_ctrl_ptr->options_item_info_ptr[i].rect))
        {
            menu_ctrl_ptr->is_item_pressed = TRUE;
            menu_ctrl_ptr->cur_item_index = i;

            lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

            DisplayItem(menu_ctrl_ptr, i, base_ctrl_ptr->rect, lcd_dev_info_ptr);

            break;
        }
    }
}

/*****************************************************************************/
//  Description : 处理tp up消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_OptionsHandleTpUp(
                                          GUI_POINT_T       *tp_point_ptr,
                                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                          )
{
    BOOLEAN             is_need_handle = FALSE;
    uint16              i = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    if (menu_ctrl_ptr->is_item_pressed
        && GUI_PointIsInRect(menu_ctrl_ptr->slide_info.start_point, base_ctrl_ptr->rect))
    {
        for (i = 0; i < menu_ctrl_ptr->dis_num_in_options_menu; i++)
        {
            if (GUI_PointIsInRect(*tp_point_ptr, menu_ctrl_ptr->options_item_info_ptr[i].rect))
            {
                break;
            }
        }

        // 点击在同一个区域内，guimenu需要处理
        if (menu_ctrl_ptr->dis_num_in_options_menu != i && menu_ctrl_ptr->cur_item_index == i)
        {
            is_need_handle = TRUE;
        }

        menu_ctrl_ptr->is_item_pressed = FALSE;

        lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);
        DisplayItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, base_ctrl_ptr->rect, lcd_dev_info_ptr);
    }

    return is_need_handle;
}

/*****************************************************************************/
//  Description : 处理tp move消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_OptionsHandleTpMove(
                                         GUI_POINT_T    *tp_point_ptr,
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         )
{
    uint16              i = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    if (menu_ctrl_ptr->is_item_pressed
        && GUI_PointIsInRect(menu_ctrl_ptr->slide_info.start_point, base_ctrl_ptr->rect))
    {
        for (i = 0; i < menu_ctrl_ptr->dis_num_in_options_menu; i++)
        {
            if (GUI_PointIsInRect(*tp_point_ptr, menu_ctrl_ptr->options_item_info_ptr[i].rect))
            {
                break;
            }
        }

        // 点击到其他区域，或者其他item了
        if (menu_ctrl_ptr->dis_num_in_options_menu == i || menu_ctrl_ptr->cur_item_index != i)
        {
            menu_ctrl_ptr->is_item_pressed = FALSE;

            lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

            DisplayItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, base_ctrl_ptr->rect, lcd_dev_info_ptr);
        }
    }
}
#endif

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_OptionsModifyRect(
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       )
{
    CTRLMENU_OptionsLayout(menu_ctrl_ptr);

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    CTRLMENU_ResetAnimLayer(menu_ctrl_ptr);
#endif
}
#endif

/*****************************************************************************/
//  Description : optionsmenu与moremenu之间的切换
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SwitchOptions(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                      BOOLEAN           is_optionsmenu
                                      )
{
    BOOLEAN result = FALSE;

    if (SetItemVisibleByNum(menu_ctrl_ptr, (uint16)(menu_ctrl_ptr->options_theme.max_item_num - 1), is_optionsmenu))
    {
        // 做向下的动画
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        CTRLMENU_MoveDown(menu_ctrl_ptr, TRUE);
#endif

        // 先清一下原先的数据
        CTRLMENU_Destroy(menu_ctrl_ptr);

        if(is_optionsmenu)
        {
            menu_ctrl_ptr->cur_style = GUIMENU_STYLE_OPTIONS;
        }
        else
        {
            menu_ctrl_ptr->cur_style = GUIMENU_STYLE_POPUP;
        }

        // 重新初始化数据
        CTRLMENU_Init(menu_ctrl_ptr);

        // 重算区域
        CTRLMENU_CalculateRect(0,menu_ctrl_ptr);

        // 重置MENU数据
        menu_ctrl_ptr->cur_item_index   = 0;
        menu_ctrl_ptr->first_item_index = 0;
        menu_ctrl_ptr->is_first_disp = TRUE;

        //destroy progress
        if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
        {
            MMK_DestroyControl(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr));
            menu_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
        }

        // 清掉原先的菜单
        CTRLMENU_UpdatePrevWindow(menu_ctrl_ptr);

        //display next menu
        CTRLMENU_Draw(menu_ctrl_ptr);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 临时隐藏前几项
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetItemVisibleByNum(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr, // [in]
                                  uint16            num,            // [in]
                                  BOOLEAN           visible         // [in]
                                  )
{
    BOOLEAN                 result = FALSE;
    uint16                  i = 0;
    uint16                  index = 0;
    CTRLMENU_GROUP_INFO_T   *group_info_ptr = PNULL;
    GUIMENU_GROUP_T         *group_ptr      = PNULL;
    CTRLMENU_ITEM_INFO_T    *item_info_ptr  = PNULL;
    CTRLMENU_NODE_T         *parent_node_ptr = PNULL;
    CTRLMENU_NODE_T         *cur_node_ptr   = PNULL;

    if (visible && PNULL != menu_ctrl_ptr->temp_invisible_index_ptr)    // 之前有隐藏项，现在将其显示
    {
        if (menu_ctrl_ptr->is_static)
        {
            group_ptr = menu_ctrl_ptr->cur_group_ptr;
            group_info_ptr = menu_ctrl_ptr->cur_group_info_ptr;
            item_info_ptr = GET_ITEM_INFO(group_info_ptr);

            for (i = 0; i < menu_ctrl_ptr->temp_invisible_num; i++)
            {
                // 之前没隐藏的索引号
                index = menu_ctrl_ptr->temp_invisible_index_ptr[i];
                SCI_ASSERT(index < group_ptr->item_count);  /*assert verified*/

                // 如果用户没有恢复过，将其显示
                if (!item_info_ptr[index].is_visible)
                {
                    item_info_ptr[index].is_visible = TRUE;
                    group_info_ptr->visible_child_item_num++;
                    menu_ctrl_ptr->item_total_num++;
                }
            }
        }
        else
        {
            // get parent node pointer
            parent_node_ptr = menu_ctrl_ptr->cur_parent_node_ptr;

            for (i = 0; i < menu_ctrl_ptr->temp_invisible_num; i++)
            {
                index = menu_ctrl_ptr->temp_invisible_index_ptr[i];
                SCI_ASSERT(index < parent_node_ptr->child_node_num);    /*assert verified*/

                // find specified node
                cur_node_ptr = CTRLMENU_FindNodeByIndex(index, parent_node_ptr);

                if (PNULL != cur_node_ptr && !cur_node_ptr->is_visible)
                {
                    cur_node_ptr->is_visible = TRUE;
                    parent_node_ptr->visible_child_node_num++;
                    menu_ctrl_ptr->item_total_num++;
                }
            }
        }

        SCI_FREE(menu_ctrl_ptr->temp_invisible_index_ptr);
        menu_ctrl_ptr->temp_invisible_num = 0;
        result = TRUE;
    }
    else if (!visible && PNULL == menu_ctrl_ptr->temp_invisible_index_ptr)  // 之前没有隐藏过，现在隐藏
    {
        SCI_ASSERT(num > 0);    /*assert verified*/
        SCI_ASSERT(num < menu_ctrl_ptr->item_total_num);    /*assert verified*/

        menu_ctrl_ptr->temp_invisible_num = 0;
        menu_ctrl_ptr->temp_invisible_index_ptr = SCI_ALLOC_APP(num * sizeof(uint16));
        SCI_MEMSET(menu_ctrl_ptr->temp_invisible_index_ptr, 0, num * sizeof(uint16));

        if (menu_ctrl_ptr->is_static)
        {
            group_ptr = menu_ctrl_ptr->cur_group_ptr;
            group_info_ptr = menu_ctrl_ptr->cur_group_info_ptr;
            item_info_ptr = GET_ITEM_INFO(group_info_ptr);

            for (i = 0; i < group_ptr->item_count; i++)
            {
                if (item_info_ptr[i].is_visible)        // while being invisible
                {
                    item_info_ptr[i].is_visible = FALSE;
                    group_info_ptr->visible_child_item_num--;
                    menu_ctrl_ptr->item_total_num--;

                    // 存储被隐藏的索引号
                    menu_ctrl_ptr->temp_invisible_index_ptr[menu_ctrl_ptr->temp_invisible_num] = i;
                    menu_ctrl_ptr->temp_invisible_num++;

                    // 已达到个数
                    if (menu_ctrl_ptr->temp_invisible_num == num)
                    {
                        result = TRUE;
                        break;
                    }
                }
            }
        }
        else
        {
            // get parent node pointer
            parent_node_ptr = menu_ctrl_ptr->cur_parent_node_ptr;

            for (i = 0; i < parent_node_ptr->child_node_num; i++)
            {
                // find specified node
                cur_node_ptr = CTRLMENU_FindNodeByIndex(i, parent_node_ptr);

                if (PNULL != cur_node_ptr && cur_node_ptr->is_visible)
                {
                    cur_node_ptr->is_visible = FALSE;
                    parent_node_ptr->visible_child_node_num--;
                    menu_ctrl_ptr->item_total_num--;

                    // 存储被隐藏的索引号
                    menu_ctrl_ptr->temp_invisible_index_ptr[menu_ctrl_ptr->temp_invisible_num] = i;
                    menu_ctrl_ptr->temp_invisible_num++;

                    // 已达到个数
                    if (menu_ctrl_ptr->temp_invisible_num == num)
                    {
                        result = TRUE;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"GUIMENU_SetItemVisibleByNum error: visible: %d , ptr: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMENU_6878_112_2_18_3_20_31_248,(uint8*)"dd", visible, menu_ctrl_ptr->temp_invisible_index_ptr);
    }

    return result;
}

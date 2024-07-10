/*==============================================================================
File Name: mmiim_sp_basic.c
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/


#include "mmiim_sp_basic.h"

#include <ctype.h>

#include "guires.h"
#include "guistring.h"
#include "mmi_theme.h"
#include "mmiim_tp_cstar_position.h"




MMIIM_SP_TEXT_BAR_THEME_T g_text_bar_theme =
{
    IMG_IMSP_CAND_BAR,
    {0, IMG_IMSP_ARROW_UP_DIS, IMG_IMSP_ARROW_UP_OFF, IMG_IMSP_ARROW_UP_ON},
    {0, IMG_IMSP_ARROW_DOWN_DIS, IMG_IMSP_ARROW_DOWN_OFF, IMG_IMSP_ARROW_DOWN_ON},

    {MMIIM_SP_TEXT_BAR_IDC_LEFT_X, MMIIM_SP_TEXT_BAR_IDC_Y, MMIIM_SP_TEXT_BAR_IDC_LEFT_X + MMIIM_SP_TEXT_BAR_IDC_W, MMIIM_SP_TEXT_BAR_IDC_Y + MMIIM_SP_TEXT_BAR_IDC_H},
    {MMIIM_SP_TEXT_BAR_IDC_RIGHT_X, MMIIM_SP_TEXT_BAR_IDC_Y, MMIIM_SP_TEXT_BAR_IDC_RIGHT_X + MMIIM_SP_TEXT_BAR_IDC_W, MMIIM_SP_TEXT_BAR_IDC_Y + MMIIM_SP_TEXT_BAR_IDC_H},

    MMIIM_SP_TEXT_BAR_FONT_TEXT,
    MMIIM_SP_TEXT_BAR_FONT_LABEL,
    MMIIM_SP_TEXT_BAR_COLOR_TEXT,
    MMIIM_SP_TEXT_BAR_COLOR_LABEL,
    MMIIM_SP_TEXT_BAR_COLOR_HOT,
};




/*
text bar 相关函数
*/

/*==============================================================================
Function Name: MMIIM_SP_TextBarInit
Description: MMIIM_SP_TextBarInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarInit(MMIIM_SP_TEXT_BAR_T * bar_ptr)
{
    if (PNULL == bar_ptr)
    {
        return 1;
    }

    SCI_MEMSET(bar_ptr, 0, sizeof(MMIIM_SP_TEXT_BAR_T));

    return 0;
}


/*==============================================================================
Function Name: MMIIM_SP_TextBarDraw
Description: MMIIM_SP_TextBarDraw
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarDraw(MMIIM_SP_TEXT_BAR_T const * bar_ptr)
{
    BOOLEAN is_bg_disp = FALSE;
    BOOLEAN is_idc_left_disp = FALSE;
    BOOLEAN is_idc_right_disp = FALSE;
    GUI_POINT_T idc_left_point = {0};
    GUI_POINT_T idc_right_point = {0};
    uint8 seg_idx = 0;

    GUI_RECT_T bar_rect = {0};
    GUI_RECT_T bar_text_rect = {0};
    GUI_RECT_T text_rect = {0};
    GUI_RECT_T hot_rect = {0};

    wchar label[2] = {0};
    MMI_STRING_T label_info = {0};
    GUISTR_STYLE_T label_style = {0};
    MMI_STRING_T text_info = {0};
    GUISTR_STYLE_T text_style = {0};

    GUISTR_STATE_T state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN;
    uint16 mmiim_sp_text_bar_cand_width = 0;


    if (PNULL == bar_ptr)
    {
        return 1;
    }

    if (0 == bar_ptr->seg_cnt)
    {
        return 0;
    }

    bar_rect = bar_ptr->rect;
    MMITHEME_DisplayCommonBg(&bar_rect,bar_ptr->win_id,bar_ptr->lcd_info_ptr);

    mmiim_sp_text_bar_cand_width = MMITHEME_IsMainScreenLandscape() ? 
        (MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H - MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H) : (MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V - MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V);

    // 1 draw bg
    bar_text_rect = bar_rect;
    bar_text_rect.right = mmiim_sp_text_bar_cand_width - bar_rect.left;
    is_bg_disp = GUIRES_DisplayImg(PNULL, &bar_text_rect, PNULL,
                                   bar_ptr->win_id, g_text_bar_theme.img_bg, bar_ptr->lcd_info_ptr);

    if (!is_bg_disp)
    {
        return 1;
    }

    // 2 draw idc
    if (MMIIM_SP_IDC_NONE < bar_ptr->idc_left && bar_ptr->idc_left < MMIIM_SP_IDC_MAX)
    {
        idc_left_point.x = bar_ptr->rect.left + g_text_bar_theme.rect_idc_left.left;
        idc_left_point.y = bar_ptr->rect.top + g_text_bar_theme.rect_idc_left.top;

        is_idc_left_disp = GUIRES_DisplayImg(&idc_left_point, PNULL, PNULL,
                                             bar_ptr->win_id, g_text_bar_theme.img_idc_left[bar_ptr->idc_left], bar_ptr->lcd_info_ptr);

        if (!is_idc_left_disp)
        {
            return 1;
        }
    }

    if (MMIIM_SP_IDC_NONE < bar_ptr->idc_right && bar_ptr->idc_right < MMIIM_SP_IDC_MAX)
    {
        idc_right_point.x = bar_ptr->rect.left + g_text_bar_theme.rect_idc_right.left;
        idc_right_point.y = bar_ptr->rect.top + g_text_bar_theme.rect_idc_right.top;

        is_idc_right_disp = GUIRES_DisplayImg(&idc_right_point, PNULL, PNULL,
                                              bar_ptr->win_id, g_text_bar_theme.img_idc_right[bar_ptr->idc_right], bar_ptr->lcd_info_ptr);

        if (!is_idc_right_disp)
        {
            return 1;
        }
    }

    // 3 draw label and text
    text_rect = bar_ptr->rect;
    text_rect.top += MMIIM_SP_TEXT_BAR_CAND_Y;
    text_rect.bottom -= MMIIM_SP_TEXT_BAR_CAND_Y;

    //修正左边, 总是显示字符的右边际
    {
        int16 text_len = 0;
        int16 rect_width = text_rect.right - text_rect.left + 1;

        for (seg_idx = 0; seg_idx < bar_ptr->seg_cnt; seg_idx++)
        {
            text_len += bar_ptr->seg[seg_idx].width;
        }

        text_len += (bar_ptr->width_label + bar_ptr->width_space) * (bar_ptr->seg_cnt - 1);

        if (text_len > rect_width)
        {
            text_rect.left -= (text_len - rect_width);
        }
    }

    label_info.wstr_ptr = label;
    label_info.wstr_len = 1;

    label_style.font = g_text_bar_theme.font_label;
    label_style.font_color = g_text_bar_theme.color_label;

    text_style.font = g_text_bar_theme.font_text;
    text_style.font_color = g_text_bar_theme.color_text;
    text_style.align = ALIGN_LVMIDDLE;

    for (seg_idx = 0; seg_idx < bar_ptr->seg_cnt; seg_idx++)
    {
        if (0 == bar_ptr->seg[seg_idx].len) continue;

        text_rect.left += bar_ptr->width_space;

        // label
        if (bar_ptr->has_label)
        {
            label[0] = '1' + seg_idx;

            GUISTR_DrawTextToLCDInRect(bar_ptr->lcd_info_ptr, &text_rect, &bar_rect,
                                       &label_info, &label_style, state, GUISTR_TEXT_DIR_AUTO);
        }

        text_rect.left += bar_ptr->width_label;

        // hot bg
        if (bar_ptr->has_hot && bar_ptr->hot_index == seg_idx)
        {
            hot_rect = text_rect;
            hot_rect.right = hot_rect.left + bar_ptr->seg[seg_idx].width;

            GUI_FillRect(bar_ptr->lcd_info_ptr, hot_rect, g_text_bar_theme.color_hot);
        }

        // text
        text_info.wstr_ptr = bar_ptr->seg[seg_idx].ptr;
        text_info.wstr_len = bar_ptr->seg[seg_idx].len;

        GUISTR_DrawTextToLCDInRect(bar_ptr->lcd_info_ptr, &text_rect, &bar_rect,
                                   &text_info, &text_style, state, GUISTR_TEXT_DIR_AUTO);

        text_rect.left += bar_ptr->seg[seg_idx].width;
    }

    return 0;
}


/*==============================================================================
Function Name: MMIIM_SP_TextBarClear
Description: 清空数据部分
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarClear(MMIIM_SP_TEXT_BAR_T * bar_ptr)
{
    if (PNULL == bar_ptr)
    {
        return 1;
    }

    bar_ptr->seg_cnt = 0;
    bar_ptr->hot_index = 0;

    return 0;
}


/*==============================================================================
Function Name: MMIIM_SP_TextBarGenTextSeg
Description: 将字符串分割, 按照片段组合
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
    text_len_limit - 字符的最大存储区间
    split - 每个片段的结尾标识
    seg_cnt - 段的个数
==============================================================================*/
int32 MMIIM_SP_TextBarGenTextSeg(
    MMIIM_SP_TEXT_BAR_T * bar_ptr,
    wchar const *text_ptr,
    size_t text_cnt_limit,
    wchar split,
    uint8 seg_cnt
)
{
    size_t text_idx = 0;
    size_t last_seg_head = 0;


    if (PNULL == bar_ptr || PNULL == text_ptr || 0 == seg_cnt)
    {
        return 1;
    }

    bar_ptr->hot_index = 0;
    bar_ptr->text_len = 0;
    bar_ptr->seg_cnt = 0;
    bar_ptr->idc_left = MMIIM_SP_IDC_NONE;
    bar_ptr->idc_right = MMIIM_SP_IDC_NONE;
    last_seg_head = 0;

    for (text_idx = 0; text_idx < text_cnt_limit; text_idx++)
    {
        if (bar_ptr->seg_cnt == seg_cnt)
        {
            break;
        }

        if (split == text_ptr[text_idx])
        {
            bar_ptr->text[text_idx] = ' ';

            bar_ptr->seg[bar_ptr->seg_cnt].ptr = bar_ptr->text + last_seg_head;
            bar_ptr->seg[bar_ptr->seg_cnt].len = text_idx - last_seg_head;
            bar_ptr->seg[bar_ptr->seg_cnt].width = MMIIM_SP_StringWidth(
                                                       bar_ptr->seg[bar_ptr->seg_cnt].ptr,
                                                       bar_ptr->seg[bar_ptr->seg_cnt].len
                                                       );

            bar_ptr->seg_cnt++;

            last_seg_head = text_idx + 1;
        }
        else
        {
            bar_ptr->text[text_idx] = text_ptr[text_idx];
        }
    }

    bar_ptr->text_len = text_idx;

    if (text_ptr[text_idx - 1] != split)
    {
        bar_ptr->seg[bar_ptr->seg_cnt].ptr = bar_ptr->text + last_seg_head;
        bar_ptr->seg[bar_ptr->seg_cnt].len = text_idx - last_seg_head;
        bar_ptr->seg[bar_ptr->seg_cnt].width = MMIIM_SP_StringWidth(
                                                   bar_ptr->seg[bar_ptr->seg_cnt].ptr,
                                                   bar_ptr->seg[bar_ptr->seg_cnt].len
                                                   );

        bar_ptr->seg_cnt++;
    }

    //SCI_ASSERT(bar_ptr->seg_cnt == seg_cnt); /*assert verified*/
    return 0;
}





/*
*/
/*==============================================================================
Function Name: MMIIM_SP_TextBarGenTextStr
Description: 将字符串分割, 每个字一个片段
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarGenTextStr(
    MMIIM_SP_TEXT_BAR_T * bar_ptr,
    wchar const *text_ptr,
    size_t text_len
)
{
    size_t text_idx = 0;

    if (PNULL == bar_ptr || PNULL == text_ptr)
    {
        return 1;
    }

    bar_ptr->seg_cnt = 0;

    for (text_idx = 0; text_idx < text_len; text_idx++)
    {
        bar_ptr->text[text_idx] = text_ptr[text_idx];

        bar_ptr->seg[bar_ptr->seg_cnt].ptr = bar_ptr->text + text_idx;
        bar_ptr->seg[bar_ptr->seg_cnt].len = 1;
        bar_ptr->seg[bar_ptr->seg_cnt].width = MMIIM_SP_StringWidth(bar_ptr->seg[bar_ptr->seg_cnt].ptr, 1);

        bar_ptr->seg_cnt++;
    }

    bar_ptr->text_len = text_len;

    return 0;
}


/*==============================================================================
Function Name: MMIIM_SP_TextBarUpdate
Description: MMIIM_SP_TextBarUpdate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarUpdate(
    MMIIM_SP_TEXT_BAR_T *bar_ptr,
    MMIIM_SP_BAR_DATA_T *data_ptr
)
{
    size_t pre_cnt = 0;
    size_t cnt = 0;
    uint8 width_min = GUI_GetFontWidth(MMIIM_SP_TEXT_BAR_FONT_TEXT, UNICODE_HANZI);
    uint8 seg_idx = 0;

    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == bar_ptr||PNULL == data_ptr || 0 == data_ptr->cnt)
    {
        return;
    }

    pre_cnt = data_ptr->cur_page * data_ptr->per_page;
    cnt = data_ptr->cnt - pre_cnt;

    if (cnt > data_ptr->per_page)
    {
        cnt = data_ptr->per_page;
    }

    MMIIM_SP_TextBarGenTextStr(bar_ptr,
                               data_ptr->list_ptr + data_ptr->cur_page * data_ptr->per_page,
                               cnt);

    for (seg_idx = 0; seg_idx < bar_ptr->seg_cnt; seg_idx++)
    {
        //修正最小宽度为汉字宽度，避免符号和字母挤到一起
        if (bar_ptr->seg[seg_idx].width < width_min)
        {
            bar_ptr->seg[seg_idx].width = width_min;
        }
    }

    bar_ptr->hot_index = 0;

    // idc flag
    bar_ptr->idc_left =
        (data_ptr->cur_page > 0) ?
        MMIIM_SP_IDC_ACTIVE : MMIIM_SP_IDC_INACTIVE;
    bar_ptr->idc_right =
        ((data_ptr->cur_page + 1) * data_ptr->per_page < data_ptr->cnt) ?
        MMIIM_SP_IDC_ACTIVE : MMIIM_SP_IDC_INACTIVE;

    return;
}



/*==============================================================================
Function Name: MMIIM_SP_TextBarGetTp
Description: MMIIM_SP_TextBarGetTp
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TextBarGetTp(MMIIM_SP_TEXT_BAR_T *bar_ptr, GUI_POINT_T *point_ptr)
{
    int32 tp_seg_idx = -1;
    size_t seg_idx = 0;
    GUI_POINT_T point = {0};
    GUI_RECT_T seg_rect = {0};

    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != point_ptr); /*assert verified*/

    if (PNULL == bar_ptr|| bar_ptr->seg_cnt == 0 ||PNULL == point_ptr)
    {
        return -1;
    }

    if (!GUI_PointIsInRect(*point_ptr, bar_ptr->rect))
    {
        return -1;
    }

    point.x = point_ptr->x - bar_ptr->rect.left;
    point.y = point_ptr->y - bar_ptr->rect.top;

    if (GUI_PointIsInRect(point, g_text_bar_theme.rect_idc_left))
    {
        if (MMIIM_SP_IDC_ACTIVE == bar_ptr->idc_left
                || MMIIM_SP_IDC_PRESSED == bar_ptr->idc_left)
        {
            tp_seg_idx = MMIIM_SP_TEXT_BAR_TP_IDC_LEFT;
        }
    }
    else if (GUI_PointIsInRect(point, g_text_bar_theme.rect_idc_right))
    {
        if (MMIIM_SP_IDC_ACTIVE == bar_ptr->idc_right
                || MMIIM_SP_IDC_PRESSED == bar_ptr->idc_right)
        {
            tp_seg_idx = MMIIM_SP_TEXT_BAR_TP_IDC_RIGHT;
        }
    }
    else
    {
        //seg index, 扩大点击区域
        seg_rect = bar_ptr->rect;

        for (seg_idx = 0; seg_idx < bar_ptr->seg_cnt; seg_idx++)
        {
            seg_rect.right = seg_rect.left;
            seg_rect.right += bar_ptr->width_space;
            seg_rect.right += bar_ptr->width_label;
            seg_rect.right += bar_ptr->seg[seg_idx].width;

            if (GUI_PointIsInRect(*point_ptr, seg_rect))
            {
                tp_seg_idx = seg_idx;
                break;
            }

            seg_rect.left = seg_rect.right + 1;
        }
    }


    return tp_seg_idx;
}


/*==============================================================================
Function Name: MMIIM_SP_TextBarLower
Description: MMIIM_SP_TextBarLower
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarLower(MMIIM_SP_TEXT_BAR_T *bar_ptr)
{
    uint8 i = 0;

    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/
    if (PNULL == bar_ptr)
    {
        return;
    }
    
    for (i = 0; i < bar_ptr->text_len; i++)
    {
        bar_ptr->text[i] = tolower(bar_ptr->text[i]);
    }

    return;
}


/*==============================================================================
Function Name: MMIIM_SP_TextBarUpper
Description: MMIIM_SP_TextBarUpper
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarUpper(MMIIM_SP_TEXT_BAR_T *bar_ptr)
{
    uint8 i = 0;

    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/
    if (PNULL == bar_ptr)
    {
        return;
    }
    
    for (i = 0; i < bar_ptr->text_len; i++)
    {
        bar_ptr->text[i] = toupper(bar_ptr->text[i]);
    }

    return;
}


/*==============================================================================
Function Name: MMIIM_SP_TextBarLeadUpper
Description: MMIIM_SP_TextBarLeadUpper
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarLeadUpper(MMIIM_SP_TEXT_BAR_T *bar_ptr)
{
    uint8 i = 0;
    BOOLEAN is_leader = TRUE;

    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/
    if (PNULL == bar_ptr)
    {
        return;
    }
    
    for (i = 0; i < bar_ptr->text_len; i++)
    {
        if (' ' == bar_ptr->text[i])
        {
            is_leader = TRUE;
            continue;
        }

        if (is_leader)
        {
            is_leader = FALSE;
            bar_ptr->text[i] = toupper(bar_ptr->text[i]);
        }
        else
        {
            bar_ptr->text[i] = tolower(bar_ptr->text[i]);
        }
    }

    return;
}


/*==============================================================================
Function Name: MMIIM_SP_TextBarClearIDC
Description: MMIIM_SP_TextBarClearIDC
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_TextBarClearIDC(MMIIM_SP_TEXT_BAR_T *bar_ptr)
{
    BOOLEAN is_changed = FALSE;
    
    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/
    if (PNULL == bar_ptr)
    {
        return	FALSE;
    }
    
    if (MMIIM_SP_IDC_PRESSED == bar_ptr->idc_left)
    {
        bar_ptr->idc_left = MMIIM_SP_IDC_ACTIVE;
        is_changed = TRUE;
    }

    if (MMIIM_SP_IDC_PRESSED == bar_ptr->idc_right)
    {
        bar_ptr->idc_right = MMIIM_SP_IDC_ACTIVE;
        is_changed = TRUE;
    }

    return is_changed;
}


/*==============================================================================
Description: MMIIM_SP_StringWidth
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
uint32 MMIIM_SP_StringWidth(wchar const *str_ptr, uint32 str_len)
{
    return GUI_GetStringWidth(MMIIM_SP_TEXT_BAR_FONT_TEXT, str_ptr, str_len);
}

/*
定时器相关函数
*/
//未用到，暂时不写...




















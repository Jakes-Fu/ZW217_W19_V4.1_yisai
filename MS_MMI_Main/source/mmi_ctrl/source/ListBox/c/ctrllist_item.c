/****************************************************************************
** File Name:      ctrllist_item.c                                          *
** Author:         xiaoqing.lu                                              *
** Date:           24/07/2012                                               *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the listbox control.       *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 24/07/2012     xiaoqing.lu      Create
****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_app.h"
#include "mmi_port.h"
#include "ctrllist_export.h"
#include "ctrllistbox.h"
#include "mmk_timer.h"
#include "mmi_theme.h"
#include "guiblock.h"
#include "guilcd.h"
#include "mmk_msg.h"
#include "mmi_textfun.h"
#include "guires.h"
#include "guistring.h"
#include "ui_layer.h"
#include "mmk_tp.h"
#include "mmi_string.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define SLOPE_60_DEGREE 1.732
#define SLOPE_30_DEGREE 0.577

//mark图标⊙距离list控件左侧的距离
#define RADIO_LEFT_MARGEN   DP2PX_VALUE(19)//DP2PX_VALUE(36)
#define RADIO_RIGHT_MARGEN   DP2PX_VALUE(19)//DP2PX_VALUE(36)
/*--------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                               */
/*--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL GUI_LCD_DEV_INFO s_item_layer = {0, UILAYER_NULL_HANDLE};          /*!< Item显示的层信息 */
LOCAL GUI_LCD_DEV_INFO s_item_mist_layer = {0, UILAYER_NULL_HANDLE};          /*!< Item显示的层信息 */

/**-------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 判断是move item 还是move整个控件，即横向还是纵向
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMoveListItem(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
);

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 判断是move item 还是move整个控件，即横向还是纵向
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMoveListItem(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
)
{
#define LIST_TPMOVE_POINT_MIN_NUM  (15)     // 超过该像素值算move

    GUI_RECT_T item_rect = {0};
    float   tg_value = 0.001f;
    BOOLEAN is_slide_item = FALSE;

    if (IS_TP_MOVE_X_EXT(point_ptr->x, list_ctrl_ptr->slide_start_point.x, LIST_TPMOVE_POINT_MIN_NUM) || IS_TP_MOVE_Y(point_ptr->y, list_ctrl_ptr->slide_start_point.y))
    {
        if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_LEFT_SLIDE_ITEM))
        {
            if (0 > point_ptr->x - list_ctrl_ptr->slide_start_point.x)
            {
                tg_value = ((float)abs(point_ptr->y - list_ctrl_ptr->slide_start_point.y) / ((float)abs(point_ptr->x - list_ctrl_ptr->slide_start_point.x)));

                if (tg_value <= (float)SLOPE_30_DEGREE)
                {
                    is_slide_item = TRUE;
                }
            }
            else
            {
                is_slide_item = FALSE;
            }
        }
        else
        {
            if (0 != point_ptr->x - list_ctrl_ptr->slide_start_point.x)
            {
                tg_value = ((float)abs(point_ptr->y - list_ctrl_ptr->slide_start_point.y) / ((float)abs(point_ptr->x - list_ctrl_ptr->slide_start_point.x)));

                if (tg_value <= (float)SLOPE_30_DEGREE)
                {
                    is_slide_item = TRUE;
                }
            }
            else
            {
                is_slide_item = FALSE;
            }
        }
    }

    LIST_SetItemMoveState(list_ctrl_ptr, is_slide_item);

    LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    LIST_DrawItemSlideData(list_ctrl_ptr, point_ptr, item_rect);

    return is_slide_item;
}

/*****************************************************************************/
//  Description : 处理滑动效果
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DisplayItemSlideColor(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr,
    GUI_RECT_T       item_rect
)
{
    BOOLEAN result = FALSE;

    GUI_RECT_T left_fill_rect   = {0};
    GUI_RECT_T right_fill_rect   = {0};
    GUI_LCD_DEV_INFO mist_item_lcd    = {0};

    mist_item_lcd = LIST_GetItemMistLayer();

    left_fill_rect = item_rect;
    left_fill_rect.right = tp_point_ptr->x;
    right_fill_rect = item_rect;
    right_fill_rect.left = tp_point_ptr->x;

    UILAYER_Clear(&mist_item_lcd);
    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_LEFT_SLIDE_ITEM))
    {
        LCD_FillRect(&mist_item_lcd,item_rect,0x000000);// BG : BLACK
    }
    else
    {
        // 向右滑动
        if (tp_point_ptr->x >= list_ctrl_ptr->slide_start_point.x)
        {
            LCD_FillPolygon(&mist_item_lcd, left_fill_rect, 0x49c200, 0x00ccff);
            LCD_FillPolygon(&mist_item_lcd, right_fill_rect, 0x00ccff, 0x00ccff);
        }
        else//  if (tp_point_ptr->x < list_ctrl_ptr->slide_start_point.x) // 向左滑动
        {
            LCD_FillPolygon(&mist_item_lcd, left_fill_rect, 0x00ccff, 0x00ccff);
            LCD_FillPolygon(&mist_item_lcd, right_fill_rect, 0x00ccff, 0x49c200);
        }
    }
    UILAYER_WeakLayerAlpha(&mist_item_lcd, CTRLLIST_WEAKLAYERALPHA);

    UILAYER_BltLayerToLayer(&list_ctrl_ptr->item_layer, &mist_item_lcd, &item_rect, TRUE);

    return result;
}

/*****************************************************************************/
//  Description : 画图
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DrawItemSlideData(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr,
    GUI_RECT_T       item_rect
)
{
    BOOLEAN result = FALSE;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)list_ctrl_ptr;

    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SLIDE_ITEM))
    {
        GUI_RECT_T                  left_rect   = {0};
        GUI_RECT_T                  right_rect  = {0};
        GUI_RECT_T                  icon_rect  = {0};
        MMITHEME_LIST_CUSTDATA_T    user_data   = {0};
        //MMI_STRING_T                str_data    = {0};
        int16                       offset_x    = 0;

        offset_x = point_ptr->x - list_ctrl_ptr->slide_start_point.x;

        // 获取当前item区域
        // CalculateListItemRect( &item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, list_ctrl_ptr->offset_y);

        GUI_CheckRectValid(item_rect);

        // UILAYER_ClearRect(&list_ctrl_ptr->item_layer, item_rect);

        // 取用户数据，如果回调指针为空，取默认值
        if (PNULL != list_ctrl_ptr->get_data_func)
        {
            list_ctrl_ptr->get_data_func(&user_data);
        }
        else
        {
            MMITHEME_GetListItemDefData(&user_data);
        }

        left_rect = LIST_GetDispRect(item_rect, user_data, TRUE, list_ctrl_ptr->win_id);
        right_rect = LIST_GetDispRect(item_rect, user_data, FALSE, list_ctrl_ptr->win_id);
        if (!LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_LEFT_SLIDE_ITEM))
        {
            if (abs(offset_x) > abs((base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left + 1) / 3))
            {
                if (point_ptr->x > list_ctrl_ptr->slide_start_point.x) // 右侧
                {
                    if (GUILIST_TYPE_TEXT_ID == user_data.data_type)
                    {
                        LIST_DisplayText(list_ctrl_ptr, user_data.right_text_id, &item_rect, &item_rect, &user_data.str_style);

                        GUIRES_DisplayImg(PNULL, &right_rect, PNULL, list_ctrl_ptr->win_id, user_data.right_icon_id, &list_ctrl_ptr->item_layer);
                    }
                    else
                    {
                        icon_rect = GUI_GetCenterRect(item_rect, right_rect.right - right_rect.left + 1, right_rect.bottom - right_rect.top + 1);
                        GUIRES_DisplayImg(PNULL, &icon_rect, PNULL, list_ctrl_ptr->win_id, user_data.right_icon_id, &list_ctrl_ptr->item_layer);
                    }
                }
                else // 左侧
                {
                    if (GUILIST_TYPE_TEXT_ID == user_data.data_type)
                    {
                        LIST_DisplayText(list_ctrl_ptr, user_data.left_text_id, &item_rect, &item_rect, &user_data.str_style);
                        GUIRES_DisplayImg(PNULL, &left_rect, PNULL, list_ctrl_ptr->win_id, user_data.left_icon_id, &list_ctrl_ptr->item_layer);
                    }
                    else
                    {
                        icon_rect = GUI_GetCenterRect(item_rect, left_rect.right - left_rect.left + 1, left_rect.bottom - left_rect.top + 1);
                        GUIRES_DisplayImg(PNULL, &left_rect, PNULL, list_ctrl_ptr->win_id, user_data.left_icon_id, &list_ctrl_ptr->item_layer);
                    }
                }
            }
            else
            {
                GUIRES_DisplayImg(PNULL, &left_rect, PNULL, list_ctrl_ptr->win_id, user_data.left_icon_id, &list_ctrl_ptr->item_layer);
                GUIRES_DisplayImg(PNULL, &right_rect, PNULL, list_ctrl_ptr->win_id, user_data.right_icon_id, &list_ctrl_ptr->item_layer);
            }
        }
        // 画分割线
        if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SPLIT_LINE))
        {
            LIST_DrawListItemSplitLine(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, item_rect, &list_ctrl_ptr->item_layer);
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 显示字符串
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DisplayText(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    MMI_TEXT_ID_T  text_id,
    GUI_RECT_T     *disp_rect_ptr,
    GUI_RECT_T     *clip_rect_ptr,
    GUISTR_STYLE_T *str_style_ptr
)
{
    MMI_STRING_T                str_data    = {0};

    if (PNULL != str_style_ptr && PNULL != list_ctrl_ptr && PNULL != disp_rect_ptr && PNULL != clip_rect_ptr)
    {
        MMITHEME_GetResText(text_id, list_ctrl_ptr->win_id, &str_data);

        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&list_ctrl_ptr->item_layer,
            (const GUI_RECT_T *)disp_rect_ptr,      //the fixed display area
            (const GUI_RECT_T *)clip_rect_ptr,      //用户要剪切的实际区域
            (const MMI_STRING_T *)&str_data,
            str_style_ptr,
            GUISTR_STATE_SINGLE_LINE,
            GUISTR_TEXT_DIR_AUTO
        );
    }

    return;
}

/*****************************************************************************/
//  Description : 判断是move item 还是move整个控件，即横向还是纵向
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetTotalHeight(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
)
{
    return SetMoveListItem(list_ctrl_ptr, point_ptr);
}

/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO LIST_GetItemDispLayer(void)
{
    return s_item_layer;
}

/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO LIST_GetItemMistLayer(void)
{
    return s_item_mist_layer;
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_CreateItemDispLayer(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_RECT_T item_rect
)
{
    UILAYER_CREATE_T create_info = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    UILAYER_RELEASELAYER(&s_item_layer);  /*lint !e506 !e774*/

    create_info.lcd_id = MAIN_LCD_ID;
    create_info.owner_handle = list_ctrl_ptr->win_id;
    create_info.offset_x = item_rect.left;
    create_info.offset_y = item_rect.top;
    create_info.width = item_rect.right - item_rect.left;
    create_info.height = item_rect.bottom - item_rect.top;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_DOUBLE_COPY;

    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_LEFT_SLIDE_ITEM))
    {
        create_info.width += (MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT + MMICOM_LIST_SLIDE_ICON_MARGIN)*MMICOM_LIST_SLIDE_ICON_NUM + MMICOM_LIST_SLIDE_ICON_MARGIN;
    }

    UILAYER_CreateLayer(&create_info, &s_item_layer);

    UILAYER_SetLayerPosition(&s_item_layer, item_rect.left, item_rect.top);

    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_LEFT_SLIDE_ITEM))
    {
        LIST_DrawHighLightItem_ForSlide(list_ctrl_ptr,&s_item_layer);
    }
    else
    {
        UILAYER_BltLayerToLayer(&s_item_layer, &list_ctrl_ptr->item_layer, &item_rect, FALSE);
    }
    

    return;
}

/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_IsItemDispLayerActive(void)
{
    return UILAYER_IsLayerActive(&s_item_layer);
}

/*****************************************************************************/
//  Description : 获得layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_CreateItemMistLayer(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_RECT_T item_rect
)
{
    UILAYER_CREATE_T create_info = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    UILAYER_RELEASELAYER(&s_item_mist_layer);  /*lint !e506 !e774*/

    create_info.lcd_id = MAIN_LCD_ID;
    create_info.owner_handle = list_ctrl_ptr->win_id;
    create_info.offset_x = item_rect.left;
    create_info.offset_y = item_rect.top;
    create_info.width = item_rect.right - item_rect.left;
    create_info.height = item_rect.bottom - item_rect.top;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_DOUBLE_COPY;

    UILAYER_CreateLayer(&create_info, &s_item_mist_layer);

    UILAYER_WeakLayerAlpha(&s_item_mist_layer, 165);

    UILAYER_SetLayerPosition(&s_item_mist_layer, item_rect.left, item_rect.top);

    return;
}

/*****************************************************************************/
//  Description : 释放层
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ReleaseItemDispLayer(void)
{
    UILAYER_RELEASELAYER(&s_item_layer);  /*lint !e506 !e774*/
}

/*****************************************************************************/
//  Description : 释放层
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ReleaseItemMistLayer(void)
{
    UILAYER_RELEASELAYER(&s_item_mist_layer);  /*lint !e506 !e774*/
}
#endif

/*****************************************************************************/
//  Description : 获得左侧的区域
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LIST_GetDispRect(
    GUI_RECT_T item_rect,
    MMITHEME_LIST_CUSTDATA_T user_data,
    BOOLEAN is_left,
    MMI_HANDLE_T win_handle
)
{
#define MARGIN_X 5

    uint16 icon_height = 0;
    //uint16 text_height = 0;
    uint16 icon_width = 0;
    //uint16 text_width = 0;
    GUI_RECT_T rect = {0};
    //MMI_STRING_T str_data = {0};
    //GUISTR_INFO_T        str_info = {0};
    //GUISTR_STATE_T str_state = GUISTR_STATE_NONE;

    rect = item_rect;

    /*switch(user_data.data_type)
    {
    case GUILIST_TYPE_TEXT_ID:
        if (is_left) // 取左边的字符串
        {
            MMITHEME_GetResText( user_data.left_text_id, win_handle, &str_data );
            GUISTR_GetStringInfo(&user_data.str_style, &str_data, str_state, &str_info);
            rect.left += MARGIN_X;
            rect.right = rect.left + str_info.width;
        }
        else // 取右边的字符串
        {
            MMITHEME_GetResText( user_data.right_text_id, win_handle, &str_data );
            GUISTR_GetStringInfo(&user_data.str_style, &str_data, str_state, &str_info);
            rect.right -= MARGIN_X;
            rect.left = rect.right - str_info.width;
        }

        // 矫正区域
        if (GUI_IntersectRect(&rect, rect, item_rect))
        {
            rect = GUI_GetCenterRect(rect, str_info.width, str_info.height);
        }
        else
        {
            rect = item_rect;
        }
        break;

    case GUILIST_TYPE_ICON_ID:*/
    if (is_left) // 取左侧的icon
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, user_data.left_icon_id, win_handle);
        rect.left += MARGIN_X;
        rect.right = rect.left + icon_width;
    }
    else // 取右侧的icon
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, user_data.right_icon_id, win_handle);
        rect.right -= MARGIN_X;
        rect.left = rect.right - icon_width;
    }

    // 矫正区域
    if (GUI_IntersectRect(&rect, rect, item_rect))
    {
        rect = GUI_GetCenterRect(rect, icon_width, icon_height);
    }
    else
    {
        rect = item_rect;
    }

    /*break;

    default:
    break;
    }*/

    return rect;
}

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CTRLLIST_ITEM_INFO_T *LIST_GetItemInfoPtr(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16                index  //from 0 to total_item_num-1
)
{
    if (PNULL == list_ctrl_ptr || index >= list_ctrl_ptr->total_item_num)
    {
        SCI_TRACE_LOW("GetItemInfoPtr PNULL index = %d", index);

        return PNULL;
    }

    return list_ctrl_ptr->item_info_ptr + index;
}

/*****************************************************************************/
//  Description : unselected item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_UnSelectItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 index
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (PNULL == (item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index)))
    {
        return FALSE;
    }

    if (LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
    {
        if (list_ctrl_ptr->selected_item_num > 0)
        {
            list_ctrl_ptr->selected_item_num--;

            result = TRUE;
        }

        LIST_SetItemState(item_ptr, GUIITEM_STATE_SELECTED, FALSE);
    }

    return result;
}

/*****************************************************************************/
//  Description : selected item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SelectedItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 index
)
{
    BOOLEAN result = TRUE;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (PNULL == (item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index)))
    {
        return FALSE;
    }

    if (LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
    {
        return TRUE;
    }

    if (CTRLLIST_MARK_RADIO== list_ctrl_ptr->mark_type)
    {
        uint16 i = 0;
        CTRLLIST_ITEM_INFO_T *pre_item_ptr = PNULL;

        /* set all item unselected */
        for (i = 0; i < list_ctrl_ptr->total_item_num; i++)
        {
            pre_item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

            if (PNULL != pre_item_ptr)
            {
                LIST_SetItemState(pre_item_ptr, GUIITEM_STATE_SELECTED, FALSE);
            }
        }

        LIST_SetItemState(item_ptr, GUIITEM_STATE_SELECTED, TRUE);
    }
    else
    {
        if (list_ctrl_ptr->selected_item_num < list_ctrl_ptr->mark_cnt_max)
        {
            LIST_SetItemState(item_ptr, GUIITEM_STATE_SELECTED, TRUE);
            list_ctrl_ptr->selected_item_num++;
        }
        else
        {
            result = FALSE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : get the item text
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetListItemContentTextInfo(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_info_ptr,
    int16 content_index,
    MMI_STRING_T *out_str
)
{
    BOOLEAN result = TRUE;

    if (PNULL == item_info_ptr || PNULL == out_str || PNULL == item_info_ptr->item.data_ptr)
    {
        return FALSE;
    }

    if (content_index < 0 || content_index >= GUIITEM_CONTENT_MAX_NUM)
    {
        return FALSE;
    }

    if (GUIITEM_DATA_TEXT_ID == item_info_ptr->item.data_ptr->item_content[content_index].item_data_type)
    {
        MMI_STRING_T temp_str = {0};

        MMITHEME_GetResText(item_info_ptr->item.data_ptr->item_content[content_index].item_data.text_id, list_ctrl_ptr->win_id, &temp_str);

        out_str->wstr_ptr = temp_str.wstr_ptr;
        out_str->wstr_len = temp_str.wstr_len;
    }
    else if (GUIITEM_DATA_TEXT_BUFFER == item_info_ptr->item.data_ptr->item_content[content_index].item_data_type)
    {
        out_str->wstr_ptr = item_info_ptr->item.data_ptr->item_content[content_index].item_data.text_buffer.wstr_ptr;
        out_str->wstr_len = item_info_ptr->item.data_ptr->item_content[content_index].item_data.text_buffer.wstr_len;
    }
    else
    {
        result = FALSE;
    }

    if (PNULL == out_str->wstr_ptr || 0 == out_str->wstr_len)
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get the item height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 LIST_GetItemHeight(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16              index  //from 0 to total_item_num-1
)
{
    CTRLLIST_ITEM_INFO_T   *item_ptr    = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    if (list_ctrl_ptr->is_full_dirty)
    {
        LIST_ResetAllItemDisp(list_ctrl_ptr);
    }

    item_ptr =  LIST_GetItemInfoPtr(list_ctrl_ptr, index);

    if (PNULL == item_ptr)
    {
        return 0;
    }

    return item_ptr->item_height;
}

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SetState(
    uint32 *dst_state_ptr,
    uint32  src_state,
    BOOLEAN is_true
)
{
    uint32 state;

    if (PNULL != dst_state_ptr)
    {
        state = *dst_state_ptr;

        if (is_true)
        {
            state |= src_state;
        }
        else
        {
            state &= ~src_state;
        }

        *dst_state_ptr = state;
    }
}

/*****************************************************************************/
//  Description : get state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetState(
    uint32 dst_state,
    uint32 src_state
)
{
    return (BOOLEAN)((dst_state & src_state) == src_state);
}

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SetItemState(
    CTRLLIST_ITEM_INFO_T *item_ptr,
    GUIITEM_STATE_T  src_state,
    BOOLEAN is_true
)
{
    if (PNULL != item_ptr)
    {
        LIST_SetState(&item_ptr->item.state, src_state, is_true);
    }
}

/*****************************************************************************/
//  Description : Get the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetItemState(
    const CTRLLIST_ITEM_INFO_T *item_ptr,
    GUIITEM_STATE_T src_state
)
{
    if (PNULL == item_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_5230_112_2_18_3_19_52_211, (uint8 *)"");
        return FALSE;
    }

    return LIST_GetState(item_ptr->item.state, src_state);
}

/*****************************************************************************/
//  Description : get the inner state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:获得list内部状态
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetInnerState(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    GUILIST_STATE_T src_state
)
{
    if (!LIST_TypeOf(list_ctrl_ptr))
    {
        return FALSE;
    }

    return LIST_GetState(list_ctrl_ptr->inner_state, src_state);
}

/*****************************************************************************/
//  Description : set the inner state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:设置list内部状态
/*****************************************************************************/
PUBLIC void LIST_SetInnerState(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUILIST_STATE_T src_state,
    BOOLEAN         is_true
)
{
    if (LIST_TypeOf(list_ctrl_ptr))
    {
        LIST_SetState(&list_ctrl_ptr->inner_state, src_state, is_true);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 计算单个item的显示宽度，高度
//  显示宽度指content的显示宽度，除掉num、select和mark宽度（此三者宽度定义相同）
/*****************************************************************************/
PUBLIC BOOLEAN LIST_ResetItemDisp(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    int32 index
)
{
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;
    THEMELIST_ITEM_STYLE_T *style_ptr = PNULL;
    GUI_RECT_T list_rect = {0};
    int16 contents_left = 0;
    int16 contents_right = 0;
    int16 disp_right = 0;
    int16 dw = 0;
    int32 idx = 0;
    BOOLEAN is_current_item_focus = FALSE;
    BOOLEAN item_need_change_height = FALSE;
    int16 base_width  = 0;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

    if (PNULL == item_ptr)
    {
        return FALSE;
    }

    style_ptr = item_ptr->item.style_ptr;

    if (PNULL == style_ptr)
    {
        return FALSE;
    }

    is_current_item_focus = (index == list_ctrl_ptr->cur_item_idx);

    if (is_current_item_focus)
    {
        item_ptr->item_height = style_ptr->height_focus;
    }
    else
    {
        item_ptr->item_height = style_ptr->height;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    disp_right = list_rect.right - list_rect.left;
    contents_right = disp_right;

    //以竖屏为基准的缩放
    //半屏
    //横竖屏
    if (style_ptr->base_width == 0)
    {
        base_width = list_ctrl_ptr->item_base_width;
    }
    else
    {
        base_width = style_ptr->base_width;
    }
    
    dw += (disp_right + 1 - base_width);
    
    //list is "marking"
    //item is "text"
    if (CTRLLIST_MARKING_NONE != list_ctrl_ptr->marking_type
        && !LIST_GetItemState(item_ptr, GUIITEM_STATE_DISABLE_MARK))
    {
        dw -= (list_ctrl_ptr->display_style.tag_width - (list_ctrl_ptr->prgbox_ptr ? MMITHEME_GetScrollBarWidthByState(list_ctrl_ptr->display_style.is_slide_used) + 1 : 0));

        if (list_ctrl_ptr->is_mark_icon_left)
        {
#ifdef ADULT_WATCH_SUPPORT
            item_ptr->mark_left = RADIO_LEFT_MARGEN;//contents_left
#else
            item_ptr->mark_left = contents_left;
#endif
            contents_left = RADIO_LEFT_MARGEN;
            contents_left += list_ctrl_ptr->display_style.tag_width;
        }
        else
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            contents_right -= list_ctrl_ptr->display_style.tag_width + RADIO_RIGHT_MARGEN;
#else
            contents_right -= list_ctrl_ptr->display_style.tag_width;
#endif
            item_ptr->mark_left = contents_right + 1;
        }

        item_ptr->mark_left += list_rect.left;
    }

    //list is "show_num"
    if (list_ctrl_ptr->show_num
        && LIST_GetItemState(item_ptr, GUIITEM_STATE_HAS_NUM))
    {
        dw -= list_ctrl_ptr->display_style.tag_width;
        item_ptr->num_left = contents_left;
        contents_left += list_ctrl_ptr->display_style.tag_width;

        item_ptr->num_left += list_rect.left;
    }

    item_need_change_height = ((0 <= style_ptr->sub_index) && (style_ptr->sub_index < GUIITEM_CONTENT_MAX_NUM)
                               && style_ptr->sub_index != style_ptr->main_index
                               && (GUIITEM_CONTENT_STATE_MULTILINE & style_ptr->content[style_ptr->sub_index].state));

    for (idx = 0; idx < GUIITEM_CONTENT_MAX_NUM; idx++)
    {
        BOOLEAN content_need_change_height = FALSE;

        if (is_current_item_focus)
        {
            item_ptr->content_rect[idx] = style_ptr->content[idx].rect_focus;
        }
        else
        {
            item_ptr->content_rect[idx] = style_ptr->content[idx].rect;
        }

        item_ptr->content_rect[idx].left += contents_left;
        item_ptr->content_rect[idx].right += contents_left;

        item_ptr->content_rect[idx].left += style_ptr->content[idx].scale_left * dw / 100;
        item_ptr->content_rect[idx].right += style_ptr->content[idx].scale_right * dw / 100;

        if (PNULL == item_ptr->item.data_ptr)
        {
            continue;
        }

        if (GUIITEM_DATA_TEXT_ID != item_ptr->item.data_ptr->item_content[idx].item_data_type
            && GUIITEM_DATA_TEXT_BUFFER != item_ptr->item.data_ptr->item_content[idx].item_data_type)
        {
            continue;
        }

        //以下针对可变行高的计算
        if (!item_need_change_height)
        {
            continue;
        }

        content_need_change_height = (style_ptr->sub_index == idx);

        if (content_need_change_height)
        {
            GUI_FONT_T  font = style_ptr->content[idx].font;
            GUI_RECT_T  sub_rect = style_ptr->content[style_ptr->sub_index].rect;
            uint16      height = 0;
            uint16      width = 0;
            uint16      font_height = 0;
            uint16      line_height = 0;
            uint16      total_line_num = 0;
            MMI_STRING_T text_info = {0};

            if (is_current_item_focus)
            {
                font = style_ptr->content[idx].font_focus;
                sub_rect = style_ptr->content[style_ptr->sub_index].rect_focus;
            }

            if (!GUI_IsInvalidRect(sub_rect))
            {
                height = sub_rect.bottom - sub_rect.top + 1;
                width = sub_rect.right - sub_rect.left + 1;

                font_height = GUI_GetFontHeight(font, UNICODE_HANZI);
                line_height = (uint16)(font_height + list_ctrl_ptr->line_space);

                if (LIST_GetListItemContentTextInfo(list_ctrl_ptr, item_ptr, style_ptr->sub_index, &text_info))
                {

                    total_line_num = GUI_CalculateStringLinesByPixelNum(width,
                                                                        text_info.wstr_ptr,
                                                                        text_info.wstr_len,
                                                                        font,
                                                                        list_ctrl_ptr->char_space,
                                                                        TRUE);

                    if (0 != total_line_num)
                    {
                        item_ptr->item_height += (total_line_num - 1) * line_height;
                    }
                }
            }
        } //content_need_change_height
    } //end for

    //如果此处是右对齐的，则做一个镜像变换
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        int16 backup_left = 0;

        //only left: left(+tag_width)->right(disp_right-)->left
        //mark
        item_ptr->mark_left = (disp_right + list_rect.left) - (item_ptr->mark_left + list_ctrl_ptr->display_style.tag_width) + list_rect.left;
        //num
        item_ptr->num_left = disp_right - (item_ptr->num_left + list_ctrl_ptr->display_style.tag_width);

        for (idx = 0; idx < GUIITEM_CONTENT_MAX_NUM; idx++)
        {
            //left & right: right(disp_right-)->left, left(disp_right-)->right
            //content[i]
            backup_left = item_ptr->content_rect[idx].left;
            item_ptr->content_rect[idx].left = disp_right - item_ptr->content_rect[idx].right;
            item_ptr->content_rect[idx].right = disp_right - backup_left;
        }
    }


    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 当控件的区域改变，重新设置item top
/*****************************************************************************/
PUBLIC void LIST_ResetItemTop(CTRLLIST_OBJ_T *list_ctrl_ptr)
{
    int32 i = 0;
    int32  offset_y = 0;
    int32 item_height = 0;
    int32 total_item_num = 0;
    int32 total_item_height = 0;
    GUI_RECT_T list_rect = {0};
    CTRLLIST_ITEM_INFO_T  *item_ptr = PNULL;

    if (NULL == list_ctrl_ptr)
    {
        return;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    if (list_ctrl_ptr->is_full_dirty)
    {
        total_item_num = list_ctrl_ptr->total_item_num;

        offset_y = list_rect.top;

        for (i = 0; i < total_item_num; i++)
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

            if (PNULL != item_ptr)
            {
                item_height = item_ptr->item_height;
                item_ptr->item_top = offset_y + total_item_height;;

                total_item_height += item_height;
            }
        }

        list_ctrl_ptr->is_full_dirty = FALSE;
    }

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 计算item的显示宽度，高度
//  显示宽度指content的显示宽度，除掉num、select和mark宽度（此三者宽度定义相同）
/*****************************************************************************/
PUBLIC void LIST_ResetAllItemDisp(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
)
{
    BOOLEAN is_reset = TRUE;
    int32 i = 0;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (!list_ctrl_ptr->is_full_dirty)
    {
        return;
    }

    for (i = 0; i < list_ctrl_ptr->total_item_num; i++)
    {
        if (!LIST_ResetItemDisp(list_ctrl_ptr, i))
        {
            is_reset = FALSE;
            break;
        }
    }

    if (is_reset)
    {
        list_ctrl_ptr->is_full_dirty = FALSE;
        list_ctrl_ptr->is_pos_dirty = TRUE;
    }

    return;
}

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyAllItemDataNotInPageExt(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // the list control pointer
    uint16         pre_top_index,
    uint16         pre_end_index
)
{
    BOOLEAN result = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    // if( LIST_GetInnerState( list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA ) )
    {
        uint16 i = 0;

        for (i = pre_top_index; i <= pre_end_index; i++)
        {
            if (i < list_ctrl_ptr->top_item_idx || i > list_ctrl_ptr->end_item_idx)
            {
                if (LIST_DestroyItemData(list_ctrl_ptr, LIST_GetItemInfoPtr(list_ctrl_ptr, i), TRUE))
                {
                    //终止当前正在解码的item
                    LIST_DestroyItemDecode(list_ctrl_ptr);
                }
            }
        }

        result = TRUE;
    }

    //终止当前正在解码的item
    LIST_DestroyItemDecode(list_ctrl_ptr);

    return result;
}

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyAllItemDataNotInPage(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // the list control pointer
    BOOLEAN        is_page
)
{
    BOOLEAN result = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (!is_page
        || LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA))
    {
        uint16 i = 0;

        for (i = 0; i < list_ctrl_ptr->total_item_num; i++)
        {
            if (i < list_ctrl_ptr->top_item_idx || i > list_ctrl_ptr->end_item_idx)
            {
                if (LIST_DestroyItemData(list_ctrl_ptr, LIST_GetItemInfoPtr(list_ctrl_ptr, i), TRUE))
                {
                    //终止当前正在解码的item
                    LIST_DestroyItemDecode(list_ctrl_ptr);
                }
            }
        }

        result = TRUE;
    }

    //终止当前正在解码的item
    LIST_DestroyItemDecode(list_ctrl_ptr);

    return result;
}

/*****************************************************************************/
//  Description : 终止当前正在解码的item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DestroyItemDecode(
    CTRLLIST_OBJ_T *list_ctrl_ptr   //in
)
{
    if (PNULL == list_ctrl_ptr || !LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_CONTENT))
    {
        return;
    }

    if ((0 <= list_ctrl_ptr->cur_object_index) && (list_ctrl_ptr->cur_object_index < list_ctrl_ptr->total_item_num))
    {
        CTRLLIST_ITEM_INFO_T *item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_object_index);

        //judge item is need item content
        if (PNULL != item_ptr
            && LIST_GetItemState(item_ptr, GUIITEM_STATE_NEED_ITEM_CONTENT)
            && (PNULL != item_ptr->item.data_ptr))
        {
            uint16  i = 0;

            for (i = 0; i < GUIITEM_CONTENT_MAX_NUM; i++)
            {
                if (0 == item_ptr->inner_handle[i])
                {
                    continue;
                }

                switch (item_ptr->item.data_ptr->item_content[i].item_data_type)
                {
                    case GUIITEM_DATA_ANIM_DATA:
                    case GUIITEM_DATA_ANIM_PATH:
                        //终止当前的解码
                        MMK_DestroyControl(item_ptr->inner_handle[i]);
                        item_ptr->inner_handle[i] = 0;
                        break;

                    default:
                        SCI_PASSERT(FALSE, ("ListDestroyItemDecode:item_data_type %d is error!", item_ptr->item.data_ptr->item_content[i].item_data_type)); /*assert verified*/
                        break;
                }
            }//end for
        }
    }

    //delete display ind msg
    MMK_DeleteMSGByHwndAndMsg(list_ctrl_ptr->win_id, MSG_CTL_ANIM_DISPLAY_IND);

    //set list is not content
    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_CONTENT, FALSE);

    return;
}

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyItemInlineCtrl(//is destroy anim ctrl
    CTRLLIST_ITEM_INFO_T  *item_ptr,
    uint16               item_content_index
)
{
    BOOLEAN result = FALSE;

    if (PNULL != item_ptr && 0 != item_ptr->inner_handle[item_content_index])
    {
        switch (item_ptr->item.data_ptr->item_content[item_content_index].item_data_type)
        {
            case GUIITEM_DATA_ANIM_DATA:
            case GUIITEM_DATA_ANIM_PATH:
                /*use handle*/
                MMK_DestroyControl(item_ptr->inner_handle[item_content_index]);
                item_ptr->inner_handle[item_content_index] = PNULL;
                result = TRUE;
                break;

            default:
                SCI_PASSERT(FALSE, ("LIST_DestroyItemInlineCtrl: item_data_type %d is error!", item_ptr->item.data_ptr->item_content[item_content_index].item_data_type)); /*assert verified*/
                break;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyItemData(
    CTRLLIST_OBJ_T      *list_ctrl_ptr,  // the list control pointer
    CTRLLIST_ITEM_INFO_T *item_ptr,
    BOOLEAN              is_page
)
{
    BOOLEAN     result = FALSE;
    uint32 i = 0;

    if (PNULL == list_ctrl_ptr || PNULL == item_ptr || PNULL == item_ptr->item.data_ptr)
    {
        return FALSE;
    }

    /* destroy inner object */
    if (LIST_GetItemState(item_ptr, GUIITEM_STATE_NEED_ITEM_CONTENT))
    {
        for (i = 0; i < GUIITEM_CONTENT_MAX_NUM; i++)
        {
            result = LIST_DestroyItemInlineCtrl(item_ptr, i);
        }
    }

    if (!is_page
        //上下换页, 当前项不销毁, 否则skb显示有问题
        || (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA) && item_ptr != LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx)))
    {
        for (i = 0; i < GUIITEM_CONTENT_MAX_NUM; i++)
        {
            //释放text buffer
            if (GUIITEM_DATA_TEXT_BUFFER == item_ptr->item.data_ptr->item_content[i].item_data_type)
            {
                SCI_FREE(item_ptr->item.data_ptr->item_content[i].item_data.text_buffer.wstr_ptr);
            }
        }

        SCI_FREE(item_ptr->item.data_ptr);
        item_ptr->item.data_ptr = PNULL;
    }

    return (result);
}

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyAllItemData(
    CTRLLIST_OBJ_T      *list_ctrl_ptr,  // the list control pointer
    BOOLEAN                  is_page
)
{
    BOOLEAN result = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (!is_page
        || LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA))
    {
        uint16 i = 0;

        for (; i < list_ctrl_ptr->total_item_num; i++)
        {
            LIST_DestroyItemData(list_ctrl_ptr, LIST_GetItemInfoPtr(list_ctrl_ptr, i), is_page);
        }

        result = TRUE;
    }

    //终止当前正在解码的item
    LIST_DestroyItemDecode(list_ctrl_ptr);

    return result;
}

/*****************************************************************************/
//  Description : get selected icon of list
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T LIST_GetSelectIconId(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    const CTRLLIST_ITEM_INFO_T *item_ptr
)
{
    MMI_IMAGE_ID_T    icon_id     = 0;
    BOOLEAN           is_selected = LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED);

    if (PNULL == list_ctrl_ptr || CTRLLIST_MARKING_NONE == list_ctrl_ptr->marking_type)
    {
        return 0;
    }

    if (CTRLLIST_MARK_RADIO == list_ctrl_ptr->mark_type)
    {
        icon_id = (is_selected)
                  ? list_ctrl_ptr->display_style.radio_selected_image_id
                  : list_ctrl_ptr->display_style.radio_unselected_image_id;
    }
    else
    {
        icon_id = (is_selected)
                  ? list_ctrl_ptr->display_style.check_selected_image_id
                  : list_ctrl_ptr->display_style.check_unselected_image_id;
    }

    return icon_id;
}

/*****************************************************************************/
//  Description : get number icon of list
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T LIST_GetNumberIconId(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16                index
)
{
    MMI_IMAGE_ID_T    icon_id     = 0;

    if (list_ctrl_ptr->show_num)
    {
        uint16 i = (index - list_ctrl_ptr->num_start_index);

        if (index >= list_ctrl_ptr->num_start_index && i < GUIITEM_NUM_ICON_MAX)
        {
            icon_id = list_ctrl_ptr->display_style.num_icon_id[i];
        }
    }

    return icon_id;
}


/*****************************************************************************/
//  Description : get the item page num
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetPageItemNum(CTRLLIST_OBJ_T *list_ctrl_ptr)
{
    GUI_RECT_T list_rect = {0};

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    return LIST_GetItemNumByHeight(list_ctrl_ptr,
                                   (uint16)(list_rect.bottom - list_rect.top + 1),
                                   list_ctrl_ptr->top_item_idx, TRUE);
}

/*****************************************************************************/
//  Description : get the item num by height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetItemNumByHeight(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16              total_height,
    uint16              index,  //from 0 to total_item_num-1
    BOOLEAN             is_next
)
{
    int32    item_num = 0;

    if (PNULL != list_ctrl_ptr && list_ctrl_ptr->total_item_num > 0)
    {
        int32   i                 = 0;
        uint32  total_item_height = 0;

        if (is_next)
        {
            for (i = index; i < index + list_ctrl_ptr->total_item_num; i++)
            {
                total_item_height = (uint32)(total_item_height + LIST_GetItemHeight(list_ctrl_ptr,
                                                                                    (uint16)(i % list_ctrl_ptr->total_item_num)));

                if (total_item_height > total_height)
                {
                    break;
                }
            }

            item_num = i - index;
        }
        else
        {
            for (i = index; i > index - list_ctrl_ptr->total_item_num; i--)
            {
                total_item_height = (uint32)(total_item_height + LIST_GetItemHeight(list_ctrl_ptr,
                                                                                    (uint16)((i + list_ctrl_ptr->total_item_num) % list_ctrl_ptr->total_item_num)));

                if (total_item_height > total_height)
                {
                    break;
                }
            }

            item_num = index - i;
        }

    }

    return (uint16)item_num;
}


/*****************************************************************************/
//  Description : Get current item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetCurItemIndexByPtr(
    CTRLLIST_OBJ_T    *list_ctrl_ptr
)
{
    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    return list_ctrl_ptr->cur_item_idx;
}

/*****************************************************************************/
//  Description : Get total item number
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetTotalItemNumByPtr(
    CTRLLIST_OBJ_T    *list_ctrl_ptr
)
{
    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    return list_ctrl_ptr->total_item_num;
}

PUBLIC BOOLEAN WATCHLIST_DrawItemBgColor(
        CTRLLIST_OBJ_T            *list_ctrl_ptr,
        const GUI_LCD_DEV_INFO    *lcd_dev_info_ptr,
        MMILIST_COLOR_ID_E       bg_color_id,
        GUI_RECT_T                rect,
        GUI_RECT_T                inter_rect,
        CTRLLIST_ITEM_INFO_T *item_ptr
        )
{
	    BOOLEAN     result          = FALSE;
	    GUI_RECT_T  bg_rect         = {0};
	    GUI_RECT_T  icon_disp_rect  = {0};
	    GUI_COLOR_T bg_color        = 0;
	    MMI_STRING_T text_info 	= {0};

	 if ((PNULL != list_ctrl_ptr) &&(MMITHEME_COLOR_NONE != bg_color_id) &&(PNULL != lcd_dev_info_ptr))
	 {
		if (GUI_IntersectRect(&bg_rect, inter_rect, rect))
        {
        	if(bg_color_id == MMILIST_COLOR_ALARMCLOCK){
				bg_color = GUI_RGB2RGB565(0x24,0x24,0x24);
				bg_rect.top += 2;
				bg_rect.bottom -= 2;
				bg_rect.left += 6;
				bg_rect.right -= 6;
			}else{
				bg_color = GUI_RGB2RGB565(0x00,0x00,0x00);
			}
    
#ifdef UI_MULTILAYER_SUPPORT
            UILAYER_ClearRect(lcd_dev_info_ptr, bg_rect);
#endif
		}
		LCD_FillRoundedRect(lcd_dev_info_ptr, bg_rect, bg_rect, bg_color);
		LCD_DrawRoundedRect(lcd_dev_info_ptr, bg_rect, bg_rect, bg_color);

        result = TRUE;
	 }
	return result;
}

PUBLIC void WATCHLIST_DrawItemIcon(
        const CTRLLIST_OBJ_T   *list_ctrl_ptr,   
        const CTRLLIST_ITEM_INFO_T *item_ptr,
        const GUI_RECT_T       *rect_ptr,       //the rect to display this item
        const GUI_RECT_T       *disp_rect_ptr,
        MMI_IMAGE_ID_T          icon_id,
        const GUI_LCD_DEV_INFO *dev_info_ptr,
        BOOLEAN                 is_highlight
        )
{
    uint16          icon_width  = 0;
    uint16          icon_height = 0;
    GUI_RECT_T      icon_rect   = {0};
    GUI_RECT_T      icon_disp_rect   = {0};
    GUI_RECT_T      inter_rect   = {0};
    GUI_POINT_T     dis_point   = {0}; 
    GUIITEM_THEME_STATE_E   state  = GUIITEM_THEME_STATE_DEFAULT;
    uint32          color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
    uint32          theme_color_neg_2 = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_2);
    uint32          theme_color_neg_3 = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_3);

    if (PNULL == rect_ptr || PNULL == dev_info_ptr || PNULL == list_ctrl_ptr || PNULL == item_ptr)
    {
        return;
    }

    state = LIST_GetItemThemeState(list_ctrl_ptr, item_ptr, is_highlight);/*lint !e605*/

    if (MMITHEME_CheckImageID(icon_id) && !GUI_IsRectEmpty(*rect_ptr))
    {
        //Get icon pointer from icon_id
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, icon_id, list_ctrl_ptr->win_id);

        // ???icon?????????
        icon_disp_rect = GUI_GetCenterRect(*rect_ptr, icon_width, icon_height);

        // ???????????????, ??????????????????????????????°???
        if (GUI_IntersectRect(&inter_rect, *disp_rect_ptr, icon_disp_rect))
        {
            icon_rect.right = icon_width - 1;

            if (icon_disp_rect.top < inter_rect.top)
            {
                icon_rect.top = (inter_rect.top - icon_disp_rect.top);
                icon_rect.bottom = icon_height - 1;
            }
            else if (icon_disp_rect.bottom > inter_rect.bottom)
            {
                icon_rect.top = 0;
                icon_rect.bottom = icon_height - (icon_disp_rect.bottom - inter_rect.bottom);
            }
            else
            {
                icon_rect.top = 0;
                icon_rect.bottom = icon_height - 1;
            }

            IMG_EnableTransparentColor(TRUE);
            dis_point.x = icon_rect.left;
            dis_point.y = icon_rect.top;

            if (GUIITEM_THEME_STATE_LEFT_FOCUS == state
                    || GUIITEM_THEME_STATE_RIGHT_FOCUS == state)
            {
                color = theme_color_neg_2;
            }

            //avatar mask, triangle
            if (list_ctrl_ptr->display_style.avatar_mask_image_id == icon_id)
            {
                if (GUIITEM_THEME_STATE_DEFAULT == state)
                {
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            //((CTRLBASE_OBJ_T *)list_ctrl_ptr)->bg.color
							list_ctrl_ptr->display_style.bg_info.color
                            );
                }
                else
                {
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            color
                            );
                }
            }
            else if (list_ctrl_ptr->display_style.left_triangle_icon_id == icon_id 
                    || list_ctrl_ptr->display_style.right_triangle_icon_id == icon_id)
            {
                GUIRES_DisplayImgWitchColorExchange (PNULL,
                        &inter_rect,
                        &icon_rect,
                        list_ctrl_ptr->win_id,
                        icon_id,
                        dev_info_ptr,
                        TRUE,
                        color
                        );
            }
            //check selected image - rectangle
            else if(list_ctrl_ptr->display_style.check_selected_image_id == icon_id)
            {
                if (GUIITEM_THEME_STATE_FOCUS == state
                        || GUIITEM_THEME_STATE_FOCUS_INVALID == state)
                {
                    //?????
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            theme_color_neg_3
                            );
                }
                else
                {
                    //?????
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            color
                            );
                }

                //??????
                GUIRES_DisplayImg(PNULL,
                        &inter_rect,
                        &icon_rect,
                        list_ctrl_ptr->win_id,
                        list_ctrl_ptr->display_style.marked_image_id,//???icon
                        dev_info_ptr);
            }
            //check selected image - avatar checkbox
            else if((LIST_GetItemState(item_ptr, GUIITEM_STATE_HAS_INITIAL_CHECKBOX) 
                        ||LIST_GetItemState(item_ptr, GUIITEM_STATE_HAS_AVATAR_CHECKBOX))
                    && (list_ctrl_ptr->display_style.avatar_checkbox_selected_image_id == icon_id))
            {
                if (GUIITEM_THEME_STATE_FOCUS == state
                        || GUIITEM_THEME_STATE_FOCUS_INVALID == state)
                {
                    //?????
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            theme_color_neg_3
                            );
                }
                else
                {
                    //?????
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            color
                            );
                }

                //??????
                GUIRES_DisplayImg(PNULL,
                        &inter_rect,
                        &icon_rect,
                        list_ctrl_ptr->win_id,
                        list_ctrl_ptr->display_style.avatar_marked_image_id,//???icon
                        dev_info_ptr);

            }
            //radio
            else if(list_ctrl_ptr->display_style.radio_selected_image_id == icon_id
                    || list_ctrl_ptr->display_style.radio_unselected_image_id == icon_id)
            {
                if (GUIITEM_THEME_STATE_FOCUS == state
                        || GUIITEM_THEME_STATE_FOCUS_INVALID == state)
                {
#if 0                
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            theme_color_neg_3
                            );
#endif
                    GUIRES_DisplayImg(PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr);
                    
                }
                else
                {
#if 0                
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            color
                            );
#endif
                    GUIRES_DisplayImg(PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr);
                }

            }
            //toggle
            else if(LIST_GetItemState(item_ptr, GUIITEM_STATE_HAS_TOGGLE_MARK))
            {
                if (GUIITEM_THEME_STATE_FOCUS == state
                        || GUIITEM_THEME_STATE_FOCUS_INVALID == state)
                {
#if 0
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            theme_color_neg_3
                            );
#else
                    GUIRES_DisplayImg(PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr);
#endif                
                }
                //[toggle quick action list draw
                // item quick icon image don't exchange its color]
                else if(state == GUIITEM_THEME_STATE_LEFT_FOCUS
                        && list_ctrl_ptr->is_show_left_triange_image
                        && icon_id == list_ctrl_ptr->display_style.left_quick_icon_image)
                {
                    GUIRES_DisplayImg(PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            list_ctrl_ptr->display_style.left_quick_icon_image,
                            dev_info_ptr);
                }
                //[end]
                else
                {
#if 0
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            color
                            );
#else
                    GUIRES_DisplayImg(PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr);
#endif
                }

            }
            else if(list_ctrl_ptr->display_style.avatar_checkbox_unselected_avatar_image_2_id == icon_id)
            {
                GUIRES_DisplayImg(PNULL,
                        &inter_rect,
                        &icon_rect,
                        list_ctrl_ptr->win_id,
                        list_ctrl_ptr->display_style.avatar_checkbox_unselected_avatar_image_1_id,
                        dev_info_ptr);

                if (GUIITEM_THEME_STATE_DEFAULT == state)
                {
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->bg.color
                            );
                }
                else
                {
                    GUIRES_DisplayImgWitchColorExchange (PNULL,
                            &inter_rect,
                            &icon_rect,
                            list_ctrl_ptr->win_id,
                            icon_id,
                            dev_info_ptr,
                            TRUE,
                            color
                            ); 
                }
            }
            //other
            else
            {
                GUIRES_DisplayImg(PNULL,
                        &inter_rect,
                        &icon_rect,
                        list_ctrl_ptr->win_id,
                        icon_id,
                        dev_info_ptr);
            }

            IMG_EnableTransparentColor(FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : draw icon in item
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawItemIcon(
    const CTRLLIST_OBJ_T   *list_ctrl_ptr,
    const GUI_RECT_T       *rect_ptr,       //the rect to display this item
    const GUI_RECT_T       *disp_rect_ptr,
    MMI_IMAGE_ID_T          icon_id,
    const GUI_LCD_DEV_INFO *dev_info_ptr
)
{
    uint16          icon_width  = 0;
    uint16          icon_height = 0;
    GUI_RECT_T      icon_rect   = {0};
    GUI_RECT_T      icon_disp_rect   = {0};
    GUI_RECT_T      inter_rect   = {0};
    GUI_POINT_T     dis_point   = {0};

    if (PNULL == rect_ptr || PNULL == dev_info_ptr || PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (MMITHEME_CheckImageID(icon_id) && !GUI_IsRectEmpty(*rect_ptr))
    {
        //Get icon pointer from icon_id
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, icon_id, list_ctrl_ptr->win_id);

        // 获得icon显示的区域
        icon_disp_rect = GUI_GetCenterRect(*rect_ptr, icon_width, icon_height);

        // 这里需要矫正区域, 以矫正区域来确定显示图片的上半部分还是下半部分
        if (GUI_IntersectRect(&inter_rect, *disp_rect_ptr, icon_disp_rect))
        {
            icon_rect.right = icon_width - 1;

            if (icon_disp_rect.top < inter_rect.top)
            {
                icon_rect.top = (inter_rect.top - icon_disp_rect.top);
                icon_rect.bottom = icon_height - 1;
            }
            else if (icon_disp_rect.bottom > inter_rect.bottom)
            {
                icon_rect.top = 0;
                icon_rect.bottom = icon_height - (icon_disp_rect.bottom - inter_rect.bottom);
            }
            else
            {
                icon_rect.top = 0;
                icon_rect.bottom = icon_height - 1;
            }

            IMG_EnableTransparentColor(TRUE);
            dis_point.x = icon_rect.left;
            dis_point.y = icon_rect.top;
            GUIRES_DisplayImg(PNULL,
                              &inter_rect,
                              &icon_rect,
                              list_ctrl_ptr->win_id,
                              icon_id,
                              dev_info_ptr);
            IMG_EnableTransparentColor(FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : item content画背景
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DrawItemContentBgColor(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_info_ptr,
    MMITHEME_COLOR_ID_E       bg_color_id,
    GUI_RECT_T                rect,
    GUI_RECT_T                inter_rect,
    MMI_IMAGE_ID_T            image_id
)
{
    uint16      icon_width      = 0;
    uint16      icon_height     = 0;
    BOOLEAN     result          = FALSE;
    GUI_RECT_T  bg_rect         = {0};
    GUI_RECT_T  icon_disp_rect  = {0};
    GUI_COLOR_T bg_color        = 0;

    if ((PNULL != list_ctrl_ptr) &&
        (MMITHEME_COLOR_NONE != bg_color_id) &&
        (PNULL != lcd_dev_info_ptr))
    {
        //获得bg 区域
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, image_id, list_ctrl_ptr->win_id);

        icon_disp_rect = GUI_GetCenterRectExt(rect, icon_width, icon_height);

        GUI_IntersectRect(&bg_rect, inter_rect, icon_disp_rect);

        //获取bg color
        bg_color = MMITHEME_GetItemContentColor(bg_color_id);

#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_ClearRect(lcd_dev_info_ptr, bg_rect);
#endif

        //画背景
        LCD_FillRect(lcd_dev_info_ptr, bg_rect, bg_color);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : item content画背景
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DrawItemContentBgIcon(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_info_ptr,
    GUI_RECT_T                item_rect,
    GUI_RECT_T                content_rect,
    MMI_IMAGE_ID_T            icon_id
)
{
    uint16      icon_width      = 0;
    uint16      icon_height     = 0;
    BOOLEAN     result          = FALSE;
    GUI_RECT_T  icon_rect         = {0};
    GUI_RECT_T  icon_disp_rect  = {0};
    GUI_RECT_T  clip_rect  = {0};
    GUI_RECT_T  disp_rect  = {0};
    GUI_RECT_T  valid_rect  = {0};

    if ((PNULL != list_ctrl_ptr) && MMITHEME_CheckImageID(icon_id) && (PNULL != lcd_dev_info_ptr))
    {
        //获得bg 区域
        if (GUIRES_GetImgWidthHeight(&icon_width, &icon_height, icon_id, list_ctrl_ptr->win_id))
        {
            LIST_GetValidRect(list_ctrl_ptr, &valid_rect);

            GUI_IntersectRect(&disp_rect, valid_rect, item_rect);

            icon_disp_rect = content_rect;
            icon_disp_rect.left = icon_disp_rect.left + 1;
            icon_disp_rect.right = icon_disp_rect.left + icon_width;
            icon_disp_rect.bottom = icon_disp_rect.bottom + icon_height / 4;
            icon_disp_rect.top = icon_disp_rect.bottom - icon_height + 1;

            icon_rect.right = icon_width - 1;
            icon_rect.bottom = icon_height - 1;

            GUI_IntersectRect(&clip_rect, icon_disp_rect, disp_rect);

            if (icon_disp_rect.top < clip_rect.top)
            {
                icon_rect.top = (clip_rect.top - icon_disp_rect.top);
                icon_rect.bottom = icon_height - 1;
            }
            else if (icon_disp_rect.bottom > clip_rect.bottom)
            {
                icon_rect.top = 0;
                icon_rect.bottom = icon_height - (icon_disp_rect.bottom - clip_rect.bottom);
            }
            else
            {
                icon_rect.top = 0;
                icon_rect.bottom = icon_height - 1;
            }

            IMG_EnableTransparentColor(TRUE);
            GUIRES_DisplayImg(PNULL,
                              &icon_disp_rect,
                              &icon_rect,
                              list_ctrl_ptr->win_id,
                              icon_id,
                              lcd_dev_info_ptr);
            IMG_EnableTransparentColor(FALSE);
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SetItemDataByPtr(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,
    const GUILIST_ITEM_DATA_T *item_data_ptr,
    uint16 pos
)
{
    BOOLEAN result = FALSE;

    if (!LIST_TypeOf(list_ctrl_ptr) || PNULL == item_data_ptr)
    {
        return result;
    }

    if (pos < list_ctrl_ptr->total_item_num
        && LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA)
        /*&& LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ON_ITEM_DATA)*/)
    {
        CTRLLIST_ITEM_INFO_T *item_ptr  = LIST_GetItemInfoPtr(list_ctrl_ptr, pos);

        LIST_CreateItemData(list_ctrl_ptr, item_ptr, item_data_ptr);

        if (PNULL != item_ptr)
        {
            LIST_SetItemState(item_ptr, INNER_STATE_TO_UPDATE_HEIGHT, TRUE);

            LIST_ResetItemHeight(list_ctrl_ptr, pos, pos);
        }

        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
//  Description : create item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_CreateItemData(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,  // the list control pointer
    CTRLLIST_ITEM_INFO_T       *item_ptr,
    const GUILIST_ITEM_DATA_T *item_data_ptr
)
{
    uint32 i = 0;
    uint16 str_len = 0;

    if (PNULL == list_ctrl_ptr || PNULL == item_ptr || PNULL == item_data_ptr)
    {
        return FALSE;
    }

    if (PNULL != item_ptr->item.data_ptr)
    {
        LIST_DestroyItemData(list_ctrl_ptr, item_ptr, FALSE);
    }

    item_ptr->item.data_ptr = (GUILIST_ITEM_DATA_T *)SCI_ALLOC_APP(sizeof(GUILIST_ITEM_DATA_T));

    SCI_MEMCPY(item_ptr->item.data_ptr, item_data_ptr, sizeof(GUILIST_ITEM_DATA_T));

    //计算所有content中字符串的长度
    for (i = 0; i < GUIITEM_CONTENT_MAX_NUM; i++)
    {
        if (GUIITEM_DATA_TEXT_BUFFER != item_data_ptr->item_content[i].item_data_type)
        {
            continue;
        }

        if (PNULL == item_data_ptr->item_content[i].item_data.text_buffer.wstr_ptr)
        {
            continue;
        }

        str_len = MIN(CTRLLIST_MAX_STRLEN, item_data_ptr->item_content[i].item_data.text_buffer.wstr_len);
        item_ptr->item.data_ptr->item_content[i].item_data.text_buffer.wstr_len = str_len;
        item_ptr->item.data_ptr->item_content[i].item_data.text_buffer.wstr_ptr = SCI_ALLOC_APP(str_len * sizeof(wchar));

        MMIAPICOM_Wstrncpy(item_ptr->item.data_ptr->item_content[i].item_data.text_buffer.wstr_ptr,
                           item_data_ptr->item_content[i].item_data.text_buffer.wstr_ptr,
                           str_len);

        GUI_ReplaceChar(&item_ptr->item.data_ptr->item_content[i].item_data.text_buffer, LF_CHAR, BL_CHAR);
        GUI_ReplaceChar(&item_ptr->item.data_ptr->item_content[i].item_data.text_buffer, CR_CHAR, BL_CHAR);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetItemHeight(
    CTRLLIST_OBJ_T *list_ctrl_ptr, // 控件指针
    uint16 pre_idx,  // 移动方向
    uint16 cur_idx
)
{
#if 0
    int32   pre_offset_height = 0;
    int32   cur_offset_height = 0;
    int32   pre_cur_pre_height = 0; // 前一条目的之前的高度
    int32   pre_cur_cur_height = 0; // 前一条目的现在的高度
    int32   cur_pre_height = 0; // 现在条目的之前的高度
    int32   cur_cur_height = 0; // 现在条目的现在的高度
    CTRLLIST_ITEM_INFO_T *pre_cur_item_info_ptr = PNULL;
    CTRLLIST_ITEM_INFO_T *cur_item_info_ptr     = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (pre_idx >= list_ctrl_ptr->total_item_num || cur_idx >= list_ctrl_ptr->total_item_num)
    {
        return;
    }

    pre_cur_item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, pre_idx);

    if (PNULL != pre_cur_item_info_ptr)
    {
        pre_cur_pre_height = pre_cur_item_info_ptr->item_height;
    }
    else
    {
        return;
    }

    cur_item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, cur_idx);

    if (PNULL != cur_item_info_ptr)
    {
        cur_pre_height = cur_item_info_ptr->item_height;
    }
    else
    {
        return;
    }

    // 两个区域不同的时候，需要进行计算
    pre_cur_cur_height = pre_cur_item_info_ptr->item.style_ptr->height;
    cur_cur_height = cur_item_info_ptr->item.style_ptr->height_focus;

    pre_offset_height = pre_cur_cur_height - pre_cur_pre_height;
    cur_offset_height = cur_cur_height - cur_pre_height;

    list_ctrl_ptr->total_item_height += cur_offset_height + pre_offset_height;

    // 1、当两个高度都没有发生变化的时候
    if (0 != pre_offset_height || 0 != cur_offset_height)// 都相等
    {
        pre_cur_item_info_ptr->item_height = pre_cur_cur_height; // 记录高度
        cur_item_info_ptr->item_height = cur_cur_height; // 记录高度

        if (cur_idx != pre_idx && 0 != pre_offset_height)
        {
            LIST_ResetItemDisp(list_ctrl_ptr, pre_idx);
        }
        if (0 != cur_offset_height)
        {
            LIST_ResetItemDisp(list_ctrl_ptr, cur_idx);
        }

        list_ctrl_ptr->is_pos_dirty = TRUE;
    }
#endif
    return;
}

#if 0
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: by xiaoqing, 2012-9-19日，暂时屏蔽该函数
/*****************************************************************************/
PUBLIC void LIST_AdjustItemHeightAndTop(
    CTRLLIST_OBJ_T *list_ctrl_ptr, // 控件指针
    uint16 pre_idx,  // 移动方向
    uint16 cur_idx
)
{
    int32   i = 0;
    int32   pre_offset_height = 0;
    int32   cur_offset_height = 0;
    int32   pre_cur_pre_height = 0; // 前一条目的之前的高度
    int32   pre_cur_cur_height = 0; // 前一条目的现在的高度
    int32   cur_pre_height = 0; // 现在条目的之前的高度
    int32   cur_cur_height = 0; // 现在条目的现在的高度
    uint16  pre_cur_item_idx = 0;  //
    uint16  cur_item_idx = 0;
    CTRLLIST_ITEM_INFO_T *pre_cur_item_info_ptr = PNULL;
    CTRLLIST_ITEM_INFO_T *cur_item_info_ptr     = PNULL;
    CTRLLIST_ITEM_INFO_T *item_ptr     = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (pre_idx >= list_ctrl_ptr->total_item_num
        || cur_idx >= list_ctrl_ptr->total_item_num)
    {
        return;
    }

    if (pre_idx <= cur_idx)
    {
        pre_cur_item_idx = pre_idx;
        cur_item_idx = cur_idx;
    }
    else
    {
        pre_cur_item_idx = cur_idx;
        cur_item_idx = pre_idx;
    }

    pre_cur_item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, pre_cur_item_idx);

    if (PNULL != pre_cur_item_info_ptr)
    {
        pre_cur_pre_height = pre_cur_item_info_ptr->item_height;
    }
    else
    {
        return;
    }

    cur_item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, cur_item_idx);

    if (PNULL != cur_item_info_ptr)
    {
        cur_pre_height = cur_item_info_ptr->item_height;
    }
    else
    {
        return;
    }

    // 两个区域不同的时候，需要进行计算
    pre_cur_cur_height = pre_cur_item_info_ptr->item.style_ptr->height;// LIST_GetItemHeight(list_ctrl_ptr, pre_cur_item_idx);
    cur_cur_height = cur_item_info_ptr->item.style_ptr->height_focus;//LIST_GetItemHeight(list_ctrl_ptr, cur_item_idx);

    pre_offset_height = pre_cur_cur_height - pre_cur_pre_height;
    cur_offset_height = cur_cur_height - cur_pre_height;

    if (pre_idx == cur_idx && 0 != pre_offset_height) // 当前item高度发生变化时
    {
        pre_cur_item_info_ptr->item_height = pre_cur_cur_height;

        for (i = cur_item_idx + 1; i < list_ctrl_ptr->total_item_num; i++)
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

            if (PNULL != item_ptr)
            {
                item_ptr->item_top += (pre_offset_height);
            }
        }

        list_ctrl_ptr->total_item_height += cur_offset_height;

        list_ctrl_ptr->is_pos_dirty = TRUE;

        return;
    }

    // 1、当两个高度都没有发生变化的时候
    if (pre_cur_pre_height == pre_cur_cur_height && cur_pre_height == cur_cur_height)// 都相等
    {
        return;
    }
    else if (pre_cur_pre_height != pre_cur_cur_height && cur_pre_height != cur_cur_height) // 都不相等
    {
        pre_cur_item_info_ptr->item_height = pre_cur_cur_height; // 记录高度

        // pre_cur_item_idx 之后的所有item top值均发生变化
        for (i = pre_cur_item_idx + 1; i <= cur_item_idx; i++)
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

            if (PNULL != item_ptr)
            {
                item_ptr->item_top += pre_offset_height;
            }
        }

        cur_item_info_ptr->item_height = cur_cur_height; // 记录高度

        // cur_item_idx 之后的所有item top值均发生变化
        if (abs(cur_offset_height) != abs(pre_offset_height))
        {
            for (i = cur_item_idx + 1; i < list_ctrl_ptr->total_item_num; i++)
            {
                item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

                if (PNULL != item_ptr)
                {
                    item_ptr->item_top += (pre_offset_height) + (cur_offset_height);
                }
            }
        }

        list_ctrl_ptr->is_pos_dirty = TRUE;
    }
    else if (pre_cur_pre_height == pre_cur_cur_height && cur_pre_height != cur_cur_height) // pre的相等
    {
        cur_item_info_ptr->item_height = cur_cur_height; // 记录高度

        // cur_item_idx 当前item之后的所有item的top值发生变化
        for (i = cur_item_idx + 1; i < list_ctrl_ptr->total_item_num; i++)
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

            if (PNULL != item_ptr)
            {
                item_ptr->item_top += cur_offset_height;
            }
        }

        list_ctrl_ptr->is_pos_dirty = TRUE;
    }
    else if (pre_cur_pre_height != pre_cur_cur_height && cur_pre_height == cur_cur_height) // cur的相等
    {
        pre_cur_item_info_ptr->item_height = pre_cur_cur_height; // 记录高度

        // pre_cur_item_idx 之后的所有item top值均发生变化
        for (i = pre_cur_item_idx + 1; i < list_ctrl_ptr->total_item_num; i++)
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

            if (PNULL != item_ptr)
            {
                item_ptr->item_top += pre_offset_height;
            }
        }

        list_ctrl_ptr->is_pos_dirty = TRUE;
    }
    else
    {
        return;
    }

    list_ctrl_ptr->total_item_height += cur_offset_height + pre_offset_height;

    return;
}
#endif // #if 0

/*****************************************************************************/
//  Description : send need item data message
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SendNeedItemDataMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         item_index
)
{
    GUILIST_NEED_ITEM_DATA_T need_item_data = {0};

    if (PNULL != list_ctrl_ptr)
    {
        need_item_data.ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        need_item_data.ctrl_id = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
        need_item_data.item_index = item_index;

        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_ON_ITEM_DATA, TRUE);

        GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_NEED_ITEM_DATA, &need_item_data);

        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_ON_ITEM_DATA, FALSE);
    }
}

/*****************************************************************************/
//  Description : send need item content message
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SendNeedItemContentMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          item_index,
    uint16          item_content_index
)
{
    GUILIST_NEED_ITEM_CONTENT_T need_item_content = {0};

    if (PNULL != list_ctrl_ptr)
    {
        need_item_content.ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        need_item_content.ctrl_id = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
        need_item_content.item_index         = item_index;
        need_item_content.item_content_index = item_content_index;

        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_ON_ITEM_CONTENT, TRUE);

        GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_NEED_ITEM_CONTENT, &need_item_content);

        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_ON_ITEM_CONTENT, FALSE);
    }
}

/*****************************************************************************/
//  Description : send item content click message
//  Parameter: [In] p_list_ctrl
//             [In] item_index
//             [In] item_content_index
//             [Return] none
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SendItemContentClickMsg(
    CTRLLIST_OBJ_T* p_list_ctrl,
    uint16          item_index,
    uint16          item_content_index
)
{
    GUILIST_ITEM_CONTENT_CLICK_T item_content_click = {0};

    if (PNULL != p_list_ctrl)
    {
        item_content_click.ctrl_handle = ((CTRLBASE_OBJ_T *)p_list_ctrl)->handle;
        item_content_click.ctrl_id = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)p_list_ctrl)->handle);
        item_content_click.item_index = item_index;
        item_content_click.item_content_index = item_content_index;

        LIST_SetInnerState(p_list_ctrl, INNER_STATE_ON_ITEM_CONTENT, TRUE);

        GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)p_list_ctrl)->handle, MSG_NOTIFY_LIST_ITEM_CONTENT_CLICK, &item_content_click);

        LIST_SetInnerState(p_list_ctrl, INNER_STATE_ON_ITEM_CONTENT, FALSE);
    }
    else
    {
        SCI_TRACE_LOW("p_list_ctrl is PNULL");
    }
}

/*****************************************************************************/
//  Description : post mark item
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_PostMarkItemMsg(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16                item_index,
    BOOLEAN               is_selected
)
{
    GUILIST_MARK_ITEM_T mark_item = {0};

    if (PNULL != list_ctrl_ptr)
    {
        mark_item.ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        mark_item.ctrl_id     = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
        mark_item.item_idx    = item_index;
        mark_item.had_maked   = is_selected;

        GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_MARK_ITEM, &mark_item, sizeof(GUILIST_MARK_ITEM_T));
    }
}

/*****************************************************************************/
//  Description : get item state by index
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetItemStateByIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index,
    GUIITEM_STATE_T item_state
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if (PNULL != list_ctrl_ptr)
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

        if (PNULL != item_ptr)
        {
            result = LIST_GetItemState(item_ptr, item_state);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : set the inner state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:设置list内部状态
/*****************************************************************************/
PUBLIC void LIST_SetItemMoveState(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN         is_true
)
{
    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_SLIDE_ITEM, is_true);
}

/*****************************************************************************/
//  Description : get item's backgroup image id
//  Global resource dependence :
//  Author:
//  Note: if(is_highlight == TRUE): get item's highlisth backgroup image
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T LIST_GetItemBgImageID(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         index,
    BOOLEAN         is_highlight
)
{
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;//
    CTRLLIST_ITEM_INFO_T *next_item_info_ptr = PNULL;//
    GUIITEM_THEME_BGIMAGE_TYPE_E  item_bg_type = 0;
    BOOLEAN             is_head = FALSE;
    BOOLEAN             is_tail = FALSE;


    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

    if (item_ptr == PNULL)
    {
        return 0;
    }

    //判断是否组的开头
    if (index == 0)
    {
        //第一个纪录肯定是组的开头
        is_head = TRUE;
    }
    else
    {
        next_item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index - 1);

        if (PNULL != next_item_info_ptr &&  next_item_info_ptr->item.item_group != item_ptr->item.item_group)
        {
            //和前面一个纪录的组不一样，该纪录是一个新组的开始
            is_head = TRUE;
        }
    }

    //判断是否组的结尾
    if (index + 1 >= list_ctrl_ptr->total_item_num)
    {
        //最后一个纪录可能是组的结尾
        is_tail = TRUE;
    }
    else
    {
        next_item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index + 1);

        if (PNULL != next_item_info_ptr && next_item_info_ptr->item.item_group != item_ptr->item.item_group)
        {
            //和后面一个纪录的组不一样，该纪录是一个组的结尾
            is_tail = TRUE;
        }
    }

    if (is_head && is_tail)
    {
        //只有一个纪录的组
        if (is_highlight)
        {
            item_bg_type = GUIITEM_THEME_HIGHLIGHT_ITEM_FULL_BG;
        }
        else
        {
            item_bg_type = GUIITEM_THEME_ITEM_FULL_BG;
        }
    }
    else if (is_head)
    {
        //组的开头
        if (is_highlight)
        {
            item_bg_type = GUIITEM_THEME_HIGHLIGHT_ITEM_HEAD_BG;
        }
        else
        {
            item_bg_type = GUIITEM_THEME_ITEM_HEAD_BG;
        }
    }
    else if (is_tail)
    {
        //组的结尾
        if (is_highlight)
        {
            item_bg_type = GUIITEM_THEME_HIGHLIGHT_ITEM_TAIL_BG;
        }
        else
        {
            item_bg_type = GUIITEM_THEME_ITEM_TAIL_BG;
        }
    }
    else
    {
        //组的中间
        if (is_highlight)
        {
            item_bg_type = GUIITEM_THEME_HIGHLIGHT_ITEM_BODAY_BG;
        }
        else
        {
            item_bg_type = GUIITEM_THEME_ITEM_BODAY_BG;
        }
    }

    return  MMITHEME_GetListItemBGImage(item_bg_type);
}

/*****************************************************************************/
//  Description : 判断是否组的开头
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_ItemIsGroupHead(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         index
)
{
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;//
    CTRLLIST_ITEM_INFO_T *pre_item_info_ptr = PNULL;//
    BOOLEAN             is_head = FALSE;

    //判断是否组的开头
    if (index == 0)
    {
        //第一个纪录肯定是组的开头
        is_head = TRUE;
    }
    else
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

        pre_item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index - 1);

        if (PNULL != item_ptr && PNULL != pre_item_info_ptr
            && pre_item_info_ptr->item.item_group != item_ptr->item.item_group)
        {
            //和前面一个纪录的组不一样，该纪录是一个新组的开始
            is_head = TRUE;
        }
    }

    return  is_head;
}

/*****************************************************************************/
//  Description : 获取本纪录的split字符串
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_GetItemSplitText(
    CTRLLIST_OBJ_T *list_ctrl_ptr,//IN
    uint16         cur_index,//IN
    MMI_STRING_T   *item_text_ptr//OUT
)
{
    uint16 i = 0;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if (list_ctrl_ptr == PNULL || item_text_ptr == PNULL)
    {
        return;
    }

    for (i = cur_index; i > 0; i--)
    {
        //find split item
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

        if (item_ptr != PNULL && LIST_GetItemState(item_ptr, GUIITEM_STATE_SPLIT))
        {
            break;
        }
    }

    //get item info
    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

    /* get dyna data */
    if (item_ptr != PNULL)
    {
        if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA) && PNULL == item_ptr->item.data_ptr)
        {
            LIST_SendNeedItemDataMsg(list_ctrl_ptr, i);
        }

        //get text
        for (i = 0; i < GUIITEM_CONTENT_MAX_NUM; i++)
        {
            //get first text string
            switch (item_ptr->item.data_ptr->item_content[i].item_data_type)
            {
                case GUIITEM_DATA_TEXT_ID:
                case GUIITEM_DATA_TEXT_BUFFER:
                    LIST_GetListItemContentTextInfo(list_ctrl_ptr, item_ptr, i, item_text_ptr);
                    break;

                default:
                    break;
            }
        }
    }
}
/*****************************************************************************/
//  Description : 计算item的实际rect
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_CalculateListItemRect(
    GUI_RECT_T     *rect_ptr,     // OUT, item rect
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // 控件指针
    uint16          item_index    // IN, 当前页中item索引号，从0到page_item_num - 1
)
{
#define INVALID_VALUE 0X7FFF
    BOOLEAN result        = TRUE;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;
    int32 item_top = 0;
    int32 invalid_value = INVALID_VALUE;

    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

    if ((PNULL == rect_ptr) || (PNULL == list_ctrl_ptr) || (PNULL == item_ptr))
    {
        return FALSE;
    }

    if (item_index >= list_ctrl_ptr->total_item_num)
    {
        result = FALSE;
    }
    else
    {
        GUI_RECT_T item_rect         = {0};
        GUI_RECT_T list_rect       = {0};

        list_rect = LIST_AdjustRect(list_ctrl_ptr);

        item_top = item_ptr->item_top + list_ctrl_ptr->offset_y  - list_rect.top;

        if (invalid_value <= (item_top + item_ptr->item_height - 1) || -invalid_value >= item_top)
        {
            result = FALSE;
        }
        else
        {
            item_rect.top = item_top;
            item_rect.bottom = item_rect.top + item_ptr->item_height - 1;

            item_rect.left  = (int16)(list_rect.left + list_ctrl_ptr->display_style.left_space);
            item_rect.right = (int16)(list_rect.right - list_ctrl_ptr->display_style.right_space);

            if (LIST_IsNeedScrollBar(list_ctrl_ptr))
            {
                item_rect.right = (int16)(item_rect.right - (MMITHEME_GetScrollBarWidthByState(list_ctrl_ptr->display_style.is_slide_used)));
                GUI_CheckRectDirection(&list_rect, &item_rect);
            }
        }

        *rect_ptr = item_rect;
    }

    return result;
}

/*****************************************************************************/
//  Description : 判断item是否可以焦点显示
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_IsItemGetFocus(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index
)
{
    if (item_index < list_ctrl_ptr->total_item_num)
    {
        return (BOOLEAN)(!LIST_GetItemStateByIndex(list_ctrl_ptr, item_index, GUIITEM_STATE_SPLIT));  // 目前之后split状态不能显示焦点
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : post mark item
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC GUILIST_QRERY_SELECT_ITEM_T LIST_QueryMarkItemMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16               item_index,
    MMI_MESSAGE_ID_E     msg_id
)
{
    GUILIST_QRERY_SELECT_ITEM_T mark_item = {0};

    SCI_ASSERT(PNULL != list_ctrl_ptr);   /*assert verified*/

    mark_item.ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
    mark_item.ctrl_id     = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
    mark_item.item_idx    = item_index;
    mark_item.msg_id      = msg_id;
    mark_item.is_check_select = FALSE;

    GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_SELECT_QUERY, &mark_item);

    return mark_item;
}

/*==============================================================================
Description: 计算splitbar的区域
Global resource dependence: NONE
Author:
Note:
==============================================================================*/
PUBLIC GUI_RECT_T LIST_GetSplitBarRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T split_rect = {0};
    CTRLLIST_ITEM_INFO_T *first_item_ptr = PNULL;
    CTRLLIST_ITEM_INFO_T *second_item_ptr = PNULL;
    THEMELIST_ITEM_STYLE_T *split_style_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return split_rect;
    }

    split_style_ptr = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT);

    split_rect.top = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect.top;
    split_rect.bottom = split_rect.top + split_style_ptr->height - 1;
    split_rect.left = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect.left;
    split_rect.right = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect.right;

    first_item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->top_item_idx);
    LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->top_item_idx);

    if (item_rect.top > ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect.top) //第一个split,需要跟着listbox一起向下滑
    {
        split_rect.top = item_rect.top;
        split_rect.bottom = split_rect.top + split_style_ptr->height - 1;
    }
    else if (!LIST_GetItemState(first_item_ptr, GUIITEM_STATE_SPLIT)) //第一个不是split，需要计算head split是否完整
    {
        if ((item_rect.bottom <  split_rect.top + split_style_ptr->height)
            && (list_ctrl_ptr->top_item_idx + 1 < list_ctrl_ptr->total_item_num))
        {
            //第一个纪录位置在head split内部，没有完全覆盖head split，需要判断下一个纪录是否是split
            second_item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->top_item_idx + 1);

            if (LIST_GetItemState(second_item_ptr, GUIITEM_STATE_SPLIT))
            {
                split_rect.top = item_rect.bottom - split_style_ptr->height + 1;
                split_rect.bottom = item_rect.bottom;
            }
        }
    }

    GUI_CheckRectDirection(&((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect, &split_rect);

    return split_rect;
}

#ifdef UI_MULTILAYER_SUPPORT
/*==============================================================================
Description: 创建splitbar的显示层
Global resource dependence: NONE
Author:
Note:
==============================================================================*/
PUBLIC BOOLEAN LIST_CreateSplitBarLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *split_rect_ptr
)
{
    if (!UILAYER_IsMultiLayerEnable())
    {
        return FALSE;
    }

    if (PNULL == list_ctrl_ptr || PNULL == split_rect_ptr)
    {
        return FALSE;
    }

    if (UILAYER_NULL_HANDLE == list_ctrl_ptr->split_layer.block_id)
    {
        UILAYER_CREATE_T create_info = {0};

        create_info.lcd_id = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info.lcd_id;
        create_info.owner_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        create_info.offset_x = split_rect_ptr->left;
        create_info.offset_y = split_rect_ptr->top;
        create_info.width = split_rect_ptr->right - split_rect_ptr->left + 1;
        create_info.height = split_rect_ptr->bottom - split_rect_ptr->top + 1;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_DOUBLE_COPY;

        UILAYER_CreateLayer(&create_info, &list_ctrl_ptr->split_layer);
        UILAYER_SetLayerColorKey(&list_ctrl_ptr->split_layer, TRUE, UILAYER_TRANSPARENT_COLOR);
    }

    return TRUE;
}

/*==============================================================================
Description: 清除Split层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void LIST_ReleaseSplitLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    if (!UILAYER_IsMultiLayerEnable())
    {
        return;
    }

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    UILAYER_RELEASELAYER(&(list_ctrl_ptr->split_layer));   /*lint !e506 !e774*/

    return;
}
#endif

/*****************************************************************************/
//  Description : reset top item index, end item index, and offset each
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetTopItemAndEndItemIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
)
{
    uint16 i = 0;
    uint16 item_num = 0;
    uint16 item_height = 0;
    uint16 total_item_num = 0;
    uint32 total_item_height = 0;
    int32  offset_y = 0;
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T inter_rect = {0};
    BOOLEAN is_start = FALSE;
    GUI_RECT_T list_rect = {0};
    CTRLLIST_ITEM_INFO_T  *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_ctrl_ptr->pre_end_item_idx = list_ctrl_ptr->end_item_idx;
    list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;

    total_item_num = list_ctrl_ptr->total_item_num;

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    if (0 == total_item_num)
    {
        list_ctrl_ptr->end_item_idx = list_ctrl_ptr->top_item_idx = 0;
        list_ctrl_ptr->offset_y = list_rect.top;
        return;
    }

    offset_y = list_ctrl_ptr->offset_y;

    item_rect.left  = (int16)(list_rect.left + list_ctrl_ptr->display_style.left_space);
    item_rect.right = (int16)(list_rect.right - list_ctrl_ptr->display_style.right_space);

    if (LIST_IsNeedScrollBar(list_ctrl_ptr))
    {
        item_rect.right = (int16)(item_rect.right - (MMITHEME_GetScrollBarWidthByState(list_ctrl_ptr->display_style.is_slide_used) + 2));
        GUI_CheckRectDirection(&list_rect, &item_rect);
    }

    for (i = 0; i < total_item_num; i++)
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

        if (NULL != item_ptr)
        {
            item_height = item_ptr->item_height;
        }

        total_item_height += item_height;

        if (offset_y + (int32)total_item_height - (int32)item_height > list_rect.bottom /*lint !e737*/
            || offset_y + (int32)total_item_height - 1 < list_rect.top) /*lint !e737*/
        {
            if (is_start) // 说明已经计算过
            {
                break;
            }
            else
            {
                continue;
            }
        }

        item_rect.top = offset_y + (int32)total_item_height - (int32)item_height;/*lint !e737*/

        item_rect.bottom = item_rect.top + item_height - 1;

        if (GUI_IntersectRect(&inter_rect, item_rect, list_rect))
        {
            item_num++;

            if (!is_start)
            {
                list_ctrl_ptr->top_item_idx = i;
            }

            is_start = TRUE;
        }
    }

    if (item_num > 0)
    {
        list_ctrl_ptr->end_item_idx = list_ctrl_ptr->top_item_idx + item_num - 1;
    }

    return;
}

/*****************************************************************************/
//  Description : calc top item index, end item index, and offset each
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_CalcEndItemByTopIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    int32 top_item_index
)
{
    int32 i = 0;
    int32  offset_y = 0;
    int32 item_num = 0;
    int32 total_item_num = 0;
    int32 end_item_idx = 0;
    int32 space_len = 0;
    GUI_RECT_T list_rect = {0};
    CTRLLIST_ITEM_INFO_T  *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    total_item_num = list_ctrl_ptr->total_item_num;

    if (0 == total_item_num)
    {
        return list_ctrl_ptr->end_item_idx; // 不改变
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    offset_y = list_ctrl_ptr->offset_y - list_rect.top; // 相对偏移量

    // 计算end index
    for (i = top_item_index; i < total_item_num; i++)   // 从之前的top index向后计算，到total值
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

        if (PNULL == item_ptr)
        {
            break;
        }

        // 判断item bottom值是否大于控件的top
        if (offset_y + item_ptr->item_top + item_ptr->item_height + space_len - 1 >= list_rect.bottom)
        {
            break;
        }

        item_num++;

    }

    // 矫正end index，防止出借
    end_item_idx = MIN(total_item_num - 1, top_item_index + item_num);

    return end_item_idx;
}

/*****************************************************************************/
//  Description : reset top item index, end item index, and offset each
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetTopItemAndEndItemIndexExt(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    int32 move_space
)
{
    int32 i = 0;
    int32  offset_y = 0;
    int32 item_num = 0;
    int32 total_item_num = 0;
    int32 pre_top_item_idx = 0;
    GUI_RECT_T list_rect = {0};
    CTRLLIST_ITEM_INFO_T  *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    list_ctrl_ptr->pre_end_item_idx = list_ctrl_ptr->end_item_idx;
    list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;

    pre_top_item_idx = list_ctrl_ptr->top_item_idx;
    total_item_num = list_ctrl_ptr->total_item_num;

    if (0 == total_item_num)
    {
        list_ctrl_ptr->end_item_idx = list_ctrl_ptr->top_item_idx = 0;
        list_ctrl_ptr->offset_y = list_rect.top;
        return;
    }

    offset_y = list_ctrl_ptr->offset_y - list_rect.top; // 相对偏移量

    if (move_space > 0) // downwards
    {
        for (i = pre_top_item_idx; i >= 0; i--) // 从之前的top index向前计算，到0
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

            if (PNULL == item_ptr)
            {
                break;
            }

            // 判断item top是否 小于控件的top
            if (offset_y + item_ptr->item_top <= list_rect.top
                && offset_y + item_ptr->item_top + item_ptr->item_height - 1 >= list_rect.top)
            {
                list_ctrl_ptr->top_item_idx -= item_num;
                break;
            }

            item_num++;
        }
    }
    else // upwards
    {
        // 计算top index
        for (i = pre_top_item_idx; i < total_item_num; i++) // 从之前的top index向后计算，到total值
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

            if (PNULL == item_ptr)
            {
                break;
            }

            // 判断item bottom值是否大于控件的top
            if (offset_y + item_ptr->item_top + item_ptr->item_height - 1 >= list_rect.top
                && offset_y + item_ptr->item_top <= list_rect.top)
            {
                list_ctrl_ptr->top_item_idx += item_num;
                break;
            }

            item_num++;

        }
    }

    // 计算end index
    list_ctrl_ptr->end_item_idx = LIST_CalcEndItemByTopIndex(list_ctrl_ptr, list_ctrl_ptr->top_item_idx);

    return;
}

/*******************s**********************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetItemPostion(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
)
{
    int32 i = 0;
    int32 offset_y = 0;
    int32 total_item_height = 0;
    GUI_RECT_T list_rect = {0};
    CTRLLIST_ITEM_INFO_T  *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (!list_ctrl_ptr->is_pos_dirty)
    {
        return;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    offset_y = list_rect.top;

    if (list_ctrl_ptr->is_full_dirty)
    {
        LIST_ResetAllItemDisp(list_ctrl_ptr);
    }

    for (i = 0; i < list_ctrl_ptr->total_item_num; i++)
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

        if (PNULL == item_ptr)
        {
            break;
        }

        if (item_ptr->item.item_group > 0 && LIST_ItemIsGroupHead(list_ctrl_ptr, i))
        {
            total_item_height += LIST_GROUP_SPACE;
        }

        item_ptr->item_top = offset_y + total_item_height;
        total_item_height += item_ptr->item_height;
    }

    list_ctrl_ptr->total_item_height = total_item_height;
    
    list_ctrl_ptr->is_pos_dirty = FALSE;
    
    return;
}

/*****************************************************************************/
//  Description : reset offset y by end item index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetOffsetYByEndIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    int32 total_height
)
{
    int32  offset_y = 0;
    int32  total_item_height = total_height;
    int32  total_rect_height = 0;
    GUI_RECT_T list_rect = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    total_rect_height = list_rect.bottom - list_rect.top + 1;

    if (total_item_height >= total_rect_height) // 当高度大于区域的高度时
    {
        offset_y = (int32)total_rect_height - (int32)total_item_height + (int32)list_rect.top;
    }
    else
    {
        offset_y = list_rect.top;
    }

    list_ctrl_ptr->offset_y = offset_y;

    return;
}

/*****************************************************************************/
//  Description : reset offset y by start item index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetOffsetYByTopIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    uint16 item_index
)
{
    uint16  i = 0;
    uint32 total_item_height = 0;
    GUI_RECT_T list_rect = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (0 == list_ctrl_ptr->total_item_num || item_index >= list_ctrl_ptr->total_item_num)
    {
        total_item_height = 0;
    }
    else
    {
        // 计算总体高度
        for (i = 0; i < item_index; i++)
        {
            total_item_height += LIST_GetItemHeight(list_ctrl_ptr, i);
        }
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    list_ctrl_ptr->offset_y = list_rect.top - (int32)total_item_height; /*lint !e737*/

    return;
}

/*****************************************************************************/
//  Description : reset offset y by start item index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetOffsetYByTopIndexExt(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    uint16 item_index
)
{
    uint32 total_item_height = 0;
    GUI_RECT_T list_rect = {0};
    CTRLLIST_ITEM_INFO_T  *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    if (0 == list_ctrl_ptr->total_item_num || item_index >= list_ctrl_ptr->total_item_num)
    {
        total_item_height = 0;
        list_ctrl_ptr->offset_y = list_rect.top;
    }
    else
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

        if (PNULL != item_ptr)
        {
            // 偏移量等于起始坐标减去总高度
            list_ctrl_ptr->offset_y = list_rect.top - (item_ptr->item_top - list_rect.top);
        }
    }

    return;
}

/*****************************************************************************/
//  Description : get item index
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetItemIndexByPoint(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // the list control pointer
    GUI_POINT_T    *point_ptr
)
{
    uint16 i = 0;
    uint16 item_index = INVALID_ITEM_INDEX;
    GUI_RECT_T item_rect = {0};

    if ((PNULL == list_ctrl_ptr) || (PNULL == point_ptr))
    {
        return item_index;
    }

    for (i = 0; i < list_ctrl_ptr->total_item_num; i++)
    {
        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, i);

        if (GUI_PointIsInRect(*point_ptr, item_rect))
        {
            item_index = i;
            break;
        }
    }

    return item_index;
}


/*****************************************************************************/
//  Description : handle the tp move message
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_HandleItemTpMove(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
)
{
    BOOLEAN result = FALSE;
    uint16 item_index = 0;

    item_index = LIST_GetItemIndexByPoint(list_ctrl_ptr, point_ptr);

    if (item_index != INVALID_ITEM_INDEX &&
        MMK_TP_SLIDE_NONE == list_ctrl_ptr->tpdown_slide_state &&
        MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state &&
        !LIST_GetItemStateByIndex(list_ctrl_ptr, item_index, (GUIITEM_STATE_SPLIT)) &&
        !LIST_GetItemStateByIndex(list_ctrl_ptr, item_index, (GUIITEM_STATE_GRAY)) &&
        !LIST_GetItemStateByIndex(list_ctrl_ptr, item_index, (GUIITEM_STATE_INVALID)) &&
        !LIST_GetItemStateByIndex(list_ctrl_ptr, item_index, (GUIITEM_STATE_NOTHOR_SLIDE)) &&
        (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SLIDE_ITEM) || LIST_GetTotalHeight(list_ctrl_ptr, point_ptr)))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : ListDrawItemAnim
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawItemAnim(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    int32 start_x, // x起始点
    int32 end_x, // x终点
    GUI_RECT_T item_rect,
    uint32 sleep_time
)
{
    int32       i           = 0;
    int32       frame_num   = MMK_RECOVER_SLIDE_TOTAL_FRAME;
    int32       offset      = 0;
    int32       offset_x    = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    offset_x = end_x - start_x;

    if (PNULL == list_ctrl_ptr || 0 == offset_x)
    {
        return;
    }

    lcd_dev_info = LIST_GetItemDispLayer();

    for (i = MMK_RECOVER_SLIDE_START_FRAME; i <= frame_num; i++)
    {
        offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, offset_x, frame_num, i);

        UILAYER_SetLayerPosition(&lcd_dev_info, start_x + offset, item_rect.top);

        GUILCD_InvalidateRect(MAIN_LCD_ID, item_rect, 0);

        SCI_Sleep(sleep_time);
    }

    return;
}

/*****************************************************************************/
//  Description : display icon bg or color
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawItemIconBg(
    GUI_RECT_T        *rect_ptr,      //in
    CTRLLIST_OBJ_T    *list_ctrl_ptr,      //in
    const GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in
)
{
    uint16      default_img_width = 0;
    uint16      default_img_height = 0;
    uint32      default_img_id = 0;
    GUI_RECT_T  image_rect = {0};
    GUI_RECT_T  center_rect = {0};
    GUI_RECT_T  clip_rect = {0};

    if ((PNULL == list_ctrl_ptr) || (PNULL == rect_ptr) || (PNULL == lcd_dev_ptr))
    {
        return;
    }

    if (!GUI_IntersectRect(&clip_rect, *rect_ptr, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect))
    {
        return;
    }

    if (0 != list_ctrl_ptr->display_style.icon_default_id)
    {
        default_img_id = list_ctrl_ptr->display_style.icon_default_id;

        //get default image width and height
        GUIRES_GetImgWidthHeight(&default_img_width, &default_img_height, default_img_id, list_ctrl_ptr->win_id);

        //set image rect
        image_rect.left   = 0;
        image_rect.top    = 0;
        image_rect.right  = MIN((int16)(rect_ptr->right - rect_ptr->left + 1), default_img_width - 1);
        image_rect.bottom = MIN((int16)(rect_ptr->bottom - rect_ptr->top + 1), default_img_height - 1);

        // 获得中心区域，居中显示
        center_rect = GUI_GetCenterRectExt(*rect_ptr, default_img_width, default_img_height);

        if (!GUI_IntersectRect(&clip_rect, center_rect, clip_rect))
        {
            return;
        }

        if (clip_rect.top > center_rect.top)
        {
            image_rect.top += clip_rect.top - center_rect.top;
        }

        if (clip_rect.bottom < center_rect.bottom)
        {
            image_rect.bottom += clip_rect.bottom - center_rect.bottom;
        }

        //display image
        IMG_EnableTransparentColor(TRUE);
        GUIRES_DisplayImg(PNULL,
                          &clip_rect,
                          &image_rect,
                          list_ctrl_ptr->win_id,
                          default_img_id,
                          lcd_dev_ptr);
        IMG_EnableTransparentColor(FALSE);
    }

    return;
}

/*****************************************************************************/
//  Description : move to next item, 有滑动效果
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_CalculateMoveItemNum(
    CTRLLIST_OBJ_T *list_ctrl_ptr,            // 控件指针
    CTRLLIST_DIRECTION_KEY_E direct_type  // 移动方向
)
{
    uint16 move_num = 0;
    uint16 cur_item_index = 0;

    if (PNULL == list_ctrl_ptr || list_ctrl_ptr->total_item_num == 0)
    {
        return 0;
    }

    cur_item_index = list_ctrl_ptr->cur_item_idx;

    switch (direct_type)
    {
        case CTRLLIST_DIRECTION_KEY_DOWN:
            do
            {
                cur_item_index++;
                cur_item_index = cur_item_index % list_ctrl_ptr->total_item_num;
                move_num++;

                if (LIST_IsItemGetFocus(list_ctrl_ptr, cur_item_index))
                {
                    break;
                }
            }
            while (cur_item_index != list_ctrl_ptr->cur_item_idx);

            break;

        case CTRLLIST_DIRECTION_KEY_UP:
            do
            {
                if (0 == cur_item_index)
                {
                    cur_item_index = list_ctrl_ptr->total_item_num - 1;
                }
                else
                {
                    cur_item_index--;
                }

                move_num++;

                if (LIST_IsItemGetFocus(list_ctrl_ptr, cur_item_index))
                {
                    break;
                }
            }
            while (cur_item_index != list_ctrl_ptr->cur_item_idx);

            break;

        default:
            break;
    }

    return move_num;
}

/*****************************************************************************/
//  Description : 将当前的索引调整到有效的索引
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ChangeCurItemToValidItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr // 控件指针
)
{
    uint16 cur_item_index = 0;

    if (PNULL == list_ctrl_ptr || list_ctrl_ptr->total_item_num == 0)
    {
        return;
    }

    cur_item_index = list_ctrl_ptr->cur_item_idx;

    do
    {
        if (LIST_IsItemGetFocus(list_ctrl_ptr, cur_item_index))
        {
            list_ctrl_ptr->cur_item_idx = cur_item_index;
            break;
        }

        cur_item_index++;
    }
    while (cur_item_index < list_ctrl_ptr->total_item_num);

    return;
}

/*****************************************************************************/
//  Description : get the item height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC GUIITEM_THEME_STATE_E LIST_GetItemThemeState(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const CTRLLIST_ITEM_INFO_T *item_ptr,
    BOOLEAN             is_highlight
)
{
    GUIITEM_THEME_STATE_E state = GUIITEM_THEME_STATE_DEFAULT;

    if (is_highlight
        && !list_ctrl_ptr->is_disable_3dmmi
        && LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
    {
        if (LIST_GetItemState(item_ptr, GUIITEM_STATE_INVALID))
        {
            state = GUIITEM_THEME_STATE_FOCUS_INVALID;
        }
        else
        {
            state = GUIITEM_THEME_STATE_FOCUS;
        }
    }
    else
    {
        if (LIST_GetItemState(item_ptr, GUIITEM_STATE_INVALID))
        {
            state = GUIITEM_THEME_STATE_INVALID;
        }
    }

    if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ACTIVE))
    {
        state = GUIITEM_THEME_STATE_DEFAULT;
    }

    return state;
}

/*****************************************************************************/
//  Description : 获取列表控件文本的style
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_GetItemContentEffectStr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUISTR_STYLE_T *str_style_ptr,
    uint16 item_index,
    uint16 content_index
)
{
    GUILIST_EFFECT_STR_T    effect_str = {0};
    CTRLLIST_ITEM_INFO_T     *item_ptr    = PNULL;
    THEMELIST_ITEM_STYLE_T   *item_style_ptr = PNULL;
    MMI_STRING_T            text_info = {0};
    GUIFONT_REGION_STYLE_T *region_ptr = PNULL;

    SCI_ASSERT(PNULL != list_ctrl_ptr);   /*assert verified*/

    if (!CTRLLIST_GetListState(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, GUILIST_STATE_EFFECT_STR))
    {
        return;
    }

    item_ptr =  LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

    if (PNULL == item_ptr)
    {
        return;
    }

    if (MMITHEME_COLOR_NONE != item_ptr->item.data_ptr->item_content[content_index].font_color_id)
    {
        str_style_ptr->font_color = MMITHEME_GetItemContentColor(item_ptr->item.data_ptr->item_content[content_index].font_color_id);
    }

    item_style_ptr = item_ptr->item.style_ptr;

    if (item_style_ptr->content[content_index].state & GUIITEM_CONTENT_STATE_EFFECT_STR)
    {
        effect_str.content_index = content_index;
        effect_str.item_index = item_index;

        MMK_SendMsg(list_ctrl_ptr->win_id, MSG_NOTIFY_LIST_GET_EFFECT_STR, (DPARAM)&effect_str);

        switch (effect_str.effect_type)
        {
            case GUILIST_EFFECT_STR:
                LIST_GetListItemContentTextInfo(list_ctrl_ptr, item_ptr, content_index, &text_info);

                if ((PNULL != effect_str.str_info.wstr_ptr && 0 != effect_str.str_info.wstr_len)
                    && (PNULL != text_info.wstr_ptr && 0 != text_info.wstr_len)
                    && effect_str.str_info.wstr_len <= text_info.wstr_len)
                {
                    wchar *tmp1_ptr = effect_str.str_info.wstr_ptr;
                    wchar *tmp2_ptr = text_info.wstr_ptr;
                    wchar *tmp3_ptr = PNULL;
                    int16 start_index = 0;
                    int16 count = effect_str.str_info.wstr_len;
                    BOOLEAN is_found = FALSE;
                    int16 ret = 0;

                    while (!is_found && '\0' != *tmp2_ptr)
                    {
                        if (*tmp2_ptr == *tmp1_ptr)
                        {
                            ret = 0;

                            tmp3_ptr = tmp2_ptr;

                            while (count--)
                            {
                                if (*tmp1_ptr != *tmp3_ptr)
                                {
                                    ret = 1;
                                    break;
                                }

                                if (*tmp1_ptr++ < *tmp3_ptr++)
                                {
                                    ret = -1;
                                    break;
                                }
                            }

                            if (0 == ret)
                            {
                                is_found = TRUE;
                                break;
                            }
                        }

                        count = effect_str.str_info.wstr_len;
                        tmp1_ptr = effect_str.str_info.wstr_ptr;

                        ++start_index;
                        ++tmp2_ptr;
                    }

                    if (is_found)
                    {
                        region_ptr = SCI_ALLOCA(sizeof(GUIFONT_REGION_STYLE_T));

                        if (PNULL != region_ptr)
                        {
                            SCI_MEMSET(region_ptr, 0, sizeof(GUIFONT_REGION_STYLE_T));
                            region_ptr->font_color = effect_str.font_color;
                            region_ptr->state = effect_str.state;
                            region_ptr->length = effect_str.str_info.wstr_len;
                            region_ptr->start = start_index;

                            str_style_ptr->region_ptr = region_ptr;
                            str_style_ptr->region_num = 1;
                        }
                        else
                        {
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_3392_112_2_18_3_19_48_208, (uint8 *)"");
                        }
                    }
                }

                break;

            case GUILIST_EFFECT_REGION:
                str_style_ptr->region_ptr = effect_str.region_ptr;
                str_style_ptr->region_num = effect_str.region_num;
                break;

            default:
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_3402_112_2_18_3_19_48_209, (uint8 *)"");
                break;
        }
    }

    return;
}

/*****************************************************************************/
//  Description : 获取列表控件文本的style
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T LIST_GetItemContentTextColor(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index,
    uint16 content_index
)
{
    BOOLEAN is_hightlight = FALSE;
    GUIITEM_THEME_STATE_E theme_state = GUIITEM_THEME_STATE_DEFAULT;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

    if (PNULL == item_ptr)
    {
        return 0;
    }

    is_hightlight = ((item_index == list_ctrl_ptr->cur_item_idx) && !LIST_GetItemState(item_ptr, GUIITEM_STATE_SPLIT));

    theme_state = LIST_GetItemThemeState(list_ctrl_ptr, item_ptr, is_hightlight);

    return THEMELIST_GetContentTextColor(theme_state, item_ptr->item.style_ptr, content_index);
}

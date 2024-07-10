/*****************************************************************************
** File Name:      ctrlstatusbar_data.c                                       *
** Author:                                                                   *
** Date:           12/23/2004                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012     xiyuan.ma       Create                                   *
******************************************************************************/
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmk_timer.h"
#include "mmk_msg.h"
#include "mmi_gui_trc.h"
#include "guilcd.h"
#include "guicommon.h"

#include "ctrlstatusbar_data.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
// item的状态，对CTRLSTBDATA_ITEM_STATE_T状态的定义；
#define CTRLSTBDATA_ITEM_STATE_DISABLE       0x00000001 // item 不能显示
#define CTRLSTBDATA_ITEM_STATE_ISVISIBLE     0x00000002 // item 可见
#define CTRLSTBDATA_ITEM_STATE_RIGHT_ALIGN   0x00000004 // item 靠右侧显示
#define CTRLSTBDATA_ITEM_STATE_CENTER_ALIGN  0x00000008 // item 居  中显示
#define CTRLSTBDATA_ITEM_STATE_VALID  0x00000010 // item 是否使用


/*---------------------------------------------------------------------------*/
/*                          LOCAL STRUCT DEFINITION                          */
/*---------------------------------------------------------------------------*/
typedef struct
{
    uint32                  item_max_num;    // the max num of items
    uint32                  item_total_num;  // the total num of items
    GUISTBDATA_ITEM_DATA_T  *item_data_ptr;  // item data ptr
} CTRLSTBDATA_ITEM_T;



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
LOCAL CTRLSTBDATA_ITEM_T s_item_info = {0};



/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : insert a new item data
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertItemData (
    CTRLSTBDATA_ITEM_T            *item_ptr,
    const GUISTBDATA_ITEM_DATA_T *item_data_ptr,
    uint32                       pos // 指定的位置, from 0 to item_total_num
);


/*****************************************************************************/
//  Description : get item data ptr
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUISTBDATA_ITEM_DATA_T *GetItemDataPtr (
    uint32 item_index
);


/*****************************************************************************/
//  Description : set the tab state
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void SetState (
    uint32* dst_state_ptr,
    uint32  src_state,
    BOOLEAN is_true
);


/*****************************************************************************/
//  Description : get state
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetState (
    uint32 dst_state,
    uint32 src_state
);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : insert a new item data
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertItemData (
    CTRLSTBDATA_ITEM_T            *item_ptr,
    const GUISTBDATA_ITEM_DATA_T *item_data_ptr,
    uint32                       pos // 指定的位置, from 0 to item_total_num
)
{
    BOOLEAN             result        = FALSE;
    GUISTBDATA_ITEM_DATA_T *tmp_item_data_ptr = PNULL;

    if (PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"InsertItemData PNULL == item_ptr!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_DATA_117_112_2_18_3_21_38_343, (uint8*) "");
        return result;
    }

    if (PNULL == item_data_ptr)
    {
        //SCI_TRACE_LOW:"InsertItemData PNULL == item_data_ptr!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_DATA_123_112_2_18_3_21_38_344, (uint8*) "");
        return result;
    }

    if (item_ptr->item_total_num < item_ptr->item_max_num)
    {
        pos = (uint32) MIN (pos, item_ptr->item_total_num);

        /* memmove */
        if (item_ptr->item_total_num - pos > 0)
        {
            memmove (item_ptr->item_data_ptr + pos + 1, \
                     item_ptr->item_data_ptr + pos, \
                     (item_ptr->item_total_num - pos) * sizeof (GUISTBDATA_ITEM_DATA_T));
        }

        tmp_item_data_ptr = item_ptr->item_data_ptr + pos;
        MMI_MEMCPY (tmp_item_data_ptr, sizeof (GUISTBDATA_ITEM_DATA_T), item_data_ptr, sizeof (GUISTBDATA_ITEM_DATA_T), sizeof (GUISTBDATA_ITEM_DATA_T));

        item_ptr->item_total_num ++;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get item data ptr
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUISTBDATA_ITEM_DATA_T *GetItemDataPtr (
    uint32 item_index
)
{
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    if (item_index < s_item_info.item_max_num)
    {
        item_data_ptr = s_item_info.item_data_ptr + item_index;
    }

    return (item_data_ptr);
}

/*****************************************************************************/
//  Description : set the tab state
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void SetState (
    uint32* dst_state_ptr,
    uint32  src_state,
    BOOLEAN is_true
)
{
    uint32 state = 0;

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
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetState (
    uint32 dst_state,
    uint32 src_state
)
{
    return (BOOLEAN) ( (dst_state & src_state) == src_state);
}



/*****************************************************************************/
//  Description : initial the item data of status bar
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note: 该接口只在app初始化时调用，不能重复调用多次，否则内存泄漏。
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_InitData (
    uint32 item_max_num
)
{
    BOOLEAN result = FALSE;

    s_item_info.item_data_ptr = SCI_ALLOC_CONST (item_max_num * sizeof (GUISTBDATA_ITEM_DATA_T));

    if (PNULL != s_item_info.item_data_ptr)
    {
        SCI_MEMSET (s_item_info.item_data_ptr, 0, (item_max_num*sizeof (GUISTBDATA_ITEM_DATA_T)));
        s_item_info.item_max_num = item_max_num;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Append item
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_AppendItemData (
    GUISTBDATA_ITEM_DATA_T *item_data_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL != item_data_ptr)
    {
        result = InsertItemData (&s_item_info, item_data_ptr, s_item_info.item_total_num);
    }

    return result;
}

/*****************************************************************************/
//  Description : set item is visible or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemVisible (
    uint32   item_index,
    BOOLEAN  is_visible
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        SetState (&item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_ISVISIBLE, is_visible);
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set item is visible or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemVisible (
    uint32   item_index
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        result = GetState (item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_ISVISIBLE);
    }

    return result;
}

/***************************************************************************//*!
@brief 设置item是否有效
@author xiaoqing.lu
@param item_index [in] 索引号
@param is_visible [in] 是否有效
@retval true/false
@return true/false
@note 内部接口
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemValid (
    uint32   item_index,
    BOOLEAN  is_valid
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        SetState (&item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_VALID, is_valid);
        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
//  Description : set item is visible or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemValid (
    uint32   item_index
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        result = GetState (item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_VALID);
    }

    return result;
}

/*****************************************************************************/
//  Description : set item is visible or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemDisable (
    uint32   item_index,
    BOOLEAN  is_disable
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        SetState (&item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_DISABLE, is_disable);
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set item is visible or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemDisable (
    uint32   item_index
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        result = GetState (item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_DISABLE);
    }

    return result;
}

/*****************************************************************************/
//  Description : set item handle function is valid or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemHandleFunc (
    uint32   item_index,
    GUISTBDATA_APP_HANDLE_FUNC app_handle_func
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        item_data_ptr->app_handle_func = app_handle_func;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set item handle function is valid or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemHandleFuncValid (
    uint32  item_index,
    BOOLEAN is_valid
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        item_data_ptr->is_handle_callback = is_valid;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get item handle function is valid or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_ItemIsHandleFuncValid (
    uint32 item_index
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        result = item_data_ptr->is_handle_callback;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIcon (
    uint32            item_index,
    const GUISTBDATA_ICON_T *icon_info_ptr
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr && PNULL != icon_info_ptr)
    {
        MMI_MEMCPY (&item_data_ptr->icon_info, sizeof (GUISTBDATA_ICON_T), icon_info_ptr, sizeof (GUISTBDATA_ICON_T), sizeof (GUISTBDATA_ICON_T));
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconStyle (
    uint32            item_index,
    GUISTBDATA_ICON_STYLE_E icon_style
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        item_data_ptr->icon_info.icon_type = icon_style;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIconStyle (
    uint32            item_index,
    GUISTBDATA_ICON_STYLE_E *icon_style_ptr
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr && PNULL != icon_style_ptr)
    {
        *icon_style_ptr = item_data_ptr->icon_info.icon_type;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconId (
    uint32         item_index,
    MMI_IMAGE_ID_T icon_id
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        item_data_ptr->icon_info.icon_array[0] = icon_id;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIcon (
    uint32            item_index,
    GUISTBDATA_ICON_T *icon_info_ptr
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr && PNULL != icon_info_ptr)
    {
        MMI_MEMCPY (icon_info_ptr, sizeof (GUISTBDATA_ICON_T), &item_data_ptr->icon_info, sizeof (GUISTBDATA_ICON_T), sizeof (GUISTBDATA_ICON_T));
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconCurFrame (
    uint32 item_index,
    uint32 cur_frame
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        if (0 != item_data_ptr->icon_info.icon_num)
        {
            item_data_ptr->cur_frame = cur_frame % item_data_ptr->icon_info.icon_num;
        }
        else
        {
            item_data_ptr->cur_frame = 0;
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIconNextFrame (
    uint32 item_index,
    uint32 *next_frame_ptr
)
{
    BOOLEAN result = FALSE;
    BOOLEAN next_frame = 0;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != next_frame_ptr && PNULL != item_data_ptr)
    {
        item_data_ptr->cur_frame++;

        if (0 != item_data_ptr->icon_info.icon_num)
        {
            item_data_ptr->cur_frame = item_data_ptr->cur_frame % item_data_ptr->icon_info.icon_num;
        }
        else
        {
            item_data_ptr->cur_frame = 0;
        }

        next_frame = item_data_ptr->cur_frame;

        *next_frame_ptr = next_frame;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIconCurFrame (
    uint32 item_index,
    uint32 *cur_frame_ptr
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr && PNULL != cur_frame_ptr)
    {
        *cur_frame_ptr = item_data_ptr->cur_frame;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconValid (
    uint32 item_index,
    BOOLEAN is_valid
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        item_data_ptr->icon_info.is_display = is_valid;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemIconValid (
    uint32 item_index
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        result = item_data_ptr->icon_info.is_display;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemText (
    uint32            item_index,
    const GUISTBDATA_TEXT_T *text_info_ptr
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != text_info_ptr && PNULL != item_data_ptr)
    {
        MMI_MEMCPY (&item_data_ptr->text_info, sizeof (GUISTBDATA_TEXT_T), text_info_ptr, sizeof (GUISTBDATA_TEXT_T), sizeof (GUISTBDATA_TEXT_T));
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemText (
    uint32            item_index,
    GUISTBDATA_TEXT_T *text_info_ptr
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr && PNULL != text_info_ptr)
    {
        MMI_MEMCPY (text_info_ptr, sizeof (GUISTBDATA_TEXT_T), &item_data_ptr->text_info, sizeof (GUISTBDATA_TEXT_T), sizeof (GUISTBDATA_TEXT_T));
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemTextValid (
    uint32 item_index,
    BOOLEAN is_valid
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        item_data_ptr->text_info.is_display = is_valid;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemTextValid (
    uint32 item_index
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        result = item_data_ptr->text_info.is_display;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemRect (
    uint32     item_index,
    GUI_RECT_T rect
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        MMI_MEMCPY (&item_data_ptr->item_rect, sizeof (GUI_RECT_T), &rect, sizeof (GUI_RECT_T), sizeof (GUI_RECT_T));
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLSTBDATA_GetItemRect (
    uint32     item_index
)
{
    GUI_RECT_T rect = {0};
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        rect = item_data_ptr->item_rect;
    }

    return rect;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLSTBDATA_GetItemTotalNum (void)
{
    return s_item_info.item_total_num;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUISTBDATA_ITEM_DATA_T *CTRLSTBDATA_GetItemData (
    uint32 item_index
)
{
    return GetItemDataPtr (item_index);
}

/*****************************************************************************/
//  Description : set item is displayed by right side
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 如果不是靠右显示，则肯定是靠左显示，没有第三种情况
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemRightAlign (
    uint32   item_index,
    BOOLEAN  is_right_align
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        SetState (&item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_RIGHT_ALIGN, is_right_align);
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set istyle item is displayed by which side
//  Global resource dependence :
//  Author:xiyuan.ma
//  Note: iphone风格下，item居中
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemCenterAlign (
    uint32   item_index,
    BOOLEAN  is_center_align
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        SetState (&item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_CENTER_ALIGN, is_center_align);
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : judge item is displayed by right side or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 如果不是靠右显示，则肯定是靠左显示，没有第三种情况
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemRightAlign (
    uint32   item_index
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        result = GetState (item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_RIGHT_ALIGN);
    }

    return result;
}


/*****************************************************************************/
//  Description : judge item is displayed by right side、left side orcenter
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: iphone风格下的状态栏居中
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemCenterAlign (
    uint32   item_index
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        result = GetState (item_data_ptr->item_state, CTRLSTBDATA_ITEM_STATE_CENTER_ALIGN);
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTABATA_SetItemBgInter (
    uint32            item_index,
    GUI_BG_T          *bg_info_ptr
)
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL != item_data_ptr)
    {
        MMI_MEMCPY (&item_data_ptr->bg_info, sizeof (GUI_BG_T), bg_info_ptr, sizeof (GUI_BG_T), sizeof (GUI_BG_T));
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set STATUSBAR owner draw callback function
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTABATA_SetOwnerDraw (
    uint32                          item_index,
    GUISTATUSBAR_OWNER_DRAW_FUNC    func
)
{
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;

    item_data_ptr = GetItemDataPtr (item_index);

    if (PNULL == item_data_ptr)
    {
        return;
    }

    //set function
    item_data_ptr->owner_draw_func = func;
}



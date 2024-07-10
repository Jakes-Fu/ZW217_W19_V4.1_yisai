/****************************************************************************
** File Name:      ctrllistbox.c                                            *
** Author:         xiaoqing.lu                                              *
** Date:           24/07/2012                                               *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the listbox control.       *
*****************************************************************************
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 24/07/2012     xiaoqing.lu      Create
****************************************************************************/

#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif

#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
#define ADULTWATCHLIST_BOTTOM_OFFSET   DP2PX_VALUE(35)// BUG-1594143
#endif

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
#include "guiprgbox.h"
#include "guianim.h"
#include "guiblock.h"
#include "guilcd.h"
#ifndef WIN32
#include "arm_reg.h"
#include "chng_freq.h"
#endif
#include "mmk_msg.h"
#ifdef UI_P3D_SUPPORT
#include "ui_special_effect.h"
#endif
#include "guitips.h"
#include "guires.h"
#include "mmi_default.h"
#include "guistring.h"
#include "ui_layer.h"
#include "mmk_tp.h"
#include "mmitheme_special.h"
#include "mmitheme_softkey.h"
#include "mmitheme_prgbox.h"
#include "mmitheme_coverbar.h"
#include "ctrlform_export.h"
#include "ctrllettersbar_export.h"
#include "ctrlanim.h"
#include "guictrl_api.h"
#include "mmi_string.h"
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN SetListMiddleItemFocus(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    if(PNULL == list_ctrl_ptr)
    {
        SCI_TRACE_LOW("param is err!!");
    }
    list_ctrl_ptr->cur_item_idx = (list_ctrl_ptr->top_item_idx + list_ctrl_ptr->end_item_idx)/2;
    return TRUE;
}

LOCAL void __inline SetRectOffset(GUI_RECT_T *rect, int16 offset_x, int16 offset_y)
{
    if (PNULL != rect)
    {
        if ( MMI_DIRECTION_RIGHT != MMITHEME_GetDisDirection() )
        {
            rect->left += offset_x;
            rect->right += offset_x;
        }

        rect->top += offset_y;
        rect->bottom += offset_y;
    }

    return;
}

/**-------------------------------------------------------------------------*
**                         MICRO DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
#define GET_SOFTKEY_CONTENT(list_softkey_type, image_id, text_id) ((list_softkey_type)? (image_id): (text_id))
#else
#define GET_SOFTKEY_CONTENT(list_softkey_type, image_id, text_id) (text_id)
#endif

#define MMICOM_LIST_BG_COLOR          0x0000
#define MMICOM_LIST_FONT_COLOR      0xffff

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle tp msg, to exchange current item index
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListTpMoveItems(
    CTRLLIST_OBJ_T *list_ctrl_ptr, // 控件指针
    uint16         item_index // move to index
);

/*****************************************************************************/
//  Description : handle list scroll for touch panel
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpScroll(
    int32 top_item_index,
    CTRLLIST_OBJ_T     *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the tp down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpNext(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the tp up message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpPre(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the tp up message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpUp(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : handle the tp down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpDown(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : handle the tp move message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpMove(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : handle the tp up message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpLong(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : 处理滑动效果
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleSlide(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
);

/*****************************************************************************/
//  Description : Stop Fling
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListStopFling(
    CTRLLIST_OBJ_T   *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : start the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStartTpMoveTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : stop the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStopTpMoveTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : start the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStartTpTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    CONTINUE_MOVE_TYPE_E move_type
);

/*****************************************************************************/
//  Description : stop the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStopTpTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : item被点击时，返回item当前选中的content信息
//  Global resource dependence :
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void ListHandleSendItemContentClick(
                                                        CTRLLIST_OBJ_T   *list_ctrl_ptr,
                                                        GUI_POINT_T      *point_ptr
                                                     );

#endif

/*****************************************************************************/
//  Description :设置list不可见，同时删除list的层
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetVisible(
    CTRLBASE_OBJ_T    *obj_ptr,  //in:
    BOOLEAN       is_visible //in/out:
);

/*****************************************************************************/
//  Description : init list class
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListCtrlInitVtbl(
    CTRLLIST_VTBL_T *list_vtbl_ptr
);

/*****************************************************************************/
//  Description : pack list init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListCtrlPackInitParam(
    GUILIST_INIT_DATA_T     *list_init_ptr,
    CTRLLIST_INIT_PARAM_T   *list_param_ptr
);

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListStartSwitchTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : stop the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStopSwitchTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : select item
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetSelectedItemByPtr(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    uint16           item_pos,   //from 0 to total_item_num-1
    BOOLEAN          is_selected //TRUE : selected; FALSE: unselected
);

/*****************************************************************************/
//  Description : alloc all items memory of the list control
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListAllocAllItemsSpace(
    CTRLLIST_OBJ_T *list_ctrl_ptr  // the list control pointer
);

/*****************************************************************************/
//  Description : reset the item pointer of list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListResetItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr  // the list control pointer
);

/*****************************************************************************/
//  Description : insert item into list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListInsertItem(
    CTRLLIST_OBJ_T       *list_ctrl_ptr,  // the list control pointer
    const CTRLLIST_ITEM_T *add_item_ptr, // 指定的Item
    uint16                index           // 指定的位置, from 0 to total_item_num
);

/*****************************************************************************/
//  Description : Replace the item in position
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListReplaceItem(
    CTRLLIST_OBJ_T       *list_ctrl_ptr,
    const CTRLLIST_ITEM_T *replace_item_ptr,
    uint16                index
);

/*****************************************************************************/
//  Description : Adjust item list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListValidateCtrl(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    BOOLEAN         is_cur_visible,
    BOOLEAN         is_must_set_offset
);

/*****************************************************************************/
//  Description : draw one item of the list control
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr,   //on witch block
    uint16 index
);

/*****************************************************************************/
//  Description : draw the current list item when timer expired
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListUpdateCurItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get the item num by height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 ListGetItemTotalHeightByIdx(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16              start_index,
    uint16              end_index
);

/*****************************************************************************/
//  Description : move to prev item
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListToPrevItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : 指针转换到next item
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListToNextItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the key down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyDown(
    CTRLLIST_OBJ_T     *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the key up message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyUp(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the key ok message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyOK(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    BOOLEAN         *is_need_notify //OUT
);

/*****************************************************************************/
//  Description : handle the key right  message
//  Global resource dependence :
//  Author:kelly.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyRight(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the key left message
//  Global resource dependence :
//  Author:kelly.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyLeft(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the key ok message of the radio list
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListRadioHandleOK(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the key ok message of the check list
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListCheckHandleOK(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : post list slide state
//  Global resource dependence :
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void ListPostSlideStateMsg(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUILIST_SLIDE_STATE_E slide_state
);

/*****************************************************************************/
//  Description : post mark all
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListSendMarkAllItemMsg(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN               is_selected
);

/*****************************************************************************/
//  Description : move to next page item
//  Global resource dependence :
//  Author:kelly.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListMoveToNextPageItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN         is_next
);

/*****************************************************************************/
//  Description : Get selected items of check & radio  list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint16 ListGetSelectedItem( //return the selected item num
    const CTRLLIST_OBJ_T   *list_ctrl_ptr,         //in:list pointer
    uint16                 *idx_ptr,           //in/out:index array,may PNULL
    CTRLLIST_ITEM_INFO_T   **selected_item_ptr,//in/out:selected item array,may PNULL
    uint16                 array_len           //in:array length
);

/*****************************************************************************/
//  Description : Stop text animation
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListStopTextAnim(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : Is text too long
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemTextNeedScroll(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const CTRLLIST_ITEM_INFO_T *item_ptr
);

/*****************************************************************************/
//  Description : Get offset between current item index and top item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL  uint16 ListGetOffsetofTowItem(
    uint16 start_idx,
    uint16 end_idx,
    uint16 total_num
);

/*****************************************************************************/
//  Description : add selected item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListUpdateSeletedState(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_ptr
);

/*****************************************************************************/
//  Description : reduce selected item num
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListReduceSelectedItemNum(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_ptr
);

/*****************************************************************************/
//  Description : Get list pointer from control id
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL CTRLLIST_OBJ_T *ListGetPtr(
    MMI_HANDLE_T ctrl_handle
);

/*****************************************************************************/
//  Description : Draw Sting
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListDrawString(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO *dev_info_ptr,
    const wchar *wstr_ptr,
    uint16 wstr_len,
    GUI_RECT_T *disp_rect_ptr,
    GUI_RECT_T *clip_rect_ptr,
    uint16 item_index,
    uint16 content_index,
    BOOLEAN is_single_line
);

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T ListGetScrollBarRect(
    const CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get scroll bar both rect
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T ListGetScrollBarBothRect(
    const CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : initial scroll bar
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListInitScrollBar(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : sat param
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetScrollBarParam(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : Draw scroll bar if need
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListDrawTitle(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN is_draw_all
);

/*****************************************************************************/
//  Description : Draw scroll bar if need
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListDrawSoftkey(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN             is_draw_all
);

/*****************************************************************************/
//  Description : draw highlight item background
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListDrawHighlightBackground(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *rect_ptr,
    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
);

/*****************************************************************************/
//  Description : draw list background
//  Global resource dependence :
//  Author: Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawBackground(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : draw item background
//  Global resource dependence :
//  Author: Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawBackgroundByRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *rect_ptr
);

/*****************************************************************************/
//  Description : redraw item
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListRedrawItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          index
);

/*****************************************************************************/
//  Description : 画softkey 和title
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawScrollBar(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN             is_draw_all
);

/*****************************************************************************/
//  Description : display list box info
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListDrawTitleSubIndex(const CTRLLIST_OBJ_T *list_ctrl_ptr);

/*****************************************************************************/
//  Description : send msg to win, notify it append the next item content
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListNotifyAppendItemContent(
    CTRLLIST_OBJ_T   *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : remove the appointed item base the position
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListRemoveItem(
    CTRLLIST_OBJ_T    *list_ctrl_ptr,
    uint16            index // 指定的Position, from 0 to total_item_num-1
);

/*****************************************************************************/
//  Description : Remove all items
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListRemoveAllItems(
    CTRLLIST_OBJ_T    *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : 显示高亮条，在OSD层显示
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawHighlightItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : 恢复item text滚动的timer
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListResumeItemTextTimer(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : move to next item
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListMoveToNextItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,            // 控件指针
    CTRLLIST_DIRECTION_KEY_E direct_type, // 移动方向
    BOOLEAN is_support_3d,               // 是否3d
    BOOLEAN is_tp_move_item,             // 是否tp移动
    uint16 item_num                      // 移动数
);

/*****************************************************************************/
//  Description : draw the list box directly
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawAllItems(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
);

/*****************************************************************************/
//  Description : draw split item bg
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawSplitStyleItemBg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T  item_rect,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr
);

/*****************************************************************************/
//  Description : draw head split item bg
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawSplitBar(CTRLLIST_OBJ_T *list_ctrl_ptr);

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawItemSplitLine(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         item_index,
    GUI_RECT_T item_rect,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr
);

/*****************************************************************************/
//  Description : 画全屏的list
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListDraw(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    INNER_UPDATE_STATE_T inner_state
);

/*****************************************************************************/
//  Description : 刷新屏幕
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListUpdateHighlightItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : 设置item标记状态
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetItemMarkStatus(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_ptr,
    uint16              item_idx
);

/*****************************************************************************/
//  Description : construct
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListCtrlConstruct(
    CTRLBASE_OBJ_T          *obj_ptr,
    CTRLLIST_INIT_PARAM_T   *list_param_ptr
);

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListCtrlDestruct(
    CTRLBASE_OBJ_T *obj_ptr
);

/******************************************************************************/
/*! \brief get the mmi system pointer
*  \author Great.Tian
*  \param[in]   ctrl_ptr        current ctrl ptr
*  \param[in]   msg_id          msg type
*  \param[in]   param           process param
*  \return the result of process
*/
/******************************************************************************/
LOCAL MMI_RESULT_E  ListCtrlHandleMsg(
    CTRLBASE_OBJ_T       *ctrl_ptr,   //current ctrl ptr
    MMI_MESSAGE_ID_E  msg_id,     //msg type
    DPARAM            param       //process param
);

/*****************************************************************************/
//  Description : init function
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListCtrlInit(
    CTRLBASE_OBJ_T          *obj_ptr,
    CTRLLIST_INIT_PARAM_T   *list_param_ptr
);

/*****************************************************************************/
//  Description : list set rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetRect(
    CTRLBASE_OBJ_T *iguictrl_ptr,
    const GUI_RECT_T *rect_ptr
);

/*****************************************************************************/
//  Description : get control display rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetDisplayRect(
    CTRLBASE_OBJ_T   *iguictrl_ptr,  //in:
    GUI_RECT_T   *rect_ptr       //in/out:
);

/*****************************************************************************/
//  Description : set control display rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetDisplayRect(
    CTRLBASE_OBJ_T     *iguictrl_ptr,  //in:
    const GUI_RECT_T   *rect_ptr,      //in:
    BOOLEAN            is_update       //in:
);

/*****************************************************************************/
//  Description : 设置item标记状态
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetItemMarkByStatus(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_ptr,
    uint16         item_idx,
    BOOLEAN        is_selected
);

/*****************************************************************************/
//  Description : draw tips info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void  ListDrawTipsInfo(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : 设置索引号信息,显示在title
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetTitleIndexInfo(
    const CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : 获取列表控件文本的style
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListGetTextStyle(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUISTR_STYLE_T *str_style_ptr,
    uint16 item_index,
    uint16 content_index
);

/*******************************************************************************************/
//  Description : calculate the ucs2 string piexl number
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*********************************************************************************************/
LOCAL uint16 ListCalculateStringPiexlNum(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const wchar *str_ptr,    //[IN] String指针
    uint16       str_len,    //[IN] String长度(Byte长度)
    uint16 item_index,
    uint16 content_index
);

/*****************************************************************************/
//  Description : 发送图片解析结果给应用窗口
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSendAnimDispResultToWin(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

/*****************************************************************************/
//  Description : init icon display layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListResetOffsetY(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : create icon display layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListCreateItemLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListReleaseItemLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetBltItemLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    UILAYER_HANDLE_T   layer1
);

/*****************************************************************************/
//  Description : is item visible?
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsListItemVisible(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index
);

/*****************************************************************************/
//  Description : GetSecMenuItemDispLcdDevInfo
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO ListGetDispLayerInfo(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : Revice list display item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListReviceDrawItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    int32  *offset_y_ptr,
    BOOLEAN is_next
);

/*****************************************************************************/
//  Description : handle list timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListHandleTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    DPARAM           param          //附带的处理参数
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : ListDrawAnim
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListDrawAnim(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    int32 move_space_y
);

/*****************************************************************************/
//  Description : 处理list滚动条
//  Global resource dependence :
//  Author: Jibin
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListHandleScrollbarTPKeydown(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the tp key down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTPKeyUp(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T       point
);

/*****************************************************************************/
//  Description : handle the tp key down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListHandleTPKeydown(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T       point
);

/*****************************************************************************/
//  Description : 画全屏的list
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListMoveAllItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle the content check
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListItemContentCheck(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    uint16           item_index,
    CTRLLIST_ITEM_INFO_T *item_ptr,
    GUI_POINT_T tp_point
);

/*****************************************************************************/
//  Description : ListItemResetContentCheck
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListItemResetContentCheck(
    CTRLLIST_OBJ_T   *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : resume tp up
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListRecoverSlideByOffsetY(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    BOOLEAN         is_update,
    int32           move_space
);
#endif

/*****************************************************************************/
//  Description : get recover space
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL int32 ListGetRecoverSpace(
    CTRLLIST_OBJ_T   *list_ctrl_ptr
);

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 拷贝不需要更新的buffer
//  Global resource dependence :
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void ListDoDoubleMemWriteBack(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    int32             offset_y
);
#endif

/*****************************************************************************/
//  Description : get item content rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetItemContentRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          item_index,
    uint16          item_content_index,
    GUI_RECT_T     *disp_rect_ptr,//out
    GUI_RECT_T     *clip_rect_ptr //out
);

/*****************************************************************************/
//  Description : set control boder info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetBorder(
    CTRLBASE_OBJ_T    *iguictrl_ptr,
    GUI_BORDER_T  *border_ptr
);

/*****************************************************************************/
//  Description : Set the back ground color.
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
LOCAL BOOLEAN ListSetBackGround(
    CTRLBASE_OBJ_T  *iguictrl_ptr,
    GUI_BG_T    *bg_ptr
);

/*****************************************************************************/
//  Description : 设置list text font
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认不需要设置
/*****************************************************************************/
PUBLIC BOOLEAN ListSetTextFont(
    CTRLBASE_OBJ_T     *iguictrl_ptr,
    GUI_FONT_ALL_T *font_ptr
);

/*****************************************************************************/
//  Description : get control font info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetTextFont(
    CTRLBASE_OBJ_T      *iguictrl_ptr,
    GUI_FONT_ALL_T  *font_ptr
);

/*****************************************************************************/
//  Description : set control is display progress
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetProgress(
    CTRLBASE_OBJ_T   *iguictrl_ptr,
    BOOLEAN      is_display
);

/*****************************************************************************/
//  Description : 设置是否循环滚动
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认时循环滚动，当控件不循环滚动，焦点在最上/最下时，会发送消息给父窗口
/*****************************************************************************/
LOCAL BOOLEAN ListSetCycScroll(
    CTRLBASE_OBJ_T   *iguictrl_ptr,
    BOOLEAN      is_circular
);

/*****************************************************************************/
//  Description : 检查是否循环滚动
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认时循环滚动，当控件不循环滚动，焦点在最上/最下时，会发送消息给父窗口
/*****************************************************************************/
LOCAL BOOLEAN ListIsCycScroll(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get control height by width,include content,border etc.
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN ListGetHeight(
    CTRLBASE_OBJ_T  *iguictrl_ptr,  //in:
    uint16      width,          //in:
    uint16      *height_ptr     //in/out:
);

/*****************************************************************************/
//  Description : draw border
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawBorder(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get control select rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetSelectRect(
    CTRLBASE_OBJ_T     *iguictrl_ptr,  //in:
    GUI_RECT_T     *rect_ptr       //in/out:
);

/*****************************************************************************/
//  Description : 判断当前item的位置，是最后一个还是第一个
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListChangeCurItemPos(
    CTRLBASE_OBJ_T    *iguictrl_ptr,  //in:
    BOOLEAN       *is_first_ptr,  //in/out:
    BOOLEAN       *is_last_ptr    //in/out:
);

/*****************************************************************************/
//  Description : set list current item pos
//  Global resource dependence :
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetCurItemPos(
                                CTRLBASE_OBJ_T  *iguictrl_ptr,  //in:
                                BOOLEAN         is_first,       //in:
                                BOOLEAN         is_last         //in:
                                );

/*****************************************************************************/
//  Description : 获得总的页数
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 每页显示的条目根据list区域及item高度计算所得
/*****************************************************************************/
LOCAL uint16 ListGetTotalPageNum(
    CTRLLIST_OBJ_T *list_ctrl_ptr,     //IN:
    uint16 total_item_num
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL int32 ListGetTopIndexByFirstLetter(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    wchar wletter
);

/*****************************************************************************/
//  Description : Set current item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListResetCurItemIndex(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    uint16          cur_item_index
);

#ifdef MMI_READMENU_ENABLE
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: violent.wei
//  Note: 当有focus item变化时，朗读list内容
/*****************************************************************************/
LOCAL void ListHandleTextToSpeech(
    CTRLLIST_OBJ_T *list_ctrl_ptr //in
);
#endif

/*****************************************************************************/
//  Description : set gui control lcd device
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetLcdDevInfo(
    CTRLBASE_OBJ_T                *iguictrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_ptr
);

/*****************************************************************************/
//  Description : set
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetItemContentByPtr(
    CTRLLIST_OBJ_T           *list_ctrl_ptr,
    const GUIITEM_CONTENT_T  *item_content_ptr,
    uint16                   item_index,
    uint16                   item_content_index,
    BOOLEAN                  is_syn_decode,
    BOOLEAN                  is_update
);

LOCAL BOOLEAN ListCreateTipLayer(CTRLLIST_OBJ_T *list_ctrl_ptr);
LOCAL void ListReleaseTipLayer(CTRLLIST_OBJ_T *list_ctrl_ptr);
LOCAL void ListDispSplitTip(CTRLLIST_OBJ_T *list_ctrl_ptr, wchar tip_text);
LOCAL void ListHideSplitTipCB(uint8 timer_id, uint32 param);

/*==============================================================================
Description: 获取纯净的item 总数
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 所谓纯净的，即不计split类的
==============================================================================*/
LOCAL uint16 ListGetPureTotalItemNumber(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

//============================================================================//
//                                   For watch                                //
//============================================================================//

/*****************************************************************************/
//  Description : is need draw list item background
//  Parameter: [In] style_id: the current item style id
//             [Out] None
//             [Return] is_need : id need draw bg (TRUE : need ; FALSE : don't need)
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN isNeedDrawListItemBG(uint32 style_id);

//============================================================================//
//                                   For watch                                //
//============================================================================//

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author: fangfang.yao
//  Note:画高亮条
//       INNER_STATE_NEED_HIGHBAR_IF_CLICK打开，则绘制；否则不绘制
/*****************************************************************************/
LOCAL void ListSetItemHighBarIfClick( CTRLLIST_OBJ_T *list_ctrl_ptr);

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get listbox type
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T LIST_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
                       CTRL_BASE_TYPE,
                       "listbox",
                       sizeof(CTRLLIST_OBJ_T),
                       (OBJECT_CONSTRUCTOR_FUNC)ListCtrlConstruct,
                       (OBJECT_DESTRUCTOR_FUNC)ListCtrlDestruct,
                       sizeof(CTRLLIST_INIT_PARAM_T),
                       (PARAM_PACKING_FUNC)ListCtrlPackInitParam,
                       sizeof(CTRLLIST_VTBL_T),
                       (VTBL_INIT_FUNC)ListCtrlInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init list class
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListCtrlInitVtbl(
    CTRLLIST_VTBL_T *list_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T *)list_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent = ListCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect =  ListSetRect;
    base_ctrl_vtbl_ptr->SetBorder =  ListSetBorder;
    base_ctrl_vtbl_ptr->SetBg =  ListSetBackGround;
    base_ctrl_vtbl_ptr->SetFont =  ListSetTextFont;
    base_ctrl_vtbl_ptr->GetFont =  ListGetTextFont;
    base_ctrl_vtbl_ptr->GetHeightByWidth = ListGetHeight;
    base_ctrl_vtbl_ptr->GetDisplayRect = ListGetDisplayRect;
    base_ctrl_vtbl_ptr->SetDisplayRect = ListSetDisplayRect;
    base_ctrl_vtbl_ptr->SetLcdDevInfo = ListSetLcdDevInfo;
    base_ctrl_vtbl_ptr->SetScrollbarEnabled = ListSetProgress;
    base_ctrl_vtbl_ptr->SetUpDownLoop = ListSetCycScroll;
    base_ctrl_vtbl_ptr->GetSelectRect = ListGetSelectRect;
    base_ctrl_vtbl_ptr->GetSelectIndex = ListChangeCurItemPos;
    base_ctrl_vtbl_ptr->SetSelectIndex = ListSetCurItemPos;
    //base_ctrl_vtbl_ptr->SetVisible = ListSetVisible;

    return TRUE;
}

/*****************************************************************************/
//  Description :设置list不可见，同时删除list的层
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetVisible(
    CTRLBASE_OBJ_T *obj_ptr,  //in:
    BOOLEAN       is_visible  //in/out:
)
{
    BOOLEAN     result = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = (CTRLLIST_OBJ_T *)obj_ptr;

    if (PNULL != obj_ptr)
    {
        if (obj_ptr->is_visible != is_visible)
        {
            obj_ptr->is_visible = is_visible;
            MMK_CheckActive(obj_ptr->handle, (BOOLEAN)!is_visible);

            if (!is_visible
                && UILAYER_IsMultiLayerEnable()
                && (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&(list_ctrl_ptr->item_layer))))
            {
                if (obj_ptr->lcd_dev_info.block_id == list_ctrl_ptr->item_layer.block_id)
                {
                    obj_ptr->lcd_dev_info.block_id = GUI_BLOCK_MAIN;
                }

                UILAYER_RELEASELAYER(&list_ctrl_ptr->item_layer);
                LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_CUSTORMER_LAYER, FALSE);
            }
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : pack list init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListCtrlPackInitParam(
    GUILIST_INIT_DATA_T     *list_init_ptr,
    CTRLLIST_INIT_PARAM_T   *list_param_ptr
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T *)list_param_ptr;

    base_ctrl_param_ptr->both_rect = list_init_ptr->both_rect;

    list_param_ptr->is_dynamic_item_data = list_init_ptr->is_dynamic_item_data;
    list_param_ptr->max_item_num = list_init_ptr->max_item_num;
    list_param_ptr->max_selected_num = list_init_ptr->max_selected_num;
    list_param_ptr->scroll_bar_type = list_init_ptr->scroll_bar_type;
    list_param_ptr->type = list_init_ptr->type;
}

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListStartSwitchTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    if ((0 == list_ctrl_ptr->switch_timer_id)
        && (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_AUTO_SWITCH))
        && (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ACTIVE | GUILIST_STATE_FOCUS))
        && (0 < list_ctrl_ptr->total_item_num))
    {
        list_ctrl_ptr->switch_timer_id = MMK_CreateWinTimer(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, list_ctrl_ptr->display_style.auto_switch_time, FALSE);
    }

    return;
}

/*****************************************************************************/
//  Description : stop the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStopSwitchTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    if (0 < list_ctrl_ptr->switch_timer_id)
    {
        MMK_StopTimer(list_ctrl_ptr->switch_timer_id);

        list_ctrl_ptr->switch_timer_id = 0;
    }
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : start the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStartTpMoveTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    if (0 == list_ctrl_ptr->tp_move_timer_id)
    {
        list_ctrl_ptr->tp_move_timer_id = MMK_CreateWinTimer(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, list_ctrl_ptr->display_style.slide_time, FALSE);
    }
}

/*****************************************************************************/
//  Description : stop the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStopTpMoveTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    if (0 < list_ctrl_ptr->tp_move_timer_id)
    {
        MMK_StopTimer(list_ctrl_ptr->tp_move_timer_id);

        list_ctrl_ptr->tp_move_timer_id = 0;
    }
}
/*****************************************************************************/
//  Description : start the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStartTpTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    CONTINUE_MOVE_TYPE_E move_type
)
{
    if (PNULL != list_ctrl_ptr)
    {
        if (0 != list_ctrl_ptr->tp_timer_id)
        {
            MMK_StopTimer(list_ctrl_ptr->tp_timer_id);
            list_ctrl_ptr->tp_timer_id = 0;
        }

        if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_FOCUS))
        {
            list_ctrl_ptr->tp_timer_id = MMK_CreateWinTimer(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, list_ctrl_ptr->display_style.long_key_period, TRUE);
            list_ctrl_ptr->continue_move_type = move_type;
        }
    }
}

/*****************************************************************************/
//  Description : stop the key timer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListStopTpTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    if (PNULL != list_ctrl_ptr)
    {
        if (0 < list_ctrl_ptr->tp_timer_id)
        {
            MMK_StopTimer(list_ctrl_ptr->tp_timer_id);

            list_ctrl_ptr->tp_timer_id = 0;
        }
    }
}
#endif


/*****************************************************************************/
//  Description : Get list pointer from control id
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL CTRLLIST_OBJ_T *ListGetPtr(
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLLIST_OBJ_T *ctrl_ptr = (CTRLLIST_OBJ_T *)MMK_GetCtrlPtr(ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        if (!LIST_TypeOf(ctrl_ptr))
        {
            ctrl_ptr = PNULL;
        }
    }

    return ctrl_ptr;
}

/*****************************************************************************/
//  Description : set gui control lcd device
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetLcdDevInfo(
    CTRLBASE_OBJ_T            *base_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_ptr
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)base_ctrl_ptr;

    if ((PNULL == base_ctrl_ptr) || (PNULL == lcd_dev_ptr))
    {
        return FALSE;
    }

    if (base_ctrl_ptr->lcd_dev_info.block_id != lcd_dev_ptr->block_id)
    {
        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_CUSTORMER_LAYER, TRUE);
        base_ctrl_ptr->lcd_dev_info = *lcd_dev_ptr;
#ifdef UI_MULTILAYER_SUPPORT
        ListReleaseItemLayer(list_ctrl_ptr);
        list_ctrl_ptr->item_layer = *lcd_dev_ptr;
#endif
    }

    return TRUE;

}

/*****************************************************************************/
//  Description : list type of
//  Global resource dependence :
//  Author:James.Zhang
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
PUBLIC BOOLEAN LIST_TypeOf(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)list_ctrl_ptr, CTRL_LIST_TYPE);
}

/*****************************************************************************/
//  Description : alloc all items memory of the list control
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListAllocAllItemsSpace(
    CTRLLIST_OBJ_T *list_ctrl_ptr  // the list control pointer
)
{
    BOOLEAN result = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (PNULL != list_ctrl_ptr->item_info_ptr)
    {
        SCI_FREE(list_ctrl_ptr->item_info_ptr);
    }

    if (list_ctrl_ptr->max_item_num > 0)
    {
        // alloc the space
        list_ctrl_ptr->item_info_ptr = SCI_ALLOCA(list_ctrl_ptr->max_item_num * sizeof(CTRLLIST_ITEM_INFO_T));

        if (PNULL != list_ctrl_ptr->item_info_ptr)
        {
            SCI_MEMSET((void *)list_ctrl_ptr->item_info_ptr, 0, (list_ctrl_ptr->max_item_num * sizeof(CTRLLIST_ITEM_INFO_T)));
            result = TRUE;
        }
        else
        {
            list_ctrl_ptr->max_item_num = 0;
        }
    }

    return result;
}


/*****************************************************************************/
//  Description : reset the item pointer of list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListResetItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr  // the list control pointer
)
{
    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    ListStopTextAnim(list_ctrl_ptr);

    list_ctrl_ptr->total_item_num = 0;
    list_ctrl_ptr->top_item_idx = 0;
    list_ctrl_ptr->cur_item_idx = 0;
    list_ctrl_ptr->pre_top_item_idx = 0;
    list_ctrl_ptr->pre_cur_item_idx = 0;

    list_ctrl_ptr->total_item_height = 0;

    list_ctrl_ptr->selected_item_num = 0;
    list_ctrl_ptr->str_start_pos = 0;

    ListResetOffsetY(list_ctrl_ptr);

    list_ctrl_ptr->cur_object_index = -1;

    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_CONTENT, FALSE);
}

/*****************************************************************************/
//  Description : update selected item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListUpdateSeletedState(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_ptr
)
{
    BOOLEAN result = TRUE;

    if ((PNULL == list_ctrl_ptr) || (PNULL == item_ptr))
    {
        return FALSE;
    }

    if (!LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
    {
        return FALSE;
    }

    if (CTRLLIST_MARK_RADIO == list_ctrl_ptr->mark_type)
    {
        uint16 i = 0;
        CTRLLIST_ITEM_INFO_T *pre_item_ptr = PNULL;

        /* set all item unselected */
        for (; i < list_ctrl_ptr->total_item_num; i++)
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
            list_ctrl_ptr->selected_item_num++;
        }
        else
        {
            LIST_SetItemState(item_ptr, GUIITEM_STATE_SELECTED, FALSE);

            result = FALSE;
        }
    }

    return result;
}


/*****************************************************************************/
//  Description : reduce selected item num
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListReduceSelectedItemNum(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_ptr
)
{
    BOOLEAN result = FALSE;

    if ((PNULL == list_ctrl_ptr) || (PNULL == item_ptr))
    {
        return FALSE;
    }

    if (! LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
    {
        if (list_ctrl_ptr->selected_item_num > 0)
        {
            list_ctrl_ptr->selected_item_num--;

            result = TRUE;
        }
        else
        {
            LIST_SetItemState(item_ptr, GUIITEM_STATE_SELECTED, TRUE);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : transfer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListTransItem(
    CTRLLIST_ITEM_INFO_T *dest_item_ptr,
    const CTRLLIST_ITEM_T *src_item_ptr
)
{
    if (PNULL == dest_item_ptr || PNULL == src_item_ptr)
    {
        return;
    }

    dest_item_ptr->item.style_id = src_item_ptr->style_id;
    dest_item_ptr->item.first_letter = src_item_ptr->first_letter;
    dest_item_ptr->item.group_idx = src_item_ptr->group_idx;
    dest_item_ptr->item.item_group = src_item_ptr->item_group;
    dest_item_ptr->item.user_data = src_item_ptr->user_data;
    dest_item_ptr->item.state = src_item_ptr->state;
    dest_item_ptr->item.style_ptr = src_item_ptr->style_ptr;
    dest_item_ptr->item.data_ptr = PNULL;

    return;
}

/*****************************************************************************/
//  Description : insert item into list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListInsertItem(
    CTRLLIST_OBJ_T       *list_ctrl_ptr,  // the list control pointer
    const CTRLLIST_ITEM_T *add_item_ptr, // 指定的Item
    uint16                index           // 指定的位置, from 0 to total_item_num
)
{
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;
    uint16 new_group_id = 0;

    if ((PNULL == list_ctrl_ptr) || (PNULL == add_item_ptr))
    {
        return FALSE;
    }

    if (list_ctrl_ptr->total_item_num >= list_ctrl_ptr->max_item_num)
    {
        return FALSE;
    }

    index = (uint16)MIN(index, list_ctrl_ptr->total_item_num);

    /* memmove */
    if (list_ctrl_ptr->total_item_num - index > 0)
    {
        memmove(list_ctrl_ptr->item_info_ptr + index + 1,
                list_ctrl_ptr->item_info_ptr + index,
                (list_ctrl_ptr->total_item_num - index) * sizeof(CTRLLIST_ITEM_INFO_T));
    }

    item_ptr = list_ctrl_ptr->item_info_ptr + index;

    ListTransItem(item_ptr, add_item_ptr);

    //add selected item num
    ListUpdateSeletedState(list_ctrl_ptr, item_ptr);

    if (!LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA))
    {
        LIST_CreateItemData(list_ctrl_ptr, item_ptr, add_item_ptr->data_ptr);
    }

    list_ctrl_ptr->total_item_num++;

    //设定group index - 1 获取
    if (0 == index)
    {
        new_group_id = 0;
    }
    else
    {
        new_group_id = list_ctrl_ptr->item_info_ptr[index - 1].item.group_idx;
    }

    //设定group index - 2 设定
    if (GUIITEM_STATE_SPLIT & add_item_ptr->state)
    {
        uint16 item_idx = 0;

        list_ctrl_ptr->item_info_ptr[index].item.group_idx = new_group_id + 1;

        //将之后的所有item的索引+1
        for (item_idx = index + 1; item_idx < list_ctrl_ptr->total_item_num; item_idx++)
        {
            list_ctrl_ptr->item_info_ptr[item_idx].item.group_idx += 1;
        }
    }
    else
    {
        list_ctrl_ptr->item_info_ptr[index].item.group_idx = new_group_id;
    }

    if (!list_ctrl_ptr->is_full_dirty)
    {
        LIST_ResetItemDisp(list_ctrl_ptr, index);
    }

    list_ctrl_ptr->is_pos_dirty = TRUE;

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);

    return TRUE;
}

/*****************************************************************************/
//  Description : Replace the item in position
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListReplaceItem(
    CTRLLIST_OBJ_T       *list_ctrl_ptr,
    const CTRLLIST_ITEM_T *replace_item_ptr,
    uint16                index
)
{
    int32  item_top = 0;
    uint16 item_height = 0;
    CTRLLIST_ITEM_INFO_T  item_info = {0};
    CTRLLIST_ITEM_INFO_T *dest_item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

    if (PNULL == list_ctrl_ptr || PNULL == replace_item_ptr || index >= list_ctrl_ptr->total_item_num)
    {
        return FALSE;
    }

    if (PNULL == dest_item_ptr)
    {
        return FALSE;
    }

    item_height = LIST_GetItemHeight(list_ctrl_ptr, index);
    item_top = dest_item_ptr->item_top;

    ListTransItem(&item_info, replace_item_ptr);

    LIST_UnSelectItem(list_ctrl_ptr, index);

    if (!LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA))
    {
        LIST_CreateItemData(list_ctrl_ptr, &item_info, replace_item_ptr->data_ptr);
    }

    //拷贝完才能释放旧item data, 否则用户get item之后replace会出现野指针!!!
    if (LIST_DestroyItemData(list_ctrl_ptr, dest_item_ptr, FALSE))
    {
        //终止当前正在解码的item
        LIST_DestroyItemDecode(list_ctrl_ptr);
    }

    SCI_MEMCPY(dest_item_ptr, &item_info, sizeof(CTRLLIST_ITEM_INFO_T));

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);

    ListUpdateSeletedState(list_ctrl_ptr, dest_item_ptr);

    if (!list_ctrl_ptr->is_full_dirty)
    {
        LIST_ResetItemDisp(list_ctrl_ptr, index);
    }

    list_ctrl_ptr->is_pos_dirty = TRUE;

    return TRUE;
}

/*****************************************************************************/
//  Description : Adjust item list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListValidateCtrl(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    BOOLEAN         is_cur_visible,
    BOOLEAN         is_must_set_offset
)
{
    int32       offset_y = 0;
    GUI_RECT_T  list_rect = {0};

    SCI_ASSERT(PNULL != list_ctrl_ptr);   /*assert verified*/

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    if (!GUI_IsInvalidRect(list_rect) && LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID))
    {
        if (list_ctrl_ptr->total_item_num > 0)
        {
            uint16 item_num = 0;

            /* validate current index */
            if (list_ctrl_ptr->cur_item_idx >= list_ctrl_ptr->total_item_num)
            {
                list_ctrl_ptr->cur_item_idx = (uint16)(list_ctrl_ptr->total_item_num - 1);
            }

            item_num = LIST_GetItemNumByHeight(list_ctrl_ptr,
                                               (uint16)(list_rect.bottom - list_rect.top + 1),
                                               list_ctrl_ptr->cur_item_idx, FALSE);

            if (0 == item_num)
            {
                list_ctrl_ptr->top_item_idx = list_ctrl_ptr->cur_item_idx;
            }
            else
            {
                /* validate top index*/
                if (list_ctrl_ptr->top_item_idx > list_ctrl_ptr->total_item_num - item_num)
                {
                    list_ctrl_ptr->top_item_idx = (uint16)(list_ctrl_ptr->total_item_num - item_num);
                }

                if (is_cur_visible)
                {
                    if (list_ctrl_ptr->top_item_idx > list_ctrl_ptr->cur_item_idx)
                    {
                        list_ctrl_ptr->top_item_idx = list_ctrl_ptr->cur_item_idx;
                    }
                    else if (list_ctrl_ptr->top_item_idx < list_ctrl_ptr->cur_item_idx - item_num + 1)
                    {
                        list_ctrl_ptr->top_item_idx = (uint16)(list_ctrl_ptr->cur_item_idx - item_num + 1);
                    }
                }
            }
        }
        else
        {
            list_ctrl_ptr->top_item_idx = 0;
            list_ctrl_ptr->cur_item_idx = 0;
        }

        if (list_ctrl_ptr->top_item_idx != list_ctrl_ptr->pre_top_item_idx || is_must_set_offset)
        {
            list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;
            LIST_ResetOffsetYByTopIndexExt(list_ctrl_ptr, list_ctrl_ptr->top_item_idx);
        }

        list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;

        if (is_cur_visible && list_ctrl_ptr->top_item_idx > list_ctrl_ptr->cur_item_idx)
        {
            ListReviceDrawItem(list_ctrl_ptr, &offset_y, FALSE);
        }
        else if (is_cur_visible && list_ctrl_ptr->top_item_idx < list_ctrl_ptr->cur_item_idx)
        {
            ListReviceDrawItem(list_ctrl_ptr, &offset_y, TRUE);
        }
        else
        {
            offset_y = ListGetRecoverSpace(list_ctrl_ptr);
        }

        list_ctrl_ptr->offset_y += offset_y;

        LIST_ResetTopItemAndEndItemIndexExt(list_ctrl_ptr, offset_y);

        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, FALSE);
    }
}
/*****************************************************************************/
//  Description : 设置List控件的默认图片ID,为了能够将image_id为NULL,能够设置成功
//  Global resource dependence :
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetDefaultIconEx(
    MMI_CTRL_ID_T     ctrl_id, //in
    MMI_IMAGE_ID_T    def_img_id,  // default image id
    MMI_IMAGE_ID_T    err_img_id   // error image id
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }
    else
    {
        list_ctrl_ptr->display_style.icon_default_id = def_img_id;
        list_ctrl_ptr->display_style.icon_error_id = err_img_id;

        return TRUE;
    }

}

/*****************************************************************************/
//  Description : 设置List控件的默认图片ID
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetDefaultIcon(
    MMI_CTRL_ID_T     ctrl_id, //in
    MMI_IMAGE_ID_T    def_img_id,  // default image id
    MMI_IMAGE_ID_T    err_img_id   // error image id
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return LIST_SetDefaultIconByPtr(list_ctrl_ptr, def_img_id, err_img_id);
}

/*****************************************************************************/
//  Description : 设置List控件的默认图片ID
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SetDefaultIconByPtr(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_IMAGE_ID_T    def_img_id,  // default image id
    MMI_IMAGE_ID_T    err_img_id   // error image id
)
{
    BOOLEAN         result          = FALSE;

    if (PNULL != list_ctrl_ptr)
    {
        if (MMITHEME_CheckImageID(def_img_id))
        {
            list_ctrl_ptr->display_style.icon_default_id = def_img_id;
        }

        if (MMITHEME_CheckImageID(err_img_id))
        {
            list_ctrl_ptr->display_style.icon_error_id = err_img_id;
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : draw highlight item background
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawHighlightBackground(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *rect_ptr,
    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
)
{
    int16               top = 0;            // 可以为负,为以后扩展到可以在负区划图作准备
    uint16              img_height = 0;     // 图片的高度
    GUI_RECT_T          display_rect = {0}; // 显示区域
    GUI_RECT_T          clip_rect  = {0};    // 图片区域
    GUI_RECT_T          image_rect = {0};   // 图片区域
    GUI_RECT_T          item_rect  = {0};
    GUI_RECT_T          valid_rect = {0};
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    // 输入参数检测
    if ((PNULL == list_ctrl_ptr) || (PNULL == lcd_dev_info_ptr) || (PNULL == rect_ptr))
    {
        return;
    }

    if (!LIST_GetValidRect(list_ctrl_ptr, &valid_rect) || !GUI_IntersectRect(&clip_rect, *rect_ptr, valid_rect))
    {
        return;
    }

    // 画OSD背景图
    if (list_ctrl_ptr->display_style.is_disp_hilight_image)
    {
        //int32 i = 0;
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        if ( (PNULL == item_ptr) || isNeedDrawListItemBG(item_ptr->item.style_id)) 
         // Bug 1321334 当list样式为特殊背景样式时，不画common背景
        {
            return;
        }

        display_rect.bottom = clip_rect.bottom;
        display_rect.left = clip_rect.left;
        display_rect.right =  clip_rect.right;
        display_rect.top = clip_rect.top;

        img_height = (uint16)(clip_rect.bottom - clip_rect.top + 1);

        top = (int16)(clip_rect.top - (img_height - (clip_rect.bottom - clip_rect.top)) / 2);
        display_rect.top = top;

        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        //set image rect
        image_rect.left   = (int16)(display_rect.left - item_rect.left);
        image_rect.top    = (int16)(display_rect.top - item_rect.top);
        image_rect.right  = (int16)(image_rect.left + display_rect.right - display_rect.left);
        image_rect.bottom = (int16)(image_rect.top + display_rect.bottom - display_rect.top);

        // 透明背景图的时候，需要重画背景
        ListDrawBackgroundByRect(list_ctrl_ptr, rect_ptr);

        IMG_EnableTransparentColor(TRUE);
        GUIRES_DisplayImg(PNULL,
                          &display_rect,
                          &image_rect,
                          list_ctrl_ptr->win_id,
                          list_ctrl_ptr->display_style.highlight_back_image,
                          lcd_dev_info_ptr);

        IMG_EnableTransparentColor(FALSE);
    }
    else
    {
        GUI_FillRect(lcd_dev_info_ptr, clip_rect, list_ctrl_ptr->display_style.highlight_back_color);
    }
}

/*****************************************************************************/
//  Description : 获取列表控件文本的style
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListGetTextStyle(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUISTR_STYLE_T *str_style_ptr,
    uint16 item_index,
    uint16 content_index
)
{
    GUI_ALIGN_E align = ALIGN_LVMIDDLE;
    CTRLLIST_ITEM_INFO_T *item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);
    THEMELIST_ITEM_STYLE_T   *item_style_ptr = PNULL;

    if ((PNULL == list_ctrl_ptr) ||
        (PNULL == str_style_ptr) ||
        (GUIITEM_CONTENT_MAX_NUM <= content_index) ||
        (PNULL == item_ptr))
    {
        return;
    }

    item_style_ptr = item_ptr->item.style_ptr;

    if (PNULL == item_style_ptr)
    {
        return;
    }

    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        if (item_style_ptr->content[content_index].state & GUIITEM_CONTENT_STATE_TEXT_R_ALIGN)
        {
            align = ALIGN_LVMIDDLE;
        }
        else if (item_style_ptr->content[content_index].state & GUIITEM_CONTENT_STATE_TEXT_M_ALIGN)
        {
            align = ALIGN_HVMIDDLE;
        }
        else
        {
            align = ALIGN_RVMIDDLE;
        }
    }
    else
    {
        if (item_style_ptr->content[content_index].state & GUIITEM_CONTENT_STATE_TEXT_R_ALIGN)
        {
            align = ALIGN_RVMIDDLE;
        }
        else if (item_style_ptr->content[content_index].state & GUIITEM_CONTENT_STATE_TEXT_M_ALIGN)
        {
            align = ALIGN_HVMIDDLE;
        }
        else
        {
            align = ALIGN_LVMIDDLE;
        }
    }

    str_style_ptr->align      = align;
    str_style_ptr->char_space = list_ctrl_ptr->char_space;
    str_style_ptr->effect     = FONT_EFFECT_NONE;

    if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_TEXT_STYLE))
    {
        str_style_ptr->font = list_ctrl_ptr->text_style.font;
        str_style_ptr->font_color = list_ctrl_ptr->text_style.font_color;
    }
    else
    {
        if (item_index == list_ctrl_ptr->cur_item_idx)
        {
            str_style_ptr->font = item_style_ptr->content[content_index].font_focus;
        }
        else
        {
            str_style_ptr->font = item_style_ptr->content[content_index].font;
        }

        str_style_ptr->font_color = MMICOM_LIST_FONT_COLOR;//LIST_GetItemContentTextColor(list_ctrl_ptr, item_index, content_index);
    }

    //
    if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_TEXT_COLOR))   // 这个状态只有form控件使用
    {
        str_style_ptr->font_color = list_ctrl_ptr->text_style.font_color;
    }

    if (LIST_GetItemState(item_ptr, GUIITEM_STATE_GRAY)) // 取灰色字体
    {
        str_style_ptr->font_color = MMITHEME_GetGrayColor();
    }
    else if (LIST_GetItemState(item_ptr, GUIITEM_STATE_COSTMER_FONT_COLOR)) // 根据item取自定义色彩
    {
        str_style_ptr->font_color = list_ctrl_ptr->display_style.item_font_color;
    }

    // 注意，分隔符使用统一的字体颜色
    if (LIST_GetItemState(item_ptr, GUIITEM_STATE_SPLIT))
    {
        str_style_ptr->font = item_style_ptr->content[content_index].font;
        str_style_ptr->font_color = LIST_GetItemContentTextColor(list_ctrl_ptr, item_index, content_index);
    }

    return;
}

/*******************************************************************************************/
//  Description : calculate the ucs2 string piexl number
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*********************************************************************************************/
LOCAL uint16 ListCalculateStringPiexlNum(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const wchar *str_ptr,    //[IN] String指针
    uint16       str_len,     //[IN] String长度(Byte长度)
    uint16 item_index,
    uint16 content_index
)
{
    GUISTR_STYLE_T style = {0};/*lint !e64*/

    ListGetTextStyle(list_ctrl_ptr, &style, item_index, content_index);

    return GUI_CalculateStringPiexlNum(str_ptr, str_len, style.font, style.char_space);
}

/*******************************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: James.Zhang
//  Note: note: 显示动画
/*********************************************************************************************/
LOCAL void ListHandleInlineAnimCtrl(
    CTRLBASE_OBJ_T *anim_ctrl_ptr,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr,
    GUI_RECT_T ctrl_rect,
    GUI_RECT_T disp_rect
)
{
    if (PNULL != anim_ctrl_ptr)
    {
        GUICTRL_SetLcdDevInfo((IGUICTRL_T *)anim_ctrl_ptr, lcd_dev_info_ptr);

        GUIANIM_SetCtrlRectByPtr(anim_ctrl_ptr, &ctrl_rect);

        GUIANIM_SetDisplayRectByPtr(anim_ctrl_ptr, &disp_rect, FALSE);

        IGUICTRL_HandleEvent((IGUICTRL_T *)anim_ctrl_ptr, MSG_CTL_PAINT, PNULL);
    }

    return;
}

/*****************************************************************************/
//  Description : draw one item of the list control
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr,   //on witch block
    uint16 index
)
{
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T list_rect = {0};
    GUI_RECT_T clip_rect = {0};
    THEMELIST_ITEM_STYLE_T *style_ptr = PNULL;
    GUI_RECT_T group_bg_rect = {0};
    BOOLEAN is_current_item_focus = FALSE;

    uint8                         i               = 0;
    GUI_RECT_T                    rect            = {0};

    if ((PNULL == list_ctrl_ptr) || (PNULL == lcd_dev_info_ptr))
    {
        return;
    }

    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

    if (PNULL == item_ptr || PNULL == item_ptr->item.data_ptr)
    {
        return;
    }

    LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, index);

    if (!LIST_GetValidRect(list_ctrl_ptr, &list_rect) || !GUI_IntersectRect(&clip_rect, item_rect, list_rect))
    {
        return;
    }

    style_ptr = item_ptr->item.style_ptr;

    if (LIST_GetItemState((const CTRLLIST_ITEM_INFO_T *)item_ptr, GUIITEM_STATE_SPLIT))
    {
#ifdef UI_MULTILAYER_SUPPORT

        if ((index == list_ctrl_ptr->top_item_idx)
            && (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SPLIT_ONSIDE | GUILIST_STATE_ACTIVE | GUILIST_STATE_FOCUS)))
        {
            //如果第一个项是滚动条，不需要再画了，已经在ListDrawSplitBar画了
            return;
        }

#endif

        // 画split bar的背景
        ListDrawSplitStyleItemBg(list_ctrl_ptr, item_rect, lcd_dev_info_ptr);
    }

    // 画分割线
    if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SPLIT_LINE))
    {
        ListDrawItemSplitLine(list_ctrl_ptr, index, item_rect, lcd_dev_info_ptr);
    }

    is_current_item_focus = (index == list_ctrl_ptr->cur_item_idx);

    //画item  背景
    if(isNeedDrawListItemBG(item_ptr->item.style_id))
    {
        GUI_BG_T    item_bg     = WATCHTHEME_GetListItemBg();
        GUI_RECT_T  icon_rect   = {0};

        if(LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
        {
            if(index == list_ctrl_ptr->cur_item_idx)
            {
                item_bg = WATCHTHEME_GetListHightLightItemBg();				
            }
        }

        if (GUI_IntersectRect(&icon_rect, item_rect, list_rect))
        {
            LIST_DrawItemIcon(list_ctrl_ptr, &item_rect, &icon_rect, item_bg.img_id, lcd_dev_info_ptr);
        }
    }

    //画分组背景
    group_bg_rect = item_rect;

    if (item_ptr->item.item_group && !(item_ptr->item.item_group & GUIITEM_GROUP_NO_BG))
    {
        GUI_RECT_T img_rect = {0};
        GUI_RECT_T bg_disp_rect = {0};

        if (GUI_IntersectRect(&bg_disp_rect, group_bg_rect, list_rect))
        {
            IMG_EnableTransparentColor(TRUE);

            img_rect = bg_disp_rect;
            img_rect.left -= group_bg_rect.left;
            img_rect.right -= group_bg_rect.left;
            img_rect.top -= group_bg_rect.top;
            img_rect.bottom -= group_bg_rect.top;

            GUIRES_DisplaySabmImg(&bg_disp_rect,
                                  PNULL,
                                  &img_rect,
                                  list_ctrl_ptr->win_id,
                                  LIST_GetItemBgImageID(list_ctrl_ptr, index, is_current_item_focus),
                                  lcd_dev_info_ptr);

            IMG_EnableTransparentColor(FALSE);
        }
    }

    //draw mark
    if (CTRLLIST_MARKING_NONE != list_ctrl_ptr->marking_type
        && !LIST_GetItemState(item_ptr, GUIITEM_STATE_DISABLE_MARK))
    {
        MMI_IMAGE_ID_T mark_icon_id = LIST_GetSelectIconId(list_ctrl_ptr, item_ptr);

        if (0 != mark_icon_id)
        {
            GUI_RECT_T mark_rect = item_rect;
            GUI_RECT_T mark_disp_rect = {0};

            mark_rect.left = item_ptr->mark_left;

            mark_rect.right = mark_rect.left + list_ctrl_ptr->display_style.tag_width - 1;

            if (GUI_IntersectRect(&mark_disp_rect, mark_rect, list_rect))
            {
                LIST_DrawItemIcon(list_ctrl_ptr, &mark_rect, &mark_disp_rect, mark_icon_id, lcd_dev_info_ptr);
            }
        }
    }

    // darw number icon
    if (list_ctrl_ptr->show_num
        && LIST_GetItemState(item_ptr, GUIITEM_STATE_HAS_NUM))
    {
        MMI_IMAGE_ID_T number_icon_id = LIST_GetNumberIconId(list_ctrl_ptr, index);

        if (0 != number_icon_id)
        {
            GUI_RECT_T num_rect = item_rect;
            GUI_RECT_T num_disp_rect = {0};

            num_rect.left = item_ptr->num_left;
            num_rect.right = num_rect.left + list_ctrl_ptr->display_style.tag_width - 1;

            //防止item rect超出list的rect
            if (GUI_IntersectRect(&num_disp_rect, num_rect, list_rect))
            {
                LIST_DrawItemIcon(list_ctrl_ptr, &num_rect, &num_disp_rect, number_icon_id, lcd_dev_info_ptr);
            }

        }
    }

    // draw all content
    for (i = 0; i < GUIITEM_CONTENT_MAX_NUM; i++)
    {
        GUI_RECT_T con_rect = item_ptr->content_rect[i];
        GUI_RECT_T con_disp_rect = {0};
        BOOLEAN need_disp = FALSE;

        //con_rect -> absolute
        SetRectOffset(&con_rect, item_rect.left, item_rect.top);

        need_disp = GUI_IntersectRect(&con_disp_rect, con_rect, list_rect);

        if (!need_disp)
        {
            continue;
        }

        switch (item_ptr->item.data_ptr->item_content[i].item_data_type)
        {
            case GUIITEM_DATA_TEXT_ID:
            case GUIITEM_DATA_TEXT_BUFFER:
            {
                MMI_STRING_T text_info = {0};
                BOOLEAN need_scrolling = FALSE;
                BOOLEAN is_single_line = TRUE;

                if (!LIST_GetListItemContentTextInfo(list_ctrl_ptr, item_ptr, i, &text_info))
                {
                    break; // 取字符串失败的话就不显示了
                }

                need_scrolling = (is_current_item_focus
                                  && i == style_ptr->main_index
                                  //&& LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR) // Bug 1335463
                                  && (!list_ctrl_ptr->display_style.is_hl_disappear  || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()) || list_ctrl_ptr->is_long_pressing)
                                  && LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ACTIVE));

                is_single_line = !(i == style_ptr->sub_index
                                   && (GUIITEM_CONTENT_STATE_MULTILINE & style_ptr->content[i].state));

                /* draw scroll string */
                if (need_scrolling)
                {
                    GUISTR_STYLE_T str_style = {0}; /*lint !e64*/                    
#ifdef SCRIPTEASY_SUPPORT
                    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN;
#else
                    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR;
#endif
                    ListGetTextStyle(list_ctrl_ptr, &str_style, index, i);
                    LIST_GetItemContentEffectStr(list_ctrl_ptr, &str_style, index, i);

                    if (is_single_line)
                    {
                        str_state |= GUISTR_STATE_SINGLE_LINE;
                    }
                    else
                    {
                        str_state |= GUISTR_STATE_WORDBREAK;
                    }

                    if (MMITHEME_GetIsScrollAsPixel())
                    {
                        GUISTR_DrawTextToLCDByOffset(lcd_dev_info_ptr, &con_rect, &con_disp_rect, list_ctrl_ptr->str_start_pos, 0, &text_info, &str_style, str_state);
                    }
                    else
                    {
                        MMI_STRING_T   str_info = {0};

                        str_info.wstr_ptr = text_info.wstr_ptr + list_ctrl_ptr->str_start_pos;
                        str_info.wstr_len = (uint16)(text_info.wstr_len - list_ctrl_ptr->str_start_pos);

                        GUISTR_DrawTextToLCDInRect(lcd_dev_info_ptr,
                                                   (const GUI_RECT_T *)&con_rect,
                                                   (const GUI_RECT_T *)&con_disp_rect,
                                                   (const MMI_STRING_T *)&str_info,
                                                   &str_style,
                                                   str_state,
                                                   GUISTR_TEXT_DIR_AUTO);
                    }

                    GUISTR_DestroyRegionStyle(str_style.region_ptr);
                }
                else
                {
                    ListDrawString(list_ctrl_ptr, lcd_dev_info_ptr, text_info.wstr_ptr, text_info.wstr_len, &con_rect, &con_disp_rect, index, i, is_single_line);
                }

                break;
            }

            case GUIITEM_DATA_IMAGE_ID:
            {
                MMI_IMAGE_ID_T image_id = item_ptr->item.data_ptr->item_content[i].item_data.image_id;

                // draw bg color
                LIST_DrawItemContentBgColor(list_ctrl_ptr, lcd_dev_info_ptr,
                                            item_ptr->item.data_ptr->item_content[i].bg_color_id,
                                            con_rect, con_disp_rect, image_id);
                // draw icon
                LIST_DrawItemIcon(list_ctrl_ptr, &con_rect, &con_disp_rect, image_id, lcd_dev_info_ptr);
                // 显示小icon
                LIST_DrawItemContentBgIcon(list_ctrl_ptr, lcd_dev_info_ptr, item_rect, con_rect, item_ptr->item.data_ptr->item_content[i].icon_id);

                break;
            }

            case GUIITEM_DATA_ANIM_DATA:
            case GUIITEM_DATA_ANIM_PATH:
                if (PNULL == item_ptr->inner_handle[i])
                {
                     if (
                        (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state) // slide的状态不需要解码，否则速度贼慢
                        || list_ctrl_ptr->is_support_slide_dec_anim
                    )
                    {
                        //display icon background
                        LIST_DrawItemIconBg(&rect, list_ctrl_ptr, lcd_dev_info_ptr);

                        LIST_SetItemState(item_ptr, GUIITEM_STATE_NEED_ITEM_CONTENT, TRUE);

                        if (!LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_CONTENT))
                        {
                            if (MMK_IsOpenWin(list_ctrl_ptr->win_id)) // Bug-1376378
                            {
                                list_ctrl_ptr->cur_object_index = index;

                                LIST_SendNeedItemContentMsg(list_ctrl_ptr, index, i);
                            }
                        }
                    }
                    else
                    {
                        //display icon background
                        LIST_DrawItemIconBg(&rect, list_ctrl_ptr, lcd_dev_info_ptr);
                    }
                }
                else
                {
                    ListHandleInlineAnimCtrl(
                        (CTRLBASE_OBJ_T *)MMK_GetCtrlPtr(item_ptr->inner_handle[i]),
                        lcd_dev_info_ptr, con_rect, con_disp_rect);
                }

                break;

            default:
                break;
        }
    }

    if( LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_SLIDE_ITEM)
        &&(LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_LEFT_SLIDE_ITEM))
        && (index == list_ctrl_ptr->cur_item_idx))
    {
        uint16                      lcd_width       = MMITHEME_GetClientRectWidth();
        GUI_RECT_T                  con_rect        = {0,0,0,0};
        GUI_RECT_T                  icon_rect       = {0,0,MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT - 1,MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT - 1};
        MMITHEME_LIST_CUSTDATA_T    user_data       = {0};

        // 取用户数据，如果回调指针为空，取默认值
        if (PNULL != list_ctrl_ptr->get_data_func)
        {
            list_ctrl_ptr->get_data_func(&user_data);
        }
        else
        {
            MMITHEME_GetListItemDefData(&user_data);
        }

        con_rect.top = (item_ptr->item_height - MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT)/2;
        con_rect.bottom = con_rect.top + MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT;
        con_rect.left = lcd_width + MMICOM_LIST_SLIDE_ICON_MARGIN - 1;
        con_rect.right = con_rect.left + MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT;

        //con_rect -> absolute
        SetRectOffset(&con_rect, item_rect.left, item_rect.top);

        GUIRES_DisplayImg(PNULL,
                          &con_rect,
                          &icon_rect,
                          list_ctrl_ptr->win_id,
                          user_data.left_icon_id,
                          lcd_dev_info_ptr);

        con_rect.left = con_rect.right + MMICOM_LIST_SLIDE_ICON_MARGIN;
        con_rect.right = con_rect.left + MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT;

        GUIRES_DisplayImg(PNULL,
                          &con_rect,
                          &icon_rect,
                          list_ctrl_ptr->win_id,
                          user_data.right_icon_id,
                          lcd_dev_info_ptr);
    }
#ifdef GUIF_LIST_PDASTYLE

    if (LIST_GetItemState(item_ptr, GUIITEM_STATE_EXPAND))
    {
        GUI_RECT_T expand_rect = item_rect;
        GUI_RECT_T expand_disp_rect = {0};

        expand_rect.left = expand_rect.right - list_ctrl_ptr->display_style.tag_width + 1;

        if (GUI_IntersectRect(&expand_disp_rect, expand_rect, list_rect))
        {
            LIST_DrawItemIcon(list_ctrl_ptr, &expand_rect, &expand_disp_rect, list_ctrl_ptr->display_style.expand_image_id, lcd_dev_info_ptr);
        }
    }

#endif

    return;
}

/*****************************************************************************/
//  Description : Draw Sting
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListDrawString(
    CTRLLIST_OBJ_T              *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO      *dev_info_ptr,
    const wchar                 *wstr_ptr,
    uint16                      wstr_len,
    GUI_RECT_T                  *disp_rect_ptr,
    GUI_RECT_T                  *clip_rect_ptr,
    uint16                      item_index,
    uint16                      content_index,
    BOOLEAN                     is_single_line
)
{
    GUISTR_STYLE_T str_style = {0}; /*lint !e64*/
    MMI_STRING_T   str_info = {0};
    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
	CTRLLIST_ITEM_INFO_T * item_ptr = PNULL;

    if ((PNULL == list_ctrl_ptr)
        || (PNULL == dev_info_ptr)
        || (PNULL == disp_rect_ptr)
        || (PNULL == clip_rect_ptr)
        || (PNULL == wstr_ptr)
        || (0 == wstr_len))
    {
        return;
    }

	item_ptr =  LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

    ListGetTextStyle(list_ctrl_ptr, &str_style, item_index, content_index);

    LIST_GetItemContentEffectStr(list_ctrl_ptr, &str_style, item_index, content_index);

    if (is_single_line)
    {
        str_state |= GUISTR_STATE_SINGLE_LINE;
    }
    else
    {
        str_state |= GUISTR_STATE_WORDBREAK;
    }

    str_info.wstr_ptr = (wchar *)wstr_ptr;
    str_info.wstr_len = wstr_len;

	if(item_ptr->item.style_id == GUIITEM_STYLE_TWO_TEXT_AND_ONE_ICON_MS)
	{
		if(item_ptr->item.data_ptr->item_content[content_index].is_custom_font_color){
			str_style.font_color = item_ptr->item.data_ptr->item_content[content_index].custom_font_color;
		}
		if(content_index == 0){
			str_style.font = SONG_FONT_20;
		}else if(content_index == 1){
			str_style.font = SONG_FONT_16;
		}
		str_state = GUISTR_STATE_FROM_HEAD;
		str_style.align = ALIGN_LVMIDDLE;
	}
	
	GUISTR_DrawTextToLCDInRect(dev_info_ptr,
	                               (const GUI_RECT_T *)disp_rect_ptr,
	                               (const GUI_RECT_T *)clip_rect_ptr,
	                               (const MMI_STRING_T *)&str_info,
	                               &str_style,
	                               str_state,
	                               GUISTR_TEXT_DIR_AUTO
	                              );
	
    GUISTR_DestroyRegionStyle(str_style.region_ptr);
}

/*****************************************************************************/
//  Description : draw list background
//  Global resource dependence :
//  Author: Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawBackground(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    if (PNULL != list_ctrl_ptr)
    {
        ListDrawBackgroundByRect(list_ctrl_ptr, &((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect);
    }

    return;
}

/*****************************************************************************/
//  Description : draw item background
//  Global resource dependence :
//  Author: Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawBackgroundByRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T    *rect_ptr
)
{
    GUI_RECT_T          clip_rect       = {0};
    GUI_RECT_T          valid_rect      = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info    = {0};
    GUI_BG_DISPLAY_T    bg_display      = {0};
    GUI_BG_T            bg_info         = {0};
    uint16              image_width     = 0;
    uint16              image_height    = 0;

    if ((PNULL == list_ctrl_ptr) || (PNULL == rect_ptr))
    {
        return;
    }

    if (LIST_GetValidRect(list_ctrl_ptr, &valid_rect)
        && GUI_IntersectRect(&clip_rect, *rect_ptr, valid_rect))
    {
        lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

        //set bg display info
        bg_display.is_transparent = FALSE;
        bg_display.rect           = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect;
        bg_display.display_rect   = clip_rect;
        bg_display.ctrl_handle    = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        bg_display.win_handle     = list_ctrl_ptr->win_id;

        // 获得背景
        if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SELFSET_BG)) // 是否自设置图片
        {
            bg_info = list_ctrl_ptr->display_style.bg_info;
        }
        else
        {
            // list显示common 背景，如果显示theme背景，这里需要屏蔽
            //@for light function modify
#if (defined MAINLCD_SIZE_240X320) || (defined DP_SCREEN_SIZE_240X240)
            // ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->bg = MMITHEME_GetCommonLightBg();
            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->bg.bg_type = GUI_BG_COLOR;
            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->bg.color = MMICOM_LIST_BG_COLOR;
#else
            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->bg = MMITHEME_GetCommonBg();
#endif
            bg_info = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->bg;
        }

        // 调整图片显示区域
        if (GUI_BG_IMG == bg_info.bg_type)
        {
            if (GUIRES_IsSabm(bg_info.img_id, list_ctrl_ptr->win_id))
            {
                bg_display.img_rect = clip_rect;
                bg_display.img_rect.left -= bg_display.rect.left;
                bg_display.img_rect.right -= bg_display.rect.left;
                bg_display.img_rect.top -= bg_display.rect.top;
                bg_display.img_rect.bottom -= bg_display.rect.top;
            }
            else
            {
                if (GUIRES_GetImgWidthHeight(&image_width, &image_height, bg_info.img_id, list_ctrl_ptr->win_id))
                {
                    bg_display.img_rect.right = image_width - 1;
                    bg_display.img_rect.bottom = image_height - 1;
                }
            }
        }

#ifdef UI_MULTILAYER_SUPPORT

        // 如果背景和item在同一layer，需要clear，避免背景画错时，文字出现重叠
        if (((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info.block_id == list_ctrl_ptr->item_layer.block_id
            && UILAYER_IsLayerActive(&list_ctrl_ptr->item_layer))
        {
            MMITHEME_ClearRect(&lcd_dev_info, &clip_rect);
        }

#endif
        // 显示图片
        GUI_DisplayBg(&bg_info,
                      &bg_display,
                      &((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info);

#ifdef UI_MULTILAYER_SUPPORT

        // 当不在同一layer时，清空item layer
        if (((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info.block_id != list_ctrl_ptr->item_layer.block_id
            && list_ctrl_ptr->display_style.is_slide_used
            && UILAYER_IsLayerActive(&list_ctrl_ptr->item_layer))
        {
            // clear lcd rect
            MMITHEME_ClearRect(&lcd_dev_info, &clip_rect);

            if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SELFSET_BG)) // 是否自设置图片
            {
                // 显示图片
                GUI_DisplayBg(&bg_info,
                              &bg_display,
                              &lcd_dev_info);
            }
        }

#endif
    }
}

/*****************************************************************************/
//  Description : draw title
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawTitle(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN         is_draw_all
)
{
    // draw title
    if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_TITLE))
    {
        if (is_draw_all || list_ctrl_ptr->cur_item_idx != list_ctrl_ptr->pre_cur_item_idx)
        {
            GUIWIN_SetTitleBackground(list_ctrl_ptr->win_id, list_ctrl_ptr->display_style.title_bg_image_id, 0);
            GUIWIN_SetTitleFontColor(list_ctrl_ptr->win_id,GUIWIN_GetTitleColor(list_ctrl_ptr->win_id));
            ListDrawTitleSubIndex(list_ctrl_ptr);
            GUIWIN_UpdateTitle(list_ctrl_ptr->win_id);
        }
    }

    return;
}

/*****************************************************************************/
//  Description : 设置索引号信息,显示在title
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetTitleIndexInfo(
    const CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    uint16      cur_idx = 0;
    uint16      total_idx = 0;
    uint16      display_str_len = 0;
    uint8       display_str[10] = {0};
    wchar       display_wstr[10] = {0};
    uint16      i = 0;
    BOOLEAN     is_need_sub_text = TRUE;

    if (NULL == list_ctrl_ptr)
    {
        return;
    }

    if (list_ctrl_ptr->total_item_num > 0)
    {
        if ((list_ctrl_ptr->title_index_type == GUILIST_TITLE_INDEX_EXCLUDE_FIRST))
        {
            cur_idx = (list_ctrl_ptr->cur_item_idx);
            total_idx = list_ctrl_ptr->total_item_num - 1;
        }
        else if ((list_ctrl_ptr->title_index_type == GUILIST_TITLE_INDEX_DEFAULT))
        {
            cur_idx = (list_ctrl_ptr->cur_item_idx + 1);
            total_idx = list_ctrl_ptr->total_item_num;
        }
        else
        {
            is_need_sub_text = FALSE;
        }

        // 格式化cur_num
        sprintf((char *)display_str, "%d", cur_idx);
        display_str_len = strlen((const char *)display_str); /*lint !e64*/

        for (i = 0; i < display_str_len; i++)
        {
            *(display_wstr + i) = *(display_str + i);
        }
    }

    GUIWIN_SetTitleSubText(list_ctrl_ptr->win_id, is_need_sub_text, display_wstr, display_str_len, TRUE);
}

/*****************************************************************************/
//  Description : display list box info
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListDrawTitleSubIndex(const CTRLLIST_OBJ_T *list_ctrl_ptr)
{
    uint16    cur_index = list_ctrl_ptr->cur_item_idx;
    uint16    total_num = list_ctrl_ptr->total_item_num;
    BOOLEAN   is_display = TRUE;

    switch (list_ctrl_ptr->title_index_type)
    {
        case GUILIST_TITLE_INDEX_DEFAULT:
            cur_index++;
            break;

        case GUILIST_TITLE_INDEX_EXCLUDE_FIRST:
            if (total_num > 0)
            {
                total_num--;
            }

            break;

        default:
            is_display = FALSE;
            // 清除titl上的附加字符串信息.
            //GUIWIN_ClearTitleSubTextInfo(list_ctrl_ptr->win_id);  //remove for Bug 119450
            break;
    }

    if (is_display)
    {
        if (MMK_GetWinTitleCtrlId(list_ctrl_ptr->win_id))
        {
            ListSetTitleIndexInfo(list_ctrl_ptr);
        }
    }

    return;
}

/*****************************************************************************/
//  Description : is need scroll bar
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_IsNeedScrollBar(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    result = (BOOLEAN)((list_ctrl_ptr->total_item_num > LIST_GetPageItemNum(list_ctrl_ptr)
#ifdef GUIF_PRGBOX_LETTERSCROLL
                        || GUIPRGBOX_STYLE_LETTERS_SCROLL == list_ctrl_ptr->scroll_bar_type
#endif
                       )
                       && LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_PRGBAR));

    return result;
}

/*****************************************************************************/
//  Description : Draw scroll bar if need
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListDrawSoftkey(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN         is_draw_all
)
{
    //设置skb背景透明
    GUISOFTKEY_IMG_T img_info = {0};
    uint32   left_sk_id   = 0;
    uint32   middle_sk_id = 0;
    uint32   right_sk_id  = 0;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;
    MMITHEME_LIST_STYLE_T *pStyle = PNULL;

    if (PNULL == list_ctrl_ptr || !LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_SOFTKEY | GUILIST_STATE_ACTIVE))
    {
        return;
    }

    pStyle = &list_ctrl_ptr->display_style;
    left_sk_id   = GET_SOFTKEY_CONTENT(pStyle->left_key_type, pStyle->image_null_id, pStyle->text_null_id);
    middle_sk_id = GET_SOFTKEY_CONTENT(pStyle->mid_key_type, pStyle->image_null_id, pStyle->text_null_id);
    right_sk_id  = GET_SOFTKEY_CONTENT(pStyle->right_key_type, pStyle->image_null_id, pStyle->text_null_id);

    //get half transparent softkey image info
    MMITHEME_GetDefaultSoftkeyImg(&img_info);

    //set list softkey font color
    GUIWIN_SetSoftkeyStyle(list_ctrl_ptr->win_id,
                           MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT), GUI_SOFTKEY_BG_COLOR_ONLY, MMITHEME_GetSoftkeyBg(), PNULL);
    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    if (PNULL == item_ptr || PNULL == item_ptr->item.data_ptr)
    {
        return;
    }

    if (CTRLLIST_MARKING_FIXED == list_ctrl_ptr->marking_type)
    {
        if (CTRLLIST_MARK_RADIO == list_ctrl_ptr->mark_type)
        {
            if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_OWN_SOFTKEY))
            {
                left_sk_id   = item_ptr->item.data_ptr->softkey_id[0];
                middle_sk_id = item_ptr->item.data_ptr->softkey_id[1];
                right_sk_id  = item_ptr->item.data_ptr->softkey_id[2];
            }
            else
            {
                left_sk_id = GET_SOFTKEY_CONTENT(pStyle->left_key_type, pStyle->image_ok_id, pStyle->text_ok_id);
                right_sk_id = GET_SOFTKEY_CONTENT(pStyle->right_key_type, pStyle->image_return_id, pStyle->text_return_id);
            }
        }
        else
        {
            if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_OWN_SOFTKEY))
            {
                left_sk_id   = item_ptr->item.data_ptr->softkey_id[0];
                middle_sk_id = item_ptr->item.data_ptr->softkey_id[1];
                right_sk_id  = item_ptr->item.data_ptr->softkey_id[2];

                switch (list_ctrl_ptr->display_style.select_cancel_index)
                {
                    case LEFT_BUTTON:
                        left_sk_id  = (LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
                                      ? GET_SOFTKEY_CONTENT(pStyle->left_key_type, pStyle->image_cancel_id, pStyle->text_cancel_id)
                                      : GET_SOFTKEY_CONTENT(pStyle->left_key_type, pStyle->image_select_id, pStyle->text_select_id);
                        break;

                    case MIDDLE_BUTTON:
                        middle_sk_id  = (LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
                                        ? GET_SOFTKEY_CONTENT(pStyle->mid_key_type, pStyle->image_cancel_id, pStyle->text_cancel_id)
                                        : GET_SOFTKEY_CONTENT(pStyle->mid_key_type, pStyle->image_select_id, pStyle->text_select_id);
                        break;

                    case RIGHT_BUTTON:
                        right_sk_id  = (LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
                                       ? GET_SOFTKEY_CONTENT(pStyle->right_key_type, pStyle->image_cancel_id, pStyle->text_cancel_id)
                                       : GET_SOFTKEY_CONTENT(pStyle->right_key_type, pStyle->image_select_id, pStyle->text_select_id);
                        break;

                    default:
                        SCI_TRACE_LOW("list_ctrl_ptr->display_style.select_cancel_index = %d", list_ctrl_ptr->display_style.select_cancel_index);
                        break;
                }
            }
            else
            {
                left_sk_id  = (LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
                              ? GET_SOFTKEY_CONTENT(pStyle->left_key_type, pStyle->image_cancel_id, pStyle->text_cancel_id)
                              : GET_SOFTKEY_CONTENT(pStyle->left_key_type, pStyle->image_select_id, pStyle->text_select_id);

                right_sk_id = GET_SOFTKEY_CONTENT(pStyle->right_key_type, pStyle->image_return_id, pStyle->text_return_id);
            }
        }
    }
    else
    {
        left_sk_id   = item_ptr->item.data_ptr->softkey_id[0];
        right_sk_id  = item_ptr->item.data_ptr->softkey_id[2];

        if ((LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENABLE_MARK))
            && (0 < ListGetSelectedItem(list_ctrl_ptr, PNULL, PNULL, 0))
            && !LIST_GetItemState(item_ptr, GUIITEM_STATE_DISABLE_MARK))
        {
            //mark状态下, 中间键显示标记\取消
            middle_sk_id  = (LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
                            ? GET_SOFTKEY_CONTENT(pStyle->mid_key_type, pStyle->image_cancel_id, pStyle->text_cancel_id)
                            : GET_SOFTKEY_CONTENT(pStyle->mid_key_type, pStyle->image_select_id, pStyle->text_mark_id);
        }
        else
        {
            middle_sk_id = item_ptr->item.data_ptr->softkey_id[1];
        }
    }

    if (!((left_sk_id == MMITHEME_GetInvalidTextID() || 0 == left_sk_id)
          && (middle_sk_id == MMITHEME_GetInvalidTextID() || 0 == middle_sk_id)
          && (right_sk_id == MMITHEME_GetInvalidTextID() || 0 == right_sk_id)))
    {
        if (is_draw_all
            || left_sk_id != list_ctrl_ptr->pre_left_sk_id
            || middle_sk_id != list_ctrl_ptr->pre_middle_sk_id
            || right_sk_id != list_ctrl_ptr->pre_right_sk_id)
        {
            list_ctrl_ptr->pre_left_sk_id   = left_sk_id;
            list_ctrl_ptr->pre_middle_sk_id = middle_sk_id;
            list_ctrl_ptr->pre_right_sk_id  = right_sk_id;

            //set left softkey
            if (GUILIST_SK_TYPE_TEXT_ID == list_ctrl_ptr->display_style.left_key_type)
            {
                GUIWIN_SeSoftkeytButtonTextId(list_ctrl_ptr->win_id, left_sk_id, 0, TRUE);
            }
            else
            {
                //防止应用没有修改list item设置为text_null 的softkey,导致显示错乱图片
				if (pStyle->text_null_id == left_sk_id)
                {
					left_sk_id = pStyle->image_null_id;
                }
				GUIWIN_SeSoftkeytButtonIconId(list_ctrl_ptr->win_id, left_sk_id, 0, TRUE);
            }

            //set mid softkey
            if (GUILIST_SK_TYPE_TEXT_ID == list_ctrl_ptr->display_style.mid_key_type)
            {
                GUIWIN_SeSoftkeytButtonTextId(list_ctrl_ptr->win_id, middle_sk_id, 1, TRUE);
            }
            else
            {
                //防止应用没有修改list item设置为text_null 的softkey,导致显示错乱图片
				if (pStyle->text_null_id == middle_sk_id)
                {
					middle_sk_id = pStyle->image_null_id;
                }
				GUIWIN_SeSoftkeytButtonIconId(list_ctrl_ptr->win_id, middle_sk_id, 1, TRUE);
            }

            //set right softkey
            if (GUILIST_SK_TYPE_TEXT_ID == list_ctrl_ptr->display_style.right_key_type)
            {
                GUIWIN_SeSoftkeytButtonTextId(list_ctrl_ptr->win_id, right_sk_id, 2, TRUE);
            }
            else
            {
                //防止应用没有修改list item设置为text_null 的softkey,导致显示错乱图片
				if (pStyle->text_null_id == right_sk_id)
                {
					right_sk_id = pStyle->image_null_id;
                }
				GUIWIN_SeSoftkeytButtonIconId(list_ctrl_ptr->win_id, right_sk_id, 2, TRUE);
            }
        }
    }
    else
    {
        if (is_draw_all)
        {
            GUIWIN_UpdateSoftkey(list_ctrl_ptr->win_id);
        }
    }
}

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T ListGetScrollBarRect(
    const CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_RECT_T prgbox_rect = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect;
#ifdef GUIF_PRGBOX_LETTERSCROLL
    uint16    font_width = GUI_GetFontWidth(GUIPRGBOX_LETTER_FONT, 'A');
    uint16    prgbox_space = 0;

    if (GUIPRGBOX_STYLE_LETTERS_SCROLL == list_ctrl_ptr->scroll_bar_type)
    {
        if (MMK_IsWindowLandscape(list_ctrl_ptr->win_id))
        {
            prgbox_space = GUIPRGBOX_SCROLL_SPACE * 3;
        }
        else
        {
            prgbox_space = GUIPRGBOX_SCROLL_SPACE * 2;
        }

        if (font_width + prgbox_space <= prgbox_rect.right)
        {
            prgbox_rect.left = prgbox_rect.right - font_width - prgbox_space;
        }
        else
        {
            prgbox_rect.left = prgbox_rect.right;
        }
    }
    else
#endif
    {
        prgbox_rect.left = (int16)(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.right - MMITHEME_GetScrollBarWidthByState(list_ctrl_ptr->display_style.is_slide_used) + 1 - list_ctrl_ptr->display_style.right_space);
        prgbox_rect.right = (int16)(prgbox_rect.left + MMITHEME_GetScrollBarWidthByState(list_ctrl_ptr->display_style.is_slide_used) - 1);
    }

    GUI_CheckRectDirection(&((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect, &prgbox_rect);

    return prgbox_rect;
}


/*****************************************************************************/
//  Description : get scroll bar both rect
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T ListGetScrollBarBothRect(
    const CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_BOTH_RECT_T both_rect = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->both_rect;
#ifdef GUIF_PRGBOX_LETTERSCROLL
    uint16     font_width = GUI_GetFontWidth(GUIPRGBOX_LETTER_FONT, 'A');

    if (GUIPRGBOX_STYLE_LETTERS_SCROLL == list_ctrl_ptr->scroll_bar_type)
    {
        //v rect
        if (font_width + GUIPRGBOX_SCROLL_SPACE * 2 <= both_rect.v_rect.right)
        {
            both_rect.v_rect.left = both_rect.v_rect.right - font_width - GUIPRGBOX_SCROLL_SPACE * 2;
        }
        else
        {
            both_rect.v_rect.left = both_rect.v_rect.right;
        }

        //h rect
        if (font_width + GUIPRGBOX_SCROLL_SPACE * 3 <= both_rect.h_rect.right)
        {
            both_rect.h_rect.left = both_rect.h_rect.right - font_width - GUIPRGBOX_SCROLL_SPACE * 3;
        }
        else
        {
            both_rect.h_rect.left = both_rect.h_rect.right;
        }
    }
    else
#endif
    {
        both_rect.h_rect.left = (int16)(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->both_rect.h_rect.right - MMITHEME_GetScrollBarWidthByState(list_ctrl_ptr->display_style.is_slide_used) + 1 - list_ctrl_ptr->display_style.right_space);
        both_rect.h_rect.right = (int16)(both_rect.h_rect.left + MMITHEME_GetScrollBarWidthByState(list_ctrl_ptr->display_style.is_slide_used) - 1);
        GUI_CheckRectDirection(&((CTRLBASE_OBJ_T *)list_ctrl_ptr)->both_rect.h_rect, &both_rect.h_rect);
        both_rect.v_rect.left = (int16)(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->both_rect.v_rect.right - MMITHEME_GetScrollBarWidthByState(list_ctrl_ptr->display_style.is_slide_used) + 1 - list_ctrl_ptr->display_style.right_space);
        both_rect.v_rect.right = (int16)(both_rect.v_rect.left  + MMITHEME_GetScrollBarWidthByState(list_ctrl_ptr->display_style.is_slide_used) - 1);
        GUI_CheckRectDirection(&((CTRLBASE_OBJ_T *)list_ctrl_ptr)->both_rect.v_rect, &both_rect.v_rect);

    }

    return both_rect;
}

/*****************************************************************************/
//  Description : sat param
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetScrollBarParam(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    uint32 item_total_height = 0;
    uint32 page_height = 0;
    GUI_RECT_T bar_rect = {0};

    if (PNULL != list_ctrl_ptr && PNULL != list_ctrl_ptr->prgbox_ptr)
    {
        bar_rect = ListGetScrollBarRect(list_ctrl_ptr);

#if defined(GUIF_PRGBOX_AUTOHIDE)

        if (bar_rect.top > bar_rect.bottom)
#else
        if (GUI_IsInvalidRect(bar_rect))
#endif
        {
            return;
        }

        item_total_height = list_ctrl_ptr->total_item_height;
        page_height = bar_rect.bottom - bar_rect.top + 1;
#ifdef GUIF_PRGBOX_LETTERSCROLL

        if (GUIPRGBOX_STYLE_LETTERS_SCROLL == list_ctrl_ptr->scroll_bar_type)
        {
            GUI_BOTH_RECT_T both_rect = {0};
            both_rect = ListGetScrollBarBothRect(list_ctrl_ptr);
            GUIPRGBOX_SetBothRectByPtr(list_ctrl_ptr->prgbox_ptr, &both_rect);
        }

#endif
        GUIPRGBOX_SetParamByPtr(list_ctrl_ptr->prgbox_ptr, item_total_height, page_height);
    }
}

/*****************************************************************************/
//  Description : initial scroll bar
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListInitScrollBar(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_RECT_T bar_rect = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (list_ctrl_ptr->is_need_reinit_scrollbar && PNULL != list_ctrl_ptr->prgbox_ptr)
    {
        MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T *)list_ctrl_ptr->prgbox_ptr));
        list_ctrl_ptr->prgbox_ptr = PNULL;
    }

    if (PNULL == list_ctrl_ptr->prgbox_ptr)
    {
        MMI_CONTROL_CREATE_T create = {0};
        GUIPRGBOX_INIT_DATA_T init_data = {0};

        bar_rect = ListGetScrollBarRect(list_ctrl_ptr);

#if defined(GUIF_PRGBOX_AUTOHIDE)

        if (bar_rect.top > bar_rect.bottom)
#else
        if (GUI_IsInvalidRect(bar_rect))
#endif
        {
            return;
        }

#ifdef GUIF_PRGBOX_LETTERSCROLL

        if (GUIPRGBOX_STYLE_LETTERS_SCROLL == list_ctrl_ptr->scroll_bar_type)
        {
            MMI_HANDLE_T ctrl_handle = 0;
            CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;
            GUI_BOTH_RECT_T both_rect = {0};

            create.ctrl_id           = 0;
            create.guid              = SPRD_GUI_LETTERS_ID;
            create.parent_win_handle = list_ctrl_ptr->win_id;
            create.parent_ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
            create.init_data_ptr     = &init_data;

            base_ctrl_ptr = (CTRLBASE_OBJ_T *)MMK_CreateControl(&create);

            if (PNULL != base_ctrl_ptr)
            {
                ctrl_handle = base_ctrl_ptr->handle;
            }

            list_ctrl_ptr->prgbox_ptr = (IGUICTRL_T *)base_ctrl_ptr;

            both_rect = ListGetScrollBarBothRect(list_ctrl_ptr);

            CTRLLETTERS_SetBothRect(ctrl_handle, &both_rect);
            list_ctrl_ptr->is_need_reinit_scrollbar = FALSE;
        }
        else
#endif
        {
            init_data.is_forbid_paint = TRUE; // 滚动条的刷新完全由list自己控制
            init_data.style = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
            init_data.both_rect.v_rect = init_data.both_rect.h_rect = bar_rect;
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
            init_data.theme_type = list_ctrl_ptr->scroll_bar_theme; 
#else
            init_data.theme_type = MMITHEME_PRGBOX_DEFAULT_TYPE;
#endif
            init_data.lcd_id = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info.lcd_id;

            create.ctrl_id = 0;
            create.guid    = SPRD_GUI_PRGBOX_ID;
            create.parent_win_handle = list_ctrl_ptr->win_id;
            create.parent_ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
            create.init_data_ptr = &init_data;

            list_ctrl_ptr->prgbox_ptr = MMK_CreateControl(&create);

            if (PNULL != list_ctrl_ptr->prgbox_ptr)
            {
                ListSetScrollBarParam(list_ctrl_ptr);

                if (list_ctrl_ptr->display_style.is_slide_used)
                {
                    GUIPRGBOX_SetNeedBg(list_ctrl_ptr->prgbox_ptr, FALSE);
                }
            }
            else
            {
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_4832_112_2_18_3_19_51_210, (uint8 *)"");
            }

            list_ctrl_ptr->is_need_reinit_scrollbar = FALSE;
        }
    }

    return;
}

/*****************************************************************************/
//  Description : Draw scroll bar if need
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListDrawScrollBar(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN is_draw_all
)
{
    int32 offset_y = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    BOOLEAN          is_scoll_update = FALSE;

    if (list_ctrl_ptr != PNULL && LIST_IsNeedScrollBar(list_ctrl_ptr))
    {
        GUI_RECT_T rect = ListGetScrollBarRect(list_ctrl_ptr);

        if (PNULL == list_ctrl_ptr->prgbox_ptr && list_ctrl_ptr->is_need_reinit_scrollbar)
        {
            //如果需要scroll自动刷新的话，第一个创建scroll，需要主动掉用一次scrollbar的update
            is_scoll_update = TRUE;
        }

        ListInitScrollBar(list_ctrl_ptr);

        if (PNULL == list_ctrl_ptr->prgbox_ptr)
        {
            //rect无效创建scroll bar会失败
            return;
        }

        IGUICTRL_SetRect((IGUICTRL_T *)list_ctrl_ptr->prgbox_ptr, &rect);

#ifdef GUIF_PRGBOX_LETTERSCROLL

        if (GUIPRGBOX_STYLE_LETTERS_SCROLL == list_ctrl_ptr->scroll_bar_type)
        {
            GUI_BOTH_RECT_T both_rect = {0};

            //字母滚动条，有scollbar自己刷新
            //由于scrollbar的父控件已经执行过了fullpaint,scrollbar只有在父控件的fullpaint才会刷新
            //所以第一次需要主动刷新scrollbar
            both_rect = ListGetScrollBarBothRect(list_ctrl_ptr);
            CTRLLETTERS_SetBothRect(((CTRLBASE_OBJ_T *)(list_ctrl_ptr->prgbox_ptr))->handle, &both_rect);
            CTRLLETTERS_Update(((CTRLBASE_OBJ_T *)(list_ctrl_ptr->prgbox_ptr))->handle);

            return;
        }

#endif
        lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

        // set lcd dev info
        IGUICTRL_SetLcdDevInfo((IGUICTRL_T *)list_ctrl_ptr->prgbox_ptr, &lcd_dev_info);

        if (is_draw_all
            /*&& lcd_dev_info.block_id == ((CTRLBASE_OBJ_T*)list_ctrl_ptr)->lcd_dev_info.block_id*/)
        {
            ListDrawBackgroundByRect(list_ctrl_ptr, &rect);
        }

        GUIPRGBOX_SetNeedSlideBlock(list_ctrl_ptr->prgbox_ptr, LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_PRGBAR_BLOCK));

        if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SET_PRGBAR_IMG)
            && 0 != list_ctrl_ptr->display_style.prgbar_image_id)
        {
            GUIPRGBOX_SetScrollBarImg(GUICTRL_GetCtrlHandle(list_ctrl_ptr->prgbox_ptr), list_ctrl_ptr->display_style.prgbar_image_id);
        }

        // 设置参数，这里重复设置是为了保证list没有重新创建，直接修改item num之后的显示正确性
        ListSetScrollBarParam(list_ctrl_ptr);

        // 计算offset y相对值，这个offset y 是相对于滚动条的左上角圆点(0,0)
        offset_y = rect.top - list_ctrl_ptr->offset_y;
        offset_y = offset_y > 0 ? offset_y : 0;

        GUIPRGBOX_SetPosByPtr(is_draw_all, offset_y, offset_y, list_ctrl_ptr->prgbox_ptr);
    }
    else if (list_ctrl_ptr != PNULL)
    {
        if (PNULL != list_ctrl_ptr->prgbox_ptr)
        {
            MMK_DestroyControl(GUICTRL_GetCtrlHandle(list_ctrl_ptr->prgbox_ptr));

            list_ctrl_ptr->prgbox_ptr = NULL;
        }
    }
}

/*****************************************************************************/
//  Description : Is text too long
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemTextNeedScroll(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const CTRLLIST_ITEM_INFO_T *item_ptr
)
{
    BOOLEAN result = FALSE;
    GUI_RECT_T item_rect = {0};

    if ((PNULL == list_ctrl_ptr) || (PNULL == item_ptr))
    {
        return FALSE;
    }

    if (PNULL != item_ptr->item.data_ptr)
    {
        THEMELIST_ITEM_STYLE_T   *item_style_ptr  = item_ptr->item.style_ptr;
        GUIITEM_DATA_TYPE_E     item_data_type = GUIITEM_DATA_NONE;

        SCI_ASSERT((item_style_ptr->main_index >= 0 && item_style_ptr->main_index < GUIITEM_CONTENT_MAX_NUM)); /*assert verified*/

        item_data_type = item_ptr->item.data_ptr->item_content[item_style_ptr->main_index].item_data_type;

        if (GUIITEM_DATA_TEXT_ID == item_data_type
            || GUIITEM_DATA_TEXT_BUFFER == item_data_type)
        {
            GUI_RECT_T  layout_rect     = item_ptr->content_rect[item_style_ptr->main_index];
            uint16      text_rect_width = (uint16)(layout_rect.right - layout_rect.left + 1);
            wchar       *str_ptr        = PNULL;
            uint16      str_len         = 0;

            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, 0);
            // 这里之所以要这么样计算,是因为当list控件不是全屏宽度的时候,取theme中的数值就有可能会出错
            text_rect_width = MIN(text_rect_width, (item_rect.right - item_rect.left + 1));

            if (GUIITEM_DATA_TEXT_ID == item_data_type)
            {
                MMI_STRING_T temp_str = {0};

                MMITHEME_GetResText(item_ptr->item.data_ptr->item_content[ item_style_ptr->main_index ].item_data.text_id, list_ctrl_ptr->win_id, &temp_str);

                str_ptr = temp_str.wstr_ptr;
                str_len = temp_str.wstr_len;
            }
            else
            {
                str_ptr = item_ptr->item.data_ptr->item_content[ item_style_ptr->main_index ].item_data.text_buffer.wstr_ptr;
                str_len = item_ptr->item.data_ptr->item_content[ item_style_ptr->main_index ].item_data.text_buffer.wstr_len;
            }

            if (text_rect_width < ListCalculateStringPiexlNum(list_ctrl_ptr, str_ptr, str_len, list_ctrl_ptr->cur_item_idx, item_style_ptr->main_index))
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Is text too long
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint16 ListGetCurItemTextRectWidth(
    CTRLLIST_OBJ_T          *list_ctrl_ptr,
    const CTRLLIST_ITEM_INFO_T    *item_ptr
)
{
    GUI_RECT_T item_rect = {0};
    uint16     text_rect_width = 0;

    if ((PNULL == list_ctrl_ptr) || (PNULL == item_ptr))
    {
        return text_rect_width;
    }

    if (PNULL != item_ptr->item.data_ptr)
    {
        THEMELIST_ITEM_STYLE_T *item_style_ptr  = item_ptr->item.style_ptr;
        GUIITEM_DATA_TYPE_E     item_data_type = GUIITEM_DATA_NONE;

        SCI_ASSERT((item_style_ptr->main_index >= 0 && item_style_ptr->main_index < GUIITEM_CONTENT_MAX_NUM)); /*assert verified*/

        item_data_type = item_ptr->item.data_ptr->item_content[item_style_ptr->main_index].item_data_type;

        if (GUIITEM_DATA_TEXT_ID == item_data_type
            || GUIITEM_DATA_TEXT_BUFFER == item_data_type)
        {
            GUI_RECT_T layout_rect = item_ptr->content_rect[item_style_ptr->main_index];

            text_rect_width = (uint16)(layout_rect.right - layout_rect.left + 1);

            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, 0);
            // 这里之所以要这么样计算,是因为当list控件不是全屏宽度的时候,取theme中的数值就有可能会出错
            text_rect_width = MIN(text_rect_width, (item_rect.right - item_rect.left + 1));
        }
    }

    return text_rect_width;
}

/*****************************************************************************/
//  Description : draw the current list item when timer expired
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ListUpdateCurItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_RECT_T          item_rect       = {0};
    GUI_RECT_T          inter_rect      = {0};
    CTRLLIST_ITEM_INFO_T *item_ptr  = PNULL;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    if (PNULL != item_ptr
        && PNULL != item_ptr->item.data_ptr)
    {
        THEMELIST_ITEM_STYLE_T *item_style_ptr = item_ptr->item.style_ptr;
        uint16                  main_index     = 0;
        GUIITEM_DATA_TYPE_E     item_data_type = GUIITEM_DATA_NONE;

        SCI_ASSERT((item_style_ptr->main_index >= 0 && item_style_ptr->main_index < GUIITEM_CONTENT_MAX_NUM)); /*assert verified*/

        main_index = item_style_ptr->main_index;
        item_data_type = item_ptr->item.data_ptr->item_content[main_index].item_data_type;

        if (GUIITEM_DATA_TEXT_ID == item_data_type
            || GUIITEM_DATA_TEXT_BUFFER == item_data_type)
        {
            MMI_STRING_T text_info = {0};

            /* need item data */
            if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA) && PNULL == item_ptr->item.data_ptr)
            {
                LIST_SendNeedItemDataMsg(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
            }

            LIST_GetListItemContentTextInfo(list_ctrl_ptr, item_ptr, main_index, &text_info);

            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

            if (!GUI_IntersectRect(&inter_rect, item_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect))
            {
                return;
            }

            if (MMITHEME_GetIsScrollAsPixel())
            {
                uint16                       text_rect_width = 0;
                uint16                       text_pixel_width = 0;

                text_rect_width = ListGetCurItemTextRectWidth(list_ctrl_ptr, item_ptr);
                text_pixel_width = ListCalculateStringPiexlNum(list_ctrl_ptr, text_info.wstr_ptr, text_info.wstr_len, list_ctrl_ptr->cur_item_idx, main_index);

                if (text_pixel_width - 1 - list_ctrl_ptr->str_start_pos >  text_rect_width)
                {
                    list_ctrl_ptr->str_start_pos += MMITHEME_ScrollPixelNum();
                }
                else
                {
                    list_ctrl_ptr->str_start_pos = 0;
                }
            }
            else if (0 != text_info.wstr_len)
            {
                list_ctrl_ptr->str_start_pos++;

                list_ctrl_ptr->str_start_pos %= text_info.wstr_len;
            }
            else
            {
                list_ctrl_ptr->str_start_pos = 0;
            }

            // 得到lcd info
            lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

            ListDrawHighlightBackground(list_ctrl_ptr, &item_rect, &lcd_dev_info);
            // 在高亮背景上重画item
            ListDrawItem(list_ctrl_ptr, &lcd_dev_info, list_ctrl_ptr->cur_item_idx);
        }
    }
}


/*****************************************************************************/
//  Description : Stop text animation
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListStopTextAnim(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL != list_ctrl_ptr)
    {
        if (0 != list_ctrl_ptr->text_scroll_timer_id)
        {
            if (MMK_StopTimer(list_ctrl_ptr->text_scroll_timer_id))
            {
                list_ctrl_ptr->str_start_pos = 0;
                list_ctrl_ptr->text_scroll_timer_id = 0;
                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Set the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemStateById(
    MMI_CTRL_ID_T   ctrl_id,
    uint32          item_index,
    GUIITEM_STATE_T src_state,
    BOOLEAN is_true
)
{
    BOOLEAN             result = FALSE;
    CTRLLIST_OBJ_T      *list_ctrl_ptr = PNULL;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    //get list ptr
    list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

        if (PNULL != item_ptr)
        {
            // 设置新的状态
            LIST_SetState(&item_ptr->item.state, src_state, is_true);

            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 设置Item是否灰化
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemGray(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index,
    BOOLEAN is_gray
)
{
    BOOLEAN             result = FALSE;
    CTRLLIST_OBJ_T      *list_ctrl_ptr = PNULL;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    //get list ptr
    list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

        if (PNULL != item_ptr)
        {
            // 设置新的状态
            LIST_SetItemState(item_ptr, GUIITEM_STATE_GRAY, is_gray);
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : set the list state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SetListStateByPtr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUILIST_STATE_T    src_state,
    BOOLEAN is_true
)
{
    if (PNULL != list_ctrl_ptr)
    {
        LIST_SetState(&list_ctrl_ptr->list_state, src_state, is_true);
    }
}

/*****************************************************************************/
//  Description : get the list state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetListStateByPtr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUILIST_STATE_T src_state
)
{
    BOOLEAN result = FALSE;

    if (PNULL != list_ctrl_ptr)
    {
        result = LIST_GetState(list_ctrl_ptr->list_state, src_state);
    }

    return result;
}

/*****************************************************************************/
//  Description : set the list state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetListState(
    MMI_CTRL_ID_T   ctrl_id,
    GUILIST_STATE_T    src_state,
    BOOLEAN is_true
)
{
    LIST_SetListStateByPtr(ListGetPtr(ctrl_id), src_state, is_true);
}

/*****************************************************************************/
//  Description : get the list state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetListState(
    MMI_CTRL_ID_T   ctrl_id,
    GUILIST_STATE_T src_state
)
{
    return  LIST_GetListStateByPtr(ListGetPtr(ctrl_id), src_state);
}

/*****************************************************************************/
//  Description : get the list state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetListStateById(
    MMI_CTRL_ID_T   ctrl_id,
    GUILIST_STATE_T src_state
)
{
    BOOLEAN             result = FALSE;
    CTRLLIST_OBJ_T      *list_ctrl_ptr = PNULL;

    //get ctrl ptr
    list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        result = LIST_GetState(list_ctrl_ptr->list_state, src_state);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get the item num by height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 ListGetItemTotalHeightByIdx(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16              start_index,
    uint16              end_index
)
{
    uint32    total_item_height = 0;

    if (PNULL != list_ctrl_ptr && list_ctrl_ptr->total_item_num > 0)
    {
        uint16    i      = 0;
        uint16    offset = ListGetOffsetofTowItem(start_index, end_index, list_ctrl_ptr->total_item_num);

        for (i = start_index; i <= start_index + offset; i++)
        {
            total_item_height = (uint32)(total_item_height + LIST_GetItemHeight(list_ctrl_ptr, (uint16)(i % list_ctrl_ptr->total_item_num)));
        }
    }

    return total_item_height;
}

/*****************************************************************************/
//  Description : Get offset between current item index and top item index
//  Global resource dependence :
//    Author:
//  Note:
/*****************************************************************************/
LOCAL  uint16 ListGetOffsetofTowItem(
    uint16 start_idx,
    uint16 end_idx,
    uint16 total_num
)
{
    uint16 offset = 0;

    if (total_num > 0)
    {
        offset = (uint16)((total_num + end_idx - start_idx) % total_num);
    }

    return offset;
}

/*****************************************************************************/
//  Description : post list slide state
//  Global resource dependence :
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void ListPostSlideStateMsg(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUILIST_SLIDE_STATE_E slide_state
)
{
    GUILIST_SLIDE_STATE_E  state = slide_state;

    if (PNULL != list_ctrl_ptr)
    {
        GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_CTL_LIST_SLIDE_STATE_CHANGE, &state, sizeof(GUILIST_SLIDE_STATE_E));
    }
}

/*****************************************************************************/
//  Description : post mark all
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListSendMarkAllItemMsg(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN               is_selected
)
{
    if (PNULL != list_ctrl_ptr)
    {
        GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_SELECTEDALL, &is_selected);
    }
}

/*****************************************************************************/
//  Description : move to next page item
//  Global resource dependence :
//  Author:kelly.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListMoveToNextPageItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    BOOLEAN         is_next
)
{
    int32 offset_y = 0;
    BOOLEAN result = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (list_ctrl_ptr->total_item_num > 0)
    {
        int32 page_num = LIST_GetPageItemNum(list_ctrl_ptr);
        int32 cur_item_index = 0;

        list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
        list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;
        offset_y = list_ctrl_ptr->offset_y;

        if (is_next)
        {
            if (list_ctrl_ptr->cur_item_idx == list_ctrl_ptr->total_item_num - 1) // 当切换到底部时循环
            {
                cur_item_index = 0;

                list_ctrl_ptr->cur_item_idx = cur_item_index;
                LIST_ResetItemHeight(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx, cur_item_index);
                LIST_ResetOffsetYByTopIndexExt(list_ctrl_ptr, cur_item_index);

                offset_y = list_ctrl_ptr->offset_y - offset_y;

                LIST_ResetTopItemAndEndItemIndexExt(list_ctrl_ptr, offset_y);

                list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;
                LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);
                ListValidateCtrl(list_ctrl_ptr , FALSE, FALSE);
                LIST_DestroyAllItemDataNotInPage(list_ctrl_ptr, TRUE);
                result = TRUE;
            }
            else // 正常切换
            {
                cur_item_index = list_ctrl_ptr->cur_item_idx + page_num;

                if (cur_item_index >= list_ctrl_ptr->total_item_num)
                {
                    cur_item_index = list_ctrl_ptr->total_item_num - 1;
                }

                if (cur_item_index != list_ctrl_ptr->cur_item_idx)
                {
                    //set top item index
                    list_ctrl_ptr->top_item_idx = list_ctrl_ptr->cur_item_idx = cur_item_index;

                    LIST_ResetItemHeight(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx, list_ctrl_ptr->cur_item_idx);

                    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);
                    ListValidateCtrl(list_ctrl_ptr , FALSE, TRUE);
                    LIST_DestroyAllItemData(list_ctrl_ptr, TRUE);
                    result = TRUE;
                }

                offset_y = list_ctrl_ptr->offset_y;

                if (IsListItemVisible(list_ctrl_ptr, list_ctrl_ptr->total_item_num - 1))
                {
                    LIST_ResetOffsetYByEndIndex(list_ctrl_ptr, list_ctrl_ptr->total_item_height);
                    LIST_ResetTopItemAndEndItemIndexExt(list_ctrl_ptr, list_ctrl_ptr->offset_y - offset_y);
                }
            }
        }
        else
        {
            if (0 == list_ctrl_ptr->cur_item_idx) // 切换到顶部时循环
            {
                list_ctrl_ptr->cur_item_idx = cur_item_index = list_ctrl_ptr->total_item_num - 1;
                LIST_ResetItemHeight(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx, list_ctrl_ptr->cur_item_idx);

                LIST_ResetOffsetYByEndIndex(list_ctrl_ptr, list_ctrl_ptr->total_item_height);
                LIST_ResetTopItemAndEndItemIndexExt(list_ctrl_ptr, list_ctrl_ptr->offset_y - offset_y);
                list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;

                LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);
                ListValidateCtrl(list_ctrl_ptr , FALSE, FALSE);
                LIST_DestroyAllItemDataNotInPage(list_ctrl_ptr, TRUE);
                result = TRUE;
            }
            else // 正常切换
            {
                cur_item_index = list_ctrl_ptr->cur_item_idx - page_num;

                if (cur_item_index < 0)
                {
                    cur_item_index = 0;
                }

                if (cur_item_index != list_ctrl_ptr->cur_item_idx)
                {
                    //set top item index
                    list_ctrl_ptr->top_item_idx = list_ctrl_ptr->cur_item_idx = cur_item_index;

                    LIST_ResetItemHeight(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx, list_ctrl_ptr->cur_item_idx);

                    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);
                    ListValidateCtrl(list_ctrl_ptr , FALSE, TRUE);
                    LIST_DestroyAllItemData(list_ctrl_ptr, TRUE);
                    result = TRUE;
                }
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : move to prev item
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListToPrevItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    int32 offset_y = 0;
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T inter_rect = {0};
    uint16 top_item_idx = 0;
    uint16 end_item_idx = 0;
    uint16 cur_item_index = 0;
    uint16 page_item_num  = 0;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (list_ctrl_ptr->total_item_num > 0)
    {
        /* keep old data */
        cur_item_index = list_ctrl_ptr->cur_item_idx;
        top_item_idx = list_ctrl_ptr->top_item_idx;
        end_item_idx = list_ctrl_ptr->end_item_idx;

        // the current item is the top display item
        if ((cur_item_index == list_ctrl_ptr->top_item_idx))
        {
            //set top item index & top item ptr
            if (cur_item_index == 0)
            {
                list_ctrl_ptr->cur_item_idx = (uint16)(list_ctrl_ptr->total_item_num - 1);

                // 重新设置item 高度
                LIST_ResetItemHeight(list_ctrl_ptr, cur_item_index, list_ctrl_ptr->cur_item_idx);

                // reset offset Y
                LIST_ResetOffsetYByEndIndex(list_ctrl_ptr, list_ctrl_ptr->total_item_height);

                // reset top and end item index
                LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);

                LIST_DestroyAllItemDataNotInPageExt(list_ctrl_ptr, top_item_idx, end_item_idx);
            }
            else
            {
                list_ctrl_ptr->cur_item_idx--;

                page_item_num = list_ctrl_ptr->end_item_idx - list_ctrl_ptr->top_item_idx + 1;

                if (LIST_DestroyItemData(list_ctrl_ptr, LIST_GetItemInfoPtr(list_ctrl_ptr, (uint16)(list_ctrl_ptr->top_item_idx + page_item_num - 1)), TRUE))
                {
                    //终止当前正在解码的item
                    LIST_DestroyItemDecode(list_ctrl_ptr);
                }

                // 重新设置item 高度
                LIST_ResetItemHeight(list_ctrl_ptr, cur_item_index, list_ctrl_ptr->cur_item_idx);

                // 计算总偏移量
                //                 list_ctrl_ptr->offset_y += LIST_GetItemHeight(list_ctrl_ptr, /*list_ctrl_ptr->cur_item_idx*/list_ctrl_ptr->top_item_idx);
                LIST_ResetOffsetYByTopIndexExt(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

                // 重新设置top和end
                LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
            }
        }
        else
        {
            if (0 == list_ctrl_ptr->cur_item_idx)
            {
                list_ctrl_ptr->cur_item_idx = list_ctrl_ptr->total_item_num - 1;
            }
            else
            {
                list_ctrl_ptr->cur_item_idx--;
            }

            // 重新设置item 高度
            LIST_ResetItemHeight(list_ctrl_ptr, cur_item_index, list_ctrl_ptr->cur_item_idx);

            // 获得区域
            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

            //计算交集
            if (GUI_IntersectRect(&inter_rect, item_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect)
                && !GUI_RectIsCovered(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect, item_rect)) // 有交集，且不完全在list范围内
            {
                offset_y = inter_rect.top - item_rect.top;

                list_ctrl_ptr->offset_y += offset_y;

                LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description : move to next item
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListToNextItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    int32 offset_y = 0;
    uint16 top_item_idx = 0;
    uint16 end_item_idx = 0;
    uint16 cur_item_index = 0;
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T inter_rect = {0};
    GUI_RECT_T list_rect = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    if (list_ctrl_ptr->total_item_num > 0)
    {
        /* backup old info */
        cur_item_index = list_ctrl_ptr->cur_item_idx;
        top_item_idx = list_ctrl_ptr->top_item_idx;
        end_item_idx = list_ctrl_ptr->end_item_idx;

        // the pre current item is the end display item
        if ((cur_item_index == list_ctrl_ptr->end_item_idx))
        {
            // set the top item
            if (cur_item_index == list_ctrl_ptr->total_item_num - 1)   // 最后一个
            {
                list_ctrl_ptr->top_item_idx = 0;
                list_ctrl_ptr->cur_item_idx = 0;

                // 重新设置offset
                list_ctrl_ptr->offset_y = list_rect.top;

                // 重新设置item 高度
                LIST_ResetItemHeight(list_ctrl_ptr, cur_item_index, list_ctrl_ptr->cur_item_idx);

                // 重新设置top 和 end index
                LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);

                LIST_DestroyAllItemDataNotInPageExt(list_ctrl_ptr, top_item_idx, end_item_idx);
            }
            else
            {
                list_ctrl_ptr->cur_item_idx++;
                list_ctrl_ptr->cur_item_idx %= list_ctrl_ptr->total_item_num;

                // 重置item 高度
                LIST_ResetItemHeight(list_ctrl_ptr, cur_item_index, list_ctrl_ptr->cur_item_idx);

                /* 高度变化后重新计算itemtop */
                LIST_ResetItemPostion(list_ctrl_ptr);

                // 获得end item 区域
                LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->end_item_idx);

                // 计算交集
                if (GUI_IntersectRect(&inter_rect, item_rect, list_rect)) // 有交集，不需要释放
                {
                    // 计算需要向上的offset
                    offset_y = inter_rect.bottom - item_rect.bottom;
                }

                // 计算总偏移量
                /*
                 * 这里不应该增加item height高度的offset 
                 * 考虑previtem高度如果变化, curr实际上会预先有部分区域进入list rect
                 * 应该加上curr bottom相对于list bottom的offset
                 */
                LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
                list_ctrl_ptr->offset_y += offset_y - (int32)(item_rect.bottom - list_rect.bottom);
//                list_ctrl_ptr->offset_y += offset_y - (int32)LIST_GetItemHeight(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

                // 重新设置top和end
                LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);

                if (top_item_idx < list_ctrl_ptr->top_item_idx || top_item_idx > list_ctrl_ptr->end_item_idx)
                {
                    if (LIST_DestroyItemData(list_ctrl_ptr, LIST_GetItemInfoPtr(list_ctrl_ptr, top_item_idx), TRUE))
                    {
                        //终止当前正在解码的item
                        LIST_DestroyItemDecode(list_ctrl_ptr);
                    }
                }
            }
        }
        else
        {
            list_ctrl_ptr->cur_item_idx++;
            list_ctrl_ptr->cur_item_idx %= list_ctrl_ptr->total_item_num;

            // 重新设置item 高度
            LIST_ResetItemHeight(list_ctrl_ptr, cur_item_index, list_ctrl_ptr->cur_item_idx);

            /* 高度变化后重新计算itemtop */
            LIST_ResetItemPostion(list_ctrl_ptr);

            // 获得区域
            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

            //计算交集
            if (GUI_IntersectRect(&inter_rect, item_rect, list_rect)
                && !GUI_RectIsCovered(list_rect, item_rect)) // 有交集，且不完全在list范围内
            {
                offset_y = inter_rect.bottom - item_rect.bottom;

                list_ctrl_ptr->offset_y += offset_y;

                LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
            }
        }
    }
}

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
/*****************************************************************************/
//  Description : save previous item for 3D
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListSavePrevItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          prev_item_index,
    uint16          cur_item_index,
    GUI_RECT_T      prev_rect
)
{
    if (MMITHEME_IsNeed3DList()
        && !GUI_IsInvalidRect(prev_rect)
        && LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
    {
        //set buffer size
        uint32 buf_size = (prev_rect.right - prev_rect.left + 1) * (prev_rect.bottom - prev_rect.top + 1) * (int16)sizeof(uint16);

        //alloc
        MMITHEME_Alloc3DListBuf(buf_size, list_ctrl_ptr->win_id);

        //save previous item
        MMITHEME_Save3DListBuf(GUILIST_PREV_BUF, prev_rect);

        //set icon move direction
        MMITHEME_Set3DListDirection((BOOLEAN)(cur_item_index > prev_item_index));
    }
}

/*****************************************************************************/
//  Description : save previous item bg for 3D
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListSavePrevItemBg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T    prev_rect
)
{
    if (MMITHEME_IsNeed3DList()
        && !GUI_IsInvalidRect(prev_rect)
        && LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
    {
        MMITHEME_Save3DListBuf(GUILIST_PREV_BG_BUF, prev_rect);
    }
}

/*****************************************************************************/
//  Description : save next item for 3D
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListSaveNextItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T    prev_rect
)
{
    GUI_RECT_T list_rect = {0};

    if (MMITHEME_IsNeed3DList()
        && !GUI_IsInvalidRect(prev_rect)
        && LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
    {
        GUI_RECT_T cur_rect = {0};

        LIST_CalculateListItemRect(&cur_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        ListDrawHighlightItem(list_ctrl_ptr);

        list_rect = LIST_AdjustRect(list_ctrl_ptr);

        GUI_IntersectRect(&cur_rect, list_rect, cur_rect);

        //避免底层assert
        if (cur_rect.bottom - cur_rect.top != prev_rect.bottom - prev_rect.top)
        {
            //cur_rect.bottom = cur_rect.top + prev_rect.bottom - prev_rect.top;
            //free menu buffer
            MMITHEME_Free3DListBuf();
            return;
        }

        // left和right一定保证相同，否则后面做不了动画
        cur_rect.left = prev_rect.left;
        cur_rect.right = prev_rect.right;

        //save icon
        MMITHEME_Save3DListBuf(GUILIST_NEXT_BUF, cur_rect);

        ListRedrawItem(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        MMITHEME_Start3DListAnim();
    }
}
#endif

/*****************************************************************************/
//  Description : move to next item, 有滑动效果
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListMoveToNextItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,            // 控件指针
    CTRLLIST_DIRECTION_KEY_E direct_type, // 移动方向
    BOOLEAN is_support_3d,               // 是否3d
    BOOLEAN is_tp_move_item,             // 是否tp移动
    uint16 item_num                      // 移动数
)
{
    uint16  i = 0;
    int32   offset_y = 0;
    BOOLEAN is_next = FALSE;
    CTRLLIST_ITEM_INFO_T *pre_cur_item_info_ptr = PNULL;
    CTRLLIST_ITEM_INFO_T *cur_item_info_ptr     = PNULL;
    GUI_RECT_T pre_cur_rect = {0};
    GUI_RECT_T list_rect = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    LIST_CalculateListItemRect(&pre_cur_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    ListStopTextAnim(list_ctrl_ptr);

    list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
    list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;

    switch (direct_type)
    {
        case CTRLLIST_DIRECTION_KEY_DOWN:
            for (i = 0; i < item_num; i++)
            {
                ListToNextItem(list_ctrl_ptr);

                if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SET_CHECK_AUTO))
                {
                    ListSetSelectedItemByPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, TRUE);
                }
            }

            is_next = TRUE;
            break;

        case CTRLLIST_DIRECTION_KEY_UP:
            for (i = 0; i < item_num; i++)
            {
                ListToPrevItem(list_ctrl_ptr);

                if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SET_CHECK_AUTO))
                {
                    ListSetSelectedItemByPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, TRUE);
                }
            }

            break;

        default:
            break;
    }

    is_next = is_tp_move_item ? FALSE : is_next; // 如果执行TP消息，那么一定为FALSE，否则根据上述运算获得is_next

    if (list_ctrl_ptr->slide_state == MMK_TP_SLIDE_FLING)
    {
        // fling状态不画滚动条
        return;
    }

    // 修正offset 值
    ListReviceDrawItem(list_ctrl_ptr, &offset_y, is_next);

    list_ctrl_ptr->offset_y += offset_y;

    // 满足下列条件需要把分隔符优先显示出来
    if (CTRLLIST_DIRECTION_KEY_UP == direct_type && // 向上移动
        !is_tp_move_item && // 执行按键操作
        list_ctrl_ptr->cur_item_idx > 0 && // 当前值大于0
        LIST_GetItemStateByIndex(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx - 1, GUIITEM_STATE_SPLIT))  // 前一个是分隔符
    {
        GUI_RECT_T pre_item_rect = {0};
        // 获取区域
        LIST_CalculateListItemRect(&pre_item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx - 1);

        // 不被完全覆盖
        if (!GUI_RectIsCovered(list_rect, pre_item_rect))
        {
            LIST_ResetOffsetYByTopIndexExt(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx - 1);
            LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
        }
    }

    is_support_3d = ((MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())) ? TRUE : is_support_3d;

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT

    if (is_support_3d
        && GUI_RectIsCovered(list_rect, pre_cur_rect)
        && !list_ctrl_ptr->is_disable_3dmmi)
    {
        ListSavePrevItem(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx, list_ctrl_ptr->cur_item_idx, pre_cur_rect);

        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_ON_3D, TRUE);
    }

#endif

    if (MMK_IsIncludeTips(list_ctrl_ptr->win_id) && GUITIPS_GetTipsValid(list_ctrl_ptr->win_id))
    {
        // validate list
        ListValidateCtrl(list_ctrl_ptr , FALSE, FALSE);
        ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
    }
    else
    {
        if (list_ctrl_ptr->pre_top_item_idx == list_ctrl_ptr->top_item_idx)
        {
            BOOLEAN is_cur_expand = FALSE;
            BOOLEAN is_pre_expand = FALSE;

            pre_cur_item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx);
            cur_item_info_ptr     = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

            if (PNULL != pre_cur_item_info_ptr && PNULL != cur_item_info_ptr)
            {
                is_cur_expand = (pre_cur_item_info_ptr->item.style_ptr->height == pre_cur_item_info_ptr->item.style_ptr->height_focus);
                is_pre_expand = (cur_item_info_ptr->item.style_ptr->height == cur_item_info_ptr->item.style_ptr->height_focus);
            }

            // when style is GUIITEM_STYLE_STATE_EXPAND, redraw all items between
            // pre-cur-item and cur-item.
            if (is_cur_expand != is_pre_expand || GUI_IsInvalidRect(pre_cur_rect))
            {
                //扩展和非扩展item混排, 必须刷新整个list, 否则会导致高亮条清除不干净
                ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_SK | INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_TITLE | INNER_STATE_UPDATE_PRG);
            }
            else
            {
                ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
            }
        }
        else
        {
            ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
        }
    }

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT

    if (is_support_3d &&
        GUI_RectIsCovered(list_rect, pre_cur_rect)
        && !list_ctrl_ptr->is_disable_3dmmi)
    {
        //save previous item bg
        ListSavePrevItemBg(list_ctrl_ptr, pre_cur_rect);

        //save next item
        ListSaveNextItem(list_ctrl_ptr, pre_cur_rect);

        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_ON_3D, FALSE);
    }

#endif

    ListUpdateHighlightItem(list_ctrl_ptr);
}

/*****************************************************************************/
//  Description : redraw item
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListRedrawItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          index
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (list_ctrl_ptr->total_item_num > 0)
    {
        GUI_RECT_T  item_rect  = {0};
        CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;//

        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

        if (PNULL == item_ptr)
        {
            return;
        }

        /* need item data */
        if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA) && PNULL == item_ptr->item.data_ptr)
        {
            LIST_SendNeedItemDataMsg(list_ctrl_ptr, index);
        }

        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, index);

        ListDrawBackgroundByRect(list_ctrl_ptr, &item_rect);

        lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

        ListDrawItem(list_ctrl_ptr, &lcd_dev_info, index);
    }
}


/*****************************************************************************/
//  Description : handle the key left message
//  Global resource dependence :
//  Author:kelly.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyLeft(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (list_ctrl_ptr->total_item_num > 0)
    {
        result = ListMoveToNextPageItem(list_ctrl_ptr, FALSE);

        if (0 == list_ctrl_ptr->cur_item_idx)
        {
            GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVETOP);
        }
        else
        {
            GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_PREPAGE);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : handle the key right  message
//  Global resource dependence :
//  Author:kelly.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyRight(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (list_ctrl_ptr->total_item_num > 0)
    {
        result = ListMoveToNextPageItem(list_ctrl_ptr, TRUE);

        if (list_ctrl_ptr->total_item_num - 1 == list_ctrl_ptr->cur_item_idx)
        {
            GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVEBOTTOM);
        }
        else
        {
            GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_NXTPAGE);
        }
    }

    return result;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 处理滑动效果
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleItemSlide(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
)
{
    BOOLEAN result = FALSE;
    int16 x_offset = 0;

    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SLIDE_ITEM)
        && !LIST_GetItemStateByIndex(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, (GUIITEM_STATE_NOTHOR_SLIDE)))
    {
        GUI_RECT_T                  item_rect   = {0};
        GUI_LCD_DEV_INFO            item_lcd    = {0};
        UILAYER_APPEND_BLT_T        append_layer = {0};

        // 获取当前item区域
        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        x_offset = tp_point_ptr->x - list_ctrl_ptr->slide_start_point.x;

        GUI_CheckRectValid(item_rect);

        // append
        item_lcd = LIST_GetItemDispLayer();
        UILAYER_SetLayerPosition(&item_lcd, item_rect.left + x_offset, item_rect.top);
        append_layer.lcd_dev_info = item_lcd;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

        UILAYER_ClearRect(&list_ctrl_ptr->item_layer, item_rect);

        LIST_DisplayItemSlideColor(list_ctrl_ptr, tp_point_ptr, item_rect);

        LIST_DrawItemSlideData(list_ctrl_ptr, tp_point_ptr, item_rect);

        result = TRUE;
    }

    return result;
}
#endif


/*****************************************************************************/
//  Description : handle list timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListHandleTimer(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    DPARAM           param          //附带的处理参数
)
{
#ifdef TOUCH_PANEL_SUPPORT //xiyuan edit for pclint
    GUI_POINT_T         point          = {0};
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;
#endif
    MMI_RESULT_E        result         = MMI_RESULT_TRUE;
    uint8               timer_id = *(uint8 *)param;

    if (timer_id == list_ctrl_ptr->text_scroll_timer_id)
    {
        list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;
        list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
        MMK_StopTimer(list_ctrl_ptr->text_scroll_timer_id);
        list_ctrl_ptr->text_scroll_timer_id = 0;

        //only update the current item
        if (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state
            && LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ACTIVE | GUILIST_STATE_FOCUS | GUILIST_STATE_TEXTSCROLL_ENABLE))
        {
            ListUpdateCurItem(list_ctrl_ptr);
            list_ctrl_ptr->text_scroll_timer_id = MMK_CreateWinTimer(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, list_ctrl_ptr->display_style.text_time, FALSE);
        }
    }

#ifdef TOUCH_PANEL_SUPPORT
    else if (timer_id == list_ctrl_ptr->tp_move_timer_id)
    {
        MMI_TP_STATUS_E state = MMI_TP_NONE;


        ListStopTpMoveTimer(list_ctrl_ptr);

        if (MMK_IsFocusWin(list_ctrl_ptr->win_id)
            && list_ctrl_ptr->display_style.is_slide_used)
        {
            ListStartTpMoveTimer(list_ctrl_ptr);

            MMK_GetLogicTPMsg(&state, &point);

            if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_SLIDE_ITEM)
                && ListHandleItemSlide(list_ctrl_ptr, &point))
            {
                //SCI_TRACE_LOW:"ListHandleItemSlide first!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_7163_112_2_18_3_19_56_212, (uint8 *)"");
            }
            else if (MMK_TP_SLIDE_NONE != list_ctrl_ptr->slide_state)
            {
                if (!list_ctrl_ptr->is_tp_down || abs(list_ctrl_ptr->slide_pre_point.y - point.y) > 3)
                {
                    ListHandleSlide(list_ctrl_ptr, &point);

                    list_ctrl_ptr->slide_pre_point.x = point.x;
                    list_ctrl_ptr->slide_pre_point.y = point.y;

                    VTLBASE_SetSlideState((CTRLBASE_OBJ_T *)list_ctrl_ptr, TRUE);
                }
            }
        }
        else
        {
            ListStopFling(list_ctrl_ptr);
        }
    }
    else if (timer_id == list_ctrl_ptr->tp_timer_id)
    {
        MMIDEFAULT_TurnOnBackLight();
        ListStopTextAnim(list_ctrl_ptr); //must stop the text timer when move

        list_ctrl_ptr->move_item_num = 1;

        if (CONTINUE_MOVE_DOWN == list_ctrl_ptr->continue_move_type)
        {
            if (list_ctrl_ptr->cur_item_idx < list_ctrl_ptr->total_item_num - 1)
            {
                list_ctrl_ptr->move_item_num = LIST_CalculateMoveItemNum(list_ctrl_ptr, CTRLLIST_DIRECTION_KEY_DOWN);
                ListHandleKeyDown(list_ctrl_ptr);
            }
            else
            {
                ListStopTpTimer(list_ctrl_ptr);
            }
        }
        else if (CONTINUE_MOVE_UP == list_ctrl_ptr->continue_move_type)
        {
            if (0 < list_ctrl_ptr->cur_item_idx)
            {
                list_ctrl_ptr->move_item_num = LIST_CalculateMoveItemNum(list_ctrl_ptr, CTRLLIST_DIRECTION_KEY_UP);
                ListHandleKeyUp(list_ctrl_ptr);
            }
            else
            {
                ListStopTpTimer(list_ctrl_ptr);
            }
        }
        else
        {
            SCI_PASSERT(FALSE, ("ListCtrlHandleMsg: the move_type is error!")); /*assert verified*/
        }

        //设置当前项的状态
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        if (PNULL != item_ptr)
        {
            ListSetItemMarkStatus(list_ctrl_ptr, item_ptr, list_ctrl_ptr->cur_item_idx);
        }
    }

#endif
    else if (timer_id == list_ctrl_ptr->switch_timer_id)
    {
        ListStopSwitchTimer(list_ctrl_ptr);
        MMK_PostMsg(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_APP_DOWN, PNULL, NULL);
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle the key down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyDown(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    BOOLEAN     result = FALSE;
    int32 offset_y = 0;
    GUIFORM_NOTIFY_LINE_T   notify_line = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (ListReviceDrawItem(list_ctrl_ptr, &offset_y, TRUE))
    {
        if (offset_y != 0)
        {
            list_ctrl_ptr->offset_y += offset_y;

            LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
            ListPostSlideStateMsg(list_ctrl_ptr, GUILIST_END_SLIDE);

            notify_line.ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;

            // 显示
            ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
            ListUpdateHighlightItem(list_ctrl_ptr);
        }
    }
    else if (0 < list_ctrl_ptr->total_item_num)
    {
        ListMoveToNextItem(list_ctrl_ptr, CTRLLIST_DIRECTION_KEY_DOWN, TRUE, FALSE, list_ctrl_ptr->move_item_num);

        if (list_ctrl_ptr->cur_item_idx == list_ctrl_ptr->total_item_num - 1)
        {
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVEBOTTOM);
        }
        else
        {
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVEDOWN);
        }

        notify_line.ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LINE_CHANGE, &notify_line);
    }

    return (result);
}


/*****************************************************************************/
//  Description : handle the key up message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyUp(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    BOOLEAN result = TRUE;
    int32 offset_y = 0;
    GUIFORM_NOTIFY_LINE_T   notify_line = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (ListReviceDrawItem(list_ctrl_ptr, &offset_y, FALSE))
    {
        if (offset_y != 0)
        {
            list_ctrl_ptr->offset_y += offset_y;

            LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
            ListPostSlideStateMsg(list_ctrl_ptr, GUILIST_END_SLIDE);

            // 显示
            ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
            ListUpdateHighlightItem(list_ctrl_ptr);
        }
    }
    else if (list_ctrl_ptr->total_item_num > 0)
    {
        ListMoveToNextItem(list_ctrl_ptr, CTRLLIST_DIRECTION_KEY_UP, TRUE, FALSE, list_ctrl_ptr->move_item_num);

        if (0 == list_ctrl_ptr->cur_item_idx)
        {
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVETOP);
        }
        else
        {
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVEUP);
        }

        notify_line.ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        //////////////////////////////////////////////////////////////////////////
        // fixed by feng.xiao for NEWMS00178620
        // -fix: 暂时注掉这个地方，在keyup时如果父控件为form，里面的list会销毁并重新construct，
        // 导致不能正常高亮prev item
        // -refix: comment removed
        GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LINE_CHANGE, &notify_line);
    }

    return result;
}

/*****************************************************************************/
//  Description : handle the key ok message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleKeyOK(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    BOOLEAN         *is_need_notify //OUT
)
{
    BOOLEAN            result          = FALSE;
    BOOLEAN            is_flash        = FALSE;

    if ((PNULL == list_ctrl_ptr) || (PNULL == is_need_notify))
    {
        return FALSE;
    }

    *is_need_notify = TRUE;

    list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;
    list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;

    if (0 < list_ctrl_ptr->total_item_num)
    {
        CTRLLIST_ITEM_INFO_T    *item_ptr  = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
        GUIITEM_THEME_STATE_E   state          = GUIITEM_THEME_STATE_DEFAULT;

        if (PNULL == item_ptr)
        {
            return FALSE;
        }

        state = LIST_GetItemThemeState(list_ctrl_ptr, item_ptr, TRUE);

        if (LIST_GetItemState(item_ptr, GUIITEM_STATE_GRAY))
        {
            *is_need_notify = FALSE; // 判断是否gray状态
        }
        else if (GUIITEM_THEME_STATE_FOCUS_INVALID != state)
        {
            if (CTRLLIST_MARKING_FIXED == list_ctrl_ptr->marking_type)
            {
                if (CTRLLIST_MARK_RADIO == list_ctrl_ptr->mark_type)
                {
                    if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_OWN_SOFTKEY))
                    {
                        is_flash = ListRadioHandleOK(list_ctrl_ptr);
                    }
                    else
                    {
                        GUILIST_QRERY_SELECT_ITEM_T query_t = {0};

                        query_t = LIST_QueryMarkItemMsg(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, msg_id);

                        if (query_t.is_check_select)
                        {
                            is_flash = ListRadioHandleOK(list_ctrl_ptr);
                        }
                    }
                }
                else
                {
                    if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_OWN_SOFTKEY))
                    {
                        is_flash = ListCheckHandleOK(list_ctrl_ptr);
                    }
                    else
                    {
                        GUILIST_QRERY_SELECT_ITEM_T query_t = {0};

                        query_t = LIST_QueryMarkItemMsg(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, msg_id);

                        if (query_t.is_check_select)
                        {
                            is_flash = ListCheckHandleOK(list_ctrl_ptr);
                        }
                    }
                }
            }
            else
            {
                if (!(MSG_APP_OK == msg_id || ! LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENABLE_MARK)))
                {
                    is_flash = ListCheckHandleOK(list_ctrl_ptr);

                    //mark状态不需要发送notify
                    *is_need_notify = !is_flash;
                }
            }

            if (is_flash)
            {
                ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
#ifdef TOUCH_PANEL_SUPPORT

                if (MSG_TP_PRESS_UP != msg_id || !list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
#else
                if (!list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))

#endif
                {
                    ListUpdateHighlightItem(list_ctrl_ptr);
                }
            }

            result = TRUE;
        }
    }

    if (*is_need_notify)
    {
        MMITheme_StopControlText();
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle the key ok message of the radio list
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListRadioHandleOK(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    CTRLLIST_ITEM_INFO_T *cur_item_info_ptr = PNULL;
    CTRLLIST_ITEM_INFO_T *selected_item_ptr = PNULL;
    uint16 selected_item_num = 0;
    BOOLEAN result = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    cur_item_info_ptr  = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    if (PNULL == cur_item_info_ptr)
    {
        return FALSE;
    }

    // set the current item state
    if (!LIST_GetItemState(cur_item_info_ptr, GUIITEM_STATE_SELECTED))
    {
        selected_item_num = ListGetSelectedItem(list_ctrl_ptr, PNULL, &selected_item_ptr, 1);

        //setl selected item  to unselected
        if (PNULL != selected_item_ptr)
        {
            LIST_SetItemState(selected_item_ptr, GUIITEM_STATE_SELECTED, FALSE);
        }

        LIST_SetItemState(cur_item_info_ptr, GUIITEM_STATE_SELECTED, TRUE);

        //update selected item num
        list_ctrl_ptr->selected_item_num = 1;

        // 通知设置了启动
        GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_SET_SELECT, PNULL, 0);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle the key ok message of the check list
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListCheckHandleOK(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    CTRLLIST_ITEM_INFO_T *cur_item_info_ptr = PNULL;
    BOOLEAN             is_selected  = FALSE;
    BOOLEAN             result       = TRUE;
    uint16              max_selected_num = 0;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    cur_item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    if (PNULL == cur_item_info_ptr)
    {
        return FALSE;
    }

    is_selected  = LIST_GetItemState(cur_item_info_ptr, GUIITEM_STATE_SELECTED);
    max_selected_num = list_ctrl_ptr->mark_cnt_max;

    if (!LIST_GetItemState(cur_item_info_ptr, GUIITEM_STATE_DISABLE_MARK))
    {
        if (is_selected)
        {
            LIST_SetItemState(cur_item_info_ptr, GUIITEM_STATE_SELECTED, FALSE);

            result = ListReduceSelectedItemNum(list_ctrl_ptr, cur_item_info_ptr);
        }
        else
        {
            LIST_SetItemState(cur_item_info_ptr, GUIITEM_STATE_SELECTED, TRUE);

            result = ListUpdateSeletedState(list_ctrl_ptr, cur_item_info_ptr);
        }

        if (result)
        {
			GUILIST_MARK_ITEM_T mark_item = {0};
			mark_item.ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
			mark_item.ctrl_id     = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
			mark_item.item_idx    = list_ctrl_ptr->cur_item_idx;
			mark_item.had_maked   = LIST_GetItemState(cur_item_info_ptr, GUIITEM_STATE_SELECTED);
			
			GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_MARK_ITEM, &mark_item);

//             LIST_PostMarkItemMsg(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, LIST_GetItemState(cur_item_info_ptr, GUIITEM_STATE_SELECTED));
        }
    }
    else
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Get selected items of check list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint16 ListGetSelectedItem( //return the selected item num
    const CTRLLIST_OBJ_T   *list_ctrl_ptr,         //in:list pointer
    uint16                 *idx_ptr,           //in/out:index array,may PNULL
    CTRLLIST_ITEM_INFO_T   **selected_item_ptr,//in/out:selected item array,may PNULL
    uint16                 array_len           //in:array length
)
{
    uint16 i = 0;
    uint16 selected_num = 0;

    SCI_ASSERT(PNULL != list_ctrl_ptr); /*assert verified*/

    for (i = 0; i < list_ctrl_ptr->total_item_num; i++)
    {
        CTRLLIST_ITEM_INFO_T *item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);

        if (PNULL != item_ptr && LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
        {
            if (selected_num < array_len)
            {
                if (PNULL != idx_ptr)
                {
                    idx_ptr[selected_num] = i;
                }

                if (PNULL != selected_item_ptr)
                {
                    selected_item_ptr[selected_num] = item_ptr;
                }
            }

            selected_num++;
        }
    }

    return selected_num;
}

/*****************************************************************************/
//  Description : Get item num in one page
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetPageItemNum(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    return LIST_GetPageItemNum(list_ctrl_ptr);
}

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetSwitchItemTime(
    MMI_CTRL_ID_T ctrl_id,
    uint32 switch_time
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        list_ctrl_ptr->display_style.auto_switch_time = switch_time;
    }
    else
    {
        //SCI_TRACE_LOW:"CTRLLIST_StartSwitchItem NULL LISt!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_7729_112_2_18_3_19_57_213, (uint8 *)"");
    }
}

/*****************************************************************************/
//  Description : 设置是否自动切换item
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认时不自动切换
/*****************************************************************************/
PUBLIC void CTRLLIST_SetAutoSwitchItem(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_true
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_AUTO_SWITCH, is_true);
    }
}

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_StartSwitchItem(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_AUTO_SWITCH, TRUE); // 开启自动切换状态
        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_STOP_SWITCH, FALSE); // 开启自动切换内部状态
        ListStartSwitchTimer(list_ctrl_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"CTRLLIST_StartSwitchItem NULL LISt!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_7768_112_2_18_3_19_57_214, (uint8 *)"");
    }
}

/*****************************************************************************/
//  Description : stop auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_StopSwitchItem(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        ListStopSwitchTimer(list_ctrl_ptr);
        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_STOP_SWITCH, TRUE); // 关闭自动切换的内部状态
    }
    else
    {
        //SCI_TRACE_LOW:"CTRLLIST_StopSwitchItem NULL LISt!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_7789_112_2_18_3_19_57_215, (uint8 *)"");
    }
}

/*****************************************************************************/
//  Description : 设置空列表信息
//  Global resource dependence :
//  Author:James.Zhang
//  Note: 请在open消息时设置
/*****************************************************************************/
PUBLIC void CTRLLIST_SetEmptyInfo(
    MMI_CTRL_ID_T               ctrl_id,   //控件id
    const GUILIST_EMPTY_INFO_T *empty_ptr  //空列表信息
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr || PNULL == empty_ptr)
    {
        return;
    }

    if (PNULL != list_ctrl_ptr->empty_info.text_buffer.wstr_ptr)
    {
        SCI_FREE(list_ctrl_ptr->empty_info.text_buffer.wstr_ptr);
        list_ctrl_ptr->empty_info.text_buffer.wstr_ptr = PNULL;
    }

    list_ctrl_ptr->empty_info = *empty_ptr;

    if (PNULL != empty_ptr->text_buffer.wstr_ptr)
    {
        list_ctrl_ptr->empty_info.text_buffer.wstr_ptr = SCI_ALLOC_APP((empty_ptr->text_buffer.wstr_len + 1) * sizeof(wchar));
        SCI_MEMSET(list_ctrl_ptr->empty_info.text_buffer.wstr_ptr, 0, ((empty_ptr->text_buffer.wstr_len + 1) * sizeof(wchar)));

        MMIAPICOM_Wstrncpy(list_ctrl_ptr->empty_info.text_buffer.wstr_ptr, empty_ptr->text_buffer.wstr_ptr, empty_ptr->text_buffer.wstr_len);
    }
}

/*****************************************************************************/
//  Description : draw empty info
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ListDrawEmpty(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
)
{
    GUI_RECT_T   valid_rect = {0};
    MMI_STRING_T empty_string = {0};
    GUISTR_STATE_T state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR;
    GUISTR_STYLE_T style = {0};/*lint !e64*/

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (!LIST_GetValidRect(list_ctrl_ptr, &valid_rect))
    {
        return;
    }

    if (PNULL != list_ctrl_ptr->empty_info.text_buffer.wstr_ptr)
    {
        empty_string = list_ctrl_ptr->empty_info.text_buffer;
    }
    else
    {
        MMI_TEXT_ID_T text_id = (0 != list_ctrl_ptr->empty_info.text_id)
                                ? list_ctrl_ptr->empty_info.text_id : list_ctrl_ptr->display_style.text_empty_id;

        MMITHEME_GetResText(text_id, list_ctrl_ptr->win_id, &empty_string);
    }

    style.align = ALIGN_HVMIDDLE;
    style.font  = list_ctrl_ptr->display_style.text_empty_font;
    style.font_color = list_ctrl_ptr->display_style.text_empty_color;

    GUISTR_DrawTextToLCDInRect(lcd_dev_info_ptr,
                               &((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect,
                               &valid_rect,
                               &empty_string,
                               &style,
                               state,
                               GUISTR_TEXT_DIR_AUTO);
}

/*****************************************************************************/
//  Description : draw head split item bg
//  Global resource dependence :
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL void ListDrawSplitBar(CTRLLIST_OBJ_T *list_ctrl_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_APPEND_BLT_T        append_layer   = {0};
    CTRLLIST_ITEM_INFO_T         *first_item_ptr = PNULL;
    GUI_RECT_T                  clip_rect = {0};
    GUI_RECT_T                  img_rect = {0};
    GUI_RECT_T                  text_rect = {0};
    //GUI_RECT_T                  layout_rect = {0};
    MMI_STRING_T                text_info = {0};
    GUISTR_STYLE_T              text_style = {0};
    THEMELIST_ITEM_STYLE_T     *split_style_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (list_ctrl_ptr->total_item_num == 0)
    {
        if (0 != list_ctrl_ptr->split_layer.block_id)
        {
            UILAYER_RemoveBltLayer(&list_ctrl_ptr->split_layer);
        }

        return;
    }

    if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SPLIT_ONSIDE | GUILIST_STATE_ACTIVE | GUILIST_STATE_FOCUS))
    {
        return;
    }

    first_item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, 0);

    if (PNULL == first_item_ptr || !LIST_GetItemState(first_item_ptr, GUIITEM_STATE_SPLIT))
    {
        return;
    }

    split_style_ptr = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT);

    if (PNULL == split_style_ptr)
    {
        return;
    }

    list_ctrl_ptr->split_rect = LIST_GetSplitBarRect(list_ctrl_ptr);

    GUI_IntersectRect(&clip_rect, list_ctrl_ptr->split_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect);

    if (GUI_IsInvalidRect(clip_rect))
    {
        return;
    }

    LIST_CreateSplitBarLayer(list_ctrl_ptr, &list_ctrl_ptr->split_rect);

    //split_layer每次都会重画，使用双缓冲切换的方式
    UILAYER_SwitchDoubleMem(&list_ctrl_ptr->split_layer, TRUE);
    UILAYER_Clear(&list_ctrl_ptr->split_layer);

    UILAYER_SetLayerPosition(&list_ctrl_ptr->split_layer, list_ctrl_ptr->split_rect.left, list_ctrl_ptr->split_rect.top);
    append_layer.lcd_dev_info = list_ctrl_ptr->split_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    img_rect.right  = (int16)(img_rect.left + clip_rect.right - clip_rect.left);
    img_rect.bottom = (int16)(img_rect.top + clip_rect.bottom - clip_rect.top);

    IMG_EnableTransparentColor(TRUE);
    GUI_SetAlphaOverwrite(TRUE);
    GUIRES_DisplaySabmImg(&clip_rect, &list_ctrl_ptr->split_rect, &img_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,
                          list_ctrl_ptr->display_style.split_item_bg_info.img_id,
                          (const GUI_LCD_DEV_INFO *)&list_ctrl_ptr->split_layer);
    GUI_SetAlphaOverwrite(FALSE);
    IMG_EnableTransparentColor(FALSE);

    UILAYER_WeakLayerAlpha(&list_ctrl_ptr->split_layer, 0xA0);

    LIST_GetItemSplitText(list_ctrl_ptr, list_ctrl_ptr->top_item_idx, &text_info);

    text_style.font = split_style_ptr->content[0].font;
    text_style.font_color = THEMELIST_GetSplitItemTextColor();

    //计算text的起始位置
    LIST_CalculateListItemRect(&text_rect, list_ctrl_ptr, list_ctrl_ptr->top_item_idx);

    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        text_style.align = ALIGN_RVMIDDLE;
    }
    else
    {
        text_style.align = ALIGN_LVMIDDLE;
    }

    text_rect.top = list_ctrl_ptr->split_rect.top;
    text_rect.bottom = list_ctrl_ptr->split_rect.bottom;
    GUI_IntersectRect(&clip_rect, text_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect);

    GUISTR_DrawTextToLCDInRect(&list_ctrl_ptr->split_layer, (const GUI_RECT_T *)&text_rect, (const GUI_RECT_T *)&clip_rect, (const MMI_STRING_T *)&text_info,
                               &text_style, GUISTR_STATE_NONE, GUISTR_TEXT_DIR_AUTO);
#endif

    return;
}


/*****************************************************************************/
//  Description : draw split item bg
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawSplitStyleItemBg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T item_rect,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr
)
{
    GUI_RECT_T          clip_rect       = {0};
    GUI_RECT_T          valid_rect      = {0};
    GUI_RECT_T          img_rect        = {0};

    if (PNULL != list_ctrl_ptr
        && LIST_GetValidRect(list_ctrl_ptr, &valid_rect)
        && GUI_IntersectRect(&clip_rect, item_rect, valid_rect))
    {
        img_rect.right  = (int16)(img_rect.left + clip_rect.right - clip_rect.left);
        img_rect.bottom = (int16)(img_rect.top + clip_rect.bottom - clip_rect.top);

        GUIRES_DisplaySabmImg(
            &clip_rect,
            &item_rect,
            &img_rect,
            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,
            list_ctrl_ptr->display_style.split_item_bg_info.img_id,
            (const GUI_LCD_DEV_INFO *)lcd_dev_info_ptr);
    }

    return;
}

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawListItemSplitLine(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         item_index,
    GUI_RECT_T item_rect,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr
)
{
    ListDrawItemSplitLine(list_ctrl_ptr, item_index, item_rect, lcd_dev_info_ptr);
}

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawItemSplitLine(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         item_index,
    GUI_RECT_T item_rect,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr
)
{
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      cross_rect = {0};

    if (PNULL != list_ctrl_ptr && 0 < list_ctrl_ptr->total_item_num && !GUI_IsInvalidRect(item_rect)
        && !LIST_GetItemStateByIndex(list_ctrl_ptr, item_index, GUIITEM_STATE_SPLIT))
    {
        // 下线
        if (list_ctrl_ptr->total_item_num - 1 == item_index)
        {
            line_rect = item_rect;
            line_rect.top = item_rect.bottom;

            if (GUI_IntersectRect(&cross_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect, line_rect))
            {
                //draw line
                GUI_FillRect((const GUI_LCD_DEV_INFO *)lcd_dev_info_ptr,
                             cross_rect,
                             list_ctrl_ptr->display_style.split_line_color);
            }
        }

        // 上线
        line_rect = item_rect;
        line_rect.bottom = item_rect.top;

        if (!GUI_IntersectRect(&cross_rect, list_ctrl_ptr->split_rect, line_rect)) // 上线
        {
            //head split不需要画线
            if (GUI_IntersectRect(&cross_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect, line_rect))
            {
                //draw line
                GUI_FillRect((const GUI_LCD_DEV_INFO *)lcd_dev_info_ptr,
                             cross_rect,
                             list_ctrl_ptr->display_style.split_line_color);
            }
        }
    }

    return;
}

/*****************************************************************************/
//  Description : draw the list box directly
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawAllItems(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
)
{
    int32               index      = 0;
    int32               end_item_index      = 0;
    BOOLEAN             is_draw = FALSE;
    GUI_RECT_T          item_rect  = {0};
    GUI_RECT_T          inter_rect = {0};
    GUI_RECT_T          list_rect = {0};
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if ((PNULL == list_ctrl_ptr) || (PNULL == lcd_dev_info_ptr))
    {
        return;
    }

    list_ctrl_ptr->str_start_pos = 0;

    // draw split bar
    ListDrawSplitBar(list_ctrl_ptr);

    if (0 == list_ctrl_ptr->total_item_num)
    {
        ListDrawEmpty(list_ctrl_ptr, lcd_dev_info_ptr);

        return;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    // display all items
    for (index = list_ctrl_ptr->top_item_idx; index <= list_ctrl_ptr->end_item_idx; index++)
    {
        //滚动条拖动时, 可能会出现最后一屏不满屏的情况
        if (index >= list_ctrl_ptr->total_item_num)
        {
            break;
        }

        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

        if (PNULL == item_ptr)
        {
            continue;
        }

        /* need item data */
        if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA) && PNULL == item_ptr->item.data_ptr)
        {
            LIST_SendNeedItemDataMsg(list_ctrl_ptr, index);

            if (list_ctrl_ptr->is_pos_dirty)
            {
                LIST_ResetItemPostion(list_ctrl_ptr);
            }
        }

        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, index);

        if (GUI_IntersectRect(&inter_rect, item_rect, list_rect))
        {
            BOOLEAN    need_draw = TRUE;
            GUI_RECT_T cur_rect = {0};

#ifdef UI_MULTILAYER_SUPPORT

            //如果做了内存回写加速
            if (! GUI_IsZeroRect(list_ctrl_ptr->writeback_rect)
                && GUI_IntersectRect(&cur_rect, inter_rect, list_ctrl_ptr->writeback_rect))// if the the item in the writeback rect
            {
                //判断边界item是否是完整的，如果不是完整的需要将拷贝的区域重置
                need_draw = !GUI_RectIsCovered(list_ctrl_ptr->writeback_rect, inter_rect);

                if (need_draw)
                {
                    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_CUSTORMER_LAYER))
                    {
                        ListDrawBackgroundByRect(list_ctrl_ptr, &cur_rect);
                    }
                    else
                    {
                        UILAYER_ClearRect(lcd_dev_info_ptr, cur_rect);
                    }
                }
            }

#endif

            if (need_draw)
            {
                ListDrawItem(list_ctrl_ptr, lcd_dev_info_ptr, index);
            }

            if (!is_draw)
            {
                list_ctrl_ptr->top_item_idx = index; // 重新设置top
            }

            is_draw = TRUE;
            end_item_index = index;
        }
        else if (is_draw)
        {
            break;
        }
    }

    if (is_draw)
    {
        list_ctrl_ptr->end_item_idx = end_item_index; // 重新设置end
    }

    return;
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 拷贝不需要更新的buffer
//  Global resource dependence :
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void ListDoDoubleMemWriteBack(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    int32             offset_y
)
{
    GUI_RECT_T  src_rc = {0};
    GUI_RECT_T  dst_rc = {0};
    GUI_RECT_T  clip_rc = {0};
    int32       offset = 0;
    //UILAYER_MEM_FORMAT_T old_format = 0;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (!(UILAYER_IsMultiLayerEnable()
          && UILAYER_IsLayerActive(&list_ctrl_ptr->item_layer)
          && list_ctrl_ptr->display_style.is_slide_used))
    {
        return;
    }

    //滑动第一帧需要重画
    if (!VTLBASE_GetSlideState((CTRLBASE_OBJ_T *)list_ctrl_ptr))
    {
        return;
    }

    if (0 == (offset = list_ctrl_ptr->offset_y - offset_y))
    {
        return;
    }

    dst_rc = src_rc = clip_rc = LIST_AdjustRect(list_ctrl_ptr);

    dst_rc.top += offset;
    dst_rc.bottom += offset;

    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_CUSTORMER_LAYER))
    {
        //是否双缓冲
        if (!(UILAYER_GetLayerMemFormat(&list_ctrl_ptr->item_layer) & UILAYER_MEM_FORMAT_DOUBLE))
        {
            return;
        }

        ListDrawBackgroundByRect(list_ctrl_ptr, &src_rc);
    }
    else
    {
        //切换双buffer
        if (!UILAYER_SwitchDoubleMem(&list_ctrl_ptr->item_layer, TRUE))
        {
            return;
        }

        UILAYER_ClearRect(&list_ctrl_ptr->item_layer, src_rc);
    }

    GUI_IntersectRect(&clip_rc, clip_rc, dst_rc);

    if (UILAYER_DoubleMemWriteBack(&list_ctrl_ptr->item_layer, &dst_rc, &clip_rc, &src_rc))
    {
        //如果回写bufer成功则将相应的区域值上
        list_ctrl_ptr->writeback_rect = clip_rc;
    }
}
#endif

/*****************************************************************************/
//  Description : 处理滑动效果
//  Global resource dependence :
//  Author: xiaoqing.lu
LOCAL void ListUpdateHighlightItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
#ifdef UI_MULTILAYER_SUPPORT

    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_SLIDE_ITEM))
    {
        GUI_RECT_T item_rect = {0};

        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        LIST_CreateItemDispLayer(list_ctrl_ptr, item_rect);
    }

#endif

    if (VTLBASE_GetVisible((CTRLBASE_OBJ_T *)list_ctrl_ptr))
    {
        // 显示高亮item
        if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
        {
            ListDrawHighlightItem(list_ctrl_ptr);
        }
// Bug 1335463 start
        //resume text timer
        ListResumeItemTextTimer(list_ctrl_ptr);
// Bug 1335463 end
        // Start switch timer
        ListStartSwitchTimer(list_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : 画全屏的list
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListDraw(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    INNER_UPDATE_STATE_T inner_state
)
{
    BOOLEAN is_set_blt = FALSE;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (!VTLBASE_GetVisible((CTRLBASE_OBJ_T *)list_ctrl_ptr))
    {
        return;
    }

#ifdef UI_MULTILAYER_SUPPORT
    ListCreateItemLayer(list_ctrl_ptr);
#endif

#ifdef UI_MULTILAYER_SUPPORT
    // 如果item layer有效，则需要进行层的合并
    is_set_blt = list_ctrl_ptr->display_style.is_slide_used && UILAYER_IsMultiLayerEnable()
                 && UILAYER_IsLayerActive(&list_ctrl_ptr->item_layer) && !list_ctrl_ptr->is_cus_blt_layer;

    if (is_set_blt)
    {
        ListSetBltItemLayer(list_ctrl_ptr, list_ctrl_ptr->item_layer.block_id);
    }

#endif

    // 画背景
    ListDrawBackground(list_ctrl_ptr);

    // 画边框
    ListDrawBorder(list_ctrl_ptr);

    // 画title
    ListDrawTitle(list_ctrl_ptr, inner_state & INNER_STATE_UPDATE_TITLE);

    // 画items
    if (inner_state & INNER_STATE_UPDATE_ITEM)
    {
        lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);
        LIST_ResetItemPostion(list_ctrl_ptr);
        ListDrawAllItems(list_ctrl_ptr, &lcd_dev_info);
    }

    // 画softkey
    ListDrawSoftkey(list_ctrl_ptr, inner_state & INNER_STATE_UPDATE_SK);

    // scroll bar
    ListDrawScrollBar(list_ctrl_ptr, inner_state & INNER_STATE_UPDATE_PRG);

    return;
}

/*****************************************************************************/
//  Description : 恢复item text滚动的timer
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListResumeItemTextTimer(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_RECT_T  item_rect = {0};
    GUI_RECT_T  inter_rect = {0};

    if (NULL == list_ctrl_ptr)
    {
        return;
    }

    if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENTER))
    {
        return;
    }

    // 当高亮条显示一下就消失的状态，是不需要启动timer滚动字符串的
    if (list_ctrl_ptr->total_item_num > 0 && (!list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()) || list_ctrl_ptr->is_long_pressing))
    {
        CTRLLIST_ITEM_INFO_T *item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        // 恢复当前item的text滚动状态
        if (PNULL != item_ptr && 0 == list_ctrl_ptr->text_scroll_timer_id && MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state)
        {
            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

            if (GUI_IntersectRect(&inter_rect, item_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect)
                && IsItemTextNeedScroll(list_ctrl_ptr, item_ptr)
                && LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ACTIVE | GUILIST_STATE_FOCUS))
            {
                list_ctrl_ptr->text_scroll_timer_id = MMK_CreateWinTimer(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, list_ctrl_ptr->display_style.text_time, FALSE);
                list_ctrl_ptr->str_start_pos = 0;
            }
        }
    }
}

/*****************************************************************************/
//  Description : 设置item标记状态
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetItemMarkStatus(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_ptr,
    uint16          item_idx
)
{
    BOOLEAN                 old_selected;

    if ((NULL == list_ctrl_ptr) || (PNULL == item_ptr))
    {
        return;
    }

    if (CTRLLIST_MARKING_NONE == list_ctrl_ptr->marking_type)
    {
        return;
    }

    old_selected = LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED);

    LIST_SetItemState(item_ptr, GUIITEM_STATE_SELECTED, (BOOLEAN)!old_selected);

    if (!old_selected)
    {
        ListUpdateSeletedState(list_ctrl_ptr, item_ptr);
    }
    else
    {
        ListReduceSelectedItemNum(list_ctrl_ptr, item_ptr);
    }

    LIST_PostMarkItemMsg(list_ctrl_ptr, item_idx, (BOOLEAN)(!old_selected));
}

/*****************************************************************************/
//  Description : construct
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListCtrlConstruct(
    CTRLBASE_OBJ_T          *obj_ptr,
    CTRLLIST_INIT_PARAM_T   *list_param_ptr
)
{
    BOOLEAN         reslut = TRUE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = (CTRLLIST_OBJ_T *)obj_ptr;

    if (PNULL != list_ctrl_ptr && PNULL != list_param_ptr)
    {
        ListCtrlInit(obj_ptr, list_param_ptr);
    }
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_8469_112_2_18_3_19_59_216, (uint8 *)"");
        reslut = FALSE;
    }

    return reslut;
}


/*****************************************************************************/
//  Description : destruct
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListCtrlDestruct(
    CTRLBASE_OBJ_T *obj_ptr
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)obj_ptr;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_8492_112_2_18_3_19_59_217, (uint8 *)"d", ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
   //清理自定义style
    MMITHEME_CustomListItemStyleUnreg(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
    //stop timer
    ListStopTextAnim(list_ctrl_ptr);

    LIST_DestroyAllItemData(list_ctrl_ptr, FALSE);

    //释放Item所占用的空间；
    if (PNULL != list_ctrl_ptr->item_info_ptr)
    {
        SCI_FREE(list_ctrl_ptr->item_info_ptr);
        list_ctrl_ptr->item_info_ptr = PNULL;
    }

    if (PNULL != list_ctrl_ptr->prgbox_ptr)
    {
        list_ctrl_ptr->prgbox_ptr = PNULL;
    }

    if (PNULL != list_ctrl_ptr->empty_info.text_buffer.wstr_ptr)
    {
        SCI_FREE(list_ctrl_ptr->empty_info.text_buffer.wstr_ptr);
        list_ctrl_ptr->empty_info.text_buffer.wstr_ptr = PNULL;
    }

#ifdef UI_MULTILAYER_SUPPORT
    ListReleaseItemLayer(list_ctrl_ptr);

    ListReleaseTipLayer(list_ctrl_ptr);

    LIST_ReleaseSplitLayer(list_ctrl_ptr);

    LIST_ReleaseItemDispLayer();
    LIST_ReleaseItemMistLayer();
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : draw tips info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void  ListDrawTipsInfo(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_RECT_T      rect            = {0};
    GUITIPS_DATA_T  need_tips_data  = {0};
    uint32          line_index      = 0;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (NULL == list_ctrl_ptr)
    {
        return;
    }

    need_tips_data.ctrl_id = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
    need_tips_data.index =  list_ctrl_ptr->cur_item_idx;

    if (MMK_IsIncludeTips(list_ctrl_ptr->win_id) && GUITIPS_GetTipsValid(list_ctrl_ptr->win_id))
    {
        GUITIPS_SetTextColor(list_ctrl_ptr->win_id, MMITHEME_GetCurThemeFontColor(MMI_THEME_TIPS_FONT/*MMI_THEME_LIST_ITEM0*/));
        // 向窗口要数据
        MMK_SendMsg(list_ctrl_ptr->win_id, MSG_CTL_TIPS_NEED_DATA, (DPARAM)&need_tips_data);

        if (need_tips_data.is_need_tips)
        {
            LIST_CalculateListItemRect(&rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

            if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENABLE_MARK))
            {
                item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, line_index);

                if (PNULL != item_ptr)
                {
                    rect.right   = item_ptr->mark_left;
                }
            }

            // 重新设置tips的下边界值
            GUITIPS_SetEdgeRect(list_ctrl_ptr->win_id, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect);

            lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

            GUITIPS_SetLcdInfo(list_ctrl_ptr->win_id, &lcd_dev_info);

            // 获取当前item的区域,该区域将作为tips的原始区域
            // 显示tips
            GUITIPS_Update(list_ctrl_ptr->win_id, &rect, &need_tips_data);
        }
    }
}

/*****************************************************************************/
//  Description : get the mmi system pointer
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListCtrlHandleMsg(
    CTRLBASE_OBJ_T      *ctrl_ptr, //当前控件的指针
    MMI_MESSAGE_ID_E msg_id,       //事件类型
    DPARAM           param          //附带的处理参数
)
{
    MMI_RESULT_E        result         = MMI_RESULT_TRUE;
    CTRLLIST_OBJ_T      *list_ctrl_ptr       = PNULL;
#ifdef TOUCH_PANEL_SUPPORT//xiyuan edit for pclint
    GUI_POINT_T         point = {0};
#endif
    BOOLEAN             is_need_notify = TRUE;

    list_ctrl_ptr = (CTRLLIST_OBJ_T *)ctrl_ptr;

    if (PNULL == list_ctrl_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_8612_112_2_18_3_19_59_218, (uint8 *)"");
        return MMI_RESULT_FALSE;
    }

    // 处理消息
    switch (msg_id)
    {
        case MSG_CTL_OPEN:
            // 创建层
            ListResetOffsetY(list_ctrl_ptr);

            if (MMK_IsFocusWin(list_ctrl_ptr->win_id))
            {
                LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_FOCUS, TRUE);
            }

            list_ctrl_ptr->cur_msg_loop_item_idx = (uint16) - 1; //初始值
            break;

        case MSG_CTL_CLOSE:
            MMITheme_StopControlText();
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
            SCI_MEMSET(&list_ctrl_ptr->move_info, 0, sizeof(CTRLLIST_MOVE_INFO_T));
            // StopTimer
            // 释放空间的函数仅仅由应用来释放
            ListStopTextAnim(list_ctrl_ptr);
            ListStopSwitchTimer(list_ctrl_ptr);

            LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_FOCUS, FALSE);
            break;

        case MSG_CTL_GET_FOCUS:
            list_ctrl_ptr->is_click_item = FALSE;
            list_ctrl_ptr->is_tp_down = FALSE;
			list_ctrl_ptr->is_long_pressing = FALSE;
            list_ctrl_ptr->cur_msg_loop_item_idx = (uint16) - 1; //初始值
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

            list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;
            list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
            LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_FOCUS, TRUE);

            // 恢复当前item的text滚动状态
            ListResumeItemTextTimer(list_ctrl_ptr);
            break;

        case MSG_CTL_LOSE_FOCUS:
#ifdef TOUCH_PANEL_SUPPORT
            ListItemResetContentCheck(list_ctrl_ptr);
#endif
            MMITheme_StopControlText();
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

            LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_FOCUS, FALSE);
            LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, FALSE);

#ifdef TOUCH_PANEL_SUPPORT
            ListStopTpMoveTimer(list_ctrl_ptr);
#endif

            if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENTER))
            {
                break;
            }

            // validate list
            ListValidateCtrl(list_ctrl_ptr , FALSE, FALSE);

#ifdef TOUCH_PANEL_SUPPORT
            ListRecoverSlideByOffsetY(list_ctrl_ptr, FALSE, ListGetRecoverSpace(list_ctrl_ptr));
#endif
            LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT
            ListReleaseTipLayer(list_ctrl_ptr);
            LIST_ReleaseSplitLayer(list_ctrl_ptr);
#endif

#ifdef UI_MULTILAYER_SUPPORT

            //若前一帧为direct draw，需要重画
            if (UILAYER_IsPerDirectDraw())
            {
                ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);

                if (!list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
                {
                    ListUpdateHighlightItem(list_ctrl_ptr);
                }
            }

#endif
            ListStopTextAnim(list_ctrl_ptr);
            ListStopSwitchTimer(list_ctrl_ptr);
            break;

        case MSG_CTL_GET_ACTIVE:
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
            LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ACTIVE, TRUE);
            // 恢复当前item的text滚动状态
            ListResumeItemTextTimer(list_ctrl_ptr);

            //notify parent
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_GET_ACTIVE);
            break;

        case MSG_CTL_LOSE_ACTIVE:
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
            LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ACTIVE, FALSE);
            //stop timer
            ListStopTextAnim(list_ctrl_ptr);
            ListStopSwitchTimer(list_ctrl_ptr);

            //notify parent
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LOSE_ACTIVE);

#ifdef UI_MULTILAYER_SUPPORT
            ListReleaseTipLayer(list_ctrl_ptr);
            LIST_ReleaseSplitLayer(list_ctrl_ptr);
#endif
            break;

        case MSG_NOTIFY_LIST_ENTER:
            break;

        case MSG_CTL_PAINT:
            if (VTLBASE_GetVisible((CTRLBASE_OBJ_T *)list_ctrl_ptr))
            {
                LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENTER, TRUE);

#ifdef UI_MULTILAYER_SUPPORT
                ListCreateItemLayer(list_ctrl_ptr);
#endif

                LIST_ResetItemPostion(list_ctrl_ptr);
                // validate list
                ListValidateCtrl(list_ctrl_ptr , FALSE, FALSE);

#ifdef TOUCH_PANEL_SUPPORT
                ListRecoverSlideByOffsetY(list_ctrl_ptr, FALSE, ListGetRecoverSpace(list_ctrl_ptr));
#endif
                LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);

                ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_SK | INNER_STATE_UPDATE_TITLE | INNER_STATE_UPDATE_PRG);

                ListStopTextAnim(list_ctrl_ptr);

                if ((list_ctrl_ptr->is_click_item && list_ctrl_ptr->is_tp_down) ||
                    MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
                {
                    LIST_ChangeCurItemToValidItem(list_ctrl_ptr);
                    ListUpdateHighlightItem(list_ctrl_ptr);
                }

#ifdef MMI_READMENU_ENABLE
                ListHandleTextToSpeech(list_ctrl_ptr);
#endif
            }

            break;

        case MSG_APP_WEB:
            if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
            {
                ListUpdateHighlightItem(list_ctrl_ptr);
            }
            else
            {
                if (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state)
                {
                    list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;
                    list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
                    //notify parent
                    ListHandleKeyOK(list_ctrl_ptr, msg_id, &is_need_notify);

                    //非标记状态, 发送notify
                    if (is_need_notify)
                    {
                        GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_MIDSK);
                    }
                }
            }

            break;

        case MSG_APP_OK:
            if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
            {
                ListUpdateHighlightItem(list_ctrl_ptr);
            }
            else
            {
                if (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state)
                {
                    //notify parent
                    ListHandleKeyOK(list_ctrl_ptr, msg_id, &is_need_notify);

                    //非标记状态, 发送notify
                    if (is_need_notify)
                    {
                        GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_OK);
                    }
                }
            }

            break;

        case MSG_APP_RIGHT:
        case MSG_KEYREPEAT_RIGHT:
        case MSG_APP_DOWN:
        case MSG_KEYREPEAT_DOWN:
            if (MMK_TP_SLIDE_NONE != list_ctrl_ptr->slide_state)
            {
                break;
            }

            if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
            {
                ListUpdateHighlightItem(list_ctrl_ptr);
                break;
            }

            ListStopSwitchTimer(list_ctrl_ptr);

            //默认不处理右键
            if ((msg_id & 0xff) != KEY_RIGHT || LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_HANDLE_LEFT_RIGHT))
            {
                // 先停止，避免和3D产生纠结，导致exp
                MMITheme_StopControlText();

                if (ListIsCycScroll(list_ctrl_ptr))
                {
                    if (list_ctrl_ptr->total_item_num > 1)
                    {
                        list_ctrl_ptr->list_state |= GUILIST_STATE_ACTIVE;
                        ListStopTextAnim(list_ctrl_ptr);
                        list_ctrl_ptr->move_item_num = LIST_CalculateMoveItemNum(list_ctrl_ptr, CTRLLIST_DIRECTION_KEY_DOWN); // 向下键每次移动一个item
                        ListHandleKeyDown(list_ctrl_ptr);
                    }
                }
                else
                {
                    uint16 pure_item_index = 0;
                    uint16 pure_item_num = 0;

                    pure_item_num = ListGetPureTotalItemNumber(list_ctrl_ptr);

                    if (pure_item_num > 1)
                    {

                        list_ctrl_ptr->list_state |= GUILIST_STATE_ACTIVE;
                        ListStopTextAnim(list_ctrl_ptr);

                        CTRLLIST_GetPureIndex(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, list_ctrl_ptr->cur_item_idx, &pure_item_index);

                        if (pure_item_num - 1 == pure_item_index)
                        {
                            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_TAIL);
                        }
                        else
                        {
                            list_ctrl_ptr->move_item_num = LIST_CalculateMoveItemNum(list_ctrl_ptr, CTRLLIST_DIRECTION_KEY_DOWN); // 向下键每次移动一个item
                            ListHandleKeyDown(list_ctrl_ptr);
                        }
                    }
                    else
                    {
                        result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_TAIL);
                    }
                }

#ifdef MMI_READMENU_ENABLE
                ListHandleTextToSpeech(list_ctrl_ptr);
#endif
            }
            else
            {
                // fix cr196451: 如果listbox用在dropdownlist风格中，那吃掉右键
                if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_DROPDOWNLIST))
                {
                    result = MMI_RESULT_FALSE;
                }
            }

            //画高亮条
            if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
            {
                LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR,TRUE);
                MMK_SendMsg(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_CTL_PAINT, PNULL);
            }

            break;

        case MSG_APP_LEFT:
        case MSG_KEYREPEAT_LEFT:
        case MSG_APP_UP:
        case MSG_KEYREPEAT_UP:
            if (MMK_TP_SLIDE_NONE != list_ctrl_ptr->slide_state)
            {
                break;
            }

            if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
            {
                ListUpdateHighlightItem(list_ctrl_ptr);
                break;
            }

            //默认不处理左键
            if ((msg_id & 0xff) != KEY_LEFT || LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_HANDLE_LEFT_RIGHT))
            {
                // 先停止，避免和3D产生纠结，导致exp
                MMITheme_StopControlText();

                if (ListIsCycScroll(list_ctrl_ptr))
                {
                    if (list_ctrl_ptr->total_item_num > 1)
                    {
                        list_ctrl_ptr->list_state |= GUILIST_STATE_ACTIVE;
                        ListStopTextAnim(list_ctrl_ptr);
                        list_ctrl_ptr->move_item_num = LIST_CalculateMoveItemNum(list_ctrl_ptr, CTRLLIST_DIRECTION_KEY_UP); // 向上键每次移动一个item
                        ListHandleKeyUp(list_ctrl_ptr);
                    }
                }
                else
                {
                    if (list_ctrl_ptr->total_item_num > 1)
                    {
                        uint16 pure_item_index = 0;

                        list_ctrl_ptr->list_state |= GUILIST_STATE_ACTIVE;
                        ListStopTextAnim(list_ctrl_ptr);

                        CTRLLIST_GetPureIndex(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, list_ctrl_ptr->cur_item_idx, &pure_item_index);

                        if (0 == pure_item_index)
                        {
                            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_HEAD);
                        }
                        else
                        {
                            list_ctrl_ptr->move_item_num = LIST_CalculateMoveItemNum(list_ctrl_ptr, CTRLLIST_DIRECTION_KEY_UP); // 向上键每次移动一个item
                            ListHandleKeyUp(list_ctrl_ptr);
                        }
                    }
                    else
                    {
                        result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_HEAD);
                    }
                }

#ifdef MMI_READMENU_ENABLE
                ListHandleTextToSpeech(list_ctrl_ptr);
#endif
            }
            else
            {
                // fix cr196451: 如果listbox用在dropdownlist风格中，那吃掉左键
                if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_DROPDOWNLIST))
                {
                    result = MMI_RESULT_FALSE;
                }
            }

            //画高亮条
            if (!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
            {
                LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR,TRUE);
                MMK_SendMsg(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_CTL_PAINT, PNULL);
            }

            break;

        case MSG_APP_STAR:
            if (MMK_TP_SLIDE_NONE != list_ctrl_ptr->slide_state)
            {
                break;
            }

            if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
            {
                ListUpdateHighlightItem(list_ctrl_ptr);
                break;
            }

            if (list_ctrl_ptr->is_handle_starhash && ListHandleKeyLeft(list_ctrl_ptr))
            {
                MMITheme_StopControlText();

                ListStopTextAnim(list_ctrl_ptr);
                //ListValidateCtrl( list_ctrl_ptr , FALSE, FALSE);
                ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
                // 画title,之所以重画title，是因为上面没有画
                ListDrawTitle(list_ctrl_ptr, TRUE);
                ListUpdateHighlightItem(list_ctrl_ptr);

#ifdef MMI_READMENU_ENABLE
                ListHandleTextToSpeech(list_ctrl_ptr);
#endif
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }

            break;

        case MSG_APP_HASH:
            if (MMK_TP_SLIDE_NONE != list_ctrl_ptr->slide_state)
            {
                break;
            }

            if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
            {
                ListUpdateHighlightItem(list_ctrl_ptr);
                break;
            }

            if (list_ctrl_ptr->is_handle_starhash && ListHandleKeyRight(list_ctrl_ptr))
            {
                MMITheme_StopControlText();

                ListStopTextAnim(list_ctrl_ptr);

                ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
                // 画title,之所以重画title，是因为上面没有画
                ListDrawTitle(list_ctrl_ptr, TRUE);
                ListUpdateHighlightItem(list_ctrl_ptr);

#ifdef MMI_READMENU_ENABLE
                ListHandleTextToSpeech(list_ctrl_ptr);
#endif
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }

            break;


        case MSG_APP_CANCEL:
#ifdef TOUCH_PANEL_SUPPORT
            ListStopTpMoveTimer(list_ctrl_ptr);
#endif
            result = GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_CANCEL);
            break;

        case MSG_TIMER:
            result = ListHandleTimer(list_ctrl_ptr, param);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_TP_PRESS_DOWN:
            // 先停止，避免和3D产生纠结，导致exp
            MMITheme_StopControlText();
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

            //画高亮条
            ListSetItemHighBarIfClick(list_ctrl_ptr); //BUG-1512867

            ListHandleTpDown(list_ctrl_ptr, msg_id, &point); //BUG-2109509
            break;

        case MSG_TP_PRESS_UP:
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            //判断当前操作是否是click
            if (
                (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state && !(IS_TP_MOVE_Y(point.y, list_ctrl_ptr->slide_start_point.y)))
                || (!list_ctrl_ptr->display_style.is_slide_used)
                )
            {
                //返回当前选中的content信息
                ListHandleSendItemContentClick(list_ctrl_ptr, &point);
            }

            ListHandleTpUp(list_ctrl_ptr, msg_id, &point);
            //去除高亮条
            if(LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_NEED_HIGHBAR_IF_CLICK))
            {
                if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
                {
                    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR,FALSE);
                    MMK_SendMsg(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_CTL_PAINT, PNULL);
                }
            }
            break;

        case MSG_TP_PRESS_LONG:
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            ListHandleTpLong(list_ctrl_ptr, &point);
            //画高亮条
            ListSetItemHighBarIfClick(list_ctrl_ptr); //BUG-1512867
            break;

        case MSG_TP_PRESS_MOVE:
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            ListHandleTpMove(list_ctrl_ptr, msg_id, &point);
            //画高亮条
            ListSetItemHighBarIfClick(list_ctrl_ptr); //BUG-1512867
            break;

        case MSG_NOTIFY_PRGBOX_CLICK:
            ListStopFling(list_ctrl_ptr);
            break;

        case MSG_CTL_PRGBOX_MOVEUP_ONE:
        case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
        case MSG_CTL_PRGBOX_MOVEUP_PAGE:
        case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
        case MSG_CTL_PRGBOX_MOVE:
        case MSG_CTRL_LETTER_CHANGED:
            if (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state)
            {
                int32       top_item_index = (int32)(*(uint32 *)param);
                wchar       tip_word = 0;

#ifdef GUIF_PRGBOX_LETTERSCROLL

                if (GUIPRGBOX_STYLE_LETTERS_SCROLL == list_ctrl_ptr->scroll_bar_type)
                {
                    uint16      item_index = (uint16)(*(uint16 *)param);

                    tip_word = CTRLLETTERS_GetWchar((((CTRLBASE_OBJ_T *)list_ctrl_ptr->prgbox_ptr)->handle), item_index);
                    item_index = ListGetTopIndexByFirstLetter(list_ctrl_ptr, tip_word);//
                    CTRLLIST_SetTopItemIndex(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, item_index);
                    CTRLLIST_SetCurItemIndex(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, item_index);
                    MMK_SendMsg(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_CTL_PAINT, PNULL);
                }
                else
#endif
                {
                    ListStopTextAnim(list_ctrl_ptr);
                    ListHandleTpScroll(top_item_index, list_ctrl_ptr);
                }

                //只有在显示滚动条的时候才需要显示tips
                ListDispSplitTip(list_ctrl_ptr, tip_word);
                GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, msg_id);
            }

            break;
#endif

        case MSG_NOTIFY_ANIM_DISPLAY:
            if (MMK_TP_SLIDE_NONE != list_ctrl_ptr->slide_state && PNULL != param)
            {
                *(BOOLEAN *)param = FALSE;
            }

            break;

        case MSG_CTL_ANIM_DISPLAY_IND:
        {
            GUIANIM_DISPLAY_IND_T anim_display_ind = *(GUIANIM_DISPLAY_IND_T *)param;

            ListSendAnimDispResultToWin(list_ctrl_ptr, MSG_NOTIFY_LIST_ANIM_DISPLAY_IND, param);

            if (GUIANIM_RESULT_INTERRUPT != anim_display_ind.result)
            {
                //send msg to win,append the next item content
                ListNotifyAppendItemContent(list_ctrl_ptr);
            }
        }
        break;

        //anim update bg
        case MSG_NOTIFY_UPDATE:

            //非滑动状态
            if (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state && 0 == list_ctrl_ptr->tp_move_timer_id)
            {
                GUI_RECT_T update_rect = *(GUI_RECT_T *)param;
                GUI_RECT_T item_rect  = {0};
                GUI_LCD_DEV_INFO lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

                LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

                if (GUI_IntersectRect(&update_rect, update_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect))
                {
                    if (GUI_IntersectRect(&item_rect, update_rect, item_rect))
                    {
                        if (!LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ON_3D))
                        {
                            if (list_ctrl_ptr->display_style.is_hl_disappear)
                            {
                                ListDrawBackgroundByRect(list_ctrl_ptr, &update_rect);
                            }
                            else
                            {
                                ListDrawHighlightBackground(list_ctrl_ptr, &item_rect, &lcd_dev_info);
                            }
                        }
                    }
                    else
                    {
                        ListDrawBackgroundByRect(list_ctrl_ptr, &update_rect);
                    }
                }
            }

            break;

        case MSG_CTL_PROPERTY_CHANGE:
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get valid rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetValidRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *rect_ptr
)
{
    BOOLEAN result = FALSE;
    GUI_RECT_T valid_rect = {0};
    GUI_RECT_T lcd_rect = {0};

    if (PNULL != list_ctrl_ptr)
    {
        lcd_rect = MMITHEME_GetFullScreenRect();

        if (GUI_IntersectRect(&valid_rect, lcd_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect)
            && GUI_IntersectRect(&valid_rect, valid_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect)
            && GUI_IntersectRect(&valid_rect, valid_rect, LIST_AdjustRect(list_ctrl_ptr)))
        {
            *rect_ptr = valid_rect;

            if (!GUI_IsRectEmpty(valid_rect))
            {
                result = TRUE;
            }
        }
    }

    return result;
}

#ifdef MMI_READMENU_ENABLE
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: violent.wei
//  Note: 当有focus item变化时，朗读list内容
/*****************************************************************************/
LOCAL void ListHandleTextToSpeech(
    CTRLLIST_OBJ_T *list_ctrl_ptr //in
)
{
    if (MMITheme_IsControlTTSOpen())
    {
        // 如果有focus项变化，才会朗读
        if ((MMK_IsActiveCtrl(list_ctrl_ptr->handle)) &&
            list_ctrl_ptr->total_item_num > 0 && list_ctrl_ptr->cur_msg_loop_item_idx != list_ctrl_ptr->cur_item_idx)
        {
            uint32 i = 0, j = 0;
            MMI_STRING_T wstr[GUIITEM_CONTENT_MAX_NUM] = {0};
            CTRLLIST_ITEM_INFO_T *item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

            if (PNULL != item_ptr && PNULL != item_ptr->item.data_ptr)
            {
                // 将所有项是text buffer和text id的全部选出来，进行朗读
                for (; i < GUIITEM_CONTENT_MAX_NUM; i ++)
                {
                    if (GUIITEM_DATA_TEXT_ID == item_ptr->item.data_ptr->item_content[i].item_data_type)
                    {
                        MMITHEME_GetLabelTextByLang(item_ptr->item.data_ptr->item_content[i].item_data.text_id, &wstr[j]);
                        j ++;
                    }
                    else if (GUIITEM_DATA_TEXT_BUFFER == item_ptr->item.data_ptr->item_content[i].item_data_type)
                    {
                        wstr[j].wstr_ptr = item_ptr->item.data_ptr->item_content[i].item_data.text_buffer.wstr_ptr;
                        wstr[j].wstr_len = item_ptr->item.data_ptr->item_content[i].item_data.text_buffer.wstr_len;
                        j ++;
                    }
                }

                if (j > 0)
                {
                    MMITheme_PlayControlText(wstr, j);
                }

                list_ctrl_ptr->cur_msg_loop_item_idx = list_ctrl_ptr->cur_item_idx;
            }
        }
    }
}
#endif


/*****************************************************************************/
//  Description : list set rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetRect(
    CTRLBASE_OBJ_T *iguictrl_ptr,
    const GUI_RECT_T *rect_ptr
)
{
    int32           offset_y = 0;
    BOOLEAN         result = FALSE;
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      inter_rect = {0};
    GUI_RECT_T      valid_rect = {0};
    CTRLLIST_OBJ_T *list_ctrl_ptr = PNULL;

    if ((PNULL == iguictrl_ptr) || (PNULL == rect_ptr))
    {
        return FALSE;
    }

    list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (!GUI_EqualRect(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect, *rect_ptr))
    {
#ifdef TOUCH_PANEL_SUPPORT
        // 停止fling状态
        ListStopFling(list_ctrl_ptr);
#endif

        list_ctrl_ptr->is_pos_dirty = TRUE;

        // 如果相等，不改变offset
        if (GUI_IsRectSizeEqual(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect, *rect_ptr))
        {
            list_ctrl_ptr->offset_y +=  rect_ptr->top - ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.top;

            // 重新设置rect区域
            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect = *rect_ptr;
            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect = *rect_ptr;

#ifdef UI_MULTILAYER_SUPPORT
            ListReleaseItemLayer(list_ctrl_ptr);
#endif

            // 设置横竖屏区域
            GUI_SetVOrHRect(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, (GUI_RECT_T *)rect_ptr, &(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->both_rect));

            list_ctrl_ptr->is_full_dirty = TRUE;
            LIST_ResetAllItemDisp(list_ctrl_ptr);
        }
        else
        {
            list_ctrl_ptr->is_full_dirty = TRUE;

            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect = *rect_ptr;
            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect = *rect_ptr;
            // Layer区域和大小发生改变，重新初始化Layer，同时初始化Offset Y
#ifdef UI_MULTILAYER_SUPPORT
            ListReleaseItemLayer(list_ctrl_ptr);
#endif
            ListResetOffsetY(list_ctrl_ptr);

            // 设置横竖屏区域
            GUI_SetVOrHRect(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, (GUI_RECT_T *)rect_ptr, &(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->both_rect));

            // 释放所有的DATA
            LIST_DestroyAllItemData(list_ctrl_ptr, TRUE);

            // 设置Invalid状态
            LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);

            LIST_ResetItemPostion(list_ctrl_ptr);

            // 矫正cur index, offset_y等
            ListValidateCtrl(list_ctrl_ptr, TRUE, TRUE);

#ifdef GUIF_LIST_PDASTYLE
            // 获得Top Item 区域
            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->top_item_idx);
#else
            // 获得当前Item 区域
            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
#endif

            // 矫正 offset Y，使当前Item完全显示
            if (LIST_GetValidRect(list_ctrl_ptr, &valid_rect)
                && GUI_IntersectRect(&inter_rect, item_rect, valid_rect)
                && !GUI_RectIsCovered(valid_rect, item_rect))
            {
                if (item_rect.top < inter_rect.top)
                {
                    offset_y = inter_rect.top - item_rect.top;
                }
                else if (item_rect.bottom > inter_rect.bottom)
                {
                    offset_y = inter_rect.bottom - item_rect.bottom;
                }
                else
                {

                }

                list_ctrl_ptr->offset_y += offset_y;

                LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
            }

            // Stop 文字滚动Timer
            ListStopTextAnim(list_ctrl_ptr);

            //destroy progress
            if (PNULL != list_ctrl_ptr->prgbox_ptr)
            {
                MMK_DestroyControl(IGUICTRL_GetCtrlHandle(list_ctrl_ptr->prgbox_ptr));
                list_ctrl_ptr->prgbox_ptr = PNULL;
            }
        }

#ifdef UI_MULTILAYER_SUPPORT
        ListReleaseTipLayer(list_ctrl_ptr);
        LIST_ReleaseSplitLayer(list_ctrl_ptr);
#endif

#ifdef GUIF_PRGBOX_LETTERSCROLL

        if (GUIPRGBOX_STYLE_LETTERS_SCROLL == list_ctrl_ptr->scroll_bar_type)
        {
            ListSetScrollBarParam(list_ctrl_ptr);
        }

#endif
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set control boder info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetBorder(
    CTRLBASE_OBJ_T    *iguictrl_ptr,
    GUI_BORDER_T  *border_ptr
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (PNULL != border_ptr && PNULL != list_ctrl_ptr)
    {
        GUI_RECT_T     list_rect = {0};
        GUI_BORDER_TYPE_E border_type = list_ctrl_ptr->display_style.border.type;

        result = TRUE;
        list_ctrl_ptr->display_style.border.color = border_ptr->color;
        list_ctrl_ptr->display_style.border.type = border_ptr->type;
        list_ctrl_ptr->display_style.border.width = border_ptr->width;

        if (border_type != list_ctrl_ptr->display_style.border.type) // 需要矫正offset Y
        {
            list_rect = LIST_AdjustRect(list_ctrl_ptr);

            if (!GUI_IsInvalidRect(list_rect) && list_rect.top != ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.top)
            {
                list_ctrl_ptr->offset_y += list_rect.top - ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.top;
                list_ctrl_ptr->is_full_dirty = TRUE;
                list_ctrl_ptr->is_pos_dirty = TRUE;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : 调整控件的区域
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LIST_AdjustRect(
    const CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    BOOLEAN     is_border = TRUE;
    GUI_RECT_T  disp_rect = {0};

    if (PNULL != list_ctrl_ptr)
    {
        //text is exist border
        if ((!list_ctrl_ptr->is_permit_border) ||
            (GUI_BORDER_NONE == list_ctrl_ptr->display_style.border.type))
        {
            is_border = FALSE;
        }

        if (is_border)
        {
            disp_rect = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect;

            disp_rect.left += list_ctrl_ptr->display_style.border.width;
            disp_rect.top += list_ctrl_ptr->display_style.border.width;
            disp_rect.right -= list_ctrl_ptr->display_style.border.width;
            disp_rect.bottom -= list_ctrl_ptr->display_style.border.width;
        }
        else
        {
            disp_rect = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect;
        }
    }

    return disp_rect;
}

/*****************************************************************************/
//  Description : draw border
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawBorder(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    if ((PNULL != list_ctrl_ptr) && (list_ctrl_ptr->is_permit_border))
    {
        GUI_DisplayBorder(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect,
                          ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect,
                          &list_ctrl_ptr->display_style.border,
                          &((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info);
    }

    return;
}

/*****************************************************************************/
//  Description : Set the back ground color.
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
LOCAL BOOLEAN ListSetBackGround(
    CTRLBASE_OBJ_T  *iguictrl_ptr,
    GUI_BG_T    *bg_ptr
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (PNULL != bg_ptr && PNULL != iguictrl_ptr)
    {
        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SELFSET_BG, TRUE);

        list_ctrl_ptr->display_style.bg_info = *bg_ptr;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 设置list text font
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认不需要设置
/*****************************************************************************/
PUBLIC BOOLEAN ListSetTextFont(
    CTRLBASE_OBJ_T     *iguictrl_ptr,
    GUI_FONT_ALL_T *font_ptr
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (PNULL != list_ctrl_ptr && PNULL != font_ptr)
    {
        result = TRUE;

        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_TEXT_COLOR, FALSE);
    }

    return result;
}

/*****************************************************************************/
//  Description : get control font info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetTextFont(
    CTRLBASE_OBJ_T      *iguictrl_ptr,
    GUI_FONT_ALL_T  *font_ptr
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (PNULL != font_ptr && PNULL != list_ctrl_ptr)
    {
        result = TRUE;

        font_ptr->color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);
    }

    return result;
}

/*****************************************************************************/
//  Description : set control is display progress
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetProgress(
    CTRLBASE_OBJ_T   *iguictrl_ptr,
    BOOLEAN      is_display
)
{
    BOOLEAN     result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (PNULL != list_ctrl_ptr)
    {
        result = TRUE;
        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_PRGBAR, is_display);
    }

    return (result);
}

/*****************************************************************************/
//  Description : 设置是否循环滚动
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认时循环滚动，当控件不循环滚动，焦点在最上/最下时，会发送消息给父窗口
/*****************************************************************************/
LOCAL BOOLEAN ListSetCycScroll(
    CTRLBASE_OBJ_T   *iguictrl_ptr,
    BOOLEAN      is_circular
)
{
    BOOLEAN        result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (PNULL != list_ctrl_ptr)
    {
        result = TRUE;
        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_CYC_SCROLL_ITEM, is_circular);
    }

    return result;
}

/*****************************************************************************/
//  Description : 检查是否循环滚动
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认时循环滚动，当控件不循环滚动，焦点在最上/最下时，会发送消息给父窗口
/*****************************************************************************/
LOCAL BOOLEAN ListIsCycScroll(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    BOOLEAN        result = FALSE;

    if (PNULL != list_ctrl_ptr)
    {
        result = LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_CYC_SCROLL_ITEM);
    }

    return result;
}

/*****************************************************************************/
//  Description : get control height by width,include content,border etc.
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN ListGetHeight(
    CTRLBASE_OBJ_T  *iguictrl_ptr,  //in:
    uint16      width,          //in:
    uint16      *height_ptr     //in/out:
)
{
    uint16      height = 0;
    BOOLEAN     result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    GUI_INVALID_PARAM(width);/*lint !e522*/

    if (PNULL != list_ctrl_ptr && PNULL != height_ptr)
    {
        if (list_ctrl_ptr->total_item_num > 0)
        {
            height = ListGetItemTotalHeightByIdx(list_ctrl_ptr, 0, list_ctrl_ptr->total_item_num - 1);
        }
        else
        {
            //get font height
            height = GUI_GetFontHeight(list_ctrl_ptr->display_style.text_empty_font, UNICODE_HANZI);

            height = (uint16)(height + 4);
        }

        //add border
        if ((list_ctrl_ptr->is_permit_border) &&
            (GUI_BORDER_NONE != list_ctrl_ptr->display_style.border.type))
        {
            height = (uint16)(height + (list_ctrl_ptr->display_style.border.width * 2));
        }

        *height_ptr = height;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 发送图片解析结果给应用窗口
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSendAnimDispResultToWin(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    GUILIST_ANIM_DISP_RESULT_T anim_display_ind = {0};
    GUIANIM_DISPLAY_IND_T anim_ind = {0};

    if (NULL == list_ctrl_ptr)
    {
        return;
    }

    if (param != PNULL)
    {
        anim_ind = *(GUIANIM_DISPLAY_IND_T *)param;
    }

    anim_display_ind.result = anim_ind.result;
    anim_display_ind.item_index = list_ctrl_ptr->cur_object_index;

    GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, msg_id, &anim_display_ind);
}

/*****************************************************************************/
//  Description : get item content rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetItemContentRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          item_index,
    uint16          item_content_index,
    GUI_RECT_T     *disp_rect_ptr,//out
    GUI_RECT_T     *clip_rect_ptr //out
)
{
    BOOLEAN result = FALSE;

    if (NULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (item_index < list_ctrl_ptr->total_item_num)
    {
        CTRLLIST_ITEM_INFO_T          *item_ptr  = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);
        GUI_RECT_T                   item_rect       = {0};
        GUI_RECT_T                   icon_rect       = {0};
        GUI_RECT_T                   clip_rect       = {0};

		if (item_ptr)
		{
			LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, item_index);

			icon_rect = item_ptr->content_rect[item_content_index];

			SetRectOffset(&icon_rect, item_rect.left, item_rect.top);

			//防止item rect超出list的rect
			if (GUI_IntersectRect(&icon_rect, icon_rect, item_rect))
			{
				result = GUI_IntersectRect(&clip_rect, icon_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect);
			}

			// 检查区域是否有效，是否为空的区域
			if (!GUI_IsRectEmpty(clip_rect) && result)
			{
				if (PNULL != disp_rect_ptr)
				{
					*disp_rect_ptr = icon_rect;
				}

				if (PNULL != clip_rect_ptr)
				{
					*clip_rect_ptr = clip_rect;
				}

				result = TRUE;
			}
		}
    }

    return result;
}

/*****************************************************************************/
//  Description : send msg to win, notify it append the next item content
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListNotifyAppendItemContent(
    CTRLLIST_OBJ_T   *list_ctrl_ptr
)
{
    BOOLEAN         is_need_content = FALSE;
    uint16          i = 0;
    uint16          j = 0;
    uint16          index = 0;
    CTRLLIST_ITEM_INFO_T *item_ptr  = PNULL;

    if (NULL == list_ctrl_ptr || 0 == list_ctrl_ptr->total_item_num)
    {
        return;
    }

    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_CONTENT, TRUE);

    for (i = list_ctrl_ptr->top_item_idx; i <= list_ctrl_ptr->end_item_idx; i++)
    {
        index = i;// (uint16)( ( list_ctrl_ptr->top_item_idx + i ) % list_ctrl_ptr->total_item_num );

        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

        if (PNULL != item_ptr &&
            LIST_GetItemState(item_ptr, GUIITEM_STATE_NEED_ITEM_CONTENT) &&
            (PNULL != item_ptr->item.data_ptr))
        {
            for (j = 0; j < GUIITEM_CONTENT_MAX_NUM; j++)
            {
                if ((GUIITEM_DATA_ANIM_DATA == item_ptr->item.data_ptr->item_content[j].item_data_type ||
                     GUIITEM_DATA_ANIM_PATH == item_ptr->item.data_ptr->item_content[j].item_data_type) &&
                    (PNULL == item_ptr->inner_handle[j]))
                {
                    // 检查区域是否有效，是否为空的区域
                    if (ListGetItemContentRect(list_ctrl_ptr, index, j, PNULL, PNULL))
                    {
                        is_need_content = TRUE;
                        break;
                    }
                }
            }
        }

        if (is_need_content)
        {
            break;
        }
    }

    if (!is_need_content)
    {
        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_CONTENT, FALSE);
    }
    else
    {
        if (MMK_IsOpenWin(list_ctrl_ptr->win_id)) // Bug-1376378
        {
            list_ctrl_ptr->cur_object_index = index;

            LIST_SendNeedItemContentMsg(list_ctrl_ptr, index, j);
        }
    }
}

/*****************************************************************************/
//  Description : get control display rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetDisplayRect(
    CTRLBASE_OBJ_T   *iguictrl_ptr,  //in:
    GUI_RECT_T   *rect_ptr       //in/out:
)
{
    BOOLEAN         result = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (PNULL != iguictrl_ptr && PNULL != rect_ptr)
    {
        // 当用在dropdownlist中时，一旦listbox显示，那它的区域就是全屏的了，
        // 后续的鼠标消息都要扔给它来处理。如果不在它的区域内时，需要通知
        // dropdownlist将其关闭。
        if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_DROPDOWNLIST))
        {
            *rect_ptr     = MMITHEME_GetClientRect();
            rect_ptr->top = 0;
        }
        else
        {
            *rect_ptr = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect;
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set control display rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetDisplayRect(
    CTRLBASE_OBJ_T     *iguictrl_ptr,  //in:
    const GUI_RECT_T   *rect_ptr,      //in:
    BOOLEAN            is_update       //in:
)
{
    BOOLEAN         result = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;
    GUI_RECT_T bar_rect = {0};

    if (PNULL != iguictrl_ptr && PNULL != rect_ptr)
    {
        list_ctrl_ptr->is_full_dirty = TRUE;
        list_ctrl_ptr->is_pos_dirty = TRUE;

        if (!GUI_EqualRect(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect, *rect_ptr))
        {
            ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect = *rect_ptr;

            // 显示区域改变了，offset Y不变的情况下，应该重新设置top index和bottom index
            LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT
            ListReleaseItemLayer(list_ctrl_ptr);
#endif
            // 显示区域改变的时候，需要重新申请Layer
            /*当list的显示区域改变的时候，子控件的显示区域也需要跟着改变 by arvin begin*/
            bar_rect = ListGetScrollBarRect(list_ctrl_ptr);  //获得默认显示区域

            if (list_ctrl_ptr->prgbox_ptr)
            {
                if (!GUI_IntersectRect(&bar_rect, bar_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect)) //跟当前list区域做交集
                {
                    //如果没有交叠区域就将区域初始化为0
                    bar_rect.bottom = 0;
                    bar_rect.left = 0;
                    bar_rect.top = 0;
                    bar_rect.right = 0;
                }

                //设置进度条的显示区域，这里要说明一下，目前的进度条实际上显示区域跟rect没有区分，共用，所以调用这个接口
                IGUICTRL_SetRect((IGUICTRL_T *)list_ctrl_ptr->prgbox_ptr, &bar_rect);
            }

            /*当list的显示区域改变的时候，子控件的显示区域也需要跟着改变 by arvin end*/

#ifdef UI_MULTILAYER_SUPPORT
            LIST_ReleaseSplitLayer(list_ctrl_ptr);
#endif
            result = TRUE;
        }

        if ((is_update) &&
            (MMK_IsFocusWin(list_ctrl_ptr->win_id)))
        {
            if (result)
            {
                LIST_DestroyAllItemData(list_ctrl_ptr, TRUE);
            }

            ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_TITLE | INNER_STATE_UPDATE_PRG | INNER_STATE_UPDATE_SK);

            if (!list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
            {
                ListUpdateHighlightItem(list_ctrl_ptr);
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get control select rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetSelectRect(
    CTRLBASE_OBJ_T     *iguictrl_ptr,  //in:
    GUI_RECT_T     *rect_ptr       //in/out:
)
{
    int32       offset_y = 0;
    BOOLEAN     result = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (PNULL != list_ctrl_ptr && PNULL != rect_ptr)
    {
        offset_y = ListGetRecoverSpace(list_ctrl_ptr);

        if (0 != offset_y)
        {
            list_ctrl_ptr->offset_y += offset_y;
            LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
        }

        result = LIST_CalculateListItemRect(rect_ptr, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
    }

    return result;
}

/*****************************************************************************/
//  Description : 判断当前item的位置，是最后一个还是第一个
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListChangeCurItemPos(
    CTRLBASE_OBJ_T    *iguictrl_ptr,  //in:
    BOOLEAN       *is_first_ptr,  //in/out:
    BOOLEAN       *is_last_ptr    //in/out:
)
{
    BOOLEAN     result = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (PNULL != list_ctrl_ptr)
    {
        if (0 == list_ctrl_ptr->cur_item_idx)
        {
            if (PNULL != is_first_ptr)
            {
                *is_first_ptr = TRUE;
            }
        }

        if (list_ctrl_ptr->cur_item_idx == (list_ctrl_ptr->total_item_num - 1))
        {
            if (PNULL != is_last_ptr)
            {
                *is_last_ptr = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : set list current item pos
//  Global resource dependence :
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetCurItemPos(
                                CTRLBASE_OBJ_T  *iguictrl_ptr,  //in:
                                BOOLEAN         is_first,       //in:
                                BOOLEAN         is_last         //in:
                                )
{
    BOOLEAN         result = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = (CTRLLIST_OBJ_T *)iguictrl_ptr;

    if (is_first)
    {
        result = CTRLLIST_SetCurItemIndex(iguictrl_ptr->ctrl_id,0);
    }
    else if (is_last)
    {
        if (0 < list_ctrl_ptr->total_item_num)
        {
            result = CTRLLIST_SetCurItemIndex(iguictrl_ptr->ctrl_id,list_ctrl_ptr->total_item_num - 1);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : init the list control
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void ListCtrlInit(
    CTRLBASE_OBJ_T          *obj_ptr,
    CTRLLIST_INIT_PARAM_T   *list_param_ptr
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = (CTRLLIST_OBJ_T *)obj_ptr;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *)obj_ptr;
    LCD_INFO_T lcd_info = {0};

    //SCI_TRACE_LOW:"ListCtrlInit: list_ctrl_ptr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_10093_112_2_18_3_20_3_220, (uint8 *)"d", list_ctrl_ptr);

    base_ctrl_ptr->lcd_dev_info = *(MMITHEME_GetDefaultLcdDev());

    GUILCD_GetInfo(base_ctrl_ptr->lcd_dev_info.lcd_id, &lcd_info);
    list_ctrl_ptr->item_base_width = MIN(lcd_info.lcd_width, lcd_info.lcd_height);

    // set list type
    if (GUILIST_RADIOLIST_E == list_param_ptr->type)
    {
        list_ctrl_ptr->marking_type = CTRLLIST_MARKING_FIXED;
        list_ctrl_ptr->mark_type = CTRLLIST_MARK_RADIO;
#ifdef ADULT_WATCH_SUPPORT
        list_ctrl_ptr->is_mark_icon_left = TRUE;
#else
        list_ctrl_ptr->is_mark_icon_left = FALSE;
#endif
    }
    else if (GUILIST_CHECKLIST_E == list_param_ptr->type)
    {
        list_ctrl_ptr->marking_type = CTRLLIST_MARKING_FIXED;
        list_ctrl_ptr->mark_type = CTRLLIST_MARK_CHECK;
        list_ctrl_ptr->is_mark_icon_left = FALSE;
    }
    else
    {
        list_ctrl_ptr->marking_type = CTRLLIST_MARKING_NONE;
        list_ctrl_ptr->mark_type = CTRLLIST_MARK_CHECK;
        list_ctrl_ptr->is_mark_icon_left = FALSE;
    }

    // set default value of the follow valiables
    list_ctrl_ptr->title_style     = GUILIST_TITLE_ONLY;
    list_ctrl_ptr->title_index_type = GUILIST_TITLE_INDEX_NONE;
    list_ctrl_ptr->win_id = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);
    list_ctrl_ptr->scroll_bar_type = list_param_ptr->scroll_bar_type;
    list_ctrl_ptr->is_transparent = TRUE;
    list_ctrl_ptr->is_disable_3dmmi = FALSE;
    // 默认情况下，item不滚动，则不会显示半个字
    list_ctrl_ptr->is_handle_starhash = TRUE;

    //set edit permit display border
    list_ctrl_ptr->is_permit_border = TRUE;
    list_ctrl_ptr->is_cus_blt_layer = FALSE;

    LIST_SetMaxItemByPtr(list_ctrl_ptr,
                         list_param_ptr->max_item_num,
                         list_param_ptr->is_dynamic_item_data);

    LIST_SetMaxSelectedItemByPtr(list_ctrl_ptr, list_param_ptr->max_selected_num);

    //Set display style
    if (!MMITHEME_GetListStyle(&list_ctrl_ptr->display_style))
    {
        SCI_PASSERT(FALSE, ("ListCtrlInit: the list style is error!")); /*assert verified*/
    }

    base_ctrl_ptr->bg = MMITHEME_GetCommonBg();

    // 计算文字距离左右边界的位置list_ctrl_ptr->left_space 和list_ctrl_ptr->right_space
    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR
                           | GUILIST_STATE_NEED_PRGBAR
#ifndef MMI_PDA_SUPPORT
                           | GUILIST_STATE_NEED_TITLE
                           | GUILIST_STATE_NEED_SOFTKEY
                           | GUILIST_STATE_CYC_SCROLL_ITEM
#endif
                           | GUILIST_STATE_NEED_PRGBAR_BLOCK,
                           TRUE);

    list_ctrl_ptr->is_full_dirty = TRUE;
    list_ctrl_ptr->is_pos_dirty = TRUE;
    list_ctrl_ptr->total_item_height = 0;
    list_ctrl_ptr->is_support_slide_dec_anim = TRUE;

    // 创建层
    list_ctrl_ptr->item_layer.block_id = UILAYER_NULL_HANDLE;
    list_ctrl_ptr->tip_layer.block_id = UILAYER_NULL_HANDLE;
    list_ctrl_ptr->split_layer.block_id = UILAYER_NULL_HANDLE;

#if defined (GUILIST_PDASTYLE_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SPLIT_LINE, TRUE);
#endif

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENTER | GUILIST_STATE_SLIDE_IN, FALSE);

    // 设置支持横向滑动item的状态为false
    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_SLIDE_ITEM, FALSE);

    return;
}

/*****************************************************************************/
//  Description : Set the max item of list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetMaxItem(
    MMI_CTRL_ID_T ctrl_id, // control id
    uint16        max_item_num,
    BOOLEAN       is_dynamic_item_data
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return LIST_SetMaxItemByPtr(list_ctrl_ptr, max_item_num, is_dynamic_item_data);
}

/*****************************************************************************/
//  Description : Set the max item of list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SetMaxItemByPtr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16        max_item_num,
    BOOLEAN       is_dynamic_item_data
)
{
    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    // destory data
    LIST_DestroyAllItemData(list_ctrl_ptr, FALSE);

    list_ctrl_ptr->max_item_num = max_item_num;

    // set state
    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA, is_dynamic_item_data);

    // 申请内存
    ListAllocAllItemsSpace(list_ctrl_ptr);

    // reset
    ListResetItem(list_ctrl_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : Set the allowed max select item num  item of list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetMaxSelectedItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 mark_cnt_max
)
{
    return LIST_SetMaxSelectedItemByPtr(ListGetPtr(ctrl_id), mark_cnt_max);
}

/*****************************************************************************/
//  Description : Set the allowed max select item num  item of list
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SetMaxSelectedItemByPtr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 mark_cnt_max
)
{
    BOOLEAN result = TRUE;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (CTRLLIST_MARK_RADIO == list_ctrl_ptr->mark_type)
    {
        list_ctrl_ptr->mark_cnt_max = 1;
    }
    else
    {
        list_ctrl_ptr->mark_cnt_max = mark_cnt_max;
    }

    return result;
}

/*****************************************************************************/
//  Description : Set the title style of list
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTitleStyle(
    MMI_CTRL_ID_T              ctrl_id,    //IN:
    GUILIST_TITLE_STYLE_E  title_style //IN:
)
{
    BOOLEAN         result    = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    //set list  title style
    if (PNULL != list_ctrl_ptr && title_style < GUILIST_TITLE_MAX)
    {
        list_ctrl_ptr->title_style = title_style;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : remove the appointed item base the position
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListRemoveItem(
    CTRLLIST_OBJ_T    *list_ctrl_ptr,
    uint16            index // 指定的Position, from 0 to total_item_num-1
)
{
    CTRLLIST_ITEM_INFO_T *removed_item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (index >= list_ctrl_ptr->total_item_num)
    {
        return FALSE;
    }

    removed_item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

    if (PNULL == removed_item_ptr)
    {
        return FALSE;
    }

    //split item不可以删除
    if (GUIITEM_STATE_SPLIT & removed_item_ptr->item.state)
    {
        return FALSE;
    }

    list_ctrl_ptr->total_item_height -= (int32)LIST_GetItemHeight(list_ctrl_ptr, index);

    //update the selected_item_num
    LIST_UnSelectItem(list_ctrl_ptr, index);

    if (LIST_DestroyItemData(list_ctrl_ptr, removed_item_ptr, FALSE))
    {
        //终止当前正在解码的item
        LIST_DestroyItemDecode(list_ctrl_ptr);
    }

    /* memmove */
    if (list_ctrl_ptr->total_item_num - 1 - index > 0)
    {
        memmove(list_ctrl_ptr->item_info_ptr + index,
                list_ctrl_ptr->item_info_ptr + index + 1,
                (list_ctrl_ptr->total_item_num - 1 - index) * sizeof(CTRLLIST_ITEM_INFO_T));
    }

    //update total_item_num
    list_ctrl_ptr->total_item_num--;

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);

    ListValidateCtrl(list_ctrl_ptr , FALSE, FALSE);

    list_ctrl_ptr->is_pos_dirty = TRUE;

    return TRUE;
}

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_InsertItem(
    MMI_CTRL_ID_T            ctrl_id, // control id
    const CTRLLIST_ITEM_T    *item_ptr,   // 指定的Item
    uint16                   pos     // 指定的位置, from 0 to total_item_num
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return ListInsertItem(list_ctrl_ptr, item_ptr, pos);
}

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_AppendItem(
    MMI_CTRL_ID_T            ctrl_id, // control id
    const CTRLLIST_ITEM_T *item_ptr    // 指定的Item
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return ListInsertItem(list_ctrl_ptr, item_ptr, list_ctrl_ptr->total_item_num);
}

/*****************************************************************************/
//  Description : Replace the item in position
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_ReplaceItem(
    MMI_CTRL_ID_T         ctrl_id,   // control id
    const CTRLLIST_ITEM_T *replace_item_ptr,  // 指定的Item
    uint16                pos                // 指定的位置, from 0 to total_item_num-1
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return ListReplaceItem(list_ctrl_ptr, replace_item_ptr, pos);
}

/*****************************************************************************/
//  Description : remove the appointed item base the position
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_RemoveItem(
    MMI_CTRL_ID_T ctrl_id, // control id
    uint16 index      // 指定的Position, from 0 to total_item_num-1
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return ListRemoveItem(list_ctrl_ptr, index);
}

/*****************************************************************************/
//  Description : Remove all items
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_RemoveAllItems(
    MMI_CTRL_ID_T ctrl_id  // control id
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return ListRemoveAllItems(list_ctrl_ptr);
}

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence :
//  Author:yelianna
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemData(
    MMI_CTRL_ID_T                     ctrl_id, // control id
    const GUILIST_ITEM_DATA_T *item_data_ptr,
    uint16                         pos
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return LIST_SetItemDataByPtr(list_ctrl_ptr, item_data_ptr, pos);
}

/*****************************************************************************/
//  Description : set scroll bar style
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetListScrollBarStyle(
    MMI_CTRL_ID_T    ctrl_id, // control id
    GUIPRGBOX_STYLE_E scroll_style
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (list_ctrl_ptr->scroll_bar_type != scroll_style)
    {
        list_ctrl_ptr->is_need_reinit_scrollbar = TRUE;
        list_ctrl_ptr->scroll_bar_type = scroll_style;

        // 需要重新创建，因此需要先销毁
        if (PNULL != list_ctrl_ptr->prgbox_ptr)
        {
            MMK_DestroyControl(GUICTRL_GetCtrlHandle(list_ctrl_ptr->prgbox_ptr));

            list_ctrl_ptr->prgbox_ptr = NULL;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : set
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetItemContentByPtr(
    CTRLLIST_OBJ_T           *list_ctrl_ptr,
    const GUIITEM_CONTENT_T  *item_content_ptr,
    uint16                   item_index,
    uint16                   item_content_index,
    BOOLEAN                  is_syn_decode,
    BOOLEAN                  is_update
)
{
    GUI_RECT_T           icon_rect = {0};
    GUI_RECT_T           clip_rect = {0};
    GUIITEM_DATA_TYPE_E  type      = GUIITEM_DATA_NONE;
    CTRLLIST_ITEM_INFO_T *item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

    if (PNULL == list_ctrl_ptr || PNULL == item_content_ptr)
    {
        return FALSE;
    }

    if (item_index >= list_ctrl_ptr->total_item_num)
    {
        return FALSE;
    }

    if (PNULL == item_ptr || PNULL == item_ptr->item.data_ptr || item_content_index >= GUIITEM_CONTENT_MAX_NUM)
    {
        return FALSE;
    }

    // 可能更改类型，需要释放content
    LIST_DestroyItemInlineCtrl(item_ptr, item_content_index);

    item_ptr->item.data_ptr->item_content[item_content_index] = *item_content_ptr;
    type = item_ptr->item.data_ptr->item_content[item_content_index].item_data_type;

    if (!ListGetItemContentRect(list_ctrl_ptr, item_index, item_content_index, &icon_rect, &clip_rect))
    {
        return FALSE;
    }

    switch (type)
    {
        case GUIITEM_DATA_ANIM_DATA:
        case GUIITEM_DATA_ANIM_PATH:
        {
            IGUICTRL_T *ctrl_ptr = PNULL;
            GUIANIM_RESULT_E        anim_result     = GUIANIM_RESULT_FAIL;
            GUIANIM_CTRL_INFO_T     ctrl_info       = {0};
            GUIANIM_DISPLAY_INFO_T  display_info    = {0};
            MMI_CONTROL_CREATE_T    create          = {0};
            GUIANIM_INIT_DATA_T     init_data       = {0};
            GUI_LCD_DEV_INFO        lcd_dev_info    = {0};

            SCI_ASSERT(0 == item_ptr->inner_handle[item_content_index]);   /*assert verified*/

            init_data.both_rect.h_rect = init_data.both_rect.v_rect = icon_rect;
            create.ctrl_id = 0;
            create.guid    = SPRD_GUI_ANIM_ID;
            create.parent_win_handle = list_ctrl_ptr->win_id;
            create.parent_ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
            create.init_data_ptr = &init_data;

            ctrl_ptr = MMK_CreateControl(&create);

            SCI_ASSERT(PNULL != ctrl_ptr);   /*assert verified*/

            item_ptr->inner_handle[item_content_index] = ((CTRLBASE_OBJ_T *)ctrl_ptr)->handle;

            GUIANIM_SetDefaultIcon(item_ptr->inner_handle[item_content_index],
                                   list_ctrl_ptr->display_style.icon_default_id,
                                   list_ctrl_ptr->display_style.icon_error_id
                                  );

            lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

            GUIAPICTRL_SetLcdDevInfo(item_ptr->inner_handle[item_content_index], &lcd_dev_info);

            //set anim param
            ctrl_info.is_ctrl_id = FALSE;
            ctrl_info.ctrl_id    = 0;
            ctrl_info.ctrl_ptr   = ctrl_ptr;

            display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
            display_info.is_play_once      = FALSE;
            display_info.is_syn_decode     = is_syn_decode;
            display_info.is_update         = is_update;
            display_info.is_disp_one_frame = TRUE;
            display_info.is_handle_transparent = TRUE;

            // 设置是否同步解码
            if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SYNC_DECODE_ANIM))
            {
                display_info.is_syn_decode = TRUE;
            }

            // 设置是否缩放
            if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ZOOM_ANIM))
            {
                display_info.is_zoom = TRUE;
            }

            GUIANIM_NotifyUpdateByPtr(GUIANIM_GetPtr(item_ptr->inner_handle[item_content_index]), TRUE);

            GUIANIM_SetCtrlRect(item_ptr->inner_handle[item_content_index], &icon_rect);

            GUIANIM_SetDisplayRect(item_ptr->inner_handle[item_content_index], &clip_rect, FALSE);

            if (GUIITEM_DATA_ANIM_DATA == type)
            {
                anim_result = GUIANIM_SetParam(&ctrl_info,
                                               item_ptr->item.data_ptr->item_content[item_content_index].item_data.anim_data_ptr,
                                               PNULL,
                                               &display_info
                                              );
            }
            else
            {
                anim_result = GUIANIM_SetParam(&ctrl_info,
                                               PNULL,
                                               item_ptr->item.data_ptr->item_content[item_content_index].item_data.anim_path_ptr,
                                               &display_info
                                              );
            }
        }
        break;

        default:
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_10616_112_2_18_3_20_4_224, (uint8 *)"d", type);
            break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence :
//  Author:yelianna
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_ListSetItemContentByPtr(
    CTRLLIST_OBJ_T       *list_ctrl_ptr,
    const GUIITEM_CONTENT_T *item_content_ptr,
    uint16                    item_index,
    uint16                    item_content_index
)
{
    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ON_ITEM_CONTENT))
    {
        return ListSetItemContentByPtr(list_ctrl_ptr, item_content_ptr, item_index, item_content_index, FALSE, TRUE);
    }
    else
    {
        return FALSE;
    }
}

PUBLIC BOOLEAN LIST_ListSetItemWithAlphaByPtr(
        CTRLLIST_OBJ_T       *list_ctrl_ptr,
        const GUIITEM_CONTENT_T *item_content_ptr,
        uint16                    item_index,
        uint16                    item_content_index
        )
{
    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ON_ITEM_CONTENT))
    {
        return ListSetItemContentByPtr(list_ctrl_ptr, item_content_ptr, item_index, item_content_index, FALSE, TRUE) ;
    }
    else
    {
        return FALSE;
    }
}

PUBLIC BOOLEAN CTRLLIST_SetItemContentWithAlpha(
	MMI_CTRL_ID_T			 ctrl_id, // control id
	const GUIITEM_CONTENT_T *item_content_ptr,
	uint16					  item_index,
	uint16					  item_content_index
)
{
	return LIST_ListSetItemWithAlphaByPtr(ListGetPtr(ctrl_id), item_content_ptr, item_index, item_content_index) ;
}


/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence :
//  Author:yelianna
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemContent(
    MMI_CTRL_ID_T            ctrl_id, // control id
    const GUIITEM_CONTENT_T *item_content_ptr,
    uint16                    item_index,
    uint16                    item_content_index
)
{
    return LIST_ListSetItemContentByPtr(ListGetPtr(ctrl_id), item_content_ptr, item_index, item_content_index);
}

/*****************************************************************************/
//  Description : set
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemContentExt(
    MMI_CTRL_ID_T           ctrl_id, // control id
    const GUIITEM_CONTENT_T *item_content_ptr,
    uint16                  item_index,
    uint16                  item_content_index,
    BOOLEAN                 is_syn_decode, // 只对动画控件有效
    BOOLEAN                 is_update // 只对动画空间有效
)
{
    return ListSetItemContentByPtr(ListGetPtr(ctrl_id), item_content_ptr, item_index, item_content_index, is_syn_decode, is_update);
}


/*****************************************************************************/
//  Description : Remove all items
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListRemoveAllItems(
    CTRLLIST_OBJ_T    *list_ctrl_ptr
)
{
    if (NULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (PNULL != list_ctrl_ptr->item_info_ptr)
    {
        LIST_DestroyAllItemData(list_ctrl_ptr, FALSE);

        // init the space to 0
        SCI_MEMSET((void *)list_ctrl_ptr->item_info_ptr, 0, (list_ctrl_ptr->max_item_num * sizeof(CTRLLIST_ITEM_INFO_T)));
    }

    ListResetItem(list_ctrl_ptr);

    list_ctrl_ptr->is_full_dirty = TRUE;
    list_ctrl_ptr->is_pos_dirty = TRUE;

    ListValidateCtrl(list_ctrl_ptr , FALSE, FALSE);

    return TRUE;
}

/*****************************************************************************/
//  Description : Get item rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: return TRUE/FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetItemRect(
    MMI_CTRL_ID_T  ctrl_id,    //in:
    uint16         item_index, //in
    GUI_RECT_T     *rect_ptr   //in/out
)
{
    int32 offset_y = 0;
    BOOLEAN result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr && PNULL != rect_ptr)
    {
        offset_y = ListGetRecoverSpace(list_ctrl_ptr);

        if (0 != offset_y)
        {
            list_ctrl_ptr->offset_y += offset_y;
            LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
        }

        result = LIST_CalculateListItemRect(rect_ptr, list_ctrl_ptr, item_index);
    }

    return result;
}

#ifdef ATEST_SUPPORT
/*****************************************************************************/
// 	Description : get item content rect
//	Global resource dependence : 
//  Author: lin.chen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN Atest_GetItemContentRect(
                                 MMI_CTRL_ID_T   ctrl_id,
                                 uint16          item_index,
                                 uint16          item_content_index,
                                 GUI_RECT_T*     disp_rect_ptr,//out
                                 GUI_RECT_T*     clip_rect_ptr //out
                                 )
{
        if(CTRLLIST_GetItemRect(ctrl_id, item_index, clip_rect_ptr))
        {
              disp_rect_ptr = clip_rect_ptr;
    		return TRUE;
    	}

	return FALSE;
}
#endif //ATEST_SUPPORT
/*****************************************************************************/
//  Description : Get selected check item indexs
//  Global resource dependence :
//  Author:Great.Tian
//  Note: return selected check item number
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetSelectedItemIndex(
    MMI_CTRL_ID_T    ctrl_id,    //in:
    uint16          *idx_ptr,    //in/out:selected index array,may PNULL
    uint16           array_len   //in
)
{
    uint16          selected_num = 0;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    if (CTRLLIST_MARKING_NONE != list_ctrl_ptr->marking_type)
    {
        selected_num = ListGetSelectedItem(list_ctrl_ptr, idx_ptr, PNULL, array_len);
    }

    return selected_num;
}

/*****************************************************************************/
//  Description : get item ptr
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC const CTRLLIST_ITEM_T *CTRLLIST_GetItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
)
{
    CTRLLIST_ITEM_T *item_ptr = PNULL;
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return PNULL;
    }

    if (index < list_ctrl_ptr->total_item_num)
    {
        CTRLLIST_ITEM_INFO_T *item_info_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

        if (PNULL != item_info_ptr)
        {
            item_ptr = &(item_info_ptr->item);
        }
    }

    return item_ptr;
}

/*****************************************************************************/
//  Description : Get current item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetCurItemIndex(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    return LIST_GetCurItemIndexByPtr(list_ctrl_ptr);
}


/*****************************************************************************/
//  Description : Set current item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetCurItemIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16 current_index   //current index
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    result = ListResetCurItemIndex(list_ctrl_ptr, current_index);

    if (result)
    {
        uint16  pre_cur_item_index = 0;

        ListStopTextAnim(list_ctrl_ptr);

        pre_cur_item_index = list_ctrl_ptr->cur_item_idx;
        
        list_ctrl_ptr->cur_item_idx = current_index;
        
        LIST_SetListStateByPtr( list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE );
        
        LIST_DestroyAllItemData( list_ctrl_ptr, TRUE );
               
        LIST_ResetOffsetYByTopIndex(list_ctrl_ptr, list_ctrl_ptr->top_item_idx);

		LIST_ValidCurItemPosByPtr(list_ctrl_ptr);

        list_ctrl_ptr->offset_y += ListGetRecoverSpace(list_ctrl_ptr);
        LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : Change the list display type
//  Global resource dependence :
//  Author:Create.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_ChangeDisplayType(
    MMI_CTRL_ID_T      ctrl_id,
    GUILIST_TYPE_E type,
    BOOLEAN                is_flash       // 是否立即显示
)
{
    BOOLEAN             result        = TRUE;
    CTRLLIST_OBJ_T *list_ctrl_ptr   = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (GUILIST_RADIOLIST_E == type)
    {
        list_ctrl_ptr->marking_type = CTRLLIST_MARKING_FIXED;
        list_ctrl_ptr->mark_type = CTRLLIST_MARK_RADIO;
    }
    else if (GUILIST_CHECKLIST_E == type)
    {
        list_ctrl_ptr->marking_type = CTRLLIST_MARKING_FIXED;
        list_ctrl_ptr->mark_type = CTRLLIST_MARK_CHECK;
    }
    else
    {
        list_ctrl_ptr->marking_type = CTRLLIST_MARKING_NONE;
        list_ctrl_ptr->mark_type = CTRLLIST_MARK_CHECK;
    }

    if (is_flash)
    {
        // validate list
        ListValidateCtrl(list_ctrl_ptr , FALSE, FALSE);
        ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_TITLE | INNER_STATE_UPDATE_PRG | INNER_STATE_UPDATE_SK);
        ListUpdateHighlightItem(list_ctrl_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : select item
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListSetSelectedItemByPtr(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    uint16           item_pos,   //from 0 to total_item_num-1
    BOOLEAN          is_selected //TRUE : selected; FALSE: unselected
)
{
    BOOLEAN         result      = FALSE;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if (item_pos < list_ctrl_ptr->total_item_num)
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_pos);

        if (PNULL != item_ptr && LIST_IsItemGetFocus(list_ctrl_ptr, item_pos)
            && !LIST_GetItemState(item_ptr, GUIITEM_STATE_DISABLE_MARK)
            && !LIST_GetItemState(item_ptr, GUIITEM_STATE_GRAY))
        {
            if (is_selected)
            {
                if (!LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
                {
                    LIST_SetItemState(item_ptr, GUIITEM_STATE_SELECTED, TRUE);

                    ListUpdateSeletedState(list_ctrl_ptr, item_ptr);
                }

                result = TRUE;
            }
            else
            {
                if (LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
                {
                    LIST_SetItemState(item_ptr, GUIITEM_STATE_SELECTED, FALSE);

                    ListReduceSelectedItemNum(list_ctrl_ptr, item_ptr);
                }

                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Update list
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetSelectedItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16        item_pos,   //from 0 to total_item_num-1
    BOOLEAN       is_selected //TRUE : selected; FALSE: unselected
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return ListSetSelectedItemByPtr(list_ctrl_ptr, item_pos, is_selected);
}

/*****************************************************************************/
//  Description : Set top item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTopItemIndex( //current item index
    MMI_CTRL_ID_T ctrl_id,
    uint16        top_index   //current index
)
{
    BOOLEAN        result    = FALSE;
    uint16         pre_top_index = 0;
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    pre_top_index = list_ctrl_ptr->top_item_idx;

    if (top_index < list_ctrl_ptr->total_item_num)
    {
        list_ctrl_ptr->top_item_idx = top_index;

        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);


        result = TRUE;
    }
    else
    {
        list_ctrl_ptr->top_item_idx = 0;
    }

    LIST_ResetOffsetYByTopIndex(list_ctrl_ptr, list_ctrl_ptr->top_item_idx);
    LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);

    return result;
}

/*****************************************************************************/
//  Description : Get top item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetTopItemIndex( //current item index
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    // validate list
    // ListValidateCtrl( list_ctrl_ptr , FALSE);

    return list_ctrl_ptr->top_item_idx;
}

/*****************************************************************************/
//  Description : Is current item invalid
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_IsCurrItemInvalid(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if ((PNULL == list_ctrl_ptr) ||
        (0 == list_ctrl_ptr->total_item_num))
    {
        return FALSE;
    }

    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    if (PNULL != item_ptr)
    {
        return LIST_GetItemState(item_ptr, GUIITEM_STATE_INVALID);
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Is Need display Info in win title
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetTitleIndexType(
    MMI_CTRL_ID_T ctrl_id,
    GUILIST_TITLE_INDEX_E title_index_type
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_ctrl_ptr->title_index_type = title_index_type;
}

/*****************************************************************************/
//  Description : Is Need display Progress bar
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetNeedPrgbar(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_need_prgbar
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_PRGBAR, is_need_prgbar);
}

#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
/***************************************************************************//*!
@brief set progressbar theme
@author fulu.song
@param ctrl_id	[in]	控件ID
@param theme 	[in]	theme to apply
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetPrgbarTheme(
    MMI_CTRL_ID_T  ctrl_id,
    MMITHEME_PRGBOX_TYPE_E theme
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }
    list_ctrl_ptr->scroll_bar_theme = theme;
}

#endif
/*****************************************************************************/
//  Description : 设置是否循环滚动
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认时循环滚动，当控件不循环滚动，焦点在最上/最下时，会发送消息给父窗口
/*****************************************************************************/
PUBLIC void CTRLLIST_SetCycScroll(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_cyc_scroll
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_CYC_SCROLL_ITEM, is_cyc_scroll);
}

/*****************************************************************************/
//  Description : 设置是否由用户built item layer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认FALSE,控件自动built
/*****************************************************************************/
PUBLIC void CTRLLIST_SetCusBltItemLayer(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN        is_cus_blt_layer
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        list_ctrl_ptr->is_cus_blt_layer = is_cus_blt_layer;
    }
}

/*****************************************************************************/
//  Description : 设置是否响应左右按键
//  Global resource dependence :
//  Author:James.Zhang
//  Note: 默认不响应左右按键
/*****************************************************************************/
PUBLIC void CTRLLIST_SetHandleLeftRight(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN        is_handle
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_HANDLE_LEFT_RIGHT, is_handle);
}

/*****************************************************************************/
//  Description : Is Need display Progress bar block
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 这个接口设置是否需要滚动条上的滑块
/*****************************************************************************/
PUBLIC void CTRLLIST_SetNeedPrgbarBlock(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_true
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_PRGBAR_BLOCK, is_true);
}

/*****************************************************************************/
//  Description : 设置是否需要显示高亮条，默认情况下是显示高两条的
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetNeedHiLightBar(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_true
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (is_true != LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
    {
        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR, is_true);
        list_ctrl_ptr->is_full_dirty = TRUE;
        list_ctrl_ptr->is_pos_dirty = TRUE;
    }
}

/*****************************************************************************/
//  Description : 设置显示动画的时候是否同步解码，默认是异步解码
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 返回设置之前的状态
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetSyncDecodeAnim(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_sync_decode
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    result = LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SYNC_DECODE_ANIM);

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SYNC_DECODE_ANIM, is_sync_decode);

    return result;
}

/*****************************************************************************/
//  Description : list上的Item动画是否需要缩放，默认情况下是不缩放的
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetZoomAnim(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_zoom
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ZOOM_ANIM, is_zoom);

    return TRUE;
}

/*****************************************************************************/
//  Description : Get total item number
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetTotalItemNum(                         //RETURN:
    MMI_CTRL_ID_T ctrl_id     //IN:
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return 0;
    }

    return LIST_GetTotalItemNumByPtr(list_ctrl_ptr);
}

/*****************************************************************************/
//  Description : 获得总的页数
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 每页显示的条目根据list区域及item高度计算所得
/*****************************************************************************/
LOCAL uint16 ListGetTotalPageNum(
    CTRLLIST_OBJ_T *list_ctrl_ptr,     //IN:
    uint16 total_item_num
)
{
    uint16 page_num = 0;
    uint16 page_item_num = 0;

    if (PNULL != list_ctrl_ptr && total_item_num <= list_ctrl_ptr->total_item_num)
    {
        if (1 == total_item_num) // 只有一条的时候，无论区域多高，都只有一页
        {
            page_num = 1;
        }
        else if (1 < total_item_num) // 大于一条的时候
        {
            page_item_num = LIST_GetPageItemNum(list_ctrl_ptr);

            page_item_num = MAX(page_item_num, 1); //每页至少一个ITEM

            page_num = total_item_num / page_item_num; // 计算页数

            if (0 < (total_item_num % page_item_num)) // 取余之后不为0，页数+1
            {
                page_num++;
            }
        }
        else
        {
            page_num = 0;
        }
    }

    return page_num;
}

/*****************************************************************************/
//  Description : 根据首字母查找listbox对应的位置
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL int32 ListGetTopIndexByFirstLetter(CTRLLIST_OBJ_T *list_ctrl_ptr, wchar wletter)
{
    int32   top_index = 0;
    int32   i = 0;

    if (list_ctrl_ptr != PNULL)
    {
        if ('@' >= wletter)
        {
            //符号，排在最前面
            top_index = 0;
        }
        else
        {
            for (i = 0; i < list_ctrl_ptr->total_item_num; i++)
            {
                top_index = i;

                if (wletter <= list_ctrl_ptr->item_info_ptr[i].item.first_letter)
                {
                    break;
                }
            }
        }
    }

    return top_index;
}
/*****************************************************************************/
//  Description : 获得总的页数
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 每页显示的条目根据list区域及item高度计算所得
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetTotalPageNum(                         //RETURN:
    MMI_CTRL_ID_T ctrl_id     //IN:
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    return ListGetTotalPageNum(list_ctrl_ptr, LIST_GetTotalItemNumByPtr(list_ctrl_ptr));
}

/*****************************************************************************/
//  Description : 获得当前页索引号
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 以当前页的最上面一个Item为准计算
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetCurrentPageIndex(                         //RETURN:
    MMI_CTRL_ID_T ctrl_id     //IN:
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    return ListGetTotalPageNum(list_ctrl_ptr, list_ctrl_ptr->top_item_idx + 1);
}

/*****************************************************************************/
//  Description : 设置是否支持slide状态
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetSlideState(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_slide_used
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return LIST_SetSlideStateByPtr(list_ctrl_ptr, is_slide_used);
}

/*****************************************************************************/
//  Description : 设置是否支持slide状态
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SetSlideStateByPtr(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    BOOLEAN is_slide_used
)
{
    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    list_ctrl_ptr->display_style.is_slide_used = is_slide_used;

    return TRUE;
}

/***************************************************************************//*!
@brief 获取是否支持slide状态
@author Xiaoming.Ren
@param ctrl_id [in] 控件HANDLE
@retval true/false
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetSlideState(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return LIST_GetSlideStateByPtr(list_ctrl_ptr);
}

/***************************************************************************//*!
@brief 获取是否支持slide状态
@author Xiaoming.Ren
@param ctrl_id [in] 控件HANDLE
@retval true/false
*******************************************************************************/
PUBLIC BOOLEAN LIST_GetSlideStateByPtr(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return list_ctrl_ptr->display_style.is_slide_used;
}

/*****************************************************************************/
//  Description : handle the content check
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetItemContentCheck(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_ptr,
    GUI_RECT_T       item_rect,
    GUI_RECT_T       *content_rect_ptr
)
{
    BOOLEAN result = FALSE;
    THEMELIST_ITEM_STYLE_T *item_style_ptr = PNULL;

    // 判断是否需要执行content
    if (PNULL != list_ctrl_ptr
        && PNULL != item_ptr
        && PNULL != content_rect_ptr
        && (LIST_GetItemState(item_ptr, GUIITEM_STATE_CONTENT_CHECK)))
    {
        item_style_ptr = item_ptr->item.style_ptr;

        if (list_ctrl_ptr->is_tp_content_valid && list_ctrl_ptr->tp_select_item_content_idx < GUIITEM_CONTENT_MAX_NUM)
        {
            if ((item_style_ptr->content[list_ctrl_ptr->tp_select_item_content_idx].state)&GUIITEM_CONTENT_STATE_CHECK)
            {
                *content_rect_ptr = item_ptr->content_rect[list_ctrl_ptr->tp_select_item_content_idx];
                SetRectOffset(content_rect_ptr, item_rect.left, item_rect.top);
                GUI_IntersectRect(content_rect_ptr, *content_rect_ptr, item_rect);

                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 显示高亮条上的item，在OSD层显示
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListDrawHighlightItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_RECT_T item_rect  = {0};
    GUI_RECT_T inter_rect = {0};
    GUI_RECT_T content_rect  = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;//

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (list_ctrl_ptr->total_item_num == 0 || !LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR)
        || !LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ACTIVE))
    {
        return;
    }

    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    if (PNULL == item_ptr)
    {
        return;
    }

    /* need item data */
    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SET_ITEM_DATA) && PNULL == item_ptr->item.data_ptr)
    {
        LIST_SendNeedItemDataMsg(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
    }

    LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    if (GUI_IntersectRect(&inter_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect, item_rect))
    {
        lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

        // 画高亮背景
        //画每个group的高亮背景
        if (item_ptr->item.item_group)
        {
            if (!(item_ptr->item.item_group & GUIITEM_GROUP_NO_BG))
            {
                GUI_RECT_T img_rect = inter_rect;

                IMG_EnableTransparentColor(TRUE);

                img_rect.left -= item_rect.left;
                img_rect.right -= item_rect.left;
                img_rect.top -= item_rect.top;
                img_rect.bottom -= item_rect.top;

                GUIRES_DisplaySabmImg(&item_rect,
                                      &inter_rect,
                                      &img_rect,
                                      list_ctrl_ptr->win_id,
                                      LIST_GetItemBgImageID(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, TRUE),
                                      &lcd_dev_info);

                IMG_EnableTransparentColor(FALSE);
            }
        }
        else
        {
            if (ListGetItemContentCheck(list_ctrl_ptr, item_ptr, item_rect, &content_rect))
            {
                //listbox默认高亮
                ListDrawBackgroundByRect(list_ctrl_ptr, &item_rect);
                ListDrawHighlightBackground(list_ctrl_ptr, &content_rect, &lcd_dev_info);
            }
            else
            {
                //listbox默认高亮
                ListDrawHighlightBackground(list_ctrl_ptr, &item_rect, &lcd_dev_info);
            }
        }

        // 在高亮背景上重画item
        ListDrawItem(list_ctrl_ptr, &lcd_dev_info, list_ctrl_ptr->cur_item_idx);

        // 画tips
        ListDrawTipsInfo(list_ctrl_ptr);
    }
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle the content check
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListItemContentCheck(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    uint16           item_index,
    CTRLLIST_ITEM_INFO_T *item_ptr,
    GUI_POINT_T      tp_point
)
{
    int32 i = 0;
    BOOLEAN result = FALSE;
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T layout_rect = {0};
    THEMELIST_ITEM_STYLE_T *item_style_ptr = PNULL;

    if (PNULL == list_ctrl_ptr || PNULL == item_ptr)
    {
        return FALSE;
    }

    if (!LIST_GetItemState(item_ptr, GUIITEM_STATE_CONTENT_CHECK))
    {
        return FALSE;
    }

    // 判断是否需要执行content

    item_style_ptr = item_ptr->item.style_ptr;
    LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, item_index);

    for (; i < GUIITEM_CONTENT_MAX_NUM; i++)
    {
        layout_rect = item_ptr->content_rect[i];
        SetRectOffset(&layout_rect, item_rect.left, item_rect.top);

        if (!(item_style_ptr->content[i].state & GUIITEM_CONTENT_STATE_CHECK))
        {
            continue;
        }

        switch (msg_id)
        {
            case MSG_TP_PRESS_DOWN:
                if (GUI_PointIsInRect(tp_point, layout_rect))
                {
                    list_ctrl_ptr->is_tp_content_valid = TRUE;
                    list_ctrl_ptr->tp_select_item_content_idx = i;
                    result = TRUE;
                    // 这里需要画图
                }

                break;

            case MSG_TP_PRESS_MOVE:
                if (!GUI_PointIsInRect(tp_point, layout_rect))
                {
                    list_ctrl_ptr->is_tp_content_valid = FALSE;
                    // 这里需要画图
                }
                else
                {
                    result = TRUE;
                }

                break;

            case MSG_TP_PRESS_UP:
                if (GUI_PointIsInRect(tp_point, layout_rect)
                    && item_index == list_ctrl_ptr->tp_select_item_idx
                    && i == list_ctrl_ptr->tp_select_item_content_idx
                    && list_ctrl_ptr->is_tp_content_valid
                    && !LIST_GetItemState(item_ptr, GUIITEM_STATE_GRAY | GUIITEM_STATE_INVALID))
                {
                    GUILIST_NEED_ITEM_CONTENT_T need_item_content = {0};

                    need_item_content.ctrl_handle   = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
                    need_item_content.ctrl_id       = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
                    need_item_content.item_index    = list_ctrl_ptr->cur_item_idx;
                    need_item_content.item_content_index = i;

                    GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_CTL_LIST_CHECK_CONTENT, &need_item_content, sizeof(GUILIST_NEED_ITEM_CONTENT_T));
                    ListRedrawItem(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

                    result = TRUE;
                }

                break;

            default:
                break;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : ListItemResetContentCheck
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListItemResetContentCheck(
    CTRLLIST_OBJ_T   *list_ctrl_ptr
)
{
    list_ctrl_ptr->is_tp_content_valid = FALSE;
}

/*****************************************************************************/
//  Description : handle the tp key down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListHandleTPKeydown(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      point
)
{
    int32        j            = 0;
    BOOLEAN      is_inrect    = FALSE;
    GUI_RECT_T   item_rect    = {0};
    MMI_RESULT_E result       = MMI_RESULT_FALSE;

    if (NULL == list_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    // 判断是否在滚动条区域
    if (GUI_PointIsInRect(point, ListGetScrollBarRect(list_ctrl_ptr))
        && LIST_IsNeedScrollBar(list_ctrl_ptr))
    {
        if (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state
#ifdef GUIF_PRGBOX_LETTERSCROLL
            && GUIPRGBOX_STYLE_LETTERS_SCROLL != list_ctrl_ptr->scroll_bar_type
#endif
           )
        {
            result = ListHandleScrollbarTPKeydown(list_ctrl_ptr);
        }
    }
    else // 当不在滚动条区域时，执行list操作
    {
        for (j = list_ctrl_ptr->top_item_idx; j <= list_ctrl_ptr->end_item_idx; j++)
        {
            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, j);

            if (GUI_PointIsInRect(point, item_rect))
            {
                CTRLLIST_ITEM_INFO_T *item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, j);

                list_ctrl_ptr->tp_select_item_idx = j;
                is_inrect = TRUE;
                list_ctrl_ptr->is_click_item = TRUE;

                ListItemContentCheck(list_ctrl_ptr, msg_id, j, item_ptr, point);

                break;
            }
        }

        // 处理移动问题
        if (is_inrect && list_ctrl_ptr->tp_select_item_idx < list_ctrl_ptr->total_item_num)
        {
            if (LIST_IsItemGetFocus(list_ctrl_ptr, list_ctrl_ptr->tp_select_item_idx)) // 能够获得焦点
            {
                result = MMI_RESULT_TRUE;

#ifdef TOUCH_PANEL_SUPPORT

                if (list_ctrl_ptr->tp_select_item_idx > list_ctrl_ptr->cur_item_idx)
                {
                    ListHandleTpNext(list_ctrl_ptr);   // 向下移动
                }
                else if (list_ctrl_ptr->tp_select_item_idx < list_ctrl_ptr->cur_item_idx)
                {
                    ListHandleTpPre(list_ctrl_ptr);   // 向上移动
                }
                else
#endif
                {
                    result = MMI_RESULT_FALSE;
                    // 更新高亮
                    //ListRedrawItem(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
                    ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_TITLE | INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG | INNER_STATE_UPDATE_SK);
                    ListUpdateHighlightItem(list_ctrl_ptr);
                }
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : handle the tp key down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTPKeyUp(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T       point
)
{
    uint16       j            = 0;
    BOOLEAN      is_inrect    = FALSE;
    BOOLEAN      result       = FALSE;
    GUI_RECT_T   item_rect    = {0};

    if (NULL == list_ctrl_ptr)
    {
        return result;
    }

    if (!GUI_PointIsInRect(point, ListGetScrollBarRect(list_ctrl_ptr))) // 当不在滚动条区域时，执行list操作
    {
        for (j = list_ctrl_ptr->top_item_idx; j <= list_ctrl_ptr->end_item_idx; j++)
        {
            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, j);

            if (GUI_PointIsInRect(point, item_rect))
            {
                CTRLLIST_ITEM_INFO_T *item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, j);

                // 当check处理之后，is_inrect设置为FALSE, 行数将返回false，外面不需要再做处理
                if (!ListItemContentCheck(list_ctrl_ptr, msg_id, j, item_ptr, point))
                {
                    is_inrect = TRUE;
                }

                list_ctrl_ptr->tp_select_item_idx = j;

                break;
            }
        }

        // 设置返回值，以便外部判断如何进行下一步操作。
        if (list_ctrl_ptr->tp_select_item_idx < list_ctrl_ptr->total_item_num
            && is_inrect
            && list_ctrl_ptr->tp_select_item_idx == list_ctrl_ptr->cur_item_idx)
        {
            result = TRUE;
        }
    }

    ListItemResetContentCheck(list_ctrl_ptr);

    return result;
}

/*****************************************************************************/
//  Description : 处理list滚动条
//  Global resource dependence :
//  Author: Jibin
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListHandleScrollbarTPKeydown(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;

    ListInitScrollBar(list_ctrl_ptr);

    return (result);
}
#endif

/*****************************************************************************/
//  Description : 判断item是否可以焦点显示
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemGetFocus(
                             CTRLLIST_OBJ_T  *list_ctrl_ptr,
                             uint16 item_index
                             )
{  
    if (item_index < list_ctrl_ptr->total_item_num)
    {
        return (BOOLEAN)(!(GUIITEM_STATE_SPLIT & list_ctrl_ptr->item_info_ptr[item_index].item.state)); // 目前之后split状态不能显示焦点
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Set current item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListResetCurItemIndex(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    uint16          cur_item_index
)
{
    BOOLEAN result = FALSE;

    if (PNULL != list_ctrl_ptr
        && cur_item_index != list_ctrl_ptr->cur_item_idx
        && cur_item_index < list_ctrl_ptr->total_item_num
        && IsItemGetFocus(list_ctrl_ptr, cur_item_index))
    {
        result = TRUE;

        list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
        list_ctrl_ptr->cur_item_idx = cur_item_index;

        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);

        LIST_ResetItemHeight(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx, list_ctrl_ptr->cur_item_idx);

        list_ctrl_ptr->is_pos_dirty = TRUE;
        LIST_ResetItemPostion(list_ctrl_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : set selected all
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:if can't select all,it will select the max allow select num
/*****************************************************************************/
PUBLIC void CTRLLIST_SetAllSelected(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_select_all //TRUE : selected; FALSE: unselected
)
{
    uint16          item_pos    = 0;
    CTRLLIST_OBJ_T *list_ctrl_ptr    = ListGetPtr(ctrl_id);
    BOOLEAN         sel_status  = FALSE;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    //不需要操作
    if ((is_select_all && list_ctrl_ptr->selected_item_num == list_ctrl_ptr->total_item_num)
        || (!is_select_all && 0 == list_ctrl_ptr->selected_item_num))
    {
        return;
    }

    for (item_pos = 0; item_pos < list_ctrl_ptr->total_item_num; item_pos ++)
    {
        CTRLLIST_SetSelectedItem(ctrl_id, item_pos, is_select_all);
    }

    //给应用发消息
    if (0 != list_ctrl_ptr->total_item_num)
    {
        if (list_ctrl_ptr->selected_item_num == list_ctrl_ptr->total_item_num)
        {
            sel_status = TRUE;

            ListSendMarkAllItemMsg(list_ctrl_ptr, sel_status);
        }
        else if (0 == list_ctrl_ptr->selected_item_num)
        {
            ListSendMarkAllItemMsg(list_ctrl_ptr, sel_status);
        }
    }
}

/*****************************************************************************/
//  Description : set text list can markable or not
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 返回设置成功或失败
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTextListMarkable(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_enable_mark
)
{
    BOOLEAN pre_state = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    pre_state = LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENABLE_MARK);

    if (pre_state == is_enable_mark)
    {
        return TRUE;
    }

    list_ctrl_ptr->marking_type = is_enable_mark ? CTRLLIST_MARKING_ENABLED : CTRLLIST_MARKING_NONE;

    if (!is_enable_mark)
    {
        list_ctrl_ptr->mark_cnt_max = 0;
        CTRLLIST_SetAllSelected(ctrl_id, FALSE);
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENABLE_MARK, is_enable_mark);

    // 状态改变时，需要重新计算
    list_ctrl_ptr->is_full_dirty = TRUE;
    list_ctrl_ptr->is_pos_dirty = TRUE;

    return TRUE;
}

/*****************************************************************************/
//  Description : set number list start index
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetNumberListStartIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16        num_start_index
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_ctrl_ptr->num_start_index = num_start_index;
}

/*****************************************************************************/
//  Description : set radio and check list has own softkey
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetOwnSofterKey(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_own_softkey
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_OWN_SOFTKEY, is_own_softkey);
}

/*****************************************************************************/
//  Description : 设置控件是否需要分割线
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetNeedSplidLine(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_true
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SPLIT_LINE, is_true);
}

/*****************************************************************************/
//  Description : set check list default softkey id
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListDefSoftkeyID(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T       text_cancel_id,
    MMI_TEXT_ID_T       text_select_id,
    MMI_TEXT_ID_T       text_return_id
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_ctrl_ptr->display_style.text_cancel_id = text_cancel_id;
    list_ctrl_ptr->display_style.text_select_id = text_select_id;
    list_ctrl_ptr->display_style.text_return_id = text_return_id;

}

/*****************************************************************************/
//  Description : set check list default image
//  Global resource dependence :
//  Author:miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListImage(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T       image_unselected,
    MMI_IMAGE_ID_T       image_selected
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_ctrl_ptr->display_style.check_unselected_image_id = image_unselected;
    list_ctrl_ptr->display_style.check_selected_image_id = image_selected;

}
/*****************************************************************************/
//  Description : set check list image display left or right
//  Global resource dependence :
//  Author:miao.xi
//  Note:    BOOLEAN  is_left: (1:left 0:right)
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListImagePosition(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_left// 1:left 0:right
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_ctrl_ptr->is_mark_icon_left = is_left;
}

/*****************************************************************************/
//  Description : set check list default softkey id
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListSoftkeyIndex(
    MMI_CTRL_ID_T ctrl_id,
    GUISOFTKEY_INDEX  select_cancel_index
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_ctrl_ptr->display_style.select_cancel_index = select_cancel_index;
}

/*****************************************************************************/
//  Description : set radio list default softkey id
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeRadioListDefSoftkeyID(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T       text_ok_id,
    MMI_TEXT_ID_T       text_return_id
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_ctrl_ptr->display_style.text_ok_id = text_ok_id;
    list_ctrl_ptr->display_style.text_return_id = text_return_id;
}


/*****************************************************************************/
//  Description : set item invalid
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemInvalid(
    MMI_CTRL_ID_T ctrl_id,    // control id
    uint16        pos,        // 指定的位置, from 0 to total_item_num-1
    BOOLEAN       is_invalid
)
{
    BOOLEAN         result         = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr       = ListGetPtr(ctrl_id);
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (pos < list_ctrl_ptr->total_item_num)
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, pos);

        if (PNULL != item_ptr)
        {
            LIST_SetItemState(item_ptr, GUIITEM_STATE_INVALID, is_invalid);
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : interface for mmk platform
//  Global resource dependence :
//  Author:peng.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetItemDataByPtr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 index,
    uint32 *user_data_ptr
)
{
    BOOLEAN         result   = FALSE;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr || PNULL == user_data_ptr)
    {
        return FALSE;
    }

    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

    if (PNULL != item_ptr)
    {
        *user_data_ptr = item_ptr->item.user_data;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : interface for mmk platform
//  Global resource dependence :
//  Author:peng.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetItemData(
    MMI_CTRL_ID_T ctrl_id,
    uint16 index,
    uint32 *user_data_ptr
)
{
    return LIST_GetItemDataByPtr(ListGetPtr(ctrl_id), index, user_data_ptr);
}


/***************************************************************************//*!
@brief  设置控件项的用户自定义数据
@author peng.chen
@param iguictrl_ptr [in]    控件指针
@param index     [in]   要获得的Item索引
@param user_data_ptr    [in]    用户数据，可以为任何类型，用户自定义
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemUserData(
    MMI_CTRL_ID_T ctrl_id,
    uint16 index,
    uint32 *user_data_ptr
)
{
    BOOLEAN is_ret = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return is_ret;
    }

    if (index < list_ctrl_ptr->total_item_num)
    {
        CTRLLIST_ITEM_INFO_T *item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, index);

        if (PNULL != item_ptr)
        {
            if (PNULL != user_data_ptr)
            {
                item_ptr->item.user_data = *user_data_ptr;
            }
            else
            {
                item_ptr->item.user_data = 0;
            }

            is_ret = TRUE;
        }
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : 设置item标记状态
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetItemMarkByStatus(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_ptr,
    uint16         item_idx,
    BOOLEAN        is_selected
)
{
    if ((PNULL == list_ctrl_ptr) || (PNULL == item_ptr))
    {
        return;
    }

    if (CTRLLIST_MARKING_NONE == list_ctrl_ptr->marking_type)
    {
        return;
    }

    if (LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED) == is_selected)
    {
        return;
    }

    LIST_SetItemState(item_ptr, GUIITEM_STATE_SELECTED, (BOOLEAN)is_selected);

    if (is_selected)
    {
        ListUpdateSeletedState(list_ctrl_ptr, item_ptr);
    }
    else
    {
        ListReduceSelectedItemNum(list_ctrl_ptr, item_ptr);
    }

    LIST_PostMarkItemMsg(list_ctrl_ptr, item_idx, is_selected);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle list scroll for touch panel
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpScroll(
    int32 top_item_index,
    CTRLLIST_OBJ_T     *list_ctrl_ptr
)
{
    BOOLEAN     result = FALSE;
    uint16      pre_top_item_idx = 0;
    uint16      pre_end_item_idx = 0;
    int32       offset_y = 0;
    int32       org_offset_y = 0;

    if (PNULL == list_ctrl_ptr)
    {
        return result;
    }

    if (list_ctrl_ptr->total_item_num > 0
        && list_ctrl_ptr->total_item_num > LIST_GetPageItemNum(list_ctrl_ptr))
    {
        GUI_RECT_T rect = ListGetScrollBarRect(list_ctrl_ptr);

        org_offset_y = list_ctrl_ptr->offset_y;
        list_ctrl_ptr->offset_y = rect.top - top_item_index;

        offset_y = ListGetRecoverSpace(list_ctrl_ptr);

        if (0 != offset_y)
        {
            list_ctrl_ptr->offset_y += offset_y;
            LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
        }
        else
        {
            LIST_ResetTopItemAndEndItemIndexExt(list_ctrl_ptr, list_ctrl_ptr->offset_y - org_offset_y);
        }

        LIST_DestroyAllItemDataNotInPageExt(list_ctrl_ptr, pre_top_item_idx, pre_end_item_idx);

        ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);

        ListUpdateHighlightItem(list_ctrl_ptr);
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : Adjust cur item pos
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ValidCurItemPosByPtr(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    if (PNULL != list_ctrl_ptr)
    {
        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);

        ListValidateCtrl(list_ctrl_ptr, TRUE, FALSE);
    }
}

/*****************************************************************************/
//  Description : Adjust cur item pos
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_ValidCurItemPos(
    MMI_CTRL_ID_T   ctrl_id
)
{

    LIST_ValidCurItemPosByPtr(ListGetPtr(ctrl_id));
}

/*****************************************************************************/
//  Description : set the list bg color
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetBgColor(
    MMI_CTRL_ID_T ctrl_id ,
    GUI_COLOR_T   back_color
)
{
    return LIST_SetBgColorByPtr(ListGetPtr(ctrl_id), back_color);
}

/*****************************************************************************/
//  Description : set the list bg color
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SetBgColorByPtr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_COLOR_T    back_color
)
{
    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SELFSET_BG, TRUE);

    list_ctrl_ptr->display_style.bg_info.color = back_color;
    list_ctrl_ptr->display_style.bg_info.img_id = 0;
    list_ctrl_ptr->display_style.bg_info.bg_type = GUI_BG_COLOR;
    list_ctrl_ptr->is_transparent = FALSE;

    return TRUE;
}

/*****************************************************************************/
//  Description : 设置是否支持分隔符浮动效果，默认不支持
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetSplitOnside(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN    is_true
)
{
    LIST_SetListStateByPtr(ListGetPtr(ctrl_id), GUILIST_STATE_SPLIT_ONSIDE, is_true);
}

/*****************************************************************************/
//  Description : set the list bg image
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetBgImage(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T image_id,
    BOOLEAN        is_transparent
)
{
    return LIST_SetDefaultIconByPtr(ListGetPtr(ctrl_id), image_id , is_transparent);
}

/*****************************************************************************/
//  Description : set the list bg image
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetPrgBarImageId(
    MMI_CTRL_ID_T   ctrl_id,
    MMI_IMAGE_ID_T image_id
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SET_PRGBAR_IMG, TRUE);

    list_ctrl_ptr->display_style.prgbar_image_id = image_id;

    return TRUE;
}

/*****************************************************************************/
//  Description : set the user bg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetUserBg(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN        is_user_bg
)
{

    LIST_SetListStateByPtr(ListGetPtr(ctrl_id), GUILIST_STATE_SELFSET_BG, is_user_bg);
}

/*****************************************************************************/
//  Description : set the list bg image
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SetBgImageByPtr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    MMI_IMAGE_ID_T image_id,
    BOOLEAN       is_transparent
)
{
    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SELFSET_BG, TRUE);

    list_ctrl_ptr->is_transparent = is_transparent;
    list_ctrl_ptr->display_style.bg_info.img_id = image_id;
    list_ctrl_ptr->display_style.bg_info.bg_type = GUI_BG_IMG;

    return TRUE;
}

/*****************************************************************************/
//  Description : set list is permit display border
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_PermitBorder(
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_permit
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = PNULL;

    //get list pointer by control id
    list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    list_ctrl_ptr->is_permit_border = is_permit;

    return TRUE;
}

/*****************************************************************************/
//  Description : 设置list text font
//  Global resource dependence :
//  Author:James.Zhang
//  Note: 默认不需要设置.与GUILIST_STATE_TEXT_STYLE状态同时使用
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTextFont(
    MMI_CTRL_ID_T   ctrl_id,
    GUI_FONT_T      font,
    GUI_COLOR_T     font_color
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_TEXT_STYLE, TRUE);

    list_ctrl_ptr->text_style.font = font;
    list_ctrl_ptr->text_style.font_color = font_color;

    return TRUE;
}
/*****************************************************************************/
//  Description : 设置list text font color
//  Global resource dependence :
//  Author:dandan.cao
//  Note: 默认不需要设置.与GUIITEM_STATE_COSTMER_FONT_COLOR状态同时使用
/*****************************************************************************/
PUBLIC BOOLEAN CTRLIST_SetOneItemFontColor(
    MMI_CTRL_ID_T   ctrl_id,
    uint32                 item_index,
    GUI_COLOR_T     item_font_color,
    BOOLEAN            is_true
)
{
    uint32  i = 0;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);
    if (PNULL != list_ctrl_ptr)
    {
        for( i = 0;i < list_ctrl_ptr->total_item_num;i++)
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, i);
            LIST_SetItemState(item_ptr, GUIITEM_STATE_COSTMER_FONT_COLOR, FALSE);
        }
    }
    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);
    LIST_SetItemState(item_ptr, GUIITEM_STATE_COSTMER_FONT_COLOR, is_true);
    list_ctrl_ptr->display_style.item_font_color = item_font_color;
    return TRUE;
}

/*****************************************************************************/
//  Description : 设置list text font
//  Global resource dependence :
//  Author:James.Zhang
//  Note: 默认不需要设置, 与GUIITEM_STATE_COSTMER_FONT_COLOR状态同时使用
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemTextFontColor(
    MMI_CTRL_ID_T   ctrl_id,
    GUI_COLOR_T     font_color
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    list_ctrl_ptr->display_style.item_font_color = font_color;

    return TRUE;
}

/*****************************************************************************/
//  Description : set list rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetRect(
    MMI_HANDLE_T    ctrl_handle,
    GUI_RECT_T      *rect_ptr
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = PNULL;

    //get list pointer by control id
    list_ctrl_ptr = ListGetPtr(ctrl_handle);

    if (PNULL != list_ctrl_ptr)
    {
        ListSetRect((CTRLBASE_OBJ_T *)list_ctrl_ptr, rect_ptr);
    }
}

/*****************************************************************************/
//  Description : set the list handle star or hash key
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: default handle star and hash key
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetHandleStarHash(
    MMI_HANDLE_T   ctrl_handle,
    BOOLEAN        is_handle
)
{
    BOOLEAN         result = FALSE;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = PNULL;

    //get control pointer
    list_ctrl_ptr = ListGetPtr(ctrl_handle);

    if (PNULL != list_ctrl_ptr)
    {
        //set handle
        list_ctrl_ptr->is_handle_starhash = is_handle;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : init icon display layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListResetOffsetY(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_RECT_T list_rect = {0};

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    list_ctrl_ptr->offset_y = list_rect.top;
}

/*****************************************************************************/
//  Description : get valid rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListGetLayerRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *rect_ptr
)
{
    BOOLEAN result = FALSE;
    GUI_RECT_T valid_rect = {0};
    GUI_RECT_T lcd_rect = {0};

    if (PNULL != list_ctrl_ptr)
    {
        lcd_rect = MMITHEME_GetFullScreenRect();

        if (GUI_IntersectRect(&valid_rect, lcd_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect)
            && GUI_IntersectRect(&valid_rect, valid_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect))
        {
            *rect_ptr = valid_rect;

            if (!GUI_IsRectEmpty(valid_rect))
            {
                result = TRUE;
            }
        }
    }

    return result;
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create icon display layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListCreateItemLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
)
{
    GUI_RECT_T          page_rect = {0};
    UILAYER_CREATE_T    create_info = {0};

    if (list_ctrl_ptr->display_style.is_slide_used // 使用滑动
        && ListGetLayerRect(list_ctrl_ptr, &page_rect)  // 区域判断
        && !GUI_IsRectEmpty(page_rect) // 区域判断
        && UILAYER_NULL_HANDLE == list_ctrl_ptr->item_layer.block_id //  ID判断
        && MMK_IsFocusWin(list_ctrl_ptr->win_id)) // 窗口是否Focus
    {
        // 创建层
        create_info.lcd_id = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info.lcd_id;
        create_info.owner_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        create_info.offset_x = page_rect.left;
        create_info.offset_y = page_rect.top;
        create_info.width = (page_rect.right + 1 - page_rect.left);
        create_info.height = (page_rect.bottom + 1 - page_rect.top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_DOUBLE_COPY;

        if (!UILAYER_CreateLayer(&create_info, &list_ctrl_ptr->item_layer))
        {
            LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_CUSTORMER_LAYER, FALSE);

            // 设置color key
            UILAYER_SetLayerColorKey(&list_ctrl_ptr->item_layer, TRUE, UILAYER_TRANSPARENT_COLOR);

            // 使用color清除层
            UILAYER_Clear(&list_ctrl_ptr->item_layer);

            UILAYER_SetLayerPosition(&list_ctrl_ptr->item_layer, page_rect.left, page_rect.top);

            // 如果是下拉框，则list菜单会把背景和item刷在同一个层上
            if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_DROPDOWNLIST))
            {
                ListSetLcdDevInfo((CTRLBASE_OBJ_T *)list_ctrl_ptr, &list_ctrl_ptr->item_layer);
            }
        }
        else
        {
            list_ctrl_ptr->item_layer.block_id = UILAYER_NULL_HANDLE;
        }
    }
}

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListReleaseItemLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
)
{
    if (UILAYER_IsMultiLayerEnable()
        && (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&(list_ctrl_ptr->item_layer)))
        && !LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_CUSTORMER_LAYER))
    {
        //消除highlight层的影响
        UILAYER_RELEASELAYER(&list_ctrl_ptr->item_layer);   /*lint !e506 !e774*/
    }

    return;
}

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListSetBltItemLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    UILAYER_HANDLE_T   layer1
)
{
    GUI_RECT_T page_rect = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    // 去除背景层
    lcd_dev_info.block_id = MMITHEME_GetBgLayer();
    UILAYER_RemoveBltLayer(&lcd_dev_info);

    // blt layer
    lcd_dev_info.block_id = layer1;

    if (0 != layer1 && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

        UILAYER_SetLayerColorKey(&lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);

        LIST_GetValidRect(list_ctrl_ptr, &page_rect);

        // UILAYER_ClearRect(&lcd_dev_info, page_rect);

        // UILAYER_Clear(&lcd_dev_info);
    }

    return;
}
#endif

/*****************************************************************************/
//  Description : is item visible?
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsListItemVisible(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index
)
{
    BOOLEAN result = FALSE;
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T clip_rect = {0};

    if (0 != list_ctrl_ptr->total_item_num)
    {
        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, item_index);

        if (GUI_IntersectRect(&clip_rect, item_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect))
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : GetSecMenuItemDispLcdDevInfo
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO ListGetDispLayerInfo(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return lcd_dev_info;
    }

#ifdef UI_MULTILAYER_SUPPORT

    if (UILAYER_IsMultiLayerEnable()
        && UILAYER_IsLayerActive(&list_ctrl_ptr->item_layer)
        && list_ctrl_ptr->display_style.is_slide_used)
    {
        lcd_dev_info.block_id = list_ctrl_ptr->item_layer.block_id;
    }
    else
#endif
    {
        lcd_dev_info = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info;
    }

    return lcd_dev_info;
}

/*****************************************************************************/
//  Description : reset list display item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListReviceDrawItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    int32  *offset_y_ptr,
    BOOLEAN is_next
)
{
    int32 offset_y = 0;
    BOOLEAN reslut = TRUE;
    GUI_RECT_T cur_item_rect = {0};
    GUI_RECT_T inter_item_rect = {0};
    GUI_RECT_T valid_rect = {0};

    if ((PNULL == list_ctrl_ptr) || (PNULL == offset_y_ptr))
    {
        return FALSE;
    }

    // 获取区域
    if (!LIST_CalculateListItemRect(&cur_item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx))
    {
        return reslut;
    }

    // 判断是否有交集
    if (LIST_GetValidRect(list_ctrl_ptr, &valid_rect)
        && GUI_IntersectRect(&inter_item_rect, cur_item_rect, valid_rect)) // 有交集
    {
        // 不被完全覆盖
        if (!GUI_RectIsCovered(valid_rect, cur_item_rect))
        {
            if (inter_item_rect.top > cur_item_rect.top && !is_next) // item的上半部被覆盖
            {
                // 被覆盖后，不管down key 还是 up key都只恢复显示cur，而不改变cur index
                offset_y = inter_item_rect.top - cur_item_rect.top; // 需要向下移动的位移
            }
            else if (inter_item_rect.bottom < cur_item_rect.bottom && is_next) // item的下半部分被覆盖
            {
                offset_y = inter_item_rect.bottom - cur_item_rect.bottom; // 需要向上移动的位移
            }
            else
            {
                reslut = FALSE;
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_13845_112_2_18_3_20_10_225, (uint8 *)"");
            }
        }
        else
        {
            offset_y = ListGetRecoverSpace(list_ctrl_ptr);
            reslut = FALSE;
        }
    }
    else // 完全没有交集
    {
        if (cur_item_rect.top < valid_rect.top) // 在菜单区域之上
        {
            offset_y = valid_rect.top - cur_item_rect.top; // 向下移动的位移
        }
        else if (cur_item_rect.bottom > valid_rect.bottom)
        {
            offset_y = valid_rect.bottom - cur_item_rect.bottom;
        }
        else
        {
            reslut = FALSE;
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_13866_112_2_18_3_20_10_226, (uint8 *)"");
        }
    }

    *offset_y_ptr = offset_y;

    return reslut;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle tp msg, to exchange current item index
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListTpMoveItems(
    CTRLLIST_OBJ_T *list_ctrl_ptr, // 控件指针
    uint16         item_index // move to index
)
{
    GUI_RECT_T list_rect = {0};

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_rect = LIST_AdjustRect(list_ctrl_ptr);

    ListStopTextAnim(list_ctrl_ptr);

    list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
    list_ctrl_ptr->pre_top_item_idx = list_ctrl_ptr->top_item_idx;

    if (list_ctrl_ptr->slide_state == MMK_TP_SLIDE_FLING)
    {
        // fling状态不画滚动条
        return;
    }

    list_ctrl_ptr->cur_item_idx = item_index;

    // 重新设置item 高度
    LIST_ResetItemHeight(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx, list_ctrl_ptr->cur_item_idx);

    // draw
    ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
    ListUpdateHighlightItem(list_ctrl_ptr);

    // 之前的current要释放
    LIST_DestroyAllItemDataNotInPageExt(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx, list_ctrl_ptr->pre_cur_item_idx);
}

/*****************************************************************************/
//  Description : handle the tp down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpNext(
    CTRLLIST_OBJ_T  *list_ctrl_ptr
)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_support_3d = FALSE;               // 是否3d

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (0 < list_ctrl_ptr->total_item_num)
    {
        is_support_3d = list_ctrl_ptr->display_style.is_slide_used ? FALSE : TRUE;
#ifdef TOUCH_PANEL_SUPPORT
        ListTpMoveItems(list_ctrl_ptr, list_ctrl_ptr->tp_select_item_idx);
#endif

        if (list_ctrl_ptr->slide_state == MMK_TP_SLIDE_FLING)
        {
            // fling状态不画滚动条
            return result;
        }

        if (list_ctrl_ptr->cur_item_idx == list_ctrl_ptr->total_item_num - 1)
        {
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVEBOTTOM);
        }
        else
        {
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVEDOWN);
        }
    }

    return (result);
}


/*****************************************************************************/
//  Description : handle the tp up message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpPre(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    BOOLEAN result = TRUE;
    BOOLEAN is_support_3d = FALSE;               // 是否3d

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    if (list_ctrl_ptr->total_item_num > 0)
    {
        is_support_3d = list_ctrl_ptr->display_style.is_slide_used ? FALSE : TRUE;
#ifdef TOUCH_PANEL_SUPPORT
        ListTpMoveItems(list_ctrl_ptr, list_ctrl_ptr->tp_select_item_idx);
#endif

        if (list_ctrl_ptr->slide_state == MMK_TP_SLIDE_FLING)
        {
            // fling状态不画滚动条
            return result;
        }

        if (0 == list_ctrl_ptr->cur_item_idx)
        {
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVETOP);
        }
        else
        {
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle,  MSG_NOTIFY_LIST_MOVEUP);
        }

    }

    return result;
}

/*****************************************************************************/
//  Description : handle the tp down message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpDown(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *point_ptr
)
{
    BOOLEAN     result = FALSE;
    GUI_RECT_T  prgbox_rect = {0};
    GUI_RECT_T  item_rect = {0};
    uint16      line_index = 0;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if (PNULL == list_ctrl_ptr)
    {
        return result;
    }

    //防止烂屏抖动
    // MMK_DelayTpMove();

    list_ctrl_ptr->is_tp_down = TRUE;

    // 记录起始点
    list_ctrl_ptr->slide_start_point.x = point_ptr->x;
    list_ctrl_ptr->slide_start_point.y = point_ptr->y;
    list_ctrl_ptr->slide_pre_point.x = point_ptr->x;
    list_ctrl_ptr->slide_pre_point.y = point_ptr->y;

    if (0 == list_ctrl_ptr->total_item_num)
    {
        return result;
    }

    prgbox_rect = ListGetScrollBarRect(list_ctrl_ptr);

    list_ctrl_ptr->list_state |= GUILIST_STATE_ACTIVE;

    // 记录这个时候的滑动状态
    list_ctrl_ptr->tpdown_slide_state = list_ctrl_ptr->slide_state;

    ListStopTpTimer(list_ctrl_ptr);

    ListStopTextAnim(list_ctrl_ptr);

    if (!GUI_PointIsInRect(*point_ptr, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect))     //不在list区域
    {
        // 当用在dropdownlist中时，如果鼠标点在listbox区域外，相当于
        // listbox失去焦点，并需要隐藏，所以这里发消息通知dropdownlist
        if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_DROPDOWNLIST))
        {
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
            ListStopTpMoveTimer(list_ctrl_ptr);
            result = GUICTRL_SendNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LOSE_ACTIVE);
        }

        // 注意，如果是dropdownlist，list指针会在dropdownlist中释放，此处不能再处理list控件的数据
        return result;
    }
    else if (GUI_PointIsInRect(*point_ptr, prgbox_rect)   //在list滚动条区域
             && LIST_IsNeedScrollBar(list_ctrl_ptr)
             && MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state)
    {
        return result;
    }

    if (MMK_TP_SLIDE_FLING == list_ctrl_ptr->slide_state)   // fling的时候直接返回
    {
        list_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
        return result;
    }

#ifdef GUIF_LIST_PDASTYLE

    //每次tp down时需要设置pre_cur_item_idx值
    if (list_ctrl_ptr->is_pre_send_ok)
    {
        list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
    }

#else
    list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
#endif

    result = ListHandleTPKeydown(list_ctrl_ptr, msg_id, *point_ptr);

    //标记时
    if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENABLE_MARK))
    {
        result = MMI_RESULT_TRUE;
    }

    SCI_MEMSET(&list_ctrl_ptr->move_info, 0, sizeof(CTRLLIST_MOVE_INFO_T));

    if (list_ctrl_ptr->total_item_num > 0)
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
        SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
        list_ctrl_ptr->is_tpdown_select = LIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED);

        if (list_ctrl_ptr->cur_item_idx >= list_ctrl_ptr->top_item_idx)
        {
            line_index = list_ctrl_ptr->cur_item_idx - list_ctrl_ptr->top_item_idx;
        }

        //无效item区域
        if (line_index >= list_ctrl_ptr->total_item_num)
        {
            return result;
        }

        if (!list_ctrl_ptr->display_style.is_slide_used)
        {
            LIST_CalculateListItemRect(&list_ctrl_ptr->move_info.prev_item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
        }

        list_ctrl_ptr->move_info.prev_item_index = list_ctrl_ptr->cur_item_idx;
        list_ctrl_ptr->move_info.list_tp_press_down = TRUE;
    }

    if (list_ctrl_ptr->display_style.is_slide_used)
    {
        ListStartTpMoveTimer(list_ctrl_ptr);
    }

    LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_SLIDE_ITEM))
    {
        if (!LIST_IsItemDispLayerActive())
        {
            LIST_CreateItemDispLayer(list_ctrl_ptr, item_rect);
        }

        LIST_CreateItemMistLayer(list_ctrl_ptr, item_rect);
    }

#ifdef MMI_READMENU_ENABLE
    ListHandleTextToSpeech(list_ctrl_ptr);
#endif

    return result;
}

/*****************************************************************************/
//  Description : handle the tp move message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpMove(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *point_ptr
)
{
    BOOLEAN result = FALSE;

    // 优先处理横向滑动
    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_SLIDE_ITEM)
        && LIST_HandleItemTpMove(list_ctrl_ptr, point_ptr))
    {
        return TRUE;
    }

    // 当move消息来时，停止timer
    if (IS_TP_MOVE_Y(point_ptr->y, list_ctrl_ptr->slide_start_point.y))
    {
        if (MMK_TP_SLIDE_FLING != list_ctrl_ptr->tpdown_slide_state
            || MMK_IsWayChanged(list_ctrl_ptr->fling_velocity, &list_ctrl_ptr->slide_start_point, point_ptr, FALSE))
        {
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
        }

        // 终止当前正在解码的item
        LIST_DestroyItemDecode(list_ctrl_ptr);
    }

    if (list_ctrl_ptr->display_style.is_slide_used)
    {
        ListPostSlideStateMsg(list_ctrl_ptr, GUILIST_START_SLIDE);
        ListItemResetContentCheck(list_ctrl_ptr);
    }
    else
    {
        int32                offset_y           = 0;
        uint16               cur_item_idx       = 0;
        uint16               tmp_item_idx       = 0;
        uint16               skip_line_num      = 0;
        uint16               i                  = 0;
        uint16               line_index         = 0;
        uint16               page_item_num      = (uint16)(list_ctrl_ptr->end_item_idx - list_ctrl_ptr->top_item_idx + 1);
        GUI_RECT_T           prgbox_rect        = {0};
        GUI_RECT_T           item_rect          = {0};
        GUI_RECT_T           inter_rect         = {0};
        CONTINUE_MOVE_TYPE_E continue_move_type = CONTINUE_MOVE_INVALID;
        CTRLLIST_ITEM_INFO_T  *item_ptr     = PNULL;

        prgbox_rect = ListGetScrollBarRect(list_ctrl_ptr);

        list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

        ListStopTpTimer(list_ctrl_ptr);

        //不在list区域
        if (!GUI_PointIsInRect(*point_ptr, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect))
        {
            if (list_ctrl_ptr->total_item_num > page_item_num)
            {
                if (point_ptr->y > ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.bottom)
                {
                    ListStartTpTimer(list_ctrl_ptr, CONTINUE_MOVE_DOWN);
                }
                else if (point_ptr->y < ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.top)
                {
                    ListStartTpTimer(list_ctrl_ptr, CONTINUE_MOVE_UP);
                }
            }

            return result;
        }
        else if (GUI_PointIsInRect(*point_ptr, prgbox_rect) //在list滚动条区域
                 && LIST_IsNeedScrollBar(list_ctrl_ptr))
        {
            return result;
        }
        else if ((cur_item_idx = LIST_GetItemIndexByPoint(list_ctrl_ptr, point_ptr)) == INVALID_ITEM_INDEX)
        {
            return result;
        }

        //向下滑动
        if (point_ptr->y > list_ctrl_ptr->move_info.prev_item_rect.bottom)
        {
            continue_move_type = CONTINUE_MOVE_DOWN;

            skip_line_num = cur_item_idx - list_ctrl_ptr->cur_item_idx;
        }
        //向上滑动
        else if (point_ptr->y < list_ctrl_ptr->move_info.prev_item_rect.top)
        {
            continue_move_type = CONTINUE_MOVE_UP;

            skip_line_num = list_ctrl_ptr->cur_item_idx - cur_item_idx;
        }
        else
        {
            return result;
        }

        //若Item项不满一屏，触笔在List列表区域但滑出Item区
        //一页的item 没有填满list区域
        if ((cur_item_idx >= list_ctrl_ptr->total_item_num
             && list_ctrl_ptr->total_item_num <= page_item_num))
        {
            return result;
        }

        ListStopTextAnim(list_ctrl_ptr);

        //修改按下位置的条目状态
        if (!list_ctrl_ptr->move_info.tp_down_handled)
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->move_info.prev_item_index);

            SCI_ASSERT(PNULL != item_ptr);/*assert verified*/

            ListSetItemMarkByStatus(list_ctrl_ptr,
                                    item_ptr,
                                    list_ctrl_ptr->move_info.prev_item_index,
                                    (BOOLEAN)!list_ctrl_ptr->is_tpdown_select);

            list_ctrl_ptr->move_info.tp_down_handled = TRUE;
        }

        cur_item_idx %= list_ctrl_ptr->total_item_num;

        //获取当前行在一页中索引
        line_index = cur_item_idx;

        //向相反方向滑动，恢复刚才的设置
        if (list_ctrl_ptr->move_info.continue_move_type != continue_move_type
            && list_ctrl_ptr->move_info.had_moved)
        {
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->move_info.prev_item_index);
            SCI_ASSERT(PNULL != item_ptr);/*assert verified*/
            ListSetItemMarkByStatus(list_ctrl_ptr,
                                    item_ptr,
                                    list_ctrl_ptr->move_info.prev_item_index,
                                    (BOOLEAN)!list_ctrl_ptr->is_tpdown_select);
        }

        //如果有跳行
        for (i = 1; i <= skip_line_num; i ++)
        {
            //下跳行
            if (CONTINUE_MOVE_DOWN == continue_move_type)
            {
                tmp_item_idx = (uint16)(list_ctrl_ptr->move_info.prev_item_index + i);
            }
            //上跳行
            else if (CONTINUE_MOVE_UP == continue_move_type)
            {
                tmp_item_idx = (uint16)(cur_item_idx + i);
            }

            tmp_item_idx %= list_ctrl_ptr->total_item_num;

            //设置跳行项的状态
            item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, tmp_item_idx);
            SCI_ASSERT(PNULL != item_ptr);/*assert verified*/

            ListSetItemMarkByStatus(list_ctrl_ptr, item_ptr, tmp_item_idx, (BOOLEAN)!list_ctrl_ptr->is_tpdown_select);
        }

        //设置当前项的状态
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, cur_item_idx);
        SCI_ASSERT(PNULL != item_ptr);/*assert verified*/

        ListSetItemMarkByStatus(list_ctrl_ptr, item_ptr, cur_item_idx, (BOOLEAN)!list_ctrl_ptr->is_tpdown_select);

        //记忆滑动信息
        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, line_index);

        //send move msg to window
        if (cur_item_idx != list_ctrl_ptr->cur_item_idx)
        {
            if (CONTINUE_MOVE_DOWN == continue_move_type)
            {
                GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_MOVEDOWN);
            }
            else if (CONTINUE_MOVE_UP == continue_move_type)
            {
                GUICTRL_PostNotify(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_LIST_MOVEUP);
            }

            ListItemResetContentCheck(list_ctrl_ptr);
        }
        else
        {
            ListItemContentCheck(list_ctrl_ptr, msg_id, cur_item_idx, item_ptr, *point_ptr);
        }

        //设置当前的item
        list_ctrl_ptr->pre_cur_item_idx = list_ctrl_ptr->cur_item_idx;
        list_ctrl_ptr->cur_item_idx = cur_item_idx;

        // 计算交集
        if (GUI_IntersectRect(&inter_rect, item_rect, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect)
            && !GUI_RectIsCovered(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect, item_rect)) // 有交集，不需要释放
        {
            if (item_rect.top < inter_rect.top)
            {
                offset_y = inter_rect.top - item_rect.top;
            }
            else if (item_rect.bottom > inter_rect.bottom)
            {
                offset_y = inter_rect.bottom - item_rect.bottom;
            }
            else
            {
                offset_y = 0;
            }
        }

        list_ctrl_ptr->offset_y += offset_y;

        // 重新设置item 高度
        LIST_ResetItemHeight(list_ctrl_ptr, list_ctrl_ptr->pre_cur_item_idx, list_ctrl_ptr->cur_item_idx);

        LIST_CalculateListItemRect(&list_ctrl_ptr->move_info.prev_item_rect, list_ctrl_ptr, line_index);

        list_ctrl_ptr->move_info.prev_item_index = cur_item_idx;
        list_ctrl_ptr->move_info.continue_move_type = continue_move_type;
        list_ctrl_ptr->move_info.had_moved = TRUE;

        if (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_SET_CHECK_AUTO))
        {
            ListSetSelectedItemByPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, TRUE);
        }

        //刷新列表
        // validate list
        ListValidateCtrl(list_ctrl_ptr , FALSE, FALSE);
        ListDraw(list_ctrl_ptr, INNER_STATE_UPDATE_ITEM | INNER_STATE_UPDATE_BG | INNER_STATE_UPDATE_PRG);
        ListUpdateHighlightItem(list_ctrl_ptr);
    }

    return result;
}
#endif

#define SLEEP_TIME1 30
#define SLEEP_TIME2 20
#define FLING_VELOCITY_MIN 400

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle the tp up message
//  Global resource dependence :
//  Author: xiaoqinglu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleItemTpUp(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_recover = FALSE;
    float       fling_velocity = 0;
    int16       offset_x = 0;
    GUI_RECT_T  item_rect = {0};


    list_ctrl_ptr->is_tp_down = FALSE;

    list_ctrl_ptr->is_pre_send_ok = FALSE;

    ListStopTpTimer(list_ctrl_ptr);

    ListStopTpMoveTimer(list_ctrl_ptr);

    ListResumeItemTextTimer(list_ctrl_ptr);

    // 恢复
    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SLIDE_ITEM)
        && !LIST_GetItemStateByIndex(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, (GUIITEM_STATE_NOTHOR_SLIDE)))
    {
        offset_x = point_ptr->x - list_ctrl_ptr->slide_start_point.x;

        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        MMK_ComputeCurrentVelocity(&fling_velocity, PNULL, FLING_MAX_VELOCITY, FLING_MAX_VELOCITY);

        if (abs(offset_x) > abs((((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.right - ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.left + 1) / 3))
        {

            if (point_ptr->x > list_ctrl_ptr->slide_start_point.x)
            {
                // 右侧发送消息
                LIST_DrawItemAnim(list_ctrl_ptr, item_rect.left + offset_x, item_rect.right, item_rect, SLEEP_TIME1);
                GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_TPMOVE_RIGHT_SIDE, PNULL, 0);
            }
            else
            {
                // 左侧发送消息
                LIST_DrawItemAnim(list_ctrl_ptr, item_rect.left + (offset_x), item_rect.left - (item_rect.right - item_rect.left + 1), item_rect, SLEEP_TIME1);
                GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_TPMOVE_LEFT_SIDE, PNULL, 0);
            }
        }
        else if (abs((int32)fling_velocity) > FLING_VELOCITY_MIN)
        {
            if (fling_velocity > 0)
            {
                // 右侧发送消息
                LIST_DrawItemAnim(list_ctrl_ptr, item_rect.left + offset_x, item_rect.right, item_rect, SLEEP_TIME2);
                GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_TPMOVE_RIGHT_SIDE, PNULL, 0);
            }
            else
            {
                // 左侧发送消息
                LIST_DrawItemAnim(list_ctrl_ptr, item_rect.left + (offset_x), item_rect.left - (item_rect.right - item_rect.left + 1), item_rect, SLEEP_TIME2);
                GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_TPMOVE_LEFT_SIDE, PNULL, 0);
            }
        }
        else
        {
            if (point_ptr->x > list_ctrl_ptr->slide_start_point.x - 5 && point_ptr->x < list_ctrl_ptr->slide_start_point.x + 5)
            {
                is_recover = TRUE;
                // to do...
                // 这里要恢复当前item的显示
            }
            else
            {
                is_recover = TRUE;
                LIST_DrawItemAnim(list_ctrl_ptr, item_rect.left + (offset_x), item_rect.left, item_rect, SLEEP_TIME1);
            }
        }

        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_SLIDE_ITEM, FALSE);

        if (is_recover)
        {
            if (!list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
            {
                // 画高亮条
                ListUpdateHighlightItem(list_ctrl_ptr);
            }
            else
            {
                ListRedrawItem(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
            }
        }

        result = TRUE;
    }

    LIST_ReleaseItemDispLayer();
    LIST_ReleaseItemMistLayer();
    list_ctrl_ptr->is_long_pressing = FALSE;
    return result;
}

/*****************************************************************************/
//  Description : handle the tp up message
//  Global resource dependence :
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleItemTpUpEx(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
)
{
    BOOLEAN     result = FALSE;
    float       fling_velocity = 0;
    int16       offset_x = 0;
    GUI_RECT_T  item_rect = {0};
    uint8       offset_max = 0;


    list_ctrl_ptr->is_tp_down = FALSE;

    list_ctrl_ptr->is_pre_send_ok = FALSE;

    ListStopTpTimer(list_ctrl_ptr);

    ListStopTpMoveTimer(list_ctrl_ptr);

    ListResumeItemTextTimer(list_ctrl_ptr);

    offset_max = (MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT + MMICOM_LIST_SLIDE_ICON_MARGIN) * MMICOM_LIST_SLIDE_ICON_NUM + MMICOM_LIST_SLIDE_ICON_MARGIN;

    // 恢复
    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_SLIDE_ITEM)
        && !LIST_GetItemStateByIndex(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx, (GUIITEM_STATE_NOTHOR_SLIDE)))
    {
        offset_x = point_ptr->x - list_ctrl_ptr->slide_start_point.x;

        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        MMK_ComputeCurrentVelocity(&fling_velocity, PNULL, FLING_MAX_VELOCITY, FLING_MAX_VELOCITY);

        if(-offset_x < offset_max)
        {
            LIST_DrawItemAnim(list_ctrl_ptr, item_rect.left + (offset_x), item_rect.left, item_rect, SLEEP_TIME1);
            LIST_ReleaseItemDispLayer();
            if ((!list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
                && (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR)))
            {
                // 画高亮条
                ListUpdateHighlightItem(list_ctrl_ptr);
            }
            else
            {
                ListRedrawItem(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
            }
        }
        else
        {
            LIST_DrawItemAnim(list_ctrl_ptr, item_rect.left + (offset_x), item_rect.left + (-offset_max), item_rect, SLEEP_TIME1);
            list_ctrl_ptr->is_left_slide_unfold = TRUE;
        }

        LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_SLIDE_ITEM, FALSE);

        result = TRUE;
    }
    else
    {
        if(list_ctrl_ptr->is_left_slide_unfold == TRUE)
        {
            LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
            if(( point_ptr->y >= item_rect.top )&&( point_ptr->y <= item_rect.bottom))
            {
                if(( point_ptr->x >( item_rect.right - offset_max ))
                    && ( point_ptr->x < item_rect.right - offset_max + MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT + MMICOM_LIST_SLIDE_ICON_MARGIN)
                    )
                {
                    GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_TPCLICK_LEFT_ICON, PNULL, 0);
                }
                else if(( point_ptr->x < item_rect.right )
                    && ( point_ptr->x > item_rect.right - offset_max + MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT + MMICOM_LIST_SLIDE_ICON_MARGIN)
                    )
                {
                    GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_NOTIFY_TPCLICK_RIGHT_ICON, PNULL, 0);
                }
            }
            list_ctrl_ptr->is_left_slide_unfold = FALSE;

            result = TRUE;
        }
        LIST_ReleaseItemDispLayer();
        if ((!list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
            && (LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR)))
        {
            // 画高亮条
            ListUpdateHighlightItem(list_ctrl_ptr);
        }
        else
        {
            ListRedrawItem(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
        }

    }

    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_SLIDE_ITEM, FALSE);
    LIST_ReleaseItemMistLayer();
    list_ctrl_ptr->is_long_pressing = FALSE;
    return result;
}

/*****************************************************************************/
//  Description : handle the tp up message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpUp(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *point_ptr
)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_need_notify = TRUE;
//     CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    // 首先处理
    if (LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_SLIDE_ITEM))
    {
        if((LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_LEFT_SLIDE_ITEM) && ListHandleItemTpUpEx(list_ctrl_ptr, point_ptr))
            || ListHandleItemTpUp(list_ctrl_ptr, point_ptr))
        {
            return TRUE;
        }
    }

    list_ctrl_ptr->is_tp_down = FALSE;

    list_ctrl_ptr->is_pre_send_ok = FALSE;

    ListStopTpTimer(list_ctrl_ptr);

    ListStopTpMoveTimer(list_ctrl_ptr);

    ListResumeItemTextTimer(list_ctrl_ptr);

//     if (list_ctrl_ptr->total_item_num > 0)
//     {
//         item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
//     }

    if ((MMK_TP_SLIDE_NONE != list_ctrl_ptr->slide_state || (IS_TP_MOVE_Y(point_ptr->y, list_ctrl_ptr->slide_start_point.y)))
        && list_ctrl_ptr->display_style.is_slide_used)
    {
        float mDeceleration = 0.0f;

        // compute
        if (list_ctrl_ptr->tpdown_slide_state == MMK_TP_SLIDE_FLING && abs((int)list_ctrl_ptr->fling_velocity) > (int)FLING_MIN_VELOCITY)
        {
            MMK_ComputeCurrentVelocity(PNULL, &list_ctrl_ptr->fling_velocity, FLING_MAX_SUM_VELOCITY, FLING_MAX_SUM_VELOCITY);
        }
        else
        {
            MMK_ComputeCurrentVelocity(PNULL, &list_ctrl_ptr->fling_velocity, FLING_MAX_VELOCITY, FLING_MAX_VELOCITY);
        }

        mDeceleration = MMITHEME_GetFlingFrictionExt();

        list_ctrl_ptr->total_distance = (int32)((list_ctrl_ptr->fling_velocity * list_ctrl_ptr->fling_velocity) / (2 * mDeceleration));

        list_ctrl_ptr->total_duration = (int32)(1000 * list_ctrl_ptr->fling_velocity / mDeceleration);

        //SCI_TRACE_LOW:"ListHandleTpUp %f"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_14646_112_2_18_3_20_12_227, (uint8 *)"f", list_ctrl_ptr->fling_velocity);

        if (FLING_MIN_VELOCITY <= abs((int32)list_ctrl_ptr->fling_velocity))
        {
            ListStartTpMoveTimer(list_ctrl_ptr);
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_FLING;
        }
        else
        {
            int32       offset_y = ListGetRecoverSpace(list_ctrl_ptr);

            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

            list_ctrl_ptr->fling_velocity = 0;
            if(LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_AUTO_SCROLL))//fix bug1594837 1595403 1586984
            {
                SetListMiddleItemFocus(list_ctrl_ptr);
            }

            MMK_ResetVelocityItem();

            if (!LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_CUSTORMER_LAYER))
            {
                //恢复双buffer copy
                UILAYER_SwitchDoubleMem(&list_ctrl_ptr->item_layer, FALSE);
            }

            if (0 != offset_y)
            {
                // 恢复
                ListRecoverSlideByOffsetY(list_ctrl_ptr, TRUE, offset_y);
            }
            else
            {
                // 画图
                ListMoveAllItem(list_ctrl_ptr);
            }

            if (!list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
            {
                // 画高亮条
                ListUpdateHighlightItem(list_ctrl_ptr);
            }
        }
    }
    else if (list_ctrl_ptr->is_click_item &&
             LIST_IsItemGetFocus(list_ctrl_ptr, list_ctrl_ptr->tp_select_item_idx) &&
             ListHandleTPKeyUp(list_ctrl_ptr, msg_id, *point_ptr))  //标记或选中
    {
        if (!(LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_ENABLE_MARK) && list_ctrl_ptr->move_info.had_moved))
        {
            uint16 pre_index = list_ctrl_ptr->pre_cur_item_idx;

            if (ListHandleKeyOK(list_ctrl_ptr, msg_id, &is_need_notify))
            {
                //非标记状态, 发送notify
                if (is_need_notify)
                {
                    //利用notify code标识是否双击
                    MMI_NOTIFY_T notify = {0};
                    MMI_MESSAGE_ID_E send_msg_id = MSG_NOTIFY_PENOK;

                    notify.src_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
                    notify.src_id     = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
                    notify.code       = (BOOLEAN)(pre_index == list_ctrl_ptr->cur_item_idx);

                    //notify parent
                    GUICTRL_PostNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, send_msg_id, &notify, sizeof(MMI_NOTIFY_T));
                    list_ctrl_ptr->is_pre_send_ok = TRUE;
                }
            }

            // 如果高亮消失，则重绘
            if (list_ctrl_ptr->display_style.is_hl_disappear)
            {
                ListRedrawItem(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
            }
        }
    }
    else
    {
        /* in any case, we should remove the highlighting */
        if (list_ctrl_ptr->display_style.is_hl_disappear)
        {
            ListRedrawItem(list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);
        }
    }

    if (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state)
    {
        ListPostSlideStateMsg(list_ctrl_ptr, GUILIST_END_SLIDE);
    }

    list_ctrl_ptr->is_click_item = FALSE; // 点击ITEM的状态恢复
    list_ctrl_ptr->is_tpdown_select = FALSE;

    ListItemResetContentCheck(list_ctrl_ptr);

    return result;
}

/*****************************************************************************/
//  Description : Stop Fling
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListStopFling(
    CTRLLIST_OBJ_T   *list_ctrl_ptr
)
{

    if (PNULL != list_ctrl_ptr)
    {
        ListStopTpMoveTimer(list_ctrl_ptr);
        list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
        list_ctrl_ptr->fling_velocity = 0;
    }
}

/*****************************************************************************/
//  Description : item被点击时，返回item当前选中的content信息
//  Global resource dependence :
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void ListHandleSendItemContentClick(
                                                        CTRLLIST_OBJ_T   *list_ctrl_ptr,
                                                        GUI_POINT_T      *point_ptr
                                                     )
{
    uint8 content_i = 0;
    GUI_RECT_T item_rect = {0};
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    if ((PNULL == list_ctrl_ptr) || (PNULL == point_ptr))
    {
        return;
    }

    //get item info
    item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, list_ctrl_ptr->tp_select_item_idx);

    //get item rect
    LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->tp_select_item_idx);

    if (PNULL == item_ptr || PNULL == item_ptr->item.data_ptr)
    {
        return;
    }

    //judge content by point
    for (content_i = 0; content_i < GUIITEM_CONTENT_MAX_NUM; content_i++)
    {
        GUI_RECT_T con_rect = item_ptr->content_rect[content_i];

        //con_rect -> absolute
        SetRectOffset(&con_rect, item_rect.left, item_rect.top);

        if(
            (point_ptr->x > con_rect.left)&&
            (point_ptr->x < con_rect.right)&&
            (point_ptr->y > con_rect.top)&&
            (point_ptr->y < con_rect.bottom)
           )
        {
             LIST_SendItemContentClickMsg(list_ctrl_ptr,list_ctrl_ptr->tp_select_item_idx,content_i);
            break;
        }
    }

}
#endif

/*****************************************************************************/
//@brief 设置控件的Item显示的layer
//@author Xiaoqing.lu
//@param ctrl_id           [in] ctrl ID
//@param get_data_func     [in] func ptr
//@retval true/false
//@return true/false
//@note
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_RegGetDataFunc(
    MMI_CTRL_ID_T ctrl_id,
    CTRLLIST_GET_DATA_FUNC get_data_func
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        result = TRUE;
        list_ctrl_ptr->get_data_func = get_data_func;
    }

    return TRUE;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle the tp up message
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleTpLong(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
)
{
    if (PNULL == list_ctrl_ptr || PNULL == point_ptr)
    {
        return FALSE;
    }

    if (MMK_TP_SLIDE_FLING == list_ctrl_ptr->slide_state)
    {
        ListStopTpTimer(list_ctrl_ptr);

        ListStopTpMoveTimer(list_ctrl_ptr);

        list_ctrl_ptr->fling_velocity = 0;
    }

//#ifdef MMI_PDA_SUPPORT
    else if (MMK_TP_SLIDE_NONE == list_ctrl_ptr->slide_state)
    {
        GUI_RECT_T item_rect = {0};
        GUILIST_NEED_ITEM_CONTENT_T need_item_content = {0};

        need_item_content.ctrl_handle   = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        need_item_content.ctrl_id       = MMK_GetCtrlId(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle);
        need_item_content.item_index    = list_ctrl_ptr->cur_item_idx;
        need_item_content.item_content_index    = list_ctrl_ptr->tp_select_item_content_idx;

        LIST_CalculateListItemRect(&item_rect, list_ctrl_ptr, list_ctrl_ptr->cur_item_idx);

        if (GUI_PointIsInRect(*point_ptr, item_rect))
        {
            list_ctrl_ptr->is_long_pressing = TRUE;
            ListResumeItemTextTimer(list_ctrl_ptr);
            GUICTRL_SendNotifyEx(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_CTL_LIST_LONGOK, &need_item_content);
        }
    }

//#endif
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUILISTBOX_14889_112_2_18_3_20_12_228, (uint8 *)"d", list_ctrl_ptr->slide_state);
    }

    return TRUE;
}
#endif


#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 处理滑动效果
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListHandleSlide(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
)
{
    BOOLEAN     result = FALSE;
    uint16      pre_top_index = 0;
    uint16      pre_end_index = 0;
    GUI_RECT_T  list_rect = {0};
    int32       orig_offset_y = 0;
    int32       recover_space = 0;
    int32       top_offset_y = 0;
    int32       end_offset_y = 0;
    int32       reach_ori_offset_y = 0;
    int32       list_rect_height = 0;
    int32       offset_y = 0;

    if ((PNULL == list_ctrl_ptr) || (PNULL == tp_point_ptr))
    {
        return FALSE;
    }

    if (0 == list_ctrl_ptr->total_item_num)
    {
        ListStopFling(list_ctrl_ptr);

        return result;
    }

    offset_y = list_ctrl_ptr->offset_y;

    pre_top_index = list_ctrl_ptr->pre_top_item_idx;
    pre_end_index = list_ctrl_ptr->pre_end_item_idx;

    if (MMK_TP_SLIDE_TP_SCROLL == list_ctrl_ptr->slide_state || MMK_TP_SLIDE_FLING == list_ctrl_ptr->slide_state)
    {
        reach_ori_offset_y = list_ctrl_ptr->offset_y;

        // 第一条显示在第一行的偏移量
        LIST_ResetOffsetYByTopIndex(list_ctrl_ptr, 0);
        top_offset_y = list_ctrl_ptr->offset_y;
        list_ctrl_ptr->offset_y = reach_ori_offset_y; // 恢复

        // 最后一条显示在最后一行的偏移量
        LIST_ResetOffsetYByEndIndex(list_ctrl_ptr, list_ctrl_ptr->total_item_height);
        end_offset_y = list_ctrl_ptr->offset_y;
        list_ctrl_ptr->offset_y = reach_ori_offset_y; // 恢复

        list_rect = LIST_AdjustRect(list_ctrl_ptr);
        list_rect_height = list_rect.bottom - list_rect.top + 1;

        if (MMK_TP_SLIDE_FLING == list_ctrl_ptr->slide_state)
        {
            orig_offset_y = list_ctrl_ptr->offset_y;

            list_ctrl_ptr->offset_y = MMK_GetFlingOffset(list_ctrl_ptr->offset_y, list_ctrl_ptr->fling_velocity, (float)FLING_TIME, &list_ctrl_ptr->fling_velocity);

            if (FLING_RUN_MIN_VELOCITY > abs((int32)list_ctrl_ptr->fling_velocity))
            {
                ListStopTpTimer(list_ctrl_ptr);
                ListStopTpMoveTimer(list_ctrl_ptr);
                ListResumeItemTextTimer(list_ctrl_ptr);
                recover_space = ListGetRecoverSpace(list_ctrl_ptr);
                list_ctrl_ptr->fling_velocity = 0;
                result = TRUE;
            }
            else if (0 > list_ctrl_ptr->fling_velocity) // 向上
            {
                if (list_ctrl_ptr->offset_y < end_offset_y - (((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.bottom - ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.top) / 2) // 1/2处恢复
                {
                    ListStopTpTimer(list_ctrl_ptr);
                    ListStopTpMoveTimer(list_ctrl_ptr);
                    ListResumeItemTextTimer(list_ctrl_ptr);
                    list_ctrl_ptr->offset_y = end_offset_y - (((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.bottom - ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.top) / 2;
                    recover_space = ListGetRecoverSpace(list_ctrl_ptr);
                    list_ctrl_ptr->fling_velocity = 0;
                    result = TRUE; // 需要恢复
                }
            }
            else // 向下
            {
                if (list_ctrl_ptr->offset_y > top_offset_y + (((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.bottom - ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.top) / 2) // 1/2处恢复
                {
                    ListStopTpTimer(list_ctrl_ptr);
                    ListStopTpMoveTimer(list_ctrl_ptr);
                    ListResumeItemTextTimer(list_ctrl_ptr);
                    list_ctrl_ptr->offset_y = top_offset_y + (((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.bottom - ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect.top) / 2;
                    recover_space = ListGetRecoverSpace(list_ctrl_ptr);
                    list_ctrl_ptr->fling_velocity = 0;
                    result = TRUE;
                }
            }

            LIST_ResetTopItemAndEndItemIndexExt(list_ctrl_ptr, list_ctrl_ptr->offset_y - orig_offset_y);
            LIST_DestroyAllItemDataNotInPageExt(list_ctrl_ptr, pre_top_index, pre_end_index);
        }
        else
        {
            // 释放所有不在当前页的数据
            list_ctrl_ptr->offset_y += (tp_point_ptr->y - list_ctrl_ptr->slide_pre_point.y);

            if (list_ctrl_ptr->top_item_idx < INNERLIST_ITEM_NUMBER) // 限制个数
            {
                LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);
            }
            else
            {
                LIST_ResetTopItemAndEndItemIndexExt(list_ctrl_ptr, tp_point_ptr->y - list_ctrl_ptr->slide_pre_point.y);
            }

            LIST_DestroyAllItemDataNotInPageExt(list_ctrl_ptr, pre_top_index, pre_end_index);
        }

        if (result) // 表示需要恢复
        {
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

            if (!LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_CUSTORMER_LAYER))
            {
                //恢复双buffer copy
                UILAYER_SwitchDoubleMem(&list_ctrl_ptr->item_layer, FALSE);
            }

            if (0 == recover_space)
            {
                // 画图
                ListMoveAllItem(list_ctrl_ptr);
            }
            else
            {
                // 恢复
                ListRecoverSlideByOffsetY(list_ctrl_ptr, TRUE, recover_space);
            }

            if(LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_AUTO_SCROLL))//fix bug1594837 1595403 1586984
            {
                SetListMiddleItemFocus(list_ctrl_ptr);
            }

            // 此时，如果高亮条不总是显示的话，不需要更新高亮条
            if (!list_ctrl_ptr->display_style.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
            {
                // 画高亮条
                ListUpdateHighlightItem(list_ctrl_ptr);
            }

            // 通知应用滑动结束
            ListPostSlideStateMsg(list_ctrl_ptr, GUILIST_END_SLIDE);
        }
        else
        {
            SCI_MEMSET(&list_ctrl_ptr->writeback_rect, 0, sizeof(GUI_RECT_T));

            //可能将list设置到主层上
            if (!LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_CUSTORMER_LAYER))
            {
                UILAYER_SetMainLayerImmutable(TRUE);
            }

            UILAYER_SetDirectDraw(TRUE);

            //设置双buffer切换，不使用copy
            ListDoDoubleMemWriteBack(list_ctrl_ptr, offset_y);

            // 画图
            ListMoveAllItem(list_ctrl_ptr);

            SCI_MEMSET(&list_ctrl_ptr->writeback_rect, 0, sizeof(GUI_RECT_T));
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : ListDrawAnim
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListDrawAnim(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    int32 move_space_y
)
{
    BOOLEAN     result      = FALSE;
    int32       i           = 0;
    int32       frame_num   = MMK_RECOVER_SLIDE_TOTAL_FRAME;
    int32       offset      = 0;
    int32       offset_y    = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T page_rect = {0};

    if (PNULL == list_ctrl_ptr || 0 == move_space_y)
    {
        return result;
    }

    lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

    LIST_GetValidRect(list_ctrl_ptr, &page_rect);

    offset_y = list_ctrl_ptr->offset_y;

    list_ctrl_ptr->slide_state = MMK_TP_SLIDE_RECOVER; // 恢复状态

    for (i = MMK_RECOVER_SLIDE_START_FRAME; i <= frame_num; i++)
    {
        offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, move_space_y, frame_num, i);

        list_ctrl_ptr->offset_y = offset + offset_y;

        LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);

        if (!UILAYER_IsLayerActive(&list_ctrl_ptr->item_layer) // 非active的时候要重新刷背景，否则花屏
            || ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info.block_id == list_ctrl_ptr->item_layer.block_id) // 此时重新刷背景
        {
            ListDrawBackground(list_ctrl_ptr);
        }
        else
        {
            UILAYER_ClearRect(&lcd_dev_info, page_rect);
        }

        if (i == frame_num)
        {
            list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE; // 回复状态之后，置NONE
        }

        // 画items
        ListDrawAllItems(list_ctrl_ptr, &lcd_dev_info);

        // ListDrawTitle(list_ctrl_ptr, TRUE);

        GUILCD_InvalidateRect(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info.lcd_id, ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->rect, 0);
    }

    list_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE; // 回复状态之后，置NONE

    // scroll bar
    ListDrawScrollBar(list_ctrl_ptr, TRUE);


    return result;
}
#endif

/*****************************************************************************/
//  Description : get recover space
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL int32 ListGetRecoverSpace(
    CTRLLIST_OBJ_T   *list_ctrl_ptr
)
{
    int32       offset_y = 0;
    int32       top_offset_y = 0;
    int32       end_offset_y = 0;
    int32       move_space = 0;

    if (PNULL == list_ctrl_ptr)
    {
        return move_space;
    }

    offset_y = list_ctrl_ptr->offset_y;

    // 第一条显示在第一行的偏移量
    LIST_ResetOffsetYByTopIndex(list_ctrl_ptr, 0);
    top_offset_y = list_ctrl_ptr->offset_y;

    // 最后一条显示在最后一行的偏移量
    LIST_ResetOffsetYByEndIndex(list_ctrl_ptr, list_ctrl_ptr->total_item_height);
    end_offset_y = list_ctrl_ptr->offset_y;

    // 恢复offset Y 值
    list_ctrl_ptr->offset_y = offset_y;

    // 判断移动方向和移动的位移
    if (offset_y > top_offset_y) // 这个说明第一条已经向下滑动了，应该向上恢复
    {
        move_space = top_offset_y - offset_y;
    }
    else if (offset_y < end_offset_y) // 比最高位置高，应该向下恢复
    {
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR) // BUG-1594143 & BUG-2086463
        uint32  total_rect_height = 0;
        uint32  cur_list_slid_height = 0;
        GUI_RECT_T list_rect = {0};

        list_rect = LIST_AdjustRect(list_ctrl_ptr);
        total_rect_height = list_rect.bottom - list_rect.top + 1;
        cur_list_slid_height = list_ctrl_ptr->total_item_height + ADULTWATCHLIST_BOTTOM_OFFSET;

        move_space = end_offset_y - offset_y;
        if ( cur_list_slid_height >= total_rect_height)
        {
            move_space = move_space - ADULTWATCHLIST_BOTTOM_OFFSET;
        }
#else
        move_space = end_offset_y - offset_y;
#endif
    }
    else
    {
        move_space = 0;
    }

    return move_space;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : resume tp up
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ListRecoverSlideByOffsetY(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    BOOLEAN         is_update,
    int32           move_space
)
{
    int32 offset_y = 0;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    offset_y = list_ctrl_ptr->offset_y;

    if (is_update && 0 != move_space)
    {
        ListDrawAnim(list_ctrl_ptr, move_space);
    }

    list_ctrl_ptr->offset_y = offset_y + move_space;

    return;
}

/*****************************************************************************/
//  Description : 画全屏的list
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ListMoveAllItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T page_rect = {0};

    LIST_GetValidRect(list_ctrl_ptr, &page_rect);

    lcd_dev_info = ListGetDispLayerInfo(list_ctrl_ptr);

    // 如果item layer有效，则需要进行层的合并
    if (UILAYER_IsMultiLayerEnable()
        && UILAYER_IsLayerActive(&list_ctrl_ptr->item_layer)
        && list_ctrl_ptr->display_style.is_slide_used)
    {
        //若无回写才需要擦图层
        if (GUI_IsZeroRect(list_ctrl_ptr->writeback_rect))
        {
            if (((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info.block_id == list_ctrl_ptr->item_layer.block_id) // 此时重新刷背景
            {
                ListDrawBackground(list_ctrl_ptr);
            }
            else
            {
                UILAYER_ClearRect(&lcd_dev_info, page_rect);
            }
        }
    }
    else
    {
        ListDrawBackground(list_ctrl_ptr);
    }

    // 画items
    ListDrawAllItems(list_ctrl_ptr, &lcd_dev_info);

    // scroll bar
    ListDrawScrollBar(list_ctrl_ptr, TRUE);
}
#endif

#ifdef UI_MULTILAYER_SUPPORT
/*==============================================================================
Description: 创建tip的显示层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN ListCreateTipLayer(CTRLLIST_OBJ_T *list_ctrl_ptr)
{
    if (!UILAYER_IsMultiLayerEnable())
    {
        return FALSE;
    }

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    // 创建层
    if (UILAYER_NULL_HANDLE == list_ctrl_ptr->tip_layer.block_id)
    {
        UILAYER_CREATE_T create_info = {0};
        uint16 tip_width = 0;
        uint16 tip_height = 0;

        //get tip bg size
        GUIRES_GetImgWidthHeight(&tip_width,
                                 &tip_height,
                                 list_ctrl_ptr->display_style.tip_bg,
                                 list_ctrl_ptr->win_id);

        list_ctrl_ptr->tip_rect.top = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect.top;
        list_ctrl_ptr->tip_rect.top += tip_height / 2;
        list_ctrl_ptr->tip_rect.left = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect.left / 2;
        list_ctrl_ptr->tip_rect.left += ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->display_rect.right / 2;
        list_ctrl_ptr->tip_rect.left -= tip_width / 2;
        list_ctrl_ptr->tip_rect.bottom = list_ctrl_ptr->tip_rect.top + tip_height - 1;
        list_ctrl_ptr->tip_rect.right = list_ctrl_ptr->tip_rect.left + tip_width - 1;

        create_info.lcd_id = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->lcd_dev_info.lcd_id;
        create_info.owner_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
        create_info.offset_x = list_ctrl_ptr->tip_rect.left;
        create_info.offset_y = list_ctrl_ptr->tip_rect.top;
        create_info.width = tip_width;
        create_info.height = tip_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_DOUBLE_COPY;

        UILAYER_CreateLayer(&create_info, &list_ctrl_ptr->tip_layer);
        UILAYER_SetLayerColorKey(&list_ctrl_ptr->tip_layer, TRUE, UILAYER_TRANSPARENT_COLOR);
    }

    return TRUE;
}

/*==============================================================================
Description: 清除tip层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ListReleaseTipLayer(CTRLLIST_OBJ_T *list_ctrl_ptr)
{
    if (!UILAYER_IsMultiLayerEnable())
    {
        return;
    }

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (0 != list_ctrl_ptr->tip_hide_timer)
    {
        MMK_StopTimer(list_ctrl_ptr->tip_hide_timer);
        list_ctrl_ptr->tip_hide_timer = 0;
    }

    UILAYER_RELEASELAYER(&(list_ctrl_ptr->tip_layer));   /*lint !e506 !e774*/

    return;
}

/*==============================================================================
Description: 显示split上的提示字符
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 找到第一个有group的项
2 找到该项所属的group，即split item
3 显示split item中的字符作为tips
==============================================================================*/
LOCAL void ListDispSplitTip(CTRLLIST_OBJ_T *list_ctrl_ptr, wchar tip_word)
{
    BOOLEAN has_split = FALSE;
    int16 item_idx = 0;
    uint16 first_group_idx = 0;
    uint16 last_group_idx = 0;
    uint16 split_item_idx = 0;
    BOOLEAN has_tip = FALSE;
    MMI_STRING_T tip_info = {0};
    wchar        tip_buf[2] = {0};
    GUISTR_STYLE_T tip_style = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    THEMELIST_ITEM_STYLE_T *item_style_ptr = PNULL;
    uint8 font_factor = 0;

    if (PNULL == list_ctrl_ptr || TRUE == list_ctrl_ptr->is_forbid_tip)
    {
        return;
    }

    if (!UILAYER_IsMultiLayerEnable())
    {
        return;
    }

    if (!MMK_IsFocusWin(list_ctrl_ptr->win_id))
    {
        return;
    }

    if (0 == tip_word)
    {
        if (PNULL == list_ctrl_ptr->item_info_ptr || list_ctrl_ptr->end_item_idx >= list_ctrl_ptr->total_item_num)
        {
            return;
        }

        //快速判断是否有split
        last_group_idx = list_ctrl_ptr->item_info_ptr[list_ctrl_ptr->end_item_idx].item.group_idx;
        has_split = (0 != last_group_idx);

        if (!has_split)
        {
            return;
        }

        first_group_idx = list_ctrl_ptr->item_info_ptr[list_ctrl_ptr->top_item_idx].item.group_idx;

        if (0 == first_group_idx)
        {
            for (item_idx = (int16)(list_ctrl_ptr->top_item_idx + 1); item_idx <= (int16)list_ctrl_ptr->end_item_idx; item_idx++)
            {
                if (list_ctrl_ptr->item_info_ptr[item_idx].item.group_idx > 0)
                {
                    //第一个非零的必然是split item
                    SCI_ASSERT(GUIITEM_STATE_SPLIT & list_ctrl_ptr->item_info_ptr[item_idx].item.state); /*assert verified*/
                    split_item_idx = item_idx;
                    break;
                }
            }
        }
        else
        {
            for (item_idx = (int16)list_ctrl_ptr->top_item_idx; item_idx >= 0; item_idx--)
            {
                //只要不是split item，必然是同一group的
                if ((list_ctrl_ptr->item_info_ptr[item_idx].item.group_idx == first_group_idx)
                    && (GUIITEM_STATE_SPLIT & list_ctrl_ptr->item_info_ptr[item_idx].item.state))
                {
                    split_item_idx = item_idx;
                    break;
                }
            }
        }

        if (GUIITEM_STATE_SPLIT & list_ctrl_ptr->item_info_ptr[split_item_idx].item.state) //; /*assert verified*/
        {
            item_style_ptr = list_ctrl_ptr->item_info_ptr[split_item_idx].item.style_ptr;

            if (PNULL != item_style_ptr)
            {
                has_tip = LIST_GetListItemContentTextInfo(list_ctrl_ptr,
                                                          &(list_ctrl_ptr->item_info_ptr[split_item_idx]),
                                                          item_style_ptr->main_index,
                                                          &tip_info);
            }
        }

        if (!has_tip)
        {
            return;
        }
    }
    else
    {
        tip_info.wstr_ptr = tip_buf;
        tip_info.wstr_ptr[0] = tip_word;
        tip_info.wstr_len = 1;
        has_tip = TRUE;
    }

    //stop timer(hide tips)
    if (0 != list_ctrl_ptr->tip_hide_timer)
    {
        MMK_StopTimer(list_ctrl_ptr->tip_hide_timer);
        list_ctrl_ptr->tip_hide_timer = 0;
    }

    ListCreateTipLayer(list_ctrl_ptr);

    UILAYER_Clear(&list_ctrl_ptr->tip_layer);

    append_layer.lcd_dev_info = list_ctrl_ptr->tip_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    //draw bg
    //...
    GUI_SetAlphaOverwrite(TRUE);

    GUIRES_DisplayImg(PNULL,
                      &(list_ctrl_ptr->tip_rect),
                      PNULL,
                      list_ctrl_ptr->win_id,
                      list_ctrl_ptr->display_style.tip_bg,
                      &(list_ctrl_ptr->tip_layer));

    GUI_SetAlphaOverwrite(FALSE);

    //draw string
    if (1 == tip_info.wstr_len)
    {
        font_factor = 2;
    }
    else if (tip_info.wstr_len < LIST_TIP_FONT_FACTOR_LIMIT)
    {
        font_factor = tip_info.wstr_len;
    }
    else
    {
        font_factor = LIST_TIP_FONT_FACTOR_LIMIT;
    }

    tip_style.font = (list_ctrl_ptr->tip_rect.bottom - list_ctrl_ptr->tip_rect.top + 1) / font_factor;
    tip_style.font_color = list_ctrl_ptr->display_style.tip_font_color;
    tip_style.align = ALIGN_HVMIDDLE;

    GUISTR_DrawTextToLCDInRect(
        &(list_ctrl_ptr->tip_layer),
        &list_ctrl_ptr->tip_rect,
        &list_ctrl_ptr->tip_rect,
        &tip_info,
        &tip_style,
        GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO
    );

    //UILAYER_WeakLayerAlpha(&(list_ctrl_ptr->tip_layer), list_ctrl_ptr->display_style.tip_layer_alpha);

    //start timer(hide tips)
    list_ctrl_ptr->tip_hide_timer = MMK_CreateTimerCallback(
                                        list_ctrl_ptr->display_style.tip_hide_tick,
                                        ListHideSplitTipCB,
                                        (uint32)list_ctrl_ptr,
                                        FALSE);

    return;
}


/*==============================================================================
Description: 隐藏显示的tip
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ListHideSplitTipCB(uint8 timer_id, uint32 param)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = (CTRLLIST_OBJ_T *)param;

    if (!UILAYER_IsMultiLayerEnable())
    {
        return;
    }

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (0 == timer_id || timer_id != list_ctrl_ptr->tip_hide_timer)
    {
        return;
    }

    UILAYER_RemoveBltLayer(&(list_ctrl_ptr->tip_layer));

    return;
}
#endif

/*****************************************************************************/
//  Description : get top item offset
//  Global resource dependence :
//  Author:xiaoqinglu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetTopItemOffset(
    MMI_CTRL_ID_T ctrl_id,
    int32 *offset_y_ptr
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr || PNULL == offset_y_ptr)
    {
        return FALSE;
    }
    else
    {
        *offset_y_ptr = list_ctrl_ptr->offset_y;
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : set top item offset
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetTopItemOffset(
    MMI_CTRL_ID_T ctrl_id,
    int32 offset_y
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    list_ctrl_ptr->offset_y = offset_y;
}
/*****************************************************************************/
//  Description : 设置是否支持3d
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetDisable3D(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_disable_3d
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    return LIST_SetSlideStateByPtr(list_ctrl_ptr, is_disable_3d);
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : CTRLLIST_SetItemLayer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemLayer(
    MMI_CTRL_ID_T ctrl_id,
    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr && PNULL != lcd_dev_info_ptr)
    {
        ListReleaseItemLayer(list_ctrl_ptr);
        list_ctrl_ptr->item_layer = *lcd_dev_info_ptr;
        return TRUE;
    }

    return FALSE;
}
#endif

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:设置list支持横向滑动，默认不支持
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemHorMove(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_true
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_SLIDE_ITEM, is_true);
}

/*==============================================================================
Description: 获取纯净的item index
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 所谓纯净的，即不计split类的
==============================================================================*/
PUBLIC BOOLEAN CTRLLIST_GetPureIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16 gross_index,
    uint16 *pure_index_ptr
)
{
    BOOLEAN result = FALSE;
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr || gross_index >= list_ctrl_ptr->total_item_num)
    {
        return FALSE;
    }

    // 如果本身是分隔符，那么肯定就取错了，返回原来的索引号
    if (LIST_GetItemStateByIndex(list_ctrl_ptr, gross_index, GUIITEM_STATE_SPLIT))
    {
        *pure_index_ptr = gross_index;
    }
    else
    {
        *pure_index_ptr = gross_index - list_ctrl_ptr->item_info_ptr[gross_index].item.group_idx;
        result = TRUE;
    }

    return result;
}

/*==============================================================================
Description: 获取纯净的item 总数
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 所谓纯净的，即不计split类的
==============================================================================*/
LOCAL uint16 ListGetPureTotalItemNumber(
    CTRLLIST_OBJ_T *list_ctrl_ptr
)
{
    if (PNULL == list_ctrl_ptr || 0 == list_ctrl_ptr->total_item_num)
    {
        return 0;
    }

    return (uint16)(list_ctrl_ptr->total_item_num - list_ctrl_ptr->item_info_ptr[list_ctrl_ptr->total_item_num - 1].item.group_idx);
}

/*****************************************************************************/
//  Description :设置控件的最大项，并申请内存
//  Global resource dependence :
//  Author: Xiaoqing.lu
//  Note: 注意，当初始化数据init_data_ptr为空时，将创建一个空的控件，该控件不能添加Item
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLLIST_CreateListBox(
    MMI_HANDLE_T base_win_handle,
    MMI_HANDLE_T base_ctrl_handle,
    MMI_CTRL_ID_T ctrl_id,
    GUILIST_INIT_DATA_T *init_data_ptr
)
{
    MMI_HANDLE_T         ctrl_handle = 0;
    GUILIST_INIT_DATA_T  init_data = {0};
    MMI_CONTROL_CREATE_T create      = {0};
    CTRLLIST_OBJ_T          *list_ctrl_ptr   = NULL;

    if (PNULL != init_data_ptr)
    {
        init_data.both_rect = init_data_ptr->both_rect;
        init_data.is_dynamic_item_data = init_data_ptr->is_dynamic_item_data;
        init_data.max_item_num = init_data_ptr->max_item_num;
        init_data.max_selected_num = init_data_ptr->max_selected_num;
        init_data.type = init_data_ptr->type;
        init_data.scroll_bar_type = init_data_ptr->scroll_bar_type;
    }
    else
    {
        init_data.type = GUILIST_TEXTLIST_E;
    }

    create.ctrl_id = ctrl_id;
    create.guid = SPRD_GUI_LIST_ID;
    create.init_data_ptr = &init_data;
    create.parent_ctrl_handle = base_ctrl_handle;
    create.parent_win_handle = base_win_handle;

    list_ctrl_ptr = (CTRLLIST_OBJ_T *)MMK_CreateControl(&create);

    if (PNULL != list_ctrl_ptr)
    {
        ctrl_handle = ((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle;
    }

    return ctrl_handle;
}

/*****************************************************************************/
//  Description : Set current item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_IsTpDown(
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN result = FALSE;
#ifdef TOUCH_PANEL_SUPPORT
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        result = list_ctrl_ptr->is_tp_down;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : Get selected check item indexs
//  Global resource dependence :
//  Author:Great.Tian
//  Note: return selected check item number
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetSelectedItemNum(
    MMI_CTRL_ID_T    ctrl_id//in:
)
{
    uint16 item_num = 0;
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        item_num = list_ctrl_ptr->selected_item_num;
    }

    return item_num;
}

/*****************************************************************************/
//  Description : 设置禁止tip显示的状态
//  Global resource dependence :
//  Author: Lianxiang.Zhou
//  Note:默认是显示的
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetForbidTip(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_forbid
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    list_ctrl_ptr->is_forbid_tip = is_forbid;

    return TRUE;
}

/*****************************************************************************/
//  Description : 获得List位置信息
//  Global resource dependence :
//  Author: Lianxiang.Zhou
//  Note:返回List位置信息
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetCurPosInfo(
    MMI_CTRL_ID_T ctrl_id,
    GUILIST_POS_INFO_T *pos_info_ptr
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == pos_info_ptr || PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    pos_info_ptr->cur_item_idx = list_ctrl_ptr->cur_item_idx;
    pos_info_ptr->top_item_idx = list_ctrl_ptr->top_item_idx;
    pos_info_ptr->offset_y = list_ctrl_ptr->offset_y;

    return TRUE;
}
/*****************************************************************************/
//  Description : 设置List位置信息
//  Global resource dependence :
//  Author: Lianxiang.Zhou
//  Note:返回List位置信息
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetCurPosInfo(
    MMI_CTRL_ID_T ctrl_id,
    GUILIST_POS_INFO_T *pos_info_ptr
)
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);
    BOOLEAN result = FALSE;

    if (PNULL == pos_info_ptr || PNULL == list_ctrl_ptr)
    {
        return result;
    }

    //Set Current index
    result = ListResetCurItemIndex(list_ctrl_ptr, pos_info_ptr->cur_item_idx);

    //Set top
    if (pos_info_ptr->top_item_idx < list_ctrl_ptr->total_item_num)
    {
        list_ctrl_ptr->top_item_idx = pos_info_ptr->top_item_idx;

        LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_INVALID, TRUE);

        result = TRUE;
    }
    else
    {
        list_ctrl_ptr->top_item_idx = 0;
    }

    LIST_ResetOffsetYByTopIndex(list_ctrl_ptr, list_ctrl_ptr->top_item_idx);
    LIST_ResetTopItemAndEndItemIndex(list_ctrl_ptr);

    //Set offset
    if (pos_info_ptr->top_item_idx < list_ctrl_ptr->total_item_num)
    {
        list_ctrl_ptr->offset_y = pos_info_ptr->offset_y;
    }

    return result;
}

/*****************************************************************************/
//  Description : Set the margin space
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetMargin(
    MMI_CTRL_ID_T ctrl_id, // control id
    uint16        left_space,
    uint16        right_space
)
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    list_ctrl_ptr->display_style.left_space = left_space;
    list_ctrl_ptr->display_style.right_space = right_space;

    return TRUE;
}

/*****************************************************************************/
//  Description : 设置Item 显示风格
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemStyle(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index,
    THEMELIST_ITEM_STYLE_T *style_ptr
)
{
    CTRLLIST_OBJ_T      *list_ctrl_ptr = PNULL;
    CTRLLIST_ITEM_INFO_T *item_ptr = PNULL;

    //get list ptr
    list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr && PNULL != style_ptr)
    {
        item_ptr = LIST_GetItemInfoPtr(list_ctrl_ptr, item_index);

        if (PNULL != item_ptr)
        {
            item_ptr->item.style_ptr = style_ptr;

            if (!list_ctrl_ptr->is_full_dirty)
            {
                LIST_ResetItemDisp(list_ctrl_ptr, item_index);

                list_ctrl_ptr->is_pos_dirty = TRUE;

                LIST_ResetItemPostion(list_ctrl_ptr);
            }
        }
    }

    return;
}

//============================================================================//
//                                   For watch                                //
//============================================================================//

/*****************************************************************************/
//  Description : is need draw list item background
//  Parameter: [In] style_id: the current item style id
//             [Out] None
//             [Return] is_need : id need draw bg (TRUE : need ; FALSE : don't need)
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN isNeedDrawListItemBG(uint32 style_id)
{
    BOOLEAN is_need = FALSE;
    if( style_id == GUIITEM_STYLE_1STR ||
        style_id == GUIITEM_STYLE_1ICON_1STR ||
        style_id == GUIITEM_STYLE_1STR_1ICON ||
        style_id == GUIITEM_STYLE_1ICON_1STR_1ICON_1STR ||
        style_id == GUIITEM_STYLE_1ICON_2STR ||
        style_id == GUIITEM_STYLE_2STR_1ICON ||
        style_id == GUIITEM_STYLE_2STR ||
        style_id == GUIITEM_STYLE_3STR ||
        style_id == GUIITEM_STYLE_1ICON_3STR||
        style_id == GUIITEM_STYLE_2STR_1ICON_2STR ||
        style_id == GUIITEM_STYLE_1STR_CHECK ||
        style_id == GUIITEM_STYLE_1STR_RADIO ||
        style_id == GUIITEM_STYLE_1STR_SWITCH ||
        style_id == GUIITEM_STYLE_1STR_ENG ||
        style_id == GUIITEM_STYLE_1RADIO_ENG ||
        style_id == GUIITEM_STYLE_1CHECK_ENG ||
		style_id == GUIITEM_STYLE_1STR_1LINE_LAYOUT1 ||
        style_id == GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1 ||
        style_id == GUIITEM_STYLE_1STR_1LINE_RADIO_LAYOUT1 ||
        style_id == GUIITEM_STYLE_TWO_TEXT_AND_ONE_ICON_MS ||
#ifdef ZTE_WATCH
        style_id == GUIITEM_STYLE_1ICON_1STR_1LINE_WITH_UNREAD_NUM ||
		style_id == GUIITEM_STYLE_1ICON_2STR_1ICON ||
#endif
        style_id == GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM)
    {
        is_need = TRUE;
    }

    return is_need;
}

//============================================================================//
//                                   For watch                                //
//============================================================================//

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author: fangfang.yao
//  Note:画高亮条
//       INNER_STATE_NEED_HIGHBAR_IF_CLICK打开，则绘制；否则不绘制
/*****************************************************************************/
LOCAL void ListSetItemHighBarIfClick( CTRLLIST_OBJ_T *list_ctrl_ptr)
{
    if( list_ctrl_ptr == PNULL)
    {
        SCI_TRACE_LOW("ListSetItemHighBarIfClick list_ctrl_ptr = PNULL");
        return;
    }
    if(LIST_GetInnerState(list_ctrl_ptr, INNER_STATE_NEED_HIGHBAR_IF_CLICK))
    {
        if(!LIST_GetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR))
        {
            LIST_SetListStateByPtr(list_ctrl_ptr, GUILIST_STATE_NEED_HIGHTBAR,TRUE);
            MMK_SendMsg(((CTRLBASE_OBJ_T *)list_ctrl_ptr)->handle, MSG_CTL_PAINT, PNULL);
        }
    }
}

/*****************************************************************************/
//  Description : draw highlight item for slide
//  Global resource dependence :
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawHighLightItem_ForSlide(
                                                            CTRLLIST_OBJ_T *list_ctrl_ptr,
                                                            GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                                                            )
{
    ListDrawItem(list_ctrl_ptr, lcd_dev_info_ptr, list_ctrl_ptr->cur_item_idx);
}

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:设置list支持向左滑动，默认不支持；该函数请搭配 CTRLLIST_SetItemHorMove() 配置使用
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemLeftMove(
                                                    MMI_CTRL_ID_T ctrl_id,
                                                    BOOLEAN       is_true
                                                )
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        SCI_TRACE_LOW("[LIST] CTRLLIST_SetItemLeftMove list_ctrl_ptr is PNULL");
        return;
    }

    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_ABLE_LEFT_SLIDE_ITEM, is_true);
}

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author: fangfang.yao
//  Note: item被点击时，list是否需要高亮条状态，默认情况下不显示高亮条
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemIsNeedHighBarIfClick(
                                                                MMI_CTRL_ID_T ctrl_id,
                                                                BOOLEAN       is_true
                                                            )
{
    CTRLLIST_OBJ_T *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        SCI_TRACE_LOW("[LIST] CTRLLIST_SetItemIsNeedHighBarIfClick list_ctrl_ptr is PNULL");
        return;
    }

    LIST_SetInnerState(list_ctrl_ptr, INNER_STATE_NEED_HIGHBAR_IF_CLICK, is_true);
}


/*****************************************************************************/
//  Description : whether support dec anim when sliding or not
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetIsSupportSlideDecAnim(
                                            MMI_CTRL_ID_T ctrl_id,
                                            BOOLEAN        is_support_slide_dec_anim
                                          )
{
    CTRLLIST_OBJ_T  *list_ctrl_ptr = ListGetPtr(ctrl_id);

    if (PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }

    list_ctrl_ptr->is_support_slide_dec_anim = is_support_slide_dec_anim;

    return TRUE;
}

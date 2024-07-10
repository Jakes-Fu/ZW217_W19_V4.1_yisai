/*****************************************************************************
** File Name:      guiprgbox.c                                               *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2007      Jassmine              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "guiprgbox.h"
#include "guicommon.h"
#include "guires.h"
#include "mmi_theme.h"
#include "guistring.h"
#include "guigraph.h"

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

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set position by pointer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetPosByPtr (
    BOOLEAN      is_update,      //is update
    uint32       cur_item_index,
    uint32       first_item_index,
    IGUICTRL_T   *prgbox_ctrl_ptr
)
{
    CTRLPROGRESSBAR_SetPos(
        is_update,
        cur_item_index,
        first_item_index,
        GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr));
}

/*****************************************************************************/
// Description : this function only for progress bar
// Global resource dependence : none
// Author: allen
// Note: 2005.01.11
/*****************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_GetRectByPtr (
    IGUICTRL_T      *prgbox_ctrl_ptr,
    GUI_RECT_T      *rect
)
{
    return CTRLBASE_GetRect(
        GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr),
        rect);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author: allen
// Note: 2005.02.02
/*****************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_ResizeRectByPtr (
    IGUICTRL_T      *prgbox_ctrl_ptr,
    GUI_RECT_T      new_rect  // the new total rectangle
)
{
    return CTRLPROGRESSBAR_ResizeRect(
        GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr),
        new_rect);
}

/*****************************************************************************/
//  Description : update progress box control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void GUIPRGBOX_UpdateByPtr(
    IGUICTRL_T *prgbox_ctrl_ptr
)
{
    CTRLPROGRESSBAR_Update(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr));

    return;
}

/***************************************************************************//*!
@brief 获取当前滚动条的位置所对应字母的unicode
@author Jassmine
@param      ctrl_id：［IN］滚动条的控件ID
@param      item_pos：滚动条的当前位置
@return 返回item_pos所对应的字母的unicode
@note
*******************************************************************************/
PUBLIC wchar  GUIPRGBOX_GetWcharByPos(
    IGUICTRL_T * prgbox_ctrl_ptr,
    uint16  item_pos
)
{
    return CTRLPROGRESSBAR_GetWcharByPos(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr), item_pos);
}

/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetBothRectByPtr(
    IGUICTRL_T          *prgbox_ctrl_ptr,
    GUI_BOTH_RECT_T     *both_rect_ptr
   )
{
    CTRLPROGRESSBAR_SetBothRect(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr), both_rect_ptr);

    return;
}

/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetParamByPtr(
    IGUICTRL_T          *prgbox_ctrl_ptr,
    uint32              total_item_count,
    uint32              items_per_page
)
{
    CTRLPROGRESSBAR_SetParam(
            GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr),
            total_item_count, items_per_page);

    return;
}

/*****************************************************************************/
//  Description : set bg color
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetBgColorByPtr(
    IGUICTRL_T          *prgbox_ctrl_ptr,
    GUI_COLOR_T         bg_color
)
{
    CTRLPROGRESSBAR_SetBgColor(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr), bg_color);

    return;
}

/*****************************************************************************/
//  Description : 设置是否需要滑块
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:返回设置前的数据
/*****************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_SetNeedSlideBlock(
    IGUICTRL_T          *prgbox_ctrl_ptr,
    BOOLEAN             is_true
)
{
    return CTRLPROGRESSBAR_SetNeedSlideBlock(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr), is_true);
}

/*****************************************************************************/
//  Description : 设置是否需要滑块
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:返回设置前的数据
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetFobidThumbByPtr(
										 IGUICTRL_T *prgbox_ctrl_ptr
										 )
{
    CTRLPROGRESSBAR_SetFobidThumb(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr));

    return;
}

/*****************************************************************************/
//  Description : 设置是否需要滑块
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:返回设置前的数据
/*****************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_SetNeedBg(
    IGUICTRL_T          *prgbox_ctrl_ptr,
    BOOLEAN             is_true
)
{
    return CTRLPROGRESSBAR_SetNeedBg(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr), is_true);
}

/*****************************************************************************/
//  Description : set whether loop scroll
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetLoopScrollByPtr(
    IGUICTRL_T          *prgbox_ctrl_ptr,
    BOOLEAN             is_loop_scroll
)
{
    CTRLPROGRESSBAR_SetLoopScroll(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr), is_loop_scroll);
    
    return;
}

//部分应用邋直接调用内部结构获取参数，需要修改
PUBLIC uint32 GUIPRGBOX_GetTotalItemNum(IGUICTRL_T *   prgbox_ctrl_ptr)
{
    return CTRLPROGRESSBAR_GetTotalItemNum(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr));
}

PUBLIC uint32 GUIPRGBOX_GetFirstItemIndex(IGUICTRL_T *   prgbox_ctrl_ptr)
{
    return CTRLPROGRESSBAR_GetFirstItemIndex(GUICTRL_GetCtrlHandle(prgbox_ctrl_ptr));
}



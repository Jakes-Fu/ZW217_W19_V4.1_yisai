/*****************************************************************************
** File Name:      mmiwidget_page.h                                          *
** Author:                                                                   *
** Date:           08/10/2012                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe   widget page funcion       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/08/2012    ying.xu              Creat
******************************************************************************/

#ifndef _MMIWIDGET_PAGE_H_
#define _MMIWIDGET_PAGE_H_ 


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "graphics_draw.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define GRID_END_FLAG 0xFFFF
#define INVALID_INDEX 0x0000    


/*****************************************************************************/
//  Description : get widget ctrl rect
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIWIDGET_GetWidgetCtrlRect(
                                   MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                   );

/*****************************************************************************/   
//  Description : get win rect on lcd page.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_GetWinRect(MMI_IDLE_GRID_DATA_T *grid_data_ptr,
                                    uint32 widget_id, GUI_RECT_T *win_rect);

/*****************************************************************************/
//  Description : GetShortcutDeleteIconRect
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIWIDGET_IsPointInDeleteIconRect( MMI_IDLE_GRID_DATA_T *idle_grid_ptr,  GUI_POINT_T point);

/*****************************************************************************/
//  Description : GET SHORTCUT RECT
//  Global resource dependence : 
//  Author:YING.XU
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T  MMIWIDGET_GetIconRectFromGridRect(GUI_RECT_T rect);

//删除一页widget页面，调整current_page, refresh page
PUBLIC int32 MMIWIDGET_DeleteWidgetPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_index);

//插入一页widget页面，调整current_page, refresh page
PUBLIC int32 MMIWIDGET_AddWidgetPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_position);

//交换相邻2页widget页面的数据
PUBLIC int32 MMIWIDGET_ExchangeWidgetPageData(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_position1, int32 page_position2);

/*****************************************************************************/
//  Description : get widget page width
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWIDGET_GetWidgetPageWidth(
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                );

/*****************************************************************************/
//  Description : reset all child window rect.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_ReCalcAllWidgetWinRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : reset  child window rect in one page
//  Global resource dependence : 
//  Author:ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_ReCalcWidgetWinRectInOnePage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_index);
//  Description : get win handle by widget id
//  Global resource dependence :
//  Author: hongbo.lan
//  Note: 

/*****************************************************************************/
//  Description : dispatch msg to all child win
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_DispatchMsgToAllChildWin(
                                             MMI_IDLE_GRID_DATA_T  *idle_grid_ptr,
                                             MMI_MESSAGE_ID_E  msg_id
                                             );

PUBLIC MMI_RESULT_E MMIWIDGET_DispatchMsgToActiveChildWin(
                                             MMI_IDLE_GRID_DATA_T  *idle_grid_ptr,
                                             MMI_MESSAGE_ID_E  msg_id
                                             );

PUBLIC MMI_RESULT_E MMIWIDGET_DispatchMsgToChildInOnePage(                                            
                                            MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                            int32             page_idx,
                                            MMI_MESSAGE_ID_E  msg_id,
                                            DPARAM            param
                                            );

/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void CreateMoveLayer(
                           MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                           MMIGRID_ITEM_TYPE_E elem_type
                           );

/*****************************************************************************/
//  Description : destroy widget layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DestroyMoveLayer(
                            MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                            );       

/*****************************************************************************/
//  Description : handle menu tp long down.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HandleIconTpLongDown(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : handle widget tp long down.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HandleWidgetTpLongDown(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

PUBLIC BOOLEAN MoveWidgetIdle2Idle(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : draw rect frame.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DrawRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_RECT_T rect, GUI_COLOR_T color);

/*****************************************************************************/
//  Description : clear showed rect frame buffer.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void ClearRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : draw page layer.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DrawPageLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : draw specific page contents.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DrawSpecificPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_idx);
PUBLIC void DrawSpecificPageEx(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,  GUI_LCD_DEV_INFO lcd_dev_info,
                                int32 page_idx, int32 page_position, int16 offset);
/*****************************************************************************/
//  Description : create  layer
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMIWIDGET_CreateLayer(MMI_WIN_ID_T win_id,GUI_RECT_T *rect_ptr, DISPLAY_DATA_TYPE_E dst_data_type);


PUBLIC BOOLEAN ThumbLayer(
                          GUI_LCD_DEV_INFO *p_src_lcd_dev,
                          GUI_RECT_T *p_src_rect,
                          GUI_LCD_DEV_INFO *p_dst_lcd_dev,
                          GUI_RECT_T *p_thumb_rect,
                          DISPLAY_DATA_TYPE_E dst_data_type,
                          SCALE_IMAGE_OUT_T *p_scale_out
                          );

#ifdef   __cplusplus
    }
#endif
#endif

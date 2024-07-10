/*****************************************************************************
** File Name:      mmiwidget_grid.c                                          *
** Author:                                                                   *
** Date:           08/10/2012                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the layout of               *
**                            elements in page                               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/08/2012     ying.xu              Creat
******************************************************************************/

#ifndef _MMIWIDGET_GRID_H_
#define _MMIWIDGET_GRID_H_ 


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

PUBLIC BOOLEAN MMIWIDGET_LayoutGridTblByRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                  uint16            page,
                                  GUI_RECT_T        rect,
                                  MMI_GRID_CELL_T  *valid_grid_cell_ptr,
                                  GUI_RECT_T       *dst_rc_ptr);


/*****************************************************************************/
//  Description : layout to grid table according to display rect
//              : if no room in current page, find room in next page,
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_LayoutGridTblByRectEx(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                             uint16            page,
                                             GUI_RECT_T        rect,
                                             MMI_GRID_FILLED_INFO_T  *valid_grid_cell_ptr,
                                             GUI_RECT_T       *dst_rc_ptr);
                                  
PUBLIC BOOLEAN MMIWIDGET_CalcFilledGridTblArea(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                    uint16 page,
                                    uint16 grid_index,
                                    uint16 row_num,
                                    uint16 col_num,
                                    MMI_GRID_CELL_T *valid_grid_cell_ptr
                                    );
/*****************************************************************************/
//  Description : filled grid table.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void  MMIWIDGET_FillGridTbl(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                        MMI_GRID_CELL_T *valid_grid_cell_ptr,
                        uint16 page,
                        uint16 shortcut_id,
                        MMIGRID_ITEM_TYPE_E item_type);                         

                        
/*****************************************************************************/
//  Description : get grid tbl index by tp position.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIWIDGET_GetGridIndexByPos(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T pos);

/*****************************************************************************/
//  Description : get firt valid grid in one page
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIWIDGET_GetFirtstValidGrid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page);

/*****************************************************************************/
//  Description : reset grid table item by widget id.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_ResetGridTblByScId(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, uint16  page, int32 reset_id, MMIGRID_ITEM_TYPE_E sc_type);

/*****************************************************************************/
//  Description : 得到点中item
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC MMIGRID_ITEM_TYPE_E MMIWIDGET_GetGridTpPressedItem(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                               GUI_POINT_T point,
                                               uint16 *click_id_ptr
                                               );

                                       
PUBLIC void MMIWIDGET_RemoveWidgetIcon(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page, 
            int32 reset_id, MMIGRID_ITEM_TYPE_E sc_type);

/*****************************************************************************/
//  Description : Check the current filled rect area.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CheckMovingEmptyGrid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_RECT_T *dst_rect_ptr);

/*****************************************************************************/
//  Description : get rect area according to grid cell table.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void GetHightlightItemRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMI_GRID_CELL_T *grid_arr_ptr, GUI_RECT_T *rect_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif

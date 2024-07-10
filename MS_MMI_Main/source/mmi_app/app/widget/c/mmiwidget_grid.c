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


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#include "mmiwidget.h"
#include "mmitheme_widget.h"
#include "mmiwidget_nv.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmiwidget_position.h"
#include "mmiwidget_text.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmiwidget_item.h"
#include "mmiwidget_page.h"
#include "mmiwidget_virtualwin.h"
#include "mmiwidget_wallpaper.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_config.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"
#include "mmiqbtheme_dockbar.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
 //  Description : get grid tbl index by tp position.
 //  Global resource dependence : 
 //  Author: hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC int32 MMIWIDGET_GetGridIndexByPos(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T pos)
 {
     int32 page_grid_num = idle_grid_ptr->grid_col*idle_grid_ptr->grid_row;
     int32 i = 0;    
 
     for(i = 0; i < page_grid_num; i++)
     {
         if(GUI_PointIsInRect(pos, idle_grid_ptr->page_grid_rect[i]) )
         {
             break;
         }
     }
     
     return (i);    
     
 }
 
 /*****************************************************************************/
 //  Description : calc filled grid area by page, grid index, row, col etc.
 //  Global resource dependence : 
 //  Author: hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIWIDGET_CalcFilledGridTblArea(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                                uint16 page,
                                                uint16 grid_index,
                                                uint16 row_num,
                                                uint16 col_num,
                                                MMI_GRID_CELL_T *valid_grid_cell_ptr
                                                )
 {
     uint16 i = 0;
     uint16 j = 0;
     uint16 k = 0;
     int16 page_grid_row = 0;
     int16 page_grid_col = 0;
     uint16 page_grid_num = 0;

     if(PNULL == idle_grid_ptr || PNULL == valid_grid_cell_ptr)
     {
         return FALSE;
     }
	 
     page_grid_row = idle_grid_ptr->grid_row;
     page_grid_col = idle_grid_ptr->grid_col;
     page_grid_num = page_grid_row*page_grid_col;
	 
     if(grid_index % page_grid_col + (col_num - 1) >= page_grid_col)
     {
         return FALSE;
     }
     
     if(grid_index + (row_num - 1) * (page_grid_col - 1) + (col_num - 1)  >= page_grid_num)
     {
         return FALSE;
     }
         
     k = 0;
     
     for(i = 0 ;i < row_num; i++)
     {
         for(j = grid_index + i * page_grid_col; j <= grid_index + i * page_grid_col + col_num - 1; j++)
         {
             if(!IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page][j]))
             {
                 return FALSE; 
             }
             valid_grid_cell_ptr->valid_cell_array[k++] = j;
         }
     }
     
     valid_grid_cell_ptr->valid_cell_array[k] = GRID_END_FLAG;
     return TRUE;
 }
 
 
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
                                    MMIGRID_ITEM_TYPE_E item_type)
 {
     uint16 i =0;
     
     if(PNULL == idle_grid_ptr || PNULL == valid_grid_cell_ptr)
     {
         return;
     }
     
     while(GRID_END_FLAG != valid_grid_cell_ptr->valid_cell_array[i])
     {
         if(MMIGRID_ITEM_TYPE_WIDGET == item_type)
         {
             if (0 == i)
             {
                 SET_NODE_WIDGET(shortcut_id, idle_grid_ptr->grid_data_tab[page][valid_grid_cell_ptr->valid_cell_array[i]]);
             }
             else
             {
                 SET_NODE_WIDGET_AUX(shortcut_id, idle_grid_ptr->grid_data_tab[page][valid_grid_cell_ptr->valid_cell_array[i]]);
             }
         }
         else if (MMIGRID_ITEM_TYPE_ICON == item_type)
         {
             SET_NODE_SHORTCUT(shortcut_id, idle_grid_ptr->grid_data_tab[page][valid_grid_cell_ptr->valid_cell_array[i]]);
         }
         i++;
     }
 }
 
 /*****************************************************************************/
 //  Description : layout to grid table according to display rect.
 //  Global resource dependence : 
 //  Author: hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIWIDGET_LayoutGridTblByRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                              uint16            page,
                                              GUI_RECT_T        rect,
                                              MMI_GRID_CELL_T  *valid_grid_cell_ptr,
                                              GUI_RECT_T       *dst_rc_ptr)
 {
     GUI_POINT_T pt = {0};
     int16 i = 0;
     uint16 search_start = 0;
     uint16 row_num = 0;
     uint16 col_num = 0;
     int16 width = 0;
     int16 height = 0;
     GUI_RECT_T rc = {0};
     uint16 backward_idx = 0;
     uint16 forward_idx  = 0;
     int16 page_row_num = idle_grid_ptr->grid_row;
     int16 page_col_num = idle_grid_ptr->grid_col;
     uint16 page_grid_num = page_row_num*page_col_num;
          
     rc = idle_grid_ptr->display_rect;
 
     width = rect.right - rect.left + 1;
     height = rect.bottom - rect.top + 1;
 
     if(rect.right > rc.right)
     {
         rect.right = rc.right ;
         rect.left = rect.right - width;
     }
     if(rect.bottom > rc.bottom )
     {
         rect.bottom = rc.bottom;
         rect.bottom = rect.bottom - height;
     }
 
     
     pt.x = rect.left;
     pt.y = rect.top;
     
     col_num = width/idle_grid_ptr->grid_width + (((width%idle_grid_ptr->grid_width) == 0)?(0):(1));
     row_num = height/idle_grid_ptr->grid_height + (((height%idle_grid_ptr->grid_height) == 0)?(0):(1));
     
      search_start = MMIWIDGET_GetGridIndexByPos(idle_grid_ptr,pt); 
         
     backward_idx =   (search_start <= row_num * page_row_num) 
                    ? 0 
                    : (search_start - row_num * page_row_num);
 
     
     forward_idx  =   ((search_start + row_num * page_row_num) > page_grid_num) 
                    ? search_start
                    : (search_start + row_num * page_row_num);
    
     /*search backward from the current postion. */
     for(i = search_start; i >= backward_idx; i--)
     {
         if(IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page][i]))
         {
             if(1 == row_num && 1 == col_num)
             {
                 valid_grid_cell_ptr->valid_cell_array[0] = i;
                 valid_grid_cell_ptr->valid_cell_array[1] = GRID_END_FLAG;
                 return TRUE;
             }
             
             if(MMIWIDGET_CalcFilledGridTblArea(idle_grid_ptr,page,i,row_num,col_num,valid_grid_cell_ptr))
             {
                 if (page_grid_num > valid_grid_cell_ptr->valid_cell_array[row_num*col_num - 1])
                 {
                     dst_rc_ptr->left   = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[0]].left;
                     dst_rc_ptr->top    = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[0]].top;
                     dst_rc_ptr->right  = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[row_num*col_num - 1]].right;
                     dst_rc_ptr->bottom = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[row_num*col_num - 1]].bottom;
                     return TRUE;
                 }
             }
         }
     }
 
     /* search forward from the current position. */
     for(i = search_start; i <= forward_idx; i++)
     {
         if(IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page][i]))
         {
             if(row_num == col_num && 1 == col_num)
             {
                 valid_grid_cell_ptr->valid_cell_array[0] = i;
                 valid_grid_cell_ptr->valid_cell_array[1] = GRID_END_FLAG;
                 return TRUE;
             }
             
             if(MMIWIDGET_CalcFilledGridTblArea(idle_grid_ptr,page,i,row_num,col_num,valid_grid_cell_ptr))
             {
                 if (page_grid_num > valid_grid_cell_ptr->valid_cell_array[row_num*col_num - 1])
                 {
                     dst_rc_ptr->left   = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[0]].left;
                     dst_rc_ptr->top    = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[0]].top;
                     dst_rc_ptr->right  = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[row_num*col_num - 1]].right;
                     dst_rc_ptr->bottom = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[row_num*col_num - 1]].bottom;
                     return TRUE;
                 }
             }
         }
     }
 
     /* from zero to search whole gird.*/
     for(i = 0; i<page_grid_num;i++)
     {
         if(IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page][i]))
         {
             if(row_num == col_num && 1 == col_num)
             {
                 valid_grid_cell_ptr->valid_cell_array[0] = i;
                 valid_grid_cell_ptr->valid_cell_array[1] = GRID_END_FLAG;
                 return TRUE;
             }
             
             if(MMIWIDGET_CalcFilledGridTblArea(idle_grid_ptr,page,i,row_num,col_num,valid_grid_cell_ptr))
             {
                 if (page_grid_num>valid_grid_cell_ptr->valid_cell_array[row_num*col_num - 1])
                 {
                     dst_rc_ptr->left = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[0]].left;
                     dst_rc_ptr->top = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[0]].top;
                     dst_rc_ptr->right = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[row_num*col_num - 1]].right;
                     dst_rc_ptr->bottom = idle_grid_ptr->page_grid_rect[valid_grid_cell_ptr->valid_cell_array[row_num*col_num - 1]].bottom;
                     return TRUE;
                 }
             }
         }
     } 
 
     return FALSE;
 }                                      
 
 /*****************************************************************************/
 //  Description : layout to grid table according to display rect
 //              : if no room in current page, find room in next page,
 //  Global resource dependence : 
 //  Author: ying.xu
 //  Note:
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIWIDGET_LayoutGridTblByRectEx(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                              uint16            page,
                                              GUI_RECT_T        rect,
                                              MMI_GRID_FILLED_INFO_T  *valid_grid_cell_ptr,
                                              GUI_RECT_T       *dst_rc_ptr)
 {
     int16 i = 0;
     BOOLEAN result = 0;
     
     for(i = page; i < idle_grid_ptr->page_num; i++)    
     {
         result = MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr, i, rect, 
                     &valid_grid_cell_ptr->filled_grid_cell, dst_rc_ptr);
         if(result)
         {
             valid_grid_cell_ptr->page = i ;
             valid_grid_cell_ptr->is_valid = TRUE;
             return TRUE;
         }
     }
     if(idle_grid_ptr->page_num < MMIQBTHEME_MAX_WIDGET_PAGE_NUM)
     {   
#ifdef QBTHEME_SUPPORT
         MMIQBTHEME_SetWidgetPreviewPageFocusedItem(idle_grid_ptr->page_num - 1);
         MMIQBTHEME_AddWidgetPagePreviewItem();  
#endif         
         result = MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr, i, rect,
                     &valid_grid_cell_ptr->filled_grid_cell, dst_rc_ptr);
         if(result)
         {
             valid_grid_cell_ptr->page = i ;
             valid_grid_cell_ptr->is_valid = TRUE;
             return TRUE;
         }
         
     }
     for(i = 0; i < page ; i++)
     {
         result = MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr, i, rect, 
                 &valid_grid_cell_ptr->filled_grid_cell, dst_rc_ptr);
         if(result)
         {
             valid_grid_cell_ptr->page = i ;
             valid_grid_cell_ptr->is_valid = TRUE;
             return TRUE;
         }
     }
     return FALSE;
 }    
 
 /*****************************************************************************/
 //  Description : get firt valid grid in one page
 //  Global resource dependence : 
 //  Author: hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC int32 MMIWIDGET_GetFirtstValidGrid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page)
 {
     int32 i = 0;
     int32 page_grid_num = idle_grid_ptr->grid_col*idle_grid_ptr->grid_row;
     for(i = 0; i<page_grid_num; i++)
     {
         if(IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page][i]))
         {
             return i;
         }
     }
     
     //MMIQBIDLE_DEBUG:"valid grid index = %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEFUNCTION_652_112_2_18_3_9_0_262,(uint8*)"d",i);
     return -1;
 }
 
 /*****************************************************************************/
 //  Description : reset grid table item by widget id.
 //  Global resource dependence : 
 //  Author: hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC void MMIWIDGET_ResetGridTblByScId(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, uint16  page, int32 reset_id, MMIGRID_ITEM_TYPE_E sc_type)
 {
     int32 i = 0;
     int32 j = 0;
     int32 page_grid_num = (int32)(idle_grid_ptr->grid_col * idle_grid_ptr->grid_row);
          
     MMIWIDGET_DEBUG(LOG_STR("reset_id = %d, sc_type = %d"), reset_id, sc_type);
     if (MMIGRID_ITEM_TYPE_WIDGET == sc_type)
     {
         if (!MMIWIDGET_IsValidWidgetId(reset_id))
         {
             return;
         }
         for(i = 0; i<page_grid_num;i++)
         {
             if(IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[page][i]))
             {
                 if(reset_id == GET_NODE_ID(idle_grid_ptr->grid_data_tab[page][i]))
                 {
                     SET_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page][i]);
                     idle_grid_ptr->old_grid.is_valid = TRUE;                  
                     idle_grid_ptr->old_grid.filled_grid_cell.valid_cell_array[j++] = i; 
                 }
             }
         }
     
         if(idle_grid_ptr->old_grid.is_valid)
         {
             idle_grid_ptr->old_grid.filled_grid_cell.valid_cell_array[j] = GRID_END_FLAG; 
             idle_grid_ptr->old_grid.page = page;             
         }
         if(j < 2)   //只占1个格子的widget也是不对的
     
         {
             MMIWIDGET_DEBUG(LOG_STR("j == %d"), j);
             return;
         }
     }
     else
     if (MMIGRID_ITEM_TYPE_ICON == sc_type)
     {
         if(reset_id >= page_grid_num)
         {
             return;
         }
         if(IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[page][reset_id]))
         {
             MMIWIDGET_DEBUG(LOG_STR("grid_data_tab[page][reset_id] == %d"), 
                 GET_NODE_ID(idle_grid_ptr->grid_data_tab[page][reset_id]));            
             return;
         }
         SET_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page][reset_id]);
         idle_grid_ptr->old_grid.is_valid = TRUE;                  
         idle_grid_ptr->old_grid.filled_grid_cell.valid_cell_array[j++] = reset_id; 
         idle_grid_ptr->old_grid.filled_grid_cell.valid_cell_array[j] = GRID_END_FLAG; 
         idle_grid_ptr->old_grid.page = page;             
     }
 }                              
 
 /*****************************************************************************/
 //  Description : 得到点中item
 //  Global resource dependence : 
 //  Author:hongbo.lan
 //  Note: 
 /*****************************************************************************/
 PUBLIC MMIGRID_ITEM_TYPE_E MMIWIDGET_GetGridTpPressedItem(
                                                           MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                                           GUI_POINT_T point,
                                                           uint16 *click_id_ptr
                                                           )
 {    
     uint16 grid_index = 0;
     uint16 grid_id = 0;
     uint16 widget_id = 0;
 
     MMIGRID_ITEM_TYPE_E click_type = MMIGRID_ITEM_TYPE_INVALID;
 
     if (PNULL == click_id_ptr)
     {
         return MMIGRID_ITEM_TYPE_INVALID;
     }
 
     grid_index = MMIWIDGET_GetGridIndexByPos(idle_grid_ptr, point);
     if(grid_index >= idle_grid_ptr->grid_col*idle_grid_ptr->grid_row)
     {
         click_type = MMIGRID_ITEM_TYPE_INVALID;
         *click_id_ptr = 0;
         MMIWIDGET_DEBUG(LOG_STR("invalid point = %d, %d"), point.x , point.y);
         return click_type;  
     }
     
     grid_id = idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][grid_index];
     if (IS_NODE_WIDGET(grid_id))
     {
         widget_id = GET_NODE_ID(grid_id);
         if (MMIWIDGET_IsValidWidgetId(widget_id))
         {
             click_type = MMIGRID_ITEM_TYPE_WIDGET;
             *click_id_ptr = widget_id;
         }
         else
         {
             click_type = MMIGRID_ITEM_TYPE_INVALID;
             *click_id_ptr = 0;
         }
     }
     else
     if (IS_NODE_ICON(grid_id))
     {
         click_type = MMIGRID_ITEM_TYPE_ICON;
         *click_id_ptr = grid_index;
     }
     else
     {
         click_type = MMIGRID_ITEM_TYPE_INVALID;
     }
     
     return click_type;       
 }
 
 PUBLIC void MMIWIDGET_RemoveWidgetIcon(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page, int32 reset_id, MMIGRID_ITEM_TYPE_E sc_type)
 {       
     int32 page_grid_num = (int32)(idle_grid_ptr->grid_col*idle_grid_ptr->grid_row);
     if (MMIGRID_ITEM_TYPE_WIDGET == sc_type)
     {
         MMIWIDGET_ResetGridTblByScId(idle_grid_ptr, idle_grid_ptr->cur_page_index, reset_id, MMIGRID_ITEM_TYPE_WIDGET);        
         MMIWIDGET_DeactiveWidgetItem(idle_grid_ptr, reset_id);
         MMIWIDGET_InsertWidget2AddingList(idle_grid_ptr, reset_id, 0);  
     }
     else if (MMIGRID_ITEM_TYPE_ICON == sc_type)
     {        
         int32 menu_id = 0;
         if(reset_id >= page_grid_num)
         {
             //MMIQBIDLE_DEBUG:"reset_id = %d"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEFUNCTION_799_112_2_18_3_9_0_265,(uint8*)"d",reset_id);
             return  ;
         }
         menu_id = GET_NODE_ID(idle_grid_ptr->grid_data_tab[page][reset_id]);
         MMIWIDGET_ResetGridTblByScId(idle_grid_ptr, idle_grid_ptr->cur_page_index, reset_id, MMIGRID_ITEM_TYPE_ICON);
         MMIWIDGET_RemoveShortcutInAddingList(idle_grid_ptr,menu_id);
     }
 }

 
 /*****************************************************************************/
 //  Description : Check the current filled rect area.
 //  Global resource dependence : 
 //  Author: hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC BOOLEAN CheckMovingEmptyGrid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_RECT_T *dst_rect_ptr)
 {
     MMI_GRID_CELL_T filled_cell = {0};
     UILAYER_INFO_T layer_info = {0};
     int16 i = 0;
     GUI_RECT_T new_rect = {0};
     uint16 row_num = 0;
     uint16 col_num = 0;
     int16 width = 0;
     int16 height = 0;
     GUI_POINT_T center_pt = {0};
     GUI_POINT_T top_pt = {0};
     int16 offset_x = 0;
     int16 offset_y = 0;
     uint16 search_start = 0;
     uint16 page = idle_grid_ptr->cur_page_index;
     uint32 page_grid_num = idle_grid_ptr->grid_col*idle_grid_ptr->grid_row;
     
     if (!UILAYER_IsLayerActive(&idle_grid_ptr->move_layer))
     {
         return FALSE;
     }
 
     if(page >= MMI_GRID_PAGE_NUM)
     {
         return FALSE;
     }
 
     UILAYER_GetLayerPosition(&idle_grid_ptr->move_layer, &offset_x, &offset_y);
     UILAYER_GetLayerInfo(&idle_grid_ptr->move_layer, &layer_info);
 
     center_pt.x = offset_x + layer_info.layer_width/2;
     center_pt.y = offset_y + layer_info.layer_height/2;
 
     if (idle_grid_ptr->showed_rect_info.is_valid && !GUI_PointIsInRect(center_pt, idle_grid_ptr->showed_rect_info.old_rect))
     {
         ClearRect(idle_grid_ptr);
         idle_grid_ptr->showed_rect_info.is_valid = FALSE;
     }
 
     if (idle_grid_ptr->is_item_upon_recycle)
     {
         return FALSE;
     }
 
     width = layer_info.layer_width;
     height = layer_info.layer_height;
     
     col_num = width/idle_grid_ptr->grid_width + (((width%idle_grid_ptr->grid_width) == 0)?(0):(1));
     row_num = height/idle_grid_ptr->grid_height + (((height%idle_grid_ptr->grid_height) == 0)?(0):(1));
 
 
     top_pt.x = offset_x;
     top_pt.y = offset_y;
     
     search_start = MMIWIDGET_GetGridIndexByPos(idle_grid_ptr, top_pt); 
     
     if(search_start < page_grid_num)
     {
     if(IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page][search_start]))
     {
         if(1 == row_num && 1 == col_num)
         {
             filled_cell.valid_cell_array[0] = i;
             filled_cell.valid_cell_array[1] = GRID_END_FLAG;
             
             new_rect.left   = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].left;
             new_rect.top    = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].top;
             new_rect.right  = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].right;
             new_rect.bottom = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].bottom;
 
             if (GUI_PointIsInRect(center_pt, new_rect))
             {
                 *dst_rect_ptr = new_rect;
                 return TRUE;
             }  
         }
         
         if(MMIWIDGET_CalcFilledGridTblArea(idle_grid_ptr,page,search_start,row_num,col_num, &filled_cell))
         {
             new_rect.left   = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].left;
             new_rect.top    = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].top;
             new_rect.right  = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[row_num*col_num - 1]].right;
             new_rect.bottom = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[row_num*col_num - 1]].bottom;
 
             if (GUI_PointIsInRect(center_pt, new_rect))
             {
                 *dst_rect_ptr = new_rect;
                 return TRUE;
             }
         }
     }
     }
     /* from zero to search whole gird.*/
     for(i = 0; i < (int16)page_grid_num; i++)
     {
         if(IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page][i]))
         {
             if(row_num == col_num && 1 == col_num)
             {
                 filled_cell.valid_cell_array[0] = i;
                 filled_cell.valid_cell_array[1] = GRID_END_FLAG;
 
                 new_rect.left   = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].left;
                 new_rect.top    = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].top;
                 new_rect.right  = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].right;
                 new_rect.bottom = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].bottom;
 
                 if (GUI_PointIsInRect(center_pt, new_rect))
                 {
                     *dst_rect_ptr = new_rect;
                     return TRUE;
                 }
             }
             else
             if(MMIWIDGET_CalcFilledGridTblArea(idle_grid_ptr,page,i,row_num,col_num, &filled_cell))
             {
                 new_rect.left = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].left;
                 new_rect.top = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[0]].top;
                 new_rect.right = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[row_num*col_num - 1]].right;
                 new_rect.bottom = idle_grid_ptr->page_grid_rect[filled_cell.valid_cell_array[row_num*col_num - 1]].bottom;
 
                 if (GUI_PointIsInRect(center_pt, new_rect))
                 {
                     *dst_rect_ptr = new_rect;
                     return TRUE;
                 }
             }
         }
     } 
 
     return FALSE;
 }


 /*****************************************************************************/
 //  Description : get rect area according to grid cell table.
 //  Global resource dependence : 
 //  Author: hongbo.lan
 //  Note: 
 /*****************************************************************************/
 PUBLIC void GetHightlightItemRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMI_GRID_CELL_T *grid_arr_ptr, GUI_RECT_T *rect_ptr)
 {
     uint32 i = 1;
     uint16 begin_idx = 0;
     uint16 end_idx = 0;
     
     while (GRID_END_FLAG != grid_arr_ptr->valid_cell_array[i])
     {
         i++;
     }
     
     begin_idx = grid_arr_ptr->valid_cell_array[0];
     end_idx   = grid_arr_ptr->valid_cell_array[i - 1];
     
     *rect_ptr = idle_grid_ptr->page_grid_rect[begin_idx];
     
     rect_ptr->right  = idle_grid_ptr->page_grid_rect[end_idx].right;
     rect_ptr->bottom = idle_grid_ptr->page_grid_rect[end_idx].bottom;
 }


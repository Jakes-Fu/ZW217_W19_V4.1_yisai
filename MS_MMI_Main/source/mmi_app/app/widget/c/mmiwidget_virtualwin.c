/*****************************************************************************
** File Name:      mmiwidget_virtualwin.c                                    *
** Author:                                                                   *
** Date:           08/10/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe widget virtual window       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/08/2012     ying.xu              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmidisplay_data.h"

#include "mmi_theme.h"
#include "mmiwidget.h"
#include "mmi_nv.h"

#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmiset_wintab.h"
#include "Mmiwidget_text.h"
#include "mmiwidget_export.h"
#include "mmiwidget_image.h"
#include "mmi_mainmenu_export.h"
#include "mmi_appmsg.h"
#include "mmi_menutable.h"

#include "mmiwidget_item.h"
#include "mmiwidget_grid.h"
#include "mmiwidget_page.h"
#include "mmiwidget_virtualwin.h"
#include "mmiwidget_wallpaper.h"
/*
#include "mmiqbtheme_config.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"
#include "mmiqbtheme_dockbar.h"
#include "mmiqbtheme_idlewin.h"
#include "mmiqbtheme_idleexport.h"
*/
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWIDGET_TP_MOVE_TIMEOUT                   10

/*****************************************************************************/
//  Description : Init Shortcuts Position
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_InitShortcutsPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,MMIGRID_SHOTRCUT_PRELOAD_POSITION_T shortcut_init_info)
{
    MMI_GRID_CELL_T valid_cell = {0};
    int16 index = 0;
    int16 page_grid_col = 0;    
    int16 grid_index = 0;
	
    if(PNULL == idle_grid_ptr )
     {
         return ;
     }
    page_grid_col = idle_grid_ptr->grid_col;  	
    grid_index = shortcut_init_info.row_index * page_grid_col + shortcut_init_info.col_index;

    if(idle_grid_ptr)
    {
        if(MMIWIDGET_CalcFilledGridTblArea(idle_grid_ptr,shortcut_init_info.preload_page,grid_index,1,1,&valid_cell))
        {
            index = MMIWIDGET_GetShortCutIdxbyMenuid(idle_grid_ptr,shortcut_init_info.menu_id);
            MMIWIDGET_FillGridTbl(idle_grid_ptr,&valid_cell,shortcut_init_info.preload_page,index,MMIGRID_ITEM_TYPE_ICON);
            idle_grid_ptr->icon_ptr[index].icon_info = idle_grid_ptr->grid_menu_info.grid_menu[index];
            idle_grid_ptr->icon_ptr[index].icon_num ++;
        }
    }
}      

/*****************************************************************************/
//  Description : Init Widgets Position
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void	MMIWIDGET_InitWidgetsPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, 
                                        MMIWIDGET_ITEM_INFO_T *item_ptr, 
                                        MMIGRID_WIDGET_PRELOAD_POSITION_T grid_idle_position)
{
    uint16 width  = 0;
    uint16 height = 0;
    MMI_GRID_CELL_T valid_grid = {0};
    uint16 row_num = 0;
    uint16 col_num = 0;
    uint16 i = 0;
    uint16 j = 0;
    uint16 cell_arrey_index = 0;
    MMIGRID_WIDGET_PRELOAD_POSITION_T init_info = grid_idle_position;
    int16 page_grid_row = idle_grid_ptr->grid_row;
    int16 page_grid_col = idle_grid_ptr->grid_col;
    MMI_IMAGE_ID_T  bg_img = 0;

    MMIWIDGET_GetWidgetImage(item_ptr, &bg_img, PNULL);
    
    GUIRES_GetImgWidthHeight(&width, &height, bg_img, idle_grid_ptr->win_id);

	//image's row and col num
	col_num= width/idle_grid_ptr->grid_width + (((width%idle_grid_ptr->grid_width) == 0)?(0):(1));
	row_num = height/idle_grid_ptr->grid_height + (((height%idle_grid_ptr->grid_height) == 0)?(0):(1));
    if(row_num*col_num == 0)
    {
        return;
    }
    if(row_num > page_grid_row)
    {
        row_num = page_grid_row;
    }
	for(i = 0; i < row_num; i++)
	{
	    for (j = 0; j < col_num; j++)
		{
            valid_grid.valid_cell_array[cell_arrey_index] = (init_info.row_index + i)* page_grid_col 
                + init_info.col_index + j;
			cell_arrey_index++;
		}
	}
    valid_grid.valid_cell_array[cell_arrey_index] = GRID_END_FLAG;

    MMIWIDGET_FillGridTbl(idle_grid_ptr, &valid_grid, grid_idle_position.preload_page, grid_idle_position.widget_id, MMIGRID_ITEM_TYPE_WIDGET);
}

/*****************************************************************************/
//  Description : Add the item to widget list
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_AppendWidgetItem(
                                      MMI_IDLE_GRID_DATA_T*   idle_grid_ptr,
                                      const MMIWIDGET_ITEM_T* add_item_ptr
                                      )
{
    BOOLEAN             result        = FALSE;

    if (   (PNULL != idle_grid_ptr) && (PNULL != idle_grid_ptr->item_ptr) 
        && (PNULL != add_item_ptr) && MMIWIDGET_IsValidWidgetId(add_item_ptr->item_info.widget_id))
    {
        if (idle_grid_ptr->total_item_num < idle_grid_ptr->max_item_num)
        {      

            idle_grid_ptr->item_ptr[add_item_ptr->item_info.widget_id - 1] = *add_item_ptr;
            idle_grid_ptr->total_item_num++;  
            MMIWIDGET_CreateWidgetItem(idle_grid_ptr, idle_grid_ptr->win_id, 
                            &idle_grid_ptr->item_ptr[add_item_ptr->item_info.widget_id - 1],
                            add_item_ptr->item_info.HandleWidgetWinMsg);
            //HandleChildWidgetWinMsg);
            
            if (add_item_ptr->is_active)
            {
                result = MMIWIDGET_ActiveWidgetItem( idle_grid_ptr, add_item_ptr->item_info.widget_id);
            }
            else
            {
                result  =  MMIWIDGET_InsertWidget2AddingList(
                                         idle_grid_ptr,
                                         add_item_ptr->item_info.widget_id, 
                                         idle_grid_ptr->shortcut.shortcut_num
                                         );
            }
           
            MMIWIDGET_ResetOneWidgetLayer(idle_grid_ptr, add_item_ptr->item_info.widget_id);
        }
    }

	return result;
}

/*****************************************************************************/
//  Description : find widget item info
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMIWIDGET_ITEM_INFO_T * FindWidgetItemInfo(
                                          uint32 widget_id,
                                            MMIWIDGET_ITEM_INFO_T *widget_table_ptr[],
                                            int32 widget_num                         
                                          )
{
    int32 i = 0;

    for (i = 0; i < widget_num; i++)
    {
        if (widget_id == widget_table_ptr[i]->widget_id)
        {
            return widget_table_ptr[i]; /*lint !e605*/           
        }
    }
  
    return PNULL;
}

/*****************************************************************************/
//  Description : load all widget to control
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_LoadAllWidget(
                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                         MMIWIDGET_ITEM_INFO_T *widget_table_ptr[],
                         int32 widget_num                         
                         )
{
    MMIWIDGET_ITEM_T widget_item = {0};    
    int32 i = 0;
    uint32 widget_id = MMIWIDGET_INVALID_ID;
    MMIWIDGET_ITEM_INFO_T *item_ptr = PNULL;   
    
    for (i = 0; i <  idle_grid_ptr->shortcut.shortcut_num; i++) /*lint !e574 !e737 */
    {
        widget_id = idle_grid_ptr->shortcut.shortcut_item[i].widget_id;        
        item_ptr = FindWidgetItemInfo(widget_id, widget_table_ptr, widget_num);
        if (PNULL != item_ptr)
        {
            widget_item.item_info = *item_ptr;
            widget_item.is_active = FALSE;         
            MMIWIDGET_AppendWidgetItem(idle_grid_ptr, &widget_item);
        }
    }

    for (i = 0; i < widget_num; i++)
    {   
        if(0 == widget_table_ptr[i])
        {
            break;
        }
        widget_id = widget_table_ptr[i]->widget_id;
        
        if (MMIWIDGET_IsWidgetExistInAddingList(idle_grid_ptr, widget_id))
        {
            continue;
        }

        item_ptr = FindWidgetItemInfo(widget_id, widget_table_ptr, widget_num);
           
        if (PNULL != item_ptr)
        {
            widget_item.item_info = *item_ptr;
            widget_item.is_active = TRUE;          
            MMIWIDGET_AppendWidgetItem(idle_grid_ptr, &widget_item);
        }
        
    }
    
    //MMIWIDGET_DispatchMsgToAllChildWin(idle_grid_ptr, MSG_GET_FOCUS);

}

/*****************************************************************************/
//  Description : un load all widget to control
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_UnLoadAllWidget(
                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                         )
{
    uint32 widget_id = MMIWIDGET_INVALID_ID;

    for (widget_id = 0; widget_id <  MMIWIDGET_WIDGET_MAX; widget_id++) /*lint !e574 !e737 */
    {
        if (MMIWIDGET_IsWidgetExistInAddingList(idle_grid_ptr, widget_id))
        {
            continue;
        }
           
       MMIWIDGET_DeactiveWidgetItem(idle_grid_ptr, widget_id);
       MMIWIDGET_InsertWidget2AddingList(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id, 0);  
    }
}

/*****************************************************************************/
//  Description : MMIGRID_getpagelayersize
//  Global resource dependence : 
//  Author:ying.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWIDGET_GetPageLayerSize(MMI_IDLE_GRID_DATA_T * idle_grid_ptr)
{
    return PAGE_LAYER_SIZE;
}

/*****************************************************************************/
//  Description : MMIGRID_get wallpaper layer size
//  Global resource dependence : 
//  Author:ying.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWIDGET_GetWallpaperLayerSize(MMI_IDLE_GRID_DATA_T * idle_grid_ptr)
{
    return WALLPAPER_LAYER_SIZE;
}

/*****************************************************************************/
//  Description : remove shortcut from idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_RemoveShortcutInAddingList(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 id)
{
    idle_grid_ptr->icon_ptr[id].icon_num--;
    
    if (0 == idle_grid_ptr->icon_ptr[id].icon_num)
    {
        idle_grid_ptr->icon_ptr[id].icon_info.group_id = 0; 
        idle_grid_ptr->icon_ptr[id].icon_info.menu_id  = 0;
        idle_grid_ptr->icon_ptr[id].icon_num = 0;       
    }
    
    return TRUE;
}


/*****************************************************************************/
//  Description : get shortcut idx.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWIDGET_GetShortCutIdx(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,MMIMAINMENU_ITEM_INFO_T item_info)
{
    uint16 i = 0;
    
    for(i = 0;i < MMI_IDLE_SC_ITEM_MAX; i++)
    {
        if(idle_grid_ptr->icon_ptr[i].icon_info.group_id == item_info.group_id &&
            idle_grid_ptr->icon_ptr[i].icon_info.menu_id== item_info.menu_id)
        {
            return i;
        }
    }
    
    return ~0;//not found
}

/*****************************************************************************/
//  Description : ShortCut GetIndex 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWIDGET_GetShortCutIdxbyMenuid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,MMI_MENU_ID_T menu_id)
{
    uint16 i = 0;
    
    for(i = 0; i < MMIMENU_MAIN_MAX_NUM; i++)
    {
        if(idle_grid_ptr->grid_menu_info.grid_menu[i].menu_id == menu_id)
        {
            return i;
        }
    }
    
    return INVALID_INDEX;
    
}

/*****************************************************************************/
//  Description : check the widget id to add is exist in shorcut or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_IsWidgetExistInAddingList(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, uint32 widget_id)
{
    uint32 i = 0;
    
    while (i < idle_grid_ptr->shortcut.shortcut_num)
    {
        if (idle_grid_ptr->shortcut.shortcut_item[i].widget_id == widget_id)
        {
            return TRUE;
        }
        i++;
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : insert shortcut
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_InsertWidget2AddingList(
                                      MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                      uint32 widget_id,
                                      uint32 pos
                                      )
{
    int32 i = 0;
        
    if(widget_id >= MMIWIDGET_WIDGET_MAX)
    {
        return  FALSE;
    }
    
    while (i < idle_grid_ptr->shortcut.shortcut_num)    /*lint !e574 !e737 */
    {
        if (idle_grid_ptr->shortcut.shortcut_item[i].widget_id == widget_id)
        {
            return FALSE;
        }
        i++;
    }
    
    for (i = idle_grid_ptr->shortcut.shortcut_num; i > pos; i--)     /*lint !e574 !e737 */
    {
        idle_grid_ptr->shortcut.shortcut_item[i] = idle_grid_ptr->shortcut.shortcut_item[i-1];
    }
    idle_grid_ptr->shortcut.shortcut_item[pos].widget_id = widget_id;
    idle_grid_ptr->shortcut.shortcut_num++;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : remove shortcut item
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_RemoveWidgetInAddingList(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 uint32 widget_id
                                 )
{
    uint32 i = 0;
    BOOLEAN is_find = FALSE;

    if (idle_grid_ptr->shortcut.shortcut_num > 0)
    {
        if (widget_id == idle_grid_ptr->shortcut.shortcut_item[idle_grid_ptr->shortcut.shortcut_num - 1].widget_id)
        {
            idle_grid_ptr->shortcut.shortcut_item[idle_grid_ptr->shortcut.shortcut_num - 1].widget_id = 0;
            idle_grid_ptr->shortcut.shortcut_num--;
            return TRUE;
        }
        
        for (i = 0; i < idle_grid_ptr->shortcut.shortcut_num; i++)
        {
            if (widget_id == idle_grid_ptr->shortcut.shortcut_item[i].widget_id)
            {
                is_find = TRUE;
                break;
            }
        }
        
        if (is_find)
        {
            for (; i < idle_grid_ptr->shortcut.shortcut_num; i++)
            {
                idle_grid_ptr->shortcut.shortcut_item[i] = idle_grid_ptr->shortcut.shortcut_item[i+1];
            }
            idle_grid_ptr->shortcut.shortcut_item[i].widget_id = 0;
            idle_grid_ptr->shortcut.shortcut_num--;
            return TRUE;
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : create tp move timer
//  Global resource dependence : 
//  Author: hongbo.lan 
//  Note:
/*****************************************************************************/
PUBLIC void CreateTpMoveTimer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr , MMI_HANDLE_T win_id)
{
    if(idle_grid_ptr->slide_anim.timer_id == 0)
    {
	    idle_grid_ptr->slide_anim.timer_id = MMK_CreateWinTimer(win_id, MMIWIDGET_TP_MOVE_TIMEOUT, FALSE);
	}
}

/*****************************************************************************/
//  Description : stop tp move timer
//  Global resource dependence : 
//  Author: hongbo.lan 
//  Note:
/*****************************************************************************/
PUBLIC void StopTpMoveTimer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if (0 != idle_grid_ptr->slide_anim.timer_id)
    {
        MMK_StopTimer(idle_grid_ptr->slide_anim.timer_id);
        idle_grid_ptr->slide_anim.timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : restart tp move timer
//  Global resource dependence : 
//  Author: hongbo.lan 
//  Note:
/*****************************************************************************/
PUBLIC void RestartTpMoveTimer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMI_HANDLE_T win_id)
{
    StopTpMoveTimer(idle_grid_ptr);
    CreateTpMoveTimer(idle_grid_ptr, win_id);
}

/*****************************************************************************/
//  Description : 设置pagelayer位置（RUNTIME_DRAW的情况记录全局变量，直接画）
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void SetPageLayerPosition(
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr, 
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                int16 offset_x,
                                int16 offset_y
                                )
{
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);    
    int16   width = MMIWIDGET_GetWidgetPageWidth(idle_grid_ptr);
    GUI_RECT_T display_rect = idle_grid_ptr->display_rect;    
    int32  page_offset = 0;
    
    idle_grid_ptr->page_layer_offset.x = offset_x;
    idle_grid_ptr->page_layer_offset.y = offset_y;

    if(1 == page_layer_size)
    {            
        if(idle_grid_ptr->page_layer_offset.x != 0)
        {
            //改变page_layer的位置
            UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);
            UILAYER_SwitchDoubleMem( &idle_grid_ptr->page_layer, TRUE );
        }        
        UILAYER_ClearRect(&idle_grid_ptr->page_layer, display_rect);
        
        DrawSpecificPageEx(idle_grid_ptr, idle_grid_ptr->page_layer, 
            idle_grid_ptr->cur_page_index, 0, offset_x);   
            
        if(idle_grid_ptr->page_layer_offset.x == 0)
        {
            return;
        }
        else if(idle_grid_ptr->page_layer_offset.x < 0)
        {
            page_offset = 1;
        }
        else
        {
            page_offset = -1;
        }
        DrawSpecificPageEx(idle_grid_ptr, idle_grid_ptr->page_layer, 
                                        (idle_grid_ptr->cur_page_index + page_offset
                                            + idle_grid_ptr->page_num)%idle_grid_ptr->page_num, 
                                        page_offset, offset_x);   
            
            
    }
    else  if(3 <= page_layer_size)
    {
        UILAYER_SetLayerPosition(dev_info_ptr, offset_x, offset_y);  
        //right
        UILAYER_SetLayerPosition( &idle_grid_ptr->page_cache[1], offset_x + width, offset_y);          /*lint !e415 !e416 !e661 !e662 !e831 */
        //left
        UILAYER_SetLayerPosition( &idle_grid_ptr->page_cache[(page_layer_size - 1)% page_layer_size], offset_x - width, offset_y);          /*lint !e415 !e416 !e661 !e662 !e831 */
    }
}

/*****************************************************************************/
//  Description : 得到pagelayer位置（RUNTIME_DRAW的情况读取全局变量）
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void GetPageLayerPosition(  
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr ,
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                int16 *offset_x_ptr, //output
                                int16 *offset_y_ptr //output
                                )
{
    
        if (PNULL != offset_x_ptr)
        {
            *offset_x_ptr = idle_grid_ptr->page_layer_offset.x;
        }
        if (PNULL != offset_y_ptr)
        {
            idle_grid_ptr->page_layer_offset.y = idle_grid_ptr->display_rect.top;
            *offset_y_ptr = idle_grid_ptr->page_layer_offset.y;
        }
}

/*****************************************************************************/
//  Description : reset widget page layer pos
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void ResetGridPageLayerPos(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                 )
{
    int16               offset_x = 0;
    int16               offset_y = 0;

    if (UILAYER_IsLayerActive(&idle_grid_ptr->page_layer))
    {
        GetPageLayerPosition( idle_grid_ptr,&idle_grid_ptr->page_layer, &offset_x, &offset_y);
        offset_x = 0;    
        SetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, offset_x, offset_y);
        offset_x = -idle_grid_ptr->cur_page_index * MMIWIDGET_GetWidgetPageWidth(idle_grid_ptr);
        SetWallpaperPosition(idle_grid_ptr, offset_x, offset_y, FALSE);
    }
}

/*****************************************************************************/
//  Description : reset widget page layer cahce
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC int32 SetGridPageLayerCacheInvalid(
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr, uint32 index
                                )
 {    
        uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);
        if(index >= page_layer_size)
        {
            return -1;
        }
        idle_grid_ptr->page_cache_flag[index] = 0;        
        return 0;
 }  

/*****************************************************************************/
//  Description : reset widget page layer cahce
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC int32 UpdateGridPageLayerCache(
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                )
 {
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);
    uint32     i = 0;
    int32       j = 0;

    GUI_LCD_DEV_INFO temp_lcd_dev = {0};
        
    if(1 == page_layer_size)
    {
        return -1;
    }
    else if(3 <= page_layer_size)
    {   
        if(idle_grid_ptr->slide_anim.dst_page_index == idle_grid_ptr->cur_page_index)
        {
            return 0;
        }

        if(MMIWIDGET_PEN_MOVE_LEFT == idle_grid_ptr->slide_anim.move_direct)
        {
            j = -1;            
            temp_lcd_dev = idle_grid_ptr->page_cache[ (page_layer_size + (uint32)j) % page_layer_size]; /*lint !e415 !e416 !e661 !e662 !e796 !e831 */
        
            for(i = 0 ; i < page_layer_size - 1; i++)
            {
                idle_grid_ptr->page_cache[(page_layer_size + (uint32)j) % page_layer_size] 
                        = idle_grid_ptr->page_cache[(page_layer_size + (uint32)j + 1) % page_layer_size];   /*lint !e415 !e416 !e661 !e662  !e796 !e831 */
                j++;
    }
            idle_grid_ptr->page_cache[(page_layer_size + (uint32)j) % page_layer_size] = temp_lcd_dev;  /*lint !e415 !e416 !e661 !e662  !e796 !e831 */
            idle_grid_ptr->page_cache_flag[(page_layer_size + (uint32)j) % page_layer_size] = 0;    /*lint !e415 !e416 !e661 !e662 !e796  !e831 */
        }
        else
        {
            j = 1;            
            temp_lcd_dev = idle_grid_ptr->page_cache[ (page_layer_size + (uint32)j) % page_layer_size]; /*lint !e415 !e416 !e661 !e662  !e796 !e831 */
            
            for(i = 0 ; i < page_layer_size - 1; i++)
            {
                idle_grid_ptr->page_cache[(page_layer_size + (uint32)j) % page_layer_size] 
                        = idle_grid_ptr->page_cache[(page_layer_size + (uint32)j - 1) % page_layer_size];   /*lint !e415 !e416 !e661 !e662  !e796 !e831 */
                j--;
            }
            idle_grid_ptr->page_cache[(page_layer_size + (uint32)j) % page_layer_size] = temp_lcd_dev;  /*lint !e415 !e416 !e661 !e662 !e796  !e831 */
            idle_grid_ptr->page_cache_flag[(page_layer_size + (uint32)j) % page_layer_size] = 0;    /*lint !e415 !e416 !e661 !e662  !e796 !e831 */
        }             
        //idle_grid_ptr->page_cache_flag[0] = 0;  //redraw current page
    }
    return 0;
 }                               


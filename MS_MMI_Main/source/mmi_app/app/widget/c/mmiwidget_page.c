/*****************************************************************************
** File Name:      mmiwidget_page.c                                          *
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


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmi_app_widget_trc.h"
#include "graphics_draw.h"
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

#include "mmiwidget_internal.h"
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
#include "mmiwidget_page.h"
#include "mmiqbtheme_idlewin.h"
#include "mmiqbtheme_idleexport.h"
*/
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : DrawWidgetDeleteIcon
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
LOCAL void DrawDeleteIcon(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
            GUI_RECT_T *rect_ptr, GUI_LCD_DEV_INFO *dev_info_ptr)
{    
    GUIRES_DisplayImg(PNULL,
        rect_ptr,
        PNULL,
        idle_grid_ptr->win_id,
        idle_grid_ptr->delect_icon_id, 
        dev_info_ptr);
}

/*****************************************************************************/
//  Description : fill specific page layer mask color.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void FillSpecificPageWithMaskColor(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, 
                                          GUI_LCD_DEV_INFO  *lcd_dev_info,  uint16 page_idx,
                                          int32 page_position, int16 offset)

{
    uint32 grid_idx = 0;
    uint16 page_width = 0;
    GUI_RECT_T rect = {0};
    uint32 page_grid_num = idle_grid_ptr->grid_col*idle_grid_ptr->grid_row;    
    uint32 color = RGB5652ARGB8888(MMI_BLACK_COLOR, 255);
    
    page_width = MMIWIDGET_GetWidgetPageWidth(idle_grid_ptr);

    for (grid_idx = 0; grid_idx < page_grid_num; grid_idx++)
    {
        if (!IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page_idx][grid_idx]))
        {
            rect = idle_grid_ptr->page_grid_rect[grid_idx];
            rect.left += page_width * page_position;   
            rect.right += page_width * page_position;
            rect.left  += offset;
            rect.right += offset;
            FillLayerWithMaskColor(lcd_dev_info, &rect, color);/*lint !e648 !e572 !e778*/
        }
    }    
}

/*****************************************************************************/   
//  Description : get win rect on lcd page.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_GetWinRect(MMI_IDLE_GRID_DATA_T *grid_data_ptr,
                                    uint32 widget_id, GUI_RECT_T *win_rect)
{
    int32 i = 0;
    int32 j = 0;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    uint16 img_width  = 0;
    uint16 img_height = 0;
    uint16 col_num    = 0;
    uint16 row_num    = 0;
    GUI_RECT_T widget_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;    
    MMI_IMAGE_ID_T    bg_img = 0;
    
    int16 page_grid_row = grid_data_ptr->grid_row;
    int16 page_grid_col = grid_data_ptr->grid_col;
    int32 page_grid_num = page_grid_row*page_grid_col;
    
    if (!MMIWIDGET_IsValidWidgetId(widget_id))
    {
        return FALSE;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    for (i = 0; i < grid_data_ptr->page_num; i++)
    {
        for (j = 0; j < page_grid_num; j++)
        {
            if (   IS_NODE_WIDGET(grid_data_ptr->grid_data_tab[i][j]) 
                && !IS_NODE_AUX(grid_data_ptr->grid_data_tab[i][j])
                && widget_id == GET_NODE_ID(grid_data_ptr->grid_data_tab[i][j]))
            {                

                item_ptr = &grid_data_ptr->item_ptr[widget_id - 1];
#if defined MMIWIDGET_SPECIAL_EFFECT
                if(MMIWIDGET_WEATHER_ID == widget_id 
                    || MMIWIDGET_ACLOCK_ID == widget_id
                    || MMIWIDGET_MP3_ID == widget_id
                    )
                {
                    widget_rect = MMITHEME_GetFullScreenRect();   
                    widget_rect.left += (i - grid_data_ptr->cur_page_index) * lcd_width;
                    widget_rect.right += (i - grid_data_ptr->cur_page_index) * lcd_width;
                    *win_rect = widget_rect;
                    return TRUE;
                }
#endif
                MMIWIDGET_GetWidgetImage(&item_ptr->item_info, &bg_img, PNULL);
               
                GUIRES_GetImgWidthHeight(&img_width, &img_height, bg_img, grid_data_ptr->win_id);                
                if(img_width >= lcd_width)
                {
                    img_width = lcd_width - 2 ;
                }
                col_num = img_width/grid_data_ptr->grid_width + 1;
                //col_num = img_width/grid_data_ptr->grid_width + (((img_width%grid_data_ptr->grid_width) == 0)?(0):(1));
                row_num = img_height/grid_data_ptr->grid_height + (((img_height%grid_data_ptr->grid_height) == 0)?(0):(1)); 
                
                if(row_num > grid_data_ptr->grid_row)
                {
                    row_num = grid_data_ptr->grid_row;
                }
                widget_rect.left   = grid_data_ptr->page_grid_rect[j].left;
                widget_rect.right  = grid_data_ptr->page_grid_rect[j + col_num - 1].right;
                widget_rect.top    = grid_data_ptr->page_grid_rect[j].top;
                widget_rect.bottom = grid_data_ptr->page_grid_rect[j + (row_num - 1) * page_grid_col + col_num - 1].bottom;
#ifdef MMI_SAM_THEME
                if( MMISET_IDLE_SUBSTYLE_SAM != MMIAPISET_GetIdleSubStyle())
#endif     
                {
                    widget_rect.top    = (widget_rect.top + widget_rect.bottom- img_height)/2 + 1;                
                }
                
                widget_rect.left   = (widget_rect.left + widget_rect.right - img_width)/2 + 1;
                widget_rect.left  += (int16)((i - grid_data_ptr->cur_page_index) * lcd_width);
                widget_rect.right  = widget_rect.left + img_width - 1;
                widget_rect.bottom = widget_rect.top + img_height - 1;
                
                win_rect->left   = widget_rect.left;   
                win_rect->top    = widget_rect.top;
                win_rect->right  = widget_rect.right;
                win_rect->bottom = widget_rect.bottom; 
                
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: ying.xu
//  Note:   插入一页widget页面，调整current_page, refresh page
/*****************************************************************************/
PUBLIC int32 MMIWIDGET_AddWidgetPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_position)
{
    uint32 page_grid_num = idle_grid_ptr->grid_row * idle_grid_ptr->grid_col;
    int32 i = 0;
    
    if(idle_grid_ptr->page_num >= MMIQBTHEME_MAX_WIDGET_PAGE_NUM)
    {
        return -1;
    }
    for(i = idle_grid_ptr->page_num -1 ; i >= page_position; i--  )
    {
        SCI_MEMCPY(idle_grid_ptr->grid_data_tab[i+1] ,idle_grid_ptr->grid_data_tab[i],
            sizeof(uint16)*page_grid_num);
        SCI_MEMCPY(idle_grid_ptr->previous_grid_data_tab[i+1] ,idle_grid_ptr->previous_grid_data_tab[i],
            sizeof(uint16)*page_grid_num);
    }
    SCI_MEMSET(idle_grid_ptr->grid_data_tab[page_position], 0, 
        sizeof(uint16)*page_grid_num);
    SCI_MEMSET(idle_grid_ptr->previous_grid_data_tab[page_position], 0xff, 
        sizeof(uint16)*page_grid_num);    
    
    idle_grid_ptr->page_num++;
    //MMIQBIDLE_SetCurPageIndex( page_position);
    //MMIQBIDLE_SlidePage(page_position);
    return 0;
}


//delete all items in a widget page
LOCAL void DeletePageItems(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_idx)
{
    uint32 j = 0;
    uint32 widget_id = 0;
    uint32 page_grid_num = idle_grid_ptr->grid_row * idle_grid_ptr->grid_col;

    if (page_idx >= (int32)idle_grid_ptr->page_num)
    {
        return;
    }
    for (j = 0; j < page_grid_num; j++)
    {         
        if (!IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page_idx][j]))
        {
            if (IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[page_idx][j]) && !IS_NODE_AUX(idle_grid_ptr->grid_data_tab[page_idx][j]))
            {
                widget_id  = GET_NODE_ID(idle_grid_ptr->grid_data_tab[page_idx][j]);
                
                if (!MMIWIDGET_IsValidWidgetId(widget_id))
                {
                    continue;
                }
                MMIWIDGET_RemoveWidgetIcon(idle_grid_ptr, page_idx, 
                    widget_id, MMIGRID_ITEM_TYPE_WIDGET );
            }
            else if (IS_NODE_ICON(idle_grid_ptr->grid_data_tab[page_idx][j]))
            {
                MMIWIDGET_RemoveWidgetIcon(idle_grid_ptr, page_idx, 
                    j, MMIGRID_ITEM_TYPE_ICON );
            }            
        }
    } 
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: ying.xu
//  Note:   删除一页widget页面，调整current_page, refresh page
/*****************************************************************************/
PUBLIC int32 MMIWIDGET_DeleteWidgetPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_index)  
{
    int32 i = 0;
    uint32 page_grid_num = idle_grid_ptr->grid_row * idle_grid_ptr->grid_col;

    if(idle_grid_ptr->page_num == 1)
    {
        return -1;
    }
    
    DeletePageItems(idle_grid_ptr, page_index);
    for(i = page_index; i < (int32)idle_grid_ptr->page_num - 1; i++ )
    {
        SCI_MEMCPY(idle_grid_ptr->grid_data_tab[i] ,idle_grid_ptr->grid_data_tab[i + 1],
            sizeof(uint16)*page_grid_num);
        SCI_MEMCPY(idle_grid_ptr->previous_grid_data_tab[i] ,idle_grid_ptr->previous_grid_data_tab[i + 1],
            sizeof(uint16)*page_grid_num);
    }    
    idle_grid_ptr->page_num--;   
    SCI_MEMSET(idle_grid_ptr->grid_data_tab[idle_grid_ptr->page_num],0 ,
        sizeof(uint16)*page_grid_num);
    
    //删除的是最后一页，当前页向前移动一页
    if(idle_grid_ptr->cur_page_index >= idle_grid_ptr->page_num)
    {
        idle_grid_ptr->cur_page_index = idle_grid_ptr->page_num -1 ;
        idle_grid_ptr->new_page_index = idle_grid_ptr->page_num -1 ;
    }
    return 0;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: ying.xu
//  Note:   交换相邻2页widget页面的数据
/*****************************************************************************/
PUBLIC int32 MMIWIDGET_ExchangeWidgetPageData(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_position1, int32 page_position2)
{
    uint32 page_grid_num = idle_grid_ptr->grid_row * idle_grid_ptr->grid_col;

    uint16 *temp_grid_data = 0;

    temp_grid_data = (uint16 *)(SCI_ALLOC_APP(page_grid_num*sizeof(uint16)));
    
    SCI_MEMCPY(temp_grid_data ,
        idle_grid_ptr->grid_data_tab[page_position1],
        sizeof(uint16)*page_grid_num);
    
    SCI_MEMCPY(idle_grid_ptr->grid_data_tab[page_position1] ,
        idle_grid_ptr->grid_data_tab[page_position2],
        sizeof(uint16)*page_grid_num);
    
    SCI_MEMCPY(idle_grid_ptr->grid_data_tab[page_position2] ,
        temp_grid_data,
        sizeof(uint16)*page_grid_num);
    
     SCI_MEMCPY(temp_grid_data ,
        idle_grid_ptr->previous_grid_data_tab[page_position1],
        sizeof(uint16)*page_grid_num);
    
    SCI_MEMCPY(idle_grid_ptr->previous_grid_data_tab[page_position1] ,
        idle_grid_ptr->previous_grid_data_tab[page_position2],
        sizeof(uint16)*page_grid_num);
    
    SCI_MEMCPY(idle_grid_ptr->previous_grid_data_tab[page_position2] ,
        temp_grid_data,
        sizeof(uint16)*page_grid_num);
    
    SCI_FREE(temp_grid_data);
    return 0;
}


/*****************************************************************************/
//  Description : get widget ctrl rect
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIWIDGET_GetWidgetCtrlRect(
                                            MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                            )
{
    GUI_RECT_T rect = {0};
    
    if (PNULL == idle_grid_ptr)
    {
        return rect;
    }
    if (idle_grid_ptr->is_main_screen_landscape)
    {
        rect = idle_grid_ptr->both_rect.h_rect;
    }
    else
    {
        rect = idle_grid_ptr->both_rect.v_rect;
    }
    return rect;
}

//qbtheme special
/*****************************************************************************/
//  Description : GET SHORTCUT RECT
//  Global resource dependence : 
//  Author:YING.XU
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T  MMIWIDGET_GetIconRectFromGridRect(GUI_RECT_T rect)
{
    GUI_RECT_T return_rect = {0};
    return_rect = GUI_GetCenterRect( rect,
                    MMITHEME_MENU_ITEM_WIDTH, 
                    MMITHEME_MENU_ITEM_HEIGHT);   
    return return_rect;
}

/*****************************************************************************/
//  Description : SetShortcutDeleteIconRect
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIWIDGET_IsPointInDeleteIconRect( MMI_IDLE_GRID_DATA_T *idle_grid_ptr,  GUI_POINT_T point)
{
    uint16   img_width = 0;
    uint16   img_height = 0;
    uint16   click_id = MMIWIDGET_INVALID_ID;
    GUI_RECT_T     win_rect = {0};
    MMI_IMAGE_ID_T icon_id = idle_grid_ptr->delect_icon_id;
    GUI_RECT_T rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = 0;
    MMIGRID_ITEM_TYPE_E press_type = 0;
    BOOLEAN result =  FALSE;
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, icon_id, idle_grid_ptr->win_id);

    press_type = MMIWIDGET_GetGridTpPressedItem(idle_grid_ptr, point, &click_id);
    
    if(MMIGRID_TP_CLICK_WIDGET == idle_grid_ptr->tp_move_info.click_type)
    {        
    	if(MMIGRID_ITEM_TYPE_WIDGET != press_type)
	    {
	        return result;
	    }
        if(click_id != idle_grid_ptr->tp_move_info.click_id)
        {
            return result;
        }
        item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);
        if(PNULL != item_ptr)
        {
            MMK_GetWinRect(item_ptr->item_info.win_id, &win_rect);	
            rect = win_rect;
            rect.right  = rect.left + 2*img_width;
            rect.bottom = rect.top + 2*img_height;
        }
    }
    else if(MMIGRID_TP_CLICK_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)    
    {
	    if(MMIGRID_ITEM_TYPE_ICON != press_type)
	    {
	        return result;
	    }
	    if(idle_grid_ptr->tp_move_info.click_id 
	        != GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][click_id]))
	    {
	        return result;
	    }

        rect = idle_grid_ptr->page_grid_rect[click_id];
        rect = MMIWIDGET_GetIconRectFromGridRect(rect);        
        rect.right  = rect.left + (int16)(1.5*img_width);
        rect.bottom = rect.top + (int16)(1.5*img_height);
    }

    if(GUI_PointIsInRect( point, rect))
	{
	    result =  TRUE;
	}
    return result;
}


PUBLIC BOOLEAN MMIWIDGET_ResetShortCutItem(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    BOOLEAN  ret = FALSE;
    uint16 index = 0;
    MMI_IDLE_SC_ICON_T shortcut_item = {0};
    
    if(PNULL == idle_grid_ptr)
    {
        return FALSE;
    }
    
    shortcut_item = idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id];
    
    if(idle_grid_ptr->old_grid.is_valid)
    {
        index = MMIWIDGET_GetShortCutIdx(idle_grid_ptr,shortcut_item.icon_info);
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&idle_grid_ptr->old_grid.filled_grid_cell,
            idle_grid_ptr->old_grid.page, index,MMIGRID_ITEM_TYPE_ICON); 
        ret = TRUE;
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : get widget page width
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWIDGET_GetWidgetPageWidth(
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                )
{
    GUI_RECT_T widget_rect = {0};
    uint16 page_width = 0;
    
    if (PNULL == idle_grid_ptr)
    {
        return 0;
    }
    widget_rect = MMIWIDGET_GetWidgetCtrlRect(idle_grid_ptr);
    page_width = (widget_rect.right + 1 - widget_rect.left);
    return page_width;
}

/*****************************************************************************/
//  Description : reset all child window rect.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_ReCalcAllWidgetWinRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    int32 i = 0;
    GUI_RECT_T win_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    
    
    for (i = MMIWIDGET_INVALID_ID + 1; i < idle_grid_ptr->max_item_num; i++)
    {        
        item_ptr = &idle_grid_ptr->item_ptr[i - 1];
        
        if(item_ptr->is_active)
        {
            if (MMIWIDGET_GetWinRect(idle_grid_ptr, i, &win_rect))
            {              
                MMIWIDGET_SetChildWinRect(item_ptr->item_info.win_id, win_rect, idle_grid_ptr->is_main_screen_landscape);            
            }
            else
            {
                MMIWIDGET_DEBUG(LOG_STR("run_mode = %d"), idle_grid_ptr->run_mode);
                if (MMIGRID_RUN_MODE_MOVABLE == idle_grid_ptr->run_mode)
                {
                    if( (idle_grid_ptr->movable_tp_info.click_type != MMIGRID_TP_CLICK_LONG_WIDGET
                        && idle_grid_ptr->movable_tp_info.click_id != (uint32)i))
                    {
                        MMIWIDGET_DEBUG(LOG_STR("click_type = %d, click_id = %d"), 
                            idle_grid_ptr->movable_tp_info.click_type, idle_grid_ptr->movable_tp_info.click_id);
                    }
                }
                else
                {
                    MMIWIDGET_DEBUG(LOG_STR("click_type = %d, click_id = %d"), 
                        idle_grid_ptr->movable_tp_info.click_type, idle_grid_ptr->movable_tp_info.click_id);
                }

            }
        } 
    }

    
    return TRUE;
}
/*****************************************************************************/
//  Description : reset  child window rect in one page
//  Global resource dependence : 
//  Author:ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_ReCalcWidgetWinRectInOnePage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_index)
{
    uint32 j = 0;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    uint16 img_width  = 0;
    uint16 img_height = 0;
    uint16 col_num    = 0;
    uint16 row_num    = 0;
    GUI_RECT_T widget_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;    
    MMI_IMAGE_ID_T    bg_img = 0;
    uint32          widget_id = 0;
    uint32 page_grid_num = idle_grid_ptr->grid_row * idle_grid_ptr->grid_col;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    for (j = 0; j < page_grid_num; j++)
    {
        if (   IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[page_index][j]) 
            && !IS_NODE_AUX(idle_grid_ptr->grid_data_tab[page_index][j])
            )
        {   
            widget_id = GET_NODE_ID(idle_grid_ptr->grid_data_tab[page_index][j]);
            
            if(!MMIWIDGET_IsValidWidgetId(widget_id))
            {
                return FALSE;
            }   
            
            item_ptr = &idle_grid_ptr->item_ptr[widget_id - 1];    
    
            if (PNULL != item_ptr->item_info.GetWidgetImage)
            {
                item_ptr->item_info.GetWidgetImage(&bg_img, PNULL);
            }
            else
            {
                bg_img = item_ptr->item_info.large_bg_img;
            } 
            
            GUIRES_GetImgWidthHeight(&img_width, &img_height, bg_img, idle_grid_ptr->win_id);                
            
            col_num = img_width/idle_grid_ptr->grid_width + (((img_width%idle_grid_ptr->grid_width) == 0)?(0):(1));
            row_num = img_height/idle_grid_ptr->grid_height + (((img_height%idle_grid_ptr->grid_height) == 0)?(0):(1)); 
            
            widget_rect.left   = idle_grid_ptr->page_grid_rect[j].left;
            widget_rect.right  = idle_grid_ptr->page_grid_rect[j + col_num - 1].right;
            widget_rect.top    = idle_grid_ptr->page_grid_rect[j].top;
            widget_rect.bottom = idle_grid_ptr->page_grid_rect[j + (row_num - 1) * idle_grid_ptr->grid_col + col_num - 1].bottom;
            
            widget_rect.left   = (widget_rect.left + widget_rect.right - img_width)/2;
            widget_rect.top    = (widget_rect.top + widget_rect.bottom- img_height)/2;                
            
            widget_rect.left  += (int16)((page_index - idle_grid_ptr->cur_page_index) * lcd_width);
            widget_rect.right  = widget_rect.left + img_width - 1;
            widget_rect.bottom = widget_rect.top + img_height - 1;
       
            MMIWIDGET_SetChildWinRect(item_ptr->item_info.win_id, widget_rect, idle_grid_ptr->is_main_screen_landscape);            
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : dispatch msg to all child win
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_DispatchMsgToAllChildWin(
                                 MMI_IDLE_GRID_DATA_T  *idle_grid_ptr,
                                 MMI_MESSAGE_ID_E  msg_id
                                 )
{
 uint32 i = 0;
 
 if (PNULL != idle_grid_ptr->item_ptr)
 {
     for (i = 0; i < MMIWIDGET_MAX_ITEM_NUM; i++)
     {
         MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, idle_grid_ptr->item_ptr[i].item_info.widget_id, msg_id, PNULL);
         // MMK_SendMsg(idle_grid_ptr->item_ptr[i].item_info.win_id, msg_id, PNULL);
     }
 }    
}

/*****************************************************************************/
//  Description : dispatch msg to active child win
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWIDGET_DispatchMsgToActiveChildWin(
                             MMI_IDLE_GRID_DATA_T  *idle_grid_ptr,
                             MMI_MESSAGE_ID_E  msg_id
                             )
{
    uint32 i = 0;
    MMI_RESULT_E result = MMI_RESULT_FALSE;    
     
    if (PNULL != idle_grid_ptr->item_ptr)
    {
        for (i = 0; i < MMIWIDGET_MAX_ITEM_NUM; i++)
        {
            MMI_RESULT_E cur_res = MMI_RESULT_FALSE;

            if(idle_grid_ptr->item_ptr[i].is_active)
            {
                cur_res = MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, idle_grid_ptr->item_ptr[i].item_info.widget_id, msg_id, PNULL);
            }
            if (MMI_RESULT_TRUE == cur_res) 
            {
                result = MMI_RESULT_TRUE;
            }
        }
    }    
    return result;
}

/*****************************************************************************/
//  Description : dispatch msg to widgets in one page
//  Global resource dependence : 
//  Author:ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWIDGET_DispatchMsgToChildInOnePage(                                            
                                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                         int32             page_idx,
                                         MMI_MESSAGE_ID_E  msg_id,
                                         DPARAM            param
                                         )
{
    uint32 j = 0;
    int32 widget_id = 0;
    MMI_RESULT_E result = MMI_RESULT_FALSE;    
    uint32 max_cell_num = 0;
    if (page_idx < 0 || page_idx >= idle_grid_ptr->page_num)
    {
        return MMI_RESULT_FALSE;
    }

    max_cell_num = idle_grid_ptr->grid_col * idle_grid_ptr->grid_row;
    for (j = 0; j < max_cell_num; j++)
    {    
        if (IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[page_idx][j]) && !IS_NODE_AUX(idle_grid_ptr->grid_data_tab[page_idx][j]))
        {
            MMI_RESULT_E cur_res = MMI_RESULT_FALSE;
            widget_id  = GET_NODE_ID(idle_grid_ptr->grid_data_tab[page_idx][j]);
            
            if (!MMIWIDGET_IsValidWidgetId(widget_id))
            {
                continue;
            }                
            cur_res = MMIWIDGET_RunChildWidgetProcEx( idle_grid_ptr,  widget_id,  msg_id,  param, TRUE );
            if (MMI_RESULT_TRUE == cur_res) 
            {
                result = MMI_RESULT_TRUE;
            }
        }
    } 
    return result;
}                                           


/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void CreateMoveLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMIGRID_ITEM_TYPE_E elem_type)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    UILAYER_CREATE_T create_info = {0};
    MMI_HANDLE_T click_win_handle = 0;
    GUI_RECT_T win_rect = {0};
    //MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"CreateMoveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_3369_112_2_18_3_9_8_299,(uint8*)"");
        return;
    }
    
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->move_layer))    
    {
        if (MMIGRID_ITEM_TYPE_WIDGET == elem_type)
        {        
            click_win_handle = MMIWIDGET_GetWinHandleByWidgetId(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);
            MMK_GetWinRect(click_win_handle, &win_rect);              
            create_info.offset_x = win_rect.left;
            create_info.offset_y = win_rect.top;
            create_info.width = (win_rect.right+1-win_rect.left);
            create_info.height = (win_rect.bottom+1-win_rect.top);
        }
        else if (MMIGRID_ITEM_TYPE_ICON == elem_type)
        {        
            create_info.offset_x = win_rect.left;
            create_info.offset_y = win_rect.top;
            create_info.width = MMITHEME_MENU_ITEM_WIDTH;
            create_info.height = MMITHEME_MENU_ITEM_HEIGHT;             
        }    
        
        create_info.lcd_id = LCD_ID_0;
        create_info.owner_handle = idle_grid_ptr->win_id;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;  
        create_info.format = UILAYER_MEM_IMMUTABLE;
        UILAYER_CreateLayer(  &create_info, &widget_layer_handle );
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        
        idle_grid_ptr->move_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }  
}

/*****************************************************************************/
//  Description : destroy widget layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DestroyMoveLayer(
                            MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                            )
{

    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->move_layer))    
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->move_layer));   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : handle menu tp long down.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HandleIconTpLongDown(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)
{
    int32 grid_idx = 0;
    GUI_RECT_T item_rect = {0};
    UILAYER_INFO_T layer_info = {0};
    
    CreateMoveLayer(idle_grid_ptr, MMIGRID_ITEM_TYPE_ICON);
    
    grid_idx = MMIWIDGET_GetGridIndexByPos(idle_grid_ptr, point);
    
    UILAYER_GetLayerInfo(&idle_grid_ptr->move_layer, &layer_info);
	
	//如果是在非空区域点击，则将layer放在正中
    if (MMI_IDLE_GRID_PAGE_CELL_NUM == grid_idx)
    {
		item_rect.left = (MMI_MAINSCREEN_WIDTH  - layer_info.layer_width)/2;
		item_rect.top  = (MMI_MAINSCREEN_HEIGHT - layer_info.layer_height)/2;
    }
	else
	{
		item_rect.left  = (idle_grid_ptr->page_grid_rect[grid_idx].left + idle_grid_ptr->page_grid_rect[grid_idx].right - layer_info.layer_width)/2;
		item_rect.top   = (idle_grid_ptr->page_grid_rect[grid_idx].top + idle_grid_ptr->page_grid_rect[grid_idx].bottom - layer_info.layer_height)/2;
	}
    
    UILAYER_SetLayerPosition(&idle_grid_ptr->move_layer, item_rect.left, item_rect.top);
    
    MMIWIDGET_DispShortCutHightlight(idle_grid_ptr, FALSE);
    
    //CreateTpMoveTimer(idle_grid_ptr->win_id); 
    return TRUE;
}

/*****************************************************************************/
//  Description : handle widget tp long down.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HandleWidgetTpLongDown(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint16 click_widget_id = MMIWIDGET_INVALID_ID;
    MMI_HANDLE_T click_win_handle = 0;
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    
    click_widget_id = idle_grid_ptr->tp_move_info.click_id;
    
    click_win_handle = MMIWIDGET_GetWinHandleByWidgetId(idle_grid_ptr, click_widget_id);
    
    //MMK_SetChildWinFocus(idle_grid_ptr->win_id, click_win_handle);
    
    CreateMoveLayer(idle_grid_ptr, MMIGRID_ITEM_TYPE_WIDGET);
    
    item_ptr = &idle_grid_ptr->item_ptr[click_widget_id - 1];
    
    MMK_SetWinLcdDevInfoNotifyCtrl(item_ptr->item_info.win_id, &idle_grid_ptr->move_layer);
    MMIWIDGET_DisplayWidgetHighlight(idle_grid_ptr, FALSE);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : move widget on idle screen.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MoveWidgetIdle2Idle(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_POINT_T offset_point = {0};
    GUI_POINT_T old_offset = {0};
    UILAYER_INFO_T layer_info = {0};
    BOOLEAN result = TRUE;
    
    UILAYER_GetLayerPosition(&idle_grid_ptr->move_layer, &offset_point.x, &offset_point.y);
    UILAYER_GetLayerInfo(&idle_grid_ptr->move_layer, &layer_info);
    
    old_offset = offset_point;
    offset_point.x += (point.x - idle_grid_ptr->save_point.x);
    offset_point.y += (point.y - idle_grid_ptr->save_point.y);
/*    
    if (offset_point.y <= MMI_STATUSBAR_HEIGHT)
    {
        UILAYER_SetLayerPosition(&idle_grid_ptr->move_layer, offset_point.x, MMI_STATUSBAR_HEIGHT);
        result = FALSE;
        goto RETURN_LABEL;
    }
*/    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);  
    
    if (   0 == idle_grid_ptr->cur_page_index 
        && offset_point.x < 0 
        && abs(offset_point.x) >= layer_info.layer_width/2)
    {
        UILAYER_SetLayerPosition(&idle_grid_ptr->move_layer, -layer_info.layer_width/2, offset_point.y);
        result = FALSE;        
        goto RETURN_LABEL;
    }
    
    if (   ((idle_grid_ptr->page_num - 1) == idle_grid_ptr->cur_page_index)
        && (offset_point.x + layer_info.layer_width) >= (lcd_width + layer_info.layer_width/2))
    {
        UILAYER_SetLayerPosition(&idle_grid_ptr->move_layer, lcd_width - layer_info.layer_width/2, offset_point.y);    
        result = FALSE;
        goto RETURN_LABEL;
    }
    
    UILAYER_SetLayerPosition(&idle_grid_ptr->move_layer, offset_point.x, offset_point.y);

RETURN_LABEL:
    idle_grid_ptr->save_point = point;        
    
    return result;
}

/*****************************************************************************/
//  Description : draw rect frame.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DrawRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_RECT_T rect, GUI_COLOR_T color)
{
    if (   idle_grid_ptr->showed_rect_info.is_valid 
        && rect.left == idle_grid_ptr->showed_rect_info.old_rect.left
        && rect.top == idle_grid_ptr->showed_rect_info.old_rect.top
        && rect.right == idle_grid_ptr->showed_rect_info.old_rect.right
        && rect.bottom == idle_grid_ptr->showed_rect_info.old_rect.bottom)
    {
        return;
    }

    if (idle_grid_ptr->showed_rect_info.is_valid 
        && idle_grid_ptr->showed_rect_info.page_idx == idle_grid_ptr->cur_page_index)
    {
        ClearRect(idle_grid_ptr);
        idle_grid_ptr->showed_rect_info.is_valid = FALSE;
    }
        
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)
#endif    
    {        
        LCD_DrawRect(&idle_grid_ptr->page_layer, rect, color);
    }    
    idle_grid_ptr->showed_rect_info.page_idx = idle_grid_ptr->cur_page_index;    
    idle_grid_ptr->showed_rect_info.old_rect = rect;
    idle_grid_ptr->showed_rect_info.is_valid = TRUE;
}

/*****************************************************************************/
//  Description : clear showed rect frame buffer.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void ClearRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint16 page_width = 0;
    GUI_RECT_T clear_rect = {0}; 

    page_width = MMIWIDGET_GetWidgetPageWidth(idle_grid_ptr);

    clear_rect = idle_grid_ptr->showed_rect_info.old_rect;
    clear_rect.left  += (idle_grid_ptr->showed_rect_info.page_idx - idle_grid_ptr->cur_page_index) * page_width;
    clear_rect.right += (idle_grid_ptr->showed_rect_info.page_idx - idle_grid_ptr->cur_page_index) * page_width;

    UILAYER_ClearRect(&idle_grid_ptr->page_layer, clear_rect);
}

/*****************************************************************************/
//  Description : draw page layer.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DrawPageLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);
    uint32     i = 0;
    int32      j = -((page_layer_size - 1)/2);   /*lint !e501 */
    uint32     k = 0;
    if(1 == page_layer_size)
    {
        DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
    }
    else if(3 == page_layer_size)
    {   
    	//draw pages with buffers when fullpaint 
        for(i = 0 ; i < page_layer_size; i++)
        {
            k = (page_layer_size + j) % page_layer_size;    /*lint !e737 */
            if(0 == idle_grid_ptr->page_cache_flag[k])
            {                
                UILAYER_SetPreMultiplyAlpha(&idle_grid_ptr->page_cache[k], FALSE, FALSE);        /*lint !e416 */
                UILAYER_Clear(&idle_grid_ptr->page_cache[k]);     /*lint !e416 */      
                DrawSpecificPageEx( idle_grid_ptr,  idle_grid_ptr->page_cache[k], 
                    (idle_grid_ptr->cur_page_index + j + idle_grid_ptr->page_num) % idle_grid_ptr->page_num,   j,  0);                    
                idle_grid_ptr->page_cache_flag[k] = 1;
            }
            j++;
        }
    }
}


/*****************************************************************************/
//  Description : draw specific page contents.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void DrawSpecificPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_idx)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (page_idx >= idle_grid_ptr->page_num|| page_idx < 0)
    {
        return;
    }
    
    if (UILAYER_IsLayerActive(&idle_grid_ptr->page_layer))
    {
        lcd_dev_info = idle_grid_ptr->page_layer;
        UILAYER_Clear(&lcd_dev_info);
    }
    else
    {
        BOOLEAN is_focus = MMK_IsFocusWin( idle_grid_ptr->win_id);
        //SCI_TRACE_LOW:"MMIGRID_IDLE DrawSpecificPage wid is focues = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_7140_112_2_18_3_7_11_35,(uint8*)"d", is_focus);
        lcd_dev_info = *MMITHEME_GetDefaultLcdDev();
    }
    
     DrawSpecificPageEx(idle_grid_ptr,lcd_dev_info,page_idx,0,0);
}

PUBLIC void DrawSpecificPageEx(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                    GUI_LCD_DEV_INFO lcd_dev_info,
                    int32 page_idx, int32 page_position, int16 offset)
{
    uint32 j = 0;
    uint32 widget_id = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_RECT_T display_rc = {0};
    uint32 item_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id  = 0;
    CTRLMAINMENU_ITEM_T menuitem_info = {0};
    uint32 page_grid_num = idle_grid_ptr->grid_col*idle_grid_ptr->grid_row;
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);
    
    if(page_idx>=idle_grid_ptr->page_num ||page_idx < 0)
    {
        return;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    if (!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
    }
    
    for (j = 0; j < page_grid_num; j++)
    {         
        if (!IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page_idx][j]))
        {
            if (IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[page_idx][j]) && !IS_NODE_AUX(idle_grid_ptr->grid_data_tab[page_idx][j]))
            {
                widget_id  = GET_NODE_ID(idle_grid_ptr->grid_data_tab[page_idx][j]);
               
                if (!MMIWIDGET_IsValidWidgetId(widget_id))
                {
                    continue;
                }               
                if(idle_grid_ptr->tp_move_info.click_id == widget_id
                    && (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
                    )
                {
                    continue;
                }
                item_ptr = &idle_grid_ptr->item_ptr[widget_id - 1];
                
                MMIWIDGET_GetWinRect(idle_grid_ptr, widget_id, &display_rc);   
                
                display_rc.left = display_rc.left % lcd_width;   
                if(display_rc.left < 0)
                {
                    display_rc.left += lcd_width;
                }
                display_rc.right = display_rc.right % lcd_width;
                if(display_rc.right <= 0)
                {
                    display_rc.right += lcd_width;
                }
                
                display_rc.left += lcd_width * page_position;   
                display_rc.right += lcd_width * page_position;
                display_rc.left += offset;   
                display_rc.right += offset;
                MMIWIDGET_SetChildWinRect(item_ptr->item_info.win_id, display_rc, idle_grid_ptr->is_main_screen_landscape);
                
#ifdef MMI_IDLE_WIDGET_CACHE
                if(1 == page_layer_size && (MMIGRID_TP_MOVE_PAGE & idle_grid_ptr->widget_mov_type))     /*lint !e655 */
                {
                    MMIWIDGET_DrawWidget(item_ptr, &lcd_dev_info, &display_rc);                 
                }
                else //if(3 == page_layer_size)
#endif                
                {                   
                    MMK_SetWinLcdDevInfoNotifyCtrl(item_ptr->item_info.win_id,&lcd_dev_info);
                    MMIWIDGET_RedrawWidgetItem(item_ptr);  
                 
                }
				if (idle_grid_ptr->is_editable)
				{
					DrawDeleteIcon(idle_grid_ptr,&display_rc, &lcd_dev_info);
				}
                
            }
            else if (IS_NODE_ICON(idle_grid_ptr->grid_data_tab[page_idx][j]))
            {
                item_id  = GET_NODE_ID(idle_grid_ptr->grid_data_tab[page_idx][j]);
                group_id = idle_grid_ptr->icon_ptr[item_id].icon_info.group_id;
                menu_id  = idle_grid_ptr->icon_ptr[item_id].icon_info.menu_id;
                
                if (!MMIMAINMENU_GetItemData(menu_id, group_id, &menuitem_info))
                {
                    SET_NODE_EMPTY(idle_grid_ptr->grid_data_tab[page_idx][j]);
                    continue;
                }
                
                display_rc = idle_grid_ptr->page_grid_rect[j];
                
                display_rc.left = display_rc.left % lcd_width;
                if(display_rc.left < 0)
                {
                    display_rc.left += lcd_width;
                }
                display_rc.right = display_rc.right % lcd_width;
                if(display_rc.right <= 0)
                {
                    display_rc.right += lcd_width;
                }
                display_rc.left += lcd_width * page_position;   
                display_rc.right += lcd_width * page_position;
                
                display_rc.left += offset;   
                display_rc.right += offset;
                
                MMIWIDGET_ShortCutDrawMenuIcon(idle_grid_ptr->win_id, &menuitem_info, 
                                                display_rc, &lcd_dev_info,
                                                idle_grid_ptr->delect_icon_id, idle_grid_ptr->is_editable);
                
            }            
                        
        }
    } 

    if ( (MMIGRID_RUN_MODE_MOVABLE == idle_grid_ptr->run_mode) &&
         ((MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->movable_tp_info.click_type
        || MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->movable_tp_info.click_type))
        )
    {
        FillSpecificPageWithMaskColor(idle_grid_ptr, &lcd_dev_info, page_idx, page_position,offset);        
    }    
}

/*****************************************************************************/
//  Description : create status bar layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMIWIDGET_CreateLayer(MMI_WIN_ID_T win_id,GUI_RECT_T *rect_ptr, DISPLAY_DATA_TYPE_E dst_data_type)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0, UILAYER_NULL_HANDLE};
    UILAYER_CREATE_T create_info         = {0};     
 
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = rect_ptr->left;
    create_info.offset_y        = rect_ptr->top;
    create_info.width           = rect_ptr->right - rect_ptr->left + 1;
    create_info.height          = rect_ptr->bottom - rect_ptr->top +  1;
    create_info.is_bg_layer     = DATA_TYPE_RGB565 == dst_data_type ? 1:0;
    create_info.is_static_layer = FALSE;   
    
   UILAYER_CreateLayer(&create_info, &widget_layer_handle);            
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);   
    }
    return widget_layer_handle;
}

/*****************************************************************************/
//  Description : zoom in page to thumb on main layer.
//  Global resource dependence : 
//  Author:  ying.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN ThumbLayer(
                                        GUI_LCD_DEV_INFO *p_src_lcd_dev,
                                        GUI_RECT_T *p_src_rect,
                                        GUI_LCD_DEV_INFO *p_dst_lcd_dev,
                                        GUI_RECT_T *p_thumb_rect,
                                        DISPLAY_DATA_TYPE_E dst_data_type,
                                        SCALE_IMAGE_OUT_T *p_scale_out
                                        )
{
    SCALE_IMAGE_IN_T    scale_in  = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};
    uint8 *temp_buf   = PNULL;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    uint16 thumb_rect_width  = p_thumb_rect->right - p_thumb_rect->left + 1;    
    uint16 thumb_rect_height = p_thumb_rect->bottom - p_thumb_rect->top + 1;
    uint8 *tar_buf   = 0;
    BOOLEAN    result = TRUE;
    uint16      size_per_pixel = 4;
    IMGREF_DATA_FORMAT_E buffer_format = IMGREF_FORMAT_ARGB888;

    if(DATA_TYPE_ARGB888 == dst_data_type)
    {
        size_per_pixel = 4;
        buffer_format = IMGREF_FORMAT_ARGB888;
    }
    else if(DATA_TYPE_RGB565 == dst_data_type)
    {
        size_per_pixel = 2;        
        buffer_format = IMGREF_FORMAT_RGB565;
    }
    
    temp_buf = UILAYER_GetLayerBufferPtr(p_src_lcd_dev);
    tar_buf   = UILAYER_GetLayerBufferPtr(p_dst_lcd_dev); 
    
    if(tar_buf == 0)
    {
        SCI_TRACE_LOW("DisplayGridPageThumbView tar buffer == 0");
        return FALSE;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);   
    scale_in.src_format           = buffer_format;
    scale_in.src_chn.chn0.ptr     = temp_buf;
    scale_in.src_chn.chn0.size    = lcd_width * lcd_height * size_per_pixel ;
    scale_in.src_size.w           = lcd_width;
    scale_in.src_size.h           = lcd_height;        
    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = lcd_width;
    scale_in.src_trim_rect.h      = lcd_height;
    scale_in.target_buf.size      = thumb_rect_width * thumb_rect_height * size_per_pixel ;          
    scale_in.target_buf.ptr       = tar_buf;                
    scale_in.scale_mode           = 0;  //SCALE_MODE_DISTORT;
    scale_in.target_format        = buffer_format;
    scale_in.target_size.w        = thumb_rect_width;
    scale_in.target_size.h        = thumb_rect_height;
    
    SCI_MEMSET(scale_in.target_buf.ptr, 0, scale_in.target_buf.size);
    
    if (SCI_SUCCESS != GRAPH_ScaleImage(&scale_in,&scale_out))            
    {
        //SCI_TRACE_LOW:"MMIGRID DisplayGridPageThumbView GRAPH_ScaleImage error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_9063_112_2_18_3_7_16_38,(uint8*)"");
        result =  FALSE;
    }
    if(p_scale_out != 0)
    {
        SCI_MEMCPY(p_scale_out, &scale_out, sizeof(SCALE_IMAGE_OUT_T));
    }
    return result;
}

/*****************************************************************************
** File Name:      mmiqbtheme_wintab.c                                       *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe                             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009     ying.xu              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmidisplay_data.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmi_theme.h"
#include "mmiwidget.h"
#include "mmi_nv.h"
#include "mmi_modu_main.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmitheme_mainmenu.h"
#include "mmiset_wintab.h"
#include "mmiwidget_export.h"
#include "mmi_mainmenu_export.h"

#include "mmiqbtheme_config.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"

LOCAL int32 CalculateRectInterval(MMIQBTHEME_GRID_T * p_grid);

PUBLIC int32 MMIQBTHEME_GridCreate(MMIQBTHEME_GRID_T ** p_grid
                                   )
{
    MMIQBTHEME_GRID_T *p_temp_grid = 0;
    
    if(p_grid == PNULL)
    {
        return -1;
    }
    
    p_temp_grid = (MMIQBTHEME_GRID_T *)SCI_ALLOCA(sizeof(MMIQBTHEME_GRID_T));
    SCI_MEMSET(p_temp_grid, 0, sizeof(MMIQBTHEME_GRID_T));    
    
    
    //create title,DOCKBAR HAS NO TITLE
    *p_grid = p_temp_grid;
    return 0;
}                                           


//destroy an virtual win objcet
PUBLIC int32 MMIQBTHEME_GridDestroy(MMIQBTHEME_GRID_T* p_grid)
{
    if(p_grid == PNULL)
    {
        return -1;
    }

    SCI_FREE(p_grid);
    return 0;
}

PUBLIC int32 MMIQBTHEME_GridSetRect(MMIQBTHEME_GRID_T * p_grid,GUI_RECT_T rect)
{
    p_grid->rect = rect;
    return 0;
}

PUBLIC int32 MMIQBTHEME_GridSetOneGridWidthHeight(MMIQBTHEME_GRID_T * p_grid, int32 grid_width,int32 grid_height)
{
    p_grid->grid_width = grid_width;    
    p_grid->grid_height = grid_height;    
    CalculateRectInterval(p_grid);
    return 0;
}

PUBLIC int32 MMIQBTHEME_GridSetRowCol(MMIQBTHEME_GRID_T * p_grid, int32 row,int32 col)
{
    p_grid->row = row;    
    p_grid->col = col;    
    CalculateRectInterval(p_grid);
    return 0;
}

//根据 row col grid_width grid_height 计算格子间距
//或者 根据row col 间距 计算格子大小 格子的空间示意图
//           interval
//              +
//  interval + grid + intervel
//              +
//           interval

LOCAL int32 CalculateRectInterval(MMIQBTHEME_GRID_T * p_grid)
{
    if(p_grid->grid_width * p_grid->col > (p_grid->rect.right - p_grid->rect.left + 1))
    {
        return -1;
    }
    if(p_grid->grid_height * p_grid->row> (p_grid->rect.bottom- p_grid->rect.top + 1))
    {
        return -2;        
    }
   if(p_grid->col!=0)
   	{
   	 p_grid->grid_x_interval = ((p_grid->rect.right - p_grid->rect.left +1) 
       	 - p_grid->grid_width * p_grid->col)/(p_grid->col)/2;
   	}
   else
   	{
	  return -3;
    }
   if(p_grid->row!=0)	
   	{
   	 p_grid->grid_y_interval = ((p_grid->rect.bottom- p_grid->rect.top + 1) 
   	     - p_grid->grid_height * p_grid->row)/(p_grid->row)/2;
   	}
   else
   	{
	  return -4;
    }
    return 0;             
}

//一般不用设置，设置完row col grid_width grid_height，会自动计算出来
PUBLIC int32 MMIQBTHEME_GridGetGridInterval(MMIQBTHEME_GRID_T * p_grid, int32* grid_x_interval,int32* grid_y_interval)
{
    *grid_x_interval = p_grid->grid_x_interval;
    *grid_y_interval = p_grid->grid_y_interval;    
    return 0;
}


PUBLIC int32 MMIQBTHEME_GridGetOneGridRect(MMIQBTHEME_GRID_T * p_grid, int32 grid_index, GUI_RECT_T*rect)
{
    int32 row = 0;
    int32 col = 0;
    row = grid_index / p_grid->col;
    col = grid_index%p_grid->col;    
    rect->top = (p_grid->rect.top  + p_grid->grid_y_interval*(row*2+1) + p_grid->grid_height*row);    
    rect->left = (p_grid->rect.left + p_grid->grid_x_interval*(col*2 + 1) + p_grid->grid_width*col);
    rect->bottom = rect->top + p_grid->grid_height - 1;    
    rect->right = rect->left  + p_grid->grid_width -1;
    
    return 0;
}

PUBLIC int32 MMIQBTHEME_GridGetPointLocation(MMIQBTHEME_GRID_T * p_grid, GUI_POINT_T point,int32 *index)
{
    GUI_RECT_T rect = {0};
    int32 temp_index = 0;
    for(temp_index = 0; temp_index < p_grid->row * p_grid->col; temp_index++)
    {
        MMIQBTHEME_GridGetOneGridRect(p_grid, temp_index, &rect);
        if(GUI_PointIsInRect( point,  rect))
        {
            *index = temp_index;
            return 0;
        }
    }
    //not in any grid area
    *index = -1;
    return -1;
}


PUBLIC int32 MMIQBTHEME_GridGetFirstValidGrid(MMIQBTHEME_GRID_T * p_grid, int32 *index)
{
    int32 temp_index = 0;
    int32 row = 0;
    int32 col = 0;
    
    for(row = 0; row < p_grid->row  ; row ++)
    {
        for(col = 0; col< p_grid->col  ; col ++)
        {       
            temp_index = row*(p_grid->col) + col;
            if(p_grid->grid_data[temp_index].type == MMIQBTHEME_ITEM_NONE)
            {
                *index =  temp_index;
                return 0;
            }
        }
    }
    //not in any grid area
    *index = -1;
    return -1;
}

PUBLIC int32 MMIQBTHEME_GridSetData(MMIQBTHEME_GRID_T * p_grid,int32 grid_index, MMIQBTHEME_GRID_DATA_T *p_data)
{    
    if(p_grid->grid_data[grid_index].type == MMIQBTHEME_ITEM_NONE)
    {
        p_grid->grid_data[grid_index] = *p_data;
    }
    
    return 0;
}

PUBLIC int32 MMIQBTHEME_GridClearData(MMIQBTHEME_GRID_T * p_grid,int32 grid_index)
{       
    SCI_MEMSET(&p_grid->grid_data[grid_index], 0, sizeof(MMIQBTHEME_GRID_DATA_T));
    p_grid->grid_data[grid_index].type = MMIQBTHEME_ITEM_NONE;
    
    return 0;
}

PUBLIC int32 MMIQBTHEME_GridClearAllData(MMIQBTHEME_GRID_T * p_grid)
{       
    SCI_MEMSET(&p_grid->grid_data, 0, sizeof(MMIQBTHEME_GRID_DATA_T)*MMIQBTHEME_GRID_PAGE_CELL_NUM);    
    return 0;
}

//set current grid
PUBLIC void  MMIQBTHEME_GridSetCurrentGrid(MMIQBTHEME_GRID_T* p_grid,
                                           uint32 current_grid_index
                                           )
{}

//get  current grid
PUBLIC int32  MMIQBTHEME_GridGetCurrentGrid(MMIQBTHEME_GRID_T* p_grid
                                            )
                                            
{
    return 0;
}
//exchange grid data
PUBLIC int32  MMIQBTHEME_GridExchangeGridData(MMIQBTHEME_GRID_T* p_grid ,
                                int32   gird_id1,
                                int32   gird_id2
                            )
{

    MMIQBTHEME_GRID_DATA_T temp_griddata = {0};
    temp_griddata = p_grid->grid_data[gird_id1];
    p_grid->grid_data[gird_id1] =  p_grid->grid_data[gird_id2];
    p_grid->grid_data[gird_id2] = temp_griddata;
    return 0;
}

 //get  offset of grids that their index offset = grid_offset
PUBLIC int32  MMIQBTHEME_GridGetOneGridPosition(MMIQBTHEME_GRID_T* p_grid ,
                                int32*   offset_x,   int32*   offset_y,
                                int32  grid_offset
                            )
{
    if(offset_x == 0 || offset_y == 0)
    {
        return -1;
    }
    *offset_x = (p_grid->grid_width +  p_grid->grid_x_interval*2)* grid_offset ;
    *offset_y = (p_grid->grid_height +  p_grid->grid_y_interval*2)* grid_offset ;
    return 0;
}                            


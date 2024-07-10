/*****************************************************************************
** File Name:      mmiqbtheme_grid.h                                          *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe phonebook                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2011        YING.XU      Create
******************************************************************************/

#ifndef _MMIQBTHEME_GRID_H_
#define _MMIQBTHEME_GRID_H_ 


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

typedef struct 
{
      MMIQBTHEME_ITEM_TPYE_E type;
      MMIQBTHEME_ITEM_ID_U data;
}MMIQBTHEME_GRID_DATA_T;

//grid struct 
typedef struct
{
    GUI_RECT_T   rect;  //相对于LCD
    int32 row;
    int32 col;
    int32 grid_width;
    int32 grid_height;
    int32 grid_x_interval;
    int32 grid_y_interval;
    //int32 Grid_rect[Row*COL]
    MMIQBTHEME_GRID_DATA_T grid_data[MMIQBTHEME_GRID_PAGE_CELL_NUM]; //row*col    
    MMIQBTHEME_DISPLAY_T    display;
}MMIQBTHEME_GRID_T;

//create an virtual win objcet 
//申请内存,赋初始值
PUBLIC int32 MMIQBTHEME_GridCreate(MMIQBTHEME_GRID_T ** p_grid);

PUBLIC int32 MMIQBTHEME_GridSetRect(MMIQBTHEME_GRID_T * p_grid,GUI_RECT_T   rect);

PUBLIC int32 MMIQBTHEME_GridSetOneGridWidthHeight(MMIQBTHEME_GRID_T * p_grid, int32 grid_width,int32 grid_height);

PUBLIC int32 MMIQBTHEME_GridSetRowCol(MMIQBTHEME_GRID_T * p_grid, int32 row,int32 col);


//一般不用设置，设置完row col grid_width grid_height，会自动计算出来
PUBLIC int32 MMIQBTHEME_GridGetGridInterval(MMIQBTHEME_GRID_T * p_grid, int32* grid_x_interval,int32* grid_y_interval);


PUBLIC int32 MMIQBTHEME_GridGetOneGridRect(MMIQBTHEME_GRID_T * p_grid, int32 grid_index, GUI_RECT_T*rect);

PUBLIC int32 MMIQBTHEME_GridGetPointLocation(MMIQBTHEME_GRID_T * p_grid, GUI_POINT_T point,int32 *index);

PUBLIC int32 MMIQBTHEME_GridGetFirstValidGrid(MMIQBTHEME_GRID_T * p_grid, int32 *index);

PUBLIC int32 MMIQBTHEME_GridSetData(MMIQBTHEME_GRID_T * p_grid,int32 grid_index, MMIQBTHEME_GRID_DATA_T *p_data);

PUBLIC int32 MMIQBTHEME_GridClearData(MMIQBTHEME_GRID_T * p_grid,int32 grid_index);

PUBLIC int32 MMIQBTHEME_GridClearAllData(MMIQBTHEME_GRID_T * p_grid);

//destroy an virtual win objcet
PUBLIC int32 MMIQBTHEME_GridDestroy(MMIQBTHEME_GRID_T* p_grid);

//set current grid
PUBLIC void  MMIQBTHEME_GridSetCurrentGrid(MMIQBTHEME_GRID_T* p_grid,
                                                 uint32 current_grid_index
                                              );
                                              
//get  current grid
PUBLIC int32  MMIQBTHEME_GridGetCurrentGrid(MMIQBTHEME_GRID_T* p_grid
                                              );
//exchange grid data
PUBLIC int32  MMIQBTHEME_GridExchangeGridData(MMIQBTHEME_GRID_T* p_grid ,
                                int32   gird_id1,
                                int32   gird_id2
                            );
                       
 //get  offset of grids that their index offset = grid_offset
PUBLIC int32  MMIQBTHEME_GridGetOneGridPosition(MMIQBTHEME_GRID_T* p_grid ,
                                int32*   offset_x,   int32*   offset_y,
                                int32  grid_offset
                            );                                         

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif

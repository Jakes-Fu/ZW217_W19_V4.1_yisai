/*****************************************************************************
** File Name:      mmigrid.h                                                 *
** Author:                                                                   *
** Date:           12/21/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe grid                        *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        YING.XU      Create
******************************************************************************/

#ifndef _MMIGRID_H_
#define _MMIGRID_H_ 


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
    
#define MMIGRID_SLIDE_TIME_INTERVAL 20      //40MS
#define MMIGRID_SLIDE_ACCELERATION_DAMP 4      //n��˥����tpup��Ļ���ʱ����tp move��n��
    
typedef int32 (*display_function)(int32 index, GUI_RECT_T *rect, GUI_LCD_DEV_INFO *lcd_dev_info);
    
    
//ƽ̨��ʾ��صĽṹ����
typedef struct
{    
    MMI_WIN_ID_T      win_id;
    GUI_LCD_DEV_INFO lcd_dev_info; //lcd dev info  
    //��������£����2��������ͬ�����ǻ���ʱ��ͬ
    GUI_RECT_T      display_rect;       //display rect  ��ʾʱ�����򣬱��绬���϶�ʱ����layout_rect��ֵ�ͻ᲻ͬ
    GUI_RECT_T      layout_rect;        //�ռ����ʱ������ rect ���ж�TP�������Ƿ���item��Χ��ʱ����display_rect
    display_function   draw   ;       //
}MMIGRID_DISPLAY_T;

// FSM state 
typedef enum
{
    MMI_FSM_STATE_NORMAL,
    MMI_FSM_STATE_EDITING,
    MMI_FSM_STATE_SLIDING,
    MMI_FSM_STATE_ITEM_MOVING,
}MMIGRID_FSM_STATE_E;

// ����Ļ�������
typedef enum
{
    MMI_SLIDE_DIRECTION_NONE,       //��������
    MMI_SLIDE_DIRECTION_X = 1,          //x ˮƽ����
    MMI_SLIDE_DIRECTION_Y = 2,           //y ���򻬶�
    MMI_SLIDE_DIRECTION_BOTH = 3,        //xy���򻪶�
}MMIGRID_SLIDE_DIRECTION_E;


//ƽ̨��ʾ��صĽṹ����
typedef struct
{    
    MMIGRID_SLIDE_DIRECTION_E  allowed_slide_direction;
    MMK_TP_SLIDE_E     slide_state;         //normal     page_sliding
    MMI_MESSAGE_ID_E    tp_state ;             //tp down/up��
    GUI_POINT_T         prevous_tp_point ;      //��һ��TP��Ϣ������  
    uint8              slide_timer_id ;        //����ʱ��ID
}MMIGRID_SLIDE_T;

typedef struct 
{
    int32 type;
    int32 data;
}MMIGRID_DATA_T;

typedef struct 
{
    GUI_POINT_T point[4];
    int32 time[4];
    int32 point_num;
    int32 max_num;      
    int32 acceleration;
    int32 velocity;
    int32 step;
}MMIGRID_PONIT_TRACK_T;


//grid struct 
typedef struct
{
    GUI_RECT_T rect;
    int32 row;
    int32 col;
    int32 grid_width;
    int32 grid_height;
    int32 grid_x_interval;
    int32 grid_y_interval;
    MMIGRID_DATA_T *p_grid_data;
    MMIGRID_DISPLAY_T    display;
    MMIGRID_SLIDE_T      slide;
    MMIGRID_PONIT_TRACK_T track;
}MMIGRID_T;


PUBLIC GUI_RECT_T MMIGRID_GetOneGridRect(MMIGRID_T * p_grid, int32 grid_index);

PUBLIC int32 MMIGRID_GetPointLocation(MMIGRID_T * p_grid, GUI_POINT_T point);


//PUBLIC int32 MMIGRID_SetData(MMIGRID_T * p_grid,int32 grid_index, MMIGRID_DATA_T *p_data);

PUBLIC int32 MMIGRID_ClearData(MMIGRID_T * p_grid,int32 grid_index);

PUBLIC int32 MMIGRID_ClearAllData(MMIGRID_T * p_grid);

//get  offset of grids that their index offset = grid_offset
PUBLIC int32  MMIGRID_GetOneGridPosition(MMIGRID_T* p_grid ,
    int32*   offset_x,   int32*   offset_y,
    int32  grid_offset
    );            


//���ڴ�����,ֻ����tp msg
PUBLIC MMI_RESULT_E MMIGRID_HandleGridMsg(MMIGRID_T*     p_grid ,
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
    );


PUBLIC int32 MMIGRID_CreateDisplayLayer( MMIGRID_DISPLAY_T  *display);

PUBLIC int32 MMIGRID_DestroyDisplayLayer( MMIGRID_DISPLAY_T  *display);

PUBLIC int32 MMIGRID_AppendDisplayLayer( MMIGRID_DISPLAY_T  *display);

PUBLIC int32 MMIGRID_RemoveDisplayLayer( MMIGRID_DISPLAY_T  *display);

PUBLIC int32 MMIGRID_RemoveDisplay( MMIGRID_DISPLAY_T  *display);

PUBLIC int32 MMIGRID_MoveDisplayRect( MMIGRID_DISPLAY_T  *display, int32 offset_x, int32 offset_y);

PUBLIC int32 MMIGRID_SetDisplayDisplayRect( MMIGRID_DISPLAY_T  *display, GUI_RECT_T *rect);

PUBLIC int32 MMIGRID_SetDisplayLayoutRect( MMIGRID_DISPLAY_T  *display, GUI_RECT_T *rect);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif

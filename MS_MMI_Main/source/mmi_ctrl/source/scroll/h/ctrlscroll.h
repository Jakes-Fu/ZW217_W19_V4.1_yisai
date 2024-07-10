/*****************************************************************************
** File Name:      ctrlscroll.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/


/*
在我们看来，滚动条大致有以下四种情形，按照thumb_only和is_thin来划分
1 full - 具备箭头、背景和滑块，可操作。thumb_only = FALSE, is_thin = FALSE
2 full thin - 具备背景和滑块，不可操作。thumb_only = FALSE, is_thin = TRUE
3 thumb - 只有滑块，可操作。thumb_only = TRUE, is_thin = FALSE
3 thumb thin - 只有滑块，不可操作。thumb_only = TRUE, is_thin = TRUE
*/


#ifndef _CTRLSCROLL_H_
#define _CTRLSCROLL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlscroll_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// scroll type info
#define CTRL_SCROLL_TYPE     (SCROLL_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// scroll init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
    //none
} CTRLSCROLL_INIT_PARAM_T;


typedef enum
{
    CTRLSCROLL_STYLE_FULL,
    CTRLSCROLL_STYLE_FULL_THIN,
    CTRLSCROLL_STYLE_THUMB,
    CTRLSCROLL_STYLE_THUMB_THIN,
} CTRLSCROLL_STYLE_T;


typedef enum
{
    CTRLSCROLL_REGION_NONE,
    CTRLSCROLL_REGION_TOP,
    CTRLSCROLL_REGION_UP,
    CTRLSCROLL_REGION_THUMB,
    CTRLSCROLL_REGION_DOWN,
    CTRLSCROLL_REGION_BOTTOM,
} CTRLSCROLL_REGION_T;


//scroll control
typedef struct _scroll_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    BOOLEAN is_dirty;
    BOOLEAN is_moved;

    // self member ...
    uint32 total_size;
    uint32 line_size;
    uint32 page_size;
    uint32 thumb_min_size;
    uint32 thumb_height;
    uint32 top_of_current_page;

    BOOLEAN is_thin;
    BOOLEAN is_thumb_only;
    CTRLSCROLL_STYLE_T style;

    //auto set thin - 非only thumb状态，这个值是没有意义的
    BOOLEAN is_auto_thin_disable;
    uint8 auto_thin_page_count;

    //auto hide setting... - 通常仅用于only thumb状态
    BOOLEAN is_auto_hide;
    CAF_LCD_DEV_INFO hide_layer;
    uint8 hide_timer;
    uint32 hide_start_tick;
    uint32 hide_tick;
    uint8 alpha_change_step;
    uint8 layer_alpha;
    

    //arrow setting
    GUI_RECT_T top_arrow_rect;
    GUI_RECT_T bottom_arrow_rect;
    MMI_IMAGE_ID_T top_arrow_img;
    MMI_IMAGE_ID_T top_arrow_pressed_img;
    MMI_IMAGE_ID_T bottom_arrow_img;
    MMI_IMAGE_ID_T bottom_arrow_pressed_img;

    //region setting
    GUI_RECT_T region_rect;
    MMI_IMAGE_ID_T region_bg_img;

    //thrumb setting
    GUI_RECT_T thumb_rect;
    MMI_IMAGE_ID_T thumb_img;

    //tp control
    CTRLSCROLL_REGION_T tp_down_region;
    CTRLSCROLL_REGION_T tp_long_region;
    BOOLEAN is_tp_move;
    BOOLEAN is_tp_long;
    int16 on_thumb_y;
    GUI_POINT_T last_tp_point;

    //timer
    uint8 tp_long_timer;
    uint32 tp_long_tick;

} CTRLSCROLL_OBJ_T;

// scroll class for public or protected method
typedef struct _scroll_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
    //none
} CTRLSCROLL_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get scroll type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SCROLL_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

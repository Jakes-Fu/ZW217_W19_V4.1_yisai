/*****************************************************************************
** File Name:      ctrlslider.h                                               *
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

#ifndef _CTRLSLIDER_H_
#define _CTRLSLIDER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlslider_export.h"

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
// slider type info
#define CTRL_SLIDER_TYPE     (SLIDER_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// slider init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
} CTRLSLIDER_INIT_PARAM_T;

// slider init param
typedef struct
{
    BOOLEAN                     is_true;
    GUI_COLOR_T                 color;
    GUI_COLOR_T                 disable_color;
    MMI_IMAGE_ID_T              image_id;
    MMI_IMAGE_ID_T              disable_image_id;

} CTRLSLIDER_IMAGE_T;

//slider control
typedef struct _slider_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    BOOLEAN                     is_dirty;

    int16                       channel_width;
    int16                       channel_height;
    GUI_RECT_T                  channel_rect;

    int16                       thumb_width;
    int16                       thumb_height;
    GUI_RECT_T                  thumb_rect;

    int16                       max;
    int16                       min;
    int16                       cur;
    int16                       line_size;
    int16                       page_size;
    int16                       tp_pos;

    BOOLEAN                     is_vertical;

    BOOLEAN                     is_disp_bg;
    CTRLSLIDER_IMAGE_T          channel_bg_info;
    CTRLSLIDER_IMAGE_T          channel_fg_info;
    CTRLSLIDER_IMAGE_T          thumb_info;

    BOOLEAN                     is_thumb_pressed;
    GUI_POINT_T                 tp_down_point;
    GUI_POINT_T                 tp_pre_point;
    uint8                       slider_timer_id;
    BOOLEAN                     is_slide_negative; // ÊÇ·ñ¸ºÏòslide

    // self member ...
} CTRLSLIDER_OBJ_T;

// slider class for public or protected method
typedef struct _slider_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLSLIDER_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get slider type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SLIDER_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

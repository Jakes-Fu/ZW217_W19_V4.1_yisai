/*****************************************************************************
** File Name:      ctrllabel.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/

#ifndef _CTRLLABEL_H_
#define _CTRLLABEL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmitheme_label.h"

#include "ctrlbase.h"
#include "ctrllabel_export.h"

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
// label type info
#define CTRL_LABEL_TYPE     (LABEL_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// label init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    GUILABEL_ALIGN_E            align;      /*!<ÅÅÁÐ·½Ê½*/
} CTRLLABEL_INIT_PARAM_T;

//label control
typedef struct _label_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    BOOLEAN                     is_active;          //label is active
    BOOLEAN                     is_auto_scroll;   //string is auto scroll if string width > diaplay width whether label is active or not
    BOOLEAN                     is_ellipsis_ex;    //string need ellipsis

    uint8                       scroll_timer_id;    //scroll timer id
    uint16                      str_index;          //string start index

    wchar                       *str_ptr;           //current string
    uint16                      str_len;            //string length
    MMI_IMAGE_ID_T              icon_id;            //icon id
    BOOLEAN                     is_bit_map;
    GUILABEL_IMAGE_TYPE_E       img_type;
    GUILABEL_DATA_IMAGE_U       img_data;
    GUILABEL_ALIGN_E            align;              //align

    GUI_RECT_T                  tp_rect;            //only receive tp msg

    uint16                      region_num;         //region num
    GUIFONT_REGION_STYLE_T      *region_ptr;        //region

    GUI_COLOR_T                 *bg_buf_ptr;        //save label bg

    MMITHEME_LABEL_T            theme;              //label theme
    
    MMI_HANDLE_T                win_handle;         //window handle
} CTRLLABEL_OBJ_T;

// label class for public or protected method
typedef struct _label_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

} CTRLLABEL_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get label type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T LABEL_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

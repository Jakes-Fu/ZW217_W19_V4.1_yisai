/*****************************************************************************
** File Name:      ctrllettersbar.h                                               *
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

#ifndef _CTRLLETTERSBAR_H_
#define _CTRLLETTERSBAR_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrllettersbar_export.h"

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
// lettersbar type info
#define CTRL_LETTERSBAR_TYPE     (LETTERSBAR_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// lettersbar init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
} CTRLLETTERSBAR_INIT_PARAM_T;

//lettersbar control
typedef struct _lettersbar_obj_tag
{
    CTRLBASE_OBJ_T      base_ctrl;

    BOOLEAN             is_disp_bg;
    BOOLEAN             is_dirty;
    BOOLEAN             is_disp_own;

    // about letters
    GUI_FONT_T          letter_font;
    GUI_FONT_T          letter_font_color;
    wchar               *letters_ptr;
    int16               *letter_postion_table;     /*<每个字母的y坐标>*/
    uint16              letters_num;
    uint16              max_disp_num;
    uint16              cur_index;

    uint16              letter_scale;

    BOOLEAN             is_tp_down;

} CTRLLETTERSBAR_OBJ_T;

// lettersbar class for public or protected method
typedef struct _lettersbar_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLLETTERSBAR_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get lettersbar type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T LETTERSBAR_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*****************************************************************************
** File Name:      ctrltimepicker.h                                               *
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

#ifndef _CTRLTIMEPICKER_H_
#define _CTRLTIMEPICKER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbasepicker.h"
#include "ctrltimepicker_export.h"
#include "mmitheme_picker.h"

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
// timepicker type info
#define CTRL_TIMEPICKER_TYPE     (TIMEPICKER_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// timepicker init param
typedef struct
{
    CTRLBASEPICKER_INIT_PARAM_T         basepicker_ctrl_param;

    // self inif param ...
} CTRLTIMEPICKER_INIT_PARAM_T;

//timepicker control
typedef struct _timepicker_obj_tag
{
    CTRLBASEPICKER_OBJ_T                basepicker_ctrl;

    MMITHEME_TIMEPICKER_T               theme;

    // self member ...
} CTRLTIMEPICKER_OBJ_T;

// timepicker class for public or protected method
typedef struct _timepicker_vtbl_tag
{
    CTRLBASEPICKER_VTBL_T               basepicker_ctrl_vtbl;

    // self vtbl ...
} CTRLTIMEPICKER_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get timepicker type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TIMEPICKER_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

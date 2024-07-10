/*****************************************************************************
** File Name:      ctrldatepicker.h                                               *
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

#ifndef _CTRLDATEPICKER_H_
#define _CTRLDATEPICKER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbasepicker.h"
#include "ctrldatepicker_export.h"
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
// datepicker type info
#define CTRL_DATEPICKER_TYPE     (DATEPICKER_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// datepicker init param
typedef struct
{
    CTRLBASEPICKER_INIT_PARAM_T         basepicker_ctrl_param;

    // self inif param ...
} CTRLDATEPICKER_INIT_PARAM_T;

//datepicker control
typedef struct _datepicker_obj_tag
{
    CTRLBASEPICKER_OBJ_T                basepicker_ctrl;

    MMITHEME_DATEPICKER_T               theme;

    // self member ...
} CTRLDATEPICKER_OBJ_T;

// datepicker class for public or protected method
typedef struct _datepicker_vtbl_tag
{
    CTRLBASEPICKER_VTBL_T               basepicker_ctrl_vtbl;

    // self vtbl ...
} CTRLDATEPICKER_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get datepicker type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T DATEPICKER_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

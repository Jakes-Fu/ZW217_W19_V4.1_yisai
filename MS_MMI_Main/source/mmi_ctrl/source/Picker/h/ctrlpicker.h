/*****************************************************************************
** File Name:      ctrlpicker.h                                               *
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

#ifndef _CTRLPICKER_H_
#define _CTRLPICKER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbasepicker.h"
#include "ctrlpicker_export.h"
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
// picker type info
#define CTRL_PICKER_TYPE     (PICKER_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// picker init param
typedef struct
{
    CTRLBASEPICKER_INIT_PARAM_T         basepicker_ctrl_param;

    // self inif param ...
} CTRLPICKER_INIT_PARAM_T;

//picker control
typedef struct _picker_obj_tag
{
    CTRLBASEPICKER_OBJ_T                basepicker_ctrl;

    MMITHEME_PICKER_T                   theme;

    MMI_STRING_T                        *content_ptr;
    uint16                              content_size;

    // self member ...
} CTRLPICKER_OBJ_T;

// picker class for public or protected method
typedef struct _picker_vtbl_tag
{
    CTRLBASEPICKER_VTBL_T               basepicker_ctrl_vtbl;

    // self vtbl ...
} CTRLPICKER_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get picker type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T PICKER_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

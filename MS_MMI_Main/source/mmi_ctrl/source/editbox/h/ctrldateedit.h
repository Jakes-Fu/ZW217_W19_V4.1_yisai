/*****************************************************************************
** File Name:      ctrldateedit.h                                               *
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

#ifndef _CTRLDATEEDIT_H_
#define _CTRLDATEEDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbasefixed.h"
#include "ctrldateedit_export.h"

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
// dateedit type info
#define CTRL_DATEEDIT_TYPE     (DATEEDIT_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// dateedit init param
typedef struct
{
    CTRLBASEFIXED_INIT_PARAM_T       basefixed_ctrl_param;

    // self inif param ...
} CTRLDATEEDIT_INIT_PARAM_T;

//dateedit control
typedef struct
{
    CTRLBASEFIXED_OBJ_T             basefixed_ctrl;

    uint8                           month;      /*!<月*/
    uint8                           day;        /*!<天*/
    uint16                          year;       /*!<年*/
    GUIEDIT_DATE_YEAR_RANGE_E       year_range; /*!<年范围*/

    MMITHEME_EDIT_DATE_T            theme;       //date theme
} CTRLDATEEDIT_OBJ_T;

// dateedit class for public or protected method
typedef struct
{
    CTRLBASEFIXED_VTBL_T            basefixed_ctrl_vtbl;

    // self vtbl ...
} CTRLDATEEDIT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get dateedit type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T DATEEDIT_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

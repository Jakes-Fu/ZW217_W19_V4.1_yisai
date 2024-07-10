/*****************************************************************************
** File Name:      ctrltimeedit.h                                               *
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

#ifndef _CTRLTIMEEDIT_H_
#define _CTRLTIMEEDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbasefixed.h"
#include "ctrltimeedit_export.h"

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
// timeedit type info
#define CTRL_TIMEEDIT_TYPE     (TIMEEDIT_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// timeedit init param
typedef struct
{
    CTRLBASEFIXED_INIT_PARAM_T      basefixed_ctrl_param;

    // self inif param ...
} CTRLTIMEEDIT_INIT_PARAM_T;

//timeedit control
typedef struct
{
    CTRLBASEFIXED_OBJ_T             basefixed_ctrl;

    uint8                           hour;   /*!<24小时制小时*/
    uint8                           minute; /*!<分*/
    uint8                           second; /*!<秒*/

    MMITHEME_EDIT_TIME_T            theme;       //time theme
} CTRLTIMEEDIT_OBJ_T;

// timeedit class for public or protected method
typedef struct
{
    CTRLBASEFIXED_VTBL_T            basefixed_ctrl_vtbl;

    // self vtbl ...
} CTRLTIMEEDIT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get timeedit type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TIMEEDIT_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

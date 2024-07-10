/*****************************************************************************
** File Name:      ctrldigitaledit.h                                               *
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

#ifndef _CTRLDIGITALEDIT_H_
#define _CTRLDIGITALEDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseflex.h"
#include "ctrldigitaledit_export.h"

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
// digitaledit type info
#define CTRL_DIGITALEDIT_TYPE     (DIGITALEDIT_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// digitaledit init param
typedef struct
{
    CTRLBASEFLEX_INIT_PARAM_T       baseflex_ctrl_param;

    // self inif param ...
} CTRLDIGITALEDIT_INIT_PARAM_T;

//digitaledit control
typedef struct
{
    CTRLBASEFLEX_OBJ_T              baseflex_ctrl;

    MMITHEME_EDIT_TEXT_T            theme;    //digital theme
} CTRLDIGITALEDIT_OBJ_T;

// digitaledit class for public or protected method
typedef struct
{
    CTRLBASEFLEX_VTBL_T             baseflex_ctrl_vtbl;

    // self vtbl ...
} CTRLDIGITALEDIT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get digitaledit type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T DIGITALEDIT_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

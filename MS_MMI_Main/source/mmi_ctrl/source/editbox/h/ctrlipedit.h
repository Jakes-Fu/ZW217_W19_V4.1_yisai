/*****************************************************************************
** File Name:      ctrlipedit.h                                               *
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

#ifndef _CTRLIPEDIT_H_
#define _CTRLIPEDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbasefixed.h"
#include "ctrlipedit_export.h"

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
// ipedit type info
#define CTRL_IPEDIT_TYPE     (IPEDIT_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// ipedit init param
typedef struct
{
    CTRLBASEFIXED_INIT_PARAM_T      basefixed_ctrl_param;

    // self inif param ...
} CTRLIPEDIT_INIT_PARAM_T;

//ipedit control
typedef struct
{
    CTRLBASEFIXED_OBJ_T             basefixed_ctrl;

    uint32                          ip_addr;    /*!<IPµØÖ·*/

    MMITHEME_EDIT_IP_T              theme;         //IP theme
} CTRLIPEDIT_OBJ_T;

// ipedit class for public or protected method
typedef struct
{
    CTRLBASEFIXED_VTBL_T            basefixed_ctrl_vtbl;

    // self vtbl ...
} CTRLIPEDIT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get ipedit type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T IPEDIT_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

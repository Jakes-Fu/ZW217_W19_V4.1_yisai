/*****************************************************************************
** File Name:      ctrlmainmenu.h                                               *
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

#ifndef _CTRLMAINMENU_H_
#define _CTRLMAINMENU_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlmainmenu_export.h"
#include "mmitheme_mainmenu.h"

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
// mainmenu type info
#define CTRL_MAINMENU_TYPE     (MAINMENU_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//typedef CTRLMAINMENU_CALLBACK_T CTRLMAINMENU_CALLBACK_T;

// mainmenu init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    MMIMAINMENU_TYPE_E          type;
    // self inif param ...
} CTRLMAINMENU_INIT_PARAM_T;


//mainmenu control
typedef struct
{
    CTRLBASE_OBJ_T              base_ctrl;

    MMIMAINMENU_TYPE_E          type;
    CTRLMAINMENU_PM_APIS_T      call_back;
    void                        *pm_data_ptr;

    // self member ...
} CTRLMAINMENU_OBJ_T;

// mainmenu class for public or protected method
typedef struct
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLMAINMENU_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get mainmenu type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T MAINMENU_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

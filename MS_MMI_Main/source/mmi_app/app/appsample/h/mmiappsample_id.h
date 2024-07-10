/*****************************************************************************
** File Name:      appsample_id.h                                            *
** Author:                                                                   *
** Date:
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe cc win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/6/30     sam.hua           Create                                    *
******************************************************************************/

#ifndef _APPSAMPLE_ID_H_
#define _APPSAMPLE_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmiappsample_define.h"

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


    /**--------------------------------------------------------------------------*
     **                         TYPE AND CONSTANT                                *
     **--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id)          win_id

    typedef enum
    {
        MMI_APPSAMPLE_WIN_ID_START = (MMI_MODULE_APPSAMPLE << 16),

#include "mmiappsample_id.def"

        MMIAPPSAMPLE_MAX_WIN_ID
    }
    MMIAPPSAMPLE_WINDOW_ID_E;

#undef WIN_ID_DEF

    typedef enum
    {
        MMIAPPSAMPLE_CTRL_ID_START = MMIAPPSAMPLE_MAX_WIN_ID,

        MMIAPPSAMPLE_MAINWIN_LIST_CTRL_ID,
        MMIAPPSAMPLE_MAINWIN_POPMENU_CTRL_ID,

        MMIAPPSAMPLE_FUNCWINLIST_LIST_CTRL_ID,
        MMIAPPSAMPLE_FUNCWINMODEA_LIST_CTRL_ID,
        MMIAPPSAMPLE_FUNCWINMODEB_LIST_CTRL_ID,
        MMIAPPSAMPLE_FUNCWINMODEC_LIST_CTRL_ID,

        MMIAPPSAMPLE_FILTERWIN_LIST_CTRL_ID,

        MMIAPPSAMPLE_WINDATAWIN_LIST_CTRL_ID,

        MMIAPPSAMPLE_EVENTWIN_LIST_CTRL_ID,
        MMIAPPSAMPLE_WINDATAWIN_AGELABEL_CTRL_ID,
        MMIAPPSAMPLE_WINDATAWIN_SEXLABEL_CTRL_ID,


        MMIAPPSAMPLE_MAX_CTRL_ID
    } APPSAMPLE_CONTROL_ID_E;

    /**--------------------------------------------------------------------------*
     **                         FUNCTION DEFINITION                              *
     **--------------------------------------------------------------------------*/



    /**--------------------------------------------------------------------------*
     **                         Compiler Flag                                    *
     **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_APPSAMPLE_ID_H_

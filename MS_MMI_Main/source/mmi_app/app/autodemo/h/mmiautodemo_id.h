/*****************************************************************************
** File Name:      mmiautodemo_id.h                                                *
** Author:                                                                   *
** Date:           2012/03/17                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe ad win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/03/17     chunjie.liu        Create                                    *
******************************************************************************/

#ifndef _MMIAUTODEMO_ID_H_
#define _MMIAUTODEMO_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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
 
 
// window ID
typedef enum
{
    MMI_AD_WIN_ID_START = (MMI_MODULE_AD << 16),
    MMIAD_AUTO_DEMO_WIN_ID,
    MMIAD_DISK_OPT_MENU_WIN_ID,
    MMIAD_MUTI_LAYER_WIN_ID,
    MMIAD_MAX_WIN_ID
}MMIAD_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIAD_CTRL_ID_START = MMIAD_MAX_WIN_ID,//(MMI_MODULE_AD << 16),
//ad
    MMIAD_AUTO_DEMO_LISTBOX_CTRL_ID,	// the listbox control of Autodemo Window
    MMIAD_DISK_OPT_LISTBOX_CTRL_ID,
    MMIAD_MAX_CTRL_ID
}AD_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIAUTODEMO_ID_H_

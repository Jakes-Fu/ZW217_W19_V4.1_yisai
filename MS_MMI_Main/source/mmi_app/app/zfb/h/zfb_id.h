/*****************************************************************************
** File Name:      mmicc_id.h                                                *
** Author:                                                                   *
** Date:           2006/09/26                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe cc win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/26     kelly.li        Create                                    *
******************************************************************************/

#ifndef _MMIZFB_ID_H_
#define _MMIZFB_ID_H_ 

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
 
#define WIN_ID_DEF(win_id, win_id_name)          win_id,
 
// window ID
typedef enum
{
    MMI_ZFB_WIN_ID_START = (MMI_MODULE_ZFB << 16),

#include "zfb_id.def"

    MMIZFB_MAX_WIN_ID
}MMIZFB_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIZFB_CTRL_ID_START = MMIZFB_MAX_WIN_ID,//(MMI_MODULE_CC << 16),
    MMIZFB_MENU_CTRL_ID,
    MMIZFB_LIST_CTRL_ID,
    MMIZFB_LABLE_CTRL_ID,
    MMIZFB_MAX_CTRL_ID
}ZFB_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISET_ID_H_

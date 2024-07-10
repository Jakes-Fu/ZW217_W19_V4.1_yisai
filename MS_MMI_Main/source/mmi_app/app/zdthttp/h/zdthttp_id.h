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

#ifndef _MMIZDTHTTP_ID_H_
#define _MMIZDTHTTP_ID_H_ 

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
    MMI_ZDTHTTP_WIN_ID_START = (MMI_MODULE_ZDTHTTP << 16),

#include "zdthttp_id.def"

    MMIZDTHTTP_MAX_WIN_ID
}MMIZDTHTTP_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIZDTHTTP_CTRL_ID_START = MMIZDTHTTP_MAX_WIN_ID,//(MMI_MODULE_CC << 16),
    MMIZDTHTTP_TEST_MENU_CTRL_ID,
    MMIZDTHTTP_MAX_CTRL_ID
}ZDTHTTP_CONTROL_ID_E;

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

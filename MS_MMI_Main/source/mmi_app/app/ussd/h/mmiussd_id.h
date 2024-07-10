/*****************************************************************************
** File Name:      mmiussd_id.h                                                *
** Author:                                                                   *
** Date:           2012/01/03                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe USSD win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/01/03              Create                                    *
******************************************************************************/
#ifndef _MMIUSSD_ID_H_
#define _MMIUSSD_ID_H_ 

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
#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    MMIUSSD_WIN_ID_START = (MMI_MODULE_USSD << 16),

#include "mmiussd_id.def"

    MMIUSSD_MAX_WIN_ID
}MMIUSSD_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIUSSD_CTRL_ID_START = MMIUSSD_MAX_WIN_ID,//(MMI_MODULE_USSD << 16),

//ussd
    MMIUSSD_INPUT_EDITBOX_CTRL_ID,		//ussd showdata editbox control id
    MMIUSSD_TEXTBOX_CTRL_ID,

    MMIUSSD_MAX_CTRL_ID
}MMIUSSD_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIUSSD_ID_H_


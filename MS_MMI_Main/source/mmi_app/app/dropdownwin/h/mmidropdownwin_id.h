/*****************************************************************************
** File Name:      mmidropdownwin_id.h                                       *
** Author:                                                                   *
** Date:           2011/05/29                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe dropdown win and control id *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011/05/29     Paul.Huang        Create                                   *
******************************************************************************/

#ifndef _MMIDROPDOWNWIN_ID_H_
#define _MMIDROPDOWNWIN_ID_H_ 

#ifdef PDA_UI_DROPDOWN_WIN

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
    MMIDROPDOWN_WIN_ID_START = (MMI_MODULE_DROPDOWNWIN << 16),

#include "mmidropdownwin_id.def"

    MMIDROPDOWN_MAX_WIN_ID
}MMIDROPDOWN_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIDROPDOWN_CTRL_ID_START = MMIDROPDOWN_MAX_WIN_ID, 

    MMIDROPDOWN_LIST_CTRL_ID,	

   MMIDROPDOWN_MAX_CTRL_ID
}DROPDOWN_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Register dropdown win id
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDROPDOWNWIN_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //PDA_UI_DROPDOWN_WIN

#endif 


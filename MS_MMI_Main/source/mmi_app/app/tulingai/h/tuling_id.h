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

#ifndef _MMI_TULING_ID_H_
#define _MMI_TULING_ID_H_ 

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
    MMI_AI_CHAT_WIN_ID_START = (MMI_MODULE_TULINGAI << 16),

#include "tuling_id.def"


    MMI_TULING_MAX_WIN_ID
}MMI_TULING_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMI_TULING_CTRL_ID_START = MMI_TULING_MAX_WIN_ID,//(MMI_MODULE_CC << 16),
    //MMI_TULING_TEST_MENU_CTRL_ID,
    MMIAI_CHAT_LIST_CTRL_ID,
    MMI_TULING_MAX_CTRL_ID
}AI_CHAT_CONTROL_ID_E;

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

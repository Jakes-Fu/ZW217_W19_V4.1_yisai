/*****************************************************************************
** File Name:      mmiset_id.h                                               *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting win and control id   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18     kelly.li        Create                                    *
******************************************************************************/

#ifndef _MMI_VIDEO_CALL_ID_H_
#define _MMI_VIDEO_CALL_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                     *
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
    VIDEO_CALL_WIN_ID_START = (MMI_MODULE_VIDEO_CALL << 16),

#include "video_call_id.def"

    VIDEO_CALL_MAX_WIN_ID
}VIDEO_CALL_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    VIDEO_CALL_CTRL_ID_ID_START = VIDEO_CALL_MAX_WIN_ID, //,
    VIDEO_CALL_CONTACT_LIST_CTRL_ID,
    VIDEO_CALL_HANGUP_BTN_ID,
    VIDEO_CALL_ACCEPT_BTN_ID,

    VIDEO_CALL_MAX_CTRL_ID
}VIDEO_CALL_CONTROL_ID_E;


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISET_ID_H_

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

#ifndef WATCH_NOTIFYCENTER_ID_H
#define WATCH_NOTIFYCENTER_ID_H

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
    WATCH_NOTIFYCENTER_WIN_ID_START = (WATCH_MODULE_NOTIFYCENTER << 16),

#include "watch_notifycenter_id.def"

    WATCH_NOTIFYCENTER_MAX_WIN_ID
}WATCH_NOTIFYCENTER_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    WATCH_NOTIFYCENTER_CONTROL_ID_START = WATCH_NOTIFYCENTER_MAX_WIN_ID,

    NOTIFY_LIST_CTRL_ID,
    NOTIFY_LABEL_CTRL_ID,
	NOTIFY_TITLE_CTRL_ID,
	NOTIFY_IMAGE_SOFTKEY_CTRL_ID,
	NOTIFY_TEXTBOX_CTRL_ID,
	NOTIFY_DETAIL_LABEL_CTRL_ID,
    WATCH_NOTIFYCENTER_MAX_CONTROL_ID
}WATCH_NOTIFYCENTER_CONTROL_ID_E;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Register notifycenter win id
//  Global resource dependence : none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_NotifyCenter_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif 


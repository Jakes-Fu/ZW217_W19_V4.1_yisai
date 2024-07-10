/*****************************************************************************
** File Name:      tp_id.h                                                   *
** Author:                                                                   *
** Date:           11/17/2005                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2005       wancan.you       Create
******************************************************************************/

#ifndef _TP_ID_H_
#define _TP_ID_H_ 

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
// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
    TP_WIN_ID_START = (MMI_MODULE_TP << 16),

#include "tp_id.def"

    TP_MAX_WIN_ID
}TP_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
// 命名规则: 控件所属的窗口名 + 控件名 + CTRL_ID, eg: IDLEWIN_TEXTBOX_CTRL_ID (IDLE窗口下text控件ID)
typedef enum
{
    TP_CTRL_ID_ID_START = TP_MAX_WIN_ID, //(MMI_MODULE_TP << 16),
    TP_COORDINATE_PROMPT_CTRL_ID,
    IM_MAX_CTRL_ID
}TP_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


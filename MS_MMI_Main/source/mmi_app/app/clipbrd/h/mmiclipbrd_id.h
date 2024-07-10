/***************************************************************************
** File Name:      mmiclipbrd_id.h                                         *
** Author:         hua.fang                                                *
** Date:           06/05/2010                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2010        hua.fang         Create                                  *
**																		   *
****************************************************************************/

#ifndef _MMICLIPBRD_ID_H_
#define _MMICLIPBRD_ID_H_

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
    MMICLIPBRD_START_WIN_ID = (MMI_MODULE_CLIPBRD << 16),

#include "mmiclipbrd_id.def"

    MMICLIPBRD_END_WIN_ID
} MMICLIPBRD_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMICLIPBRD_CTRL_ID_START = MMICLIPBRD_END_WIN_ID,

    MMICLIPBRD_MENU_CTRL_ID,

    MMICLIPBRD_CUT_BUTTON_CTRL_ID,
    MMICLIPBRD_COPY_BUTTON_CTRL_ID,
    MMICLIPBRD_PASTE_BUTTON_CTRL_ID,
    MMICLIPBRD_SELALL_BUTTON_CTRL_ID,
    MMICLIPBRD_SELECT_BUTTON_CTRL_ID,

    MMICLIPBRD_MAX_CTRL_ID
} MMICLIPBRD_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register clipboard win
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void MMICLIPBRD_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMICLIPBRD_ID_H_

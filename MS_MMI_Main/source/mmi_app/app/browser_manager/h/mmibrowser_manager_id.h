/*****************************************************************************
** File Name:      mmibrowser_manager_id.h                                   *
** Author:          fen.xie                                                  *
** Date:           01/11/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser id              *
*****************************************************************************/
#ifndef MMIBROWSER_MANAGER_ID_H_
#define MMIBROWSER_MANAGER_ID_H_

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
#define WIN_ID_DEF(win_id)          win_id

// window ID
typedef enum
{
    MMI_BM_BEGIN_WIN_ID = (MMI_MODULE_MMIBROWSER_MANAGER<< 16),                   // begin

#include "mmibrowser_manager_id.def"

	MMIBM_WINDOW_ID_NUMBER          // 窗口ID的总数目
}MMIBM_WINDOW_ID_E;

#undef WIN_ID_DEF
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMIBM_CONTROL_ID = MMIBM_WINDOW_ID_NUMBER,
    //MMIBM_SELECT_NERWORK_CTRL_ID,
    MMIBM_SELECT_BROWSER_FORM_CTRL_ID,
    MMIBM_SELECT_BROWSER_LIST1_CTRL_ID,
    MMIBM_SELECT_BROWSER_LIST2_CTRL_ID,
    
    MMIBM_SET_BROWSER_FORM_CTRL_ID,
    MMIBM_SET_BROWSER_LIST_CTRL_ID,
    
    MMIBM_CC_SELECTION_MENU_CTRL_ID,
    
    MMIBM_CONTROL_ID_NUMBER
}MMIBM_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*****************************************************************************
** File Name:      mmibrowser_manager_wintable.h                             *
** Author:          fen.xie                                                  *
** Date:           01/11/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/
#ifndef MMIBROWSER_MANAGER_WINTABLE_H
#define MMIBROWSER_MANAGER_WINTABLE_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "mmi_module.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmipub.h"
#include "mmi_signal_ext.h"
#include "mmifmm_export.h"
#include "mmibrowser_manager_func.h"

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
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open the browser type select window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_OpenBrowserSelectWin(void);
/*****************************************************************************/
//  Description : close the browser select window
//  Global resource dependence : 
//  Author: jinju.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_CloseBrowserSelectWin(void);
#if defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT)
/*****************************************************************************/
//  Description : Create CC select win
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBM_OpenCCSelWin(MMIBM_CC_INFO_T *in_cc_info_ptr);
#endif
#ifdef  CMCC_UI_STYLE
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBM_QueryOpenUrlWin(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

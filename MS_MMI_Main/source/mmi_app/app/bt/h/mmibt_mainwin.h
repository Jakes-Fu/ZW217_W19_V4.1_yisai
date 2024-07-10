/****************************************************************************
** File Name:      mmibt_mainwin.h                                         *
** Author:         yuantao xu                                            *
** Date:           08/06/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** Aug/06/2012    yuantao.xu     Create                                  *
**                                                                         *
****************************************************************************/

#ifndef _MMIBT_MAINWIN_H_    
#define  _MMIBT_MAINWIN_H_    

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "guilistbox.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmibt_export.h"
#include "mmifmm_export.h"
#include "mmibt_id.h"
#include "mmibt_menutable.h"
#include "mmifilearray_export.h"
#include "mmibt_nv.h"
#include "bt_abs.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//New UI
/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
 #ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: 
//  available
//  Global resource dependence: none 
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_UpdateMainMenu(BOOLEAN is_init);
/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void AppendOneLineTextListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            ); 
#endif
/*****************************************************************************/
//  Discription: Open the blue tooth main menu win  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpenMainMenuWin(void);
/*****************************************************************************/
// Description : 蓝牙开关设置页面
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIBT_SetBluetoothMode (void);

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_UpdateBtPhoneMenu
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIBT_UpdateBtPhoneWin(BOOLEAN is_update);

#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_CreateTimer
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 创建超时定时器
/*****************************************************************************/
PUBLIC void MMIBT_CreatePbapSyncTimer(void);

/*****************************************************************************/
//  Description : MMIBT_CreateTimer
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 创建超时定时器
/*****************************************************************************/
PUBLIC void MMIBT_CreatePbapSyncTimer(void);

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif


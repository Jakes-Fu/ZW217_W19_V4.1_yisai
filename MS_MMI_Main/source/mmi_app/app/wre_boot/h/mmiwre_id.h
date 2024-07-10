/****************************************************************************
** File Name:      mmiwre_id.h                                               *
** Author:                                                                 *
** Date:           03/29/2006                                             *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006        Annie.an        Create
** 
****************************************************************************/
#ifndef  _MMIWRE_ID_H_    
#define  _MMIWRE_ID_H_  
#ifdef WRE_SUPPORT
 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
   extern   "C"
   {
#endif

#define WIN_ID_DEF(win_id)          win_id

// window ID
typedef enum
{
	 WRE_WIN_ID_START = (MMI_MODULE_WRE<< 16),       

#include "mmiwre_id.def"
	 MMI_WIN_ID_DOWNLOAD_WAIT,                       //自定义等待窗口
     WRE_MAX_WIN_ID
}WRE_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{ 
    WRE_CTRL_ID_ID_START = WRE_MAX_WIN_ID, //(MMI_MODULE_WRE << 16),
    IDWRE_MENU_CTRL_ID,
	MMISET_SET_WRE_MENU_CTRL_ID,                   // wre 管理器
	MMIWRE_CTRL_ID_SELECT_SIM_ID,     
	MMIWRE_PIC_PREVIEW_CTRL_ID,
	MMI_DOWNLOAD_ANIM_CTRL_ID, 
    WRE_MAX_CTRL_ID
}WRE_CTRL_ID_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
#endif

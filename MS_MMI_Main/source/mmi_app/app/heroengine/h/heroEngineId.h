/****************************************************************************
** File Name:      heroEngineId.h                                               *
** Author:              qgp                                                   *
** Date:           05/15/2006                                             *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2006        qgp		       Create
** 
****************************************************************************/
#ifndef  _MMI_HERO_ENGINE_ID_H_    
#define  _MMI_HERO_ENGINE_ID_H_  

 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "heroEngineInternal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
   extern   "C"
   {
#endif


//temp. 
//#define	MMI_MODULE_HEROENGINE  (MMI_MODULE_CONNECTION+1)


typedef enum
{

	MMI_HERO_APP_WIN_ID_START = (MMI_MODULE_HEROENGINE << 16),
	MMI_HERO_APP_BASE_WIN_ID,
	MMI_HERO_APP_ACCOUNT_WIN_ID,
#ifdef __HERO_CFG_CAMERA_SUPPORT__
    MMI_HERO_APP_CAMERA_WIN_ID,
#endif
	HERO_APP_TIPS_WIN_ID, //modify 20181129

	HERO_APP_DYN_BASE_WIN_ID,
	HERO_APP_DYN_END_WIN_ID = HERO_APP_DYN_BASE_WIN_ID + HERO_APP_DYN_WIN_ID_MAXNUM,

	HERO_APP_CURRENT_WIN_ID,
	HERO_APP_WEATHER_WIN_ID,

        HERO_APP_SHOWFACTORYINFO_TEXT_WIN_ID,
	HERO_APP_MAX_WIN_ID
}MMI_HERO_APP_WINDOW_ID_E;

// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
  
	MMI_HERO_APP_CTRL_ID_START = HERO_APP_MAX_WIN_ID,//2009 L change, (MMI_MODULE_HEROENGINE << 16),

	HERO_APP_TIPS_TEXT_CTRL_ID, //modify 20181129

	HERO_APP_DYN_BASE_CTRL_ID,
	HERO_APP_ACCOUNT_LISTCTRL_ID,
	HERO_APP_DYN_END_CTRL_ID = HERO_APP_DYN_BASE_CTRL_ID + HERO_APP_DYN_WIN_ID_MAXNUM,

        HERO_APP_SHOWFACTORYINFO_TEXT_CTRL_ID,

	MMIHERO_APP_MAX_CTRL_ID  
}MMI_HERO_APP_CTRL_ID_E;

typedef enum
{
	HERO_APP_ENV_INFO = (MMI_MODULE_HEROENGINE << 16),
	HERO_APP_ENV_INITED,

	NVRAM_EF_HERO_APP_CONFIG_LID, //modify 20181117

	NVRAM_EF_SMART_REMINDER_LID, //modify 20181120 begin

#ifdef HERO_APP_SHORTCUT_SUPPORT
	NVRAM_EF_SHORTCUT_MENU_LID,
#endif

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
	NVRAM_EF_OLALA_CFG_LID,
#endif

	HERO_APP_MAX_NV_ITEM_NUM
}MMI_HERO_APP_NV_ITEM_E;


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif/*_MMI_HERO_ENGINE_ID_H_*/


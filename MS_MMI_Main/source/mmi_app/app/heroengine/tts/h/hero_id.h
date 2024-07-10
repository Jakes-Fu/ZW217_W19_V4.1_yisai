/****************************************************************************
** File Name:      hero_id.h                                               *
** Author:                                                               *
** Date:                                                        *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
****************************************************************************/
#ifndef  _HERO_ID_H_    
#define  _HERO_ID_H_  
#ifdef HERO_ENGINE_TTS_SUPPORT

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

#define HERO_DYN_WIN_ID_MAXNUM            25   //可以取得的最大win id数目
#define HERO_DEFAULT_MAX_LEN             500 		//允许的最大输入值

//temp. 
typedef enum
{
	HERO_WIN_ID_START = (MMI_MODULE_HEROENGINE << 16),
	HERO_BASE_WIN_ID,
	HERO_ACCOUNT_WIN_ID,
	HERO_DYN_BASE_WIN_ID,
	HERO_DYN_END_WIN_ID = HERO_DYN_BASE_WIN_ID + HERO_DYN_WIN_ID_MAXNUM,
	HERO_CURRENT_WIN_ID,
	HERO_WEATHER_WIN_ID,
	
	HERO_MMITEST_WIN_ID,
	HERO_MMITEST_WIN_ID1,
	HERO_MMITEST_WIN_ID2,
	HERO_MMITEST_WIN_ID3,
	HERO_MMITEST_WIN_ID4,
	HERO_INCOMING_CALL_WIN_ID,
	HERO_SMS_ASSISTANT_WIN_ID,
	HERO_AUTO_SMS_WIN_ID,
	HERO_READ_MENU_WIN_ID,
	HERO_READ_MANIMENU_WIN_ID,
	HERO_READ_SECMENU_WIN_ID,
	HERO_AKEY_TIME_WIN_ID,
	HERO_AUTO_TIME_WIN_ID,
	HERO_AUTO_TIME_TIME_QUANTUM_WIN_ID,
	HERO_AUTO_TIME_CUSTOM_MODE_WIN_ID,
	HERO_AKEY_POWER_LEVEL_WIN_ID,
	HERO_DIALOG_WIN_ID,
	
	HERO_MAX_WIN_ID
}HERO_WINDOW_ID_E;

//命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
	HERO_CTRL_ID_START = HERO_MAX_WIN_ID,
	HERO_DYN_BASE_CTRL_ID,
	HERO_ACCOUNT_LISTCTRL_ID,
	
	HERO_CTRL_ID_VOICE_MAIN,
	HERO_CTRL_ID_SELECT_ON_OFF,
	HERO_CTRL_ID_INCOMING_CALL,
	HERO_CTRL_ID_AUTO_SMS,
	HERO_CTRL_ID_SMS_ASSISTANT,
	HERO_CTRL_ID_READ_MENU,
	HERO_CTRL_ID_READ_MAINMENU,
	HERO_CTRL_ID_READ_SECMENU,
	HERO_CTRL_ID_AKEY_TIME,
	HERO_CTRL_ID_AUTO_TIME,
	HERO_CTRL_ID_AUTO_EDIT_START_TIME_EDITBOX,
	HERO_CTRL_ID_AUTO_EDIT_END_TIME_EDITBOX,
	HERO_CTRL_ID_AUTO_START_LABEL,
	HERO_CTRL_ID_AUTO_END_LABEL,
	HERO_CTRL_ID_AUTO_EDIT_START_TIME_FORM,
	HERO_CTRL_ID_AUTO_EDIT_END_TIME_FORM,
	HERO_CTRL_ID_AUTO_TIME_FORM,
	HERO_CTRL_ID_AKEY_POWER_LEVEL,
	HERO_CTRL_ID_DIALOG_TEXTBOX,
	
	HERO_DYN_END_CTRL_ID = HERO_DYN_BASE_CTRL_ID + HERO_DYN_WIN_ID_MAXNUM,
	HERO_MAX_CTRL_ID  
}HERO_CTRL_ID_E;

typedef enum
{
	HERO_ENV_INFO = (MMI_MODULE_HEROENGINE << 16),
	HERO_ENV_INITED,
	HERO_MAX_NV_ITEM_NUM
}HERO_NV_ITEM_E;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif

#endif
#endif/*_HERO_ID_H_*/

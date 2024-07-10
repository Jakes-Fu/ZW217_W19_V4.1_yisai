/*****************************************************************************
** File Name:      mmicc_id.h                                                *
** Author:                                                                   *
** Date:           2006/09/26                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe cc win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/26     kelly.li        Create                                    *
******************************************************************************/

#ifndef _MMIZDT_ID_H_
#define _MMIZDT_ID_H_ 

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
 
#define WIN_ID_DEF(win_id, win_id_name)          win_id,
 
// window ID
typedef enum
{
    MMI_ZDT_WIN_ID_START = (MMI_MODULE_ZDT << 16),

#include "zdt_id.def"

    MMIZDT_MAX_WIN_ID
}MMIZDT_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIZDT_CTRL_ID_START = MMIZDT_MAX_WIN_ID,//(MMI_MODULE_CC << 16),
    MMIZDT_TEST_MENU_CTRL_ID,
    MMIZDT_YX_MENU_CTRL_ID,
    MMIZDT_YX_ONOFF_CTRL_ID,
    MMIZDT_YX_CHAT_CTRL_ID,
    MMIZDT_YX_2VM_CTRL_ID,
    MMIZDT_YX_HELP_CTRL_ID,
    MMIZDT_TINY_CHAT_LIST_CTRL_ID,
    MMIZDT_2VM_ANIM_CTRL_ID,
    #ifdef ZDT_MESSAGE_DELETE_FEATRUE
     MESSAGE_LIST_OPT_MENU_CTRL_ID,
     MESSAGE_LIST_CTRL_ID,
    #endif
    MMIZDT_VOICE_POPMENU_CTRL_ID,
    MMIZDT_VOICE_WAIT_CTRL_ID,
#ifdef ZDT_PLAT_SHB_SUPPORT
	MMIZDT_SHB_MENU_CTRL_ID,
	MMIZDT_ABOUT_SHB_MENU_CTRL_ID,

	SHB_SERVER_TYPE_CTRL_ID,

	INTRODUCTION_INFO_CTRL_ID,
	
	USERGUIDE_INFO_CTRL_ID,
	HOTLINE_INFO_CTRL_ID,
	SHB_VERSION_INFO_CTRL_ID,
	ABOUTHELP_INFO_CTRL_ID,
	SHB_ALERT_INFO_CTRL_ID,
	SHB_BIND_INFO_CTRL_ID,
	SHB_TEST_1_CTRL_ID,
	SHB_TEST_2_CTRL_ID,
#endif
    MMIZDT_DIAL_EDIT_CTRL_ID,  
    MMIZDT_DIAL_LIST_CTRL_ID,
    MMIZDT_PB_LIST_CTRL_ID,
    MMIZDT_FINDWATCH_ANIM_CTRL_ID,
    MMIZDT_FACTORY_LIST_CTRL_ID,
    MMIZDT_MESSAGE_LIST_CTRL_ID,
    MMIZDT_MESSAGE_PREVIEW_ID,

    MMIZDT_WLDT_LIST_YES_NO_ID,
    MMIZDT_WLDT_LIST_SINGLE_ID,
    MMIZDT_WLDT_LIST_MULTI_ID,
#ifdef ZDT_CYHEALTH_SUPPORT
    MMIZDT_URL_HEALTH_IMAGE_CTRL_ID,
    MMIZDT_URL_NUCLEIC_IMAGE_CTRL_ID,
    MMIZDT_URL_ACC_IMAGE_CTRL_ID,
    
    MMIZDT_TYPE1_HEALTH_IMAGE_CTRL_ID,
    MMIZDT_TYPE1_NUCLEIC_IMAGE_CTRL_ID,
    MMIZDT_TYPE1_ACC_IMAGE_CTRL_ID,
#endif
	MMIZDT_SCHEDULE_CTRL_ID,//add_schedule

#if defined(ZDT_NO_SIM_ICON_GIF)// 未插SIM 卡提示图标用动画显示
    MMIZDT_INSERTSIM_ANIM_CTRL_ID,
#endif

    MMIZDT_MAX_CTRL_ID
}ZDT_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISET_ID_H_

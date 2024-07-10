/*****************************************************************************
** File Name:      mmidcd_id.h                                               *
** Author:         haiyang.hu                                                *
** Date:           2006/09/22                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ACC win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/22     haiayng.hu        Create                                   *
******************************************************************************/

#ifndef _MMIDCD_ID_H_
#define _MMIDCD_ID_H_ 

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
    MMI_DCD_WIN_ID_START = (MMI_MODULE_DCD << 16),


    #include "mmidcd_id.def"    

    MMIDCD_MAX_WIN_ID

}MMIDCD_WINDOW_ID_E;

#undef WIN_ID_DEF
 // window ID

// control ID
typedef enum
{
    MMIDCD_CTRL_ID_START = MMIDCD_MAX_WIN_ID,
//dcd
    MMIDCD_TAB_CTRL_ID,                  //dcd main tab ctrl
    
    MMIDCD_ITEM_LIST_CTRL_ID,     //dcd item listbox ctrl


	
    MMIDCD_ITEM_MENU_CTRL_ID,

    MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID,
    
    MMIDCD_ITEM_ORER_RICHTEXT_CTRL_ID,  //订购详情控件//@jun.hu, 2009/8/4
    
    MMIDCD_ITEM_HELP_RICHTEXT_CTRL_ID,  //帮助控件//@jun.hu, 2009/8/12
    
    MMIDCD_CONTENT_EMPTY_LABEL_CTRL_ID, //@jun.hu, 2009/8/19
    
    MMIDCD_IDLEWIN_TAB_CTRL_ID,   //dcd idle tab ctrl

    MMIDCD_IDLEWIN_ANIM_CTRL_ID,  //dcd idle animate ctrl
    
    MMIDCD_IDLEWIN_TEXT_CTRL_ID,  //dcd idle text ctrl

    MMIDCD_LOCAL_ITEM_LIST_CTRL_ID,  //dcd local listbox ctrl

    MMIDCD_LINKLIST_CTRL_ID,         // dcd linklist ctrl

   MMIDCD_DETAIL_CTRL_ID,    // DCD 记录详情ctrl

   MMIDCD_PREVIEW_MENU_CTRL_ID,

   MMIDCD_SETTING_MENU_CTRL_ID,  // DCD设置菜单控件

    MMIDCD_ANIMATE_UPDATING_CTRL_ID,  //DCD更新动画控件.......
   
    MMIDCD_LOCAL_ITEM_EXPAND_CTRL_ID,
    MMIDCD_LOCAL_ITEM_MENU_CTRL_ID,
    MMIDCD_PREVIEW_CONTENT_ANIMATE_CTRL_ID,
    MMIDCD_PREVIEW_TITLE_TEXTBOX_CTRL_ID,
    MMIDCD_PREVIEW_SUMMARY_TEXTBOX_CTRL_ID,
    MMIDCD_SETTING_LIST_CTRL_ID,
    MMIDCD_NETWORK_SETTING_CTRL_ID,
    
    MMIDCD_EDIT_HOMEPAGE_CTRL_ID,
    MMIDCD_EDIT_APN_CTRL_ID,
    MMIDCD_EDIT_USER_CTRL_ID,
    MMIDCD_EDIT_PASSWORD_CTRL_ID,
    MMIDCD_EDIT_PROXY_CTRL_ID,
    MMIDCD_EDIT_PORT_CTRL_ID,
    
    MMIDCD_LINK_LABEL_CTRL_ID,
    MMIDCD_ORDER_CTRL_ID,		
 
    MMIDCD_DL_CTRL_ID,	      
    MMIDCD_CONTENT_CTRL_ID,
    MMIDCD_CONNECT_CTRL_ID,	
    MMIDCD_TIME_CTRL_ID,

    MMIDCD_ABSTRACT_CTRL_ID,
	
    MMIDCD_MAX_CTRL_ID
    
}MMIDCD_CONTROL_ID_E;
/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DECLARE                          *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIDCD_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIDCD_ID_H_


/*****************************************************************************
** File Name:      mmiwidget_id.h                                               *
** Author:                                                 *
** Date:                                                         *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe mp3 win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**                     *
******************************************************************************/
#ifndef _MMIWIDGET_ID_H_
#define _MMIWIDGET_ID_H_ 

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
    MMIWIDGET_WIN_ID_START = (MMI_MODULE_WIDGET << 16),

#include "mmiwidget_id.def"

    MMIWIDGET_MAX_WIN_ID
}MMIWIDGET_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIWIDGET_CTRL_ID_ID_START = MMIWIDGET_MAX_WIN_ID, //(MMI_MODULE_MP3 << 16),

    MMIWIDGET_WALLPAPER_ANIM_CTRL_ID,
    MMIWIDGET_WALLPAPER_ANIM2_CTRL_ID,
    MMIWIDGET_WALLPAPER_ANIM3_CTRL_ID,
    MMIGRID_POP_MENU_ID,
    MMIGRID_IDLE_MENU_ID, 
    MMIQBTHEME_SHORTCUT_MENU_ID,
    MMIQBTHEME_WIDGET_MENU_ID,

    MMIWIDGET_PB_ANIM_CTRL_ID,
    MMIWIDGET_PB_ANIM2_CTRL_ID,
    MMIWIDGET_PB_ANIM3_CTRL_ID,
    MMIWIDGET_PB_ANIM4_CTRL_ID,
    MMIWIDGET_PB_ANIM5_CTRL_ID,
    MMIWIDGET_MEMO_EDITBOX_CTRL_ID, 
    MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID,//Jonathan
    MMIWIDGETPB_MAKECALL_OPT_CTRL_ID,
    MMIWIDGETPB_SENDMESSAGE_OPT_CTRL_ID,
    MMIWIDGETPB_FAVORITE_LIST_MENU_CTRL_ID,
    MMIWIDGETPB_FAVORITE_LIST_DETAIL_CTRL_ID,

    MMIWIDGET_ALARM_LIST_CTRL_ID,
    MMIWIDGET_DCD_TEXT_CTRL_ID,
    MMIWIDGET_DCD_ANIM_CTRL_ID,
    MMIWIDGET_DCD_TAB_ANIM_CTRL_ID,
    MMIWIDGET_DCD_TAB_ANIM1_CTRL_ID,
    MMIWIDGET_DCD_TAB_ANIM2_CTRL_ID,
    MMIWIDGET_DCD_TAB_ANIM3_CTRL_ID,    
    
#ifdef MMI_GRID_IDLE_SUPPORT
	MMIWIDGET_MEMO_FORM_CTRL_ID,
	MMIWIDGET_MEMO_SOFTKEY_CTRL_ID,
    MMIGRID_PB_ANIM0_CTRL_ID,
    MMIGRID_PB_ANIM1_CTRL_ID,    
    MMIGRID_PB_ANIM2_CTRL_ID,
    MMIGRID_PB_ANIM3_CTRL_ID,
    MMIGRID_PB_ANIM4_CTRL_ID,
    MMIGRID_PB_ANIM5_CTRL_ID,    
    MMIGRID_PB_ANIM6_CTRL_ID,
    MMIGRID_PB_ANIM7_CTRL_ID,
    MMIGRID_PB_ANIM8_CTRL_ID,
    MMIGRID_PB_ANIM9_CTRL_ID,

    MMIGRID_IMGVIEWER_ANIM0_ID,
    MMIGRID_IMGVIEWER_ANIM1_ID,
    MMIGRID_IMGVIEWER_ANIM2_ID,
    MMIGRID_IMGVIEWER_ANIM3_ID,
    MMIGRID_IMGVIEWER_ANIM4_ID,
    MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID,
    MMI_MP3_ANIM_CTRL_ID, 
#endif
    MMIWIDGET_SCHEDULE_LIST_CTRL_ID,
    MMIWIDGET_SCHEDULE_ANIM_CTRL_ID,

#ifdef SNS_SUPPORT
    MMIWIDGET_SNS_IMG_BG_CTRL_ID,
    MMIWIDGET_SNS_IMG_TETAIL1_CTRL_ID,
    MMIWIDGET_SNS_IMG_TETAIL2_CTRL_ID,
    MMIWIDGET_SNS_IMG_TETAIL3_CTRL_ID,
    MMIWIDGET_SNS_IMG_ADD_CTRL_ID,
#endif

#ifdef SNS_PULLING_SUPPORT
    MMIWIDGET_SNS_SETTING_CTRL_ID,
#endif //SNS_PULLING_SUPPORT

    MMIWIDGET_MAX_CTRL_ID
}MMIWIDGET_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIWIDGET_ID_H_

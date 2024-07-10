/*****************************************************************************
** File Name:      mmiqbtheme_id.h                                                   *
** Author:         cheng.luo                                                        *
** Date:           2011/09/17                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe QBTHEME win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011/09/17     cheng.luo      Create                                    *
******************************************************************************/

#ifndef _QBTHEME_ID_H_
#define _QBTHEME_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

#include "mmiqbtheme_internal.h"

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
    QBTHEME_WIN_ID_START = (MMI_MODULE_QBTHEME << 16),

#include "mmiqbtheme_id.def"

    QBTHEME_MAX_WIN_ID
}QBTHEME_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    QBTHEME_CTRL_ID_ID_START = QBTHEME_MAX_WIN_ID,
		
	QBTHEME_MAINWIN_FORM_CTRL_ID,
	QBTHEME_CHANGE_THEME_FORM_CTRL_ID,
	QBTHEME_CHANGE_THEME_LABEL_CTRL_ID,
	QBTHEME_CHANGE_THEME_CONTENT_FORM_CTRL_ID,
	QBTHEME_CHANGE_THEME_NAME_LABEL_CTRL_ID,
	QBTHEME_CHANGE_THEME_AUTHOR_LABEL_CTRL_ID,
	QBTHEME_OVERVIEW_ANIM_CTRL_ID,
	QBTHEME_CHANGE_STYLE_FORM_CTRL_ID,
	QBTHEME_CHANGE_STYLE_LABEL_CTRL_ID,
	QBTHEME_CHANGE_STYLE_WALLPAPER_FORM_CTRL_ID,
	QBTHEME_CHANGE_STYLE_WALLPAPER_LABEL1_CTRL_ID,
	QBTHEME_CHANGE_STYLE_WALLPAPER_LABEL2_CTRL_ID,
	QBTHEME_CHANGE_STYLE_ICON_FORM_CTRL_ID,
	QBTHEME_CHANGE_STYLE_ICON_LABEL1_CTRL_ID,
	QBTHEME_CHANGE_STYLE_ICON_LABEL2_CTRL_ID,
	QBTHEME_CHANGE_STYLE_ICONBG_FORM_CTRL_ID,
	QBTHEME_CHANGE_STYLE_ICONBG_LABEL1_CTRL_ID,
	QBTHEME_CHANGE_STYLE_ICONBG_LABEL2_CTRL_ID,
	QBTHEME_CHANGE_STYLE_FONT_FORM_CTRL_ID,
	QBTHEME_CHANGE_STYLE_FONT_LABEL1_CTRL_ID,
	QBTHEME_CHANGE_STYLE_FONT_LABEL2_CTRL_ID,

	QBTHEME_LISTWIN_LISTBOX_CTRL_ID,
    QBTHEME_LISTWIN_CHANGE_BTN_CTRL_ID,
    QBTHEME_LISTWIN_EXPLORER_BTN_CTRL_ID,
	
	QBTHEME_WALLPAPER_LISTWIN_LISTBOX_CTRL_ID,

	QBTHEME_ICON_LISTWIN_LISTBOX_CTRL_ID,

	QBTHEME_ICON_BG_LISTWIN_LISTBOX_CTRL_ID,

	QBTHEME_FONT_LISTWIN_LISTBOX_CTRL_ID,

	QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID,
	QBTHEME_WALLPAPER_PREVIEW2_ANIM_CTRL_ID,
	QBTHEME_WALLPAPER_PREVIEW3_ANIM_CTRL_ID,
	QBTHEME_WALLPAPER_PREVIEW4_ANIM_CTRL_ID,
	QBTHEME_WALLPAPER_PREVIEW_NAME_LABEL_CTRL_ID,
	QBTHEME_WALLPAPER_PREVIEW_SIZE_LABEL_CTRL_ID,
	QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID,
	QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID,
	QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID,
	QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID,
    QBTHEME_WALLPAPER_PREVIEW_RETURN_BTN_CTRL_ID,

	QBTHEME_PREVIEW1_ANIM_CTRL_ID,
	QBTHEME_PREVIEW2_ANIM_CTRL_ID,
	QBTHEME_PREVIEW3_ANIM_CTRL_ID,
	QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID,
	QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID,
	QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID,
	QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID,
	QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID,
	QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID,
    QBTHEME_PREVIEW_RETURN_BTN_CTRL_ID,

	QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID,
	QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID,
	QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID,
	QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID,
	QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID,
	QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID,
	QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID,
    QBTHEME_ICON_PREVIEW_RETURN_BTN_CTRL_ID,

	QBTHEME_ICON_BG_PREVIEW_NAME_LABEL_CTRL_ID,
	QBTHEME_ICON_BG_PREVIEW_SIZE_LABEL_CTRL_ID,
	QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID,
	QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID,
	QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID,
	QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID,
    QBTHEME_ICON_BG_PREVIEW_RETURN_BTN_CTRL_ID,

	QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID,
	QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID,
	QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID,
	QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID,
	QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID,
	QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID,
	QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID,
    QBTHEME_FONT_PREVIEW_RETURN_BTN_CTRL_ID,

	QBTHEME_MAX_CTRL_ID
}QBTHEME_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

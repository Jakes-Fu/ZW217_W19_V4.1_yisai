/*****************************************************************************
** File Name:      mmipic_id.h                                                *
** Author:                                                                   *
** Date:           2008/01/24                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe dc win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008/01/24     vincent        Create                                    *
******************************************************************************/
//macro replace
//#ifdef MMI_PIC_EDITOR_ENABLE
#ifdef PIC_EDITOR_SUPPORT
#ifndef _MMIPIC_ID_H_
#define _MMIPIC_ID_H_ 

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
    PIC_WIN_ID_START = (MMI_MODULE_PIC << 16),

#include "mmipic_id.def"
#ifdef PIC_EDIT_U200_SUPPORT
    WIN_ID_DEF(MMIPIC_EDIT_ADJUST_WIN_ID),
    WIN_ID_DEF(MMIPIC_CHAR_SIZE_WIN_ID),
#endif
    WIN_ID_DEF(MMIPIC_EDIT_SAVE_WIN_ID),

    PIC_MAX_WIN_ID
}PIC_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    PIC_CTRL_ID_ID_START = PIC_MAX_WIN_ID,//(MMI_MODULE_PIC << 16),
    MMIPIC_FUNC_MENU_CTRL_ID,
    MMIDC_MENU_CTRL_ID,
    MMIPIC_VIEWPIC_ICONLIST_CTRL_ID,
    MMIPIC_RESIZE_LISTBOX_CTRL_ID,

    MMIPIC_CHARACTER_NAME_FORM_CTRL_ID,
    MMIPIC_NAME_TITLE_LABEL_CTRL_ID,
    MMIPIC_NAME_LABEL_CTRL_ID,  
    MMIPIC_CHARACTER_SIZE_FORM_CTRL_ID,
    MMIPIC_SIZE_TITLE_LABEL_CTRL_ID,
    MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID,
    MMIPIC_CHARACTER_FONT_COLOR_FORM_CTRL_ID,
    MMIPIC_COLOR_TITLE_LABEL_CTRL_ID,
    MMIPIC_COLOR_LABEL_CTRL_ID,
    MMIPIC_CHARACTER_FRAME_COLOR_FORM_CTRL_ID,
    MMIPIC_FRAME_COLOR_TITLE_LABEL_CTRL_ID,
    MMIPIC_FRAME_COLOR_LABEL_CTRL_ID,
    MMIPIC_CHARACTER_DIRECTION_COLOR_FORM_CTRL_ID,
    MMIPIC_DIRECTION_TITLE_LABEL_CTRL_ID,
    MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID,

    MMIPIC_TEXTEDIT_CTRL_ID,
    MMIPIC_SAVE_AS_EDITBOX_CTRL_ID,
    MMIPIC_CHARACTER_FORM_CTRL1_ID,    

    MMIPIC_LEFT_ARROW_CTRL_ID,
    MMIPIC_RIGHT_ARROW_CTRL_ID,

#ifdef PIC_EDIT_U200_SUPPORT
    MMIPIC_MAIN_CANCEL_CTRL_ID,
    MMIPIC_MAIN_SAVE_CTRL_ID,
#endif
    MMIPIC_EDIT_MAINMENU_FORM_ID,
    MMIPIC_EDIT_MAINMENU_RAOTATE_ID,
    MMIPIC_EDIT_MAINMENU_ADJUST_ID,
    MMIPIC_EDIT_MAINMENU_EFFECT_ID,
#ifdef PIC_EDIT_FRAME_SUPPORT
    MMIPIC_EDIT_MAINMENU_FRAME_ID,
#endif
    MMIPIC_EDIT_MAINMENU_TEXT_ID,
    MMIPIC_EDIT_MAINMENU_CLIP_ID,
#ifdef PIC_EDIT_U200_SUPPORT        
    MMIPIC_CANCEL_CTRL_ID,
    MMIPIC_OK_CTRL_ID,
#endif
    MMIPIC_ADJUST_BRI_GROUP_ID,
    MMIPIC_ADJUST_BRI_FORM_ID,
    MMIPIC_ADJUST_BRI_BRI_BUTTON_ID,  
    MMIPIC_ADJUST_BRI_COL_BUTTON_ID,
    
    MMIPIC_ADJUST_COL_GROUP_ID,
    MMIPIC_ADJUST_COL_FORM_ID,
    MMIPIC_ADJUST_COL_BRI_BUTTON_ID,  
    MMIPIC_ADJUST_COL_COL_BUTTON_ID,  
       
    MMIPIC_EDIT_FRAME_ICONLIST_CTRL_ID,
    MMIPIC_EDIT_EFFECT_ICONLIST_CTRL_ID,
    MMIPIC_EDIT_CLIP_ICONLIST_CTRL_ID,
#ifdef PIC_EDIT_U200_SUPPORT        
    MMIPIC_EDIT_CANCEL_CTRL_ID,//for internal edit window
    MMIPIC_EDIT_OK_CTRL_ID,
#endif    
    MMIPIC_CHAR_PREVIEW_FORM_ID,
    MMIPIC_CHAR_COLOR_CTRL_ID,//"color"button in add_chara window 
    MMIPIC_CHAR_SIZE_CTRL_ID,//"size"button in add_chara window 
    
    MMIPIC_CHAR_SIZE_LIST_CTRL_ID,//list ctrl of pop window for size in add_chara window 
    MMIPIC_SAVE_EDIT_CTRL_ID,//edit ctrl for save window
    
    PIC_MAX_CTRL_ID
}PIC_CONTROL_ID_E;

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
#endif

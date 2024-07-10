/*****************************************************************************
** File Name:      mmiebook_id.h                                                *
** Author:                                                                   *
** Date:           2006/05/17                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe ebook win and control id    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/05/17     songbin.zeng      Create                                   *
******************************************************************************/
#ifdef EBOOK_SUPPORT
#ifndef _MMIEBOOK_ID_H_
#define _MMIEBOOK_ID_H_ 

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
    MMIEBOOK_WIN_ID_MIN = (MMI_MODULE_EBOOK << 16),

#include "mmiebook_id.def"

    MMIEBOOK_WIN_ID_MAX
}MMIEBOOK_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIEBOOK_CTRL_ID_MIN = MMIEBOOK_WIN_ID_MAX, //(MMI_MODULE_EBOOK << 16),
    MMIEBOOK_TXT_LIST_CTRL_ID,
    MMIEBOOK_TXT_LIST_OPT_CTRL_ID,

    MMIEBOOK_FORM_TXT_SHOW_CTRL_ID,
    MMIEBOOK_FORM_LABEL_BUTTON_CTRL_ID,

    MMIEBOOK_PERCENT_INPUT_CTRL_ID,         //跳转百分比输入界面
    MMIEBOOK_TXT_SHOW_CTRL_ID,
    MMIEBOOK_TXT_SHOW_OPT_CTRL_ID,
    MMIEBOOK_TXT_SHOW_OPT_MORE_MENU_CTRL_ID,
    
    MMIEBOOK_BOOKMARK_LIST_CTRL_ID,
    MMIEBOOK_BOOKMARK_LIST_OPT_CTRL_ID,
    MMIEBOOK_BOOKMARK_LIST_OPT_POPUP_CTRL_ID,
    
    MMIEBOOK_CUR_BOOKMARK_CTRL_ID,
    MMIEBOOK_TXT_SHOW_BOOKMARK_CTRL_ID,
    MMIEBOOK_TXT_RENAME_EDITBOX_CTRL_ID,//@maggie add for ebook
    MMIEBOOK_SET_CODINGTYPE_CTRL_ID,
    MMIEBOOK_FONT_SIZE_CTRL_ID,

    MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID,    
    MMIEBOOK_FORM_FONT_COLOR_CTRL_ID,    
    MMIEBOOK_FORM_PREVIEW_CTRL_ID,
    
    MMIEBOOK_FONT_COLOR_LABEL_CTRL_ID,    
    MMIEBOOK_BACK_COLOR_LABEL_CTRL_ID,
    MMIEBOOK_FONT_SIZE_LABEL_CTRL_ID,
    
    MMIEBOOK_PREVIEW_COLOR_LABEL_CTRL_ID,
    MMIEBOOK_PREVIEW_DISP_LABEL_CTRL_ID,

    MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL,
    MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL,    
    MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL,

    MMIEBOOK_BUTTON_COLOR1_FONT_CTRL,
    MMIEBOOK_BUTTON_COLOR2_FONT_CTRL,
    MMIEBOOK_BUTTON_COLOR3_FONT_CTRL,
    MMIEBOOK_BUTTON_COLOR4_FONT_CTRL,
    MMIEBOOK_BUTTON_COLOR5_FONT_CTRL,
    MMIEBOOK_BUTTON_COLOR6_FONT_CTRL,

    MMIEBOOK_BUTTON_COLOR1_BACK_CTRL,
    MMIEBOOK_BUTTON_COLOR2_BACK_CTRL,
    MMIEBOOK_BUTTON_COLOR3_BACK_CTRL,
    MMIEBOOK_BUTTON_COLOR4_BACK_CTRL,
    MMIEBOOK_BUTTON_COLOR5_BACK_CTRL,
    MMIEBOOK_BUTTON_COLOR6_BACK_CTRL,

    MMIEBOOK_BUTTON_FONT_SIZE1_CTRL,
    MMIEBOOK_BUTTON_FONT_SIZE2_CTRL,
    MMIEBOOK_BUTTON_FONT_SIZE3_CTRL,    
    
    MMIEBOOK_LATEST_LIST_CTRL_ID,       
    MMIEBOOK_LATEST_LIST_OPT_CTRL_ID,    
#ifdef TTS_SUPPORT
    MMIEBOOK_SET_TTS_CTRL_ID,
#endif
	MMIEBOOK_SET_AUTO_READ_CTRL_ID,
    MMIEBOOK_SET_FONT_SIZE_CTRL_ID,
    MMIEBOOK_OSD_TIME_CTRL_ID,
    MMIEBOOK_OSD_NAME_CTRL_ID,
    MMIEBOOK_OSD_PERCENT_CTRL_ID,
    MMIEBOOK_OSD_FONTCOLOR_CTRL_ID,
    MMIEBOOK_OSD_FONTSIZE_CTRL_ID,
    MMIEBOOK_OSD_BG_CTRL_ID,    
    MMIEBOOK_OSD_DISPLAY_SET_CTRL_ID,
    MMIEBOOK_OSD_MORE_CTRL_ID,  
    MMIEBOOK_OSD_LSK_CTRL_ID,
    MMIEBOOK_OSD_RSK_CTRL_ID,
    MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2,
    MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2,
    MMIEBOOK_FORM_BACK_COLOR_CTRL_ID,
    MMIEBOOK_FORM_PERCENT_INPUT_CTRL_ID,
    MMIEBOOK_RETURN_BUTTON_CTRL_ID,
    MMIEBOOK_MENU_BUTTON_CTRL_ID,
    MMIEBOOK_DAY_NIGHT_MODE_CTRL_ID,

    MMIEBOOK_CTRL_ID_MAX
}MMIEBOOK_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif//ENG_SUPPORT

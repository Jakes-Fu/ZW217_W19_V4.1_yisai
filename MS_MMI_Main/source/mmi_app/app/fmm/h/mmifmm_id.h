/*****************************************************************************
** File Name:      mmifmm_id.h                                               *
** Author:         robert.lu                                                *
** Date:           2007/04/06                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe fmm win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/04/06  robert.lu        Create                                   *
******************************************************************************/

#ifndef _MMIFMM_ID_H_
#define _MMIFMM_ID_H_ 

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
    MMIFMM_WIN_ID_START = (MMI_MODULE_FMM << 16),

#include "mmifmm_id.def"

    MMIFMM_MAX_WIN_ID
}MMIFMM_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIFMM_CTRL_ID_ID_START = MMIFMM_MAX_WIN_ID, //(MMI_MODULE_FMM << 16),
//    MMIFMM_LISTBOX_CTRL_ID,        //listbox control id
    MMIFMM_UDISK_LABEL_CTRL_ID,
    //MMIFMM_SDCARD_LABEL_CTRL_ID,
    MMIFMM_NORMAL_OPT_MENU_CTRL_ID,
    MMIFMM_PIC_OPT_MENU_CTRL_ID,
    MMIFMM_MUSIC_OPT_MENU_CTRL_ID,
    MMIFMM_MOVIE_OPT_MENU_CTRL_ID,
	MMIFMM_VCARD_OPT_MENU_CTRL_ID,
	MMIFMM_OTHER_OPT_MENU_CTRL_ID,
    MMIFMM_PIC_PREVIEW_CTRL_ID,
    MMIFMM_PIC_GLIDE1_CTRL_ID,
    MMIFMM_PIC_GLIDE2_CTRL_ID,
    MMIFMM_PIC_GLIDE3_CTRL_ID,
    MMIFMM_PREVIEW_SETLIST_CTRL_ID,
    MMIFMM_SEL_PICTURE_PREVIEW_CTRL_ID,
    MMIFMM_SEL_PICTURE_PREVIEW_BUTTON_CTRL_ID,
    MMIFMM_PIC_PREVIEW_OPT_MENU_CTRL_ID,
    MMIFMM_SEND_OPT_MENU_CTRL_ID,
    MMIFMM_SEND_VCARD_OPT_MENU_CTRL_ID,
    MMIFMM_SORT_OPT_MENU_CTRL_ID,
    MMIFMM_EMPTY_FOLDER_MENU_CTRL_ID,
    MMIFMM_PATH_UDISK_LABEL_CTRL_ID,
    //MMIFMM_PATH_SDCARD_LABEL_CTRL_ID,
    MMIFMM_PATH_LISTBOX_CTRL_ID,
    MMIFMM_FIND_FILE_EDITBOX_CTRL_ID,
    MMIFMM_CREATE_FOLDER_EDITBOX_CTRL_ID,
    MMIFMM_RENAME_EDITBOX_CTRL_ID,

	MMIFMM_FILE_DETAIL_TEXT_CTRL_ID,
    MMIFMM_MEM_DETAIL_TEXT_CTRL_ID,
    MMIFMM_GETFILELISTBOX_CTRL_ID,
	MMIFMM_SELECT_MUSIC_LISTBOX_CTRL_ID,
	MMIFMM_SELECT_MOV_LISTBOX_CTRL_ID,
	MMIFMM_SELECT_MOV_OPT_MENU_CTRL_ID,

	MMIFMM_OPEN_FILE_LIST_CTRL_ID,
	MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID,
	MMIFMM_OPEN_FILE_BYLAYER_FORM_CTRL_ID,
	MMIFMM_OPEN_FILE_BYLAYER_TITLE_FORM_CTRL_ID,
	MMIFMM_OPEN_FILE_BYLAYER_TITLE_LABEL_CTRL_ID,
	MMIFMM_OPEN_FILE_BYLAYER_TITLE_BUTTON_CTRL_ID,
	
	MMIFMM_TAB_CTRL_ID,
	//MMIFMM_SD_LISTBOX_CTRL_ID,
    MMIFMM_UDISK_LISTBOX_CTRL_ID,
	MMIFMM_PATH_TAB_CTRL_ID,
	//MMIFMM_PATH_SD_LISTBOX_CTRL_ID,
	MMIFMM_PATH_UDISK_LISTBOX_CTRL_ID,
	#ifdef  JAVA_SUPPORT_IA
    MMIFMM_JAVA_SELECT_WIN_CTRL_ID,
    #endif
    MMIFMM_VCALENDAR_OPT_MENU_CTRL_ID,
    //MMIFMM_PATH_SD1_LISTBOX_CTRL_ID,
#ifdef DRM_SUPPORT
    MMIFMM_DRM_DOWNLOAD_OPT_MENU_CTRL_ID,
#endif
//#ifdef MMI_PDA_SUPPORT
    MMIFMM_FOLDER_OPT_CTRL_ID,
    MMIFMM_RENAME_FORM_CTRL_ID,
    MMIFMM_RENAME_SOFTKEY_CTRL_ID,
    MMIFMM_PICTURE_OPT_CTRL_ID,
    MMIFMM_MUSIC_OPT_CTRL_ID,
    MMIFMM_MOVIE_OPT_CTRL_ID,
    MMIFMM_VCARD_OPT_CTRL_ID,
    MMIFMM_VCALENDAR_OPT_CTRL_ID,
    MMIFMM_OTHER_OPT_CTRL_ID,
    MMIFMM_MARKED_OPT_CTRL_ID,
    MMIFMM_FILE_OPT_CTRL_ID,
    MMIFMM_SELECT_SOFTKEY_CTRL_ID,
    MMIFMM_FIND_FORM_CTRL_ID,
    MMIFMM_FIND_SOFTKEY_CTRL_ID,
    MMIFMM_CREATE_FOLDER_FORM_CTRL_ID,
    MMIFMM_CREATE_FOLDER_SOFTKEY_CTRL_ID,
    MMIFMM_UDISK_FORM_CTRL_ID,
    MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID,
    MMIFMM_UDISK_TITLE_LABEL_CTRL_ID,
    MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID,
    MMIFMM_MARK_OPT_MENU_CTRL_ID,
    MMIFMM_OPTION_BUTTON_CTRL_ID, 
    MMIFMM_RETURN_BUTTON_CTRL_ID,
//#endif
    MMIFMM_LAYER_TAB_CTRL_ID,
    MMIFMM_MEMCARD_RENAME_FORM_CTRL_ID,
    MMIFMM_MEMCARD_RENAME_SOFTKEY_CTRL_ID,
    MMIFMM_MEMCARD_RENAME_EDITBOX_CTRL_ID,
    MMIFMM_MAX_CTRL_ID
}MMIFMM_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIFMM_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIMP3_ID_H_

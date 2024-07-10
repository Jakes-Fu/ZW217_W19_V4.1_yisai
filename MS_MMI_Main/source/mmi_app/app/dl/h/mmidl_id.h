/***************************************************************************
** File Name:      mmidl_id.h                                             *
** Author:                                                                 *
** Date:           28/07/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to download id define        *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2009        wancan.you    Create
****************************************************************************/
#ifndef  _MMIDL_ID_H_    
#define  _MMIDL_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
 
// Download Window ID
typedef enum
{
    MMIDL_WIN_ID_START = (MMI_MODULE_DL << 16),

#include "mmidl_id.def"

    MMIDL_MAX_WIN_ID
}MMIDL_WINDOW_ID_E;

#undef WIN_ID_DEF

// Download Control ID
typedef enum
{
    MMIDL_CTRL_ID_START = MMIDL_MAX_WIN_ID,
    MMIDL_DOWNLOAD_AUTH_ACCOUNT_CTRL_ID,
    MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_CTRL_ID,
    MMIDL_DOWNLOAD_AUTH_PASSWORD_CTRL_ID,
    MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_CTRL_ID,
    MMIDL_DOWNLOAD_AUTH_INPUT_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_FILE_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_FILE_NAME_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_PATH_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_PATH_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_PATH_VALUE_CTRL_ID,
    MMIDL_DOWNLOAD_INPUT_FILENAME_CTRL_ID,
    MMIDL_DOWNLOAD_MANAGER_LIST_CTRL_ID,
    MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID,
    MMIDL_DOWNLOAD_TASK_DETAIL_CTRL_ID,
    MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID,

    MMIDL_DOWNLOAD_SELECT_DEVICE_LIST_CTRL_ID,

#ifdef MMI_GUI_STYLE_TYPICAL
    MMIDL_DOWNLOAD_SETTINGS_METHOD_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SETTINGS_METHOD_LABEL_CTRL_ID,
    MMIDL_DOWNLOAD_SETTINGS_METHOD_LIST_CTRL_ID,
    MMIDL_DOWNLOAD_SETTINGS_PATH_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SETTINGS_PATH_LABEL_CTRL_ID,
    MMIDL_DOWNLOAD_SETTINGS_PATH_LIST_CTRL_ID,
#else
    MMIDL_DOWNLOAD_SETTINGS_LIST_CTRL_ID,
    MMIDL_DOWNLOAD_SETTINGS_METHOD_MENU_CTRL_ID,
    MMIDL_DOWNLOAD_SETTINGS_PATH_MENU_CTRL_ID,

#endif
    MMIDL_DOWNLOAD_SAVE_PICTURE_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_MUSIC_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_VIDEO_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_OTHERS_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_AS_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_AS_FILE_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_VALUE_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_AS_PATH_FORM_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_AS_PATH_CTRL_ID,
    MMIDL_DOWNLOAD_SAVE_AS_PATH_VALUE_CTRL_ID,
    MMIDL_DD_PROPERTY_RICHTEXT_CTRL_ID,

    MMIDL_MAX_CTRL_ID         // 控件ID的总数目

}MMIDL_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif


